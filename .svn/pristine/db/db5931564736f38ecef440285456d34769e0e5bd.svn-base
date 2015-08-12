#define __SERVER_CPP__

#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Log.h"
#include "Server.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"
#include "../ShareLib/packetType/ptype_syndicate.h"

#ifdef PREMIUM_CHAR
#include "../ShareLib/packetType/ptype_premium_char.h"
#endif

CServer gserver;

void ServerSrandom(unsigned long initial_seed);

CServer::CServer()
{
	m_desclist = NULL;
	m_nDesc = 0;

	m_seqGen = 1;

	m_serverpath = GetServerPath();
	m_maxplayers = 0;
	m_bshutdown = false;

	m_pulse = 0;

	m_nMoonStoneNas = 0;

	mysql_init(&m_dbchar);

#ifdef EXTREME_CUBE_VER2
	m_CubePointUpdateTime = 0;
#endif // EXTREME_CUBE_VER2
}

CServer::~CServer()
{
	if (m_serverpath)
		delete[] m_serverpath;

	{
		map_party_t::iterator it = m_listParty.begin();
		map_party_t::iterator endit = m_listParty.end();
		for (; it != endit; ++it)
		{
			delete it->second;
		}
		m_listParty.clear();
	}

	{
		map_expedition_t::iterator it = m_listExped.begin();
		map_expedition_t::iterator endit = m_listExped.end();
		for (; it != endit; ++it)
		{
			delete it->second;
		}
		m_listExped.clear();
	}

	{
		map_partymatchmember_t::iterator it = m_listPartyMatchMember.begin();
		map_partymatchmember_t::iterator endit = m_listPartyMatchMember.end();
		for (; it != endit; ++it)
		{
			delete it->second;
		}
		m_listPartyMatchMember.clear();
	}
	{
		map_partymatchparty_t::iterator it = m_listPartyMatchParty.begin();
		map_partymatchparty_t::iterator endit = m_listPartyMatchParty.end();
		for (; it != endit; ++it)
		{
			delete it->second;
		}
		m_listPartyMatchParty.clear();
	}
#ifdef DEV_GUILD_STASH
	std::map<int,CGuildStash*>::iterator itr = m_guildstash.begin();
	std::map<int, CGuildStash*>::iterator itrEnd = m_guildstash.end();
	for( ; itr != itrEnd ; ++itr )
	{
		CGuildStash* delStash = itr->second;
		if( delStash )
		{
			delStash->RemoveItem();
			delete delStash;
		}
	}
	m_guildstash.clear();
#endif //DEV_GUILD_STASH
}

char* CServer::GetServerPath()
{
	char szBuffer[1024];
#ifdef CIRCLE_WINDOWS
	GetModuleFileName(::GetModuleHandle(NULL), szBuffer, 1000);
	int path_len = strlen(szBuffer);
	int i;

	for (i = path_len - 1; i >= 0; i-- )
	{
		if (szBuffer[i] == '\\')
		{
			szBuffer[i+1] = '\0';
			break;
		}
	}
	if (i < 0)
		return false;
#else
	getcwd (szBuffer, 512);
	strcat (szBuffer, "/");
#endif

	char* ret = new char[strlen(szBuffer) + 1];
	strcpy(ret, szBuffer);
	return ret;
}

bool CServer::LoadSettingFile()
{
	puts("Load setting file....");

	CLCString strTemp(1024);

	strTemp.Format("%s%s", m_serverpath, "newStobm.bin");

	if (!m_config.Load(strTemp))
		return false;

	m_serverno = atoi(m_config.Find("Server", "Number"));

	return true;
}

bool CServer::InitGame()
{
	ServerSrandom(time(0));

	GAMELOG << init("SYSTEM") << "Finding player limit." << end;
	m_maxplayers = GetMaxPlayers();

	GAMELOG << init("SYSTEM") << "Entering game loop." << end;

	this->initTime();

	return true;
}

int CServer::GetMaxPlayers()
{
#ifndef CIRCLE_UNIX
	return MAX_PLAYING;
#else
	int max_descs = 0;
	const char *method;
	/*
	 * First, we'll try using getrlimit/setrlimit.  This will probably work
	 * on most systems.  HAS_RLIMIT is defined in sysdep.h.
	 */
#ifdef HAS_RLIMIT
	{
		struct rlimit limit;

		/* find the limit of file descs */
		method = "rlimit";
		if (getrlimit(RLIMIT_NOFILE, &limit) < 0)
		{
			GAMELOG << init("SYS_ERR") << "calling getrlimit" << end;
			exit(1);
		}

		/* set the current to the maximum */
		limit.rlim_cur = limit.rlim_max;
		if (setrlimit(RLIMIT_NOFILE, &limit) < 0)
		{
			GAMELOG << init("SYS_ERR") << "calling setrlimit" << end;
			exit(1);
		}
#ifdef RLIM_INFINITY
		if (limit.rlim_max == RLIM_INFINITY)
			max_descs = MAX_PLAYING + NUM_RESERVED_DESCS;
		else
			max_descs = MIN(MAX_PLAYING + NUM_RESERVED_DESCS, limit.rlim_max);
#else
		max_descs = MIN(MAX_PLAYING + NUM_RESERVED_DESCS, limit.rlim_max);
#endif
	}
#elif defined (OPEN_MAX) || defined(FOPEN_MAX)
#if !defined(OPEN_MAX)
#define OPEN_MAX FOPEN_MAX
#endif
	method = "OPEN_MAX";
	max_descs = OPEN_MAX;		/* Uh oh.. rlimit didn't work, but we have
				 * OPEN_MAX */
#elif defined (_SC_OPEN_MAX)
	/*
	* Okay, you don't have getrlimit() and you don't have OPEN_MAX.  Time to
	* try the POSIX sysconf() function.  (See Stevens' _Advanced Programming
	* in the UNIX Environment_).
	*/
	method = "POSIX sysconf";
	errno = 0;
	if ((max_descs = sysconf(_SC_OPEN_MAX)) < 0)
	{
		if (errno == 0)
			max_descs = m_MaxPlaying + NUM_RESERVED_DESCS;
		else
		{
			GAMELOG << init("SYS_ERR") << "Error calling sysconf" << end;
			exit(1);
		}
	}
#else
	/* if everything has failed, we'll just take a guess */
	method = "random guess";
	max_descs = m_MaxPlaying + NUM_RESERVED_DESCS;
#endif

	/* now calculate max _players_ based on max descs */
	max_descs = MIN(MAX_PLAYING, max_descs - NUM_RESERVED_DESCS);

	if (max_descs <= 0)
	{
		GAMELOG << init("SYS_ERR") << "Non-positive max player limit!  (Set at" << max_descs << "using" << method << ")" << end;
		exit(1);
	}

	GAMELOG << init("SYSTEM") << "Setting player limit to" << max_descs << "using" << method << "." << end;

	return (max_descs);
#endif /* CIRCLE_UNIX */
}

