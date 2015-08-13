#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UISlideBar.h>
#include <Engine/Interface/UIManager.h>


// ----------------------------------------------------------------------------
// Name : CUISlideBar()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUISlideBar::CUISlideBar()
{
	m_bSlideBarClick = FALSE;
	m_nCurPos = 0;
	m_nRange = 0;
	m_nMinPos = 0;
	m_nMaxPos = 0;

	setType(eUI_CONTROL_SLIDEBAR);
}

// ----------------------------------------------------------------------------
// Name : ~CUISlideBar()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISlideBar::~CUISlideBar()
{
	Destroy();
}

CUIBase* CUISlideBar::Clone()
{
	CUISlideBar* pSlideBar = new CUISlideBar(*this);

	CUIBase::CloneChild(pSlideBar);

	return pSlideBar;
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
							int nBarWidth, int nBarHeight,
							int nCurPos, int nRange, int nMinPos, int nMaxPos )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_rcBar.Top = ( m_nHeight - nBarHeight ) / 2;
	m_rcBar.Bottom = m_rcBar.Top + nBarHeight;
	m_rcBar.Left = 0;
	m_rcBar.Right = nBarWidth;

	m_rcSlide.SetRect( 0, m_rcBar.Top, m_nWidth, m_rcBar.Bottom );

	m_nCurPos = nCurPos;
	m_nRange = nRange;
	m_nMinPos = nMinPos;
	m_nMaxPos = nMaxPos;

	SetCurPos( nCurPos );
}

// ----------------------------------------------------------------------------
// Name : AdjustSlidePos()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::AdjustSlidePos( int nX )
{
	int	nAbsX = GetAbsPosX();

	FLOAT	fCellWidth = ( m_nWidth - m_rcBar.GetWidth() ) / (FLOAT)( m_nRange - 1 );
	int		nSX = nAbsX + ( m_rcBar.GetWidth() - fCellWidth ) / 2;
	int		nCurPos = ( nX - nSX ) / fCellWidth;

	SetCurPos( nCurPos );
}

// ----------------------------------------------------------------------------
// Name : SetCurPos()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::SetCurPos( int nCurPos )
{
	if( nCurPos >= m_nMinPos && nCurPos <= m_nMaxPos )
	{
		m_nCurPos = nCurPos;
		int		nNewWidth = m_nWidth - m_rcBar.GetWidth();
		FLOAT	fOffset = m_nCurPos * (FLOAT)( nNewWidth / (FLOAT)( m_nRange - 1 ) );
		int		nBarWidth = m_rcBar.GetWidth();
		m_rcBar.Left = fOffset;
		m_rcBar.Right = m_rcBar.Left + nBarWidth;
	}
}

// ----------------------------------------------------------------------------
// Name : SetRange()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::SetRange( int nRange )
{
	if( m_nMinPos + nRange <= (m_nMaxPos+1) )
		m_nRange = nRange;
}

// ----------------------------------------------------------------------------
// Name : SetMinPos()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::SetMinPos( int nMinPos )
{
	m_nMinPos = nMinPos;
	if( m_nCurPos < m_nMinPos )
	{
		m_nCurPos = m_nMinPos;
		SetCurPos( m_nCurPos );
	}
}

// ----------------------------------------------------------------------------
// Name : SetMaxPos()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::SetMaxPos( int nMaxPos )
{
	m_nMaxPos = nMaxPos;
	if( m_nCurPos > m_nMaxPos )
	{
		m_nCurPos = m_nMaxPos;
		SetCurPos( m_nCurPos );
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISlideBar::Render()
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Add render regions
	// Background
	pDrawPort->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight,
										m_rtBackground.U0, m_rtBackground.V0,
										m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );

	// Bar
	pDrawPort->AddTexture( nX + m_rcBar.Left, nY + m_rcBar.Top,
										nX + m_rcBar.Right, nY + m_rcBar.Bottom,
										m_rtBar.U0, m_rtBar.V0, m_rtBar.U1, m_rtBar.V1,
										0xFFFFFFFF );
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUISlideBar::MouseMessage( MSG *pMsg )
{
	// If slide bar is disabled
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
			// Bar button
			if( m_bSlideBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				AdjustSlidePos( nX );

				return WMSG_COMMAND;
			}

			if( IsInsideRect( nX, nY, m_rcSlide ) )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInsideRect( nX, nY, m_rcSlide ) )
			{
				// If slide bar is clicked
				if( IsInsideRect( nX, nY, m_rcBar ) )
					m_bSlideBarClick = TRUE;
				// If sliding region is clicked
				else
				{
					AdjustSlidePos( nX );
					return WMSG_COMMAND;
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			if( m_bSlideBarClick )
			{
				m_bSlideBarClick = FALSE;
				return WMSG_SUCCESS;
			}

			m_bSlideBarClick = FALSE;
		}
		break;
	}

	return WMSG_FAIL;
}

void CUISlideBar::OnRender( CDrawPort* pDraw )
{
	if( m_pTexData == NULL )
	{
#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
		return;
	}

	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	pDraw->InitTextureData( m_pTexData );
	// Add render regions
	// Background
	pDraw->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight,
		m_rtBackground.U0, m_rtBackground.V0,
		m_rtBackground.U1, m_rtBackground.V1,
		0xFFFFFFFF );

	// Bar
	pDraw->AddTexture( nX + m_rcBar.Left, nY + m_rcBar.Top,
		nX + m_rcBar.Right, nY + m_rcBar.Bottom,
		m_rtBar.U0, m_rtBar.V0, m_rtBar.U1, m_rtBar.V1,
		0xFFFFFFFF );

	pDraw->FlushRenderingQueue();

#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

void CUISlideBar::setBar( int nWidth, int nHeight )
{
	m_rcBar.Top = ( m_nHeight - nHeight ) / 2;
	m_rcBar.Bottom = m_rcBar.Top + nHeight;
	m_rcBar.Left = 0;
	m_rcBar.Right = nWidth;

	m_rcSlide.SetRect( 0, m_rcBar.Top, m_nWidth, m_rcBar.Bottom );
}

UIRectUV CUISlideBar::getBackUV()
{
	UIRectUV uv = m_rtBackground;

	std::string strImg = getTexString();
	if(m_pTexData)
	{
		float fTexW, fTexH;
		fTexW = m_pTexData->GetPixWidth();
		fTexH = m_pTexData->GetPixHeight();

		uv.U0 = uv.U0 * fTexW;
		uv.V0 = uv.V0 * fTexH;
		uv.U1 = uv.U1 * fTexW;
		uv.V1 = uv.V1 * fTexH;
	}

	return uv; 
}

UIRectUV CUISlideBar::getBarUV()
{
	UIRectUV uv = m_rtBar;

	std::string strImg = getTexString();
	if(m_pTexData)
	{
		float fTexW, fTexH;
		fTexW = m_pTexData->GetPixWidth();
		fTexH = m_pTexData->GetPixHeight();

		uv.U0 = uv.U0 * fTexW;
		uv.V0 = uv.V0 * fTexH;
		uv.U1 = uv.U1 * fTexW;
		uv.V1 = uv.V1 * fTexH;
	}

	return uv; 
}