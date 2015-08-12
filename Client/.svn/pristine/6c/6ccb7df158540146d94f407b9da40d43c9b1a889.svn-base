/*
 *  Serious Sam
 *  Copyright (c) 1998, CroTeam. All rights reserved.
 *
 */

#ifndef __NKSP_H__
#define __NKSP_H__

#define APPLICATION_NAME "Nksp"
#include "CDCheck.h"

//extern HINSTANCE _hInstance;		// yjpark
extern BOOL _bRunning, _bQuitScreen;

extern ENGINE_API INDEX sam_bFullScreenActive;
extern ENGINE_API INDEX sam_iScreenSizeI;
extern ENGINE_API INDEX sam_iScreenSizeJ;
extern ENGINE_API INDEX sam_iDisplayDepth;
extern ENGINE_API INDEX sam_iDisplayAdapter;
extern ENGINE_API INDEX sam_iGfxAPI;
extern INDEX sam_iVideoSetup;  // 0==speed, 1==normal, 2==quality, 3==custom
extern BOOL  sam_bAutoAdjustAudio;

extern void StartNewMode( enum GfxAPIType eAPI, INDEX iAdapter, PIX pixSizeI, PIX pixSizeJ,
                          enum DisplayDepth eColorDepth, BOOL bFullScreenMode);
struct KeyData {
  char *kd_strASCII;
};

extern CGame *_pGame;
extern HWND g_parenthWnd;

//	김영환 : 클라이언트 전용 엔진 설정 변수
extern ENGINE_API BOOL _bClientApp;			// TRUE 설정되면 게임 클라이언트 이다.

//	김영환 : 화면 변경 
BOOL TryToSetDisplayMode( enum GfxAPIType eGfxAPI, INDEX iAdapter, PIX pixSizeI, PIX pixSizeJ,
													enum DisplayDepth eColorDepth, BOOL bFullScreenMode,bool bCenter = true);
//	김영환 : 윈도우 크기 변경
bool	Set_Window_Size();


//2013/04/15 jeil 런처에 해상도 변경 추가

void GetWindowSizeFromTxt();
void SaveWindowSizeToTxt();

#ifdef KALYDO
#include <deque>
void KalydoUserData();
void KalydoSetUserAccount( std::string& sID, std::string& sPWD );
bool KalydoInit( HWND Parent );
void KalydoStart();
bool KalydoUpdate();
void KalydoEnd();
HWND getGameWindow();
extern std::deque<MSG> deqMsg;
#endif

#endif