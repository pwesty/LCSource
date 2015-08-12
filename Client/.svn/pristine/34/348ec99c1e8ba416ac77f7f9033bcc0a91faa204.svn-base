
#ifndef		OPTION_DATA_H_
#define		OPTION_DATA_H_

#include "def_base.h"

#define DEF_OPTION_MAX_LEVEL 36
#define DEF_RAREOPTION_MAX 10

#pragma pack(push, 1)

struct stOption : public stTbl_base
{
	int						type;
	int						value[DEF_OPTION_MAX_LEVEL];
};

struct stRareOptionInfo
{
	int OptionIdx;
	int OptionLevel;
};

struct stRareOption : public stTbl_base
{
	int		a_grade;
	int		a_type;
	int		a_attack;
	int		a_defense;
	int		a_magic;
	int		a_resist;
	stRareOptionInfo rareOption[DEF_RAREOPTION_MAX];
};

#pragma pack(pop)

#endif		// OPTION_DATA_H_