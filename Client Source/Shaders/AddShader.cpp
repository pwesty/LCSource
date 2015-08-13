#include "StdH.h"
#include <Shaders/Common.h>

static const INDEX ctTextures = 1;
static const INDEX ctUVMaps   = 1;
static const INDEX ctColors   = 1;
static const INDEX ctFloats   = 0;
static const INDEX ctFlags    = 2;
static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms = 1;

static const INDEX iBaseTexture = 0;
static const INDEX iBaseUVMap   = 0;
static const INDEX iBaseColor   = 0;
static const INDEX iBaseVP = 0;
static const INDEX iBasePP = 0;

SHADER_MAIN(Add)
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
	const BOOL bHardwareShader = shaUseHWShaders();
	
	shaEnableBlend();
	
	if(bHardwareShader) 
	{
		GFXColor colBase;
		GFXColor colAdd; 
		colBase.MultiplyRGBA( (GFXColor)shaGetModelColor(), (GFXColor)shaGetCurrentColor());
		colAdd = 0xFFFFFFFF;
		const ShaderRegister srBaseColor(colBase);
		const ShaderRegister srAddColor(colAdd);
		
		shaBlendFunc(GFX_ONE, GFX_ONE);
		shaSetFogTextureUnit(1, FALSE);
		shaSetVertexProgram(iBaseVP);
		shaSetPixelProgram(iBasePP);
		shaSetPixelProgramConst( 0, &srBaseColor, 1);
		shaSetPixelProgramConst( 1, &srAddColor,  1);
		// prepare fog and haze
		shaPrepareFogAndHaze(FALSE);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		shaSetDefaultConstantRegisters();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	}
	else 
	{
		shaBlendFunc(GFX_SRC_ALPHA, GFX_ONE);
		// enable overbrightning
		shaEnableOverBrightning();
		// calculate shading
		shaCalculateLight();
		// prepare fog and haze
		shaPrepareFogAndHaze(FALSE);
		// adjust alpha for fog and haze
		shaAttenuateColorForFogAndHaze();
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

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(Add,ShaderDesc *&pshDesc)
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
		
		shDescMe.sd_astrTextureNames[0] = "Add texture";
		shDescMe.sd_astrTexCoordNames[0] = "Add uvmap";
		shDescMe.sd_astrColorNames[0] = "Add color";
		shDescMe.sd_astrFlagNames[0] = "Double sided";
		shDescMe.sd_astrFlagNames[1] = "Full bright";
		shDescMe.sd_strShaderInfo = "Add shader";
		
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_NORMAL_STREAM;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(Add, CTString &strVPCode, INDEX iVertexProgram)
{
	ASSERT(iVertexProgram==iBaseVP);
	strVPCode = CALCULATE_SHADING_VP
		STANDARD_OUTPUT_VP
		;
}

// Fc = (Bc*Ba)
// Fa = 1.0f
SHADER_PCODE(Add, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	ASSERT(iPixelProgram==iBasePP);
	if(eFogType==FT_NONE) 
	{
/*		strPPCode = "tex    t0                     \n" // load base texture
			"mul    t0,      t0,   c0      \n" // mul base texture with base color
			"mul    r0.rgb,  t0,   t0.a    \n" // Fc = Bc*Ba
			"mul_x2 r0.rgb,  r0,   v0      \n" // Shade pixel
			"+mov   r0.a,    c1            \n" // Fa = 1.0f
			;                             */
		strPPCode = "texld	r0,    t0                     \n" // load base texture
			"mul    r1,      r0,   c0      \n" // mul base texture with base color
			"mul    r0.rgb,  r1,   r1.a    \n" // Fc = Bc*Ba
			"mul_x2 r0.rgb,  r0,   v0      \n" // Shade pixel
			"+mov   r0.a,    c1            \n" // Fa = 1.0f
			;
	}
	else if(eFogType==FT_OPAQUE) 
	{
		ASSERTALWAYS("Add shader does not have opaque mode");
	}
	else if(eFogType==FT_NON_OPAQUE) 
	{
/*		strPPCode = "tex    t0                     \n" // load base texture
			"tex    t1                     \n" // load fog texture
			"mul    t0,      t0,   c0      \n" // mul base texture with base color
			"mul    t1,      t1,   c7      \n" // mul fog texture with fog color
			"mul    t0.rgb,  t0,   1-t1.a  \n" // attenuate base tex color with fog alpha
			"mul    r0.rgb,  t0,   t0.a    \n" // Fc = Bc*Ba
			"mul_x2 r0.rgb,  r0,   v0      \n" // Shade pixel
			"+mov   r0.a,    c1            \n" // Fa = 1.0f
			;*/
		strPPCode = "texld	r0,    t0                     \n" // load base texture
			"texld	r1,    t1                     \n" // load fog texture
			"mul    r2,      r0,   c0      \n" // mul base texture with base color
			"mul    r1,      r1,   c7      \n" // mul fog texture with fog color
			"mul    r2.rgb,  r2,   1-r1.a  \n" // attenuate base tex color with fog alpha
			"mul    r0.rgb,  r2,   r2.a    \n" // Fc = Bc*Ba
			"mul_x2 r0.rgb,  r0,   v0      \n" // Shade pixel
			"+mov   r0.a,    c1            \n" // Fa = 1.0f
			;
	}
}
