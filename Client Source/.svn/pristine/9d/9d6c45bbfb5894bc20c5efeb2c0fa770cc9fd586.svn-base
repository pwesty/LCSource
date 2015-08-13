#include "stdh.h"
#include <Engine/Math/Functions.h>
#include <Engine/Base/Rect.h>
#include <Engine/Base/CTString.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/TRLayer.h>
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Terrain/TREditing.hpp>
#include <Engine/Terrain/TRGenerate.hpp>
#include <Engine/Terrain/TRLayer.hpp>

static INDEX _ctBuffersAllocated = 0;

// Terrain selection data
extern INDEX           _iSelFrameNumber    = 1;        // Time of selection
extern BOOL            _bShowTerrainSel    = FALSE;    // Show selection
extern const CTerrain *_ptrSelTerrain      = NULL;
extern CTextureData   *_ptrSelBrush        = NULL;
extern COLOR           _colSelColor        = 0x00000000;
extern CTRect          _rcSelRectangle     = CTRect();
extern FLOAT           _fSelStrength       = 0.0f;
extern SelectionFill   _slSelFill          = SF_POLYGON;
extern BufferType      _btSelBufferType    = BT_HEIGHT_MAP;

static void UpdateEditedTerrainTiles(CTerrainImp *ptr, CTRect &rcUpdate, ULONG ulForcedFlags)
{
	const INDEX cttt=ptr->tr_attTiles.Count();
	for(INDEX itt=0;itt<cttt;itt++)
	{
		const CTerrainTile &ttTile = ptr->tr_attTiles[itt];
		if(RectTouchesRect(ttTile.tt_rcSource,rcUpdate))
		{
			QueueTileRegeneration(ptr,itt,ulForcedFlags);
		}
	}
}

extern void TRE_ShowSelection(const CTerrain *ptrTerrain, CTRect &rcSelection, CTextureData *ptdBrush,
								COLOR colSelection, FLOAT fStrength, SelectionFill sfFill, BufferType btType)
{
	_ptrSelTerrain   = ptrTerrain;
	_rcSelRectangle  = rcSelection;
	_ptrSelBrush     = ptdBrush;
	_colSelColor     = colSelection;
	_fSelStrength    = fStrength;
	_slSelFill       = sfFill;
	_btSelBufferType = btType;
	_bShowTerrainSel = TRUE;
	_iSelFrameNumber = _pGfx->gl_iFrameNumber;
}

// yjpark |<--
//-------------------------------------------------------------------------------------------------
// Desc : These contents aren't included in final release version
// ------------------------------------------------------------------------------------------------
#ifndef FINALVERSION

