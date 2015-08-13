#include "stdh.h"

#include <Engine/Graphics/DrawPort.h>

#include <Engine/Base/Statistics_internal.h>
#include <Engine/Base/Console.h>
#include <Engine/Math/Projection.h>
#include <Engine/Graphics/RenderScene.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/ShadowMap.h>
#include <Engine/Graphics/Fog_internal.h>
#include <Engine/Brushes/Brush.h>
#include <Engine/Brushes/BrushTransformed.h>

#include <Engine/Templates/StaticStackArray.cpp>

// asm shortcuts
#define O offset
#define Q qword ptr
#define D dword ptr
#define W  word ptr
#define B  byte ptr

#define ASMOPT 1

#define SHADOWTEXTURE 3

extern INDEX wld_bShowTriangles;
extern INDEX wld_bShowDetailTextures;
extern INDEX wld_bRenderShadowMaps;
extern INDEX wld_bRenderTextures;
extern INDEX wld_bRenderDetailPolygons;
extern INDEX wld_iDetailRemovingBias;

extern INDEX gfx_bRenderWorld;
extern INDEX shd_iForceFlats;
extern INDEX shd_bShowFlats;

extern BOOL _bMultiPlayer;
extern BOOL CVA_bWorld;
static GfxAPIType _eAPI;


// general coordinate stack referenced by the scene polygons
CStaticStackArray<GFXVertex> _avtxScene;

// vertex coordinates and elements used by one pass of polygons
static CStaticStackArray<UWORD> _auwElements;

// 월드 에디터에서 속성맵을 보여주기 위한 임시용..
// 프로젝트를 따로 추가할 예정..
//#define	WORLD_EDITOR

// group flags (single-texturing)
#define GF_TX0					(1L<<0) 
#define GF_TX1					(1L<<1)
#define GF_TX2					(1L<<2)
#define GF_SHD					(1L<<3)
#define GF_FLAT					(1L<<4)		// flat fill instead of texture 1
#define GF_TA1					(1L<<5)		// texture 2 after shade
#define GF_TA2					(1L<<6)		// texture 3 after shade
#define GF_FOG					(1L<<7)
#define GF_HAZE					(1L<<8)
#define GF_KEY					(1L<<9)	// first layer requires alpha-keying

#ifdef	WORLD_EDITOR			// yjpark |<--
	#define GF_SEL						(1L<<10)
	#define GF_ATTWALK_EVEN				(1L<<11)	// Attribute - walkable
	#define GF_ATTWALK_ODD				(1L<<12)	// Attribute - walkable
	#define	GF_ATTUNWALK_EVEN			(1L<<13)	// Attribute - unwalkable
	#define	GF_ATTUNWALK_ODD			(1L<<14)	// Attribute - unwalkable
	#define	GF_ATTSTAIR					(1L<<15)	// Attribute - stair
	#define GF_ATTSTAIRWALL				(1L<<16)	// Attribute - wall of stair
	#define	GF_ATTPEACE_ZONE			(1L<<17)	// Attribute - peace zone
	#define	GF_ATTPRODUCT_ZONE_PUBLIC	(1L<<17)	// Attribute - production zone - public
	#define	GF_ATTPRODUCT_ZONE_PRIVATE	(1L<<17)	// Attribute - production zone - private
	#define GF_ATTWAR_AREA				(1L<<18)	// Attribute - Castle War
	#define GF_ATTFREEPK_ZONE			(1L<<19)	// Attribute - Free PK Zone
	#define	GF_ATTRIBUTE	( GF_ATTWALK_EVEN | GF_ATTWALK_ODD | GF_ATTUNWALK_EVEN | GF_ATTUNWALK_ODD |	GF_ATTSTAIR | GF_ATTSTAIRWALL | GF_ATTPEACE_ZONE | GF_ATTPRODUCT_ZONE_PUBLIC | GF_ATTPRODUCT_ZONE_PRIVATE | GF_ATTWAR_AREA | GF_ATTFREEPK_ZONE )
#endif // WORLD_EDITOR			// yjpark     -->|

// texture combinations for max 4 texture units (fog, haze and selection not included)
#ifdef	WORLD_EDITOR			// yjpark |<--
	#define GF_TX0_TX1         (1L<<20)
	#define GF_TX0_TX2         (1L<<21)
	#define GF_TX0_SHD         (1L<<22)
	#define GF_TX2_SHD         (1L<<23)  // second pass
	#define GF_TX0_TX1_TX2     (1L<<24)
	#define GF_TX0_TX1_SHD     (1L<<25)
	#define GF_TX0_TX2_SHD     (1L<<26)
	#define GF_TX0_TX1_TX2_SHD (1L<<27)
#else
	#define GF_TX0_TX1         (1L<<10)
	#define GF_TX0_TX2         (1L<<11)
	#define GF_TX0_SHD         (1L<<12)
	#define GF_TX2_SHD         (1L<<13)  // second pass
	#define GF_TX0_TX1_TX2     (1L<<14)
	#define GF_TX0_TX1_SHD     (1L<<15)
	#define GF_TX0_TX2_SHD     (1L<<16)
	#define GF_TX0_TX1_TX2_SHD (1L<<17)
#endif	// WORLD_EDITOR			// yjpark     -->|

// total number of groups
#ifdef	WORLD_EDITOR			// yjpark |<--
	#define GROUPS_MAXCOUNT (1L<<20)   // max group +1 !		// yjpark
#else
	#define GROUPS_MAXCOUNT (1L<<10)   // max group +1 !		// yjpark
#endif	// WORLD_EDITOR			// yjpark     -->|

#define GROUPS_MINCOUNT (1L<<4)-1  // min group !
static ScenePolygon *_apspoGroups[GROUPS_MAXCOUNT];
static INDEX _aiGroupVtx[GROUPS_MAXCOUNT];  // total number of vertices in groups
static INDEX _ctGroupsCount=0;


// some static vars

static FLOAT _fHazeMul, _fHazeAdd;
static FLOAT _fFogMul;
static FLOAT _fTmp; // for haze/fog to avoid reading from vertex buffer array

static COLOR _colSelection;
static INDEX _ctGroupVtx;            // vertices in this group
static INDEX _ctUsableTexUnits;
static BOOL  _bTranslucentPass;      // rendering translucent polygons

static ULONG _ulLastFlags[GFX_USETEXUNITS];
static ULONG _ulLastBlends[GFX_USETEXUNITS];
static INDEX _iLastFrameNo[GFX_USETEXUNITS];
static CTextureData *_ptdLastTex[GFX_USETEXUNITS];

static CDrawPort *_pDP;
static CPerspectiveProjection3D *_ppr = NULL;


// draw batched elements
static void FlushElements(void) 
{
	// skip if empty
	const INDEX ctElements = _auwElements.Count();
	if( ctElements<3) return;

//강동민 수정 시작 테스트 클라이언트 작업	06.29
	extern INDEX gfx_bRenderReflection;
	if(gfx_bRenderReflection)
	{
		_sfStats.IncrementCounter( CStatForm::SCI_REFLECTION_TRI, ctElements);
	}
//강동민 수정 끝 테스트 클라이언트 작업		06.29
	// draw
	_sfStats.IncrementCounter( CStatForm::SCI_SCENE_TRIANGLEPASSES, ctElements);
	_pGfx->gl_ctWorldElements += ctElements; 
	gfxDrawElements( ctElements, &_auwElements[0]);
	// reset
	_auwElements.PopAll();
}


// batch elements of one polygon
__forceinline void AddElements( ScenePolygon *pspo) 
{
	ASSERT( pspo->spo_iVtx0Pass < 32768);
	const INDEX ctElems = pspo->spo_ctElements;
	UWORD *puwDst = _auwElements.Push(ctElems);

#if ASMOPT == 1

	__asm {
		mov     eax,D [pspo]
		mov     ecx,D [ctElems]
		mov     edi,D [puwDst]
		mov     esi,D [eax]ScenePolygon.spo_piElements
		mov     edx,D [eax]ScenePolygon.spo_iVtx0Pass
		shr     ecx,2
		jz      elemRest2

		movd    mm7,edx
	punpcklwd mm7,mm7
	punpckldq mm7,mm7
elemLoop:
		movq    mm1,Q [esi+0]
		movq    mm2,Q [esi+8]
	 packssdw mm1,mm2
		paddd   mm1,mm7
		movq    Q [edi],mm1
		add     esi,4*4
		add     edi,4*2
		dec     ecx
		jnz     elemLoop
		emms

elemRest2:
		test    [ctElems],2
		jz      elemRest1
		mov     eax,D [esi+0]
		mov     ecx,D [esi+4]
		add     eax,edx
		add     ecx,edx
		shl     ecx,16
		or      eax,ecx
		mov     D [edi],eax
		add     esi,4*2
		add     edi,2*2

elemRest1:
		test    [ctElems],1
		jz      elemDone
		mov     eax,D [esi]
		add     eax,edx
		mov     W [edi],ax
elemDone:
	}

#else

	const INDEX iVtx0Pass = pspo->spo_iVtx0Pass;
	const INDEX *piSrc = pspo->spo_piElements;
	for( INDEX iElem=0; iElem<ctElems; iElem++) {
		// make an element in per-pass arrays
		puwDst[iElem] = piSrc[iElem]+iVtx0Pass;
	}

#endif
}


// draw all elements of one pass
static __forceinline void DrawAllElements( ScenePolygon *pspoFirst) 
{
	ASSERT( _auwElements.Count()==0);
	for( ScenePolygon *pspo=pspoFirst; pspo!=NULL; pspo=pspo->spo_pspoSucc) { 
		_sfStats.IncrementCounter( CStatForm::SCI_SCENE_TRIANGLEPASSES, pspo->spo_ctElements);
		_pGfx->gl_ctWorldElements += pspo->spo_ctElements; 
		AddElements(pspo);
	}
	FlushElements();
}


