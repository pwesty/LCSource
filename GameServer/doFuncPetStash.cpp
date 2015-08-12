#include "stdhdrs.h"

#include "Character.h"
#include "Server.h"
#include "doFunc.h"
#include "CmdMsg.h"
#include "../ShareLib/packetType/ptype_pet_stash.h"

void do_PetStash_List(CPC* ch, CNetMsg::SP& msg);
void do_PetStash_Take(CPC* ch, CNetMsg::SP& msg);
void do_PetStash_Keep(CPC* ch, CNetMsg::SP& msg);
void do_PetStash_Proxy(CPC* ch, CNetMsg::SP& msg);
void do_PetStash_Proxy_Cancel(CPC* ch, CNetMsg::SP& msg);
void do_PetStash_Request_Effect_Broadcast(CPC* ch, CNetMsg::SP& msg);

void do_PetStash(CPC* ch, CNetMsg::SP& msg)
{
	if (!ch || DEAD(ch) || ch->m_currentSkill != NULL || ch->m_personalshop != NULL)
		return ;

	msg->MoveFirst();

	unsigned char subtype;
	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_PET_STASH_TAKE:
		do_PetStash_Take(ch, msg);
		break;
	case MSG_PET_STASH_KEEP:
		do_PetStash_Keep(ch, msg);
		break;
	case MSG_PET_STASH_PROXY:
		do_PetStash_Proxy(ch, msg);
		break;
	case MSG_PET_STASH_PROXY_CANCEL:
		do_PetStash_Proxy_Cancel(ch, msg);
		break;
	case MSG_PET_STASH_REQUEST_EFFECT_BROADCAST:
		do_PetStash_Request_Effect_Broadcast(ch, msg);
		break;
	}
}