bool CServer::ConnectDB()
{
	if (!mysql_real_connect (
				&m_dbchar,
				m_config.Find("Char DB", "IP"),
				m_config.Find("Char DB", "User"),
				m_config.Find("Char DB", "Password"),
				m_config.Find("Char DB", "DBName"),
				0, NULL, 0))
	{
		LOG_ERROR("Can't connect data DB : ip[%s] id[%s] pw[%s] dbname[%s] error[%s]",
				  m_config.Find("Char DB", "IP"),
				  m_config.Find("Char DB", "User"),
				  m_config.Find("Char DB", "Password"),
				  m_config.Find("Char DB", "DBName"),
				  mysql_error(&m_dbchar)
				 );
		return false;
	}
	return true;
}

void CServer::DisconnectDB()
{
	mysql_close (&m_dbchar);
}

bool CServer::LoadSettings()
{
	GAMELOG << init("SYSTEM")
			<< "Load settings...."
			<< end;

	// LOADING
	GAMELOG << init("SYSTEM")
			<< "Load Guild...."
			<< end;

	CDBCmd cmd;
	cmd.Init(&m_dbchar);

	// check t_extend_guild, t_extend_guildmember, t_characters_guildpoint
	//<===! kjban add
	CDBCmd cmdNewGuild;
	int cnt;

	cmdNewGuild.Init(&m_dbchar);
	cmdNewGuild.SetQuery( "SELECT count(*) FROM t_characters_guildpoint" );
	if( !cmdNewGuild.Open() )
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_characters_guildpoint") << end;
		return false;
	}

	cmdNewGuild.MoveFirst();
	cmdNewGuild.GetRec(0, cnt);

	if(cnt == 0)
	{
		// 테이블에 내용이 없을때
		cmdNewGuild.SetQuery("insert into t_characters_guildpoint select a_index, 0, 0 from t_characters");
		cmdNewGuild.Open();
	}

	cmdNewGuild.SetQuery("SELECT count(*) FROM t_extend_guild");
	if (!cmdNewGuild.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_extend_guild") << end;
		return false;
	}

	cmdNewGuild.MoveFirst();
	cmdNewGuild.GetRec(0, cnt);

	if(cnt == 0)
	{
		// 테이블에 내용이 없을때
		cmdNewGuild.SetQuery("insert into t_extend_guild select a_index, 0, 5+a_level*5, 0, '', '', '' from t_guild");
		cmdNewGuild.Open();
	}

	cmdNewGuild.SetQuery("SELECT count(*) FROM t_extend_guildmember");
	if (!cmdNewGuild.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_extend_guildmember") << end;
		return false;
	}

	cmdNewGuild.MoveFirst();
	cmdNewGuild.GetRec(0, cnt);

	if(cnt == 0)
	{
		// 테이블에 내용이 없을때
		cmdNewGuild.SetQuery("insert into t_extend_guildmember select a_guild_index, a_char_index, '', 0, 0, 0 from t_guildmember");
		cmdNewGuild.Open();
	}
	//!===>

#ifdef DEV_GUILD_MARK
	cmd.SetQuery( "SELECT g.*, eg.a_guild_point, eg.a_guild_incline, eg.a_guild_maxmember, eg.a_guild_land, eg.a_passiveskill_index, eg.a_passiveskill_level, eg.a_gm_row, eg.a_gm_col, eg.a_bg_row, eg.a_bg_col, eg.a_marktime, eg.a_kick_status, eg.a_kick_request_char_index, eg.a_kick_request_time"
				  " FROM t_guild AS g, t_extend_guild AS eg WHERE g.a_index = eg.a_guild_index AND g.a_enable = 1 ORDER BY g.a_index " );
#else
	cmd.SetQuery( "SELECT g.*, eg.a_guild_point, eg.a_guild_incline, eg.a_guild_maxmember, eg.a_guild_land, eg.a_passiveskill_index, eg.a_passiveskill_level, eg.a_kick_status, eg.a_kick_request_char_index, eg.a_kick_request_time"
				  " FROM t_guild AS g, t_extend_guild AS eg WHERE g.a_index = eg.a_guild_index AND g.a_enable = 1 ORDER BY g.a_index " );
