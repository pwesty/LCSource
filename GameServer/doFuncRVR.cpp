#include "stdhdrs.h"

#include "Character.h"
#include "Server.h"
#include "doFunc.h"
#include "CmdMsg.h"
#include "../ShareLib/packetType/ptype_syndicate.h"

void do_RVRRemoveSyndicate(CPC* ch, CNetMsg::SP& msg);
void do_RVRGiveJewel(CPC* ch, CNetMsg::SP& msg);
void do_RVRUserInfo(CPC* ch, CNetMsg::SP& msg);
void do_RVRServerInfo(CPC* ch, CNetMsg::SP& msg);

void do_RVR(CPC* ch, CNetMsg::SP& msg)
{
	if (!ch)
		return;

	msg->MoveFirst();

	unsigned char subtype;
	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_RVR_REMOVE_REQ:
		do_RVRRemoveSyndicate(ch, msg);
		break;

	case MSG_RVR_GIVE_JEWEL_REQ:
		do_RVRGiveJewel(ch, msg);
		break;

	case MSG_RVR_SYNDICATE_INFO_USER_REQ:
		do_RVRUserInfo(ch, msg);
		break;

	case MSG_RVR_SYNDICATE_INFO_SERVER_REQ:
		do_RVRServerInfo(ch, msg);
		break;
	}
}

void do_RVRRemoveSyndicate(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::RVRRemoveRequest* p = reinterpret_cast<RequestClient::RVRRemoveRequest*>(msg->m_buf);
	int npcIndex = p->npcIndex;
	CNPC* npc = (CNPC *)ch->m_pArea->FindCharInCell(ch, npcIndex, MSG_CHAR_NPC);

	if (npc == NULL)
	{
		LOG_ERROR("HACKING? : not exist rvr npc. char_index[%d] rvr_type[%d] npc_index[%d]", ch->m_index, ch->getSyndicateType(), npcIndex);
		ch->m_desc->Close("invalid npc");
		return;
	}

	if (npc->m_proto->m_rvr_value != ch->getSyndicateType())
	{
		LOG_ERROR("HACKING? : not exist rvr npc. char_index[%d] rvr_type[%d] npc_index[%d]", ch->m_index, ch->getSyndicateType(), npcIndex);
		ch->m_desc->Close("invalid npc");
		return;
	}

	if (ch->getSyndicateType() == 0 )
	{
		LOG_ERROR("HACKING? : invalid syndicate type. char_index[%d] rvr_type[%d] npc_index[%d]", ch->m_index, ch->getSyndicateType(), npcIndex);
		ch->m_desc->Close("invalid npc");
		return;
	}

	// 결사대 장비를 착용하고 있는지 검사
	if (ch->m_wearInventory.isSyndicateEquipment())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSyndicateErrorMsg(rmsg, ResponseClient::ERROR_SYNDICATE_REMOVE_FAIL_ALREADY_EQUIP, ch->getSyndicateType());
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItem* item = ch->m_inventory.getItem(p->tab, p->invenIndex);	
	// 결사대 탈퇴 처리
	ch->m_syndicateManager.removeSyndicate(item);
}

void do_RVRGiveJewel(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::RVRGiveJewel* p = reinterpret_cast<RequestClient::RVRGiveJewel*>(msg->m_buf);
	CNPC* npc = (CNPC *)ch->m_pArea->FindCharInCell(ch, p->npcIndex, MSG_CHAR_NPC);
	if (npc == NULL)
	{
		LOG_ERROR("HACKING? : not exist rvr npc. char_index[%d] rvr_type[%d]", ch->m_index, ch->getSyndicateType());
		ch->m_desc->Close("invalid npc");
		return;
	}
	if (npc->m_proto->m_rvr_value != ch->getSyndicateType())
	{
		LOG_ERROR("HACKING? : not exist rvr npc. char_index[%d] rvr_type[%d]", ch->m_index, ch->getSyndicateType());
		ch->m_desc->Close("invalid npc");
		return;
	}
	if (ch->getSyndicateType() == SYNDICATE::eSYNDICATE_NONE || ch->getSyndicateType() == SYNDICATE::eSYNDICATE_FLOR)
	{
		LOG_ERROR("HACKING? : invalid syndicate type. char_index[%d] rvr_type[%d]", ch->m_index, ch->getSyndicateType());
		ch->m_desc->Close("invalid npc");
		return;
	}

	ch->m_syndicateManager.syndicateAddJewel(p->vIndexByCreateJewel);
}

void do_RVRUserInfo(CPC* ch, CNetMsg::SP& msg)
{
	//유저의 결사대 정보를 보내준다.
	ch->m_syndicateManager.sendUserInfo();
	ch->m_syndicateManager.historyManager_.sendHistory(ch->getSyndicateType());
	SyndicateInfoDataManager::instance()->sendJewelPercentageInfo(ch->m_desc);
}

void do_RVRServerInfo(CPC* ch, CNetMsg::SP& msg)
{
	SyndicateInfoDataManager::instance()->sendSyndicateInfo(ch->m_desc);
}