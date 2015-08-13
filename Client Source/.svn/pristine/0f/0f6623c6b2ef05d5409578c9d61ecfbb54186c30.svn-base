#include "stdh.h"
#include <Engine/Interface/UIListBoxMultiList.h>
#include <Engine/Interface/UITextureManager.h>



// ----------------------------------------------------------------------------
// Name : CUIListBoxMultiCol()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIListBoxMultiCol::CUIListBoxMultiCol()
{
	m_nColumnCount		= 0;
	m_nSelectList		= -1;
	m_nOverList			= -1;
	m_bScrollBar		= FALSE;
	m_colSelectList		= 0xB8B8B8FF;
	m_colOverList		= 0xB8B8B8FF;
	m_nRowCount			= 0;
	m_nLineHeight		= 0;
	m_bSelectList		= FALSE;
	m_bAtLeastOneSelect = FALSE;
	m_nTextSX			= 1;
	m_nTextSY			= 1;
	m_nSelectOffsetY	= 1;
	m_nLinePerPage		= 1;
	
}

// ----------------------------------------------------------------------------
// Name : ~CUIListBoxMultiCol()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIListBoxMultiCol::~CUIListBoxMultiCol()
{
	int iCol = 0;
	for( iCol = 0; iCol < m_vecString.size(); iCol++ )
	{
		m_vecString[iCol].vecString.clear();				
	}
	

	for( iCol = 0; iCol < m_vecColor.size(); iCol++ )
	{
		m_vecColor[iCol].vecColor.clear();				
	}

	m_vecString.clear();
	m_vecColor.clear();
	m_vecColumnSX.clear();
	m_vecAlign.clear();
	m_vecSelectable.clear();
	m_vecDataptr.clear();
	m_vecRowWidht.clear();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBoxMultiCol::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
							int nLineHeight, int nSpaceX, int nSpaceY, int nColumn, int nRow, BOOL bSelectList, BOOL bAtLeastOneSelect )
{
	ASSERT( nColumn > 0 );

	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_nLineHeight		= nLineHeight;
	m_bSelectList		= bSelectList;
	m_bAtLeastOneSelect = bAtLeastOneSelect;

	m_nTextSX			= nSpaceX;
	m_nTextSY			= nSpaceY;

	int	nOffset			= nLineHeight - _pUIFontTexMgr->GetFontHeight();
	if( nOffset < 0 )
		nOffset = 0;

	m_nSelectOffsetY	= nOffset / 2;
	m_nLinePerPage		= ( nHeight ) / nLineHeight;

	m_nColumnCount		= nColumn;
	m_nRowCount			= nRow;

	m_vecString.reserve( nColumn );  //줄 수 예약 
	m_vecColor.reserve( nColumn );
	m_vecColumnSX.reserve( nColumn );
	m_vecAlign.reserve( nRow );

	m_vecRowWidht.reserve( nRow );
	//*
	for ( int iRow = 0; iRow < nRow; ++iRow )
	{
		m_vecRowWidht.push_back(1);
		m_vecAlign.push_back( TEXT_LEFT );
	}
	/**/
	
	for( int iCol = 0; iCol < nColumn; iCol++ )
	{
		sVecString	sVecTempString;
		sVecColor	sVecTempColor;

		m_vecString.push_back( sVecTempString );		
		m_vecColor.push_back( sVecTempColor );
		m_vecColumnSX.push_back( 0 );		
		m_vecDataptr.push_back(0);
	}	
}

// ----------------------------------------------------------------------------
// Name : CreateScroll()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBoxMultiCol::CreateScroll( BOOL bRight, int nOffsetX, int nOffsetY, int nWidth, int nHeight,
								int nBtnWidth, int nBtnHeight, int nBarWidthGap, int nBarHeightGap,
								int nBarVertEdgeSize )
{
	m_bScrollBar = TRUE;

	if( bRight )
	{
		m_sbScrollBar.Create( this, m_nWidth + nOffsetX, nOffsetY, nWidth, nHeight );
		m_sbScrollBar.SetWheelRect( -( m_nWidth + nOffsetX ), -nOffsetY, m_nWidth, m_nHeight );
	}
	else
	{
		m_sbScrollBar.Create( this, nOffsetX - nWidth, nOffsetY, nWidth, nHeight );
		m_sbScrollBar.SetWheelRect( nWidth - nOffsetX, -nOffsetY, m_nWidth, m_nHeight );
	}

	m_sbScrollBar.CreateButtons( TRUE, nBtnWidth, nBtnHeight, nBarWidthGap, nBarHeightGap, nBarVertEdgeSize );
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetItemsPerPage( m_nLinePerPage );
}


