// 장비 내구도
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"

#ifdef DURABILITY
#include "../ShareLib/packetType/ptype_durability.h"
#endif

#ifdef DURABILITY

void process_durability_normal_repair(CPC* ch, CNetMsg::SP& msg);
void process_durability_special_repair(CPC* ch, CNetMsg::SP& msg);
void process_durability_recovery(CPC* ch, CNetMsg::SP& msg);


void do_Durability(CPC* ch, CNetMsg::SP& msg)
{
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);

	switch (pBase->subType)
	{
	case MSG_SUB_DURABILITY_NORMAL_REPAIR:
		process_durability_normal_repair(ch, msg);
		break;

	case MSG_SUB_DURABILITY_SPECIAL_REPAIR:
		process_durability_special_repair(ch, msg);
		break;

	case MSG_SUB_DURABILITY_RECOVERY:
		process_durability_recovery(ch, msg);
		break;

	default:
		{
			LOG_ERROR("Durability : invalid packet. subtype[%d]", pBase->subType);
			ch->m_desc->Close("Durability : invalid packet");
			break;
		}
	}
}

static CItem* pre_check_for_item(CPC* ch, int tab_no, int invenIndex, int virtualIndex, const char *funcname, const int lineno)
{
	if (ch->m_inventory.isValidNormalInventory(tab_no, invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d] / %s : %d",
				  ch->m_index, tab_no, invenIndex, funcname, lineno);
		ch->m_desc->Close("invalid packet");
		return NULL;
	}

	CItem* item = ch->m_inventory.getItem(tab_no, invenIndex);

	if (item == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d] / %s : %d",
				  ch->m_index, tab_no, invenIndex, funcname, lineno);
		ch->m_desc->Close("not found item");
		return NULL;
	}

	if (item->getVIndex() != virtualIndex)
	{
		LOG_ERROR("HACKING? : invalid virtualIndex. char_index[%d] tab[%d] invenIndex[%d] / %s : %d",
				  ch->m_index, tab_no, invenIndex, funcname, lineno);
		ch->m_desc->Close("invalid virtualIndex");
		return NULL;
	}

	return item;
}

