/*
 *  Game.h - Game header file
 *  Game library
 *  Copyright (c) 1998, CroTeam. All rights reserved.
 *
 */
#ifndef __GAME_H
#define __GAME_H 1

#include <GameMP/PlayerSettings.h>
#include <GameMP/SessionProperties.h>
#include <Engine/Entities/PlayerCharacter.h>
#include <Engine/Network/ThreadWrapper.h>

#define GAME_SHELL_VER "V012"

#define SENSITIVITY_SLIDER_POSITIONS 25

enum ConsoleState {
	CS_OFF,
	CS_ON,
	CS_TURNINGON,
	CS_TURNINGOFF,
	CS_ONINBACKGROUND,
	//CS_TALK,		// yjpark
};

class CGameTimerHandler : public CTimerHandler
{
public:
	/* This is called every TickQuantum seconds. */
	virtual void HandleTimer(void);
};

class CButtonAction {
public:
	// default constructor
	CButtonAction();
	CListNode	ba_lnNode;
	BOOL		ba_bFirstKeyDown;
	BOOL		ba_bSecondKeyDown;
	ULONG		ba_ulFirstControl;	// first control gid
	ULONG		ba_ulSecondControl;	// second control gid
	BOOL		ba_bHidden;			// is control hidden from user
	CTString	ba_strName;
	CTString	ba_strCommandLineWhenPressed;
	CTString	ba_strCommandLineWhenReleased;
	CTString	ba_strCommandLineWhenHeld;
	// Assignment operator.
	virtual CButtonAction &operator=( CButtonAction &baOriginal );
	virtual void Read_t( CTStream &istrm );		// throw char*
	virtual void Write_t( CTStream &ostrm );	// throw char*
};

/*
 * Class containing information concerning controls system
 */
class CControls
{
public:
	// list of mounted button actions
	CListHead	ctrl_lhButtonActions;
	FLOAT		ctrl_fSensitivity;		// global sensitivity for all axes
	BOOL		ctrl_bInvertLook;		// inverts up/down looking
	BOOL		ctrl_bSmoothAxes;		// smooths axes movements

	CTimerValue	ctrl_tvLastRead;		// when these controls were last read

	// operations
	CControls(void);			// default constructor
	virtual ~CControls(void);	// default destructor
	// Assignment operator.
	virtual CControls &operator=( CControls &ctrlOriginal );

	// check if these controls use any joystick
	virtual BOOL UsesJoystick(void);
	// switches button and axis action mounters to defaults
	virtual void SwitchToDefaults(void);
	virtual void DoControlActions(void);
	virtual void CreateAction( const CPlayerCharacter &pc, CPlayerAction &paAction, BOOL bPreScan );
	virtual CButtonAction &AddButtonAction(void);
	virtual void RemoveButtonAction( CButtonAction &baButtonAction );
	virtual void Load_t( CTFileName fnFile );	// throw char *
	virtual void Save_t( CTFileName fnFile );	// throw char *
};

class CLocalPlayer
{
public:
	// Attributes
	BOOL			lp_bActive;
	INDEX			lp_iPlayer;
	CPlayerSource	*lp_pplsPlayerSource;
	UBYTE			lp_ubPlayerControlsState[2048];	// current state of player controls that are local to the player
	// Construction
	CLocalPlayer(void)
	{
		lp_pplsPlayerSource = NULL; 
		lp_bActive = FALSE; 
		memset( lp_ubPlayerControlsState, 0, sizeof(lp_ubPlayerControlsState) );
	};
};


/*
#define HIGHSCORE_COUNT 10
class CHighScoreEntry
{
public:
	CTString	hse_strPlayer;
	enum CSessionProperties::GameDifficulty	hse_gdDifficulty;
	TIME		hse_tmTime;
	INDEX		hse_ctKills;
	INDEX		hse_ctScore;
public:
	CHighScoreEntry(void);
};
*/

/*
 * Class responsible for handling game interface
 */
