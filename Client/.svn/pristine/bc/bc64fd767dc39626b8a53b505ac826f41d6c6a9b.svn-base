#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIInvenCashBagBox.h>
#include <Engine/Help/Util_Help.h>

#include "UIInvenCashBag.h"

#define CASH_SLOT_SY	(99)
#define TITLE_SX		(102)
#define TITLE_SY		(9)
#define TEXT_GAP		(130)
CUIInvenCashBag::CUIInvenCashBag( int _bag_index ) 
	: CUIChildInvenSlot(_bag_index)
	,	m_bLock(true)
	,	m_strTime("")
	,	m_nCashBagType(-1)
	,	m_bPremiumChar(false)
{
}

void CUIInvenCashBag::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	m_pParent = (CUIBase*)pParentWnd;
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );

	m_rcTitle.SetRect( 0, 0, ITEM_SLOT_TOTAL_WIDTH, ITEM_SLOT_TITLE_HEIGHT );
	m_rcItemSlot.SetRect( ITEM_SLOT_SX, CASH_SLOT_SY, ITEM_SLOT_SX + ITEM_SLOT_WIDTH, CASH_SLOT_SY + ITEM_SLOT_HEIGHT );

	// Create inventory texture
	m_ptdBGSlotTexture = CreateTexture(CTString("Data\\Interface\\IrisInventory.tex"));
	FLOAT fTexWidth = m_ptdBGSlotTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdBGSlotTexture->GetPixHeight();

	m_rsItemSlotSurface.AddRectSurface(UIRect(0, 0, nWidth, nHeight),
		UIRectUV(0, 0, INVEN_CASH_BAG_W, INVEN_CASH_BAG_H,fTexWidth,fTexHeight));	 // 배경 슬롯 UV

	m_rcLockArea.SetRect(30, 39, 30 + 37, 39 + 47);		// 자물쇠 세팅
	m_uvLockArea[eLOCK_ON].SetUV(212, 68, 249, 120, fTexWidth, fTexHeight);
	m_uvLockArea[eLOCK_OFF].SetUV(212, 7, 249, 54, fTexWidth, fTexHeight);

	// Create inventory texture
	m_ptdBaseTexture = CreateTexture(CTString("Data\\Interface\\NewInventory.tex"));
	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_SlotInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(3,441,36,474,fTexWidth,fTexHeight)); // 인벤 아이템 공간 선택이미지
	m_SlotInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(41,441,74,474,fTexWidth,fTexHeight)); // 인벤 장비 공간 선택이미지
	m_SlotInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(79,441,112,474,fTexWidth,fTexHeight)); // 인벤 조합 중인 버튼 이미지
	m_SlotInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(117,441,150,474,fTexWidth,fTexHeight)); // 인벤 봉인 중인 버튼 이미지
	m_SlotInvenSurface.AddRectSurface(UIRect(0,0,0,0), UIRectUV(173,421,184,432,fTexWidth,fTexHeight)); // 인벤 대여 마크 이미지

	m_NewItemEffectSurface.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(157, 441, 191, 475, fTexWidth, fTexHeight)); // 새로 습득한 아이템 깜빡이 연출이미지

	m_ptdCommonTexture = CreateTexture(CTString("Data\\Interface\\CommonBtn.tex"));
	fTexWidth = m_ptdCommonTexture->GetPixWidth();
	fTexHeight = m_ptdCommonTexture->GetPixHeight();

	m_btnClose.Create(this, CTString(""), ITEM_SLOT_TOTAL_WIDTH - CLOSE_BTN_SIZE - CLOSE_BTN_WIDTH_GAP, 4, CLOSE_BTN_SIZE, CLOSE_BTN_SIZE);
	m_btnClose.SetUV(UBS_IDLE, 211, 33, 226, 48, fTexWidth, fTexHeight);
	m_btnClose.SetUV(UBS_CLICK, 229, 33, 244, 48, fTexWidth, fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE, UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);

	m_pImgSelect = UISUPPORT()->clone_Image(eIMG_TYPE_SELECT);
	m_pImgSelect->setParent(this);
}

void CUIInvenCashBag::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	m_SlotInvenSurface.SetPos(m_nPosX, m_nPosY);
	m_NewItemEffectSurface.SetPos(m_nPosX, m_nPosY);
	
	{
		pDrawPort->InitTextureData( m_ptdBGSlotTexture );

		m_rsItemSlotSurface.SetPos(m_nPosX, m_nPosY);
		m_rsItemSlotSurface.RenderRectSurface(pDrawPort, 0xFFFFFFFF);
		if( !IsInvenCashBagLock() )
		{
			pDrawPort->AddTexture( m_nPosX + m_rcLockArea.Left, m_nPosY + m_rcLockArea.Top, m_nPosX + m_rcLockArea.Right, m_nPosY + m_rcLockArea.Bottom,
				m_uvLockArea[eLOCK_ON].U0, m_uvLockArea[eLOCK_ON].V0, m_uvLockArea[eLOCK_ON].U1, m_uvLockArea[eLOCK_ON].V1, 0xFFFFFFFF );
		}
		else
		{
			pDrawPort->AddTexture( m_nPosX + m_rcLockArea.Left, m_nPosY + m_rcLockArea.Top, m_nPosX + m_rcLockArea.Right, m_nPosY + m_rcLockArea.Bottom,
				m_uvLockArea[eLOCK_OFF].U0, m_uvLockArea[eLOCK_OFF].V0, m_uvLockArea[eLOCK_OFF].U1, m_uvLockArea[eLOCK_OFF].V1, 0xFFFFFFFF );
		}
		pDrawPort->FlushRenderingQueue();
	}
	
	pDrawPort->InitTextureData( m_ptdCommonTexture );
	m_btnClose.Render();
	pDrawPort->FlushRenderingQueue();

	RenderExpireTime(pDrawPort);

	CUIChildInvenSlot::ItemRender();
	CUIChildInvenSlot::PostRender();
}

