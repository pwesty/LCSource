// SeriousSkaStudioView.cpp : implementation of the CSeriousSkaStudioView class
//

#include "stdafx.h"
#include "SeriousSkaStudio.h"

#include "SeriousSkaStudioDoc.h"
#include "SeriousSkaStudioView.h"

#include <Engine/Math/Matrix12.h>
#include <Engine/Ska/Render.h>
#include <Engine/Templates/Stock_CMesh.h>
#include <Engine/Templates/Stock_CSkeleton.h>
#include <Engine/Templates/Stock_CAnimSet.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/base/Shell.h>
#include <vector>

//강동민 수정 시작
#include <Engine/Graphics/Texture.h>
//강동민 수정 끝

#include <Engine/Effect/CSkaEffect.h>
#include <Engine/Effect/CWorldTag.h>
#include <Engine/Effect/CTagManager.h>
#include <Engine/Effect/CEffectManager.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CTag.h>
#include <Engine/Effect/CMdlEffect.h>
#include <Engine/Effect/CCameraEffect.h>
#include <Engine/Effect/CParticleEffect.h>
#include <Engine/Effect/EffectCommon.h>

#include <Engine/Effect/CParticleGroup.h>
#include <Engine/Effect/CParticleGroupManager.h>
#include <Engine/Effect/CTraceEffect.h>
#include <Engine/Effect/CGroupTag.h>
#include <Engine/Effect/CEffectGroup.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Effect/CTraceEffect.h>
#include <Engine/Effect/CSkaEffect.h>
#include <Engine/Effect/CEntityEffect.h>
#include <Engine/Effect/CMdlEffect.h>
#include <Engine/Effect/CSoundEffect.h>
#include <Engine/Effect/CParticleEffect.h>
#include <Engine/Effect/CParticleGroup.h>
#include <Engine/Effect/CParticleGroupManager.h>
#include <Engine/Effect/CParticlesAbsorption.h>
#include <Engine/Effect/CParticlesEmitterSphere.h>
#include <Engine/Effect/CParticlesProcessDynamicState.h>
#include <Engine/Effect/CParticlesProcessForce.h>

#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Effect/EffectCommon.h>

#include "MainFrm.h"
#define EVENT_FADE_TIME 25.0f
#define QUICK_EVENT_FADE_TIME 2.5f

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL _bSelectedItemChanged = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CSeriousSkaStudioView

IMPLEMENT_DYNCREATE(CSeriousSkaStudioView, CView)

BEGIN_MESSAGE_MAP(CSeriousSkaStudioView, CView)
	//{{AFX_MSG_MAP(CSeriousSkaStudioView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_RESET_VIEW, OnResetView)
	ON_COMMAND(ID_SHOW_WIREFRAME, OnShowWireframe)
	ON_COMMAND(ID_SHOW_SKELETON, OnShowSkeleton)
	ON_COMMAND(ID_SHOW_TEXTURE, OnShowTexture)
	ON_COMMAND(ID_ADD_MESHLOD, OnAddMeshlod)
	ON_COMMAND(ID_ADD_ANIMATION, OnAddAnimation)
	ON_COMMAND(ID_ADD_SKELETONLOD, OnAddSkeletonlod)
	ON_COMMAND(ID_DELETESELECTED, OnDeleteselected)
	ON_COMMAND(ID_ADD_ANIMSET, OnAddAnimset)
	ON_COMMAND(ID_ADD_MESHLIST, OnAddMeshlist)
	ON_COMMAND(ID_ADD_SKELETONLIST, OnAddSkeletonlist)
	ON_COMMAND(ID_ADD_TEXTURE, OnAddTexture)
	ON_COMMAND(ID_ADD_CHILD_MODEL_INSTANCE, OnAddChildModelInstance)
	ON_COMMAND(ID_ADD_COLISIONBOX, OnAddColisionbox)
	ON_COMMAND(ID_ANIM_STOP, OnAnimStop)
	ON_COMMAND(ID_ANIM_SYNC, OnAnimSync)
	ON_COMMAND(ID_AUTO_MIPING, OnAutoMiping)
	ON_UPDATE_COMMAND_UI(ID_AUTO_MIPING, OnUpdateAutoMiping)
	ON_COMMAND(ID_SHOW_GROUND, OnShowGround)
	ON_UPDATE_COMMAND_UI(ID_SHOW_GROUND, OnUpdateShowGround)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SKELETON, OnUpdateShowSkeleton)
	ON_UPDATE_COMMAND_UI(ID_SHOW_TEXTURE, OnUpdateShowTexture)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WIREFRAME, OnUpdateShowWireframe)
	ON_COMMAND(ID_SHOW_ANIM_QUEUE, OnShowAnimQueue)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ANIM_QUEUE, OnUpdateShowAnimQueue)
	ON_COMMAND(ID_FILE_SAVEMI, OnFileSaveModel)
	ON_COMMAND(ID_SHOW_NORMALS, OnShowNormals)
	ON_UPDATE_COMMAND_UI(ID_SHOW_NORMALS, OnUpdateShowNormals)
	ON_COMMAND(ID_SHOW_LIGHTS, OnShowLights)
	ON_UPDATE_COMMAND_UI(ID_SHOW_LIGHTS, OnUpdateShowLights)
	ON_COMMAND(ID_CHANGE_AMBIENTCOLOR, OnChangeAmbientcolor)
	ON_COMMAND(ID_CHANGE_LIGHTCOLOR, OnChangeLightcolor)
	ON_COMMAND(ID_ANIM_LOOP, OnAnimLoop)
	ON_UPDATE_COMMAND_UI(ID_ANIM_LOOP, OnUpdateAnimLoop)
	ON_COMMAND(ID_ANIM_PAUSE, OnAnimPause)
	ON_UPDATE_COMMAND_UI(ID_ANIM_PAUSE, OnUpdateAnimPause)
	ON_COMMAND(ID_SHOW_COLISION, OnShowColision)
	ON_UPDATE_COMMAND_UI(ID_SHOW_COLISION, OnUpdateShowColision)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_COMMAND(ID_FILE_SAVEMI_AS, OnFileSavemiAs)
	ON_COMMAND(ID_FILE_RECREATETEXTURE, OnFileRecreatetexture)
	ON_UPDATE_COMMAND_UI(ID_FILE_RECREATETEXTURE, OnUpdateFileRecreatetexture)
	ON_COMMAND(ID_VK_DOWN, OnVkDown)
	ON_COMMAND(ID_VK_UP, OnVkUp)
	ON_COMMAND(ID_VK_ESCAPE, OnVkEscape)
	ON_COMMAND(ID_CREATE_ADD_TEXTURE, OnCreateAddTexture)
	ON_COMMAND(ID_ADD_TEXTURE_BUMP, OnAddTextureBump)
	ON_COMMAND(ID_ADD_TEXTURE_REFLECTION, OnAddTextureReflection)
	ON_COMMAND(ID_ADD_TEXTURE_SPECULAR, OnAddTextureSpecular)
	ON_COMMAND(ID_SHOW_ACTIVE_SKELETON, OnShowActiveSkeleton)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ACTIVE_SKELETON, OnUpdateShowActiveSkeleton)
	ON_COMMAND(ID_SHOW_ALL_FRAMES_BBOX, OnShowAllFramesBbox)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ALL_FRAMES_BBOX, OnUpdateShowAllFramesBbox)
	ON_COMMAND(ID_MODELINSTANCE_SAVEWITHOFFSET, OnModelinstanceSavewithoffset)
	ON_COMMAND(ID_VK_LEFT, OnVkLeft)
	ON_COMMAND(ID_VK_RIGHT, OnVkRight)
	ON_COMMAND(ID_VK_LEFT_WITH_CTRL, OnVkLeftWithCtrl)
	ON_COMMAND(ID_VK_RIGHT_WITH_CTRL, OnVkRightWithCtrl)
	ON_COMMAND(ID_CONVERT_SELECTED, OnConvertSelected)
	ON_COMMAND(ID_RESET_COLISIONBOX, OnResetColisionbox)
	ON_COMMAND(ID_ALL_FRAMES_RECALC, OnAllFramesRecalc)
	ON_COMMAND(ID_RELOAD_TEXTURE, OnReloadTexture)
	ON_COMMAND(ID_RECREATE_TEXTURE, OnRecreateTexture)
	ON_COMMAND(ID_BROWSE_TEXTURE, OnBrowseTexture)
	ON_COMMAND(ID_MEASURE_TOOL, OnMeasureTool)
	ON_UPDATE_COMMAND_UI(ID_MEASURE_TOOL, OnUpdateMeasureTool)
	ON_COMMAND(ID_ROTATE_VIEWER, OnRotateViewer)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_VIEWER, OnUpdateRotateViewer)
	ON_COMMAND(ID_ROTATE_LIGHT, OnRotateLight)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_LIGHT, OnUpdateRotateLight)
	ON_COMMAND(ID_INCREMENT_FADE_TIME, OnIncrementFadeTime)
	ON_COMMAND(ID_DECREMENT_FADE_TIME, OnDecrementFadeTime)
	ON_COMMAND(ID_FREEZE_ON_BLEND, OnFreezeOnBlend)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_COUNT_WEIGHT_PER_VERTEX, OnCountWeightPerVertex)
	ON_COMMAND(ID_INCREMENT_BPV, OnIncrementBpv)
	ON_COMMAND(ID_DECREMENT_BPV, OnDecrementBpv)
	ON_COMMAND(ID_USE_SOFTWARE_VP, OnUseSoftwareVp)
	ON_COMMAND(ID_USE_HARDWARE_VP, OnUseHardwareVp)
	ON_COMMAND(ID_VK_CTRL0, OnVkCtrl0)
	ON_COMMAND(ID_VK_CTRL1, OnVkCtrl1)
	ON_COMMAND(ID_VK_CTRL2, OnVkCtrl2)
	ON_COMMAND(ID_VK_CTRL3, OnVkCtrl3)
	ON_COMMAND(ID_VK_CTRL4, OnVkCtrl4)
	ON_COMMAND(ID_VK_CTRL5, OnVkCtrl5)
	ON_COMMAND(ID_VK_CTRL6, OnVkCtrl6)
	ON_COMMAND(ID_VK_CTRL7, OnVkCtrl7)
	ON_COMMAND(ID_VK_CTRL8, OnVkCtrl8)
	ON_COMMAND(ID_VK_CTRL9, OnVkCtrl9)
	ON_COMMAND(ID_VK_CTRL_DELETE, OnVkCtrlDelete)
	ON_COMMAND(ID_VK_CTRL_INSERT, OnVkCtrlInsert)
	ON_COMMAND(ID_SHOW_SUBSURFACES, OnShowSubsurfaces)
	ON_COMMAND(ID_VK_Z, OnVkZ)
	ON_COMMAND(ID_SHOW_AXIS, OnShowAxis)
	ON_COMMAND(ID_SHOWALL_EVENTS, OnShowallEvents)
	ON_COMMAND(ID_PROFILING, OnProfiling)
	ON_COMMAND(ID_SHOW_FPS, OnShowFps)
	ON_COMMAND(ID_SHOWPROJECTIONSHADOW, OnShowprojectionshadow)
	ON_UPDATE_COMMAND_UI(ID_SHOWPROJECTIONSHADOW, OnUpdateShowprojectionshadow)
	ON_COMMAND(ID_REFRESH_TAGMANAGER, OnRefreshTagmanager)
	ON_COMMAND(ID_SKATAG_NEW, OnSkatagNew)
	ON_COMMAND(ID_SKATAG_DELETE, OnSkatagDelete)
	ON_COMMAND(ID_SHOW_EFFECT, OnShowEffect)
	ON_COMMAND(ID_ANIM_FORWARD_ONE_TICK, OnAnimForwardOneTick)
	ON_UPDATE_COMMAND_UI(ID_SHOW_EFFECT, OnUpdateShowEffect)
	ON_COMMAND(ID_ANIMATION_EFFECT_ADD, OnAnimationEffectAdd)
	ON_COMMAND(ID_ANIMEFFECT_REG_ADDNEW, OnAnimeffectRegAddnew)
	ON_COMMAND(ID_ANIM_STRENGTH_DOWN, OnAnimStrengthDown)
	ON_COMMAND(ID_ANIM_STRENGTH_UP, OnAnimStrengthUp)
	ON_COMMAND(ID_MI_AIRCOLOR, OnMiAircolor)
	ON_COMMAND(ID_MI_GROUNDCOLOR, OnMiGroundcolor)
	ON_COMMAND(ID_VK_TAB, OnVkTab)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_RIDESETTING, OnRidesetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeriousSkaStudioView construction/destruction

static UINT cntRef = 0;

CSeriousSkaStudioView::CSeriousSkaStudioView()
{
	m_pdpDrawPort = NULL;
	m_pvpViewPort = NULL;

	m_angModelAngle = ANGLE3D(0.0f, 0.0f, 0.0f);
	m_plLightPlacement.pl_PositionVector = FLOAT3D( 0.0f, 0.0f, 0.0f); // center
	m_plLightPlacement.pl_OrientationAngle(1) = AngleDeg( 135.0f);      // heading
	m_plLightPlacement.pl_OrientationAngle(2) = AngleDeg( -45.0f);     // pitch
	m_plLightPlacement.pl_OrientationAngle(3) = AngleDeg( 0.0f);       // banking
	m_fLightDistance = 3.5f;
	OnResetView();
}

CSeriousSkaStudioView::~CSeriousSkaStudioView()
{
}

static FLOAT _fLastViewTick = 0;
static FLOAT _fLastLightTick = 0;

void CSeriousSkaStudioView::SetProjectionData( CPerspectiveProjection3D &prProjection, CDrawPort *pDP)
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	prProjection.FOVL() = AngleDeg(theApp.m_fFOV);
	prProjection.ScreenBBoxL() = FLOATaabbox2D( FLOAT2D(0.0f,0.0f),
	FLOAT2D((float)pDP->GetWidth(), (float)pDP->GetHeight()));
	prProjection.AspectRatioL() = 1.0f;
	prProjection.FrontClipDistanceL() = 0.05f;


	FLOAT fLerpedTick = _pTimer->GetLerpedCurrentTick();

	if(pDoc->m_bRotateView) {
		FLOAT fRot = (fLerpedTick-_fLastViewTick) * 20;
		// m_angViewerOrientation += FLOAT3D(fRot,0,0);
		m_angModelAngle += FLOAT3D(fRot,0,0);

		_fLastViewTick = fLerpedTick;
	}
	if(pDoc->m_bRotateLight) {
		FLOAT fRot = (fLerpedTick-_fLastLightTick) * 40;
		m_plLightPlacement.pl_OrientationAngle -= FLOAT3D(fRot,0,0);
		_fLastLightTick = fLerpedTick;
	}

	extern ENGINE_API CCameraEffect::CCameraValue g_cvCameraShake;
	static FLOAT3D vOldCameraPos;
	static FLOAT3D vOldCameraPosByCharCoord;
	
	ANGLE3D aView = m_angViewerOrientation;
	FLOAT3D vTarget = m_vTarget;

	//twist
	aView(3) += g_cvCameraShake.m_fTwist;

	//카메라 좌표기준 이동처리
	FLOAT3D vCameraShake(g_cvCameraShake.m_fHorizonal, g_cvCameraShake.m_fVertical, 0);
	FLOATmatrix3D matRot;
	MakeRotationMatrixFast(matRot, aView);
	RotateVector(vCameraShake, matRot);
	vTarget += vCameraShake - vOldCameraPos;
	vOldCameraPos = vCameraShake;

	FLOAT fTargetDistance = m_fTargetDistance;
	fTargetDistance += g_cvCameraShake.m_fZoom;

	//캐릭터 좌표기준 이동처리
	FLOAT3D vCameraPosByCharCoord = g_cvCameraShake.m_vMoveByCharCoord;
	if(pDoc->m_ModelInstance)
	{
		RotateVector(vCameraPosByCharCoord, pDoc->m_ModelInstance->mi_qvOffset.qRot);
	}
	vTarget += vCameraPosByCharCoord - vOldCameraPosByCharCoord;
	vOldCameraPosByCharCoord = vCameraPosByCharCoord;

	g_cvCameraShake.Reset();

	prProjection.ViewerPlacementL().pl_PositionVector = vTarget;
	prProjection.ViewerPlacementL().pl_OrientationAngle = aView;
	prProjection.Prepare();
	prProjection.ViewerPlacementL().Translate_OwnSystem(FLOAT3D( 0.0f, 0.0f, fTargetDistance));
}

void CSeriousSkaStudioView::OnIdle(void)
{
	Invalidate(FALSE);
}

BOOL CSeriousSkaStudioView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSeriousSkaStudioView drawing
static INDEX ctChildLevel=0;
#define MAKESPACE(x) (x>0?"%*c":""),x,' '
void CreateCurrentAnimationList(CModelInstance *pmi,CTString &strAnimations)
{
	CTString strText;
	CTString strTemp;
	INDEX ial = 0;
	const FLOAT fLerpedTick = _pTimer->GetLerpedCurrentTick();

	// return if no animsets
	INDEX ctas = pmi->mi_aAnimSet.Count();
	if(ctas == 0) {
		strText.PrintF(MAKESPACE(ctChildLevel));
		strText += CTString(0,"[%s]\n",(const char*)pmi->GetName());
		strAnimations += strText;
		return;
	}

	// count animlists
	INDEX ctal = pmi->mi_aqAnims.aq_Lists.Count();
	// find newes animlist that has fully faded in
	INDEX iFirstAnimList = 0;
	// loop from newer to older
	for( ial = ctal-1; ial >= 0; ial--) {
		AnimList &alList = pmi->mi_aqAnims.aq_Lists[ial];
		// calculate fade factor
		FLOAT fFadeFactor = CalculateFadeFactor(alList);
		if(fFadeFactor >= 1.0f) {
			iFirstAnimList = ial;
			break;
		}
	}

	strText.PrintF(MAKESPACE(ctChildLevel));
	strText += CTString(0,"[%s]\n",(const char*)pmi->GetName());
	strAnimations += strText;

	// for each anim list after iFirstAnimList
	for( ial = iFirstAnimList; ial < ctal; ial++) {
		AnimList &al = pmi->mi_aqAnims.aq_Lists[ial];
		AnimList *palListNext=NULL;
		if(ial+1<ctal) palListNext = &pmi->mi_aqAnims.aq_Lists[ial+1];
		
		INDEX ctpa = al.al_PlayedAnims.Count();
		// for each played anim in played anim list
		for(int ipa=0;ipa<ctpa;ipa++) {
			FLOAT fTime = _pTimer->GetLerpedCurrentTick();
			PlayedAnim &pa = al.al_PlayedAnims[ipa];

			strText.PrintF(MAKESPACE(ctChildLevel+1));
			strAnimations += strText;

			BOOL bAnimLooping = pa.pa_ulFlags & AN_LOOPING;

			INDEX iAnimSetIndex;
			INDEX iAnimIndex;
			// find anim by ID in all anim sets within this model
			if(pmi->FindAnimationByID(pa.pa_iAnimID,&iAnimSetIndex,&iAnimIndex)) {
				// if found, animate bones
				Animation &an = pmi->mi_aAnimSet[iAnimSetIndex].as_Anims[iAnimIndex];
				
				/*
				// calculate end time for this animation list
				FLOAT fEndTime = alList.al_fStartTime + alList.al_fFadeTime;
				// calculate curent and next frame in animation
				if(palListNext!=NULL) {
					if(fTime > palListNext->al_fStartTime) fTime = palListNext->al_fStartTime;
				}

				if(fTime < fEndTime) fTime = fEndTime;
				FLOAT f = (fTime - fEndTime) / (TIME)an.an_fSecPerFrame;

				INDEX iCurentFrame;
				INDEX iAnimFrame,iNextAnimFrame;
				
				if(bAnimLooping) {
					f = fmod(f,an.an_iFrames);
					iCurentFrame = INDEX(f);
					iAnimFrame = iCurentFrame % an.an_iFrames;
					iNextAnimFrame = (iCurentFrame+1) % an.an_iFrames;
				} else {
					if(f>an.an_iFrames) f = an.an_iFrames-1;
					iCurentFrame = INDEX(f);
					iAnimFrame = ClampUp(iCurentFrame,an.an_iFrames-1L);
					iNextAnimFrame = ClampUp(iCurentFrame+1L,an.an_iFrames-1L);
				}
				*/

				// calculate end time for this animation list
				FLOAT fFadeInEndTime = al.al_fStartTime + al.al_fFadeTime;

				// if animation freeze is on and if there is a newer animation list
				if(pa.pa_ulFlags&AN_FREEZE_ON_BLEND && palListNext!=NULL) {
					// freeze time of this one to never overlap with the newer list
					fTime = ClampUp(fTime, palListNext->al_fStartTime);
				}

				// calculate time passed since the animation started
				FLOAT fTimeOffset = fTime - pa.pa_fStartTime;
				// if animation freeze is on and if this animation list is fading in
				if(pa.pa_ulFlags&AN_FREEZE_ON_BLEND && fLerpedTick<fFadeInEndTime) {
					// offset the time so that it is paused at the end of fadein interval
					fTimeOffset += fFadeInEndTime - fLerpedTick;
				}

				// if fade out flag is set 
				if(pa.pa_ulFlags&AN_FADEOUT) {
					// calculate current animation strenght
					FLOAT fAnimEndLength = fTime - (pa.pa_fStartTime + (an.an_iFrames * an.an_fSecPerFrame * pa.pa_fSpeedMul));
					// fAnimStrength = Clamp(pa.pa_fStrength-(fAnimEndLength/al.al_fFadeTime),0.0f,1.0f);
				}
				
				FLOAT f = fTimeOffset / (an.an_fSecPerFrame*pa.pa_fSpeedMul);
				INDEX iCurentFrame;
				INDEX iAnimFrame,iNextAnimFrame;

				// if animation looping flag is set
				if(bAnimLooping) {
					// calculate current and next frame (wraped if anim has reached end)
					f = fmod(f,an.an_iFrames);
					iCurentFrame = INDEX(f);
					iAnimFrame = iCurentFrame % an.an_iFrames;
					iNextAnimFrame = (iCurentFrame+1) % an.an_iFrames;
				} else {
					// calculate current and next frame (last frame if anim has reached end)
					if(f>an.an_iFrames) {
						 f = an.an_iFrames;
						// if animation has flag to be removed on end
						if(pa.pa_ulFlags&AN_REMOVE_ON_END) {
							// add internal fade out flag
							pa.pa_ulFlags|=AN_FADEOUT;
						}
					}
					iCurentFrame = INDEX(f);
					iAnimFrame = ClampUp(iCurentFrame,an.an_iFrames-1L);
					iNextAnimFrame = ClampUp(iCurentFrame+1L,an.an_iFrames-1L);
				}
				iAnimFrame = ClampDn(iAnimFrame,(INDEX)0);


				FLOAT fAnimLength = an.an_iFrames * an.an_fSecPerFrame;
				FLOAT fFadeFactor = CalculateFadeFactor(al);
				strText.PrintF("%s %g - %g",(const char*)ska_GetStringFromTable(pa.pa_iAnimID),f,fFadeFactor);
				INDEX iLength = strText.Length();
				if(iLength<30) {
					strTemp.PrintF(MAKESPACE(30-iLength));
					strText+=strTemp;
				}
				strTemp.PrintF("[%g / %g] s=%g\n",f*an.an_fSecPerFrame,fAnimLength,pa.pa_fStrength);
				strAnimations += strText+strTemp;
			}
		}
		strText.PrintF(MAKESPACE(ctChildLevel+1));
		strAnimations += strText+"-----------------------\n";
	}
				
	INDEX ctcmi = pmi->mi_cmiChildren.Count();
	ctChildLevel+=2;
	// for each child model instance
	for(INDEX icmi=0;icmi<ctcmi;icmi++)   {
		CModelInstance *pcmi = &pmi->mi_cmiChildren[icmi];
		// create list of animations for child
		CreateCurrentAnimationList(pcmi,strAnimations);
	}
	ctChildLevel-=2;
}

