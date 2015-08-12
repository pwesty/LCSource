#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "PCList.h"
#include "DBManager.h"
#include "EventProcessWithThread.h"

#include "../ShareLib/packetType/ptype_gps.h"

void do_GPS_AddTarget(CPC* pc, CNetMsg::SP& msg);

void do_GPS(CPC* ch, CNetMsg::SP& msg)
{
	if (!ch)
		return;

	msg->MoveFirst();

	unsigned char subtype;
	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_SUB_GPS_ADD_TARGET_REQ:
		do_GPS_AddTarget(ch, msg);
		break;
	}	
}


void do_GPS_AddTarget(CPC* pc, CNetMsg::SP& msg)
{
	RequestClient::GPSAddTarget* packet = reinterpret_cast<RequestClient::GPSAddTarget*>(msg->m_buf);
	std::string search_name = packet->UserName;

	if (pc->GetName() == search_name)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeGPSErrorMsg(rmsg, ResponseClient::ERROR_GPS_NOT_FOUND_USER);
		SEND_Q(rmsg, pc->m_desc);
		
		return;
	}

	CItem *pItem = pc->m_inventory.getItem(packet->tab, packet->invenIndex);
	if(pItem == NULL)
	{
		LOG_ERROR("HACKING? Invalid Packet.tab[%d], invenIndex[%d]", packet->tab, packet->invenIndex);
		pc->m_desc->Close("Invalid packet.");
		return;
	}

	if (pItem->m_itemProto->isGPS() == false)
	{
		LOG_ERROR("HACKING? Invalid Item type[%d], subtype[%d]", pItem->m_itemProto->getItemTypeIdx(), pItem->m_itemProto->getItemSubTypeIdx());
		pc->m_desc->Close("Invalid Item.");
		return;
	}

	DBManager::instance()->pushGetCharIndexByGPS(pc->m_desc, search_name, pItem->getVIndex());
}


void getCharIndexByGPS_return_DBThread(boost::any& any)
{
	EventProcessForDB::getCharIndexByGPS* data = boost::any_cast<EventProcessForDB::getCharIndexByGPS>(&any);
	CPC* ch = PCManager::instance()->getPlayerByUserIndex(data->user_index);
	if (ch == NULL)
		return;

	if (ch->m_desc->m_seq_index != data->seq_index)
		return;

	if (data->char_index == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeGPSErrorMsg(rmsg, ResponseClient::ERROR_GPS_NOT_FOUND_USER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItem* pItem = ch->m_inventory.FindByVirtualIndex(data->itemVIndex);
	if (pItem == NULL)
	{
		LOG_ERROR("HACKING? Not found GPS Item. charIndex[%d]", data->char_index);
		ch->m_desc->Close("Not found GPS Item.");
		return;
	}

	if( do_UseGpsItem(pItem, ch) == false )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DO_NOT_USE_DURING_IMMORTAL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	
	ch->m_inventory.decreaseItemCount(pItem, 1);
	ch->m_gpsManager.inputTargetInfo(data->char_index, data->char_name.c_str());
	ch->m_gpsManager.sendTargetInfo();
}
