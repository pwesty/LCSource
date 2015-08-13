// ----------------------------------------------------------------------------
//  File : UIShop.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UISHOP_H_
#define	UISHOP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define item slot
#define SHOP_SHOP_WIDTH					216
#define SHOP_SHOP_HEIGHT				151
#define SHOP_TRADE_HEIGHT				98
#define SHOP_LEASE_HEIGHT				77

#define SHOP_BUY_TOP_SLOT_SX			11		// Buy Mode(Top)
#define SHOP_BUY_TOP_SLOT_SY			28
#define SHOP_BUY_BOTTOM_SLOT_SX			11		// (Bottom)
#define SHOP_BUY_BOTTOM_SLOT_SY			180

#define SHOP_SELL_TOP_SLOT_SX			11		// Sell Mode(Top)
#define SHOP_SELL_TOP_SLOT_SY			28
#define SHOP_SELL_BOTTOM_SLOT_SX		11		// (Bottom)
#define SHOP_SELL_BOTTOM_SLOT_SY		127


// Column & Row
#define SHOP_SLOT_COL					5
#define SHOP_SLOT_ROW					4
#define SHOP_SLOT_ROW_BUY_TOTAL			50
#define SHOP_SLOT_ROW_SELL_TOTAL		20
#define SHOP_SLOT_BUY_MAX				(SHOP_SLOT_ROW_BUY_TOTAL * SHOP_SLOT_COL)
#define SHOP_SLOT_SELL_MAX				(ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1 + ITEM_COUNT_IN_INVENTORY_CASH_2)
#define SHOP_TRADE_SLOT_COL				5
#define	SHOP_TRADE_SLOT_TOTAL			10


// Define text position
#define	SHOP_TITLE_TEXT_OFFSETX			25
#define	SHOP_TITLE_TEXT_OFFSETY			5
#define	SHOP_MYMONEY_POSX				95
#define	SHOP_MYMONEY_POSY				283
#define	SHOP_TRADEPRICE_POSX			177
#define	SHOP_BUY_TRADEPRICE_POSY		253
#define	SHOP_SELL_TRADEPRICE_POSY		101


// Define size of shop
#define SHOP_MAIN_WIDTH					216
#define	SHOP_MAIN_HEIGHT				305

class CItemData;


// ----------------------------------------------------------------------------
// Name : CUIShop
// Desc :
// ----------------------------------------------------------------------------
class CUIShop : public CUIWindow
{
protected:
	BOOL				m_bBuyShop;
	int					m_nCurRow;

	int					m_nSelShopItemID;
	int					m_nSelTradeItemID;
	int					m_nSelectedShopID;		// Shop ID

	CTString			m_strPlayerMoney;
	CTString			m_strTotalPrice;

	__int64				m_nTotalPrice;
	__int64				m_nSendTotalPrice;
	int					m_nNumOfItem;
	// Region
	UIRect				m_rcMainTitle;
	UIRect				m_rcBuyShopSlot;		// Buy Mode
	UIRect				m_rcBuyTradeSlot;
	UIRect				m_rcSellShopSlot;		// Sell Mode
	UIRect				m_rcSellTradeSlot;
	UIRect				m_rcTop;				// Shop
	UIRect				m_rcBottom;				// Trade

	// Background
	UIRectUV			m_rtBackTop;
	UIRectUV			m_rtBackMiddle;
	UIRectUV			m_rtBackBottom;

	// Position of target npc
	FLOAT				m_fNpcX, m_fNpcZ;
	int					m_nNpcVirIdx;

	// Inventory
	UIRectUV			m_rtTopInven;
	UIRectUV			m_rtBottomInven;
	UIRectUV			m_rtSeperatorInven;

	// for lease 
	UIRect				m_rcBottom1;
	UIRect				m_rcBottom2;
	UIRectUV			m_rtBottom1;
	UIRectUV			m_rtBottom2;
	UIRectUV			m_rtBottom3;
	UIRectUV			m_rtleaseMark;

