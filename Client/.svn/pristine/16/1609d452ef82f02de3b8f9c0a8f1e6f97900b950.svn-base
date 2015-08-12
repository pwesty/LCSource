// ----------------------------------------------------------------------------
//  File : UIRefine.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIREFINE_H_
#define	UIREFINE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define max char and line of strings
#define	MAX_REFINE_STRING				4


// Define text position
#define	REFINE_TITLE_TEXT_OFFSETX		25
#define	REFINE_TITLE_TEXT_OFFSETY		5
#define	REFINE_DESC_TEXT_SX				15
#define	REFINE_DESC_TEXT_SY				33


// Define size of refine
#define	REFINE_TOP_HEIGHT				26
#define	REFINE_BOTTOM_HEIGHT			7
#define	REFINE_WIDTH					216
#define	REFINE_HEIGHT					190


// ----------------------------------------------------------------------------
// Name : CUIRefine
// Desc :
// ----------------------------------------------------------------------------
class CUIRefine : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnClose;								// Close button
	CUIButton			m_btnOK;								// OK button
	CUIButton			m_btnCancel;							// Cancel button
	CUIIcon*			m_pIconSlotItem;							// Slot item button
	BOOL				m_bWaitRefineResult;					// If UI wait result from server or not

	// Strings
	int					m_nStringCount;							// Count of string
	CTString			m_strRefineDesc[MAX_REFINE_STRING];		// Strings of refine description
	CTString			m_strRefineMoney;						// String of refine money
	SQUAD				m_llRefineMoney;						// Refine money
	int					m_nMoneyPosY;							// Position y of refine money text
	int					m_nTextRegionHeight;					// Height of text region

	// Position of target npc
	FLOAT				m_fNpcX, m_fNpcZ;

	// Region of each part
	UIRect				m_rcTitle;								// Region of title
	UIRect				m_rcItemSlot;							// Region of item slot
	UIRect				m_rcInsertItem;							// Region of inserting item

	// UV of each part
	UIRectUV			m_rtTop;								// UV of top background
	UIRectUV			m_rtMiddle1;							// UV of middle background
	UIRectUV			m_rtMiddle2;							// UV of middle background
	UIRectUV			m_rtBottom;								// UV of bottom background
	UIRectUV			m_rtItemSlot;							// UV of item slot

protected:
	// Internal functions
	void	AddString( CTString &strDesc );

	// Command functions
	void	SetRefineItem();

	// Network message functions ( send )
	void	SendRefineReq();

public:
	CUIRefine();
	~CUIRefine();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Open & close refine
	ENGINE_API	void	OpenRefine(int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	void	CloseRefine();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Network message functions ( receive )
	void	RefineRep( SBYTE sbResult );

	// Command functions
	void	MsgBoxLCommand( int nCommandCode, int nResult );
};


#endif	// UIREFINE_H_

