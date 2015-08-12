#include "stdh.h"
#include <Engine/Math/Projection.h>
#include <Engine/Math/Matrix12.h>
#include <Engine/Math/OBBox.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Graphics/Shader.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Terrain/TRLayer.h>
#include <Engine/Terrain/TRLayer.hpp>
#include <Engine/Terrain/TRRayCast.hpp>
#include <Engine/Terrain/TRGenerate.hpp>
#include <Engine/Terrain/TRMisc.hpp>
#include <Engine/Terrain/TREditing.hpp>
#include <Engine/Rendering/Render.h>
#include <Engine/Graphics/Fog_internal.h>
#include <Engine/Templates/Stock_CTextureData.h>

//강동민 수정 시작
#include <Engine/Rendering/Render_internal.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/SKA/Render.h>
//강동민 수정 끝

#include <Engine/Base/Console.h>

static CAnyProjection3D _aprProjection;
static CDrawPort *_pdp = NULL;

#include <Engine/Light/LightSource.h>							// yjpark
#include <list>													// yjpark
extern std::list<CLightSource *>	_lstTerrainLightSource;		// yjpark

//안태훈 수정 시작	//(For Performance)(0.1)
EXTERN_SHADER_MAIN(TRShader_Base);
EXTERN_SHADER_DESC(TRShader_Base, ShaderDesc *&pshDesc);
EXTERN_SHADER_VCODE(TRShader_Base, CTString &strVPCode, INDEX iVertexProgram);
EXTERN_SHADER_PCODE(TRShader_Base, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType);

EXTERN_SHADER_MAIN(TRShader_1TRL);
EXTERN_SHADER_DESC(TRShader_1TRL, ShaderDesc *&pshDesc);
EXTERN_SHADER_VCODE(TRShader_1TRL, CTString &strVPCode, INDEX iVertexProgram);
EXTERN_SHADER_PCODE(TRShader_1TRL, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType);

EXTERN_SHADER_MAIN(TRShader_2TRL);
EXTERN_SHADER_DESC(TRShader_2TRL, ShaderDesc *&pshDesc);
EXTERN_SHADER_VCODE(TRShader_2TRL, CTString &strVPCode, INDEX iVertexProgram);
EXTERN_SHADER_PCODE(TRShader_2TRL, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType);

EXTERN_SHADER_MAIN( TRShader_Shadow );																	// yjpark |<--
EXTERN_SHADER_DESC( TRShader_Shadow, ShaderDesc *&pshDesc );
EXTERN_SHADER_VCODE( TRShader_Shadow, CTString &strVPCode, INDEX iVertexProgram );
EXTERN_SHADER_PCODE( TRShader_Shadow, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType );		// yjpark     -->|

EXTERN_SHADER_MAIN(TRShader_Fog);
EXTERN_SHADER_DESC(TRShader_Fog, ShaderDesc *&pshDesc);
EXTERN_SHADER_VCODE(TRShader_Fog, CTString &strVPCode, INDEX iVertexProgram);
EXTERN_SHADER_PCODE(TRShader_Fog, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType);
//안태훈 수정 끝	//(For Performance)(0.1)

// terrain base shader
static CShader _shTRShader_Base(FUNC_SHADER_MAIN(TRShader_Base), FUNC_SHADER_DESC(TRShader_Base),
																FUNC_SHADER_VCODE(TRShader_Base), FUNC_SHADER_PCODE(TRShader_Base));
// terrain shader with one layer
static CShader _shTRShader_1TRL(FUNC_SHADER_MAIN(TRShader_1TRL), FUNC_SHADER_DESC(TRShader_1TRL),
																FUNC_SHADER_VCODE(TRShader_1TRL), FUNC_SHADER_PCODE(TRShader_1TRL));
// terrain shader with two layers
static CShader _shTRShader_2TRL(FUNC_SHADER_MAIN(TRShader_2TRL), FUNC_SHADER_DESC(TRShader_2TRL),
																FUNC_SHADER_VCODE(TRShader_2TRL), FUNC_SHADER_PCODE(TRShader_2TRL));
// terrain shadow shader																																// yjpark |<--
static CShader	_shTRShader_Shadow( FUNC_SHADER_MAIN( TRShader_Shadow ), FUNC_SHADER_DESC( TRShader_Shadow ),
																		FUNC_SHADER_VCODE( TRShader_Shadow ), FUNC_SHADER_PCODE( TRShader_Shadow ) );	// yjpark     -->|
// terrain fog shader
static CShader _shTRShader_Fog(FUNC_SHADER_MAIN(TRShader_Fog), FUNC_SHADER_DESC(TRShader_Fog),
																FUNC_SHADER_VCODE(TRShader_Fog), FUNC_SHADER_PCODE(TRShader_Fog));

// Terrain transformation matrices
static Matrix12 _mStretch          = {0};
static Matrix12 _mAbsToViewer      = {0};
static Matrix12 _mObjToAbs         = {0};
static Matrix12 _mObjToView        = {0};
static Matrix12 _mObjToViewStretch = {0};
static Matrix12 _mObjToAbsStretch  = {0};

static BOOL    _bUseShadowMap       = TRUE;  // Use shadow map
static INDEX   _iForcedLOD          = -1;    // Use forced LOD
static BOOL    _bUseHardwareShaders = FALSE; // use vertex program
static CStaticStackArray<INDEX> _aiVisibleTiles;            // Array of tiles that are visible
static CStaticStackArray<TerrainViewer> _atvTerrainViewers; // Array of terrain viewers
static CStaticStackArray<GFXVertex>   _avxTileVertices;     // Array of tile vertices prepared for rendering
static CStaticStackArray<GFXTexCoord> _atcTileTexCoords;    // Array of tile texcoords prepared for rendering
static CStaticStackArray<GFXVertex>   _avxBatchVertices;    // Array of batched vertices prepared for rendering
static CStaticStackArray<GFXTexCoord> _atcBatchTexCoords;   // Array of batched texcoords prepared for rendering
static CStaticStackArray<UWORD>       _auwBatchIndices;   // Array of batched triangle indices prepared for rendering

// Fog and haze
static FLOAT3D _vViewerObj;
static FLOAT3D _vFViewerObj, _vHDirObj;
static FLOAT   _fFogAddZ, _fFogAddH;
static FLOAT   _fHazeAdd;
static CStaticStackArray<struct GFXColor>    _acolFog;  // fog colors
static CStaticStackArray<struct GFXTexCoord> _atcFog;   // fog tex coords
static CStaticStackArray<struct GFXColor>    _acolHaze; // haze colors
static CStaticStackArray<struct GFXTexCoord> _atcHaze;  // haze tex coords

static const BOOL ter_bShowShadowMap = TRUE;

// Debuging info
extern TerrainInfo _tiTerrainInfo = {0};

static void UnpackTileVertices(CTerrainImp *ptr, INDEX iTileIndex)
{
	const CTerrainTile &ttTile = ptr->tr_attTiles[iTileIndex];
	const INDEX cttvx = ttTile.tt_avxVertices.Count();

	_avxTileVertices.PopAll();
	_atcTileTexCoords.PopAll();
	_avxTileVertices.Push(cttvx);
	_atcTileTexCoords.Push(cttvx);
	
	// Lock vertices for read
	ptr->tr_attTiles[iTileIndex].tt_avxVertices.Lock(TDAF_LOCK_READ);

	const INDEX iLOD = ttTile.tt_iCurrentLOD;
	const FLOAT fLerpFactor = ttTile.tt_fLODLerpFactor;
	const INDEX ittTopNB = ttTile.tt_aiNeighbours[NB_TOP];
	const INDEX ittLeftNB = ttTile.tt_aiNeighbours[NB_LEFT];
	const INDEX ittRightNB = ttTile.tt_aiNeighbours[NB_RIGHT];
	const INDEX ittBottomNB = ttTile.tt_aiNeighbours[NB_BOTTOM];

	// Colect lerp factors
	FLOAT afLerpFactors[10];
	afLerpFactors[VLF_CTLF] = fLerpFactor;
	afLerpFactors[VLF_NOLERP] = 0.0f;

	if(ittTopNB>=0) {
		const CTerrainTile &ttTop = ptr->tr_attTiles[ittTopNB];
		const INDEX iTopLOD = ttTop.tt_iCurrentLOD;
		const FLOAT fTopLerpFactor = ttTop.tt_fLODLerpFactor;
		afLerpFactors[VLF_TTLF] = fTopLerpFactor;
		if(iTopLOD==iLOD) {
			afLerpFactors[VLF_TCTLF] = Min(fLerpFactor,fTopLerpFactor);
		}
	}
	if(ittLeftNB>=0) {
		const CTerrainTile &ttLeft = ptr->tr_attTiles[ittLeftNB];
		const INDEX iLeftLOD = ttLeft.tt_iCurrentLOD;
		const FLOAT fLeftLerpFactor = ttLeft.tt_fLODLerpFactor;
		afLerpFactors[VLF_LTLF] = fLeftLerpFactor;
		if(iLeftLOD==iLOD) {
			afLerpFactors[VLF_LCTLF] = Min(fLerpFactor,fLeftLerpFactor);
		}
	}
	if(ittRightNB>=0) {
		const CTerrainTile &ttRight = ptr->tr_attTiles[ittRightNB];
		const FLOAT fRightLerpFactor = ttRight.tt_fLODLerpFactor;
		const INDEX iRightLOD = ttRight.tt_iCurrentLOD;
		afLerpFactors[VLF_RTLF] = fRightLerpFactor;
		if(iRightLOD==iLOD) {
			afLerpFactors[VLF_RCTLF] = Min(fLerpFactor,fRightLerpFactor);
		}
	}
	if(ittBottomNB>=0) {
		const CTerrainTile &ttBottom = ptr->tr_attTiles[ittBottomNB];
		const FLOAT fBottomLerpFactor = ttBottom.tt_fLODLerpFactor;
		const INDEX iBottomLOD = ttBottom.tt_iCurrentLOD;
		afLerpFactors[VLF_BTLF] = fBottomLerpFactor;
		if(iBottomLOD==iLOD) {
			afLerpFactors[VLF_BCTLF] = Min(fLerpFactor,fBottomLerpFactor);
		}
	}

	// For each vertex
	for(INDEX ivx=0;ivx<cttvx;ivx++) 
	{
		const TerrainVertex &tvx = ttTile.tt_avxVertices[ivx];
		GFXVertex &vx = _avxTileVertices[ivx];
		GFXTexCoord &tc = _atcTileTexCoords[ivx];
		ASSERT(afLerpFactors[tvx.tv_ubLerp]>=0.0f && afLerpFactors[tvx.tv_ubLerp]<=1.0f);

		const FLOAT fX  = tvx.tv_ubX;
		const FLOAT fZ  = tvx.tv_ubZ;
		const FLOAT fY1 = tvx.tv_uwY;
		const FLOAT fY2 = tvx.tv_uwY2;
		const FLOAT fLerpFactor = afLerpFactors[tvx.tv_ubLerp];
		const FLOAT fY = Lerp(fY1,fY2,fLerpFactor);
		vx.x = fX;
		vx.z = fZ;
		vx.y = fY;
		tc.u = fX;
		tc.v = fZ;
	}

	// Unlock vertices
	ptr->tr_attTiles[iTileIndex].tt_avxVertices.Unlock();
}

static void UnpackTileVerticesBatch(CTerrainImp *ptr, INDEX iTileIndex)
{
	const CTerrainTile &ttTile = ptr->tr_attTiles[iTileIndex];
	const INDEX cttvx = ttTile.tt_avxVertices.Count();
	const INDEX cttri = ttTile.tt_atriTriangles.Count();
	const INDEX ctind = cttri*3;

	const INDEX iFirstVertex = _avxBatchVertices.Count();
	const INDEX iFirstIndex  = _auwBatchIndices.Count();

	_avxBatchVertices.Push(cttvx);
	_atcBatchTexCoords.Push(cttvx);
	_auwBatchIndices.Push(ctind);
	
	// Lock vertices for read
	ptr->tr_attTiles[iTileIndex].tt_avxVertices.Lock(TDAF_LOCK_READ);

	const INDEX iLOD = ttTile.tt_iCurrentLOD;
	const FLOAT fLerpFactor = ttTile.tt_fLODLerpFactor;
	const INDEX ittTopNB = ttTile.tt_aiNeighbours[NB_TOP];
	const INDEX ittLeftNB = ttTile.tt_aiNeighbours[NB_LEFT];
	const INDEX ittRightNB = ttTile.tt_aiNeighbours[NB_RIGHT];
	const INDEX ittBottomNB = ttTile.tt_aiNeighbours[NB_BOTTOM];

	// Colect lerp factors
	FLOAT afLerpFactors[10];
	afLerpFactors[VLF_CTLF] = fLerpFactor;
	afLerpFactors[VLF_NOLERP] = 0.0f;

	if(ittTopNB>=0) {
		const CTerrainTile &ttTop = ptr->tr_attTiles[ittTopNB];
		const INDEX iTopLOD = ttTop.tt_iCurrentLOD;
		const FLOAT fTopLerpFactor = ttTop.tt_fLODLerpFactor;
		afLerpFactors[VLF_TTLF] = fTopLerpFactor;
		if(iTopLOD==iLOD) {
			afLerpFactors[VLF_TCTLF] = Min(fLerpFactor,fTopLerpFactor);
		}
	}
	if(ittLeftNB>=0) {
		const CTerrainTile &ttLeft = ptr->tr_attTiles[ittLeftNB];
		const INDEX iLeftLOD = ttLeft.tt_iCurrentLOD;
		const FLOAT fLeftLerpFactor = ttLeft.tt_fLODLerpFactor;
		afLerpFactors[VLF_LTLF] = fLeftLerpFactor;
		if(iLeftLOD==iLOD) {
			afLerpFactors[VLF_LCTLF] = Min(fLerpFactor,fLeftLerpFactor);
		}
	}
	if(ittRightNB>=0) {
		const CTerrainTile &ttRight = ptr->tr_attTiles[ittRightNB];
		const FLOAT fRightLerpFactor = ttRight.tt_fLODLerpFactor;
		const INDEX iRightLOD = ttRight.tt_iCurrentLOD;
		afLerpFactors[VLF_RTLF] = fRightLerpFactor;
		if(iRightLOD==iLOD) {
			afLerpFactors[VLF_RCTLF] = Min(fLerpFactor,fRightLerpFactor);
		}
	}
	if(ittBottomNB>=0) {
		const CTerrainTile &ttBottom = ptr->tr_attTiles[ittBottomNB];
		const FLOAT fBottomLerpFactor = ttBottom.tt_fLODLerpFactor;
		const INDEX iBottomLOD = ttBottom.tt_iCurrentLOD;
		afLerpFactors[VLF_BTLF] = fBottomLerpFactor;
		if(iBottomLOD==iLOD) {
			afLerpFactors[VLF_BCTLF] = Min(fLerpFactor,fBottomLerpFactor);
		}
	}

	const FLOAT fTransX = ttTile.tt_mTileToTerrain[ 3];
	const FLOAT fTransZ = ttTile.tt_mTileToTerrain[11];

	// For each vertex
	for(INDEX ivx=0;ivx<cttvx;ivx++) {
		const TerrainVertex &tvx = ttTile.tt_avxVertices[ivx];
		GFXVertex &vx = _avxBatchVertices[iFirstVertex + ivx];
		GFXTexCoord &tc = _atcBatchTexCoords[iFirstVertex + ivx];
		ASSERT(afLerpFactors[tvx.tv_ubLerp]>=0.0f && afLerpFactors[tvx.tv_ubLerp]<=1.0f);

		const FLOAT fX  = tvx.tv_ubX;
		const FLOAT fZ  = tvx.tv_ubZ;
		const FLOAT fY1 = tvx.tv_uwY;
		const FLOAT fY2 = tvx.tv_uwY2;
		const FLOAT fLerpFactor = afLerpFactors[tvx.tv_ubLerp];
		const FLOAT fY = Lerp(fY1,fY2,fLerpFactor);
		vx.x = fX + fTransX;
		vx.z = fZ + fTransZ;
		vx.y = fY;
		tc.u = fX + fTransX;
		tc.v = fZ + fTransZ;
	}

	// Copy indices
	for(INDEX itri=0;itri<cttri;itri++) {
		const TerrainTriangle &tt = ttTile.tt_atriTriangles[itri];
		const INDEX iind = itri*3;
		_auwBatchIndices[iFirstIndex+iind+0] = tt.trt_auwIndices[0] + iFirstVertex;
		_auwBatchIndices[iFirstIndex+iind+1] = tt.trt_auwIndices[1] + iFirstVertex;
		_auwBatchIndices[iFirstIndex+iind+2] = tt.trt_auwIndices[2] + iFirstVertex;
	}

	// Unlock vertices
	ptr->tr_attTiles[iTileIndex].tt_avxVertices.Unlock();
}

