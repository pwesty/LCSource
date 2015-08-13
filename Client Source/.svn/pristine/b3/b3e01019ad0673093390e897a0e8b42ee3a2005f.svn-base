#include "StdH.h"
#include <Engine/GameStageManager/StageMgr.h>
#include "StageCreateChar.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/GameState.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Contents/Login/UICharacterCreateNew.h>

void CStageCreateChar::Init()
{
	_pGameState->SetCameraByJob(0);
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->SetUIGameState();
	pUIManager->GetCreateChar()->OpenUI();
}

void CStageCreateChar::Release()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->GetCreateChar()->CloseUI();
}
