#include "../ShareLib/bnf.h"
#include "../ShareLib/NetMsg.h"
#include "../ShareLib/packetType/ptype_authentication.h"
#include "ClientForServer.h"

void ClientForServer::operate( rnSocketIOService* service )
{
	CNetMsg::SP msg(service->GetMessage());

	Authentication::auth* packet = reinterpret_cast<Authentication::auth*>(msg->m_buf);
	packet->ip[15] = '\0';

	LOG_INFO("SERVER_INFO - nation : %d : ip : %s : port : %d : client-ip : %s",
		(int)packet->nation, packet->ip, packet->port, service->ip().c_str());

	service->Close("end");
}

void ClientForServer::onClose( rnSocketIOService* service )
{
	delete this;

	bnf::instance()->RemoveSession(service);
}

//////////////////////////////////////////////////////////////////////////

static ClientForServerListen __instance;
ClientForServerListen* ClientForServerListen::instance()
{
	return &__instance;
}

void ClientForServerListen::operate( rnSocketIOService* service )
{
	ClientForServer* pServer = new ClientForServer;
	service->SetUserData(pServer);
}
