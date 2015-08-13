// ----------------------------------------------------------------------------
//  File : UISystemMenu.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UISYSTEMMENU_H_
#define	UISYSTEMMENU_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


// #include <Engine/Interface/UIButton.h>


// Define text position
#define	SYSMENU_TITLE_TEXT_OFFSETX	25
#define	SYSMENU_TITLE_TEXT_OFFSETY	5


// Define size of system menu
//#define	SYSTEMMENU_WIDTH			216
//#define	SYSTEMMENU_HEIGHT			114
#define	SYSTEMMENU_WIDTH			178
#define	SYSTEMMENU_HEIGHT			191


// ----------------------------------------------------------------------------
// Name : CUISystemMenu
// Desc :
// ----------------------------------------------------------------------------
class CUISystemMenu : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnClose;								// Close button
	CUIButton			m_btnOption;							// Option button
	CUIButton			m_btnHelp;								// Help button
	CUIButton			m_btnRestart;							// Restart button
	CUIButton			m_btnExit;								// Exit button

	// Region of each part
	UIRect				m_rcTitle;								// Region of title

	// UV of each part
	UIRectUV			m_rtBackground;							// UV of background

	CTextureData		*m_ptdButtonTexture;		// Texture of Button
	
	BOOL				m_bCharacterMove;
	__int64				m_llStartTime;							// restart start time attack


protected:
	// Command functions
	void	OpenOption();
	void	OpenHelp();
	void	Restart();
	void	Exit();

public:
	CUISystemMenu();
	~CUISystemMenu();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Toggle visible
	void	ToggleVisible();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	void	ResetRestartTime(void)				{ m_llStartTime = 0;		}	
	__int64	GetRestartTime()					{ return m_llStartTime;		}
	BOOL	GetMoveCharSel()					{ return m_bCharacterMove;	}

	ENGINE_API void	Restart_Internal();
	ENGINE_API void	CancelRestart();
	void	ReStartNow();
	bool	IsHaveRelic(); // 유물 아이템을 가지고 있는지 검사
	void	LogoutRelicWarring(); // 유물 아이템을 가지고 있다면 경고
	
};


#endif	// UISYSTEMMENU_H_

