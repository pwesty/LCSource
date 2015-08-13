// ----------------------------------------------------------------------------
//  File : UIWareHouse.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIWAREHOUSE_H_
#define	UIWAREHOUSE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


class CItems;

// Column & Row
#define WAREHOUSE_WAREHOUSE_SLOT_COL		5
#define WAREHOUSE_WAREHOUSE_SLOT_ROW		5
#define WAREHOUSE_WAREHOUSE_SLOT_ROW_TOTAL	60	 // wooss 050817 10 -> 20 050917 20 -> 10 유료화시 복구
												 // 051208 -> 유료화 작업으로 
#define WAREHOUSE_TRADE_SLOT_COL			5
#define	WAREHOUSE_TRADE_SLOT_TOTAL			10


// Define text position
#define	WAREHOUSE_TITLE_TEXT_OFFSETX		25
#define	WAREHOUSE_TITLE_TEXT_OFFSETY		5
#define	WAREHOUSE_MYMONEY_POSX				95
#define	WAREHOUSE_MYMONEY_POSY				310		// wooss 050811 283 -> 310
#define	WAREHOUSE_TRADEPRICE_POSX			177
#define	WAREHOUSE_BUY_TRADEPRICE_POSY		312		// wooss 050811 285 -> 312
#define	WAREHOUSE_SELL_TRADEPRICE_POSY		101
#define	WAREHOUSE_EXTEND_TEXT_POSX			15		// wooss 
#define	WAREHOUSE_EXTEND_TEXT_POSY			26		// wooss 
#define	WAREHOUSE_EXTEND_DAYS_POSX			180		// wooss 

// Define size of warehouse
#define WAREHOUSE_WIDTH						216
#define	WAREHOUSE_HEIGHT					392	// wooss 050811 338 -> 369 //jeil -> 369->392

enum ErrCode 
{
	eERR_KEEP_FAIL_EMPTY,
	eERR_TAKE_FAIL_EMPTY
};

// ----------------------------------------------------------------------------
// Name : CUIWareHouse
// Desc :
// ----------------------------------------------------------------------------
class CUIWareHouse : public CUIWindow
{
public:
	CUIWareHouse();
	~CUIWareHouse();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	OpenWareHouse( SBYTE sbSetPW, bool bCashRemoteOpen = false );
	void	SetNPCPos(int nNPCIdx, FLOAT fX, FLOAT fZ);

	BOOL	CloseWindowByEsc()	{ ResetWareHouse(); return TRUE; }

	// Add & Remove Item
	void	AddItemToList( int iUniIndex, SQUAD llCount );
	void	DelItemFromList( int iUniIndex, SQUAD llCount );

	inline bool HasPassword() const		{ return m_bHasPassword; }
	void	SetHasPassword(bool bVal)	{ m_bHasPassword = bVal; }

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// Clear
	void	ClearItems();
	void	ResetWareHouse();

	// WareHouse -> Inventory
	void	DelWareHouseItemToInventory();

	// Refresh
	void	RefreshWareHouse();
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Network functions
	void	ReceiveNas(CNetworkMessage *istr);
	void	ReceiveList(CNetworkMessage *istr);

	// GetItemInfo
	int			GetWareHouseItemIndex( int nInvenIdx );
	LONGLONG	GetWareHouseItemCount( int nInvenIdx );

	// set,get use time
	void SetUseTime(int t){m_useTime=t;}
	int GetUseTime(){return m_useTime;}

	float GetNpcPosX() { return m_fNpcX; }
	float GetNpcPosZ() { return m_fNpcZ; }

	bool GetUseCashRemote() { return m_bCashRemote; }

	void	SendListReq(CTString& strPW);

	void	AddItemCallback();
	void	DelItemCallback();

protected:
	// Internal functions
	void	RenderWareHouseItems();

	void	PrepareWareHouse();
	void	TradeToWareHouse(SQUAD llCount = 1);
	void	WareHouseToTrade(SQUAD llCount = 1);
	void	PrepareBuyWareHouse();
	void	PrepareSellWareHouse();	
	SQUAD	CalculateStoragePrice();

	// Command functions
	void	AddWareHouseItem(int nIndex);
	void	DelWareHouseItem();
	void	AddWareHouseItemFromInventory();	
	void	KeepItems();
	void	TakeItems();

