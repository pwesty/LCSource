#include "stdh.h"
#include <Engine/Math/Vector.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/Plane.h>
#include <Engine/Math/Matrix12.h>
#include <Engine/Math/Placement.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Terrain/TRGenerate.hpp>
#include <Engine/Terrain/TRLayer.h>
#include <Engine/Terrain/TRLayer.hpp>
#include <Engine/Terrain/TRShadows.hpp>
#include <Engine/Terrain/TRMisc.hpp>
#include <Engine/Rendering/Render.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Base/ObjectRestore.h>
#include <Engine/Entities/ShadingInfo.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Effect/CTerrainEffect.h>

extern void AllocateTerrainMaps(CTerrain *ptrTerrain, PIX pixMapWidth, PIX pixMapHeight);
extern BOOL _bFullLayerCopy;

#if TR_DEBUG_PLANEFROMPT
CStaticStackArray<SimpleTerrainTriangle> _asttPlaneFromPt;
#endif

#include <list>															// yjpark |<--
#include <algorithm>
std::list<CLightSource *>	_lstTerrainLightSource;						// yjpark     -->|

CTerrainImp::CTerrainImp(void)
{
	tr_iTerrainVersion = 0;			// yjpark

	tr_puwHeightMap  = NULL;
	tr_puwShadingMap = NULL;
	tr_pubEdgeMap    = NULL;

	tr_ptdEdgeMap = NULL;
	tr_ptdTopMap = NULL;

	tr_pubAttributeMap = NULL;			// yjpark |<--
	tr_ptdShadowMap = NULL;
	tr_iCurrentShadow = 0;
	tr_iNextShadow = 1;
	tr_fShadowOverbright = 1.0f;
	tr_fShadowBlendFactor = 1.0f;
	tr_slAttributeMapSizeAspect = 1;	// yjpark     -->|

	tr_pixHeightMapWidth      = 0;
	tr_pixHeightMapHeight     = 0;
	tr_pixTopMapWidth         = 1024;
	tr_pixTopMapHeight        = 1024;
	tr_slShadowMapSizeAspect  = 0;
	tr_slShadingMapSizeAspect = 0;	

	tr_ctTilesX               = 0;
	tr_ctTilesY               = 0;
	tr_ctQuadsPerTileX        = 32;
	tr_iMaxTileLOD            = 0;
	tr_iFirstTopMapLOD        = 4;
	tr_fDistFactor            = 32;
	tr_vStretch               = FLOAT3D(1.0f,0.0015f,1.0f);
	tr_vMetricSize            = FLOAT3D(0,0,0);
}

CTerrainImp::~CTerrainImp(void)
{
	ASSERT(tr_puwHeightMap==NULL);
	ASSERT(tr_puwShadingMap==NULL);
	ASSERT(tr_pubEdgeMap==NULL);
	ASSERT(tr_ptdEdgeMap==NULL);
	ASSERT(tr_ptdTopMap==NULL);
	ASSERT(tr_ptdShadowMap==NULL);
	ASSERT(tr_aqtnQuadTreeNodes.Count()==0);
	ASSERT(tr_aqtlQuadTreeLevels.Count()==0);
	ASSERT(tr_attTiles.Count()==0);
	ASSERT(tr_atlLayers.Count()==0);
	ASSERT(tr_artRegenQueue.Count()==0);
	ASSERT( tr_aulShadowTimes.Count() == 0 );														// yjpark |<--
	ASSERT( tr_afShadowBlurRadius.Count() == 0 );
	ASSERT( tr_acolObjShadowColor.Count() == 0 );
	ASSERT( tr_aptdShadowMaps.Count() == 0 );
	_lstTerrainLightSource.erase( _lstTerrainLightSource.begin(), _lstTerrainLightSource.end() );	// yjpark     -->|
}

// Update terrain shadow map
extern void TR_UpdateShadowMap(CTerrain *ptrTerrain, const Matrix12 &mObjToAbs, FLOATaabbox3D bboxUpdate,
								BOOL bAutoColectLights/*=TRUE*/, BOOL bUpdateObjectShadow/*= FALSE*/ )	// yjpark
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const BOOL bUpdateShadowMap = _wrpWorldRenderPrefs.GetShadowsType()==CWorldRenderPrefs::SHT_FULL;
	if(bUpdateShadowMap) {
		TRS_BeginShadowMapUpdate(ptrTerrain);
		TRS_SetTerrainPlacement(mObjToAbs);
		if(bAutoColectLights) {
			TRS_ColectLightsFromWorld(ptrTerrain);
		}
		TRS_UpdateShadowMap( ptrTerrain, bboxUpdate, bUpdateObjectShadow );		// yjpark
		TRS_EndShadowMapUpdate(ptrTerrain);
	}
}

