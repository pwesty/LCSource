#include "StdH.h"
#include <Shaders/Common.h>

//안태훈 수정 시작	//(Easy Use Ska Studio)(0.1)
#define TRANSLUCENT_FLIP				(1UL<<2)	// Use Flip
//안태훈 수정 끝	//(Easy Use Ska Studio)(0.1)

static const INDEX ctTextures = 1;
static const INDEX ctUVMaps   = 1;
static const INDEX ctColors   = 1;
static const INDEX ctFloats   = 0;
//안태훈 수정 시작	//(Easy Use Ska Studio)(0.1)
static const INDEX ctFlags    = 3;
//안태훈 수정 끝	//(Easy Use Ska Studio)(0.1)
static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms = 1;

static const INDEX iBaseTexture = 0;
static const INDEX iBaseUVMap   = 0;
static const INDEX iBaseColor   = 0;
static const INDEX iBaseVP = 0;
static const INDEX iBasePP = 0;

SHADER_MAIN(Translucent)
{
	// Set texture wrapping
	shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
	// Set depth test
	shaEnableDepthTest();
	shaDisableDepthWrite();
	shaDepthFunc(GFX_LESS_EQUAL);
	// Set alpha test
	shaDisableAlphaTest();
	
	shaSetTexture(iBaseTexture,0);
	shaSetUVMap(iBaseUVMap);
	shaSetColor(iBaseColor);
	
	const BOOL bDoubleSided = shaGetFlags()&BASE_DOUBLE_SIDED;
//안태훈 수정 시작	//(Easy Use Ska Studio)(0.1)
	const BOOL bFlip = shaGetFlags() & TRANSLUCENT_FLIP;
//안태훈 수정 끝	//(Easy Use Ska Studio)(0.1)
	const BOOL bHardwareShader = shaUseHWShaders();
	
	shaEnableBlend();
	if(bHardwareShader) 
	{
		GFXColor colBase;
		colBase.MultiplyRGBA( (GFXColor)shaGetModelColor(), (GFXColor)shaGetCurrentColor());
		
		const ShaderRegister srBaseColor(colBase);
		
		// Set blend mode
		shaBlendFunc(GFX_ONE, GFX_SRC_ALPHA);
		shaSetFogTextureUnit(1,FALSE);
		shaSetVertexProgram(iBaseVP);
		shaSetPixelProgram(iBasePP);
		shaSetPixelProgramConst(  0, &srBaseColor, 1);
		// prepare fog and haze
		shaPrepareFogAndHaze(FALSE);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		shaSetDefaultConstantRegisters();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	}
	else 
	{
		// Set blend mode
		shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
		// enable overbrightning
		shaEnableOverBrightning();
		// calculate shading
		shaCalculateLight();
		// prepare fog and haze
		shaPrepareFogAndHaze(FALSE);
		// adjust alpha for fog and haze
		shaAttenuateAlphaForFogAndHaze();
	}
	
//안태훈 수정 시작	//(Easy Use Ska Studio)(0.1)
	// if double sided flag is on
	if(bDoubleSided) 
	{
		// Render back side
		if(bFlip) shaCullFace(GFX_BACK);
		else shaCullFace(GFX_FRONT);
		shaRender();
	}
	// Render front side
	if(bFlip) shaCullFace(GFX_FRONT);
	else shaCullFace(GFX_BACK);
	shaRender();
//안태훈 수정 끝	//(Easy Use Ska Studio)(0.1)
	
	if(!bHardwareShader) 
	{
		// disable overbrighning
		shaDisableOverBrightning();
	}
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(Translucent,ShaderDesc *&pshDesc)
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
		
		shDescMe.sd_astrTextureNames[0] = "Translucent texture";
		shDescMe.sd_astrTexCoordNames[0] = "Translucent uvmap";
		shDescMe.sd_astrColorNames[0] = "Translucent color";
		shDescMe.sd_astrFlagNames[0] = "Double sided";
		shDescMe.sd_astrFlagNames[1] = "Full bright";
		shDescMe.sd_astrFlagNames[2] = "Flip";
		shDescMe.sd_strShaderInfo = "Translucent shader";
		
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_NORMAL_STREAM;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}
