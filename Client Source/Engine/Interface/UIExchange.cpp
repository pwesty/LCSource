#include "stdh.h"
// 헤더 정리. [12/1/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIExchange.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Interface/UIWareHouse.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UIWildPetInfo.h>
#include <Engine/Interface/UIPersonalShop.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/Contents/Base/UIMsgBoxNumeric_only.h>

extern INDEX g_iCountry;
extern INDEX g_iRejectExchange;	// [7/13/2009 rumist] reject request of exchange message.
// ----------------------------------------------------------------------------
// Name : CUIExchange()
// Desc : Constructor
// ----------------------------------------------------------------------------

class CmdExChangeAddItem : public Command
{
public:
	CmdExChangeAddItem() : m_pWnd(NULL)	{}
	void setData(CUIExchange* pWnd)	{ m_pWnd = pWnd;	}
	void execute() 
	{
		if (m_pWnd != NULL)
			m_pWnd->AddItemCallback();
	}
private:
	CUIExchange* m_pWnd;
};

CUIExchange::CUIExchange()
{
	m_nSelMyItemID = -1;
	m_nSelITab = -1;
	m_nSelInvenItemID = -1;
	m_bShowItemInfo = FALSE;
	m_bDetailItemInfo = FALSE;
	m_nCurInfoLines = 0;

	m_bAmISrc = FALSE;
	m_nSrcIndex = -1;
	m_nDestIndex = -1;
	m_strSrcName = CTString( "" );
	m_strDestName = CTString( "" );
	m_bExchReady = FALSE;
	m_bExchReadyDest = FALSE;
	m_strMyCurMoney = CTString( "0" );
	m_strOppExchMoney = CTString( "0" );
	m_strMyExchMoney = CTString( "0" );
}

// ----------------------------------------------------------------------------
// Name : ~CUIExchange()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIExchange::~CUIExchange()
{
	int		i;

	for (i = 0; i < EXCH_EXCH_SLOT_TOTAL; ++i)
	{
		SAFE_DELETE(m_pIconsOppItems[i]);
		SAFE_DELETE(m_pIconsMyItems[i]);
	}

	for (i = 0; i < DEF_INVENTORY_MAX; ++i)
	{
		SAFE_DELETE(m_pIconsInvenItems[i]);
	}
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcOppSlot.SetRect( 6, 27, 191, 142 );
	m_rcMySlot.SetRect( 6, 149, 191, 245 );
	m_rcInvenSlot.SetRect( 6, 253, 192, 417 );
	m_rtTouchNas.SetRect( 75, 385, 190, 405);
	// Create exchange texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Exchange.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackground.SetUV( 0, 0, 216, 452, fTexWidth, fTexHeight );

	// Item information region
	m_rtInfoL.SetUV( 0, 478, 7, 500, fTexWidth, fTexHeight );
	m_rtInfoM.SetUV( 10, 478, 12, 500, fTexWidth, fTexHeight );
	m_rtInfoR.SetUV( 15, 478, 22, 500, fTexWidth, fTexHeight );
	m_rtInfoUL.SetUV( 0, 478, 7, 485, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 10, 478, 12, 485, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 15, 478, 22, 485, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 0, 488, 7, 490, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 10, 488, 12, 490, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 15, 488, 22, 490, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 0, 493, 7, 500, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 10, 493, 12, 500, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 15, 493, 22, 500, fTexWidth, fTexHeight );

	// Outline of item that isn't exchangeable
	m_rtWearingOutline.SetUV( 218, 115, 250, 147, fTexWidth, fTexHeight );

	// Outline of selected item
	m_rtSelectOutline.SetUV( 218, 80, 250, 112, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Ready button
	m_btnReady.Create( this, _S( 148, "준비 완료" ), 11, 420, 63, 21 );
	m_btnReady.SetUV( UBS_IDLE, 0, 454, 63, 475, fTexWidth, fTexHeight );
	m_btnReady.SetUV( UBS_CLICK, 64, 454, 127, 475, fTexWidth, fTexHeight );
	m_btnReady.SetUV( UBS_DISABLE, 128, 454, 191, 475, fTexWidth, fTexHeight );
	m_btnReady.CopyUV( UBS_IDLE, UBS_ON );	

	// Exchange button
	m_btnExchange.Create( this, _S( 149, "교환" ), 76, 420, 63, 21 );
	m_btnExchange.SetUV( UBS_IDLE, 0, 454, 63, 475, fTexWidth, fTexHeight );
	m_btnExchange.SetUV( UBS_CLICK, 64, 454, 127, 475, fTexWidth, fTexHeight );
	m_btnExchange.SetUV( UBS_DISABLE, 128, 454, 191, 475, fTexWidth, fTexHeight );
	m_btnExchange.CopyUV( UBS_IDLE, UBS_ON );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), 141, 420, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 454, 63, 475, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 64, 454, 127, 475, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Scroll bar
	m_sbScrollBar.Create( this, 195, 249, 9, 137 );
	m_sbScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetScrollRange( DEF_INVENTORY_MAX / EXCH_EXCH_SLOT_COL );
	m_sbScrollBar.SetCurItemCount( DEF_INVENTORY_MAX / EXCH_EXCH_SLOT_COL );
	m_sbScrollBar.SetItemsPerPage( EXCH_INVEN_SLOT_ROW );
	// Up button
	m_sbScrollBar.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbScrollBar.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbScrollBar.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbScrollBar.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbScrollBar.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Wheel region
	m_sbScrollBar.SetWheelRect( -193, -5, 192, 145 );
	

	// Opposite items
	int	iBtn;
	int	nBtnX = EXCH_SLOT_SX;
	int	nBtnY = EXCH_OPP_SLOT_SY;
	for( iBtn = 0; iBtn < EXCH_EXCH_SLOT_TOTAL; iBtn++ )
	{
		if( iBtn == EXCH_EXCH_SLOT_COL )
		{
			nBtnX = EXCH_SLOT_SX;	nBtnY += 35;
		}

		//m_pIconsOppItems[iBtn].Create( this, nBtnX, nBtnY, BTN_SIZE, BTN_SIZE, UI_EXCHANGE, UBET_ITEM );
		m_pIconsOppItems[iBtn] = new CUIIcon();
		m_pIconsOppItems[iBtn]->Create(this, nBtnX, nBtnY, BTN_SIZE, BTN_SIZE, UI_EXCHANGE, UBET_ITEM);
		nBtnX += 35;
	}

	// Opposite items
	nBtnX = EXCH_SLOT_SX;
	nBtnY = EXCH_MY_SLOT_SY;
	for( iBtn = 0; iBtn < EXCH_EXCH_SLOT_TOTAL; iBtn++ )
	{
		if( iBtn == EXCH_EXCH_SLOT_COL )
		{
			nBtnX = EXCH_SLOT_SX;	nBtnY += 35;
		}

		//m_pIconsMyItems[iBtn].Create( this, nBtnX, nBtnY, BTN_SIZE, BTN_SIZE, UI_EXCHANGE, UBET_ITEM );
		m_pIconsMyItems[iBtn] = new CUIIcon();
		m_pIconsMyItems[iBtn]->Create(this, nBtnX, nBtnY, BTN_SIZE, BTN_SIZE, UI_EXCHANGE, UBET_ITEM);
		nBtnX += 35;
	}

	// Slot items
	for (int i = 0; i < DEF_INVENTORY_MAX; ++i)
	{
		//m_pIconsInvenItems[i].Create( this, 0, 0, BTN_SIZE, BTN_SIZE, UI_EXCHANGE, UBET_ITEM, 0, i );
		m_pIconsInvenItems[i] = new CUIIcon();
		m_pIconsInvenItems[i]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_EXCHANGE, UBET_ITEM);
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : PrepareExchange()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::PrepareExchange()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_WAREHOUSE))
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S(1890 ,"창고 사용중에는 교환을 할수 없습니다." ) );	
		pUIManager->GetWareHouse()->ResetWareHouse();
	}

	// Close message box of inventory
	pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_EXCHANGE );

	// Initialize inventory items of exchange
	int		i;
	int		tab, idx;

	for (i = 0; i < DEF_INVENTORY_MAX; ++i)
	{
		if (i >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
		{
			tab = INVENTORY_TAB_CASH_2;
			idx = i - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
		}
		else if (i >= ITEM_COUNT_IN_INVENTORY_NORMAL)
		{
			tab = INVENTORY_TAB_CASH_1;
			idx = i - ITEM_COUNT_IN_INVENTORY_NORMAL;
		}
		else
		{
			tab = INVENTORY_TAB_NORMAL;
			idx = i;
		}

		CItems*		pItems = &_pNetwork->MySlotItem[tab][idx];

		if( pItems->Item_Sum > 0 )
		{
			CItems* pClone = new CItems;
			memcpy(pClone, pItems, sizeof(CItems));
			m_pIconsInvenItems[i]->setData(pClone, false);
			m_pIconsInvenItems[i]->setCount(pItems->Item_Sum);
		}
	}

	// Set money
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		m_strMyCurMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		pUIManager->InsertCommaToString( m_strMyCurMoney );
	}

	// Set state of controls
	m_btnReady.SetEnable( TRUE );
	m_btnExchange.SetEnable( FALSE );

	pUIManager->RearrangeOrder( UI_EXCHANGE, TRUE );
}

