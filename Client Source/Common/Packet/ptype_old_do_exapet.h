#ifndef __PTYPE_OLD_DO_EXAPET_H__
#define __PTYPE_OLD_DO_EXAPET_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
// subtype - MSG_SUB_REBIRTH
struct doExApetRebirth : public pTypeThirdBase
{
	unsigned short	npcIndex;
	unsigned short	tab;
	unsigned short	invenIndex;
};

// subtype - MSG_SUB_ITEM_WEAR
struct doExApetItemWear : public pTypeThirdBase
{
	unsigned short	tab;
	unsigned short	invenIndex;
	char			itemIdx;
	char			wearPos;
};

// subtype - MSG_SUB_EVOLUTION
struct doExApetEvolution : public pTypeThirdBase
{
	unsigned short	tab;
	unsigned short	invenIndex;
};
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
