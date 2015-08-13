#ifndef SE_INCL_TERRAIN_EDITING_HPP
#define SE_INCL_TERRAIN_EDITING_HPP
#ifdef PRAGMA_ONCE
  #pragma once
#endif

enum BufferType {
  BT_INVALID   = 0,
  BT_HEIGHT_MAP,
  BT_LAYER_MASK,
  BT_EDGE_MAP,
  BT_SHADOW_MAP,		// yjpark
  BT_ATTRIBUTE_MAP,		// yjpark
};

enum SelectionFill {
  SF_POLYGON = 0,
  SF_WIREFRAME,
  SF_POINTS
};

// yjpark |<--
//-------------------------------------------------------------------------------------------------
// Desc : These contents aren't included in final version
// ------------------------------------------------------------------------------------------------
#ifndef FINALVERSION

ENGINE_API extern UWORD *TRE_GetBufferForEditing(CTerrain *ptrTerrain, CTRect rcRect, BufferType btType, INDEX iBufferData=-1);
ENGINE_API extern void TRE_SetBufferForEditing(CTerrain *ptrTerrain, UWORD *puwBuffer, CTRect rcRect, BufferType btType, INDEX iBufferData=-1);
ENGINE_API extern void TRE_FreeBufferForEditing(UWORD *puwBuffer);
// yjpark |<--
ENGINE_API extern ULONG* TRE_Get4ByteBufferForEditing( CTerrain* ptrTerrain, CTRect rcRect, BufferType btType, INDEX iBufferData = -1 );
ENGINE_API extern void TRE_Set4ByteBufferForEditing( CTerrain* ptrTerrain, ULONG* pulBuffer, CTRect rcRect, BufferType btType, INDEX iBufferData = -1 );
ENGINE_API extern void TRE_Free4ByteBufferForEditing( ULONG* pulBuffer );

ENGINE_API extern UBYTE* TRE_Get1ByteBufferForEditing( CTerrain* ptrTerrain, CTRect rcRect, BufferType btType, INDEX iBufferData = -1 );
ENGINE_API extern void TRE_Set1ByteBufferForEditing( CTerrain* ptrTerrain, UBYTE* pubBuffer, CTRect rcRect, BufferType btType, INDEX iBufferData = -1 );
ENGINE_API extern void TRE_Free1ByteBufferForEditing( UBYTE* pubBuffer );
// yjpark     -->|
ENGINE_API extern void TRE_ShowSelection(const CTerrain *ptrTerrain, CTRect &rcSelection, CTextureData *ptdBrush,
                                         COLOR colSelection, FLOAT fStrength, SelectionFill sfFill, BufferType btType);

#endif	// FINALVERSION
// yjpark     -->|

#endif