INDEX CSeriousSkaStudioView::TestRayCastHit(CPoint &pt) 
{
	INDEX iHitBone = -1;
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	ASSERT(pDoc!=NULL);
	if(pDoc!=NULL) {
		CModelInstance *pmi = pDoc->m_ModelInstance;
		ASSERT(pmi!=NULL);
		if(pmi!=NULL) {
		
			INDEX iBoneID = -1;

			// set projection data
			CPerspectiveProjection3D prPerspectiveProjection;
			SetProjectionData( prPerspectiveProjection, m_pdpDrawPort);

			// prepare render model structure
			CAnyProjection3D apr;
			apr = prPerspectiveProjection;
			apr->Prepare();


			// get viewer's direction vector
			CPlacement3D plRay;
			apr->RayThroughPoint(FLOAT3D((float)pt.x,
				m_pdpDrawPort->GetHeight()-(float)pt.y, 0.0f), plRay);

			FLOAT3D vDirection;
			AnglesToDirectionVector( plRay.pl_OrientationAngle, vDirection);
			FLOAT3D vResult;
			vResult = plRay.pl_PositionVector + vDirection;

			// get placement of model
			FLOATmatrix3D mRot;
			MakeRotationMatrixFast( mRot, m_angModelAngle);
			FLOAT fZPos = pDoc->m_fSpeedZ*fmod(_pTimer->GetLerpedCurrentTick(),pDoc->m_fLoopSecends);
			FLOAT3D vPos = FLOAT3D(0.0f, 0.0f, -fZPos);

			RM_TestRayCastHit(*pmi,mRot,vPos,plRay.pl_PositionVector,vResult,300000,&iBoneID);
			if(iBoneID>=0) {
				iHitBone = iBoneID;
				const CTString &strBone = ska_GetStringFromTable(iBoneID);
				int a=0;
			}
		}
	}
	return iHitBone;
}

void CSeriousSkaStudioView::RenderLightModels(CDrawPort *pdp,CPlacement3D &pl)
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	// show light objects
	if(pDoc!=NULL && pDoc->m_bShowLights && theApp.pmiLight!=NULL)
	{
		CPlacement3D plLightPlacement;
		plLightPlacement.pl_OrientationAngle = m_plLightPlacement.pl_OrientationAngle;
		plLightPlacement.pl_PositionVector = FLOAT3D(0,0,0);
		plLightPlacement.Translate_OwnSystem( FLOAT3D( 0.0f, 0.0f, m_fLightDistance));
		if(pDoc->m_ModelInstance != NULL) {
			plLightPlacement.Translate_AbsoluteSystem( pDoc->m_ModelInstance->GetOffsetPos());
		}

		// back up current model flags
		ULONG ulModelFlags = RM_GetFlags();
		// change render flags
		RM_SetFlags(RMF_SHOWTEXTURE);
		
		RM_SetObjectPlacement(plLightPlacement);
		// render light model 
		RM_RenderSKA(*theApp.pmiLight);
		
		// restore model flags
		RM_SetFlags(ulModelFlags);
	}
}

void CSeriousSkaStudioView::RenderAxisModel(CDrawPort *pdp,CPlacement3D &pl)
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	// show light objects
	if(pDoc!=NULL && theApp.m_bShowAxis && theApp.pmiAxis!=NULL)
	{
		CPlacement3D plLightPlacement;
		plLightPlacement.pl_OrientationAngle = ANGLE3D(0,0,0);
		plLightPlacement.pl_PositionVector   = FLOAT3D(0,0,0);

		// back up current model flags
		ULONG ulModelFlags = RM_GetFlags();
		// change render flags
		RM_SetFlags(RMF_SHOWTEXTURE);
		RM_SetObjectPlacement(plLightPlacement);
		// render light model 
		RM_RenderSKA(*theApp.pmiAxis);
		// restore model flags
		RM_SetFlags(ulModelFlags);
	}
}

static void AdjustBonesCallback(void *pData)
{
	// aditionaly adjust bones 
}

static FLOAT AdjustMipDistance(const FLOAT fDistance)
{
	FLOAT fUpper = UpperLimit(1.0f);
	if(fDistance==fUpper || fDistance==(-1.0f)) {
		return fDistance;
	}

	const FLOAT fMipAdjustFactor = Tan(theApp.m_fFOV/2.0f);
	return fDistance / fMipAdjustFactor;
}


// yjpark      지울거야.. 불만이야?? -->

//effect sort시 사용될 class
//정렬 순서
//EFFECT_TYPE의 값 순.(작은게 먼저).
//active()한 것이 먼저. Active()하지 않은 것은 정렬하지 않음.
//뷰frustum안에 있는 것이 먼저. 뷰frustum안에 없는 것은 정렬하지 않음.
//뷰어에서 거리가 먼 것이 먼저.
template<class T>
class CompareEffect
{
public:
	//T는 포인터로 들어옴.
	bool operator()(T leftEffect, T rightEffect)
	{
		//EFFECT_TYPE의 값 순.(작은게 먼저).
		if(leftEffect->GetType() > rightEffect->GetType()) return true;
		else false;

		//active()한 것이 먼저. Active()하지 않은 것은 정렬하지 않음.
		if(leftEffect->Active() && !rightEffect->Active()) return true;
		if(!leftEffect->Active() && rightEffect->Active()) return false;
		if(!leftEffect->Active() && !rightEffect->Active()) return false;

		//뷰어에서 거리가 먼 것이 먼저.
		if(leftEffect->AbsoluteDistance() > rightEffect->AbsoluteDistance()) return true;
		return false;
	}
};

typedef CompareEffect<CEffect *> compare_effect;

//class for render
class CRenderFunc
{
public:
	inline void operator()(CEffect *pEffect)
	{
		if(pEffect->Playing()) pEffect->Render();
	}
};

static FLOAT s_fOneCircleTime = 4.0f;
static FLOAT s_fCircleRadius = 4.0f;
extern FLOAT g_tmEffectTimeAvr = 0;

void RunSkaStudioEffects(CAnyProjection3D &apr, CDrawPort *pdp, CTagManager *ptm, CModelInstance *pMI)
{
	FLOAT fCurrentTime = _pTimer->GetLerpedCurrentTick();
	
	CSoundListener sliSound;
	sliSound.sli_penEntity = NULL;
	sliSound.sli_vPosition = apr.ap_CurrentProjection->pr_ViewerPlacement.pl_PositionVector;
	sliSound.sli_mRotation = apr.ap_CurrentProjection->pr_ViewerRotationMatrix;
	sliSound.sli_vSpeed    = FLOAT3D(0,0,0);
	sliSound.sli_fVolume   = 1.0f;
	sliSound.sli_eEnvType = (SndEnvFX)0;
	sliSound.sli_fFilter  = 0.0f;
	
	// add this listener to list
	_pSound->Listen(sliSound);
	
	//moving tag
	CWorldTag hitTag;
	hitTag.SetName("TEST-HIT");
	hitTag.Activate();
	hitTag.AddNewTagInfo(0, FLOATquat3D(0.7071f,0.7071f,0,0), FLOAT3D(0, 1, -1), FLOAT3D(1,1,1));
	ptm->Register(&hitTag);
	
	if(!ptm->IsRegister("TEST-MOVE"))
	{
		CWorldTag movingTag;
		movingTag.SetName("TEST-MOVE");
		ptm->Register(&movingTag);
		movingTag.SetName("__0_MOVE_G2");
		ptm->Register(&movingTag);
		movingTag.SetName("__1_MOVE_G2");
		ptm->Register(&movingTag);
		movingTag.SetName("__0_MOVE_G3");
		ptm->Register(&movingTag);
		movingTag.SetName("__1_MOVE_G3");
		ptm->Register(&movingTag);
		movingTag.SetName("__2_MOVE_G3");
		ptm->Register(&movingTag);
	}
	CWorldTag &movingTag = (CWorldTag&)*ptm->Find("TEST-MOVE");
	movingTag.Activate();

	CWorldTag &movingTagG2_1 = (CWorldTag&)*ptm->Find("__0_MOVE_G2");
	movingTagG2_1.Activate();
	CWorldTag &movingTagG2_2 = (CWorldTag&)*ptm->Find("__1_MOVE_G2");
	movingTagG2_2.Activate();
	if(!ptm->IsRegister("MOVE_G2"))
	{
		CGroupTag movingTagG2;
		movingTagG2.SetName("MOVE_G2");
		movingTagG2_1.Activate();
		movingTagG2.AddTag(ptr_tag(&movingTagG2_1));
		movingTagG2_2.Activate();
		movingTagG2.AddTag(ptr_tag(&movingTagG2_2));
		ptm->Register(&movingTagG2);
	}
	CGroupTag &movingTagG2 = (CGroupTag&)*ptm->Find("MOVE_G2");
	movingTagG2.Activate();

	CWorldTag &movingTagG3_1 = (CWorldTag&)*ptm->Find("__0_MOVE_G3");
	movingTagG3_1.Activate();
	CWorldTag &movingTagG3_2 = (CWorldTag&)*ptm->Find("__1_MOVE_G3");
	movingTagG3_2.Activate();
	CWorldTag &movingTagG3_3 = (CWorldTag&)*ptm->Find("__2_MOVE_G3");
	movingTagG3_3.Activate();
	if(!ptm->IsRegister("MOVE_G3"))
	{
		CGroupTag movingTagG3;
		movingTagG3.SetName("MOVE_G3");
		movingTagG3_1.Activate();
		movingTagG3.AddTag(ptr_tag(&movingTagG3_1));
		movingTagG3_2.Activate();
		movingTagG3.AddTag(ptr_tag(&movingTagG3_2));
		movingTagG3_3.Activate();
		movingTagG3.AddTag(ptr_tag(&movingTagG3_3));
		ptm->Register(&movingTagG3);
	}
	CGroupTag &movingTagG3 = (CGroupTag&)*ptm->Find("MOVE_G3");
	movingTagG3.Activate();

	FLOAT radian = 2 * PI * (fCurrentTime - INDEX(fCurrentTime / s_fOneCircleTime) * s_fOneCircleTime) / s_fOneCircleTime;
	FLOAT3D pos(cos(radian), sin(radian*4), sin(radian));
	FLOATquat3D quat;
	FLOAT3D axis(pos(1), 0, pos(3));
	quat.FromAxisAngle(axis, PI*0.5f);

	FLOAT3D vPos = pos;
	vPos(1) *= s_fCircleRadius;
	vPos(3) *= s_fCircleRadius;
	movingTag.AddNewTagInfo(fCurrentTime, quat, vPos, FLOAT3D(1,1,1));
	vPos = pos;
	vPos(1) *= s_fCircleRadius;
	vPos(2) -= 0.1f;
	vPos(3) *= s_fCircleRadius;
	movingTagG2_1.AddNewTagInfo(fCurrentTime, quat, vPos, FLOAT3D(1,1,1));
	movingTagG3_1.AddNewTagInfo(fCurrentTime, quat, vPos, FLOAT3D(1,1,1));
	vPos = pos;
	vPos(1) *= s_fCircleRadius - 0.05f;
	vPos(2) += 0.05f;
	vPos(3) *= s_fCircleRadius - 0.05f;
	movingTagG2_2.AddNewTagInfo(fCurrentTime, quat, vPos, FLOAT3D(1,1,1));
	movingTagG3_2.AddNewTagInfo(fCurrentTime, quat, vPos, FLOAT3D(1,1,1));
	vPos = pos;
	vPos(1) *= s_fCircleRadius + 0.05f;
	vPos(2) += 0.05f;
	vPos(3) *= s_fCircleRadius + 0.05f;
	movingTagG3_3.AddNewTagInfo(fCurrentTime, quat, vPos, FLOAT3D(1,1,1));
	
	extern CTimer *_pTimer;
	CTimerValue tvOld = _pTimer->GetHighPrecisionTimer();
	
	//hardcoding process
	CEffectManager::my_list listEE = CEffectManager::Instance().GetCreatedList(ET_ENTITY);
	if(!listEE.empty())
	{
		CEffectManager::my_list::iterator iterBegin = listEE.begin();
		CEffectManager::my_list::iterator iterEnd = listEE.end();
		CEffectManager::my_list::iterator iter;
		for(iter = iterBegin; iter != iterEnd; ++iter)
		{
			((CEntityEffect*)(*iter))->SetTestValue(pMI);
		}
	}

	//Process Effects

	CEffectGroupManager::my_list listDelete;
	CEffectGroupManager::my_list::const_iterator iterBegin = CEffectGroupManager::Instance().GetCreatedList().begin();
	CEffectGroupManager::my_list::const_iterator iterEnd = CEffectGroupManager::Instance().GetCreatedList().end();
	CEffectGroupManager::my_list::const_iterator iter;
	for(iter = iterBegin; iter != iterEnd; ++iter)
	{
		CEffectGroup &eg = *(*iter);
		eg.SetProjection(apr);
		if(eg.Active() && !eg.Process(fCurrentTime))
		{
			listDelete.push_back(&eg);
		}
	}
	for(CEffectGroupManager::my_list::iterator it=listDelete.begin(); it!=listDelete.end(); ++it)
	{
		CEffectGroupManager::Instance().Destroy(*it);
	}
	listDelete.clear();

	//Render Effects
	CAnyProjection3D &prProjection = apr;
	
	//PrepareRender_EffectSystem(NULL, NULL);
	
	CRenderFunc renderFunc;
	for(DWORD dwEffectType=0; dwEffectType<(DWORD)ET_COUNT; ++dwEffectType)
	{
		EFFECT_TYPE effectType = (EFFECT_TYPE)dwEffectType;
		
		if(effectType == ET_LIGHT) continue;

		CEffectManager::my_list &listCreated = CEffectManager::Instance().GetCreatedList(effectType);
		if(listCreated.empty()) continue;
		if((*listCreated.begin())->BeginRender(prProjection, pdp))
		{
			CEffectManager::my_list::const_iterator iter;
			std::for_each(listCreated.begin(), listCreated.end(), renderFunc);
			(*listCreated.begin())->EndRender(FALSE);
		}
	}

	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
	DOUBLE elapse = (tvNow-tvOld).GetSeconds();
	static int countEffect = 0;
	static float timeEffect = 0;
	timeEffect += elapse;
	++countEffect;
	//if(timeEffect >= 0.01f)
	{
		g_tmEffectTimeAvr = timeEffect / countEffect;
		countEffect = 0;
		timeEffect = 0;
	}
}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

