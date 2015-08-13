#include "stdhdrs.h"

#include "Log.h"
#include "Server.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"
#include "doFunc.h"
#include "../ShareLib/packetType/ptype_reserved_gm_command.h"
#include "../ShareLib/packetType/ptype_server_to_server.h"
#include "../ShareLib/LogInOutManager.h"
#include "ReservedGmCommandManager.h"
#include "../ShareLib/packetType/ptype_express_system.h"

void ServerSrandom(unsigned long initial_seed);

void CServer::CommandInterpreter(CDescriptor* d, CNetMsg::SP& msg)
{
	switch (msg->m_mtype)
	{
	case MSG_SUBHELPER_REQ:
		do_Request(msg, d);
		break;

	case MSG_SUBHELPER_REP:
		do_Reply(msg, d);
		break;

	case MSG_SUBHELPER_COMMAND:
		do_Command(msg, d);
		break;

	case MSG_TRADEAGENT:
		do_TradeAgent(msg, d);
		break;

#if defined (NOTICE_SYSTEM)
	case MSG_SUBHELPER_NOTICE:
		do_Notice(msg, d);
		break;
#endif // NOTICE_SYSTEM

	case MSG_RESERVED_GM_COMMAND:
		{
			RequestClient::reservedGMCommandList* packet = reinterpret_cast<RequestClient::reservedGMCommandList*>(msg->m_buf);
			ReservedGmCommandManager::Instance()->sendListToClient(d->service_, packet->m_Index, packet->year, packet->month);
		}
		break;

	case MSG_EXPRESS_SYSTEM:
		{
			pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);
			// GameServer에서 보내온 Express 알림 패킷을 처리함
			if (pBase->subType == MSG_SUB_EXPRESS_ONE_USER)
			{
				RequestGMTool::sendItemToOneUser* packet = reinterpret_cast<RequestGMTool::sendItemToOneUser*>(msg->m_buf);
				rnSocketIOService* pService = LogInOutManager::Instance()->getSocketIOByCharIndex(packet->char_index);
				if (pService)
				{
					pService->deliver(msg);
				}
			}
		}
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
//