static void DrawWireBox(const FLOATaabbox3D &bbox, COLOR col, const Matrix12 &mTransf)
{
	FLOAT3D vMinVtx = bbox.Min();
	FLOAT3D vMaxVtx = bbox.Max();
	// fill vertex array so it represents bounding box
	FLOAT3D vBoxVtxs[8];
	vBoxVtxs[0] = FLOAT3D( vMinVtx(1), vMinVtx(2), vMinVtx(3));
	vBoxVtxs[1] = FLOAT3D( vMaxVtx(1), vMinVtx(2), vMinVtx(3));
	vBoxVtxs[2] = FLOAT3D( vMaxVtx(1), vMinVtx(2), vMaxVtx(3));
	vBoxVtxs[3] = FLOAT3D( vMinVtx(1), vMinVtx(2), vMaxVtx(3));
	vBoxVtxs[4] = FLOAT3D( vMinVtx(1), vMaxVtx(2), vMinVtx(3));
	vBoxVtxs[5] = FLOAT3D( vMaxVtx(1), vMaxVtx(2), vMinVtx(3));
	vBoxVtxs[6] = FLOAT3D( vMaxVtx(1), vMaxVtx(2), vMaxVtx(3));
	vBoxVtxs[7] = FLOAT3D( vMinVtx(1), vMaxVtx(2), vMaxVtx(3));

	// connect vertices into lines of bounding box
	INDEX iBoxLines[12][2];
	iBoxLines[ 0][0] = 0;  iBoxLines[ 0][1] = 1;  iBoxLines[ 1][0] = 1;  iBoxLines[ 1][1] = 2;
	iBoxLines[ 2][0] = 2;  iBoxLines[ 2][1] = 3;  iBoxLines[ 3][0] = 3;  iBoxLines[ 3][1] = 0;
	iBoxLines[ 4][0] = 0;  iBoxLines[ 4][1] = 4;  iBoxLines[ 5][0] = 1;  iBoxLines[ 5][1] = 5;
	iBoxLines[ 6][0] = 2;  iBoxLines[ 6][1] = 6;  iBoxLines[ 7][0] = 3;  iBoxLines[ 7][1] = 7;
	iBoxLines[ 8][0] = 4;  iBoxLines[ 8][1] = 5;  iBoxLines[ 9][0] = 5;  iBoxLines[ 9][1] = 6;
	iBoxLines[10][0] = 6;  iBoxLines[10][1] = 7;  iBoxLines[11][0] = 7;  iBoxLines[11][1] = 4;
	// for all vertices in bounding box
	for( INDEX i=0; i<12; i++) {
		// get starting and ending vertices of one line
		FLOAT3D v0 = vBoxVtxs[iBoxLines[i][0]];
		FLOAT3D v1 = vBoxVtxs[iBoxLines[i][1]];
		TransformVector(v0.vector,mTransf);
		TransformVector(v1.vector,mTransf);
		_pdp->DrawLine3D(v0,v1,col);
	} 
}

// render bounding box
static void DrawFullBox(const FLOATaabbox3D &bbox, COLOR col, const Matrix12 &mTransf)
{
	FLOAT3D vMinVtx = bbox.Min();
	FLOAT3D vMaxVtx = bbox.Max();
	// prepare settings
	gfxDisableTexture();
	gfxEnableBlend();
	gfxBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
	gfxCullFace(GFX_NONE);
	gfxEnableDepthTest();
	// gfxEnableDepthWrite();
	// gfxDisableBlend();
	gfxDisableDepthWrite();

	GFXColor colArray[8];
	colArray[0] = col; colArray[1] = col; colArray[2] = col;
	colArray[3] = col; colArray[3] = col; colArray[4] = col;
	colArray[5] = col; colArray[6] = col; colArray[7] = col;
	// gfxSetConstantColor(col);
	// fill vertex array so it represents bounding box
	GFXVertex vBoxVtxs[8];
	vBoxVtxs[0].x = vMinVtx(1);  vBoxVtxs[0].y = vMaxVtx(2);  vBoxVtxs[0].z = vMinVtx(3);
	vBoxVtxs[1].x = vMinVtx(1);  vBoxVtxs[1].y = vMaxVtx(2);  vBoxVtxs[1].z = vMaxVtx(3);
	vBoxVtxs[2].x = vMaxVtx(1);  vBoxVtxs[2].y = vMaxVtx(2);  vBoxVtxs[2].z = vMinVtx(3);
	vBoxVtxs[3].x = vMaxVtx(1);  vBoxVtxs[3].y = vMaxVtx(2);  vBoxVtxs[3].z = vMaxVtx(3);
	vBoxVtxs[4].x = vMinVtx(1);  vBoxVtxs[4].y = vMinVtx(2);  vBoxVtxs[4].z = vMinVtx(3);
	vBoxVtxs[5].x = vMinVtx(1);  vBoxVtxs[5].y = vMinVtx(2);  vBoxVtxs[5].z = vMaxVtx(3);
	vBoxVtxs[6].x = vMaxVtx(1);  vBoxVtxs[6].y = vMinVtx(2);  vBoxVtxs[6].z = vMinVtx(3);
	vBoxVtxs[7].x = vMaxVtx(1);  vBoxVtxs[7].y = vMinVtx(2);  vBoxVtxs[7].z = vMaxVtx(3);

	const UWORD auwIndices[36] = {
		0,3,1, 0,2,3, 5,1,3, 7,5,3,
		2,7,3, 6,7,2, 4,2,0, 4,6,2,
		5,0,1, 5,4,0, 4,5,7, 6,4,7
	};

	for(INDEX iwx=0;iwx<8;iwx++) {
		TransformVector(&vBoxVtxs[iwx].x,mTransf);
	}

	gfxSetVertexArray( vBoxVtxs,8);
	gfxSetColorArray(&colArray[0]);
	gfxDrawElements( 36, auwIndices);

	gfxDisableBlend();
	gfxEnableDepthTest();

	DrawWireBox(bbox, C_BLACK|CT_OPAQUE,mTransf);
	gfxEnableDepthWrite();
	gfxDisableDepthBias();
}

static void DrawWireRect(const CTRect &rc, FLOAT fMinY, FLOAT fMaxY, COLOR col, const Matrix12 &mTransf)
{
	FLOAT fTop = rc.rc_slTop;
	FLOAT fLeft = rc.rc_slLeft;
	FLOAT fRight = rc.rc_slRight;
	FLOAT fBottom = rc.rc_slBottom;
	FLOAT3D vMin(fLeft,fMinY,fTop);
	FLOAT3D vMax(fRight,fMaxY,fBottom);
	DrawWireBox(FLOATaabbox3D(vMin,vMax),col,mTransf);
}

// Set view matrix
static void SetViewMatrix(const Matrix12 &mView12)
{
	FLOAT mView16[16];
	mView16[ 0] = mView12[ 0];  mView16[ 1] = mView12[ 4];  mView16[ 2] = mView12[ 8];  mView16[ 3] = 0;
	mView16[ 4] = mView12[ 1];  mView16[ 5] = mView12[ 5];  mView16[ 6] = mView12[ 9];  mView16[ 7] = 0;
	mView16[ 8] = mView12[ 2];  mView16[ 9] = mView12[ 6];  mView16[10] = mView12[10];  mView16[11] = 0;
	mView16[12] = mView12[ 3];  mView16[13] = mView12[ 7];  mView16[14] = mView12[11];  mView16[15] = 1;
	gfxSetViewMatrix(mView16);
}

ENGINE_API extern BOOL TR_AreHardwareShadersAvailable(void)
{
	extern INDEX ter_bUseHardwareShaders;
	return ter_bUseHardwareShaders && 
				 (_pGfx->gl_ulFlags&GLF_VERTEXPROGRAM) && 
				 (_pGfx->gl_ulFlags&GLF_PIXELPROGRAM) && 
				 (_pGfx->gl_ctTextureUnits>=4);
}

static void UpdateTerrainTileLOD(CTerrainImp *ptr, INDEX iTerrainTile)
{
	CTerrainTile &ttTile = ptr->tr_attTiles[iTerrainTile];
	const QuadTreeNode &qtn = ptr->tr_aqtnQuadTreeNodes[iTerrainTile];

	// Claculate distance to tile center

	INDEX iRequestedLOD = ptr->tr_iMaxTileLOD;
	FLOAT fLODLerpFactor = 1.0f;

	// for each viewer
	const INDEX cttv = _atvTerrainViewers.Count();
	for(INDEX itv=0;itv<cttv;itv++) {
		const TerrainViewer &tv= _atvTerrainViewers[itv];
		const FLOAT3D &vViewerAbs = tv.tv_vViewerAbs;
		const FLOAT fDistFactor = tv.tv_fDistFactor;
		FLOAT3D vTileCentar = qtn.qtn_aabbox.Center();
		FLOAT3D vTileSize = qtn.qtn_aabbox.Size();
		// Transform tile center to abs space
		TransformVector(vTileCentar.vector,_mObjToAbsStretch);
		TransformVector(vTileSize.vector,_mStretch);
		// use tile dist offset so current tile will not lerp
		const FLOAT fTileDistOffset = vTileSize.Length() / 2.0f;
		const FLOAT fTileDist = (vTileCentar-vViewerAbs).Length() - fTileDistOffset;

		// Calculate new tile lod
		const INDEX iCalculatedLOD = fTileDist/fDistFactor;
		const INDEX iClampedLOD = Clamp(iCalculatedLOD,0L,ptr->tr_iMaxTileLOD);
		const FLOAT fCalculatedLerpFactor = (fTileDist/fDistFactor) - iCalculatedLOD;
		const FLOAT fClampedLerpFactor = Clamp(fCalculatedLerpFactor,0.0f,1.0f);

		// if calculated LOD is higher than requested one
		if(iClampedLOD<iRequestedLOD) {
			// use calculated LOD and its lerp factor
			iRequestedLOD = iClampedLOD;
			fLODLerpFactor = fClampedLerpFactor;
		// if calculated LOD is same as requested one
		} else if(iRequestedLOD==iClampedLOD) {
			// use preciser lerp factor
			fLODLerpFactor = Min(fLODLerpFactor,fClampedLerpFactor);
		}
	}

	// if max tile LOD has been reached
	if(iRequestedLOD==ptr->tr_iMaxTileLOD) {
		// do not lerp vertices
		fLODLerpFactor = 0.0f;
	}

	// apply forced LOD if any
	if(_iForcedLOD!=-1) {
		iRequestedLOD = Clamp(_iForcedLOD,0L,ptr->tr_iMaxTileLOD);
		fLODLerpFactor = 0.0f;
	}
	// set tile requested lod
	ttTile.tt_iRequestedLOD = iRequestedLOD;
	ttTile.tt_fLODLerpFactor = fLODLerpFactor;
	
	// if tile lod has changed
	if(ttTile.tt_iRequestedLOD!=ttTile.tt_iCurrentLOD) {
		// Add tile to regen queue
		QueueTileRegeneration(ptr,iTerrainTile);
	}
}

static void UpdateQuadTreeNodeVisibility(CTerrainImp *ptr, INDEX iqtn)
{
	const QuadTreeNode &qtn = ptr->tr_aqtnQuadTreeNodes[iqtn];
	const FLOATaabbox3D &bbox = qtn.qtn_aabbox;

	FLOATmatrix3D mObjToView;
	FLOAT3D vObjToView;
	Matrix12ToMatrixVector(mObjToView,vObjToView,_mObjToViewStretch);
	
	// Create oobb for quad tree node
	const FLOATobbox3D obbox = FLOATobbox3D(bbox,vObjToView,mObjToView);
	// Test oobb in frustum
	const INDEX iFrustumTest = _aprProjection->TestBoxToFrustum(obbox);
	// if test passed
	if(iFrustumTest!=(-1)) {
		const INDEX iTerrainTile = qtn.qtn_iTileIndex;
		// if this is leaf node
		if(iTerrainTile!=(-1)) {
			
			const CTerrainTile &ttTile = ptr->tr_attTiles[iTerrainTile];
			// if tile has no edges
			if(ttTile.tt_ulFlags&TTF_BLANK_EDGE_MAP) {
				// skip it
				TR_SETINFO(ti_ctTileEdgeRejected++);
			// else 
			} else {
				// Add tile to array of visible tiles
				INDEX &iVisTileIndex = _aiVisibleTiles.Push();
				iVisTileIndex = iTerrainTile;
				TR_SETINFO(ti_ctVisibleNodes++);
			}
		// if not
		} else {
			// for each quad tree node child
			for(INDEX iqtnc=0;iqtnc<4;iqtnc++) {
				const INDEX iqtncIndex = qtn.qtn_aiChild[iqtnc];
				// if quad tree node child is valid
				if(iqtncIndex!=(-1)) {
					// Test for child node visibility
					UpdateQuadTreeNodeVisibility(ptr,iqtncIndex);
				}
			}
		}
	}
}

static void UpdateTilesVisibility(CTerrainImp *ptr)
{
//강동민 수정 시작 Water 구현		04.23
	//const INDEX ctTiles = ptr->tr_ctTilesX * ptr->tr_ctTilesY;		// 원본(쓰이지 않음.)
//강동민 수정 끝 Water 구현			04.23
	_aiVisibleTiles.PopAll();
	// _aiVisibleTiles.Push(ctTiles);
	// memset(&_aiVisibleTiles[0],0,sizeof(INDEX)*ctTiles);

	// Get last quad tree level
	const INDEX ctqtl = ptr->tr_aqtlQuadTreeLevels.Count();
	const QuadTreeLevel &qtlLast = ptr->tr_aqtlQuadTreeLevels[ctqtl-1];
	const INDEX ctqtn = qtlLast.qtl_ctNodes;
	// for each quad tree node in last level (should be only one)
	for(INDEX iqtn=0;iqtn<ctqtn;iqtn++) {
		const INDEX iqtnIndex = qtlLast.qtl_iFirstNode + iqtn;
		UpdateQuadTreeNodeVisibility(ptr,iqtnIndex);
	}
}

static void UpdateTerrainTiles(CTerrainImp *ptr)
{
	// for each terrain tile
	const INDEX cttt = ptr->tr_attTiles.Count();
	for(INDEX itt=0;itt<cttt;itt++) {
		CTerrainTile &tt = ptr->tr_attTiles[itt];
		// calculate tile lod
		UpdateTerrainTileLOD(ptr,itt);
	}
}

static void UpdateTerrainTileNeighbours(CTerrainImp *ptr)
{
	const INDEX aiReverseNB[4] = {
		NB_RIGHT,NB_LEFT,NB_BOTTOM,NB_TOP
	};

	// for each terrain tile in regen queue
	const INDEX ctrt = ptr->tr_artRegenQueue.Count();
	for(INDEX irt=0;irt<ctrt;irt++) {
		const RegenerateTile &rt = ptr->tr_artRegenQueue[irt];
		const INDEX iTerrainTile = rt.rt_iTileIndex;
		const CTerrainTile &ttTile = ptr->tr_attTiles[iTerrainTile];
		const INDEX iCurrentLOD = ttTile.tt_iCurrentLOD;
		const INDEX iRequestedLOD = ttTile.tt_iRequestedLOD;
		const BOOL bParentLODDiff = ttTile.tt_iCurrentLOD!=ttTile.tt_iRequestedLOD;

		// for each tile neighbour
		for(INDEX inb=0;inb<4;inb++) {
			const INDEX inbi = ttTile.tt_aiNeighbours[inb];
			// if tile neighbour is valid
			if(inbi>=0) {
				// Get neighbour tile
				const CTerrainTile &ttNeighbour = ptr->tr_attTiles[inbi];
				const INDEX iReverseNB = aiReverseNB[inb];
				// Get neighbour tile LOD
				const INDEX iNeighbourLOD = ttNeighbour.tt_iRequestedLOD;
				// Get LOD neighbour is linked to
				const INDEX iNBLinkedToLOD = ttNeighbour.tt_aiNBLODs[iReverseNB];

				// if borders must be updated
				const BOOL bDisconectNBs = iCurrentLOD<=iNeighbourLOD;
				const BOOL bConectNBs = iRequestedLOD<=iNeighbourLOD;
				if(bDisconectNBs||bConectNBs) {
					// regenerate neighbor tile
					QueueTileRegeneration(ptr,inbi,RTF_GEOMETRY,TRUE);
				}
			}
		}
	}
}

