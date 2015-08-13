403
%{
#include "StdH.h"
#include "EntitiesMP/Player.h"
#include "EntitiesMP/PlayerWeapons.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIInitJob.h>
%}


enum ViewType {
	0 VT_PLAYERDEATH    "",   // player death
	1 VT_PLAYERREBIRTH  "",   // player rebirth (player is spawned)
	2 VT_CAMERA         "",   // camera view
	3 VT_3RDPERSONVIEW  "",   // 3rd person view
};

// input parameter for viewer
event EViewInit {
	CEntityID eidOwner,        // who owns it
	CEntityID eidCamera,       // first camera for camera view
	enum ViewType vtView,           // view type
	BOOL bDeathFixed,
};

%{
extern ENGINE_API INDEX g_iCountry;
#include <Engine/GlobalDefinition.h>
#include <Engine/Effect/CCameraEffect.h>
extern ENGINE_API CCameraEffect::CCameraValue g_cvCameraShake;
FLOAT3D g_vOldCameraShake;
FLOAT3D g_vOldCameraPosByCharCoord;

void CPlayerView_Precache(void) 
{
	CDLLEntityClass *pdec = &CPlayerView_DLLClass;
	pdec->PrecacheModel(MODEL_MARKER);
	pdec->PrecacheTexture(TEXTURE_MARKER);
}

extern ENGINE_API CViewPort	*_pvpViewPortMain;

static FLOAT m_fLastTime;
static BOOL m_bLockRotate = FALSE;

// FIXME : 임시적으로 만든 변수.
// 추적 모드를 쓸 것인지 기존 방식을 쓸것인지...
static BOOL m_bUseTraceMode	= FALSE;

static ANGLE3D _aAbs;
static CPlacement3D _plNew;
static CPlacement3D _plOld;

void CameraEffect(const CPlacement3D &plPlr, CPlacement3D &plCam)
{
	CPlacement3D plNew = plCam;
	//twist 처리
	if(g_cvCameraShake.m_fTwist != 0)
	{
		plNew.pl_OrientationAngle(3) += g_cvCameraShake.m_fTwist;
	}

	//카메라 좌표기준 이동처리
	FLOATmatrix3D m;
	if(g_cvCameraShake.m_fHorizonal != 0
	|| g_cvCameraShake.m_fVertical != 0
	|| g_cvCameraShake.m_fZoom != 0)
	{
		MakeRotationMatrixFast(m, plCam.pl_OrientationAngle);
		FLOAT3D vMoveByCamCoord(g_cvCameraShake.m_fHorizonal
								, g_cvCameraShake.m_fVertical
								, g_cvCameraShake.m_fZoom);
		RotateVector(vMoveByCamCoord, m);
		plNew.pl_PositionVector += vMoveByCamCoord;
	}

	//캐릭터 좌표기준 이동처리
	if(g_cvCameraShake.m_vMoveByCharCoord(1) != 0
	|| g_cvCameraShake.m_vMoveByCharCoord(2) != 0
	|| g_cvCameraShake.m_vMoveByCharCoord(3) != 0)
	{
		MakeRotationMatrixFast(m, plPlr.pl_OrientationAngle);
		FLOAT3D vMoveByChaCoord = g_cvCameraShake.m_vMoveByCharCoord;
		RotateVector(vMoveByChaCoord, m);
		plNew.pl_PositionVector += vMoveByChaCoord;
	}

	plCam = plNew;
}
%}

