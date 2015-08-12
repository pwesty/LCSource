// ----------------------------------------------------------------------------
//  File : UICashShop.h
//  Desc : cashitem shop interface
//  code : wooss
// ----------------------------------------------------------------------------
#ifndef	UICASHSHOP_H_
#define	UICASHSHOP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIMessageBox.h>
#include <Engine/Interface/UIQuickSlot.h>

class CUIQuickSlot;
#define CASHSHOP_WIDTH						598		
#define	CASHSHOP_HEIGHT						449
#define SETINFO_WIDTH						300
#define SETINFO_HEIGHT						330
#define INVEN_SLOT_TOTAL					10
#define KIT_SLOT_TOTAL						25
#define	LEFT_SLOT_SIZE_BUY					8
#define RIGHT_SLOT_SIZE_BUY					5
#define	SET_ITEM_INFO						5
#define MAX_KIT_SIZE						10

#define MAX_GIFT_MESSAGE					100		//선물메시지의 최대 길이	:Su-won

// max line of item informaion 
#define	MAX_CASH_ITEMINFO_LINE				20


// type of Main TAB
enum SHOP_TAB
{
	UCST_NEW_CASH = 0,
	UCST_BUY,
	UCST_BUY_INFO,
	UCST_HELP,
	UCST_MAIN_END,
};

enum SHOP_SECTION
{	
	// SHOP_SECTION
	UCSS_CONSUME	= 0,
	UCSS_TIME_LIMIT,
	UCSS_POTION,
	UCSS_AVATA,
	UCSS_WEAPON,
	UCSS_DEFEND,
	UCSS_SET,
	// NEW_SECTION
	UCSS_PLATINUM,
	UCSS_NEWITEM,
	UCSS_HOTITEM,	
	//INFO_SECTION
	UCSS_BUY_INFO ,
	UCSS_BUY_HISTORY,
	UCSS_BUY_SENDHISTORY,		//'선물 내역(보낸 선물내역 확인)' 페이지 :Su-won
	UCSS_BUY_RECEIVEHISTORY,	//'선물 내역(받은 선물내역 확인)' 페이지 :Su-won
	UCSS_BUY_RECEIVE,			//'받은 선물' 페이지  :Su-won
	
	UCSS_SUB_END
};


class CUIConfirmInfo : public CUIWindow
{
		
protected :
	// Set Item Info Wondow ------------------------------------------------->
	
	// Region of each part
	UIRect				m_rcTitle;						// Region of title
	CUIButton			m_btnClose;							// Close Info window
	CUIButton			m_btnOK;							// Close Info window
	CUIButton			m_btnCancel;							// Close Info window
	
	
	// Set Item Info Wondow <-------------------------------------------------

	// UV of each part
	UIRectUV			m_rtBackUL;							// UV of upper left background
	UIRectUV			m_rtBackUM;							// UV of upper middle background
	UIRectUV			m_rtBackUR;							// UV of upper right background
	UIRectUV			m_rtBackML;							// UV of middle left background
	UIRectUV			m_rtBackMM;							// UV of middle middle background
	UIRectUV			m_rtBackMR;							// UV of middle right background
	UIRectUV			m_rtBackLL;							// UV of lower left background
	UIRectUV			m_rtBackLM;							// UV of lower middle background
	UIRectUV			m_rtBackLR;							// UV of lower right background

	UIRectUV			m_rtItemSlot;						// UV of item slot
	UIRectUV			m_rtLineH;							// UV of upper line 
	UIRectUV			m_rtLineV;							// UV of Lower line
	UIRectUV			m_rtBlackBox;						// UV of Black Box
	UIRectUV			m_rtBlackboxL;						// UV of Black Box with line 


public :
	CUIConfirmInfo();
	~CUIConfirmInfo();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
		
	// Render
	void	Render();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	

};


class CUISetInfo : public CUIWindow
{
		
protected :
	// Set Item Info Wondow ------------------------------------------------->
	
