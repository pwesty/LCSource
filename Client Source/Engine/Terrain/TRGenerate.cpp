#include "stdh.h"
#include <Engine/Base/Memory.h>
#include <Engine/Base/FileName.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Base/ObjectRestore.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Terrain/TRLayer.h>
#include <Engine/Terrain/TRLayer.hpp>
#include <Engine/Terrain/TRMisc.hpp>
#include <Engine/Terrain/TRGenerate.hpp>
#include <Engine/Math/Matrix12.h>
#include <Engine/Graphics/ImageInfo.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/Stream.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Brushes/Brush.h>

#include <Engine/Base/StackDump.h>

static CListHead _lhUnusedArrays;  // list of unused data arrays
extern TerrainInfo _tiTerrainInfo; // Terrain info

template <class Type>
extern void TerrainMapCrop(Type *ptDst, SLONG slDstWidth, SLONG slDstHeight,
							const Type *ptSrc, SLONG slSrcWidth, SLONG slSrcHeight, UBYTE ubFill)
{
	SLONG slDstSize = slDstWidth * slDstHeight * sizeof(Type);
	memset(ptDst, ubFill, slDstSize);
	SLONG slWidth    = Min(slSrcWidth,slDstWidth);
	SLONG slHeight   = Min(slSrcHeight,slDstHeight);
	SLONG slNewStepX = ClampDn(slDstWidth-slSrcWidth,0L);
	SLONG slOldStepX = ClampDn(slSrcWidth-slDstWidth,0L);

	SLONG slNew = 0;
	SLONG slOld = 0;
	for(INDEX iy=0;iy<slHeight;iy++) {
		for(INDEX ix=0;ix<slWidth;ix++) {
			ptDst[slNew] = ptSrc[slOld];
			slNew++;
			slOld++;
		}
		slNew += slNewStepX;
		slOld += slOldStepX;
	}
}

// Copy terrain map from one mem location to another with optional map resizing
template <class Type>
static void TerrainMapCopy(Type *ptDst, SLONG slDstWidth, SLONG slDstHeight,
							const Type *ptSrc, SLONG slSrcWidth, SLONG slSrcHeight, UBYTE ubFill)
{
	SLONG slDstMapSize = slDstWidth * slDstHeight * sizeof(Type);

	// if no source map 
	if(ptSrc==NULL) {
		// Clear destination map
		memset(ptDst,ubFill,slDstMapSize);
	} else {
		TerrainMapCrop(ptDst, slDstWidth, slDstHeight, ptSrc, slSrcWidth, slSrcHeight, ubFill);
	}
}

// Change terrain maps size (height, edge and layer)
extern void AllocateTerrainMaps(CTerrain *ptrTerrain, PIX pixMapWidth, PIX pixMapHeight)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const PIX pixNewHeightMapWidth  = pixMapWidth;
	const PIX pixNewHeightMapHeight = pixMapHeight;
	const PIX pixOldHeightMapWidth  = ptr->tr_pixHeightMapWidth;
	const PIX pixOldHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX pixNewEdgeMapWidth    = pixMapWidth-1;
	const PIX pixNewEdgeMapHeight   = pixMapHeight-1;
	const PIX pixOldEdgeMapWidth    = pixOldHeightMapWidth-1;
	const PIX pixOldEdgeMapHeight   = pixOldHeightMapHeight-1;
	const PIX pixNewLayerMapWidth   = pixMapWidth-1;
	const PIX pixNewLayerMapHeight  = pixMapHeight-1;
	const PIX pixOldLayerMapWidth   = pixOldHeightMapWidth-1;
	const PIX pixOldLayerMapHeight  = pixOldHeightMapHeight-1;

	const SLONG slHeightMapSize = pixNewHeightMapWidth*pixNewHeightMapHeight*sizeof(UWORD);
	const SLONG slEdgeMapSize = pixNewEdgeMapWidth*pixNewEdgeMapHeight*sizeof(UBYTE);

	UWORD *puwOldHeightMap = ptr->tr_puwHeightMap;
	UBYTE *pubOldEdgeMap = ptr->tr_pubEdgeMap;

	// Allocate memory for new height and edge maps
	UWORD *puwNewHeightMap = (UWORD*)AllocMemory(slHeightMapSize);
	UBYTE *pubNewEdgeMap = (UBYTE*)AllocMemory(slEdgeMapSize);
	#pragma message(">> edge map uses UBYTE per pixel")

	// Copy height and edge maps to new memory
	TerrainMapCopy(puwNewHeightMap,pixNewHeightMapWidth,pixNewHeightMapHeight,
								 puwOldHeightMap,pixOldHeightMapWidth,pixOldHeightMapHeight, 0x00);
	TerrainMapCopy(pubNewEdgeMap,pixNewEdgeMapWidth,pixNewEdgeMapHeight,
								 pubOldEdgeMap,pixOldEdgeMapWidth,pixOldEdgeMapHeight, 0xFF);

	// Free old height map memory
	if(puwOldHeightMap!=NULL) {
		FreeMemory(puwOldHeightMap);
		puwOldHeightMap = NULL;
	}
	// Free old edge map memory
	if(pubOldEdgeMap) {
		FreeMemory(pubOldEdgeMap);
		pubOldEdgeMap = NULL;
	}

	// Apply new height and edge maps
	ptr->tr_puwHeightMap = puwNewHeightMap;
	ptr->tr_pubEdgeMap   = pubNewEdgeMap;
	// Apply new maps size
	ptr->tr_pixHeightMapWidth  = pixNewHeightMapWidth;
	ptr->tr_pixHeightMapHeight = pixNewHeightMapHeight;

	// Upload edge map (without tile optimizations)
	if( ptrTerrain->tr_ptrTerrain->tr_ptdEdgeMap == NULL )
		ptrTerrain->tr_ptrTerrain->tr_ptdEdgeMap = TR_CreateTexture();
	TR_UploadEdgeMap(ptrTerrain, FALSE);

	extern BOOL	_bWorldEditorApp;															// yjpark |<--
	if( _bWorldEditorApp || ptr->tr_iTerrainVersion < 16 )
	{
		// Update shadow map size (shadow map size is relative to height map size)
		const SLONG slShadowMapSizeAspect = ptr->tr_slShadowMapSizeAspect;
		const SLONG slShadingMapSizeAspect = ptr->tr_slShadingMapSizeAspect;
		TR_SetShadowMapSize(ptrTerrain, slShadowMapSizeAspect, slShadingMapSizeAspect);
	}

	if( _bWorldEditorApp  )
	{
		extern void TRL_UpdateLayerSize(CTerrain *ptrTerrain, INDEX iTerrainLayer, PIX pixNewLMW, PIX pixNewLMH,
										PIX pixOldLMW, PIX pixOldLMH);
		// for each terrain layer
		const INDEX	cttrl = ptr->tr_atlLayers.Count();
		for( INDEX itrl = 0; itrl < cttrl; itrl++ )
		{
			// Apply new layer size
			CTerrainLayer &tlLayer = ptr->tr_atlLayers[itrl];
			const PIX pixNewLayerMaskWidth = ( pixMapWidth - 1 ) << tlLayer.tl_slMaskStretch;
			const PIX pixNewLayerMaskHeight = ( pixMapHeight - 1 ) << tlLayer.tl_slMaskStretch;
			const PIX pixOldLayerMaskWidth = ( pixOldHeightMapWidth - 1) << tlLayer.tl_slMaskStretch;
			const PIX pixOldLayerMaskHeight = ( pixOldHeightMapHeight - 1) << tlLayer.tl_slMaskStretch;
			TRL_UpdateLayerSize(ptrTerrain, itrl, pixNewLayerMaskWidth, pixNewLayerMaskHeight,
								pixOldLayerMaskWidth, pixOldLayerMaskHeight);
		}
	}																						// yjpark     -->|

	ASSERT(_CrtCheckMemory());
}

static UnusedDataArray *FindFreeDataArray(SLONG slMemRequired, ULONG ulFlags)
{

	FOREACHINLIST(UnusedDataArray,uda_lnUnusedArrays,_lhUnusedArrays,it) {
		// __asm int 0x03;
		const UnusedDataArray *pudait = it;
		// if array holds enough memory
		if(slMemRequired<=pudait->uda_slMemSize) {
			// found it
			return it;
		}
	}
	return NULL;
}

template <class Type>
static void FreeDataArray(CTRDataArray<Type> &tdaArray)
{
	const INDEX ctAllocated    = tdaArray.tda_ctAllocated;
	const SLONG slMemAllocated = ctAllocated * sizeof(Type);
	
	// if no memory was allocated
	if(slMemAllocated<=0) {
		// nothing to free
		return;
	}


	// at least one array data must be valid
	ASSERT(tdaArray.tda_slBufferID!=(-1) || tdaArray.tda_paObjects!=NULL);

	// Create new unused data entry
	UnusedDataArray *puda = new UnusedDataArray;
	puda->uda_slMemSize   = slMemAllocated;
	puda->uda_ulFlags     = tdaArray.tda_ulFlags;
	puda->uda_paObjects   = tdaArray.tda_paObjects;
	puda->uda_slBufferID  = tdaArray.tda_slBufferID;

	// find node to add new data array after(list is sorted by memory usage)
	// __asm {int 3};
	UnusedDataArray *pudaInsertAfter = NULL;
	FOREACHINLIST(UnusedDataArray,uda_lnUnusedArrays,_lhUnusedArrays,it) {
		const UnusedDataArray *pudait = it;
		if(slMemAllocated>=pudait->uda_slMemSize) {
			pudaInsertAfter = it;
		} else {
			break;
		}
	}

	// if node wasn't found
	if(pudaInsertAfter==NULL) {
		// Add array at end of list
		_lhUnusedArrays.AddHead(puda->uda_lnUnusedArrays);
	// if node was found
	} else {
		// add array after found node
		pudaInsertAfter->uda_lnUnusedArrays.AddAfter(puda->uda_lnUnusedArrays);
	}

	// Reset array data
	tdaArray.tda_ctObjects = 0;
	tdaArray.tda_ctAllocated = 0;
	tdaArray.tda_slBufferID = -1;
	tdaArray.tda_paObjects  = NULL;

	// validate list
#if _DEBUG
	SLONG slLastMemSize = -1;
	FOREACHINLIST(UnusedDataArray,uda_lnUnusedArrays,_lhUnusedArrays,it2) {
		const UnusedDataArray *pudait = it2;
		ASSERT(slLastMemSize<=pudait->uda_slMemSize);
		slLastMemSize = pudait->uda_slMemSize;
	}
#endif
}

template <class Type>
static void RequestDataArray(CTRDataArray<Type> &tdaArray, INDEX ctObjects, ULONG ulFlags)
{
	// Get allocation size
	const INDEX ctAllocated    = tdaArray.tda_ctAllocated;
	const SLONG slMemRequested = ctObjects * sizeof(Type);
	const SLONG slMemAllocated = ctAllocated * sizeof(Type);

	// if data fit in current array
	if(slMemRequested<=slMemAllocated) {
		// keep current array
		tdaArray.tda_ctObjects = ctObjects;
		return;
	}

	// Free current array
	FreeDataArray(tdaArray);

	// try to find data array with same size
	UnusedDataArray *pudaFree = FindFreeDataArray(slMemRequested,ulFlags);
	// if array was not found
	if(pudaFree==NULL) {
		// Create new data array
		tdaArray.Allocate(ctObjects,ulFlags);
		return;
	} else {
		// __asm {int 0x03 }
		ASSERT(tdaArray.tda_paObjects == NULL &&
					 tdaArray.tda_slBufferID == (-1) && 
					 tdaArray.tda_ctObjects == 0 && 
					 tdaArray.tda_ctAllocated == 0);
		// Remove free list from nodes list
		pudaFree->uda_lnUnusedArrays.Remove();
		// Copy data to new array
		tdaArray.tda_ulFlags     = pudaFree->uda_ulFlags;
		tdaArray.tda_ctObjects   = ctObjects;
		tdaArray.tda_ctAllocated = pudaFree->uda_slMemSize / sizeof(Type);
		tdaArray.tda_paObjects   = (Type*)pudaFree->uda_paObjects;
		tdaArray.tda_slBufferID  = pudaFree->uda_slBufferID;
	}
}

static void ClearTileMemory(CTerrainImp *ptr, INDEX iTileIndex, INDEX iTileLOD)
{
}

static INDEX GetTileVertexCount(CTerrainImp *ptr, INDEX iTerrainTile)
{
	ASSERT(ptr->tr_attTiles[iTerrainTile].tt_iRequestedLOD>=0); // must be calculated

	// Count vertices for requested tile lod
	const CTerrainTile &ttTile = ptr->tr_attTiles[iTerrainTile];
	const INDEX iLOD = ttTile.tt_iRequestedLOD;
	const INDEX ctTileVerticesPerRow = (ptr->tr_ctQuadsPerTileX>>iLOD) + 1;
	const INDEX ctTileVertices = ctTileVerticesPerRow * ctTileVerticesPerRow;
	INDEX ctBorderVertices = 0;

	// for each neighbour
	const INDEX cttnb = 4;
	for(INDEX itnb=0;itnb<cttnb;itnb++) {
		const INDEX inb = ttTile.tt_aiNeighbours[itnb];
		// if neighbour is valid and tile isn't in highest lod
		if(inb!=(-1) && iLOD!=0) {
			// Validate neighbour lod index
			ASSERT(ptr->tr_attTiles[inb].tt_iRequestedLOD>=0);
			// Get neighbour
			const CTerrainTile &ttNeighbour = ptr->tr_attTiles[inb];
			const INDEX iNeighbourLOD = ttNeighbour.tt_iRequestedLOD;
			// if tile neighbour is in higher lod
			if(iNeighbourLOD<iLOD) {
				// Count vertices for neighbour lod
				const INDEX ctNeighbourVerticesPerRow = (ptr->tr_ctQuadsPerTileX>>iNeighbourLOD) + 1;
				// Count vertices that will be inserted in tile borders
				ctBorderVertices += ctNeighbourVerticesPerRow - ctTileVerticesPerRow;
			}
		}
	}
	const INDEX ctTotalVertices = ctTileVertices + ctBorderVertices;
	return ctTotalVertices;
}

static INDEX GetTileTriangleCount(CTerrainImp *ptr, INDEX iTerrainTile)
{
#pragma message(">> Do GetTileTriangleCount again!!")
	ASSERT(ptr->tr_attTiles[iTerrainTile].tt_iRequestedLOD>=0); // must be calculated

	// Count triangles and vertices for requested tile lod
	const CTerrainTile &ttTile = ptr->tr_attTiles[iTerrainTile];
	const INDEX iLOD = ttTile.tt_iRequestedLOD;
	const INDEX ctTileQuadsPerRow = (ptr->tr_ctQuadsPerTileX>>iLOD) - 2;
	const INDEX ctTileVerticesPerRow = ctTileQuadsPerRow + 1;
	const INDEX ctTileQuads = ctTileQuadsPerRow * ctTileQuadsPerRow;
	const INDEX ctNormalBorderTriangles = ((ptr->tr_ctQuadsPerTileX>>iLOD)*2) - 2; // border tris count if neighbour is in lower lod (-2 for two tris in corners)
	INDEX ctTileTriangles = ctTileQuads * 2;
	INDEX ctBorderTriangles = 0;

	// for each neighbour
	const INDEX cttnb = 4;
	for(INDEX itnb=0;itnb<cttnb;itnb++) {
		if(itnb!=NB_LEFT && itnb!=NB_RIGHT && itnb!=NB_TOP) {
			// continue;
		}
		const INDEX inb = ttTile.tt_aiNeighbours[itnb];
		// if neighbour is valid and tile isn't in highest lod
		if(inb!=(-1) && iLOD!=0) {
			// Validate neighbour lod index
			ASSERT(ptr->tr_attTiles[inb].tt_iRequestedLOD>=0);
			// Get neighbour
			const CTerrainTile &ttNeighbour = ptr->tr_attTiles[inb];
			const INDEX iNeighbourLOD = ttNeighbour.tt_iRequestedLOD;
			// if tile neighbour is in higher lod
			if(iNeighbourLOD<iLOD) {
				// Count vertices for neighbour lod
				const INDEX ctNeighbourVerticesPerRow = (ptr->tr_ctQuadsPerTileX>>iNeighbourLOD) + 1;
				// Count triangles that will be inserted in tile borders
				// ASSERT(ctNeighbourVerticesPerRow - ctTileVerticesPerRow>=ctNormalBorderTriangles);
				ctBorderTriangles += (ctNeighbourVerticesPerRow-ctTileVerticesPerRow) + ctNormalBorderTriangles-2;//ctNeighbourVerticesPerRow - ctTileVerticesPerRow;
			} else {
				ctBorderTriangles += ctNormalBorderTriangles;
			}
		// normal border
		} else {
			ctBorderTriangles += ctNormalBorderTriangles;
		}
	}
	const INDEX ctTotalTriangles = ctTileTriangles + ctBorderTriangles;
	return ctTotalTriangles;
}