	// Network functions
// 강동민 수정 시작		// 창고
	void	SendWareHouseIsSetPassword();
	void	SendWareHouseListReq( const CTString& strPW );
	void	SendWareHouseKeepReq();
	void	SendWareHouseTakeReq();
// 강동민 수정 끝		// 창고


	//2013/04/02 jeil 나스 아이템 제거
	void	InNas();
	void	OutNas();

	// Err Msg
	void	ErrMessage( int nErr );	

private:
	BOOL				m_bStorageWareHouse;
	bool				m_bHasPassword;
	int					m_nCurRow;
	int					m_nNpcIndex;				// For showing name of npc
	int					m_nNPCVIdx;
	BOOL				m_bHasQuest;

	int					m_nSelWareHouseItemID;
	int					m_nSelTradeItemID;
	int					m_nSelectedWareHouseID;		// WareHouse ID
	int					m_nTex;

	BOOL				m_bRareItem;
	int					m_iRareGrade;

	CTString			m_strTotalPrice;	//보관료 
	//2013/04/02 jeil 나스 아이템 제거 
	CTString			m_strInNas;	//입금 금액
	CTString			m_strOutNas;	//출금 금액 
	CTString			m_strTotalNas;	//보관 금액 

	CTString			m_strPW;

	__int64				m_nTotalPrice;
	//2013/04/03 jeil 나스 아이템 제거
	__int64				m_nInNas;
	__int64				m_nOutNas;
	__int64				m_nTotalNas;

	int					m_nNumOfItem;

	//cash item remain time
	int					m_useTime;				// wooss 050817	
	// Region
	UIRect				m_rcMainTitle;	
	UIRect				m_rcTop;				// WareHouse
	UIRect				m_rcBottom;				// Trade
	UIRect				m_rcExInvenRect;		// wooss 050811

	// Background
	UIRectUV			m_rtBackTop;
	UIRectUV			m_rtBackMiddle;
	UIRectUV			m_rtBackBottomLine;
	UIRectUV			m_rtBackBottom;

	// wooss 050811
	// extand inventroy remain days
	UIRectUV			m_rtExInven;


	// Position of target npc
	FLOAT				m_fNpcX, m_fNpcZ;

	// Inventory
	UIRectUV			m_rtTopTopInven;
	UIRectUV			m_rtTopMiddleInven;
	UIRectUV			m_rtTopBottomInven;
	UIRectUV			m_rtBottomInven;
	UIRectUV			m_rtSeperatorInven;
	UIRectUV			m_rtBlankInven;
	UIRectUV			m_rtPriceInven;

	// Item Info
	UIRectUV			m_rtSelectOutline;						// UV of outline of selected items

	// Buttons
	CUIButton			m_btnClose;								// Close button of WareHouse
	CUIButton			m_btnWareHouseKeep;						// Keep button of WareHouse
	CUIButton			m_btnWareHouseTake;						// Take button of WareHouse
	CUIButton			m_btnWareHouseCancel;					// Cancel button of WareHouse	
	CUIScrollBar		m_sbTopScrollBar;						// Scroll bar of warehouse
	//2013/04/02 jeil 나스 아이탬 제거 
	CUIButton			m_btnInNas;								//입금 버튼
	CUIButton			m_btnOutNas;							//출금 버튼

	// Storage Items
	typedef		std::vector< CItems* >		vec_Items;
	typedef		vec_Items::iterator			vec_Items_iter;
	
	vec_Items			m_vectorStorageItems;
	vec_Items			m_vecTradeItems;
	CItems*				m_pItemsTemp;
	int					m_nTempTradeIdx;
	SQUAD				m_nTempMax;
	int					m_nTempStorageIdx;
	// Items	
	CUIIcon*			m_pIconTradeItems[WAREHOUSE_TRADE_SLOT_TOTAL];			// Player Slot items
	CUIIcon* 			m_pIconWareHouseItems[ITEM_COUNT_IN_INVENTORY_STASH];	// WareHouse Slot items

	bool				m_bCashRemote;
	int					m_nTmpUniIndex; 
};

#endif // UIWAREHOUSE_H_

