#ifndef SE_INCL_TERRAIN_RAYCAST_HPP
#define SE_INCL_TERRAIN_RAYCAST_HPP
#ifdef PRAGMA_ONCE
  #pragma once
#endif

struct TerrainRay
{
  TerrainRay(void) {
    trr_vHitPoint = FLOAT3D(UpperLimit(1.0f),UpperLimit(1.0f),UpperLimit(1.0f));
  };
  TerrainRay(FLOAT3D vHitBegin, FLOAT3D vHitEnd) {
    trr_vHitBegin = vHitBegin;
    trr_vHitEnd   = vHitEnd;
    trr_vHitPoint = FLOAT3D(UpperLimit(1.0f),UpperLimit(1.0f),UpperLimit(1.0f));
  };
  TerrainRay(FLOAT3D vHitBegin, FLOAT3D vHitEnd, FLOAT3D vHitPoint) {
    trr_vHitBegin = vHitBegin;
    trr_vHitEnd   = vHitEnd;
    trr_vHitPoint = vHitPoint;
  };
  FLOAT3D trr_vHitBegin;
  FLOAT3D trr_vHitEnd;
  FLOAT3D trr_vHitPoint;
};

#endif