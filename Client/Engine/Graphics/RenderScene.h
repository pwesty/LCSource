#ifndef SE_INCL_RENDERSCENE_H
#define SE_INCL_RENDERSCENE_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Math/TextureMapping.h>
//#include <Engine/Graphics/Vertex.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////
// WORLD RENDER CONSTANTS

// scene polygon flags
#define SPOF_SELECTED					(1UL<<0)   // is polygon currently selected or not?
#define SPOF_TRANSPARENT				(1UL<<1)   // polygon has alpha keying
#define SPOF_ATTRIBUTE					(1UL<<3)   // Polygon has the attribute						// yjpark |<--
#define SPOF_ATTWALKABLE_EVEN			(1UL<<4)   // The attribute is walkable
#define SPOF_ATTWALKABLE_ODD			(1UL<<5)   // The attribute is walkable
#define SPOF_ATTUNWALKABLE_EVEN			(1UL<<6)   // The attribute is unwalkable
#define SPOF_ATTUNWALKABLE_ODD			(1UL<<7)   // The attribute is unwalkable
#define SPOF_ATTSTAIR					(1UL<<8)   // The attribute is stair
#define SPOF_ATTSTAIRWALL				(1UL<<9)  // The attribute is the wall of stair
#define	SPOF_ATTPEACE_ZONE				(1UL<<10)  // The attribute is peace zone
#define	SPOF_ATTPRODUCT_ZONE_PUBLIC		(1UL<<11)  // The attribute is production zone - public
#define	SPOF_ATTPRODUCT_ZONE_PRIVATE	(1UL<<12)  // The attribute is production zone - private	// yjpark     -->|
#define SPOF_RENDERFOG					(1UL<<13)  // polygon has fog
#define SPOF_RENDERHAZE					(1UL<<14)  // polygon has haze
#define SPOF_ATTWAR_AREA				(1UL<<15)
#define SPOF_FREEPK_ZONE				(1UL<<16)  // Free PK Zone
#define SPOF_BACKLIGHT					(1UL<<31)  // used internaly

// scene texture flags
#define STXF_CLAMPU         (0x01)    // clamp u coordinate in texture
#define STXF_CLAMPV         (0x02)    // clamp v coordinate in texture
#define STXF_REFLECTION     (0x04)    // clamp v coordinate in texture
#define STXF_AFTERSHADOW    (0x08)    // texture is to be applied after shadow

#define STXF_BLEND_OPAQUE   (0x00)    // opaque texture (just put it on screen)
#define STXF_BLEND_ALPHA    (0x10)    // alpha blend with screen
#define STXF_BLEND_ADD      (0x20)    // add to screen
#define STXF_BLEND_SHADE    (0x30)    // darken or brighten (same as used by shadow maps)
//¾ÈÅÂÈÆ ¼öÁ¤ ½ÃÀÛ	//(Modify Worldbase Overbright to NonOver)(0.1)
#define STXF_BLEND_MODIFIED_SHADE    (0x40)    // darken or brighten ½Å°×¿ë.
//¾ÈÅÂÈÆ ¼öÁ¤ ³¡	//(Modify Worldbase Overbright to NonOver)(0.1)

#define STXF_BLEND_MASK     (0x70)

/*
 * RENDER SCENE structures (for world purposes)
 */
// structure that holds information about a polygon that is fully on partially visible
struct ScenePolygon {
  ScenePolygon *spo_pspoSucc; // next polygon in list
  INDEX  spo_iVtx0;           // first vertex in arrays
  INDEX  spo_ctVtx;           // number of vertices in arrays
  INDEX *spo_piElements;      // array of triangle elements
  INDEX  spo_ctElements;      // element count

  // texture and shadow parameters: 0, 1, 2 are texture maps, 3 is shadow
  CMappingVectors spo_amvMapping[4];  // texture colors and alpha
  COLOR spo_acolColors[4];            // texture flags
  UBYTE spo_aubTextureFlags[4];       // current mip factors for each texture
  // textures and shadowmap
  class CTextureObject *spo_aptoTextures[3];
  class CShadowMap     *spo_psmShadowMap; 

  // internal for rendering
  INDEX spo_iVtx0Pass;     // index of first coordinate in per-pass arrays
  COLOR spo_cColor;        // polygon color (for flat or shadow modes)
  ULONG spo_ulFlags;       // polygon flags (selected or not? ...)
  FLOAT spo_fNearestZ;     // Z coord of nearest vertex to viewer
  void *spo_pvPolygon;     // user data for high level renderer (brush polygon)
};

// renders whole scene (all visible polygons) to screen drawport
void RenderScene( CDrawPort *pDP, ScenePolygon *pspoFirst, CAnyProjection3D &prProjection,
                  COLOR colSelection, BOOL bTranslucent);
// renders only scene z-buffer
void RenderSceneZOnly( CDrawPort *pDP, ScenePolygon *pspoFirst, CAnyProjection3D &prProjection);
// renders flat background of the scene
void RenderSceneBackground(CDrawPort *pDP, COLOR col);


#endif  /* include-once check. */

