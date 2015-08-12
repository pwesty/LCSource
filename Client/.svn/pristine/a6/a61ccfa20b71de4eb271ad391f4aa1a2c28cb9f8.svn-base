#include "stdh.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Base/Stream.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/Action.h>
#include <Engine/Base/CTString.h>
#include <Engine/Secure/FileSecure.h>	// [2012/07/18 : Sora]  파일 보안코드 추가

//[sora] 미번역 스트링 index 표시
void CAction::SetNoTranslate()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
		Action_strName.PrintF("[%d] : action name", index);

	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
		Action_strDesc.PrintF("[%d] : item desc", index);
}

void CAction::ClearNoTranslate()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
		Action_strName = "";

	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
		Action_strDesc = "";
}

bool CAction::loadEx(const char* FileName)
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

	stAction* pdata = new stAction[_nSize];
	fread(pdata, sizeof(stAction) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CAction* ptmp = new CAction;
		memcpy(ptmp, &pdata[i], sizeof(stAction));

		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new CAction; // 더미데이타 생성
	memset(m_dummy, 0, sizeof(stAction));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}
