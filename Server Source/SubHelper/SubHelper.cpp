// NewEarthServer.cpp : Defines the entry point for the console application.
//

#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "../ShareLib/SignalProcess.h"
#include "Log.h"
#include "Server.h"
#include "MessengerInSubHelper.h"

#include "../ShareLib/PrintExcuteInfo.h"
#include "ReservedGmCommandManager.h"
#include "NoticeSystemAgent.h"

#ifdef SERVER_AUTHENTICATION
#include "../ShareLib/ServerAuthentication.h"
#endif

void process_after_signal(int signo);

int main(int argc, char* argv[], char* envp[])
{
	int server_no = 0;

	if (!gserver.LoadSettingFile())
	{
		puts("Setting Error!!");
		exit(0);
	}

	server_no = atoi(gserver.m_config.Find("Server", "Number"));

	// 로그 초기화
	std::string tstr = boost::str(boost::format("SubHelper_%1%") % server_no);
	LogSystem::setSubstitutedValue("logfile", tstr.c_str());
	LogSystem::configureXml("../log.xml");

	PrintExcuteInfo::Instance()->PrintStart("SubHelper", LC_LOCAL_STRING);

	if (InitSignal(process_after_signal) == false)
		return 1;

	if (!gserver.ConnectDB())
	{
		LOG_INFO("Cannot Connect DB!!");
		exit(0);
	}
	if (!gserver.LoadSettings())
	{
		LOG_INFO("Setting Error!!");
		exit(0);
	}

	if (!gserver.InitGame())		return 1;

	std::string bind_host = gserver.m_config.Find("Server", "IP");
	int port = atoi(gserver.m_config.Find("Server", "Port"));
	bnf::instance()->CreateListen(bind_host, port, 0, &gserver);

	MessengerInSubHelper::instance()->connect(
		gserver.m_config.Find("Messenger Server", "IP"),
		gserver.m_config.Find("Messenger Server", "Port"));

	CServerTimerPerMinute::instance()->Init();
	CServerTimerPerHour::instance()->Init();

	// CServer에서 HeartBeat()를 실행할 타이머
	bnf::instance()->CreateMSecTimer(1 * 1000, CServerTimer::instance());

	// 메시지를 지우기 위한 타이머 생성
	bnf::instance()->CreateMSecTimer(1 * 1000, ClearMessageListTimer::instance());

	// 이벤트 달력
	ReservedGmCommandManager::Instance()->Init();

#if defined (NOTICE_SYSTEM)
	// 유저가 보낸 [전체 메시지]를 관리하는 타이머 생성
	bnf::instance()->CreateMSecTimer(11 * 1000, CNoticeSystemAgent::Instance());
#endif

#ifdef SERVER_AUTHENTICATION
	if (ServerAuthentication::instance()->isValidCompileTime() == false)
		return 1;
#endif

	puts("SubHelper server started...");
	bnf::instance()->Run();
	//////////////////////////////////////////////////////////////////////////

	if (gserver.m_bshutdown)
	{
		FILE *fp = fopen (".shutdown", "w");
		fclose (fp);
	}

	PrintExcuteInfo::Instance()->PrintEnd();

	return 0;
}

void process_after_signal(int signo)
{
	bnf::instance()->Stop();

	PrintExcuteInfo::Instance()->SetSignalNo(signo);
}