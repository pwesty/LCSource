#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "../ShareLib/packetType/ptype_old_do_pk.h"

void do_PKRecoverItemSealed(CPC* ch, CNetMsg::SP& msg);

void do_PK(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;

	msg->MoveFirst();

	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_PK_RECOVER_ITEMSEAL:
		do_PKRecoverItemSealed(ch, msg);
		break;
	}
}

void do_PKRecoverItemSealed(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doPKRecoverItemSealed* packet = reinterpret_cast<RequestClient::doPKRecoverItemSealed*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	CItem* item = ch->m_inventory.getItem(packet->tab, packet->invenIndex);
	if (item == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("not found item");
		return;
	}

	if (item->getVIndex() != packet->virtualIndex || item->IsAPet() == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PKErrorMsg(rmsg, 3);
		SEND_Q(rmsg, ch->m_desc);

		return;
	}

	if ((item->getFlag() & FLAG_ITEM_SEALED) == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PKErrorMsg(rmsg, 3);
		SEND_Q(rmsg, ch->m_desc);

		return;
	}

	// µ· °Ë»ç
	LONGLONG needmoney  = (LONGLONG)100 * item->GetItemLevel() * item->GetItemLevel();

	switch (item->m_itemProto->getItemTypeIdx())
	{
	case ITYPE_WEAPON:
		needmoney *= 2;
		break;
	case ITYPE_WEAR:
		switch (item->m_itemProto->getItemSubTypeIdx())
		{
		case IWEAR_HELMET:
		case IWEAR_GLOVE:
		case IWEAR_SHOES:
			break;
		case IWEAR_ARMOR:
		case IWEAR_PANTS:
			needmoney = needmoney * 3 / 2;
			break;
		case IWEAR_SHIELD:
			needmoney *= 2;
		}
		break;
	}

	if (ch->m_inventory.getMoney() < needmoney)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PKErrorMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	ch->m_inventory.decreaseMoney(needmoney);

	item->setFlag(item->getFlag() & ~FLAG_ITEM_SEALED);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeUpdateItemFlag(rmsg, item->tab(), item->getInvenIndex(), item->getFlag());
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PKRecoverItemSealMsg(rmsg, item);
		SEND_Q(rmsg, ch->m_desc);
	}

	ch->CalcStatus(true);
}
