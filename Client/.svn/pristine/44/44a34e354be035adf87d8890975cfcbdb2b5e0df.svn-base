// ----------------------------------------------------------------------------
//  File : UIScrollBar.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UISCROLLBAR_H_
#define	UISCROLLBAR_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <Engine/Interface/UIButton.h>

enum eBAR_POS { 
	ePOS_TOP = 0, 
	ePOS_MID, 
	ePOS_BOTTOM , 
	ePOS_MAX 
};

enum eSCROLL_BUTTON {
	eSCROLL_BUTTON_UP, 
	eSCROLL_BUTTON_DOWN, 
	eSCROLL_BUTTON_MAX 
};

// ----------------------------------------------------------------------------
// Name : CUIScrollBar
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUIScrollBar : public CUIWindow
{
public:
	CUIScrollBar();
	~CUIScrollBar();

	CUIBase* Clone();

	// Create buttons
	void	CreateButtons( BOOL bVerticalScroll, int nBtnWidth, int nBtnHeight,
							int nBarWidthGap, int nBarHeightGap, int nBarVertEdgeSize );

	// Render
	void	Render();

	// Seet wheel region
	void	SetWheelRect( int nLeft, int nTop, int nWidth, int nHeight )
	{
		m_rcWheel.SetRect( nLeft, nTop, nLeft + nWidth, nTop + nHeight );
	}

	// UV
	void	SetUpUV( UIBtnState bsState,
						FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_btnUp.SetUV( bsState, fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	CopyUpUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_btnUp.CopyUV( bsSrcState, bsDstState );
	}
	void	SetBarTopUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtBarTop.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetBarMiddleUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rcBarMiddle.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetBarBottomUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtBarBottom.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	SetDownUV( UIBtnState bsState,
						FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_btnDown.SetUV( bsState, fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	CopyDownUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_btnDown.CopyUV( bsSrcState, bsDstState );
	}

	// Size
	void	ChangeSize( int ndSize );

	// 신규 UI전용.
	void	UpdateSize( int nSize, int nItemPerPage );
	void	SetScrollCurPos(int nPos);
	// Scroll information
	void	SetScrollPos( int nScrollPos )
	{
		m_nScrollPos = nScrollPos;
		UpdateScrollBarPos();
	}
	void	ChangeScrollPos( int ndPos )
	{
		MoveScrollPos( ndPos );
	}
	void	SetScrollRange( int nScrollRange )
	{
		m_nScrollRange = nScrollRange;
		UpdateScrollBarSize();
	}
	void	SetCurItemCount( int nCount )
	{
		m_nCurItemCount = nCount;
		UpdateScrollBarSize();
		updateThumbSize();			// 새로운 UI 전용
	}
	void	SetItemsPerPage( int nCount )
	{
		m_nItemsPerPage = nCount;
		if( m_nScrollRange < m_nItemsPerPage )
			m_nScrollRange = m_nItemsPerPage;

		UpdateScrollBarSize();
		updateThumbSize();			// 새로운 UI 전용
	}
	void	ChangeItemsPerPage( int ndCount )
	{
		m_nItemsPerPage += ndCount;
		UpdateScrollBarSize();
	}

	int		GetScrollPos() const { return m_nScrollPos; }
	int		GetScrollRange() const { return m_nScrollRange; }
	int		GetItemsPerPage() const { return m_nItemsPerPage; }
	int		GetCurItemCount() const { return m_nCurItemCount; }

	// Message
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void initialize();

	void OnUpdate( float fElapsedTime );
	void OnRender( CDrawPort* pDraw );
	
	UIRectUV getThumbUV( int nPos )					{ return m_rtBarUV[nPos]; }
	void	 getBtnSize( int &nW, int &nH )			{ nW = m_nBtnWidth; nH = m_nBtnHeight; }
	void	 getBarPos( int &nGapX, int &nGapY )	{ nGapX = m_nBarGapX; nGapY = m_nbarGapY; }
	int		 getEdgeSize( )							{ return m_nBarEdgeSize; }
	BOOL	 getVertical( )							{ return m_bVertical; }
	
	void setThumbUV( int nPos, UIRectUV uv );
	void setBtnSize( int nW, int nH )				{ m_nBtnWidth = nW; m_nBtnHeight = nH; };
	void setBarPos( int nGapX, int nGapY )			{  m_nBarGapX = nGapX; m_nbarGapY = nGapY; }
	void setEdgeSize( int nEdge )					{ m_nBarEdgeSize = nEdge; }
	void setVertical( BOOL bVertical )				{ m_bVertical = bVertical; }	
	


	void setRight(bool bRight)		{ m_bRight = bRight; }
	bool getRight()					{ return m_bRight;	 }

	void setItemSize(float size)	{ m_fItemSize = size; }
	float getItemSize()				{ return m_fItemSize; }
	void setBackGround(UIRect rect, UIRectUV uv);
	
	void setButton(int idx, UIRect rect, UIRectUV uv0, UIRectUV uv1);
	void setThumb(UIRect rect, UIRectUV uv, int unit);

	int	getThumbWidth()				{ return m_rcThumb.GetWidth(); }

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnMouseWheel(UINT16 x, UINT16 y, int wheel);

	//--------------------- Clone() 함수 사용을 위해 ---------------------------
	void	SetRSBackground(CUIRectSurface* pRS)	{ m_pBackground = pRS;		}
	CUIRectSurface* GetRSBackground()				{ return m_pBackground;		}
	void	SetRSButton(CUIRectSurface* pRS, int idx) { m_rsButton[idx] = pRS;	}
	CUIRectSurface* GetRSButton(int idx)			{ return m_rsButton[idx];	}
	void	SetRSThumb(CUIRectSurface* pRS)			{ m_rsThumb = pRS;			}
	CUIRectSurface* GetRSThumb()					{ return m_rsThumb;			}
	//--------------------------------------------------------------------------
	void changeDragPos(int nPos);
#ifdef UI_TOOL
	// UITool에서 스크롤 버튼 연결을 위한 함수.
	void SetScrollButton(int type, CUIButton* pButton);
	void getBackGround(UIRect& rc, UIRectUV& uv)	{ rc = m_rcBackGround; uv = m_uvBackGround; }

	void getThumb(UIRect& rc, UIRectUV& uv, int& unit)	
	{
		rc = m_rcThumb;
		uv = m_rcBarMiddle;
		unit = m_nThumbUnit;
	}
	void getScrollBtn(int idx, UIRect& rc, UIRectUV& uv0, UIRectUV& uv1)
	{
		uv0 = m_uvScrollBtnIdle[idx];
		uv1 = m_uvScrollBtnClick[idx];
		rc = m_rcScrollBtn[idx];
	}
private:
	UIRectUV m_uvBackGround;
	UIRect	 m_rcBackGround;

	UIRectUV m_uvScrollBtnIdle[eSCROLL_BUTTON_MAX];
	UIRectUV m_uvScrollBtnClick[eSCROLL_BUTTON_MAX];
	UIRect	 m_rcScrollBtn[eSCROLL_BUTTON_MAX];
#endif // UI_TOOL

protected:
	// Internal functions
	void	MoveScrollPos( int ndPos );
	void	UpdateScrollBarSize();
	void	UpdateScrollBarPos();
	void	updateDragPos(float gap = 0.f);
	void	updateThumbSize();

	//--------------------------------------------------------------------------

	BOOL			m_bVertical;				// Vertical scrolling
	bool			m_bRight;
	int				m_nScrollPos;				// Position of scroll bar
	int				m_nScrollOldPos;
	int				m_nScrollRange;				// Range of scroll bar	
	int				m_nCurItemCount;			// Current count of items
	int				m_nItemsPerPage;			// Count of items per one page
	CUIButton		m_btnUp;					// Up button of scroll bar
	CUIButton		m_btnDown;					// Down button of scroll bar
	BOOL			m_bScrollBarClick;			// If bar button is clicked or not
	int				m_nDragPos;					// Position x or y for dragging
	int				m_nDragPosOffset;			// Offset x or y for dragging
	int				m_nBarPosX, m_nBarPosY;		// Position of bar button
	int				m_nBarWidth, m_nBarHeight;	// Size of bar button
	int				m_nBarEdgeSize;				// Edge size of bar button

	// Region
	UIRect			m_rcScrolling;				// Region of scrolling
	UIRect			m_rcWheel;					// Region of wheel
	UIRect			m_rcArea;					// Region of wheel

	// UV
	UIRectUV		m_rtBarTop;					// UV of top
	UIRectUV		m_rcBarMiddle;				// UV of middle
	UIRectUV		m_rtBarBottom;				// UV of bottom


	UIRectUV		m_rtBarUV[ePOS_MAX];
	CUIButton*		m_pButton[eSCROLL_BUTTON_MAX];

	int				m_nBtnWidth;
	int				m_nBtnHeight;
	int				m_nBarGapX;
	int				m_nbarGapY;

	float			m_fItemSize;
	float			m_fMoveOffset;
	float			m_fOriginMoveOffset;
	bool			m_bBtnPress[2];
	UIRect			m_rcBtn[2];
	UIRect			m_rcThumb;
	int				m_nThumbUnit;

	CUIRectSurface*	m_pBackground;				// 배경
	CUIRectSurface*	m_rsButton[2];				// Button 출력
	CUIRectSurface*	m_rsThumb;

	int				m_nDownBtnMovePos;
};


#endif	// UISCROLLBAR_H_


