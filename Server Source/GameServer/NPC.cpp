#include "stdhdrs.h"

#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "Exp.h"
#include "doFunc.h"

extern void DelAttackList(CCharacter* ch);

//////////////
// CNPC member
CNPC::CNPC()
{
	m_type = MSG_CHAR_NPC;

	m_proto = NULL;
	m_idNum = -1;
	m_moveArea = 0;
	m_flag = 0;
	m_delay = 0;
	m_pulseAttack = 0;
	m_regenX = 0;
	m_regenY = 0;
	m_regenZ = 0;
	m_disableTime = 0;
	m_bStop = true;
	m_pNPCPrev = NULL;
	m_pNPCNext = NULL;
	m_aipulse = gserver->m_pulse;
	m_attackType = MSG_DAMAGE_MELEE;
	m_postregendelay = gserver->m_pulse;

	m_moveDir = 0;
	m_nBlockAI = 0;
	m_bChangeDir = false;
	m_bFail45 = false;

	m_nBlockRegen = 0;

	m_bMoveRun = false;

	// 050221 : bs : NPC 스킬
	memset(m_skills, 0, sizeof(CSkill*) * MAX_NPC_SKILL);
	// --- 050221 : bs : NPC 스킬

	memset(m_quest, 0, sizeof(int) * QUEST_MAX_NPC);
	m_nQuestCount = 0;

	m_regenInfo = NULL;

	m_nRaidMoveTime = gserver->m_pulse + GetRandom(PULSE_MONSTER_RAID_MOVE * 9 / 10, PULSE_MONSTER_RAID_MOVE * 11 / 10);
	m_bRaidNPCSummon = false;

	m_ctCount = 0;
	m_ctTime = -1;

	m_MobScrollType = -1;
	m_NextMobIndex = -1;
	m_UseCharIndex = -1;

	m_coinidx = 0;
	m_coincount = 0;

#ifdef MONSTER_AI
	m_nOldDist = 0;
	m_pulseMoveLock = 0;
	m_bMoveLock = false;
	m_bMoveToRegen = false;
#endif

#ifdef EXTREME_CUBE
	m_bCubeRegen = false;
#endif

	m_Mob_State = 0;
	m_owner	= NULL;
	m_pulseSoulRecover = 0;

	m_pMercenaryClassData = NULL;

	m_lifetime = 0;
}

CNPC::~CNPC()
{
	m_assist.ClearAssist(false, false, true, true, true);

	m_proto = NULL;
	m_regenInfo = NULL;

	int i;
	for (i = 0; i < MAX_NPC_SKILL; i++)
	{
		if (m_skills[i])
		{
			if (m_currentSkill == m_skills[i])
				m_currentSkill = NULL;
			delete m_skills[i];
		}
		m_skills[i] = NULL;
	}
	if (m_currentSkill)
		delete m_currentSkill;
	m_currentSkill = NULL;

	m_Mob_State = 0;
	m_owner	= NULL;
	m_pulseSoulRecover = 0;
	if( m_pMercenaryClassData )
	{
		delete [] m_pMercenaryClassData;
		m_pMercenaryClassData = NULL;
	}
}

void CNPC::DeleteNPC()
{
	DelAttackList(this);
	delete this;
}

void CNPC::InitAIVar()
{
	m_bChangeDir = false;
	m_nBlockAI = 0;
	m_bFail45 = false;
}

