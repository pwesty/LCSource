#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Common/Packet/ptype_express_system.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Contents/Base/ExpressSystem.h>
#include "UIMysteriousBead.h"
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Contents/Base/UIMsgBoxNumeric_only.h>

class CmdMysteriousBeadClose : public Command
{
public:
	CmdMysteriousBeadClose() : m_pWnd(NULL) {}
	void setData( CUIMysteriousBead* pWnd ) { m_pWnd = pWnd; }
	void execute()
	{
		if( m_pWnd )
			m_pWnd->CloseUI();
	}
private:
	CUIMysteriousBead* m_pWnd;
};

class CmdMysteriousBeadSearch : public Command
{
public:
	CmdMysteriousBeadSearch() : m_pWnd(NULL) {}
	void setData( CUIMysteriousBead* pWnd ) { m_pWnd = pWnd; }
	void execute()
	{
		if( m_pWnd )
			m_pWnd->SendPlayerSearch();
	}
private:
	CUIMysteriousBead* m_pWnd;
};

class CmdMysteriousBeadOk : public Command
{
public:
	CmdMysteriousBeadOk() : m_pWnd(NULL) {}
	void setData( CUIMysteriousBead* pWnd ) { m_pWnd = pWnd; }
	void execute()
	{
		if( m_pWnd )
			m_pWnd->SendItemMsgBox();
	}
private:
	CUIMysteriousBead* m_pWnd;
};

class CmdMysteriousBeadCountItem : public Command
{
public:
	CmdMysteriousBeadCountItem() : m_pWnd(NULL) {}
	void setData(CUIMysteriousBead* pWnd)	{ m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd)
			m_pWnd->CountItemCallback();
	}

private:
	CUIMysteriousBead* m_pWnd;
};
CUIMysteriousBead::CUIMysteriousBead()
	: m_pSearchPlayer(NULL),
	m_pPutArea(NULL),
	m_pTitle(NULL),
	m_pDragArea(NULL),
	m_pBackImagePutOn(NULL),
	m_pPutOnArea(NULL),
	m_bDrag(false),
	m_bPlayerSerach(false),
	m_bPutOnItem(false),
	m_nOriX(0),
	m_nOriY(0),
	m_pSendItems(NULL)
{
	setInherit(false);
}

void CUIMysteriousBead::initialize()
{
	CUIButton* pCloseBtn = (CUIButton*)findUI("btn_close");
	if( pCloseBtn )
	{
		CmdMysteriousBeadClose* pCmd = new CmdMysteriousBeadClose;
		pCmd->setData(this);
		pCloseBtn->SetCommandUp( pCmd );
	}

	CUIButton* pCancelBtn = (CUIButton*)findUI("btn_cancel");
	if( pCancelBtn )
	{
		CmdMysteriousBeadClose* pCmd = new CmdMysteriousBeadClose;
		pCmd->setData(this);
		pCancelBtn->SetCommandUp( pCmd );
	}

	m_pSearchPlayer	= (CUIEditBox*)findUI("edit_id");
	CUIButton* pSearch = (CUIButton*)findUI("btn_search");
	if( pSearch )
	{
		CmdMysteriousBeadSearch* pCmd = new CmdMysteriousBeadSearch;
		pCmd->setData(this);
		pSearch->SetCommandUp( pCmd );
	}
	m_pPutArea		= (CUIIcon*)findUI("icon_item_area");
	Reset();

	CUIButton* pBtnOk = (CUIButton*)findUI("btn_ok");
	if( pBtnOk )
	{
		CmdMysteriousBeadOk* pCmd = new CmdMysteriousBeadOk;
		pCmd->setData(this);
		pBtnOk->SetCommandUp( pCmd );
		pBtnOk->SetEnable(FALSE);
	}

	m_pBackImagePutOn = (CUIImage*)findUI("img_base1");

	m_pTitle = (CUIText*)findUI("text_title");

	m_pDragArea = (CUIText*)findUI("text_drag_area");

	m_uvEditImage[eEDIT_IMAGE_FOUND].SetUV(97, 386, 252, 408);
	m_uvEditImage[eEDIT_IMAGE_NOT_FOUND].SetUV(97, 355, 252, 377);

	m_pPutOnArea = new CUIBase;
	m_pPutOnArea->InitPos(16, 154, 227, 141);
	this->addChild(m_pPutOnArea);

}

