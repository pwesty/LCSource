// ----------------------------------------------------------------------------
//  File : GameGuard.h
//  Desc : Orignal Author Sam
// ----------------------------------------------------------------------------

#ifndef GAME_GUARD_H
#define GAME_GUARD_H

#ifndef NO_GAMEGUARD
	#include "NPGameLib.h"

const int CHECK_TERM = 5000;

void CREATE_GAMEGUARD_WRAPPER();
void DESTROY_GAMEGUARD_WRAPPER();

class CGameGuard 
{
public: 
	CGameGuard();
	virtual ~CGameGuard ();
	
	static CGameGuard * Get ();
	
	BOOL		Init ();
	void		Release ();
	// 메인 윈도우 핸들 게임가드에 설정 
	void		SetMainHwnd ( HWND hWnd );								
	// GameMoe에 유저 아이디 보내기 
	void		SendUserId ( char* pszUserID );							
	// 게임 가드 정상 작동중인지 확인 
	void		CheckGameGuard ();										
	// 디바이스 후킹 체크 
	BOOL		CheckDevice ( LPVOID lpD3ddevice, LPSTR lpszDll );         
	
	// 문제 발생시 에러메시지 보이기 
	void		ShowErrMsg ();												
	
	CNPGameLib* GetLibInstance () { return m_pNpgl; }
	// 2~5초 마다 확인하기 위한 시간 설정
	inline void	SetLastCheckTime ( DWORD dwTime );
	
	void		Auth( GG_AUTH_DATA* pData );
	
	//초기화시와 게임실행 후 체크시 문제가 발생 했을 때 호출 	
	void		GameGuardErrorProcess ( DWORD dwMsg , bool bGameUI );
	BOOL		CallBackErrCheck ( DWORD dwMsg, DWORD dwArg );

	static CGameGuard* m_spThis;
private:
	
	CNPGameLib *m_pNpgl;	
	DWORD		m_dwLastCheckTime;
	CTString	m_szGameGuardMsg;
	CTString	m_szErrorCaption;
	
	
	DWORD		m_dwMsg;
	HWND		m_hWnd;
	
	
};

#define GAME_GUARD() CGameGuard::Get()

#endif 
#endif