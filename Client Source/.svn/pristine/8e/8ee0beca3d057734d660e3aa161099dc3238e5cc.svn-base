#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>

#include "UITrade.h"

#include <Engine/Entities/TradeItem.h>
#include <Engine/Network/CNetwork.h>
#include <Common/Packet/ptype_old_do_item.h>
#include <common/header/EventTrade.h>
#include <Engine/Interface/UIInventory.h>

#define DEF_CONDITION_ITEM_MAX	(5)

class CmdTradeClose : public Command
{
public :
	CmdTradeClose() : m_pWnd(NULL)	{}
	void setData( CUITrade* pWnd )	{ m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd)
			m_pWnd->CloseUI();
	}

private :
	CUITrade* m_pWnd;
};

class CmdTradeSend : public Command
{
public :
	CmdTradeSend() : m_pWnd(NULL)	{}
	void setData( CUITrade* pWnd )	{ m_pWnd = pWnd;	}
	void execute()
	{
		// 현재는 송크란 이벤트만 사용하도록.
		if (IS_EVENT_ON(TEVENT_SONGKRAN) && m_pWnd)
			m_pWnd->SendTrade();
	}

private :
	CUITrade* m_pWnd;
};

class CmdSelectItem : public Command
{
public :
	CmdSelectItem()	: m_pWnd(NULL), m_nSelectIndex(-1)	{}
	void setData( CUITrade* pWnd, int nSelectIndex )	{ m_pWnd = pWnd; m_nSelectIndex = nSelectIndex;	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->UpdateAmendItem(m_nSelectIndex);
			m_pWnd->UpdateAmendCondition(m_nSelectIndex);
		}
	}

private:
	CUITrade* m_pWnd;
	int m_nSelectIndex;
};

CUITrade::CUITrade()
	: m_pDragArea(NULL)
	, m_pBtnOk(NULL)
	, m_pTextTitle(NULL)
	, m_pBaseAmendInfo(NULL)
	, m_nOriX(0)
	, m_nOriY(0)
{
	setInherit(false);

	int i;

	for (i = 0; i < eLIST_END; ++i)
	{
		m_pList[i] = NULL;
	}
}

CUITrade::~CUITrade()
{
	m_pDragArea		= NULL;
	m_pBtnOk		= NULL;
	m_pTextTitle	= NULL;
	m_pBaseAmendInfo = NULL;

	int i;

	for (i = 0; i < eLIST_END; ++i)
	{
		m_pList[i] = NULL;
	}
}

void CUITrade::initialize()
{
	CUIButton* pClose = (CUIButton*)findUI("btn_close");

	if (pClose)
	{
		CmdTradeClose* pCmd = new CmdTradeClose;
		pCmd->setData(this);
		pClose->SetCommandUp(pCmd);
	}

	CUIButton* pCancel = (CUIButton*)findUI("btn_cancel");

	if (pCancel)
	{
		CmdTradeClose* pCmd = new CmdTradeClose;
		pCmd->setData(this);
		pCancel->SetCommandUp(pCmd);
	}

	m_pBtnOk = (CUIButton*)findUI("btn_ok");
	
	if (m_pBtnOk)
	{
		CmdTradeSend* pCmd = new CmdTradeSend;
		pCmd->setData(this);
		m_pBtnOk->SetCommandUp(pCmd);
	}

	m_pDragArea		= findUI("base_drag_area");
	m_pTextTitle	= (CUIText*)findUI("text_title");
	
	m_pList[eLIST_AMEND_ITEM] = (CUIList*)findUI("list_amend");
	m_pList[eLIST_AMEND_CONDITION] = (CUIList*)findUI("list_amend_condition");

	m_pBaseAmendInfo = findUI("base_amend_info");
}

void CUITrade::OpenUI()
{
	if(IsVisible() == TRUE || GetHide() == FALSE )
		return;

	if (UIMGR()->GetInventory()->IsLocked() == TRUE ||
		UIMGR()->GetInventory()->IsLockedArrange() == TRUE)
	{
		UIMGR()->GetInventory()->ShowLockErrorMessage();
		return;
	}

	UIMGR()->GetInventory()->Lock(TRUE, TRUE, LOCK_TRADE);

	SetVisible(TRUE);
	Hide(FALSE);

	if (m_pBtnOk != NULL)
		m_pBtnOk->SetEnable(FALSE);

	UpdateItemList();

	UIMGR()->RearrangeOrder(UI_TRADE, TRUE);
}

void CUITrade::CloseUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	UIMGR()->GetInventory()->Lock(FALSE, FALSE, LOCK_TRADE);
	UIMGR()->RearrangeOrder(UI_TRADE, FALSE);
}

