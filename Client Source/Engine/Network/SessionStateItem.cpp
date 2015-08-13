
#include "StdH.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIInternalClasses.h>
#include "SessionState.h"
#include <Engine/Ska/Render.h>
#include <Common/Packet/ptype_char_status.h>
#include <Common/Packet/ptype_old_do_item.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/PetStash.h>
#include <Engine/Contents/Base/Auction.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Templates/Stock_CEntityClass.h>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/GameState.h>
#include <Engine/Help/ItemHelp.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/Interface/UIPetTraining.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIChildInvenSlot.h>
#include <Engine/Interface/UITeleport.h>
#include <Engine/Interface/UIInvenCashBagBox.h>
#include <Engine/Contents/Base/UISkillNew.h>
#include <Engine/Interface/UIWareHouse.h>
#include <Engine/Interface/UIProcess.h>
#include <Engine/Interface/UISocketSystem.h>
#include <Engine/Interface/UIProduct.h>
#include <Engine/Contents/Base/UIChangeWeaponNew.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIPersonalShop.h>
#include <Engine/Interface/UIMixNew.h>
#include <Engine/Interface/UIWildPetInfo.h>
#include <Engine/Object/ActorMgr.h>
#include <Common/Packet/ptype_durability.h>
#include <Engine/Contents/Base/UIDurability.h>
#include <Engine/Interface/UIGuildWarPortal.h>
#include <Engine/Contents/Base/Notice.h>
#include <Engine/Contents/Base/UITrade.h>
#include <Engine/Contents/Base/InvenData.h>
#include <Engine/Info/MyInfo.h>

DECLARE_PACKET(MoneyStash);

void CSessionState::reg_packet()
{
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_FIRST_MONENY, updateMoney);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_MONENY, updateMoney);
	REG_PACKET(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_FIRST_MONENY_IN_STASH, MoneyStash);
	REG_PACKET(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_MONENY_IN_STASH, MoneyStash);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_INVEN_LIST, updateInvenList);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_ADD_ITEM, updateItemAdd);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_DELETE_ITEM, updateItemDelete);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_ITEM_COUNT, updateItemCount);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_ITEM_COUNT_WITH_SWAP, updateItemCountSwap);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_ITEM_PLUS, updateItemPlus);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_ITEM_PLUS_2, updateItemPlus2);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_ITEM_FLAG, updateItemFlag);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_ITEM_USED, updateItemUsed);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_ITEM_USED_2, updateItemUsed2);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_ITEM_WEAR_POS, updateItemWearPos);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_ITEM_OPTION, updateItemOption);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_ITEM_SOCKET, updateItemSocket);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_ITEM_ORIGIN_VAR, updateItemOriginVar);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_ITEM_COMPOSITE_ITEM, updateItemComposite);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_NAS_DESC_REASON, updateMoneyReason);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_OLD_TIMER_ITEM, updateOldTimerItem);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_ERASE_ALL_ITEM, updateEraseAllItem);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_WEAR, updateItemWear);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_WEAR_TAKEOFF, updateItemTakeOff);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_WEAR_LIST, updateWearItemList);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_WEAR_CHANGE_INFO, updateWearItemInfo);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_COST_WEAR, updateCostWear);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_COST_WEAR_TAKEOFF, updateCostTakeOff);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_COST_WEAR_LIST, updateWearCostList);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_COST_WEAR_SUIT, updateCostSuitWear);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_WEAR_ITEM_USED, updateWearItemUsed);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_WEAR_ITEM_PLUS, updateWearItemPlus);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHANGE_WEAR_ITEM_FLAG, updateWearItemFlag);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_STASH_PASSWORD_FLAG, updateStashPasswordFlag);
#ifdef DURABILITY
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_ITEM_DURABILITY_FOR_INVENTORY, updateDurabilityForInventory);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_ITEM_DURABILITY_FOR_WEAR, updateDurabilityForWear);
#endif	//	DURABILITY	
}

//------------------------------------------------------------------------

void CSessionState::updateMoney( CNetworkMessage* istr )
{
	UpdateClient::money* pPack = reinterpret_cast<UpdateClient::money*>(istr->GetBuffer());

	CTString	strSysMessage;
	bool bChange = false;
	INT64	delta = 0;
	
	if (_pNetwork->MyCharacterInfo.money != pPack->nas)
	{
		// 나스가 변화 되었다면
		bChange = true;

		delta = pPack->nas - _pNetwork->MyCharacterInfo.money;
	}

	_pNetwork->MyCharacterInfo.money = pPack->nas;

	if (pPack->subType == MSG_SUB_UPDATE_MONENY)
	{
		if (delta > 0)	//나스 증가
		{
			CUIManager* pUIMgr = CUIManager::getSingleton();

			CTString strResult;

			if (pPack->bonus > 0)
			{
				CTString strCount;
				CTString strBonus;

				strCount.PrintF( "%I64d", delta - pPack->bonus );
				pUIMgr->InsertCommaToString( strCount );
				
				strBonus.PrintF( "%I64d", pPack->bonus );
				pUIMgr->InsertCommaToString( strBonus );

				strResult.PrintF( "%s(+%s)", strCount, strBonus );				
			}
			else
			{
				strResult.PrintF( "%I64d", delta );
				pUIMgr->InsertCommaToString( strResult );
			}

			strSysMessage.PrintF( _S( 416, "%s 나스를 얻었습니다." ), strResult ); // 번역 수정
			_pNetwork->ClientSystemMessage( strSysMessage );	
		}
	}

	if (bChange == true)
	{
		GameDataManager::getSingleton()->GetAuction()->updateNas();

		CUIManager* pUIMgr = CUIManager::getSingleton();

		if (pUIMgr->GetInventory()->getLocked() == LOCK_WAREHOUSE)
			pUIMgr->GetWareHouse()->ResetWareHouse();

		pUIMgr->GetSkillNew()->UpdateSPointAndMoney();
	}
}

IMPLEMENT_PACKET(MoneyStash)
{
	UIMGR()->GetWareHouse()->ReceiveNas( istr );
}

void CSessionState::setWearItemInfo( UpdateClient::itemInfo* pInfo )
{
	CUIManager* pUIMgr = CUIManager::getSingleton();
	
	CItemData* pItemData	= _pNetwork->GetItemData(pInfo->dbIndex);

	if (pItemData == NULL)
		return;

	if(pItemData->GetType()==CItemData::ITEM_WEAPON)
	{
		const BOOL bExtension = _pNetwork->IsExtensionState( _pNetwork->MyCharacterInfo.job, *pItemData );
		_pNetwork->MyCharacterInfo.bExtension = bExtension;
	}
	
	_pNetwork->MyWearItem[pInfo->wearPos].Item_Wearing = pInfo->wearPos;
	_pNetwork->MyWearItem[pInfo->wearPos].ItemData	 = pItemData;
	_pNetwork->MyWearItem[pInfo->wearPos].SetData( pInfo->dbIndex, pInfo->virtualIndex, 
		0, 0, pInfo->plus, pInfo->flag, pInfo->nCompositeItem, 
		pInfo->used, pInfo->used_2, pInfo->wearPos, pInfo->itemCount );

	_pNetwork->MyWearItem[pInfo->wearPos].SetItemPlus2(pInfo->plus_2);

	// 펫을 장착한 경우....
	if( pItemData->GetType() == CItemData::ITEM_ACCESSORY && 
		pItemData->GetSubType() == CItemData::ACCESSORY_PET &&
		pInfo->wearPos != -1)
	{
		const INDEX iPetIndex = _pNetwork->MyWearItem[pInfo->wearPos].Item_Plus;
		CNetworkLibrary::sPetInfo	TempPet;
		CPetTargetInfom* pPetInfo = INFO()->GetMyPetInfo();
		TempPet.lIndex				= iPetIndex;
		pPetInfo->lIndex		= iPetIndex;
		std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
			std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
		if( iter != _pNetwork->m_vectorPetList.end() )
		{
			pPetInfo->iLevel		= (*iter).lLevel;
			pPetInfo->fHealth		= (*iter).lHP;
			pPetInfo->fMaxHealth	= (*iter).lMaxHP;
			pPetInfo->fMaxHungry	= (*iter).lMaxHungry;
			pPetInfo->fHungry		= (*iter).lHungry;
			pPetInfo->lAbility		= (*iter).lAbility;
			pPetInfo->bIsActive		= TRUE;				
			pPetInfo->strNameCard	= (*iter).strNameCard;

			INDEX iPetType;
			INDEX iPetAge;

			_pNetwork->CheckPetType( (*iter).sbPetTypeGrade, iPetType, iPetAge );

			pPetInfo->iType			= iPetType;
			pPetInfo->iAge			= iPetAge;

			const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);
			if( bPetRide )
			{
				// 마운트 상태에서 애완동물의 배고픔이 0이라면 움직이지 못함.
				if (pPetInfo->fHungry <= 0 )
					CUIManager::getSingleton()->SetCSFlagOn( CSF_MOUNT_HUNGRY );
				else			
					CUIManager::getSingleton()->SetCSFlagOff( CSF_MOUNT_HUNGRY );
			}
		}
	}	

	_pNetwork->MyWearItem[pInfo->wearPos].InitOptionData();

	//레어 아이템일때...
	if( pItemData->GetFlag() & ITEM_FLAG_RARE )
	{
		//옵션 개수가 0이면 미감정 레어아이템
		if (pInfo->option_count == 0)
			_pNetwork->MyWearItem[pInfo->wearPos].SetRareIndex(0);
		//감정된 레어아이템이면...
		else
		{
			pUIMgr->SetRareOption( pInfo, _pNetwork->MyWearItem[pInfo->wearPos] );
		}
	}
	//레어 아이템이 아니면.....
	else
	{
		int		opt;

		if (pItemData->GetFlag() & ITEM_FLAG_ORIGIN)
		{
			_pNetwork->MyWearItem[pInfo->wearPos].SetItemBelong(pItemData->GetItemBelong());

			for (opt = 0; opt < MAX_ORIGIN_OPTION; ++opt)
			{
				_pNetwork->MyWearItem[pInfo->wearPos].SetOptionData( opt, pItemData->GetOptionOriginType(opt), 
					pItemData->GetOptionOriginLevel(opt), pInfo->origin_var[opt] );
			}

			// 아이템 스킬 세팅
			for (opt = 0; opt < MAX_ITEM_SKILL; ++opt)
			{
				_pNetwork->MyWearItem[pInfo->wearPos].SetItemSkill(opt, 
					pItemData->GetOptionSkillType(opt), pItemData->GetOptionSkillLevel(opt));
			}
		}
		else
		{
			for (opt = 0; opt < pInfo->option_count; ++opt)
			{
				_pNetwork->MyWearItem[pInfo->wearPos].SetOptionData( opt, 
					pInfo->option_type[opt], 
					pInfo->option_level[opt], 
					ORIGIN_VAR_DEFAULT );
			}
		}
	}

	_pNetwork->MyWearItem[pInfo->wearPos].InitSocketInfo();
	// receive data about socket type. [6/23/2010 rumist]
	if( pItemData->GetFlag() & ITEM_FLAG_SOCKET )
	{
		SBYTE	sbSocketCreateCount = 0;
		SBYTE	sbSocketCount = 0;
		LONG	lSocketJewelIndex = 0;

		int	sc;

		for (sc = 0; sc < JEWEL_MAX_COUNT; ++sc)
		{
			if (pInfo->socket[sc] >= 0)
			{
				sbSocketCreateCount++;
				_pNetwork->MyWearItem[pInfo->wearPos].SetSocketOption( sc, pInfo->socket[sc] );
			}
		}

		_pNetwork->MyWearItem[pInfo->wearPos].SetSocketCount( sbSocketCreateCount );
	}

#ifdef DURABILITY
	_pNetwork->MyWearItem[pInfo->wearPos].SetDurability(pInfo->now_durability, pInfo->max_durability);
#endif	//	DURABILITY
}

