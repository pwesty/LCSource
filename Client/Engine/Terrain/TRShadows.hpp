#ifndef SE_INCL_TERRAIN_SHADOWS_HPP
#define SE_INCL_TERRAIN_SHADOWS_HPP
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#define TLF_TOUCHES_TERRAIN (1UL<<1)
struct TerrainPointLight
{
  ULONG         tpl_ulFlags;
  FLOAT         tpl_fFallOff;
  FLOAT         tpl_fHotSpot;
  COLOR         tpl_colLight;
  FLOAT3D       tpl_vLightPos;
  CPlacement3D  tpl_plPlacement;
  CTRect        tpl_rcLight;
  FLOATaabbox3D tpl_bboxLight;
};

struct TerrainDirectionalLight
{
  ULONG         tdl_ulFlags;
  COLOR         tdl_colLight;
  COLOR         tdl_colAmbient;
  FLOAT3D       tdl_vLightNormal;
  CPlacement3D  tdl_plPlacement;
  FLOATaabbox3D tdl_bboxLight;
};

ENGINE_API extern void TRS_BeginShadowMapUpdate(CTerrain *ptrTerrain);
ENGINE_API extern void TRS_EndShadowMapUpdate(CTerrain *ptrTerrain);
// Add point light to shadow map calculation
ENGINE_API extern void TRS_AddPointLight(FLOAT fFallOff, FLOAT fHotSpot, COLOR colLight, CPlacement3D plPlacement);
// Add directional light to shadow map calculation
ENGINE_API extern void TRS_AddDirectionalLight(COLOR colLight, COLOR colAmbient, CPlacement3D plPlacement);
// Add all lights in world
ENGINE_API extern void TRS_ColectLightsFromWorld(CTerrain *ptrTerrain);
// setup terrain position (must be set for shadow map update)
ENGINE_API extern void TRS_SetTerrainPlacement(const CPlacement3D &pl);
ENGINE_API extern void TRS_SetTerrainPlacement(const Matrix12 &mObjToAbs);
// Calculate region of shadow map with colected lights
ENGINE_API extern void TRS_UpdateShadowMap(CTerrain *ptrTerrain, FLOATaabbox3D bboxUpdate,
											BOOL bUpdateObjectShadow = FALSE );		// yjpark


#endif