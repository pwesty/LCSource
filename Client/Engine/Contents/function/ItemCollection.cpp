#include "StdH.h"
#include "ItemCollection.h"
#include <Engine/Interface/UIInternalClasses.h>

#include <Engine/Contents/function/ItemCollectionUI.h>
#include <Engine/Contents/function/ItemCollectionData.h>
#include <Engine/Contents/Base/UIMsgBoxMgr.h>

#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>


CItemCollection::CItemCollection()
{

}

CItemCollection::~CItemCollection()
{

}

void CItemCollection::RecvItemCollectionMsg( CNetworkMessage* istr )
{
	pTypeBase* pPacket = reinterpret_cast<pTypeBase*>(istr->GetBuffer());

	switch(pPacket->subType)
	{
	case MSG_SUB_ITEM_COLLECTION_ERROR:
		ErrorMessage(istr);
		break;
	case MSG_SUB_ITEM_COLLECTION_DOING_LIST_UPD:
		UpdateDoingList(istr);
		break;
	case MSG_SUB_ITEM_COLLECTION_END_LIST_UPD:
		UpdateEndList(istr);
		break;
	case MSG_SUB_ITEM_COLLECTION_DOING_DATA_UPD:
		UpdateDoingData(istr);
		break;
	case MSG_SUB_ITEM_COLLECTION_END_DATA_UPD:
		UpdateEndData(istr);
		break;
	}
}

void CItemCollection::UpdateDoingList( CNetworkMessage* istr )
{
	UpdateClient::ItemCollection_DoingList* pPack = reinterpret_cast<UpdateClient::ItemCollection_DoingList*>(istr->GetBuffer());

	for (int i = 0; i < pPack->count; ++i)
	{
		CItemCollectionData::UpdateItemCount(pPack->data[i].category_index, pPack->data[i].theme_index, pPack->data[i].need_num);
	}

	UIMGR()->GetItemCollection()->ArrangeTopThemeList();
}

void CItemCollection::UpdateEndList( CNetworkMessage* istr )
{
	UpdateClient::ItemCollection_EndList* pPack = reinterpret_cast<UpdateClient::ItemCollection_EndList*>(istr->GetBuffer());

	for (int i = 0; i < pPack->count; ++i)
		CItemCollectionData::CompleteTheme(pPack->data[i].category_index, pPack->data[i].theme_index, pPack->data[i].end_time);

	UIMGR()->GetItemCollection()->ArrangeTopThemeList();
}

void CItemCollection::UpdateDoingData( CNetworkMessage* istr )
{
	UpdateClient::ItemCollection_DoingData* pPack = reinterpret_cast<UpdateClient::ItemCollection_DoingData*>(istr->GetBuffer());

	CItemCollectionData::UpdateItemCount(pPack->category_index, pPack->theme_index, pPack->need_num);
	UIMGR()->GetItemCollection()->UpdateSelectItem(CItemCollectionData::calcIndex(pPack->category_index, pPack->theme_index));

	UIMGR()->GetItemCollection()->ArrangeTopThemeList();
}

void CItemCollection::UpdateEndData( CNetworkMessage* istr )
{
	UpdateClient::ItemCollection_EndData* pPack = reinterpret_cast<UpdateClient::ItemCollection_EndData*>(istr->GetBuffer());

	CItemCollectionData::CompleteTheme(pPack->category_index, pPack->theme_index, pPack->end_time);
	UIMGR()->GetItemCollection()->UpdateSelectItem(CItemCollectionData::calcIndex(pPack->category_index, pPack->theme_index));

	UIMGR()->GetItemCollection()->ArrangeTopThemeList();
}

void CItemCollection::ErrorMessage( CNetworkMessage* istr )
{
	ResponseClient::ItemCollection_Error* pErr = reinterpret_cast<ResponseClient::ItemCollection_Error*>(istr->GetBuffer());
	switch(pErr->errorCode)
	{
		//아이템 등록 성공
	case ITEM_COLLECTION_ERROR_GIVE_SUCCESS:
		{
		}
		break;
		//아이템 보상 성공
	case ITEM_COLLECTION_ERROR_RESULT_SUCCESS:
		{
		}
		break;
		//아이템 등록 실패
	case ITEM_COLLECTION_ERROR_GIVE_FAIL:
		{
		}
		break;
		//아이템 보상 실패
	case ITEM_COLLECTION_ERROR_RESULT_FAIL:
		{
		}
		break;
	case ITEM_COLLECTION_NOT_ENOUGH_INVEN:
		{
			MSGBOXMGR()->CreateMsgBox(_S(7036, "아이리스 아이템 도감"), 
				_S(116, "인벤토리 공간이 부족하여 보상을 받을 수 없습니다."), eMSG_OK);
		}
		break;
	}
}

void CItemCollection::SendGiveItem( int nCat, int nTheme, RequestClient::NEED_ITEM_DATA* pData )
{
	CNetworkMessage nmList;
	RequestClient::ItemCollection_GiveItem* packet = reinterpret_cast<RequestClient::ItemCollection_GiveItem*>(nmList.nm_pubMessage);

	packet->type = MSG_ITEMCOLLECTION;
	packet->subType = MSG_SUB_ITEM_COLLECTION_ITEM_UPDATE_REQ;
	packet->category_index = nCat;
	packet->theme_index = nTheme;

	for (int i = 0; i < DEF_NEED_ITEM_COUNT; ++i)
		packet->data[i] = pData[i];

	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
}

void CItemCollection::SendAmendItem( int nCat, int nTheme )
{
	CNetworkMessage nmList;
	RequestClient::ItemCollection_Result* packet = reinterpret_cast<RequestClient::ItemCollection_Result*>(nmList.nm_pubMessage);

	packet->type = MSG_ITEMCOLLECTION;
	packet->subType = MSG_SUB_ITEM_COLLECTION_RESULT_REQ;
	packet->category_index = nCat;
	packet->theme_index = nTheme;

	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
}