void CNPC::ResetStat()
{
	m_maxHP			= m_proto->m_hp;
	m_maxMP			= m_proto->m_mp;
	m_walkSpeed		= m_proto->m_walkSpeed;
	//m_runSpeed		= m_proto->m_runSpeed;
	SetRunSpeed(m_proto->m_runSpeed);
	m_recoverHP		= m_proto->m_recoverHP;
	m_recoverMP		= m_proto->m_recoverMP;
	m_str			= m_proto->m_str;
	m_dex			= m_proto->m_dex;
	m_int			= m_proto->m_int;
	m_con			= m_proto->m_con;
	m_attackSpeed	= m_proto->m_attackSpeed;
	m_attackType	= m_proto->m_attackType;
	if (m_attackType == MSG_DAMAGE_MELEE)
		m_eqMelee	= m_proto->m_attack;
	else if (m_attackType == MSG_DAMAGE_RANGE)
		m_eqRange	= m_proto->m_attack;
	m_eqMagic		= m_proto->m_magic;
	m_eqDefense		= m_proto->m_defense;
	m_eqResist		= m_proto->m_resist;
	m_attackRange	= m_proto->m_attackArea;
	m_statPall		= 0;
	m_statPall_per	= 0;
	m_attrdef = m_proto->m_attrdef;
	m_attratt = m_proto->m_attratt;

	// 리젠되는 NPC의 파라미터를 리젠 정보에서 가져와 조정
	if (m_regenInfo)
	{
		m_maxHP += m_regenInfo->m_paramHP;
	}

	if( GetOwner() && this == GetOwner()->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY) && GetMercenaryClassData() )
	{
		m_str = GetOwner()->m_str;
		m_dex = GetOwner()->m_dex;
		m_int = GetOwner()->m_int;
		m_con = GetOwner()->m_con;

		m_maxMP = m_maxHP = GetOwner()->m_level * GetMercenaryClassData()->nHpMp;
		m_recoverMP = m_recoverHP = GetOwner()->m_level;

		m_eqMelee	= GetOwner()->m_level * GetMercenaryClassData()->nAttDef + GetMercenaryClassData()->nBonusAtt;
		m_eqMagic	= GetOwner()->m_level * GetMercenaryClassData()->nAttDef + GetMercenaryClassData()->nBonusAtt;
		m_eqDefense	= GetOwner()->m_level * GetMercenaryClassData()->nAttDef * 2 + GetMercenaryClassData()->nBonusDef;
		m_eqResist	= GetOwner()->m_level + GetMercenaryClassData()->nBonusDef;

#ifdef SYSTEM_MONSTER_MERCENARY_CARD_EX
		m_walkSpeed	= GetMercenaryClassData()->nWalkSpeed;
		//m_runSpeed	= GetMercenaryClassData()->nRunSpeed;
		SetRunSpeed(GetMercenaryClassData()->nRunSpeed);
#endif
	}

#ifdef DYNAMIC_DUNGEON_BUG_FIX
	if( (this->m_regenInfo != NULL) && (this->m_regenInfo->m_zoneNo == ZONE_DRATAN_CASTLE_DUNGEON) )
	{
		CDratanCastle* pCastle = CDratanCastle::CreateInstance();
		int nMobRate[10][4] =  	// 공격력, 방어력, 최대체력, 시야범위
		{
			{20, 20, 30, 0},
			{10, 10, 15, 0},
			{5, 5, 10, 0},
			{0, 0, 0, 0},
			{-5, -5, -5, 0},
			{-7, -7, -7, 0},
			{-10, -10, -10, 0},
			{-15, -15, -15, 0},
			{-20, -20, -20, 0},
			{-25, -25, -25, -80},
		};
		int midx = pCastle->m_dvd.GetMobRate()/10;
		if(midx > 9)
			midx = 9;
		if(m_attackType == MSG_DAMAGE_MELEE)
		{
			m_eqMelee = m_proto->m_attack + m_proto->m_attack * nMobRate[midx][0] / 100;
			if(m_eqMelee < 0)
				m_eqMelee = 1;
		}
		else if(m_attackType == MSG_DAMAGE_RANGE)
		{
			m_eqRange = m_proto->m_attack + m_proto->m_attack * nMobRate[midx][0] / 100;
			if(m_eqRange < 0)
			{
				m_eqRange = 1;
			}
		}
		else if(m_attackType == MSG_DAMAGE_MAGIC)
		{
			m_eqMagic = m_proto->m_magic + m_proto->m_magic * nMobRate[midx][0] / 100;
			if(m_eqMagic < 0)
			{
				m_eqMagic = 1;
			}
		}

		// 방어력 적용
		m_eqDefense = m_proto->m_defense + m_proto->m_defense * nMobRate[midx][1] / 100;
		// 최대 체력 적용
		m_maxHP = m_proto->m_hp + m_proto->m_hp * nMobRate[midx][2] / 100;
		if(m_maxHP < 0)
		{
			m_maxHP = 1;
		}

		// 시야 거리 적용
		if(nMobRate[midx][3] != 0 && m_attackRange != 0)
		{
			m_attackRange = m_proto->m_attackArea + m_proto->m_attackArea * nMobRate[midx][3] / 100;
		}
	}
