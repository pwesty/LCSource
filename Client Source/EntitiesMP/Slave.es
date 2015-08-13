
19589
%{
#include "StdH.h"

#include <Engine/Effect/CEffectGroup.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CSkaEffect.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Network/ItemTarget.h>
#include <deque>
#include <Engine/Entities/Skill.h>
#include <Engine/Network/MessageDefine.h>
#include "EntitiesMP/Common/CharacterDefinition.h"
#include "EntitiesMP/PlayerWeapons.h"
#include <Engine/SlaveInfo.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/Base/ChattingUI.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Info/MyInfo.h>

#define MOB_ATTACK_MOTION_NUM		(1)

void ShotFallDown(FLOAT3D vStartPos, FLOAT3D vToTargetDir, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szFallObjEffectName
				 , bool bCritical);
void GetTargetDirection3(CEntity *penMe, CEntity *penTarget, FLOAT3D &vTargetPos, FLOAT3D &vDirection)
{
	if(penTarget == NULL) {return;}

	if(penTarget == penMe)
	{
		vTargetPos = penMe->en_plPlacement.pl_PositionVector;
		vDirection = FLOAT3D(0,1,1);
		return;
	}
	FLOAT3D vCurrentCenter(((EntityInfo*)(penMe->GetEntityInfo()))->vTargetCenter[0],
	((EntityInfo*)(penMe->GetEntityInfo()))->vTargetCenter[1],
	((EntityInfo*)(penMe->GetEntityInfo()))->vTargetCenter[2] );
	FLOAT3D vCurrentPos = penMe->en_plPlacement.pl_PositionVector + vCurrentCenter;
	FLOAT3D vTargetCenter(0, 0, 0);
	vTargetPos = FLOAT3D(0, 0, 0);
	vDirection = FLOAT3D(0, 0, 0);
	FLOAT size = 0;
	if(penTarget != NULL && penTarget->GetFlags()&ENF_ALIVE)
	{
		if(penTarget->en_RenderType == CEntity::RT_SKAMODEL)
		{
			// Bounding Box를 이용하여 Effect를 찍을 위치를 찾는다.
			FLOATaabbox3D bbox;
			penTarget->GetModelInstance()->GetCurrentColisionBox(bbox);
			FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*단순히 원으로 하면 너무 큼. 사이즈 조절 상수*/;
			size = vRadius.Length();
		}
		vTargetCenter = FLOAT3D(((EntityInfo*)(penTarget->GetEntityInfo()))->vTargetCenter[0],
		((EntityInfo*)(penTarget->GetEntityInfo()))->vTargetCenter[1],
		((EntityInfo*)(penTarget->GetEntityInfo()))->vTargetCenter[2] );
		vTargetPos = penTarget->en_plPlacement.pl_PositionVector + vTargetCenter;
		vDirection = vTargetPos - vCurrentPos;
		vDirection.Normalize();
		vTargetPos -= vDirection * size;
	}
	vDirection.Normalize(); 				
};

ENGINE_API void SetDropItemModel(CEntity *penEntity, const CItemData &ItemData, CItemTarget* pTarget);
%}

uses "EntitiesMP/SlaveBase";
uses "EntitiesMP/Global";
uses "EntitiesMP/BasicEffects";
uses "EntitiesMP/EnemyBase";
uses "EntitiesMP/Enemy";

event ESlaveAttackDamage {//0629
};

%{
// info structure
static EntityInfo eiSlaveBoss = {
	EIBT_FLESH, 200.0f,
	//0.0f, 1.75f*m_fStretch, 0.0f,     // source (eyes)
	//0.0f, 1.30f*m_fStretch, 0.0f,     // target (body)
	0.0f, 1.75f*1.0f, 0.0f,     // source (eyes)
	0.0f, 1.30f*1.0f, 0.0f,     // target (body)
};

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
static INDEX g_iNPCMinBrightness = 32;	//명도 0~240
void CSlave_OnInitClass(void)
{
	_pShell->DeclareSymbol("persistent user INDEX g_iNPCMinBrightness;", &g_iNPCMinBrightness);
}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)


//-----------------------------------------------------------------------------
// Purpose: 컨테이너 내의 모든 엔티티에 한번에 데미지를 줍니다.
// Input  : pEntity - 
//			sTI - 
//			vCenter - 
// Output : static void
//-----------------------------------------------------------------------------
void DamagedTargetsInRange(CEntity* pEntity, CSelectedEntities& dcEntities, enum DamageType dmtType, FLOAT fDamageAmmount, FLOAT3D vCenter, BOOL bSkill);
void ShotMissile(CEntity *penShoter, const char *szShotPosTagName
				 , CEntity *penTarget, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szArrowEffectName
				 , bool bCritical
				 , FLOAT fHorizonalOffset = 0.0f, FLOAT fVerticalOffset = 0.0f	//-1.0f ~ 1.0f
				 , INDEX iArrowType = 0, const char *szMissileModel = NULL, CSelectedEntities* dcEntities = NULL);
void ShotMagicContinued(CEntity *penShoter, FLOAT3D vStartPos, FLOATquat3D qStartRot
				 , CEntity *penTarget, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szMagicEffectName
				 , bool bCritical, INDEX iOrbitType, BOOL bDirectTag = FALSE);
%}

class CSlave: CSlaveBase {
name      "Slave";
thumbnail "Mods\\test.tbn";
features  "ImplementsOnInitClass", "HasName", "IsTargetable";

properties:	
	3 FLOAT m_fBodyAnimTime		= -1.0f,	
	5 FLOAT m_fStretch			= 1.0f,		
	100 INDEX iAnimSlave_Walk		= -1,	
	101 INDEX iAnimSlave_Idle1		= -1,
	102 INDEX iAnimSlave_Idle2		= -1,
	103 INDEX iAnimSlave_Run			= -1,
	104 INDEX iAnimSlave_Death		= -1,	
	105 INDEX iAnimSlave_Wound		= -1,
	106 INDEX iAnimSlave_Attack		= -1,		
	107 INDEX iAnimSlave_NormalBox	= -1,		
	// 소셜 애니메이션.
	109 INDEX iAnimSlave_Skill01		= -1,
	110 INDEX iAnimSlave_Skill02		= -1,	
	120 INDEX iAnimSlave_Attack02		= -1,	
	121	INDEX iAttackAnim				= -1,	

	200	CTString	m_strName	" Name" = "Slave",			
	247 enum EventType	m_EventType			"Event Type"		= EVENT_NONE,
	248 CEntityPointer m_penEventTarget		"Event Target",					// 트리거를 통해서 Blind를 해제하도록 함.	
	251 BOOL	m_bIsRun					= FALSE,
	252 BOOL	m_bDie	= FALSE,
	253 INDEX					m_nSkillChack = 0,			// 소환수 스킬 예약
	//253 FLOAT m_fIdleTime = 0.0f,		
	{
		INDEX					m_nEffectStep;	
		CEffectGroup			*m_pSkillReadyEffect;
		CStaticArray<TagInfo>	m_saLastEffectInfo;
		sSkillEffectInfo		m_SkillEffectInfo;								
		
		//CSelectedEntities		m_dcEnemies;

		BOOL					m_bPlayAttackAnim;
	}

components:	
	// 사운드 처리가 가장 문제되는 부분.
functions:

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	BOOL AdjustShadingParameters(FLOAT3D &vLightDirection, COLOR &colLight, COLOR &colAmbient)
	{
		//이 주석을 풀면 몹사망시 Fade-out처리가 된다. 단 약간 튀는 듯한 문제 있음.
		//CSlaveBase::AdjustShadingParameters(vLightDirection, colLight, colAmbient);
		g_iNPCMinBrightness = Clamp(g_iNPCMinBrightness, (INDEX)0, (INDEX)240);
		BOOL bRecalcAmbient = FALSE;
		UBYTE ubAH, ubAS, ubAV;
		ColorToHSV(colAmbient, ubAH, ubAS, ubAV);
		if(ubAV < g_iNPCMinBrightness)
		{
			UBYTE ubAR, ubAG, ubAB;
			ColorToRGB(colAmbient, ubAR, ubAG, ubAB);
			ubAR = g_iNPCMinBrightness;
			ubAG = g_iNPCMinBrightness;
			ubAB = g_iNPCMinBrightness;
			ColorToHSV(RGBToColor(ubAR, ubAG, ubAB), ubAH, ubAS, ubAV);
			bRecalcAmbient = TRUE;
		}
		if(bRecalcAmbient) {colAmbient = HSVToColor(ubAH, ubAS, ubAV);}
		return TRUE;
		//return CSlaveBase::AdjustShadingParameters(vLightDirection, colLight, colAmbient);
	}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

