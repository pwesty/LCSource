#include "StdH.h"
#include "ItemHelp.h"
#include <Common/CommonDef.h>
#include <Common/Packet/ptype_inventory.h>
#include <Engine/Network/CNetwork.h>

bool ItemHelp::HaveItem( int ItemIndex, OUT SWORD* nTab, SWORD* nInvenIdx, __int64 nCnt /*= -1*/ )
{
	bool	bFind = false;
	int		t, i, each;
	int		haveItemCount = 0;

	if (ItemIndex == NAS_ITEM_DB_INDEX)
	{
		if (_pNetwork->MyCharacterInfo.money >= nCnt)
			return true;

		return false;
	}

	for (t = 0; t <= INVENTORY_TAB_CASH_2; ++t)
	{
		if (t == INVENTORY_TAB_NORMAL)			each = ITEM_COUNT_IN_INVENTORY_NORMAL;
		else if ( t == INVENTORY_TAB_CASH_1 )	each = ITEM_COUNT_IN_INVENTORY_CASH_1;
		else each = ITEM_COUNT_IN_INVENTORY_CASH_2; 

		for (i = 0 ; i < each; ++i)
		{
			if (_pNetwork->MySlotItem[t][i].ItemData == NULL)
				continue;

			if (_pNetwork->MySlotItem[t][i].ItemData->GetItemIndex() == ItemIndex)
			{
				if (_pNetwork->MySlotItem[t][i].Item_Sum > 0)
				{
					haveItemCount += _pNetwork->MySlotItem[t][i].Item_Sum;

					if (nTab != NULL)
						*nTab = t;
					if (nInvenIdx != NULL)
						*nInvenIdx = i;

					bFind = true;
				}
			}
		}
	}

	if (bFind == true)
	{
		if (haveItemCount >= nCnt)
			return true;
	}

	return false;	
}

bool ItemHelp::HasReuseTimeItem( CItems& rItem )
{
	if (rItem.Item_Index == -1)
		return FALSE;

	if( rItem.Item_Index == 2407 || rItem.Item_Index == 2408 || rItem.Item_Index == 2609 || 
		rItem.Item_Index == 5018 || rItem.Item_Index == 5019 || rItem.Item_Index == 2500 || 
		rItem.Item_Index == 2609)
	{
		return TRUE;
	}

	CItemData*	pItemData = rItem.ItemData;

	if (pItemData == NULL)
		return FALSE;

	if( ( pItemData->GetType() == CItemData::ITEM_POTION ) ||
		( ( pItemData->GetType() == CItemData::ITEM_ONCEUSE && pItemData->GetSubType() == CItemData::ITEM_SUB_TARGET ) ) ||
		( pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD ) )  
	{	// [2010/10/20 : Sora] 몬스터 용병 카드추가
		return TRUE;
	}

	return FALSE;
}

DOUBLE ItemHelp::GetCoolTime( DOUBLE reUseTime, DOUBLE startTime )
{
	if (startTime <= 0.0)
		return 0.0;

	DOUBLE coolTime = reUseTime - (_pTimer->GetHighPrecisionTimer().GetSeconds() - startTime);

	if (coolTime > reUseTime) // 방어코드 재사용 시간보다 더 큰 값은 오지 못하도록 한다.
		return reUseTime;

	if (coolTime <= 0.0)
		return 0.0;	// 남은 시간이 0보다 작으면 쿨타임 없음
	else
		return coolTime;
}