class CGame
{
public:
	CGame();
	~CGame();
	enum ConsoleState	gm_csConsoleState;
	enum ConsoleState	gm_csComputerState;

	CTFileName			gm_fnSaveFileName;

	CTString			gam_strCustomLevel;
	CTString			gam_strSessionName;
	CTString			gam_strJoinAddress;
	CTString			gam_strConsoleInputBuffer;

	//CHighScoreEntry		gm_ahseHighScores[HIGHSCORE_COUNT];
	//INDEX				gm_iLastSetHighScore;

//강동민 수정 시작 클로즈 준비 작업	08.10
	//CPlayerCharacter	gm_apcPlayers[8];
	//CControls			gm_actrlControls[8];
	CPlayerCharacter	gm_apcPlayers[1];
	CControls			gm_actrlControls[1];
//강동민 수정 끝 클로즈 준비 작업		08.10	
	CControls			gm_ctrlControlsExtra;
	INDEX				gm_iSinglePlayer;
	INDEX				gm_iWEDSinglePlayer;

	enum SplitScreenCfg
	{
		SSC_DEDICATED = -2,
		SSC_OBSERVER = -1,
		SSC_PLAY1 = 0,
//강동민 수정 시작 클로즈 준비 작업	08.10
		//SSC_PLAY2 = 1,
		//SSC_PLAY3 = 2,
		//SSC_PLAY4 = 3,
//강동민 수정 끝 클로즈 준비 작업		08.10
	};
	//enum SplitScreenCfg	gm_MenuSplitScreenCfg;
	enum SplitScreenCfg	gm_StartSplitScreenCfg;
	enum SplitScreenCfg	gm_CurrentSplitScreenCfg;

	// Attributes
	CGameTimerHandler	m_gthGameTimerHandler;
	BOOL				gm_bGameOn;
	BOOL				gm_bFirstLoading;	// set by serioussam.exe to notify first loading

	// network provider itself
	CNetworkProvider	gm_npNetworkProvider;
	// network provider's description
	CTString			gm_strNetworkProvider;

	// controls that are local to each player
	SLONG	gm_slPlayerControlsSize;
	void	*gm_pvGlobalPlayerControls;

	// index of local player
	// (-1) if not active
	// 로컬 플레이어의 인덱스.
//강동민 수정 시작 로그인 변경 작업	08.10
	INDEX	gm_aiMenuLocalPlayers[1];
	//INDEX	gm_aiStartLocalPlayers[4];
	//INDEX	gm_aiMenuLocalPlayers[1];
	INDEX	gm_aiStartLocalPlayers[1];

	// players that are currently playing on local machine (up to 4)
	// 로컬에서 현재 실행되는 플레이어...
//강동민 수정 시작 로그인 변경 작업	08.10
	// NOTE : Save_t와 Load_t에서 읽어들이는 부분이 있어서 제거하지 못했음.
	CLocalPlayer	gm_lpLocalPlayers[1];

	ULONG			m_oldTime;

	// Operations
	void InitInternal(void);
	void EndInternal(void);
	BOOL StartProviderFromName(void);
	void SetupLocalPlayers( void);
	BOOL AddPlayers(void);
	//SLONG PackHighScoreTable(void);
	//void RecordHighScore(void);
	//void UnpackHighScoreTable( SLONG slSize );
	void SaveThumbnail( const CTFileName &fnm );
	//CTFileName GetQuickSaveName( BOOL bSave );
	void GameHandleTimer(void);

	HANDLE GetUpdateHandle() { return gmUpdateHandle; }
	virtual void LoadPlayersAndControls(void);
	virtual void SavePlayersAndControls(void);
	virtual void Load_t(void);
	virtual void Save_t(void);

	// set properties for a quick start session
	virtual void SetQuickStartSession( CSessionProperties &sp );
	// set properties for a single player session
	virtual void SetSinglePlayerSession( CSessionProperties &sp );
	// set properties for a multiplayer session
	virtual void SetMultiPlayerSession( CSessionProperties &sp );

