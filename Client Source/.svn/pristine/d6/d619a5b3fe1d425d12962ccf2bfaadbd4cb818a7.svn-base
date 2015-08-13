#include "StdH.h"
#include <Engine/GameStageManager/StageMgr.h>
#include "StageLoading.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UISingleBattle.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Network/LevelChange.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/Interface/UIOption.h>


#include <Engine/GameState.h>
#include <Engine/Loading.h>

void CStageLoading::Init()
{
	CLoadingImage::getSingleton()->InitLoadingData();
	CUIManager* pUIManager = CUIManager::getSingleton();

	_pNetwork->TogglePause();

	if(CZoneInfo::getSingleton()->GetZoneType(g_slZone) == ZONE_SDUNGEON)
	{
		CPrintF("===Start Single Mode===\n");
		_pNetwork->m_bSingleMode = TRUE;
		_pNetwork->wo_dwEnemyCount		 = 0;
		_pNetwork->wo_dwKilledEnemyCount = 0;
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SingleModeOn();
	}
	else
	{
		CPrintF("===End Single Mode===\n");
		_pNetwork->m_bSingleMode = FALSE;
		pUIManager->GetSingleBattle()->Close();
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->FieldModeOn();
	}

	_pNetwork->ChangeLevel(CZoneInfo::getSingleton()->GetZoneWldFile(g_slZone), FALSE, 0);
	_pNetwork->SetLoadingZone( g_slZone );
	pUIManager->SetUIGameState();
	pUIManager->GetGame()->gm_bGameOn = FALSE;
	_pGameState->GetGameMode() = CGameState::GM_NETWORK;

	EPreLevelChange ePreChange;
	ePreChange.iUserData = _pNetwork->ga_iNextLevelUserData;
	_pNetwork->ga_sesSessionState.SendLevelChangeNotification(ePreChange);
	CEntity::HandleSentEvents();
	_pNetwork->ChangeLevel_internal();
	EPostLevelChange ePostChange;
	ePostChange.iUserData = _pNetwork->ga_iNextLevelUserData;
	_pNetwork->ga_sesSessionState.SendLevelChangeNotification(ePostChange);
	CEntity::HandleSentEvents();

	_lphCurrent=LCP_NOCHANGE;

	for(std::vector<CEntity*>::iterator iter = _pNetwork->ga_World.m_vectorPreCreateNPC.begin(); iter != _pNetwork->ga_World.m_vectorPreCreateNPC.end(); ++iter)
	{
		(*iter)->Reinitialize();
	}
	_pNetwork->ga_World.m_vectorPreCreateNPC.clear();

	if (g_slZone == 42)	// 임시 설정.
	{
		_pNetwork->SetRvrZone(TRUE);
		ACTORMGR()->initSyndiMark();
	}
	else
		_pNetwork->SetRvrZone(FALSE);

	m_bGameStart = false;

	pUIManager->GetOption()->UpdateTerMul();
}

void CStageLoading::Run()
{
	if (CLoadingImage::getSingleton()->GetLoadingState() == CLoadingImage::ePS_END)
	{
		// 로딩이 완료되면, Stage 전환
		if (m_bGameStart == false && STAGEMGR()->GetCurStage() == eSTAGE_ZONELOADING)
		{
			UIMGR()->GetGame()->gm_bGameOn = TRUE;

			if (_pNetwork->GetDelivery() == false)
			{
				_pNetwork->SetDelivery(true);
				_pNetwork->SendGameStart();
			}

			_pNetwork->SendGameStart();
			m_bGameStart = true;
		}
	}
}

void CStageLoading::Release()
{
	CLoadingImage::getSingleton()->Hide(TRUE);
}
