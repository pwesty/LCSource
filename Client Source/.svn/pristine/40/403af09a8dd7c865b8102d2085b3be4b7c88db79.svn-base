#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIInvenCashBagBox.h"
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Interface/UIInventory.h>


class CmdCashBagBoxClose : public Command
{
public:
	CmdCashBagBoxClose() : m_pWnd(NULL) {}
	void setData( CUIInvenCashBagBox* pWnd ) { m_pWnd = pWnd; }
	void execute()
	{
		if( m_pWnd )
			m_pWnd->CloseUI();
	}
private:
	CUIInvenCashBagBox* m_pWnd;
};

class CmdCashBagBoxOk : public Command
{
public:
	CmdCashBagBoxOk() : m_pWnd(NULL) {}
	void setData( CUIInvenCashBagBox* pWnd ) { m_pWnd = pWnd; }
	void execute()
	{
		if( m_pWnd )
			m_pWnd->SendItemUsed();
	}
private:
	CUIInvenCashBagBox* m_pWnd;
};

CUIInvenCashBagBox::CUIInvenCashBagBox()
	: m_pIconKey(NULL)
	, m_pDrag(NULL)
	, m_pTextTitle(NULL)
	, m_nOriX(0)
	, m_nOriY(0)
	, m_bDrag(false)
	, m_nCashBagNum(-1)
	, m_pSendItems(NULL)
{
	setInherit(false);
}

void CUIInvenCashBagBox::initialize()
{
	CUIButton* pBtnClose = (CUIButton*)findUI("btn_close");
	if( pBtnClose )
	{
		CmdCashBagBoxClose* pCmd = new CmdCashBagBoxClose;
		pCmd->setData(this);
		pBtnClose->SetCommandUp(pCmd);
		
	}

	CUIButton* pBtnCancel = (CUIButton*)findUI("btn_cancel");
	if( pBtnCancel )
	{
		CmdCashBagBoxClose* pCmd = new CmdCashBagBoxClose;
		pCmd->setData(this);
		pBtnCancel->SetCommandUp(pCmd);
	}

	CUIButton* pBtnOk = (CUIButton*)findUI("btn_ok");
	if( pBtnOk )
	{
		CmdCashBagBoxOk* pCmd = new CmdCashBagBoxOk;
		pCmd->setData(this);
		pBtnOk->SetCommandUp(pCmd);
	}

	m_pIconKey = (CUIIcon*)findUI("icon_key");
	if( m_pIconKey )
		m_pIconKey->clearIconData();

	m_pTextTitle = (CUIText*)findUI("text_title");

	m_pDrag	= (CUIText*)findUI("text_drag");

}

void CUIInvenCashBagBox::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}
void CUIInvenCashBagBox::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIInvenCashBagBox::OpenUI(int nBagNum)
{
	if(IsVisible() == TRUE || GetHide() == FALSE )
		return;

	m_nCashBagNum = nBagNum;
	SetTitle();

	SetVisible(TRUE);
	Hide(FALSE);
	CUIManager::getSingleton()->RearrangeOrder(UI_INVEN_CASH_BAG_BOX, TRUE);
}

void CUIInvenCashBagBox::CloseUI()
{
	Hide(TRUE);
	SetVisible(FALSE);
	Reset();
	m_nCashBagNum = -1;
	CUIManager::getSingleton()->RearrangeOrder(UI_INVEN_CASH_BAG_BOX, FALSE);
}

void CUIInvenCashBagBox::SendItemUsed()
{
	if (m_pSendItems == NULL || m_pIconKey == NULL)
	{
		ErrMessage(eERR_EMPTY_KEY);
		return;
	}

	_pNetwork->UseSlotItem(m_pSendItems->Item_Tab, m_pSendItems->InvenIndex);
}

WMSG_RESULT CUIInvenCashBagBox::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pDrag && m_pDrag->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder(UI_INVEN_CASH_BAG_BOX, TRUE);
	return CUIBase::OnLButtonDown(x, y);
}

WMSG_RESULT CUIInvenCashBagBox::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pIconKey && m_pIconKey->IsInside(x, y) )
	{
		if (UIMGR()->GetDragIcon() != NULL)
			SetPutOnItemData();
	}

	return CUIBase::OnLButtonUp(x, y);
}

WMSG_RESULT CUIInvenCashBagBox::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	return CUIBase::OnMouseMove(x, y, pMsg);
}

void CUIInvenCashBagBox::SetPutOnItemData()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetMessenger()->SetDragging(false);

	if (pUIManager->GetDragIcon()->getBtnType() == UBET_ITEM )
	{
		CItems* pItems = pUIManager->GetDragIcon()->getItems();

		if (pItems == NULL)
			return;

		CItemData* pItemData = pItems->ItemData;

		if (pItemData->GetType() == CItemData::ITEM_ONCEUSE &&
			pItemData->GetSubType() == CItemData::ITEM_SUB_INVEN_CASH_BAG_KEY)
		{
			int item_index = pItemData->GetItemIndex();
			if( item_index - INVEN_CASH_KEY1 == m_nCashBagNum - 1 )
			{
				if( m_pIconKey )
				{
					m_pSendItems = pItems;
					m_pIconKey->setData(m_pSendItems);
					
					pUIManager->ResetHoldBtn();
					return;
				}
			}
		}
	}
	ErrMessage(eERR_CAN_NOT_OPEN_KEY);
	pUIManager->ResetHoldBtn();
}

void CUIInvenCashBagBox::Reset()
{
	if( m_pIconKey )
		m_pIconKey->clearIconData();

	m_pSendItems = NULL;
	m_nCashBagNum = -1;
}

void CUIInvenCashBagBox::SetTitle()
{
	if( !m_pTextTitle )
		return;
	CTString strMessage;
	strMessage.PrintF(_S(6060, "%d번 아이리스 가방"), m_nCashBagNum);
	m_pTextTitle->SetText( strMessage );
}

void CUIInvenCashBagBox::ErrMessage( int errCode )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
	
	switch( errCode )
	{
	case eERR_CAN_NOT_OPEN_KEY:
		{
			strMessage.PrintF( _S(6065, "아이리스 가방을 열 수 있는 열쇠가 아닙니다."));
		}
		break;
	case eERR_EMPTY_KEY:
		{
			strMessage.PrintF( _S(6063, "가방을 열 수 있는 열쇠를 올려주세요."));
		}
		break;
	}
	if( strMessage.IsEmpty() == FALSE )
	{
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}

	if( m_pIconKey )
		m_pIconKey->clearIconData();

	m_pSendItems = NULL;
	//Reset();
}

BOOL CUIInvenCashBagBox::CloseWindowByEsc()
{
	CloseUI();
	return TRUE;
}

void CUIInvenCashBagBox::UseItemMessage( int nCashBagIdx )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	if( pUIManager->DoesMessageBoxLExist(MSGCMD_NULL) )
		return;

	CTString strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );								
	strMessage.PrintF( _S(6064, "%d번 아이리스 가방이 열렸습니다."), nCashBagIdx + 1);
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}