	// Region of each part
	UIRect				m_rcSetTitle;						// Region of title
	CUIScrollBar		m_sbSet;							// Set item info scroll bar
	CUIButtonEx			m_btnSetItem;						// Set Item Button Image	
	CUIButtonEx*		m_btnSetInfo;						// Set Item Button Image	
	CUIButton			m_btnSetClose;						// Close Info window
	CUIButton			m_btnSetOK;						// Close Info window
	
	// Variable
	int					m_nGoodsNum;					// select section slot item number

	// Set Item Info Wondow <-------------------------------------------------

	// UV of each part
	UIRectUV			m_rtBackUL;							// UV of upper left background
	UIRectUV			m_rtBackUM;							// UV of upper middle background
	UIRectUV			m_rtBackUR;							// UV of upper right background
	UIRectUV			m_rtBackML;							// UV of middle left background
	UIRectUV			m_rtBackMM;							// UV of middle middle background
	UIRectUV			m_rtBackMR;							// UV of middle right background
	UIRectUV			m_rtBackLL;							// UV of lower left background
	UIRectUV			m_rtBackLM;							// UV of lower middle background
	UIRectUV			m_rtBackLR;							// UV of lower right background

	UIRectUV			m_rtItemSlot;						// UV of item slot
	UIRectUV			m_rtLineH;							// UV of upper line 
	UIRectUV			m_rtLineV;							// UV of Lower line
	UIRectUV			m_rtBlackBox;						// UV of Black Box
	UIRectUV			m_rtBlackboxL;						// UV of Black Box with line 


public :
	CUISetInfo();
	~CUISetInfo();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight ,int nGoodsNum);

	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
		
	// SetItem Info
	void	SetItemInfo(CUIButtonEx& btnSrc);

	// Clrear Button Items
	void	ClearBtnItems(CUIButtonEx* btn,int size);


	// Render
	void	Render();

	void	RenderSetItem();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	

};


// ----------------------------------------------------------------------------
// Name : CUICashShop
// Desc : ref. CUIShop 
// ----------------------------------------------------------------------------
class CUICashShop : public CUIWindow
{
protected:

	
	int					m_nCurShopTab;						// cash shop current tab
	int					m_nCurShopSection;					// cash shop current section
	int					m_nOldShopTab;						// cash shop current tab
	int					m_nOldShopSection;					// cash shop current section
	int					m_nGoodsSlotNum;					// select section slot item number
	int					m_nKitSlotNum;						// shopping basket slot item number
	int					m_nShopItemID;						// mouse clicked shop item
	int					m_nTradeItemID;						// mouse clicked trade item
	int					m_nKitItemID;						// mouse clicked shop item
	int					m_nInvenItemID;						// mouse clicked trade item
	int					m_nRecvItemID;						// mouse clicked Receive Gift item	 :Su-won
	SQUAD				m_nMyCash;							// my cash			

	// Show item info
	BOOL				m_bShowShopInfo;
	BOOL				m_bShowTradeInfo;
	BOOL				m_bShowKitInfo;
	BOOL				m_bShowInvenInfo;
	BOOL				m_bShowHistoryInfo;
	int					m_iShowItemIdx;						// show item index
	UIRect				m_rcItemInfo;						// Item information region	


	// temp mouse message
	int	m_x,m_y;

	// BUttons
	// BUY TAB
	CUIButton			m_btnClose;							// Close button
	
	CUIButton			m_btnNewItem;						// New ITems
	CUIButton			m_btnHotItem;						// Hot Items

	CUIButton			m_btnConsume;						// consume item button
	CUIButton			m_btnTimeLimit;						// time limit item
	CUIButton			m_btnPotion;						// potion item
	CUIButton			m_btnAvata;							// avata item
	CUIButton			m_btnWeapon;						// arm item
	CUIButton			m_btnDefend;						// depend item
	CUIButton			m_btnSet;							// item set button
	CUIButton			m_btnDetail;						// detail list of set item
	CUIButton			m_btnClear;							// remove all items in buy bastket
	CUIButton			m_btnPay;							// pay button
	CUIButton			m_btnCancel;						// same function with close button
	CUIButton			m_btnSearch;						// Search puchase history button
	CUIButton			m_btnPlatinum;	
	CUIButton			m_btnBillingLink;					// Billing Homepage Link

