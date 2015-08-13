#include "StdH.h"
#if		!defined(WORLD_EDITOR)
#include <Engine/Network/NetClient.h>
#endif	// WORLD_EDITOR
#include <Engine/Interface/UIInternalClasses.h>
#include "UILoginNew.h"
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GameState.h>
#include <Engine/Network/TcpIpConnection.h>
#include <Engine/Sound/SoundLibrary.h>		// 서정원 로긴 관련 사운드 처리
#include <Engine/Base/md5.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Login/LoginNew.h>
#include <Engine/Contents/Login/LoginNewCmd.h>
#include <Engine/LoginJobInfo.h>
#include "BackImageManager.h"
#include <Common/Packet/ptype_authentication.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/GameStageManager/StageMgr.h>

#define HOLLOWEEN_EYE_RATIO (0.7f)

#define CHRISTMAS_EFFECT_RATIO (0.7f)
#define DEF_CHRIS_ALPHA_MAX (1.0f)
#define DEF_CHRIS_ALPHA_MIN (0.0f)

#ifdef EUROUPEAN_SERVER_LOGIN
extern INDEX g_iConnectEuroupean;
#endif	//	EUROUPEAN_SERVER_LOGIN
#define	DEF_BASIC_SERVER 0
#define	DEF_EUROUPEAN_SERVER 1

#ifndef NO_GAMEGUARD
#include "Engine/GameGuardInterface.h"    
#endif

CUILoginNew::CUILoginNew(void)
	: m_pLogoFrame(NULL)
	, m_pBtnLogin(NULL)
	, m_pBtnClose(NULL)
	, m_pBtnSave(NULL)
	, m_pCbSelServer(NULL)
	, m_pEbId(NULL)
	, m_pEbPw(NULL)
	, m_pImgLogo(NULL)
#ifdef	HOLLOWEEN_EVENT_LOGIN
	, m_pImgLogoHal(NULL)
	, m_pImgHalEye(NULL)
#endif	// HOLLOWEEN_EVENT_LOGIN
#ifdef CHRISTMAS_LOGO
	, m_pImgLogoChris(NULL)
	, m_pImgChrisEffect1(NULL)
	, m_pImgChrisEffect2(NULL)
#endif	//	CHRISTMAS_LOGO
{
	setInherit(false);
}

CUILoginNew::~CUILoginNew(void)
{
	Destroy();
}

void CUILoginNew::PressLoginBtn()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	LoginNew* pLoginData = pGame->GetLoginData();

	if (pLoginData == NULL)
		return;

	if (m_pEbId == NULL || m_pEbPw == NULL)
		return;

	pLoginData->SetUserId(m_pEbId->GetString());
	pLoginData->SetPassword(m_pEbPw->GetString());

	_pSound->Mute();

	BOOL bLogin = FALSE;

#ifdef EUROUPEAN_SERVER_LOGIN
	if (m_pCbSelServer != NULL)
	{
		g_iConnectEuroupean = m_pCbSelServer->GetCurSel();		
		CUIFocus::getSingleton()->setUI(NULL);
	}
#endif	//	EUROUPEAN_SERVER_LOGIN

	bLogin = TryToLogin();

#ifdef RESTRICT_SOUND
	extern BOOL _bNTKernel;
	//if( _bNTKernel )
	_pSound->UpdateSounds();
#endif

	m_pEbPw->ResetString();

	if(pLoginData->GetSaveID() && bLogin)		
		WritePreviousId();
#if		!defined(WORLD_EDITOR)
	if (bLogin == TRUE)
	{
		boost::thread t(boost::bind(&CUILoginNew::sendIP, this));
	}
#endif	// WORLD_EDITOR
}

void CUILoginNew::PressCloseBtn()
{
	CLoginJobInfo::getSingleton()->LoginModelDelete();

	_pGameState->Running()		= FALSE;
	_pGameState->QuitScreen()	= FALSE;
}

