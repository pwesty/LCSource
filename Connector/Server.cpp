#define __SERVER_CPP__

#include <string>
#include <vector>
#include <boost/format.hpp>

#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Log.h"
#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "ProcConnMsg.h"
#include "CmdMsg.h"
#include "../ShareLib/PrintExcuteInfo.h"

CServer gserver;

CServer::CServer()
{
	m_desclist = NULL;

	m_hardcore_flag_in_connector = 0;

	m_serverpath = GetServerPath();
	m_maxplayers = 0;
	m_maxSubServer = 0;
	m_bshutdown = false;

	struct tm m_now = NOW();
	m_now.tm_min = m_now.tm_min - (m_now.tm_min % PULSE_SAVEMAXCOUNT_NEW);
	m_now.tm_min += PULSE_SAVEMAXCOUNT_NEW;
	m_now.tm_sec = 0;
	m_now.tm_isdst = -1;
	m_pulseSaveMax = mktime(&m_now);

	m_now = NOW();
	m_now.tm_min = m_now.tm_min - (m_now.tm_min % PULSE_SAVECOUNT_NEW);
	m_now.tm_min += PULSE_SAVECOUNT_NEW;
	m_now.tm_sec = 0;
	m_now.tm_isdst = -1;
	m_pulseSaveCount = mktime(&m_now);

	mysql_init(&m_dbuser);
	mysql_init(&m_dbauth);

	m_changeLimit = false;

	m_nEventGomdori2007LastUpdate = -1;
	m_nEventGomdori2007LastRead = -1;
	m_nEventGomdori2007Count = 0;
	m_nEventGomdori2007Status = NULL;
	m_nEventGomdori2007StatusCurServer = 0;

	m_bEventXmasTimerStart = false;
	m_nEventXmasTreeOldTime = 0;
	m_nEventXmasTreeNoticeTime = 0;
	m_nEventXmasTreeLastUpdate = -1;
	m_nEventXmasTreeLastRead = -1;
	m_unEventXmasTreePoint = 0;
}

