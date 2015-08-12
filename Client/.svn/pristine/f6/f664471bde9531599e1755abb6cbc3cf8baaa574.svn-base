// ----------------------------------------------------------------------------
//  File : UIComboBox.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UICOMBOBOX_H_
#define	UICOMBOBOX_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIScrollBar.h>
#include <vector>


#define	CMB_DROPLIST_VERT_GAP		4
#define	CMB_DROPLIST_HORI_GAP		4

class CUICheckButton;

// ----------------------------------------------------------------------------
// Name : CUIComboBox
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUIComboBox : public CUIWindow
{
public:
	CUIComboBox();
	~CUIComboBox();

	CUIBase* Clone();
	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
					int nBtnSX, int nBtnSY, int nBtnWidth, int nBtnHeight,
					int nMaxLine, int nLineHeight, int nSpaceX, int nSpaceY );
	void	CreateScroll( BOOL bRight, int nBtnWidth, int nBtnHeight, int nBarWidthGap, int nBarHeightGap,
							int nBarVertEdgeSize );

	// Render
	void	Render();

	// Selected index
	int		GetCurSel() const { return m_nSelectList; }
	void	SetCurSel( int nCurSelection )
	{
		if( nCurSelection < m_vecString.size() )
			m_nSelectList = nCurSelection;
	}
	// Get string vector
	std::vector<CTString> GetVecString() { return m_vecString; }

	// Strings
	void	AddString( CTString &strString );
	void	ResetStrings();

	// UV of combo box
	void	SetBackUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		//m_rtBackground.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
		if( m_pBackground == NULL )
			m_pBackground = new CUIRectSurface;

		m_pBackground->AddRectSurface(UIRect(0,0,CMB_DROPLIST_HORI_GAP,m_nHeight), UIRectUV(fTx0, fTy0, fTx0+CMB_DROPLIST_HORI_GAP, fTy1, fTexWidth, fTexHeight));
		m_pBackground->AddRectSurface(UIRect(CMB_DROPLIST_HORI_GAP,0,m_nWidth-CMB_DROPLIST_HORI_GAP,m_nHeight), UIRectUV(fTx0+CMB_DROPLIST_HORI_GAP, fTy0, fTx1-CMB_DROPLIST_HORI_GAP, fTy1, fTexWidth, fTexHeight));
		m_pBackground->AddRectSurface(UIRect(m_nWidth-CMB_DROPLIST_HORI_GAP,0,m_nWidth,m_nHeight), UIRectUV(fTx1-CMB_DROPLIST_HORI_GAP, fTy0, fTx1, fTy1, fTexWidth, fTexHeight));
	}
	void	SetUpBtnUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtUp.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetDownBtnUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtDown.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetDropListUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight );
	// UV of scroll abr
	void	SetScrollUpUV( UIBtnState bsState,
							FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_sbScrollBar.SetUpUV( bsState, fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	CopyScrollUpUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_sbScrollBar.CopyUpUV( bsSrcState, bsDstState );
	}
	void	SetScrollBarTopUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_sbScrollBar.SetBarTopUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetScrollBarMiddleUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_sbScrollBar.SetBarMiddleUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetScrollBarBottomUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_sbScrollBar.SetBarBottomUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetScrollDownUV( UIBtnState bsState,
						FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_sbScrollBar.SetDownUV( bsState, fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	CopyScrollDownUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_sbScrollBar.CopyDownUV( bsSrcState, bsDstState );
	}

	int GetMaxLine() { return m_nMaxLine; }

	//wooss 050829
	BOOL IsDropList() {return m_bDropList;}
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void SetDropList(BOOL bDrop);

	void setMaxLine( int nmaxLine ) { m_nMaxLine = nmaxLine; }
	void setLineHeight( int nlineHeight ) { m_nLineHeight = nlineHeight; }
	void setSpace( int nSpaceX, int nSpaceY ) { m_nSpaceX = nSpaceX; m_nSpaceY = nSpaceY; }
// 	void setButton( int nX, int nY, int nW, int nH ) { 
// 		m_rcBtn.Left = nX; m_rcBtn.Top = nY;
// 		m_rcBtn.Right = nW; m_rcBtn.Bottom = nH; }

	void setShowLine(int line)		{ m_nShowLine = line; }
	void setComboButton(UIRect rect, UIRectUV uv0, UIRectUV uv1);
	void setBackGround(UIRect rect, UIRectUV uv);
	void setDropBG(UIRect rect, UIRectUV uv, int unit = 4);
	void setScroll(CUIScrollBar* pScroll);
	void OnUpdateDropList();
	void setAlignTextH(int align)	{ m_eAlignText_h = (eALIGN_H)align; }

	void SetRectSurfaceBack(CUIRectSurface* pRS)	{ m_pBackground = pRS; }
	CUIRectSurface* GetRectSurfaceBack()			{ return m_pBackground;	}

	void SetRectSurfaceDropList(CUIRectSurface* pRS)	{ m_pDropListSurface = pRS; }
	CUIRectSurface* GetRectSurfaceDropList()			{ return m_pDropListSurface; }

	void SetRectSurfaceButton(CUIRectSurface* pRS)	{ m_prsButton = pRS;	}
	CUIRectSurface* GetRectSurfaceButton()			{ return m_prsButton;	}

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);


	///////////////////////////////////////////////////////////////////////////////////
#ifdef UI_TOOL
	void		setComboBtnRect(UIRect rc)	{ m_rcComboButton  = rc;	}
	void		setComboBtnUV0(UIRectUV uv) { m_uvComboButton0 = uv;	}
	void		setComboBtnUV1(UIRectUV uv) { m_uvComboButton1 = uv;	}
	void		setBaseRect(UIRect rc)		{ m_rcComboBase	   = rc;	}
	void		setBaseUV(UIRectUV uv)		{ m_uvComboBase	   = uv;	}

	int			getlineHeight()			 { return m_nLineHeight; }
	void		getSpace(int &x, int &y) { x = m_nSpaceX; y = m_nSpaceY; }
	int			getShowLine()			 { return m_nShowLine; }
	UIRect		getDropArea()			 { return m_rcDropArea; }
	void		getBackGround(UIRect& rc, UIRectUV& uv) { rc = m_rcComboBase; uv = m_uvComboBase; }
	void		getComboButton(UIRect& rc, UIRectUV& uv0, UIRectUV& uv1)
	{
		rc  = m_rcComboButton;
		uv0 = m_uvComboButton0;
		uv1 = m_uvComboButton1;
	}
	void		getDropArea(UIRect& rc, UIRectUV& uv, int& nUint)
	{
		rc		= m_rcDropArea;
		uv		= m_uvDropArea;
		nUint	= m_nDropAreaUnit;
	}
#endif // UI_TOOL
private:
	
	UIRectUV m_uvComboBase;

	UIRect	 m_rcComboButton;
	UIRectUV m_uvComboButton0;
	UIRectUV m_uvComboButton1;

	int		 m_nDropAreaUnit;
	UIRectUV m_uvDropArea;
	int m_nSpaceX, m_nSpaceY;
	eALIGN_H m_eAlignText_h;
	//////////////////////////////////////////////////////////////////////////////////
protected:
	void initialize();
	void OnUpdate( float fElapsedTime );
	void OnRender( CDrawPort* pDraw );	

	BOOL					m_bScrollBarExist;				// If scroll bar is exist or not
	BOOL					m_bShowScrollBar;				// If scroll bar is shown or not
	CUIScrollBar			m_sbScrollBar;					// Scroll bar
	BOOL					m_bDropList;					// If list is drop or not
	int						m_nSelectList;					// Selected list
	int						m_nOverList;					// Over list
	std::vector<CTString>	m_vecString;					// Strings
	int						m_nMaxLine;						// Maximum count of line
	int						m_nLineHeight;					// Height of one line
	int						m_nComboTextSY;					// Position y of text in combo box
	int						m_nTextSX;						// Position x of text in drop list
	int						m_nTextSY;						// Position y of text in drop list
	int						m_nSelectOffsetY;				// Offset y of selection region

	// Region of each part
	UIRect					m_rcDropList;
	UIRect					m_rcBtn;
	UIRect					m_rcComboBase;
	UIRect					m_rcDropArea;					// m_rcDropList 동일한 의미이나, 기존 시스템 유지 때문에 새로 정의함.
	COLOR					m_colorDrop;
	COLOR					m_colorDropSel;

	// UV of each part
	UIRectUV				m_rtUp;							// UV of up button
	UIRectUV				m_rtDown;		             	// UV of down button
	CUIRectSurface*			m_pBackground;
	CUIRectSurface*			m_pDropListSurface;
	CUIRectSurface*			m_prsButton;						// Button 출력
	int						m_nScrollPos;
	int						m_nShowLine;
	CUIScrollBar*			m_pScroll;
};


#endif	// UICOMBOBOX_H_

