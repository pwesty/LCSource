// ----------------------------------------------------------------------------
//  File : UIMessageBoxL.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIMESSAGEBOXL_H_
#define	UIMESSAGEBOXL_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIListBox.h>
#include <vector>


// Define max line of strings
#define	MAX_SELMSGSTRING				4


// Define text position
#define	MSGBOXL_TITLE_TEXT_OFFSETX		25 
#define	MSGBOXL_TITLE_TEXT_OFFSETY		5
#define	MSGBOXL_SEL_TEXT_SX				18
#define	MSGBOXL_SEL_TEXT_SY				200


// Define size of message box
#define	MSGBOXL_TOP_HEIGHT				26
#define	MSGBOXL_DESC_HEIGHT				164
#define	MSGBOXL_GAP_HEIGHT				3
#define	MSGBOXL_BOTTOM_HEIGHT			7
#define MSGBOXL_WIDTH					311
#define	MSGBOXL_HEIGHT					239


// ----------------------------------------------------------------------------
// Name : CUIMessageBoxL
// Desc :
// ----------------------------------------------------------------------------
class CUIMessageBoxL : public CUIWindow
{
protected:

	typedef struct tagSelTable
	{
		int			iIndex;		// 인덱스
		int			iValue;		// 값.
	}sSelTable;

	int						m_nUIIndex;								// Index of message box
	CUIButton				m_btnClose;								// Close button
	CUIListBox				m_lbDescription;						// List box of description
	CTString				m_strTitle;								// Title of message box
	std::vector<CTString>	m_vecStrSelMessage;						// Selection message strings of message box
	std::vector<COLOR>		m_vecColSelMessage;						// Color of selection message strings of message box
	std::vector<sSelTable>	m_vecSelIndex;							// Indices of selection message strings
	int						m_nSelStringCount;						// Current count of selection message strings
	int						m_nOverSelection;						// Over selection
	int						m_nWhichUI;								// Which UI does create this message box
	int						m_nCommandCode;							// Command code for returning to UI
	BOOL					m_bTitleBarClick;						// If title bar is clicked or not

	// Region of each part
	UIRect					m_rcTitle;								// Region of title bar
	UIRect					m_rcSelection;							// Region of selection
	int						m_nSelHeight;							// Height of selection region

	// UV of each part
	UIRectUV				m_rtTopL;								// UV of top background
	UIRectUV				m_rtTopM;								// UV of top background
	UIRectUV				m_rtTopR;								// UV of top background
	UIRectUV				m_rtMiddleScrollL;						// UV of middle background with scroll bar
	UIRectUV				m_rtMiddleScrollM;						// UV of middle background with scroll bar
	UIRectUV				m_rtMiddleScrollR;						// UV of middle background with scroll bar
	UIRectUV				m_rtMiddleL;							// UV of middle background
	UIRectUV				m_rtMiddleM;							// UV of middle background
	UIRectUV				m_rtMiddleR;							// UV of middle background
	UIRectUV				m_rtMiddleGapL;							// UV of middle gap background
	UIRectUV				m_rtMiddleGapM;							// UV of middle gap background
	UIRectUV				m_rtMiddleGapR;							// UV of middle gap background
	UIRectUV				m_rtBottomL;							// UV of bottom background
	UIRectUV				m_rtBottomM;							// UV of bottom background
	UIRectUV				m_rtBottomR;							// UV of bottom background

	int						m_nCurNum;

	__int64					m_nStartTime;							// [sora] 시작 시간
	__int64					m_nTime;								// [sora] 대기 시간
	int						m_nTimeOutBtnMessage;					// [sora] 실행 메시지

protected:
	// Internal functions
	void	ReturnCommand( int nResult );

public:
	CUIMessageBoxL();
	~CUIMessageBoxL();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	CreateMessageBox( CTString &strTitle, int nWhichUI, int nCommandCode, int nPosX, int nPosY );
	void	AddDescString( CTString &strMessage, const COLOR colDesc );
	void	AddSelString( CTString &strMessage, const COLOR colSel, int iValue = -1 );
	void	InitMessageBox();
	void	SetUIIndex( int nUIIndex ) { m_nUIIndex = nUIIndex; }

	// Get data of message box
	int		GetCmdCode() const { return m_nCommandCode; }
	int&	GetCurNum() { return m_nCurNum; }

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	// Key Messages.
	WMSG_RESULT KeyMessage(MSG *pMsg );

	void	SetMsgBoxLTimer(__int64 nTime, int nMessage)	// [sora] 동작 시간 설정
	{
		m_nStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

		m_nTime = nTime;
		m_nTimeOutBtnMessage = nMessage;
	}
};


#endif	// UIMESSAGEBOXL_H_

