#define __LOGIN_SERVER_CPP__

#include <string>
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Log.h"
#include "Server.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"
#include "../ShareLib/PrintExcuteInfo.h"

CServer gserver;

void ServerSrandom(unsigned long initial_seed);

//////////////////////////////////////////////////////////////////////////
// 1초당
void CServerTimer::operate( rnSocketIOService* service )
{
	static int _local_sec = 0;

	gserver.m_pulse += 20;

	// 매 10초마다 접속된 모든 Connector server에게 현재 사용자 정보를 요구한다.
	if (((++_local_sec) % 10) == 0)
	{
		_local_sec = 0;

		CNetMsg::SP rmsg(new CNetMsg);
		PlayerReqMsg(rmsg);

		for (int i = 0; i < gserver.m_nConnector; i++)
		{
			if (gserver.m_connector[i]->isRun() == false)
				continue ;

			SEND_Q(rmsg, gserver.m_connector[i]);
		}
	}
}

CServerTimer* CServerTimer::instance()
{
	static CServerTimer __instance;
	return &__instance;
}

//////////////////////////////////////////////////////////////////////////
// 1분당
void CServerTimerPerMinute::operate( rnSocketIOService* service )
{
	mysql_ping(&gserver.m_dbuser);
//	mysql_ping(&gserver.m_dbdata);
	mysql_ping(&gserver.m_dbAuth);

#ifdef LOCAL_LOGIN_LOG
	mysql_ping(&gserver.m_dbLocalLog);
#endif
}

void CServerTimerPerMinute::Init()
{
	time_t nowtime = time(0);
	int remain_sec = 60 - ((int)nowtime % 60);

	bnf::instance()->CreateSecTimerPeriod(remain_sec, 60, this);
}

CServerTimerPerMinute* CServerTimerPerMinute::instance()
{
	static CServerTimerPerMinute __instance;
	return &__instance;
}
//////////////////////////////////////////////////////////////////////////

CServer::CServer()
{
	m_connector = NULL;

	m_serverpath = GetServerPath();
	m_maxplayers = MAX_PLAYING;
	m_bshutdown = false;

	m_pulse = 0;

	mysql_init(&m_dbuser);
	mysql_init(&m_dbdata);
	mysql_init(&m_dbAuth);
#ifdef LOCAL_LOGIN_LOG
	mysql_init(&m_dbLocalLog);
#endif //LOCAL_LOGIN_LOG

	m_clientversionMin = 0;
	m_clientversionMax = 0;

#ifdef IP_BLOCK_AUTO
	m_autoBlockTable = NULL;
	m_autoBlockPulse = gserver.m_pulse;
#endif

	m_nRecommendServer = -1;

#if defined(LC_USA)
	m_pulseSaveTableCount = 0;
#endif

#if defined (LC_TLD)
	m_national = LC_TLD;
#elif defined (LC_USA)
	m_national = LC_USA;
#elif defined (LC_BRZ_EP)
	m_national = LC_BRZ_EP;
#elif defined (LC_BRZ)
	m_national = LC_BRZ;
#elif defined (LC_GER)
	m_national = LC_GER;
#elif defined (LC_SPN)
	m_national = LC_SPN;
#elif defined (LC_FRC)
	m_national = LC_FRC;
#elif defined (LC_PLD)
	m_national = LC_PLD;
#elif defined (LC_RUS)
	m_national = LC_RUS;
#elif defined (LC_TUR)
	m_national = LC_TUR;
#elif defined (LC_ITA)
	m_national = LC_ITA;
#elif defined (LC_MEX_EP)
	m_national = LC_MEX_EP;
#elif defined (LC_MEX)
	m_national = LC_MEX;
#elif defined (LC_UK)
	m_national = LC_UK;
#else
	m_national = LC_KOR;
#endif

#if defined (BILA_INTERGRATION_SERVER)
	m_intergrationInfo.AddNation(INTERGRATION_BRAZIL);
	m_intergrationInfo.AddNation(INTERGRATION_MEXICO);
#endif
#if defined (EU_INTERGRATION_SERVER)
	m_intergrationInfo.AddNation(INTERGRATION_GER);
	m_intergrationInfo.AddNation(INTERGRATION_SPN);
	m_intergrationInfo.AddNation(INTERGRATION_FRC);
	m_intergrationInfo.AddNation(INTERGRATION_PLD);
	m_intergrationInfo.AddNation(INTERGRATION_TUR);
	m_intergrationInfo.AddNation(INTERGRATION_ITA);
	m_intergrationInfo.AddNation(INTERGRATION_UK);
#endif
	m_bHttps_Auth = true;
}