static void UpdateEditingData(CTerrain *ptrTerrain)
{
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	// Calculate obj to view matrix
	MatrixMultiply(_mObjToView,_mAbsToViewer, _mObjToAbs);
	MakeStretchMatrix(_mStretch,ptr->tr_vStretch);
	MatrixMultiply(_mObjToViewStretch,_mObjToView,_mStretch);
	MatrixMultiply(_mObjToAbsStretch,_mObjToAbs,_mStretch);

	extern CTRect _rcSelRectangle;
	FLOAT3D vViewer;
	const FLOATaabbox3D bboxStretch = TR_GetStretchedTerrainBBox(ptrTerrain);
	const FLOAT fMinY = bboxStretch.minvect(2);
	const FLOAT fMaxY = bboxStretch.maxvect(2);
	const FLOAT fPosY = (fMaxY-fMinY) / 2.0f;

	SLONG			slStretch = 0;
	extern BufferType	_btSelBufferType;
	if( _btSelBufferType == BT_LAYER_MASK || _btSelBufferType == BT_SHADOW_MAP )
	{
		if( _btSelBufferType == BT_LAYER_MASK )
		{
			INDEX	iSelLayer = ptrTerrain->tr_iSelectedLayer;
			slStretch = ptr->tr_atlLayers[iSelLayer].tl_slMaskStretch;
		}
		else if( _btSelBufferType == BT_SHADOW_MAP )
		{
			slStretch = ptr->tr_slShadowMapSizeAspect;
		}

		if( slStretch < 0 )
		{
			vViewer(1) = (_rcSelRectangle.rc_slLeft + _rcSelRectangle.GetWidth()/2) << (-slStretch);
			vViewer(2) = fPosY;
			vViewer(3) = (_rcSelRectangle.rc_slTop + _rcSelRectangle.GetHeight()/2) << (-slStretch);
		}
		else
		{
			vViewer(1) = (_rcSelRectangle.rc_slLeft + _rcSelRectangle.GetWidth()/2) >> slStretch;
			vViewer(2) = fPosY;
			vViewer(3) = (_rcSelRectangle.rc_slTop + _rcSelRectangle.GetHeight()/2) >> slStretch;
		}
	}
	else if( _btSelBufferType == BT_ATTRIBUTE_MAP )
	{
		slStretch = ptr->tr_slAttributeMapSizeAspect;
		vViewer(1) = (_rcSelRectangle.rc_slLeft + _rcSelRectangle.GetWidth()/2) / slStretch;
		vViewer(2) = fPosY;
		vViewer(3) = (_rcSelRectangle.rc_slTop + _rcSelRectangle.GetHeight()/2) / slStretch;
	}
	else
	{
		vViewer(1) = (_rcSelRectangle.rc_slLeft + _rcSelRectangle.GetWidth()/2);
		vViewer(2) = fPosY;
		vViewer(3) = (_rcSelRectangle.rc_slTop + _rcSelRectangle.GetHeight()/2);
	}


	TransformVector(vViewer.vector,_mObjToAbsStretch);
	TR_AddTerrainViewer(ptrTerrain, vViewer);
}

// Update terrain rendering data
static void UpdateRenderingData(CTerrain *ptrTerrain)
{
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	// Calculate obj to view matrix
	MatrixMultiply(_mObjToView,_mAbsToViewer, _mObjToAbs);
	MakeStretchMatrix(_mStretch,ptr->tr_vStretch);
	MatrixMultiply(_mObjToViewStretch,_mObjToView,_mStretch);
	MatrixMultiply(_mObjToAbsStretch,_mObjToAbs,_mStretch);

#ifndef	FINALVERSION			// yjpark |<--
	// if not normal viewer is enabled
	extern INDEX ter_bDisableViewer;
	if(!ter_bDisableViewer) {
		const FLOAT3D vViewerAbs = _aprProjection->ViewerPlacementR().pl_PositionVector;
		TR_AddTerrainViewer(ptrTerrain, vViewerAbs);
	}
#else
	const FLOAT3D vViewerAbs = _aprProjection->ViewerPlacementR().pl_PositionVector;
	TR_AddTerrainViewer(ptrTerrain, vViewerAbs);
#endif	// FINALVERSION			// yjpark     -->|

	// Get visible tiles
	UpdateTilesVisibility(ptr);

#ifndef	FINALVERSION			// yjpark |<--
	// if terrain update is enabled
	extern INDEX ter_bUpdateTerrain;
	if(ter_bUpdateTerrain) {
		// Update terrain tiles
		UpdateTerrainTiles(ptr);
		// Update tile neighbours
		UpdateTerrainTileNeighbours(ptr);
		// Regenerate terrain tiles
		RegenerateTerrainTiles(ptr);
	}
#else
	UpdateTerrainTiles( ptr );
	UpdateTerrainTileNeighbours( ptr );
	RegenerateTerrainTiles( ptr );
#endif	// FINALVERSION			// yjpark     -->|

	_bUseShadowMap = _wrpWorldRenderPrefs.wrp_shtShadows!=CWorldRenderPrefs::SHT_NONE;

	// set gfx render states
	gfxDepthFunc(GFX_LESS_EQUAL); 
	gfxDisableBlend();
	gfxDisableAlphaTest();
	gfxEnableDepthTest();
	gfxEnableDepthWrite();
//강동민 수정 끝 Water 구현			04.21	
	if((_aprProjection->pr_bNiceWater == TRUE) 
		|| _aprProjection->pr_bMirror == TRUE 
		|| _aprProjection->pr_bWarp == TRUE)
		gfxCullFace(GFX_FRONT);
	else
		gfxCullFace(GFX_BACK);
//강동민 수정 끝 Water 구현			04.19

#pragma message(">> Den please check this")
#pragma message(">> Change disabling of overbrightning")

	const INDEX cttxu = _pGfx->gl_ctTextureUnits;
	for(INDEX itxu=1;itxu<cttxu;itxu++) {
		gfxSetTextureUnit(itxu);
		gfxSetTextureModulation(1);
	}
	gfxSetTextureUnit(0);
	gfxSetTextureModulation(1);
}

// check vertex against fog
static inline void GetFogMapInVertex( const GFXVertex &vtx, GFXTexCoord &tex)
{
	const FLOAT fD = vtx.x*_vFViewerObj(1) + vtx.y*_vFViewerObj(2) + vtx.z*_vFViewerObj(3);
	const FLOAT fH = vtx.x*_vHDirObj(1)    + vtx.y*_vHDirObj(2)    + vtx.z*_vHDirObj(3);
	tex.s = (fD+_fFogAddZ) * _fog_fMulZ;
	tex.t = (fH+_fFogAddH) * _fog_fMulH;
}

// check vertex against haze
static inline void GetHazeMapInVertex( const GFXVertex &vtx, FLOAT &tx1)
{
	const FLOAT fD = vtx.x*_vViewerObj(1) + vtx.y*_vViewerObj(2) + vtx.z*_vViewerObj(3);
	tx1 = (fD+_fHazeAdd) * _haze_fMul;
}

static void RenderTerrainFogView(CTerrainImp *ptr, INDEX iTerrainTile)
{
	// check if rendering batched tiles
	const BOOL bBathedTiles = iTerrainTile==(-1);

	GFXTexCoord *ptcFog;
	GFXColor    *pcolFog;
	GFXColor colFog(_fog_fp.fp_colColor);

	// if rendering batched tiles
	if(bBathedTiles) {
		Matrix12 mInvObjToAbs;
		Matrix12 mInvObjToView;
		MatrixTranspose(mInvObjToAbs, _mObjToAbsStretch);
		MatrixTranspose(mInvObjToView, _mObjToViewStretch);

		FLOAT3D vObjToView;
		FLOATmatrix3D mObjToView33;
		Matrix12ToMatrixVector(mObjToView33,vObjToView,_mObjToViewStretch);

		const FLOAT3D vObjPos(_mObjToAbsStretch[ 3], _mObjToAbsStretch[ 7], _mObjToAbsStretch[11]);

		// calculate projection of viewer in absolute space
		FLOATmatrix3D &mViewer = _aprProjection->pr_ViewerRotationMatrix;
		const FLOAT3D vViewer(-mViewer(3,1), -mViewer(3,2), -mViewer(3,3));

		_vViewerObj = vViewer;
		RotateVector(_vViewerObj.vector, mInvObjToAbs);

		// get viewer -z in object space
		_vFViewerObj = FLOAT3D(0,0,-1);
		RotateVector(_vFViewerObj.vector, mInvObjToView);

		// get fog direction in object space
		_vHDirObj = _fog_vHDirAbs * !(!mViewer*mObjToView33);
		// get viewer offset
		_fFogAddZ  = vViewer(1) * (vObjPos(1) - _aprProjection->pr_vViewerPosition(1));
		_fFogAddZ += vViewer(2) * (vObjPos(2) - _aprProjection->pr_vViewerPosition(2));
		_fFogAddZ += vViewer(3) * (vObjPos(3) - _aprProjection->pr_vViewerPosition(3));
		// get fog offset
		_fFogAddH = (_fog_vHDirAbs % vObjPos) + _fog_fp.fp_fH3;

		// if software fog
		if(!_bUseHardwareShaders) {
			// Calculate fog texcoord and color in each vertex
			_atcFog.PopAll();
			_acolFog.PopAll();

			const INDEX ctvx = _avxBatchVertices.Count();
			ptcFog  = _atcFog.Push(ctvx);
			pcolFog = _acolFog.Push(ctvx);

			// for each vertex
			for(INDEX ivx=0;ivx<ctvx;ivx++) {
				const GFXVertex &vx = _avxBatchVertices[ivx];
				GFXTexCoord &tc = _atcFog[ivx];
				GetFogMapInVertex(vx,tc);
				pcolFog[ivx] = colFog;
			}
		}

	// else normal tile rendering
	} else {
		const CTerrainTile &tt = ptr->tr_attTiles[iTerrainTile];
		Matrix12 mTileToAbs;
		Matrix12 mTileToView;
		Matrix12 mInvTileToAbs;
		Matrix12 mInvTileToView;
		MatrixMultiply(mTileToAbs, _mObjToAbsStretch, tt.tt_mTileToTerrain);
		MatrixMultiply(mTileToView, _mObjToViewStretch, tt.tt_mTileToTerrain);
		MatrixTranspose(mInvTileToAbs, mTileToAbs);
		MatrixTranspose(mInvTileToView, mTileToView);

		FLOAT3D vTileToView;
		FLOATmatrix3D mTileToView33;
		Matrix12ToMatrixVector(mTileToView33,vTileToView,mTileToView);

		const FLOAT3D vObjPos(mTileToAbs[ 3], mTileToAbs[ 7], mTileToAbs[11]);

		// calculate projection of viewer in absolute space
		FLOATmatrix3D &mViewer = _aprProjection->pr_ViewerRotationMatrix;
		const FLOAT3D vViewer(-mViewer(3,1), -mViewer(3,2), -mViewer(3,3));

		_vViewerObj = vViewer;
		RotateVector(_vViewerObj.vector, mInvTileToAbs);

		// get viewer -z in object space
		_vFViewerObj = FLOAT3D(0,0,-1);
		RotateVector(_vFViewerObj.vector, mInvTileToView);

		// get fog direction in object space
		_vHDirObj = _fog_vHDirAbs * !(!mViewer*mTileToView33);
		// get viewer offset
		_fFogAddZ  = vViewer(1) * (vObjPos(1) - _aprProjection->pr_vViewerPosition(1));
		_fFogAddZ += vViewer(2) * (vObjPos(2) - _aprProjection->pr_vViewerPosition(2));
		_fFogAddZ += vViewer(3) * (vObjPos(3) - _aprProjection->pr_vViewerPosition(3));
		// get fog offset
		_fFogAddH = (_fog_vHDirAbs % vObjPos) + _fog_fp.fp_fH3;

		// if software fog
		if(!_bUseHardwareShaders) {
			// Calculate fog texcoord and color in each vertex
			_atcFog.PopAll();
			_acolFog.PopAll();

			const INDEX ctvx = _avxTileVertices.Count();
			ptcFog  = _atcFog.Push(ctvx);
			pcolFog = _acolFog.Push(ctvx);

			// for each vertex
			for(INDEX ivx=0;ivx<ctvx;ivx++) {
				const GFXVertex &vx = _avxTileVertices[ivx];
				GFXTexCoord &tc = _atcFog[ivx];
				GetFogMapInVertex(vx,tc);
				pcolFog[ivx] = colFog;
			}
		}
	}

	// Render fog pass
	shaBegin(_aprProjection, &_shTRShader_Fog);
	shaBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
	shaEnableBlend();
	shaSetTextureUnit(0);
	shaDepthFunc(GFX_EQUAL);
	shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP);
	gfxSetTexture( _fog_ulTexture, _fog_tpLocal);

	if(!_bUseHardwareShaders) {
		shaClearTextureMatrix();
		shaSetTexCoordArray(ptcFog);
		shaSetColorArray(pcolFog);
	} else {
		shaSetVertexProgram(0);
		shaSetPixelProgram(0);

		const ShaderRegister srFog(_fFogAddZ, _fFogAddH, _fog_fMulZ, _fog_fMulH);
		const ShaderRegister srFViewerObj(_vFViewerObj);
		const ShaderRegister srHDirObj(_vHDirObj);
		const ShaderRegister srFogColor(colFog);
		shaSetVertexProgramConst(18,&srFViewerObj,1);
		shaSetVertexProgramConst(19,&srHDirObj,1);
		shaSetVertexProgramConst(20,&srFog,1);
		shaSetPixelProgramConst(  7,&srFogColor,1);
	}

	shaEnd();
}

static void RenderTerrainHazeLayer(CTerrainImp *ptr, INDEX iTerrainTile)
{
	GFXTexCoord *ptcHaze;
	GFXColor    *pcolHaze;
	GFXColor     colHaze(_haze_hp.hp_colColor);

	// check if rendering batched tiles
	const BOOL bBathedTiles = iTerrainTile==(-1);

	// if rendering batched tiles
	if(bBathedTiles) {
		Matrix12 mInvObjToAbs;
		MatrixTranspose(mInvObjToAbs, _mObjToAbsStretch);

		const FLOAT3D vObjPos(_mObjToAbsStretch[ 3], _mObjToAbsStretch[ 7], _mObjToAbsStretch[11]);

		// calculate projection of viewer in absolute space
		FLOATmatrix3D &mViewer = _aprProjection->pr_ViewerRotationMatrix;
		const FLOAT3D vViewer(-mViewer(3,1), -mViewer(3,2), -mViewer(3,3));

		_vViewerObj = vViewer;
		RotateVector(_vViewerObj.vector, mInvObjToAbs);

		// Prepare haze
		_fHazeAdd  = -_haze_hp.hp_fNear;
		_fHazeAdd += vViewer(1) * (vObjPos(1) - _aprProjection->pr_vViewerPosition(1));
		_fHazeAdd += vViewer(2) * (vObjPos(2) - _aprProjection->pr_vViewerPosition(2));
		_fHazeAdd += vViewer(3) * (vObjPos(3) - _aprProjection->pr_vViewerPosition(3));

		if(!_bUseHardwareShaders) {
			_atcHaze.PopAll();
			_acolHaze.PopAll();

			const INDEX ctvx = _avxBatchVertices.Count();
			ptcHaze  = _atcHaze.Push(ctvx);
			pcolHaze = _acolHaze.Push(ctvx);

			// for each vertex
			for(INDEX ivx=0;ivx<ctvx;ivx++) {
				GFXVertex vx = _avxBatchVertices[ivx];
				GFXTexCoord &tc = _atcHaze[ivx];
				GetHazeMapInVertex(vx,tc.s);
				tc.t = 0.0f;
				pcolHaze[ivx] = colHaze;
			}
		}
	// else normal tile rendering
	} else {
		const CTerrainTile &tt = ptr->tr_attTiles[iTerrainTile];

		Matrix12 mTileToAbs;
		Matrix12 mInvTileToAbs;
		MatrixMultiply(mTileToAbs, _mObjToAbsStretch, tt.tt_mTileToTerrain);
		MatrixTranspose(mInvTileToAbs, mTileToAbs);

		const FLOAT3D vObjPos(mTileToAbs[ 3], mTileToAbs[ 7], mTileToAbs[11]);

		// calculate projection of viewer in absolute space
		FLOATmatrix3D &mViewer = _aprProjection->pr_ViewerRotationMatrix;
		const FLOAT3D vViewer(-mViewer(3,1), -mViewer(3,2), -mViewer(3,3));

		_vViewerObj = vViewer;
		RotateVector(_vViewerObj.vector, mInvTileToAbs);

		// Prepare haze
		_fHazeAdd  = -_haze_hp.hp_fNear;
		_fHazeAdd += vViewer(1) * (vObjPos(1) - _aprProjection->pr_vViewerPosition(1));
		_fHazeAdd += vViewer(2) * (vObjPos(2) - _aprProjection->pr_vViewerPosition(2));
		_fHazeAdd += vViewer(3) * (vObjPos(3) - _aprProjection->pr_vViewerPosition(3));

		if(!_bUseHardwareShaders) {
			_atcHaze.PopAll();
			_acolHaze.PopAll();   

			const INDEX ctvx = _avxTileVertices.Count();
			ptcHaze  = _atcHaze.Push(ctvx);
			pcolHaze = _acolHaze.Push(ctvx);

			// for each vertex
			for(INDEX ivx=0;ivx<ctvx;ivx++) {
				GFXVertex vx = _avxTileVertices[ivx];
				GFXTexCoord &tc = _atcHaze[ivx];
				GetHazeMapInVertex(vx,tc.s);
				tc.t = 0.0f;
				pcolHaze[ivx] = colHaze;
			}
		}
	}

	// Render haze pass
	shaBegin(_aprProjection, &_shTRShader_Fog);
	shaBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
	shaEnableBlend();
	shaSetTextureUnit(0);
	shaDepthFunc(GFX_EQUAL);
	shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP);
	gfxSetTexture( _haze_ulTexture, _haze_tpLocal);

	if(!_bUseHardwareShaders) {
		shaClearTextureMatrix();
		shaSetTexCoordArray(ptcHaze);
		shaSetColorArray(pcolHaze);
	} else {
		shaSetVertexProgram(0);
		shaSetPixelProgram(0);

		const ShaderRegister srHaze(_fHazeAdd, 0.0f, _haze_fMul, 0.0f);
		const ShaderRegister srDummy(0.0f, 0.0f, 0.0f, 0.0f);
		const ShaderRegister srViewerObj(_vViewerObj);
		const ShaderRegister srHazeColor(colHaze);
		shaSetVertexProgramConst(18,&srViewerObj,1);
		shaSetVertexProgramConst(19,&srDummy,1);
		shaSetVertexProgramConst(20,&srHaze,1);
		shaSetPixelProgramConst(  7,&srHazeColor,1);
	}

	shaEnd();
}