//안태훈 수정 시작	//(Open beta)(2004-11-30)
#define TEXT_MUL 1.25f
//안태훈 수정 끝	//(Open beta)(2004-11-30)
void CSeriousSkaStudioView::RenderView(CDrawPort *pdp)
{
	// do not render if rendering is disabled
	if(theApp.m_bDisableRender) {
		return;
	}

	CSeriousSkaStudioDoc* pDoc = GetDocument();

	pDoc->SetTimerForDocument();

	pdp->Fill(pDoc->m_colAirColor | CT_OPAQUE);
	pdp->FillZBuffer(ZBUF_BACK);
	// pdp->SetTextLineSpacing( 64);
	// pdp->SetFont( _pfdDisplayFont);
	pdp->SetFont(theApp.m_pfdDefaultFont);
	// pdp->SetTextAspect( 1.0f);
			
	CPerspectiveProjection3D prPerspectiveProjection;

	CPlacement3D pl;
	pl.pl_OrientationAngle = m_angModelAngle;
	pl.pl_PositionVector   = FLOAT3D(0.0f, 0.0f, 0.0f);

//강동민 수정 시작
	//-----------------------------------------------------------------------------
	// 쉐도우 매핑 행렬 생성.
	// Eye의 위치 설정.
	CPlacement3D plEye;
	plEye.pl_PositionVector = m_vTarget;						// Camera의 월드상의 위치.
	plEye.pl_OrientationAngle = m_angViewerOrientation;
	plEye.Translate_OwnSystem(FLOAT3D( 0.0f, 0.0f, m_fTargetDistance));

	// 광원의 위치 설정.
	CPlacement3D plLight = m_plLightPlacement;
	plLight.Translate_OwnSystem(FLOAT3D( 0.0f, 0.0f, m_fLightDistance));

	Matrix16 matShadowUV;
//강동민 수정 시작	03.05
//	CreateShadowMatrix(pDoc->m_ModelInstance, pl, matShadowUV, SHADOWTEXTURESIZE, plEye);
//강동민 수정 끝	03.05
	CreateShadowMatrix(pDoc->m_ModelInstance, pl, matShadowUV, pDoc->m_matWorldToLight, pDoc->m_matShadowProj, SHADOWTEXTURESIZE, plEye);
//강동민 수정 끝

	// 모델에 이펙트 효과 적용중인데 아래에서 새로 설정해 버린다.
	
	if(pmiSelected!=NULL) {
		// Adjust custom model instance stretch
		CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
		CString strStretch;
		pMainFrame->m_ctrlMIStretch.GetWindowText(strStretch);
		FLOAT fStretch = atof((const char*)strStretch);
		pmiSelected->mi_vStretch = FLOAT3D(fStretch,fStretch,fStretch);
//안태훈 수정 시작	//(Open beta)(2004-11-30)
		CString strTemp;
		pMainFrame->m_ctrlMIAlpha.GetWindowText(strTemp);
		ULONG ulAlpha = atoi((const char*)strTemp);
		ULONG ulAlphaNew = ulAlpha > 255 ? 255 : ulAlpha;
		if(ulAlphaNew != ulAlpha)
		{
			strTemp.Format("%d", ulAlphaNew);
			pMainFrame->m_ctrlMIAlpha.SetWindowText(strTemp);
		}
		pmiSelected->mi_colModelColor = (pmiSelected->mi_colModelColor & 0xFFFFFF00) | ulAlphaNew;
//안태훈 수정 끝	//(Open beta)(2004-11-30)
	}


	if(pDoc->m_ModelInstance != NULL)
	{
/*
		ANGLE3D a3dTemp = GetEulerAngleFromDir(pDoc->m_vMeasurePointPosition);
		if(pDoc->m_vMeasurePointPosition(1) < 0)
		{
			a3dTemp(1) = a3dTemp(1) - 6;
		}
		FLOAT offset[6] = {0,0,0, a3dTemp(1), a3dTemp(2), a3dTemp(3)};
		pDoc->m_ModelInstance->SetOffset(offset);
*/

		// set projection data
		SetProjectionData( prPerspectiveProjection, pdp);

		// prepare render model structure
		CAnyProjection3D apr;
		apr = prPerspectiveProjection;

		RM_BeginRenderingView(apr, m_pdpDrawPort);
		// set placement of model
		CPlacement3D pl;
		pl.pl_OrientationAngle = m_angModelAngle;
		FLOAT fZPos = pDoc->m_fSpeedZ*fmod(_pTimer->GetLerpedCurrentTick(),pDoc->m_fLoopSecends);
		pl.pl_PositionVector   = FLOAT3D(0.0f, 0.0f, -fZPos);
		RM_SetObjectPlacement(pl);

		// Set color of shading light
		RM_SetLightColor(pDoc->m_colAmbient, pDoc->m_colLight);
		// Set light direction
		FLOAT3D vLightDir;
		AnglesToDirectionVector(pDoc->m_vLightDir,vLightDir);

		CPlacement3D plLightPlacement;
		plLightPlacement.pl_OrientationAngle = m_plLightPlacement.pl_OrientationAngle;
		plLightPlacement.pl_PositionVector = FLOAT3D(0,0,0);
		plLightPlacement.Translate_OwnSystem( FLOAT3D( 0.0f, 0.0f, m_fLightDistance));
		if(pDoc->m_ModelInstance != NULL) {
			plLightPlacement.Translate_AbsoluteSystem( pDoc->m_ModelInstance->GetOffsetPos());
		}

		RM_SetLightDirection(-plLightPlacement.pl_PositionVector);
//강동민 수정 시작		03.05
		/*
	// NOTE : 흰색으로 칠해지지 않고 그림자 색상으로 칠해지고 있음...		
	if(pDoc->m_bShowShadowMap)
	{
		shaCullFace(GFX_NONE);
		shaClearTextureMatrix();
		
		pDoc->m_ModelInstance->mi_rtShadow.rt_tdTexture.SetAsCurrent();
		
		float vertex2D[4][4+2] = 
		{
			{   0,					0,					0,1, 0,0 },
			{ SHADOWTEXTURESIZE,	0,					0,1, 1,0 },
			{ SHADOWTEXTURESIZE,	SHADOWTEXTURESIZE,	0,1, 1,1 },
			{   0,					SHADOWTEXTURESIZE,	0,1, 0,1 },
		};
		_pGfx->gl_pd3dDevice->SetVertexShader( D3DFVF_XYZRHW|D3DFVF_TEX1 );
		_pGfx->gl_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,vertex2D,sizeof(float)*(4+2));
		shaCullFace(GFX_FRONT);
	}
	*/
//강동민 수정 끝		03.05
		// 모델 상태 이펙트 테스트용 다른 이펙트에 영향이 가므로, 여기서 설정하면 안된다.
		// 추후 소스 변경요망
		//RunSkaStudioEffects(apr, pdp, &(pDoc->m_ModelInstance->m_tmSkaTagManager), pDoc->m_ModelInstance);

	// Show ground
		if(pDoc->m_bShowGround) 
		{
//강동민 수정 시작
			// 지형 텍스쳐.
			shaEnableDepthTest();
			shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
			RM_RenderGround(theApp.toGroundTexture, pDoc->m_colGroundColor);		
		
//강동민 수정 시작		03.05
			// Shadow를 그릴것인가?
			/*
			if(pDoc->m_bShowShadow)
			{
				// 그림자 텍스쳐.
				shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP);
				shaBlendFunc(GFX_ZERO, GFX_SRC_COLOR);
				shaEnableBlend();
				shaSetTextureUnit(0);
				// 카메라 좌표계의 버텍스좌표를 텍스쳐 행렬의 입력으로 사용함.
				_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION );
				_pGfx->gl_pd3dDevice->SetTransform( D3DTS_TEXTURE0, (_D3DMATRIX*)&matShadowUV);
				
				// 동차좌표계 텍스쳐 변환 행렬을 사용하도록 설정해준다. 
				// 이렇게 해야 D3D가 투영 나눗셈(projection divide)연산을 한다.
				_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT4|D3DTTFF_PROJECTED );			
				RM_RenderGround(pDoc->m_ModelInstance->mi_rtShadow.rt_tdTexture);
				shaDisableBlend();
				_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU );
				_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
				shaClearTextureMatrix();
			}
			*/
//강동민 수정 끝		03.05
			// Shadow를 그릴것인가?
/*
			if(pDoc->m_bShowShadow && pDoc->m_bInitTexture)
			{
				// 그림자 텍스쳐.
				shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP);
				shaBlendFunc(GFX_ZERO, GFX_SRC_COLOR);
				shaEnableBlend();
				shaSetTextureUnit(0);
				// 카메라 좌표계의 버텍스좌표를 텍스쳐 행렬의 입력으로 사용함.
				_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION );
				_pGfx->gl_pd3dDevice->SetTransform( D3DTS_TEXTURE0, (_D3DMATRIX*)&matShadowUV);
				
				// 동차좌표계 텍스쳐 변환 행렬을 사용하도록 설정해준다. 
				// 이렇게 해야 D3D가 투영 나눗셈(projection divide)연산을 한다.
				_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT4|D3DTTFF_PROJECTED );			
				RM_RenderGround(pDoc->m_rtShadow.rt_tdTexture);
				shaDisableBlend();
				_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU );
				_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
				shaClearTextureMatrix();
			}
*/
//강동민 수정 끝
		}

		// Calc mesh and skeleton distance
		FLOAT fMeshDistance = 0.0f;
		FLOAT fSkelDistance = 0.0f;

		// auto mipping
		if(pDoc->m_bAutoMiping) {
			fMeshDistance = -1;
			if(pDoc->m_bOnlyMorphs) {
				fSkelDistance = UpperLimit(1.0f);
			} else {
				fSkelDistance = -1;
			}
		} else {
			// custom mipping
			fMeshDistance = pDoc->m_fCustomLodDist;
			if(pDoc->m_bOnlyMorphs) {
				fSkelDistance = UpperLimit(1.0f);
			} else {
				fSkelDistance = pDoc->m_fCustomLodDist;
			}
		}

		// fMeshDistance = AdjustMipDistance(fMeshDistance);
		// fSkelDistance = AdjustMipDistance(fSkelDistance);

		// set max weights per vertex
		RM_SetMaxWeightsPerVertex(pDoc->m_ctBonesPerVertex);
		
		/*
		// Render shadow
		FLOATplane3D plShadowPlane = FLOATplane3D(FLOAT3D(0,1,0),0);
		pDoc->m_ModelInstance->AddSimpleShadow(1.0f,plShadowPlane);
		*/

		RM_SetBoneAdjustCallback(&AdjustBonesCallback,this);

		// if showing selection
		if(theApp.m_bDisplaySelection && pDoc->m_spfPreviewMode==SPF_SELECTION && theApp.iSelectedItemID>=0) {
			NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
			SkaPreviewFlags spf = SPF_NONE;
			switch(ni.ni_iType) {
				case NT_BONE:
					spf = SPF_WEIGHTS;
				break;
				case NT_WEIGHT_MAP:
					spf = SPF_WEIGHTS;
				break;
				case NT_MORPH_MAP:
					spf = SPF_MORPHS;
				break;
				case NT_MESHSURFACE:
					if(theApp.m_bDisplaySubSurfaces) {
						spf = SPF_SUBSURFACES;
					} else {
						spf = SPF_SURFACES;
					}
				break;
				default:
					spf = SPF_WEIGHTS;
				break;
			}
			RM_SetCurrentDistance(fMeshDistance);
			RM_RenderSKA(*pDoc->m_ModelInstance, TRUE);

			RM_SetCurrentDistance(fMeshDistance);
			RM_PreviewSKA(*pDoc->m_ModelInstance,m_pdpDrawPort,spf,theApp.iSelectedItemID);
		} else if(pDoc->m_spfPreviewMode!=SPF_SELECTION && pDoc->m_spfPreviewMode!=SPF_NONE) {

			RM_SetCurrentDistance(fMeshDistance);
			RM_PreviewSKA(*pDoc->m_ModelInstance,m_pdpDrawPort,pDoc->m_spfPreviewMode);
		// normal rendering
		} else {
			INDEX ctModels;
			if(theApp.m_bSceneProfiling) {
				ctModels = 100;
			} else {
				ctModels = 1;
			}
			for(INDEX irm=0;irm<ctModels;irm++) {
				RM_SetCurrentDistance(fMeshDistance);
				RM_RenderSKA(*pDoc->m_ModelInstance, TRUE);
			}
		}
		
		// Show wireframe
		if(pDoc->m_bShowWireframe) {

			RM_SetCurrentDistance(fMeshDistance);
			RM_PreviewSKA(*pDoc->m_ModelInstance,m_pdpDrawPort,SPF_WIREFRAME);
		}
		
		// Show normals
		if(pDoc->m_bShowNormals) {

			RM_SetCurrentDistance(fMeshDistance);
			RM_PreviewSKA(*pDoc->m_ModelInstance,m_pdpDrawPort,SPF_NORMALS);
		}

		CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
		HTREEITEM hSelectedItem = m_TreeCtrl.GetSelectedItem();
		NodeInfo *pni = NULL;
		if(hSelectedItem!=NULL) {
			pni = &theApp.m_dlgBarTreeView.GetNodeInfo(hSelectedItem);
		}
		
		BOOL bShowColisionBoxes = pDoc->m_bShowColisionBox;
		if(pni!=NULL && pni->ni_iType == NT_COLISIONBOX) {
			bShowColisionBoxes = TRUE;
		}
		BOOL bShowAllFramesBBox = pDoc->m_bShowAllFramesBBox;
		if(pni!=NULL && pni->ni_iType == NT_ALLFRAMESBBOX) {
			bShowAllFramesBBox = TRUE;
		}

		// show colision box
		if(pmiSelected->mi_cbAABox.Count()>0 &&pmiSelected->mi_iCurentBBox>=0) {
			if(bShowColisionBoxes) {
				ColisionBox &cb = pmiSelected->GetColisionBox(pmiSelected->mi_iCurentBBox);
				RM_RenderColisionBox(*pmiSelected,cb,C_mlGREEN);
			}
		}
		// show all frames box
		if(bShowAllFramesBBox) {
			ColisionBox &cbAllFrames = pmiSelected->mi_cbAllFramesBBox;
			RM_RenderColisionBox(*pmiSelected,cbAllFrames,C_ORANGE);
		}
		// Show cross
		if(pDoc->m_bMeasuringToolOn) {
			TIME tmNow = _pTimer->GetHighPrecisionTimer().GetSeconds();
			ULONG ulAlpha = sin(tmNow*16)*96 +128;
			RM_RenderCross(pDoc->m_vMeasurePointPosition,0xFFFF003C|(ulAlpha&0x000000FF));
		}

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
		if(pDoc->m_bShowSpline)
		{
			RM_RenderSpline(pDoc->m_vectorSplinePoint.size(), &pDoc->m_vectorSplinePoint[0], pDoc->m_siCurrentPoint, 0xFF0000FF);
		}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

		// Show ligth
		RenderLightModels(pdp,pl);
		// Show axis
		RenderAxisModel(pdp,pl);

		RM_EndRenderingView();

//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
		RM_BeginRenderingView(apr, pdp);
		RM_SetLightDirection(FLOAT3D(0,-1,0));
		RM_SetLightColor(C_WHITE|CT_OPAQUE, C_BLACK|CT_OPAQUE);
		RM_SetMaxWeightsPerVertex( 4 );
		RM_SetCurrentDistance( 0.0f );	//always use first lod
		extern CTag *_pSelectedTag;
		if(_pSelectedTag != NULL)
		{
			FLOATmatrix3D mRot;
			_pSelectedTag->GetOffsetRot().ToMatrix(mRot);
			RM_SetObjectPlacement(mRot, _pSelectedTag->GetOffsetPos());
			_pSelectedTag->Render(0x00000000);
		}
		RM_EndRenderingView(TRUE);
		
		RunSkaStudioEffects(apr, pdp, &(pDoc->m_ModelInstance->m_tmSkaTagManager), pDoc->m_ModelInstance);
		pdp->SetOrtho();

		shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

		// show paused indicator
		if(pDoc->m_bViewPaused) {
			TIME tmNow = _pTimer->GetHighPrecisionTimer().GetSeconds();
			ULONG ulAlpha = sin(tmNow*16)*96 +128;
			COLOR colEvent = 0xffff0000|ulAlpha;
			pDoc->AddShowEvent("Paused",EV_APP_PAUSED,1.0f,colEvent);
			/*
			pdp->SetFont(theApp.m_pfdDefaultFont);
			pdp->PutText( "Paused", m_pdpDrawPort->GetWidth()-50, ClampDn(m_pdpDrawPort->GetHeight()-15,0), 0xffff0000|ulAlpha);
			*/
		}

		if(theApp.m_bShowFPS) {
			#define FRAMES_AVERAGING_MAX 20L
			static  CTimerValue _tvLasts[FRAMES_AVERAGING_MAX];
			static  CTimerValue _tvDelta[FRAMES_AVERAGING_MAX];
			static  INDEX _iCheckNow  = 0;
			static  INDEX _iCheckMax  = 0;
			// calculate FPS
			FLOAT fFPS = 0.0f;
			_iCheckMax++;
			if( _iCheckMax >= FRAMES_AVERAGING_MAX) {
				for( INDEX i=0; i<FRAMES_AVERAGING_MAX; i++) fFPS += _tvDelta[i].GetSeconds();
				fFPS = FRAMES_AVERAGING_MAX*FRAMES_AVERAGING_MAX / fFPS;
				_iCheckMax = FRAMES_AVERAGING_MAX;
			}

			// determine newest time
			CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
			_tvDelta[_iCheckNow] = tvNow - _tvLasts[_iCheckNow];
			_tvLasts[_iCheckNow] = tvNow;
			_iCheckNow = (_iCheckNow+1) % FRAMES_AVERAGING_MAX;
			// display colored FPS 
			COLOR colFPS = C_RED;
			if( fFPS >= 30) colFPS = C_GREEN;
			if( fFPS >= 60) colFPS = C_WHITE;
			pDoc->AddShowEvent(CTString(0,"FPS = %3.2f",fFPS),EV_FPS,1.0f,colFPS);
//안태훈 수정 시작	//(For Performance)(0.1)
			extern FLOAT g_tmEffectTimeAvr;
			COLOR colEffectTime = C_WHITE;
			if( g_tmEffectTimeAvr > 0.005f ) colEffectTime = C_GREEN;
			if( g_tmEffectTimeAvr > 0.01f ) colEffectTime = C_YELLOW;
			if( g_tmEffectTimeAvr > 0.05f ) colEffectTime = C_RED;
			pDoc->AddShowEvent(CTString(0, "Effect Time = %0.6f", g_tmEffectTimeAvr), EV_EFFECT_TIME, 1.0f, colEffectTime);
//안태훈 수정 끝	//(For Performance)(0.1)
		}

		PIX iMeasureTextOffsetY = 15;
		// Show anim queue
		if(pDoc->m_bShowAnimQueue) {
			iMeasureTextOffsetY+=15;
			pdp->Fill(0,0,200,m_pdpDrawPort->GetHeight(),0x0000007f,0x00000000,0x0000007f,0x00000000);
			// show curent played animations
			pdp->SetFont(theApp.m_pfdDefaultFont);

			CTString strAnimations;
			CreateCurrentAnimationList(pDoc->m_ModelInstance,strAnimations);
//안태훈 수정 시작	//(Taiwan Closed beta)(2004-11-26)
			m_pdpDrawPort->dp_fTextScaling = TEXT_MUL;
//안태훈 수정 끝	//(Taiwan Closed beta)(2004-11-26)
			pdp->PutText( strAnimations, 0, 0, 0xffffff00|CT_OPAQUE);

			FLOATmatrix3D m;
			Matrix12 mObjToAbs;
			Matrix12 mAbsToViewer;
			Matrix12 mObjToView;

			MakeRotationMatrixFast( m, pl.pl_OrientationAngle);
			MatrixVectorToMatrix12(mObjToAbs,m, pl.pl_PositionVector);
			MatrixVectorToMatrix12(mAbsToViewer,apr->pr_ViewerRotationMatrix, 
														 -apr->pr_vViewerPosition*apr->pr_ViewerRotationMatrix);

			MatrixMultiply(mObjToView,mAbsToViewer,mObjToAbs);


			// show distance 
			// pdp->SetFont( _pfdDisplayFont);
			pdp->SetFont(theApp.m_pfdDefaultFont);
			// pdp->SetTextScaling(0.95f);

			FLOAT3D vDist = FLOAT3D(mObjToView[ 3], mObjToView[7], mObjToView[11]);
			FLOAT fDistance = vDist.Length();
			if(!pDoc->m_bAutoMiping) {
				fDistance = pDoc->m_fCustomLodDist;
			} else {
				fDistance = fDistance * Tan(theApp.m_fFOV/2.0f);
			}
			
			// FLOAT fMipDistance = AdjustMipDistance(fDistance);//fDistance;// * Tan(theApp.m_fFOV/2.0f);
			FLOAT fMipDistance = fDistance;
			CTString strDistance = CTString(0,"Distance = %g",fMipDistance);
//안태훈 수정 시작	//(Taiwan Closed beta)(2004-11-26)
			m_pdpDrawPort->dp_fTextScaling = TEXT_MUL;
//안태훈 수정 끝	//(Taiwan Closed beta)(2004-11-26)
			pdp->PutText(strDistance, 0, ClampDn(PIX(m_pdpDrawPort->GetHeight()-15), PIX(0x00ff00ff)));
		}

		// if measuring tool is on
		if(pDoc->m_bMeasuringToolOn) {
			FLOAT3D &vPoint = pDoc->m_vMeasurePointPosition;
			pdp->SetFont( _pfdConsoleFont);
//안태훈 수정 시작	//(Taiwan Closed beta)(2004-11-26)
			m_pdpDrawPort->dp_fTextScaling = TEXT_MUL;
//안태훈 수정 끝	//(Taiwan Closed beta)(2004-11-26)
			pdp->PutText(CTString(0,"%g,%g,%g",vPoint(1),vPoint(2),vPoint(3)),0,ClampDn((INDEX)m_pdpDrawPort->GetHeight()-iMeasureTextOffsetY,0L),0xffff00FF);
		}

		INDEX ctse = pDoc->m_seShowEvents.Count();
		if(ctse>0) {
			const TIME tmNow = _pTimer->GetHighPrecisionTimer().GetSeconds();
			INDEX iOffsetY = 0;
			pdp->SetFont(theApp.m_pfdDefaultFont);
			CTString strEventText;
			for(INDEX ise=0;ise<ctse;ise++) {
				const ShowEvent &se = pDoc->m_seShowEvents[ise];
				TIME tmPassed = tmNow - se.se_fStartAt;
				FLOAT fFade = se.se_fExpireAfter - tmPassed;
				if(fFade<0.0f) {
					continue;
				}
				fFade = ClampUp(fFade,1.0f);
				ULONG ulAlpha = NormFloatToByte(fFade); // 1 second fade out

				iOffsetY += theApp.m_pfdDefaultFont->GetHeight()+1;
				PIX pixTextWidth = 0;
				const CTString &strText = se.se_strText;
				for(INDEX ich=0;ich<strText.Length();ich++) {
					const char ch = strText[ich];
					pixTextWidth+=theApp.m_pfdDefaultFont->GetCharWidth(ch);
				}
				pixTextWidth+=theApp.m_pfdDefaultFont->GetCharWidth(32);
				COLOR colEvent = se.se_colColor;
				// if alpha was specified
				if((colEvent&0x000000FF)==0) {
					colEvent|=ulAlpha;
				}
				// PIX pixTextWidth = se.se_strText.Length() * _pfdConsoleFont->GetCharWidth(32);
//안태훈 수정 시작	//(Taiwan Closed beta)(2004-11-26)
				m_pdpDrawPort->dp_fTextScaling = TEXT_MUL;
//안태훈 수정 끝	//(Taiwan Closed beta)(2004-11-26)
				pdp->PutText(se.se_strText,ClampDn(m_pdpDrawPort->GetWidth() - 2 * pixTextWidth,0L), 
										 ClampDn(m_pdpDrawPort->GetHeight()-/*((ise+1)*15)*/2 * iOffsetY,0L),colEvent);
			}
		}

		/*
		if(theApp.iSelectedBoneID>=0) {
			FLOAT3D vStartPoint;
			FLOAT3D vEndPoint;
			if(RM_GetBoneAbsPosition(*pDoc->m_ModelInstance,theApp.iSelectedBoneID,vStartPoint,vEndPoint)) {
				CDlgClient *pDlgCurrent = (CDlgClient*)theApp.m_dlgBarTreeView.pdlgCurrent;
				if(pDlgCurrent == &theApp.m_dlgBarTreeView.m_dlgBone) {
					FLOAT3D vDirection = vEndPoint - vStartPoint;
					ANGLE3D angAngle;
					DirectionVectorToAngles(vDirection,angAngle);
					
					pDlgCurrent->GetDlgItem(IDC_LB_POSX)->SetWindowText((const char*)CTString(0,"%g",vStartPoint(1)));
					pDlgCurrent->GetDlgItem(IDC_LB_POSY)->SetWindowText((const char*)CTString(0,"%g",vStartPoint(2)));
					pDlgCurrent->GetDlgItem(IDC_LB_POSZ)->SetWindowText((const char*)CTString(0,"%g",vStartPoint(3)));
					pDlgCurrent->GetDlgItem(IDC_LB_HEADING)->SetWindowText((const char*)CTString(0,"%g",angAngle(1)));
					pDlgCurrent->GetDlgItem(IDC_LB_PITCH)->SetWindowText((const char*)CTString(0,"%g",angAngle(2)));
					pDlgCurrent->GetDlgItem(IDC_LB_BANKING)->SetWindowText((const char*)CTString(0,"%g",angAngle(3)));
				} else {
					ASSERT(FALSE); // Current client dialog must be texture dialog
				}
			}
		}*/
	}
}