#define GET_HEIGHT(x,z) \
	puwHeightMap[(ivxOffsetX+x)+(ivxOffsetZ+z)*pixHeightMapWidth]

#define VALIDATE_HEIGHT(x,z) \
	ASSERT((ivxOffsetX+x) + (ivxOffsetZ+z)*pixHeightMapWidth<pixHeightMapHeight*pixHeightMapWidth); \
	ASSERT((ivxOffsetX+x) + (ivxOffsetZ+z)*pixHeightMapWidth>=0)

#define LERP_HEIGHT(x1,z1,x2,z2) \
	Lerp(GET_HEIGHT(x1,z1),GET_HEIGHT(x2,z2),0.5f); \
	VALIDATE_HEIGHT(x1,z1); \
	VALIDATE_HEIGHT(x2,z2)

static void GenerateTileBorderVertices(CTerrainImp *ptr, INDEX iTerrainTile)
{
	CTerrainTile &ttTile = ptr->tr_attTiles[iTerrainTile];
	const INDEX ivxOffsetX = ttTile.tt_rcSource.rc_slLeft;
	const INDEX ivxOffsetZ = ttTile.tt_rcSource.rc_slTop;
	const INDEX pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const INDEX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const UWORD *puwHeightMap = &ptr->tr_puwHeightMap[0];
	const INDEX iLOD = ttTile.tt_iRequestedLOD;

	const INDEX ctqX = ptr->tr_ctQuadsPerTileX>>iLOD;
	const INDEX ctqXInv = 1.0f/ctqX;
	const INDEX ctvxX = ctqX+1;
	const INDEX ctvxZ = ctvxX;

	const INDEX ctvtxTile   = ctvxX*ctvxZ; // tile vertex count (without borders)
	INDEX ctvtxBorder = 0;           // counter for inserted border vertices

	const INDEX ittTopNB = ttTile.tt_aiNeighbours[NB_TOP];
	const INDEX ittLeftNB = ttTile.tt_aiNeighbours[NB_LEFT];
	const INDEX ittRightNB = ttTile.tt_aiNeighbours[NB_RIGHT];
	const INDEX ittBottomNB = ttTile.tt_aiNeighbours[NB_BOTTOM];

	CTRDataArray<TerrainVertex> &avxVertices = ttTile.tt_avxVertices;

	// if left neighbour is valid
	if(ittLeftNB>=0) {
		const CTerrainTile &ttNeighbour = ptr->tr_attTiles[ittLeftNB];
		const INDEX iNeighbourLOD = ttNeighbour.tt_iRequestedLOD;
		const INDEX ctqnbX = ptr->tr_ctQuadsPerTileX>>iNeighbourLOD;
		const INDEX ivxStep = (1<<iNeighbourLOD);
		// if some vertices must be inserted per quad
		const INDEX ctvxpq = (ctqnbX/ctqX)-1;
		if(ctvxpq>0) {
			const INDEX ivxX=0;
			INDEX ivxZ=0;
			// for each quad in tile row
			for(INDEX iqX=0;iqX<ctqX;iqX++) {
				ivxZ+=ivxStep;
				// for each vertex that needs to be inserted per quad
				for(INDEX ivxpq=0;ivxpq<ctvxpq;ivxpq++) {
					// Lookup in heigh map
					VALIDATE_HEIGHT(ivxX,ivxZ);
					UWORD uwHeight = GET_HEIGHT(ivxX,ivxZ);
					UWORD uwHeight2;

					if(ivxZ&ivxStep) {
						uwHeight2 = LERP_HEIGHT(ivxX,ivxZ-ivxStep,ivxX,ivxZ+ivxStep);
					} else {
						uwHeight2 = uwHeight;
					}

					// Set vertex
					TerrainVertex &tvx = avxVertices[ctvtxTile+ctvtxBorder];
					tvx = TerrainVertex(ivxX,ivxZ,uwHeight,uwHeight2,VLF_LTLF);
					ctvtxBorder++;
					ivxZ+=ivxStep;
				}
			}
		}
	}

	// if right neighbour is valid
	if(ittRightNB>=0) {
		const CTerrainTile &ttNeighbour = ptr->tr_attTiles[ittRightNB];
		const INDEX iNeighbourLOD = ttNeighbour.tt_iRequestedLOD;
		const INDEX ctqnbX = ptr->tr_ctQuadsPerTileX>>iNeighbourLOD;
		const INDEX ivxStep = (1<<iNeighbourLOD);
		// if some vertices must be inserted per quad
		const INDEX ctvxpq = (ctqnbX/ctqX)-1;
		if(ctvxpq>0) {
			const INDEX ivxX=ptr->tr_ctQuadsPerTileX;
			INDEX ivxZ=0;
			// for each quad in tile row
			for(INDEX iqX=0;iqX<ctqX;iqX++) {
				ivxZ+=ivxStep;
				// for each vertex that needs to be inserted per quad
				for(INDEX ivxpq=0;ivxpq<ctvxpq;ivxpq++) {
					// Lookup in heigh map
					VALIDATE_HEIGHT(ivxX,ivxZ);
					UWORD uwHeight = GET_HEIGHT(ivxX,ivxZ);
					UWORD uwHeight2;
					if(ivxZ&ivxStep) {
						uwHeight2 = LERP_HEIGHT(ivxX,ivxZ-ivxStep,ivxX,ivxZ+ivxStep);
					} else {
						uwHeight2 = uwHeight;
					}

					// Set vertex
					TerrainVertex &tvx = avxVertices[ctvtxTile+ctvtxBorder];
					tvx = TerrainVertex(ivxX,ivxZ,uwHeight, uwHeight2, VLF_RTLF);
					ctvtxBorder++;
					ivxZ+=ivxStep;
				}
			}
		}
	}

	// if top neighbour is valid
	if(ittTopNB>=0) {
		const CTerrainTile &ttNeighbour = ptr->tr_attTiles[ittTopNB];
		const INDEX iNeighbourLOD = ttNeighbour.tt_iRequestedLOD;
		const INDEX ctqnbX = ptr->tr_ctQuadsPerTileX>>iNeighbourLOD;
		const INDEX ivxStep = (1<<iNeighbourLOD);
		// if some vertices must be inserted per quad
		const INDEX ctvxpq = (ctqnbX/ctqX)-1;
		if(ctvxpq>0) {
			INDEX ivxX=0;
			const INDEX ivxZ=0;
			// for each quad in tile row
			for(INDEX iqX=0;iqX<ctqX;iqX++) {
				ivxX+=ivxStep;
				// for each vertex that needs to be inserted per quad
				for(INDEX ivxpq=0;ivxpq<ctvxpq;ivxpq++) {
					// Lookup in heigh map
					VALIDATE_HEIGHT(ivxX,ivxZ);
					UWORD uwHeight = GET_HEIGHT(ivxX,ivxZ);
					UWORD uwHeight2;

					if(ivxX&ivxStep) {
						uwHeight2 = LERP_HEIGHT(ivxX-ivxStep,ivxZ,ivxX+ivxStep,ivxZ);
					} else {
						uwHeight2 = uwHeight;
					}

					// Set vertex
					TerrainVertex &tvx = avxVertices[ctvtxTile+ctvtxBorder];
					tvx = TerrainVertex(ivxX, ivxZ, uwHeight, uwHeight2, VLF_TTLF);
					ctvtxBorder++;
					ivxX+=ivxStep;
				}
			}
		}
	}

	// if bottom neighbour is valid
	if(ittBottomNB>=0) {
		const CTerrainTile &ttNeighbour = ptr->tr_attTiles[ittBottomNB];
		const INDEX iNeighbourLOD = ttNeighbour.tt_iRequestedLOD;
		const INDEX ctqnbX = ptr->tr_ctQuadsPerTileX>>iNeighbourLOD;
		const INDEX ivxStep = (1<<iNeighbourLOD);
		// if some vertices must be inserted per quad
		const INDEX ctvxpq = (ctqnbX/ctqX)-1;
		if(ctvxpq>0) {
			INDEX ivxX=0;
			const INDEX ivxZ=ptr->tr_ctQuadsPerTileX;
			// for each quad in tile row
			for(INDEX iqX=0;iqX<ctqX;iqX++) {
				ivxX+=ivxStep;
				// for each vertex that needs to be inserted per quad
				for(INDEX ivxpq=0;ivxpq<ctvxpq;ivxpq++) {
					// Lookup in heigh map
					VALIDATE_HEIGHT(ivxX,ivxZ);
					UWORD uwHeight = GET_HEIGHT(ivxX,ivxZ);
					UWORD uwHeight2;

					if(ivxX&ivxStep) {
						uwHeight2 = LERP_HEIGHT(ivxX-ivxStep,ivxZ,ivxX+ivxStep,ivxZ);
					} else {
						uwHeight2 = uwHeight;
					}

					// Set vertex
					TerrainVertex &tvx = avxVertices[ctvtxTile+ctvtxBorder];
					tvx = TerrainVertex(ivxX, ivxZ, uwHeight, uwHeight2, VLF_BTLF);
					ctvtxBorder++;
					ivxX+=ivxStep;
				}
			}
		}
	}
}

static void FixBorderCracks(CTerrainImp *ptr, INDEX iTerrainTile)
{
	CTerrainTile &ttTile = ptr->tr_attTiles[iTerrainTile];
	const INDEX iLOD = ttTile.tt_iRequestedLOD;
	const INDEX ctqX = ptr->tr_ctQuadsPerTileX>>iLOD;
	const INDEX ctvxX = ctqX+1;
	const INDEX ctvxZ = ctvxX;
	const INDEX ctvtxTile = ctvxX*ctvxZ; // tile vertex count (without borders)

	UBYTE aubLerp[4] = {
		VLF_CTLF,
		VLF_CTLF,
		VLF_CTLF,
		VLF_CTLF
	};

	// for each neighbour
	for(INDEX inb=0;inb<4;inb++) {
		// if neighbour is valid
		const INDEX ittNB = ttTile.tt_aiNeighbours[inb];
		if(ittNB>=0) {
			const CTerrainTile &tt = ptr->tr_attTiles[ittNB];
			const int iNeighbourLOD = tt.tt_iRequestedLOD;
			// if neighbour is in higher LOD
			if(iNeighbourLOD<iLOD) {
				// neighbour vertex isn't border
				aubLerp[inb] = VLF_NOLERP;
			// if in same LOD
			} else if(iNeighbourLOD==iLOD) {
				// use min of current and neightbour lerp factor 
				aubLerp[inb] = VLF_LCTLF+inb;
			}
		}
	}

	// if top border needs to be updated
	if(aubLerp[NB_TOP]!=VLF_CTLF) {
		for(INDEX ivxX=1;ivxX<ctvxX;ivxX+=2) {
			const INDEX ivx = ivxX;
			TerrainVertex &tvx = ttTile.tt_avxVertices[ivx];
			tvx.tv_ubLerp = aubLerp[NB_TOP];
		}
	}
	// if bottom border needs to be updated
	if(aubLerp[NB_BOTTOM]!=VLF_CTLF) {
		for(INDEX ivxX=1;ivxX<ctvxX;ivxX+=2) {
			const INDEX ivx = ivxX + ctvtxTile-ctvxX;
			TerrainVertex &tvx = ttTile.tt_avxVertices[ivx];
			tvx.tv_ubLerp = aubLerp[NB_BOTTOM];
		}
	}
	// it left border needs to be updated
	if(aubLerp[NB_LEFT]!=VLF_CTLF) {
		for(INDEX ivxZ=1;ivxZ<ctvxZ;ivxZ+=2) {
			const INDEX ivx = ivxZ * ctvxX;
			TerrainVertex &tvx = ttTile.tt_avxVertices[ivx];
			tvx.tv_ubLerp = aubLerp[NB_LEFT];
		}
	}
	// it right border needs to be updated
	if(aubLerp[NB_RIGHT]!=VLF_CTLF) {
		for(INDEX ivxZ=1;ivxZ<ctvxZ;ivxZ+=2) {
			const INDEX ivx = ivxZ * ctvxX + ctvxX-1;
			TerrainVertex &tvx = ttTile.tt_avxVertices[ivx];
			tvx.tv_ubLerp = aubLerp[NB_RIGHT];
		}
	}
}