void CUITrade::UpdateItemList()
{
	if (m_pList[eLIST_AMEND_ITEM] == NULL)
		return;

	m_pList[eLIST_AMEND_ITEM]->DeleteAllListItem();

	int i = 0, max = CTradeItem::GetItemCount();

	if (max < 0)
		return;

	int nItemCnt = m_pList[eLIST_AMEND_ITEM]->getListItemCount();

	CUIListItem* pItem = m_pList[eLIST_AMEND_ITEM]->GetListItemTemplate();
	CUIListItem* pItemTmp = NULL;

	CUIIcon* pIcon = NULL;
	CUIText* pText = NULL;
	stEventTrade* pTradeItem = NULL;

	for (i = 0; i < max; ++i)
	{
		pTradeItem = CTradeItem::GetTradeItem(i);

		if (pTradeItem == NULL || pTradeItem->result_itemIndex <= 0)
			continue;

		if (nItemCnt <= i)
			m_pList[eLIST_AMEND_ITEM]->AddListItem(pItem->Clone());

		pItemTmp = (CUIListItem*)m_pList[eLIST_AMEND_ITEM]->GetListItem(i);

		if (pItemTmp == NULL)
			continue;

		CmdSelectItem* pCmd = new CmdSelectItem;
		pCmd->setData(this, i);
		pItemTmp->SetCommand(pCmd);

		if (pIcon = (CUIIcon*)pItemTmp->findUI("icon_item"))
		{
			pIcon->setData(UBET_ITEM, pTradeItem->result_itemIndex);
		}

		if (pText = (CUIText*)pItemTmp->findUI("text_name"))
		{
			CTString strTmp = _pNetwork->GetItemName(pTradeItem->result_itemIndex);

			strTmp = UtilHelp::getSingleton()->GetCalcStringEllipsis(strTmp, pText->GetWidth(), CTString("..."));

			pText->SetText(strTmp);
		}
	}

	m_pList[eLIST_AMEND_ITEM]->setCurSel(-1);
	m_pList[eLIST_AMEND_ITEM]->UpdateList();

	UpdateAmendItem(-1);
	UpdateAmendCondition(-1);
}

void CUITrade::UpdateAmendCondition( int nAmendIndex )
{
	if (m_pList[eLIST_AMEND_CONDITION] == NULL)
		return;

	m_pList[eLIST_AMEND_CONDITION]->DeleteAllListItem();

	if (nAmendIndex < 0)
		m_pList[eLIST_AMEND_CONDITION]->Hide(TRUE);
	else
		m_pList[eLIST_AMEND_CONDITION]->Hide(FALSE);

	int i = 0;

	int nItemCnt = m_pList[eLIST_AMEND_CONDITION]->getListItemCount();

	CUIListItem* pItem = m_pList[eLIST_AMEND_CONDITION]->GetListItemTemplate();
	CUIListItem* pItemTmp = NULL;

	CUIIcon* pIcon = NULL;
	stEventTrade* pTradeItem = NULL;

	pTradeItem = CTradeItem::GetTradeItem(nAmendIndex);

	if (pTradeItem == NULL)
		return;

	for (i = 0; i < DEF_CONDITION_ITEM_MAX; ++i)
	{
		if (pTradeItem->st_source_item[i].src_item_idx <= 0)
			continue;

		if (nItemCnt <= i)
			m_pList[eLIST_AMEND_CONDITION]->AddListItem(pItem->Clone());

		pItemTmp = (CUIListItem*)m_pList[eLIST_AMEND_CONDITION]->GetListItem(i);

		if (pItemTmp == NULL)
			continue;

		int nMyItemCount = UIMGR()->GetInventory()->GetItemCount(pTradeItem->st_source_item[i].src_item_idx);

		if (pIcon = (CUIIcon*)pItemTmp->findUI("icon_item"))
		{
			pIcon->setData(UBET_ITEM, pTradeItem->st_source_item[i].src_item_idx);
		}
	}

	m_pList[eLIST_AMEND_CONDITION]->setCurSel(-1);
	m_pList[eLIST_AMEND_CONDITION]->UpdateList();

	UpdateAmendConditionCount(nAmendIndex);
}

