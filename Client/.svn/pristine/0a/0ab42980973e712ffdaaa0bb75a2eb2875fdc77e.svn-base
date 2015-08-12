#ifndef SE_INCL_TERRAIN_H
#define SE_INCL_TERRAIN_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/Lists.h>
#include <Engine/Brushes/BrushBase.h>
#include <Engine/Terrain/TREditing.hpp>
#include <Engine/Graphics/GfxLibrary.h>

#define TR_USES_TOPMAP             (1UL<<0) // does terrain uses top map
#define TR_SYNC_TOPMAP             (1UL<<1) // terrain top map must be sync before saving
#define TR_HAS_FOG                 (1UL<<7) // terrain has fog
#define TR_HAS_HAZE                (1UL<<8) // terrain has haze
#define TR_INITIALIZED             (1UL<<9) // if this flag is missing terrains isn't initialized

// Terrain class
class ENGINE_API CTerrain : public CBrushBase
{
public:
	// Constructor
	CTerrain(void);
	// Destructor
	~CTerrain(void);

	// Read from stream
	void Read_t(CTStream *istrFile);
	// Write to stream
	void Write_t(CTStream *ostrFile);
	// Clean terrain data
	void Clear(void);

	// Identify as terrain
	INDEX GetBrushType(void)       { return CBrushBase::BT_TERRAIN; }

	void AddFlag(ULONG ulFlags)    { tr_ulTerrainFlags |= ulFlags;  } // Add terrain flag
	void RemoveFlag(ULONG ulFlags) { tr_ulTerrainFlags &= ~ulFlags; } // Remove terrain flag
	void SetFlags(ULONG ulFlags)   { tr_ulTerrainFlags = ulFlags;   } // Set terrain flags
	ULONG GetFlags(void)           { return tr_ulTerrainFlags;      } // Get terrain flags

public:
	INDEX			tr_iSelectedLayer;		// Selected layer
	INDEX			tr_iSelectedShadowMap;	// Selected shadow map					// yjpark
	ULONG			tr_ulTerrainFlags;		// Terrain flags
	CListHead		tr_lhShadingInfos;		// For linking shading infos of entities
	CListNode		tr_lnInActiveTerrains;	// For linking in list of active terrains in renderer
	CListNode		tr_lnLoadedTerrains;	// For linking in list of all terrains loaded (for reloadtexture)
	CEntity			*tr_penEntity;			// Pointer to entity that holds this terrain
	CTerrainImp		*tr_ptrTerrain;			// Internal terrain object
};


// initialize batch terrain rendering
ENGINE_API extern void TR_BeginRenderingView(CAnyProjection3D &apr, CDrawPort *pdp);
// cleanup after batch terrain rendering
ENGINE_API extern void TR_EndRenderingView(void);

//강동민 수정 시작
ENGINE_API extern void TR_GetStretchMatrix(Matrix12 &matStretch);
ENGINE_API extern void TR_GetObjToViewMatrix(Matrix12 &matObjToView);
//강동민 수정 끝

// setup terrain position
ENGINE_API extern void TR_SetTerrainPlacement(const CPlacement3D &pl);
ENGINE_API extern void TR_SetTerrainPlacement(const FLOATmatrix3D &m, const FLOAT3D &v);
// Additional viewers
ENGINE_API extern void TR_AddTerrainViewer(const CTerrain *ptrTerrain, FLOAT3D vViewerAbs, FLOAT fDistFactor=-1.0f);

// Renders one terrain
ENGINE_API extern void TR_RenderTerrain(CTerrain *ptrTerrain);
// Renders one terrain in wireframe 
ENGINE_API extern void TR_RenderWireFrame(CTerrain *ptrTerrain, COLOR colEdges);
// Render terrain lighting effect
ENGINE_API extern void TR_RenderTerrainLighting( CTerrain *ptrTerrain, CTextureData* ptdTexture );	// yjpark
// Render terrain mesh effect
//ENGINE_API extern void TR_RenderTerrainMesh( CTerrain *ptrTerrain, CTextureData* ptdTextureGfxBlend, gfxSrcBlend, GfxBlend gfxDstBlend );	// yjpark
// Cleans terrain rendering data
ENGINE_API extern void TR_CleanRender(CTerrain *ptrTerrain);

// Copy terrain from other terrain
ENGINE_API extern void TR_CopyTerrain(CTerrain *ptrDest, const CTerrain *ptrSrc);

// Rebuild terrain geometry
ENGINE_API extern void TR_RegenerateAll(CTerrain *ptrTerrain);
// Clear terrain 
ENGINE_API extern void TR_ClearTerrain(CTerrain *ptrTerrain);
// Update terrain shadow map
ENGINE_API extern void TR_UpdateShadowMap(CTerrain *ptrTerrain, const Matrix12 &mObjToAbs, FLOATaabbox3D bboxUpdate,
											BOOL bAutoColectLights = TRUE, BOOL bUpdateObjectShadow = FALSE );	// yjpark
