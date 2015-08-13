#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Server.h"
#include "SubHelperInGame.h"

#define RECONNECT_TIMER_FOR_MESSENGER		(10 * 1000)

SubHelperInGameConnectTimer* SubHelperInGameConnectTimer::instance()
{
	static SubHelperInGameConnectTimer __instance;
	return &__instance;
}

void SubHelperInGameConnectTimer::operate( rnSocketIOService* service )
{
	// 현 타이머 삭제
	bnf::instance()->RemoveSession(service);

	// 메신져 서버에 접속
	SubHelperInGame::instance()->connect();
}

//////////////////////////////////////////////////////////////////////////

void SubHelperInGame::connect()
{
	// 메시젼 서버에 접속 시도
	host_ = gserver->m_config.Find("SubHelper Server", "IP");
	port_ = atoi(gserver->m_config.Find("SubHelper Server", "Port"));
	bnf::instance()->CreateAsyncConnect(host_, port_, this, CONNECT_TYPE_FOR_CNETMSG);
}

void SubHelperInGame::WriteToOutput( CNetMsg::SP& msg )
{
	if (msg->m_mtype == MSG_UNKNOWN)
		return;

	if (service_ == NULL)
		return;

	service_->deliver(msg);
}

void SubHelperInGame::operate( rnSocketIOService* service )
{
	CNetMsg::SP msg(service->GetMessage());

	gserver->ProcessSubHelperMessage(msg);
}

void SubHelperInGame::onClose( rnSocketIOService* service )
{
	LOG_FATAL("%%%%%%%%%% Disconnected from SubHelper Server (%s : %d) %%%%%%%%%%\n\n",
			  host_.c_str(), port_);

	bnf::instance()->RemoveSession(service);
	service_ = NULL;

	// 재접속 타이머 생성
	session_handle handle = bnf::instance()->CreateMSecTimer(RECONNECT_TIMER_FOR_MESSENGER,
							(void *)SubHelperInGameConnectTimer::instance());
}

void SubHelperInGame::onConnect( rnSocketIOService* service )
{
	service_ = service;

	// 접속 후 최초 메시지 전달
	this->WriteToOutput(gserver->m_subHelperConnMsg);

	LOG_INFO("connected to SubHelper server (%s : %d)", host_.c_str(), port_);
}

void SubHelperInGame::onConnectFail( rnSocketIOService* service )
{
	LOG_INFO("Can't connected to SubHelper server (%s : %d)", host_.c_str(), port_);
}

SubHelperInGame* SubHelperInGame::instance()
{
	static SubHelperInGame __instance;
	return &__instance;
}