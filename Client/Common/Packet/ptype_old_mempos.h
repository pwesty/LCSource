#ifndef __PTYPE_OLD_MEMPOS_H__
#define __PTYPE_OLD_MEMPOS_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	struct memposWrite : public pTypeBase 
	{
		char		slot;
		char		comment[MEMPOS_COMMENT_LENGTH + 1];
	};

	struct memposMove : public pTypeBase 
	{
		char		slot;
	};
}

namespace ResponseClient
{
	struct memposElement 
	{
		unsigned char	index;
		int				zone;
		float			x;
		float			z;
		char		comment[MEMPOS_COMMENT_LENGTH + 1];
	};

	struct memposWrite : public pTypeBase 
	{
		memposElement	data;
	};

	struct memposList : public pTypeBase 
	{
		int					memposTime;
		unsigned short		listCount;
		memposElement		list[0];
	};
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
