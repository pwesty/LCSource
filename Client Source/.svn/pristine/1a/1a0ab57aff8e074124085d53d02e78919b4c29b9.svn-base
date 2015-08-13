#pragma once

#include "ISvnController.h"
#include "../Common/ISingleton.h"
#include <string>
#include <vector>

//enum E_SVN_LOG_OPTIONS
//{
//	ESLO_UNKNOWN,
//	ESLO_NONE,
//	ESLO_REVISION,
//	ESLO_QUIET,				// print as little as possible
//	ESLO_VERBOSE,			// print extra information
//	ESLO_TARGETS,
//	ESLO_STOP_ON_COPY,		// do not cross copies while traversing history
//	ESLO_INCREAMENTAL,		// give output suitable for concatenation
//	ESLO_XML,
//	ESLO_USERNAME,			// specify a username
//	ESLO_PASSWORD,			// specify a password
//	ESLO_NO_AUTH_CACHE,		// do not cache authentication tokens
//	ESLO_NON_INTERACTIVE,	// do no interactive prompting
//	ESLO_CONFIG_DIR,		// read user configuration files from directory
//};

class CSvnController : public ISvnController, public ISingleton<CSvnController>
{
//private:
//	int m_nLimit;
//	bool m_bXML;
//
//private:
//	CSvnController()
//	: m_nLimit(0), m_bXML(false) {}

	// 인터페이스를 어떻게 제공해야되나 하ㅏㅏㅏ
public:
	// default log
	virtual std::string log(const std::string& strURL);

	// log option : xml, verbose, revision
	virtual std::string log(const std::string& strURL, bool bXML, bool bVerbose, int nStartRev, int nEndRev = -1);

	// log option : xml, vervose, revisions
	virtual std::string log(const std::string& strURL, bool bXML, bool bVerbose, const std::vector<int>& vecRevisions);

	// log option : limit, xml
	virtual std::string log(const std::string& strURL, int nLimit, bool bXML = false, bool bVerbose = false);

	//std::string log(const std::string& url, const E_SVN_LOG_OPTIONS options, long extra)
	//{
	//	std::string log = this->log(url);

	//	switch (options)
	//	{
	//	case ESLO_NONE :
	//		break;
	//	}

	//	return log;
	//}

	// dispatch : string
	virtual std::string dispatchToString(const std::string& strLog);

	// 조합 순서 상관없음
private:
	std::string _revision(int nStartRev, int nEndRev = -1);

	std::string _limit(int nLimit);

	std::string _xml();

	std::string _verbose();
};

#define SvnController CSvnController::instance()