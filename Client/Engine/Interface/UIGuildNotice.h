// ----------------------------------------------------------------------------
//  File : UIGuildNotice.h
//  Desc : 
// ----------------------------------------------------------------------------

#ifndef	UIGUILDNOTICE_H_
#define	UIGUILDNOTICE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#define	GUILD_NOTICE_WIDTH		212
#define GUILD_NOTICE_HEIGHT		257

// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIDrawFigure.h>
// #include <Engine/Interface/UIListBox.h>

// ----------------------------------------------------------------------------
// Name : CUIGuildNotice
// Desc :
// ----------------------------------------------------------------------------
class CUIGuildNotice : public CUIWindow
{
protected:
	
	// 공지 출력 
	CTString				m_strGuildName;						// Guild Name
	UIRect					m_rcTitle;							// Region of title bar
	
	CUIDrawBox				m_bxBackGroundBox;					
	CUIDrawBox				m_bxBackGroundBox2;		
	CUIDrawBox				m_bxTopNotic;						// UV of Top
	CUIDrawBox				m_bxBottomNotice;					// UV of Bottom 
	CUIListBox				m_lbGuildNoticeStr;					// Notice String
	CUIButton				m_btnCloseNotice;					// 닫기 버튼		
	
	
public:
	CUIGuildNotice();
	~CUIGuildNotice();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();	

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Set Notice String
	void	SetGuildNotice( CTString guildName,CTString noticeTitle,CTString guildNotice);

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};


#endif	// UIHELPICON_H_