void CUITrade::UpdateAmendItem( int nAmendIndex )
{
	if (m_pBaseAmendInfo == NULL)
		return;

	m_pBaseAmendInfo->Hide(TRUE);
	
	if (nAmendIndex < 0)
		return;

	stEventTrade* pTradeItem = CTradeItem::GetTradeItem(nAmendIndex);

	if (pTradeItem->result_itemIndex <= 0 || pTradeItem == NULL)
		return;
		
	m_pBaseAmendInfo->Hide(FALSE);
	
	CItemData* pItemData = _pNetwork->GetItemData(pTradeItem->result_itemIndex);

	if (pItemData == NULL)
		return;

	if (CUIIcon* pIcon = (CUIIcon*)m_pBaseAmendInfo->findUI("icon_amend_item"))
	{
		pIcon->setData(UBET_ITEM, pTradeItem->result_itemIndex);
	}

	if (CUITextBox* pTextBox = (CUITextBox*)m_pBaseAmendInfo->findUI("tb_amend_desc"))
	{
 		CTString strTmp = pItemData->GetDesc();
 
 		pTextBox->SetText(strTmp);
	}

	if (CUIText* pText = (CUIText*)m_pBaseAmendInfo->findUI("text_amend_name"))
	{
 		CTString strTmp = pItemData->GetName();
		
		strTmp = UtilHelp::getSingleton()->GetCalcStringEllipsis(strTmp, pText->GetWidth(), CTString("..."));

 		pText->SetText(strTmp);
	}

	UpdateAmendItemCount(nAmendIndex);
}

void CUITrade::UpdateAmendConditionCount( int nAmendIndex )
{
	if (m_pList[eLIST_AMEND_CONDITION] == NULL)
		return;

	if (nAmendIndex < 0 && m_pList[eLIST_AMEND_ITEM] != NULL)
		nAmendIndex = m_pList[eLIST_AMEND_ITEM]->getCurSel();

	if (m_pBtnOk != NULL)
		m_pBtnOk->SetEnable(IsAmend(nAmendIndex) ? TRUE : FALSE);

	if (nAmendIndex < 0)
		return;

	stEventTrade* pTradeItem = CTradeItem::GetTradeItem(nAmendIndex);

	if (pTradeItem == NULL)
		return;

	CUIListItem* pItemTmp = NULL;

	int i;
	int nListCnt = m_pList[eLIST_AMEND_CONDITION]->getListItemCount();

	CUIText* pText = NULL;

	for (i = 0; i < DEF_CONDITION_ITEM_MAX; ++i)
	{
		if (pTradeItem->st_source_item[i].src_item_idx <= 0 || i > nListCnt)
			continue;

		pItemTmp = (CUIListItem*)m_pList[eLIST_AMEND_CONDITION]->GetListItem(i);

		if (pItemTmp == NULL)
			continue;

		COLOR	 strCol = 0x00FF21FF;

		int nMyItemCount = UIMGR()->GetInventory()->GetItemCount(pTradeItem->st_source_item[i].src_item_idx);

		if (CheckItem(nAmendIndex, i) == false)
			strCol = DEF_UI_COLOR_RED;

		if (pText = (CUIText*)pItemTmp->findUI("text_item_cnt"))
		{
			CTString strTmp;

			strTmp.PrintF("%d/%d", nMyItemCount, pTradeItem->st_source_item[i].src_item_cnt);
			pText->SetText(strTmp);
			pText->setFontColor(strCol);
		}

		if (pText = (CUIText*)pItemTmp->findUI("text_name"))
		{
			CTString strTmp = _pNetwork->GetItemName(pTradeItem->st_source_item[i].src_item_idx);

			strTmp = UtilHelp::getSingleton()->GetCalcStringEllipsis(strTmp, pText->GetWidth(), CTString("..."));

			pText->SetText(strTmp);
			pText->setFontColor(strCol);
		}
	}
}

void CUITrade::UpdateAmendItemCount( int nAmendIndex )
{
	if (m_pBaseAmendInfo == NULL || nAmendIndex < 0)
		return;

	stEventTrade* pTradeItem = CTradeItem::GetTradeItem(nAmendIndex);

	if (pTradeItem == NULL)
		return;

	if (CUIText* pText = (CUIText*)m_pBaseAmendInfo->findUI("text_amend_cnt"))
	{
		CTString strTmp;

		strTmp.PrintF("%d", pTradeItem->result_itemCount);
		pText->SetText(strTmp);
	}
}

bool CUITrade::IsAmend( int nAmendIndex )
{
	stEventTrade* pTradeItem = CTradeItem::GetTradeItem(nAmendIndex);

	if (pTradeItem == NULL)
		return false;

	CUIInventory* pInven = UIMGR()->GetInventory();

	int i;

	for (i = 0; i < DEF_CONDITION_ITEM_MAX; ++i)
	{
		if (pTradeItem->st_source_item[i].src_item_idx <= 0)
			continue;

		int nInvenItemCnt = pInven->GetItemCount(pTradeItem->st_source_item[i].src_item_idx);

		if (nInvenItemCnt <= 0)
			return false;

		if (nInvenItemCnt < pTradeItem->st_source_item[i].src_item_cnt)
			return false;
	}

	return true;
}

