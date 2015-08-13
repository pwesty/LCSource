#include "StdH.h"
#include <Shaders/Common.h>

static const INDEX ctTextures = 0;
static const INDEX ctUvmaps   = 0;
static const INDEX ctColors   = 1;
static const INDEX ctFloats   = 0;
static const INDEX ctFlags    = 2;
static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms = 1;

static const INDEX iBaseColor   = 0;
static const INDEX iBaseVP = 0;
static const INDEX iBasePP = 0;

SHADER_MAIN(Color)
{
	// Set depth test
	shaEnableDepthTest();
	shaEnableDepthWrite();
	shaDepthFunc(GFX_LESS_EQUAL);
	// Set blend mode
	shaDisableBlend();
	// Set alpha test
	shaDisableAlphaTest();
	
	shaSetTexture(-1);
	shaSetColor(iBaseColor);
	
	const BOOL bDoubleSided = shaGetFlags()&BASE_DOUBLE_SIDED;
	const BOOL bHardwareShader = shaUseHWShaders();
	
	if(bDoubleSided) 
	{
		shaCullFace(GFX_NONE);
	}
	else 
	{
		shaCullFace(GFX_BACK);
	}
	
	if(bHardwareShader) 
	{
		GFXColor colBase;
		colBase.MultiplyRGBA( (GFXColor)shaGetModelColor(), (GFXColor)shaGetCurrentColor());
		const ShaderRegister srBaseColor(colBase);
		
		shaSetFogTextureUnit(0,TRUE);
		shaSetVertexProgram(iBaseVP);
		shaSetPixelProgram(iBasePP);
		shaSetPixelProgramConst( 0, &srBaseColor, 1);
		// prepare fog and haze
		shaPrepareFogAndHaze(TRUE);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		shaSetDefaultConstantRegisters();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	}
	else 
	{
		// calculate shading
		shaCalculateLight();
		// prepare fog and haze
		shaPrepareFogAndHaze(TRUE);
	}
	
	// Render surface
	shaRender();
	if(!bHardwareShader) 
	{
		// render fog and haze pass for surface
		shaDoFogAndHazePass();
	}
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(Color,ShaderDesc *&pshDesc)
{
	static bool bInit = false;
	static ShaderDesc shDescMe;
	if(!bInit)
	{
		bInit = true;
		shDescMe.sd_astrTextureNames.New(ctTextures);
		shDescMe.sd_astrTexCoordNames.New(ctUvmaps);
		shDescMe.sd_astrColorNames.New(ctColors);
		shDescMe.sd_astrFloatNames.New(ctFloats);
		shDescMe.sd_astrFlagNames.New(ctFlags);
		
		shDescMe.sd_astrColorNames[0] = "Surface color";
		shDescMe.sd_astrFlagNames[0] = "Double sided";
		shDescMe.sd_astrFlagNames[1] = "Full bright";
		shDescMe.sd_strShaderInfo = "Color shader";
		
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM|GFX_NORMAL_STREAM;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(Color, CTString &strVPCode, INDEX iVertexProgram)
{
	ASSERT(iVertexProgram==iBaseVP);
	strVPCode = CALCULATE_SHADING_VP
				"m4x4 oPos, r0,   c0            \n" // Transform vector position in screen space
				;
}

// F = B
SHADER_PCODE(Color, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	ASSERT(iPixelProgram==iBasePP);
//안태훈 수정 시작	//(Modify Sam's Old Shaders)(0.1)
	if(eFogType==FT_NONE) 
	{
		strPPCode = "mul_x2 r0,     c0,    v0      \n" // shade pixel
					;
	}
	else if(eFogType==FT_OPAQUE) 
	{
/*		strPPCode = "tex    t0                     \n" // load fog texture
					"mul    t0,     t0,    c7      \n" // mul fog texture with fog color
					"mul_x2 r0,     c0,    v0      \n" // shade pixel
					"lrp    r0.rgb, t0.a,  t0,  r0 \n" // Add fog
					;*/
		strPPCode = "texld	r0,    t0                     \n" // load fog texture
					"mul    r1,     r0,    c7      \n" // mul fog texture with fog color
					"mul_x2 r0,     c0,    v0      \n" // shade pixel
					"lrp    r0.rgb, r1.a,  r1,  r0 \n" // Add fog
					;
	}
	else 
	{
		ASSERTALWAYS("Color shader cant be non opaque");
	}
//안태훈 수정 끝	//(Modify Sam's Old Shaders)(0.1)
}