// ----------------------------------------------------------------------------
// Name : ResetExchange()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ResetExchange()
{
	m_nSelMyItemID = -1;
	m_nSelInvenItemID = -1;
	m_bShowItemInfo = FALSE;
	m_bDetailItemInfo = FALSE;
	m_nCurInfoLines = 0;

	m_bAmISrc = FALSE;
	m_nSrcIndex = -1;
	m_nDestIndex = -1;
	m_strSrcName = CTString( "" );
	m_strDestName = CTString( "" );
	m_bExchReady = FALSE;
	m_bExchReadyDest = FALSE;
	m_strMyCurMoney = CTString( "0" );
	m_strOppExchMoney = CTString( "0" );
	m_strMyExchMoney = CTString( "0" );
	m_nMyNas = 0;
	m_nTradeNas = 0;

	m_sbScrollBar.SetScrollPos( 0 );

	// Initialize buttons
	for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
	{
		m_pIconsOppItems[iItem]->clearIconData();
		m_pIconsMyItems[iItem]->clearIconData();
	}
	for (int i = 0; i < DEF_INVENTORY_MAX; ++i)
	{
		m_pIconsInvenItems[i]->clearIconData();
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Unlock inventory
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_EXCHANGE );

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_EXCHANGE );

	// Close message box of exchange
	pUIManager->GetMsgBoxNumOnly()->CloseBox();
	pUIManager->CloseMessageBox( MSGCMD_EXCH_DEL_ITEM );
	pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );
	pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_DEST );

	pUIManager->RearrangeOrder( UI_EXCHANGE, FALSE );
}