void CServer::CommandInterpreter(CDescriptor* d, CNetMsg::SP& msg)
{
	msg->MoveFirst();
	switch (msg->m_mtype)
	{
	case MSG_CONN_REQ:
		{
			unsigned char subtype;
			RefMsg(msg) >> subtype;
			switch (subtype)
			{
			case MSG_CONN_EVENT_XMAS_2007 :
				ConnEventXmas2007( d, msg );
				break;

			case MSG_CONN_EVENT_GOMDORI_2007:
				ConnEventGomdori2007(d, msg);
				break;

			case MSG_CONN_GPARA_PROMOTION:
				break;

			case MSG_CONN_OCN_GOO_PROMOTION:
				break;

			case MSG_CONN_MSN_PROMOTION:
				break;

			case MSG_CONN_CREATEEVENT:
				break;

			case MSG_CONN_CASHITEM_GIFT:
				ConnCashItemGift(d, msg);
				break;
			case MSG_CONN_CASHITEM_GIFT_SENDHISTORY:
				ConnCashItemSendHistory(d, msg);
				break;
			case MSG_CONN_CASHITEM_GIFT_RECVHISTORY:
				ConnCashItemRecvHistory(d, msg);
				break;
			case MSG_CONN_CASHITEM_GIFT_RECVLIST:
				ConnCashItemRecvList(d, msg);
				break;
			case MSG_CONN_CASHITEM_GIFT_RECV:
				ConnCashItemRecv(d, msg);
				break;

			case MSG_CONN_LIMIT_CATALOG:
				ConnSetLimit(d, msg);
				break;

#ifdef EVENT_PACKAGE_ITEM
			case MSG_CONN_COUPON_USE:
				ConnCouponUse(d, msg);
				break;
			case MSG_CONN_COUPON_CONFIRM:
				ConnCouponConfirm(d, msg);
				break;
#endif

			case MSG_CONN_RANKER_REQ:
				ConnRankerReq(d, msg);
				break;
			case MSG_CONN_EVENT_2PAN4PAN_MONEY_REQ:
				ConnEvent2pan4panMoneyReq(d, msg);
				break;

			case MSG_CONN_EVENT_2PAN4PAN_BOX_REQ:
				ConnEvent2pan4panBoxReq(d, msg);
				break;
			case MSG_CONN_EVENT_2PAN4PAN_BOX_COMMIT:
				ConnEvent2pan4panBoxCommit(d, msg);
				break;

			case MSG_CONN_EVENT_2PAN4PAN_GOODS_REQ:
				ConnEvent2pan4panGoodsReq(d, msg);
				break;
			case MSG_CONN_EVENT_2PAN4PAN_GOODS_COMMIT:
				ConnEvent2pan4panGoodsCommit(d, msg);
				break;

			//case MSG_CONN_EVENT_2PAN4PAN_LETTER_REQ:
			//	ConnEvent2pan4panLetterReq(d, msg);
			//	break;
			//case MSG_CONN_EVENT_2PAN4PAN_LETTER_ROLLBACK_REQ:
			//	ConnEvent2pan4panLetterRollbackReq(d, msg);
			//	break;

			case MSG_CONN_CASHITEM_PURCHASEHISTORY_REQ:
				ConnCashItemPurchaseHistoryReq(d, msg);
				break;
			case MSG_CONN_CASHITEM_PURCHASELIST_REQ:
				ConnCashItemPurchaselistReq(d, msg);
				break;
			case MSG_CONN_CASHITEM_BRING_REQ:
				ConnCashItemBringReq(d, msg);
				break;
			case MSG_CONN_CASHITEM_BALANCE_REQ:
				ConnCashItemBalanceReq(d, msg);
				break;
			case MSG_CONN_CASHITEM_PURCHASE_REQ:
				ConnCashItemPurchaseReq(d, msg);
				break;
			case XXX_MSG_CONN_STASH_SET_NEW_PASSWORD:
				break;
			case XXX_MSG_CONN_STASH_DELETE_PASSWORD:
				break;

			case XXX_MSG_CONN_STASH_SEAL:
				break;

			case XXX_MSG_CONN_STASH_CHANGE_PASSWORD:
				break;

			case XXX_MSG_CONN_STASH_CHECK_PASSWORD:
				break;

			case XXX_MSG_CONN_STASH_STATE:
				break;

			case MSG_CONN_LEVELUP_POINT:
				ConnLevelupPoint(d, msg);
				break;

			case MSG_CONN_PREOPEN_GIFT:
				ConnPreopenGift(d, msg);
				break;

			case MSG_CONN_LOGIN_REQ:
				ConnLogin(d, msg);
				break;

			case MSG_CONN_LOGOUT_REQ:
				ConnLogout(d, msg);
				break;

			case MSG_CONN_BLOGIN_REQ:
				ConnBLogin(d, msg);
				break;

			case MSG_CONN_BLOGOUT_REQ:
				ConnBLogout(d, msg);
				break;

			case MSG_CONN_PLAYER_REQ:
				ConnPlayerReq(d, msg);
				break;

			case MSG_CONN_PLAYING:
				ConnPlaying(d, msg);
				break;

			case MSG_CONN_REBOOT_REQ:		//XX endgame - 4 end
				PrintExcuteInfo::Instance()->SetStopMessage("reboot");
				bnf::instance()->Stop();
				m_bshutdown = true;
				break;

#ifdef GMTOOL
			case MSG_CONN_GMTOOL_GOMMAND:
				ConnGMToolCommand( d, msg );
				break;
			case MSG_CONN_GMTOOL_CHATTING:
				ConnGMToolChatting( d, msg );
				break;
			case MSG_CONN_GMTOOL_CHATMONITOR:
				ConnGMToolChatMonitor( d, msg );
				break;
#endif // GMTOOL

#ifdef LACARETTE_SYSTEM
			case MSG_CONN_LACARETTE_SYSTEM:
				ConnLacaretteSystemMsg(d, msg);
				break;
#endif

#ifdef DEV_EVENT_PROMOTION2
			case MSG_CONN_EVENT_PROMOTION2:
				ConnEventPromotion2(d,msg);
				break;
#endif

			case MSG_CONN_WISHLIST:
				ConnWishListMsg(d, msg);
				break;

#ifdef EVENT_USER_COMEBACK
			case MSG_CONN_EVENT_USER_COMEBACK:
				ConnEventUserComeback(d, msg);
				break;
#endif

#ifdef EVENT_EUR2_PROMOTION
			case MSG_CONN_EVENT_EUR2_PROMOTION:
				ConnEventEUR2Promotion(d, msg);
				break;
#endif
			case MSG_CONN_EVENT_TABLE_INIT:
				ConnEventTableInit(d, msg);
				break;
			default:
				goto ERROR_INVALID_COMMAND;
			}
		}
		break;

	default:
		goto ERROR_INVALID_COMMAND;
	}

	return ;

ERROR_INVALID_COMMAND:
	GAMELOG << init("SYS_ERR") << "Invalid Command: " << msg->m_mtype << "(" << msg->m_buf[1] << ")" << end;
}