class export CPlayerView : CMovableEntity {
name      "Player View";
thumbnail "";
features "CanBePredictable","NotSentOverNet";

properties:
	1 CEntityPointer m_penOwner,            // class which owns it
	2 INDEX m_iViewType=0,                  // view type  
	3 FLOAT m_fDistance = 1.0f,             // current distance
	4 FLOAT3D m_vZLast = FLOAT3D(0,0,0), 
	5 FLOAT3D m_vTargetLast = FLOAT3D(0,0,0), 
	6 BOOL m_bFixed = FALSE,  // fixed view (player falling in abyss)
	7 FLOAT m_fCameraHeight = 0.0f,

//강동민 수정 시작 싱글 던젼 작업	08.05
	10 BOOL m_bInit			= FALSE,
	15 ANGLE m_aCurAngle	= 0.0f,			// 현재 각도
	16 ANGLE m_aAccel		= 0.0f,			// 가속도
	17 ANGLE m_aVelocity	= 0.0f,			// 속도
	18 COLOR m_PrevModelColor = 0.0f,		// 모델의 컬러 저장
	19 BOOL m_bPrevModelColor = FALSE,		// 모델의 컬러 변경 유무
	20 FLOAT m_fFaceChangeDistance = 0.0f,	// Camera Distance for Face Change Mode
//강동민 수정 끝 싱글 던젼 작업		08.05	
	{
		CPlacement3D m_FreePosition;
	}

components:
	1 editor model   MODEL_MARKER     "Data\\Models\\Editor\\Axis.mdl",
	2 editor texture TEXTURE_MARKER   "Data\\Models\\Editor\\Vector.tex"

functions:
	void PreMoving() {};
	void DoMoving() 
	{
		en_plLastPlacement = GetPlacement();  // remember old placement for lerping
	};
	void PostMoving() 
	{
		// 뒤에서 추적하는 방식.
		if(m_bUseTraceMode)
		{
			// 매 프레임 마다 호출되는 부분.
			SetCameraPosition();
		}
		// 기존 방식.
		else
		{
			if (_pInput->inp_bFreeMode)
			{
				SetCameraFreeMode();
			}
			else if ( SE_Get_UIManagerPtr()->GetInitJob()->IsFaceDecoMode() )
			{
				SetCameraDecoMode();
			}
			else
			{
				SetCameraPositionEx();
			}
		}
	}
	CPlacement3D GetLerpedPlacement(void) const
	{
		FLOAT fLerpFactor;
		fLerpFactor = _pTimer->GetLerpFactor2();
		return LerpPlacementsPrecise(en_plLastPlacement, en_plPlacement, fLerpFactor);
//		return en_plPlacement;//이렇게 해놓으면 캐릭터가 떨림.
		//return CMovableEntity::GetLerpedPlacement();
	}

	// render particles
	void RenderParticles(void)
	{
		if (Particle_GetViewer()==this) {
			Particles_ViewerLocal(m_penOwner);
		}
	}

//강동민 수정 시작 싱글 던젼 작업	08.05
	void UnlockRotate()
	{
		m_bLockRotate = FALSE;
	};
//강동민 수정 끝 싱글 던젼 작업		08.05

