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

static const INDEX iReflectionTexture = 1;
static const INDEX iReflectionColor   = 1;

SHADER_MAIN(Reflection)
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
	
	if(bHardwareShader) 
	{
		GFXColor colBase,colRefl;
		colBase.MultiplyRGBA((GFXColor)shaGetModelColor(),(GFXColor)shaGetCurrentColor());
		colRefl = (GFXColor)shaGetColor(iReflectionColor);
		colRefl.AttenuateA((shaGetModelColor()&CT_AMASK)>>CT_ASHIFT);
		
		CAnyProjection3D &apr  = *shaGetProjection();
		Matrix12 &mObjToAbs    = *shaGetObjToAbsMatrix();
		const FLOATmatrix3D &mViewer = apr->pr_ViewerRotationMatrix;
		
		const ShaderRegister srViewer(-mViewer(3,1),-mViewer(3,2),-mViewer(3,3));
		const ShaderRegister srBaseColor(colBase);
		const ShaderRegister srReflColor(colRefl);
		
		shaSetFogTextureUnit(2,bOpaque);
		shaSetVertexProgram(iBaseVP);
		shaSetPixelProgram(iBasePP);
		shaSetTexture(iReflectionTexture,1);
		shaSetVertexProgramConst( 8, &mObjToAbs,   3);
		shaSetVertexProgramConst(11, &srViewer,    1);
		shaSetPixelProgramConst(0,   &srBaseColor, 1);
		shaSetPixelProgramConst(1,   &srReflColor, 1);
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
		// else surface is translucent
	}
	else 
	{
		
		// Set blend mode
		// if using hardware shaders
		if(bHardwareShader) 
		{
			// match pixel shader settings
			shaBlendFunc(GFX_ONE, GFX_SRC_ALPHA);
			// else in software
		}
		else 
		{
			// use translucency
			shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
			// Adjust alpha for fog
			shaAttenuateAlphaForFogAndHaze();
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
	
	// if not using hardware shaders
	if(!bHardwareShader) 
	{
		// do reflection pass
		DoReflectionLayer(iReflectionTexture,iReflectionColor,bOpaque);
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
SHADER_DESC(Reflection,ShaderDesc *&pshDesc)
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
		shDescMe.sd_astrTextureNames[1] = "Reflection texture";
		shDescMe.sd_astrTexCoordNames[0] = "Base uvmap";
		shDescMe.sd_astrColorNames[0] = "Base color";
		shDescMe.sd_astrFlagNames[0] = "Double sided";
		shDescMe.sd_astrFlagNames[1] = "Full bright";
		shDescMe.sd_strShaderInfo = "Basic shader";
		shDescMe.sd_astrColorNames[1] = "Reflection color";
		
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_NORMAL_STREAM;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(Reflection, CTString &strVPCode, INDEX iVertexProgram)
{
	ASSERT(iVertexProgram==iBaseVP);
	strVPCode = CALCULATE_SHADING_VP
				STANDARD_OUTPUT_VP
				CALCULATE_REFLECTION_VP
				"mad  oT1.xy, r9.xz,r8.yy, c7.ww \n"
				;
}

// Fc = Bc*Ba*(1-Ra) + Rc*Ra
// Fa = (1-Ba)*(1-Ra)
SHADER_PCODE(Reflection, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	ASSERT(iPixelProgram==iBasePP);
	if(eFogType==FT_NONE) 
	{
/*		strPPCode = "tex    t0                      \n" // load base texture
					"tex    t1                      \n" // load reflection texture
					"mul    t0,      t0,   c0       \n" // mul base texture with base color
					"mul    t1,      t1,   c1       \n" // mul reflection texture with reflection color
					"mul    r0.rgb,  t0,   t0.a     \n" // Bc*Ba
					"lrp    r0.rgb,  1-t1.a, r0, t1 \n" // Fc = Bc*Ba*(1-Ra) + Rc*Ra
					"+mul   r0.a,    1-t0, 1-t1     \n" // Fa = (1-Ba)*(1-Ra)
					"mul_x2 r0.rgb,  r0,   v0       \n" // Shade pixel
					;*/
		strPPCode = "texld	r0,    t0               \n" // load base texture
					"texld	r1,    t1               \n" // load reflection texture
					"mul    r2,      r0,   c0       \n" // mul base texture with base color
					"mul    r1,      r1,   c1       \n" // mul reflection texture with reflection color
					"mul    r0.rgb,  r2,   r2.a     \n" // Bc*Ba
					"lrp    r0.rgb,  1-r1.a, r0, r1 \n" // Fc = Bc*Ba*(1-Ra) + Rc*Ra
					"+mul   r0.a,    1-r2, 1-r1     \n" // Fa = (1-Ba)*(1-Ra)
					"mul_x2 r0.rgb,  r0,   v0       \n" // Shade pixel
					;
	}
	else if(eFogType==FT_OPAQUE) 
	{
/*		strPPCode = "tex    t0                      \n" // load base texture
					"tex    t1                      \n" // load reflection texture
					"tex    t2                      \n" // load fog texture
					"mul    t0,      t0,   c0       \n" // mul base texture with base color
					"mul    t1,      t1,   c1       \n" // mul reflection texture with reflection color
					"mul    t2,      t2,   c7       \n" // mul fog texture with fog color
					"mul    r0.rgb,  t0,   t0.a     \n" // Bc*Ba
					"lrp    r0.rgb,  1-t1.a,r0, t1  \n" // Fc = Bc*Ba*(1-Ra) + Rc*Ra
					"+mul   r0.a,    1-t0, 1-t1     \n" // Fa = (1-Ba)*(1-Ra)
					"mul_x2 r0.rgb,  r0,   v0       \n" // Shade pixel
					"lrp    r0.rgb,  t2.a, t2,  r0  \n" // Add fog
					;*/
		strPPCode = "texld	r0,    t0               \n" // load base texture
					"texld	r1,    t1               \n" // load reflection texture
					"texld	r2,    t2               \n" // load fog texture
					"mul    r3,      r0,   c0       \n" // mul base texture with base color
					"mul    r1,      r1,   c1       \n" // mul reflection texture with reflection color
					"mul    r2,      r2,   c7       \n" // mul fog texture with fog color
					"mul    r0.rgb,  r3,   r3.a     \n" // Bc*Ba
					"lrp    r0.rgb,  1-r1.a,r0, r1  \n" // Fc = Bc*Ba*(1-Ra) + Rc*Ra
					"+mul   r0.a,    1-r3, 1-r1     \n" // Fa = (1-Ba)*(1-Ra)
					"mul_x2 r0.rgb,  r0,   v0       \n" // Shade pixel
					"lrp    r0.rgb,  r2.a, r2,  r0  \n" // Add fog
					;
	}
	else if(eFogType==FT_NON_OPAQUE) 
	{
/*		strPPCode = "tex    t0                      \n" // load base texture
					"tex    t1                      \n" // load reflection texture
					"tex    t2                      \n" // load fog texture
					"mul    t0,      t0,   c0       \n" // mul base texture with base color
					"mul    t1,      t1,   c1       \n" // mul reflection texture with reflection color
					"mul    t2,      t2,   c7       \n" // mul fog texture with fog color
					"mul    t0.a,    t0.a, 1-t2.a   \n" // attenuate base tex alpha with fog alpha
					"mul    r0.rgb,  t0,   t0.a     \n" // Bc*Ba
					"+mul   t1.a,    t1.a, 1-t2.a   \n" // attenuate reflection tex alpha with fog alpha
					"lrp    r0.rgb,  1-t1.a, r0, t1 \n" // Fc = Bc*Ba*(1-Ra) + Rc*Ra
					"+mul   r0.a,    1-t0, 1-t1     \n" // Fa = (1-Ba)*(1-Ra)
					"mul_x2 r0.rgb,  r0,   v0       \n" // Shade pixel
					;*/
		strPPCode = "texld	r0,    t0               \n" // load base texture
					"texld	r1,	   t1               \n" // load reflection texture
					"texld	r2,    t2               \n" // load fog texture
					"mul    r3,      r0,   c0       \n" // mul base texture with base color
					"mul    r1,      r1,   c1       \n" // mul reflection texture with reflection color
					"mul    r2,      r2,   c7       \n" // mul fog texture with fog color
					"mul    r3.a,    r3.a, 1-r2.a   \n" // attenuate base tex alpha with fog alpha
					"mul    r0.rgb,  r3,   r3.a     \n" // Bc*Ba
					"+mul   r1.a,    r1.a, 1-r2.a   \n" // attenuate reflection tex alpha with fog alpha
					"lrp    r0.rgb,  1-r1.a, r0, r1 \n" // Fc = Bc*Ba*(1-Ra) + Rc*Ra
					"+mul   r0.a,    1-r3, 1-r1     \n" // Fa = (1-Ba)*(1-Ra)
					"mul_x2 r0.rgb,  r0,   v0       \n" // Shade pixel
					;
	}
}
