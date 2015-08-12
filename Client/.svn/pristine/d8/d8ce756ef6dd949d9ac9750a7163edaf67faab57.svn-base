#include "StdH.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIInternalClasses.h>
#include "LoginNew.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Templates/StaticArray.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/GameState.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Network/TcpIpConnection.h>
#include <Engine/Sound/SoundLibrary.h>		// 서정원 로긴 관련 사운드 처리
#include <Engine/Base/md5.h>
#include <Engine/Contents/Login/UILoginNew.h>
#include <Engine/Help/DefineHelp.h>

// External variables
extern INDEX g_iSaveID;
extern int g_iLocalVersion;
extern INDEX	g_iCountry;


LoginNew::LoginNew(void)
	:	m_bSave(FALSE)
	,	m_strUserID(CTString(""))
	,	m_strUserPW(CTString(""))
{
	m_bSave = g_iSaveID;
	SetVersion();
}

LoginNew::~LoginNew(void)
{
}

void LoginNew::SetSaveID( BOOL bSave )
{
	m_bSave = bSave;
	g_iSaveID = bSave;
}

void LoginNew::SetVersion()
{
	CTString strFullPath = _fnmApplicationPath.FileDir();
//	strFullPath += "Local\\" + DefHelp::getNationPostfix(g_iCountry, true) + "\\";
	CTString fnTemp		= strFullPath + "vtm.brn";

	//int LocalVersion;
	int encr_version;
	FILE *fp;
	fp = fopen(fnTemp, "rb");
	if (fp) {
		fread(&encr_version, sizeof(int), 1, fp);
		g_iLocalVersion = (encr_version - 27) / 3;		

		fclose(fp);
	} else {
		g_iLocalVersion = 700;
	}

	CUIManager::getSingleton()->SetVersion(g_iLocalVersion);
}

BOOL LoginNew::ConnectToLoginServer()
{
	if(_pNetwork->m_bSendMessage)
		return FALSE;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// 문제가 발생했습니다.
	if( _tcpip.Socket == INVALID_SOCKET || !_tcpip.IsConnected(_tcpip.Socket) )
	{
		CPrintF("로그인 서버와 연결할수 없습니다.\n");

		pUIManager->CloseMessageBox(MSGCMD_CONNECT_ERROR);
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 424, "접속 오류" ), UMBS_OK, UI_LOGIN, MSGCMD_CONNECT_ERROR );
		MsgBoxInfo.AddString( _S( 425, "로그인 서버와 연결할 수 없습니다." ) );
		pUIManager->CreateMessageBox( MsgBoxInfo );

		_pNetwork->m_bSendMessage = FALSE;
		return FALSE;
	}
	// 소켓의 연결을 끊었다가, 다시 연결함.	
	//-------------------------------------------------
	pUIManager->GetLogin()->Lock(TRUE);

	_pNetwork->SendLoginMessage(m_strUserID, m_strUserPW, pUIManager->GetVersion());		

	_pNetwork->m_strUserID = m_strUserID;
	_pNetwork->m_strUserPW = m_strUserPW;


#ifndef NO_GAMEGUARD
	if ( g_pGameGuardSendUserID )
	{
		g_pGameGuardSendUserID( m_strUserID.str_String );
	}
#endif

	return TRUE;
}