// calculate mip factor for a texture and adjust its mapping vectors
static BOOL RSMakeMipFactorAndAdjustMapping( ScenePolygon *pspo, INDEX iLayer)
{
	BOOL bRemoved = false;
	MEX mexTexSizeU, mexTexSizeV;
	CMappingVectors &mv = pspo->spo_amvMapping[iLayer];

	// texture map ?
	if( iLayer<SHADOWTEXTURE)
	{ 
		const ULONG ulBlend = pspo->spo_aubTextureFlags[iLayer] & STXF_BLEND_MASK;
		CTextureData *ptd = (CTextureData*)pspo->spo_aptoTextures[iLayer]->GetData();
		mexTexSizeU = ptd->GetWidth();
		mexTexSizeV = ptd->GetHeight();

		// check whether detail can be rejected (but don't reject colorized textures)
		if( ulBlend==STXF_BLEND_SHADE && (ptd->td_ulFlags&TEX_EQUALIZED)
		&& (pspo->spo_acolColors[iLayer]&0xFFFFFF00)==0xFFFFFF00)
		{ // get nearest vertex Z distance from viewer and u and v steps
			const FLOAT fZ = pspo->spo_fNearestZ;
			const FLOAT f1oPZ1 = fZ / _ppr->ppr_PerspectiveRatios(1);
			const FLOAT f1oPZ2 = fZ / _ppr->ppr_PerspectiveRatios(2);
			const FLOAT fDUoDI = Abs( mv.mv_vU(1) *f1oPZ1);
			const FLOAT fDUoDJ = Abs( mv.mv_vU(2) *f1oPZ2);
			const FLOAT fDVoDI = Abs( mv.mv_vV(1) *f1oPZ1);
			const FLOAT fDVoDJ = Abs( mv.mv_vV(2) *f1oPZ2);
			// find mip factor and adjust removing of texture layer
			const FLOAT fMaxDoD    = Max( Max(fDUoDI,fDUoDJ), Max(fDVoDI,fDVoDJ));
			const INDEX iMipFactor = wld_iDetailRemovingBias + (((SLONG&)fMaxDoD)>>23) -127 +10;
			const INDEX iLastMip   = ptd->td_iFirstMipLevel + ptd->GetNoOfMips() -1; // determine last mipmap in texture
			bRemoved = (iMipFactor>=iLastMip);
			// check for detail texture showing
			extern INDEX wld_bShowDetailTextures;
			if( wld_bShowDetailTextures) {
				if( iLayer==2) pspo->spo_acolColors[iLayer] = C_MAGENTA|255;
				else           pspo->spo_acolColors[iLayer] = C_CYAN   |255;
			}
		}
		// check if texture has been blended with low alpha 
		else bRemoved = (ulBlend==STXF_BLEND_ALPHA) && ((pspo->spo_acolColors[iLayer]&CT_AMASK)>>CT_ASHIFT)<3;
	}

	// shadow map
	else
	{ 
		mexTexSizeU = pspo->spo_psmShadowMap->sm_mexWidth;
		mexTexSizeV = pspo->spo_psmShadowMap->sm_mexHeight;
	}

	// adjust texture gradients
	if( mexTexSizeU!=1024) {
		const FLOAT fMul = 1024.0f /mexTexSizeU;  // (no need to do shift-opt, because it won't speed up much!)
		mv.mv_vU(1) *=fMul;  mv.mv_vU(2) *=fMul;  mv.mv_vU(3) *=fMul;
	}
	if( mexTexSizeV!=1024) {
		const FLOAT fMul = 1024.0f /mexTexSizeV;
		mv.mv_vV(1) *=fMul;  mv.mv_vV(2) *=fMul;  mv.mv_vV(3) *=fMul;
	}

	// all done
	return bRemoved;
}



// Remove all polygons with no triangles from a list
static void RSRemoveDummyPolygons( ScenePolygon *pspoAll, ScenePolygon **ppspoNonDummy)
{
	*ppspoNonDummy = NULL;
	// for all span polygons in list (remember one ahead to be able to reconnect them)
	ScenePolygon *pspoNext;
	for( ScenePolygon *pspoThis=pspoAll; pspoThis!=NULL; pspoThis=pspoNext) {
		pspoNext = pspoThis->spo_pspoSucc;
		// if the polygon has some triangles
		if( pspoThis->spo_ctElements >0) {
			// move it to the other list
			pspoThis->spo_pspoSucc = *ppspoNonDummy;
			*ppspoNonDummy = pspoThis;
		}
	}
}


// bin polygons into groups
static void RSBinToGroups( ScenePolygon *pspoFirst)
{
	// clamp texture layers
	extern INDEX wld_bTextureLayers;
	BOOL bTextureLayer1 =(wld_bTextureLayers /100) || _bMultiPlayer; // must be enabled in multiplayer mode!
	BOOL bTextureLayer2 = wld_bTextureLayers /10 %10;
	BOOL bTextureLayer3 = wld_bTextureLayers %10;
	wld_bTextureLayers  = 0;
	if( bTextureLayer1) wld_bTextureLayers += 100;
	if( bTextureLayer2) wld_bTextureLayers += 10;
	if( bTextureLayer3) wld_bTextureLayers += 1;

	// cache rendering states
	bTextureLayer1 = bTextureLayer1 && wld_bRenderTextures;
	bTextureLayer2 = bTextureLayer2 && wld_bRenderTextures;
	bTextureLayer3 = bTextureLayer3 && wld_bRenderTextures;

	// clear all groups initially
	StoreLongs( 0, (ULONG*)_apspoGroups, sizeof(_apspoGroups)/sizeof(ULONG));
	StoreLongs( 0, (ULONG*)_aiGroupVtx,  sizeof(_aiGroupVtx) /sizeof(ULONG));
	_ctGroupsCount = GROUPS_MINCOUNT;

	// for all span polygons in list (remember one ahead to be able to reconnect them)
	for( ScenePolygon *pspoNext, *pspo=pspoFirst; pspo!=NULL; pspo=pspoNext)
	{
		pspoNext = pspo->spo_pspoSucc;
		const INDEX ctTris = pspo->spo_ctElements/3;
		ULONG ulBits = NONE;

		// if it has texture 1 active
		if( pspo->spo_aptoTextures[0]!=NULL && bTextureLayer1) {
			// prepare mapping for texture 0 and generate its mip factor
			const BOOL bRemoved = RSMakeMipFactorAndAdjustMapping( pspo, 0);
			if( !bRemoved) ulBits |= GF_TX0; // add if not removed
		} else {
			// flat fill is mutually exclusive with texture layer0
			_ctGroupsCount |= GF_FLAT;
			ulBits |= GF_FLAT; 
		}

		// if it has texture 2 active
		if( pspo->spo_aptoTextures[1]!=NULL && bTextureLayer2) {
			// prepare mapping for texture 1 and generate its mip factor
			const BOOL bRemoved = RSMakeMipFactorAndAdjustMapping( pspo, 1);
			if( !bRemoved) { // add if not removed
				if( pspo->spo_aubTextureFlags[1] & STXF_AFTERSHADOW) {
					_ctGroupsCount |= GF_TA1;
					ulBits |= GF_TA1;
				} else {
					ulBits |= GF_TX1;
				}
			}
		}
		// if it has texture 3 active
		if( pspo->spo_aptoTextures[2]!=NULL && bTextureLayer3) {
			// prepare mapping for texture 2 and generate its mip factor
			const BOOL bRemoved = RSMakeMipFactorAndAdjustMapping( pspo, 2);
			if( !bRemoved) { // add if not removed
				if( pspo->spo_aubTextureFlags[2] & STXF_AFTERSHADOW) {
					_ctGroupsCount |= GF_TA2;
					ulBits |= GF_TA2;
				} else {
					ulBits |= GF_TX2;
				}
			}
		}

		// if it has shadowmap active
		if( pspo->spo_psmShadowMap!=NULL && wld_bRenderShadowMaps) {
			// prepare shadow map
			CShadowMap *psmShadow = pspo->spo_psmShadowMap;
			psmShadow->Prepare();
			const BOOL bFlat = psmShadow->IsFlat();
			COLOR colFlat = psmShadow->sm_colFlat; 

			// don't allow combining with 1st layer if texture layers are not in shade mode
			BOOL bOverbright;
			if(  (ulBits&GF_FLAT)
			 || ((ulBits&GF_TX1) && (pspo->spo_aubTextureFlags[1]&STXF_BLEND_MASK)!=STXF_BLEND_SHADE)
			 || ((ulBits&GF_TX2) && (pspo->spo_aubTextureFlags[2]&STXF_BLEND_MASK)!=STXF_BLEND_SHADE)) {
				bOverbright = TRUE;
			} else {
				bOverbright = (colFlat & 0x80808000);
			}

			// only need to update poly color if shadowmap is flat
			if( bFlat) {
				if( !bOverbright || shd_iForceFlats==1) {
					if( shd_bShowFlats) colFlat = C_mdMAGENTA; // show flat shadows?
					else { // enhance light color to emulate overbrighting
						if( !bOverbright) colFlat<<=1;
						else {
							UBYTE ubR,ubG,ubB;
							ColorToRGB( colFlat, ubR,ubG,ubB);
							ULONG ulR = ClampUp( ((ULONG)ubR)<<1, 255UL);
							ULONG ulG = ClampUp( ((ULONG)ubG)<<1, 255UL);
							ULONG ulB = ClampUp( ((ULONG)ubB)<<1, 255UL);
							colFlat = RGBToColor(ulR,ulG,ulB);
						}
					} // mix color in the first texture layer
					COLOR &colTotal = pspo->spo_acolColors[0];
					COLOR  colLayer = pspo->spo_acolColors[3];
					if( colTotal==0xFFFFFFFF) colTotal = colLayer;
					else if( colLayer!=0xFFFFFFFF) colTotal = MulColors( colTotal, colLayer);
					colFlat |= CT_OPAQUE;
					if(  colTotal==0xFFFFFFFF) colTotal = colFlat;
					else colTotal = MulColors( colTotal,  colFlat);
					psmShadow->MarkDrawn();
				}
				else {
					// need to update poly color if shadowmap is flat and overbrightened
					COLOR &colTotal = pspo->spo_acolColors[3];
					if( shd_bShowFlats) colFlat = C_mdBLUE; // overbrightened!
					if(  colTotal==0xFFFFFFFF) colTotal = colFlat;
					else colTotal = MulColors( colTotal,  colFlat);
					ulBits |= GF_SHD; // mark the need for shadow layer
				} 
			} else {
				// prepare mapping for shadowmap and generate its mip factor
				RSMakeMipFactorAndAdjustMapping( pspo, SHADOWTEXTURE);
				ulBits |= GF_SHD;
			}
		}

		// if it has fog active
		if( pspo->spo_ulFlags&SPOF_RENDERFOG) {
			_ctGroupsCount |= GF_FOG;
			ulBits |= GF_FOG;
		}
		// if it has haze active
		if( pspo->spo_ulFlags&SPOF_RENDERHAZE) {
			_ctGroupsCount |= GF_HAZE;
			ulBits |= GF_HAZE;
		}

#ifdef	WORLD_EDITOR			// yjpark |<--
		// if it is selected
		if( pspo->spo_ulFlags&SPOF_SELECTED )
		{
			_ctGroupsCount |= GF_SEL;
			ulBits |= GF_SEL;
		}
		else if( pspo->spo_ulFlags & SPOF_ATTRIBUTE )	// If it has the attribute
		{
			if( pspo->spo_ulFlags & SPOF_ATTWALKABLE_EVEN )				// Walkable
			{
				_ctGroupsCount |= GF_ATTWALK_EVEN;
				ulBits |= GF_ATTWALK_EVEN;
			}
			else if( pspo->spo_ulFlags & SPOF_ATTWALKABLE_ODD )			// Walkable
			{
				_ctGroupsCount |= GF_ATTWALK_ODD;
				ulBits |= GF_ATTWALK_ODD;
			}
			else if( pspo->spo_ulFlags & SPOF_ATTUNWALKABLE_EVEN )		// Unwalkable
			{
				_ctGroupsCount |= GF_ATTUNWALK_EVEN;
				ulBits |= GF_ATTUNWALK_EVEN;
			}
			else if( pspo->spo_ulFlags & SPOF_ATTUNWALKABLE_ODD )		// Unwalkable
			{
				_ctGroupsCount |= GF_ATTUNWALK_ODD;
				ulBits |= GF_ATTUNWALK_ODD;
			}
			else if( pspo->spo_ulFlags & SPOF_ATTSTAIR )				// Stair
			{
				_ctGroupsCount |= GF_ATTSTAIR;
				ulBits |= GF_ATTSTAIR;
			}
			else if( pspo->spo_ulFlags & SPOF_ATTSTAIRWALL )			// Wall of stair
			{
				_ctGroupsCount |= GF_ATTSTAIRWALL;
				ulBits |= GF_ATTSTAIRWALL;
			}
			else if( pspo->spo_ulFlags & SPOF_ATTPEACE_ZONE )			// Peace zone
			{
				_ctGroupsCount |= GF_ATTPEACE_ZONE;
				ulBits |= GF_ATTPEACE_ZONE;
			}
			else if( pspo->spo_ulFlags & SPOF_ATTPRODUCT_ZONE_PUBLIC )	// Product zone - public
			{
				_ctGroupsCount |= GF_ATTPRODUCT_ZONE_PUBLIC;
				ulBits |= GF_ATTPRODUCT_ZONE_PUBLIC;
			}
			else if( pspo->spo_ulFlags & SPOF_ATTPRODUCT_ZONE_PRIVATE )	// Product zone - private
			{
				_ctGroupsCount |= GF_ATTPRODUCT_ZONE_PRIVATE;
				ulBits |= GF_ATTPRODUCT_ZONE_PRIVATE;
			}
			else if( pspo->spo_ulFlags & SPOF_ATTWAR_AREA )				// War Area
			{
				_ctGroupsCount |= GF_ATTWAR_AREA;
				ulBits |= GF_ATTWAR_AREA;
			}
			else if( pspo->spo_ulFlags & SPOF_FREEPK_ZONE )				// Free PK Zone
			{
				_ctGroupsCount |= GF_ATTFREEPK_ZONE;
				ulBits |= GF_ATTFREEPK_ZONE;
			}
		}
		
#endif	// WORLD_EDITOR			// yjpark     -->|

		// if it is transparent
		if( pspo->spo_ulFlags&SPOF_TRANSPARENT) {
			_ctGroupsCount |= GF_KEY;
			ulBits |= GF_KEY;
		}

		// in case of at least one layer, add it to proper group
		if( ulBits) {
			pspo->spo_pspoSucc   = _apspoGroups[ulBits];
			_apspoGroups[ulBits] = pspo;
			_aiGroupVtx[ulBits] += pspo->spo_ctVtx;
		}
	}

	// determine maximum used groups
	ASSERT( _ctGroupsCount);
	__asm {
		mov     eax,2
		bsr     ecx,D [_ctGroupsCount]
		shl     eax,cl
		mov     D [_ctGroupsCount],eax
	}

	// done
	_ctGroupVtx = 0;
}



