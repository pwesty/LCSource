#ifndef INCL_OPTIONDATA_H
#define INCL_OPTIONDATA_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <Engine/LocalDefine.h>
#include <Engine/Help/LoadLod.h>
#include <common/header/def_option.h>
#include <common/header/def_title.h>
#include <vector>

#define		OPTION_STR_UP				0		// 힘 상승
#define		OPTION_DEX_UP				1		// 민첩 상승
#define		OPTION_INT_UP				2		// 지혜 상승
#define		OPTION_CON_UP				3		// 체질 상승
#define		OPTION_HP_UP				4		// 생명력 상승
#define		OPTION_MP_UP				5		// 마나 상승

#define		OPTION_DAMAGE_UP			6		// 물리 공격력 상승
#define		OPTION_MELEE_DAMAGE_UP		7		// 물리 근거리 공격력 상승
#define		OPTION_RANGE_DAMAGE_UP		8		// 물리 원거리 공격력 상승
#define		OPTION_MELEE_HIT_UP			9		// 물리 근접 명중률 상승
#define		OPTION_RANGE_HIT_UP			10		// 물리 원거리 명중률 상승

#define		OPTION_DEFENSE_UP			11		// 물리 방어력 상승
#define		OPTION_MELEE_DEFENSE_UP		12		// 물리 근거리 방어력 상승
#define		OPTION_RANGE_DEFENSE_UP		13		// 물리 원거리 방어력 상승
#define		OPTION_MELEE_AVOID_UP		14		// 물리 근접 회피율 상승
#define		OPTION_RANGE_AVOID_UP		15		// 물리 원거리 회피율 상승

#define		OPTION_MAGIC_UP				16		// 마법 공격력 상승
#define		OPTION_MAGIC_HIT_UP			17		// 마법 명중률 상승
#define		OPTION_RESIST_UP			18		// 마법 방어력 상승
#define		OPTION_RESIST_AVOID_UP		19		// 마법 회피율 상승

#define		OPTION_ALL_DAMAGE_UP		20		// 전체 공격력 상승 (근접물리, 원격물리, 마법)
#define		OPTION_ALL_HIT_UP			21		// 전체 명중률 상승 (근접물리, 원격물리, 마법)

#define		OPTION_ALL_DEFENSE_UP		22		// 전체 방어력 상승 (근접물리, 원격물리, 마법)
#define		OPTION_ALL_AVOID_UP			23		// 전체 회피율 상승 (근접물리, 원격물리, 마법)

#define		OPTION_NOT_USED_24			24		// 사용 안함
#define		OPTION_NOT_USED_25			25		// 사용 안함

#define		OPTION_ATTR_FIRE			26
#define		OPTION_ATTR_WATER			27
#define		OPTION_ATTR_WIND			28
#define		OPTION_ATTR_EARTH			29
#define		OPTION_ATTR_DARK			30
#define		OPTION_ATTR_LIGHT			31

// 공성 조합 아이템용 옵션
#define		OPTION_MIX_STR				36		// 힘
#define		OPTION_MIX_DEX				37		// 민첩
#define		OPTION_MIX_INT				38		// 지혜
#define		OPTION_MIX_CON				39		// 체질

#define		OPTION_MIX_ATTACK			40		// 물공
#define		OPTION_MIX_MAGIC			41		// 마공
#define		OPTION_MIX_DEFENSE			42		// 물방
#define		OPTION_MIX_RESIST			43		// 마방

#define		OPTION_MIX_STURN			44		// 스턴
#define		OPTION_MIX_BLOOD			45		// 블러드
#define		OPTION_MIX_MOVE				46		// 무브
#define		OPTION_MIX_POISON			47		// 포이즌
#define		OPTION_MIX_SLOW				48		// 슬로우
// -- 공성 조합 아이템용 옵션