// ----------------------------------------------------------------------------
// Name : RenderItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::RenderItems()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Opposite items
	int	iItem;
	for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
	{
		if (m_pIconsOppItems[iItem]->IsEmpty() == true)
			continue;

		m_pIconsOppItems[iItem]->Render(pDrawPort);
	}

	// My items
	for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
	{
		if (m_pIconsMyItems[iItem]->IsEmpty() == true)
			continue;

		m_pIconsMyItems[iItem]->Render(pDrawPort);
	}

	// Inven items
	int	nX = EXCH_SLOT_SX, nY = EXCH_INVEN_SLOT_SY;
	int	nIdx;
	int	iRowS = m_sbScrollBar.GetScrollPos() * EXCH_INVEN_SLOT_COL;
	int	iRowE = (m_sbScrollBar.GetScrollPos() + EXCH_INVEN_SLOT_ROW) * EXCH_INVEN_SLOT_COL;
	for (nIdx = iRowS; nIdx < iRowE; ++nIdx)
	{
		if ((nIdx % EXCH_INVEN_SLOT_COL) == 0)
		{
			nX = EXCH_SLOT_SX;	
			if (nIdx > iRowS)	nY += 35;
		}

		m_pIconsInvenItems[nIdx]->SetPos( nX, nY );

		nX += 35;

		if (m_pIconsInvenItems[nIdx]->IsEmpty() == true)
			continue;

		m_pIconsInvenItems[nIdx]->Render(pDrawPort);
	}

	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

	pDrawPort->EndTextEx();

	// Set exchange texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Outline of ready items ( not movable )
	// Opposite items
	if( m_bExchReadyDest )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if (m_pIconsOppItems[iItem]->IsEmpty() == true)
				continue;

			m_pIconsOppItems[iItem]->GetAbsPos( nX, nY );
			pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtWearingOutline.U0, m_rtWearingOutline.V0,
												m_rtWearingOutline.U1, m_rtWearingOutline.V1,
												0xFFFFFFFF );
		}
	}
	// My items
	if( m_bExchReady )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if (m_pIconsMyItems[iItem]->IsEmpty() == true)
				continue;

			m_pIconsMyItems[iItem]->GetAbsPos( nX, nY );
			pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtWearingOutline.U0, m_rtWearingOutline.V0,
												m_rtWearingOutline.U1, m_rtWearingOutline.V1,
												0xFFFFFFFF );
		}
	}

	int		tab, idx;

	// Outline of items in inventory slot of exchange ( not exchangeable )
	for (nIdx = iRowS; nIdx < iRowE; ++nIdx)
	{
		if (nIdx >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
		{
			tab = INVENTORY_TAB_CASH_2;
			idx = nIdx - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
		}
		else if (nIdx >= ITEM_COUNT_IN_INVENTORY_NORMAL)
		{
			tab = INVENTORY_TAB_CASH_1;
			idx = nIdx - ITEM_COUNT_IN_INVENTORY_NORMAL;
		}
		else
		{
			tab = INVENTORY_TAB_NORMAL;
			idx = nIdx;
		}

		// Not exchangeable
		CItems*		pItems = &_pNetwork->MySlotItem[tab][idx];
		CItemData*	pItemData = pItems->ItemData;

		// If button is empty or not weraing item
		if (m_pIconsInvenItems[nIdx]->IsEmpty() == true)
			continue;

		if (!(pItems->Item_Flag& FLAG_ITEM_BELONG))
		{
#ifdef ADD_SUBJOB

			if (pItemData != NULL && pItemData->IsFlag(ITEM_FLAG_SELLER))
			{
				if (pUIManager->CheckSellerItem(UI_EXCHANGE, pItemData->GetFlag()))
					continue;
			}
			else
#endif
			{
				if ((pItemData != NULL && pItemData->GetFlag() & ITEM_FLAG_EXCHANGE))
					continue;
			}
		}

		m_pIconsInvenItems[nIdx]->GetAbsPos( nX, nY );
		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtWearingOutline.U0, m_rtWearingOutline.V0,
											m_rtWearingOutline.U1, m_rtWearingOutline.V1,
											0xFFFFFFFF );
	}

	// Outline of selected item
	if( m_nSelMyItemID >= 0 )
	{
		m_pIconsMyItems[m_nSelMyItemID]->GetAbsPos( nX, nY );
		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtSelectOutline.U0, m_rtSelectOutline.V0,
											m_rtSelectOutline.U1, m_rtSelectOutline.V1,
											0xFFFFFFFF );
	}
	if( m_nSelInvenItemID >= 0 )
	{
		int	nSelRow = m_nSelInvenItemID / EXCH_EXCH_SLOT_COL;
		if( nSelRow >= iRowS && nSelRow < iRowE )
		{
			m_pIconsInvenItems[m_nSelInvenItemID]->GetAbsPos( nX, nY );
			pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelectOutline.U0, m_rtSelectOutline.V0,
												m_rtSelectOutline.U1, m_rtSelectOutline.V1,
												0xFFFFFFFF );
		}
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Set exchange texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );

	// Close button
	m_btnClose.Render();

	// Ready button
	m_btnReady.Render();

	// Exchange button
	m_btnExchange.Render();

	// Cancel button
	m_btnCancel.Render();

	// Scroll bar
	m_sbScrollBar.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Text in exchange
	// Title
	pDrawPort->PutTextEx( _S( 149, "교환" ), m_nPosX + EXCH_TITLE_TEXT_OFFSETX,
										m_nPosY + EXCH_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	// Opposite name
	if( m_bAmISrc )
	{
		pDrawPort->PutTextExCX( m_strDestName, m_nPosX + EXCH_OPP_NAME_OFFSETX,
												m_nPosY + EXCH_OPP_NAME_OFFSETY );
	}
	else
	{
		pDrawPort->PutTextExCX( m_strSrcName, m_nPosX + EXCH_OPP_NAME_OFFSETX,
												m_nPosY + EXCH_OPP_NAME_OFFSETY );
	}

	// Date : 2005-11-11(오전 10:48:01), By Lee Ki-hwan 
	// 나스 색 추가 
	// Money
	COLOR colNas = 0;

	colNas = pUIManager->GetNasColor( m_strOppExchMoney );
	pDrawPort->PutTextExRX( m_strOppExchMoney, m_nPosX + EXCH_MONEY_OFFSETX,
											m_nPosY + EXCH_OPP_MONEY_OFFSETY, colNas );
	
	colNas = pUIManager->GetNasColor( m_strMyExchMoney );
	pDrawPort->PutTextExRX( m_strMyExchMoney, m_nPosX + EXCH_MONEY_OFFSETX,
											m_nPosY + EXCH_MY_MONEY_OFFSETY, colNas );

	colNas = pUIManager->GetNasColor( m_strMyCurMoney );
	pDrawPort->PutTextExRX( m_strMyCurMoney, m_nPosX + EXCH_MONEY_OFFSETX,
											m_nPosY + EXCH_MYCUR_MONEY_OFFSETY, colNas );

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Items
	RenderItems();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIExchange::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInItem && ( pMsg->wParam & MK_LBUTTON ) &&
				( ndX != 0 || ndY != 0 ) )
			{
				// My items
				/*if( !m_bExchReady && m_nSelMyItemID >= 0 )		// 아이템 삭제 막음
				{
					// Close message box of exchange
					pUIManager->CloseMessageBox( MSGCMD_EXCH_ADD_ITEM );
					pUIManager->CloseMessageBox( MSGCMD_EXCH_DEL_ITEM );

					pUIManager->SetHoldBtn( m_abtnMyItems[m_nSelMyItemID] );
					int	nOffset = BTN_SIZE / 2;
					pUIManager->GetHoldBtn().SetPos( nX - nOffset, nY - nOffset );

					m_abtnMyItems[m_nSelMyItemID].SetBtnState( UBES_IDLE );
				}
				// Inven items
				else*/ if( !m_bExchReady && m_nSelInvenItemID >= 0 )
				{

					// Exchangeable
					// [2010/10/20 : Sora] 사용중인 몬스터 용병 카드는 교환 불가
					CItems*		pItems = &_pNetwork->MySlotItem[m_nSelITab][m_nSelInvenItemID];
					CItemData*	pItemData = pItems->ItemData;

					if (pItemData == NULL)
						return WMSG_FAIL;

					if (pItems->IsFlag(FLAG_ITEM_BELONG))
					{
						bLButtonDownInItem = FALSE;
						return WMSG_FAIL;
					}
#ifdef ADD_SUBJOB
					if( pItemData->IsFlag( ITEM_FLAG_SELLER ) ? 
						pUIManager->CheckSellerItem(UI_EXCHANGE, pItemData->GetFlag()) : 
						( pItemData->GetFlag() & ITEM_FLAG_EXCHANGE || ( pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD &&
						pItems->Item_Used <= 0 ) ) )
#else
					if( pItemData->GetFlag() & ITEM_FLAG_EXCHANGE || 
						( pItemData->GetType() == CItemData::ITEM_ETC && 
						  pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD &&
						  pItems->Item_Used < 1 ) )
#endif
					{
						// Close message box of exchange
						int idx = m_nSelInvenItemID;

						if (m_nSelITab >= INVENTORY_TAB_CASH_1)
							idx += ITEM_COUNT_IN_INVENTORY_NORMAL;
						if (m_nSelITab >= INVENTORY_TAB_CASH_2)
							idx += ITEM_COUNT_IN_INVENTORY_CASH_1;

						pUIManager->SetHoldBtn(m_pIconsInvenItems[idx]);
					}
				}

				bLButtonDownInItem = FALSE;
			}

			// Move exchange
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Ready button
			else if( m_btnReady.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Exchange button
			else if( m_btnExchange.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Scroll bar
			else if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				int	iItem;
				for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
				{
					m_pIconsOppItems[iItem]->MouseMessage( pMsg );

					m_pIconsMyItems[iItem]->MouseMessage( pMsg );
				}

				int	nIdx;
				int	iRowS = m_sbScrollBar.GetScrollPos();
				int	iRowE = iRowS + EXCH_INVEN_SLOT_ROW;
				
				for (nIdx = (iRowS * EXCH_INVEN_SLOT_COL); nIdx < (iRowE * EXCH_INVEN_SLOT_COL); ++nIdx)
				{
					m_pIconsInvenItems[nIdx]->MouseMessage( pMsg );
				}

				return WMSG_SUCCESS;
			}
			// Opposite items
		/*	else if( IsInsideRect( nX, nY, m_rcOppSlot ) )
			{
				int	iItem;
				int	nWhichItem = -1;
				for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
				{
					if( m_abtnOppItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						nWhichItem = iItem;
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, nWhichItem );

				return WMSG_SUCCESS;
			}
			// My items
			else if( IsInsideRect( nX, nY, m_rcMySlot ) )
			{
				int	iItem;
				int	nWhichItem = -1;
				for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
				{
					if( m_abtnMyItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						nWhichItem = iItem;
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, -1, nWhichItem );

				return WMSG_SUCCESS;
			}
			// Inven items
			else if( IsInsideRect( nX, nY, m_rcInvenSlot ) )
			{
				int	iRow, iCol;
				int	iRowS = m_sbScrollBar.GetScrollPos();
				int	iRowE = iRowS + EXCH_INVEN_SLOT_ROW;
				int	nWhichRow = -1, nWhichCol;
				for( iRow = iRowS; iRow < iRowE; iRow++ )
				{
					for( iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++ )
					{
						if( m_abtnInvenItems[iRow][iCol].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							nWhichRow = iRow;	nWhichCol = iCol;
						}
					}
				}

				// Show tool tip
				ShowItemInfo( TRUE, FALSE, -1, -1, nWhichRow, nWhichCol );

				return WMSG_SUCCESS;
			}

			// Hide tool tip
			ShowItemInfo( FALSE );*/
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Opposite items
				else if( IsInsideRect( nX, nY, m_rcOppSlot ) )
				{
					pUIManager->RearrangeOrder( UI_EXCHANGE, TRUE );
					return WMSG_SUCCESS;
				}
				// My items
				/*else if( IsInsideRect( nX, nY, m_rcMySlot ) )		// 아이템 삭제 막음
				{
					m_nSelMyItemID = -1;
					m_nSelInvenItemID = -1;

					for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
					{
						if( m_abtnMyItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected item
							m_nSelMyItemID = iItem;

							// Show tool tip
							ShowItemInfo( TRUE, TRUE, -1, iItem );

							bLButtonDownInItem = TRUE;

							pUIManager->RearrangeOrder( UI_EXCHANGE, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}*/
				// Inventory items of exchange
				else if( IsInsideRect( nX, nY, m_rcInvenSlot ) )
				{
					m_nSelInvenItemID = -1;
					m_nSelMyItemID = -1;

					int	nIdx;
					int	iRowS = m_sbScrollBar.GetScrollPos();
					int	iRowE = iRowS + EXCH_INVEN_SLOT_ROW;
					
					for (nIdx = (iRowS * EXCH_INVEN_SLOT_COL); nIdx < (iRowE * EXCH_INVEN_SLOT_COL); ++nIdx)
					{
						if( m_pIconsInvenItems[nIdx]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							if (nIdx >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
							{
								m_nSelITab = INVENTORY_TAB_CASH_2;
								m_nSelInvenItemID = nIdx - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
							}
							else if (nIdx >= ITEM_COUNT_IN_INVENTORY_NORMAL)
							{
								m_nSelITab = INVENTORY_TAB_CASH_1;
								m_nSelInvenItemID = nIdx - ITEM_COUNT_IN_INVENTORY_NORMAL;
							}
							else
							{
								m_nSelITab = INVENTORY_TAB_NORMAL;
								m_nSelInvenItemID = nIdx;
							}

							// Show tool tup
							//ShowItemInfo( TRUE, TRUE, -1, -1, iRow, iCol );

							bLButtonDownInItem = TRUE;

							pUIManager->RearrangeOrder( UI_EXCHANGE, TRUE );
							return WMSG_SUCCESS;
						}
					}

				}
				// Ready button
				else if( m_btnReady.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Exchange button
				else if( m_btnExchange.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Scroll bar
				else if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}

				pUIManager->RearrangeOrder( UI_EXCHANGE, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			bLButtonDownInItem = FALSE;

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If exchange isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendExchangeReq_Rej();

					return WMSG_SUCCESS;
				}
				// Ready button
				else if( ( wmsgResult = m_btnReady.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendExchangeReq_Ready();

					return WMSG_SUCCESS;
				}
				// Exchange button
				else if( ( wmsgResult = m_btnExchange.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendExchangeReq_Ok();

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendExchangeReq_Rej();

					return WMSG_SUCCESS;
				}
				// Scroll bar
				else if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// Opposite item
				else if( IsInsideRect( nX, nY, m_rcOppSlot ) )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// My item
				else if( IsInsideRect( nX, nY, m_rcMySlot ) )
				{
					for( int iWear = 0; iWear < EXCH_EXCH_SLOT_TOTAL; iWear++ )
					{
						if (m_pIconsMyItems[iWear]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
				// Inventory items of exchange
				else if( IsInsideRect( nX, nY, m_rcInvenSlot ) )
				{
					int	nIdx;
					int	iRowS = m_sbScrollBar.GetScrollPos();
					int	iRowE = iRowS + EXCH_INVEN_SLOT_ROW;
					
					for (nIdx = (iRowS * EXCH_INVEN_SLOT_COL); nIdx < (iRowE * EXCH_INVEN_SLOT_COL); ++nIdx)
					{
						if (m_pIconsInvenItems[nIdx]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
				//2013/04/03 jeil 나스 아이템 제거 
				if( IsInsideRect( nX, nY, m_rtTouchNas ) )
				{
					if(!m_bExchReady)
						ExchangeNas();

					return WMSG_SUCCESS;
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// If holding button is item and is from exchange
					if (pUIManager->GetDragIcon()->getBtnType() == UBET_ITEM &&
						pUIManager->GetDragIcon()->GetWhichUI() == UI_EXCHANGE)
					{
						// My items
						if( IsInsideRect( nX, nY, m_rcMySlot ) )
						{
							CItems* pItems = pUIManager->GetDragIcon()->getItems();

							if (pItems == NULL)
								return WMSG_FAIL;

							// If this item is moved from inventory slot of exchange
							/*if( m_nSelMyItemID < 0 ||
								m_abtnMyItems[m_nSelMyItemID].GetBtnID() != pUIManager->GetHoldBtn().GetBtnID() )*/	// 아이템 삭제 막음
							{
								AddExchItem(pItems->Item_Tab, pItems->InvenIndex,
									pItems->Item_UniIndex, pItems->Item_Sum);

								// Reset holding button
								pUIManager->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
						// Inventory items of exchange
						/*else if( IsInsideRect( nX, nY, m_rcInvenSlot ) )		// 아이템 삭제 막음
						{
							int	nSelRow = m_nSelInvenItemID / EXCH_INVEN_SLOT_COL;
							int	nSelCol = m_nSelInvenItemID % EXCH_INVEN_SLOT_COL;
							// If this item is moved from my slot
							if( m_nSelInvenItemID < 0 ||
								m_abtnInvenItems[nSelRow][nSelCol].GetBtnID() != pUIManager->GetHoldBtn().GetBtnID() )
							{
								DelExchItem( pUIManager->GetHoldBtn().GetInvenIndex(),
												pUIManager->GetHoldBtn().GetItemCol(),
												pUIManager->GetHoldBtn().GetItemUniIndex(),
												pUIManager->GetHoldBtn().GetItemCount() );

								// Reset holding button
								pUIManager->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}*/
					}

					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// My items
				/*if( IsInsideRect( nX, nY, m_rcMySlot ) )		// 아이템 삭제 막음
				{
					m_nSelMyItemID = -1;

					for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
					{
						if( m_abtnMyItems[iItem].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							if( !m_bExchReady )
							{
								// Close message box of exchange
								pUIManager->CloseMessageBox( MSGCMD_EXCH_ADD_ITEM );
								pUIManager->CloseMessageBox( MSGCMD_EXCH_DEL_ITEM );

								DelExchItem( m_abtnMyItems[iItem].GetInvenIndex(),
												m_abtnMyItems[iItem].GetItemCol(),
												m_abtnMyItems[iItem].GetItemUniIndex(),
												m_abtnMyItems[iItem].GetItemCount() );
							}

							// Show tool tip
							ShowItemInfo( TRUE, TRUE, -1, iItem );

							return WMSG_SUCCESS;
						}
					}
				}
				// Inventory items of exchange
				else*/ if( IsInsideRect( nX, nY, m_rcInvenSlot ) )
				{
					m_nSelInvenItemID = -1;

					int tab, idx;
					int	nIdx;
					int	iRowS = m_sbScrollBar.GetScrollPos();
					int	iRowE = iRowS + EXCH_INVEN_SLOT_ROW;
					
					for (nIdx = (iRowS * EXCH_INVEN_SLOT_COL); nIdx < (iRowE * EXCH_INVEN_SLOT_COL); ++nIdx)
					{
						if( m_pIconsInvenItems[nIdx]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							if (m_pIconsInvenItems[nIdx]->IsEmpty() == true)
								return WMSG_FAIL;

							if (m_pIconsInvenItems[nIdx]->getItems()->GetToggle() == true)
								return WMSG_FAIL;

							if( !m_bExchReady )
							{
								if (nIdx >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
								{
									tab = INVENTORY_TAB_CASH_2;
									idx = nIdx - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
								}
								else if (nIdx >= ITEM_COUNT_IN_INVENTORY_NORMAL)
								{
									tab = INVENTORY_TAB_CASH_1;
									idx = nIdx - ITEM_COUNT_IN_INVENTORY_NORMAL;
								}
								else
								{
									tab = INVENTORY_TAB_NORMAL;
									idx = nIdx;
								}
								
								CUIManager* pUIManager = CUIManager::getSingleton();

								// Exchangeable
								CItems*		pItems = &_pNetwork->MySlotItem[tab][idx];
								CItemData*	pItemData = pItems->ItemData;

								if (pItemData == NULL)
									continue;
#ifdef ADD_SUBJOB
								if( pItemData->IsFlag( ITEM_FLAG_SELLER ) ? 
									pUIManager->CheckSellerItem(UI_EXCHANGE, pItemData->GetFlag()) : 
								( pItemData->GetFlag() & ITEM_FLAG_EXCHANGE || 
									( pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD &&
									pItems->Item_Used <= 0 ) ) )
#else
								if( pItemData->GetFlag() & ITEM_FLAG_EXCHANGE || 
									( pItemData->GetType() == CItemData::ITEM_ETC && 
									pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD &&
									pItems->Item_Used < 1 ))
#endif
								{
									// Close message box of exchange
									pUIManager->GetMsgBoxNumOnly()->CloseBox();
									pUIManager->CloseMessageBox( MSGCMD_EXCH_DEL_ITEM );

									AddExchItem( tab, idx, m_pIconsInvenItems[nIdx]->getItems()->Item_UniIndex,
										m_pIconsInvenItems[nIdx]->getItems()->Item_Sum);
								}
							}

							return WMSG_SUCCESS;
						}
					}
				}
				// Scroll bar
				else if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// Scroll bar
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //
static int		nTempUniIndex;
static SQUAD	llTempCount;

// ----------------------------------------------------------------------------
// Name : AddExchItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::AddExchItem( int nTab, int inven_idx, int nUniIndex, SQUAD llCount )
{
	if(nUniIndex == 19)
	{
		SendExchangeItem_Add( nUniIndex, llCount );
		return;
	}

	nTempUniIndex = nUniIndex;
	llTempCount = llCount;

	CItems*		pItems = &_pNetwork->MySlotItem[nTab][inven_idx];
	CItemData*	pItemData = pItems->ItemData;

	if (pItemData == NULL)
		return;

	if( ( pItemData->GetFlag() & ITEM_FLAG_COUNT ) && llTempCount > 1 )
	{
		CTString		strMessage;
		const char	*szItemName = _pNetwork->GetItemName(pItemData->GetItemIndex());
		strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

		CmdExChangeAddItem* pCmd = new CmdExChangeAddItem;
		pCmd->setData(this);
		UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 149, "교환" ), strMessage, 1, pItems->Item_Sum);
	}
	else
	{
		SendExchangeItem_Add( nTempUniIndex, llTempCount);
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_EXCH_REQ_SRC:
		if( !bOK )
		{
			m_nSrcIndex = -1;
			m_nDestIndex = -1;
			SendExchangeReq_Rej();
		}

		break;

	case MSGCMD_EXCH_REQ_DEST:
		if( bOK )
			SendExchangeReq_Rep();
		else
			SendExchangeReq_Rej();

		break;

	case MSGCMD_EXCH_ADD_NAS:
		{
			char	*pcInput	= strInput.str_String;
			int		nLength		= strInput.Length();
			int		iChar;
			for( iChar = 0; iChar < nLength; iChar++ )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}
			
			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				//if( llCount > 0 && llCount <= llTempCount )
				if( (llCount+m_nMyNas) > 0 && (llCount+m_nMyNas) <= _pNetwork->MyCharacterInfo.money)
				{
					m_nMyNas		+= llCount;
					m_strMyExchMoney.PrintF( "%I64d", m_nMyNas );
					CUIManager::getSingleton()->InsertCommaToString( m_strMyExchMoney );

					AddExchItem(-1, -1, 19, m_nMyNas);
					
				}
				else
				{
					CTString strInNasError;
					strInNasError.PrintF( _S( 5902, "현재 가지고 있는 나스 보다 많은 나스를 교환 할 수 없습니다." ));		
					_pNetwork->ClientSystemMessage( strInNasError );
				}
			}
		}
		
		break;
	}
}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendExchangeReq_Req()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::SendExchangeReq_Req( CTString &strDestName )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Already exchange
	if( m_nSrcIndex != -1 && m_nDestIndex != -1 )
	{
		CTString	strSysMessage;
		if( m_bAmISrc )
			strSysMessage.PrintF( _S2( 422, m_strDestName, "이미 %s<과> 교환중입니다." ), m_strDestName );
		else
			strSysMessage.PrintF( _S2( 422, m_strSrcName, "이미 %s<과> 교환중입니다." ), m_strSrcName );

		pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );

		return;
	}
	
	// Find index of dest character
	SLONG	slDestIndex = -1;	
	INDEX	iSyndicateType = 0;


	ObjectBase* pObject = ACTORMGR()->GetObjectByName(strDestName.str_String, eOBJ_CHARACTER);

	if (pObject != NULL)
	{
		CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

		if (pTarget->cha_sbShopType != PST_NOSHOP)
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 737, "개인 상점과는 교환을 할 수 없습니다." ), SYSMSG_ERROR );		
			return;
		}

		slDestIndex = pTarget->m_nIdxServer;
		iSyndicateType = pTarget->cha_iSyndicateType;

		if (_pNetwork->IsRvrZone() == TRUE)
		{
			if (iSyndicateType != _pNetwork->MyCharacterInfo.iSyndicateType)
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 6090, "분쟁 지역에서 적대 관계인 경우 사용할 수 없는 기능입니다."), SYSMSG_ERROR);
				return;
			}
		}
	}

	// If dest character doesn't exist
	if( slDestIndex == -1 )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 151, "교환 대상을 찾을 수 없습니다." ), SYSMSG_ERROR );
		return ;
	}

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	_pNetwork->ExchangeReq_Req( slDestIndex, strDestName );

	m_bAmISrc = TRUE;
	m_nSrcIndex = _pNetwork->MyCharacterInfo.index;
	m_nDestIndex = slDestIndex;
	m_strSrcName = _pNetwork->MyCharacterInfo.name;
	m_strDestName = strDestName;

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );

	// Create message box
	CTString		strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 149, "교환" ), UMBS_CANCEL,
								UI_EXCHANGE, MSGCMD_EXCH_REQ_SRC );
	strMessage.PrintF( _S( 152, "%s에게 교환을 신청중입니다. 취소하시겠습니까?" ), m_strDestName );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	// Character state flags
	pUIManager->SetCSFlagOn( CSF_EXCHANGE );
}