static void RenderTerrainViewBatched(CTerrain *ptrTerrain)
{
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	CTextureData *ptdTopMap = ptr->tr_ptdTopMap;
	CTextureData *ptdEdgeMap = ptr->tr_ptdEdgeMap;

	const INDEX cttvx = _avxBatchVertices.Count();
	const INDEX ctind = _auwBatchIndices.Count();

	// if nothing to render
	if( ctind == 0 || cttvx == 0 )
	{
		// bail out
		return;
	}

	const INDEX		ctTXUs = _bUseHardwareShaders ? 4 : 2;
	extern INDEX	ter_bUseTopMap;
	const BOOL	bTopMapSynced = !( ptrTerrain->tr_ulTerrainFlags & TR_SYNC_TOPMAP );
	const BOOL	bTopMapEnabled = ptrTerrain->tr_ulTerrainFlags & TR_USES_TOPMAP;
	const BOOL	bTopMapLODMatch = ptr->tr_iMaxTileLOD >= ptr->tr_iFirstTopMapLOD;
	const BOOL	bUseTopMap = bTopMapEnabled && bTopMapSynced && bTopMapLODMatch && ter_bUseTopMap;

	// Set view matrix for this tile
	SetViewMatrix( _mObjToViewStretch );

	// Get pointer to rendering arrays
	GFXVertex	*pvxVertices = &_avxBatchVertices[0];
	GFXTexCoord	*ptcTexCoords = &_atcBatchTexCoords[0];
	const UWORD	*puwIndices = &_auwBatchIndices[0];

	// Prepare shader rendering
	shaSetVertexArray( pvxVertices, 0, cttvx );
	shaSetIndices( puwIndices, ctind );
	shaSetConstantColor( 0xFFFFFFFF );
	shaLockArrays();

	if( bUseTopMap )
	{
		shaDisableBlend();
		shaEnableDepthWrite();
		shaEnableAlphaTest();
		shaEnableDepthWrite();

		shaBegin( _aprProjection, &_shTRShader_Base );
		shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP, 0 );
		shaSetTexture( ptdTopMap, 0 );
		shaClearTextureMatrix( 0 );

		// if using hardware shaders
		if( _bUseHardwareShaders )
		{
			// set vertex and pixel programs
			shaSetVertexProgram( 0 );
			shaSetPixelProgram( 0 );
			const INDEX	iRegister = 21;
			const ShaderRegister	*psrMatrix = (ShaderRegister *)&ptr->tr_mTopMap;
			shaSetVertexProgramConst(iRegister, psrMatrix, 3);
		}
		// if software shaders
		else
		{
			// set layer transformation matrix
			shaSetTexCoordArray( ptcTexCoords, 0 );
			shaSetTextureMatrix( ptr->tr_mTopMap, 0 );
		}

		shaEnd();
		shaDisableAlphaTest();
		shaDisableDepthWrite();
		shaDepthFunc( GFX_EQUAL );
	}
	else
	{
		// do edge pass if needed
		const BOOL	bUseEdgeMap = TRUE;
		if( bUseEdgeMap )
		{
			// Render edge mask (in depth buffer only)
			shaSetColorMask( NONE );
			shaEnableAlphaTest();
			shaEnableDepthWrite();

			const INDEX	iRegister = 21;
			shaBegin( _aprProjection, &_shTRShader_Base );
			shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP, 0 );
			shaSetTexture( ptdEdgeMap, 0 );
			if( _bUseHardwareShaders )
			{
				shaSetVertexProgram( 0 );
				shaSetPixelProgram( 0 );
				const ShaderRegister	*psrMatrix = (ShaderRegister *)&ptr->tr_mEdgeMap;
				shaSetVertexProgramConst( iRegister, psrMatrix, 3 );
			}
			else
			{
				shaSetTexCoordArray( ptcTexCoords, 0 );
				shaSetTextureMatrix( ptr->tr_mEdgeMap, 0 );
			}

			shaEnd();
			// Enable normal rendering
			shaDisableAlphaTest();
			shaDisableDepthWrite();
			shaClearTextureMatrix( 0 );
			shaSetColorMask( CT_RMASK | CT_GMASK | CT_BMASK | CT_AMASK );
			TR_SETINFO( ti_ctTileEdgeTested++ );
		}

		if( _bUseHardwareShaders )
		{
			shaBlendFunc( GFX_ONE, GFX_SRC_ALPHA );
			shaEnableBlend();
		}
		else
		{
			shaBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA );
			shaEnableBlend();
		}

		INDEX		iUsedTXUs = 0;	// used texture units count
		INDEX		iUsedLayers = 0;	// used layers count for current pass
		INDEX		iRenderPass = 0;	// render pass count
		Matrix12	mTexTransf[4];		// array of tranform matrices for each layer (must be set after shader is set)

		// for each layer
		const INDEX	cttrl = ptr->tr_atlLayers.Count();
		for( INDEX itrl = 0; itrl < cttrl; itrl++ )
		{
			// Get layer
			const CTerrainLayer		&tlLayer = ptr->tr_atlLayers[itrl];
			const CTerrainLayerImp	*ptrl = tlLayer.trl_trlLayerImp;
			CTextureData	*ptdLayerTex  = ptrl->trl_ptdTexture;
			CTextureData	*ptdLayerMask = ptrl->trl_ptdLayerMask;

			// check for layer flags
			const BOOL	bLayerValid = ( (tlLayer.tl_bVisible) && (ptdLayerTex != NULL) );
			const BOOL	bBlankMask = tlLayer.tl_ulFlags & TLF_BLANK_MASK;
			const BOOL	bFullMask = tlLayer.tl_ulFlags & TLF_FULL_MASK;

			// if layer isn't valid for rendering
			if( !bLayerValid || bBlankMask )
			{
				// advance to next layer
				continue;
			}

			const INDEX	ctFreeTXUs = ctTXUs - iUsedTXUs;
			// if layer won't fit in available texture units
			const INDEX ctTXUsForLayer = 2;
			if( ctTXUsForLayer + iUsedTXUs > ctTXUs )
			{
				ASSERTMSG( iUsedTXUs > 0, "Some texture units must be used" );
				// Select shader for rendering (based on layer count)
				if( iUsedLayers == 1 )
				{
					shaBegin( _aprProjection, &_shTRShader_1TRL );
				}
				else
				{
					ASSERT( iUsedLayers == 2 );
					shaBegin( _aprProjection, &_shTRShader_2TRL );
				}

				// if using hardware shaders
				if( _bUseHardwareShaders )
				{
					// set vertex and pixel programs
					shaSetVertexProgram( 0 );
					shaSetPixelProgram( 0 );
					// for each texture unit
					for( INDEX itxu = 0; itxu < iUsedTXUs; itxu++ )
					{
						// set layer transformation matrix
						const INDEX	iRegister = 21 + itxu * 3;
						const ShaderRegister	*psrMatrix = (ShaderRegister *)&mTexTransf[itxu];
						shaSetVertexProgramConst( iRegister, psrMatrix, 3 );
					}
				}
				// if software shaders
				else
				{
					// for each texture unit
					for( INDEX itxu = 0; itxu < iUsedTXUs; itxu++ )
					{
						// set layer transformation matrix
						shaSetTexCoordArray( ptcTexCoords, itxu );
						shaSetTextureMatrix( mTexTransf[itxu], itxu );
					}
				}

				// if this is first pass and there is no edge map for this tile
				const BOOL	bFirstPass = iRenderPass == 0;
				if( bFirstPass && !bUseEdgeMap )
				{
					// write tile in z buffer with this tile
					shaEnableDepthWrite();
					shaDepthFunc( GFX_LESS_EQUAL );
				// if not
				}
				else
				{
					// no depth write 
					shaDisableDepthWrite();
					shaDepthFunc( GFX_EQUAL );
				}
				shaEnd();
				iRenderPass++;
				TR_SETINFO( ti_ctVisibleTriangles += ctind / 3 );
				// Reset used texture unit count
				iUsedTXUs = 0;
				iUsedLayers = 0;
			}

			// Prepare layer texture unit
			shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT, iUsedTXUs );
			shaSetTexture( ptdLayerTex, iUsedTXUs);

			// Remember texture matrix for this unit
			MatrixCopy( mTexTransf[iUsedTXUs], ptrl->trl_mTransform );

			// Prepare layer mask unit
			shaSetTextureWrapping( GFX_CLAMP,GFX_CLAMP, iUsedTXUs + 1 );
			shaSetTexture( ptdLayerMask, iUsedTXUs + 1 );

			// Remember texture matrix for this unit
			MatrixCopy( mTexTransf[iUsedTXUs + 1], ptr->tr_mEdgeMap );
			iUsedTXUs += 2;
			iUsedLayers++;
		}

		// if some layers need to be rendered
		if( iUsedLayers > 0 )
		{
			// Select shader for rendering (based on layer count)
			if( iUsedLayers == 1 )
			{
				shaBegin( _aprProjection, &_shTRShader_1TRL );
			}
			else
			{
				ASSERT( iUsedLayers == 2 );
				shaBegin( _aprProjection, &_shTRShader_2TRL );
			}

			// if using hardware shaders
			if( _bUseHardwareShaders )
			{
				// set vertex and pixel programs
				shaSetVertexProgram( 0 );
				shaSetPixelProgram( 0 );
				// for each texture unit
				for( INDEX itxu = 0; itxu < iUsedTXUs; itxu++ )
				{
					// set layer transformation matrix
					const INDEX	iRegister = 21 + itxu * 3;
					const ShaderRegister	*psrMatrix = (ShaderRegister *)&mTexTransf[itxu];
					shaSetVertexProgramConst( iRegister, psrMatrix, 3 );
				}
			
			}
			// if software shaders
			else
			{
				// for each texture unit
				for( INDEX itxu = 0; itxu < iUsedTXUs; itxu++ )
				{
					// set layer transformation matrix
					shaSetTexCoordArray( ptcTexCoords, itxu );
					shaSetTextureMatrix( mTexTransf[itxu], itxu );
				}
			}

			// if this is first pass and there is no edge map for this tile
			const BOOL	bFirstPass = iRenderPass == 0;
			if( bFirstPass && !bUseEdgeMap )
			{
				// write tile in z buffer with this tile
				shaEnableDepthWrite();
				shaDepthFunc( GFX_LESS_EQUAL );
			// if not
			}
			else
			{
				// no depth write 
				shaDisableDepthWrite();
				shaDepthFunc( GFX_EQUAL );
			}
			shaEnd();
			iRenderPass++;
			TR_SETINFO( ti_ctVisibleTriangles += ctind / 3 );
			// Reset used texture unit count
			iUsedTXUs = 0;
			iUsedLayers = 0;
		}

		// Disable texture units
		const INDEX	cttxu = _pGfx->gl_ctTextureUnits;
		for( INDEX itxu = 1; itxu < cttxu; itxu++ )
		{
			shaClearTextureMatrix( itxu );
			shaDisableTexture( itxu );
		}
		shaSetTextureUnit( 0 );
	}

	// if shadow map is visible
	if( _bUseShadowMap && ter_bShowShadowMap )					// yjpark |<--
	{
#ifndef	FINALVERSION			// yjpark |<--
		extern BOOL	_bWorldEditorApp;
		extern BOOL	_bInTestGame;
		if( _bWorldEditorApp && !_bInTestGame )
		{
			extern BufferType	_btSelBufferType;
			shaBegin(_aprProjection, &_shTRShader_Base );
			shaEnableBlend();
			shaBlendFunc( GFX_DST_COLOR, GFX_SRC_COLOR );
			shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP, 0 );
			CTextureData	*ptdShadowMap;
			if( _btSelBufferType == BT_SHADOW_MAP )
				ptdShadowMap = ptr->tr_aptdShadowMaps[ptrTerrain->tr_iSelectedShadowMap];
			else
				ptdShadowMap = ptr->tr_ptdShadowMap;
			shaSetTexture( ptdShadowMap, 0 );

			if( _bUseHardwareShaders )
			{
				shaSetVertexProgram( 0 );
				shaSetPixelProgram( 0 );
				const INDEX	iRegister = 21;
				const ShaderRegister*	psrMatrix = (ShaderRegister*)&ptr->tr_mShadowMap;
				shaSetVertexProgramConst( iRegister, psrMatrix, 3 );
			}
			else
			{
				shaSetTexCoordArray( ptcTexCoords, 0 );
				shaSetTextureMatrix( ptr->tr_mShadowMap, 0 );
			}
			shaEnd();
			TR_SETINFO( ti_ctVisibleTriangles += ctind / 3 );
		}
		else
#endif	// FINALVERSION			// yjpark     -->|
		{
			shaBegin( _aprProjection, &_shTRShader_Shadow );
			shaBlendFunc( GFX_DST_COLOR, GFX_SRC_COLOR );
			shaEnableBlend();
			shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP, 0 );
			CTextureData	*ptdCurrentShadowMap = ptr->tr_aptdShadowMaps[ptr->tr_iCurrentShadow];
			CTextureData	*ptdNextShadowMap = ptr->tr_aptdShadowMaps[ptr->tr_iNextShadow];
			shaSetTexture( ptdCurrentShadowMap, 0 );
			shaSetTexture( ptdNextShadowMap, 1 );

			extern COLOR g_colWeather;
			GFXColor gcWeather(g_colWeather);
//안태훈 수정 시작	//(Terrain Render for No Shader)(0.1)
			DWORD dw_tss_top_value0 = 0;
			DWORD dw_tss_top_value1 = 0;
			DWORD dw_tss_aop_value0 = 0;
			DWORD dw_tss_aop_value1 = 0;
			DWORD dw_tfactor = 0;
			DWORD dw_tss_ta_value0_arg1 = 0;
			DWORD dw_tss_ta_value0_arg2 = 0;
			DWORD dw_tss_ta_value0_arg0 = 0;
			DWORD dw_tss_ta_value1_arg1 = 0;
			DWORD dw_tss_ta_value1_arg2 = 0;
			DWORD dw_tss_ta_value1_arg0 = 0;
//안태훈 수정 끝	//(Terrain Render for No Shader)(0.1)
			if( _bUseHardwareShaders )
			{
				shaSetVertexProgram( 0 );
				shaSetPixelProgram( 0 );
				const INDEX iShadow1Register = 21;
				const INDEX iShadow2Register = 24;
				const ShaderRegister*	psrMatrix = (ShaderRegister*)&ptr->tr_mShadowMap;
				const FLOAT				fFactor = ptr->tr_fShadowBlendFactor;
				const ShaderRegister	srBlendFactor( fFactor, fFactor, fFactor, fFactor );
				ShaderRegister srWeatherColor( gcWeather );
				shaSetPixelProgramConst( 6, &srWeatherColor, 1 );
				shaSetPixelProgramConst( 7, &srBlendFactor, 1 );
				shaSetVertexProgramConst( iShadow1Register, psrMatrix, 3 );
				shaSetVertexProgramConst( iShadow2Register, psrMatrix, 3 );
			}