void CSessionState::setWearCostItemInfo( UpdateClient::itemInfo* pInfo )
{
	int nVirIdx = pInfo->virtualIndex;
	SBYTE wear_pos = pInfo->wearPos;

	if ( pInfo->wearPos == WEAR_BACKWING )
		wear_pos = WEAR_COSTUME_BACKWING;

	CUIManager* pUIMgr = CUIManager::getSingleton();
	CItemData* pItemData	= _pNetwork->GetItemData(pInfo->dbIndex);

	_pNetwork->MyWearCostItem[wear_pos].Item_Wearing = pInfo->wearPos;
	_pNetwork->MyWearCostItem[wear_pos].ItemData	 = pItemData;
	_pNetwork->MyWearCostItem[wear_pos].SetData( pInfo->dbIndex, nVirIdx, 
		0, 0, pInfo->plus, pInfo->flag, pInfo->nCompositeItem, 
		pInfo->used, pInfo->used_2, wear_pos, pInfo->itemCount );

	_pNetwork->MyWearCostItem[wear_pos].SetItemPlus2(pInfo->plus_2);
	_pNetwork->MyWearCostItem[wear_pos].InitOptionData();

	//레어 아이템일때...
	if( pItemData->GetFlag() & ITEM_FLAG_RARE )
	{
		//옵션 개수가 0이면 미감정 레어아이템
		if (pInfo->option_count == 0)
			_pNetwork->MyWearCostItem[wear_pos].SetRareIndex(0);
		//감정된 레어아이템이면...
		else
		{
			pUIMgr->SetRareOption( pInfo, _pNetwork->MyWearCostItem[wear_pos] );
		}
	}
	//레어 아이템이 아니면.....
	else
	{
		int		opt;

		if (pItemData->GetFlag() & ITEM_FLAG_ORIGIN)
		{
			_pNetwork->MyWearCostItem[wear_pos].SetItemBelong(pItemData->GetItemBelong());

			for (opt = 0; opt < MAX_ORIGIN_OPTION; ++opt)
			{
				_pNetwork->MyWearCostItem[wear_pos].SetOptionData( opt, pItemData->GetOptionOriginType(opt), 
					pItemData->GetOptionOriginLevel(opt), pInfo->origin_var[opt] );
			}

			// 아이템 스킬 세팅
			for (opt = 0; opt < MAX_ITEM_SKILL; ++opt)
			{
				_pNetwork->MyWearCostItem[wear_pos].SetItemSkill(opt, 
					pItemData->GetOptionSkillType(opt), pItemData->GetOptionSkillLevel(opt));
			}
		}
		else
		{
			for (opt = 0; opt < pInfo->option_count; ++opt)
			{
				_pNetwork->MyWearCostItem[wear_pos].SetOptionData( opt, 
					pInfo->option_type[opt], 
					pInfo->option_level[opt], 
					ORIGIN_VAR_DEFAULT );
			}
		}
	}

	_pNetwork->MyWearCostItem[wear_pos].InitSocketInfo();
	// receive data about socket type. [6/23/2010 rumist]
	if( pItemData->GetFlag() & ITEM_FLAG_SOCKET )
	{
		SBYTE	sbSocketCreateCount = 0;
		SBYTE	sbSocketCount = 0;
		LONG	lSocketJewelIndex = 0;

		int	sc;

		for (sc = 0; sc < JEWEL_MAX_COUNT; ++sc)
		{
			if (pInfo->socket[sc] >= 0)
			{
				sbSocketCreateCount++;
				_pNetwork->MyWearCostItem[wear_pos].SetSocketOption( sc, pInfo->socket[sc] );
			}
		}

		_pNetwork->MyWearCostItem[wear_pos].SetSocketCount( sbSocketCreateCount );
	}
}

void CSessionState::setItemInfo( int tabId, UpdateClient::itemInfo* pInfo )
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	int		invenidx;

	invenidx = pInfo->invenIndex;

	CItemData* pItemData	= _pNetwork->GetItemData(pInfo->dbIndex);

	_pNetwork->MySlotItem[tabId][invenidx].Item_Wearing = pInfo->wearPos;
	_pNetwork->MySlotItem[tabId][invenidx].ItemData		= pItemData;
	_pNetwork->MySlotItem[tabId][invenidx].SetData( pInfo->dbIndex, pInfo->virtualIndex, 
		tabId, invenidx, pInfo->plus, pInfo->flag, pInfo->nCompositeItem, 
		pInfo->used, pInfo->used_2, pInfo->wearPos, pInfo->itemCount );

	_pNetwork->MySlotItem[tabId][invenidx].SetItemPlus2(pInfo->plus_2);
	_pNetwork->MySlotItem[tabId][invenidx].SetToggle(pInfo->toggle);

	// 펫을 장착한 경우....
	if( pItemData->GetType() == CItemData::ITEM_ACCESSORY && 
		pItemData->GetSubType() == CItemData::ACCESSORY_PET &&
		pInfo->wearPos != -1)
	{
		const INDEX iPetIndex = _pNetwork->MySlotItem[tabId][invenidx].Item_Plus;
		CNetworkLibrary::sPetInfo	TempPet;
		CPetTargetInfom* pPetInfo = INFO()->GetMyPetInfo();
		TempPet.lIndex				= iPetIndex;
		pPetInfo->lIndex		= iPetIndex;
		std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
			std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
		if( iter != _pNetwork->m_vectorPetList.end() )
		{
			pPetInfo->iLevel		= (*iter).lLevel;
			pPetInfo->fHealth		= (*iter).lHP;
			pPetInfo->fMaxHealth	= (*iter).lMaxHP;
			pPetInfo->fMaxHungry	= (*iter).lMaxHungry;
			pPetInfo->fHungry		= (*iter).lHungry;
			pPetInfo->lAbility		= (*iter).lAbility;
			pPetInfo->bIsActive		= TRUE;				
			pPetInfo->strNameCard	= (*iter).strNameCard;

			INDEX iPetType;
			INDEX iPetAge;
			_pNetwork->CheckPetType( (*iter).sbPetTypeGrade, iPetType, iPetAge );

			pPetInfo->iType			= iPetType;
			pPetInfo->iAge			= iPetAge;
		}
	}	

	_pNetwork->MySlotItem[tabId][invenidx].InitOptionData();

	//레어 아이템일때...
	if( pItemData->GetFlag() & ITEM_FLAG_RARE )
	{
		//옵션 개수가 0이면 미감정 레어아이템
		if (pInfo->option_count == 0)
			_pNetwork->MySlotItem[tabId][invenidx].SetRareIndex(0);
		//감정된 레어아이템이면...
		else
		{
			pUIMgr->SetRareOption(pInfo, _pNetwork->MySlotItem[tabId][invenidx]);
		}
	}
	//레어 아이템이 아니면.....
	else
	{
		int		opt;

		if (pItemData->GetFlag() & ITEM_FLAG_ORIGIN)
		{
			_pNetwork->MySlotItem[tabId][invenidx].SetItemBelong(pItemData->GetItemBelong());

			for (opt = 0; opt < MAX_ORIGIN_OPTION; ++opt)
			{
				_pNetwork->MySlotItem[tabId][invenidx].SetOptionData( opt, pItemData->GetOptionOriginType(opt), 
					pItemData->GetOptionOriginLevel(opt), pInfo->origin_var[opt] );
			}

			// 아이템 스킬 세팅
			for (opt = 0; opt < MAX_ITEM_SKILL; ++opt)
			{
				_pNetwork->MySlotItem[tabId][invenidx].SetItemSkill(opt, 
					pItemData->GetOptionSkillType(opt), pItemData->GetOptionSkillLevel(opt));
			}
		}
		else
		{
			for (opt = 0; opt < pInfo->option_count; ++opt)
			{
				_pNetwork->MySlotItem[tabId][invenidx].SetOptionData( opt, 
					pInfo->option_type[opt], 
					pInfo->option_level[opt], 
					ORIGIN_VAR_DEFAULT );
			}
		}
	}

	_pNetwork->MySlotItem[tabId][invenidx].InitSocketInfo();
	// receive data about socket type. [6/23/2010 rumist]
	if( pItemData->GetFlag() & ITEM_FLAG_SOCKET )
	{
		SBYTE	sbSocketCreateCount = 0;
		SBYTE	sbSocketCount = 0;
		LONG	lSocketJewelIndex = 0;

		int	sc;

		for (sc = 0; sc < JEWEL_MAX_COUNT; ++sc)
		{
			if (pInfo->socket[sc] >= 0)
			{
				sbSocketCreateCount++;
				_pNetwork->MySlotItem[tabId][invenidx].SetSocketOption( sc, pInfo->socket[sc] );
			}
		}

		_pNetwork->MySlotItem[tabId][invenidx].SetSocketCount( sbSocketCreateCount );
	}

#ifdef DURABILITY
	_pNetwork->MySlotItem[tabId][invenidx].SetDurability(pInfo->now_durability, pInfo->max_durability);
#endif	//	DURABILITY

	pUIMgr->GetInventory()->InitInventory( tabId, invenidx, 
		pInfo->virtualIndex, pInfo->dbIndex, pInfo->wearPos );

	//===================================================================>>
	CItems* pItem = NULL;
	pItem = new CItems;

	memcpy(pItem, &_pNetwork->MySlotItem[tabId][invenidx], sizeof(CItems));

	CInvenData* pInvenData = CInvenData::getSingleton();
	pInvenData->InitItemData(pItem);
	//===================================================================<<
}

void CSessionState::updateInvenList(CNetworkMessage* istr)
{
	int		i;
	int		tabId;

	CUIManager* pUIMgr = CUIManager::getSingleton();
	CInvenData* pInvenData = CInvenData::getSingleton();

	UpdateClient::invenList* pPack = reinterpret_cast<UpdateClient::invenList*>(istr->GetBuffer());
	
	// 캐쉬 탭을 포함한 인벤토리
	tabId = pPack->tab_no;

	UpdateClient::itemInfo* pInfo;

	if (tabId <= INVENTORY_TAB_CASH_2)
	{
		for (i = 0; i < pPack->count; ++i)
		{
			pInfo = &pPack->info_list[i];

			setItemInfo(tabId, pInfo);
			setNewItemEffect(tabId, pInfo->invenIndex, FALSE);
			pUIMgr->GetInventory()->InitInventory(tabId, pInfo->invenIndex, pInfo->virtualIndex, pInfo->dbIndex, pInfo->wearPos);
		}
	}
	else if (pPack->tab_no == INVENTORY_TAB_STASH)
	{
		// 창고
		pUIMgr->GetWareHouse()->ReceiveList( istr );
	}
}