void CUIInvenCashBag::_openUI( const BOOL bType )
{
	if (IsVisible()) // 중복 실행 방지
		return;
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->RearrangeOrder( m_nRearrangeOrderID, TRUE );

	for (int i = 0; i < INVEN_ONE_BAG; i++)
	{
		SetStartItemEffect(i);
	}
}

void CUIInvenCashBag::_closeUI()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( m_nRearrangeOrderID, FALSE );
	ClearNewItemEffect();
}

void CUIInvenCashBag::SetInventoryAllItem( btnAllItems* pAllItem, int nTab )
{
	m_pAllItem = pAllItem;

	int i;
	int nX, nY;
	int tmpX = 14, tmpY = 99;
	int btnGap = 4;
	for( i = 0; i < ITEM_COUNT_IN_INVENTORY_CASH_1; i++ )
	{
		nX = tmpX + ((BTN_SIZE + btnGap) * (i % INVEN_SLOT_COL));
		nY = tmpY + ((BTN_SIZE + btnGap) * ((i / INVEN_SLOT_COL) % INVEN_SLOT_COL));

		(*m_pAllItem)[nTab][i] = new CUIIcon;
		(*m_pAllItem)[nTab][i]->initialize();
		(*m_pAllItem)[nTab][i]->Hide(TRUE);
		(*m_pAllItem)[nTab][i]->SetWhichUI(UI_INVENTORY);

		(*m_pAllItem)[nTab][i]->InitPos(nX, nY, BTN_SIZE, BTN_SIZE);
		(*m_pAllItem)[nTab][i]->setParent(this);
	}
}

WMSG_RESULT CUIInvenCashBag::MouseMessage( MSG *pMsg )
{
	// Title bar
	static BOOL bLButtonClick = FALSE;

	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
	case WM_LBUTTONDOWN:
		{
			if ( IsInsideRect(nX, nY, m_rcLockArea) )
			{
				bLButtonClick = TRUE;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			if ( bLButtonClick && IsInsideRect(nX, nY, m_rcLockArea) )
			{
				if( IsInvenCashBagLock() )
				{
					int t = m_nIndexBag - INVEN_TAB_CASH1 == 0 ? INVENTORY_TAB_CASH_1 : INVENTORY_TAB_CASH_2;
					pUIManager->GetInvenCashBagBox()->OpenUI(t);
				}
			}
		}
		break;
	}
	return CUIChildInvenSlot::MouseMessage(pMsg);
}

void CUIInvenCashBag::SetCashBagExpireTime(int uiType)
{
	UtilHelp* pHelp = UtilHelp::getSingleton();

	if( _pUIBuff->GetMyTimerItemBuffExist(uiType) == true )
	{
		CUIManager::getSingleton()->GetInvenCashBagBox()->CloseUI();

		int nTime = _pUIBuff->GetMyTimerItemBuffExpireTime(uiType);

		tm* pTime = NULL;
		pTime = localtime((time_t*)&nTime);
		m_strTime.PrintF( _S(6068, "%d년%d월%d일%d시"), pTime->tm_year + 1900, pTime->tm_mon + 1, pTime->tm_mday, pTime->tm_hour );
		m_bLock = false;
	}
	else
	{
		m_bLock = true;
	}
}

void CUIInvenCashBag::RenderExpireTime(CDrawPort* pDrawPort)
{
	if (pDrawPort == NULL)
		return;

	CTString strMessage;
	
#ifdef PREMIUM_CHAR
	if (_pUIBuff->GetMyTimerItemBuffExist(TIMERITEM_TYPE_CASH_INVENTORY_1) == false && m_bPremiumChar == true)
	{
		m_bLock = false;
		strMessage.PrintF( _S(6328, "프리미엄 캐릭터 혜택 적용 중"));
		pDrawPort->PutTextExCX( strMessage, m_nPosX + TEXT_GAP, m_nPosY + m_rcLockArea.Top + 10, 0xD67FFFFF );	// red

		pDrawPort->EndTextEx();
	}
	else
#endif	//	PREMIUM_CHAR
	{
		if( IsInvenCashBagLock() )
		{
			strMessage.PrintF( _S(6061, "사용기간 만료"));
			pDrawPort->PutTextExCX( strMessage, m_nPosX + TEXT_GAP, m_nPosY + m_rcLockArea.Top + 10, 0xFF0000FF );	// red
		}
		else
		{
			strMessage.PrintF( _S(6067, "남은 사용기간 :"));
			pDrawPort->PutTextExCX( strMessage, m_nPosX + TEXT_GAP, m_nPosY + m_rcLockArea.Top + 10, 0xFF0000FF);
			pDrawPort->PutTextExCX( m_strTime, m_nPosX + TEXT_GAP, m_nPosY + m_rcLockArea.Top + 30, 0xFFFFFFFF );
		}
		pDrawPort->EndTextEx();
	}

	strMessage.PrintF( _S(6060, "%d번 아이리스 가방"), m_nIndexBag - INVEN_TAB_CASH1 + 1);
	pDrawPort->PutTextExCX( strMessage, m_nPosX + TITLE_SX, m_nPosY + TITLE_SY, 0xFFFFFFFF );	// 하늘색
	pDrawPort->EndTextEx();
}

void CUIInvenCashBag::SetPremiumBenefit( bool bUse )
{
	m_bPremiumChar = bUse;

	if (bUse == true)
		m_bLock = false;
	else
	{
		if (_pUIBuff->GetMyTimerItemBuffExist(TIMERITEM_TYPE_CASH_INVENTORY_1) == false)
			m_bLock = true;
	}
}
