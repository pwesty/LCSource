#ifndef _LUCKY_DRAW_BOX_HEADER_20090924_
#define _LUCKY_DRAW_BOX_HEADER_20090924_

typedef struct __tagLuckyDrawBoxNeed
{
//	int index;
//	int lucky_idx;
	int item_idx;
	LONGLONG count;
	__tagLuckyDrawBoxNeed()
	{
//		index	= 0;
//		lucky_idx	= 0;
		item_idx	= 0;
		count		= 0;
	}
} LuckyDrawBoxNeed;

typedef struct __tagLuckyDrawBoxResult
{
//	int index;
//	int lucky_idx;
	int item_idx;
	LONGLONG count;
	int upgrade;
	int	prob;
	int	flag;

	__tagLuckyDrawBoxResult()
	{
//		index		= 0;
//		lucky_idx	= 0;
		item_idx	= 0;
		count		= 0;
		upgrade		= 0;
		prob		= 0;
		flag		= 0;
	}
} LuckyDrawBoxResult;

typedef struct __tagLuckyDrawBoxData
{
	int			index;
	int			random;
	CLCString	strName;

	int			nNeedCount;
	int			nResultCount;

	LuckyDrawBoxNeed	* needList;
	LuckyDrawBoxResult	* resultList;
	__tagLuckyDrawBoxData()
	{
		index		 = 0;
		random		 = -1;
		nNeedCount	 = 0;
		nResultCount = 0;
		needList	= NULL;
		resultList	= NULL;
	}
} LuckyDrawBoxData;

class CLuckyDrawBox
{
public:
	typedef std::map<int, LuckyDrawBoxData *> map_t;

	CLuckyDrawBox();
	~CLuckyDrawBox();

protected:
	map_t			map_;
	LuckyDrawBoxData * m_DataList;
	int	m_nCount;

public:

	bool Load();
	void RemoveAll();

	int GetCount()
	{
		return m_nCount;
	}
	LuckyDrawBoxData * GetData(int Luckyidx);
};

#endif // #ifndef _LUCKY_DRAW_BOX_HEADER_20090924_
//
