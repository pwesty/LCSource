#include "stdhdrs.h"

#include "Server.h"
#include "Character.h"

extern int base_stat[JOBCOUNT][4];

int CElemental::m_nVirtualIndex = 1;

CElemental::CElemental(CPC* owner, char elementalType)
{
	m_prevElemental = NULL;
	m_nextElemental = NULL;

	m_bChangeStatus = false;

	m_owner = owner;

	m_elementalType = elementalType;

	// 기본 스탯
	m_defStr = m_owner->m_str;
	m_defDex = m_owner->m_dex;
	m_defInt = m_owner->m_int;
	m_defCon = m_owner->m_con;

	// HP
	m_defHP = 0;
	switch (m_elementalType)
	{
	case ELEMENTAL_FIRE:
		m_defHP = 50;
		break;
	case ELEMENTAL_WIND:
		m_defHP = 30;
		break;
	case ELEMENTAL_EARTH:
		m_defHP = 50;
		break;
	case ELEMENTAL_WATER:
		m_defHP = 30;
		break;
	}
	m_defHP += 5 * (m_defStr - base_stat[JOB_SORCERER][0]) + 5 * (m_defCon - base_stat[JOB_SORCERER][3]);

	m_defAttackLevel = owner->GetAttackLevel();
	m_defDefenseLevel = owner->GetDefenseLevel();

	m_pulseRemain = SUMMON_DURATION;

	// CCharacter의 멤버 초기화
	int skillflag = 0;			// 엘리멘탈에 따른 SSF_USE_ 플래그
	m_type = MSG_CHAR_ELEMENTAL;

	m_index = GetVirtualIndex();
	m_name.Format("Elemental_%d", m_elementalType);
	m_level = owner->m_level;

	m_hp = m_maxHP = m_defHP;

	m_str = m_defStr;
	m_dex = m_defDex;
	m_int = m_defInt;
	m_con = m_defCon;

	m_eqResist = owner->m_level;
	m_attackSpeed = ELEMENTAL_ATTACK_SPEED;
	//m_walkSpeed = m_runSpeed = ELEMENTAL_RUN_SPEED;
	m_walkSpeed = ELEMENTAL_RUN_SPEED;
	SetRunSpeed(ELEMENTAL_RUN_SPEED);
	m_recoverHP = ELEMENTAL_RECOVER_HP;

	switch (m_elementalType)
	{
	case ELEMENTAL_FIRE:
		skillflag = SSF_USE_FIRE;
		m_eqMagic = owner->m_eqMagic * 8 / 10;
		m_opMagic = owner->m_opMagic * 8 / 10;
		m_eqDefense = owner->m_eqDefense * 15 / 10;
		m_opDMelee = owner->m_opDMelee * 15 / 10;
		m_opDRange = owner->m_opDRange * 15 / 10;
		m_attackRange = ELEMENTAL_ATTACK_RANGE_FIRE;
		break;
	case ELEMENTAL_WIND:
		skillflag = SSF_USE_WIND;
		m_eqMagic = owner->m_eqMagic * 11 / 10;
		m_opMagic = owner->m_opMagic * 11 / 10;
		m_eqDefense = owner->m_eqDefense * 14 / 10;
		m_opDMelee = owner->m_opDMelee * 14 / 10;
		m_opDRange = owner->m_opDRange * 14 / 10;
		m_attackRange = ELEMENTAL_ATTACK_RANGE_WIND;
		break;
	case ELEMENTAL_EARTH:
		skillflag = SSF_USE_EARTH;
		m_eqMagic = owner->m_eqMagic * 12 / 10;
		m_opMagic = owner->m_opMagic * 12 / 10;
		m_eqDefense = owner->m_eqDefense * 12 / 10;
		m_opDMelee = owner->m_opDMelee * 12 / 10;
		m_opDRange = owner->m_opDRange * 12 / 10;
		m_attackRange = ELEMENTAL_ATTACK_RANGE_EARTH;
		break;
	case ELEMENTAL_WATER:
		skillflag = SSF_USE_WATER;
		m_eqMagic = owner->m_eqMagic * 88 / 100;
		m_opMagic = owner->m_opMagic * 88 / 100;
		m_eqDefense = owner->m_eqDefense * 14 / 10;
		m_opDMelee = owner->m_opDMelee * 14 / 10;
		m_opDRange = owner->m_opDRange * 14 / 10;
		m_attackRange = ELEMENTAL_ATTACK_RANGE_WATER;
		break;
	}

	// 소서러의 스킬중 정령이 사용할 스킬을 가져온다
	// 액티브 스킬
	void* pos = owner->m_activeSkillList.GetHeadPosition();
	while (pos)
	{
		CSkill* skill = owner->m_activeSkillList.GetNext(pos);
		if (skill && skill->m_proto)
		{
			if (skill->m_proto->CheckSorcererFlag(skillflag))
			{
				CSkill* elementalSkill = gserver->m_skillProtoList.Create(skill->m_proto->m_index, skill->m_level);
				m_skillList.Add(elementalSkill);
			}
		}
	}
	// 패시브 스킬
	pos = owner->m_passiveSkillList.GetHeadPosition();
	while (pos)
	{
		CSkill* skill = owner->m_passiveSkillList.GetNext(pos);
		if (skill && skill->m_proto)
		{
			if (skill->m_proto->CheckSorcererFlag(skillflag))
				AddSkill(skill->m_proto->m_index, skill->m_level);
		}
	}
}

