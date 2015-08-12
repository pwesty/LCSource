#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Interface/UIPersonalShop.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UISocketSystem.h>
#include <Engine/Interface/UIWildPetInfo.h>

#include <Engine/Contents/Base/UIMsgBoxNumeric_only.h>
#include <Engine/Info/MyInfo.h>

#define PACKAGE_ITEM_INDEX		(506)

#define PERSONAL_SELL_TOP_SLOT_SX			11		// Sell Mode(Top)
#define PERSONAL_SELL_TOP_SLOT_SY			47
#define PERSONAL_SELL_BOTTOM_SLOT_SX		11		// (Bottom)
#define PERSONAL_SELL_BOTTOM_SLOT_SY		217		// 238
#define PERSONAL_SELL_PACKAGE_SLOT_SX		11		// (Package)
#define PERSONAL_SELL_PACKAGE_SLOT_SY		146		// 167

class CmdPersnalShopAddItem : public Command
{
public:
	CmdPersnalShopAddItem() : m_pWnd(NULL)	{}
	void setData(CUIPersonalShop* pWnd)	{ m_pWnd = pWnd;	}
	void execute() 
	{
		if (m_pWnd != NULL)
			m_pWnd->AddItemCallback();
	}
private:
	CUIPersonalShop* m_pWnd;
};

class CmdPersnalShopDelItem : public Command
{
public:
	CmdPersnalShopDelItem() : m_pWnd(NULL)	{}
	void setData(CUIPersonalShop* pWnd)	{ m_pWnd = pWnd;	}
	void execute() 
	{
		if (m_pWnd != NULL)
			m_pWnd->DelItemCallback();
	}
private:
	CUIPersonalShop* m_pWnd;
};

// ----------------------------------------------------------------------------
// Name : CUIPersonalShop()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIPersonalShop::CUIPersonalShop()
{
	m_bBuyShop			= FALSE;
	m_bShopStart		= FALSE;
	m_bPackageBuy		= FALSE;
	m_nSelShopItemID	= -1;
	m_nSelTradeItemID	= -1;
	m_nSelPackageItemID = -1;
	m_nSelectedChaID	= -1;	
	m_nCurRow			= 0;
	m_nNumOfItem		= 0;
	m_nTotalPrice		= 0;
	m_bPremium			= FALSE;
	m_strPlayerMoney	= CTString( "0" );
	m_strTotalPrice		= CTString( "0" );
	m_strPackagePrice	= CTString( "0" );
	m_llPackagePrice	= 0;
	m_bCashPersonShop	= FALSE;
	m_bCashPersonShop_open = FALSE;
	m_nSelITab			= -1;
}

