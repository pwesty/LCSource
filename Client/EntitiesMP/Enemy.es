
1864
%{
#include "StdH.h"

#include <Engine/Effect/CWorldTag.h>
#include <Engine/Effect/CEffectGroup.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CSkaEffect.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Network/ItemTarget.h>
#include <deque>
#include <Engine/Entities/Skill.h>
#include <Engine/Network/MessageDefine.h>
#include <Engine/Network/CharacterTarget.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Object/ActorMgr.h>
#include <Common/Packet/ptype_old_do_move.h>
#include <Common/Packet/ptype_old_do_skill.h>

#include "EntitiesMP/RestrictedField.h"
#include "EntitiesMP/Common/RestrictedFieldContainer.h"
#include "EntitiesMP/Common/CharacterDefinition.h"

#define MOB_DOOMSLAYER	158
#define MOB_BAAL		152

#define MOB_ATTACK_MOTION_NUM		(4)

//-----------------------------------------------------------------------------
// Purpose: 컨테이너 내의 모든 엔티티에 한번에 데미지를 줍니다.
// Input  : pEntity - 
//			sTI - 
//			vCenter - 
// Output : static void
//-----------------------------------------------------------------------------
void DamagedTargetsInRange(CEntity* pEntity, CSelectedEntities& dcEntities, enum DamageType dmtType, FLOAT fDamageAmmount, FLOAT3D vCenter, BOOL bSkill);

void ShotFallDown(FLOAT3D vStartPos, FLOAT3D vToTargetDir, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szFallObjEffectName
				 , bool bCritical);
void GetTargetDirection(CEntity *penMe, CEntity *penTarget, FLOAT3D &vTargetPos, FLOAT3D &vDirection)
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

ENGINE_API void SetDropItemModel(CEntity *penEntity, CItemData *pItemData, CItemTarget* pTarget);
%}

uses "EntitiesMP/EnemyBase";
uses "EntitiesMP/Global";
uses "EntitiesMP/BasicEffects";
uses "EntitiesMP/CharacterBase";
uses "EntitiesMP/Character";

event EAttackDamage {//0629
};

// 공격 타입.
enum AttackType
{
	0 ATK_NORMAL	"Attack Normal",
	1 ATK_MAGIC		"Attack Magic",
};

enum MobType
{
	0 MOB_ENEMY		"Boss Mob",		// 일반 몹.
	1 MOB_FOLLOWPC	"Follow PC",	// PC를 따라가는 NPC
};