// bin polygons that can use dual-texturing
static void RSBinByDualTexturing( ScenePolygon *pspoGroup, INDEX iLayer1, INDEX iLayer2,
																	ScenePolygon **ppspoST, ScenePolygon **ppspoMT,
																	INDEX &ctVtxST, INDEX &ctVtxMT)
{
	*ppspoST = NULL;
	*ppspoMT = NULL;
	ctVtxST  = 0;
	ctVtxMT  = 0;
	// for all span polygons in list (remember one ahead to be able to reconnect them)
	for( ScenePolygon *pspoNext, *pspo=pspoGroup; pspo!=NULL; pspo=pspoNext)
	{
		pspoNext = pspo->spo_pspoSucc;
		// if first texture is opaque or shade and second layer is shade
		if( ((pspo->spo_aubTextureFlags[iLayer1]&STXF_BLEND_MASK)==STXF_BLEND_OPAQUE
		 ||  (pspo->spo_aubTextureFlags[iLayer1]&STXF_BLEND_MASK)==STXF_BLEND_SHADE) 
		 &&  (pspo->spo_aubTextureFlags[iLayer2]&STXF_BLEND_MASK)==STXF_BLEND_SHADE) {
			// can be merged,  so put to multi-texture
			pspo->spo_pspoSucc = *ppspoMT;
			*ppspoMT = pspo;
			ctVtxMT += pspo->spo_ctVtx;
		} else {
			// cannot be merged, so put to single-texture
			pspo->spo_pspoSucc = *ppspoST;
			*ppspoST = pspo;
			ctVtxST += pspo->spo_ctVtx;
		}
	}
}


// bin polygons that can use triple-texturing
static void RSBinByTripleTexturing( ScenePolygon *pspoGroup, INDEX iLayer3,
									ScenePolygon **ppspoST, ScenePolygon **ppspoMT,
									INDEX &ctVtxST, INDEX &ctVtxMT)
{
	*ppspoST = NULL;
	*ppspoMT = NULL;
	ctVtxST  = 0;
	ctVtxMT  = 0;
	// for all span polygons in list (remember one ahead to be able to reconnect them)
	for( ScenePolygon *pspoNext, *pspo=pspoGroup; pspo!=NULL; pspo=pspoNext)
	{
		pspoNext = pspo->spo_pspoSucc;
		// if texture is shade and colors allow merging
		if( (pspo->spo_aubTextureFlags[iLayer3]&STXF_BLEND_MASK)==STXF_BLEND_SHADE) {
			// can be merged, so put to multi-texture
			pspo->spo_pspoSucc = *ppspoMT;
			*ppspoMT = pspo;
			ctVtxMT += pspo->spo_ctVtx;
		} else {
			// cannot be merged, so put to single-texture
			pspo->spo_pspoSucc = *ppspoST;
			*ppspoST = pspo;
			ctVtxST += pspo->spo_ctVtx;
		}
	}
}


// check if all layers in all shadow maps are up to date
static void RSCheckLayersUpToDate( ScenePolygon *pspoFirst)
{
	// for all span polygons in list
	for( ScenePolygon *pspo=pspoFirst; pspo!=NULL; pspo=pspo->spo_pspoSucc) {
		if( pspo->spo_psmShadowMap!=NULL) pspo->spo_psmShadowMap->CheckLayersUpToDate();
	}
}


// prepare parameters individual to a polygon texture
inline void RSSetTextureWrapping( ULONG ulFlags)
{
	gfxSetTextureWrapping( (ulFlags&STXF_CLAMPU) ? GFX_CLAMP : GFX_REPEAT,
							 (ulFlags&STXF_CLAMPV) ? GFX_CLAMP : GFX_REPEAT);
}


// prepare parameters individual to a polygon texture
static void RSSetInitialTextureParameters(void)
{
	_ulLastFlags[0]  = STXF_BLEND_OPAQUE;
	_ulLastBlends[0] = STXF_BLEND_OPAQUE;
	_iLastFrameNo[0] = 0;
	_ptdLastTex[0]   = NULL;
	gfxSetTextureModulation(1);
	gfxDisableBlend();
}


static void RSSetTextureParameters( ULONG ulFlags)
{
	// if blend flags have changed
	ULONG ulBlendFlags = ulFlags&STXF_BLEND_MASK;
	if( _ulLastBlends[0] != ulBlendFlags)
	{ // determine new texturing mode
		switch( ulBlendFlags) {
		case STXF_BLEND_OPAQUE: // opaque texturing
			gfxDisableBlend();
			break;
		case STXF_BLEND_ALPHA:  // blend using texture alpha
			gfxEnableBlend();
			gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA); 
			break;
		case STXF_BLEND_ADD:  // add to screen
			gfxEnableBlend();
			gfxBlendFunc( GFX_ONE, GFX_ONE); 
			break;
//안태훈 수정 시작	//(Modify Worldbase Overbright to NonOver)(0.1)
		case STXF_BLEND_SHADE: // screen*texture*2
			gfxEnableBlend();
			gfxBlendFunc( GFX_DST_COLOR, GFX_SRC_COLOR); 
			//gfxBlendFunc( GFX_ZERO, GFX_SRC_COLOR); 
			break;
		case STXF_BLEND_MODIFIED_SHADE:
			gfxEnableBlend();
			gfxBlendFunc( GFX_SRC_ALPHA, GFX_ONE); 
			//gfxBlendFunc( GFX_ZERO, GFX_SRC_COLOR); 
			break;
		default:
			ASSERTALWAYS("블렌드모드가 이상함 이쪽 루틴을 타면 안됨");
//안태훈 수정 끝	//(Modify Worldbase Overbright to NonOver)(0.1)
			break;
		}
		// remember new flags
		_ulLastBlends[0] = ulFlags;
	}
}


// prepare initial parameters for polygon texture
static void RSSetInitialTextureParametersMT(void)
{
	INDEX i;
	// reset bleding modes
	for( i=0; i<GFX_USETEXUNITS; i++) {
		_ulLastFlags[i]  = STXF_BLEND_OPAQUE;
		_ulLastBlends[i] = STXF_BLEND_OPAQUE;
		_iLastFrameNo[i] = 0;
		_ptdLastTex[i]   = NULL;
	}
	// reset for texture
	gfxDisableBlend();
	for( i=1; i<_ctUsableTexUnits; i++) {
		gfxSetTextureUnit(i);
//안태훈 수정 시작	//(Modify Worldbase Overbright to NonOver)(0.1)
		gfxSetTextureModulation(2);
		//gfxSetTextureModulation(1);
//안태훈 수정 끝	//(Modify Worldbase Overbright to NonOver)(0.1)
	}
	gfxSetTextureUnit(0);
	gfxSetTextureModulation(1);
}


// prepare parameters individual to a polygon texture
static void RSSetTextureParametersMT( ULONG ulFlags)
{
	// skip if the same as last time
	const ULONG ulBlendFlags = ulFlags&STXF_BLEND_MASK;
	if( _ulLastBlends[0]==ulBlendFlags) return; 
	// update
	if( ulBlendFlags==STXF_BLEND_OPAQUE) {
		// opaque texturing
		gfxDisableBlend();
	} else {
		// shade texturing
		ASSERT( ulBlendFlags==STXF_BLEND_SHADE);
		gfxEnableBlend();
		gfxBlendFunc( GFX_DST_COLOR, GFX_SRC_COLOR); 
	} // keep
	_ulLastBlends[0] = ulBlendFlags;
}


// make vertex coordinates for all polygons in the group
static void RSMakeVertexCoordinates( ScenePolygon *pspoGroup)
{
	ASSERT( _ctGroupVtx>0);
	GFXVertex *pvtxPass = (GFXVertex*)gfxLockSubBuffer( GFX_VBA_POS, 0, _ctGroupVtx, GFX_WRITE);

	// for all scene polygons in list
	INDEX ctLastVtx = 0;
	for( ScenePolygon *pspo=pspoGroup; pspo!=NULL; pspo=pspo->spo_pspoSucc)
	{ 
		// create new vertices for that polygon in per-pass array
		pspo->spo_iVtx0Pass = ctLastVtx;
		const INDEX ctVtx = pspo->spo_ctVtx;
		const GFXVertex *pvtxScene = &_avtxScene[pspo->spo_iVtx0];
		// copy the vertex coordinates
		CopyLongs( (ULONG*)&pvtxScene[0], (ULONG*)&pvtxPass[ctLastVtx], ctVtx*sizeof(GFXVertex)/sizeof(ULONG));
		ctLastVtx += ctVtx; // add polygon vertices to total
	}
	// check and unlock
	ASSERT( _ctGroupVtx == ctLastVtx); 
	gfxUnlockSubBuffer(GFX_VBA_POS, 0);
}


static void RSSetPolygonColors( ScenePolygon *pspoGroup, UBYTE ubAlpha)
{
	GFXColor *pcolPass = (GFXColor*)gfxLockSubBuffer( GFX_VBA_COL, 0, _ctGroupVtx, GFX_WRITE);
	for( ScenePolygon *pspo = pspoGroup; pspo != NULL; pspo = pspo->spo_pspoSucc) {
		const INDEX iFirstVtx = pspo->spo_iVtx0Pass;
		const INDEX iLastVtx  = iFirstVtx + pspo->spo_ctVtx;
		const ULONG ulCol = ColorAPI(pspo->spo_cColor|ubAlpha);
		for( INDEX iVtx=iFirstVtx; iVtx<iLastVtx; iVtx++) pcolPass[iVtx].abgr = ulCol;
	} // done
	gfxUnlockSubBuffer(GFX_VBA_COL, 0);
}