MSG_DAMAGE_TYPE CElemental::GetAttackType(const CSkillProto* proto) const
{
	if (!proto)
		return MSG_DAMAGE_MAGIC;
	else
	{
		switch (proto->m_type)
		{
		case ST_MELEE:
			return MSG_DAMAGE_MELEE;
		case ST_RANGE:
			return MSG_DAMAGE_RANGE;
		case ST_MAGIC:
		default:
			return MSG_DAMAGE_MAGIC;
		}
	}
}

void CElemental::ApplyPassive()
{
	CPC* pc = GetOwner();
	if (!pc)
		return ;

	// 초기화
	memset(&m_avPassiveAddition, 0, sizeof(m_avPassiveAddition));
	memset(&m_avPassiveRate, 0, sizeof(m_avPassiveRate));

	// 주인의 패시브 리스트에서 적용
	CSkill* pSkill;
	void* pos = pc->m_passiveSkillList.GetHeadPosition();
	while (pos)
	{
		pSkill = pc->m_passiveSkillList.GetNext(pos);

		if (pSkill && pSkill->m_proto)
		{
			bool bApply = true;
			// 소환수는 무조건 플래그가 있어야 함
			switch (GetElementalType())
			{
			case ELEMENTAL_FIRE:
				if (!pSkill->m_proto->CheckSorcererFlag(SSF_APP_FIRE))
					bApply = false;
				break;
			case ELEMENTAL_WIND:
				if (!pSkill->m_proto->CheckSorcererFlag(SSF_APP_WIND))
					bApply = false;
				break;
			case ELEMENTAL_EARTH:
				if (!pSkill->m_proto->CheckSorcererFlag(SSF_APP_EARTH))
					bApply = false;
				break;
			case ELEMENTAL_WATER:
				if (!pSkill->m_proto->CheckSorcererFlag(SSF_APP_WATER))
					bApply = false;
				break;
			default:
				bApply = false;
				break;
			}

			if (bApply)
				ApplyPassiveSkill(pSkill, 100);
		}
	}

	ApplyAssistData(&m_avPassiveAddition, &m_avPassiveRate);
}

CSkill* CElemental::FindSkill(int skillindex)
{
	return m_skillList.Find(skillindex);
}

void CElemental::AddSkill(int skillindex, int level)
{
	if (level < 1)
		return ;

	CSkill* skill = FindSkill(skillindex);
	if (skill)
		skill->m_level = level;
	else
	{
		CSkill* skill = gserver->m_skillProtoList.Create(skillindex, level);
		m_skillList.Add(skill);
	}
}

void CElemental::ResetStat()
{
	// CCharacter의 멤버 초기화
	m_maxHP = m_defHP;

	m_str = m_defStr;
	m_dex = m_defDex;
	m_int = m_defInt;
	m_con = m_defCon;

	m_attackSpeed	= ELEMENTAL_ATTACK_SPEED;
	//m_walkSpeed		= m_runSpeed = ELEMENTAL_RUN_SPEED;
	m_walkSpeed = ELEMENTAL_RUN_SPEED;
	SetRunSpeed(ELEMENTAL_RUN_SPEED);
	m_recoverHP		= ELEMENTAL_RECOVER_HP;
	m_statPall		= 0;
	m_statPall_per	= 0;

	switch (m_elementalType)
	{
	case ELEMENTAL_FIRE:
		m_attackRange = ELEMENTAL_ATTACK_RANGE_FIRE;
		break;
	case ELEMENTAL_WIND:
		m_attackRange = ELEMENTAL_ATTACK_RANGE_WIND;
		break;
	case ELEMENTAL_EARTH:
		m_attackRange = ELEMENTAL_ATTACK_RANGE_EARTH;
		break;
	case ELEMENTAL_WATER:
		m_attackRange = ELEMENTAL_ATTACK_RANGE_WATER;
		break;
	}
}

void CElemental::CalcStatus(bool bSend)
{
	// 원상태로
	ResetStat();

	// 패시브 스킬 적용
	ApplyPassive();

	m_assist.Apply();

	m_bChangeStatus = true;
}

float CElemental::GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type)
{
	if (m_owner)
		return m_owner->GetHitrate(df, type);
	else
		return 0.0f;
}

float CElemental::GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type)
{
	if (m_owner)
		return m_owner->GetAvoid(of, type);
	else
		return 0.0f;
}
