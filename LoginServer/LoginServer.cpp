// LoginServer.cpp : Defines the entry point for the console application.
//
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "../ShareLib/SignalProcess.h"
#include "Log.h"
#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "MessengerInLogin.h"

#include "../ShareLib/PrintExcuteInfo.h"

#ifdef SERVER_AUTHENTICATION
#include "../ShareLib/ServerAuthentication.h"
#endif

#if !defined(CIRCLE_WINDOWS) && defined(DEV_GAMIGO_HTTPS)
#include "../ShareLib/LCHttps.h"
#endif // Linux Lib 사용에 GER 만 적용, // Gamigo 요청 Https 인증 방식

void at_exit_exec (void)
{
	gserver.DisconnectDB();
}

void process_after_signal(int signo);

int main(int argc, char* argv[], char* envp[])
{
	// 로그 초기화
	LogSystem::setSubstitutedValue("logfile", "Login");
	LogSystem::configureXml("../log.xml");

	PrintExcuteInfo::Instance()->PrintStart("Login", LC_LOCAL_STRING);

	if (InitSignal(process_after_signal) == false)
		return 1;

	if (!gserver.LoadSettingFile())
	{
		LOG_INFO("Load Setting File Error!!");
		exit(0);
	}

	if (!gserver.ConnectDB())
	{
		LOG_INFO("Connect DB Error!!");
		exit(0);
	}

	if (!gserver.InitGame())		return 1;

	mysql_close(&gserver.m_dbdata);

#if !defined(CIRCLE_WINDOWS) && defined(DEV_GAMIGO_HTTPS)
	//char * ipAddr = inet_ntoa( *gserver.GetBindAddr() );
	//if( strcmp(ipAddr, "10.1.90.31") !=0 && strcmp(ipAddr, "10.1.90.34") !=0 && strcmp(ipAddr, "10.1.90.35") !=0 && strcmp(ipAddr, "101.79.53.208") !=0)
	if ( gserver.DoHttpsAuth() )
	{
		CLCHttps gamigoAuth;
		if( gamigoAuth.Connect("https://chk.global.lc.gamigo.com/auth",
							   "E8:E6:4C:1B:58:40:C9:70:A4:74:EB:F9:CF:C6:19:17:E3:F8:A7:DD") != CLCHttps::E_MATCH_FINGERPRINT )
		{
			LOG_INFO("Connect Fingerprint Error");
			return 1;
		}
	}
#endif // Linux Lib 사용에 GER 만 적용, // Gamigo 요청 Https 인증 방식

	LOG_INFO("Login Server Running...");

	atexit (at_exit_exec);

	//////////////////////////////////////////////////////////////////////////
	std::string bind_host = gserver.m_config.Find("Server", "IP");
	if (bind_host == "ALL")
		bind_host = "0.0.0.0";

	int bind_port = atoi(gserver.m_config.Find("Server", "Port"));
	if (bnf::instance()->CreateListen(bind_host, bind_port, 15, &gserver) == SessionBase::INVALID_SESSION_HANDLE)
	{
		puts("Login : can't bind listen session");
		return 1;
	}

	bnf::instance()->CreateMSecTimer(1 * 1000, CServerTimer::instance());
	CServerTimerPerMinute::instance()->Init();

	MessengerInLogin::instance()->Connect();

#ifdef SERVER_AUTHENTICATION
	if (ServerAuthentication::instance()->isValidCompileTime() == false)
		return 1;
#endif

	puts("Login server started...");
	bnf::instance()->Run();
	//////////////////////////////////////////////////////////////////////////

	if (gserver.m_bshutdown)
	{
		remove(LOGIN_SERVER_OPEN_CHECK_FILENAME);
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