// NewEarthServer.cpp : Defines the entry point for the console application.
//
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "../ShareLib/SignalProcess.h"
#include "Server.h"
#include "GMToolInMessenger.h"
#include "SubHelperInMessenger.h"

#include "../ShareLib/PrintExcuteInfo.h"

#ifdef SERVER_AUTHENTICATION
#include "../ShareLib/ServerAuthentication.h"
#endif

void process_after_signal(int signo);

int main(int argc, char* argv[], char* envp[])
{
	// 로그 초기화
	LogSystem::setSubstitutedValue("logfile", "Messenger");
	LogSystem::configureXml("../log.xml");

	PrintExcuteInfo::Instance()->PrintStart("Messenger", LC_LOCAL_STRING);

	if (InitSignal(process_after_signal) == false)
		return 1;

	if (!gserver.LoadSettingFile())
	{
		LOG_INFO("Setting Error!!");
		exit(0);
	}

	if (!gserver.InitGame())		return 1;

	//////////////////////////////////////////////////////////////////////////
	std::string bind_host = gserver.m_config.Find("Server", "IP");
	int bind_port = atoi(gserver.m_config.Find("Server", "Port"));
	if (bnf::instance()->CreateListen(bind_host, bind_port, 0, &gserver) == SessionBase::INVALID_SESSION_HANDLE)
	{
		puts("Messenger : can't bind listen session");
		return 1;
	}

	if (GMToolInMessenger::instance()->Init() == false)
		return 1;

	if (SubHelperInMessenger::instance()->Init() == false)
		return 1;

	// 메시지를 지우기 위한 타이머 생성
	bnf::instance()->CreateMSecTimer(1 * 1000, ClearMessageListTimer::instance());

#ifdef SERVER_AUTHENTICATION
	if (ServerAuthentication::instance()->isValidCompileTime() == false)
		return 1;
#endif

	puts("Messenger server started...");
	bnf::instance()->Run();
	//////////////////////////////////////////////////////////////////////////

	PrintExcuteInfo::Instance()->PrintEnd();

	return 0;
}

void process_after_signal(int signo)
{
	bnf::instance()->Stop();

	PrintExcuteInfo::Instance()->SetSignalNo(signo);
}