#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Server.h"
#include "ConnectorInGame.h"

#define RECONNECT_TIMER_FOR_MESSENGER		(10 * 1000)

ConnectorInGameConnectTimer* ConnectorInGameConnectTimer::instance()
{
	static ConnectorInGameConnectTimer __instance;
	return &__instance;
}

void ConnectorInGameConnectTimer::operate( rnSocketIOService* service )
{
	// 현 타이머 삭제
	bnf::instance()->RemoveSession(service);

	// 메신져 서버에 접속
	ConnectorInGame::instance()->connect();
}

//////////////////////////////////////////////////////////////////////////

void ConnectorInGame::connect()
{
	// 메시젼 서버에 접속 시도
	host_ = gserver->m_config.Find("Connector Server", "IP");
	port_ = atoi(gserver->m_config.Find("Connector Server", "Port"));
	bnf::instance()->CreateAsyncConnect(host_, port_, this, CONNECT_TYPE_FOR_CNETMSG);
}

void ConnectorInGame::WriteToOutput( CNetMsg::SP& msg )
{
	if (msg->m_mtype == MSG_UNKNOWN)
		return;

	if (service_ == NULL)
		return;

	service_->deliver(msg);
}

void ConnectorInGame::operate( rnSocketIOService* service )
{
	CNetMsg::SP msg(service->GetMessage());

	gserver->ProcessBilling(msg);
}

void ConnectorInGame::onClose( rnSocketIOService* service )
{
	LOG_FATAL("%%%%%%%%%% Disconnected from Connector Server Please Check (%s : %d) %%%%%%%%%%\n",
			  host_.c_str(), port_);

	bnf::instance()->RemoveSession(service);
	service_ = NULL;

	// 재접속 타이머 생성
	session_handle handle = bnf::instance()->CreateMSecTimer(RECONNECT_TIMER_FOR_MESSENGER,
							(void *)ConnectorInGameConnectTimer::instance());
}

void ConnectorInGame::onConnect( rnSocketIOService* service )
{
	service_ = service;

	// 접속 후 최초 메시지 전달
	this->WriteToOutput(gserver->m_connectorConnMsg);

	LOG_INFO("connected to Connector server Awsome Keep Going (%s : %d)", host_.c_str(), port_);
}

void ConnectorInGame::onConnectFail( rnSocketIOService* service )
{
	LOG_INFO("Can't connected to Connector server Check Port And IP (%s : %d)", host_.c_str(), port_);
}

ConnectorInGame* ConnectorInGame::instance()
{
	static ConnectorInGame __instance;
	return &__instance;
}