void CSessionState::updateItemAdd( CNetworkMessage* istr )
{
	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	CUIManager* pUIManager = CUIManager::getSingleton();
	UpdateClient::addItemInfo* pPack = reinterpret_cast<UpdateClient::addItemInfo*>(istr->GetBuffer());
	
	Notice* pNotice = GAMEDATAMGR()->GetNotice();

	if (pPack->tab_no <= INVENTORY_TAB_CASH_2)
	{
		int		item_index = pPack->info.dbIndex;

		// 드레이크의 알을 얻었을 경우, Notice에 표시해줌.
		if( item_index == 949 )
		{
			if( !ItemHelp::HaveItem( item_index ) && !ItemHelp::HaveItem( 872 ))
			{
				if (pNotice != NULL)
					pNotice->AddToNoticeList(4001, Notice::NOTICE_EVENT);
			}
		}

		// 판의 피리를 얻었을 경우, Notice에 표시해줌.
		if( item_index == 948 )
		{
			if( !ItemHelp::HaveItem( item_index ) && !ItemHelp::HaveItem( 871 ) )
			{
				if (pNotice != NULL)
					pNotice->AddToNoticeList(4000, Notice::NOTICE_EVENT);
			}
		}

		// wooss 060810
		// 핑크 드레이크의 알을 얻었을 경우, Notice에 표시해줌.
		if( item_index == 1706 )
		{
			if( !ItemHelp::HaveItem( item_index ) && !ItemHelp::HaveItem( 1711 ) )
			{
				if (pNotice != NULL)
					pNotice->AddToNoticeList(4003, Notice::NOTICE_EVENT);
			}
		}
		// 불가사의한 드레이크의 알을 얻었을 경우, Notice에 표시해줌.
		if( item_index == 1709 )
		{
			if( !ItemHelp::HaveItem( item_index ) && !ItemHelp::HaveItem( 1712 ) )
			{
				if (pNotice != NULL)
					pNotice->AddToNoticeList(4005, Notice::NOTICE_EVENT);
			}
		}
		// 파란 판의 피리를 얻었을 경우, Notice에 표시해줌.
		if( item_index == 1707 )
		{
			if( !ItemHelp::HaveItem( item_index ) && !ItemHelp::HaveItem( 1710 ) )
			{
				if (pNotice != NULL)
					pNotice->AddToNoticeList(4002, Notice::NOTICE_EVENT);
			}
		}
		// 불가사의한의 피리를 얻었을 경우, Notice에 표시해줌.
		if( item_index == 1708 )
		{
			if( !ItemHelp::HaveItem( item_index ) && !ItemHelp::HaveItem( 1713 ) )
			{
				if (pNotice != NULL)
					pNotice->AddToNoticeList(4004, Notice::NOTICE_EVENT);
			}
		}

		CItemData*	pItemData = _pNetwork->GetItemData(item_index);
		const char* szItemName = _pNetwork->GetItemName( item_index );

		setItemInfo(pPack->tab_no, &pPack->info);
		setNewItemEffect(pPack->tab_no, pPack->info.invenIndex, TRUE);

		CTString	strSysMessage;

		if( pPack->info.itemCount > 0 )
			strSysMessage.PrintF( _S( 417, "%s %d개를 얻었습니다." ), szItemName, pPack->info.itemCount );
		else
			strSysMessage.PrintF( _S2( 303, szItemName, "%s<를> 얻었습니다." ),
			szItemName );

		_pNetwork->ClientSystemMessage( strSysMessage );

		// edit by cpp2angel (044.12.20) : 
		if ( pUIManager->GetProcess()->IsVisible () )
		{
			pUIManager->GetProcess()->SelectItem ();
		}

		if ( pUIManager->GetProduct()->IsVisible () )
		{
			pUIManager->GetProduct()->SelectItem ();
		}

		if(pUIManager->GetChangeWeapon()->GetCashItem()){ //wooss 051011
			CUIMsgBox_Info	MsgBoxInfo;
			if( pItemData->GetType() == CItemData::ITEM_WEAPON)
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(1049,"무기 교체" ), UMBS_OK,UI_NONE, MSGCMD_NULL);
				MsgBoxInfo.AddString( _S(2229, "무기가 성공적으로 교체되었습니다.") );
			}
			else if(pItemData->GetType() == CItemData::ITEM_SHIELD)
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(3536, "방어구 교체" ), UMBS_OK,UI_NONE, MSGCMD_NULL);
				MsgBoxInfo.AddString( _S(3537, "방어구가 성공적으로 교체되었습니다.") );
			}
			pUIManager->CreateMessageBox( MsgBoxInfo );
			pUIManager->GetChangeWeapon()->close();												
		}

		// [091216: selo] 스킬 배우기 UI 갱신
		if(pUIManager->DoesUIExist(UI_SKILLLEARN))
		{
			pUIManager->GetSkillLearn()->UpdateSkillLearn();
		}

		if (pItemData->GetType() == CItemData::ITEM_WEAPON ||
			pItemData->GetType() == CItemData::ITEM_SHIELD ||
			pItemData->GetType() == CItemData::ITEM_ACCESSORY)
		{
			pUIManager->GetQuickSlot()->SetWearingLock(FALSE);
		}

		// 교환 UI가 떠있는 상태라면 아이템 카운트 갱신.
		if (pUIManager->DoesUIExist(UI_TRADE))
			pUIManager->GetTrade()->UpdateAmendConditionCount(-1);
	}
	else if (pPack->tab_no == INVENTORY_TAB_STASH)
	{
		CTString strMessage;
		const char* szName = _pNetwork->GetItemName( pPack->info.dbIndex );
		CItemData* pItemData = _pNetwork->GetItemData( pPack->info.dbIndex );
		if( pItemData->GetType() == CItemData::ITEM_ETC &&
			pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			strMessage.PrintF( _S( 1346, "나스를 %I64d개 보관하였습니다." ), pPack->info.itemCount);		
		}
		else
		{
			strMessage.PrintF(_S( 808, "%s를 %I64d개 보관하였습니다." ), szName, pPack->info.itemCount);
		}
		_pNetwork->ClientSystemMessage( strMessage );

		// 창고 UI가 열려 있다면, 닫는다
		if (pUIManager->GetInventory()->getLocked() == LOCK_WAREHOUSE)
			pUIManager->GetWareHouse()->ResetWareHouse();
	}
}

void CSessionState::updateItemDelete( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::deleteItemInfo* pPack = reinterpret_cast<UpdateClient::deleteItemInfo*>(istr->GetBuffer());

	if (pPack->tab_no <= INVENTORY_TAB_CASH_2)
	{
		CItems*		pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];
		CItemData* pItemData = pItems->ItemData;

		if (pItemData == NULL)
			return;

		// 투명 코스튬이 아니어야 한다.
		if (pItems->Item_Wearing > -1 && pItemData->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
		{
			if (!(pItemData->GetWearingPosition() == WEAR_HELMET && (CTString)pItemData->GetItemSmcFileName() == MODEL_TREASURE))
			{
				CEntity			*penPlEntity;
				CPlayerEntity	*penPlayerEntity;
				penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
				penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

				penPlayerEntity->DeleteCurrentArmor(pItems->Item_Wearing);//1005 아이템 깨지는 버그수정
				penPlayerEntity->DeleteDefaultArmor(pItems->Item_Wearing);

				//_pNetwork->DeleteMyCurrentWearing(pItems->Item_Wearing);
			}
		}

		_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex].Init();
		pUIManager->GetInventory()->InitInventory( pPack->tab_no, pPack->invenIndex, -1, -1, -1 );
		setNewItemEffect(pPack->tab_no, pPack->invenIndex, FALSE);

		// [091216: selo] 스킬 배우기 UI 갱신
		if(pUIManager->DoesUIExist(UI_SKILLLEARN))
		{
			pUIManager->GetSkillLearn()->UpdateSkillLearn();
		}

		if (pItemData->GetType() == CItemData::ITEM_WEAPON ||
			pItemData->GetType() == CItemData::ITEM_SHIELD ||
			pItemData->GetType() == CItemData::ITEM_ACCESSORY)
		{
			pUIManager->GetQuickSlot()->SetWearingLock(FALSE);
		}
	}
	else if (pPack->tab_no == INVENTORY_TAB_STASH)
	{
		if (pUIManager->GetInventory()->getLocked() == LOCK_WAREHOUSE)
			pUIManager->GetWareHouse()->ResetWareHouse();
	}
}

void CSessionState::updateItemCount(CNetworkMessage* istr)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeItemCount* pPack = reinterpret_cast<UpdateClient::changeItemCount*>(istr->GetBuffer());	

	if (pPack->tab_no <= INVENTORY_TAB_CASH_2)
	{
		CItems* pItems = NULL;	

		pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];
		int delta = pItems->Item_Sum;
		pItems->Item_Sum = pPack->count;

		delta = pPack->count - delta;

		pUIManager->GetInventory()->GetItemIcon(pItems->Item_UniIndex)->setCount(pItems->Item_Sum);

		if( delta > 0 )
		{
			CTString	strSysMessage;
			const char* szItemName	= _pNetwork->GetItemName(pItems->Item_Index);

			strSysMessage.PrintF( _S( 417, "%s %d개를 얻었습니다." ), szItemName, delta );
			_pNetwork->ClientSystemMessage( strSysMessage );

			setNewItemEffect(pPack->tab_no, pPack->invenIndex, TRUE);
		}

		pUIManager->GetQuickSlot()->UpdateItemCount(pItems->Item_UniIndex, pItems->Item_Sum);
	}
	else if (pPack->tab_no == INVENTORY_TAB_STASH)
	{
		LONGLONG llItemSum = pUIManager->GetWareHouse()->GetWareHouseItemCount( pPack->invenIndex );

		if ( llItemSum < pPack->count )
		{
			int nDbIndex = pUIManager->GetWareHouse()->GetWareHouseItemIndex( pPack->invenIndex );
			LONGLONG llAddCount = pPack->count - llItemSum;
			CTString strMessage;
			const char* szName = _pNetwork->GetItemName( nDbIndex );
			CItemData* pItemData = _pNetwork->GetItemData( nDbIndex );

			if( pItemData->GetType() == CItemData::ITEM_ETC &&
				pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
			{
				strMessage.PrintF( _S( 1346, "나스를 %I64d개 보관하였습니다." ), llAddCount );		
			}
			else
			{
				strMessage.PrintF( _S( 808, "%s를 %I64d개 보관하였습니다." ), szName, llAddCount );
			}
			_pNetwork->ClientSystemMessage( strMessage );
		}	

		// 창고 UI가 열려 있다면, 닫는다
		if (pUIManager->GetInventory()->getLocked() == LOCK_WAREHOUSE)
			pUIManager->GetWareHouse()->ResetWareHouse();

		// 교환 UI가 떠있는 상태라면 아이템 카운트 갱신.
		if (pUIManager->DoesUIExist(UI_TRADE))
			pUIManager->GetTrade()->UpdateAmendConditionCount(-1);
	}
}

void CSessionState::updateItemCountSwap(CNetworkMessage* istr)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeItemCount* pPack = reinterpret_cast<UpdateClient::changeItemCount*>(istr->GetBuffer());	

	if (pPack->tab_no <= INVENTORY_TAB_CASH_2)
	{
		CItems* pItems = NULL;

		pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];
		pItems->Item_Sum = pPack->count;

		CUIIcon* pIcon = pUIManager->GetInventory()->GetItemIcon(pItems->Item_UniIndex);
		if (pIcon != NULL)
			pIcon->setCount(pPack->count);

		pUIManager->GetQuickSlot()->UpdateItemCount(pItems->Item_UniIndex, pItems->Item_Sum);
	}
	else if (pPack->tab_no == INVENTORY_TAB_STASH)
	{
		if (pUIManager->GetInventory()->getLocked() == LOCK_WAREHOUSE)
			pUIManager->GetWareHouse()->ResetWareHouse();
	}	
}

void CSessionState::updateItemPlus( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeItemPlus* pPack = reinterpret_cast<UpdateClient::changeItemPlus*>(istr->GetBuffer());

	if (pPack->tab_no < 0 || pPack->tab_no > INVENTORY_TAB_CASH_2)
		return;

	if (pPack->invenIndex < 0 || pPack->invenIndex >= ITEM_COUNT_IN_INVENTORY_NORMAL)
		return;

	CItems* pItems = NULL;
	pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];
	pItems->Item_Plus = pPack->plus;
}

void CSessionState::updateItemPlus2( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeItemPlus_2* pPack = reinterpret_cast<UpdateClient::changeItemPlus_2*>(istr->GetBuffer());
	
	CItems* pItems = NULL;
	pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];
	pItems->SetItemPlus2(pPack->plus_2);
}

void CSessionState::updateItemFlag( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeItemFlag* pPack = reinterpret_cast<UpdateClient::changeItemFlag*>(istr->GetBuffer());
	
	if (pPack->tab_no < 0 || pPack->tab_no > INVENTORY_TAB_CASH_2)
		return;

	if (pPack->invenIndex < 0 || pPack->invenIndex >= ITEM_COUNT_IN_INVENTORY_NORMAL)
		return;

	CItems* pItems = NULL;
	pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];
	pItems->Item_Flag = pPack->flag;

	///=====================================================================================>>
	CItems* pItem = CInvenData::getSingleton()->GetItemInfo(pPack->tab_no, pPack->invenIndex);
	
	if (pItem != NULL)
		pItem->Item_Flag = pPack->flag;
	///=====================================================================================<<
}

void CSessionState::updateItemUsed( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeItemUsed* pPack = reinterpret_cast<UpdateClient::changeItemUsed*>(istr->GetBuffer());

	if (pPack->tab_no < 0 || pPack->tab_no > INVENTORY_TAB_CASH_2)
		return;

	if (pPack->invenIndex < 0 || pPack->invenIndex >= ITEM_COUNT_IN_INVENTORY_NORMAL)
		return;

	CItems* pItems = NULL;
	pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];
	pItems->Item_Used = pPack->used;

	pUIManager->GetInventory()->InitInventory( pPack->tab_no, pPack->invenIndex, pItems->Item_UniIndex, 
		pItems->Item_Index, pItems->Item_Wearing );
}

void CSessionState::updateItemUsed2( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeItemUsed_2* pPack = reinterpret_cast<UpdateClient::changeItemUsed_2*>(istr->GetBuffer());

	CItems* pItems = NULL;
	pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];
	pItems->Item_Used2 = pPack->used_2;
}

void CSessionState::updateItemWearPos( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeItemWearPos* pPack = reinterpret_cast<UpdateClient::changeItemWearPos*>(istr->GetBuffer());

	CItems* pItems = NULL;
	pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];
	
	pItems->Item_Wearing = pPack->wearPos;

	pUIManager->GetInventory()->InitInventory( pPack->tab_no, pPack->invenIndex, pItems->Item_UniIndex, 
		pItems->Item_Index, pPack->wearPos );

	pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	CModelInstance *pMI	= NULL;
	if( _pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide)
	{
		CModelInstance *pMITemp = penPlayerEntity->GetModelInstance();

		INDEX ctmi = pMITemp->mi_cmiChildren.Count();
		if( ctmi > 0 )
			pMI = &pMITemp->mi_cmiChildren[0];
		else
			pMI	= penPlayerEntity->GetModelInstance();
	}
	else
	{
		pMI	= penPlayerEntity->GetModelInstance();
	}			
	_pNetwork->MyCharacterInfo.itemEffect.Change(_pNetwork->MyCharacterInfo.job
		, _pNetwork->GetItemData(pItems->Item_Index)
		, pItems->Item_Wearing
		, pItems->Item_Plus
		, &pMI->m_tmSkaTagManager
		, 1, _pNetwork->GetItemData(pPack->wearPos)->GetSubType() );
}

