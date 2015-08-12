// ----------------------------------------------------------------------------
//  File : UIPersonalShop.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIPERSONALSHOP_H_
#define	UIPERSONALSHOP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIEditBox.h>

class CItems;
class CItemData;
class CUIEditBox;

// SHOP CONF
#define PS_MAX_SHOPNAME		100				// 상점 이름 최대 길이 100 바이트
#define PS_MAX_NORMAL_ITEM	10				// 일반 아이템 거래 최대 수
#define PS_MAX_PACKAGE_ITEM	5				// 패키지 판매 거래 최대 수

// PERSONAL SHOP TYPE
#define PST_NOSHOP			0
#define PST_SELL			(1 << 0)		// 판매
#define PST_BUY				(1 << 1)		// 구매
#define PST_PREMIUM			(1 << 2)		// 프리미엄
#define PST_PACKAGE			(1 << 3)		// 패키지

// Define item slot
#define PERSONAL_SHOP_WIDTH					216
#define PERSONAL_SHOP_HEIGHT				151
#define PERSONAL_TRADE_HEIGHT				77		// 98

// Column & Row
#define PERSONAL_SHOP_SLOT_COL				5
#define PERSONAL_SHOP_SLOT_ROW				4
#define PERSONAL_SHOP_SLOT_ROW_TOTAL		20
#define	PERSONAL_SHOP_SLOT_MAX				(ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1 + ITEM_COUNT_IN_INVENTORY_CASH_2)
#define PERSONAL_TRADE_SLOT_COL				5
#define	PERSONAL_TRADE_SLOT_TOTAL			10
#define PERSONAL_PACKAGE_SLOT_COL			5


// Define text position
#define	PERSONAL_TITLE_TEXT_OFFSETX			25
#define	PERSONAL_TITLE_TEXT_OFFSETY			5
#define	PERSONAL_MYMONEY_POSX				98
#define	PERSONAL_MYMONEY_POSY				425
#define	PERSONAL_TRADEPRICE_POSX			180
#define	PERSONAL_SELL_TRADEPRICE_POSY		366


// Define size of private shop
#define PERSONAL_MAIN_WIDTH					216
#define	PERSONAL_MAIN_HEIGHT				447


// ----------------------------------------------------------------------------
// Name : CUIPersonalShop
// Desc :
// ----------------------------------------------------------------------------
class CUIPersonalShop : public CUIWindow
{
private:
	typedef		std::vector< CItems* >		vec_Items;
	typedef		vec_Items::iterator			vec_Items_iter;

protected:
	enum eSlotType
	{
		SLOT_PACKAGE	= 0,
		SLOT_TRADE		= 1,
		SLOT_SHOP		= 2,
	};

	BOOL				m_bBuyShop;				// 판매 상점인가? 구매 창인가?
	BOOL				m_bShopStart;			// 판매 개시했는가?
	BOOL				m_bPremium;				// 프리미엄 서비스 사용.
	SQUAD				m_llPackagePrice;
	BOOL				m_bPackageBuy;

	//int					m_nBackSplitHeight;
	int					m_nCurRow;

	int					m_nSelITab;
	int					m_nSelShopItemID;
	int					m_nSelTradeItemID;
	int					m_nSelPackageItemID;
	int					m_nSelectedChaID;	// Character ID

	CTString			m_strPlayerMoney;
	CTString			m_strTotalPrice;
	CTString			m_strPackagePrice;

	__int64				m_nTotalPrice;	
	int					m_nNumOfItem;

	BOOL				m_bRareItem;
	int					m_iRareGrade;

	// Region
	UIRect				m_rcMainTitle;
	UIRect				m_rcTop;				// Shop
	UIRect				m_rcMiddle;				// Package
	UIRect				m_rcBottom;				// Trade

	// Background
	UIRectUV			m_rtBackTop;
	UIRectUV			m_rtBackMiddle;
	UIRectUV			m_rtBackBottom;
	//UIRectUV			m_rtBackTitle;
	UIRectUV			m_rtBackSellShop;		// 판매 상점
	//UIRectUV			m_rtBackBuyShop;		// 구매 상점
	UIRectUV			m_rtleaseMark;			// 대여 표시

	// Position of target npc
	FLOAT				m_fNpcX, m_fNpcZ;

	// Inventory
	UIRectUV			m_rtTopInven;
	UIRectUV			m_rtMiddleInven;
	UIRectUV			m_rtBottomInven;
	UIRectUV			m_rtSeperatorInven;
	UIRectUV			m_rtTitleInven;
	UIRectUV			m_rtNameInven;
	UIRectUV			m_rtPackageBar;
	UIRectUV			m_rtBlankBar;
	UIRectUV			m_rtPriceBar;