//안태훈 수정 시작	//(Terrain Render for No Shader)(0.1)
			else
			{
				// save old
				/*_pGfx->gl_pd3dDevice->GetRenderState(D3DRS_TEXTUREFACTOR, &dw_tfactor);
				_pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &dw_tss_ta_value0_arg1);
				_pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &dw_tss_ta_value0_arg2);
				_pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG0, &dw_tss_ta_value0_arg0);
				_pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_COLOROP, &dw_tss_top_value0);
				_pGfx->gl_pd3dDevice->GetTextureStageState(1, D3DTSS_COLORARG1, &dw_tss_ta_value1_arg1);
				_pGfx->gl_pd3dDevice->GetTextureStageState(1, D3DTSS_COLORARG2, &dw_tss_ta_value1_arg2);
				_pGfx->gl_pd3dDevice->GetTextureStageState(1, D3DTSS_COLORARG0, &dw_tss_ta_value1_arg0);
				_pGfx->gl_pd3dDevice->GetTextureStageState(1, D3DTSS_COLOROP, &dw_tss_top_value1);
				_pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &dw_tss_aop_value0);
				_pGfx->gl_pd3dDevice->GetTextureStageState(1, D3DTSS_ALPHAOP, &dw_tss_aop_value1);

				// setting new
				_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
				_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
				_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TFACTOR);
				_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
				_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG0, D3DTA_TEXTURE);
				_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_LERP);
				_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
				_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
				UBYTE ubBlendFactor = 0xFF * ptr->tr_fShadowBlendFactor;
				DWORD dwBlendFactor = (ubBlendFactor << 24) | (ubBlendFactor << 16) | (ubBlendFactor << 8) | ubBlendFactor;
				_pGfx->gl_pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, dwBlendFactor);*/

				shaSetTexCoordArray( ptcTexCoords, 0 );
				shaSetTextureMatrix( ptr->tr_mShadowMap, 0 );
				//shaSetTexCoordArray( ptcTexCoords, 1 );
				//shaSetTextureMatrix( ptr->tr_mShadowMap, 1 );
				shaSetTexture( (CTextureData*)NULL, 1 );
			}
			shaEnd();
			// restore old
			if( !_bUseHardwareShaders )
			{
				/*_pGfx->gl_pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, dw_tfactor);
				_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dw_tss_ta_value0_arg1);
				_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, dw_tss_ta_value0_arg2);
				_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG0, dw_tss_ta_value0_arg0);
				_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, dw_tss_top_value0);
				_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, dw_tss_ta_value1_arg1);
				_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, dw_tss_ta_value1_arg2);
				_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG0, dw_tss_ta_value1_arg0);
				_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, dw_tss_top_value1);
				_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, dw_tss_aop_value0);
				_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, dw_tss_aop_value1);*/
			}
//안태훈 수정 끝	//(Terrain Render for No Shader)(0.1)
			TR_SETINFO( ti_ctVisibleTriangles += ctind / 3 );
		}
	}															// yjpark     -->|

	const BOOL	bHasFog = ptrTerrain->tr_ulTerrainFlags & TR_HAS_FOG;
	const BOOL	bHasHaze = ptrTerrain->tr_ulTerrainFlags & TR_HAS_HAZE;

	if( bHasFog )
		RenderTerrainFogView( ptr, -1 );
	else if( bHasHaze )
		RenderTerrainHazeLayer( ptr, -1 );

	// Unlock stuff
	shaUnlockArrays();
	TR_SETINFO( tr_ctVisibleVertices += _avxBatchVertices.Count() );
	// Clear rendering stuff
	shaClean();
	shaDepthFunc( GFX_LESS_EQUAL );

	// Clean batch arrays
	_avxBatchVertices.PopAll();
	_atcBatchTexCoords.PopAll();
	_auwBatchIndices.PopAll();
}

static void RenderTerrainView(CTerrain *ptrTerrain)
{
	// If terrain rendering is disabled
	extern INDEX	ter_bRenderTerrain;
	if( !ter_bRenderTerrain )
	{
		// bail out
		return;
	}

	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;
	CTextureData *ptdTopMap = ptr->tr_ptdTopMap;
	CTextureData *ptdEdgeMap = ptr->tr_ptdEdgeMap;

	// Enable hardware shaders if required
	if( _bUseHardwareShaders )
	{
		shaEnableVertexProgram();
		shaEnablePixelProgram();
	}

	// Get available tex unit count for tile rendering
	const INDEX	ctTXUs = _bUseHardwareShaders ? 4 : 2;
	// Get visible terrain tile count
	const INDEX	ctVisibleTiles = _aiVisibleTiles.Count();
	// for each terrain tile
	for( INDEX itt = 0;itt < ctVisibleTiles; itt++ )
	{
		const INDEX	iTerrainTile = _aiVisibleTiles[itt];
		const CTerrainTile	&tt = ptr->tr_attTiles[iTerrainTile];
		const INDEX	cttvx = tt.tt_avxVertices.Count();
		const INDEX	ctind = tt.tt_atriTriangles.Count() * 3;

		// Lock tile triangles
		ptr->tr_attTiles[iTerrainTile].tt_atriTriangles.Lock( TDAF_LOCK_READ );

		// check if small tiles should be batched
		extern INDEX	ter_bOptimizeRendering;
		extern INDEX	ter_bUseTopMap;
		const BOOL	bBatchTile = ter_bOptimizeRendering && tt.tt_iCurrentLOD == ptr->tr_iMaxTileLOD;
		const BOOL	bTopMapSynced = !( ptrTerrain->tr_ulTerrainFlags & TR_SYNC_TOPMAP);
		const BOOL	bTopMapEnabled = ptrTerrain->tr_ulTerrainFlags & TR_USES_TOPMAP;
		const BOOL	bTopMapLODMatch = tt.tt_iCurrentLOD >= ptr->tr_iFirstTopMapLOD;
		const BOOL	bUseTopMap = bTopMapEnabled && bTopMapSynced && bTopMapLODMatch && ter_bUseTopMap;

		// if this tile should be batched
		if( bBatchTile )
		{
			// Unpack tile vertices in batch tile array
			UnpackTileVerticesBatch( ptr, iTerrainTile );
		}
		else
		{
			// Unpack tile vertices
			UnpackTileVertices( ptr, iTerrainTile );

			// Set view matrix for this tile
			Matrix12	mView;
			MatrixMultiply( mView, _mObjToViewStretch, tt.tt_mTileToTerrain );
			SetViewMatrix( mView );

			// Get pointer to rendering arrays
			GFXVertex	*pvxVertices = &_avxTileVertices[0];
			GFXTexCoord	*ptcTexCoords = &_atcTileTexCoords[0];
			const UWORD	*puwIndices = &tt.tt_atriTriangles[0].trt_auwIndices[0];

			// Prepare shader rendering
			shaSetVertexArray( pvxVertices, 0, cttvx );
			shaSetIndices( puwIndices, ctind );
			shaSetConstantColor( 0xFFFFFFFF );
			shaLockArrays();

			// if using top map for rendering
			if( bUseTopMap )
			{
				shaDisableBlend();
				shaEnableDepthWrite();
				shaEnableAlphaTest();
				shaEnableDepthWrite();

				shaBegin( _aprProjection, &_shTRShader_Base );
				shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP, 0 );
				shaSetTexture( ptdTopMap, 0 );
				shaClearTextureMatrix( 0 );

				// if using hardware shaders
				if( _bUseHardwareShaders )
				{
					// set vertex and pixel programs
					shaSetVertexProgram( 0 );
					shaSetPixelProgram( 0 );
					const INDEX	iRegister = 21;
					const ShaderRegister	*psrMatrix = (ShaderRegister *)&tt.tt_mTopMap;
					shaSetVertexProgramConst( iRegister, psrMatrix, 3 );
				}
				// if software shaders
				else
				{
					// set layer transformation matrix
					shaSetTexCoordArray( ptcTexCoords, 0 );
					shaSetTextureMatrix( tt.tt_mTopMap, 0 );
				}

				shaEnd();
				shaDisableAlphaTest();
				shaDisableDepthWrite();
				shaDepthFunc( GFX_EQUAL );
			}
			// else render each layer
			else
			{
				// do edge pass if needed
				const BOOL	bUseEdgeMap = !( tt.tt_ulFlags & TTF_FULL_EDGE_MAP );
				if( bUseEdgeMap )
				{
					// Render edge mask (in depth buffer only)
					shaSetColorMask( NONE );
					shaEnableAlphaTest();
					shaEnableDepthWrite();

					const INDEX	iRegister = 21;
					shaBegin( _aprProjection, &_shTRShader_Base );
					shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP, 0 );
					shaSetTexture( ptdEdgeMap, 0 );
					if( _bUseHardwareShaders )
					{
						shaSetVertexProgram( 0 );
						shaSetPixelProgram( 0 );
						const ShaderRegister	*psrMatrix = (ShaderRegister *)&tt.tt_mEdgeMap;
						shaSetVertexProgramConst( iRegister, psrMatrix, 3 );
					}
					else
					{
						shaSetTexCoordArray( ptcTexCoords, 0 );
						shaSetTextureMatrix( tt.tt_mEdgeMap, 0 );
					}

					shaEnd();
					// Enable normal rendering
					shaDisableAlphaTest();
					shaDisableDepthWrite();
					shaClearTextureMatrix( 0 );
					shaSetColorMask( CT_RMASK | CT_GMASK | CT_BMASK | CT_AMASK );
					TR_SETINFO( ti_ctTileEdgeTested++ );
				}

				if( _bUseHardwareShaders )
				{
					shaBlendFunc( GFX_ONE, GFX_SRC_ALPHA );
					shaEnableBlend();
				}
				else
				{
					shaBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA );
					shaEnableBlend();
				}

				INDEX		iUsedTXUs  = 0;		// used texture units count
				INDEX		iUsedLayers = 0;	// used layers count for current pass
				INDEX		iRenderPass = 0;	// render pass count
				Matrix12	mTexTransf[4];		// array of tranform matrices for each layer (must be set after shader is set)

				// for each layer
				const INDEX	cttrl = ptr->tr_atlLayers.Count();
				for( INDEX itrl = 0; itrl < cttrl; itrl++ )
				{
					// Get layer
					const CTerrainLayer		&tlLayer = ptr->tr_atlLayers[itrl];
					const CTerrainLayerImp	*ptrl = tlLayer.trl_trlLayerImp;
					CTextureData	*ptdLayerTex  = ptrl->trl_ptdTexture;
					CTextureData	*ptdLayerMask = ptrl->trl_ptdLayerMask;

					// Check if layer is visble
					BOOL	bLayerMaskEmpty;
					if( itrl < 32 )
					{
						const ULONG	ulVisLayerMask = ( 1UL << itrl );
						if( tt.tt_ulVisLayers & ulVisLayerMask )
						{
							bLayerMaskEmpty = FALSE;
						}
						else
						{
							bLayerMaskEmpty = TRUE;
							TR_SETINFO( ti_ctTileLayersSkiped++ );
						}
					}
					else
					{
						bLayerMaskEmpty = FALSE;
					}

					// check for layer flags
					const BOOL	bLayerValid = ( tlLayer.tl_bVisible && ( ptdLayerTex != NULL ) ) && !bLayerMaskEmpty;
					const BOOL	bBlankMask = tlLayer.tl_ulFlags & TLF_BLANK_MASK;
					const BOOL	bFullMask = tlLayer.tl_ulFlags & TLF_FULL_MASK;

					// if layer isn't valid for rendering
					if( !bLayerValid || bBlankMask )
					{
						// advance to next layer
						continue;
					}

					const INDEX	ctFreeTXUs = ctTXUs - iUsedTXUs;
					// if layer won't fit in available texture units
					const INDEX	ctTXUsForLayer = 2;
					if( ctTXUsForLayer + iUsedTXUs > ctTXUs )
					{
						ASSERTMSG( iUsedTXUs > 0, "Some texture units must be used" );
						// Select shader for rendering (based on layer count)
						if( iUsedLayers == 1 )
						{
							shaBegin( _aprProjection, &_shTRShader_1TRL );
						}
						else
						{
							ASSERT( iUsedLayers == 2 );
							shaBegin( _aprProjection, &_shTRShader_2TRL );
						}

						// if using hardware shaders
						if( _bUseHardwareShaders )
						{
							// set vertex and pixel programs
							shaSetVertexProgram( 0 );
							shaSetPixelProgram( 0 );
							// for each texture unit
							for( INDEX itxu = 0; itxu < iUsedTXUs; itxu++ )
							{
								// set layer transformation matrix
								const INDEX	iRegister = 21 + itxu*3;
								const ShaderRegister	*psrMatrix = (ShaderRegister *)&mTexTransf[itxu];
								shaSetVertexProgramConst( iRegister, psrMatrix, 3 );
							}
						}
						// if software shaders
						else
						{
							// for each texture unit
							for( INDEX itxu = 0; itxu < iUsedTXUs; itxu++ )
							{
								// set layer transformation matrix
								shaSetTexCoordArray( ptcTexCoords, itxu );
								shaSetTextureMatrix( mTexTransf[itxu], itxu );
							}
						}

						// if this is first pass and there is no edge map for this tile
						const BOOL	bFirstPass = iRenderPass == 0;
						if( bFirstPass && !bUseEdgeMap )
						{
							// write tile in z buffer with this tile
							shaEnableDepthWrite();
							shaDepthFunc( GFX_LESS_EQUAL );
						
						}
						// if not
						else
						{
							// no depth write 
							shaDisableDepthWrite();
							shaDepthFunc( GFX_EQUAL );
						}
						shaEnd();
						iRenderPass++;
						TR_SETINFO( ti_ctVisibleTriangles += ctind / 3 );
						// Reset used texture unit count
						iUsedTXUs = 0;
						iUsedLayers = 0;
					}

					// Prepare layer texture unit
					Matrix12	mTransform;
					Matrix12	mTileToTerrain;
					MatrixCopy( mTileToTerrain, tt.tt_mTileToTerrain );
					Swap( mTileToTerrain[ 7], mTileToTerrain[11] );
					MatrixMultiplyCP( mTransform, ptrl->trl_mTransform, mTileToTerrain ); 
					shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT, iUsedTXUs );
					shaSetTexture( ptdLayerTex, iUsedTXUs );

					if( ptrl->trl_ptoTexObj )
					{
						const INDEX iFrameNo = ptrl->trl_ptoTexObj->GetFrame();
						ptdLayerTex->SetAsCurrent(iFrameNo);
					}

					// Remember texture matrix for this unit
					MatrixCopy( mTexTransf[iUsedTXUs], mTransform );

					// Prepare layer mask unit
					shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP, iUsedTXUs + 1 );
					shaSetTexture( ptdLayerMask, iUsedTXUs + 1 );

					// Remember texture matrix for this unit
					MatrixCopy( mTexTransf[iUsedTXUs + 1], tt.tt_mEdgeMap );
					iUsedTXUs += 2;
					iUsedLayers++;
				}

				// if some layers need to be rendered
				if( iUsedLayers > 0 )
				{
					// Select shader for rendering (based on layer count)
					if( iUsedLayers == 1 )
					{
						shaBegin( _aprProjection, &_shTRShader_1TRL );
					}
					else
					{
						ASSERT( iUsedLayers == 2 );
						shaBegin( _aprProjection, &_shTRShader_2TRL );
					}

					// if using hardware shaders
					if( _bUseHardwareShaders )
					{
						// set vertex and pixel programs
						shaSetVertexProgram( 0 );
						shaSetPixelProgram( 0 );
						// for each texture unit
						for( INDEX itxu = 0; itxu < iUsedTXUs; itxu++ )
						{
							// set layer transformation matrix
							const INDEX	iRegister = 21 + itxu * 3;
							const ShaderRegister	*psrMatrix = (ShaderRegister *)&mTexTransf[itxu];
							shaSetVertexProgramConst( iRegister, psrMatrix, 3 );
						}
					}
					// if software shaders
					else
					{
						// for each texture unit
						for( INDEX itxu = 0; itxu < iUsedTXUs; itxu++ )
						{
							// set layer transformation matrix
							shaSetTexCoordArray( ptcTexCoords, itxu );
							shaSetTextureMatrix( mTexTransf[itxu], itxu );
						}
					}

					// if this is first pass and there is no edge map for this tile
					const BOOL	bFirstPass = iRenderPass == 0;
					if( bFirstPass && !bUseEdgeMap )
					{
						// write tile in z buffer with this tile
						shaEnableDepthWrite();
						shaDepthFunc( GFX_LESS_EQUAL );
					}
					// if not
					else
					{
						// no depth write 
						shaDisableDepthWrite();
						shaDepthFunc( GFX_EQUAL );
					}
					shaEnd();
					iRenderPass++;
					TR_SETINFO( ti_ctVisibleTriangles += ctind / 3 );
					// Reset used texture unit count
					iUsedTXUs = 0;
					iUsedLayers = 0;
				}

				// Disable texture units
				const INDEX	cttxu = _pGfx->gl_ctTextureUnits;
				for( INDEX itxu = 1; itxu < cttxu; itxu++ )
				{
					shaClearTextureMatrix( itxu );
					shaDisableTexture( itxu );
				}
				shaSetTextureUnit( 0 );
			}

			// if shadow map is visible
			if( _bUseShadowMap && ter_bShowShadowMap )				// yjpark |<--
			{
#ifndef	FINALVERSION			// yjpark |<--
				extern BOOL	_bWorldEditorApp;
				extern BOOL	_bInTestGame; 
				if( _bWorldEditorApp && !_bInTestGame )
				{
					shaBegin( _aprProjection, &_shTRShader_Base );
					shaBlendFunc( GFX_DST_COLOR, GFX_SRC_COLOR );
					shaEnableBlend();
					shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP, 0 );
					CTextureData*	ptdShadowMap;
					extern BufferType	_btSelBufferType;
					if( _btSelBufferType == BT_SHADOW_MAP )
						ptdShadowMap = ptr->tr_aptdShadowMaps[ptrTerrain->tr_iSelectedShadowMap];
					else
						ptdShadowMap = ptr->tr_ptdShadowMap;
					shaSetTexture( ptdShadowMap, 0 );

					if( _bUseHardwareShaders )
					{
						shaSetVertexProgram( 0 );
						shaSetPixelProgram( 0 );
						const INDEX iRegister = 21;
						const ShaderRegister*	psrMatrix = (ShaderRegister*)&tt.tt_mShadowMap;
						shaSetVertexProgramConst( iRegister, psrMatrix, 3 );
					}
					else
					{
						shaSetTexCoordArray( ptcTexCoords, 0 );
						shaSetTextureMatrix( tt.tt_mShadowMap, 0 );
					}
					shaEnd();
					TR_SETINFO( ti_ctVisibleTriangles += ctind / 3 );
				}
				else
