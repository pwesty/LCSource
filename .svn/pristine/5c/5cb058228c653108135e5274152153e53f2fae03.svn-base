#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "../ShareLib/SignalProcess.h"
#include "Log.h"
#include "Server.h"
#include "DBManager.h"
#include "ServerTimer.h"

#include "../ShareLib/PrintExcuteInfo.h"

#ifdef SERVER_AUTHENTICATION
#include "../ShareLib/ServerAuthentication.h"
#endif

void process_after_signal(int signo);

int main(int argc, char* argv[], char* envp[])
{
	gserver = new CServer;

	//////////////////////////////////////////////////////////////////////////
	// 사내 서버일 경우를 체크
	gserver->barunsongames_flag = false;

	if (argc == 3)
	{
		std::string tstr = "qatest";
		if (tstr == argv[2])
		{
			gserver->barunsongames_flag = true;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	if (!gserver->LoadSettingFile())
	{
		puts("Setting Error!!");
		exit(0);
	}

	// 로그 초기화
	std::string tstr = boost::str(boost::format("Gameserver_%1%_%2%") % gserver->m_serverno % gserver->m_subno);
	LogSystem::setSubstitutedValue("logfile", tstr.c_str());
	LogSystem::configureXml("../log.xml");

	PrintExcuteInfo::Instance()->PrintStart("GameServer", LC_LOCAL_STRING);

	if (InitSignal(process_after_signal) == false)
		return 1;

	gserver->m_serverAddr = gserver->m_config.Find("Server", "IP");
	gserver->m_serverPort = atoi(gserver->m_config.Find("Server", "Port"));

	int i;
	int count = atoi(gserver->m_config.Find("Zones", "Count"));
	for (i = 0; i < count; i++)
	{
		CLCString tmp(20);
		tmp.Format("Zone_%d", i);
		if (strcmp(gserver->m_config.Find(tmp, "Remote"), "FALSE") == 0)
			break;
	}
	if (i == atoi(gserver->m_config.Find("Zones", "Count")))
		i = -1;
	else
	{
		CLCString tmp(20);
		tmp.Format("Zone_%d", i);
		i = atoi(gserver->m_config.Find(tmp, "No"));
	}

	if (!gserver->ConnectDB())
	{
		LOG_INFO("Cannot Connect DB!!");
		exit(0);
	}

	if (!gserver->LoadSettings())
	{
		LOG_INFO("Setting Error!!");
		exit(0);
	}

	gserver->ClearMobAISeq();

	if (!gserver->InitGame())		return 1;

	LOG_INFO("SETTINGS");

	if (gserver->m_bOnlyLocal)
		LOG_INFO(" : Only Local IP");

	LOG_INFO("Force start zone 0");

	char tmpBuf[100] = {0,};
	strcpy (tmpBuf, "LastChaos Running...");

#ifdef IMP_SPEED_SERVER
	if( gserver->m_bSpeedServer )
		strcat( tmpBuf, " --[ SPEED ] " );
#endif //IMP_SPEED_SERVER
	LOG_INFO (tmpBuf);

#ifdef TLD_EVENT_SONG
	gserver->checkTldEvent();
#endif

	//////////////////////////////////////////////////////////////////////////
	if (DBManager::instance()->Init() == false)
		return 1;

	// 각 서버에 접속
	gserver->m_messenger->connect();
	gserver->m_connector->connect();
	gserver->m_helper->connect();
	gserver->m_subHelper->connect();

	gserver->m_serverAddr = gserver->m_config.Find("Server", "IP");
	gserver->m_serverPort = atoi(gserver->m_config.Find("Server", "Port"));

	int wait_time_sec = 70;
#ifdef LC_KOR
	wait_time_sec = 5 * 60;
#endif
	if (bnf::instance()->CreateListen(gserver->m_serverAddr, gserver->m_serverPort, wait_time_sec, gserver) == SessionBase::INVALID_SESSION_HANDLE)
	{
		puts("GameServer : can't bind listen session");
		return 1;
	}

	ServerTimerPerSecond::instance()->Init();
	ServerTimerPerHundredMSecond::instance()->Init();
	ServerTimerPerMinute::instance()->Init();
	ServerTimerPerHour::instance()->Init();
	ServerTimerPerMidnight::instance()->Init();

#ifdef SERVER_AUTHENTICATION
	if (ServerAuthentication::instance()->isValidCompileTime() == false)
		return 1;

// 	if (ServerAuthentication::instance()->isValidIp() == false)
// 		return 1;

	ServerAuthentication::instance()->setServerInfo(LC_LOCAL_CODE, (const char *)gserver->m_serverAddr, gserver->m_serverPort);
#endif

	puts("Game server started...");
	bnf::instance()->Run();
	//////////////////////////////////////////////////////////////////////////

	DBManager::instance()->JoinAll();

	if (gserver->m_bShutdown)
	{
		FILE *fp = fopen (".shutdown", "w");
		fclose (fp);
	}

	PrintExcuteInfo::Instance()->PrintEnd();

	delete gserver;

	return 0;
}

void process_after_signal(int signo)
{
	DBManager::instance()->JoinAll();
	bnf::instance()->Stop();

	PrintExcuteInfo::Instance()->SetSignalNo(signo);
}