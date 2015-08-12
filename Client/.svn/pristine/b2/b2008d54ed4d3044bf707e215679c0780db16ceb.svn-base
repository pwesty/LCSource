// ----------------------------------------------------------------------------
//  File : UIButton.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIBUTTON_H_
#define	UIBUTTON_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>

class CUIBase;

// UI button state
enum UIBtnState
{
	UBS_IDLE	= 0,
	UBS_ON,
	UBS_CLICK,
	UBS_DISABLE,
	UBS_TOTAL,
};


// ----------------------------------------------------------------------------
// Name : CUIButton
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUIButton : public CUIWindow
{
protected:

	BOOL			m_bLButtonDown;					// If left button is down or not
	BOOL			m_bMouseOver;
	BOOL			m_bUseNewType;					// Using for "CUIRectSurface" class
	CTString		m_strText;						// String of button text
	BOOL			m_bValidText;					// If text is valid or not
	BOOL			m_bOnlyText;					// If button has only text
	BOOL			m_bVerticality;
	COLOR			m_colText[UBS_TOTAL];			// Color of button text
	UIBtnState		m_bsState;						// State of UI button
	UIRectUV		m_rtUV[UBS_TOTAL];				// UV of each button stste
	CUIRectSurface	m_rtSurface[UBS_TOTAL];
	BOOL			m_bAutoInfo;
	bool			m_bEdge;
	DWORD			m_dwCurTime;

public:
	CUIButton();
	~CUIButton();

	CUIBase* Clone();

	// Create
	void	Create( CUIWindow *pParentWnd, CTString &strText, int nX, int nY, int nWidth, int nHeight );

	// Render
	// [090709: selo] Render() 함수에 텍스쳐 색상을 지정할 수 있게 함
	void	Render( COLOR textureColor = 0xFFFFFFFF );
	void	OnRender( CDrawPort* pDraw );
	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );
	void	RenderHighlight( const COLOR colHighlight);

	// Text
	int		SetText( CTString &strText, BOOL bVerticality = FALSE, BOOL bOnlyText = FALSE );
	inline CTString	GetText() {return m_strText;}	// WSS_NEW_GUILD_SYSTEM 070721 

	// Color
	void	SetTextColor( UIBtnState bsState, COLOR colText ) { m_colText[bsState] = colText; }
	COLOR	GetTextColor( UIBtnState bsState )	{ return m_colText[bsState]; }

	// Window state
	void	SetEnable( BOOL bEnable )
	{
		if( bEnable )
		{
			if ((m_dwWndState & UWS_ENABLE) == false)
				SetBtnState( UBS_IDLE );

			m_dwWndState |= UWS_ENABLE;			
		}
		else
		{
			if (m_dwWndState & UWS_ENABLE)
				SetBtnState( UBS_DISABLE );

			m_dwWndState &= ~UWS_ENABLE;			
		}
	}

	// Button state
	void	SetBtnState( UIBtnState bsState ) { m_bsState = bsState; }
	UIBtnState	GetBtnState() const { return m_bsState; }

	// UV
	void	SetUV( UIBtnState bsState,
					FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtUV[bsState].SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetUV( UIBtnState bsState, UIRectUV uv );
	void	CopyUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_rtUV[bsDstState].CopyUV( m_rtUV[bsSrcState] );
	}
	void	CopyUV( UIBtnState bsState, UIRectUV &rtUV )
	{
		m_rtUV[bsState].CopyUV( rtUV );
	}

	void	SetRTSurface(UIBtnState bsState, UIRect rt, UIRectUV uv)
	{
		m_rtSurface[bsState].AddRectSurface(rt, uv);
	}

	void	SetRTSurfaceEx(UIBtnState bsState, UIRect rt, UIRectUV uv);

	void	CopyRTSurface(UIBtnState Src, UIBtnState Dest)
	{
		m_rtSurface[Dest] = m_rtSurface[Src];
	}

	void	CopyRTSurface(UIBtnState Src, CUIRectSurface& rtSurface)
	{
		m_rtSurface[Src] = rtSurface;
	}

	UIRectUV GetUV( UIBtnState bsState );
	CUIRectSurface	&GetRectSurface(UIBtnState bsState) { return m_rtSurface[bsState]; }

	void	SetNewType(BOOL bTrue)
	{
		m_bUseNewType = bTrue;
	}
	BOOL	GetNewType() { return m_bUseNewType; }

	void	SetMouseOver(BOOL bOver) { m_bMouseOver = bOver;}
	BOOL	GetMouseOver(void) const  { return m_bMouseOver; }

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void SetAutoInfo( BOOL bAutoInfo )	{ m_bAutoInfo = bAutoInfo; }
	BOOL GetAutoInfo()	{ return m_bAutoInfo; }

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	void OnLeave(UINT16 x, UINT16 y);
	void OnEnter(UINT16 x, UINT16 y);

	void SetEdge(bool bEdge)	{ m_bEdge = bEdge;	}
	bool GetEdge()				{ return m_bEdge;	}

#ifdef UI_TOOL
public:
	void	setStringIdx(int idx) { m_strIndex = idx; }
	INDEX		getStringIdx() { return m_strIndex; }
private:
	INDEX			m_strIndex;
#endif // UI_TOOL
};


#endif	// UIBUTTON_H_

