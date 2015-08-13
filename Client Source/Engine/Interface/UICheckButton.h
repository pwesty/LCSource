// ----------------------------------------------------------------------------
//  File : UICheckButton.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UICHECKBUTTON_H_
#define	UICHECKBUTTON_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>


// UI check button state
enum UICheckBtnState
{
	UCBS_NONE			= 0,
	UCBS_CHECK			= 1,
	UCBS_NONE_DISABLE	= 2,
	UCBS_CHECK_DISABLE	= 3,
	UCBS_TOTAL			= 4,
};


// ----------------------------------------------------------------------------
// Name : CUICheckButton
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUICheckButton : public CUIWindow
{
protected:
	BOOL			m_bChecked;						// If button is checked
	CTString		m_strText;						// String of button text
	BOOL			m_bValidText;					// If text is valid or not
	COLOR			m_colText[2];					// Color of button text
	int				m_nTextSX, m_nTextSY;			// Position of text
	UIRect			m_rcCheckRegion;				// Checking region
	UIRectUV		m_rtUV[UCBS_TOTAL];				// UV of each button stste

public:
	CUICheckButton();

	CUIBase* Clone();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
					CTString &strText, BOOL bCheckRegionLeft = TRUE, int nTextSX = 0, int nCheckRegion = 0 );

	// Render
	void	Render();
	void	OnRender(CDrawPort* pDraw);
	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );
	void	SetHighlight(bool bHighlight, COLOR colHighlight = 0xFFFFFFFF)	
	{
		m_colHighlight = colHighlight;
		m_bHighlightOn = bHighlight;
	}
	void	SetHighlightTime(float fTime)	
	{
		m_fHighlightTime = fTime;	
		m_bHighlightOn = false;
	}

	// Check state
	void	SetCheck( BOOL bCheck ) { m_bChecked = bCheck; }
	BOOL	IsChecked() const { return m_bChecked; }

	// Text
	void	SetText( CTString &strText );
	// Color
	void	SetTextColor( BOOL bEnableState, COLOR colText )
	{
		bEnableState ? m_colText[1] = colText : m_colText[0] = colText;
	}
	void	GetTextColor( COLOR& colOn, COLOR& colOff ) 
	{
		colOn = m_colText[1]; colOff = m_colText[0];
	}
	// UV
	void	SetUV( UICheckBtnState bsState,
					FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtUV[bsState].SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetUV( UICheckBtnState bsState, UIRectUV &rt)
	{
		m_rtUV[bsState].CopyUV(rt);
	}
	void	SetUVTex(UICheckBtnState bsState, UIRectUV &rt)
	{
		if( m_pTexData != NULL )
		{
			FLOAT fW = m_pTexData->GetPixWidth();
			FLOAT fH = m_pTexData->GetPixHeight();

			rt.U0 /= fW;		rt.V0 /= fH;
			rt.U1 /= fW;		rt.V1 /= fH;
		}
		SetUV(bsState, rt);
	}
	void	CopyUV( UICheckBtnState bsSrcState, UICheckBtnState bsDstState )
	{
		m_rtUV[bsDstState].CopyUV( m_rtUV[bsSrcState] );
	}

	// 기본 정보 세팅 후에 호출해야 한다.
	void	SetCheckRegion( BOOL bLeft, int nTextSX = 0, int nCheckRegion = 0 );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void	setTextArea(int nArea)		{ m_nTextArea = nArea;	}
	void	setAlignTextH(eALIGN_H align)	{ m_eAlignText_h = align;	}
	int		getTextArea()				{ return m_nTextArea;	}
	int		getAlignTextH()					{ return m_eAlignText_h;	}
	void	setEdge(bool bEdge)			{ m_bTextEdge = bEdge;	}
	bool	getEdge()					{ return m_bTextEdge;	}

	virtual WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);

public:
#ifdef UI_TOOL
	BOOL		GetLeft() { return m_bLeft; }
	int			GetCheckRegion() { return m_nCheckRegion; }
	int			GetTextSX() { return m_nTempTextSX; }
	UIRectUV	GetUV( UICheckBtnState btnState );

	INDEX		getStringIndex() { return m_stringIndex; }
	const char* getText() { return m_strText.str_String; }

	void		setStringIndex(INDEX iIdx)	{ m_stringIndex = iIdx; }
#endif //UI_TOOL
public :
	BOOL m_bLeft;
	int	 m_nCheckRegion;
	int	 m_nTempTextSX;
	eALIGN_H	m_eAlignText_h;
	int		m_nTextArea;
	bool	m_bTextEdge;
	COLOR	m_colHighlight;
	float	m_fHighlightTime;
	ULONG	m_ulOldTime;
	bool	m_bHighlightOn;

#ifdef UI_TOOL
	INDEX		m_stringIndex;
#endif // UI_TOOL

};


#endif	// UICHECKBUTTON_H_

