// Game.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Game.h"
#include <direct.h> // for _mkdir()
#include <sys/timeb.h>
#include <time.h>
#include <locale.h>
#include <io.h>
#include <Engine/Base/Profiling.h>
#include <Engine/Base/Statistics.h>
#include <Engine/CurrentVersion.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Network/Web.h>
#include <tlhelp32.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Object/ActorMgr.h>
#include "LCDDrawing.h"

extern FLOAT con_fHeightFactor = 0.5f;
extern FLOAT con_tmLastLines   = 5.0f;
//extern INDEX con_bTalk = 0;		// yjpark
//CTimerValue _tvMenuQuickSave(0I64);

// used filenames
//안태훈 수정 시작	//(5th Closed beta)(0.2)
#ifdef KALYDO
CTFileName fnmPersistentSymbols = CTString("ps.dat");
#else
CTFileName fnmPersistentSymbols = CTString("Data\\etc\\ps.dat");
#endif
//안태훈 수정 끝	//(5th Closed beta)(0.2)
//CTFileName fnmStartupScript     = CTString("Data\\Etc\\Game_startup.ini");
//CTFileName fnmConsoleHistory    = CTString("Data\\Etc\\ConsoleHistory.txt");
//CTFileName fnmCommonControls    = CTString("Data\\Etc\\Common.ctl");

// force dependency for player class
DECLARE_CTFILENAME( fnmPlayerClass, "Classes\\Player.ecl");

//#define MAX_HIGHSCORENAME 16
//#define MAX_HIGHSCORETABLESIZE ((MAX_HIGHSCORENAME+1+sizeof(INDEX)*4)*HIGHSCORE_COUNT)*2
//UBYTE _aubHighScoreBuffer[MAX_HIGHSCORETABLESIZE];
//UBYTE _aubHighScorePacked[MAX_HIGHSCORETABLESIZE];

// controls used for all commands not belonging to any particular player
static CControls _ctrlCommonControls;

// array for keeping all frames' times
static CStaticStackArray<TIME>  _atmFrameTimes;
static CStaticStackArray<INDEX> _actTriangles;  // world, model, particle, total

// float for current input control value
static FLOAT ctl_fInputValue = 0;
// time passed since last input read
static FLOAT ctl_fFrameTime  = 1;
// one and only Game object
extern CGame *_pGame = NULL;

extern "C" __declspec (dllexport) CGame *GAME_Create(void)
{
	_pGame = new CGame;

	return _pGame;
}

extern "C" __declspec (dllexport) void GAME_Destroy(void)
{
	delete _pGame;
	_pGame = NULL;
}

// recorded profiling stats
static CTimerValue _tvDemoStarted;
static CTimerValue _tvLastFrame;
static CTString _strProfile;
static BOOL  _bDumpNextTime = FALSE;
static BOOL  _bStartProfilingNextTime = FALSE;
static BOOL  _bProfiling = FALSE;
static INDEX _ctProfileRecording = 0;
//static FLOAT gam_iRecordHighScore = -1.0f;


extern FLOAT gam_afAmmoQuantity[5]        = {2.0f,  2.0f,  1.0f, 1.0f , 2.0f };
extern FLOAT gam_afDamageStrength[5]      = {0.25f, 0.5f,  1.0f, 1.5f , 2.0f };
extern FLOAT gam_afEnemyAttackSpeed[5]    = {0.75f, 0.75f, 1.0f, 2.0f , 2.0f };
extern FLOAT gam_afEnemyMovementSpeed[5]  = {1.0f , 1.0f , 1.0f, 1.25f, 1.25f};
extern FLOAT gam_fManaTransferFactor = 0.5f;
extern FLOAT gam_fExtraEnemyStrength          = 0;
extern FLOAT gam_fExtraEnemyStrengthPerPlayer = 0;
//extern INDEX gam_iCredits = -1;   // number of credits for respawning
extern FLOAT gam_tmSpawnInvulnerability = 3;
//extern INDEX gam_iScoreLimit = 1000;
extern INDEX gam_iFragLimit = 20;
extern INDEX gam_iTimeLimit = 0;
//extern INDEX gam_bWeaponsStay = TRUE;
//extern INDEX gam_bAmmoStays = TRUE;
//extern INDEX gam_bHealthArmorStays = TRUE;
//extern INDEX gam_bAllowHealth = TRUE;
//extern INDEX gam_bAllowArmor = TRUE;
//extern INDEX gam_bInfiniteAmmo = FALSE;
extern INDEX gam_bRespawnInPlace = TRUE;
extern INDEX gam_bPlayEntireGame = TRUE;
//extern INDEX gam_bFriendlyFire = FALSE;
extern INDEX gam_ctMaxPlayers = 8;
extern INDEX gam_bWaitAllPlayers = FALSE;
//extern INDEX gam_iInitialMana = 100;
//extern INDEX gam_bQuickLoad = FALSE;
//extern INDEX gam_bQuickSave = FALSE;
//extern INDEX gam_iQuickSaveSlots = 8;

extern INDEX gam_iQuickStartDifficulty = 1;
extern INDEX gam_iQuickStartMode = 0;
extern INDEX gam_bQuickStartMP = 0;

extern INDEX gam_bEnableAdvancedObserving = 0;
extern INDEX gam_iObserverConfig = 0;
extern INDEX gam_iObserverOffset = 0;

extern INDEX gam_iStartDifficulty = 1;
extern INDEX gam_iStartMode = 0;
//extern CTString gam_strGameSpyExtras = "";

//extern INDEX gam_iBlood = 2;     // 0=none, 1=green, 2=red, 3=hippie
//extern INDEX gam_bGibs  = TRUE;   
extern INDEX gam_iBlood;
extern INDEX gam_bGibs;

extern INDEX gam_iJpgScreenShot = 1;	// 0 = TGA, 1 = JPG

extern INDEX gam_bUseExtraEnemies = TRUE;

#ifdef _SHOW_DEBUG_STATUS
INDEX hud_iStats = 3;
static INDEX hud_iFPSGraphSpeed = 1;
static INDEX hud_iFPSGraphMax = 400;
static INDEX hud_bShowTime = TRUE;
#else
INDEX hud_iStats = 0;
static INDEX hud_iFPSGraphSpeed = 4;
static INDEX hud_iFPSGraphMax = 400;
static INDEX hud_bShowTime = FALSE;
#endif	// _SHOW_DEBUG_STATUS

static INDEX hud_bShowClock = FALSE;
static INDEX hud_bShowNetGraph = FALSE;
static INDEX hud_bShowResolution = FALSE;
static INDEX hud_bShowPauseText  = TRUE;
static INDEX dem_bOSD         = FALSE;
static INDEX dem_bPlay        = FALSE;
static INDEX dem_bPlayByName  = FALSE;
static INDEX dem_bProfile     = FALSE;
static INDEX dem_iProfileRate = 5;
static FLOAT dem_fMotionBlurStrength = 0.5f;  // 0.1=min, 1=max
static CTString dem_strPostExec = "";

static INDEX ctl_iCurrentPlayerLocal = -1;
static INDEX ctl_iCurrentPlayer = -1;
static FLOAT gam_fChatSoundVolume = 0.25f;

extern BOOL _bUserBreakEnabled = FALSE;



// for motion blur animation capturing
static UWORD *_puwAccBuffer = NULL;
static PIX _pixAccSize = 0;

void AccumulateFrame( CImageInfo *pii, const INDEX ctInterFrames, const BOOL bRootFrame)
{
	ASSERT( pii!=NULL && ctInterFrames>0);
	ASSERT( pii->ii_BitsPerPixel==24);
	ASSERT( _pixAccSize == pii->ii_Width*pii->ii_Height);
	dem_fMotionBlurStrength = Clamp( dem_fMotionBlurStrength, 0.1f, 1.0f);
	UBYTE *pubImage = pii->ii_Picture;
	const FLOAT fRootStrength = ctInterFrames*(1-dem_fMotionBlurStrength) +dem_fMotionBlurStrength;
	const FLOAT fStrength  = bRootFrame ? fRootStrength : dem_fMotionBlurStrength;
	const SLONG slStrength = FloatToInt(fStrength*65536); // fix-int
	for( INDEX i=0; i<_pixAccSize*3; i++) {
		_puwAccBuffer[i] += (pubImage[i]*slStrength) >>16;
	}
}


void FlushFrames( CImageInfo *pii, const INDEX ctInterFrames)
{
	// if there's a destination image
	if( pii!=NULL) {
		// flush accumulation buffer there
		ASSERT( ctInterFrames>1);
		ASSERT( _pixAccSize == pii->ii_Width*pii->ii_Height);
		UBYTE *pubImage = pii->ii_Picture;
		const SLONG sl1oFrames = FloatToInt(65536.0f/ctInterFrames); // fix-int
		for( INDEX i=0; i<_pixAccSize*3; i++) {
			pubImage[i] = (UBYTE)((_puwAccBuffer[i]*sl1oFrames)>>16);
		 _puwAccBuffer[i] = 0;
		}
	} else {
		// just reset accumulation buffer
		ASSERT(_pixAccSize>0);
		for( INDEX i=0; i<_pixAccSize*3; i++) _puwAccBuffer[i] = 0;
	}
}



// make sure that console doesn't show last lines if not playing in network
void MaybeDiscardLastLines(void)
{
	// if not in network
	if (!_pNetwork->IsNetworkEnabled()) {
		// don't show last lines on screen after exiting console
		CON_DiscardLastLineTimes();
	}
}

// [100122: selo] 윈도우 핸들을 찾아 프로세스 찾기 루틴
ULONG ProcIDFromWnd(HWND hwnd) // 윈도우 핸들로 프로세스 아이디 얻기  
{  
  ULONG idProc;  
  GetWindowThreadProcessId( hwnd, &idProc );  
  return idProc;  
}

// [100122: selo] 위 함수와 연계되는 함수 
HWND GetInputH(ULONG pid) // 프로세스 아이디로 윈도우 핸들 얻기  
{  
  HWND tempHwnd = FindWindow(NULL,NULL); // 최상위 윈도우 핸들 찾기  
  
  while( tempHwnd != NULL )  
  {  
    if( GetParent(tempHwnd) == NULL ) // 최상위 핸들인지 체크, 버튼 등도 핸들을 가질 수 있으므로 무시하기 위해  
      if( pid == ProcIDFromWnd(tempHwnd) )  
        return tempHwnd;  
    tempHwnd = GetWindow(tempHwnd, GW_HWNDNEXT); // 다음 윈도우 핸들 찾기  
  }  
  return NULL;  
}

// [100122: selo] 해킹 프로세스 찾기
BOOL InputState(void)
{
	HANDLE      hInputSnap = NULL;
	BOOL		Return       = FALSE;
	
	PROCESSENTRY32 pe32         = {0};
	
	char buf[256];
	
	hInputSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);    
	
	if (hInputSnap == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	
	if (Process32First(hInputSnap, &pe32))
	{   
		do
		{
			HANDLE hInput;                        
			// Get the actual priority class.            
			hInput = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
			
			if( hInput != NULL )
			{
				HWND hWnd = GetInputH(pe32.th32ProcessID); // GetWinHandle --> GetInputH

#if !defined (G_RUSSIA)
				GetClassName((HWND)hWnd, buf, 256);	
				if( 0 == strncmp(buf, "AutoIt", 6) )
				{
					Return = TRUE;	
					CPrintF("나는 행유져");
				}	
#else
				GetWindowText((HWND)hWnd, buf, 256);
				if( 0 == strncmp(buf, "LastChaos-Bot -", 15) )
				{
					Return = TRUE;	
					CPrintF("나는 행유져");
				}	
#endif			
				CloseHandle (hInput);
			}
		}
		while (Process32Next(hInputSnap, &pe32));		
	}	
	
	CloseHandle (hInputSnap);

	return Return;
}

/*
class CEnableUserBreak {
public:
	BOOL bOld;
	CEnableUserBreak();
	~CEnableUserBreak();
};

CEnableUserBreak::CEnableUserBreak() {
	bOld = _bUserBreakEnabled;
	_bUserBreakEnabled = TRUE;
}
CEnableUserBreak::~CEnableUserBreak() {
	_bUserBreakEnabled = bOld;
}

// wrapper function for dump and printout of extensive demo profile report
static void DumpDemoProfile(void)
{
	CTString strFragment, strAnalyzed;
	dem_iProfileRate = Clamp( dem_iProfileRate, 0L, 60L);
	strFragment = _pGame->DemoReportFragmentsProfile( dem_iProfileRate);
	strAnalyzed = _pGame->DemoReportAnalyzedProfile();
	try {
	// create file
		CTFileStream strm;
		CTString strFileName = CTString( "temp\\DemoProfile.lst");
		strm.Create_t( strFileName, CTStream::CM_TEXT);
		// dump results
		strm.FPrintF_t( strFragment);
		strm.FPrintF_t( strAnalyzed);
		// done!
		CPrintF( TRANS("Demo profile data dumped to '%s'.\n"), strFileName);
	} 
	catch (char *strError) {
		// something went wrong :(
		CPrintF( TRANS("Cannot dump demo profile data: %s\n"), strError);
	}
}

static void ReportDemoProfile(void)
{
	CTString strFragment, strAnalyzed;
	dem_iProfileRate = Clamp( dem_iProfileRate, 0L, 60L);
	strFragment = _pGame->DemoReportFragmentsProfile( dem_iProfileRate);
	strAnalyzed = _pGame->DemoReportAnalyzedProfile();
	CPrintF( strFragment);
	CPrintF( strAnalyzed);
	CPrintF( "-\n");
}
*/

#define MAX_SCRIPTSOUNDS 16
static CSoundObject *_apsoScriptChannels[MAX_SCRIPTSOUNDS] = {0};

static void PlayScriptSound(INDEX iChannel, const CTString &strSound, FLOAT fVolume, FLOAT fPitch, BOOL bLooping)
{
	if (iChannel<0 || iChannel>=MAX_SCRIPTSOUNDS) {
		return;
	}
	if (_apsoScriptChannels[iChannel]==NULL) {
		_apsoScriptChannels[iChannel] = new CSoundObject;
	}
	_apsoScriptChannels[iChannel]->SetVolume(fVolume);
	_apsoScriptChannels[iChannel]->SetPitch(fPitch);
	try {
		_apsoScriptChannels[iChannel]->Play_t(strSound, SOF_NONGAME|(bLooping?SOF_LOOP:0));
	} catch (char *strError) {
		CPrintF("%s\n", strError);
	}
}
static void StopScriptSound(INDEX iChannel)
{
	if (iChannel<0 || iChannel>=MAX_SCRIPTSOUNDS||_apsoScriptChannels[iChannel]==NULL) {
		return;
	}
	_apsoScriptChannels[iChannel]->Stop();
} 
static INDEX IsScriptSoundPlaying(INDEX iChannel)
{
	if (iChannel<0 || iChannel>=MAX_SCRIPTSOUNDS||_apsoScriptChannels[iChannel]==NULL) {
		return 0;
	}
	return _apsoScriptChannels[iChannel]->IsPlaying();
}

// Dump recorded profiling stats to file.
static void DumpProfileToFile(void)
{
	_bDumpNextTime = TRUE;
}

// Dump recorded profiling stats to console.
static void DumpProfileToConsole(void)
{
	CPutString(_strProfile);
}

// Record profiling stats.
static void RecordProfile(void)
{
	_bStartProfilingNextTime = TRUE;
}

// screen shot saving feature in console
static BOOL  bSaveScreenShot = FALSE;
static INDEX dem_iAnimFrame  = -1;
static void SaveScreenShot(void)
{
	bSaveScreenShot=TRUE;
}

static void Say(const CTString &strText)
{
	_pNetwork->SendChat(-1, -1, strText);
}
static void SayFromTo(INDEX ulFrom, INDEX ulTo, const CTString &strText)
{
	_pNetwork->SendChat(ulFrom, ulTo, strText);
}


// create name for a new screenshot
static CTFileName MakeScreenShotName(void)
{
//강동민 수정 시작 다중 공격 작업	09.06
	/*
	// create base name from the world name
	CTFileName fnmBase = CTString("Shots\\")+_pNetwork->GetCurrentWorld().FileName();

	// start at counter of zero
	INDEX iShot = 0;
	// repeat forever
	FOREVER {
		// create number for the file
		CTString strNumber;
		strNumber.PrintF("_shot%04d", iShot);
		// create the full filename
		CTFileName fnmFullTGA = fnmBase+strNumber+".tga";
		CTFileName fnmFullJPG = fnmBase+strNumber+".jpg";
		// if the file doesn't exist
		if (!FileExistsForWriting(fnmFullTGA) && !FileExistsForWriting(fnmFullJPG)) {
			// that is the right filename
			return fnmFullTGA;
		}
		// if it exists, increment the number and retry
		iShot++;
	}
	*/
//강동민 수정 끝 다중 공격 작업		09.06
	// 오늘 날짜 현재 시간이 파일이름된다.
	char szDate[9];
	char szTime[9];
	_strdate( szDate );
	szDate[2] = szDate[5] = '-';
	
	_strtime( szTime );	
	szTime[2] = szTime[5] = ';';

	CTString	strDate = CTString(szDate);
	CTString	strTime = CTString(szTime);
	CTString	strExt	= ".jpg";
	if(!gam_iJpgScreenShot)
	{
		strExt = ".tga";
	}
	CTFileName	fnmBase = "shot(" + strDate+ ")" + strTime + strExt;
	return fnmBase;
}


CButtonAction::CButtonAction(void)
{
	ba_ulFirstControl = CONTROL_NONE;
	ba_ulSecondControl = CONTROL_NONE;
	ba_bFirstKeyDown = FALSE;
	ba_bSecondKeyDown = FALSE;
	ba_bHidden  = FALSE;
}

// Assignment operator.
CButtonAction &CButtonAction ::operator=(CButtonAction &baOriginal)
{
	ba_strName                    = baOriginal.ba_strName;
	ba_strCommandLineWhenPressed  = baOriginal.ba_strCommandLineWhenPressed;
	ba_strCommandLineWhenReleased = baOriginal.ba_strCommandLineWhenReleased;
	ba_strCommandLineWhenHeld     = baOriginal.ba_strCommandLineWhenHeld;
	ba_bHidden                    = baOriginal.ba_bHidden;
	ba_bFirstKeyDown = FALSE;
	ba_bSecondKeyDown = FALSE;

	return *this;
}

void CButtonAction::Read_t( CTStream &istrm)
{
	istrm>>ba_strName;
	istrm>>ba_strCommandLineWhenPressed;
	istrm>>ba_strCommandLineWhenReleased;
	istrm>>ba_strCommandLineWhenHeld;
	istrm>>ba_bHidden;
}

