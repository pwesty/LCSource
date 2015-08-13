#include "StdH.h"
#include <Engine/Base/CTString.h>
#include <Engine/Base/ProgressHook.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/CommunicationInterface.h>

// profiling variables
static FLOAT _fLastTime = 0.0f;
static FLOAT _fFirstTime = 0.0f;
static INDEX _ctLastFromDisk = 0;
static INDEX _ctLastEffective = 0;
static INDEX _ctFirstFromDisk = 0;
static INDEX _ctFirstEffective  = 0;

extern INDEX _ctTotalFromDisk;
extern INDEX _ctTotalEffective;
extern INDEX fil_iReportStats;

// progress variables
static CTCriticalSection _csHookInfo;    // just for acces to the info - info must be copied!
static void (*_pLoadingHook_t)(CProgressHookInfo *pgli) = NULL;  // hook for loading/connecting
static CProgressHookInfo _phiLoadingInfo; // info passed to the hook
static FLOAT _fUpdateInterval = 0.0f;
static FLOAT _fLastRatio = 0.0f;

// variables use for controling the progress rendering thread
static DWORD _dwThread = 0;
static HANDLE _hThread = 0;
static HANDLE _hEventAsyncEnabled = 0;
static HANDLE _hEventAsyncNotRunning = 0;
static CTCriticalSection _csAsyncThread;
static BOOL _bRunThread = FALSE;  // set while the thread should run
static INDEX _ctAsyncEnabled = 0;
static INDEX _iOldSwapInterval = 0;
extern INDEX gap_iSwapInterval;


//----------------------------------------------
// part used for loading performance profiling


// format a bytes count into bytes, kilobytes or megabytes, to be readable
static CTString FormatBytes(INDEX ctBytes)
{
  if (ctBytes<1024) {
    return CTString(0, "%dB", ctBytes);
  } else if (ctBytes<1024*1024) {
    return CTString(0, "%.1fkB", ctBytes/1024.0);
  } else {
    return CTString(0, "%.1fMB", ctBytes/(1024.0*1024.0));
  }
}

// print loading performance data
static void PrintLoadingPerformance(const char *strLabel, INDEX ctFromDisk, INDEX ctEffective, FLOAT fTime)
{
  if (!fil_iReportStats) {
    return;
  }
  CPrintF("%20.20s: %5.2fs %10s(%10s) %10.1fkB/s(%10.1fkB/s)\n", 
    strLabel, fTime,
    (const char*)FormatBytes(ctEffective), 
    (const char*)FormatBytes(ctFromDisk ),
    (ctEffective>>10)/fTime,
    (ctFromDisk >>10)/fTime
  );
}


static void MeasurePerformance(const CTString &strDescription)
{
  CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
  FLOAT fTimeNow = tvNow.GetSeconds();

  // if just started loading
  if(_fLastTime==0.0f) {
    // report boot-up time
    PrintLoadingPerformance("BOOTUP", _ctTotalFromDisk, _ctTotalEffective, fTimeNow);
    _fFirstTime = fTimeNow;
    _ctFirstEffective = _ctTotalEffective;
    _ctFirstFromDisk  = _ctTotalFromDisk ;

  // if inside loading
  } else {
    // report performance of previous pass
    PrintLoadingPerformance(_phiLoadingInfo.phi_strDescription, 
      _ctTotalFromDisk  - _ctLastFromDisk, 
      _ctTotalEffective - _ctLastEffective, 
      fTimeNow - _fLastTime);
  }
  _fLastTime = fTimeNow;
  _ctLastEffective = _ctTotalEffective;
  _ctLastFromDisk  = _ctTotalFromDisk ;

  // if this is the last pass
  if(strDescription=="starting server") {
    // report total performance
    PrintLoadingPerformance("TOTAL", 
      _ctTotalFromDisk  - _ctFirstFromDisk, 
      _ctTotalEffective - _ctFirstEffective, 
      fTimeNow - _fFirstTime);
    _fLastTime = 0.0f;
  }
}

//----------------------------------------------
// part used for network updates


static CTimerValue _tvLastNetUpdate;
static BOOL  _bNetTimeInitialized = FALSE;
extern FLOAT net_fSendRetryWait;
//! 로딩하는 동안 네트워크를 업데이트 한다.
static void DoNetworkUpdates(void)
{
  // for network updating during loading
  if (_pNetwork!=NULL) {
    if (!_bNetTimeInitialized) {
      _tvLastNetUpdate = _pTimer->GetHighPrecisionTimer();
      _bNetTimeInitialized = TRUE;
    }  
    {CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
    if ((tvNow-_tvLastNetUpdate) > CTimerValue(net_fSendRetryWait*1.1)) {
      if (_pNetwork->ga_IsServer) {
        // handle server messages
        _cmiComm.Server_Update();
      } else {
        // handle client messages
        _cmiComm.Client_Update();
      }
      _tvLastNetUpdate = _pTimer->GetHighPrecisionTimer();
    }}
  }
}
  