#endif
	if (!cmd.Open())
	{
		GAMELOG << init("SYS_ERR")
				<< "Cannot load guild"
				<< end;
		return false;
	}

	CDBCmd cmd2;
	cmd2.Init(&m_dbchar);
	while (cmd.MoveNext())
	{
		int guildindex;
		CLCString guildname(MAX_GUILD_NAME_LENGTH + 1);
		int guildlevel;

		int battleIndex;
		int battlePrize;
		int battleZone;
		int battleTime;
		int battleKillCount;
		int battleState;
#ifdef DEV_GUILD_MARK
		char gm_row;
		char gm_col;
		char bg_row;
		char bg_col;
		int markTime;
#endif
		int kickStatus			= GMKS_NORMAL;
		int kickRequestChar		= 0;
		int kickRequestTime		= 0;
		//int lastTimeBossChange	= 0;

		cmd.GetRec("a_index", guildindex);
		cmd.GetRec("a_name", guildname);
		cmd.GetRec("a_level", guildlevel);

		cmd.GetRec("a_battle_index",	battleIndex);
		cmd.GetRec("a_battle_prize",	battlePrize);
		cmd.GetRec("a_battle_zone",		battleZone);
		cmd.GetRec("a_battle_time",		battleTime);
		cmd.GetRec("a_battle_killcount",battleKillCount);
		cmd.GetRec("a_battle_state",	battleState);
#ifdef DEV_GUILD_MARK
		cmd.GetRec("a_gm_row", gm_row);
		cmd.GetRec("a_gm_col", gm_col);
		cmd.GetRec("a_bg_row", bg_row);
		cmd.GetRec("a_bg_col", bg_col);
		cmd.GetRec("a_marktime", markTime);
#endif

		cmd.GetRec("a_kick_status", kickStatus);
		cmd.GetRec("a_kick_request_char_index", kickRequestChar);
		cmd.GetRec("a_kick_request_time", kickRequestTime);

		int guildpoint;
		int incline;
		CLCString	guildland(256);
		CLCString	guildskillindex(256);
		CLCString	guildskillLevel(256);
		int	maxmember;
// 		char tbuf[256];
//		const char* pland = guildland;
		int land[256];

		memset( land, -1, sizeof(land) );

		cmd.GetRec( "a_guild_point", guildpoint );
		cmd.GetRec( "a_guild_incline", incline );
		cmd.GetRec( "a_guild_maxmember", maxmember );

		CDBCmd Castle;
		Castle.Init(&m_dbchar);
		CLCString sql( 256 );
		sql.Format( "SELECT a_zone_index  FROM t_castle  WHERE a_owner_guild_index = %d", guildindex );

		Castle.SetQuery( sql );
		if (!Castle.Open() )
		{
			GAMELOG << init("SYS_ERR")
					<< "Cannot load castle"
					<< end;
			return false;
		}
		int landcount = Castle.m_nrecords;
		if( landcount > 0 && Castle.MoveFirst() )
		{
			int i =0;

			do
			{
				Castle.GetRec( "a_zone_index", land[i] );
				i++;
			}
			while( Castle.MoveNext() );
		}

		std::string select_guild_member_query = boost::str(boost::format(
				"SELECT g.*, eg.a_position_name, eg.a_contribute_exp, eg.a_contribute_fame, a_point"
				" FROM t_guildmember AS g, t_extend_guildmember AS eg "
				" WHERE g.a_guild_index = eg.a_guild_index AND g.a_char_index = eg.a_char_index AND g.a_guild_index = %d ")
												% guildindex);
		cmd2.SetQuery(select_guild_member_query);
		if (!cmd2.Open())
		{
			GAMELOG << init("SYS_ERR")
					<< "Cannot load guild"
					<< end;
			return false;
		}

		if (cmd2.m_nrecords == 0)
		{
			DeleteGuild(guildindex);
			continue ;
		}

		// TODO : GUILD : 차후 트리 구조 고려
		CGuild * guild = m_guildlist.create(guildindex, guildname, guildlevel, battleIndex, battlePrize, battleTime, battleZone, battleKillCount, battleState);

		if (guild == NULL)
			continue ;

		m_guildlist.add(guild);
		GAMELOG << init("NEW_GUILD" )
				<< "guildIndex" << delim << guild->index() << delim
				<< "guildPoint" << delim << guildpoint << delim
				<< "incline" << delim << incline << delim
				<< "landcount" << delim << landcount << delim
				<< "Maxmember" << delim << maxmember << delim;
		guild->guildPoint( guildpoint );
		guild->incline( incline );
		guild->landcount( landcount );
		if( landcount >  0 )
			guild->land( landcount, land );
		guild->setMaxmember( maxmember );
#ifdef DEV_GUILD_MARK
		guild->SetGuildMark(gm_row, gm_col, bg_row, bg_col, markTime);
#endif

		guild->getGuildKick()->setKickStatus(kickStatus);
		guild->getGuildKick()->setKickReuestChar(kickRequestChar);
		guild->getGuildKick()->setKickRequestTime(kickRequestTime);

		bool bFoundBoss = false;

		while (cmd2.MoveNext())
		{
			int charindex;
			CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
			int pos;

			cmd2.GetRec("a_char_index", charindex);
			cmd2.GetRec("a_char_name", charname);
			cmd2.GetRec("a_pos", pos);

			int listidx = guild->addmember(charindex, charname);
			switch (pos)
			{
			case MSG_GUILD_POSITION_BOSS:
				bFoundBoss = true;
				guild->changeboss(listidx);
				break;

			case MSG_GUILD_POSITION_OFFICER:
				guild->appoint(listidx);
				break;
			}

			CLCString positionname( GUILD_POSITION_NAME + 1 );
			//char positionname[GUILD_POSITION_NAME+1];
			int	contributeExp;
			int contributeFame;
			int point;

			cmd2.GetRec( "a_position_name", positionname );
			cmd2.GetRec( "a_contribute_exp", contributeExp );
			cmd2.GetRec( "a_contribute_fame", contributeFame );
			cmd2.GetRec( "a_point", point );

			guild->member(listidx)->contributeExp( contributeExp );
			guild->member(listidx)->contributeFame( contributeFame );
			guild->member(listidx)->positionName( positionname );
			guild->member(listidx)->cumulatePoint( point );

			switch( pos )
			{
			case MSG_GUILD_POSITION_RUSH_CAPTAIN:		// 돌격조 대장
			case MSG_GUILD_POSITION_SUPPORT_CAPTAIN:	// 지원조 대장
			case MSG_GUILD_POSITION_RECON_CAPTAIN:		// 정찰조 대장
			case MSG_GUILD_POSITION_RUSH_MEMBER:		// 돌격조원
			case MSG_GUILD_POSITION_SUPPORT_MEMBER:		// 지원조원
			case MSG_GUILD_POSITION_RECON_MEMBER:		// 정찰조원
			case MSG_GUILD_POSITION_MEMBER:				// 일반 길드원
				guild->member(listidx)->pos(pos);
				break;
			}
			guild->AddGradeExPosCount(pos);
		}

		if (!bFoundBoss)
		{
			if (guild->member(0))
				guild->changeboss(0);
		}
	}

	CDBCmd cmd3;
	cmd3.Init(&m_dbchar);
	std::string select_quild_mak_query = boost::str(boost::format("SELECT * FROM t_guildmark WHERE a_server = %d") % gserver.m_serverno);
	cmd3.SetQuery(select_quild_mak_query);

	if (!cmd3.Open())
	{
		GAMELOG << init("SYS_ERR")
				<< "Cannot load guildmark"
				<< end;
		return false;
	}

	int rank = 0, idx = 0;
	while(cmd3.MoveNext())
	{
		cmd3.GetRec("a_rank", rank);
		cmd3.GetRec("a_idx", idx);
		if( rank < 4 && rank > 0)
		{
			gserver.m_nGuildMarkTable[rank-1] = idx;
		}
	}