	// game loop functions
#define GRV_SHOWEXTRAS  (1L<<0)		// add extra stuff like console, weapon, pause
//강동민 수정 시작 클로즈 준비 작업	08.10
	virtual void GameRedrawView( CDrawPort *pdpDrawport, ULONG ulFlags, CEntity* pViewer = NULL );
//강동민 수정 끝 클로즈 준비 작업		08.10
	virtual void GameMainLoop(void);

	// console functions
	virtual void ConsoleKeyDown( MSG msg );
	virtual void ConsoleChar( MSG msg );
	virtual void ConsoleRender( CDrawPort *pdpDrawport );
	virtual void ConsolePrintLastLines( CDrawPort *pdpDrawport );

	// get default description for a game (for save games/demos)
	virtual CTString GetDefaultGameDescription( BOOL bWithInfo );
//강동민 수정 시작 다중 공격 작업	08.22
	virtual void CaptureScreen();
//강동민 수정 끝 다중 공격 작업		08.22

	// game start/end functions
	virtual BOOL PreNewGame();	//0524 kwon
	virtual BOOL NewGame( const CTString &strSessionName, const CTFileName &fnWorld,
							class CSessionProperties &sp );
//	virtual BOOL JoinGame( CNetworkSession &session );
	//virtual BOOL LoadGame( const CTFileName &fnGame );
	//virtual BOOL SaveGame( const CTFileName &fnGame );
	virtual void StopGame(void);
	//virtual BOOL StartDemoPlay( const CTFileName &fnDemo );
	//virtual BOOL StartDemoRec( const CTFileName &fnDemo );
	//virtual void StopDemoRec(void);
	virtual INDEX GetPlayersCount(void);
	virtual INDEX GetLivePlayersCount(void);

	// printout and dump extensive demo profile report
	virtual CTString DemoReportFragmentsProfile( INDEX iRate );
	virtual CTString DemoReportAnalyzedProfile(void);

	// functions called from world editor
	virtual void Initialize( const CTFileName &fnGameSettings, BOOL bDummy );
	virtual void End(void);
	virtual void QuickTest( const CTFileName &fnMapName, CDrawPort *pdpDrawport, CViewPort *pvpViewport );
//강동민 수정 시작 클로즈 준비 작업	08.10
	virtual void LoginGame( const CTFileName &fnMapName);
//강동민 수정 끝 클로즈 준비 작업		08.10

	// interface rendering functions
	virtual void LCDInit(void);
	virtual void LCDEnd(void);
	virtual void LCDPrepare( FLOAT fFade );
	virtual void LCDSetDrawport( CDrawPort *pdp );
	virtual void LCDDrawBox( PIX pixUL, PIX pixDR, PIXaabbox2D &box, COLOR col );
	virtual void LCDScreenBox( COLOR col );
	virtual void LCDScreenBoxOpenLeft( COLOR col );
	virtual void LCDScreenBoxOpenRight( COLOR col );
	virtual void LCDRenderClouds1(void);
	virtual void LCDRenderClouds2(void);
			void LCDRenderCloudsForComp(void);
			void LCDRenderCompGrid(void);
	virtual void LCDRenderGrid(void);
	virtual void LCDDrawPointer( PIX pixI, PIX pixJ );
	virtual COLOR LCDGetColor( COLOR colDefault, const char *strName );
	virtual COLOR LCDFadedColor( COLOR col );
	virtual COLOR LCDBlinkingColor( COLOR col0, COLOR col1 );

	// menu interface functions
	virtual void MenuPreRenderMenu( const char *strMenuName );
	virtual void MenuPostRenderMenu( const char *strMenuName );

	virtual void GameRedrawCursor(CDrawPort *pdpDrawPort, ULONG ulFlags);

	virtual void SetHud_Stats(INDEX stats);

private:
	cThreadWrapper*	gmUpdateInputState; // 프로세스 검사를 위한 스레드 랩퍼 클래스
	HANDLE			gmUpdateHandle;		// 프로세스 검사 이벤트 핸들

	static UINT WINAPI UpdateThreadFunction(void *parameter);
};

#endif
