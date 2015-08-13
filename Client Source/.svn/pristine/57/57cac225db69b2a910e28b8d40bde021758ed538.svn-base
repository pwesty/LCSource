#include "stdh.h"

#include <Engine/Brushes/Brush.h>
#include <Engine/Brushes/BrushTransformed.h>
#include <Engine/Rendering/Render.h>
#include <Engine/Rendering/Render_internal.h>
#include <Engine/Base/Console.h>
#include <Engine/Templates/DynamicContainer.h>
#include <Engine/Templates/DynamicContainer.cpp>

#include <Engine/Light/LightSource.h>
#include <Engine/Light/Gradient.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/World/World.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Math/Clipping.inl>
#include <Engine/Entities/EntityClass.h>
#include <Engine/World/WorldSettings.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Entities/FieldSettings.h>
#include <Engine/Entities/ShadingInfo.h>
#include <Engine/Light/LensFlares.h>
#include <Engine/Models/ModelObject.h>
#include <Engine/Models/RenderModel.h>
#include <Engine/Ska/Render.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Templates/BSP.h>
#include <Engine/World/WorldEditingProfile.h>
#include <Engine/Brushes/BrushArchive.h>
#include <Engine/Math/Float.h>
#include <Engine/Math/OBBox.h>
#include <Engine/Math/Geometry.inl>

#include <Engine/Graphics/DrawPort.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/Fog_internal.h>

#include <Engine/Base/Statistics_internal.h>
#include <Engine/Base/MemoryTracking.h>

#include <Engine/Templates/LinearAllocator.cpp>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Templates/StaticStackArray.cpp>
#include <Engine/Templates/DynamicStackArray.cpp>

#include <Engine/Network/CNetwork.h>

#include <Engine/Interface/UIManager.h>
#include <Engine/Effect/CMdlEffect.h>

// sehan
//BOOL g_bUseBloom = FALSE;
extern CRenderTexture       *_prtFilterTarget   [];
extern LPDIRECT3DVERTEXBUFFER8  _pFilterVertexBuffer;
extern LPDIRECT3DVERTEXBUFFER8  _pBackVertexBuffer[MAX_BACKVERTEX_WIDTH][MAX_BACKVERTEX_HEIGHT];
// sehan end

extern BOOL _bSomeDarkExists;
extern INDEX d3d_bAlternateDepthReads;

// general coordinate stack referenced by the scene polygons
extern CStaticStackArray<GFXVertex> _avtxScene;

extern void SetViewMatrix( Matrix12 &mView12);

//강동민 수정 시작 그림자 옵션 작업	08.16
// FIXME : 그림자 단계를 어떻게 나눌것인가?
enum eShadowDetail
{
	SHADOW_NONE			= 0,// 그림자 없음.	
	SHADOW_SIMPLE_ONLY	= 1,// 플레이어만 둥근 그림자 하나
	SHADOW_PROJ_ONLY	= 2,// 플레이어만 풀 그림자 하나
	SHADOW_SIMPLE_ALL	= 3,// 모두 둥근 그림자
	SHADOW_PROJ_ALL		= 4,// 모두 풀 그림자
};

enum eReflectionDetail
{
	REFLECTION_NONE = 0,	// 반사 안함
	REFLECTION_SKY,			// 하늘만 반사
	REFLECTION_OBJECT,
	REFLECTION_ALL,
};
//강동민 수정 끝 그림자 옵션 작업		08.16

extern Matrix16 _matWaterProj = {0};
static BOOL bRenderReflection = FALSE;
extern BOOL				_bRenderProjectionShadow	= FALSE;
//#define MAXDISTANCE		(15.0f)
static const float _fSHDPlayerDistance = 15.0f;
static const float _fSHDCameraDistance = 100.0f;
// NOTE : Refraction은 추후에 지원할 것을 고려하여...
extern CRenderTexture*	re_prtReflection			= NULL;		// Reflection으로 사용할 텍스쳐.
extern BOOL				re_bInitReflectionTexture	= FALSE;

extern BOOL				re_bRenderReflection		= TRUE;
/*
extern CRenderTexture*	re_prtRefraction			= NULL;		// Refraction으로 사용할 텍스쳐.
extern BOOL				re_bInitRefractionTexture	= FALSE;
extern BOOL				re_bRenderRefraction		= FALSE;
*/
static BOOL				re_bRenderingReflectionTexture = FALSE;
extern FLOATplane3D		_plReflect(FLOAT3D(0.0f, 1.0f, 0.0f), 0.05f);

extern CNiceWater		_WaterInformation(NULL);

extern void ReleaseReflectionTexture()
{
	if(re_prtReflection)
	{
		delete re_prtReflection;
		re_prtReflection = NULL;
		re_bInitReflectionTexture = FALSE;
	}
}

extern void ReleaseRefractionTexture()
{
	// NOTE : Refraction은 추후에 지원할 것을 고려하여...
	/*
	if(re_prtRefraction)
	{
	delete re_prtRefraction;
	re_prtRefraction = NULL;
	re_bInitRefractionTexture = FALSE;
	}
	*/
}

static Matrix16			_matShadowProj;	// 광원에서 투영하는 행렬...
static Matrix12			_matWorldToLight;
static CRenderTexture	*_prtShadow			= NULL;
static BOOL				_bInitShadowTexture	= FALSE;	

extern void ReleaseShadowTexture()
{
	if(_prtShadow)
	{
		delete _prtShadow;
		_prtShadow = NULL;
		_bInitShadowTexture = FALSE;
	}
}

//#pragma optimize ("gt", on)
#pragma inline_depth(255)
#pragma inline_recursion(on)

#ifndef NDEBUG
//#define ASER_EXTREME_CHECKING 1
#endif

// the renderer structures used in rendering
//안태훈 수정 시작	//(Add CRenderTexture class for Render to Texture)(0.1)
#include <algorithm>
//#define MAX_RENDERERS 2	//원본

#define MAX_RENDERERS 3
#define IDX_MAIN_RENDERER				0
#define IDX_MIRROR_RENDERER				1
#define IDX_WATER_RENDERER				2
//안태훈 수정 끝	//(Add CRenderTexture class for Render to Texture)(0.1)
static CRenderer _areRenderers[MAX_RENDERERS];
static BOOL _bMirrorDrawn = FALSE;

extern INDEX wld_bAlwaysAddAll;
extern INDEX wld_bRenderEmptyBrushes;
extern INDEX wld_bRenderDetailPolygons;
extern INDEX gfx_bRenderParticles;
extern INDEX gfx_bRenderModels;
extern INDEX gfx_bRenderFog;
extern INDEX gfx_iLensFlareQuality;
extern BOOL _bMultiPlayer;

// variables for selection on rendering
extern CBrushVertexSelection *_pselbvxtSelectOnRender = NULL;
extern CStaticStackArray<PIX2D> *_pavpixSelectLasso = NULL;
extern CEntitySelection *_pselenSelectOnRender = NULL;
extern PIX2D _vpixSelectNearPoint = PIX2D(0,0);
extern BOOL _bSelectAlternative   = FALSE;
extern PIX _pixDeltaAroundVertex  = 10;

// shading info for viewer of last rendered view
FLOAT3D _vViewerLightDirection;
COLOR _colViewerLight;
COLOR _colViewerAmbient;



// handy statistic helper routines

static enum CStatForm::StatTimerIndex _stiLastStatsMode = (enum CStatForm::StatTimerIndex)-1;

void StopStatsMode(void)
{
	ASSERT( (INDEX)_stiLastStatsMode != -1);
	if( _stiLastStatsMode>=0) _sfStats.StopTimer(_stiLastStatsMode);
	_stiLastStatsMode = (enum CStatForm::StatTimerIndex)-1;
}

void StartStatsMode( enum CStatForm::StatTimerIndex sti)
{
	ASSERT( (INDEX)sti != -1);
	ASSERT( (INDEX)_stiLastStatsMode == -1);
	if( sti>=0) _sfStats.StartTimer(sti);
	_stiLastStatsMode = sti;
}

void ChangeStatsMode( enum CStatForm::StatTimerIndex sti)
{
	StopStatsMode();
	StartStatsMode(sti);
}


// screen edges, polygons and trapezoids used in rasterizing
CDynamicStackArray<CAddEdge> CRenderer::re_aadeAddEdges;
CDynamicStackArray<CScreenEdge> CRenderer::re_asedScreenEdges;
// spans for current scan line
CDynamicStackArray<CSpan> CRenderer::re_aspSpans;

// vertices clipped to current clip plane
CStaticStackArray<INDEX> CRenderer::re_aiClipBuffer;
// buffers for edges of polygons
CStaticStackArray<INDEX> CRenderer::re_aiEdgeVxClipSrc;
CStaticStackArray<INDEX> CRenderer::re_aiEdgeVxClipDst;

// add and remove lists for each scan line
CStaticArray<CListHead> CRenderer::re_alhAddLists;
CStaticArray<INDEX> CRenderer::re_actAddCounts;   // count of edges in given add list
CStaticArray<CScreenEdge *> CRenderer::re_apsedRemoveFirst;
CStaticStackArray<CActiveEdge>  CRenderer::re_aaceActiveEdgesTmp;
CStaticStackArray<CActiveEdge>  CRenderer::re_aaceActiveEdges;

// container for sorting translucent polygons
CDynamicStackArray<CTranslucentPolygon> CRenderer::re_atcTranslucentPolygons;

// container for all light influencing current model
struct ModelLight 
{
	CLightSource *ml_plsLight;  // the light source
	FLOAT3D ml_vDirection;      // direction from light to the model position (normalized)
	FLOAT ml_fShadowIntensity;  // intensity at the model position (for shadow)
	FLOAT ml_fR, ml_fG, ml_fB;  // light components at light source (0..255)
	inline void Clear(void) {};
};
static CDynamicStackArray<struct ModelLight> _amlLights;

static INDEX _ctMaxAddEdges=0;
static INDEX _ctMaxActiveEdges=0;

void RendererInfo(void)
{
	CPrintF("Renderer information:\n");
	
	SLONG slMem = 0;
	
	slMem += CRenderer::re_aadeAddEdges.da_Count*sizeof(CAddEdge);
	slMem += CRenderer::re_asedScreenEdges.da_Count*sizeof(CScreenEdge);
	
	slMem += CRenderer::re_aspSpans.da_Count*sizeof(CSpan);
	
	slMem += CRenderer::re_aiClipBuffer.sa_Count*sizeof(INDEX);
	slMem += CRenderer::re_aiEdgeVxClipSrc.sa_Count*sizeof(INDEX);
	slMem += CRenderer::re_aiEdgeVxClipDst.sa_Count*sizeof(INDEX);
	
	slMem += CRenderer::re_alhAddLists.sa_Count*sizeof(CListHead);
	slMem += CRenderer::re_actAddCounts.sa_Count*sizeof(INDEX);
	slMem += CRenderer::re_apsedRemoveFirst.sa_Count*sizeof(CScreenEdge *);
	
	slMem += CRenderer::re_atcTranslucentPolygons.da_Count*sizeof(CTranslucentPolygon);
	slMem += CRenderer::re_aaceActiveEdges.sa_Count*sizeof(CActiveEdge);
	slMem += CRenderer::re_aaceActiveEdgesTmp.sa_Count*sizeof(CActiveEdge);
	
	for (INDEX ire = 0; ire<MAX_RENDERERS; ire++) 
	{
		CRenderer &re = _areRenderers[ire];
		slMem += re.re_aspoScreenPolygons.da_Count*sizeof(CScreenPolygon);
		slMem += re.re_admDelayedModels.da_Count*sizeof(CDelayedModel);
		slMem += re.re_cenDrawn.sa_Count*sizeof(CEntity*);
		slMem += re.re_alfiLensFlares.sa_Count*sizeof(CLensFlareInfo);
		slMem += re.re_amiMirrors.da_Count*sizeof(CMirror);
		slMem += re.re_amiWaters.da_Count*sizeof(CWaterEntity);		
		slMem += re.re_avvxViewVertices.sa_Count*sizeof(CViewVertex);
		slMem += re.re_aiEdgeVxMain.sa_Count*sizeof(INDEX);
	}
	
	CPrintF("Temporary memory used: %dk\n", slMem/1024);
}

void ClearRenderer(void)
{
	CRenderer::re_aadeAddEdges.Clear();
	CRenderer::re_asedScreenEdges.Clear();
	CRenderer::re_aspSpans.Clear();
	
	CRenderer::re_aiClipBuffer.Clear();
	CRenderer::re_aiEdgeVxClipSrc.Clear();
	CRenderer::re_aiEdgeVxClipDst.Clear();
	
	CRenderer::re_alhAddLists.Clear();
	CRenderer::re_actAddCounts.Clear();
	CRenderer::re_apsedRemoveFirst.Clear();
	CRenderer::re_atcTranslucentPolygons.Clear();
	CRenderer::re_aaceActiveEdges.Clear();
	CRenderer::re_aaceActiveEdgesTmp.Clear();
	
	for (INDEX ire = 0; ire<MAX_RENDERERS; ire++) 
	{
		CRenderer &re = _areRenderers[ire];
		
		re.re_aspoScreenPolygons.Clear();
		re.re_admDelayedModels.Clear();
		re.re_cenDrawn.Clear();
		re.re_alfiLensFlares.Clear();
		re.re_amiMirrors.Clear();
		re.re_amiWaters.Clear();
		re.re_avvxViewVertices.Clear();
		re.re_aiEdgeVxMain.Clear();
		re.re_admDelayedModels_AddWater.Clear();
	}
	
	CPrintF("Renderer buffers cleared.\n");
}

/*
* How much to offset left, right, top and bottom clipping towards inside (in pixels).
* This can be used to test clipping or to add an epsilon value for it.
*/
//#define CLIPMARGIN 10.0f    // used for debugging clipping
#define CLIPMARGIN 0.0f
#define CLIPEPSILON 0.5f
#define CLIPMARGADD (CLIPMARGIN-CLIPEPSILON)
#define CLIPMARGSUB (CLIPMARGIN+CLIPEPSILON)
#define SENTINELEDGE_EPSILON 0.4f

#include "RendMisc.cpp"
#include "RenCache.cpp"
#include "RendClip.cpp"
#include "RendASER.cpp"
#include "RenderModels.cpp"
#include "RenderBrushes.cpp"
#include "RenderAdding.cpp"

extern FLOAT wld_fEdgeOffsetI;
extern FLOAT wld_fEdgeAdjustK;