// Disacard all shading infos
extern void TR_DiscardShadingInfos(CTerrain *ptrTerrain)
{
	FORDELETELIST( CShadingInfo, si_lnInPolygon, ptrTerrain->tr_lhShadingInfos, itsi) {
		itsi->si_penEntity->en_ulFlags &= ~ENF_VALIDSHADINGINFO;
		itsi->si_lnInPolygon.Remove();
		itsi->si_pbpoPolygon = NULL;
	}
}

// Fill height map with const value
extern void TR_FillHeightMap(CTerrain *ptrTerrain, UWORD uwFill)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain->tr_puwHeightMap!=NULL);
	
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	const PIX pixHeightMapWidth  = ptr->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX pixHeightMapPixels = pixHeightMapWidth*pixHeightMapHeight;

	UWORD *puwDst = ptr->tr_puwHeightMap;
	for(INDEX ipix=0;ipix<pixHeightMapPixels;ipix++) {
		puwDst[ipix] = uwFill;
	}
}

extern void TR_FillEdgeMap(CTerrain *ptrTerrain, UBYTE ubFill)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain->tr_pubEdgeMap!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const PIX pixEdgeMapWidth = ptr->tr_pixHeightMapWidth-1;
	const PIX pixEdgeMapHeight = ptr->tr_pixHeightMapHeight-1;
	const SLONG slMapSize = pixEdgeMapWidth * pixEdgeMapHeight * sizeof(UBYTE);

	memset(ptr->tr_pubEdgeMap,ubFill,slMapSize);

	// Upload terrain edge map
	TR_UploadEdgeMap(ptrTerrain);
}

extern void TR_UploadEdgeMap(CTerrain *ptrTerrain, BOOL bOptimize/*=TRUE*/)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain->tr_pubEdgeMap!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain->tr_ptdEdgeMap!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const PIX pixHeightMapWidth  = ptr->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX pixEdgeMapWidth    = pixHeightMapWidth-1;
	const PIX pixEdgeMapHeight   = pixHeightMapHeight-1;
	const PIX pixEdgeMapPixels   = pixEdgeMapWidth*pixEdgeMapHeight;

	ULONG ulTexFlags = PTF_ALPHAONLY;
	// In world editor layer mask must be static cos of top map regen
	extern BOOL _bWorldEditorApp;
	if(_bWorldEditorApp) {
		ulTexFlags |= PTF_STATIC;
	}

	// Prepare edge map texture
	TR_PrepareTexture(*ptr->tr_ptdEdgeMap,pixEdgeMapWidth,pixEdgeMapHeight,ulTexFlags);
	const UBYTE *pubSrc = ptr->tr_pubEdgeMap;
	ULONG *pulDst = ptr->tr_ptdEdgeMap->td_pulFrames;

	// if optimizing while copying pixels
	if(bOptimize) {
		const INDEX cttt = ptr->tr_attTiles.Count();
		for(INDEX itt=0;itt<cttt;itt++) {
			CTerrainTile &tt = ptr->tr_attTiles[itt];
			const PIX pixLeft    = tt.tt_rcSource.rc_slLeft;
			const PIX pixTop     = tt.tt_rcSource.rc_slTop;
			const PIX pixWidth   = tt.tt_rcSource.GetWidth() - 1;
			const PIX pixHeight  = tt.tt_rcSource.GetHeight() - 1;
			const INDEX ctPixels = pixWidth * pixHeight;

			INDEX ctEdgePixels = 0;
			for(PIX pixY=0;pixY<pixHeight;pixY++) {
				for(PIX pixX=0;pixX<pixWidth;pixX++) {
					const PIX pixRealX = pixLeft+pixX;
					const PIX pixRealY = pixTop+pixY;
					const PIX pix = pixRealX + pixRealY*pixEdgeMapWidth;
					const UBYTE ubEdgePixel = pubSrc[pix];
					ASSERT(pix<pixEdgeMapWidth*pixEdgeMapHeight);
					// Copy pixel in texture
					pulDst[pix] = 0x00FFFFFF | ((ULONG)ubEdgePixel)<<24;
					if(ubEdgePixel>127) {
						ctEdgePixels++;
					}
				}
			}

			// check for edge map optimizations
			tt.tt_ulFlags&=~TTF_BLANK_EDGE_MAP;
			tt.tt_ulFlags&=~TTF_FULL_EDGE_MAP;
			if(ctEdgePixels==0) {
				tt.tt_ulFlags|=TTF_BLANK_EDGE_MAP;
			} else if(ctEdgePixels==ctPixels) {
				tt.tt_ulFlags|=TTF_FULL_EDGE_MAP;
			} else {
				int a=0;
			}
		}
	// else copy without optimizations
	} else {
		for(INDEX ipix=0;ipix<pixEdgeMapPixels;ipix++) {
			pulDst[ipix] = 0x00FFFFFF | ((ULONG)pubSrc[ipix])<<24;
		}
	}
	// Upload edge map texture
	TR_UploadTexture(*ptr->tr_ptdEdgeMap,pixEdgeMapWidth,pixEdgeMapHeight,ulTexFlags);
}

