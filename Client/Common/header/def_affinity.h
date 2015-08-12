
#ifndef		AFFINITY_DATA_H_
#define		AFFINITY_DATA_H_

#include "def_base.h"
#pragma pack(push, 1)
struct stAffinity : public stTbl_base
{
	int		contribute_item_count;
	int*	itemidx;
	int*	itemidx_contribute_point;
	int		texture_id;
	int		texture_row;
	int		texture_col;
	int		needitemidx;
	int		needitemcount;
	int		needlevel;
	int     needaffinidx;
	int		needaffinpoint;
	int		affinitynpc_count;
	int*	affinitynpc_index;
	int*	affinitynpc_flag;
	int*	affinitynpc_stringidx;

	//퀘스트, 몬스터, 보상 아이템
	int		contribute_quest_count;
	int*	questindex;
	int*	quest_contribute_point;
	int		contribute_monster_count;
	int*	monster_npcindex;
	int*	monster_npc_contribute_point;		
	int*	rewarditemidx;
	int*	reward_point;
	int		rewarditem_count;
	int*	monster_texture_id;
	int*	monster_texture_row;
	int*	monster_texture_col;
};
#pragma pack(pop)
#endif		// AFFINITY_DATA_H_