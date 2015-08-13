#ifndef SE_INCL_TERRAIN_MISC_HPP
#define SE_INCL_TERRAIN_MISC_HPP
#ifdef PRAGMA_ONCE
  #pragma once
#endif

struct SimpleTerrainTriangle
{
  SimpleTerrainTriangle(void) {};
	SimpleTerrainTriangle(FLOAT3D v0, FLOAT3D v1, FLOAT3D v2) 
	{
    stt_avx[0] = v0;
    stt_avx[1] = v1;
    stt_avx[2] = v2;
  }
  FLOAT3D stt_avx[3];
};

ENGINE_API extern FLOAT TR_GetHeight( CTerrainImp *ptr, FLOAT fX, FLOAT fZ );		// yjpark

// Extract vertices and triangles from specified rectange on height map
extern void TR_ExtractVertices(CTerrainImp *ptr, CTRect rcSource,
                               CStaticArray<FLOAT3D> &avxVertices);

extern void TR_ExtractAttrPoligonsInRect( CTerrainImp *ptrTerrain, CTRect &rcExtract, 
											FLOAT3D **pavRetVtx, INDEX &ctRetVertices, GFXColor **pacolRetCol,
											UWORD **puwRetInd, INDEX &ctRetIndices, UWORD ubAttr );

// [070705: Su-won] ATTRIBUTEMAP_BLOCK
extern void TR_ExtractAttrBlockPolygonsInRect( CTerrainImp *ptr, CTRect &rcExtract, 
											FLOAT3D **pavRetVtx, INDEX &ctRetVertices, 
											UWORD **puwRetInd, INDEX &ctRetIndices, float fHeight );

#endif