// ----------------------------------------------------------------------------
//  File : UIQuiz.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIQUIZ_H_
#define	UIQUIZ_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define item slot
#define QUIZ_QUIZ_WIDTH					216
#define QUIZ_QUIZ_HEIGHT				151
#define QUIZ_TRADE_HEIGHT				75		// 98 - 23 = 75

// Column & Row
#define QUIZ_USER_SLOT_COL				5
#define QUIZ_USER_SLOT_ROW				4
#define QUIZ_USER_SLOT_ROW_TOTAL		20
#define QUIZ_USER_SLOT_MAX				(QUIZ_USER_SLOT_ROW_TOTAL * QUIZ_USER_SLOT_COL)
#define QUIZ_QUIZ_SLOT_COL				5
#define	QUIZ_QUIZ_SLOT_TOTAL			10

// Define text position
#define	QUIZ_TITLE_TEXT_OFFSETX			25
#define	QUIZ_TITLE_TEXT_OFFSETY			5

// Define size of shop
#define QUIZ_MAIN_WIDTH					216
#define	QUIZ_MAIN_HEIGHT				290		// 305 - 23 = 282

// ----------------------------------------------------------------------------
// Name : CUIQuiz
// Desc :
// ----------------------------------------------------------------------------
class CUIQuiz : public CUIWindow
{
protected:		
	int					m_nCurRow;

	int					m_nSelUserItemID;
	int					m_nSelQuizItemID;
	
	int					m_nNumOfItem;
	// Region
	UIRect				m_rcMainTitle;
	UIRect				m_rcTop;				// Quiz
	UIRect				m_rcBottom;				// Trade

	// Background
	UIRectUV			m_rtBackTop;
	UIRectUV			m_rtBackMiddle;
	UIRectUV			m_rtBackBottom;
	UIRectUV			m_rtBlank;

	// Inventory
	UIRectUV			m_rtUserInven;
	UIRectUV			m_rtQuizInven;
	UIRectUV			m_rtSeperatorInven;

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
	CTString			m_strItemInfo[MAX_ITEMINFO_LINE];		// Item information string
	COLOR				m_colItemInfo[MAX_ITEMINFO_LINE];		// Color of item information string
	UIRect				m_rcItemInfo;							// Item information region	

	// Buttons
	CUIButton			m_btnClose;								// Close button of Quiz
	CUIButton			m_btnOK;								// Sell button of Quiz
	CUIButton			m_btnCancel;						// Cancel button of Quiz
	CUIScrollBar		m_sbScrollBar;					// Scroll bar of inventory	

public:
	// Items	
	CUIButtonEx			m_abtnQuizItems[QUIZ_QUIZ_SLOT_TOTAL];				// Trade Slot items
	CUIButtonEx			m_abtnUserItems[QUIZ_USER_SLOT_MAX];				// Quiz Slot items

public:
	CUIQuiz();
	~CUIQuiz();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	OpenQuiz( );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// Clear
	void	ClearItems();
	void	RefreshPlayerItem();
	void	ResetQuiz();	

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

protected:
	// Internal functions
	void	AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo = 0xF2F2F2FF );
	BOOL	GetItemInfo( int nWhichSlot, int &nInfoWidth, int &nInfoHeight,
							int nTradeItem = -1, int inven_idx = -1 );
	void	ShowItemInfo( BOOL bShowInfo, BOOL bRenew = FALSE, int nTradeItem = -1, int inven_idx = -1 );
	void	RenderItems();
	void	QuizToUser( SQUAD llCount );
	void	UserToQuiz( SQUAD llCount );	
	void	PrepareUserItems();	

	// Command functions
	void	AddQuizItem( int nIdx, int nUniIndex, SQUAD llCount );
	void	DelQuizItem( int nIdx, int nUniIndex, SQUAD llCount, int nTradeItemID );	
	void	SendBingo();
};

#endif // UIQUIZ_H_

