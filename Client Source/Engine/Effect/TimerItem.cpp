#include "StdH.h"

#include "TimerItem.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Contents/Base/UIPetStash.h>
#include <Engine/Interface/UIInventory.h>

TimerItem::TimerItem(void)
{
	for (int i = 0; i < TIMERITEM_MAX; i++)
	{
		m_TimerItemExist[i] = false;
	}

	m_mapMyTimerItem.clear();
}

TimerItem::~TimerItem(void)
{
	ClearTimerItemList();
}
/////////////////////////// Interface ////////////////////////////////////
void TimerItem::ClearTimerItemList()
{
	for (int i = 0; i < TIMERITEM_MAX; i++)
	{
		m_TimerItemExist[i] = false;
	}

	m_mapMyTimerItem.clear();
}

void TimerItem::AddTimerItem( int nTimerType, int nTime )
{
	if (nTimerType < 0 || nTime <= 0 || nTimerType >= TIMERITEM_MAX)
		return;

	if ( _findItem(nTimerType) == false )
	{
		// add list
		bool bResult = _AddItem(nTimerType, nTime);
		_SetExist(nTimerType, bResult);
	}
}

void TimerItem::ModifyTimerItem( int nTimerType, int nTime )
{
	if (nTimerType < 0 || nTime <= 0 || nTimerType >= TIMERITEM_MAX)
		return;

	if ( _findItem(nTimerType) == true )
	{		
		// modify
		_ModifyItem(nTimerType, nTime);
	}
	else
	{
		bool bResult = _AddItem(nTimerType, nTime);
		_SetExist(nTimerType, bResult);
	}
}

void TimerItem::DeleteTimerItem( int nTimerType )
{
	if (nTimerType < 0 || nTimerType >= TIMERITEM_MAX)
		return;

	if ( _findItem(nTimerType) == true )
	{
		// delete
		bool bResult = _DelItem(nTimerType);
		_SetExist(nTimerType, !bResult);
	}
}

bool TimerItem::GetTimerItemExist( int nTimerType )
{
	if (nTimerType < 0 || nTimerType >= TIMERITEM_MAX)
		return false;

	return m_TimerItemExist[nTimerType];
}

int TimerItem::GetTimerItemExpireTime( int nTimerType )
{
	if (nTimerType < 0 || nTimerType >= TIMERITEM_MAX)
		return 0;
	
	mapTimerItemTypeiter ItemIter = _Find(nTimerType);

	if ( ItemIter != m_mapMyTimerItem.end() )
		return ItemIter->second;

	return 0;
}
//////////////////////////  Network   ////////////////////////////////////
void TimerItem::RecvTimerItemMessage(CNetworkMessage* istr)
{
	CUIManager* pUimanager = CUIManager::getSingleton();
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(istr->GetBuffer());

	switch(pBase->subType)
	{
	case MSG_SUB_TIMER_ITEM_LIST:
		{
			ResponseClient::listTimerItem* pRecv = reinterpret_cast<ResponseClient::listTimerItem*>(istr->GetBuffer());
			
			for (int i = 0; i < pRecv->count; i++)
			{
				AddTimerItem( pRecv->list[i].type, pRecv->list[i].expire_time );
				switch (pRecv->list[i].type)
				{
				case TIMERITEM_TYPE_CASH_INVENTORY_1:
				case TIMERITEM_TYPE_CASH_INVENTORY_2:
					{
						pUimanager->GetInventory()->SetCashBagExpireTime(pRecv->list[i].type);
					}
					break;
				default:

					break;

				}					
			}
		}
		break;

	case MSG_SUB_TIMER_ITEM_MODIFY:
		{
			ResponseClient::modifyTimerItem* pRecv = reinterpret_cast<ResponseClient::modifyTimerItem*>(istr->GetBuffer());

			ModifyTimerItem(pRecv->item_type, pRecv->expire_time);
			switch(pRecv->item_type)
			{
			case TIMERITEM_NON:
				{

				}
				break;
			case TIMETITEM_TYPE_REMOTE_EXPRESS_SYSTEM:
				{

				}
				break;
			case TIMERITEM_TYPE_CASH_INVENTORY_1:
			case TIMERITEM_TYPE_CASH_INVENTORY_2:
				{
					pUimanager->GetInventory()->SetCashBagExpireTime(pRecv->item_type);
				}
				break;
			case TIMERITEM_TYPE_PET_STASH:
				{
					pUimanager->GetPetStash()->UpdateReuseTime();
				}
				break;
			}
		}
		break;

	case MSG_SUB_TIMER_ITEM_DELETE:
		{
			ResponseClient::deleteTimerItem* pRecv = reinterpret_cast<ResponseClient::deleteTimerItem*>(istr->GetBuffer());
			
			DeleteTimerItem(pRecv->item_type);

			switch (pRecv->item_type)
			{
			case TIMERITEM_TYPE_PET_STASH:
				{
					pUimanager->GetPetStash()->UpdateReuseTime();
				}
				break;
			case TIMERITEM_TYPE_CASH_INVENTORY_1:
			case TIMERITEM_TYPE_CASH_INVENTORY_2:
				{
					pUimanager->GetInventory()->SetCashBagExpireTime(pRecv->item_type);
				}
				break;
			}

		}
		break;

	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
mapTimerItemTypeiter TimerItem::_Find( int nTimerType )
{
	mapTimerItemTypeiter iter = m_mapMyTimerItem.find( nTimerType );
	mapTimerItemTypeiter iter_End = m_mapMyTimerItem.end();

	if (iter == iter_End)
		return iter_End;

	return iter;	
}

bool TimerItem::_findItem( int nTimerType )
{
	mapTimerItemTypeiter ItemIter = _Find(nTimerType);
	
	if ( ItemIter != m_mapMyTimerItem.end() )
		return true;

	return false;
}

bool TimerItem::_AddItem( int nTimerType, int nTime )
{
	mapTimerItemType::value_type info(nTimerType, nTime);
	m_mapMyTimerItem.insert( info );
	return true;
}

void TimerItem::_ModifyItem( int nTimerType, int nTime )
{
	mapTimerItemTypeiter ItemIter = _Find(nTimerType);
	
	if ( ItemIter != m_mapMyTimerItem.end() )
	{
		ItemIter->second = nTime;
	}
}

bool TimerItem::_DelItem( int nTimerType )
{
	mapTimerItemTypeiter ItemIter = _Find(nTimerType);

	if ( ItemIter != m_mapMyTimerItem.end() )
	{
		m_mapMyTimerItem.erase( ItemIter );
		return true;
	}

	return false;
}

void TimerItem::_SetExist( int nTimerType, bool bExist )
{
	if (nTimerType < 0 || nTimerType >= TIMERITEM_MAX)
		return;

	m_TimerItemExist[nTimerType] = bExist;
}