extern void TR_UploadLayerMask(CTerrain *ptrTerrain, INDEX iTerrainLayer, BOOL bOptimize/*=TRUE*/)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	ASSERT(iTerrainLayer>=0 && iTerrainLayer<ptrTerrain->tr_ptrTerrain->tr_atlLayers.Count());
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer *ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];
	CTerrainLayerImp *ptrl = ptlLayer->trl_trlLayerImp;

	const PIX pixHeightMapWidth  = ptr->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX pixLayerMaskWidth  = ( pixHeightMapWidth - 1 ) << ptlLayer->tl_slMaskStretch;			// yjpark
	const PIX pixLayerMaskHeight = ( pixHeightMapHeight - 1 ) << ptlLayer->tl_slMaskStretch;		// yjpark
	const PIX pixLayerMaskPixels = pixLayerMaskWidth*pixLayerMaskHeight;

	ULONG ulTexFlags = PTF_ALPHAONLY;
	// In world editor layer mask must be static cos of top map regen
	extern BOOL _bWorldEditorApp;
	if(_bWorldEditorApp) {
		ulTexFlags |= PTF_STATIC;
	}

	// Prepare layer mask texture
	TR_PrepareTexture(*ptrl->trl_ptdLayerMask,pixLayerMaskWidth,pixLayerMaskHeight,ulTexFlags);
	const UBYTE *pubSrc = ptrl->trl_pubLayerMask;
	ULONG *pulDst = ptrl->trl_ptdLayerMask->td_pulFrames;
	
	// if optimizing while copying pixels
	if(bOptimize) {
		const INDEX cttt = ptr->tr_attTiles.Count();
		for(INDEX itt=0;itt<cttt;itt++) {
			CTerrainTile &tt = ptr->tr_attTiles[itt];
			CTerrainLayer *ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];

			const PIX pixLeft    = tt.tt_rcSource.rc_slLeft << ptlLayer->tl_slMaskStretch;				// yjpark |<--
			const PIX pixTop     = tt.tt_rcSource.rc_slTop << ptlLayer->tl_slMaskStretch;
			const PIX pixWidth   = ( tt.tt_rcSource.GetWidth() - 1 ) << ptlLayer->tl_slMaskStretch;
			const PIX pixHeight  = ( tt.tt_rcSource.GetHeight() - 1 ) << ptlLayer->tl_slMaskStretch;	// yjpark     -->|
			const INDEX ctPixels = pixWidth * pixHeight;

			INDEX ctLayerPixels = 0;
			for(PIX pixY=0;pixY<pixHeight;pixY++) {
				for(PIX pixX=0;pixX<pixWidth;pixX++) {
					const PIX pixRealX = pixLeft+pixX;
					const PIX pixRealY = pixTop+pixY;
					const PIX pix = pixRealX + pixRealY*pixLayerMaskWidth;
					const UBYTE ubLayerPixel = pubSrc[pix];
					ASSERT(pix<pixLayerMaskWidth*pixLayerMaskHeight);
					// Copy pixel in texture
					pulDst[pix] = 0x00FFFFFF | ((ULONG)ubLayerPixel)<<24;
					if(ubLayerPixel>0) {
						ctLayerPixels++;
					}
				}
			}

			if(iTerrainLayer<32) {
				const ULONG ulVisLayerMask = (1UL<<iTerrainLayer);
				// if no layer pixels for this tile
				if(ctLayerPixels==0) {
					// Mark layer invisible for this tile
					tt.tt_ulVisLayers&=~ulVisLayerMask;
				} else {
					// Mark layer visible for this tile
					tt.tt_ulVisLayers|=ulVisLayerMask;
				}
			}
		}
	// Copy pixels in layer mask texture without optimizations
	} else {
		for(INDEX ipix=0;ipix<pixLayerMaskPixels;ipix++) {
			pulDst[ipix] = 0x00FFFFFF | ((ULONG)pubSrc[ipix])<<24;
		}
		// for each terrain tile
		const INDEX cttt = ptr->tr_attTiles.Count();
		for(INDEX itt=0;itt<cttt;itt++) {
			CTerrainTile &ttTile = ptr->tr_attTiles[itt];
			const ULONG ulVisLayerMask = (1UL<<iTerrainLayer);
			ttTile.tt_ulVisLayers|=ulVisLayerMask;
		}
	}
	// Upload layer mask texture
	TR_UploadTexture(*ptrl->trl_ptdLayerMask,pixLayerMaskWidth,pixLayerMaskHeight,ulTexFlags);
}