static void GenerateTileBorderTriangles(CTerrainImp *ptr, INDEX iTerrainTile)
{
	CTerrainTile &ttTile = ptr->tr_attTiles[iTerrainTile];
	const INDEX iLOD = ttTile.tt_iRequestedLOD;

	const INDEX ctqX = (ptr->tr_ctQuadsPerTileX>>iLOD);
	const INDEX ctqZ = ctqX;
	const INDEX ctvxX = (ptr->tr_ctQuadsPerTileX>>iLOD)+1;
	const INDEX ctvxZ = ctvxX;
	
	const INDEX ctqXMid = (ptr->tr_ctQuadsPerTileX>>iLOD)-2; // count of quads in middle of tile per row
	const INDEX cttriTileMid = (ctqXMid*ctqXMid)*2;          // tile triangle count (without borders)
	const INDEX ctvtxTileMid = ctvxX*ctvxZ;                  // tile vertex count (without borders)
	INDEX cttriBorder = 0; // counter for inserted border triangles
	INDEX ctvtxBorder = 0; // counter for inserted border vertices

	INDEX actTris[4];
	INDEX ctTris;
	INDEX ivxFan;
	INDEX iqX;
	INDEX iqZ;

	// for each neighbour
	for(INDEX inb=0;inb<4;inb++) {
		// default border LOD is tile LOD
		INDEX iBorderLOD = iLOD;
		// if neighbour is valid
		const INDEX inbi = ttTile.tt_aiNeighbours[inb];
		if(inbi!=(-1)) {
			const CTerrainTile &ttNeighbour = ptr->tr_attTiles[inbi];
			const INDEX iNeighbourLOD = ttNeighbour.tt_iRequestedLOD;
			// and it is in higher LOD
			if(iNeighbourLOD<iLOD) {
				// border LOD is neighbour LOD
				iBorderLOD = iNeighbourLOD;
			}
		}
		const INDEX iLODDiff = iLOD-iBorderLOD;
		actTris[inb] = (1UL<<iLODDiff)*2;
	}


	CTRDataArray<TerrainTriangle> &atriTriangles = ttTile.tt_atriTriangles;

	/*
	 * Generate triangles for left border
	 */
	ctTris = actTris[NB_LEFT];
	ivxFan = ctvxX + 1;
	// for each quad pair
	for(iqZ=0;iqZ<ctqZ;iqZ+=2) {

		const INDEX ivxQ1First = (iqZ+0)*ctvxX; // first vertex in first quad
		const INDEX ivxQ1Last  = (iqZ+1)*ctvxX; // last vertex in first quad
		const INDEX ivxQ2Last  = (iqZ+2)*ctvxX; // last vertex in second quad
		INDEX ivxPrev = ivxQ1First;
		INDEX ivxNext = -1;

		// for each triangle that needs to be inserted in border
		for(INDEX itri=0;itri<ctTris;itri++) {
			// if this is last triangle of first quad
			if(itri==((ctTris-1)>>1)) {
				// next index is last vertex in first quad
				ivxNext = ivxQ1Last;
			// if this is last triangle of second quad
			} else if(itri==ctTris-1) {
				// next index is last vertex in second quad
				ivxNext = ivxQ2Last;
			// if normal triangle
			} else {
				// next is index of next border vertex
				ivxNext = ctvtxTileMid+ctvtxBorder;
				ctvtxBorder++;
			}

			// Insert triangle
			TerrainTriangle &tri = atriTriangles[cttriTileMid+cttriBorder];
			tri = TerrainTriangle(ivxPrev,ivxNext,ivxFan);
			cttriBorder++;

			// Next triangle will have next as prev
			ivxPrev = ivxNext;
		}

		// if this isn't last quad
		if(iqZ+2<ctqZ) {
			// Close quad and begin new one
			TerrainTriangle &tri3 = atriTriangles[cttriTileMid+cttriBorder+0];
			TerrainTriangle &tri4 = atriTriangles[cttriTileMid+cttriBorder+1];
			tri3 = TerrainTriangle(ivxFan+0,ivxQ2Last,ivxFan+ctvxX);
			tri4 = TerrainTriangle(ivxFan+ctvxX,ivxQ2Last,ivxFan+2*ctvxX);
			cttriBorder+=2;
		}
		ivxFan+=2*ctvxX;
	}

	/*
	 * Generate triangles for right border
	 */
	ctTris = actTris[NB_RIGHT];
	ivxFan = ctvxX+ctvxX-2;
	// for each quad pair
	for(iqZ=0;iqZ<ctqZ;iqZ+=2) {

		const INDEX ivxQ1First = (iqZ+0)*ctvxX+ctvxX-1; // first vertex in first quad
		const INDEX ivxQ1Last  = (iqZ+1)*ctvxX+ctvxX-1; // last vertex in first quad
		const INDEX ivxQ2Last  = (iqZ+2)*ctvxX+ctvxX-1; // last vertex in second quad
		INDEX ivxPrev = ivxQ1First;
		INDEX ivxNext = -1;

		// for each triangle that needs to be inserted in border
		for(INDEX itri=0;itri<ctTris;itri++) {
			// if this is last triangle of first quad
			if(itri==((ctTris-1)>>1)) {
				// next index is last vertex in first quad
				ivxNext = ivxQ1Last;
			// if this is last triangle of second quad
			} else if(itri==ctTris-1) {
				// next index is last vertex in second quad
				ivxNext = ivxQ2Last;
			// if normal triangle
			} else {
				// next is index of next border vertex
				ivxNext = ctvtxTileMid+ctvtxBorder;
				ctvtxBorder++;
			}

			// Insert triangle
			TerrainTriangle &tri = atriTriangles[cttriTileMid+cttriBorder];
			tri = TerrainTriangle(ivxNext,ivxPrev,ivxFan);
			cttriBorder++;

			// Next triangle will have next as prev
			ivxPrev = ivxNext;
		}

		// if this isn't last quad
		if(iqZ+2<ctqZ) {
			// Close quad and begin new one
			TerrainTriangle &tri3 = atriTriangles[cttriTileMid+cttriBorder+0];
			TerrainTriangle &tri4 = atriTriangles[cttriTileMid+cttriBorder+1];
			tri3 = TerrainTriangle(ivxQ2Last,ivxFan+0,ivxFan+ctvxX);
			tri4 = TerrainTriangle(ivxQ2Last,ivxFan+ctvxX,ivxFan+2*ctvxX);
			cttriBorder+=2;
		}
		ivxFan+=2*ctvxX;
	}

	/*
	 * Generate triangles for top border
	 */
	ctTris = actTris[NB_TOP];
	ivxFan = ctvxX + 1;
	// for each quad pair
	for(iqX=0;iqX<ctqX;iqX+=2) {

		const INDEX ivxQ1First = iqX+0; // first vertex in first quad
		const INDEX ivxQ1Last  = iqX+1; // last vertex in first quad
		const INDEX ivxQ2Last  = iqX+2; // last vertex in second quad
		INDEX ivxPrev = ivxQ1First;
		INDEX ivxNext = -1;

		// for each triangle that needs to be inserted in border
		for(INDEX itri=0;itri<ctTris;itri++) {
			// if this is last triangle of first quad
			if(itri==((ctTris-1)>>1)) {
				// next index is last vertex in first quad
				ivxNext = ivxQ1Last;
			// if this is last triangle of second quad
			} else if(itri==ctTris-1) {
				// next index is last vertex in second quad
				ivxNext = ivxQ2Last;
			// if normal triangle
			} else {
				// next is index of next border vertex
				ivxNext = ctvtxTileMid+ctvtxBorder;
				ctvtxBorder++;
			}

			// Insert triangle
			TerrainTriangle &tri = atriTriangles[cttriTileMid+cttriBorder];
			tri = TerrainTriangle(ivxNext,ivxPrev,ivxFan);
			cttriBorder++;

			// Next triangle will have next as prev
			ivxPrev = ivxNext;
		}

		// if this isn't last quad
		if(iqX+2<ctqX) {
			// Close quad and begin new one
			TerrainTriangle &tri3 = atriTriangles[cttriTileMid+cttriBorder+0];
			TerrainTriangle &tri4 = atriTriangles[cttriTileMid+cttriBorder+1];
			tri3 = TerrainTriangle(ivxQ2Last,ivxFan+0,ivxFan+1);
			tri4 = TerrainTriangle(ivxQ2Last,ivxFan+1,ivxFan+2);
			cttriBorder+=2;
		}
		ivxFan+=2;
	}

	/*
	 * Generate triangles for bottom border
	 */
	ctTris = actTris[NB_BOTTOM];
	ivxFan = ctvxX*(ctvxZ-2) + 1;
	// for each quad pair
	for(iqX=0;iqX<ctqX;iqX+=2) {

		const INDEX ivxQ1First = ctvxX*(ctvxZ-1)+iqX+0; // first vertex in first quad
		const INDEX ivxQ1Last  = ctvxX*(ctvxZ-1)+iqX+1; // last vertex in first quad
		const INDEX ivxQ2Last  = ctvxX*(ctvxZ-1)+iqX+2; // last vertex in second quad
		INDEX ivxPrev = ivxQ1First;
		INDEX ivxNext = -1;

		// for each triangle that needs to be inserted in border
		for(INDEX itri=0;itri<ctTris;itri++) {
			// if this is last triangle of first quad
			if(itri==((ctTris-1)>>1)) {
				// next index is last vertex in first quad
				ivxNext = ivxQ1Last;
			// if this is last triangle of second quad
			} else if(itri==ctTris-1) {
				// next index is last vertex in second quad
				ivxNext = ivxQ2Last;
			// if normal triangle
			} else {
				// next is index of next border vertex
				ivxNext = ctvtxTileMid+ctvtxBorder;
				ctvtxBorder++;
			}

			// Insert triangle
			TerrainTriangle &tri = atriTriangles[cttriTileMid+cttriBorder];
			tri = TerrainTriangle(ivxPrev,ivxNext,ivxFan);
			cttriBorder++;

			// Next triangle will have next as prev
			ivxPrev = ivxNext;
		}

		// if this isn't last quad
		if(iqX+2<ctqX) {
			// Close quad and begin new one
			TerrainTriangle &tri3 = atriTriangles[cttriTileMid+cttriBorder+0];
			TerrainTriangle &tri4 = atriTriangles[cttriTileMid+cttriBorder+1];
			tri3 = TerrainTriangle(ivxFan+0,ivxQ2Last,ivxFan+1);
			tri4 = TerrainTriangle(ivxFan+1,ivxQ2Last,ivxFan+2);
			cttriBorder+=2;
		}
		ivxFan+=2;
	}
}

static void GenerateTileGeometry(CTerrainImp *ptr, INDEX iTerrainTile)
{
	TR_SETINFO(ti_ctGeometryBuilds++);

	CTerrainTile &ttTile = ptr->tr_attTiles[iTerrainTile];

	// Count vertices and triangles for tile geometry
	INDEX cttvx = GetTileVertexCount(ptr, iTerrainTile);
	INDEX cttris = GetTileTriangleCount(ptr, iTerrainTile);
	
	// Prepare tile geometry memory
	RequestDataArray(ttTile.tt_avxVertices,cttvx,0);
	RequestDataArray(ttTile.tt_atriTriangles,cttris,0);

	const INDEX iLOD = ttTile.tt_iRequestedLOD;
	const INDEX ctvxX = ptr->tr_ctQuadsPerTileX+1;
	const INDEX ctvxZ = ctvxX;
	const INDEX ctvxLODX = (ptr->tr_ctQuadsPerTileX>>iLOD)+1;
	const INDEX ctvxLODZ = ctvxLODX;
	const INDEX ivxStepX = 1<<iLOD;
	const INDEX ivxStepZ = ivxStepX;
	const INDEX ivxOffsetX = ttTile.tt_rcSource.rc_slLeft;
	const INDEX ivxOffsetZ = ttTile.tt_rcSource.rc_slTop;
	const INDEX pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const INDEX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const UWORD *puwHeightMap = &ptr->tr_puwHeightMap[0];
	
	/*
	 * Generate tile vertices
	 */
	// lock vertices for write
	CTRDataArray<TerrainVertex> &avxVertices = ttTile.tt_avxVertices;
	avxVertices.Lock(TDAF_LOCK_READ|TDAF_LOCK_WRITE);

	// for each vertex
	INDEX ivx = 0;
	INDEX ivxZ;
	for(ivxZ=0;ivxZ<ctvxZ;ivxZ+=ivxStepZ) {
		for(INDEX ivxX=0;ivxX<ctvxX;ivxX+=ivxStepX) {
			VALIDATE_HEIGHT(ivxX,ivxZ);
			UWORD uwHeight = GET_HEIGHT(ivxX,ivxZ);
			// Set vertex
			TerrainVertex &tvx = avxVertices[ivx];
			tvx = TerrainVertex(ivxX, ivxZ, uwHeight, uwHeight, VLF_CTLF);
			ivx++;
		}
	}
	// Generate tile border vertices
	GenerateTileBorderVertices(ptr,iTerrainTile);


	/*
	 * Generate lerp targets
	 */
	INDEX iFacing = 1;
	ivx = 0;
	for(ivxZ=0;ivxZ<ctvxZ;ivxZ+=ivxStepZ) {
		for(INDEX ivxX=0;ivxX<ctvxX-2;ivxX+=ivxStepX*2) {
			// Lerp horizontal vertex
			TerrainVertex &tvxH = avxVertices[ivx+1];
			tvxH.tv_uwY2 = LERP_HEIGHT(ivxX,ivxZ,ivxX+ivxStepX*2,ivxZ);
			ivx+=2;
		}

		ivx++;
		ivxZ+=ivxStepZ;
		if(ivxZ<ctvxZ) {
			INDEX ivxX;
			for(ivxX = 0; ivxX < ctvxX-2; ivxX += ivxStepX * 2) {
				// Lerp left vertical vertex
				TerrainVertex &tvxV1 = avxVertices[ivx+0];
				tvxV1.tv_uwY2 = LERP_HEIGHT(ivxX,ivxZ-ivxStepZ,ivxX,ivxZ+ivxStepZ);
				if(iFacing&1) {
					// Lerp diagonal vertex
					TerrainVertex &tvxD = avxVertices[ivx+1];
					tvxD.tv_uwY2 = LERP_HEIGHT(ivxX,ivxZ-ivxStepZ,ivxX+ivxStepX*2,ivxZ+ivxStepZ);
				} else {
					// Lerp diagonal vertex
					TerrainVertex &tvxD = avxVertices[ivx+1];
					tvxD.tv_uwY2 = LERP_HEIGHT(ivxX,ivxZ+ivxStepZ,ivxX+ivxStepX*2,ivxZ-ivxStepZ);
				}
				iFacing++;
				ivx+=2;
			}
			// Lerp right vertical vertex
			TerrainVertex &tvxV2 = avxVertices[ivx+0];
			tvxV2.tv_uwY2 = LERP_HEIGHT(ivxX,ivxZ-ivxStepZ,ivxX,ivxZ+ivxStepZ);
			iFacing++;
		}
		ivx++;
	}

	// Fix cracks that may appear on borders
	FixBorderCracks(ptr,iTerrainTile);
	
	// Unlock vertex array
	avxVertices.Unlock();

	/*
	 * Generate tile triangles
	 */
	CTRDataArray<TerrainTriangle> &avxTriangles = ttTile.tt_atriTriangles;
	avxTriangles.Lock(TDAF_LOCK_READ|TDAF_LOCK_WRITE);

	const INDEX ctqX = ptr->tr_ctQuadsPerTileX>>iLOD;
	const INDEX ctqZ = ctqX;

	INDEX iTriangle = 0;
	for(INDEX iqZ=1;iqZ<ctqZ-1;iqZ++) {
		for(INDEX iqX=1;iqX<ctqX-1;iqX++) {
			const INDEX ivx = iqX + iqZ*(ctqX+1);
			TerrainTriangle &tri1  = avxTriangles[iTriangle];
			TerrainTriangle &tri2  = avxTriangles[iTriangle+1];
			if(ivx&1) {
				tri1 = TerrainTriangle(ivx,       ivx+ctqX+1,ivx+1);
				tri2 = TerrainTriangle(ivx+1,     ivx+ctqX+1,ivx+ctqX+2);
			} else {
				tri1 = TerrainTriangle(ivx+ctqX+1,ivx+ctqX+2,ivx);
				tri2 = TerrainTriangle(ivx,       ivx+ctqX+2,ivx+1);
			}
			iTriangle+=2;
		}
	}

	// Generate tile border triangles
	GenerateTileBorderTriangles(ptr,iTerrainTile);

	// Unlock triangle array
	avxTriangles.Unlock();

	// Update tile LOD
	ttTile.tt_iCurrentLOD = iLOD;

	// Update neighbour LODs at time of tile generation
	for(INDEX inb=0;inb<4;inb++) {
		const INDEX inbi = ttTile.tt_aiNeighbours[inb];
		if(inbi>=0) {
			const CTerrainTile &ttNeighbour = ptr->tr_attTiles[inbi];
			const INDEX iNeighbourLOD = ttNeighbour.tt_iRequestedLOD;
			ttTile.tt_aiNBLODs[inb] = iNeighbourLOD;
		} else {
			ttTile.tt_aiNBLODs[inb] = -1;
		}
	}
}

static void ClearTileGeometry(CTerrainImp *ptr, INDEX iTerrainTile)
{
}

extern void GenerateTileTopMap(CTerrainImp *ptr, INDEX iTerrainTile)
{
}

static void ClearTileTopMap(CTerrainImp *ptr, INDEX iTerrainTile)
{
}

static FLOATaabbox3D GenerateTileQuadTreeNodeBox(CTerrainImp *ptr, INDEX iTerrainTile)
{
	// Extract tile vertices
	const CTerrainTile &ttTile = ptr->tr_attTiles[iTerrainTile];
	FLOATaabbox3D bbox;

	// Prepare extraction arrays
	CStaticArray<FLOAT3D> avxVertices;
	const INDEX ctVertices = (ptr->tr_ctQuadsPerTileX+1) * (ptr->tr_ctQuadsPerTileX+1);
	const INDEX ctTriangles = (ptr->tr_ctQuadsPerTileX * ptr->tr_ctQuadsPerTileX) * 2;
	avxVertices.New(ctVertices);

	TR_ExtractVertices(ptr, ttTile.tt_rcSource, avxVertices);

	// Add each vertex to tile quad tree node box
	const INDEX cttvx = avxVertices.Count();
	const FLOAT3D &vxFirst = avxVertices[0];
	bbox = vxFirst;
	for(INDEX itvx=1;itvx<cttvx;itvx++) {
		const FLOAT3D &vx = avxVertices[itvx];
		bbox |= vx;
	}

	return bbox;
}

static void GenerateTerrainGeometry(CTerrainImp *ptr)
{
}

static void ClearTerrainGeometry(CTerrainImp *ptr)
{
}

static void GenerateTerrainTopMap(CTerrainImp *ptr)
{
}

static void ClearTerrainTopMap(CTerrainImp *ptr)
{
}