#endif	// FINALVERSION			// yjpark     -->|
				{
					shaBegin( _aprProjection, &_shTRShader_Shadow );
					shaBlendFunc( GFX_DST_COLOR, GFX_SRC_COLOR );
					shaEnableBlend();
					shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP, 0 );
					CTextureData*	ptdCurrentShadowMap = ptr->tr_aptdShadowMaps[ptr->tr_iCurrentShadow];
					CTextureData*	ptdNextShadowMap = ptr->tr_aptdShadowMaps[ptr->tr_iNextShadow];
					shaSetTexture( ptdCurrentShadowMap, 0 );
					shaSetTexture( ptdNextShadowMap, 1 );

					extern COLOR g_colWeather;
					GFXColor gcWeather(g_colWeather);
//안태훈 수정 시작	//(Terrain Render for No Shader)(0.1)
					DWORD dw_tss_top_value0 = 0;
					DWORD dw_tss_top_value1 = 0;
					DWORD dw_tss_top_value2 = 0;
					DWORD dw_tss_aop_value0 = 0;
					DWORD dw_tss_aop_value1 = 0;
					DWORD dw_tfactor = 0;
					DWORD dw_tss_ta_value0_arg1 = 0;
					DWORD dw_tss_ta_value0_arg2 = 0;
					DWORD dw_tss_ta_value0_arg0 = 0;
					DWORD dw_tss_ta_value1_arg1 = 0;
					DWORD dw_tss_ta_value1_arg2 = 0;
					DWORD dw_tss_ta_value1_arg0 = 0;
//안태훈 수정 끝	//(Terrain Render for No Shader)(0.1)
					if( _bUseHardwareShaders )
					{
						shaSetVertexProgram( 0 );
						shaSetPixelProgram( 0 );
						const INDEX iShadow1Register = 21;
						const INDEX iShadow2Register = 24;
						const ShaderRegister*	psrMatrix = (ShaderRegister*)&tt.tt_mShadowMap;
						const FLOAT				fFactor = ptr->tr_fShadowBlendFactor;
						const ShaderRegister	srBlendFactor( fFactor, fFactor, fFactor, fFactor );
						ShaderRegister srWeatherColor( gcWeather );
						shaSetPixelProgramConst( 7, &srBlendFactor, 1 );
						shaSetPixelProgramConst( 6, &srWeatherColor, 1 );
						shaSetVertexProgramConst( iShadow1Register, psrMatrix, 3 );
						shaSetVertexProgramConst( iShadow2Register, psrMatrix, 3 );
					}
//안태훈 수정 시작	//(Terrain Render for No Shader)(0.1)
					else
					{
						// save old
						_pGfx->gl_pd3dDevice->GetRenderState(D3DRS_TEXTUREFACTOR, &dw_tfactor);
						_pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &dw_tss_ta_value0_arg1);
						_pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &dw_tss_ta_value0_arg2);
						_pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG0, &dw_tss_ta_value0_arg0);
						_pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_COLOROP, &dw_tss_top_value0);
						_pGfx->gl_pd3dDevice->GetTextureStageState(1, D3DTSS_COLORARG1, &dw_tss_ta_value1_arg1);
						_pGfx->gl_pd3dDevice->GetTextureStageState(1, D3DTSS_COLORARG2, &dw_tss_ta_value1_arg2);
						_pGfx->gl_pd3dDevice->GetTextureStageState(1, D3DTSS_COLORARG0, &dw_tss_ta_value1_arg0);
						_pGfx->gl_pd3dDevice->GetTextureStageState(1, D3DTSS_COLOROP, &dw_tss_top_value1);
						_pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &dw_tss_aop_value0);
						_pGfx->gl_pd3dDevice->GetTextureStageState(1, D3DTSS_ALPHAOP, &dw_tss_aop_value1);
						_pGfx->gl_pd3dDevice->GetTextureStageState(2, D3DTSS_COLOROP, &dw_tss_top_value2);

						// setting new
						_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
						_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
						_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
						_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
						_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
						_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG0, D3DTA_TFACTOR);
						_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_LERP);
						_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
						_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
						_pGfx->gl_pd3dDevice->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
						UBYTE ubBlendFactor = 0xFF * ptr->tr_fShadowBlendFactor;
						DWORD dwBlendFactor = (ubBlendFactor << 24) | (ubBlendFactor << 16) | (ubBlendFactor << 8) | ubBlendFactor;
						dwBlendFactor |= 0xFF000000;
						_pGfx->gl_pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, dwBlendFactor);

						shaSetTexCoordArray( ptcTexCoords, 0 );
						shaSetTextureMatrix( tt.tt_mShadowMap, 0 );
						shaSetTexCoordArray( ptcTexCoords, 1 );
						shaSetTextureMatrix( tt.tt_mShadowMap, 1 );
						//shaSetTexture( (CTextureData*)NULL, 1 );
					}
					shaEnd();
					// restore old
					if( !_bUseHardwareShaders )
					{
						_pGfx->gl_pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, dw_tfactor);
						_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dw_tss_ta_value0_arg1);
						_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, dw_tss_ta_value0_arg2);
						_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG0, dw_tss_ta_value0_arg0);
						_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, dw_tss_top_value0);
						_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, dw_tss_ta_value1_arg1);
						_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, dw_tss_ta_value1_arg2);
						_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG0, dw_tss_ta_value1_arg0);
						_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, dw_tss_top_value1);
						_pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, dw_tss_aop_value0);
						_pGfx->gl_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, dw_tss_aop_value1);
						_pGfx->gl_pd3dDevice->SetTextureStageState(2, D3DTSS_COLOROP, dw_tss_top_value2);
					}
					TR_SETINFO( ti_ctVisibleTriangles += ctind / 3 );
				}
			}														// yjpark     -->|

			const BOOL	bHasFog = ptrTerrain->tr_ulTerrainFlags & TR_HAS_FOG;
			const BOOL	bHasHaze = ptrTerrain->tr_ulTerrainFlags & TR_HAS_HAZE;

			if( bHasFog )
				RenderTerrainFogView( ptr, iTerrainTile );
			else if( bHasHaze )
				RenderTerrainHazeLayer( ptr, iTerrainTile );

			// Unlock stuff
			shaUnlockArrays();
			TR_SETINFO( tr_ctVisibleVertices += tt.tt_avxVertices.Count() );
			// Clear rendering stuff
			shaClean();
			shaDepthFunc( GFX_LESS_EQUAL );
		}
		ptr->tr_attTiles[iTerrainTile].tt_atriTriangles.Unlock();
	}

	// if optimizing bath tile rendering
	extern INDEX ter_bOptimizeRendering;
	if( ter_bOptimizeRendering )
	{
		// Render batched terrain tiles (if any)
		RenderTerrainViewBatched(ptrTerrain);
	}

	// Disable hardware shaders
	if( _bUseHardwareShaders )
	{
		shaDisablePixelProgram();
		shaDisableVertexProgram();
	}
}

static void RenderTerrainTriangles(CTerrainImp *ptr)
{
	// set wireframe mode
	gfxDisableBlend();
	gfxDisableDepthWrite();
	gfxEnableDepthTest();
	gfxEnableDepthBias();
	gfxPolygonMode(GFX_LINE);
	gfxSetTextureMatrix(NULL);
	gfxDisableTexture();

	// for each terrain tile
	const INDEX cttt = ptr->tr_attTiles.Count();
	for(INDEX itt=0;itt<cttt;itt++) 
	{
		const CTerrainTile &tt = ptr->tr_attTiles[itt];
		const INDEX cttvx = tt.tt_avxVertices.Count();
		const INDEX ctind = tt.tt_atriTriangles.Count()*3;
		ASSERT(cttvx>0 && ctind>=3);

		// Set view matrix for this tile
		Matrix12 mTrans;
		MatrixMultiply(mTrans, _mObjToViewStretch, tt.tt_mTileToTerrain);
		SetViewMatrix(mTrans);

		// Unpack tile vertices
		UnpackTileVertices(ptr,itt);
		GFXVertex *pvxVertices = &_avxTileVertices[0];

		// Get tile triangles
		ptr->tr_attTiles[itt].tt_atriTriangles.Lock(TDAF_LOCK_READ);
		const UWORD *puwIndices = &tt.tt_atriTriangles[0].trt_auwIndices[0];

		// Render tile
		gfxSetVertexArray(pvxVertices,cttvx);
		gfxSetConstantColor(0xFF0000FF);
		gfxLockArrays();
		gfxDrawElements( ctind, puwIndices);
		gfxUnlockArrays();

		ptr->tr_attTiles[itt].tt_atriTriangles.Unlock();
	}

	// set fill mode
	gfxDisableDepthBias();
	gfxPolygonMode(GFX_FILL);
}

#ifndef	FINALVERSION			// yjpark |<--

static void RenderTerrainAttributeTriangles( CTerrainImp *ptr )
{
	CTPoint	pt;
	pt(1) = ptr->tr_vHitPoint(1) / ptr->tr_slAttributeMapSizeAspect;
	pt(2) = ptr->tr_vHitPoint(3) / ptr->tr_slAttributeMapSizeAspect;

	// Calculate the region of drawn attribute triangles
	extern INDEX ter_iAttributeRange;

	const PIX	pixWidth = ptr->tr_pixHeightMapWidth - 1;

	CTRect	rcSource;
	if( ter_iAttributeRange ==0 || ter_iAttributeRange>=pixWidth )
	{
		rcSource.rc_slLeft = 0;
 		rcSource.rc_slRight = pixWidth;
		rcSource.rc_slTop = 0;
		rcSource.rc_slBottom = pixWidth;
	}
	else
	{
		rcSource.rc_slLeft = pt(1) - ter_iAttributeRange/2;
 		rcSource.rc_slRight = pt(1) + ter_iAttributeRange/2;
		rcSource.rc_slTop = pt(2) - ter_iAttributeRange/2;
		rcSource.rc_slBottom = pt(2) + ter_iAttributeRange/2;
	}
	// Check valid region
	if( rcSource.rc_slLeft < 0 ) rcSource.rc_slLeft = 0;
	if( rcSource.rc_slTop < 0 ) rcSource.rc_slTop = 0;
	if( rcSource.rc_slRight > pixWidth ) rcSource.rc_slRight = pixWidth;
	if( rcSource.rc_slBottom > pixWidth ) rcSource.rc_slBottom = pixWidth;

	for(int iY =rcSource.rc_slTop; iY<rcSource.rc_slBottom; iY+=40)
	{
		for(int iX =rcSource.rc_slLeft; iX<rcSource.rc_slRight; iX+=40)
		{
			CTRect rcTemp;
			rcTemp.rc_slLeft = iX;
 			rcTemp.rc_slRight = iX + 40;
			rcTemp.rc_slTop = iY;
			rcTemp.rc_slBottom = iY + 40;

			// Check valid region
			if( rcTemp.rc_slRight > rcSource.rc_slRight ) rcTemp.rc_slRight = rcSource.rc_slRight;
			if( rcTemp.rc_slBottom > rcSource.rc_slBottom ) rcTemp.rc_slBottom = rcSource.rc_slBottom;

			FLOAT3D		*pavVertices;
			GFXColor	*pacColors;
			UWORD		*puwIndices;
			INDEX		ctVertices, ctIndices;

			gfxEnableBlend();
			gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA );
			gfxDisableDepthWrite();

			extern BOOL _bAttributemap_DepthTest;

			if(_bAttributemap_DepthTest)
				gfxDisableDepthTest();
			else
				gfxEnableDepthTest();

			gfxEnableDepthBias();
			gfxSetTextureMatrix(NULL);
			gfxDisableTexture();
			SetViewMatrix( _mObjToView );

			// Mouse point

			// Unwalkable
			TR_ExtractAttrPoligonsInRect( ptr, rcTemp, &pavVertices, ctVertices, &pacColors, &puwIndices, ctIndices, MATT_UNWALKABLE );
			if( pavVertices && puwIndices )
			{
				gfxSetVertexArray( (GFXVertex *)&pavVertices[0], ctVertices );
				gfxSetColorArray( &pacColors[0] );
				gfxDrawElements( ctIndices, puwIndices );
			}
		}
	}

	// Water etc...
	gfxDisableDepthBias();
}

static void RenderTerrainPoints(CTerrainImp *ptr)
{
	gfxEnableDepthBias();
	extern INDEX ter_bShowLerpTargets;
	const BOOL bShowTargets = ter_bShowLerpTargets;

	// for each terrain tile
	const INDEX cttt = ptr->tr_attTiles.Count();
	for(INDEX itt=0;itt<cttt;itt++) {
		const CTerrainTile &tt = ptr->tr_attTiles[itt];
		const QuadTreeNode &qtn = ptr->tr_aqtnQuadTreeNodes[itt];
		
		const FLOAT fLerpFactor = tt.tt_fLODLerpFactor;

		// Set view matrix for this tile
		Matrix12 mTrans;
		MatrixMultiply(mTrans, _mObjToViewStretch, tt.tt_mTileToTerrain);
		SetViewMatrix(mTrans);

		// lock tile vertex array for reading
		ptr->tr_attTiles[itt].tt_avxVertices.Lock(TDAF_LOCK_READ);

		// for each vertex
		const INDEX cttvx = tt.tt_avxVertices.Count();
		for(INDEX itvx=0;itvx<cttvx;itvx++) {
			const TerrainVertex &tvx = tt.tt_avxVertices[itvx];
	
			const FLOAT fX = tvx.tv_ubX;
			const FLOAT fZ = tvx.tv_ubZ;
			const FLOAT fY1 = tvx.tv_uwY;
			const FLOAT fY2 = tvx.tv_uwY2;
			const FLOAT fY = Lerp(fY1,fY2,fLerpFactor);

			const FLOAT3D vx = FLOAT3D(fX, fY1, fZ);
			const FLOAT3D vxL = FLOAT3D(fX, fY, fZ);
			const FLOAT3D vxLT = FLOAT3D(fX, fY2, fZ);

#if _DEBUG
			FLOAT3D vxTest = vx;
			FLOAT3D vMinBox = qtn.qtn_aabbox.minvect;
			FLOAT3D vMaxBox = qtn.qtn_aabbox.maxvect;
			TransformVector(vxTest.vector,tt.tt_mTileToTerrain);
			ASSERT(vxTest(1)>=vMinBox(1) && vxTest(2)>=vMinBox(2) && vxTest(3)>=vMinBox(3));
			ASSERT(vxTest(1)<=vMaxBox(1) && vxTest(2)<=vMaxBox(2) && vxTest(3)<=vMaxBox(3));
#endif

			const FLOAT fPointSize = 3.5f;
			// Draw vertex
			_pdp->DrawPoint3D(vx,0xFF0000FF,fPointSize);
			if(bShowTargets) {
				
				_pdp->DrawPoint3D(vxLT,0x0000FFFF,fPointSize);
				_pdp->DrawPoint3D(vxL, 0x00FF00FF,fPointSize);
			}
		}

		// unlock tile vertex array
		ptr->tr_attTiles[itt].tt_avxVertices.Unlock();
	}
	gfxDisableDepthBias();
}

