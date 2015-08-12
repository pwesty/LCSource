#include "StdH.h"
#include <Shaders/Common.h>

static const INDEX ctTextures = 3;
static const INDEX ctUVMaps   = 1;
static const INDEX ctColors   = 3;
static const INDEX ctFloats   = 0;
static const INDEX ctFlags    = 2;
static const INDEX ctVertexPrograms = 3;
static const INDEX ctPixelPrograms = 3;

static const INDEX iBaseTexture = 0;
static const INDEX iBaseUVMap   = 0;
static const INDEX iBaseColor   = 0;

static const INDEX iGlassVP = 0;
static const INDEX iGlassPP = 0;
static const INDEX iAdd_ReflectionVP = 1;
static const INDEX iAdd_ReflectionPP = 1;
static const INDEX iSpecularVP = 2;
static const INDEX iSpecularPP = 2;

static const INDEX iReflectionTexture = 1;
static const INDEX iReflectionColor   = 1;
static const INDEX iSpecularTexture = 2;
static const INDEX iSpecularColor   = 2;

SHADER_MAIN(Glass)
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
	
	const BOOL bHardwareShader = shaUseHWShaders();
	const BOOL bHasFog = shaGetRenFlags()&SHA_RMF_HAZE || shaGetRenFlags()&SHA_RMF_FOG;
	
	D3D_ENABLE_SPECULAR();
	
	if(bHardwareShader) 
	{
		// set surface color
		GFXColor colBase;
		GFXColor colRefl;
		GFXColor colSpec;
		GFXColor colAdd = 0xFFFFFFFF;
		
		colBase.MultiplyRGBA( (GFXColor)shaGetModelColor(), (GFXColor)shaGetCurrentColor());
		colRefl = (GFXColor)shaGetColor(iReflectionColor);
		colRefl.AttenuateA((shaGetModelColor()&CT_AMASK)>>CT_ASHIFT);
		
		const COLOR colLight = shaGetLightColor();
		const SLONG slLR = (colLight & CT_RMASK)>>(CT_RSHIFT);
		const SLONG slLG = (colLight & CT_GMASK)>>(CT_GSHIFT);
		const SLONG slLB = (colLight & CT_BMASK)>>(CT_BSHIFT);
		colSpec = shaGetColor(iSpecularColor);
		colSpec.AttenuateRGB( (shaGetModelColor()&CT_AMASK)>>CT_ASHIFT);
		colSpec.r = ClampUp( (colSpec.r *slLR)>>8, 255L);
		colSpec.g = ClampUp( (colSpec.g *slLG)>>8, 255L);
		colSpec.b = ClampUp( (colSpec.b *slLB)>>8, 255L);
		
		CAnyProjection3D &apr  = *shaGetProjection();
		Matrix12 &mObjToAbs    = *shaGetObjToAbsMatrix();
		FLOATmatrix3D &mViewer = apr->pr_ViewerRotationMatrix;
		const Matrix12 &mObjToView = *shaGetObjToViewMatrix();
		FLOAT3D vLightDir = -shaGetLightDirection();
		const FLOAT3D vLightObj = vLightDir.Normalize();
		
		const ShaderRegister srViewer(-mViewer(3,1),-mViewer(3,2),-mViewer(3,3));
		const ShaderRegister srLightObj(vLightObj);
		const ShaderRegister srBaseColor(colBase);
		const ShaderRegister srReflColor(colRefl);
		const ShaderRegister srSpecColor(colSpec);
		const ShaderRegister srAddColor(colAdd);
		
		// Set blend mode
		shaBlendFunc(GFX_ONE, GFX_SRC_ALPHA);
		shaEnableBlend();
		
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		shaSetDefaultConstantRegisters();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

		// if in fog it can't be done in one pass 
		if(bHasFog) 
		{
			// First pass add + reflection + fog
			shaSetFogTextureUnit(2,FALSE);
			shaSetVertexProgram(iAdd_ReflectionVP);
			shaSetPixelProgram(iAdd_ReflectionPP);
			shaSetTexture(iReflectionTexture,1);
			
			// Set reflection const registers
			shaSetVertexProgramConst( 8, &mObjToAbs,   3);
			shaSetVertexProgramConst(11, &srViewer,    1);
			shaSetPixelProgramConst(0,   &srBaseColor, 1);
			shaSetPixelProgramConst(1,   &srReflColor, 1);
			
			shaPrepareFogAndHaze(FALSE);
			
			// if double sided flag is on
			if(shaGetFlags()&BASE_DOUBLE_SIDED) 
			{
				// Render back side
				shaCullFace(GFX_FRONT);
				shaRender();
			}
			// Render front side
			shaCullFace(GFX_BACK);
			shaRender();
			
			// Second pass specular + fog
			shaSetFogTextureUnit(1,FALSE);
			shaSetVertexProgram(iSpecularVP);
			shaSetPixelProgram(iSpecularPP);
			shaSetTexture(iSpecularTexture,0);
			
			// Set specular const registers
			shaSetVertexProgramConst(13, &srSpecColor, 1);
			shaSetVertexProgramConst(14, &mObjToView,  3);
			shaSetVertexProgramConst(17, &srLightObj,  1);
			
			// prepare fog and haze
			shaPrepareFogAndHaze(FALSE);
			
			shaBlendFunc( GFX_SRC_ALPHA, GFX_ONE);
			// if double sided flag is on
			if(shaGetFlags()&BASE_DOUBLE_SIDED) 
			{
				// Render back side
				shaCullFace(GFX_FRONT);
				shaRender();
			}
			// Render front side
			shaCullFace(GFX_BACK);
			shaRender();
			
			// normal glass shader done in one pass
		}
		else 
		{
			shaSetVertexProgram(iGlassVP);
			shaSetPixelProgram(iGlassPP);
			shaSetTexture(iReflectionTexture,1);
			shaSetTexture(iSpecularTexture,2);
			
			shaSetVertexProgramConst( 8, &mObjToAbs,   3);
			shaSetVertexProgramConst(11, &srViewer,    1);
			shaSetVertexProgramConst(13, &srSpecColor, 1);
			shaSetVertexProgramConst(14, &mObjToView,  3);
			shaSetVertexProgramConst(17, &srLightObj,  1);
			shaSetPixelProgramConst(0,   &srBaseColor, 1);
			shaSetPixelProgramConst(1,   &srReflColor, 1);
			shaSetPixelProgramConst(2,   &srAddColor,  1);
			// if double sided flag is on
			if(shaGetFlags()&BASE_DOUBLE_SIDED) 
			{
				// Render back side
				shaCullFace(GFX_FRONT);
				shaRender();
			}
			// Render front side
			shaCullFace(GFX_BACK);
			shaRender();
		}
  }
  else 
{
	  // enable overbrightning
	  shaEnableOverBrightning();
	  // calculate shading
	  shaCalculateLight();
	  // prepare fog and haze
	  shaPrepareFogAndHaze(FALSE);
	  // adjust alpha for fog and haze
	  shaAttenuateColorForFogAndHaze();
	  // Set blend mode
	  shaBlendFunc(GFX_SRC_ALPHA, GFX_ONE);
	  shaEnableBlend();
	  
	  
	  // if double sided flag is on
	  if(shaGetFlags()&BASE_DOUBLE_SIDED) 
	  {
		  // Render back side
		  shaCullFace(GFX_FRONT);
		  shaRender();
	  }
	  // Render front side
	  shaCullFace(GFX_BACK);
	  shaRender();
	  
	  DoReflectionLayer(iReflectionTexture,iReflectionColor,FALSE);
	  
	  // disable overbrighning
	  shaDisableOverBrightning();
	  
	  DoSpecularLayer(iSpecularTexture,iSpecularColor,FALSE);
  }
  
  D3D_DISABLE_SPECULAR();
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(Glass,ShaderDesc *&pshDesc)
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
		
		shDescMe.sd_astrTextureNames[0] = "Glass texture";
		shDescMe.sd_astrTextureNames[1] = "Reflection texture";
		shDescMe.sd_astrTextureNames[2] = "Specular texture";
		shDescMe.sd_astrTexCoordNames[0] = "Glass uvmap";
		shDescMe.sd_astrColorNames[0] = "Glass color";
		shDescMe.sd_astrColorNames[1] = "Reflection color";
		shDescMe.sd_astrColorNames[2] = "Specular color";
		shDescMe.sd_astrFlagNames[0] = "Double sided";
		shDescMe.sd_astrFlagNames[1] = "Full bright";
		shDescMe.sd_strShaderInfo = "Glass shader";
		
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_NORMAL_STREAM;
		shDescMe.sd_ulStreamFlags[1] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_NORMAL_STREAM;
		shDescMe.sd_ulStreamFlags[2] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_NORMAL_STREAM;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(Glass, CTString &strVPCode, INDEX iVertexProgram)
{
	// if this is glass vertex program
	if(iVertexProgram==iGlassPP) 
	{
		strVPCode = CALCULATE_SHADING_VP
					STANDARD_OUTPUT_VP
					CALCULATE_REFLECTION_VP
					"mad  oT1.xy, r9.xz,r8.yy, c7.ww  \n"
					CALCULATE_SPECULAR_VP
					"mad  oT2.xy, r7.xy,r4.ww, c7.ww  \n"
					;
		// if first pass of glass shader
	}
	else if(iVertexProgram==iAdd_ReflectionVP) 
	{
		strVPCode = CALCULATE_SHADING_VP
					STANDARD_OUTPUT_VP
					CALCULATE_REFLECTION_VP
					"mad  oT1.xy, r9.xz,r8.yy, c7.ww  \n"
					;
		// if second pass of glass shader
	}
	else if(iVertexProgram==iSpecularVP) 
	{
		strVPCode = CALCULATE_SHADING_VP
					STANDARD_OUTPUT_VP
					CALCULATE_SPECULAR_VP
					"mad  oT0.xy, r7.xy,r4.ww, c7.ww  \n"
					;
	}
	else 
	{
		ASSERT(FALSE);
	}
}

