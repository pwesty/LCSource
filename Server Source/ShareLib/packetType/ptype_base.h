#ifndef __PTYPE_BASE_H__
#define __PTYPE_BASE_H__

#define INVALID_SHORT_INDEX		((unsigned short) -1)

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

struct pTypeServer
{
	unsigned char	type;
	int				subType;
};

#pragma pack(pop)

#endif