static void RenderTerrainQuadTree(CTerrainImp *ptr)
{
	// No view matrix
	gfxSetViewMatrix(NULL);

	extern INDEX ter_iShowQuadTreeLev;
	const INDEX ctqtls = ptr->tr_aqtlQuadTreeLevels.Count();
	INDEX iQuadTreeLevel = Clamp(ter_iShowQuadTreeLev,0L,ctqtls-1L);

	const QuadTreeLevel &qtl = ptr->tr_aqtlQuadTreeLevels[iQuadTreeLevel];
	const INDEX ctqtns = qtl.qtl_ctNodes;
	const INDEX iqtnFirst = qtl.qtl_iFirstNode;
	for(INDEX iqtn=0;iqtn<ctqtns;iqtn++) {
		const QuadTreeNode &qtn = ptr->tr_aqtnQuadTreeNodes[iqtn+iqtnFirst];
		DrawWireBox(qtn.qtn_aabbox,0x00FF00FF,_mObjToViewStretch);
		FLOAT3D vCenter = qtn.qtn_aabbox.Center();
		TransformVector(vCenter.vector,_mObjToViewStretch);
		_pdp->DrawPoint3D(vCenter,0xFF00FFFF,3.0f);
	}
}

static CTRect AdjustSelectionRect(CTRect rc)
{
	while(TRUE) {
		const SLONG slWidth  = rc.GetWidth();
		const SLONG slHeight = rc.GetHeight();
		const INDEX ctqdsX = slWidth;
		const INDEX ctqdsZ = slHeight;
		const INDEX ctind = ctqdsX * ctqdsZ * 6;
		if(ctind<65535) {
			break;
		} else {
			rc.rc_slLeft++;
			rc.rc_slTop++;
			rc.rc_slRight--;
			rc.rc_slBottom--;
		}
	}
	return rc;
}

static CStaticStackArray<GFXColor> _acolExtractedColors;
static CStaticStackArray<GFXTexCoord> _atcExtractedTexcoords;
static void RenderSelectionTriangles(CTerrain *ptrTerrain)
{
	extern const CTerrain *_ptrSelTerrain;
	extern CTextureData   *_ptrSelBrush;
	extern COLOR           _colSelColor;
	extern CTRect          _rcSelRectangle;
	extern FLOAT           _fSelStrength;
	extern SelectionFill   _slSelFill;
	extern BufferType      _btSelBufferType;


	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	ASSERT(_ptrSelTerrain!=NULL);
	ASSERT(_ptrSelBrush!=NULL);
	ASSERT(_ptrSelBrush->td_pulFrames!=NULL);
//   ASSERT(_rcSelRectangle.GetWidth()==_ptrSelBrush->td_mexWidth);
//   ASSERT(_rcSelRectangle.GetHeight()==_ptrSelBrush->td_mexHeight);

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	FLOAT3D *pavVertices;
	UWORD *puwIndices;
	INDEX ctVertices, ctIndices;
	
	const CTRect	rcTerrain = CTRect(CTPoint(0,0),CTPoint(ptr->tr_pixHeightMapWidth-1,ptr->tr_pixHeightMapHeight-1));
	CTRect			rcSelRectangle = _rcSelRectangle;									// yjpark |<--
	FLOAT			fOffsetU = 0.0f;
	FLOAT			fOffsetV = 0.0f;
	if( _btSelBufferType == BT_LAYER_MASK || _btSelBufferType == BT_SHADOW_MAP )
	{
		SLONG	slStretch;
		if( _btSelBufferType == BT_LAYER_MASK )
		{
			INDEX	iSelLayer = ptrTerrain->tr_iSelectedLayer;
			slStretch = ptr->tr_atlLayers[iSelLayer].tl_slMaskStretch;
		}
		else if( _btSelBufferType == BT_SHADOW_MAP )
		{
			slStretch = ptr->tr_slShadowMapSizeAspect;
		}

		if( slStretch < 0 )
		{
			rcSelRectangle.rc_slLeft <<= -slStretch;
			rcSelRectangle.rc_slTop <<= -slStretch;
			rcSelRectangle.rc_slRight <<= -slStretch;
			rcSelRectangle.rc_slBottom <<= -slStretch;
		}
		else if( slStretch > 0 )
		{
			rcSelRectangle.rc_slLeft >>= slStretch;
			rcSelRectangle.rc_slTop >>= slStretch;
			rcSelRectangle.rc_slRight >>= slStretch;
			rcSelRectangle.rc_slBottom >>= slStretch;

			const SLONG	slCenterX = ( _rcSelRectangle.rc_slLeft + _rcSelRectangle.rc_slRight ) >> 1;
			const SLONG	slCenterY = ( _rcSelRectangle.rc_slTop + _rcSelRectangle.rc_slBottom ) >> 1;
			const SLONG	slWhichX = slCenterX % ( 1 << slStretch );
			const SLONG	slWhichY = slCenterY % ( 1 << slStretch );
			const	FLOAT	fInvU = 1.0f / (FLOAT)( rcSelRectangle.GetWidth() ) / (FLOAT)( 1 << slStretch );
			const	FLOAT	fInvV = 1.0f / (FLOAT)( rcSelRectangle.GetHeight() ) / (FLOAT)( 1 << slStretch );
			fOffsetU = -fInvU * (FLOAT)slWhichX;
			fOffsetV = -fInvV * (FLOAT)slWhichY;
		}
	}
	else if( _btSelBufferType == BT_ATTRIBUTE_MAP )
	{
		SLONG	slStretch = ptr->tr_slAttributeMapSizeAspect;

		rcSelRectangle.rc_slLeft /= slStretch;
		rcSelRectangle.rc_slTop /= slStretch;
		rcSelRectangle.rc_slRight /= slStretch;
		rcSelRectangle.rc_slBottom /= slStretch;

		const SLONG	slCenterX = ( _rcSelRectangle.rc_slLeft + _rcSelRectangle.rc_slRight ) >> 1;
		const SLONG	slCenterY = ( _rcSelRectangle.rc_slTop + _rcSelRectangle.rc_slBottom ) >> 1;
		const SLONG	slWhichX = slCenterX % slStretch;
		const SLONG	slWhichY = slCenterY % slStretch;
		const FLOAT	fInvU = 1.0f / (FLOAT)( rcSelRectangle.GetWidth() ) / (FLOAT)( slStretch );
		const FLOAT	fInvV = 1.0f / (FLOAT)( rcSelRectangle.GetHeight() ) / (FLOAT)( slStretch );
		fOffsetU = -fInvU * (FLOAT)slWhichX;
		fOffsetV = -fInvV * (FLOAT)slWhichY;
	}																						// yjpark     -->|

	CTRect	rcSelection = ClampRect( rcSelRectangle, rcTerrain );
	rcSelection = AdjustSelectionRect( rcSelection );

	TR_ExtractPoligonsInRect(ptrTerrain,rcSelection,&pavVertices, ctVertices, &puwIndices, ctIndices);
	if(ctVertices<=0 || ctIndices<=0) {
		return;
	}

	// Prepare colors for extracted vertices
	const UWORD *puwHeightMap      = ptr->tr_puwHeightMap;
	const UBYTE *pubEdgeMap        = ptr->tr_pubEdgeMap;
	const ULONG *pulBrushData      = _ptrSelBrush->td_pulFrames;
	const PIX    pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX    pixEdgeMapWidth   = ptr->tr_pixHeightMapWidth-1;

	const SLONG slLeft   = rcSelection.rc_slLeft;
	const SLONG slTop    = rcSelection.rc_slTop;
	const SLONG slWidth  = rcSelection.GetWidth();
	const SLONG slHeight = rcSelection.GetHeight();

	const INDEX ctvtxX = slWidth +1;
	const INDEX ctvtxZ = slHeight+1;
	const INDEX ctvx  = ctvtxX * ctvtxZ;

	const SLONG slOffsetX = rcSelection.rc_slLeft - rcSelRectangle.rc_slLeft;					// yjpark |<--
	const SLONG slOffsetZ = rcSelection.rc_slTop  - rcSelRectangle.rc_slTop;

	SLONG slBrushWidth = rcSelRectangle.GetWidth();
	SLONG slBrushHeight = rcSelRectangle.GetHeight();											// yjpark     -->|
	if(_btSelBufferType==BT_HEIGHT_MAP) {
		slBrushWidth--;
		slBrushHeight--;
	}

	_atcExtractedTexcoords.PopAll();
	_atcExtractedTexcoords.Push(ctvx);
	_acolExtractedColors.PopAll();
	_acolExtractedColors.Push(ctvx);

	const FLOAT fInvStretchX = 1.0f / ptr->tr_vStretch(1);
	const FLOAT fInvStretchZ = 1.0f / ptr->tr_vStretch(3);
	const UBYTE ubAlpha = (UBYTE)(ULONG)(_fSelStrength*255.0f);
	for(INDEX ivx=0;ivx<ctvx;ivx++) {
		FLOAT3D &vx     = pavVertices[ivx];
		GFXTexCoord &tc = _atcExtractedTexcoords[ivx];
		tc.u = ((FLOAT)(( (vx(1)*fInvStretchX) - rcSelection.rc_slLeft)) / (FLOAT)slBrushWidth) + (slOffsetX/(FLOAT)slBrushWidth);		// yjpark
		tc.v = ((FLOAT)(( (vx(3)*fInvStretchZ) - rcSelection.rc_slTop)) / (FLOAT)slBrushHeight) + (slOffsetZ/(FLOAT)slBrushHeight);		// yjpark
		tc.u += fOffsetU;
		tc.v += fOffsetV;

		_acolExtractedColors[ivx] = (_colSelColor&0xFFFFFF00)|ubAlpha;
	}

	gfxEnableBlend();
	gfxBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);

	gfxDisableDepthWrite();
	gfxEnableDepthTest();
	_ptrSelBrush->SetAsCurrent();
	gfxSetTextureMatrix(NULL);
	SetViewMatrix(_mObjToView);

	gfxEnableDepthBias();
	// gfxPolygonMode(GFX_LINE);

	gfxSetVertexArray((GFXVertex*)&pavVertices[0],ctVertices);
	gfxSetColorArray(&_acolExtractedColors[0]);
	gfxSetTexCoordArray(&_atcExtractedTexcoords[0], FALSE);
	gfxDrawElements(ctIndices, puwIndices);

	// set fill mode
	gfxDisableDepthBias();
	// gfxPolygonMode(GFX_FILL);
}
#endif	// FINALVERSION			// yjpark     -->|

