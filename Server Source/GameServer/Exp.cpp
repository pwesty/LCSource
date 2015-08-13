#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Exp.h"
#include "Server.h"
#include "../ShareLib/DBCmd.h"

static LONGLONG gLevelupExp[MAX_LEVEL];

void InitExp()
{
	CDBCmd dbexp;
	dbexp.Init(&gserver->m_dbdata);

	std::string select_exp_query = "";
#ifdef IMP_SPEED_SERVER
	if( gserver->m_bSpeedServer )
		select_exp_query = boost::str(boost::format(" SELECT a_level, a_exp FROM t_exp_speed WHERE a_level <= %d order by a_level ") % MAX_LEVEL );
	else
#endif //IMP_SPEED_SERVER
		select_exp_query = boost::str(boost::format(" SELECT a_level, a_exp FROM t_exp WHERE a_level <= %d order by a_level ") % MAX_LEVEL );
	dbexp.SetQuery(select_exp_query);
	if( !dbexp.Open() )
	{
		LOG_INFO("t_exp Error");
		exit(0);
	}

	while (dbexp.MoveNext())
	{
		int level = 0;
		dbexp.GetRec("a_level", level );
		if( level > 0 )
			dbexp.GetRec("a_exp", gLevelupExp[level-1] );
		else
		{
			LOG_INFO("Exp Error");
			exit(0);
		}
	}
}

LONGLONG GetLevelupExp(int level)
{
	level--;
	if (level < 0)
		return 0;
	else if (level >= MAX_LEVEL)
		level = MAX_LEVEL - 1;

	return gLevelupExp[level];
}
//