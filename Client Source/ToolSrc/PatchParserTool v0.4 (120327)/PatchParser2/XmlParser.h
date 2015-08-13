#pragma once

#include "IParser.h"
#include "../Common/ISingleton.h"
#include "SLogData.h"

class TiXmlElement;

class CXmlParser : public IParser, public ISingleton<CXmlParser>
{
public:
	virtual bool parser(const std::string& strXml, _LogDataList& outLogDataList);

private:
	bool _parseDataFromNode(const TiXmlElement* pLogEntry, SLogData& LogData);
};

#define XmlParser CXmlParser::instance()