	// Set detail
	CUIButton			m_aBtnSetItem[LEFT_SLOT_SIZE_BUY];	// set item array
 
	
	// INFO_TAB
	CUIButton			m_btnInfoBuyItem;					// bought item list
	CUIButton			m_btnInfoBuyHistory;				// buy history
	CUIButton			m_btnInfoMove;						// move into user inventory button
	CUIButton			m_btnInfoGiftHistory;				// "선물내역" 버튼				:Su-won
	CUIButton			m_btnInfoSendHistory;				// "보낸 선물내역 확인"	버튼	:Su-won
	CUIButton			m_btnInfoReceiveHistory;			// "받은 선물내역 확인"	버튼	:Su-won
	CUIButton			m_btnGiftSearch;					// "검색" 버튼					:Su-won
	CUIButton			m_btnInfoReceive;					// "받은선물" 버튼				:Su-won
	CUIButton			m_btnGift;							// "선물하기" 버튼				:Su-won
	CUIButton			m_btnSend;							// "보내기"	버튼				:Su-won

	// Scroll Bar
	CUIScrollBar		m_sbLeft;							// left Scroll Bar
	CUIScrollBar		m_sbRight;							// Right Scroll Bar

public :
	CUIScrollBar		m_sbHistory;						// History Scroll Bar
	CUIScrollBar		m_sbGift;							// 선물 내역 스크롤바 :Su-won
	CUIScrollBar		m_sbReceive;						// 받은 선물 스크롤바: Su-won
	int					m_nSendScrollPos, m_nRecvScrollPos; // 선물 내역(보낸 선물, 받은 선물) 스크롤바 위치:Su-won
	// List Box		
	CUIComboBox			m_cbYear;							// search year list
	CUIComboBox			m_cbMonth;							// search month list
	CUIComboBox			m_cbDay;							// search day list

	CUIComboBox			m_cbGiftYear;							// 선물 내역 '년' 리스트 :Su-won
	CUIComboBox			m_cbGiftMonth;							// 선물 내역 '달' 리스트 :Su-won
	CUIComboBox			m_cbGiftDay;							// 선물 내역 '일' 리스트 :Su-won
	int					m_nSendYear, m_nSendMonth, m_nSendDay;	// 보낸 선물 내역 날짜 :Su-won
	int					m_nRecvYear, m_nRecvMonth, m_nRecvDay;	// 받은 선물 내역 날짜 : Su-won

	BOOL				m_bShowSendGift;					// 선물 보내기창 생성 여부 :Su-won
	CUIEditBox			m_ebChar;							// 선물 보내기창에서 캐릭 이름을 위한 에디트박스  :Su-won
	CUIMultiEditBox		m_ebGiftMessage;					// 선물 보내기창에서 선물 메시지를 위한 에디트박스 :Su-won
	
	BOOL				m_bBringItem;						// 캐쉬 아이템을 가져오는 중

protected :	

	// Region of each part
	UIRect				m_rcTitle;							// Region of title
	UIRect				m_rcTab;							// Region of tab
	UIRect				m_rcShopItems;						// Region of ShopItems
	UIRect				m_rcTradeItems;						// Region of KitItems
	UIRect				m_rcKitItems;
	UIRect				m_rcInvenItems;
	UIRect				m_rcRecvItems;						// 받은 선물을 렌더링할 영역 :Su-won
	UIRect				m_rcHelp;							// Region of Help
	// connie [2009/10/23] - test
	UIRect				m_rcKitItemsJP;
	// UV of each part
	UIRectUV			m_rtBackUL;							// UV of upper left background
	UIRectUV			m_rtBackUM;							// UV of upper middle background
	UIRectUV			m_rtBackUR;							// UV of upper right background
	UIRectUV			m_rtBackML;							// UV of middle left background
	UIRectUV			m_rtBackMM;							// UV of middle middle background
	UIRectUV			m_rtBackMR;							// UV of middle right background
	UIRectUV			m_rtBackLL;							// UV of lower left background
	UIRectUV			m_rtBackLM;							// UV of lower middle background
	UIRectUV			m_rtBackLR;							// UV of lower right background
	UIRectUV			m_rtTab;							// UV of section tab
	UIRectUV			m_rtItemSlot;						// UV of item slot
	UIRectUV			m_rtLineH;							// UV of upper line 
	UIRectUV			m_rtLineV;							// UV of Lower line
	UIRectUV			m_rtBlackBox;						// UV of Black Box
	UIRectUV			m_rtBlackboxL;						// UV of Black Box with line 

