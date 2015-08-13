#include "stdhdrs.h"
#include "Exp.h"
#include "Server.h"
#include "DBCmd.h"

static LONGLONG gLevelupExp[MAX_LEVEL];
char			g_buf2[8192];
void InitExp()
{
	CDBCmd dbexp;
	dbexp.Init(&*(MYSQL *)(0x8428870));
	memset( g_buf2, 0, MAX_STRING_LENGTH );
	sprintf( g_buf2, " SELECT a_level, a_exp FROM t_exp WHERE a_level <= %d order by a_level ", MAX_LEVEL );
	dbexp.SetQuery(g_buf2);
	if( !dbexp.Open() )
	{
		puts("t_exp Error");
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
			puts("Exp Error");
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
