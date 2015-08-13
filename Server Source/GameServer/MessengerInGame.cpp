#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Server.h"
#include "MessengerInGame.h"

#define RECONNECT_TIMER_FOR_MESSENGER		(10 * 1000)

MessengerInGameConnectTimer* MessengerInGameConnectTimer::instance()
{
	static MessengerInGameConnectTimer __instance;
	return &__instance;
}

void MessengerInGameConnectTimer::operate( rnSocketIOService* service )
{
	// 현 타이머 삭제
	bnf::instance()->RemoveSession(service);

	// 메신져 서버에 접속
	MessengerInGame::instance()->connect();
}

//////////////////////////////////////////////////////////////////////////

void MessengerInGame::connect()
{
	// 메시젼 서버에 접속 시도
	host_ = gserver->m_config.Find("Messenger Server", "IP");
	port_ = atoi(gserver->m_config.Find("Messenger Server", "Port"));
	bnf::instance()->CreateAsyncConnect(host_, port_, this, CONNECT_TYPE_FOR_CNETMSG);
}

void MessengerInGame::WriteToOutput( CNetMsg::SP& msg )
{
	if (msg->m_mtype == MSG_UNKNOWN)
		return;

	if (service_ == NULL)
		return;

	service_->deliver(msg);
}

void MessengerInGame::operate( rnSocketIOService* service )
{
	CNetMsg::SP msg(service->GetMessage());

	gserver->ProcessMessengerMessage(msg);
}

void MessengerInGame::onClose( rnSocketIOService* service )
{
	bnf::instance()->RemoveSession(service);
	service_ = NULL;

	// 재접속 타이머 생성
	session_handle handle = bnf::instance()->CreateMSecTimer(RECONNECT_TIMER_FOR_MESSENGER,
							(void *)MessengerInGameConnectTimer::instance());

	LOG_FATAL("%%%%%%%%%% Disconnected from Messenger Server (%s : %d) %%%%%%%%%%\n\n",
			  host_.c_str(), port_);
}

void MessengerInGame::onConnect( rnSocketIOService* service )
{
	service_ = service;

	// 접속 후 최초 메시지 전달
	this->WriteToOutput(gserver->m_messengerConnMsg);

	LOG_INFO("connected to Messenger server (%s : %d)", host_.c_str(), port_);
}

void MessengerInGame::onConnectFail( rnSocketIOService* service )
{
	LOG_INFO("Can't connect to Messenger server (%s : %d)", host_.c_str(), port_);
}

MessengerInGame* MessengerInGame::instance()
{
	static MessengerInGame __instance;
	return &__instance;
}