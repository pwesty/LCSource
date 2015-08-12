#include "stdhdrs.h"

#include "Log.h"
#include "Server.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"
#include "doFunc.h"
#include "../ShareLib/packetType/ptype_server_to_server.h"
#include "../ShareLib/packetType/ptype_server_to_server_kick.h"
#include "../ShareLib/LogInOutManager.h"

void ServerSrandom(unsigned long initial_seed);
void kickUser(rnSocketIOService* service, CNetMsg::SP& msg);
void kickUserAnswer(rnSocketIOService* service, CNetMsg::SP& msg);
void kickUserByCharIndex(rnSocketIOService* service, CNetMsg::SP& msg);
void kickUserByUserIndex(rnSocketIOService* service, CNetMsg::SP& msg);
void kickUserByUserId(rnSocketIOService* service, CNetMsg::SP& msg);

void CServer::CommandInterpreter(CDescriptor* d, CNetMsg::SP& msg)
{
	switch (msg->m_mtype)
	{
	case MSG_HELPER_REQ:
		do_Request(msg, d);
		break;

	case MSG_HELPER_REP:
		do_Reply(msg, d);
		break;

	case MSG_HELPER_COMMAND:
		do_Command(msg, d);
		break;

	case MSG_HELPER_COMMAND_STRUCT:
		do_Command_Struct(msg, d);
		break;

	case MSG_SERVER_TO_SERVER:
		{
			pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);
			switch (pBase->subType)
			{
			case MSG_SUB_SERVERTOSERVER_USER_LOGIN:
				LogInOutManager::Instance()->Add(d->service_, msg);
				break;

			case MSG_SUB_SERVERTOSERVER_USER_LOGOUT:
				LogInOutManager::Instance()->Delete(msg);
				break;

			case MSG_SUB_SERVERTOSERVER_CHANGE_NICK_NAME:
				{
					ServerToServerPacket::changeNickName* packet = reinterpret_cast<ServerToServerPacket::changeNickName*>(msg->m_buf);
					LogInOutManager::Instance()->changeNickName(packet->oldNickName, packet->newNickName);
				}
				break;

			case MSG_SUB_SERVERTOSERVER_KICK_REQ:
				kickUser(d->service_, msg);
				break;

			case MSG_SUB_SERVERTOSERVER_KICK_ANSER:
				kickUserAnswer(d->service_, msg);
				break;

			case MSG_SUB_SERVERTOSERVER_KICK_BY_CHAR_INDEX_REQ:
				kickUserByCharIndex(d->service_, msg);
				break;

			case MSG_SUB_SERVERTOSERVER_KICK_BY_USER_INDEX_REQ:
				kickUserByUserIndex(d->service_, msg);
				break;

			case MSG_SUB_SERVERTOSERVER_KICK_BY_USER_ID_REQ:
				kickUserByUserId(d->service_, msg);
				break;

			default:
				LOG_ERROR("SubHelper : invalid subtye[%d]", pBase->subType);
				break;
			}
		}
		break;
	default:
		{
			unsigned char subtype = 0;
			msg->MoveFirst();
			RefMsg(msg) >> subtype;

			GAMELOG << init("WARNING", d->service_->ip().c_str())
					<< "INVALID COMMAND" << delim
					<< msg->m_mtype << delim << msg->m_size << delim
					<< (int)subtype << end;
		}

		break;
	}
}

void kickUser(rnSocketIOService* service, CNetMsg::SP& msg)
{
	ServerToServerPacket::kickUser* packet = reinterpret_cast<ServerToServerPacket::kickUser*>(msg->m_buf);
	const LogInOutManager::userInfo* pInfo = LogInOutManager::Instance()->getUserInfoByCharname(packet->kicked_charName);

	if (pInfo == NULL)
	{
		// 캐릭터를 찾지 못한 경우
		packet->subType = MSG_SUB_SERVERTOSERVER_KICK_ANSER;
		packet->result = false;
		service->deliver(msg);
		return;
	}

	pInfo->service->deliver(msg);
}

void kickUserAnswer(rnSocketIOService* service, CNetMsg::SP& msg)
{
	ServerToServerPacket::kickUser* packet = reinterpret_cast<ServerToServerPacket::kickUser*>(msg->m_buf);
	const LogInOutManager::userInfo* pInfo = LogInOutManager::Instance()->getUserInfoByCharIndex(packet->req_charIndex);
	if (pInfo == NULL)
	{
		// "kick" 요청자가 접속을 종료했을 경우
		return;
	}

	pInfo->service->deliver(msg);
}

void kickUserByCharIndex(rnSocketIOService* service, CNetMsg::SP& msg)
{
	ServerToServerPacket::kickUserByCharIndex* packet = reinterpret_cast<ServerToServerPacket::kickUserByCharIndex*>(msg->m_buf);
	const LogInOutManager::userInfo* pInfo = LogInOutManager::Instance()->getUserInfoByCharIndex(packet->kicked_charIndex);
	if (pInfo == NULL)
	{
		packet->subType = MSG_SUB_SERVERTOSERVER_KICK_BY_CHAR_INDEX_ANSWER;
		packet->result = false;
		service->deliver(msg);
		return;
	}

	{
		// 캐명으로 변경하여 처리
		CNetMsg::SP rmsg(new CNetMsg);
		ServerToServerPacket::makeKickUser(rmsg, packet->req_charIndex, pInfo->charName.c_str());
		pInfo->service->deliver(rmsg);
	}
}

void kickUserByUserIndex(rnSocketIOService* service, CNetMsg::SP& msg)
{
	ServerToServerPacket::kickUserByUserIndex* packet = reinterpret_cast<ServerToServerPacket::kickUserByUserIndex*>(msg->m_buf);
	const LogInOutManager::userInfo* pInfo = LogInOutManager::Instance()->getUserInfoByUserIndex(packet->kicked_userIndex);
	if (pInfo == NULL)
	{
		packet->subType = MSG_SUB_SERVERTOSERVER_KICK_BY_USER_INDEX_ANSWER;
		packet->result = false;
		service->deliver(msg);
		return;
	}

	{
		// 캐명으로 변경하여 처리
		CNetMsg::SP rmsg(new CNetMsg);
		ServerToServerPacket::makeKickUser(rmsg, packet->req_charIndex, pInfo->charName.c_str());
		pInfo->service->deliver(rmsg);
	}
}

void kickUserByUserId(rnSocketIOService* service, CNetMsg::SP& msg)
{
	ServerToServerPacket::kickUserByUserId* packet = reinterpret_cast<ServerToServerPacket::kickUserByUserId*>(msg->m_buf);
	const LogInOutManager::userInfo* pInfo = LogInOutManager::Instance()->getUserInfoByUserId(packet->kicked_userId);
	if (pInfo == NULL)
	{
		packet->subType = 	MSG_SUB_SERVERTOSERVER_KICK_BY_USER_ID_ANSER;
		packet->result = false;
		service->deliver(msg);
		return;
	}

	{
		// 캐명으로 변경하여 처리
		CNetMsg::SP rmsg(new CNetMsg);
		ServerToServerPacket::makeKickUser(rmsg, packet->req_charIndex, pInfo->charName.c_str());
		pInfo->service->deliver(rmsg);
	}

}
