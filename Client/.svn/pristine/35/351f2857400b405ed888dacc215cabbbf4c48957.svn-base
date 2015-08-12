#ifndef SE_INCL_TERRAIN_HPP
#define SE_INCL_TERRAIN_HPP
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Math/AABBox.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Terrain/TerrainTile.hpp>

#if _DEBUG
  #define TERRAIN_DEBUG        0
  #define TR_DEBUG_SHADOWMAP   0
  #define TR_DEBUG_NORMALS     0
  #define TR_DEBUG_RAYCASTING  0
  #define TR_DEBUG_EXTRACTPOLY 0
  #define TR_DEBUG_PLANEFROMPT 0
  #define TR_DEBUG_EDITING     0 // Not implemented
#endif


// quad tree node child indices
#define QTCN_TOP_LEFT     0
#define QTCN_TOP_RIGHT    1
#define QTCN_BOTTOM_LEFT  2
#define QTCN_BOTTOM_RIGHT 3

// Tile regeneration flags
#define RTF_GEOMETRY  (1UL<<0)    // regenerate tile geometry
#define RTF_LODCHANGE (1UL<<1)    // 
#define RTF_TOPMAP    (1UL<<2)    // regenerate tile topmap
#define RTF_QUADNODE  (1UL<<3)    // regenerate tile quad tree node

#define TER_SHADER_OPT 1

struct QuadTreeNode
{
  FLOATaabbox3D qtn_aabbox;       // Bounding box for this quadtree node
  INDEX         qtn_iTileIndex;   // Index of tile that this node represents
  INDEX         qtn_aiChild[4];   // Indices of children for this quadtree node
};                                
                                  
struct QuadTreeLevel              
{                                 
  INDEX qtl_iFirstNode;           // Index of first quadtree node in this level
  INDEX qtl_ctNodes;              // Count of nodes in this level
  INDEX qtl_ctNodesX;             // Count of nodes in row
  INDEX qtl_ctNodesY;             // Count of nodes in col
};

struct RegenerateTile
{
  INDEX rt_iTileIndex;            // Index of tile that needs to be regenerated
  ULONG rt_ulRegenFlags;          // Regeneration flags
  INDEX rt_iFrameRequest;         // Time of regen request
};

struct TerrainViewer
{
  FLOAT3D tv_vViewerAbs;
  FLOAT   tv_fDistFactor;
};

struct TerrainInfo
{
  void Reset(void) {
    ti_ctVisibleNodes = 0;
    tr_ctVisibleVertices = 0;
    ti_ctVisibleTriangles = 0;
    ti_ctTileEdgeTested   = 0;
    ti_ctTileEdgeRejected = 0;
    ti_ctTileLayersSkiped = 0;
  }
  INDEX ti_ctVisibleNodes;
  INDEX tr_ctVisibleVertices;
  INDEX ti_ctVisibleTriangles;
  INDEX ti_ctGeometryBuilds;
  INDEX ti_ctTileQueued;
  INDEX ti_ctNeighborsQueued;
  INDEX ti_ctTileEdgeTested;
  INDEX ti_ctTileEdgeRejected;
  INDEX ti_ctTileLayersSkiped;
  SLONG ti_slUsedMemory;
  SLONG ti_slSlackMemory;
};

#if _DEBUG
 #define TR_SETINFO(var) _tiTerrainInfo.var
#else 
 #define TR_SETINFO(var)
#endif

class CTerrainImp
{
public:
  // Default constructor
  CTerrainImp(void);
  // Default destructor
  ~CTerrainImp(void);
public:
  INDEX	tr_iTerrainVersion;			// Terrain version													// yjpark

  UWORD *tr_puwHeightMap;           // Terrain height map
  UWORD *tr_puwShadingMap;          // Terrain shading map (for entity shading)
  UBYTE *tr_pubEdgeMap;             // Terrain edge map
  UWORD	*tr_pubAttributeMap;		// Terrain attribute map											//yjpark
  CTextureData *tr_ptdEdgeMap;      // Terrain edge map as texture
  CTextureData *tr_ptdShadowMap;    // Terrain shadow map
  CTextureData *tr_ptdTopMap;       // Terrain top map

  CStaticArray<FLOAT>			tr_afShadowBlurRadius;		// Array of radius for blurring terrain shading map	// yjpark |<--
  CStaticArray<GFXColor>		tr_acolObjShadowColor;		// Array of color for object shadow map
  CStaticArray<CTextureData*>	tr_aptdShadowMaps;			// Array of terrain shadow maps
  FLOAT							tr_fShadowOverbright;		// Overbright strength of terrain shadow map
  INDEX							tr_iCurrentShadow;			// Index of current shadow
  INDEX							tr_iNextShadow;				// Index of next shadow
  FLOAT							tr_fShadowBlendFactor;		// Blending factor of terrain shadow
  CStaticArray<ULONG>			tr_aulShadowTimes;			// Array of world times for terrain shadow
  FLOAT3D						tr_vHitPoint;				// Hit point for showing attribute triangles		// yjpark     -->|

  PIX     tr_pixHeightMapWidth;     // Terrain height map width
  PIX     tr_pixHeightMapHeight;    // Terrain height map height
  PIX     tr_pixTopMapWidth;        // Terrain top map width
  PIX     tr_pixTopMapHeight;       // Terrain top map height
  SLONG   tr_slShadowMapSizeAspect; // Terrain shadow map size aspect ( bit shift )
  SLONG   tr_slShadingMapSizeAspect;// Terrain shading map size aspect ( bit shift )
  FLOAT   tr_fDistFactor;           // Distance for LOD switching
  FLOAT3D tr_vStretch;              // Terrain stretch
  FLOAT3D tr_vMetricSize;           // Terrain size in metars
  SLONG   tr_slAttributeMapSizeAspect;	// Terrain attribute map size aspect ( scalar multiply )		// yjpark

  INDEX tr_ctTilesX;                // Terrain tiles in row
  INDEX tr_ctTilesY;                // Terrain tiles in col
  INDEX tr_ctQuadsPerTileX;         // Quads count per tile row
  INDEX tr_iMaxTileLOD;             // Maximum tile LOD
  INDEX tr_iFirstTopMapLOD;         // First tile LOD that will be using top map
  Matrix12 tr_mTopMap;              // Terrain top map transform matrix
  Matrix12 tr_mEdgeMap;             // Terrain edge map transform matrix
  Matrix12 tr_mShadowMap;           // Terrain shadow map transform matrix
  CStaticArray<QuadTreeNode>        tr_aqtnQuadTreeNodes;  // Array of quadtree nodes
  CStaticArray<QuadTreeLevel>       tr_aqtlQuadTreeLevels; // Array of quadtree levels
  CStaticArray<CTerrainTile>        tr_attTiles;           // Array of terrain tiles for terrain
  CStaticArray<CTerrainLayer>       tr_atlLayers;          // Array of terrain layers
  CStaticStackArray<RegenerateTile> tr_artRegenQueue;      // List of tiles that need to be regenerated
};

// Upload edge map as texture
extern void TR_UploadEdgeMap(CTerrain *ptrTerrain, BOOL bOptimize=TRUE);
// Upload top map as texture
extern void TR_UploadTopMap(CTerrain *ptrTerrain);
// Upload layer mask as texture
extern void TR_UploadLayerMask(CTerrain *ptrTerrain, INDEX iTerrainLayer, BOOL bOptimize=TRUE);
// Upload attribute map as texture
extern void TR_UploadAttributeMap( CTerrain *ptrTerrain );			// yjpark

#endif