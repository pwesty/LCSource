#ifndef _PROMOTION2_EVENT_HEADER_
#define _PROMOTION2_EVENT_HEADER_

typedef struct _promotion2ItemInfo
{
	int			index;
	LONGLONG	count;
	int			flag;
	int			plus;
	_promotion2ItemInfo()
	{
		index	= -1;
		count	= -1;
		flag	= -1;
		plus	= -1;
	}
} Promotion2ItemInfo;

typedef struct _promotion2Info
{
	int kind;
	int count;
	Promotion2ItemInfo * itemlist;
	_promotion2Info()
	{
		kind = 0;
		count = 0;
		itemlist = NULL;
	}
} Promorion2Info;

class CPromotion2
{
public:
	typedef std::map<int, Promorion2Info *> map_t;

protected:
	int	m_count;
	Promorion2Info* m_data;
	map_t			map_;

public:
	CPromotion2();
	~CPromotion2();

	bool Load();
	void RemoveAll();
	Promorion2Info * GetPromotionData(int kind);
};
#endif // _PROMOTION2_EVENT_HEADER_
//
