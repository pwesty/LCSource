19601
%{
#include "StdH.h"
#include "EntitiesMP/Common/PathFinding.h"
#include "EntitiesMP/NavigationMarker.h"
#include "EntitiesMP/TacticsHolder.h"
#include "EntitiesMP/PlayerWeapons.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIWildPetInfo.h>
%}

uses "EntitiesMP/Player";
uses "EntitiesMP/BasicEffects";
uses "EntitiesMP/Projectile";
uses "EntitiesMP/Debris";
uses "EntitiesMP/EnemyMarker";
uses "EntitiesMP/MusicHolder";
uses "EntitiesMP/BloodSpray";
uses "EntitiesMP/Unit";

event EWildPetBaseReconsiderBehavior {

};

// entity base sends this event to itself uppon receiving EDeath from the engine
event EWildPetBaseDeath {
	CEntityID eidInflictor, // 가해자 ?
};

// entity has been damaged
event EWildPetBaseDamage {
	CEntityID eidInflictor,	// entity that inflicted the damage
	FLOAT3D vDirection,		// where the damage came from (in absolute space)
	FLOAT3D vHitPoint,		// where the damage hit the entity (in absolute space)
	FLOAT fAmount,			// amount of damage done
	enum DamageType dmtType,	// type of damage
	FLOAT fHealth,
};

class export CWildPetBase : CUnit 
{
name	"WildPet Base";
thumbnail	"";
features	"HasName", "IsTargetable";

properties:
	1	CEntityPointer	m_penOwner,			// this is my owner
	2	CEntityPointer	m_penEnemy,			// target of enemy
	3	CEntityPointer	m_penKillEnemy,		// Killing have to a Entity
	4	CEntityPointer	m_penPathMarker,	// Current path finding marker
	5	CEntityPointer	m_penMarker "Marker" 'M' COLOR(C_RED|0xFF),		// 
	6	CEntityPointer	m_penSpray,			// the blood spray	(사용하지 않는다. 그래도 함수 구현을 위해 선언해 둔다.)
	7	CEntityPointer	m_penMainMusicHolder,	//
	8	CEntityPointer	m_penPlayerOfEnemy,	// 주인을 공격하는 적
	9	CEntityPointer	m_penPetOfEnemy,	// 펫을 공격하는 적

	10	CSoundObject	m_WildPetSound,

	11	CTString	m_strName	"Name"	'N' = "WildPet Base",
	12	CTString m_strDescription = "WildPet Base",

	13	enum EventEType	m_eeDeathType	"Death event type" 'F' = EET_TRIGGER,	// death event type(use event generate)
	14	enum SprayParticlesType	m_sptType = SPT_BEAST_PROJECTILE_SPRAY,	// type of particles
	15	enum wPetAIState m_wPetState = WPET_AI_FOLLOW,//WPET_AI_NONE,	// type of AI State

