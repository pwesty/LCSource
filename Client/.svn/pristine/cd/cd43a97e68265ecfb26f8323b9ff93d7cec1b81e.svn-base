#ifndef SE_INCL_TERRAIN_GENERATE_HPP
#define SE_INCL_TERRAIN_GENERATE_HPP
#ifdef PRAGMA_ONCE
  #pragma once
#endif

// Texture generation
#define PTF_ALPHAONLY (1UL<<0)
#define PTF_STATIC    (1UL<<1)
#define PTF_CONSTANT  (1UL<<2)
#define PTF_ZEROMEM   (1UL<<16)

extern CTextureData *TR_CreateTexture(void);
extern void TR_MarkTextureUsed(CTextureData *ptd);
extern void TR_MarkTextureUnused(CTextureData *ptd);
extern void TR_PrepareTexture(CTextureData &td, PIX pixWidth, PIX pixHeight,ULONG ulFlags);
extern void TR_UploadTexture(CTextureData &td, PIX pixWidth, PIX pixHeight,ULONG ulFlags);
extern void TR_BakeTopMap(CTerrain *ptrTerrain);


// Tile generation
struct UnusedDataArray
{
  CListNode uda_lnUnusedArrays;
  SLONG     uda_slMemSize;  // data array size
  ULONG     uda_ulFlags;    // data array flags
  void     *uda_paObjects;  // data array
  SLONG     uda_slBufferID; // data array vertex buffer id
};
extern void QueueTileRegeneration(CTerrainImp *ptr, INDEX iTerrainTile, ULONG ulForcedFlags=0, BOOL bNeighbor=FALSE);
extern void RegenerateTerrainTiles(CTerrainImp *ptr);

#endif