CServer::~CServer()
{
	if (m_serverpath)
		delete[] m_serverpath;

	delete [] m_connector;
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
	LOG_INFO("Load setting file....");

	CLCString strTemp(1024);

	strTemp.Format("%s%s", m_serverpath, "newStobm.bin");

	if (!m_config.Load(strTemp))
		return false;

	char tmpBuf[100] = {0,};
	strcpy(tmpBuf, m_config.Find("Server", "No"));
	if (strlen(tmpBuf) < 1)
		return false;
	m_subno = atoi(tmpBuf);

	if (strcmp(m_config.Find("Server", "AllowExternalIP"), "TRUE") == 0)
		m_bOnlyLocal = false;
	else
	{
		FILE* fpCheckOpen = fopen(LOGIN_SERVER_OPEN_CHECK_FILENAME, "r+");
		if (fpCheckOpen)
		{
			m_bOnlyLocal = false;
			fclose(fpCheckOpen);
		}
		else
		{
			m_bOnlyLocal = true;
			CheckInternalIP(m_config.Find("Server", "AllowExternalIP"));
		}
	}

#ifdef PASSWORD_ENCRYPT_SHA256
	strcpy(m_salt, m_config.Find("SHA256","Salt"));
	if(strlen(m_salt) < 1)
		return false;
#endif // PASSWORD_ENCRYPT_SHA256

#if defined (INTERGRATION_SERVER)
	if (strcmp(m_config.Find("Server", "IntergrationServer"), "TRUE") == 0)
		m_intergrationInfo.SetEnable(true);
	else
		m_intergrationInfo.SetEnable(false);
#endif
	if (strcmp(m_config.Find("Server", "HttpsAuth"), "FALSE") == 0)
		m_bHttps_Auth = false;
	else
		m_bHttps_Auth = true;

	return true;
}

void CServer::CheckInternalIP(const char* strData)
{
	unsigned int		nIP[8];							// ip 4자리 + 서브넷마스크 4자리
	const char*			pData				= strData;
	unsigned int		nData;
	char				strBuf[256];
	char*				pBuf;
	int					nState				= 0;		// state	: 0 - 아이피 읽기 시작
	//			: 1 - 아이피 둘째 자리
	//			: 2 - 아이피 셋째 자리
	//			: 3 - 아이피 넷째 자리
	//			: 4 - 서브넷 읽기 시작
	//			: 5 - 서브넷 둘째 자리
	//			: 6 - 서브넷 셋째 자리
	//			: 7 - 서브넷 넷째 자리
	//			: 8 - 읽기 완료

	memset(strBuf, 0, sizeof(strBuf));
	pBuf = strBuf;

	while (*pData)
	{
		switch (*pData)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			*pBuf = *pData;
			pBuf++;
			*pBuf = '\0';
			break;

		case '.':
		case '/':
			if (nState != 3 && *pData == '/')
				goto EXIT_FUNC_DEFAULT;
			if (strlen(strBuf) < 1 || strlen(strBuf) > 3)
				goto EXIT_FUNC_DEFAULT;
			nData = (unsigned int)atoi(strBuf);
			if (nData < 0 || nData > 255)
				goto EXIT_FUNC_DEFAULT;
			nIP[nState] = nData;
			nState++;
			memset(strBuf, 0, sizeof(strBuf));
			pBuf = strBuf;
			break;

		default:
			goto EXIT_FUNC_DEFAULT;
		} // switch (*pData)

		pData++;
	} // while (*pData)

	if (strlen(strBuf) < 1 || strlen(strBuf) > 3)
		goto EXIT_FUNC_DEFAULT;
	nData = (unsigned int)atoi(strBuf);
	if (nData < 0 || nData > 255)
		goto EXIT_FUNC_DEFAULT;
	nIP[nState] = nData;
	nState++;

	if (strcmp(strData, m_config.Find("Server", "IP")) == 0)
	{
		// 서버 아이피를 사용할 경우 서브넷은 기본적으로 255.255.255.0
		nIP[4] = 255;
		nIP[5] = 255;
		nIP[6] = 255;
		nIP[7] = 0;
	}
	else if (nState != 8)
	{
		goto EXIT_FUNC_DEFAULT;
	}

	// 여기까지 오면 111.222.123.234/255.255.254.0이면
	// nIP 가 {111, 222, 123, 234, 255, 255, 254, 0} 이 된다
	m_nInternalIPFilter = ((nIP[0] << 24) & 0xff000000)
						  + ((nIP[1] << 16) & 0x00ff0000)
						  + ((nIP[2] <<  8) & 0x0000ff00)
						  + ((nIP[3]      ) & 0x000000ff);
	m_nInternalSubnetMask	= ((nIP[4] << 24) & 0xff000000)
							  + ((nIP[5] << 16) & 0x00ff0000)
							  + ((nIP[6] <<  8) & 0x0000ff00)
							  + ((nIP[7]      ) & 0x000000ff);
	m_nInternalIPFilter &= m_nInternalSubnetMask;

	return ;