static void GenerateTerrainQuadTreeLevels(CTerrainImp *ptr)
{
	INDEX ctNodesX = ptr->tr_ctTilesX;
	INDEX ctNodesY = ptr->tr_ctTilesY;
	INDEX ctNodesAll  = 0;

	CStaticStackArray<QuadTreeLevel> aqtlLevels;
	
	while(TRUE) {
		// Add new quad tree level
		QuadTreeLevel &qtl = aqtlLevels.Push();
		qtl.qtl_ctNodesX   = ctNodesX;
		qtl.qtl_ctNodesY   = ctNodesY;
		qtl.qtl_ctNodes    = ctNodesX * ctNodesY;
		qtl.qtl_iFirstNode = ctNodesAll;

		// Update total nodes count
		ctNodesAll += ctNodesX * ctNodesY;

		// if there is only one node in this level
		if(qtl.qtl_ctNodes==1) {
			// this is last quad tree level
			break;
		}

		// Update nodes count for next level
		ctNodesX = ClampDn(ctNodesX/2L,1L);
		ctNodesY = ClampDn(ctNodesY/2L,1L);
	}

	// Allocate array of qtl in terrain
	const INDEX ctqtl = aqtlLevels.Count();
	ptr->tr_aqtlQuadTreeLevels.New(ctqtl);

	// Copy quad tree level array in terrain
	for(INDEX iqtl=0;iqtl<ctqtl;iqtl++) {
		ptr->tr_aqtlQuadTreeLevels[iqtl] = aqtlLevels[iqtl];
	}
}

static void UpdateTileQTNBox(CTerrainImp *ptr, INDEX iTerrainTile)
{
	// Generate tile quad tree node
	QuadTreeNode &qtn = ptr->tr_aqtnQuadTreeNodes[iTerrainTile];
	qtn.qtn_aabbox = GenerateTileQuadTreeNodeBox(ptr,iTerrainTile);
}

static void UpdateLeafTerrainQuadTreeNodes(CTerrainImp *ptr)
{
	// Update first level of quad tree nodes
	QuadTreeLevel &qtl = ptr->tr_aqtlQuadTreeLevels[0];
	const INDEX ctqtn = qtl.qtl_ctNodes;
	const INDEX iqtnFirst = qtl.qtl_iFirstNode;

	// for each node
	for(INDEX iqtn=0;iqtn<ctqtn;iqtn++) {
		// Update node box
		QuadTreeNode &qtn = ptr->tr_aqtnQuadTreeNodes[iqtn];
		UpdateTileQTNBox(ptr, iqtn);
	}
}

static void UpdateHighterTerrainQuadTreeNodes(CTerrainImp *ptr)
{
	const INDEX ctqtl = ptr->tr_aqtlQuadTreeLevels.Count();
	for(INDEX iqtl=1;iqtl<ctqtl;iqtl++) {
		const QuadTreeLevel &qtl = ptr->tr_aqtlQuadTreeLevels[iqtl];
		const INDEX ctqtn = qtl.qtl_ctNodes;
		const INDEX iqtnFirst = qtl.qtl_iFirstNode;
		for(INDEX iqtn=0;iqtn<ctqtn;iqtn++) {
			const INDEX iqtnIndex = iqtnFirst + iqtn;
			QuadTreeNode &qtn = ptr->tr_aqtnQuadTreeNodes[iqtnIndex];
			for(INDEX iqtnc=0;iqtnc<4;iqtnc++) {
				const INDEX iqtncIndex = qtn.qtn_aiChild[iqtnc];
				if(iqtncIndex!=(-1)) {
					const QuadTreeNode &qtnChild = ptr->tr_aqtnQuadTreeNodes[iqtncIndex];
					if(qtn.qtn_aabbox.IsEmpty()) {
						qtn.qtn_aabbox  = qtnChild.qtn_aabbox;
					} else {
						qtn.qtn_aabbox |= qtnChild.qtn_aabbox;
					}
				}
			}
		}
	}
}

static void UpdateAllTerrainQuadTreeNodes(CTerrainImp *ptr)
{
	UpdateLeafTerrainQuadTreeNodes(ptr);
	UpdateHighterTerrainQuadTreeNodes(ptr);
}

static void GenerateTerrainQuadTreeNodes(CTerrainImp *ptr)
{
	CStaticStackArray<QuadTreeNode> aqtnNodes;
	// Generate first level of quad tree nodes
	QuadTreeLevel &qtl = ptr->tr_aqtlQuadTreeLevels[0];
	const INDEX ctqtn = qtl.qtl_ctNodes;
	const INDEX iqtnFirst = qtl.qtl_iFirstNode;
	INDEX iqtn;

	// for each node
	for( iqtn = 0; iqtn < ctqtn; iqtn++ ) {
		QuadTreeNode &qtn = aqtnNodes.Push();
		// Fill node data
		qtn.qtn_iTileIndex = iqtn;
		qtn.qtn_aiChild[QTCN_TOP_LEFT] = -1;
		qtn.qtn_aiChild[QTCN_TOP_RIGHT] = -1;
		qtn.qtn_aiChild[QTCN_BOTTOM_LEFT] = -1;
		qtn.qtn_aiChild[QTCN_BOTTOM_RIGHT] = -1;
	}

	// Generate nodes for rest of quad tree levels
	const INDEX ctqtl = ptr->tr_aqtlQuadTreeLevels.Count();
	for(INDEX iqtl=1;iqtl<ctqtl;iqtl++) {
		QuadTreeLevel &qtlCurrent = ptr->tr_aqtlQuadTreeLevels[iqtl];
		const QuadTreeLevel &qtlPrev = ptr->tr_aqtlQuadTreeLevels[iqtl-1];
		const INDEX ctNodesX = qtlPrev.qtl_ctNodesX;
		const INDEX ctNodesY = qtlPrev.qtl_ctNodesY;
		for(INDEX iqtnY=0;iqtnY<ctNodesY;iqtnY+=2) {
			for(INDEX iqtnX=0;iqtnX<ctNodesX;iqtnX+=2) {
				const INDEX iqtn = qtlPrev.qtl_iFirstNode + (iqtnY*ctNodesX + iqtnX);
				// Add new quad tree node
				QuadTreeNode &qtn = aqtnNodes.Push();
				qtn.qtn_iTileIndex = -1;
				qtn.qtn_aiChild[QTCN_TOP_LEFT] = -1;
				qtn.qtn_aiChild[QTCN_TOP_RIGHT] = -1;
				qtn.qtn_aiChild[QTCN_BOTTOM_LEFT] = -1;
				qtn.qtn_aiChild[QTCN_BOTTOM_RIGHT] = -1;
				
				// Set top left child (always exists)
				qtn.qtn_aiChild[QTCN_TOP_LEFT] = iqtn;
				
				// if top right child exists
				if(iqtnX+1<ctNodesX) {
					// Update node child and box
					const INDEX iqtnc = iqtn+1;
					const QuadTreeNode &qtnChild = aqtnNodes[iqtnc];
					qtn.qtn_aiChild[QTCN_TOP_RIGHT] = iqtnc;
				}
				// if bottom left child exists
				if(iqtnY+1<ctNodesY) {
					// Update node child and box
					const INDEX iqtnc = iqtn+ctNodesX;
					const QuadTreeNode &qtnChild = aqtnNodes[iqtnc];
					qtn.qtn_aiChild[QTCN_BOTTOM_LEFT] = iqtnc;
				}
				// if bottom right child exists
				if(iqtnY+1<ctNodesY && iqtnX+1<ctNodesX) {
					// Update node child and box
					const INDEX iqtnc = iqtn+1+ctNodesX;
					const QuadTreeNode &qtnChild = aqtnNodes[iqtnc];
					qtn.qtn_aiChild[QTCN_BOTTOM_RIGHT] = iqtnc;
				}
			}
		}
	}

	// Allocate array of qtn in terrain
	const INDEX ctqtnCopy = aqtnNodes.Count();
	ptr->tr_aqtnQuadTreeNodes.New(ctqtnCopy);

	// Copy qtn array in terrain
	for( iqtn = 0; iqtn < ctqtnCopy; iqtn++ ) {
		ptr->tr_aqtnQuadTreeNodes[iqtn] = aqtnNodes[iqtn];
	}
}

static void GenerateTerrainQuadTree(CTerrainImp *ptr)
{
	ASSERT(ptr->tr_aqtlQuadTreeLevels.Count()==0); // must be cleared
	ASSERT(ptr->tr_aqtnQuadTreeNodes.Count()==0);  // must be cleared

	GenerateTerrainQuadTreeLevels(ptr);
	GenerateTerrainQuadTreeNodes(ptr);
	UpdateAllTerrainQuadTreeNodes(ptr);
}

static void ClearTerrainQuadTree(CTerrainImp *ptr)
{
	ptr->tr_aqtlQuadTreeLevels.Clear();
	ptr->tr_aqtnQuadTreeNodes.Clear();
}

static void GenerateTerrainTileData(CTerrainImp *ptr)
{
	ASSERT(ptr->tr_ctTilesX==0);         // must be cleared
	ASSERT(ptr->tr_ctTilesY==0);         // must be cleared
	ASSERT(ptr->tr_attTiles.Count()==0); // must be cleared

	const INDEX pixTileWidth  = ptr->tr_ctQuadsPerTileX;
	const INDEX pixTileHeight = ptr->tr_ctQuadsPerTileX;

	// Calculate terrain tiles count
	const INDEX ctTilesX = (ptr->tr_pixHeightMapWidth-1)  / ptr->tr_ctQuadsPerTileX;
	const INDEX ctTilesY = (ptr->tr_pixHeightMapHeight-1) / ptr->tr_ctQuadsPerTileX;
	const INDEX ctTiles  = ctTilesX * ctTilesY;

	// Allocate terrain tiles
	ptr->tr_ctTilesX = ctTilesX;
	ptr->tr_ctTilesY = ctTilesY;
	ptr->tr_attTiles.New(ctTiles);

	// for each terrain tile
	for(INDEX ittZ=0;ittZ<ctTilesY;ittZ++) {
		for(INDEX ittX=0;ittX<ctTilesX;ittX++) {
			// Get terrain tile index
			INDEX iTileIndex = ittX + ittZ*ctTilesX;
			// Get terrain tile
			CTerrainTile &ttTile = ptr->tr_attTiles[iTileIndex];
			// Remember tile neighbours if they exists
			if(ittX>0)          ttTile.tt_aiNeighbours[NB_LEFT]   = iTileIndex - 1;
			if(ittX<ctTilesX-1) ttTile.tt_aiNeighbours[NB_RIGHT]  = iTileIndex + 1;
			if(ittZ>0)          ttTile.tt_aiNeighbours[NB_TOP]    = iTileIndex - ctTilesX;
			if(ittZ<ctTilesY-1) ttTile.tt_aiNeighbours[NB_BOTTOM] = iTileIndex + ctTilesX;

			// Set source rectangle of tile on height map
			const CTPoint ptMin = CTPoint(ittX*pixTileWidth, ittZ*pixTileHeight);
			const CTPoint ptMax = CTPoint((ittX+1)*pixTileWidth+1, (ittZ+1)*pixTileHeight+1);
			ttTile.tt_rcSource  = CTRect(ptMin,ptMax);

			// Set tile to terrain translation matrix
			MakeIdentityMatrix(ttTile.tt_mTileToTerrain);
			ttTile.tt_mTileToTerrain[ 3] = ittX * pixTileWidth;
			ttTile.tt_mTileToTerrain[11] = ittZ * pixTileHeight;

			// Calculate edge and shadow map transform matrix
			const FLOAT fStretchX = 1.0f/(ptr->tr_pixHeightMapWidth-1);
			const FLOAT fStretchY = 1.0f/(ptr->tr_pixHeightMapHeight-1);
			Matrix12 mTransf;
			MakeIdentityMatrix(mTransf);
			mTransf[ 0] = fStretchX;
			mTransf[ 5] = fStretchY;
			mTransf[ 3] = ittX/(FLOAT)ctTilesX;
			mTransf[ 7] = ittZ/(FLOAT)ctTilesY;
			// Set top map transfrom matrix
			MatrixCopy(ttTile.tt_mTopMap,mTransf);
			// Set edge map transform matrix
			MatrixCopy(ttTile.tt_mEdgeMap,mTransf);
			// Set shadow map transform matrix
			MatrixCopy(ttTile.tt_mShadowMap,mTransf);
		}
	}
}

extern void TR_GenerateTerrainTileOptimizationData(CTerrain *ptrTerrain)
{
	ASSERT( ptrTerrain != NULL );                                 // must be valid
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );                  // must be valid
	ASSERT( ptrTerrain->tr_ptrTerrain->tr_ctTilesX > 0 );         // must not be cleared
	ASSERT( ptrTerrain->tr_ptrTerrain->tr_ctTilesY > 0 );         // must not be cleared
	ASSERT( ptrTerrain->tr_ptrTerrain->tr_attTiles.Count() > 0 ); // must not be cleared
	ASSERT( ptrTerrain->tr_ptrTerrain->tr_pubEdgeMap != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain->tr_puwHeightMap != NULL );

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const PIX	pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX	pixEdgeMapWidth = pixHeightMapWidth-1;
	const PIX	pixEdgeMapHeight = pixHeightMapHeight-1;

	const INDEX	ctTiles = ptr->tr_attTiles.Count();
	for( INDEX itt = 0; itt < ctTiles; itt++ )
	{
		CTerrainTile	&ttTile = ptr->tr_attTiles[itt];

		// Reset tile optimization flags
		ttTile.tt_ulFlags &= ~TTF_BLANK_EDGE_MAP;
		ttTile.tt_ulFlags &= ~TTF_FULL_EDGE_MAP;
		ttTile.tt_ulVisLayers = 0xFFFFFFFF;

		const PIX	pixLeft = ttTile.tt_rcSource.rc_slLeft;
		const PIX	pixTop = ttTile.tt_rcSource.rc_slTop;
		const PIX	pixWidth = ttTile.tt_rcSource.GetWidth() - 1;
		const PIX	pixHeight = ttTile.tt_rcSource.GetHeight() - 1;
		const INDEX	ctPixels = pixWidth * pixHeight;

		// Test edge map 
		const UBYTE	*pubEdgeSrc = ptr->tr_pubEdgeMap;

		INDEX ctEdgePixels = 0;
		for( PIX pixY = 0; pixY < pixHeight; pixY++ )
		{
			for( PIX pixX = 0; pixX < pixWidth;pixX++ )
			{
				const PIX	pixRealX = pixLeft+pixX;
				const PIX	pixRealY = pixTop+pixY;
				const PIX	pix = pixRealX + pixRealY * pixEdgeMapWidth;
				const UBYTE	ubEdgePixel = pubEdgeSrc[pix];
				ASSERT( pix < pixEdgeMapWidth * pixEdgeMapHeight );
				if( ubEdgePixel > 127 )
					ctEdgePixels++;
			}
		}

		// check for edge map optimizations
		if( ctEdgePixels == 0 )
			ttTile.tt_ulFlags |= TTF_BLANK_EDGE_MAP;
		else if( ctEdgePixels == ctPixels )
			ttTile.tt_ulFlags |= TTF_FULL_EDGE_MAP;

		const INDEX	cttrl = ptr->tr_atlLayers.Count();
		for( INDEX itrl = 0; itrl < cttrl; itrl++ )
		{
			const CTerrainLayer		*ptrLayer = &ptr->tr_atlLayers[itrl];
			const CTerrainLayerImp	*ptrl = ptrLayer->trl_trlLayerImp;
			const UBYTE	*pubLayerMaskSrc = ptrl->trl_pubLayerMask;
			const PIX	pixLayerMaskWidth  = ( pixHeightMapWidth - 1 ) << ptrLayer->tl_slMaskStretch;			// yjpark |<--
			const PIX	pixLayerMaskHeight = ( pixHeightMapHeight - 1 ) << ptrLayer->tl_slMaskStretch;
			const PIX	pixLeftMask = ttTile.tt_rcSource.rc_slLeft << ptrLayer->tl_slMaskStretch;
			const PIX	pixTopMask = ttTile.tt_rcSource.rc_slTop << ptrLayer->tl_slMaskStretch;
			const PIX	pixWidthMask = ( ttTile.tt_rcSource.GetWidth() - 1 ) << ptrLayer->tl_slMaskStretch;
			const PIX	pixHeightMask = ( ttTile.tt_rcSource.GetHeight() - 1 ) << ptrLayer->tl_slMaskStretch;	// yjpark     -->|

			INDEX ctLayerPixels = 0;
			for( PIX pixY = 0; pixY < pixHeightMask; pixY++ )				// yjpark |<--
			{
				for( PIX pixX = 0; pixX < pixWidthMask; pixX++ )
				{
					const PIX	pixRealX = pixLeftMask + pixX;
					const PIX	pixRealY = pixTopMask + pixY;				// yjpark     -->|
					const PIX	pix = pixRealX + pixRealY*pixLayerMaskWidth;
					const UBYTE	ubLayerPixel = pubLayerMaskSrc[pix];
					ASSERT( pix < pixLayerMaskWidth * pixLayerMaskHeight );
					if( ubLayerPixel > 0 )
						ctLayerPixels++;
				}
			}

			if( itrl < 32 )
			{
				const ULONG	ulVisLayerMask = ( 1UL << itrl );
				// if no layer pixels for this tile
				if( ctLayerPixels == 0 )	// Mark layer invisible for this tile
					ttTile.tt_ulVisLayers &= ~ulVisLayerMask;
				else						// Mark layer visible for this tile
					ttTile.tt_ulVisLayers |= ulVisLayerMask;
			}
		}
	}
}

