#include "../ShareLib/bnf.h"
#include "../ShareLib/NetMsg.h"
#include "../ShareLib/packetType/ptype_authentication.h"
#include "ClientForClient.h"

void ClientForClient::operate( rnSocketIOService* service )
{
	CNetMsg::SP msg(service->GetMessage());

	Authentication::auth* packet = reinterpret_cast<Authentication::auth*>(msg->m_buf);
	packet->ip[15] = '\0';

	LOG_INFO("CLIENT_INFO - nation : %d : ip : %s : port : %d : client-ip : %s",
		(int)packet->nation, packet->ip, packet->port, service->ip().c_str());

	service->Close("end");
}

void ClientForClient::onClose( rnSocketIOService* service )
{
	delete this;

	bnf::instance()->RemoveSession(service);
}

//////////////////////////////////////////////////////////////////////////

static ClientForClientListen __instance;
ClientForClientListen* ClientForClientListen::instance()
{
	return &__instance;
}

void ClientForClientListen::operate( rnSocketIOService* service )
{
	ClientForClient* pClient = new ClientForClient;
	service->SetUserData(pClient);
	service->setCrypt();
}