// 2006 하반기 신규 옵션
#define		OPTION_DOWN_LIMITLEVEL		49		// 착용제한 레벨 다운
#define		OPTION_STEAL_MP				51		// 마나 흡수
#define		OPTION_STEAL_HP				52		// 생명력 흡수
#define		OPTION_ATTACK_BLIND			53		// 암흑 공격
#define		OPTION_ATTACK_POISON		54		// 독 공격
#define		OPTION_ATTACK_CRITICAL		55		// 크리티컬 확률 증가
#define		OPTION_RECOVER_HP			56		// HP 회복력 상승
#define		OPTION_RECOVER_MP			57		// MP 회복력 상승
#define		OPTION_DECREASE_SKILL_DELAY	58		// 스킬 쿨타임 감소
#define		OPTION_DECREASE_SKILL_MP	59		// MP 소모량 감소
#define		OPTION_RESIST_STONE			60		// 스톤 내성 증가
#define		OPTION_RESIST_STURN			61		// 스턴 내성 증가
#define		OPTION_RESIST_SILENT		62		// 침묵 내성 증가
#define		OPTION_BLOCKING				63		// 블럭률 증가
#define		OPTION_MOVESPEED			64		// 이동 속도 향상
// --- 2006 하반기 신규 옵션
#define		OPTION_FLYSPEED				65		// 비행 속도 향상
#define     OPTION_ATTACK_DEADLY        66      // 데들리 확률 증가

#define		OPTION_STR_UP_RATE			67
#define		OPTION_DEX_UP_RATE			68
#define		OPTION_INT_UP_RATE			69
#define		OPTION_CON_UP_RATE			70
#define		OPTION_HP_UP_RATE			71
#define		OPTION_MP_UP_RATE			72
#define		OPTION_WEAPON_UP_RATE		73
#define		OPTION_ARMOR_UP_RATE		74
#define		OPTION_MELEE_HIT_UP_RATE	75
#define		OPTION_MAGIC_HIT_UP_RATE	76
#define		OPTION_MELEE_AVOID_RATE		77
#define		OPTION_MAGIC_AVOID_RATE		78
#define		OPTION_RECOVER_HP_RATE		79
#define		OPTION_RECOVER_MP_RATE		80
#define		OPTION_TEST_QUEST			81
#define		OPTION_EXP_UP_RATE			82
#define		OPTION_SP_UP_RATE			83
#define		OPTION_APET_TRANS_END		84		// 변신 종료 시간
#define		OPTION_APET_OWNER_HPUP		85
#define		OPTION_APET_OWNER_ATTUP		86
#define		OPTION_ATT_FIRE_UP			87		// 화염속성 공격 레벨상승
#define		OPTION_ATT_WATER_UP			88		// 빙정속성 공격 레벨상승
#define		OPTION_ATT_WIND_UP			89		// 폭풍속성 공격 레벨상승
#define		OPTION_ATT_EARTH_UP			90		// 대지속성 공격 레벨상승
#define		OPTION_ATT_DARK_UP			91		// 어둠속성 공격 레벨상승
#define		OPTION_ATT_LIGHT_UP			92		// 빛속성 공격 레벨상승
#define		OPTION_DEF_FIRE_UP			93		// 화염속성 방어 레벨상승
#define		OPTION_DEF_WATER_UP			94		// 빙정속성 방어 레벨상승
#define		OPTION_DEF_WIND_UP			95		// 폭풍속성 방어 레벨상승
#define		OPTION_DEF_EARTH_UP			96		// 대지속성 방어 레벨상승
#define		OPTION_DEF_DARK_UP			97		// 어둠속성 방어 레벨상승
#define		OPTION_DEF_LIGHT_UP			98		// 빛속성 방어 레벨상승
#define		OPTION_ALL_STAT_UP			99		// 모든스텟(4종) 상승
#define		OPTION_PVP_DAMAGE_ABSOLB	100		// PVP 데미지 흡수(%)
#define		OPTION_DEBUF_DECR_TIME		101		// 디버프시간 감소(%)
#define		OPTION_RECOVER_HP_NOTRATE	102		// HP회복 정수값
#define		OPTION_RECOVER_MP_NOTRATE	103		// MP회복 정수값
#define		OPTION_INCREASE_STRONG		104		// 강인함증가 (정수)
#define		OPTION_INCREASE_HARD		105		// 하드 적중도 증가 (정수)
#define		OPTION_INCREASE_HP			106		// hp 회복력 상승 (정수)
#define		OPTION_PERSONAL_ALL_STAT_UP	107		// [개인] 전체 스텟 상승
#define		OPTION_PERSONAL_EXP_UP		108		// [개인] 전체 스텟 상승
#define		OPTION_PERSONAL_GOLD_UP		109		// [개인] 전체 스텟 상승
#define		OPTION_PARTY_ALL_STAT_UP	110		// [파티] 전체 스텟 상승
#define		OPTION_PARTY_EXP_UP			111		// [파티] 전체 스텟 상승
#define		OPTION_PARTY_GOLD_UP		112		// [파티] 전체 스텟 상승

