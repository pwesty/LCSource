
#ifndef		ITEM_COLLECTION_H_
#define		ITEM_COLLECTION_H_

#include "def_base.h"

#define DEF_NEED_ITEM_COUNT 6

enum eNEED_TYPE
{
	eNEED_TYPE_NONE = 0,
	eNEED_TYPE_ITEM,
	eNEED_TYPE_THEME
};

struct stItemEtc
{
	int	  nIndex;
	SQUAD llEtc;
};

struct stNeedItem
{
	int need_type;
	int need_index;
	int need_num;
};

struct stItemCollection : public stTbl_base
{
	//char enable;
	char id;
	char row;
	char col;
	stNeedItem need_item[DEF_NEED_ITEM_COUNT];
	int result_type;
	int result_index;
	int result_num;

	// Category 는 상위 8비트
	int getCategory() {
		return (char)(index >> 24);
	}

	// Theme 는 하위 24비트
	int getTheme() {
		return (index & 0x00ffffff);
	}
};

//typedef std::map

#endif		// ACTION_DATA_H_