#ifndef __PTYPE_INVENTORY_H__
#define __PTYPE_INVENTORY_H__

#include "ptype_base.h"
#include "ptype_char_status.h"

// 각 inventory의 크기
enum
{
	ITEM_COUNT_IN_INVENTORY_NORMAL = 100,
	ITEM_COUNT_IN_INVENTORY_CASH_1 = 25,
	ITEM_COUNT_IN_INVENTORY_CASH_2 = 25,

	ITEM_COUNT_IN_INVENTORY_STASH = 300,
	ITEM_COUNT_IN_INVENTORY_STASH_NORMAL = 150,

	MAX_STASH_KEEP = 10,
	MAX_STASH_TAKE = 10,
};

// 새로운 가방을 만들경우 INVENTORY_TAB_STASH 보다 값을 작게 만들면 됨
enum
{
	INVENTORY_TAB_NORMAL,
	INVENTORY_TAB_CASH_1,
	INVENTORY_TAB_CASH_2,
	INVENTORY_TAB_STASH,	// 계정 창고

	INVENTORY_TAB_MAX,
};

enum
{
	NAS_DESC_REASON_GUILD_KEEP,				// 길드창고에 맡길때
	NAS_DESC_REASON_STASH_KEEP,				// 계정창고에 맡길때
};

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
typedef unsigned char inven_tab_t;
typedef unsigned short inven_index_t;

namespace UpdateClient
{
struct itemInfo
{
	short		invenIndex;			// 인벤토리내에서의 위치 0 ~

	int			virtualIndex;		// 서버가 발행한 가상 index
	int			dbIndex;			// DB상의 Index
	char		wearPos;
	int			plus;
	int			plus_2;
	int			flag;
	int			nCompositeItem;
	int			used;
	int			used_2;
	LONGLONG	itemCount;
	bool		toggle;

	int			option_count;
	int			option_type[MAX_ITEM_OPTION];
	int			option_level[MAX_ITEM_OPTION];

	int			socket[MAX_SOCKET_COUNT];