//강동민 수정 시작
//-----------------------------------------------------------------------------
// Purpose: Shadow Map을 생성합니다.
//-----------------------------------------------------------------------------
void CSeriousSkaStudioView::GenerateShadowTexture()
{
	static int i = 0;
	CSeriousSkaStudioDoc* pDoc = GetDocument();
	ASSERT(m_pdpDrawPort != NULL && "메인 DrawPort가 유효하지 않습니다.");

	m_pdpDrawPort->SetAsCurrent();
	CPlacement3D plLight = m_plLightPlacement;
	plLight.Translate_OwnSystem(FLOAT3D( 0.0f, 0.0f, m_fLightDistance));
	
//강동민 수정 시작		03.05
	/*
	CModelInstance* pMI = pDoc->m_ModelInstance;
	if(!pMI->mi_bInitTexture)
	{
		pMI->mi_rtShadow.Init(SHADOWTEXTURESIZE, SHADOWTEXTURESIZE);
		pMI->mi_bInitTexture = TRUE;
	}
	pMI->mi_rtShadow.Begin();
	pMI->mi_rtShadow.Clear(0xFFFFFFFF);
				
	// SKA 모델 렌더링.
	//RenderOneSkaModelToTexture(en, CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0)), plLight, FALSE, dm.dm_fMipFactor, dm.dm_ulFlags);
	RenderModelShadow(pDoc->m_ModelInstance, m_pdpDrawPort, m_pvpViewPort, SHADOWTEXTURESIZE, plLight);
	pMI->mi_rtShadow.End();
	*/
//강동민 수정 끝		03.05
	CModelInstance* pMI = pDoc->m_ModelInstance;
	if(pDoc->m_bInitTexture == FALSE)
	{
		pDoc->m_rtShadow.Init(SHADOWTEXTURESIZE, SHADOWTEXTURESIZE);
		pDoc->m_bInitTexture = TRUE;
	}	
	pDoc->m_rtShadow.Begin();
	pDoc->m_rtShadow.Clear(0xFFFFFFFF);
				
	// SKA 모델 렌더링.
	//RenderOneSkaModelToTexture(en, CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0)), plLight, FALSE, dm.dm_fMipFactor, dm.dm_ulFlags);
	RenderModelShadow(pDoc->m_ModelInstance, SHADOWTEXTURESIZE, plLight);
	pDoc->m_rtShadow.End();
}
//강동민 수정 끝

//강동민 수정 시작		03.05
/*
// NOTE : RenderModelShadow 및 CreateShadowMatrix, RenderOneSkaModelToTexture등에서 중복되는 코드가 많음.
void CSeriousSkaStudioView::RenderModelShadow(CModelInstance *pMI, CDrawPort *pdp, CViewPort* pViewPort, FLOAT fTextureSize, CPlacement3D& plLight)
{
	ASSERT(pMI != NULL && "유효하지 않은 ModelInstance입니다.");
	ASSERT(_pGfx->gl_eCurrentAPI == GAT_D3D && "D3D에서만 사용가능합니다.");

	FLOATaabbox3D FrameBBox;
	pMI->GetAllFramesBBox(FrameBBox);
	FLOAT3D vMin = FrameBBox.minvect;
	FLOAT3D vMax = FrameBBox.maxvect;
	FLOAT fHeight = fabs((vMax(2) - vMin(2))/2.0f);

	Matrix12 matTrans;
	MakeIdentityMatrix(matTrans);
	matTrans[3] = 0.0f;
	matTrans[7] = -fHeight;
	matTrans[11] = 0.0f;

	// 광원에서 World를 바라보도록 설정함.
	GetViewMatrix(plLight, pMI->mi_matWorldToLight);

	//-----------------------------------------------------------------------------
	// 물체의 월드행렬.
	FLOATmatrix3D m;
	Matrix12 matLocalToWorld;
	MakeRotationMatrixFast( m, pMI->GetOffsetRot());
	MatrixVectorToMatrix12(matLocalToWorld, m, pMI->GetOffsetPos());
	
	//-----------------------------------------------------------------------------	
	// 광원에서...
	// 투영행렬을 만들기 위한 계산을 수행함.
	SetAdjustedProjectionMatrix(pMI->mi_matShadowProj, D3DX_PI/4.0f, 1.0f, 0.05f, 500.0f, 
		0.0f, 0.0f, fTextureSize, fTextureSize);	
	
	  // 직교투영.
	//D3DXMatrixOrthoRH((D3DXMATRIX*)matProj, 0.98f/fXMax, 0.98f/fYMax, 1.0f, 500.0f);
	//D3DXMatrixOrthoOffCenterRH((D3DXMATRIX*)matProj, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 500.0f);
	//matProj[0] = 0.12f/fXMax;
	//matProj[5] = 0.12f/fYMax;

	Matrix12 matTemp;
	MatrixMultiply(matTemp, pMI->mi_matWorldToLight, matTrans);	
	FLOAT fXMax, fYMax;
	FindBestFOV(pMI, matTemp, fXMax, fYMax);

	pMI->mi_matShadowProj[0] = 0.98f/fXMax;
	pMI->mi_matShadowProj[5] = 0.98f/fYMax;
	
	HRESULT hr = _pGfx->gl_pd3dDevice->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&pMI->mi_matShadowProj);

	//-----------------------------------------------------------------------------	
	RM_SetRenderingType(1);	
	
	// 모델을 렌더링할 때 사용할 VIEW 행렬로 지정합니다.
	// World -> View
	RM_SetAbsToView(pMI->mi_matWorldToLight);
	
	// 모델의 위치 설정.
	CPlacement3D pl;
	pl.pl_OrientationAngle = FLOAT3D(0.0f, 0.0f, 0.0f);
	pl.pl_PositionVector   = FLOAT3D(0.0f, -fHeight, 0.0f);
	
	// 오브젝트의 월드 행렬 설정.
	// Local -> World
	RM_SetObjectPlacement(pl);
	
	//-----------------------------------------------------------------------------
	// 모델을 그립니다.
	// Calc mesh and skeleton distance
	FLOAT fMeshDistance = 0.0f;	
	fMeshDistance = -1;
	
	// LOD factor management
	RM_SetCurrentDistance(fMeshDistance);
	
	RM_AddFlag(RMF_SHOWSHADOW);
	// 내부에서 월드 행렬 설정함...
	// 뷰도 설정함...
	RM_RenderSKA(*pMI);	
	RM_RemoveFlag(RMF_SHOWSHADOW);
	RM_SetRenderingType(0);
}
*/
//강동민 수정 끝		03.05
//강동민 수정 시작
// NOTE : RenderModelShadow 및 CreateShadowMatrix, RenderOneSkaModelToTexture등에서 중복되는 코드가 많음.
void CSeriousSkaStudioView::RenderModelShadow(CModelInstance *pMI, FLOAT fTextureSize, const CPlacement3D& plLight)
{	
	ASSERT(pMI != NULL && "유효하지 않은 ModelInstance입니다.");
	ASSERT(_pGfx->gl_eCurrentAPI == GAT_D3D && "D3D에서만 사용가능합니다.");	
	CSeriousSkaStudioDoc* pDoc = GetDocument();

	FLOATaabbox3D FrameBBox;
	pMI->GetAllFramesBBox(FrameBBox);
	FLOAT3D vMin = FrameBBox.minvect;
	FLOAT3D vMax = FrameBBox.maxvect;
	FLOAT fHeight = fabs((vMax(2) - vMin(2))/2.0f);

	Matrix12 matTrans;
	MakeIdentityMatrix(matTrans);
	matTrans[3] = 0.0f;
	matTrans[7] = -fHeight;
	matTrans[11] = 0.0f;

	// 광원에서 World를 바라보도록 설정함.
	GetViewMatrix(plLight, pDoc->m_matWorldToLight);
	
	//-----------------------------------------------------------------------------	
	// 광원에서...
	// 투영행렬을 만들기 위한 계산을 수행함.
	SetAdjustedProjectionMatrix(pDoc->m_matShadowProj, D3DX_PI/4.0f, 1.0f, 0.05f, 500.0f, 
		0.0f, 0.0f, fTextureSize, fTextureSize);
	/*
	// 직교투영.
	//D3DXMatrixOrthoRH((D3DXMATRIX*)matProj, 0.98f/fXMax, 0.98f/fYMax, 1.0f, 500.0f);
	D3DXMatrixOrthoOffCenterRH((D3DXMATRIX*)matProj, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 500.0f);
	matProj[0] = 0.12f/fXMax;
	matProj[5] = 0.12f/fYMax;
	*/

	Matrix12 matTemp;
	MatrixMultiply(matTemp, pDoc->m_matWorldToLight, matTrans);	
	FLOAT fXMax, fYMax;
	FindBestFOV(FrameBBox, matTemp, fXMax, fYMax);

	fXMax = Max(fXMax, fYMax);
	fYMax = fXMax;

	pDoc->m_matShadowProj[0] = 0.98f/fXMax;
	pDoc->m_matShadowProj[5] = 0.98f/fYMax;
	
	HRESULT hr = _pGfx->gl_pd3dDevice->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&pDoc->m_matShadowProj);

	//-----------------------------------------------------------------------------	
	RM_SetRenderingType(1);	
	
	// 모델을 렌더링할 때 사용할 VIEW 행렬로 지정합니다.
	// World -> View
	RM_SetAbsToView(pDoc->m_matWorldToLight);
	
	// 오브젝트의 월드 행렬 설정.
	// Local -> World	
	//RM_SetObjectPlacement(CPlacement3D(FLOAT3D(0.0f,-fHeight,0.0f), plModel.pl_OrientationAngle));
	RM_SetObjectPlacement(CPlacement3D(FLOAT3D(0.0f,-fHeight,0.0f), FLOAT3D(0.0f, 0.0f, 0.0f)));
	
	//-----------------------------------------------------------------------------
	// 모델을 그립니다.
	// Calc mesh and skeleton distance
	FLOAT fMeshDistance = 0.0f;	
	fMeshDistance = -1;
	
	// LOD factor management
	RM_SetCurrentDistance(fMeshDistance);	

	// 02.23 수정.
	D3DVIEWPORT8 vpOld;
	D3DVIEWPORT8 vpNew;
	_pGfx->gl_pd3dDevice->GetViewport(&vpOld);
	vpNew = vpOld;
	vpNew.X = 1;
	vpNew.Y = 1;
	vpNew.Width = fTextureSize-2.0f;
	vpNew.Height = fTextureSize-2.0f;
	_pGfx->gl_pd3dDevice->SetViewport(&vpNew);
	
	RM_AddFlag(RMF_SHOWSHADOW);
	// 내부에서 월드 행렬 설정함...
	// 뷰도 설정함...
	RM_RenderSKA(*pMI);	
	RM_RemoveFlag(RMF_SHOWSHADOW);
	RM_SetRenderingType(0);

	// 02.23 수정.
	_pGfx->gl_pd3dDevice->SetViewport(&vpOld);
}
//강동민 수정 끝


void CSeriousSkaStudioView::OnDraw(CDC* pDC)
{
	CSeriousSkaStudioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// theApp.EnableConsole();
	// if there is a valid drawport, and the drawport can be locked
	if( m_pdpDrawPort!=NULL ) 
	{
	/*
	if(_pGfx->gl_ulFlags & GLF_INITONNEXTWINDOW) {
	theApp.DisableConsole();
	}
		*/
		//강동민 수정 시작
		//GenerateShadowTexture();
		//강동민 수정 끝
		m_pdpDrawPort->SetAsCurrent();
		
		// render view
		RenderView( m_pdpDrawPort);
		
		// swap if there is a valid viewport
		if( m_pvpViewPort!=NULL)
		{
			m_pvpViewPort->SwapBuffers();
		}
	}
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	_pSound->UpdateSounds();
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
}

void CSeriousSkaStudioView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	// get mainfrm
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	// get mdi client
	CMDIClientWnd *pMDIClient = &pMainFrame->m_wndMDIClient;
	// get mdi client rect
	CRect rc;
	pMDIClient->GetWindowRect(&rc);
	INDEX iWidth = rc.right - rc.left - 4;
	INDEX iHeight = rc.bottom - rc.top - 4;
	// check if view is maximized
	if((iWidth == cx) && (iHeight == cy))
	{
		m_iViewSize = SIZE_MAXIMIZED;
		theApp.bChildrenMaximized = TRUE;
	}
	else 
	{
		m_iViewSize = 0;
		theApp.bChildrenMaximized = FALSE;
	}


	// if we are not in game mode and changing of display mode is not on
	if( m_pvpViewPort!=NULL)
	{ // resize it
		m_pvpViewPort->Resize();
	}
}

void CSeriousSkaStudioView::OnDestroy() 
{
	// destroy canvas that is currently used
	ViewExitInstance();

	CView::OnDestroy();
}

void CSeriousSkaStudioView::ViewExitInstance()
{
	// destroy canvas that is currently used
	_pGfx->DestroyWindowCanvas( m_pvpViewPort);
	m_pvpViewPort = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CSeriousSkaStudioView diagnostics

#ifdef _DEBUG
void CSeriousSkaStudioView::AssertValid() const
{
	CView::AssertValid();
}

void CSeriousSkaStudioView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSeriousSkaStudioDoc* CSeriousSkaStudioView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSeriousSkaStudioDoc)));
	return (CSeriousSkaStudioDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSeriousSkaStudioView message handlers

void CSeriousSkaStudioView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	CSeriousSkaStudioDoc* pDoc = GetDocument();
	// at this time, m_hWnd is valid, so we do canvas initialization here
 	_pGfx->CreateWindowCanvas(m_hWnd, &m_pvpViewPort, &m_pdpDrawPort);
}

void CSeriousSkaStudioView::OnMouseMove(UINT nFlags, CPoint point) 
{
	BOOL bSpace = (GetKeyState(' ') & 128) != 0;  
	BOOL bLMB  = nFlags & MK_LBUTTON;
	BOOL bRMB  = nFlags & MK_RBUTTON;
	BOOL bCtrl = nFlags & MK_CONTROL;
	BOOL bShift = nFlags & MK_SHIFT;


	CPoint pntDelta = point - m_pntLastMouse;  
	m_pntLastMouse = point;
	FLOAT fDistance = -m_fTargetDistance;

	CPlacement3D plViewer;
	plViewer.pl_PositionVector = m_vTarget;
	plViewer.pl_OrientationAngle = m_angViewerOrientation;
	// moving offsets need small amounts
	FLOAT dx = 0.001f * pntDelta.x * fDistance;
	FLOAT dy = 0.001f * pntDelta.y * fDistance;
	FLOAT dz =  0.01f * pntDelta.y * fDistance;
	// angles need lot for rotation
	ANGLE dAngleX = AngleDeg( -0.5f * pntDelta.x);
	ANGLE dAngleY = AngleDeg( -0.5f * pntDelta.y);
 
	if (bSpace && bCtrl) {
	// if only space
	} else if (bSpace)
	{
		if (bRMB && bLMB)
		{
			m_angViewerOrientation(1)+=dAngleX;
			m_angViewerOrientation(2)+=dAngleY;
			// if only left button
		} else if (bLMB) {
			CPlacement3D plTarget;
			plTarget.pl_PositionVector = m_vTarget;
			plTarget.pl_OrientationAngle = m_angViewerOrientation;
			
			// project the placement to the viewer's system
			plTarget.AbsoluteToRelative( plViewer);
			// translate it
			plTarget.Translate_AbsoluteSystem( FLOAT3D( dx, -dy, 0.0f));
			// project the placement back from viewer's system
			plTarget.RelativeToAbsolute( plViewer);
			m_vTarget = plTarget.pl_PositionVector;
			// if only right button
		} else if (bRMB) {
			// move target away
			m_fTargetDistance += dz;

			// now apply left/right movement
			CPlacement3D plTarget;
			plTarget.pl_PositionVector = m_vTarget;
			plTarget.pl_OrientationAngle = m_angViewerOrientation;
			
			// project the placement to the viewer's system
			plTarget.AbsoluteToRelative( plViewer);
			// translate it
			plTarget.Translate_AbsoluteSystem( FLOAT3D( dx, 0.0f, 0.0f));
			// project the placement back from viewer's system
			plTarget.RelativeToAbsolute( plViewer);
			m_vTarget = plTarget.pl_PositionVector;
			// if no buttons are held
		} else {
			
		}
	}
	// if only ctrl
	else if (bCtrl && (bRMB || bLMB))
	{
		CSeriousSkaStudioDoc* pDoc = GetDocument();
		CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
		HTREEITEM hSelectedItem = m_TreeCtrl.GetSelectedItem();
		if(hSelectedItem==NULL) {
			return;
		}
		NodeInfo &niSelected = theApp.m_dlgBarTreeView.GetNodeInfo(hSelectedItem);
		CModelInstance *pmi = NULL;
		// if messuring tool is activated
		if(pDoc->m_bMeasuringToolOn) {
			// set min and max of colision box
			CPlacement3D plMeasurePoint;
			plMeasurePoint.pl_PositionVector = pDoc->m_vMeasurePointPosition;
			plMeasurePoint.pl_OrientationAngle = ANGLE3D(0,0,0);
			plMeasurePoint.AbsoluteToRelative(plViewer);

			if(bLMB) {
				plMeasurePoint.pl_PositionVector +=  FLOAT3D( -dx, dy, 0.0f);
				plMeasurePoint.pl_PositionVector +=  FLOAT3D( -dx, dy, 0.0f);
			} else if(bRMB) {
				plMeasurePoint.pl_PositionVector +=  FLOAT3D( 0.0f, 0.0f, -dy);
			}

			plMeasurePoint.RelativeToAbsolute(plViewer);
			pDoc->m_vMeasurePointPosition = plMeasurePoint.pl_PositionVector;

		// is selected item model instance
		} else if(niSelected.ni_iType == NT_MODELINSTANCE){
			// get pointer to model instance
			pmi = (CModelInstance*)niSelected.ni_pPtr;

			// set default bone placement
			QVect qvBonePlacement;
			memset(&qvBonePlacement,0,sizeof(QVect));
			qvBonePlacement.qRot.q_w = 1;
			// find parent bone of selected model instance
			RenBone rb;
			if(RM_GetRenBoneAbs(*pDoc->m_ModelInstance,pmi->mi_iParentBoneID,rb)) {
				// get bone qvect
				Matrix12ToQVect(qvBonePlacement,rb.rb_mBonePlacement);
			}
			CPlacement3D plBone,plOffset;
			FLOATmatrix3D matBone,matOffset;

			// set bone placement
			qvBonePlacement.qRot.ToMatrix(matBone);
			plBone.pl_PositionVector = qvBonePlacement.vPos; 
			DecomposeRotationMatrix(plBone.pl_OrientationAngle,matBone);
		
			// set offset
			pmi->mi_qvOffset.qRot.ToMatrix(matOffset);
			plOffset.pl_PositionVector = pmi->mi_qvOffset.vPos;
			DecomposeRotationMatrix(plOffset.pl_OrientationAngle,matOffset);

			plOffset.RelativeToRelative(plBone,plViewer);

			if(bLMB && bRMB) {
				plOffset.Rotate_TrackBall( ANGLE3D( -dAngleX/2, -dAngleY/2, 0));
			} else if(bLMB) {
				plOffset.pl_PositionVector +=  FLOAT3D( -dx, dy, 0.0f);
			} else if(bRMB) {
				plOffset.pl_PositionVector +=  FLOAT3D( 0.0f, 0.0f, -dy);
			}
			plOffset.RelativeToRelative(plViewer,plBone);

			pmi->mi_qvOffset.vPos=plOffset.pl_PositionVector;
			pmi->mi_qvOffset.qRot.FromEuler(plOffset.pl_OrientationAngle);

			_bSelectedItemChanged = TRUE;
			pDoc->MarkAsChanged();
		// is selected item colision box
		} else if (niSelected.ni_iType == NT_COLISIONBOX) {
			// get pointer to parent model instance
			pmi = (CModelInstance*)niSelected.pmi;

			ColisionBox &cb = pmi->GetColisionBox(pmi->mi_iCurentBBox);

			CPlacement3D plCBMin;
			CPlacement3D plCBMax;
			FLOATmatrix3D matOffset;

			// set min and max of colision box
			plCBMin.pl_PositionVector = cb.Min();
			plCBMin.pl_OrientationAngle = ANGLE3D(0,0,0);
			plCBMin.AbsoluteToRelative(plViewer);

			plCBMax.pl_PositionVector = cb.Max();
			plCBMax.pl_OrientationAngle = ANGLE3D(0,0,0);
			plCBMax.AbsoluteToRelative(plViewer);
			// if shift is pressed
			if(bShift) {
				if(bLMB) {
					plCBMin.pl_PositionVector +=  FLOAT3D( -dx, dy, 0.0f);
				} else if(bRMB) {
					plCBMax.pl_PositionVector +=  FLOAT3D( -dx, dy, 0.0f);
				}
			// if not
			} else {
				if(bLMB) {
					plCBMin.pl_PositionVector +=  FLOAT3D( -dx, dy, 0.0f);
					plCBMax.pl_PositionVector +=  FLOAT3D( -dx, dy, 0.0f);
				} else if(bRMB) {
					plCBMin.pl_PositionVector +=  FLOAT3D( 0.0f, 0.0f, -dy);
				}
			}

			plCBMin.RelativeToAbsolute(plViewer);
			plCBMax.RelativeToAbsolute(plViewer);
//안태훈 수정 시작	//(Easy Use Ska Studio)(0.1)
			if(plCBMin.pl_PositionVector(1) > plCBMax.pl_PositionVector(1))
				Swap(plCBMin.pl_PositionVector(1), plCBMax.pl_PositionVector(1));
			if(plCBMin.pl_PositionVector(2) > plCBMax.pl_PositionVector(2))
				Swap(plCBMin.pl_PositionVector(2), plCBMax.pl_PositionVector(2));
			if(plCBMin.pl_PositionVector(3) > plCBMax.pl_PositionVector(3))
				Swap(plCBMin.pl_PositionVector(3), plCBMax.pl_PositionVector(3));
//안태훈 수정 끝	//(Easy Use Ska Studio)(0.1)
			cb.SetMin(plCBMin.pl_PositionVector);
			cb.SetMax(plCBMax.pl_PositionVector);

			_bSelectedItemChanged = TRUE;
			pDoc->MarkAsChanged();
		}
	// if only shift is presed
	} else if(bShift) {
		if(bLMB) {
			// project the placement to the viewer's system
			m_plLightPlacement.AbsoluteToRelative( plViewer);
			// rotate it
			m_plLightPlacement.Rotate_TrackBall( ANGLE3D( -dAngleX/2, -dAngleY/2, 0));
			// project the placement back from viewer's system
			m_plLightPlacement.RelativeToAbsolute( plViewer);
		} else if(bRMB) {
			FLOAT fNewDistance = m_fLightDistance - dz;
			if( fNewDistance > 0.2f) {
				m_fLightDistance = fNewDistance;
			}
		}
	}


	// SetActiveWindow();
	SetFocus();
	/*
	GetParentFrame()->SetActiveView( this);
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	pMainFrame->MDIActivate(GetParentFrame());
	*/
/*
	HWND hwndUnderMouse = ::WindowFromPoint( point);
	HWND hwndInfo = NULL;
	if( pMainFrame->m_pInfoFrame != NULL)
		hwndInfo = pMainFrame->m_pInfoFrame->m_hWnd;

	if( (m_hWnd != ::GetActiveWindow()) && ( hwndInfo != hwndUnderMouse) )
	{
		SetActiveWindow();
		SetFocus();
		GetParentFrame()->SetActiveView( this);
		pMainFrame->MDIActivate(GetParentFrame());
	}
*/  
CView::OnMouseMove(nFlags, point);
}