void CButtonAction::Write_t( CTStream &ostrm)
{
	ostrm<<ba_strName;
	ostrm<<ba_strCommandLineWhenPressed;
	ostrm<<ba_strCommandLineWhenReleased;
	ostrm<<ba_strCommandLineWhenHeld;
	ostrm<<ba_bHidden;
}

void CControls::DoControlActions(void)
{
	// for all button actions
	FOREACHINLIST( CButtonAction, ba_lnNode, ctrl_lhButtonActions, itButtonAction)
	{
		// test if first button is pressed
		BOOL bFirstPressed = _pInput->GetControlValue(itButtonAction->ba_ulFirstControl)>0.0f;

		// remember control value
		ctl_fInputValue = _pInput->GetControlValue(itButtonAction->ba_ulFirstControl);

		// if it was just pressed
		if(bFirstPressed && !itButtonAction->ba_bFirstKeyDown) {
			// call pressed command
			_pShell->Execute(itButtonAction->ba_strCommandLineWhenPressed);
		// if it was just released
		} else if(!bFirstPressed && itButtonAction->ba_bFirstKeyDown) {
			// call released command
			_pShell->Execute(itButtonAction->ba_strCommandLineWhenReleased);
		// if held down
		} else if(bFirstPressed) {
			// call held command
			_pShell->Execute(itButtonAction->ba_strCommandLineWhenHeld);
		}

		// remember pressed state
		itButtonAction->ba_bFirstKeyDown = bFirstPressed;

		// test if second button is pressed
		BOOL bSecondPressed = _pInput->GetControlValue(itButtonAction->ba_ulSecondControl)>0.0f;

		// remember control value
		ctl_fInputValue = _pInput->GetControlValue(itButtonAction->ba_ulSecondControl);

		// if it was just pressed
		if (bSecondPressed && !itButtonAction->ba_bSecondKeyDown) {
			// call pressed command
			_pShell->Execute(itButtonAction->ba_strCommandLineWhenPressed);
		// if it was just released
		} else if (!bSecondPressed && itButtonAction->ba_bSecondKeyDown) {
			// call released command
			_pShell->Execute(itButtonAction->ba_strCommandLineWhenReleased);
		// if held down
		} else if(bSecondPressed) {
			// call held command
			_pShell->Execute(itButtonAction->ba_strCommandLineWhenHeld);
		}
		// remember pressed state
		itButtonAction->ba_bSecondKeyDown = bSecondPressed;
	}
}

void CControls::CreateAction(const CPlayerCharacter &pc, CPlayerAction &paAction, BOOL bPreScan)
{
	// execute all button-action shell commands
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
	ctl_fFrameTime = (FLOAT)Clamp((ctrl_tvLastRead-tvNow).GetSeconds(), 0.001, 0.2);
	ctrl_tvLastRead = tvNow;
	DoControlActions();
//  CPlayerSettings *pps = (CPlayerSettings *)pc.pc_aubAppearance;
//  pps->ps_iSensitivity = NormFloatToByte(_pGame->gm_ctrlControlsExtra.ctrl_fSensitivity/100 - 0.5);
	// make the player class create the action packet
	ctl_ComposeActionPacket(pc, paAction, bPreScan);

/*	if (paAction.pa_aRotation(1) != 0.0f)
	{
		CPrintF("creating: prescan %d, x:%g\n", bPreScan, paAction.pa_aRotation(1));
	}*/
}

CButtonAction &CControls::AddButtonAction(void)
{
	// create a new action
	CButtonAction *pbaNew = new CButtonAction;
	// add it to end of list
	ctrl_lhButtonActions.AddTail(pbaNew->ba_lnNode);
	return *pbaNew;
}

void CControls::RemoveButtonAction( CButtonAction &baButtonAction)
{
	// remove from list
	baButtonAction.ba_lnNode.Remove();
	// free it
	delete &baButtonAction;
}



// calculate some useful demo vars
static void CalcDemoProfile( INDEX ctFrames, INDEX &ctFramesNoPeaks,
						DOUBLE &dTimeSum, DOUBLE &dTimeSumNoPeaks, TIME &tmAverage, TIME &tmAverageNoPeaks,
						TIME &tmSigma, TIME &tmHighLimit, TIME &tmLowLimit, TIME &tmHighPeak, TIME &tmLowPeak,
						FLOAT &fAvgWTris, FLOAT &fAvgMTris, FLOAT &fAvgPTris, FLOAT &fAvgTTris,
						FLOAT &fAvgWTrisNoPeaks, FLOAT &fAvgMTrisNoPeaks, FLOAT &fAvgPTrisNoPeaks, FLOAT &fAvgTTrisNoPeaks)
{
	// calculate raw average
	INDEX i;
	TIME tmCurrent;
	dTimeSum = 0;
	DOUBLE dWTriSum=0, dMTriSum=0, dPTriSum=0, dTTriSum=0;
	DOUBLE dWTriSumNoPeaks=0, dMTriSumNoPeaks=0, dPTriSumNoPeaks=0, dTTriSumNoPeaks=0;
	for( i=0; i<ctFrames; i++) {
		dTimeSum += _atmFrameTimes[i];
		dWTriSum += _actTriangles[i*4 +0];  // world
		dMTriSum += _actTriangles[i*4 +1];  // model
		dPTriSum += _actTriangles[i*4 +2];  // particle
		dTTriSum += _actTriangles[i*4 +3];  // total
	}
	tmAverage = dTimeSum / ctFrames;
	fAvgWTris = dWTriSum / ctFrames;
	fAvgMTris = dMTriSum / ctFrames;
	fAvgPTris = dPTriSum / ctFrames;
	fAvgTTris = dTTriSum / ctFrames;

	// calc raw sigma and limits
	DOUBLE dSigmaSum=0;
	for( i=0; i<ctFrames; i++) {
		tmCurrent = _atmFrameTimes[i];
		TIME tmDelta = tmCurrent-tmAverage;
		dSigmaSum += tmDelta*tmDelta;
	}
	tmSigma = Sqrt(dSigmaSum/ctFrames);
	tmHighLimit = (tmAverage-tmSigma*2);
	tmLowLimit  = (tmAverage+tmSigma*2);

	// eliminate low peaks
	ctFramesNoPeaks = ctFrames;
	dTimeSumNoPeaks = dTimeSum;
	dWTriSumNoPeaks = dWTriSum;
	dMTriSumNoPeaks = dMTriSum;
	dPTriSumNoPeaks = dPTriSum;
	dTTriSumNoPeaks = dTTriSum;

	for( i=0; i<ctFrames; i++) {
		tmCurrent = _atmFrameTimes[i];
		if( tmHighLimit>tmCurrent || tmLowLimit<tmCurrent) {
			dTimeSumNoPeaks -= tmCurrent;
			dWTriSumNoPeaks -= _actTriangles[i*4 +0];
			dMTriSumNoPeaks -= _actTriangles[i*4 +1];
			dPTriSumNoPeaks -= _actTriangles[i*4 +2];
			dTTriSumNoPeaks -= _actTriangles[i*4 +3];
			ctFramesNoPeaks--;
		} 
	}

	// calculate peaks, new averages and sigma (without peaks)
	tmAverageNoPeaks = dTimeSumNoPeaks / ctFramesNoPeaks;
	fAvgWTrisNoPeaks = dWTriSumNoPeaks / ctFramesNoPeaks;
	fAvgMTrisNoPeaks = dMTriSumNoPeaks / ctFramesNoPeaks;
	fAvgPTrisNoPeaks = dPTriSumNoPeaks / ctFramesNoPeaks;
	fAvgTTrisNoPeaks = dTTriSumNoPeaks / ctFramesNoPeaks;

	dSigmaSum=0;
	tmHighPeak=99999, tmLowPeak=0;
	for( i=0; i<ctFrames; i++) {
		tmCurrent = _atmFrameTimes[i];
		if( tmHighLimit>tmCurrent || tmLowLimit<tmCurrent) continue;
		TIME tmDelta = tmCurrent-tmAverageNoPeaks;
		dSigmaSum += tmDelta*tmDelta;
		if( tmHighPeak > tmCurrent) tmHighPeak = tmCurrent;
		if( tmLowPeak  < tmCurrent) tmLowPeak  = tmCurrent;
	}
	tmSigma = Sqrt( dSigmaSum/ctFramesNoPeaks);
}

CGame::CGame()
	: m_oldTime(0)
{
	gmUpdateInputState = NULL;
}

CGame::~CGame()
{
	if (gmUpdateInputState)
	{
		delete gmUpdateInputState;
	}
}


// dump demo profile to file 
CTString CGame::DemoReportFragmentsProfile( INDEX iRate)
{
	CTString strRes="";
	CTString strTmp;
	INDEX ctFrames = _atmFrameTimes.Count();

	// if report is not required
	if( dem_iProfileRate==0) {
		strRes.PrintF( TRANS("\nFragments report disabled.\n"));
		return strRes;
	}

	// if not enough frames
	if( ctFrames<20) {
		strRes.PrintF( TRANS("\nNot enough recorded frames to make fragments report.\n"));
		return strRes;
	}

	// enough frames - calc almost everything
	strRes.PrintF( TRANS("\nDemo performance results (fragment time = %d seconds):\n"), dem_iProfileRate);
	strTmp.PrintF(         "------------------------------------------------------\n\n");
	strRes += strTmp;
	DOUBLE dTimeSum, dTimeSumNoPeaks;
	INDEX  ctFramesNoPeaks;
	TIME   tmAverage, tmAverageNoPeaks;
	TIME   tmSigma, tmHighLimit, tmLowLimit, tmHighPeak, tmLowPeak;
	FLOAT  fAvgWTris, fAvgMTris, fAvgPTris, fAvgTTris;
	FLOAT  fAvgWTrisNoPeaks, fAvgMTrisNoPeaks, fAvgPTrisNoPeaks, fAvgTTrisNoPeaks;
	CalcDemoProfile( ctFrames, ctFramesNoPeaks, dTimeSum, dTimeSumNoPeaks, tmAverage, tmAverageNoPeaks,
									 tmSigma, tmHighLimit, tmLowLimit, tmHighPeak, tmLowPeak,
									 fAvgWTris, fAvgMTris, fAvgPTris, fAvgTTris,
									 fAvgWTrisNoPeaks, fAvgMTrisNoPeaks, fAvgPTrisNoPeaks, fAvgTTrisNoPeaks);
	strTmp.PrintF( TRANS("   #   average FPS     average FPS (W/O peaks)\n"));
	strRes += strTmp;
	// loop thru frames and create output of time fragmens
	dTimeSum = 0;
	dTimeSumNoPeaks = 0;
	ctFramesNoPeaks = 0;
	FLOAT fFrameCounter = 0;
	FLOAT fFrameCounterNoPeaks = 0;
	TIME  tmRate = dem_iProfileRate;
	INDEX iFragment=1;
	for( INDEX i=0; i<ctFrames; i++)
	{ // get current frame time and calc sums
		TIME tmCurrent = _atmFrameTimes[i];
		dTimeSum += tmCurrent;
		fFrameCounter++;
		if( tmHighLimit<=tmCurrent && tmLowLimit>=tmCurrent) {
			dTimeSumNoPeaks += tmCurrent;
			fFrameCounterNoPeaks++;
		}
		// enough data for one time fragment
		if( dTimeSum>=tmRate) {
			FLOAT fTimeOver  = dTimeSum - tmRate;
			FLOAT fFrameOver = fTimeOver/tmCurrent;
			FLOAT fFragmentAverage = tmRate / (fFrameCounter-fFrameOver);
			FLOAT fFragmentNoPeaks = (tmRate-(dTimeSum-dTimeSumNoPeaks)) / (fFrameCounterNoPeaks-fFrameOver);
			strTmp.PrintF( "%4d    %6.1f           %6.1f", iFragment, 1.0f/fFragmentAverage, 1.0f/fFragmentNoPeaks);
			strRes += strTmp;
			INDEX iFragmentAverage10 = FloatToInt(5.0f/fFragmentAverage);
			INDEX iFragmentNoPeaks10 = FloatToInt(5.0f/fFragmentNoPeaks);
			if( iFragmentAverage10 != iFragmentNoPeaks10) strTmp.PrintF( "    !\n");
			else strTmp.PrintF( "\n");
			strRes += strTmp;
			// restart time and frames
			dTimeSum = fTimeOver;
			dTimeSumNoPeaks = fTimeOver;
			fFrameCounter = fFrameOver;
			fFrameCounterNoPeaks = fFrameOver;
			iFragment++;
		}
	}

	// all done
	return strRes;
}


// printout extensive demo profile report
CTString CGame::DemoReportAnalyzedProfile(void)
{
	CTString strRes="";
	INDEX ctFrames = _atmFrameTimes.Count();
	// nothing kept?
	if( ctFrames<20) {
		strRes.PrintF( TRANS("\nNot enough recorded frames to analyze.\n"));
		return strRes;
	}

	// calc almost everything
	DOUBLE dTimeSum, dTimeSumNoPeaks;
	INDEX  ctFramesNoPeaks;
	TIME   tmAverage, tmAverageNoPeaks;
	TIME   tmSigma, tmHighLimit, tmLowLimit, tmHighPeak, tmLowPeak;
	FLOAT  fAvgWTris, fAvgMTris, fAvgPTris, fAvgTTris;
	FLOAT  fAvgWTrisNoPeaks, fAvgMTrisNoPeaks, fAvgPTrisNoPeaks, fAvgTTrisNoPeaks;
	CalcDemoProfile( ctFrames, ctFramesNoPeaks, dTimeSum, dTimeSumNoPeaks, tmAverage, tmAverageNoPeaks,
									 tmSigma, tmHighLimit, tmLowLimit, tmHighPeak, tmLowPeak,
									 fAvgWTris, fAvgMTris, fAvgPTris, fAvgTTris,
									 fAvgWTrisNoPeaks, fAvgMTrisNoPeaks, fAvgPTrisNoPeaks, fAvgTTrisNoPeaks);
	// calc sustains
	DOUBLE dHighSum=0, dLowSum=0;
	DOUBLE dCurrentHighSum=0, dCurrentLowSum=0;
	INDEX  ctHighFrames=0, ctLowFrames=0;
	INDEX  ctCurrentHighFrames=0, ctCurrentLowFrames=0;
	for( INDEX i=0; i<ctFrames; i++)
	{ // skip low peaks
		TIME tmCurrent = _atmFrameTimes[i];
		if( tmHighLimit>tmCurrent || tmLowLimit<tmCurrent)  continue;

		// high?
		if( (tmAverageNoPeaks-tmSigma) > tmCurrent) {
			// keep high sustain
			dCurrentHighSum += tmCurrent;
			ctCurrentHighFrames++;
		} else {
			// new high sustain found?
			if( ctHighFrames < ctCurrentHighFrames) {
				ctHighFrames = ctCurrentHighFrames;
				dHighSum     = dCurrentHighSum;
			}
			// reset high sustain
			ctCurrentHighFrames = 0;
			dCurrentHighSum     = 0;
		} 
		// low?
		if( (tmAverageNoPeaks+tmSigma) < tmCurrent) {
			// keep low sustain
			dCurrentLowSum += tmCurrent;
			ctCurrentLowFrames++;
		} else {
			// new low sustain found?
			if( ctLowFrames < ctCurrentLowFrames) {
				ctLowFrames = ctCurrentLowFrames;
				dLowSum     = dCurrentLowSum;
			}
			// reset low sustain
			ctCurrentLowFrames = 0;
			dCurrentLowSum     = 0;
		} 
	}
	// and results are ...
	TIME tmHighSustained = ctHighFrames / dHighSum;
	TIME tmLowSustained  = ctLowFrames  / dLowSum; 

	// printout
	CTString strTmp;
	strTmp.PrintF( TRANS("\n%.1f KB used for demo profile:\n"), 1+ ctFrames*5*sizeof(FLOAT)/1024.0f);
	strRes += strTmp;
	strTmp.PrintF( TRANS("    Originally recorded: %d frames in %.1f seconds => %5.1f FPS average.\n"),
								 ctFrames, dTimeSum, 1.0f/tmAverage);
	strRes += strTmp;
	strTmp.PrintF( TRANS("Without excessive peaks: %d frames in %.1f seconds => %5.1f FPS average.\n"),
								 ctFramesNoPeaks, dTimeSumNoPeaks, 1.0f/tmAverageNoPeaks);
	strRes += strTmp;
	strTmp.PrintF( TRANS("       High peak: %5.1f FPS\n"), 1.0f/tmHighPeak);
	strRes += strTmp;
	strTmp.PrintF( TRANS("        Low peak: %5.1f FPS\n"), 1.0f/tmLowPeak);
	strRes += strTmp;
	// enough values recorder for high sustain?
	if( ctHighFrames > (ctFrames/1024+5)) {
		strTmp.PrintF( TRANS("  High sustained: %5.1f FPS (%d frames in %.1f seconds)\n"),
									 tmHighSustained, ctHighFrames, dHighSum);
		strRes += strTmp;
	}
	// enough values recorder for low sustain?
	if( ctLowFrames > (ctFrames/1024+5)) {
		strTmp.PrintF( TRANS("   Low sustained: %5.1f FPS (%d frames in %.1f seconds)\n"),
									 tmLowSustained,  ctLowFrames,  dLowSum);
		strRes += strTmp;
	}

	// do triangle profile output (hidden - maybe not so wise idea)
	if( dem_bProfile==217) {
		const FLOAT fAvgRTris = fAvgTTris - (fAvgWTris+fAvgMTris+fAvgPTris);
		const FLOAT fAvgRTrisNoPeaks = fAvgTTrisNoPeaks - (fAvgWTrisNoPeaks+fAvgMTrisNoPeaks+fAvgPTrisNoPeaks);
		strTmp.PrintF( TRANS("Triangles per frame (with and without excessive peaks):\n")); strRes += "\n"+strTmp;
		strTmp.PrintF( TRANS("      World: %7.1f / %.1f\n"), fAvgWTris, fAvgWTrisNoPeaks);  strRes += strTmp;
		strTmp.PrintF( TRANS("      Model: %7.1f / %.1f\n"), fAvgMTris, fAvgMTrisNoPeaks);  strRes += strTmp;
		strTmp.PrintF( TRANS("   Particle: %7.1f / %.1f\n"), fAvgPTris, fAvgPTrisNoPeaks);  strRes += strTmp;
		strTmp.PrintF( TRANS("  rest (2D): %7.1f / %.1f\n"), fAvgRTris, fAvgRTrisNoPeaks);  strRes += strTmp;
		strRes +=            "           --------------------\n";
		strTmp.PrintF( TRANS("      TOTAL: %7.1f / %.1f\n"), fAvgTTris, fAvgTTrisNoPeaks);  strRes += strTmp;
	}

	// all done
	return strRes;
}



