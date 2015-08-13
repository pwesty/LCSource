// ----------------------------------------------------------------------------
//  File : UIListBoxMultiCol.h
//  Desc : 
// ----------------------------------------------------------------------------

#ifndef	UILISTBOXMULTICOL_H_
#define	UILISTBOXMULTICOL_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


#include <vector>
#include <Engine/Interface/UIScrollBar.h>



// ----------------------------------------------------------------------------
// Name : CUIListBoxMultiCol
// Desc :
// ----------------------------------------------------------------------------
class CUIListBoxMultiCol : public CUIWindow
{
protected:
	typedef std::vector<CTString> VEC_CTSTRING;
	typedef VEC_CTSTRING::iterator VEC_CTSTRING_IT;
	typedef struct _sVecString
	{		
		VEC_CTSTRING	vecString;
	} sVecString;

	typedef struct _sVecColor
	{	
		std::vector<COLOR>		vecColor;
	} sVecColor;

	typedef std::vector<sVecString> VEC_STRING_STRUCT;
	typedef VEC_STRING_STRUCT::iterator VEC_STRING_STRUCT_IT;
	
	std::vector<int> m_vecRowWidht;
	// Controls
	BOOL					m_bScrollBar;			// If scroll bar is shown or not
	BOOL					m_bSelectList;			// If list is selected or not
	BOOL					m_bAtLeastOneSelect;
	BOOL					m_bShowSelectBar;		// If selection bar is used or not
	int						m_nSelectList;			// Index of selected list
	int						m_nOverList;			// Index of list on cursor
	int						m_nSelectOffsetY;		// Offset y of selection region
	int						m_nLinePerPage;			// Line count per page
	int						m_nColumnCount;			// Count of column 행 
	int						m_nRowCount;			// Count of Row 열 (Row는 가로 행이나 column이 행으로 쓰인 듯하여 일단 열로 처리)
	CUIScrollBar			m_sbScrollBar;			// Scroll bar of list box

	
	// Strings
	VEC_STRING_STRUCT		m_vecString;			// Strings of list box
	std::vector<sVecColor>	m_vecColor;				// Colors of list box
	std::vector<int>		m_vecColumnSX;			// Position x of column
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
	CUIListBoxMultiCol();
	~CUIListBoxMultiCol();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, int nLineHeight,
					int nSpaceX, int nSpaceY, int nColumn, int nRow, BOOL bSelectList, BOOL bAtLeastOneSelect = FALSE );
	void	CreateScroll( BOOL bRight, int nOffsetX, int nOffsetY, int nWidth, int nHeight,
							int nBtnWidth, int nBtnHeight, int nBarWidthGap, int nBarHeightGap,
							int nBarVertEdgeSize );
	
	// 각 열의 폭 설정 
	void	SetRowWidth ( int nRow, int nWidth );
	// Render
	void	Render();

	// Change size
	void	ChangeSize( int ndLine );
	void	ChangeLineCount( int nLine );

	// Set string
	//void	SetColumnPosX( int nCol, int nPosX, TEXT_ALIGN eAlign = TEXT_LEFT );	
	void	AddString( int nCol, int nRow, CTString &strText, COLOR colText = 0xF2F2F2FF, BOOL bSelectable = TRUE, void* ptr = NULL );
	//void	InsertString( int nIndex, int nCol, int nRow, CTString &strText, COLOR colText = 0xF2F2F2FF, BOOL bSelectable = TRUE );
	void	RemoveString( int nIndex, int nCol ); //삭제는 한 줄 통으로 지울 것 그렇지 않으면 내용이 안 맞을 듯 
	//void	MoveString( int nFromIndex, int nToIndex, int nCol );
	void	ResetAllStrings();
	//void	SetString( int nCol, int nIndex, CTString &strText );
	void	SetColor( int nCol, int nIndex, COLOR colText );
	void	SetSelectable( int nIndex, BOOL bSelectable );
	void	SetAllNotSelectable();
	void	SetSelectColor( COLOR colSelection ) { m_colSelectList = colSelection; }
	void	SetOverColor( COLOR colOver ) { m_colOverList = colOver; }
	CTString	&GetString( int nCol, int nRow )
	{
		ASSERT( nCol < m_nColumnCount );
		ASSERT( nRow < m_vecString[nCol].vecString.size() );
		return m_vecString[nCol].vecString[nRow];
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

	void* GetSelData(int nCol) const
	{
		return m_vecDataptr[nCol];
	}

	int GetListRowWidth ( int nRow )
	{
		if ( nRow < m_nRowCount )
		{
			return m_vecRowWidht[nRow];
		}
		return 0;
	}

	// 사이즈 만큼 더 할당 
	void AddReserveColumn ( int nSize );
		// align 설정
	void SetRowAlign( int nRow, TEXT_ALIGN nAlign );

	void DrawText ( int nCol, int nRow, int nX, int nY, TEXT_ALIGN enAlign, COLOR TextColor );
	BOOL IsSelectable ( int nSel );
};


#endif	// UILISTBOX_H_

