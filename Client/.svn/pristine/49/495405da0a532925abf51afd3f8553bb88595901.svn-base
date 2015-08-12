#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/NpcHelp.h>
//강동민 수정 시작 테스트 클라이언트 작업	06.09
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/World/World.h>
#include <Engine/Interface/UIManager.h>
//강동민 수정 끝 테스트 클라이언트 작업		06.09
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &apShopData - 
//			FileName - 
// Output : int
//-----------------------------------------------------------------------------
bool CNpcHelp::loadEx(const char* FileName)
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

	stNpcHelp* pdata = new stNpcHelp[_nSize];
	fread(pdata, sizeof(stNpcHelp) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CNpcHelp* ptmp = new CNpcHelp;
		memcpy(ptmp, &pdata[i], sizeof(stNpcHelp));
		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new CNpcHelp; // 더미데이타 생성
	memset(m_dummy, 0, sizeof(stNpcHelp));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}

bool CMobHelp::loadEx( const char* FileName )
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

	stNpcHelp* pdata = new stNpcHelp[_nSize];
	fread(pdata, sizeof(stNpcHelp) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CMobHelp* ptmp = new CMobHelp;
		memcpy(ptmp, &pdata[i], sizeof(stNpcHelp));
		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new CMobHelp; // 더미데이타 생성
	memset(m_dummy, 0, sizeof(stNpcHelp));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}
