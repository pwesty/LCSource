#include "stdhdrs.h"

#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "CmdMsg.h"
#include "LuckyDrawBox.h"
#include "Log.h"

CLuckyDrawBox::CLuckyDrawBox()
{
	m_DataList = NULL;
	m_nCount = 0;
}

CLuckyDrawBox::~CLuckyDrawBox()
{
	RemoveAll();
}

void CLuckyDrawBox::RemoveAll()
{
	if( m_DataList )
	{
		int i;
		int count = GetCount();
		for(i=0; i<count; i++)
		{
			if( m_DataList[i].needList )
			{
				delete [] m_DataList[i].needList;
				m_DataList[i].needList = NULL;
			}

			if( m_DataList[i].resultList )
			{
				delete [] m_DataList[i].resultList;
				m_DataList[i].resultList = NULL;
			}
		}
		delete [] m_DataList;
		m_DataList = NULL;
	}

	m_nCount = 0;
}

bool CLuckyDrawBox::Load()
{
	RemoveAll();
	CDBCmd dbQuest;
	CDBCmd dbQuest1;
	dbQuest.Init(&gserver->m_dbdata);
	dbQuest1.Init(&gserver->m_dbdata);
	dbQuest.SetQuery("SELECT * FROM t_luckydrawbox WHERE a_enable=1 ORDER BY a_index");

	if ( !dbQuest.Open() )
		return false;

	if( !dbQuest.MoveFirst() )
		return true;

	if( dbQuest.m_nrecords <= 0)
		return false;

	m_nCount = dbQuest.m_nrecords;
	LuckyDrawBoxData * td = new LuckyDrawBoxData[dbQuest.m_nrecords];
	int j=0;
	do
	{
		dbQuest.GetRec("a_index",	td[j].index);
		dbQuest.GetRec("a_name",	td[j].strName);
		dbQuest.GetRec("a_random",	(int&)td[j].random);

		char strTmp[512]= {0,};
		strcpy(strTmp, "SELECT * FROM t_luckydrawNeed WHERE a_luckydraw_idx=");
		IntCat(strTmp, td[j].index, false);
		strcat(strTmp, " ORDER BY a_item_idx");

		dbQuest1.SetQuery(strTmp);
		dbQuest1.Open();
		if(dbQuest1.m_nrecords > 0 )
		{
			td[j].nNeedCount = dbQuest1.m_nrecords;
			LuckyDrawBoxNeed * ntd = new LuckyDrawBoxNeed[dbQuest1.m_nrecords];
			int i=0;
			if( dbQuest1.MoveFirst() )
			{
				do
				{
//					dbQuest1.GetRec("a_index",			rtd[i].index);
//					dbQuest1.GetRec("a_Luckydraw_idx",	ntd[i].lucky_idx);
					dbQuest1.GetRec("a_item_idx",		ntd[i].item_idx);
					dbQuest1.GetRec("a_count",			ntd[i].count);
					i++;
				}
				while( dbQuest1.MoveNext() );
			}
			td[j].needList = ntd;
		}

		strcpy(strTmp, "SELECT * FROM t_luckydrawResult WHERE a_luckydraw_idx=");
		IntCat(strTmp, td[j].index, false);
		strcat(strTmp, " ORDER BY a_item_idx");

		dbQuest1.SetQuery(strTmp);
		if(!dbQuest1.Open())
			return false;

		if(dbQuest1.m_nrecords > 0 )
		{
			if( !dbQuest1.MoveFirst() )
				return false;

			td[j].nResultCount = dbQuest1.m_nrecords;
			LuckyDrawBoxResult * rtd = new LuckyDrawBoxResult[dbQuest1.m_nrecords];
			int i=0;
			do
			{
//				dbQuest1.GetRec("a_index",			rtd[i].index);
//				dbQuest1.GetRec("a_Luckydraw_idx",	rtd[i].lucky_idx);
				dbQuest1.GetRec("a_item_idx",		rtd[i].item_idx);
				dbQuest1.GetRec("a_count",			rtd[i].count);
				dbQuest1.GetRec("a_upgrade",		rtd[i].upgrade);
				dbQuest1.GetRec("a_prob",			rtd[i].prob);
				dbQuest1.GetRec("a_flag",			rtd[i].flag);
				i++;
			}
			while( dbQuest1.MoveNext() );
			td[j].resultList = rtd;
		}
		else
			return false;

		map_.insert(map_t::value_type(td[j].index, &td[j]));

		j++;
	}
	while( dbQuest.MoveNext() );
	m_DataList = td;
	return true;
}

LuckyDrawBoxData * CLuckyDrawBox::GetData(int luckyidx)
{
	map_t::iterator it = map_.find(luckyidx);
	return (it != map_.end()) ? it->second : NULL;
}