	virtual FLOAT GetLockRotationSpeed(void) 
	{ 
		return AngleDeg(1800.0f); 
	};	

	void OnInitialize(const CEntityEvent &eeInput)
	{
		CSlaveBase::OnInitialize(eeInput);
	};

	void OnEnd(void) 
	{
		CSlaveBase::OnEnd();		
	};

	void SetCurrentCommand( INDEX iCommand )
	{
//		if( m_bPlayAttackAnim )					{ return; } 				
		if( m_iCurrentCommand == iCommand )		{ return; }

		if( m_iCurrentCommand == CSlaveInfo::COMMAND_HOLD && 
			m_iCurrentCommand != iCommand )
		{
			SendEvent(ESlaveBaseReconsiderBehavior());			
		}
		
		//m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bStop = FALSE;
		StandingAnim();
		SetTargetNone();

		SetTargetByCommand( iCommand );
	};

	// FIXME : 중복되는 부분이 많은 코드임.  정리 좀 하삼~~~!!!
	virtual BOOL UseSkill( int iSkillIndex )
	{
		// 공격 애니메이션 중에는 스킬을 사용할 수 없음.
		if( m_bPlayAttackAnim )	
		{ 
			m_nSkillChack = iSkillIndex;   //ttos : 소환수 스킬 예약함
			return FALSE; 
		}	
		// 스킬 딜레이 적용하기.
		CSkill &SkillData = _pNetwork->GetSkillData(iSkillIndex);
		int lv = MY_INFO()->GetSkillLevel(iSkillIndex);

		m_dcEnemies.Clear();

		// 버프가 걸린 상태에서 스킬 사용 불가.
		if( m_bCannotUseSkill ) { return FALSE; }		

		BOOL bHelp = SkillData.GetFlag() & SF_FORHELP;		

		// 타겟이 필요한 스킬
		if( SkillData.IsNeedTarget() )
		{
			if( !m_penEnemy )		{ return FALSE; }
			if( m_penEnemy && m_penEnemy->GetFlags() & ENF_ALIVE)
			{
				// HELP 스킬이 아닌데 플레이어를 공격 할수는 없음.
				if( !bHelp && m_penEnemy->IsPlayer() )
				{
					return FALSE;
				}

				FLOAT3D vDelta = GetPlacement().pl_PositionVector - m_penEnemy->GetPlacement().pl_PositionVector;
				if( vDelta.Length() > SkillData.GetFireRange() )
				{
					return FALSE;
				}

				// 여기서 범위 공격 처리해줄것...
				if( SkillData.GetTargetNum(lv) > 1 )
				{
					if( _pNetwork->m_bSingleMode )
					{
						CPlacement3D Myplacement = GetPlacement();					
						_pNetwork->SendMoveMessage(this, Myplacement, m_fMoveSpeed);
					}

					// 주 타겟 에너미.								
					m_dcEnemies.Clear();
					m_dcEnemies.Add(m_penEnemy);	// this need to ready status.

					_pNetwork->SendSlaveSkillMessageInContainer(iSkillIndex, this, m_dcEnemies, FALSE);

					const char chTargetType = SkillData.GetTargetType();
					FLOAT fAngle = 360.0f;
					// 자기 중심으로 120도
					if( chTargetType == CSkill::STT_TARGET_D120 )
					{
						fAngle = 60.0f;
					}
					
					m_dcEnemies.Add(m_penEnemy);	// this need to fire status of target member.

					_pNetwork->FindTargetsInRange(this, m_penEnemy, m_dcEnemies, 
													SkillData.GetAppRange(), SkillData.GetTargetNum(lv)-1, 360.0f, iSkillIndex, chTargetType );
					
					_pNetwork->SendSlaveSkillMessageInContainer(iSkillIndex, this, m_dcEnemies, TRUE);
				}
				else
				{
					if( _pNetwork->m_bSingleMode )
					{
						CPlacement3D Myplacement = GetPlacement();					
						_pNetwork->SendMoveMessage(this, Myplacement, m_fMoveSpeed);
					}

					_pNetwork->SendSlaveSkillMessage( iSkillIndex, this, m_penEnemy, FALSE);
					_pNetwork->SendSlaveSkillMessage( iSkillIndex, this, m_penEnemy, TRUE);
				}
			}			
		}
		// 셀프 스킬
		else
		{
			if( SkillData.GetTargetNum(lv) > 1 )
			{
				if( _pNetwork->m_bSingleMode )
				{
					CPlacement3D Myplacement = GetPlacement();					
					_pNetwork->SendMoveMessage(this, Myplacement, m_fMoveSpeed);
				}

				// 주 타겟 에너미.	
				m_dcEnemies.Clear();
				m_dcEnemies.Add(this);

				_pNetwork->SendSlaveSkillMessageInContainer(iSkillIndex, this, m_dcEnemies, FALSE);

				const char chTargetType = SkillData.GetTargetType();

				// 자기 중심으로 멀티 공격
				if( chTargetType == CSkill::STT_SELF_RANGE )
				{
					_pNetwork->FindTargetsInRange(this, this, m_dcEnemies, 
						SkillData.GetAppRange(), SkillData.GetTargetNum(lv), 360.0f, iSkillIndex );
				}

				_pNetwork->SendSlaveSkillMessageInContainer(iSkillIndex, this, m_dcEnemies, TRUE);
			}
			else
			{
				if( _pNetwork->m_bSingleMode )
				{
					CPlacement3D Myplacement = GetPlacement();					
					_pNetwork->SendMoveMessage(this, Myplacement, m_fMoveSpeed);
				}

				_pNetwork->SendSlaveSkillMessage( iSkillIndex, this, this, FALSE);
				_pNetwork->SendSlaveSkillMessage( iSkillIndex, this, this, TRUE);
			}
		}

		return TRUE;
	};