void CSessionState::updateItemOption( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeItemOption* pPack = reinterpret_cast<UpdateClient::changeItemOption*>(istr->GetBuffer());

	CItems* pItems = NULL;
	pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];

	pItems->InitOptionData();

	CItemData* pItemData = pItems->ItemData;

	if( pItemData->GetFlag() & ITEM_FLAG_RARE )
	{
		//옵션 개수가 0이면 미감정 레어아이템
		if (pPack->option_count == 0)
			_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex].SetRareIndex(0);
		//감정된 레어아이템이면...
		else
		{
			LONG iRareIndex = pPack->option_level[0];
			pItems->SetRareIndex(iRareIndex);

			// option_level[1] bit mask
			WORD iRareOption = pPack->option_level[1];
			WORD wCBit =1;
			SBYTE sbOption =-1;
			for(int iBit=0; iBit<10; ++iBit)
			{
				if(iRareOption & wCBit)
				{
					CItemRareOption* pItem = CItemRareOption::getData(iRareIndex);

					if (pItem == NULL )
						continue;

					if (pItem->GetIndex() < 0)
						continue;

					int OptionType = pItem->rareOption[iBit].OptionIdx;
					int OptionLevel = pItem->rareOption[iBit].OptionLevel;

					pItems->SetOptionData( ++sbOption, OptionType, OptionLevel, ORIGIN_VAR_DEFAULT );
				}
				wCBit <<=1;
			}
		}
	}
	//레어 아이템이 아니면.....
	else
	{		
		if ((pItemData->GetFlag() & ITEM_FLAG_ORIGIN) == false)
		{
			int		opt;

			for (opt = 0; opt < pPack->option_count; ++opt)
			{
				_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex].SetOptionData( opt, 
					pPack->option_type[opt], 
					pPack->option_level[opt], 
					ORIGIN_VAR_DEFAULT );
			}
		}
	}
}

void CSessionState::updateItemSocket( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeItemSocket* pPack = reinterpret_cast<UpdateClient::changeItemSocket*>(istr->GetBuffer());

	CItems* pItems = NULL;
	pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];

	CItemData* pItemData = pItems->ItemData;
	
	// receive data about socket type. [6/23/2010 rumist]
	if( pItemData->GetFlag() & ITEM_FLAG_SOCKET )
	{
		pItems->InitSocketInfo();

		SBYTE	sbSocketCreateCount = 0;
		int	sc;

		for (sc = 0; sc < JEWEL_MAX_COUNT; ++sc)
		{
			if (pPack->socket[sc] >= 0)
			{
				sbSocketCreateCount++;
				pItems->SetSocketOption( sc, pPack->socket[sc] );
			}
		}

		pItems->SetSocketCount( sbSocketCreateCount );
	}
}

void CSessionState::updateItemOriginVar( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeItemOriginVar* pPack = reinterpret_cast<UpdateClient::changeItemOriginVar*>(istr->GetBuffer());

	CItems* pItems = NULL;
	pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];

	CItemData* pItemData = pItems->ItemData;

	if (pItemData->GetFlag() & ITEM_FLAG_ORIGIN)
	{
		int		opt;

		pItems->InitOptionData();

		_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex].SetItemBelong(pItemData->GetItemBelong());

		for (opt = 0; opt < MAX_ORIGIN_OPTION; ++opt)
		{
			_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex].SetOptionData( opt, pItemData->GetOptionOriginType(opt), 
				pItemData->GetOptionOriginLevel(opt), pPack->origin_var[opt] );
		}

 		// 아이템 스킬 세팅
 		for (opt = 0; opt < MAX_ITEM_SKILL; ++opt)
 		{
 			_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex].SetItemSkill(opt, 
 				pItemData->GetOptionSkillType(opt), pItemData->GetOptionSkillLevel(opt));
 		}
	}
}

void CSessionState::updateItemComposite( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeItemComposite* pPack = reinterpret_cast<UpdateClient::changeItemComposite*>(istr->GetBuffer());

	CItems* pItems = NULL;
	pItems = &_pNetwork->MySlotItem[pPack->tab_no][pPack->invenIndex];
	pItems->ComItem_index = pPack->nCompositeItem;
}

void CSessionState::updateItemUse( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	ResponseClient::doItemUse* pPack = reinterpret_cast<ResponseClient::doItemUse*>(istr->GetBuffer());

	LONG lItemIndex = pUIManager->GetInventory()->GetItemIndex( pPack->tab, pPack->invenIndex );

	CItemData*	pItemData = _pNetwork->GetItemData( lItemIndex );

	INDEX db_ItemIndex = pItemData->GetItemIndex();

	//071009_ttos: 창고 이용 주문서, 잡화상 이용 주문서
	if(db_ItemIndex == 2455 || db_ItemIndex == 2456 || db_ItemIndex == 2457 || db_ItemIndex == 2607)	//창고 이용 주문서
	{
		_pNetwork->MyCharacterInfo.bOtherZone = FALSE;
		pUIManager->GetWareHouse()->OpenWareHouse(0, true);
		return;
	}

	if(db_ItemIndex == 2458 || db_ItemIndex == 2459 || db_ItemIndex == 2460 || db_ItemIndex == 2608)	//잡화상 이용 주문서
	{
		pUIManager->GetShop()->SetFieldShopChack(TRUE);
		pUIManager->GetShop()->OpenShop(130, -1, FALSE,_pNetwork->MyCharacterInfo.x,_pNetwork->MyCharacterInfo.z);
		return;
	}

	if(db_ItemIndex == 2565 || db_ItemIndex == 2566 || db_ItemIndex == 2567)
	{
		pUIManager->GetPersonalShop()->SetCashPersonShop(TRUE);
		pUIManager->GetPersonalShop()->OpenPersonalShop( TRUE );
		return;
	}

	if (db_ItemIndex == 2867) // 애완동물 조련사 NPC 이용 카드
	{
		pUIManager->GetPetTraining()->OpenPetTraining(pItemData->GetNum0(), FALSE, _pNetwork->MyCharacterInfo.x,_pNetwork->MyCharacterInfo.z);
		return;
	}

	if (db_ItemIndex == 2868) // 스킬 마스터 NPC 이용카드
	{
		pUIManager->GetSkillLearn()->PriorityOpen(pItemData->GetNum0(), FALSE, TRUE);
		return;
	}

	if( db_ItemIndex == INVEN_CASH_KEY1 || db_ItemIndex == INVEN_CASH_KEY2 )
	{
		pUIManager->GetInvenCashBagBox()->UseItemMessage(db_ItemIndex - INVEN_CASH_KEY1);	// 0 or 1
		return;
	}

	if ( db_ItemIndex == 10421 ) // 출석 유지 아이템
	{
		CTString strMessage;
		strMessage.PrintF(_S(6314, "%s  이 적용되어 연속 출석 일 수가 저장되었습니다."), pItemData->GetName() );
		pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL);
	}

	// [2012/01/16 : Sora] 마트로시카 define제거
	//#ifdef MATRYOSHKA
	if ( pItemData->GetFlag()&ITEM_FLAG_BOX && pItemData->GetNum1() > 0 )
	{
		pUIManager->SetShowAni( TRUE );
		pUIManager->HUD_SetItemModelData( db_ItemIndex );
	}
	//#endif
	// 포션이 아니라면 CoolTime 처리를 하지 않음.
	// [2010/10/20 : Sora] 몬스터 용병 카드
	if( pItemData->GetType() != CItemData::ITEM_POTION 
		&& (pItemData->GetSubType() != CItemData::ITEM_SUB_TARGET) 
		//광속 아이템의 경우 사용한 시간을 저장해서 버튼의 쿨타임을 그리는데 사용하기 위해 리턴하지 않음...
		&&(db_ItemIndex!=2407 && db_ItemIndex!=2408 && db_ItemIndex!=2609 && db_ItemIndex!=5018 && db_ItemIndex!=5019 && db_ItemIndex!=2500)
		&& ( pItemData->GetType() != CItemData::ITEM_SUB_ETC && pItemData->GetSubType() != CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD ) )
		return;

	pUIManager->GetQuickSlot()->StartSkillDelay(pItemData->GetNum0());
	pUIManager->GetInventory()->StartSkillDelay(pItemData->GetNum0());

	pItemData->StartTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
}