// initialize all rendering structures
void CRenderer::Initialize(void)
{
	// used for fixing problems with extra trapezoids generated on t-junctions
	if( !re_bRenderingShadows) 
	{
		re_fEdgeOffsetI = wld_fEdgeOffsetI;  //0.125f;
		re_fEdgeAdjustK = wld_fEdgeAdjustK;  //1.0001f;
	}
	else 
	{
		re_fEdgeOffsetI = 0.0f;
		re_fEdgeAdjustK = 1.0f;
	}
	
	// prepare the raw projection (used for rendering target lines and getting object distances)
	re_prProjection->ObjectPlacementL() = CPlacement3D(FLOAT3D(0.0f,0.0f,0.0f), ANGLE3D(0,0,0));
	re_prProjection->ObjectFaceForwardL() = FALSE;
	re_prProjection->ObjectStretchL() = FLOAT3D(1.0f, 1.0f, 1.0f);
	re_prProjection->DepthBufferNearL() = 0.0f;
	re_prProjection->DepthBufferFarL()  = 0.9f;
	re_prProjection->Prepare();
	
	re_asedScreenEdges.PopAll();
	re_aadeAddEdges.PopAll();
	re_aspSpans.PopAll();
	re_avvxViewVertices.PopAll();
	re_aiEdgeVxMain.PopAll();
	
	// if more scan lines are needed than last time
	if (re_alhAddLists.Count()<re_ctScanLines) 
	{
		re_alhAddLists.Clear();
		re_alhAddLists.New(re_ctScanLines);
		re_actAddCounts.Clear();
		re_actAddCounts.New(re_ctScanLines);
		
		re_apsedRemoveFirst.Clear();
		re_apsedRemoveFirst.New(re_ctScanLines);
	}
	// clear all add/remove lists
	for(INDEX iScan=0; iScan<re_ctScanLines; iScan++) 
	{
		re_actAddCounts[iScan] = 0;
		re_apsedRemoveFirst[iScan] = NULL;
	}
	
	// find selection color
	re_colSelection = C_RED;
	if (_wrpWorldRenderPrefs.GetSelectionType() == CWorldRenderPrefs::ST_POLYGONS) 
	{
		re_colSelection = C_YELLOW;
	}
	else if (_wrpWorldRenderPrefs.GetSelectionType() == CWorldRenderPrefs::ST_SECTORS) 
	{
		re_colSelection = C_GREEN;
	}
	else if (_wrpWorldRenderPrefs.GetSelectionType() == CWorldRenderPrefs::ST_ENTITIES) 
	{
		re_colSelection = C_BLUE;
	}
	
	// set up renderer for first scan line
	re_iCurrentScan = 0;
	re_pixCurrentScanJ = re_iCurrentScan + re_pixTopScanLineJ;
	re_fCurrentScanJ = FLOAT(re_pixCurrentScanJ);
	
	// no fog or haze initially
	re_bCurrentSectorHasHaze = FALSE;
	re_bCurrentSectorHasFog  = FALSE;
	
}


// add initial sectors to active lists
void CRenderer::AddInitialSectors(void)
{
	re_bViewerInHaze = FALSE;
	re_ulVisExclude = 0;
	re_ulVisInclude = 0;
	
	// if showing vis tweaks
	if (_wrpWorldRenderPrefs.wrp_bShowVisTweaksOn && _pselbscVisTweaks!=NULL) 
	{
		// add flags for selected flags
		if (_pselbscVisTweaks->Count()>0) 
		{
			re_ulVisExclude = VISM_INCLUDEEXCLUDE;
		}
		FOREACHINDYNAMICCONTAINER(*_pselbscVisTweaks, CBrushSector, itbsc) 
		{
			if (itbsc->bsc_ulFlags2&BSCF2_VISIBILITYINCLUDE) 
			{
				re_ulVisInclude = itbsc->bsc_ulVisFlags&VISM_INCLUDEEXCLUDE;
			}
			else 
			{
				re_ulVisExclude &= itbsc->bsc_ulVisFlags&VISM_INCLUDEEXCLUDE;
			}
		}
	}
	
	// check if the background is needed
	re_bBackgroundEnabled = FALSE;
	if (!re_bRenderingShadows && _wrpWorldRenderPrefs.wrp_bBackgroundTextureOn) 
	{
		CEntity *penBackgroundViewer = re_pwoWorld->GetBackgroundViewer();
		if (penBackgroundViewer!=NULL) 
		{
			re_bBackgroundEnabled = TRUE;
			re_penBackgroundViewer = penBackgroundViewer;
			re_prBackgroundProjection = re_prProjection;
			CPlacement3D plViewer = re_prProjection->ViewerPlacementR();
			plViewer.pl_PositionVector = FLOAT3D(0,0,0);
			CPlacement3D plBcgViewer = penBackgroundViewer->GetLerpedPlacement();
			if (re_prProjection->pr_bMirror) 
			{
				ReflectPositionVectorByPlane(re_prProjection->pr_plMirror, plBcgViewer.pl_PositionVector);
			}
//강동민 수정 시작 Water 구현		04.20
			if(re_prProjection->pr_bNiceWater)
			{
				extern FLOATplane3D		_plReflect;
				ReflectPositionVectorByPlane(_plReflect, plBcgViewer.pl_PositionVector);
			}
//강동민 수정 끝 Water 구현			04.20
			plViewer.RelativeToAbsoluteSmooth(plBcgViewer);
			re_prBackgroundProjection->ViewerPlacementL() = plViewer;
			re_prBackgroundProjection->ObjectPlacementL() = CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0));
			re_prBackgroundProjection->FarClipDistanceL() = -1.0f;
			re_prBackgroundProjection->DepthBufferNearL() = 0.9f;
			re_prBackgroundProjection->DepthBufferFarL()  = 1.0f;
			re_prBackgroundProjection->TurnOffWarpPlane();  // background never needs warp-plane clipping
			re_prBackgroundProjection->Prepare();
		}
	}
	
	// if a viewer entity is given
	if (re_penViewer!=NULL) 
	{
		// add all zoning sectors near the entity
		AddZoningSectorsAroundEntity(re_penViewer, re_prProjection->ViewerPlacementR().pl_PositionVector);
		// make sure the viewer is always added (if model)
		if(re_penViewer->en_RenderType==CEntity::RT_MODEL ||
			re_penViewer->en_RenderType==CEntity::RT_EDITORMODEL) 
		{
			AddModelEntity(re_penViewer);
		}
		// if a viewer polygons are given
	}
	else if (re_pcspoViewPolygons!=NULL) 
	{
		// for each polygon
		FOREACHINDYNAMICCONTAINER(*re_pcspoViewPolygons, CScreenPolygon, itspo) 
		{
			CBrushPolygon *pbpo = itspo->spo_pbpoBrushPolygon;
			// get the sector, sector's brush mip, brush and entity
			CBrushSector *pbsc = pbpo->bpo_pbscSector;
			CBrushMip *pbmBrushMip = pbsc->bsc_pbmBrushMip;
			CBrush3D *pbrBrush = pbmBrushMip->bm_pbrBrush;
			ASSERT(pbrBrush!=NULL);
			CEntity *penBrush = pbrBrush->br_penEntity;
			// if the brush is zoning
			if (penBrush->en_ulFlags&ENF_ZONING) 
			{
				// add the sector that the polygon is in
				AddGivenZoningSector(pbsc);
				// if the brush is non-zoning
			}
			else 
			{
				// add sectors around it
				AddZoningSectorsAroundEntity(penBrush, penBrush->GetPlacement().pl_PositionVector);
			}
		}
		// if there is no viewer entity/polygon
	}
	else 
	{
		// set up viewer bounding box as box of minimum redraw range around viewer position
		if (re_bRenderingShadows) 
		{
			// NOTE: when rendering shadows, this is set in ::RenderShadows()
			//re_boxViewer = FLOATaabbox3D(re_prProjection->ViewerPlacementR().pl_PositionVector,
			//  1.0f);
		}
		else 
		{
			re_boxViewer = FLOATaabbox3D(re_prProjection->ViewerPlacementR().pl_PositionVector,
				_wrpWorldRenderPrefs.wrp_fMinimumRenderRange);
		}
		// add all zoning sectors near viewer box
		AddZoningSectorsAroundBox(re_boxViewer);
		// NOTE: this is so entities outside of world can be edited in WEd
		// if editor models should be rendered
		if (_wrpWorldRenderPrefs.IsEditorModelsOn()) 
		{
			// add all nonzoning entities near viewer box
			AddEntitiesInBox(re_boxViewer);
		}
	}
	
	if( wld_bAlwaysAddAll) 
	{
		AddAllEntities(); // used for profiling
	}
	else 
	{
		// NOTE: this is so that world can be viewed from the outside in game
		// if no brush sectors have been added so far
		if (!re_bRenderingShadows && re_lhActiveSectors.IsEmpty()) 
		{
			// add all entities in the world
			AddAllEntities();
		}
	}
	
	// add the background if needed
	if (re_bBackgroundEnabled) 
	{
		AddZoningSectorsAroundEntity(re_penBackgroundViewer, 
			re_penBackgroundViewer->GetPlacement().pl_PositionVector);
	}
}


// scan through portals for other sectors
void CRenderer::ScanForOtherSectors(void)
{
	ChangeStatsMode(CStatForm::STI_WORLDVISIBILITY);
	extern INDEX g_iReflectionDetail;
	// if shadows or polygons should be drawn
	if (re_bRenderingShadows
		||_wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE) 
	{
		// rasterize edges into spans
		ScanEdges();
	}
	// for each of models that were kept for delayed rendering
	for(INDEX iModel=0; iModel<re_admDelayedModels.Count(); ++iModel) 
	{
		// mark the entity as not active in rendering anymore
		re_admDelayedModels[iModel].dm_penModel->en_ulFlags &= ~ENF_INRENDERING;
		
	if((!bRenderReflection && re_iIndex == IDX_MAIN_RENDERER) &&
		g_iReflectionDetail != REFLECTION_NONE)
		{
			
			if(!(re_admDelayedModels[iModel].dm_ulFlags & DMF_VISIBLE))		// Visible Check
				continue;
			if(re_admDelayedModels[iModel].dm_penModel->en_ulFlags & ENF_NICEWATER)
			{
				CEntity* pEnt = re_admDelayedModels[iModel].dm_penModel;
				AddWater(*pEnt);
			}
		}
	}
	//if((!bRenderReflection && re_iIndex == IDX_MAIN_RENDERER) &&
	//	g_iReflectionDetail != REFLECTION_NONE)
	//{
		//ScanWaterEntity();
	//}
	ChangeStatsMode(CStatForm::STI_WORLDTRANSFORM);
}

BOOL CRenderer::ScanWaterEntity()
{
	// 거울에 해당하는 엔티티를 스캔합니다.
	for(INDEX iModel=0; iModel<re_admDelayedModels.Count(); ++iModel) 
	{		
		if(!(re_admDelayedModels[iModel].dm_ulFlags & DMF_VISIBLE))		// Visible Check
			continue;
		if(re_admDelayedModels[iModel].dm_penModel->en_ulFlags & ENF_NICEWATER)
		{
			CEntity* pEnt = re_admDelayedModels[iModel].dm_penModel;
			AddWater(*pEnt);
		}
	}
	return FALSE;
}

// cleanup after scanning
void CRenderer::CleanupScanning(void)
{
	// for all active sectors
	{
		FORDELETELIST(CBrushSector, bsc_lnInActiveSectors, re_lhActiveSectors, itbsc) 
		{
			if( !itbsc )
				continue;

			// remove it from list
			itbsc->bsc_lnInActiveSectors.Remove();
			
			// for all polygons in sector
			FOREACHINSTATICARRAY(itbsc->bsc_abpoPolygons, CBrushPolygon, itpo) 
			{
				if( !itpo )
					continue;
				CBrushPolygon &bpo = *itpo;
				// clear screen polygon pointers
				bpo.bpo_pspoScreenPolygon = NULL;
			}
		}
	}
	ASSERT(re_lhActiveSectors.IsEmpty());
	
	// for all active brushes
	{
		FORDELETELIST(CBrush3D, br_lnInActiveBrushes, re_lhActiveBrushes, itbr) 
		{
			if( !itbr )
				continue;
			// remove it from list
			itbr->br_lnInActiveBrushes.Remove();
		}
	}
	ASSERT(re_lhActiveBrushes.IsEmpty());
	
	// for all active terrains
	{
		FORDELETELIST(CTerrain, tr_lnInActiveTerrains, re_lhActiveTerrains, ittr) 
		{
			if( !ittr )
				continue;
			// remove it from list
			ittr->tr_lnInActiveTerrains.Remove();
		}
	}
	ASSERT(re_lhActiveTerrains.IsEmpty());
}


// Render active terrains
void CRenderer::RenderTerrains(void)
{
	CAnyProjection3D *papr;
	papr = &re_prProjection;
	
	// begin terrain rendering
	TR_BeginRenderingView(*papr, re_pdpDrawPort);
//강동민 수정 시작 Water 구현		04.19
	// NOTE : 클리핑을 하게 되면 Terrain이 0의 위치를 벗어나게 될때 안나오게 되는 문제가 발생함.
//강동민 수정 시작 접속 시퀀스 작업	05.22
	//if(papr->ap_CurrentProjection->pr_bNiceWater)
	//	gfxDisableClipPlane();
//강동민 수정 끝 접속 시퀀스 작업	05.22
//강동민 수정 끝 Water 구현			04.19
//강동민 수정 시작 Water 구현		04.21
	//if(papr->ap_CurrentProjection->pr_bMirror || papr->ap_CurrentProjection->pr_bWarp)
	//	gfxDisableClipPlane();
//강동민 수정 끝 Water 구현			04.21
	
	// for all active terrains
	{
		FORDELETELIST(CTerrain, tr_lnInActiveTerrains, re_lhActiveTerrains, ittr) 
		{
			CTerrain *ptrTerrain = ittr;
			const CEntity *penEntity = ptrTerrain->tr_penEntity; 
			ASSERT(penEntity!=NULL); // must be valid
			TR_SetTerrainPlacement(penEntity->en_mRotation,penEntity->en_plPlacement.pl_PositionVector);
			// render terrain
			TR_RenderTerrain(ptrTerrain);
		}
	}
	
	// end terrain rendering
	TR_EndRenderingView();
}

// Render active terrains in wireframe mode
void CRenderer::RenderWireFrameTerrains(void)
{
	CAnyProjection3D *papr;
	papr = &re_prProjection;
	
	BOOL bShowEdges = _wrpWorldRenderPrefs.wrp_ftEdges != CWorldRenderPrefs::FT_NONE;
	BOOL bShowVertices = _wrpWorldRenderPrefs.wrp_ftVertices != CWorldRenderPrefs::FT_NONE;
	// BOOL bForceRegenerate = _wrpWorldRenderPrefs.wrp_ftPolygons
	
	COLOR colEdges    = _wrpWorldRenderPrefs.wrp_colEdges;
	COLOR colVertices = 0xFF0000FF;
	
	// begin terrain rendering
	TR_BeginRenderingView(*papr, re_pdpDrawPort);
	
	// for all active terrains
	{
		FORDELETELIST(CTerrain, tr_lnInActiveTerrains, re_lhActiveTerrains, ittr) 
		{
			CTerrain *ptrTerrain = ittr;
			const CEntity *penEntity = ptrTerrain->tr_penEntity; 
			ASSERT(penEntity!=NULL); // must be valid
			
			// render terrain in wireframe
			if(bShowEdges) 
			{
				TR_SetTerrainPlacement(penEntity->en_mRotation,penEntity->en_plPlacement.pl_PositionVector);
				TR_RenderWireFrame(ptrTerrain, colEdges);
			}
			if(bShowVertices) 
			{
				TR_SetTerrainPlacement(penEntity->en_mRotation,penEntity->en_plPlacement.pl_PositionVector);
				//ittr->RenderVertices(*papr, re_pdpDrawPort,colVertices);
			}
		}
	}
	
	// end terrain rendering
	TR_EndRenderingView();
}

