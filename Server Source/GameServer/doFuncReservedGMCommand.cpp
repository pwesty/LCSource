#define __DO_FUNC_RESERVED_GM_COMMAND_CPP__

#include "stdhdrs.h"

#include "GMCmdList.h"
#include "Character.h"
#include "Server.h"
#include "doFunc.h"
#include "../ShareLib/packetType/ptype_reserved_gm_command.h"
#include "DescManager.h"

void do_ReservedGMCommand(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::reservedGMCommandList* packet = reinterpret_cast<RequestClient::reservedGMCommandList*>(msg->m_buf);
	packet->m_Index = ch->m_desc->m_index;

	SEND_Q(msg, gserver->m_subHelper);
}

//////////////////////////////////////////////////////////////////////////
void ProcSubHelperReservedGMCommand(CNetMsg::SP& msg)
{
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);

	switch (pBase->subType)
	{
	case MSG_SUB_RESERVED_GM_COMMAND_LIST:
		{
			ResponseClient::reservedGMCommandList* packet = reinterpret_cast<ResponseClient::reservedGMCommandList*>(msg->m_buf);
			CDescriptor* desc = DescManager::instance()->getDescByUserIndex(packet->m_Index);
			if (desc == NULL)
				return;

			SEND_Q(msg, desc);
		}
		break;

	case MSG_SUB_ACTION_RESERVED_GM_COMMAND:
		{
			ResposeGameServer::addReservedGMCommand* packet = reinterpret_cast<ResposeGameServer::addReservedGMCommand*>(msg->m_buf);
			// 기존의 GM명령어를 실행하기 위해 가상의 캐릭터를 생성
			CPC* dummyChar = new CPC;
			dummyChar->m_desc = NULL;
			dummyChar->m_admin = 10;
			gGMCmdList->run(dummyChar, packet->command);
			delete dummyChar;
		}
		break;

	default:
		LOG_ERROR("reserved gm command : invalid subtype[%d]", pBase->subType);
		break;
	}
}