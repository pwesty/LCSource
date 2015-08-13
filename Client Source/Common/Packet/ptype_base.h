#ifndef __PTYPE_BASE_H__
#define __PTYPE_BASE_H__

#define		INVALID_SHORT_INDEX		((unsigned short)-1)

#ifdef	_CLIENT_
#	include <Common/CommonDef.h>
#endif	// _CLIENT_

#pragma pack(push, 1)

struct pTypeBase 
{
	unsigned char		type;
	unsigned char		subType;
};

struct pTypeThirdBase
{
	unsigned char		type;
	int					subType;
	unsigned char		thirdType;
};

#pragma pack(pop)
#endif
