#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIHelpIcon.h>
#include <Engine/LocalDefine.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Contents/function/HelpWebUI.h>

extern INDEX g_iShowHelp1Icon;

// ----------------------------------------------------------------------------
// Name : CUIHelpIcon()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIHelpIcon::CUIHelpIcon()
{

}

// ----------------------------------------------------------------------------
// Name : ~CUIHelpIcon()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIHelpIcon::~CUIHelpIcon()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpIcon::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 14, 44 );
	m_rcBack.SetRect( 14, 0, 49, 44 );
	m_rcMark.SetRect( 0, 0, 49, 44 );
	m_rcLineV.SetRect( 49, 0, 52, 44 );

	// Create inventory texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\QuickSlot.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Title UV
	m_rtTitle.SetUV( 0, 64, 13, 107, fTexWidth, fTexHeight );
	// Background UV
	m_rtBack.SetUV( 67, 64, 103, 107, fTexWidth, fTexHeight );
		
	// Help Button UV
	m_rtHelpIdle.SetUV( 70, 0, 104, 34, fTexWidth, fTexHeight );
	m_rtHelpClick.SetUV( 105, 0, 139, 34, fTexWidth, fTexHeight );

	// Vertical Line 
	m_rtLineV.SetUV( 474,64,477,107, fTexWidth, fTexHeight );

	// Help Button Create
	m_btnHelp.Create( this, CTString(""), 16, 6, 32, 32 );
	m_btnHelp.CopyUV( UBS_IDLE, m_rtHelpIdle );
	m_btnHelp.CopyUV( UBS_CLICK, m_rtHelpClick );
	m_btnHelp.CopyUV( UBS_IDLE, UBS_ON );
	m_btnHelp.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	m_bShowBack = FALSE;	

}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpIcon::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMaxI - m_nWidth - RADAR_WIDTH, pixMinJ );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpIcon::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpIcon::Render()
{	
	if( g_iShowHelp1Icon)
	{
		RenderIcon();
	}
}

// ----------------------------------------------------------------------------
// Name : RenderInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelpIcon::RenderIcon()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	if(m_bShowBack)
	{
	// Title region
	pDrawPort->AddTexture( m_nPosX + m_rcTitle.Left, m_nPosY +m_rcTitle.Top,
										m_nPosX + m_rcTitle.Right, m_nPosY +m_rcTitle.Bottom,
										m_rtTitle.U0, m_rtTitle.V0, m_rtTitle.U1, m_rtTitle.V1,
										0xFFFFFFFF );
	// Background region
	pDrawPort->AddTexture( m_nPosX + m_rcBack.Left, m_nPosY + m_rcBack.Top,
										m_nPosX + m_rcBack.Right, m_nPosY + m_rcBack.Bottom,
										m_rtBack.U0, m_rtBack.V0, m_rtBack.U1, m_rtBack.V1,
										0xFFFFFFFF );
	// Vertical Line
	pDrawPort->AddTexture( m_nPosX + m_rcLineV.Left, m_nPosY + m_rcLineV.Top,
										m_nPosX + m_rcLineV.Right, m_nPosY + m_rcLineV.Bottom,
										m_rtLineV.U0, m_rtLineV.V0, m_rtLineV.U1, m_rtLineV.V1,
										0xFFFFFFFF );
	}

	m_btnHelp.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIHelpIcon::MouseMessage( MSG *pMsg )
{
	if (!g_iShowHelp1Icon)
	{
		return WMSG_FAIL;
	}
	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
			}

			m_bShowBack = IsInsideRect(nX,nY,m_rcMark);
			
			// Move target information
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}

			m_btnHelp.MouseMessage(pMsg);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}

				if(m_btnHelp.MouseMessage(pMsg) != WMSG_FAIL){
					// TODO : Nothing
				};

				CUIManager::getSingleton()->RearrangeOrder( UI_HELP_ICON, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;

				if( IsInside( nX, nY ) )
				{
					if(m_btnHelp.MouseMessage(pMsg) == WMSG_COMMAND)
					{
						pUIManager->GetHelpWebUI()->OpenUI();
					};
					return WMSG_SUCCESS;
				}

				

			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}
