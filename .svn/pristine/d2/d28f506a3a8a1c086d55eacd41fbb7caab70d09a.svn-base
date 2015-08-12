#ifndef __FACTORY_H__
#define __FACTORY_H__


#include <set>

typedef struct __tagFactoryStuff
{
	int		nItemIdx;
	int		nCount;
} FACTORY_STUFF;

class CFactoryProto
{
	int				m_nIndex;
	int				m_nJob;
	int				m_nSealType;			// 제작 스킬 타입
	int				m_nItemType;			// 아이템 서브 타입 (소모품은 타입)
	int				m_nItemIdx;
	LONGLONG		m_llMakeExp;
	LONGLONG		m_llNeedExp;
	LONGLONG		m_llNas;
	FACTORY_STUFF	m_stuff[MAX_FACTORY_ITEM_STUFF];

public:
	CFactoryProto();
	~CFactoryProto();

	void InitData(int nPos,
				  int nIndex,	 int nJob,
				  int nSealType, int nItemType, int nItemIdx,
				  LONGLONG llMakeExp,
				  LONGLONG llNeedExp,
				  LONGLONG llNas,
				  FACTORY_STUFF * pStuff);

	const int 				GetIndex() const
	{
		return m_nIndex;
	}
	int						GetSealType()
	{
		return m_nSealType;
	}
	int						GetItemIdx()
	{
		return m_nItemIdx;
	}
	const FACTORY_STUFF *	GetStuff(int nPos)
	{
		return &m_stuff[nPos];
	}
	LONGLONG				GetNas()
	{
		return m_llNas;
	}
	LONGLONG				GetNeedExp()
	{
		return m_llNeedExp;
	}
	LONGLONG				GetMakeExp()
	{
		return m_llMakeExp;
	}

	void SetIndex(int nIndex)
	{
		m_nIndex = nIndex;
	}
};

class CFactoryProtoList
{
public:
	typedef std::map<int, CFactoryProto*> map_t;

private:
	map_t				map_;
	CFactoryProto*		m_listFactoryItem;

public:
	CFactoryProtoList();
	~CFactoryProtoList();

	bool Load();
	CFactoryProto* Find(int nIndex);
};

///////////////////////////
// 캐릭터 제작 아이템 리스트
//
class CFactoryList
{
public:
	CFactoryList();
	~CFactoryList();

public:
	typedef std::set<CFactoryProto *> set_t;
	set_t m_listItem;

public:
	int GetCount()
	{
		return m_listItem.size();
	}

	bool Add(CFactoryProto * pFactory);
	bool Find(CFactoryProto * pFactory);
	CFactoryProto * Find(int nIndex);

	set_t& getList()
	{
		return m_listItem;
	}

	void clear();
};

#endif // __FACTORY_H__
//