// 각 열의 폭 설정 
void CUIListBoxMultiCol::SetRowWidth ( int nRow, int nWidth )
{
	if ( nRow >= m_vecRowWidht.size() )
	{
		m_vecRowWidht.resize( nRow + 1 );
		m_nRowCount = nRow + 1;
	}

	m_vecRowWidht[nRow] = nWidth;
}

// ----------------------------------------------------------------------------
// Name : SetSelBar()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBoxMultiCol::SetSelBar( int nWidth, int nHeight,
							FLOAT fU0, FLOAT fV0, FLOAT fU1, FLOAT fV1, FLOAT fTexWidth, FLOAT fTexHeight )
{
	m_bShowSelectBar = TRUE;
	m_rtSelectOver.SetUV( fU0, fV0, fU1, fV1, fTexWidth, fTexHeight );

	if( nWidth > m_nWidth || nWidth == -1 )
		nWidth = m_nWidth;
	int	nHalfWidth = nWidth / 2;
	int	nLeft = m_nWidth / 2 - nHalfWidth;

	if( nHeight < m_nLineHeight || nHeight == -1 )
		nHeight = m_nLineHeight;

	m_rcSelectOver.SetRect( nLeft, -m_nSelectOffsetY, nLeft + nWidth, nHeight - m_nSelectOffsetY );
}

// ----------------------------------------------------------------------------
// Name : ChangeSize()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBoxMultiCol::ChangeSize( int ndLine )
{
	int	ndHeight = ndLine * m_nLineHeight;

	m_nHeight += ndHeight;
	m_nLinePerPage += ndLine;

	m_sbScrollBar.SetItemsPerPage( m_nLinePerPage );
	m_sbScrollBar.ChangeSize( ndHeight );
}


//------------------------------------------------------------------------------
// CUIListBoxMultiCol::ChangeLineCount
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIListBoxMultiCol::ChangeLineCount( int nLine )
{
	int	ndHeight = nLine * m_nLineHeight;

	m_nHeight = ndHeight;
	m_nLinePerPage = nLine;

	m_sbScrollBar.SetItemsPerPage( m_nLinePerPage );
	m_sbScrollBar.ChangeSize( ndHeight );

}


// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBoxMultiCol::AddString( int nCol, int nRow, CTString &strText, COLOR colText, BOOL bSelectable, void* ptr )
{
	//ASSERT( nCol < m_nColumnCount && nRow < m_nRowCount );

	if ( nCol >= m_nColumnCount )
	{
		m_nColumnCount = nCol + 1;
		m_vecString.resize(m_nColumnCount);
		m_vecColor.resize(m_nColumnCount);
		//m_vecAlign.resize(m_nColumnCount);
	}	
	m_vecString[nCol].vecString.push_back(strText);
	m_vecColor[nCol].vecColor.push_back( colText );
	m_vecDataptr.push_back(ptr);
	
	m_vecSelectable.push_back( bSelectable );
	

	if( m_sbScrollBar.GetCurItemCount() < m_vecString.size() )
		m_sbScrollBar.SetCurItemCount( m_vecString.size());
}