// ----------------------------------------------------------------------------
// Name : SendExchangeReq_Rep()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::SendExchangeReq_Rep()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		_pNetwork->ExchangeReq_Rej();
		return;
	}

	_pNetwork->ExchangeReq_Rep();
}

// ----------------------------------------------------------------------------
// Name : SendExchangeReq_Rej()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::SendExchangeReq_Rej()
{
	_pNetwork->ExchangeReq_Rej();
}

// ----------------------------------------------------------------------------
// Name : SendExchangeReq_Ready()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::SendExchangeReq_Ready()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of exchange
	pUIManager->CloseMessageBox( MSGCMD_EXCH_DEL_ITEM );

	_pNetwork->ExchangeReq_Ready();
	pUIManager->GetMsgBoxNumOnly()->CloseBox();
}

// ----------------------------------------------------------------------------
// Name : SendExchangeReq_Ok()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::SendExchangeReq_Ok()
{
	_pNetwork->ExchangeReq_Ok();
}

// ----------------------------------------------------------------------------
// Name : SendExchangeItem_Add()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::SendExchangeItem_Add( int nUniIndex, SQUAD llCount)
{

	_pNetwork->ExchangeItem_Add( nUniIndex, llCount );
}
// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ExchangeReq_Req()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Req( int nSrcIndex, CTString &strSrcName,
									int nDestIndex, CTString &strDestName )
{
	m_bAmISrc = FALSE;
	m_nSrcIndex = nSrcIndex;
	m_nDestIndex = nDestIndex;
	m_strSrcName = strSrcName;
	m_strDestName = strDestName;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_DEST );
	// rejection bug fix [8/5/2010 rumist]
	if( g_iRejectExchange )
	{
		// [7/13/2009 rumist] if i want reject request message, 
		// call send exchange request message reject function. 
		SendExchangeReq_Rej();
		return;
	}

	// Create message box
	CTString		strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 149, "교환" ), UMBS_YESNO,
								UI_EXCHANGE, MSGCMD_EXCH_REQ_DEST );
	strMessage.PrintF( _S2( 153, m_strSrcName, "%s<가> 교환을 신청했습니다. 수락하시겠습니까?" ), m_strSrcName );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	// Character state flags
	pUIManager->SetCSFlagOn( CSF_EXCHANGE );
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_Rep()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Rep()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strMessage;

	if (pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();

		SendExchangeReq_Rej();
		return;
	}

	if( m_bAmISrc )
	{
		pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );
		strMessage.PrintF( _S2( 154, m_strDestName, "%s<과> 교환을 시작합니다." ), m_strDestName );
	}
	else
		strMessage.PrintF( _S2( 154, m_strSrcName, "%s<과> 교환을 시작합니다." ), m_strSrcName );

	// Add system message
	pUIManager->GetChattingUI()->AddSysMessage( strMessage );

	// Prepare exchange
	PrepareExchange();
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_Rej()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Rej( BOOL bSrcReject )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strMessage;

	if( m_bAmISrc )
	{
		if( bSrcReject )
			strMessage.PrintF( _S2( 155, m_strDestName, "%s<과>의 교환을 취소하셨습니다." ), m_strDestName );
		else
		{
			pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );
			strMessage.PrintF( _S2( 156, m_strDestName, "%s<가> 교환을 취소했습니다." ), m_strDestName );
		}
	}
	else
	{
		if( bSrcReject )
		{
			pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_DEST );
			strMessage.PrintF( _S2( 156, m_strSrcName, "%s<가> 교환을 취소했습니다." ), m_strSrcName );
		}
		else
			strMessage.PrintF( _S2( 155, m_strSrcName, "%s<과>의 교환을 취소하셨습니다." ), m_strSrcName );
	}

	// Add system message
	pUIManager->GetChattingUI()->AddSysMessage( strMessage );

	// Reset exchange
	ResetExchange();
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_Ready()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Ready( BOOL bSrcReady )
{
	if( ( m_bAmISrc && bSrcReady ) || ( !m_bAmISrc && !bSrcReady ) )
	{
		m_bExchReady = TRUE;
		m_btnReady.SetEnable( FALSE );
	}
	else
	{
		m_bExchReadyDest = TRUE;
	}
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_BothReady()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_BothReady()
{
	m_bExchReady = TRUE;
	m_bExchReadyDest = TRUE;

	// Disable ready button
	m_btnReady.SetEnable( FALSE );

	// Enable exchange button
	m_btnExchange.SetEnable( TRUE );

	// Add system message
	CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 157, "교환 준비가 완료되었습니다." ), SYSMSG_ERROR );
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_Full()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Full( BOOL bSrcFull )
{
	CTString	strMessage;

	if( m_bAmISrc )
	{
		if( bSrcFull )
			strMessage = _S( 265, "인벤토리 공간이 부족합니다." );
		else
			strMessage.PrintF( _S( 158, "%s의 인벤토리 공간이 부족합니다." ), m_strDestName );
	}
	else
	{
		if( bSrcFull )
			strMessage.PrintF( _S( 158, "%s의 인벤토리 공간이 부족합니다." ), m_strSrcName );
		else
			strMessage = _S( 265, "인벤토리 공간이 부족합니다." );
	}

	m_bExchReady = FALSE;
	m_bExchReadyDest = FALSE;

	// Enable ready button
	m_btnReady.SetEnable( TRUE );

	// Disable exchange button
	m_btnExchange.SetEnable( FALSE );

	// Add system message
	CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_Ok()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Ok( BOOL bSrcOk )
{
	if( m_bAmISrc )
	{
		if( bSrcOk )
			m_btnExchange.SetEnable( FALSE );
	}
	else
	{
		if( !bSrcOk )
			m_btnExchange.SetEnable( FALSE );
	}
}

// ----------------------------------------------------------------------------
// Name : ExchangeReq_BothOk()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_BothOk()
{
	// Disable exchange button
	m_btnExchange.SetEnable( FALSE );

	// Add system message
	CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 159, "교환이 성공적으로 이루어졌습니다." ) );

	ResetExchange();
}