void CSessionState::updateItemUseError( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	CTString strTitle;
	CTString strMessage1;
	CUIMsgBox_Info	MsgBoxInfo;

	ResponseClient::doItemNotuse* pPack = reinterpret_cast<ResponseClient::doItemNotuse*>(istr->GetBuffer());

	switch (pPack->error_type)
	{
	case MSG_ITEM_USE_ERROR_REVIVAL_EXP:	// 부활 주문서 때문에 경험치 복구 주문서 사용 할수 없음
		//Client 자체적으로 처리함 buff의 상태를 체크하여 처리
		break;	
	case MSG_ITEM_USE_ERROR_REVIVAL_SP :			// 부활 주문서 때문에 숙련도 복구 주문서를 사용할수 없음
		//Client 자체적으로 처리함 buff의 상태를 체크하여 처리
		break;
	case MSG_ITEM_USE_ERROR_SUMMON_NOTALLOWZONE : // 소환 불가능한 상태나 지역입니다.
		strTitle	=	_S(191,"확인");
		strMessage1	=	_S( 2130, "소환 불가능한 지역 또는 상태입니다" ); 
		MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
		MsgBoxInfo.AddString(strMessage1);
		pUIManager->CreateMessageBox(MsgBoxInfo);
		break;
	case MSG_ITEM_USE_ERROR_PLATINUM_SPECIAL:
		strTitle	=	_S(191,"확인");
		strMessage1	=	_S( 2728, "플래티늄 제련석 감정이 완료 되었습니다." ); 
		MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
		MsgBoxInfo.AddString(strMessage1);
		pUIManager->CreateMessageBox(MsgBoxInfo);
		break;
	case MSG_ITEM_USE_ERROR_PLATINUM_SPECIAL_EXPIRE:
		strTitle	=	_S(191,"확인");		
		strMessage1.PrintF( _S( 2738, "[%s]플래티늄 아이템의 사용기간이 만료되어 효과가 사라집니다."), CItemData::getData(pPack->index)->GetName());
		MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
		MsgBoxInfo.AddString(strMessage1);
		pUIManager->CreateMessageBox(MsgBoxInfo);
		break;
	case MSG_ITEM_USE_ERROR_EXPUP: // 같은 스킬의 아이템 사용시 사용 확인
		{
			CItems& Items = _pNetwork->MySlotItem[pPack->tab][pPack->invenIndex];

			if( pPack->index == Items.Item_UniIndex )
			{
				// 아이리스의 축복
				strTitle = _S( 2088, "사용 확인" );
				strMessage1.PrintF( _S( 2638, "현재 경험의묘약을 복용중입니다.만약 아이리스의 축복을 사용하면 그 시간동안 경험의 묘약 효과가 적용되지 않습니다" ) );

				MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_YESNO, UI_NONE, MSGCMD_COMFIRM_USE_ITEM );
				MsgBoxInfo.AddString( strMessage1 );
				
				pUIManager->CreateMessageBox( MsgBoxInfo );
					pUIManager->GetMessageBox(MSGCMD_COMFIRM_USE_ITEM)->SetInvenUseItemInfo( pPack->tab, pPack->invenIndex );
			}
		}
		break;
	case MSG_ITEM_USE_ERROR_IDENTIFY_RARE_OK:
		{
			strTitle	=	_S(191,"확인");
			strMessage1	=	_S(3163, "옵션이 성공적으로 감정되었습니다." ); 
			MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
			MsgBoxInfo.AddString(strMessage1);
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_ITEM_USE_ERROR_IDENTIFY_RARE_FAIL:
		{
			strTitle	=	_S(191,"확인");
			strMessage1	=	_S(3164, "옵션 감정이 실패하였습니다." ); 
			MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
			MsgBoxInfo.AddString(strMessage1);
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_ITEM_USE_ERROR_WARN_PARTYRECALL:
		{
			strTitle = _S(191, "확인");
			strMessage1 = _S(3069, "공성중에 성안이나 성인근에서는 같은 길드원만 리콜할 수 있습니다.");
			MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
			MsgBoxInfo.AddString(strMessage1);
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_ITEM_USE_ERROR_SUMMON_GUARD:
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 3596, "소환수가 이미 소환되어 있어 소환수를 소환할 수 없습니다." ), SYSMSG_ERROR );
		}
		break;
	case MSG_ITEM_USE_ERROR_FLYING:
		{
			pUIManager->GetChattingUI()->AddSysMessage(_S(4688, "비행 중에는 사용할 수 없습니다."));
		}
		break;
	case MSG_ITEM_USE_ERROR_PARTY_OR_EXPED:
		{
			pUIManager->GetChattingUI()->AddSysMessage(_S(4752, "원정대중에는 사용할 수 없습니다."));
		}
		break;
	case MSG_ITEM_USE_ERROR_RAIDZONE:
		{
			pUIManager->GetChattingUI()->AddSysMessage(_S(4753, "인스턴트존에서는 사용할 수 없습니다."));
		}
		break;
	case MSG_ITEM_USE_ERROR_LOWLEVEL:
		{
			pUIManager->GetChattingUI()->AddSysMessage(_S(4758, "레벨이 낮아 사용할 수 없습니다."));
		}
		break;
		// NC card error message. [11/13/2009 rumist]
	case MSG_ITEM_USE_ERROR_NS_CARD_USE_OK:
		{
			strTitle = _S(191, "확인");
			strMessage1 = _S(4779, "성공하였습니다. 이제부터 나이트 쉐도우 캐릭터를 생성할 수 있습니다.");
			MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
			MsgBoxInfo.AddString(strMessage1);
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_ITEM_USE_ERROR_NS_CARD_INSUFF:
		{
			strTitle = _S(191, "확인");
			strMessage1 = _S(4780, "더 이상 사용할 수 없는 아이템입니다. 확인후 다시 시도하여 주십시오.");
			MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
			MsgBoxInfo.AddString(strMessage1);
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_ITEM_USE_ERROR_SOCKET_CARD_USE_OK:
		{
			strTitle = _S(191, "확인");
			strMessage1 = _S( 5006, "소켓 정보가 모두 초기화 되었습니다.");
			MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
			MsgBoxInfo.AddString(strMessage1);
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	// [Sora] 길드마크 관련 아이템 사용 메시지 추가
	case MSG_ITEM_USE_ERROR_NOT_SELECT:
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 5514, "사용할 아이템을 선택하지 않았습니다."));
		}
		break;
	case MSG_ITEM_USE_ERROR_NOITEM:
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 5515, "사용할 아이템이 없습니다."));
		}
		break;
	case MSG_ITEM_USE_ERROR_CANNOT_USE_ZONE:	// royal rumble [4/26/2011 rumist]
		{
			pUIManager->GetChattingUI()->AddSysMessage(_S(5413, "현재 존에서는 사용할 수 없습니다."));
		}
		break;
	case MSG_ITEM_USE_ERROR_CANNOT_USE_COMP:
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 5669, "결합주문서를 사용 하실 수 없는 상태입니다."));
			pUIManager->GetMixNew()->CloseMixNew();
		}
		break;
	case MSG_ITEM_USE_ERROR_DUP_COMP_PREFINE:
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 5670, "플래티늄 제련석과 결합주문서는 중복하여 사용 하실 수 없습니다."));
			pUIManager->GetMixNew()->CloseMixNew();
		}
		break;
	case MSG_ITEM_USE_ERROR_EXT_CHAR_SLOT:	// [2012/07/05 : Sora]  캐릭터 슬롯 확장 아이템
		{
			pUIManager->CloseMessageBox( MSGCMD_NULL );

			MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( _S( 5705, "이미 추가 캐릭터 생성 슬롯이 활성화 된 상태 입니다.") );
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_ITEM_USE_ERROR_DEATH_PENALTY:	// [2012/08/10 : Sora] 부활 주문서 사망 패널티 복구
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 5770, "캐릭터가 사망 패널티를 받지 않았습니다. ") );
		}
		break;
	case MSG_ITEM_USE_ERROR_GUARDIAN:	// [2012/12/05 : Sora] 후견인 관계가 있을 경우 아이템 사용 불가(27)
		{
			pUIManager->CloseMessageBox( MSGCMD_NULL );

			MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( _S( 5818, "후견인이 지정되어 있으면 점핑 포션을 사용할 수 없습니다. 후견인을 삭제하고 다시 시도해 주세요") );
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_ITEM_USE_DO_NOT_MEET_CONDITION:
		{
			CTString str;
			
			// 강신해제 직후 변신주문서를 사용할 경우 에러메시지
			if (_pNetwork->MyCharacterInfo.eMorphStatus == CNetworkLibrary::MyChaInfo::eMORPH_TRANSFORMATION_BEGIN)
			{
				_pNetwork->Set_MyChar_MorphStatus_MORPH_END();
				str.PrintF(_S(2585, "강신 중에는 변신할 수 없습니다."));
			}
			else
			{
				str.PrintF(_S(5312, "아이템을 사용할 수 없습니다."));
			}

			pUIManager->GetChattingUI()->AddSysMessage(str, SYSMSG_ERROR);

			if (pUIManager->GetQuickSlot()->GetWearingLock() == TRUE)
			{
				pUIManager->GetQuickSlot()->SetWearingLock(FALSE);
				pUIManager->SetCSFlagOffElapsed(CSF_ITEMWEARING);
			}
		}
		break;
	case MSG_ITEM_USE_PVP_PROTECT_SUCCESS:
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 6348, "PVP보호 상태가 되어 PVP를 할 수 없고 PVP공격을 받지 않게 되었습니다.") );
		}
		break;
	}

}

void CSessionState::updateMoneyReason( CNetworkMessage* istr )
{
	UpdateClient::moneyDescReason* pPack = reinterpret_cast<UpdateClient::moneyDescReason*>(istr->GetBuffer());

	CTString	strSysMessage;

	switch (pPack->reason)
	{
	case NAS_DESC_REASON_GUILD_KEEP:
	case NAS_DESC_REASON_STASH_KEEP:
		{
			strSysMessage.PrintF( _S( 1346, "나스를 %I64d개 보관하였습니다." ), pPack->nas ); // 번역 수정
			_pNetwork->ClientSystemMessage( strSysMessage );	
		}
		break;
	}
}

void CSessionState::updateOldTimerItem( CNetworkMessage* istr )
{
	UpdateClient::oldTimerItem* pPack = reinterpret_cast<UpdateClient::oldTimerItem*>(istr->GetBuffer());

	CUIManager* pUIMgr = CUIManager::getSingleton();

	if ( pPack->memposTime > 0 )
		pUIMgr->GetTeleport()->SetUseTime( pPack->memposTime );

	if ( pPack->stashextTime > 0)
		pUIMgr->GetWareHouse()->SetUseTime( pPack->stashextTime );
}

void CSessionState::updateEraseAllItem( CNetworkMessage* istr )
{
	UpdateClient::eraseAllItem * pPack = reinterpret_cast<UpdateClient::eraseAllItem *>(istr->GetBuffer());

	int tab_index = pPack->tab_no;

	if (tab_index < INVENTORY_TAB_MAX && tab_index != INVENTORY_TAB_STASH)
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		int		i, max;

		switch(tab_index)
		{		
		case INVENTORY_TAB_CASH_1:
			max = ITEM_COUNT_IN_INVENTORY_CASH_1;
			break;
		case INVENTORY_TAB_CASH_2:
			max = ITEM_COUNT_IN_INVENTORY_CASH_2;
			break;
		//case INVENTORY_TAB_NORMAL:
		default:
			max = ITEM_COUNT_IN_INVENTORY_NORMAL;
			break;
		}

		for (i = 0; i < max; ++i)
		{
			_pNetwork->MySlotItem[tab_index][i].Init();
			pUIManager->GetInventory()->InitInventory( tab_index, i, -1, -1, -1 );
			setNewItemEffect(tab_index, i, FALSE);
		}
	}	
}

void CSessionState::setNewItemEffect( int tabId, int InvenIdx, BOOL bOnOff )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if ( tabId == INVENTORY_TAB_NORMAL)
	{
		int NormalTab = INVENTORY_TAB_NORMAL;

		if ( InvenIdx >= INVEN_ONE_BAG )
		{
			NormalTab = (InvenIdx / INVEN_ONE_BAG) - 1;
			int nUIIdx = 0;
			int nInvenIdx = InvenIdx % INVEN_ONE_BAG;

			switch (NormalTab)
			{
			case INVENSLOT_NUM1:
				nUIIdx = UI_INVEN_SLOT1;
				break;
			case INVENSLOT_NUM2:
				nUIIdx = UI_INVEN_SLOT2;
				break;
			case INVENSLOT_NUM3:
				nUIIdx = UI_INVEN_SLOT3;
				break;
			}

			((CUIChildInvenSlot*)pUIManager->GetUI(nUIIdx))->SetNewItemEffect(bOnOff, nInvenIdx);

			pUIManager->GetInventory()->SetNewItemBagEffect(bOnOff, (eInvenSlot)NormalTab);
		}
		else
		{
			pUIManager->GetInventory()->SetNewItemEffect(bOnOff, InvenIdx);
		}
	}
	else if ( tabId == INVENTORY_TAB_CASH_1)
	{
		((CUIChildInvenSlot*)pUIManager->GetUI(UI_INVEN_CASH1))->SetNewItemEffect(bOnOff, InvenIdx);
		pUIManager->GetInventory()->SetNewItemBagEffect(bOnOff, INVENSLOT_CASH1);
	}
	else if ( tabId == INVENTORY_TAB_CASH_2)
	{
		((CUIChildInvenSlot*)pUIManager->GetUI(UI_INVEN_CASH2))->SetNewItemEffect(bOnOff, InvenIdx);
		pUIManager->GetInventory()->SetNewItemBagEffect(bOnOff, INVENSLOT_CASH2);
	}

	pUIManager->GetInventory()->SetIconNewItem(tabId, InvenIdx, bOnOff > 0 ? true : false);
}