// ----------------------------------------------------------------------------
// Name : InsertString()
// Desc :
// ----------------------------------------------------------------------------
/*
void CUIListBoxMultiCol::InsertString( int nIndex, int nCol, int nRow, CTString &strText, COLOR colText, BOOL bSelectable )
{
	ASSERT( nCol < m_nColumnCount );
	ASSERT( nIndex < m_vecString[nCol].vecString.size() );

	std::vector<CTString>::iterator	itText = m_vecString[nCol].vecString.begin();
	itText += nIndex;
	//for( int i = 0; i < nIndex; i++, itText++ );
	m_vecString[nCol].vecString.insert( itText, strText );

	std::vector<COLOR>::iterator	itCol = m_vecColor[nCol].vecColor.begin();
	itCol += nIndex;
	//for( i = 0; i < nIndex; i++, itCol++ );
	m_vecColor[nCol].vecColor.insert( itCol, colText );

	std::vector<BOOL>::iterator	itBool = m_vecSelectable.begin();
	itBool += nIndex;
	//for( i = 0; i < nIndex; i++, itBool++ );
	m_vecSelectable.insert( itBool, bSelectable );

	if( m_sbScrollBar.GetCurItemCount() < m_vecString[nCol].vecString.size() )
		m_sbScrollBar.SetCurItemCount( m_vecString[nCol].vecString.size() );
}
/**/
// ----------------------------------------------------------------------------
// Name : RemoveString()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBoxMultiCol::RemoveString( int nIndex, int nCol )
{
	ASSERT( nCol < m_nColumnCount );
	ASSERT( nIndex < m_vecString[nCol].vecString.size() );

	int i;

	std::vector<CTString>::iterator	itText = m_vecString[nCol].vecString.begin();
	itText += nIndex;
	//for( i = 0; i < nIndex; i++, itText++ );
	m_vecString[nCol].vecString.erase( itText );

	std::vector<COLOR>::iterator	itCol = m_vecColor[nCol].vecColor.begin();
	itCol += nIndex;
	//for( i = 0; i < nIndex; i++, itCol++ );
	m_vecColor[nCol].vecColor.erase( itCol );

	if(nCol == 0)
	{
		std::vector<BOOL>::iterator	itBool = m_vecSelectable.begin();
		itBool += nIndex;
		//for( i = 0; i < nIndex; i++, itBool++ );
		m_vecSelectable.erase( itBool );
	}

	int	nMaxCount = 0;
	for( i = 0; i < nCol; i++ )
	{
		if( nMaxCount < m_vecString[nCol].vecString.size() )
			nMaxCount = m_vecString[nCol].vecString.size();
	}
	m_sbScrollBar.SetCurItemCount( nMaxCount );
}

// ----------------------------------------------------------------------------
// Name : MoveString()
// Desc :
// ----------------------------------------------------------------------------
/*
void CUIListBoxMultiCol::MoveString( int nFromIndex, int nToIndex, int nCol )
{
	ASSERT( nCol < m_nColumnCount );
	ASSERT( nFromIndex < m_vecString[nCol].vecString.size() );
	ASSERT( nToIndex < m_vecString[nCol].vecString.size() );

	if( nFromIndex == nToIndex )
		return;

	CTString	strTemp = m_vecString[nCol].vecString[nFromIndex];
	COLOR		colTemp = m_vecColor[nCol].vecColor[nFromIndex];
	BOOL		bTemp = m_vecSelectable[nFromIndex];

	std::vector<CTString>::iterator	itText = m_vecString[nCol].vecString.begin();
	itText += nFromIndex;
	//for( int i = 0; i < nFromIndex; i++, itText++ );
	m_vecString[nCol].vecString.erase( itText );

	std::vector<COLOR>::iterator	itCol = m_vecColor[nCol].vecColor.begin();
	itCol += nFromIndex;
	//for( i = 0; i < nFromIndex; i++, itCol++ );
	m_vecColor[nCol].vecColor.erase( itCol );
	
	std::vector<BOOL>::iterator	itBool = m_vecSelectable.begin();
	itBool += nFromIndex;
	//for( i = 0; i < nFromIndex; i++, itBool++ );
	m_vecSelectable.erase( itBool );


	itText = m_vecString[nCol].vecString.begin();
	itText += nToIndex;
	//for( i = 0; i < nToIndex; i++, itText++ );
	m_vecString[nCol].vecString.insert( itText, strTemp );

	itCol = m_vecColor[nCol].vecColor.begin();
	itCol += nToIndex;
	//for( i = 0; i < nToIndex; i++, itCol++ );
	m_vecColor[nCol].vecColor.insert( itCol, colTemp );

	itBool = m_vecSelectable.begin();
	itBool += nToIndex;
	//for( i = 0; i < nToIndex; i++, itBool++ );
	m_vecSelectable.insert( itBool, bTemp );
}
/**/
// ----------------------------------------------------------------------------
// Name : ResetAllStrings()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBoxMultiCol::ResetAllStrings()
{
	m_nSelectList	= -1;
	m_nOverList		= -1;
	m_nColumnCount	= 0;
	int iCol		= 0; 

	for( iCol = 0; iCol < m_vecString.size(); iCol++ )
	{
		m_vecString[iCol].vecString.clear();		
	}
	
	for( iCol = 0; iCol < m_vecColor.size(); iCol++ )
	{
		m_vecColor[iCol].vecColor.clear();		
	}
	
	
	m_vecSelectable.clear();
	m_vecString.clear();
	m_vecColor.clear();
	m_vecDataptr.clear();

	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetCurItemCount( 0 );
}