//////////////////////////////////////////////////////////////////////////
// 일반 수리
void process_durability_normal_repair(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::repairDurability* packet = reinterpret_cast<RequestClient::repairDurability*>(msg->m_buf);

	CItem* weapon = pre_check_for_item(ch, packet->tab_no, packet->invenIndex, packet->virualIndex, __FUNCTION__, __LINE__);
	if (weapon == NULL)
		return;

	if ((weapon->m_itemProto->getItemFlag() & ITEM_FLAG_DURABILITY) == 0)
	{
		LOG_ERROR("HACKING? : not durability item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_no, packet->invenIndex);
		ch->m_desc->Close("not durability item");
		return;
	}

	if (weapon->getNowDurability() == weapon->getMaxDurability())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeRepairDurability(rmsg, ERR_NO_DURABILITY_DO_NOT_REPAIR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 수수료 (클라이언트는 별도 계산)
	LONGLONG dmax = weapon->getMaxDurability();
	LONGLONG dnow = weapon->getNowDurability();
	LONGLONG fee =  ((dmax - dnow) * 19991) / 10;

	if (fee != packet->fee)
	{
		std::string tstr = boost::str(boost::format(
										  "HACKING? : not equal fee. server fee[%1%] - client fee[%2%]. char_index[%3%]")
									  % fee % packet->fee % ch->m_index);
		LOG_ERROR(tstr.c_str());
		ch->m_desc->Close("not equal fee");
		return;
	}

	if (ch->m_inventory.getMoney() < fee)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeRepairDurability(rmsg, ERR_NO_DURABILITY_NOT_ENOUGH_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 수리하자
	{
		weapon->setMaxDurability(weapon->getMaxDurability() - 10);
		if (weapon->getMaxDurability() < 0)
			weapon->setMaxDurability(0);

		weapon->setNowDurability(weapon->getMaxDurability());

		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeItemDurabilityForInventory(rmsg, weapon->tab(), weapon->getInvenIndex(),
				weapon->getNowDurability(), weapon->getMaxDurability());
		SEND_Q(rmsg, ch->m_desc);
	}

	// 수수료 차감
	ch->m_inventory.decreaseMoney(fee);

	// 결과 알려주기
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeRepairDurability(rmsg, ERR_NO_DURABILITY_OK);
		SEND_Q(rmsg, ch->m_desc);
	}
}

// 특수 수리
void process_durability_special_repair(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::specialRepairDurability* packet = reinterpret_cast<RequestClient::specialRepairDurability*>(msg->m_buf);

	CItem* weapon = pre_check_for_item(ch, packet->tab_no, packet->invenIndex, packet->virualIndex, __FUNCTION__, __LINE__);
	if (weapon == NULL)
		return;

	if ((weapon->m_itemProto->getItemFlag() & ITEM_FLAG_DURABILITY) == 0)
	{
		LOG_ERROR("HACKING? : not durability item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_no, packet->invenIndex);
		ch->m_desc->Close("not durability item");
		return;
	}

	if (weapon->getNowDurability() == weapon->getMaxDurability())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSpecialRepairDurability(rmsg, ERR_NO_DURABILITY_DO_NOT_REPAIR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItem* material = pre_check_for_item(ch, packet->material_tab_no, packet->material_invenIndex, packet->material_virtualIndex, __FUNCTION__, __LINE__);
	if (material == NULL)
		return;

	if (material->getDBIndex() != 10127)
	{
		LOG_ERROR("HACKING? : invalid item index. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_no, packet->invenIndex);
		ch->m_desc->Close("invalid item index");
		return;
	}

	// 수리하자
	{
		weapon->setNowDurability(weapon->getMaxDurability());

		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeItemDurabilityForInventory(rmsg, weapon->tab(), weapon->getInvenIndex(),
				weapon->getNowDurability(), weapon->getMaxDurability());
		SEND_Q(rmsg, ch->m_desc);
	}

	// 신비한 숫돌 차감
	ch->m_inventory.decreaseItemCount(material, 1);

	// 결과 알려주기
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSpecialRepairDurability(rmsg, ERR_NO_DURABILITY_OK);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void process_durability_recovery(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::recoveryDurability* packet = reinterpret_cast<RequestClient::recoveryDurability*>(msg->m_buf);

	CItem* weapon = pre_check_for_item(ch, packet->tab_no, packet->invenIndex, packet->virualIndex, __FUNCTION__, __LINE__);
	if (weapon == NULL)
		return;

	if ((weapon->m_itemProto->getItemFlag() & ITEM_FLAG_DURABILITY) == 0)
	{
		LOG_ERROR("HACKING? : not durability item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab_no, packet->invenIndex);
		ch->m_desc->Close("not durability item");
		return;
	}

	if (weapon->getMaxDurability() == weapon->m_itemProto->getDurability())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeRecoveryDurability(rmsg, ERR_NO_DURABILITY_DO_NOT_RECOVERY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItem* material = pre_check_for_item(ch, packet->material_tab_no, packet->material_invenIndex, packet->material_virtualIndex, __FUNCTION__, __LINE__);
	if (material == NULL)
		return;

	// 수량 체크
	if (packet->material_count <= 0)
	{
		LOG_ERROR("HACKING? : invalid material item count. char_index[%d] count[%d]",
				  ch->m_index, packet->material_count);
		ch->m_desc->Close("invalid material item count");
		return;
	}

	if (material->getItemCount() < packet->material_count)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeRecoveryDurability(rmsg, ERR_NO_DURABILITY_NOT_ENOUGH_ITEM_COUNT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if ((weapon->getMaxDurability() + (packet->material_count * 10)) > weapon->m_itemProto->getDurability())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeRecoveryDurability(rmsg, ERR_NO_DURABILITY_TOO_MUSH_ITEM_COUNT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 복구하자
	{
		weapon->setMaxDurability(weapon->getMaxDurability() + (packet->material_count * 10));
		if (weapon->getMaxDurability() > weapon->m_itemProto->getDurability())
			weapon->setMaxDurability(weapon->m_itemProto->getDurability());

		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeItemDurabilityForInventory(rmsg, weapon->tab(), weapon->getInvenIndex(),
				weapon->getNowDurability(), weapon->getMaxDurability());
		SEND_Q(rmsg, ch->m_desc);

	}

	// 신비한 망치 감소
	ch->m_inventory.decreaseItemCount(material, packet->material_count);

	// 결과 알려주기
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeRecoveryDurability(rmsg, ERR_NO_DURABILITY_OK);
		SEND_Q(rmsg, ch->m_desc);
	}
}

#endif
