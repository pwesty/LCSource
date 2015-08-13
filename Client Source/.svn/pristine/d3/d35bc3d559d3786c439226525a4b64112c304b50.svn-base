
#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIServerSelect.h"
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/GameState.h>
#include <Engine/Contents/Base/UIRankingSystem.h>
#include <Engine/Contents/Login/UICharacterSelect.h>
#include <Engine/GameStageManager/StageMgr.h>

#include "ServerSelect.h"
#include "BackImageManager.h"

#ifdef EUROUPEAN_SERVER_LOGIN
extern INDEX g_iConnectEuroupean;
#endif
extern INDEX g_iCountry;

class CmdServerOkCancel : public Command
{
public:
	CmdServerOkCancel() : m_pWnd(NULL), m_bJoin(false){}

	void setData(CUIServerSelect* pWnd, bool bJoin)	
	{
		m_pWnd = pWnd;
		m_bJoin = bJoin;
	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			if (!m_bJoin)
				m_pWnd->GotoLogin();
			else
				m_pWnd->ConnectServer();
		}
	}

private:
	CUIServerSelect* m_pWnd;
	bool			 m_bJoin;
};

class CmdOkButtonEnable : public Command
{
public:
	CmdOkButtonEnable() : m_pWnd(NULL) {}

	void setData(CUIServerSelect* pWnd)	{ m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd)
			m_pWnd->EnableButton();
	}
private:
	CUIServerSelect* m_pWnd;
};

CUIServerSelect::CUIServerSelect()
	: m_pServerList(NULL),
	m_pChannelList(NULL),
	m_pBtnCancel(NULL),
	m_pBtnOk(NULL)
{

	int i;

	for (i = 0; i < SERVER_MAX; ++i)
	{
		m_strServerName[LOCAL_NONE][i] = _S(796 + i, "ServerName");
		m_strServerName[LOCAL_EUROPEAN][i] = _S(5786 + i, "ServerName");
	}
}

void CUIServerSelect::initialize()
{
	m_pServerList	= (CUIList*)findUI("list_server");
	m_pChannelList	= (CUIList*)findUI("list_channel");

	if (m_pBtnCancel = (CUIButton*)findUI("btn_cancel"))
	{
		CmdServerOkCancel* pCmd = new CmdServerOkCancel;
		pCmd->setData(this, false);
		m_pBtnCancel->SetCommandUp(pCmd);
	}

	if (m_pBtnOk = (CUIButton*)findUI("btn_ok"))
	{
		CmdServerOkCancel* pCmd = new CmdServerOkCancel;
		pCmd->setData(this, true);
		m_pBtnOk->SetCommandUp(pCmd);
	}
}

void CUIServerSelect::OpenUI()
{
	if(IsVisible() == TRUE || GetHide() == FALSE )
		return;

	_pSound->Mute();

#ifdef RESTRICT_SOUND
	_pSound->UpdateSounds();
#endif // RESTRICT_SOUND

	UpdateServerList();

	SetVisible(TRUE);
	Hide(FALSE);
	CUIManager::getSingleton()->RearrangeOrder(UI_SERVER_SELECT, TRUE);

	CBackImageManager* pBGMgr = CBackImageManager::getSingleton();

	if (pBGMgr->IsInit() == false)
		pBGMgr->InitData();

	pBGMgr->SetRenderTex(CBackImageManager::eTEX_LOGIN_BACK);
	
	UIMGR()->SetCSFlagOff(CSF_TELEPORT);
	Lock(FALSE);
}

void CUIServerSelect::CloseUI()
{
	_pSound->Mute();

	Hide(TRUE);
	SetVisible(FALSE);
	Lock(FALSE);

	CUIManager::getSingleton()->RearrangeOrder(UI_SERVER_SELECT, FALSE);
	
	
	if (GameDataManager* pGameData = GameDataManager::getSingleton())
	{
		if (CServerSelect* pServerData = pGameData->GetServerData())
			pServerData->ResetData();
	}

	CBackImageManager::getSingleton()->SetRenderTex(CBackImageManager::eTEX_NONE);

	if (m_pServerList != NULL && m_pChannelList != NULL)
	{
		m_pServerList->DeleteAllListItem();
		m_pChannelList->DeleteAllListItem();
	}
}

