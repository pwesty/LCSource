#include "StdAfx.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/GameState.h>
#include <Engine/LoginJobInfo.h>

extern CGame *_pGame;

extern INDEX gam_iQuickStartDifficulty;
extern INDEX gam_iQuickStartMode;
extern INDEX gam_iStartDifficulty;
extern INDEX gam_iStartMode;

// initialize game and load settings
void CGame::Initialize(const CTFileName &fnGameSettings, BOOL bDummy )
{
	gm_fnSaveFileName = fnGameSettings;
	InitInternal();
}

// save settings and cleanup
void CGame::End(void)
{
	EndInternal();
}

// automaticaly manage input enable/disable toggling
static BOOL _bInputEnabled = FALSE;
void UpdateInputEnabledState(CViewPort *pvp)
{
	// input should be enabled if application is active
	// and no menu is active and no console is active
	BOOL bShouldBeEnabled = _pGame->gm_csConsoleState==CS_OFF && _pGame->gm_csComputerState==CS_OFF;

	// if should be turned off
	if (!bShouldBeEnabled && _bInputEnabled) {
		// disable it
		_pInput->DisableInput();

		// remember new state
		_bInputEnabled = FALSE;
	}

	// if should be turned on
	if (bShouldBeEnabled && !_bInputEnabled) {
		// enable it
		_pInput->EnableInput(pvp);

		// remember new state
		_bInputEnabled = TRUE;
	}
}

// automaticaly manage pause toggling
void UpdatePauseState(void)
{
	BOOL bShouldPause = 
		 _pGame->gm_csConsoleState ==CS_ON || _pGame->gm_csConsoleState ==CS_TURNINGON || _pGame->gm_csConsoleState ==CS_TURNINGOFF ||
		 _pGame->gm_csComputerState==CS_ON || _pGame->gm_csComputerState==CS_TURNINGON || _pGame->gm_csComputerState==CS_TURNINGOFF;

	_pNetwork->SetLocalPause(bShouldPause);
}

