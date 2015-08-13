#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "ItemCollection.h"

#include "../ShareLib/packetType/ptype_item_collection.h"

struct st_need_item
{
	CItem* item;
	int need_num;
};

void do_ItemCollection_ItemUpdate(CPC* pc, CNetMsg::SP& msg);
void do_ItemCollection_Result(CPC* pc, CNetMsg::SP& msg);

void do_ItemCollection(CPC* pc, CNetMsg::SP& msg)
{
	if(pc == NULL)
		return;

	msg->MoveFirst();

	unsigned char subtype;
	RefMsg(msg) >> subtype;

	switch(subtype)
	{
	case MSG_SUB_ITEM_COLLECTION_ITEM_UPDATE_REQ:
		do_ItemCollection_ItemUpdate(pc, msg);
		break;
	case MSG_SUB_ITEM_COLLECTION_RESULT_REQ:
		do_ItemCollection_Result(pc, msg);
		break;
	}
}

void do_ItemCollection_ItemUpdate(CPC* pc, CNetMsg::SP& msg)
{
	RequestClient::ItemCollection_GiveItem* packet = reinterpret_cast<RequestClient::ItemCollection_GiveItem*>(msg->m_buf);
	
	//패킷 유효성 검사
	ST_COLLECTION_DATA* data = ItemCollectionData::instance()->findCollectionData(packet->category_index, packet->theme_index);
	if(data == NULL)
	{
		LOG_ERROR("HACKING? Invalid Collection Data. category[%d], theme[%d]", packet->category_index, packet->theme_index);
		pc->m_desc->Close("Invalid Collection Data");
		return ;
	}
	
	item_search_t vec[ITEM_COLLECTION_MAX_NEED_VALUE];
	int need_data[ITEM_COLLECTION_MAX_NEED_VALUE] = {0,};
	
	for(int i = 0; i < ITEM_COLLECTION_MAX_NEED_VALUE; i++)
	{
		if(packet->data[i].need_index == -1)
			continue;

		//기부 목록의 형태를 유지하기 위해 카운트를 0으로 보낼 수 있음.
		if(packet->data[i].need_num == 0)
			continue;
		if(packet->data[i].need_num < 0)
		{
			LOG_ERROR("HACKING? Invalid Item Count. charIndex[%d], itemIndex[%d], needCount[%d]", pc->m_index, packet->data[i].need_index, packet->data[i].need_num);
			pc->m_desc->Close("Invalid Item Count");
			return ;
		}

		if(data->need_data[i].need_type != ITEM_COLLECTION_NEED_TYPE_ITEM)
			continue;

		//인벤토리에 해당 아이템이 존재하는지 검사
		int itemCount = pc->m_inventory.searchItemByDBIndex(packet->data[i].need_index, vec[i]);

		if(itemCount == 0)
		{
			LOG_ERROR("HACKING? Invalid Item Count1. charIndex[%d], itemIndex[%d]", pc->m_index, packet->data[i].need_index);
			pc->m_desc->Close("Invalid Item");
			return ;
		}

		item_search_t::iterator it = vec[i].begin();

		for( ;it != vec[i].end(); )
		{
			if( (it->pItem->getFlag() & FLAG_ITEM_COMPOSITION)
				|| (it->pItem->m_itemProto->getItemTypeIdx() == ITYPE_ETC && it->pItem->m_itemProto->getItemTypeIdx() == IETC_MONSTER_MERCENARY_CARD)
				|| (it->pItem->m_itemProto->getItemTypeIdx() == ITYPE_ETC && it->pItem->m_itemProto->getItemTypeIdx() == IETC_PET_AI))
			{
				itemCount -= it->pItem->getItemCount();
				it = vec[i].erase(it);
			}
			else
				++it;
		}

		if(itemCount <= 0)
		{
			LOG_ERROR("HACKING? Invalid Item Count2. charIndex[%d], itemIndex[%d]", pc->m_index, packet->data[i].need_index);
			pc->m_desc->Close("Invalid Item");
			return ;
		}
		
		if(itemCount < packet->data[i].need_num)
		{
			LOG_ERROR("HACKING? Invalid Item Count. charIndex[%d], itemIndex[%d], itemCount[%d], needCount[%d]", pc->m_index, packet->data[i].need_index, itemCount, packet->data[i].need_num);
			pc->m_desc->Close("Invalid Item Count");
			return ;
		}

		need_data[i] = packet->data[i].need_num;
	}

	//아이템 차감은 일괄 작업
	for(int i = 0; i < ITEM_COLLECTION_MAX_NEED_VALUE; i++)
	{
		if( need_data[i] == 0 )
			continue;

		pc->m_inventory.deleteItem(vec[i], need_data[i]);
	}

	pc->m_itemCollectionManager.updateNeedData(packet->category_index, packet->theme_index, need_data);
}