	16	BOOL m_bFadeOut = FALSE,
	17	BOOL m_bMoving = FALSE,
	18	BOOL m_bStop = TRUE,		// 처음 리젠 되면 멈춤
	19	BOOL m_bAttack = FALSE,
	20	BOOL m_bPreSkilling = FALSE,
	21	FLOAT m_fFadeStartTime = 0.0f,
	22	FLOAT m_fFadeTime = 0.0f,
	23	FLOAT m_fRangeLast = 0.0f,
	24	FLOAT m_fTacticVar3 = 0.0f,
	25	FLOAT m_fTacticVar4 = 0.0f,
	26	FLOAT m_fTacticVar5 = 0.0f,
	27	FLOAT m_fSpiritStartTime = 0.0f,			// time when spirit effect has started (죽었을 때 사용할 effect time?)
	28	FLOAT m_fAttackFrequency = 0.0f,
	29	FLOAT m_fAttackStartTime = 0.0f,
	30	FLOAT m_fImpactTime	= 0.0f,
	///////////////////////////우리 게임에서는 필요가 없음/////////////////////////////////////////////// 
/*	17	FLOAT m_tmSpraySpawned = 0.0f,				// time when damage has been applied
	18	FLOAT m_fSprayDamage = 0.0f,				// total ammount of damage
	19	FLOAT m_fMaxDamageAmmount = 0.0f,			// max ammount of damage received in last few ticks*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	31	FLOAT3D m_vLastStain = FLOAT3D(0, 0, 0),	// where last stain was left
	32	FLOAT3D m_vPlayerSpotted = FLOAT3D(0, 0, 0),	// where player was last spotted
	33	FLOAT3D m_vTacticsStartPosition = FLOAT3D(0, 0, 0),	
	34	INDEX m_nAttackCnt = 0,

	35	COLOR	m_colColor "Color"	'L' = 0x00	// color
	
	{
		CSelectedEntities	m_dcTargets;
	}
components:
	//1	class	CLASS_BLOOD_SPRAY	"Classes\\BloodSpray.ecl",		// Rander() 가 구현되어 있지 않다. Normal Hit 이펙트에서 bloodspray효과가 붙어있다.
	1 class   CLASS_BASIC_EFFECT    "Classes\\BasicEffect.ecl",

functions:
	void CWildPetBase(void)
	{

	};
	// 이하 일반 함수

//////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////
	// 필요없을 것 같은 함수(SlaveBase기준비교)
	// virtual CTString GetPlayerKillDescription(const CTString &strPlayerName, const EDeath &eDeath) // 누가 플레이어를 죽였는지 출력
	// virtual FLOAT GetCrushHealth(void)
	// virtual FLOAT &GetProp(FLOAT &m_fBase)
	// MaybeSwitchToAnotherPlayer()
	// SetTargetByCommand( INDEX iCommand )
	// void Precache(void)
	// FLOAT3D PlayerDestinationPos(void) --> MyOwnerDestinationPos(), EnemyDestinationPos() 주인과 적의 위치를 구분
	// IfTargetCrushed() : etouch 이벤트 발생시 crashHealth 값을 통해서 데미지를 Hit Entity에게 주기 위한 함수
	// 펫이 폭발형 데미지를 스킬을 사용치 않고 줄 상황도 없고, Touch 이벤트를 받을 만한 부분도 없다.
	// BOOL MovesByTargetedRoute(CTString &strTargetProperty) const : WorldEditor에만 사용하고, 설정된 Property Name을 통해서 Entity를
	// 가져온게 된다.(이함수는 단지 property name만 설정한다.) 사용안하는 것 같음
	// BOOL DropsMarker(CTFileName &fnmMarkerClass, CTString &strTargetProperty) const : 이것도 WorldEditor에서만 볼수 있음.
	// GetComputerMessageName()
	// void Read_t( CTStream *istr,BOOL bNetwork) : 
	// BOOL FillEntityStatistics(EntityStats *pes) : 오직 AI 목적으로 사용한다고 한다. 복잡하고 잘 모르겠으니 필요 없음 ^^;
	//
	// void ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType, FLOAT fDamageAmount,
	//	const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)  //CMovableModelEntity 에서  Override
	// : ReceiveDamage를 사용하지 않아도 무관하다. 데미지를 입었을 경우 데미지 이펙트만 표시되면 되기 때문.
	// 시리우스 샘에서 Health 부분을 처리 한듯 함.
	//
	// 	void AddToFuss(void)
	//	void RemoveFromFuss(void)
	// 	virtual BOOL CanHitEnemy(CEntity *penTarget, FLOAT fCosAngle) : 타겟 가능한 적인지 확인. CanAttackEnemy와 같아서 필요 없음
	//	WouldNotLeaveAttackRadius(void) // 이동 위치가 공격 범위를 벗어 나는가? (<StartPosition - CurrentPosition>.Length() < m_fAttackRadius)
	//	BOOL MayMoveToAttack(void) // 이동중에 공격범위를 벗어나지 않고 공격이 가능한가?
	//	virtual BOOL ShouldBlowUp(void)  // 폭발 데미지 처리를 위해 폭발 해야 하는지 확인하는 함수
	//	void BlowUpBase(void) // 전체적으로 쓰이지 않음 .. 폭발 데미지 처리함수(폭발 함수를 호출)
	//	void BlowUp(void) // 폭발 적용 함수
	//	virtual void AdjustDifficulty(void) // 엔티티의 이동속도, 공격속도 등을 향상시켜 난이도를 조절할 수 있게 하는 함수
	////////////////////////////////////////////

	virtual void OnEnd(void)
	{
		m_dcTargets.Clear();
		CUnit::OnEnd();
	}

	virtual void MovementAnimation(ULONG ulFlags) // 이동중 동작할 애니메이션을 선택하여 애니메이션을 진행한다.
	{
		if (ulFlags&MF_MOVEZ)
		{
			if (m_fMoveSpeed == GetProp(m_fAttackRunSpeed) || m_fMoveSpeed == GetProp(m_fAttackRunSpeed) || m_fMoveSpeed > GetProp(m_fWalkSpeed)) {
				// 달려야 하는 속도
				RunningAnim();
			} else {
				// 걸어야 하는 속도
				WalkingAnim();
			}
		}
		else if (ulFlags&MF_ROTATEH)
		{
			if (m_bAttack) { // 회전하지 않고 공격한다.
				AttackAnim();
			} else { // 회전한다.
				RotatingAnim();
			}
		}
		else
		{
			if (m_penEnemy != NULL && m_bAttack)	{ // 적 타겟이 있고, 공격을 할때
				AttackAnim();
			} else { // 그냥 정지해 있는다.
				StandingAnim();
			}
		}
	}

	BOOL SetTargetEntity(CEntity *pen) // CUnit에서 Override
	{
		// 탈것 여부 체크.
//		if( !_pNetwork->MyCharacterInfo.bWildPetRide )
		CWildPetData *m_Petdata;
		m_Petdata = CWildPetData::getData(en_pWildPetTarget->m_nIndex);
		
		if (m_Petdata->nFlag & WILDPET_FLAG_ATTACK)
		{
//			return FALSE;
//		}
//		{
			m_penEnemy = pen;
		}
		return TRUE;
	}

	BOOL SetEnemyEntity(CEntity *pen, int type)
	{
		if (pen == NULL)
		{
			return FALSE;
		}

		switch (type)
		{
		case 0:	// 주인을 공격하는 적
			{
				m_penPlayerOfEnemy = pen;
			}
			break;
		case 1: // 펫을 공격하는 적
			{
				m_penPetOfEnemy = pen;
			}
			break;
		}

		return TRUE;
	}

	void SetNoTargetEntity(void) // CUnit에서 Override
	{
		m_penEnemy = NULL; // 지정된 타겟을 해제한다.
	}

	BOOL SetKillTargetEntity(CEntity * penKill) // CUnit에서 Override(외부에서 설정)
	{
		m_penKillEnemy = penKill;
		return TRUE;
	}

	BOOL CheckTarget(CEntity* penTarget)
	{
		// 이하 조건이 만족 되면 TRUE

		return FALSE;
	}

	void DeathNow()
	{
		m_bStop = FALSE;
		m_bAttack = FALSE;
		m_bMoving = FALSE;
		if (en_pWildPetTarget)
		{
			en_pWildPetTarget->bDeath = TRUE;
		}		
		
		if (GetFlags() & ENF_ALIVE)
		{
			SendEvent(EDeath());
		}
	}

	void MoveNow()
	{
		m_bStop = FALSE;
		m_bAttack = FALSE;
		m_bMoving = TRUE;
		SendEvent(EWildPetBaseReconsiderBehavior());
	}

	void AttackNow()
	{
		m_bStop = FALSE;
		m_bAttack = TRUE;
		m_bMoving = FALSE;
		m_bSkilling = FALSE;
		m_bPreSkilling = FALSE;

		++m_nAttackCnt;
		SendEvent(EWildPetBaseReconsiderBehavior());
	}

	void StopNow()
	{
		m_bStop = TRUE;
		m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bSkilling = FALSE;
		m_bPreSkilling = FALSE;

		SendEvent(EWildPetBaseReconsiderBehavior());
	}

	void SkillNow()
	{
		m_bStop = FALSE;
		m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bSkilling = TRUE;
		m_bPreSkilling = FALSE;

		SendEvent(EWildPetBaseReconsiderBehavior());
	}

	void PreSkillNow()
	{
		m_bStop = FALSE;
		m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bSkilling = FALSE;
		m_bPreSkilling = TRUE;

		SendEvent(EWildPetBaseReconsiderBehavior());
	}

	void ActionNow()	// 액션을 취할 만한 내용이 없다.
	{
		m_bStop = FALSE;
		m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bSkilling = FALSE;
		m_bPreSkilling = FALSE;
		ForceFullStop(); // CMovableEntity의 이동 및 회전을 멈춤
		SendEvent(EWildPetBaseReconsiderBehavior());
	}

	void SendActEvent()
	{
		if (!m_bUseAI)
		{
			ActEventAnim();
		}
	}

	void StopandTeleport()
	{ // 강제 이동
		FLOAT3D vDeltaPlane = m_vDesiredPosition - GetPlacement().pl_PositionVector;

		if (vDeltaPlane.Length() > 0.5f)
		{
			vDeltaPlane(2) = 0;
			FLOAT3D vWhere = m_vDesiredPosition;
			vWhere(2) += 2.0f;
			vDeltaPlane.Normalize();
			CPlacement3D plWhere = CPlacement3D(vWhere, ANGLE3D(vDeltaPlane(1),0,0));
			this->Teleport(plWhere, FALSE);
		}

		m_vDesiredPosition = GetPlacement().pl_PositionVector;

		StopNow();
		StandingAnim();
	}

	// 테스트 결과 물리 적인 적용을 위해서 사용해야 한다.(데미지에 의해 상태 이상을 준다거나 할때)
	void ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType, FLOAT fDamageAmount,
		const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)  //CMovableModelEntity 에서  Override
	{
		// adjust damage
		// DamageStrength() 결과는 항상 1.0 (Eeibt == FRESH)
		FLOAT fNewDamage = fDamageAmount * DamageStrength((static_cast<EntityInfo*>(GetEntityInfo()))->Eeibt, dmtType);
		
		// apply game extra damaeg per enemy and per player
		// GetGameDamageMultiplier() 는 시리우스 샘에서 사용하는 인터페이스
		// 데미지가 적 또는 플레이어에게 분산 적용될때 비율 값을 돌려준다. or 추가 데미지로 사용 가능 or 데미지를 줄 수 없는 상황으로도 만듦.
		// 그러나 위 모든 사항은 시리우스 샘에서만 적용됨
		// fNewDamage *= GetGameDamageMultiplier();
		// if (fNewDamage == 0) {return;}

		// CLiveEntity에서 Entity의 고유의 Health 값이 있으며, 시리우스 샘의 고유의 Health정보로 판단된다.
		// 현재 싱글던전의 Entity를 모두 클라이언트가 관리 하기 때문에, CLiveEntity::en_fHealth 의 값이 0일때 Death 이벤트를 발생 시켜야 한다.
		// 아마도 Trap같은 Entity로 인한 부분으로 판단 되며, 테스트 결과 일반 CEnemy 와는 관계가 없음.
		// 싱글던전에서 캐릭터 및 몬스터 등에서는 Death이벤트를 발생하지 않도록, 데미지를 받을때 CLiveEntity::en_fHealth를 아주 크게 설정
		SetHealth(1000.0f);
		CMovableModelEntity::ReceiveDamage(penInflictor, dmtType, fNewDamage, vHitPoint, vDirection);
	}



//////////////////////////////////////////////////////////////////////////
	void SetTargetNone(void) // CWildPetBase에서만 사용(다른 Base클래스에서도 사용)
	{ // 일부 CUnit의 타겟 정보를 초기화
		m_ttTarget = TT_NONE;
		m_dtDestination = DT_PLAYERCURRENT;
		m_penEnemy = NULL;
	}

	BOOL SetTargetItem(CEntity *pItem)
	{
		if (pItem->GetFlags() & ENF_ITEM && pItem->GetModelInstance())
		{
			m_ttTarget = TT_SOFT;
			m_dtDestination = DT_PLAYERCURRENT;
			m_penEnemy = pItem;
			return TRUE;
		}

		return FALSE;
	}

	BOOL SetTargetSoft(CEntity *penPlayer)
	{
		CWildPetData *m_Petdata;
		m_Petdata = CWildPetData::getData(en_pWildPetTarget->m_nIndex);
		
		if (!(m_Petdata->nFlag & WILDPET_FLAG_ATTACK))
		{
			return FALSE;
		}

		// if invalid target
		//if (!IsValidForEnemy(penPlayer) && !((CPlayer*)CEntity::GetPlayerEntity(0))->CheckAttackTarget(-1, penPlayer, 0.0f))
		if (!((CPlayer*)CEntity::GetPlayerEntity(0))->CheckAttackTarget(-1, penPlayer, 0.0f))
		{ // do nothing
			return FALSE;
		}

		// remember new soft target
		CEntity *penOld = m_penEnemy;
		m_ttTarget = TT_SOFT;
		m_dtDestination = DT_PLAYERCURRENT;
		m_penEnemy = penPlayer;
		return (penOld != penPlayer);
	}

	BOOL SetTargetHard(CEntity *penPlayer) // TT_HARD 타입의 타겟이 아니면 새롭게 셋팅
	{
		// if invalid target
		if (!IsValidForEnemy(penPlayer) || m_ttTarget == TT_HARD)
		{	// do nothing
			return FALSE;
		}

		// remember new hard target
		CEntity *penOld = m_penEnemy;
		m_ttTarget = TT_HARD;
		m_dtDestination = DT_PLAYERCURRENT;
		m_penEnemy = penPlayer;
		return (penOld != penPlayer);
	}

	BOOL SetTargetHardForce(CEntity *penPlayer) // 무조건 TT_HARD 타입의 캐릭터 타겟을 새롭게 셋팅
	{
		// if invalid target
		if (!IsValidForEnemy(penPlayer) && !(penPlayer->IsCharacter()))
		{
			return FALSE;
		}

		// remember new hard target
		CEntity *penOld = m_penEnemy;
		m_ttTarget = TT_HARD;
		m_dtDestination = DT_PLAYERCURRENT;
		m_penEnemy = penPlayer;
		return (penOld != penPlayer);
	}

	export void Copy(CEntity & enOther,ULONG ulFlags)
	{
		CMovableModelEntity::Copy(enOther, ulFlags);
		CWildPetBase *penOther = static_cast<CWildPetBase *>(&enOther);
	}

	virtual FLOAT3D MyOwnerDestinationPos(void) // 자신의 주인을 따르기 위해 주인의 위치를 준다. 사용전 NULL 검사 필수
	{
		if (m_penOwner == NULL)
		{
			return GetPlacement().pl_PositionVector;
		}
		return m_penOwner->GetPlacement().pl_PositionVector;
	}

	virtual FLOAT3D EnemyDestinationPos(void) // 적의 위치를 돌려준다. 사용전 NULL검사 필수
	{
		if (m_penEnemy == NULL)
		{
			return GetPlacement().pl_PositionVector;
		}
		return m_penEnemy->GetPlacement().pl_PositionVector;
	}

	virtual FLOAT3D TargetDestinationPos(wPetAIState wpst)
	{
		switch (wpst)
		{
		case WPET_AI_FOLLOW:
			{
				return MyOwnerDestinationPos();
			}
			break;
		default:
			{
				return EnemyDestinationPos();
			}
			break;
		}
	}

	virtual CEntity* GetTargetEntity(wPetAIState wpst)
	{
		switch (wpst)
		{
		case WPET_AI_FOLLOW:
			{
				return m_penOwner;
			}
			break;
		default:
			{
				return m_penEnemy;
			}
			break;
		}
	}

	const CTString &GetDescription(void) const
	{
		((CTString&)m_strDescription).PrintF("-><none>");

		if (m_penMarker != NULL)
		{
			((CTString&)m_strDescription).PrintF("->%s", m_penMarker->GetName());
		}

		return m_strDescription;
	}

	BOOL AdjustShadingParameters(FLOAT3D &vLightDirection, COLOR &colLight, COLOR &colAmbient)
	{
		COLOR colAlpha;

		colAmbient = AddColors(colAmbient, m_colColor);

		if (m_bFadeOut)
		{
			FLOAT fTimeRemain = m_fFadeStartTime + m_fFadeTime - _pTimer->CurrentTick();

			if (fTimeRemain < 0.0f)
			{ fTimeRemain = 0.0f; }

			if (en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL)
			{
				colAlpha = GetModelInstance()->GetModelColor();
				colAlpha = (colAlpha&0xFFFFFF00) + (static_cast<COLOR>(fTimeRemain/m_fFadeTime*0xFF)&0xFF);
				GetModelInstance()->SetModelColor(colAlpha);
			}
			else
			{
				colAlpha = GetModelObject()->mo_colBlendColor;
				colAlpha = (colAlpha&0xFFFFFF00) + (static_cast<COLOR>(fTimeRemain/m_fFadeTime*0xFF)&0xFF);
				GetModelObject()->mo_colBlendColor = colAlpha;
			}
		}

		// CMovableModelEntity 에서는 AdjustShadingParameters를 Override하지 않았다.
		// CEntity::AdjustShadingParameters()가 호출 된다.(항상 TRUE만 리턴한다.)
		// return CMovableModelEntity::AdjustShadingParameters(vLightDirection, colLight, colAmbient);

		return TRUE;
	}
	
	// check if should give up attacking
	BOOL ShouldCeaseAttack(void)
	{
		if (m_penEnemy == NULL ||
			(!(m_penEnemy->GetFlags()&ENF_ALIVE) && !(m_penEnemy->GetFlags() & ENF_ITEM)) || (m_penEnemy->GetFlags()&ENF_DELETED))
		{ // cease attack
			return TRUE;
		}
		// otherwise, continue
		return FALSE;
	}

/************************************************************
 *                     MOVING FUNCTIONS                     *
 ************************************************************/
	// get movement frequency for attack
	virtual FLOAT GetAttackMoveFrequency(FLOAT fEnemyDistance)
	{
		return 0.5f;
	}