static void ClearTerrainTileData(CTerrainImp *ptr)
{
	ASSERT(ptr->tr_ctTilesX * ptr->tr_ctTilesY == ptr->tr_attTiles.Count()); // must be same
	ptr->tr_ctTilesX = 0;
	ptr->tr_ctTilesY = 0;
	ptr->tr_attTiles.Clear();
}

extern void RegenerateTerrainTiles(CTerrainImp *ptr)
{
#if 0&&TERRAIN_DEBUG
	if(ptr->tr_artRegenQueue.Count()>0) {
		CPrintF("/*************************\\\n");
	}
#endif
	BOOL bQuadTreeChanged = FALSE;
	const INDEX ctrt = ptr->tr_artRegenQueue.Count();
	for(INDEX irt=0;irt<ctrt;irt++) {
		const RegenerateTile &rt = ptr->tr_artRegenQueue[irt];
		const INDEX iTerrainTile = rt.rt_iTileIndex;
		const CTerrainTile &ttTile = ptr->tr_attTiles[iTerrainTile];
#if 0&&TERRAIN_DEBUG
		CPrintF("%4d [%2d->%2d]\n",iTerrainTile,ttTile.tt_iCurrentLOD,ttTile.tt_iRequestedLOD);
#endif

		// if tile needs to regenerate geometry
		if(rt.rt_ulRegenFlags&RTF_GEOMETRY) {
			GenerateTileGeometry(ptr,iTerrainTile);
		}
		// if tile needs to regenerate top map
		if(rt.rt_ulRegenFlags&RTF_TOPMAP) {
		}
		// if tile needs to regenerate quad tree node
		if(rt.rt_ulRegenFlags&RTF_QUADNODE) {
			QuadTreeNode &qtn = ptr->tr_aqtnQuadTreeNodes[iTerrainTile];
			UpdateTileQTNBox(ptr, iTerrainTile);
			bQuadTreeChanged = TRUE;
		}
	}
	// Clear whole regen queue (for now)
	ptr->tr_artRegenQueue.PopAll();

	// if quad tree has changed 
	if(bQuadTreeChanged) {
		// Update all higher qtn
		UpdateHighterTerrainQuadTreeNodes(ptr);
	}
}

static void GenerateTerrainData(CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	// no height map, no terrain
	if(ptr->tr_puwHeightMap==NULL) {
		return;
	}
	
	// Calculate max posible tile LOD
	INDEX ctQuadsPerRow = ptr->tr_ctQuadsPerTileX;
	INDEX iMaxTileLOD = 0;
	while(ctQuadsPerRow>2) {
		iMaxTileLOD++;
		ctQuadsPerRow = ctQuadsPerRow>>1;
	}
	ptr->tr_iMaxTileLOD = iMaxTileLOD;

	// Calculate edge and shadow map transform matrix
	const FLOAT fStretchX = 1.0f/(ptr->tr_pixHeightMapWidth-1);
	const FLOAT fStretchY = 1.0f/(ptr->tr_pixHeightMapHeight-1);
	Matrix12 mTransf;
	MakeIdentityMatrix(mTransf);
	mTransf[ 0] = fStretchX;
	mTransf[ 5] = fStretchY;
	// Set top map transfrom matrix
	MatrixCopy(ptr->tr_mTopMap,mTransf);
	// Set edge map transform matrix
	MatrixCopy(ptr->tr_mEdgeMap,mTransf);
	// Set shadow map transform matrix
	MatrixCopy(ptr->tr_mShadowMap,mTransf);

	// Generate terrain tile data
	GenerateTerrainTileData(ptr);
	// Generate terrain tile optimization data
	TR_GenerateTerrainTileOptimizationData(ptrTerrain);
	// Generate terrain quad tree
	GenerateTerrainQuadTree(ptr);

	// Mark terrain as initialized
	ptrTerrain->tr_ulTerrainFlags|=TR_INITIALIZED;
}

static void ClearTerrainData(CTerrainImp *ptr)
{
	// Clear terrain tile data
	ClearTerrainTileData(ptr);
	// Clear terrain quad tree
	ClearTerrainQuadTree(ptr);
}

extern void QueueTileRegeneration(CTerrainImp *ptr, INDEX iTerrainTile, ULONG ulForcedFlags/*=0*/, BOOL bNeighbor/*=FALSE*/)
{
	ASSERT(ptr!=NULL); // must be valid
	const CTerrainTile &ttTile = ptr->tr_attTiles[iTerrainTile];
	// Default regeneration flags are only forced flags
	ULONG ulRegenFlags = ulForcedFlags;
	const BOOL bDifferentLOD = ttTile.tt_iCurrentLOD!=ttTile.tt_iRequestedLOD;
	const BOOL bInMip        = ttTile.tt_iRequestedLOD>0;

	// if tile lod has changed
	if(bDifferentLOD) {
		// regenerate tile geometry
		ulRegenFlags|=RTF_GEOMETRY;
	}
	// if tile is in mip and geometry has changed
	if(bDifferentLOD && bInMip) {
		// Regenerate tile topmap
		ulRegenFlags|=RTF_TOPMAP;
	}

	INDEX irtTileIndex = -1; // index of tile in reqen queue

	// for each tile in reqen queue
	const INDEX ctrt = ptr->tr_artRegenQueue.Count();
	for(INDEX irt=0;irt<ctrt;irt++) {
		const RegenerateTile &rt = ptr->tr_artRegenQueue[irt];
		// if tile is allready in regen queue
		if(rt.rt_iTileIndex==iTerrainTile) {
			// remember its index
			irtTileIndex = irt;
			break;
		}
	}

	// if tile isn't in reqen queue
	if(irtTileIndex==-1) {
		// add it now
		RegenerateTile &rt = ptr->tr_artRegenQueue.Push();
		rt.rt_iFrameRequest = 0;
		rt.rt_iTileIndex = iTerrainTile;
		rt.rt_ulRegenFlags = ulRegenFlags;

		if(bNeighbor) {
			TR_SETINFO(ti_ctNeighborsQueued++);
		} else {
			TR_SETINFO(ti_ctTileQueued++);
		}
	} else {
		#pragma message(">> Update QueueTileRegeneration for tile that are in queue")
		// ASSERT(FALSE); // not supported yet
	}
}

extern void TR_PrepareTexture(CTextureData &td, PIX pixWidth, PIX pixHeight,ULONG ulFlags)
{
	// if new texture has diff size from last one
	const BOOL bClearTexture = td.td_mexWidth!=pixWidth || td.td_mexHeight!=pixHeight ||
								( td.td_ulFlags & TEX_COMPRESSED );			// yjpark
	const BOOL bZeroMemory = ulFlags&PTF_ZEROMEM;
	if(bClearTexture) {
		// Clear texture
		td.Clear();
		td.DefaultAnimation();
		td.td_mexWidth = pixWidth;
		td.td_mexHeight = pixHeight;
		ASSERT(td.td_pulFrames==NULL);
	}

	if(td.td_pulFrames==NULL) {
		// Allocate memory for texture
		INDEX ctMipMaps = GetNoOfMipmaps(pixWidth,pixHeight);
		SLONG slSize = GetMipmapOffset(ctMipMaps,pixWidth,pixHeight)*BYTES_PER_TEXEL;
		td.td_pulFrames = (ULONG*)AllocMemory(slSize);
		td.td_slFrameSize = slSize;
		td.td_ctFrames = 1;
		td.td_iFirstMipLevel = 0;
		td.td_ctFineMipLevels = GetNoOfMipmaps(pixWidth,pixHeight);
	}

	if(bZeroMemory) {
		const SLONG slSizeZM = pixWidth*pixHeight*sizeof(PIX);
		memset(td.td_pulFrames, 0xFF, slSizeZM);
	}

	td.td_ulFlags = 0;
	if(ulFlags&PTF_STATIC) {
		td.td_ulFlags |= TEX_STATIC;
	}
	if(ulFlags&PTF_ALPHAONLY) {
		#pragma message(">> Alpha only?!?")
		td.td_ulInternalFormat = TS.ts_tfRGBA8;
	} else {
		td.td_ulInternalFormat = TS.ts_tfRGBA8;
	}
}

extern void TR_UploadTexture(CTextureData &td, PIX pixWidth, PIX pixHeight,ULONG ulFlags)
{
	// make mipmaps
	MakeMipmaps(td.td_pulFrames, pixWidth, pixHeight);
	td.SetAsCurrent(0,TRUE);
}

static INDEX _ctTexturesUsed = 0;
static INDEX _ctTexturesCreated = 0;
extern CTextureData *TR_CreateTexture(void)
{
	// Create new texture
	CTextureData *ptdNewTexture = new CTextureData;
	_ctTexturesCreated++;
	// Mark texture used
	TR_MarkTextureUsed(ptdNewTexture);
	// return pointer to created texture
	return ptdNewTexture;
}

#if _DEBUG
class CTempTexCheck {
public:
	CTempTexCheck(void) {
		ASSERT(_ctTexturesUsed==0);
		ASSERT(_ctTexturesCreated==0);
	}
	~CTempTexCheck(void) {
		ASSERT(_ctTexturesUsed==0);
		ASSERT(_ctTexturesCreated==0);
	}
};
CTempTexCheck ttc;
#endif

extern void TR_MarkTextureUsed(CTextureData *ptd)
{
	ASSERT(ptd->ser_ctUsed>=0);
	_ctTexturesUsed++;
	ptd->ser_ctUsed++;
}

extern void TR_MarkTextureUnused(CTextureData *ptd)
{
	ASSERT(ptd->ser_ctUsed>0);
	_ctTexturesUsed--;
	ptd->ser_ctUsed--;
	if(ptd->ser_ctUsed==0) {
		_ctTexturesCreated--;
		ptd->Clear();
	// WSS_BUGFIX 070510
	// Top Map 관련 수정 사항 처리 
	// 여기서 Delete를 하면 이후 ptd->ser_ctUsed를 다시 참조하는 곳이 있어 문제 발생 주석 처리
	// delete ptd;
	}
}

