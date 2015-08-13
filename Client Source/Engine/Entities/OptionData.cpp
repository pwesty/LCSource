#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/OptionData.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Help/LoadLod.h>

//-----------------------------------------------------------------------------
// Purpose: 아이템 데이터를 파일로부터 읽어들입니다.
// Input  : &apOptionData - 아이템 목록이 저장될 배열.
//			FileName - 파일명.
// Output : 	static int
//-----------------------------------------------------------------------------
bool COptionData::loadEx(const char* FileName)
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

	stOption* pdata = new stOption[_nSize];
	fread(pdata, sizeof(stOption) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		COptionData* ptmp = new COptionData;
		memcpy(ptmp, &pdata[i], sizeof(stOption));
		if (_mapdata.insert(std::make_pair(ptmp->type, ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new COptionData; // 더미데이타 생성
	memset(m_dummy, 0, sizeof(stOption));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 호칭 데이터를 파일로부터 읽어들입니다.
// Input  : &aNickNameData - 호칭 목록이 저장될 배열.
//			FileName - 파일명.
// Output : 	static int (호칭 총 갯수)
//-----------------------------------------------------------------------------
bool CNickNameData::loadEx(const char* FileName)
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

	stTitle* pdata = new stTitle[_nSize];
	fread(pdata, sizeof(stTitle) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CNickNameData* ptmp = new CNickNameData;
		memcpy(ptmp, &pdata[i], sizeof(stTitle));
		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new CNickNameData; // 더미데이타 생성
	memset(m_dummy, 0, sizeof(stTitle));
	
	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}