	virtual void SetSpeedsToDesiredPosition(const FLOAT3D &vPosDelta, FLOAT fPosDist, BOOL bGoingToPlayer)
	{
		FLOAT fCloseDistance = GetProp(m_fCloseDistance);
		FLOAT fStopDistance = GetProp(m_fStopDistance);

		// find relative direction angle
		FLOAT fCos = GetPlaneFrustumAngle(vPosDelta);

		// if may move and not too close
		if (fCos > CosFast(45.0f) && fPosDist > fStopDistance)
		{
			if (fPosDist < fCloseDistance) {
				m_fMoveSpeed = GetProp(m_fCloseRunSpeed);
				m_aRotateSpeed = GetProp(m_aCloseRotateSpeed);
			} else {
				m_fMoveSpeed = GetProp(m_fAttackRunSpeed);
				m_aRotateSpeed = GetProp(m_aAttackRotateSpeed);
			}
		}
		else if (fCos < CosFast(15.0f))
		{ // just rotate towards it
			m_fMoveSpeed = 0;

			if (fPosDist < fCloseDistance) {
				m_aRotateSpeed = GetProp(m_aCloseRotateSpeed);
			} else {
				m_aRotateSpeed = GetProp(m_aAttackRotateSpeed);
			}
		}
		else // otherwise (stop range) // 정지 범위에 있을때
		{ // if going towards player, or would leave attackradius
			if (bGoingToPlayer)	{ // stand in place
				m_fMoveSpeed = 0.0f;
				m_aRotateSpeed = 0.0f;
				// 여기서 AI 작동중이면

				// 아이템이 타겟으로 지정된 경우.
				if (m_bUseAI && m_wPetState == WPET_AI_ACTION)
				{
					if (m_penEnemy->GetFlags() & ENF_ITEM && m_penEnemy->GetModelInstance())
					{
						_pNetwork->SendPickMessage( this, (ULONG)m_penEnemy->GetNetworkID(), TRUE );
					}

					m_wPetState = WPET_AI_FOLLOW;
					m_bAIStart = TRUE;
				}
				else if (m_bUseAI && m_wPetState == WPET_AI_FOLLOW)
				{
					m_bAIStart = TRUE;
				}
			} else { // if going to some other location (some pathfinding AI scheme)
				m_fMoveSpeed = GetProp(m_fCloseRunSpeed);
				m_aRotateSpeed = GetProp(m_aCloseRotateSpeed);
			}
		}
	}

/************************************************************
 *                   ATTACK SPECIFIC                        *
 ************************************************************/
	// can attack (shoot) at entity in plane - ground support
	BOOL CanAttackEnemy(CEntity *penTarget, FLOAT fCosAngle)
	{
		// CEntity::IsPlayer()는 무조건 FALSE리턴, Override된 CPlayerEntity::IsPlayer()는 무조건 TRUE 리턴
		if (penTarget->IsPlayer()) 
		{
			return FALSE;
		}

		if (IsInPlaneFrustum(penTarget, fCosAngle))
		{
			if (m_bUseAI) {
				return ((CPlayer*)CEntity::GetPlayerEntity(0))->CheckAttackTarget(-1, penTarget, 0.0f); // 주인이 공격할 수 있는적은 펫도 공격할 수 있다.	
			} else {
				return IsVisibleCheckAll(penTarget);
			}
		}

		return FALSE;
	}