	// FIXME : 기존 방식의 카메라.  
	// FIXME : 급조한 부분.
	void SetCameraPositionEx()
	{
		// 3rd person view
		FLOAT fDistance = 1.0f;
		//FLOAT fDistanceLimit = 4.5f;
		FLOAT fDistanceLimit = 9.5f;
		static FLOAT fDistanceTmp	= 0.0f;
		BOOL bHit = FALSE;

		// dungeon과 field 지역은 zoom out를 각기 다르게 설정한다. *** 이렇게 할 필요 없을꺼 같아서 아래 IF문 제거 ***
//		if (g_slZone == 0 || g_slZone == 4 || g_slZone == 7) 
//		{
			//fDistanceLimit = 6.5f;
			//fDistanceLimit = 9.5f;
			if (_pNetwork->MyCharacterInfo.m_ModelColor > 0)
			{
				fDistanceLimit = 24.5f;
			}
/*			else
			{
				fDistanceLimit = 9.5f;
			}*/
	/*		
			// 애완동물을 타고 있을때 카메라의 처리.
			if( ((CPlayer&) *m_penOwner).m_bRide)
			{
				if( ((CPlayer&) *m_penOwner).m_iRideType%2 == 0 )
				{
					fDistanceLimit = 2.0f;
				}
				else 
				{
					fDistanceLimit = 2.5f;
				}
			}
	*/
//		} 
		
		// -------wooss 060521 Camera Zoom fix -------------->>
		static BOOL bLimitIn	= FALSE;
		static FLOAT fSmooth	= 0.0f;
		//FLOAT zoomMin = -10.0f;
		FLOAT zoomMin = -2.0f;
		
		// 애완동물을 타고 있을때 카메라의 처리.
		if( ((CPlayer&) *m_penOwner).m_bRide)
		{
			if( ((CPlayer&) *m_penOwner).m_iRideType%2 == 0 )
			{
				zoomMin = 3.0f;
			}
			else 
			{
				zoomMin = 2.5f;
			}
		}
		// --------------------------------------------------<<

		CPlacement3D pl = ((CPlayerEntity&) *m_penOwner).en_plViewpoint;		// 카메라 위치?
		BOOL bFollowCrossHair;
		
		if (m_iViewType == VT_3RDPERSONVIEW || m_iViewType == VT_PLAYERDEATH) 
		{
			// ui 개편 작업중 카메라 뷰 포트 늘리기. [9/15/2009 rumist]
			//fDistance = 15.0f;//5.0f;//5.75f;			
			fDistance = 5.0f;			

			int WheelDelta = abs(_pInput->m_WheelPos)/120;
			if(_pInput->m_WheelPos > 0 || _pInput->GetInputDevice(1)->id_aicControls[KID_PAGEUP].ic_fValue==1.0f)
			{
				//휠을 위로 밀어올리기.Zoom In
				if(_pInput->GetInputDevice(1)->id_aicControls[KID_PAGEUP].ic_fValue==1.0f)
				{
					WheelDelta = 1.0f;
				}
				if(fDistanceTmp> zoomMin)
				{
					if(fDistanceTmp <= -1.6)
					{
						//fDistanceTmp -= 2.0f*WheelDelta;
						fDistanceTmp -= 0.2f*WheelDelta;
					}
					else
					{
						fDistanceTmp -= 2.0f*WheelDelta;
					}

					if(fDistanceTmp< zoomMin)
					{
						fDistanceTmp = zoomMin;
					
					}
								
					_pInput->m_WheelPos = 0;		
				}
				else 
				{
					if( fSmooth	> 0.0f && !bLimitIn) { fSmooth -=0.03f; }
					if( fSmooth	<= 0.0f) {
						fSmooth = 0.0f;
						_pInput->m_WheelPos = 0;
						if(WheelDelta > 1 ) {
							fDistanceTmp = zoomMin-1.0f;	
							fSmooth = 1.0f;
							bLimitIn =TRUE;

						}
					}

				}

				// 애완동물을 타고 있을때 카메라의 처리.
				if( ((CPlayer&) *m_penOwner).m_bRide && fDistanceTmp <= -1.1 )
				{
					if( ((CPlayer&) *m_penOwner).m_iRideType%2 != 0 )
					{
						fDistanceTmp = -1.1f;
					}
				}
			}
			else if(_pInput->m_WheelPos < 0 || _pInput->GetInputDevice(1)->id_aicControls[KID_PAGEDOWN].ic_fValue==1.0f)
			{//휠 아래로.Zoom Out
				
				if( bLimitIn ) { fDistanceTmp = zoomMin; }
				bLimitIn = FALSE;
				if(fSmooth < 0.9f) { fSmooth+=0.1f;}
				else if(fSmooth > 0.9f ) { 
					fSmooth =0.9f; 
				}
				if(_pInput->GetInputDevice(1)->id_aicControls[KID_PAGEDOWN].ic_fValue==1.0f)
				{
					WheelDelta = 1.0f;
				}

				if(fDistanceTmp < fDistanceLimit)
				{
					if(fDistanceTmp <= -1.6)
					{
						//fDistanceTmp += 2.0f*WheelDelta;
						fDistanceTmp += 0.2f*WheelDelta;
					}
					else
					{
						//fDistanceTmp += 2.0f*WheelDelta;
						fDistanceTmp += 0.4f*WheelDelta;
					}

					//	fDistanceTmp += 0.5f;
					_pInput->m_WheelPos = 0;
				} 
				
			}			

			if(fDistanceTmp > fDistanceLimit )
			{
				fDistanceTmp = fDistanceLimit;
			}
			if(fDistanceTmp < zoomMin-1.0f)
			{
				fDistanceTmp = zoomMin-1.0f;
			}
		
			
			fDistance = fDistance + fDistanceTmp;
			
			bFollowCrossHair = TRUE;
			// death
		} 
		
		// transform rotation angle
		// 회전 앵글을 변환함.
		// 상대적 좌표인 pl을 캐릭터의 위치를 바탕으로 변환시킴.
		CPlacement3D plTmp;
		plTmp = m_penOwner->GetLerpedPlacement();
		plTmp.pl_OrientationAngle(1) = pl.pl_OrientationAngle(1);

		pl.RelativeToAbsolute(plTmp);
		//pl.RelativeToAbsolute(m_penOwner->GetPlacement());
		// make base placement to back out from
		FLOAT3D vBase;
		EntityInfo *pei= (EntityInfo*) (m_penOwner->GetEntityInfo());
		
//강동민 수정 끝 싱글 던젼 작업		07.29

//안태훈 수정 시작	//(Modify Camera Behavior)(0.1)
		//계단을 올라갈때 캐릭터가 통통튀는 효과 막기.
		//GetLerpedPlacement는 한프레임전의 placement를 리턴하는 듯함.
		//GetEntityInfoPosition(m_penOwner, pei->vTargetCenter, vBase);	//원본
		vBase = m_penOwner->GetPlacement().pl_PositionVector;

		FLOATmatrix3D mRotation;
		// 캐릭터의 Angle을 회전 행렬로 변환.
		MakeRotationMatrixFast(mRotation, m_penOwner->GetPlacement().pl_OrientationAngle);
		vBase += FLOAT3D(pei->vTargetCenter[0], pei->vTargetCenter[1], pei->vTargetCenter[2])*mRotation;
		//vBase += FLOAT3D(pei->vTargetCenter[0], pei->vTargetCenter[1], pei->vTargetCenter[2]);
//안태훈 수정 끝	//(Modify Camera Behavior)(0.1)
		
		// create a set of rays to test
		FLOATmatrix3D m;
		// 회전 행렬 만들기...
		MakeRotationMatrixFast(m, pl.pl_OrientationAngle);

		// 축 벡터 얻기.
		FLOAT3D vRight = m.GetColumn(1);
		FLOAT3D vUp    = m.GetColumn(2);
		FLOAT3D vFront = m.GetColumn(3);
		
		// Ray를 쏘기위한 5개 벡터.
		FLOAT3D vDest[5];
		vDest[0] = vBase+vFront*fDistance+vUp*1.5f;
		vDest[1] = vBase+vFront*fDistance+vRight*1.5f;
		vDest[2] = vBase+vFront*fDistance-vRight*1.5f;
		vDest[3] = vBase+vFront*fDistance-vUp*1.5f;
		vDest[4] = vBase+vFront*fDistance;
			
		FLOAT fBack = 0;		
	//	if(_pNetwork->m_ubGMLevel < 2)	//0820	
		{		
		// for each ray
		for (INDEX i=0; i<5; i++) //0915 레이 하나만 써도 될듯하다...
		{
			// cast a ray to find if any brush is hit
			CCastRay crRay( m_penOwner, vBase, vDest[i]);
			crRay.cr_bHitTranslucentPortals = FALSE;
//			if(i==4)
//			{
			//crRay.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;
			crRay.cr_ttHitModels = CCastRay::TT_NONE;
//			}
//			else
//			{
//				crRay.cr_ttHitModels = CCastRay::TT_NONE;
//			}
			GetWorld()->CastRay(crRay);
								
			// if hit something //0809 아이템이나 몬스터,캐릭터가 아닐때는 거리 보정이 된다.
			if (crRay.cr_penHit!=NULL && (!(crRay.cr_penHit->GetFlags()&ENF_ALIVE) && !(crRay.cr_penHit->GetFlags()&ENF_ITEM))) 
			{			
				// clamp distance
				fDistance = Min(fDistance, crRay.cr_fHitDistance-0.5f);			

				// if hit polygon
				if (crRay.cr_pbpoBrushPolygon!=NULL) 
				{
					// back off
					FLOAT3D vDir = (vDest[i]-vBase).Normalize();

					// 충돌한 폴리곤과 방향의 내적 후 1/4 곱한후 카메라의 Distance 조절.
					FLOAT fD = Abs(FLOAT3D(crRay.cr_pbpoBrushPolygon->bpo_pbplPlane->bpl_plAbsolute)%vDir)*0.25f;
					fBack = Max(fBack, fD);
					bHit = TRUE;
				}	
			}
		}
		}	
		
		// 애완동물을 타고 있을때 카메라의 처리.
		if( ((CPlayer&) *m_penOwner).m_bRide)
		{
			if( ((CPlayer&) *m_penOwner).m_iRideType%2 == 0 )
			{
				fDistance -= 5.0f;
			}
			else 
			{
				fDistance -= 4.5f;
			}
		}

		fDistance = ClampDn(fDistance-fBack, 0.0f); // 거리는 0이상

		if (m_fDistance < fDistance)
		{
			m_fDistance += 0.5f; // 점점 멀어지게
		}
		
		if (m_fDistance >= fDistance)
		{
			m_fDistance = fDistance;
		}

		vBase += vFront*m_fDistance;
		pl.pl_PositionVector = vBase;//0610 kwon 추가
		if (_pNetwork->MyCharacterInfo.m_ModelColor > 0 && g_iCountry != JAPAN)
		{
			pl.pl_PositionVector(2) += m_fCameraHeight;
		}

		// wooss 060803 수정
		// 캐릭터 절단면이 보이지 않게 카메라는 기존대로 줌인되지만 
		// 캐릭터 절단거리에서 캐릭터는 그리지 않게 한다.
		if(m_fDistance<1.0f) 
		{ 
			if (!m_bPrevModelColor)
			{
				m_PrevModelColor = ((CPlayer&)*m_penOwner).GetModelInstance()->GetModelColor();
				COLOR tColor = m_PrevModelColor;
				tColor &= 0xFFFFFF00;
				tColor |= 0x0F;
				((CPlayer&)*m_penOwner).GetModelInstance()->mi_colModelColor = tColor;
				m_bPrevModelColor = TRUE;
			}
		}
		else
		{
			if (m_bPrevModelColor)
			{
				((CPlayer&)*m_penOwner).GetModelInstance()->mi_colModelColor = m_PrevModelColor;
				m_bPrevModelColor = FALSE;
			}
		}

		

		// 애완동물의 카메라 조정을 위한 부분.
		if(((CPlayer&) *m_penOwner).m_bRide)
		{
			if( ((CPlayer&) *m_penOwner).m_iRideType %2 == 0 )
			{
				vBase += vFront*m_fDistance;
		    	pl.pl_PositionVector = vBase;
				pl.pl_PositionVector(2) += 1.0f;
			}
			else
			{
				vBase += vFront*m_fDistance;
		    	pl.pl_PositionVector = vBase;
				pl.pl_PositionVector(2) += 3.0f;
			}
		}
		if(!_pNetwork->m_bSingleMode)		
		{
			// wooss 060323
			// 카메라 기본 위치 상향 조정
			pl.pl_PositionVector(2) += fSmooth;
		}

		// set camera placement
		// 카메라의 위치 설정.
		// en_plPlacement값을 설정함.
		SetPlacement_special(pl, m, SPIF_NEAR); // TRUE = try to optimize for small movements
	};
	
