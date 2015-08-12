#ifndef SE_INCL_PROGRESSHOOK_H
#define SE_INCL_PROGRESSHOOK_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif
#include <Engine/Base/FileName.h>

// structure describing current state during loading, passed to loading hook
enum ProgressHookStatus {
  PHS_INIT,           // called on StartProgress in main thread
  PHS_WORKING,        // called in regular intervals during processing in 2nd thread
  PHS_NEWDESCRIPTION, // called when description is changed in main thread
  PHS_NEWFILE,        // called for each new file opened in main thread
  PHS_END,            // called on StopProgress in main thread
};
class CProgressHookInfo {
public:
  CTString phi_strDescription;    // world/savegame/session that is loading/connecting, etc.
  FLOAT phi_fCompleted;           // completed ratio [0..1]
  CTFileName phi_fnmFileName;     // current filename loading
  enum ProgressHookStatus phi_phsStatus;
};

//
// this part of interface is towards the application (progress display)
//

// set hook for loading/connecting
extern ENGINE_API void SetProgressHook(void (*pHook_t)(CProgressHookInfo *pgli));

//
// this part of interface is towards the loading system (progress updates)
//

// call loading/connecting hook
extern ENGINE_API void SetProgressDescription(const CTString &strDescription);
extern ENGINE_API void SetProgressFile(const CTFileName &strNewFile);
extern ENGINE_API void CallProgressHook_t(FLOAT fCompleted);

// start/stop progress display
extern ENGINE_API void StartProgress(void);
extern ENGINE_API void StopProgress(void);

// object used to lock out the progress thread
class ENGINE_API CDisableAsyncProgress {
  int iDummy;
public:
  CDisableAsyncProgress(void);
  ~CDisableAsyncProgress(void);
};

#endif  /* include-once check. */