void CSeriousSkaStudioView::FastZoomIn()
{
	if( m_fTargetDistance > 1.0f) {
		m_fTargetDistance /= 2.0f;
	}
}

void CSeriousSkaStudioView::FastZoomOut()
{
	if( m_fTargetDistance < 65535.0f) {
		m_fTargetDistance *= 2.0f;
	}
}

void CSeriousSkaStudioView::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bSpace = (GetKeyState( ' ') & 128) != 0;  
	BOOL bCtrl = nFlags & MK_CONTROL;
	BOOL bShift = nFlags & MK_SHIFT;
	// ctrl+space+XMB is used for 2x zooming
	if( bCtrl && bSpace) {
		FastZoomIn();
		return;
	}

	if(bCtrl||bSpace||bShift) {}
	else {
		INDEX iHitBone = TestRayCastHit(point);
		theApp.iSelectedItemID = iHitBone;
	}

	m_pntLastMouse = point;
	CView::OnLButtonDown(nFlags, point);
}

void CSeriousSkaStudioView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	BOOL bSpace = (GetKeyState( ' ') & 128) != 0;  
	BOOL bCtrl = nFlags & MK_CONTROL;
	// ctrl+space+XMB is used for 2x zooming
	if( bCtrl && bSpace) {
		FastZoomIn();
	}
	CView::OnLButtonDblClk(nFlags, point);
}

void CSeriousSkaStudioView::OnRButtonDown(UINT nFlags, CPoint point) {
	BOOL bSpace = (GetKeyState( ' ') & 128) != 0;  
	BOOL bCtrl = nFlags & MK_CONTROL;
	// ctrl+space+XMB is used for 2x zooming
	if( bCtrl && bSpace) {
		FastZoomOut();
		return;
	}
	CView::OnRButtonDown(nFlags, point);
}
void CSeriousSkaStudioView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	BOOL bSpace = (GetKeyState( ' ') & 128) != 0;  
	BOOL bCtrl = nFlags & MK_CONTROL;
	// ctrl+space+XMB is used for 2x zooming
	if( bCtrl && bSpace) {
		FastZoomOut();
	}
	CView::OnRButtonDblClk(nFlags, point);
}


void CSeriousSkaStudioView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
	HTREEITEM hSelectedItem = m_TreeCtrl.GetSelectedItem();
	if(hSelectedItem!=NULL && _bSelectedItemChanged) {
		theApp.m_dlgBarTreeView.SelItemChanged(hSelectedItem);
		_bSelectedItemChanged = FALSE;
	}
	CView::OnLButtonUp(nFlags, point);
}
void CSeriousSkaStudioView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
	HTREEITEM hSelectedItem = m_TreeCtrl.GetSelectedItem();
	if(hSelectedItem!=NULL && _bSelectedItemChanged) {
		theApp.m_dlgBarTreeView.SelItemChanged(hSelectedItem);
		_bSelectedItemChanged = FALSE;
	}
	CView::OnRButtonUp(nFlags, point);
}

void CSeriousSkaStudioView::OnResetView() 
{
	m_angViewerOrientation = ANGLE3D(180.0f, -20.0f, 0.0f);
	m_vTarget = FLOAT3D(0.0f, 1.0f, 0.0f);
	m_fTargetDistance = 5.0f;
}

// add CMesh to selected model instance
void CSeriousSkaStudioView::OnAddMeshlist() 
{
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	CTFileName fnSim;

	if(pmiSelected == NULL)
	{
		theApp.ErrorMessage("Model instance is not selected!");
		return;
	}

	CTFileName fnSelected = pmiSelected->mi_fnSourceFile;
	fnSelected = fnSelected.FileName();
	// get file name  
	fnSim = _EngineGUI.FileRequester( "Type name for new Mesh list or select existing",
		"ASCII model files (*.aml)\0*.aml\0"
		"All files (*.*)\0*.*\0\0",
		"Open directory", "Data\\Models\\",fnSelected);
	if (fnSim=="") return;
	// check if file allready exist
	if(FileExists(fnSim))
	{
		CTString strText = CTString(0,"File '%s' exist\n\nPress OK to load it, Cancel to overwrite it",fnSim);
		int iRet = AfxMessageBox((const char*)strText,MB_OKCANCEL);
		if(iRet == IDOK)
		{
			// convert and load animset
			if(theApp.ConvertMesh(fnSim))
			{
				fnSim = fnSim.NoExt() + ".bm";
				try
				{
					pmiSelected->AddMesh_t(fnSim);
					theApp.UpdateRootModelInstance();
					pDoc->MarkAsChanged();
					return;
				}
				catch(char *strError)
				{
					theApp.ErrorMessage("%s",strError);
					return;
				}
			}
			return;
		}
		// else owerwrite it bellow
	}
	// if file does not exist create new one
	CTFileStream ostrFile;
	try
	{
		// create new file
		ostrFile.Create_t(fnSim,CTStream::CM_TEXT);
		// write empty header in file
		ostrFile.FPrintF_t("MESHLODLIST\n{\n}\n");
		// close file
		ostrFile.Close();
		// convert it to binary (just to exist)
		if(theApp.ConvertMesh(fnSim))
		{
			fnSim = fnSim.NoExt() + ".bm";
			// add it to selected model instance
			try
			{
				pmiSelected->AddMesh_t(fnSim);
				theApp.UpdateRootModelInstance();
			}
			catch(char *strError)
			{
				theApp.ErrorMessage("%s",strError);
				return;
			}
		}
	}
	catch(char *strError)
	{
		theApp.ErrorMessage("%s",strError);
		return;
	}
	pDoc->MarkAsChanged();
	return;
}

// add CSkeleton to selected model instance
void CSeriousSkaStudioView::OnAddSkeletonlist() 
{
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	CTFileName fnSim;

	if(pmiSelected == NULL)
	{
		theApp.ErrorMessage("Model instance is not selected!");
		return;
	}

	CSkeleton *skl = pmiSelected->mi_psklSkeleton;
	if(skl != NULL)
	{
		theApp.ErrorMessage("Skeleton for this object exists. Only one skeleton per model is allowed!");
		return;
	}

	CTFileName fnSelected = pmiSelected->mi_fnSourceFile;
	fnSelected = fnSelected.FileName();
	// get file name  
	fnSim = _EngineGUI.FileRequester( "Type name for new Skeleton list or select existing",
		"ASCII model files (*.asl)\0*.asl\0"
		"All files (*.*)\0*.*\0\0",
		"Open directory", "Data\\Models\\", fnSelected);
	if (fnSim=="") return;
	// check if file allready exist
	if(FileExists(fnSim))
	{
		CTString strText = CTString(0,"File '%s' exist\n\nPress OK to load it, Cancel to overwrite it",fnSim);
		int iRet = AfxMessageBox((const char*)strText,MB_OKCANCEL);
		if(iRet == IDOK)
		{
			// convert and load animset
			if(theApp.ConvertSkeleton(fnSim))
			{
				fnSim = fnSim.NoExt() + ".bs";
				try
				{
					pmiSelected->AddSkeleton_t(fnSim);
					theApp.UpdateRootModelInstance();
					pDoc->MarkAsChanged();
				}
				catch(char *strError)
				{
					theApp.ErrorMessage("%s",strError);
					return;
				}
			}
			return;
		}
		// else owerwrite it bellow
	}
	// if file does not exist create new one
	CTFileStream ostrFile;
	try
	{
		// create new file
		ostrFile.Create_t(fnSim,CTStream::CM_TEXT);
		// write empty header in file
		ostrFile.FPrintF_t("SKELETONLODLIST\n{\n}\n");
		// close file
		ostrFile.Close();
		// convert it to binary (just to exist)
		if(theApp.ConvertSkeleton(fnSim))
		{
			fnSim = fnSim.NoExt() + ".bs";
			// add it to selected model instance
			pmiSelected->AddSkeleton_t(fnSim);
			// save smc
			theApp.UpdateRootModelInstance();
		}
	}
	catch(char *strError)
	{
		theApp.ErrorMessage("%s",strError);
	}
	pDoc->MarkAsChanged();
	return;
}

// add anim set to selected model instance
void CSeriousSkaStudioView::OnAddAnimset() 
{
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	CTFileName fnSim;

	if(pmiSelected == NULL)
	{
		theApp.ErrorMessage("Model instance is not selected!");
		return;
	}

	CTFileName fnSelected = pmiSelected->mi_fnSourceFile;
	fnSelected = fnSelected.FileName();
	// get file name  
	fnSim = _EngineGUI.FileRequester( "Type name for new Animset or select existing",
		"ASCII model files (*.aal)\0*.aal\0"
		"All files (*.*)\0*.*\0\0",
		"Open directory", "Data\\Models\\", fnSelected);
	if (fnSim=="") return;
	// check if file allready exist
	if(FileExists(fnSim))
	{
		CTString strText = CTString(0,"File '%s' exist\n\nPress OK to load it, Cancel to overwrite it",fnSim);
		int iRet = AfxMessageBox((const char*)strText,MB_OKCANCEL);
		if(iRet == IDOK)
		{
			// convert and load animset
			if(theApp.ConvertAnimSet(fnSim))
			{
				fnSim = fnSim.NoExt() + ".ba";
				try
				{
					pmiSelected->AddAnimSet_t(fnSim);
					theApp.UpdateRootModelInstance();
					pDoc->MarkAsChanged();
				}
				catch(char *strError)
				{
					theApp.ErrorMessage("%s",strError);
					return;
				}
			}
			return;
		}
		// else owerwrite it bellow
	}
	// if file does not exist create new one
	CTFileStream ostrFile;
	try
	{
		// create new file
		ostrFile.Create_t(fnSim,CTStream::CM_TEXT);
		// write empty header in file
		ostrFile.FPrintF_t("ANIMSETLIST\n{\n}\n");
		// close file
		ostrFile.Close();
		// convert it to binary (just to exist)
		if(theApp.ConvertAnimSet(fnSim))
		{
			fnSim = fnSim.NoExt() + ".ba";
			// add it to selected model instance
			pmiSelected->AddAnimSet_t(fnSim);
		}
	}
	catch(char *strError)
	{
		theApp.ErrorMessage("%s",strError);
	}

	theApp.UpdateRootModelInstance();
	pDoc->MarkAsChanged();
	return;
}

// add new child model instance to selected model instance
void CSeriousSkaStudioView::OnAddChildModelInstance() 
{
	if(pmiSelected == NULL)
	{
		theApp.ErrorMessage("Model instance is not selected!");
		return;
	}
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	if(pDoc->m_ModelInstance == NULL)
	{
		theApp.ErrorMessage("There is no root model instance");
		return;
	}
	
	CSkeleton *psklParent = pmiSelected->mi_psklSkeleton;
	SkeletonLOD *psklodParent = NULL;
	INDEX iParentBoneID = -1;
	// if parent has skeleton
	if(psklParent != NULL)
	{
		// if parent has skeleton lods
		INDEX ctslod = psklParent->skl_aSkeletonLODs.Count();
		if(ctslod > 0)
		{
			psklodParent = &psklParent->skl_aSkeletonLODs[0];
			INDEX ctsb = psklodParent->slod_aBones.Count();
			// if parent has any bones
			if(ctsb > 0)
			{
				SkeletonBone &sb = psklodParent->slod_aBones[0];
				iParentBoneID = sb.sb_iID;
			}
		}
	}
 
	CTFileName fnSim;
	fnSim = _EngineGUI.FileRequester( "Open ASCII intermediate files",
		"ASCII model files (*.smc)\0*.smc\0"
		"All files (*.*)\0*.*\0\0",
		"Open directory", "Data\\Models\\", "");
	if (fnSim=="") return;

	CTFileName fnFull;
	fnFull = _fnmApplicationPath + fnSim;
	CModelInstance *pcmi=NULL;
	try
	{
		pcmi = ParseSmcFile_t(fnFull);
	}
	catch(char *strError)
	{
		// error in parsing occured
		theApp.ErrorMessage("%s",strError);
		if(pcmi != NULL) pcmi->Clear();
		return;
	}

	CTString strText = CTString(0,"Add this model as reference or use own copy");
	int iRet = AfxMessageBox((const char*)strText,MB_YESNO);
	if(iRet == IDNO)
	{
		// assign same source file name as parent model instance so it will be saved in same file
		pcmi->mi_fnSourceFile = pmiSelected->mi_fnSourceFile;
	}
	pcmi->mi_iParentBoneID = iParentBoneID;

	/*
	// reset offset
	memset(&pcmi->mi_qvOffset,0,sizeof(QVect));
	pcmi->mi_qvOffset.qRot.q_w = 1;
	*/
	// if there is selected model instance ,add as child
	pmiSelected->mi_cmiChildren.Add(pcmi);
//안태훈 수정 시작	//(Remake Effect)(0.1)
	pmiSelected->m_tmSkaTagManager.AddChild(&pcmi->m_tmSkaTagManager);
//안태훈 수정 끝	//(Remake Effect)(0.1)
	theApp.UpdateRootModelInstance();
	pDoc->MarkAsChanged();
}

// add mesh lod to existing CMesh in selected model instance
void CSeriousSkaStudioView::OnAddMeshlod() 
{
	INDEX imlod = 0;
	CSeriousSkaStudioDoc *pDoc = GetDocument();

	if(pmiSelected == NULL) {
		theApp.ErrorMessage("Model instance is not selected!");
		return;
	}

	// if mesh list does not exist
	INDEX ctmshi = pmiSelected->mi_aMeshInst.Count();
	if(ctmshi < 1) {
		theApp.ErrorMessage("Model instance must have mesh lod list to add mesh lod to it");
		return;
	}
	// get pointer to selected item
	CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
	HTREEITEM hSelected = m_TreeCtrl.GetSelectedItem();
	INDEX iSelected = m_TreeCtrl.GetItemData(hSelected);
	NodeInfo &niSelected = theApp.aNodeInfo[iSelected];
	MeshInstance *pmshi;

	// get selected mesh instance
	if(niSelected.ni_iType == NT_MESHLODLIST) {
		// selected item is mesh instance so use its pointer as current mesh instance
		pmshi=(MeshInstance*)niSelected.ni_pPtr;
	} else if(niSelected.ni_iType == NT_MESHLOD) {
		// selected item is mesh lod, so get his parent and use his pointer as current mesh instance
		HTREEITEM hParent = m_TreeCtrl.GetParentItem(hSelected);
		INDEX iParent = m_TreeCtrl.GetItemData(hParent);
		NodeInfo &niParent = theApp.aNodeInfo[iParent];
		pmshi=(MeshInstance*)niParent.ni_pPtr;
	} else {
		// other item is selected, curent mesh instance is first mesh instance in model instance
		pmshi=&pmiSelected->mi_aMeshInst[0];
	}
	
	CMesh *pMesh = pmshi->mi_pMesh;

	CDynamicArray<CTFileName> afnMesh;
	_EngineGUI.FileRequester( "Open ASCII intermediate files",
		FILTER_MESH, "Open directory", "Data\\Models\\", "", &afnMesh);

	// return if no files selected
	if(afnMesh.Count()<=0) return;
	FLOAT fMaxDistance = 10;
	INDEX ctmlod = pMesh->msh_aMeshLODs.Count();
	// for each lod in mesh
	for( imlod = 0; imlod < ctmlod; imlod++) {
		// find current max distance in mesh
		if((fMaxDistance-5)<pMesh->msh_aMeshLODs[imlod].mlod_fMaxDistance) 
			fMaxDistance = pMesh->msh_aMeshLODs[imlod].mlod_fMaxDistance;
	}
	// create new mesh list that holds only new mesh lods
	CTString strMeshList;
	strMeshList = "MESHLODLIST\n{\n";

	FOREACHINDYNAMICARRAY( afnMesh, CTFileName, itMesh)
	{
		fMaxDistance+=5;
		CTString strLine;
		strLine.PrintF("  MAX_DISTANCE %g;\n  #INCLUDE \"%s\"\n",fMaxDistance,(const char*)itMesh.Current());
		// add this file to mesh list file
		strMeshList += strLine;
	}
	strMeshList += "}\n";

	CTFileName fnMeshList = (CTString)"Temp/mesh.aml";
	CTFileName fnMeshBin = (CTString)"Temp/mesh.bm";

	CMesh mshTemp;
	try
	{
		// save list file
		strMeshList.Save_t(fnMeshList);
		// convert animset list file
		if(!theApp.ConvertMesh(fnMeshList)) {
			RemoveFile(fnMeshList);
			return;
		}
		// load it
		mshTemp.Load_t(fnMeshBin);
	} catch(char *strErr) {
		theApp.ErrorMessage(strErr);
		mshTemp.Clear();
		RemoveFile(fnMeshBin);
		return;
	}
	RemoveFile(fnMeshList);
	RemoveFile(fnMeshBin);

	// count new mesh lods in temp mesh list
	INDEX ctNewMlods = mshTemp.msh_aMeshLODs.Count();
	// for each new mesh in temp mesh lod list
	for( imlod = 0; imlod < ctNewMlods; imlod++)
	{
		// add mesh lod to selected skeleton
		pMesh->AddMeshLod(mshTemp.msh_aMeshLODs[imlod]);
	}
	// clear temp mesh
	mshTemp.Clear();
	// update model instance
	theApp.UpdateRootModelInstance();
	pDoc->MarkAsChanged();
}