/* This is called every TickQuantum seconds. */
void CGameTimerHandler::HandleTimer(void)
{
	// call game's timer routine
	_pGame->GameHandleTimer();
}


void CGame::GameHandleTimer(void)
{
	// if direct input is active
	if( _pInput->IsInputEnabled() )
	{
		/*		// yjpark
		// check if any active control uses joystick
		BOOL bAnyJoy = _ctrlCommonControls.UsesJoystick();
		for( INDEX iPlayer=0; iPlayer<4; iPlayer++) {
			if( gm_lpLocalPlayers[ iPlayer].lp_pplsPlayerSource != NULL) {
				INDEX iCurrentPlayer = gm_lpLocalPlayers[ iPlayer].lp_iPlayer;
				CControls &ctrls = gm_actrlControls[ iCurrentPlayer];
				if (ctrls.UsesJoystick()) {
					bAnyJoy = TRUE;
					break;
				}
			}
		}
		_pInput->SetJoyPolling(bAnyJoy);
		*/

		// read input devices
		_pInput->GetInput( FALSE );

		// if game is currently active, and not paused
		if( gm_bGameOn && !_pNetwork->IsPaused() && !_pNetwork->GetLocalPause() )
		{
			// for all possible local players
//강동민 수정 시작 로그인 변경 작업	08.10
			//for( INDEX iPlayer = 0; iPlayer < 4; iPlayer++ )
			for( INDEX iPlayer = 0; iPlayer < NET_MAXGAMEPLAYERS; iPlayer++ )
//강동민 수정 끝 로그인 변경 작업		08.10
			{
				// if this player exist
				if( gm_lpLocalPlayers[iPlayer].lp_pplsPlayerSource != NULL )
				{
					// publish player index to console
					ctl_iCurrentPlayerLocal = iPlayer;
					ctl_iCurrentPlayer = gm_lpLocalPlayers[iPlayer].lp_pplsPlayerSource->pls_Index;
					
					// copy its local controls to current controls
					memcpy(	ctl_pvPlayerControls, gm_lpLocalPlayers[ iPlayer].lp_ubPlayerControlsState,
							ctl_slPlayerControlsSize );

					// create action for it for this tick
					CPlayerAction paAction;
					INDEX iCurrentPlayer = gm_lpLocalPlayers[iPlayer].lp_iPlayer;
					CControls &ctrls = gm_actrlControls[iCurrentPlayer];

					ctrls.CreateAction( gm_apcPlayers[iCurrentPlayer], paAction, FALSE );
					// set the action in the client source object
					gm_lpLocalPlayers[iPlayer].lp_pplsPlayerSource->SetAction( paAction );

					// copy the local controls back
					memcpy(	gm_lpLocalPlayers[ iPlayer].lp_ubPlayerControlsState, ctl_pvPlayerControls,
							ctl_slPlayerControlsSize );
				}
			}
			// clear player indices
			ctl_iCurrentPlayerLocal = -1;
			ctl_iCurrentPlayer = -1;
		}
		// execute all button-action shell commands for common controls
		if( gm_bGameOn )
		{
			_ctrlCommonControls.DoControlActions();
		}
	}
	// if DirectInput is disabled, and game is currently active
	else if( gm_bGameOn )
	{
		// for all possible local players
//강동민 수정 시작 로그인 변경 작업	08.10
		//for( INDEX iPlayer = 0; iPlayer < 4; iPlayer++ )
		for( INDEX iPlayer = 0; iPlayer < NET_MAXGAMEPLAYERS; iPlayer++ )
//강동민 수정 끝 로그인 변경 작업		08.10
		{
			// if this player exist
			if( gm_lpLocalPlayers[iPlayer].lp_pplsPlayerSource != NULL )
			{ 
				CPlayerSource &pls = *gm_lpLocalPlayers[iPlayer].lp_pplsPlayerSource;
				// create dummy action for the player for this tick
				CPlayerAction paClearAction;
				// clear actions
				paClearAction = pls.pls_paAction;
				paClearAction.pa_vTranslation  = FLOAT3D( 0.0f, 0.0f, 0.0f );
				//paClearAction.pa_aRotation     = ANGLE3D( 0, 0, 0 );
				//paClearAction.pa_aViewRotation = ANGLE3D( 0, 0, 0 );
				paClearAction.pa_ulButtons     = 0;

				// clear the action in the client source object
				pls.SetAction( paClearAction );
			}
		}
	}
	static DWORD dwWhat = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	if (gm_bGameOn && !_pNetwork->IsDisconnected())
	{
		if (_pTimer->GetHighPrecisionTimer().GetMilliseconds() - dwWhat >= 60000)
		{
			SetEvent(gmUpdateHandle);
			dwWhat = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		}
	}
}

UINT WINAPI CGame::UpdateThreadFunction(void *parameter)
{
	while (1)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(_pGame->GetUpdateHandle(), INFINITE))
		{
			BOOL bState = FALSE;
			// IDA 등 디버깅 툴에서의 난독처리. 단 올리디버그 2.0 이상 버전에서는 난독처리 되지 않음. [3/2/2011 rumist]
			__asm
			{
				push	eax
				mov		eax,	1
				cmp		eax,	0			// 항상 실패하도록 코드가 되어있음. 때문에 기본적으로 tagJunk로 이동함.
				je		[tagOriginal]
				jmp		[tagJunk]
			tagOriginal:				
				_emit	0xE8				// check Jmp short pos.  여기서는 그냥 안티 디버거에서 위치좌표 읽기 방해용임.
											// _emit가 한 바이트의 수정이므로 실제로 tagJunk 포인터를 찾을 수 없도록 변경함.
			tagJunk:
				call	InputState
				mov		bState, eax
				pop		eax
			}
			// if detected hack user, request disconnect.
			if (bState)
			{
				_pNetwork->SendInput();
			}
		}
	}

	return 0;
}

/*
 * Global game object (in our case Flesh) initialization function
 */
void CGame::InitInternal( void)
{
	gam_strCustomLevel = ""; // filename of custom level chosen
	gam_strSessionName = TRANS("Unnamed session"); // name of multiplayer network session
	gam_strJoinAddress = TRANS("serveraddress");   // join address

//	gm_MenuSplitScreenCfg    = SSC_PLAY1;
	gm_StartSplitScreenCfg   = SSC_PLAY1;
	gm_CurrentSplitScreenCfg = SSC_PLAY1;
//	gm_iLastSetHighScore = 0;
	gm_iSinglePlayer = 0;
	gm_iWEDSinglePlayer = 0;
	gm_bGameOn = FALSE;
	gm_bFirstLoading = FALSE;
	gm_slPlayerControlsSize = 0;
	gm_pvGlobalPlayerControls = NULL;
	memset(gm_aiMenuLocalPlayers, 0, sizeof(gm_aiMenuLocalPlayers));
	memset(gm_aiStartLocalPlayers, 0, sizeof(gm_aiStartLocalPlayers));

	gm_csConsoleState = CS_OFF;
	gm_csComputerState = CS_OFF;

	gm_bGameOn = FALSE;
	gm_bFirstLoading = FALSE;
	gm_aiMenuLocalPlayers[0] =  0;
	//gm_aiMenuLocalPlayers[1] = -1;
	//gm_aiMenuLocalPlayers[2] = -1;
	//gm_aiMenuLocalPlayers[3] = -1;

//	gm_MenuSplitScreenCfg = SSC_PLAY1;

	LoadPlayersAndControls();

	gm_iWEDSinglePlayer = 0;
	gm_iSinglePlayer = 0;

	// add game timer handler
	_pTimer->AddHandler(&m_gthGameTimerHandler);
	// add shell variables
	_pShell->DeclareSymbol("user FLOAT ctl_fFrameTime;",            &ctl_fFrameTime);
	_pShell->DeclareSymbol("user FLOAT ctl_fInputValue;",           &ctl_fInputValue);
	_pShell->DeclareSymbol("user void RecordProfile(void);",        &RecordProfile);
	_pShell->DeclareSymbol("user void SaveScreenShot(void);",       &SaveScreenShot);
	_pShell->DeclareSymbol("user void DumpProfileToConsole(void);", &DumpProfileToConsole);
	_pShell->DeclareSymbol("user void DumpProfileToFile(void);",    &DumpProfileToFile);

	_pShell->DeclareSymbol("user INDEX hud_iStats;", &hud_iStats);
	_pShell->DeclareSymbol("persistent user INDEX hud_iFPSGraphSpeed;", &hud_iFPSGraphSpeed);  
	_pShell->DeclareSymbol("persistent user INDEX hud_iFPSGraphMax;",   &hud_iFPSGraphMax);  

	_pShell->DeclareSymbol("user INDEX hud_bShowResolution;", &hud_bShowResolution);
	_pShell->DeclareSymbol("user INDEX hud_bShowPauseText;",  &hud_bShowPauseText);
	_pShell->DeclareSymbol("persistent user INDEX hud_bShowTime;",  &hud_bShowTime);
	_pShell->DeclareSymbol("persistent user INDEX hud_bShowClock;", &hud_bShowClock);
	_pShell->DeclareSymbol("persistent user INDEX hud_bShowNetGraph;", &hud_bShowNetGraph);

	_pShell->DeclareSymbol("user INDEX dem_bOnScreenDisplay;", &dem_bOSD);
	_pShell->DeclareSymbol("user INDEX dem_bPlay;",            &dem_bPlay);
	_pShell->DeclareSymbol("user INDEX dem_bPlayByName;",      &dem_bPlayByName);
	_pShell->DeclareSymbol("user INDEX dem_bProfile;",         &dem_bProfile);
	_pShell->DeclareSymbol("persistent user INDEX dem_iProfileRate;", &dem_iProfileRate);
	_pShell->DeclareSymbol("user INDEX dem_iAnimFrame;",       &dem_iAnimFrame);
	_pShell->DeclareSymbol("persistent user FLOAT dem_fMotionBlurStrength;", &dem_fMotionBlurStrength);
	_pShell->DeclareSymbol("user CTString dem_strPostExec;",   &dem_strPostExec);

	_pShell->DeclareSymbol("FLOAT gam_afEnemyMovementSpeed[5];", &gam_afEnemyMovementSpeed);
	_pShell->DeclareSymbol("FLOAT gam_afEnemyAttackSpeed[5];",   &gam_afEnemyAttackSpeed);
	_pShell->DeclareSymbol("FLOAT gam_afDamageStrength[5];",     &gam_afDamageStrength);
	_pShell->DeclareSymbol("FLOAT gam_afAmmoQuantity[5];",       &gam_afAmmoQuantity);

	_pShell->DeclareSymbol("persistent user FLOAT gam_fManaTransferFactor;", &gam_fManaTransferFactor);
	_pShell->DeclareSymbol("persistent user FLOAT gam_fExtraEnemyStrength         ;", &gam_fExtraEnemyStrength          );
	_pShell->DeclareSymbol("persistent user FLOAT gam_fExtraEnemyStrengthPerPlayer;", &gam_fExtraEnemyStrengthPerPlayer );
//	_pShell->DeclareSymbol("persistent user INDEX gam_iInitialMana;",        &gam_iInitialMana);
//	_pShell->DeclareSymbol("persistent user INDEX gam_iScoreLimit;",  &gam_iScoreLimit);
	_pShell->DeclareSymbol("persistent user INDEX gam_iFragLimit;",   &gam_iFragLimit);
	_pShell->DeclareSymbol("persistent user INDEX gam_iTimeLimit;",   &gam_iTimeLimit);
	_pShell->DeclareSymbol("persistent user INDEX gam_ctMaxPlayers;", &gam_ctMaxPlayers);
	_pShell->DeclareSymbol("persistent user INDEX gam_bWaitAllPlayers;", &gam_bWaitAllPlayers);
//	_pShell->DeclareSymbol("persistent user INDEX gam_bFriendlyFire;",   &gam_bFriendlyFire);
	_pShell->DeclareSymbol("persistent user INDEX gam_bPlayEntireGame;", &gam_bPlayEntireGame);
	//_pShell->DeclareSymbol("persistent user INDEX gam_bWeaponsStay;",    &gam_bWeaponsStay);

	//_pShell->DeclareSymbol("persistent user INDEX gam_bAmmoStays       ;", &gam_bAmmoStays       );
	//_pShell->DeclareSymbol("persistent user INDEX gam_bHealthArmorStays;", &gam_bHealthArmorStays);
	//_pShell->DeclareSymbol("persistent user INDEX gam_bAllowHealth     ;", &gam_bAllowHealth     );
	//_pShell->DeclareSymbol("persistent user INDEX gam_bAllowArmor      ;", &gam_bAllowArmor      );
	//_pShell->DeclareSymbol("persistent user INDEX gam_bInfiniteAmmo    ;", &gam_bInfiniteAmmo    );
	_pShell->DeclareSymbol("persistent user INDEX gam_bRespawnInPlace  ;", &gam_bRespawnInPlace  );

//	_pShell->DeclareSymbol("persistent user INDEX gam_iCredits;", &gam_iCredits);
	_pShell->DeclareSymbol("persistent user FLOAT gam_tmSpawnInvulnerability;", &gam_tmSpawnInvulnerability);

//  _pShell->DeclareSymbol("persistent user INDEX gam_iBlood;", &gam_iBlood);
//  _pShell->DeclareSymbol("persistent user INDEX gam_bGibs;",  &gam_bGibs);

	_pShell->DeclareSymbol("persistent user INDEX gam_bUseExtraEnemies;",  &gam_bUseExtraEnemies);

	//_pShell->DeclareSymbol("user INDEX gam_bQuickLoad;", &gam_bQuickLoad);
	//_pShell->DeclareSymbol("user INDEX gam_bQuickSave;", &gam_bQuickSave);
	//_pShell->DeclareSymbol("user INDEX gam_iQuickSaveSlots;", &gam_iQuickSaveSlots);
	_pShell->DeclareSymbol("user INDEX gam_iQuickStartDifficulty;", &gam_iQuickStartDifficulty);
	_pShell->DeclareSymbol("user INDEX gam_iQuickStartMode;",       &gam_iQuickStartMode);
	_pShell->DeclareSymbol("user INDEX gam_bQuickStartMP;",       &gam_bQuickStartMP);
	_pShell->DeclareSymbol("persistent user INDEX gam_iStartDifficulty;", &gam_iStartDifficulty);
	_pShell->DeclareSymbol("persistent user INDEX gam_iStartMode;",       &gam_iStartMode);
	_pShell->DeclareSymbol("persistent user INDEX gam_iJpgScreenShot;",   &gam_iJpgScreenShot);
//	_pShell->DeclareSymbol("persistent user CTString gam_strGameSpyExtras;", &gam_strGameSpyExtras);
	_pShell->DeclareSymbol("persistent user CTString gam_strCustomLevel;", &gam_strCustomLevel);
	_pShell->DeclareSymbol("persistent user CTString gam_strSessionName;", &gam_strSessionName);
	_pShell->DeclareSymbol("persistent user CTString gam_strJoinAddress;", &gam_strJoinAddress);
	_pShell->DeclareSymbol("persistent user INDEX gam_bEnableAdvancedObserving;", &gam_bEnableAdvancedObserving);
	_pShell->DeclareSymbol("user INDEX gam_iObserverConfig;", &gam_iObserverConfig);
	_pShell->DeclareSymbol("user INDEX gam_iObserverOffset;", &gam_iObserverOffset);

//	_pShell->DeclareSymbol("INDEX gam_iRecordHighScore;", &gam_iRecordHighScore);

	_pShell->DeclareSymbol("persistent user FLOAT con_fHeightFactor;", &con_fHeightFactor);
	_pShell->DeclareSymbol("persistent user FLOAT con_tmLastLines;",   &con_tmLastLines);
	//_pShell->DeclareSymbol("user INDEX con_bTalk;", &con_bTalk);		// yjpark
//	_pShell->DeclareSymbol("user void ReportDemoProfile(void);", &ReportDemoProfile);
//	_pShell->DeclareSymbol("user void DumpDemoProfile(void);",   &DumpDemoProfile);
//	extern CTString GetGameSpyRulesInfo(void);
	extern CTString GetGameTypeName(INDEX);
	extern CTString GetCurrentGameTypeName(void);
	extern ULONG GetSpawnFlagsForGameType(INDEX);
//	extern BOOL IsMenuEnabled(const CTString &strMenuName);
//	_pShell->DeclareSymbol("user CTString GetGameSpyRulesInfo(void);",    &GetGameSpyRulesInfo);
	_pShell->DeclareSymbol("user CTString GetGameTypeName(INDEX);",       &GetGameTypeName);
	_pShell->DeclareSymbol("user CTString GetCurrentGameTypeName(void);", &GetCurrentGameTypeName);
	_pShell->DeclareSymbol("user INDEX GetSpawnFlagsForGameType(INDEX);", &GetSpawnFlagsForGameType);
//	_pShell->DeclareSymbol("user INDEX IsMenuEnabled(CTString);",         &IsMenuEnabled);
	_pShell->DeclareSymbol("user void Say(CTString);",     &Say);
	_pShell->DeclareSymbol("user void SayFromTo(INDEX, INDEX, CTString);", &SayFromTo);

	_pShell->DeclareSymbol("user const INDEX ctl_iCurrentPlayerLocal;", &ctl_iCurrentPlayerLocal);
	_pShell->DeclareSymbol("user const INDEX ctl_iCurrentPlayer;",      &ctl_iCurrentPlayer);

	_pShell->DeclareSymbol("user FLOAT gam_fChatSoundVolume;",      &gam_fChatSoundVolume);

	_pShell->DeclareSymbol("user void PlaySound(INDEX, CTString, FLOAT, FLOAT, INDEX);", &PlayScriptSound);
	_pShell->DeclareSymbol("user void StopSound(INDEX);", &StopScriptSound);
	_pShell->DeclareSymbol("user INDEX IsSoundPlaying(INDEX);", &IsScriptSoundPlaying);

	// load persistent symbols
	if (!_bDedicatedServer) {
//안태훈 수정 시작	//(Taiwan Closed beta)(0.2)
		_pShell->Execute(CTString("decode \"")+fnmPersistentSymbols+"\";");
//안태훈 수정 끝	//(Taiwan Closed beta)(0.2)
	}
	// execute the startup script
	// deleted by seo
	//_pShell->Execute(CTString("include \"")+fnmStartupScript+"\";");

	// check the size and pointer of player control variables that are local to each player
	if (ctl_slPlayerControlsSize<=0
		||ctl_slPlayerControlsSize>sizeof(((CLocalPlayer*)NULL)->lp_ubPlayerControlsState)
		||ctl_pvPlayerControls==NULL) {
		FatalError(TRANS("Current player controls are invalid."));
	}

	// load common controls
	try {
//		_ctrlCommonControls.Load_t(fnmCommonControls);
	} catch (char *strError) {
		FatalError(TRANS("Cannot load common controls: %s\n"), strError);
	}
	// 프로세스 검사용 이벤트 핸들 및 스레드
	gmUpdateHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	gmUpdateInputState = new cThreadWrapper(UpdateThreadFunction);
	gmUpdateInputState->Start(NULL);

	// init LCD textures/fonts
	LCDInit();

	// load console history
	CTString strConsole;
	try {
		//strConsole.LoadKeepCRLF_t(fnmConsoleHistory);
		//gam_strConsoleInputBuffer = strConsole;
	} catch (char *strError) {
		(void)strError; // must ignore if there is no history file
	}

	// load game shell settings
	try {
		Load_t();
	} catch (char *strError) {
		CPrintF(TRANS("Cannot load game settings:\n%s\n  Using defaults\n"), strError);
	}

	CON_DiscardLastLineTimes();

	// provide URL to the engine
	_strModURL = "http://www.croteam.com/mods/TheSecondEncounter";
}

