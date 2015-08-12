#include <string>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <boost/assign/list_of.hpp>
#include "Conf.h"
#include "mysql.h"
#include "logsystem.h"
#include "PrintExcuteInfo.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/utsname.h>
#include "svn_info.h"
#endif

PrintExcuteInfo::PrintExcuteInfo( void )
{
#ifdef _DEBUG
	debug_str_ = "DEBUG";
#else
	debug_str_ = "RELEASE";
#endif

	sig_no_ = -1;
}

PrintExcuteInfo* PrintExcuteInfo::Instance()
{
	static PrintExcuteInfo __instance;
	return &__instance;
}

void PrintExcuteInfo::PrintStart( const std::string server_name, const std::string national )
{
	server_name_ = server_name;
	national_ = national;

	LOG_INFO("\n\n\n");
	LOG_INFO("-------------------------- %s (%s / %s / %s) start ------------------------",
			 server_name_.c_str(), national_.c_str(), debug_str_.c_str(), test_str_.c_str());
	LOG_INFO("\n\n\n");

#ifndef WIN32
	LOG_INFO("svn info : %s", last_svn_revision);
	LOG_INFO("compile time : %d", compile_time ^ 0x12345678);

	// 이 프로세서가 열수 있는 최대 파일수
	LOG_INFO("openfiles = getdtablesize[%d]", getdtablesize());

	{
		// OS Name 출력하기
		struct utsname uts;
		uname(&uts);
		LOG_INFO("OS = sysname : %s, release : %s, version : %s, machine : %s",
				 uts.sysname, uts.release, uts.version, uts.machine);
	}

	{
		// CPU 정보 출력하기
		std::string outstr = "\n-- CPU INFORMATION --\n";
		outstr.reserve(10240);
		std::string line;
		std::ifstream finfo("/proc/cpuinfo");
		while (std::getline(finfo, line))
		{
			if (line.find("processor", 0) != std::string::npos)
			{
				outstr += line + "\n";
			}
			else if (line.find("model name", 0) != std::string::npos)
			{
				outstr += line + "\n";
			}
		}
		LOG_INFO(outstr.c_str());
	}

	{
		// MEMORY 정보 출력하기
		std::string outstr = "\n-- MEMORY INFORMATION --\n";
		outstr.reserve(10240);
		std::string line;
		std::ifstream finfo("/proc/meminfo");
		while (std::getline(finfo, line))
		{
			outstr += line + "\n";
		}
		LOG_INFO(outstr.c_str());
	}

#endif

	LOG_INFO("MYSQL CLIENT VERSION : %s", MYSQL_SERVER_VERSION);
}

void PrintExcuteInfo::PrintEnd()
{
	if (sig_no_ != -1)
	{
		PrintEndBySignal();
		return;
	}

	LOG_INFO("\n\n\n");
	LOG_INFO("-------------------------- %s (%s / %s / %s) end (%s) ------------------------",
			 server_name_.c_str(), national_.c_str(), debug_str_.c_str(), test_str_.c_str(), stop_message_.c_str());
	LOG_INFO("\n\n\n");
}

void PrintExcuteInfo::PrintEndBySignal()
{
	std::map<int, std::string> localmap = boost::assign::map_list_of(1, "SIGHUP")
										  (2, "SIGINT")
										  (3, "SIGQUIT")
										  (4, "SIGILL")
										  (6, "SIGABRT")
										  (7, "SIGBUS")
										  (8, "SIGFPE")
										  (9, "SIGKILL")
										  (11, "SIGSEGV")
										  (13, "SIGPIPE")
										  (14, "SIGALRM")
										  (15, "SIGTERM");

	std::string signame = "Unknown (to use man 7 signal)";
	std::map<int, std::string>::iterator it = localmap.find(sig_no_);
	if (it != localmap.end())
	{
		signame = it->second;
	}

	LOG_INFO("\n\n\n");
	LOG_INFO("-------------------------- %s (%s / %s / %s) end : signal %s(%d) ------------------------",
			 server_name_.c_str(), national_.c_str(), debug_str_.c_str(), test_str_.c_str(), signame.c_str(), sig_no_);
	LOG_INFO("\n\n\n");
}