extern UWORD *TRE_GetBufferForEditing(CTerrain *ptrTerrain, CTRect rcRect, BufferType btType, INDEX iBufferData/*=-1*/)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const PIX pixLeft   = rcRect.rc_slLeft;
	const PIX pixRight  = rcRect.rc_slRight;
	const PIX pixTop    = rcRect.rc_slTop;
	const PIX pixBottom = rcRect.rc_slBottom;
	const PIX pixWidth  = rcRect.GetWidth();
	const PIX pixHeight = rcRect.GetHeight();

	const PIX pixHeightMapWidth  = ptr->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX pixEdgeMapWidth    = ptr->tr_pixHeightMapWidth-1;
	const PIX pixEdgeMapHeight   = ptr->tr_pixHeightMapHeight-1;

	// Allocate memory for buffer
	const SLONG slMemRequired = pixWidth*pixHeight*sizeof(UWORD);
	UWORD *puwBuffer = (UWORD*)AllocMemory(slMemRequired);
	UWORD *puwBufferData = &puwBuffer[0];

	if(btType==BT_HEIGHT_MAP)
	{
		const UWORD *puwHeightMap = &ptr->tr_puwHeightMap[0];
		for(PIX pixZ=0;pixZ<pixHeight;pixZ++)
		{
			const PIX pixRealZ = pixZ+pixTop;
			const PIX pixClampZ = Clamp(pixRealZ,0L,pixHeightMapHeight-1L);
			for(PIX pixX=0;pixX<pixWidth;pixX++)
			{
				const PIX pixRealX = pixX+pixLeft;
				const PIX pixClampX = Clamp(pixRealX,0L,pixHeightMapWidth-1L);
				const PIX pixPixel = pixClampX + pixClampZ*pixHeightMapWidth;
				// Copy pixel from height map to editing buffer
				const UWORD *puwHeight = &puwHeightMap[pixPixel];
				*puwBufferData = *puwHeight;
				puwBufferData++;
			}
		}
	}
	else if(btType==BT_LAYER_MASK)
	{
		const CTerrainLayer &tlLayer = ptr->tr_atlLayers[iBufferData];
		const CTerrainLayerImp *ptrl = tlLayer.trl_trlLayerImp;
		const UBYTE *pubLayerMask = ptrl->trl_pubLayerMask;
		const PIX pixLayerMaskWidth  = ( ptr->tr_pixHeightMapWidth - 1 ) << tlLayer.tl_slMaskStretch;		// yjpark
		const PIX pixLayerMaskHeight = ( ptr->tr_pixHeightMapHeight - 1 ) << tlLayer.tl_slMaskStretch;		// yjpark
		for(PIX pixZ=0;pixZ<pixHeight;pixZ++)
		{
			const PIX pixRealZ = pixZ+pixTop;
			const PIX pixClampZ = Clamp(pixRealZ,0L,pixLayerMaskHeight-1L);
			for(PIX pixX=0;pixX<pixWidth;pixX++)
			{
				const PIX pixRealX = pixX+pixLeft;
				const PIX pixClampX = Clamp(pixRealX,0L,pixLayerMaskWidth-1L);
				const PIX pixPixel = pixClampX + pixClampZ*pixLayerMaskHeight;
				// Copy pixel from layer mask to editing buffer
				const UBYTE *pubLayerValue = &pubLayerMask[pixPixel];
				*puwBufferData = (*pubLayerValue)<<8|(*pubLayerValue);
				puwBufferData++;
			}
		}
	}
	else if(btType==BT_EDGE_MAP)
	{
		const UBYTE *pubEdgeMask = ptr->tr_pubEdgeMap;
		for(PIX pixZ=0;pixZ<pixHeight;pixZ++)
		{
			const PIX pixRealZ = pixZ+pixTop;
			const PIX pixClampZ = Clamp(pixRealZ,0L,pixEdgeMapHeight-1L);
			for(PIX pixX=0;pixX<pixWidth;pixX++)
			{
				const PIX pixRealX = pixX+pixLeft;
				const PIX pixClampX = Clamp(pixRealX,0L,pixEdgeMapWidth-1L);
				const PIX pixPixel = pixClampX + pixClampZ*pixEdgeMapHeight;
				// Copy pixel from height map to editing buffer
				const UBYTE *pubEdgeValue = &pubEdgeMask[pixPixel];
				if((*pubEdgeValue)==255)
				{
					*puwBufferData = 1;
				}
				else
				{
					*puwBufferData = 0;
				}
				puwBufferData++;
			}
		}
	}
	else if( btType==BT_ATTRIBUTE_MAP)
	{

	}
	else
	{
		ASSERTALWAYS("Ilegal buffer type");
		return NULL;
	}

	_ctBuffersAllocated++;
	return puwBuffer;
}

// yjpark |<--
extern ULONG* TRE_Get4ByteBufferForEditing( CTerrain* ptrTerrain, CTRect rcRect, BufferType btType, INDEX iBufferData )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp*	ptr = ptrTerrain->tr_ptrTerrain;

	const PIX	pixLeft = rcRect.rc_slLeft;
	const PIX	pixRight = rcRect.rc_slRight;
	const PIX	pixTop = rcRect.rc_slTop;
	const PIX	pixBottom = rcRect.rc_slBottom;
	const PIX	pixWidth = rcRect.GetWidth();
	const PIX	pixHeight = rcRect.GetHeight();

	// Allocate memory for buffer
	const SLONG	slMemRequired = pixWidth * pixHeight * sizeof(ULONG);
	ULONG*	pulBuffer = (ULONG*)AllocMemory( slMemRequired );
	ULONG*	pulBufferData = &pulBuffer[0];

	const PIX	pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	PIX			pixBufferWidth;
	PIX			pixBufferHeight;
	ULONG		*pulFrames;
	if( btType == BT_SHADOW_MAP )
	{
		pixBufferWidth = GetShadowMapWidth( pixHeightMapWidth, ptr->tr_slShadowMapSizeAspect );
		pixBufferHeight = GetShadowMapHeight( pixHeightMapHeight, ptr->tr_slShadowMapSizeAspect );
		pulFrames = ptr->tr_aptdShadowMaps[iBufferData]->td_pulFrames;
	}
	else
		return NULL;

	for( PIX pixZ = 0; pixZ < pixHeight; pixZ++ )
	{
		const PIX	pixRealZ = pixZ + pixTop;
		const PIX	pixClampZ = Clamp( pixRealZ, 0L, pixBufferHeight );
		for( PIX pixX = 0; pixX < pixWidth; pixX++ )
		{
			const PIX	pixRealX = pixX + pixLeft;
			const PIX	pixClampX = Clamp( pixRealX, 0L, pixBufferWidth );
			const PIX	pixPixel = pixClampX + pixClampZ * pixBufferWidth;
			*pulBufferData = pulFrames[pixPixel];
			pulBufferData++;
		}
	}

	_ctBuffersAllocated++;
	return pulBuffer;
}