	// Item Info from UIShop ^^;
	UIRectUV			m_rtInfoUL;								// UV of upper left region of information
	UIRectUV			m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV			m_rtInfoUR;								// UV of upper right region of information
	UIRectUV			m_rtInfoML;								// UV of middle left region of information
	UIRectUV			m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV			m_rtInfoMR;								// UV of middle right region of information
	UIRectUV			m_rtInfoLL;								// UV of lower left region of information
	UIRectUV			m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV			m_rtInfoLR;								// UV of lower right region of information
	
	int				 	m_nCurInfoLines;						// Count of current item information lines
	CTString			m_strItemInfo[MAX_CASH_ITEMINFO_LINE];							// Item information string
	COLOR				m_colItemInfo[MAX_CASH_ITEMINFO_LINE];							// Color of item information string
	
	UIRectUV			m_rtSelectOutlineUL;					// UV of upper left outline of selected items
	UIRectUV			m_rtSelectOutlineUM;					// UV of upper middle outline of selected items
	UIRectUV			m_rtSelectOutlineUR;					// UV of upper right outline of selected items
	UIRectUV			m_rtSelectOutlineML;					// UV of middle left outline of selected items
	UIRectUV			m_rtSelectOutlineMR;					// UV of middle right outline of selected items
	UIRectUV			m_rtSelectOutlineLL;					// UV of lower left outline of selected items
	UIRectUV			m_rtSelectOutlineLM;					// UV of lower middle outline of selected items
	UIRectUV			m_rtSelectOutlineLR;					// UV of lower right outline of selected items

public:
	CUICashShop();
	~CUICashShop();

	// setitem info 
	CUISetInfo* m_pSetInfo;
	// confirm item info
	CUIConfirmInfo* m_pConfirmInfo;

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	void	CreateSetUI( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	void	OpenCashShop();

	void	OpenRecvGiftPage();		// 선물 도착 버튼을 클릭했을 때 바로 받은 선물 페이지를 오픈 :Su-won
	
	
	void	PrepareShopItems();

	void	ClearBtnItems(CUIButtonEx* btn,int size);

	void	CloseCashShop();

	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
		
	// Render
	void	Render();

	void	RenderSetUI();

	void	RenderShopItems();
	
	void	RenderTradeItems();

	void	RenderKitItems();

	void	RenderInvenItems();

	void	RenderHistoryItems();
	void	RenderSendHistoryItems();		// 받은 선물 내역 렌더링 :Su-won
	void	RenderReceiveHistoryItems();	// 보낸 선물 내역 렌더링 :Su-won
	void	RenderRecvGift();				// 받은 선물 렌더링 :Su-won

	void	RenderItemInfo( CUIButtonEx& btnInfo );

	void	RednerShopItemInfo( CUIButtonEx& btnInfo );

	void	RenderOutline( const float fI0, const float fJ0 );

	void	RenderHelp();

	void	RenderSendGiftUI();				// 선물 보내기창 렌더링 :Su-won
	// Other Func
	void	InsideNumXY(UIRect rc,int btnSize,int btnGap, int* n_x, int* n_y );
	int		CalculateKitPrice();
	BOOL	SetCashIndexToBtn(int nCashIndex,int nUniIndex,CUIButtonEx& btn );
	inline void	SetMyCash( int nCash) 
	{	m_nMyCash=nCash; 
#if defined(G_GERMAN) || defined(G_EUROPE3) || defined(G_EUROPE2)
		if(m_nMyCash <= 0)
		{
			m_btnPay.SetEnable(FALSE);
		}else
		{
			m_btnPay.SetEnable(TRUE);	
		}
#endif
	}
	inline int GetMyCash() { return m_nMyCash; }
	inline int GetCurShopSection(){ return m_nCurShopSection; }


	// 받은 선물함이 비었는지 확인
	BOOL	IsEmptyRecvGift();
	
	//Su-won   --->
	BOOL	IsEditBoxFocused();
	void	KillFocusEditBox()
	{
		m_ebChar.SetFocus( FALSE );
		m_ebGiftMessage.SetFocus( FALSE );
	}
	void Message( int nCommandCode, CTString strTitle, CTString strMessage, DWORD dwStyle );
	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	//Su-won   <--
	
	// For multi lines
	void	AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo ,int maxLine, int maxChars );
	void	InitShowInfo();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	void	ButtonMoveMsg( MSG *pMsg );

