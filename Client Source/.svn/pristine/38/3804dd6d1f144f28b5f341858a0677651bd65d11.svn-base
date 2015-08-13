#pragma once

#include "../Common/DLLAPI.h"
#include "SLogData.h"

class IParser
{
public:
	virtual bool parser(const std::string& strXml, _LogDataList& outLogDataList) = 0;
};

DLL_API IParser* getXmlParser();