
#ifndef		ACTION_DATA_H_
#define		ACTION_DATA_H_

#include "def_base.h"

#define DEF_ACTION_DEFAULT_LENGTH 32

#pragma pack(push, 1)
struct stAction : public stTbl_base
{
	char	type;
	int		job;
	int		iconid;
	int		iconrow;
	int		iconcol;
};
#pragma pack(pop)

#endif		// ACTION_DATA_H_