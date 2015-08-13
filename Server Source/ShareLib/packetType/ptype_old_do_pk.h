#ifndef __PTYPE_OLD_DO_PK_H__
#define __PTYPE_OLD_DO_PK_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
// MSG_PK_RECOVER_ITEMSEAL
struct doPKRecoverItemSealed : public pTypeBase
{
	unsigned short			tab;
	unsigned short			invenIndex;
	int						virtualIndex;
};
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