//안태훈 수정 시작	//(Add CRenderTexture class for Render to Texture)(0.1)
#include <Engine/Graphics/Texture.h>
//안태훈 수정 끝	//(Add CRenderTexture class for Render to Texture)(0.1)
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include <Engine/Effect/EffectCommon.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CEffectManager.h>

/* Process effect */
void CRenderer::ProcessEffects(BOOL bBackground, EFF_RENDER_TYPE erType /* = ER_NORMAL */, CAnyProjection3D* ppr /* = NULL */)
{
	CAnyProjection3D *papr;

	if (ppr == NULL)
	{
		if( bBackground) papr = &re_prBackgroundProjection;
		else papr = &re_prProjection;
	}
	else
	{
		papr = ppr;
	}

	CAnyProjection3D &prProjection = *papr;
	papr = NULL;

	FLOAT fCurrentTime = _pTimer->GetLerpedCurrentTick();
	
	CEffectGroupManager::my_list listDelete;
	CEffectGroupManager::my_list::const_iterator iterBegin = CEffectGroupManager::Instance().GetCreatedList().begin();
	CEffectGroupManager::my_list::const_iterator iterEnd = CEffectGroupManager::Instance().GetCreatedList().end();
	CEffectGroupManager::my_list::const_iterator iter;
	for(iter = iterBegin; iter != iterEnd; ++iter)
	{
		CEffectGroup *pEG = *iter;

		if (pEG->GetgERType() != erType) { continue; }
	
		pEG->SetProjection(prProjection);
		if(pEG->Active() && !pEG->Process(fCurrentTime))
		{
			listDelete.push_back(pEG);
		}
	}
	for(CEffectGroupManager::my_list::iterator it=listDelete.begin(); it!=listDelete.end(); ++it)
	{
		if ((*it)->GetgERType() != erType) { continue; }
		CEffectGroupManager::Instance().Destroy(*it);
	}
	listDelete.clear();
}

static EFF_RENDER_TYPE g_ERType = ER_NORMAL;

//class for render
class CRenderFunc
{
public:
	inline void operator()(CEffect *pEffect)
	{
		if(pEffect->GetERType() == g_ERType && pEffect->Playing())
			pEffect->Render();
	}
};

/* Render effect */
void CRenderer::RenderEffects(BOOL bBackground, EFF_RENDER_TYPE erType /* = ER_NORMAL */,
							  CAnyProjection3D* ppr /* = NULL */, CDrawPort* pdp /* = NULL */)
{
	CAnyProjection3D *papr;
	
	if (ppr == NULL)
	{
		if( bBackground) papr = &re_prBackgroundProjection;
		else papr = &re_prProjection;
	}
	else
	{
		papr = ppr;
	}

	CAnyProjection3D &prProjection = *papr;
	CDrawPort*	pdpDrawPort = re_pdpDrawPort;

	if (pdp != NULL)
	{
		pdpDrawPort = pdp;
	}

	papr = NULL;
	g_ERType = erType;
	
	CRenderFunc renderFunc;
	for(DWORD dwEffectType=0; dwEffectType<(DWORD)ET_COUNT; ++dwEffectType)
	{
		EFFECT_TYPE effectType = (EFFECT_TYPE)dwEffectType;

		CEffectManager::my_list &listCreated = CEffectManager::Instance().GetCreatedList(effectType);
		if(listCreated.empty()) continue;
		if(effectType != ET_MDL)
		{
			if((*listCreated.begin())->BeginRender(prProjection, pdpDrawPort))
			{
				std::for_each(listCreated.begin(), listCreated.end(), renderFunc);
				(*listCreated.begin())->EndRender(FALSE);
			}
		}
		else
		{
			if((*listCreated.begin())->BeginRender(prProjection, pdpDrawPort))
			{
				CEffectManager::my_list::const_iterator iter;
				for(iter=listCreated.begin(); iter!=listCreated.end(); ++iter)
				{
					if((*iter)->GetERType() == g_ERType && (*iter)->Playing() && !((CMdlEffect*)(*iter))->GetOverDraw())
						(*iter)->Render();
				}
				(*listCreated.begin())->EndRender(FALSE);
			}
		}
	}
	CEffectManager::my_list &listCreated = CEffectManager::Instance().GetCreatedList(ET_MDL);
	if(listCreated.empty()) return;
	if((*listCreated.begin())->BeginRender(prProjection, pdpDrawPort))
	{
		CEffectManager::my_list::const_iterator iter;
		for(iter=listCreated.begin(); iter!=listCreated.end(); ++iter)
		{
			if((*iter)->GetERType() == g_ERType && (*iter)->Playing() && ((CMdlEffect*)(*iter))->GetOverDraw())
				(*iter)->Render();
		}
		(*listCreated.begin())->EndRender(FALSE);
	}
}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

extern INDEX g_bShowTerrain;
extern INDEX g_bShowBSP;
extern INDEX g_bShowModel;
extern INDEX g_iShowReflectionMap;

//안태훈 수정 시작	//(Option)(0.1)
extern INDEX g_iUseBloom;
//안태훈 수정 끝	//(Option)(0.1)

// draw the prepared things to screen
void CRenderer::DrawToScreen(void)
{
	// sehan
//안태훈 수정 시작	//(Option)(0.1)
	if( re_iIndex == IDX_MAIN_RENDERER && re_prProjection->IsPerspective())
	{		
		if (re_bRenderBloom && g_iUseBloom > 0) 
		{
			HRESULT hr = InitBloom();
			if (FAILED(hr)) 
			{
				g_iUseBloom = 0;
			}
		}
	}
//안태훈 수정 끝	//(Option)(0.1)
	// sehan end
	
//안태훈 수정 시작	//(Easy Use World Editor)(0.1)
	extern INDEX g_bShowTerrain;
	extern INDEX g_bShowBSP;
	extern INDEX g_bShowModel;
//안태훈 수정 끝	//(Easy Use World Editor)(0.1)

	extern INDEX g_iShadowDetail;

	ChangeStatsMode(CStatForm::STI_WORLDRENDERING);
	
	//------------------------------------------------- first render background
	// if polygons should be drawn
	if (!re_bRenderingShadows &&
		_wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE) 
	{
		if( re_bBackgroundEnabled) 
		{
			// render the polygons to screen
			CPerspectiveProjection3D *pprPerspective =
				(CPerspectiveProjection3D *)(CProjection3D *)(re_prBackgroundProjection);
			pprPerspective->Prepare();
			RenderScene( re_pdpDrawPort, re_pspoFirstBackground, re_prBackgroundProjection, re_colSelection, FALSE);
		}
		else 
		{
			// this is just for far sentinel
			RenderSceneBackground( re_pdpDrawPort, re_spoFarSentinel.spo_spoScenePolygon.spo_cColor);
		}
	}

	extern BOOL		_bWorldEditorApp;
	//wooss 050822
	
	
	if (re_bBackgroundEnabled) 
	{
		// render models that were kept for delayed rendering.
		ChangeStatsMode(CStatForm::STI_MODELSETUP);
		RenderModels(TRUE);   // render background models
		ChangeStatsMode(CStatForm::STI_WORLDRENDERING);
	}

	// if polygons should be drawn
	if (!re_bRenderingShadows &&
		re_bBackgroundEnabled
		&&_wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE) 
	{
		// render translucent portals
		CPerspectiveProjection3D *pprPerspective = (CPerspectiveProjection3D*)(CProjection3D*)(re_prBackgroundProjection);
		RenderScene( re_pdpDrawPort, SortTranslucentPolygons(re_pspoFirstBackgroundTranslucent),
			re_prBackgroundProjection, re_colSelection, TRUE);
	}

	if( re_bBackgroundEnabled) 
	{
		ChangeStatsMode(CStatForm::STI_PARTICLERENDERING);
		RenderParticles(TRUE); // render background particless
		ChangeStatsMode(CStatForm::STI_WORLDRENDERING);
	}

	//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	//render effect, 일단 백그라운드 이펙트는 없음.
	//ChangeStatsMode(CStatForm::STI_EFFECT);
	//ProcessEffects();
	//RenderEffects(TRUE); // render background effects
	//ChangeStatsMode(CStatForm::STI_WORLDRENDERING);
	//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)	

	//------------------------------------------------- second render non-background
	// if polygons should be drawn
	//안태훈 수정 시작	//(Easy Use World Editor)(0.1)
	if( !re_bRenderingShadows
		&& _wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE
		&& g_bShowBSP) 
		//안태훈 수정 끝	//(Easy Use World Editor)(0.1)
	{
		// render the spans to screen
		re_prProjection->Prepare();
		CPerspectiveProjection3D *pprPerspective = (CPerspectiveProjection3D*)(CProjection3D*)re_prProjection;
		RenderScene( re_pdpDrawPort, re_pspoFirst, re_prProjection, re_colSelection, FALSE);
	}

	// Render active terrains
	//안태훈 수정 시작	//(Easy Use World Editor)(0.1)
	//g_bShowTerrain = FALSE;			// inserted by seo 4 test.
	if( !re_bRenderingShadows
		&& _wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE
		&& g_bShowTerrain) 
		//안태훈 수정 끝	//(Easy Use World Editor)(0.1)
	{
		RenderTerrains();
	}

	//강동민 수정 시작
	CTerrain *ptrTerrain = NULL;
	FORDELETELIST(CTerrain, tr_lnInActiveTerrains, re_lhActiveTerrains, ittr) 
	{
		ptrTerrain = ittr;
	}

	extern CNetworkLibrary *_pNetwork;

	if(ptrTerrain)
	{
		extern INDEX ter_bShowCharacterShadow;		// 추가됨.	

		// 캐릭터 그림자를 지형에 그립니다.
		// FIXME : 싱글모드에서는 그림자를 그리지 않음.(속도 향상을 위해서...)
		if(g_iShadowDetail == SHADOW_PROJ_ALL || g_iShadowDetail == SHADOW_PROJ_ONLY)
		{
			if(ter_bShowCharacterShadow && !_pNetwork->m_bSingleMode)
			{
				if( re_iIndex == IDX_MAIN_RENDERER)
					RenderSkaModelShadowToTerrain(ptrTerrain, re_prProjection.ap_CurrentProjection->pr_ViewerPlacement);
			}
		}
	}

	// if wireframe should be drawn
	if( !re_bRenderingShadows &&
		( _wrpWorldRenderPrefs.wrp_ftEdges     != CWorldRenderPrefs::FT_NONE
		|| _wrpWorldRenderPrefs.wrp_ftVertices  != CWorldRenderPrefs::FT_NONE
		|| _wrpWorldRenderPrefs.wrp_stSelection == CWorldRenderPrefs::ST_VERTICES
		|| _wrpWorldRenderPrefs.IsFieldBrushesOn())) 
	{
		// render in wireframe all brushes that were added (in orthographic projection!)
		re_pdpDrawPort->SetOrtho();
		//안태훈 수정 시작	//(Easy Use World Editor)(0.1)
		if(g_bShowBSP) RenderWireFrameBrushes();
		if(g_bShowTerrain) RenderWireFrameTerrains();
		//안태훈 수정 끝	//(Easy Use World Editor)(0.1)
	}

	// render models that were kept for delayed rendering
	ChangeStatsMode(CStatForm::STI_MODELSETUP);
	//안태훈 수정 시작	//(Easy Use World Editor)(0.1)
	if(g_bShowModel) 
	{
		RenderModels(FALSE); // render non-background models

		//_pGfx->gl_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	//안태훈 수정 끝	//(Easy Use World Editor)(0.1)
		ChangeStatsMode(CStatForm::STI_PARTICLERENDERING);
		RenderParticles(FALSE); // render non-background particles
		ChangeStatsMode(CStatForm::STI_WORLDRENDERING);

	//안태훈 수정 시작	//(Easy Use World Editor)(0.1)
	//강동민 수정 시작 물 퍼포먼스 작업
		extern INDEX g_iReflectionDetail;
		if(!re_prProjection->pr_bNiceWater && g_bShowModel && g_iReflectionDetail != REFLECTION_NONE)
	//강동민 수정 끝 물 퍼포먼스 작업
	//안태훈 수정 끝	//(Easy Use World Editor)(0.1)
			RenderNiceWaterModels();
		
		// if polygons should be drawn
	//안태훈 수정 시작	//(Easy Use World Editor)(0.1)
		if (!re_bRenderingShadows
			&&_wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE
			&& g_bShowBSP) 
	//안태훈 수정 끝	//(Easy Use World Editor)(0.1)
		{
			// render translucent portals
			CPerspectiveProjection3D *pprPerspective = (CPerspectiveProjection3D*)(CProjection3D*)re_prProjection;
			pprPerspective->Prepare();
			RenderScene( re_pdpDrawPort, SortTranslucentPolygons(re_pspoFirstTranslucent),
				re_prProjection, re_colSelection, TRUE);
		}

	//강동민 수정 시작 접속 시퀀스 작업	06.07
		// NOTE : 아래 루틴을 Terrain에 그림자를 그리는 루틴과 분리해서 생각할것인가?
		// NOTE : 합쳐져도 무관할거 같음.
		// NOTE : 속도가 느려질 수 있는 부분임.
		// NOTE : 왜냐하면, 쉐도우 맵을 2번 생성하게 되기때문(SKA를 총 3번 렌더링한다구 생각해보자.)
	//강동민 수정 시작 테스트 클라이언트 작업	06.16
		const BOOL bRenderShadowToBrush = TRUE;	
	//강동민 수정 끝 테스트 클라이언트 작업		06.16
	//안태훈 수정 시작	//(Easy Use World Editor)(0.1)
		if(bRenderShadowToBrush && g_bShowBSP && !_pNetwork->m_bSingleMode)
	//안태훈 수정 끝	//(Easy Use World Editor)(0.1)
		{
			if(g_iShadowDetail == SHADOW_PROJ_ALL || g_iShadowDetail == SHADOW_PROJ_ONLY)
			{
				if(re_iIndex == IDX_MAIN_RENDERER)
				{
					// Brush Polygon에 캐릭터 쉐도우를 그립니다.
					RenderShadowToBrushPolygon(re_prProjection->pr_ViewerPlacement);
				}
			}
		}
	//강동민 수정 끝 접속 시퀀스 작업	06.07
		
	//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
		if(re_pdpDrawPort)
		{
			//render effect
			ChangeStatsMode(CStatForm::STI_EFFECT);
			PrepareRender_EffectSystem(ptrTerrain, re_pwoWorld);
			ProcessEffects(FALSE);
			RenderEffects(FALSE); // render non-background effects
			ChangeStatsMode(CStatForm::STI_WORLDRENDERING);
		}
	//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
		
		// render lens flares
		if( !re_bRenderingShadows) 
		{
			ChangeStatsMode(CStatForm::STI_FLARESRENDERING);
			RenderLensFlares(); // (this also sets orthographic projection!)
			ChangeStatsMode(CStatForm::STI_WORLDRENDERING);
		}

		// sehan
	//안태훈 수정 시작	//(Option)(0.1)
		extern BOOL _bUseBloomInWorldEditor;
		if(re_iIndex == IDX_MAIN_RENDERER && re_prProjection->IsPerspective())
		{		
			if (re_bRenderBloom && g_iUseBloom > 0
				&& _bUseBloomInWorldEditor) 
			{
				RenderBloom();
			}
		}
	//안태훈 수정 끝	//(Option)(0.1)
		// sehan end
	
	
	//full screen effect render.
	//일단은 blind effect 처리만 hardcoding함.
	extern INDEX g_iShowName;
	INDEX iShowNameOld = g_iShowName;
	extern INDEX g_iShowNameItem;
	INDEX iShowNameItemOld = g_iShowNameItem;

	if(_pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_BLIND))
	{
		g_iShowName = -1;
		g_iShowNameItem = -1;
	}
/*
	class cStaticDealloc
	{
	public:
		cStaticDealloc(CEffectGroup *p) : m_p( p ) {}
		~cStaticDealloc()
		{
			delete m_p;
		}
		CEffectGroup *m_p;
	};
	static bool bFirst = true;
	static cStaticDealloc blindEG(NULL);
	if(bFirst)
	{
		bFirst = false;
		CEffectGroup *pBlindOld = StartEffectGroup("STATE"
			, &CEntity::GetPlayerEntity(0)->en_pmiModelInstance->m_tmSkaTagManager
			,_pTimer->GetLerpedCurrentTick());
		blindEG.m_p = pBlindOld->Copy();
		DestroyEffectGroup(pBlindOld);
	}
	if((_pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_BLIND))
		&& blindEG.m_p != NULL)
	{
		g_iShowName = 0;
		if( !blindEG.m_p->Process(_pTimer->GetLerpedCurrentTick()) )
		{
			DestroyEffectGroup(blindEG.m_p);
			bFirst = true;
		}
		else
		{
			for(INDEX i=0; i<blindEG.m_p->GetEffectCount(); ++i)
			{
				if(blindEG.m_p->GetEffectKeyVector()[i].m_pCreatedEffect->GetType() == ET_MDL)
				{
					blindEG.m_p->BeginRender(prProjection, re_pdpDrawPort);
					blindEG.m_p->GetEffectKeyVector()[i].m_pCreatedEffect->Render();
					blindEG.m_p->EndRender(FALSE);
				}
			}
		}
	}
*/


	// User interface
	if( re_iIndex == IDX_MAIN_RENDERER && re_pdpDrawPort )
	{
		CUIManager::getSingleton()->Render( re_pdpDrawPort, re_prProjection );
		g_iShowName = iShowNameOld;
		g_iShowNameItem = iShowNameItemOld;
	}

	extern BOOL		_bWorldEditorApp;
	// 월드에디터 상에서 타겟과의 관계를 보여주는 부분임.
	// if entity targets should be drawn
	if( _bWorldEditorApp )
	{
		if( !re_bRenderingShadows && _wrpWorldRenderPrefs.wrp_bShowTargetsOn) 
		{
			// render entity targets
			RenderEntityTargets();
		}
		
		// if entity targets should be drawn
		if( !re_bRenderingShadows && _wrpWorldRenderPrefs.wrp_bShowEntityNames) 
		{
			RenderEntityNames();
		}
	}
	
	// NOTE : 디버깅용.
	// NOTE : 반사맵을 보여줍니다.
	extern INDEX g_iShowReflectionMap;
	if(!re_prProjection->pr_bNiceWater && g_iShowReflectionMap && re_prtReflection)
	{
		gfxDisableTexture();
		gfxDisableBlend();
		CTextureObject		toReflect;
		toReflect.SetData(&re_prtReflection->rt_tdTexture);				
		MEXaabbox2D boxReflectionTexture;
		re_pdpDrawPort->PutTexture(&toReflect, PIXaabbox2D( PIX2D(0, 0), PIX2D(128 * g_iShowReflectionMap, 128 * g_iShowReflectionMap)));
	}

	// clean all buffers after rendering
	re_aspoScreenPolygons.PopAll();
	re_admDelayedModels.PopAll();
	re_cenDrawn.PopAll();
	re_avvxViewVertices.PopAll();
}