	int			origin_var[MAX_ORIGIN_OPTION];
#ifdef DURABILITY
	unsigned short	now_durability;
	unsigned short	max_durability;
#endif
};

//////////////////////////////////////////////////////////////////////////

struct money : public pTypeBase
{
	LONGLONG	nas;
	LONGLONG	bonus;
};

struct moneyDescReason : public pTypeBase	// NAS 감소 이유를 클라이언트에게 전달
{
	short		reason;
	LONGLONG	nas;
};

struct invenList : public pTypeBase
{
	short		tab_no;
	short		count;
	itemInfo	info_list[0];
};

struct addItemInfo : public pTypeBase
{
	short		tab_no;
	itemInfo	info;
};

struct deleteItemInfo : public pTypeBase
{
	short		tab_no;
	int			invenIndex;
};

struct changeItemCount : public pTypeBase
{
	short		tab_no;
	int			invenIndex;
	LONGLONG	count;
};

struct changeItemCountWithSwap : public pTypeBase
{
	short		tab_no;
	int			invenIndex;
	LONGLONG	count;
};

struct changeItemPlus : public pTypeBase
{
	short		tab_no;
	int			invenIndex;
	int			plus;
};

struct changeItemPlus_2 : public pTypeBase
{
	short		tab_no;
	int			invenIndex;
	int			plus_2;
};

struct changeItemFlag : public pTypeBase
{
	short		tab_no;
	int			invenIndex;
	int			flag;
};

struct changeItemUsed : public pTypeBase
{
	short		tab_no;
	int			invenIndex;
	int			used;
};

struct changeItemUsed_2 : public pTypeBase
{
	short		tab_no;
	int			invenIndex;
	int			used_2;
};

struct changeItemWearPos : public pTypeBase
{
	short		tab_no;
	int			invenIndex;
	int			wearPos;
};

struct changeItemOption : public pTypeBase
{
	short		tab_no;
	int			invenIndex;
	int			option_count;
	int			option_type[MAX_ITEM_OPTION];
	int			option_level[MAX_ITEM_OPTION];
};

struct changeItemSocket : public pTypeBase
{
	short		tab_no;
	int			invenIndex;
	int			socket[MAX_SOCKET_COUNT];
};

struct changeItemOriginVar : public pTypeBase
{
	short		tab_no;
	int			invenIndex;
	int			origin_var[MAX_ORIGIN_OPTION];
};

struct changeItemComposite : public pTypeBase
{
	short		tab_no;
	int			invenIndex;
	int			nCompositeItem;
};

struct eraseAllItem : public pTypeBase
{
	short		tab_no;
};

struct changeWearItemUsed : public pTypeBase
{
	int			wearPos;
	int			vIndex;
	int			used;
};
struct changeWearItemPlus : public pTypeBase
{
	int			wearPos;
	int			vIndex;
	int			plus;
};

struct changeWearItemFlag : public pTypeBase
{
	int			wearPos;
	int			vIndex;
	int			flag;
};

#ifndef _CLIENT_
inline void makeFirstMoney(CNetMsg::SP& msg, LONGLONG nas)
{
	money* packet = reinterpret_cast<money*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_FIRST_MONENY;
	packet->nas = nas;
	packet->bonus = 0;
	msg->setSize(sizeof(money));
}

inline void makeFirstMoneyInStash(CNetMsg::SP& msg, LONGLONG nas)
{
	money* packet = reinterpret_cast<money*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_FIRST_MONENY_IN_STASH;
	packet->nas = nas;
	packet->bonus = 0;
	msg->setSize(sizeof(money));
}

inline void makeUpdateMoney(CNetMsg::SP& msg, LONGLONG nas, LONGLONG bonus)
{
	money* packet = reinterpret_cast<money*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_MONENY;
	packet->nas = nas;
	packet->bonus = bonus;
	msg->setSize(sizeof(money));
}

inline void makeUpdateMoneyInStash(CNetMsg::SP& msg, LONGLONG nas)
{
	money* packet = reinterpret_cast<money*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_MONENY_IN_STASH;
	packet->nas = nas;
	packet->bonus = 0;
	msg->setSize(sizeof(money));
}

inline void makeUpdateMoneyDescReason(CNetMsg::SP& msg, int reason, LONGLONG nas)
{
	moneyDescReason* packet = reinterpret_cast<moneyDescReason*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_NAS_DESC_REASON;
	packet->reason = reason;
	packet->nas = nas;
	msg->setSize(sizeof(moneyDescReason));
}

inline void makeUpdateItemDelete(CNetMsg::SP& msg, int tab, int iindex)
{
	deleteItemInfo* packet = reinterpret_cast<deleteItemInfo*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_DELETE_ITEM;
	packet->tab_no = tab;
	packet->invenIndex = iindex;
	msg->setSize(sizeof(deleteItemInfo));
}

inline void makeUpdateItemCount(CNetMsg::SP& msg, int tab, int iindex, LONGLONG count)
{
	changeItemCount* packet = reinterpret_cast<changeItemCount*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_CHANGE_ITEM_COUNT;
	packet->tab_no = tab;
	packet->invenIndex = iindex;
	packet->count = count;
	msg->setSize(sizeof(changeItemCount));
}

inline void makeUpdateItemCountWithSwap(CNetMsg::SP& msg, int tab, int iindex, LONGLONG count)
{
	changeItemCountWithSwap* packet = reinterpret_cast<changeItemCountWithSwap*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_CHANGE_ITEM_COUNT_WITH_SWAP;
	packet->tab_no = tab;
	packet->invenIndex = iindex;
	packet->count = count;
	msg->setSize(sizeof(changeItemCountWithSwap));
}


inline void makeUpdateItemPlus(CNetMsg::SP& msg, int tab, int iindex, int plus)
{
	changeItemPlus* packet = reinterpret_cast<changeItemPlus*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_CHANGE_ITEM_PLUS;
	packet->tab_no = tab;
	packet->invenIndex = iindex;
	packet->plus = plus;
	msg->setSize(sizeof(changeItemPlus));
}

inline void makeUpdateItemPlus_2(CNetMsg::SP& msg, int tab, int iindex, int plus_2)
{
	changeItemPlus_2* packet = reinterpret_cast<changeItemPlus_2*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_CHANGE_ITEM_PLUS_2;
	packet->tab_no = tab;
	packet->invenIndex = iindex;
	packet->plus_2 = plus_2;
	msg->setSize(sizeof(changeItemPlus_2));
}

inline void makeUpdateItemFlag(CNetMsg::SP& msg, int tab, int iindex, int flag)
{
	changeItemFlag* packet = reinterpret_cast<changeItemFlag*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_CHANGE_ITEM_FLAG;
	packet->tab_no = tab;
	packet->invenIndex = iindex;
	packet->flag = flag;
	msg->setSize(sizeof(changeItemFlag));
}

inline void makeUpdateItemUsed(CNetMsg::SP& msg, int tab, int iindex, int used)
{
	changeItemUsed* packet = reinterpret_cast<changeItemUsed*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_CHANGE_ITEM_USED;
	packet->tab_no = tab;
	packet->invenIndex = iindex;
	packet->used = used;
	msg->setSize(sizeof(changeItemUsed));
}

inline void makeUpdateItemUsed_2(CNetMsg::SP& msg, int tab, int iindex, int used_2)
{
	changeItemUsed_2* packet = reinterpret_cast<changeItemUsed_2*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_CHANGE_ITEM_USED_2;
	packet->tab_no = tab;
	packet->invenIndex = iindex;
	packet->used_2 = used_2;
	msg->setSize(sizeof(changeItemUsed_2));
}

inline void makeUpdateItemWearpos(CNetMsg::SP& msg, int tab, int iindex, int wearpos)
{
	changeItemWearPos* packet = reinterpret_cast<changeItemWearPos*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_CHANGE_ITEM_WEAR_POS;
	packet->tab_no = tab;
	packet->invenIndex = iindex;
	packet->wearPos = wearpos;
	msg->setSize(sizeof(changeItemWearPos));
}

inline void makeUpdateItemComposite(CNetMsg::SP& msg, int tab, int iindex, int composite)
{
	changeItemComposite* packet = reinterpret_cast<changeItemComposite*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_CHANGE_ITEM_SOCKET;
	packet->tab_no = tab;
	packet->invenIndex = iindex;
	packet->nCompositeItem = composite;
	msg->setSize(sizeof(changeItemComposite));
}

inline void makeUpdateEraseAllItem(CNetMsg::SP& msg, int tab)
{
	eraseAllItem* packet = reinterpret_cast<eraseAllItem*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_ERASE_ALL_ITEM;
	packet->tab_no = tab;
	msg->setSize(sizeof(eraseAllItem));
}

inline void makeUpdateWearItemUsed(CNetMsg::SP& msg, char wearPos, int vIndex, int used)
{
	changeWearItemUsed* packet = reinterpret_cast<changeWearItemUsed*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_CHANGE_WEAR_ITEM_USED;
	packet->wearPos = (int)wearPos;
	packet->vIndex = vIndex;
	packet->used = used;
	msg->setSize(sizeof(changeWearItemUsed));
}

inline void makeUpdateWearItemPlus(CNetMsg::SP& msg, char wearPos, int vIndex, int plus)
{
	changeWearItemPlus* packet = reinterpret_cast<changeWearItemPlus*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_CHANGE_WEAR_ITEM_PLUS;
	packet->wearPos = (int)wearPos;
	packet->vIndex = vIndex;
	packet->plus = plus;
	msg->setSize(sizeof(changeWearItemPlus));
}

inline void makeUpdateWearItemFlag(CNetMsg::SP& msg, char wearPos, int vIndex, int flag)
{
	changeWearItemFlag* packet = reinterpret_cast<changeWearItemFlag*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_CHANGE_WEAR_ITEM_FLAG;
	packet->wearPos = (int)wearPos;
	packet->vIndex = vIndex;
	packet->flag = flag;
	msg->setSize(sizeof(changeWearItemPlus));
}
#endif
}

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