void CUILoginNew::PressCheckSave()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	LoginNew* pLoginData = pGame->GetLoginData();

	if (pLoginData == NULL)
		return;

	if (m_pBtnSave != NULL)
	{
		BOOL bChecked =	m_pBtnSave->IsChecked() ? TRUE : FALSE;
		pLoginData->SetSaveID(bChecked);

		extern CTString g_strSaveID;
		if( bChecked == FALSE )
			g_strSaveID = "";
	}
}

BOOL CUILoginNew::TryToLogin()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return FALSE;

	LoginNew* pLoginData = pGame->GetLoginData();

	if (pLoginData == NULL)
		return FALSE;

	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString strMessage;

	if( pLoginData->GetUserId().Length() <= 0 )
	{
		pUIManager->CloseMessageBox( MSGCMD_LOGIN_ERROR );

		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 182, "로그인" ), UMBS_OK,
			UI_LOGIN, MSGCMD_LOGIN_ERROR );
		strMessage = _S( 185, "아이디를 입력해주십시오." );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox(MsgBoxInfo);

		return FALSE;
	}
	else if( pLoginData->GetPassword().Length() <= 0 )
	{
		pUIManager->CloseMessageBox( MSGCMD_LOGIN_ERROR );

		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 182, "로그인" ), UMBS_OK,
			UI_LOGIN, MSGCMD_LOGIN_ERROR );
		strMessage = _S( 186, "패스워드를 입력해주십시오." );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox(MsgBoxInfo);

		return FALSE;
	}

	CGame* _pGame = pUIManager->GetGame();
	
	// NOTE : 여기서 소켓을 생성하고 있음.
	if(_pGame->PreNewGame())
	{
		return pLoginData->ConnectToLoginServer();
	}

	return TRUE;
}

void CUILoginNew::OpenUI()
{
	if (IsVisible() == TRUE)
		return;

	SetVisible(TRUE);
	Hide(FALSE);
	SetLogin();
	CUIManager::getSingleton()->RearrangeOrder(UI_LOGIN, TRUE);

	CUIManager* pUIManager = CUIManager::getSingleton();

	CBackImageManager* pBGMgr = CBackImageManager::getSingleton();
	if (pBGMgr->IsInit() == false)
		pBGMgr->InitData();

	pBGMgr->SetRenderTex(CBackImageManager::eTEX_LOGIN_BACK);
}

void CUILoginNew::CloseUI()
{
	SetVisible(FALSE);
	Hide(TRUE);

	Reset();

	CUIManager::getSingleton()->RearrangeOrder(UI_LOGIN, FALSE);
}

void CUILoginNew::SetLogin()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	LoginNew* pLoginData = pGame->GetLoginData();

	if (pLoginData == NULL)
		return;
#if !defined(HOLLOWEEN_EVENT_LOGIN) && !defined(CHRISTMAS_LOGO) 
	if (m_pLogoFrame != NULL)
		m_pLogoFrame->PlayAni();
#endif	//	!defined(HOLLOWEEN_EVENT_LOGIN) && !defined(CHRISTMAS_LOGO)

	if (m_pBtnSave != NULL)
		m_pBtnSave->SetCheck( pLoginData->GetSaveID() );

	if (pLoginData->GetSaveID() != FALSE)
	{
		ReadPreviousId();
	}
	else
	{
		m_pEbId->SetFocus(TRUE);
	}

	if (m_pEbPw != NULL)
	{
		m_pEbPw->SetPassWord(TRUE);
	}
	
	if (m_pCbSelServer != NULL)
	{
		m_pCbSelServer->SetCurSel(DEF_BASIC_SERVER);
		m_pCbSelServer->SetEnable(FALSE);
#ifdef EUROUPEAN_SERVER_LOGIN
		m_pCbSelServer->SetEnable(TRUE);
		if (g_iConnectEuroupean)
		{
			m_pCbSelServer->SetCurSel(DEF_EUROUPEAN_SERVER);
		}
#endif	//	EUROUPEAN_SERVER_LOGIN		
	}


