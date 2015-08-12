
#ifndef TERRAINEDITING_H
#define TERRAINEDITING_H

#define FLT_FINEBLUR 0
#define FLT_SHARPEN 1
#define FLT_EMBOSS 2
#define FLT_EDGEDETECT 3

#define FLT_COUNT 4

enum ETerrainEdit {
  TE_NONE,
  // brush based altitude editing tools
  TE_BRUSH_ALTITUDE_START,
  TE_BRUSH_ALTITUDE_PAINT,
  TE_BRUSH_ALTITUDE_SMOOTH,
  TE_BRUSH_ALTITUDE_FILTER,
  TE_BRUSH_ALTITUDE_MINIMUM,
  TE_BRUSH_ALTITUDE_MAXIMUM,
  TE_BRUSH_ALTITUDE_FLATTEN,
  TE_BRUSH_ALTITUDE_POSTERIZE,
  TE_BRUSH_ALTITUDE_RND_NOISE,
  TE_BRUSH_ALTITUDE_CONTINOUS_NOISE,
  TE_BRUSH_ALTITUDE_END,
  // brush based layer texture editing tools
  TE_BRUSH_LAYER_START,
  TE_BRUSH_LAYER_PAINT,
  TE_BRUSH_LAYER_SMOOTH,
  TE_BRUSH_LAYER_FILTER,
  TE_BRUSH_LAYER_RND_NOISE,
  TE_BRUSH_LAYER_CONTINOUS_NOISE,
  TE_BRUSH_LAYER_ERASE,
  TE_BRUSH_LAYER_END,
  // brush based shadow map editing tools		// yjpark |<--
  TE_BRUSH_SHADOW_START,
  TE_BRUSH_SHADOWMAP_PAINT,
  TE_BRUSH_SHADOWMAP_SMOOTH,
  TE_BRUSH_SHADOW_END,
  // brush based attribute map editing tools
  TE_BRUSH_ATTRIBUTE_START,
  TE_BRUSH_ATTRIBUTEMAP_PAINT,
  //TE_BRUSH_SHADOW_SMOOTH,
  //TE_BRUSH_SHADOW_FILTER,
  TE_BRUSH_ATTRIBUTE_END,						// yjpark     -->|
  // edge editing tools
  TE_BRUSH_EDGE_START,
  TE_BRUSH_EDGE_ERASE,
  TE_BRUSH_EDGE_END,
  // whole terrain altitude operations
  TE_ALTITUDE_START,
  TE_ALTITUDE_SMOOTH,
  TE_ALTITUDE_FILTER,
  TE_ALTITUDE_MINIMUM,
  TE_ALTITUDE_MAXIMUM,
  TE_ALTITUDE_FLATTEN,
  TE_ALTITUDE_POSTERIZE,
  TE_ALTITUDE_RND_NOISE,
  TE_ALTITUDE_CONTINOUS_NOISE,
  TE_ALTITUDE_END,
  // generate terrain
  TE_GENERATE_TERRAIN,
  // equalize heightmap
  TE_ALTITUDE_EQUALIZE,
  // tile painting
  TE_TILE_PAINT,
  // whole layer operations
  TE_LAYER_START,
  TE_LAYER_SMOOTH,
  TE_LAYER_FILTER,
  TE_LAYER_RND_NOISE,
  TE_LAYER_CONTINOUS_NOISE,
  TE_CLEAR_LAYER_MASK,
  TE_FILL_LAYER_MASK,
  TE_LAYER_END,
  // whole shadow map operations		// yjpark |<--
  TE_SHADOW_START,
  TE_SHADOW_END,
  // whole attribute map operations
  TE_ATTRIBUTE_START,
  TE_CLEAR_ATTRIBUTE_MASK,
  TE_FILL_ATTRIBUTE_MASK,
  TE_ATTRIBUTE_END,						// yjpark     -->|
};

class CTerrainUndo {
public:
  ULONG tu_ulEntityID;
  UWORD *tu_puwUndoBuffer;
  ULONG *tu_pulUndoBuffer;
  UWORD *tu_puwRedoBuffer;
  ULONG *tu_pulRedoBuffer;
  CTRect tu_rcRect;
  BufferType tu_btUndoBufferType;
  INDEX tu_iUndoBufferData;
  
  // constructor
  CTerrainUndo();
};

class CTileInfo {
public:
  INDEX ti_ix;
  INDEX ti_iy;
  BOOL ti_bSwapXY;
  BOOL ti_bFlipX;
  BOOL ti_bFlipY;
  CTileInfo();
};

extern CTextureData *_ptdContinousRandomNoise;
extern CTextureData *_ptdDistributionRandomNoise;
BOOL SetupContinousNoiseTexture( void);
void FreeContinousNoiseTexture( void);
BOOL SetupDistributionNoiseTexture( void);
void FreeDistributionNoiseTexture( void);

void RandomizeWhiteNoise(void);
FLOAT *GenerateTerrain_FBMBuffer(PIX pixW, PIX pixH, INDEX ctOctaves, FLOAT fHighFrequencyStep,
                                 FLOAT fStepFactor, FLOAT fMaxAmplitude, FLOAT fAmplitudeDecreaser,
                                 BOOL bAddNegativeValues, BOOL bRandomOffest, FLOAT &fMin, FLOAT &fMax);
void GenerateLayerDistribution(INDEX iForLayer, CTRect rect);
void GenerateLayerDistribution(INDEX iLayer);
void OptimizeLayers(void);
void RecalculateShadows(void);
void SetHMPixel( UWORD pix, INDEX x, INDEX y);
UWORD GetHMPixel(INDEX x, INDEX y);
void EditTerrain(CTextureData *ptdBrush, FLOAT3D &vHitPoint, FLOAT fStrength, ETerrainEdit teTool);
void UpdateLayerDistribution(void);
void ApplyFilterOntoTerrain(void);
void ApplySmoothOntoTerrain(void);
void ApplyContinousNoiseOntoTerrain(void);
void ApplyRndNoiseOntoTerrain(void);
void ApplyGenerateTerrain(void);
void ApplyEqualizeOntoTerrain(void);
void ApplyMinimumOntoTerrain(void);
void ApplyMaximumOntoTerrain(void);
void ApplyFlattenOntoTerrain(void);
void ApplyPosterizeOntoTerrain(void);
void DiscardLayerDistribution(CTRect rect);
void ApplyTerrainUndo(CTerrainUndo *ptrud);
void ApplyTerrainRedo(CTerrainUndo *ptrud);
void DeleteTerrainUndo(CWorldEditorDoc* pDoc);
void TerrainEditBegin(void);
void TerrainEditEnd(int iTerrainWidth =0);
void ObtainLayerTileInfo(CDynamicContainer<CTileInfo> *pdcTileInfo, CTextureData *ptdTexture, INDEX &ctTilesPerRaw);

#endif // TERRAINEDITING_H
