// ----------------------------------------------------------------------------
// 새로운 방식에서는 사용 안함!!!!!!!!!!!!
// UIList 로 대체함.


// ----------------------------------------------------------------------------
//  File : UIListBox.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UILISTBOX_H_
#define	UILISTBOX_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <vector>
#include <Engine/Interface/UIScrollBar.h>



// ----------------------------------------------------------------------------
// Name : CUIListBox
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUIListBox : public CUIWindow
{
protected:
	typedef struct _sVecString
	{
		std::vector<CTString>	vecString;
	} sVecString;

	typedef struct _sVecColor
	{
		std::vector<COLOR>		vecColor;
	} sVecColor;

	typedef struct _sVecBold
	{
		std::vector<BOOL>		vecBold;
	} sVecBold;

	// Controls
	BOOL					m_bScrollBar;			// If scroll bar is shown or not
	BOOL					m_bSelectList;			// If list is selected or not
	BOOL					m_bAtLeastOneSelect;
	BOOL					m_bShowSelectBar;		// If selection bar is used or not
	int						m_nSelectList;			// Index of selected list
	int						m_nOverList;			// Index of list on cursor
	int						m_nSelectOffsetY;		// Offset y of selection region
	int						m_nLinePerPage;			// Line count per page
	int						m_nColumnCount;			// Count of column
	CUIScrollBar			m_sbScrollBar;			// Scroll bar of list box

	// Strings
	std::vector<sVecString>	m_vecString;			// Strings of list box
	std::vector<sVecColor>	m_vecColor;				// Colors of list box
	std::vector<sVecBold>	m_vecBold;				// Bolds of list box
	std::vector<int>		m_vecColumnSX;			// Position x of column
	std::vector<ULONG>		m_vecColumnWidth;		// Width of column
	std::vector<TEXT_ALIGN>	m_vecAlign;				// Aligns of list box
	std::vector<BOOL>		m_vecSelectable;		// Can be selected?
	std::vector<void*>		m_vecDataptr;			
	COLOR					m_colSelectList;		// Color of selected list
	COLOR					m_colOverList;			// Color of list on cursor
	int						m_nLineHeight;			// Line height
	int						m_nTextSX;				// Position x of text
	int						m_nTextSY;				// Position y of text

	// Region of each part
	UIRect					m_rcSelectOver;			// Region of selection

	// UV of each part
	UIRectUV				m_rtSelectOver;			// UV of selection bar
	
public:
	CUIListBox();
	~CUIListBox();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, int nLineHeight,
					int nSpaceX, int nSpaceY, int nColumn, BOOL bSelectList, BOOL bAtLeastOneSelect = FALSE );
	void	CreateScroll( BOOL bRight, int nOffsetX, int nOffsetY, int nWidth, int nHeight,
							int nBtnWidth, int nBtnHeight, int nBarWidthGap, int nBarHeightGap,
							int nBarVertEdgeSize );
	void SetListValue();

	// Render
	void	Render();

	// Change size
	void	ChangeSize( int ndLine );
	void	ChangeLineCount( int nLine );

	// Set string
	void	SetColumnPosX( int nCol, int nPosX, TEXT_ALIGN eAlign = TEXT_LEFT );
	void	SetColumnWidth( int nCol, ULONG ulWidth );
	void	AddString( int nCol, CTString &strText, COLOR colText = 0xF2F2F2FF, BOOL bSelectable = TRUE, void* ptr = NULL, BOOL bBold = FALSE );
	void	InsertString( int nIndex, int nCol, CTString &strText, COLOR colText = 0xF2F2F2FF, BOOL bSelectable = TRUE, BOOL bBold = FALSE );
	void	RemoveString( int nIndex, int nCol );
	void	MoveString( int nFromIndex, int nToIndex, int nCol );
	void	ResetAllStrings();
	void	SetString( int nCol, int nIndex, CTString &strText );
	void	SetColor( int nCol, int nIndex, COLOR colText );
	void	SetBold( int nCol, int nIndex, BOOL bBold );
	void	SetSelectable( int nIndex, BOOL bSelectable );
	void	SetAllNotSelectable();
	void	SetSelectColor( COLOR colSelection ) { m_colSelectList = colSelection; }
	void	SetOverColor( COLOR colOver ) { m_colOverList = colOver; }
	CTString	&GetString( int nCol, int nIndex )
	{
		ASSERT( nCol < m_nColumnCount );
		ASSERT( nIndex < m_vecString[nCol].vecString.size() );
		return m_vecString[nCol].vecString[nIndex];
	}

	// Current selection
	void	SetCurSel( int nCurSelection )
	{
		m_nSelectList = nCurSelection;
	}

	int		GetCurSel()
	{
		return m_nSelectList;
	}
	
	// WSS_NEW_GUILD_SYSTEM 070720
	// Get count of current item
	int		GetColumnCount() const
	{		
		return m_nColumnCount;
	}

	// Get count of current item
	int		GetCurItemCount( int nCol ) const
	{
		ASSERT( nCol < m_nColumnCount );
		return m_vecString[nCol].vecString.size();
	}

	// Get lines per page
	int		GetLinePerPage() const { return m_nLinePerPage; }

	// Get height of line
	int		GetLineHeight() const { return m_nLineHeight; }

	// Set selection bar
	void	SetSelBar( int nWidth, int nHeight, FLOAT fU0, FLOAT fV0, FLOAT fU1, FLOAT fV1, FLOAT fTexWidth, FLOAT fTexHeight );

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

	void SetScrollBarPos( int nPos ) { m_sbScrollBar.SetScrollPos( nPos ); }
	int GetScrollBarPos() { return m_sbScrollBar.GetScrollPos(); }
	int GetScrollBarItemsPerPage() { return m_sbScrollBar.GetItemsPerPage();	}
	
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void SetScrollBar( BOOL bScrollBar )
	{
		m_bScrollBar = bScrollBar;
	}

	BOOL GetScrollBar()
	{
		return m_bScrollBar;
	}

	void* GetSelData(int nCol) const
	{
		return m_vecDataptr[nCol];
	}
	
	// 현재 over하고 있는 라인을 넘겨준다.
	int GetCurOverList () const
	{
		return m_nOverList;
	}

};


#endif	// UILISTBOX_H_