// add skeleton lod to existing CSkeleton in selected model instance
void CSeriousSkaStudioView::OnAddSkeletonlod() 
{
	CSeriousSkaStudioDoc *pDoc = GetDocument();

	if(pmiSelected == NULL)
	{
		theApp.ErrorMessage("Model instance is not selected!");
		return;
	}

	CSkeleton *pSkeleton = pmiSelected->mi_psklSkeleton;
	if(pSkeleton == NULL)
	{
		theApp.ErrorMessage("Model instance must have Skeleton list to add skeleton lod to it");
		return;
	}

	CDynamicArray<CTFileName> afnSkeleton;
	_EngineGUI.FileRequester( "Open ASCII intermediate files",
		FILTER_SKELETON,
		"Open directory", "Data\\Models\\", "", &afnSkeleton);

	// return if no files selected
	if(afnSkeleton.Count()<=0) return;

	FLOAT fMaxDistance=10;
	INDEX ctslod = pSkeleton->skl_aSkeletonLODs.Count();
	// for each lod in skeleton
	for(INDEX ilod=0;ilod<ctslod;ilod++)
	{
		// find current max distance in skeleton
		if((fMaxDistance-5)<pSkeleton->skl_aSkeletonLODs[ilod].slod_fMaxDistance) fMaxDistance = pSkeleton->skl_aSkeletonLODs[ilod].slod_fMaxDistance;
	}

	// create new skeleton list that holds only new skeleton lods
	CTString strSkeletonList;
	strSkeletonList = "SKELETONLODLIST\n{\n";

	FOREACHINDYNAMICARRAY( afnSkeleton, CTFileName, itSkeleton)
	{
		fMaxDistance+=5;
		CTString strLine;
		strLine.PrintF("  MAX_DISTANCE %g;\n  #INCLUDE \"%s\"\n",fMaxDistance,(const char*)itSkeleton.Current());
		// add this file to skeleton list file
		strSkeletonList += strLine;
	}
	strSkeletonList += "}\n";

	CTFileName fnSklList = (CTString)"Temp/skeleton.asl";
	CTFileName fnSklBin = (CTString)"Temp/skeleton.bs";
	CSkeleton slTemp;

	try
	{
		// save list file
		strSkeletonList.Save_t(fnSklList);
		// convert animset list file
		if(!theApp.ConvertSkeleton(fnSklList))
		{
			slTemp.Clear();
			RemoveFile(fnSklList);
			return;
		}
		// load it
		slTemp.Load_t(fnSklBin);
	}
	catch(char *strErr)
	{
		theApp.ErrorMessage(strErr);
		slTemp.Clear();
		RemoveFile(fnSklBin);
		return;
	}
	RemoveFile(fnSklList);
	RemoveFile(fnSklBin);

	// count new skleton lods in temp skeleton list
	INDEX ctNewSlods = slTemp.skl_aSkeletonLODs.Count();
	// for each new skeleton in temp skeleton lod list
	for(INDEX isl=0;isl<ctNewSlods;isl++)
	{
		// add skeleton lod to selected skeleton
		pSkeleton->AddSkletonLod(slTemp.skl_aSkeletonLODs[isl]);
	}
	// clear temp skeleton
	slTemp.Clear();
	// update model instance
	theApp.UpdateRootModelInstance();
	pDoc->MarkAsChanged();
}
// add animation to existing Anim set in selected model instance
void CSeriousSkaStudioView::OnAddAnimation() 
{
	// check for selected model instance
	if(pmiSelected == NULL) {
		theApp.ErrorMessage("Model instance is not selected!");
		return;
	}
	// get animset count
	INDEX ctas = pmiSelected->mi_aAnimSet.Count();
	if(ctas < 1) {
		theApp.ErrorMessage("Model instance must have Anim set to add animations to it");
		return;
	}
	// get document
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	CDynamicArray<CTFileName> afnAnimation;
	_EngineGUI.FileRequester( "Open ASCII intermediate files",
		FILTER_ANIMATION,
		"Open directory", "Data\\Models\\", "", &afnAnimation);

	// return if no files selected
	if(afnAnimation.Count()<=0) return;

	// get pointer to seleceted animset
	CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
	HTREEITEM hSelected = m_TreeCtrl.GetSelectedItem();
	INDEX iSelected = m_TreeCtrl.GetItemData(hSelected);
	NodeInfo &niSelected = theApp.aNodeInfo[iSelected];
	CAnimSet *pas=NULL;
	if(niSelected.ni_iType == NT_ANIMSET) {
		// selected item is animset
		pas = (CAnimSet*)niSelected.ni_pPtr;
	} else if(niSelected.ni_iType == NT_ANIMATION) {
		// selected item is animation, get its animset
		HTREEITEM hParent = m_TreeCtrl.GetParentItem(hSelected);
		INDEX iParent = m_TreeCtrl.GetItemData(hParent);
		NodeInfo &niParent = theApp.aNodeInfo[iParent];
		pas = (CAnimSet*)niParent.ni_pPtr;
	} else {
		// something else is selected, use first animset in model instance
		pas = &pmiSelected->mi_aAnimSet[0];
	}
	
	// create new animset that holds only new animations
	CTString strAnimSet;
	strAnimSet = "ANIMSETLIST\n{\n";
	// for each selected file
	FOREACHINDYNAMICARRAY( afnAnimation, CTFileName, itAnimation)
	{
		// add this file to animset list file
		CTString strLine;
		strLine.PrintF("  TRESHOLD 0;\n  COMPRESION FALSE;\n  #INCLUDE \"%s\"\n",(const char*)itAnimation.Current());
		strAnimSet += strLine;
	}
	strAnimSet += "}\n";

	CTFileName fnAnimSetList = (CTString)"Temp/animset.aal";
	CTFileName fnAnimSetBin = (CTString)"Temp/animset.ba";
	CAnimSet asTemp;

	try
	{
		// save list file
		strAnimSet.Save_t(fnAnimSetList);
		// convert animset list file
		if(!theApp.ConvertAnimSet(fnAnimSetList))
		{
			RemoveFile(fnAnimSetList);
			return;
		}
		// load it
		asTemp.Load_t(fnAnimSetBin);
	}
	catch(char *strErr)
	{
		theApp.ErrorMessage(strErr);
		asTemp.Clear();
		RemoveFile(fnAnimSetBin);
		return;
	}
	RemoveFile(fnAnimSetList);
	RemoveFile(fnAnimSetBin);

	// count new animations in temp animset
	INDEX ctan = asTemp.as_Anims.Count();
	// for each new animation in temp animset
	for(INDEX ian=0;ian<ctan;ian++)
	{
		Animation &anTemp = asTemp.as_Anims[ian];
		// add animation to selected animset
		pas->AddAnimation(&anTemp);
	}
	// clear temp animset
	asTemp.Clear();
	// update model instance
	theApp.UpdateRootModelInstance();
	pDoc->MarkAsChanged();
}

// add texture to existing mesh instance
void CSeriousSkaStudioView::AddTexture(CTFileName &fnFull)
{
	if(pmiSelected == NULL) {
		theApp.ErrorMessage("Model instance is not selected!");
		return;
	}

	INDEX ctmshi = pmiSelected->mi_aMeshInst.Count();
	if(ctmshi < 1) {
		theApp.ErrorMessage("Model instance must have mesh lod list to add texture to it");
		return;
	}

	// get selected item in tree view
	CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
	HTREEITEM hSelected = m_TreeCtrl.GetSelectedItem();
	INDEX iSelected = m_TreeCtrl.GetItemData(hSelected);
	NodeInfo &niSelected = theApp.aNodeInfo[iSelected];
	MeshInstance *pmshi;
	// get selected mesh instance
	if(niSelected.ni_iType == NT_MESHLODLIST) {
		// selected item is mesh instance so use its pointer as current mesh instance
		pmshi=(MeshInstance*)niSelected.ni_pPtr;
	} else if(niSelected.ni_iType == NT_MESHLOD) {
		// selected item is mesh lod, so get his parent and use his pointer as current mesh instance
		HTREEITEM hParent = m_TreeCtrl.GetParentItem(hSelected);
		INDEX iParent = m_TreeCtrl.GetItemData(hParent);
		NodeInfo &niParent = theApp.aNodeInfo[iParent];
		pmshi=(MeshInstance*)niParent.ni_pPtr;
	} else {
		// other item is selected, curent mesh instance is first mesh instance in model instance
		pmshi=&pmiSelected->mi_aMeshInst[0];
	}
	/*
	// check if file exists
	if(!FileExists(fnFull)) {
		theApp.ErrorMessage("Texture file '%s' does not exists",fnFull);
		continue;
	}
	*/
	// try adding texture to model instance
	try {
		pmiSelected->AddTexture_t(fnFull,fnFull.FileName(),pmshi);
	} catch(char *strError) {
		theApp.ErrorMessage(strError);
		//continue;
	}
}

void CSeriousSkaStudioView::BrowseTexture(CTString strTextureDir)
{
	CString strRegKeyName = "";
	if(strTextureDir=="Data\\Models\\") {
		strTextureDir = "Open directory";
	}
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	CDynamicArray<CTFileName> afnTexture;
	_EngineGUI.FileRequester( "Open texture files",
		FILTER_TEXTURE,
		(char*)(const char*)strTextureDir, strTextureDir, "", &afnTexture);

	// return if no files selected
	if(afnTexture.Count()<=0) return;
	// for each selected filename
	FOREACHINDYNAMICARRAY( afnTexture, CTFileName, itTexture) {
		CTFileName fnFull;
		fnFull = itTexture.Current();
		AddTexture(fnFull);
	}
	pDoc->MarkAsChanged();
	theApp.UpdateRootModelInstance();
}

// add texture to existing mesh instance
void CSeriousSkaStudioView::OnAddTexture() 
{
	BrowseTexture("Data\\Models\\");
}
void CSeriousSkaStudioView::OnAddTextureBump() 
{
	BrowseTexture("Data\\Models\\BumpTextures\\");
}

void CSeriousSkaStudioView::OnAddTextureReflection() 
{
	BrowseTexture("Data\\Models\\ReflectionTextures\\");
}

void CSeriousSkaStudioView::OnAddTextureSpecular() 
{
	BrowseTexture("Data\\Models\\SpecularTextures\\");
}

void CSeriousSkaStudioView::OnCreateAddTexture() 
{
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	CTFileName fn = (CTString)pMainFrame->CreateTexture();
	if(fn!="") {
		AddTexture(fn);
		pDoc->MarkAsChanged();
		theApp.UpdateRootModelInstance();
	}
}

void CSeriousSkaStudioView::OnAddColisionbox() 
{
	if(pmiSelected == NULL) return; 
	CSeriousSkaStudioDoc *pDoc = GetDocument();

	INDEX ctcb = pmiSelected->mi_cbAABox.Count();
	FLOAT3D vMin = FLOAT3D(-.5f,0,-.5f);
	FLOAT3D vMax = FLOAT3D(.5f,1,.5f);
	// CTString strName = CTString(0,"Default %d",ctcb);
	CTString strName = "Default";
	pmiSelected->AddColisionBox(strName,vMin,vMax);
	theApp.UpdateRootModelInstance();
	pDoc->MarkAsChanged();
}

// delete selected item in tree view control
void CSeriousSkaStudioView::OnDeleteselected() 
{
	// get tree ctrl
	CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
	// get current document
	CSeriousSkaStudioDoc *pDoc = GetDocument();
	// get selected item and its parent
	HTREEITEM hSelected = m_TreeCtrl.GetSelectedItem();
	if(hSelected == NULL) return;
	INDEX iSelIndex = m_TreeCtrl.GetItemData(hSelected);
	INDEX iParentIndex = -1;
	NodeInfo *pniParent = NULL;
	NodeInfo *pni = &theApp.aNodeInfo[iSelIndex];
	HTREEITEM hParent = m_TreeCtrl.GetParentItem(hSelected);
	if(hParent != NULL)
	{
		 iParentIndex = m_TreeCtrl.GetItemData(hParent);
		 pniParent = &theApp.aNodeInfo[iParentIndex];
	}
	// switch type of selected item
	switch(pni->ni_iType)
	{
		case NT_MODELINSTANCE:
		{
			// find parent of selected model instance
			CModelInstance *pmiParent = pmiSelected->GetParent(pDoc->m_ModelInstance);
			if(pmiParent != NULL) {
				// remove selected model instance form parent 
				pmiParent->DeleteChild(pmiSelected);
				// update root model instance
				theApp.UpdateRootModelInstance();
			} else {
				// root item is selected
				theApp.ErrorMessage("Can't delete root model instance");
				return;
			}
		}
		break;
		case NT_MESHLODLIST:
		{
			// get pointer to mesh instance
			MeshInstance *pmshi = (MeshInstance*)pni->ni_pPtr;
			// get pointer to mesh
			CMesh *pMesh = pmshi->mi_pMesh;
			// release mesh
			_pMeshStock->Release(pMesh);
			// count textures
			INDEX ctti=pmshi->mi_tiTextures.Count();
			// for each texture in selected mesh instance
			for(INDEX iti=0;iti<ctti;iti++) {
				// release texture from stock
				TextureInstance &ti = pmshi->mi_tiTextures[iti];
				ti.ti_toTexture.SetData(NULL);
				//CTextureData &td = *ti.ti_tdTexture;
				//_pTextureStock->Release(&td);
			}
			// count mesh instances
			INDEX ctmshi=pmiSelected->mi_aMeshInst.Count();
			INDEX itmpmshi=0;
			CStaticArray<struct MeshInstance> atmpMeshInst;
			// create array for mesh instances 
			atmpMeshInst.New(ctmshi-1);
			// for each mesh instance in selected model instance 
			for(INDEX imshi=0;imshi<ctmshi;imshi++) {
				MeshInstance *ptmpmshi = &pmiSelected->mi_aMeshInst[imshi];
				// if mesh instance is diferent then selected mesh instance
				if(ptmpmshi != pmshi) {
					// copy it to temp array
					atmpMeshInst[itmpmshi] = *ptmpmshi;
					itmpmshi++;
				}
			}
			// clear mesh instance
			pmiSelected->mi_aMeshInst.Clear();
			pmiSelected->mi_aMeshInst.CopyArray(atmpMeshInst);
			// update root model instance
			theApp.UpdateRootModelInstance();
		}
		break;
		case NT_MESHLOD:
		{
			ASSERT(pniParent!=NULL);
			MeshLOD *pmlodSelected = (MeshLOD*)pni->ni_pPtr;
			MeshInstance *pmshi = (MeshInstance*)pniParent->ni_pPtr;
			CMesh *pmesh = pmshi->mi_pMesh;
			pmesh->RemoveMeshLod(pmlodSelected);
			theApp.UpdateRootModelInstance();
		}
		break;
		case NT_TEXINSTANCE:
		{
			// get pointers to texture and mesh instances
			TextureInstance *pti = (TextureInstance*)pni->ni_pPtr;
			MeshInstance *pmshi = (MeshInstance*)pniParent->ni_pPtr;
			ASSERT(pti!=NULL);
			ASSERT(pmshi!=NULL);

			pmiSelected->RemoveTexture(pti,pmshi);
			// update root model instance
			theApp.UpdateRootModelInstance();
		}
		break;
		case NT_SKELETONLODLIST:
		{
			CSkeleton *skl = pmiSelected->mi_psklSkeleton;
			_pSkeletonStock->Release(skl);
			pmiSelected->mi_psklSkeleton = NULL;
			// update root model instance
			theApp.UpdateRootModelInstance();//!!!
		}
		break;
		case NT_SKELETONLOD:
		{
			// get pointers to skeleton and skeleton lod
			SkeletonLOD *pslodSelected = (SkeletonLOD*)pni->ni_pPtr;
			CSkeleton *pskl = (CSkeleton*)pniParent->ni_pPtr;
			pskl->RemoveSkeletonLod(pslodSelected);
			// update root model instance
			theApp.UpdateRootModelInstance();
		}
		break;
		case NT_BONE:
		{
		 return;
		}
		break;
		case NT_ANIM_BONEENV:
		{
			return;
		}
		break;
		case NT_MESHSURFACE:
		{
			return;
		}
		break;
		case NT_ANIMSET:
		{
			CAnimSet *pas = (CAnimSet*)pni->ni_pPtr;
			pmiSelected->mi_aAnimSet.Remove(pas);
			_pAnimSetStock->Release(pas);
			// update root model instance
			theApp.UpdateRootModelInstance();
		}
		break;
		case NT_ANIMATION:
		{
			// take pointer to anim set
			Animation *panSelected = (Animation*)pni->ni_pPtr;
			CAnimSet *pas = (CAnimSet*)pniParent->ni_pPtr;
			pas->RemoveAnimation(panSelected);
			// update model instance
			theApp.UpdateRootModelInstance();
		}
		break;
		case NT_COLISIONBOX:
		{
			ColisionBox *pcbSelected = (ColisionBox*)pni->ni_pPtr;
			// find colision box
			INDEX iIndex = -1;
			// count colision boxes
			INDEX ctcb = pmiSelected->mi_cbAABox.Count();
			// for each colision box in model instance
			for(INDEX icb=0;icb<ctcb;icb++) {
				ColisionBox *pcb2 = &pmiSelected->mi_cbAABox[icb];
				if(pcbSelected == pcb2) {
					// remember index of selected colision box
					iIndex = icb;
					break;
				}
			}
			// return if no index
			if(iIndex<0) return;
			// remove bounding box from mi
			pmiSelected->RemoveColisionBox(iIndex);
			// update root model instance
			theApp.UpdateRootModelInstance();
		}
		break;
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
		case NT_ANIMEFFECT_REG:
		{
			if(pniParent == NULL) break;
			CAnimEffect &ae = *(CAnimEffect*)pniParent->ni_pPtr;
			if(ae.DeleteReservedEG((INDEX)pni->ni_pPtr))
			{
				if(ae.GetReservedEGCount() == 0)
				{
					pmiSelected->DeleteAnimEffect(ae.GetAnimName());
				}
				theApp.UpdateRootModelInstance();
			}
		} break;
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
		case NT_RIDINGPOSITION:
		{
			CModelInstance* pmsi = pni->pmi;
			pmsi->mi_iRideParentBoneID = -1;
			pmsi->mi_qvRideOffset.vPos = FLOAT3D(0,0,0);
			pmsi->mi_qvRideOffset.qRot = FLOATquat3D(1,0,0,0);

			theApp.UpdateRootModelInstance();
		}
		break;
	}
	pDoc->MarkAsChanged();
}

// fill tree view with new selected document
void CSeriousSkaStudioView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if(bActivate) {
		// refresh tree view for curent view
		CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
		CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
		HTREEITEM hRoot = m_TreeCtrl.GetRootItem();

		if(pDoc == NULL) {
			theApp.m_dlgBarTreeView.UpdateModelInstInfo(NULL);
			CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
			return;
		}
		if(hRoot == NULL) {
			theApp.UpdateRootModelInstance();
			CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
			return;
		}
		
		INDEX iRoot = m_TreeCtrl.GetItemData(hRoot);
		NodeInfo &ni = theApp.aNodeInfo[iRoot];
		// update only ih root model changed
		if(ni.ni_pPtr != pDoc->m_ModelInstance) {
			theApp.UpdateRootModelInstance();
		}
		OnRefreshTagmanager();
	}
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void SyncModelInstance(CModelInstance *pmi,FLOAT fTime)
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();

	AnimQueue &anq = pmi->mi_aqAnims;
	INDEX ctal = anq.aq_Lists.Count();
	// for each anim list
	for(INDEX ial=0;ial<ctal;ial++)
	{
		AnimList &al = anq.aq_Lists[ial];
		al.al_fStartTime = fTime;
		
		// for each played anim
		INDEX ctpa=al.al_PlayedAnims.Count();
		for(INDEX ipa=0;ipa<ctpa;ipa++)
		{
			PlayedAnim &pa = al.al_PlayedAnims[ipa];
			pa.pa_fStartTime = fTime;

			if(pDoc->m_bAnimLoop) {
				pa.pa_ulFlags |= AN_LOOPING;
			} else {
				pa.pa_ulFlags &= ~AN_LOOPING;
			}
		}
	}

	INDEX ctmi = pmi->mi_cmiChildren.Count();
	// for each child
	for(INDEX imi=0;imi<ctmi;imi++)
	{
		CModelInstance &cmi = pmi->mi_cmiChildren[imi];
		SyncModelInstance(&cmi,fTime);
	}
}

void CSeriousSkaStudioView::OnAnimSync() 
{
	SyncModelInstance(theApp.GetDocument()->m_ModelInstance,_pTimer->CurrentTick());
}

void CSeriousSkaStudioView::OnAnimLoop() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_bAnimLoop=!pDoc->m_bAnimLoop;
	SyncModelInstance(theApp.GetDocument()->m_ModelInstance,_pTimer->CurrentTick());
}

