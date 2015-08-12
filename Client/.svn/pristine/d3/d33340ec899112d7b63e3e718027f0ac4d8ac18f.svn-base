//
//	*******************
//	* Sample Source 1 *
//	*******************
//
//
//	#include "XmlParser.h"
//	#include "BatchFilePatcher.h"
//	#include "../SvnController/SvnController.h"
//
//	void main()
//	{
//		std::string url("svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosKOR");
//
//		ISvnController* svn = getSvnController();
//		std::string log = svn->log(url, 3, true);
//		std::string xml = svn->dispatchToString(log);
//
//		_LogDataList list;
//		IParser* parser = getXmlParser();
//		parser->parser(xml, list);
//
//		SPatchConfig config;
//		config.strSvnUrl = url;
//		config.strFilePath = "patch.bat";
//
//		IPatcher* patcher = getBatchFilePatcher();
//		patcher->patch(list, config);
//	}
//
//


//
//	*******************
//	* Sample Source 2 *
//	*******************
//
//
//	#include "XmlParser.h"
//	#include "BatchFilePatcher.h"
//	#include "../SvnController/SvnController.h"
//	#include <vector>
//
//	void main()
//	{
//		std::string url("svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosHK_EP2");
//
//		std::vector<int> revisions;
//		revisions.push_back(15333);
//		revisions.push_back(15618);
//		revisions.push_back(15486);
//		revisions.push_back(15424);
//
//		ISvnController* svn = getSvnController();
//		std::string log = svn->log(url, true, true, revisions);
//		std::string xml = svn->dispatchToString(log);
//
//		_LogDataList list;
//		IParser* parser = getXmlParser();
//		parser->parser(xml, list);
//
//		SPatchConfig config;
//		config.strSvnUrl = url;
//		config.strFilePath = "patch.bat";
//
//		IPatcher* patcher = getBatchFilePatcher();
//		patcher->patch(list, config);
//	}
//
//