void CUIServerSelect::GotoLogin()
{
#ifdef RESTRICT_SOUND
	_pSound->UpdateSounds();
#endif // RESTRICT_SOUND

	extern BOOL g_bAutoLogin;
	extern BOOL g_bAutoRestart;
	if(g_bAutoLogin)
	{
		_pGameState->Running()		= FALSE;
		_pGameState->QuitScreen()	= TRUE;
	}
	else if( g_bAutoRestart )
		g_bAutoRestart = FALSE;

	_cmiComm.Reconnect(_cmiComm.cci_szAddr, _cmiComm.cci_iPort);
	
	STAGEMGR()->SetNextStage(eSTAGE_LOGIN);
}

void CUIServerSelect::Lock( BOOL bLock )
{
	if (m_pBtnCancel == NULL || m_pBtnOk == NULL)
		return;

	m_pBtnOk->SetEnable(!bLock);
	m_pBtnCancel->SetEnable(!bLock);
}

void CUIServerSelect::ErrMessage( int nErrCode )
{
	CUIMsgBox_Info	MsgBoxInfo;
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox(MSGCMD_CONNECT_ERROR);

	switch( nErrCode )
	{
	case eERR_NOT_CONNECT:
		{
			MsgBoxInfo.SetMsgBoxInfo( _S( 424, "접속 오류" ), UMBS_OK, UI_SERVER_SELECT, MSGCMD_CONNECT_ERROR );
			MsgBoxInfo.AddString( _S( 426, "게임 서버와 연결할 수 없습니다." ) );
		}
		break;
	}
	
	pUIManager->CreateMessageBox( MsgBoxInfo );

	_pNetwork->m_bSendMessage = FALSE;
}

void CUIServerSelect::UpdateServerList()
{
	GameDataManager* pDataManager = GameDataManager::getSingleton();

	if (m_pServerList == NULL || pDataManager == NULL)
		return;

	CServerSelect* pServerData = pDataManager->GetServerData();

	if (pServerData == NULL)
		return;

	CUIText* pText = NULL;
	CUIImageArray* pImgArray = NULL;
	CUIListItem* pListItem = NULL;
	CUIListItem* pTmpItem = NULL;

	pListItem = m_pServerList->GetListItemTemplate();

	if (pListItem == NULL)
		return;

	int nItemCnt = m_pServerList->getListItemCount();
	int nServerCnt = pServerData->GetServerCount();
	int i;

	for (i = 0; i < nServerCnt; ++i)
	{
		if (i >= nItemCnt)
			m_pServerList->AddListItem((CUIListItem*)pListItem->Clone());

		pTmpItem = (CUIListItem*)m_pServerList->GetListItem(i);

		sServerInfo* pServerInfo = pServerData->ServerListAt(i);

		if (pTmpItem == NULL || pServerInfo == NULL)
			continue;

		if (pText = (CUIText*)pTmpItem->findUI("text_name"))
		{
			pText->SetText(GetServerName(pServerInfo->iServerNo));
		}

		if (pImgArray = (CUIImageArray*)pTmpItem->findUI("img_server_state"))
		{
 			UBYTE Type = pServerInfo->ubServerType;

			if (Type <= 0)
				pImgArray->Hide(TRUE);
			else
			{
				pImgArray->Hide(FALSE);
				pImgArray->SetRenderIdx(Type-1);	//타잎 0 == 일반, 1 == Recommend, 2 == Hardcore 이미지는 0 == Recommend 1 == Hardcore
			}
		}
	}

	m_pServerList->UpdateList();

	if (nServerCnt > 0)
	{
		int nCurServer = 0;
		if (pServerData->GetRecentServer() > 0)
			nCurServer = pServerData->GetRecentServer() - 1;	// 서버에서 주는 데이타는 1부터시작.

		m_pServerList->setCurSel(nCurServer);
		UpdateChannelList(nCurServer);
	}

	CmdOkButtonEnable* pCmd = new CmdOkButtonEnable;
	pCmd->setData(this);
	m_pServerList->SetCommand(pCmd);
}

