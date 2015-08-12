#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIGuildNotice.h>
#include <Engine/Interface/UIQuickSlot.h>

extern CUIFontTextureManager	*_pUIFontTexMgr;

// ----------------------------------------------------------------------------
// Name : CUIGuildNotice()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIGuildNotice::CUIGuildNotice()
{
	
}

// ----------------------------------------------------------------------------
// Name : ~CUIGuildNotice()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIGuildNotice::~CUIGuildNotice()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuildNotice::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 212, 22 );	

	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// Set Box
	m_bxBackGroundBox.SetBoxUV(m_ptdBaseTexture,30,22,WRect(0,0,216,22));
	m_bxBackGroundBox2.SetBoxUV(m_ptdBaseTexture,20,11,WRect(0,22,216,45));	

	// 길드 공지 출력--------------------------------------------->>
	m_bxTopNotic.SetBoxUV(m_ptdBaseTexture,46,31,WRect(256,223,360,254));
	m_bxBottomNotice.SetBoxUV(m_ptdBaseTexture,50,28,WRect(360,193,512,253));

	// List box of Notice String
	m_lbGuildNoticeStr.Create( this, 35 , 83, 145, 116, _pUIFontTexMgr->GetLineHeight(), 13, 3, 2, TRUE );
	m_lbGuildNoticeStr.CreateScroll( TRUE, 0, 0, 9, 116, 9, 7, 0, 0, 10 );
	m_lbGuildNoticeStr.SetSelBar( 129 , _pUIFontTexMgr->GetLineHeight(), 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbGuildNoticeStr.SetOverColor( 0xF8E1B5FF );
	m_lbGuildNoticeStr.SetSelectColor( 0xF8E1B5FF );	
	// Up button
	m_lbGuildNoticeStr.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbGuildNoticeStr.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbGuildNoticeStr.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbGuildNoticeStr.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbGuildNoticeStr.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbGuildNoticeStr.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbGuildNoticeStr.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbGuildNoticeStr.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbGuildNoticeStr.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbGuildNoticeStr.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbGuildNoticeStr.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	

	// Close button
	m_btnCloseNotice.Create( this, _S( 191 , "확인" ) , 78, 226 , 63, 21 );	
	m_btnCloseNotice.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCloseNotice.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCloseNotice.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCloseNotice.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// -----------------------------------------------------------<<

}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuildNotice::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMaxI - GUILD_NOTICE_WIDTH , pixMaxJ - GUILD_NOTICE_HEIGHT - QUICKSLOT_HEIGHT);
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :// // 
// ----------------------------------------------------------------------------
void CUIGuildNotice::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIGuildNotice::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY;
	
	nX = m_nPosX;
	nY = m_nPosY;

	// Background
	m_bxBackGroundBox.SetBoxPos(WRect(nX,nY,nX+212,nY+22));
	m_bxBackGroundBox.Render();
	m_bxBackGroundBox2.SetBoxPos(WRect(nX,nY+22,nX+212,nY+257));
	m_bxBackGroundBox2.Render();

	m_bxTopNotic.SetBoxPos(WRect(nX+13,nY+30,nX+202,nY+61));
	m_bxTopNotic.Render();
	m_bxBottomNotice.SetBoxPos(WRect(nX+13,nY+61,nX+202,nY+223));
	m_bxBottomNotice.Render();	

	m_lbGuildNoticeStr.Render();

	m_btnCloseNotice.Render();

	pDrawPort->PutTextEx( m_strGuildName+CTString(" ")+_S(75, "길드" ), nX +22, nY+5);
	pDrawPort->PutTextExCX( _S(252, "공지사항" ), nX +106, nY+35 ,0x471C0AFF );

	// Render all elements
	pDrawPort->FlushRenderingQueue();
	// Flush all render text queue
	pDrawPort->EndTextEx();

}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIGuildNotice::MouseMessage( MSG *pMsg )
{
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
				CUIManager* pUIManager = CUIManager::getSingleton();

				pUIManager->SetMouseCursorInsideUIs();
				pUIManager->RearrangeOrder( UI_GUILD_NOTICE, TRUE );
				
				m_lbGuildNoticeStr.MouseMessage(pMsg);
				m_btnCloseNotice.MouseMessage(pMsg);
			}			

			// Move target information
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}
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

				if(m_btnCloseNotice.MouseMessage(pMsg) != WMSG_FAIL){
					// TODO : Nothing
				}
				else if(m_lbGuildNoticeStr.MouseMessage(pMsg) != WMSG_FAIL){
					// TODO : Nothing
				}				

				CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_NOTICE, TRUE );
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
					if(m_btnCloseNotice.MouseMessage(pMsg) != WMSG_FAIL){
						pUIManager->RearrangeOrder( UI_GUILD_NOTICE, FALSE );						
					}
					else if(m_lbGuildNoticeStr.MouseMessage(pMsg) != WMSG_FAIL){
					// TODO : Nothing
					}
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
	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if(m_lbGuildNoticeStr.MouseMessage(pMsg) != WMSG_FAIL)
				{
					// TODO : Nothing
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}

// Set Notice String
void CUIGuildNotice::SetGuildNotice( CTString guildName,CTString noticeTitle,CTString guildNotice)
{
	m_strGuildName = guildName;
	m_lbGuildNoticeStr.ResetAllStrings();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->AddStringToList(&m_lbGuildNoticeStr,noticeTitle,18,0x471C0AFF);
	pUIManager->AddStringToList(&m_lbGuildNoticeStr,guildNotice,18,0xFFFFFFFF);	
}