#ifdef EXTREME_CUBE
	cmd.SetQuery("SELECT * FROM t_cubepoint LIMIT 1");
	if(!cmd.Open())
	{
		GAMELOG << init("SYS_ERR")
				<< "cannot load t_cubepoint"
				<< end;
		return false;
	}
	cmd.SetQuery("SELECT * FROM t_cubepoint_personal LIMIT 1");
	if(!cmd.Open())
	{
		GAMELOG << init("SYS_ERR")
				<< "cannot load t_cubepoint_personal"
				<< end;
		return false;
	}
#ifdef EXTREME_CUBE_VER2
	cmd.SetQuery("SELECT * FROM t_cuberank LIMIT 1");
	if(!cmd.Open())
	{
		GAMELOG << init("SYS_ERR")
				<< "cannot load t_cuberank"
				<< end;
		return false;
	}
#endif // EXTREME_CUBE_VER2
#endif // EXTREME_CUBE

	cmd.SetQuery("SELECT * FROM t_trigger_saveinfo LIMIT 1");
	if(!cmd.Open())
	{
		GAMELOG << init("SYS_ERR")
				<< "cannot load t_trigger_saveinfo"
				<< end;
		return false;
	}

#ifdef EXTREME_CUBE_VER2
	int updatetime;
	updatetime = SetCurCubePointUpdateTime();
	CheckCubeReward();
	GAMELOG << init("SYSTEM")
			<< "SetCubeUpdateTime" << delim << updatetime << end;
#endif // EXTREME_CUBE_VER2

#ifdef DEV_GUILD_STASH
	GAMELOG << init("SYSTEM") << "BACKUP GUILD_STASH LOG";
	cmd.SetQuery("INSERT DELAYED INTO t_guild_stash_log_backup SELECT * FROM t_guild_stash_log where a_date < DATE_SUB( NOW(), INTERVAL 1 MONTH );");
	if( cmd.Update() )
	{
		GAMELOG << delim << "DELETE GUILD_STASH LOG";
		cmd.SetQuery("DELETE FROM t_guild_stash_log where a_date < DATE_SUB( NOW(), INTERVAL 1 MONTH );");
		cmd.Update();
	}
	GAMELOG << end;
#endif // DEV_GUILD_STASH

	// RVR 창조의 보석 로딩
	m_syndicate.load();
	return true;
}

void CServer::SendToAllGameServer(CNetMsg::SP msg)
{
	CDescriptor* d = m_desclist;
	while (d)
	{
		if (d->m_countZone && STATE(d) == CON_PLAYING)
		{
			SEND_Q(msg, d);
		}
		d = d->m_pNext;
	}
}

bool CServer::DeleteGuild(int guildindex)
{
	// DB
	struct tm ti = NOW();
	std::string update_guild_query = boost::str(boost::format("UPDATE t_guild SET a_name=CONCAT(a_name,'_%02d%02d%02d%02d%02d%02d'), a_enable=0, a_recentdate=NOW() WHERE a_index=%d")
									 % ti.tm_year % ti.tm_mon % ti.tm_mday % ti.tm_hour % ti.tm_min % ti.tm_sec % guildindex);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(update_guild_query);
	if (!cmd.Update())
		return false;

#ifdef DEV_GUILD_STASH
	std::string update_guild_stash_query = boost::str(boost::format("UPDATE t_guild_stash_info SET a_enable=0 WHERE a_guild_idx = %d")% guildindex);
	cmd.SetQuery(update_guild_stash_query);
	cmd.Update();
#endif //DEV_GUILD_STASH
	return true;
}

