#include "stdh.h"
#include <Engine/Interface/UIScrollBar.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Help/UISupport.h>
#include <Engine/Util.h>

#define	DEF_SCROLL_BTN_UP		"btn_up"
#define	DEF_SCROLL_BTN_DOWN		"btn_down"
#define DEF_BUTTON_SIZEX		9
#define DEF_BUTTON_SIZEY		8
#define DEF_THUMB_SIZE_MIN		17

// ----------------------------------------------------------------------------
// Name : CUIScrollBar()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIScrollBar::CUIScrollBar()
{
	m_bScrollBarClick = FALSE;
	m_nScrollPos = 0;
	m_nScrollOldPos = 0;
	m_nScrollRange = 0;
	m_nCurItemCount = 0;
	m_nItemsPerPage = 0;
	m_rcWheel.SetRect( 0, 0, 0, 0 );
	m_rcArea.SetRect( 0, 0, 0, 0 );
	m_nDragPosOffset = 0;
	m_fItemSize = 0.f;
	m_fMoveOffset = 0.f;
	m_nThumbUnit  = 0;
	m_nBarEdgeSize = 0;
	m_fOriginMoveOffset = 0;
	m_nDownBtnMovePos = 0;
#ifdef UI_TOOL
	m_nBtnWidth = 0;
	m_nBtnHeight = 0;
	m_nBarGapX = 0;
	m_nbarGapY = 0;
	m_bVertical = TRUE;
#endif // UI_TOOL
	setType(eUI_CONTROL_SCROLL);

	int i;

	m_pBackground = NULL;
	m_rsThumb	  = NULL;

	for (i = 0; i < eSCROLL_BUTTON_MAX; ++i)
		m_rsButton[i] = NULL;
}

CUIScrollBar::~CUIScrollBar()
{
	Destroy();

	SAFE_DELETE(m_pBackground);
	SAFE_DELETE(m_rsThumb);
	
	int i;

	for (i = 0; i < eSCROLL_BUTTON_MAX; ++i)
		SAFE_DELETE(m_rsButton[i]);
}

CUIBase* CUIScrollBar::Clone()
{
	CUIScrollBar* pScroll = new CUIScrollBar(*this);

	if (pScroll == NULL)
		return NULL;

	pScroll->setTexString( getTexString() );

	CUIRectSurface* pRS = NULL;
	pRS = GetRSBackground();

	if( pRS != NULL )
	{
		CUIRectSurface* pCopy = new CUIRectSurface;
		pCopy->CopyRectSurface(pRS);

		pScroll->SetRSBackground(pCopy);
		pRS = NULL;
	}

	pRS = GetRSThumb();
	if( pRS != NULL )
	{
		CUIRectSurface* pCopy = new CUIRectSurface;
		pCopy->CopyRectSurface(pRS);

		pScroll->SetRSThumb(pCopy);
		pRS = NULL;
	}

	int i;
	
	for (i = 0 ; i < eSCROLL_BUTTON_MAX; ++i)
	{
		pRS = GetRSButton(i);

		if( pRS != NULL )
		{
			CUIRectSurface* pCopy = new CUIRectSurface;
			pCopy->CopyRectSurface(pRS);
	
			pScroll->SetRSButton(pCopy, i);
			pRS = NULL;
		}
	}

	CUIBase::CloneChild(pScroll);

	return pScroll;
}