void CUIMysteriousBead::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}
void CUIMysteriousBead::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIMysteriousBead::OpenUI()
{
	if(IsVisible() == TRUE || GetHide() == FALSE )
		return;

	CUIManager* pUIMgr = CUIManager::getSingleton();

	if (pUIMgr->GetInventory()->IsLocked() == TRUE ||
		pUIMgr->GetInventory()->IsLockedArrange() == TRUE)
	{
		// 이미 Lock 인 창이 있을 경우 열지 못한다.
		pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	SetVisible(TRUE);
	Hide(FALSE);
	if( m_pSearchPlayer )
		m_pSearchPlayer->Hide(FALSE);	

	pUIMgr->GetInventory()->GetUseItemSlotInfo( m_UseBeadInfo.tab, m_UseBeadInfo.invenIndex);
	pUIMgr->GetInventory()->Lock(TRUE, TRUE, LOCK_MYSTERIOUSBEAD);

	pUIMgr->RearrangeOrder(UI_MYSTERYOUSBEAD, TRUE);
}

void CUIMysteriousBead::CloseUI()
{
	Hide(TRUE);
	SetVisible(FALSE);
	if( m_pSearchPlayer )
		m_pSearchPlayer->Hide(TRUE);

	Reset();

	CUIManager* pUIMgr = CUIManager::getSingleton();

	pUIMgr->RearrangeOrder(UI_MYSTERYOUSBEAD, FALSE);
	pUIMgr->GetInventory()->Lock(FALSE, FALSE, LOCK_MYSTERIOUSBEAD);
	
	pUIMgr->GetMsgBoxNumOnly()->CloseBox();

	CUIBase::CloseProc();
}

void CUIMysteriousBead::SetFoundPlayer( BOOL bFound )
{
	m_bPlayerSerach = bFound ? true : false;
	if( m_pSearchPlayer )
	{
		UIRect rc = m_pSearchPlayer->getBackGroundRect();
		if( bFound )	
			m_pSearchPlayer->setBackGround(rc, m_uvEditImage[eEDIT_IMAGE_FOUND]);
		else
			m_pSearchPlayer->setBackGround(rc, m_uvEditImage[eEDIT_IMAGE_NOT_FOUND]);
	}
	SetOkButtonEnable();
}

void CUIMysteriousBead::SendPlayerSearch()
{
	if( m_pSearchPlayer == NULL )
		return;

	CTString strName = m_pSearchPlayer->GetString();

	if( strName.IsEmpty() == TRUE )
		return;

	if (UIMGR()->checkName(strName, 0) == FALSE)
		return;

	GameDataManager::getSingleton()->GetExpressData()->SendPlayerSearchReq(strName);
}

void CUIMysteriousBead::SendTransItem()
{
	if( m_pSearchPlayer == NULL )
		return;

	if( !m_bPlayerSerach )
	{
		ErrMessage(eERR_INVALID_NAME);
		return;
	}

	if (!m_bPutOnItem)
		return;

	CUIManager* pManager = CUIManager::getSingleton();

	CTString strName		= m_pSearchPlayer->GetString();
	int invenIndex			= m_UseBeadInfo.invenIndex;
	int tab					= m_UseBeadInfo.tab;
	int virtualIndex		= pManager->GetInventory()->GetItemUniIndex(tab, invenIndex);

	CItems* pItems = &_pNetwork->MySlotItem[tab][invenIndex];

	if (pItems->Item_Index < 0)
	{
		ErrMessage(eERR_ITEM_COUNT_OVER);
		CloseUI();
		return;
	}

	int send_invenIndex		= m_pSendItems->InvenIndex;
	int send_tab			= m_pSendItems->Item_Tab;
	int send_virtualIndex	= pManager->GetInventory()->GetItemUniIndex(send_tab, send_invenIndex);
	int send_count			= m_pSendItems->Item_Sum;

	pItems = &_pNetwork->MySlotItem[send_tab][send_invenIndex];

	if (pItems->Item_Index < 0)
	{
		ErrMessage(eERR_CAN_NOT_REGIST);
		return;
	}

	GameDataManager::getSingleton()->GetExpressData()->SendItemTransReq(tab, invenIndex, virtualIndex,
		send_tab, send_invenIndex, send_virtualIndex, send_count, strName);
}

int CUIMysteriousBead::SetPutOnItemData()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CUIIcon* pDrag = pUIManager->GetDragIcon();
	if (pDrag == NULL)
		return eERR_SUB_INVALID_ITEM;

	pUIManager->GetMessenger()->SetDragging(false);

	CItems* pItems = pDrag->getItems();

	if (pItems == NULL)
		return eERR_SUB_INVALID_ITEM;

	if (pDrag->getIndex() == pUIManager->GetInventory()->GetItemIndex(m_UseBeadInfo.tab, m_UseBeadInfo.invenIndex))
		return eERR_SUB_USING_ITEM;	// 현재 사용하고 있는 석상 아이템은 발신 불가.

	if (pItems->InvenIndex < 0)
		return eERR_SUB_NOT_IN_THE_INVEN;	// 인벤토리에 없는 아이템은 발신 불가.

	if (pDrag->getBtnType() != UBET_ITEM )
		return eERR_SUB_ITEM_IS_NOT;	// 아이템이 아니라면 발신 불가.

	if (pItems->IsFlag(FLAG_ITEM_LENT))
		return eERR_SUB_ITEM_IS_LENT;	// 렌트 아이템 발신 불가.

	if (pItems->IsFlag(FLAG_ITEM_BELONG))
		return eERR_SUB_ITEM_IS_BELONG;

	CItemData* pItemData = pItems->ItemData;

	if ( !(pItemData->GetFlag() & ITEM_FLAG_EXCHANGE) )
		return eERR_SUB_ITEM_IS_UNTRADEABLE;	// 교환품목이 아니라면 발신 불가.

	if ((pItemData->GetType() == CItemData::ITEM_ACCESSORY) && 
		pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET || pItemData->GetSubType() == CItemData::ACCESSORY_PET)
		return eERR_SUB_ITEM_IS_ACC_PET;	// 펫도 발신 불가.

	if (pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD)
		return eERR_SUB_ITEM_IS_MONSTER_CARD;	// 몬스터 용병카드 발신 불가.

	if (pItemData->GetType() == CItemData::ITEM_ACCESSORY && pItemData->GetSubType() == CItemData::ACCESSORY_RELIC)
		return eERR_SUB_ITEM_IS_RELIC;	// 유물 아이템 발송 불가.

	if (pItems->Item_Flag & FLAG_ITEM_COMPOSITION)	// 결합된 아이템에 한해서 발신 불가.
		return eERR_SUB_ITEM_IS_COMPOSITION;

	if (pItems->Item_Flag & PLATINUM_MAX_PLUS)	// 플레티늄 결합시 발신 불가
		return eERR_SUB_ITEM_IS_PLATINUM_PLUS;

	if( pItemData->GetFlag() & ITEM_FLAG_COUNT )	// 셀수 있는 아이템
	{
		m_pSendItems = new CItems;
		memcpy(m_pSendItems, pItems, sizeof(CItems));

		CTString strMessage;
		const char	*szItemName = _pNetwork->GetItemName(m_pSendItems->Item_Index);
		strMessage.PrintF(_S(150, "몇 개의 %s를 옮기시겠습니까?"), szItemName);

		UIMGR()->GetMsgBoxNumOnly()->CloseBox();
		CmdMysteriousBeadCountItem* pCmd = new CmdMysteriousBeadCountItem;
		pCmd->setData(this);
		UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S(191,"확인"), strMessage, 1, pItems->Item_Sum);
	}
	else	// 그외
	{
		m_pSendItems = new CItems;
		memcpy(m_pSendItems, pItems, sizeof(CItems));
		m_pPutArea->setData(pItems);
		
		CUIButton* pBtnOk = (CUIButton*)findUI("btn_ok");
		SetPutImage(true);
		SetOkButtonEnable();
	}

	return eERR_NONE;
}