	// see entity
	BOOL SeeEntity(CEntity *pen, FLOAT fCosAngle)
	{
		if (IsInFrustum(pen, fCosAngle)) {
			return IsVisible(pen);
		}

		return FALSE;
	}

	// see entity in plane
	BOOL SeeEntityInPlane(CEntity *pen, FLOAT fCosAngle)
	{
		//CalcPlaneDist(pen);
		if (IsInPlaneFrustum(pen, fCosAngle)) {
			return IsVisible(pen);
		}

		return FALSE;
	}

/************************************************************
 *                 BLOW UP FUNCTIONS                        *
 ************************************************************/
// 폭발을 적용할 만한 부분이 없다.


/************************************************************
 *                CLASS SUPPORT FUNCTIONS                   *
 ************************************************************/
	// leave stain
	// 죽었을때 나올 이펙트 적용(얼룩을 남김.. 피를 흘린다거나)
	virtual void LeaveStain(BOOL bGrow)
	{
		ESpawnEffect ese;
		FLOAT3D vPoint;
		FLOATplane3D vPlaneNormal;
		FLOAT fDistanceToEdge;
		// get your size
		FLOATaabbox3D box;
		GetBoundingBox(box);

		// on plane
		if (GetNearestPolygon(vPoint, vPlaneNormal, fDistanceToEdge))
		{
			// if near to polygon and away from last stain point
			if ((vPoint-GetPlacement().pl_PositionVector).Length() < 0.5f && (m_vLastStain-vPoint).Length() > 1.0f)
			{
				m_vLastStain = vPoint;
				FLOAT fStretch = box.Size().Length();
				ese.colMuliplier = C_WHITE | CT_OPAQUE;
				// stain
				if (bGrow) {
					ese.betType = BET_BLOODSTAINGROW;
					ese.vStretch = FLOAT3D(fStretch*1.5f, fStretch*1.5f, 1.0f);
				} else {
					ese.betType = BET_BLOODSTAIN;
					ese.vStretch = FLOAT3D(fStretch*0.75f, fStretch*0.75f, 1.0f);
				}

				ese.vNormal = FLOAT3D(vPlaneNormal);
				ese.vDirection = FLOAT3D(0,0,0);
				FLOAT3D vPos = vPoint + ese.vNormal/50.0f * (FRnd() + 0.5f);
				CEntityPointer penEffect = CreateEntity(CPlacement3D(vPos, ANGLE3D(0,0,0)), CLASS_BASIC_EFFECT, WLD_AUTO_ENTITY_ID, FALSE);
				penEffect->Initialize(ese, FALSE);
			}
		}
	}

/************************************************************
 *                SOUND VIRTUAL FUNCTIONS                   *
 ************************************************************/

	// wounded -> yell --> 부상당했을때 크게 소리를 지른다.
	void WoundedNotify(const EWildPetBaseDamage &eEBDamage)
	{
		// if no enemy
		if (m_penEnemy == NULL)
		{ // do nothing
			return;
		}

		// if not killed from short distance
		if (eEBDamage.dmtType != DMT_CLOSERANGE && eEBDamage.dmtType != DMT_CHAINSAW)
		{ // yell  -- 소리 질러~~
			ESound eSound;
			eSound.EsndtSound = SNDT_YELL;
			eSound.penTarget = m_penEnemy;
			SendEventInRange(eSound, FLOATaabbox3D(GetPlacement().pl_PositionVector, 25.0f));
		}
	}

	// see enemy --> shout -> 적을 발견하고 크게 외치다.
	void SeeNotify()
	{
		// if no enemy
		if (m_penEnemy == NULL)
		{ // do nothing
			return;
		}
		
		ESound eSound;
		eSound.EsndtSound = SNDT_SHOUT;
		eSound.penTarget = m_penEnemy;
		SendEventInRange(eSound, FLOATaabbox3D(GetPlacement().pl_PositionVector, 50.0f));
	}

/************************************************************
 *          VIRTUAL FUNCTIONS THAT NEED OVERRIDE            *
 ************************************************************/
	virtual void ChargeAnim(void) {}
	virtual INDEX AnimForDamage(FLOAT fDamage) { return 0; }
	virtual void BlowUpNotify(void) {}
	virtual INDEX AnimForDeath(void) { return 0; }
	virtual FLOAT WaitForDust(FLOAT3D &vStretch) { return -1; }
	virtual void DeathNotify(void) {}
	virtual void IdleSound(void) {}
	// virtual void SightSound(void) {}
	virtual void WoundSound(void) {}
	virtual void DeathSound(void) {}
	virtual FLOAT GetLockRotationSpeed(void) { return 2000.0f; }
	virtual void AttackAnim(void) {}
	virtual void IdleLoop(void) {}
	virtual void StandUpAnim(void) {}
	virtual void AISearch() {}

	// render particles
	void RenderParticles(void)
	{
		// no particles when not existing
		if (GetRenderType() != CEntity::RT_MODEL && GetRenderType() != CEntity::RT_SKAMODEL)
		{
			return;
		}
		// if is dead
		if (m_fSpiritStartTime != 0.0f)
		{
			// 죽었을 때 파티클 렌더링
			const FLOAT tmNow = _pTimer->CurrentTick();
			// Particles_ModelGlow(this, tmNow + 20, PT_STAR08, 0.15f, 2, 0.03f, 0xFF00FF00);
			Particles_Death(this, m_fSpiritStartTime);
		}
	}

	// adjust sound and watcher parameters here it needed
	virtual void EnemyPostInit(void) {}