// Bake top map
extern void TR_BakeTopMap(CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain->tr_ptdTopMap!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	if( ptr->tr_iFirstTopMapLOD < 0 )		// yjpark 
		return ;

	const PIX pixHeightMapWidth  = ptr->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX pixTopMapWidth     = ptr->tr_pixTopMapWidth;
	const PIX pixTopMapHeight    = ptr->tr_pixTopMapHeight;
	const PIX pixLayerMaskWidth  = pixHeightMapWidth-1;
	const PIX pixLayerMaskHeight = pixHeightMapHeight-1;
	const PIX pixEdgeMapWidth    = pixHeightMapWidth-1;
	const PIX pixEdgeMapHeight   = pixHeightMapHeight-1;
	const PIX pixTopMapPixels    = pixTopMapWidth*pixTopMapHeight;

	// Prepare top map texture
	TR_PrepareTexture(*ptr->tr_ptdTopMap,pixTopMapWidth,pixTopMapHeight,PTF_STATIC|PTF_ZEROMEM);
	ULONG *pulDst = ptr->tr_ptdTopMap->td_pulFrames;
	const ULONG *pulEMapSrc = ptr->tr_ptdEdgeMap->td_pulFrames;
	ASSERT(pulEMapSrc!=NULL);

	/*
	 * Bake edge map in top map
	 */ 
	const FLOAT fEMapRatioX  = (FLOAT)pixTopMapWidth/pixEdgeMapWidth;
	const FLOAT fEMapRatioY  = (FLOAT)pixTopMapHeight/pixEdgeMapHeight;

	FLOAT fEMapStepUI = (1.0f/fEMapRatioX);
	FLOAT fEMapStepVJ = (1.0f/fEMapRatioY);
	const FLOAT fEMapStepUJ = 0.0f;
	const FLOAT fEMapStepVI = 0.0f;

	INDEX iEMapMipSrc = 0;
	SLONG slEMapMipOffset = 0;
	const INDEX iEMapMaxMip = GetNoOfMipmaps(pixEdgeMapWidth, pixEdgeMapHeight);
	const SLONG slEMapMaxMipOffset = GetMipmapOffset(255, pixEdgeMapWidth, pixEdgeMapHeight);

	while((fEMapStepUI>1.0f || fEMapStepVJ>1.0f) && (iEMapMipSrc+1<iEMapMaxMip)) {
		fEMapStepUI/=2.0f;
		fEMapStepVJ/=2.0f;
		const PIX pixEMMipWidth = pixEdgeMapWidth>>iEMapMipSrc;
		const PIX pixEMMipHeight = pixEdgeMapHeight>>iEMapMipSrc;
		slEMapMipOffset += pixEMMipWidth*pixEMMipHeight;
		iEMapMipSrc++;
	}
	ASSERT(iEMapMipSrc<iEMapMaxMip);

	const PIX pixEMapMipWidth   = (pixEdgeMapWidth>>iEMapMipSrc);
	const PIX pixEMapMipHeight  = (pixEdgeMapHeight>>iEMapMipSrc);

	FLOAT fEMapStartU  = 0.0f;
	FLOAT fEMapStartV  = 0.0f;

	FLOAT fEMapPosU;
	FLOAT fEMapPosV;

	for(INDEX iPixY=0;iPixY<pixTopMapHeight;iPixY++) {
		fEMapPosU = fEMapStartU;
		fEMapPosV = fEMapStartV;
		for(INDEX iPixX=0;iPixX<pixTopMapWidth;iPixX++) {
			const PIX pixEMapU = FloatToInt(fEMapPosU)&(pixEMapMipWidth-1); 
			const PIX pixEMapV = FloatToInt(fEMapPosV)&(pixEMapMipHeight-1);
			// const PIX pixEMapU = ((PIX)floor(fEMapPosU))&(pixEMapMipWidth-1);
			// const PIX pixEMapV = ((PIX)floor(fEMapPosV))&(pixEMapMipHeight-1);
			const PIX pixEMap  = pixEMapU + pixEMapV*pixEMapMipWidth;
			const PIX pixTMap   = iPixX + iPixY*pixTopMapWidth;
			const SLONG slEMapOffset = slEMapMipOffset+pixEMap;

			ASSERT(pixEMapU>=0 && pixEMapU<pixEMapMipWidth);
			ASSERT(pixEMapV>=0 && pixEMapV<pixEMapMipHeight);
			ASSERT(slEMapOffset>=0 && slEMapOffset<slEMapMaxMipOffset);

			const SLONG slEdge = (pulEMapSrc[slEMapOffset]&0xFF000000)>>24;
			const ULONG ulEMap = (slEdge<<24)|0x00FFFFFF;

			pulDst[pixTMap] = ulEMap;
			fEMapPosU  += fEMapStepUI;
			fEMapPosV  += fEMapStepVI;
		}
		fEMapStartU  += fEMapStepUJ;
		fEMapStartV  += fEMapStepVJ;
	}



	/*
	 * Bake layers in top map
	 */

	// for each terrain layer
	const INDEX cttrl = ptr->tr_atlLayers.Count();
	for(INDEX itrl=0;itrl<cttrl;itrl++) {
		const CTerrainLayer *ptlLayer = &ptr->tr_atlLayers[itrl];
		const CTerrainLayerImp *ptrl = ptlLayer->trl_trlLayerImp;
		const PIX pixLayerTexWidth = ptrl->trl_ptdTexture->GetPixWidth();
		const PIX pixLayerTexHeight = ptrl->trl_ptdTexture->GetPixHeight();
		const ULONG *pulLTexSrc = ptrl->trl_ptdTexture->td_pulFrames;
		const ULONG *pulLMaskSrc = ptrl->trl_ptdLayerMask->td_pulFrames;
		ASSERT(pulLTexSrc!=NULL);
		ASSERT(pulLMaskSrc!=NULL);

		const FLOAT fRotU = ptlLayer->tl_fRotateX;
		const FLOAT fRotV = ptlLayer->tl_fRotateY;
		const FLOAT fStrU = (ptr->tr_ctQuadsPerTileX*ptr->tr_ctTilesX) / ptlLayer->tl_fStretchX;
		const FLOAT fStrV = (ptr->tr_ctQuadsPerTileX*ptr->tr_ctTilesY) / ptlLayer->tl_fStretchY;

		// Set texture ratios
		const FLOAT fLTexRatioX  = (FLOAT)pixTopMapWidth/pixLayerTexWidth;
		const FLOAT fLTexRatioY  = (FLOAT)pixTopMapHeight/pixLayerTexHeight;
		const FLOAT fLMaskRatioX = (FLOAT)pixTopMapWidth/pixLayerMaskWidth;
		const FLOAT fLMaskRatioY = (FLOAT)pixTopMapHeight/pixLayerMaskHeight;

		// Set texture steps
		FLOAT fLTexStepUI = CosFast(fRotU) * fStrU * (1.0f/fLTexRatioX);
		FLOAT fLTexStepUJ =-SinFast(fRotV) * fStrV * (1.0f/fLTexRatioY);
		FLOAT fLTexStepVI = SinFast(fRotU) * fStrU * (1.0f/fLTexRatioX);
		FLOAT fLTexStepVJ = CosFast(fRotV) * fStrV * (1.0f/fLTexRatioY);

		FLOAT fLMaskStepUI = (1.0f/fLMaskRatioX);
		FLOAT fLMaskStepVJ = (1.0f/fLMaskRatioY);
		const FLOAT fLMaskStepUJ = 0.0f;
		const FLOAT fLMaskStepVI = 0.0f;

		// Set max mip sources and max offsets
		INDEX iLTexMipSrc = 0;
		SLONG slLTexMipOffset = 0;
		const INDEX iLTexMaxMip = GetNoOfMipmaps(pixLayerTexWidth, pixLayerTexHeight);
		const SLONG slLTexMaxMipOffset = GetMipmapOffset(255, pixLayerTexWidth, pixLayerTexHeight);

		INDEX iLMaskMipSrc = 0;
		SLONG slLMaskMipOffset = 0;
		const INDEX iLMaskMaxMip = GetNoOfMipmaps(pixLayerMaskWidth, pixLayerMaskHeight);
		const SLONG slLMaskMaxMipOffset = GetMipmapOffset(255, pixLayerMaskWidth, pixLayerMaskHeight);

		while((Max(fLTexStepUI,fLTexStepUJ)>1.0f || Max(fLTexStepVI,fLTexStepVJ)>1.0f) && (iLTexMipSrc+1<iLTexMaxMip)) {
			fLTexStepUI/=2.0f;
			fLTexStepUJ/=2.0f;
			fLTexStepVI/=2.0f;
			fLTexStepVJ/=2.0f;
			const PIX pixLTMipWidth = pixLayerTexWidth>>iLTexMipSrc;
			const PIX pixLTMipHeight = pixLayerTexHeight>>iLTexMipSrc;
			slLTexMipOffset += pixLTMipWidth*pixLTMipHeight;
			iLTexMipSrc++;
		}
		ASSERT(iLTexMipSrc<iLTexMaxMip);

		while((fLMaskStepUI>1.0f || fLMaskStepVJ>1.0f) && (iLMaskMipSrc+1<iLMaskMaxMip)) {
			fLMaskStepUI/=2.0f;
			fLMaskStepVJ/=2.0f;
			const PIX pixLMMipWidth = pixLayerMaskWidth>>iLMaskMipSrc;
			const PIX pixLMMipHeight = pixLayerMaskHeight>>iLMaskMipSrc;
			slLMaskMipOffset += pixLMMipWidth*pixLMMipHeight;
			iLMaskMipSrc++;
		}
		ASSERT(iLMaskMipSrc<iLMaskMaxMip);

		const PIX pixLTexMipWidth   = (pixLayerTexWidth>>iLTexMipSrc);
		const PIX pixLTexMipHeight  = (pixLayerTexHeight>>iLTexMipSrc);
		const PIX pixLMaskMipWidth  = (pixLayerMaskWidth>>iLMaskMipSrc);
		const PIX pixLMaskMipHeight = (pixLayerMaskHeight>>iLMaskMipSrc);

		const FLOAT fOffU = ptlLayer->tl_fOffsetX * pixLTexMipWidth;
		const FLOAT fOffV = ptlLayer->tl_fOffsetY * pixLTexMipHeight;

		FLOAT fLTexStartU  = CosFast(fRotU)*fOffU - SinFast(fRotV)*fOffV;
		FLOAT fLTexStartV  = SinFast(fRotU)*fOffU + CosFast(fRotV)*fOffV;
		FLOAT fLMaskStartU = 0.0f;
		FLOAT fLMaskStartV = 0.0f;

		FLOAT fLTexPosU;
		FLOAT fLTexPosV;
		FLOAT fLMaskPosU;
		FLOAT fLMaskPosV;
		
		for(INDEX iPixY=0;iPixY<pixTopMapHeight;iPixY++) {
			fLTexPosU = fLTexStartU;
			fLTexPosV = fLTexStartV;
			fLMaskPosU = fLMaskStartU;
			fLMaskPosV = fLMaskStartV;

			for(INDEX iPixX=0;iPixX<pixTopMapWidth;iPixX++) {

				const PIX pixLTexU = FloatToInt(fLTexPosU)&(pixLTexMipWidth-1);
				const PIX pixLTexV = FloatToInt(fLTexPosV)&(pixLTexMipHeight-1);
				const PIX pixLMaskU = FloatToInt(fLMaskPosU)&(pixLMaskMipWidth-1);
				const PIX pixLMaskV = FloatToInt(fLMaskPosV)&(pixLMaskMipHeight-1);

				const PIX pixLTex  = pixLTexU + pixLTexV*pixLTexMipWidth;
				const PIX pixLMask = pixLMaskU + pixLMaskV*pixLMaskMipHeight;
				const PIX pixTMap  = iPixX + iPixY*pixTopMapWidth;

				const SLONG slLTexOffset = slLTexMipOffset+pixLTex;
				const SLONG slLMaskOffset = slLMaskMipOffset+pixLMask;

				ASSERT(pixLTexU>=0 && pixLTexU<pixLTexMipWidth);
				ASSERT(pixLTexV>=0 && pixLTexV<pixLTexMipHeight);
				ASSERT(slLTexOffset>=0 && slLTexOffset<slLTexMaxMipOffset);

				ASSERT(pixLMaskU>=0 && pixLMaskU<pixLMaskMipWidth);
				ASSERT(pixLMaskV>=0 && pixLMaskV<pixLMaskMipHeight);
				ASSERT(slLMaskOffset>=0 && slLMaskOffset<slLMaskMaxMipOffset);

/*
// SW
				F  = Fc*Fa + D*(1-Fa)

// HW
				F  = Fc*1.0f + D*Fa
				Fc = Lc0 * (La0 * Ma0)
				Fa = 1-(La0 * Ma0);
*/

				// Get layer texture pixel
				const ULONG ulTex  = pulLTexSrc[slLTexOffset];
				const SLONG slTexA = (ulTex&0xFF000000)>>24;
				// Get topmap pixel
				const ULONG ulDst = pulDst[pixTMap];
				// Get layer mask alpha
				const SLONG slMaskA = (pulLMaskSrc[slLMaskOffset]&0xFF000000)>>24;

				// Get source alpha
				const SLONG slSrcA = (slTexA*slMaskA)>>8;
				const SLONG slInvSrcA = 0xFF - slSrcA;


				const SLONG slDstR = (((ulDst&0x000000FF)>>0 ) * slInvSrcA)>>8;
				const SLONG slDstG = (((ulDst&0x0000FF00)>>8 ) * slInvSrcA)>>8;
				const SLONG slDstB = (((ulDst&0x00FF0000)>>16) * slInvSrcA)>>8;
				const SLONG slDstA =  ((ulDst&0xFF000000)>>24);

				const SLONG slSrcR = (((ulTex&0x000000FF)>>0 ) * slSrcA)>>8;
				const SLONG slSrcG = (((ulTex&0x0000FF00)>>8 ) * slSrcA)>>8;
				const SLONG slSrcB = (((ulTex&0x00FF0000)>>16) * slSrcA)>>8;

				pulDst[pixTMap] = slDstA<<24 |
									(ClampUp(slSrcB+slDstB, 255L)<<16) |
									(ClampUp(slSrcG+slDstG, 255L)<<8 ) |
									(ClampUp(slSrcR+slDstR, 255L)<<0 );

				fLTexPosU  += fLTexStepUI;
				fLTexPosV  += fLTexStepVI;
				fLMaskPosU += fLMaskStepUI;
				fLMaskPosV += fLMaskStepVI;
			}
			fLTexStartU  += fLTexStepUJ;
			fLTexStartV  += fLTexStepVJ;
			fLMaskStartU += fLMaskStepUJ;
			fLMaskStartV += fLMaskStepVJ;
		}
	}
	// Upload top map texture
	TR_UploadTexture(*ptr->tr_ptdTopMap,pixTopMapWidth,pixTopMapHeight,PTF_STATIC);
	// Flag that top map has been synced
	ptrTerrain->tr_ulTerrainFlags&=~TR_SYNC_TOPMAP;
}

// Rebuild terrain geometry
extern void TR_RegenerateAll(CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	// Clear terrain data
	ClearTerrainData(ptr);
	// Generate new terrain data
	GenerateTerrainData(ptrTerrain);
}

extern void TR_ClearTerrain(CTerrain *ptrTerrain)	// yjpark |<--
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	// Discard shading infos on terrain
	TR_DiscardShadingInfos(ptrTerrain);

	// Clear terrain data
	ClearTerrainData(ptr);

	// Free height map memory
	if( ptr->tr_puwHeightMap != NULL )
	{
		FreeMemory( ptr->tr_puwHeightMap );
		ptr->tr_puwHeightMap = NULL;
	}

	// Free edge map memory
	if( ptr->tr_pubEdgeMap != NULL )
	{
		FreeMemory( ptr->tr_pubEdgeMap );
		ptr->tr_pubEdgeMap = NULL;
	}

	// Free attribute map memory
	if( ptr->tr_pubAttributeMap != NULL )			
	{
		FreeMemory( ptr->tr_pubAttributeMap );
		ptr->tr_pubAttributeMap = NULL;
	}

	// Free shadow map texture memory
	if( ptr->tr_ptdShadowMap != NULL )
	{
		TR_MarkTextureUnused( ptr->tr_ptdShadowMap );
		ptr->tr_ptdShadowMap = NULL;
	}

	extern BOOL	_bWorldEditorApp;
	if( _bWorldEditorApp || ptr->tr_iTerrainVersion < 16 )
	{
		// Free top map texture memory
		if( ptr->tr_ptdTopMap != NULL )
		{
			TR_MarkTextureUnused( ptr->tr_ptdTopMap );
			ptr->tr_ptdTopMap = NULL;
		}

		// Free edge map texture
		if( ptr->tr_ptdEdgeMap != NULL )
		{
			TR_MarkTextureUnused( ptr->tr_ptdEdgeMap );
			ptr->tr_ptdEdgeMap = NULL;
		}
	}
	else
	{
		// Free top map texture
		if( ptr->tr_ptdTopMap != NULL )
		{
			_pTextureStock->Release( ptr->tr_ptdTopMap );
			ptr->tr_ptdTopMap = NULL;
		}

		// Free edge map texture
		if( ptr->tr_ptdEdgeMap != NULL )
		{
			_pTextureStock->Release( ptr->tr_ptdEdgeMap );
			ptr->tr_ptdEdgeMap = NULL;
		}
	}

	// Free shadow map texture
	INDEX	cttrs = ptr->tr_aptdShadowMaps.Count();
	for( INDEX itrs = 0; itrs < cttrs; itrs++ )
	{
		if( ptr->tr_aptdShadowMaps[itrs] )
		{
			_pTextureStock->Release( ptr->tr_aptdShadowMaps[itrs] );
			ptr->tr_aptdShadowMaps[itrs] = NULL;
		}
	}

	ptr->tr_aptdShadowMaps.Clear();
	ptr->tr_aulShadowTimes.Clear();
	ptr->tr_afShadowBlurRadius.Clear();
	ptr->tr_acolObjShadowColor.Clear();

	ASSERT(ptr->tr_puwShadingMap==NULL); // Not used, yet?

	// Free layers
	if( _bWorldEditorApp || ptr->tr_iTerrainVersion < 16 )
	{
		// for each layer 
		const INDEX cttrl = ptr->tr_atlLayers.Count();
		for( INDEX itrl = 0; itrl < cttrl; itrl++ )
			TRL_DeleteLayer( ptrTerrain, 0, TRUE );
	}
	else
		ptr->tr_atlLayers.Clear();
}													// yjpark     -->|

// Resize existing height map
extern void TR_SetHeightMapSize(CTerrain *ptrTerrain, PIX pixMapWidth, PIX pixMapHeight)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid

	// Validate height map dimensions
	const FLOAT fLogWidth  = Log2(pixMapWidth-1);
	const FLOAT fLogHeight = Log2(pixMapHeight-1);
	if(fLogWidth!=INDEX(fLogWidth)) {
		ASSERTALWAYS("Height map has invalid width");
		return;
	}
	if(fLogHeight!=INDEX(fLogHeight)) {
		ASSERTALWAYS("Height map has invalid height");
		return;
	}

	// Get internal terrain object
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	// Prepare terrain maps for new terrain size
	AllocateTerrainMaps(ptrTerrain, pixMapWidth, pixMapHeight);
	// Mark terrain as initialized
	ptrTerrain->tr_ulTerrainFlags|=TR_INITIALIZED;

	TR_SetTerrainSize(ptrTerrain, ptr->tr_vMetricSize);

	TR_RegenerateAll(ptrTerrain);
}

// Set terrain shadowmap size aspect (relative to height map size) and shading map aspect (relative to shadow map size)
extern void TR_SetShadowMapSize(CTerrain *ptrTerrain, INDEX iShadowMapAspect, INDEX iShadingMapAspect)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	if( ptr->tr_aptdShadowMaps.Count() == 0 )					// yjpark |<--
	{
		ptr->tr_aptdShadowMaps.New( 2 );
		ptr->tr_aulShadowTimes.New( 3 );
		ptr->tr_afShadowBlurRadius.New( 2 );
		ptr->tr_acolObjShadowColor.New( 2 );
		for( INDEX itrs = 0; itrs < 2; itrs++ )
		{
			ptr->tr_aptdShadowMaps[itrs] = TR_CreateTexture();
			ptr->tr_aulShadowTimes[itrs] = 0;
			ptr->tr_afShadowBlurRadius[itrs] = 0.0f;
			ptr->tr_acolObjShadowColor[itrs].abgr = 0xFF000000;
		}
		ptr->tr_aulShadowTimes[2] = 0;
		ptr->tr_ptdShadowMap = TR_CreateTexture();
	}															// yjpark     -->|

	const PIX pixHeightMapWidth  = ptr->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX pixShadowMapWidth  = GetShadowMapWidth(pixHeightMapWidth,iShadowMapAspect);
	const PIX pixShadowMapHeight = GetShadowMapHeight(pixHeightMapHeight,iShadowMapAspect);
	const PIX pixShadowMapSize   = pixShadowMapWidth * pixShadowMapHeight * sizeof(ULONG);			// yjpark |<--
	
	INDEX	cttrs = ptr->tr_aptdShadowMaps.Count();
	for( INDEX itrs = 0; itrs < cttrs; itrs++ )
	{
		TR_PrepareTexture( *(ptr->tr_aptdShadowMaps[itrs]), pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC | PTF_ZEROMEM );
		TR_UploadTexture( *(ptr->tr_aptdShadowMaps[itrs]), pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC );
	}

	extern BOOL	_bWorldEditorApp;
	if( _bWorldEditorApp )
	{
		TR_PrepareTexture(*ptr->tr_ptdShadowMap,pixShadowMapWidth,pixShadowMapHeight,PTF_STATIC);
		memset(ptr->tr_ptdShadowMap->td_pulFrames, 0x00, pixShadowMapSize);
		TR_UploadTexture(*ptr->tr_ptdShadowMap,pixShadowMapWidth,pixShadowMapHeight,PTF_STATIC);
	}																								// yjpark     -->|

	ptr->tr_slShadowMapSizeAspect = iShadowMapAspect;
	ptr->tr_slShadingMapSizeAspect = iShadingMapAspect;

	TR_DiscardShadingInfos(ptrTerrain);
}