// draw mirror polygons to z-buffer to enable drawing of mirror
void CRenderer::FillMirrorDepth(CMirror &mi)
{
	// create a list of scene polygons for mirror
	ScenePolygon *pspoFirst = NULL;
	// for each polygon
	FOREACHINDYNAMICCONTAINER(mi.mi_cspoPolygons, CScreenPolygon, itspo) 
	{
		CScreenPolygon &spo = *itspo;
		CBrushPolygon &bpo = *spo.spo_pbpoBrushPolygon;
		// create a new screen polygon
		CScreenPolygon &spoNew = re_aspoScreenPolygons.Push();
		ScenePolygon &sppoNew = spoNew.spo_spoScenePolygon;
		// add it to mirror list
		sppoNew.spo_pspoSucc = pspoFirst;
		pspoFirst = &sppoNew;
		
		// use same triangles
		sppoNew.spo_iVtx0 = spo.spo_spoScenePolygon.spo_iVtx0;
		sppoNew.spo_ctVtx = spo.spo_spoScenePolygon.spo_ctVtx;
		sppoNew.spo_piElements = spo.spo_spoScenePolygon.spo_piElements;
		sppoNew.spo_ctElements = spo.spo_spoScenePolygon.spo_ctElements;
	}
	
	// render all those polygons just to clear z-buffer
	RenderSceneZOnly( re_pdpDrawPort, pspoFirst, re_prProjection);
}

//강동민 수정 시작 접속 시퀀스 작업	06.01
void CRenderer::RenderShadowToBrushPolygon(const CPlacement3D &plEye)
{
	if(re_avvxViewVertices.Count() <= 0)		return;

	extern INDEX g_iShadowDetail;

	//gfxCullFace(GFX_NONE);
	//gfxPolygonMode(GFX_FILL);
	gfxEnableBlend();	

	// 사용할 그림자 텍스쳐 생성.
	if(_bInitShadowTexture == FALSE)
	{
		if(_prtShadow)
		{
			delete _prtShadow;
			_prtShadow = NULL;
		}
		_prtShadow = new CRenderTexture();
		_prtShadow->Init(SHADOWTEXTURESIZE, SHADOWTEXTURESIZE);
		_bInitShadowTexture = TRUE;
	}
	
	// 이전의 상태 저장.
	INDEX iOldFilter, iOldAnisotropy;
	gfxGetTextureFiltering( iOldFilter, iOldAnisotropy);
	
	INDEX iNewFilter = 202, iNewAnisotropy = 1;
	gfxSetTextureFiltering( iNewFilter, iNewAnisotropy);

	gfxEnableDepthBias();
	
	gfxBlendFunc(
		GFX_SRC_ALPHA,				// 입혀질...
		GFX_INV_SRC_ALPHA);			// 바닥

	CPlacement3D plLight;
	HRESULT hr;
	//-------------------------------------------------------------------------------
	// 모델들에 대해서 처리함.
	for( INDEX iModel=0; iModel<CRenderer::re_admDelayedModels.Count(); iModel++) 
	{
		CDelayedModel &dm	= CRenderer::re_admDelayedModels[iModel];
		CEntity &en			= *dm.dm_penModel;
		
		if (en.GetRenderType()==CEntity::RT_SKAMODEL || en.GetRenderType()==CEntity::RT_SKAEDITORMODEL)
		{
			CModelInstance &mi = *en.GetModelInstance();
			if(!en.IsPlayer() && !mi.mi_bRenderShadow)			
				continue;
			// skip invisible models
			if( mi.mi_vStretch==FLOAT3D(0,0,0)) continue;				

			if(en.GetFlags() & ENF_ITEM)
				continue;
			
			// NOTE : 플레이어의 그림자만 보이도록 함.
			if(g_iShadowDetail == SHADOW_PROJ_ONLY)
			{
				if(!en.IsPlayer())				continue;
			}
			
			const SLONG slModelAlpha = (mi.mi_colModelColor & CT_AMASK) >>CT_ASHIFT;
			if( slModelAlpha<4)					continue;
			
			BOOL bIsBackground = re_bBackgroundEnabled && (en.en_ulFlags&ENF_BACKGROUND);
			BOOL bBackground = FALSE;
			if(  (bBackground && !bIsBackground)
				|| (!bBackground &&  bIsBackground)
				|| !(dm.dm_ulFlags&DMF_VISIBLE)) continue;

			CPlacement3D plModel = en.GetLerpedPlacement();
			
			extern BOOL	_bInTestGame;
			if(CEntity::GetPlayerEntity(0) != NULL)
			{
				CPlacement3D plPlayer	= CEntity::GetPlayerEntity(0)->GetLerpedPlacement();
				FLOAT3D pl_Delta		= plModel.pl_PositionVector - plPlayer.pl_PositionVector;
				FLOAT fLength			= pl_Delta.Length();
				if(fLength >= _fSHDPlayerDistance)			continue;
			}
			else if(!_bInTestGame)
			{
				CPlacement3D plViewer	= re_prProjection->ViewerPlacementR();
				FLOAT3D pl_Delta		= plModel.pl_PositionVector - plViewer.pl_PositionVector;
				FLOAT fLength			= pl_Delta.Length();
				if(fLength >= _fSHDCameraDistance)		continue;
			}
			
			// create a default light
			COLOR colLight   = C_GRAY;
			COLOR colAmbient = C_dGRAY;
			FLOAT3D vTotalLightDirection( 1.0f, -1.0f, 1.0f);
			FLOATplane3D plFloorPlane(FLOAT3D( 0.0f, 1.0f, 0.0f), 0.0f);	
			
			FLOAT fTotalShadowIntensity = 0.0f;		// 원본
			BOOL bRenderModelShadow = FALSE;		// 원본
			
			if( !re_bRenderingShadows)
			{
				mdl_iShadowQuality = Clamp( mdl_iShadowQuality, 0L, 3L);
				// find model lights only if not custom shaded
				if(en.IsFlagOff(ENF_CUSTOMSHADING))
				{
					bRenderModelShadow = FindModelLights( en, plModel, colLight, colAmbient, 
						fTotalShadowIntensity, vTotalLightDirection, plFloorPlane);
				} // let the entity adjust shading parameters if it wants to
				const BOOL bAllowShadows = en.AdjustShadingParameters( vTotalLightDirection, colLight, colAmbient);
			}
			
			//////////////////////////////////////////////////////////////////////////
			// 02.20. 금.
			// NOTE : 각도를 제한합니다(240도에서 300도로 제한했음.)
			ANGLE3D vAngleLightTemp;
			DirectionVectorToAngles(vTotalLightDirection, vAngleLightTemp);
			if(vAngleLightTemp(2) < 0)
			{
				vAngleLightTemp(2) = vAngleLightTemp(2) + 360.0f;
			}
			vAngleLightTemp(2) = Clamp(vAngleLightTemp(2), 220.0f, 320.0f);	// Pitch
			AnglesToDirectionVector(vAngleLightTemp, vTotalLightDirection);
			
			// Directional
			if(_amlLights.Count() <= 0) continue;
			
			//////////////////////////////////////////////////////////////////////////
			// 추출할 면의 면적 구하기.
			FLOATaabbox3D FrameBBox;
			mi.GetAllFramesBBox(FrameBBox);
			FrameBBox.StretchByVector(mi.mi_vStretch);
			FLOAT3D vMin = FrameBBox.minvect;
			FLOAT3D vMax = FrameBBox.maxvect;
			FLOAT3D vLength = vMin - vMax;
			
			// NOTE : 모델의 비율을 감안하여 이동시켜야합니다.
			plLight.pl_PositionVector = -vTotalLightDirection * (vLength.Length() * 1.2f);
			plLight.pl_OrientationAngle = vAngleLightTemp;
			
			//////////////////////////////////////////////////////////////////////////
			// Shadow Map 생성.
			D3DXMATRIX matProjOld;
			hr = _pGfx->gl_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProjOld);
			
			// 02.23 수정.
			D3DVIEWPORT8 vpOld;
			D3DVIEWPORT8 vpNew;
			_pGfx->gl_pd3dDevice->GetViewport(&vpOld);
			
			_prtShadow->Begin();				
			_prtShadow->Clear(0x00050505);
			RM_SetRenderingType(1);
			
			vpNew = vpOld;
			vpNew.X = 1;
			vpNew.Y = 1;
			vpNew.Width = SHADOWTEXTURESIZE-2;
			vpNew.Height = SHADOWTEXTURESIZE-2;
			_pGfx->gl_pd3dDevice->SetViewport(&vpNew);
			
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0);				D3D_CHECKERROR(hr);
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);	D3D_CHECKERROR(hr);
			
			// Shadow Map을 생성하기 위해서 지정한 색으로 칠해진 모델을 렌더링합니다.
			RM_AddFlag(RMF_SHOWSHADOW);
			_bRenderProjectionShadow = TRUE;
			
			// SKA 모델 렌더링.
			RenderOneSkaModelToTexture(en, plModel, plLight, FALSE, dm.dm_fMipFactor, dm.dm_ulFlags);
			_bRenderProjectionShadow = FALSE;
			RM_RemoveFlag(RMF_SHOWSHADOW);
			_prtShadow->End();
			_pGfx->gl_pd3dDevice->SetViewport(&vpOld);
			RM_SetRenderingType(0);
			
			// 이전의 투영행렬로 복구시킵니다.(이 코드가 없으면 화면이 제대로 Clear되지 않음)
			hr = _pGfx->gl_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProjOld);
			//////////////////////////////////////////////////////////////////////////
			gfxEnableBlend();			
			
			//////////////////////////////////////////////////////////////////////////				
			Matrix16 matShadowUV;
			CreateShadowMatrix(
				&mi,
				en.GetLerpedPlacement(),							// SKA 모델의 위치.
				matShadowUV,										// 얻은 텍스쳐 투영 행렬
				_matWorldToLight,
				_matShadowProj,
				SHADOWTEXTURESIZE,									// 텍스쳐 크기
				plEye);												// 카메라 위치
			
			//////////////////////////////////////////////////////////////////////////
			// 텍스쳐 설정 및 텍스쳐 행렬 설정.
			gfxSetTextureUnit(0);
			_prtShadow->rt_tdTexture.SetAsCurrent();
			//gfxDisableTexture();
			
			gfxSetTextureWrapping( GFX_CLAMP, GFX_CLAMP);
			
			// NOTE : 엔진에서 설정하는 부분이 없으므로, 직접 설정했음.
			// 카메라 좌표계의 버텍스좌표를 텍스쳐 행렬의 입력으로 사용함.
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);	D3D_CHECKERROR(hr);
			hr = _pGfx->gl_pd3dDevice->SetTransform( D3DTS_TEXTURE0, (_D3DMATRIX*)&matShadowUV);						D3D_CHECKERROR(hr);
			
			// 동차좌표계 텍스쳐 변환 행렬을 사용하도록 설정해준다. 
			// 이렇게 해야 D3D가 투영 나눗셈(projection divide)연산을 한다.
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT4|D3DTTFF_PROJECTED );	D3D_CHECKERROR(hr);

		int iCount = _avtxScene.Count();
		if(iCount <= 0)				continue;

		FLOAT3D			vPoint;
		FLOATplane3D	plPlane;
		
		// NOTE : 아래 컨테이너를 루프 바깥으로 빼고서, 브러시 폴리곤을 한꺼번에 처리하도록 하자.
		CDynamicContainer<CBrushPolygon> dcPolygons;
		//en.GetNearPolygonsInSphere(35.0f, dcPolygons);
		
		// 모델의 위치에서 해당 구의 범위내의 폴리곤들을 추출해냅니다.
		// NOTE : 이와 같은 방식외에 다른 방식은 없을까???
		// NOTE : 모델의 바운딩 구의 범위내에 있는 모든 폴리곤들을 추출해냄...
		en.GetNearPolygonsInSphere(1.0f, dcPolygons);

		if(dcPolygons.Count() > 0)
		{
			// for each polyon in selection
			FOREACHINDYNAMICCONTAINER(dcPolygons, CBrushPolygon, itbpo)
			//for(int i = 0; i < dcPolygons.Count(); i++)
			{	
				//CBrushPolygon &bpo				= dcPolygons[i];
				CBrushPolygon &bpo				= *itbpo;
				CBrushSector &bsc				= *bpo.bpo_pbscSector;
				if(&bsc == NULL)					continue;
				if(bsc.bsc_ivvx0 < 0)				continue;
				
				// 화면에 보이는 폴리곤을 얻고,
				// (뷰 공간으로 변환된 정점.)
				if(bsc.bsc_ivvx0 >= re_avvxViewVertices.Count()) continue;
				const CViewVertex *pvvx0		= &re_avvxViewVertices[bsc.bsc_ivvx0];
				ASSERT(pvvx0 != NULL && "Invalid View Vertex");
				const INDEX ctVtx				= bpo.bpo_apbvxTriangleVertices.Count();
				const INDEX ctIdx				= bpo.bpo_aiTriangleElements.Count();
				
				// 보이는 폴리곤의 Primitive를 렌더링합니다.
				// NOTE : 아래와 같은 방식으로 렌더링하면 속도가 느릴듯...
				// NOTE : AddPolygonToScene를 사용해서 한꺼번에 렌더링해도 될것 같기도 함.
				{
					// 아랫에서 인덱스 얻고, 정점 얻고 하는 부분은 한번에 처리 가능할듯...
					for( INDEX iTri=0;iTri<ctIdx;iTri+=3)
					{
						const INDEX iind1		= bpo.bpo_aiTriangleElements[iTri+0];
						const INDEX iind2		= bpo.bpo_aiTriangleElements[iTri+1];
						const INDEX iind3		= bpo.bpo_aiTriangleElements[iTri+2];
						CBrushVertex &Brush0	= *bpo.bpo_apbvxTriangleVertices[iind1];
						CBrushVertex &Brush1	= *bpo.bpo_apbvxTriangleVertices[iind2];
						CBrushVertex &Brush2	= *bpo.bpo_apbvxTriangleVertices[iind3];

						FLOAT3D		vTriangleVtxs[3];
						
						INDEX iVtx				= bsc.bsc_abvxVertices.Index(&Brush0);
						vTriangleVtxs[0]		= pvvx0[iVtx].vvx_vView;
						
						iVtx					= bsc.bsc_abvxVertices.Index(&Brush1);
						vTriangleVtxs[1]		= pvvx0[iVtx].vvx_vView;
						
						iVtx					= bsc.bsc_abvxVertices.Index(&Brush2);
						vTriangleVtxs[2]		= pvvx0[iVtx].vvx_vView;
						
						//////////////////////////////////////////////////////////////////////////
						// NOTE : Render Triangle
						GFXColor		colArray[3];

						// set Primitive color
						COLOR col				= 0xFFFFFFFF;
						colArray[0] = col; colArray[1] = col; colArray[2] = col;						
	
						const UWORD auwIndices[3] = {0, 1, 2};
						
						gfxSetVertexArray( (GFXVertex*)&vTriangleVtxs[0], 3);
						//gfxSetTexCoordArray( tcTriangleTex, FALSE);
						gfxSetColorArray(&colArray[0]);
						gfxDrawElements( 3, auwIndices);
					}
				}
			}
			dcPolygons.Clear();		
		}
	}
	}
	//-------------------------------------------------------------------------------
	hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU );		D3D_CHECKERROR(hr);
	hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);		D3D_CHECKERROR(hr);

	gfxDisableTexture();
	shaClearTextureMatrix();
		
	gfxDisableDepthBias();
	// 이전의 필터링 모드로 복구.
	gfxSetTextureFiltering( iOldFilter, iOldAnisotropy);
	
	gfxDisableBlend();
	//gfxCullFace(GFX_FRONT);
	//gfxPolygonMode(GFX_FILL);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *ptrTerrain - 