bool CServer::DeleteGuildMember(int guildindex, const char* guildname, int charindex, const char* charname, bool bKick)
{
	// DB
	std::string delete_guild_member_query = boost::str(boost::format("DELETE FROM t_guildmember WHERE a_guild_index = %d AND a_char_index = %d") % guildindex % charindex);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(delete_guild_member_query);
	if (!cmd.Update())
		return false;

	std::string delete_extend_guild_member_query = boost::str(boost::format("DELETE FROM t_extend_guildmember WHERE a_guild_index = %d AND a_char_index = %d") % guildindex % charindex);
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(delete_extend_guild_member_query);
	if (!cmd.Update())
		return false;

	std::string udpate_guild_query = boost::str(boost::format("UPDATE t_guild SET a_recentdate=NOW() WHERE a_index = %d") % guildindex);
	cmd.SetQuery(udpate_guild_query);
	cmd.Update();

	if (bKick)
	{
		GAMELOG << init("GUILD MEMBER KICK")
				<< guildindex << delim
				<< guildname << delim
				<< "CHARACTER" << delim
				<< charindex << delim
				<< charname
				<< end;
	}
	else
	{
		GAMELOG << init("GUILD MEMBER OUT")
				<< guildindex << delim
				<< guildname << delim
				<< "CHARACTER" << delim
				<< charindex << delim
				<< charname
				<< end;
	}

	return true;
}

void CServer::SendToAllSubServer(CNetMsg::SP msg, int nSubNo)
{
	CDescriptor* d = m_desclist;
	while (d)
	{
		if (d->m_countZone && STATE(d) == CON_PLAYING && d->m_subNo == nSubNo)
		{
			SEND_Q(msg, d);
		}
		d = d->m_pNext;
	}
}

CParty* CServer::FindPartyByBossIndex(int nSubNo, int nBossIndex)
{
	LONGLONG key = MAKE_LONGLONG_KEY(nSubNo, nBossIndex);
	map_party_t::iterator it = m_listParty.find(key);
	return (it != m_listParty.end()) ? it->second : NULL;
}

CParty* CServer::FindPartyByMemberIndex(int nSubNo, int nMemberIndex, bool bIncludeRequest)
{
	map_party_t::iterator it = m_listParty.begin();
	map_party_t::iterator endit = m_listParty.end();
	for( ; it != endit; ++it)
	{
		CParty* pParty = it->second;

		if (pParty->GetSubNo() != nSubNo)
			continue;

		if (pParty->FindMember(nMemberIndex) != -1)
			return pParty;
		if (bIncludeRequest && pParty->GetRequestIndex() == nMemberIndex)
			return pParty;
	}

	return NULL;
}

CExpedition* CServer::FindExpedByBossIndex(int nSubNo, int nBossIndex)
{
	LONGLONG key = MAKE_LONGLONG_KEY(nSubNo, nBossIndex);
	map_expedition_t::iterator it = m_listExped.find(key);
	return (it != m_listExped.end()) ? it->second : NULL;
}

CExpedition* CServer::FindExpedByMemberIndex(int nSubNo, int nMemberIndex, bool bIncludeRequest)
{
	map_expedition_t::iterator it = m_listExped.begin();
	map_expedition_t::iterator endit = m_listExped.end();
	for (; it != endit; ++it)
	{
		CExpedition* pExped = it->second;
		if (pExped->GetSubNo() != nSubNo)
			continue;

		if (pExped->FindMemberListIndex(nMemberIndex) != -1)
			return pExped;
		if (bIncludeRequest && pExped->GetRequestIndex() == nMemberIndex)
			return pExped;
	}

	return NULL;
}

CPartyMatchMember* CServer::FindPartyMatchMemberByCharIndex(int nSubNo, int nCharIndex)
{
	LONGLONG key = MAKE_LONGLONG_KEY(nSubNo, nCharIndex);
	map_partymatchmember_t::iterator it = m_listPartyMatchMember.find(key);
	return (it != m_listPartyMatchMember.end()) ? it->second : NULL;
}

CPartyMatchParty* CServer::FindPartyMatchPartyByBossIndex(int nSubNo, int nBossIndex)
{
	LONGLONG key = MAKE_LONGLONG_KEY(nSubNo, nBossIndex);
	map_partymatchparty_t::iterator it = m_listPartyMatchParty.find(key);
	return (it != m_listPartyMatchParty.end()) ? it->second : NULL;
}

void CServer::SendGuildMemberList(CGuild* guild, CDescriptor* desc)
{
	const int maxmember = 20;

	CGuildMember* guildmember[maxmember];
	CGuildMember* member;

	int startidx = 0;			// 원본 데이터 index
	int count;					// 1회 전송시 데이터 count
	int sendcount = 0;			// 전체 전송완료한 데이터 count
	int sendmaxmember = guild->membercount();			// 데이터 보내야할 전체 길드멤버 count

	do
	{
		count = 0;
		memset(guildmember, 0, sizeof(CGuildMember*) * maxmember);
		for( ; startidx < GUILD_MAX_MEMBER && count < maxmember; ++startidx)
		{
			member = guild->member(startidx);
			if(member)
			{
				guildmember[count] = member;
				count++;
			}
		}

		if(count > 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);

			rmsg->Init(MSG_HELPER_COMMAND);
			RefMsg(rmsg) << MSG_HELPER_GUILD_MEMBER_LIST
						 << guild->index()
						 << count;

			int i;
			for(i = 0; i < count; ++i)
			{
				if(guildmember[i])
				{
					RefMsg(rmsg) << guildmember[i]->charindex()
								 << guildmember[i]->GetName()
								 << guildmember[i]->pos()
								 << guildmember[i]->online();
				}
				else
				{
					// 로그찍기 : error
					GAMELOG << init("GUILD MEMBER SEND ERROR")
							<< guild->index() << delim
							<< sendmaxmember << delim
							<< sendcount << delim
							<< count << delim
							<< i << delim
							<< startidx << delim
							<< end;
				}
			}

			SEND_Q(rmsg, desc);

			sendcount += count;
		}
	}
	while( (startidx < GUILD_MAX_MEMBER) && sendcount < sendmaxmember);
}

