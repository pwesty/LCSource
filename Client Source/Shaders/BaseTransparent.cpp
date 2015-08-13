#include "StdH.h"
#include <Shaders/Common.h>

static const INDEX ctTextures = 1;
static const INDEX ctUVMaps   = 1;
static const INDEX ctColors   = 1;
static const INDEX ctFloats   = 0;
static const INDEX ctFlags    = 2;
static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms = 2;

static const INDEX iBaseTexture = 0;
static const INDEX iBaseUVMap   = 0;
static const INDEX iBaseColor   = 0;
static const INDEX iBaseVP = 0;
static const INDEX iTransparentPP = 0;
static const INDEX iTranslucentPP = 1;
/*
static const char strTransparentVertexCode[] =
CALCULATE_SHADING_VP
STANDARD_OUTPUT_VP
;

  static const char strTransparentPixelCode[] = 
  "tex    t0                \n"
  "mul    t0,      t0,   c0 \n"
  "mul_x2 r0.rgb,  t0,   v0 \n"
  "+mul   r0.a,    t0,   v0 \n"
  ;
*/
SHADER_MAIN(Base_Transparent)
{
	// Set texture wrapping
	shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
	// Set depth test
	shaEnableDepthTest();
	shaEnableDepthWrite();
	shaDepthFunc(GFX_LESS_EQUAL);
	
	shaSetTexture(iBaseTexture,0);
	shaSetUVMap(iBaseUVMap);
	shaSetColor(iBaseColor);
	
	const COLOR colModelColor = MulColors(shaGetModelColor(),shaGetColor(iBaseColor));
	const BOOL bOpaque = (colModelColor&0xFF)==0xFF;
	const BOOL bDoubleSided = shaGetFlags()&BASE_DOUBLE_SIDED;
	const BOOL bHardwareShader = shaUseHWShaders();
	
	if(bOpaque) 
	{
		// Set blend mode
		shaDisableBlend();
		// Set alpha test
		shaEnableAlphaTest();
	}
	else 
	{
		if(bHardwareShader) 
		{
			shaBlendFunc(GFX_ONE, GFX_SRC_ALPHA);
		}
		else 
		{
			// Set blend mode
			shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
		}
		shaEnableBlend();
		shaDisableDepthWrite();
		// Set alpha test
		shaDisableAlphaTest();
	}
	
	if(bHardwareShader) 
	{
		GFXColor colBase;
		colBase.MultiplyRGBA( (GFXColor)shaGetModelColor(), (GFXColor)shaGetCurrentColor());
		const ShaderRegister srBaseColor(colBase);
		
		shaSetFogTextureUnit(1,bOpaque);
		shaSetVertexProgram(iBaseVP);
		if(bOpaque) 
		{
			shaSetPixelProgram(iTransparentPP);
		}
		else 
		{
			shaSetPixelProgram(iTranslucentPP);
		}
		shaSetPixelProgramConst( 0, &srBaseColor, 1);
		// prepare fog and haze
		shaPrepareFogAndHaze(bOpaque);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		shaSetDefaultConstantRegisters();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	}
	else 
	{
		// enable overbrightning
		shaEnableOverBrightning();
		// calculate shading
		shaCalculateLight();
		// prepare fog and haze
		shaPrepareFogAndHaze(bOpaque);
	}
	
	if(bOpaque) 
	{
		// if double sided flag is on
		if(bDoubleSided) 
		{
			// no face culling
			shaCullFace(GFX_NONE);
		}
		else 
		{
			// back face culling
			shaCullFace(GFX_BACK);
		}
		// Render surface
		shaRender();
		
		// disable overbrighning
		shaDisableOverBrightning();
		if(!bHardwareShader) 
		{
			shaDepthFunc(GFX_EQUAL);
			// render fog and haze pass for surface
			shaDoFogAndHazePass();
			shaDepthFunc(GFX_LESS_EQUAL);
		}
	}
	else 
	{
		
		if(!bHardwareShader) 
		{
			// adjust alpha for fog and haze
			shaAttenuateAlphaForFogAndHaze();
		}
		
		// if double sided flag is on
		if(bDoubleSided) 
		{
			// Render back side
			shaCullFace(GFX_FRONT);
			shaRender();
		}
		// Render front side
		shaCullFace(GFX_BACK);
		shaRender();
		
		if(!bHardwareShader) 
		{
			// disable overbrighning
			shaDisableOverBrightning();
		}
	}
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(Base_Transparent,ShaderDesc *&pshDesc)
{
	static bool bInit = false;
	static ShaderDesc shDescMe;
	if(!bInit)
	{
		bInit = true;
		shDescMe.sd_astrTextureNames.New(ctTextures);
		shDescMe.sd_astrTexCoordNames.New(ctUVMaps);
		shDescMe.sd_astrColorNames.New(ctColors);
		shDescMe.sd_astrFloatNames.New(ctFloats);
		shDescMe.sd_astrFlagNames.New(ctFlags);
		
		shDescMe.sd_astrTextureNames[0] = "Base texture";
		shDescMe.sd_astrTexCoordNames[0] = "Base uvmap";
		shDescMe.sd_astrColorNames[0] = "Base color";
		shDescMe.sd_astrFlagNames[0] = "Double sided";
		shDescMe.sd_astrFlagNames[1] = "Full bright";
		shDescMe.sd_strShaderInfo = "Basic shader";
		
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_NORMAL_STREAM;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(Base_Transparent, CTString &strVPCode, INDEX iVertexProgram)
{
	ASSERT(iVertexProgram==iBaseVP);
	strVPCode = CALCULATE_SHADING_VP
		STANDARD_OUTPUT_VP
		;
}

SHADER_PCODE(Base_Transparent, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	// Transparent - Fc=Bc, Fa=Ba
	// Translucent - Fc=Bc*Ba, Fa=(1-Ba)
	
	// if not fog
	if(eFogType==FT_NONE) 
	{
		// Transparent pixel program
		if(iPixelProgram==iTransparentPP) 
		{
/*			strPPCode = "tex    t0                     \n" // load base texture
						"mul    t0,      t0,   c0      \n" // mul base texture with base color
						"mul_x2 r0.rgb,  t0,   v0      \n" // Shade pixel
						"+mov   r0.a,    t0            \n" // Fa = Ba
						;
			// Translucent pixel program*/
			strPPCode = "texld	r0,    t0                     \n" // load base texture
						"mul    r1,      r0,   c0      \n" // mul base texture with base color
						"mul_x2 r0.rgb,  r1,   v0      \n" // Shade pixel
						"+mov   r0.a,    r1            \n" // Fa = Ba
						;
			// Translucent pixel program
		}
		else if(iPixelProgram==iTranslucentPP) 
		{
/*			strPPCode = "tex    t0                     \n" // load base texture
						"mul    t0,     t0,    c0      \n" // mul base texture with base color
						"mul    r0.rgb, t0,    t0.a    \n" // Fc = Bc*Ba
						"mul_x2 r0.rgb, r0,    v0      \n" // Shade pixel
						"+mov   r0.a,   1-t0.a         \n" // Fa = (1-Ba)
						;*/
			strPPCode = "texld	r0,    t0                     \n" // load base texture
						"mul    r1,     r0,    c0      \n" // mul base texture with base color
						"mul    r0.rgb, r1,    r1.a    \n" // Fc = Bc*Ba
						"mul_x2 r0.rgb, r0,    v0      \n" // Shade pixel
						"+mov   r0.a,   1-r1.a         \n" // Fa = (1-Ba)
						;
		}
		else 
		{
			ASSERT(FALSE);
		}
		// Fog on transparent pixel program
	}
	else if(eFogType==FT_OPAQUE) 
	{
		ASSERT(iPixelProgram==iTransparentPP);
/*		strPPCode = "tex    t0                     \n" // load base texture
					"tex    t1                     \n" // load fog texture
					"mul    t0,     t0,    c0      \n" // mul base texture with base color
					"mul    t1,     t1,    c7      \n" // mul fog texture with fog color
					"mul_x2 r0.rgb, t0,    v0      \n" // Shade pixel
					"+mov   r0.a,   t0             \n" // Fa = Ba
					"lrp    r0.rgb, t1.a,  t1,  r0 \n" // Add fog
					;
		// Fog on translucent pixel program*/
		strPPCode = "texld	r0,    t0                     \n" // load base texture
					"texld	r1,    t1                     \n" // load fog texture
					"mul    r2,     r0,    c0      \n" // mul base texture with base color
					"mul    r1,     r1,    c7      \n" // mul fog texture with fog color
					"mul_x2 r0.rgb, r2,    v0      \n" // Shade pixel
					"+mov   r0.a,   r2             \n" // Fa = Ba
					"lrp    r0.rgb, r1.a,  r1,  r0 \n" // Add fog
					;
		// Fog on translucent pixel program
	}
	else if(eFogType==FT_NON_OPAQUE) 
	{
		ASSERT(iPixelProgram==iTranslucentPP);
/*		strPPCode = "tex    t0                     \n" // load base texture
					"tex    t1                     \n" // load fog texture
					"mul    t0,     t0,    c0      \n" // mul base texture with base color
					"mul    t1,     t1,    c7      \n" // mul fog texture with fog color
					"lrp    r0.rgb, t1.a,  t1,  r0 \n" // Add fog
//					"+mul   t0.a,   t0.a,  1-t1.a  \n" // attenuate base tex with fog alpha
					"mul    r0.rgb, t0,    t0.a    \n" // Fc = Bc*Ba
					"mul_x2 r0.rgb, r0,    v0      \n" // Shade pixel
					"+mov   r0.a,   1-t0.a         \n" // Fa = (1-Ba)
					;*/
		strPPCode = "texld	r0,    t0                     \n" // load base texture
					"texld	r1,    t1                     \n" // load fog texture
					"mul    r2,     r0,    c0      \n" // mul base texture with base color
					"mul    r1,     r1,    c7      \n" // mul fog texture with fog color
					"lrp    r0.rgb, r1.a,  r1,  r0 \n" // Add fog
//					"+mul   t0.a,   t0.a,  1-t1.a  \n" // attenuate base tex with fog alpha
					"mul    r0.rgb, r2,    r2.a    \n" // Fc = Bc*Ba
					"mul_x2 r0.rgb, r0,    v0      \n" // Shade pixel
					"+mov   r0.a,   1-r2.a         \n" // Fa = (1-Ba)
					;
	}
}