#define		MAX_NUM_OPTION				120		// 옵션 총 숫자

#define		MAX_OPTION_INC_ORIGIN		6		// 오리지널 포함 최대 옵션 수

#define		MAX_SOCKET_OPTION			7		// socket system : max count about socket hole [5/10/2010 rumist]
#define		OPTION_VALUE_SHIFT			8		// 2byte 값중 왼쪽 1Byte 옵션 번호 오른쪽 1Byte 옵션 레벨
#define		OPTION_VALUE_AND			0x11111111
#define		OPTION_MAX_LEVEL			7		// 옵션 최대 레벨 현재 7

#define		ORIGIN_VAR_DEFAULT			100

enum eJewel_OptionType
{
	JEWEL_OPTION_PACIVE = 0,
	JEWEL_OPTION_ACTIVE_SKILL_ATT = 1,
	JEWEL_OPTION_ACTIVE_SKILL_DEF = 2,
};

class ENGINE_API COptionData : public stOption, public LodLoader<COptionData>
{
private:
	std::vector<int> optionVal;
	std::string	name;
public:
	inline int GetIndex()			
	{	return getindex(); 	}
	void SetName(const char* str)	{ name = str; }
	inline const char* GetName() const
	{	return name.c_str(); }

	inline int GetValue(int iLevel) const
	{
		if (iLevel >= 0 && iLevel < DEF_OPTION_MAX_LEVEL)
			return value[iLevel];

		return 0;
	}

	//-----------------------------------------------------------------------------
	// Purpose: 옵션 데이터를 화일로부터 읽어들입니다.
	// Input  : &apOptionData - 저장될 배열.
	//			FileName - 파일명
	// Output : 	static int
	//-----------------------------------------------------------------------------
	static bool loadEx(const char* FileName);

	//[sora] 미번역 스트링 index 표시
	void SetNoTranslate()
	{
		char buff[MAX_PATH] = {0,};
		sprintf( buff, "[%d] : option", getindex() );
		name = buff;
	}

	void ClearNoTranslate()
	{
		name = "";
	}
};

class ENGINE_API CNickNameData : public stTitle, public LodLoader<CNickNameData>
{
public:
 	static bool loadEx(const char* FileName);

	inline const char* GetEffectFile() const
	{	return a_effect;	}

	inline const char* GetAttackEffectFile() const
	{	return a_attack;	}

	inline const char* GetDamageEffectFile() const
	{	return a_damage;	}

	inline COLOR GetColor() const
	{	return a_icolor;	}

	inline COLOR GetBGColor() const
	{	return a_ibgcolor;	}

	inline const int* GetOptionIndex() const
	{	return a_option_index;	}

	inline const BYTE* GetOptionLevel() const
	{	return a_option_level;	}

	inline int GetItemIndex() const
	{	return a_item_index;	}
};
#endif