// Resize existing top map
extern void TR_SetTopMapSize(CTerrain *ptrTerrain, PIX pixMapWidth, PIX pixMapHeight)
{
	ASSERT( ptrTerrain != NULL );					// must be valid
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );	// must be valid
	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;

	// if this is world editor app
	extern BOOL	_bWorldEditorApp;
	if( _bWorldEditorApp )
	{
		ptr->tr_pixTopMapWidth = pixMapWidth;
		ptr->tr_pixTopMapHeight = pixMapHeight;

		if( ptr->tr_ptdTopMap == NULL )
		{
			ptr->tr_ptdTopMap = TR_CreateTexture();
			TR_PrepareTexture( *ptr->tr_ptdTopMap, pixMapWidth, pixMapHeight, PTF_STATIC );
		}

		// Bake terrain top map
		TR_BakeTopMap( ptrTerrain );
	}
	else
	{
		ASSERTALWAYS( "Top map can only be resized from we" );
	}
}

// yjpark |<--
//-------------------------------------------------------------------------------------------------
// Desc : These contents aren't included in final release version
// ------------------------------------------------------------------------------------------------
#ifndef FINALVERSION

extern void TR_SetAttributeMapSize( CTerrain *ptrTerrain )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;

	if( ptr->tr_vMetricSize( 1 ) == 0.0f )
		return ;

	ASSERT( ptr->tr_vMetricSize( 1 ) == ptr->tr_vMetricSize( 3 ) );
	const PIX	pixMapWidth = ptr->tr_pixHeightMapWidth - 1;

	if (ptr->tr_vMetricSize(1) > pixMapWidth)
	{
		ptr->tr_slAttributeMapSizeAspect = ptr->tr_vMetricSize( 1 ) / pixMapWidth;
	}
	else
	{
		ptr->tr_slAttributeMapSizeAspect = 1;
	}

	if( ptr->tr_pubAttributeMap != NULL )
	{
		FreeMemory( ptr->tr_pubAttributeMap );
		ptr->tr_pubAttributeMap = NULL;
	}

	const SLONG	slAttributeMapSize = ptr->tr_vMetricSize( 1 ) * ptr->tr_vMetricSize( 3 ) * sizeof(UWORD);
	ptr->tr_pubAttributeMap = (UWORD*)AllocMemory( slAttributeMapSize );
}

// Import height map from targa file
extern void TR_ImportHeightMap_t(CTerrain *ptrTerrain, const CTFileName &fnm, BOOL bUse16b/*=TRUE*/)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid

	// Load terrain height map
	CImageInfo iiHeightMap;
	iiHeightMap.LoadAnyGfxFormat_t(fnm);

	const PIX pixMapWidth = iiHeightMap.ii_Width;
	const PIX pixMapHeight = iiHeightMap.ii_Height;
	const SLONG slHeightMapSize = pixMapWidth*pixMapHeight;
	const INDEX iBpp = iiHeightMap.ii_BitsPerPixel/8;


	// Validate height map dimensions
	const FLOAT fLogWidth  = Log2(pixMapWidth-1);
	const FLOAT fLogHeight = Log2(pixMapHeight-1);
	if(fLogWidth!=INDEX(fLogWidth)) {
		ThrowF_t("Height map from '%s' has invalid width",(const char*)fnm);
	}
	if(fLogHeight!=INDEX(fLogHeight)) {
		ThrowF_t("Height map from '%s' has invalid width",(const char*)fnm);
	}

	// Get internal terrain object
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	// Prepare terrain maps for new terrain size
	AllocateTerrainMaps(ptrTerrain, pixMapWidth, pixMapHeight);

	// Read height map from file
	const UBYTE *pubSrc = &iiHeightMap.ii_Picture[0];
	UWORD *puwDst = &ptr->tr_puwHeightMap[0];
	if(bUse16b) {
		for(PIX pix=0;pix<slHeightMapSize;pix++) {
			*puwDst = *(UWORD*)pubSrc;
			puwDst++;
			pubSrc+=iBpp;
		}
	} else {
		for(PIX pix=0;pix<slHeightMapSize;pix++) {
			*puwDst = *(UBYTE*)pubSrc<<8;
			puwDst++;
			pubSrc+=iBpp;
		}
	}

	TR_RegenerateAll(ptrTerrain);
}

// Export height map to targa file
extern void TR_ExportHeightMap_t(CTerrain *ptrTerrain, const CTFileName &fnm, BOOL bUse16b/*=TRUE*/)
{
	ASSERT(ptrTerrain!=NULL); // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	
	const CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	if(ptr->tr_puwHeightMap==NULL) {
		ThrowF_t("Terrain has no height map to export");
	}

	const PIX pixMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX pixMapHeight = ptr->tr_pixHeightMapHeight;
	const SLONG slHeightMapSize = pixMapWidth*pixMapHeight;

	CImageInfo iiHeightMap;
	iiHeightMap.ii_Width = pixMapWidth;
	iiHeightMap.ii_Height = pixMapHeight;
	iiHeightMap.ii_BitsPerPixel = 32;
	iiHeightMap.ii_Picture = (UBYTE*)AllocMemory(slHeightMapSize*iiHeightMap.ii_BitsPerPixel/8);
	memset(iiHeightMap.ii_Picture,0,slHeightMapSize*iiHeightMap.ii_BitsPerPixel/8);

	const UWORD *puwSrc = &ptr->tr_puwHeightMap[0];
	ULONG *pulDst = (ULONG*)&iiHeightMap.ii_Picture[0];

	if(bUse16b) {
		for(PIX pix=0;pix<slHeightMapSize;pix++) {
			UWORD *puwData = (UWORD*)&pulDst[0];
			*puwData = *puwSrc;
			pulDst++;
			puwSrc++;
		}
	} else {
		for(PIX pix=0;pix<slHeightMapSize;pix++) {
			UBYTE *pubData = (UBYTE*)&pulDst[0];
			*pubData = (UBYTE)((*puwSrc)>>8);
			pulDst++;
			puwSrc++;
		}
	}

	iiHeightMap.SaveTGA_t(fnm);
	iiHeightMap.Clear();
}

// Import edge map from targa file
extern void TR_ImportEdgeMap_t(CTerrain *ptrTerrain, const CTFileName &fnm)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	ASSERT(ptr->tr_puwHeightMap!=NULL);
	ASSERT(ptr->tr_pubEdgeMap!=NULL);

	// Load terrain edge map
	CImageInfo iiImage;
	iiImage.LoadAnyGfxFormat_t(fnm);

	const PIX pixMapWidth = iiImage.ii_Width;
	const PIX pixMapHeight = iiImage.ii_Height;
	const SLONG slEdgeMapSize = pixMapWidth*pixMapHeight;
	const INDEX iBpp = iiImage.ii_BitsPerPixel/8;

	if(pixMapWidth+1!=ptr->tr_pixHeightMapWidth) {
		ThrowF_t("Edge map width must match terrain height map width -1");
	}
	if(pixMapHeight+1!=ptr->tr_pixHeightMapHeight) {
		ThrowF_t("Edge map height must match terrain height map height -1");
	}

	// Read height map from file
	const UBYTE *pubSrc = &iiImage.ii_Picture[0];
	UBYTE *pubDst = &ptr->tr_pubEdgeMap[0];

	// Create edge map that will be in local memory
	for(PIX pix=0;pix<slEdgeMapSize;pix++) {
		*pubDst = *(UBYTE*)pubSrc;
		pubDst++;
		pubSrc+=iBpp;
	}

	// Upload edge map
	TR_UploadEdgeMap(ptrTerrain);
}

// Export edge map to targa file
extern void TR_ExportEdgeMap_t(CTerrain *ptrTerrain, const CTFileName &fnm)
{
	ASSERT(ptrTerrain!=NULL); // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	
	const CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	if(ptr->tr_pubEdgeMap==NULL) {
		ThrowF_t("Terrain has no edge map to export");
	}

	const PIX pixMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX pixMapHeight = ptr->tr_pixHeightMapHeight;
	const SLONG slEdgeMapSize = pixMapWidth*pixMapHeight;

	CImageInfo iiImage;
	iiImage.ii_Width = pixMapWidth;
	iiImage.ii_Height = pixMapHeight;
	iiImage.ii_BitsPerPixel = 32;
	iiImage.ii_Picture = (UBYTE*)AllocMemory(slEdgeMapSize*iiImage.ii_BitsPerPixel/8);
	memset(iiImage.ii_Picture,0,slEdgeMapSize*iiImage.ii_BitsPerPixel/8);

	const UBYTE *pubSrc = &ptr->tr_pubEdgeMap[0];
	ULONG *pulDst = (ULONG*)&iiImage.ii_Picture[0];

	for(PIX pix=0;pix<slEdgeMapSize;pix++) {
		UBYTE *pubData = (UBYTE*)&pulDst[0];
		*pubData = *pubSrc;
		pulDst++;
		pubSrc++;
	}
	iiImage.SaveTGA_t(fnm);
	iiImage.Clear();
}

// yjpark |<--
extern void TR_ClearAttributeMap( CTerrain *ptrTerrain )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;

	const SLONG	slSizeAspect = ptr->tr_slAttributeMapSizeAspect;
	const PIX	pixAttributeMapWidth  = ( ptr->tr_pixHeightMapWidth - 1 ) * slSizeAspect;
	const PIX	pixAttributeMapHeight = ( ptr->tr_pixHeightMapHeight - 1 ) * slSizeAspect;

	memset( ptr->tr_pubAttributeMap, 0, pixAttributeMapWidth * pixAttributeMapHeight * sizeof(UWORD));
}

extern void TR_ImportAttributeMap_t( CTerrain *ptrTerrain, CTFileName &fnm )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;

	const SLONG	slSizeAspect = ptr->tr_slAttributeMapSizeAspect;
	const PIX	pixAttributeMapWidth  = ( ptr->tr_pixHeightMapWidth - 1 ) * slSizeAspect;
	const PIX	pixAttributeMapHeight = ( ptr->tr_pixHeightMapHeight - 1 ) * slSizeAspect;
	const PIX	pixAttributeMapSize = pixAttributeMapWidth * pixAttributeMapHeight;
	UWORD		*pubTemp = ptr->tr_pubAttributeMap;

	CTFileStream	strmFile;
	strmFile.Open_t( fnm );

	for( PIX pix = 0; pix < pixAttributeMapSize; pix++ )
	{
		strmFile >> *pubTemp;
		pubTemp++;
	}

	strmFile.Close();
}

extern void TR_ExportAttributeMap_t( CTerrain *ptrTerrain, CTFileName &fnm )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;

	const SLONG	slSizeAspect = ptr->tr_slAttributeMapSizeAspect;
	const PIX	pixAttributeMapWidth  = ( ptr->tr_pixHeightMapWidth - 1 ) * slSizeAspect;
	const PIX	pixAttributeMapHeight = ( ptr->tr_pixHeightMapHeight - 1 ) * slSizeAspect;
	const PIX	pixAttributeMapSize = pixAttributeMapWidth * pixAttributeMapHeight;
	UWORD		*pubTemp = ptr->tr_pubAttributeMap;

	CTFileName	fnmAttributeMap;
	if( fnm.FindSubstr( CTString( "_" ) ) == -1 )
	{
		CTString	strAttrMapInfo;
		strAttrMapInfo.PrintF( "_%d_%d_0", pixAttributeMapWidth, pixAttributeMapHeight );
		fnmAttributeMap = fnm.NoExt() + strAttrMapInfo + CTString( ".sat" );
	}
	else
		fnmAttributeMap = fnm;

	CTFileStream	strmFile;
	strmFile.Create_t( fnmAttributeMap, CTStream::CM_BINARY );
	strmFile.Write_t( pubTemp, pixAttributeMapSize * sizeof(UWORD));
	strmFile.Close();
}

static void TR_DrawMapAttrLine( int nSX, int nSY, int nEX, int nEY, UBYTE aTempAttrMap[][512], UWORD ubAttr )
{
	// Starting point of line
	int	x = nSX;
	int	y = nSY;

	// Direction of line
	int	dx = nEX - nSX;
	int	dy = nEY - nSY;

	// Increment or decrement depending on direction of line
	int	sx, sy;
	
	if( dx > 0 )
		sx = 1;
	else if( dx < 0 )
	{
		sx = -1;		dx = -dx;
	}
	else
		sx = 0;

	if( dy > 0 )
		sy = 1;
	else if( dy < 0 )
	{
		sy = -1;		dy = -dy;
	}
	else
		sy = 0;

	int	ax = 2 * dx;
	int	ay = 2 * dy;

	if( dy <= dx )
	{
		// Single step in x-direction
		for( int decy = ay - dx; ; x += sx, decy += ay )
		{
			aTempAttrMap[y][x] = ubAttr;

			// Take bresenham step
			if( x == nEX )
				break;

			if( decy >= 0 )
			{
				decy -= ax;		y += sy;
			}
		}
	}
	else
	{
		// Single step in y-direction
		for( int decx = ax - dy; ; y += sy, decx += ax )
		{
			aTempAttrMap[y][x] = ubAttr;

			// Take bresenham step
			if( y == nEY )
				break;

			if( decx >= 0 )
			{
				decx -= ay;		x += sx;
			}
		}
	}
}

