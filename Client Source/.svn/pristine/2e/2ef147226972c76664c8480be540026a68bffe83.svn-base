#include "StdH.h"

/*
#define TEXTURE_COUNT 2
#define UVMAPS_COUNT  1
#define COLOR_COUNT   2
#define FLOAT_COUNT   4
#define FLAGS_COUNT   2

#define BASE_TEXTURE 0
#define BASE_UVMAP   0
#define BASE_COLOR   0

#define DETAIL_TEXTURE 1
//#define DETAIL_UVMAP   1
#define DETAIL_COLOR   1
//#define DETAIL_TILING  0

#define BASE_DOUBLE_SIDED (1UL<<0) // Double sided
#define BASE_FULL_BRIGHT  (1UL<<1) // Full bright

#define FLOAT_UVMAPF    0
#define FLOAT_AMPLITUDE 1
#define FLOAT_RIPPLES   2
#define FLOAT_FREQUENCY 3
*/
static const INDEX ctTextures = 2;
static const INDEX ctUVMaps   = 1;
static const INDEX ctColors   = 2;
static const INDEX ctFloats   = 4;
static const INDEX ctFlags    = 2;

static const INDEX iBaseTexture = 1;
static const INDEX iBaseUVMap   = 0;
static const INDEX iBaseColor   = 0;
static const INDEX iDetailTexture = 0;
static const INDEX iDetailUVMap   = 1;
static const INDEX iDetailColor   = 1;
static const INDEX iDetailFloat   = 0;

static const INDEX iUVMapFloat     = 0;
static const INDEX iAmplitudeFloat = 1;
static const INDEX iRipplesFloat   = 2;
static const INDEX iFrequencyFloat = 3;

SHADER_MAIN(LavaDisplace)
{
	// Set texture wrapping
	shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
	// Set depth test
	shaEnableDepthTest();
	shaDepthFunc(GFX_LESS_EQUAL);
	// Set alpha test
	shaDisableAlphaTest();

	shaSetTexture(iBaseTexture);
	shaSetUVMap(iBaseUVMap);
	shaSetColor(iBaseColor);

	COLOR colModelColor = MulColors(shaGetModelColor(),shaGetCurrentColor());
	BOOL bDoubleSided = shaGetFlags()&BASE_DOUBLE_SIDED;
	BOOL bOpaque = (colModelColor&0xFF)==0xFF;

	// enable overbrightning
	shaEnableOverBrightning();
	// calculate shading
	shaCalculateLight();
	// prepare fog and haze
	shaPrepareFogAndHaze(bOpaque);

	// if surface is opaque
	if(bOpaque) {
		// disable blend
		shaDisableBlend();
		// enable depth write
		shaEnableDepthWrite();
		// if double sided flag is on
		if(bDoubleSided) {
			// no face culling 
			shaCullFace(GFX_NONE);
		} else {
			// back face culling
			shaCullFace(GFX_BACK);
		}
	} else { // else surface is translucent
		// Set blend mode
		shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
		shaEnableBlend();
		shaDisableDepthWrite();

		// Adjust alpha for fog
		shaAttenuateAlphaForFogAndHaze();

		// if double sided
		if(bDoubleSided) {
			// render back side
			shaCullFace(GFX_FRONT);
			shaRender();
		}
		// back face culling
		shaCullFace(GFX_BACK);
	}
	// render front side
	shaRender();
	// disable overbrighning
	shaDisableOverBrightning();


	/* Do detail pass */

	// Set blend mode
	shaBlendFunc( GFX_DST_COLOR, GFX_SRC_COLOR);
	shaEnableBlend();

	shaSetTexture(iDetailTexture);
	shaSetUVMap(iDetailUVMap);
	shaSetColor(iDetailColor);

	// prepare detail texcoords
	FLOAT fMul = shaGetFloat(iDetailFloat);
	GFXTexCoord *ptxcOld = shaGetUVMap(0);
	GFXTexCoord *ptxcNew = shaGetNewTexCoordArray();
	INDEX cttxc = shaGetVertexCount();
	if(cttxc>0) {
		for(INDEX itxc=0;itxc<cttxc;itxc++) {
			ptxcNew[itxc].u = ptxcOld[itxc].u * fMul;
			ptxcNew[itxc].v = ptxcOld[itxc].v * fMul;
		}
		// set new tex coords
		shaSetTexCoordArray(ptxcNew);
		// render detail pass
		shaRender();
	}


	// if surface is opaque
	if(bOpaque) {
		// render fog and haze pass for surface
		shaDoFogAndHazePass();
	}
}