// Disacard all shading infos
ENGINE_API extern void TR_DiscardShadingInfos(CTerrain *ptrTerrain);
// Fill edge map with const value
ENGINE_API extern void TR_FillEdgeMap(CTerrain *ptrTerrain, UBYTE ubFill);
// Resize existing height map
ENGINE_API extern void TR_SetHeightMapSize(CTerrain *ptrTerrain, PIX pixMapWidth, PIX pixMapHeight);
// Resize existing top map
ENGINE_API extern void TR_SetTopMapSize(CTerrain *ptrTerrain, PIX pixMapWidth, PIX pixMapHeight);
// Bind textures used by terrain
ENGINE_API extern void TR_BindTerrainTextures(CTerrain *ptrTerrain);
// Unbind textures used by terrain
ENGINE_API extern void TR_UnbindTerrainTextures(CTerrain *ptrTerrain);

// Set terrain shadowmap size aspect (relative to height map size) and shading map aspect (relative to shadow map size)
ENGINE_API extern void TR_SetShadowMapSize(CTerrain *ptrTerrain, INDEX iShadowMapAspect, INDEX iShadingMapAspect);
// Set terrain metric size
ENGINE_API extern void TR_SetTerrainSize(CTerrain *ptrTerrain, FLOAT3D vSize);
// Add terrain lighting effect
ENGINE_API extern void TR_AddTerrainLighting( CLightSource *plsTerrainLight );						// yjpark
// Remove terrain lighting effect
ENGINE_API extern void TR_RemoveTerrainLighting( CLightSource *plsTerrainLight );					// yjpark
// Get index of soung of terrain layer
ENGINE_API extern INDEX TR_GetTerrainLayerSound( FLOAT fX, FLOAT fZ );								// yjpark

/*
 * TerrainMisc
 */
// Get plane from given point
ENGINE_API extern FLOATplane3D TR_GetPlaneFromPoint(const CTerrain *ptrTerrain, const Matrix12 &mTerrain, const FLOAT3D &vAbsPoint);
// Get terrain bounding box
ENGINE_API extern FLOATaabbox3D TR_GetTerrainBBox(const CTerrain *ptrTerrain);
ENGINE_API extern FLOATaabbox3D TR_GetStretchedTerrainBBox(const CTerrain *ptrTerrain);

ENGINE_API extern FLOAT TR_TestRayCastHit(CTerrain *ptrTerrain, const FLOATmatrix3D &mRotation, const FLOAT3D &vPosition, 
																					FLOAT3D vOrigin, FLOAT3D vTarget,FLOAT fOldDistance, BOOL bHitInvisibleTris);

ENGINE_API extern FLOAT TR_TestRayCastHit(CTerrain *ptrTerrain, const FLOATmatrix3D &mRotation, const FLOAT3D &vPosition, 
																					FLOAT3D vOrigin, FLOAT3D vTarget,FLOAT fOldDistance, BOOL bHitInvisibleTris,
																					FLOATplane3D &plHitPlane, FLOAT3D &vHitPoint);

ENGINE_API extern void TR_ExtractPolygonsInBox(const CTerrain *ptrTerrain, const FLOATaabbox3D &bboxExtract,
																							 const Matrix12 &mObjToAbs, FLOAT3D **pavVtx, INDEX &ctVtx,
																							 UWORD **puwInd, INDEX &ctIndices);
//강동민 수정 시작
//ENGINE_API extern void TR_ExtractPoligonsInRect(const CTerrain *ptrTerrain, const CTRect &rcExtract,
//																								FLOAT3D **pavVtx, INDEX &ctVtx, UWORD **puwInd, INDEX &ctIndices);
ENGINE_API extern void TR_ExtractPoligonsInRect(const CTerrain *ptrTerrain, CTRect &rcExtract,
																								FLOAT3D **pavVtx, INDEX &ctVtx, UWORD **puwInd, INDEX &ctIndices);
//강동민 수정 끝

ENGINE_API extern FLOAT2D TR_CalcShadingTC(const CTerrain *ptrTerrain, const FLOAT3D &vPointAbs, const Matrix12 &mObjToAbs);

ENGINE_API extern BOOL TR_IsTerrainBelowPoint(const CTerrain *ptrTerrain, const FLOAT3D &vPoint,
												FLOAT fMaxDist, const FLOAT3D &vGravityDir);

ENGINE_API extern void TR_GetMatrixFromEntity(Matrix12 &m, const CTerrain *ptrTerrain);

ENGINE_API extern void TRS_UpdateRealShadowMap( CTerrain* ptrTerrain, INDEX iShadowMap,
												GFXColor colObjShadow, FLOAT fShadowBlur,
												BOOL bHitSka = FALSE, BOOL bHitMdl = FALSE,
												BOOL bHitBrush = FALSE, BOOL bHitTerrain = FALSE );			// yjpark

inline PIX GetShadowMapWidth(PIX pixHeightMapWidth, SLONG slSizeAspect) {
	if(slSizeAspect<0) {
		return (pixHeightMapWidth-1)>>(-slSizeAspect);
	} else {
		return (pixHeightMapWidth-1)<<( slSizeAspect);
	}
};
#define GetShadowMapHeight  GetShadowMapWidth