// Copy terrain from other terrain
extern void TR_CopyTerrain(CTerrain *ptrTerrainDst, const CTerrain *ptrTerrainSrc)
{
	ASSERT(ptrTerrainSrc!=NULL);
	ASSERT(ptrTerrainSrc->tr_ptrTerrain!=NULL);

	ASSERT(ptrTerrainDst!=NULL);
	ASSERT(ptrTerrainDst->tr_ptrTerrain!=NULL);

	CTerrainImp *ptrDst = ptrTerrainDst->tr_ptrTerrain;
	const CTerrainImp *ptrSrc = ptrTerrainSrc->tr_ptrTerrain;
	TR_ClearTerrain(ptrTerrainDst);

	ASSERT(ptrDst->tr_ptdEdgeMap==NULL);
	ASSERT(ptrDst->tr_ptdShadowMap==NULL);
	ASSERT(ptrDst->tr_ptdTopMap==NULL);
	ASSERT( ptrDst->tr_aulShadowTimes.Count() == 0 );					// yjpark |<--	
	ASSERT( ptrDst->tr_afShadowBlurRadius.Count() == 0 );
	ASSERT( ptrDst->tr_acolObjShadowColor.Count() == 0 );
	ASSERT( ptrDst->tr_aptdShadowMaps.Count() == 0 );					// yjpark     -->|
	ptrDst->tr_ptdEdgeMap   = TR_CreateTexture();
	ptrDst->tr_ptdTopMap    = TR_CreateTexture();

	const PIX pixHeightMapWidth  = ptrSrc->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight = ptrSrc->tr_pixHeightMapHeight;
	const PIX pixEdgeMapWidth    = pixHeightMapWidth-1;
	const PIX pixEdgeMapHeight   = pixHeightMapHeight-1;
	const PIX pixLayerMaskWidth  = pixHeightMapWidth-1;
	const PIX pixLayerMaskHeight = pixHeightMapHeight-1;

	const SLONG slHeightMapSize  = pixHeightMapWidth * pixHeightMapHeight * sizeof(UWORD);
	const SLONG slEdgeMapSize    = pixEdgeMapWidth * pixEdgeMapHeight * sizeof(UBYTE);
	const SLONG slLayerMaskSize  = pixLayerMaskWidth * pixLayerMaskHeight * sizeof(UBYTE);
	
	const SLONG slShadowMapSizeAspect = ptrSrc->tr_slShadowMapSizeAspect;
	const SLONG slShadingMapSizeAspect = ptrSrc->tr_slShadingMapSizeAspect;
	const PIX pixShadowMapWidth   = GetShadowMapWidth(pixHeightMapWidth, slShadowMapSizeAspect);
	const PIX pixShadowMapHeight  = GetShadowMapHeight(pixHeightMapHeight, slShadowMapSizeAspect);
	const FLOAT3D &vMetricSize   = ptrSrc->tr_vMetricSize;

	const INDEX	ctShadowMaps = ptrSrc->tr_aptdShadowMaps.Count();		// yjpark |<--
	ptrDst->tr_slShadowMapSizeAspect = slShadowMapSizeAspect;
	ptrDst->tr_slShadingMapSizeAspect = slShadingMapSizeAspect;
	ptrDst->tr_aptdShadowMaps.New( ctShadowMaps );
	ptrDst->tr_aulShadowTimes.New( ctShadowMaps + 1 );
	ptrDst->tr_afShadowBlurRadius.New( ctShadowMaps );
	ptrDst->tr_acolObjShadowColor.New( ctShadowMaps );
	for( INDEX itrs = 0; itrs < ctShadowMaps; itrs++ )
		ptrDst->tr_aptdShadowMaps[itrs] = TR_CreateTexture();

	extern BOOL	_bWorldEditorApp;
	if( _bWorldEditorApp )
	{
		ptrDst->tr_ptdShadowMap = TR_CreateTexture();
		ptrDst->tr_slAttributeMapSizeAspect = ptrSrc->tr_slAttributeMapSizeAspect;
	}																	// yjpark     -->|

	// Allocate terrain height, edge and shadow maps
	AllocateTerrainMaps(ptrTerrainDst, pixHeightMapWidth, pixHeightMapHeight);
	// Mark terrain as initialized
	ptrTerrainDst->tr_ulTerrainFlags|=TR_INITIALIZED;

	const UWORD *puwSrcHeightMap = ptrSrc->tr_puwHeightMap;
	const UBYTE *pubSrcEdgeMap   = ptrSrc->tr_pubEdgeMap;
	UWORD *puwDstHeightMap = ptrDst->tr_puwHeightMap;
	UBYTE *pubDstEdgeMap = ptrDst->tr_pubEdgeMap;

	// Copy height and edge maps
	memcpy(puwDstHeightMap, puwSrcHeightMap, slHeightMapSize);
	memcpy(pubDstEdgeMap, pubSrcEdgeMap, slEdgeMapSize);
	// Upload edge map
	TR_UploadEdgeMap(ptrTerrainDst);

	// for each terrain layer
	const INDEX cttrl=ptrSrc->tr_atlLayers.Count();
	for(INDEX itrl=0;itrl<cttrl;itrl++) {
		const CTerrainLayer *ptlLayer = &ptrSrc->tr_atlLayers[itrl];
		// Copy terrain layer
		INDEX iTerrainLayer = TRL_CreateLayer_t(ptrTerrainDst, CTFILENAME("Data\\Textures\\Editor\\Default.tex"));
		CTerrainLayer &tlDst = ptrDst->tr_atlLayers[iTerrainLayer];
		// Enable full layer copy
		ASSERT(!_bFullLayerCopy);
		CObjectRestore<BOOL> or(_bFullLayerCopy, TRUE);
		// Copy layer
		TRL_CopyLayer(ptrTerrainDst, iTerrainLayer, *ptlLayer);
	}

	// Set terrain size
	TR_SetTerrainSize(ptrTerrainDst, vMetricSize);
	// Regenerate terrain data
	TR_RegenerateAll(ptrTerrainDst);
}