#endif // DYNAMIC_DUNGEON_BUG_FIX

	setSyndicateType(m_proto->m_rvr_value);
}

int CNPC::GetAttackLevel()
{
	return m_proto->m_attacklevel;
}

int CNPC::GetDefenseLevel()
{
	return m_proto->m_defenselevel;
}

MSG_DAMAGE_TYPE CNPC::GetAttackType(const CSkillProto* proto) const
{
	if (proto)
	{
		switch (proto->m_type)
		{
		case ST_MELEE:
			return MSG_DAMAGE_MELEE;
		case ST_RANGE:
			return MSG_DAMAGE_RANGE;
		case ST_MAGIC:
			return MSG_DAMAGE_MAGIC;
		}
	}

	return (MSG_DAMAGE_TYPE)m_proto->m_attackType;
}

CSkill* CNPC::FindSkill(int idx)
{
	int i;
	for (i = 0; i < MAX_NPC_SKILL; i++)
	{
		if (m_skills[i])
		{
			if (m_skills[i]->m_proto->m_index == idx)
				return m_skills[i];
		}
	}
	return NULL;
}

bool CNPC::CanMove()
{
	if (!m_proto->CheckFlag(NPC_MOVING))
		return false;
	return CCharacter::CanMove();
}

void CNPC::CalcStatus(bool bSend)
{
	ResetStat();
	m_assist.Apply();
}
LONGLONG CNPC::GetTotalDamage()
{
	LONGLONG ret = 0;
	CAttackChar* pAttackChar = m_attackList;
	while (pAttackChar)
	{
		if (pAttackChar->m_damage > 0)
			ret += pAttackChar->m_damage;

		pAttackChar = pAttackChar->m_next;
	}

	return ret;
}

bool CNPC::CanAttack()
{
#if defined(LC_BILA)
#else
	if (m_assist.m_state & AST_HOLD)
		return false;
#endif // LC_BILA
	return CCharacter::CanAttack();
}

float CNPC::GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type)
{
#ifdef DEV_SYSTEM_MONSTER_MERCENARY
	if( GetOwner() && GetOwner()->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY) == this && GetMercenaryClassData() )
		return 1.0f * GetOwner()->m_level * GetMercenaryClassData()->nHit + (GetOwner()->m_level - df->m_level);
#endif
	return 1.0f * m_proto->m_nHit + (m_level - df->m_level);
}

float CNPC::GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type)
{
	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
#ifdef DEV_SYSTEM_MONSTER_MERCENARY
		if( GetOwner() && GetOwner()->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY) == this && GetMercenaryClassData() )
			return 1.0f * GetOwner()->m_level * GetMercenaryClassData()->nDodge + (GetOwner()->m_level - of->m_level);
#endif
		return 1.0f * m_proto->m_nDodge + (m_level - of->m_level);

	case MSG_DAMAGE_MAGIC:
#ifdef DEV_SYSTEM_MONSTER_MERCENARY
		if( GetOwner() && GetOwner()->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY) == this && GetMercenaryClassData() )
			return GetOwner()->GetAvoid(of, type);
#endif
		return 1.0f * m_proto->m_nMagicAvoid + (m_level - of->m_level);
	default:
		return 0.0f;
	}
}

#ifdef MONSTER_AI
float CNPC::GetDistToRegen()
{
	float xx, zz, hh;
	xx = m_regenX;
	zz = m_regenZ;
	hh = m_pArea->GetHeight(GET_YLAYER(this), m_regenX, m_regenZ);

	return GetDistance(xx, zz, hh, this);
}
#endif

