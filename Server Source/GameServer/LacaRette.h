#ifndef _LACARETTE_HEADER_
#define _LACARETTE_HEADER_

#include <string>
#include <boost/format.hpp>

#define LACARETTE_GIVE_ITEM_MAX			(10)

typedef struct _giveItemInfo
{
	int			index;
	LONGLONG	count;
	int			flag;
	int			prob;
	_giveItemInfo()
	{
		index	= -1;
		count	= -1;
		flag	= -1;
		prob	= -1;
	}
} GiveItemInfo;

typedef struct _courseInfo
{
	int index;
	int	coinUseCnt;
	int* coinIndex;
	int* coinDefCnt;
	int* coinAddCnt;
	GiveItemInfo * giveItem;
	_courseInfo()
	{
		index		= -1;
		coinUseCnt	= 0;
		coinIndex	= NULL;
		coinDefCnt	= NULL;
		coinAddCnt	= NULL;
	}
} CourseInfo;

class CLacarette
{
protected:
	int	m_count;
	CourseInfo* m_data;
	std::vector<std::string>	a_giveItem_str;

public:
	CLacarette();
	~CLacarette();

	bool Load();
	void RemoveAll();
	int	GetNeedTokenCount(int cosArrNum, int tokenIndex, int repeat, int &tokenArrNum);
	CourseInfo * GetCourseData(int CourseArrNum);
};
#endif
//