	void SetCameraFreeMode()
	{
		CPlacement3D pl = GetPlacement();
		_plNew = pl;

		FLOAT fFB = 0.0f; // forward/backward movement
		FLOAT fLR = 0.0f; // left/right movement
		FLOAT fUD = 0.0f; // up/down movement
		FLOAT fRLR = 0.0f; // rotate left/right
		FLOAT fRUD = 0.0f; // rotate up/down

		_pInput->GetInput(FALSE);

		if (_pInput->m_WheelPos > 0) // Up
		{
			_pInput->inp_fSpeedMultiplier += 0.1f;

			if (_pInput->inp_fSpeedMultiplier > 3.0f)
			{
				_pInput->inp_fSpeedMultiplier = 3.0f;
			}

			_pInput->m_WheelPos = 0;
		}
		else if (_pInput->m_WheelPos < 0) // Down
		{
			_pInput->inp_fSpeedMultiplier -= 0.1f;

			if (_pInput->inp_fSpeedMultiplier < 0.0f)
			{
				_pInput->inp_fSpeedMultiplier = 0.0f;
			}

			_pInput->m_WheelPos = 0;
		}

		if (BOOL(_pInput->GetControlValue(GetControlGID(DT_MOUSE,CID_MOUSE_BUTTON2))))
		{
			fFB = -1.0f * _pInput->inp_fSpeedMultiplier;
		}
		else if (BOOL(_pInput->GetControlValue(GetControlGID(DT_MOUSE,CID_MOUSE_BUTTON1))))
		{
			fFB = +1.0f * _pInput->inp_fSpeedMultiplier;
		}

		_pInput->GetInput(TRUE);
		
		// get current rotation
		//fRLR = _pInput->GetControlValue(GetControlGID(DT_MOUSE,CID_MOUSE_AXIS_XP));//*0.75f;
		//fRLR-= _pInput->GetControlValue(GetControlGID(DT_MOUSE,CID_MOUSE_AXIS_XN));//*0.75f;

		//fRUD = _pInput->GetControlValue(GetControlGID(DT_MOUSE,CID_MOUSE_AXIS_YP));//*0.75f;
		//fRUD-= _pInput->GetControlValue(GetControlGID(DT_MOUSE,CID_MOUSE_AXIS_YN));//*0.75f;
		//fRUD = -fRUD;
		pl.pl_OrientationAngle = m_FreePosition.pl_OrientationAngle;
		pl.Translate_OwnSystem( FLOAT3D(fLR, fUD, fFB));

		// create a set of rays to test
		FLOATmatrix3D m;
		// 회전 행렬 만들기...
		MakeRotationMatrixFast(m, pl.pl_OrientationAngle);
		SetPlacement_special(pl, m, SPIF_NEAR); // TRUE = try to optimize for small movements
		// apply rotation
		//pl.pl_OrientationAngle = _aAbs;//.Rotate_HPB( ANGLE3D(-fRLR, -fRUD, 0));
		//_aAbs+=ANGLE3D(-fRLR, -fRUD, 0);

		//pl.pl_OrientationAngle = _aAbs;
		//pl.Lerp(_plOld, _plNew, _pTimer->GetLerpFactor2());

		//FLOATmatrix3D m;
		// 회전 행렬 만들기...
		//MakeRotationMatrixFast(m, pl.pl_OrientationAngle);
		
		//SetPlacement_special(pl, m, SPIF_NEAR); // TRUE = try to optimize for small movements
	}