void CServer::SendExtendGuildMemberList(CGuild* guild, CDescriptor* desc)
{
	const int maxmember = 20;

	CGuildMember* guildmember[maxmember];
	CGuildMember* member;

	int startidx = 0;
	int count;
	int sendcount = 0;
	int sendmaxmember = guild->membercount();

	do
	{
		count = 0;
		memset(guildmember, 0, sizeof(CGuildMember*) * maxmember);
		for( ; startidx < GUILD_MAX_MEMBER && count < maxmember; ++startidx)
		{
			member = guild->member(startidx);
			if(member)
			{
				guildmember[count] = member;
				count++;
			}
		}

		if(count > 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);

			rmsg->Init(MSG_HELPER_COMMAND);
			RefMsg(rmsg) << MSG_HELPER_NEW_GUILD_MEMBER_LIST
						 << guild->index()
						 << count;

			int i;
			for(i = 0; i < count; ++i)
			{
				if(guildmember[i])
				{
					RefMsg(rmsg) << guildmember[i]->charindex();
					RefMsg(rmsg) << guildmember[i]->GetcontributeExp()
								 << guildmember[i]->GetcontributeFame()
								 << guildmember[i]->GetcumulatePoint()
								 << guildmember[i]->GetPositionName()
								 << guildmember[i]->GetChannel()
								 << guildmember[i]->GetZoneNum();
				}
				else
				{
					// 로그찍기 error
					GAMELOG << init("EXTEND GUILD MEMBER SEND ERROR")
							<< guild->index() << delim
							<< sendmaxmember << delim
							<< sendcount << delim
							<< count << delim
							<< i << delim
							<< startidx << delim
							<< end;
				}
			}

			SEND_Q(rmsg, desc);

			sendcount += count;
		}
	}
	while( (startidx < GUILD_MAX_MEMBER) && sendcount < sendmaxmember);
}

#ifdef PARTY_BUG_GER
void CServer::PrintPartyMemberList(int nSubNo, int nFindCharIdx)
{
	CParty* party = gserver.FindPartyByMemberIndex(nSubNo, nFindCharIdx, true);
	CPartyMember* member;
	if(party)
	{
		GAMELOG << init("PARTY_BUG_GER PRINT PARTY LIST");

		int i;
		for (i = 0; i < MAX_PARTY_MEMBER; ++i)
		{
			member = party->GetMemberByListIndex(i);
			if(member)
			{
				GAMELOG << member->GetCharIndex() << " ";
			}
		}

		GAMELOG << end;
	}
	else
	{
		GAMELOG << init("PARTY_BUG_GER NOT FOUND PARTY")
				<< "CHAR INDEX" << delim << nFindCharIdx
				<< end;
	}
}
#endif // PARTY_BUG_GER

#ifdef EXTREME_CUBE_VER2
void CServer::CubePointRanking(int CubePointUpdateTime)
{
	int guildidx, cubepoint, charidx;
	char rank;
	CLCString sql(1024), sql2(1024);
	int CubePointRankingTime = CubePointUpdateTime+604800;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	sql.Format("DELETE FROM t_cuberank WHERE a_insert_week=%d", CubePointRankingTime);
	cmd.SetQuery(sql);
	cmd.Update();

	// 1. 길드큐브포인트 순위
	sql.Format("SELECT c.* FROM t_cubepoint c, t_guild g "
			   "WHERE g.a_enable=1 AND g.a_index=c.a_guild_index AND c.a_week_date=%d"
			   " ORDER BY c.a_cubepoint desc, c.a_update_date asc LIMIT 5", CubePointUpdateTime);

	cmd.SetQuery(sql);
	cmd.Open();

	CDBCmd cmd2;
	cmd2.Init(&gserver.m_dbchar);

	rank = 1;
	int guildpoint;
	CGuild* guild;
	while(cmd.MoveNext())
	{
		guildpoint = 0;

		cmd.GetRec("a_guild_index", guildidx);
		cmd.GetRec("a_cubepoint", cubepoint);

		sql2.Format("INSERT INTO t_cuberank (a_insert_week, a_type, a_rank, a_typeidx, a_cubepoint)"
					"VALUES(%d, %d, %d, %d, %d)", CubePointRankingTime, 0, rank, guildidx, cubepoint);
		cmd2.SetQuery(sql2);
		if(cmd2.Update())
		{
			GAMELOG << init("EXTREME_CUBE_GUILDCUBE_RANKING")
					<< "RANK" << delim << rank << delim
					<< "GUILD" << delim << guildidx << delim
					<< "GUILDPOINT" << delim << guildpoint
					<< end;
		}
		else
		{
			GAMELOG << init("EXTREME_CUBE_GUILDCUBE_RANKING_FAIL")
					<< "RANK" << delim << rank << delim
					<< "GUILD" << delim << guildidx << delim
					<< "GUILDPOINT" << delim << guildpoint
					<< end;
		}

		// 길드포인트를 증가시켜준다.
		switch(rank)
		{
		case 1:
			guildpoint = 1000;
			break;
		case 2:
			guildpoint = 700;
			break;
		case 3:
			guildpoint = 500;
			break;
		case 4:
			guildpoint = 300;
			break;
		case 5:
			guildpoint = 100;
			break;
		default:
			break;
		}

		guild = gserver.m_guildlist.findguild(guildidx);
		if(guildpoint > 0 && guild)
		{
			// 길드포인트 넣어줌(보상)
			sql2.Format("UPDATE t_extend_guild eg, t_guild g SET eg.a_guild_point=eg.a_guild_point+%d"
						" WHERE g.a_index=eg.a_guild_index AND g.a_index=1 AND eg.a_guild_index=%d",
						guildpoint,
						guildidx);
			cmd2.SetQuery(sql2);
			if(cmd2.Update())
			{
				GAMELOG << init("EXTREME_CUBE_GUILDCUBE_REWARD")
						<< "RANK" << delim << rank << delim
						<< "GUILD" << delim << guildidx << delim
						<< "GUILDPOINT" << delim << guildpoint
						<< end;
			}
			else
			{
				GAMELOG << init("EXTREME_CUBE_GUILDCUBE_REWARD_FAIL")
						<< "RANK" << delim << rank << delim
						<< "GUILD" << delim << guildidx << delim
						<< "GUILDPOINT" << delim << guildpoint
						<< end;
			}
			guild->AddGuildPoint(guildpoint);

			CNetMsg::SP rmsg(new CNetMsg);
			HelperCubeRewardGuildPointRepMsg(rmsg, guildidx, rank, guildpoint);
			SendToAllGameServer(rmsg);
		}

		rank++;
	}

	// 2. 개인큐브포인트 순위
	rank = 1;
	sql.Format("SELECT cp.* FROM t_cubepoint_personal cp, t_characters c "
			   "WHERE c.a_enable=1 AND c.a_index=cp.a_char_idx AND cp.a_week_date=%d"
			   " ORDER BY cp.a_cubepoint desc, cp.a_update_date asc LIMIT 5", CubePointUpdateTime);

	cmd.SetQuery(sql);
	cmd.Open();
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_char_idx", charidx);
		cmd.GetRec("a_cubepoint", cubepoint);

		sql2.Format("INSERT INTO t_cuberank (a_insert_week, a_type, a_rank, a_typeidx, a_cubepoint)"
					"VALUES(%d, %d, %d, %d, %d)", CubePointRankingTime, 1, rank, charidx, cubepoint);
		cmd2.SetQuery(sql2);
		if(cmd2.Update())
		{
			GAMELOG << init("EXTREME_CUBE_PERSONAL_RANKING")
					<< "RANK" << delim << rank << delim
					<< "CHARINDEX" << delim << charidx << delim
					<< "CUBEPOINT" << delim << cubepoint
					<< end;
		}
		else
		{
			GAMELOG << init("EXTREME_CUBE_PERSONAL_RANKING_FAIL")
					<< "RANK" << delim << rank << delim
					<< "CHARINDEX" << delim << charidx << delim
					<< "CUBEPOINT" << delim << cubepoint
					<< end;
		}
		rank++;
	}
}

