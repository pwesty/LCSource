
#ifndef		ITEM_DATA_H_
#define		ITEM_DATA_H_

#include "def_base.h"

#define	MAX_MAKE_ITEM_MATERIAL		10
#define DEF_MAX_ORIGIN_OPTION		10
#define MAX_WEARING					12
#define MAX_SET_ITEM_OPTION			11			// 세트 아이템에 붙는 최대 옵션 개수

#define DEF_SMC_DEFAULT_LENGTH		64
#define DEF_EFFECT_DEFAULT_LENGTH	32

/*

union
{
int	num0;
int pAttack;		// 물리 공격력
int	pDefense;		// 물리 방어력
int sec;			// 초(s)(마다) 즉효(-1)
int skillType;		// 보조.
int	warpType;		// 워프 타입
int questnum;		// 퀘스트 번호.
int processtype;	// 가공 타입
int optiontype;		// 옵션 타입
int	producttype;	// 제조 타입
int boxtype;		// Box Type // Date : 2005-01-12,   By Lee Ki-hwan
int	refinetype;		// Refine Type // Date : 2005-01-12,   By Lee Ki-hwan
int pettype;		// 애완동물 종류.
int jumpinglevel;	// 점핑 포션 레벨
};

union
{
int num1;
int	mAttack;		// 마법 공격력
int	mDffense;		// 마법 방어력
int recoverHP;		// HP 회복...
int skillLevel;		// 보조
int zonenum;		// 월드 번호.
int processsubtype;	// 가공 서브 타입.
int	productsubtype;	// 제조 타입
};

union
{
int num2;
int attackSpeed;	// 공격 속도.
int recoverMP;		// MP 회복
int extranum;		// Extra 번호.
int sskill;			// 특수 스킬
int grade;			// 등급
};

union
{
int num3;
int count;			// 포션 제작 갯수 
};

*/
#pragma pack(push, 1)

struct stItem : public stTbl_base
{
	int		job;
	int		stack;
	int		fame;
	int		level;
	__int64	flag;
	int		wearing;
	int		type;
	int		subType;
	int		needItemIndex[MAX_MAKE_ITEM_MATERIAL];
	int		needItemCount[MAX_MAKE_ITEM_MATERIAL];
	int		needSSkillIndex;
	int		needSSkillCount;
	int		needSSkillIndex2;
	int		needSSkillCount2;
	int		textureID;
	int		textureRow;
	int		textureCol;
	int		num0;
	int		num1;
	int		num2;
	int		num3;
	int		price;

	int     set0;
	int     set1;
	int     set2;
	int     set3;
	int     set4;

	char	fileSMC[DEF_SMC_DEFAULT_LENGTH];
	char	efffectName[DEF_EFFECT_DEFAULT_LENGTH];
	char	attackEffectName[DEF_EFFECT_DEFAULT_LENGTH];
	char	damageEffectName[DEF_EFFECT_DEFAULT_LENGTH];

	int		JewelOptionType;
	int		JewelOptionLevel;

	int		rareOptionType[DEF_MAX_ORIGIN_OPTION];
	int		rareOptionLevel[DEF_MAX_ORIGIN_OPTION];
	int		syndicate_type;
	int		syndicate_grade;

	int		fortuneIndex;
	char	castleWar;
};

struct stFortune
{
	int		skill_index;
	int		skill_level;
	int		string_index;
	int		prob;
};

struct stSetItem : public stTbl_base
{
	int		job;
	int		optionCnt;
	int		item[MAX_WEARING];
	int		wearCnt[MAX_SET_ITEM_OPTION];
	int		optionType[MAX_SET_ITEM_OPTION];
	int		option[MAX_SET_ITEM_OPTION];
	int		optionLevel[MAX_SET_ITEM_OPTION];
};

#pragma pack(pop)

#endif		// ITEM_DATA_H_