void CUITrade::SetTitle( std::string strTitle )
{

}

bool CUITrade::CheckItem( int nAmendIndex, int nConditionIndex )
{
	stEventTrade* pTradeItem = CTradeItem::GetTradeItem(nAmendIndex);

	if (pTradeItem == NULL)
		return false;

	CUIInventory* pInven = UIMGR()->GetInventory();

	if (pTradeItem->st_source_item[nConditionIndex].src_item_idx <= 0)
		return false;

	int nInvenItemCnt  = pInven->GetItemCount(pTradeItem->st_source_item[nConditionIndex].src_item_idx);

	if (nInvenItemCnt <= 0)
		return false;

	int nSrcItemCnt = pTradeItem->st_source_item[nConditionIndex].src_item_cnt;

	if (nInvenItemCnt < nSrcItemCnt)
		return false;

	return true;
}

void CUITrade::SendTrade()
{
	if (UIMGR()->GetInventory()->IsInventoryFull() == TRUE)
	{
		UIMGR()->GetChattingUI()->AddSysMessage(_S(598, "인벤토리가 꽉 차서 보상을 받을 수 없습니다."), SYSMSG_ERROR);
		return;
	}

	if (m_pList[eLIST_AMEND_ITEM] == NULL)
		return;

	CNetworkMessage nmMessage;
	RequestClient::doItemExchange* packet = reinterpret_cast<RequestClient::doItemExchange*>(nmMessage.nm_pubMessage);

	int i, nSelect = m_pList[eLIST_AMEND_ITEM]->getCurSel();

	if (nSelect < 0)
		return;

	if (IsAmend(nSelect) == false)
		return;

	stEventTrade* pTradeItem = CTradeItem::GetTradeItem(nSelect);

	if (pTradeItem == NULL)
		return;

	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_EXCHANGE;
	packet->exchangeIndex = pTradeItem->index;
	
	int nCnt = 0;

	for (i = 0; i < DEF_CONDITION_ITEM_MAX; ++i)
	{
		if (pTradeItem->st_source_item[i].src_item_idx <= 0)
			continue;

		packet->list[i].itemIndex = pTradeItem->st_source_item[i].src_item_idx;
		packet->list[i].itemCount = pTradeItem->st_source_item[i].src_item_cnt;
		nCnt = i + 1;
	}

	if (nCnt <= 0)
		return;

	packet->listCount = nCnt;

	nmMessage.setSize( sizeof(*packet) + (sizeof(packet->list[0]) * nCnt));

	_pNetwork->SendToServerNew(nmMessage);
}

WMSG_RESULT CUITrade::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if( m_bDrag && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int ndX = x - m_nOriX;
		int ndY = y - m_nOriY;

		m_nOriX = x;
		m_nOriY = y;

		Move( ndX, ndY );
		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	UIMGR()->SetMouseCursorInsideUIs();

	return CUIBase::OnMouseMove(x, y, pMsg);
}

WMSG_RESULT CUITrade::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pDragArea && m_pDragArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	if (m_pList[eLIST_AMEND_ITEM]->IsInside(x, y) == TRUE)
	{
		if (m_pList[eLIST_AMEND_ITEM]->HitTest(x, y) < 0)
		{
			UpdateAmendItem(-1);
			UpdateAmendCondition(-1);

			if (m_pBtnOk != NULL)
				m_pBtnOk->SetEnable(FALSE);
		}
	}

	UIMGR()->RearrangeOrder(UI_TRADE, TRUE);

	return CUIBase::OnLButtonDown(x, y);
}

WMSG_RESULT CUITrade::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	UIMGR()->ResetHoldBtn();

	return CUIBase::OnLButtonUp(x, y);
}

WMSG_RESULT CUITrade::OnLButtonDBLClick( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pList[eLIST_AMEND_ITEM]->IsInside(x, y) == TRUE)
	{
		if (m_pList[eLIST_AMEND_ITEM]->HitTest(x, y) < 0)
		{
			UpdateAmendItem(-1);
			UpdateAmendCondition(-1);

			if (m_pBtnOk != NULL)
				m_pBtnOk->SetEnable(FALSE);
		}
	}

	return WMSG_SUCCESS;
}
