#ifndef __PTYPE_OLD_DO_REFORM_SYSTEM_H__
#define __PTYPE_OLD_DO_REFORM_SYSTEM_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	// MSG_EX_REFORMER_SYSTEM
	struct doReform : public pTypeThirdBase
	{
		int						npcIndex;
		unsigned short			reformer_tab;
		unsigned short			reformer_invenIndex;
		unsigned short			magnifire_tab;
		unsigned short			magnifire_invenIndex;
		unsigned short			reform_tab;
		unsigned short			reform_invenIndex;
		char					grade;
		int						count;
	};
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