	void SetCameraDecoMode()
	{
		FLOAT fDistance = m_fFaceChangeDistance;
		CPlacement3D pl = ((CPlayerEntity&) *m_penOwner).en_plViewpoint;

		// View Point를 캐릭터 기준으로 변환
		CPlacement3D plTmp = m_penOwner->GetLerpedPlacement();
		plTmp.pl_OrientationAngle(1) = pl.pl_OrientationAngle(1);
		pl.RelativeToAbsolute(plTmp);
		
		// 회전 행렬 만들기
		FLOATmatrix3D m;
		MakeRotationMatrixFast(m, pl.pl_OrientationAngle);

		// Distance
		FLOAT3D vBase = m_penOwner->GetPlacement().pl_PositionVector;
		FLOAT3D vFront = m.GetColumn(3);
		vBase += vFront*fDistance;

		pl.pl_PositionVector = vBase;

		// Camera Height
		pl.pl_PositionVector(2) += m_fCameraHeight;

		SetPlacement_special(pl, m, SPIF_NEAR);

	}
//0408 kwon
	void SetCameraPosition() 
	{
		// 3rd person view
		FLOAT fDistance = 1.0f;
		static FLOAT fDistanceTmp	= 0.0f;
		static FLOAT fUpTmp			= 0.0f;		

		CPlacement3D pl = ((CPlayerEntity&) *m_penOwner).en_plViewpoint;		// 카메라 위치?
		BOOL bFollowCrossHair;
		
//강동민 수정 시작 싱글 던젼 작업	08.05
		extern BOOL	_bWorldEditorApp;
		//BOOL bBackView		= !_bWorldEditorApp;

		// FIXME : 임시적으로 테스트.
		BOOL bBackView		= TRUE;
//강동민 수정 끝 싱글 던젼 작업		08.05
		
		if (m_iViewType == VT_3RDPERSONVIEW || m_iViewType == VT_PLAYERDEATH) 
		{
		
			if(_pNetwork->m_bSingleMode)
			{
				fDistance	= 6.0f;//5.75f;				
				}
			else
			{
				fDistance = 5.0f;//5.75f;
				}

			if(_pInput->GetInputDevice(1)->id_aicControls[KID_PAGEUP].ic_fValue==1.0f)
			{
				fUpTmp -= 10.0f;
				m_bLockRotate			= TRUE;
			}
			if(_pInput->GetInputDevice(1)->id_aicControls[KID_PAGEDOWN].ic_fValue==1.0f)
			{
				fUpTmp += 10.0f;
				m_bLockRotate			= TRUE;
			}

			//pl.pl_OrientationAngle(1) += fUpTmp;

			if(_pInput->m_WheelPos > 0){//휠을 위로 밀어올리기.Zoom In
				if(fDistanceTmp> -3.6f){
					if(fDistanceTmp <= -1.6){
						fDistanceTmp -= 0.2f;
					}
					else
					{
						fDistanceTmp -= 1.0f;
					}
					//fDistanceTmp -= 0.5f;
					_pInput->m_WheelPos = 0;		
				}
			}
			else if(_pInput->m_WheelPos < 0){//휠 아래로.Zoom Out
				if(fDistanceTmp < 25.0f){//10.0f){ 
					if(fDistanceTmp <= -1.6){
						fDistanceTmp += 0.2f;
					}
					else
					{
						fDistanceTmp += 1.0f;
					}
					//	fDistanceTmp += 0.5f;
					_pInput->m_WheelPos = 0;
				}
			}
			
			if(_pNetwork->m_bSingleMode)
			{
				fDistanceTmp = 0.0f;
			}
			
			fDistance = fDistance + fDistanceTmp;
			
			bFollowCrossHair = TRUE;
			// death
		} 
		
		pl.pl_OrientationAngle(3) = 0.0f;
		
		// transform rotation angle
		// 회전 앵글을 변환함.
		// 상대적 좌표인 pl을 캐릭터의 위치를 바탕으로 변환시킴.
		CPlacement3D plTmp;
		plTmp = m_penOwner->GetPlacement();
		//plTmp.pl_OrientationAngle(1) = pl.pl_OrientationAngle(1);

		pl.RelativeToAbsolute(plTmp);
		pl.RelativeToAbsolute(m_penOwner->GetPlacement());
		// make base placement to back out from
		FLOAT3D vBase;
		EntityInfo *pei= (EntityInfo*) (m_penOwner->GetEntityInfo());

//강동민 수정 시작 싱글 던젼 작업	07.29
		if(_pNetwork->m_bSingleMode && bBackView)
		{
			CPlayerWeapons *ppw = ((CPlayer&) *m_penOwner).GetPlayerWeapons();			
			FLOAT m_fCurTime	= _pTimer->CurrentTick();

			if(!m_bLockRotate)
			{				
				// 플레이어와 각 엔티티 사이의 각도를 계산하여 처리함.
				if(ppw->m_penRayHitTmp)
				{				
					FLOAT3D vDelta	= ppw->m_vRayHitTmp - m_penOwner->GetPlacement().pl_PositionVector;
					vDelta.SafeNormalize();
					ANGLE aDelta	= GetRelativeHeading(vDelta);
					aDelta			= NormalizeAngle(aDelta);
	
					if(!m_bInit)
					{
						m_bInit		= TRUE;
						m_fLastTime	= m_fCurTime;
					}
					m_aVelocity		= aDelta;
				}
				FLOAT m_fDeltaTime	= m_fCurTime - m_fLastTime;
				m_aVelocity			= m_aVelocity + (m_aAccel * m_fDeltaTime);
				m_aCurAngle			= m_aCurAngle + (m_aVelocity * m_fDeltaTime);				
			}

			m_fLastTime				= m_fCurTime;
			//pl.pl_OrientationAngle(1) = m_aCurAngle;
			pl.pl_OrientationAngle(1) = m_aCurAngle - fUpTmp;
		}
		
//강동민 수정 끝 싱글 던젼 작업		07.29

//안태훈 수정 시작	//(Modify Camera Behavior)(0.1)
		//계단을 올라갈때 캐릭터가 통통튀는 효과 막기.
		//GetLerpedPlacement는 한프레임전의 placement를 리턴하는 듯함.
		//GetEntityInfoPosition(m_penOwner, pei->vTargetCenter, vBase);	//원본
		vBase = m_penOwner->GetLerpedPlacement().pl_PositionVector;

		FLOATmatrix3D mRotation;
		// 캐릭터의 Angle을 회전 행렬로 변환.
		MakeRotationMatrixFast(mRotation, m_penOwner->GetPlacement().pl_OrientationAngle);
		vBase += FLOAT3D(pei->vTargetCenter[0], pei->vTargetCenter[1], pei->vTargetCenter[2])*mRotation;
		//vBase += FLOAT3D(pei->vTargetCenter[0], pei->vTargetCenter[1], pei->vTargetCenter[2]);
//안태훈 수정 끝	//(Modify Camera Behavior)(0.1)
		
		// create a set of rays to test
		FLOATmatrix3D m;

		// 회전 행렬 만들기...
		MakeRotationMatrixFast(m, pl.pl_OrientationAngle);

		// 축 벡터 얻기.
		FLOAT3D vRight = m.GetColumn(1);
		FLOAT3D vUp    = m.GetColumn(2);
		FLOAT3D vFront = m.GetColumn(3);
		
		// Ray를 쏘기위한 5개 벡터.
		FLOAT3D vDest[5];
		vDest[0] = vBase+vFront*fDistance+vUp*1.0f;
		vDest[1] = vBase+vFront*fDistance-vUp*1.0f;
		vDest[2] = vBase+vFront*fDistance+vRight*1.0f;
		vDest[3] = vBase+vFront*fDistance-vRight*1.0f;
		vDest[4] = vBase+vFront*fDistance;
		
		FLOAT fBack = 0;
		// for each ray
		for (INDEX i=0; i<5; i++) 
		{
			// cast a ray to find if any brush is hit
			CCastRay crRay( m_penOwner, vBase, vDest[i]);
			crRay.cr_bHitTranslucentPortals = FALSE;
			crRay.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;
			GetWorld()->CastRay(crRay);
			
			// if hit something //0809 아이템은 hit되지 않는다..
			if (crRay.cr_penHit!=NULL && (!(crRay.cr_penHit->GetFlags()&ENF_ALIVE) && !(crRay.cr_penHit->GetFlags()&ENF_ITEM))) 
			{
				// clamp distance
				fDistance = Min(fDistance, crRay.cr_fHitDistance-0.5f);
				// if hit polygon
				if (crRay.cr_pbpoBrushPolygon!=NULL) 
				{
					// back off
					FLOAT3D vDir = (vDest[i]-vBase).Normalize();

					/*
					// FIXME : 맘에 들지 않는 부분.
					// FIXME : 벽과 충돌했을때, 위에서 바라보도록 처리하는 부분임.
					// FIXME : 변환된 값을 다시 수정해서 다시 변환하고... 맘에 안듬.
					if(_pNetwork->m_bSingleMode)
					{
						// 카메라를 위로 올리는 부분.
						pl.AbsoluteToRelative(m_penOwner->GetPlacement());
						pl.pl_OrientationAngle(2) = -85.0f;
						pl.RelativeToAbsolute(m_penOwner->GetPlacement());
						MakeRotationMatrixFast(m, pl.pl_OrientationAngle);
						vFront = m.GetColumn(3);
						*/

						// 충돌했을때 알파값을 넣어주는 부분.
						/*
						COLOR crModel	= m_penOwner->GetModelColor();
						COLOR crColor	= 0xFFFFFF00;
						m_penOwner->SetModelColor(0xFFFFFFA0);
						*/
						/*
					}
					else
					{*/
						// 충돌한 폴리곤과 방향의 내적 후 1/4 곱한후 카메라의 Distance 조절.
					FLOAT fD = Abs(FLOAT3D(crRay.cr_pbpoBrushPolygon->bpo_pbplPlane->bpl_plAbsolute)%vDir)*0.25f;
					fBack = Max(fBack, fD);
					//}					
					//m_penOwner->SetModelColor(0xFFFFFF10);
				}	
			}
		}
		
		fDistance = ClampDn(fDistance-fBack, 0.0f);
		m_fDistance = fDistance;
		vBase += vFront*fDistance;		
		
//안태훈 수정 시작	//(Modify Camera Behavior)(0.1)
		//계단을 올라갈때 화면이 울렁거리는 효과 막기.
/*
		if (bFollowCrossHair) {
			FLOAT3D vTarget = vBase-ppw->m_vRayHit;
			FLOAT fLen = vTarget.Length();
			if (fLen>0.01) {
				vTarget/=fLen;
			} else {
				vTarget = FLOAT3D(0,1,0);
			}
			
			FLOAT3D vX;
			FLOAT3D vY = m.GetColumn(2);
			FLOAT3D vZ = vTarget;
			vZ.Normalize();
			
			if (Abs(vY%vZ)>0.9f) {
				vY = -m.GetColumn(3);
			}
			
			vX = vY*vZ;
			vX.Normalize();
			vY = vZ*vX;
			vY.Normalize();
			m_vZLast = vZ;
			
			m(1,1) = vX(1); m(1,2) = vY(1); m(1,3) = vZ(1);
			m(2,1) = vX(2); m(2,2) = vY(2); m(2,3) = vZ(2);
			m(3,1) = vX(3); m(3,2) = vY(3); m(3,3) = vZ(3);
			DecomposeRotationMatrixNoSnap(pl.pl_OrientationAngle, m);
		}
*/		//원본
//안태훈 수정 끝	//(Modify Camera Behavior)(0.1)

    	pl.pl_PositionVector = vBase;//0610 kwon 추가
	/* //0610 kwon 삭제.
		if (m_bFixed) {
			pl.pl_PositionVector = GetPlacement().pl_PositionVector;
			pl.pl_OrientationAngle = ANGLE3D(0,-90, 0);
			m_fDistance = (pl.pl_PositionVector-m_penOwner->GetPlacement().pl_PositionVector).Length();
			MakeRotationMatrixFast(m, pl.pl_OrientationAngle);
		} else {
			pl.pl_PositionVector = vBase;
		}
	*/		
		
		// set camera placement
		// 카메라의 위치 설정.
		// en_plPlacement값을 설정함.
		SetPlacement_special(pl, m, SPIF_NEAR); // TRUE = try to optimize for small movements
	};
	
procedures:
	Main(EViewInit eInit) 
	{
		// remember the initial parameters
		ASSERT(((CEntity*)eInit.eidOwner)!=NULL);
		m_penOwner = eInit.eidOwner;
//0213 시작부터 3인칭
		m_iViewType = VT_3RDPERSONVIEW; //eInit.vtView;
		m_bFixed = eInit.bDeathFixed;
		ASSERT(IsOfClass(m_penOwner, &CPlayer_DLLClass));

		// init as model
		InitAsEditorModel();
		SetFlags(GetFlags()|ENF_CROSSESLEVELS);
		SetPhysicsFlags(EPF_MODEL_IMMATERIAL|EPF_MOVABLE);
		SetCollisionFlags(ECF_IMMATERIAL);

		SetFlagOff(ENF_PLACCHANGED|ENF_PROPSCHANGED);
		SetFlagOff(ENF_PROPSCHANGED);

		// set appearance
		SetModel(MODEL_MARKER);
		SetModelMainTexture(TEXTURE_MARKER);
/*//0815 삭제.
		// add to movers list if needed
		if (m_iViewType == VT_PLAYERDEATH) 
		{
			AddToMovers();
		}
*/
		m_bLockRotate = FALSE;

		SendEvent(EStart());
		wait() 
		{
			on (EBegin) : 
			{ 				
				resume; 
			}
			on (EStart) : 
			{
				// PlayerView가 활동을 시작하면 먼저 카메라 위치를 설정함(딱 한번만.)
				//SetCameraPosition();
				// 뒤에서 추적하는 방식.
				if(m_bUseTraceMode)
				{
					// 매 프레임 마다 호출되는 부분.
					SetCameraPosition();
				}
				// 기존 방식.
				else
				{
					SetCameraPositionEx();
				}
				en_plLastPlacement = GetPlacement();  // remember old placement for lerping
				m_vTargetLast = ((CPlayer&) *m_penOwner).GetPlayerWeapons()->m_vRayHit;
				resume;
			};
			on (EEnd) : { stop; }
			on (ETrigger) : 
			{
				_pInput->inp_bFreeMode = !_pInput->inp_bFreeMode;
				if (_pInput->inp_bFreeMode)
				{
					m_FreePosition = GetPlacement();
					while (ShowCursor(FALSE)>=0){};
				}
				else
				{
					while (ShowCursor(TRUE)<0){};
				}
				
				resume;
			}
			otherwise() : { resume; }
		}
		// cease to exist
		Destroy(FALSE);

		return;
	};
};

