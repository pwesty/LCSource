#include "XmlParser.h"
#include "tinyxml.h"
#include <windows.h>

DLL_API IParser* getXmlParser()
{
	//IParser* pParser = new CXmlParser;
	//return pParser;
	return CXmlParser::instance();
}

bool CXmlParser::parser(const std::string& strXml, _LogDataList& outLogDataList)
{
	TiXmlDocument doc;

	doc.Parse(strXml.c_str());
	TiXmlNode* pLog = doc.FirstChild("log");

	if (!pLog)
		return false;

	TiXmlElement* pLogEntry = pLog->FirstChildElement("logentry");

	while (pLogEntry)
	{
		SLogData LogData;

		if (!_parseDataFromNode(pLogEntry, LogData))
			return false;

		outLogDataList.push_back(LogData);

		pLogEntry = pLogEntry->NextSiblingElement();
	}

	return true;
}

bool CXmlParser::_parseDataFromNode(const TiXmlElement* pLogEntry, SLogData& LogData)
{
	if (!pLogEntry)
		return false;

	// revision
	LogData.revision = atoi(pLogEntry->Attribute("revision"));

	// author
	const TiXmlElement* pAuthor = pLogEntry->FirstChildElement("author");

	if (pAuthor)
		LogData.author = pAuthor->GetText();

	// date
	const TiXmlElement* pDate = pLogEntry->FirstChildElement("date");

	if (pDate)
		LogData.date = pDate->GetText();

	// paths
	const TiXmlElement* pPaths = pLogEntry->FirstChildElement("paths");

	if (pPaths)
	{
		// path
		const TiXmlElement* pPath = pPaths->FirstChildElement("path");

		while (pPath)
		{
			std::string action = pPath->Attribute("action");
			std::string text = pPath->GetText();
			LogData.paths.push_back(SResourcePath(action, text));

			pPath = pPath->NextSiblingElement();
		}
	}

	// msg
	const TiXmlElement* pMsg = pLogEntry->FirstChildElement("msg");

	if (pMsg)
	{
		if (pMsg->GetText())
		{
			LogData.msg = pMsg->GetText();

			/////// -> UTF-8
			int lengthW = MultiByteToWideChar(CP_UTF8, 0, LogData.msg.c_str(), -1, NULL, NULL);
			
			wchar_t* strXmlW = new wchar_t[lengthW];
			ZeroMemory(strXmlW, sizeof(wchar_t) * lengthW);
			
			MultiByteToWideChar(CP_UTF8, 0, LogData.msg.c_str(), -1, strXmlW, lengthW);
			strXmlW[lengthW - 1] = '\0';
			/////////////////

			/////// -> MultiByte
			int lengthA = WideCharToMultiByte(CP_ACP, 0, strXmlW, -1, NULL, NULL, NULL, NULL);
			
			char* strXmlA = new char[lengthA];
			ZeroMemory(strXmlA, sizeof(char) * lengthA);
			
			WideCharToMultiByte(CP_ACP, 0, strXmlW, -1, strXmlA, lengthA, NULL, NULL);
			strXmlA[lengthA - 1] = '\0';
			/////////////////

			LogData.msg = strXmlA;

			delete[] strXmlW;
			delete[] strXmlA;
		}
	}

	return true;
}
