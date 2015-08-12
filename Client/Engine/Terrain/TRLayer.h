#ifndef SE_INCL_TERRAIN_LAYER_H
#define SE_INCL_TERRAIN_LAYER_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

// Not used
enum LayerType {
  LT_NORMAL = 0,
  LT_TILE,
};

// Not used
#define TL_FLIPX_SHIFT   4
#define TL_FLIPY_SHIFT   5
#define TL_SWAPXY_SHIFT  6
#define TL_VISIBLE_SHIFT 7

#define TL_TILE_INDEX 0x0F
#define TL_VISIBLE    (1<<TL_VISIBLE_SHIFT)
#define TL_FLIPX      (1<<TL_FLIPX_SHIFT)
#define TL_FLIPY      (1<<TL_FLIPY_SHIFT)
#define TL_SWAPXY     (1<<TL_SWAPXY_SHIFT)

#define TLF_BLANK_MASK (1UL<<0) // no layer triangles visible
#define TLF_FULL_MASK  (1UL<<1) // all layer triangles are visible

class ENGINE_API CTerrainLayer
{
public:
  CTerrainLayer(void);
  ~CTerrainLayer(void);
  CTerrainLayer &operator=(const CTerrainLayer &trlOther);
public:
  CTerrainLayerImp *trl_trlLayerImp; // Internal terrain layer
  CTextureData     *trl_ptdThumbnail;

  // Layer data
  CTString  tl_strName;       // not used
  BOOL      tl_bVisible;
  LayerType tl_ltType;        // not used
  COLOR     tl_colMultiply;   // not used
  ULONG     tl_ulFlags;
  SLONG		tl_slMaskStretch;		// yjpark - for the extension of the layer mask
  INDEX		tl_iSoundIndex;			// yjpark - for terrain sound

  // Layer transformation
  FLOAT     tl_fOffsetX;
  FLOAT     tl_fOffsetY;
  FLOAT     tl_fRotateX;
  FLOAT     tl_fRotateY;
  FLOAT     tl_fStretchX;
  FLOAT     tl_fStretchY;

  // Layer distribution
  BOOL      tl_bAutoRegenerated;
  FLOAT     tl_fCoverage;
  FLOAT     tl_fCoverageNoise;
  FLOAT     tl_fCoverageRandom;
  BOOL      tl_bApplyMinAltitude;
  BOOL      tl_bApplyMaxAltitude;
  FLOAT     tl_fMinAltitude;
  FLOAT     tl_fMaxAltitude;
  FLOAT     tl_fMinAltitudeFade;
  FLOAT     tl_fMaxAltitudeFade;
  FLOAT     tl_fMinAltitudeNoise;
  FLOAT     tl_fMaxAltitudeNoise;
  FLOAT     tl_fMinAltitudeRandom;
  FLOAT     tl_fMaxAltitudeRandom;
  BOOL      tl_bApplyMinSlope;
  BOOL      tl_bApplyMaxSlope;
  FLOAT     tl_fMinSlope;
  FLOAT     tl_fMaxSlope;
  FLOAT     tl_fMinSlopeFade;
  FLOAT     tl_fMaxSlopeFade;
  FLOAT     tl_fMinSlopeNoise;
  FLOAT     tl_fMaxSlopeNoise;
  FLOAT     tl_fMinSlopeRandom;
  FLOAT     tl_fMaxSlopeRandom;
};

// Create new terrain layer
ENGINE_API extern INDEX TRL_CreateLayer_t(CTerrain *ptrTerrain, const CTFileName &fnmTexture);
// Delete existing terrain layer
ENGINE_API extern void TRL_DeleteLayer(CTerrain *ptrTerrain, INDEX iTerrainLayer, BOOL bDestroyingTerrain=FALSE);
// Change layer texture
ENGINE_API extern void TRL_SetLayerTexture_t(CTerrain *ptrTerrain, INDEX iTerrainLayer, const CTFileName &fnmTexture);
ENGINE_API extern void TRL_SetLayerTexture_t(CTerrain *ptrTerrain, CTerrainLayer *ptlLayer, const CTFileName &fnmTexture);
// Fill layer with constant value
ENGINE_API extern void TRL_FillLayerData(CTerrain *ptrTerrain, INDEX iTerrainLayer, UBYTE ubFill);
// Precache layer matrix
ENGINE_API extern void TRL_ApplyLayerMatrix(CTerrain *ptrTerrain, INDEX iTerrainLayer);

// Read/Write layer
ENGINE_API extern void TRL_ReadLayerData_t(CTerrain *ptrTerrain, INDEX iTerrainLayer, CTStream *istrFile);
ENGINE_API extern void TRL_WriteLayerData_t(CTerrain *ptrTerrain, INDEX iTerrainLayer, CTStream *ostrFile);

// Resize layer mask
ENGINE_API extern void TRL_UpdateLayerSize(CTerrain *ptrTerrain, INDEX iTerrainLayer, PIX pixNewLMW, PIX pixNewLMH,
						                      PIX pixOldLMW, PIX pixOldLMH);		// yjpark

// yjpark |<--
//-------------------------------------------------------------------------------------------------
// Desc : These contents aren't included in final version
// ------------------------------------------------------------------------------------------------
#ifndef FINALVERSION

// Set layer position
ENGINE_API extern INDEX TRL_SetLayerIndex(CTerrain *ptrTerrain, INDEX iTerrainLayer, INDEX iNewLayerIndex);
// Get current layer texture
ENGINE_API extern CTextureData *TRL_GetLayerTexture_t(CTerrain *ptrTerrain, INDEX iTerrainLayer);
// Import layer mask
ENGINE_API extern void TRL_ImportLayerMask_t(CTerrain *ptrTerrain, INDEX iTerrainLayer, const CTFileName &fnm);
ENGINE_API extern void TRL_ImportLayerMask_t(CTerrain *ptrTerrain, CTerrainLayer *ptlLayer, const CTFileName &fnm);
// Export layer mask
ENGINE_API extern void TRL_ExportLayerMask_t(CTerrain *ptrTerrain, INDEX iTerrainLayer, const CTFileName &fnm);
ENGINE_API extern void TRL_ExportLayerMask_t(CTerrain *ptrTerrain, CTerrainLayer *ptlLayer, const CTFileName &fnm);
// Layer thumbnail
ENGINE_API extern CTextureData *TRL_GetLayerThumbnail(CTerrain *ptrTerrain, INDEX iTerrainLayer, PIX pixWidth, PIX pixHeight);

#endif	// FINALVERSION
// yjpark     -->|

#endif