// internal cleanup
void CGame::EndInternal(void)
{
	// stop game if eventually started
	StopGame();

	// 프로세스 검사용 이벤트 핸들 및 스레드
	delete gmUpdateInputState;
	gmUpdateInputState = NULL;
	CloseHandle(gmUpdateHandle);

	// remove game timer handler
	_pTimer->RemHandler( &m_gthGameTimerHandler);
	// save persistent symbols
	if (!_bDedicatedServer) {
		_pShell->StoreEncodePersistentSymbols(fnmPersistentSymbols);
	}

	LCDEnd();

	// stop and delete any playing sound
	#define MAX_SCRIPTSOUNDS 16
	for( INDEX i=0; i<MAX_SCRIPTSOUNDS; i++) {
		if( _apsoScriptChannels[i]==NULL) continue;
		_apsoScriptChannels[i]->Stop();
		delete _apsoScriptChannels[i];
	}

	// save console history
	CTString strConsole = gam_strConsoleInputBuffer;
	strConsole.TrimLeft(8192);
	try {
		//strConsole.SaveKeepCRLF_t(fnmConsoleHistory);
	} catch (char *strError) {
		WarningMessage(TRANS("Cannot save console history:\n%s"), strError);
	}
	SavePlayersAndControls();

	// save game shell settings
	try {
		Save_t();
	} catch (char *strError) {
		WarningMessage("Cannot load game settings:\n%s\nUsing defaults!", strError);
	}
}

//0524 kwon 함수추가.
BOOL CGame::PreNewGame()
{
	gam_iObserverConfig = 0;
	gam_iObserverOffset = 0;
//강동민 수정 시작 클로즈 준비 작업	08.10
	extern BOOL _bLoginProcess;
	// stop eventually running game
	if(!_bLoginProcess)
//강동민 수정 끝 클로즈 준비 작업		08.10
		StopGame();

	/*
	CEnableUserBreak eub;
	if (!gm_bFirstLoading) 
	{
		_bUserBreakEnabled = FALSE;
	}
	*/

	// try to start current network provider
	if( !StartProviderFromName()) 
	{
		return FALSE;
		gm_bFirstLoading = FALSE;
	}

	return TRUE;
}

//강동민 수정 끝 클로즈 준비 작업		08.10
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &strSessionName - 
//			&fnWorld - 
//			&sp - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CGame::NewGame(const CTString &strSessionName, 
					const CTFileName &fnWorld,
	 CSessionProperties &sp)
{
#if _SE_DEMO
	try {
		ULONG ulCRC = GetFileCRC32_t(fnWorld);
		if (ulCRC!=0x5cf3f219/*intro*/ && ulCRC!=0x9f2ae3f6/*splevel*/ && ulCRC!=0x61586e9d/*techtest*/ && ulCRC!=0x4060d0dd/*dmlevel*/) {
			ThrowF_t(TRANS("Wrong CRC check."));
			//ThrowF_t("Wrong CRC check (%x).", ulCRC); 
		}
	} catch (char *strError) {
		CPrintF("%s\n", strError);
		return FALSE;              
	}
#endif
/* //0524 kwon 삭제.
	gam_iObserverConfig = 0;
	gam_iObserverOffset = 0;
	// stop eventually running game
	StopGame();

	CEnableUserBreak eub;
	if (!gm_bFirstLoading) {
		_bUserBreakEnabled = FALSE;
	}

	// try to start current network provider
	if( !StartProviderFromName()) {
		return FALSE;
		gm_bFirstLoading = FALSE;
	}
*/
	// clear profile array and start game
	_atmFrameTimes.Clear();
	_actTriangles.Clear();

	// start the new session
	try 
	{
//강동민 수정 시작 클로즈 준비 작업	08.10
		/*
		if( dem_bPlay) 
		{
			gm_aiStartLocalPlayers[0] = -2;

			CTFileName fnmDemo = CTString("Temp\\Play.dem");
			if( dem_bPlayByName) 
			{
				fnmDemo = fnWorld;
			}
			_pNetwork->StartDemoPlay_t(fnmDemo);
		} 
		else
		*/
//강동민 수정 끝 클로즈 준비 작업		08.10
		{
			BOOL bWaitAllPlayers = sp.sp_bWaitAllPlayers && _pNetwork->IsNetworkEnabled();
			_pNetwork->StartPeerToPeer_t( strSessionName, fnWorld, 
				sp.sp_ulSpawnFlags, sp.sp_ctMaxPlayers, bWaitAllPlayers, &sp);
			_pNetwork->MainLoop();
		}
	} 
	catch (char *strError) 
	{
		gm_bFirstLoading = FALSE;
		// stop network provider
		_pNetwork->StopProvider();
		// and display error
		CPrintF(TRANS("Cannot start game:\n%s\n"), strError);
		return FALSE;
	}

	// setup players from given indices
	SetupLocalPlayers();

	// 플레이어가 없다면???  그리고 데모 플레이 중?
	if( !dem_bPlay && !AddPlayers()) 
	{
		_pNetwork->StopGame();
		_pNetwork->StopProvider();
		gm_bFirstLoading = FALSE;
		return FALSE;
	}
	gm_bFirstLoading = FALSE;
//강동민 수정 시작 클로즈 준비 작업	08.10
	extern BOOL	_bWorldEditorApp;
	//if(_bWorldEditorApp)
	//{
	gm_bGameOn = TRUE;
	//}
//강동민 수정 끝 클로즈 준비 작업		08.10
	gm_CurrentSplitScreenCfg = gm_StartSplitScreenCfg;
	// clear last set highscore
//	gm_iLastSetHighScore = -1;

	MaybeDiscardLastLines();
	return TRUE;
}

/*
BOOL CGame::JoinGame(CNetworkSession &session)
{
	CEnableUserBreak eub;
	gam_iObserverConfig = 0;
	gam_iObserverOffset = 0;

	// stop eventually running game
	StopGame();

	// try to start current network provider
	if( !StartProviderFromName()) return FALSE;

	// start the new session
	try 
	{
		INDEX ctLocalPlayers = 0;
//강동민 수정 시작 클로즈 준비 작업	08.10
		if (gm_StartSplitScreenCfg>=SSC_PLAY1) 
//강동민 수정 끝 클로즈 준비 작업		08.10
		{
			ctLocalPlayers = (gm_StartSplitScreenCfg-SSC_PLAY1)+1;
		}
		_pNetwork->JoinSession_t(session, ctLocalPlayers);
	} 
	catch (char *strError) 
	{
		// stop network provider
		_pNetwork->StopProvider();
		// and display error
		CPrintF(TRANS("Cannot join game:\n%s\n"), strError);
		return FALSE;
	}

	// setup players from given indices
	SetupLocalPlayers();

	// 플레이어가 없다면???
	if( !AddPlayers())
	{
		_pNetwork->StopGame();
		_pNetwork->StopProvider();
		return FALSE;
	}
	gm_bGameOn = TRUE;
	gm_CurrentSplitScreenCfg = gm_StartSplitScreenCfg;
	return TRUE;
}

BOOL CGame::LoadGame(const CTFileName &fnGame)
{
	gam_iObserverConfig = 0;
	gam_iObserverOffset = 0;

	// stop eventually running game
	StopGame();

	// try to start current network provider
	if( !StartProviderFromName()) return FALSE;

	// start the new session
	try {
		_pNetwork->Load_t( fnGame);
		CPrintF(TRANS("Loaded game: %s\n"), fnGame);
	} catch (char *strError) {
		// stop network provider
		_pNetwork->StopProvider();
		// and display error
		CPrintF(TRANS("Cannot load game: %s\n"), strError);
		return FALSE;
	}

	// setup players from given indices
	SetupLocalPlayers();

	// 플레이어가 없다면???
	if( !AddPlayers())
	{
		_pNetwork->StopGame();
		_pNetwork->StopProvider();
		return FALSE;
	}
	gm_bGameOn = TRUE;
	gm_CurrentSplitScreenCfg = gm_StartSplitScreenCfg;
	// clear last set highscore
	//gm_iLastSetHighScore = -1;

	// if it was a quicksave, and not the newest one
	if (fnGame.Matches("*\\QuickSave*") && fnGame!=GetQuickSaveName(FALSE)) {
		// mark that it should be resaved as newest
		gam_bQuickSave = TRUE;
	}


	MaybeDiscardLastLines();
	return TRUE;
}

BOOL CGame::StartDemoPlay(const CTFileName &fnDemo)
{
	CEnableUserBreak eub;

	// stop eventually running game
	StopGame();

	// try to start current network provider
	if( !StartProviderFromName()) {
		gm_bFirstLoading = FALSE;
		return FALSE;
	}

	// start the new session
	try {
		_pNetwork->StartDemoPlay_t( fnDemo);
		CPrintF(TRANS("Started playing demo: %s\n"), fnDemo);
	} catch (char *strError) {
		// stop network provider
		_pNetwork->StopProvider();
		// and display error
		CPrintF(TRANS("Cannot play demo: %s\n"), strError);
		gm_bFirstLoading = FALSE;
		return FALSE;
	}
	gm_bFirstLoading = FALSE;

	// setup players from given indices
	gm_StartSplitScreenCfg = CGame::SSC_OBSERVER;
	SetupLocalPlayers();
	gm_bGameOn = TRUE;
	gm_CurrentSplitScreenCfg = gm_StartSplitScreenCfg;

	// prepare array for eventual profiling
	_atmFrameTimes.PopAll();
	_actTriangles.PopAll();
	_tvDemoStarted = _pTimer->GetHighPrecisionTimer();
	_tvLastFrame   = _tvDemoStarted;

	CTFileName fnmScript = fnDemo.NoExt()+".ini";
	if (!FileExists(fnmScript)) {
		fnmScript = CTString("Demos\\Default.ini");
	}
	CTString strCmd;
	strCmd.PrintF("include \"%s\"", (const char*)fnmScript);
	_pShell->Execute(strCmd);

	MaybeDiscardLastLines();
	// all done
	return TRUE;
}


BOOL CGame::StartDemoRec(const CTFileName &fnDemo)
{
	// save demo recording
	try {
		_pNetwork->StartDemoRec_t( fnDemo);
		CPrintF(TRANS("Started recording demo: %s\n"), fnDemo);
		// save a thumbnail
		SaveThumbnail(fnDemo.NoExt()+"Tbn.tex");
		return TRUE;
	} catch (char *strError) {
		// and display error
		CPrintF(TRANS("Cannot start recording: %s\n"), strError);
		return FALSE;
	}
}


void CGame::StopDemoRec(void)
{
	// if no game is currently running
	if (!gm_bGameOn) return;

	_pNetwork->StopDemoRec();
	CPrintF(TRANS("Finished recording.\n"));
}

BOOL CGame::SaveGame(const CTFileName &fnGame)
{
	// if no live players
	INDEX ctPlayers = GetPlayersCount();
	INDEX ctLivePlayers = GetLivePlayersCount();
	if (ctPlayers>0 && ctLivePlayers<=0) {
		// display error
		CPrintF(TRANS("Won't save game when dead!\n"));
		// do not save
		return FALSE;
	}

	// save new session
	try {
		_pNetwork->Save_t( fnGame);
		CPrintF(TRANS("Saved game: %s\n"), fnGame);
		SaveThumbnail(fnGame.NoExt()+"Tbn.tex");
		return TRUE;
	} catch (char *strError) {
		// and display error
		CPrintF(TRANS("Cannot save game: %s\n"), strError);
		return FALSE;
	}
}
*/

void CGame::StopGame(void)
{
	// disable computer quickly
	//ComputerForceOff();

	// if no game is currently running
	if (!gm_bGameOn)
	{
		// do nothing
		return;
	}
	// disable direct input
//  _pInput->DisableInput();
	// and game
	gm_bGameOn = FALSE;
	// stop the game
	_pNetwork->StopGame();
	// stop the provider
	if (_pNetwork->bMoveCharacterSelectUI == FALSE)
	{
		_pNetwork->StopProvider();
	}	
	// for all four local players
//강동민 수정 시작 로그인 변경 작업	08.10
	//for( INDEX iPlayer=0; iPlayer<4; iPlayer++)
	for( INDEX iPlayer=0; iPlayer<NET_MAXGAMEPLAYERS; iPlayer++)
//강동민 수정 끝 로그인 변경 작업		08.10
	{
		// mark that current player does not exist any more
		gm_lpLocalPlayers[ iPlayer].lp_bActive = FALSE;
		gm_lpLocalPlayers[ iPlayer].lp_pplsPlayerSource = NULL;
	}
}

BOOL CGame::StartProviderFromName(void)
{
	BOOL bSuccess = FALSE;
	// list to contain available network providers
	CListHead lhAvailableProviders;
	// enumerate all providers
	_pNetwork->EnumNetworkProviders( lhAvailableProviders);

	// for each provider
	FOREACHINLIST(CNetworkProvider, np_Node, lhAvailableProviders, litProviders)
	{
		// generate provider description
		CTString strProviderName = litProviders->GetDescription();
		// is this the provider we are searching for ?
		if( strProviderName == gm_strNetworkProvider)
		{
			// it is, set it as active network provider
			gm_npNetworkProvider = litProviders.Current();
			bSuccess = TRUE;
			break;
		}
	}

	// delete list of providers
	FORDELETELIST(CNetworkProvider, np_Node, lhAvailableProviders, litDelete)
	{
		delete &litDelete.Current();
	}

	// try to
	try
	{
		// start selected network provider
		_pNetwork->StartProvider_t( gm_npNetworkProvider);
	}
	// catch throwed error
	catch (char *strError)
	{
		// if unable, report error
		CPrintF( TRANS("Can't start provider:\n%s\n"), strError);
		bSuccess = FALSE;
	}
	return bSuccess;
}

/*
CHighScoreEntry::CHighScoreEntry(void)
{
	hse_strPlayer = "";
	hse_gdDifficulty = (CSessionProperties::GameDifficulty)-100;
	hse_tmTime = -1.0f;
	hse_ctKills = -1;
	hse_ctScore = 0;
}
SLONG CGame::PackHighScoreTable(void)
{
	// start at the beginning of buffer
	UBYTE *pub = _aubHighScoreBuffer;
	// for each entry
	for (INDEX i=0; i<HIGHSCORE_COUNT; i++) {
		// make its string
		char str[MAX_HIGHSCORENAME+1];
		memset(str, 0, sizeof(str));
		strncpy(str, gm_ahseHighScores[i].hse_strPlayer, MAX_HIGHSCORENAME);
		// copy the value and the string
		memcpy(pub, str, sizeof(str));
		pub += MAX_HIGHSCORENAME+1;
		memcpy(pub, &gm_ahseHighScores[i].hse_gdDifficulty, sizeof(INDEX));
		pub += sizeof(INDEX);
		memcpy(pub, &gm_ahseHighScores[i].hse_tmTime,       sizeof(FLOAT));
		pub += sizeof(FLOAT);
		memcpy(pub, &gm_ahseHighScores[i].hse_ctKills,      sizeof(INDEX));
		pub += sizeof(INDEX);
		memcpy(pub, &gm_ahseHighScores[i].hse_ctScore,      sizeof(INDEX));
		pub += sizeof(INDEX);
	}
	// just copy it for now
	memcpy(_aubHighScorePacked, _aubHighScoreBuffer, MAX_HIGHSCORETABLESIZE);
	return MAX_HIGHSCORETABLESIZE;
}

void CGame::UnpackHighScoreTable(SLONG slSize)
{
	// just copy it for now
	memcpy(_aubHighScoreBuffer, _aubHighScorePacked, slSize);
	// start at the beginning of buffer
	UBYTE *pub = _aubHighScoreBuffer;
	// for each entry
	for (INDEX i=0; i<HIGHSCORE_COUNT; i++) {
		gm_ahseHighScores[i].hse_strPlayer = (const char*)pub;
		pub += MAX_HIGHSCORENAME+1;
		memcpy(&gm_ahseHighScores[i].hse_gdDifficulty, pub, sizeof(INDEX));
		pub += sizeof(INDEX);
		memcpy(&gm_ahseHighScores[i].hse_tmTime      , pub, sizeof(FLOAT));
		pub += sizeof(FLOAT);
		memcpy(&gm_ahseHighScores[i].hse_ctKills     , pub, sizeof(INDEX));
		pub += sizeof(INDEX);
		memcpy(&gm_ahseHighScores[i].hse_ctScore     , pub, sizeof(INDEX));
		pub += sizeof(INDEX);
	}

	// try to
	try {
		CTFileStream strm;
		strm.Open_t(CTString("table.txt"));
		{for (INDEX i=0; i<HIGHSCORE_COUNT; i++) {
			gm_ahseHighScores[i].hse_gdDifficulty = (CSessionProperties::GameDifficulty)-100;
		}}
		{for (INDEX i=0; i<HIGHSCORE_COUNT; i++) {
			CTString strLine;
			strm.GetLine_t(strLine);
			char strName[256];
			strLine.ScanF("\"%256[^\"]\" %d %g %d %d", 
				strName, 
				&gm_ahseHighScores[i].hse_gdDifficulty,
				&gm_ahseHighScores[i].hse_tmTime,
				&gm_ahseHighScores[i].hse_ctKills,
				&gm_ahseHighScores[i].hse_ctScore);
			gm_ahseHighScores[i].hse_strPlayer = strName;
		}}
	} catch (char *strError) {
		(void)strError;
	}

	// remember best for player hud and statistics
	plr_iHiScore = gm_ahseHighScores[0].hse_ctScore;

	// no last set
	gm_iLastSetHighScore = -1;
}
*/

