#include "stdh.h"
#include <Engine/Graphics/Texture.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Terrain/TRLayer.h>
#include <Engine/Terrain/TRLayer.hpp>
#include <Engine/Terrain/TRGenerate.hpp>

extern BOOL _bFullLayerCopy = FALSE;

CTerrainLayer::CTerrainLayer(void)
{
	// Create internal layer object
	trl_trlLayerImp = new CTerrainLayerImp;
	// Create layer thumbnail
	trl_ptdThumbnail = NULL;
	extern BOOL	_bWorldEditorApp;
	if( _bWorldEditorApp )
		trl_ptdThumbnail = TR_CreateTexture();

	// Layer data
	tl_bVisible      = TRUE;
	tl_ltType        = LT_NORMAL;
	tl_colMultiply   = 0xFFFFFFFF;
	tl_ulFlags       = 0;
	tl_slMaskStretch = 0;		// yjpark - default : no stretch
	tl_iSoundIndex = 0;			// yjpark

	// Layer transformation
	tl_fOffsetX = 0.0f;
	tl_fOffsetY = 0.0f;
	tl_fRotateX = 0.0f;
	tl_fRotateY = 0.0f;
	tl_fStretchX = 1.0f;
	tl_fStretchY = 1.0f;

	// Layer distribution
	tl_bAutoRegenerated=FALSE;
	tl_fCoverage=1.0f;
	tl_fCoverageNoise=0.5f;
	tl_fCoverageRandom=0.0f;

	tl_bApplyMinAltitude=TRUE;
	tl_fMinAltitude=0.0f;
	tl_fMinAltitudeFade=0.25f;
	tl_fMinAltitudeNoise=0.25f;
	tl_fMinAltitudeRandom=0;

	tl_bApplyMaxAltitude=TRUE;
	tl_fMaxAltitude=1.0f;
	tl_fMaxAltitudeFade=0.25f;
	tl_fMaxAltitudeNoise=0.25f;
	tl_fMaxAltitudeRandom=0;

	tl_bApplyMinSlope=TRUE;
	tl_fMinSlope=0.0f;
	tl_fMinSlopeFade=0.25f;
	tl_fMinSlopeNoise=0.25f;
	tl_fMinSlopeRandom=0;

	tl_bApplyMaxSlope=TRUE;
	tl_fMaxSlope=1.0f;
	tl_fMaxSlopeFade=0.25f;
	tl_fMaxSlopeNoise=0.25f;
	tl_fMaxSlopeRandom=0;
}