// Bind textures used by terrain
extern void TR_BindTerrainTextures(CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	if( !( ptrTerrain->tr_ulTerrainFlags & TR_INITIALIZED ) )
		return;

	// Upload shadow map															// yjpark |<--
	extern BOOL	_bWorldEditorApp;
	if( _bWorldEditorApp )
	{
		ptr->tr_ptdShadowMap->td_ulInternalFormat = TS.ts_tfRGBA8;
		ptr->tr_ptdShadowMap->SetAsCurrent( 0, TRUE );
	}

	if( _bWorldEditorApp && ptr->tr_iTerrainVersion < 16 )
	{
		// Upload edge map
		TR_UploadEdgeMap(ptrTerrain);

		// Upload top map
		ptr->tr_ptdTopMap->td_ulInternalFormat = TS.ts_tfRGBA8;
		ptr->tr_ptdTopMap->SetAsCurrent(0,TRUE);

		for( INDEX itrs = 0; itrs < ptr->tr_aptdShadowMaps.Count(); itrs++ )
		{
			ptr->tr_aptdShadowMaps[itrs]->td_ulInternalFormat = TS.ts_tfRGBA8;
			ptr->tr_aptdShadowMaps[itrs]->SetAsCurrent( 0, TRUE );
		}

		// Upload layer mask
		const INDEX	cttrl = ptr->tr_atlLayers.Count();
		for( INDEX itrl = 0; itrl < cttrl; itrl++ )
			TR_UploadLayerMask( ptrTerrain, itrl );
	}																				// yjpark     -->|
}

