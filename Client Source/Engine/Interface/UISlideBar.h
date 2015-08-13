// ----------------------------------------------------------------------------
//  File : UISlideBar.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UISLIDEBAR_H_
#define	UISLIDEBAR_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


// #include <Engine/Interface/UIWindow.h>


// ----------------------------------------------------------------------------
// Name : CUISlideBar
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUISlideBar : public CUIWindow
{
protected:
	BOOL			m_bSlideBarClick;				// If slide bar is clicked or not
	int				m_nCurPos;						// Current position of slide bar
	int				m_nRange;						// Sliding range
	int				m_nMinPos;						// Minimum position of slide bar
	int				m_nMaxPos;						// Maximum position of slide bar

	UIRect			m_rcSlide;						// Sliding region
	UIRect			m_rcBar;						// Region of bar

	UIRectUV		m_rtBackground;					// UV of background
	UIRectUV		m_rtBar;						// UV of bar

protected:
	// Internal functions
	void	AdjustSlidePos( int nX );

public:
	CUISlideBar();
	~CUISlideBar();

	CUIBase* Clone();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
					int nBarWidth, int nBarHeight,
					int nCurPos, int nRange, int nMinPos, int nMaxPos );

	// Render
	void	Render();

	// Set properties
	void	SetCurPos( int nCurPos );
	void	SetRange( int nRange );
	void	SetMinPos( int nMinPos );
	void	SetMaxPos( int nMaxPos );
	int		GetCurPos() const { return m_nCurPos; }
	int		GetRange() const { return m_nRange; }
	int		GetMinPos() const { return m_nMinPos; }
	int		GetMaxPos() const { return m_nMaxPos; }

	void		setBar(int nWidth, int nHeight);
	int			getBarWidth() { return m_rcBar.GetWidth(); }
	int			getBarHeight() { return m_rcBar.GetHeight(); }
	UIRectUV	getBackUV();
	UIRectUV	getBarUV();

	// UV
	void	SetBackgroundUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtBackground.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetBarUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtBar.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

protected:
	void OnRender(CDrawPort* pDraw);
};


#endif	// UISLIDEBAR_H_