extern UBYTE* TRE_Get1ByteBufferForEditing( CTerrain* ptrTerrain, CTRect rcRect, BufferType btType, INDEX iBufferData )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp*	ptr = ptrTerrain->tr_ptrTerrain;

	const PIX	pixLeft = rcRect.rc_slLeft;
	const PIX	pixRight = rcRect.rc_slRight;
	const PIX	pixTop = rcRect.rc_slTop;
	const PIX	pixBottom = rcRect.rc_slBottom;
	const PIX	pixWidth = rcRect.GetWidth();
	const PIX	pixHeight = rcRect.GetHeight();

	// Allocate memory for buffer
	const SLONG	slMemRequired = pixWidth * pixHeight * sizeof(UWORD);
	UBYTE*	pubBuffer = (UBYTE*)AllocMemory( slMemRequired );
	UBYTE*	pubBufferData = &pubBuffer[0];

	const PIX	pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	PIX			pixBufferWidth;
	PIX			pixBufferHeight;
	UWORD		*pubFrames;
	if( btType == BT_ATTRIBUTE_MAP )
	{
		const SLONG	slSizeAspect = ptr->tr_slAttributeMapSizeAspect;
		pixBufferWidth = ( pixHeightMapWidth - 1 ) * slSizeAspect;
		pixBufferHeight = ( pixHeightMapHeight - 1 ) * slSizeAspect;
		pubFrames = ptr->tr_pubAttributeMap;
	}
	else
		return NULL;

	for( PIX pixZ = 0; pixZ < pixHeight; pixZ++ )
	{
		const PIX	pixRealZ = pixZ + pixTop;
		const PIX	pixClampZ = Clamp( pixRealZ, 0L, pixBufferHeight );
		for( PIX pixX = 0; pixX < pixWidth; pixX++ )
		{
			const PIX	pixRealX = pixX + pixLeft;
			const PIX	pixClampX = Clamp( pixRealX, 0L, pixBufferWidth );
			const PIX	pixPixel = pixClampX + pixClampZ * pixBufferWidth;
			*pubBufferData = pubFrames[pixPixel];
			pubBufferData++;
		}
	}

	_ctBuffersAllocated++;
	return pubBuffer;
}
// yjpark     -->|