// Unbind textures used by terrain
extern void TR_UnbindTerrainTextures(CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	if( !( ptrTerrain->tr_ulTerrainFlags & TR_INITIALIZED ) )
		return;

	extern BOOL	_bWorldEditorApp;								// yjpark |<--
	if( _bWorldEditorApp )
	{
		ptr->tr_ptdShadowMap->td_tpLocal.Clear();
		ptr->tr_ptdShadowMap->Unbind();
	}

	if( _bWorldEditorApp && ptr->tr_iTerrainVersion < 16 )
	{
		ptr->tr_ptdEdgeMap->td_tpLocal.Clear();
		ptr->tr_ptdEdgeMap->Unbind();

		ptr->tr_ptdTopMap->td_tpLocal.Clear();
		ptr->tr_ptdTopMap->Unbind();

		for( INDEX itrs = 0; itrs < ptr->tr_aptdShadowMaps.Count(); itrs++ )
		{
			ptr->tr_aptdShadowMaps[itrs]->td_tpLocal.Clear();
			ptr->tr_aptdShadowMaps[itrs]->Unbind();
		}

		const INDEX	cttrl = ptr->tr_atlLayers.Count();
		for( INDEX itrl = 0; itrl < cttrl; itrl++ )
		{
			CTerrainLayer *ptlLayer = &ptr->tr_atlLayers[itrl];
			CTerrainLayerImp *ptrl = ptlLayer->trl_trlLayerImp;

			if( _bWorldEditorApp )
			{
				ptlLayer->trl_ptdThumbnail->td_tpLocal.Clear();
				ptlLayer->trl_ptdThumbnail->Unbind();
			}
			ptrl->trl_ptdLayerMask->td_tpLocal.Clear();
			ptrl->trl_ptdLayerMask->Unbind();
		}
	}															// yjpark     -->|
}

// Set terrain metric size
extern void TR_SetTerrainSize(CTerrain *ptrTerrain, FLOAT3D vSize)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const PIX pixHeightMapWidth  = ptr->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const FLOAT fStretchX = vSize(1) / (pixHeightMapWidth-1);
	const FLOAT fStretchY = vSize(2) / 65535.0f;
	const FLOAT fStretchZ = vSize(3) / (pixHeightMapHeight-1);

	ptr->tr_vMetricSize = vSize;
	ptr->tr_vStretch    = FLOAT3D(fStretchX, fStretchY, fStretchZ);

	// Update attribute map size
	extern BOOL	_bWorldEditorApp;
	if( _bWorldEditorApp )							
	{
		TR_SetAttributeMapSize( ptrTerrain );
	}
}