/*
SHADER_MAIN(LavaDisplace)
{
	shaSetTexture(BASE_TEXTURE);
	shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
	shaSetUVMap(BASE_UVMAP);
	shaSetColor(BASE_COLOR);
	shaEnableDepthTest();
	shaDepthFunc(GFX_LESS_EQUAL);

	COLOR &colModelColor = shaGetModelColor();
	BOOL bDoubleSided = shaGetFlags()&BASE_DOUBLE_SIDED;
	BOOL bOpaque = (colModelColor&0xFF)==0xFF;

	if(bDoubleSided) {
		shaCullFace(GFX_NONE);
	} else {
		shaCullFace(GFX_BACK);
	}

	shaCalculateLight();

	// if fully opaque
	if(bOpaque) {
		// shaEnableAlphaTest(TRUE);
		shaDisableBlend();
		shaEnableDepthWrite();
	// if translucent
	} else {
		// shaEnableAlphaTest(FALSE);
		shaEnableBlend();
		shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
		shaDisableDepthWrite();
	}

	shaEnableOverBrightning();
	
	// displace geometry
	GFXVertex *paVertices    = shaGetVertexArray();
	GFXVertex *paNewVertices = shaGetNewVertexArray();
	INDEX ctVertices = shaGetVertexCount();
	// get some values, but clamp them
	FLOAT fAmplitude = Clamp(shaGetFloat(FLOAT_AMPLITUDE), 0.0f, 0.75f);
	FLOAT fRipples   = shaGetFloat(FLOAT_RIPPLES);  
	FLOAT fFrequency = shaGetFloat(FLOAT_FREQUENCY);  
	Matrix12 &mInvAbsToViewer = *shaGetObjToAbsMatrix();
	Matrix12 mAbsToView;
	MatrixTranspose(mAbsToView, mInvAbsToViewer);
	 
	// for each vertex
	for(INDEX ivx=0; ivx<ctVertices; ivx++) {
		paNewVertices[ivx] = paVertices[ivx];
		
		TransformVertex(paNewVertices[ivx],mInvAbsToViewer);
		paNewVertices[ivx].x *= 1.0f + fAmplitude * sin((paNewVertices[ivx].y+(_pTimer->GetLerpedCurrentTick()*fFrequency))*fRipples);
		paNewVertices[ivx].z *= 1.0f + fAmplitude * sin((paNewVertices[ivx].y+(_pTimer->GetLerpedCurrentTick()*fFrequency))*fRipples);
		//paNewVertices[ivx].x += fAmplitude * sin((paNewVertices[ivx].y+(_pTimer->GetLerpedCurrentTick()*fFrequency))*fRipples);
		//paNewVertices[ivx].y += vDisplace.y;
		//paNewVertices[ivx].z += vDisplace.z;
		TransformVertex(paNewVertices[ivx],mAbsToView);
	}
	shaSetVertexArray(paNewVertices, ctVertices);
	
	FLOAT fMul = shaGetFloat(FLOAT_UVMAPF);
	GFXTexCoord *ptxcOld = shaGetUVMap(0);
	GFXTexCoord *ptxcNew = shaGetNewTexCoordArray();
	INDEX ctTexCoords = shaGetVertexCount();
	if(ctTexCoords>0) {
		for(INDEX itxc=0;itxc<ctTexCoords;itxc++)
		{
			ptxcNew[itxc].u = ptxcOld[itxc].u * fMul;
			ptxcNew[itxc].v = ptxcOld[itxc].v * fMul;
		}
		shaSetTexCoords(ptxcNew);
	}

	shaRender();

	// do detail pass
	//FLOAT fMul = shaGetFloat(FLOAT_UVMAPF);
	shaBlendFunc( GFX_DST_COLOR, GFX_SRC_COLOR);
	shaSetTexture(DETAIL_TEXTURE);
	shaSetUVMap(BASE_UVMAP);
	shaSetColor(DETAIL_COLOR);
	shaCalculateLight();

	shaEnableBlend();

	#if 0
	GFXTexCoord *ptxcOld = shaGetUVMap(0);
	GFXTexCoord *ptxcNew = shaGetNewTexCoordArray();
	INDEX ctTexCoords = shaGetVertexCount();
	if(ctTexCoords>0) {
		for(INDEX itxc=0;itxc<ctTexCoords;itxc++)
		{
			ptxcNew[itxc].u = ptxcOld[itxc].u * fMul;
			ptxcNew[itxc].v = ptxcOld[itxc].v * fMul;
		}
		shaSetTexCoords(ptxcNew);
	}
	#endif
	shaRender();
	shaDisableBlend();

	shaDisableOverBrightning();

	#if 0
	if(bOpaque) {
		shaDoFogAndHazePass(SSTT_OPAQUE);
	} else {
		shaDoFogAndHazePass(SSTT_TRANSLUCENT);
	}
	#endif
}
*/
//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(LavaDisplace, ShaderDesc *&pshDesc)
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

		shDescMe.sd_astrTextureNames[0]  = "Base texture";
		shDescMe.sd_astrTextureNames[1]  = "Detail texture";
		shDescMe.sd_astrTexCoordNames[0] = "Base UVMap";
		shDescMe.sd_astrColorNames[0] = "Surface color";
		shDescMe.sd_astrColorNames[1] = "Detail color";

		shDescMe.sd_astrFlagNames[0] = "Double sided";
		shDescMe.sd_astrFlagNames[1] = "Full bright";

		shDescMe.sd_astrFloatNames[iUVMapFloat] = "UVMap factor";
		shDescMe.sd_astrFloatNames[iAmplitudeFloat] = "Amp (max 0.75)";
		shDescMe.sd_astrFloatNames[iRipplesFloat]   = "Ripple density";
		shDescMe.sd_astrFloatNames[iFrequencyFloat] = "Ripple speed";
		shDescMe.sd_strShaderInfo = "Detail shader";
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}
 
SHADER_VCODE(LavaDisplace, CTString &strVPCode, INDEX iVertexProgram)
{
	ASSERT(FALSE);
}

SHADER_PCODE(LavaDisplace, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	ASSERT(FALSE);
}
