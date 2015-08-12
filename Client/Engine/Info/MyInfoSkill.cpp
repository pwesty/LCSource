#include "StdH.h"
#include "MyInfoSkill.h"
#include <Engine/Entities/ItemData.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Interface/UIManager.h>

MyInfoSkill::MyInfoSkill()
{

}

void MyInfoSkill::clear()
{
	ClearSkill();
	ClearSSkill();
	m_mapUseItem.clear();
}

int MyInfoSkill::GetSkillLevel( int nSkill_idx, bool bSpecial )
{
	int retLevel = 0;

	if (nSkill_idx == 436)
		return 1;

	if (bSpecial == false)
	{
		map_skill_iter iter = m_mapSkill.find(nSkill_idx);

		if (iter != m_mapSkill.end())
			retLevel = iter->second._level;
	}
	else
	{
		map_skill_iter iter = m_mapSpecialSkill.find(nSkill_idx);

		if (iter != m_mapSpecialSkill.end())
			retLevel = iter->second._level;
	}

	return retLevel;
}

void MyInfoSkill::SetSkill( int nSkill_idx, int nSkill_lv, bool bSpecial )
{
	if (bSpecial == false)
	{
		map_skill_iter iter = m_mapSkill.find(nSkill_idx);

		if (iter != m_mapSkill.end())
		{
			iter->second._level = nSkill_lv;
		}
		else
		{
			skill_t stSkill;
			stSkill._level = nSkill_lv;
			m_mapSkill.insert( std::make_pair(nSkill_idx, stSkill) );
		}
	}
	else
	{
		map_skill_iter iter = m_mapSpecialSkill.find(nSkill_idx);

		if (iter != m_mapSpecialSkill.end())
		{
			iter->second._level = nSkill_lv;
		}
		else
		{
			skill_t stSkill;
			stSkill._level = nSkill_lv;
			m_mapSpecialSkill.insert( std::make_pair(nSkill_idx, stSkill) );
		}
	}
}

void MyInfoSkill::SetSkillExp( int nSkill_idx, __int64 nExp )
{
	map_skill_iter iter = m_mapSkill.find(nSkill_idx);

	if (iter != m_mapSkill.end())
	{
		iter->second._value = nExp;
	}
	else
	{
		skill_t stSkill;
		stSkill._value = nExp;
		m_mapSkill.insert( std::make_pair(nSkill_idx, stSkill) );
	}
}

__int64 MyInfoSkill::GetSkillExp( int nSkill_idx )
{
	__int64 nRet = 0;

	map_skill_iter iter = m_mapSkill.find(nSkill_idx);

	if (iter != m_mapSkill.end())
		nRet = iter->second._value;

	return nRet;
}