void CServer::ProcessLogout(CUser* user)
{
	if (user == NULL)
		return ;

	int subno = user->m_subnum;

	GAMELOG << init("LOGOUT", user->m_name)
			<< end;

	// 존별 사용자 수 감소
	m_userList[subno - 1].m_playersPerZone[user->m_zone]--;

	// 빌링에 로그아웃 알리기
	if (m_billing.IsRunning())
		m_billing.Logout(user->m_name);

	// DB에 저장
	int playtime = time(0) - user->m_timestamp;

	if (WriteDB(user->m_index, user->m_server, subno, -1, playtime, NULL, NULL, false) != MSG_CONN_ERROR_SUCCESS)
		GAMELOG << init("SYS_ERR", user->m_name) << "Cannot Update DB for Logout" << end;

#ifdef EVENT_PCBANG_2ND
	if(user->m_location == BILL_LOCATION_PCBANG)
	{
		// 피씨방 로그 아웃 기록 남기기
		CDBCmd cmd;
		cmd.Init(&gserver.m_dbauth);

		std::string insertQuery = boost::str(boost::format(
				"iNSERT INTO t_pcbang_log VALUES (%d, '%s', %d, now())")
											 % user->m_index % (const char *)user->m_ip % PCBANG_LOGOUT);
		cmd.SetQuery(insertQuery);
		cmd.Update();
	}
#endif // EVENT_PCBANG_2ND

#ifdef LOGIN_TIME_CHECK
	int restart = 0;
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	char select_users_sql[MAX_STRING_LENGTH] = {0,};
	sprintf(select_users_sql, "SELECT a_restart FROM t_users WHERE a_portal_index=%d", user->m_index);
	cmd.SetQuery(select_users_sql);
	if(cmd.Open() && cmd.MoveFirst())
	{
		if(!cmd.GetRec("a_restart", restart))
			restart = 0;
	}

	char update_users_query[MAX_STRING_LENGTH] = {0,};

	if(restart > 0)
	{
		sprintf(update_users_query, "UPDATE t_users SET a_login_time=UNIX_TIMESTAMP(NOW()), a_restart=0 WHERE a_portal_index=%d", user->m_index);
	}
	else
	{
		sprintf(update_users_query, "UPDATE t_users SET a_login_time=0, a_restart=0 WHERE a_portal_index=%d", user->m_index);
	}

	cmd.SetQuery(update_users_query);
	cmd.Update();

#endif // LOGIN_TIME_CHECK

	// 유저를 리스트에서 삭제
	m_userList[subno - 1].Remove(user);
}

void ServerSrandom(unsigned long initial_seed);

CServer::~CServer()
{
	if (m_serverpath)
		delete[] m_serverpath;

	if (m_nEventGomdori2007Status)
		delete [] m_nEventGomdori2007Status;
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
	puts("Loading Setting File...");

	CLCString strTemp(1024);

	strTemp.Format("%s%s", m_serverpath, "newStobm.bin");

	if (!m_config.Load(strTemp))
		return false;

	m_serverno = atoi(m_config.Find("Server", "Number"));
	m_maxSubServer = atoi(m_config.Find("Server", "MaxSubServer"));
	if (m_maxSubServer < 1)
		return false;

#ifdef PASSWORD_ENCRYPT_SHA256
	strcpy(m_salt, m_config.Find("SHA256","Salt"));
	if(strlen(m_salt) < 1)
		return false;
#endif // PASSWORD_ENCRYPT_SHA256

#ifdef HARDCORE_SERVER
	if (strcmp(m_config.Find("Server", "HARDCORE"), "TRUE") == 0)
	{
		m_hardcore_flag_in_connector = 1;
	}
#endif

	return true;
}