//-----------------------------------------------------------------------------
// Purpose: run a quicktest game from within editor
// Input  : &fnMapName - 
//			*pdp - 
//			*pvp - 
//-----------------------------------------------------------------------------
void CGame::QuickTest(const CTFileName &fnMapName, CDrawPort *pdp, CViewPort *pvp)
{
	UINT uiMessengerMsg = RegisterWindowMessage("Croteam Messenger: Incoming Message");

	// quick start game with the world
	gm_strNetworkProvider = "Local";
	gm_aiStartLocalPlayers[0] = gm_iWEDSinglePlayer;
	gm_CurrentSplitScreenCfg = CGame::SSC_PLAY1;

	// set properties for a quick start session
	CSessionProperties sp;
	SetQuickStartSession(sp);
	
	//0625 kwon
	// 월드 에디터 상에서는 타이탄으로 플레이 하도록 함.
	CPlayerCharacter &pc = gm_apcPlayers[0];
	pc.pc_iPlayerType = 0; //TITAN

	// start the game
	if(!_pGame->PreNewGame() || !NewGame( fnMapName, fnMapName, sp)) 
		return;

	CEntity* penPlEntity			= CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	CPlayerEntity *penPlayerEntity	= (CPlayerEntity*) penPlEntity;	
	penPlayerEntity->CharacterChanged(pc);

	// enable input
	_pInput->EnableInput(pvp);

	// initialy, game is running
	BOOL bRunning = TRUE;
	// while it is still running
	while( bRunning)
	{
		// while there are any messages in the message queue
		MSG msg;
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE)) {
			// if it is not a mouse message
			if( !(msg.message>=WM_MOUSEFIRST && msg.message<=WM_MOUSELAST)) {
				// if not system key messages
				if( !(msg.message==WM_KEYDOWN && msg.wParam==VK_F10
						||msg.message==WM_SYSKEYDOWN)) {
					// dispatch it
					TranslateMessage(&msg);
				}
				// if paint message
				if( msg.message==WM_PAINT) {
					// dispatch it
					DispatchMessage(&msg);
				}
			}

			// if should stop
			if ((msg.message==WM_QUIT)
				||(msg.message==WM_CLOSE)
				||(msg.message==WM_KEYDOWN && msg.wParam==VK_ESCAPE)
				||(msg.message==WM_ACTIVATE)
				||(msg.message==WM_CANCELMODE)
				//||(msg.message==WM_KILLFOCUS)
				||(msg.message==WM_ACTIVATEAPP)) {
				// stop running
				bRunning = FALSE;
				break;
			}

			if (msg.message==uiMessengerMsg)
			{
				if(!_pNetwork->IsPaused()) 
				{
					// pause it
					_pNetwork->TogglePause();
				}
				char *pachrTemp=getenv("TEMP");
				if( pachrTemp!=NULL)
				{
					FILE *pfileMessage=fopen(CTString(pachrTemp)+"Messenger.msg","r");
					if( pfileMessage!=NULL)
					{
						char achrMessage[1024];
						char *pachrMessage=fgets( achrMessage, 1024-1, pfileMessage);
						if( pachrMessage!=NULL)
						{
							CPrintF("%s",pachrMessage);
						}
					}
					fclose(pfileMessage);
				}
			}

			// if pause pressed
			if (msg.message==WM_KEYDOWN && msg.wParam==VK_PAUSE && 
				_pGame->gm_csConsoleState==CS_OFF && _pGame->gm_csComputerState==CS_OFF) {
				// toggle pause
				_pNetwork->TogglePause();
			}
			if(msg.message==WM_KEYDOWN && 
				(MapVirtualKey(msg.wParam, 0)==41 // scan code for '~'
				||msg.wParam==VK_F1)) {
				if (_pGame->gm_csConsoleState==CS_OFF || _pGame->gm_csConsoleState==CS_TURNINGOFF) {
					_pGame->gm_csConsoleState = CS_TURNINGON;
				} else {
					_pGame->gm_csConsoleState = CS_TURNINGOFF;
				}
			}
			/*extern INDEX con_bTalk;		// yjpark
			if (con_bTalk && _pGame->gm_csConsoleState==CS_OFF) {
				con_bTalk = FALSE;
				_pGame->gm_csConsoleState = CS_TALK;
			}*/
			if (msg.message==WM_KEYDOWN) 
			{
				ConsoleKeyDown(msg);
				/*if (_pGame->gm_csConsoleState!=CS_ON) {
					ComputerKeyDown(msg);
				}*/
			}/* else if (msg.message==WM_KEYUP) {		// yjpark
				// special handler for talk (not to invoke return key bind)
				if( msg.wParam==VK_RETURN && _pGame->gm_csConsoleState==CS_TALK) _pGame->gm_csConsoleState = CS_OFF;
			}*/ else if (msg.message==WM_CHAR) 
			{
				ConsoleChar(msg);
			}
		}

		UpdatePauseState();
		UpdateInputEnabledState(pvp);
			
		/*
		// if playing a demo and it is finished
		if (_pNetwork->IsDemoPlayFinished()) {
			// stop running
			bRunning = FALSE;
		}
		*/

		// do the main game loop
		GameMainLoop();
		
		// redraw the view
		pdp->SetAsCurrent();
		// if current view preferences will not clear the background, clear it here
		if( _wrpWorldRenderPrefs.GetPolygonsFillType() == CWorldRenderPrefs::FT_NONE) {
			// clear background
			pdp->Fill(C_BLACK| CT_OPAQUE);
			pdp->FillZBuffer(ZBUF_BACK);
		}
		// redraw view
		if (_pGame->gm_csComputerState != CS_ON) {
			GameRedrawView(pdp, (_pGame->gm_csConsoleState==CS_ON)?0:GRV_SHOWEXTRAS);
		}
		
		//0625 kwon
		_pInput->m_bTcpIp = TRUE;
		_pGame->GameRedrawCursor( pdp, (_pGame->gm_csConsoleState==CS_ON)?0:GRV_SHOWEXTRAS);

		//ComputerRender(pdp);
		ConsoleRender(pdp);
		// show it
		pvp->SwapBuffers();
	}

	if (_pGame->gm_csConsoleState != CS_OFF) {
		_pGame->gm_csConsoleState = CS_TURNINGOFF;
	}
	if (_pGame->gm_csComputerState != CS_OFF) {
		_pGame->gm_csComputerState = CS_TURNINGOFF;
		cmp_ppenPlayer = NULL;
	}

	_pInput->m_bTcpIp = FALSE;
	_pInput->DisableInput();
	StopGame();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &fnMapName - 
//-----------------------------------------------------------------------------
void CGame::LoginGame(const CTFileName &fnMapName)
{
	// set properties for a quick start session
	gm_strNetworkProvider = "Local";
	CSessionProperties sp;
	//SetSinglePlayerSession(sp);	

	SetSinglePlayerSession(sp);
	//SetMultiPlayerSession(sp);

	if (_pNetwork->bMoveCharacterSelectUI == FALSE)
		CLoginJobInfo::getSingleton()->LoginModelCreate();

	// 월드 에디터 상에서는 타이탄으로 플레이 하다록 함.
	CPlayerCharacter &pc = gm_apcPlayers[0];
	gm_CurrentSplitScreenCfg = CGame::SSC_PLAY1;
	pc.pc_iPlayerType = TITAN; //TITAN

	// start the game
	if(//!_pGame->PreNewGame() ||			// 소켓 생성 및 연결 시도함.
		!NewGame( fnMapName, fnMapName, sp)) 
	{
		return;
	}

	extern BOOL g_bAutoLogin;
	extern BOOL g_bAutoRestart;

	if(g_bAutoLogin)	
	{
		if(PreNewGame())
		{
			extern CTString g_nmID;
			extern CTString g_nmPW;
			extern CTString g_nmCID;
			_pNetwork->m_strUserID = g_nmID;
			_pNetwork->m_strUserPW = g_nmPW;
			_pNetwork->m_strUserCID = g_nmCID;
		}
	}

	// NOTE : 메인 루프를 타도록 하기 위해서 게임 모드를 변경함.
	_pGameState->GetGameMode() = CGameState::GM_INTRO;
}
