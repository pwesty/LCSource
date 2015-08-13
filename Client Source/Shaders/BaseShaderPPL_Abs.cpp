#include "StdH.h"
#include <Shaders/Common.h>

static const INDEX ctTextures = 2;
static const INDEX ctUVMaps   = 2;
static const INDEX ctColors   = 1;
static const INDEX ctFloats   = 0;
static const INDEX ctFlags    = 2;
static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms = 1;

static const INDEX iBaseTexture   = 0;
static const INDEX iNormalTexture = 1;
static const INDEX iBaseUVMap     = 0;
static const INDEX iNormalUVMap   = 1;
static const INDEX iBaseColor     = 0;
static const INDEX iBaseFloat     = 0;
static const INDEX iBaseVP = 0;
static const INDEX iBasePP = 0;

SHADER_MAIN(BasePPL_Abs)
{
	// Set texture wrapping
	shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
	// Set depth test
	shaEnableDepthTest();
	shaDepthFunc(GFX_LESS_EQUAL); 
	// Set alpha test
	shaDisableAlphaTest();
	
	shaSetTexture(iBaseTexture,0);
	shaSetUVMap(iBaseUVMap,0);
	shaSetColor(iBaseColor);
	
	const COLOR colModelColor = MulColors(shaGetModelColor(),shaGetCurrentColor());
	const BOOL bDoubleSided = shaGetFlags()&BASE_DOUBLE_SIDED;
	const BOOL bOpaque = (colModelColor&0xFF)==0xFF;
	const BOOL bHardwareShader = shaUseHWShaders();
	
	if(bHardwareShader) 
	{
		GFXColor colBase;
		colBase.MultiplyRGBA( (GFXColor)shaGetModelColor(), (GFXColor)shaGetCurrentColor());
		
		// Set light vector
		const FLOAT3D vLightDir = shaGetLightDirection();
		const GFXColor colLight = (GFXColor)shaGetLightColor();
		const GFXColor colAmbient = (GFXColor)shaGetAmbientColor();
		
		const ShaderRegister srBaseColor(colBase);
		const ShaderRegister srLightColor(colLight);
		const ShaderRegister srAmbientColor(colAmbient);
		const ShaderRegister srLightDir(vLightDir(1), vLightDir(2), vLightDir(3));
		
		shaSetTexture(iNormalTexture,1);
		shaSetUVMap(iNormalUVMap,1);
		
		shaSetFogTextureUnit(1, bOpaque);
		shaSetVertexProgram(iBaseVP);
		shaSetPixelProgram(iBasePP);
		// Set base color
		shaSetPixelProgramConst( 0, &srBaseColor,    1);
		shaSetPixelProgramConst( 1, &srLightDir,     1);
		shaSetPixelProgramConst( 2, &srLightColor,   1);
		shaSetPixelProgramConst( 3, &srAmbientColor, 1);
		
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
	
	// if surface is opaque
	if(bOpaque) 
	{
		// disable blend
		shaDisableBlend();
		// enable depth write
		shaEnableDepthWrite();
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
	}
	else 
	{ // else surface is translucent
		// Set blend mode
		shaEnableBlend();
		shaDisableDepthWrite();
		if(bHardwareShader) 
		{
			shaBlendFunc(GFX_ONE, GFX_SRC_ALPHA);
		}
		else 
		{
			// Adjust alpha for fog
			shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
			shaAttenuateAlphaForFogAndHaze();
		}
		
		// if double sided
		if(bDoubleSided) 
		{
			// render back side
			shaCullFace(GFX_FRONT);
			shaRender();
		}
		// back face culling
		shaCullFace(GFX_BACK);
	}
	// render front side
	shaRender();
	
	if(!bHardwareShader) 
	{
		// disable overbrighning
		shaDisableOverBrightning();
		// if surface is opaque
		if(bOpaque) 
		{
			// render fog and haze pass for surface
			shaDoFogAndHazePass();
		}
	}
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(BasePPL_Abs,ShaderDesc *&pshDesc)
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
		shDescMe.sd_astrTextureNames[1] = "Normal texture";
		shDescMe.sd_astrTexCoordNames[0] = "Base uvmap";
		shDescMe.sd_astrTexCoordNames[1] = "Normal uvmap";
		shDescMe.sd_astrColorNames[0] = "Base color";
		shDescMe.sd_astrFlagNames[0] = "Double sided";
		shDescMe.sd_astrFlagNames[1] = "Full bright";
		shDescMe.sd_strShaderInfo = "Basic shader";
		
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_TEXCOORD1|GFX_NORMAL_STREAM;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(BasePPL_Abs, CTString &strVPCode, INDEX iVertexProgram)
{
	ASSERT(iVertexProgram==iBaseVP);
	strVPCode = STANDARD_OUTPUT_VP
				"mov     oT1,     v6              \n"
				"mov     r11,     c4              \n"
				"add     r10.xyz, c7.yyy, r11.xyz \n" // 1.0f + LightDir
				"mul     oT3.xyz, r10.xyz, c7.www \n" // 0.5f * (1.0f + LightDir) // [0.0f - 0.1f];
				;
}

// Fc = (Bc*Ba)
// Fa = (1-Ba)
SHADER_PCODE(BasePPL_Abs, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	ASSERT(iPixelProgram==iBasePP);
	if(eFogType==FT_NONE) 
	{
/*		strPPCode = "tex      t0                     \n" // load base texture
					"tex      t1                     \n" // load normal texture
					"texcoord t3                     \n"
					"mul      t0,     t0,     c0     \n" // mul base texture with base color
					"mul      r0.rgb, t0,     t0.a   \n" // Fc=Bc*Ba
					// "dp3_sat  r1,     t1_bx2, c1     \n" // shade pixel
					"dp3_sat  r1,     t1_bx2, t3_bx2 \n" // shade pixel
					"mad      r1,     r1,     c2, c3 \n"
					"mul_x2   r0.rgb, r0,     r1     \n" 
					"+mov     r0.a,   1-t0.a         \n" // Fa = (1-Ba)
					;                             */
		strPPCode = "texld	r0,      t0                     \n" // load base texture
					"texld	r1,      t1                     \n" // load normal texture
					"texcoord t3                     \n"
					"mov	r2,		t3				\n"
					"mul      r3,     r0,     c0     \n" // mul base texture with base color
					"mul      r0.rgb, r3,     r3.a   \n" // Fc=Bc*Ba
					// "dp3_sat  r1,     t1_bx2, c1     \n" // shade pixel
					"dp3_sat  r1,     r1_bx2, r2_bx2 \n" // shade pixel
					"mad      r1,     r1,     c2, c3 \n"
					"mul_x2   r0.rgb, r0,     r1     \n" 
					"+mov     r0.a,   1-r3.a         \n" // Fa = (1-Ba)
					;
	}
	else if(eFogType==FT_OPAQUE) 
	{
/*		strPPCode = "tex    t0                     \n" // load base texture
					"tex    t1                     \n" // load fog texture
					"mul    t0,     t0,    c0      \n" // mul base texture with base color
					"mul    t1,     t1,    c7      \n" // mul fog texture with fog color
					"mul    r0.rgb, t0,    t0.a    \n" // Fc = Bc*Ba
					"mul_x2 r0.rgb, r0,    v0      \n" // Shade pixel
					"+mov   r0.a,   1-t0.a         \n" // Fa = (1-Ba)
					"lrp    r0.rgb, t1.a,  t1,  r0 \n" // Add fog
					;*/
		strPPCode = "texld	r0,    t0                     \n" // load base texture
					"texld	r1,    t1                     \n" // load fog texture
					"mul    r2,     r0,    c0      \n" // mul base texture with base color
					"mul    r1,     r1,    c7      \n" // mul fog texture with fog color
					"mul    r0.rgb, r2,    r2.a    \n" // Fc = Bc*Ba
					"mul_x2 r0.rgb, r0,    v0      \n" // Shade pixel
					"+mov   r0.a,   1-r2.a         \n" // Fa = (1-Ba)
					"lrp    r0.rgb, r1.a,  r1,  r0 \n" // Add fog
					;
	}
	else if(eFogType==FT_NON_OPAQUE) 
	{
/*		strPPCode = "tex    t0                     \n" // load base texture
					"tex    t1                     \n" // load fog texture
					"mul    t0,     t0,    c0      \n" // mul base texture with base color
					"mul    t1,     t1,    c7      \n" // mul fog texture with fog color
					"mul    t0.a,   t0,    1-t1.a  \n" // attenuate base tex alpha with fog alpha
					"mul    r0.rgb, t0,    t0.a    \n" // Fc = Bc*Ba
					"mul_x2 r0.rgb, r0,    v0      \n" // Shade pixel
					"+mov   r0.a,   1-t0.a         \n" // Fa = (1-Ba)
					;*/
		strPPCode = "texld	r0,    t0                     \n" // load base texture
					"texld	r1,    t1                     \n" // load fog texture
					"mul    r2,     r0,    c0      \n" // mul base texture with base color
					"mul	r1,     r1,    c7      \n" // mul fog texture with fog color
					"mul    r2.a,   r2,    1-r1.a  \n" // attenuate base tex alpha with fog alpha
					"mul    r0.rgb, r2,    r2.a    \n" // Fc = Bc*Ba
					"mul_x2 r0.rgb, r0,    v0      \n" // Shade pixel
					"+mov   r0.a,   1-r2.a         \n" // Fa = (1-Ba)
					;
	}
}