bool CServer::ConnectDB()
{
	if (!mysql_real_connect(
				&m_dbuser,
				m_config.Find("User DB", "IP"),
				m_config.Find("User DB", "User"),
				m_config.Find("User DB", "Password"),
				m_config.Find("User DB", "DBName"),
				0, NULL, 0))
	{
		LOG_ERROR("can't connect to db : ip[%s] user[%s] password[%s] dbname[%s] error[%s]",
				  m_config.Find("User DB", "IP"),
				  m_config.Find("User DB", "User"),
				  m_config.Find("User DB", "Password"),
				  m_config.Find("User DB", "DBName"),
				  mysql_error(&m_dbuser)
				 );
		return false;
	}

	if (!mysql_real_connect(
				&m_dbauth,
				m_config.Find("Auth DB", "IP"),
				m_config.Find("Auth DB", "User"),
				m_config.Find("Auth DB", "Password"),
				m_config.Find("Auth DB", "DBName"),
				0, NULL, 0))
	{
		LOG_ERROR("can't connect to db : ip[%s] user[%s] password[%s] dbname[%s] error[%s]",
				  m_config.Find("Auth DB", "IP"),
				  m_config.Find("Auth DB", "User"),
				  m_config.Find("Auth DB", "Password"),
				  m_config.Find("Auth DB", "DBName"),
				  mysql_error(&m_dbauth)
				 );
		return false;
	}

	CDBCmd cmdUser;
	cmdUser.Init(&m_dbuser);

	std::string selectQuery = boost::str(boost::format(
			"SELECT * FROM t_event_gomdori_count WHERE a_server=%d") % gserver.m_serverno);
	cmdUser.SetQuery(selectQuery);
	if (cmdUser.Open())
	{
		struct tm tmCur = NOW();
		gserver.m_nEventGomdori2007LastUpdate = tmCur.tm_min - tmCur.tm_min % 5;
		if (cmdUser.MoveFirst())
			cmdUser.GetRec("a_count", gserver.m_nEventGomdori2007StatusCurServer);
		else
		{
			std::string insertQuery = boost::str(boost::format(
					"INSERT INTO t_event_gomdori_count (a_server, a_updatedate) VALUES (%d, NOW())") % gserver.m_serverno);
			cmdUser.SetQuery(insertQuery);
			cmdUser.Update();
		}
	}
	else
	{
		GAMELOG << init("ERROR : EVENT GOMDORI 2007 TABLE NOT FOUND: t_event_gomdori_count") << end;
		return false;
	}

	std::string selectXMasTreeQuery = boost::str(boost::format(
										  "SELECT * FROM t_event_xmastree_point WHERE a_server=%d") % gserver.m_serverno);

	cmdUser.SetQuery(selectXMasTreeQuery);
	if ( cmdUser.Open() )
	{
		struct tm tmCur = NOW();
		gserver.m_nEventXmasTreeLastUpdate = tmCur.tm_min - tmCur.tm_min % 5;
		if (cmdUser.MoveFirst())
			cmdUser.GetRec("a_point", gserver.m_unEventXmasTreePoint );
		else
		{
			std::string insertXMasTreeQuery = boost::str(boost::format(
												  "INSERT INTO t_event_xmastree_point (a_server, a_lastupdate) VALUES (%d, NOW())") % gserver.m_serverno);
			cmdUser.SetQuery(insertXMasTreeQuery);
			cmdUser.Update();
		}
	}
	else
	{
		GAMELOG << init("ERROR : EVENT XMAS 2007 TABLE NOT FOUND: t_event_xmastree_point") << end;
		return false;
	}

	return true;
}

bool CServer::InitGame()
{
	m_userList = new CUserList[m_maxSubServer];

	ServerSrandom((unsigned long)time(0));
	InitUserData();

	GAMELOG << init("SYSTEM") << "Finding Connection Limit" << end;
	m_maxplayers = GetMaxPlayers();

	GAMELOG << init("SYSTEM") << "Initialize Server Socket" << end;

	CDBCmd cmd;
	int i;

	i = 0;
	cmd.Init(&gserver.m_dbuser);

	// 팔린 상품 갯수 로딩
	std::string selectQuery = boost::str(boost::format(
			"SELECT * FROM t_ct_limit WHERE a_server = %d ") % this->m_serverno);

	cmd.SetQuery(selectQuery);
	if( !cmd.Open() )
	{
		GAMELOG << init("SYSTEM_ERROR") << "Cannot find t_ct_limit" << end;
		return false;
	}

	CLimitSell* sellList;
	int ctid, sell;
	while( cmd.MoveNext() )
	{
		if( !cmd.GetRec("a_ct_id", ctid)
				|| !cmd.GetRec("a_sell", sell) )
		{
			GAMELOG << init("SYSTEM_ERROR") << "t_ct_limit RECORD ERROR CANNOT GET" << end;
			return false;
		}

		sellList = new CLimitSell(ctid, sell, false);

		gserver.m_limitSell.insert(map_limitsell_t::value_type(ctid, sellList));
	}

	GAMELOG << init("SYSTEM") << "Entering Connector Loop" << end;

	return true;
}