static void TR_DrawAttributeMap( POINT ptLB, POINT ptLT, POINT ptRT, POINT ptRB,
									UWORD *pubAttr, PIX pixAttrMapWidth, UWORD ubAttr )
{
	// Get min & max box
	POINT	ptMin = ptLB;
	POINT	ptMax = ptLB;
	if( ptLT.x < ptMin.x ) ptMin.x = ptLT.x;
	else if( ptLT.x > ptMax.x ) ptMax.x = ptLT.x;
	if( ptLT.y < ptMin.y ) ptMin.y = ptLT.y;
	else if( ptLT.y > ptMax.y ) ptMax.y = ptLT.y;
	if( ptRT.x < ptMin.x ) ptMin.x = ptRT.x;
	else if( ptRT.x > ptMax.x ) ptMax.x = ptRT.x;
	if( ptRT.y < ptMin.y ) ptMin.y = ptRT.y;
	else if( ptRT.y > ptMax.y ) ptMax.y = ptRT.y;
	if( ptRB.x < ptMin.x ) ptMin.x = ptRB.x;
	else if( ptRB.x > ptMax.x ) ptMax.x = ptRB.x;
	if( ptRB.y < ptMin.y ) ptMin.y = ptRB.y;
	else if( ptRB.y > ptMax.y ) ptMax.y = ptRB.y;

	//****************************************************************************//
	// [070620: Su-won] Attribute 맵을 생성할 때 에러 발생 -> 수정
	// 모델이 위치한 지역에 맵을 그릴 때 모델의 범위가 Terrain의 범위를 넘어가 있을 경우
	// 에러가 발생하기 때문에 맵을 그릴 범위를 Terrain의 범위에 한정시킴.

	ptMin.x = Clamp(ptMin.x, 0L, pixAttrMapWidth);
	ptMin.y = Clamp(ptMin.y, 0L, pixAttrMapWidth);
	ptMax.x = Clamp(ptMax.x, 0L, pixAttrMapWidth);
	ptMax.y = Clamp(ptMax.y, 0L, pixAttrMapWidth);
	
	//****************************************************************************//

	// Get width/height of temp attribute map
	int	nWidth = ptMax.x - ptMin.x;
	int	nHeight = ptMax.y - ptMin.y;
	UBYTE	aTempAttrMap[512][512];
	int	nX, nY;
	for( nY = 0; nY <= nHeight; nY++ )
	{
		for( nX = 0; nX <= nWidth; nX++ )
			aTempAttrMap[nY][nX] = 0;
	}	

	// Get new points
	POINT	ptNewLB, ptNewLT, ptNewRT, ptNewRB;
	ptNewLB.x = ptLB.x - ptMin.x;	ptNewLB.y = ptLB.y - ptMin.y;
	ptNewLT.x = ptLT.x - ptMin.x;	ptNewLT.y = ptLT.y - ptMin.y;
	ptNewRT.x = ptRT.x - ptMin.x;	ptNewRT.y = ptRT.y - ptMin.y;
	ptNewRB.x = ptRB.x - ptMin.x;	ptNewRB.y = ptRB.y - ptMin.y;

	// Draw map attribute line
	TR_DrawMapAttrLine( ptNewLB.x, ptNewLB.y, ptNewLT.x, ptNewLT.y, aTempAttrMap, ubAttr );
	TR_DrawMapAttrLine( ptNewLT.x, ptNewLT.y, ptNewRT.x, ptNewRT.y, aTempAttrMap, ubAttr );
	TR_DrawMapAttrLine( ptNewRT.x, ptNewRT.y, ptNewRB.x, ptNewRB.y, aTempAttrMap, ubAttr );
	TR_DrawMapAttrLine( ptNewRB.x, ptNewRB.y, ptNewLB.x, ptNewLB.y, aTempAttrMap, ubAttr );

	// Fill map attribute region
	int		nSX, nEX;
	for( nY = 0; nY <= nHeight; nY++ )
	{
		nSX = 0;	nEX = 0;
		// Find start point
		for( nX = 0; nX <= nWidth; nX++ )
		{
			if( aTempAttrMap[nY][nX] == ubAttr )
			{
				nSX = nX;	break;
			}
		}

		// Find end point
		for( nX = nWidth; nX >= 0; nX-- )
		{
			if( aTempAttrMap[nY][nX] == ubAttr )
			{
				nEX = nX;	break;
			}
		}

		// Fill attribute value from start point to end point
		if( nSX != nEX && nSX < nEX )
		{
			for( nX = nSX; nX <= nEX; nX++ )
				aTempAttrMap[nY][nX] = ubAttr;
		}
	}

	// Copy temp attribute valuse to attribute map
	UWORD	*pTemp;
	for( nY = 0; nY <= nHeight; nY++ )
	{
		pTemp = &pubAttr[ptMin.x + ( ptMin.y + nY ) * pixAttrMapWidth];
		for( nX = 0; nX <= nWidth; nX++ )
		{
			if( aTempAttrMap[nY][nX] == ubAttr )
				*pTemp = aTempAttrMap[nY][nX];
			pTemp++;
		}
	}
}

static void TR_DrawSlopAttribute( CTerrainImp *ptr, UWORD *pubAttr, PIX pixAttrMapWidth )
{
	FLOAT	fX, fY, fZ, fMinY, fMaxY, fSize;
	DOUBLE	dPi = atan( 1.0 ) * 4.0;
	DOUBLE	fMaxSlope = tan( 45.0 / 180.0 * dPi );		// Maximum slope is 45 degree
	PIX		pixSel1;									// Selected position of minimum y
	PIX		pixSel2;									// Selected position of maximum y
	PIX		pixAttr = 0;
	for( PIX pixY = 0; pixY < pixAttrMapWidth; pixY++ )
	{
		for( PIX pixX = 0; pixX < pixAttrMapWidth; pixX++, pixAttr++ )
		{
			// Upper left
			fSize = 1.0f;
			pixSel1 = pixSel2 = 0;
			fX = pixX;		fZ = pixY;
			fMinY = fMaxY = fY = TR_GetHeight( ptr, fX, fZ );

			// Upper right
			fX = pixX + 1;	fZ = pixY;
			fY = TR_GetHeight( ptr, fX, fZ );
			if( fMinY > fY )
			{
				fMinY = fY;		pixSel1 = 1;
			}
			if( fMaxY < fY )
			{
				fMaxY = fY;		pixSel2 = 1;
			}

			// Lower left
			fX = pixX;	fZ = pixY + 1;
			fY = TR_GetHeight( ptr, fX, fZ );
			if( fMinY > fY )
			{
				fMinY = fY;		pixSel1 = 2;
			}
			if( fMaxY < fY )
			{
				fMaxY = fY;		pixSel2 = 2;
			}

			// Lower right
			fX = pixX + 1;	fZ = pixY + 1;
			fY = TR_GetHeight( ptr, fX, fZ );
			if( fMinY > fY )
			{
				fMinY = fY;		pixSel1 = 3;
			}
			if( fMaxY < fY )
			{
				fMaxY = fY;		pixSel2 = 3;
			}

			// Same point
			if( pixSel1 == pixSel2 )
				continue;

			// Diagonal
			if( pixSel1 + pixSel2 == 3 )
				fSize = 1.414f;

			// Steep slope
			if( ( fMaxY - fMinY ) / fSize >= fMaxSlope )
				pubAttr[pixAttr] |= MATT_UNWALKABLE;
		}
	}
}

#include <Engine/World/World.h>
#include <Engine/Models/ModelData.h>
#include <Engine/Models/ModelObject.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Templates/DynamicArray.cpp>
extern void TR_GenerateAttributeMap( CTerrain *ptrTerrain, CWorld *pWorld )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;

	const SLONG	slSizeAspect = ptr->tr_slAttributeMapSizeAspect;
	const PIX	pixAttributeMapWidth  = ( ptr->tr_pixHeightMapWidth - 1 ) * slSizeAspect;
	const PIX	pixAttributeMapHeight = ( ptr->tr_pixHeightMapHeight - 1 ) * slSizeAspect;
	const PIX	pixAttributeMapSize = pixAttributeMapWidth * pixAttributeMapHeight;
	UWORD		*pubTemp = ptr->tr_pubAttributeMap;
	UWORD		ubAttr = MATT_UNWALKABLE;		// can't walk

	// Clear the attribute map
	memset(pubTemp, 0x00, pixAttributeMapSize* sizeof(UWORD));

	// Slop attribute ( maximum slope angle is 45 degree )
	TR_DrawSlopAttribute( ptr, pubTemp, pixAttributeMapWidth );

	/*******
	// Each entities...
	FOREACHINDYNAMICCONTAINER( pWorld->wo_cenAllEntities, CEntity, iten )
	{
		CEntity	*penEntity = iten;

		if( penEntity->en_RenderType != CEntity::RT_MODEL && penEntity->en_RenderType != CEntity::RT_SKAMODEL )
			continue;

		// Skip
		if( ( penEntity->en_ulFlags & ENF_BACKGROUND ) ||
			( penEntity->en_ulFlags & ENF_INVISIBLE ) ||
			( penEntity->en_ulCollisionFlags == 0 ) )
			continue;

		FLOAT3D	vMin, vMax, vStretch;
		// If the entity is a mdl model...
		if( penEntity->en_RenderType == CEntity::RT_MODEL )
		{
			CModelObject	*pmo = penEntity->en_pmoModelObject;
			vStretch = pmo->mo_Stretch;
			FOREACHINDYNAMICARRAY( pmo->GetData()->md_acbCollisionBox, CModelCollisionBox, itcb )
			{
				vMin = itcb->mcb_vCollisionBoxMin;
				vMax = itcb->mcb_vCollisionBoxMax;
				break;
			}
		}
		// If the entity is a ska model...
		else
		{
			CModelInstance	*pmi = penEntity->en_pmiModelInstance;
			vStretch = pmi->mi_vStretch;
			vMin = pmi->GetCollisionBoxMin( 0 );
			vMax = pmi->GetCollisionBoxMax( 0 );
		}

		// Get real size of bounding box
		vMin( 1 ) *= vStretch( 1 );
		vMin( 2 ) *= vStretch( 2 );
		vMin( 3 ) *= vStretch( 3 );
		vMax( 1 ) *= vStretch( 1 );
		vMax( 2 ) *= vStretch( 2 );
		vMax( 3 ) *= vStretch( 3 );

		// Translation & rotation
		FLOAT3D			vTranslation = penEntity->en_plPlacement.pl_PositionVector;
		FLOATmatrix3D	matRotation = penEntity->en_mRotation;
		FLOAT3D			vLB = FLOAT3D( vMin( 1 ), vMin( 2 ), vMin( 3 ) );
		FLOAT3D			vLT = FLOAT3D( vMin( 1 ), vMin( 2 ), vMax( 3 ) );
		FLOAT3D			vRT = FLOAT3D( vMax( 1 ), vMin( 2 ), vMax( 3 ) );
		FLOAT3D			vRB = FLOAT3D( vMax( 1 ), vMin( 2 ), vMin( 3 ) );
		vLB = vTranslation + vLB * matRotation;
		vLT = vTranslation + vLT * matRotation;
		vRT = vTranslation + vRT * matRotation;
		vRB = vTranslation + vRB * matRotation;

		// Get points
		POINT	ptLB, ptLT, ptRT, ptRB;
		ptLB.x = vLB( 1 );
		ptLB.y = vLB( 3 );
		ptLT.x = vLT( 1 );
		ptLT.y = vLT( 3 );
		ptRT.x = vRT( 1 );
		ptRT.y = vRT( 3 );
		ptRB.x = vRB( 1 );
		ptRB.y = vRB( 3 );

		// Draw attribute map
		TR_DrawAttributeMap( ptLB, ptLT, ptRT, ptRB, pubTemp, pixAttributeMapWidth, ubAttr );
	}
	***********/
	TR_DrawModelAttribute(pWorld, pubTemp, pixAttributeMapWidth);
}

extern void TR_ExportServerHeightMap_t( CTerrain *ptrTerrain, CTFileName &fnm )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;

	const PIX	pixHeightMapWidth  = ptr->tr_vMetricSize( 1 );
	const PIX	pixHeightMapHeight = ptr->tr_vMetricSize( 3 );
	const PIX	pixHeightMapSize = pixHeightMapWidth * pixHeightMapHeight * sizeof(UWORD);
	UWORD		*puwHeightMap = (UWORD *)AllocMemory( pixHeightMapSize );
	UWORD		uwTempHeight, uwTempHeight2, uwHeight;

	FLOAT	fX, fZ;
	PIX		pixHM = 0;
	for( PIX pixY = 0; pixY < pixHeightMapHeight; pixY++ )
	{
		fZ = pixY;
		for( PIX pixX = 0; pixX < pixHeightMapWidth; pixX++ )
		{
			fX = pixX;
			uwTempHeight = ( TR_GetHeight( ptr, fX, fZ ) + 0.009999f ) * 100.0f;
			uwTempHeight2 = ( TR_GetHeight( ptr, fX + 1.0f, fZ ) + 0.009999f ) * 100.0f;
			if( uwTempHeight < uwTempHeight2 )
				uwTempHeight = uwTempHeight2;
			uwTempHeight2 = ( TR_GetHeight( ptr, fX, fZ + 1.0f ) + 0.009999f ) * 100.0f;
			if( uwTempHeight < uwTempHeight2 )
				uwTempHeight = uwTempHeight2;
			uwTempHeight2 = ( TR_GetHeight( ptr, fX + 1.0f, fZ + 1.0f ) + 0.009999f ) * 100.0f;
			if( uwTempHeight < uwTempHeight2 )
				uwTempHeight = uwTempHeight2;
			uwTempHeight2 = ( TR_GetHeight( ptr, fX + 0.5f, fZ + 0.5f ) + 0.009999f ) * 100.0f;
			if( uwTempHeight < uwTempHeight2 )
				uwTempHeight = uwTempHeight2;
			uwHeight = ( ( uwTempHeight & 0xFF00 ) >> 8 ) | ( ( uwTempHeight & 0x00FF ) << 8 );
			puwHeightMap[pixHM++] = uwHeight;
		}
	}

	CTFileName	fnmSvrHeightMap;
	if( fnm.FindSubstr( CTString( "_" ) ) == -1 )
	{
		CTString	strSvrHMInfo;
		strSvrHMInfo.PrintF( "_%d_%d_0", UINT(ptr->tr_vMetricSize( 1 )), UINT(ptr->tr_vMetricSize( 3 )) );
		fnmSvrHeightMap = fnm.NoExt() + strSvrHMInfo + CTString( ".sht" );
	}
	else
		fnmSvrHeightMap = fnm;

	CTFileStream	strmFile;
	strmFile.Create_t( fnmSvrHeightMap, CTStream::CM_BINARY );
	strmFile.Write_t( puwHeightMap, pixHeightMapSize );
	strmFile.Close();
}

void TR_DrawModelAttribute( CWorld *pWorld, UWORD* pbMap, PIX pixAttributeMapWidth )
{
	UWORD ubAttr = MATT_UNWALKABLE;		// can't walk

	// Each entities...
	FOREACHINDYNAMICCONTAINER( pWorld->wo_cenAllEntities, CEntity, iten )
	{
		CEntity	*penEntity = iten;

		if( penEntity->en_RenderType != CEntity::RT_MODEL && penEntity->en_RenderType != CEntity::RT_SKAMODEL )
			continue;

		// Skip
		if( ( penEntity->en_ulFlags & ENF_BACKGROUND ) ||
			( penEntity->en_ulFlags & ENF_INVISIBLE ) ||
			( penEntity->en_ulCollisionFlags == 0 ) )
			continue;

		FLOAT3D	vMin, vMax, vStretch;
		// If the entity is a mdl model...
		if( penEntity->en_RenderType == CEntity::RT_MODEL )
		{
			CModelObject	*pmo = penEntity->en_pmoModelObject;
			vStretch = pmo->mo_Stretch;
			FOREACHINDYNAMICARRAY( pmo->GetData()->md_acbCollisionBox, CModelCollisionBox, itcb )
			{
				vMin = itcb->mcb_vCollisionBoxMin;
				vMax = itcb->mcb_vCollisionBoxMax;
				break;
			}
		}
		// If the entity is a ska model...
		else
		{
			CModelInstance	*pmi = penEntity->en_pmiModelInstance;
			vStretch = pmi->mi_vStretch;
			vMin = pmi->GetCollisionBoxMin( 0 );
			vMax = pmi->GetCollisionBoxMax( 0 );
		}

		// Get real size of bounding box
		vMin( 1 ) *= vStretch( 1 );
		vMin( 2 ) *= vStretch( 2 );
		vMin( 3 ) *= vStretch( 3 );
		vMax( 1 ) *= vStretch( 1 );
		vMax( 2 ) *= vStretch( 2 );
		vMax( 3 ) *= vStretch( 3 );

		// Translation & rotation
		FLOAT3D			vTranslation = penEntity->en_plPlacement.pl_PositionVector;
		FLOATmatrix3D	matRotation = penEntity->en_mRotation;
		FLOAT3D			vLB = FLOAT3D( vMin( 1 ), vMin( 2 ), vMin( 3 ) );
		FLOAT3D			vLT = FLOAT3D( vMin( 1 ), vMin( 2 ), vMax( 3 ) );
		FLOAT3D			vRT = FLOAT3D( vMax( 1 ), vMin( 2 ), vMax( 3 ) );
		FLOAT3D			vRB = FLOAT3D( vMax( 1 ), vMin( 2 ), vMin( 3 ) );
		vLB = vTranslation + vLB * matRotation;
		vLT = vTranslation + vLT * matRotation;
		vRT = vTranslation + vRT * matRotation;
		vRB = vTranslation + vRB * matRotation;

		// Get points
		POINT	ptLB, ptLT, ptRT, ptRB;
		ptLB.x = vLB( 1 );
		ptLB.y = vLB( 3 );
		ptLT.x = vLT( 1 );
		ptLT.y = vLT( 3 );
		ptRT.x = vRT( 1 );
		ptRT.y = vRT( 3 );
		ptRB.x = vRB( 1 );
		ptRB.y = vRB( 3 );

		// Draw attribute map
		TR_DrawAttributeMap( ptLB, ptLT, ptRT, ptRB, pbMap, pixAttributeMapWidth, ubAttr );
	}

}



// yjpark     -->|

#endif	// FINALVERSION
// yjpark     -->|