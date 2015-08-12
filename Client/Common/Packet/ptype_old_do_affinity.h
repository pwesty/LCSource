#ifndef __PTYPE_OLD_DO_AFFINITY_H__
#define __PTYPE_OLD_DO_AFFINITY_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	// thirdtype - MSG_EX_AFFINITY_ITEM_CONTRIBUTE_REQ
	struct doAffinityContribute : public pTypeThirdBase 
	{
		int					npcIndex;
		int					clientPoint;
		unsigned short		listCount;
		struct  
		{
			int				itemDBIndex;
			int				count;
		} list[0];
	};
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
