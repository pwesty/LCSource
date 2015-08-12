#ifndef	UISINGLE_BATTLE_H_
#define	UISINGLE_BATTLE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Include Files
// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIEditBox.h>

// Define size of SINGLE Battle
#define	SB_WIDTH						118
#define	SB_HEIGHT						61

//------------------------------------------------------------------------------
// CUISingleBattle
// Explain:  길드 전투 
//------------------------------------------------------------------------------
class ENGINE_API CUISingleBattle : public CUIWindow
{
protected:
	UIRect					m_rcBox;						// 시간 표시 창 (배경)
	UIRect					m_rcRedBox;						// RedBox : KillCount
	UIRect					m_rcBlueBox;					// BlueBox : KillCount
		
	UIRectUV				m_rtBox;						
	UIRectUV				m_rtBlueBox;					// BlueBox : KillCount
	UIRectUV				m_rtRedBox;						// RedBox : KillCount
	
	UIRectUV				m_rtSmallNumber[10];			// 시간 표시 용 숫자
	UIRectUV				m_rtLargeNumber[10];			// KillCount 표시 용 숫자 
	UIRectUV				m_rtColon;						// 시간 표시 용 :(콜론)		

	// etc
	CTString				m_strTitle;						// 타이틀 이름 (길드전투 관련 메세지 뿌릴때 사용)	
	
public:

	CUISingleBattle();
	~CUISingleBattle();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Clear();	//
	void	Close();	// 종료

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void	SetFocus( BOOL bVisible );
	void	OpenSingleBattle();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	

	// Render
	void	Render();					// Render	
	void	RenderGBStatus();			// 길드 전투 상태창
	void	DrawNumber( int x, int y, int nNumber, bool bLarge = false );	// 숫자	
	void	DrawColon( int x, int y );	// Colon Render

	void	RenderKillPoint();			// Render Kill Point
};

#endif	// UIGB_H_