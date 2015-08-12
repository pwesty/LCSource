#include "stdh.h"
#include "LevelUpGuide.h"

#include <Engine/Util.h>

bool CLevelUpGuide::loadEx( const char* szFileName )
{
	FILE*	fp = NULL;

	fp = fopen(szFileName, "rb");

	if (fp == NULL)
		return false;

	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stLevelUpMsgData* pdata = new stLevelUpMsgData[_nSize];
	fread(pdata, sizeof(stLevelUpMsgData) * _nSize, 1, fp);
	
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		stLevelUpMsgData* pTmp = new stLevelUpMsgData;

		memcpy(pTmp, &pdata[i], sizeof(stLevelUpMsgData));

		_mapdata.insert(std::make_pair(pdata[i].level, pTmp));
	}

	SAFE_ARRAY_DELETE(pdata);

	return true;
}
