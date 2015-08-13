
#include "stdafx.h"
#include "LCDDrawing.h"
#include <locale.h>
#include <Engine/Loading.h>
#define USECUSTOMTEXT 0

extern CGame *_pGame;

#if USECUSTOMTEXT
  static CTString _strCustomText = "";
#endif
static CDrawPort *_pdpLoadingHook = NULL;  // drawport for loading hook
extern BOOL _bUserBreakEnabled;


#define REFRESHTIME (0.2f)

void RemapLevelNames(INDEX &iLevel)
{
  switch( iLevel) {
  case 10:  iLevel =  1;  break;
  case 11:  iLevel =  2;  break;
  case 12:  iLevel =  3;  break;
  case 13:  iLevel =  4;  break;
  case 14:  iLevel =  5;  break;
  case 15:  iLevel =  6;  break;
  case 21:  iLevel =  7;  break;
  case 22:  iLevel =  8;  break;
  case 23:  iLevel =  9;  break;
  case 24:  iLevel = 10;  break;
  case 31:  iLevel = 11;  break;
  case 32:  iLevel = 12;  break;
  case 33:  iLevel = 13;  break;
  default:  iLevel = -1;	break;
  }
}


static void LoadingHook_t(CProgressHookInfo *pphi)
{
  if (pphi->phi_phsStatus==PHS_NEWFILE) {
    return;
  }
  // if user presses escape
  ULONG ulCheckFlags = 0x8000;
  if (pphi->phi_fCompleted>0) {
    ulCheckFlags |= 0x0001;
  }
  if (_bUserBreakEnabled && (GetAsyncKeyState(VK_ESCAPE)&ulCheckFlags)) {
    // break loading
    throw TRANS("User break!");
  }

#if USECUSTOMTEXT
  // if no custom loading text
  if (_strCustomText=="") {
    // load it
    try {
      _strCustomText.Load_t(CTFILENAME("Data\\LoadingText.txt"));
    } catch (char *strError) {
      _strCustomText = strError;
    }
  }
#endif

  // measure time since last call
  static CTimerValue tvLast(0I64);
  CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();

  // if not first or final update, and not enough time passed
  if (pphi->phi_fCompleted!=0 && pphi->phi_fCompleted!=1 &&
     (tvNow-tvLast).GetSeconds() < REFRESHTIME) {
    // do nothing
    return;
  }
  tvLast = tvNow;

  // skip if cannot lock drawport
  CDrawPort *pdp = _pdpLoadingHook;                           
  ASSERT(pdp!=NULL);
  CDrawPort dpHook(pdp, 0);
  dpHook.SetAsCurrent();

  // clear screen
  dpHook.Fill(C_BLACK|255);

  // get session properties currently loading
  CSessionProperties *psp = (CSessionProperties *)_pNetwork->GetSessionProperties();
  ULONG ulLevelMask = psp->sp_ulLevelsMask;
  INDEX iLevel = -1;
  if (psp->sp_bCooperative) {
    INDEX iLevel = -1;
    INDEX iLevelNext = -1;
    CTString strLevelName = _pNetwork->ga_fnmWorld.FileName();
    CTString strNextLevelName = _pNetwork->ga_fnmNextLevel.FileName();
    
    INDEX u, v;
    u = v = -1;
    strLevelName.ScanF("%01d_%01d_", &u, &v);
    iLevel = u*10+v;
    RemapLevelNames(iLevel);
    u = v = -1;
    strNextLevelName.ScanF("%01d_%01d_", &u, &v);
    iLevelNext = u*10+v;
    RemapLevelNames(iLevelNext);
    
    //strLevelName.ScanF("%02d_", &iLevel);
    //strNextLevelName.ScanF("%02d_", &iLevelNext);
   
    if (iLevel>0) {
      ulLevelMask|=1<<(iLevel-1);
    }
    if (iLevelNext>0) {
      ulLevelMask|=1<<(iLevelNext-1);
    }
  }

  /*if (ulLevelMask!=0 && !_pNetwork->IsPlayingDemo()) {
    // map hook
    extern void RenderMap( CDrawPort *pdp, ULONG ulLevelMask, CProgressHookInfo *pphi);
    RenderMap(&dpHook, ulLevelMask, pphi);

    // finish rendering
    dpHook.dp_Raster->ra_pvpViewPort->SwapBuffers();

    // keep current time
    tvLast = _pTimer->GetHighPrecisionTimer();
    return;
  }*/

  // map hook
  // by seo
//     extern void RenderLoading( CDrawPort *pdp, ULONG ulLevelMask, CProgressHookInfo *pphi);
//     RenderLoading(&dpHook, ulLevelMask, pphi);

    // finish rendering
    dpHook.dp_Raster->ra_pvpViewPort->SwapBuffers();

    // keep current time
    tvLast = _pTimer->GetHighPrecisionTimer();
    return;
  // end seo

  // get sizes
  PIX pixSizeI = dpHook.GetWidth();
  PIX pixSizeJ = dpHook.GetHeight();
  CFontData *pfd = _pfdConsoleFont;
  PIX pixCharSizeI = pfd->fd_pixCharWidth  + pfd->fd_pixCharSpacing;
  PIX pixCharSizeJ = pfd->fd_pixCharHeight + pfd->fd_pixLineSpacing;

  PIX pixBarSizeJ = 17;//*pixSizeJ/480;

  COLOR colBcg = LerpColor(C_BLACK, SE_COL_BLUE_LIGHT, 0.30f)|0xff;
  COLOR colBar = LerpColor(C_BLACK, SE_COL_BLUE_LIGHT, 0.45f)|0xff;
  COLOR colLines = colBar; //C_vdGREEN|0xff;
  COLOR colText = LerpColor(C_BLACK, SE_COL_BLUE_LIGHT, 0.95f)|0xff;
  COLOR colEsc = C_WHITE|0xFF;

  dpHook.Fill(0, pixSizeJ-pixBarSizeJ, pixSizeI, pixBarSizeJ, colBcg);
  dpHook.Fill(0, pixSizeJ-pixBarSizeJ, pixSizeI*pphi->phi_fCompleted, pixBarSizeJ, colBar);
  dpHook.DrawBorder(0, pixSizeJ-pixBarSizeJ, pixSizeI, pixBarSizeJ, colLines);

  dpHook.SetFont( _pfdConsoleFont);
  dpHook.SetTextScaling( 1.0f);
  dpHook.SetTextAspect( 1.0f);
  // print status text
  setlocale(LC_ALL, "");
  CTString strDesc(0, "%s", pphi->phi_strDescription);  strupr((char*)(const char*)strDesc);
  setlocale(LC_ALL, "C");
  CTString strPerc(0, "%3.0f%%", pphi->phi_fCompleted*100);
  //dpHook.PutText(strDesc, pixCharSizeI/2, pixSizeJ-pixBarSizeJ-2-pixCharSizeJ, C_GREEN|255);
  //dpHook.PutTextCXY(strPerc, pixSizeI/2, pixSizeJ-pixBarSizeJ/2+1, C_GREEN|255);
  dpHook.PutText(strDesc, pixCharSizeI/2, pixSizeJ-pixBarSizeJ+pixCharSizeJ/2, colText);
  dpHook.PutTextR(strPerc, pixSizeI-pixCharSizeI/2, pixSizeJ-pixBarSizeJ+pixCharSizeJ/2, colText);
  if (_bUserBreakEnabled && !_pGame->gm_bFirstLoading) {
    dpHook.PutTextC( TRANS( "PRESS ESC TO ABORT"), pixSizeI/2, pixSizeJ-pixBarSizeJ-2-pixCharSizeJ, colEsc);
  }

  // finish rendering
  dpHook.dp_Raster->ra_pvpViewPort->SwapBuffers();

  // keep current time
  tvLast = _pTimer->GetHighPrecisionTimer();
}

// loading hook functions
void CGame::EnableLoadingHook(CDrawPort *pdpDrawport)
{
  _pdpLoadingHook = pdpDrawport;
  SetProgressHook(LoadingHook_t);
}

void CGame::DisableLoadingHook(void)
{
  SetProgressHook(NULL);
  _pdpLoadingHook = NULL;
}