void CUIServerSelect::UpdateChannelList(int serverNum)
{
	if (m_pServerList == NULL || m_pChannelList == NULL)
		return;

	if (serverNum < 0)	// 서버 선택이 안된 상태라면 채널리스트 하이드.
	{
		ChildHide(m_pChannelList, TRUE);
		m_pChannelList->setCurSel(serverNum);
	}

	GameDataManager* pDataManager = GameDataManager::getSingleton();

	if (pDataManager == NULL)
		return;

	CServerSelect* pServerData = pDataManager->GetServerData();

	if (pServerData == NULL)
		return;

	sServerInfo* pServerInfo = pServerData->ServerListAt(serverNum);

	if (pServerInfo == NULL)
		return;

	m_pChannelList->DeleteAllListItem();

	int nChannelCnt = pServerInfo->GetChannelCount();
	int i;

	CUIText* pText = NULL;
	CUIListItem* pListItem = NULL;
	CUIListItem* pTmpItem = NULL;	

	pListItem = m_pChannelList->GetListItemTemplate();

	for (i = 0; i < nChannelCnt; ++i)
	{
		m_pChannelList->AddListItem((CUIListItem*)pListItem->Clone());

		pTmpItem = (CUIListItem*)m_pChannelList->GetListItem(i);
		sChannelInfo* pChannelInfo = pServerInfo->ChannelListAt(i);

		if (pTmpItem == NULL || pChannelInfo == NULL)
			continue;

		if (pText = (CUIText*)pTmpItem->findUI("text_name"))
		{
			CTString strTmp;
			strTmp.PrintF("-%d", pChannelInfo->iSubNum);
			strTmp += GetChannelSubString(serverNum, i);

			pText->SetText(GetServerName(pServerInfo->iServerNo) + strTmp);
		}

		if (pText = (CUIText*)pTmpItem->findUI("text_state"))
		{
			if( pChannelInfo->iPlayerNum == -1 )
			{
				pText->SetText(CTString(_S( 372, "점검" )));
				pText->setFontColor(0x9400D6FF);
			}
			else if( pChannelInfo->iPlayerNum >= _cmiComm.cci_iFullUsers )
			{
				pText->SetText(CTString(_S( 373, "FULL" )));
				pText->setFontColor(0xD11184FF);
			}
			else if( pChannelInfo->iPlayerNum >= _cmiComm.cci_iBusyUsers )
			{
				pText->SetText(CTString(_S( 374, "혼잡" )));
				pText->setFontColor(0xFF9533FF);
			}
			else
			{
				pText->SetText(CTString(_S( 371, "원활" )));
				pText->setFontColor(0xCCCCCCFF);
			}
		}
	}

	m_pChannelList->UpdateList();
	m_pChannelList->UpdateScroll(nChannelCnt);

	if (nChannelCnt > 0)
	{
		int nCurChannel = 0;
		if (pServerData->GetRecentChannel() > 0)
			nCurChannel = pServerData->GetRecentChannel() - 1;

		m_pChannelList->setCurSel(nCurChannel);
	}

	CmdOkButtonEnable* pCmd = new CmdOkButtonEnable;
	pCmd->setData(this);
	m_pChannelList->SetCommand(pCmd);

}

CTString CUIServerSelect::GetServerName( int serverNum )
{
	--serverNum;	// 서버 넘버는 첫번째가 1, 배열 인덱스는 0부터 시작.

	if (serverNum < 0 && serverNum >= SERVER_MAX)
		return CTString("");

#ifdef EUROUPEAN_SERVER_LOGIN
	if (g_iConnectEuroupean)
		return m_strServerName[LOCAL_EUROPEAN][serverNum];
#endif // EUROUPEAN_SERVER_LOGIN

	return m_strServerName[LOCAL_NONE][serverNum];
}

WMSG_RESULT CUIServerSelect::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pServerList && m_pServerList->IsInside(x, y) )
	{
		int nIdx = m_pServerList->HitTest(x, y);

		UpdateChannelList(nIdx);
	}
	return WMSG_FAIL;
}