static void RSSetConstantColors( COLOR col)
{
	GFXColor *pcolPass = (GFXColor*)gfxLockSubBuffer( GFX_VBA_COL, 0, _ctGroupVtx, GFX_WRITE);
	const ULONG ulCol = ColorAPI(col);
	for( INDEX i=0; i<_ctGroupVtx; i++) pcolPass[i].abgr = ulCol;
	gfxUnlockSubBuffer(GFX_VBA_COL, 0);
}


static void RSSetTextureColors( ScenePolygon *pspoGroup, ULONG ulLayerMask)
{
#ifdef	WORLD_EDITOR			// yjpark |<--
	ASSERT( !(ulLayerMask & (GF_TA1|GF_TA2|GF_FOG|GF_HAZE|GF_SEL|GF_ATTRIBUTE)));
#else
	ASSERT( !(ulLayerMask & (GF_TA1|GF_TA2|GF_FOG|GF_HAZE)));
#endif	// WORLD_EDITOR			// yjpark     -->|
	ASSERT( _ctGroupVtx>0);
	GFXColor *pcolPass = (GFXColor*)gfxLockSubBuffer( GFX_VBA_COL, 0, _ctGroupVtx, GFX_WRITE);

	// for all scene polygons in list
	COLOR colLayer, colTotal;
	for( ScenePolygon *pspo = pspoGroup; pspo != NULL; pspo = pspo->spo_pspoSucc)
	{ // adjust hue/saturation and set colors
		const INDEX iFirstVtx = pspo->spo_iVtx0Pass;
		const INDEX iLastVtx  = iFirstVtx + pspo->spo_ctVtx;
		colTotal = C_WHITE|CT_OPAQUE;
		if( ulLayerMask&GF_TX0) {
			colLayer = pspo->spo_acolColors[0];
			if( colLayer!=0xFFFFFFFF) colTotal = MulColors( colTotal, colLayer);
		}
		if( ulLayerMask&GF_TX1) {
			colLayer = pspo->spo_acolColors[1];
			if( colLayer!=0xFFFFFFFF) colTotal = MulColors( colTotal, colLayer);
		}
		if( ulLayerMask&GF_TX2) {
			colLayer = pspo->spo_acolColors[2];
			if( colLayer!=0xFFFFFFFF) colTotal = MulColors( colTotal, colLayer);
		}
		if( ulLayerMask&GF_SHD) {
			colLayer = pspo->spo_acolColors[3];
			if( colLayer!=0xFFFFFFFF) colTotal = MulColors( colTotal, colLayer);
		}
		// store
		colTotal = ColorAPI(colTotal);
		for( INDEX iVtx=iFirstVtx; iVtx<iLastVtx; iVtx++) pcolPass[iVtx].abgr = colTotal;
	}
	// done
	gfxUnlockSubBuffer(GFX_VBA_COL, 0);
}


// make texture coordinates for one texture in all polygons in group
static INDEX _iLastUnit = -1;
static void RSSetTextureCoords( ScenePolygon *pspoGroup, INDEX iLayer, INDEX iUnit)
{
	// eventualy switch texture unit
	if( _iLastUnit != iUnit) {
		gfxSetTextureUnit(iUnit);
		gfxEnableTexture();
		_iLastUnit = iUnit;
	}

	// lock the texture coordinates array
	ASSERT( _ctGroupVtx>0);
	GFXTexCoord *ptexPass = (GFXTexCoord*)gfxLockSubBuffer( GFX_VBA_TEX, 0, _ctGroupVtx, GFX_WRITE);

	// generate tex coord for all scene polygons in list
	const FLOATmatrix3D &mViewer = _ppr->pr_ViewerRotationMatrix;
	const INDEX iMappingOffset = iLayer * sizeof(CMappingVectors);

	for( ScenePolygon *pspo=pspoGroup; pspo!=NULL; pspo=pspo->spo_pspoSucc)
	{
		ASSERT( pspo->spo_ctVtx>0);
		const FLOAT3D &vN = ((CBrushPolygon*)pspo->spo_pvPolygon)->bpo_pbplPlane->bpl_pwplWorking->wpl_plView;
		const GFXVertex   *pvtx = &_avtxScene[pspo->spo_iVtx0];
					GFXTexCoord *ptex = &ptexPass[pspo->spo_iVtx0Pass];
		
		// reflective mapping?
		if( pspo->spo_aubTextureFlags[iLayer] & STXF_REFLECTION) {
			for( INDEX i=0; i<pspo->spo_ctVtx; i++) { 
				const FLOAT fNorm = 1.0f / sqrt(pvtx[i].x*pvtx[i].x + pvtx[i].y*pvtx[i].y + pvtx[i].z*pvtx[i].z);
				const FLOAT fVx = pvtx[i].x *fNorm;
				const FLOAT fVy = pvtx[i].y *fNorm;
				const FLOAT fVz = pvtx[i].z *fNorm;
				const FLOAT fNV = fVx*vN(1) + fVy*vN(2) + fVz*vN(3);
				const FLOAT fRVx = fVx - 2*vN(1)*fNV;
				const FLOAT fRVy = fVy - 2*vN(2)*fNV;
				const FLOAT fRVz = fVz - 2*vN(3)*fNV;
				const FLOAT fRVxT = fRVx*mViewer(1,1) + fRVy*mViewer(2,1) + fRVz*mViewer(3,1);
				const FLOAT fRVzT = fRVx*mViewer(1,3) + fRVy*mViewer(2,3) + fRVz*mViewer(3,3);
				ptex[i].s = fRVxT*0.5f +0.5f;
				ptex[i].t = fRVzT*0.5f +0.5f;
			}
			// advance to next polygon
			continue;
		}

		// diffuse mapping
		const FLOAT3D &vO = pspo->spo_amvMapping[iLayer].mv_vO;

#if ASMOPT == 1
		__asm {
			mov     esi,D [pspo]
			mov     edi,D [iMappingOffset]
			lea     eax,[esi].spo_amvMapping[edi].mv_vO
			lea     ebx,[esi].spo_amvMapping[edi].mv_vU
			lea     ecx,[esi].spo_amvMapping[edi].mv_vV
			mov     edx,D [esi].spo_ctVtx
			mov     esi,D [pvtx]
			mov     edi,D [ptex]
vtxLoop:
			fld     D [ebx+0]
			fld     D [esi]GFXVertex.x
			fsub    D [eax+0]
			fmul    st(1),st(0)
			fmul    D [ecx+0]   // vV(1)*fDX, vU(1)*fDX
			fld     D [ebx+4]
			fld     D [esi]GFXVertex.y
			fsub    D [eax+4]
			fmul    st(1),st(0)
			fmul    D [ecx+4]   // vV(2)*fDY, vU(2)*fDY, vV(1)*fDX, vU(1)*fDX
			fld     D [ebx+8]
			fld     D [esi]GFXVertex.z
			fsub    D [eax+8]
			fmul    st(1),st(0)
			fmul    D [ecx+8]   // vV(3)*fDZ, vU(3)*fDZ, vV(2)*fDY, vU(2)*fDY, vV(1)*fDX, vU(1)*fDX
			fxch    st(5)
			faddp   st(3),st(0) // vU(3)*fDZ, vV(2)*fDY, vU(1)*fDX+vU(2)*fDY, vV(1)*fDX, vV(3)*fDZ
			fxch    st(1)
			faddp   st(3),st(0) // vU(3)*fDZ, vU(1)*fDX+vU(2)*fDY, vV(1)*fDX+vV(2)*fDY, vV(3)*fDZ
			faddp   st(1),st(0) // vU(1)*fDX+vU(2)*fDY+vU(3)*fDZ,  vV(1)*fDX+vV(2)*fDY, vV(3)*fDZ
			fxch    st(1)
			faddp   st(2),st(0) // vU(1)*fDX+vU(2)*fDY+vU(3)*fDZ,  vV(1)*fDX+vV(2)*fDY+vV(3)*fDZ
			fstp    D [edi]GFXTexCoord.s
			fstp    D [edi]GFXTexCoord.t
			add     esi,3*4
			add     edi,2*4
			dec     edx
			jnz     vtxLoop
		}
#else
		const FLOAT3D &vO = pspo->spo_amvMapping[iLayer].mv_vO;
		const FLOAT3D &vU = pspo->spo_amvMapping[iLayer].mv_vU;
		const FLOAT3D &vV = pspo->spo_amvMapping[iLayer].mv_vV;
		for( INDEX i=0; i<pspo->spo_ctVtx; i++) {
			const FLOAT fDX = pvtx[i].x -vO(1);
			const FLOAT fDY = pvtx[i].y -vO(2);
			const FLOAT fDZ = pvtx[i].z -vO(3);
			ptex[i].s = vU(1)*fDX + vU(2)*fDY + vU(3)*fDZ;
			ptex[i].t = vV(1)*fDX + vV(2)*fDY + vV(3)*fDZ;
		}
#endif
	}

	// done
	gfxUnlockSubBuffer(GFX_VBA_TEX, 0);
}



// make fog texture coordinates for all polygons in group
static void RSSetFogCoordinates( ScenePolygon *pspoGroup)
{
	// lock the texture coordinates array
	ASSERT( _ctGroupVtx>0);
	GFXTexCoord *ptexPass = (GFXTexCoord*)gfxLockSubBuffer( GFX_VBA_TEX, 0, _ctGroupVtx, GFX_WRITE);

	// for all scene polygons in list
	for( ScenePolygon *pspo=pspoGroup; pspo!=NULL; pspo=pspo->spo_pspoSucc)
	{
		BOOL bHasFog = FALSE;
		const GFXVertex *pvtx = &_avtxScene[pspo->spo_iVtx0];
				GFXTexCoord *ptex = &ptexPass[pspo->spo_iVtx0Pass];
		for( INDEX i=0; i<pspo->spo_ctVtx; i++) {
			// need fT to avoid read from write-combined memory
			_fTmp = (_fog_vHDirView(1)*pvtx[i].x + _fog_vHDirView(2)*pvtx[i].y
						+  _fog_vHDirView(3)*pvtx[i].z + _fog_fAddH) * _fog_fMulH;
			bHasFog  |= InFog(_fTmp);
			ptex[i].s = pvtx[i].z *_fFogMul;
			ptex[i].t = _fTmp;
		}
		// mark if polygon wasn't touched by fog
		if( !bHasFog) pspo->spo_ulFlags &= ~SPOF_RENDERFOG;
	}
	// done
	gfxUnlockSubBuffer(GFX_VBA_TEX, 0);
}


// make haze texture coordinates for all polygons in group
static void RSSetHazeCoordinates( ScenePolygon *pspoGroup)
{
	// lock the texture coordinates array
	ASSERT( _ctGroupVtx>0);
	GFXTexCoord *ptexPass = (GFXTexCoord*)gfxLockSubBuffer( GFX_VBA_TEX, 0, _ctGroupVtx, GFX_WRITE);

	// for all scene polygons in list
	for( ScenePolygon *pspo=pspoGroup; pspo!=NULL; pspo=pspo->spo_pspoSucc)
	{
		BOOL bHasHaze = FALSE;
		const GFXVertex *pvtx = &_avtxScene[pspo->spo_iVtx0];
				GFXTexCoord *ptex = &ptexPass[pspo->spo_iVtx0Pass];
		for( INDEX i=0; i<pspo->spo_ctVtx; i++) {
			// need fS to avoid read from write-combined memory
			_fTmp = (pvtx[i].z + _fHazeAdd) *_fHazeMul;
			bHasHaze |= InHaze(_fTmp);
			ptex[i].s = _fTmp;
			ptex[i].t = 0;
		}
		// mark if polygon wasn't touched by haze
		if( !bHasHaze) pspo->spo_ulFlags &= ~SPOF_RENDERHAZE;
	}
	// done
	gfxUnlockSubBuffer(GFX_VBA_TEX, 0);
}