// ----------------------------------------------------------------------------
// Name : CreateButtons()
// Desc :
// ----------------------------------------------------------------------------
void CUIScrollBar::CreateButtons( BOOL bVerticalScroll,	int nBtnWidth, int nBtnHeight,
									int nBarWidthGap, int nBarHeightGap, int nBarVertEdgeSize )
{
	m_bVertical = bVerticalScroll;
	m_nBarEdgeSize = nBarVertEdgeSize;

	// Create buttons
	if( bVerticalScroll )
	{
		m_nBarPosX = nBarWidthGap;
		m_nBarPosY = nBtnHeight + nBarHeightGap;
		m_nBarWidth = m_nWidth - nBarWidthGap * 2;
		m_nBarHeight = m_nHeight - nBarHeightGap * 2 - nBtnHeight * 2;

		m_btnUp.Create( this, CTString( "" ), 0, 0, nBtnWidth, nBtnHeight );
		m_btnDown.Create( this, CTString( "" ), 0, m_nHeight - nBtnHeight, nBtnWidth, nBtnHeight );
	}
	else
	{
		m_nBarPosX = nBtnWidth + nBarWidthGap;
		m_nBarPosY = nBarHeightGap;
		m_nBarWidth = m_nWidth - nBarWidthGap * 2 - nBtnWidth * 2;
		m_nBarHeight = m_nHeight - nBarHeightGap * 2;

		m_btnUp.Create( this, CTString( "" ), 0, 0, nBtnWidth, nBtnHeight );
		m_btnDown.Create( this, CTString( "" ), m_nWidth - nBtnWidth, 0, nBtnWidth, nBtnHeight );
	}

	m_rcScrolling.SetRect( m_nBarPosX, m_nBarPosY, m_nBarPosX + m_nBarWidth, m_nBarPosY + m_nBarHeight );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIScrollBar::Render()
{
	// Get position
	int	nAbsX, nAbsY;
	GetAbsPos( nAbsX, nAbsY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Add render regions
	// Bar button
	int	nBarX = nAbsX + m_nBarPosX;
	int	nBarY = nAbsY + m_nBarPosY;
	if( m_bVertical )
	{
		pDrawPort->AddTexture( nBarX, nBarY,
											nBarX + m_nBarWidth, nBarY + m_nBarEdgeSize,
											m_rtBarTop.U0, m_rtBarTop.V0,
											m_rtBarTop.U1, m_rtBarTop.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nBarX, nBarY + m_nBarEdgeSize,
											nBarX + m_nBarWidth, nBarY + m_nBarHeight - m_nBarEdgeSize,
											m_rcBarMiddle.U0, m_rcBarMiddle.V0,
											m_rcBarMiddle.U1, m_rcBarMiddle.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nBarX, nBarY + m_nBarHeight - m_nBarEdgeSize,
											nBarX + m_nBarWidth, nBarY + m_nBarHeight,
											m_rtBarBottom.U0, m_rtBarBottom.V0,
											m_rtBarBottom.U1, m_rtBarBottom.V1,
											0xFFFFFFFF );
	}
	else
	{
		pDrawPort->AddTexture( nBarX, nBarY,
											nBarX + m_nBarEdgeSize, nBarY + m_nBarHeight,
											m_rtBarTop.U0, m_rtBarTop.V0,
											m_rtBarTop.U1, m_rtBarTop.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nBarX + m_nBarEdgeSize, nBarY,
											nBarX + m_nBarWidth - m_nBarEdgeSize, nBarY + m_nBarHeight,
											m_rcBarMiddle.U0, m_rcBarMiddle.V0,
											m_rcBarMiddle.U1, m_rcBarMiddle.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nBarX + m_nBarWidth - m_nBarEdgeSize, nBarY,
											nBarX + m_nBarWidth, nBarY + m_nBarHeight,
											m_rtBarBottom.U0, m_rtBarBottom.V0,
											m_rtBarBottom.U1, m_rtBarBottom.V1,
											0xFFFFFFFF );
	}

	// Up button
	m_btnUp.Render();

	// Down button
	m_btnDown.Render();	
}

// ----------------------------------------------------------------------------
// Name : ChangeSize()
// Desc :
// ----------------------------------------------------------------------------
void CUIScrollBar::ChangeSize( int ndSize )
{
	if( m_bVertical )
	{
		m_rcScrolling.Bottom += ndSize;
		m_nHeight += ndSize;
		m_rcWheel.Bottom += ndSize;
		m_btnDown.Move( 0, ndSize );
	}
	else
	{
		m_rcScrolling.Right += ndSize;
		m_nWidth += ndSize;
		m_rcWheel.Right += ndSize;
		m_btnDown.Move( ndSize, 0 );
	}

	UpdateScrollBarSize();
}

void CUIScrollBar::UpdateSize( int nSize, int nItemPerPage )
{
	m_nItemsPerPage = nItemPerPage;
	m_nDownBtnMovePos -= m_nHeight - nSize;

	if (m_bVertical == TRUE)
		m_nHeight = nSize;
	else
		m_nWidth = nSize;
	
	updateThumbSize();
}

// ----------------------------------------------------------------------------
// Name : MoveScrollPos()
// Desc :
// ----------------------------------------------------------------------------
void CUIScrollBar::MoveScrollPos( int ndPos )
{
	if( ndPos < 0 )
	{
		m_nScrollPos += ndPos;
		
		if( m_nScrollPos < 0 )
			m_nScrollPos = 0;
	}
	else
	{
		m_nScrollPos += ndPos;
		
		int	nScrollLine = m_nCurItemCount - m_nItemsPerPage;
		if( nScrollLine < 0 )
			m_nScrollPos = 0;
		else if( m_nScrollPos > nScrollLine )
			m_nScrollPos = nScrollLine;
	}

	UpdateScrollBarPos();
}

// ----------------------------------------------------------------------------
// Name : UpdateScrollBarSize()
// Desc :
// ----------------------------------------------------------------------------
void CUIScrollBar::UpdateScrollBarSize()
{
	FLOAT	fSizeRatio = 1.0f;
	if( m_nCurItemCount > 0 && m_nItemsPerPage > 0 && m_nCurItemCount > m_nItemsPerPage )
		fSizeRatio = m_nItemsPerPage / (FLOAT)m_nCurItemCount;
	else
		m_nScrollPos = 0;

	int	nMinBarSize = m_nBarEdgeSize * 2;
	if( m_bVertical )
	{
		m_nBarHeight = m_rcScrolling.GetHeight() * fSizeRatio;
		if( m_nBarHeight < nMinBarSize )
			m_nBarHeight = nMinBarSize;
	}
	else
	{
		m_nBarWidth = m_rcScrolling.GetWidth() * fSizeRatio;
		if( m_nBarWidth < nMinBarSize )
			m_nBarWidth = nMinBarSize;
	}

	UpdateScrollBarPos();
}

// ----------------------------------------------------------------------------
// Name : UpdateScrollBarPos()
// Desc :
// ----------------------------------------------------------------------------
void CUIScrollBar::UpdateScrollBarPos()
{
	FLOAT	fPosRatio = 0.0f;
	int	nScrollLine = m_nCurItemCount - m_nItemsPerPage;
	if( nScrollLine > 0 )
		fPosRatio = m_nScrollPos / (FLOAT)nScrollLine;

	if( m_bVertical )
		m_nBarPosY = m_rcScrolling.Top + ( m_rcScrolling.GetHeight() - m_nBarHeight ) * fPosRatio;
	else
		m_nBarPosX = m_rcScrolling.Left + ( m_rcScrolling.GetWidth() - m_nBarWidth ) * fPosRatio;

	m_fMoveOffset = m_fOriginMoveOffset;
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIScrollBar::MouseMessage( MSG *pMsg )
{
	// If scroll bar is disabled
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Messages
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			// Bar button
			if( m_bScrollBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	nAbsY, nAbsX;
				GetAbsPos( nAbsX, nAbsY );

				int	nScrollLine = m_nCurItemCount - m_nItemsPerPage;
				if( nScrollLine < 0 )
					nScrollLine = 0;

				if( m_bVertical )
				{
					if( m_nDragPosOffset != 0 )
					{
						BOOL	bOldNegative = m_nDragPosOffset < 0;
						m_nDragPosOffset += nY - m_nDragPos;
						BOOL	bNewNegative = m_nDragPosOffset < 0;

						if( bOldNegative != bNewNegative )
						{
							m_nBarPosY += m_nDragPosOffset;
							m_nDragPosOffset = 0;
						}
					}
					else
						m_nBarPosY += nY - m_nDragPos;

					if( m_nBarPosY < m_rcScrolling.Top )
					{
						m_nDragPosOffset += m_nBarPosY - m_rcScrolling.Top;
						m_nBarPosY = m_rcScrolling.Top;
					}
					else if( m_nBarPosY > m_rcScrolling.Bottom - m_nBarHeight )
					{
						m_nDragPosOffset += m_nBarPosY - ( m_rcScrolling.Bottom - m_nBarHeight );
						m_nBarPosY = m_rcScrolling.Bottom - m_nBarHeight;
					}

					FLOAT	fPosRatio = ( m_nBarPosY - m_rcScrolling.Top ) /
										(FLOAT)( m_rcScrolling.GetHeight() - m_nBarHeight );
					m_nScrollPos = nScrollLine * fPosRatio;

					m_nDragPos = nY;
				}
				else
				{
					if( m_nDragPosOffset != 0 )
					{
						BOOL	bOldNegative = m_nDragPosOffset < 0;
						m_nDragPosOffset += nX - m_nDragPos;
						BOOL	bNewNegative = m_nDragPosOffset < 0;

						if( bOldNegative != bNewNegative )
						{
							m_nBarPosX += m_nDragPosOffset;
							m_nDragPosOffset = 0;
						}
					}
					else
						m_nBarPosX += nX - m_nDragPos;

					if( m_nBarPosX < m_rcScrolling.Left )
					{
						m_nDragPosOffset += m_nBarPosX - m_rcScrolling.Left;
						m_nBarPosX = m_rcScrolling.Left;
					}
					else if( m_nBarPosX > m_rcScrolling.Right - m_nBarWidth )
					{
						m_nDragPosOffset += m_nBarPosX - ( m_rcScrolling.Right - m_nBarWidth );
						m_nBarPosX = m_rcScrolling.Right - m_nBarWidth;
					}

					FLOAT	fPosRatio = ( m_nBarPosX - m_rcScrolling.Left ) /
										(FLOAT)( m_rcScrolling.GetWidth() - m_nBarWidth );
					m_nScrollPos = nScrollLine * fPosRatio;

					m_nDragPos = nX;
				}

				return WMSG_COMMAND;
			}
			// Up button
			else if( m_btnUp.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Down button
			else if( m_btnDown.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_nCurItemCount <= m_nItemsPerPage )
					return WMSG_SUCCESS;

				// Up button
				if( m_btnUp.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					MoveScrollPos( -1 );
					return WMSG_COMMAND;
				}
				// Down button
				else if( m_btnDown.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					MoveScrollPos( +1 );
					return WMSG_COMMAND;
				}
				// Bar button
				else if( IsInsideRect( nX, nY, m_rcScrolling ) )
				{
					m_nDragPosOffset = 0;

					int	nAbsY, nAbsX;
					GetAbsPos( nAbsX, nAbsY );
					nAbsX += m_nBarPosX;	nAbsY += m_nBarPosY;
					if( nX >= nAbsX && nY >= nAbsY && nX < nAbsX + m_nBarWidth && nY < nAbsY + m_nBarHeight )
					{
						m_bScrollBarClick = TRUE;
						if( m_bVertical )
							m_nDragPos = nY;
						else
							m_nDragPos = nX;
					}
					else
					{
						if( m_bVertical )
						{
							int	nCenterY = nAbsY + m_nBarHeight / 2;
							if( nY < nCenterY )
								MoveScrollPos( -m_nItemsPerPage );
							else
								MoveScrollPos( m_nItemsPerPage );
						}
						else
						{
							int	nCenterX = nAbsX + m_nBarWidth / 2;
							if( nX < nCenterX )
								MoveScrollPos( -m_nItemsPerPage );
							else
								MoveScrollPos( m_nItemsPerPage );
						}

						return WMSG_COMMAND;
					}
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			// Bar button
			if( m_bScrollBarClick )
			{
				m_bScrollBarClick = FALSE;
				m_nDragPosOffset = 0;
				return WMSG_SUCCESS;
			}
			// Up button
			else if( m_btnUp.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Down button
			else if( m_btnDown.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// Up button
				if( m_btnUp.MouseMessage( pMsg ) == WMSG_COMMAND )
				{
					MoveScrollPos( -1 );
					return WMSG_COMMAND;
				}
				// Down button
				else if( m_btnDown.MouseMessage( pMsg ) == WMSG_COMMAND )
				{
					MoveScrollPos( +1 );
					return WMSG_COMMAND;
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) || IsInsideRect( nX, nY, m_rcWheel ) )
			{
				SWORD	swDelta = SWORD( HIWORD( pMsg->wParam ) ) / WHEEL_DELTA;
				MoveScrollPos( -swDelta );

				return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

void CUIScrollBar::OnUpdate( float fElapsedTime )
{
	// 값이 변경될 경우 여기서 처리해 줍니다.
	//m_nBarPosX
	//m_nBarPosY

	//m_imgBar[0].SetPos( m_nBarPosX, m_nBarPosY  );
}

void CUIScrollBar::OnRender( CDrawPort* pDraw )
{
	int		i;

	if( m_pTexData == NULL )
	{
#ifdef UI_TOOL
		RenderBorder(pDraw);
#endif // UI_TOOL
		return;
	}

	pDraw->InitTextureData( m_pTexData );
	
	int	nAbsX, nAbsY;
	GetAbsPos( nAbsX, nAbsY );

	if (m_pBackground != NULL)
	{
		m_pBackground->SetPos(nAbsX, nAbsY);
		m_pBackground->RenderRectSurface(pDraw, DEF_UI_COLOR_WHITE);
	}

	for (i = 0; i < 2; ++i)
	{
		if (m_rsButton[i] == NULL)
			continue;

		if( m_rsButton[i]->m_RectSurfaceArray.Count() <= 0 )
			break;

		if (i == eSCROLL_BUTTON_UP)
			m_rsButton[i]->SetPos(nAbsX, nAbsY);	
		else if (i == eSCROLL_BUTTON_DOWN)
		{
			if (m_bVertical)
				m_rsButton[i]->SetPos(nAbsX, nAbsY + m_nDownBtnMovePos);
			else
				m_rsButton[i]->SetPos(nAbsX + m_nDownBtnMovePos, nAbsY);
		}

		if (m_bBtnPress[i] == true)
			m_rsButton[i]->RenderRectSurface(pDraw, DEF_UI_COLOR_WHITE, 0);
		else
			m_rsButton[i]->RenderRectSurface(pDraw, DEF_UI_COLOR_WHITE, 1);
	}

	if (m_rsThumb != NULL)
	{
		m_rsThumb->SetPos(nAbsX, nAbsY + (int)m_fMoveOffset);
		m_rsThumb->RenderRectSurface(pDraw, DEF_UI_COLOR_WHITE);
	}


	pDraw->FlushRenderingQueue();
#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

void CUIScrollBar::setThumbUV( int nPos, UIRectUV uv )
{
	if(nPos > ePOS_MAX ) return;

	UISUPPORT()->DivideTexUV(m_pTexData, uv);

	m_rtBarUV[nPos].SetUV(uv.U0, uv.V0, uv.U1, uv.V1);
}


void CUIScrollBar::initialize()
{

}

#ifdef UI_TOOL
void CUIScrollBar::SetScrollButton( int type, CUIButton* pButton )
{
	if( pButton == NULL )
		return;

	if( type < 0 && type > eSCROLL_BUTTON_MAX )
		return;

	m_pButton[(eSCROLL_BUTTON)type]	= pButton;
	this->addChild(m_pButton[(eSCROLL_BUTTON)type]);
}

#endif // UI_TOOL=======

void CUIScrollBar::setBackGround( UIRect rect, UIRectUV uv )
{
	if (m_pBackground == NULL)
		m_pBackground = new CUIRectSurface;

#ifdef UI_TOOL
	m_rcBackGround = rect;
	m_uvBackGround = uv;
#endif // UI_TOOL

	UISUPPORT()->DivideTexUV(m_pTexData, uv);
	// 현재 renderSurface 가 UV 개념으로 되어 있기때문에 가공.
	UIRect rc = rect;
	
	// 자동으로 위치를 잡아준다.
	if (m_bRight)
	{
		rc.Left = m_nWidth - rect.Right;
	}	

	rc.Right += rc.Left;
	rc.Bottom += rc.Top;

	m_rcScrolling = rc;

	m_pBackground->AddRectSurface(rc, uv);
}

void CUIScrollBar::setButton( int idx, UIRect rect, UIRectUV uv0, UIRectUV uv1 )
{
	if (idx >= 2)
		return;

	if (m_rsButton[idx] == NULL)
		m_rsButton[idx] = new CUIRectSurface;

#ifdef UI_TOOL
	m_uvScrollBtnIdle[idx] = uv0;
	m_uvScrollBtnClick[idx] = uv1;
	m_rcScrollBtn[idx] = rect;
#endif // UI_TOOL
	UISUPPORT()->DivideTexUV(m_pTexData, uv0);
	UISUPPORT()->DivideTexUV(m_pTexData, uv1);

	// 현재 renderSurface 가 UV 개념으로 되어 있기때문에 가공.
	UIRect rc = rect;	

	// 자동으로 위치를 잡아준다.
	if (m_bRight)
	{
		rc.Left = m_nWidth - rect.Right;
	}

	if (idx == 0)
	{
		// offset 초기화
		m_fMoveOffset = (float)rc.Bottom;
		m_fOriginMoveOffset = m_fMoveOffset;
	}
	else if (idx == 1)
	{
		rc.Top = m_nHeight - rect.Bottom;
	}	

	rc.Right += rc.Left;
	rc.Bottom += rc.Top;

	m_rcBtn[idx] = rc;

	m_rsButton[idx]->AddRectSurface(rc, uv0);
	m_rsButton[idx]->AddRectSurface(rc, uv1);
}

void CUIScrollBar::setThumb( UIRect rect, UIRectUV uv, int unit )
{
	UIRect rc = rect;

	m_rcThumb = rect;
	m_rcBarMiddle = uv;		// thumb UV 로 사용
	m_nThumbUnit = unit;

	
	// 자동으로 위치를 잡아준다.
	if (m_bRight)
	{
		rc.Left = m_nWidth - rect.Right;
	}

	rc.Right += rc.Left;
	rc.Bottom += rc.Top;

	m_rcArea = rc;

	if (uv.U0 <= 0.f && uv.V0 <= 0.f && uv.U1 <= 0.f && uv.V1 <= 0.f)
		return;

	if (m_rsThumb == NULL)
		m_rsThumb = new CUIRectSurface;

	UISUPPORT()->Split3PlaneVertical((*m_rsThumb), m_pTexData, rc, uv, unit);
}

//------------------------------------------------------------------

WMSG_RESULT CUIScrollBar::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	int		i;

	for (i = 0; i < 2; ++i)
	{
		int nOffsetX = 0, nOffsetY = 0;

		if (i == eSCROLL_BUTTON_DOWN)
		{
			if (m_bVertical)
				nOffsetY = m_nDownBtnMovePos;
			else
				nOffsetX = m_nDownBtnMovePos;
		}

		if (IsInsideRect(x - nOffsetX, y - nOffsetY, m_rcBtn[i]) == TRUE)
		{
			m_bBtnPress[i] = true;
			if (i == 0)
				updateDragPos( -m_fItemSize );
			else
				updateDragPos( m_fItemSize );

			return WMSG_SUCCESS;
		}
	}

	if (IsInsideRect(x, y - m_fMoveOffset, m_rcArea) == TRUE)
	{
		// thumb 버튼이 눌렸다.
		m_bScrollBarClick = TRUE;
		m_nDragPos = y;

		return WMSG_SUCCESS;
	}

	if (IsInsideRect(x, y, m_rcScrolling) == TRUE)
	{
		int		nY = GetAbsPosY();

		if ((y - nY) < m_fMoveOffset)
			updateDragPos( -(m_fItemSize * (float)m_nItemsPerPage) );
		else
			updateDragPos( (m_fItemSize * (float)m_nItemsPerPage) );

		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

WMSG_RESULT CUIScrollBar::OnLButtonUp( UINT16 x, UINT16 y )
{
	int		i;

	if (m_bHide == TRUE)
		return WMSG_FAIL;

	for (i = 0; i < 2; ++i)
	{
		if (m_bBtnPress[i] == true)
			m_bBtnPress[i] = false;
	}

	m_bScrollBarClick = FALSE;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	return WMSG_FAIL;
}

WMSG_RESULT CUIScrollBar::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	int		i;

	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	for (i = 0; i < 2; ++i)
	{
		int nOffsetX = 0, nOffsetY = 0;

		if (i == eSCROLL_BUTTON_DOWN)
		{
			if (m_bVertical)
				nOffsetY = m_nDownBtnMovePos;
			else
				nOffsetX = m_nDownBtnMovePos;
		}

		if (IsInsideRect(x - nOffsetX, y - nOffsetY, m_rcBtn[i]) == FALSE)
			m_bBtnPress[i] = false;
	}

	if( m_bScrollBarClick && ( pMsg->wParam & MK_LBUTTON ) )
	{
		updateDragPos(y - m_nDragPos);

		m_nDragPos = y;
	}

	return WMSG_FAIL;
}

WMSG_RESULT CUIScrollBar::OnMouseWheel( UINT16 x, UINT16 y, int wheel )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	float		offset = 0.f;

	if (wheel > 0)
		offset = -m_fItemSize;
	else 
		offset = m_fItemSize;

	updateDragPos( offset );

	return WMSG_SUCCESS;
}

void CUIScrollBar::changeDragPos( int nPos )
{
	m_fMoveOffset = 0;
	updateDragPos(m_fItemSize * nPos);
}

//-------------------------------------------------------------------
void CUIScrollBar::updateDragPos(float gap)
{
	m_fMoveOffset += gap;

	int top = m_rcBtn[0].GetHeight();

	if (m_fMoveOffset < top)
	{
		m_fMoveOffset = (float)top;
	}
	else 
	{
		int		size_ = m_nHeight - m_rcThumb.GetHeight() - m_rcBtn[1].GetHeight();

		if (m_fMoveOffset > size_)
			m_fMoveOffset = (float)size_;
	}

	m_nScrollPos = (int)((float)(m_fMoveOffset - top) / (float)m_fItemSize);

	if (m_nScrollPos < 0)	// 0으로 나누기 할 경우나 음수값이 들어 올 경우 예외처리.
		m_nScrollPos = 0;

	if (m_nScrollPos != m_nScrollOldPos)
	{
		m_nScrollOldPos = m_nScrollPos;

		if (m_pCmd)
			m_pCmd->execute();

	//	LOG_DEBUG( "callback!! %d", m_nScrollOldPos );
	}
}

void CUIScrollBar::SetScrollCurPos( int nPos )
{
	if (nPos > m_nCurItemCount)
		return;

	if (nPos > (m_nCurItemCount - m_nItemsPerPage))
		nPos = (m_nCurItemCount - m_nItemsPerPage);

	if (nPos < 0)
		nPos = 0;

	m_nScrollPos = nPos;
	m_fMoveOffset = m_fOriginMoveOffset + (float)((float)m_nScrollPos * m_fItemSize);
}

void CUIScrollBar::updateThumbSize()
{
	if (m_nItemsPerPage == 0)
		return;

	// 스크롤이 만들어질 수 있는 최대 크기
	int		size_max = m_nHeight - m_rcBtn[0].GetHeight() - m_rcBtn[1].GetHeight();
	float	size_ratio = size_max;	

	float fRatio = 1.f;
	if (m_nCurItemCount > m_nItemsPerPage)
	{
		fRatio = (float)m_nItemsPerPage / (float)m_nCurItemCount;

		size_ratio = size_max * fRatio;

		if (size_ratio < DEF_THUMB_SIZE_MIN)
			size_ratio = DEF_THUMB_SIZE_MIN;

		// Item 1개당 이동 간격을 계산한다.
		m_fItemSize = ( (float)(size_max - size_ratio) / (float)(m_nCurItemCount - m_nItemsPerPage) );
	}

	m_rcThumb.Bottom = m_rcThumb.Top + size_ratio;

	setThumb(m_rcThumb, m_rcBarMiddle, m_nThumbUnit);	
}

