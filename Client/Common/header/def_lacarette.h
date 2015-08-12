
#ifndef		LACARETTE_DATA_H_
#define		LACARETTE_DATA_H_

#include "def_base.h"

#define DEF_GIVE_ITEM_MAX 10
#define DEF_COIN_IDX_MAX 32
#define DEF_COIN_COUNT_MAX 8

#pragma pack(push, 1)
struct stGiveItem
{
	int itemIndex;
	LONGLONG ItemCount;
	int	itemFlag;
	int itemProb;
	int retteNum;
};

struct stCoinInfo
{
	int useCoinCnt;

	struct coin
	{
		int		coinIdx;
		int		coindefcnt;
		bool	bcoinaddcnt;
	} list[0];
};

struct stLacarette : public stTbl_base
{
	int		enable;
	int		coinIdx;
	int		coindefcnt;
	bool	bcoinaddcnt;
	stGiveItem	giveItemList[DEF_GIVE_ITEM_MAX];
};

struct stLacaretteTool : public stLacarette
{
	int		useCoinCnt;
	char	strCoinIndex[DEF_COIN_IDX_MAX];
	char	strCoinDefCount[DEF_COIN_COUNT_MAX];
	char	strCoinAddCount[DEF_COIN_COUNT_MAX];
};

struct stLacaretteClient : public stLacarette
{
	char		strName[50];
	stCoinInfo	coinInfo;
	void CopyFrom(stLacarette* src) {
		memcpy(this, src, sizeof(stLacarette));
	}
};
#pragma pack(pop)
#endif		// LACARETTE_DATA_H_