void CSeriousSkaStudioView::OnUpdateAnimLoop(CCmdUI* pCmdUI) 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) pCmdUI->SetCheck(pDoc->m_bAnimLoop);
}

void CSeriousSkaStudioView::OnAnimPause() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_bViewPaused = !pDoc->m_bViewPaused;
	// if now unpaused
	if(!pDoc->m_bViewPaused) {
		// calculate time in pause
		pDoc->m_tvPauseTime += _pTimer->GetHighPrecisionTimer() - pDoc->m_tvPauseStart;
		pDoc->AddShowEvent("Paused",EV_APP_PAUSED,1.0f,0xffff0000);
	}
	pDoc->m_tvPauseStart = _pTimer->GetHighPrecisionTimer();
}

void CSeriousSkaStudioView::OnUpdateAnimPause(CCmdUI* pCmdUI) 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pCmdUI->SetCheck(pDoc->m_bViewPaused);
}

void CSeriousSkaStudioView::OnVkEscape() 
{
	/*
	// if error list is visible
	if(theApp.IsErrorDlgVisible()) {
		// hide it
		theApp.ShowErrorDlg(FALSE);
	// else
	} else {
		// stop all animations
		OnAnimStop();
	}
	*/
	
	// Date : 2005-11-08(오후 2:23:14), By Lee Ki-hwan	
	OnAnimStop();
}

void CSeriousSkaStudioView::OnAnimStop() 
{
	CModelInstance *pmi = theApp.GetDocument()->m_ModelInstance;
	pmi->StopAllAnimations(0);
//안태훈 수정 시작	//(Easy Use Ska Studio)(0.1)
	_pSound->Flush();
	pmi->StopAllAnimEffect(0);
	CEffectGroupManager::Instance().StopAll();
//안태훈 수정 끝	//(Easy Use Ska Studio)(0.1)
}

void CSeriousSkaStudioView::OnAutoMiping() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_bAutoMiping=!pDoc->m_bAutoMiping;
}
void CSeriousSkaStudioView::OnUpdateAutoMiping(CCmdUI* pCmdUI) 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) pCmdUI->SetCheck(pDoc->m_bAutoMiping);
}

void CSeriousSkaStudioView::OnShowGround() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_bShowGround=!pDoc->m_bShowGround;
}
void CSeriousSkaStudioView::OnUpdateShowGround(CCmdUI* pCmdUI) 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) pCmdUI->SetCheck(pDoc->m_bShowGround);
}

void CSeriousSkaStudioView::OnShowSkeleton() 
{
	if(RM_GetFlags() & RMF_SHOWSKELETON) RM_RemoveFlag(RMF_SHOWSKELETON);
	else RM_AddFlag(RMF_SHOWSKELETON);
}
void CSeriousSkaStudioView::OnUpdateShowSkeleton(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((RM_GetFlags() & RMF_SHOWSKELETON) ? 1:0);
}

void CSeriousSkaStudioView::OnShowActiveSkeleton() 
{
	if(RM_GetFlags() & RMF_SHOWACTIVEBONES) RM_RemoveFlag(RMF_SHOWACTIVEBONES);
	else RM_AddFlag(RMF_SHOWACTIVEBONES);
}

void CSeriousSkaStudioView::OnUpdateShowActiveSkeleton(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((RM_GetFlags() & RMF_SHOWACTIVEBONES) ? 1:0);
}

void CSeriousSkaStudioView::OnShowTexture() 
{
	if(RM_GetFlags() & RMF_SHOWTEXTURE) RM_RemoveFlag(RMF_SHOWTEXTURE);
	else RM_AddFlag(RMF_SHOWTEXTURE);
}

void CSeriousSkaStudioView::OnShowNormals() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) {
		pDoc->m_bShowNormals = !pDoc->m_bShowNormals;
	}
}

void CSeriousSkaStudioView::OnUpdateShowNormals(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((RM_GetFlags() & RMF_SHOWNORMALS) ? 1:0);
}

void CSeriousSkaStudioView::OnUpdateShowTexture(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((RM_GetFlags() & RMF_SHOWTEXTURE) ? 1:0);
}

void CSeriousSkaStudioView::OnShowWireframe() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) {
		pDoc->m_bShowWireframe = !pDoc->m_bShowWireframe;
	}
}

void CSeriousSkaStudioView::OnUpdateShowWireframe(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck((RM_GetFlags() & RMF_WIREFRAME) ? 1:0);
}


void CSeriousSkaStudioView::OnShowColision() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) {
		pDoc->m_bShowColisionBox = !pDoc->m_bShowColisionBox;
	}
}
void CSeriousSkaStudioView::OnUpdateShowColision(CCmdUI* pCmdUI) 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) pCmdUI->SetCheck(pDoc->m_bShowColisionBox);
}

void CSeriousSkaStudioView::OnShowAllFramesBbox() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) {
		pDoc->m_bShowAllFramesBBox = !pDoc->m_bShowAllFramesBBox;
	}
}
void CSeriousSkaStudioView::OnUpdateShowAllFramesBbox(CCmdUI* pCmdUI) 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) pCmdUI->SetCheck(pDoc->m_bShowAllFramesBBox);
}

void CSeriousSkaStudioView::OnShowAnimQueue() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) pDoc->m_bShowAnimQueue=!pDoc->m_bShowAnimQueue;
}
void CSeriousSkaStudioView::OnUpdateShowAnimQueue(CCmdUI* pCmdUI) 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) pCmdUI->SetCheck(pDoc->m_bShowAnimQueue);
}

void CSeriousSkaStudioView::OnFileSaveModel() 
{
	theApp.SaveRootModel();
}

void CSeriousSkaStudioView::OnFileSavemiAs() 
{
	theApp.SaveRootModelAs();
}

void CSeriousSkaStudioView::OnShowLights() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_bShowLights=!pDoc->m_bShowLights;
}

void CSeriousSkaStudioView::OnUpdateShowLights(CCmdUI* pCmdUI) 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) pCmdUI->SetCheck(pDoc->m_bShowLights);
}

void CSeriousSkaStudioView::OnChangeAmbientcolor() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();

	COLORREF acrCustClr[16];
	CHOOSECOLOR cc;
	acrCustClr[ 0] = CLRF_CLR(C_BLACK);
	acrCustClr[ 1] = CLRF_CLR(C_WHITE);
	acrCustClr[ 2] = CLRF_CLR(C_dGRAY);
	acrCustClr[ 3] = CLRF_CLR(C_GRAY);
	acrCustClr[ 4] = CLRF_CLR(C_lGRAY);
	acrCustClr[ 5] = CLRF_CLR(C_dRED); 
	acrCustClr[ 6] = CLRF_CLR(C_dGREEN);
	acrCustClr[ 7] = CLRF_CLR(C_dBLUE);
	acrCustClr[ 8] = CLRF_CLR(C_dCYAN);
	acrCustClr[ 9] = CLRF_CLR(C_dMAGENTA);
	acrCustClr[10] = CLRF_CLR(C_dYELLOW);
	acrCustClr[11] = CLRF_CLR(C_dORANGE);
	acrCustClr[12] = CLRF_CLR(C_dBROWN);
	acrCustClr[13] = CLRF_CLR(C_dPINK);
	acrCustClr[14] = CLRF_CLR(C_lORANGE);
	acrCustClr[15] = CLRF_CLR(C_lBROWN);

	pDoc->m_colAmbient &= 0xFFFFFF00;

	memset(&cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.rgbResult = ByteSwap(pDoc->m_colAmbient);
	cc.hwndOwner = GetSafeHwnd();
	cc.lpCustColors = (LPDWORD) acrCustClr;
	if(ChooseColor(&cc))
	{
		COLOR colAmbient = ByteSwap(cc.rgbResult);
		colAmbient |= 0xFF;
		pDoc->m_colAmbient  = colAmbient;
	}
}

void CSeriousSkaStudioView::OnChangeLightcolor() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();

	COLORREF acrCustClr[16];
	CHOOSECOLOR cc;
	acrCustClr[ 0] = CLRF_CLR(C_BLACK);
	acrCustClr[ 1] = CLRF_CLR(C_WHITE);
	acrCustClr[ 2] = CLRF_CLR(C_dGRAY);
	acrCustClr[ 3] = CLRF_CLR(C_GRAY);
	acrCustClr[ 4] = CLRF_CLR(C_lGRAY);
	acrCustClr[ 5] = CLRF_CLR(C_dRED); 
	acrCustClr[ 6] = CLRF_CLR(C_dGREEN);
	acrCustClr[ 7] = CLRF_CLR(C_dBLUE);
	acrCustClr[ 8] = CLRF_CLR(C_dCYAN);
	acrCustClr[ 9] = CLRF_CLR(C_dMAGENTA);
	acrCustClr[10] = CLRF_CLR(C_dYELLOW);
	acrCustClr[11] = CLRF_CLR(C_dORANGE);
	acrCustClr[12] = CLRF_CLR(C_dBROWN);
	acrCustClr[13] = CLRF_CLR(C_dPINK);
	acrCustClr[14] = CLRF_CLR(C_lORANGE);
	acrCustClr[15] = CLRF_CLR(C_lBROWN);

	pDoc->m_colLight &= 0xFFFFFF00;
	memset(&cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.rgbResult = ByteSwap(pDoc->m_colLight);
	cc.hwndOwner = GetSafeHwnd();
	cc.lpCustColors = (LPDWORD) acrCustClr;
	if(ChooseColor(&cc))
	{
		COLOR colDiffuse = ByteSwap(cc.rgbResult);
		colDiffuse |= 0xFF;
		pDoc->m_colLight = colDiffuse;
	}
}

void CSeriousSkaStudioView::OnMeasureTool() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_bMeasuringToolOn=!pDoc->m_bMeasuringToolOn;
	CTString strPositition;
	strPositition.PrintF("%g,%g,%g",pDoc->m_vMeasurePointPosition(1),pDoc->m_vMeasurePointPosition(2),pDoc->m_vMeasurePointPosition(3));
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,strPositition.Length()+1);
	char *pchMem = (char *)GlobalLock(hMem);
	lstrcpy(pchMem,&strPositition[0]);
	GlobalUnlock(hMem);

	ASSERT(_CrtCheckMemory());
	if(OpenClipboard()) {
		if(EmptyClipboard()) {
				if(SetClipboardData(CF_TEXT,hMem)==NULL) {
					ULONG ulErr = GetLastError();
					ASSERT(FALSE);
				}
			}
		if(!CloseClipboard()) {
			ASSERT(FALSE);
		}
	} else {
		GlobalFree(hMem);
	}
}

void CSeriousSkaStudioView::OnUpdateMeasureTool(CCmdUI* pCmdUI) 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) pCmdUI->SetCheck(pDoc->m_bMeasuringToolOn);
}


void CSeriousSkaStudioView::OnRotateViewer() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_bRotateView=!pDoc->m_bRotateView;
	_fLastViewTick = _pTimer->GetLerpedCurrentTick();
}

void CSeriousSkaStudioView::OnUpdateRotateViewer(CCmdUI* pCmdUI) 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) pCmdUI->SetCheck(pDoc->m_bRotateView);
}

void CSeriousSkaStudioView::OnRotateLight() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_bRotateLight=!pDoc->m_bRotateLight;
	_fLastLightTick = _pTimer->GetLerpedCurrentTick();
}

void CSeriousSkaStudioView::OnUpdateRotateLight(CCmdUI* pCmdUI) 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) pCmdUI->SetCheck(pDoc->m_bRotateLight);
}


void CSeriousSkaStudioView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
/*
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
		if(nChar==VK_LEFT) {
			if(!pDoc->m_bViewPaused) {
				OnAnimPause();
			} else {
				pDoc->m_tvPauseStart.tv_llValue -= 10000000;
			}
			return;
		}
		if(nChar==VK_RIGHT) {
			if(!pDoc->m_bViewPaused) {
				OnAnimPause();
			} else {
				pDoc->m_tvPauseStart.tv_llValue += 10000000;
			}
			return;
		}
*/
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}



void CSeriousSkaStudioView::OnFileRecreatetexture() 
{
	CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
	HTREEITEM hSelected = m_TreeCtrl.GetSelectedItem();
	// if selected item exists
	if(hSelected!=NULL) {
		NodeInfo &niSelected = theApp.m_dlgBarTreeView.GetNodeInfo(hSelected);
		// is selected item texture instance
		if(niSelected.ni_iType == NT_TEXINSTANCE) {
			// recreate texture
			theApp.m_dlgBarTreeView.m_dlgTexture.OnBtRecreateTexture();
		}
	}
}

void CSeriousSkaStudioView::OnUpdateFileRecreatetexture(CCmdUI* pCmdUI) 
{
	CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
	HTREEITEM hSelected = m_TreeCtrl.GetSelectedItem();
	// if selected item exists
	if(hSelected!=NULL) {
		NodeInfo &niSelected = theApp.m_dlgBarTreeView.GetNodeInfo(hSelected);
		// is selected item texture instance
		if(niSelected.ni_iType == NT_TEXINSTANCE) {
			// enable recreate texture menu item
			pCmdUI->Enable(TRUE);
		} else {
			// disable recreate texture menu item
			pCmdUI->Enable(FALSE);
		}
	}
}

void CSeriousSkaStudioView::OnVkDown() 
{
	CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
	HTREEITEM hSelected = m_TreeCtrl.GetSelectedItem();
	if(hSelected!=NULL) {
		HTREEITEM hNext = m_TreeCtrl.GetNextItem(hSelected,TVGN_NEXTVISIBLE);
		if(hNext!=NULL) {
			m_TreeCtrl.SelectItem(hNext);
		}
	}
}

void CSeriousSkaStudioView::OnVkUp() 
{
	CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
	HTREEITEM hSelected = m_TreeCtrl.GetSelectedItem();
	if(hSelected!=NULL) {
		HTREEITEM hNext = m_TreeCtrl.GetNextItem(hSelected,TVGN_PREVIOUSVISIBLE);
		if(hNext!=NULL) {
			m_TreeCtrl.SelectItem(hNext);
		}
	}
}

static void SelectLeftItemInTree()
{
	CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
	HTREEITEM hSelected = m_TreeCtrl.GetSelectedItem();
	if(hSelected!=NULL) {
		UINT uiMask = TVIF_STATE;
		UINT uiState = m_TreeCtrl.GetItemState(hSelected,uiMask);
		if(uiState&TVIS_EXPANDED) {
			m_TreeCtrl.Expand(hSelected,TVE_COLLAPSE);
		} else {
			HTREEITEM hParent = m_TreeCtrl.GetNextItem(hSelected,TVGN_PARENT);
			if(hParent!=NULL) {
				m_TreeCtrl.SelectItem(hParent);
			}
		}
	}
}
static void SelectRightItemInTree()
{
	CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
	HTREEITEM hSelected = m_TreeCtrl.GetSelectedItem();
	if(hSelected!=NULL) {
		UINT uiMask = TVIF_STATE;
		UINT uiState = m_TreeCtrl.GetItemState(hSelected,uiMask);
		if(uiState&TVIS_EXPANDED) {
			HTREEITEM hClild = m_TreeCtrl.GetNextItem(hSelected,TVGN_CHILD);
			if(hClild!=NULL) {
				m_TreeCtrl.SelectItem(hClild);
			}
		} else {
			m_TreeCtrl.Expand(hSelected,TVE_EXPAND);
		}
	}
}

static void NextFrame()
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_tvPauseStart.tv_llValue += 10000000;
}
static void PrevFrame()
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_tvPauseStart.tv_llValue -= 10000000;
}

void CSeriousSkaStudioView::OnVkLeft() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc->m_bViewPaused) {
		PrevFrame();
	} else {
		SelectLeftItemInTree();
	}
}

void CSeriousSkaStudioView::OnVkRight() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc->m_bViewPaused) {
		NextFrame();
	} else {
		SelectRightItemInTree();
	}
}

void CSeriousSkaStudioView::OnVkLeftWithCtrl() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc->m_bViewPaused) {
		SelectLeftItemInTree();
	} else {
		OnAnimPause();
		PrevFrame();
	}
}

void CSeriousSkaStudioView::OnVkRightWithCtrl() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc->m_bViewPaused) {
		SelectRightItemInTree();
	} else {
		OnAnimPause();
		NextFrame();
	}
}

/*
 *  Menus 
 */ 

void CSeriousSkaStudioView::OnModelinstanceSavewithoffset() 
{
	NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
	if(ni.ni_iType == NT_MODELINSTANCE) {
		CModelInstance *pmi = (CModelInstance*)ni.ni_pPtr;
		CModelInstance *pmiParent = pmi->GetParent(theApp.GetDocument()->m_ModelInstance);
	
		CString fnOldSmcFile;
		// if parent of model instance exists
		if(pmiParent!=NULL) {
			// remmeber current fn of model instance
			fnOldSmcFile = pmi->mi_fnSourceFile;
			// temporary remove it to save offset of model instance in same file
			pmiParent->RemoveChild(pmi);
		}

		// save model instance
		theApp.SaveModelAs(pmi);

		// if there was a parent for model instance
		if(pmiParent!=NULL) {
			// return model instance to his parent
			pmiParent->AddChild(pmi);
			// restore old filename
			pmi->mi_fnSourceFile = (CTString)fnOldSmcFile;
		}
	} else {
		ASSERT(FALSE); // This must be model instance
	}
}

void CSeriousSkaStudioView::OnConvertSelected() 
{
	CDlgClient *pDlgCurrent = (CDlgClient*)theApp.m_dlgBarTreeView.pdlgCurrent;
	if(pDlgCurrent == &theApp.m_dlgBarTreeView.m_dlgListOpt) {
		pDlgCurrent->OnBtConvert();
	} else {
		ASSERT(FALSE); // Current client dialog must be listopt dialog
	}
}

void CSeriousSkaStudioView::OnResetColisionbox() 
{
	CDlgClient *pDlgCurrent = (CDlgClient*)theApp.m_dlgBarTreeView.pdlgCurrent;
	if(pDlgCurrent == &theApp.m_dlgBarTreeView.m_dlgColision) {
		pDlgCurrent->OnBtResetColision();
	} else {
		ASSERT(FALSE); // Current client dialog must be colision dialog
	}
}

void CSeriousSkaStudioView::OnAllFramesRecalc() 
{
	CDlgClient *pDlgCurrent = (CDlgClient*)theApp.m_dlgBarTreeView.pdlgCurrent;
	if(pDlgCurrent == &theApp.m_dlgBarTreeView.m_dlgAllFrames) {
		pDlgCurrent->OnBtCalcAllframesBbox();
	} else {
		ASSERT(FALSE); // Current client dialog must be allframes dialog
	}
}

void CSeriousSkaStudioView::OnReloadTexture() 
{
	CDlgClient *pDlgCurrent = (CDlgClient*)theApp.m_dlgBarTreeView.pdlgCurrent;
	if(pDlgCurrent == &theApp.m_dlgBarTreeView.m_dlgTexture) {
		pDlgCurrent->OnBtReloadTexture();
	} else {
		ASSERT(FALSE); // Current client dialog must be texture dialog
	}
}

void CSeriousSkaStudioView::OnRecreateTexture() 
{
	CDlgClient *pDlgCurrent = (CDlgClient*)theApp.m_dlgBarTreeView.pdlgCurrent;
	if(pDlgCurrent == &theApp.m_dlgBarTreeView.m_dlgTexture) {
		pDlgCurrent->OnBtRecreateTexture();
	} else {
		ASSERT(FALSE); // Current client dialog must be texture dialog
	}
}

void CSeriousSkaStudioView::OnBrowseTexture() 
{
	CDlgClient *pDlgCurrent = (CDlgClient*)theApp.m_dlgBarTreeView.pdlgCurrent;
	if(pDlgCurrent == &theApp.m_dlgBarTreeView.m_dlgTexture) {
		pDlgCurrent->OnBtBrowseTexture();
	} else {
		ASSERT(FALSE); // Current client dialog must be texture dialog
	}
}