	// Item Info
	UIRectUV			m_rtInfoUL;								// UV of upper left region of information
	UIRectUV			m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV			m_rtInfoUR;								// UV of upper right region of information
	UIRectUV			m_rtInfoML;								// UV of middle left region of information
	UIRectUV			m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV			m_rtInfoMR;								// UV of middle right region of information
	UIRectUV			m_rtInfoLL;								// UV of lower left region of information
	UIRectUV			m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV			m_rtInfoLR;								// UV of lower right region of information
	UIRectUV			m_rtUnmovableOutline;					// UV of outline of unmovable items
	UIRectUV			m_rtSelectOutline;						// UV of outline of selected items

	BOOL				m_bShowItemInfo;						// If item tool tip is shown or not
	BOOL				m_bDetailItemInfo;						// If item informaion is shown in detail or not
	int					m_nCurInfoLines;						// Count of current item information lines
	UIRect				m_rcItemInfo;							// Item information region	

	// Buttons
	CUIButton			m_btnClose;								// Close button of Shop
	CUIButton			m_btnShopBuy;							// Buy button of Shop
	CUIButton			m_btnShopSell;							// Sell button of Shop
	CUIButton			m_btnShopCancel;						// Cancel button of Shop
	CUIScrollBar		m_sbTopScrollBar;						// Scroll bar of shop
	CUIScrollBar		m_sbBottomScrollBar;					// Scroll bar of inventory

	// etc
	BOOL				m_bIsLease;
	BOOL				m_bIsFieldShop;							// ttos : 잡화상 이용 주문서 사용시 체크
	// event : 가정의 달 이벤트
	CUIButton			m_btnEvent_weapon;						// ttos : 무기류
	CUIButton			m_btnEvent_shield;						// ttos : 방어구류
	BOOL				m_bIsEvent;								// ttos : 2007 가정의 달 이벤트 
	int					m_iEventjob;

	// Items	
	CUIIcon*			m_pIconsTrade[SHOP_TRADE_SLOT_TOTAL];		// Trade Slot items
	CUIIcon*			m_pIconsShop[SHOP_SLOT_BUY_MAX];			// Shop Slot items

	typedef		std::vector< CItems* >		vec_items;

	vec_items			m_vecTrade;

public:
	CUIShop();
	~CUIShop();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	OpenShop( int iMobIndex, int iMobVirIdx, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	void	EventOpenShop( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// Clear
	void	ClearItems();
	void	RefreshPlayerItem();
	void	ResetShop();
	void	CloseShop(); //WSS_SHOP_BUGFIX 070530

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );
	inline void SetFieldShopChack ( BOOL bChack) {m_bIsFieldShop = bChack;}

	void	CreateCubeStateMsgBox(CNetworkMessage *istr, BOOL bGuild);

	void	AddItemCallback();
	void	DelItemCallback();

	CItems*	GetTradeItem(int idx);

	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );
	BOOL	IsBuyShop()	{ return m_bBuyShop;	}
	int		GetShopID()	{ return m_nSelectedShopID;	}
	__int64	CalculateItemPrice(int iShopID, const CItemData &ID, int iNumOfItem, BOOL bSell);
protected:
	// Internal functions
	void	RenderShopItems();

	void	TradeToShop( SQUAD llCount );
	void	ShopToTrade( SQUAD llCount );
	__int64	CalculateTotalPrice(int iShopID, int& iCount, BOOL bSell, BOOL bRate = TRUE);
	
	void	PrepareBuyShop();
	void	PrepareSellShop();	
	void	PrepareLeaseShop(int jobIdx, int itype = CItemData::ITEM_WEAPON);	

	// Command functions
	void	AddShopItem( int invenIdx, int nUniIndex, SQUAD llCount );
	void	DelShopItem( int invenIdx, int nUniIndex, SQUAD llCount, int nTradeItemID );
	void	BuyItems();
	void	SellItems();

	int		RefreshShopItem(int iShopIndex);
	int		RefreshLeaseItem(int iJobIndex);
	int		RefreshEventItem(int iJobIndex, int itype); // 2007 가정의 달 이벤트 (어린이날 이벤트)

	void	clearTrade();
};

#endif // UISHOP_H_