void MyInfoSkill::SetSkillDelay( int nSkill_idx, bool bDelay, bool bSpecial )
{
	if (bSpecial == false)
	{
		map_skill_iter iter = m_mapSkill.find(nSkill_idx);

		if (iter != m_mapSkill.end())
		{
			iter->second._delay = bDelay;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else
	{
		map_skill_iter iter = m_mapSpecialSkill.find(nSkill_idx);

		if (iter != m_mapSpecialSkill.end())
		{
			iter->second._delay = bDelay;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
}

bool MyInfoSkill::GetSkillDelay( int nSkill_idx, bool bSpecial )
{
	bool bRet = false;

	if (bSpecial == false)
	{
		map_skill_iter iter = m_mapSkill.find(nSkill_idx);

		if (iter != m_mapSkill.end())
			bRet = iter->second._delay;
	}
	else
	{
		map_skill_iter iter = m_mapSpecialSkill.find(nSkill_idx);

		if (iter != m_mapSpecialSkill.end())
			bRet = iter->second._delay;
	}

	return bRet;
}

DOUBLE MyInfoSkill::GetReuseItem( int nIndex )
{
	CItemData* pData = CItemData::getData(nIndex);

	if (pData != NULL)
	{
		int skill_index = pData->GetNum0();

		if (skill_index > 0)
			return DOUBLE(_pNetwork->GetSkillData(skill_index).GetReUseTime()) / 10.0;
	}
	
	return 0.0;	
}

DOUBLE MyInfoSkill::GetReuseSkill( int nIndex )
{
	CSkill&	rSkill = _pNetwork->GetSkillData(nIndex);

	int nCoolTimeReductionRate = 0;
	DOUBLE dCoolTime = 0.0;
	DOUBLE dReuseTime = 0.0;
	int nReuseTime = 0;

	if (rSkill.GetJob() != PET_JOB && rSkill.GetJob() != WILDPET_JOB && 
		!(rSkill.GetFlag() & SF_GUILD))
	{ 
		// 스킬 쿨타임 옵션은 캐릭터에게만 붙는다.
		if (!(rSkill.Skill_Data.appState & SCT_NOCOOLTIME))
		{
			nCoolTimeReductionRate = UIMGR()->GetCoolTimeReductionRate();
			dCoolTime = ( 150.0 / (150.0 + nCoolTimeReductionRate ) * 100.0 / (100.0 + nCoolTimeReductionRate) );
		}
	}

	if(rSkill.GetReUseTime())
	{
		dReuseTime = DOUBLE( rSkill.GetReUseTime() + _pNetwork->MyCharacterInfo.magicspeed ) / 10.0 *  (nCoolTimeReductionRate ? dCoolTime : 1);

		return dReuseTime;
	}

	return 0.0;
}

__int64 MyInfoSkill::GetUseItem( int nIndex )
{
	map_useItem_iter iter = m_mapUseItem.find(nIndex);

	if (iter != m_mapUseItem.end())
		return iter->second;

	return 0;
}

void MyInfoSkill::SetUseItem(int nIndex, __int64 llTime)
{
	map_useItem_iter iter = m_mapUseItem.find(nIndex);

	if (iter != m_mapUseItem.end())
		iter->second = llTime;
	else
		m_mapUseItem.insert(std::make_pair(nIndex, llTime));
}

int MyInfoSkill::GetPetSkillLevel( int nPetIdx, int nSkill_idx )
{
	int		nRet = 0;

	map_pet_skill_iter iter = m_mapSkillPet.find(nPetIdx);

	if (iter != m_mapSkillPet.end())
	{
		map_skill_iter skill_iter = iter->second.find(nSkill_idx);

		if (skill_iter != iter->second.end())
			nRet = skill_iter->second._level;
	}

	return nRet;
}

void MyInfoSkill::SetPetSkill( int nPetIdx, int nSkill_idx, int nSkill_lv )
{
	map_pet_skill_iter iter = m_mapSkillPet.find(nPetIdx);

	if (iter != m_mapSkillPet.end())
	{
		map_skill_iter skill_iter = iter->second.find(nSkill_idx);

		if (skill_iter != iter->second.end())
		{
			skill_iter->second._level = nSkill_lv;
		}
		else
		{
			skill_t stSkill;
			stSkill._level = nSkill_lv;
			iter->second.insert( std::make_pair(nSkill_idx, stSkill) );
		}

	}
	else
	{
		skill_t stSkill;
		stSkill._level = nSkill_lv;
		map_skill map_skill;

		map_skill.insert( std::make_pair(nSkill_idx, stSkill) );

		m_mapSkillPet.insert( std::make_pair(nPetIdx, map_skill) );
	}
}

void MyInfoSkill::SetPetSkillDelay( int nPetIdx, int nSkill_idx, bool bDelay )
{
	map_pet_skill_iter iter = m_mapSkillPet.find(nPetIdx);

	if (iter != m_mapSkillPet.end())
	{
		map_skill_iter skill_iter = iter->second.find(nSkill_idx);

		if (skill_iter != iter->second.end())
			skill_iter->second._delay = bDelay;
	}
}

bool MyInfoSkill::GetPetSkillDelay( int nPetIdx, int nSkill_idx )
{
	bool bRet = false;

	map_pet_skill_iter iter = m_mapSkillPet.find(nPetIdx);

	if (iter != m_mapSkillPet.end())
	{
		map_skill_iter skill_iter = iter->second.find(nSkill_idx);

		if (skill_iter != iter->second.end())
			bRet = skill_iter->second._delay;
	}

	return bRet;
}

void MyInfoSkill::ClearPetSkill()
{
	m_mapSkillPet.clear();
}

void MyInfoSkill::ClearPetSkill( int nIndex )
{
	map_pet_skill_iter iter = m_mapSkillPet.find(nIndex);

	if (iter != m_mapSkillPet.end())
	{
		iter->second.clear();
	}
}

void MyInfoSkill::ClearWildPetSkill()
{
	map_pet_skill_iter iter = m_mapSkillPet.find(0);

	if (iter != m_mapSkillPet.end())
	{
		iter->second.clear();
	}
}

void MyInfoSkill::ClearSkill()
{
	m_mapSkill.clear();
}

void MyInfoSkill::ClearSSkill()
{
	m_mapSpecialSkill.clear();
}