inline PIX GetShadingMapWidth(PIX pixShadowMapWidth, SLONG slSizeAspect) {
	if(slSizeAspect<0) {
		return (pixShadowMapWidth)>>(-slSizeAspect);
	} else {
		return (pixShadowMapWidth)<<( slSizeAspect);
	}
};

#define GetShadingMapHeight GetShadingMapWidth

// yjpark |<--
//-------------------------------------------------------------------------------------------------
// Desc : These contents aren't included in final version
// ------------------------------------------------------------------------------------------------
#ifndef FINALVERSION

// LOD factor management
ENGINE_API extern void TR_SetCurrentDistance(FLOAT fDistFactor);
ENGINE_API extern void TR_ForceCustomTileLOD(INDEX iForcedLOD);
// Fill height map with const value
ENGINE_API extern void TR_FillHeightMap(CTerrain *ptrTerrain, UWORD uwFill);
// Update terrain top map
ENGINE_API extern void TR_UpdateTopMap(CTerrain *ptrTerrain);
// Change distance factor (call TR_RegenerateAll after this)
ENGINE_API extern void TR_SetDistanceFactor(CTerrain *ptrTerrain, FLOAT fDistFactor);
// Get terrain metric size
ENGINE_API extern FLOAT3D TR_GetTerrainSize(const CTerrain *ptrTerrain);
// Get terrain stretch
ENGINE_API extern FLOAT3D TR_GetTerrainStretch(const CTerrain *ptrTerrain);
// Import * export height map from targa file
ENGINE_API extern void TR_ImportHeightMap_t(CTerrain *ptrTerrain, const CTFileName &fnm, BOOL bUse16b = TRUE);
ENGINE_API extern void TR_ExportHeightMap_t(CTerrain *ptrTerrain, const CTFileName &fnm, BOOL bUse16b = TRUE);
// Import & export edge map from targa file
ENGINE_API extern void TR_ImportEdgeMap_t(CTerrain *ptrTerrain, const CTFileName &fnm);
ENGINE_API extern void TR_ExportEdgeMap_t(CTerrain *ptrTerrain, const CTFileName &fnm);

// Get color for shading info
ENGINE_API extern COLOR TR_GetShadeColor(const CTerrain *ptrTerrain, CShadingInfo *psi);
// Get terrain height map width
ENGINE_API extern PIX TR_GetHeightMapWidth(const CTerrain *ptrTerrain);
// Get terrain height map height
ENGINE_API extern PIX TR_GetHeightMapHeight(const CTerrain *ptrTerrain);
// Get terrain top map width
ENGINE_API extern PIX TR_GetTopMapWidth(const CTerrain *ptrTerrain);
// Get terrain top map height
ENGINE_API extern PIX TR_GetTopMapHeight(const CTerrain *ptrTerrain);
// Get terrain info
ENGINE_API extern CTString TR_GetTerrainInfo(const CTerrain *ptrTerrain);

ENGINE_API extern CTPoint TR_RelFromAbsPoint(const CTerrain *ptrTerrain, const FLOAT3D &vPointAbs, const Matrix12 &mObjToAbs,
											 BufferType btType = BT_HEIGHT_MAP, SLONG slStretch = 1);	// yjpark

// Set terrain attribute map size
ENGINE_API extern void TR_SetAttributeMapSize( CTerrain *ptrTerrain );									// yjpark

// Shadow map functions
ENGINE_API extern SLONG TR_GetShadowMapAspect(const CTerrain *ptrTerrain);								// yjpark |<--
ENGINE_API extern SLONG TR_GetShadingMapAspect(const CTerrain *ptrTerrain);
ENGINE_API extern void TRS_DeleteShadowMap( CTerrain *ptrTerrain, INDEX iTerrainLayer );
ENGINE_API extern void TRS_AddShadowMap( CTerrain *ptrTerrain );
ENGINE_API extern void TRS_SetShadowMapIndex( CTerrain *ptrTerrain, INDEX iShadowIndex, INDEX iNewShadowIndex);
ENGINE_API extern void TRS_ClearRealShadowMap( CTerrain* ptrTerrain, INDEX iShadowMap );

// Attribute map functions
ENGINE_API extern void TR_GenerateAttributeMap( CTerrain *ptrTerrain, CWorld *pWorld );
ENGINE_API extern void TR_ClearAttributeMap( CTerrain *ptrTerrain );
ENGINE_API extern void TR_ImportAttributeMap_t( CTerrain *ptrTerrain, CTFileName &fnm );
ENGINE_API extern void TR_ExportAttributeMap_t( CTerrain *ptrTerrain, CTFileName &fnm );

ENGINE_API extern void TR_DrawModelAttribute( CWorld *pWorld, UWORD* pbMap, PIX pixAttributeMapWidth );

// Server height map
ENGINE_API extern void TR_ExportServerHeightMap_t( CTerrain *ptrTerrain, CTFileName &fnm );				// yjpark     -->|

#endif	// FINALVERSION
// yjpark     -->|


#endif