// ----------------------------------------------------------------------------
// Name : SetString()
// Desc :
// ----------------------------------------------------------------------------
/*
void CUIListBoxMultiCol::SetString( int nCol, int nIndex, CTString &strText )
{
	ASSERT( nCol < m_nColumnCount );

	if ( nCol >= m_nColumnCount )
		m_vecString.resize( nCol );
	ASSERT( !m_vecString[nCol].vecString.empty() && nIndex < m_vecString[nCol].vecString.size() );

	if ( nIndex >= m_vecString[nCol].vecString.size() )
		m_vecString[nCol].vecString.resize(nIndex);

	m_vecString[nCol].vecString[nIndex] = strText;
}
/**/

// ----------------------------------------------------------------------------
// Name : SetColor()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBoxMultiCol::SetColor( int nCol, int nIndex, COLOR colText )
{
	ASSERT( nCol < m_nColumnCount );
	if ( nCol >= m_nColumnCount )
		m_vecColor.resize(nCol);
	if ( nIndex >= m_vecColor[nCol].vecColor.size() || m_vecColor[nCol].vecColor.empty() )
		m_vecColor[nCol].vecColor.resize( nIndex );
	
	m_vecColor[nCol].vecColor[nIndex] = colText;
}

// ----------------------------------------------------------------------------
// Name : SetColor()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBoxMultiCol::SetSelectable( int nIndex, BOOL bSelectable )
{
	ASSERT( nIndex < m_vecSelectable.size() );

	if ( nIndex >= m_vecSelectable.size() )
		m_vecSelectable.resize( nIndex );

	m_vecSelectable[nIndex] = bSelectable;
}

// ----------------------------------------------------------------------------
// Name : SetColor()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBoxMultiCol::SetAllNotSelectable()
{
	for(int i=0; i<m_vecSelectable.size(); ++i)
	{
		m_vecSelectable[i] = FALSE;
	}
}

