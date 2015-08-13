#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/MissionCase.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/Interface/UIManager.h>

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &apShopData - 
//			FileName - 
// Output : int
//-----------------------------------------------------------------------------
bool CMissionCase::loadEx(const char* FileName)
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

	stMonsterCombo* pdata = new stMonsterCombo[_nSize];
	fread(pdata, sizeof(stMonsterCombo) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CMissionCase* ptmp = new CMissionCase;
		memcpy(ptmp, &pdata[i], sizeof(stMonsterCombo));

		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
		else
		{
			_vecdata.push_back(ptmp);
		}
	}

	m_dummy = new CMissionCase; // 더미데이타 생성
	memset(m_dummy, 0, sizeof(stMonsterCombo));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}