// render textures for all triangles in polygon list
static void RSRenderTEX( ScenePolygon *pspoFirst, INDEX iLayer)
{
	RSSetInitialTextureParameters();

	// for all span polygons in list
	for( ScenePolygon *pspo=pspoFirst; pspo!=NULL; pspo=pspo->spo_pspoSucc)
	{
		ASSERT(pspo->spo_aptoTextures[iLayer]!=NULL);
		CTextureData *ptdTextureData = (CTextureData*)pspo->spo_aptoTextures[iLayer]->GetData();
		const INDEX iFrameNo = pspo->spo_aptoTextures[iLayer]->GetFrame();

		if( _ptdLastTex[0]   != ptdTextureData
		 || _ulLastFlags[0]  != pspo->spo_aubTextureFlags[iLayer]
		 || _iLastFrameNo[0] != iFrameNo) {
			// flush
			FlushElements();
			_ptdLastTex[0]   = ptdTextureData;
			_ulLastFlags[0]  = pspo->spo_aubTextureFlags[iLayer];
			_iLastFrameNo[0] = iFrameNo;
			// set texture parameters if needed
			RSSetTextureWrapping(   pspo->spo_aubTextureFlags[iLayer]);
			RSSetTextureParameters( pspo->spo_aubTextureFlags[iLayer]);
			// prepare texture to be used by accelerator
			ptdTextureData->SetAsCurrent(iFrameNo);
		}
		// render all triangles
		AddElements(pspo);
	}
	// flush leftovers
	FlushElements();
}


// render shadows for all triangles in polygon list
static void RSRenderSHD( ScenePolygon *pspoFirst)
{
	RSSetInitialTextureParameters();
	// for all span polygons in list
	for( ScenePolygon *pspo = pspoFirst; pspo != NULL; pspo = pspo->spo_pspoSucc)
	{ 
		// get shadow map for the polygon
		CShadowMap *psmShadow = pspo->spo_psmShadowMap;
		ASSERT( psmShadow!=NULL);  // shadows have been already sorted out

		// set texture parameters if needed
		RSSetTextureWrapping(   pspo->spo_aubTextureFlags[SHADOWTEXTURE]);
		RSSetTextureParameters( pspo->spo_aubTextureFlags[SHADOWTEXTURE]);

		// upload the shadow to accelerator memory
		psmShadow->SetAsCurrent();

		// batch and render triangles
		AddElements(pspo);
		FlushElements();
	}
}


// render texture and shadow for all triangles in polygon list
static void RSRenderTEX_SHD( ScenePolygon *pspoFirst, INDEX iLayer)
{
	RSSetInitialTextureParametersMT();

	// for all span polygons in list
	for( ScenePolygon *pspo=pspoFirst; pspo!=NULL; pspo=pspo->spo_pspoSucc)
	{
		// render batched triangles
		FlushElements();
		ASSERT( pspo->spo_aptoTextures[iLayer]!=NULL && pspo->spo_psmShadowMap!=NULL);

		// upload the shadow to accelerator memory
		gfxSetTextureUnit(1);
		RSSetTextureWrapping( pspo->spo_aubTextureFlags[SHADOWTEXTURE]);
		pspo->spo_psmShadowMap->SetAsCurrent();

		// prepare texture to be used by accelerator
		CTextureData *ptd = (CTextureData*)pspo->spo_aptoTextures[iLayer]->GetData();
		const INDEX iFrameNo = pspo->spo_aptoTextures[iLayer]->GetFrame();
		
		gfxSetTextureUnit(0);
		if( _ptdLastTex[0]!=ptd || _iLastFrameNo[0]!=iFrameNo || _ulLastFlags[0]!=pspo->spo_aubTextureFlags[iLayer]) {
			_ptdLastTex[0]=ptd;  _iLastFrameNo[0]=iFrameNo;  _ulLastFlags[0]=pspo->spo_aubTextureFlags[iLayer];
			RSSetTextureWrapping( pspo->spo_aubTextureFlags[iLayer]);
			ptd->SetAsCurrent(iFrameNo);
			// set rendering parameters if needed
			RSSetTextureParametersMT( pspo->spo_aubTextureFlags[iLayer]);
		}
		// batch triangles
		AddElements(pspo);
	}

	// flush leftovers
	FlushElements();
}


// render two textures for all triangles in polygon list
static void RSRender2TEX( ScenePolygon *pspoFirst, INDEX iLayer2)
{
	RSSetInitialTextureParametersMT();

	// for all span polygons in list
	for( ScenePolygon *pspo=pspoFirst; pspo!=NULL; pspo=pspo->spo_pspoSucc)
	{
		ASSERT( pspo->spo_aptoTextures[0]!=NULL && pspo->spo_aptoTextures[iLayer2]!=NULL);
		CTextureData *ptd0 = (CTextureData*)pspo->spo_aptoTextures[0]->GetData();
		CTextureData *ptd1 = (CTextureData*)pspo->spo_aptoTextures[iLayer2]->GetData();
		const INDEX iFrameNo0 = pspo->spo_aptoTextures[0]->GetFrame();
		const INDEX iFrameNo1 = pspo->spo_aptoTextures[iLayer2]->GetFrame();

		if( _ptdLastTex[0]!=ptd0 || _iLastFrameNo[0]!=iFrameNo0 || _ulLastFlags[0]!=pspo->spo_aubTextureFlags[0]
		 || _ptdLastTex[1]!=ptd1 || _iLastFrameNo[1]!=iFrameNo1 || _ulLastFlags[1]!=pspo->spo_aubTextureFlags[iLayer2]) {
			FlushElements();
			_ptdLastTex[0]=ptd0;  _iLastFrameNo[0]=iFrameNo0;  _ulLastFlags[0]=pspo->spo_aubTextureFlags[0];
			_ptdLastTex[1]=ptd1;  _iLastFrameNo[1]=iFrameNo1;  _ulLastFlags[1]=pspo->spo_aubTextureFlags[iLayer2];
			// upload the second texture to unit 1
			gfxSetTextureUnit(1);
			RSSetTextureWrapping( pspo->spo_aubTextureFlags[iLayer2]);
			ptd1->SetAsCurrent(iFrameNo1);
			// upload the first texture to unit 0
			gfxSetTextureUnit(0);
			RSSetTextureWrapping( pspo->spo_aubTextureFlags[0]);
			ptd0->SetAsCurrent(iFrameNo0);
			// set rendering parameters if needed
			RSSetTextureParametersMT( pspo->spo_aubTextureFlags[0]);
		}
		// render all triangles
		AddElements(pspo);
	}

	// flush leftovers
	FlushElements();
}



// render two textures and shadowmap for all triangles in polygon list
static void RSRender2TEX_SHD( ScenePolygon *pspoFirst, INDEX iLayer2)
{
	RSSetInitialTextureParametersMT();

	// for all span polygons in list
	for( ScenePolygon *pspo=pspoFirst; pspo!=NULL; pspo=pspo->spo_pspoSucc)
	{
		ASSERT( pspo->spo_aptoTextures[0]!=NULL
				 && pspo->spo_aptoTextures[iLayer2]!=NULL
				 && pspo->spo_psmShadowMap!=NULL);

		// render batched triangles
		FlushElements();

		// upload the shadow to accelerator memory
		gfxSetTextureUnit(2);
		RSSetTextureWrapping( pspo->spo_aubTextureFlags[SHADOWTEXTURE]);
		pspo->spo_psmShadowMap->SetAsCurrent();

		// prepare textures to be used by accelerator
		CTextureData *ptd0 = (CTextureData*)pspo->spo_aptoTextures[0]->GetData();
		CTextureData *ptd1 = (CTextureData*)pspo->spo_aptoTextures[iLayer2]->GetData();
		const INDEX iFrameNo0 = pspo->spo_aptoTextures[0]->GetFrame();
		const INDEX iFrameNo1 = pspo->spo_aptoTextures[iLayer2]->GetFrame();

		gfxSetTextureUnit(0);
		if( _ptdLastTex[0]!=ptd0 || _iLastFrameNo[0]!=iFrameNo0 || _ulLastFlags[0]!=pspo->spo_aubTextureFlags[0]
		 || _ptdLastTex[1]!=ptd1 || _iLastFrameNo[1]!=iFrameNo1 || _ulLastFlags[1]!=pspo->spo_aubTextureFlags[iLayer2]) {
			_ptdLastTex[0]=ptd0;  _iLastFrameNo[0]=iFrameNo0;  _ulLastFlags[0]=pspo->spo_aubTextureFlags[0];      
			_ptdLastTex[1]=ptd1;  _iLastFrameNo[1]=iFrameNo1;  _ulLastFlags[1]=pspo->spo_aubTextureFlags[iLayer2];
			// upload the second texture to unit 1
			gfxSetTextureUnit(1);
			RSSetTextureWrapping( pspo->spo_aubTextureFlags[iLayer2]);
			ptd1->SetAsCurrent(iFrameNo1);
			// upload the first texture to unit 0
			gfxSetTextureUnit(0);
			RSSetTextureWrapping( pspo->spo_aubTextureFlags[0]);
			ptd0->SetAsCurrent(iFrameNo0);
			// set rendering parameters if needed
			RSSetTextureParametersMT( pspo->spo_aubTextureFlags[0]);
		}
		// render all triangles
		AddElements(pspo);
	}

	// flush leftovers
	FlushElements();
}



// render three textures for all triangles in polygon list
static void RSRender3TEX( ScenePolygon *pspoFirst)
{
	RSSetInitialTextureParametersMT();

	// for all span polygons in list
	for( ScenePolygon *pspo=pspoFirst; pspo!=NULL; pspo=pspo->spo_pspoSucc)
	{
		ASSERT( pspo->spo_aptoTextures[0]!=NULL
				 && pspo->spo_aptoTextures[1]!=NULL
				 && pspo->spo_aptoTextures[2]!=NULL);
		CTextureData *ptd0 = (CTextureData*)pspo->spo_aptoTextures[0]->GetData();
		CTextureData *ptd1 = (CTextureData*)pspo->spo_aptoTextures[1]->GetData();
		CTextureData *ptd2 = (CTextureData*)pspo->spo_aptoTextures[2]->GetData();
		const INDEX iFrameNo0 = pspo->spo_aptoTextures[0]->GetFrame();
		const INDEX iFrameNo1 = pspo->spo_aptoTextures[1]->GetFrame();
		const INDEX iFrameNo2 = pspo->spo_aptoTextures[2]->GetFrame();

		if( _ptdLastTex[0]!=ptd0 || _iLastFrameNo[0]!=iFrameNo0 || _ulLastFlags[0]!=pspo->spo_aubTextureFlags[0]
		 || _ptdLastTex[1]!=ptd1 || _iLastFrameNo[1]!=iFrameNo1 || _ulLastFlags[1]!=pspo->spo_aubTextureFlags[1]
		 || _ptdLastTex[2]!=ptd2 || _iLastFrameNo[2]!=iFrameNo2 || _ulLastFlags[2]!=pspo->spo_aubTextureFlags[2]) {
			FlushElements();
			_ptdLastTex[0]=ptd0;  _iLastFrameNo[0]=iFrameNo0;  _ulLastFlags[0]=pspo->spo_aubTextureFlags[0];      
			_ptdLastTex[1]=ptd1;  _iLastFrameNo[1]=iFrameNo1;  _ulLastFlags[1]=pspo->spo_aubTextureFlags[1];
			_ptdLastTex[2]=ptd2;  _iLastFrameNo[2]=iFrameNo2;  _ulLastFlags[2]=pspo->spo_aubTextureFlags[2];
			// upload the third texture to unit 2
			gfxSetTextureUnit(2);
			RSSetTextureWrapping( pspo->spo_aubTextureFlags[2]);
			ptd2->SetAsCurrent(iFrameNo2);
			// upload the second texture to unit 1
			gfxSetTextureUnit(1);
			RSSetTextureWrapping( pspo->spo_aubTextureFlags[1]);
			ptd1->SetAsCurrent(iFrameNo1);
			// upload the first texture to unit 0
			gfxSetTextureUnit(0);
			RSSetTextureWrapping( pspo->spo_aubTextureFlags[0]);
			ptd0->SetAsCurrent(iFrameNo0);
			// set rendering parameters if needed
			RSSetTextureParametersMT( pspo->spo_aubTextureFlags[0]);
		}
		// render all triangles
		AddElements(pspo);
	}

	// flush leftovers
	FlushElements();
}


