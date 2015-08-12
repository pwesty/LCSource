#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/WildPetData.h>
#include <Engine/Network/CNetwork.h>

bool CWildPetData::loadEx(const char* FileName)
{	
	FILE*	fp = NULL;

	fp = fopen(FileName, "rb");

	if (fp == NULL)
		return false;

	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stApet* pdata = new stApet[_nSize];
	fread(pdata, sizeof(stApet) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CWildPetData* ptmp = new CWildPetData;
		memcpy(ptmp, &pdata[i], sizeof(stApet));

		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new CWildPetData; // 더미데이타 생성
	memset(m_dummy, 0, sizeof(stApet));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}