	/* Handle an event, return false if the event is not handled */
	// 이벤트 처리
	BOOL HandleEvent(const CEntityEvent &ee)
	{
		if (ee.ee_slEvent == EVENTCODE_ETouch) // GetCrushHealth 가 적용되면 각 터치한 엔티티에 데미지를 준다.
		{
/*			if (GetCrushHealth() != 0.0f) {
				ETouch eTouch = (static_cast<ETouch &>(ee));

				if (IsOfClass(eTouch.penOther, &CModelHolder2_DLLClass) ||
					IsOfClass(eTouch.penOther, &CModelHolder3_DLLClass) ||
					IsOfClass(eTouch.penOther, &CMovingBrush_DLLClass) ||
					IsOfClass(eTouch.penOther, &CDestroyableArchitecture_DLLClass))
				{
					InflictDirectDamage(eTouch.penOther, this DMT_EXPLOSION, GetCrushHealth(),
						eTouch.penOther->GetPlacement().pl_PositionVector, -(static_cast<FLOAT3D&>(eTouch.plCollision)));
				}
			}*/
		}
		else if (ee.ee_slEvent == EVENTCODE_EWildPetBaseDamage) // 데미지를 입었을 때
		{
/*			if (!_pNetwork->IsServer())	{
				en_fHealth = ((const EWildPetBaseDamage&)ee).fHealth;
				//SpawnBlood_client((const EWildPetBaseDamage&)ee); // 데미지를 받았을때 뿌릴 이펙트 (쓰지 않는다)
			}*/
		}
		else if (ee.ee_slEvent == EVENTCODE_EWildPetBaseDeath) // 죽음을 당했을 때
		{
			if (!CMovableModelEntity::HandleEvent(ee)) {
				if (en_ulFlags & ENF_ALIVE)
				{
					CPrintF("Warning : enemy death event not handled! Entity ID : %ld\b", en_ulID);

					if (!_pNetwork->IsServer()) {
						Destroy(FALSE);
						return TRUE;
					}
				}
			}

			return FALSE;
		}

		return CMovableModelEntity::HandleEvent(ee);
	}

	// returns length of animation
	FLOAT GetAnimLength(int iAnim)
	{
		if (en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) {
			return GetModelInstance()->GetAnimLength(iAnim);
		} else {
			return GetModelObject()->GetAnimLength(iAnim);
		}
	}

	// returns length of current anim length
	FLOAT GetCurrentAnimLength(void)
	{
		if (en_RenderType == RT_SKAMODEL) {
			return 0.5f;
		} else {
			return GetModelObject()->GetCurrentAnimLength();
		}
	}

	// is animation finished
	BOOL IsAnimFinished()
	{
		if (en_RenderType == RT_SKAMODEL) {
			return TRUE;
		} else {
			return GetModelObject()->IsAnimFinished();
		}
	}

	FLOAT3D &GetModelStretch()
	{
		if (en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) {
			return GetModelInstance()->mi_vStretch;
		} else {
			return GetModelObject()->mo_Stretch;
		}
	}

	// Stretch model
	void StretchModel(FLOAT3D vStretch)
	{
		if (en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) {
			GetModelInstance()->StretchModel(vStretch);
		} else {
			GetModelInstance()->StretchModel(vStretch);
		}
	}

	// Stretch single model
	void StretchSingleModel(FLOAT3D vStretch)
	{
		if (en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) {
			GetModelInstance()->StretchSingleModel(vStretch);
		} else {
			GetModelObject()->StretchSingleModel(vStretch);
		}
	}

	// returns bytes of memory used by this object (얻다 쓰는지 모르겠다. 그냥 쓰자)
	SLONG GetUsedMemory(void)
	{
		// initial
		SLONG slUsedMemory = sizeof(CWildPetBase) - sizeof(CMovableModelEntity) + CMovableModelEntity::GetUsedMemory();
		// add some more
		slUsedMemory += m_strDescription.Length();
		slUsedMemory += m_strName.Length();
		slUsedMemory += (1 * sizeof(CSoundObject));
		return slUsedMemory;
	}
	// 데미지를 받을 때 뿌려주는 이펙트 처리 // 데미지 이벤트를 받으면 호출
/*	void SpawnBlood_client(EWildPetBaseDamage eEBDamage) // 우리 게임에서는 사용하지 않는다. // 추후에 사용하는걸 고려해 보자.
	{
		FLOAT fNewDamage = eEBDamage.fAmount;
		DamageType dmtType = eEBDamage.dmtType;
		FLOAT3D vHitPoint = eEBDamage.vHitPoint;
		FLOAT3D vDirection = eEBDamage.vDirection;

		// if it has no spray, or if this damage overflows it, and not already disappearing
		if ((m_tmSparaySpawned <= _pTimer->CurrentTick() - _pTimer->TickQuantum * 8) ||
			m_fSprayDamage + fNewDamage > 50.0f && m_fSpiritStartTime == 0.0f && dmtType != DMT_CHAINSAW &&
			!(dmtType == DMT_BURNING && GetHealth() < 0.0f))
		{
			// spawn blood spray
			CPlacement3D plSpray = CPlacement3D(vHitPoint, ANGLE3D(0, 0, 0));
			m_penSpray = CreateEntity(plSpray, CLASS_BLOOD_SPRAY, WLD_AUTO_ENTITY_ID, FALSE);

			if (m_sptType != SPT_ELECTRICITY_SPARKS)
			{
				m_penSpray->SetParent(this);
			}

			ESpawnSpray eSpawnSpray;
			eSpawnSpray.colBurnColor = C_WHITE | CT_OPAQUE;

			if (m_fMaxDamageAmmount > 10.0f)
			{
				eSpawnSpray.fDamagePower = 3.0f;
			}
			else if (m_fSprayDamage + fNewDamage > 50.0f)
			{
				eSpawnSpray.fDamagePower = 2.0f;
			}
			else
			{
				eSpawnSpray.fDamagePower = 1.0f;
			}
			
			eSpawnSpray.sptType = m_sptType;
			eSpawnSpray.fSizeMultiplier = 1.0f;

			// setup direction of spray
			FLOAT3D vHitPointRelative = vHitPoint - GetPlacement().pl_PositionVector;
			FLOAT3D vReflectingNormal;
			GetNormalComponent(vHitPointRelative, en_vGravityDir, vReflectingNormal);
			vReflectingNormal.SafeNormalize();

			vReflectingNormal(1) /= 5.0f;

			FLOAT3D vProjectedComponent = vReflectingNormal * (vDirection % vReflectingNormal);
			FLOAT3D vSpilDirection = vDirection - vProjectedComponent*2.0f - en_vGravityDir*0.5f;
			
			eSpawnSpray.vDirection = vSpilDirection;
			eSpawnSpray.eidOwner = this;
			
			// initialize spray
			m_penSpray->Initialize(eSpawnSpray, FALSE);
			m_tmSpraySpawned = _pTimer->CurrentTick();
			m_fSprayDamage = 0.0f;
			m_fMaxDamageAmmount = 0.0f;
		}

		m_fSprayDamage += fNewDamage;
	}*/
procedures:
	// dummy main
	Main(EVoid)
	{
		return;
	}
//////////////////////////////////////////////////////////////////////////
// 필요 없을 것 같은 procedures 
// NewEnemySpotted() 새로운 적을 찾을 때 (pet AI시 사용할 수 있을지 두고 보자)
//////////////////////////////////////////////////////////////////////////

//**********************************************************
//                 ATTACK PROCEDURES
//**********************************************************
	// stop attack
	StopAttack(EVoid)
	{
		// no damager
		SetTargetNone();
		m_fDamageConfused = 0.0f;
		// stop moving
		StopMoving();

		return EReturn();
	}

	// initial preparation
	InitializeAttack(EVoid)
	{
		// disable blind/deaf mode
		SeeNotify();
		// make fuss
		//AddToFuss();
		// remember spotted position
		m_vPlayerSpotted = EnemyDestinationPos();

		// set timers
		if (CalcDist(m_penEnemy) < GetProp(m_fCloseDistance)) {
			m_fShootTime = 0.0f;
		} else {
			m_fShootTime = _pTimer->CurrentTick() + FRnd();
		}

		m_fDamageConfused = m_fDamageWounded;
		
		return EReturn();
	}

	// attack enemy
	WPetAction(EVoid)
	{
		// while you have some enemy
		//while (m_penEnemy != NULL)	// 여기서 적이 없어 질때까지 공격 수행
		//while ((m_wPetState == WPET_AI_NONE && m_penEnemy != NULL) || m_wPetState != WPET_AI_NONE)	// 여기서 적이 없어 질때까지 공격 수행
		while ((m_wPetState == WPET_AI_FOLLOW && m_penOwner != NULL && !m_bAIStart) || (m_wPetState != WPET_AI_FOLLOW) && m_penEnemy != NULL)
		{	// attack it
			autocall PerFormWPet() EReturn;
		}

		// stop attack
		autocall StopAttack() EReturn;

		// return to Active() procedure
		return EBegin();
	}

