/*
 *  Serious Sam
 *  Copyright (c) 1998, CroTeam. All rights reserved.
 *
 */
//extern BOOL _bWindowChanging;    // ignores window messages while this is set		// yjpark
//extern HWND _hwndMain;
extern BOOL _bIMEProc; // 이기환 수정 11.12

// init/end main window management
void MainWindow_Init(void);
void MainWindow_End(void);
// close the main application window
//	김영환 : 종료 함수 변경
void CloseMainWindow(bool p_Exit = false);

// open the main application window for windowed mode
void OpenMainWindowNormal(PIX pixSizeI, PIX pixSizeJ,bool p_Center = true);

//	김영환 : 기본이 중앙 정렬임, false 이면 중앙정렬 안함.
void ResetMainWindowNormal(bool p_Center = true);
// open the main application window for fullscreen mode
void OpenMainWindowFullScreen(PIX pixSizeI, PIX pixSizeJ);
// open the main application window invisible
void OpenMainWindowInvisible(void);
