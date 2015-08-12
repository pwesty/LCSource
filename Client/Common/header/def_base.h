
#ifndef		DEF_BASE_H_
#define		DEF_BASE_H_

struct stTbl_base
{
	int		index;

	int		getindex()	{ return index; }

	stTbl_base() : index(-1) {}
};

#endif		// DEF_BASE_H_