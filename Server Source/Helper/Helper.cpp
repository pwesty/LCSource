// NewEarthServer.cpp : Defines the entry point for the console application.
//
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "../ShareLib/SignalProcess.h"
#include "Log.h"
#include "Server.h"

#include "../ShareLib/PrintExcuteInfo.h"

#ifdef SERVER_AUTHENTICATION
#include "../ShareLib/ServerAuthentication.h"
#endif

void process_after_signal(int signo);

int main(int argc, char* argv[], char* envp[])
{
	if (!gserver.LoadSettingFile())
	{
		puts("Setting Error!!");
		return 1;
	}

	// 로그 초기화
	std::string tstr = boost::str(boost::format("Helper_%1%") % gserver.m_config.Find("Server", "Number"));
	LogSystem::setSubstitutedValue("logfile", tstr.c_str());
	LogSystem::configureXml("../log.xml");

	PrintExcuteInfo::Instance()->PrintStart("Helper", LC_LOCAL_STRING);

	if (InitSignal(process_after_signal) == false)
		return 1;

	if (!gserver.ConnectDB())
	{
		LOG_INFO("Cannot Connect DB!!");
		return 1;
	}

	if (!gserver.LoadSettings())
	{
		LOG_INFO("Setting Error!!");
		return 1;
	}

	if (!gserver.InitGame())		return 1;

	//////////////////////////////////////////////////////////////////////////
	std::string bind_host = gserver.m_config.Find("Server", "IP");
	int bind_port = atoi(gserver.m_config.Find("Server", "Port"));
	if (bnf::instance()->CreateListen(bind_host, bind_port, 0, &gserver) == SessionBase::INVALID_SESSION_HANDLE)
	{
		puts("Helper : can't bind listen session");
		return 1;
	}

	timerPerMinute::instance()->Init();
	timerPerHour::instance()->Init();

	// 메시지를 지우기 위한 타이머 생성
	bnf::instance()->CreateMSecTimer(1 * 1000, ClearMessageListTimer::instance());

	//////////////////////////////////////////////////////////////////////////
	// 게임에서 사용할 1초짜리 타이머 생성
	bnf::instance()->CreateMSecTimer(1 * 1000, timerPerSecond::instance());

#ifdef SERVER_AUTHENTICATION
	if (ServerAuthentication::instance()->isValidCompileTime() == false)
		return 1;
#endif

	timerPerMidnight::instance()->Init();

	puts("Helper server started...");
	bnf::instance()->Run();
	//////////////////////////////////////////////////////////////////////////
	//결사대 정보 저장
	gserver.m_syndicate.save();
	gserver.saveCastleDungeonInfo();

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