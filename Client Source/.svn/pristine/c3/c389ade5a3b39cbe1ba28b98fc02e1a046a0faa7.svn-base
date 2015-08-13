// ----------------------------------------------------------------------------
//  File : UIRemission.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIREMISSION_H_
#define	UIREMISSION_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIButtonEx.h>

#define	REMISSION_TAB_WIDTH					96

// Define remission slot
#define	REMISSION_SLOT_SX					18
#define	REMISSION_SLOT_SY					56
#define	REMISSION_SLOT_OFFSETY				37
#define	REMISSION_SLOT_ROW					4
#define	REMISSION_SLOT_ROW_TOTAL			30

// Define text
#define	REMISSION_NAME_CX					122
#define	REMISSION_NAME_SY					58
#define	REMISSION_NEED_RX					184
#define	REMISSION_CURSP_SX					52
#define	REMISSION_CURSP_RX					199
#define	REMISSION_CURSP_SY					343
#define	REMISSION_TAB_CX					108
#define	REMISSION_TAB_SY					34
#define	REMISSION_DESC_CHAR_WIDTH			170

// Define text position
#define	REMISSION_TITLE_TEXT_OFFSETX		25
#define	REMISSION_TITLE_TEXT_OFFSETY		5


// Define size of remission learn
#define	REMISSION_WIDTH						216
#define	REMISSION_HEIGHT					400


// ----------------------------------------------------------------------------
// Name : CUIRemission
// Desc :
// ----------------------------------------------------------------------------
class CUIRemission : public CUIWindow
{
protected:
	enum eRemissionType
	{
		REMISSION_NONE	= 0,
		REMISSION_HP	= 1,	// HP ∏È¡À
		REMISSION_MP	= 2,	// MP ∏È¡À
		REMISSION_ITEM	= 3,	// æ∆¿Ã≈€ ∏È¡À
	};

	// Controls
	CUIButton				m_btnClose;								// Close button
	CUIButton				m_btnOK;								// Learn button
	CUIButton				m_btnCancel;							// Cancel button
	CUIScrollBar			m_sbRemissionIcon;						// Scrollbar of special remission icon
	CUIListBox				m_lbRemissionDesc;						// List box of remission description

	// Remission buttons
	std::vector<CUIButtonEx>	m_vectorbtnRemissions;				// Buttons of special remission

	// Remission information
	int						m_nSelRemissionID;						// Selected special remission ID
	CTString				m_strShortDesc;							// Short remission information string

	// Position of target npc
	FLOAT					m_fNpcX, m_fNpcZ;

	// Region of each part
	UIRect					m_rcTitle;								// Region of title bar
	UIRect					m_rcIcons;								// Region of icons

	// UV of each part
	UIRectUV				m_rtBackground;							// UV of background
	UIRectUV				m_rtSelOutline;							// UV of outline of selected button
	UIRectUV				m_rtTabLine;							// UV of tab seperator

protected:
	// Internal functions
	void	RenderRemissionBtns();
	void	GetRemissionDesc( int iRemissionType, int nIndex = -1, SWORD nIdx = -1);
	void	AddRemissionDescString( CTString &strDesc, const COLOR colDesc );
	SQUAD	CalculatePrice(int iRemissionType, SWORD nIdx);
	void	PressOK( );

	// Network message functions ( send )
	void	SendRemission();
	
public:
	CUIRemission();
	~CUIRemission();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open remission learn
	ENGINE_API void	OpenRemission( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Network message functions ( receive )
	void	RemissionError( UBYTE ubError );

	// Init & Close
	BOOL	InitRemission( );
	void	CloseRemission();
};


#endif	// UIREMISSION_H_