//			&plEye - 
//-----------------------------------------------------------------------------
void CRenderer::RenderSkaModelShadowToTerrain(CTerrain *ptrTerrain, const CPlacement3D &plEye)
{
	extern INDEX g_iShadowDetail;
	// profile rendering
	const BOOL bModelSetupTimer = _sfStats.CheckTimer(CStatForm::STI_MODELSETUP);
	if( bModelSetupTimer) _sfStats.StopTimer(CStatForm::STI_MODELSETUP);
	_sfStats.StartTimer(CStatForm::STI_SHADOWRENDERING);
	
	// NOTE : 그림자를 그릴지 안 그릴지 플래그 필요(전체적으로 적용할 부분...)
	BOOL bRenderShadow = TRUE;
	if(bRenderShadow)
	{
		// NOTE : 이전 상태가 Blend 되는 상태이므로 꺼줘야 함.
//강동민 수정 시작		03.22
		//gfxDisableBlend();
//강동민 수정 끝		03.22
		
		//////////////////////////////////////////////////////////////////////////
		// 사용할 그림자 텍스쳐 생성.
		if(_bInitShadowTexture == FALSE)
		{
			if(_prtShadow)
			{
				delete _prtShadow;
				_prtShadow = NULL;
			}
			_prtShadow = new CRenderTexture();
			_prtShadow->Init(SHADOWTEXTURESIZE, SHADOWTEXTURESIZE);
			_bInitShadowTexture = TRUE;
		}
		
		// 이전의 상태 저장.
		INDEX iOldFilter, iOldAnisotropy;
		gfxGetTextureFiltering( iOldFilter, iOldAnisotropy);
		
		// 외곽선을 해결하기 위해서 필터링 적용
		// 필터링 off
		// 확대 될경우엔 아무래도 필터링을 켜는것이 보기 좋다.
		// 그러나 스프라이트는 투명색과 필터링이 일어나기때문에
		// 투명색과의 경계부분이 테두리가 생길수있다.	
		INDEX iNewFilter=202, iNewAnisotropy=1;
		gfxSetTextureFiltering( iNewFilter, iNewAnisotropy);
		
		gfxEnableDepthBias();
		
		FLOAT3D *pavVertices	= NULL;			// 정점 포인터.
		UWORD *puwIndices		= NULL;			// 인덱스 포인터.
		INDEX ctVertices		= 0;			// 정점의 갯수.
		INDEX ctIndices			= 0;			// 인덱스의 개수.		
		
		//////////////////////////////////////////////////////////////////////////
		// 그림자가 겹치지 않도록 처리하기 위해서 스텐실을 사용하고 있음.
		/*
		HRESULT hr;	
		hr = _pGfx->gl_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);				D3D_CHECKERROR(hr);
		hr = _pGfx->gl_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);			D3D_CHECKERROR(hr);
		hr = _pGfx->gl_pd3dDevice->SetRenderState(D3DRS_STENCILREF,   0);					D3D_CHECKERROR(hr);
		hr = _pGfx->gl_pd3dDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);	D3D_CHECKERROR(hr);
		
		// NOTE : Shadow Map을 생성 혹은...
		hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	D3D_CHECKERROR(hr);
		// NOTE : 필터링 적용후 외곽선 문제를 해결하기 위한...
		//hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,  1);
		hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,  0x40);					D3D_CHECKERROR(hr);
		*/
		
		gfxBlendFunc(
			GFX_SRC_ALPHA,			// 입혀질...
			GFX_INV_SRC_ALPHA);		// 바닥
		
		CPlacement3D plLight;
		//////////////////////////////////////////////////////////////////////////
		for( INDEX iModel=0; iModel<re_admDelayedModels.Count(); iModel++) 
		{
			CDelayedModel &dm = re_admDelayedModels[iModel];
			CEntity &en = *dm.dm_penModel;
			
			// ska 모델일 경우.
			if( en.en_RenderType == CEntity::RT_SKAMODEL || en.en_RenderType == CEntity::RT_SKAEDITORMODEL)
			{
				CModelInstance &mi = *en.GetModelInstance();
				
				if(!en.IsPlayer() && !mi.mi_bRenderShadow)			continue;
				// skip invisible models
				if( mi.mi_vStretch==FLOAT3D(0,0,0)) continue;				
				
				const SLONG slModelAlpha = (mi.mi_colModelColor & CT_AMASK) >>CT_ASHIFT;
				if( slModelAlpha<4)					continue;

				if(en.GetFlags() & ENF_ITEM)
					continue;

				// NOTE : 플레이어의 그림자만 보이도록 함.
				if(g_iShadowDetail == SHADOW_PROJ_ONLY)
				{
					if(!en.IsPlayer())				continue;
					}
				
				BOOL bIsBackground = re_bBackgroundEnabled && (en.en_ulFlags&ENF_BACKGROUND);
				BOOL bBackground = FALSE;
				if(  (bBackground && !bIsBackground)
					|| (!bBackground &&  bIsBackground)
					|| !(dm.dm_ulFlags&DMF_VISIBLE)) continue;		// Visible Check
				
				CPlacement3D plModel = en.GetLerpedPlacement();
				extern BOOL	_bInTestGame;
				if(CEntity::GetPlayerEntity(0) != NULL)
				{
					//if(_pMajorEntity == &en)		continue;
					CPlacement3D plPlayer = CEntity::GetPlayerEntity(0)->GetLerpedPlacement();
					FLOAT3D pl_Delta = plModel.pl_PositionVector - plPlayer.pl_PositionVector;
					FLOAT fLength = pl_Delta.Length();
					if(fLength >= _fSHDPlayerDistance)			continue;
				}
				else if(!_bInTestGame)
				{
					CPlacement3D plViewer	= re_prProjection->ViewerPlacementR();
					FLOAT3D pl_Delta		= plModel.pl_PositionVector - plViewer.pl_PositionVector;
					FLOAT fLength			= pl_Delta.Length();
					if(fLength >= _fSHDCameraDistance)		continue;
				}
				
				// create a default light
				COLOR colLight   = C_GRAY;
				COLOR colAmbient = C_dGRAY;
				FLOAT3D vTotalLightDirection( 1.0f, -1.0f, 1.0f);
				FLOATplane3D plFloorPlane(FLOAT3D( 0.0f, 1.0f, 0.0f), 0.0f);	
				
				FLOAT fTotalShadowIntensity = 0.0f;		// 원본
				BOOL bRenderModelShadow = FALSE;		// 원본
				
				if( !re_bRenderingShadows)
				{
					mdl_iShadowQuality = Clamp( mdl_iShadowQuality, 0L, 3L);
					// find model lights only if not custom shaded
					if(en.IsFlagOff(ENF_CUSTOMSHADING))
					{
						bRenderModelShadow = FindModelLights( en, plModel, colLight, colAmbient, 
							fTotalShadowIntensity, vTotalLightDirection, plFloorPlane);
					} // let the entity adjust shading parameters if it wants to
					const BOOL bAllowShadows = en.AdjustShadingParameters( vTotalLightDirection, colLight, colAmbient);
				}
				
				//////////////////////////////////////////////////////////////////////////
				// 02.20. 금.
				// NOTE : 각도를 제한합니다(240도에서 300도로 제한했음.)
				ANGLE3D vAngleLightTemp;
				DirectionVectorToAngles(vTotalLightDirection, vAngleLightTemp);
				if(vAngleLightTemp(2) < 0)
				{
					vAngleLightTemp(2) = vAngleLightTemp(2) + 360.0f;
				}
				vAngleLightTemp(2) = Clamp(vAngleLightTemp(2), 220.0f, 320.0f);	// Pitch
				AnglesToDirectionVector(vAngleLightTemp, vTotalLightDirection);
				
				// Directional
				if(_amlLights.Count() <= 0) continue;
				
				//////////////////////////////////////////////////////////////////////////
				// 추출할 면의 면적 구하기.
				FLOATaabbox3D FrameBBox;
				mi.GetAllFramesBBox(FrameBBox);
				FrameBBox.StretchByVector(mi.mi_vStretch);
				FLOAT3D vMin = FrameBBox.minvect;
				FLOAT3D vMax = FrameBBox.maxvect;
				FLOAT3D vLength = vMin - vMax;
				
				// NOTE : 모델의 비율을 감안하여 이동시켜야합니다.
				plLight.pl_PositionVector = -vTotalLightDirection * (vLength.Length() * 1.2f);
				//plLight.pl_PositionVector(2) += fHeight/2.0f;
				plLight.pl_OrientationAngle = vAngleLightTemp;
				
				//////////////////////////////////////////////////////////////////////////
				// Shadow Map 생성.
				D3DXMATRIX matProjOld;
				HRESULT hr = _pGfx->gl_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProjOld);				
				
				// 02.23 수정.
				D3DVIEWPORT8 vpOld;
				D3DVIEWPORT8 vpNew;
				_pGfx->gl_pd3dDevice->GetViewport(&vpOld);
				
				_prtShadow->Begin();				
				//				_prtShadow->Clear(0x00FFFFFF);
				_prtShadow->Clear(0x00050505);
				RM_SetRenderingType(1);
				
				vpNew = vpOld;
				vpNew.X = 1;
				vpNew.Y = 1;
				vpNew.Width = SHADOWTEXTURESIZE-2;
				vpNew.Height = SHADOWTEXTURESIZE-2;
				_pGfx->gl_pd3dDevice->SetViewport(&vpNew);
				
				// Shadow Map을 생성하기 위해서 지정한 색으로 칠해진 모델을 렌더링합니다.
				RM_AddFlag(RMF_SHOWSHADOW);
				_bRenderProjectionShadow = TRUE;
				
				// SKA 모델 렌더링.
				RenderOneSkaModelToTexture(en, plModel, plLight, FALSE, dm.dm_fMipFactor, dm.dm_ulFlags);

				_bRenderProjectionShadow = FALSE;
				RM_RemoveFlag(RMF_SHOWSHADOW);
				_prtShadow->End();
				_pGfx->gl_pd3dDevice->SetViewport(&vpOld);
				RM_SetRenderingType(0);
				
				// 이전의 투영행렬로 복구시킵니다.(이 코드가 없으면 화면이 제대로 Clear되지 않음)
				hr = _pGfx->gl_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProjOld);
				//////////////////////////////////////////////////////////////////////////
				//gfxEnableAlphaTest();
				gfxEnableBlend();
				
				// 추출 범위를 조절하기 위해서 Terrain의 Stretch행렬의 역을 곱합니다.
				Matrix12 matStretch;
				TR_GetStretchMatrix(matStretch);
				matStretch[0] = 1.0f/matStretch[0];
				matStretch[5] = 1.0f;
				matStretch[10] = 1.0f/matStretch[10];
				TransformVector(plModel.pl_PositionVector.vector, matStretch);			
				
				FLOAT fSize = vLength.Length() * 0.8f;
				
				// 추출할 영역 설정.
				CTRect rcExtract(
					plModel.pl_PositionVector(1) - fSize,	// Left
					plModel.pl_PositionVector(3) - fSize,	// Top
					plModel.pl_PositionVector(1) + fSize,	// Right
					plModel.pl_PositionVector(3) + fSize);	// Bottom
				
				// Terrain으로부터 면을 추출함.
				TR_ExtractPoligonsInRect(ptrTerrain, rcExtract, &pavVertices, ctVertices, &puwIndices, ctIndices);
				if(pavVertices == NULL || puwIndices == NULL || ctVertices == 0 || ctIndices == 0)	break;
				
				//////////////////////////////////////////////////////////////////////////				
				Matrix16 matShadowUV;
				CreateShadowMatrix(
					&mi,
					en.GetLerpedPlacement(),							// SKA 모델의 위치.
					matShadowUV,										// 얻은 텍스쳐 투영 행렬
					_matWorldToLight,
					_matShadowProj,
					SHADOWTEXTURESIZE,									// 텍스쳐 크기
					plEye);												// 카메라 위치
				
				//////////////////////////////////////////////////////////////////////////
				// 텍스쳐 설정 및 텍스쳐 행렬 설정.
				gfxSetTextureUnit(0);
				_prtShadow->rt_tdTexture.SetAsCurrent();
					
				
				gfxSetTextureWrapping( GFX_CLAMP, GFX_CLAMP);
				
				// NOTE : 엔진에서 설정하는 부분이 없으므로, 직접 설정했음.
				// 카메라 좌표계의 버텍스좌표를 텍스쳐 행렬의 입력으로 사용함.
				hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION );		D3D_CHECKERROR(hr);
				hr = _pGfx->gl_pd3dDevice->SetTransform( D3DTS_TEXTURE0, (_D3DMATRIX*)&matShadowUV);							D3D_CHECKERROR(hr);
				
				// 동차좌표계 텍스쳐 변환 행렬을 사용하도록 설정해준다. 
				// 이렇게 해야 D3D가 투영 나눗셈(projection divide)연산을 한다.
				hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT4|D3DTTFF_PROJECTED );	D3D_CHECKERROR(hr);
				
				//////////////////////////////////////////////////////////////////////////
				// Fog 및 Haze의 적용.
				// NOTE : 2월 11일 작업 내용.			
				// NOTE : 그림자를 캐릭터가 Fog내에 있을 때 처리하는 경우에는 캐릭터가 Fog 위쪽있을경우,
				// NOTE : 그림자는 Terrain위에 생기게 된다.  즉, 그림자는 Terrain위에 생기므로,
				// NOTE : 지형에 Fog가 적용되어있을 때 처리해줌.
				// NOTE : 각 정점마다 Fog와 Haze로부터 영향받은 알파값을 계산해야 하기 때문에 Triangle마다 처리했으나,
				// NOTE : 최적화가 가능한 부분(추후 작업)
				// NOTE : 전체 정점의 알파값을 미리 변경한뒤에 한꺼번에 렌더링하는 식으로 수정할것.
				
				Matrix12 matObjToView;
				TR_GetObjToViewMatrix(matObjToView);
				SetViewMatrix(matObjToView);
				
				gfxSetVertexArray((GFXVertex*)pavVertices, ctVertices);
				gfxDrawElements(ctIndices, puwIndices);

				//////////////////////////////////////////////////////////////////////////
				//gfxDisableAlphaTest();
				gfxDisableBlend();
				hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU );		D3D_CHECKERROR(hr);
				hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);		D3D_CHECKERROR(hr);
			}
		}		
		//////////////////////////////////////////////////////////////////////////
		
		//hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		//hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,  128);
		
		//////////////////////////////////////////////////////////////////////////
		//_pGfx->gl_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		gfxDisableTexture();
		shaClearTextureMatrix();
		
		gfxDisableDepthBias();
		// 이전의 필터링 모드로 복구.
		gfxSetTextureFiltering( iOldFilter, iOldAnisotropy);
	}
	_sfStats.StopTimer(CStatForm::STI_SHADOWRENDERING);
