#include "StdH.h"
#include <Engine/GameStageManager/StageMgr.h>
#include "StageGameEnd.h"
#include <Engine/GameState.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UISystemMenu.h>
#include <Engine/Contents/function/gps.h>
#include <Engine/GameDataManager/GameDataManager.h>

void CStageGameEnd::Init()
{
	_pGameState->GetGameMode() = CGameState::GM_RESTART;
}

void CStageGameEnd::Release()
{
	_pGameState->SetRestartGameValue(FALSE);
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->GetSystemMenu()->Restart_Internal();
	GAMEDATAMGR()->GetGPS()->clear();
}
