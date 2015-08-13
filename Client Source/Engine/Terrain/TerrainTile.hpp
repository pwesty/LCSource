#ifndef SE_INCL_TERRAIN_TILE_HPP
#define SE_INCL_TERRAIN_TILE_HPP
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/Rect.h>
#include <Engine/Terrain/TRDataArray.hpp>
#include <Engine/Templates/StaticArray.cpp>

// neighbour indices
#define NB_FIRST  NB_LEFT
#define NB_LEFT   0 // Left neighbour
#define NB_RIGHT  1 // Right neighbour
#define NB_TOP    2 // Top neighbour
#define NB_BOTTOM 3 // Bottom neighbour

// reverse neighbour indices
#define NB_REV_FIRST  NB_REV_LEFT
#define NB_REV_LEFT   NB_RIGHT  // Reverse left neighour index
#define NB_REV_RIGHT  NB_LEFT   // Reverse right neighbour index
#define NB_REV_TOP    NB_BOTTOM // Reverse top neighbour index
#define NB_REV_BOTTOM NB_TOP    // Reverse bottom neighbour index

// Vertex lerp factors (stored in terrain vertex)
#define VLF_CTLF   0 // index of lerp factor for current tile
#define VLF_LTLF   1 // index of lerp factor for left neighbour
#define VLF_RTLF   2 // index of lerp factor for right neighbour
#define VLF_TTLF   3 // index of lerp factor for top neightbour
#define VLF_BTLF   4 // index of lerp factor for bottom neighbour
#define VLF_LCTLF  5 // Min of left and current tile lerp factor
#define VLF_RCTLF  6 // Min of right and current tile lerp factor
#define VLF_TCTLF  7 // Min of top and current tile lerp factor
#define VLF_BCTLF  8 // Min of bottom and current tile lerp factor
#define VLF_NOLERP 9 // Lerp factor is 0

// Terrain tile flags
#define TTF_BLANK_EDGE_MAP (1UL<<0) // no tile triangles visible
#define TTF_FULL_EDGE_MAP  (1UL<<1) // all tile triangles are visible

// Terrain vertex
struct TerrainVertex
{
  TerrainVertex(void) {
  };
  TerrainVertex(UBYTE ubX, UBYTE ubZ, UWORD uwY, UWORD uwY2, UBYTE ubLerp) {
    tv_ubX  = ubX;
    tv_ubZ  = ubZ;
    tv_ubLerp = ubLerp;
    tv_uwY  = uwY;
    tv_uwY2 = uwY2;
  };
  // Reg 0
  UBYTE tv_ubX;    // x pos
  UBYTE tv_ubZ;    // z pos
  UBYTE tv_ubLerp; // lerp factor index
  UBYTE tv_ubMask; // vertex edge mask
  // Reg 1
  UWORD tv_uwY;    // y pos
  UWORD tv_uwY2;   // target y pos
};

// Terrain triangle
struct TerrainTriangle
{
  TerrainTriangle(void) {
  }
  TerrainTriangle(UWORD uw1, UWORD uw2, UWORD uw3) {
    trt_auwIndices[0] = uw1;
    trt_auwIndices[1] = uw2;
    trt_auwIndices[2] = uw3;
  }
  UWORD trt_auwIndices[3];
};

// Terrain vertex alpha
struct TerrainVertexAlpha
{
  UBYTE tva_ubAlpha;
};

// Array of vertex alpha colors for 4 terrain tile layers
struct TerrainGroupVertexAlpha
{
  UBYTE tgva_aubAplha[4];
};

struct TerrainTextureLayer
{
  CTRDataArray<TerrainVertexAlpha> txl_avaAlphaCols;
  CTRDataArray<TerrainTriangle>    txl_atriTriangles;
  CTextureData                    *txl_ptdTexture;
};

struct TerrainTileLayer
{
  CTRDataArray<TerrainVertex>      til_avxVertices;
  CTRDataArray<TerrainVertexAlpha> til_avaAlphaCols;
  CTRDataArray<TerrainTriangle>    til_atriTriangles;
  CTextureData                    *til_ptdTexture;
};

class CTerrainTile
{
public:
  // Constructor
  CTerrainTile(void);
  // Destructor
  ~CTerrainTile(void);
  // Clear tile data
  void Clear(void);

public:
  ULONG    tt_ulFlags;         // tile flags
  ULONG    tt_ulVisLayers;     // bit array of visible layers [
  INDEX    tt_iCurrentLOD;     // current tile LOD
  INDEX    tt_iRequestedLOD;   // requested tile LOD
  FLOAT    tt_fLODLerpFactor;  // LOD lerp factor
  INDEX    tt_iTileIndex;      // Tile index in array of terrain tiles
  INDEX    tt_aiNeighbours[4]; // Tile neighbour indices
  INDEX    tt_aiNBLODs[4];     // Neighbour LODs at tile generation time
  CTRect   tt_rcSource;        // Source rectangle on height map
  Matrix12 tt_mTileToTerrain;  // From tile space to terrain space translation matrix
  Matrix12 tt_mTopMap;         // Tile top map transfrom matrix
  Matrix12 tt_mEdgeMap;        // Tile edge map transform matrix
  Matrix12 tt_mShadowMap;      // Tile shadow map transform matrix
  Matrix12 tt_mAttributeMap;   // Tile shadow map transform matrix
                                        
  CStaticArray<TerrainTextureLayer>     tt_atxlLayers;     // Array of texture layers
  CStaticArray<TerrainTileLayer>        tt_atilLayers;     // Array of tile layers
  CTRDataArray<TerrainVertex>           tt_avxVertices;    // Array of vertices
  CTRDataArray<TerrainTriangle>         tt_atriTriangles;  // Array of triangles
  CTRDataArray<TerrainGroupVertexAlpha> tt_atgvaAlphaCols; // Array of vertex alphas for 4 texture layers
};

#endif