void do_ItemCollection_Result(CPC* pc, CNetMsg::SP& msg)
{
	RequestClient::ItemCollection_Result* packet = reinterpret_cast<RequestClient::ItemCollection_Result*>(msg->m_buf);
	
	//완료가능 항목인지 검사
	bool check = pc->m_itemCollectionManager.checkCanComplete(packet->category_index, packet->theme_index);
	if(check == false)
	{
		LOG_ERROR("HACKING? ItemCollection Not Complete. charIndex[%d] category[%d] theme[%d]", pc->m_index, packet->category_index, packet->theme_index);
		pc->m_desc->Close("ItemCollection Not Complete");
		return;
	}

	//인벤토리 공간 검사
	if(pc->m_inventory.getEmptyCount() <= 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeItemCollectionError(rmsg, ITEM_COLLECTION_NOT_ENOUGH_INVEN);
		SEND_Q(rmsg, pc->m_desc);
		return ;
	}

	ST_COLLECTION_DATA* data = ItemCollectionData::instance()->findCollectionData(packet->category_index, packet->theme_index);

	if(data->result_data.result_num <= 0)
	{
		LOG_ERROR("ITEM_COLLECTION_RESULT FAIL DATA ERROR.... charIndex[%d], category_index[%d], theme_index[%d], result_count[%d]", pc->m_index, packet->category_index, packet->theme_index, data->result_data.result_num);
		return;
	}

	switch(data->result_data.result_type)
	{
	case ITEMCOLLECTION_RESULT_TYPE_ITEM:
		{
			CItem* item = ItemCollectionData::instance()->getResultItem(packet->category_index, packet->theme_index);
			if(item == NULL)
			{
				LOG_ERROR("HACKING? result type is not item. charIndex[%d] category[%d] theme[%d]", pc->m_index, packet->category_index, packet->theme_index);
				pc->m_desc->Close("Item Collection Type Error");
				return;
			}

			pc->m_inventory.addItem(item);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeItemCollectionError(rmsg, ITEM_COLLECTION_ERROR_RESULT_SUCCESS);
				SEND_Q(rmsg, pc->m_desc);
			}
		}
		break;

	case ITEMCOLLECTION_RESULT_TYPE_NAS:
		pc->m_inventory.increaseMoney(data->result_data.result_num);
		break;

	case ITEMCOLLECTION_RESULT_TYPE_EXP:
		pc->m_exp += data->result_data.result_num;
		pc->m_bChangeStatus = true;

		{
			// Exp, SP 메시지 전송
			CNetMsg::SP rmsg(new CNetMsg);
			ExpSPMsg(rmsg, data->result_data.result_num, 0);
			SEND_Q(rmsg, pc->m_desc);
		}
		break;

	case ITEMCOLLECTION_RESULT_TYPE_SP:
		pc->m_skillPoint += data->result_data.result_num * 1000;
		pc->m_bChangeStatus = true;

		{
			// Exp, SP 메시지 전송
			CNetMsg::SP rmsg(new CNetMsg);
			ExpSPMsg(rmsg, 0, data->result_data.result_num);
			SEND_Q(rmsg, pc->m_desc);
		}
		break;

	default:
		LOG_ERROR("ITEM_COLLECTION_RESULT FAIL.... charIndex[%d], category_index[%d], theme_index[%d]", pc->m_index, packet->category_index, packet->theme_index);
		return;
	}

	pc->m_itemCollectionManager.complete(packet->category_index, packet->theme_index);
}
