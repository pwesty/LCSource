//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
#include "StdH.h"
#include <Shaders/Common.h>

#define DETAIL_FLIP				(1UL<<2)	// Use Flip

static const INDEX ctTextures = 2;
static const INDEX ctUVMaps   = 2;
static const INDEX ctColors   = 2;
static const INDEX ctFloats   = 2;
static const INDEX ctFlags    = 3;
static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms = 2;

static const INDEX iBaseTexture = 0;
static const INDEX iBaseUVMap   = 0;
static const INDEX iBaseColor   = 0;
static const INDEX iTranslucentPP = 1;
static const INDEX iDetailVP = 0;
static const INDEX iDetailPP = 0;

static const INDEX iDetailTexture = 1;
static const INDEX iDetailUVMap   = 0;
static const INDEX iDetailColor   = 1;
static const INDEX iDetailTilingU = 0;
static const INDEX iDetailTilingV = 1;

SHADER_MAIN(Detail)
{
	// Set texture wrapping
	shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
	// Set depth test
	shaEnableDepthTest();
	shaDepthFunc(GFX_LESS_EQUAL);
	
	// Set alpha test
	//shaDisableAlphaTest();	--->
	shaEnableAlphaTest();			//알파 테스트를 수행하게 수정 :Su-won
	
	shaSetTexture(iBaseTexture,0);
	shaSetUVMap(iBaseUVMap);
	shaSetColor(iBaseColor);
	
	const COLOR colModelColor = MulColors(shaGetModelColor(),shaGetCurrentColor());
	const BOOL bDoubleSided = shaGetFlags()&BASE_DOUBLE_SIDED;
	const BOOL bFlip = shaGetFlags() & DETAIL_FLIP;
	const BOOL bOpaque = (colModelColor&0xFF)==0xFF;
	const BOOL bHardwareShader = shaUseHWShaders();
	
	if(bHardwareShader) 
	{
		GFXColor colBase,colDetail;
		colBase.MultiplyRGBA( (GFXColor)shaGetModelColor(), (GFXColor)shaGetCurrentColor());
		colDetail = shaGetColor(iDetailColor);

		//알파테스트가 활성화되어 Fade Out할 때, ModelHolder3에서 설정한 모델의 알파값이
		//128~255사이일 때 모델이 화면에서 사라지는 현상(알파테스트시 비교값이 128로 되어있어서...)때문에
		//이 부분 추가.		:Su-won
		if( colBase.a >=128)
			colBase.a /=2.0f;

		// prepare detail texcoords
		const FLOAT fTilingX = shaGetFloat(iDetailTilingU);
		const FLOAT fTilingY = shaGetFloat(iDetailTilingV);
		
		const ShaderRegister srBaseColor(colBase);
		const ShaderRegister srDetailColor(colDetail);
		const ShaderRegister srTiling(fTilingX,fTilingY,0,0);
		
		
		if(bOpaque) 
		{
			shaSetFogTextureUnit(2,TRUE);
			shaSetVertexProgram(iDetailVP);
			shaSetPixelProgram(iDetailPP);
			
			shaSetTextureUnit(1);
			shaSetTextureWrapping(GFX_REPEAT, GFX_REPEAT);
			shaSetTexture(iDetailTexture,1);
			shaSetUVMap(iDetailUVMap,1);
			
			shaSetVertexProgramConst(12, &srTiling, 1);
		}
		else 
		{
			shaSetFogTextureUnit(2,FALSE);
			shaSetVertexProgram(iDetailVP);
			shaSetPixelProgram(iTranslucentPP);
		}
		
		shaSetPixelProgramConst(0, &srBaseColor,   1);
		shaSetPixelProgramConst(1, &srDetailColor, 1);
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
			if(!bFlip) shaCullFace(GFX_BACK);
			else shaCullFace(GFX_FRONT);
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
			if(!bFlip) shaCullFace(GFX_FRONT);
			else shaCullFace(GFX_BACK);
			shaRender();
		}
		// back face culling
		if(!bFlip) shaCullFace(GFX_BACK);
		else shaCullFace(GFX_FRONT);
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
			
/*
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
*/	//일단 Shader없는 카드에서 Detail 텍스쳐 렌더링 안함. 
			
			// disable overbrighning
			shaDisableOverBrightning();
			
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
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(Detail,ShaderDesc *&pshDesc)
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
		shDescMe.sd_astrTexCoordNames[0] = "Base UVMap";
		shDescMe.sd_astrTexCoordNames[1] = "Detail UVMap";
		shDescMe.sd_astrColorNames[0] = "Surface color";
		shDescMe.sd_astrColorNames[1] = "Detail color";
		shDescMe.sd_astrFloatNames[0] = "U tiling";
		shDescMe.sd_astrFloatNames[1] = "V tiling";
		shDescMe.sd_astrFlagNames[0] = "Double sided";
		shDescMe.sd_astrFlagNames[1] = "Full bright";
		shDescMe.sd_astrFlagNames[2] = "Flip";
		shDescMe.sd_strShaderInfo = "Detail shader";
		
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_NORMAL_STREAM;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(Detail, CTString &strVPCode, INDEX iVertexProgram)
{
	ASSERT(iVertexProgram==iDetailVP);
	strVPCode = CALCULATE_SHADING_VP
				STANDARD_OUTPUT_VP
				"mul  oT1.xy, v5.xy, c12.xy  \n" // Calculate detail texcoords
				;
}

SHADER_PCODE(Detail, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	// Detail      Fc = (Bc*Dc)+(Bc*Dc)
	// Translucent Fc = (Bc*Ba), Fa = (1-Ba)
	
	ASSERT(iPixelProgram==iDetailPP || iPixelProgram==iTranslucentPP);
	if(eFogType==FT_NONE) 
	{
		// if this is detail pixel shader
		if(iPixelProgram==iDetailPP) 
		{
/*			strPPCode = "tex    t0                   \n" // load base texture
						"tex    t1                   \n" // load detail texture
						"mul    t0,     t0,    c0    \n" // mul base texture with base color
						"mul    t1,     t1,    c1    \n" // mul detail texture with detail color
						"mul_x2 t0,     t0,    v0    \n" // shade pixel
						"mul_x2 r0,     t0,    t1    \n" // combine textures
						;*/
			// else if this is base pixel shader
			strPPCode = "texld	r0,    t0                   \n" // load base texture
						"texld	r1,    t1                   \n" // load detail texture
						"mul    r2,     r0,    c0    \n" // mul base texture with base color
						"mul    r1,     r1,    c1    \n" // mul detail texture with detail color
						"mul_x2 r2,     r2,    v0    \n" // shade pixel
						"mul_x2 r0,     r2,    r1    \n" // combine textures
						;
		}
		else if(iPixelProgram==iTranslucentPP) 
		{
/*			strPPCode = "tex    t0                   \n" // load base texture
						"mul    t0,     t0,    c0    \n" // mul base texture with base color
						"mul    r0.rgb, t0,    t0.a  \n" // Fc=Bc*Ba
						"mul_x2 r0.rgb, r0,    v0    \n" // Shade pixel
						"+mov   r0.a,   1-t0.a       \n" // Fa = (1-Ba)
						;*/
			strPPCode = "texld	r0,	    t0                   \n" // load base texture
						"mul    r1,     r0,    c0    \n" // mul base texture with base color
						"mul    r0.rgb, r1,    r1.a  \n" // Fc=Bc*Ba
						"mul_x2 r0.rgb, r0,    v0    \n" // Shade pixel
						"+mov   r0.a,   1-r1.a       \n" // Fa = (1-Ba)
						;
		}
	}
	else if(eFogType==FT_OPAQUE) 
	{
		ASSERT(iPixelProgram==iDetailPP);
/*		strPPCode = "tex    t0                     \n" // load base texture
					"tex    t1                     \n" // load detail texture
					"tex    t2                     \n" // load fog texture
					"mul    t0,     t0,    c0      \n" // mul base texture with base color
					"mul    t1,     t1,    c1      \n" // mul detail texture with detail color
					"mul    t2,     t2,    c7      \n" // mul fog texture with fog color
					"mul_x2 t0,     t0,    v0      \n" // Shade pixel
					"mul_x2 r0,     t0,    t1      \n" // Combine textures
					"lrp    r0.rgb, t2.a,  t2,  r0 \n" // Add fog
					;*/
		strPPCode = "texld  r0,    t0              \n" // load base texture
					"texld  r1,    t1              \n" // load detail texture
					"texld  r2,    t2              \n" // load fog texture
					"mul    r3,     r0,    c0      \n" // mul base texture with base color
					"mul    r1,     r1,    c1      \n" // mul detail texture with detail color
					"mul    r2,     r2,    c7      \n" // mul fog texture with fog color
					"mul_x2 r3,     r3,    v0      \n" // Shade pixel
					"mul_x2 r0,     r3,    r1      \n" // Combine textures
					"lrp    r0.rgb, r2.a,  r2,  r0 \n" // Add fog
					;
	}
	else if(eFogType==FT_NON_OPAQUE) 
	{
		ASSERT(iPixelProgram==iTranslucentPP);
/*		strPPCode = "tex    t0                     \n" // load base texture
					"tex    t2                     \n" // load fog texture
					"mul    t0,     t0,    c0      \n" // mul base texture with base color
					"mul    t2,     t2,    c7      \n" // mul fog texture with fog color
					"mul    t0.a,   t0,    1-t2.a  \n" // attenuate base tex alpha with fog alpha
					"mul    r0.rgb, t0,    t0.a    \n" // Fc = Bc*Ba
					"mul_x2 r0.rgb, r0,    v0      \n" // Shade pixel
					"+mov   r0.a,   1-t0.a         \n" // Fa = (1-Ba)
					;*/
		strPPCode = "texld	r0,		t0             \n" // load base texture
					"texld	r2,		t2             \n" // load fog texture
					"mul    r1,     r0,    c0      \n" // mul base texture with base color
					"mul    r2,     r2,    c7      \n" // mul fog texture with fog color
					"mul    r1.a,   r1,    1-r2.a  \n" // attenuate base tex alpha with fog alpha
					"mul    r0.rgb, r1,    r1.a    \n" // Fc = Bc*Ba
					"mul_x2 r0.rgb, r0,    v0      \n" // Shade pixel
					"+mov   r0.a,   1-r1.a         \n" // Fa = (1-Ba)
					;
	}
}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