//----------------------------------------------
// actual progress work
  
// this calls the hook
void DoCall(enum ProgressHookStatus phs)
{
  if (_pLoadingHook_t!=NULL) {
    BOOL bGfxReady = FALSE;
    bGfxReady = TRUE;

    if (phs!=PHS_WORKING || bGfxReady) {
      CProgressHookInfo phi;
      {CTSingleLock sl(&_csHookInfo, TRUE);
       phi = _phiLoadingInfo;}
      phi.phi_phsStatus = phs;
      _pLoadingHook_t(&phi);
    }
  }
}

// the thread loop
static DWORD WINAPI ProgressThread(LPVOID lpParameter)
{
	// deleted by seo
  /*INDEX iHz = _pGfx->GetRefreshHz();
  if (iHz==0) {
    iHz = 60;
  }

  while(_bRunThread) {
    WaitForSingleObject(_hEventAsyncEnabled, INFINITE);
    
    if (!_bRunThread) {
      break;
    }

    CTimerValue tvBefore;
    {CTSingleLock sl(&_csAsyncThread, TRUE);
      ResetEvent(_hEventAsyncNotRunning);
      tvBefore = _pTimer->GetHighPrecisionTimer();
      DoCall(PHS_WORKING);
      SetEvent(_hEventAsyncNotRunning);
    }

    CTimerValue tvAfter = _pTimer->GetHighPrecisionTimer();
    FLOAT fDelta = (tvAfter-tvBefore).GetSeconds();
    INDEX iSleep = ceil((1.0f/iHz-fDelta)*1000);
    if (iSleep>0) {
      Sleep(iSleep);
    }
  }

  FreeVPrintF();*/
  return 0;
}  


// set hook for loading/connecting
void SetProgressHook(void (*pHook)(CProgressHookInfo *pgli))
{
  _pLoadingHook_t = pHook;
}

// call loading/connecting hook
void SetProgressDescription(const CTString &strDescription)
{
  TRACKMEM(Mem, "Progress hook");
  MeasurePerformance(strDescription);

  {_csHookInfo.cs_iIndex = -1;
   CTSingleLock sl(&_csHookInfo, TRUE);
  _phiLoadingInfo.phi_strDescription = strDescription;}

  DoCall(PHS_NEWDESCRIPTION);
}

void SetProgressFile(const CTFileName &strNewFile)
{
  TRACKMEM(Mem, "Progress hook");
  {_csHookInfo.cs_iIndex = -1;
   CTSingleLock sl(&_csHookInfo, TRUE);
  _phiLoadingInfo.phi_fnmFileName = strNewFile;}

  DoCall(PHS_NEWFILE);
}


void CallProgressHook_t(FLOAT fCompleted)
{
  TRACKMEM(Mem, "Progress hook");
  {_csHookInfo.cs_iIndex = -1;
   CTSingleLock sl(&_csHookInfo, TRUE);
  _phiLoadingInfo.phi_fCompleted = fCompleted;}

  DoCall(PHS_WORKING);
//0524 kwon 임시 삭제.
//  DoNetworkUpdates();
}

// enable/disable asynchronious progress display (uses a second thread)
void EnableAsyncProgress(void)
{
  // only on X-Box
  _ctAsyncEnabled++;
}


void DisableAsyncProgress(void)
{
  // only on X-Box
  _ctAsyncEnabled--;
}


// start/stop progress display
void StartProgress(void)
{
  TRACKMEM(Mem, "Progress hook");

  _csAsyncThread.cs_iIndex = -1;

  {_csHookInfo.cs_iIndex = -1;
   CTSingleLock sl(&_csHookInfo, TRUE);
  _phiLoadingInfo.phi_phsStatus = PHS_INIT;
  _phiLoadingInfo.phi_fCompleted = 0;
  _phiLoadingInfo.phi_strDescription = "";
  _phiLoadingInfo.phi_fnmFileName = CTString("");}
  DoCall(PHS_INIT);

  ASSERT(_hThread == NULL);

}


void StopProgress(void)
{
  TRACKMEM(Mem, "Progress hook");

  DoCall(PHS_END);
}

// object used to lock out the progress thread
CDisableAsyncProgress::CDisableAsyncProgress(void)
{
  _csAsyncThread.Lock();
}
CDisableAsyncProgress::~CDisableAsyncProgress(void)
{
  _csAsyncThread.Unlock();
}

