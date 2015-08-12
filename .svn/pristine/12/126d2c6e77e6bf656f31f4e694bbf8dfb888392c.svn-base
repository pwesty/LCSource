#define __DO_FUNC_EXPRESS_SYSTEM_CPP__

#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "DBManager.h"
#include "../ShareLib/packetType/ptype_express_system.h"

//////////////////////////////////////////////////////////////////////////
void process_expressList(CPC* ch, CNetMsg::SP& msg);
void process_expressTake(CPC* ch, CNetMsg::SP& msg);
void process_expressDelete(CPC* ch, CNetMsg::SP& msg);
void process_unsearchable_stonestatue_searchName(CPC* ch, CNetMsg::SP& msg);
void process_use_unsearchable_stonestatue(CPC* ch, CNetMsg::SP& msg);
ExpressSystemItemInfo* makeExpressSystemItemInfo(CItem* pItem, int itemCount, int sendType, const char* sendName);
//////////////////////////////////////////////////////////////////////////

void do_ExpressSystem(CPC* ch, CNetMsg::SP& msg)
{
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);

	switch (pBase->subType)
	{
	case MSG_SUB_EXPREES_LIST:
		process_expressList(ch, msg);
		break;

	case MSG_SUB_EXPRESS_TAKE:
		process_expressTake(ch, msg);
		break;

	case MSG_SUB_EXPRESS_DELETE:
		process_expressDelete(ch, msg);
		break;

	case MSG_SUB_EXPRESS_SEARCH_NICKNAME:
		process_unsearchable_stonestatue_searchName(ch, msg);
		break;

	case MSG_SUB_EXPRESS_SEND_ITEM_TO_NICKNAME:
		process_use_unsearchable_stonestatue(ch, msg);
		break;

	default:
		{
			std::string str = boost::str(boost::format("Invalid subtype[%1%]") % (int)pBase->subType);
			ch->m_desc->Close(str.c_str());
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

void process_expressList(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::expressList* packet = reinterpret_cast<RequestClient::expressList*>(msg->m_buf);

	// 원격 창고 기간제 아이템체크, 없으면 NCP 체크
	if (ch->m_TimerItem.isRemoteExpressSystem() == false)
	{
		CNPC* pNpc = ch->m_pArea->FindProtoNPCInCell(ch, packet->npcIndex, false, 2);
		if (pNpc == NULL)
		{
			return;
		}
	}

	DBManager::instance()->PushExpressList(ch->m_desc, packet->pageIndex);
}

//XX 거래대행 - 찾기 1
void process_expressTake(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::expressTake* packet = reinterpret_cast<RequestClient::expressTake*>(msg->m_buf);

	LOG_INFO("EXPRESS SYSTEM TAKE. TRYING TO TAKE. charIndex : %d, expressIndex : %d", ch->m_index, packet->index);

	// 원격 창고 기간제 아이템체크, 없으면 NCP 체크
	if (ch->m_TimerItem.isRemoteExpressSystem() == false)
	{
		CNPC* pNpc = ch->m_pArea->FindProtoNPCInCell(ch, packet->npcIndex, false, 2);
		if (pNpc == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeExpressTake(rmsg, ResponseClient::ERR_NOT_FOUND_NPC);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	DBManager::instance()->PushExpressTake(ch->m_desc, packet->index);
}

void process_expressDelete(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::expressDelete* packet = reinterpret_cast<RequestClient::expressDelete*>(msg->m_buf);

	// 원격 창고 기간제 아이템체크, 없으면 NCP 체크
	if (ch->m_TimerItem.isRemoteExpressSystem() == false)
	{
		CNPC* pNpc = ch->m_pArea->FindProtoNPCInCell(ch, packet->npcIndex, false, 2);
		if (pNpc == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeExpressDelete(rmsg, ResponseClient::ERR_NOT_FOUND_NPC);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	DBManager::instance()->PushExpressDelete(ch->m_desc, packet->index, true);
}

//////////////////////////////////////////////////////////////////////////

void process_expressInputItem(CNetMsg::SP& msg)
{
	RequestGMTool::sendItemToPlayingUser* packet = reinterpret_cast<RequestGMTool::sendItemToPlayingUser*>(msg->m_buf);

	if (packet->nas <= 0 && packet->item_index <= 0)
	{
		LOG_FATAL("invalid express item info. item_index[%d] / nas[%d]", packet->item_index, packet->nas);
		return;
	}

	if (packet->nas <= 0 && packet->item_index > 0) // 아이템 전송의 경우 item index의 유효성 검증
	{
		CItemProto* pItemProto = gserver->m_itemProtoList.FindIndex(packet->item_index);
		if (pItemProto == NULL)
		{
			LOG_FATAL("invalid express item index. item_index[%d]", packet->item_index);
			return;
		}
	}

	switch (packet->subType)
	{
	case MSG_SUB_EXPRESS_ALL_USER:
	case MSG_SUB_EXPRESS_SERVER_GROUP:
		{
			// 창고NPC에 아이템(또는 돈)을 저장
			PCManager::map_t& playerMap			= PCManager::instance()->getPlayerMap();
			PCManager::map_t::iterator it		= playerMap.begin();
			PCManager::map_t::iterator endit	= playerMap.end();
			for (; it != endit; ++it)
			{
				CPC* pChar = (*it).pPlayer;
				if (pChar == NULL)
					continue;

				if(pChar->m_level < packet->minLevel || pChar->m_level > packet->maxLevel)
					continue;

				if (packet->item_index > 0)
				{
					ExpressSystemItemInfo* iteminfo = new ExpressSystemItemInfo;
					iteminfo->item_index = packet->item_index;
					iteminfo->item_count = packet->item_count;
					iteminfo->send_type = EXPRESS_SEND_TYPE_GM;
					memcpy(iteminfo->sender, packet->sender, sizeof(iteminfo->sender));
					DBManager::instance()->PushExpressInputItem(pChar->m_desc, iteminfo, false);
				}
				else
				{
					DBManager::instance()->PushExpressInputNas(pChar->m_desc, packet->nas,
							EXPRESS_SEND_TYPE_GM, packet->sender, false);
				}
			}
		}
		break;

	case MSG_SUB_EXPRESS_ZONE:
		{
			if (gserver->FindZone(packet->zone_no) == NULL)
			{
				LOG_FATAL("invalid zone no[%d]", packet->zone_no);
				return;
			}
			PCManager::map_t& playerMap			= PCManager::instance()->getPlayerMap();
			PCManager::map_t::iterator it		= playerMap.begin();
			PCManager::map_t::iterator endit	= playerMap.end();
			for (; it != endit; ++it)
			{
				CPC* pChar = (*it).pPlayer;

				if (pChar->m_pZone->m_index != packet->zone_no)
					continue;

				if (packet->item_index > 0)
				{
					ExpressSystemItemInfo* iteminfo = new ExpressSystemItemInfo;
					iteminfo->item_index = packet->item_index;
					iteminfo->item_count = packet->item_count;
					iteminfo->send_type = EXPRESS_SEND_TYPE_GM;
					memcpy(iteminfo->sender, packet->sender, sizeof(iteminfo->sender));
					DBManager::instance()->PushExpressInputItem(pChar->m_desc, iteminfo, false);
				}
				else
				{
					DBManager::instance()->PushExpressInputNas(pChar->m_desc, packet->nas,
							EXPRESS_SEND_TYPE_GM, packet->sender, false);
				}
			}
		}
		break;

	default:
		LOG_FATAL("messenger server -> game server : invalid sub type[%d]", packet->subType);
		break;
	}
}

void process_unsearchable_stonestatue_searchName(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::express_unsearchable_stonestatue_searchName* packet = reinterpret_cast<RequestClient::express_unsearchable_stonestatue_searchName*>(msg->m_buf);

	packet->nickName[MAX_CHAR_NAME_LENGTH] = '\0';
	if (packet->nickName[0] == '\0')
	{
		LOG_ERROR("HACKING? : Nick name is empty. charIndex : %d", ch->m_index);
		ch->m_desc->Close("Nick name is empty");
		return;
	}

	if(findPercentChar(packet->nickName) != NULL)
		return ;

	// 자기 자신일 경우 바로 처리
	{
		std::string tname = packet->nickName;
		if ((const char *)ch->m_nick == tname)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeExpressSearchNickname(rmsg, 0);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	DBManager::instance()->PushExpressSearchNickName(ch->m_desc, packet->nickName);
}

void process_use_unsearchable_stonestatue(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::express_use_unsearchable_stonestatue* packet = reinterpret_cast<RequestClient::express_use_unsearchable_stonestatue*>(msg->m_buf);
	packet->toNickName[MAX_CHAR_NAME_LENGTH] = '\0';

	if (packet->toNickName[0] == '\0')
	{
		LOG_ERROR("HACKING? : Nick name is empty. charIndex : %d", ch->m_index);
		ch->m_desc->Close("Nick name is empty");
		return;
	}

	// 신비한 석상의 구슬 검사
	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	CItem* gemItem = ch->m_inventory.getItem(packet->tab, packet->invenIndex);
	
	if (gemItem == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("not found item");
		return;
	}

	if (gemItem->getVIndex() != packet->virualIndex)
	{
		LOG_ERROR("HACKING? : not equal virtual index. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("not equal virtual index");
		return;
	}

	if (gemItem->getDBIndex() != 9938)
	{
		LOG_ERROR("HACKING? : not gem item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("not gem item");
		return;
	}

	// 보내는 아이템 검사
	if (ch->m_inventory.isValidNormalInventory(packet->send_tab, packet->send_invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->send_tab, packet->send_invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	if (ch->m_inventory.isUsable(packet->send_tab) == false)
	{
		LOG_ERROR("HACKING? : not usable inventory. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->send_tab, packet->send_invenIndex);
		ch->m_desc->Close("not usable inventory");
		return;
	}

	CItem* sendItem = ch->m_inventory.getItem(packet->send_tab, packet->send_invenIndex);
	if (sendItem == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->send_tab, packet->send_invenIndex);
		ch->m_desc->Close("not found item");
		return;
	}

	//플래티늄 결합아이템은 불가능
	if(sendItem->IsUsedPlatinumSpecial())
	{
		LOG_ERROR("HACKING? : item used platinumSpecial. char_index[%d] tab[%d] invenIndex[%d]",
			ch->m_index, packet->send_tab, packet->send_invenIndex);
		ch->m_desc->Close("item used platinumSpecial");
		return;
	}

	// 귀속 아이템 불가능
	if(sendItem->IsBelong() == true)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_BELONG_ITEM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (sendItem->getVIndex() != packet->send_virutalIndex)
	{
		LOG_ERROR("HACKING? : not equal virtual index. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->send_tab, packet->send_invenIndex);
		ch->m_desc->Close("not equal virtual index");
		return;
	}

	if (sendItem->getWearPos() != WEARING_NONE)
	{
		LOG_ERROR("HACKING? : this item is wear item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->send_tab, packet->send_invenIndex);
		ch->m_desc->Close("this item is wear item");
		return;
	}

	if ((sendItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (sendItem->getFlag() & FLAG_ITEM_COMPOSITION))
	{
		LOG_ERROR("HACKING? : this item is composite item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->send_tab, packet->send_invenIndex);
		ch->m_desc->Close("this item is composite item");
		return;
	}

	if (sendItem->m_itemProto->isMonsterCard())
	{
		LOG_ERROR("HACKING? : this item is monster card. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->send_tab, packet->send_invenIndex);
		ch->m_desc->Close("this item is monster card");
		return;
	}

	if ((sendItem->m_itemProto->getItemFlag() & ITEM_FLAG_EXCHANGE) == 0)
	{
		LOG_ERROR("HACKING? : not exchange item. char_index[%d] itemDBIndex[%d]",
				  ch->m_index, sendItem->getDBIndex());
		ch->m_desc->Close("not exchange item");
		return;
	}

	if (sendItem->getFlag() & FLAG_ITEM_LENT)
	{
		LOG_ERROR("HACKING? : is rent item. char_index[%d] itemDBIndex[%d]",
				  ch->m_index, sendItem->getDBIndex());
		ch->m_desc->Close("is rent item");
		return;
	}

	if (sendItem->getDBIndex() == 9938)
	{
		LOG_ERROR("HACKING? : item is gem. char_index[%d] itemDBIndex[%d]",
				  ch->m_index, sendItem->getDBIndex());
		ch->m_desc->Close("item is gem");
		return;
	}

	if (sendItem->IsPet())
	{
		LOG_ERROR("HACKING? : item is pet. char_index[%d] itemDBIndex[%d]",
				  ch->m_index, sendItem->getDBIndex());
		ch->m_desc->Close("item is pet");
		return;
	}

	if (sendItem->IsAPet())
	{
		LOG_ERROR("HACKING? : item is apet. char_index[%d] itemDBIndex[%d]",
				  ch->m_index, sendItem->getDBIndex());
		ch->m_desc->Close("item is apet");
		return;
	}

	if (sendItem->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && sendItem->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
	{
		LOG_ERROR("HACKING? : item is artifactitem. char_index[%d] itemDBIndex[%d]",
			ch->m_index, sendItem->getDBIndex());
		ch->m_desc->Close("item is artifact");
		return;
	}

	if (sendItem->Count() < packet->itemCount)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeExpressSendToNickName(rmsg, ResponseClient::ERR_NOT_ENOUGHT_COUNT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	ExpressSystemItemInfo* expressItem = makeExpressSystemItemInfo(sendItem, packet->itemCount, EXPRESS_SEND_UNSEARCHABLE_STONESTATUS, ch->m_nick);
	DBManager::instance()->PushExpressSendItemToNickName(ch->m_desc, expressItem, packet->toNickName, EXPRESS_SENDER_TYPE_MARBLE);

	GAMELOG << init("MARBLE_SEND", ch) << itemlog(sendItem) << " : move count : " << packet->itemCount << " <=> " << packet->toNickName <<  end;


	// 신비한 석상의 구슬 삭제
	ch->m_inventory.decreaseItemCount(gemItem, 1);

	// 보낸 아이템 삭제
	ch->m_inventory.decreaseItemCount(sendItem, packet->itemCount);

	// 해당 아이템이 퀵슬롯에 있다면 퀵슬롯에서 삭제
	ch->RemoveItemFromQuickSlot(sendItem);
}

ExpressSystemItemInfo* makeExpressSystemItemInfo(CItem* pItem, int itemCount, int sendType, const char* sendName)
{
	ExpressSystemItemInfo* itemInfo = new ExpressSystemItemInfo;

	itemInfo->index = 0;
	itemInfo->item_index = pItem->getDBIndex();
	itemInfo->plus = pItem->getPlus();
	itemInfo->plus2 = pItem->getPlus_2();
	itemInfo->flag = pItem->getFlag();
	if (pItem->m_serial.empty())
	{
		itemInfo->serial[0] = '\0';
	}
	else
	{
		strncpy(itemInfo->serial, pItem->m_serial.c_str(), sizeof(itemInfo->serial));
	}
	itemInfo->item_count = itemCount;
	itemInfo->option_count = pItem->m_nOption;

	if (pItem->IsOriginItem())
	{
		for (int i = 0; i < MAX_ORIGIN_OPTION; ++i)
		{
			itemInfo->item_origin[i] = pItem->getOriginVar(i);
		}
	}
	else
	{
		itemInfo->option_count = pItem->m_nOption;

		for (int i = 0; i < MAX_ITEM_OPTION; ++i)
		{
			itemInfo->option_type[i] = pItem->m_option[i].m_type;
			itemInfo->option_level[i] = pItem->m_option[i].m_level;
		}
	}

	for (int i = 0; i < MAX_SOCKET_COUNT; ++i)
	{
		itemInfo->socket[i] = pItem->m_socketList.GetJewelAt(i);
	}

	itemInfo->nas = 0;
	itemInfo->send_type = sendType;
	strncpy(itemInfo->sender, sendName, sizeof(itemInfo->sender));

#ifdef DURABILITY
	itemInfo->now_durability = pItem->getNowDurability();
	itemInfo->max_durability = pItem->getMaxDurability();
#endif

	return itemInfo;
}