	UIRectUV			m_rtUnmovableOutline;					// UV of outline of unmovable items
	UIRectUV			m_rtSelectOutline;						// UV of outline of selected items
	// Buttons
	CUIButton			m_btnClose;								// Close button of Shop
	CUIButton			m_btnShopBuy;							// Buy button of Shop
	CUIButton			m_btnShopSell;							// Sell button of Shop
	CUIButton			m_btnShopCancel;						// Cancel button of Shop	
	CUIButton			m_btnShopStart;							// Shop Start
	CUIButton			m_btnShopStop;							// Shop Stop
	CUIButton			m_btnPackageOK;							// Package OK
	CUIButton			m_btnPackageCancel;						// Package Cancel
	//CUIScrollBar		m_sbTopScrollBar;						// Scroll bar of shop
	CUIScrollBar		m_sbBottomScrollBar;					// Scroll bar of inventory

	CUICheckButton		m_cbtnPremium;							// Premium check button
	CUIEditBox			m_ebPackagePrice;						// Input Box for Package Price
	CUIEditBox			m_ebShopName;							// Input Box for Shop Name
	CTString			m_strShopName;

	vec_Items			m_vectorSellItemList;					// 판매 물품에 등록.(네트워크에서 받아와서...)
	vec_Items			m_vectorSellPackageList;				// 판매 패키지 물품에 등록.(네트워크에서 받아와서...)
	
	BOOL				m_bCashPersonShop;						// 판매대행 상인
	BOOL				m_bCashPersonShop_open;					// 판매 대행 상인 거래중

private:	
	// Items	
	CUIIcon*			m_pIconsTradeItem[PERSONAL_TRADE_SLOT_TOTAL];			// Player Slot items
	CUIIcon*			m_pIconsShopItem[PERSONAL_SHOP_SLOT_MAX];				// Shop Slot items
	CUIIcon*			m_pIconsPackageItem[PERSONAL_PACKAGE_SLOT_COL];			// Package Slot items

	SQUAD				m_aiTradeItemCount[PERSONAL_TRADE_SLOT_TOTAL];			// Player Slot items

	CItems*				m_pTempItems;

public:
	CUIPersonalShop();
	~CUIPersonalShop();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	ENGINE_API void	OpenPersonalShop( BOOL bSellShop );			// 개인 상점 개설 ( TRUE -> 판매, FALSE -> 구매 )
	ENGINE_API void	TradePersonalShop( INDEX iChaIndex, FLOAT fX, FLOAT fZ, BOOL bBuy );				// 개인 상점에서 아이템 사기&팔기

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// Clear
	void	ClearItems();
	void	RefreshPlayerItem();
	void	RefreshUserItem();
	ENGINE_API void	ResetShop(BOOL bOpen=FALSE);

	BOOL	IsBuyShop() const { return m_bBuyShop; }
	void	ChangeShopState(BOOL bShopStart);
	BOOL	EndBuyShop(int nChaID);
	//void	ToggleVisible();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	CharMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	// Set focus
	void	SetFocus( BOOL bVisible );

	// Edit box focus
	BOOL	IsEditBoxFocused() { return m_ebPackagePrice.IsFocused() || m_ebShopName.IsFocused(); }
	void	KillFocusEditBox()
	{
		m_ebPackagePrice.SetFocus( FALSE );
		m_ebShopName.SetFocus( FALSE );
	}

	// Network Message 
	void	SendPersonalShopStart();
	void	SendPersonalShopStop();
	void	SendPersonalShopBuy();
	void	SendPersonalOpenShop(int iChaIndex);
	void	ReceivePersonalShopItemList(int iChaIndex, CNetworkMessage *istr);
	void	ReceivePersonalShopItemUpdate(CNetworkMessage *istr);

	// 판매대행 상인
	BOOL	GetCashPersonShop() {return m_bCashPersonShop;}
	void	SetCashPersonShop(BOOL bPersonShop) { m_bCashPersonShop = bPersonShop;}
	void	SendCashPersonOpenShop(int iChaIndex);
	void	SendCashPersonShopBuy();

	void	AddItemCallback();
	void	DelItemCallback();
protected:
	// Internal functions
	void	RenderShopItems();

	void	TradeToShop( SQUAD llCount, int iSlot );				// Trade -> Shop
	void	ShopToTrade( SQUAD llCount, SQUAD llPrice, int iSlot);	// Shop -> Trade
	void	PackageToShop( BOOL bAdd );

	void	FindShopSlot( int& nIdx, int iIndex, ULONG ulFlag);
	void	FindShopSlot( int& nIdx, int iVirtualIdx );
	void	FindEmptySlot( int& nIdx );

	__int64	CalculateTotalPrice(int& iCount);
	__int64	CalculateItemPrice(int iShopID, const CItemData &ID, int iNumOfItem, BOOL bSell);		
	void	PrepareBuyShop();
	void	PrepareSellShop();	

	// Command functions
	void	AddShopItem( int nIdx, int nUniIndex, SQUAD llCount, int nWhichSlot );
	void	AskQuantity();
	void	DelShopItem( int nTab, int nIdx, int nUniIndex, SQUAD llCount, int nTradeItemID, int nPackageItemID, int nWhichSlot );
	void	BuyItems();	
	
	void	ClearPackageSlot();

private:
	void	clearContainer();
	bool	IsAvailable4Sale(CItems* pItem);
};

#endif // UIPERSONALSHOP_H_

