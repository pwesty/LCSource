#include "stdhdrs.h"

#include "Server.h"
#include "Factory.h"
#include "../ShareLib/DBCmd.h"


///////////////////
// CFactoryProto
CFactoryProto::CFactoryProto()
{
	m_nIndex		= 0;
	m_nJob			= -1;
	m_nSealType		= -1;
	m_nItemType		= -1;
	m_nItemIdx		= -1;
	m_llMakeExp		= 0;
	m_llNeedExp		= 0;
	m_llNas			= 0;

	memset(m_stuff, -1, sizeof(FACTORY_STUFF) * MAX_FACTORY_ITEM_STUFF);
}

CFactoryProto::~CFactoryProto()
{
}

void CFactoryProto::InitData(int nPos,
							 int nIndex,	int nJob,
							 int nSealType, int nItemType, int nItemIdx,
							 LONGLONG llMakeExp,
							 LONGLONG llNeedExp,
							 LONGLONG llNas,
							 FACTORY_STUFF * pStuff
							)
{
	m_nIndex = nIndex;
	m_nJob	= nJob;
	m_nSealType = nSealType;
	m_nItemType = nItemType;
	m_nItemIdx	= nItemIdx;
	m_llMakeExp	= llMakeExp;
	m_llNeedExp	= llNeedExp;
	m_llNas		= llNas;

	for (int i = 0; i < MAX_FACTORY_ITEM_STUFF; i++)
	{
		m_stuff[i].nItemIdx	= pStuff[i].nItemIdx;
		m_stuff[i].nCount	= pStuff[i].nCount;
	}
}

///////////////////////
// CFactoryProtoList
CFactoryProtoList::CFactoryProtoList()
	: m_listFactoryItem(NULL)
{
}

CFactoryProtoList::~CFactoryProtoList()
{
	if (m_listFactoryItem)
		delete [] m_listFactoryItem;
}

CFactoryProto* CFactoryProtoList::Find(int nIndex)
{
	map_t::iterator it = map_.find(nIndex);
	return (it != map_.end()) ? it->second : NULL;
}

bool CFactoryProtoList::Load()
{
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery("SELECT * FROM t_factory_item WHERE a_enable = 1 ORDER BY a_index");
	if (!cmd.Open())
		return false;

	if (cmd.m_nrecords == 0)
	{
		LOG_ERROR("Factory item table is empty.");
		return true;
	}

	m_listFactoryItem = new CFactoryProto[cmd.m_nrecords];

	int				a_index		= 0;
	int				a_job		= 0;
	int				a_seal_type	= 0;
	int				a_item_type = 0;
	int				a_item_idx	= 0;
	LONGLONG		a_make_exp	= 0;
	LONGLONG		a_need_exp	= 0;
	LONGLONG		a_nas		= 0;
	FACTORY_STUFF	stuff[MAX_FACTORY_ITEM_STUFF];
	memset(&stuff, 0x00, sizeof(stuff));

	int count = 0;
	while (cmd.MoveNext())
	{
		if (!cmd.GetRec("a_index",		a_index		))	return false;
		if (!cmd.GetRec("a_job",		a_job		))	return false;
		if (!cmd.GetRec("a_seal_type",	a_seal_type	))	return false;
		if (!cmd.GetRec("a_item_type",	a_item_type	))	return false;
		if (!cmd.GetRec("a_item_idx",	a_item_idx	))	return false;
		if (!cmd.GetRec("a_make_exp",	a_make_exp	))	return false;
		if (!cmd.GetRec("a_need_exp",	a_need_exp	))	return false;
		if (!cmd.GetRec("a_nas",		a_nas		))	return false;

		static CLCString stuffIdx(256);
		static CLCString stuffCnt(256);
		const char* pStuffIdx = stuffIdx;
		const char* pStuffCnt = stuffCnt;

		cmd.GetRec("a_stuff",		stuffIdx);
		cmd.GetRec("a_stuff_cnt",	stuffCnt);

		char tbuf[256];
		int i = 0;
		while ((*pStuffIdx && *pStuffCnt) && i < MAX_FACTORY_ITEM_STUFF)
		{
			pStuffIdx = AnyOneArg(pStuffIdx, tbuf);
			stuff[i].nItemIdx = atoi(tbuf);

			pStuffCnt = AnyOneArg(pStuffCnt, tbuf);
			stuff[i].nCount = atoi(tbuf);
			i++;
		}

		m_listFactoryItem[count].InitData(count, a_index, a_job,
										  a_seal_type, a_item_type, a_item_idx,
										  a_make_exp, a_need_exp, a_nas, stuff);
		map_.insert(map_t::value_type(m_listFactoryItem[count].GetIndex(), &m_listFactoryItem[count]));

		++count;
	}

	return true;
}

///////////////////////////
// CFactoryList
CFactoryList::CFactoryList()
{
}

CFactoryList::~CFactoryList()
{
}

bool CFactoryList::Add(CFactoryProto * pFactory)
{
	if (!pFactory)
		return false;

	m_listItem.insert(pFactory);
	return true;
}

bool CFactoryList::Find(CFactoryProto * pFactory)
{
	set_t::iterator it = m_listItem.find(pFactory);
	return (it != m_listItem.end()) ? true : false;
}

CFactoryProto* CFactoryList::Find(int nIndex)
{
	if (nIndex < 0)
		return NULL;

	set_t::iterator it = m_listItem.begin();
	set_t::iterator endit = m_listItem.end();
	for(; it != endit; ++it)
	{
		CFactoryProto* p = *(it);

		if (p->GetIndex() == nIndex)
		{
			return p;
		}
	}

	return NULL;
}

void CFactoryList::clear()
{
	m_listItem.clear();
}