%{
// info structure
static EntityInfo eiMobBoss = {
	EIBT_FLESH, 200.0f,
	//0.0f, 1.75f*m_fStretch, 0.0f,     // source (eyes)
	//0.0f, 1.30f*m_fStretch, 0.0f,     // target (body)
	0.0f, 1.75f*1.0f, 0.0f,     // source (eyes)
	0.0f, 1.30f*1.0f, 0.0f,     // target (body)
};

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
static INDEX g_iNPCMinBrightness = 32;	//명도 0~240
void CEnemy_OnInitClass(void)
{
	_pShell->DeclareSymbol("persistent user INDEX g_iNPCMinBrightness;", &g_iNPCMinBrightness);
}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

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

class CEnemy: CEnemyBase {
name      "Enemy";
thumbnail "Mods\\test.tbn";
features  "ImplementsOnInitClass", "HasName", "IsTargetable";

properties:
	1 enum	MobType m_MobType "Type" 'Y'		= MOB_ENEMY,	
	3 FLOAT m_fBodyAnimTime		= -1.0f,
	91 INDEX m_nMobDBIndex "Enemy List" 'E'		= 0,
	21	INDEX		m_iRegenTime "Regen Time" 'R' = 300,
	5 FLOAT m_fStretch			= 1.0f,
	80 enum AttackType m_AttackType = ATK_NORMAL,	
	99  INDEX idMob_Start		= -1,
	100 INDEX idMob_Walk		= -1,
	101 INDEX idMob_Attack		= -1,
	102 INDEX idMob_Default		= -1,
	103 INDEX idMob_Death		= -1,
	104 INDEX idMob_Wound		= -1,
	105 INDEX idMob_NormalBox	= -1,
	106 INDEX idMob_Attack2		= -1,
	107 INDEX idMob_Run			= -1,
	108 INDEX idMob_Default2	= -1,
	109 INDEX idMob_Attack3		= -1,
	110 INDEX idMob_Attack4		= -1,
	150 INDEX m_iTotalNum	"Total Num"	= -1,
	151 FLOAT	m_fScale = 1.0f,
	152 BOOL	m_bMobChange = FALSE,
// FIXME : 싱글던젼을 위해서만 필요함...ㅡ.ㅡ
// FIXME : 보스몹은 안 밀리게 해달라구 해서....
	160 BOOL	m_bBoss			= FALSE,

	200	CTString	m_strName	" Name" = "Enemy",		
//	201 INDEX	m_nMaxiHealth = 0,
//	202 INDEX	m_nCurrentHealth = 0,	
	203 INDEX	m_nMobClientIndex	= -1,
	//204 CSoundObject m_soSound2,
	//205 INDEX	m_nMobLevel = 0,
	//206 INDEX	m_nPreHealth = -1,	
	//235	INDEX m_iZoneMovingFlag "Zone Moving"		= 513,	// Zone Moving
	236 BOOL    m_bChanging = FALSE,
	237 zoneflags_ex ZoneBits	m_zfZoneFlagBits	= 0,	// Zone Moving Flag
	238 zoneflags_ex ExtraBits	m_efExtraBits		= 0,	// Extra Flag
	240 BOOL	m_bPreCreate				"Precreated NPC"	= FALSE,	// 미리 생성되어야 하는 NPC의 경우.
	245 BOOL	m_bHostageNpc				"Is hostage NPC?"	= FALSE,	// 구출해야하는 NPC인가?
	247 enum EventType	m_EventType			"Event Type"		= EVENT_NONE,
	248 CEntityPointer m_penEventTarget		"Event Target",					// 트리거를 통해서 Blind를 해제하도록 함.
	250 BOOL	m_bInit						= FALSE,
	251 BOOL	m_bIsRun					= FALSE,
	252 BOOL	m_bDie	= FALSE,
	253 INDEX   m_nChangeMonsterId = -1,
	254	FLOAT	m_fActionAnimTime = 0.0f,
	255 CSoundObject m_soMessage,  // message sounds (필요할까나?)
	//253 FLOAT m_fIdleTime = 0.0f,
	{
		CEffectGroup *m_pSkillReadyEffect;
		CStaticArray<FLOAT3D> m_aDeathItemPosDrop;
		CStaticArray< CItemTarget* > m_aDeathItemTargetDrop;
		CStaticArray<void *> m_aDeathItemDataDrop;
		INDEX	m_nMagicAttackSequence;
		sSkillEffectInfo	m_SkillEffectInfo;
		INDEX	m_nEffectStep;
		//INDEX	m_iSkillEffectStep;
		//CStaticArray<FLOAT> m_afHardCodeValue;
		//BOOL	m_bHardcodingSkillLoop;		
		CEnemyAnimation		m_AnimationIndices;
		INDEX	m_idMob_Action;
		DWORD	m_dwAniMationFlag;
	}

components:	
	// 사운드 처리가 가장 문제되는 부분.
	1 sound SOUND_POLYMOPH				"Data\\sounds\\character\\public\\C_transformation.wav",
functions:

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	BOOL AdjustShadingParameters(FLOAT3D &vLightDirection, COLOR &colLight, COLOR &colAmbient)
	{
		//이 주석을 풀면 몹사망시 Fade-out처리가 된다. 단 약간 튀는 듯한 문제 있음.
		//CEnemyBase::AdjustShadingParameters(vLightDirection, colLight, colAmbient);
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
		//return TRUE;
		return CEnemyBase::AdjustShadingParameters(vLightDirection, colLight, colAmbient);
	}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

	virtual FLOAT GetLockRotationSpeed(void) 
	{ 
		return AngleDeg(1800.0f); 
	};
	
	void OnInitialize(const CEntityEvent &eeInput)
	{
		CEnemyBase::OnInitialize(eeInput);
	};

	void OnEnd(void) 
	{
		CEnemyBase::OnEnd();		
	};	

	// 에너미가 움직일수 있는지 판단함...
	virtual BOOL IsMovable()
	{
		// 내 소환수이며...
		if( m_bUseAI )
		{			
			if( m_bStuned || m_bHold )
			{
				return FALSE;
			}
		}
		return TRUE;
	};

	// 에너미가 공격할 수 있는 상태인가?
	virtual BOOL IsAttackable()
	{
		if( IsFirstExtraFlagOn(ENF_EX1_NPC) )
		{
			return FALSE;		
		}
		else if( m_bUseAI )
		{
			if( m_bStuned || m_bHold )
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

	virtual BOOL IsBossMob()
	{
		return m_bBoss;
	};

	virtual BOOL IsSkillAttack( )
	{
		float frandom = FRnd();

		// 카미라의 스킬 공격 확률.
		if( frandom > 0.2f )
		{
			// 일반 공격
			return FALSE;
		}
			
		if( m_bUseAI )
		{
			// 버프가 걸린 상태에서 스킬 사용 불가.
			if( m_bCannotUseSkill ) { return FALSE; }

			CMobData* MD = CMobData::getData(m_nMobDBIndex);

			if( MD->GetSkill0Index() <= 0 && MD->GetSkill1Index() <= 0 )
			{
				return FALSE;
			}
			
			float frandom = FRnd();
			if( frandom > 0.5f && MD->GetSkill1Index() > 0 )
			{
				m_nCurrentSkillNum	= MD->GetSkill1Index();
			}
			else
			{
				m_nCurrentSkillNum	= MD->GetSkill0Index();
			}

			CSkill& rSkill = _pNetwork->GetSkillData( m_nCurrentSkillNum );
			if( ( CalcDist(m_penEnemy) < rSkill.GetFireRange() && CanHitEnemy(m_penEnemy, Cos(AngleDeg(45.0f))) ) ) 
			{
				CPlacement3D pl		= GetLerpedPlacement();

				CNetworkMessage nmMobMove;
				RequestClient::moveForNormal* packet = reinterpret_cast<RequestClient::moveForNormal*>(nmMobMove.nm_pubMessage);
				packet->type = MSG_MOVE;
				packet->subType = 0;
				packet->charType = MSG_CHAR_NPC;
				packet->moveType = MSG_MOVE_STOP;
				packet->index = GetNetworkID();
				packet->speed = 0.0f;
				packet->x = pl.pl_PositionVector(1);
				packet->z = pl.pl_PositionVector(3);
				packet->h = pl.pl_PositionVector(2);
				packet->r = pl.pl_OrientationAngle(1);
				packet->ylayer = _pNetwork->MyCharacterInfo.yLayer;

				nmMobMove.setSize(sizeof(*packet));
				_pNetwork->SendToServerNew(nmMobMove);	
				
				{
					CNetworkMessage nmSkill(MSG_SKILL);
					RequestClient::skillReady* Readypacket = reinterpret_cast<RequestClient::skillReady*>(nmSkill.nm_pubMessage);

					Readypacket->type = MSG_SKILL;
					Readypacket->subType = MSG_SKILL_READY;

					Readypacket->skillIndex = m_nCurrentSkillNum;
					Readypacket->charType = MSG_CHAR_NPC;
					Readypacket->charIndex = GetNetworkID();
					Readypacket->targetType = MSG_CHAR_PC;
					Readypacket->targetIndex = _pNetwork->MyCharacterInfo.index;
					Readypacket->nDummySkillSpeed = 0;
					nmSkill.setSize( sizeof(*Readypacket) );

					_pNetwork->SendToServerNew(nmSkill);
				}
				{
					CNetworkMessage nmSkill(MSG_SKILL);
					RequestClient::skillFire* Firepacket = reinterpret_cast<RequestClient::skillFire*>(nmSkill.nm_pubMessage);

					Firepacket->type = MSG_SKILL;
					Firepacket->subType = MSG_SKILL_FIRE;

					Firepacket->skillIndex = m_nCurrentSkillNum;
					Firepacket->charType = MSG_CHAR_NPC;
					Firepacket->charIndex = GetNetworkID();
					Firepacket->targetType = MSG_CHAR_PC;
					Firepacket->targetIndex = _pNetwork->MyCharacterInfo.index;
					Firepacket->listCount = 0;
					nmSkill.setSize( sizeof(*Firepacket) );

					_pNetwork->SendToServerNew(nmSkill);					
				}				
				return TRUE;
			}
		}
		m_nCurrentSkillNum	= -1;
		return FALSE;
	};

	/* Entity info */
	void *GetEntityInfo(void)
	{
			return &eiMobBoss;
	};

	void Precache(void) 
	{
		CEnemyBase::Precache();		

		//-------------------------------------------------------------------------------
		if(m_bPreCreate)
		{
			int max = CMobData::getsize();
			if(_pNetwork && _pNetwork->m_bSingleMode && max)
			{
				en_pwoWorld->m_vectorPreCreateNPC.push_back(this);
			}
		}
		//-------------------------------------------------------------------------------

//안태훈 수정 시작	//(Fix Snd Effect Bug)(0.1)
		if(en_pmiModelInstance) { en_pmiModelInstance->m_tmSkaTagManager.SetOwner(this); }
//안태훈 수정 끝	//(Fix Snd Effect Bug)(0.1)
	};

	virtual BOOL IsEnemy(void) const
	{
		return TRUE;
	}

	void InflictDirectDamage(CEntity *penToDamage, CEntity *penInflictor, enum DamageType dmtType,
	FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
	{
		if(penToDamage)
		{
			if( !(penToDamage->GetFlags()&ENF_ALIVE ) )	{ return; }

			SE_Get_UIManagerPtr()->ShowDamage( penToDamage->en_ulID );
			// Date : 2005-11-16(오후 2:05:09), By Lee Ki-hwan

			INDEX preHealth, curHealth;
			preHealth = ((CUnit*)penToDamage)->m_nPreHealth;
			curHealth = ((CUnit*)penToDamage)->m_nCurrentHealth;

			if(preHealth!= -1)// && curHealth > preHealth)
			{
				fDamageAmmount = 1;
				((CUnit*)penToDamage)->m_nCurrentHealth = ((CUnit*)penToDamage)->m_nPreHealth;			
			}
			else fDamageAmmount = 0;
	
			if (penToDamage->IsEnemy() && preHealth >= 0) // ?? Enemy가 Enemy(권좌)를 공격할 일이 있는가?
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

		CEntity::InflictDirectDamage(penToDamage, penInflictor, dmtType,
			fDamageAmmount,vHitPoint, vDirection);	

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

			INDEX iHitType = 0;
			INDEX iJob = 0;

			if (m_enAttackerID == _pNetwork->MyCharacterInfo.index)
			{
				iJob = _pNetwork->MyCharacterInfo.job;
				iHitType = _pNetwork->MyCharacterInfo.iHitEffectType;
			}

			ObjectBase* pObj = ACTORMGR()->GetObject(eOBJ_CHARACTER, m_enAttackerID);
			if (pObj != NULL)
			{
				CCharacterTarget* pCT = static_cast<CCharacterTarget*>(pObj);

				if (pCT != NULL)
				{
					iJob = pCT->GetType();
					iHitType = pCT->cha_iHitType;
				}
			}
			
			if (iHitType < 0 || iHitType >= DEF_HIT_TYPE_MAX)
			{
				iHitType = 0;
			}				

			StartEffectGroup(szHitEffect[iJob][iHitType], 
				_pTimer->GetLerpedCurrentTick(), vHitPoint, quat);
		}
		SE_Get_UIManagerPtr()->ShowDamage( en_ulID );

		// 평화 NPC(예를 들면 공주)가 아닐 경우에는 타겟이 때린 놈으로 변경되도록 함.
		if( m_bUseAI && !( GetFirstExFlags() & ENF_EX1_PEACEFUL ) )
		{
			// 무조건 때린 놈으로 바뀌면 안될거 같아서, 랜덤하게 바뀌도록 설정함.
			FLOAT fRand = FRnd();		
			if( fRand > 0.5f )
			{
				// 때린 놈 중에서 플레이어나 소환수의 경우에만...
				if( penInflictor->IsPlayer() || penInflictor->IsSlave() || penInflictor->IsWildPet())
				{
					// 때린 놈으로 타겟을 변경합니다.
					SetTargetEntity(penInflictor);				
				}
			}			
		}
		
		CEnemyBase::ReceiveDamage(penInflictor, dmtType, fDamageAmmount, vHitPoint, vDirection);
	};

	//--------------------------------------------------------------
	// 몬스터의 애니메이션을 설정하는 함수들...
	//--------------------------------------------------------------
	// damage anim
	INDEX AnimForDamage(FLOAT fDamage) 
	{
		ASSERT(idMob_Wound		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		// NOTE : 공성탑의 경우에는 데미지 애니메이션을 보여주지 않는다...
		CMobData* MD = CMobData::getData(m_nMobDBIndex);
		if(MD->IsCastleTower())
		{
			return -1;
		}

		//0820 가만히 서있을때만 데미지 모션이 나와야 된다. 꼭!
		if( !IsAnimationPlaying(idMob_Default) && 
			!IsAnimationPlaying(idMob_Default2) )
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
		GetModelInstance()->AddAnimation(idMob_Wound,AN_CLEAR,1,0);	
		
		return idMob_Wound;
	};

	// death
	INDEX AnimForDeath(void)
	{	
		ASSERT(idMob_Death		!= -1	&& "Invalid Animation");
	
		DropDeathItem();//1019
				
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		NewClearState(CLEAR_STATE_LENGTH);    
		GetModelInstance()->AddAnimation(idMob_Death,AN_NORESTART,1,0);

		return idMob_Death;
	};

	// 죽을때 뭔가가...
	FLOAT WaitForDust(FLOAT3D &vStretch)
	{
		//0317 kwon
		vStretch=FLOAT3D(1,1,1);		
		if(GetModelInstance()->IsAnimationPlaying(idMob_Death)) 
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

		if(_pNetwork->m_bSingleMode)
		{
			--_pNetwork->wo_dwEnemyCount;

			// NOTE : 밴더스내치의 경우에는 카운트에서 제외.
			if( m_nMobDBIndex != 220 )
			{
				_pNetwork->wo_dwKilledEnemyCount++;
			}

			if (g_slZone == 6) // 튜토리얼 던전에서
			{
				if (_pNetwork->wo_dwKilledEnemyCount == 1)
				{
					_UIAutoHelp->SetGMNotice(_S( 5037, "마우스 왼쪽 버튼으로 원하는 아이템을 클릭 하거나 키보드 F2 버튼을 누르세요."));
				}
				else if (_pNetwork->wo_dwKilledEnemyCount == 5)
				{
					_UIAutoHelp->SetGMNotice(_S( 5036, "타켓을 클릭 후 키보드 F1버튼으로 스킬을 시전해 보세요."));
				}
				else if (m_nMobDBIndex == 1129) // 보스 몹을 죽였을 때
				{
					_pNetwork->SendQuestMessage(MSG_QUEST_COMPLETE, 45);
				}
			}
			
			// 특정 몹의 수를 확인합니다.
			CRestrictedField *pField = FindPlayerInRestrictedField((CPlayer*)CEntity::GetPlayerEntity(0));
			if(pField)
			{
				if( _pNetwork->wo_dwEnemyCount <= pField->iMobCount )
				{
					pField->m_bActive	= FALSE;
					SendToTarget(pField->m_penTarget, EET_TRIGGER, CEntity::GetPlayerEntity(0));
				}
			}

			if(_pNetwork->m_ubGMLevel > 1)
			{
				CTString strMessage;
				strMessage.PrintF("=====current enemies : %d=====\n", _pNetwork->wo_dwEnemyCount);
				_pNetwork->ClientSystemMessage(strMessage);
			}
		}
	};

	// 클릭했을때의 이벤트.
	void ClickedEvent(void)
	{
		if(m_EventType == EVENT_MOBCLICK)
		{
			SendToTarget(m_penEventTarget, EET_TRIGGER, this);
		}
	};

	void StartAnim(void)
	{
		ASSERT(idMob_Start	!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(idMob_Start,AN_NORESTART|AN_CLEAR,1,0);
	};

	// virtual anim functions
	void StandingAnim(void) 
	{		
		m_bIsRun = FALSE;

		float fRnd = FRnd();
		INDEX iIdleAni;

		if (fRnd < 0.4f)
		{
			iIdleAni = idMob_Default2;
		}
		else
		{
			iIdleAni = idMob_Default;
		}

		ASSERT(iIdleAni	!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		GetModelInstance()->AddAnimation(idMob_Default,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
	};
	void RunningAnim(void) 
	{
		m_bIsRun = TRUE;
		ASSERT(idMob_Run		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(idMob_Run,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
	};
	void WalkingAnim(void) 
	{
		m_bIsRun = FALSE;
		ASSERT(idMob_Walk		!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		GetModelInstance()->AddAnimation(idMob_Walk,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);		
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
		ASSERT(idMob_Attack		!= -1		&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL	&& "Invalid ModelInstance");

		static FLOAT	start_attack_time=0;
		static float  m_fBodyAnimTime=0;
		
		if(start_attack_time==0)
		{
			start_attack_time = _pTimer->CurrentTick();
			GetModelInstance()->AddAnimation(idMob_Attack,AN_NORESTART|AN_CLEAR,1.0f,0);
			m_fBodyAnimTime = GetModelInstance()->GetAnimLength(idMob_Attack);
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
	void AddDeathItem(FLOAT3D pos, class CItemTarget *pItemTarget, class CItemData *pItemData)
	{
		INDEX cnt = m_aDeathItemTargetDrop.Count();
		m_aDeathItemPosDrop.Expand(cnt + 1);
		m_aDeathItemPosDrop[cnt] = pos;
		m_aDeathItemTargetDrop.Expand(cnt + 1);
		m_aDeathItemTargetDrop[cnt] = pItemTarget;
		m_aDeathItemDataDrop.Expand(cnt + 1);
		m_aDeathItemDataDrop[cnt] = pItemData;
	}

	virtual void DropDeathItem()
	{
		for(INDEX i = 0; i < m_aDeathItemTargetDrop.Count(); ++i )
		{
			CItemTarget* pTarget = NULL;
			pTarget = m_aDeathItemTargetDrop[i];

			CItemData* pItemData = ((CItemData*)m_aDeathItemDataDrop[i]);
			const char* szItemData = _pNetwork->GetItemName( pItemData->GetItemIndex() );
			CEntity* penEntity = NULL;
			
			CPlacement3D plPlacement;
			plPlacement.pl_PositionVector = m_aDeathItemPosDrop[i];
			plPlacement.pl_OrientationAngle = ANGLE3D(0.0f,0.0f,0.0f);
			
			ASSERT(pTarget->m_pEntity == NULL);
			penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_ITEM,-1,TRUE);
			if(penEntity == NULL) {continue;}

			penEntity->SetNetworkID(pTarget->GetSIndex());

			pTarget->m_pEntity = penEntity;
			pTarget->m_nIdxClient = penEntity->en_ulID;
			penEntity->en_strItemName = szItemData;
			SetDropItemModel(penEntity, pItemData, pTarget);

			ACTORMGR()->AddObject(pTarget);

			//드랍 사운드 플레이
			if( pItemData->GetType() == CItemData::ITEM_ETC
				&& pItemData->GetType() == CItemData::ITEM_ETC_MONEY )
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayItemSound(FALSE, TRUE);
			}
			else
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayItemSound(FALSE, FALSE);
			}
		}
		m_aDeathItemPosDrop.Clear();
		m_aDeathItemTargetDrop.Clear();
		m_aDeathItemDataDrop.Clear();
	}

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
				GetTargetDirection(this, m_penEnemy, vHitPoint, vHitDir);

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
					m_dcEnemies.Clear();
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
					m_dcEnemies.Clear();
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
// 그냥 Character.es에서 가져다 썼다
	void MinMaximize()
	{
		static BOOL bMinimize = TRUE;

		if(bMinimize)//작아진다. 
		{
			static int cnt=0;//누적 카운트

			static unsigned int tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);

			if(cnt==0)
			{  	
				tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);		
				cnt++;
			}

			int time = ((unsigned int(_pTimer->GetLerpedCurrentTick()*1000) - tmStartTime )%1000)/100;
			if(time < 1)
			{
				return;
			}

			cnt = cnt+ time;

			if(cnt > 9)
			{
				cnt = 9;
			}		
			m_fScale = 0.1f*(10-cnt);
			GetModelInstance()->StretchModel(FLOAT3D( m_fScale,m_fScale,m_fScale ));

			if(cnt==9)
			{				
				if(!m_bMobChange)
				{
					//MonsterChange(m_nChangeMonsterId,FALSE);//다 작아졌으니 변신한다.
				}
				else
				{
					m_nChangeMonsterId = -1;
					//ReturnChange(FALSE);	//다 작아졌으니 본모습으로 돌아온다.
				}

				bMinimize = FALSE;	
				cnt = 0;			
			}			
		}
		else//이제 커져야한다.
		{

			static int cnt=10;//누적 카운트

			static unsigned int tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);	
			
			if(cnt==10)
			{
				tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);		
				cnt--;
				StartEffectGroup("Polymorph", &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				PlaySound(m_soMessage, SOUND_POLYMOPH, SOF_3D | SOF_VOLUMETRIC);	
			}
	

			int time = ((unsigned int(_pTimer->GetLerpedCurrentTick()*1000) - tmStartTime )%1000)/100;
			if(time < 1)
			{
				return;
			}

			cnt = cnt - time;

			if(cnt < 1)
			{
				cnt = 1;
			}	
			
			GetModelInstance()->StretchModel(FLOAT3D( 0.1f*(11-cnt),0.1f*(11-cnt),0.1f*(11-cnt) ));

			if(cnt==1)
			{
			
				SetSkaColisionInfo(); //마지막에 충돌박스를 다시 셋팅해줘야 현재의 크기로 셋팅된다.
				m_bChanging = FALSE;	//변신이 완전 끝났다.	
				bMinimize = TRUE;	
				cnt = 10;						
			}
		}		

	}

	void SetNickNameDamageEffect(INDEX iNickIndex, NickNameEffectType iType)
	{
		CTString  strNickDamageFile;

		if(iNickIndex > 0 && iType == NICKNAME_ATTACK_EFFECT)	// 호칭이 있으면
		{
			strNickDamageFile = CNickNameData::getData(iNickIndex)->GetAttackEffectFile();
		}
		else if(iNickIndex > 0 && iType == NICKNAME_DAMAGE_EFFECT)
		{
			strNickDamageFile = CNickNameData::getData(iNickIndex)->GetDamageEffectFile();
		}

		if( strNickDamageFile != CTString(""))
		{
			StartEffectGroup(strNickDamageFile
						, &this->en_pmiModelInstance->m_tmSkaTagManager
						, _pTimer->GetLerpedCurrentTick());
		}
	}

procedures:
/************************************************************
 *                       M  A  I  N                         *
 ************************************************************/
	Main(EVoid) 
	{
		CMobData* MD = CMobData::getData(m_nMobDBIndex);

		/*if (m_nMobDBIndex > 0)
		{
			SetSkaModel(MD->GetMobSmcFileName());
			GetModelInstance()->m_tmSkaTagManager.SetOwner(this);
			FallDownToFloor();
		}*/

		if(GetModelInstance() == NULL)
		{ 
			InitAsSkaEditorModel();
			SetSkaModel("Models\\Editor\\Ska\\Axis.smc");

			if(!m_bInit)
			{
				if(m_nMobDBIndex > 0)
				{					
					// Mob Respawn Message
					if(_cmiComm.IsNetworkOn())
					{
						CPlacement3D pl = GetLerpedPlacement();
						CNetworkMessage nmMobSpawn(MSG_NPC_REGEN);
						INDEX iIndex	= -1;
						INDEX iYLayer	= 0;
						INDEX iMobType	= m_nMobDBIndex;
						INDEX iEntityID = en_ulID;					
						_pNetwork->AddRegenList(iIndex, iMobType, 
						pl.pl_PositionVector(1), pl.pl_PositionVector(2), pl.pl_PositionVector(3), 
						pl.pl_OrientationAngle(1), iYLayer, iEntityID);

						en_RenderType = RT_SKAMODEL;
					}
				}
			}
			return EEnd();
		}
	
		// declare yourself as a model
		SetPhysicsFlags(EPF_ONBLOCK_CLIMBORSLIDE
			| EPF_TRANSLATEDBYGRAVITY
			| EPF_PUSHABLE
			| EPF_MOVABLE			
			| EPF_ABSOLUTETRANSLATE /*| EPF_RT_SYNCHRONIZED*/);//|EPF_STICKYFEET|EPF_RT_SYNCHRONIZED);
		SetPhysicsFlags(GetPhysicsFlags() &~EPF_PUSHABLE);
// NOTE : 공성관련되서...
		// 이벤트 NPC만 충돌 체크

		// [070607: Su-won] 월드에디터에서는 그냥 통과.
		// 에디터상에서는 CNetworkLibrary::ga_World.wo_aMobData 에 NPC 데이터가 저장되지 않아서
		// CNetworkLibrary::GetMobData() 함수를 통해서 제대로된 NPC 데이터를 가져오지 못하기때문에
		// 월드에 배치된 NPC의 모델을 나타낼려고 할 때 에러발생함.
		extern BOOL _bWorldEditorApp;
		if( !_bWorldEditorApp )
		{
			if(MD->IsEvent() || MD->IsLordSymbol() || MD->IsCollsionModel())
			{
				SetCollisionFlags(ECF_MODEL);		
			}
			else 
			{
				SetCollisionFlags(ECF_MODEL_NO_COLLISION);
			}
		}

		en_sbNetworkType = MSG_CHAR_NPC;

//강동민 수정 시작 테스트 클라이언트 작업	06.22
		if(!_pNetwork->m_bSingleMode || m_bPreCreate)
		{		
			// 공성 관련되서... hp가 0이라면...
			// 죽은 NPC이지만 사라지지는 않는 NPC로...(그리고 타겟팅도 안되도록...)
			/*if( (MD->IsCastleTower()) && m_nCurrentHealth <= 0 || m_nMobDBIndex == 491)
			{
				SetFlagOff(ENF_ALIVE);
			}
			else*/
			{
				SetFlagOn(ENF_ALIVE);
			}
		}
		else
		{
			// 싱글 모드의 경우.
			m_bUseAI				= TRUE;
			m_fIgnoreRange			= 1000.0f;
			SetFlagOff(ENF_ALIVE);
		}
//강동민 수정 끝 테스트 클라이언트 작업		06.22
		en_tmMaxHoldBreath = 5.0f;
		en_fDensity = 2000.0f;
		if(	idMob_Walk			== -1 ||
			idMob_Run			== -1 ||
			idMob_Attack		== -1 ||
			idMob_Default		== -1 ||
			idMob_Death			== -1 ||
			idMob_Wound			== -1 ||
			idMob_NormalBox		== -1)
			{ 
				return EEnd();
			}

		SetFlagOn(ENF_RENDERREFLECTION);

		// setup moving speed
		//m_fWalkSpeed			= FRnd() + 0.5f;
		//m_fWalkSpeed			= 4.0f;
		m_aWalkRotateSpeed		= AngleDeg(1800.0f);
		//m_fAttackRunSpeed		= FRnd() + 1.0f;
		//m_fAttackRunSpeed		= 4.0f;
		m_aAttackRotateSpeed	= AngleDeg(1800.0f);
		//m_fCloseRunSpeed		= FRnd() + 1.0f;
		//m_fCloseRunSpeed		= 4.0f;
		m_aCloseRotateSpeed		= AngleDeg(1800.0f);

		// setup attack distances
		//m_fAttackDistance		= 5.0f;
		//m_fCloseDistance		= 20.0f;		// 근거리 모드????
		//m_fStopDistance			= 1.5f;			// (Stop Distance보다 가깝다면 적을 쫓아가지 않음.)
		m_fAttackFireTime		= 2.0f;
		m_fCloseFireTime		= 2.0f;
		m_fBlowUpAmount			= 80.0f;
		m_fBodyParts			= 4;
		m_fDamageWounded		= 0.0f;
//				m_iScore				= 0;

		m_nMobClientIndex		= en_ulID;	

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
		if(GetModelInstance() != NULL)
		{
			// set stretch factors for height and width
			GetModelInstance()->StretchModel(FLOAT3D(m_fStretch, m_fStretch, m_fStretch));

			CSkaTag tag;
			tag.SetName("__ROOT");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			FLOATaabbox3D aabb;
			GetModelInstance()->GetAllFramesBBox(aabb);
			tag.SetName("CENTER");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			tag.SetOffsetPos(0, aabb.Size()(2) * 0.5f * GetModelInstance()->mi_vStretch(2), 0);
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			tag.SetOffsetPos(0, aabb.Size()(2) * GetModelInstance()->mi_vStretch(2) + 0.5f, 0);
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
		}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

		extern BOOL	_bWorldEditorApp;
		if( !_bWorldEditorApp )
		{
			// 자이언트 석상의 경우 그림자 안나오도록...
			CMobData* MD = CMobData::getData(m_nMobDBIndex);

			// TRUE 로 값이 넘어온다. 
			// 그림자 Cast 꺼지지 않음 FALSE 지정 필요
			if (MD->IsNPCFlag2nd(NPC_NO_SHADOW_CAST) == FALSE)
			{
				GetModelInstance()->mi_bRenderShadow = TRUE;
			}
			else
			{
				GetModelInstance()->mi_bRenderShadow = FALSE;
			}
		}

		SetHealth(10000.0f);//0807 몹의 체력.
//		m_soSound2.Set3DParameters(25.0f, 5.0f, 1.0f, 1.0f);

		if(idMob_Start != -1)
		{
			FLOAT fAnimLength = GetAnimLength(idMob_Start);
			//GetModelInstance()->AddAnimation(idMob_Start,AN_NORESTART,1,0);
			StartAnim();
//			PlaySound(m_soSound, SOUND_ZOMBI_APPEAR, SOF_3D|SOF_VOLUMETRIC);
			autowait(fAnimLength);
		}

		ModelChangeNotify();
		//StandingAnim();

		m_dwAniMationFlag = 0;
		m_idMob_Action = 0;

		// continue behavior in base class
		jump CEnemyBase::MainLoop();
	};

/************************************************************
 *                A T T A C K   E N E M Y                   *
 ************************************************************/
	Action(EVoid) : CEnemyBase::Action
	{
		if (m_idMob_Action > 0)
		{
			m_bAction = FALSE;
			GetModelInstance()->AddAnimation(m_idMob_Action, m_dwAniMationFlag, 1.0f, 0);
			
			if (IsFlagOn(ENF_INVISIBLE))
			{
				SetFlagOff(ENF_INVISIBLE);
			}

			autowait(GetAnimLength(m_idMob_Action)*0.8f);
			m_bStop = TRUE;
		}

		return EReturn();
	}

	Fire(EVoid) : CEnemyBase::Fire
	{
		jump CEnemyBase::Fire();
	}

	Hit(EVoid) : CEnemyBase::Hit 
	{
		if( !IsAttackable() )
		{
			return EReturn();
		}

		if (m_MobType == MOB_ENEMY) 
		{
			autocall NormalAttack() EEnd;
		// should never get here
		} 
		else
		{
			ASSERT(FALSE);
		}
		return EReturn();
	};
		
	//0628 kwon
	AttackTarget(EVoid) : CEnemyBase::AttackTarget // 이 프로시저를 call하기전에 SetTargetEntity()가 선행되어야 한다.
	{
		m_vDesiredPosition = PlayerDestinationPos();
		
		INDEX attackAnimID = -1;
		const int iAttackMotion = m_nAttackCnt % MOB_ATTACK_MOTION_NUM;
		if( iAttackMotion == 0 )
		{
			attackAnimID = idMob_Attack;
		}
		else if( iAttackMotion == 1 )
		{
			attackAnimID = idMob_Attack2;
		}
		else if( iAttackMotion == 2 )
		{
			attackAnimID = idMob_Attack3;
		}
		else if( iAttackMotion == 3 )
		{
			attackAnimID = idMob_Attack4;
		}

		m_SkillEffectInfo.InitForNormalAttack(CMobData::getData(m_nMobDBIndex), attackAnimID);

		if(strlen(m_SkillEffectInfo.szEffectNameHit) == 0) {m_SkillEffectInfo.szEffectNameHit = "Normal Hit";}
		if(m_SkillEffectInfo.iFireDelayCount == 0)
		{
			m_SkillEffectInfo.iMissileType = CSkill::MT_NONE;
			m_SkillEffectInfo.iFireDelayCount = 1;
			m_SkillEffectInfo.fFireDelay[0] = GetAnimLength(idMob_Attack)/3;
		}
		autocall SkillAndMagicAnimation() EReturn;

		return EReturn();
	};

	SkillingTarget(EVoid) : CEnemyBase::SkillingTarget // 이 프로시저를 call하기전에 SetTargetEntity()가 선행되어야 한다.
	{
		m_vDesiredPosition = PlayerDestinationPos();
		
		CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);

		m_SkillEffectInfo.InitForSkillAttack(skill);
		autocall SkillAndMagicAnimation() EReturn;
	
		return EReturn();
	};

	IdleAnimation(EVoid) : CEnemyBase::IdleAnimation
	{
		autowait(15.0f + FRnd()*5.0f);
		GetModelInstance()->AddAnimation(idMob_Default2,AN_CLEAR,1.0f,0);
		//m_fIdleTime = _pTimer->CurrentTick();

		FLOAT animtime = GetAnimLength(idMob_Default2);
		autowait(animtime);
		StandingAnim();
		return EReturn();
		
	}

	AttackAnimation(EVoid) //0628
	{
		const int iAttackMotion = m_nAttackCnt % MOB_ATTACK_MOTION_NUM;
		if( iAttackMotion == 0 )
		{
			GetModelInstance()->AddAnimation(idMob_Attack, AN_CLEAR, 1.0f, 0);	
		}
		else if( iAttackMotion == 1 )
		{
			GetModelInstance()->AddAnimation(idMob_Attack2, AN_CLEAR, 1.0f, 0);	
		}
		else if( iAttackMotion == 2 )
		{
			GetModelInstance()->AddAnimation(idMob_Attack3, AN_CLEAR, 1.0f, 0);	
		}
		else if( iAttackMotion == 3 )
		{
			GetModelInstance()->AddAnimation(idMob_Attack4, AN_CLEAR, 1.0f, 0);	
		}

		m_fAttackFrequency = 0.25f;
		m_fAttackStartTime = _pTimer->CurrentTick();
		m_fImpactTime = GetAnimLength(idMob_Attack)/3;//0630 우선 공격 애니메이션의 절반에서 임팩트.

		while(_pTimer->CurrentTick() - m_fAttackStartTime < GetAnimLength(idMob_Attack)*0.8f){

			wait(m_fAttackFrequency) 
			{
				on (EBegin) : 
				{					
					if(m_bMoving)
					{						
					//	m_bStop = FALSE;		
					//	m_bAttack = FALSE;
					//	SetNoTargetEntity();	
						return EReturn();
					}

					m_fMoveSpeed = 0.0f; 
					ULONG ulFlags = SetDesiredMovement(); 				

					if(_pTimer->CurrentTick() - m_fAttackStartTime >m_fImpactTime) 
					{					
						SendEvent(EAttackDamage());
						m_fImpactTime = 1000.0f;//충분히 긴시간.
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
				on (EAttackDamage eAttackDamage) : 
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
					// Enemy에 ReceiveDamage를 전송합니다.
					// ReceiveDamage에서 Health가 0보다 작으면 엔티티를 Destroy하는 부분이 있음.
					this->InflictDirectDamage(m_penEnemy, this , DMT_CLOSERANGE, (int)1, vTargetPos, vDirection);
					// FIXME:
					//((CLiveEntity*)((CEntity*)m_penEnemy))->SetHealth(1000.0f);
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

		m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bStop = FALSE;		
		StandingAnim();
		SetNoTargetEntity();	

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
		m_fImpactTime = GetAnimLength(idMob_Attack)/3;

		GetModelInstance()->AddAnimation(m_SkillEffectInfo.iAnimatioID,AN_CLEAR,1.0f,0);

		m_nEffectStep = 1;

		if (m_pSkillReadyEffect != NULL)
		{
			DestroyEffectGroup(m_pSkillReadyEffect);
		}
		
		m_pSkillReadyEffect = StartEffectGroup(m_SkillEffectInfo.szEffectNameCast
			, &en_pmiModelInstance->m_tmSkaTagManager
			, _pTimer->GetLerpedCurrentTick());

		while(_pTimer->GetLerpedCurrentTick() - m_fAttackStartTime < GetAnimLength(m_SkillEffectInfo.iAnimatioID)*0.8f)
		{
			wait(m_fAttackFrequency) 
			{
				on (EBegin) : 
				{					
					if(m_bMoving || m_bStop)
					{
						DestroyEffectGroupIfValid(m_pSkillReadyEffect);
						GetModelInstance()->AddAnimation(idMob_Default,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);							
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

		if( !m_bUseAI )
		{
			m_bAttack = FALSE;
			m_bMoving = FALSE;
			m_bStop = TRUE;		
			//StandingAnim();
			SetNoTargetEntity();	
		}

		return EReturn();
	};

	// Normal attack
	NormalAttack(EVoid)
	{
		StandingAnim();
		autowait(0.8f + FRnd()*0.25f);

		INDEX attackAnimID = -1;
		const int iAttackMotion = rand() % MOB_ATTACK_MOTION_NUM;
		if( iAttackMotion == 0 )
		{
			attackAnimID = idMob_Attack;
		}
		else if( iAttackMotion == 1 )
		{
			attackAnimID = idMob_Attack2;
		}
		else if( iAttackMotion == 2 )
		{
			attackAnimID = idMob_Attack3;
		}
		else if( iAttackMotion == 3 )
		{
			attackAnimID = idMob_Attack4;
		}

		m_nAttackCnt = attackAnimID;

		if(m_bUseAI)
		{
// EDIT : BS : BEGIN
//			CPlacement3D pl		= GetLerpedPlacement();
//			_pNetwork->AddMoveList( 
//				GetNetworkID(),
//				pl.pl_PositionVector(1), 
//				pl.pl_PositionVector(3), 
//				pl.pl_PositionVector(2),
//				pl.pl_OrientationAngle(1) );
			_pNetwork->AddMoveList(*this);
			_pNetwork->SendMoveList();
// EDIT : BS : END
			INDEX iAttackerIndex	= GetNetworkID();
			_pNetwork->AddAttackList( 0, iAttackerIndex, m_penEnemy->GetNetworkType(), m_penEnemy->GetNetworkID() );			
		}

		autocall AttackTarget() EEnd;

		//autowait(0.60f);

		return EEnd();
	};
// Character.es에서 가져다 썼다
	Polymoph(EVoid): CCharacterBase::Polymoph //변신.
	{
		m_fActionAnimTime = _pTimer->CurrentTick();
		
		m_vDesiredPosition = GetPlacement().pl_PositionVector;
		StopMoving(); 

		while(_pTimer->CurrentTick() - m_fActionAnimTime < 1.8f)//변신 시간은 실제 1.8초
		{

			wait(0.1f) {
				on (EBegin) : {
					if(!m_bChanging)
					{
						return EReturn();
					}
					MinMaximize();
					resume; 
				}

				on (ETimer) : { stop; }	
			}
		}

		return EReturn();
	};
};