	//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	void GetTargetDirection(FLOAT3D &vTargetPos, FLOAT3D &vDirection)
	{
		if(m_penEnemy.ep_pen != NULL)
		{
			CEntity *pen = m_penEnemy;
			FLOAT3D vCurrentCenter(((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[0],
			((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[1],
			((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[2] );
			FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vCurrentCenter;
			FLOAT3D vTargetCenter(0, 0, 0);
			vTargetPos = FLOAT3D(0, 0, 0);
			vDirection = FLOAT3D(0, 0, 0);
			FLOAT size = 0;
			if(pen != NULL && pen->GetFlags()&ENF_ALIVE)
			{
				if(pen->en_RenderType == RT_SKAMODEL)
				{
					// Bounding Box를 이용하여 Effect를 찍을 위치를 찾는다.
					FLOATaabbox3D bbox;
					pen->GetModelInstance()->GetCurrentColisionBox(bbox);
					FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*단순히 원으로 하면 너무 큼. 사이즈 조절 상수*/;
					size = vRadius.Length();
				}
				vTargetCenter = FLOAT3D(((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[0],
				((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[1],
				((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[2] );
				vTargetPos = pen->en_plPlacement.pl_PositionVector + vTargetCenter;
				vDirection = vTargetPos - vCurrentPos;
				vDirection.Normalize();
				vTargetPos -= vDirection * size;
			}
			vDirection.Normalize(); 				
		}
	}

	// 소환수가 움직일 수 있는 조건인지 판단합니다.
	virtual BOOL IsMovable()
	{
		// 내 소환수이며...
		if(m_bUseAI)
		{
			// 방어상태인지 확인함.
			if( m_iCurrentCommand == CSlaveInfo::COMMAND_HOLD || m_bStuned || m_bHold )
			{
				return FALSE;
			}
		}
		return TRUE;
	};
	
	// describe how this enemy killed player
	virtual CTString GetPlayerKillDescription(const CTString &strPlayerName, const EDeath &eDeath)
	{
		CTString str;
		str.PrintF(TRANS("A %s sent %s into the halls of Valhalla") ,m_strName, strPlayerName);
		return str;
	}
	
	/* Entity info */
	void *GetEntityInfo(void)
	{
		return &eiSlaveBoss;
	};

	void Precache(void) 
	{
		CSlaveBase::Precache();

//안태훈 수정 시작	//(Fix Snd Effect Bug)(0.1)
		if(en_pmiModelInstance) { en_pmiModelInstance->m_tmSkaTagManager.SetOwner(this); }
//안태훈 수정 끝	//(Fix Snd Effect Bug)(0.1)
	};

	virtual BOOL IsSlave(void) const {return TRUE;	}

	// 펫이 데미지를 입힐경우...
	void InflictDirectDamage(CEntity *penToDamage, CEntity *penInflictor, enum DamageType dmtType,
	FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
	{
		if(penToDamage)
		{
			SE_Get_UIManagerPtr()->ShowDamage( penToDamage->en_ulID );

			if( !(penToDamage->GetFlags()&ENF_ALIVE ) )	{ return; }
			INDEX preHealth, curHealth;
			preHealth = ((CUnit*)penToDamage)->m_nPreHealth;
			curHealth = ((CUnit*)penToDamage)->m_nCurrentHealth;

			if(preHealth!= -1)// && curHealth > preHealth)
			{
				fDamageAmmount = 1;
				((CUnit*)penToDamage)->m_nCurrentHealth = ((CUnit*)penToDamage)->m_nPreHealth;			
			}
			else 
			{
				fDamageAmmount = 0;
			}

			if(penToDamage ==INFO()->GetTargetEntity(eTARGET))
			{
				if( penToDamage->IsCharacter() )
				{
					penToDamage->UpdateTargetInfo(((CUnit*) penToDamage)->m_nMaxiHealth,((CUnit*) penToDamage)->m_nCurrentHealth,((CCharacter*) penToDamage)->m_nPkMode,((CCharacter*) penToDamage)->m_nPkState,((CCharacter*) penToDamage)->m_nLegit);		
				}
				else
				{
					penToDamage->UpdateTargetInfo(((CUnit*) penToDamage)->m_nMaxiHealth,((CUnit*) penToDamage)->m_nCurrentHealth);
				}
			}
	
	
			if (penToDamage->IsEnemy() && preHealth >= 0) // ?? Slave가 Enemy(권좌)를 공격할 일이 있는가?
			{
				const INDEX iMobIndex = ((CEnemy*)penToDamage)->m_nMobDBIndex;

				if (iMobIndex == LORD_SYMBOL_INDEX) // 메라크 공성 권좌 NPC(사이즈는 바뀌지 않고, 모델을 바꾼다)
				{
					CMobData* MD = CMobData::getData(iMobIndex);
					FLOAT fMaxHealth = ((CUnit*)penToDamage)->m_nMaxiHealth;

					if ((curHealth <= fMaxHealth * 0.25f))
					{
						if (((CUnit*)penToDamage)->m_nPlayActionNum != 0)
						{
							penToDamage->SetSkaModel("data\\npc\\Gguard\\sword04.smc");
							penToDamage->GetModelInstance()->RefreshTagManager();
							penToDamage->GetModelInstance()->StretchModel(FLOAT3D(MD->GetScale(), MD->GetScale(), MD->GetScale()));
							((CUnit*)penToDamage)->m_nPlayActionNum = 0;
						}
					}
					else if ((curHealth <= fMaxHealth * 0.50f))
					{
						if (((CUnit*)penToDamage)->m_nPlayActionNum != 1)
						{
							penToDamage->SetSkaModel("data\\npc\\Gguard\\sword03.smc");
							penToDamage->GetModelInstance()->RefreshTagManager();
							penToDamage->GetModelInstance()->StretchModel(FLOAT3D(MD->GetScale(), MD->GetScale(), MD->GetScale()));
							((CUnit*)penToDamage)->m_nPlayActionNum = 1;
						}
					}
					else if ((curHealth <= fMaxHealth * 0.75f))
					{
						if (((CUnit*)penToDamage)->m_nPlayActionNum != 2)
						{
							penToDamage->SetSkaModel("data\\npc\\Gguard\\sword02.smc");
							penToDamage->GetModelInstance()->RefreshTagManager();
							penToDamage->GetModelInstance()->StretchModel(FLOAT3D(MD->GetScale(), MD->GetScale(), MD->GetScale()));
							((CUnit*)penToDamage)->m_nPlayActionNum = 2;
						}
					}
				}
			}

			((CUnit*)penToDamage)->m_nPreHealth = -1; //1103
		}
		
		if(penToDamage)
		{
			CEntity::InflictDirectDamage(penToDamage, penInflictor, dmtType,
				fDamageAmmount,vHitPoint, vDirection);	
		}

		//((CLiveEntity*)((CEntity*)penToDamage))->SetHealth(1000.0f);

		if(penToDamage )
		{
			if(((CUnit*)penToDamage)->m_nCurrentHealth <= 0 && penToDamage != this)
			{
				((CUnit*)((CEntity*)penToDamage))->DeathNow();				
			}
		}
	};

	//--------------------------------------------------------------
	// 데미지를 입었을 경우의 처리...
	//--------------------------------------------------------------
	/* Receive damage */
	void ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection) 
	{
		if(dmtType != DMT_NONE && fDamageAmmount > 0)
		{
			FLOAT3D vAxisY(0,1,0);
			FLOAT angle = acos(vDirection % vAxisY);
			FLOAT3D axis = vAxisY * vDirection;
			axis.Normalize();
			FLOATquat3D quat;
			quat.FromAxisAngle(axis, angle);
			StartEffectGroup("Normal Hit"		//Hardcording
				, _pTimer->GetLerpedCurrentTick(), vHitPoint, quat);
		}
		SE_Get_UIManagerPtr()->ShowDamage( en_ulID );
		CSlaveBase::ReceiveDamage(penInflictor, dmtType, fDamageAmmount, vHitPoint, vDirection);
	};

	//--------------------------------------------------------------
	// 몬스터의 애니메이션을 설정하는 함수들...
	//--------------------------------------------------------------
	// damage anim
	INDEX AnimForDamage(FLOAT fDamage) 
	{
		ASSERT(iAnimSlave_Wound		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		//0820 가만히 서있을때만 데미지 모션이 나와야 된다. 꼭!
		if(!IsAnimationPlaying(iAnimSlave_Idle1) || !IsAnimationPlaying(iAnimSlave_Idle2) )
		{
			INDEX animSet, animIdx;
			static INDEX animId = ska_StringToID("mldam");//HardCoding
			if(this->en_pmiModelInstance
			&& this->en_pmiModelInstance->FindAnimationByID(animId, &animSet, &animIdx))
			{
				//타격 모션
				this->en_pmiModelInstance->AddAnimation(animId, AN_REMOVE_ON_END, 1.0f, 0);
			}
			return animId;
		}

		NewClearState(CLEAR_STATE_LENGTH);    
		GetModelInstance()->AddAnimation(iAnimSlave_Wound,AN_CLEAR,1,0);	
		
		return iAnimSlave_Wound;
	};

	// death
	INDEX AnimForDeath(void)
	{	
		ASSERT(iAnimSlave_Death		!= -1	&& "Invalid Animation");
		
				
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		NewClearState(CLEAR_STATE_LENGTH);    
		GetModelInstance()->AddAnimation(iAnimSlave_Death,AN_NORESTART,1,0);

		return iAnimSlave_Death;
	};

	// 죽을때 뭔가가...
	FLOAT WaitForDust(FLOAT3D &vStretch)
	{
		//0317 kwon
		vStretch=FLOAT3D(1,1,1);		
		if(GetModelInstance()->IsAnimationPlaying(iAnimSlave_Death)) 
		{
			return 0.5f;
		} 
		return -1.0f;
	};

	void DeathNotify(void) 
	{
		// 몸의 비중...
		en_fDensity = 500.0f;

		if(m_EventType == EVENT_MOBDESTROY)
		{
			SendToTarget(m_penEventTarget, EET_TRIGGER, this);
		}		
	};

	// virtual anim functions
	void StandingAnim(void) 
	{		
		m_bIsRun = FALSE;
		ASSERT(iAnimSlave_Idle1	!= -1	&& "Invalid Animation");		
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");		
		GetModelInstance()->AddAnimation(iAnimSlave_Idle1,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);	

	};

	void RunningAnim(void) 
	{
		m_bIsRun = TRUE;
		ASSERT(iAnimSlave_Run		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(iAnimSlave_Run,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
	};

	void WalkingAnim(void) 
	{
		m_bIsRun = FALSE;
		ASSERT(iAnimSlave_Walk		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(iAnimSlave_Walk,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);		
	};

	void RotatingAnim(void) 
	{
		if(m_bIsRun)
		{
			RunningAnim();
		}
		else
		{
			WalkingAnim();
		}
	};

	void AttackAnim(void) 
	{		
		float frandom = FRnd();
		if( frandom < 0.5f )
		{
			iAttackAnim = iAnimSlave_Attack;
		}
		else
		{
			iAttackAnim = iAnimSlave_Attack02;
		}

		ASSERT(iAnimSlave_Attack		!= -1		&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL	&& "Invalid ModelInstance");

		static FLOAT	start_attack_time=0;
		static float  m_fBodyAnimTime=0;
		
		if(start_attack_time==0)
		{
			start_attack_time = _pTimer->CurrentTick();
			GetModelInstance()->AddAnimation(iAttackAnim,AN_NORESTART|AN_CLEAR,1.0f,0);
			m_fBodyAnimTime = GetModelInstance()->GetAnimLength(iAttackAnim);
		}
		if(start_attack_time !=0 && (_pTimer->CurrentTick() - start_attack_time >= m_fBodyAnimTime))
		{
			m_bAttack = FALSE;	
			start_attack_time = 0;		
		}
	};

	// adjust sound and watcher parameters here if needed
	void EnemyPostInit(void) 
	{
		// set sound default parameters
		m_soSound.Set3DParameters(30.0f, 5.0f, 1.0f, 1.0f);
	};

	
//안태훈 수정 시작	//(5th Closed beta)(0.2)	
	void SkillAndAttackFire(int num)
	{
		//ULONG MobIndex = penEntity->en_ulID;

		if( !(m_penEnemy.ep_pen != NULL
			&& m_penEnemy->en_pmiModelInstance != NULL
			&& m_penEnemy->GetFlags() & ENF_ALIVE) )
		{
			return;
		}

		this->InflictDirectDamage(m_penEnemy, this, DMT_NONE, 1, FLOAT3D(0,0,0), FLOAT3D(0,0,0));
		switch(m_SkillEffectInfo.iMissileType)
		{
		case 0/*MT_NONE*/:
			{
				FLOAT3D vHitPoint;
				FLOAT3D vHitDir;
				GetTargetDirection3(this, m_penEnemy, vHitPoint, vHitDir);

				if( m_dcEnemies.Count() > 0 )
				{
					DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, vHitPoint, TRUE);
					m_dcEnemies.Clear();
				}
				else
				{
					//damage effect 처리
					this->InflictDirectDamage(m_penEnemy, this, DMT_NONE, 1, vHitPoint, vHitDir);
				}

				FLOAT3D vAxisY(0,1,0);
				FLOAT angle = acos(vHitDir % vAxisY);
				FLOAT3D axis = vAxisY * vHitDir;
				axis.Normalize();
				FLOATquat3D quat;
				quat.FromAxisAngle(axis, angle);
				StartEffectGroup(m_SkillEffectInfo.szEffectNameHit
								, _pTimer->GetLerpedCurrentTick()
								, vHitPoint, quat);
			} break;
		case 1/*MT_ARROW*/:
			{
				if( m_dcEnemies.Count() > 0 )
				{
					for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
						it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
					{																		
						CEntity *pEn = (*it);
						if(pEn != NULL && pEn->IsFlagOff(ENF_DELETED))
						{
							ShotMissile( this, "RHAND", pEn, m_SkillEffectInfo.fMissileSpeed, "Normal Hit", "Normal Arrow Trace", false );
						}
					}
				}
				else
				{
					ShotMissile( this, "RHAND", m_penEnemy, m_SkillEffectInfo.fMissileSpeed, "Normal Hit", "Normal Arrow Trace", false );
				}
			} break;
		case 2/*MT_DIRECT*/:
			{
				StartEffectGroup(m_SkillEffectInfo.szEffectNameHit
					, &m_penEnemy->en_pmiModelInstance->m_tmSkaTagManager
					, _pTimer->GetLerpedCurrentTick());
			} break;
		case 3/*MT_CONTINUE*/:
			{
				FLOAT3D lastEffectInfo = m_penEnemy->en_plPlacement.pl_PositionVector;
				lastEffectInfo(2) += 1;
				if(m_pSkillReadyEffect != NULL && CEffectGroupManager::Instance().IsValidCreated(m_pSkillReadyEffect))
				{
					for(INDEX i=0; i<m_pSkillReadyEffect->GetEffectCount(); ++i)
					{
						CEffect *pEffect = m_pSkillReadyEffect->GetEffectKeyVector()[i].m_pCreatedEffect;
						if(pEffect != NULL && pEffect->GetType() == ET_SKA)
						{
							CSkaEffect *pSkaEffect = (CSkaEffect*)pEffect;
							lastEffectInfo = pSkaEffect->GetInstancePosition();
							break;
						}
					}
					if(num == m_SkillEffectInfo.iFireDelayCount)
					{
						m_pSkillReadyEffect->Stop(0.04f);
					}
				}
				if( m_dcEnemies.Count() > 0 )
				{
					for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
						it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
					{
						CEntity *pEn = (*it);
						if(pEn != NULL && pEn->IsFlagOff(ENF_DELETED))
						{
							ShotMagicContinued(this, lastEffectInfo, FLOATquat3D(1,0,0,0)
										, pEn, m_SkillEffectInfo.fMissileSpeed
										, m_SkillEffectInfo.szEffectNameHit, m_SkillEffectInfo.szEffectNameMissile
										, false, 3);
						}
					}
				}
				else
				{
					ShotMagicContinued(this, lastEffectInfo, FLOATquat3D(1,0,0,0)
										, m_penEnemy, m_SkillEffectInfo.fMissileSpeed
										, m_SkillEffectInfo.szEffectNameHit, m_SkillEffectInfo.szEffectNameMissile
										, false, 3);
				}
			} break;
		case 4/*MT_INVISIBLE*/:		{} break;//안쓰임. 캐릭터, 플레이어쪽도 미구현.
		case 5/*MT_MAGIC*/:			{} break;//아직 안쓰임
		case 6/*MT_INVERT*/:		{} break;//아직 안쓰임
		case 7/*MT_MAGECUTTER*/:	{} break;//안쓰임, 캐릭 전용
		case 8/*MT_DIRECTDAMAGE*/:
			{
				StartEffectGroup(m_SkillEffectInfo.szEffectNameHit
					, &m_penEnemy->en_pmiModelInstance->m_tmSkaTagManager
					, _pTimer->GetLerpedCurrentTick());
				if( m_dcEnemies.Count() > 0 )
				{
					DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, FLOAT3D(0,0,0), TRUE);
					m_dcEnemies.Clear();
				}
				else
				{
					this->InflictDirectDamage( m_penEnemy, this, DMT_NONE, 1, FLOAT3D(0,0,0), FLOAT3D(0,0,0) );
				}
			} break;
		case 9/*MT_NOTHING*/:		{} break;
		//-----boss mob hardcoding area begin-----//
		case 21://baal skill 01, like meteo strike
			{
				CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
				static INDEX s_iFireBallCount = 10;
				static FLOAT s_fFallHeight = 3;
				static FLOAT s_fFallHeightVariation = 3;
				static FLOAT s_fSpeed = 5;
				static FLOAT s_fSpeedVariation = 5;
				for(INDEX i=0; i<s_iFireBallCount; ++i)
				{
					FLOAT3D pos = CRandomTable::Instance().GetRndCylinderPos() * skill.GetAppRange();
					pos(2) = CRandomTable::Instance().GetRndFactor() * s_fFallHeightVariation + s_fFallHeight;
					pos += m_penEnemy->GetPlacement().pl_PositionVector;
					ShotFallDown(pos, FLOAT3D(0,1,0), s_fSpeed + s_fSpeedVariation * CRandomTable::Instance().GetRndFactor()
								, m_SkillEffectInfo.szEffectNameHit, m_SkillEffectInfo.szEffectNameMissile, FALSE);
				}
			} break;
		//-----boss mob hardcoding area end-----//
		default:
			{
				ASSERTALWAYS("Something wrong... Plz check fire obj type of this skill.");
			} break;
		}
	}

procedures:
/************************************************************
 *                       M  A  I  N                         *
 ************************************************************/
	Main(EVoid) 
	{
		CModelInstance *pMI = GetModelInstance();
		if(pMI == NULL)
		{ 
			InitAsSkaEditorModel();
			SetSkaModel("Models\\Editor\\Ska\\Axis.smc");			
			return EEnd();
		}
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
		if(pMI != NULL)
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			pMI->m_tmSkaTagManager.Register(&tag);
			FLOATaabbox3D aabb;
			pMI->GetAllFramesBBox(aabb);
			tag.SetName("CENTER");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			tag.SetOffsetPos(0, aabb.Size()(2) * 0.5f * pMI->mi_vStretch(2), 0);
			pMI->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			tag.SetOffsetPos(0, aabb.Size()(2) * pMI->mi_vStretch(2), 0);
			pMI->m_tmSkaTagManager.Register(&tag);
		}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)		
		
		SetFlags(GetFlags()|ENF_ALIVE);		
		
		en_sbNetworkType = MSG_CHAR_ELEMENTAL;

		SetPhysicsFlags(EPF_ONBLOCK_CLIMBORSLIDE
			| EPF_TRANSLATEDBYGRAVITY
			//| EPF_PUSHABLE
			| EPF_MOVABLE			
			| EPF_ABSOLUTETRANSLATE );
		//SetPhysicsFlags(GetPhysicsFlags() &~EPF_PUSHABLE);
		SetCollisionFlags(ECF_MODEL_NO_COLLISION);
		
		// set initial vars
		en_tmMaxHoldBreath = 60.0f;
		en_fDensity = 1000.0f;    // same density as water - to be able to dive freely		

		SetFlagOn(ENF_RENDERREFLECTION);

		// setup moving speed
		m_aWalkRotateSpeed		= AngleDeg(3600.0f);
		m_aAttackRotateSpeed	= AngleDeg(3600.0f);				
		m_aCloseRotateSpeed		= AngleDeg(3600.0f);

		// setup attack distances
		m_fAttackFireTime		= 2.0f;
		m_fCloseFireTime		= 2.0f;
		m_fBlowUpAmount			= 80.0f;
		m_fBodyParts			= 4;
		m_fDamageWounded		= 0.0f;
		
		// set stretch factors for height and width
		pMI->StretchModel(FLOAT3D(m_fStretch, m_fStretch, m_fStretch));
		pMI->mi_bRenderShadow = TRUE;

		SetHealth(10000.0f);//0807 몹의 체력.
		ModelChangeNotify();
		//StandingAnim();

		m_iCurrentCommand	= CSlaveInfo::COMMAND_PROTECTION;
		m_bPlayAttackAnim	= FALSE;
		
//안태훈 수정 시작	//(Open beta)(2004-12-14)
		if(pMI)
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			pMI->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			FLOATaabbox3D aabb;
			pMI->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2), 0);
			pMI->m_tmSkaTagManager.Register(&tag);
		}
//안태훈 수정 끝	//(Open beta)(2004-12-14)
		
		// 수호 모드의 경우.
		if(m_bUseAI && m_iCurrentCommand == CSlaveInfo::COMMAND_PROTECTION)
		{
			SetTargetByCommand( m_iCurrentCommand );
		}

		// continue behavior in base class
		jump CSlaveBase::MainLoop();
	};

/************************************************************
 *                A T T A C K   E N E M Y                   *
 ************************************************************/
	Fire(EVoid) : CSlaveBase::Fire
	{
		jump CSlaveBase::Fire();
	}

	Hit(EVoid) : CSlaveBase::Hit 
	{		
		autocall NormalAttack() EReturn;		
		return EReturn();
	};
		
	//0628 kwon
	AttackTarget(EVoid) : CSlaveBase::AttackTarget // 이 프로시저를 call하기전에 SetTargetEntity()가 선행되어야 한다.
	{
		m_vDesiredPosition = PlayerDestinationPos();
		if(m_nSkillChack)
		{
			UseSkill(m_nSkillChack);
			return EReturn();
		}
		m_bPlayAttackAnim = TRUE;
		
		INDEX attackAnimID = -1;		
		attackAnimID = iAnimSlave_Attack;
		autocall AttackAnimation() EReturn;
		m_bPlayAttackAnim = FALSE;
		return EReturn();
	};

	SkillingTarget(EVoid) : CSlaveBase::SkillingTarget // 이 프로시저를 call하기전에 SetTargetEntity()가 선행되어야 한다.
	{
		m_vDesiredPosition = PlayerDestinationPos();
		
		CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
		m_SkillEffectInfo.InitForSkillAttack(skill);

		autocall SkillAnimation() EReturn;
	
		return EReturn();
	};	

	SpellSkill(EVoid) : CSlaveBase::SpellSkill
	{		
		m_vDesiredPosition = PlayerDestinationPos();
	
		autocall PreSkillAnimation() EReturn;
	
		return EReturn();
	};

	AttackAnimation(EVoid) //0628
	{		
		float frandom = FRnd();
		if( frandom < 0.5f )
		{
			iAttackAnim = iAnimSlave_Attack;
		}
		else
		{
			iAttackAnim = iAnimSlave_Attack02;
		}

		GetModelInstance()->AddAnimation(iAttackAnim, AN_CLEAR, 1.0f, 0);			

		AddAnimation(iAttackAnim, AN_CLEAR, 1.0f, 0, -1, m_fAttackSpeedMul);

		m_fAttackFrequency = 0.25f;
		m_fAttackStartTime = _pTimer->CurrentTick();
		m_fImpactTime = GetAnimLength(iAttackAnim)*0.8f/2 * m_fAttackLengthMul;//0630 우선 공격 애니메이션의 절반에서 임팩트.

		while(_pTimer->CurrentTick() - m_fAttackStartTime < GetAnimLength(iAttackAnim)*0.8f*m_fAttackLengthMul){

			wait(m_fAttackFrequency) 
			{
				on (EBegin) : 
				{
					/*					
					if(m_bStop)
					{			
						StandingAnim();
						return EReturn();
					}
					*/

					m_fMoveSpeed = 0.0f; 
					ULONG ulFlags = SetDesiredMovement();

					if(_pTimer->CurrentTick() - m_fAttackStartTime >m_fImpactTime - 0.1f) 
					{
						// 원거리로 공격하는 소환수의 경우.
						if(m_iSlaveType == CSlaveInfo::SLAVE_WATER)
						{
							if( m_penEnemy && iAttackAnim == iAnimSlave_Attack )
							{
								ShotMissile(this, "RHAND", m_penEnemy.ep_pen, 40.0f
											, "swater_att01_hit", "swater_att01_mis", false);	//Hardcoding
							}
							else if( m_penEnemy && iAttackAnim == iAnimSlave_Attack02 )
							{
								FLOAT3D		vStartPos;
								FLOATquat3D	qStartRot;	
								CTag		*pTag = NULL;

								pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
								if( pTag != NULL )
								{
									vStartPos = pTag->CurrentTagInfo().m_vPos;
									qStartRot = pTag->CurrentTagInfo().m_qRot;
								}
								if( pTag != NULL )
								{
									ShotMagicContinued( this, vStartPos, qStartRot, m_penEnemy.ep_pen, 35.0F,
														"swater_att02_hit", "swater_att02_mis",
														false, 0 );
								}
							}
							
							if( m_penEnemy )
							{
								SE_Get_UIManagerPtr()->ShowDamage( m_penEnemy->en_ulID );
							}
						}
						// 근거리로 공격하는 경우.
						else
						{
							if( m_penEnemy )
							{
								SE_Get_UIManagerPtr()->ShowDamage( m_penEnemy->en_ulID );
							}
							SendEvent(ESlaveAttackDamage());						
						}
						m_fImpactTime = 1000.0f;//충분히 긴시간.
					}

					resume; 
				}
				on (ESlaveBaseDamage eEBDamage) : 
				{
					// if confused
					m_fDamageConfused -= eEBDamage.fAmount;
					if (m_fDamageConfused < 0.001f) 
					{
						m_fDamageConfused = m_fDamageWounded;
						// play wounding animation
						INDEX animSet, animIdx;
						static INDEX animId = ska_StringToID("mldam");//HardCoding
						if(this->en_pmiModelInstance
						&& this->en_pmiModelInstance->FindAnimationByID(animId, &animSet, &animIdx))
						{
							//타격 모션
							this->en_pmiModelInstance->AddAnimation(animId, AN_REMOVE_ON_END, 1.0f, 0);
						}
					}
					resume;
				}
				on (ESlaveAttackDamage eAttackDamage) : 
				{
						CEntity *pen = m_penEnemy;
						FLOAT3D vCurrentCenter(((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[0],
							((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[1],
							((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[2] );
						FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vCurrentCenter;
						FLOAT3D vTargetCenter(0, 0, 0);
						FLOAT3D vTargetPos(0, 0, 0);
						FLOAT3D vDirection(0, 0, 0);
						FLOAT size = 0;
						if(pen != NULL && pen->GetFlags()&ENF_ALIVE)
						{
							if(pen->en_RenderType == RT_SKAMODEL)
							{
								// Bounding Box를 이용하여 Effect를 찍을 위치를 찾는다.
								FLOATaabbox3D bbox;
								pen->GetModelInstance()->GetCurrentColisionBox(bbox);
								FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*단순히 원으로 하면 너무 큼. 사이즈 조절 상수*/;
								size = vRadius.Length();
							}
							vTargetCenter = FLOAT3D(((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[0],
								((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[1],
								((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[2] );
							vTargetPos = pen->en_plPlacement.pl_PositionVector + vTargetCenter;
							vDirection = vTargetPos - vCurrentPos;
							vDirection.Normalize();
							vTargetPos -= vDirection * size;
						}
						vDirection.Normalize();

//강동민 수정 시작 싱글 던젼 작업	07.25
					if( m_penEnemy )
					{
						// Enemy에 ReceiveDamage를 전송합니다.
						// ReceiveDamage에서 Health가 0보다 작으면 엔티티를 Destroy하는 부분이 있음.
						InflictDirectDamage(m_penEnemy, this , DMT_CLOSERANGE, (int)1, vTargetPos, vDirection);
						// FIXME:
						//((CLiveEntity*)((CEntity*)m_penEnemy))->SetHealth(1000.0f);						
					}
//강동민 수정 끝 싱글 던젼 작업		07.25
					m_fImpactTime = 1000.0f;//충분히 긴시간.
					stop;
				}
				on (ETimer) : { stop; }			
			}
		}

		if(m_penKillEnemy!=NULL)
		{		
			if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
			{
				((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
			}		
			m_penKillEnemy = NULL;
		}

		// 혼자서 움직이는 넘이 아닐때 StandingAnim을 해줘야 함.
		if( !m_bUseAI )
		{
			m_bAttack = FALSE;
			m_bMoving = FALSE;
			m_bStop = FALSE;		
			StandingAnim();
			SetNoTargetEntity();	
		}

		return EReturn();	
	};

	PreSkillAnimation(EVoid)
	{
		m_vDesiredPosition = PlayerDestinationPos();
		
		StopMoving();//0831

		m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetReadyAnim(FALSE));
		m_fSkillAnimTime = GetModelInstance()->GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed;		
		GetModelInstance()->AddAnimation(m_idCurrentSkillAnim,AN_NORESTART,1.0f,0, m_fSkillSpeed);		
		m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
		m_nEffectStep = 0;
		m_saLastEffectInfo.Clear();
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

		wait(m_fSkillAnimTime) {
			on (EBegin) : { 
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
					CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
					if(m_nEffectStep == 0)
					{
						++m_nEffectStep;
						m_pSkillReadyEffect = StartEffectGroup(skill.GetReadyEffect(FALSE)
										, &en_pmiModelInstance->m_tmSkaTagManager
										, _pTimer->GetLerpedCurrentTick());
						if(m_pSkillReadyEffect) {m_pSkillReadyEffect->SetSpeedMul(m_fSkillSpeed);}
					}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
					resume; 
				};
			on (ETimer) : { stop; };
		}

		m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(FALSE));
	
		m_fSkillAnimTime = GetModelInstance()->GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed;		
		
	
		GetModelInstance()->AddAnimation(m_idCurrentSkillAnim,AN_LOOPING|AN_NORESTART|AN_CLEAR,1.0f,0, m_fSkillSpeed);	
		m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
		autowait(0.1f);//0114 임시코드 still애니메이션 0.1초

		return EReturn();
	}

	SkillAnimation(EVoid) //0628
	{	
		m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(FALSE));

		if(m_idCurrentSkillAnim == -1)
		{
			return EReturn();
		}		

		StopRotating();
		if( m_penEnemy == NULL )
		{
			if( m_pSkillReadyEffect != NULL && CEffectGroupManager::Instance().IsValidCreated( m_pSkillReadyEffect ) )
			{
				m_pSkillReadyEffect->Stop( 0.1f );
				m_pSkillReadyEffect = NULL;
			}
		}

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
		m_nEffectStep = 0;
//		m_iSkillEffectStep = 0;
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

		NewClearState(CLEAR_STATE_LENGTH);    
		
		m_fSkillAnimTime = GetModelInstance()->GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed;
		m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();			

		GetModelInstance()->AddAnimation(m_idCurrentSkillAnim,AN_NORESTART,1.0f,m_fSkillSpeed);	
		
		m_fAttackFrequency = 0.25f;
		m_fImpactTime = GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed / 2;//0630 우선 스킬 애니메이션의 절반에서 임팩트.		

		while(_pTimer->CurrentTick() - m_tmSkillStartTime < m_fSkillAnimTime) //GetAnimLength(m_idCurrentSkillAnim))
		{
			wait(m_fAttackFrequency) 
			{
				on (EBegin) : 
				{
					if(m_bMoving)
					{										
						return EReturn();
					}

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
					CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
					
					if(m_nEffectStep == 0)
					{
						++m_nEffectStep;

						CEffectGroup *pFireEffect = StartEffectGroup(skill.GetFireEffect1(FALSE), &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						if(pFireEffect) {pFireEffect->SetSpeedMul(m_fSkillSpeed);}
						if(m_pSkillReadyEffect != NULL && CEffectGroupManager::Instance().IsValidCreated(m_pSkillReadyEffect))
						{
							for(INDEX i=0; i<m_pSkillReadyEffect->GetEffectCount(); ++i)
							{
								CEffect *pEffect = m_pSkillReadyEffect->GetEffectKeyVector()[i].m_pCreatedEffect;
								if(pEffect != NULL && pEffect->GetType() == ET_SKA)
								{
									CSkaEffect *pSkaEffect = (CSkaEffect*)pEffect;
									TagInfo ti;
									ti.m_vPos = pSkaEffect->GetInstancePosition();
									ti.m_qRot = FLOATquat3D(1,0,0,0);
									INDEX cnt = m_saLastEffectInfo.Count();
									m_saLastEffectInfo.Expand(cnt + 1);
									m_saLastEffectInfo[cnt] = ti;
								}
							}
							m_pSkillReadyEffect->Stop(0.04f);
							m_pSkillReadyEffect = NULL;
						}
						else if(pFireEffect != NULL)
						{
							pFireEffect->Process(_pTimer->GetLerpedCurrentTick());
							for(INDEX i=0; i<pFireEffect->GetEffectCount(); ++i)
							{
								CEffect *pEffect = pFireEffect->GetEffectKeyVector()[i].m_pCreatedEffect;
								if(pEffect != NULL && pEffect->GetType() == ET_SKA)
								{
									CSkaEffect *pSkaEffect = (CSkaEffect*)pEffect;
									TagInfo ti;
									ti.m_vPos = pSkaEffect->GetInstancePosition();
									ti.m_qRot = FLOATquat3D(1,0,0,0);
									INDEX cnt = m_saLastEffectInfo.Count();
									m_saLastEffectInfo.Expand(cnt + 1);
									m_saLastEffectInfo[cnt] = ti;
									pSkaEffect->SetLifeTime(0);
								}
							}
						}
					}
					ASSERT(skill.GetDelay(m_nEffectStep-1, FALSE) < GetAnimLength(m_idCurrentSkillAnim) && "SkillAnimTime은 반드시 MissileSpeedFireTime보다 커야합니다.");

					// 이 부분에서 iSkillEffectStep의 값은 1~skill.GetDelayCount()이다.
					if( m_nEffectStep <= skill.GetDelayCount(FALSE)
					 && _pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > skill.GetDelay(m_nEffectStep-1, FALSE) * m_fSkillSpeed)
					{
						++m_nEffectStep;
						if(skill.GetMissileType(FALSE) == CSkill::MT_NONE && m_penEnemy.ep_pen != NULL)	// 근접 공격 - 마법 없음
						{
							FLOAT3D vHitPoint;
							FLOAT3D vHitDir;
							GetTargetDirection(vHitPoint, vHitDir);

							if( m_dcEnemies.Count() > 0 )
							{
								DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, vHitPoint, TRUE);
								m_dcEnemies.Clear();
							}
							else
							{
								//damage effect 처리
								if(m_penEnemy != NULL)//0817 버그 수정.
								{
									this->InflictDirectDamage(m_penEnemy, this, DMT_NONE, 1, vHitPoint, vHitDir);
								}
							}
							FLOAT3D vAxisY(0,1,0);
							FLOAT angle = acos(vHitDir % vAxisY);
							FLOAT3D axis = vAxisY * vHitDir;
							axis.Normalize();
							FLOATquat3D quat;
							quat.FromAxisAngle(axis, angle);
							StartEffectGroup(skill.GetFireEffect3(FALSE), _pTimer->GetLerpedCurrentTick(), vHitPoint, quat);
						}
						else if(skill.GetMissileType(FALSE) == CSkill::MT_ARROW && m_penEnemy.ep_pen != NULL)	// 화살
						{
							ASSERT(en_pmiModelInstance != NULL);

							FLOAT fHoriOffset = 0.0f;
							FLOAT fVertiOffset = 0.0f;
							if(skill.GetDelayCount(FALSE) > 1)
							{
								FLOAT fDist = (m_penEnemy->en_plPlacement.pl_PositionVector - en_plPlacement.pl_PositionVector).Length();
								FLOAT factor = 1.3f * fDist / skill.GetFireRange();
								factor = Clamp(factor, 0.0f, 1.0f);
								if(m_nEffectStep == 2) {fHoriOffset = +1.0f * factor;}
								if(m_nEffectStep == 3) {fHoriOffset = -1.0f * factor;}
								//if(m_nEffectStep == 4) {fVertiOffset = +0.0f * factor;}
								if(m_nEffectStep == 5) {fVertiOffset = -1.0f * factor;}
							}
							if( FALSE )
							{
								if( m_dcEnemies.Count() > 0 )
								{
									for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
										it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
									{																		
										CEntity *pEn = (*it);
										if(pEn != NULL && pEn->IsFlagOff(ENF_DELETED))
										{
											ShotMissile(this, "STAFF", pEn, skill.GetMissileSpeed(FALSE)
												, skill.GetFireEffect3(FALSE), skill.GetFireEffect2(FALSE)
												, false, fHoriOffset, fVertiOffset, 0);
										}
									}
									m_dcEnemies.Clear();
								}
								else
								{
									ShotMissile(this, "STAFF", m_penEnemy.ep_pen, skill.GetMissileSpeed(FALSE)
												, skill.GetFireEffect3(FALSE), skill.GetFireEffect2(FALSE)
												, false, fHoriOffset, fVertiOffset, 1);
								}
							}
							else
							{
								if( m_dcEnemies.Count() > 0 )
								{
									for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
										it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
									{																		
										CEntity *pEn = (*it);
										if(pEn != NULL && pEn->IsFlagOff(ENF_DELETED))
										{
											ShotMissile(this, "RHAND", pEn, skill.GetMissileSpeed(FALSE)
												, skill.GetFireEffect3(FALSE), skill.GetFireEffect2(FALSE)
												, false, fHoriOffset, fVertiOffset, 0);
										}
									}
									m_dcEnemies.Clear();
								}
								else
								{
									ShotMissile(this, "RHAND", m_penEnemy.ep_pen, skill.GetMissileSpeed(FALSE)
												, skill.GetFireEffect3(FALSE), skill.GetFireEffect2(FALSE)
												, false, fHoriOffset, fVertiOffset, 0);
								}
							}
						}
						else if(skill.GetMissileType(FALSE) == CSkill::MT_DIRECT && m_penEnemy.ep_pen != NULL)	// 발사체 없음, 데미지 없음, 버프 효과 등
						{
							if( m_penEnemy->GetFlags() & ENF_ALIVE && m_penEnemy->en_pmiModelInstance != NULL)
							{
								StartEffectGroup(skill.GetFireEffect3(FALSE)
												, &m_penEnemy->en_pmiModelInstance->m_tmSkaTagManager
												, _pTimer->GetLerpedCurrentTick());
							}
						}
						else if(skill.GetMissileType(FALSE) == CSkill::MT_DIRECTDAMAGE && m_penEnemy.ep_pen != NULL)	// 발사체 없음, 데미지 있음
						{
							if( m_dcEnemies.Count() > 0 )
							{						
								for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
									it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
								{									
									CEntity &en = *(*it);
									if(en.IsFlagOn(ENF_ALIVE) && en.en_pmiModelInstance != NULL)
									{
										StartEffectGroup(skill.GetFireEffect3(FALSE)
														, &(en.en_pmiModelInstance->m_tmSkaTagManager)
														, _pTimer->GetLerpedCurrentTick());								
									}
								}

								// 메이지의 싱글던젼용 스킬을 쓰면 몹이 안죽길래...
								if(m_dcEnemies.Count() > 0)
								{
									FLOAT3D vHitPoint;
									FLOAT3D vHitDir;
									GetTargetDirection(vHitPoint, vHitDir);
									DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, vHitPoint, TRUE);
									m_dcEnemies.Clear();
								}
							}
							else
							{
								this->InflictDirectDamage( m_penEnemy, this, DMT_NONE, 1, FLOAT3D(0,0,0), FLOAT3D(0,0,0) );

								if(m_penEnemy->en_pmiModelInstance != NULL)
								{
									StartEffectGroup( skill.GetFireEffect3(FALSE)
													, &m_penEnemy->en_pmiModelInstance->m_tmSkaTagManager
													, _pTimer->GetLerpedCurrentTick() );
								}
							}
						}
						else if( (skill.GetMissileType(FALSE) == CSkill::MT_CONTINUE || 
								skill.GetMissileType(FALSE) == CSkill::MT_MAGECUTTER )
							&& m_penEnemy != NULL && m_saLastEffectInfo.Count() != 0)	// 시전중인 이펙트가 발사체가 되어 날아감
						{
							if( m_dcEnemies.Count() > 0 )
							{
								for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
									it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
								{															
									CEntity &en = *(*it);
									if(en.IsFlagOff(ENF_DELETED))
									{
										ShotMagicContinued(this, m_saLastEffectInfo[0].m_vPos, m_saLastEffectInfo[0].m_qRot
													, &en, skill.GetMissileSpeed(FALSE)
													, skill.GetFireEffect3(FALSE), skill.GetFireEffect2(FALSE)
													, false, skill.GetMissileType(FALSE));
									}
								}
								m_dcEnemies.Clear();
							}
							else
							{
								ShotMagicContinued(this, m_saLastEffectInfo[0].m_vPos, m_saLastEffectInfo[0].m_qRot
													, m_penEnemy, skill.GetMissileSpeed(FALSE)
													, skill.GetFireEffect3(FALSE), skill.GetFireEffect2(FALSE)
													, false, skill.GetMissileType(FALSE));
							}
							INDEX cnt = m_saLastEffectInfo.Count();
							CStaticArray<TagInfo> temp;
							temp.New(cnt-1);
							for(INDEX i=1; i<m_saLastEffectInfo.Count(); ++i)
							{
								temp[i-1] = m_saLastEffectInfo[i];
							}
							m_saLastEffectInfo = temp;
						}						
					}
					m_fMoveSpeed = 0.0f;
					if(m_penEnemy != NULL
						&& skill.GetMissileType(FALSE) != CSkill::MT_NOTHING)
					{
						ULONG ulFlags = SetDesiredMovement();
					}

					resume; 
				}
				on (ESlaveAttackDamage eAttackDamage) : 
				{
					CEntity *pen = m_penEnemy;
					FLOAT3D vCurrentCenter(((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[0],
						((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[1],
						((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[2] );
					FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vCurrentCenter;
					FLOAT3D vTargetCenter(0, 0, 0);
					FLOAT3D vTargetPos(0, 0, 0);
					FLOAT3D vDirection(0, 0, 0);
					FLOAT size = 0;
					if(pen != NULL && pen->GetFlags()&ENF_ALIVE)
					{
						if(pen->en_RenderType == RT_SKAMODEL)
						{
							// Bounding Box를 이용하여 Effect를 찍을 위치를 찾는다.
							FLOATaabbox3D bbox;
							pen->GetModelInstance()->GetCurrentColisionBox(bbox);
							FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*단순히 원으로 하면 너무 큼. 사이즈 조절 상수*/;
							size = vRadius.Length();
						}
						vTargetCenter = FLOAT3D(((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[0],
							((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[1],
							((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[2] );
						vTargetPos = pen->en_plPlacement.pl_PositionVector + vTargetCenter;
						vDirection = vTargetPos - vCurrentPos;
						vDirection.Normalize();
						vTargetPos -= vDirection * size;
					}
					vDirection.Normalize();
					if( m_dcEnemies.Count() > 0 )
					{
						DamagedTargetsInRange(this, m_dcEnemies, DMT_CLOSERANGE, 1, vTargetPos, TRUE);
						m_dcEnemies.Clear();
					}
					else
					{
						if(m_penEnemy!=NULL)//0817 버그 수정.
						{
							this->InflictDirectDamage(m_penEnemy, this , DMT_CLOSERANGE, (int)1, vTargetPos, vDirection);
						}
					}
					m_fImpactTime = 1000.0f;//충분히 긴시간.

					stop;
				}
				
				on (ETimer) : { stop; }			
			}
		}

/*
		m_bAttack = FALSE;		
		
		
		*/
		m_bMoving = FALSE;		
		m_bStop = FALSE;		
		StandingAnim();				
		m_bSkilling = FALSE;
		m_nSkillChack = 0;			// ttos : 소환수 스킬 해제
		//SetNoTargetEntity();

		return EReturn();
	};

	SkillAndMagicAnimation(EVoid)
	{
		if(m_SkillEffectInfo.dwValidValue != 0)
		{
			m_SkillEffectInfo.dwValidValue = 0xBAD0BEEF;
			return EReturn();
		}

		m_fAttackFrequency = 0.05f;
		m_fAttackStartTime = _pTimer->GetLerpedCurrentTick();
		m_fImpactTime = GetAnimLength(iAnimSlave_Attack)/3;

		GetModelInstance()->AddAnimation(m_SkillEffectInfo.iAnimatioID,AN_CLEAR,1.0f,0);	

		m_nEffectStep = 1;
		m_pSkillReadyEffect = NULL;
		m_pSkillReadyEffect = StartEffectGroup(m_SkillEffectInfo.szEffectNameCast
			, &en_pmiModelInstance->m_tmSkaTagManager
			, _pTimer->GetLerpedCurrentTick());

		while(_pTimer->GetLerpedCurrentTick() - m_fAttackStartTime < GetAnimLength(m_SkillEffectInfo.iAnimatioID)*0.8f)
		{
			wait(m_fAttackFrequency) 
			{
				on (EBegin) : 
				{					
					if(m_bStop)
					{
						DestroyEffectGroupIfValid(m_pSkillReadyEffect);
						//GetModelInstance()->AddAnimation(iAnimSlave_Idle1,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);							
						StandingAnim();
						m_SkillEffectInfo.dwValidValue = 0xBAD0BEEF;
						return EReturn();
					}
											
					m_fMoveSpeed = 0.0f;
					if(m_penEnemy.ep_pen != this)
					{
						SetDesiredMovement();
					}

					FLOAT time = _pTimer->GetLerpedCurrentTick() - m_fAttackStartTime;
					if(time >= m_SkillEffectInfo.fFireDelay[0] && m_nEffectStep == 1 && m_SkillEffectInfo.iFireDelayCount > 0)
					{
						++m_nEffectStep;
						SkillAndAttackFire(1);
					}
					if(time >= m_SkillEffectInfo.fFireDelay[1] && m_nEffectStep == 2 && m_SkillEffectInfo.iFireDelayCount > 1)
					{
						++m_nEffectStep;
						SkillAndAttackFire(2);
					}
					if(time >= m_SkillEffectInfo.fFireDelay[2] && m_nEffectStep == 3 && m_SkillEffectInfo.iFireDelayCount > 2)
					{
						++m_nEffectStep;
						SkillAndAttackFire(3);
					}
					if(time >= m_SkillEffectInfo.fFireDelay[3] && m_nEffectStep == 4 && m_SkillEffectInfo.iFireDelayCount > 3)
					{
						++m_nEffectStep;
						SkillAndAttackFire(4);
					}
					resume;
				}
				on (EEnemyBaseDamage eEBDamage) : 
				{
					// if confused
					m_fDamageConfused -= eEBDamage.fAmount;
					if (m_fDamageConfused < 0.001f) 
					{
						m_fDamageConfused = m_fDamageWounded;
						// play wounding animation
						INDEX animSet, animIdx;
						static INDEX animId = ska_StringToID("mldam");//HardCoding
						if(this->en_pmiModelInstance
						&& this->en_pmiModelInstance->FindAnimationByID(animId, &animSet, &animIdx))
						{
							//타격 모션
							this->en_pmiModelInstance->AddAnimation(animId, AN_REMOVE_ON_END, 1.0f, 0);
						}
					}
					resume;
				}
				on (ETimer) : { stop; }
			}
		}

		m_nEffectStep = 0;
		m_SkillEffectInfo.dwValidValue = 0xBAD0BEEF;
		return EReturn();
	};

	// Normal attack
	NormalAttack(EVoid)
	{
		// EDIT : BS 060526
//		StandingAnim();
//		autowait(0.8f + FRnd()*0.25f);				
		// EDIT : BS 060526

		if( m_penEnemy && m_penEnemy->GetFlags()&ENF_ALIVE)
		{
			if( m_bUseAI )
			{
				// 플레이어가 죽었다면....
				if( !(CEntity::GetPlayerEntity(0)->GetFlags()&ENF_ALIVE) )
				{
					return EReturn();
				}
				//070525_ttos : 소환수 스킬 예약 
				if(m_bSkilling)
				{
					return EReturn();
				}else if(m_nSkillChack)
				{
					UseSkill(m_nSkillChack);	//ttos : 소환수 스킬 예약된거 시작
					m_nSkillChack = 0;			// 소환수 스킬 해제
					return EReturn();
				} 
				// 싱글 모드의 경우.
				if(_pNetwork->m_bSingleMode)
				{	
					// 공격하는 놈과 맞는 놈의 위치를 보내줌.				
					CPlacement3D Myplacement = GetPlacement();					
					_pNetwork->SendMoveMessage(this, Myplacement, m_fMoveSpeed);

// EDIT : BS : BEGIN
//					CPlacement3D placement = m_penEnemy->GetPlacement();
//					
//					if( m_penEnemy->IsEnemy() )
//					{
//						CPlacement3D pl		= m_penEnemy->GetLerpedPlacement();
//						_pNetwork->AddMoveList( 
//							m_penEnemy->GetNetworkID(),
//							placement.pl_PositionVector(1), 
//							placement.pl_PositionVector(3), 
//							placement.pl_PositionVector(2),
//							placement.pl_OrientationAngle(1) );
//					}
					_pNetwork->AddMoveList(*m_penEnemy);
// EDIT : BS : END
					_pNetwork->SendMoveList();
				}				
				_pNetwork->SendAttackMessage(this, m_penEnemy, FALSE);
			}
		}

		//autowait(0.60f);

		return EReturn();
	};
};