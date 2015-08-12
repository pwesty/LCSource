// ----------------------------------------------------------------------------
//  File : UIExchange.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIEXCHANGE_H_
#define	UIEXCHANGE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

//#include <Engine/Interface/UIInventory.h>

// Exchange request type
enum ExchangeReq
{
	EXCHREQ_REQ				= 0,
	EXCHREQ_REP				= 1,
	EXCHREQ_REJECT			= 2,
	EXCHREQ_REJECT_SRC		= 3,
	EXCHREQ_REJECT_DEST		= 4,
	EXCHREQ_READY			= 5,
	EXCHREQ_READY_SRC		= 6,
	EXCHREQ_READY_DST		= 7,
	EXCHREQ_FULL_SRC		= 8,
	EXCHREQ_FULL_DEST		= 9,
	EXCHREQ_REQ_OK			= 10,
	EXCHREQ_REQ_OK_SRC		= 11,
	EXCHREQ_REQ_OK_DEST		= 12
};


// Exchange item action type
enum ExchangeItemAction
{
	EXCHITEM_ADD			= 0,
	EXCHITEM_ADD_SRC		= 1,
	EXCHITEM_ADD_DEST		= 2,
	EXCHITEM_DEL			= 3,
	EXCHITEM_DEL_SRC		= 4,
	EXCHITEM_DEL_DEST		= 5,
	EXCHITEM_UPDATE_SRC		= 6,
	EXCHITEM_UPDATE_DEST	= 7
};


// Define exchange item slot
#define	EXCH_EXCH_SLOT_COL			5
#define	EXCH_EXCH_SLOT_TOTAL		10
#define	EXCH_INVEN_SLOT_COL			5
#define	EXCH_INVEN_SLOT_ROW			4		
#define	EXCH_INVEN_SLOT_ROW_TOTAL	20
#define	EXCH_SLOT_SX				14
#define	EXCH_OPP_SLOT_SY			51
#define	EXCH_MY_SLOT_SY				150
#define	EXCH_INVEN_SLOT_SY			249


// Define text position
#define	EXCH_TITLE_TEXT_OFFSETX		25
#define	EXCH_TITLE_TEXT_OFFSETY		5
#define	EXCH_OPP_NAME_OFFSETX		58
#define	EXCH_OPP_NAME_OFFSETY		33
#define	EXCH_MONEY_OFFSETX			180
#define	EXCH_OPP_MONEY_OFFSETY		124
#define	EXCH_MY_MONEY_OFFSETY		223
#define	EXCH_MYCUR_MONEY_OFFSETY	392


// Define size of exchange
#define	EXCHANGE_WIDTH				216
#define	EXCHANGE_HEIGHT				452

#define	DEF_INVENTORY_MAX			(ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1 + ITEM_COUNT_IN_INVENTORY_CASH_2)


// ----------------------------------------------------------------------------
// Name : CUIExchange
// Desc :
// ----------------------------------------------------------------------------
class CUIExchange : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnClose;								// Close button
	CUIButton			m_btnReady;								// Ready button
	CUIButton			m_btnExchange;							// Exchange button
	CUIButton			m_btnCancel;							// Cancel button
	CUIScrollBar		m_sbScrollBar;							// Scroll bar

	// Exchange items
	CUIIcon*			m_pIconsOppItems[EXCH_EXCH_SLOT_TOTAL];	// Items of opposite slot
	CUIIcon*			m_pIconsMyItems[EXCH_EXCH_SLOT_TOTAL];	// Items of my slot
	CUIIcon*			m_pIconsInvenItems[DEF_INVENTORY_MAX];	// Items of inventory slot

	// Item information
	int					m_nSelMyItemID;							// Selected item ID of my slot
	int					m_nSelITab;
	int					m_nSelInvenItemID;						// Selected item ID of inventory slot
	BOOL				m_bShowItemInfo;						// If item tool tip is shown or not
	BOOL				m_bDetailItemInfo;						// If item informaion is shown in detail or not
	int					m_nCurInfoLines;						// Count of current item information lines
	UIRect				m_rcItemInfo;							// Item information region

	BOOL				m_bRareItem;
	int					m_iRareGrade;

	// Exchange information
	BOOL				m_bAmISrc;								// If I am source or not
	int					m_nSrcIndex;							// Source index
	int					m_nDestIndex;							// Destination index
	CTString			m_strSrcName;							// Source name
	CTString			m_strDestName;							// Destination name
	BOOL				m_bExchReady;							// If exchange is ready or not
	BOOL				m_bExchReadyDest;						// If exchange of target is ready or not
	CTString			m_strMyCurMoney;						// Current money in inventory
	CTString			m_strOppExchMoney;						// Exchange money of opposite
	CTString			m_strMyExchMoney;						// Exchange money of me

	// Region of each part
	UIRect				m_rcTitle;								// Region of title
	UIRect				m_rcOppSlot;							// Region of opposite slot
	UIRect				m_rcMySlot;								// Region of my slot
	UIRect				m_rcInvenSlot;							// Region of inventory slot

	// UV of each part
	UIRectUV			m_rtBackground;							// UV of background
	UIRectUV			m_rtInfoL;								// UV of left region of information
	UIRectUV			m_rtInfoM;								// UV of middle region of information
	UIRectUV			m_rtInfoR;								// UV of right region of information
	UIRectUV			m_rtInfoUL;								// UV of upper left region of information
	UIRectUV			m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV			m_rtInfoUR;								// UV of upper right region of information
	UIRectUV			m_rtInfoML;								// UV of middle left region of information
	UIRectUV			m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV			m_rtInfoMR;								// UV of middle right region of information
	UIRectUV			m_rtInfoLL;								// UV of lower left region of information
	UIRectUV			m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV			m_rtInfoLR;								// UV of lower right region of information
	UIRectUV			m_rtWearingOutline;						// UV of outline of wearing items
	UIRectUV			m_rtSelectOutline;						// UV of outline of selected items

	//2013/04/03 jeil 나스 아이템 제거
	UIRect			m_rtTouchNas;							//터치했을때 나스 입력창을 열어주는 부분 
	
	__int64				m_nMyNas;
	__int64				m_nTradeNas;