	// repeat attacking enemy until enemy is dead or you give up
	PerFormWPet(EVoid)
	{
		// reset last range
		m_fRangeLast = 1E9F;

		// set player's position as destination position
		//m_vDesiredPosition = EnemyDestinationPos();
		m_vDesiredPosition = TargetDestinationPos(m_wPetState);
		m_dtDestination = DT_PLAYERCURRENT;

		// repeat
		while (TRUE)
		{
			if (m_wPetState != WPET_AI_FOLLOW)
			{
				if (ShouldCeaseAttack())
				{
					// cease it (그만둠)
					SetTargetNone();
					m_wPetState = WPET_AI_FOLLOW;
					m_bAIStart = TRUE;
					return EReturn();
				}
			}

			if (m_wPetState == WPET_AI_FOLLOW)
			{
				if (m_bAIStart)
				{
					return EReturn();
				}

				// AI 적용되어 있지 않을때
				if (!SE_Get_UIManagerPtr()->GetWildPetInfo()->GetAIActive())
				{
					CEntity* pTmpEnt = ((CPlayer*)CEntity::GetPlayerEntity(0))->GetPlayerWeapons()->m_penRayHitTmp;
					//CEntity* pTmpEnt = ((CPlayer*)CEntity::GetPlayerEntity(0))->GetPlayerWeapons()->m_penRayHitNow;

					if (pTmpEnt != NULL && pTmpEnt != m_penEnemy)
					{
						if (SetTargetItem(pTmpEnt))	//  아이템 줍기 테스트
						{
							m_wPetState = WPET_AI_ACTION;
							return EReturn();
						}
						else if (pTmpEnt->IsFlagOn(ENF_ALIVE))
						{
							if (SetTargetSoft(pTmpEnt))
							{
								m_wPetState = WPET_AI_NORMALATTACK;
								return EReturn();
							}
						}
					}
				}
			}
			else if (m_wPetState == WPET_AI_NORMALATTACK)
			{
				if (GetTargetEntity(m_wPetState) == NULL || GetTargetEntity(m_wPetState)->IsFlagOff(ENF_ALIVE) || m_penOwner == m_penEnemy )
				{
					m_wPetState = WPET_AI_FOLLOW;
					m_bAIStart = TRUE;
					return EReturn();
				}
			}

			// get distance from enemy (적(플레이어~)과의 거리 측정)
			//FLOAT fEnemyDistance = CalcDist(m_penEnemy);
			FLOAT fEnemyDistance = CalcDist(GetTargetEntity(m_wPetState));
			// if just entered close range(방금 가까운 범위에 들어 갔으면...)
			if (m_fRangeLast > GetProp(m_fCloseDistance) && fEnemyDistance <= GetProp(m_fCloseDistance))
			{
				// reset shoot time to make it attack immediately(즉시 공격 형태로)
				// 시리우스 샘에서 사용
				m_fShootTime = 0.0f;
			}

			m_fRangeLast = fEnemyDistance;

			// determine movement frequency depending on enemy distance or path finding
			// 이동 횟수를 계산. 에너미(플레이어)와의  거리 또는 길찾기경로에 따라서 
			// 그러나 GetAttackMoveFrequency()에서 구현되어 있지 않다.
			m_fMoveFrequency = GetAttackMoveFrequency(fEnemyDistance);

			// wait one time interval
			wait(m_fMoveFrequency)
			{
				on (ETimer) :
				{
					stop;
				}

				// initially
				on (EBegin) :
				{
					// if you haven't fired/hit enemy for some time
					if (_pTimer->CurrentTick() > m_fShootTime)
					{
						// if you can see target
						// owner or enemy
						//if (IsVisible(m_penEnemy))
						if (IsVisible(GetTargetEntity(m_wPetState)))
						{
							// remember spotted position
							//m_vPlayerSpotted = EnemyDestinationPos();
							m_vPlayerSpotted = TargetDestinationPos(m_wPetState);
							// if going to player spotted or temporary path position
							if (m_dtDestination == DT_PLAYERSPOTTED || m_dtDestination == DT_PATHTEMPORARY)
							{ // switch to player current position
								m_dtDestination = DT_PLAYERCURRENT;
							}
						}
						else
						{	// if going to player's current position
							if (m_dtDestination == DT_PLAYERCURRENT)
							{	// switch to position where player was last seen
								m_dtDestination = DT_PLAYERSPOTTED;
							}
						}

						// try firing / hitting again now
						call FireOrHit();
					}
					else
					{
						// if going to player spotted or temporary path position and you just seen the player
						//if ((m_dtDestination == DT_PLAYERSPOTTED || m_dtDestination == DT_PATHTEMPORARY) && IsVisible(m_penEnemy))
						if ((m_dtDestination == DT_PLAYERSPOTTED || m_dtDestination == DT_PATHTEMPORARY) && IsVisible(GetTargetEntity(m_wPetState)))
						{	// switch to player current position
							m_dtDestination =  DT_PLAYERCURRENT;
							// remember spotted position
							m_vPlayerSpotted = TargetDestinationPos(m_wPetState);
						}
					}

					// if you are not following the player and you are near current destination position
					FLOAT fAllowedError = m_fMoveSpeed * m_fMoveFrequency * 2.0f;

					if (m_dtDestination == DT_PATHPERSISTENT || m_dtDestination == DT_PATHTEMPORARY)
					{
						fAllowedError = ((CNavigationMarker&)*m_penPathMarker).m_fMarkerRange;
					}

					if (m_dtDestination != DT_PLAYERCURRENT && (CalcDistanceInPlaneToDestination() < fAllowedError || fAllowedError < 0.1f))
					{ // if going to where player was last spotted
						if (m_dtDestination == DT_PLAYERSPOTTED)
						{ // if you see the target
							//if (IsVisible(m_penEnemy))
							if (IsVisible(GetTargetEntity(m_wPetState)))
							{	// switch to following player
								m_dtDestination = DT_PLAYERCURRENT;
							}
							// if you don't see him
							// if using pathfinding
						}
					}

					// if following player
					if (m_dtDestination == DT_PLAYERCURRENT)
					{	// set target's position as destination position
						//m_vDesiredPosition = EnemyDestinationPos();
						m_vDesiredPosition = TargetDestinationPos(m_wPetState);
					}

					if (IsMovable()) // 하위 클래스에서 구현
					{
						// set speeds for movement towards desired position (목적지와 현재의 위치상의 거리 계산)
						FLOAT3D vPosDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;
						FLOAT fPosDistance = vPosDelta.Length();

						if (vPosDelta(2) > 10.f)
						{
							en_plPlacement.pl_PositionVector = m_vPlayerSpotted;
							en_plPlacement.pl_PositionVector(2) += 1.f;
							StopMove();

							//LOG_DEBUG("Stop Move -P2 %.2f %.2f %.2f", en_plPlacement.pl_PositionVector(1), en_plPlacement.pl_PositionVector(3), en_plPlacement.pl_PositionVector(2));
							resume;
						}

						SetSpeedsToDesiredPosition(vPosDelta, fPosDistance, TRUE);

						// adjust driection and speed
						ULONG ulFlags = SetDesiredMovementEx();
						MovementAnimation(ulFlags);
					}
					else
					{
						SetDesiredMovement();
						MovementAnimation(0);
					}

					resume;
				}

				// if touched something
				on (ETouch eTouch) : { pass; }
				// if came to an edge
				on (EWouldFall eWouldFall) : { pass; }
				// pass all death events
				on (EDeath eDeath) : 
				{
					EWildPetBaseDeath eEBDeath;
					eEBDeath.eidInflictor = (CEntity*)eDeath.eLastDamage.penInflictor;
					SendEvent(eEBDeath,TRUE);
					resume;		// 계속 wait
				}
				on (EWildPetBaseDeath eEBDeath) :
				{
					m_bWasKilled = TRUE; 
					// die
					jump Die(eEBDeath);		// 죽었으면 프로시저를 멈추고 Die프로시저로 넘어가서 끝냄.
				}

				on (ESound) : { resume; } // ignore all sounds
				// on (EWatch) : { resume; } // ignore watch
				on (EReturn) : { stop; } // returned from subprocedure
			}
		}
	}