//-----------------------------------------------------------------------------
}

// do the rendering
void CRenderer::Render(void)
{
	extern INDEX g_iReflectionDetail;
//안태훈 수정 시작	//(For Performance)(0.2)
	//this->PrepareTerrainLights();
//안태훈 수정 끝	//(For Performance)(0.2)

	// if the world doesn't have all portal-sector links updated
	if( !re_pwoWorld->wo_bPortalLinksUpToDate) 
	{
		// update the links
		CSetFPUPrecision FPUPrecision(FPT_53BIT);
		re_pwoWorld->wo_baBrushes.LinkPortalsAndSectors();
		re_pwoWorld->wo_bPortalLinksUpToDate = TRUE;
	}
	
	StartStatsMode(CStatForm::STI_WORLDTRANSFORM);
	
	// set FPU to single precision while rendering
	CSetFPUPrecision FPUPrecision(FPT_24BIT);
	
	// initialize all rendering structures
	Initialize();
	// init select-on-render functionality if not rendering shadows
	extern void InitSelectOnRender( PIX pixSizeI, PIX pixSizeJ);
	if( re_pdpDrawPort!=NULL) InitSelectOnRender( re_pdpDrawPort->GetWidth(), re_pdpDrawPort->GetHeight());
	// add initial sectors to active lists
	AddInitialSectors();
	// scan through portals for other sectors
	ScanForOtherSectors();
	
	// force finishing of all OpenGL pending operations, if required
	ChangeStatsMode(CStatForm::STI_SWAPBUFFERS);
	extern INDEX ogl_iFinish;  ogl_iFinish = Clamp( ogl_iFinish, 0L, 3L);
	extern INDEX d3d_iFinish;  d3d_iFinish = Clamp( d3d_iFinish, 0L, 3L);
	// NOTE : 쓸모없는 부분.
	/*
	if( (ogl_iFinish==1 && _pGfx->gl_eCurrentAPI==GAT_OGL) 
		|| (d3d_iFinish==1 && _pGfx->gl_eCurrentAPI==GAT_D3D)) 
	{
		extern INDEX ogl_bHardFinish;
		gfxFinish(ogl_bHardFinish);
	}
	*/
	
	// check any eventual delayed depth points outside the mirror (if API and time allows)
	if( !re_bRenderingShadows && re_iIndex==0) 
	{
		// OpenGL allows us to check z-buffer from previous frame - cool deal!
		// Direct3D is, of course, totally different story. :(
		if( _pGfx->gl_eCurrentAPI==GAT_OGL || d3d_bAlternateDepthReads) 
		{
			ChangeStatsMode(CStatForm::STI_FLARESRENDERING);
			extern void CheckDelayedDepthPoints( const CDrawPort *pdp, INDEX iMirrorLevel=0);
			CheckDelayedDepthPoints(re_pdpDrawPort);
		}
		// in 1st pass - mirrors are not drawn
		_bMirrorDrawn = FALSE;
	}
	INDEX iWaterCount = re_amiWaters.Count();
	
	//extern INDEX ska_bUseHardwareShaders;
	extern BOOL RM_AreHardwareShadersAvailable(void);
	const BOOL bHardwareShader = RM_AreHardwareShadersAvailable();
	
	// if may render one more mirror recursion
	ChangeStatsMode(CStatForm::STI_WORLDTRANSFORM);
	if(  !re_bRenderingShadows
		&&  re_prProjection.IsPerspective()
//안태훈 수정 시작	//(Add CRenderTexture class for Render to Texture)(0.1)
		//		&&  re_iIndex<MAX_RENDERERS-1	//원본
		&&  re_iIndex == IDX_MAIN_RENDERER	//miain renderer에서만 mirror쪽 렌더를 호출하는 루틴을 탈 수 있음.
//안태훈 수정 끝	//(Add CRenderTexture class for Render to Texture)(0.1)
		&&  re_amiMirrors.Count()>0
		&& !re_pdpDrawPort->IsOverlappedRendering())
	{
		re_prProjection->pr_bNiceWater	= FALSE;
		// cleanup after scanning
		CleanupScanning();
		
		// take next renderer
//안태훈 수정 시작	//(Add CRenderTexture class for Render to Texture)(0.1)
		//		CRenderer &re = _areRenderers[re_iIndex+1];	//원본
		CRenderer &re = _areRenderers[IDX_MIRROR_RENDERER];
//안태훈 수정 끝	//(Add CRenderTexture class for Render to Texture)(0.1)
		// for each mirror
		for( INDEX i=0; i<re_amiMirrors.Count(); i++)
		{
			// skip invalid mirrors
			CMirror &mi = re_amiMirrors[i];
			if( mi.mi_iMirrorType<0) continue;
			
			// calculate all needed data for the mirror
			mi.FinishAdding();
			// skip mirror that has no significant area
			if( mi.mi_fpixMaxPolygonArea<5) continue;
			
			// expand mirror in each direction, but keep it inside drawport
			PIX pixDPSizeI = re_pdpDrawPort->GetWidth();
			PIX pixDPSizeJ = re_pdpDrawPort->GetHeight();
			mi.mi_boxOnScreen.Expand(1);
			mi.mi_boxOnScreen &= PIXaabbox2D( PIX2D(0,0), PIX2D(pixDPSizeI,pixDPSizeJ));
			
			// get drawport and mirror coordinates
			PIX pixMirrorMinI = mi.mi_boxOnScreen.Min()(1);
			PIX pixMirrorMinJ = mi.mi_boxOnScreen.Min()(2);
			PIX pixMirrorMaxI = mi.mi_boxOnScreen.Max()(1);
			PIX pixMirrorMaxJ = mi.mi_boxOnScreen.Max()(2);
			
			// calculate mirror size
			PIX pixMirrorSizeI = pixMirrorMaxI-pixMirrorMinI;
			PIX pixMirrorSizeJ = pixMirrorMaxJ-pixMirrorMinJ;
			// clone drawport (must specify doubles here, to keep the precision)
			CDrawPort dpMirror( re_pdpDrawPort, pixMirrorMinI /(DOUBLE)pixDPSizeI, pixMirrorMinJ /(DOUBLE)pixDPSizeJ,
				pixMirrorSizeI/(DOUBLE)pixDPSizeI, pixMirrorSizeJ/(DOUBLE)pixDPSizeJ);
			dpMirror.SetAsCurrent();
			
			// recalculate mirror size to compensate for possible lost precision
			pixMirrorMinI  = dpMirror.dp_MinI - re_pdpDrawPort->dp_MinI;
			pixMirrorMinJ  = dpMirror.dp_MinJ - re_pdpDrawPort->dp_MinJ;
			pixMirrorMaxI  = dpMirror.dp_MaxI - re_pdpDrawPort->dp_MinI;
			pixMirrorMaxJ  = dpMirror.dp_MaxJ - re_pdpDrawPort->dp_MinJ;
			pixMirrorSizeI = pixMirrorMaxI-pixMirrorMinI;
			pixMirrorSizeJ = pixMirrorMaxJ-pixMirrorMinJ;
			ASSERT( pixMirrorSizeI==dpMirror.GetWidth() && pixMirrorSizeJ==dpMirror.GetHeight());
			
			// set it up for rendering
			re.re_pwoWorld     = re_pwoWorld;
			re.re_prProjection = re_prProjection;
			re.re_pdpDrawPort  = &dpMirror;
			// initialize clipping rectangle around the mirror size
			re.InitClippingRectangle( 0, 0, pixMirrorSizeI, pixMirrorSizeJ);
			// setup projection to use the mirror drawport and keep same perspective as before
			re.re_prProjection->ScreenBBoxL() = FLOATaabbox2D( FLOAT2D(0,0), FLOAT2D(pixDPSizeI, pixDPSizeJ));
			((CPerspectiveProjection3D&)(*re.re_prProjection)).ppr_boxSubScreen =
				FLOATaabbox2D( FLOAT2D(pixMirrorMinI, pixMirrorMinJ), FLOAT2D(pixMirrorMaxI, pixMirrorMaxJ));
			
			// warp?
			if( mi.mi_mp.mp_ulFlags&MPF_WARP) 
			{
				// warp clip plane is parallel to view plane and contains the closest point
				re.re_penViewer = mi.mi_mp.mp_penWarpViewer;
				re.re_pcspoViewPolygons = NULL;
				re.re_prProjection->WarpPlaneL() = FLOATplane3D(FLOAT3D(0,0,-1), mi.mi_vClosest);
				// create new viewer placement
				CPlacement3D pl = re.re_prProjection->ViewerPlacementR();
				FLOATmatrix3D m;
				MakeRotationMatrixFast(m, pl.pl_OrientationAngle);
				pl.AbsoluteToRelativeSmooth(mi.mi_mp.mp_plWarpIn);
				pl.RelativeToAbsoluteSmooth(mi.mi_mp.mp_plWarpOut);
				re.re_prProjection->ViewerPlacementL() = pl;
				if (re.re_prProjection.IsPerspective() && mi.mi_mp.mp_fWarpFOV>=1 && mi.mi_mp.mp_fWarpFOV<=170) 
				{
					((CPerspectiveProjection3D&)*re.re_prProjection).FOVL() = mi.mi_mp.mp_fWarpFOV;
				}
				// mirror!
			}
			else 
			{
				re.re_penViewer = NULL;
				re.re_pcspoViewPolygons = &mi.mi_cspoPolygons;
				re.re_prProjection->MirrorPlaneL() = mi.mi_plPlane;
				re.re_prProjection->MirrorPlaneL().Offset(0.05f); // move projection towards mirror a bit, to avoid cracks
			}
			re.re_bRenderingShadows = FALSE;
			re.re_ubLightIllumination = 0;
			
			// just flat-fill if mirrors are disabled
			extern INDEX wld_bRenderMirrors;
			if( !wld_bRenderMirrors) dpMirror.Fill(C_GRAY|CT_OPAQUE);
			else 
			{
				// render the view inside mirror
				StopStatsMode();
				re.Render();
				StartStatsMode(CStatForm::STI_WORLDTRANSFORM);
			} 
			// unlock mirror's and lock back the original drawport
			re_pdpDrawPort->SetAsCurrent();
			// clear entire buffer to back value
			re_pdpDrawPort->FillZBuffer(ZBUF_BACK);
			// fill depth buffer of the mirror, so that scene cannot be drawn through it
			FillMirrorDepth(mi);
			_bMirrorDrawn = TRUE;
		}
		
		// flush all mirrors
		re_amiMirrors.PopAll();
		
		// fill z-buffer only if no mirrors have been drawn, not rendering second layer in world editor and not in wireframe mode
		if(  !_bMirrorDrawn
			&& !re_pdpDrawPort->IsOverlappedRendering()
			&& _wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE) 
		{
			re_pdpDrawPort->FillZBuffer(ZBUF_BACK);
		}

		// NOTE : 월드 내에 Terrain 엔티티가 하나만 들어가게 될 경우에는
		// NOTE : Terrain 엔티티를 외부로 빼놓아도 좋을듯...
		if(re_lhActiveTerrains.IsEmpty())
		{
			// for all entities in world 
            FOREACHINDYNAMICCONTAINER(re_pwoWorld->wo_cenEntities, CEntity, iten) 
            { 
                if(iten->en_RenderType==CEntity::RT_TERRAIN) 
                { 
                    // add it as a terrain 
                    AddTerrainEntity( &iten.Current()); 
                } 
            }
		}
		// draw the prepared things to screen
		DrawToScreen();
	}
	// Water를 렌더링함.
	else if(
		re_iIndex == IDX_MAIN_RENDERER &&	
		iWaterCount > 0 &&
		!re_bRenderingShadows &&
		(g_iReflectionDetail != REFLECTION_NONE) &&
		bHardwareShader &&
		!re_pdpDrawPort->IsOverlappedRendering()
		)
	{
		CleanupScanning();
		// NOTE : 새로운 렌더러를 설정했기 때문에,
		// NOTE : 섹터 계산 및, 엔티티 목록을 얻어와야 합니다.
		// NOTE : 이 부분은, RenderReflection()에서 수행됩니다
		// take next renderer
		CRenderer &re = _areRenderers[IDX_WATER_RENDERER];
		
		//강동민 수정 시작 Water 구현		04.21
		PIX pixDPSizeI = re_pdpDrawPort->GetWidth();
		PIX pixDPSizeJ = re_pdpDrawPort->GetHeight();

		FLOAT3D vProjMin(0.0f, 0.0f, 0.0f);
		FLOAT3D vProjMax(0.0f, 0.0f, 0.0f);

		CDynamicStackArray<FLOAT3D> ClippedVertices;
		
		//-----------------------------------------------------------------------------------------------
		// DrawPort의 영역 계산하는 부분.
		//-----------------------------------------------------------------------------------------------
		int		i, j;

		for( i = 0; i < re_amiWaters.Count(); ++i )
		{
			CWaterEntity &water	= re_amiWaters[i];
			CEntity &en			= *water.m_pWaterEntity;
			CModelInstance* pMI = en.GetModelInstance();
			ASSERT(pMI != NULL && "Invalid Water Entity!!!");
			_plReflect			= water.m_plPlane;
			
			CPlacement3D plModel = en.GetLerpedPlacement();		// 모델의 위치.
			FLOATmatrix3D mat;			
			MakeRotationMatrixFast(mat, ANGLE3D(0.0f, 0.0f, 0.0f));
			Matrix12 matTemp;
			MatrixVectorToMatrix12(matTemp, mat, plModel.pl_PositionVector);
			
			FLOATaabbox3D aabbAllFrames;
			pMI->GetAllFramesBBox( aabbAllFrames);
			aabbAllFrames.StretchByVector(pMI->mi_vStretch);
			
			FLOAT3D vMin = aabbAllFrames.minvect;
			FLOAT3D vMax = aabbAllFrames.maxvect;

			FLOAT3D vertex[4];
			vertex[0] = vMin;vertex[1] = vMin;vertex[2] = vMin;
			vertex[1](1)	= vMax(1);			
			vertex[2](3)	= vMax(3);			
			vertex[3]		= vMax;

			for( i = 0; i < 4; ++i )
			{
				TransformVector((float*)&vertex[i], matTemp);
				re_prProjection->PreClip(vertex[i], vertex[i]);
			}

			for( i = 0; i < 4; ++i)
			{
				for( j = i; j < 4; ++j)
				{
					if(i == j) 
						continue;
					FLOAT3D vTemp1		= vertex[i];
					FLOAT3D vTemp2		= vertex[j];
					ULONG ulClipFlags	= re_prProjection->ClipLine(vTemp1, vTemp2);
					if (ulClipFlags != LCF_EDGEREMOVED) 
					{						
						re_prProjection->PostClip(vTemp1, vTemp1);
						re_prProjection->PostClip(vTemp2, vTemp2);
						FLOAT3D &vTemp3 = ClippedVertices.Push();
						FLOAT3D &vTemp4 = ClippedVertices.Push();
						vTemp3			= vTemp1;
						vTemp4			= vTemp2;
					}
				}
			}
		}

		if(ClippedVertices.Count() > 0)
		{
			vProjMin	= ClippedVertices[0];
			vProjMax	= ClippedVertices[0];
			for( i = 0; i < ClippedVertices.Count(); i++)
			{
				FLOAT3D vTemp = ClippedVertices[i];
				if(vProjMin(1) > vTemp(1))		vProjMin(1) = vTemp(1);
				if(vProjMin(2) > vTemp(2))		vProjMin(2) = vTemp(2);
				if(vProjMax(1) < vTemp(1))		vProjMax(1) = vTemp(1);
				if(vProjMax(2) < vTemp(2))		vProjMax(2) = vTemp(2);
				if(vProjMin(1) < 0)				vProjMin(1) = 0.0f;
				if(vProjMin(2) < 0)				vProjMin(2) = 0.0f;
				if(vProjMax(1) < 0)				vProjMax(1) = 0.0f;
				if(vProjMax(2) < 0)				vProjMax(2) = 0.0f;
			}
			ClippedVertices.PopAll();
		}
		
		// NOTE : 반사 영역을 제한하여, 최대한 속도 향상을 꾀해보자...
		PIX pixMirrorMinI = vProjMin(1);
		PIX pixMirrorMinJ = vProjMin(2);
		
		PIX pixMirrorMaxI = vProjMax(1);
		PIX pixMirrorMaxJ = vProjMax(2);

		if(pixMirrorMaxI > pixDPSizeI)
		{
			pixMirrorMaxI = pixDPSizeI;
		}

		if(pixMirrorMaxJ > pixDPSizeJ)
		{
			pixMirrorMaxJ = pixDPSizeJ;
		}

		if(pixMirrorMinI > pixMirrorMaxI)
		{
			pixMirrorMinI = 0;
		}

		if(pixMirrorMinJ > pixMirrorMaxJ)
		{
			pixMirrorMinJ = 0;
		}
		
		// calculate mirror size
		// 거울의 크기 계산.
		PIX pixMirrorSizeI = pixMirrorMaxI-pixMirrorMinI;		// Width
		PIX pixMirrorSizeJ = pixMirrorMaxJ-pixMirrorMinJ;		// Height
		
		// 인자로 넘겨진 값을 바탕으로 Sub DrawPort를 만듭니다.
		CDrawPort dpMirror( re_pdpDrawPort, 
			pixMirrorMinI /(DOUBLE)pixDPSizeI,		// 거울의 시작좌표 계산(화면상의 비율인듯...)
			pixMirrorMinJ /(DOUBLE)pixDPSizeJ,		// 거울의 시작좌표 계산(화면상의 비율로...)
			pixMirrorSizeI/(DOUBLE)pixDPSizeI,		// 거울의 크기 계산(화면상의 비율로...)
			pixMirrorSizeJ/(DOUBLE)pixDPSizeJ);		// 거울의 크기 계산(화면상의 비율로...)
		
		//re_pdpDrawPort->SetAsCurrent();				// 원본
		dpMirror.SetAsCurrent();
		
		// set it up for rendering
		re.re_pwoWorld     = re_pwoWorld;
		re.re_prProjection = re_prProjection;
		//re.re_pdpDrawPort  = re_pdpDrawPort;			// 원본.
		re.re_pdpDrawPort  = &dpMirror;
		//re.InitClippingRectangle( 0, 0, re_pdpDrawPort->GetWidth(), re_pdpDrawPort->GetHeight());		// 원본
		re.InitClippingRectangle( 0, 0, dpMirror.GetWidth(), dpMirror.GetHeight());
		
		// setup projection to use the mirror drawport and keep same perspective as before
		// 이전과 같은 원근을 유지하고, 거울의 Drawport를 사용하도록 프로젝션 설정함.
		// NOTE : 중요함...
		re.re_prProjection->ScreenBBoxL() = FLOATaabbox2D( 
			FLOAT2D(0,0),						// 시작 좌표
			FLOAT2D(pixDPSizeI, pixDPSizeJ));	// 크기
		
		// SubDrawPort의 크기 지정.
		// NOTE : 중요함.  거울의 영역을 설정하는 부분으로 매우 중요함.
		// NOTE : 원근 투영 상수의 값을 설정함.
		// NOTE : 클리핑 영역의 값이 제대로 넘어오기는 하지만, Reflection 텍스쳐를 생성할때 View의 시점이 이상하게 적용됨.
		((CPerspectiveProjection3D&)(*re.re_prProjection)).ppr_boxSubScreen =
			FLOATaabbox2D( FLOAT2D(pixMirrorMinI, pixMirrorMinJ),				// 거울 영역
			FLOAT2D(pixMirrorMaxI, pixMirrorMaxJ));
		re.re_penViewer				= NULL;
		re.re_bRenderingShadows		= FALSE;
		re.re_ubLightIllumination	= 0;
		
		if(re_bInitReflectionTexture == FALSE)
		{
			if(re_prtReflection)
			{
				delete re_prtReflection;
				re_prtReflection = NULL;
			}
			re_prtReflection = new CRenderTexture();
			
			//re_prtReflection->Init(re_pdpDrawPort->GetWidth(), re_pdpDrawPort->GetHeight());
			re_prtReflection->Init(256, 256);
			re_bInitReflectionTexture = TRUE;
		}
		
		const BOOL bShowReflectionTexture = TRUE;
		if(bShowReflectionTexture)
		{
			if(re_iIndex == IDX_MAIN_RENDERER)
			{
				if(re_prtReflection && !bRenderReflection)
				{
					re_prtReflection->Begin();
					re_prtReflection->Clear(0x00FFFFFF);
					bRenderReflection = TRUE;
					re_prProjection->pr_bNiceWater = TRUE;
				}
			}
		}
		
		// render the view inside mirror
		//StopStatsMode();
		//re.Render();

		//////////////////////////////////////////////////////////////////////////
		// 반사 맵을 생성합니다.
		//////////////////////////////////////////////////////////////////////////		
		// NOTE : 물에 필요없는 부분을 제거하기 위해서 따로 렌더링 루틴을 타도록 함.
		if(g_iReflectionDetail != REFLECTION_NONE)
		{
			re.RenderReflection(/*water.m_matWaterProj*/);
		}
		//StartStatsMode(CStatForm::STI_WORLDTRANSFORM);
		
		if(bShowReflectionTexture)
		{
			if(re_iIndex == IDX_MAIN_RENDERER)
			{
				if(re_prtReflection && bRenderReflection)
				{
					re_prtReflection->End();
					bRenderReflection = FALSE;
				}				
				re_prProjection->pr_bNiceWater = FALSE;
			}
		}
		re_prProjection->pr_bNiceWater	= FALSE;
		
		re_pdpDrawPort->SetAsCurrent();
		re_pdpDrawPort->FillZBuffer(ZBUF_BACK);
		
		re_amiWaters.PopAll();		
		
		if(re_lhActiveTerrains.IsEmpty())
		{
			// for all entities in world 
			FOREACHINDYNAMICCONTAINER(re_pwoWorld->wo_cenEntities, CEntity, iten) 
			{ 
				if(iten->en_RenderType==CEntity::RT_TERRAIN)
				{ 
					// add it as a terrain 
					AddTerrainEntity( &iten.Current()); 
				}
			}
		}
		DrawToScreen();
	}

	//////////////////////////////////////////////////////////////////////////
	// no mirrors
	else
	{
		re_prProjection->pr_bNiceWater	= FALSE;
		if(re_iIndex == IDX_WATER_RENDERER)
		{
			re_prProjection->pr_bNiceWater	= TRUE;
		}
		// if rendering a mirror
		// or not rendering second layer in world editor
		// and not in wireframe mode
		if(  re_iIndex>0 
			|| !re_bRenderingShadows
			&& !re_pdpDrawPort->IsOverlappedRendering()
			&& _wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE) 
		{
			if(!re_prProjection->pr_bNiceWater)			
				re_pdpDrawPort->FillZBuffer(ZBUF_BACK);
		}		

		// draw the prepared things to screen and finish
		DrawToScreen();
		
		CleanupScanning();
		re_prProjection->pr_bNiceWater	= FALSE;
	}
	
	// disable fog/haze
	StopFog();
	StopHaze();
	// reset vertex arrays if this is the last renderer  
	if( re_iIndex==0) _avtxScene.PopAll();
	
	// for D3D (or mirror) we have to check depth points now, because we need back (not depth!) buffer for it,
	// and D3D can't guarantee that it won't be discarded upon swapbuffers (especially if multisampling is on!) :(
//강동민 수정 시작 Water 구현		04.16
	//if( !re_bRenderingShadows && ((_pGfx->gl_eCurrentAPI==GAT_D3D && !d3d_bAlternateDepthReads) || re_iIndex>0))	// 원본
	if( re_iIndex != IDX_WATER_RENDERER && !re_bRenderingShadows && ((_pGfx->gl_eCurrentAPI==GAT_D3D && !d3d_bAlternateDepthReads) || re_iIndex>0)) 
//강동민 수정 끝 Water 구현			04.16
	{
		extern void CheckDelayedDepthPoints( const CDrawPort *pdp, INDEX iMirrorLevel=0);
		CheckDelayedDepthPoints( re_pdpDrawPort, re_iIndex);
	}
	
	CleanupScanning();
	
	// end select-on-render functionality
	extern void EndSelectOnRender(void);
	EndSelectOnRender();
	
	// assure that FPU precision was low all the rendering time
	ASSERT( GetFPUPrecision()==FPT_24BIT);
	StopStatsMode();
}

