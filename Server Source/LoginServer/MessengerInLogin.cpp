#include <string>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Server.h"
#include "MessengerInLogin.h"

#define RECONNECT_TO_MESSENGER_TIME		(5 * 1000)

void MessengerInLoginTimer::operate( rnSocketIOService* service )
{
	// 자신의 타이머 삭제
	bnf::instance()->RemoveSession(service);

	// 재접속 시도
	MessengerInLogin::instance()->Connect();
}

MessengerInLoginTimer* MessengerInLoginTimer::instance()
{
	static MessengerInLoginTimer __instance;
	return &__instance;
}

//////////////////////////////////////////////////////////////////////////
void MessengerInLogin::Connect()
{
	host_ = gserver.m_config.Find("Messenger Server", "IP");
	port_ = atoi(gserver.m_config.Find("Messenger Server", "Port"));

	bnf::instance()->CreateAsyncConnect(host_, port_, this);
}

void MessengerInLogin::WriteToOutput( CNetMsg::SP& msg )
{
	if (msg->m_mtype == MSG_UNKNOWN)
		return ;

	if (service_ == NULL)
		return;

	service_->deliver(msg);
}

void MessengerInLogin::operate( rnSocketIOService* service )
{
	CNetMsg::SP msg(service->GetMessage());

	gserver.ProcMessenger(msg);
}

void MessengerInLogin::onClose( rnSocketIOService* service )
{
	service_ = NULL;

	bnf::instance()->RemoveSession(service);

	// 접속 타이머 생성
	bnf::instance()->CreateMSecTimer(RECONNECT_TO_MESSENGER_TIME, MessengerInLoginTimer::instance());

	LOG_FATAL("********************* Disconnect from Messenger server (%s : %d) *********************",
			  host_.c_str(), port_);
}

void MessengerInLogin::onConnect( rnSocketIOService* service )
{
	service_ = service;

	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_MSGR_CONNECT);
	RefMsg(rmsg)	<< SERVER_VERSION
					<< (int)LOGIN_SERVER_NUM	// 서버군번호
					<< (int)0					// 서브 넘버
					<< 0;						// 카운트존

	this->WriteToOutput(rmsg);

	LOG_INFO("connected to Messenger server (%s : %d)", host_.c_str(), port_);
}

void MessengerInLogin::onConnectFail( rnSocketIOService* service )
{
	LOG_INFO("Can't connect to Messenger server (%s : %d)", host_.c_str(), port_);
}

MessengerInLogin* MessengerInLogin::instance()
{
	static MessengerInLogin __instance;
	return &__instance;
}