extern void TRL_CopyLayer(CTerrain *ptrTerrain, INDEX iTerrainLayer, const CTerrainLayer &tlSrc)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	ASSERT(tlSrc.trl_trlLayerImp!=NULL);

	CTerrainImp *ptrDst = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer *ptlDst = &ptrDst->tr_atlLayers[iTerrainLayer];
	CTerrainLayerImp *ptrl = ptlDst->trl_trlLayerImp;

	TRL_CopyLayerImp(ptrTerrain, iTerrainLayer, *tlSrc.trl_trlLayerImp);

	// Layer data
	ptlDst->tl_strName            = tlSrc.tl_strName;
	ptlDst->tl_bVisible           = tlSrc.tl_bVisible;
	ptlDst->tl_ltType             = tlSrc.tl_ltType;
	ptlDst->tl_colMultiply        = tlSrc.tl_colMultiply;
	ptlDst->tl_ulFlags            = tlSrc.tl_ulFlags;
	ptlDst->tl_ulFlags            = tlSrc.tl_ulFlags;

	// Layer transformation
	ptlDst->tl_fOffsetX           = tlSrc.tl_fOffsetX;
	ptlDst->tl_fOffsetY           = tlSrc.tl_fOffsetY;
	ptlDst->tl_fRotateX           = tlSrc.tl_fRotateX;
	ptlDst->tl_fRotateY           = tlSrc.tl_fRotateY;
	ptlDst->tl_fStretchX          = tlSrc.tl_fStretchX;
	ptlDst->tl_fStretchY          = tlSrc.tl_fStretchY;				// yjpark
	ptlDst->tl_slMaskStretch      = tlSrc.tl_slMaskStretch;			// yjpark
	ptlDst->tl_iSoundIndex        = tlSrc.tl_iSoundIndex;			// yjpark

	// Layer distribution
	ptlDst->tl_bAutoRegenerated   = tlSrc.tl_bAutoRegenerated;
	ptlDst->tl_fCoverage          = tlSrc.tl_fCoverage;
	ptlDst->tl_fCoverageNoise     = tlSrc.tl_fCoverageNoise;
	ptlDst->tl_fCoverageRandom    = tlSrc.tl_fCoverageRandom;
	ptlDst->tl_bApplyMinAltitude  = tlSrc.tl_bApplyMinAltitude;
	ptlDst->tl_bApplyMaxAltitude  = tlSrc.tl_bApplyMaxAltitude;
	ptlDst->tl_fMinAltitude       = tlSrc.tl_fMinAltitude;
	ptlDst->tl_fMaxAltitude       = tlSrc.tl_fMaxAltitude;
	ptlDst->tl_fMinAltitudeFade   = tlSrc.tl_fMinAltitudeFade;
	ptlDst->tl_fMaxAltitudeFade   = tlSrc.tl_fMaxAltitudeFade;
	ptlDst->tl_fMinAltitudeNoise  = tlSrc.tl_fMinAltitudeNoise;
	ptlDst->tl_fMaxAltitudeNoise  = tlSrc.tl_fMaxAltitudeNoise;
	ptlDst->tl_fMinAltitudeRandom = tlSrc.tl_fMinAltitudeRandom;
	ptlDst->tl_fMaxAltitudeRandom = tlSrc.tl_fMaxAltitudeRandom;
	ptlDst->tl_bApplyMinSlope     = tlSrc.tl_bApplyMinSlope;
	ptlDst->tl_bApplyMaxSlope     = tlSrc.tl_bApplyMaxSlope;
	ptlDst->tl_fMinSlope          = tlSrc.tl_fMinSlope;
	ptlDst->tl_fMaxSlope          = tlSrc.tl_fMaxSlope;
	ptlDst->tl_fMinSlopeFade      = tlSrc.tl_fMinSlopeFade;
	ptlDst->tl_fMaxSlopeFade      = tlSrc.tl_fMaxSlopeFade;
	ptlDst->tl_fMinSlopeNoise     = tlSrc.tl_fMinSlopeNoise;
	ptlDst->tl_fMaxSlopeNoise     = tlSrc.tl_fMaxSlopeNoise;
	ptlDst->tl_fMinSlopeRandom    = tlSrc.tl_fMinSlopeRandom;
	ptlDst->tl_fMaxSlopeRandom    = tlSrc.tl_fMaxSlopeRandom;

	extern BOOL	_bWorldEditorApp;		// yjpark |<--
	if( _bWorldEditorApp )
	{
		// if doing quick copy
		if( !_bFullLayerCopy )
		{
			// Release current layer thumbnail if it exists
			if( ptlDst->trl_ptdThumbnail != NULL )
			{
			  TR_MarkTextureUnused(ptlDst->trl_ptdThumbnail);
			  ptlDst->trl_ptdThumbnail = NULL;
			}

			// if other layer has valid thumbnail
			if( tlSrc.trl_ptdThumbnail != NULL )
			{
				// 'Copy' layer thumbnail
				ptlDst->trl_ptdThumbnail = tlSrc.trl_ptdThumbnail;
				TR_MarkTextureUsed( ptlDst->trl_ptdThumbnail );
			}
			else
			{
				ptlDst->trl_ptdThumbnail = TR_CreateTexture();
			}
		}
		// Else doing a full copy
		else
		{
			// Release current layer thumbnail if it exists
			if( ptlDst->trl_ptdThumbnail != NULL )
			{
				TR_MarkTextureUnused( ptlDst->trl_ptdThumbnail );
				ptlDst->trl_ptdThumbnail = NULL;
			}
			// Create empty thumbnail texture
			ptlDst->trl_ptdThumbnail = TR_CreateTexture();
		}
	}									// yjpark     -->|
}

CTerrainLayer &CTerrainLayer::operator=(const CTerrainLayer &tlOther)
{
  ASSERTALWAYS("Forbiden");
  return (*this);
}

CTerrainLayer::~CTerrainLayer(void)
{
	// Delete layer thumbnail
	extern BOOL	_bWorldEditorApp;
	if( _bWorldEditorApp )
		TR_MarkTextureUnused(trl_ptdThumbnail);

	// Delete internal layer object
	if( trl_trlLayerImp != NULL )
	{
		delete trl_trlLayerImp;
		trl_trlLayerImp = NULL;
	}
}
