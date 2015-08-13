
#ifndef		QUEST_DATA_H_
#define		QUEST_DATA_H_

#include "def_base.h"

#define		QUEST_MAX_NEED_ITEM			5		// 퀘스트 시 필요아이템 최대 5
#define		QUEST_MAX_CONDITION			3		// 퀘스트 수행조건 최대 3
#define		QUEST_MAX_CONDITION_DATA	4		// 퀘스트 수행조건 하나당 부가 데이타 최대 4
#define		QUEST_MAX_PRIZE				5		// 퀘스트 보상 최대 5
#define		QUEST_MAX_OPTPRIZE			7

#pragma pack(push, 1)

struct stQuest : public stTbl_base
{
	int		type1;
	int		type2;
	int		startType;
	int		startData;
	int		prizeNPC;

	int		preQuestNo;
	int		startNpcZoneNo;
	int		prizeNpcZoneNo;

	int		needExp;

	int		needMinLevel;
	int		needMaxLevel;
	int		needJob;
	int		needMinPenalty;
	int		needMaxPenalty;
	int		needItemIndex[QUEST_MAX_NEED_ITEM];
	int		needItemCount[QUEST_MAX_NEED_ITEM];
	int		rvr_type;
	int		rvr_grade;

	int		conditionType[QUEST_MAX_CONDITION];
	int		conditionIndex[QUEST_MAX_CONDITION];
	int		conditionNum[QUEST_MAX_CONDITION];
	int		conditionData[QUEST_MAX_CONDITION][QUEST_MAX_CONDITION_DATA];

	int		prizeType[QUEST_MAX_PRIZE];
	int		prizeIndex[QUEST_MAX_PRIZE];
	__int64 prizeData[QUEST_MAX_PRIZE];
	int		optionPrize;
	int		optPrizeType[QUEST_MAX_OPTPRIZE];
	int		optPrizeIndex[QUEST_MAX_OPTPRIZE];
	int		optPrizeData[QUEST_MAX_OPTPRIZE];
	int		optPrizePlus[QUEST_MAX_OPTPRIZE];

	int		partyscale;
	int		onlyOptPrize;
};

#pragma pack(pop)

#endif		// QUEST_DATA_H_