// ----------------------------------------------------------------------------
// Name : ~CUIPersonalShop()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIPersonalShop::~CUIPersonalShop()
{
	int i;

	clearContainer();	

	for (i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i)
		SAFE_DELETE(m_pIconsShopItem[i]);

	for (i = 0; i < PERSONAL_TRADE_SLOT_TOTAL; ++i)
		SAFE_DELETE(m_pIconsTradeItem[i]);

	for (i = 0; i < PERSONAL_PACKAGE_SLOT_COL; ++i)
		SAFE_DELETE(m_pIconsPackageItem[i]);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	//m_nBackSplitHeight = 50;
	m_rcMainTitle.SetRect( 0, 0, 216, 22 );

	// Inventory region
	m_rcTop.SetRect( 4, 214, 190, 365 );
	m_rcMiddle.SetRect( 4, 144, 190, 182 );
	m_rcBottom.SetRect( 4, 48, 190, 115 );

	// Create shop texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Shop.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 24, fTexWidth, fTexHeight );
	m_rtBackMiddle.SetUV( 0, 25, 216, 28, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 29, 216, 62, fTexWidth, fTexHeight );	
	m_rtBackSellShop.SetUV( 0, 64, 216, 96, fTexWidth, fTexHeight );	

	// Inventory
	m_rtTopInven.SetUV( 0, 131, 216, 282, fTexWidth, fTexHeight );
	//m_rtBottomInven.SetUV( 0, 283, 216, 381, fTexWidth, fTexHeight );	
	m_rtBottomInven.SetUV( 0, 283, 216, 360, fTexWidth, fTexHeight );	
	m_rtMiddleInven.SetUV( 0, 387, 216, 451, fTexWidth, fTexHeight );	
	m_rtSeperatorInven.SetUV( 0, 382, 216, 385, fTexWidth, fTexHeight );
	m_rtTitleInven.SetUV( 0, 393, 216, 411, fTexWidth, fTexHeight );
	m_rtNameInven.SetUV( 0, 98, 216, 130, fTexWidth, fTexHeight );
	m_rtPackageBar.SetUV( 0, 30, 216, 60, fTexWidth, fTexHeight );
	m_rtBlankBar.SetUV( 0, 387, 216, 393, fTexWidth, fTexHeight );
	m_rtPriceBar.SetUV( 0, 361, 216, 380, fTexWidth, fTexHeight );

	// Outline of unmovable item
	m_rtUnmovableOutline.SetUV( 218, 86, 250, 118, fTexWidth, fTexHeight );

	// Outline of selected item
	m_rtSelectOutline.SetUV( 218, 51, 250, 83, fTexWidth, fTexHeight );

	// Lease Item UV
	m_rtleaseMark.SetUV(216,148,231,163,fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 217, 0, 231, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 232, 0, 246, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Buy button of shop
	m_btnShopBuy.Create( this, _S( 253, "구매" ), 105, 419, 51, 21 );
	m_btnShopBuy.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnShopBuy.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnShopBuy.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopBuy.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button of shop
	m_btnShopCancel.Create( this, _S( 139, "취소" ), 159, 419, 51, 21 );
	m_btnShopCancel.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnShopCancel.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnShopCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Start button of shop
	m_btnShopStart.Create( this, _S( 742, "판매개시" ), 148, 419, 62, 21 );		
	m_btnShopStart.SetUV( UBS_IDLE, 25, 476, 88, 496, fTexWidth, fTexHeight );
	m_btnShopStart.SetUV( UBS_CLICK, 89, 476, 152, 496, fTexWidth, fTexHeight );
	m_btnShopStart.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopStart.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Stop button of shop
	m_btnShopStop.Create( this, _S( 743, "판매중지" ), 148, 419, 62, 21 );		
	m_btnShopStop.SetUV( UBS_IDLE, 25, 476, 88, 496, fTexWidth, fTexHeight );
	m_btnShopStop.SetUV( UBS_CLICK, 89, 476, 152, 496, fTexWidth, fTexHeight );
	m_btnShopStop.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopStop.CopyUV( UBS_IDLE, UBS_DISABLE );	

	// Package OK of shop
	m_btnPackageOK.Create( this, _S( 191, "확인" ), 105, 186, 51, 21 );		
	m_btnPackageOK.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnPackageOK.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnPackageOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPackageOK.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnPackageOK.SetEnable( m_bPremium );

	// Package Cancel of shop
	m_btnPackageCancel.Create( this, _S( 139, "취소" ), 159, 186, 51, 21 );		
	m_btnPackageCancel.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnPackageCancel.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnPackageCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPackageCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnPackageCancel.SetEnable( m_bPremium );

	// Premium check button
	int	nStrWidth = ( strlen( _S( 744, "프리미엄" ) ) + 1 ) *				
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	m_cbtnPremium.Create( this, 193, 128, 11, 11, _S( 744, "프리미엄" ), TRUE, nStrWidth, nStrWidth );		
	m_cbtnPremium.SetUV( UCBS_NONE, 230, 119, 241, 130, fTexWidth, fTexHeight );
	m_cbtnPremium.SetUV( UCBS_CHECK, 218, 119, 229, 130, fTexWidth, fTexHeight );	
	m_cbtnPremium.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_cbtnPremium.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_cbtnPremium.SetCheck( m_bPremium );
	//m_cbtnPremium.SetEnable( m_bPremium );
	//m_cbtnPremium.SetVisible( m_bPremium );

	// Input Box for Package Price
	m_ebPackagePrice.Create( this, 26, 191, 72, 13, 12 );
	m_ebPackagePrice.SetFocus( FALSE );
	m_ebPackagePrice.SetReadingWindowUV( 218, 132, 235, 148, fTexWidth, fTexHeight );
	m_ebPackagePrice.SetCandidateUV( 218, 132, 235, 148, fTexWidth, fTexHeight );

	// Input Box for Shop Name
	m_ebShopName.Create( this, 11, 388, 195, 13, 80 );
	m_ebShopName.SetFocus( FALSE );
	m_ebShopName.SetReadingWindowUV( 218, 132, 235, 148, fTexWidth, fTexHeight );
	m_ebShopName.SetCandidateUV( 218, 132, 235, 148, fTexWidth, fTexHeight );
	
	// Bottom scroll bar
	m_sbBottomScrollBar.Create( this, 194, 218, 9, 137 );
	m_sbBottomScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10);
	m_sbBottomScrollBar.SetScrollPos( 0 );
	m_sbBottomScrollBar.SetScrollRange( PERSONAL_SHOP_SLOT_MAX / PERSONAL_SHOP_SLOT_COL );
	m_sbBottomScrollBar.SetCurItemCount( PERSONAL_SHOP_SLOT_MAX / PERSONAL_SHOP_SLOT_COL );
	m_sbBottomScrollBar.SetItemsPerPage( PERSONAL_SHOP_SLOT_ROW );
	// Up button
	m_sbBottomScrollBar.SetUpUV( UBS_IDLE, 228, 16, 237, 22, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.SetUpUV( UBS_CLICK, 228, 33, 237, 39, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbBottomScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbBottomScrollBar.SetDownUV( UBS_IDLE, 228, 25, 237, 32, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.SetDownUV( UBS_CLICK, 228, 42, 237, 49, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbBottomScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbBottomScrollBar.SetBarTopUV( 217, 16, 226, 25, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.SetBarMiddleUV( 217, 26, 226, 30, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.SetBarBottomUV( 217, 31, 226, 41, fTexWidth, fTexHeight );
	// Wheel region
	m_sbBottomScrollBar.SetWheelRect( -192, -6, 191, 148 );

	// Slot items
	// Shop Slot(5x4)
	for (int i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i)
	{
		m_pIconsShopItem[i] = new CUIIcon;
		m_pIconsShopItem[i]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PERSONALSHOP, UBET_ITEM);
	}

	// Trade Slot(5x2)
	for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
	{
		m_pIconsTradeItem[iItem] = new CUIIcon;
		m_pIconsTradeItem[iItem]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PERSONALSHOP, UBET_ITEM);
	}

	// Package Slot(5x1)
	for(int iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
	{
		m_pIconsPackageItem[iCol] = new CUIIcon;
		m_pIconsPackageItem[iCol]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PERSONALSHOP, UBET_ITEM);
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//------------------------------------------------------------------------------
// CUIPersonalShop::SetFocus
// Explain:  
// Date : 2005-03-10(오후 1:03:58) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIPersonalShop::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );
	m_ebShopName.SetFocus( bVisible );

	if( !bVisible )
	{
		m_ebPackagePrice.SetFocus( FALSE );
	}
}

// ----------------------------------------------------------------------------
// Name : OpenPersonalShop()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::OpenPersonalShop( BOOL bSellShoop )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsCSFlagOn( CSF_PERSONALSHOP ) || IsVisible() )
	{
		// FIXME : 코드가 중복되는 곳이 너무 많음.
		if(!m_bBuyShop && m_bShopStart)
		{
			//pUIManager->GetInventory()->Lock( m_bShopStart );

			//BOOL	bVisible = !IsVisible();
			//pUIManager->RearrangeOrder( UI_PERSONALSHOP, bVisible );
			pUIManager->GetChattingUI()->AddSysMessage( _S( 745, "상점 개설중에는 창을 닫을 수 없습니다." ), SYSMSG_ERROR );	
			return;
		}
		else
		{
			ResetShop();
			pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
		}
		return;
	}

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() ||
		pUIManager->GetInventory()->IsLockedArrange())
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
		return;
	}

	m_btnShopCancel.SetVisible(FALSE);
	m_btnShopCancel.SetEnable(FALSE);
	m_btnShopBuy.SetVisible(FALSE);
	m_btnShopBuy.SetEnable(FALSE);
	m_cbtnPremium.SetEnable( TRUE );
	//m_btnPackageOK.SetEnable( FALSE );
	//m_btnPackageCancel.SetEnable( FALSE );
	m_ebPackagePrice.SetEnable( m_bPremium );

	// 판매 상점 창의 경우 초기화 될 필요 없음.
	//ResetShop();
	
	m_nSelectedChaID	= -1;

	pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

	PrepareSellShop();
}

// ----------------------------------------------------------------------------
// Name : TradePersonalShop()
// Desc : 개인 상점에서 아이템 사기 & 팔기
// ----------------------------------------------------------------------------
void CUIPersonalShop::TradePersonalShop( INDEX iChaIndex, FLOAT fX, FLOAT fZ, BOOL bBuy )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// FIXME : 중복코드 제거
	if( pUIManager->IsCSFlagOn( CSF_PERSONALSHOP ) || IsVisible() )
	{
		return;
	}

	if( (m_nSelectedChaID == iChaIndex) && IsVisible() )
	{
		return;
	}

	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsMoving())
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 746, "이동중에는 개임상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSkilling())
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 747, "스킬 사용중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	if(pUIManager->IsCSFlagOn( CSF_TELEPORT ))
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 748, "순간 이동중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	pUIManager->SetCSFlagOn( CSF_PERSONALSHOP );
	//m_bBuyShop = TRUE;

	ResetShop(TRUE);

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	if(INFO()->GetTargetDBIdx(eTARGET) == 482)
	{
		m_bCashPersonShop_open = TRUE;
		SendCashPersonOpenShop( iChaIndex );
	}else
	{
		SendPersonalOpenShop( iChaIndex );
	}
	
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalShop::PrepareBuyShop()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_PERSONAL_SHOP );

	m_bBuyShop			= TRUE;
	RefreshUserItem();

	// Set money
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		m_strPlayerMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		pUIManager->InsertCommaToString( m_strPlayerMoney );
	}

	// Set money
	if( m_llPackagePrice > 0 )
	{
		m_strPackagePrice.PrintF( "%I64d", m_llPackagePrice );
		pUIManager->InsertCommaToString( m_strPackagePrice );
	}

	// Set position of trade items
	int nX = PERSONAL_SELL_TOP_SLOT_SX;
	int nY = PERSONAL_SELL_TOP_SLOT_SY;
	int	nItem;
	for( nItem = 0; nItem < PERSONAL_TRADE_SLOT_TOTAL; ++nItem )
	{
		if( nItem == PERSONAL_TRADE_SLOT_COL )
		{
			nY += 35;	nX = PERSONAL_SELL_BOTTOM_SLOT_SX;
		}
		m_pIconsTradeItem[nItem]->SetPos( nX, nY );
		nX += 35;
	}

	// Set position of trade items
	nX = PERSONAL_SELL_PACKAGE_SLOT_SX;
	nY = PERSONAL_SELL_PACKAGE_SLOT_SY;
	for( nItem = 0; nItem < PERSONAL_PACKAGE_SLOT_COL; ++nItem )
	{
		if( nItem == PERSONAL_PACKAGE_SLOT_COL )
		{
			nY += 35;	nX = PERSONAL_SELL_PACKAGE_SLOT_SX;
		}
		m_pIconsPackageItem[nItem]->SetPos( nX, nY );
		nX += 35;
	}

	pUIManager->RearrangeOrder( UI_PERSONALSHOP, TRUE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalShop::PrepareSellShop()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_PERSONAL_SHOP );

	m_bBuyShop			= FALSE;
	RefreshPlayerItem();

	// Set money
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		m_strPlayerMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		pUIManager->InsertCommaToString( m_strPlayerMoney );
	}

	// Set money
	if( m_llPackagePrice > 0 )
	{
		m_strPackagePrice.PrintF( "%I64d", m_llPackagePrice );
		pUIManager->InsertCommaToString( m_strPackagePrice );
	}

	// Set position of trade items
	int nX = PERSONAL_SELL_TOP_SLOT_SX;
	int nY = PERSONAL_SELL_TOP_SLOT_SY;
	int nItem;
	for( nItem = 0; nItem < PERSONAL_TRADE_SLOT_TOTAL; ++nItem )
	{
		if( nItem == PERSONAL_TRADE_SLOT_COL )
		{
			nY += 35;	nX = PERSONAL_SELL_BOTTOM_SLOT_SX;
		}
		m_pIconsTradeItem[nItem]->SetPos( nX, nY );
		nX += 35;
	}

	// Set position of trade items
	nX = PERSONAL_SELL_PACKAGE_SLOT_SX;
	nY = PERSONAL_SELL_PACKAGE_SLOT_SY;
	for( nItem = 0; nItem < PERSONAL_PACKAGE_SLOT_COL; ++nItem )
	{
		if( nItem == PERSONAL_PACKAGE_SLOT_COL )
		{
			nY += 35;	nX = PERSONAL_SELL_PACKAGE_SLOT_SX;
		}
		m_pIconsPackageItem[nItem]->SetPos( nX, nY );
		nX += 35;
	}

	pUIManager->RearrangeOrder( UI_PERSONALSHOP, TRUE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalShop::RefreshPlayerItem()
{
	int		i;
	int		tab, idx;

	for (i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i)
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
		
		CItems*	pItems = &_pNetwork->MySlotItem[tab][idx];

		if( pItems->Item_Sum > 0 )
		{
			CItems* pCopy = new CItems;
			memcpy(pCopy, pItems, sizeof(CItems));
			m_pIconsShopItem[i]->setData(pCopy, false);		// count 변경을 하기 때문에 복제한다.			

			if(pItems->ItemData->GetType() == CItemData::ITEM_ACCESSORY &&
				pItems->ItemData->GetSubType() == CItemData::ACCESSORY_WILDPET)
			{
				_pNetwork->SendPetitemInfo(_pNetwork->MyCharacterInfo.index, pItems->Item_Plus);
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalShop::RefreshUserItem()
{
	int iPos = 0;
	vec_Items_iter end = m_vectorSellItemList.end();
	vec_Items_iter pos;
	
	for(pos = m_vectorSellItemList.begin(); pos != end; ++pos, ++iPos)
	{		
		if( (*pos)->Item_Sum > 0 )
		{
			const int iRow = iPos / PERSONAL_TRADE_SLOT_COL;
			const int iCol = iPos % PERSONAL_TRADE_SLOT_COL;

			m_pIconsTradeItem[iPos]->setData((*pos));
		}
	}

	{
		int iPos = 0;
		vec_Items_iter end = m_vectorSellPackageList.end();
		vec_Items_iter pos;

		for(pos = m_vectorSellPackageList.begin(); pos != end; ++pos, ++iPos)
		{		
			if( (*pos)->Item_Sum > 0 )
			{
				const int iRow = iPos / PERSONAL_PACKAGE_SLOT_COL;
				const int iCol = iPos % PERSONAL_PACKAGE_SLOT_COL;

				m_pIconsPackageItem[iPos]->setData((*pos));
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalShop::ResetShop(BOOL bOpen)
{
	m_nSelShopItemID	= -1;
	m_nSelTradeItemID	= -1;
	m_nSelPackageItemID	= -1;
	m_nSelectedChaID	= -1;
	m_strTotalPrice		= CTString( "0" );
	m_strPlayerMoney	= CTString( "0" );
	m_strPackagePrice	= CTString( "0" );
	m_nTotalPrice		= 0;
	m_nNumOfItem		= 0;
	m_llPackagePrice	= 0;
	m_bBuyShop			= FALSE;
	m_bShopStart		= FALSE;
	m_bPremium			= FALSE;
	m_bPackageBuy		= FALSE;
	m_ebPackagePrice.ResetString();
	m_ebPackagePrice.SetFocus(FALSE);
	m_ebShopName.ResetString();
	m_ebShopName.SetFocus(FALSE);

	ChangeShopState( m_bShopStart );
	m_cbtnPremium.SetCheck( m_bPremium );

	m_btnShopCancel.SetVisible(FALSE);
	m_btnShopCancel.SetEnable(FALSE);
	m_btnShopBuy.SetVisible(FALSE);
	m_btnShopBuy.SetEnable(FALSE);
	//m_cbtnPremium.SetEnable( m_bPremium );
	m_btnPackageOK.SetEnable( m_bPremium );
	m_btnPackageCancel.SetEnable( m_bPremium );
	m_ebPackagePrice.SetEnable( m_bPremium );

	ClearItems();

	if( bOpen == FALSE )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		pUIManager->RearrangeOrder( UI_PERSONALSHOP, FALSE );

		// Unlock inventory
		pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_PERSONAL_SHOP );

		// Close message box of shop
		pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
		pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
		pUIManager->CloseMessageBox( MSGCMD_SHOP_PRICE );
		pUIManager->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
		pUIManager->GetMsgBoxNumOnly()->CloseBox();
	}

	m_nCurRow = 0;
	m_sbBottomScrollBar.SetScrollPos( 0 );

	m_bCashPersonShop = FALSE;
	m_bCashPersonShop_open = FALSE;
	
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIPersonalShop::ClearItems()
{
	int		i;
	for( i = 0; i < PERSONAL_TRADE_SLOT_TOTAL; ++i)
	{
		m_pIconsTradeItem[i]->clearIconData();
		m_pIconsTradeItem[i]->Hide(FALSE);
	}

	for( i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i)
	{
		m_pIconsShopItem[i]->clearIconData();
		m_pIconsShopItem[i]->Hide(FALSE);
	}

	// Package Slot(5x1)	
	for( i = 0; i < PERSONAL_PACKAGE_SLOT_COL; ++i)
	{
		m_pIconsPackageItem[i]->clearIconData();
		m_pIconsPackageItem[i]->Hide(FALSE);
	}

	clearContainer();
}

// ----------------------------------------------------------------------------
// Name : RenderShopItems()
// Desc : 구입모드...
// ----------------------------------------------------------------------------
void CUIPersonalShop::RenderShopItems()
{
	int iShopX, iShopY;	
	iShopX = PERSONAL_SELL_BOTTOM_SLOT_SX;
	iShopY = PERSONAL_SELL_BOTTOM_SLOT_SY;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	int	i;
	int	nX = iShopX;
	int	nY = iShopY;
	int	iRowS = m_nCurRow * PERSONAL_SHOP_SLOT_COL;
	int	iRowE = (m_nCurRow + PERSONAL_SHOP_SLOT_ROW) * PERSONAL_SHOP_SLOT_COL;
	int	iItem;
	
	for (i = iRowS; i < iRowE; ++i)
	{
		if ((i % PERSONAL_SHOP_SLOT_COL) == 0)
		{
			nX = iShopX;	
			if (i > iRowS)	nY += 35;
		}

		m_pIconsShopItem[i]->SetPos( nX, nY );
		nX += 35;

		if (m_pIconsShopItem[i]->IsEmpty() == true)
			continue;
			
		m_pIconsShopItem[i]->Render(pDrawPort);
	}

	// ---Trade slot items---
	for( iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
	{
		if (m_pIconsTradeItem[iItem]->IsEmpty() == true)
			continue;
			
		m_pIconsTradeItem[iItem]->Render(pDrawPort);
	}

	// Package Slot(5x1)	
	for(i = 0; i < PERSONAL_PACKAGE_SLOT_COL; ++i)
	{
		if (m_pIconsPackageItem[i]->IsEmpty() == true)
			continue;
		
		m_pIconsPackageItem[i]->Render(pDrawPort);
	}

	

	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );
	pDrawPort->EndTextEx();

	// Set shop texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Outline of items in shop slot ( unmovable )
	if( !m_bBuyShop )
	{
		for (i = iRowS; i < iRowE; ++i)
		{
			// If button is empty
			if (m_pIconsShopItem[i]->IsEmpty() == true)
				continue;

			CItems*		pItems = m_pIconsShopItem[i]->getItems();

			if (pItems == NULL)
				continue;

			m_pIconsShopItem[i]->GetAbsPos( nX, nY );

			if (IsAvailable4Sale(pItems) == true)
				continue;
			
			pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtUnmovableOutline.U0, m_rtUnmovableOutline.V0,
												m_rtUnmovableOutline.U1, m_rtUnmovableOutline.V1,
												0xFFFFFFFF );
				
		}
	}

	// Outline of selected item
	if( m_nSelTradeItemID >= 0 )
	{
		m_pIconsTradeItem[m_nSelTradeItemID]->GetAbsPos( nX, nY );
		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtSelectOutline.U0, m_rtSelectOutline.V0,
											m_rtSelectOutline.U1, m_rtSelectOutline.V1,
											0xFFFFFFFF );
	}
	if( m_nSelPackageItemID >= 0 )
	{
		m_pIconsPackageItem[m_nSelPackageItemID]->GetAbsPos( nX, nY );
		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtSelectOutline.U0, m_rtSelectOutline.V0,
											m_rtSelectOutline.U1, m_rtSelectOutline.V1,
											0xFFFFFFFF );
	}
	if( m_nSelShopItemID >= 0 )
	{	
		m_pIconsShopItem[m_nSelShopItemID]->GetAbsPos( nX, nY );
		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelectOutline.U0, m_rtSelectOutline.V0,
												m_rtSelectOutline.U1, m_rtSelectOutline.V1,
												0xFFFFFFFF );
	}
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Reading window
	if( m_ebShopName.DoesShowReadingWindow() )
	{
		// Set texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		// Reading window
		m_ebShopName.RenderReadingWindow();

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::Render()
{
	// FIXME : 각각 상태에 따른 정리가 필요함.
	//ResetShop();
	COLOR colNas;

	if(m_bPremium != m_cbtnPremium.IsChecked())
	{
		m_bPremium = m_cbtnPremium.IsChecked();
		//m_btnPackageOK.SetEnable( m_bBuyShop & m_bPremium & !m_bPackageBuy);
		//m_btnPackageCancel.SetEnable( m_bBuyShop & m_bPremium & m_bPackageBuy);
		m_ebPackagePrice.SetEnable( m_bPremium );
		m_ebPackagePrice.SetFocus( m_bPremium );
		m_ebShopName.SetFocus(FALSE);

	}

	m_btnPackageOK.SetEnable( m_bBuyShop & m_bPremium & !m_bPackageBuy);
	m_btnPackageCancel.SetEnable( m_bBuyShop & m_bPremium & m_bPackageBuy);

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Set shop texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	int	nX, nY;

	// Add render regions
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + 24,
										m_rtBackTop.U0, m_rtBackTop.V0,
										m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX, m_nPosY + 24,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight - 33,
										m_rtBackMiddle.U0, m_rtBackMiddle.V0,
										m_rtBackMiddle.U1, m_rtBackMiddle.V1,
										0xFFFFFFFF );
	if(m_bBuyShop)
	{
		pDrawPort->AddTexture( m_nPosX, m_nPosY + m_nHeight - 33,
											m_nPosX + m_nWidth, m_nPosY + m_nHeight,
											m_rtBackBottom.U0, m_rtBackBottom.V0,
											m_rtBackBottom.U1, m_rtBackBottom.V1,
											0xFFFFFFFF );
	}
	else
	{
		pDrawPort->AddTexture( m_nPosX, m_nPosY + m_nHeight - 33,
											m_nPosX + m_nWidth, m_nPosY + m_nHeight,
											m_rtBackSellShop.U0, m_rtBackSellShop.V0,
											m_rtBackSellShop.U1, m_rtBackSellShop.V1,
											0xFFFFFFFF );
	}
	
	// 판매 상점 & 구매 시...
	nX = m_nPosX;
	nY = m_nPosY + 22;
	pDrawPort->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + 18,
										m_rtTitleInven.U0, m_rtTitleInven.V0,
										m_rtTitleInven.U1, m_rtTitleInven.V1,
										0xFFFFFFFF );										

	nY += 18;
	pDrawPort->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + PERSONAL_TRADE_HEIGHT,
										m_rtBottomInven.U0, m_rtBottomInven.V0,
										m_rtBottomInven.U1, m_rtBottomInven.V1,
										0xFFFFFFFF );

	nY += PERSONAL_TRADE_HEIGHT;
	pDrawPort->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + 1,
										m_rtSeperatorInven.U0, m_rtSeperatorInven.V0,
										m_rtSeperatorInven.U1, m_rtSeperatorInven.V1,
										0xFFFFFFFF );
	
	nY += 1;
	pDrawPort->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + 64,
										m_rtMiddleInven.U0, m_rtMiddleInven.V0,
										m_rtMiddleInven.U1, m_rtMiddleInven.V1,
										0xFFFFFFFF );

	nY += 64;
	pDrawPort->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + 30,
										m_rtPackageBar.U0, m_rtPackageBar.V0,
										m_rtPackageBar.U1, m_rtPackageBar.V1,
										0xFFFFFFFF );

	nY += 30;
	pDrawPort->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + 1,
										m_rtSeperatorInven.U0, m_rtSeperatorInven.V0,
										m_rtSeperatorInven.U1, m_rtSeperatorInven.V1,
										0xFFFFFFFF );

	nY += 1;
	pDrawPort->AddTexture( nX, nY,
										nX + PERSONAL_SHOP_WIDTH, nY + PERSONAL_SHOP_HEIGHT,
										m_rtTopInven.U0, m_rtTopInven.V0,
										m_rtTopInven.U1, m_rtTopInven.V1,
										0xFFFFFFFF );

	if(!m_bBuyShop)
	{
		nY += PERSONAL_SHOP_HEIGHT;
		pDrawPort->AddTexture( nX, nY,
											nX + PERSONAL_SHOP_WIDTH, nY + 18,
											m_rtTitleInven.U0, m_rtTitleInven.V0,
											m_rtTitleInven.U1, m_rtTitleInven.V1,
											0xFFFFFFFF );

		nY += 18;
		pDrawPort->AddTexture( nX, nY,
											nX + PERSONAL_SHOP_WIDTH, nY + 32,
											m_rtNameInven.U0, m_rtNameInven.V0,
											m_rtNameInven.U1, m_rtNameInven.V1,
											0xFFFFFFFF );
	}
	else
	{
		nY += PERSONAL_SHOP_HEIGHT;
		pDrawPort->AddTexture( nX, nY,
											nX + PERSONAL_SHOP_WIDTH, nY + 19,
											m_rtPriceBar.U0, m_rtPriceBar.V0,
											m_rtPriceBar.U1, m_rtPriceBar.V1,
											0xFFFFFFFF );

		nY += 19;
		pDrawPort->AddTexture( nX, nY,
											nX + PERSONAL_SHOP_WIDTH, nY + 31,
											m_rtBlankBar.U0, m_rtBlankBar.V0,
											m_rtBlankBar.U1, m_rtBlankBar.V1,
											0xFFFFFFFF );
	}

	// Bottom scroll bar		
	m_sbBottomScrollBar.Render();

	// Close button
	m_btnClose.Render();

	// Buy & sell button of shop
	if(m_bBuyShop)
	{
		m_btnShopBuy.Render();
	
		// Cancel button of shop
		m_btnShopCancel.Render();
	}

	m_btnPackageOK.Render();
	m_btnPackageCancel.Render();

	// Start & Stop Button of Shop
	if( !m_bBuyShop )
	{
		if(m_bShopStart)
		{
			m_btnShopStop.Render();			
		}
		else
		{	
			m_btnShopStart.Render();
		}		
		m_cbtnPremium.Render();
		m_ebPackagePrice.Render();
		m_ebShopName.Render();
	}	

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	if( !m_bBuyShop )
	{
		pDrawPort->PutTextExCX( _S( 753, "상점 이름" ), m_nPosX + 55, m_nPosY + 368 );		
		pDrawPort->PutTextExCX( _S( 754, "물품 등록" ), m_nPosX + 55, m_nPosY + 26 );		
	}
	else
	{
		pDrawPort->PutTextExCX( _S( 755, "판매 물품" ), m_nPosX + 55, m_nPosY + 26 );		
	}
	pDrawPort->PutTextExCX( _S( 756, "패키지 판매" ), m_nPosX + 55, m_nPosY + 128 );		

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Render item information
	pDrawPort->PutTextEx( _S( 757, "개인상점" ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX,	
										m_nPosY + INVEN_TITLE_TEXT_OFFSETY );
	

	if( m_bBuyShop )
	{
		colNas = pUIManager->GetNasColor( m_strTotalPrice );
		pDrawPort->PutTextExRX( m_strTotalPrice, m_nPosX + PERSONAL_TRADEPRICE_POSX, m_nPosY + PERSONAL_SELL_TRADEPRICE_POSY, colNas );
	}

	if( m_bBuyShop )
	{
		colNas = pUIManager->GetNasColor( m_strPlayerMoney );
		pDrawPort->PutTextExRX( m_strPlayerMoney, m_nPosX + PERSONAL_MYMONEY_POSX, m_nPosY + PERSONAL_MYMONEY_POSY, colNas );

		if(m_bPremium)
		{
			colNas = pUIManager->GetNasColor( m_strPackagePrice );	
			pDrawPort->PutTextExRX( m_strPackagePrice, m_nPosX + PERSONAL_MYMONEY_POSX, m_nPosY + 191, colNas );
		}
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Render Items
	RenderShopItems();

	// Reading window
	if( !m_bBuyShop )
	{
		if( m_ebPackagePrice.DoesShowReadingWindow() )
		{
			// Set texture
			pDrawPort->InitTextureData( m_ptdBaseTexture );

			// Reading window
			m_ebPackagePrice.RenderReadingWindow();

			// Render all elements
			pDrawPort->FlushRenderingQueue();

			// Flush all render text queue
			pDrawPort->EndTextEx();
		}
		else if( m_ebShopName.DoesShowReadingWindow() )
		{
			// Set texture
			pDrawPort->InitTextureData( m_ptdBaseTexture );

			// Reading window
			m_ebShopName.RenderReadingWindow();

			// Render all elements
			pDrawPort->FlushRenderingQueue();

			// Flush all render text queue
			pDrawPort->EndTextEx();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPersonalShop::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL	bTitleBarClick = FALSE;

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

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}

			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInItem && 
				(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
			{
				// Shop items
				if( m_nSelShopItemID >= 0 )
				{
					if( m_bBuyShop )
					{
						// Close message box of shop
						pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
						pUIManager->CloseMessageBox( MSGCMD_SHOP_PRICE );
						pUIManager->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
						pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
						pUIManager->GetMsgBoxNumOnly()->CloseBox();

						pUIManager->SetHoldBtn(m_pIconsShopItem[m_nSelShopItemID]);
					}
					else
					{
						int idx = m_nSelShopItemID;

						if ( m_nSelITab >= INVENTORY_TAB_CASH_1 )
						{
							idx -= ITEM_COUNT_IN_INVENTORY_NORMAL;
						}

						if ( m_nSelITab >= INVENTORY_TAB_CASH_2 )
						{
							idx -= ITEM_COUNT_IN_INVENTORY_CASH_1;
						}
												
						// Not wearing, not refine stone, can trade
						CItems		*pItems = &_pNetwork->MySlotItem[m_nSelITab][idx];

						if (IsAvailable4Sale(pItems) == false)
							return WMSG_FAIL;

						pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
						pUIManager->CloseMessageBox( MSGCMD_SHOP_PRICE );
						pUIManager->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
						pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
						pUIManager->GetMsgBoxNumOnly()->CloseBox();

						pUIManager->SetHoldBtn(m_pIconsShopItem[m_nSelShopItemID]);
					}
				}
				else if( m_nSelPackageItemID >= 0 )
				{
					// Close message box of shop
					pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
					pUIManager->CloseMessageBox( MSGCMD_SHOP_PRICE );
					pUIManager->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
					pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
					pUIManager->GetMsgBoxNumOnly()->CloseBox();

					pUIManager->SetHoldBtn(m_pIconsPackageItem[m_nSelPackageItemID]);
				}
				// Trade items
				else if( m_nSelTradeItemID >= 0 )
				{
					// Close message box of shop
					pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
					pUIManager->CloseMessageBox( MSGCMD_SHOP_PRICE );
					pUIManager->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
					pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
					pUIManager->GetMsgBoxNumOnly()->CloseBox();

					pUIManager->SetHoldBtn(m_pIconsTradeItem[m_nSelTradeItemID]);
				}

				bLButtonDownInItem	= FALSE;
			}

			// Close button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Buy button of shop
			else if( m_bBuyShop && m_btnShopBuy.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button of shop
			else if( m_btnShopCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Start button of shop
			else if( m_btnShopStart.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Stop button of shop
			else if( m_btnShopStop.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnPackageOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnPackageCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_cbtnPremium.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;			
			
			// Bottom Scroll bar
			if( !m_bBuyShop && ( wmsgResult = m_sbBottomScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND)
					m_nCurRow = m_sbBottomScrollBar.GetScrollPos();

				return WMSG_SUCCESS;
			}
			else
			{
				int	i;
				int	iRowS = m_nCurRow;							// Start Row
				int	iRowE = iRowS + PERSONAL_SHOP_SLOT_ROW;			// End Row
				
				for (i = (iRowS * PERSONAL_SHOP_SLOT_COL);
					i < (iRowE * PERSONAL_SHOP_SLOT_COL); ++i)
				{
					m_pIconsShopItem[i]->MouseMessage( pMsg );
				}

				int	iItem;
				for( iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
				{
					m_pIconsTradeItem[iItem]->MouseMessage( pMsg );
				}

				for( iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem )
				{
					m_pIconsPackageItem[iItem]->MouseMessage( pMsg );
				}

				return WMSG_FAIL;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcMainTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Buy button of shop
				else if( m_bBuyShop && m_btnShopBuy.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel Button of shop
				else if( m_btnShopCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Start Button of shop
				else if( m_btnShopStart.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Stop Button of shop
				else if( m_btnShopStop.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnPackageOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					PackageToShop( TRUE );
					return WMSG_SUCCESS;
				}
				else if( m_btnPackageCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					PackageToShop( FALSE );
					return WMSG_SUCCESS;
				}
				// Premium Check Box of shop
				else if( !m_bShopStart && m_cbtnPremium.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					ClearPackageSlot();
					return WMSG_SUCCESS;
				}
				else if( m_ebShopName.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				//	m_ebShopName.SetFocus(TRUE);
					m_ebPackagePrice.SetFocus(FALSE);
					CUIManager::getSingleton()->RearrangeOrder( UI_PERSONALSHOP, TRUE );
					return WMSG_SUCCESS;
				}
				else if( m_ebPackagePrice.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				//	m_ebPackagePrice.SetFocus(TRUE);
					m_ebShopName.SetFocus(FALSE);
					CUIManager::getSingleton()->RearrangeOrder( UI_PERSONALSHOP, TRUE );
					return WMSG_SUCCESS;
				}
				// Shop items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					m_nSelShopItemID = -1;
					m_nSelTradeItemID = -1;
					m_nSelPackageItemID = -1;

					CUIManager* pUIManager = CUIManager::getSingleton();

					int	i;
					int	iRowS = m_sbBottomScrollBar.GetScrollPos();						
					int	iRowE = iRowS + PERSONAL_SHOP_SLOT_ROW;

					for (i = (iRowS * PERSONAL_SHOP_SLOT_COL); i < (iRowE * PERSONAL_SHOP_SLOT_COL); ++i)
					{
						if (m_pIconsShopItem[i]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							if (i >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
							{
								m_nSelITab = INVENTORY_TAB_CASH_2;
							}
							else if (i >= ITEM_COUNT_IN_INVENTORY_NORMAL)
							{
								m_nSelITab = INVENTORY_TAB_CASH_1;
							}
							else
							{
								m_nSelITab = INVENTORY_TAB_NORMAL;
							}
							// Update selected item
							m_nSelShopItemID = i;
							bLButtonDownInItem	= TRUE;

							pUIManager->RearrangeOrder( UI_PERSONALSHOP, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				// Package Items
				else if( m_bPremium && !m_bBuyShop && IsInsideRect( nX, nY, m_rcMiddle ) )
				{
					CUIManager* pUIManager = CUIManager::getSingleton();

					m_nSelTradeItemID = -1;
					m_nSelShopItemID = -1;
					m_nSelPackageItemID = -1;

					for( int iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem )
					{
						if( m_pIconsPackageItem[iItem]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected item
							m_nSelPackageItemID = iItem;			// 선택된 슬롯의 아이템 ID

							bLButtonDownInItem	= TRUE;

							pUIManager->RearrangeOrder( UI_PERSONALSHOP, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				// Trade items
				else if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					CUIManager* pUIManager = CUIManager::getSingleton();

					m_nSelTradeItemID = -1;
					m_nSelShopItemID = -1;
					m_nSelPackageItemID = -1;

					for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
					{
						if( m_pIconsTradeItem[iItem]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected item
							m_nSelTradeItemID = iItem;			// 선택된 슬롯의 아이템 ID

							bLButtonDownInItem	= TRUE;

							pUIManager->RearrangeOrder( UI_PERSONALSHOP, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}				
				// Bottom Scroll bar
				else if( !m_bBuyShop && ( wmsgResult = m_sbBottomScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbBottomScrollBar.GetScrollPos();
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_PERSONALSHOP, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			bLButtonDownInItem = FALSE;

			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If shop isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						// FIXME : 코드가 중복되는 곳이 너무 많음.
						if(!m_bBuyShop && m_bShopStart)
						{
							//pUIManager->GetInventory()->Lock( m_bShopStart );
							//pUIManager->RearrangeOrder( UI_PERSONALSHOP, FALSE );
							pUIManager->GetChattingUI()->AddSysMessage( _S( 745, "상점 개설중에는 창을 닫을 수 없습니다." ), SYSMSG_ERROR );	
						}
						else
						{
							ResetShop();
							pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
						}
					}
					return WMSG_SUCCESS;
				}
				// Buy button of shop
				else if( m_bBuyShop && ( wmsgResult = m_btnShopBuy.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						 // Send Network Message
						 BuyItems();
						 
						 // Clear Member Value
						 ResetShop();

						 pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
					}

					return WMSG_SUCCESS;
				}
				// Cancel button of shop
				else if( ( wmsgResult = m_btnShopCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						// FIXME : 코드가 중복되는 곳이 너무 많음.
						if(!m_bBuyShop && m_bShopStart)
						{
							//pUIManager->GetInventory()->Lock( m_bShopStart );
							//pUIManager->RearrangeOrder( UI_PERSONALSHOP, FALSE );
							pUIManager->GetChattingUI()->AddSysMessage( _S( 745, "상점 개설중에는 창을 닫을 수 없습니다." ), SYSMSG_ERROR );	
						}
						else
						{
							ResetShop();
							pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
						}
					}
					return WMSG_SUCCESS;
				}
				// Premium OK button of shop
				else if( ( wmsgResult = m_btnPackageOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				// Premium Cancel button of shop
				else if( ( wmsgResult = m_btnPackageCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				// Start button of shop
				else if( ( wmsgResult = m_btnShopStart.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						pUIManager->CloseMessageBox( MSGCMD_SHOP_START );

						//m_bShopStart = TRUE;
						CTString	strMessage;
						CUIMsgBox_Info	MsgBoxInfo;

						// 펫 탑승중일 때, 개인상점을 열 수 없다.
						if(_pNetwork->MyCharacterInfo.bWildPetRide == TRUE)
						{
							pUIManager->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);
							MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK, UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
							MsgBoxInfo.AddString( _S( 5314, "개인 상점을 개설할 수 없는 상태입니다." ) );
							pUIManager->CreateMessageBox( MsgBoxInfo );
							return WMSG_SUCCESS;
						}
						
						MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OKCANCEL,		
							UI_PERSONALSHOP, MSGCMD_SHOP_START );						

						int iRatio = 2;
						if(m_bPremium)
						{
#if defined (G_JAPAN)
							iRatio = 10;
#else
							iRatio = 2;
#endif
							strMessage.PrintF( _S( 758, "프리미엄 서비스의 사용료는 판매된 아이템 가격의 %d 퍼센트입니다.  계속 진행하시겠습니까?" ), iRatio );
						}
						else
						{
							strMessage.PrintF( _S( 759, "개인상점을 개설합니다.  계속 진행하시겠습니까?" ) );		
						}
						MsgBoxInfo.AddString( strMessage );
						pUIManager->CreateMessageBox( MsgBoxInfo );
					}
					return WMSG_SUCCESS;
				}
				// Stop button of shop
				else if( ( wmsgResult = m_btnShopStop.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{						
						SendPersonalShopStop();
					}
					return WMSG_SUCCESS;
				}			
				// Bottom Scroll bar
				else if( !m_bBuyShop && m_sbBottomScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// Shop items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					int	i;
					int	iRowS = m_nCurRow * PERSONAL_SHOP_SLOT_COL;
					int	iRowE = (m_nCurRow + PERSONAL_SHOP_SLOT_ROW) * PERSONAL_SHOP_SLOT_COL;
					for (i = iRowS; i < iRowE; ++i)
					{
						if( m_pIconsShopItem[i]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
				// Trade items
				else if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
					{
						if( m_pIconsTradeItem[iItem]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
				// Package items
				else if( m_bPremium && IsInsideRect( nX, nY, m_rcMiddle ) )
				{
					for( int iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem )
					{
						if( m_pIconsPackageItem[iItem]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
			}		
			else
			{
				//////////////////////////////////////////////////////////////////////////
				if( (m_bBuyShop || !m_bShopStart) && IsInside( nX, nY ) )
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();

					CItems* pItems = pDrag->getItems();

					if (pItems == NULL)
						return WMSG_FAIL;

					// If holding button is item and is from shop
					if (pDrag->getBtnType() == UBET_ITEM &&
						pDrag->GetWhichUI() == UI_PERSONALSHOP)
					{
						// Trade items
						if( m_nSelPackageItemID == -1 && IsInsideRect( nX, nY, m_rcBottom ) )
						{
							// If this item is moved from shop slot
							if (m_nSelTradeItemID < 0 ||								
								m_pIconsTradeItem[m_nSelTradeItemID]->getItems()->Item_UniIndex != pItems->Item_UniIndex )
							{

								int nTab = pItems->Item_Tab;
								int nIdx = pItems->InvenIndex;

								if ( nTab >= INVENTORY_TAB_CASH_1 )
								{
									nIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;
								}

								if ( nTab >= INVENTORY_TAB_CASH_2 )
								{
									nIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;
								}

								AddShopItem(nIdx, pItems->Item_UniIndex, pItems->Item_Sum, SLOT_TRADE);

								// Reset holding button
								pUIManager->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
						// Package Items
						else if( m_nSelTradeItemID == -1 && IsInsideRect( nX, nY, m_rcMiddle ) )
						{
							// If this item is moved from shop slot
							if (m_nSelPackageItemID < 0 ||
								m_pIconsPackageItem[m_nSelPackageItemID]->getItems()->Item_UniIndex != pItems->Item_UniIndex )
							{
								if(m_bPremium)
								{
									int nTab = pItems->Item_Tab;
									int nIdx = pItems->InvenIndex;

									if ( nTab >= INVENTORY_TAB_CASH_1 )
									{
										nIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;
									}

									if ( nTab >= INVENTORY_TAB_CASH_2 )
									{
										nIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;
									}

									AddShopItem(nIdx, pItems->Item_UniIndex, pItems->Item_Sum, SLOT_PACKAGE);
								}
								else
								{
									
									pUIManager->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);
									CUIMsgBox_Info	MsgBoxInfo;
									MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK,	
										UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
									CTString	strMessage = _S( 760, "패키지 판매는 프리미엄 서비스를 사용해야만 가능합니다." );		
									MsgBoxInfo.AddString( strMessage );
									pUIManager->CreateMessageBox( MsgBoxInfo );									
								}

								// Reset holding button
								pUIManager->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
						// Shop items
						else if( IsInsideRect( nX, nY, m_rcTop ) )
						{
							// If this item is moved from trade slot
							if (m_nSelShopItemID < 0 ||
								m_pIconsShopItem[m_nSelShopItemID]->getItems()->Item_UniIndex != pItems->Item_UniIndex)
							{
								DelShopItem(pItems->Item_Tab, pItems->InvenIndex,
									pItems->Item_UniIndex, pItems->Item_Sum, 
									m_nSelTradeItemID, m_nSelPackageItemID, SLOT_SHOP );

								// Reset holding button
								pUIManager->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
					} // If - If holding button is item

					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				} // If - IsInside
				//////////////////////////////////////////////////////////////////////////
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) == FALSE )
				break;

			CUIManager* pUIManager = CUIManager::getSingleton();

			// Trade items
			if( (m_bBuyShop || !m_bShopStart) && IsInsideRect( nX, nY, m_rcBottom ) )
			{
				int	nTradeItemID = m_nSelTradeItemID;
				m_nSelTradeItemID = -1;

				for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
				{
					if (m_pIconsTradeItem[iItem]->MouseMessage(pMsg) != WMSG_FAIL)
					{
						// Close message box of shop
						pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
						pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
						pUIManager->CloseMessageBox( MSGCMD_SHOP_PRICE );
						pUIManager->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
						pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
						pUIManager->GetMsgBoxNumOnly()->CloseBox();

						if (m_pIconsTradeItem[iItem]->IsEmpty() == true)
							return WMSG_FAIL;

						CItems* pItems = m_pIconsTradeItem[iItem]->getItems();

						if (pItems == NULL)
							return WMSG_FAIL;

						DelShopItem( pItems->Item_Tab, pItems->InvenIndex,
							pItems->Item_UniIndex, pItems->Item_Sum,
							nTradeItemID, -1, SLOT_TRADE );

						return WMSG_SUCCESS;
					}
				}
			}
			// Package items
			if( (!m_bBuyShop && !m_bShopStart) && m_bPremium && IsInsideRect( nX, nY, m_rcMiddle ) )
			{
				int	nPackageItemID = m_nSelPackageItemID;
				m_nSelPackageItemID = -1;

				for( int iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem )
				{
					if( m_pIconsPackageItem[iItem]->MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Close message box of shop
						pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
						pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
						pUIManager->CloseMessageBox( MSGCMD_SHOP_PRICE );
						pUIManager->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
						pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
						pUIManager->GetMsgBoxNumOnly()->CloseBox();

						if (m_pIconsPackageItem[iItem]->IsEmpty() == true)
							return WMSG_FAIL;

						CItems* pItems = m_pIconsPackageItem[iItem]->getItems();

						if (pItems == NULL)
							return WMSG_FAIL;

						DelShopItem( pItems->Item_Tab, pItems->InvenIndex,
							pItems->Item_UniIndex, pItems->Item_Sum,
							-1, nPackageItemID, SLOT_PACKAGE );

						return WMSG_SUCCESS;
					}
				}
			}
			// Shop items
			else if( (m_bBuyShop || !m_bShopStart) && IsInsideRect( nX, nY, m_rcTop ) )
			{
				m_nSelShopItemID = -1;

				int tab, idx;
				int	i;
				int	iRowS = m_nCurRow * PERSONAL_SHOP_SLOT_COL;
				int	iRowE = (m_nCurRow + PERSONAL_SHOP_SLOT_ROW) * PERSONAL_SHOP_SLOT_COL;
				for (i = iRowS; i < iRowE; ++i)
				{
					if( m_pIconsShopItem[i]->MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_nSelShopItemID = i;

						if( m_bBuyShop )
						{
							// Close message box of shop
							pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
							pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
							pUIManager->CloseMessageBox( MSGCMD_SHOP_PRICE );
							pUIManager->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
							pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
							pUIManager->GetMsgBoxNumOnly()->CloseBox();

							if (m_pIconsShopItem[i]->getIndex() == PACKAGE_ITEM_INDEX)
							{
								m_pIconsShopItem[i]->clearIconData();
								m_bPackageBuy	= FALSE;
								return WMSG_FAIL;
							}

							CItems* pItems = m_pIconsShopItem[i]->getItems();

							if (pItems == NULL)
								return WMSG_FAIL;

							AddShopItem( i,	pItems->Item_UniIndex,
								pItems->Item_Sum, SLOT_TRADE );

						}
						else
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

							// Not wearing, not refine stone, can trade
							CItems		*pItems = &_pNetwork->MySlotItem[tab][idx];

							if (IsAvailable4Sale(pItems) == false)
								continue;

							// Close message box of shop
							pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
							pUIManager->CloseMessageBox( MSGCMD_SHOP_PRICE );
							pUIManager->CloseMessageBox( MSGCMD_PERSONALSHOP_ERROR );
							pUIManager->CloseMessageBox( MSGCMD_SHOP_START );
							pUIManager->GetMsgBoxNumOnly()->CloseBox();

							pItems = m_pIconsShopItem[i]->getItems();

							if (pItems == NULL)
								return WMSG_FAIL;

							AddShopItem(i, pItems->Item_UniIndex,
								pItems->Item_Sum, SLOT_TRADE);
						}

						return WMSG_SUCCESS;
					}
				}
			}
			// Bottom Scroll bar
			else if( !m_bBuyShop && ( wmsgResult = m_sbBottomScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
			{
				if( wmsgResult == WMSG_COMMAND)
					m_nCurRow = m_sbBottomScrollBar.GetScrollPos();
			}

			return WMSG_SUCCESS;
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				
				// Bottom Scroll bar
				if( !m_bBuyShop && m_sbBottomScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_nCurRow = m_sbBottomScrollBar.GetScrollPos();
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc : 
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIPersonalShop::KeyMessage( MSG *pMsg )
{
	// If login is not focused
	if( !IsFocused() )
		return WMSG_FAIL;

	if( m_ebPackagePrice.KeyMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}
	else if( m_ebShopName.KeyMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc : 
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIPersonalShop::IMEMessage( MSG *pMsg )
{
	WMSG_RESULT wmsgResult;

	if( ( wmsgResult = m_ebShopName.IMEMessage( pMsg ) ) != WMSG_FAIL )
	{
		return wmsgResult;
	}
	else 
	{
		return m_ebPackagePrice.IMEMessage( pMsg );
	}
	// 이기환 수정 끝
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc : 
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIPersonalShop::CharMessage( MSG *pMsg )
{
	if( m_ebPackagePrice.CharMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}
	else if( m_ebShopName.CharMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS;
	}
	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

static int		nTempIdx;
static int		nTempTradeItemID;
static int		nTempPackageItemID;
static SQUAD	llTempPrice;
static int		nTempSlot;

// ----------------------------------------------------------------------------
// Name : AddShopItem()
// Desc : From shop to trade
// ----------------------------------------------------------------------------
void CUIPersonalShop::AddShopItem( int nIdx, int nUniIndex, SQUAD llCount, int nWhichSlot )
{
	int	tab, idx;
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

	nTempIdx		= nIdx;
	nTempSlot		= nWhichSlot;

	CUIManager* pUIManager = CUIManager::getSingleton();

	CItems* pItems = m_pIconsShopItem[nIdx]->getItems();

	if (pItems == NULL)
		return;

	CItemData*	pItemData = pItems->ItemData;

	m_pTempItems = pItems;
	
	BOOL bMonsterMercenaryItem = pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD; 

	if ((pItemData->GetType() == CItemData::ITEM_ETC && 
		pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY))
		return;

	if (m_bBuyShop == FALSE)
	{
	// 대여 아이템 판매 불가
		if (IsAvailable4Sale(pItems) == false)
		{
			// [2010/12/09 : Sora] 몬스터용병카드 조건 추가
			pUIManager->CloseMessageBox(MSGCMD_NULL);
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인 상점" ), UMBS_OK,UI_NONE, MSGCMD_NULL);
			CTString strMessage(_S(3052,"판매할 수 없는 아이템 입니다."));
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			return;
		}
	}

	if( !m_bBuyShop && nWhichSlot != SLOT_PACKAGE )
	{
		// Ask Price
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인 상점" ), UMBS_OKCANCEL | UMBS_INPUTBOX,		
			UI_PERSONALSHOP, MSGCMD_SHOP_PRICE );
		const char*	szItemName = _pNetwork->GetItemName(pItems->Item_Index);
	
		if (pItems->GetItemPlus() > 0 && !pUIManager->IsPet(pItemData) && !pUIManager->IsWildPet(pItemData) &&
			bMonsterMercenaryItem == FALSE)
		{
			// [2010/12/09 : Sora] 몬스터용병카드 조건 추가
			strMessage.PrintF( _S( 761, "%s +%d의 개당 가격을 입력해 주십시오." ), szItemName, pItems->Item_Plus );		
		}
		else 
		{
			strMessage.PrintF( _S( 762, "%s의 개당 가격을 입력해 주십시오." ), szItemName );		
		}
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}	
	else
	{
		AskQuantity();
	}
}

// ----------------------------------------------------------------------------
// Name : AskQuantity()
// Desc : 
// ----------------------------------------------------------------------------
void CUIPersonalShop::AskQuantity()
{
	if (m_pTempItems == NULL)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	CItemData*	pItemData = m_pTempItems->ItemData;
	const char*	szItemName = _pNetwork->GetItemName(m_pTempItems->Item_Index);

	// Ask quantity
	if ((m_bBuyShop && (pItemData->GetFlag() & ITEM_FLAG_COUNT)) ||
		(!m_bBuyShop&&  (pItemData->GetFlag() & ITEM_FLAG_COUNT) && m_pTempItems->Item_Sum > 1))
	{
		CTString	strMessage;
		strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

		CmdPersnalShopAddItem* pCmd = new CmdPersnalShopAddItem;
		pCmd->setData(this);
		UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 757, "개인상점" ), strMessage, 1, m_pTempItems->Item_Sum);
	}
	else if( m_pTempItems->Item_Plus > 0 && !(pUIManager->IsPet(pItemData)||pUIManager->IsWildPet(pItemData) || 
			pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD))
	{
		// [2010/12/09 : Sora] 몬스터용병카드 조건 추가
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_YESNO,
									UI_PERSONALSHOP, MSGCMD_SHOP_ADD_PLUSITEM );
		strMessage.PrintF(_S2(264, szItemName, "[%s +%d]<를> 옮기시겠습니까?"), szItemName, m_pTempItems->Item_Plus);
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
		
	}
	else
	{
		ShopToTrade(m_pTempItems->Item_Sum, llTempPrice, nTempSlot);
	}
}

// ----------------------------------------------------------------------------
// Name : DelShopItem()
// Desc : From trade to shop
// ----------------------------------------------------------------------------
void CUIPersonalShop::DelShopItem( int nTab, int nIdx, int nUniIndex, SQUAD llCount, int nTradeItemID, int nPackageItemID, int nWhichSlot )
{
	nTempIdx			= nIdx;

	if ( nTab >= INVENTORY_TAB_CASH_1 )
	{
		nTempIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;
	}

	if ( nTab >= INVENTORY_TAB_CASH_2 )
	{
		nTempIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;
	}
	
	nTempTradeItemID	= nTradeItemID;
	nTempPackageItemID	= nPackageItemID;
	nTempSlot			= nWhichSlot;

	if(nTempTradeItemID >= 0)
	{
		CItems* pItems = m_pIconsTradeItem[nTempTradeItemID]->getItems();

		if (pItems == NULL)
			return;

		m_pTempItems = pItems;
		CItemData*	pItemData = pItems->ItemData;
		
		// Ask quantity
		if (pItems->Item_Sum > 1)
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			CTString	strMessage;
			const char*	szItemName = _pNetwork->GetItemName(pItems->Item_Index);
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );

			CmdPersnalShopDelItem* pCmd = new CmdPersnalShopDelItem;
			pCmd->setData(this);
			UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 263, "상점" ), strMessage, 1, pItems->Item_Sum);
		}
		else
		{
			TradeToShop( pItems->Item_Sum, nWhichSlot );
		}
	}
	else if(nPackageItemID >= 0)
	{
		CItems* pItems = m_pIconsPackageItem[nTempPackageItemID]->getItems();

		if (pItems == NULL)
			return;

		m_pTempItems = pItems;
		
		CItemData*	pItemData = pItems->ItemData;
		
		// Ask quantity
		if (pItems->Item_Sum > 1)
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			CTString	strMessage;
			const char*	szItemName = _pNetwork->GetItemName(pItems->Item_Index);
			strMessage.PrintF( _S2( 150, szItemName, "몇 개의 %s<를> 옮기시겠습니까?" ), szItemName );
			
			CmdPersnalShopDelItem* pCmd = new CmdPersnalShopDelItem;
			pCmd->setData(this);
			UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 263, "상점" ), strMessage, 1, pItems->Item_Sum);
		}
		else
		{
			TradeToShop(pItems->Item_Sum, nWhichSlot);
		}
	}
	else
	{
//		TradeToShop(pItems->Item_Sum, nWhichSlot);
	}
}

// ----------------------------------------------------------------------------
// Name : BuyItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIPersonalShop::BuyItems()
{
	// Buy nothing
	if( m_nTotalPrice < 0 )
		return;

	// Not enough money
	if(_pNetwork->MyCharacterInfo.money < m_nTotalPrice )
	{
		// Add system message
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 266, "소지금이 부족합니다." ), SYSMSG_ERROR );
		return;
	}

	SendPersonalShopBuy();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nCommandCode - 
//			bOK - 
//			&strInput - 
//-----------------------------------------------------------------------------
void CUIPersonalShop::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( !bOK )
		return;

	switch( nCommandCode )
	{
	case MSGCMD_SHOP_ADD_PLUSITEM:
		{
			if (m_pTempItems != NULL)
			{
				ShopToTrade(1, llTempPrice, nTempSlot);
			}
		}
		break;

	case MSGCMD_SHOP_PRICE:		// 가격 입력.
		{
			char	*pcInput = strInput.str_String;
			int		nLength = strInput.Length();
			int iChar;
			for( iChar = 0; iChar < nLength; ++iChar )
			{
				if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
					break;
			}

			if( iChar == nLength )
			{
				SQUAD	llCount = _atoi64( pcInput );
				llTempPrice		= llCount;
				AskQuantity();
			}
		}
		break;
	case MSGCMD_PERSONALSHOP_ERROR:
		{
			// Nothing
		}
		break;
	case MSGCMD_SHOP_START:		// 판매 개시
		{			
			SendPersonalShopStart();
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIPersonalShop::ShopToTrade( SQUAD llCount, SQUAD llPrice, int iSlot )
{
	if (m_pTempItems == NULL)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// My Inventory -> Package Slot
	if(iSlot == SLOT_PACKAGE)
	{
		// Find same item in trade slot
		int		iItem;
		for (iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem)
		{
			if (m_pIconsPackageItem[iItem]->getItems() != NULL &&
				m_pIconsPackageItem[iItem]->getItems()->Item_UniIndex == m_pTempItems->Item_UniIndex)
				break;
		}
		
		// If there is same item
		if (iItem < PERSONAL_PACKAGE_SLOT_COL)
		{
			// Check if item is countable
			CItems* pItems = m_pIconsPackageItem[iItem]->getItems();

			if (pItems == NULL)
				return;

			CItemData*	pItemData = pItems->ItemData;
			
			if( pItemData->GetFlag() & ITEM_FLAG_COUNT )
			{
				if (m_bBuyShop == FALSE)
				{
					// Update count of trade item
					SQUAD	llNewCount = pItems->Item_Sum;
					llNewCount += llCount;
					m_pIconsPackageItem[iItem]->setCount(llNewCount);
					
					// Update count of shop item
					CItems* pTempItem = m_pIconsShopItem[nTempIdx]->getItems();

					if (pTempItem == NULL)
						return;

					llNewCount = pTempItem->Item_Sum;
					llNewCount -= llCount;
					m_pIconsShopItem[nTempIdx]->setCount(llNewCount);
					
					if (llNewCount <= 0)
					{
						m_pIconsShopItem[nTempIdx]->Hide(TRUE);
						
						// Unselect item
						if (m_nSelShopItemID == nTempIdx)
							m_nSelShopItemID = -1;
					}

					m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
					m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
					pUIManager->InsertCommaToString( m_strTotalPrice );
					return;
				}
			}
		}
		
		// Find empty slot
		for( iItem = 0; iItem < PERSONAL_PACKAGE_SLOT_COL; ++iItem )
		{
			if (m_pIconsPackageItem[iItem]->IsEmpty() == true)
				break;
		}
		
		// If there is not empty slot
		if( iItem == PERSONAL_PACKAGE_SLOT_COL )
		{
			// Add system message
			if( m_bBuyShop )
				pUIManager->GetChattingUI()->AddSysMessage( _S( 267, "최대 10개의 아이템을 구매할 수 있습니다." ), SYSMSG_ERROR );
			else
				pUIManager->GetChattingUI()->AddSysMessage( _S( 268, "최대 10개의 아이템을 판매할 수 있습니다." ), SYSMSG_ERROR );
			
			return;
		}
		
		if (m_bBuyShop == FALSE)
		{
			CItems* pCopy = new CItems;
			memcpy(pCopy, m_pTempItems, sizeof(CItems));
			pCopy->Item_Sum = llCount;
			pCopy->Item_Price = llPrice;
			m_pIconsPackageItem[iItem]->setData(pCopy);
			m_vectorSellPackageList.push_back(pCopy);
		}
	}
	// My Inventory -> Trade Slot
	else if (m_pTempItems->Item_Index != PACKAGE_ITEM_INDEX && iSlot == SLOT_TRADE)
	{
		// Find same item in trade slot
		int iItem;
		for( iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
		{
			if (m_pIconsTradeItem[iItem]->getItems() != NULL &&
				m_pIconsTradeItem[iItem]->getItems()->Item_UniIndex == m_pTempItems->Item_UniIndex)
				break;
		}
		
		// If there is same item
		if (iItem < PERSONAL_TRADE_SLOT_TOTAL)
		{
			// Check if item is countable
			CItemData*	pItemData = m_pTempItems->ItemData;
			
			if (pItemData->GetFlag() & ITEM_FLAG_COUNT)
			{				
				if( !m_bBuyShop )		// 개인상점 구매중 중첩 아이템의 경우 [최대 10개...]란 메시지가 뜨는 버그로 삭제함
										// 이 부분을 막을 경우에. 개인 상점 구매중에 물품 목록의 갯수가 계속 늘어나게 됨.
				{
					CItems* pItems = m_pIconsTradeItem[iItem]->getItems();

					if (pItems == NULL)
						return;

					// 같은 UniIndex 인데 다른 가격이라면 등록하지 않는다.
					if (pItems->Item_Price != llPrice)
						return;

					// Update count of trade item
					SQUAD	llNewCount = pItems->Item_Sum;
					llNewCount += llCount;
					m_pIconsTradeItem[iItem]->setCount(llNewCount);
					
					
					// Update count of shop item
					CItems* pTempItem = m_pIconsShopItem[nTempIdx]->getItems();

					if (pTempItem == NULL)
						return;

					llNewCount = pTempItem->Item_Sum;
					llNewCount -= llCount;
					m_pIconsShopItem[nTempIdx]->setCount(llNewCount);
					
					if( llNewCount <= 0 )
					{
						m_pIconsShopItem[nTempIdx]->Hide(TRUE);
						
						// Unselect item
						if( m_nSelShopItemID == nTempIdx )
							m_nSelShopItemID = -1;
					}

					m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
					m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
					pUIManager->InsertCommaToString( m_strTotalPrice );
					return;
				}
			}
		}
		
		// Find empty slot
		for( iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
		{
			if (m_pIconsTradeItem[iItem]->IsEmpty() == true)
				break;
		}
		
		// If there is not empty slot
		if( iItem == PERSONAL_TRADE_SLOT_TOTAL )
		{
			// Add system message
			if( m_bBuyShop )
				pUIManager->GetChattingUI()->AddSysMessage( _S( 267, "최대 10개의 아이템을 구매할 수 있습니다." ), SYSMSG_ERROR );
			else
				pUIManager->GetChattingUI()->AddSysMessage( _S( 268, "최대 10개의 아이템을 판매할 수 있습니다." ), SYSMSG_ERROR );
			
			return;
		}
		
		if( !m_bBuyShop )
		{
			CItems* pCopy = new CItems;
			memcpy(pCopy, m_pTempItems, sizeof(CItems));
			pCopy->Item_Sum = llCount;
			pCopy->Item_Price = llPrice;
			m_pIconsTradeItem[iItem]->setData(pCopy);
			m_vectorSellItemList.push_back(pCopy);
		}
	}
	
	//if (m_bBuyShop == false)
	{
		// Update count of shop item
		CItems* pTempItem = m_pIconsShopItem[nTempIdx]->getItems();

		if (pTempItem == NULL)
			return;
				
		SQUAD	llNewCount = pTempItem->Item_Sum;
		if(llNewCount < llCount)
			return;
		llNewCount -= llCount;
		m_pIconsShopItem[nTempIdx]->setCount(llNewCount);
		
		if( llNewCount <= 0 )
		{
			// 패키지 아이템의 경우, 패키지 상태를 해제하는 것임?
			if(m_pTempItems->Item_Index == PACKAGE_ITEM_INDEX)
			{
				m_bPackageBuy	= FALSE;				
			}

			m_pIconsShopItem[nTempIdx]->Hide(TRUE);

			if (m_bBuyShop == TRUE)
				m_pIconsShopItem[nTempIdx]->clearIconData();

			// Unselect item
			if (m_nSelShopItemID == nTempIdx)
				m_nSelShopItemID = -1;
		}
	}

	m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
	m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
	pUIManager->InsertCommaToString( m_strTotalPrice );
}

//-----------------------------------------------------------------------------
// Purpose: Trade 슬롯에서 상점 슬롯으로.
// Input  :
//-----------------------------------------------------------------------------
void CUIPersonalShop::TradeToShop( SQUAD llCount, int iSlot )
{
	SQUAD	llNewCount;
	SQUAD	llPrice = 0;

	// 선택된 아이템이 Trade 창에 있던것.
	if (nTempTradeItemID >= 0)
	{
		m_pTempItems = m_pIconsTradeItem[nTempTradeItemID]->getItems();

		if (m_pTempItems == NULL)
			return;
		
		llNewCount = m_pTempItems->Item_Sum;
		
		if( !m_bBuyShop )
		{
			llNewCount -= llCount;
			m_pIconsTradeItem[nTempTradeItemID]->setCount(llNewCount);
			
			if( llNewCount <= 0 )
			{
				// Rearrange buttons
				for( int iArrItem = nTempTradeItemID; iArrItem < PERSONAL_TRADE_SLOT_TOTAL; ++iArrItem )
				{
					if( ( iArrItem + 1 ) == PERSONAL_TRADE_SLOT_TOTAL )
					{
						m_pIconsTradeItem[iArrItem]->clearIconData();
						break;
					}
					
					if (m_pIconsTradeItem[iArrItem + 1]->IsEmpty() == true)
					{
						m_pIconsTradeItem[iArrItem]->clearIconData();
						break;
					}

					m_pIconsTradeItem[iArrItem]->setData(m_vectorSellItemList[iArrItem + 1]);
				}

				for (int i = 0; i < m_vectorSellItemList.size(); ++i)
				{
					if (m_vectorSellItemList[i] != NULL &&
						m_vectorSellItemList[i] == m_pTempItems)
					{
						SAFE_DELETE(m_vectorSellItemList[i]);
						m_vectorSellItemList.erase(m_vectorSellItemList.begin() + i);
						break;
					}
				}
				
				// Unselect item
				if( nTempTradeItemID == m_nSelTradeItemID )
					m_nSelTradeItemID = -1;
			}
		}
	}
	// 패키지에서 선택한 경우. m_bBuyShop일 경우 필요 없음)
	else if( !m_bBuyShop && nTempPackageItemID >= 0 )
	{
		// Update count of Package item
		m_pTempItems = m_pIconsPackageItem[nTempPackageItemID]->getItems();

		if (m_pTempItems == NULL)
			return;

		llNewCount = m_pTempItems->Item_Sum;
		
		//if( !m_bBuyShop )
		//{
			llNewCount -= llCount;
			m_pIconsPackageItem[nTempPackageItemID]->setCount(llNewCount);
			
			if( llNewCount <= 0 )
			{
				// Rearrange buttons
				for( int iArrItem = nTempPackageItemID; iArrItem < PERSONAL_PACKAGE_SLOT_COL; ++iArrItem )
				{
					if( ( iArrItem + 1 ) == PERSONAL_PACKAGE_SLOT_COL )
					{
						m_pIconsPackageItem[iArrItem]->clearIconData();
						break;
					}
					
					if (m_pIconsPackageItem[iArrItem + 1]->IsEmpty() == true)
					{
						m_pIconsPackageItem[iArrItem]->clearIconData();
						break;
					}
									
					m_pIconsPackageItem[iArrItem]->setData(m_vectorSellPackageList[iArrItem + 1]);
				}

				SAFE_DELETE(m_vectorSellPackageList[nTempPackageItemID]);
				m_vectorSellPackageList.erase(m_vectorSellPackageList.begin() + nTempPackageItemID);
				
				// Unselect item
				if( nTempPackageItemID == m_nSelPackageItemID )
					m_nSelPackageItemID = -1;
			}
		//}
	}

	// 판매 모드 일때...
	if( !m_bBuyShop )
	{
		// Update count of shop item
		if (m_pIconsShopItem[nTempIdx]->getItems() != NULL)
		{
			llNewCount = m_pIconsShopItem[nTempIdx]->getItems()->Item_Sum;
			llNewCount += llCount;

			m_pIconsShopItem[nTempIdx]->Hide(FALSE);

			m_pIconsShopItem[nTempIdx]->setCount(llNewCount);
		}		
	}
	// 구매 모드일때...
	else
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		int nIdx = -1;

		if (m_pTempItems == NULL)
			return;

		FindShopSlot(nIdx, m_pTempItems->Item_UniIndex);

		if( nIdx >= 0 && nIdx < PERSONAL_SHOP_SLOT_MAX )
		{
			// Check if item is countable
			CItemData*	pItemData = m_pTempItems->ItemData;
			if (pItemData != NULL && pItemData->GetFlag() & ITEM_FLAG_COUNT)
			{
				// Update count of trade item
				CItems* pItems = m_pIconsShopItem[nIdx]->getItems();

				if (pItems == NULL)
					return;

				SQUAD	llNewCount = pItems->Item_Sum;
				llNewCount += llCount;

				// 원본의 합계보다 클 수 없다.
				if (pItems->Item_Sum < llNewCount)
					return;

				if (llNewCount <= pItemData->GetStack())
				{
					m_pIconsShopItem[nIdx]->setCount(llNewCount);
				
					if( !m_bBuyShop )
					{
						// Update count of shop item
						llNewCount = pItems->Item_Sum;
						llNewCount -= llCount;
						m_pIconsShopItem[nIdx]->setCount(llNewCount);

						if( llNewCount <= 0 )
						{
							m_pIconsShopItem[nIdx]->clearIconData();
						
							// Unselect item
							if( m_nSelShopItemID == nIdx )
								m_nSelShopItemID = -1;
						}
					}
				
					m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
					m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
					pUIManager->InsertCommaToString( m_strTotalPrice );
					return;
				}
			}

			return;
		}

		int iEmptyIdx = -1;

		//FindShopSlot( iEmptyRow, iEmptyCol, -1 );
		FindEmptySlot( iEmptyIdx );

		// If there is not empty slot
		if( iEmptyIdx == PERSONAL_SHOP_SLOT_MAX )
		{
			// Add system message
			if( m_bBuyShop )
				pUIManager->GetChattingUI()->AddSysMessage( _S( 267, "최대 10개의 아이템을 구매할 수 있습니다." ), SYSMSG_ERROR );
			else
				pUIManager->GetChattingUI()->AddSysMessage( _S( 268, "최대 10개의 아이템을 판매할 수 있습니다." ), SYSMSG_ERROR );
			
			return;
		}
		
		// Update count of shop item
		llNewCount = 0;
		if (m_pIconsShopItem[iEmptyIdx]->getItems() != NULL)
			llNewCount += m_pIconsShopItem[iEmptyIdx]->getItems()->Item_Sum;
		llNewCount += llCount;
		
		CItems* pCopy = new CItems;
		memcpy(pCopy, m_pTempItems, sizeof(CItems));
		pCopy->InvenIndex = iEmptyIdx;
		pCopy->Item_Sum = llNewCount;
		m_pIconsShopItem[iEmptyIdx]->setData(pCopy, false);
		m_pIconsShopItem[iEmptyIdx]->Hide(FALSE);
		
		m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
		m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
		pUIManager->InsertCommaToString( m_strTotalPrice );
		return;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIPersonalShop::PackageToShop( BOOL bAdd )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(bAdd)
	{		
		for( int i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
		{
			if (m_pIconsShopItem[i]->IsEmpty() == true)
			{
				m_pIconsShopItem[i]->setData(UBET_ITEM, PACKAGE_ITEM_INDEX);
				m_pIconsShopItem[i]->setCount(1);
				m_pIconsShopItem[i]->Hide(FALSE);

				m_bPackageBuy	= bAdd;
				m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
				m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
				pUIManager->InsertCommaToString( m_strTotalPrice );					
				return;
			}
		}
	}
	else
	{
		for( int i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
		{
			if (m_pIconsShopItem[i]->IsEmpty() == true)
				continue;
			else
			{
				const int iIndex = m_pIconsShopItem[i]->getIndex();
				if(iIndex == PACKAGE_ITEM_INDEX)
				{
					m_pIconsShopItem[i]->clearIconData();

					m_bPackageBuy	= bAdd;
					m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
					m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
					pUIManager->InsertCommaToString( m_strTotalPrice );						
					return;
				}
			}
		}
	}

	m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
	m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
	pUIManager->InsertCommaToString( m_strTotalPrice );
}

//-----------------------------------------------------------------------------
// Purpose: 가격을 계산합니다.
// Input  : iShopID - 
//			bSell - 
// Output : __int64
//-----------------------------------------------------------------------------
__int64 CUIPersonalShop::CalculateTotalPrice(int& iCount)
{
	__int64	iTotalPrice	= 0;
	iCount				= 0;
	
	for( int i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
	{
		if( m_pIconsShopItem[i]->IsEmpty() == true)
			continue;

		CItems* pItems = m_pIconsShopItem[i]->getItems();

		if(pItems->Item_Index == -1 || pItems->Item_UniIndex == -1)
			continue;

		iTotalPrice					+= pItems->Item_Price * pItems->Item_Sum;
		iCount++;
	}
	if( m_bPackageBuy )
	{
		iTotalPrice		+= m_llPackagePrice;
	}
	
	return iTotalPrice;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iShopID - 
//			&ID - 
//			iNumOfItem - 
//			bSell - 
//-----------------------------------------------------------------------------
__int64	CUIPersonalShop::CalculateItemPrice(int iShopID, const CItemData &ID, int iNumOfItem, BOOL bSell)
{
	if(iShopID <= 0)
		return 0;
	
	const CShopData &SD = _pNetwork->GetShopData(iShopID);	
	const int iShopSellRate	= SD.GetSellRate();
	const int iShopBuyRate	= SD.GetBuyRate();
	__int64	iPrice		= 0;

	if( bSell )
		iPrice = (ID.GetPrice() * iShopBuyRate) / 100;
	else
		iPrice = (ID.GetPrice() * iShopSellRate) / 100;

	iPrice *= iNumOfItem;
	return iPrice;
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점 판매 개시
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::SendPersonalShopStart()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// FIXME : 중복코드 제거
	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsMoving())
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 746, "이동중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}
	
	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSkilling())
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 747, "스킬 사용중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}
	
	if(pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 748, "순간 이동중에는 개인상점거래를 할 수 없습니다." ), SYSMSG_ERROR );	
		return;
	}
	
	if( ((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSitting() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 763, "서 있는 상태에서만 개인상점거래를 개설 할 수 있습니다." ), SYSMSG_ERROR );	
		return;
	}
	// 와일드펫 탑승 상태일 경우 상점을 개설할 수 없음.
	if(_pNetwork->MyCharacterInfo.bWildPetRide == TRUE || pUIManager->IsCSFlagOn(CSF_PETRIDING))
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 5314, "개인 상점을 개설할 수 없는 상태입니다." ), SYSMSG_ERROR );	
		return;
	}
	// 로그의 데스모션은 상점을 개설할 수 없음
	if(_pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_FAKEDEATH))
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 5314, "개인 상점을 개설할 수 없는 상태입니다." ), SYSMSG_ERROR );	
		return;
	}

	// 서버로부터 응답 체크가 필요한 메세지

	// 판매시작					: charindex(n) shoptype(c) shopname(str) [normal_count(c) normal_item(v:normal_count) pack_price(ll) pack_count(c) pack_item(v:pack_count)]:client
	// normal_item				: row(c) col(c) item_idx(n) item_db_idx(n) count(ll) price(ll)
	// pack_item				: row(c) col(c) item_idx(n) item_db_idx(n) count(ll)
	SBYTE sbShopType = PST_NOSHOP;

	if( m_ebShopName.GetString() )
	{
		m_strShopName = m_ebShopName.GetString();

		if(m_strShopName.Length() < 4)
		{
			pUIManager->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);
			
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK, UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
			CTString	strMessage = _S( 764, "상점 이름이 너무 짧습니다.\n(한글 2자이상, 영문 4자이상)" );		
			
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			
			m_ebShopName.ResetString();
			m_ebShopName.SetFocus( TRUE );
			return;
		}
		
		int nIndexBuffer[32];
		char szBuffer[256];

		strcpy ( szBuffer,m_strShopName.str_String );

		// Date : 2005-01-15,   By Lee Ki-hwan
		if( _UIFiltering.Filtering ( szBuffer, nIndexBuffer ) == TRUE ||
			UTIL_HELP()->IsSpecialChar(szBuffer) == true)
		{
			pUIManager->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK, UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
			
			CTString	strMessage = _S ( 751, "상점 이름에 잘못된 문자가 포함되어 있습니다." );

			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
					
			m_ebShopName.SetFocus( TRUE );

			return;
		}
	}

	int iNormalItemCount	= 0;
	int iPackageItemCount	= 0;
	int iItem, iCol;
	// Trade Slot(5x2)
	for( iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
	{
		if (m_pIconsTradeItem[iItem]->IsEmpty() == true)
			continue;
		iNormalItemCount++;
	}

	// Package Slot(5x1)
	for( iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
	{
		if (m_pIconsPackageItem[iCol]->IsEmpty() == true)
			continue;
		iPackageItemCount++;
	}

	if( iPackageItemCount > 0 && m_bPremium && m_ebPackagePrice.GetString() )
	{
		CTString strPrice;
		strPrice = m_ebPackagePrice.GetString();

		if(strPrice == "" || strPrice.Length() <= 0)
		{
			pUIManager->CloseMessageBox(MSGCMD_PERSONALSHOP_ERROR);
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 757, "개인상점" ), UMBS_OK,	
				UI_PERSONALSHOP, MSGCMD_PERSONALSHOP_ERROR );
			CTString	strMessage = _S( 765, "패키지 가격을 입력해주십시오." );		
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			
			m_ebPackagePrice.ResetString();
			return;
		}
		char	*pcInput	= strPrice.str_String;
		int		nLength		= strPrice.Length();
		int		iChar;
		for( iChar = 0; iChar < nLength; ++iChar )
		{
			if( pcInput[iChar] < '0' || pcInput[iChar] > '9' )
				break;
		}
		
		if( iChar == nLength )
		{
			SQUAD	llCount		= _atoi64( pcInput );
			m_llPackagePrice	= llCount;
		}
	}

	sbShopType |= PST_SELL;

	if(m_bPremium)
	{
		sbShopType |= PST_PREMIUM;	
	}

	if(iPackageItemCount > 0)
	{
		sbShopType |= PST_PACKAGE;	
	}
	
//	UBYTE Maintype;
	
//	Maintype = m_bCashPersonShop ? MSG_EXTEND:MSG_PERSONALSHOP;

	CNetworkMessage nmPersonalShop(m_bCashPersonShop ? MSG_EXTEND:MSG_PERSONALSHOP);

		// 판매대행 상인
	if(m_bCashPersonShop)
	{
		nmPersonalShop << (LONG)MSG_EX_ALTERNATE_MERCHANT;
		nmPersonalShop << (UBYTE)MSG_ALTERNATEMERCHANT_SELL_START;
	
	}else
	{
//		Maintype = MSG_PERSONALSHOP;
		nmPersonalShop << (UBYTE)MSG_PERSONALSHOP_SELL_START;

	}	

//	if(m_bCashPersonShop) nmPersonalShop << MSG_EX_ALTERNATE_MERCHANT;
	
//	nmPersonalShop << Subtype;
	nmPersonalShop << _pNetwork->MyCharacterInfo.index;
	nmPersonalShop << (SBYTE)sbShopType;
	nmPersonalShop << m_strShopName;

	// Normal Item List
	nmPersonalShop << (SBYTE)iNormalItemCount;	

	//row(c) col(c) item_idx(n) item_db_idx(n) count(ll) price(ll)
	for( iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
	{
		if (m_pIconsTradeItem[iItem]->IsEmpty() == true)
			continue;

		nmPersonalShop << (SWORD)m_vectorSellItemList[iItem]->Item_Tab;
		nmPersonalShop << (SWORD)m_vectorSellItemList[iItem]->InvenIndex;
		nmPersonalShop << (LONG)m_vectorSellItemList[iItem]->Item_UniIndex;
		nmPersonalShop << (LONG)m_vectorSellItemList[iItem]->Item_Index;
		nmPersonalShop << m_vectorSellItemList[iItem]->Item_Sum;
		nmPersonalShop << m_vectorSellItemList[iItem]->Item_Price;
	}
	
	// Package Item List
	nmPersonalShop << m_llPackagePrice;
	nmPersonalShop << (SBYTE)iPackageItemCount;

	//row(c) col(c) item_idx(n) item_db_idx(n) count(ll)
	for( iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
	{
		if (m_pIconsPackageItem[iCol]->IsEmpty() == true)
			continue;

		nmPersonalShop << (SWORD)m_vectorSellPackageList[iCol]->Item_Tab;
		nmPersonalShop << (SWORD)m_vectorSellPackageList[iCol]->InvenIndex;
		nmPersonalShop << (LONG)m_vectorSellPackageList[iCol]->Item_UniIndex;
		nmPersonalShop << (LONG)m_vectorSellPackageList[iCol]->Item_Index;
		nmPersonalShop << m_vectorSellPackageList[iCol]->Item_Sum;
	}

	_pNetwork->SendToServerNew(nmPersonalShop);	
	_pNetwork->m_iNetworkResponse[MSG_PERSONALSHOP]		= 1;
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점에서 중단.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::SendPersonalShopStop()
{
	// FIXME : 중복코드 제거
	if(((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsActionSitting())
	{
		return;
	}

	CNetworkMessage nmPersonalShop(MSG_PERSONALSHOP);
	nmPersonalShop << (UBYTE)MSG_PERSONALSHOP_CHANGE;	
	nmPersonalShop << _pNetwork->MyCharacterInfo.index;
	nmPersonalShop << (SBYTE)PST_NOSHOP;
	_pNetwork->SendToServerNew(nmPersonalShop);	
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점에서 아이템 사기
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::SendPersonalShopBuy()
{
	SBYTE sbBuyPack = m_bPackageBuy;
	// 구매요청					: charindex(n) pack_buy(c) normal_count(c) ([normal_item_index(n) normal_item_count(ll)]:normal_count)
	CNetworkMessage nmPersonalShop(MSG_PERSONALSHOP);
	nmPersonalShop << (UBYTE)MSG_PERSONALSHOP_BUY;
	nmPersonalShop << (ULONG)m_nSelectedChaID;	
	nmPersonalShop << sbBuyPack;

	int iNormalItemCount	= 0;

	int		i;
	for( i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
	{
		if (m_pIconsShopItem[i]->IsEmpty() == true ||
			m_pIconsShopItem[i]->getIndex() == PACKAGE_ITEM_INDEX)
			continue;
		iNormalItemCount++;
	}
	nmPersonalShop << (SBYTE)iNormalItemCount;

	for( i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
	{
		if (m_pIconsShopItem[i]->IsEmpty() == true ||
			m_pIconsShopItem[i]->getIndex() == PACKAGE_ITEM_INDEX)
			continue;

		if (m_pIconsShopItem[i]->getItems() == NULL)
			continue;

		nmPersonalShop << (LONG)m_pIconsShopItem[i]->getItems()->Item_UniIndex;
		nmPersonalShop << m_pIconsShopItem[i]->getItems()->Item_Sum;
	}

	_pNetwork->SendToServerNew(nmPersonalShop);
}
//-----------------------------------------------------------------------------
// Purpose: 판매 대행 상점에서 아이템 사기
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::SendCashPersonShopBuy()
{
	SBYTE sbBuyPack = m_bPackageBuy;
	// 구매요청					: charindex(n) pack_buy(c) normal_count(c) ([normal_item_index(n) normal_item_count(ll)]:normal_count)
	CNetworkMessage nmPersonalShop(MSG_EXTEND);
	nmPersonalShop << (LONG)MSG_EX_ALTERNATE_MERCHANT;
	nmPersonalShop << (UBYTE)MSG_ALTERNATEMERCHANT_BUY;
	nmPersonalShop << (ULONG)m_nSelectedChaID;	
	nmPersonalShop << sbBuyPack;

	int iNormalItemCount	= 0;
	int	i;

	for( i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
	{
		if (m_pIconsShopItem[i]->IsEmpty() == true)
			continue;
		iNormalItemCount++;
	}
	nmPersonalShop << (SBYTE)iNormalItemCount;

	for( i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
	{
		if (m_pIconsShopItem[i]->IsEmpty() == true)
			continue;

		if (m_pIconsShopItem[i]->getItems() == NULL)
			continue;

		nmPersonalShop << (LONG)m_pIconsShopItem[i]->getItems()->Item_UniIndex;
		nmPersonalShop << m_pIconsShopItem[i]->getItems()->Item_Sum;
	}

	_pNetwork->SendToServerNew(nmPersonalShop);
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점 물품 목록 얻기.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::ReceivePersonalShopItemList(int iChaIndex, CNetworkMessage *istr) 
{
	// 판매리스트 요청			: charindex(n) [ shoptype(c) normal_count(c) normal_item(v:normal_count) pack_price(ll) pack_count(c) pack_item(v:pack_count)]:server
	// normal_item				: item_idx(n) item_db_idx(n) plus(n) flag(n) option_count(c) ([option_type(c) option_level(c)]:option_count) count(ll) price(ll)
	// pack_item				: item_idx(n) item_db_idx(n) plus(n) flag(n) option_count(c) ([option_type(c) option_level(c)]:option_count) count(ll)

	CUIManager* pUIManager = CUIManager::getSingleton();

	ClearItems();

	SBYTE	sbShopType;
	SBYTE	sbNormalCount;
	SBYTE	sbPackageCount;

	//m_nSelectedChaID = iChaIndex;

	(*istr) >> sbShopType
			>> sbNormalCount;

	if(sbShopType & PST_PREMIUM)
	{
		m_bPremium = TRUE;
		m_cbtnPremium.SetCheck(TRUE);
	}
	
	for(int i = 0; i < sbNormalCount; ++i)
	{
		LONG	lIndex;
		LONG	lUniIndex;
		ULONG	ulPlus,ulPlus2;
		ULONG	ulFlag;
		SLONG	slUsed;
		SLONG	slUsed2;
		SBYTE	sbOptionCount;
		SBYTE	sbOptionType;
		LONG	lOptionLevel;
		SQUAD	llCount;
		SQUAD	llPrice;
#ifdef	DURABILITY
		LONG	nDurabilityNow;
		LONG	nDurabilityMax;
#endif	// DURABILITY

		(*istr) >> lUniIndex;
		(*istr) >> lIndex;		
		(*istr) >> ulPlus;
		(*istr) >> ulFlag;
#ifdef	DURABILITY
		(*istr) >> nDurabilityNow;
		(*istr) >> nDurabilityMax;
#endif
		(*istr) >> sbOptionCount;

		CItems* TempItem = new CItems(lIndex);

		TempItem->InitOptionData();

		//레어 아이템이면...
		if( _pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_RARE )
		{
			//옵션 개수가 0이면 미감정 레어아이템
			if( sbOptionCount ==0)
				TempItem->SetRareIndex(0);
			//감정된 레어 아이템
			else
				pUIManager->SetRareOption(istr, *TempItem);
		}
		//레어 아이템이 아니면...
		else
		{
			LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT;

			for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
			{
				(*istr) >> sbOptionType;
				(*istr) >> lOptionLevel;

				if ( _pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_ORIGIN )
				{
					(*istr) >> lOriginOptionVar;
				}
			
				TempItem->SetOptionData(sbOption, sbOptionType, lOptionLevel, lOriginOptionVar);
			}
		}
		(*istr) >> llCount;
		(*istr) >> llPrice;
		(*istr) >> slUsed;
		(*istr) >> slUsed2;

		if (_pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_ORIGIN)
		{
			SBYTE sbBelong, sbSkillcont;
			LONG lSkillIndex;
			SBYTE sbSkillLevel;

			(*istr) >> sbBelong;
			(*istr) >> sbSkillcont;

			TempItem->SetItemBelong(sbBelong);

			for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
			{
				(*istr) >> lSkillIndex;
				(*istr) >> sbSkillLevel;

				TempItem->SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
			}
		}

		(*istr) >> ulPlus2;
		TempItem->SetItemPlus2(ulPlus2);
		TempItem->InitSocketInfo();

		if( _pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_SOCKET )
		{
			SBYTE	sbSocketCreateCount = 0;
			SBYTE	sbSocketCount = 0;
			LONG	lSocketJewelIndex = 0;
			
			LONG	lSocketInfo[JEWEL_MAX_COUNT] = {-1,};
			int	i;
			
			for (i = 0; i < JEWEL_MAX_COUNT; i++)
			{
				(*istr) >> lSocketInfo[i];
				if (lSocketInfo[i] >= 0)
					sbSocketCreateCount++;
			}
			
			TempItem->SetSocketCount( sbSocketCreateCount );	
			
			for (i = 0; i < JEWEL_MAX_COUNT; i++)
			{
				if (lSocketInfo[i] >= 0)
					TempItem->SetSocketOption( i, lSocketInfo[i] );
			}
		}

		TempItem->SetData(lIndex, lUniIndex, 0, i, ulPlus, ulFlag, -1, slUsed, slUsed2, -1, llCount);
		TempItem->SetPrice(llPrice);
#ifdef	DURABILITY
		TempItem->SetDurability(nDurabilityNow, nDurabilityMax);
#endif	// DURABILITY

		if(_pNetwork->GetItemData(lIndex)->GetType() == CItemData::ITEM_ACCESSORY &&
			_pNetwork->GetItemData(lIndex)->GetSubType() == CItemData::ACCESSORY_WILDPET)
		{
			_pNetwork->SendPetitemInfo(iChaIndex, ulPlus);
		}
		
		m_vectorSellItemList.push_back(TempItem);
	}

	if(sbShopType & PST_PACKAGE)
	{
		(*istr) >> m_llPackagePrice;
		(*istr) >> sbPackageCount;	
		for(int i = 0; i < sbPackageCount; ++i)
		{
			LONG	lIndex;
			LONG	lUniIndex;
			ULONG	ulPlus,ulPlus2 = 0;
			ULONG	ulFlag;
			SLONG	slUsed;
			SLONG	slUsed2;
			SBYTE	sbOptionCount;
			SBYTE	sbOptionType;
			LONG	lOptionLevel;
			SQUAD	llCount;
#ifdef	DURABILITY
			LONG	nDurabilityNow;
			LONG	nDurabilityMax;
#endif	// DURABILITY

			(*istr) >> lUniIndex;
			(*istr) >> lIndex;		
			(*istr) >> ulPlus;
			(*istr) >> ulFlag;
#ifdef	DURABILITY
			(*istr) >> nDurabilityNow;
			(*istr) >> nDurabilityMax;
#endif
			(*istr) >> sbOptionCount;

			CItems*	TempItem = new CItems(lIndex);
			TempItem->InitOptionData();

			if( _pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_RARE )
			{
				//옵션 개수가 0이면 미감정 레어아이템
				if( sbOptionCount ==0)
					TempItem->SetRareIndex(0);
				//감정된 레어 아이템
				else
					pUIManager->SetRareOption(istr, *TempItem);
			}
			else
			{
				LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT;

				for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
				{
					(*istr) >> sbOptionType;
					(*istr) >> lOptionLevel;

					if ( _pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_ORIGIN )
					{
						(*istr) >> lOriginOptionVar;
					}

					TempItem->SetOptionData( sbOption, sbOptionType, lOptionLevel, lOriginOptionVar );
				}
			}
			(*istr) >> llCount;
			(*istr) >> slUsed;
			(*istr) >> slUsed2;

			if (_pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_ORIGIN)
			{
				SBYTE sbBelong, sbSkillcont;
				LONG lSkillIndex;
				SBYTE sbSkillLevel;

				(*istr) >> sbBelong;
				(*istr) >> sbSkillcont;

				TempItem->SetItemBelong(sbBelong);

				for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
				{
					(*istr) >> lSkillIndex;
					(*istr) >> sbSkillLevel;

					TempItem->SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
				}
			}

			(*istr) >> ulPlus2;
			TempItem->SetItemPlus2(ulPlus2);
			TempItem->InitSocketInfo();

			if( _pNetwork->GetItemData(lIndex)->GetFlag() & ITEM_FLAG_SOCKET )
			{
				SBYTE	sbSocketCreateCount = 0;
				SBYTE	sbSocketCount = 0;
				LONG	lSocketJewelIndex = 0;
				LONG	lSocketOptionIndex = 0;
				SBYTE	sbSocketOptionLevel = 0;

				TempItem->InitSocketInfo();

				LONG	lSocketInfo[JEWEL_MAX_COUNT] = {-1,};
				int	i;

				for (i = 0; i < JEWEL_MAX_COUNT; i++)
				{
					(*istr) >> lSocketInfo[i];
					if (lSocketInfo[i] >= 0)
						sbSocketCreateCount++;
				}

				TempItem->SetSocketCount( sbSocketCreateCount );	

				for (i = 0; i < JEWEL_MAX_COUNT; i++)
				{
					if (lSocketInfo[i] >= 0)
						TempItem->SetSocketOption( i, lSocketInfo[i] );
				}
			}

			TempItem->SetData(lIndex, lUniIndex, -1, -1, ulPlus, ulFlag, -1, slUsed, slUsed2, -1, llCount);
#ifdef	DURABILITY
			TempItem->SetDurability(nDurabilityNow, nDurabilityMax);
#endif	// DURABILITY

			if(_pNetwork->GetItemData(lIndex)->GetType() == CItemData::ITEM_ACCESSORY &&
				_pNetwork->GetItemData(lIndex)->GetSubType() == CItemData::ACCESSORY_WILDPET)
			{
				_pNetwork->SendPetitemInfo(iChaIndex, ulPlus);
			}

			m_vectorSellPackageList.push_back(TempItem);
		}
	}

	m_btnShopCancel.SetVisible(TRUE);
	m_btnShopCancel.SetEnable(TRUE);
	m_btnShopBuy.SetVisible(TRUE);
	m_btnShopBuy.SetEnable(TRUE);
	m_cbtnPremium.SetVisible( FALSE );
	m_cbtnPremium.SetEnable( FALSE );
	m_ebShopName.SetEnable( FALSE );
	m_ebPackagePrice.SetEnable( FALSE );
	m_nSelectedChaID = iChaIndex;

	pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

	PrepareBuyShop();
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점 물품 목록 갱신
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::ReceivePersonalShopItemUpdate(CNetworkMessage *istr)
{
	SBYTE	sbBuyPack		= 0;
	LONG	lChaIndex		= 0;
	SBYTE	sbNormalCount	= 0;

	// 구매요청					: charindex(n) pack_buy(c) normal_count(c) ([normal_item_index(n) normal_item_count(ll)]:normal_count)
	(*istr) >> lChaIndex;	
	(*istr) >> sbBuyPack;
	(*istr) >> sbNormalCount;

	// Package 상품을 팔았을때...
	if(sbBuyPack == 1)
	{
		for(int iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
		{
			m_pIconsPackageItem[iCol]->clearIconData();
		}

		m_llPackagePrice	= 0;
		m_strPackagePrice	= CTString( "0" );

		int nCnt = m_vectorSellPackageList.size();

		for (int i = 0; i < nCnt; ++i)
		{
			SAFE_DELETE(m_vectorSellPackageList[i]);
		}

		m_vectorSellPackageList.clear();
	}

	RefreshPlayerItem();

	LONG	lItemIndex		= -1;
	SQUAD	llItemCount		= 0;
	for( int i = 0; i < sbNormalCount; ++i)
	{
		(*istr) >> lItemIndex;
		(*istr) >> llItemCount;

		for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
		{
			if (m_pIconsTradeItem[iItem]->IsEmpty() == true)
				continue;

			if (m_pIconsTradeItem[iItem]->getItems() == NULL)
				continue;

			if (m_pIconsTradeItem[iItem]->getItems()->Item_UniIndex == lItemIndex)
			{
				SQUAD llNewItemCount	= m_pIconsTradeItem[iItem]->getItems()->Item_Sum;
				llNewItemCount			-= llItemCount;

				if(llNewItemCount <= 0)
				{
					m_pIconsTradeItem[iItem]->clearIconData();

					vec_Items_iter iter = m_vectorSellItemList.begin();
					vec_Items_iter eiter = m_vectorSellItemList.end();

					for (; iter != eiter; ++iter)
					{
						if ((*iter)->Item_UniIndex == lItemIndex)
						{
							SAFE_DELETE(*iter);
							iter = m_vectorSellItemList.erase(iter);
							break;
						}
					}
				}
				else
				{
					m_pIconsTradeItem[iItem]->setCount(llNewItemCount);
				}
				break;
			}
		}
	}

	// 상점 윗부분과 아래 인벤토리에 해당하는 부분의 중복을 제거하기 위한 부분.
	// FIXME : 느릴거 같은 코드.  최적화 필요.
	for( int iItem = 0; iItem < PERSONAL_TRADE_SLOT_TOTAL; ++iItem )
	{
		if (m_pIconsTradeItem[iItem]->IsEmpty() == true)
			continue;

		if (m_pIconsTradeItem[iItem]->getItems() == NULL)
			continue;

		SQUAD llItemCount	= m_pIconsTradeItem[iItem]->getItems()->Item_Sum;
		int iItemUniIndex	= m_pIconsTradeItem[iItem]->getItems()->Item_UniIndex;
		
		// 빈 공간에 추가함.
		for( int i = 0; i < PERSONAL_SHOP_SLOT_MAX; ++i )
		{
			if (m_pIconsShopItem[i]->IsEmpty() == true)
				continue;

			if (m_pIconsShopItem[i]->getItems() == NULL)
				continue;

			if( m_pIconsShopItem[i]->getItems()->Item_UniIndex == iItemUniIndex )
			{
				SQUAD llNewItemCount	= m_pIconsShopItem[i]->getItems()->Item_Sum;
				llNewItemCount			-= llItemCount;
				m_pIconsShopItem[i]->setCount(llNewItemCount);

				if(llItemCount <= 0)
				{
					m_pIconsShopItem[i]->Hide(TRUE);
				}

				break;
			}
		}
	}

	//RefreshPlayerItem();

	m_nTotalPrice = CalculateTotalPrice( m_nNumOfItem );
	m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
	CUIManager::getSingleton()->InsertCommaToString( m_strTotalPrice );
}

//-----------------------------------------------------------------------------
// Purpose: 개인 상점 물품 목록 얻기.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::SendPersonalOpenShop(int iChaIndex)
{
	CNetworkMessage nmPersonalShop(MSG_PERSONALSHOP);
	nmPersonalShop << (UBYTE)MSG_PERSONALSHOP_SELL_LIST;
	nmPersonalShop << (ULONG)iChaIndex;
	_pNetwork->SendToServerNew(nmPersonalShop);
}

//-----------------------------------------------------------------------------
// Purpose: 판매 대행 상점 물품 목록 얻기.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::SendCashPersonOpenShop(int iChaIndex)
{
	CNetworkMessage nmPersonalShop(MSG_EXTEND);
	nmPersonalShop << (LONG)MSG_EX_ALTERNATE_MERCHANT;
	nmPersonalShop << (UBYTE)MSG_ALTERNATEMERCHANT_SELL_LIST;
	nmPersonalShop << (ULONG)iChaIndex;
	_pNetwork->SendToServerNew(nmPersonalShop);
}

//-----------------------------------------------------------------------------
// Purpose: 패키지 슬롯을 클리어함.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::ClearPackageSlot()
{
	// Package Slot(5x1)
	for(int iCol = 0; iCol < PERSONAL_PACKAGE_SLOT_COL; ++iCol)
	{
		if (m_pIconsPackageItem[iCol]->IsEmpty() == true)
			continue;

		if (m_pIconsPackageItem[iCol]->getItems() == NULL)
			continue;

		const SQUAD llOldCount	= m_pIconsPackageItem[iCol]->getItems()->Item_Sum;

		int nTab = m_pIconsPackageItem[iCol]->getItems()->Item_Tab;
		int nIdx = m_pIconsPackageItem[iCol]->getItems()->InvenIndex;

		if ( nTab >= INVENTORY_TAB_CASH_1 )
		{
			nIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;
		}

		if ( nTab >= INVENTORY_TAB_CASH_2 )
		{
			nIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;
		}

		if (nIdx < 0 || nIdx >= PERSONAL_SHOP_SLOT_MAX)
		{
			m_pIconsPackageItem[iCol]->clearIconData();
			continue;
		}

		SQUAD llNew				= m_pIconsShopItem[nIdx]->getItems()->Item_Sum + llOldCount;

		m_pIconsShopItem[nIdx]->Hide(FALSE);
		m_pIconsShopItem[nIdx]->setCount(llNew);

		m_pIconsPackageItem[iCol]->clearIconData();
	}

	for (int i = 0; i < m_vectorSellPackageList.size(); ++i)
		SAFE_DELETE(m_vectorSellPackageList[i]);

	m_vectorSellPackageList.clear();
	m_ebPackagePrice.ResetString();
	m_llPackagePrice = 0;
}

//-----------------------------------------------------------------------------
// Purpose: 상점의 상태를 변경합니다.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::ChangeShopState(BOOL bShopStart)
{
	m_bShopStart = bShopStart;
	m_btnShopStart.SetVisible(!m_bShopStart);
	m_btnShopStart.SetEnable(!m_bShopStart);
	m_btnShopStop.SetVisible(m_bShopStart);						
	m_btnShopStop.SetEnable(m_bShopStart);
	//m_btnPackageOK.SetEnable(m_bPremium & !m_bShopStart);
	//m_btnPackageCancel.SetEnable(m_bPremium & !m_bShopStart);
	//m_cbtnPremium.SetEnable(!m_bShopStart);
	m_ebPackagePrice.SetEnable(!m_bShopStart);
	m_ebShopName.SetEnable(!m_bShopStart);
}

//-----------------------------------------------------------------------------
// Purpose: 인덱스에 해당하는 상점 슬롯을 찾음.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::FindShopSlot( int& nIdx, int iVirtualIdx )
{
	// 빈 공간에 추가함.
	for( nIdx = 0; nIdx < PERSONAL_SHOP_SLOT_MAX; ++nIdx )
	{
		if (m_pIconsShopItem[nIdx]->IsEmpty() == false && m_pIconsShopItem[nIdx]->getItems() &&
			m_pIconsShopItem[nIdx]->getItems()->Item_UniIndex == iVirtualIdx)
			return;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 비어있는 상점 슬롯을 찾음.
// Input  : 
//-----------------------------------------------------------------------------
void CUIPersonalShop::FindEmptySlot( int& nIdx )
{
	// 빈 공간에 추가함.
	for( nIdx = 0; nIdx < PERSONAL_SHOP_SLOT_MAX; ++nIdx )
	{
		if (m_pIconsShopItem[nIdx]->IsEmpty() == true)
			return;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 구매중에 판매자가 개인상점 닫았다. 구매창을 닫는다.
// Input  : 
//-----------------------------------------------------------------------------
BOOL CUIPersonalShop::EndBuyShop(int nChaID)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->IsCSFlagOn( CSF_PERSONALSHOP ) || IsVisible())
	{ //구매자 상태이고
		if (m_nSelectedChaID == nChaID)
		{ // 구매하려는 판매 상점이 닫히면 구매창을 닫자
			ResetShop();
			pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
			return TRUE;
		}
	}

	return FALSE;
}

void CUIPersonalShop::AddItemCallback()
{
	SQUAD	llCount = UIMGR()->GetMsgBoxNumOnly()->GetData();

	if( m_bBuyShop )
	{
		if( llCount > 0 )
		{
			ShopToTrade(llCount, llTempPrice, nTempSlot);
		}
	}
	else
	{
		if (llCount > 0 && llCount <= m_pTempItems->Item_Sum)
		{
			ShopToTrade(llCount, llTempPrice, nTempSlot);
		}
	}
}

void CUIPersonalShop::DelItemCallback()
{
	SQUAD	llCount = UIMGR()->GetMsgBoxNumOnly()->GetData();

	if (llCount > 0 && llCount <= m_pTempItems->Item_Sum)
		TradeToShop( llCount, nTempSlot );
}

void CUIPersonalShop::clearContainer()
{
	int		i;
	
	for (i = 0; i < m_vectorSellItemList.size(); ++i)
		SAFE_DELETE(m_vectorSellItemList[i]);

	m_vectorSellItemList.clear();

	for (i = 0; i < m_vectorSellPackageList.size(); ++i)
		SAFE_DELETE(m_vectorSellPackageList[i]);

	m_vectorSellPackageList.clear();
}

bool CUIPersonalShop::IsAvailable4Sale( CItems* pItem )
{
	if (pItem == NULL)
		return false;

	CItemData* pItemData = pItem->ItemData;

	if (pItemData == NULL)
		return false;

	bool bSubJob = false;

#ifdef ADD_SUBJOB
	bSubJob = true;
#endif

	if (pItem->IsFlag(FLAG_ITEM_BELONG))
		return false;

	if (pItemData->GetType() == CItemData::ITEM_ACCESSORY &&
		pItemData->GetSubType() == CItemData::ACCESSORY_RELIC)
		return false;

	if (bSubJob == true && pItemData->IsFlag( ITEM_FLAG_SELLER ))
	{
		if (!UIMGR()->CheckSellerItem(UI_PERSONALSHOP, pItemData->GetFlag()))
			return false;
	}
	else
	{
		bool bMonsterMercenaryItem = false; 

		if (pItemData->GetType() == CItemData::ITEM_ETC &&
			pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD)
		{
			bMonsterMercenaryItem = true;
		}

		if (!pItemData->IsFlag(ITEM_FLAG_EXCHANGE)||
			pItem->IsFlag(FLAG_ITEM_LENT) ||
			pItem->IsFlag(FLAG_ITEM_PLATINUMBOOSTER_ADDED) ||
			pItem->Item_Wearing > 0 || 
			( bMonsterMercenaryItem && pItem->Item_Used > 0 ))
			return false;
	}

	return true;
}