protected:
	// Internal functions
	void	PrepareExchange();
	void	RenderItems();

	// Command functions
	void	AddExchItem( int nTab, int inven_idx, int nUniIndex, SQUAD llCount );

	// Network message functions ( send )
	void	SendExchangeReq_Rep();
	void	SendExchangeReq_Rej();
	void	SendExchangeReq_Ready();
	void	SendExchangeReq_Ok();

	//2013/04/09 jeil 나스 아이템 제거 
	void	SendExchangeItem_Add( int nUniIndex, SQUAD llCount = 0 );
public:
	CUIExchange();
	~CUIExchange();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Network message functions ( send )
	void	SendExchangeReq_Req( CTString &strSrcName );

	// Network message functions ( receive )
	void	ExchangeReq_Req( int nSrcIndex, CTString &strSrcName,
								int nDestIndex, CTString &strDestName );
	void	ExchangeReq_Rep();
	void	ExchangeReq_Rej( BOOL bSrcReject );
	void	ExchangeReq_Ready( BOOL bSrcReady );
	void	ExchangeReq_BothReady();
	void	ExchangeReq_Full( BOOL bSrcFull );
	void	ExchangeReq_Ok( BOOL bSrcOk );
	void	ExchangeReq_BothOk();
	void	ExchangeReq_Pet_Err(BOOL petErr);

	void	ExchangeItem_Add( BOOL bSrcAdd, int nUniIndex, SQUAD llCount,
								int nIndex, ULONG ulPlus, ULONG ulFlag, LONG ulUsed, LONG ulUsed2, LONG lRareIndex =0 );
	void	SetDurability( BOOL bSrcAdd, int nUniIndex, LONG nDuraNow, LONG nDuraMax );
	void	ExchangeItem_SetOptionData( BOOL bSrcAdd, int nUniIndex, SBYTE sbOption,
										SBYTE sbOptionType, LONG lOptionLevel, LONG lRareIndex, LONG lOriginOptionVar );
	void	ExchangeItem_SetRareOption( CNetworkMessage *istr, BOOL bSrcAdd, int nUniIndex);
	void	ExchangeItem_Del( BOOL bSrcDel, int nUniIndex );
	// by jeil Delete nas item
	void	ExchangeItem_Update( BOOL bSrcUpdate, int nUniIndex, SQUAD llCount , SQUAD llNas = 0);

	void	ResetExchange();

	void	ExchangeItem_SetSkill( CNetworkMessage *istr, BOOL bSrcAdd, int nUniIndex);
	void	ExchangeItem_InitSocket( BOOL bSrcAdd, int nUniIndex );
	void	ExchangeItem_SetSocketData( BOOL bSrcAdd, int nUniIndex, SBYTE sbSocketCreateCount,
												SBYTE sbSlotIndex, LONG lJewelIndex );
	void	ExchangeItem_SetPlus2( BOOL bSrcAdd, int nUniIndex, LONG plus2 );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	//2013/04/03 jeil 나스 아이템 제거
	void	ExchangeNas();

	void	AddItemCallback();
	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );
};


#endif	// UIINVENTORY_H_

