#ifndef SE_INCL_TERRAIN_LAYER_HPP
#define SE_INCL_TERRAIN_LAYER_HPP
#ifdef PRAGMA_ONCE
  #pragma once
#endif

class CTerrainLayerImp
{
public:
  CTerrainLayerImp(void);
  CTerrainLayerImp(const CTerrainLayerImp &trlOther);
  ~CTerrainLayerImp(void);
  CTerrainLayerImp &operator=(const CTerrainLayerImp &trlOther);
public:
  INDEX			trl_iLayerVersion;	// Layer version		// yjpark

  Matrix12      trl_mTransform;   // Layer transformation matrix
  UBYTE        *trl_pubLayerMask; // Layer mask
  CTextureData *trl_ptdTexture;   // Layer texture
  CTextureData *trl_ptdLayerMask; // Layer mask as texture

  CTextureObject* trl_ptoTexObj;
};

extern void TRL_CopyLayer(CTerrain *ptrTerrain, INDEX iTerrainLayer, const CTerrainLayer &tlSrc);
extern void TRL_CopyLayerImp(CTerrain *ptrTerrain, INDEX iTerrainLayer, const CTerrainLayerImp &trlSrc);

#endif
