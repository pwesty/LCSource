#ifndef __PTYPE_OLD_DO_EVENT_H__
#define __PTYPE_OLD_DO_EVENT_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
// subtype - MSG_EX_CHAOSBALL
struct doChaosBall : public pTypeThirdBase
{
	unsigned short	tab;
	unsigned short	invenIndex;
};
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