void CUIMysteriousBead::SetPutImage( bool bPutOn )
{
	if( m_pBackImagePutOn )
	{
		m_pBackImagePutOn->Hide(bPutOn ? FALSE : TRUE);
	}
	m_bPutOnItem = bPutOn;
}

void CUIMysteriousBead::SetOkButtonEnable( )
{
	CUIButton* pBtnOk = (CUIButton*)findUI("btn_ok");
	if( pBtnOk == NULL )
		return;

	if( m_bPlayerSerach == true && m_bPutOnItem == true )
		pBtnOk->SetEnable(TRUE);
	else
		pBtnOk->SetEnable(FALSE);
}

void CUIMysteriousBead::Reset()
{
	if( m_pPutArea != NULL )
		m_pPutArea->clearIconData();

	SAFE_DELETE(m_pSendItems);

	if( m_pSearchPlayer != NULL )
		m_pSearchPlayer->ClearInputBuffer();

	SetPutImage(false);
	SetFoundPlayer(FALSE);
	m_bPlayerSerach = false;
	SetOkButtonEnable();
}

void CUIMysteriousBead::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	switch( nCommandCode )
	{
	case MSGCMD_MYSTERIOUS_BEAD_SEND_ITEM:
		{
			if( bOK )
			{
				SendTransItem();
			}
		}
		break;
	}
}