EXIT_FUNC_DEFAULT:
	if (strcmp(strData, m_config.Find("Server", "IP")) == 0)
	{
		m_nInternalIPFilter = 0xffffffff;
		m_nInternalSubnetMask = 0xffffffff;
	}
	else
	{
		CheckInternalIP(m_config.Find("Server", "IP"));
	}
}

bool CServer::ConnectDB()
{
	if (!mysql_real_connect (
				&m_dbuser,
				m_config.Find("User DB", "IP"),
				m_config.Find("User DB", "User"),
				m_config.Find("User DB", "Password"),
				m_config.Find("User DB", "DBName"),
				0, NULL, 0))
	{
		LOG_ERROR("Can't connect data DB : ip[%s] id[%s] pw[%s] dbname[%s] error[%s]",
				  m_config.Find("User DB", "IP"),
				  m_config.Find("User DB", "User"),
				  m_config.Find("User DB", "Password"),
				  m_config.Find("User DB", "DBName"),
				  mysql_error(&m_dbuser)
				 );
		return false;
	}

	if (!mysql_real_connect (
				&m_dbdata,
				m_config.Find("Data DB", "IP"),
				m_config.Find("Data DB", "User"),
				m_config.Find("Data DB", "Password"),
				m_config.Find("Data DB", "DBName"),
				0, NULL, 0))
	{
		LOG_ERROR("Can't connect data DB : ip[%s] id[%s] pw[%s] dbname[%s] error[%s]",
				  m_config.Find("Data DB", "IP"),
				  m_config.Find("Data DB", "User"),
				  m_config.Find("Data DB", "Password"),
				  m_config.Find("Data DB", "DBName"),
				  mysql_error(&m_dbdata)
				 );
		return false;
	}

	if (!mysql_real_connect (
				&m_dbAuth,
				m_config.Find("Auth DB", "IP"),
				m_config.Find("Auth DB", "User"),
				m_config.Find("Auth DB", "Password"),
				m_config.Find("Auth DB", "DBName"),
				0, NULL, 0))
	{
		LOG_ERROR("Can't connect data DB : ip[%s] id[%s] pw[%s] dbname[%s] error[%s]",
				  m_config.Find("Auth DB", "IP"),
				  m_config.Find("Auth DB", "User"),
				  m_config.Find("Auth DB", "Password"),
				  m_config.Find("Auth DB", "DBName"),
				  mysql_error(&m_dbAuth)
				 );
		return false;
	}

#ifdef LOCAL_LOGIN_LOG
	if (!mysql_real_connect (
				&m_dbLocalLog,
				m_config.Find("LocalLog DB", "IP"),
				m_config.Find("LocalLog DB", "User"),
				m_config.Find("LocalLog DB", "Password"),
				m_config.Find("LocalLog DB", "DBName"),
				0, NULL, 0))
	{
		LOG_ERROR("Can't connect data DB : ip[%s] id[%s] pw[%s] dbname[%s] error[%s]",
				  m_config.Find("LocalLog DB", "IP"),
				  m_config.Find("LocalLog DB", "User"),
				  m_config.Find("LocalLog DB", "Password"),
				  m_config.Find("LocalLog DB", "DBName"),
				  mysql_error(&m_dbLocalLog)
				 );
		return false;
	}
#endif //LOCAL_LOGIN_LOG
	return true;
}

void CServer::DisconnectDB()
{
	mysql_close (&m_dbuser);
	mysql_close (&m_dbAuth);

#ifdef LOCAL_LOGIN_LOG
	mysql_close(&m_dbLocalLog);
#endif //LOCAL_LOGIN_LOG
}

