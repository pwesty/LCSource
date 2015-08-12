#include <string>
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Character.h"
#include "Item.h"
#include "Descriptor.h"
#include "Server.h"
#include "DBManager.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WearInvenManager.h"
#include "Artifact_Manager.h"
#include "../ShareLib/packetType/ptype_old_do_skill.h"

std::set<int>	CWearInvenManager::warePos_set_;
int				CWearInvenManager::m_suitWearingPos[6];

CWearInvenManager::CWearInvenManager(CPC* pc)
	: _owner(pc)
	, _lastError(0)
	,dressCostumCount(0)
	,wearSuitItem(NULL)
{
	wearItemInfo.reserve(MAX_WEARING);
	for(int i = 0; i < MAX_WEARING; ++i)
	{
		wearItemInfo.push_back(static_cast<CItem*>(NULL));
	}
}


CWearInvenManager::~CWearInvenManager(void)
{
	for(int i = 0; i < MAX_WEARING; ++i)
	{
		if(wearItemInfo[i] != NULL)
		{
			delete wearItemInfo[i];
			wearItemInfo[i] = NULL;
		}
	}

	delete wearSuitItem;
}

bool CWearInvenManager::AddNormalItem( CItem* item, int wearPos )
{
	if (CanDressNormal(item, wearPos) == false)
		return false;

	char mountPetType = 0;
	bool bChangeMountPet = false;
	CPet* pet1 = NULL;
	CPet* pet2 = NULL;

	// 벗기고 입기
	CItem* takeOffWearItem = NULL;
	if ( wearItemInfo[wearPos] != NULL )	//장착하려는 자리에 아이템이 이미 존재한다면
	{
		takeOffWearItem = getWearItem(wearPos);		//벗길 아이템

		if (!takeOffWearItem->IsAPet() &&	takeOffWearItem->getFlag() & FLAG_ITEM_SEALED)
			return false;

		if (takeOffWearItem->IsPet())
		{
			// 착용 아이템에 해당하는 펫을 찾는다
			CPet* pet = _owner->GetPet(takeOffWearItem->getPlus());
			if (pet)
			{
				if (pet->IsMount())
				{
					bChangeMountPet = true;
					mountPetType = 0;
					pet2 = pet;
				}
				else
				{
					pet->Disappear();
				}
			}
		}
		if( takeOffWearItem->IsAPet() )
		{
			CAPet* pet = _owner->GetAPet(takeOffWearItem->getPlus());
			if( pet )
			{
				if( pet && pet->IsMount() )
					pet->Mount(false);
				pet->m_nDelay = pet->m_pProto->m_nDelay ;
				pet->Disappear();
			}
		}

		CItem *pItemNormal = takeOffWearItem;
		//결합 아이템인 경우
		if((pItemNormal->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (pItemNormal->getFlag() & FLAG_ITEM_COMPOSITION))
		{
			pItemNormal = _owner->m_inventory.FindByVirtualIndex(pItemNormal->m_nCompositeItem);
		}
		//오리진 옵션 인 경우
		if ( pItemNormal != NULL && pItemNormal->IsOriginItem() )
		{
			// 오리지날 옵션 및 세트 아이템의 스킬 삭제
			for (int i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
			{
				int nIndex = pItemNormal->GetOriginIndex(i);
				if (nIndex <= 0)
					continue;

				CSkill * pSkill = gserver->m_skillProtoList.Create(nIndex, pItemNormal->GetOriginLevel(i));
				if(pSkill)
				{
					if (pSkill->m_proto->IsActive())
					{
						//아이템 스킬 쿨타임을 저장
						{
							CSkill* skill = _owner->m_activeSkillList.Find(pSkill->m_proto->m_index);
							if(skill == NULL)
							{
								LOG_ERROR("invalid Skill Info. charIndex[%d], skillIndex[%d]", _owner->m_index, pSkill->m_proto->m_index);
								return false;
							}

							CPC::map_t::iterator it = _owner->itemSkill.find(pSkill->m_proto->m_index);
							if(it != _owner->itemSkill.end())
							{
								it->second = skill->m_usetime;
							}
							else
							{
								_owner->itemSkill.insert(std::pair<int, int>(pSkill->m_proto->m_index, skill->m_usetime));
							}
						}

						pSkill->Cancel(_owner);

						{
							CNetMsg::SP rmsg(new CNetMsg);
							ResponseClient::makeSkillCancelMsg(rmsg, _owner);
							_owner->m_pArea->SendToCell(rmsg, _owner, true);
						}

						_owner->RemoveSkillFromQuickSlot(pSkill);
						_owner->m_activeSkillList.Remove(pSkill);
					}
					else if (pSkill->m_proto->IsPassive())
					{
						_owner->m_passiveSkillList.Remove(pSkill);
					}
					else
					{
						_owner->m_etcSkillList.Remove(pSkill);
					}
				}
			}

			// 세트 아이템 착용 개수 감소 및 세트 효과 스킬 삭제
			if (pItemNormal->m_itemProto->GetSetIndex())
			{
				_owner->CountSetItem(pItemNormal, false);
				_owner->RemoveSetItemValue(pItemNormal);

				pItemNormal->m_nSetWearCnt = 0;
				pItemNormal->m_nSetOption = 0;
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeSkillListMsg(rmsg, _owner);
				SEND_Q(rmsg, _owner->m_desc);
			}
		}
		_owner->m_optionAttSkillList.Clear();
		_owner->m_optionDefSkillList.Clear();
	}

	// 양손무기와 방패를 같이 착용하지 못하게
	switch (item->m_itemProto->getItemTypeIdx())
	{
	case ITYPE_WEAPON:
		switch (item->m_itemProto->getItemSubTypeIdx())
		{
		case IWEAPON_CROSSBOW:
		case IWEAPON_STAFF:
		case IWEAPON_BIGSWORD:
		case IWEAPON_AXE:
		case IWEAPON_BOW:
		case IWEAPON_MINING:
		case IWEAPON_GATHERING:
		case IWEAPON_CHARGE:
		case IWEAPON_TWOSWORD:
		case IWEAPON_SCYTHE:
		case IWEAPON_POLEARM:
		case IWEAPON_SOUL:
			if (wearItemInfo[WEARING_SHIELD] != NULL)
			{
				CItem* pitem = wearItemInfo[WEARING_SHIELD];

				if (pitem->m_itemProto->getItemTypeIdx() == ITYPE_WEAR && pitem->m_itemProto->getItemSubTypeIdx() == IWEAR_SHIELD)
				{
					if( DelNormalItem(WEARING_SHIELD) == false)
					{
						return false;
					}
				}
			}
			break;
		}
		break;
	case ITYPE_WEAR:
		switch (item->m_itemProto->getItemSubTypeIdx())
		{
		case IWEAR_SHIELD:
			if (wearItemInfo[WEARING_WEAPON] != NULL)
			{
				CItem* pitem = wearItemInfo[WEARING_WEAPON];
				if (pitem->getFlag() & FLAG_ITEM_SEALED)
					return false;

				switch (pitem->m_itemProto->getItemTypeIdx())
				{
				case ITYPE_WEAPON:
					switch (pitem->m_itemProto->getItemSubTypeIdx())
					{
					case IWEAPON_CROSSBOW:
					case IWEAPON_STAFF:
					case IWEAPON_BIGSWORD:
					case IWEAPON_AXE:
					case IWEAPON_BOW:
					case IWEAPON_MINING:
					case IWEAPON_GATHERING:
					case IWEAPON_CHARGE:
					case IWEAPON_TWOSWORD:
					case IWEAPON_SCYTHE:
					case IWEAPON_POLEARM:
					case IWEAPON_SOUL:
						if( DelNormalItem(WEARING_WEAPON) == false)
						{
							return false;
						}
					}
					break;
				}
			}
			break;
		}
		break;

	case ITYPE_ACCESSORY:
		{
			if(item->m_itemProto->getItemIndex() == 2371
					|| item->m_itemProto->getItemIndex() == ONE_PERIOD_ITEM
					|| item->m_itemProto->getItemIndex() == SEVEN_PERIOD_ITEM
					|| item->m_itemProto->getItemIndex() == THIRTY_PERIOD_ITEM
					|| item->m_itemProto->getItemIndex() == 2610
					|| item->m_itemProto->getItemIndex() == 4940
					|| item->m_itemProto->getItemIndex() == 4941
					|| item->m_itemProto->getItemIndex() == 4942
			  )
			{
				CSkill* pSkill;
				bool bApply;
				pSkill = gserver->m_skillProtoList.Create(item->m_itemProto->getItemNum0(), item->m_itemProto->getItemNum1());
				if(pSkill)
				{
					ApplySkill(_owner, _owner, pSkill, item->m_itemProto->getItemIndex(), bApply);
					delete pSkill;
					pSkill = NULL;
				}
				GAMELOG << init("ITEM WEAR AUTO POTION") << delim << item->m_itemProto->getItemIndex() << end;
			}
		}
		break;
	}
	// --- 양손무기와 방패를 같이 착용하지 못하게

	CItem *pItemNormal = item;
	if((pItemNormal->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (pItemNormal->getFlag() & FLAG_ITEM_COMPOSITION))
	{
		pItemNormal = _owner->m_inventory.FindByVirtualIndex(item->m_nCompositeItem);
	}
	if (pItemNormal && pItemNormal->IsOriginItem())
	{
		// 귀속 설정
		if (pItemNormal->GetBelongType() != ORIGIN_OPTION_BELONG_NONE && !pItemNormal->IsBelong())
		{
			pItemNormal->setFlag(pItemNormal->getFlag() | FLAG_ITEM_BELONG);
			GAMELOG << init("ITEM WEAR BELONG", _owner) << itemlog(pItemNormal) << end;
		}

		// 착용 개수, 옵션 적용
		if (pItemNormal->m_itemProto->GetSetIndex())
		{
			_owner->CountSetItem(pItemNormal);
		}
	}

	if( pItemNormal && (pItemNormal->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (pItemNormal->getFlag() & FLAG_ITEM_COMPOSITION) )
	{
		pItemNormal = _owner->m_inventory.FindByVirtualIndex(item->m_nCompositeItem);
	}

	// 애완동물 입을 때에는 애완동물을 Appear
	if (item->IsPet())
	{
		// 착용 아이템에 해당하는 펫을 찾는다
		CPet* pet = _owner->GetPet(item->getPlus());
		if (pet)
		{
			if (pet->IsMountType())
			{
				bChangeMountPet = true;
				mountPetType = pet->GetPetTypeGrade();
				pet1 = pet;
			}
			else
			{
				pet->Appear(true);
			}
			//펫에게도 나의 결사대 타입을 복사
			pet->setSyndicateType(_owner->getSyndicateType());
		}
	}

	if (bChangeMountPet)
	{
		if (pet2)
		{
			pet2->Mount(false);
		}
		if (pet1)
		{
			pet1->Mount(true);
		}
	}

	this->DressNormal(item, wearPos);

	_owner->CalcStatus(true);

	if( item->IsAPet() )	// 공격형 애완동물 Appear
	{
		CAPet* apet = _owner->GetAPet(item->getPlus());

		if( !( item->getFlag() & FLAG_ITEM_SEALED ) && apet && DEAD(apet) )
		{
			apet->m_hp = 1;
			apet->m_mp = 1;
		}

		if( apet && !DEAD(apet) )
		{
			apet->Appear(true,true);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExAPetFuntionMsg(rmsg, MSG_SUB_SKILLLIST, apet, 0);
				SEND_Q(rmsg, apet->GetOwner()->m_desc);
			}
			{
				CNetMsg::SP rmsg(new CNetMsg);
				APetAIOnOffMsg(rmsg, apet, MSG_APET_ERROR_OK);
				SEND_Q(rmsg, apet->GetOwner()->m_desc);
			}
			{
				CNetMsg::SP rmsg(new CNetMsg);
				APetAIListMsg(rmsg, apet);
				SEND_Q(rmsg, apet->GetOwner()->m_desc);
			}

			apet->setSyndicateType(_owner->getSyndicateType());
		}
	}

	_owner->m_assist.CheckApplyConditions();

	if(_owner->GetAPet(0))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetStatusMsg(rmsg, _owner->GetAPet(0));
		_owner->m_pArea->SendToCell(rmsg, _owner, true);
		SEND_Q(rmsg, _owner->m_desc);
	}

	if (pItemNormal && (pItemNormal->GetOriginSkillCnt() > 0 || pItemNormal->m_nSetOption == 2))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillListMsg(rmsg, _owner);
		SEND_Q(rmsg, _owner->m_desc)
	}

	if(pItemNormal != NULL)
	{
		switch(pItemNormal->getDBIndex())
		{
		case ARTIFACT_LEVEL1_ITEM_INDEX:
		case ARTIFACT_LEVEL2_ITEM_INDEX:
		case ARTIFACT_LEVEL3_ITEM_INDEX:
			if(_owner->IsParty() == true)
				_owner->m_party->updateCalcMember();
			break;
		}

	}

	return true;
}

bool CWearInvenManager::AddCostumItem( CItem* item, int wearPos )
{
	//유효성 검사
	if( CanDressCostum(item, wearPos) == false )
		return false;

	//한벌 의상을 입고 있는 경우
	if( NULL != getCosWearSuitItem() )
	{
		if(DelCostumSuitItem() == false)
		{
			return false;
		}
	}
	//개별 코스튬 아이템 장착
	this->DressCostum(item, wearPos);

	return true;
}

bool CWearInvenManager::DelNormalItem( int wearPos, short tab /*= -1*/, short invenIndex /*= -1*/ )
{
	CItem* item = _owner->m_wearInventory.wearItemInfo[wearPos];

	if(item == NULL)
		return false;

	//벗을 수 있는지에 대한 유효성 검사
	if (CanUndressNormal(item, wearPos) == false)
	{
		/*CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::doItemWearError *packet = reinterpret_cast<ResponseClient::doItemWearError*>(rmsg->m_buf);
		ResponseClient::ItemTakeoffMsg(rmsg, GetLastError());
		SEND_Q(rmsg, _owner->m_desc);*/

		return false;
	}

	//벗은 아이템의 위치정보가 있는 경우
	if( tab >= 0 && invenIndex >= 0)
	{
		//드래그 앤 드랍으로 아이템을 벗는 경우에는 인벤토리에서 해당 위치의 아이템을 가져와서 비교 한 후 처리
		CItem* dressItem = _owner->m_inventory.getItem(tab, invenIndex);
		if(dressItem == NULL)
		{
			this->UndressNormal(item, wearPos);
			_owner->m_inventory.addItemToInvenIndex(item, tab, invenIndex);
		}
		else if (dressItem->m_itemProto->getItemWearing() == item->m_itemProto->getItemWearing() && dressItem->m_itemProto->getItemJobFlag() & (1 << _owner->m_job))
		{
			AddNormalItem(dressItem, dressItem->m_itemProto->getItemWearing());
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::ItemTakeoffMsg(rmsg, ResponseClient::WEAR_ERR_CANNOT_WEAR);
			SEND_Q(rmsg, _owner->m_desc);
			return false;
		}
	}
	else
	{
		if(_owner->m_inventory.getEmptyCount() <= 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::ItemTakeoffMsg(rmsg, ResponseClient::WEAR_ERR_NOT_ENOUGH_INVEN);
			SEND_Q(rmsg, _owner->m_desc);
			return false;
		}

		this->UndressNormal(item, wearPos);
		_owner->m_inventory.addItem(item);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemTakeoffMsg(rmsg, ResponseClient::WEAR_ERR_OK);
		SEND_Q(rmsg, _owner->m_desc);
	}

	//장착 해제한 아이템들에 대한 옵션 적용 빼기
	ApplyDeleteItem(item, wearPos);
	//장착 해제한 아이템들의 퀵 슬롯 데이터를 유지시켜준다. (인벤토리 정보로 세팅)
	_owner->ChangeQuickSlot(item, QUICKSLOT_TYPE_ITEM);

	switch(item->getDBIndex())
	{
	case ARTIFACT_LEVEL1_ITEM_INDEX:
	case ARTIFACT_LEVEL2_ITEM_INDEX:
	case ARTIFACT_LEVEL3_ITEM_INDEX:
		if(_owner->IsParty() == true)
			_owner->m_party->updateCalcMember();
		break;
	}

	return true;
}

bool CWearInvenManager::DelCostumItem( int wearPos, short tab /*= -1*/, short invenIndex /*= -1*/ )
{
	if (isValidWarePos(wearPos + COSTUME2_WEARING_START) == false)
	{
		LOG_ERROR("HACKING? : invalid wear pos[%d]. charIndex[%d]", wearPos + COSTUME2_WEARING_START, _owner->m_index);
		_owner->m_desc->Close("invalid wear pos");
		return false;
	}

	CItem* TakeOffItem = _owner->m_wearInventory.wearItemInfo[wearPos + COSTUME2_WEARING_START];

	//유효성 검사
	if (CanUndressCostum(TakeOffItem, wearPos) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::doItemWearError *packet = reinterpret_cast<ResponseClient::doItemWearError*>(rmsg->m_buf);
		ResponseClient::ItemTakeoffMsg(rmsg, GetLastError());
		SEND_Q(rmsg, _owner->m_desc);
		return false;
	}

	if (tab >= 0 && invenIndex >= 0)
	{
		//드래그 앤 드랍으로 아이템을 벗는 경우에는 인벤토리에서 해당 위치의 아이템을 가져와서 비교 한 후 처리
		CItem* dressItem = _owner->m_inventory.getItem(tab, invenIndex);
		if(dressItem == NULL)
		{
			this->UndressCostum(TakeOffItem, wearPos);
			_owner->m_inventory.addItemToInvenIndex(TakeOffItem, tab, invenIndex);
		}
		else if( dressItem->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2 )
		{
			return false;
		}
		else if( dressItem && (dressItem->m_itemProto->getItemWearing() == TakeOffItem->m_itemProto->getItemWearing()) && dressItem->m_itemProto->getItemJobFlag() & (1 << _owner->m_job) )
		{
			this->UndressCostum(TakeOffItem, wearPos);
			AddCostumItem(dressItem, dressItem->m_itemProto->getItemWearing());
			_owner->m_inventory.addItemToInvenIndex(TakeOffItem, tab, invenIndex);
		}
		else
		{
			this->_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	}
	else
	{
		this->UndressCostum(TakeOffItem, wearPos);
		_owner->m_inventory.addItem(TakeOffItem);
	}

	_owner->ChangeQuickSlot(TakeOffItem, QUICKSLOT_TYPE_ITEM);

	return true;
}

CItem* CWearInvenManager::getWearItem( int wearPos )
{
	if (isValidWarePos(wearPos) == false)
	{
		LOG_ERROR("HACKING? : invalid wear pos[%d]. charIndex[%d]", wearPos, _owner->m_index);
		_owner->m_desc->Close("invalid wear pos");
		return NULL;
	}

	return wearItemInfo[wearPos];
}

CItem* CWearInvenManager::getCosWearItem( int wearPos )
{
	wearPos += COSTUME2_WEARING_START;

	if (isValidWarePos(wearPos) == false)
	{
		LOG_ERROR("HACKING? : invalid wear pos[%d]. charIndex[%d]", wearPos, _owner->m_index);
		_owner->m_desc->Close("invalid wear pos");
		return NULL;
	}

	return wearItemInfo[wearPos];
}

bool CWearInvenManager::CanDressNormal( CItem* item, int wearPos )
{
	if( item == NULL )
	{
		_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
		return false;
	}
	if (isValidWarePos(wearPos) == false)
		return false;

	if (item->m_itemProto->getItemRvRType() != SYNDICATE::eSYNDICATE_NONE)
	{
		if(_owner->getSyndicateType() != item->m_itemProto->getItemRvRType())
		{
			return false;
		}
		if(_owner->m_syndicateManager.getSyndicateGrade(_owner->getSyndicateType()) < item->m_itemProto->getItemRvRGrade())
		{
			return false;
		}
	}

	if(item->m_itemProto->getItemFlag() & ITEM_FLAG_CASTLLAN)
	{
		int zonenum;
		char castellan;
		bool ret = _owner->CheckCastellanType(zonenum, castellan);

		if(item->m_itemProto->getItemNum3() & 0xffff0000)  // 성주멤버용 아이템
		{
			if(!ret || castellan)
			{
				// 성주멤버가 아니다.
				CNetMsg::SP rmsg(new CNetMsg);
				CastllanErrorMsg(rmsg, MSG_EX_CASTLLAN_ERROR_NOT_CASTLLAN_GUILD);
				SEND_Q(rmsg, _owner->m_desc);

				_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
				return false;
			}

			if(zonenum != (item->m_itemProto->getItemNum3() & 0xffff))
			{
				// 소속된 성의 아이템이 아님
				_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
				return false;
			}
		}
		else		// 성주용 아이템
		{
			if(!ret || !castellan)
			{
				// 성주멤버가 아니다.
				CNetMsg::SP rmsg(new CNetMsg);
				CastllanErrorMsg(rmsg, MSG_EX_CASTLLAN_ERROR_NOT_CASTLLAN);
				SEND_Q(rmsg, _owner->m_desc);
				_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
				return false;
			}
			if(zonenum != (item->m_itemProto->getItemNum3() & 0xffff))
			{
				// 소속된 성의 아이템이 아님
				_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
				return false;
			}
		}
	}

	//아이템 직업 검사
	if (!(item->m_itemProto->getItemJobFlag() & (1 << _owner->m_job)))
	{
		_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
		return false;
	}

	// 명성치 검사
	if (item->m_itemProto->getItemFrame() != -1)
	{
		if (_owner->m_fame < item->m_itemProto->getItemFrame())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_SHORT_FAME);
			SEND_Q(rmsg, _owner->m_desc);
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	}

	if( item->IsAPet() )	// 재사용시간이 아직 남았음
	{
		CAPet *pet = _owner->GetAPet(item->getPlus());
		if( pet && pet->m_nDelay > 0 )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExAPetFuntionMsg( rmsg, MSG_SUB_SUMMON_ERROR, pet , (unsigned char)pet->m_nDelay );
			SEND_Q( rmsg,  pet->GetOwner()->m_desc );
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	}

	// 착용 위치가 올바른지 검사
	if (item->m_itemProto->getItemWearing() != wearPos)
	{
		// 올바르지 않은 착용 위치이면 착용 아이템이 악세인지 검사
		// 악세사리는 올바르지 않은 착용 위치가 오면 서버에서 조정함
		if (item->IsAccessary())
		{
			int i;
			for (i = WEARING_ACCESSORY1; i <= WEARING_ACCESSORY3; i++)
			{
				if (_owner->m_wearInventory.wearItemInfo[i] == NULL)
					break;
			}

			if (i > WEARING_ACCESSORY3)
				wearPos = WEARING_ACCESSORY1;
			else
				wearPos = i;
		}
		else
		{
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	}

	// 입는 아이템이 결합된 아이템이면 불가능
	if (item->CanWear() == false)
	{
		_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
		return false;
	}

	// 060221 : bs : 펫 착용시 최근 죽은 시간을 검사
	if (item->IsPet())
	{
		CPet* pet = _owner->GetPet(item->getPlus());
		if (!pet)
		{
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}

		if( _owner->m_job == JOB_NIGHTSHADOW && pet->IsMountType() )
		{
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}

		if (_owner->m_pZone)
		{
			if(pet->IsMountType() == false && !_owner->m_pZone->m_bCanSummonPet)
			{
				_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
				return false;
			}

			else if(pet->IsMountType() && !_owner->m_pZone->m_bCanMountPet)
			{
				_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
				return false;
			}
		}
		else
			return true;

		if (pet->GetRemainRebirthTime() > 0)
		{
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	}

	if(_owner->m_pZone)
	{
		if(item->IsAPet() && !_owner->m_pZone->m_bCanSummonPet)
		{
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	}

	CItem* pCompositItem = NULL;
	// 레어 아이템은 착용레벨만 제한을 검사한다

	CItem* pEquip = item;
	if (item->getFlag() & FLAG_ITEM_COMPOSITION)
	{
		if (item->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE)
		{
			pEquip = _owner->m_inventory.FindByVirtualIndex(item->m_nCompositeItem);
			pCompositItem = pEquip;
		}
	}

	if (pCompositItem &&  (pCompositItem->IsRareItem() || pCompositItem->IsOriginItem()))
	{
		/** 결합 아이템의 레벨 체크
		* 래어 아이템, 오리지날 옵션 아이템일 경우에만 레벨 체크
		**/
#ifdef PREMIUM_CHAR
		int itemLevel = pCompositItem->GetItemLevelForPremiumChar(_owner->m_premiumChar.getPremiumType());
#else
		int itemLevel = pCompositItem->GetItemLevel();
#endif

		if (itemLevel > _owner->m_level)
		{
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	}

	//유물 아이템일 경우에는 레벨 체크 한다. (프리미엄 캐릭터 적용 X)
	if(item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
	{
		int itemLevel = item->GetItemLevel();

		if (itemLevel > _owner->m_level)
		{
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	}

	if (item != NULL && item->IsRareItem())
	{
#ifdef PREMIUM_CHAR
		int itemLevel = item->GetItemLevelForPremiumChar(_owner->m_premiumChar.getPremiumType());
#else
		int itemLevel = item->GetItemLevel();
#endif

		if (itemLevel > _owner->m_level)
		{
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	}

	if (item->IsOriginItem())
	{
#ifdef PREMIUM_CHAR
		int itemLevel = item->GetItemLevelForPremiumChar(_owner->m_premiumChar.getPremiumType());
#else
		int itemLevel = item->GetItemLevel();
#endif

		if (itemLevel > _owner->m_level)
		{
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	}

	switch(item->m_itemProto->getItemIndex())
	{
	case 2669:	// 타이탄 목걸이
	case 2670:	// 나이트 목걸이
	case 2671:	// 힐러 목걸이
	case 2672:	// 메이지 목걸이
	case 2673:	// 로그 목걸이
	case 2674:	// 소서러 목걸이

		if(_owner->m_level > 45)		// 45레벨 초과시 사용금지
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
			SEND_Q(rmsg, _owner->m_desc);
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	default:
		break;
	}

	if ( (item->tab() == INVENTORY_TAB_CASH_1 && _owner->m_inventory.isUsable(INVENTORY_TAB_CASH_1) == false) ||
			(item->tab() == INVENTORY_TAB_CASH_2 && _owner->m_inventory.isUsable(INVENTORY_TAB_CASH_2) == false) )
	{
		_lastError = ResponseClient::WEAR_ERR_CANNOT_SWAP_ITEM_CASH_INVEN;
		return false;
	}

	CItem * item2 = _owner->m_wearInventory.wearItemInfo[wearPos];		// 벗는 아이템

	// 펫은 앉은 상태로 입거나 벗기 불가
	if (_owner->IsSetPlayerState(PLAYER_STATE_SITDOWN) && ((item->IsPet()) || (item2 && item2->IsPet())))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
		SEND_Q(rmsg, _owner->m_desc);
		_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
		return false;
	}

	// 나이트 쉐도우는 마운트 타입 착용 불가
	if ( _owner->m_job == JOB_NIGHTSHADOW && item->IsPet() )
	{
		CPet *pet = _owner->GetPet( item->getPlus() );
		if( pet && pet->IsMountType() )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
			SEND_Q(rmsg, _owner->m_desc);
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	}

	// 둘다 NULL 이면 무시
	if (!item && !item2)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
		SEND_Q(rmsg, _owner->m_desc);
		_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
		return false;
	}

	// 벗기이면 무조건 위치가 올바르게 전달되야 함
	if (item2 != NULL && item2->m_itemProto->getItemWearing() != wearPos)
	{
		// 악세는 위치 비교시 2, 3 자리를 검사
		if (item2->IsAccessary())
		{
			if (wearPos < WEARING_ACCESSORY1 || wearPos > WEARING_ACCESSORY3)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
				SEND_Q(rmsg, _owner->m_desc);
				_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
				return false;
			}
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_CANNOT_WEAR);
			SEND_Q(rmsg, _owner->m_desc);
			_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	}

	return true;
}

bool CWearInvenManager::CanUndressNormal( CItem* item, int wearPos )
{
	if (isValidWarePos(wearPos) == false)
	{
		_lastError = ResponseClient::WEAR_ERR_INVALID_POS;
		return false;
	}

	// 봉인 아이템 불가능
	if ( item->IsAPet() == false
			&& item->getFlag() & FLAG_ITEM_SEALED )
	{
		_lastError = ResponseClient::WEAR_ERR_ITEM_SEAL;
		return false;
	}

	// 방어코드 : 원래 벗었었는데 또 벗을라 하믄 리턴
	if (item->getWearPos() == WEARING_NONE)
	{
		_lastError = ResponseClient::WEAR_ERR_CANNOT_TAKEOFF;
		return false;
	}

	return true;
}

void CWearInvenManager::Init()
{
	warePos_set_.insert(WEARING_HELMET);
	warePos_set_.insert(WEARING_ARMOR_UP);
	warePos_set_.insert(WEARING_WEAPON);
	warePos_set_.insert(WEARING_ARMOR_DOWN);
	warePos_set_.insert(WEARING_SHIELD);
	warePos_set_.insert(WEARING_GLOVE);
	warePos_set_.insert(WEARING_BOOTS);
	warePos_set_.insert(WEARING_ACCESSORY1);
	warePos_set_.insert(WEARING_ACCESSORY2);
	warePos_set_.insert(WEARING_ACCESSORY3);
	warePos_set_.insert(WEARING_PET);
	warePos_set_.insert(WEARING_BACKWING);
	warePos_set_.insert(COSTUME2_WEARING_HELMET);
	warePos_set_.insert(COSTUME2_WEARING_ARMOR_UP);
	warePos_set_.insert(COSTUME2_WEARING_WEAPON);
	warePos_set_.insert(COSTUME2_WEARING_ARMOR_DOWN);
	warePos_set_.insert(COSTUME2_WEARING_SHIELD);
	warePos_set_.insert(COSTUME2_WEARING_GLOVE);
	warePos_set_.insert(COSTUME2_WEARING_BOOTS);
	warePos_set_.insert(COSTUME2_WEARING_BACK);


	m_suitWearingPos[0] = WEARING_HELMET;
	m_suitWearingPos[1] = WEARING_ARMOR_UP;
	m_suitWearingPos[2] = WEARING_WEAPON;
	m_suitWearingPos[3] = WEARING_ARMOR_DOWN;
	m_suitWearingPos[4] = WEARING_SHIELD;
	m_suitWearingPos[5] = WEARING_SHIELD;


}

bool CWearInvenManager::isValidWarePos( int pos )
{
	std::set<int>::iterator it = warePos_set_.find(pos);
	if (it == warePos_set_.end())
	{
		_lastError = ResponseClient::WEAR_ERR_INVALID_POS;
		return false;
	}

	return true;
}

void CWearInvenManager::DressNormal( CItem* item, int wearPos )
{
	if (isValidWarePos(wearPos) == false)
	{
		LOG_ERROR("HACKING? : invalid wear pos[%d]. charIndex[%d]", wearPos, _owner->m_index);
		_owner->m_desc->Close("invalid wear pos");
		return;
	}

	if(wearItemInfo[wearPos] != NULL)
	{
		short tab = item->tab();
		short invenIndex = item->getInvenIndex();

		CItem *takeOffItem = getWearItem(wearPos);
		takeOffItem->unWearPos();
		_owner->m_wearInventory.UndressNormal(takeOffItem, wearPos);
		_owner->m_wearInventory.DressNormal(item, wearPos);
		_owner->m_inventory.addItemToInvenIndex(takeOffItem, tab, invenIndex);
		_owner->ChangeQuickSlot(takeOffItem, QUICKSLOT_TYPE_ITEM);

		return ;
	}
	else
	{
		item->setWearPos(wearPos);
		wearItemInfo[wearPos] = item;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::doItemWear* packet = reinterpret_cast<UpdateClient::doItemWear*>(rmsg->m_buf);
		UpdateClient::ItemWearMsg(rmsg);
		_owner->m_wearInventory.makeItemInfo(&packet->info, item);
		SEND_Q(rmsg, _owner->m_desc);
	}

	if ( (wearPos >= WEARING_SHOW_START && wearPos <= WEARING_SHOW_END)
			|| wearPos == WEARING_BACKWING )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		WearingMsg(rmsg, _owner, wearPos, item->getDBIndex(), item->getPlus());
		_owner->m_pArea->SendToCell(rmsg, _owner, false);
	}

	if(item->tab() != -1)
		_owner->m_inventory.eraseNotFree(item);
}

void CWearInvenManager::UndressNormal( CItem* item, int wearPos )
{
	if (isValidWarePos(wearPos) == false)
	{
		LOG_ERROR("HACKING? : invalid wear pos[%d]. charIndex[%d]", wearPos, _owner->m_index);
		_owner->m_desc->Close("invalid wear pos");
		return;
	}

	item->unWearPos();
	wearItemInfo[(int)wearPos] = NULL;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::ItemTakeoffMsg(rmsg, wearPos);
		SEND_Q(rmsg, _owner->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		WearingMsg(rmsg, _owner, wearPos, -1, 0);
		_owner->m_pArea->SendToCell(rmsg, _owner, false);
	}

	if(wearPos == WEARING_WEAPON)
	{
		if(_owner->m_evocationIndex > EVOCATION_NONE)
		{
			_owner->changeToggleState(_owner->m_evocationIndex, TOGGLE_SKILL);
			_owner->ChangeEvocationState(_owner->m_evocationIndex);
		}
	}
}

void CWearInvenManager::makeItemInfo( UpdateClient::itemInfo* info, CItem* pitem)
{
	memset(info, 0x00, sizeof(*info));
	info->virtualIndex = pitem->getVIndex();
	info->dbIndex = pitem->getDBIndex();
	info->wearPos = pitem->getWearPos();
	info->plus = pitem->getPlus();
	info->plus_2 = pitem->getPlus_2();
	info->flag = pitem->getFlag();
	info->nCompositeItem = pitem->m_nCompositeItem;
	info->used = pitem->getUsed();
	info->used_2 = pitem->getUsed_2();
	info->itemCount = pitem->Count();

	if (pitem->IsOriginItem())
	{
		for (int i = 0; i < MAX_ORIGIN_OPTION; ++i)
		{
			info->origin_var[i] = pitem->getOriginVar(i);
		}
	}
	else
	{
		info->option_count = pitem->m_nOption;

		for (int i = 0; i < MAX_ITEM_OPTION; ++i)
		{
			info->option_type[i] = pitem->m_option[i].m_type;
			info->option_level[i] = pitem->m_option[i].m_level;
		}
	}

	for (int i = 0; i < MAX_SOCKET_COUNT; ++i)
	{
		info->socket[i] = pitem->m_socketList.GetJewelAt(i);
	}

#ifdef DURABILITY
	info->now_durability = pitem->getNowDurability();
	info->max_durability = pitem->getMaxDurability();
#endif
}

void CWearInvenManager::SaveWearInvenInfo(std::vector<std::string>& vec)
{
	std::string deletestr = boost::str(boost::format(
										   "delete from t_wear_inven where a_char_index = %1%") % _owner->m_index);

	vec.push_back(deletestr);

	std::string insertstr;
	insertstr.reserve(2048);

#ifdef DURABILITY
	insertstr = "INSERT INTO t_wear_inven("
				"a_char_index, a_wear_pos, a_item_idx, a_plus, a_flag, a_serial, a_used, a_used2, a_item_option0, a_item_option1, a_item_option2, a_item_option3, a_item_option4,"
				"a_item_origin_var0, a_item_origin_var1, a_item_origin_var2, a_item_origin_var3, a_item_origin_var4, a_item_origin_var5,"
				"a_socket0, a_socket1, a_socket2, a_socket3, a_socket4, a_socket5, a_socket6, a_now_dur, a_max_dur) VALUES";
#else
	insertstr = "INSERT INTO t_wear_inven("
				"a_char_index, a_wear_pos, a_item_idx, a_plus, a_flag, a_serial, a_used, a_used2, a_item_option0, a_item_option1, a_item_option2, a_item_option3, a_item_option4,"
				"a_item_origin_var0, a_item_origin_var1, a_item_origin_var2, a_item_origin_var3, a_item_origin_var4, a_item_origin_var5,"
				"a_socket0, a_socket1, a_socket2, a_socket3, a_socket4, a_socket5, a_socket6) VALUES";
#endif

	int count = 0;
	CItem* wearItem;
	//일반 장비 부터 붙이고
	for(int i = 0; i <= WEARING_BACKWING; i++)
	{
		wearItem = this->wearItemInfo[i];
		makeSaveInfo( &insertstr, wearItem, &count, i );
	}
	//코스튬 아이템 저장
	CItem* wearCostItem = _owner->m_wearInventory.getCosWearSuitItem();
	if ( wearCostItem != NULL )
	{
		makeSaveInfo( &insertstr, wearCostItem, &count, COSTUME2_WEARING_SUIT );
	}
	else
	{
		for(int i = COSTUME2_WEARING_START; i < COSTUME2_WEARING_START + MAX_COSTUME_WEARING; i++)
		{
			wearItem = this->wearItemInfo[i];
			makeSaveInfo( &insertstr, wearItem, &count, i );
		}
	}
	if (count == 0)
		return;

	int index = insertstr.rfind(",");
	insertstr.erase(index);

	vec.push_back(insertstr);
}

bool CWearInvenManager::CanDressCostum( CItem* item, int wearPos )
{
	if (item == NULL)	// 입는 아이템도 벗는 아이템도 없음.
	{
		this->_lastError = ResponseClient::WEAR_ERR_INVALID_ITEM_INFO;
		return false;
	}
	if (wearPos < WEARING_HELMET || wearPos > WEARING_BOOTS)
	{
		_lastError = ResponseClient::WEAR_ERR_INVALID_ITEM_INFO;
		return false;
	}

	if (wearPos == WEARING_WEAPON || wearPos == WEARING_SHIELD)
	{
		_lastError = ResponseClient::WEAR_ERR_INVALID_ITEM_INFO;
		return false;
	}

	/////////////////////////////////// RETURN 되는 조건 검사 ///////////////////////////////////

	if(item && item->m_itemProto->getItemFlag() & ITEM_FLAG_CASTLLAN)
	{
		int zonenum;
		char castellan;
		bool ret;
		ret = _owner->CheckCastellanType(zonenum, castellan);

		if(item->m_itemProto->getItemNum3() & 0xffff0000)  // 성주멤버용 아이템
		{
			if(!ret || castellan)
			{
				// 성주멤버가 아니다.
				CNetMsg::SP rmsg(new CNetMsg);
				CastllanErrorMsg(rmsg, MSG_EX_CASTLLAN_ERROR_NOT_CASTLLAN_GUILD);
				SEND_Q(rmsg, _owner->m_desc);
				_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
				return false;
			}
			if(zonenum != (item->m_itemProto->getItemNum3() & 0xffff))
			{
				// 소속된 성의 아이템이 아님
				_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
				return false;
			}
		}
		else		// 성주용 아이템
		{
			if(!ret || !castellan)
			{
				// 성주멤버가 아니다.
				CNetMsg::SP rmsg(new CNetMsg);
				CastllanErrorMsg(rmsg, MSG_EX_CASTLLAN_ERROR_NOT_CASTLLAN);
				SEND_Q(rmsg, _owner->m_desc);
				_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
				return false;
			}
			if(zonenum != (item->m_itemProto->getItemNum3() & 0xffff))
			{
				// 소속된 성의 아이템이 아님
				_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
				return false;
			}
		}
	}

	if (!(item->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2))												// 입는아이템이 COSTUME2 Flag가 아니면.
	{
		_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
		return false;
	}

	if ((item->getFlag() & FLAG_ITEM_COMPOSITION) && (item->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE))	// 입는아이템이 결합 되었거나 결합 가능한 아이템이면.
	{
		_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
		return false;
	}

	if (wearPos != item->m_itemProto->getItemWearing())													// 입는 아이템의 입는 위치와 wear_pos와 틀리면.
	{
		_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
		return false;
	}

	if (!(item->m_itemProto->getItemJobFlag() & (1 << _owner->m_job)))
	{
		_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
		return false;
	}
	if ( item->getFlag() & FLAG_ITEM_SEALED )
	{
		_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
		return false;
	}
	if (item->getWearPos() != WEARING_NONE)
	{
		_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
		return false;
	}

	return true;
}

bool CWearInvenManager::CanUndressCostum( CItem* item, int wearPos )
{
	if (item == NULL)
	{
		_lastError = ResponseClient::WEAR_ERR_INVALID_ITEM_INFO;
		return false;
	}

	if (_owner->m_inventory.getEmptyCount() < 1 )
	{
		_lastError = ResponseClient::WEAR_ERR_NOT_ENOUGH_INVEN;
		return false;
	}

	if( !(item->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2) )
	{
		this->_lastError = ResponseClient::WEAR_ERR_INVALID_ITEM_INFO;
		return false;
	}

	return true;
}

void CWearInvenManager::ApplyDeleteItem( CItem* item, int wearPos )
{
	if (isValidWarePos(wearPos) == false)
	{
		LOG_ERROR("HACKING? : invalid wear pos[%d]. charIndex[%d]", wearPos, _owner->m_index);
		_owner->m_desc->Close("invalid wear pos");
		return;
	}

	CItem* pItemNormal = item;
	if((pItemNormal->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (pItemNormal->getFlag() & FLAG_ITEM_COMPOSITION))
	{
		pItemNormal = _owner->m_inventory.FindByVirtualIndex(pItemNormal->m_nCompositeItem);
	}
	if (pItemNormal && pItemNormal->IsOriginItem())
	{
		// 오리지날 옵션 및 세트 아이템의 스킬 삭제
		for (int i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
		{
			int nIndex = pItemNormal->GetOriginIndex(i);
			if (nIndex <= 0)
				continue;

			CSkill * pSkill = gserver->m_skillProtoList.Create(nIndex, pItemNormal->GetOriginLevel(i));
			if(pSkill)
			{
				if (pSkill->m_proto->IsActive())
				{
					//아이템 스킬 쿨타임을 저장
					{
						CSkill* skill = _owner->m_activeSkillList.Find(pSkill->m_proto->m_index);

						if(skill == NULL)
						{
							LOG_ERROR("HACKING? : invalid Skill Info. charIndex[%d], skillIndex[%d]", _owner->m_index, pSkill->m_proto->m_index);
							_owner->m_desc->Close("invalid Skill Info.");
							return;
						}

						CPC::map_t::iterator it = _owner->itemSkill.find(pSkill->m_proto->m_index);						
						if(it != _owner->itemSkill.end())
						{
							it->second = skill->m_usetime;
						}
						else
						{
							_owner->itemSkill.insert(std::pair<int, int>(pSkill->m_proto->m_index, skill->m_usetime));
						}
					}

					pSkill->Cancel(_owner);

					{
						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::makeSkillCancelMsg(rmsg, _owner);
						_owner->m_pArea->SendToCell(rmsg, _owner, true);
					}

					_owner->RemoveSkillFromQuickSlot(pSkill);
					_owner->m_activeSkillList.Remove(pSkill);
				}
				else if (pSkill->m_proto->IsPassive())
				{
					_owner->m_passiveSkillList.Remove(pSkill);
				}
				else
				{
					_owner->m_etcSkillList.Remove(pSkill);
				}
			}
		}

		// 세트 아이템 착용 개수 감소 및 세트 효과 스킬 삭제
		if (pItemNormal->m_itemProto->GetSetIndex())
		{
			_owner->CountSetItem(pItemNormal, false);
			_owner->RemoveSetItemValue(pItemNormal);

			pItemNormal->m_nSetWearCnt = 0;
			pItemNormal->m_nSetOption = 0;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillListMsg(rmsg, _owner);
			SEND_Q(rmsg, _owner->m_desc);
		}
	}

	// 애완동물 벗을 때에는 애완동물을 disappear 시킨다
	if (item->IsPet())
	{
		// 착용 아이템에 해당하는 펫을 찾는다
		CPet* pet = _owner->GetPet(item->getPlus());
		if (pet)
		{
			if (pet->IsMount())
			{
				pet->Mount(false);
			}
			else
			{
				pet->Disappear();
			}
		}
	}

	if( item->IsAPet() )
	{
		CAPet* pet = _owner->GetAPet(item->getPlus());
		if( pet )
		{
			if( pet->IsMount() )
			{
				pet->Mount(false);
			}
			pet->m_nDelay = pet->m_pProto->m_nDelay ;
			// 소확시 발생하는 스킬 사용-- 클라이언트 처리
			pet->Disappear();
		}
	}

	if( item->m_itemProto->getItemIndex() == ONE_PERIOD_ITEM
			|| item->m_itemProto->getItemIndex() == SEVEN_PERIOD_ITEM
			|| item->m_itemProto->getItemIndex() == THIRTY_PERIOD_ITEM
			|| item->m_itemProto->getItemIndex() == 2610
			|| item->m_itemProto->getItemIndex() == 4940
			|| item->m_itemProto->getItemIndex() == 4941
			|| item->m_itemProto->getItemIndex() == 4942
	  )
	{
		bool bClear = true;
		for( int nAcc = 0 ; nAcc < 3 ; nAcc++ )
		{
			CItem* pAcc = _owner->m_wearInventory.wearItemInfo[WEARING_ACCESSORY1+nAcc];
			if( pAcc && ( pAcc->m_itemProto->getItemIndex() == ONE_PERIOD_ITEM ||
						  pAcc->m_itemProto->getItemIndex() == SEVEN_PERIOD_ITEM ||
						  pAcc->m_itemProto->getItemIndex() == THIRTY_PERIOD_ITEM ||
						  pAcc->m_itemProto->getItemIndex() == 2610 ||
						  pAcc->m_itemProto->getItemIndex() == 4940 ||
						  pAcc->m_itemProto->getItemIndex() == 4941 ||
						  pAcc->m_itemProto->getItemIndex() == 4942 ) )
			{
				bClear = false;
				break;
			}
		}

		if( bClear )
			_owner->m_assist.CureByItemIndex(item->m_itemProto->getItemIndex());
	}
	//검사 다~하고 캐릭터의 보석 스킬 옵션 목록을 초기화 해준다
	_owner->m_optionAttSkillList.Clear();
	_owner->m_optionDefSkillList.Clear();

	_owner->CalcStatus(true);
	//펫의 HP,MP를 올려줄 수 있는 보석은 한 개만 밖을 수 있다고 함.
	if(_owner->GetAPet(0))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetStatusMsg(rmsg, _owner->GetAPet());
		_owner->m_pArea->SendToCell(rmsg, _owner, true);
		SEND_Q(rmsg, _owner->m_desc);
	}
}

bool CWearInvenManager::RemoveItem( char wearPos )
{
	if (isValidWarePos(wearPos) == false)
	{
		LOG_ERROR("HACKING? : invalid wear pos[%d]. charIndex[%d]", wearPos, _owner->m_index);
		_owner->m_desc->Close("invalid wear pos");
		return false;
	}

	CItem* item = _owner->m_wearInventory.wearItemInfo[wearPos];

	if(item == NULL)
		return false;

	if(wearPos < COSTUME2_WEARING_START)
	{
		ApplyDeleteItem(item, wearPos);
		this->UndressNormal(item, wearPos);
	}
	else if(_owner->m_wearInventory.getCosWearSuitItem() != NULL)
	{
		item = this->getCosWearSuitItem();
		this->UndressCostumeSuit();
	}
	else
	{
		this->UndressCostum(item, wearPos - COSTUME2_WEARING_START);
	}

	_owner->RemoveItemFromQuickSlot(item);
	delete item;

	return true;
}

void CWearInvenManager::sendWearInfoToClient()
{
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::doItemWearList* packet = reinterpret_cast<UpdateClient::doItemWearList*>(rmsg->m_buf);
		UpdateClient::ItemWearListMsg(rmsg);

		packet->count = 0;

		for(int i = 0; i < DEFAULT_MAX_WEARING; i++)
		{
			if( wearItemInfo[i] != NULL )
			{
				makeItemInfo(&packet->info_list[packet->count], wearItemInfo[i]);
				packet->count++;
			}
		}

		rmsg->setSize(sizeof(UpdateClient::doItemWearList) + (packet->count * sizeof(UpdateClient::itemInfo)));
		SEND_Q(rmsg, _owner->m_desc);
	}

	{
		if(this->getCosWearSuitItem() != NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::doItemWearCostumeSuit* packet = reinterpret_cast<UpdateClient::doItemWearCostumeSuit*>(rmsg->m_buf);
			UpdateClient::ItemWearCostumeSuitMsg(rmsg, this->getCosWearSuitItem());

			for(int i = 0, j = 0; i < MAX_COSTUME_WEARING; i++)
			{
				if ( wearItemInfo[i + COSTUME2_WEARING_START] != NULL )
				{
					makeItemInfo(&packet->info_list[j], wearItemInfo[i + COSTUME2_WEARING_START]);
					j++;
				}
			}

			rmsg->setSize(sizeof(UpdateClient::doItemWearCostumeSuit) + COSTUME2_SUIT_MAX_WEARING * sizeof(UpdateClient::itemInfo));
			SEND_Q(rmsg, _owner->m_desc);
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::doItemWearCostumeList* packet = reinterpret_cast<UpdateClient::doItemWearCostumeList*>(rmsg->m_buf);
			UpdateClient::ItemWearCostumeListMsg(rmsg);

			packet->count = 0;

			for(int i = COSTUME2_WEARING_START; i < COSTUME2_WEARING_START + MAX_COSTUME_WEARING; i++)
			{
				if ( wearItemInfo[i] != NULL )
				{
					makeItemInfo(&packet->info_list[packet->count], wearItemInfo[i]);
					packet->count++;
				}
			}

			rmsg->setSize(sizeof(UpdateClient::doItemWearCostumeList) + packet->count * sizeof(UpdateClient::itemInfo));
			SEND_Q(rmsg, _owner->m_desc);
		}
	}
}

CItem* CWearInvenManager::FindByVirtualIndex( int vIndex )
{
	for(int i = 0; i < MAX_WEARING; i++)
	{
		if(wearItemInfo[i] == NULL)
			continue;

		if(wearItemInfo[i]->getVIndex() == vIndex)
		{
			return wearItemInfo[i];
		}
	}
	return NULL;
}

void CWearInvenManager::DressCostumSuit(CItem* pSuit)
{
	//아이템 장착
	setCosWearSuitItem(pSuit);
	//아이템 세팅
	getCosWearSuitItem()->setWearPos(COSTUME2_WEARING_SUIT);


	//개별의상의 아이템을 생성하여 아이템 옵션 세팅 및 장착 정보 업데이트
	CItem* pItem = NULL;
	int itemIndex = -1;
	bool bWearing = false;
	for( int i = 0; i < COSTUME2_SUIT_MAX_WEARING; i++ )
	{
		itemIndex = pSuit->GetOriginIndex(i-1);
		CItemProto* proto = gserver->m_itemProtoList.FindIndex(itemIndex);
		if (!proto)
		{
			GAMELOG << init("SYS_ERR")
					<< "DressCostumeSuit proto NULL"
					<< pSuit->getDBIndex()
					<< itemIndex;
			continue;
		}

		pItem = new CItem();
		pItem->m_itemProto = proto;
		pItem->setDBIndex(proto->getItemIndex());
		pItem->setWearPos(proto->getItemWearing());
		pItem->setUsed(pSuit->getUsed());
		if (pItem->IsOriginItem())
		{
			for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
			{
				if(pItem->GetOriginIndex(i) >= 0)
				{
					pItem->setOriginVar(i, 100);
				}
			}
		}
		wearItemInfo[pItem->getWearPos() + COSTUME2_WEARING_START] = pItem;
		bWearing = true;
	}

	//포츈 버프 지급
	if (bWearing)
	{
		int skill_index = 0, skill_level = 0;
		gserver->m_Fortunelist.Resolve_fortune_value(skill_index, skill_level, pSuit->getPlus());
		CSkill *pSkill = gserver->m_skillProtoList.Create(skill_index, skill_level);
		bool bApply = false;
		if(pSkill)
		{
			ApplySkill(_owner, _owner, pSkill, -1, bApply);
			if(!bApply && pSkill)
			{
				delete pSkill;
				pSkill = NULL;
			}
		}
	}

	//패킷 전송
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::ItemWearCostumeSuitMsg(rmsg, pSuit);
		prepareSuitMsg(rmsg, _owner);
		SEND_Q(rmsg, _owner->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CostumeWearingSuitMsg(rmsg, _owner);
		_owner->m_pArea->SendToCell(rmsg, _owner, false);
	}
}

void CWearInvenManager::UndressCostumeSuit()
{
	//착용 아이템의 포츈 옵션 제거
	int skill_index = 0, skill_level = 0;
	gserver->m_Fortunelist.Resolve_fortune_value(skill_index, skill_level, getCosWearSuitItem()->getPlus());
	_owner->m_assist.CureBySkillIndex(skill_index);

	getCosWearSuitItem()->unWearPos();
	setCosWearSuitItem(NULL);

	for( int i = COSTUME2_WEARING_START; i < COSTUME2_WEARING_START + MAX_COSTUME_WEARING; i++ )
	{
		if( wearItemInfo[i] == NULL )
			continue;

		ApplyDeleteItem(wearItemInfo[i], i);

		delete wearItemInfo[i];
		wearItemInfo[i] = NULL;
	}

	//장착 해제 패킷 전송
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::CostSuitItemTakeOffMsg(rmsg, GetLastError());
		SEND_Q(rmsg, _owner->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CostumeWearingSuitMsg(rmsg, _owner);
		_owner->m_pArea->SendToCell(rmsg, _owner, false);
	}
}

void CWearInvenManager::UndressCostumeAll()
{
	for( int i = COSTUME2_WEARING_START; i < COSTUME2_WEARING_START + MAX_COSTUME_WEARING; i++ )
	{
		if( wearItemInfo[i] == NULL )
			continue;

		ApplyDeleteItem(wearItemInfo[i], i);
		_owner->m_inventory.addItem(wearItemInfo[i]);
		UndressCostum(wearItemInfo[i], wearItemInfo[i]->getWearPos());

		_owner->ChangeQuickSlot(wearItemInfo[i], QUICKSLOT_TYPE_ITEM);
	}
}

void CWearInvenManager::DressCostum( CItem* item, int wearPos )
{
	if (isValidWarePos(wearPos + COSTUME2_WEARING_START) == false)
	{
		LOG_ERROR("HACKING? : invalid wear pos[%d]. charIndex[%d]", wearPos + COSTUME2_WEARING_START, _owner->m_index);
		_owner->m_desc->Close("invalid wear pos");
		return;
	}

	if(wearItemInfo[wearPos + COSTUME2_WEARING_START] != NULL)
	{
		short tab = item->tab();
		short invenIndex = item->getInvenIndex();

		CItem* takeOffItem = getWearItem(wearPos + COSTUME2_WEARING_START);
		takeOffItem->unWearPos();

		_owner->m_wearInventory.UndressCostum(takeOffItem, wearPos);
		_owner->m_wearInventory.DressCostum(item, wearPos);
		_owner->m_inventory.addItemToInvenIndex(takeOffItem, tab, invenIndex);
		_owner->ChangeQuickSlot(takeOffItem, QUICKSLOT_TYPE_ITEM);

		return;
	}
	else
	{
		item->setWearPos(wearPos);
		wearItemInfo[(int)wearPos + COSTUME2_WEARING_START] = item;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::doItemWearCostume* packet = reinterpret_cast<UpdateClient::doItemWearCostume*>(rmsg->m_buf);
		UpdateClient::ItemWearCostumeMsg(rmsg);
		_owner->m_wearInventory.makeItemInfo(&packet->info, item);
		SEND_Q(rmsg, _owner->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CostumeWearingMsg(rmsg, _owner, wearPos, item->getDBIndex());
		_owner->m_pArea->SendToCell(rmsg, _owner, false);
	}
	this->dressCostumCount++;

	if(item->tab() != -1)
		_owner->m_inventory.eraseNotFree(item);
}

void CWearInvenManager::UndressCostum( CItem* item, int wearPos )
{
	if (isValidWarePos(wearPos + COSTUME2_WEARING_START) == false)
	{
		LOG_ERROR("HACKING? : invalid wear pos[%d]. charIndex[%d]", wearPos + COSTUME2_WEARING_START, _owner->m_index);
		_owner->m_desc->Close("invalid wear pos");
		return;
	}

	item->unWearPos();
	wearItemInfo[(int)wearPos + COSTUME2_WEARING_START] = NULL;

	//착용 해제 아이템 옵션 제거
	ApplyDeleteItem(item, wearPos);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::ItemWearCostumeTakeoffMsg(rmsg, wearPos);
		SEND_Q(rmsg, _owner->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CostumeWearingMsg(rmsg, _owner, wearPos, -1);
		_owner->m_pArea->SendToCell(rmsg, _owner, false);
	}

	//착용하고 있는 코스튬 개수 정보 감소
	this->dressCostumCount--;
}

bool CWearInvenManager::AddCostumSuitItem( CItem *item )
{
	//유효성 검사
	if(CanDressCostumSuit(item) == false)
	{
		return false;
	}

	//한벌 의상을 입고 있는 경우
	if( NULL != getCosWearSuitItem() )
	{
		//한벌의상 제거
		if(DelCostumSuitItem(item->tab(), item->getInvenIndex()) == false)
		{
			return false;
		}
	}
	//개별 코스튬 의상을 입고 있는 경우(싹다 회수)
	else if( dressCostumCount > 0 )
	{
		//장착 해제를 하려면 인벤토리 검사
		if(_owner->m_inventory.getEmptyCount() < dressCostumCount )
		{
			return false;
		}
		//개별의상 모두 장착 해제
		UndressCostumeAll();
		DressCostumSuit(item);
		_owner->m_inventory.eraseNotFree(item);
	}
	else
	{
		DressCostumSuit(item);
		_owner->m_inventory.eraseNotFree(item);
	}

	_owner->CalcStatus(true);

	return true;
}

bool CWearInvenManager::DelCostumSuitItem(short tab, short invenIndex)
{
	CItem* item = wearSuitItem;

	if (CanUndressCostumSuit(item) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::doItemWearError *packet = reinterpret_cast<ResponseClient::doItemWearError*>(rmsg->m_buf);
		ResponseClient::ItemTakeoffMsg(rmsg, GetLastError());
		SEND_Q(rmsg, _owner->m_desc);
		return false;
	}

	if (tab >= 0 && invenIndex >= 0)
	{
		//드래그 앤 드랍으로 아이템을 벗는 경우에는 인벤토리에서 해당 위치의 아이템을 가져와서 비교 한 후 처리
		CItem* dressItem = _owner->m_inventory.getItem(tab, invenIndex);
		if(dressItem == NULL)
		{
			UndressCostumeSuit();
			_owner->m_inventory.addItemToInvenIndex(item, tab, invenIndex);
		}
		else if( (dressItem->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2) == false )
		{
			UndressCostumeSuit();
			_owner->m_inventory.addItem(item);
		}
		else if( (dressItem->m_itemProto->getItemWearing() == item->m_itemProto->getItemWearing()) && dressItem->m_itemProto->getItemJobFlag() & (1 << _owner->m_job) )
		{
			UndressCostumeSuit();
			DressCostumSuit(dressItem);
			_owner->m_inventory.eraseNotFree(dressItem);
			_owner->m_inventory.addItemToInvenIndex(item, tab, invenIndex);
		}
		else
		{
			this->_lastError = ResponseClient::WEAR_ERR_CANNOT_WEAR;
			return false;
		}
	}
	else
	{
		UndressCostumeSuit();
		_owner->m_inventory.addItem(item);
	}

	_owner->ChangeQuickSlot(item, QUICKSLOT_TYPE_ITEM);

	_owner->CalcStatus(true);

	return true;
}

CItem* CWearInvenManager::getCosWearSuitItem()
{
	return wearSuitItem;
}

void CWearInvenManager::setCosWearSuitItem( CItem* item )
{
	wearSuitItem = item;
}

void CWearInvenManager::makeSaveInfo( std::string* insertstr, CItem* wearItem, int* count, int wearPos )
{
	//wearItem = _owner->m_wearInventory.wearItemInfo[i];
	if(wearItem != NULL)
	{
		int used = 0;
		if (wearItem->m_itemProto->getItemFlag() & ITEM_FLAG_ABS)
		{
			// 시간제 아이템
			if( wearItem->m_itemProto->getItemTypeIdx() == ITYPE_ETC
					&& wearItem->m_itemProto->getItemSubTypeIdx() == IETC_MONSTER_MERCENARY_CARD )
			{
				used = wearItem->getUsed();
			}
			else
			{
				used = wearItem->getUsed() - gserver->getNowSecond();
			}

		}
		else
		{
			used = wearItem->getUsed();
		}

		*insertstr += boost::str(boost::format(
									 "(%1%, %2%, %3%, %4%, %5%,"
									 " '%6%', %7%, %8% ")
								 % _owner->m_index % wearPos % wearItem->getDBIndex() % wearItem->GetItemPlus() % wearItem->getFlag()
								 % wearItem->m_serial % used % wearItem->getUsed_2()
								);
		if ((wearItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (wearItem->getFlag() & FLAG_ITEM_COMPOSITION))
		{
			// 결합된 의상 아이템은 결합된 일반 아이템의 virtualIndex를 가지고 있음
			CItem* pItemEquip = _owner->m_inventory.FindByVirtualIndex(wearItem->m_nCompositeItem);
			if (pItemEquip)
			{
				// option 0 : row, option 1 : col
				int row = pItemEquip->tab();
				int col = pItemEquip->getInvenIndex();
				*insertstr += boost::str(boost::format(", %d, %d") % row % col);
			}
			else
			{
				*insertstr += ", 0, 0";

				LOG_ERROR("Not found item [%d]", wearItem->m_nCompositeItem);
			}

			for (int j = 2; j < MAX_ITEM_OPTION; ++j)
			{
				*insertstr += ", 0";
			}
		}
		else
		{
			bool bIsRare = (wearItem->IsRareItem()) ? true : false;

			if (bIsRare)
			{
				int start = 0;
				if (wearItem->m_nOption == 2)
				{
					*insertstr += boost::str(boost::format(", %d, %d")
											 % wearItem->m_nRareOptionIndex % wearItem->m_nRareOptionBit);

					start = 2;
				}

				for (int j = start; j < MAX_ITEM_OPTION; ++j)
				{
					*insertstr += ",0";
				}
			}
			else
			{
				for (int j = 0; j < MAX_ITEM_OPTION; ++j)
				{
					int temp = 0;
					if (wearItem->m_option[j].m_level > 0)
					{
						wearItem->m_option[j].SetDBValue();
						temp = wearItem->m_option[j].m_dbValue;
					}

					*insertstr += boost::str(boost::format(", %d") % temp);
				}
			}
		}
		for (int j = 0; j < MAX_VARIATION_COUNT; ++j)
		{
			*insertstr += boost::str(boost::format(", %d") % wearItem->getOriginVar(j));
		}
		for (int j = 0; j < MAX_SOCKET_COUNT; ++j)
		{
			if(wearItem->m_socketList.m_socketpos != NULL)
			{
				*insertstr += boost::str(boost::format(", %d") % wearItem->m_socketList.m_socketpos[j].GetJewelDBIdx());
			}
			else
				*insertstr += ", -1";

		}

#ifdef DURABILITY
		*insertstr += boost::str(boost::format(",%d,%d") % wearItem->getNowDurability() % wearItem->getMaxDurability());
#endif

		*insertstr += "), ";
		(*count) ++;
	}
}

bool CWearInvenManager::CanDressCostumSuit( CItem* item )
{
	if(item == NULL)
	{
		_lastError = ResponseClient::WEAR_ERR_INVALID_ITEM_INFO;
		return false;
	}
	if( !(item->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2) )
	{
		_lastError = ResponseClient::WEAR_ERR_INVALID_ITEM_INFO;
		return false;
	}
	return true;
}

bool CWearInvenManager::CanUndressCostumSuit( CItem* item )
{
	if(item == NULL)
	{
		this->_lastError = ResponseClient::WEAR_ERR_CANNOT_TAKEOFF;
		return false;
	}

	if(_owner->m_inventory.getEmptyCount() < 1)
	{
		this->_lastError = ResponseClient::WEAR_ERR_NOT_ENOUGH_INVEN;
		return false;
	}

	if( !(item->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2) )
	{
		this->_lastError = ResponseClient::WEAR_ERR_INVALID_ITEM_INFO;
		return false;
	}
	return true;
}

CItem* CWearInvenManager::FindByDBIndex( int index )
{
	for (int i = 0; i < MAX_WEARING; i++)
	{
		if (wearItemInfo[i] == NULL)
			continue;

		CItem* pItem = wearItemInfo[i];
		if(pItem->getDBIndex() == index)
		{
			return pItem;
		}
	}

	return NULL;
}

bool CWearInvenManager::isSyndicateEquipment()
{
	for (int i = 0; i < MAX_WEARING; ++i)
	{
		if (wearItemInfo[i] == NULL)
			continue;

		CItem* pItem = wearItemInfo[i];
		if (pItem->isSyndicate())
		{
			return true;
		}
	}
	return false;
}

CItem* CWearInvenManager::getCalcWear( int index )
{
	CItem* item = wearItemInfo[index];
	if (item == NULL)
		return NULL;

	if (item->getFlag() & FLAG_ITEM_SEALED) // 봉인
		return NULL;

#ifdef DURABILITY
	if (item->m_itemProto->getItemFlag() & ITEM_FLAG_DURABILITY)
	{
		if (item->getNowDurability() < 10)
			return NULL;
	}
#endif

	// 결합 검사
	if ((item->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (item->getFlag() & FLAG_ITEM_COMPOSITION))
	{
#ifdef DURABILITY
		CItem* tItem = _owner->m_inventory.FindByVirtualIndex(item->m_nCompositeItem);
		if (tItem == NULL)
			return NULL;

		if (tItem->m_itemProto->getItemFlag() & ITEM_FLAG_DURABILITY)
		{
			if (tItem->getNowDurability() < 10)
				return NULL;
		}
#else
		if (_owner->m_inventory.FindByVirtualIndex(item->m_nCompositeItem) == NULL)
			return NULL;
#endif
	}

	return item;
}

void CWearInvenManager::searchWearItem( wearItem_search_t& vec )
{
	for (int i = 0; i < DEFAULT_MAX_WEARING; ++i)
	{
		if (wearItemInfo[i] == NULL)
			continue;

		if(wearItemInfo[i]->getWearPos() == WEARING_NONE)
			continue;

		wearItem_search_pair_t pair;
		pair.wearpos = i;
		pair.pItem = wearItemInfo[i];
		vec.push_back(pair);
	}
}

#ifdef HARDCORE_SERVER

extern ExpressSystemItemInfo* makeExpressSystemItemInfo(CItem* pItem, int itemCount, int sendType, const char* sendName);

void CWearInvenManager::sendToLCE_Only_hardcore()
{
	for(int i = 0; i < MAX_WEARING; ++i)
	{
		if(wearItemInfo[i] == NULL)
			continue;

		CItem* pItem = wearItemInfo[i];

		// LCE로 돌리지 말아야할 아이템
		// 기간제
		if (pItem->m_itemProto->isTimer())
			continue;

		// 캐쉬
		if (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_CASH)
			continue;

		// 결합
		if ((pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (pItem->getFlag() & FLAG_ITEM_COMPOSITION))
			continue;

		// 강화
		if (pItem->getPlus() > 0)
			continue;

		// 펫
		if (pItem->IsPet() || pItem->IsAPet())
			continue;

		// 대여
		if (pItem->IsLent())
			continue;

		// 봉인
		if (pItem->m_itemProto->getItemFlag() & FLAG_ITEM_SEALED)
			continue;

		{
			// 성주
			if (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_LORD)
				continue;

			if (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_CASTLLAN)
				continue;
		}

		pItem->unWearPos();

		ExpressSystemItemInfo* expressItem = makeExpressSystemItemInfo(pItem, pItem->Count(), EXPRESS_SEND_UNSEARCHABLE_STONESTATUS, "HARDCORE");
		DBManager::instance()->PushExpressSendItemToNickName(_owner->m_desc, expressItem, (const char *)_owner->m_nick, EXPRESS_SENDER_TYPE_WEAR_INVENTORY);

		delete wearItemInfo[i];
		wearItemInfo[i] = NULL;
	}
}
#endif

void CWearInvenManager::sendOneItemInfo( int wearPos )
{
	if (isValidWarePos(wearPos) == false)
	{
		LOG_ERROR("HACKING? : invalid wear pos[%d]. charIndex[%d]", wearPos, _owner->m_index);
		_owner->m_desc->Close("invalid wear pos");
		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::doItemWearChangeInfo* packet = reinterpret_cast<UpdateClient::doItemWearChangeInfo*>(rmsg->m_buf);
	packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
	packet->subType = MSG_SUB_UPDATE_WEAR_CHANGE_INFO;
	this->makeItemInfo(&packet->info, wearItemInfo[wearPos]);
	rmsg->setSize(sizeof(UpdateClient::doItemWearChangeInfo));
	SEND_Q(rmsg, _owner->m_desc);
}

void CWearInvenManager::checkItemTime()
{
	for (int i = 0; i < MAX_WEARING; ++i)
	{
		CItem* pItem = wearItemInfo[i];
		if (pItem == NULL)
			continue;

		if ((pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE )
				&& pItem->m_itemProto->getItemNum4() == - 1)
		{
			// 기간 무제한 아이템
			continue;
		}

		if((pItem->getDBIndex() >= 4927 && pItem->getDBIndex() <= 4932) || pItem->getDBIndex() == 6228)
		{
			// 할로윈 이벤트 호박머리 기간 지나면 삭제
			if((pItem->m_itemProto->getItemFlag() & ITEM_FLAG_ABS) &&
					(pItem->getUsed() < gserver->getNowSecond()))
			{
				GAMELOG << init("CLOTHES EXPIRE", _owner)
						<< itemlog(pItem)
						<< end;

				_owner->m_wearInventory.RemoveItem(i);
				continue;
			}
		}
		else if (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2)
		{
			if(pItem->getUsed() < gserver->getNowSecond())
			{
				GAMELOG << init("COSTUME2 CLOTHES EXPIRE", _owner)
						<< itemlog(pItem)
						<< end;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					SysExpireCostumeItem(rmsg, pItem->getDBIndex());
					SEND_Q(rmsg, _owner->m_desc);
				}

				_owner->m_wearInventory.RemoveItem(i);

				continue;
			}
		}
		// 대여 무기
		else if (pItem->IsLent())
		{
			if (pItem->getUsed() < gserver->getNowSecond())
			{
				// 아이템 삭제 후 리턴
				GAMELOG << init("ITEM USE TIMEOUT", _owner)
						<< "ITEM" << delim
						<< itemlog(pItem)
						<< end;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ItemLendWeaponMsg(rmsg, pItem->getDBIndex(), MSG_ITEM_LEND_TIMEOUT);
					SEND_Q(rmsg, _owner->m_desc);
				}

				_owner->m_wearInventory.RemoveItem(i);
				continue;
			}
		}// 대여 무기
		// 결합 의상
		else if (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE)
		{
			// 결합 의상 체크가 플래티늄보다 위에 있어야 함
			// 의상도 플래티늄 맥스 플러스 사용 -_-;
			// 결합용 의상 아이템
			// 결합된 아이템
			if (!(pItem->m_itemProto->getItemFlag() & ITEM_FLAG_ABS))
			{
				// 기간제
				if (pItem->getUsed_2() > 0
						&& pItem->getUsed_2() < gserver->getNowSecond())
				{
					// 결합 주문서 시간 만료
					// 결합한 아이템이 있는가?
					// 있으면 결합을 해제하고
					// 착용 중이면 착용 해제하고
					// 인벤에 삽입
					if (pItem->getFlag() & FLAG_ITEM_COMPOSITION)
					{
						CItem * pItemEquip = _owner->m_inventory.FindByVirtualIndex(pItem->m_nCompositeItem);
						if (pItemEquip)
						{
							pItemEquip->setFlag(pItemEquip->getFlag() & ~FLAG_ITEM_COMPOSITION);
							_owner->m_inventory.sendOneItemInfo(pItemEquip);

							GAMELOG << init("ITEM SEPARATE", _owner)
									<< "CLOTHES" << delim
									<< itemlog(pItem) << delim
									<< "EQUIP" << delim
									<< itemlog(pItemEquip)
									<< end;
						}

						GAMELOG << init("COMPOSITE_TIME EXPIRE", _owner)
								<< itemlog(pItem)
								<< end;

						{
							CNetMsg::SP rmsg(new CNetMsg);
							SysExpireItemcompositeMsg(rmsg, pItem->getDBIndex());
							SEND_Q(rmsg, _owner->m_desc);
						}
						RemoveItem(i);
					}
				}	// 결합된 아이템
			}

			if (pItem->getUsed() < gserver->getNowSecond())
			{
				// 유료 의상 시간 만료
				// 결합한 아이템이 있는가?
				// 있으면 결합을 해제하고
				// 착용 중이면 착용 해제하고
				// 인벤에서 제거
				if (pItem->getFlag() & FLAG_ITEM_COMPOSITION)
				{
					pItem->setFlag(pItem->getFlag() & ~FLAG_ITEM_COMPOSITION);

					CItem* pItemEquip = _owner->m_inventory.FindByVirtualIndex(pItem->m_nCompositeItem);
					if (pItemEquip)
					{
						pItemEquip->setFlag(pItemEquip->getFlag() & ~FLAG_ITEM_COMPOSITION);

						_owner->m_inventory.sendOneItemInfo(pItemEquip);

						GAMELOG << init("ITEM SEPARATE", _owner)
								<< "CLOTHES" << delim
								<< itemlog(pItem) << delim
								<< "EQUIP" << delim
								<< itemlog(pItemEquip)
								<< end;
					}
				}
				GAMELOG << init("CLOTHES EXPIRE", _owner)
						<< itemlog(pItem)
						<< end;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					SysExpireItemcompositeMsg(rmsg, pItem->getDBIndex());
					SEND_Q(rmsg, _owner->m_desc);
				}

				this->RemoveItem(i);
				continue;
			}
		} //결합 의상
		else if( pItem->IsUsedPlatinumSpecial() )
		{
			if (pItem->getUsed() < gserver->getNowSecond())
			{
				// 삭제 후 리턴
				int platinumPlus = pItem->getFlag() & PLATINUM_MAX_PLUS;
				pItem->setPlus(pItem->getPlus() - platinumPlus);

				if(pItem->getPlus() <= 0) // LC-TR-P20100428-018 버그 내용 수정 ( 플래티넘 기간 만료시 0 보다 작게 되는 경우 수정)
				{
					pItem->setPlus(0);
				}

				pItem->setUsed(-1);

				GAMELOG << init("PLATINUM USE TIMEOUT", _owner)
					<< "ITEM" << delim
					<< itemlog(pItem) << delim
					<< platinumPlus << delim
					<< end;

				pItem->setFlag(FLAG_ITEM_PLATINUM_SET(pItem->getFlag(), 0));

				this->sendOneItemInfo(pItem->getWearPos());

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_PLATINUM_SPECIAL_EXPIRE, 0, 0, pItem->m_itemProto->getItemIndex());
					SEND_Q(rmsg, _owner->m_desc);
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					WearingMsg(rmsg, _owner, pItem->getWearPos(), pItem->getDBIndex(), pItem->getPlus());
					_owner->m_pArea->SendToCell(rmsg, _owner, false);
				}

				_owner->m_assist.CheckApplyConditions();
				_owner->CalcStatus(true);

				continue;
			}
		} // 플래티늄

		switch (pItem->m_itemProto->getItemIndex())
		{
		case ONE_PERIOD_ITEM:
		case SEVEN_PERIOD_ITEM:
		case THIRTY_PERIOD_ITEM:
		case 2610:
		case 4940:
		case 4941:
		case 4942:
			{
				if( pItem->getUsed() != -1
						&& pItem->getUsed() < gserver->getNowSecond())
				{
					// 기간 만료
					// 아이템 삭제 후 리턴
					GAMELOG << init("ITEM USE TIMEOUT", _owner)
							<< "ITEM" << delim
							<< itemlog(pItem)
							<< end;
					for( int k = WEARING_ACCESSORY1; k <= WEARING_ACCESSORY3; k++ )
					{
						if (_owner->m_wearInventory.wearItemInfo[k])
						{
							if( _owner->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == ONE_PERIOD_ITEM
									|| _owner->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == SEVEN_PERIOD_ITEM
									|| _owner->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == THIRTY_PERIOD_ITEM
									|| _owner->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 2610
									|| _owner->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 4940
									|| _owner->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 4941
									|| _owner->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 4942
							  )
							{
								char wear_pos = _owner->m_wearInventory.wearItemInfo[k]->getWearPos();
								_owner->m_assist.CureByItemIndex(_owner->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex());
								_owner->m_wearInventory.RemoveItem(k);
							}
						}
					}
					continue;
				}
			}
			break;
		}

		// ATTACK_PET  관련 아이템 시간 체크
		if( ( pItem->m_itemProto->getItemJobFlag()&(1<<JOB_APET) ) &&
				( pItem->m_itemProto->getItemFlag()&ITEM_FLAG_ABS) )
		{
			if (pItem->getUsed() < gserver->getNowSecond())
			{
				// AI 아이템은 AI 리스트에서도 삭제
				if (pItem->m_itemProto->getItemTypeIdx() == ITYPE_ETC && pItem->m_itemProto->getItemSubTypeIdx() == IETC_PET_AI)
				{
					CAPet * pAPetNetxt = _owner->m_pApetlist;
					CAPet * pAPet;
					while((pAPet = pAPetNetxt))
					{
						pAPetNetxt = pAPet->m_pNextPet;
						pAPet->DelAIList(pItem);
					}

					pAPet = _owner->GetAPet();
					if (pAPet)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						APetAIListMsg(rmsg, pAPet);
						SEND_Q(rmsg, _owner->m_desc);
					}
				}
				// 아이템 삭제 후 리턴
				GAMELOG << init("ITEM USE TIMEOUT ATTACK_PET", _owner)
						<< "ITEM" << delim
						<< itemlog(pItem)
						<< end;

				RemoveItem(i);
				continue;
			}
		}

		if( (pItem->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY) && (pItem->m_itemProto->getItemNum4() > 0) )
		{
			if (pItem->getUsed() < gserver->getNowSecond())
			{
				GAMELOG << init("ACCESSORY DELETE", _owner)
						<< itemlog(pItem)
						<< end;

				switch(pItem->getDBIndex())
				{
				case ARTIFACT_LEVEL1_ITEM_INDEX:
				case ARTIFACT_LEVEL2_ITEM_INDEX:
				case ARTIFACT_LEVEL3_ITEM_INDEX:
					{
						ArtifactManager::instance()->expire_time(pItem);
					}
					break;
				}

				_owner->m_wearInventory.wearItemInfo[(int)pItem->getWearPos()] = NULL;
				RemoveItem(i);
				continue;
			}
		}

		if((pItem->m_itemProto->getItemFlag() & ITEM_FLAG_TRIGGER))
		{
			if (pItem->getUsed() < gserver->getNowSecond() && _owner->m_pZone )
			{
				GAMELOG << init("TRIGGER ITEM DELETE", _owner)
						<< itemlog(pItem)
						<< end;

				RemoveItem(i);
				continue;
			}
		}

#if defined(LC_TLD)
		if (!gserver->isActiveEvent(A_EVENT_HALLOWEEN) && (pItem->getDBIndex() >= 2482 && pItem->getDBIndex() <= 2491))
		{
			GAMELOG << init("HALLOWEEN EVENT END DELETE ITEM", _owner) << itemlog(pItem) << end;

			RemoveItem(i);
			continue;
		}

#elif defined(LC_GAMIGO)
		if (!gserver->isActiveEvent(A_EVENT_HALLOWEEN) && ((pItem->getDBIndex() >= 2464 && pItem->getDBIndex() <= 2475) || (pItem->getDBIndex() >= 2488 && pItem->getDBIndex() <= 2491)))
		{
			GAMELOG << init("HALLOWEEN EVENT END DELETE ITEM", _owner) << itemlog(pItem) << end;

			RemoveItem(i);
			continue;
		}
#else
		if (!gserver->isActiveEvent(A_EVENT_HALLOWEEN) && (pItem->getDBIndex() >= 2488 && pItem->getDBIndex() <= 2491))
		{
			GAMELOG << init("HALLOWEEN EVENT END DELETE ITEM", _owner) << itemlog(pItem) << end;

			RemoveItem(i);
			continue;
		}
#endif // HALLOWEEN ITEM DELETE
		if (!gserver->isActiveEvent(A_EVENT_XMAS) && ((pItem->getDBIndex() >= 2584 && pItem->getDBIndex() <= 2589) || (pItem->getDBIndex() >= 2604 && pItem->getDBIndex() <= 2605) || (pItem->getDBIndex() >= 2607 && pItem->getDBIndex() <= 2608)))
		{
			GAMELOG << init("XMAS EVENT END DELETE ITEM", _owner) << itemlog(pItem) << end;

			RemoveItem(i);
			continue;
		}
	} // end for
}

void CWearInvenManager::initCostumSuitItem( CItem* item )
{
	//아이템 장착
	setCosWearSuitItem(item);
	//아이템 세팅
	getCosWearSuitItem()->setWearPos(COSTUME2_WEARING_SUIT);


	//개별의상의 아이템을 생성하여 아이템 옵션 세팅 및 장착 정보 업데이트
	CItem* pItem = NULL;
	int itemIndex = -1;
	bool bWearing = false;
	for( int i = 0; i < COSTUME2_SUIT_MAX_WEARING; i++ )
	{
		itemIndex = item->GetOriginIndex(i-1);
		CItemProto* proto = gserver->m_itemProtoList.FindIndex(itemIndex);
		if (!proto)
		{
			GAMELOG << init("SYS_ERR")
				<< "DressCostumeSuit proto NULL"
				<< item->getDBIndex()
				<< itemIndex;
			continue;
		}

		pItem = new CItem();
		pItem->m_itemProto = proto;
		pItem->setDBIndex(proto->getItemIndex());
		pItem->setWearPos(proto->getItemWearing());
		pItem->setUsed(item->getUsed());
		if (pItem->IsOriginItem())
		{
			for (int i = 0; i < MAX_ORIGIN_OPTION; i++)
			{
				if(pItem->GetOriginIndex(i) >= 0)
				{
					pItem->setOriginVar(i, 100);
				}
			}
		}
		wearItemInfo[pItem->getWearPos() + COSTUME2_WEARING_START] = pItem;
		bWearing = true;
	}

	//포츈 버프 지급
	if (bWearing)
	{
		int skill_index = 0, skill_level = 0;
		gserver->m_Fortunelist.Resolve_fortune_value(skill_index, skill_level, item->getPlus());
		CSkill *pSkill = gserver->m_skillProtoList.Create(skill_index, skill_level);
		bool bApply = false;
		if(pSkill)
		{
			ApplySkill(_owner, _owner, pSkill, -1, bApply);
			if(!bApply && pSkill)
			{
				delete pSkill;
				pSkill = NULL;
			}
		}
	}
	_owner->CalcStatus(true);
}

void CWearInvenManager::initCostumItem( CItem* item, int wearPos )
{
	item->setWearPos(wearPos);
	wearItemInfo[(int)wearPos + COSTUME2_WEARING_START] = item;
	dressCostumCount++;
}

int CWearInvenManager::getWearItemPlusSumCount()
{
	int getCount = 0;
	static int armors[] = {WEARING_HELMET, WEARING_ARMOR_UP, WEARING_ARMOR_DOWN, WEARING_SHIELD, WEARING_GLOVE, WEARING_BOOTS, WEARING_BACKWING };
	CItem* item;
	for (int i = 0; i < sizeof(armors) / sizeof(armors[0]); i++)
	{
		item = getCalcWear(armors[i]);
		if (item && item->IsArmorType())
		{
			getCount += item->getPlus();
		}
	}

	return getCount;
}