void CGame::Load_t( void)
{
	ASSERT( gm_fnSaveFileName != "");
	CTFileStream strmFile;
	// open file with saved CGameObject
	strmFile.Open_t( gm_fnSaveFileName,CTStream::OM_READ);
	// read file ID
	strmFile.ExpectID_t( CChunkID( "GAME"));  // game
	// check version number
	if( !( CChunkID(GAME_SHELL_VER) == strmFile.GetID_t()) )
	{
		throw( TRANS("Invalid version of game shell."));
	}
	// load all of the class members
	strmFile>>gm_strNetworkProvider;
	strmFile>>gm_iWEDSinglePlayer;
	strmFile>>gm_iSinglePlayer;
	strmFile>>gm_aiMenuLocalPlayers[0];
	//strmFile>>gm_aiMenuLocalPlayers[1];
	//strmFile>>gm_aiMenuLocalPlayers[2];
	//strmFile>>gm_aiMenuLocalPlayers[3];

//	strmFile.Read_t( &gm_MenuSplitScreenCfg, sizeof( enum SplitScreenCfg));

	// read high-score table
//	SLONG slHSSize;
//	strmFile>>slHSSize;
//	strmFile.Read_t(&_aubHighScorePacked, slHSSize);
	//UnpackHighScoreTable(slHSSize);
}

void CGame::Save_t( void)
{
	ASSERT( gm_fnSaveFileName != "");
	CTFileStream strmFile;
	// create file to save CGameObject
	strmFile.Create_t( gm_fnSaveFileName);
	// write file ID
	strmFile.WriteID_t( CChunkID( "GAME"));  // game shell
	// write version number
	strmFile.WriteID_t( CChunkID(GAME_SHELL_VER));

	// save all of the class members
	strmFile<<gm_strNetworkProvider;
	strmFile<<gm_iWEDSinglePlayer;
	strmFile<<gm_iSinglePlayer;
	strmFile<<gm_aiMenuLocalPlayers[0];
	//strmFile<<gm_aiMenuLocalPlayers[1];
	//strmFile<<gm_aiMenuLocalPlayers[2];
	//strmFile<<gm_aiMenuLocalPlayers[3];

//	strmFile.Write_t( &gm_MenuSplitScreenCfg, sizeof( enum SplitScreenCfg));

	// write high-score table
	//SLONG slHSSize = PackHighScoreTable();
	//strmFile<<slHSSize;
	//strmFile.Write_t(&_aubHighScorePacked, slHSSize);
}

void LoadControls(CControls &ctrl, INDEX i)
{
	try {
		CTFileName fnm;
		fnm.PrintF("Data\\etc\\Controls%d.ctl", i);
		ctrl.Load_t(fnm);
	} catch (char *strError) {
		(void) strError; 
		try {
//			ctrl.Load_t(CTFILENAME("Controls\\00-Default.ctl"));
		} catch (char *strError) {
			(void) strError; 
			ctrl.SwitchToDefaults();
		}
	}
}

void LoadPlayer(CPlayerCharacter &pc, INDEX i)
{
	try {
		CTFileName fnm;
		fnm.PrintF("Data\\etc\\Player%d.plr", i);
		pc.Load_t(fnm);
		CPlayerSettings *pps = (CPlayerSettings *)pc.pc_aubAppearance;
		pps->ps_iSensitivity = NormFloatToByte(_pGame->gm_actrlControls[i].ctrl_fSensitivity/100 - 0.5);
	} catch (char *strError) {
		(void) strError;
		CTString strName;
		if (i==0) {
			LoadStringVar(CTString("Data\\Var\\DefaultPlayer.var"), strName);
			strName.OnlyFirstLine();
		}
		if (strName=="") {
			strName.PrintF("Player %d", i);
		}
		pc = CPlayerCharacter(strName);
	}
}


/*
 * Loads 8 players and 8 controls
 */
void CGame::LoadPlayersAndControls( void)
{
//강동민 수정 시작 클로즈 준비 작업	08.10
	for (INDEX iControls=0; iControls<1; iControls++) {
//강동민 수정 끝 클로즈 준비 작업		08.10
		LoadControls(gm_actrlControls[iControls], iControls);
	}

//강동민 수정 시작 클로즈 준비 작업	08.10
	for (INDEX iPlayer=0; iPlayer<1; iPlayer++) {
//강동민 수정 끝 클로즈 준비 작업		08.10
		LoadPlayer(gm_apcPlayers[iPlayer], iPlayer);
	}

	SavePlayersAndControls();
}

/*
 * Saves 8 players and 8 controls
 */