bool CServer::InitGame()
{
	// version information loading
	GAMELOG << init("SYSTEM")
			<< "Client Version Loading..."
			<< end;

	CDBCmd dbcmd;
	dbcmd.Init(&m_dbdata);
	dbcmd.SetQuery("SELECT a_min, a_max FROM t_clientversion");
	if (!dbcmd.Open() || !dbcmd.MoveFirst() || !dbcmd.GetRec("a_min", m_clientversionMin) || !dbcmd.GetRec("a_max", m_clientversionMax))
		return false;
	dbcmd.Close();

	//////////////////////////////////////////////////////////////////////////

	m_nConnector = atoi(m_config.Find("Connector Server", "Count"));
	if (m_nConnector == 0)
		return false;

	m_connector = new CDescriptor*[m_nConnector];
	memset(m_connector, 0, sizeof(CDescriptor*) * m_nConnector);

	for (int i = 0; i < m_nConnector; ++i)
	{
		m_connector[i] = new CDescriptor(NULL, CLIENT_TYPE_CONNECTOR);

		std::string str = boost::str(boost::format("Connector_%d") % i);

		m_connector[i]->m_serverNo = atoi(m_config.Find(str.c_str(), "Number"));
		m_connector[i]->m_nMaxServer = atoi(m_config.Find(str.c_str(), "MaxSubNumber"));

		m_connector[i]->m_playerNum = new int[m_connector[i]->m_nMaxServer];
		m_connector[i]->m_portNumber = new int[m_connector[i]->m_nMaxServer];
		m_connector[i]->m_ipAddr = new char*[m_connector[i]->m_nMaxServer];
		m_connector[i]->m_serverSubNo = new int[m_connector[i]->m_nMaxServer];

		for (int j = 0; j < m_connector[i]->m_nMaxServer; ++j)
		{
			m_connector[i]->m_playerNum[j] = -1;
			m_connector[i]->m_serverSubNo[j] = j + 1;
			m_connector[i]->m_ipAddr[j] = (char *)"";
			m_connector[i]->m_portNumber[j] = -1;
		}

		std::string connect_host = m_config.Find(str.c_str(), "IP");
		int connect_port = atoi(m_config.Find(str.c_str(), "Port"));
		m_connector[i]->setConnectInfo(connect_host, connect_port);
		m_connector[i]->Connect();
	}

	GAMELOG << init("SYSTEM")
			<< "Entering Login Server..."
			<< end;

	return true;
}

void CServer::ProcConnector(CNetMsg::SP& msg)
{
	msg->MoveFirst();

	if (msg->m_mtype != MSG_CONN_REP)
		return;

	// MSG_CONN_PLAYER_REP,		// 서버군별 유저수 응답	: serverno(n) serversubno(n) usercount(n)
	char subtype;

	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_CONN_PLAYER_REP:
		{
			int serverCount, server, subNo, player, port;
			CLCString ip(HOST_LENGTH + 1);
			int i, j, k;
			int hardcore_flag = 0;

			// 서브 서버 숫자 더 작은걸로 선택(컨넥터, 로긴서버중)
			int maxServer;

			RefMsg(msg) >> server
						>> serverCount
						>> hardcore_flag;

			for (i=0; i < m_nConnector; i++)
			{
				// 서버군 번호가 같은 인덱스 찾음
				if (m_connector[i]->isRun())
				{
					if (server == m_connector[i]->m_serverNo)
					{
						m_connector[i]->m_hardcore_flag = hardcore_flag;

						// 더 작은걸로
						if (serverCount < m_connector[i]->m_nMaxServer)
							maxServer = serverCount;
						else
							maxServer = m_connector[i]->m_nMaxServer;

						break;
					}
				}
			}

			if (i != m_nConnector)
			{
				for (j=0; j < maxServer; j++)
				{
					RefMsg(msg) >> subNo
								>> player
								>> ip
								>> port;

					LOG_TRACE("receivce player info : subNo[%d] / player[%d] / ip[%s] / port[%d]",
							  subNo, player, (const char *)ip, port);

#ifdef SETTING_IF_INNER_IP // 외부 아이피와 내부 아이피가 나뉘면 사용 ...yhj
					if(  strcmp(m_config.Find("Connector Server", "EX_IP"),"") != 0 )
						ip.Format("%s", m_config.Find("Connector Server", "EX_IP") );
#endif // SETTING_IF_INNER_IP

					// 로긴서버의 서브 서버 번호 중에서 찾기
					for (k=0; k < m_connector[i]->m_nMaxServer; k++)
					{
						if (subNo == m_connector[i]->m_serverSubNo[k])
						{
							m_connector[i]->m_playerNum[k] = player;
							m_connector[i]->m_ipAddr[k] = new char[HOST_LENGTH + 1];
							strcpy(m_connector[i]->m_ipAddr[k], ip);
							m_connector[i]->m_portNumber[k] = port;
						}
					}
				}
			}
		}
		break;

	default:
		break;
	}
}