// ----------------------------------------------------------------------------
// Name : ExchangeReq_Pet_Err()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeReq_Pet_Err(BOOL petErr)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Disable exchange button
	m_btnExchange.SetEnable( FALSE );

	if(!petErr){
		// Add system message
		pUIManager->GetChattingUI()->AddSysMessage( _S(2221, "펫을 더 이상 소유할 수 없습니다." ),SYSMSG_ERROR );
	}
	else{
		CTString	strMessage;
		strMessage.PrintF(_S(2555, "%s님은 펫을 더 이상 소유할 수 없는 상태 입니다." ) ,m_strDestName);
		pUIManager->GetChattingUI()->AddSysMessage(strMessage,SYSMSG_ERROR); 
	}
		
	ResetExchange();
}

// ----------------------------------------------------------------------------
// Name : ExchangeItem_Add()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeItem_Add( BOOL bSrcAdd, int nUniIndex, SQUAD llCount,
									int nIndex, ULONG ulPlus, ULONG ulFlag, LONG ulUsed, LONG ulUsed2, LONG lRareIndex )
{
	// My items
	int		iItem;

	if ((m_bAmISrc && bSrcAdd) || (!m_bAmISrc && !bSrcAdd))
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsMyItems[iItem]->IsEmpty() )
				break;
		}
		
		CItems* pItems = new CItems;
		pItems->Item_Index = nIndex;
		pItems->Item_UniIndex = nUniIndex;
		pItems->Item_Sum = llCount;
		pItems->Item_Plus = ulPlus;
		pItems->Item_Used = ulUsed;
		pItems->Item_Used2 = ulUsed2;
		pItems->Item_Flag = ulFlag;
		pItems->SetRareIndex(lRareIndex);
		pItems->ItemData = CItemData::getData(nIndex);

		m_pIconsMyItems[iItem]->setData(pItems, false);

		CUIManager* pUIManager = CUIManager::getSingleton();

		if (ulFlag & FLAG_ITEM_SEALED)
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 1079, "봉인된 아이템은 교체할 수 없습니다." ), SYSMSG_ERROR );		
			m_pIconsMyItems[iItem]->clearIconData();
			return;
		}

		// Update inventory of exchange
		for (int nIdx = 0; nIdx < DEF_INVENTORY_MAX; ++nIdx)
		{
			if (m_pIconsInvenItems[nIdx]->getItems() != NULL && 
				m_pIconsInvenItems[nIdx]->getItems()->Item_UniIndex == nUniIndex)
			{
				// Set item location of my item
				pItems->Item_Tab = 0;
				pItems->InvenIndex = nIdx;

				// Update count
				SQUAD	llNewCount = m_pIconsInvenItems[nIdx]->getItems()->Item_Sum;
				llNewCount -= llCount;
				m_pIconsInvenItems[nIdx]->setCount(llNewCount);
				
				if( llNewCount <= 0 )
				{
					m_pIconsInvenItems[nIdx]->clearIconData();

					// Unselect item
					if( m_nSelInvenItemID == nIdx )
						m_nSelInvenItemID = -1;
				}

				// If item is money
				CItemData* pItemData = pItems->ItemData;

				if( pItemData->GetType() == CItemData::ITEM_ETC &&
					pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
				{
					m_strMyExchMoney.PrintF( "%I64d", llCount );
					pUIManager->InsertCommaToString( m_strMyExchMoney );
				}

				if(pItemData->GetType() == CItemData::ITEM_ACCESSORY &&
				pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET)
				{
					_pNetwork->SendPetitemInfo(_pNetwork->MyCharacterInfo.index,ulPlus);
				}

				return;
			}
		}
	}
	// Opposite items
	else
	{
		int		iItem;
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->IsEmpty() )
				break;
		}

		CItems* pItems = new CItems;
		pItems->Item_Index = nIndex;
		pItems->Item_UniIndex = nUniIndex;
		pItems->Item_Sum = llCount;
		pItems->Item_Plus = ulPlus;
		pItems->Item_Used = ulUsed;
		pItems->Item_Used2 = ulUsed2;
		pItems->Item_Flag = ulFlag;
		pItems->SetRareIndex(lRareIndex);
		pItems->ItemData = CItemData::getData(nIndex);

		m_pIconsOppItems[iItem]->setData(pItems, false);

		// If item is money
		CItemData* pItemData = _pNetwork->GetItemData( nIndex );

		if(pItemData->GetType() == CItemData::ITEM_ACCESSORY &&
			pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET)
		{
			_pNetwork->SendPetitemInfo(m_bAmISrc ? m_nDestIndex : m_nSrcIndex, ulPlus);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ExchangeItem_SetOptionData()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeItem_SetOptionData( BOOL bSrcAdd, int nUniIndex, SBYTE sbOption,
												SBYTE sbOptionType, LONG lOptionLevel, LONG lRareIndex, LONG lOriginOptionVar )
{
	// My items
	int		iItem;
	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if (m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex)
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		CItems* pItems = m_pIconsMyItems[iItem]->getItems();

		pItems->SetOptionData(sbOption, sbOptionType, lOptionLevel, lOriginOptionVar);
		pItems->SetRareIndex(lRareIndex);
	}
	// Opposite items
	else
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		CItems* pItems = m_pIconsOppItems[iItem]->getItems();

		pItems->SetOptionData(sbOption, sbOptionType, lOptionLevel, lOriginOptionVar);
		pItems->SetRareIndex(lRareIndex);
	}
}

