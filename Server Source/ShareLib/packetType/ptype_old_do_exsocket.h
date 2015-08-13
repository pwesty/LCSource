#ifndef __PTYPE_OLD_DO_EX_SOCKET_H__
#define __PTYPE_OLD_DO_EX_SOCKET_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
// subtype - MSG_EX_SOCKET_UPGRADE_JEWEL_REQ
struct doExSocketUpgradeJewel : public pTypeThirdBase
{
	int				npcIndex;
	unsigned short	tab_1;			// 보석 아이템 1
	unsigned short	invenIndex_1;
	int				virualIndex_1;
	unsigned short	tab_2;			// 보석 아이템 2
	unsigned short	invenIndex_2;
	int				virualIndex_2;
	unsigned short	tab_3;			// 보석 아이템 3
	unsigned short	invenIndex_3;
	int				virualIndex_3;
	unsigned short	tab_4;			// 안정제
	unsigned short	invenIndex_4;
	int				virualIndex_4;
	int				itemCount;
};

// subtype - MSG_EX_SOCKET_UPGRADE_JEWEL_CHAOS_REQ
struct doExSocketUpgradeChaosJewel : public pTypeThirdBase
{
	int				npcIndex;
	unsigned short	tab_1;			// 보석 아이템 1
	unsigned short	invenIndex_1;
	int				virualIndex_1;
	unsigned short	tab_2;			// 보석 아이템 2
	unsigned short	invenIndex_2;
	int				virualIndex_2;
	unsigned short	tab_3;			// 보석 아이템 3
	unsigned short	invenIndex_3;
	int				virualIndex_3;
	unsigned short	tab_4;			// 안정제
	unsigned short	invenIndex_4;
	int				virualIndex_4;
	int				itemCount;
};

// subtype - MSG_EX_SOCKET_MAKE_REQ
struct doExSocketMake : public pTypeThirdBase
{
	int				npcIndex;
	unsigned short	tab_1;
	unsigned short	invenIndex_1;
	int				virualIndex_1;
	unsigned short	tab_2;
	unsigned short	invenIndex_2;
	int				virualIndex_2;
};

// subtype - MSG_EX_SOCKET_COMBINE_JEWEL_REQ
struct doExSocketCombineJewel : public pTypeThirdBase
{
	int				npcIndex;
	unsigned short	tab_1;
	unsigned short	invenIndex_1;
	int				virualIndex_1;
	unsigned short	tab_2;
	unsigned short	invenIndex_2;
	int				virualIndex_2;
	unsigned short	tab_3;				// 파괴 방지 보조제
	unsigned short	invenIndex_3;
	int				virualIndex_3;
	char			pos;
};

// subtype - MSG_EX_SOCKET_CLEAN_JEWEL_REQ
struct doExSocketCleanJewel : public pTypeThirdBase
{
	int				npcIndex;
	unsigned short	tab;
	unsigned short	invenIndex;
	int				virualIndex;
};

// subtype - MSG_EX_SOCKET_CLEAN_ONE_JEWEL_REQ
struct doExSocketCleanOneJewel : public pTypeThirdBase
{
	int				npcIndex;
	unsigned short	tab_1;
	unsigned short	invenIndex_1;
	int				virualIndex_1;
	unsigned short	tab_2;
	unsigned short	invenIndex_2;
	int				virualIndex_2;
	char			pos;
};
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