void do_PetStash_Take(CPC* ch, CNetMsg::SP& msg)
{
	if( ch == NULL )
		return;

	//아이템 부터 만들엉
	RequestClient::PetStashTake* p = reinterpret_cast<RequestClient::PetStashTake*>(msg->m_buf);
	CItem *item = ch->m_petStashManager.GetPetItemByVIndex(p->petItemVIndex);

	//Reponse Packet 에서는 유효성 검사만 한다.
	//Update Packet 에서는 데이터를 계산 후 패킷을 보낸다.
	//그래야지 추후에 Update 패킷을 재활용 할 수 있다.

	if(item == NULL)
	{
		LOG_ERROR("PET_STASH : Not Exist Item. character index : %d, character name : %s", ch->m_index, ch->GetName());
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashTakeMsg(rmsg, ResponseClient::ERROR_PET_STASH_KEEP_FAIL_NOT_EXIST_PET);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	if(item->IsAPet() == false && item->IsPet() == false)
	{
		LOG_ERROR("PET_STASH : Not Pet Item. character index : %d, character name : %s, petItemIndex = %d", ch->m_index, ch->GetName(), item->m_itemProto->getItemIndex());
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashTakeMsg(rmsg, ResponseClient::ERROR_PET_STASH_KEEP_FAIL_NOT_EXIST_PET);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		LOG_ERROR("PET_STASH : Inventory is Full. character index : %d, character name : %s", ch->m_index, ch->GetName());
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashTakeMsg(rmsg, ResponseClient::ERROR_PET_STASH_TAKE_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//Response Packet
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::PetStashTake* packet = reinterpret_cast<ResponseClient::PetStashTake*>(rmsg->m_buf);
		makePetStashTakeMsg(rmsg, ResponseClient::ERROR_PET_STASH_PROXY_SUCCESS);
		SEND_Q(rmsg, ch->m_desc);
	}
	//Update Packet
	ch->m_petStashManager.DelPetToStash(item);
}

void do_PetStash_Keep(CPC* ch, CNetMsg::SP& msg)
{
	if( ch == NULL )
		return;

	RequestClient::PetStashKeep* p = reinterpret_cast<RequestClient::PetStashKeep*>(msg->m_buf);
	CItem* item = ch->m_inventory.FindByVirtualIndex(p->petItemVIndex);

	//Reponse Packet 에서는 유효성 검사만 한다.
	//Update Packet 에서는 데이터를 계산 후 패킷을 보낸다.
	//그래야지 추후에 Update 패킷을 재활용 할 수 있다.

	if(item == NULL)
	{
		LOG_ERROR("PET_STASH : Invalid Item character index : %d, character name : %s", ch->m_index, ch->GetName());
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashKeepMsg(rmsg, ResponseClient::ERROR_PET_STASH_KEEP_FAIL_NOT_EXIST_PET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(ch->m_inventory.FindByVirtualIndex(item->getVIndex()) == NULL)
	{
		LOG_ERROR("PET_STASH : Not Exist PetItem. character index : %d, character name : %s, itemIndex : %d", ch->m_index, ch->GetName(), item->m_itemProto->getItemIndex());
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashKeepMsg(rmsg, ResponseClient::ERROR_PET_STASH_KEEP_FAIL_NOT_EXIST_PET);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int petIndex = 0;

	if(item->IsAPet())
	{
		CAPet* apet = ch->GetAPet(item->getPlus());
		if (apet == NULL)
		{
			LOG_ERROR("not found apet. charIndex[%d]", ch->m_index);
			return;
		}

		//착용중
		if(apet->IsMount() || apet->IsSummon() || apet->IsWearing())
		{
			LOG_ERROR("PET_STASH : PetMount or PetSummon or PetWearing. character index : %d, character name : %s, petIndex = %d", ch->m_index, ch->GetName(), apet->m_index);
			CNetMsg::SP rmsg(new CNetMsg);
			makePetStashKeepMsg(rmsg, ResponseClient::ERROR_PET_STASH_KEEP_FAIL_ALREADY_EQ_PET);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
		//봉인상태
		if( DEAD( apet ) )
		{
			LOG_ERROR("PET_STASH : Dead Pet. character index : %d, character name : %s, petIndex = %d", ch->m_index, ch->GetName(), apet->m_index);
			CNetMsg::SP rmsg(new CNetMsg);
			makePetStashKeepMsg(rmsg, ResponseClient::ERROR_PET_STASH_KEEP_FAIL_PET_SEAL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		petIndex = apet->m_index;
	}
	else if(item->IsPet())
	{
		CPet* pet = ch->GetPet(item->getPlus());
		if (pet == NULL)
		{
			LOG_ERROR("not found pet. charIndex[%d]", ch->m_index);
			return;
		}

		//착용중
		if(pet->IsMount() || pet->IsSummon() || pet->IsWearing())
		{
			LOG_ERROR("PET_STASH : PetMount or PetSummon or PetWearing. character index : %d, character name : %s, petIndex = %d", ch->m_index, ch->GetName(), pet->m_index);
			CNetMsg::SP rmsg(new CNetMsg);
			makePetStashKeepMsg(rmsg, ResponseClient::ERROR_PET_STASH_KEEP_FAIL_ALREADY_EQ_PET);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
		//봉인상태
		if(pet->GetRemainRebirthTime() > 0)
		{
			LOG_ERROR("PET_STASH : Dead Pet. character index : %d, character name : %s, petIndex = %d", ch->m_index, ch->GetName(), pet->m_index);
			CNetMsg::SP rmsg(new CNetMsg);
			makePetStashKeepMsg(rmsg, ResponseClient::ERROR_PET_STASH_KEEP_FAIL_PET_SEAL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		petIndex = pet->m_index;
	}
	else
	{
		LOG_ERROR("PET_STASH : Not Pet Item(HACK USER). character index : %d, character name : %s, Index = %d", ch->m_index, ch->GetName(), item->m_itemProto->getItemIndex());
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashKeepMsg(rmsg, ResponseClient::ERROR_PET_STASH_FAIL_ETC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( ch->m_petStashManager.getCount() >= MAX_PET_STASH_KEEP_COUNT )
	{
		LOG_ERROR("PET_STASH : PetStash is Full. character index : %d, character name : %s, petIndex = %d, petStashCount = %d", ch->m_index, ch->GetName(), petIndex, ch->m_petStashManager.getCount());
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashKeepMsg(rmsg, ResponseClient::ERROR_PET_STASH_KEEP_FAIL_FULL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (ch->m_TimerItem.isPetStash() == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashKeepMsg(rmsg, ResponseClient::ERROR_PET_STASH_KEEP_FAIL_TERMINATION_ITEM);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//Response Packet
	{
		LOG_INFO("PET_STASH : Keep Pet Item Success. character index : %d, charanter name : %s, petIndex = %d, petStashCount = %d", ch->m_index, ch->GetName(), item->getPlus(), ch->m_petStashManager.getCount());
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::PetStashKeep* packet = reinterpret_cast<ResponseClient::PetStashKeep*>(rmsg->m_buf);
		makePetStashKeepMsg(rmsg, ResponseClient::ERROR_PET_STASH_KEEP_SUCCESS);
		SEND_Q(rmsg, ch->m_desc);
	}

	//Update
	ch->m_petStashManager.AddPetToStash(item);
}

void do_PetStash_Proxy(CPC* ch, CNetMsg::SP& msg)
{
	if( ch == NULL )
		return;

	//아이템 부터 만들엉
	RequestClient::PetStashProxyReg* p = reinterpret_cast<RequestClient::PetStashProxyReg*>(msg->m_buf);
	CItem *item = ch->m_petStashManager.GetPetItemByVIndex(p->petItemVIndex);

	//Reponse Packet 에서는 유효성 검사만 한다.
	//Update Packet 에서는 데이터를 계산 후 패킷을 보낸다.
	//그래야지 추후에 Update 패킷을 재활용 할 수 있다.

	if(item == NULL)
	{
		LOG_ERROR("PET_STASH : Not Exist Item. character index : %d, character name : %s", ch->m_index, ch->GetName());
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashProxyRegMsg(rmsg, ResponseClient::ERROR_PET_STASH_KEEP_FAIL_NOT_EXIST_PET);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	if(item->IsAPet() == false && item->IsPet() == false)
	{
		LOG_ERROR("PET_STASH : Not Pet Item. character index : %d, character name : %s, petItemIndex = %d", ch->m_index, ch->GetName(), item->m_itemProto->getItemIndex());
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashProxyRegMsg(rmsg, ResponseClient::ERROR_PET_STASH_KEEP_FAIL_NOT_EXIST_PET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	//Response Packet
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::PetStashProxyReg* packet = reinterpret_cast<ResponseClient::PetStashProxyReg*>(rmsg->m_buf);
		makePetStashProxyRegMsg(rmsg, ResponseClient::ERROR_PET_STASH_PROXY_SUCCESS);
		SEND_Q(rmsg, ch->m_desc);
	}
	//Update Packet
	ch->m_petStashManager.SelProxyPet(item);
}

void do_PetStash_Proxy_Cancel(CPC* ch, CNetMsg::SP& msg)
{
	if (ch == NULL)
		return ;

	RequestClient::PetStashProxyCancel* p = reinterpret_cast<RequestClient::PetStashProxyCancel*>(msg->m_buf);
	CItem* item = ch->m_petStashManager.GetPetItemByVIndex(p->petItemVIndex);

	if(item == NULL)
	{
		LOG_ERROR("PET_STASH : Not Exist Item. character index : %d, character name : %s", ch->m_index, ch->GetName());
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashProxyCancelMsg(rmsg, ResponseClient::ERROR_PET_STASH_TAKE_FAIL_NOT_EXIST_PET);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(item->getVIndex() != ch->m_petStashManager.getProxyPetItemvIndex())
	{
		LOG_ERROR("PET_STASH : Not Choice ProxyPet(HACK USER). character index : %d, character name : %s, curProxyPetIndex : %d, reqPetIndex : %d", ch->m_index, ch->GetName(), ch->m_petStashManager.getProxyPetItemvIndex(), item->getPlus());
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashProxyCancelMsg(rmsg, ResponseClient::ERROR_PET_STASH_PROXY_CANCEL_NOT_EXIST);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//Response Packet
	{
		LOG_INFO("PET_STASH : Proxy Cancel Success. character index : %d, character name : %s, cancelPetIndex : %d", ch->m_index, ch->GetName(), item->getPlus());
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashProxyCancelMsg(rmsg, ResponseClient::ERROR_PET_STASH_PROXY_CANCEL_SUCCESS);
		SEND_Q(rmsg, ch->m_desc);
	}
	//Update Packet
	ch->m_petStashManager.CancelProxyPet(item);
}

void do_PetStash_Request_Effect_Broadcast(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::PetStashRequestEffectBroadcast* packet = reinterpret_cast<RequestClient::PetStashRequestEffectBroadcast*>(msg->m_buf);
	if (packet->effectNo != ch->m_petStashManager.getEffect())
	{
		LOG_ERROR("HACKING? : invalid effect no[%d]. charIndex[%d]", packet->effectNo, ch->m_index);
		ch->m_desc->Close("invalid effect no");
		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	makePetStashEffectBrMsg(rmsg, ch->m_petStashManager.getEffect(), ch->m_index);
	ch->m_pArea->SendToCell(rmsg, ch, true);
}