extern void TRE_SetBufferForEditing(CTerrain *ptrTerrain, UWORD *puwBuffer, CTRect rcRect, BufferType btType, INDEX iBufferData/*=-1*/)
{
	ASSERT(puwBuffer!=NULL);
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const PIX pixLeft   = rcRect.rc_slLeft;
	const PIX pixRight  = rcRect.rc_slRight;
	const PIX pixTop    = rcRect.rc_slTop;
	const PIX pixBottom = rcRect.rc_slBottom;
	const PIX pixWidth  = rcRect.GetWidth();
	const PIX pixHeight = rcRect.GetHeight();

	const PIX pixHeightMapWidth  = ptr->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX pixEdgeMapWidth    = ptr->tr_pixHeightMapWidth-1;
	const PIX pixEdgeMapHeight   = ptr->tr_pixHeightMapHeight-1;

	// Validate buffer memory
	const SLONG slMemRequired = pixWidth*pixHeight*sizeof(UWORD);
	ASSERT(MEM_Size(puwBuffer) == slMemRequired);
	UWORD *puwBufferData = &puwBuffer[0];

	if(btType==BT_HEIGHT_MAP)
	{
		UWORD *puwHeightMap = &ptr->tr_puwHeightMap[0];
		for(PIX pixZ=0;pixZ<pixHeight;pixZ++)
		{
			const PIX pixRealZ = pixZ+pixTop;
			if(pixRealZ>=0 && pixRealZ<pixHeightMapHeight)
			{
				for(PIX pixX=0;pixX<pixWidth;pixX++)
				{
					const PIX pixRealX = pixX+pixLeft;
					if(pixRealX>=0 && pixRealX<pixHeightMapWidth)
					{
						// Copy pixel from height map to editing buffer
						const PIX pixPixel = pixRealX + pixRealZ*pixHeightMapWidth;
						UWORD *puwHeight = &puwHeightMap[pixPixel];
						ASSERT(puwBufferData>=puwBuffer && puwBufferData<puwBuffer+slMemRequired);
						*puwHeight = *puwBufferData;
					}
					puwBufferData++;
				}
			}
			else
			{
				puwBufferData+=pixWidth;
			}
		}
		UpdateEditedTerrainTiles(ptr,rcRect,RTF_GEOMETRY|RTF_QUADNODE);
	// if this is mask editing
	}
	else if(btType==BT_LAYER_MASK)
	{
		const CTerrainLayer &tlLayer = ptr->tr_atlLayers[iBufferData];
		const CTerrainLayerImp *ptrl = tlLayer.trl_trlLayerImp;
		UBYTE *pubLayerMask = ptrl->trl_pubLayerMask;
		const PIX pixLayerMaskWidth = ( ptr->tr_pixHeightMapWidth - 1 ) << tlLayer.tl_slMaskStretch;		// yjpark
		const PIX pixLayerMaskHeight = ( ptr->tr_pixHeightMapHeight - 1 ) << tlLayer.tl_slMaskStretch;		// yjpark
		for(PIX pixZ=0;pixZ<pixHeight;pixZ++)
		{
			const PIX pixRealZ = pixZ+pixTop;
			if(pixRealZ>=0 && pixRealZ<pixLayerMaskHeight)
			{
				for(PIX pixX=0;pixX<pixWidth;pixX++)
				{
					const PIX pixRealX = pixX+pixLeft;
					if(pixRealX>=0 && pixRealX<pixLayerMaskWidth)
					{
						// Copy pixel from buffer to layer mask
						const PIX pixPixel = pixRealX + pixRealZ*pixLayerMaskWidth;
						UBYTE *pubLayerVaule = &pubLayerMask[pixPixel];
						*pubLayerVaule = (*puwBufferData)>>8;
					}
					puwBufferData++;
				}
			}
			else
			{
				puwBufferData+=pixWidth;
			}
		}
		// Upload layer mask
		TR_UploadLayerMask(ptrTerrain, iBufferData);
		// Update Top map
		TR_UpdateTopMap(ptrTerrain);
	}
	else if(btType==BT_EDGE_MAP)
	{
		UBYTE *pubEdgeMask = ptr->tr_pubEdgeMap;
		for(PIX pixZ=0;pixZ<pixHeight;pixZ++)
		{
			const PIX pixRealZ = pixZ+pixTop;
			if(pixRealZ>=0 && pixRealZ<pixEdgeMapHeight)
			{
				for(PIX pixX=0;pixX<pixWidth;pixX++)
				{
					const PIX pixRealX = pixX+pixLeft;
					if(pixRealX>=0 && pixRealX<pixEdgeMapWidth)
					{
						// Copy pixel from buffer to layer mask
						const PIX pixPixel = pixRealX + pixRealZ*pixEdgeMapWidth;
						UBYTE *pubEdgeValue = &pubEdgeMask[pixPixel];
						if(*puwBufferData>=1)
						{
							*pubEdgeValue = 255;
						}
						else
						{
							*pubEdgeValue = 0;
						}
					}
					puwBufferData++;
				}
			}
			else
			{
				puwBufferData+=pixWidth;
			}
		}
		// Upload edge map
		TR_UploadEdgeMap(ptrTerrain);
		// Update Top map
		TR_UpdateTopMap(ptrTerrain);
	}
	else
	{
		ASSERTALWAYS("Ilegal buffer type");
		return;
	}
}