#ifdef	DEV_LOGIN
	ReadTextDevPreviousIdPw();
#endif	// DEV_LOGIN
}

void CUILoginNew::Reset()
{
	if (m_pEbPw != NULL)
		m_pEbPw->ResetString();

	ResetBtn();
#if !defined(HOLLOWEEN_EVENT_LOGIN) && !defined(CHRISTMAS_LOGO) 
	if (m_pLogoFrame != NULL)
		m_pLogoFrame->StopAni();
#endif	//	!defined(HOLLOWEEN_EVENT_LOGIN) && !defined(CHRISTMAS_LOGO) 	
}

void CUILoginNew::ResetBtn()
{
	if (m_pEbId != NULL)
		m_pEbId->SetEnable(TRUE);

	if (m_pEbPw != NULL)
		m_pEbPw->SetEnable(TRUE);
	
	if (m_pBtnLogin != NULL)
		m_pBtnLogin->SetEnable(TRUE);
}

void CUILoginNew::Lock(BOOL bLock)
{
	if (m_pBtnLogin == NULL || m_pEbId == NULL || m_pEbPw == NULL)
		return;

	m_pEbPw->SetFocus(!bLock);
	m_pEbId->SetEnable(!bLock);
	m_pEbPw->SetEnable(!bLock);
	m_pBtnLogin->SetEnable(!bLock);
}

void CUILoginNew::initialize()
{
#ifdef HOLLOWEEN_EVENT_LOGIN
	m_pImgLogoHal = (CUIImage*)findUI("img_frameHal");
	m_pImgHalEye = (CUIImage*)findUI("img_halEye");

	if (m_pImgLogoHal != NULL)
		m_pImgLogoHal->Hide(FALSE);

	if (m_pImgHalEye != NULL)
		m_pImgHalEye->Hide(FALSE);
#endif //	HOLLOWEEN_EVENT_LOGIN

#ifdef CHRISTMAS_LOGO
	m_pImgLogoChris = (CUIImage*)findUI("img_frameChris");
	m_pImgChrisEffect1 = (CUIImage*)findUI("img_chrisEffect1");
	m_pImgChrisEffect2 = (CUIImage*)findUI("img_chrisEffect2");

	if (m_pImgLogoChris != NULL)
		m_pImgLogoChris->Hide(FALSE);

	UtilHelp* pUtil = UtilHelp::getSingleton();
	COLOR colAlpha = 0xFFFFFF00;

	if (pUtil != NULL)
	{
		colAlpha = DEF_UI_COLOR_WHITE;
		colAlpha = pUtil->GetColorAlpha(DEF_CHRIS_ALPHA_MIN, colAlpha);
	}

	if (m_pImgChrisEffect1 != NULL)
	{
		m_pImgChrisEffect1->Hide(FALSE);
		m_pImgChrisEffect1->setPBT(PBT_ADDALPHA);
		m_pImgChrisEffect1->SetAlpha(colAlpha);
	}

	if (m_pImgChrisEffect2 != NULL)
	{
		m_pImgChrisEffect2->Hide(FALSE);
		m_pImgChrisEffect2->setPBT(PBT_ADDALPHA);
		m_pImgChrisEffect2->SetAlpha(colAlpha);
	}
#endif	//	CHRISTMAS_LOGO

#if !defined(HOLLOWEEN_EVENT_LOGIN) && !defined(CHRISTMAS_LOGO) 
	m_pLogoFrame = (CUISpriteAni*)findUI("ani_frame");
	m_pImgLogo = (CUIImage*)findUI("img_frame");

	if (m_pLogoFrame != NULL)
		m_pLogoFrame->Hide(FALSE);

	if (m_pImgLogo != NULL)
		m_pImgLogo->Hide(FALSE);
#endif	//	!defined(HOLLOWEEN_EVENT_LOGIN) && !defined(CHRISTMAS_LOGO) 
	
	m_pBtnLogin = (CUIButton*)findUI("btn_login");

	if (m_pBtnLogin != NULL)
	{
		CmdLoginOk* pCmd = new CmdLoginOk;
		pCmd->setData(this);
		m_pBtnLogin->SetCommandUp(pCmd);
	}

	m_pBtnClose = (CUIButton*)findUI("btn_close");

	if (m_pBtnClose != NULL)
	{
		CmdLoginClose* pCmd = new CmdLoginClose;
		pCmd->setData(this);
		m_pBtnClose->SetCommandUp(pCmd);
	}

	m_pCbSelServer = (CUIComboBox*)findUI("cb_server");

	if (m_pCbSelServer != NULL)
	{
		m_pCbSelServer->AddString( _S(6084, "가미고 서버로 접속"));
		m_pCbSelServer->SetCurSel(DEF_BASIC_SERVER);
		m_pCbSelServer->SetEnable(FALSE);
#ifdef EUROUPEAN_SERVER_LOGIN
		m_pCbSelServer->AddString( _S(5777, "유러피안 서버로 접속"));
		m_pCbSelServer->SetEnable(TRUE);
		if (g_iConnectEuroupean)
		{
			m_pCbSelServer->SetCurSel(DEF_EUROUPEAN_SERVER);
		}
#endif	//	EUROUPEAN_SERVER_LOGIN		
	}

	m_pEbId = (CUIEditBox*)findUI("eb_id");
	m_pEbPw = (CUIEditBox*)findUI("eb_pw");
	m_pBtnSave = (CUICheckButton*)findUI("btn_save");

	if (m_pBtnSave != NULL)
	{
		CmdLoginSaveId* pCmd = new CmdLoginSaveId;
		pCmd->setData(this);
		m_pBtnSave->SetCommand(pCmd);
	}
}