// Get plane from given point
extern FLOATplane3D TR_GetPlaneFromPoint(const CTerrain *ptrTerrain, const Matrix12 &mTerrain, const FLOAT3D &vAbsPoint)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const PIX pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const FLOAT3D &vStretch = ptr->tr_vStretch;
	const UWORD *puwHeightMap = ptr->tr_puwHeightMap;

	Matrix12 mStretch;
	Matrix12 mInvObjToAbs;
	Matrix12 mObjToAbsStretch;
	MatrixTranspose(mInvObjToAbs, mTerrain);
	MakeStretchMatrix(mStretch, vStretch);
	MatrixMultiply(mObjToAbsStretch, mTerrain, mStretch);

	FLOAT3D vRelPoint = vAbsPoint;
	TransformVector(vRelPoint.vector, mInvObjToAbs);
	vRelPoint(1) /= vStretch(1);
	vRelPoint(3) /= vStretch(3);

	const PIX pixX = floor(vRelPoint(1));
	const PIX pixZ = floor(vRelPoint(3));
	const FLOAT fXRatio = vRelPoint(1) - pixX;
	const FLOAT fZRatio = vRelPoint(3) - pixZ;
	
	const INDEX ipix = pixX + pixZ*pixHeightMapWidth;

	const FLOAT fY0 = puwHeightMap[ipix+0];
	const FLOAT fY1 = puwHeightMap[ipix+1];
	const FLOAT fY2 = puwHeightMap[ipix+pixHeightMapWidth+0];
	const FLOAT fY3 = puwHeightMap[ipix+pixHeightMapWidth+1];

	FLOAT3D v0((pixX+0), fY0, (pixZ+0));
	FLOAT3D v1((pixX+1), fY1, (pixZ+0));
	FLOAT3D v2((pixX+0), fY2, (pixZ+1));
	FLOAT3D v3((pixX+1), fY3, (pixZ+1));

	TransformVector(v0.vector, mObjToAbsStretch);
	TransformVector(v1.vector, mObjToAbsStretch);
	TransformVector(v2.vector, mObjToAbsStretch);
	TransformVector(v3.vector, mObjToAbsStretch);

	FLOAT3D pl0;
	FLOAT3D pl1;
	FLOAT3D pl2;

	const BOOL bFacing = ipix&1;
	if(bFacing) {
		if(1.0f-fXRatio>=fZRatio) {
			pl0 = v0;
			pl1 = v2;
			pl2 = v1;
		} else {
			pl0 = v1;
			pl1 = v2;
			pl2 = v3;
		}
	} else {
		if(fXRatio>=fZRatio) {
			pl0 = v0;
			pl1 = v3;
			pl2 = v1;
		} else {
			pl0 = v2;
			pl1 = v3;
			pl2 = v0;
		}
	}

#if TR_DEBUG_PLANEFROMPT
	_asttPlaneFromPt.Push() = SimpleTerrainTriangle(pl0, pl1, pl2);
#endif
	return FLOATplane3D(pl0, pl1, pl2);
}

// Get terrain bounding box
extern FLOATaabbox3D TR_GetTerrainBBox(const CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	// Get last quad tree level
	const INDEX ctqtl = ptr->tr_aqtlQuadTreeLevels.Count();
	const QuadTreeLevel &qtl = ptr->tr_aqtlQuadTreeLevels[ctqtl-1];
	ASSERT(qtl.qtl_ctNodes==1);
	// Get quad tree node for last level
	const QuadTreeNode &qtn = ptr->tr_aqtnQuadTreeNodes[qtl.qtl_iFirstNode];
	return qtn.qtn_aabbox;
}

extern FLOATaabbox3D TR_GetStretchedTerrainBBox(const CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	FLOATaabbox3D bbox = TR_GetTerrainBBox(ptrTerrain);
	Matrix12 mStretch;
	MakeStretchMatrix(mStretch, ptr->tr_vStretch);
	TransformVector(bbox.minvect.vector,mStretch);
	TransformVector(bbox.maxvect.vector,mStretch);
	return bbox;
}

/*
 * TerrainMisc
 */

extern BOOL TR_IsTerrainBelowPoint(const CTerrain *ptrTerrain, const FLOAT3D &vPoint,
																	 FLOAT fMaxDist, const FLOAT3D &vGravityDir)
{
	return TRUE;
}

// yjpark |<--
extern void TR_AddTerrainLighting( CLightSource *plsTerrainLight )
{
	if( std::find( _lstTerrainLightSource.begin(), _lstTerrainLightSource.end(), plsTerrainLight )
		== _lstTerrainLightSource.end() )
	{
		_lstTerrainLightSource.push_back( plsTerrainLight );
	}	
}

extern void TR_RemoveTerrainLighting( CLightSource *plsTerrainLight )
{
	std::list<CLightSource *>::iterator	iterLightSource;
	iterLightSource = std::find( _lstTerrainLightSource.begin(), _lstTerrainLightSource.end(), plsTerrainLight );
	if( iterLightSource	!= _lstTerrainLightSource.end() )
	{
		_lstTerrainLightSource.erase( iterLightSource );
	}
}

