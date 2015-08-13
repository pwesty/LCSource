221
%{
#include "StdH.h"
#include <Engine/CurrentVersion.h>
#include <Engine/Interface/UIManager.h>

#define CHECK_MIN_ENEMY		5
%}

//uses "EntitiesMP/KeyItem";
uses "EntitiesMP/Player";
uses "EntitiesMP/MovingBrush";

enum DoorType {
  0 DT_AUTO       "Auto",       // opens automatically
  1 DT_TRIGGERED  "Triggered",  // opens when triggered
  2 DT_LOCKED     "Locked",     // requires a key
  3 DT_TRIGGEREDAUTO     "Triggered Auto",     // opens automatically after being triggered
};

class CDoorController : CRationalEntity {
name      "DoorController";
thumbnail "Thumbnails\\DoorController.tbn";
features  "HasName", "IsTargetable", "HasRaidObject", "RaidEvent";


properties:

  1 CTString m_strName          "Name" 'N' = "DoorController",
  200 BOOL m_bRaidObject		"This entity is RaidObject" = FALSE,		// 레이드 오브젝트 설정
  201 INDEX m_RaidEventType		"Raid Event Type" = 0,			// 레이드 이벤트 타입	  
  2 CTString m_strDescription = "",
  3 CEntityPointer m_penTarget1  "Target1" 'T' COLOR(C_MAGENTA|0xFF),
  4 CEntityPointer m_penTarget2  "Target2" COLOR(C_MAGENTA|0xFF),
  5 FLOAT m_fWidth              "Width"  'W' = 2.0f,
  6 FLOAT m_fHeight             "Height" 'H' = 3.0f,
  7 BOOL m_bPlayersOnly         "Players Only" 'P' = TRUE,
  8 enum DoorType m_dtType      "Type" 'Y' = DT_AUTO,
  9 CTStringTrans m_strLockedMessage "Locked message" 'L' = "",
  13 CEntityPointer m_penLockedTarget  "Locked target" COLOR(C_dMAGENTA|0xFF),   // target to trigger when locked
  14 BOOL m_bTriggerOnAnything "Trigger on anything" = FALSE,
  15 BOOL m_bActive "Active" 'A' = TRUE,    // automatic door function can be activated/deactivated

  10 BOOL m_bLocked = FALSE,  // for lock/unlock door
  11 CEntityPointer m_penCaused,    // for trigger relaying
//강동민 수정 시작 다중 공격 작업	08.27
  17 BOOL m_bOpened	"Opened?"	= FALSE,
  18 INDEX	m_iOpenMobNum		"Mob Count"	= 1,			// 문을 열때 몹이 몇마리 이하 남아있어야 하는가?
  20 INDEX	m_iMaxCheckPoint	"Max Check Point"	= 0,	// MAX CHECK POINT
//강동민 수정 끝 다중 공격 작업		08.27

  21 FLOAT m_tmRaidSendedTime = 0.0f,	//[091106: selo] 레이드 이벤트 정보를 보낸 시간


components:

  1 model   MODEL_DOORCONTROLLER     "Data\\Models\\Editor\\DoorController.mdl",
  2 editor texture TEXTURE_DOORCONTROLLER   "Data\\Models\\Editor\\DoorController.tex",


functions:

  CEntity *GetTarget(void) const { return m_penTarget1; };

  const CTString &GetDescription(void) const
  {
    if (m_penTarget1!=NULL && m_penTarget2!=NULL) {
      ((CTString&)m_strDescription).PrintF("->%s,%s", 
        m_penTarget1->GetName(), m_penTarget2->GetName());
    } else if (m_penTarget1!=NULL) {
      ((CTString&)m_strDescription).PrintF("->%s", 
        m_penTarget1->GetName());
    } else {
      ((CTString&)m_strDescription).PrintF("-><none>");
    }
    return m_strDescription;
  }

//강동민 수정 시작 다중 공격 작업	08.27
  virtual void Read_t( CTStream *istr, BOOL bNetwork)
  {
	CRationalEntity::Read_t(istr,bNetwork);
	// 공선전 지역 문을 월드 로드시에 열어 둔다.(월드 에디터에서 열어 둠)
  }
//강동민 수정 끝 다중 공격 작업		08.27

  // test if this door reacts on this entity
  BOOL CanReactOnEntity(CEntity *pen)
  {
    if (pen==NULL) {
      return FALSE;
    }
    // never react on non-live or dead entities
    if (!(pen->GetFlags()&ENF_ALIVE)) {
      return FALSE;
    }

    if (m_bPlayersOnly && !IsDerivedFromClass(pen, &CPlayer_DLLClass)) {
      return FALSE;
    }

    return TRUE;
  }

  // test if this door can be triggered by this entity
  BOOL CanTriggerOnEntity(CEntity *pen)
  {
    return m_bTriggerOnAnything || CanReactOnEntity(pen);
  }