SHADER_PCODE(Glass, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	ASSERT(eFogType==FT_NONE || eFogType==FT_NON_OPAQUE);
	if(iPixelProgram==iGlassPP) 
	{
		ASSERT(eFogType==FT_NONE);
		// Fc = (Bc*Ba)*(1-Ra) + Rc*Ra + Sc
		// Fa = (1-Ra)
		// Glass shader
/*		strPPCode = "tex    t0                      \n" // load base texture
					"tex    t1                      \n" // load reflection texture
					"tex    t2                      \n" // load specular texture
					"mul    t0,      t0,   c0       \n" // mul base texture with base color
					"mul    t1,      t1,   c1       \n" // mul reflection texture with reflection color
					"mul    r0.rgb,  t0,   t0.a     \n" // Bc*Ba
					"lrp    r0.rgb,  1-t1.a, r0, t1 \n" // Fc = Bc*Ba*(1-Ra) + Rc*Ra
					"+mov   r0.a,    1-t1           \n" // Fa = (1-Ra)
					"mul_x2 r0.rgb,  r0,   v0       \n" // Shade pixel
					"mad    r0.rgb,  t2,   v1,   r0 \n" // Fc = Bc*Ba*(1-Ra) + Rc*Ra + Sc
					D3D_DISABLE_SPECULAR_PS
					;*/
		strPPCode = "texld	r0,    t0                      \n" // load base texture
					"texld	r1,    t1                      \n" // load reflection texture
					"texld	r2,    t2                      \n" // load specular texture
					"mul    r3,      r0,   c0       \n" // mul base texture with base color
					"mul    r1,      r1,   c1       \n" // mul reflection texture with reflection color
					"mul    r0.rgb,  r3,   r3.a     \n" // Bc*Ba
					"lrp    r0.rgb,  1-r1.a, r0, r1 \n" // Fc = Bc*Ba*(1-Ra) + Rc*Ra
					"+mov   r0.a,    1-r1           \n" // Fa = (1-Ra)
					"mul_x2 r0.rgb,  r0,   v0       \n" // Shade pixel
					"mad    r0.rgb,  r2,   v1,   r0 \n" // Fc = Bc*Ba*(1-Ra) + Rc*Ra + Sc
					D3D_DISABLE_SPECULAR_PS
					;
		// if in fog
	}
	else if(iPixelProgram==iAdd_ReflectionPP) 
	{
		// if first shader pass
		// ASSERT(eFogType==FT_NON_OPAQUE);
		// Fc = (Bc*Ba)*(1-Ra) + Rc*Ra
		// Fa = (1-Ra)
/*		strPPCode = "tex    t0                      \n" // load base texture
					"tex    t1                      \n" // load reflection texture
					"tex    t2                      \n" // load fog texture
					"mul    t0,      t0,   c0       \n" // mul base texture with base color
					"mul    t1,      t1,   c1       \n" // mul reflection texture with reflection color
					"mul    t2,      t2,   c7       \n" // mul fog texture with fog color
					"mul    t0.rgb,  t0,   1-t2.a   \n" // attenuate base tex color with fog alpha
					"mul    r0.rgb,  t0,   t0.a     \n" // Fc=Bc*Ba
					"+mul   t1.a,    t1.a, 1-t2.a   \n" // attenuate reflection alpha with fog alpha
					"lrp    r0.rgb,  1-t1.a, r0, t1 \n" // Fc = Bc*Ba*(1-Ra) + Rc*Ra
					"+mov   r0.a,    1-t1.a         \n" // Fa = (1-Ra)
					"mul_x2 r0.rgb,  r0,   v0       \n" // Shade pixel
					D3D_DISABLE_SPECULAR_PS
					;*/
		strPPCode = "texld	r0,    t0                      \n" // load base texture
					"texld	r1,    t1                      \n" // load reflection texture
					"texld	r2,    t2                      \n" // load fog texture
					"mul    r3,      r0,   c0       \n" // mul base texture with base color
					"mul    r1,      r1,   c1       \n" // mul reflection texture with reflection color
					"mul    r2,      r2,   c7       \n" // mul fog texture with fog color
					"mul    r3.rgb,  r3,   1-r2.a   \n" // attenuate base tex color with fog alpha
					"mul    r0.rgb,  r3,   r3.a     \n" // Fc=Bc*Ba
					"+mul   r1.a,    r1.a, 1-r2.a   \n" // attenuate reflection alpha with fog alpha
					"lrp    r0.rgb,  1-r1.a, r0, r1 \n" // Fc = Bc*Ba*(1-Ra) + Rc*Ra
					"+mov   r0.a,    1-r1.a         \n" // Fa = (1-Ra)
					"mul_x2 r0.rgb,  r0,   v0       \n" // Shade pixel
					D3D_DISABLE_SPECULAR_PS
					;
		// else if second shader pass
	}
	else if(iPixelProgram==iSpecularPP) 
	{
		// ASSERT(eFogType==FT_NON_OPAQUE);
		// Fc = Sc
/*		strPPCode = "tex    t0                      \n" // load specular texture
					"tex    t1                      \n" // load fog texture
					"mul    t1,      t1,   c7       \n" // mul fog texture with fog color
					"mul    t0.rgb,  t0,   1-t1.a   \n" // attenuate specular tex color with fog alpha
					"mul    r0.rgb,  t0,   v1       \n" // Fc = Sc
					"+mov   r0.a,    c2.a           \n" // Fa = 1.0f
					D3D_DISABLE_SPECULAR_PS
					;*/
		strPPCode = "texld	r0,		t0                      \n" // load specular texture
					"texld	r1,		t1                      \n" // load fog texture
					"mul    r1,      r1,   c7       \n" // mul fog texture with fog color
					"mul    r0.rgb,  r0,   1-r1.a   \n" // attenuate specular tex color with fog alpha
					"mul    r0.rgb,  r0,   v1       \n" // Fc = Sc
					"+mov   r0.a,    c2.a           \n" // Fa = 1.0f
					D3D_DISABLE_SPECULAR_PS
					;
	}
}
