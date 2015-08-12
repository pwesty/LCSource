#include "stdh.h"
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Terrain/TerrainTile.hpp>

CTerrainTile::CTerrainTile(void)
{
  tt_ulFlags = 0;
  tt_ulVisLayers = 0xFFFFFFFF;
  tt_iCurrentLOD   = -1;
  tt_iRequestedLOD = -1;
  tt_fLODLerpFactor = 0.0f;
  tt_iTileIndex = -1;
  tt_aiNeighbours[NB_TOP] = -1;
  tt_aiNeighbours[NB_LEFT] = -1;
  tt_aiNeighbours[NB_RIGHT] = -1;
  tt_aiNeighbours[NB_BOTTOM] = -1;
  tt_aiNBLODs[NB_TOP] = -1;
  tt_aiNBLODs[NB_LEFT] = -1;
  tt_aiNBLODs[NB_RIGHT] = -1;
  tt_aiNBLODs[NB_BOTTOM] = -1;
}

CTerrainTile::~CTerrainTile(void)
{
}

// Clear tile data
void CTerrainTile::Clear(void)
{
}