WMSG_RESULT CUIServerSelect::OnKeyMessage( MSG* pMsg )
{
	if (m_pBtnOk == NULL || m_pBtnOk->IsEnabled() == FALSE)
		return WMSG_FAIL;

	if( pMsg->wParam == VK_RETURN )
	{	
		ConnectServer();
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

void CUIServerSelect::ConnectServer()
{
	if (m_pServerList == NULL || m_pChannelList == NULL)
		return;

	int nSelServer = m_pServerList->getCurSel();
	int nSelChannel = m_pChannelList->getCurSel();

	if (nSelServer < 0 || nSelChannel < 0)
		return;

	GameDataManager* pDataManager = GameDataManager::getSingleton();

	if (pDataManager == NULL)
		return;

	CServerSelect* pServerData = pDataManager->GetServerData();

	if (pServerData == NULL)
		return;

	sServerInfo* pServerInfo = pServerData->ServerListAt(nSelServer);

	if (pServerInfo == NULL)
		return;

	sChannelInfo* pChannel = pServerInfo->ChannelListAt(nSelChannel);

	if (pChannel == NULL)
		return;
	
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(_pNetwork->m_iServerGroup != pServerInfo->iServerNo)
	{
		pUIManager->GetRankingViewEx()->ClearRankingList();
	}

	pServerData->ConnectToServer(pChannel->strAddress, pChannel->iPortNum);	// 서버에 접속.

	_pNetwork->m_iServerGroup = pServerInfo->iServerNo;
	_pNetwork->m_iServerCh = pChannel->iSubNum;		// UI_REFORM :Su-won
	_pNetwork->m_iServerType = pServerInfo->ubServerType;

	if (_pNetwork->m_iServerType == SERVER_TYPE_HARDCORE)
		pUIManager->InitHardCoreCreate();
	
	_pGameState->ClearCharacterSlot();
}

CTString CUIServerSelect::GetChannelSubString( int nServer, int nChannel )
{
	GameDataManager* pDataManager = GameDataManager::getSingleton();

	if (pDataManager == NULL)
		return CTString("");

	CServerSelect* pServerData = pDataManager->GetServerData();

	if (pServerData == NULL)
		return CTString("");

	sServerInfo* pServerInfo = pServerData->ServerListAt(nServer);

	if (pServerInfo == NULL)
		return CTString("");

	sChannelInfo* pChannelInfo = pServerInfo->ChannelListAt(nChannel);

	if (pChannelInfo == NULL)
		return CTString("");

	int serverNum = pServerInfo->iServerNo;
	int channelNum = pChannelInfo->iSubNum;

	if ( serverNum == 1001 )
	{
		serverNum = 11;
	}

	CTString strServer;
	extern INDEX g_iCountry;
		
	int nStrIdx;

	nStrIdx = getNomalChannelStrIdx(channelNum);

#ifdef EUROUPEAN_SERVER_LOGIN
	if(g_iConnectEuroupean)
		nStrIdx = getEUChannelStrIdx(channelNum);
#endif

	if (g_iCountry == THAILAND)
	{
		switch(channelNum)
		{
		case 1:
			strServer.PrintF( "(%s)", _S( 2417, "공성" ) );
			break;
		case 5:
			strServer.PrintF( "(%s)", _S( 5599, "상점채널" ) );
			break;
		default:
			strServer = CTString("");
		}
	}
	else
	{
		if (nStrIdx <= 0)
			strServer = CTString("");
		else
			strServer.PrintF( "%s", _S( nStrIdx, "서버 상태" ) );
	}

	return strServer;
}

void CUIServerSelect::EnableButton()
{
	if (m_pBtnOk == NULL || m_pChannelList == NULL)
		return;

	if (m_pChannelList->getCurSel() < 0)
		m_pBtnOk->SetEnable(FALSE);
	else
		m_pBtnOk->SetEnable(TRUE);
}

void CUIServerSelect::ChildHide( CUIList* pList, BOOL bHide )
{
	if (pList == NULL)
		return;

	int i, nCnt = pList->getListItemCount();

	CUIListItem* pItem;

	for (i = 0; i < nCnt; ++i)
	{
		pItem = (CUIListItem*)pList->GetListItem(i);

		if ( pItem == NULL )
			continue;

		pItem->Hide(bHide);
	}
}

int CUIServerSelect::getNomalChannelStrIdx( int nChannel )
{
	switch(nChannel)
	{
	case 1:
		return 6291;
	case 2:
		return 6292;
	case 3:
		return 6293;
	case 4:
		return 6294;
	case 5:
		return 6295;
	case 6:
		return 6296;
	case 7:
		return 6297;
	case 8:
		return 6298;
	case 9:
		return 6299;
	case 10:
		return 6300;
	}
	return 0;
}

int CUIServerSelect::getEUChannelStrIdx( int nChannel )
{
	switch(nChannel)
	{
	case 1:
		return 6301;
	case 2:
		return 6302;
	case 3:
		return 6303;
	case 4:
		return 6304;
	case 5:
		return 6305;
	case 6:
		return 6306;
	case 7:
		return 6307;
	case 8:
		return 6308;
	case 9:
		return 6309;
	case 10:
		return 6310;
	}
	return 0;
}