	// 선물 보낼 때 캐릭 이름과 선물 메시지를 입력받기 위해 추가 :Su-won	|--->
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	CharMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	//	<---|

	void OpenBillingHomePage();
	// Item Buttons	
	CUIButtonEx			m_abtnTradeItems[MAX_KIT_SIZE];						// Trade Slot items
	
	CUIButtonEx*		m_pbtnShopItems;				// Shop items
	int					m_shopItemCnt;

	CUIButtonEx			m_abtnShowItem;		// show item image
		
	CStaticArray<CUIButtonEx>	m_abtnKitItems;//[KIT_SLOT_TOTAL]];			// Bought items
	CStaticArray<CUIButtonEx>	m_abtnInvenItems;//[SHOP_SLOT_ROW_TOTAL];	// items to move to Inventory 
	CStaticArray<CUIButtonEx>	m_abtnHistoryItems;							// items of Histrory Slot 
	
	// Su-won	|--->
	CStaticArray<CUIButtonEx>	m_abtnSendHistoryItems;			// 보낸 선물 내역의 아이템 슬롯	:Su-won
	CStaticArray<CTString>		m_astrSendChar;					// 보낸 선물 내역의 선물 보낸 캐릭명 :Su-won
	CStaticArray<CUIButtonEx>	m_abtnRecvHistoryItems;			// 받은 선물 내역의 아이템 슬롯 :Su-won
	CStaticArray<CTString>		m_astrReceiveChar;				// 받은 선물 내역의 선물 받은 캐릭명 :Su-won
	
	CStaticArray<CUIButtonEx>	m_abtnRecvGift;					// 받은 선물 슬롯 :Su-won
	CStaticArray<CTString>		m_astrSend;						// 받은 선물의 선물 보낸 캐릭명 :Suw0on
	CStaticArray<CTString>		m_astrGiftMessage;				// 받은 선물의 선물 메시지 :Su-won
	CStaticArray<UINT>			m_anDate;						// 받은 선물의 선물 보낸 날짜 :Su-won

	// 받은 선물이 선물 슬롯과 인벤 슬롯을 왔다갔다하면서 순서가 바뀌기 때문에 순서를 저장.
	// 선물 버튼과 보낸 (캐릭명, 날짜, 선물 메시지)를 따로 처리하기때문에 필요...
	CStaticArray<UINT>			m_anRecvOrder;					// 받은 선물 슬롯에 저장된 받은 선물 순서 :Su-won
	CStaticArray<UINT>			m_anInvenOrder;					// 인벤 슬롯에 저장된 받은 선물 순서 :Su-won
	
	CUIMultiEditBox				m_ebRecvGiftMsg[2];				// 받은 선물의 메시지를 나타낼 에디트 박스
	// Su-won	<---|
};


#endif UICASHSHOP_H_