  void TriggerDoor(void)
  {
		//m_bOpened = !m_bOpened;
		// 왼쪽문과 오른쪽 문에 이벤트를 보내줌.
		if (m_penTarget1!=NULL) 
		{
			//m_penTarget1->SetPhysicsFlags(EPF_MOVABLE | GetPhysicsFlags());
			//SendToTarget(m_penTarget1, EET_TRIGGER, m_penCaused);			
			// 메라크 공성 지역일때는...
			if(g_slZone == 7 )
			{					
				if( ((CMovingBrush*)(CEntity*)m_penTarget1)->m_penTarget )
				{
					((CMovingBrush*)(CEntity*)m_penTarget1)->m_bForceStop		= TRUE;					
					((CMovingBrush*)(CEntity*)m_penTarget1)->m_bMoveToMarker	= FALSE;
					((CMovingBrush*)(CEntity*)m_penTarget1)->m_bStopMoving		= TRUE;
					((CMovingBrush*)(CEntity*)m_penTarget1)->m_bMoving			= FALSE;
					((CMovingBrush*)(CEntity*)m_penTarget1)->m_bRotating		= FALSE;
					((CMovingBrush*)(CEntity*)m_penTarget1)->SetDesiredTranslation(FLOAT3D(0.0f, 0.0f, 0.0f));
					((CMovingBrush*)(CEntity*)m_penTarget1)->SetDesiredRotation(FLOAT3D(0.0f, 0.0f, 0.0f));
					
					if( !m_bOpened )
					{
						CEntity* penTarget = ((CMovingBrush*)(CEntity*)m_penTarget1)->m_penTarget->GetTarget();						
						m_penTarget1->SetPlacement( penTarget->en_plPlacement );
					}
					else
					{
						CEntity* penTargetTemp = ((CMovingBrush*)(CEntity*)m_penTarget1)->m_penTarget->GetTarget();						
						CEntity* penTarget = ((CMovingBrushMarker*)(CEntity*)penTargetTemp)->m_penTarget;						
						m_penTarget1->SetPlacement( penTarget->en_plPlacement );
					}
				}
			}
			else
			{
				SendToTarget(m_penTarget1, EET_TRIGGER, m_penCaused);
			}
		}
		if (m_penTarget2!=NULL) 
		{
			//m_penTarget2->SetPhysicsFlags(EPF_MOVABLE | GetPhysicsFlags());
			// 메라크 공성 지역일때는...
			if(g_slZone == 7 )
			{				
				if( ((CMovingBrush*)(CEntity*)m_penTarget2)->m_penTarget )
				{
					((CMovingBrush*)(CEntity*)m_penTarget2)->m_bForceStop		= TRUE;					
					((CMovingBrush*)(CEntity*)m_penTarget2)->m_bMoveToMarker	= FALSE;
					((CMovingBrush*)(CEntity*)m_penTarget2)->m_bStopMoving		= TRUE;
					((CMovingBrush*)(CEntity*)m_penTarget2)->m_bMoving			= FALSE;
					((CMovingBrush*)(CEntity*)m_penTarget2)->m_bRotating		= FALSE;
					((CMovingBrush*)(CEntity*)m_penTarget2)->SetDesiredTranslation(FLOAT3D(0.0f, 0.0f, 0.0f));
					((CMovingBrush*)(CEntity*)m_penTarget2)->SetDesiredRotation(FLOAT3D(0.0f, 0.0f, 0.0f));
					if( !m_bOpened )
					{
						CEntity* penTarget = ((CMovingBrush*)(CEntity*)m_penTarget2)->m_penTarget->GetTarget();						
						m_penTarget2->SetPlacement( penTarget->en_plPlacement );
					}
					else
					{
						CEntity* penTargetTemp = ((CMovingBrush*)(CEntity*)m_penTarget2)->m_penTarget->GetTarget();						
						CEntity* penTarget = ((CMovingBrushMarker*)(CEntity*)penTargetTemp)->m_penTarget;						
						m_penTarget2->SetPlacement( penTarget->en_plPlacement );
					}
				}
			}
			else
			{
				SendToTarget(m_penTarget2, EET_TRIGGER, m_penCaused);
			}
		}
  }

  // apply mirror and stretch to the entity
  void MirrorAndStretch(FLOAT fStretch, BOOL bMirrorX)
  {
    // stretch its ranges
    m_fWidth*=fStretch;
    m_fHeight*=fStretch;
  }


  // returns bytes of memory used by this object
  SLONG GetUsedMemory(void)
  {
    // initial
    SLONG slUsedMemory = sizeof(CDoorController) - sizeof(CRationalEntity) + CRationalEntity::GetUsedMemory();
    // add some more
    slUsedMemory += m_strDescription.Length();
    slUsedMemory += m_strName.Length();
    slUsedMemory += m_strLockedMessage.Length();
    return slUsedMemory;
  }