#if TR_DEBUG_RAYCASTING
static CStaticStackArray<TerrainTriangle> _attRayCastIndices;
#endif
#if TR_DEBUG_EXTRACTPOLY
static CStaticStackArray<TerrainTriangle> _attExtractPolyIndices;
#endif
#if TR_DEBUG_PLANEFROMPT
static CStaticStackArray<TerrainTriangle> _attPlaneFromPt;
#endif
static void RenderDebugStuff(CTerrain *ptrTerrain)
{
#if TR_DEBUG_RAYCASTING
	{
	gfxSetViewMatrix(NULL); // No view matrix
	gfxSetTextureMatrix(NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	extern CStaticStackArray<TerrainRay> _atrRays;
	extern CStaticStackArray<SimpleTerrainTriangle> _asttRayTris;
	const INDEX cttrr = _atrRays.Count();
	for(INDEX itrr=0;itrr<cttrr;itrr++) {
		const TerrainRay &trr = _atrRays[itrr];
		FLOAT3D v0 = trr.trr_vHitBegin;
		FLOAT3D v1 = trr.trr_vHitEnd;
		FLOAT3D v2 = trr.trr_vHitPoint;
		TransformVector(v0.vector,_mObjToView);
		TransformVector(v1.vector,_mObjToView);
		_pdp->DrawLine3D(v0,v1,0xFFFFFFFF);
		_pdp->DrawPoint3D(v0,0x0000FFFF,5.0f);
		_pdp->DrawPoint3D(v1,0xFF0000FF,5.0f);

		if(v2(1) != UpperLimit(0.0f) && v2(2) != UpperLimit(0.0f) && v2(3) != UpperLimit(0.0f)) {
			TransformVector(v2.vector,_mObjToView);
			_pdp->DrawPoint3D(v2,0x00FF00FF,5.0f);
		}
	}

	_attRayCastIndices.PopAll();
	const INDEX ctrt = _asttRayTris.Count();
	for(INDEX irt=0;irt<ctrt;irt++) {
		TerrainTriangle &tt = _attRayCastIndices.Push();
		tt.trt_auwIndices[0] = (irt*3)+0;
		tt.trt_auwIndices[1] = (irt*3)+1;
		tt.trt_auwIndices[2] = (irt*3)+2;
	}

	gfxEnableDepthBias();
	gfxPolygonMode(GFX_LINE);
	SetViewMatrix(_mObjToView);
	gfxDisableTexture();
	gfxSetConstantColor(0xFFFFFFFF);

	if(_asttRayTris.Count()>0 && _attRayCastIndices.Count()>0) {
		gfxSetVertexArray((GFXVertex*)&_asttRayTris[0].stt_avx[0],ctrt*3);
		gfxDrawElements(ctrt*3, &_attRayCastIndices[0].trt_auwIndices[0]);
	}

	// set fill mode
	gfxDisableDepthBias();
	gfxPolygonMode(GFX_FILL);

	extern INDEX ter_iRecordRayCasting;
	if(ter_iRecordRayCasting==0) {
		_atrRays.PopAll();
		_asttRayTris.PopAll();
	}
	}
#endif

#if TR_DEBUG_SHADOWMAP
	{
	gfxSetViewMatrix(NULL); // No view matrix
	gfxSetTextureMatrix(NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	extern CStaticStackArray<FLOATaabbox3D> _abbLightRegion;
	extern CStaticStackArray<FLOATaabbox3D> _abbUpdateShadowMap;
	extern CStaticStackArray<CTRect> _arcLightRegion;
	extern CStaticStackArray<CTRect> _arcUpdateShadowMap;


	const FLOATaabbox3D bbox = TR_GetTerrainBBox(ptrTerrain);

	const INDEX ctbusm = _abbUpdateShadowMap.Count();
	for(INDEX ibusm=0;ibusm<ctbusm;ibusm++) {
		const FLOATaabbox3D &bbox = _abbUpdateShadowMap[ibusm];
		Matrix12 mIdent;
		MakeIdentityMatrix(mIdent);
		DrawWireBox(bbox,0xFFFF00FF,_mAbsToViewer);
	}
#if 0
	const INDEX ctusm = _arcUpdateShadowMap.Count();
	for(INDEX iusm=0;iusm<ctusm;iusm++) {
		const CTRect &rc = _arcUpdateShadowMap[iusm];
		DrawWireRect(rc,bbox.minvect(2),bbox.maxvect(2),0xFF00FFFF,_mObjToViewStretch);
	}
#endif

	const INDEX ctlr = _abbLightRegion.Count();
	for(INDEX ilr=0;ilr<ctlr;ilr++) {
		const FLOATaabbox3D &bbox = _abbLightRegion[ilr];
		DrawWireBox(bbox,0xFF0000FF,_mObjToViewStretch);
	}

	const INDEX ctrc = _arcLightRegion.Count();
	for(INDEX irc=0;irc<ctrc;irc++) {
		const CTRect &rc = _arcLightRegion[irc];
		DrawWireRect(rc,bbox.minvect(2),bbox.maxvect(2),0x0000FFFF,_mObjToViewStretch);
	}
	}
#endif

#if TR_DEBUG_NORMALS
	{
	gfxSetViewMatrix(NULL); // No view matrix
	gfxSetTextureMatrix(NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	extern CStaticStackArray<FLOAT3D> _anxNormals;
	extern CStaticStackArray<FLOAT3D> _avxVertices;

	const INDEX ctnx=_anxNormals.Count();
	for(INDEX inx=0;inx<ctnx;inx++) {
		FLOAT3D v0 = _avxVertices[inx];
		TransformVector(v0.vector,_mObjToView);
		FLOAT3D v1 = v0 + _anxNormals[inx];
		_pdp->DrawLine3D(v0,v1,0xFFFFFFFF);
	}
	}
#endif

#if TR_DEBUG_EXTRACTPOLY
	{
	gfxSetViewMatrix(NULL); // No view matrix
	gfxSetTextureMatrix(NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	extern CStaticStackArray<FLOAT3D> _avxExtractedPolys;
	extern CStaticStackArray<UWORD>   _auwExtractedPolys;
	extern CStaticStackArray<CTRect> _arcExtractedPolys;

	const FLOATaabbox3D bbox = TR_GetTerrainBBox(ptrTerrain);

	const INDEX ctrc = _arcExtractedPolys.Count();
	for(INDEX irc=0;irc<ctrc;irc++) {
		const CTRect &rc = _arcExtractedPolys[irc];
		DrawWireRect(rc,bbox.minvect(2),bbox.maxvect(2),0x0000FFFF,_mObjToViewStretch);
	}

	gfxEnableDepthBias();
	gfxPolygonMode(GFX_LINE);
	SetViewMatrix(_mObjToView);
	gfxDisableTexture();
	gfxSetConstantColor(0xFFFFFFFF);

	const INDEX ctVertices = _avxExtractedPolys.Count();
	const INDEX ctIndices  = _auwExtractedPolys.Count();
	if(ctVertices>0 && ctIndices>0) {
		gfxSetVertexArray((GFXVertex*)&_avxExtractedPolys[0],ctVertices);
		gfxDrawElements(ctIndices, &_auwExtractedPolys[0]);
	}

	// set fill mode
	gfxDisableDepthBias();
	gfxPolygonMode(GFX_FILL);

	_attExtractPolyIndices.PopAll();
	_avxExtractedPolys.PopAll();
	_auwExtractedPolys.PopAll();
	_arcExtractedPolys.PopAll();
	}
#endif

#if TR_DEBUG_PLANEFROMPT
	{

	SetViewMatrix(_mObjToView);
	gfxSetTextureMatrix(NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	extern CStaticStackArray<SimpleTerrainTriangle> _asttPlaneFromPt;

	_attPlaneFromPt.PopAll();
	const INDEX ctrt = _asttPlaneFromPt.Count();
	for(INDEX irt=0;irt<ctrt;irt++) {
		TerrainTriangle &tt = _attPlaneFromPt.Push();
		tt.trt_auwIndices[0] = (irt*3)+0;
		tt.trt_auwIndices[1] = (irt*3)+1;
		tt.trt_auwIndices[2] = (irt*3)+2;
	}

	gfxEnableDepthBias();
	gfxPolygonMode(GFX_LINE);
	gfxDisableTexture();
	gfxSetConstantColor(0xFFFFFFFF);

	if(_asttPlaneFromPt.Count()>0 && _attPlaneFromPt.Count()>0) {
		gfxSetVertexArray((GFXVertex*)&_asttPlaneFromPt[0].stt_avx[0],ctrt*3);
		gfxDrawElements(ctrt*3, &_attPlaneFromPt[0].trt_auwIndices[0]);
	}

	// set fill mode
	gfxDisableDepthBias();
	gfxPolygonMode(GFX_FILL);

	_asttPlaneFromPt.PopAll();

	}
#endif

}

static void ClearStats(void)
{
	TR_SETINFO(ti_ctVisibleNodes=0);
	TR_SETINFO(ti_ctVisibleTriangles=0);
}

// initialize batch terrain rendering
extern void TR_BeginRenderingView(CAnyProjection3D &apr, CDrawPort *pdp)
{
	_pdp = pdp;
	// prepare and set the projection
	apr->ObjectPlacementL() = CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0));
	apr->Prepare();

//강동민 수정 시작		03.11
	if( apr->pr_bMirror ) 
		apr->pr_plMirrorView.pl_distance -= 0.06f; // -0.06 is because entire projection is offseted by +0.05
//강동민 수정 끝		03.11
//강동민 수정 시작 Water 구현		04.22
	// NOTE : 별로 필요해 보이지는 않지만, Mirror가 되어있으므로...ㅡ.ㅡㆀ
	if( apr->pr_bNiceWater ) 
		apr->pr_plNiceWaterView.pl_distance -= 0.06f; // -0.06 is because entire projection is offseted by +0.05
//강동민 수정 끝 Water 구현			04.22

	_aprProjection = apr;
	_pdp->SetProjection( _aprProjection);

	// remember the abs to viewer transformation
	MatrixVectorToMatrix12(_mAbsToViewer, _aprProjection->pr_ViewerRotationMatrix, 
		-_aprProjection->pr_vViewerPosition*_aprProjection->pr_ViewerRotationMatrix);

	_bUseHardwareShaders = TR_AreHardwareShadersAvailable();
}

// cleanup after batch terrain rendering
extern void TR_EndRenderingView(void)
{
	_pdp = NULL;

	_iForcedLOD = -1;
}

//강동민 수정 시작
extern void TR_GetObjToViewMatrix(Matrix12& matObjToView)
{
	MatrixCopy(matObjToView, _mObjToView);
}

extern void TR_GetStretchMatrix(Matrix12& matStretch)
{
	MatrixCopy(matStretch, _mStretch);
}
//강동민 수정 끝

// setup terrain position
extern void TR_SetTerrainPlacement(const CPlacement3D &pl)
{
	FLOATmatrix3D m;
	MakeRotationMatrixFast( m, pl.pl_OrientationAngle);
	MatrixVectorToMatrix12(_mObjToAbs,m, pl.pl_PositionVector);
}
extern void TR_SetTerrainPlacement(const FLOATmatrix3D &m, const FLOAT3D &v)
{
	MatrixVectorToMatrix12(_mObjToAbs,m, v);
}

extern void TR_AddTerrainViewer(const CTerrain *ptrTerrain, FLOAT3D vViewerAbs, FLOAT fDistFactor/*=-1.0f*/)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	TerrainViewer &tv = _atvTerrainViewers.Push();
	tv.tv_vViewerAbs = vViewerAbs;
	if(fDistFactor==-1.0f) {
		tv.tv_fDistFactor = ptr->tr_fDistFactor;
	} else {
		tv.tv_fDistFactor = fDistFactor;
	}
}

// Renders one terrain
extern void TR_RenderTerrain(CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	ASSERT(_pGfx->gl_ctTextureUnits>=2);    // at least 2 tex units are required

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	// no height map, no terrain
	if(ptr->tr_puwHeightMap==NULL) {
		return;
	}

	TR_SETINFO(Reset());

	// select two shadow maps																	// yjpark |<--
//안태훈 수정 시작	//(Add Sun Moon Entity and etc)(0.2)
	ULONG	ulWorldTime = g_fGWTime;
//안태훈 수정 끝	//(Add Sun Moon Entity and etc)(0.2)
	INDEX	ctShadows = ptr->tr_aptdShadowMaps.Count() - 1;
	INDEX iShadow;
	for( iShadow = 0; iShadow < ctShadows; iShadow++ )
	{
		if( ulWorldTime >= ptr->tr_aulShadowTimes[iShadow] &&
			ulWorldTime < ptr->tr_aulShadowTimes[iShadow + 1] )
		{
			ptr->tr_iCurrentShadow = iShadow;
			ptr->tr_iNextShadow = iShadow + 1;
			break;
		}
	}

	INDEX	iRange, iPos;
	if( iShadow == ctShadows )
	{
		ptr->tr_iCurrentShadow = iShadow;
		ptr->tr_iNextShadow = 0;
		iRange = 108000 - ptr->tr_aulShadowTimes[ptr->tr_iCurrentShadow] + ptr->tr_aulShadowTimes[ptr->tr_iNextShadow];
		iPos = ulWorldTime < ptr->tr_aulShadowTimes[ptr->tr_iNextShadow] ?
					 108000 - ptr->tr_aulShadowTimes[ptr->tr_iCurrentShadow] + ulWorldTime :
					 ulWorldTime - ptr->tr_aulShadowTimes[ptr->tr_iCurrentShadow];
	}
	else
	{
		iRange = ptr->tr_aulShadowTimes[ptr->tr_iNextShadow] - ptr->tr_aulShadowTimes[ptr->tr_iCurrentShadow];
		iPos = ulWorldTime - ptr->tr_aulShadowTimes[ptr->tr_iCurrentShadow];
	}
	ptr->tr_fShadowBlendFactor = 1.0f - ( (FLOAT)iPos / (FLOAT)iRange );						// yjpark     -->|

#ifndef	FINALVERSION			// yjpark |<--
	// if selection is visible
	extern BOOL _bShowTerrainSel;
	extern const CTerrain *_ptrSelTerrain;
	BOOL bShowSelection = _bShowTerrainSel&&(ptrTerrain==_ptrSelTerrain);
	if(bShowSelection) {
		// add terrain viewer to selection point
		UpdateEditingData(ptrTerrain);
	}
#endif	// FINALVERSION			// yjpark     -->|

	// Update terrain data
	UpdateRenderingData(ptrTerrain);
	// Render terrain
	RenderTerrainView(ptrTerrain);

#ifndef	FINALVERSION			// yjpark |<--
	extern BOOL			_bWorldEditorApp;
	extern BufferType	_btSelBufferType;
	if( _bWorldEditorApp && _btSelBufferType == BT_ATTRIBUTE_MAP )
	{
		RenderTerrainAttributeTriangles( ptr );
	}

	extern INDEX ter_bShowTriangles;
	extern INDEX ter_bShowVertices;
	extern INDEX ter_bShowLerpTargets;
	extern INDEX ter_bShowQuadTree;
	
	// Show wireframe 
	if(ter_bShowTriangles) {
		RenderTerrainTriangles(ptr);
	}
	// Show selection
	if(bShowSelection) {
		RenderSelectionTriangles(ptrTerrain);
	}
	// Show vertices
	if(ter_bShowVertices||ter_bShowLerpTargets) {
		RenderTerrainPoints(ptr);
	}
	// Show terrain quad tree
	if(ter_bShowQuadTree) {
		RenderTerrainQuadTree(ptr);
	}
	// Show stuff for debuging
	RenderDebugStuff(ptrTerrain);
#endif	// FINALVERSION			// yjpark     -->|

	// Clear rendering data
	TR_CleanRender(ptrTerrain);
}

// Renders one terrain in wireframe 
extern void TR_RenderWireFrame(CTerrain *ptrTerrain, COLOR colEdges)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	ASSERT(_pGfx->gl_ctTextureUnits>=2);    // at least 2 tex units are required

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	// no height map, no terrain
	if(ptr->tr_puwHeightMap==NULL) {
		return;
	}

#ifndef	FINALVERSION			// yjpark |<--
	// if selection is visible
	extern BOOL _bShowTerrainSel;
	extern const CTerrain *_ptrSelTerrain;
	BOOL bShowSelection = _bShowTerrainSel&&(ptrTerrain==_ptrSelTerrain);
	if(bShowSelection) {
		// add terrain viewer to selection point
		UpdateEditingData(ptrTerrain);
	}
#endif	// FINALVERSION			// yjpark     -->|

	// Update terrain data
	UpdateRenderingData(ptrTerrain);

	RenderTerrainTriangles(ptr);

#ifndef	FINALVERSION			// yjpark |<--
	// Show selection
	if(bShowSelection) {
		RenderSelectionTriangles(ptrTerrain);
	}
#endif	// FINALVERSION			// yjpark     -->|

	// Clear rendering data
	TR_CleanRender(ptrTerrain);
}

// yjpark |<--
extern void TR_RenderTerrainLighting( CTerrain *ptrTerrain, CTextureData* ptdTexture )
{
	if( ptrTerrain == NULL )
		return ;

	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;

	std::list<CLightSource *>::iterator	iterLightSource;
	CLightSource*	pTempLightSource;
	FLOAT3D	*pavVertices;
	UWORD	*puwIndices;
	INDEX	ctVertices, ctIndices;
	CTRect	rcLighting;
	FLOAT3D	vStretch = ptr->tr_vStretch;
	FLOAT	fInvStretchX = 1.0f / vStretch( 1 );
	FLOAT	fInvStretchZ = 1.0f / vStretch( 3 );
	const CTRect	rcTerrain = CTRect( CTPoint( 0, 0 ),
										CTPoint( ptr->tr_pixHeightMapWidth - 1, ptr->tr_pixHeightMapHeight - 1 ) );
	INDEX	ctvtxX, ctvtxZ, ctvx;
	FLOAT	fOffsetX, fOffsetZ, fSize;

	// Set rendering options
	gfxEnableBlend();
	gfxBlendFunc( GFX_DST_COLOR, GFX_SRC_COLOR );
	gfxDisableDepthWrite();
	gfxEnableDepthTest();
	gfxSetTextureWrapping( GFX_CLAMP, GFX_CLAMP );
	gfxSetTextureMatrix(NULL);
	SetViewMatrix(_mObjToView);
	gfxEnableDepthBias();

	// Set texture
	ptdTexture->SetAsCurrent();

	for( iterLightSource = _lstTerrainLightSource.begin(); iterLightSource != _lstTerrainLightSource.end(); iterLightSource++ )
	{
		pTempLightSource = *iterLightSource;
		if(pTempLightSource->ls_rFallOff < 0.1f || pTempLightSource->ls_colColor < 1) continue;

		// Get light information
		FLOAT3D	vPosition = pTempLightSource->ls_penEntity->en_plPlacement.pl_PositionVector;
		FLOAT	fRadius = pTempLightSource->ls_rFallOff;
		COLOR	colColor = 0xFFFFFFFF;//pTempLightSource->ls_colColor;

		// Test sphere in frustum
		FLOAT3D	vViewPosition;
		_aprProjection->PreClip( vPosition, vViewPosition );
		if( _aprProjection->TestSphereToFrustum( vViewPosition, fRadius ) == -1 )
			continue;

		// Prepare terrain lighting
		rcLighting.rc_slLeft = ( vPosition( 1 ) - fRadius ) * fInvStretchX;
		rcLighting.rc_slTop = ( vPosition( 3 ) - fRadius ) * fInvStretchZ;
		rcLighting.rc_slRight = ( vPosition( 1 ) + fRadius ) * fInvStretchX + 1;
		rcLighting.rc_slBottom = ( vPosition( 3 ) + fRadius ) * fInvStretchZ + 1;
		rcLighting = ClampRect( rcLighting, rcTerrain );

		TR_ExtractPoligonsInRect( ptrTerrain, rcLighting, &pavVertices, ctVertices, &puwIndices, ctIndices );
		if( ctVertices <= 0 || ctIndices <= 0 || ctVertices > 65536 || ctVertices > 65536 )
			continue;

		ctvtxX = rcLighting.GetWidth() + 1;
		ctvtxZ = rcLighting.GetHeight() + 1;
		ctvx = ctvtxX * ctvtxZ;

		_atcExtractedTexcoords.PopAll();
		_atcExtractedTexcoords.Push( ctvx );
		_acolExtractedColors.PopAll();
		_acolExtractedColors.Push( ctvx );

		fOffsetX = vPosition( 1 ) - fRadius;
		fOffsetZ = vPosition( 3 ) - fRadius;
		fSize = fRadius * 2.0f;
		for( INDEX ivx = 0; ivx < ctvx; ivx++ )
		{
			FLOAT3D		&vx = pavVertices[ivx];
			GFXTexCoord	&tc = _atcExtractedTexcoords[ivx];
			tc.u = ( vx( 1 ) - fOffsetX ) / fSize;
			tc.v = ( vx( 3 ) - fOffsetZ ) / fSize;
			_acolExtractedColors[ivx] = colColor;
		}

		// Render terrain lighting
		gfxSetVertexArray( (GFXVertex *)&pavVertices[0], ctVertices );
		gfxSetColorArray( &_acolExtractedColors[0] );
		gfxSetTexCoordArray( &_atcExtractedTexcoords[0], FALSE );
		gfxDrawElements( ctIndices, puwIndices );		
	}

	gfxDisableDepthBias();
}
// yjpark     -->|

extern void TR_CleanRender(CTerrain *ptrTerrain)
{
	// Disable texture matrix
	shaClearTextureMatrix(0);

	// Clear fog or haze flags on terrain
	extern INDEX gfx_bRenderReflection;
	// NOTE : 반사맵을 만들고 있는중에 FOG와 Haze를 없애면, 실제 Terrain을 렌더링할때 Fog & Haze가 동작하지 않음.
	if(!gfx_bRenderReflection)
	{
	ptrTerrain->tr_ulTerrainFlags&=~TR_HAS_FOG;
	ptrTerrain->tr_ulTerrainFlags&=~TR_HAS_HAZE;
	}

	extern INDEX _iSelFrameNumber;
	extern BOOL  _bShowTerrainSel;
	extern const CTerrain *_ptrSelTerrain;
	BOOL bShowSelection = _bShowTerrainSel&&(ptrTerrain==_ptrSelTerrain);

	// if selection wasn't visible in this frame
	if(bShowSelection && _iSelFrameNumber<_pGfx->gl_iFrameNumber) {
		// Do not show selection on next rendering
		_bShowTerrainSel = FALSE;
	}

	_atvTerrainViewers.PopAll();
}

extern void TR_ClearTerrainShaders(void)
{
	_shTRShader_Base.DeleteHandles();
	_shTRShader_1TRL.DeleteHandles();
	_shTRShader_2TRL.DeleteHandles();
	_shTRShader_Shadow.DeleteHandles();			// yjpark
	_shTRShader_Fog.DeleteHandles();
}


// yjpark |<--
//-------------------------------------------------------------------------------------------------
// Desc : These contents aren't included in final release version
// ------------------------------------------------------------------------------------------------
#ifndef FINALVERSION

// LOD factor management
extern void TR_SetCurrentDistance(FLOAT fDistFactor)
{
	ASSERTALWAYS("Not implemented yet");
}

extern void TR_ForceCustomTileLOD(INDEX iForcedLOD)
{
	_iForcedLOD = iForcedLOD;
}

#endif	// FINALVERSION
// yjpark     -->|