#ifndef __PTYPE_DURABILITY_H__
#define __PTYPE_DURABILITY_H__

#include "ptype_base.h"

// MAIN TYPE = MSG_DURABILITY
enum // SUB TYPE
{
	MSG_SUB_DURABILITY_NORMAL_REPAIR,		// 장비 내구도 일반 수리
	MSG_SUB_DURABILITY_SPECIAL_REPAIR,		// 장비 내구도 특수 수리
	MSG_SUB_DURABILITY_RECOVERY,			// 장비 내구도 복구
};

enum
{
	ERR_NO_DURABILITY_OK,				// 성공
	ERR_NO_DURABILITY_NOT_ENOUGH_MONEY,	// 나스 부족
	ERR_NO_DURABILITY_NOT_ENOUGH_ITEM_COUNT,	// 신비한 망치 갯수가 부족함
	ERR_NO_DURABILITY_TOO_MUSH_ITEM_COUNT,	// 신비한 망치 갯수가 너무 많습니다.
	ERR_NO_DURABILITY_DO_NOT_REPAIR,		// 수리할 필요가 없음
	ERR_NO_DURABILITY_DO_NOT_RECOVERY,		// 복구할 필요가 없음
};

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
struct repairDurability : public pTypeBase	// 장비 내구도 일반 수리
{
	unsigned short		tab_no;
	unsigned short		invenIndex;
	int					virualIndex;
	LONGLONG			fee;
};

struct specialRepairDurability : public pTypeBase // 장비 내구도 특수 수리
{
	unsigned short		tab_no;
	unsigned short		invenIndex;
	int					virualIndex;

	// 특수 재료
	unsigned short		material_tab_no;
	unsigned short		material_invenIndex;
	int					material_virtualIndex;
};

struct recoveryDurability : public pTypeBase // 장비 내구도 복구
{
	unsigned short		tab_no;
	unsigned short		invenIndex;
	int					virualIndex;

	// 특수 재료
	unsigned short		material_tab_no;
	unsigned short		material_invenIndex;
	int					material_virtualIndex;
	unsigned short		material_count;
};
}

namespace ResponseClient
{
struct repairDurability : public pTypeBase	// 장비 내구도 일반 수리
{
	unsigned char		result;
};

struct specialRepairDurability : public pTypeBase // 장비 내구도 특수 수리
{
	unsigned char		result;
};

struct recoveryDurability : public pTypeBase // 장비 내구도 복구
{
	unsigned char		result;
};

#ifndef _CLIENT_
inline void makeRepairDurability(CNetMsg::SP& msg, int result)
{
	repairDurability* packet = reinterpret_cast<repairDurability*>(msg->m_buf);
	packet->type = MSG_DURABILITY;
	packet->subType = MSG_SUB_DURABILITY_NORMAL_REPAIR;
	packet->result = result;
	msg->setSize(sizeof(repairDurability));
}

inline void makeSpecialRepairDurability(CNetMsg::SP& msg, int result)
{
	specialRepairDurability* packet = reinterpret_cast<specialRepairDurability*>(msg->m_buf);
	packet->type = MSG_DURABILITY;
	packet->subType = MSG_SUB_DURABILITY_SPECIAL_REPAIR;
	packet->result = result;
	msg->setSize(sizeof(specialRepairDurability));
}

inline void makeRecoveryDurability(CNetMsg::SP& msg, int result)
{
	recoveryDurability* packet = reinterpret_cast<recoveryDurability*>(msg->m_buf);
	packet->type = MSG_DURABILITY;
	packet->subType = MSG_SUB_DURABILITY_RECOVERY;
	packet->result = result;
	msg->setSize(sizeof(recoveryDurability));
}
#endif
}

namespace UpdateClient
{
struct itemDurabilityForInventory : public pTypeBase
{
	unsigned short		tab;
	unsigned short		invenIndex;

	unsigned short		now_durability; // 현재 장비 내구도
	unsigned short		max_durability; // 최대 장비 내구도
};

struct itemDurabilityForWear: public pTypeBase
{
	unsigned short		wearPos;

	unsigned short		now_durability; // 현재 장비 내구도
};

#ifndef _CLIENT_
inline void makeItemDurabilityForInventory(CNetMsg::SP& msg, int tab, int invenIndex, int now_dur, int max_dur)
{
	itemDurabilityForInventory* packet = reinterpret_cast<itemDurabilityForInventory*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_ITEM_DURABILITY_FOR_INVENTORY;
	packet->tab = tab;
	packet->invenIndex = invenIndex;
	packet->now_durability = now_dur;
	packet->max_durability = max_dur;
	msg->setSize(sizeof(itemDurabilityForInventory));
}

inline void makeItemDurabilityForWear(CNetMsg::SP& msg, int wearPos, int now_dur)
{
	itemDurabilityForWear* packet = reinterpret_cast<itemDurabilityForWear*>(msg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_ITEM_DURABILITY_FOR_WEAR;
	packet->wearPos = wearPos;
	packet->now_durability = now_dur;
	msg->setSize(sizeof(itemDurabilityForWear));
}
#endif
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
