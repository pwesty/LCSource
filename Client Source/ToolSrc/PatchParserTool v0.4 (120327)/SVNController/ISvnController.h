#pragma once

#include "../Common/DLLAPI.h"
#include <string>
#include <vector>

class ISvnController
{
public:
	// default log
	virtual std::string log(const std::string& strURL) = 0;

	// log option : xml, verbose, revision
	virtual std::string log(const std::string& strURL, bool bXML, bool bVerbose, int nStartRev, int nEndRev = -1) = 0;

	// log option : xml, vervose, revisions
	virtual std::string log(const std::string& strURL, bool bXML, bool bVerbose, const std::vector<int>& vecRevisions) = 0;

	// log option : limit, xml
	virtual std::string log(const std::string& strURL, int nLimit, bool bXML = false, bool bVerbose = false) = 0;

	// dispatch : stinrg
	virtual std::string dispatchToString(const std::string& strLog) = 0;
};

extern "C" DLL_API ISvnController* getSvnController();