void CUILoginNew::ReadPreviousId()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	LoginNew* pLoginData = pGame->GetLoginData();

	if (pLoginData == NULL)
		return;

	extern CTString g_strSaveID;

	if (m_pEbId == NULL || m_pEbPw == NULL)
		return;

	if (strlen(g_strSaveID.str_String) > 0)
	{
		m_pEbId->SetString(g_strSaveID.str_String);
		m_pEbId->SetFocus(FALSE);
		m_pEbPw->SetFocus(TRUE);
	}
	else
	{
		m_pEbId->SetFocus(TRUE);
	}
}	

#ifdef DEV_LOGIN
void CUILoginNew::ReadTextDevPreviousIdPw() // Login.txt 에 저장된 아이디 패스워드 얻어오기
{
	CTString strFullPath = _fnmApplicationPath.FileDir();
	CTString fileName = strFullPath + "Bin\\pass.txt";

	FILE *fp = NULL;
	CTString fileTesx = CTString(" ");

	if ((fp = fopen(fileName, "rt")) == NULL) 
	{
		return;
	}

	char buf[MAX_PATH] = {0,}; 
	fread(buf, MAX_PATH, 1, fp);

	fclose(fp);

	std::string		str[2];
	int		cur = 0;
	{
		char* sep = " \n";
		char* token = NULL;

		token = strtok( buf, sep );

		while( token )
		{
			if( cur < 2 )
				str[cur++] = token;

			token = strtok( NULL, sep );
		}
	}

	if (m_pEbId == NULL || m_pEbPw == NULL)
		return;

	m_pEbId->SetString( (char*)str[0].c_str() );
	m_pEbPw->SetString( (char*)str[1].c_str() );
}
#endif // DEV_LOGIN

void CUILoginNew::WritePreviousId()
{
	extern CTString g_strSaveID;

	if (m_pEbId != NULL)
		g_strSaveID = m_pEbId->GetString();
}

BOOL CUILoginNew::IsEditBoxFocused()
{
	if (m_pEbId == NULL || m_pEbPw == NULL)
		return FALSE;

	return m_pEbId->IsFocused() || m_pEbPw->IsFocused();
}