///////////////////// Wear Error
void CSessionState::ReceiveItemWearError( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	ResponseClient::doItemWearError* pPack = reinterpret_cast<ResponseClient::doItemWearError*>(istr->GetBuffer());

	if (pPack->errorCode > WEAR_ERR_OK)
	{
		CTString strError;
		switch (pPack->errorCode)
		{
		case WEAR_ERR_INVALID_POS:
			break;
		case WEAR_ERR_CANNOT_TAKEOFF:
		case WEAR_ERR_NOT_ENOUGH_INVEN:
		case WEAR_ERR_INVALID_ITEM_INFO:
			{
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
				MsgBoxInfo.AddString( _S( 265, "인벤토리 공간이 부족합니다." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			return;
		case WEAR_ERR_CANNOT_WEAR:
			strError.PrintF( _S( 305, "장비를 착용할 수 없습니다." ) );
			break;
		case WEAR_ERR_CANNOT_SWAP_ITEM_CASH_INVEN:
			strError.PrintF( _S( 6276, "아이리스 가방의 기간이 만료되어, 선택 하신 장비를 교체 할 수 없습니다. 현재 착용 중인 장비를 먼저 해제 하신 후 착용해 주시기 바랍니다.") );
			break;
		}

		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage(strError, SYSMSG_ERROR);
	}

	pUIManager->GetQuickSlot()->SetWearingLock(FALSE);
}

///////////////////// Wear Item Update

void CSessionState::updateWearItemUsed( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeWearItemUsed* pPack = reinterpret_cast<UpdateClient::changeWearItemUsed*>(istr->GetBuffer());

	if (pPack->wearPos < 0 || pPack->wearPos >= WEAR_TOTAL)
		return;

	CItems* pItems = NULL;
	pItems = &_pNetwork->MyWearItem[pPack->wearPos];
	pItems->Item_Used = pPack->used;
}

void CSessionState::updateWearItemPlus( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeWearItemPlus* pPack = reinterpret_cast<UpdateClient::changeWearItemPlus*>(istr->GetBuffer());

	if (pPack->wearPos < 0 || pPack->wearPos >= WEAR_TOTAL)
		return;

	CItems* pItems = NULL;
	pItems = &_pNetwork->MyWearItem[pPack->wearPos];
	pItems->Item_Plus = pPack->plus;
}

void CSessionState::updateWearItemFlag( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	UpdateClient::changeWearItemFlag* pPack = reinterpret_cast<UpdateClient::changeWearItemFlag*>(istr->GetBuffer());

	if (pPack->wearPos < 0 || pPack->wearPos >= WEAR_TOTAL)
		return;

	CItems* pItems = NULL;
	pItems = &_pNetwork->MyWearItem[pPack->wearPos];
	pItems->Item_Flag = pPack->flag;
}


///////////////////// Wear
void CSessionState::updateItemTakeOff( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	CTString strMessage1;		

	UpdateClient::doItemWearTakeOff* pPack = reinterpret_cast<UpdateClient::doItemWearTakeOff*>(istr->GetBuffer());

	// 장비 착용 시도 상태 해제
	CUIManager::getSingleton()->SetCSFlagOffElapsed(CSF_ITEMWEARING);

	INDEX iCurCosIndex = _pNetwork->MyWearCostItem[pPack->wearPos].Item_Index;

	if (pPack->wearPos == WEAR_BACKWING)
		iCurCosIndex = _pNetwork->MyWearCostItem[WEAR_COSTUME_BACKWING].Item_Index;

	CModelInstance *pMI			= NULL;
	INDEX iCurWearIndex = -1;

	if( _pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide)
	{
		CModelInstance *pMITemp = penPlayerEntity->GetModelInstance();

		INDEX ctmi = pMITemp->mi_cmiChildren.Count();
		if( ctmi > 0 )
			pMI = &pMITemp->mi_cmiChildren[0];
		else
			pMI	= penPlayerEntity->GetModelInstance();
	}
	else
	{
		pMI	= penPlayerEntity->GetModelInstance();
	}	

	ASSERT( pMI != NULL && "Invalid Model Instance!!!" );

	// 생산중에 장비를 벗으면 생산을 중단 한다.
	if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsProduct())
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();

	CItemData* pOffItemData	= _pNetwork->MyWearItem[pPack->wearPos].ItemData;
	CItemData* pCosItem = _pNetwork->GetItemData(iCurCosIndex);

	if (pOffItemData == NULL)
		return;

	bool bDeleteRelicEffect = false;

	if( pOffItemData->GetType() == CItemData::ITEM_ACCESSORY && 
		pOffItemData->GetSubType() == CItemData::ACCESSORY_RELIC )
	{
		bDeleteRelicEffect = true;
	}

	if( pOffItemData->GetType() == CItemData::ITEM_ACCESSORY && 
		pOffItemData->GetSubType() == CItemData::ACCESSORY_PET )
	{					
		// 펫을 타고 있으면, 펫에서 내리도록 처리.
		_pNetwork->LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );
		pMI	= penPlayerEntity->GetModelInstance();
		MY_PET_INFO()->Init();
		pUIManager->GetPetInfo()->ClosePetInfo();
	}
	else if(pOffItemData->GetType() == CItemData::ITEM_ACCESSORY && 
		pOffItemData->GetSubType() == CItemData::ACCESSORY_WILDPET)
	{
		INFO()->SetMyApet(NULL);
		pUIManager->GetWildPetInfo()->CloseWildPetInfo();					
	}
	// 강신중이고, 무기를 벗으려고 한다면...
	else if( _pNetwork->MyCharacterInfo.nEvocationIndex > 0 && 
		pOffItemData->GetType() == CItemData::ITEM_WEAPON )
	{
		//_pNetwork->SendStopEvocation();
	}
	// 아이템 텍스쳐 관련. [8/13/2010 rumist]
	// 투명 코스튬 처리 추가.
	else if( _pNetwork->MyCharacterInfo.nEvocationIndex == 0 && 
		(iCurCosIndex <= 0 || pCosItem->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == true))
	{
		penPlayerEntity->DeleteCurrentArmor(pPack->wearPos);
		penPlayerEntity->WearingDefaultArmor(pPack->wearPos);
	}

	const char* szItemName = _pNetwork->GetItemName(pOffItemData->GetItemIndex());
	strMessage1.PrintF( _S2( 388, szItemName, "%s<를> 벗었습니다." ), szItemName );
	pUIManager->GetChattingUI()->AddSysMessage( strMessage1 );

	if ( pPack->wearPos == WEAR_WEAPON )
	{
		_pNetwork->MyCharacterInfo.bExtension = FALSE;
	}
	
	if ( pPack->wearPos == WEAR_PET ) // 장착 위치가 펫일때
	{
		MY_INFO()->ClearWildPetSkill();

		const INDEX iIndex = _pNetwork->MyWearItem[pPack->wearPos].Item_Plus;
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_WILDPET, iIndex);

		if (pObject != NULL)
		{
			CWildPetTarget* pTarget = static_cast< CWildPetTarget* >(pObject);
			ACTORMGR()->RemoveObject(eOBJ_WILDPET, pTarget->GetSIndex());
		}

		CItemData* pItemData = _pNetwork->MyWearItem[pPack->wearPos].ItemData;

		if (pItemData->GetType() == CItemData::ITEM_ACCESSORY)
		{
			if (pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET)
				pUIManager->GetQuickSlot()->RemoveWildPetSkill();
			else if (pItemData->GetSubType() == CItemData::ACCESSORY_PET)
				pUIManager->GetQuickSlot()->RemovePetSkill();			
		}			
	}

	_pNetwork->MyWearItem[pPack->wearPos].Init();
	pUIManager->GetInventory()->InitWearBtn(-1, -1, pPack->wearPos);

	const int iJob				= _pNetwork->MyCharacterInfo.job;
	iCurWearIndex = pOffItemData->GetItemIndex();

	if (iCurCosIndex > 0)
	{
		iCurWearIndex = iCurCosIndex;
	}	

	_pNetwork->MyCharacterInfo.itemEffect.Change( iJob
		, _pNetwork->GetItemData(iCurWearIndex)
		, pPack->wearPos
		, iCurCosIndex < 0 ? -1 : 0
		, &pMI->m_tmSkaTagManager
		, 1, _pNetwork->GetItemData(iCurWearIndex)->GetSubType() );

	if (bDeleteRelicEffect == true)
	{
		_pNetwork->MyCharacterInfo.itemEffect.DeleteRelicEffect();

		int nEffectType = INFO()->GetShowAnotherRelicEffect();

		if (nEffectType >= 0)
		{
			_pNetwork->MyCharacterInfo.itemEffect.AddRelicEffect(nEffectType, &pMI->m_tmSkaTagManager);
		}		
	}

	_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
	_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
}

void CSessionState::updateWearItemList( CNetworkMessage* istr )
{
	int		i;
	UpdateClient::itemInfo* pInfo;
	CUIManager* pUIMgr = CUIManager::getSingleton();

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	UpdateClient::doItemWearList* pPack = reinterpret_cast<UpdateClient::doItemWearList*>(istr->GetBuffer());

	for (i = 0; i < pPack->count; ++i)
	{
		pInfo = &pPack->info_list[i];

		if (pInfo == NULL)
			continue;

		setWearItemInfo(pInfo);		

		CModelInstance *pMI			= NULL;
		// wildpet or pet 탑승일시 모델은 child로. [6/15/2011 rumist]
		if (_pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide)
		{
			CModelInstance *pMITemp = penPlayerEntity->GetModelInstance();

			INDEX ctmi = pMITemp->mi_cmiChildren.Count();
			if( ctmi > 0 )
				pMI = &pMITemp->mi_cmiChildren[0];
			else
				pMI	= penPlayerEntity->GetModelInstance();
		}
		else
		{
			pMI	= penPlayerEntity->GetModelInstance();
		}

		CItemData* pItemData = _pNetwork->GetItemData(pInfo->dbIndex);

		//INDEX PlayerType = _pNetwork->ga_srvServer.srv_apltPlayers[0].plt_penPlayerEntity->en_pcCharacter.pc_iPlayerType;//타이탄:0,나이트:1,healer:2 메이지:3	
		if(pInfo->wearPos != -1 && _pNetwork->MyCharacterInfo.nEvocationIndex == 0)
		{
			int WearPos = pItemData->GetWearingPosition();
			INDEX iCurWearIndex = -1;
			if ( !(pItemData->GetFlag()&ITEM_FLAG_COSTUME2))
			{
				INDEX iCosWearIndex = _pNetwork->MyWearCostItem[WearPos].Item_Index;
				if (WearPos == WEAR_BACKWING)
					iCosWearIndex = _pNetwork->MyWearCostItem[WEAR_COSTUME_BACKWING].Item_Index;

				if (iCosWearIndex < 0)
				{
					if (!(pItemData->GetWearingPosition() == WEAR_HELMET && (CTString)pItemData->GetItemSmcFileName() == MODEL_TREASURE))
					{
						penPlayerEntity->DeleteCurrentArmor(pInfo->wearPos);//1005 아이템 깨지는 버그수정
						penPlayerEntity->DeleteDefaultArmor(pInfo->wearPos);
					}

					if(!(pItemData->GetWearingPosition() == WEAR_HELMET && (CTString)pItemData->GetItemSmcFileName() == MODEL_TREASURE))
					{
						_pGameState->WearingArmor( pMI, *pItemData );
					}
					iCurWearIndex = pInfo->dbIndex;
				}
				else
				{
					iCurWearIndex = iCosWearIndex;
				}
				// Date : 2005-04-07(오후 3:39:57), By Lee Ki-hwan
				_pNetwork->MyCharacterInfo.itemEffect.Change(_pNetwork->MyCharacterInfo.job
					, _pNetwork->GetItemData(iCurWearIndex)
					, pInfo->wearPos
					, pInfo->plus
					, &pMI->m_tmSkaTagManager
					, 1, _pNetwork->GetItemData(iCurWearIndex)->GetSubType() );
			}
		}

		pUIMgr->GetInventory()->InitWearBtn( pInfo->dbIndex, pInfo->virtualIndex, pInfo->wearPos);

		if( pItemData->GetType() == CItemData::ITEM_ACCESSORY && 
			pItemData->GetSubType() == CItemData::ACCESSORY_RELIC )
		{
			int nRelicType = -1;

			switch(pItemData->getindex())
			{
			case 10951:
				nRelicType = 0;
				break;
			case 10952:
				nRelicType = 1;
				break;
			case 10953:
				nRelicType = 2;
				break;
			default:
				return;
			}

			_pNetwork->MyCharacterInfo.itemEffect.AddRelicEffect(nRelicType, &penPlayerEntity->GetModelInstance()->m_tmSkaTagManager);
		}

		_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
		_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
	}
}

