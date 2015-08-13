#include "StdH.h"
#include "LoadString.h"
#include <Engine/Help/DefineHelp.h>
#include <Engine/Base/CTString.h>
#include <Engine/Base/Stream.h>

bool StringLoader::LoadString(eSTRING eType, int nNation)
{
	bool bRet = false;

	if (eType < 0 || eType >= eSTRING_MAX)
		return false;

	const char* strString[] = {
		"strClient",
		"strHelp",
		"strItem",
		"strSetItem",
		"strOption",
		"strRareOption",
		"strNPCName",
		"strNPCHelp",
		"strNPCShop",
		"strQuest",
		"strSkill",
		"strSSkill",
		"strAction",
		"strCombo",
		"strAffinity",
		"strLacarette",
		"strItemCollection"
	};

	std::string strPath;

	{
		CTString	strFullPath = _fnmApplicationPath.FileDir();

		strPath = strFullPath + "Local\\";
		strPath += DefHelp::getNationPostfix(nNation, true);
		strPath += "\\String\\";
		strPath += strString[eType];
		strPath += DefHelp::getNationPostfix(nNation);
		strPath += ".lod";

		bRet = LoadData(eType, strPath.c_str());
	}

	return bRet;
}

void StringLoader::release(eSTRING eType)
{
	if (eType < 0 || eType >= eSTRING_MAX)
		return;

	_tbl_string[eType].release();
}

bool StringLoader::LoadData( eSTRING eType, const char* strPath )
{
	if (eType < 0 || eType >= eSTRING_MAX)
		return false;

	return _tbl_string[eType].load_file(strPath, StringCount[eType]);
}

const char* StringLoader::getData( eSTRING eType, int index, int id )
{
	if (eType < 0 || eType >= eSTRING_MAX)
		return NULL;

	stSTRING& stStr = _tbl_string[eType].get_data(index);

	if (stStr._vecString.size() == 0 || stStr._vecString.size() <= id)
		return NULL;

	return stStr._vecString[id].c_str();
}

int StringLoader::getCount( eSTRING eType )
{
	if (eType < 0 || eType >= eSTRING_MAX)
		return 0;

	return _tbl_string[eType].getCount();
}