void CServer::CheckCubeReward()
{
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	CLCString sql(1024);
	sql.Format("SELECT * FROM t_cuberank WHERE a_insert_week=%d", m_CubePointUpdateTime);
	cmd.SetQuery(sql);
	cmd.Open();
	if(cmd.m_nrecords == 0)
	{
		CubePointRanking(m_CubePointUpdateTime-604800);
	}
}

int CServer::SetCurCubePointUpdateTime()
{
	time_t nowtime;
	time(&nowtime);
	if(m_CubePointUpdateTime > nowtime || nowtime >= m_CubePointUpdateTime + 60 * 60 * 24 * 7)
	{
		tm now = NOW();
		int subdate;
		subdate = now.tm_wday - 1;
		if(subdate < 0)
			subdate += 7;
		now.tm_mday -= subdate;
		now.tm_hour = 0;
		now.tm_min = 0;
		now.tm_sec = 0;
		now.tm_isdst = -1;
		m_CubePointUpdateTime = mktime(&now);
	}
	return m_CubePointUpdateTime;
}

#endif // EXTREME_CUBE_VER2

void CServer::operate( rnSocketIOService* service )
{
	CDescriptor* newd = new CDescriptor(service);
	service->SetUserData((void *)newd);

	STATE(newd) = CON_GET_LOGIN;

	/* prepend to list */
	ADD_TO_BILIST(newd, m_desclist, m_pPrev, m_pNext);

	LOG_INFO("CLIENT CONNECTED... IP[%s]", service->ip().c_str());
}

void CServer::saveCastleDungeonInfo()
{
	CDBCmd cmd;
	std::string query = boost::str(boost::format("select a_zone_index from t_castle_dungeon where a_zone_index = %d") % ZONE_DRATAN_CASTLE_DUNGEON);
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(query);

	if(cmd.m_nrecords == 0)
	{
		query = boost::str(boost::format("update t_castle_dungeon set a_env_rate = %d, a_mop_rate = %d, a_tax_rate = %d, a_hunt_rate = %d")
			% m_castle_env_rate % m_castle_mob_rate % m_castle_tax_rate % m_castle_hunt_rate);
		cmd.SetQuery(query);
		cmd.Update();
	}
	else
	{
		query = boost::str(boost::format("INSERT INTO t_castle_dungeon (a_zone_index, a_tax_rate, a_env_rate, a_mop_rate, a_hunt_rate) VALUES (%d, %d, %d, %d, %d)")
			% ZONE_DRATAN_CASTLE_DUNGEON % m_castle_env_rate % m_castle_mob_rate % m_castle_tax_rate % m_castle_hunt_rate);
		cmd.SetQuery(query);
		cmd.Update();
	}
}


//////////////////////////////////////////////////////////////////////////

void timerPerSecond::operate( rnSocketIOService* service )
{
	gserver.m_pulse += 20;
	gserver.HeartBeat();
}

timerPerSecond* timerPerSecond::instance()
{
	static timerPerSecond __instance;
	return &__instance;
}

//////////////////////////////////////////////////////////////////////////

void timerPerMinute::operate( rnSocketIOService* service )
{
	mysql_ping(&gserver.m_dbchar);

	// 자동 저장 rvr - 정보
	gserver.m_syndicate.save();
}

void timerPerMinute::Init()
{
	time_t nowtime = time(0);
	int remain_sec = 60 - ((int)nowtime % 60);

	bnf::instance()->CreateSecTimerPeriod(remain_sec, 60, this);
}

