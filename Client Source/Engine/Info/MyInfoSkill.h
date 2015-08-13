#ifndef		MY_INFO_SKILL_H_
#define		MY_INFO_SKILL_H_

#include <map>

class skill_t
{
public:
	skill_t() : _level(-1), _value(0), _delay(false) {}

	int		_level;
	__int64	_value;
	bool	_delay;
};

class ENGINE_API MyInfoSkill
{
public:
	MyInfoSkill();

	void	clear();

	int		GetSkillLevel(int nSkill_idx, bool bSpecial = false);
	void	SetSkill(int nSkill_idx, int nSkill_lv, bool bSpecial = false);
	
	void	SetSkillExp(int nSkill_idx, __int64 nExp);
	__int64 GetSkillExp(int nSkill_idx);

	void	SetSkillDelay(int nSkill_idx, bool bDelay, bool bSpecial = false);
	bool	GetSkillDelay(int nSkill_idx, bool bSpecial = false);

	DOUBLE	GetReuseItem(int nIndex);
	DOUBLE	GetReuseSkill(int nIndex);

	__int64 GetUseItem(int nIndex);
	void	SetUseItem(int nIndex, __int64 llTime);

	int		GetPetSkillLevel(int nPetIdx, int nSkill_idx);
	void	SetPetSkill(int nPetIdx, int nSkill_idx, int nSkill_lv);
	void	SetPetSkillDelay(int nPetIdx, int nSkill_idx, bool bDelay);
	bool	GetPetSkillDelay(int nPetIdx, int nSkill_idx);

	void	ClearPetSkill();
	void	ClearPetSkill(int nIndex);
	void	ClearWildPetSkill();
	void	ClearSkill();
	void	ClearSSkill();

protected:
	typedef std::map< int, skill_t>		map_skill;
	typedef map_skill::iterator			map_skill_iter;

	typedef std::map< int, map_skill >	map_pet_skill;
	typedef map_pet_skill::iterator		map_pet_skill_iter;

	typedef std::map< int, __int64 >	map_useItem;
	typedef map_useItem::iterator		map_useItem_iter;

	map_skill	m_mapSkill;
	map_skill	m_mapSpecialSkill;

	map_useItem	m_mapUseItem;

	map_pet_skill	m_mapSkillPet;
};


#endif		// MY_INFO_SKILL_H_