#include "StdH.h"
#include <Shaders/Common.h>

static const INDEX ctTextures = 2;
static const INDEX ctUVMaps   = 1;
static const INDEX ctColors   = 2;
static const INDEX ctFloats   = 0;
static const INDEX ctFlags    = 2;
static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms = 1;

static const INDEX iBaseTexture = 0;
static const INDEX iBaseUVMap   = 0;
static const INDEX iBaseColor   = 0;
static const INDEX iBaseVP = 0;
static const INDEX iBasePP = 0;

static const INDEX iSpecularTexture = 1;
static const INDEX iSpecularColor   = 1;

SHADER_MAIN(Specular)
{
	// Set texture wrapping
	shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
	// Set depth test
	shaEnableDepthTest();
	shaDepthFunc(GFX_LESS_EQUAL);
	// Set alpha test
	shaDisableAlphaTest();
	
	shaSetTexture(iBaseTexture,0);
	shaSetUVMap(iBaseUVMap);
	shaSetColor(iBaseColor);
	
	const COLOR colModelColor = MulColors(shaGetModelColor(),shaGetCurrentColor());
	const BOOL bDoubleSided = shaGetFlags()&BASE_DOUBLE_SIDED;
	const BOOL bOpaque = (colModelColor&0xFF)==0xFF;
	const BOOL bHardwareShader = shaUseHWShaders();
	
	D3D_ENABLE_SPECULAR();
	
	if(bHardwareShader) 
	{
		GFXColor colBase;
		GFXColor colSpec;
		colBase.MultiplyRGBA((GFXColor)shaGetModelColor(),(GFXColor)shaGetCurrentColor());
		
		// cache light intensities
		const COLOR colLight = shaGetLightColor();
		const SLONG slLR = (colLight & CT_RMASK)>>(CT_RSHIFT);
		const SLONG slLG = (colLight & CT_GMASK)>>(CT_GSHIFT);
		const SLONG slLB = (colLight & CT_BMASK)>>(CT_BSHIFT);
		colSpec = shaGetColor(iSpecularColor);
		colSpec.AttenuateRGB( (shaGetModelColor()&CT_AMASK)>>CT_ASHIFT);
		colSpec.r = ClampUp( (colSpec.r *slLR)>>8, 255L);
		colSpec.g = ClampUp( (colSpec.g *slLG)>>8, 255L);
		colSpec.b = ClampUp( (colSpec.b *slLB)>>8, 255L);
		
		const Matrix12 &mObjToView = *shaGetObjToViewMatrix();
		FLOAT3D vLightDir = -shaGetLightDirection();
		const FLOAT3D vLightObj = vLightDir.Normalize();
		
		const ShaderRegister srLightObj(vLightObj);
		const ShaderRegister srBaseColor(colBase);
		const ShaderRegister srSpecColor(colSpec);
		
		
		shaSetFogTextureUnit(2,bOpaque);
		shaSetVertexProgram(iBaseVP);
		shaSetPixelProgram(iBasePP);
		shaSetTexture(iSpecularTexture,1);
		
		shaSetVertexProgramConst(13, &srSpecColor, 1);
		shaSetVertexProgramConst(14, &mObjToView, 3);
		shaSetVertexProgramConst(17, &srLightObj, 1);
		shaSetPixelProgramConst(0,   &srBaseColor, 1);
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
		if(bHardwareShader) 
		{
			// match pixel shader settings
			shaBlendFunc(GFX_ONE, GFX_SRC_ALPHA);
		}
		else 
		{
			// Adjust alpha for fog
			shaAttenuateAlphaForFogAndHaze();
			// Set blend mode
			shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
		}
		shaEnableBlend();
		shaDisableDepthWrite();
		
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
		shaDisableOverBrightning();
		// calculate shading for specular pass
		DoSpecularLayer(iSpecularTexture,iSpecularColor,bOpaque);
		// disable overbrighning
		shaCullFace(GFX_BACK);
		// if surface is opaque
		if(bOpaque) 
		{
			// render fog and haze pass for surface
			shaDoFogAndHazePass();
		}
	}
	
	D3D_DISABLE_SPECULAR();
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(Specular,ShaderDesc *&pshDesc)
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
		shDescMe.sd_astrTextureNames[1] = "Specular texture";
		shDescMe.sd_astrTexCoordNames[0] = "Base uvmap";
		shDescMe.sd_astrColorNames[0] = "Base color";
		shDescMe.sd_astrColorNames[1] = "Specular color";
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

SHADER_VCODE(Specular, CTString &strVPCode, INDEX iVertexProgram)
{
	ASSERT(iVertexProgram==iBaseVP);
	strVPCode = CALCULATE_SHADING_VP
				STANDARD_OUTPUT_VP
				CALCULATE_SPECULAR_VP
				"mad  oT1.xy, r7.xy,r4.ww, c7.ww \n"
				;
}

// Fc = Bc*Ba + Sc*Sa
// Fa = 1-Ba
SHADER_PCODE(Specular, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	ASSERT(iPixelProgram==iBasePP);
	if(eFogType==FT_NONE) 
	{
		
/*		strPPCode = "tex    t0                      \n" // load base texture
					"tex    t1                      \n" // load specular texture
					"mul    t0,     t0,   c0        \n" // mul base texture with base color
					"mul    r0.rgb, t0,   t0.a      \n" // Bc*Ba
					"mul_x2 r0.rgb, r0,   v0        \n" // Shade pixel
					"+mov   r0.a,   1-t0.a          \n" // Fa = 1-Ba
					"mad    r0.rgb, t1,   v1,  r0   \n" // Fc = Bc*Ba + Sc*Sa
					D3D_DISABLE_SPECULAR_PS
					;*/
		strPPCode = "texld  r0,		t0              \n" // load base texture
					"texld  r1,		t1              \n" // load specular texture
					"mul    r2,     r0,   c0        \n" // mul base texture with base color
					"mul    r2.rgb, r0,   r0.a      \n" // Bc*Ba
					"mul_x2 r2.rgb, r0,   v0        \n" // Shade pixel
					"+mov   r0.a,   1-r2.a          \n" // Fa = 1-Ba
					"mad    r0.rgb, r1,   v1,  r2   \n" // Fc = Bc*Ba + Sc*Sa
					D3D_DISABLE_SPECULAR_PS
					;
	}
	else if(eFogType==FT_OPAQUE) 
	{
/*		strPPCode = "tex    t0                      \n" // load base texture
					"tex    t1                      \n" // load specular texture
					"tex    t2                      \n" // load fog texture
					"mul    t0,     t0,   c0        \n" // mul base texture with base color
					"mul    t2,     t2,   c7        \n" // mul fog texture with fog color
					"mul    r0.rgb, t0,   t0.a      \n" // Bc*Ba
					"mul_x2 r0.rgb, r0,   v0        \n" // Shade pixel
					"+mov   r0.a,   1-t0.a          \n" // Fa = 1-Ba
					"mad    r0.rgb, t1,   v1,  r0   \n" // Fc = Bc*Ba + Sc*Sa
					"lrp    r0.rgb, t2.a, t2,  r0   \n" // Add fog
					D3D_DISABLE_SPECULAR_PS
					;*/
		strPPCode = "texld  r0,     t0              \n" // load base texture
					"texld  r1,		t1              \n" // load specular texture
					"texld	r2,		t2                      \n" // load fog texture
					"mul    r3,     r0,   c0        \n" // mul base texture with base color
					"mul    r2,     r2,   c7        \n" // mul fog texture with fog color
					"mul    r0.rgb, r3,   r3.a      \n" // Bc*Ba
					"mul_x2 r0.rgb, r0,   v0        \n" // Shade pixel
					"+mov   r0.a,   1-r3.a          \n" // Fa = 1-Ba
					"mad    r0.rgb, r1,   v1,  r0   \n" // Fc = Bc*Ba + Sc*Sa
					"lrp    r0.rgb, r2.a, r2,  r0   \n" // Add fog
					D3D_DISABLE_SPECULAR_PS
					;
	}
	else if(eFogType==FT_NON_OPAQUE) 
	{
/*		strPPCode = "tex    t0                      \n" // load base texture
					"tex    t1                      \n" // load specular texture
					"tex    t2                      \n" // load fog texture
					"mul    t0,     t0,   c0        \n" // mul base texture with base color
					"mul    t2,     t2,   c7        \n" // mul fog texture with fog color
					"mul    t1.rgb, t1,   1-t2.a    \n" // attenuate specular tex color with fog alpha
					"+mul   t0.a,   t0.a, 1-t2.a    \n" // attenuate base tex alpha with fog alpha
					"mul    r0.rgb, t0,   t0.a      \n" // Bc*Ba
					"mul_x2 r0.rgb, r0,   v0        \n" // Shade pixel
					"+mov   r0.a,   1-t0.a          \n" // Fa = 1-Ba
					"mad    r0.rgb, t1,   v1,  r0   \n" // Fc = Bc*Ba + Sc*Sa
					D3D_DISABLE_SPECULAR_PS
					;*/
		strPPCode = "texld	r0,		t0              \n" // load base texture
					"texld  r1,		t1              \n" // load specular texture
					"texld  r2,		t2              \n" // load fog texture
					"mul    r3,     r0,   c0        \n" // mul base texture with base color
					"mul    r2,     r2,   c7        \n" // mul fog texture with fog color
					"mul    r1.rgb, r1,   1-r2.a    \n" // attenuate specular tex color with fog alpha
					"+mul   r3.a,   r3.a, 1-r2.a    \n" // attenuate base tex alpha with fog alpha
					"mul    r0.rgb, r3,   r3.a      \n" // Bc*Ba
					"mul_x2 r0.rgb, r0,   v0        \n" // Shade pixel
					"+mov   r0.a,   1-r3.a          \n" // Fa = 1-Ba
					"mad    r0.rgb, r1,   v1,  r0   \n" // Fc = Bc*Ba + Sc*Sa
					D3D_DISABLE_SPECULAR_PS
					;
	}
}