// yjpark |<--
extern void TRE_Set4ByteBufferForEditing( CTerrain* ptrTerrain, ULONG* pulBuffer, CTRect rcRect, BufferType btType, INDEX iBufferData )
{
	ASSERT( pulBuffer != NULL );
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp*	ptr = ptrTerrain->tr_ptrTerrain;

	const PIX	pixLeft   = rcRect.rc_slLeft;
	const PIX	pixRight  = rcRect.rc_slRight;
	const PIX	pixTop    = rcRect.rc_slTop;
	const PIX	pixBottom = rcRect.rc_slBottom;
	const PIX	pixWidth  = rcRect.GetWidth();
	const PIX	pixHeight = rcRect.GetHeight();

	// Validate buffer memory
	const SLONG	slMemRequired = pixWidth * pixHeight * sizeof(ULONG);
	ASSERT( MEM_Size( pulBuffer ) == slMemRequired );
	const ULONG*	pulBufferData = &pulBuffer[0];

	const PIX	pixHeightMapWidth  = ptr->tr_pixHeightMapWidth;
	const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	PIX			pixBufferWidth;
	PIX			pixBufferHeight;
	ULONG		*pulFrames;
	if( btType == BT_SHADOW_MAP )
	{
		pixBufferWidth = GetShadowMapWidth( pixHeightMapWidth, ptr->tr_slShadowMapSizeAspect );
		pixBufferHeight = GetShadowMapHeight( pixHeightMapHeight, ptr->tr_slShadowMapSizeAspect );
		pulFrames = ptr->tr_aptdShadowMaps[iBufferData]->td_pulFrames;
	}
	else
		return;
	
	for( PIX pixZ = 0; pixZ < pixHeight; pixZ++ )
	{
		const PIX	pixRealZ = pixZ + pixTop;
		if( pixRealZ >= 0 && pixRealZ < pixBufferHeight )
		{
			for( PIX pixX = 0; pixX < pixWidth; pixX++ )
			{
				const PIX	pixRealX = pixX + pixLeft;
				if( pixRealX >= 0 && pixRealX < pixBufferWidth )
				{					
					const PIX	pixPixel = pixRealX + pixRealZ * pixBufferWidth;
					pulFrames[pixPixel] = *pulBufferData;
				}
				pulBufferData++;
			}
		}
		else
		{
			pulBufferData += pixWidth;
		}
	}

	if( btType == BT_SHADOW_MAP )
		TR_UploadTexture( *(ptr->tr_aptdShadowMaps[iBufferData]), pixBufferWidth, pixBufferHeight, PTF_STATIC );
}

extern void TRE_Set1ByteBufferForEditing( CTerrain* ptrTerrain, UBYTE* pubBuffer, CTRect rcRect, BufferType btType, INDEX iBufferData )
{
	ASSERT( pubBuffer != NULL );
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp*	ptr = ptrTerrain->tr_ptrTerrain;

	const PIX	pixLeft   = rcRect.rc_slLeft;
	const PIX	pixRight  = rcRect.rc_slRight;
	const PIX	pixTop    = rcRect.rc_slTop;
	const PIX	pixBottom = rcRect.rc_slBottom;
	const PIX	pixWidth  = rcRect.GetWidth();
	const PIX	pixHeight = rcRect.GetHeight();

	// Validate buffer memory
	const SLONG	slMemRequired = pixWidth * pixHeight * sizeof(UWORD);
	ASSERT( MEM_Size( pubBuffer ) == slMemRequired );
	const UBYTE*	pubBufferData = &pubBuffer[0];

	const PIX	pixHeightMapWidth  = ptr->tr_pixHeightMapWidth;
	const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	PIX			pixBufferWidth;
	PIX			pixBufferHeight;
	UWORD		*pubFrames;
	if( btType == BT_ATTRIBUTE_MAP )
	{
		const SLONG	slSizeAspect = ptr->tr_slAttributeMapSizeAspect;
		pixBufferWidth = ( pixHeightMapWidth - 1 ) * slSizeAspect;
		pixBufferHeight = ( pixHeightMapHeight - 1 ) * slSizeAspect;
		pubFrames = ptr->tr_pubAttributeMap;
	}
	else
		return ;
	
	for( PIX pixZ = 0; pixZ < pixHeight; pixZ++ )
	{
		const PIX	pixRealZ = pixZ + pixTop;
		if( pixRealZ >= 0 && pixRealZ < pixBufferHeight )
		{
			for( PIX pixX = 0; pixX < pixWidth; pixX++ )
			{
				const PIX	pixRealX = pixX + pixLeft;
				if( pixRealX >= 0 && pixRealX < pixBufferWidth )
				{					
					const PIX	pixPixel = pixRealX + pixRealZ * pixBufferWidth;
					pubFrames[pixPixel] = *pubBufferData;
				}
				pubBufferData++;
			}
		}
		else
		{
			pubBufferData += pixWidth;
		}
	}
}
// yjpark     -->|

extern void TRE_FreeBufferForEditing(UWORD *puwBuffer)
{
	// Free buffer 
	_ctBuffersAllocated--;
	FreeMemory(puwBuffer);
}

// yjpark |<--
extern void TRE_Free4ByteBufferForEditing( ULONG* pulBuffer )
{
	// Free buffer 
	_ctBuffersAllocated--;
	FreeMemory( pulBuffer );
}

extern void TRE_Free1ByteBufferForEditing( UBYTE* pubBuffer )
{
	// Free buffer 
	_ctBuffersAllocated--;
	FreeMemory( pubBuffer );
}
// yjpark     -->|

#endif	// FINALVERSION
// yjpark     -->|