// ----------------------------------------------------------------------------
// Name : ExchangeItem_Del()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeItem_Del( BOOL bSrcDel, int nUniIndex )
{
	// My items			// 아이템 삭제 막음
	/*if( ( m_bAmISrc && bSrcDel ) || ( !m_bAmISrc && !bSrcDel ) )
	{
		for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_abtnMyItems[iItem].GetItemUniIndex() == nUniIndex )
				break;
		}

		SQUAD	llMySlotCount = m_abtnMyItems[iItem].GetItemCount();

		// Rearrange buttons
		for( int iArrItem = iItem; iArrItem < EXCH_EXCH_SLOT_TOTAL; iArrItem++ )
		{
			if( ( iArrItem + 1 ) == EXCH_EXCH_SLOT_TOTAL )
			{
				m_abtnMyItems[iArrItem].InitBtn();
				break;
			}

			if( m_abtnMyItems[iArrItem + 1].IsEmpty() )
			{
				m_abtnMyItems[iArrItem].InitBtn();
				break;
			}

			m_abtnMyItems[iArrItem].SetItemInfo( 0, 0, 0, m_abtnMyItems[iArrItem + 1].GetItemIndex(),
														m_abtnMyItems[iArrItem + 1].GetItemUniIndex(), -1 );
			m_abtnMyItems[iArrItem].SetItemCount( m_abtnMyItems[iArrItem + 1].GetItemCount() );
			m_abtnMyItems[iArrItem].SetItemPlus( m_abtnMyItems[iArrItem + 1].GetItemPlus() );
			m_abtnMyItems[iArrItem].SetItemUsed( m_abtnMyItems[iArrItem + 1].GetItemUsed() );
			m_abtnMyItems[iArrItem].SetItemFlag( m_abtnMyItems[iArrItem + 1].GetItemFlag() );
		}

		// Unselect item
		if( iItem == m_nSelMyItemID )
			m_nSelMyItemID = -1;

		// Update inventory of exchange
		for( int iRow = 0; iRow < EXCH_INVEN_SLOT_ROW_TOTAL; iRow++ )
		{
			for( int iCol = 0; iCol < EXCH_INVEN_SLOT_COL; iCol++ )
			{
				if( m_abtnInvenItems[iRow][iCol].GetItemUniIndex() == nUniIndex )
				{
					SQUAD	llInvenCount = m_abtnInvenItems[iRow][iCol].GetItemCount() + llMySlotCount;
					m_abtnInvenItems[iRow][iCol].SetItemCount( llInvenCount );
					if( llInvenCount > 0 )
						m_abtnInvenItems[iRow][iCol].SetEmpty( FALSE );

					// If item is money
					int			nIndex = m_abtnInvenItems[iRow][iCol].GetItemIndex();
					CItemData	&ItemData = _pNetwork->GetItemData( nIndex );
					if( pItemData->GetType() == CItemData::ITEM_ETC &&
						pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
					{
						m_strMyExchMoney.PrintF( "%d", 0 );
					}

					return;
				}
			}
		}
	}
	// Opposite items
	else
	{
		for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_abtnOppItems[iItem].GetItemUniIndex() == nUniIndex )
				break;
		}

		// If item is money
		int			nIndex = m_abtnOppItems[iItem].GetItemIndex();
		CItemData	&ItemData = _pNetwork->GetItemData( nIndex );
		if( pItemData->GetType() == CItemData::ITEM_ETC &&
			pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
		{
			m_strOppExchMoney.PrintF( "%d", 0 );
		}

		m_abtnOppItems[iItem].InitBtn();
	}*/
}

