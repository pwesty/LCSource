#include "StdH.h"
#include <Engine/GameStageManager/StageMgr.h>
#include "StageSelChar.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/GameState.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Contents/Login/UICharacterSelect.h>

void CStageSelChar::Init()
{
	_pGameState->BackToSelChar();
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->SetUIGameState();
	pUIManager->GetCharacterSelect()->OpenUI();
}

void CStageSelChar::Release()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->GetCharacterSelect()->CloseUI();
}