  BOOL AnyPlayerHasKey(INDEX iKey)
  {
    ULONG ulKey = (1<<INDEX(iKey));
    for(INDEX iplt=0; iplt<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); iplt++) {
      CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iplt];          
      if (plt.plt_bActive) {
        CPlayer *penPlayer = (CPlayer *)plt.plt_penPlayerEntity;
        if (penPlayer->m_ulKeys&ulKey) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  void RemoveKeyFromAllPlayers(INDEX iKey)
  {
    ULONG ulKey = (1<<INDEX(iKey));
    for(INDEX iplt=0; iplt<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); iplt++) {
      CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iplt];          
      if (plt.plt_bActive) {
        CPlayer *penPlayer = (CPlayer *)plt.plt_penPlayerEntity;
        penPlayer->m_ulKeys&=~ulKey;        
      }
    }    
  }

procedures:
  Main()
  {
    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_TOUCHMODEL);

    // set appearance
    GetModelObject()->StretchModel(FLOAT3D(m_fWidth, m_fHeight, m_fWidth));
    SetModel(MODEL_DOORCONTROLLER);
    SetModelMainTexture(TEXTURE_DOORCONTROLLER);
    ModelChangeNotify();

    // don't start in wed
    autowait(0.1f);

    // dispatch to aproppriate loop
    switch(m_dtType) {
    case DT_AUTO: {
      jump DoorAuto();
                  } break;
    case DT_TRIGGERED: {
      jump DoorTriggered();
                       } break;
    case DT_TRIGGEREDAUTO: {
      jump DoorTriggeredAuto();
                       } break;
    case DT_LOCKED: {
      jump DoorLocked();
                    } break;
    }
  }

  // entry point for automatic functioning
  DoorAuto()
  {
    // go into active or inactive state
    if (m_bActive) {
      jump DoorAutoActive();
    } else {
      jump DoorAutoInactive();
    }
  }

  // automatic door active state
  DoorAutoActive()
  {
    ASSERT(m_bActive);
    while (TRUE) {
      // wait 
      wait() {
        // when someone enters
        on (EPass ePass) : {
          // if he can open the door
          if (CanReactOnEntity(ePass.penOther)) {
            // do it
            m_penCaused = ePass.penOther;
            TriggerDoor();
              
            // this is a very ugly fix for cooperative not finishing in the demo level
            // remove this when not needed any more!!!!
            if(_SE_DEMO && GetSP()->sp_bCooperative && !GetSP()->sp_bSinglePlayer) {
              if (m_strName=="Appear gold amon") {
                CPlayer *penPlayer = (CPlayer*)&*ePass.penOther;
                penPlayer->SetGameEnd();
              }
            }

            resume;
          }
          resume;
        }
        // if door is deactivated
        on (EDeactivate) : {
          // go to inactive state
          m_bActive = FALSE;
          jump DoorAutoInactive();
        }
        otherwise() : {
          resume;
        };
      };
      
      // wait a bit to recover
      autowait(0.1f);
    }
  }

  // automatic door inactive state
  DoorAutoInactive()
  {
    ASSERT(!m_bActive);
    while (TRUE) {
      // wait 
      wait() {
        // if door is activated
        on (EActivate) : {
          // go to active state
          m_bActive = TRUE;
		  switch(m_dtType)
		  {
		  case DT_AUTO: 
			  {
				jump DoorAuto();
			  }
			  break;
		  case DT_TRIGGERED:
			  {
				jump DoorTriggered();
			  }
			  break;
		  case DT_TRIGGEREDAUTO:
			  {
				jump DoorTriggeredAuto();
			  }
			  break;
		  case DT_LOCKED:
			  {
				jump DoorLocked();
			  }
			  break;
		  default:
			  {
				jump DoorAutoActive();
			  }
		  }
        }
        otherwise() : {
          resume;
        };
      };
      
      // wait a bit to recover
      autowait(0.1f);
    }
  }

  // door when do not function anymore
  DoorDummy()
  {
    wait() {
      on (EBegin) : {
        resume;
      }
      otherwise() : {
        resume;
      };
    }
  }

  // door that wait to be triggered to open
  DoorTriggered()
  {
		while (TRUE) 
		{
      // wait to someone enter
			wait() 
			{
				on (ETrigger eTrigger) : 
				{
					m_penCaused = eTrigger.penCaused;
					
					if (m_bActive)
					{
						TriggerDoor();
					}
					//jump DoorDummy();
					//m_bActive = FALSE;
					//jump DoorAutoInactive();					
					resume;
				}
				on (EPass ePass) : 
				{					
					if (CanReactOnEntity(ePass.penOther)) 
					{
						if (m_strLockedMessage!="") 
						{
						    //PrintCenterMessage(this, ePass.penOther, TranslateConst(m_strLockedMessage), 3.0f, MSS_INFO);
						}
						if (m_penLockedTarget!=NULL) 
						{
							SendToTarget(m_penLockedTarget, EET_TRIGGER, ePass.penOther);
						}
						resume;
					}
				}
				on (EActivate) :
				{
					m_bActive = TRUE;
					resume;
				}

				on (EDeactivate) :
				{
					m_bActive = FALSE;
					resume;
				}

				on (EDoorControll eDoor) :
				{
					if (eDoor.bOpen && !m_bOpened || !eDoor.bOpen && m_bOpened)	// Opening or closing
					{
						TriggerDoor();
						m_bOpened = !m_bOpened;
					}
					resume;
				}

				otherwise() : 
				{
					resume;
				};
			};
      
			// wait a bit to recover
			autowait(0.1f);
		}
  }

  // door that need a key to be unlocked to open
  DoorLocked()
  {
		while (TRUE) 
		{
      // wait to someone enter
			wait() 
			{
				on (EPass ePass) : 
				{
					if( IsDerivedFromClass( ePass.penOther, &CPlayer_DLLClass)) 
					{
			           CPlayer *penPlayer = (CPlayer*)&*ePass.penOther;
						
						// if someone has the key            
					   if(_pNetwork->m_ubGMLevel > 1)
					   {
							CTString strMessage;
							strMessage.PrintF("=====Remain Enemy : %d=====\n", _pNetwork->wo_dwEnemyCount);
							_pNetwork->ClientSystemMessage(strMessage);
							strMessage.PrintF("=====MAX Check Point : %X=====\n", m_iMaxCheckPoint);
							_pNetwork->ClientSystemMessage(strMessage);
							strMessage.PrintF("=====Check Point : %X=====\n", _pNetwork->wo_stCheckPoint.m_iCheckFlag);
							_pNetwork->ClientSystemMessage(strMessage);
							
					   }
//						if( _pNetwork->wo_dwEnemyCount <= m_iOpenMobNum && _pNetwork->m_bSingleMode)
						if( _pNetwork->wo_stCheckPoint.m_iCheckFlag >= m_iMaxCheckPoint && 
							_pNetwork->wo_dwEnemyCount  <= m_iOpenMobNum && 
							_pNetwork->m_bSingleMode)
						{
							// EnemyCount Initialize
							_pNetwork->wo_dwEnemyCount = 0;

							// open the door
							TriggerDoor();

//강동민 수정 시작 다중 공격 작업	08.27
							//m_bOpened = TRUE;

							// 자동문으로 만듬(문제가 될듯함...)
							// become automatic door
							//jump DoorAuto();
							jump DoorAutoInactive();
//강동민 수정 끝 다중 공격 작업		08.27
						}
//강동민 수정 끝 싱글 던젼 작업		07.27
						else if (m_bRaidObject && m_bActive)
						{
							// [091106: selo] 보낸 시간 후 1초 이전에는 서버에 보내지 않는다
							FLOAT tmCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
							if( tmCurTime - m_tmRaidSendedTime > 1000 )
							{
								m_tmRaidSendedTime = tmCurTime;
								_pNetwork->SendRaidObjectEvent(en_ulID);
							}
						}
						// if he has no key
						else 
						{	
							if (m_penLockedTarget!=NULL) 
							{
								SendToTarget(m_penLockedTarget, EET_TRIGGER, ePass.penOther);
							}
						}
						//resume;
					}
					resume;
				}
				on (ETrigger eTrigger) : 
				{
					m_penCaused = eTrigger.penCaused;
					TriggerDoor();
					//jump DoorDummy();
					m_bActive = FALSE;
					jump DoorAutoInactive();					
					resume;
				}
				otherwise() : 
				{
					resume;
				};
			};
      
      // wait a bit to recover
      autowait(0.1f);
    }
  }

  // door that need to be triggered to start working automatically
  DoorTriggeredAuto()
  {
    while (TRUE) {
      // wait to be triggered
      wait() {
        on (ETrigger eTrigger) : {
          // become auto door
          jump DoorAuto();
        }
        on (EPass ePass) : {
          if (CanReactOnEntity(ePass.penOther)) {
            if (m_strLockedMessage!="") {
              //PrintCenterMessage(this, ePass.penOther, TranslateConst(m_strLockedMessage), 3.0f, MSS_INFO);
            }
            if (m_penLockedTarget!=NULL) {
              SendToTarget(m_penLockedTarget, EET_TRIGGER, ePass.penOther);
            }
          }
          resume;
        }
        otherwise() : {
          resume;
        };
      };
      
      // wait a bit to recover
      autowait(0.1f);
    }
  }
};

