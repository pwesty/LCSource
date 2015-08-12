#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/LCString.h"
#include "Character.h"
#include "Item.h"
#include "Descriptor.h"
#include "Server.h"
#include "DBManager.h"
#include "CmdMsg.h"
#include "TimerItemManager.h"
#include "Exp.h"
#include "PetStashManager.h"
#include "../ShareLib/packetType/ptype_pet_stash.h"

CPetStashManager::CPetStashManager(CPC* pc)
	:_owner(pc), m_count(0)
{
	proxyPetItemVIndex = -1;
	m_effectNo = -1;
}


CPetStashManager::~CPetStashManager(void)
{
}

void CPetStashManager::AddPetToStash( CItem* item )
{
	CPet* pet;
	CAPet* apet;

	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::PetStashKeep* packet = reinterpret_cast<UpdateClient::PetStashKeep*>(rmsg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_KEEP_UPDATE;

	if(this->m_count >= MAX_PET_STASH_KEEP_COUNT)
	{
		LOG_ERROR("HACKING? : Pet Stash Keep Count Over. charIndex[%d], count[%d]", _owner->m_index, this->m_count);
		_owner->m_desc->Close("invalid Count");
		return;
	}

	if(item->IsAPet())
	{
		apet = _owner->GetAPet(item->getPlus());

		if(apet == NULL)
		{
			LOG_ERROR("NOT FOUND APET DATA. DELETE ITEM INDEX[%d] FROM CHARACTER INDEX[%d]", item->getDBIndex(), _owner->m_index);
			_owner->m_inventory.deleteItemByItem(item);
			return ;
		}

		apet->resetStashTimer();

		packet->petStashData.virtualIndex = item->getVIndex();
		packet->petStashData.dbIndex = item->getDBIndex();
		packet->petStashData.plus = item->getPlus();
		packet->petStashData.isPetOrApet = APET;
		packet->petStashData.petLevel = apet->m_level;
		packet->petStashData.petHungryMax = apet->m_pProto->m_nMaxStm;
		packet->petStashData.petHungry = apet->GetStamina();
		packet->petStashData.petLevelupExp = apet->GetNeedExp();
		packet->petStashData.petExp = apet->m_exp;
		packet->petStashData.petFaithMax = apet->m_pProto->m_nMaxFaith;
		packet->petStashData.petFaith = apet->GetFaith();
		packet->petStashData.petStr = apet->m_str;
		packet->petStashData.petInt = apet->m_int;
		packet->petStashData.petDex = apet->m_dex;
		packet->petStashData.petCon = apet->m_con;
		packet->petStashData.petPlusStr = apet->m_nPlusStr;
		packet->petStashData.petPlusInt = apet->m_nPlusInt;
		packet->petStashData.petPlusDex = apet->m_nPlusDex;
		packet->petStashData.petPlusCon = apet->m_nPlusCon;
		packet->petStashData.petAccExp = apet->GetAccExp();
		packet->petStashData.remainTime = apet->GetCooltime();
		std::string name;
		name = (const char*)apet->m_name;
		strcpy(packet->petStashData.petName, name.c_str());
	}
	else if(item->IsPet())
	{
		pet = _owner->GetPet(item->getPlus());
		pet->resetStashTimer();

		packet->petStashData.virtualIndex = item->getVIndex();
		packet->petStashData.dbIndex = item->getDBIndex();
		packet->petStashData.plus = item->getPlus();
		packet->petStashData.isPetOrApet = PET;
		packet->petStashData.petLevel = pet->m_level;
		packet->petStashData.petHungryMax = PET_MAX_HUNGRY;
		packet->petStashData.petHungry = pet->GetHungryPoint();
		packet->petStashData.petLevelupExp = pet->GetNeedExp();
		packet->petStashData.petExp = pet->m_exp;
		packet->petStashData.petFaithMax = PET_MAX_SYMPATHY;
		packet->petStashData.petFaith = pet->GetSympathyPoint();
		packet->petStashData.hp = pet->m_hp;
		packet->petStashData.maxHp = pet->m_maxHP;
		packet->petStashData.abilityPoint = pet->GetAbilityPoint();
		packet->petStashData.grade = pet->GetPetTypeGrade();
		std::string name;
		name = (const char*)pet->m_name;
		strcpy(packet->petStashData.petName, name.c_str());
	}
	rmsg->setSize(sizeof(UpdateClient::PetStashKeep));
	SEND_Q(rmsg, _owner->m_desc);

	this->m_petItem.push_back(item);
	_owner->m_inventory.eraseNotFree(item);
	m_count++;
}

void CPetStashManager::DelPetToStash( CItem* petItem )
{
	//아이템 인벤에 넣고
	_owner->m_inventory.addItem(petItem);
	//창고에서 아이템 지우고
	this->DelPetItem(petItem);

	LOG_INFO("PET_STASH : Take a pet Success. character index : %d, character name = %s", _owner->m_index, _owner->GetName());
	CNetMsg::SP rmsg(new CNetMsg);
	makePetStashTakeUpdateMsg(rmsg, petItem->getVIndex());
	SEND_Q(rmsg, _owner->m_desc);

	return;
}

void CPetStashManager::DelPetItem( CItem* petItem )
{
	std::vector<CItem*>::iterator it = this->m_petItem.begin();
	std::vector<CItem*>::iterator it_end = this->m_petItem.end();

	for(; it != it_end; it++)
	{
		if((*it)->getVIndex() == petItem->getVIndex())
		{
			if(proxyPetItemVIndex == petItem->getVIndex())
			{
				proxyPetItemVIndex = -1;
			}
			this->m_petItem.erase(it);
			this->m_count--;
			if(this->m_count < 0)
				this->m_count = 0;
			return ;
		}
	}
}

void CPetStashManager::SendPetStashList()
{
	CAPet* apet;
	CPet* pet;

	CNetMsg::SP rmsg(new CNetMsg);

	UpdateClient::PetStashList *packet = reinterpret_cast<UpdateClient::PetStashList*>(rmsg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_LIST_UPDATE;

	packet->proxyPetItemVIndex = this->proxyPetItemVIndex;
	packet->count = this->m_petItem.size();

	std::vector<CItem*>::iterator it = this->m_petItem.begin();
	std::vector<CItem*>::iterator it_end = this->m_petItem.end();
	for(int i=0; it != it_end; it++, i++)
	{
		packet->petStashData[i].virtualIndex = (*it)->getVIndex();
		packet->petStashData[i].dbIndex = (*it)->getDBIndex();
		packet->petStashData[i].plus = (*it)->getPlus();
		if((*it)->IsAPet())
		{
			apet = _owner->GetAPet((*it)->getPlus());
			if(apet == NULL)
				continue;
			packet->petStashData[i].isPetOrApet = APET;
			packet->petStashData[i].petLevel = apet->m_level;
			packet->petStashData[i].petHungryMax = apet->m_pProto->m_nMaxStm;
			packet->petStashData[i].petHungry = apet->GetStamina();
			packet->petStashData[i].petLevelupExp = apet->GetNeedExp();
			packet->petStashData[i].petExp = apet->m_exp;
			packet->petStashData[i].petFaithMax = apet->m_pProto->m_nMaxFaith;
			packet->petStashData[i].petFaith = apet->GetFaith();
			packet->petStashData[i].petStr = apet->m_str;
			packet->petStashData[i].petInt = apet->m_int;
			packet->petStashData[i].petDex = apet->m_dex;
			packet->petStashData[i].petCon = apet->m_con;
			packet->petStashData[i].petAccExp = apet->GetAccExp();
			packet->petStashData[i].remainTime = apet->GetCooltime();
			packet->petStashData[i].petPlusStr = apet->m_nPlusStr;
			packet->petStashData[i].petPlusInt = apet->m_nPlusInt;
			packet->petStashData[i].petPlusDex = apet->m_nPlusDex;
			packet->petStashData[i].petPlusCon = apet->m_nPlusCon;
			std::string name;
			name = (const char*)apet->m_name;
			strcpy(packet->petStashData[i].petName, name.c_str());
		}
		else if((*it)->IsPet())
		{
			pet = _owner->GetPet((*it)->getPlus());
			if(pet == NULL)
				continue;
			packet->petStashData[i].isPetOrApet = PET;
			packet->petStashData[i].petLevel = pet->m_level;
			packet->petStashData[i].petHungryMax = PET_MAX_HUNGRY;
			packet->petStashData[i].petHungry = pet->GetHungryPoint();
			packet->petStashData[i].petLevelupExp = pet->GetNeedExp();
			packet->petStashData[i].petExp = pet->m_exp;
			packet->petStashData[i].petFaithMax = PET_MAX_SYMPATHY;
			packet->petStashData[i].petFaith = pet->GetSympathyPoint();
			packet->petStashData[i].hp = pet->m_hp;
			packet->petStashData[i].maxHp = pet->m_maxHP;
			packet->petStashData[i].abilityPoint = pet->GetAbilityPoint();
			packet->petStashData[i].grade = pet->GetPetTypeGrade();
			std::string name;
			name = (const char*)pet->m_name;
			strcpy(packet->petStashData[i].petName, name.c_str());
		}
	}

	rmsg->setSize(sizeof(UpdateClient::PetStashList) + sizeof(UpdateClient::PetStashData) * this->m_petItem.size());
	SEND_Q(rmsg, _owner->m_desc);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashEffectMsgByInmap(rmsg, this->getEffect());
		SEND_Q(rmsg, _owner->m_desc);
	}
}

bool CPetStashManager::Insert(std::vector<CItem*> &vec)
{
	//쿼리 날려서 테이블에서 데이터 읽어온다음에 리스트에 저장해준다.
	std::vector<CItem*>::iterator it = vec.begin();
	std::vector<CItem*>::iterator it_end = vec.end();

	for(; it != it_end; it++)
	{
		m_petItem.push_back(*it);
	}
	return true;
}

void CPetStashManager::SelProxyPet( CItem* petItem )
{
	if(petItem->IsAPet())
	{
		proxyPetItemVIndex = petItem->getVIndex();
		proxyPetItemPlus = petItem->getPlus();
		LOG_INFO("PET_STASH : Choice ProxyAPet Success. character index : %d, character name : %s, petIndex : %d", _owner->m_index, _owner->GetName(), petItem->getPlus());

	}
	else if(petItem->IsPet())
	{
		proxyPetItemVIndex = petItem->getVIndex();
		proxyPetItemPlus = petItem->getPlus();
		LOG_INFO("PET_STASH : Choice ProxyPet Success. character index : %d, character name : %s, petIndex : %d", _owner->m_index, _owner->GetName(), petItem->getPlus());
	}

	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::PetStashTake* packet = reinterpret_cast<UpdateClient::PetStashTake*>(rmsg->m_buf);
	makePetStahsProxyRegUpdateMsg(rmsg, petItem->getVIndex());
	SEND_Q(rmsg, _owner->m_desc);
}

CItem* CPetStashManager::GetPetItemByVIndex( int index )
{
	std::vector<CItem*>::iterator it = this->m_petItem.begin();
	std::vector<CItem*>::iterator it_end = this->m_petItem.end();
	for(; it != it_end; it++)
	{
		if((*it)->getVIndex() == index)
		{
			return (*it);
		}
	}
	return NULL;
}

CItem* CPetStashManager::GetPetItemByPlus( int plus )
{
	std::vector<CItem*>::iterator it = this->m_petItem.begin();
	std::vector<CItem*>::iterator it_end = this->m_petItem.end();
	for(; it != it_end; it++)
	{
		if((*it)->getPlus() == plus)
		{
			return (*it);
		}
	}
	return NULL;
}

void CPetStashManager::GetDataToQuery( std::vector<std::string>& vec )
{
	std::vector<CItem*>::iterator it = this->m_petItem.begin();
	std::vector<CItem*>::iterator it_end = this->m_petItem.end();

	bool IsProxyPet;

	for(; it != it_end; it++)
	{
		if(_owner->m_petStashManager.proxyPetItemVIndex == (*it)->getVIndex())
		{
			IsProxyPet = true;
		}
		else
		{
			IsProxyPet = false;
		}
		std::string str = boost::str(boost::format(
										 "INSERT into t_pet_stash (a_char_idx, a_item_idx, a_plus, a_proxy) values (%1%, %2%, %3%, %4%)")
									 % _owner->m_index % (*it)->getDBIndex() % (*it)->getPlus() % IsProxyPet);
		vec.push_back(str);
	}
}

CAPet* CPetStashManager::getProxyAPet()
{
	return _owner->GetAPet(this->proxyPetItemPlus);
}

CPet* CPetStashManager::getProxyPet()
{
	return _owner->GetPet(this->proxyPetItemPlus);
}

void CPetStashManager::CancelProxyPet( CItem* petItem )
{
	//요청 보낸 아이템이 현재 대리육성 펫으로 등록되어 있지 않다
	CNetMsg::SP rmsg(new CNetMsg);
	makePetStashProxyCancelUpdateMsg(rmsg, petItem->getPlus());
	SEND_Q(rmsg, _owner->m_desc);

	proxyPetItemVIndex = -1;
	proxyPetItemPlus = -1;
}

void CPetStashManager::ExpireTime()
{
	if (proxyPetItemVIndex != -1)
	{
		CItem* item = this->GetPetItemByVIndex(proxyPetItemVIndex);
		if (item)
		{
			this->CancelProxyPet(item);
		}

	}

	{
		this->setEffect(-1);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			makePetStashEffectMsgByInmap(rmsg, this->getEffect());
			SEND_Q(rmsg, _owner->m_desc);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			makePetStashEffectBrMsg(rmsg, this->_owner->m_petStashManager.getEffect(), this->_owner->m_index);
			_owner->m_pArea->SendToCell(rmsg, this->_owner, true);
		}
	}
}