//-----------------------------------------------------------------------------
// Purpose: 반사맵을 얻기위한 부분.
//-----------------------------------------------------------------------------
void CRenderer::RenderReflection(/*Matrix16 &matWaterProj*/)
{	
	extern INDEX g_iReflectionDetail;
	extern INDEX gfx_bRenderReflection;
	gfx_bRenderReflection = TRUE;

	re_prProjection->pr_bNiceWater	= FALSE;
	if(re_iIndex == IDX_WATER_RENDERER)
	{
		re_prProjection->pr_bNiceWater	= TRUE;
	}
	
	// set FPU to single precision while rendering
	CSetFPUPrecision FPUPrecision(FPT_24BIT);
	
	// initialize all rendering structures
	// NOTE : (내부적으로 Mirror 평면이 설정되어 있을 경우, Viewer의 위치를 반사시킵니다.)	
	// NOTE : (내부에서 호출되는 Prepare()에서 반사됨...)
	Initialize();

	// 엔티티들을 각각의 Active List에 추가합니다.	
	// add initial sectors to active lists
	AddInitialSectors();

	// scan through portals for other sectors
	// NOTE : (내부적으로는 거울을 추가하고, 거울 내의 폴리곤 추가등의 역할을 합니다.)
	// NOTE : 반드시 있어야 되는 부분...
	ScanForOtherSectors();

	// if rendering a mirror
	// or not rendering second layer in world editor
	// and not in wireframe mode
	if(  re_iIndex>0 
		|| !re_bRenderingShadows
		&& !re_pdpDrawPort->IsOverlappedRendering()
		&& _wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE) 
	{
			re_pdpDrawPort->FillZBuffer(ZBUF_BACK);
	}
//강동민 수정 시작 Water 구현		04.21
	// DrawPort의 프로젝션 행렬값을 저장함.
	if(re_prProjection->pr_bNiceWater)
	{
		CAnyProjection3D *papr;
		re_prProjection->ObjectPlacementL() = CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0));
		re_prProjection->Prepare();
		papr = &re_prProjection;
		re_pdpDrawPort->SetProjection(*papr);
		_pGfx->gl_pd3dDevice->GetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&_matWaterProj);
	}
