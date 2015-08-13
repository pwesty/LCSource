// ----------------------------------------------------------------------------
//	File : UIOXQuizEvent.h
//	Desc : Created by eons
// ----------------------------------------------------------------------------

#ifndef UIOXQUIZEVENT_H_
#define UIOXQUIZEVENT_H_
#ifdef PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIScrollBar.h>

#define MAX_QUIZ_DESC				50
// Define position of text
#define	QUIZ_STRING_OFFSETX			23
#define	QUIZ_STRING_OFFSTY			7

#define	UI_OX_WIDTH					216
#define	UI_OX_HEIGHT				200

// ----------------------------------------------------------------------------
//	Name : CUIOXQuizEvent
//	Desc :
// ----------------------------------------------------------------------------
class CUIOXQuizEvent : public CUIWindow
{
protected:
	CTString	m_strQuizString[MAX_QUIZ_DESC];		// String of Quiz contents
	COLOR		m_colQuizString[MAX_QUIZ_DESC];		// Color of Quiz contents

	int			m_nQuizNum;							// Current Number of Quiz
	int			m_nCorrect;							// Correct Answer Number
	int			m_nMaxCharCount;
	int			m_nChatLineHeight;
	int			m_nCurQuizInsert;
	int			m_nCurQuizCount;

	double		m_dOldTime;
	double		m_dCurTime;

	BOOL		m_bTitleBarClick;						// If title bar is clicked or not
	BOOL		m_bQuizStart;

	// Region of each part
	UIRect		m_rcTitle;							// Region of title bar

	// UV of each part
	UIRectUV	m_rtTopL;								// UV of top background
	UIRectUV	m_rtTopM;								// UV of top background
	UIRectUV	m_rtTopR;								// UV of top background
	UIRectUV	m_rtMiddleScrollL;						// UV of middle background with scroll bar
	UIRectUV	m_rtMiddleScrollM;						// UV of middle background with scroll bar
	UIRectUV	m_rtMiddleScrollR;						// UV of middle background with scroll bar
	UIRectUV	m_rtMiddleL;							// UV of middle background
	UIRectUV	m_rtMiddleM;							// UV of middle background
	UIRectUV	m_rtMiddleR;							// UV of middle background
	UIRectUV	m_rtMiddleGapL;							// UV of middle gap background
	UIRectUV	m_rtMiddleGapM;							// UV of middle gap background
	UIRectUV	m_rtMiddleGapR;							// UV of middle gap background
	UIRectUV	m_rtBottomL;							// UV of bottom background
	UIRectUV	m_rtBottomM;							// UV of bottom background
	UIRectUV	m_rtBottomR;							// UV of bottom background

protected:
	void	AddQuizDescString( CTString &strQuizString, COLOR colQuizColor );
	void	RenderQuizStringList( void );
	void	ResetQuizData( void );
public:
	CUIOXQuizEvent();
	~CUIOXQuizEvent();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	WMSG_RESULT	MouseMessage( MSG *pMsg );
	
	void	ReceiveQuiz( int nQuizNo, int limitsec, CTString QuizStr );
	BOOL	IsQuizStart( void ) { return m_bQuizStart; }
};

#endif