void CGame::SavePlayersAndControls( void)
{
	try
	{
		// save players
		gm_apcPlayers[0].Save_t( CTString( "Data\\etc\\Player0.plr"));
		/*gm_apcPlayers[1].Save_t( CTString( "Players\\Player1.plr"));
		gm_apcPlayers[2].Save_t( CTString( "Players\\Player2.plr"));
		gm_apcPlayers[3].Save_t( CTString( "Players\\Player3.plr"));
		gm_apcPlayers[4].Save_t( CTString( "Players\\Player4.plr"));
		gm_apcPlayers[5].Save_t( CTString( "Players\\Player5.plr"));
		gm_apcPlayers[6].Save_t( CTString( "Players\\Player6.plr"));
		gm_apcPlayers[7].Save_t( CTString( "Players\\Player7.plr"));*/
		// save controls
		gm_actrlControls[0].Save_t( CTString( "Data\\etc\\Controls0.ctl"));
		/*gm_actrlControls[1].Save_t( CTString( "Controls\\Controls1.ctl"));
		gm_actrlControls[2].Save_t( CTString( "Controls\\Controls2.ctl"));
		gm_actrlControls[3].Save_t( CTString( "Controls\\Controls3.ctl"));
		gm_actrlControls[4].Save_t( CTString( "Controls\\Controls4.ctl"));
		gm_actrlControls[5].Save_t( CTString( "Controls\\Controls5.ctl"));
		gm_actrlControls[6].Save_t( CTString( "Controls\\Controls6.ctl"));
		gm_actrlControls[7].Save_t( CTString( "Controls\\Controls7.ctl"));*/
	}
	// catch throwed error
	catch (char *strError)
	{
		(void) strError;
	}

	// skip checking of players if game isn't on
	if( !gm_bGameOn) return;

	// for each local player
//강동민 수정 시작 로그인 변경 작업	08.10
	//for( INDEX i=0; i<4; i++) 
	for( INDEX i=0; i<NET_MAXGAMEPLAYERS; i++) 
//강동민 수정 끝 로그인 변경 작업		08.10
	{		
		CLocalPlayer &lp = gm_lpLocalPlayers[i];
		// if active
		if( lp.lp_bActive && lp.lp_pplsPlayerSource!=NULL && lp.lp_iPlayer>=0 && lp.lp_iPlayer<8) 
		{
			// if its character in game is different than the one in config
			CPlayerCharacter &pcInGame = lp.lp_pplsPlayerSource->pls_pcCharacter;
			CPlayerCharacter &pcConfig = gm_apcPlayers[lp.lp_iPlayer];
			if( pcConfig.pc_strName!=pcInGame.pc_strName			 
			 || memcmp(pcConfig.pc_aubAppearance, pcInGame.pc_aubAppearance, sizeof(pcInGame.pc_aubAppearance))!=0 ) 
			{
				// demand change in game
				lp.lp_pplsPlayerSource->ChangeCharacter(pcConfig);
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CGame::SetupLocalPlayers( void)
{
	// setup local players and their controls
	gm_lpLocalPlayers[0].lp_iPlayer = gm_aiStartLocalPlayers[0];
//강동민 수정 시작 로그인 변경 작업	08.10
	//gm_lpLocalPlayers[1].lp_iPlayer = gm_aiStartLocalPlayers[1];
	//gm_lpLocalPlayers[2].lp_iPlayer = gm_aiStartLocalPlayers[2];
	//gm_lpLocalPlayers[3].lp_iPlayer = gm_aiStartLocalPlayers[3];
//강동민 수정 끝 로그인 변경 작업		08.10
	if (gm_StartSplitScreenCfg < CGame::SSC_PLAY1) 
	{
		gm_lpLocalPlayers[0].lp_iPlayer = -1;
	}
//강동민 수정 시작 로그인 변경 작업	08.10
	/*
	if (gm_StartSplitScreenCfg < CGame::SSC_PLAY2) {
		gm_lpLocalPlayers[1].lp_iPlayer = -1;
	}
	if (gm_StartSplitScreenCfg < CGame::SSC_PLAY3) {
		gm_lpLocalPlayers[2].lp_iPlayer = -1;
	}
	if (gm_StartSplitScreenCfg < CGame::SSC_PLAY4) {
		gm_lpLocalPlayers[3].lp_iPlayer = -1;
	}
	*/
//강동민 수정 끝 로그인 변경 작업		08.10
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CGame::AddPlayers(void)
{
	// add local player(s) into game
	try 
	{
		for(INDEX i=0; i<NET_MAXGAMEPLAYERS; i++) 
		{
			CLocalPlayer &lp = gm_lpLocalPlayers[i];
			INDEX iPlayer = lp.lp_iPlayer;
			if (iPlayer>=0) 
			{
				ASSERT(iPlayer>=0 && iPlayer<8);
				lp.lp_pplsPlayerSource = _pNetwork->AddPlayer_t(gm_apcPlayers[iPlayer]);
				gm_apcPlayers[iPlayer].pc_iPlayerIndex = lp.lp_pplsPlayerSource->pls_pcCharacter.pc_iPlayerIndex;
				lp.lp_bActive = TRUE;
			}
		}
	} 
	catch (char *strError) 
	{
		CPrintF(TRANS("Cannot add player:\n%s\n"), strError);
		return FALSE;
	}

	return TRUE;
}

// save thumbnail for savegame
static CTFileName _fnmThumb=CTString("");
void CGame::SaveThumbnail( const CTFileName &fnm)
{
	// request saving of thumbnail only (need drawport)
	// (saving will take place in GameRedrawView())
	_fnmThumb = fnm;
}


// timer variables
#define FRAMES_AVERAGING_MAX 20L
static  CTimerValue _tvLasts[FRAMES_AVERAGING_MAX];
static  CTimerValue _tvDelta[FRAMES_AVERAGING_MAX];
static  INDEX _iCheckNow  = 0;
static  INDEX _iCheckMax  = 0;
#define FPS_LAST_CACHED 120
static  INDEX _aiFPS[FPS_LAST_CACHED];
static  INDEX _iStartingFPSIndex = 0;
static  INDEX _iLastFPSOut = 0;


// print resolution, frame rate or extensive profiling, and elapsed time
static void PrintStats( CDrawPort *pdpDrawPort)
{
	// cache some general vars
	const SLONG slDPWidth  = pdpDrawPort->GetWidth();
	const SLONG slDPHeight = pdpDrawPort->GetHeight();
	// determine proper text scale for statistics display
	const FLOAT fTextScale = slDPHeight/480.0f *0.5f;
	
	// display resolution info (if needed)
	if( hud_bShowResolution) {
		CTString strRes;
		strRes.PrintF( "%dx%dx%s", slDPWidth, slDPHeight, _pGfx->gl_dmCurrentDisplayMode.DepthString());
		pdpDrawPort->SetFont( _pfdDisplayFont);
		pdpDrawPort->SetTextScaling( fTextScale);
		pdpDrawPort->SetTextAspect( 1.0f);
		pdpDrawPort->SetTextShadow( +2);
		pdpDrawPort->PutTextC( strRes, slDPWidth*0.5f, slDPHeight*0.15f, C_WHITE|255);
	}

	// if required, print elapsed playing time
	if( hud_bShowTime) {
//안태훈 수정 시작	//(Add Sun Moon Entity and etc)(0.2)
		// set font, spacing and scale
		pdpDrawPort->SetFont( _pfdDisplayFont);
		pdpDrawPort->SetTextScaling( fTextScale * 0.5 );
		pdpDrawPort->SetTextAspect( 1.0f);
		pdpDrawPort->SetTextShadow( +2);
		INDEX currentHour = INDEX(g_fGWTime / (FLOAT)GAMETIME_ONEHOUR_SECOND);
		INDEX currentMinute = INDEX( (g_fGWTime - (currentHour * GAMETIME_ONEHOUR_SECOND)) / (FLOAT)GAMETIME_ONEMINUTE_SECOND );
		INDEX currentSecond = INDEX( g_fGWTime - (currentHour * (FLOAT)GAMETIME_ONEHOUR_SECOND) - (currentMinute * (FLOAT)GAMETIME_ONEMINUTE_SECOND) );
		CTString strTime;
		strTime.PrintF( "GameWorldTime : %02d:%02d:%02d\nGWTimeMul : %6.2f\nRWT2GWT : %6.2f", currentHour, currentMinute, currentSecond, _pShell->GetFLOAT("g_fGWTimeMul"), RWTOneSec2GWTSec);
		pdpDrawPort->PutTextC( strTime, slDPWidth*0.5f, slDPHeight*0.03f, C_WHITE|CT_OPAQUE);
//안태훈 수정 끝	//(Add Sun Moon Entity and etc)(0.2)
	}

	// if required, print real time
	if( hud_bShowClock) {
		// set font, spacing and scale
		pdpDrawPort->SetFont( _pfdConsoleFont);
		pdpDrawPort->SetTextScaling(1.0f);
		pdpDrawPort->SetTextAspect( 1.0f);
		pdpDrawPort->SetTextShadow( +1);
		// determine time
		struct tm *newtime;
		time_t long_time;
		time(&long_time);
		newtime = localtime(&long_time);
		// printout
		CTString strTime;
		strTime.PrintF( "%2d:%02d", newtime->tm_hour, newtime->tm_min);
		pdpDrawPort->PutTextR( strTime, slDPWidth-3, 2, C_lYELLOW|CT_OPAQUE);
	}

	// if required, draw netgraph
	if( hud_bShowNetGraph)
	{ // determine and clamp size
		INDEX ctLines = _pNetwork->ga_angeNetGraph.Count();
		ctLines = ClampUp( ctLines, (INDEX)(slDPHeight*0.7f));
		FLOAT f192oLines = 192.0f / (FLOAT)ctLines;
		const FLOAT fMaxWidth = slDPWidth  *0.1f;
		const PIX pixJ = slDPHeight *0.9f;
		// draw canvas
		pdpDrawPort->Fill(       slDPWidth-1-fMaxWidth, pixJ-ctLines+1, fMaxWidth,   ctLines,   SE_COL_BLUE_DARK_HV|64);
		pdpDrawPort->DrawBorder( slDPWidth-2-fMaxWidth, pixJ-ctLines,   fMaxWidth+2, ctLines+2, C_WHITE  |192);
		// draw graph
		for( INDEX i=0; i<ctLines; i++) {
			FLOAT fValue  = _pNetwork->ga_angeNetGraph[i].nge_fLatency;
			enum NetGraphEntryType nge = _pNetwork->ga_angeNetGraph[i].nge_ngetType;
			FLOAT fWidth = Clamp( fValue, 0.0f, 1.0f)*fMaxWidth;
			COLOR colLine = C_GREEN;                 
					 if( nge==NGET_ACTION)            colLine = C_GREEN;  // normal action (default)
			else if( nge==NGET_MISSING)           colLine = C_RED;    // missing sequence
			else if( nge==NGET_NONACTION)         colLine = C_WHITE;  // non-action sequence
			else if( nge==NGET_REPLICATEDACTION)  colLine = C_BLUE;   // duplicated action
			else if( nge==NGET_SKIPPEDACTION)     colLine = C_YELLOW; // skipped action
			else                 colLine = C_BLACK;  // unknown ???
			ULONG ulAlpha = FloatToInt( ((FLOAT)ctLines-(i*0.3333f)) *f192oLines);
			pdpDrawPort->DrawLine( slDPWidth-2, pixJ-i, slDPWidth-2-fWidth, pixJ-i, colLine|ulAlpha);
		}
	}

	// clamp stat values
	extern ENGINE_API INDEX _iStatsMode;
	hud_iStats  = Clamp( hud_iStats, 0L, 4L);
	_iStatsMode = hud_iStats; // send it to engine
	hud_iFPSGraphSpeed = Clamp( hud_iFPSGraphSpeed, 0L, 50L);
	hud_iFPSGraphMax   = Clamp( hud_iFPSGraphMax, 50L, 500L);

//강동민 수정 시작 사내 테스트 작업	08.10
	// NOTE : FRAME을 안보이게 하기 위한 부분.
	// if stats aren't required
	if( hud_iStats==0 ) {
		// display nothing
		_iCheckNow = 0;
		_iCheckMax = 0;
		return;
	}
//강동민 수정 끝 사내 테스트 작업		08.10

	// calculate FPS
	FLOAT fFPS = 0.0f;
	_iCheckMax++;
	if( _iCheckMax >= FRAMES_AVERAGING_MAX) {
		for( INDEX i=0; i<FRAMES_AVERAGING_MAX; i++) fFPS += _tvDelta[i].GetSeconds();
		fFPS = FRAMES_AVERAGING_MAX*FRAMES_AVERAGING_MAX / fFPS;
		_iCheckMax = FRAMES_AVERAGING_MAX;
	}

	// determine newest time
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
	_tvDelta[_iCheckNow] = tvNow - _tvLasts[_iCheckNow];
	_tvLasts[_iCheckNow] = tvNow;
	_iCheckNow = (_iCheckNow+1) % FRAMES_AVERAGING_MAX;

	// fill FPS graph
	INDEX iTimeNow = tvNow.GetSeconds()*2*hud_iFPSGraphSpeed;
	if( iTimeNow!=_iLastFPSOut && iTimeNow%2) {
		_iStartingFPSIndex = (_iStartingFPSIndex+1)%FPS_LAST_CACHED;
		_aiFPS[_iStartingFPSIndex] = fFPS;
		_iLastFPSOut = iTimeNow;
	}

	// set display interface (proportional) font
	pdpDrawPort->SetFont( _pfdDisplayFont);
	pdpDrawPort->SetTextScaling( fTextScale);
	pdpDrawPort->SetTextAspect( 1.0f);
	pdpDrawPort->SetTextShadow( +2);

	// signal debug mode!
	CTString strDebug;
#ifndef NDEBUG
	strDebug = "DEBUG:";
#else
	strDebug = "";
#endif

	// display colored FPS 
	COLOR colFPS = C_RED;
	if( fFPS >= 20) colFPS = C_GREEN;
	if( fFPS >= 60) colFPS = C_WHITE;
	if( fFPS <  10) pdpDrawPort->SetTextScaling( fTextScale*1.5); // enlarge output if FPS is extremely low

	// prepare FPS string for printing
	CTString strFPS = "?";
			 if( fFPS >= 30)   strFPS.PrintF( "%s%3.0f", (const char*)strDebug, fFPS);
	else if( fFPS >= 0.1f) strFPS.PrintF( "%s%3.1f", (const char*)strDebug, fFPS);
	// printout FPS number (if allowed)
	pdpDrawPort->PutTextC( strFPS, slDPWidth*0.75f, slDPHeight*0.005f, colFPS|192);

	extern ENGINE_API INDEX  cli_iCurrentFPS;
	cli_iCurrentFPS = fFPS;
	//CPrintF("fFps: %f, ic: %d\n",fFPS,cli_iCurrentFPS);
	// if in extensive stats mode
	if( hud_iStats>1 && hud_iStats<4)
	{ // display extensive statistics
		CTString strReport;
		STAT_Report(strReport);
		STAT_Reset();

		// adjust and set font
		pdpDrawPort->SetFont( _pfdConsoleFont);
		pdpDrawPort->SetTextScaling( 1.0f);
		pdpDrawPort->SetTextShadow( +1);

		// put filter
		pdpDrawPort->Fill( 0,0, 150,slDPHeight, C_BLACK|200, C_BLACK|0, C_BLACK|200, C_BLACK|0);
		// printout statistics
		strFPS.PrintF( " frame =%3.0f ms\n---------------\n", 1000.0f/fFPS);
		pdpDrawPort->PutText( strFPS,    0, 30, C_WHITE|CT_OPAQUE);
		pdpDrawPort->PutText( strReport, 4, 55, C_GREEN|CT_OPAQUE);
	}

	// if in FPS graph stats mode
	if( hud_iStats>2 && !hud_bShowNetGraph)
	{ // draw canvas
		const INDEX iWidth  = FPS_LAST_CACHED;
		const INDEX iHeight = 60;
		const INDEX iX = pdpDrawPort->GetWidth() *0.98f - iWidth;
		const INDEX iY = pdpDrawPort->GetHeight()*0.91f - iHeight;
		pdpDrawPort->Fill( iX, iY, iWidth, iHeight, SE_COL_BLUE_DARK_HV|64);
		pdpDrawPort->DrawBorder( iX-1, iY-1, iWidth+2, iHeight+2, C_WHITE|192);
				
		// calculate FPS
		FLOAT fFPS = 0.0f;
		_iCheckMax++;
		if( _iCheckMax >= FRAMES_AVERAGING_MAX) {
			for( INDEX i=0; i<FRAMES_AVERAGING_MAX; i++) fFPS += _tvDelta[i].GetSeconds();
			fFPS = FRAMES_AVERAGING_MAX*FRAMES_AVERAGING_MAX / fFPS;
			_iCheckMax = FRAMES_AVERAGING_MAX;
		}
		
		// draw graph lines
		for( INDEX i=0; i<FPS_LAST_CACHED; i++) {
			const INDEX iValue = ClampUp(_aiFPS[(i+_iStartingFPSIndex+1)%FPS_LAST_CACHED], hud_iFPSGraphMax);  
			COLOR iCol = C_RED;
			if( iValue>100) iCol = C_lBLUE;
			if( iValue> 60) iCol = C_WHITE;
			if( iValue> 20) iCol = C_GREEN;   
			const INDEX iNormalizedValue = ((FLOAT)iValue)/hud_iFPSGraphMax * iHeight;      
			pdpDrawPort->DrawLine( iX+i, iY+iHeight-1, iX+i, iY+(iHeight-iNormalizedValue), iCol|196);
		}       

		// print max normalization value
		CTString strMaxFPS;
		strMaxFPS.PrintF( "%d", hud_iFPSGraphMax);
		pdpDrawPort->SetFont( _pfdConsoleFont);
		pdpDrawPort->SetTextScaling( 1.0f);
		pdpDrawPort->SetTextShadow( +1);
		pdpDrawPort->PutText( strMaxFPS, iX+2, iY+3, C_YELLOW|192);
	}
}


// max possible drawports
CDrawPort adpDrawPorts[7];
// and ptrs to them
CDrawPort *apdpDrawPorts[7];

INDEX iFirstObserver = 0;

static void MakeSplitDrawports(enum CGame::SplitScreenCfg ssc, INDEX iCount, CDrawPort *pdp)
{
	apdpDrawPorts[0] = NULL;
	apdpDrawPorts[1] = NULL;
	apdpDrawPorts[2] = NULL;
	apdpDrawPorts[3] = NULL;
	apdpDrawPorts[4] = NULL;
	apdpDrawPorts[5] = NULL;
	apdpDrawPorts[6] = NULL;

	// if observer
	if (ssc==CGame::SSC_OBSERVER) 
	{
		// must have at least one screen
		iCount = Clamp(iCount, 1L, 4L);
		// starting at first drawport
		iFirstObserver = 0;
	}

	// if one player or observer with one screen
	if (ssc==CGame::SSC_PLAY1 || ssc==CGame::SSC_OBSERVER && iCount==1) {

		// the only drawport covers entire screen
		adpDrawPorts[0] = CDrawPort( pdp, 0.0, 0.0, 1.0, 1.0);
		apdpDrawPorts[0] = &adpDrawPorts[0];
	// if two players or observer with two screens
	} 
	/*
	else if (ssc==CGame::SSC_PLAY2 || ssc==CGame::SSC_OBSERVER && iCount==2) {




		// if the drawport is not dualhead
		if (!pdp->IsDualHead()) {

			// need two drawports for filling the empty spaces left and right
			CDrawPort dpL( pdp, 0.0, 0.0, 0.2, 1.0f);
			CDrawPort dpR( pdp, 0.8, 0.0, 0.2, 1.0f);
			dpL.SetAsCurrent();  dpL.Fill(C_BLACK|CT_OPAQUE);
			dpR.SetAsCurrent();  dpR.Fill(C_BLACK|CT_OPAQUE);
			// first of two draw ports covers upper half of the screen
			 adpDrawPorts[0] = CDrawPort( pdp, 0.1666, 0.0, 0.666, 0.5);
			apdpDrawPorts[0] = &adpDrawPorts[0];
			// second draw port covers lower half of the screen
			 adpDrawPorts[1] = CDrawPort( pdp, 0.1666, 0.5, 0.666, 0.5);
			apdpDrawPorts[1] = &adpDrawPorts[1];
		// if the drawport is dualhead
		} else {
			// first of two draw ports covers left half of the screen
			 adpDrawPorts[0] = CDrawPort( pdp, 0.0, 0.0, 0.5, 1.0);
			apdpDrawPorts[0] = &adpDrawPorts[0];
			// second draw port covers right half of the screen
			 adpDrawPorts[1] = CDrawPort( pdp, 0.5, 0.0, 0.5, 1.0);
			apdpDrawPorts[1] = &adpDrawPorts[1];
		}
	// if three players or observer with three screens
	} else if (ssc==CGame::SSC_PLAY3 || ssc==CGame::SSC_OBSERVER && iCount==3) {
		// if the drawport is not dualhead
		if (!pdp->IsDualHead()) {
			// need two drawports for filling the empty spaces left and right
			CDrawPort dpL( pdp, 0.0, 0.0, 0.2, 0.5f);
			CDrawPort dpR( pdp, 0.8, 0.0, 0.2, 0.5f);
			dpL.SetAsCurrent();  dpL.Fill(C_BLACK|CT_OPAQUE);
			dpR.SetAsCurrent();  dpR.Fill(C_BLACK|CT_OPAQUE);
			// first of three draw ports covers center upper half of the screen
			 adpDrawPorts[0] = CDrawPort( pdp, 0.16666, 0.0, 0.66666, 0.5);
			apdpDrawPorts[0] = &adpDrawPorts[0];
			// second draw port covers lower-left part of the screen
			 adpDrawPorts[1] = CDrawPort( pdp, 0.0, 0.5, 0.5, 0.5);
			apdpDrawPorts[1] = &adpDrawPorts[1];
			// third draw port covers lower-right part of the screen
			 adpDrawPorts[2] = CDrawPort( pdp, 0.5, 0.5, 0.5, 0.5);
			apdpDrawPorts[2] = &adpDrawPorts[2];
		// if the drawport is dualhead
		} else {
			// first player uses entire left part
			 adpDrawPorts[0] = CDrawPort( pdp, 0.0, 0.0, 0.5, 1.0);
			apdpDrawPorts[0] = &adpDrawPorts[0];
			// make right DH part
			CDrawPort dpDHR( pdp, 0.5, 0.0, 0.5, 1.0);
			// need two drawports for filling the empty spaces left and right on the right DH part
			CDrawPort dpL( &dpDHR, 0.0, 0.0, 0.2, 1.0f);
			CDrawPort dpR( &dpDHR, 0.8, 0.0, 0.2, 1.0f);
			dpL.SetAsCurrent();  dpL.Fill(C_BLACK|CT_OPAQUE);
			dpR.SetAsCurrent();  dpR.Fill(C_BLACK|CT_OPAQUE);
			// second draw port covers upper half of the right screen
			 adpDrawPorts[1] = CDrawPort( &dpDHR, 0.1666, 0.0, 0.6667, 0.5);
			apdpDrawPorts[1] = &adpDrawPorts[1];
			// third draw port covers lower half of the right screen
			 adpDrawPorts[2] = CDrawPort( &dpDHR, 0.1666, 0.5, 0.6667, 0.5);
			apdpDrawPorts[2] = &adpDrawPorts[2];
		}
	// if four players or observer with four screens
	} else if (ssc==CGame::SSC_PLAY4 || ssc==CGame::SSC_OBSERVER && iCount==4) {
		// if the drawport is not dualhead
		if (!pdp->IsDualHead()) {
			// first of four draw ports covers upper-left part of the screen
			 adpDrawPorts[0] = CDrawPort( pdp, 0.0, 0.0, 0.5, 0.5);
			apdpDrawPorts[0] = &adpDrawPorts[0];
			// second draw port covers upper-right part of the screen
			 adpDrawPorts[1] = CDrawPort( pdp, 0.5, 0.0, 0.5, 0.5);
			apdpDrawPorts[1] = &adpDrawPorts[1];
			// third draw port covers lower-left part of the screen
			 adpDrawPorts[2] = CDrawPort( pdp, 0.0, 0.5, 0.5, 0.5);
			apdpDrawPorts[2] = &adpDrawPorts[2];
			// fourth draw port covers lower-right part of the screen
			 adpDrawPorts[3] = CDrawPort( pdp, 0.5, 0.5, 0.5, 0.5);
			apdpDrawPorts[3] = &adpDrawPorts[3];
		// if the drawport is dualhead
		} else {
			// make the DH parts
			CDrawPort dpDHL( pdp, 0.0, 0.0, 0.5, 1.0);
			CDrawPort dpDHR( pdp, 0.5, 0.0, 0.5, 1.0);
			// on the left part
			{
				// need two drawports for filling the empty spaces left and right
				CDrawPort dpL( &dpDHL, 0.0, 0.0, 0.2, 1.0f);
				CDrawPort dpR( &dpDHL, 0.8, 0.0, 0.2, 1.0f);
				dpL.SetAsCurrent();  dpL.Fill(C_BLACK|CT_OPAQUE);
				dpR.SetAsCurrent();  dpR.Fill(C_BLACK|CT_OPAQUE);
				// first of two draw ports covers upper half of the screen
				 adpDrawPorts[0] = CDrawPort( &dpDHL, 0.1666, 0.0, 0.6667, 0.5);
				apdpDrawPorts[0] = &adpDrawPorts[0];
				// second draw port covers lower half of the screen
				 adpDrawPorts[1] = CDrawPort( &dpDHL, 0.1666, 0.5, 0.6667, 0.5);
				apdpDrawPorts[1] = &adpDrawPorts[1];
			}
			// on the right part
			{
				// need two drawports for filling the empty spaces left and right
				CDrawPort dpL( &dpDHR, 0.0, 0.0, 0.2, 1.0f);
				CDrawPort dpR( &dpDHR, 0.8, 0.0, 0.2, 1.0f);
				dpL.SetAsCurrent();  dpL.Fill(C_BLACK|CT_OPAQUE);
				dpR.SetAsCurrent();  dpR.Fill(C_BLACK|CT_OPAQUE);
				// first of two draw ports covers upper half of the screen
				 adpDrawPorts[2] = CDrawPort( &dpDHR, 0.1666, 0.0, 0.6667, 0.5);
				apdpDrawPorts[2] = &adpDrawPorts[2];
				// second draw port covers lower half of the screen
				 adpDrawPorts[3] = CDrawPort( &dpDHR, 0.1666, 0.5, 0.6667, 0.5);
				apdpDrawPorts[3] = &adpDrawPorts[3];
			}
		}
	}
	*/

	// if observer
	if (ssc==CGame::SSC_OBSERVER) {
		// observing starts at first drawport
		iFirstObserver = 0;
	// if not observer
	} else {
		// observing starts after all players
		iFirstObserver = INDEX(ssc)+1;
	}

	// if not observer and using more than one screen
	if (ssc!=CGame::SSC_OBSERVER && iCount>=1) {
		// create extra small screens
		#define FREE (1/16.0)
		#define FULL (4/16.0)
		if (iCount==1) {
			 adpDrawPorts[iFirstObserver+0] = CDrawPort( pdp, 1.0-FREE-FULL, FREE, FULL, FULL);
			apdpDrawPorts[iFirstObserver+0] = &adpDrawPorts[iFirstObserver+0];
		} else if (iCount==2) {
			 adpDrawPorts[iFirstObserver+0] = CDrawPort( pdp, 1.0-FREE-FULL, FREE+0*(FULL+FREE), FULL, FULL);
			apdpDrawPorts[iFirstObserver+0] = &adpDrawPorts[iFirstObserver+0];
			 adpDrawPorts[iFirstObserver+1] = CDrawPort( pdp, 1.0-FREE-FULL, FREE+1*(FULL+FREE), FULL, FULL);
			apdpDrawPorts[iFirstObserver+1] = &adpDrawPorts[iFirstObserver+1];
		} else if (iCount==3) {
			 adpDrawPorts[iFirstObserver+0] = CDrawPort( pdp, 1.0-FREE-FULL, FREE+0*(FULL+FREE), FULL, FULL);
			apdpDrawPorts[iFirstObserver+0] = &adpDrawPorts[iFirstObserver+0];
			 adpDrawPorts[iFirstObserver+1] = CDrawPort( pdp, 1.0-FREE-FULL, FREE+1*(FULL+FREE), FULL, FULL);
			apdpDrawPorts[iFirstObserver+1] = &adpDrawPorts[iFirstObserver+1];
			 adpDrawPorts[iFirstObserver+2] = CDrawPort( pdp, 1.0-FREE-FULL, FREE+2*(FULL+FREE), FULL, FULL);
			apdpDrawPorts[iFirstObserver+2] = &adpDrawPorts[iFirstObserver+2];
		}
	}
}

// this is used to make sure that the thumbnail is never saved with an empty screen
static BOOL _bPlayerViewRendered = FALSE;

//-----------------------------------------------------------------------------
// Purpose: redraw all game views (for case of split-screens and such)
// Input  : *pdpDrawPort - 
//			ulFlags - 렌더링 관련 플래그.
//-----------------------------------------------------------------------------
//강동민 수정 시작 클로즈 준비 작업	08.10
// 인자로 넘겨지는 엔티티가 반드시 Player이어야 함.
void CGame::GameRedrawView( CDrawPort *pdpDrawPort, ULONG ulFlags, CEntity* penViewer)
//강동민 수정 끝 클로즈 준비 작업		08.10
{
	// if thumbnail saving has been required
	if( _fnmThumb!="")
	{ // reset the need for saving thumbnail
		CTFileName fnm = _fnmThumb;
		_fnmThumb = CTString("");
		// render one game view to a small cloned drawport
		PIX pixSizeJ = pdpDrawPort->GetHeight();
		PIXaabbox2D boxThumb( PIX2D(0,0), PIX2D(128,128));
		CDrawPort dpThumb( pdpDrawPort, boxThumb);
		_bPlayerViewRendered = FALSE;
		GameRedrawView( &dpThumb, 0);
		if (_bPlayerViewRendered) 		
		{
			// grab screenshot
			CImageInfo iiThumb;
			CTextureData tdThumb;
			dpThumb.GrabScreen( iiThumb);
			// try to save thumbnail
			try 
			{
				CTFileStream strmThumb;
				tdThumb.Create_t( &iiThumb, 128, MAX_MEX_LOG2, FALSE);
				strmThumb.Create_t(fnm);
				tdThumb.Write_t( &strmThumb);
				strmThumb.Close();
			} 
			catch( char *strError) 
			{
				// report an error to console, if failed
				CPrintF( "%s\n", strError);
			}
		}
		else 
		{
			_fnmThumb = fnm;
		}
	}

	if( ulFlags) {
		// pretouch memory if required (only if in game mode, not screengrabbing or profiling!)
		SE_PretouchIfNeeded();
	}

	// if game is started and computer isn't on
	BOOL bClientJoined = FALSE;
	if( gm_bGameOn && gm_CurrentSplitScreenCfg != SSC_DEDICATED )		// yjpark
	{
//강동민 수정 시작 싱글 던젼 작업	07.29
		//INDEX ctObservers = Clamp(gam_iObserverConfig, 0L, 4L);
		INDEX ctObservers = Clamp(gam_iObserverConfig, 0L, 1L);
//강동민 수정 끝 싱글 던젼 작업		07.29
		INDEX iObserverOffset = ClampDn(gam_iObserverOffset, 0L);
		if (gm_CurrentSplitScreenCfg==SSC_OBSERVER) {
			ctObservers = ClampDn(ctObservers, 1L);
		}
		if (gm_CurrentSplitScreenCfg!=SSC_OBSERVER) 
		{
			if (!gam_bEnableAdvancedObserving || !GetSP()->sp_bCooperative) 
			{
				ctObservers = 0;
			}
		}
		MakeSplitDrawports(gm_CurrentSplitScreenCfg, ctObservers, pdpDrawPort);

		// get number of local players
		// 게임 시작후 플레이어가 설정되었다면, ctLocals = 1이 되겠지?
		INDEX ctLocals = 0;
		{for (INDEX i=0; i<NET_MAXGAMEPLAYERS; i++) 
		{
			if (gm_lpLocalPlayers[i].lp_pplsPlayerSource!=NULL) 
			{
				ctLocals++;
			}
		}}

		// 뷰잉 시점이 되는 엔티티.
		CEntity *apenViewers[7];
		INDEX ctViewers = 0;
		if(penViewer != NULL)
		{
			apenViewers[0] = penViewer;
			ctViewers++;
		}
		else
		{
		apenViewers[0] = NULL;
		}
		apenViewers[1] = NULL;
		apenViewers[2] = NULL;
		apenViewers[3] = NULL;
		apenViewers[4] = NULL;
		apenViewers[5] = NULL;
		apenViewers[6] = NULL;

		// check if input is enabled
		BOOL bDoPrescan = _pInput->IsInputEnabled() &&
			!_pNetwork->IsPaused() && !_pNetwork->GetLocalPause() &&
			_pShell->GetINDEX("inp_bAllowPrescan");
		// prescan input
		if (bDoPrescan) 
		{
			_pInput->GetInput(TRUE);
		}
		// timer must not occur during prescanning
		{ CTSingleLock csTimer(&_pTimer->tm_csHooks, TRUE);
		// for each local player
//강동민 수정 시작 싱글 던젼 작업	07.29
		// 게임 시작후에는 플레이어 엔티티가 1이 되겠지?
		for( INDEX i=0; i<NET_MAXGAMEPLAYERS; i++) 
		{
//강동민 수정 끝 싱글 던젼 작업		07.29
			// if local player
			CPlayerSource *ppls = gm_lpLocalPlayers[i].lp_pplsPlayerSource;
			if( ppls!=NULL) 
			{
				// get local player entity
				apenViewers[ctViewers++] = _pNetwork->GetLocalPlayerEntity(ppls);
				// precreate action for it for this tick
				if (bDoPrescan) 
				{
					// copy its local controls to current controls
					memcpy(
						ctl_pvPlayerControls,
						gm_lpLocalPlayers[i].lp_ubPlayerControlsState,
						ctl_slPlayerControlsSize);

					// do prescanning
					CPlayerAction paPreAction;
					INDEX iCurrentPlayer = gm_lpLocalPlayers[i].lp_iPlayer;
					CControls &ctrls = gm_actrlControls[ iCurrentPlayer];
					ctrls.CreateAction(gm_apcPlayers[iCurrentPlayer], paPreAction, TRUE);

					// copy the local controls back
					memcpy(
						gm_lpLocalPlayers[i].lp_ubPlayerControlsState,
						ctl_pvPlayerControls,
						ctl_slPlayerControlsSize);
				}
			}
		}}

		// fill in all players that are not local
		INDEX ctNonlocals = 0;
//강동민 수정 시작 싱글 던젼 작업	07.29
		CEntity *apenNonlocals[NET_MAXGAMEPLAYERS];
//강동민 수정 끝 싱글 던젼 작업		07.29
		memset(apenNonlocals, 0, sizeof(apenNonlocals));
//강동민 수정 시작 싱글 던젼 작업	07.29
		{for (INDEX i=0; i<NET_MAXGAMEPLAYERS; i++) 
		{
//강동민 수정 끝 싱글 던젼 작업		07.29

			// 플레이어 엔티티의 포인터를 얻어서...
			CEntity *pen = CEntity::GetPlayerEntity(i);
			if (pen!=NULL && !_pNetwork->IsPlayerLocal(pen)) 
			{
				apenNonlocals[ctNonlocals++] = pen;
			}
		}}

		// Local Player가 있을경우...
		if (ctNonlocals>0) 
		{
			// for each observer
			{for (INDEX i=0; i<ctObservers; i++) 
			{
				// get the given player with given offset that is not local
				INDEX iPlayer = (i+iObserverOffset)%ctNonlocals;

				// 뷰어에 Non Local Player를 넣어줌.
				apenViewers[ctViewers++] = apenNonlocals[iPlayer];
			}}
		}

		// for each view
		// 각각의 뷰에 대해서...
		BOOL bHadViewers = FALSE;
		for (INDEX i=0; i<ctViewers; i++) 
		{
			CDrawPort *pdp = apdpDrawPorts[i];
			if (pdp!=NULL) 
			{
				pdp->SetAsCurrent();

				// if there is a viewer
				// 뷰어가 있을때면, 화면을 렌더링 해줌.
				if (apenViewers[i]!=NULL) 
				{
					bHadViewers = TRUE;

					_bPlayerViewRendered = TRUE;
					// render it
					// 결국은 RenderView()를 호출함.
					apenViewers[i]->RenderGameView(pdp, (void*)ulFlags);
				} 
				else 
				{
					pdp->Fill( C_BLACK|CT_OPAQUE);
				}
			}
		}

		if (!bHadViewers) 
		{
			pdpDrawPort->SetAsCurrent();
			pdpDrawPort->Fill( C_BLACK|CT_OPAQUE);
		}

//강동민 수정 시작 클로즈 준비 작업	08.10
		// create drawport for messages (left on DH)
		CDrawPort dpMsg(pdpDrawPort, 0);
		if ((ulFlags&GRV_SHOWEXTRAS)) 
		{
			dpMsg.SetAsCurrent();
			// print pause indicators
			CTString strIndicator;
//강동민 수정 시작 다중 공격 작업	08.22
			/*
			if (_pNetwork->IsDisconnected()) 
			{
				//0611 kwon 수정.
				strIndicator.PrintF(TRANS("Disconnected: %s\n"), (const char *)_pNetwork->WhyDisconnected());
				//strIndicator.PrintF(TRANS("Disconnected: %s\nPress F9 to reconnect"), (const char *)_pNetwork->WhyDisconnected());
			} 
			else if (_pNetwork->IsWaitingForPlayers()) 
			{
				strIndicator = TRANS("Waiting for all players to connect");
			} 
			else if (_pNetwork->IsWaitingForServer()) 
			{
				strIndicator = TRANS("Waiting for server to continue");
			} 
			else if (!_pNetwork->IsConnectionStable()) 
			{
				strIndicator = TRANS("Trying to stabilize connection...");
			} 
			else if (_pNetwork->IsGameFinished()) 
			{
				strIndicator = TRANS("Game finished");
			} 
			else if ((_pNetwork->IsPaused() || _pNetwork->GetLocalPause()) && hud_bShowPauseText) 
			{
				strIndicator = TRANS("Paused");
				hud_bShowPauseText = 1; // clamp
			} 
			*/
//강동민 수정 끝 다중 공격 작업		08.22

			extern INDEX net_bLocalPrediction;      
			if (strIndicator!="") 
			{
				net_bLocalPrediction=FALSE;
			} 
			else 
			{
				net_bLocalPrediction=TRUE;
				/*
				if (_tvMenuQuickSave.tv_llValue!=0I64 && 
					(_pTimer->GetHighPrecisionTimer()-_tvMenuQuickSave).GetSeconds()<3) 
				{
					strIndicator = TRANS("Use F6 for QuickSave during game!");
				} 
				else if (_pNetwork->ga_sesSessionState.ses_strMOTD!="") 
				{
					CTString strMotd = _pNetwork->ga_sesSessionState.ses_strMOTD;
					static CTString strLastMotd = "";
					static CTimerValue tvLastMotd(0I64);
					if (strLastMotd!=strMotd) 
					{
						tvLastMotd = _pTimer->GetHighPrecisionTimer();
						strLastMotd = strMotd;
					}
					if (tvLastMotd.tv_llValue!=0I64 && (_pTimer->GetHighPrecisionTimer()-tvLastMotd).GetSeconds()<3) 
					{
						strIndicator = strMotd;
					}
				}
				*/
			}

			if( strIndicator!="") 
			{
				// setup font
				dpMsg.SetFont( _pfdDisplayFont);
				dpMsg.SetTextScaling( 0.5f * dpMsg.GetHeight()/480);
				dpMsg.SetTextAspect(  1.0f);
				dpMsg.SetTextShadow( +2);
				dpMsg.PutTextCXY( strIndicator, dpMsg.GetWidth()*0.5f, dpMsg.GetHeight()*0.4f, SE_COL_BLUEGREEN_LT|192);
			}
			/*
			// print recording indicator
			if( _pNetwork->IsRecordingDemo()) {
				// setup font
				dpMsg.SetFont( _pfdDisplayFont);
				dpMsg.SetTextScaling( 0.666f);
				dpMsg.SetTextAspect(  1.0f);
				dpMsg.SetTextShadow( +2);
				dpMsg.PutText( TRANS("Recording"), dpMsg.GetWidth()*0.1f, dpMsg.GetHeight()*0.1f, C_CYAN|192);
			}
			*/

			// print some statistics
			PrintStats( &dpMsg);
	
			/*
			// print last few lines from console to top of screen
			// 스크린의 꼭대기에 콘솔창의 마지막 라인을 출력합니다.
			if (_pGame->gm_csConsoleState==CS_OFF) 
				ConsolePrintLastLines( &dpMsg);				

			// print demo OSD
			if( dem_bOSD) 
			{
				CTString strMessage;
				// print the message
				strMessage.PrintF("%.2fs", _pNetwork->ga_fDemoTimer);
				dpMsg.SetFont( _pfdDisplayFont);
				dpMsg.SetTextAspect( 1.0f);
				dpMsg.PutText( strMessage, 20, 20);
			}
			*/
		}		
//강동민/ 수정 끝 클로즈 준비 작업		08.10

//강동민 수정 시작 클로즈 준비 작업	08.10
		/*
		// keep frames' time if required
		if( gm_bProfileDemo)
		{
			CTimerValue tvThisFrame = _pTimer->GetHighPrecisionTimer();
			// if demo has been finished
			if( _pNetwork->IsDemoPlayFinished()) {
				// end profile
				gm_bProfileDemo = FALSE;
				CPrintF( DemoReportAnalyzedProfile());
				CPrintF( "-\n");
			} else {
				// determine frame time delta
				TIME tmDelta = (tvThisFrame - _tvLastFrame).GetSeconds();
				_tvLastFrame =  tvThisFrame;
				_atmFrameTimes.Push() = tmDelta;  // add new frame time stamp
				INDEX *piTriangles = _actTriangles.Push(4); // and polygons count
				piTriangles[0] = _pGfx->gl_ctWorldElements/3;
				piTriangles[1] = _pGfx->gl_ctModelElements/3;
				piTriangles[2] = _pGfx->gl_ctParticleTriangles;
				piTriangles[3] = _pGfx->gl_ctTotalElements/3;
			}
		}
		
		// execute cvar after demoplay
		if( _pNetwork->IsDemoPlayFinished() && dem_strPostExec!="") _pShell->Execute(dem_strPostExec);
		*/
//강동민 수정 끝 클로즈 준비 작업		08.10
	}
	// if no game is active
	else
	{
		// clear background
		pdpDrawPort->SetAsCurrent();
		pdpDrawPort->Fill( SE_COL_BLUE_DARK|CT_OPAQUE);
		pdpDrawPort->FillZBuffer( ZBUF_BACK);
	}

	// check for new chat message
	/*static INDEX ctChatMessages=0;
	INDEX ctNewChatMessages = _pShell->GetINDEX("net_ctChatMessages");
	if (ctNewChatMessages!=ctChatMessages) 
	{
		ctChatMessages=ctNewChatMessages;
		PlayScriptSound(MAX_SCRIPTSOUNDS-1, CTFILENAME("Data\\Sounds\\Menu\\Chat.wav"), 4.0f*gam_fChatSoundVolume, 1.0f, FALSE);
	}*/

	// update sounds and forbid probing
	_pSound->UpdateSounds();
	_pGfx->gl_bAllowProbing = FALSE;

	// 화면 캡쳐시 처리 루틴.
	if( bSaveScreenShot || dem_iAnimFrame>=0)
	{
		// make the screen shot directory if it doesn't already exist
		bSaveScreenShot = FALSE;
		CTFileName fnmExpanded;
		ExpandFilePath(EFP_WRITE, CTString("ScreenShots"), fnmExpanded);
		_mkdir(fnmExpanded);

		// create a name for screenshot
		CTFileName fnmScreenShot;
//강동민 수정 시작 다중 공격 작업	09.06
		//if( dem_iAnimFrame<0) 
		//{
//강동민 수정 끝 다중 공격 작업		09.06
			// fnmScreenShot = MakeScreenShotName();			// 원본.
			fnmScreenShot = fnmExpanded + "\\" + MakeScreenShotName();
//강동민 수정 시작 다중 공격 작업	09.06
			//}
		/*
		else 
		{
			// create number for the file
			CTString strNumber;
			strNumber.PrintF("%05d", (INDEX)dem_iAnimFrame);
			fnmScreenShot = CTString("Shots\\Anim_")+strNumber+".tga";
			dem_iAnimFrame+=1;
		}
		*/
//강동민 수정 끝 다중 공격 작업		09.06
		// grab screen creating image info
		CImageInfo iiImageInfo;
		pdpDrawPort->SetAsCurrent();
		pdpDrawPort->GrabScreen( iiImageInfo, 1);
		// try to
		try 
		{
			// save screen shot as JPG
			if(gam_iJpgScreenShot)			
				iiImageInfo.SaveJPG_t( fnmScreenShot);
			// save screen shot as TGA
			else
				iiImageInfo.SaveTGA_t( fnmScreenShot);

			if( dem_iAnimFrame<0) 
				CPrintF( TRANS("screen shot: %s\n"), (CTString&)fnmScreenShot);
		}
		// if failed
		catch (char *strError) 
		{
			// report error
			CPrintF( TRANS("Cannot save screenshot:\n%s\n"), strError);
		}
	}
}

//0609 kwon
void CGame::GameRedrawCursor(CDrawPort *pdpDrawPort, ULONG ulFlags)
{
	
	CEntity *apenViewers =NULL;

	CPlayerSource *ppls = gm_lpLocalPlayers[0].lp_pplsPlayerSource;
	if( ppls!=NULL) 
	{	
		apenViewers = _pNetwork->GetLocalPlayerEntity(ppls);
	}

	BOOL bHadViewers = FALSE;
	
//안태훈 수정 시작	//(Bug Fix)(0.1)
	CDrawPort *pdp = pdpDrawPort;
	if (pdp!=NULL) {
		pdp->SetAsCurrent();
		
		// if there is a viewer
		if (apenViewers != NULL) {
			bHadViewers = TRUE;			
			_bPlayerViewRendered = TRUE;
			
			extern ENGINE_API cWeb g_web;

			if (!g_web.IsWebHandle())
			{
				// render it
				apenViewers->RenderCursor(pdp, (void*)ulFlags);
			}
		}
	}	
//안태훈 수정 끝	//(Bug Fix)(0.1)
}

/*
void CGame::RecordHighScore(void)
{
	// if game is not running
	if (!gm_bGameOn) {
		// do nothing
		return;
	}
	// find that player
	INDEX ipl= Clamp(int(gam_iRecordHighScore), 0, NET_MAXGAMEPLAYERS);
	CPlayer *penpl = (CPlayer *)&*CEntity::GetPlayerEntity(ipl);
	if (penpl==NULL) {
		//CPrintF( TRANS("Warning: cannot record score for player %d!\n"), ipl);
		return;
	}

	// get its score
	INDEX ctScore = penpl->m_psGameStats.ps_iScore;

	// find entry with lower score
	for(INDEX iLess=0; iLess<HIGHSCORE_COUNT; iLess++) {
		if (gm_ahseHighScores[iLess].hse_ctScore<ctScore) {
			break;
		}
	}
	// if none
	if (iLess>=HIGHSCORE_COUNT) {
		// do nothing more
		return;
	}

	// move all lower entries one place down, dropping the last one
	for(INDEX i=HIGHSCORE_COUNT-1; i>iLess; i--) {
		gm_ahseHighScores[i] = gm_ahseHighScores[i-1];
	}

	// remember new one
	gm_ahseHighScores[iLess].hse_ctScore = ctScore;
	gm_ahseHighScores[iLess].hse_strPlayer = penpl->GetPlayerName();
	gm_ahseHighScores[iLess].hse_gdDifficulty = GetSP()->sp_gdGameDifficulty;
	if (GetSP()->sp_bMental) {
		(INDEX&)gm_ahseHighScores[iLess].hse_gdDifficulty = CSessionProperties::GD_EXTREME+1;
	}
	gm_ahseHighScores[iLess].hse_tmTime = _pTimer->CurrentTick();
	gm_ahseHighScores[iLess].hse_ctKills = penpl->m_psGameStats.ps_iKills;

	// remember best for player hud and statistics
	plr_iHiScore = gm_ahseHighScores[0].hse_ctScore;

	// remember last set
	gm_iLastSetHighScore = iLess;
}
*/

INDEX CGame::GetLivePlayersCount(void)
{
	INDEX ctLive = 0;

	for (INDEX ipl=0; ipl<NET_MAXGAMEPLAYERS; ipl++) {
		CEntity *penpl = CEntity::GetPlayerEntity(ipl);
		if (penpl!=NULL && (penpl->GetFlags()&ENF_ALIVE)) {
			ctLive++;
		}
	}

	return ctLive;
}

INDEX CGame::GetPlayersCount(void)
{
	INDEX ctPlayers = 0;

	for (INDEX ipl=0; ipl<NET_MAXGAMEPLAYERS; ipl++) {
		CEntity *penpl = CEntity::GetPlayerEntity(ipl);
		if (penpl!=NULL) {
			ctPlayers++;
		}
	}

	return ctPlayers;
}

// get default description for a game (for save games/demos)
CTString CGame::GetDefaultGameDescription(BOOL bWithInfo)
{
	CTString strDescription;

	struct tm *newtime;
	time_t long_time;
	time(&long_time);
	newtime = localtime(&long_time);

	setlocale(LC_ALL, "");
	CTString strTimeline;
	char achTimeLine[256]; 
	strftime( achTimeLine, sizeof(achTimeLine)-1, "%a %x %H:%M", newtime);
	strTimeline = achTimeLine;
	setlocale(LC_ALL, "C");

	strDescription.PrintF( "%s - %s", TranslateConst(_pNetwork->ga_World.GetName(), 0), strTimeline);

	if (bWithInfo) {
		CPlayer *penPlayer = (CPlayer *)&*CEntity::GetPlayerEntity(0);
		CTString strStats;
		if (penPlayer!=NULL) {
			penPlayer->GetStats(strStats, CST_SHORT, 40);
		}
		strDescription += "\n"+strStats;
	}

	return strDescription;
}

/*
struct QuickSave 
{
	CListNode qs_lnNode;
	CTFileName qs_fnm;
	INDEX qs_iNumber;
};

int qsort_CompareQuickSaves_FileUp( const void *elem1, const void *elem2)
{
	const QuickSave &qs1 = **(QuickSave **)elem1;
	const QuickSave &qs2 = **(QuickSave **)elem2;
	return strcmp(qs1.qs_fnm, qs2.qs_fnm);
}

// delete extra quicksaves and find the next free number
INDEX FixQuicksaveDir(const CTFileName &fnmDir, INDEX ctMax)
{
	// list the directory
	CDynamicStackArray<CTFileName> afnmDir;
	MakeDirList(afnmDir, fnmDir, "*.sav", 0);

	CListHead lh;
	INDEX iMaxNo = -1;
	
	// for each file in the directory
	for (INDEX i=0; i<afnmDir.Count(); i++) 
	{
		CTFileName fnmName = afnmDir[i];

		// parse it 
		INDEX iFile = -1;
		fnmName.FileName().ScanF("QuickSave%d", &iFile);
		// if it can be parsed
		if (iFile>=0) 
		{
			// create new info for that file
			QuickSave *pqs = new QuickSave;
			pqs->qs_fnm = fnmName;
			pqs->qs_iNumber = iFile;
			if (iFile>iMaxNo) 
			{
				iMaxNo = iFile;
			}
			// add it to list
			lh.AddTail(pqs->qs_lnNode);
		}
	}

	// sort the list
	lh.Sort(qsort_CompareQuickSaves_FileUp, offsetof(QuickSave, qs_lnNode));
	INDEX ctCount = lh.Count();

	// delete all old ones while number of files is too large
	FORDELETELIST(QuickSave, qs_lnNode, lh, itqs) 
	{
		if (ctCount>ctMax) 
		{
			RemoveFile(itqs->qs_fnm);
			RemoveFile(itqs->qs_fnm.NoExt()+"Tbn.tex");
			RemoveFile(itqs->qs_fnm.NoExt()+".des");
			ctCount--;
		}
		delete &*itqs;
	}

	return iMaxNo;
}
*/

//강동민 수정 시작 다중 공격 작업	08.22
void CGame::CaptureScreen()
{	
	SaveScreenShot();
}
//강동민 수정 끝 다중 공격 작업		08.22

/*
CTFileName CGame::GetQuickSaveName(BOOL bSave)
{
	// find out base directory
	CTFileName fnmDir;
	if (GetSP()->sp_ctMaxPlayers==1) 
	{
		INDEX iPlayer = gm_iSinglePlayer;
		if (GetSP()->sp_bQuickTest) 
		{
			iPlayer = gm_iWEDSinglePlayer;
		}
		fnmDir.PrintF("SaveGame\\Player%d\\Quick\\", iPlayer);
	} else {
		if (_pNetwork->IsNetworkEnabled()) {
			fnmDir = CTString("SaveGame\\Network\\Quick\\");
		} else {
			fnmDir = CTString("SaveGame\\SplitScreen\\Quick\\");
		}
	}
	// load last saved number 
	INDEX iLast = FixQuicksaveDir(fnmDir, bSave ? gam_iQuickSaveSlots-1 : gam_iQuickSaveSlots );
	if (bSave) {
		iLast++;
	}

	// add save name to that
	CTFileName fnmName;
	fnmName.PrintF("QuickSave%06d.sav", iLast);
	return fnmDir+fnmName;
}
*/


void CGame::GameMainLoop(void)
{
	// 실질적으로 게임의 루틴.
	if (gm_bGameOn) 
	{
		// do main loop procesing
		_pNetwork->MainLoop();

		ULONG	cur = (ULONG)_pTimer->GetHighPrecisionTimer().GetMilliseconds();
		float	delta = (float)(cur - m_oldTime) * 0.001f;
		ActorMgr::getSingleton()->Update(delta, cur);
		SE_Get_UIManagerPtr()->Update(delta, cur);
		m_oldTime = cur;
	}
}

/*************************************************************
 *         S E C O N D   E N C O U N T E R   M E N U         *
 *************************************************************/

//static CTextureObject _toPointer;
static CTextureObject _toBcgClouds;
//static CTextureObject _toBcgGrid;
static CTextureObject _toBackdrop;
//static CTextureObject _toSamU;
//static CTextureObject _toSamD;
//static CTextureObject _toLeftU;
//static CTextureObject _toLeftD;

static PIXaabbox2D _boxScreen_SE;
static PIX _pixSizeI_SE;
static PIX _pixSizeJ_SE;
CDrawPort *_pdp_SE = NULL;
static FLOAT _tmNow_SE;
static ULONG _ulA_SE;
static BOOL  _bPopup;

void TiledTextureSE( PIXaabbox2D &_boxScreen, FLOAT fStretch, MEX2D &vScreen, MEXaabbox2D &boxTexture)
{
	PIX pixW = _boxScreen.Size()(1);
	PIX pixH = _boxScreen.Size()(2);
	boxTexture = MEXaabbox2D(MEX2D(0, 0), MEX2D(pixW/fStretch, pixH/fStretch));
	boxTexture+=vScreen;
}

////

void CGame::LCDInit(void)
{
	try {
		_toBcgClouds.SetData_t(CTFILENAME("Data\\Textures\\General\\Background6.tex"));
		//_toPointer.SetData_t(CTFILENAME("TexturesMP\\General\\Pointer.tex",));
		//_toBcgGrid.SetData_t(CTFILENAME("TexturesMP\\General\\grid.tex"));
		_toBackdrop.SetData_t(CTFILENAME("Data\\Textures\\General\\MenuBack.tex"));
		//_toSamU.SetData_t(CTFILENAME("TexturesMP\\General\\SamU.tex"));
		//_toSamD.SetData_t(CTFILENAME("TexturesMP\\General\\SamD.tex"));
		//_toLeftU.SetData_t(CTFILENAME("TexturesMP\\General\\LeftU.tex"));
		//_toLeftD.SetData_t(CTFILENAME("TexturesMP\\General\\LeftD.tex"));
		// force constant textures
		((CTextureData*)_toBcgClouds.GetData())->Force(TEX_CONSTANT);
		//((CTextureData*)_toPointer  .GetData())->Force(TEX_CONSTANT);
		//((CTextureData*)_toBcgGrid  .GetData())->Force(TEX_CONSTANT);
		((CTextureData*)_toBackdrop .GetData())->Force(TEX_CONSTANT);
		//((CTextureData*)_toSamU     .GetData())->Force(TEX_CONSTANT);
		//((CTextureData*)_toSamD     .GetData())->Force(TEX_CONSTANT);
		//((CTextureData*)_toLeftU    .GetData())->Force(TEX_CONSTANT);
		//((CTextureData*)_toLeftD    .GetData())->Force(TEX_CONSTANT);

	} catch (char *strError) {
		FatalError("%s\n", strError);
	}
	::LCDInit();
}
void CGame::LCDEnd(void)
{
	_toBackdrop.SetData_t(CTFileName(""));
	_toBcgClouds.SetData_t(CTFileName(""));
	::LCDEnd();
}
void CGame::LCDPrepare(FLOAT fFade)
{
	// get current time and alpha value
	_tmNow_SE = (FLOAT)_pTimer->GetHighPrecisionTimer().GetSeconds();
	_ulA_SE   = NormFloatToByte(fFade);

	::LCDPrepare(fFade);
}


void CGame::LCDSetDrawport(CDrawPort *pdp)
{
	_pdp_SE = pdp;
	_pixSizeI_SE = _pdp_SE->GetWidth();
	_pixSizeJ_SE = _pdp_SE->GetHeight();
	_boxScreen_SE = PIXaabbox2D ( PIX2D(0,0), PIX2D(_pixSizeI_SE, _pixSizeJ_SE));
	_bPopup = (pdp->GetWidth()  != pdp->dp_Raster->ra_Width
					&& pdp->GetHeight() != pdp->dp_Raster->ra_Height);
	::LCDSetDrawport(pdp);
}


void CGame::LCDDrawBox(PIX pixUL, PIX pixDR, PIXaabbox2D &box, COLOR col)
{
	col = SE_COL_BLUE_NEUTRAL|255;

	::LCDDrawBox(pixUL, pixDR, box, col);
}


void CGame::LCDScreenBox(COLOR col)
{
	col = SE_COL_BLUE_NEUTRAL|255;

	::LCDScreenBox(col);
}


void CGame::LCDScreenBoxOpenLeft(COLOR col)
{
	col = SE_COL_BLUE_NEUTRAL|255;

	::LCDScreenBoxOpenLeft(col);
}
void CGame::LCDScreenBoxOpenRight(COLOR col)
{
	col = SE_COL_BLUE_NEUTRAL|255;

	::LCDScreenBoxOpenRight(col);
}
void CGame::LCDRenderClouds1(void)
{
	_pdp_SE->PutTexture(&_toBackdrop, _boxScreen_SE, C_WHITE|255);

	if (!_bPopup) {
//0214 백그라운드에 샘 안나오게 하기.
/*    
		PIXaabbox2D box;
				
		// right character - Sam
		INDEX iSize = 170;
		INDEX iYU = 120;
		INDEX iYM = iYU + iSize;
		INDEX iYB = iYM + iSize;
		INDEX iXL = 420;
		INDEX iXR = iXL + iSize*_pdp_SE->dp_fWideAdjustment;

		box = PIXaabbox2D( PIX2D( iXL*_pdp_SE->GetWidth()/640, iYU*_pdp_SE->GetHeight()/480) ,
											 PIX2D( iXR*_pdp_SE->GetWidth()/640, iYM*_pdp_SE->GetHeight()/480));

		_pdp_SE->PutTexture(&_toSamU, box, SE_COL_BLUE_NEUTRAL|255);
		box = PIXaabbox2D( PIX2D( iXL*_pdp_SE->GetWidth()/640, iYM*_pdp_SE->GetHeight()/480) ,
											 PIX2D( iXR*_pdp_SE->GetWidth()/640, iYB*_pdp_SE->GetHeight()/480));
		_pdp_SE->PutTexture(&_toSamD, box, SE_COL_BLUE_NEUTRAL|255);

		iSize = 120;
		iYU = 0;
		iYM = iYU + iSize;
		iYB = iYM + iSize;
		iXL = -20;
		iXR = iXL + iSize;
		box = PIXaabbox2D( PIX2D( iXL*_pdp_SE->GetWidth()/640, iYU*_pdp_SE->GetWidth()/640) ,
											 PIX2D( iXR*_pdp_SE->GetWidth()/640, iYM*_pdp_SE->GetWidth()/640));
		_pdp_SE->PutTexture(&_toLeftU, box, SE_COL_BLUE_NEUTRAL|200);
		box = PIXaabbox2D( PIX2D( iXL*_pdp_SE->GetWidth()/640, iYM*_pdp_SE->GetWidth()/640) ,
											 PIX2D( iXR*_pdp_SE->GetWidth()/640, iYB*_pdp_SE->GetWidth()/640));
		_pdp_SE->PutTexture(&_toLeftD, box, SE_COL_BLUE_NEUTRAL|200);
		iYU = iYB;
		iYM = iYU + iSize;
		iYB = iYM + iSize;
		iXL = -20;
		iXR = iXL + iSize;
		box = PIXaabbox2D( PIX2D( iXL*_pdp_SE->GetWidth()/640, iYU*_pdp_SE->GetWidth()/640) ,
											 PIX2D( iXR*_pdp_SE->GetWidth()/640, iYM*_pdp_SE->GetWidth()/640));
		_pdp_SE->PutTexture(&_toLeftU, box, SE_COL_BLUE_NEUTRAL|200);
		box = PIXaabbox2D( PIX2D( iXL*_pdp_SE->GetWidth()/640, iYM*_pdp_SE->GetWidth()/640) ,
											 PIX2D( iXR*_pdp_SE->GetWidth()/640, iYB*_pdp_SE->GetWidth()/640));
		_pdp_SE->PutTexture(&_toLeftD, box, SE_COL_BLUE_NEUTRAL|200);
	*/
	}

	MEXaabbox2D boxBcgClouds1;
	TiledTextureSE(_boxScreen_SE, 1.2f*_pdp_SE->GetWidth()/640.0f, 
		MEX2D(sin(_tmNow_SE*0.5f)*35,sin(_tmNow_SE*0.7f+1)*21),   boxBcgClouds1);
	_pdp_SE->PutTexture(&_toBcgClouds, _boxScreen_SE, boxBcgClouds1, C_BLACK|_ulA_SE>>2);
	TiledTextureSE(_boxScreen_SE, 0.7f*_pdp_SE->GetWidth()/640.0f, 
		MEX2D(sin(_tmNow_SE*0.6f+1)*32,sin(_tmNow_SE*0.8f)*25),   boxBcgClouds1);
	_pdp_SE->PutTexture(&_toBcgClouds, _boxScreen_SE, boxBcgClouds1, C_BLACK|_ulA_SE>>2);
}
void CGame::LCDRenderCloudsForComp(void)
{
	MEXaabbox2D boxBcgClouds1;
	TiledTextureSE(_boxScreen_SE, 1.856f*_pdp_SE->GetWidth()/640.0f, 
		MEX2D(sin(_tmNow_SE*0.5f)*35,sin(_tmNow_SE*0.7f)*21),   boxBcgClouds1);
	_pdp_SE->PutTexture(&_toBcgClouds, _boxScreen_SE, boxBcgClouds1, SE_COL_BLUE_NEUTRAL|_ulA_SE>>2);
	TiledTextureSE(_boxScreen_SE, 1.323f*_pdp_SE->GetWidth()/640.0f, 
		MEX2D(sin(_tmNow_SE*0.6f)*31,sin(_tmNow_SE*0.8f)*25),   boxBcgClouds1);
	_pdp_SE->PutTexture(&_toBcgClouds, _boxScreen_SE, boxBcgClouds1, SE_COL_BLUE_NEUTRAL|_ulA_SE>>2);
}
void CGame::LCDRenderClouds2(void)
{
	NOTHING;
}
void CGame::LCDRenderGrid(void)
{
	NOTHING;
}
void CGame::LCDRenderCompGrid(void)
{
	 /*MEXaabbox2D boxBcgGrid;
	 FLOAT fSizeIOverRasterSizeI = FLOAT(_pdp_SE->GetWidth())/_pdp_SE->dp_Raster->ra_Width;
	 TiledTextureSE(_boxScreen_SE, 0.5f*_pdp_SE->GetWidth()/(fSizeIOverRasterSizeI*640.0f), MEX2D(0,0), boxBcgGrid);
	 _pdp_SE->PutTexture(&_toBcgGrid, _boxScreen_SE, boxBcgGrid, SE_COL_BLUE_NEUTRAL|_ulA_SE>>1); */
}
void CGame::LCDDrawPointer(PIX pixI, PIX pixJ)
{
	/*CDisplayMode dmCurrent;
	_pGfx->GetCurrentDisplayMode(dmCurrent);
	if (dmCurrent.IsFullScreen()) {
		while (ShowCursor(FALSE) >= 0);
	} else {
		if (!_pInput->IsInputEnabled()) {
			while (ShowCursor(TRUE) < 0);
		}
		return;
	}
	PIX pixSizeI = _toPointer.GetWidth();
	PIX pixSizeJ = _toPointer.GetHeight();
	pixI-=1;
	pixJ-=1;
	_pdp_SE->PutTexture( &_toPointer, PIXaabbox2D( PIX2D(pixI, pixJ), PIX2D(pixI+pixSizeI, pixJ+pixSizeJ)),
										LCDFadedColor(C_WHITE|255));

	//::LCDDrawPointer(pixI, pixJ);*/
}
COLOR CGame::LCDGetColor(COLOR colDefault, const char *strName)
{
	if (!strcmp(strName, "thumbnail border")) {
		colDefault = SE_COL_BLUE_NEUTRAL|255;
	} else if (!strcmp(strName, "no thumbnail")) {
		colDefault = SE_COL_ORANGE_NEUTRAL|255;
	} else if (!strcmp(strName, "popup box")) {
		colDefault = SE_COL_BLUE_NEUTRAL|255;
	} else if (!strcmp(strName, "tool tip")) {
		colDefault = SE_COL_ORANGE_LIGHT|255;
	} else if (!strcmp(strName, "unselected")) {
		colDefault = SE_COL_ORANGE_NEUTRAL|255;
	} else if (!strcmp(strName, "selected")) {
		colDefault = SE_COL_ORANGE_LIGHT|255;
	} else if (!strcmp(strName, "disabled selected")) {
		colDefault = SE_COL_ORANGE_DARK_LT |255;
	} else if (!strcmp(strName, "disabled unselected")) {
		colDefault = SE_COL_ORANGE_DARK|255;
	} else if (!strcmp(strName, "label")) {
		colDefault = C_WHITE|255;
	} else if (!strcmp(strName, "title")) {
		colDefault = C_WHITE|255;
	} else if (!strcmp(strName, "editing")) {
		colDefault = SE_COL_ORANGE_NEUTRAL|255;
	} else if (!strcmp(strName, "hilited")) {
		colDefault = SE_COL_ORANGE_LIGHT|255;
	} else if (!strcmp(strName, "hilited rectangle")) {
		colDefault = SE_COL_ORANGE_NEUTRAL|255;
	} else if (!strcmp(strName, "edit fill")) {
		colDefault = SE_COL_BLUE_DARK_LT|75;
	} else if (!strcmp(strName, "editing cursor")) {
		colDefault = SE_COL_ORANGE_NEUTRAL|255;
	} else if (!strcmp(strName, "model box")) {
		colDefault = SE_COL_ORANGE_NEUTRAL|255;
	} else if (!strcmp(strName, "hiscore header")) {
		colDefault = SE_COL_ORANGE_LIGHT|255;
	} else if (!strcmp(strName, "hiscore data")) {
		colDefault = SE_COL_ORANGE_NEUTRAL|255;
	} else if (!strcmp(strName, "hiscore last set")) {
		colDefault = SE_COL_ORANGE_NEUTRAL|255;
	} else if (!strcmp(strName, "slider box")) {
		colDefault = SE_COL_ORANGE_NEUTRAL|255;
	} else if (!strcmp(strName, "file info")) {
		colDefault = SE_COL_ORANGE_NEUTRAL|255;
	} else if (!strcmp(strName, "display mode")) {
		colDefault = SE_COL_ORANGE_NEUTRAL|255;
	} else if (!strcmp(strName, "bcg fill")) {
		colDefault = SE_COL_BLUE_DARK|255;
	}
	return ::LCDGetColor(colDefault, strName);
}
COLOR CGame::LCDFadedColor(COLOR col)
{
	return ::LCDFadedColor(col);
}
COLOR CGame::LCDBlinkingColor(COLOR col0, COLOR col1)
{
	return ::LCDBlinkingColor(col0, col1);
}

// menu interface functions
void CGame::MenuPreRenderMenu(const char *strMenuName)
{
}
void CGame::MenuPostRenderMenu(const char *strMenuName)
{
}

void CGame::SetHud_Stats( INDEX stats )
{
	hud_iStats = stats;
}