//강동민 수정 끝 Water 구현			04.21
	//-----------------------------------------------------------------------------
	{
		ChangeStatsMode(CStatForm::STI_WORLDRENDERING);	
		
		//const BOOL bModelSetupTimer = _sfStats.CheckTimer(CStatForm::STI_MODELSETUP);
		//if( bModelSetupTimer) _sfStats.StopTimer(CStatForm::STI_MODELSETUP);
		_sfStats.StartTimer(CStatForm::STI_WATERRENDERING);
	
		//------------------------------------------------- first render background
		// if polygons should be drawn
		if (!re_bRenderingShadows &&
			_wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE) 
		{
			// 배경(Sky Box) -> Not Ska 모델 렌더링.
			if( re_bBackgroundEnabled) 
			{
				// render the polygons to screen
				CPerspectiveProjection3D *pprPerspective =
					(CPerspectiveProjection3D *)(CProjection3D *)(re_prBackgroundProjection);
				pprPerspective->Prepare();
				RenderScene( re_pdpDrawPort, re_pspoFirstBackground, re_prBackgroundProjection, re_colSelection, FALSE);
			}
		}		
		
		if (re_bBackgroundEnabled)
		{
			// Sky Box(SKA 모델) 렌더링.
			// render models that were kept for delayed rendering.
			//ChangeStatsMode(CStatForm::STI_MODELSETUP);
			RenderModels(TRUE);   // render background models
			//ChangeStatsMode(CStatForm::STI_WORLDRENDERING);
		}
		
//강동민 수정 시작 접속 시퀀스 작업	05.31
		// if polygons should be drawn
		if (!re_bRenderingShadows &&
			re_bBackgroundEnabled
			&&_wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE) 
		{
			// render translucent portals
			CPerspectiveProjection3D *pprPerspective = (CPerspectiveProjection3D*)(CProjection3D*)(re_prBackgroundProjection);
			RenderScene( re_pdpDrawPort, SortTranslucentPolygons(re_pspoFirstBackgroundTranslucent),
				re_prBackgroundProjection, re_colSelection, TRUE);
		}
		
		/*
		if( re_bBackgroundEnabled) 
		{
			ChangeStatsMode(CStatForm::STI_PARTICLERENDERING);
			RenderParticles(TRUE); // render background particless
			ChangeStatsMode(CStatForm::STI_WORLDRENDERING);
		}
		*/
//강동민 수정 끝 접속 시퀀스 작업	05.31
		
		//------------------------------------------------- second render non-background
		// if polygons should be drawn
		if( !re_bRenderingShadows
			&& _wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE
			&& g_bShowBSP
			&& (g_iReflectionDetail == REFLECTION_ALL))
		{
			// render the spans to screen
			re_prProjection->Prepare();
			CPerspectiveProjection3D *pprPerspective = (CPerspectiveProjection3D*)(CProjection3D*)re_prProjection;
			RenderScene( re_pdpDrawPort, re_pspoFirst, re_prProjection, re_colSelection, FALSE);
		}
		
		// FIXME : SKY만 렌더링할 것인가? Terrain도 렌더링 할것인가?
		// Render active terrains
		if( !re_bRenderingShadows
			&& _wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE
			&& g_bShowTerrain)
		{
			RenderTerrains();
		}
		
		// render models that were kept for delayed rendering
		//ChangeStatsMode(CStatForm::STI_MODELSETUP);
		if(g_bShowModel && (g_iReflectionDetail == REFLECTION_ALL))
			RenderModels(FALSE); // render non-background models
		//ChangeStatsMode(CStatForm::STI_PARTICLERENDERING);
		//		RenderParticles(FALSE); // render non-background particles
		//ChangeStatsMode(CStatForm::STI_WORLDRENDERING);
		
		// if polygons should be drawn
		if (!re_bRenderingShadows
			&&_wrpWorldRenderPrefs.wrp_ftPolygons != CWorldRenderPrefs::FT_NONE
			&& g_bShowBSP
			&& (g_iReflectionDetail == REFLECTION_ALL))
		{
			// render translucent portals
			CPerspectiveProjection3D *pprPerspective = (CPerspectiveProjection3D*)(CProjection3D*)re_prProjection;
			pprPerspective->Prepare();
			RenderScene( re_pdpDrawPort, SortTranslucentPolygons(re_pspoFirstTranslucent),
				re_prProjection, re_colSelection, TRUE);
		}

		//_sfStats.StopTimer(CStatForm::STI_WATERRENDERING);
		
		// clean all buffers after rendering
		//re_aspoScreenPolygons.PopAll();
		//re_admDelayedModels.PopAll();
		//re_cenDrawn.PopAll();
		//re_avvxViewVertices.PopAll();
	}
	//-----------------------------------------------------------------------------
	// draw the prepared things to screen and finish
	//DrawToScreen();	
	//CleanupScanning();
	re_prProjection->pr_bNiceWater	= FALSE;
	
//강동민 수정 시작 그림자 옵션 작업	08.16
	// FIXME : 물이 들어가면 FOG와 HAZE가 적용되지 않음...
	// disable fog/haze
	//StopFog();
	//StopHaze();
//강동민 수정 끝 그림자 옵션 작업		08.16
	// reset vertex arrays if this is the last renderer  
//강동민 수정 시작 Water 구현		04.22
	//if( re_iIndex==0) _avtxScene.PopAll();	
//강동민 수정 끝 Water 구현			04.22

	re_aspoScreenPolygons.Clear();
	re_admDelayedModels.Clear();
	re_cenDrawn.Clear();
	re_alfiLensFlares.Clear();
	re_amiMirrors.Clear();
	re_amiWaters.Clear();
	re_avvxViewVertices.Clear();
	re_aiEdgeVxMain.Clear();
	re_admDelayedModels_AddWater.Clear();

	CleanupScanning();
	
	// assure that FPU precision was low all the rendering time
	//ASSERT( GetFPUPrecision()==FPT_24BIT);
	//StopStatsMode();

//강동민 수정 시작 테스트 클라이언트 작업	06.29
	extern INDEX gfx_bRenderReflection;
	gfx_bRenderReflection = FALSE;
//강동민 수정 끝 테스트 클라이언트 작업		06.29
}
//강동민 수정 끝 Water 구현			04.20


/*
* Constructor.
*/
CRenderer::CRenderer(void)
{
	// setup self index
	INDEX i = this - _areRenderers;
	ASSERT(i>=0 && i<MAX_RENDERERS);
	re_iIndex = i;

	// sehan
	re_bRenderBloom = FALSE;

	for (i = 0; i < 2; ++i )
	{
		_prtFilterTarget[i] = NULL;
	}

	m_pGaussianProp		= NULL;
	m_fTemp = -0.1f;
	m_bElimSmallContributors	= false;
	//_pVertexBuffer			= NULL;
	for (int height = 0; height < MAX_BACKVERTEX_HEIGHT; height++) {
		for (int width = 0; width < MAX_BACKVERTEX_WIDTH; width++) {
			_pBackVertexBuffer[width][height] = NULL;
		}
	}
//강동민 수정 시작 버그 사냥 작업	09.09
    //m_dwTexCoord4OffsetVertexShader = 0L;
    //m_dwAddFourPixelShader = 0L;
//강동민 수정 끝 버그 사냥 작업		09.09
	// sehan end
}
/*
* Destructor.
*/
CRenderer::~CRenderer(void)
{
//강동민 수정 시작 버그 사냥 작업	09.09
	// StopDisplayMode()에서 해제해주기 때문에 주석처리함.
	/*
	// sehan
	for( int i = 0; i < 2; i++ )
    {
		if (_prtFilterTarget[i]) {
			delete _prtFilterTarget[i];
			_prtFilterTarget[i] = NULL;
		}
	}

	SAFE_RELEASE( _pFilterVertexBuffer );			// sets pointers to null after delete
	for (int height = 0; height < MAX_BACKVERTEX_HEIGHT; height++) {
		for (int width = 0; width < MAX_BACKVERTEX_WIDTH; width++) {
			SAFE_RELEASE( _pBackVertexBuffer[width][height] );			// sets pointers to null after delete
		}
	}	
	*/
//강동민 수정 끝 버그 사냥 작업		09.09
	extern void ClearFog();
	extern void ClearHaze();
	ClearFog();
	ClearHaze();

	SAFE_ARRAY_DELETE( m_pGaussianProp );
	m_vGaussian1D[0].clear();
	m_vGaussian1D[1].clear();
	// sehan end
}

// initialize clipping rectangle
void CRenderer::InitClippingRectangle(PIX pixMinI, PIX pixMinJ, PIX pixSizeI, PIX pixSizeJ)
{
	re_pspoFirst = NULL;
	re_pspoFirstTranslucent = NULL;
	re_pspoFirstBackground = NULL;
	re_pspoFirstBackgroundTranslucent = NULL;
	
	re_fMinJ = (FLOAT) pixMinJ;
	re_fMaxJ = (FLOAT) pixSizeJ+pixMinJ;
	re_pixSizeI = pixSizeI;
	re_fbbClipBox =
		FLOATaabbox2D( FLOAT2D((FLOAT) pixMinI+CLIPMARGADD,
		(FLOAT) pixMinJ+CLIPMARGADD),
		FLOAT2D((FLOAT) pixMinI+pixSizeI-CLIPMARGSUB,
		(FLOAT) pixMinJ+pixSizeJ-CLIPMARGSUB));
	
	re_pixTopScanLineJ = PIXCoord(pixMinJ+CLIPMARGADD);
	re_ctScanLines =
		PIXCoord(pixSizeJ-CLIPMARGSUB) - PIXCoord(CLIPMARGADD)/* +1*/;
	re_pixBottomScanLineJ = re_pixTopScanLineJ+re_ctScanLines;
}

// render a 3D view to a drawport
void RenderView(CWorld &woWorld, CEntity &enViewer,
				CAnyProjection3D &prProjection, CDrawPort &dpDrawport)
{
	extern INDEX gfx_bRenderView;
	if (!gfx_bRenderView) 
	{
		return;
	}
	
	TRACKMEM(mem,"RenderView");
	
	// let the worldbase execute its render function
	if (woWorld.wo_pecWorldBaseClass!=NULL
		&&woWorld.wo_pecWorldBaseClass->ec_pdecDLLClass!=NULL
		&&woWorld.wo_pecWorldBaseClass->ec_pdecDLLClass->dec_OnWorldRender!=NULL) 
	{
		woWorld.wo_pecWorldBaseClass->ec_pdecDLLClass->dec_OnWorldRender(&woWorld);
	}
	
	if(_wrpWorldRenderPrefs.GetShadowsType() == CWorldRenderPrefs::SHT_FULL)
	{
		// calculate all non directional shadows that are not up to date
		woWorld.CalculateNonDirectionalShadows();
	}
	
	// take first renderer object
	CRenderer &re = _areRenderers[0];
	// set it up for rendering
	re.re_penViewer = &enViewer;
	re.re_pcspoViewPolygons = NULL;
	re.re_pwoWorld = &woWorld;
	re.re_prProjection = prProjection;
	re.re_pdpDrawPort = &dpDrawport;
	// initialize clipping rectangle around the drawport
	re.InitClippingRectangle(0, 0, dpDrawport.GetWidth(), dpDrawport.GetHeight());
	prProjection->ScreenBBoxL() = FLOATaabbox2D(
		FLOAT2D(0.0f, 0.0f),
		FLOAT2D((float)dpDrawport.GetWidth(), (float)dpDrawport.GetHeight())
		);
	re.re_bRenderingShadows = FALSE;
	re.re_ubLightIllumination = 0;

	// sehan
//안태훈 수정 시작	//(Option)(0.1)
	if( re.re_iIndex == IDX_MAIN_RENDERER)
	{
		if (g_iUseBloom > 0) 
		{
//안태훈 수정 끝	//(Option)(0.1)
			re.re_bRenderBloom = TRUE;
			if( (_pGfx->gl_ulFlags&GLF_VERTEXPROGRAM) && 
				(_pGfx->gl_ulFlags&GLF_PIXELPROGRAM) && 
				(_pGfx->gl_ctTextureUnits>=4) )
			{
			}
			else
			{
				g_iUseBloom = 0;
			}
		}
	// sehan end
	}
	
//안태훈 수정 시작	//(Option)(0.1)1
	extern FLOAT ter_fLODMul;
	extern FLOAT g_fFarClipPlus;
	re.re_prProjection->FarClipDistanceL() = 450 * ter_fLODMul + g_fFarClipPlus;
	re.re_prProjection->NearClipDistanceL() = 0.7f;
//안태훈 수정 끝	//(Option)(0.1)

	// render the view
	re.Render();

	// sehan
	re.re_bRenderBloom = FALSE;
	// sehan end
}


// Render a world with some viewer, projection and drawport. (viewer may be NULL)
// internal version used for rendering shadows
ULONG RenderShadows(CWorld &woWorld, CEntity &enViewer,
					CAnyProjection3D &prProjection, const FLOATaabbox3D &boxViewer,
					UBYTE *pubShadowMask, SLONG slShadowWidth, SLONG slShadowHeight,
					UBYTE ubIllumination)
{
	_pfWorldEditingProfile.StartTimer(CWorldEditingProfile::PTI_RENDERSHADOWS);
	
	// take a renderer object
	CRenderer &re = _areRenderers[0];
	// set it up for rendering
	re.re_penViewer = &enViewer;
	re.re_pcspoViewPolygons = NULL;
	re.re_pwoWorld = &woWorld;
	re.re_prProjection = prProjection;
	re.re_pdpDrawPort = NULL;
	re.re_boxViewer = boxViewer;
	// initialize clipping rectangle around the drawport
	const FLOATaabbox2D &box = prProjection->ScreenBBoxR();
	//re.InitClippingRectangle(box.Min()(1), box.Min()(2), box.Size()(1), box.Size()(2));
	re.InitClippingRectangle(0, 0, box.Size()(1), box.Size()(2));
	
	re.re_bRenderingShadows = TRUE;
	re.re_bDirectionalShadows = prProjection.IsParallel();
	re.re_bSomeLightExists = FALSE;
	re.re_bSomeDarkExists = FALSE;
	_bSomeDarkExists = FALSE;
	re.re_pubShadow = pubShadowMask;
	re.re_slShadowWidth  = slShadowWidth;
	re.re_slShadowHeight = slShadowHeight;
	re.re_ubLightIllumination = ubIllumination;
	// render the view
	re.Render();
	
	ULONG ulFlags = 0;
	if (!re.re_bSomeLightExists) 
	{
		ulFlags|=BSLF_ALLDARK;
	}
	if (!(re.re_bSomeDarkExists|_bSomeDarkExists)) 
	{
		ulFlags|=BSLF_ALLLIGHT;
	}
	
	_pfWorldEditingProfile.StopTimer(CWorldEditingProfile::PTI_RENDERSHADOWS);
	
	return ulFlags;
}