	// fire or hit the enemy if possible
	FireOrHit()
	{
		//if (CalcDist(m_penEnemy) < GetProp(m_fAttackDistance) && CanAttackEnemy(m_penEnemy, Cos(AngleDeg(45.0f))))
		if (CalcDist(GetTargetEntity(m_wPetState)) < GetProp(m_fAttackDistance) && CanAttackEnemy(GetTargetEntity(m_wPetState), Cos(AngleDeg(45.0f))))
		{
			// make fuss
			// AddToFuss();
			// stop moving (rotation and translation)
			StopMoving();
			// set next shoot time
			//if (CalcDist(m_penEnemy) < GetProp(m_fCloseDistance)) {
			if (CalcDist(GetTargetEntity(m_wPetState)) < GetProp(m_fCloseDistance)) {
				m_fShootTime = _pTimer->CurrentTick() + GetProp(m_fCloseFireTime) * (1.0f + FRnd()/3.0f);
			} else {
				m_fShootTime = _pTimer->CurrentTick() + GetProp(m_fAttackFireTime) * (1.0f + FRnd()/3.0f);
			}

			m_bAIStart = TRUE;
			// fire
			autocall Hit() EReturn;
		}
		else
		{	// if cannot fire or hit
			// make sure we don't retry again too soon
			m_fShootTime = _pTimer->CurrentTick() + 0.25f;
		}

		// return to caller
		return EReturn();
	}
//**********************************************************
//                 COMBAT IMPLEMENTATION
//**********************************************************
	// this is called to hit the player when enar
	Hit(EVoid)
	{
		return EReturn();
	}

	// this is called to shoot at player when far away or otherwise unreachable
	Fire(EVoid)
	{
		 return EReturn();
	}
//**********************************************************
//                 COMBAT HELPERS
//**********************************************************

//**********************************************************
//             WOUNDING AND DYING PROCEDURES
//**********************************************************
	// play wounding animation
	BeWounded(EWildPetBaseDamage eEBDamage)
	{
		StopMoving();

		return EReturn();
	}

	// we get here once the enemy is dead
	Die(EWildPetBaseDeath eEBDeath)
	{
		SetCollisionFlags(ECF_IMMATERIAL);

		// not alive anymore
		SetFlagOff(ENF_ALIVE);
		// should be handled on the client ide (to fadeout and selfdestroy)
		SetFlagOn(ENF_CLIENTHANDLING);
		
		// find the one who killed, or other best suitable player
		CEntityPointer penKiller = static_cast<CEntity*>(eEBDeath.eidInflictor);
		
		if (penKiller == NULL)
		{
			if (m_penEnemy != NULL) {
				penKiller = m_penEnemy;
			} else {
				penKiller = FixupCausedToPlayer(this, penKiller, FALSE); // 가장 가까운 플레이어를 찾는다.
			}
		}

		// see what killed the enemy
		/*KillType ekt = KT_OTHER;
		if (penKiller == NULL) {
			ekt = KT_OTHER;
		} else if (IsOfClass(eEBDeath.eidInflictor, &CPlayer_DLLClass)) {
			ekt = KT_PLAYER;
		} else if (IsDerivedFromClass(static_cast<CEntity*>(eEBDeath.eidInflictor), &CWildPetBase_DLLClass)) {
			ekt = KT_ENEMY;
		}*/

		MarkEnemyDead(this);
		
		wait()
		{
			// initially
			on (EBegin) : 
			{
				// invoke death animation sequence
				call DeathSequence();
			}

			on (EEnd) : 
			{ // stop waiting
				stop;
			}
		}
		
		// stop making fuss
		//RemoveFromFuss();
		_pNetwork->DelWildPetTarget(en_lNetworkID);

		return;
	}

	Death(EVoid)
	{
		StopMoving();	// stop moving
		DeathSound();	// death sound
		LeaveStain(FALSE);

		// set physic flags
		SetPhysicsFlags(EPF_MODEL_CORPSE);
		SetCollisionFlags(ECF_CORPSE);
		SetFlags(GetFlags() | ENF_SEETHROUGH);

		// stop making fuss
		//RemoveFromFuss();

		// death notify (usually change collision box and change body density)
		DeathNotify();

		// start death anim
		INDEX iAnim = AnimForDeath();

		// use tatic variables for temporary data ( 전술 같은건 없는데~)
		m_vTacticsStartPosition = FLOAT3D(1, 1, 1);
		m_fTacticVar4 = WaitForDust(m_vTacticsStartPosition);
		// remember start time
		m_fTacticVar5 = _pTimer->CurrentTick();
		// mark that we didn't spawned dust yet
		m_fTacticVar3 = -1;

		autowait(0.5f);

		// if no dust should be spawned --> m_fTacticVar4 < 0
		if (m_fTacticVar4 >= 0)
		{ // should spawn dust
			while (_pTimer->CurrentTick() < (m_fTacticVar5 + GetCurrentAnimLength()))
			{
				autowait(_pTimer->TickQuantum);

				if (en_penReference != NULL && _pTimer->CurrentTick() >= (m_fTacticVar5 + m_fTacticVar4) && m_fTacticVar3 < 0)
				{ // spawn dust effect
					CPlacement3D plFX = GetPlacement();
					ESpawnEffect ese;
					ese.colMuliplier = C_WHITE | CT_OPAQUE;
					ese.vStretch = m_vTacticsStartPosition;
					ese.betType = BET_DUST_FALL;
					CPlacement3D plSmoke = plFX;
					plSmoke.pl_PositionVector += FLOAT3D(0, 0.035f * m_vTacticsStartPosition(2), 0);
					CEntityPointer penFX = CreateEntity(plSmoke, CLASS_BASIC_EFFECT);
					penFX->Initialize(ese);
					penFX->SetParent(this);
					// mark that we spawned dust
					m_fTacticVar3 = 1;
				}
			}
		}

		return EEnd();
	}

