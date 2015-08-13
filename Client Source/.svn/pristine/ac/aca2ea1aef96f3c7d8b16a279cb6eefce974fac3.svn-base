#ifndef __PTYPE_TIMER_ITEM_H__
#define __PTYPE_TIMER_ITEM_H__

#include "ptype_base.h"

// 기간제 아이템
enum
{
	TIMERITEM_NON,
	TIMETITEM_TYPE_REMOTE_EXPRESS_SYSTEM,		// 원격 창고NPC

	TIMERITEM_TYPE_CASH_INVENTORY_1,
	TIMERITEM_TYPE_CASH_INVENTORY_2,
	TIMERITEM_TYPE_PET_STASH,

	TIMERITEM_MAX,
};

// 패킷 sub type
enum
{
	MSG_SUB_TIMER_ITEM_LIST,		// 기간제 아이템 리스트
	MSG_SUB_TIMER_ITEM_MODIFY,		// 기간제 아이템 추가 및 시간 변경
	MSG_SUB_TIMER_ITEM_DELETE,		// 기간제 아이템 삭제
};

#pragma pack(push, 1)

namespace ResponseClient
{
	struct listTimerItem : public pTypeBase
	{
		int count;
		struct _list
		{
			int type;
			int expire_time;	// unix time
		} list[0];
	};

	struct modifyTimerItem : public pTypeBase
	{
		int item_type;
		int expire_time;
	};

	struct deleteTimerItem : public pTypeBase
	{
		int item_type;
	};

#ifndef _CLIENT_
	inline void makeTimerItemModify(CNetMsg::SP& msg, int type, int expire_time)
	{
		modifyTimerItem* p = reinterpret_cast<modifyTimerItem*>(msg->m_buf);
		p->type = MSG_TIMER_ITEM;
		p->subType = MSG_SUB_TIMER_ITEM_MODIFY;
		p->item_type = type;
		p->expire_time = expire_time;
		msg->setSize(sizeof(modifyTimerItem));
	}

	inline void makeTiemrItemDelete(CNetMsg::SP& msg, int type)
	{
		deleteTimerItem* p = reinterpret_cast<deleteTimerItem*>(msg->m_buf);
		p->type = MSG_TIMER_ITEM;
		p->subType = MSG_SUB_TIMER_ITEM_DELETE;
		p->item_type = type;
		msg->setSize(sizeof(deleteTimerItem));
	}
#endif
}

#pragma pack(pop)

#endif
