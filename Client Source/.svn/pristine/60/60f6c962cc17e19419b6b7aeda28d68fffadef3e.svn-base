// ----------------------------------------------------------------------------
//  File : GameGuard.cpp
//  Desc : Orignal Author Sam
// ----------------------------------------------------------------------------

#include "StdH.h"
#include "GameGuard.h"
#include "Engine//GameState.h"
#include "Engine/Network/SessionState.h"
#include "Engine/Network/MessageDefine.h"
#include "Engine/Interface/UIManager.h"

#ifndef NO_GAMEGUARD
	#include "Engine/GameGuardInterface.h"
	#pragma comment(lib, "NPGameLib.lib" )


ENGINE_API extern CGameState *_pGameState;
ENGINE_API extern CNetworkLibrary *_pNetwork;


CGameGuard* CGameGuard::m_spThis = NULL;

CGameGuard* g_pGameGuard = NULL;	

void GameGuard_Auth_Recv (GG_AUTH_DATA AuthData)
{
	g_pGameGuard->Auth (&AuthData);
}

void GameGuard_Send_ID (char* pszUserID)
{
	g_pGameGuard->SendUserId( pszUserID );
}

void CREATE_GAMEGUARD_WRAPPER()
{
	g_pGameGuard			= new CGameGuard();   
	
	g_pGameGuardRecvAuth	= GameGuard_Auth_Recv;
	g_pGameGuardSendUserID  = GameGuard_Send_ID;
}

void DESTROY_GAMEGUARD_WRAPPER()
{
	if (g_pGameGuard)
    {
        delete g_pGameGuard;
        g_pGameGuard = NULL;
    }
}


CGameGuard::CGameGuard()
{
	m_spThis			= this;
	m_pNpgl				= NULL;	
	m_dwLastCheckTime	= 0;	
	m_dwMsg				= 0;
	m_szGameGuardMsg.Clear();
}

CGameGuard::~CGameGuard ()
{
	m_spThis = NULL;
	Release();
}

CGameGuard* CGameGuard::Get()
{
    ASSERT(m_spThis);
    return m_spThis;
}

BOOL CGameGuard::Init ()
{
	m_pNpgl = new CNPGameLib("ShadowTW");

	if ( !m_pNpgl )
	{
		m_szGameGuardMsg = _S(3326,"게임가드 실행에 실패했습니다. 게임가드 셋업 파일을 다시 설치해 보시기 바랍니다.");
		::MessageBox(NULL, m_szGameGuardMsg , "Error", MB_OK );
		return FALSE;
	}

	DWORD dwErrCode = m_pNpgl->Init();

	if ( NPGAMEMON_SUCCESS != dwErrCode )
	{		
		GameGuardErrorProcess( dwErrCode, false );
		//Release();
		return FALSE;
	}
	SetLastCheckTime( GetTickCount() );
	
	return TRUE;
}

void CGameGuard::Release ()
{
	if ( m_pNpgl )
	{
		delete m_pNpgl;
		m_pNpgl = NULL;
	}
	//if ( m_spThis )
	//	m_spThis = NULL;
}

inline void	CGameGuard::SetLastCheckTime ( DWORD dwTime )
{
	m_dwLastCheckTime = dwTime;
}
	
void CGameGuard::SetMainHwnd ( HWND hWnd )
{
	m_hWnd = hWnd;
	if ( m_pNpgl )
		m_pNpgl->SetHwnd( hWnd );
}

void CGameGuard::SendUserId ( char* pszUserID )
{
	if ( m_pNpgl )
	{	
		if ( m_pNpgl->Send( pszUserID ) )
		{
			//보내기 성공
		}
		else
		{
			//보내기 실패 시 처리가 필요한가?
		}
	}
}

void  CGameGuard::CheckGameGuard ()
{
	if ( m_pNpgl && ( GetTickCount() - m_dwLastCheckTime ) >= CHECK_TERM )
	{
		DWORD dwResult  = m_pNpgl->Check();
		m_dwLastCheckTime = GetTickCount();
			
	
		if( dwResult  != NPGAMEMON_SUCCESS )
		{
			GameGuardErrorProcess( dwResult, true );
			_pGameState->Running()		= FALSE;
			_pGameState->QuitScreen()	= FALSE;
		}
	}
	
}