// ----------------------------------------------------------------------------
// Name : SetColumnPosX()
// Desc :
// ----------------------------------------------------------------------------
/*
void CUIListBoxMultiCol::SetColumnPosX( int nCol, int nPosX, TEXT_ALIGN eAlign )
{
	ASSERT( nCol < m_nColumnCount );

	if ( nCol >= m_vecColumnSX.size())
	{
		m_vecColumnSX.resize( nCol );
		m_nColumnCount = nCol;
	}
	m_vecColumnSX[nCol] = nPosX;
	m_vecAlign[nCol] = eAlign;
}
/**/
// ----------------------------------------------------------------------------
// Name : AddReserveColumn()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBoxMultiCol::AddReserveColumn ( int nSize )
{
	m_nColumnCount += nSize;
	m_vecString.resize( m_nColumnCount );  //줄 수 예약 
	m_vecColor.resize( m_nColumnCount );
	m_vecColumnSX.resize( m_nColumnCount );
	//m_vecAlign.resize( m_nColumnCount );
	
	for( int iCol = 0; iCol < m_nColumnCount; iCol++ )
	{
		sVecString	sVecTempString;
		sVecColor	sVecTempColor;
		
		m_vecString.push_back( sVecTempString );		
		m_vecColor.push_back( sVecTempColor );
		m_vecColumnSX.push_back( 0 );
		//m_vecAlign.push_back( TEXT_LEFT );
		m_vecDataptr.push_back(0);
	}	
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIListBoxMultiCol::Render()
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	// Add render regions
	// Selection bar
	if( m_bShowSelectBar && m_nSelectList >= 0 )
	{
		int	nSelLine = m_nSelectList - m_sbScrollBar.GetScrollPos();
		
		if( nSelLine >= 0 )
		{
			if( !m_bScrollBar || nSelLine < m_sbScrollBar.GetScrollRange() )
			{
				int	nBarY = nY + m_nTextSY + nSelLine * m_nLineHeight;
				CUIManager::getSingleton()->GetDrawPort()->AddTexture( nX + m_rcSelectOver.Left, nBarY + m_rcSelectOver.Top,
													nX + m_rcSelectOver.Right, nBarY + m_rcSelectOver.Bottom,
													m_rtSelectOver.U0, m_rtSelectOver.V0,
													m_rtSelectOver.U1, m_rtSelectOver.V1,
													0xFFFFFFFF );
			}
		}
	}

	// Scroll bar
	if( m_bScrollBar )
		m_sbScrollBar.Render();

	// Text in list box
	int		nTextY = nY;// + m_nTextSY;

	int	nRowS = m_sbScrollBar.GetScrollPos();
	int	nRowE = nRowS + m_nLinePerPage;
	if( nRowE > m_vecString.size())
		nRowE = m_vecString.size();

	for( int nCol = nRowS; nCol < nRowE; nCol++ )
	{
		if ( m_vecString[nCol].vecString.empty())
			continue;
		
		COLOR	colText = 0x00000000;		
		int		nSelList = -1, nOverList = -1;
		if( m_bSelectList )
		{
			nSelList = m_nSelectList;
			nOverList = nRowS + m_nOverList;
		}

		int		nTextX = nX;	

		for( int nList = 0; nList < m_nRowCount; nList++ )
		{
			if( !m_vecSelectable.empty() && m_vecSelectable.size () > nCol && IsSelectable(nCol) )
			{
				if( nCol == nSelList )
					colText = m_colSelectList;
				else if( nCol == nOverList )
					colText = m_colOverList;
				else
				{
					if ( nList < m_vecColor[nCol].vecColor.size())
						colText = m_vecColor[nCol].vecColor[nList];
					
				}
			}
			else
			{
				if ( nList < m_vecColor[nCol].vecColor.size())
						colText = m_vecColor[nCol].vecColor[nList];
			}

			if ( m_vecAlign.size() > nList )
			{			
				DrawText ( nCol, nList,nTextX, nTextY, m_vecAlign[nList], colText );
			}
			else
				DrawText ( nCol, nList,nTextX, nTextY, TEXT_LEFT, colText );
			
			if ( m_vecRowWidht.size () > nList )
				nTextX += m_vecRowWidht[nList];
			else 
				nTextX += _pUIFontTexMgr->GetFontWidth()* 20;
		}
		nTextY += m_nLineHeight;
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIListBoxMultiCol::MouseMessage( MSG *pMsg )
{
	// If list box is disabled
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( m_bSelectList )
			{
				m_nOverList = -1;

				if( IsInside( nX, nY ) )
				{
					int	nAbsY = GetAbsPosY();
					m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight;

					return WMSG_SUCCESS;
				}
			}

			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( m_bSelectList )
			{
				if( IsInside( nX, nY ) )
				{
					int	nAbsY = GetAbsPosY();
					m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight;

					int	nRealSel = m_nOverList + m_sbScrollBar.GetScrollPos();
					if( nRealSel >= 0 && nRealSel < m_sbScrollBar.GetCurItemCount()
						&& IsSelectable(nRealSel) )
					{
						m_nSelectList = nRealSel;
						return WMSG_COMMAND;
					}
					else if(!m_bAtLeastOneSelect)
					{
						m_nSelectList = -1;
					}

					return WMSG_SUCCESS;
				}
			}

			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( m_bSelectList )
			{
				if( IsInside( nX, nY ) )
				{
					int	nAbsY = GetAbsPosY();
					m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight;

					int	nRealSel = m_nOverList + m_sbScrollBar.GetScrollPos();
					if( nRealSel >= 0 && nRealSel < m_sbScrollBar.GetCurItemCount()
						&& IsSelectable(nRealSel) )
					{
						m_nSelectList = nRealSel;
						return WMSG_COMMAND;
					}

					return WMSG_SUCCESS;
				}
			}

			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

void CUIListBoxMultiCol::SetRowAlign( int nRow, TEXT_ALIGN nAlign )
{
	if ( nRow < m_vecAlign.size() )
	{
		m_vecAlign [nRow] = nAlign;
	}
	else
	{
		m_vecAlign.resize( nRow + 1);
		m_vecAlign [nRow] = nAlign;
	}

}

void CUIListBoxMultiCol::DrawText ( int nCol, int nRow, int nX, int nY, TEXT_ALIGN enAlign, COLOR TextColor )
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	switch( enAlign )
	{
		case TEXT_LEFT:
			pDrawPort->PutTextEx( m_vecString[nCol].vecString[nRow], nX, nY, TextColor );
			break;
		case TEXT_CENTER:
			pDrawPort->PutTextExCX( m_vecString[nCol].vecString[nRow], nX + m_vecRowWidht[nRow]/2, nY, TextColor );
			break;
		case TEXT_RIGHT:
			pDrawPort->PutTextExRX( m_vecString[nCol].vecString[nRow], nX, nY, TextColor );
			break;
		default:
			pDrawPort->PutTextEx( m_vecString[nCol].vecString[nRow], nX, nY, TextColor );
	}
}

BOOL CUIListBoxMultiCol::IsSelectable ( int nSel )
{
	if ( nSel < m_vecSelectable.size() )
	{
		return m_vecSelectable[nSel];
	}
	else
		return FALSE;
}