// render three textures and shadowmap for all triangles in polygon list
static void RSRender3TEX_SHD( ScenePolygon *pspoFirst)
{
	RSSetInitialTextureParametersMT();

	// for all span polygons in list
	for( ScenePolygon *pspo=pspoFirst; pspo!=NULL; pspo=pspo->spo_pspoSucc)
	{
		ASSERT( pspo->spo_aptoTextures[0]!=NULL
				 && pspo->spo_aptoTextures[1]!=NULL
				 && pspo->spo_aptoTextures[2]!=NULL
				 && pspo->spo_psmShadowMap!=NULL);

		// render batched triangles
		FlushElements();

		// upload the shadow to accelerator memory
		gfxSetTextureUnit(3);
		RSSetTextureWrapping( pspo->spo_aubTextureFlags[SHADOWTEXTURE]);
		pspo->spo_psmShadowMap->SetAsCurrent();

		// prepare textures to be used by accelerator
		CTextureData *ptd0 = (CTextureData*)pspo->spo_aptoTextures[0]->GetData();
		CTextureData *ptd1 = (CTextureData*)pspo->spo_aptoTextures[1]->GetData();
		CTextureData *ptd2 = (CTextureData*)pspo->spo_aptoTextures[2]->GetData();
		const INDEX iFrameNo0 = pspo->spo_aptoTextures[0]->GetFrame();
		const INDEX iFrameNo1 = pspo->spo_aptoTextures[1]->GetFrame();
		const INDEX iFrameNo2 = pspo->spo_aptoTextures[2]->GetFrame();

		gfxSetTextureUnit(0);
		if( _ptdLastTex[0]!=ptd0 || _iLastFrameNo[0]!=iFrameNo0 || _ulLastFlags[0]!=pspo->spo_aubTextureFlags[0]
		 || _ptdLastTex[1]!=ptd1 || _iLastFrameNo[1]!=iFrameNo1 || _ulLastFlags[1]!=pspo->spo_aubTextureFlags[1]
		 || _ptdLastTex[2]!=ptd2 || _iLastFrameNo[2]!=iFrameNo2 || _ulLastFlags[2]!=pspo->spo_aubTextureFlags[2]) {
			_ptdLastTex[0]=ptd0;  _iLastFrameNo[0]=iFrameNo0;  _ulLastFlags[0]=pspo->spo_aubTextureFlags[0];      
			_ptdLastTex[1]=ptd1;  _iLastFrameNo[1]=iFrameNo1;  _ulLastFlags[1]=pspo->spo_aubTextureFlags[1];
			_ptdLastTex[2]=ptd2;  _iLastFrameNo[2]=iFrameNo2;  _ulLastFlags[2]=pspo->spo_aubTextureFlags[2];
			// upload the third texture to unit 2
			gfxSetTextureUnit(2);
			RSSetTextureWrapping( pspo->spo_aubTextureFlags[2]);
			ptd2->SetAsCurrent(iFrameNo2);
			// upload the second texture to unit 1
			gfxSetTextureUnit(1);
			RSSetTextureWrapping( pspo->spo_aubTextureFlags[1]);
			ptd1->SetAsCurrent(iFrameNo1);
			// upload the first texture to unit 0
			gfxSetTextureUnit(0);
			RSSetTextureWrapping( pspo->spo_aubTextureFlags[0]);
			ptd0->SetAsCurrent(iFrameNo0);
			// set rendering parameters if needed
			RSSetTextureParametersMT( pspo->spo_aubTextureFlags[0]);
		}
		// render all triangles
		AddElements(pspo);
	}

	// flush leftovers
	FlushElements();
}


// render fog for all affected triangles in polygon list
__forceinline void RSRenderFog( ScenePolygon *pspoFirst)
{
	// for all scene polygons in list
	for( ScenePolygon *pspo=pspoFirst; pspo!=NULL; pspo=pspo->spo_pspoSucc)
	{ // add only polygon that has fog
		if( pspo->spo_ulFlags&SPOF_RENDERFOG) AddElements(pspo);
	} // done
	FlushElements();
}


// render haze for all affected triangles in polygon list
__forceinline void RSRenderHaze( ScenePolygon *pspoFirst)
{
	// for all scene polygons in list
	for( ScenePolygon *pspo=pspoFirst; pspo!=NULL; pspo=pspo->spo_pspoSucc)
	{ // add only polygon that has haze
		if( pspo->spo_ulFlags&SPOF_RENDERHAZE) AddElements(pspo);
	} // done
	FlushElements();
}



static void RSStartupFog(void)
{
	// upload fog texture
	gfxSetTextureWrapping( GFX_CLAMP, GFX_CLAMP);
	gfxSetTexture( _fog_ulTexture, _fog_tpLocal);
	// prepare fog rendering parameters
	gfxEnableBlend();
	gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
	// calculate fog mapping
	_fFogMul = -1.0f / _fog_fp.fp_fFar;
}


static void RSStartupHaze(void)
{
	// upload haze texture
	gfxEnableTexture();
	gfxSetTextureWrapping( GFX_CLAMP, GFX_CLAMP);
	gfxSetTexture( _haze_ulTexture, _haze_tpLocal);
	// prepare haze rendering parameters
	gfxEnableBlend();
	gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
	// calculate haze mapping
	_fHazeMul = -1.0f / (_haze_hp.hp_fFar - _haze_hp.hp_fNear);
	_fHazeAdd = _haze_hp.hp_fNear;
}