void CUIMysteriousBead::SendItemMsgBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	if( pUIManager->DoesMessageBoxLExist(MSGCMD_MYSTERIOUS_BEAD_SEND_ITEM) )
		return;

	CTString strName = m_pSearchPlayer->GetString();
	
	if( strName.IsEmpty() || m_bPlayerSerach == false )
		return;

	CTString strMessage;
	const char	*szItemName = _pNetwork->GetItemName(m_pSendItems->Item_Index);
	int		 nCount			= (int)m_pSendItems->Item_Sum;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OKCANCEL, UI_MYSTERYOUSBEAD, MSGCMD_MYSTERIOUS_BEAD_SEND_ITEM );								
	strMessage.PrintF( _S(6046, "%s %d개를 %s의 신비한 석상으로 전송 하시겠습니까?"), szItemName, nCount, strName);
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CUIMysteriousBead::Success()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString strMessage;
	const char	*szItemName = _pNetwork->GetItemName(m_pSendItems->Item_Index);
	CTString strName		= m_pSearchPlayer->GetString();
	int		 nCount			= (int)m_pSendItems->Item_Sum;
	CUIMsgBox_Info	MsgBoxInfo;

	MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
	strMessage.PrintF(_S(6047, "%s %d개를 %s의 신비한 석상으로 전송 하였습니다."), szItemName, nCount, strName);
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	CloseUI();
}

