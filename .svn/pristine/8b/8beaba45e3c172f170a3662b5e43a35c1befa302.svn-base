#include "stdhdrs.h"

#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "CmdMsg.h"
#include "Promotion2Event.h"
#include "Log.h"

CPromotion2::CPromotion2()
{
	m_data = NULL;
	m_count = 0;
}

CPromotion2::~CPromotion2()
{
	RemoveAll();
}

void CPromotion2::RemoveAll()
{
	if( m_data )
	{
		int i;
		for(i=0; i<m_count; i++)
		{
			int j;
			for(j=0; j<m_data[i].count; j++)
			{
				if( m_data[i].itemlist )
				{
					delete [] m_data[i].itemlist;
					m_data[i].itemlist = NULL;
				}
			}
		}
		delete [] m_data;
		m_data = NULL;
	}
	m_count = 0;
}

bool CPromotion2::Load()
{
	RemoveAll();

	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery("SELECT * FROM t_promotion2 GROUP BY a_kind_idx order by a_kind_idx");

	if ( !cmd.Open() )
		return false;

	if( !cmd.MoveFirst() )
		return true;

	if( cmd.m_nrecords <= 0)
		return false;

	m_count = cmd.m_nrecords;

	_promotion2Info * pi = new _promotion2Info[m_count];
	int i=0;
	do
	{
		cmd.GetRec("a_kind_idx" , pi[i].kind );
		i++;
	}
	while( cmd.MoveNext() );

	for( i=0; i<m_count; i++ )
	{
		CLCString sql;
		sql.Format( "SELECT * FROM t_promotion2 where a_kind_idx=%d" , pi[i].kind );
		cmd.SetQuery(sql);

		if ( !cmd.Open() )
			return false;

		if( !cmd.MoveFirst() )
			return true;

		pi[i].count = cmd.m_nrecords;
		pi[i].itemlist = new Promotion2ItemInfo[pi[i].count];

		int j=0;
		do
		{
			cmd.GetRec("a_item_idx", pi[i].itemlist[j].index);
			cmd.GetRec("a_item_flag", pi[i].itemlist[j].flag);
			cmd.GetRec("a_item_plus", pi[i].itemlist[j].plus);
			cmd.GetRec("a_item_count", pi[i].itemlist[j].count);
			j++;
		}
		while( cmd.MoveNext() );

		map_.insert(map_t::value_type(pi[i].kind, &pi[i]));
	}
	m_data = pi;
	return true;
}

Promorion2Info * CPromotion2::GetPromotionData(int kind)
{
	map_t::iterator it = map_.find(kind);
	return (it != map_.end()) ? it->second : NULL;
}

//