void CSessionState::updateItemWear( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	CTString strMessage1;		

	UpdateClient::doItemWear* pPack = reinterpret_cast<UpdateClient::doItemWear*>(istr->GetBuffer());

	setWearItemInfo(&pPack->info);
	int		wear_type = pPack->info.wearPos;
	CItemData* pWearItemData = _pNetwork->MyWearItem[wear_type].ItemData;

	if (pWearItemData == NULL)
		return;

	pUIManager->GetInventory()->InitWearBtn(pPack->info.dbIndex, pPack->info.virtualIndex, wear_type);

	// 장비 착용 시도 상태 해제
	CUIManager::getSingleton()->SetCSFlagOffElapsed(CSF_ITEMWEARING);	

	INDEX iCurCosIndex = _pNetwork->MyWearCostItem[wear_type].Item_Index;

	if (wear_type == WEAR_BACKWING)
		iCurCosIndex = _pNetwork->MyWearCostItem[WEAR_COSTUME_BACKWING].Item_Index;

	CItemData* pCurCosItem = _pNetwork->GetItemData(iCurCosIndex);

	CModelInstance *pMI			= NULL;

	INDEX iCurWearIndex = -1;

	if( _pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide)
	{
		CModelInstance *pMITemp = penPlayerEntity->GetModelInstance();

		INDEX ctmi = pMITemp->mi_cmiChildren.Count();
		if( ctmi > 0 )
			pMI = &pMITemp->mi_cmiChildren[0];
		else
			pMI	= penPlayerEntity->GetModelInstance();
	}
	else
	{
		pMI	= penPlayerEntity->GetModelInstance();
	}			
	const int iJob				= _pNetwork->MyCharacterInfo.job;

	ASSERT( pMI != NULL && "Invalid Model Instance!!!" );	

	// 펫을 장착한 경우....
	if( pWearItemData->GetType() == CItemData::ITEM_ACCESSORY && 
		(pWearItemData->GetSubType() == CItemData::ACCESSORY_PET || pWearItemData->GetSubType() == CItemData::ACCESSORY_WILDPET))
	{
		const INDEX iPetIndex = _pNetwork->MyWearItem[wear_type].Item_Plus;

		if( pWearItemData->GetSubType() != CItemData::ACCESSORY_WILDPET)
		{
			CPetTargetInfom* pPetInfo = INFO()->GetMyPetInfo();
			pPetInfo->lIndex		= iPetIndex;					
			pPetInfo->bIsActive		= TRUE;
			_pNetwork->UpdatePetTargetInfo( iPetIndex );
		}
	}
	else if( _pNetwork->MyCharacterInfo.nEvocationIndex == 0 &&
		(iCurCosIndex <= 0 || pCurCosItem->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == true)) // 코스튬이 없거나 투명 코스튬일때.
	{
		if(wear_type != -1)
		{								
			penPlayerEntity->DeleteDefaultArmor(wear_type);
		}

		if ((CTString)pWearItemData->GetItemSmcFileName() != MODEL_TREASURE)
		{
			_pGameState->WearingArmor( pMI, *pWearItemData );
		}
		else if ((CTString)pWearItemData->GetItemSmcFileName() == MODEL_TREASURE && wear_type == WEAR_HELMET)
		{
			penPlayerEntity->ChangeHairMesh(pMI, _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.hairStyle - 1);
		}
	}

	CItems&	WearNormalItem = _pNetwork->MyWearItem[wear_type];

	WearNormalItem.Item_Wearing = wear_type;

	const char* szItemName = _pNetwork->GetItemName(pWearItemData->GetItemIndex());
	strMessage1.PrintF( _S2( 387, szItemName, "%s<를> 착용했습니다." ), szItemName );
	pUIManager->GetChattingUI()->AddSysMessage( strMessage1 );

	iCurWearIndex = pWearItemData->GetItemIndex();

	if (iCurCosIndex >= 0)
	{
		iCurWearIndex = iCurCosIndex;
	}

	if(!((static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->IsSitting() && _pNetwork->MyCharacterInfo.job == NIGHTSHADOW))
	{
		_pNetwork->MyCharacterInfo.itemEffect.Change( iJob
			, _pNetwork->GetItemData(iCurWearIndex)
			, wear_type
			, WearNormalItem.Item_Plus
			, &pMI->m_tmSkaTagManager
			, 1, _pNetwork->GetItemData(iCurWearIndex)->GetSubType() );
		_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
		_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
	}

	if( pWearItemData->GetType() == CItemData::ITEM_ACCESSORY && 
		pWearItemData->GetSubType() == CItemData::ACCESSORY_RELIC )
	{
		int nRelicType = -1;

		switch(pWearItemData->getindex())
		{
		case 10951:
			nRelicType = 0;
			break;
		case 10952:
			nRelicType = 1;
			break;
		case 10953:
			nRelicType = 2;
			break;
		default:
			return;
		}

		_pNetwork->MyCharacterInfo.itemEffect.AddRelicEffect(nRelicType, &penPlayerEntity->GetModelInstance()->m_tmSkaTagManager);
	}
}

void CSessionState::updateWearItemInfo( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	UpdateClient::doItemWearChangeInfo* pPack = reinterpret_cast<UpdateClient::doItemWearChangeInfo*>(istr->GetBuffer());
	
	setWearItemInfo(&pPack->info);
	pUIManager->GetInventory()->InitWearBtn(pPack->info.dbIndex, pPack->info.virtualIndex, pPack->info.wearPos);

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	CModelInstance *pMI			= NULL;
	if( _pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide)
	{
		CModelInstance *pMITemp = penPlayerEntity->GetModelInstance();

		INDEX ctmi = pMITemp->mi_cmiChildren.Count();
		if( ctmi > 0 )
			pMI = &pMITemp->mi_cmiChildren[0];
		else
			pMI	= penPlayerEntity->GetModelInstance();
	}
	else
	{
		pMI	= penPlayerEntity->GetModelInstance();
	}			

	if(!((static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->IsSitting() && _pNetwork->MyCharacterInfo.job == NIGHTSHADOW))
	{
		_pNetwork->MyCharacterInfo.itemEffect.Change( _pNetwork->MyCharacterInfo.job
			, _pNetwork->GetItemData(pPack->info.dbIndex)
			, pPack->info.wearPos
			, pPack->info.plus
			, &pMI->m_tmSkaTagManager
			, 1, _pNetwork->GetItemData(pPack->info.dbIndex)->GetSubType() );
		_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
		_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
	}
}

///////////////////// Wear Cost
void CSessionState::updateCostWear( CNetworkMessage* istr )
{
	CTString strMessage;		
	
	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	UpdateClient::doItemWearCostume* pPack = reinterpret_cast<UpdateClient::doItemWearCostume*>(istr->GetBuffer());

	setWearCostItemInfo(&pPack->info);

	// 장비 착용 시도 상태 해제
	CUIManager::getSingleton()->SetCSFlagOffElapsed(CSF_ITEMWEARING);

	SBYTE	wear_type = pPack->info.wearPos; // 입는 위치
	SLONG	wear_index = pPack->info.dbIndex; // 인덱스

	if ( pPack->info.wearPos == WEAR_BACKWING )
		wear_type = WEAR_COSTUME_BACKWING;

	CUIManager* pUIManager = CUIManager::getSingleton();

	BOOL bNotWear = FALSE;
	int iItemPlus = 0;
	
	CItems* CurWear = &_pNetwork->MyWearItem[pPack->info.wearPos];
	CItemData* pItems = _pNetwork->GetItemData(wear_index);

	if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsProduct())
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();

	CModelInstance *pMI			= NULL;

	if( _pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide)
	{
		CModelInstance *pMITemp = penPlayerEntity->GetModelInstance();

		INDEX ctmi = pMITemp->mi_cmiChildren.Count();
		if( ctmi > 0 )
			pMI = &pMITemp->mi_cmiChildren[0];
		else
			pMI	= penPlayerEntity->GetModelInstance();
	}
	else
	{
		pMI	= penPlayerEntity->GetModelInstance();
	}

	// 강신이 아니고 투명 코스튬이 아닐때.
	if( _pNetwork->MyCharacterInfo.nEvocationIndex == 0 &&
		pItems->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
	{
		if (CurWear->ItemData != NULL)
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->DeleteCurrentArmor(pPack->info.wearPos);
			bNotWear = TRUE;
		}
		else
		{
			_pGameState->DeleteDefaultArmor(pMI, pPack->info.wearPos, _pNetwork->MyCharacterInfo.job);
			bNotWear = TRUE;
		}
	}
	
	// 강신이라면 bNotWear이 FALSE;
	if (bNotWear)
	{
		if ((CTString)pItems->GetItemSmcFileName() != MODEL_TREASURE)
		{
			_pGameState->WearingArmor( pMI, *pItems );
		}
		else if ((CTString)pItems->GetItemSmcFileName() == MODEL_TREASURE && pPack->info.wearPos == WEAR_HELMET)
		{
			penPlayerEntity->ChangeHairMesh(pMI, _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.hairStyle - 1);
		}
	}

	if ( CurWear->ItemData != NULL )
	{
		iItemPlus = CurWear->Item_Plus;	// 코스튬 장비는 Plus 수치가 0
	}

	if (wear_index >= 0)
	{
		pUIManager->GetInventory()->InitCostumeInventory(pPack->info.virtualIndex, wear_index, wear_type);
		const char* szItemName = _pNetwork->GetItemName(wear_index);
		strMessage.PrintF( _S2( 387, szItemName, "%s<를> 착용했습니다." ), szItemName );
		pUIManager->GetChattingUI()->AddSysMessage( strMessage );
	}

	if(!((static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->IsSitting() && _pNetwork->MyCharacterInfo.job == NIGHTSHADOW))
	{
		_pNetwork->MyCharacterInfo.itemEffect.Change( _pNetwork->MyCharacterInfo.job
			, _pNetwork->GetItemData(wear_index)
			, pPack->info.wearPos
			, iItemPlus
			, &pMI->m_tmSkaTagManager
			, 1, _pNetwork->GetItemData(wear_index)->GetSubType() );
		_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
		_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
	}

	pUIManager->GetInventory()->SetInventoryType( INVEN_TAB_COSTUME );
}

void CSessionState::updateCostTakeOff( CNetworkMessage* istr )
{
	CTString strMessage;		

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	UpdateClient::doItemWearCostumeTakeOff* pPack = reinterpret_cast<UpdateClient::doItemWearCostumeTakeOff*>(istr->GetBuffer());
		
	// 장비 착용 시도 상태 해제
	CUIManager::getSingleton()->SetCSFlagOffElapsed(CSF_ITEMWEARING);

	SBYTE	wear_type = pPack->wearPos; // 입는 위치

	if ( pPack->wearPos == WEAR_BACKWING )
		wear_type = WEAR_COSTUME_BACKWING;
	
	CUIManager* pUIManager = CUIManager::getSingleton();

	int iItemPlus = -1;
	int iRealTakeoffIndex = 0;
	int nTakeOffIdx = 0;

	INDEX iCosIndex = _pNetwork->MyWearCostItem[wear_type].Item_Index;
	CItems* CurWear = &_pNetwork->MyWearItem[pPack->wearPos];

	if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsProduct())
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();

	CModelInstance *pMI			= NULL;

	if( _pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide)
	{
		CModelInstance *pMITemp = penPlayerEntity->GetModelInstance();

		INDEX ctmi = pMITemp->mi_cmiChildren.Count();
		if( ctmi > 0 )
			pMI = &pMITemp->mi_cmiChildren[0];
		else
			pMI	= penPlayerEntity->GetModelInstance();
	}
	else
	{
		pMI	= penPlayerEntity->GetModelInstance();
	}
	
	CItemData* pTakeOffItemData = _pNetwork->GetItemData(iCosIndex);

	// 투명 코스튬이 아닐때.
	if (pTakeOffItemData->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
	{
		if ( iCosIndex >= 0)
		{
			if ( (CTString)pTakeOffItemData->GetItemSmcFileName() != MODEL_TREASURE )
			{
				_pGameState->TakeOffArmor(pMI, *pTakeOffItemData);
			}
			else if ((CTString)pTakeOffItemData->GetItemSmcFileName() == MODEL_TREASURE && pPack->wearPos == WEAR_HELMET)
			{
				_pGameState->DeleteDefaultArmor(pMI, pPack->wearPos, _pNetwork->MyCharacterInfo.job);
			}
			iRealTakeoffIndex = iCosIndex;
		}
		
		if (CurWear->ItemData != NULL)
		{
			if ((CTString)CurWear->ItemData->GetItemSmcFileName() != MODEL_TREASURE)
			{
				_pGameState->WearingArmor( pMI, *_pNetwork->GetItemData(CurWear->Item_Index) );
			}
			else if ((CTString)CurWear->ItemData->GetItemSmcFileName() == MODEL_TREASURE && pPack->wearPos == WEAR_HELMET)
			{
				penPlayerEntity->ChangeHairMesh(pMI, _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.hairStyle - 1);
			}
	
			iItemPlus = CurWear->Item_Plus;
			iRealTakeoffIndex = CurWear->Item_Index;
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->WearingDefaultArmor(pPack->wearPos);
		}
	}

	nTakeOffIdx = _pNetwork->MyWearCostItem[wear_type].Item_Index;

	const char* szItemName = _pNetwork->GetItemName(nTakeOffIdx);
	strMessage.PrintF( _S2( 388, szItemName, "%s<를> 벗었습니다." ), szItemName );
	pUIManager->GetChattingUI()->AddSysMessage( strMessage );

	_pNetwork->MyWearCostItem[wear_type].Init();
	pUIManager->GetInventory()->InitCostumeInventory(-1, -1, wear_type);
	
	if(!((static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->IsSitting() && _pNetwork->MyCharacterInfo.job == NIGHTSHADOW))
	{
		_pNetwork->MyCharacterInfo.itemEffect.Change( _pNetwork->MyCharacterInfo.job
			, _pNetwork->GetItemData(iRealTakeoffIndex)
			, pPack->wearPos
			, iItemPlus
			, &pMI->m_tmSkaTagManager
			, 1, _pNetwork->GetItemData(iRealTakeoffIndex)->GetSubType() );
		_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
		_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
	}

	pUIManager->GetInventory()->SetInventoryType( INVEN_TAB_COSTUME );
}

void CSessionState::updateWearCostList( CNetworkMessage* istr )
{
	int		i;
	UpdateClient::itemInfo* pInfo;
	CUIManager* pUIMgr = CUIManager::getSingleton();

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	UpdateClient::doItemWearCostumeList* pPack = reinterpret_cast<UpdateClient::doItemWearCostumeList*>(istr->GetBuffer());

	for (i = 0; i < pPack->count; ++i)
	{
		pInfo = &pPack->info_list[i];

		if (pInfo == NULL)
			continue;

		setWearCostItemInfo(pInfo);

		SBYTE wear_pos = pInfo->wearPos;

		if (pInfo->wearPos == WEAR_BACKWING)
			wear_pos = WEAR_COSTUME_BACKWING;
		
		CItemData* pItemData = _pNetwork->GetItemData(pInfo->dbIndex);
		CItems* CurWear = &_pNetwork->MyWearItem[pInfo->wearPos];

		int iItemPlus = 0;

		CModelInstance *pMI			= NULL;

		if( _pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide)
		{
			CModelInstance *pMITemp = penPlayerEntity->GetModelInstance();

			INDEX ctmi = pMITemp->mi_cmiChildren.Count();
			if( ctmi > 0 )
				pMI = &pMITemp->mi_cmiChildren[0];
			else
				pMI	= penPlayerEntity->GetModelInstance();
		}
		else
		{
			pMI	= penPlayerEntity->GetModelInstance();
		}

		if (pItemData->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
		{
			if (CurWear->ItemData != NULL)
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->DeleteCurrentArmor(pInfo->wearPos);
			else
				_pGameState->DeleteDefaultArmor(pMI, pInfo->wearPos, _pNetwork->MyCharacterInfo.job);
			
			CItemData* pItems = _pNetwork->GetItemData(pInfo->dbIndex);
	
			if ((CTString)pItems->GetItemSmcFileName() != MODEL_TREASURE)
			{
				_pGameState->WearingArmor( pMI, *pItems );
			}
			else if ((CTString)pItems->GetItemSmcFileName() == MODEL_TREASURE && pInfo->wearPos == WEAR_HELMET)
			{
				penPlayerEntity->ChangeHairMesh(pMI, _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.hairStyle - 1);
			}
	
			if ( CurWear->ItemData != NULL )
			{
				iItemPlus = CurWear->Item_Plus;	// 코스튬 장비는 Plus 수치가 0
			}
		}
		
		if(!((static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->IsSitting() && _pNetwork->MyCharacterInfo.job == NIGHTSHADOW))
		{
			_pNetwork->MyCharacterInfo.itemEffect.Change( _pNetwork->MyCharacterInfo.job
				, _pNetwork->GetItemData(pInfo->dbIndex)
				, pInfo->wearPos
				, iItemPlus
				, &pMI->m_tmSkaTagManager
				, 1, _pNetwork->GetItemData(pInfo->dbIndex)->GetSubType() );

			_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
			_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
		}

		pUIMgr->GetInventory()->InitCostumeInventory( pInfo->virtualIndex, pInfo->dbIndex, wear_pos);
	}
}

///////////////////// Cost Suit
void CSessionState::updateCostSuitWear( CNetworkMessage* istr )
{
	CTString strMessage;		
	int		i;
	UpdateClient::itemInfo* pInfo;

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	UpdateClient::doItemWearCostumeSuit* pPack = reinterpret_cast<UpdateClient::doItemWearCostumeSuit*>(istr->GetBuffer());

	_pNetwork->MyCharacterInfo.iOneSuitDBIndex	= pPack->DBIndex;
	_pNetwork->MyCharacterInfo.iOneSuitUniIndex	= pPack->VIndex;

	_pNetwork->MyCharacterInfo.bOneSuit = TRUE;

	// 장비 착용 시도 상태 해제
	CUIManager::getSingleton()->SetCSFlagOffElapsed(CSF_ITEMWEARING);
	CUIManager* pUIManager = CUIManager::getSingleton();

	for (i = 0; i < ONE_SUIT_MAX; i++)
	{
		pInfo = &pPack->info_list[i];

		if (pInfo == NULL)
			continue;

		setWearCostItemInfo(pInfo);
		
		SBYTE	wear_type = pInfo->wearPos; // 입는 위치
		SLONG	wear_index = pInfo->dbIndex; // 인덱스

		if (pInfo->wearPos == WEAR_BACKWING)
			wear_type = WEAR_COSTUME_BACKWING;			
		
		BOOL bNotWear = FALSE;
		int iItemPlus = 0;

		CItems* CurWear = &_pNetwork->MyWearItem[pInfo->wearPos];
		CItemData* pItems = _pNetwork->GetItemData(wear_index);

		if (pItems == NULL)
			continue;

		if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsProduct())
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();

		CModelInstance *pMI			= NULL;

		if( _pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide)
		{
			CModelInstance *pMITemp = penPlayerEntity->GetModelInstance();

			INDEX ctmi = pMITemp->mi_cmiChildren.Count();
			if( ctmi > 0 )
				pMI = &pMITemp->mi_cmiChildren[0];
			else
				pMI	= penPlayerEntity->GetModelInstance();
		}
		else
		{
			pMI	= penPlayerEntity->GetModelInstance();
		}

		// 강신이 아니고 투명 코스튬이 아닐때.
		if( _pNetwork->MyCharacterInfo.nEvocationIndex == 0 &&
			pItems->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
		{
			if (CurWear->ItemData != NULL)
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->DeleteCurrentArmor(pInfo->wearPos);
			}
			else
			{
				_pGameState->DeleteDefaultArmor(pMI, pInfo->wearPos, _pNetwork->MyCharacterInfo.job);
			}

			bNotWear = TRUE;
		}

		// 강신일땐 bNotWear가 FALSE
		if (bNotWear)
		{
			if ((CTString)pItems->GetItemSmcFileName() != MODEL_TREASURE)
			{
				_pGameState->WearingArmor( pMI, *pItems );
			}
			else if ((CTString)pItems->GetItemSmcFileName() == MODEL_TREASURE && pInfo->wearPos == WEAR_HELMET)
			{
				penPlayerEntity->ChangeHairMesh(pMI, _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.hairStyle - 1);
			}
		}

		if ( CurWear->ItemData != NULL )
		{
			iItemPlus = CurWear->Item_Plus;	// 코스튬 장비는 Plus 수치가 0
		}
				
		if(!((static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->IsSitting() && _pNetwork->MyCharacterInfo.job == NIGHTSHADOW))
		{
			_pNetwork->MyCharacterInfo.itemEffect.Change( _pNetwork->MyCharacterInfo.job
				, _pNetwork->GetItemData(wear_index)
				, pInfo->wearPos
				, iItemPlus
				, &pMI->m_tmSkaTagManager
				, 1, _pNetwork->GetItemData(wear_index)->GetSubType() );
			_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
			_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
		}
	}

	if (pPack->DBIndex >= 0)
	{
		// 한벌의상 인벤토리 셋팅 (버츄얼 인덱스, 착용 -1 외의 값)
		pUIManager->GetInventory()->InitCostumeInventory(pPack->VIndex, 0, 0);
	}

	const char* szItemName = _pNetwork->GetItemName(pPack->DBIndex);
	strMessage.PrintF( _S2( 387, szItemName, "%s<를> 착용했습니다." ), szItemName );
	pUIManager->GetChattingUI()->AddSysMessage( strMessage );
	
	if ( pUIManager->GetInventory()->IsVisible() != FALSE )
		pUIManager->GetInventory()->SetInventoryType( INVEN_TAB_COSTUME );
}

void CSessionState::ReceiveCostSuitTakeOff()
{
	CTString strMessage;		
	int i;
	CUIManager* pUIManager = CUIManager::getSingleton();

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
	
	// 한벌의상 아이템 제거
	pUIManager->GetInventory()->InitCostumeInventory(0, 0, -1);
	
	// 장비 착용 시도 상태 해제
	CUIManager::getSingleton()->SetCSFlagOffElapsed(CSF_ITEMWEARING);
	
	int iItemPlus = -1;
	int iRealTakeoffIndex = 0;
	int nTakeOffIdx = 0;

	for ( i = 0; i < WEAR_COSTUME_TOTAL; i++ )
	{
		if (i == WEAR_COSTUME_WEAPON || i == WEAR_COSTUME_SHIELD)
			continue;

		SBYTE wear_pos = i;

		if ( i == WEAR_COSTUME_BACKWING)
			wear_pos = WEAR_BACKWING;

		INDEX iCosIndex = _pNetwork->MyWearCostItem[i].Item_Index;
		CItems* CurWear = &_pNetwork->MyWearItem[wear_pos];

		if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsProduct())
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();

		CModelInstance *pMI			= NULL;

		if( _pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide)
		{
			CModelInstance *pMITemp = penPlayerEntity->GetModelInstance();

			INDEX ctmi = pMITemp->mi_cmiChildren.Count();
			if( ctmi > 0 )
				pMI = &pMITemp->mi_cmiChildren[0];
			else
				pMI	= penPlayerEntity->GetModelInstance();
		}
		else
		{
			pMI	= penPlayerEntity->GetModelInstance();
		}

		if ( iCosIndex < 0)
			continue;

		CItemData* pTakeOffItemData = _pNetwork->GetItemData(iCosIndex);

		// 투명 코스튬일 경우에는 건너 뜀.
		if (pTakeOffItemData->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
		{
			if ( (CTString)pTakeOffItemData->GetItemSmcFileName() != MODEL_TREASURE )
			{
				_pGameState->TakeOffArmor(pMI, *pTakeOffItemData);
			}
			else if ((CTString)pTakeOffItemData->GetItemSmcFileName() == MODEL_TREASURE && wear_pos == WEAR_HELMET)
			{
				_pGameState->DeleteDefaultArmor(pMI, wear_pos, _pNetwork->MyCharacterInfo.job);
			}
			iRealTakeoffIndex = iCosIndex;
			
			if (CurWear->ItemData != NULL)
			{
				if ((CTString)CurWear->ItemData->GetItemSmcFileName() != MODEL_TREASURE)
				{
					_pGameState->WearingArmor( pMI, *_pNetwork->GetItemData(CurWear->Item_Index) );
				}
				else if ((CTString)CurWear->ItemData->GetItemSmcFileName() == MODEL_TREASURE && wear_pos == WEAR_HELMET)
				{
					penPlayerEntity->ChangeHairMesh(pMI, _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.hairStyle - 1);
				}
	
				iItemPlus = CurWear->Item_Plus;
				iRealTakeoffIndex = CurWear->Item_Index;
			}
			else
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->WearingDefaultArmor(wear_pos);
			}
		}

		_pNetwork->MyWearCostItem[i].Init();

		if(!((static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->IsSitting() && _pNetwork->MyCharacterInfo.job == NIGHTSHADOW))
		{
			_pNetwork->MyCharacterInfo.itemEffect.Change( _pNetwork->MyCharacterInfo.job
				, _pNetwork->GetItemData(iRealTakeoffIndex)
				, wear_pos
				, iItemPlus
				, &pMI->m_tmSkaTagManager
				, 1, _pNetwork->GetItemData(iRealTakeoffIndex)->GetSubType() );
			_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
			_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
		}
	}

	nTakeOffIdx = _pNetwork->MyCharacterInfo.iOneSuitDBIndex;

	const char* szItemName = _pNetwork->GetItemName(nTakeOffIdx);
	strMessage.PrintF( _S2( 388, szItemName, "%s<를> 벗었습니다." ), szItemName );
	pUIManager->GetChattingUI()->AddSysMessage( strMessage );

	_pNetwork->MyCharacterInfo.iOneSuitDBIndex	= -1;
	_pNetwork->MyCharacterInfo.iOneSuitUniIndex	= -1;

	_pNetwork->MyCharacterInfo.bOneSuit = FALSE;
}

void CSessionState::updateStashPasswordFlag( CNetworkMessage* istr )
{
	UpdateClient::stashPasswordFlag* pPack = reinterpret_cast<UpdateClient::stashPasswordFlag*>(istr->GetBuffer());

	CUIManager::getSingleton()->GetWareHouse()->SetHasPassword( pPack->flag == 1 ? true : false);
}

void CSessionState::updateDurabilityForInventory( CNetworkMessage* istr )
{
#ifdef DURABILITY
	UpdateClient::itemDurabilityForInventory* pPack = reinterpret_cast<UpdateClient::itemDurabilityForInventory*>(istr->GetBuffer());

	if (pPack == NULL)
		return;

	CItems* pItems = NULL;

	pItems = &_pNetwork->MySlotItem[pPack->tab][pPack->invenIndex];

	pItems->Item_durability_now = pPack->now_durability;
	pItems->Item_durability_max = pPack->max_durability;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetQuickSlot()->UpdateItemDurability(pItems->Item_UniIndex, pItems->Item_durability_now, pItems->Item_durability_max);

	pUIManager->GetDurability()->UpdateResultIcon(pPack->tab, pPack->invenIndex);

#endif	//	DURABILITY
}

void CSessionState::updateDurabilityForWear( CNetworkMessage* istr )
{
#ifdef DURABILITY
	UpdateClient::itemDurabilityForWear* pPack = reinterpret_cast<UpdateClient::itemDurabilityForWear*>(istr->GetBuffer());

	if (pPack == NULL)
		return;

	if (pPack->wearPos < 0 || pPack->wearPos >= WEAR_TOTAL)
		return;

	CItems* pItems = NULL;
	pItems = &_pNetwork->MyWearItem[pPack->wearPos];
	pItems->Item_durability_now = pPack->now_durability;

	if (pItems->Item_durability_now / 10 <= 0)
	{
		CTString strMessage;		
		strMessage.PrintF( _S( 6174, "%s 아이템의 내구도가 0이 되었습니다."), _pNetwork->GetItemName(pItems->Item_Index));
		_pNetwork->ClientSystemMessage(strMessage, SYSMSG_ERROR);
		// 아이콘 표시
	}

	UIMGR()->GetQuickSlot()->UpdateItemDurability(pItems->Item_UniIndex, pItems->Item_durability_now, pItems->Item_durability_max);
#endif	//	DURABILITY
}

void CSessionState::ReceiveItemExchange( CNetworkMessage* istr )
{
	ResponseClient::doItemExchange* pRecv = reinterpret_cast<ResponseClient::doItemExchange*>(istr->GetBuffer());

	// UpdateAmendconditionCount()에 들어가는 인자가 -1이면 현재 선택된 아이템을 얻어와서 갱신함.
	if (pRecv->result == 0)
		UIMGR()->GetTrade()->UpdateAmendConditionCount(-1);
}