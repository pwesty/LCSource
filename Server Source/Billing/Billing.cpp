// NewEarthServer.cpp : Defines the entry point for the console application.
//

#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"

#include <boost/format.hpp>

void at_exit_exec (void)
{
}

int main(int argc, char* argv[], char* envp[])
{
	int nRetCode = 1004;

#ifdef SIGPIPE
	signal (SIGPIPE, SIG_IGN);
#endif

	if (!gserver.LoadSettingFile())
	{
		puts("Setting Error!!");
		exit(0);
	}


	std::string tstr = boost::str(boost::format("Connector_%1%") % gserver.m_config.Find("Server", "Number"));
	LogSystem::setSubstitutedValue("logfile", tstr.c_str());
	LogSystem::configureXml("../log.xml");
	//g_log.InitLogFile(false, &g_gamelogbuffer, "BS");

#ifndef LC_TLD
	//gserver.ReadSectionList();
#endif //#ifndef LC_TLD

	if (!gserver.InitGame())		return 1;

	if (!gserver.ConnectDB())		return 2;

	puts("Billing Server Running...");

	atexit (at_exit_exec);

	gserver.Run();
	gserver.Close();

	if (gserver.m_breboot)
	{
		FILE *fp = fopen (".shutdown", "w");
		fclose (fp);
	}

	GAMELOG << init("SYSTEM") << "End!" << end;

	return nRetCode;
}