// group rendering routine
static void RSRenderGroup_internal( ScenePolygon *pspoGroup, ULONG ulGroupFlags, const INDEX ctGroupVertices)
{
	// make vertex coordinates for all polygons in the group
	_ctGroupVtx = ctGroupVertices;    // RSMake... routines will use it
	RSMakeVertexCoordinates(pspoGroup);
	if(CVA_bWorld) gfxLockArrays();

	// set alpha keying if required
	if( ulGroupFlags & GF_KEY) gfxEnableAlphaTest();
	else gfxDisableAlphaTest();
	
	// prepare texture and color arrays
	_iLastUnit = 0;   // reset multi-tex unit change
	BOOL bUsedMT = FALSE;
	BOOL bUsesMT = ulGroupFlags & (GF_TX0_TX1 | GF_TX0_TX2 | GF_TX0_SHD | GF_TX2_SHD 
															|  GF_TX0_TX1_TX2 | GF_TX0_TX1_SHD | GF_TX0_TX2_SHD
															|  GF_TX0_TX1_TX2_SHD);
	// dual texturing
	if( ulGroupFlags & GF_TX0_SHD) {
		RSSetTextureCoords( pspoGroup, SHADOWTEXTURE, 1);
		RSSetTextureCoords( pspoGroup, 0, 0);
		RSSetTextureColors( pspoGroup, GF_TX0|GF_SHD);
		RSRenderTEX_SHD( pspoGroup, 0);
		bUsedMT = TRUE;
	}
	else if( ulGroupFlags & GF_TX0_TX1) {
		RSSetTextureCoords( pspoGroup, 1, 1);
		RSSetTextureCoords( pspoGroup, 0, 0);
		RSSetTextureColors( pspoGroup, GF_TX0|GF_TX1);
		RSRender2TEX( pspoGroup, 1);
		bUsedMT = TRUE;
	} 
	else if( ulGroupFlags & GF_TX0_TX2) {
		RSSetTextureCoords( pspoGroup, 2, 1);
		RSSetTextureCoords( pspoGroup, 0, 0);
		RSSetTextureColors( pspoGroup, GF_TX0|GF_TX2);
		RSRender2TEX( pspoGroup, 2);
		bUsedMT = TRUE;
	}

	// triple texturing
	else if( ulGroupFlags & GF_TX0_TX1_TX2) {
		RSSetTextureCoords( pspoGroup, 2, 2);
		RSSetTextureCoords( pspoGroup, 1, 1);
		RSSetTextureCoords( pspoGroup, 0, 0);
		RSSetTextureColors( pspoGroup, GF_TX0|GF_TX1|GF_TX2);
		RSRender3TEX( pspoGroup);
		bUsedMT = TRUE;
	}
	else if( ulGroupFlags & GF_TX0_TX1_SHD) {
		RSSetTextureCoords( pspoGroup, SHADOWTEXTURE, 2);
		RSSetTextureCoords( pspoGroup, 1, 1);
		RSSetTextureCoords( pspoGroup, 0, 0);
		RSSetTextureColors( pspoGroup, GF_TX0|GF_TX1|GF_SHD);
		RSRender2TEX_SHD( pspoGroup, 1);
		bUsedMT = TRUE;
	}
	else if( ulGroupFlags & GF_TX0_TX2_SHD) {
		RSSetTextureCoords( pspoGroup, SHADOWTEXTURE, 2);
		RSSetTextureCoords( pspoGroup, 2, 1);
		RSSetTextureCoords( pspoGroup, 0, 0);
		RSSetTextureColors( pspoGroup, GF_TX0|GF_TX2|GF_SHD);
		RSRender2TEX_SHD( pspoGroup, 2);
		bUsedMT = TRUE;
	}

	// quad texturing
	else if( ulGroupFlags & GF_TX0_TX1_TX2_SHD) {
		RSSetTextureCoords( pspoGroup, SHADOWTEXTURE, 3);
		RSSetTextureCoords( pspoGroup, 2, 2);
		RSSetTextureCoords( pspoGroup, 1, 1);
		RSSetTextureCoords( pspoGroup, 0, 0);
		RSSetTextureColors( pspoGroup, GF_TX0|GF_TX1|GF_TX2|GF_SHD);
		RSRender3TEX_SHD( pspoGroup);
		bUsedMT = TRUE;
	}

	// if something was drawn and alpha keying was used
	if( bUsedMT && (ulGroupFlags&GF_KEY)) {
		// force z-buffer test to equal and disable subsequent alpha tests
		gfxDepthFunc( GFX_EQUAL);
		gfxDisableAlphaTest();
	}

	// dual texturing leftover
	if( ulGroupFlags & GF_TX2_SHD) {
		RSSetTextureCoords( pspoGroup, SHADOWTEXTURE, 1);
		RSSetTextureCoords( pspoGroup, 2, 0);
		RSSetTextureColors( pspoGroup, GF_TX2|GF_SHD);
		RSRenderTEX_SHD( pspoGroup, 2);
		bUsedMT = TRUE;
	}

	// paranoid android
	ASSERT( !bUsedMT == !bUsesMT);

	// if some multi-tex units were used
	if( bUsesMT) {
		// disable them now
		for( INDEX i=1; i<_ctUsableTexUnits; i++) {
			gfxSetTextureUnit(i);
			gfxDisableTexture();
		}
		_iLastUnit = 0;
		gfxSetTextureUnit(0);
	}
	
	// if group has color for first layer
	if( ulGroupFlags&GF_FLAT)
	{ // render colors
		if( _bTranslucentPass) {
			// set opacity to 50%
			if( !wld_bRenderTextures) RSSetConstantColors( 0x3F3F3F7F);
			else RSSetPolygonColors( pspoGroup, 0x7F);
			gfxEnableBlend();
			gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
		} else {
			// set opacity to 100%
			if( !wld_bRenderTextures) RSSetConstantColors( 0x7F7F7FFF);
			else RSSetPolygonColors( pspoGroup, CT_OPAQUE);
			gfxDisableBlend();
		}
		gfxDisableTexture();
		DrawAllElements( pspoGroup);
	}   

	// if group has texture for first layer
	if( ulGroupFlags&GF_TX0) {
		// render texture 0
		RSSetTextureCoords( pspoGroup, 0, 0);
		RSSetTextureColors( pspoGroup, GF_TX0);
		RSRenderTEX( pspoGroup, 0);
		// eventually prepare subsequent layers for transparency
		if( ulGroupFlags & GF_KEY) {
			gfxDepthFunc( GFX_EQUAL);
			gfxDisableAlphaTest();
		}
	}
	// if group has texture for second layer
	if( ulGroupFlags & GF_TX1) {
		// render texture 1
		RSSetTextureCoords( pspoGroup, 1, 0);
		RSSetTextureColors( pspoGroup, GF_TX1);
		RSRenderTEX( pspoGroup, 1);
	}
	// if group has texture for third layer
	if( ulGroupFlags & GF_TX2) {
		// render texture 2
		RSSetTextureCoords( pspoGroup, 2, 0);
		RSSetTextureColors( pspoGroup, GF_TX2);
		RSRenderTEX( pspoGroup, 2);
	}

	// if group has shadow
	if( ulGroupFlags & GF_SHD) {
		// render shadow
		RSSetTextureCoords( pspoGroup, SHADOWTEXTURE, 0);
		RSSetTextureColors( pspoGroup, GF_SHD);
		RSRenderSHD( pspoGroup);
	}

	// if group has aftershadow texture for second layer
	if( ulGroupFlags & GF_TA1) {
		// render texture 1
		RSSetTextureCoords( pspoGroup, 1, 0);
		RSSetTextureColors( pspoGroup, GF_TX1);
		RSRenderTEX( pspoGroup, 1);
	}
	// if group has aftershadow texture for third layer
	if( ulGroupFlags & GF_TA2) {
		// render texture 2
		RSSetTextureCoords( pspoGroup, 2, 0);
		RSSetTextureColors( pspoGroup, GF_TX2);
		RSRenderTEX( pspoGroup, 2);
	}

	// if group has fog
	if( ulGroupFlags & GF_FOG) {
		// render fog
		RSStartupFog();
		RSSetConstantColors( _fog_fp.fp_colColor);
		RSSetFogCoordinates( pspoGroup);
		RSRenderFog( pspoGroup);
	}
	// if group has haze
	if( ulGroupFlags & GF_HAZE) {
		// render haze
		RSStartupHaze();
		RSSetConstantColors( _haze_hp.hp_colColor);
		RSSetHazeCoordinates( pspoGroup);
		RSRenderHaze( pspoGroup);
	}

	// reset depth function and alpha keying back
	// (maybe it was altered for transparent polygon rendering)
	gfxDepthFunc( GFX_LESS_EQUAL);
	gfxDisableAlphaTest();

#ifdef	WORLD_EDITOR			// yjpark |<--
	// if group has selection
	if( ulGroupFlags & GF_SEL) {
		// render selection
		gfxEnableBlend();
		gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA); 
		RSSetConstantColors( _colSelection|128);
		gfxDisableTexture();
		DrawAllElements( pspoGroup);
	}
	else if( ulGroupFlags & GF_ATTRIBUTE )	// If group has the attribute
	{
		// Render attributes
		gfxEnableBlend();
		gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA ); 
		if( ulGroupFlags & GF_ATTWALK_EVEN )					// Walkable
			RSSetConstantColors( 0x00FFFF80 );
		else if( ulGroupFlags & GF_ATTWALK_ODD )				// Walkable
			RSSetConstantColors( 0x0066FF80 );
		else if( ulGroupFlags & GF_ATTUNWALK_EVEN )				// Unwalkable
			RSSetConstantColors( 0xFF660080 );
		else if( ulGroupFlags & GF_ATTUNWALK_ODD )				// Unwalkable
			RSSetConstantColors( 0xFFB40080 );
		else if( ulGroupFlags & GF_ATTSTAIR )					// Stair
			RSSetConstantColors( 0xAA00FF80 );
		else if( ulGroupFlags & GF_ATTSTAIRWALL )				// Wall of stair
			RSSetConstantColors( 0x00FF7E80 );
		else if( ulGroupFlags & GF_ATTPEACE_ZONE )				// Peace zone
			RSSetConstantColors( 0xE28EF180 );
		else if( ulGroupFlags & GF_ATTPRODUCT_ZONE_PUBLIC )		// Product zone - public
			RSSetConstantColors( 0x3CB43980 );
		else if( ulGroupFlags & GF_ATTPRODUCT_ZONE_PRIVATE )	// Product zone - private
			RSSetConstantColors( 0xB0E73980 );
		else if( ulGroupFlags & GF_ATTWAR_AREA )				// War Area
			RSSetConstantColors( 0xA0E3C9B5 );
		else if( ulGroupFlags & GF_ATTFREEPK_ZONE )				// Free PK Zone
			RSSetConstantColors( 0xD03CE89C );

		gfxDisableTexture();
		DrawAllElements( pspoGroup );
	}
#endif	// WORLD_EDITOR			// yjpark     -->|

	// render triangle wireframe if needed
	extern INDEX wld_bShowTriangles;
	if( wld_bShowTriangles) {
		gfxEnableBlend();
		gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA); 
		RSSetConstantColors( C_mdYELLOW|222);
		gfxDisableTexture();
		// must write to front in z-buffer
		gfxPolygonMode(GFX_LINE);
		gfxEnableDepthTest();
		gfxEnableDepthWrite();
		gfxDepthFunc(GFX_ALWAYS);
		gfxDepthRange( 0,0);
		DrawAllElements(pspoGroup);
		gfxDepthRange( _ppr->pr_fDepthBufferNear, _ppr->pr_fDepthBufferFar);
		gfxDepthFunc(GFX_LESS_EQUAL);
		if( _bTranslucentPass) gfxDisableDepthWrite();
		gfxPolygonMode(GFX_FILL);
	}

	// all done
	gfxUnlockArrays();
}