void CServer::ProcMessenger(CNetMsg::SP& msg)
{
	msg->MoveFirst();

	int seq, server, subno, zone;
	char subtype;

	RefMsg(msg) >> seq
				>> server
				>> subno
				>> zone
				>> subtype;

	switch (msg->m_mtype)
	{
	case MSG_MSGR_REP:
		{
//			int i, j;

			switch (subtype)
			{
			case MSG_MSGR_LOGOUT_REP:
				{
					char success;
					CLCString id(MAX_ID_NAME_LENGTH + 1);

					RefMsg(msg) >> success
								>> id;

					GAMELOG << init("LOGIN ERROR", id)
							<< "CANNOT DISCONNECT ALREADY CONNECTION"
							<< end;
				}

				break;

			default:
				break;
			}
		}
		break;

	case MSG_MSGR_REQ:
		{
			switch (subtype)
			{
			case MSG_MSGR_RECOMMEND:
				{
					int nGMCharIndex;
					int nRecommendServer;

					RefMsg(msg) >> nGMCharIndex
								>> nRecommendServer;

					m_nRecommendServer = nRecommendServer;
				}
				break;

			case MSG_MSGR_ENDGAME_REQ: //XX endgame - 3
				PrintExcuteInfo::Instance()->SetStopMessage("endgame");
				bnf::instance()->Stop();
				m_bshutdown = true;
				break;

			case MSG_MSGR_LOGIN_SERVER:
				{
					int nCode;
					RefMsg(msg) >> nCode;
					switch (nCode)
					{
					case 0:		// close
						if (!m_bOnlyLocal)
						{
							remove(LOGIN_SERVER_OPEN_CHECK_FILENAME);
							PrintExcuteInfo::Instance()->SetStopMessage("subtype[MSG_MSGR_LOGIN_SERVER] code=0");
							bnf::instance()->Stop();
							m_bshutdown = false;
						}
						break;
					case 1:		// open
						if (m_bOnlyLocal)
						{
							FILE* fpCheckFile = fopen(LOGIN_SERVER_OPEN_CHECK_FILENAME, "w");
							fclose(fpCheckFile);
							PrintExcuteInfo::Instance()->SetStopMessage("subtype[MSG_MSGR_LOGIN_SERVER] code=1");
							bnf::instance()->Stop();
							m_bshutdown = false;
						}
						break;
					case 2:		// reboot
						PrintExcuteInfo::Instance()->SetStopMessage("subtype[MSG_MSGR_LOGIN_SERVER] code=2");
						bnf::instance()->Stop();
						m_bshutdown = false;
						break;
					}
				}
				break;

			default:
				break;
			}
		}
		break;

	default:
		break;
	}
}

#if defined(LC_USA)
void CServer::WriteDBPlayerCount()
{
	if(m_pulseSaveTableCount <= m_pulse)
	{
		CDBCmd dbUser;
		dbUser.Init(&gserver.m_dbuser);

		int i, k;
		for(i = 0; i < m_nConnector; ++i)
		{
			if(m_connector[i])
			{
				for(k = 0; k < m_connector[i]->m_nMaxServer; ++k)
				{
					char replace_connect_status_sql[1024] = {0,};
					sprintf(replace_connect_status_sql,"REPLACE INTO t_connect_status (a_server, a_sub_num, a_count) VALUES ( %d, %d, %d )",
							m_connector[i]->m_serverNo, m_connector[i]->m_serverSubNo[k], m_connector[i]->m_playerNum[k]);

					dbUser.SetQuery(replace_connect_status_sql);
					dbUser.Update();
				}
			}
		}

		m_pulseSaveTableCount  +=  60 * 5 * PASSES_PER_SEC;
	}
}

void CServer::BlockIPLog( const char* user_ip )
{
	CDBCmd dbUser;
	dbUser.Init( &gserver.m_dbuser );

	char insert_ip_log_query[1024] = {0,};
	sprintf( insert_ip_log_query, "INSERT INTO t_ip_log (a_ip, a_date) VALUES ( '%s', NOW() )", user_ip );
	dbUser.SetQuery( insert_ip_log_query );
	dbUser.Update();
}
#endif	// #if defined(LC_USA)

void CServer::operate( rnSocketIOService* service )
{
	// 클라이언트 Accept 처리 부분
	CDescriptor* newd = new CDescriptor(service, CLIENT_TYPE_CLIENT);
	service->SetUserData((void *)newd);
	service->setCrypt();

	STATE(newd) = CON_GET_LOGIN;
}