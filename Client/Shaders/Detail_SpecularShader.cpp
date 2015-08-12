#include "StdH.h"
#include <Shaders/Common.h>

static const INDEX ctTextures = 3;
static const INDEX ctUVMaps   = 2;
static const INDEX ctColors   = 3;
static const INDEX ctFloats   = 2;
static const INDEX ctFlags    = 2;
static const INDEX ctVertexPrograms = 2;
static const INDEX ctPixelPrograms = 2;

static const INDEX iBaseTexture = 0;
static const INDEX iBaseUVMap   = 0;
static const INDEX iBaseColor   = 0;
static const INDEX iTranslucentPP = 1;
static const INDEX iBaseVP = 1;
static const INDEX iDetailVP = 0;
static const INDEX iDetailPP = 0;

static const INDEX iDetailTexture = 1;
static const INDEX iDetailUVMap   = 1;
static const INDEX iDetailColor   = 1;
static const INDEX iDetailTilingU = 0;
static const INDEX iDetailTilingV = 1;

static const INDEX iSpecularTexture = 2;
static const INDEX iSpecularColor   = 2;

SHADER_MAIN(Detail_Specular)
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
		GFXColor colDetail;
		GFXColor colSpec;
		colBase.MultiplyRGBA( (GFXColor)shaGetModelColor(), (GFXColor)shaGetCurrentColor());
		colDetail = shaGetColor(iDetailColor);
		
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
		
		// prepare detail texcoords
		const FLOAT fTilingX = shaGetFloat(iDetailTilingU);
		const FLOAT fTilingY = shaGetFloat(iDetailTilingV);
		
		const ShaderRegister srTiling(fTilingX,fTilingY,0,0);
		const ShaderRegister srLightObj(vLightObj);
		const ShaderRegister srBaseColor(colBase);
		const ShaderRegister srDetailColor(colDetail);
		const ShaderRegister srSpecColor(colSpec);
		
		
		if(bOpaque) 
		{
			shaSetFogTextureUnit(3,TRUE);
			shaSetTexture(iSpecularTexture,2);
			shaSetVertexProgram(iDetailVP);
			shaSetPixelProgram(iDetailPP);
			shaSetTextureUnit(1);
			shaSetTextureWrapping(GFX_REPEAT, GFX_REPEAT);
			shaSetTexture(iDetailTexture,1);
			
			shaSetVertexProgramConst(12, &srTiling, 1);
		}
		else 
		{
			shaSetFogTextureUnit(2,FALSE);
			shaSetTexture(iSpecularTexture,1);
			shaSetVertexProgram(iBaseVP);
			shaSetPixelProgram(iTranslucentPP);
		}
		shaSetVertexProgramConst(13, &srSpecColor,   1);
		shaSetVertexProgramConst(14, &mObjToView,    3);
		shaSetVertexProgramConst(17, &srLightObj,    1);
		shaSetPixelProgramConst(0,   &srBaseColor,   1);
		shaSetPixelProgramConst(1,   &srDetailColor, 1);
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
		// prepare fog and haze
		shaPrepareFogAndHaze(bOpaque);
		// calculate shading
		shaCalculateLight();
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
		shaEnableBlend();
		shaDisableDepthWrite();
		if(bHardwareShader) 
		{
			shaBlendFunc(GFX_ONE, GFX_SRC_ALPHA);
		}
		else 
		{
			shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
			// Adjust alpha for fog
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
	
	/* Do detail pass */
	
	if(!bHardwareShader) 
	{
		if(bOpaque) 
		{
			// Set blend mode
			shaBlendFunc( GFX_DST_COLOR, GFX_SRC_COLOR);
			shaEnableBlend();
			
			shaSetTexture(iDetailTexture,0);
			shaSetUVMap(iDetailUVMap);
			
			// calculate new shading (full bright)
			shaSetSurfaceFlags(shaGetFlags()|BASE_FULL_BRIGHT);
			shaSetColor(iDetailColor);
			shaCalculateLight();
			
			// prepare detail texcoords
			const FLOAT fTilingX = shaGetFloat(iDetailTilingU);
			const FLOAT fTilingY = shaGetFloat(iDetailTilingV);
			
			GFXTexCoord *ptxcOld = shaGetUVMap(iDetailUVMap);
			GFXTexCoord *ptxcNew = shaGetNewTexCoordArray();
			INDEX cttxc = shaGetVertexCount();
			if(cttxc>0) 
			{
				for(INDEX itxc=0;itxc<cttxc;itxc++) 
				{
					ptxcNew[itxc].u = ptxcOld[itxc].u * fTilingX;
					ptxcNew[itxc].v = ptxcOld[itxc].v * fTilingY;
				}
				// set new tex coords
				shaSetTexCoordArray(ptxcNew);
				// render detail pass
				shaRender();
			}
			
			// disable overbrighning
			shaDisableOverBrightning();
			
			DoSpecularLayer(iSpecularTexture,iSpecularColor,bOpaque);
			
			// if surface is opaque
			if(bOpaque) 
			{
				// render fog and haze pass for surface
				shaDoFogAndHazePass();
			}
		}
		else 
		{
			// disable overbrighning
			shaDisableOverBrightning();
		}
	}
	
	D3D_DISABLE_SPECULAR();
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(Detail_Specular,ShaderDesc *&pshDesc)
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
		shDescMe.sd_astrTextureNames[1] = "Detail texture";
		shDescMe.sd_astrTextureNames[2] = "Specular texture";
		shDescMe.sd_astrTexCoordNames[0] = "Base UVMap";
		shDescMe.sd_astrTexCoordNames[1] = "Detail UVMap";
		shDescMe.sd_astrColorNames[0] = "Surface color";
		shDescMe.sd_astrColorNames[1] = "Detail color";
		shDescMe.sd_astrColorNames[2] = "Specular color";
		shDescMe.sd_astrFloatNames[0] = "U tiling";
		shDescMe.sd_astrFloatNames[1] = "V tiling";
		shDescMe.sd_astrFlagNames[0] = "Double sided";
		shDescMe.sd_astrFlagNames[1] = "Full bright";
		shDescMe.sd_strShaderInfo = "Detail&Specular shader";
		
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_NORMAL_STREAM;
		shDescMe.sd_ulStreamFlags[1] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_NORMAL_STREAM;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(Detail_Specular, CTString &strVPCode, INDEX iVertexProgram)
{
	// if this is detail vertex program
	if(iVertexProgram==iDetailVP) 
	{
		strVPCode = CALCULATE_SHADING_VP
					STANDARD_OUTPUT_VP
					CALCULATE_SPECULAR_VP
					"mad  oT2.xy, r7.xy,r4.ww, c7.ww \n" // Output specular texcoords
					"mul  oT1.xy, v5.xy, c12.xy      \n" // Calculate detail texcoords
					;
		// else if this is base vertex program
	}
	else if(iVertexProgram==iBaseVP) 
	{
		strVPCode = CALCULATE_SHADING_VP
					STANDARD_OUTPUT_VP
					CALCULATE_SPECULAR_VP
					"mad  oT1.xy, r7.xy,r4.ww, c7.ww \n"
					;
	}
	else 
	{
		ASSERTALWAYS("Invalid pixel program index");
	}
}

SHADER_PCODE(Detail_Specular, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	// Detail      Fc = (Bc*Dc)+(Bc*Dc)+Sc
	// Translucent Fc = (Bc*Ba), Fa = (1-Ba)
	
	ASSERT(iPixelProgram==iDetailPP || iPixelProgram==iTranslucentPP);
	if(eFogType==FT_NONE) 
	{
		// if this is detail pixel shader
		if(iPixelProgram==iDetailPP) 
		{
/*			strPPCode = "tex    t0                     \n" // load base texture
						"tex    t1                     \n" // load detail texture
						"tex    t2                     \n" // load specular texture
						"mul    t0,     t0,   c0       \n" // mul base texture with base color
						"mul    t1,     t1,   c1       \n" // mul detail texture with detail color
						"mul_x2 t0,     t0,   v0       \n" // shade pixel
						"mul_x2 r0,     t0,   t1       \n" // combine textures
						"mad    r0.rgb, t2,   v1,  r0  \n" // add specular
						D3D_DISABLE_SPECULAR_PS
						;*/
			// else if this is base pixel shader
			strPPCode = "texld	r0,		t0                     \n" // load base texture
						"texld	r1,		t1                     \n" // load detail texture
						"texld	r2,		t2                     \n" // load specular texture
						"mul    r3,     r0,   c0       \n" // mul base texture with base color
						"mul    r1,     r1,   c1       \n" // mul detail texture with detail color
						"mul_x2 r3,     r3,   v0       \n" // shade pixel
						"mul_x2 r0,     r3,   r1       \n" // combine textures
						"mad    r0.rgb, r2,   v1,  r0  \n" // add specular
						D3D_DISABLE_SPECULAR_PS
						;
		}
		else if(iPixelProgram==iTranslucentPP) 
		{
/*			strPPCode = "tex    t0                     \n" // load base texture
						"tex    t1                     \n" // load specular texture
						"mul    t0,     t0,   c0       \n" // mul base texture with base color
						"mul    r0.rgb, t0,   t0.a     \n" // Bc*Ba
						"mul_x2 r0.rgb, r0,   v0       \n" // Shade pixel
						"+mov   r0.a,   1-t0.a         \n" // Fa = 1-Ba
						"mad    r0.rgb, t1,   v1,  r0  \n" // Fc = Bc*Ba+Sc*Sa
						D3D_DISABLE_SPECULAR_PS
						;*/
			strPPCode = "texld	r0,    t0                     \n" // load base texture
						"texld	r1,    t1                     \n" // load specular texture
						"mul    r2,     r0,   c0       \n" // mul base texture with base color
						"mul    r0.rgb, r2,   r2.a     \n" // Bc*Ba
						"mul_x2 r0.rgb, r0,   v0       \n" // Shade pixel
						"+mov   r0.a,   1-r2.a         \n" // Fa = 1-Ba
						"mad    r0.rgb, r1,   v1,  r0  \n" // Fc = Bc*Ba+Sc*Sa
						D3D_DISABLE_SPECULAR_PS
						;
		}
	}
	else if(eFogType==FT_OPAQUE) 
	{
		ASSERT(iPixelProgram==iDetailPP);
/*		strPPCode = "tex    t0                     \n" // load base texture
					"tex    t1                     \n" // load detail texture
					"tex    t2                     \n" // load specular texture
					"tex    t3                     \n" // load fog texture
					"mul    t0,     t0,   c0       \n" // mul base texture with base color
					"mul    t1,     t1,   c1       \n" // mul detail texture with detail color
					"mul    t3,     t3,   c7       \n" // mul fog texture with fog color
					"mul_x2 t0,     t0,   v0       \n" // shade pixel
					"mul_x2 r0,     t0,   t1       \n" // combine textures
					"mad    r0.rgb, t2,   v1,  r0  \n" // add specular
					"lrp    r0.rgb, t3.a, t3,  r0  \n" // Add fog
					D3D_DISABLE_SPECULAR_PS
					;*/
		strPPCode = "texld	r0,    t0                     \n" // load base texture
					"texld	r1,    t1                     \n" // load detail texture
					"texld	r2,    t2                     \n" // load specular texture
					"texld	r3,    t3                     \n" // load fog texture
					"mul    r4,     r0,   c0       \n" // mul base texture with base color
					"mul    r1,     r1,   c1       \n" // mul detail texture with detail color
					"mul    r3,     r3,   c7       \n" // mul fog texture with fog color
					"mul_x2 r4,     r4,   v0       \n" // shade pixel
					"mul_x2 r0,     r4,   r1       \n" // combine textures
					"mad    r0.rgb, r2,   v1,  r0  \n" // add specular
					"lrp    r0.rgb, r3.a, r3,  r0  \n" // Add fog
					D3D_DISABLE_SPECULAR_PS
					;
	}
	else if(eFogType==FT_NON_OPAQUE) 
	{
		ASSERT(iPixelProgram==iTranslucentPP);
/*		strPPCode = "tex    t0                     \n" // load base texture
					"tex    t1                     \n" // load specular texture
					"tex    t2                     \n" // load fog texture
					"mul    t0,     t0,   c0       \n" // mul base texture with base color
					"mul    t2,     t2,   c7       \n" // mul base texture with base color
					"mul    t1.rgb, t1,   1-t2.a   \n" // attenuate specular tex color with fog alpha 
					"+mul   t0.a,   t0,   1-t2.a   \n" // attenuate base tex alpha with fog alpha
					"mul    r0.rgb, t0,   t0.a     \n" // Bc*Ba
					"mul_x2 r0.rgb, r0,   v0       \n" // Shade pixel
					"+mov   r0.a,   1-t0.a         \n" // Fa = 1-Ba
					"mad    r0.rgb, t1,   v1,  r0  \n" // Fc = Bc*Ba+Sc*Sa
					D3D_DISABLE_SPECULAR_PS
					;*/
		strPPCode = "texld	r0,    t0                     \n" // load base texture
					"texld	r1,    t1                     \n" // load specular texture
					"texld	r2,    t2                     \n" // load fog texture
					"mul    r3,     r0,   c0       \n" // mul base texture with base color
					"mul    r2,     r2,   c7       \n" // mul base texture with base color
					"mul    r1.rgb, r1,   1-r2.a   \n" // attenuate specular tex color with fog alpha 
					"+mul   r3.a,   r3,   1-t2.a   \n" // attenuate base tex alpha with fog alpha
					"mul    r0.rgb, r3,   r3.a     \n" // Bc*Ba
					"mul_x2 r0.rgb, r0,   v0       \n" // Shade pixel
					"+mov   r0.a,   1-r3.a         \n" // Fa = 1-Ba
					"mad    r0.rgb, r1,   v1,  r0  \n" // Fc = Bc*Ba+Sc*Sa
					D3D_DISABLE_SPECULAR_PS
					;
	}
}