void CUIMysteriousBead::ErrMessage(int type)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);

	switch( type )
	{
	case eERR_INVALID_NAME:
		{
			strMessage.PrintF( _S( 2500, "잘못된 이름입니다.") );
		}
		break;
	case eERR_CAN_NOT_REGIST:
	case eERR_SUB_USING_ITEM:
	case eERR_SUB_NOT_IN_THE_INVEN:
	case eERR_SUB_ITEM_IS_NOT:
	case eERR_SUB_ITEM_IS_ACC_PET:
	case eERR_SUB_ITEM_IS_MONSTER_CARD:
	case eERR_SUB_ITEM_IS_LENT:
	case eERR_SUB_ITEM_IS_COMPOSITION:
	case eERR_SUB_ITEM_IS_UNTRADEABLE:
	case eERR_SUB_ITEM_IS_RELIC:
	case eERR_SUB_ITEM_IS_BELONG:
	case eERR_SUB_ITEM_IS_PLATINUM_PLUS:
		{
			strMessage.PrintF( _S( 5988, "등록할 수 없는 아이템 입니다.") );
#ifdef VER_TEST
			strMessage += CTString(0, " (%d)", type);
#endif // _DEBUG
		}
		break;
	case eERR_ITEM_COUNT_OVER:
		{
			strMessage.PrintF( _S( 6109, "선택한 슬롯에 존재하는 아이템의 최대수량을 넘길 수 없습니다.") );
		}
		break;
	}
	if( strMessage.IsEmpty() == FALSE )
	{
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );

		if( m_pPutArea != NULL )
			m_pPutArea->clearIconData();

		SetPutImage(false);
		SetOkButtonEnable();
	}
}

WMSG_RESULT CUIMysteriousBead::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pPutArea &&
		( m_pPutArea->IsInside(x, y) || m_pPutOnArea->IsInside(x, y) ))
	{
		int ret = SetPutOnItemData();

		if (ret > eERR_NONE)
			ErrMessage(ret);
	}

	CUIManager::getSingleton()->ResetHoldBtn();
	return CUIBase::OnLButtonUp(x, y);
}

WMSG_RESULT CUIMysteriousBead::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pDragArea && m_pDragArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	if (m_pSearchPlayer)
	{
		if (m_pSearchPlayer->IsInside(x, y))
			m_pSearchPlayer->SetFocus(TRUE);
		else
			m_pSearchPlayer->SetFocus(FALSE);
	}

	CUIManager::getSingleton()->RearrangeOrder(UI_MYSTERYOUSBEAD, TRUE);

	return CUIBase::OnLButtonDown(x, y);
}

WMSG_RESULT CUIMysteriousBead::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

WMSG_RESULT CUIMysteriousBead::OnKeyMessage( MSG* pMsg )
{
	if( !m_pSearchPlayer || !m_pSearchPlayer->IsFocused() )
		return WMSG_FAIL;

	if( !m_bPlayerSerach )
		return WMSG_FAIL;

	switch( pMsg->wParam )
	{
	case VK_DELETE:
	case VK_BACK:
		{
			SetFoundPlayer(FALSE);
			SetOkButtonEnable();
		}
		break;
	}
	return CUIBase::OnKeyMessage(pMsg);
}

WMSG_RESULT CUIMysteriousBead::OnCharMessage( MSG* pMsg )
{
	if( !m_pSearchPlayer || !m_pSearchPlayer->IsFocused() )
		return WMSG_FAIL;

	if( !m_bPlayerSerach )
		return WMSG_FAIL;

	SetFoundPlayer(FALSE);
	SetOkButtonEnable();
	return CUIBase::OnCharMessage(pMsg);
}

WMSG_RESULT CUIMysteriousBead::OnIMEMessage( MSG* pMsg )
{
	if( !m_pSearchPlayer || !m_pSearchPlayer->IsFocused() )
		return WMSG_FAIL;

	if( !m_bPlayerSerach )
		return WMSG_FAIL;
	
	SetFoundPlayer(FALSE);
	SetOkButtonEnable();
	return CUIBase::OnIMEMessage(pMsg);
}

void CUIMysteriousBead::CountItemCallback()
{
	SQUAD llCount = UIMGR()->GetMsgBoxNumOnly()->GetData();

	if( llCount > 0 )
	{
		if (llCount > m_pSendItems->Item_Sum)
		{
			ErrMessage((int)eERR_ITEM_COUNT_OVER);
		}
		else
		{
			m_pSendItems->Item_Sum = llCount;
			m_pPutArea->setData(m_pSendItems);
			SetPutImage(true);
			SetOkButtonEnable();
		}
	}
}
