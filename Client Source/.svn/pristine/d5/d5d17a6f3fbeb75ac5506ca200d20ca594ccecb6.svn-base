#ifndef __PTYPE_OLD_DO_ATTACK_H__
#define __PTYPE_OLD_DO_ATTACK_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	struct doPDAttack : public pTypeBase 
	{
		int			tIndex;
		char		tCharType;
		char		attackType;
		short		multicount;
		struct tag_multi  
		{
			int			index;
		} list[0];
	};

	struct doAttack : public pTypeBase 
	{
		int			tIndex;
		int			aIndex;
		char		tCharType;
		char		aCharType;
		char		attackType;
		short		multicount;
		struct tag_multi
		{
			int			index;
		} list[0];
	};
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