// process for rendering one group of polygons
void RSRenderGroup( ScenePolygon *pspoGroup, ULONG ulGroupFlags, ULONG ulTestedFlags, const INDEX ctGroupVertices)
{
	// skip if the group is empty
	if( pspoGroup==NULL) return;
	ASSERT( ctGroupVertices>0);
	ASSERT( !(ulTestedFlags&(GF_FOG|GF_HAZE))); // paranoia
	ScenePolygon *pspoST, *pspoMT;
	INDEX ctVtxST, ctVtxMT;

	// if multitexturing is enabled (start with 2-layer MT)
	if( _ctUsableTexUnits>=2)
	{
		// if texture 1 could be merged with shadow
		if( !(ulTestedFlags&GF_TX0_SHD)
		 &&  (ulGroupFlags &GF_TX0)
		 && !(ulGroupFlags &GF_TX1)
		 && !(ulGroupFlags &GF_TX2)
		 &&  (ulGroupFlags &GF_SHD))
		{ // bin polygons that can use the merge and those that cannot
			RSBinByDualTexturing( pspoGroup, 0, SHADOWTEXTURE, &pspoST, &pspoMT, ctVtxST, ctVtxMT);
			ASSERT( (ctVtxST+ctVtxMT) == ctGroupVertices);
			// process the two groups separately
			ulTestedFlags |= GF_TX0_SHD;
			RSRenderGroup( pspoST, ulGroupFlags, ulTestedFlags, ctVtxST);
			ulGroupFlags &= ~(GF_TX0|GF_SHD);
			ulGroupFlags |=   GF_TX0_SHD;
			RSRenderGroup( pspoMT, ulGroupFlags, ulTestedFlags, ctVtxMT);
			return;
		}

		// if texture 1 could be merged with texture 2
		if( !(ulTestedFlags&GF_TX0_TX1)
		 &&  (ulGroupFlags &GF_TX0)
		 &&  (ulGroupFlags &GF_TX1))
		{ // bin polygons that can use the merge and those that cannot
			RSBinByDualTexturing( pspoGroup, 0, 1, &pspoST, &pspoMT, ctVtxST, ctVtxMT);
			ASSERT( (ctVtxST+ctVtxMT) == ctGroupVertices);
			// process the two groups separately
			ulTestedFlags |= GF_TX0_TX1;
			RSRenderGroup( pspoST, ulGroupFlags, ulTestedFlags, ctVtxST);
			ulGroupFlags &= ~(GF_TX0|GF_TX1);
			ulGroupFlags |=   GF_TX0_TX1;
			RSRenderGroup( pspoMT, ulGroupFlags, ulTestedFlags, ctVtxMT);
			return;
		}

		// if texture 1 could be merged with texture 3
		if( !(ulTestedFlags&GF_TX0_TX2)
		 &&  (ulGroupFlags &GF_TX0)
		 && !(ulGroupFlags &GF_TX1)
		 &&  (ulGroupFlags &GF_TX2))
		{ // bin polygons that can use the merge and those that cannot
			RSBinByDualTexturing( pspoGroup, 0, 2, &pspoST, &pspoMT, ctVtxST, ctVtxMT);
			ASSERT( (ctVtxST+ctVtxMT) == ctGroupVertices);
			// process the two groups separately
			ulTestedFlags |= GF_TX0_TX2;
			RSRenderGroup( pspoST, ulGroupFlags, ulTestedFlags, ctVtxST);
			ulGroupFlags &= ~(GF_TX0|GF_TX2);
			ulGroupFlags |=   GF_TX0_TX2;
			RSRenderGroup( pspoMT, ulGroupFlags, ulTestedFlags, ctVtxMT);
			return;
		}

		// if texture 3 could be merged with shadow
		if( !(ulTestedFlags&GF_TX2_SHD)
		 &&  (ulGroupFlags &GF_TX0_TX1)
		 &&  (ulGroupFlags &GF_TX2)
		 &&  (ulGroupFlags &GF_SHD))
		{ // bin polygons that can use the merge and those that cannot
			RSBinByDualTexturing( pspoGroup, 2, SHADOWTEXTURE, &pspoST, &pspoMT, ctVtxST, ctVtxMT);
			ASSERT( (ctVtxST+ctVtxMT) == ctGroupVertices);
			// process the two groups separately
			ulTestedFlags |= GF_TX2_SHD;
			RSRenderGroup( pspoST, ulGroupFlags, ulTestedFlags, ctVtxST);
			ulGroupFlags &= ~(GF_TX2|GF_SHD);
			ulGroupFlags |=   GF_TX2_SHD;
			RSRenderGroup( pspoMT, ulGroupFlags, ulTestedFlags, ctVtxMT);
			return;
		}
	}

	// 4-layer multitexturing?
	if( _ctUsableTexUnits>=4)
	{
		// if texture 1 and 2 could be merged with 3 and shadow
		if( !(ulTestedFlags&GF_TX0_TX1_TX2_SHD)
			&& (ulGroupFlags &GF_TX0_TX1)
			&& (ulGroupFlags &GF_TX2_SHD)) 
		{ // all polygons can be merged - no bining needed
			ulGroupFlags &= ~(GF_TX0_TX1|GF_TX2_SHD);
			ulGroupFlags |=   GF_TX0_TX1_TX2_SHD;
			RSRenderGroup( pspoGroup, ulGroupFlags, ulTestedFlags, ctGroupVertices);
			return;
		}
	} 

	// 3-layer multitexturing?
	if( _ctUsableTexUnits>=3)
	{
		// if texture 1 and 2 could be merged with 3
		if( !(ulTestedFlags&GF_TX0_TX1_TX2)
		 &&  (ulGroupFlags &GF_TX0_TX1)
		 &&  (ulGroupFlags &GF_TX2))
		{ // bin polygons that can use the merge and those that cannot
			RSBinByTripleTexturing( pspoGroup, 2, &pspoST, &pspoMT, ctVtxST, ctVtxMT);
			// process the two groups separately
			ulTestedFlags |= GF_TX0_TX1_TX2;
			RSRenderGroup( pspoST, ulGroupFlags, ulTestedFlags, ctVtxST);
			ulGroupFlags &= ~(GF_TX0_TX1|GF_TX2);
			ulGroupFlags |=   GF_TX0_TX1_TX2;
			RSRenderGroup( pspoMT, ulGroupFlags, ulTestedFlags, ctVtxMT);
			return;
		}

		// if texture 1 and 2 could be merged with shadow
		if( !(ulTestedFlags&GF_TX0_TX1_SHD)
		 &&  (ulGroupFlags &GF_TX0_TX1)
		 && !(ulGroupFlags &GF_TX2)
		 &&  (ulGroupFlags &GF_SHD))
		{ // bin polygons that can use the merge and those that cannot
			RSBinByTripleTexturing( pspoGroup, SHADOWTEXTURE, &pspoST, &pspoMT, ctVtxST, ctVtxMT);
			// process the two groups separately
			ulTestedFlags |= GF_TX0_TX1_SHD;
			RSRenderGroup( pspoST, ulGroupFlags, ulTestedFlags, ctVtxST);
			ulGroupFlags &= ~(GF_TX0_TX1|GF_SHD);
			ulGroupFlags |=   GF_TX0_TX1_SHD;
			RSRenderGroup( pspoMT, ulGroupFlags, ulTestedFlags, ctVtxMT);
			return;
		}

		// if texture 1 and 3 could be merged with shadow
		if( !(ulTestedFlags&GF_TX0_TX2_SHD)
		 &&  (ulGroupFlags &GF_TX0_TX2)
		 && !(ulGroupFlags &GF_TX1)
		 &&  (ulGroupFlags &GF_SHD))
		{ // bin polygons that can use the merge and those that cannot
			RSBinByTripleTexturing( pspoGroup, SHADOWTEXTURE, &pspoST, &pspoMT, ctVtxST, ctVtxMT);
			// process the two groups separately
			ulTestedFlags |= GF_TX0_TX2_SHD;
			RSRenderGroup( pspoST, ulGroupFlags, ulTestedFlags, ctVtxST);
			ulGroupFlags &= ~(GF_TX0_TX2|GF_SHD);
			ulGroupFlags |=   GF_TX0_TX2_SHD;
			RSRenderGroup( pspoMT, ulGroupFlags, ulTestedFlags, ctVtxMT);
			return;
		}
	}

	// render one group
	extern INDEX ogl_iMaxBurstSize;
	extern INDEX d3d_iMaxBurstSize;
	ogl_iMaxBurstSize = Clamp( ogl_iMaxBurstSize, 0L, 9999L);
	d3d_iMaxBurstSize = Clamp( d3d_iMaxBurstSize, 0L, 9999L);
	const INDEX iMaxBurstSize = (_eAPI==GAT_OGL) ? ogl_iMaxBurstSize : d3d_iMaxBurstSize;

	// if unlimited lock count
	if( iMaxBurstSize==0)
	{ // render whole group
		RSRenderGroup_internal( pspoGroup, ulGroupFlags, ctGroupVertices);
	}
	// if lock count is specified
	else
	{ // render group in segments
		while( pspoGroup!=NULL)
		{ // find segment size
			INDEX ctVtx = 0;
			ScenePolygon *pspoThis = pspoGroup;
			ScenePolygon *pspoLast = pspoGroup;
			while( ctVtx<iMaxBurstSize && pspoGroup!=NULL) {
				ctVtx    += pspoGroup->spo_ctVtx;
				pspoLast  = pspoGroup;
				pspoGroup = pspoGroup->spo_pspoSucc;
			} // render one group segment
			pspoLast->spo_pspoSucc = NULL;
			RSRenderGroup_internal( pspoThis, ulGroupFlags, ctVtx);
		}
	}
}



static void RSPrepare(void)
{
	// set general params
	gfxCullFace(GFX_NONE);
	gfxEnableDepthTest();
	gfxEnableClipping();
}


static void RSEnd(void)
{
	// reset unusual gfx API parameters
	gfxSetTextureUnit(0);
	gfxSetTextureModulation(1);
}


void RenderScene( CDrawPort *pDP, ScenePolygon *pspoFirst, CAnyProjection3D &prProjection,
									COLOR colSelection, BOOL bTranslucent)
{
	// check API
	_eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( _eAPI==GAT_OGL || _eAPI==GAT_D3D || _eAPI==GAT_NONE);
	if( _eAPI!=GAT_OGL && _eAPI!=GAT_D3D) return;

	// some cvars cannot be altered in multiplayer mode!
	if( _bMultiPlayer) {
		shd_bShowFlats = FALSE;
		gfx_bRenderWorld = TRUE;
		wld_bRenderShadowMaps = TRUE;
		wld_bRenderTextures = TRUE; 
		wld_bRenderDetailPolygons = TRUE;
		wld_bShowDetailTextures = FALSE;
		wld_bShowTriangles = FALSE;
	}

	// skip if not rendering world
	if( !gfx_bRenderWorld) return;

	// remember input parameters
	ASSERT( pDP != NULL);
	_ppr = (CPerspectiveProjection3D*)&*prProjection;
	_pDP = pDP;
	_colSelection = colSelection;
	_bTranslucentPass = bTranslucent;

	// clamp detail textures LOD biasing
	wld_iDetailRemovingBias = Clamp( wld_iDetailRemovingBias, -9L, +9L);

	// set perspective projection
	_pDP->SetProjection(prProjection);

	// adjust multi-texturing support (for clip-plane emulation thru texture units)
	extern BOOL  GFX_bClipPlane; // WATCHOUT: set by 'SetProjection()' !
	extern INDEX gap_iUseTextureUnits;
	extern INDEX ogl_bAlternateClipPlane;
	INDEX ctMaxUsableTexUnits = _pGfx->gl_ctTextureUnits;
	if( _eAPI==GAT_OGL && ogl_bAlternateClipPlane && GFX_bClipPlane && ctMaxUsableTexUnits>1) ctMaxUsableTexUnits--;
	_ctUsableTexUnits = Clamp( gap_iUseTextureUnits, 1L, ctMaxUsableTexUnits);

	// prepare
	RSPrepare();

	// turn depth buffer writing on or off
	if( bTranslucent) gfxDisableDepthWrite();
	else gfxEnableDepthWrite();

	// remove all polygons with no triangles from the polygon list
	ScenePolygon *pspoNonDummy;
	RSRemoveDummyPolygons( pspoFirst, &pspoNonDummy);

	// check that layers of all shadows are up to date
	RSCheckLayersUpToDate(pspoNonDummy);

	// bin polygons to groups by texture passes
	RSBinToGroups(pspoNonDummy);

	// for each group                           
	ASSERT( _apspoGroups[0]==NULL); // zero group must always be empty
	for( INDEX iGroup=1; iGroup<_ctGroupsCount; iGroup++) {
		// get the group polygon list and render it if not empty
		ScenePolygon *pspoGroup = _apspoGroups[iGroup];
		if( pspoGroup!=NULL) RSRenderGroup( pspoGroup, iGroup, 0, _aiGroupVtx[iGroup]);
	}

	// all done
	RSEnd();
}


// renders only scene z-buffer
void RenderSceneZOnly( CDrawPort *pDP, ScenePolygon *pspoFirst, CAnyProjection3D &prProjection)
{
	if( _bMultiPlayer) gfx_bRenderWorld = 1;
	if( !gfx_bRenderWorld) return;

	// set perspective projection
	ASSERT(pDP!=NULL);
	pDP->SetProjection(prProjection);

	// prepare
	RSPrepare();

	// set for depth-only rendering
	const ULONG ulCurrentColorMask = gfxGetColorMask();
	gfxSetColorMask(NONE);
	gfxEnableDepthTest();
	gfxEnableDepthWrite();
	gfxDisableTexture();
	gfxDisableColorArray();

	// count total number of vertices
	_ctGroupVtx = 0;
	for( ScenePolygon *pspo=pspoFirst; pspo!=NULL; pspo=pspo->spo_pspoSucc) _ctGroupVtx += pspo->spo_ctVtx;

	// make vertex coordinates for all polygons in the group and render the polygons
	RSMakeVertexCoordinates(pspoFirst);
	DrawAllElements(pspoFirst);

	// restore color masking
	gfxSetColorMask( ulCurrentColorMask);
	RSEnd();
}


// renders flat background of the scene
void RenderSceneBackground(CDrawPort *pDP, COLOR col)
{
	if( _bMultiPlayer) gfx_bRenderWorld = 1;
	if( !gfx_bRenderWorld) return;

	// set orthographic projection
	ASSERT(pDP!=NULL);
	pDP->SetOrtho();

	// prepare
	gfxEnableDepthTest();
	gfxDisableDepthWrite();
	gfxDisableBlend();
	gfxDisableAlphaTest();
	gfxDisableTexture();
	gfxEnableClipping();

	GFXColor glcol(col|CT_OPAQUE);
	const INDEX iW = pDP->GetWidth();
	const INDEX iH = pDP->GetHeight();

	// set arrays
	gfxResetArrays();
	GFXVertex   *pvtx = _avtxCommon.Push(4);
	GFXTexCoord *ptex = _atexCommon[0].Push(4);
	GFXColor    *pcol = _acolCommon.Push(4);
	pvtx[0].x =  0;  pvtx[0].y =  0;  pvtx[0].z = 1;
	pvtx[1].x =  0;  pvtx[1].y = iH;  pvtx[1].z = 1;
	pvtx[2].x = iW;  pvtx[2].y = iH;  pvtx[2].z = 1;
	pvtx[3].x = iW;  pvtx[3].y =  0;  pvtx[3].z = 1;
	pcol[0] = glcol;
	pcol[1] = glcol;
	pcol[2] = glcol;
	pcol[3] = glcol;

	// render
	_pGfx->gl_ctWorldElements += 6; 
	gfxFlushQuads();
}
