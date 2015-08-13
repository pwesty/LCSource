// ----------------------------------------------------------------------------
//  File : UIHelpIcon.h
//  Desc : 
// ----------------------------------------------------------------------------

#ifndef	UIHELPICON_H_
#define	UIHELPICON_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIButton.h>

// Define size of player information
#define	HELPICON_WIDTH				50
#define	HELPICON_HEIGHT				44

// ----------------------------------------------------------------------------
// Name : CUITargetInfo
// Desc :
// ----------------------------------------------------------------------------
class CUIHelpIcon : public CUIWindow
{
protected:
	
	BOOL				m_bShowBack;

	// Region of each part
	UIRect				m_rcTitle;						// Title
	UIRect				m_rcBack;						// Back Region
	UIRect				m_rcMark;						// Mark Region(Title+Back)
	UIRect				m_rcLineV;						// Vertical Line
	

	// UV of each part
	UIRectUV			m_rtTitle;
	UIRectUV			m_rtBack;
	UIRectUV			m_rtHelpIdle;						
	UIRectUV			m_rtHelpClick;						
	UIRectUV			m_rtLineV;						// vertical line	

	// Buttons
	CUIButton			m_btnHelp;						// Help Button			
	
	
public:
	CUIHelpIcon();
	~CUIHelpIcon();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderIcon();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};


#endif	// UIHELPICON_H_