extern INDEX TR_GetTerrainLayerSound( FLOAT fX, FLOAT fZ )
{
	// 월드내에 터레인이 하나므로...
	CTerrain	*ptrTerrain = CTerrainEffect::GetTerrain();
	if( ptrTerrain == NULL )
		return -2;

	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;
	ASSERT( ptr != NULL );

	if( ptr == NULL )
		return -1;

	const PIX	pixMapWidth = ptr->tr_pixHeightMapWidth - 1;
	const PIX	pixMapHeight = ptr->tr_pixHeightMapHeight - 1;
	const INDEX	cttrl = ptr->tr_atlLayers.Count();
	INDEX		iSelLayer = cttrl - 1;
	UBYTE		ubMaxValue = 0;
	ULONG		ulSumValue = 0;

	for( INDEX itrl = cttrl - 1; itrl > 0; itrl-- )
	{
		const CTerrainLayer		*ptrLayer = &ptr->tr_atlLayers[itrl];
		const PIX	pixMulStretch = 1 << ptrLayer->tl_slMaskStretch;
		const PIX	pixLayerMaskWidth = pixMapWidth * pixMulStretch;
		const PIX	pixLayerMaskHeight = pixMapHeight * pixMulStretch;
		const FLOAT	fDiv = 1.0f / ( 6.0f / FLOAT(pixMulStretch) );
		const PIX	pixX = fX * fDiv;
		const PIX	pixY = fZ * fDiv;

		if (pixX < 0 || pixY < 0 || pixX > pixLayerMaskWidth || pixY > pixLayerMaskHeight)
		{
			continue;
		}

		const UBYTE	ubCurValue = ptrLayer->trl_trlLayerImp->trl_pubLayerMask[pixX + pixY * pixLayerMaskWidth];
		ulSumValue += ubCurValue;

		if( ubCurValue > ubMaxValue )
		{
			iSelLayer = itrl;
			ubMaxValue = ubCurValue;

			if( ubMaxValue > 255 - ulSumValue )
				return ptr->tr_atlLayers[iSelLayer].tl_iSoundIndex;
		}
	}

	if( ubMaxValue > 255 - ulSumValue )
		return ptr->tr_atlLayers[iSelLayer].tl_iSoundIndex;
	else
		return ptr->tr_atlLayers[0].tl_iSoundIndex;
}
// yjpark     -->|


// yjpark |<--
//-------------------------------------------------------------------------------------------------
// Desc : These contents aren't included in final release version
// ------------------------------------------------------------------------------------------------
#ifndef FINALVERSION

// Update terrain top map
extern void TR_UpdateTopMap(CTerrain *ptrTerrain)
{
	extern BOOL _bWorldEditorApp;
	extern INDEX ter_bUpdateTopMapOnEdit;
	// if in editor and update is allowed
	if(_bWorldEditorApp && ter_bUpdateTopMapOnEdit) {
		// Bake top map 
		TR_BakeTopMap(ptrTerrain);
	// if update isn't allowed
	} else if(_bWorldEditorApp) {
		// Flag that sync must be done on save time
		ptrTerrain->tr_ulTerrainFlags|=TR_SYNC_TOPMAP;
	}
}

// Change distance factor (call TR_RegenerateAll after this)
extern void TR_SetDistanceFactor(CTerrain *ptrTerrain, FLOAT fDistFactor)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	ptr->tr_fDistFactor = fDistFactor;
}

// Get terrain metric size
extern FLOAT3D TR_GetTerrainSize(const CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	return ptr->tr_vMetricSize;
}

// Get terrain stretch
extern FLOAT3D TR_GetTerrainStretch(const CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	return ptr->tr_vStretch;
}

// Get terrain height map width
extern PIX TR_GetHeightMapWidth(const CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	const CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	return ptr->tr_pixHeightMapWidth;
}

// Get terrain height map height
extern PIX TR_GetHeightMapHeight(const CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	const CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	return ptr->tr_pixHeightMapHeight;
}

// Get terrain top map width
extern PIX TR_GetTopMapWidth(const CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	const CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	return ptr->tr_pixTopMapWidth;
}

// Get terrain top map height
extern PIX TR_GetTopMapHeight(const CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	const CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	return ptr->tr_pixTopMapWidth;
}

extern SLONG TR_GetShadowMapAspect(const CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	return ptr->tr_slShadowMapSizeAspect;
}

extern SLONG TR_GetShadingMapAspect(const CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	return ptr->tr_slShadingMapSizeAspect;
}

#endif	// FINALVERSION
// yjpark     -->|