	DeathSequence(EVoid)
	{
		// not alive anymore
		SetFlagOff(ENF_ALIVE);
		// should be handled on the client side (to fadeout and selfdestroy)
		SetFlagOn(ENF_CLIENTHANDLING);

		// entity death
		autocall Death() EEnd;

		// check if you have attached flame
		CEntityPointer penFlame = GetChildOfClass("Flame");

		if (penFlame != NULL)
		{
			// send the event to stop burning
			EStopFlaming esf;
			esf.m_bNow = FALSE;
			penFlame->SendEvent(esf);
		}

		FLOAT fWaitTime = 2.0f;
		autowait(fWaitTime);

		// start fading out and turning into stardust effect
		m_fSpiritStartTime = _pTimer->CurrentTick();
		m_fFadeStartTime = _pTimer->CurrentTick();
		m_fFadeTime = 4.0f;
		m_bFadeOut = TRUE;
		// become passable even if very large corpse
		SetCollisionFlags(ECF_CORPSE &~ ((ECBI_PROJECTILE_MAGIC | ECBI_PROJECTILE_SOLID)<<ECB_TEST));

		// wait for fading
		autowait(m_fFadeTime);

		FLOAT fEffectWait = 6.0f;
		// wait for the stardust effect
		autowait(fEffectWait);

		return EEnd();
	}

//**********************************************************
//                MAIN LOOP PROCEDURES
//**********************************************************
	// move
	// 엔티티가 활성화된 상태
	Active(EVoid)
	{
		m_fDamageConfused = 0.0f;

		// logic loop
		wait()
		{
			// initially
			on (EBegin) :
			{
				// start new behavior //EWildPetBaseReconsiderBehavior
				SendEvent(EWildPetBaseReconsiderBehavior());
				resume;
			}

			// if new behavior is requested
			on (EWildPetBaseReconsiderBehavior) :
			{
				if (m_bUseAI) // AI 작동
				{
					if (m_bAIStart)
					{ // 여기서 행동 판단
						AISearch();
						m_bAIStart = FALSE;
					}
				}

				if (m_penEnemy != NULL || (m_wPetState == WPET_AI_FOLLOW && m_penOwner != NULL))
				{
					if (m_bSkilling)
					{
						m_bSkilling = FALSE;
						call SkillingTarget();
					}
					else if (m_bPreSkilling)
					{
						m_bPreSkilling = FALSE;
						call SpellSkill();
					}
					else if(m_bAttack)
					{
						m_bAttack = FALSE;
						call AttackTarget();
					}
					else
					{
						if (m_bUseAI) {
							call WPetAction();
						} else {
							SetNoTargetEntity();

							if (m_bStop) // 리젠 되었을때 또는 이동을 멈출때
							{
								call StopMovement();
							}
							else if(m_bMoving)
							{
								call MovetoPoint();
							}
						}
					}
				}
				else
				{
					if (m_bUseAI && m_wPetState != WPET_AI_FOLLOW)
					{
						m_wPetState = WPET_AI_FOLLOW;
						m_bAIStart = TRUE;
					}

					if (m_bStop) // 리젠 되었을때 또는 이동을 멈출때
					{
						call StopMovement();
					}
					else if(m_bMoving)
					{
						call MovetoPoint();
					}
				}

				resume;
			}

			// on return from some of the sub-procedures
			on (EReturn) :
			{
				// start new behavior(새로운 행동을 시작함)
				SendEvent(EWildPetBaseReconsiderBehavior());
				resume;
			}
			
			// if damaged, send the message to yourself to apply the damage (so the client side would get hurt too)
			on (EDamage eDamage) :
			{
				if (_pNetwork->IsServer())
				{
					EWildPetBaseDamage eEBDamage;
					eEBDamage.eidInflictor = static_cast<CEntity*>(eDamage.penInflictor);
					eEBDamage.vDirection = eDamage.vDirection;
					eEBDamage.vHitPoint = eDamage.vHitPoint;
					eEBDamage.fAmount = eDamage.fAmount;
					eEBDamage.dmtType = eDamage.dmtType;
					eEBDamage.fHealth = en_fHealth;
					SendEvent(eEBDamage, TRUE);
					resume;
				}
			}
			// if received
			on (EWildPetBaseDamage eEBDamage) :
			{
				// if should already blow up
				m_fDamageConfused -= eEBDamage.fAmount;
				if (m_fDamageConfused < 0.001f)
				{
					m_fDamageConfused = m_fDamageWounded;

					WoundSound();

					// play wounding animation
					if (!m_bAttack && !m_bMoving)
					{
						call BeWounded(eEBDamage);
					}
					else
					{
						INDEX animSet, animIdx;
						INDEX animId = ska_StringToID("mldam");

						if (this->en_pmiModelInstance && this->en_pmiModelInstance->FindAnimationByID(animId, &animSet, &animIdx))
						{ // 타격 모션
							this->en_pmiModelInstance->AddAnimation(animId, AN_REMOVE_ON_END, 1.0f, 0);
						}
					}
				}

				resume;
			}

			on (EDeath eDeath) :
			{
				EWildPetBaseDeath eEBDeath;
				eEBDeath.eidInflictor = static_cast<CEntity*>(eDeath.eLastDamage.penInflictor);
				SendEvent(eEBDeath, TRUE);
				resume;
			}

			on (EWildPetBaseDeath eEBDeath) :
			{
				m_bWasKilled = TRUE;
				jump Die(eEBDeath);	// 죽었으면 프로시저를 멈추고 Die 프로시저로 넘어가서 끝냄.
			}

			// on touch
			on (ETouch eTouch) : 
			{
				pass;
			}
		}
	}

	// overridable called before main enemy loop actually begins
	PreMainLoop(EVoid)
	{
		return EReturn();
	}

	MainLoop_internal()
	{
		if (m_bWasKilled)
		{
			Destroy(FALSE);
			return;
		}

		if (m_fStepHeight == -1)
		{
			m_fStepHeight = 2.0f;
		}

		SetFlagOn(ENF_CLIENTHANDLING);
		autowait(_pTimer->TickQuantum); // 1/20초 기다림
		SetFlagOff(ENF_CLIENTHANDLING);

		//m_vStartPosition = GetPlacement().pl_PositionVector;

		// set sound default parameters
		//m_WildPetSound.Set3DParameters(100.0f, 10.0f, 1.0f, 1.0f);
		
		// adjust falldown and step up values
		en_fStepUpHeight = m_fStepHeight + 0.01f;
		en_fStepDnHeight = m_fFallHeight + 0.01f;

		// let derived class(es) adjust parameters if needed
		EnemyPostInit();

		ASSERT(m_fStopDistance >= 0);
		ASSERT(m_fCloseDistance >= 0);
		ASSERT(m_fIgnoreRange > m_fAttackDistance);
		autocall PreMainLoop() EReturn;

		jump StandardBehavior(); // 현재 프로시저를 멈추고 StandardBehavior를 수행함
	}

	// main entry point for enemy behavior
	MainLoop(EVoid)
	{
		if (_pNetwork->IsServer())
		{
			jump MainLoop_internal();
		}
	}

	StandardBehavior(EVoid)
	{
		wait()
		{
			on (EBegin) :
			{
				call Active();
			}

			on (EWildPetBaseDamage eEBDamage) :
			{
				resume;
			}

			on (EDeath eDeath) :
			{
				EWildPetBaseDeath eEBDeath;
				eEBDeath.eidInflictor = static_cast<CEntity*>(eDeath.eLastDamage.penInflictor);
				SendEvent(eEBDeath, TRUE);
				resume;
			}

			on (EWildPetBaseDeath eEBDeath) :
			{
				m_bWasKilled = TRUE;
				jump Die(eEBDeath);	// 죽었으면 프로시저를 멈추고 Die 프로시저로 넘어가서 끝냄.
			}

			// if an entity exits a teleport nearby
			on (ETeleport et) :
			{
				// proceed message to watcher (so watcher can quickly recheck for players)
				resume;
			}
		}
	}

	AttackTarget() // 이 프로시저를 call 하기전에 SetTargetEntity()가 선행 되어야 한다.
	{
		return EReturn();
	}

	SkillingTarget() // override..
	{
		return EReturn();
	}

	SpellSkill() // override...
	{
		return EReturn();
	}

	StopMovement()
	{
		m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bStop = FALSE;

		SetNoTargetEntity();
		StopMoving();

		StandingAnim();
		return EReturn();
	}

	MovetoPoint() // 호출전에 m_fMoveSpeed 와 m_vDesiredPosition를 설정해줘야 한다.
	{
		// SetNoTargetEntity();
		m_aRotateSpeed = AngleDeg(1800.0f);
		m_fMoveFrequency = 0.5f;
		m_dtDestination = DT_PLAYERCURRENT;

		while (TRUE)
		{
			wait(m_fMoveFrequency)
			{
				on (EBegin) :
				{
					// 공격하려고 하는 경우에는 움직이던 것을 멈춤
					if (m_bAttack)
					{
						m_bMoving = FALSE;
						EReturn();
					}

					// set speeds for movement towards desired position
					FLOAT3D vPosDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;
					FLOAT fPosDistance = vPosDelta.Length();

					SetSpeedsToDesiredPosition(vPosDelta, fPosDistance, m_dtDestination == DT_PLAYERCURRENT);

					// adjust direction and speed
					ULONG ulFlags = SetDesiredMovement();
					MovementAnimation(ulFlags);
					resume;
				}

				on (ETimer) : { stop; }
			}
		}

		// m_bAttack = FALSE; m_bStop = FALSE;
		m_bMoving = FALSE;

		m_aRotateSpeed = 0.0f;
		m_fMoveSpeed = 0.0f;
		// m_vDesiredPosition = GetPlacement().pl_PositionVector;
		// StopMoving();
		StandingAnim();
		return EReturn();
	}

};