timerPerMinute* timerPerMinute::instance()
{
	static timerPerMinute __instance;
	return &__instance;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

void timerPerHour::operate( rnSocketIOService* service )
{
	int jewelPoint_k = gserver.m_syndicate.getJewelPoint(SYNDICATE::eSYNDICATE_KAILUX);
	int jewelPoint_d = gserver.m_syndicate.getJewelPoint(SYNDICATE::eSYNDICATE_DEALERMOON);

	if( jewelPoint_k > 100 )
	{
		gserver.m_syndicate.setJewelPoint(SYNDICATE::eSYNDICATE_KAILUX, jewelPoint_k - 100);
	}
	if( jewelPoint_d > 100 )
	{
		gserver.m_syndicate.setJewelPoint(SYNDICATE::eSYNDICATE_DEALERMOON, jewelPoint_d - 100);
	}

	gserver.m_syndicate.sendInfo();
}

void timerPerHour::Init()
{
	time_t nowtime = time(0);
	int remain_sec = 3600 - ((int)nowtime % 3600);

	bnf::instance()->CreateSecTimerPeriod(remain_sec, 3600, this);
}

timerPerHour* timerPerHour::instance()
{
	static timerPerHour __instance;
	return &__instance;
}

//////////////////////////////////////////////////////////////////////////

// 이 함수는 매 1초마다 호출됨
void ClearMessageListTimer::operate( rnSocketIOService* service )
{
	CMsgListNode* nowNode = NULL;
	CMsgListNode* nextNode = gserver.m_msgList.m_head;
	while (nowNode = nextNode)
	{
		nextNode = nowNode->m_pNext;

		++nowNode->m_sendTime;
		if (nowNode->m_sendTime > 5)
		{
			int seq, server, subno, zone;
			unsigned char subtype;

			CNetMsg::SP& msg = nowNode->m_msg;

			if (msg->m_mtype != MSG_HELPER_REQ)
				continue;

			msg->MoveFirst();
			RefMsg(msg) >> seq
						>> server >> subno >> zone
						>> subtype;

			GAMELOG << init("TIMEOUT")
					<< "server" << delim << server << delim << "sub" << delim << subno << delim
					<< "messege" << delim << msg->m_mtype << delim << subtype << end;

			// 노드 지우기
			REMOVE_FROM_BILIST(nowNode, gserver.m_msgList.m_head, m_pPrev, m_pNext);
			delete nowNode;
		}
	}
}

ClearMessageListTimer* ClearMessageListTimer::instance()
{
	static ClearMessageListTimer __instance;
	return &__instance;
}

//////////////////////////////////////////////////////////////////////////

void timerPerMidnight::Init()
{
	time_t nowtime = time(0);
	struct tm nowtm;
	memcpy(&nowtm, localtime(&nowtime), sizeof(nowtm));
	nowtm.tm_isdst = -1;
	nowtm.tm_hour = 0;
	nowtm.tm_min = 0;
	nowtm.tm_sec = 0;

	time_t newtime = mktime(&nowtm);	// 오늘 0시 0분 0초
	newtime += 86400;					// 내일 0시 0분 0초

	int remain_sec = (int)(newtime - nowtime);

	bnf::instance()->CreateSecTimerPeriod(remain_sec, 86400, this);

#ifdef PREMIUM_CHAR
	//////////////////////////////////////////////////////////////////////////
	// DB에서 이전에 초기화 한 시간을 얻음
	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);
	dbChar.SetQuery("SELECT UNIX_TIMESTAMP(reset_time) as reset_time FROM t_premiumchar_reset_jumpcount ORDER BY reset_time DESC LIMIT 1");
	if (dbChar.Open() == false)
	{
		return;
	}

	int resetTime = 0;

	// 최초 즉, 데이터가 없는 경우
	if (dbChar.m_nrecords == 0)
	{
		resetTime = (int)mktime(&nowtm);
		
		std::string query = boost::str(boost::format(
			"INSERT INTO t_premiumchar_reset_jumpcount VALUES(FROM_UNIXTIME(%1%))") % resetTime);
		dbChar.SetQuery(query);
		dbChar.Update();
	}
	else
	{
		dbChar.MoveNext();
		dbChar.GetRec("reset_time", resetTime);

		time_t t = resetTime;

		struct tm oldresettm;
		memcpy(&oldresettm, localtime(&t), sizeof(oldresettm));

		if (nowtm.tm_mday != oldresettm.tm_mday)
		{
			resetTime = (int)mktime(&nowtm);			

			std::string query = boost::str(boost::format(
				"INSERT INTO t_premiumchar_reset_jumpcount VALUES(FROM_UNIXTIME(%1%))") % resetTime);
			dbChar.SetQuery(query);
			dbChar.Update();
		}
	}

	gserver.m_premiumchar_reset_jump_count_time = resetTime;
#endif
}

void timerPerMidnight::operate( rnSocketIOService* service )
{
#ifdef PREMIUM_CHAR
	gserver.m_premiumchar_reset_jump_count_time = (int)time(0);

	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);
	std::string query = boost::str(boost::format(
		"INSERT INTO t_premiumchar_reset_jumpcount VALUES(FROM_UNIXTIME(%1%))") % gserver.m_premiumchar_reset_jump_count_time);
	dbChar.SetQuery(query);
	dbChar.Update();

	{
		// 모든 게임서버에게 reset 패킷 전송
		CNetMsg::SP rmsg(new CNetMsg);
		ServerToServerPacket::premiumCharResetJumpCount* packet = reinterpret_cast<ServerToServerPacket::premiumCharResetJumpCount*>(rmsg->m_buf);
		packet->type = MSG_PREMIUM_CHAR;
		packet->subType = MSG_SUB_PREMIUM_CHAR_RESET_JUMP_COUNT;
		rmsg->setSize(sizeof(ServerToServerPacket::premiumCharResetJumpCount));
		gserver.SendToAllGameServer(rmsg);
	}
#endif
}

timerPerMidnight* timerPerMidnight::instance()
{
	static timerPerMidnight __instance;
	return &__instance;
}
