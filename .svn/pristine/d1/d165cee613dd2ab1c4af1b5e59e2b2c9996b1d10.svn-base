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
		puts("error load Setting File");
		exit(0);
	}

	// 로그 초기화
	std::string tstr = boost::str(boost::format("Connector_%1%") % gserver.m_config.Find("Server", "Number"));
	LogSystem::setSubstitutedValue("logfile", tstr.c_str());
	LogSystem::configureXml("../log.xml");

	PrintExcuteInfo::Instance()->PrintStart("Connector", LC_LOCAL_STRING);

	if (InitSignal(process_after_signal) == false)
		return 1;

	if (!gserver.ConnectDB())
	{
		GAMELOG << init("SYS_ERR") << "Connect DB" << end;
		exit(0);
	}

	if (!gserver.InitGame())
	{
		GAMELOG << init("SYS_ERR") << "Initialize Connector" << end;
		return 1;
	}

	//////////////////////////////////////////////////////////////////////////
	// GameServer용 listen
	std::string bind_host = gserver.m_config.Find("Server", "IP");
	if (bind_host == "ALL")
		bind_host = "0.0.0.0";
	int bind_port = atoi(gserver.m_config.Find("Server", "Port"));
	if (bnf::instance()->CreateListen(bind_host, bind_port, 0, &gserver) == SessionBase::INVALID_SESSION_HANDLE)
	{
		puts("Connector : can't bind listen session");
		return 1;
	}

	// 빌링 서버에 연결
	gserver.m_billing.connect();

	// HeartBeat timer
	bnf::instance()->CreateMSecTimer(1 * 1000, (void *)HeartBeatTimer::instance());

	// alive timer to billing
	bnf::instance()->CreateMSecTimer(2 * 60 * 1000, (void *)ServerAliveTimer::instance());

#ifdef SERVER_AUTHENTICATION
	if (ServerAuthentication::instance()->isValidCompileTime() == false)
		return 1;
#endif

	puts("Connect server started...");
	bnf::instance()->Run();
	//////////////////////////////////////////////////////////////////////////
	gserver.Close();

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