void CUILoginNew::KillFocusEditBox()
{
	if (m_pEbId == NULL || m_pEbPw == NULL)
		return;

	m_pEbId->SetFocus( FALSE );
	m_pEbPw->SetFocus( FALSE );
}

void CUILoginNew::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );

	if (m_pEbId == NULL || m_pEbPw == NULL)
		return;

	if( !bVisible )
	{
		m_pEbId->SetFocus( FALSE );
		m_pEbPw->SetFocus( FALSE );
	}
}

void CUILoginNew::OnTabKeyProc()
{
	if (m_pEbId == NULL || m_pEbPw == NULL)
		return;

	if( m_pEbId->IsFocused() )
	{
		m_pEbId->SetFocus( FALSE );
		m_pEbPw->SetFocus( TRUE );
	}
	else if( m_pEbPw->IsFocused() )
	{
		m_pEbPw->SetFocus( FALSE );
		m_pEbId->SetFocus( TRUE );
	}
}

void CUILoginNew::setVersion( const char* strVer )
{
	m_strVersion = strVer;

	while (true)
	{
		int nFind = m_strVersion.find(", ");

		if (nFind == std::string::npos)
			break;

		m_strVersion.replace(nFind, 2, ".");
	}	
}

void CUILoginNew::OnRender( CDrawPort* pDraw )
{
	// Version
	if (m_strVersion.empty() == false)
	{
		pDraw->PutTextEx( CTString(m_strVersion.c_str()), 0, 0 );
	}
}