// ----------------------------------------------------------------------------
// Name : ExchangeItem_Update()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeItem_Update( BOOL bSrcUpdate, int nUniIndex, SQUAD llCount,SQUAD llNas )
{
	// My items
	int		iItem;
	SQUAD	llCountDiff;

	if( ( m_bAmISrc && bSrcUpdate ) || ( !m_bAmISrc && !bSrcUpdate ) )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if (m_pIconsMyItems[iItem]->IsEmpty() == false && 
				m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex)
				break;
		}

		if (iItem < EXCH_EXCH_SLOT_TOTAL)
		{
			llCountDiff = m_pIconsMyItems[iItem]->getItems()->Item_Sum - llCount;
			m_pIconsMyItems[iItem]->setCount(llCount);
		}

		// Update inventory of exchange
		for (int nIdx = 0; nIdx < DEF_INVENTORY_MAX; ++nIdx)
		{
			if (m_pIconsInvenItems[nIdx]->getItems() == NULL)
				continue;

			if( m_pIconsInvenItems[nIdx]->getItems()->Item_UniIndex == nUniIndex )
			{
				SQUAD	llNewCount = m_pIconsInvenItems[nIdx]->getItems()->Item_Sum + llCountDiff;
				m_pIconsInvenItems[nIdx]->setCount(llNewCount);
				
				if( llNewCount <= 0 )
				{
					m_pIconsInvenItems[nIdx]->clearIconData();

					// Unselect item
					if( m_nSelInvenItemID == nIdx )
						m_nSelInvenItemID = -1;
				}

				// If item is money
				int			nIndex = m_pIconsInvenItems[nIdx]->getIndex();
				CItemData* pItemData = _pNetwork->GetItemData( nIndex );
				if( pItemData->GetType() == CItemData::ITEM_ETC &&
					pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
				{
					m_strMyExchMoney.PrintF( "%I64d", llCount );
					CUIManager::getSingleton()->InsertCommaToString( m_strMyExchMoney );
				}
				if(pItemData->GetType() == CItemData::ITEM_ACCESSORY &&
					pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET)
				{
					_pNetwork->SendPetitemInfo(_pNetwork->MyCharacterInfo.index, m_pIconsMyItems[iItem]->getItems()->Item_Plus);
				}

				return;
			}
		}
	}
	// Opposite items
	else
	{
		m_nTradeNas = llNas;
		m_strOppExchMoney.PrintF( "%I64d", m_nTradeNas );
		CUIManager::getSingleton()->InsertCommaToString( m_strOppExchMoney );

		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if (m_pIconsOppItems[iItem]->IsEmpty() == false && 
				m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex)
				break;
		}

		if (iItem >= EXCH_EXCH_SLOT_TOTAL)
			return;
		
		SQUAD	llCountDiff = m_pIconsOppItems[iItem]->getItems()->Item_Sum - llCount;
		m_pIconsOppItems[iItem]->setCount(llCount);

		// If item is money
		int			nIndex = m_pIconsOppItems[iItem]->getIndex();
		CItemData* pItemData = _pNetwork->GetItemData( nIndex );

		if(pItemData->GetType() == CItemData::ITEM_ACCESSORY &&
			pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET)
		{
			_pNetwork->SendPetitemInfo(_pNetwork->MyCharacterInfo.index, m_pIconsOppItems[iItem]->getItems()->Item_Plus);
		}
	}
}

