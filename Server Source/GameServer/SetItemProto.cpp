#include "stdhdrs.h"

#include "Server.h"
#include "SetItemProto.h"
#include "../ShareLib/DBCmd.h"

///////////////////
// CSetItemProto
CSetItemProto::CSetItemProto()
{
	m_nIndex = 0;
	m_nJob = 0;
	memset(m_nItemIdx,	0, sizeof(int) * MAX_WEARING);
	memset(m_option,	0, sizeof(m_option));
}

CSetItemProto::~CSetItemProto()
{
}

void CSetItemProto::InitData(int nPos, int nIndex, int nJob, int nOptionCnt, int* pItemIdx, SETITEMOPTION * pOption)
{
	m_nIndex = nIndex;
	m_nJob	= nJob;
	m_nOptionCnt = nOptionCnt;

	int i = 0;
	for (i = 0; i < MAX_WEARING; i++)
	{
		m_nItemIdx[i] = pItemIdx[i];
	}

	for (i = 0; i < MAX_SET_ITEM_OPTION; i++)
	{
		m_option[i].nWearCnt	= pOption[i].nWearCnt;
		m_option[i].nType		= pOption[i].nType;
		m_option[i].nIndex		= pOption[i].nIndex;
		m_option[i].nLevel		= pOption[i].nLevel;
	}
}

///////////////////////
// CSetItemProtoList
CSetItemProtoList::CSetItemProtoList()
{
	m_listSetItem = NULL;
	m_nCount = 0;
}

CSetItemProtoList::~CSetItemProtoList()
{
	while (m_listSetItem)
	{
		delete [] m_listSetItem;
		m_listSetItem = NULL;
	}
	m_nCount = 0;
}

CSetItemProto* CSetItemProtoList::Find(int nIndex)
{
	map_t::iterator it = map_.find(nIndex);
	return (it != map_.end()) ? it->second : NULL;
}

bool CSetItemProtoList::Load()
{
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery("SELECT * FROM t_set_item WHERE a_enable = 1 ORDER BY a_set_idx");
	if (!cmd.Open())
		return false;

	if (cmd.m_nrecords == 0)
	{
		LOG_ERROR("Set_Item table is empty.");
		return true;
	}

	m_listSetItem = new CSetItemProto[cmd.m_nrecords];

	int				a_set_idx		= 0;
	int				a_job			= 0;
	int				a_option_count	= 0;
	int				a_item_idx[MAX_WEARING]	= {-1,};
	SETITEMOPTION	option[MAX_SET_ITEM_OPTION];
	memset(&option, 0x00, sizeof(option));

	m_nCount = 0;
	while (cmd.MoveNext())
	{
		if (!cmd.GetRec("a_set_idx",		a_set_idx		))	return false;
		if (!cmd.GetRec("a_job",			a_job			))	return false;
		if (!cmd.GetRec("a_option_count",	a_option_count	))	return false;

		static CLCString itemIdx(256);
		static CLCString wearCnt(256);
		static CLCString optionType(256);
		static CLCString optionIdx(256);
		static CLCString optionLevel(256);

		const char* pItemIdx	 = itemIdx;
		const char* pWearCnt	 = wearCnt;
		const char* pOptionType	 = optionType;
		const char* pOptionIdx	 = optionIdx;
		const char* pOptionLevel = optionLevel;

		cmd.GetRec("a_item_idx",		itemIdx		);
		cmd.GetRec("a_wear_count",		wearCnt		);
		cmd.GetRec("a_option_type",		optionType	);
		cmd.GetRec("a_option_idx",		optionIdx	);
		cmd.GetRec("a_option_level",	optionLevel	);

		char tbuf[256];
		int i = 0;
		while (*pItemIdx && i < MAX_WEARING)
		{
			pItemIdx = AnyOneArg(pItemIdx, tbuf);
			a_item_idx[i] = atoi(tbuf);
			i++;
		}

		i = 0;
		while ((*pWearCnt && *pOptionType && *pOptionIdx && *pOptionLevel) && i < MAX_SET_ITEM_OPTION)
		{
			pWearCnt = AnyOneArg(pWearCnt, tbuf);
			option[i].nWearCnt = atoi(tbuf);

			pOptionType = AnyOneArg(pOptionType, tbuf);
			option[i].nType = atoi(tbuf);

			pOptionIdx = AnyOneArg(pOptionIdx, tbuf);
			option[i].nIndex = atoi(tbuf);

			pOptionLevel = AnyOneArg(pOptionLevel, tbuf);
			option[i].nLevel = atoi(tbuf);

			i++;
		}

		m_listSetItem[m_nCount].InitData(m_nCount, a_set_idx, a_job, a_option_count, a_item_idx, option);

		map_.insert(map_t::value_type(m_listSetItem[m_nCount].GetIndex(), &m_listSetItem[m_nCount]));

		m_nCount++;
	}

	return true;
}
