#ifndef INCL_SPECIALSKILL_H
#define INCL_SPECIALSKILL_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <Engine/Help/LoadLod.h>
#include <common/header/def_skill.h>

// Special Skill 타입
#define		SSKILL_MINING		0	// 채굴
#define		SSKILL_GATHERING	1	// 채집
#define		SSKILL_CHARGE		2	// 차지
#define		SSKILL_STONE		3	// 광석정련술
#define		SSKILL_PLANT		4	// 식물가공술
#define		SSKILL_ELEMENT		5	// 원소정제술

#define		SSKILL_MAKE_WEAPON	6	// 무기 제작 기술
#define		SSKILL_MAKE_WEAR	7	// 방어구 제작 기술
#define		SSKILL_MAKE_G_B		8	// 장갑 부츠 제작 기술
#define		SSKILL_MAKE_ARMOR	9	// 갑바 바지 제작 기술
#define		SSKILL_MAKE_H_S		10	// 투구 방패 제작 기술
#define		SSKILL_MAKE_POTINO	11  // 포션 제작 // Date : 2005-01-07,   By Lee Ki-hwan
#define		SSKILL_PROCESS_NPC	12	// 가공 NPC

#define		SSKILL_STONE_0		3	// 스톤 온 정련술
#define		SSKILL_STONE_1		4	// 스톤 리스 정련술
#define		SSKILL_STONE_2		5	// 스톤 웨버 정련술
#define		SSKILL_STONE_3		6	// 스톤 비스트 정련술
#define		SSKILL_STONE_4		7	// 스톤 웰스던 정련술

#define		SSKILL_PLANT_0		8	// 크락의 노란잎 가공술
#define		SSKILL_PLANT_1		9	// 크락의 줄기 가공술
#define		SSKILL_PLANT_2		10	// 크락의 파란잎 가공술
#define		SSKILL_PLANT_3		11	// 크락의 가시 가공술
#define		SSKILL_PLANT_4		12	// 크락의 꽃 가공술

#define		SSKILL_ELEMENT_0	13	// 원소 E등급 정제술
#define		SSKILL_ELEMENT_1	14	// 원소 D등급 정제술
#define		SSKILL_ELEMENT_2	15	// 원소 C등급 정제술
#define		SSKILL_ELEMENT_3	16	// 원소 B등급 정제술
#define		SSKILL_ELEMENT_4	17	// 원소 A등급 정제술

class CSpecialSkill : public stSpecailSkill, public LodLoader<CSpecialSkill>
{
public:
	static bool loadEx(const char* str_path);
	std::string name;
	std::string	desc;
	int		transFlag;	//[sora] 미번역 스트링 index 표시	

	inline int GetIndex()			const
	{	return index;	}

	inline const char* GetName()	const
	{	return name.c_str();	}

	inline int GetType()			const
	{	return type;	}

	inline int GetLearnLevel( int iIndex) const
	{	return needLevel[iIndex];	}

	inline int GetLearnSP( int iIndex) const
	{	return needSP[iIndex];	}

	inline int GetLearnSkillIndex()		const
	{	return needSSkill;		}

	inline int GetLearnSkillLevel()	const
	{	return needSSkillLevel;	}

	inline int GetMaxLevel()	const
	{	return maxLevel;	}

	inline int GetPreference()	const
	{	return preference;	}

	inline int	GetIconTexID()	const { return textureID; }
	inline int	GetIconTexRow() const { return textureRow; }
	inline int	GetIconTexCol() const { return textureCol; }	

	inline const char* GetDescription()	const
	{	return desc.c_str();	}
	
//	static int LoadSSkillDataFromFile(CStaticArray<CSpecialSkill> &apSkillData, const char* FileName);

	//[sora] 미번역 스트링 index 표시
	void SetNoTranslate();
	void ClearNoTranslate();

	void SetName(const char* str)		{name = str; }
	void SetDesc(const char* str)		{desc = str; }
	int& GetTransFlag()					{ return transFlag; }

//	static CSpecialSkill* m_pCont;
};

#endif