void CPetStashManager::ApplyEffect()
{
	int rand = GetRandom(0, MAX_EFFECT_COUNT);
	this->_owner->m_petStashManager.setEffect(rand);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		makePetStashEffectMsg(rmsg, rand);
		SEND_Q(rmsg, _owner->m_desc);
	}
}

bool CPetStashManager::FindApet( CAPet* apet )
{
	std::vector<CItem*>::iterator it = m_petItem.begin();
	std::vector<CItem*>::iterator it_end = m_petItem.end();

	for(; it != it_end; it++)
	{
		if((*it)->getPlus() == apet->m_index)
		{
			return true;
		}
	}
	return false;
}

bool CPetStashManager::FindPet( CPet* pet )
{
	std::vector<CItem*>::iterator it = m_petItem.begin();
	std::vector<CItem*>::iterator it_end = m_petItem.end();

	for(; it != it_end; it++)
	{
		if((*it)->getPlus() == pet->m_index)
		{
			return true;
		}
	}
	return false;
}

void CPetStashManager::UpdatePetData( int petIndex )
{
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::PetStashPetData *packet = reinterpret_cast<UpdateClient::PetStashPetData*>(rmsg->m_buf);

	CItem* petItem = _owner->m_petStashManager.GetPetItemByPlus(petIndex);

	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_PET_UPDATE;
	packet->petStashData.virtualIndex = petItem->getVIndex();
	packet->petStashData.dbIndex = petItem->getDBIndex();
	packet->petStashData.plus = petItem->getPlus();

	if(petItem->IsAPet())
	{
		//pet packet 생성 후 전달
		CAPet* apet = _owner->GetAPet(petItem->getPlus());

		if(apet == NULL)
			return;

		packet->petStashData.isPetOrApet = APET;
		packet->petStashData.petLevel = apet->m_level;
		packet->petStashData.petHungryMax = apet->m_pProto->m_nMaxStm;
		packet->petStashData.petHungry = apet->GetStamina();
		packet->petStashData.petLevelupExp = apet->GetNeedExp();
		packet->petStashData.petExp = apet->m_exp;
		packet->petStashData.petFaithMax = apet->m_pProto->m_nMaxFaith;
		packet->petStashData.petFaith = apet->GetFaith();
		packet->petStashData.petStr = apet->m_str;
		packet->petStashData.petInt = apet->m_int;
		packet->petStashData.petDex = apet->m_dex;
		packet->petStashData.petCon = apet->m_con;
		packet->petStashData.petAccExp = apet->GetAccExp();
		packet->petStashData.remainTime = apet->GetCooltime();
		packet->petStashData.petPlusStr = apet->m_nPlusStr;
		packet->petStashData.petPlusInt = apet->m_nPlusInt;
		packet->petStashData.petPlusDex = apet->m_nPlusDex;
		packet->petStashData.petPlusCon = apet->m_nPlusCon;
		std::string name;
		name = (const char*)apet->m_name;
		strcpy(packet->petStashData.petName, name.c_str());

	}
	else if(petItem->IsPet())
	{
		//apet packet 생성 후 전달
		CPet* pet = _owner->GetPet(petItem->getPlus());

		if(pet == NULL)
			return;

		packet->petStashData.isPetOrApet = PET;
		packet->petStashData.petLevel = pet->m_level;
		packet->petStashData.petHungryMax = PET_MAX_HUNGRY;
		packet->petStashData.petHungry = pet->GetHungryPoint();
		packet->petStashData.petLevelupExp = pet->GetNeedExp();
		packet->petStashData.petExp = pet->m_exp;
		packet->petStashData.petFaithMax = PET_MAX_SYMPATHY;
		packet->petStashData.petFaith = pet->GetSympathyPoint();
		packet->petStashData.hp = pet->m_hp;
		packet->petStashData.maxHp = pet->m_maxHP;
		packet->petStashData.abilityPoint = pet->GetAbilityPoint();
		packet->petStashData.grade = pet->GetPetTypeGrade();
		std::string name;
		name = (const char*)pet->m_name;
		strcpy(packet->petStashData.petName, name.c_str());
	}

	rmsg->setSize(sizeof(UpdateClient::PetStashPetData));
	SEND_Q(rmsg, _owner->m_desc)
}