void CServer::InitUserData()
{
	// t_user의 연결 초기화
	// UPDATE t_users SET a_zone_num=-1 WHERE a_server_num=serverno AND a_subnum=subnum
	std::string updateQuery = boost::str(boost::format(
			"UPDATE t_users SET a_zone_num=-1 WHERE a_server_num = %d") % m_serverno);

	CDBCmd cmd;
	cmd.Init(&m_dbuser);
	cmd.SetQuery(updateQuery);
	cmd.Update();

#ifdef LOGIN_TIME_CHECK
	std::string updateQuery1 = boost::str(boost::format(
			"UPDATE t_users SET a_login_time=0, a_restart=0 WHERE a_server_num= %d") % m_serverno);
	cmd.Init(&m_dbuser);
	cmd.SetQuery(updateQuery1);
	cmd.Update();
#endif // LOGIN_TIME_CHECK
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
			GAMELOG << init("SYS_ERR") << "Calling getrlimit()" << end;
			exit(1);
		}

		/* set the current to the maximum */
		limit.rlim_cur = limit.rlim_max;
		if (setrlimit(RLIMIT_NOFILE, &limit) < 0)
		{
			GAMELOG << init("SYS_ERR") << "Calling setrlimit()" << end;
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
			GAMELOG << init("SYS_ERR") << "Calling sysconf()" << end;
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
		GAMELOG << init("SYS_ERR") << "Non-positive max player limit!  (Set at" << max_descs << "using" << method << ")." << end;
		exit(1);
	}

	GAMELOG << init("SYSTEM") << "Setting player limit to" << max_descs << "using" << method << "." << end;

	return (max_descs);
#endif /* CIRCLE_UNIX */
}

void CServer::CloseSocket(CDescriptor* d)
{
	// 로그 남기고 사용자 정리
	GAMELOG << init("DISCONNECT", d->service_->ip().c_str())
			<< end;

	// 사용자 루프 돌면서
	if (!d->m_bLoginServer && d->m_subno > 0)
	{
		int i;
		for (i = 0; i <= m_userList[d->m_subno - 1].m_last; i++)
		{
			// 현재 접속 종료된 서버의 사용자이면
			if (m_userList[d->m_subno - 1].m_users[i] && m_userList[d->m_subno - 1].m_users[i]->m_descserver == d)
			{
				// 로그아웃 처리
				CNetMsg::SP rmsg(new CNetMsg);
				rmsg->Init(MSG_CONN_REQ);
				RefMsg(rmsg) << (unsigned char)MSG_CONN_LOGOUT_REQ
							 << m_userList[d->m_subno - 1].m_users[i]->m_name;
				ConnLogout(d, rmsg);
			}
		}
	}

	REMOVE_FROM_BILIST(d, m_desclist, m_pPrev, m_pNext);
}

