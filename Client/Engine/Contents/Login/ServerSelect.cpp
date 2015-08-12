
#include "StdH.h"

#include <Engine/Interface/UIManager.h>
#include <Engine/Network/NetworkMessage.h>
#include <Engine/network/CNetwork.h>
#include <Engine/Network/TcpIpConnection.h>

#include "UIServerSelect.h"
#include "ServerSelect.h"

CServerSelect::CServerSelect()
	: m_nRecentServer(-1)
	, m_nRecentChannel(-1)
{

}

CServerSelect::~CServerSelect()
{
	ResetData();
}

ENGINE_API void CServerSelect::ConnectToServer( CTString strIP, ULONG ulPort )
{
	if(_pNetwork->m_bSendMessage)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// 소켓의 연결을 끊었다가, 다시 연결함.
	_cmiComm.Reconnect(strIP, ulPort);
	if(_tcpip.Socket == INVALID_SOCKET)
	{
		pUIManager->GetServerSelect()->ErrMessage(CUIServerSelect::eERR_NOT_CONNECT);
		return;
	}

	_pNetwork->SendLoginMessage(_pNetwork->m_strUserID, _pNetwork->m_strUserPW, pUIManager->GetVersion());
	pUIManager->GetServerSelect()->Lock(TRUE);
}

void CServerSelect::AddServerList( LONG lServerNo, LONG lSubNum, LONG lPlayerNum, CTString &strAddress, LONG lPortNum ,UBYTE ubServerType )
{
	sServerInfo	ServerInfo;
	ServerInfo.iServerNo		= lServerNo;
	ServerInfo.ubServerType		= ubServerType;
	ServerInfo.nOrder			= -1;

	vecServerInfoIter iter = std::find_if(m_vecServerInfo.begin(), m_vecServerInfo.end(), FindServer(ServerInfo));

	if(iter == m_vecServerInfo.end())
	{
		m_vecServerInfo.push_back(ServerInfo);
		iter = m_vecServerInfo.end() - 1;
	}

	sChannelInfo	ChannelInfo;
	ChannelInfo.iPlayerNum	= lPlayerNum;
	ChannelInfo.iSubNum		= lSubNum;
	ChannelInfo.strAddress	= strAddress;
	ChannelInfo.iPortNum	= lPortNum;

	(*iter).vecChannelInfo.push_back(ChannelInfo);
}

void CServerSelect::ResetData()
{
	m_vecServerInfo.clear();
}

sServerInfo* CServerSelect::ServerListAt( int idx )
{
	if (idx < 0 || idx >= m_vecServerInfo.size() )
		return NULL;

	return (&m_vecServerInfo[idx]);
}