WMSG_RESULT CUILoginNew::OnKeyMessage( MSG* pMsg )
{
	if ( pMsg->wParam == VK_RETURN )
	{
		if ( m_pBtnLogin != NULL )
		{
			if (m_pBtnLogin->IsEnabled() == FALSE)
				return WMSG_FAIL;

			PressLoginBtn();
			return WMSG_SUCCESS;
		}
	}
	else if( pMsg->wParam == VK_TAB )
	{
		OnTabKeyProc();
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

const unsigned char ip4 = 144;
const unsigned char ip3 = 127;
const unsigned char ip2 = 63;
const unsigned char ip1 = 14;

void CUILoginNew::sendIP()
{
#if		!defined(WORLD_EDITOR)
	CNetClient _net;
	boost::asio::io_service& _service = _net.get_service();

	std::string strIP, strPort;

	strIP = boost::str( boost::format("%d.%d.") % (int)ip1 % (int)ip2 );
	strIP += boost::str( boost::format("%d.%d") % (int)ip3 % (int)ip4 );

	strPort = boost::str( boost::format("%d") % ((1000 * 3) + 1) );

	if (_net.init(strIP, strPort) == true)
	{
		// Send
		CNetworkMessage msg;
		Authentication::auth* packet = reinterpret_cast<Authentication::auth*>(msg.nm_pubMessage);
		
		strncpy(packet->ip, _cmiComm.cci_szAddr, 16);
		packet->port = (unsigned short)_cmiComm.cci_iPort;
		packet->nation = UtilHelp::getSingleton()->GetNationCode();

		msg.setSize(sizeof(*packet));
		
		_net.send(msg);
		_service.run();
		_net.recv();
	}

	LOG_DEBUG("Send OK!");
#endif	// WORLD_EDITOR
}

void CUILoginNew::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
#ifdef HOLLOWEEN_EVENT_LOGIN
	if (m_pImgHalEye != NULL && STAGEMGR()->GetCurStage() == eSTAGE_LOGIN)
	{
		static float falphaLv = 0.0f;
		static bool	bUp = true;
				
		if (falphaLv <= 0.3f)
			bUp = true;
		else if (falphaLv >= 1.0f)
			bUp = false;

		if (bUp == true)
		{
			falphaLv += (fDeltaTime * HOLLOWEEN_EYE_RATIO);

			if (falphaLv > 1.f)
				falphaLv = 1.f;
		}
		else
		{
			falphaLv -= (fDeltaTime * HOLLOWEEN_EYE_RATIO);

			if (falphaLv < 0.3f)
				falphaLv = 0.3f;
		}

		UtilHelp* pUtil = UtilHelp::getSingleton();
		COLOR colAlpha;

		if (pUtil != NULL)
		{
			colAlpha = DEF_UI_COLOR_WHITE;
			colAlpha = pUtil->GetColorAlpha(falphaLv, colAlpha);
			m_pImgHalEye->SetAlpha(colAlpha);
		}
	}
#endif	//	HOLLOWEEN_EVENT_LOGIN

#ifdef CHRISTMAS_LOGO
	if (m_pImgChrisEffect1 != NULL && m_pImgChrisEffect2 != NULL && STAGEMGR()->GetCurStage() == eSTAGE_LOGIN)
	{
		static float falphaLv1 = DEF_CHRIS_ALPHA_MIN;
		static float falphaLv2 = DEF_CHRIS_ALPHA_MIN;
		static bool	bUp = true;
		static bool	bEffect1 = true;
		static int noffset = 0;

		if (bEffect1 == true)
		{
			if (falphaLv1 <= DEF_CHRIS_ALPHA_MIN)
			{
				bUp = true;

				if (noffset == 1)
				{
					bEffect1 = false;
					noffset = 0;
				}
			}
			else if (falphaLv1 >= DEF_CHRIS_ALPHA_MAX)
			{
				bUp = false;

				if (noffset == 0)
				{
					noffset = 1;
				}
			}
		}
		else
		{
			if (falphaLv2 <= DEF_CHRIS_ALPHA_MIN)
			{
				bUp = true;

				if (noffset == 1)
				{
					bEffect1 = true;
					noffset = 0;
				}
			}
			else if (falphaLv2 >= DEF_CHRIS_ALPHA_MAX)
			{
				bUp = false;

				if (noffset == 0)
				{
					noffset = 1;
				}
			}
		}

		if (bUp == true)
		{
			if (bEffect1 == true)
			{
				falphaLv1 += (fDeltaTime * CHRISTMAS_EFFECT_RATIO);

				if (falphaLv1 > DEF_CHRIS_ALPHA_MAX)
					falphaLv1 = DEF_CHRIS_ALPHA_MAX;
			}
			else
			{
				falphaLv2 += (fDeltaTime * CHRISTMAS_EFFECT_RATIO);

				if (falphaLv2 > DEF_CHRIS_ALPHA_MAX)
					falphaLv2 = DEF_CHRIS_ALPHA_MAX;
			}
		}
		else
		{
			if (bEffect1 == true)
			{
				falphaLv1 -= (fDeltaTime * CHRISTMAS_EFFECT_RATIO);

				if (falphaLv1 < DEF_CHRIS_ALPHA_MIN)
					falphaLv1 = DEF_CHRIS_ALPHA_MIN;
			}
			else
			{
				falphaLv2 -= (fDeltaTime * CHRISTMAS_EFFECT_RATIO);

				if (falphaLv2 < DEF_CHRIS_ALPHA_MIN)
					falphaLv2 = DEF_CHRIS_ALPHA_MIN;
			}
		}

					
		UtilHelp* pUtil = UtilHelp::getSingleton();
		COLOR colAlpha;

		if (pUtil != NULL)
		{
			colAlpha = DEF_UI_COLOR_WHITE;
			colAlpha = pUtil->GetColorAlpha(falphaLv1, colAlpha);
			m_pImgChrisEffect1->SetAlpha(colAlpha);

			colAlpha = DEF_UI_COLOR_WHITE;
			colAlpha = pUtil->GetColorAlpha(falphaLv2, colAlpha);
			m_pImgChrisEffect2->SetAlpha(colAlpha);
		}
	}
#endif	//	CHRISTMAS_LOGO
}