void CServer::SavePlayersCount()
{
	time_t now;
	time(&now);

	if (m_pulseSaveCount > now && m_pulseSaveMax > now)
		return ;

	CDBCmd cmd;
	int i;
	int s;

	cmd.Init(&m_dbuser);

	// 존별 사용자 수 저장
	for (s = 0; s < m_maxSubServer; s++)
	{
		// 팔릿 갯수에 벼화가 있으면 저장
		if(gserver.m_changeLimit)
		{
			// 한정 판매 팔린 수량도 5분마다 일단 저장
			// 한정 상품이 팔린 갯수를 디비에 저장
			map_limitsell_t::iterator it = gserver.m_limitSell.begin();
			map_limitsell_t::iterator endit = gserver.m_limitSell.end();
			for(; it != endit; ++it)
			{
				CLimitSell* limit = it->second;
				if( limit->IsChange() == false)
					continue;

				std::string updateQuery = boost::str(boost::format(
						"UPDATE t_ct_limit SET a_sell = %d WHERE a_server = %d and a_ct_id = %d ") % limit->GetSell() % gserver.m_serverno % limit->GetIndex() );

				cmd.SetQuery(updateQuery);
				// 업데이트가 실패하면 현재 등록안된 상품이니까 인서트
				if( !cmd.Update() || cmd.m_nrecords < 1)
				{
					std::string insertQuery = boost::str(boost::format(
							"INSERT INTO t_ct_limit (a_server, a_ct_id, a_sell ) VALUES ( %d, %d, %d) ") % gserver.m_serverno % limit->GetIndex() % limit->GetSell() );
					cmd.SetQuery(insertQuery);
					cmd.Update();
				}
			}

			gserver.m_changeLimit = false;
			GAMELOG << init("SYSTEM")
					<< "t_ct_limt UPDATED" << end;
		}

		for (i = 0; i < MAX_ZONES; i++)
		{
			if (m_userList[s].m_playersPerZone[i] < 0)
				continue ;

			// t_connect_count 갱신: 10분
			if (m_pulseSaveCount <= now)
			{
				// INSERT INTO t_connect_count (a_count, a_date, a_server, a_sub_num, a_zone_num) VALUES (count, date, server, subno, zoneno)
				std::string insertSql = boost::str(boost::format(
													   "INSERT INTO t_connect_count (a_count, a_date, a_server, a_sub_num, a_zone_num) VALUES "
													   "(%d, FROM_UNIXTIME(%d), %d, %d, %d)") % m_userList[s].m_playersPerZone[i] % (int) m_pulseSaveCount % m_serverno % (s + 1) % i);
				cmd.SetQuery(insertSql);
				cmd.Update();

				GAMELOG << init("COUNT") << "Sub" << delim << (s + 1) << delim << "Zone" << delim << i << delim << m_userList[s].m_playersPerZone[i] << end;
			}

			// t_connect_max 갱신: 2분
			if (m_pulseSaveMax <= now)
			{
				// 오늘 날짜로 찾고
				// SELECT * FROM t_connect_max WHERE a_date=CURDATE() AND a_server=server AND a_sub_num=subno AND a_zone_num=zone
				std::string selectQuery = boost::str(boost::format(
						"SELECT * FROM t_connect_max WHERE a_date=CURDATE() AND a_server = %d AND a_sub_num = %d AND a_zone_num = %d")
													 % m_serverno % (s + 1) % i);

				cmd.SetQuery(selectQuery);

				if (cmd.Open())
				{
					if (cmd.MoveNext())
					{
						// 있으면 카운트 비교해서 큰수 저장하고
						LONGLONG index;
						int countmax = 0;
						cmd.GetRec("a_index", index);
						cmd.GetRec("a_count_max", countmax);
						if (countmax < m_userList[s].m_playersPerZone[i])
							countmax = m_userList[s].m_playersPerZone[i];
						// 업데이트
						// UPDATE t_connect_max SET a_count=count, a_count_max=countmax WHERE a_index=index
						std::string updateQuery = boost::str(boost::format(
								"UPDATE t_connect_max SET a_count = %d, a_count_max = %d WHERE a_index = %d")
															 % m_userList[s].m_playersPerZone[i] % countmax % index);

						cmd.SetQuery(updateQuery);
						cmd.Update();
					}
					else
					{
						// 없으면 추가
						// INSERT INTO t_connect_max (a_count, a_count_max, a_date, a_server, a_sub_num, a_zone_num) VALUES (count, countmax, CURDATE(), server, subno, zone)
						std::string insertQuery = boost::str(boost::format(
								"INSERT INTO t_connect_max (a_count, a_count_max, a_date, a_server, a_sub_num, a_zone_num) VALUES (%d, %d, CURDATE(), %d, %d, %d) ")
															 % m_userList[s].m_playersPerZone[i] % m_userList[s].m_playersPerZone[i] % m_serverno % (s + 1) % i);

						cmd.SetQuery(insertQuery);
						cmd.Update();
					}
				} // if (cmd.Open())
			} // if (m_pulseSaveMax < 1)
		} // for (i = 0; i < MAX_ZONES; i++)
	}

	struct tm *saveCount;
	if (m_pulseSaveCount <= now)
	{
		saveCount = localtime(&m_pulseSaveCount);
		if( saveCount )
		{
			saveCount->tm_min += PULSE_SAVECOUNT_NEW;
			saveCount->tm_isdst = -1;
			m_pulseSaveCount = mktime(saveCount);
		}
	}
	if (m_pulseSaveMax <= now)
	{
		saveCount = localtime(&m_pulseSaveMax);
		if( saveCount )
		{
			saveCount->tm_min += PULSE_SAVEMAXCOUNT_NEW;
			saveCount->tm_isdst = -1;
			m_pulseSaveMax = mktime(saveCount);
		}
	}
}