void CUIExchange::ExchangeItem_SetRareOption( CNetworkMessage *istr, BOOL bSrcAdd, int nUniIndex)
	{
		SBYTE	sbOptionType;
		LONG	lOptionLevel;

		//레어 옵션 인덱스
		(*istr) >> sbOptionType;
		(*istr) >> lOptionLevel;
		UWORD iRareIndex = lOptionLevel;

		//레어 옵션 종류
		(*istr) >> sbOptionType;
		(*istr) >> lOptionLevel;
	
		WORD iRareOption = lOptionLevel;
		WORD wCBit =1;
		SBYTE sbOption =-1;
		for(int iBit=0; iBit<10; ++iBit)
		{
			if( iRareOption & wCBit)
			{
				CItemRareOption* pItem = CItemRareOption::getData(iRareIndex);

				if (pItem == NULL)
					continue;

				if (pItem->GetIndex() < 0)
					continue;

				int OptionType = pItem->rareOption[iBit].OptionIdx;
				int OptionLevel = pItem->rareOption[iBit].OptionLevel;

				this->ExchangeItem_SetOptionData(bSrcAdd, nUniIndex, ++sbOption, OptionType, OptionLevel, iRareIndex, ORIGIN_VAR_DEFAULT );
			}
			wCBit <<=1;
	}
}

void CUIExchange::ExchangeItem_SetSkill( CNetworkMessage *istr, BOOL bSrcAdd, int nUniIndex)
{
	SBYTE sbBelong, sbSkillcont;
	LONG lSkillIndex,lplus;
	SBYTE sbSkillLevel;

	// My items
	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
			{
				(*istr) >> sbBelong;
				(*istr) >> sbSkillcont;

				m_pIconsMyItems[iItem]->getItems()->Item_Belong = sbBelong;

				for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
				{
					(*istr) >> lSkillIndex;
					(*istr) >> sbSkillLevel;

					m_pIconsMyItems[iItem]->getItems()->SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
				}

				(*istr) >> lplus;
				m_pIconsMyItems[iItem]->getItems()->SetItemPlus2(lplus);
				break;
			}
				
		}
	}
	// Opposite items
	else
	{
		for( int iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
			{
				(*istr) >> sbBelong;
				(*istr) >> sbSkillcont;

				m_pIconsOppItems[iItem]->getItems()->Item_Belong = sbBelong;

				for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
				{
					(*istr) >> lSkillIndex;
					(*istr) >> sbSkillLevel;

					m_pIconsOppItems[iItem]->getItems()->SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
				}

				(*istr) >> lplus;
				m_pIconsOppItems[iItem]->getItems()->SetItemPlus2(lplus);
				break;
			}
		}
	}

}

// ----------------------------------------------------------------------------
// Name : ExchangeItem_SetSocketData()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeItem_InitSocket(BOOL bSrcAdd, int nUniIndex )
{
	// My items
	int		iItem;
	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;
		m_pIconsMyItems[iItem]->getItems()->InitSocketInfo();
	}
	// Opposite items
	else
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		m_pIconsOppItems[iItem]->getItems()->InitSocketInfo();
	}
		
}

// ----------------------------------------------------------------------------
// Name : ExchangeItem_SetSocketData()
// Desc :
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeItem_SetSocketData( BOOL bSrcAdd, int nUniIndex, SBYTE sbSocketCreateCount,
												SBYTE sbSlotIndex, LONG lJewelIndex )
{
	// My items
	int		iItem;
	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;
		m_pIconsMyItems[iItem]->getItems()->SetSocketCount(sbSocketCreateCount);
		m_pIconsMyItems[iItem]->getItems()->SetSocketOption( sbSlotIndex, lJewelIndex );
	}
	// Opposite items
	else
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		m_pIconsOppItems[iItem]->getItems()->SetSocketCount( sbSocketCreateCount );
		m_pIconsOppItems[iItem]->getItems()->SetSocketOption( sbSlotIndex, lJewelIndex );
	}
}

// ----------------------------------------------------------------------------
// Name : ExchangeItem_SetPlus2()
// Desc : // [2010/12/16 : Sora] 아이템 plus2설정
// ----------------------------------------------------------------------------
void CUIExchange::ExchangeItem_SetPlus2( BOOL bSrcAdd, int nUniIndex, LONG plus2 )
{
	// My items
	int		iItem;
	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;
		m_pIconsMyItems[iItem]->getItems()->SetItemPlus2( plus2 );
	}
	// Opposite items
	else
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		m_pIconsOppItems[iItem]->getItems()->SetItemPlus2( plus2 );
	}	
}

void CUIExchange::SetDurability( BOOL bSrcAdd, int nUniIndex, LONG nDuraNow, LONG nDuraMax )
{
	// My items
	int			iItem;
	CItems*		pItems = NULL;

	if( ( m_bAmISrc && bSrcAdd ) || ( !m_bAmISrc && !bSrcAdd ) )
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsMyItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		pItems = m_pIconsMyItems[iItem]->getItems();

		if (pItems != NULL)
			pItems->SetDurability(nDuraNow, nDuraMax);

	}
	// Opposite items
	else
	{
		for( iItem = 0; iItem < EXCH_EXCH_SLOT_TOTAL; iItem++ )
		{
			if( m_pIconsOppItems[iItem]->getItems()->Item_UniIndex == nUniIndex )
				break;
		}

		if( iItem == EXCH_EXCH_SLOT_TOTAL )
			return;

		pItems = m_pIconsOppItems[iItem]->getItems();

		if (pItems != NULL)
			pItems->SetDurability(nDuraNow, nDuraMax);
	}	
}

//2013/04/03 jeil 나스 아이템 제거 
//나스 교환 팝업 
void CUIExchange::ExchangeNas()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	if(pUIManager->DoesMessageBoxExist(MSGCMD_EXCH_ADD_NAS))
		return;
	MsgBoxInfo.SetMsgBoxInfo( _S( 149, "교환" ), UMBS_OKCANCEL | UMBS_INPUTBOX,	
		UI_EXCHANGE, MSGCMD_EXCH_ADD_NAS );
	strMessage.PrintF( _S( 5903, "몇 개의 나스를 교환 하시겠습니까?" ) );		
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	return;

}

void CUIExchange::AddItemCallback()
{
	if (UIMGR()->GetMsgBoxNumOnly()->GetData() > 0)
		SendExchangeItem_Add( nTempUniIndex, UIMGR()->GetMsgBoxNumOnly()->GetData());
}

void CUIExchange::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	int i;
	for (i = 0; i < EXCH_EXCH_SLOT_TOTAL; ++i)
	{
		if (m_pIconsOppItems[i]->IsEmpty() == false)
			m_pIconsOppItems[i]->Update(fDeltaTime, ElapsedTime);
		if (m_pIconsMyItems[i]->IsEmpty() == false)
			m_pIconsMyItems[i]->Update(fDeltaTime, ElapsedTime);
	}

	for (i = 0; i < DEF_INVENTORY_MAX; ++i)
	{
		if (m_pIconsInvenItems[i]->IsEmpty() == false)
			m_pIconsInvenItems[i]->Update(fDeltaTime, ElapsedTime);
	}
}
