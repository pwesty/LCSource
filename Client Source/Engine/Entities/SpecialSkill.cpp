#include "stdh.h"
#include <string>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/SpecialSkill.h>
#include <Engine/Interface/UIManager.h>

//[sora] 미번역 스트링 index 표시
void CSpecialSkill::SetNoTranslate()
{
	char buff[MAX_PATH];
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
	{
		memset(buff, 0, MAX_PATH);
		sprintf( buff, "[%d] : special skill name", getindex() );
		name = buff;
	}
	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
	{
		memset(buff, 0, MAX_PATH);
		sprintf( buff, "[%d] : special skill desc", getindex() );
		desc = buff;
	}
}

void CSpecialSkill::ClearNoTranslate()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
		name = "";
	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
		desc = "";
}

bool CSpecialSkill::loadEx(const char* str_path)
{	
	FILE*	fp = NULL;

	fp = fopen(str_path, "rb");

	if (fp == NULL)
		return false;
	
	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stSpecailSkill* pdata = new stSpecailSkill[_nSize];
	fread(pdata, sizeof(stSpecailSkill) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CSpecialSkill* ptmp = new CSpecialSkill;
		memcpy(ptmp, &pdata[i], sizeof(stSpecailSkill));
		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new CSpecialSkill; // 더미데이타 생성
	memset(m_dummy, 0, sizeof(stSpecailSkill));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}
