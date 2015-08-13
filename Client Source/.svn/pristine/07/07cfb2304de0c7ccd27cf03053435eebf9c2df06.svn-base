// ----------------------------------------------------------------------------
//  File : UIWindow.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIWINDOW_H_
#define	UIWINDOW_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#ifndef	WM_MOUSEWHEEL
	#define	WM_MOUSEWHEEL	0x020A
#endif


#ifndef	WHEEL_DELTA
	#define	WHEEL_DELTA		120
#endif

//#include <Engine/Graphics/DrawPort.h>
#include "UIBase.h"

// ----------------------------------------------------------------------------
// Name : CUIWindow
// Desc :
// ----------------------------------------------------------------------------
class CUIWindow : public CUIBase
{
protected:
	//CUIWindow		*m_pParentWnd;				// Pointer of parent window
	DWORD			m_dwWndState;				// State flags of window
	//int				m_nPosX, m_nPosY;			// Position of window
	//int				m_nWidth, m_nHeight;		// Size of window
	CTextureData	*m_ptdBaseTexture;			// Texture of window
    CTString        m_strXMLFileName;	

public:

    CUIWindow(const char* pcXMLFileName=NULL) 
		: /*m_pParentWnd( NULL ),*/ m_ptdBaseTexture( NULL )
		, m_dwWndState( UWS_ENABLE | UWS_VISIBLE )
    {
        if (pcXMLFileName)
            m_strXMLFileName = pcXMLFileName;
        else
            m_strXMLFileName.Clear();

		setType(eUI_CONTROL_WINDOW);
	}
	virtual ~CUIWindow() { Destroy(); }

	// Create & destroy window
	virtual void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
	{
		m_pParent = pParentWnd;
		SetPos( nX, nY );
		SetSize( nWidth, nHeight );
	}

	void Destroy()
	{
		CUIBase::Destroy();
		STOCK_RELEASE(m_ptdBaseTexture);
	}

	// Render window
	virtual void	PostRender() {}

	// Leave

	virtual void	FocusLeave() {}

	virtual void	OnRender(CDrawPort* pDraw)
	{
#ifdef UI_TOOL
		if(m_bSelect == FALSE)
			return;
		RenderBorder( pDraw );
#endif //UI_TOOL
	}
	

	// Window state
	void	SetWndState( DWORD dwWndState ) { m_dwWndState = dwWndState; }
	virtual void	SetEnable( BOOL bEnable ) { bEnable ? m_dwWndState |= UWS_ENABLE : m_dwWndState &= ~UWS_ENABLE; }
	void	SetVisible( BOOL bVisible ) { bVisible ? m_dwWndState |= UWS_VISIBLE : m_dwWndState &= ~UWS_VISIBLE; }
	virtual void	SetFocus( BOOL bVisible ) { bVisible ? m_dwWndState |= UWS_FOCUS : m_dwWndState &= ~UWS_FOCUS; }
	void	SetFloat( BOOL bFloat ) { bFloat ? m_dwWndState |= UWS_FLOAT : m_dwWndState &= ~UWS_FLOAT; }
	DWORD	GetWndState() const { return m_dwWndState; }
	BOOL	IsEnabled() const { return m_dwWndState & UWS_ENABLE; }
	BOOL	IsVisible() const { return m_dwWndState & UWS_VISIBLE; }
	BOOL	IsFocused() const { return m_dwWndState & UWS_FOCUS; }
	BOOL	IsFloating() const { return m_dwWndState & UWS_FLOAT; }
	virtual BOOL	IsEditBoxFocused() { return FALSE; }
	virtual void	KillFocusEditBox() {}

	

	// Adjust position
	virtual void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ ) {}
	virtual void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ ) {}
	virtual void	ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ ) {} // 유저 정보가 저장되는 UI 리셋용 함수

	// Window size
	void	SetSize( int nWidth, int nHeight ) { m_nWidth = nWidth; m_nHeight = nHeight; }
	void	SetWidth( int nWidth ) { m_nWidth = nWidth; }
	void	SetHeight( int nHeight ) { m_nHeight = nHeight; }

	// Region check
	
	// Rectangle region
	BOOL	IsInsideRect( int nX, int nY, const UIRect& rcRect )
	{
		ConvertToWindow( nX, nY );

		if( nX >= rcRect.Left && nY >= rcRect.Top && nX < rcRect.Right && nY < rcRect.Bottom )
			return TRUE;
		return FALSE;
	}

	// Command functions
	virtual void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput ) {}
	virtual void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput ,CTString &strConfirm) {}
	virtual void	MsgBoxLCommand( int nCommandCode, int nResult ) {}
	virtual void	MsgCommonCommand( int nCommandCode ) {} // 커먼 메시지 커맨드 (타이머 등의 커맨드 호출에 사용하기 위해 만듬) 처리할 내용은 각자의 UI에서 모두 처리한다
	
	// Close Window by Escape Key Event.
	virtual BOOL	CloseWindowByEsc() {	return FALSE;	}

	void Hide( BOOL bHide ) {
		if (bHide == TRUE)
			CloseProc();

		CUIBase::Hide(bHide);
	}
};


#endif	// UIWINDOW_H_