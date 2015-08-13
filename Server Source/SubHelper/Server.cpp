#define __SERVER_CPP__

#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Log.h"
#include "Server.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"
#include "doFunc.h"
#include "../ShareLib/packetType/ptype_server_to_server.h"

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

	mysql_init(&m_dbchar);
	mysql_init(&m_dbData);

	m_tradeAgentFee = 5;
	m_tradeAgentDeposit = 1000;
}

CServer::~CServer()
{
	if (m_serverpath)
		delete[] m_serverpath;
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
	ServerSrandom((unsigned long)time(0));

	GAMELOG << init("SYSTEM") << "Finding player limit." << end;
	m_maxplayers = GetMaxPlayers();

	GAMELOG << init("SYSTEM") << "Entering game loop." << end;

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
		return false;

	if (!mysql_real_connect (
				&m_dbData,
				m_config.Find("Data DB", "IP"),
				m_config.Find("Data DB", "User"),
				m_config.Find("Data DB", "Password"),
				m_config.Find("Data DB", "DBName"),
				0, NULL, 0))
		return false;

	if (gserver.m_RankingSystem.TestConnectDB() == false)
		return false;

	return true;
}

void CServer::DisconnectDB()
{
	mysql_close (&m_dbchar);
	mysql_close(&m_dbData);
}

bool CServer::LoadSettings()
{
	GAMELOG << init("SYSTEM")
			<< "Load settings...."
			<< end;

	// LOADING
	CDBCmd cmd;
	cmd.Init(&m_dbchar);

	// LOADING
	GAMELOG << init("SYSTEM")
			<< "Load TradeAgent...."
			<< end;

	//dataDB 의 item 정보를 거래대행 보조 테이블에 복사...아이템의 옵션이 바뀌어도 검색이 원활하게 되기 위해서
	gserver.CopyToTradeAgentItemInfo();

	return true;
}

void CServer::SendToAllGameServer(CNetMsg::SP& msg)
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

void CServer::operate( rnSocketIOService* service )
{
	// 서버들이 accpet 되는곳
	CDescriptor* newd = new CDescriptor(service, true);
	service->SetUserData((void *)newd);

	STATE(newd) = CON_GET_LOGIN;
	ADD_TO_BILIST(newd, m_desclist, m_pPrev, m_pNext);
}

bool CServer::CopyToTradeAgentItemInfo()
{
	std::string tempStr;
	tempStr.resize(1024);
	tempStr = "select a_index, a_type_idx, a_subtype_idx, ";

#ifdef LC_KOR
	tempStr += "a_name as a_name";
#endif
#ifdef LC_GER
	tempStr += "a_name_ger as a_name";
#endif
#ifdef LC_FRC
	tempStr += "a_name_frc as a_name";
#endif
#ifdef LC_SPN
	tempStr += "a_name_spn2 as a_name";
#endif
#ifdef LC_PLD
	tempStr += "a_name_pld as a_name";
#endif
#ifdef LC_USA
	tempStr += "a_name_usa as a_name";
#endif
#ifdef LC_RUS
	tempStr += "a_name_rus as a_name";
#endif
#ifdef LC_TLD
	tempStr += "a_name_thai as a_name";
#endif
#ifdef LC_MEX
	tempStr += "a_name_mex as a_name";
#endif
#ifdef LC_BRZ
	tempStr += "a_name_brz as a_name";
#endif
#ifdef LC_ITA
	tempStr += "a_name_ita as a_name";
#endif
#ifdef LC_UK
	tempStr += "a_name_uk as a_name";
#endif
	tempStr += ", a_flag, a_level, a_job_flag, a_num_0, a_num_1 from t_item";

	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbData );
	cmd.SetQuery(tempStr);

	if( cmd.Open() == false || cmd.MoveFirst() == false )
	{
		return false;
	}

	CopyItem item;
	std::vector<CopyItem> itemList;
	std::vector<CopyItem>::iterator it;
	std::vector<CopyItem>::iterator it_end;
	itemList.reserve(cmd.m_nrecords);

	do
	{
		cmd.GetRec("a_index", item.a_index);
		cmd.GetRec("a_type_idx", item.a_type_idx);
		cmd.GetRec("a_subtype_idx", item.a_subtype_idx);
		cmd.GetRec("a_name", item.a_name);
		cmd.GetRec("a_flag", item.a_flag);
		cmd.GetRec("a_level", item.a_level);
		cmd.GetRec("a_job_flag", item.a_job_flag);
		cmd.GetRec("a_num_0", item.a_num0);
		cmd.GetRec("a_num_1", item.a_num1);
		itemList.push_back(item);
	}
	while ( cmd.MoveNext() );

	tempStr = "delete from t_tradeagent_iteminfo";
	cmd.Init( &gserver.m_dbchar );
	cmd.SetQuery(tempStr);

	if( cmd.Update() == false )
	{
		return false;
	}

	it = itemList.begin();
	it_end = itemList.end();

	char tname[256];
	for(; it != it_end; it++)
	{
		mysql_real_escape_string(&gserver.m_dbchar, tname, it->a_name.c_str(), it->a_name.length());

		tempStr = boost::str(boost::format(
								 "insert into t_tradeagent_iteminfo values( %d, %d, '%s', %d, %d, %d, %d, %d, %d )")
							 % it->a_index % it->a_flag % tname % it->a_level
							 % it->a_type_idx % it->a_subtype_idx % it->a_job_flag % it->a_num0 % it->a_num1);
		cmd.SetQuery(tempStr);

		if( cmd.Update() == false )
		{
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

CServerTimer* CServerTimer::instance()
{
	static CServerTimer __instance;
	return &__instance;
}

void CServerTimer::operate( rnSocketIOService* service )
{
	gserver.HeartBeat();
}

//////////////////////////////////////////////////////////////////////////

CServerTimerPerMinute* CServerTimerPerMinute::instance()
{
	static CServerTimerPerMinute __instance;
	return &__instance;
}

void CServerTimerPerMinute::Init()
{
	time_t nowtime = time(0);
	int remain_sec = 60 - ((int)nowtime % 60);

	bnf::instance()->CreateSecTimerPeriod(remain_sec, 60, this);
}

void CServerTimerPerMinute::operate( rnSocketIOService* service )
{
	mysql_ping(&gserver.m_dbchar);

	mysql_ping(&gserver.m_dbData);

	// 매 분마다 판매대행중 반품이 있는지 검사함, 한번에 검사하는 갯수는 한정함
	checkExpireItemInTradeAgent();
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

void CServerTimerPerHour::Init()
{
	time_t nowtime = time(0);
	int remain_sec = 3600 - ((int)nowtime % 3600);

	bnf::instance()->CreateSecTimerPeriod(remain_sec, 3600, this);
}

void CServerTimerPerHour::operate( rnSocketIOService* service )
{
	gserver.m_RankingSystem.CheckRankingRefreshTime();
}

CServerTimerPerHour* CServerTimerPerHour::instance()
{
	static CServerTimerPerHour __instance;
	return &__instance;
}