void CSeriousSkaStudioView::OnIncrementFadeTime() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	//pDoc->m_fFadeTime+=0.01f;
	pDoc->m_fFadeTime+=0.1f;
	pDoc->m_fFadeTime = ClampUp(pDoc->m_fFadeTime,10.0f);
	pDoc->AddShowEvent(CTString(0,"Fade time = %g",pDoc->m_fFadeTime),EV_FADE_TIME,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnDecrementFadeTime() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	// pDoc->m_fFadeTime-=0.01f;
	pDoc->m_fFadeTime-=0.1f;
	pDoc->m_fFadeTime = ClampDn(pDoc->m_fFadeTime,0.00f);
	pDoc->AddShowEvent(CTString(0,"Fade time = %g",pDoc->m_fFadeTime),EV_FADE_TIME,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnFreezeOnBlend() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_bFreezeOnBlend=!pDoc->m_bFreezeOnBlend;
	CTString strEvent;
	if(pDoc->m_bFreezeOnBlend) {
		strEvent = "Freeze on blend = TRUE";
	} else {
		strEvent = "Freeze on blend = FALSE";
	}

	pDoc->AddShowEvent(strEvent,EV_FREEZE_ON_BLEND,EVENT_FADE_TIME);
}

BOOL CSeriousSkaStudioView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	INDEX ctScrols = zDelta/120;

	m_fTargetDistance -= ctScrols;

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CSeriousSkaStudioView::OnCountWeightPerVertex() 
{
	NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
	if(ni.ni_iType != NT_MESHLOD) {
		theApp.NotificationMessage("Mesh lod must be selected");
		return;
	}

	MeshLOD *pmlod = (MeshLOD*)ni.ni_pPtr;
	ASSERT(pmlod!=NULL);
	
	CStaticArray<INDEX> aiVertexWeights;
	INDEX ctvx = pmlod->mlod_aVertices.Count();
	INDEX ctmwm = pmlod->mlod_aWeightMaps.Count();
	aiVertexWeights.New(ctvx);
	memset(&aiVertexWeights[0],0,sizeof(INDEX)*ctvx);
	// for each weight map in mesh
	for(INDEX imwm=0;imwm<ctmwm;imwm++) {
		MeshWeightMap &mwm = pmlod->mlod_aWeightMaps[imwm];
		// for each vertex weight in weight map
		INDEX ctmvw = mwm.mwm_aVertexWeight.Count();
		for(INDEX imvw=0;imvw<ctmvw;imvw++) {
			MeshVertexWeight &mvw = mwm.mwm_aVertexWeight[imvw];
			INDEX &vw = aiVertexWeights[mvw.mww_iVertex];
			vw++;
		}
	}

#define MAX_WMAPS 32
	INDEX iMaxVW = 0;
	INDEX aiWeightsPerVertex[MAX_WMAPS];
	memset(&aiWeightsPerVertex[0],0,sizeof(aiWeightsPerVertex));
	for(INDEX ivx=0;ivx<ctvx;ivx++) {
		INDEX &vw = aiVertexWeights[ivx];
		iMaxVW = Max(iMaxVW,vw);
		if(vw>=MAX_WMAPS) {
			ASSERT(FALSE);
			return;
		}
		aiWeightsPerVertex[vw]++;

	}

	CTString strInfo = "Vertices per weight:\n";
	for(INDEX ivpw=0;ivpw<iMaxVW+1;ivpw++) {
		strInfo += CTString(0,"  %d vertices in %d weights\n",aiWeightsPerVertex[ivpw],ivpw);
	}
	AfxMessageBox((const char*)strInfo);
}

void CSeriousSkaStudioView::OnIncrementBpv() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	INDEX ctBPV = pDoc->m_ctBonesPerVertex;
	pDoc->m_ctBonesPerVertex = ClampUp(INDEX(ctBPV+1),INDEX(MAX_BPV));

	CTString strEvent;
	strEvent = CTString(0, "Weights per vertex = %d",pDoc->m_ctBonesPerVertex);

	pDoc->AddShowEvent(strEvent,EV_WEIGHTS_PER_VERTEX,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnDecrementBpv() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	INDEX ctBPV = pDoc->m_ctBonesPerVertex;
	pDoc->m_ctBonesPerVertex = ClampDn(INDEX(ctBPV-1),INDEX(1));

	CTString strEvent;
	strEvent = CTString(0, "Weights per vertex = %d",pDoc->m_ctBonesPerVertex);

	pDoc->AddShowEvent(strEvent,EV_WEIGHTS_PER_VERTEX,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnUseSoftwareVp() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	_pShell->SetINDEX("ska_bUseHardwareShaders",TRUE);
	CTString strEvent;
	// if hardware shaders are available
	ENGINE_API BOOL RM_AreHardwareShadersAvailable(void);
	if(RM_AreHardwareShadersAvailable()) {
		strEvent = "Using hardware vertex processing";
	} else {
		strEvent = "Using software vertex processing";
	}
	pDoc->AddShowEvent(strEvent,EV_HARDWARE_VP,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnUseHardwareVp() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	_pShell->SetINDEX("ska_bUseHardwareShaders",FALSE);
	CTString strEvent = "Using software vertex processing";
	pDoc->AddShowEvent(strEvent,EV_HARDWARE_VP,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnVkCtrl1() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_spfPreviewMode = SPF_SELECTION;
	pDoc->AddShowEvent("Showing selection",EV_SKAPREVIEW,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnVkCtrl2() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_spfPreviewMode = SPF_SURFACES;
	pDoc->AddShowEvent("Showing surfaces",EV_SKAPREVIEW,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnVkCtrl3() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_spfPreviewMode = SPF_SUBSURFACES;
	pDoc->AddShowEvent("Showing sub surfaces",EV_SKAPREVIEW,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnVkCtrl4() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_spfPreviewMode = SPF_WEIGHTS_PER_VERTEX;
	pDoc->AddShowEvent("Showing weights per vertex",EV_SKAPREVIEW,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnVkCtrl5() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_spfPreviewMode = SPF_MORPHS_PER_VERTEX;
	pDoc->AddShowEvent("Showing morphs per vertex",EV_SKAPREVIEW,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnVkCtrl6() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_spfPreviewMode = SPF_STRIPS;
	pDoc->AddShowEvent("Showing strips",EV_SKAPREVIEW,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnVkCtrl7() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_spfPreviewMode = SPF_MODELS;
	pDoc->AddShowEvent("Showing models",EV_SKAPREVIEW,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnVkCtrl8() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_spfPreviewMode = SPF_MESH_LOD;
	pDoc->AddShowEvent("Showing meshes",EV_SKAPREVIEW,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnVkCtrl9() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_spfPreviewMode = SPF_NONE;
	pDoc->AddShowEvent("Showing normal",EV_SKAPREVIEW,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnVkCtrl0() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_spfPreviewMode = SPF_NONE;
	pDoc->AddShowEvent("Showing normal",EV_SKAPREVIEW,EVENT_FADE_TIME);
}


void CSeriousSkaStudioView::OnVkCtrlDelete() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_bOnlyMorphs = TRUE;
	pDoc->AddShowEvent("Animating only morphs",EV_ONLY_MORPHS,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnVkCtrlInsert() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->m_bOnlyMorphs = FALSE;
	pDoc->AddShowEvent("Animating weights and morphs",EV_ONLY_MORPHS,EVENT_FADE_TIME);
}

void CSeriousSkaStudioView::OnShowSubsurfaces() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(!theApp.m_bDisplaySubSurfaces) {
		pDoc->AddShowEvent("Sub surfaces are visible",EV_SHOW_SUBSURFACES,EVENT_FADE_TIME);
		theApp.m_bDisplaySubSurfaces = TRUE;
	} else {
		pDoc->AddShowEvent("Sub surfaces are hidden",EV_SHOW_SUBSURFACES,EVENT_FADE_TIME);
		theApp.m_bDisplaySubSurfaces = FALSE;
	}
	theApp.UpdateRootModelInstance();
}

void CSeriousSkaStudioView::OnVkZ() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(theApp.m_bDisplaySelection) {
		pDoc->AddShowEvent("Selection in not visible",EV_SHOW_SELECTION,EVENT_FADE_TIME);
		theApp.m_bDisplaySelection = FALSE;
	} else {
		pDoc->AddShowEvent("Selection is visible",EV_SHOW_SELECTION,EVENT_FADE_TIME);
		theApp.m_bDisplaySelection = TRUE;
	}
}

void CSeriousSkaStudioView::OnShowAxis() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(!theApp.m_bShowAxis) {
		pDoc->AddShowEvent("Axis model is visible",EV_SHOW_AXIS,EVENT_FADE_TIME);
		theApp.m_bShowAxis = TRUE;
	} else {
		pDoc->AddShowEvent("Axis model is hiden",EV_SHOW_AXIS,EVENT_FADE_TIME);
		theApp.m_bShowAxis = FALSE;
	}
}

void CSeriousSkaStudioView::OnShowallEvents() 
{

}

void CSeriousSkaStudioView::OnProfiling() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(theApp.m_bSceneProfiling) {
		pDoc->AddShowEvent("Profiling is off",EV_PROFILING,EVENT_FADE_TIME);
		theApp.m_bSceneProfiling = FALSE;
	} else {
		pDoc->AddShowEvent("Profiling is on",EV_PROFILING,EVENT_FADE_TIME);
		theApp.m_bSceneProfiling = TRUE;
	}
}

void CSeriousSkaStudioView::OnShowFps() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(theApp.m_bShowFPS) {
		pDoc->AddShowEvent("FPS are hidden",EV_FPS,QUICK_EVENT_FADE_TIME);
		theApp.m_bShowFPS = FALSE;
	} else {
		pDoc->AddShowEvent("FPS are visible",EV_FPS,QUICK_EVENT_FADE_TIME);
		theApp.m_bShowFPS = TRUE;
	}
}

void CSeriousSkaStudioView::OnShowprojectionshadow() 
{
	// TODO: Add your command handler code here
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc!=NULL) {
		pDoc->m_bShowShadow = !pDoc->m_bShowShadow;
	}
	
}

void CSeriousSkaStudioView::OnUpdateShowprojectionshadow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pCmdUI->SetCheck(pDoc->m_bShowShadow);	
}

//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
void CSeriousSkaStudioView::OnRefreshTagmanager() 
{
	// TODO: Add your command handler code here
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc->m_ModelInstance == NULL) return;
	CSkaTag tag;
	tag.SetName("__ROOT");
	pDoc->m_ModelInstance->m_tmSkaTagManager.Register(&tag);
	tag.SetName("__TOP");
	FLOATaabbox3D aabb;
	pDoc->m_ModelInstance->GetAllFramesBBox(aabb);
	tag.SetOffsetPos(0, aabb.Size()(2) * pDoc->m_ModelInstance->mi_vStretch(2), 0);
	pDoc->m_ModelInstance->m_tmSkaTagManager.Register(&tag);
//안태훈 수정 시작	//(Remake Effect)(0.1)
	for(INDEX i=0; i<pDoc->m_ModelInstance->mi_cmiChildren.Count(); ++i)
	{
		pDoc->m_ModelInstance->mi_cmiChildren[i].m_tmSkaTagManager.Register(&tag);
		pDoc->m_ModelInstance->mi_cmiChildren[i].RefreshTagManager();
	}
//안태훈 수정 끝	//(Remake Effect)(0.1)
//	_pShell->SetINDEX("ska_bShowSkaTag", !_pShell->GetINDEX("ska_bShowSkaTag"));
	pDoc->m_ModelInstance->RefreshTagManager();
}

void CSeriousSkaStudioView::OnSkatagNew() 
{
	// TODO: 안태훈, 새로운 스카 태그를 추가한다.
	ASSERTALWAYS("아직 구현하지 않았음. By Ant");
}

void CSeriousSkaStudioView::OnSkatagDelete() 
{
	// TODO: Add your command handler code here
}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

//안태훈 수정 시작	//(Remake Effect)(0.1)
void CSeriousSkaStudioView::OnAnimForwardOneTick() 
{
	// TODO: Add your command handler code here
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	if(pDoc->m_bViewPaused)
	{
		pDoc->m_tvPauseTime -= 0.01f;//_pTimer->TickQuantum;
	}
}
//안태훈 수정 끝	//(Remake Effect)(0.1)

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
#include <Engine/Effect/COrbitEffect.h>
void CSeriousSkaStudioView::OnShowEffect() 
{
	// TODO: Add your command handler code here
	COrbitEffect orbit;
	orbit.SetName("Orbit");
	orbit.SetLifeTime(1000);
	orbit.SetBoundingSphereRadius(100);
	COrbitEffect::sOrbitInfo info0;
	info0.fRadiusX = 1;
	info0.fPhaseMulX = 16;
	info0.fPhaseOffsetX = 0;
	info0.fRadiusY = 1;
	info0.fPhaseMulY = 15.9462f;
	info0.fPhaseOffsetY = 0.1657f;
	info0.fRadiusZ = 1;
	info0.fPhaseMulZ = 15;
	info0.fPhaseOffsetZ = PI*0.5f;
	info0.strEffectName = "(SB)TeleportSpline";
	info0.fTimeBeforeStart = 0;
	orbit.AddOrbit(info0);
	COrbitEffect::sOrbitInfo info1;
	info1.fRadiusX = 1;
	info1.fPhaseMulX = 15;
	info1.fPhaseOffsetX = 0;
	info1.fRadiusY = 1;
	info1.fPhaseMulY = 14;
	info1.fPhaseOffsetY = 0.25f;
	info1.fRadiusZ = 0;
	info1.fPhaseMulZ = 17;
	info1.fPhaseOffsetZ = 0.25f;
	info1.strEffectName = "(SB)TeleportSpline";
	info1.fTimeBeforeStart = 0;
	orbit.AddOrbit(info1);
	CEffectManager::Instance().Register(orbit.Copy());
	CEffectGroup eg;
	eg.SetName("Orbit");
	eg.AddEffect(0, "Orbit");
	eg.AddTagForAllEffect(0, "TEST-MOVE");
	CEffectGroupManager::Instance().Register(eg.Copy());
	for(int i=0; i<100; ++i)
	{
		StartEffectGroup("Orbit"
			//, &GetDocument()->m_ModelInstance->m_tmSkaTagManager
			, _pTimer->GetLerpedCurrentTick()
			, FLOAT3D(0,1,0)
			, ANGLE3D(0,0,0)
			);
	}
	CEffectGroupManager::Instance().Unregister("Orbit");
	CEffectManager::Instance().Unregister("Orbit");
}

void CSeriousSkaStudioView::OnUpdateShowEffect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}

void CSeriousSkaStudioView::OnAnimationEffectAdd() 
{
	if(pmiSelected == NULL) return; 
	CSeriousSkaStudioDoc *pDoc = GetDocument();

	HTREEITEM hSelected = theApp.m_dlgBarTreeView.m_TreeCtrl.GetSelectedItem();
	if(hSelected == NULL) return;
	INDEX iSelIndex = theApp.m_dlgBarTreeView.m_TreeCtrl.GetItemData(hSelected);
	NodeInfo &ni = theApp.aNodeInfo[iSelIndex];

	Animation *pan = (Animation*)ni.ni_pPtr;
	pmiSelected->AddAnimEffect(ska_IDToString(pan->an_iID), "null", 0);

	theApp.UpdateRootModelInstance();
	pDoc->MarkAsChanged();
}

void CSeriousSkaStudioView::OnAnimeffectRegAddnew() 
{
	if(pmiSelected == NULL) return; 
	CSeriousSkaStudioDoc *pDoc = GetDocument();

	HTREEITEM hSelected = theApp.m_dlgBarTreeView.m_TreeCtrl.GetSelectedItem();
	if(hSelected == NULL) return;
	INDEX iSelIndex = theApp.m_dlgBarTreeView.m_TreeCtrl.GetItemData(hSelected);
	NodeInfo &ni = theApp.aNodeInfo[iSelIndex];
	HTREEITEM hParent = theApp.m_dlgBarTreeView.m_TreeCtrl.GetParentItem(hSelected);
	if(hParent == NULL) return;
	INDEX iParent = theApp.m_dlgBarTreeView.m_TreeCtrl.GetItemData(hParent);
	NodeInfo &niParent = theApp.aNodeInfo[iParent];

	CAnimEffect &ae = *(CAnimEffect*)niParent.ni_pPtr;
	pmiSelected->AddAnimEffect(ae.GetAnimName(), "null", 0);

	theApp.UpdateRootModelInstance();
	pDoc->MarkAsChanged();
}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
extern FLOAT g_fAnimStrength;
void CSeriousSkaStudioView::OnAnimStrengthDown() 
{
	// TODO: Add your command handler code here
	g_fAnimStrength -= 0.05f;
	g_fAnimStrength = ClampDn(g_fAnimStrength, 0.0f);
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->AddShowEvent(CTString(0,"Strength time = %g",g_fAnimStrength),EV_ANIM_STRENGTH,5.0f);
}

void CSeriousSkaStudioView::OnAnimStrengthUp() 
{
	// TODO: Add your command handler code here
	g_fAnimStrength += 0.05f;
	g_fAnimStrength = ClampUp(g_fAnimStrength, 1.0f);
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	pDoc->AddShowEvent(CTString(0,"Strength time = %g",g_fAnimStrength),EV_ANIM_STRENGTH,5.0f);
}

void CSeriousSkaStudioView::OnMiAircolor() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();

	COLORREF acrCustClr[16];
	CHOOSECOLOR cc;
	acrCustClr[ 0] = CLRF_CLR(C_BLACK);
	acrCustClr[ 1] = CLRF_CLR(C_WHITE);
	acrCustClr[ 2] = CLRF_CLR(C_dGRAY);
	acrCustClr[ 3] = CLRF_CLR(C_GRAY);
	acrCustClr[ 4] = CLRF_CLR(C_lGRAY);
	acrCustClr[ 5] = CLRF_CLR(C_dRED); 
	acrCustClr[ 6] = CLRF_CLR(C_dGREEN);
	acrCustClr[ 7] = CLRF_CLR(C_dBLUE);
	acrCustClr[ 8] = CLRF_CLR(C_dCYAN);
	acrCustClr[ 9] = CLRF_CLR(C_dMAGENTA);
	acrCustClr[10] = CLRF_CLR(C_dYELLOW);
	acrCustClr[11] = CLRF_CLR(C_dORANGE);
	acrCustClr[12] = CLRF_CLR(C_dBROWN);
	acrCustClr[13] = CLRF_CLR(C_dPINK);
	acrCustClr[14] = CLRF_CLR(C_lORANGE);
	acrCustClr[15] = CLRF_CLR(C_lBROWN);

	pDoc->m_colAirColor &= 0xFFFFFF00;

	memset(&cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.rgbResult = ByteSwap(pDoc->m_colAirColor);
	cc.hwndOwner = GetSafeHwnd();
	cc.lpCustColors = (LPDWORD) acrCustClr;
	if(ChooseColor(&cc))
	{
		COLOR colColor = ByteSwap(cc.rgbResult);
		colColor |= 0xFF;
		pDoc->m_colAirColor  = colColor;
	}
}

void CSeriousSkaStudioView::OnMiGroundcolor() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();

	COLORREF acrCustClr[16];
	CHOOSECOLOR cc;
	acrCustClr[ 0] = CLRF_CLR(C_BLACK);
	acrCustClr[ 1] = CLRF_CLR(C_WHITE);
	acrCustClr[ 2] = CLRF_CLR(C_dGRAY);
	acrCustClr[ 3] = CLRF_CLR(C_GRAY);
	acrCustClr[ 4] = CLRF_CLR(C_lGRAY);
	acrCustClr[ 5] = CLRF_CLR(C_dRED); 
	acrCustClr[ 6] = CLRF_CLR(C_dGREEN);
	acrCustClr[ 7] = CLRF_CLR(C_dBLUE);
	acrCustClr[ 8] = CLRF_CLR(C_dCYAN);
	acrCustClr[ 9] = CLRF_CLR(C_dMAGENTA);
	acrCustClr[10] = CLRF_CLR(C_dYELLOW);
	acrCustClr[11] = CLRF_CLR(C_dORANGE);
	acrCustClr[12] = CLRF_CLR(C_dBROWN);
	acrCustClr[13] = CLRF_CLR(C_dPINK);
	acrCustClr[14] = CLRF_CLR(C_lORANGE);
	acrCustClr[15] = CLRF_CLR(C_lBROWN);

	pDoc->m_colGroundColor &= 0xFFFFFF00;

	memset(&cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.rgbResult = ByteSwap(pDoc->m_colGroundColor);
	cc.hwndOwner = GetSafeHwnd();
	cc.lpCustColors = (LPDWORD) acrCustClr;
	if(ChooseColor(&cc))
	{
		COLOR colColor = ByteSwap(cc.rgbResult);
		colColor |= 0xFF;
		pDoc->m_colGroundColor  = colColor;
	}
}

void CSeriousSkaStudioView::OnVkTab() 
{
	// TODO: Add your command handler code here
	theApp.m_dlgEffectEditType.OnBtnTestEffectsystem();
}

/*!
 *	Child Model을 RideSetting offset으로 설정한다.
 */
void CSeriousSkaStudioView::OnRidesetting()
{
	// TODO: Add your command handler code here
	NodeInfo& Ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();

	if (Ni.ni_iType == NT_MODELINSTANCE) // 모델 정보 노드여야 한다.
	{
		// Child 모델만 동작한다.
		CModelInstance *pmi = (CModelInstance*)Ni.ni_pPtr;
		CModelInstance *pmiParent = pmi->GetParent(theApp.GetDocument()->m_ModelInstance);

		if (pmiParent != NULL) // Child Model!
		{
			// Parent모델에 RideSetting value를 Child Model Offset으로 설정한다.
			pmiParent->SetRideOffsetPos(pmi->GetOffsetPos());
			pmiParent->SetRideOffsetRot(pmi->GetOffsetRot());
			pmiParent->mi_iRideParentBoneID = pmi->mi_iParentBoneID;

			// 설정한 Child Model은 삭제한다.
			// remove selected model instance from parent 
			pmiParent->DeleteChild(pmiSelected);
			// update root model instance
			theApp.UpdateRootModelInstance();
			theApp.GetDocument()->MarkAsChanged();
		}
	}
}