void CServer::SendAllGameServer(CNetMsg::SP& msg)
{
	CDescriptor* desc = gserver.m_desclist;
	while (desc)
	{
		if( desc->m_subno >= 1 && desc->m_subno <= gserver.m_maxSubServer )
			SEND_Q(msg, desc);
		desc = desc->m_pNext;
	}
}

void CServer::TimerToGameServer()
{
	// 시간에 따라 공지 날리기
	struct tm currentTime = NOW();
	int nXMasTreeTimeDiff;
	if( m_nEventXmasTreeNoticeTime > 0 )
	{
		// 5분 타이머 체크 시작
		if( !m_bEventXmasTimerStart && m_nEventXmasTreeNoticeTime%6 == 5 )
		{
			// 5분 남음 공지
			CNetMsg::SP rmsg(new CNetMsg);
			ConnEventXmas2007Msg( rmsg , MSG_CONN_EVENT_XMASTREE_ITEMDROP) ;
			RefMsg(rmsg) << (int) 5;		// 5분 남았음 MSG
			gserver.SendAllGameServer(rmsg);
			m_bEventXmasTimerStart = true;

			GAMELOG << init("5 Min Notice XMAS EVENT ITEM : Remain Time " ) <<  m_nEventXmasTreeNoticeTime << end;
		}

		if( m_bEventXmasTimerStart && m_nEventXmasTreeNoticeTime%6 == 1 )
		{
			// ITEM DROP
			CNetMsg::SP rmsg(new CNetMsg);
			ConnEventXmas2007Msg( rmsg , MSG_CONN_EVENT_XMASTREE_ITEMDROP) ;
			RefMsg(rmsg) << (int) 0;		// 0분 남았음 MSG ITEM DROP
			gserver.SendAllGameServer(rmsg);

			GAMELOG << init("DROP XMAS EVENT ITEM : Remain Time " ) <<  m_nEventXmasTreeNoticeTime << end;

			if( m_nEventXmasTreeNoticeTime == 1 )
			{
				m_nEventXmasTreeNoticeTime =0;
			}
			m_bEventXmasTimerStart = false;
		}

		nXMasTreeTimeDiff = currentTime.tm_min - m_nEventXmasTreeOldTime ;
		if (nXMasTreeTimeDiff < 0)
			nXMasTreeTimeDiff += 60;
		m_nEventXmasTreeNoticeTime -= nXMasTreeTimeDiff;
	}
	m_nEventXmasTreeOldTime = currentTime.tm_min;
}

void CServer::operate( rnSocketIOService* service )
{
	CDescriptor* newd = new CDescriptor(service);
	service->SetUserData((void *)newd);

	STATE(newd) = CON_GET_LOGIN;
	ADD_TO_BILIST(newd, m_desclist, m_pPrev, m_pNext);
}

//////////////////////////////////////////////////////////////////////////
void ServerAliveTimer::operate( rnSocketIOService* service )
{
#if !defined (USE_TENTER_BILLING)
	if (gserver.m_billing.IsRunning())
	{
		// 빌링서버에 얼라이브 패킷을 보낸다.
		CBPacket::SP pack(new CBPacket);
		pack->Init(MSG_BILLITEM_SERVERLIVE);
		gserver.m_billing.WriteToOutput(pack);
	}
#endif
}

ServerAliveTimer* ServerAliveTimer::instance()
{
	static ServerAliveTimer __instance;
	return &__instance;
}

//////////////////////////////////////////////////////////////////////////

void HeartBeatTimer::operate( rnSocketIOService* service )
{
	gserver.HeartBeat();
}

HeartBeatTimer* HeartBeatTimer::instance()
{
	static HeartBeatTimer __instance;
	return &__instance;
}

//////////////////////////////////////////////////////////////////////////