BOOL CGameGuard::CheckDevice ( LPVOID lpD3ddevice, LPSTR lpszDll )
{
	return TRUE;
}

void CGameGuard::ShowErrMsg ()
{			
	if( m_szGameGuardMsg.Length() > 0)
	{
		if ( 0 == m_dwMsg)
			m_szErrorCaption.PrintF(_S(3342,"게임가드 에러"));
		else
			m_szErrorCaption.PrintF(_S(3342,"게임가드 에러 : %lu"),m_dwMsg);
		::MessageBox(NULL, m_szGameGuardMsg.str_String,m_szErrorCaption.str_String,MB_OK);
	}
}


void CGameGuard::Auth( GG_AUTH_DATA* pData )
{
	if ( m_pNpgl )
		m_pNpgl->Auth2( pData );
}

void CGameGuard::GameGuardErrorProcess ( DWORD dwMsg, bool bGameUI )
{
	m_dwMsg = dwMsg;	
	switch (dwMsg)
	{		
	//Error Code
	case NPGAMEMON_ERROR_EXIST:
		m_szGameGuardMsg = _S(3321,"게임가드가 실행 중입니다. 잠시 후나 재부팅 후에 다시 실행해 보시기 바랍니다.");
		break;
		
		/*
	case NPGAMEMON_ERROR_CREATE :
		m_szGameGuardMsg =  "GameGuard Directory Create Error";
		break;
		/**/

	case NPGAMEMON_ERROR_NPSCAN:
		m_szGameGuardMsg = _S(3335,"바이러스 및 해킹툴 검사 모듈 로딩에 실패 했습니다. 메모리 부족이거나 바이러스에 의한 감염일 수 있습니다.");
		break;

		/*
	case NPGAMEMON_ERROR_THREAD :
		strErrorMessage =  "CreateThread Error";
		break;
		/**/
		
	case NPGAMEMON_ERROR_INIT:
		m_szGameGuardMsg = _S(3323,"게임가드 초기화 에러입니다. 재부팅 후 다시 실행해 보거나 충돌할 수 있는 다른 프로그램들을 종료한 후 실행해 보시기 바랍니다.");
		break;

	case NPGAMEMON_ERROR_GAME_EXIST:
		m_szGameGuardMsg = _S(3322,"게임이 중복 실행되었거나 게임가드가 이미 실행 중 입니다. 게임 종료 후 다시 실행해 보시기 바랍니다,");
		break;
		
	/*	
	case NPGAMEMON_ERROR_AUTH_NPGMUP:
		strErrorMessage =  "npgmup.des Authentication Fail";
		break;
		/**/
		
	case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
	case NPGAMEMON_ERROR_AUTH_INI:
	//case NPGAMEMON_ERROR_AUTH_DLL:
	//case NPGAMEMON_ERROR_DECRYPT:
	case NPGAMEMON_ERROR_NFOUND_GG:	
	case NPGAMEMON_ERROR_NFOUND_INI:
		m_szGameGuardMsg = _S(3324,"게임가드 파일이 없거나 변조되었습니다. 게임가드 셋업 파일을 설치해보시기 바랍니다.");
		break;

	case NPGAMEMON_ERROR_CRYPTOAPI:
		m_szGameGuardMsg = _S(3325,"윈도우의 일부 시스템 파일이 손상되었습니다. 인터넷 익스플로러(IE)를 다시 설치해 보시기 바랍니다.");
		break;

	case NPGAMEMON_ERROR_EXECUTE:
		m_szGameGuardMsg = _S(3326,"게임가드 실행에 실패했습니다. 게임가드 셋업 파일을 다시 설치해 보시기 바랍니다.");
		break;

	case NPGAMEMON_ERROR_ILLEGAL_PRG:
		m_szGameGuardMsg = _S(3327,"불법 프로그램이 발견되었습니다. 불필요한 프로그램을 종료한 후 다시 실행해 보시기 바랍니다.");
		break;

	case NPGMUP_ERROR_ABORT:
		m_szGameGuardMsg = _S(3328,"게임가드 업데이트를 취소하셨습니다. 접속이 계속되지 않을 경우 인터넷 및 개인 방화벽 설정을 조정해 보시기 바랍니다.");
		break;

	case NPGMUP_ERROR_CONNECT:
		m_szGameGuardMsg = _S(3329,"바이러스나 스파이웨어로 인해 후킹이 실패하였습니다. 최신백신을 받으신 후 컴퓨터 전체검사를 해 주십시오");
		break;


		//현재 버전에서는 없어진듯 
 	//case HOOK_TIMEOUT:
	//	m_szGameGuardMsg = _S(3329,"바이러스나 스파이웨어로 인해 후킹이 실패하였습니다. 최신백신을 받으신 후 컴퓨터 전체검사를 해 주십시오");
 	//	break;

	case NPGAMEMON_ERROR_GAMEGUARD:
		m_szGameGuardMsg = _S(3330,"게임가드 초기화 에러 또는 구버젼의 게임가드 파일입니다. 게임가드 셋업 파일을 다시 설치하고 게임을 실행해 주십시오");
		break;

			//Callback Message
		/*
	case NPGAMEMON_UNDEFINED:
		{
			break;
		}
		/**/
	case NPGAMEMON_COMM_ERROR:
		{
			//GameMon과의 통신 채널이 끊어졌습니다. 보통 GameMon이 비
			//정상적으로 종료되었을 경우이므로 게임도 종료해줍니다.
			break;
		}
		
	case NPGAMEMON_COMM_CLOSE:
		{
			// GameMon정상 종료 게임도 종료시키도 좋음 
			break;
		}
		
		
		//추가 에러 메시지 
	case NPGMUP_ERROR_PARAM:
		m_szGameGuardMsg = _S(3331,"ini 파일이 없거나 변조되었습니다. 게임가드 셋업 파일을 설치하면 해결할 수 있습니다.");
		break;

	case NPGMUP_ERROR_INIT:
		m_szGameGuardMsg = _S(3332,"npgmup.des 초기화 에러입니다. 게임가드 폴더를 삭제후 다시 게임 실행해 주십시오.");
		break;

	case NPGMUP_ERROR_DOWNCFG:
		m_szGameGuardMsg = _S(3333,"게임가드 업데이트 서버 접속에 실패하였습니다. 잠시 후 재시도 해보거나, 개인 방화벽이 있다면 설정을 조정해 보시기 바랍니다.");
		break;

	case NPGMUP_ERROR_AUTH:
		m_szGameGuardMsg = _S(3334,"게임가드 업데이트를 완료하지 못 했습니다. 바이러스 백신을 일시 중지시킨 후 재시도 해보시거나, PC관리 프로그램을 사용하시면 설정을 조정해 보시기 바랍니다.");
		break;

	case NPGG_ERROR_COLLISION:
//		strErrorMessage =  "게임가드와 충돌 프로그램이 발견되었습니다.";
		break;
		
	default:		
		m_szGameGuardMsg = _S(3336,"게임가드 실행 중 에러가 발생하였습니다. 게임 폴더 안의 GameGuard폴더에 있는 *erl 파일을 버그레포팅시 첨부하여 주시기 바랍니다.");
		break;
	}	
	
}

 CGameGuard::CallBackErrCheck ( DWORD dwMsg, DWORD dwArg )
{
	BOOL bQuitApplication = FALSE;

	switch( dwMsg )
	{
	case NPGAMEMON_SUCCESS :
		break;

	case NPGAMEMON_COMM_ERROR:
		bQuitApplication = TRUE;
		break;
	case NPGAMEMON_COMM_CLOSE:
		bQuitApplication = TRUE;
		break;
	case NPGAMEMON_INIT_ERROR:
		{
			// GameMon 실행 에러입니다. 게임 종료 처리를 시작하고 종료 직전
			//에 에러코드인 dwArg 값과 함께 적절한 메시지를 출력해 줍니다.
			m_szGameGuardMsg.PrintF(_S(3337,"게임가드 초기화 에러 : %lu"), dwArg);			
			bQuitApplication = TRUE;
			break;
		}
	case NPGAMEMON_SPEEDHACK:
		{
			//스피드핵이 감지되었습니다. 게임 종료 처리를 시작하고 종료 직전
			//에 적절한 메시지를 출력해줍니다
			m_szGameGuardMsg .PrintF(_S(3338,"스피드핵이 감지되었습니다."));
			bQuitApplication = TRUE;
			break;
		}
	case NPGAMEMON_GAMEHACK_KILLED :
		// 게임핵이 실행되었지만 성공적으로 강제 종료시켰습니다.
		// 게임을 계속 진행해도 무방하지만 종료하기를 권장합니다.
		m_szGameGuardMsg.PrintF(_S(3339,"게임핵이 발견되었습니다.\r\n%s"),m_pNpgl->GetInfo());
		bQuitApplication = TRUE;
		break;

	case NPGAMEMON_GAMEHACK_DETECT :
		{
			//게임핵이 발견되었습니다. 강제 종료가 적합하지 않은 경
			//우이므로 게임 종료 처리를 시작하고 종료 직전에 적절한 메시지 출력해줍니다.
			m_szGameGuardMsg.PrintF(_S(3340,"게임핵이 발견되었습니다.\r\n%s"),m_pNpgl->GetInfo());
			if ( m_pNpgl->GetInfo() )
			{

			}				
			else
			{
				; // 불법 유저 로그 남기기? 서버로 보내는 처리 필요 
			}
			bQuitApplication = TRUE;
		}		
		break;	
		
	case NPGAMEMON_GAMEHACK_DOUBT :
		{
			//게임핵으로 의심되는 프로그램이 실행 중 입니다. 혹은 게
			//임이나 게임가드가 변조되었습니다. 게임 종료 처리를 시작하고 종료 직전에 불필요한 프로그램을
			//종료하고 다시 게임을 해보라는 메시지를 출력해줍니다.
			m_szGameGuardMsg.PrintF(_S(3341,"게임이나 게임가드가 변조되었습니다.\r\n%s"),m_pNpgl->GetInfo());
			bQuitApplication = TRUE;
			break;
		}

	case NPGAMEMON_GAMEHACK_REPORT:
		{
			DWORD dwHackInfoSize	= 0;
			LPBYTE pHackInfo		= NULL;
			pHackInfo = m_pNpgl->GetHackInfo(&dwHackInfoSize);
			
			if (pHackInfo && dwHackInfoSize > 0)
			{
				//게임핵이 발견되었지만 정상 동작합니다. 게임클라이언트
				//는 전달받은 데이터를 서버에 전송하고 정상동작 합니다.
				// 아래 함수는 게임가드에서 제공하는 함수가 아닙니다.
				//SendToHackLog(pHackInfo, dwHackInfoSize); // 서버로 데이터 전송.
			}			
			break;
		}


	case NPGAMEMON_CHECK_CSAUTH2 :
		{
			GG_AUTH_DATA m_AuthData;
			memcpy( &m_AuthData, (PVOID)dwArg, sizeof( GG_AUTH_DATA ) );			
			CNetworkMessage nmItem(MSG_EXTEND);
			nmItem << (ULONG)MSG_EX_NPROTECT;
			nmItem << m_AuthData.dwIndex;
			nmItem << m_AuthData.dwValue1;
			nmItem << m_AuthData.dwValue2;
			nmItem << m_AuthData.dwValue3;			
			_pNetwork->SendToServerNew(nmItem);
		}
		break;		
	}
	

	return bQuitApplication;
}
BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
	BOOL tAppExit =FALSE;
	
	tAppExit = GAME_GUARD()->CallBackErrCheck ( dwMsg, dwArg );
	
	if(tAppExit)
	{
		// 게임 종료
		_pGameState->Running()		= FALSE;
		_pGameState->QuitScreen()	= FALSE;
		return FALSE;
	}
	return TRUE;
}



#endif