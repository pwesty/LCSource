#include "stdh.h"


#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIRankingClassify.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Contents/Base/UIRankingSystem.h>

extern INDEX  cli_iCurrentFPS;

CUIRankingClassify::CUIRankingClassify() : 
m_ptdImageToolTip(NULL),
m_bVisible(FALSE),
m_bVisibleImg(FALSE),
m_bSlideImgX(FALSE)
{
}

CUIRankingClassify::~CUIRankingClassify ()
{
	Destroy();

	STOCK_RELEASE( m_ptdImageToolTip );
		
	m_vecCtstringToolTip.clear();
}


void CUIRankingClassify::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 600, 22 );	
	
	m_ptdBaseTexture	= CreateTexture( CTString( "Data\\Interface\\WebBoard.tex" ) );
	m_ptdImageToolTip	= CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );

	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();


	
	// Close Button
	m_btnClose.Create( this, CTString( "" ), nWidth - 30, 3, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 108, 15, 122, 29,  fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 108, 0, 122, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	
	// List box of list type
	m_lbListContent.Create( this, 15, 42, nWidth - 30 , nHeight - 50, _pUIFontTexMgr->GetFontHeight() + 2, 0, 0, 1, TRUE );
	m_lbListContent.SetEnable(true);
	m_lbListContent.SetOverColor( 0x66B5FFFF );
	//	m_lbListContent.SetSelectColor( 0x66B5FFFF );
	//m_lbListContent.CreateScroll( TRUE, 0, 0, 9, 290, 9, 7, 0, 0, 10 );
	m_lbListContent.CreateScroll( TRUE, -7, -5, 9, nHeight - 50, 9, 7, 0, 0, 10 );
	// Scroll bar of list box
	// Up button
	m_lbListContent.SetScrollUpUV( UBS_IDLE, 46, 117, 55, 124, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollUpUV( UBS_CLICK, 56, 117, 65, 124, fTexWidth, fTexHeight );
	m_lbListContent.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbListContent.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbListContent.SetScrollBarTopUV( 90, 43, 99, 53, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollBarMiddleUV( 90, 53, 99, 56, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollBarBottomUV( 90, 56, 99, 67, fTexWidth, fTexHeight );
	// Down button
	m_lbListContent.SetScrollDownUV( UBS_IDLE, 66, 117, 75, 124, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollDownUV( UBS_CLICK, 76, 117, 85, 124, fTexWidth, fTexHeight );
	m_lbListContent.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbListContent.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Adjust column position of list box
	m_lbListContent.SetColumnPosX( 0, 5 , TEXT_LEFT );	
	
	
	// UV of each part
	m_rtTitleUL.SetUV( 0, 0, 49, 28, fTexWidth, fTexHeight );
	m_rtTitleUM.SetUV( 50, 0, 56, 28, fTexWidth, fTexHeight );
	m_rtTitleUR.SetUV( 57, 0, 89, 28, fTexWidth, fTexHeight );
	
	m_rtBackUL.SetUV( 0, 42, 49, 52, fTexWidth, fTexHeight );
	m_rtBackUM.SetUV( 50, 42, 56, 52, fTexWidth, fTexHeight );
	m_rtBackUR.SetUV( 57, 42, 89, 52, fTexWidth, fTexHeight );
	m_rtBackML.SetUV( 0, 53, 49, 58, fTexWidth, fTexHeight );
	m_rtBackMM.SetUV( 51, 54, 55, 57, fTexWidth, fTexHeight );
	m_rtBackMR.SetUV( 57, 53, 89, 58, fTexWidth, fTexHeight );
	m_rtBackLL.SetUV( 0, 59, 49, 74, fTexWidth, fTexHeight );
	m_rtBackLM.SetUV( 50, 59, 56, 74, fTexWidth, fTexHeight );
	m_rtBackLR.SetUV( 57, 59, 89, 74, fTexWidth, fTexHeight );
	
	
	// 툴팁 영역
	m_rtToolTipUL.SetUV( 0, 229, 8, 237, m_ptdImageToolTip->GetPixWidth(),  m_ptdImageToolTip->GetPixHeight() );
	m_rtToolTipUM.SetUV( 8, 229, 76, 237, m_ptdImageToolTip->GetPixWidth(),  m_ptdImageToolTip->GetPixHeight() );
	m_rtToolTipUR.SetUV( 76, 229, 84, 237, m_ptdImageToolTip->GetPixWidth(),  m_ptdImageToolTip->GetPixHeight() );
	m_rtToolTipML.SetUV( 0, 237, 8, 243, m_ptdImageToolTip->GetPixWidth(),  m_ptdImageToolTip->GetPixHeight() );
	m_rtToolTipMM.SetUV( 8, 237, 76, 243, m_ptdImageToolTip->GetPixWidth(),  m_ptdImageToolTip->GetPixHeight() );
	m_rtToolTipMR.SetUV( 76, 237, 84, 243, m_ptdImageToolTip->GetPixWidth(),  m_ptdImageToolTip->GetPixHeight() );
	m_rtToolTipLL.SetUV( 0, 243, 8, 251, m_ptdImageToolTip->GetPixWidth(),  m_ptdImageToolTip->GetPixHeight() );
	m_rtToolTipLM.SetUV( 8, 243, 76, 251, m_ptdImageToolTip->GetPixWidth(),  m_ptdImageToolTip->GetPixHeight() );
	m_rtToolTipLR.SetUV( 76, 243, 84, 251, m_ptdImageToolTip->GetPixWidth(),  m_ptdImageToolTip->GetPixHeight() );	
	

	AddListContents(5189, "최고 레벨");
	AddListContents(5190, "타이탄 최고 레벨");
	AddListContents(5191, "나이트 최고 레벨");
	AddListContents(5192,  "힐러 최고 레벨");
	AddListContents(5193,  "메이지 최고 레벨");
	AddListContents(5194,  "로그 최고 레벨");
	AddListContents(5195,  "소서러 최고 레벨");
	AddListContents(5196,  "나이트 쉐도우 최고 레벨");

	AddTooltip(5198, "가장 높은 레벨을 도달한 캐릭터");
	AddTooltip(5199, "가장 높은 레벨을 제외한 타이탄 최고 레벨 캐릭터");
	AddTooltip(5200, "가장 높은 레벨을 제외한 나이트 최고 레벨 캐릭터");
	AddTooltip(5201,  "가장 높은 레벨을 제외한 힐러 최고 레벨 캐릭터");
	AddTooltip(5202,  "가장 높은 레벨을 제외한 메이지 최고 레벨 캐릭터");
	AddTooltip(5203,  "가장 높은 레벨을 제외한 로그 최고 레벨 캐릭터");
	AddTooltip(5204,  "가장 높은 레벨을 제외한 소서러 최고 레벨 캐릭터");
	AddTooltip(5205,  "가장 높은 레벨을 제외한 나이트 쉐도우최고 레벨 캐릭터");
}


WMSG_RESULT	 CUIRankingClassify::MouseMessage(MSG *pMsg)
{
	WMSG_RESULT	wmsgResult = WMSG_FAIL;
	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	if(!m_bVisible) 
		return WMSG_FAIL;
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();				
			

			// Move web board
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// List box
			else if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				ShowToolTip(TRUE, m_lbListContent.GetCurOverList(), nX, nY );
				return WMSG_SUCCESS;			
			}
			ShowToolTip(false, 0);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				SetFocus ( TRUE );
				CUIManager::getSingleton()->RearrangeOrder( UI_RANKINGCLASSIFY, TRUE );

				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// List box
				if( ( wmsgResult = m_lbListContent.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{						
						ShowSelectList( m_lbListContent.GetCurSel());
					}
				}				
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

				// If teleport isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						ToggleVisible();

					return WMSG_SUCCESS;
				}
				// List box
				else if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
				{					
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
			{
				// DO NOTTHING
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// List box of read type
				if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;									
			}
		}
		break;
	}
	
	return WMSG_FAIL;
}

/*
WMSG_RESULT	CUIRankingClassify::KeyMessage(struct tagMSG *)
{
	// If web board is not focused
	if( !IsFocused() )
		return WMSG_FAIL;
	
	return WMSG_SUCCESS;
}
/**/

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingClassify::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingClassify::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIRankingClassify::Render(void)
{	
	m_bVisible = true;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set web board texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	int	nX, nY, nX2, nY2;
	// Background
	// Upper left
	nX = m_nPosX;
	nY = m_nPosY;
	nX2 = m_nPosX + m_nWidth;
	nY2 = m_nPosY + 28;

	pDrawPort->AddTexture( nX, nY, nX + 49, nY2,
		m_rtTitleUL.U0, m_rtTitleUL.V0, m_rtTitleUL.U1, m_rtTitleUL.V1,
		0xFFFFFFFF );
	// Upper middle
	pDrawPort->AddTexture( nX + 49, nY, nX2 - 32, nY2,
		m_rtTitleUM.U0, m_rtTitleUM.V0, m_rtTitleUM.U1, m_rtTitleUM.V1,
		0xFFFFFFFF );
	// Upper right
	pDrawPort->AddTexture( nX2 - 32, nY, nX2, nY2,
		m_rtTitleUR.U0, m_rtTitleUR.V0, m_rtTitleUR.U1, m_rtTitleUR.V1,
		0xFFFFFFFF );
	
	nY = nY2;
	nY2 = nY2 + 10;
	
	pDrawPort->AddTexture( nX, nY, nX + 49, nY2,
		m_rtBackUL.U0, m_rtBackUL.V0, m_rtBackUL.U1, m_rtBackUL.V1,
		0xFFFFFFFF );
	// Upper middle
	pDrawPort->AddTexture( nX + 49, nY, nX2 - 32, nY2,
		m_rtBackUM.U0, m_rtBackUM.V0, m_rtBackUM.U1, m_rtBackUM.V1,
		0xFFFFFFFF );
	// Upper right
	pDrawPort->AddTexture( nX2 - 32, nY, nX2, nY2,
		m_rtBackUR.U0, m_rtBackUR.V0, m_rtBackUR.U1, m_rtBackUR.V1,
		0xFFFFFFFF );
	
	// Middle left
	nY = m_nPosY + m_nHeight - 15;
	pDrawPort->AddTexture( nX, nY2, nX + 49, nY,
		m_rtBackML.U0, m_rtBackML.V0, m_rtBackML.U1, m_rtBackML.V1,
		0xFFFFFFFF );
	// Middle middle
	pDrawPort->AddTexture( nX + 49, nY2, nX2 - 32, nY,
		m_rtBackMM.U0, m_rtBackMM.V0, m_rtBackMM.U1, m_rtBackMM.V1,
		0xFFFFFFFF );
	// Middle right
	pDrawPort->AddTexture( nX2 - 32, nY2, nX2, nY,
		m_rtBackMR.U0, m_rtBackMR.V0, m_rtBackMR.U1, m_rtBackMR.V1,
		0xFFFFFFFF );
	
	// Lower left
	nY2 = m_nPosY + m_nHeight;
	pDrawPort->AddTexture( nX, nY, nX + 49, nY2,
		m_rtBackLL.U0, m_rtBackLL.V0, m_rtBackLL.U1, m_rtBackLL.V1,
		0xFFFFFFFF );
	// Lower middle
	pDrawPort->AddTexture( nX + 49, nY, nX2 - 32, nY2,
		m_rtBackLM.U0, m_rtBackLM.V0, m_rtBackLM.U1, m_rtBackLM.V1,
		0xFFFFFFFF );
	// Lower right
	pDrawPort->AddTexture( nX2 - 32, nY, nX2, nY2,
		m_rtBackLR.U0, m_rtBackLR.V0, m_rtBackLR.U1, m_rtBackLR.V1,
		0xFFFFFFFF );	
	
	// Close button
	m_btnClose.Render();
	
	RenderList();		
	// Flush all render queue
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();	

	RenderToolTip();
	
	// Title
	pDrawPort->PutTextEx( _S( 5213, "랭킹 목록" ), m_nPosX + 25,
		m_nPosY + 5, 0xFFFFFFFF );	
	
	pDrawPort->EndTextEx();
}


void CUIRankingClassify::RenderToolTip()
{
	// Tool tip
	if( m_bShowToolTip == FALSE )
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	
	// Set texture
	pDrawPort->InitTextureData( m_ptdImageToolTip );

	pDrawPort->AddTexture( m_rcToolTip.Left, m_rcToolTip.Top,
		m_rcToolTip.Left + 8, m_rcToolTip.Top + 8,
		m_rtToolTipUL.U0, m_rtToolTipUL.V0, m_rtToolTipUL.U1, m_rtToolTipUL.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcToolTip.Left + 8, m_rcToolTip.Top,
		m_rcToolTip.Right - 8, m_rcToolTip.Top + 8,
		m_rtToolTipUM.U0, m_rtToolTipUM.V0, m_rtToolTipUM.U1, m_rtToolTipUM.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcToolTip.Right - 8, m_rcToolTip.Top,
		m_rcToolTip.Right, m_rcToolTip.Top + 8,
		m_rtToolTipUR.U0, m_rtToolTipUR.V0, m_rtToolTipUR.U1, m_rtToolTipUR.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcToolTip.Left, m_rcToolTip.Top + 8,
		m_rcToolTip.Left + 8, m_rcToolTip.Bottom - 8,
		m_rtToolTipML.U0, m_rtToolTipML.V0, m_rtToolTipML.U1, m_rtToolTipML.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcToolTip.Left + 8, m_rcToolTip.Top + 8,
		m_rcToolTip.Right - 8, m_rcToolTip.Bottom - 8,
		m_rtToolTipMM.U0, m_rtToolTipMM.V0, m_rtToolTipMM.U1, m_rtToolTipMM.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcToolTip.Right - 8, m_rcToolTip.Top + 8,
		m_rcToolTip.Right, m_rcToolTip.Bottom - 8,
		m_rtToolTipMR.U0, m_rtToolTipMR.V0, m_rtToolTipMR.U1, m_rtToolTipMR.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcToolTip.Left, m_rcToolTip.Bottom - 8,
		m_rcToolTip.Left + 8, m_rcToolTip.Bottom,
		m_rtToolTipLL.U0, m_rtToolTipLL.V0, m_rtToolTipLL.U1, m_rtToolTipLL.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcToolTip.Left + 8, m_rcToolTip.Bottom - 8,
		m_rcToolTip.Right - 8, m_rcToolTip.Bottom,
		m_rtToolTipLM.U0, m_rtToolTipLM.V0, m_rtToolTipLM.U1, m_rtToolTipLM.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcToolTip.Right - 8, m_rcToolTip.Bottom - 8,
		m_rcToolTip.Right, m_rcToolTip.Bottom,
		m_rtToolTipLR.U0, m_rtToolTipLR.V0, m_rtToolTipLR.U1, m_rtToolTipLR.V1,
		0xFFFFFFBB );

	// Render all elements
	pDrawPort->FlushRenderingQueue();
	
	// Text in tool tip
	pDrawPort->PutTextEx( m_cszCurToolTip, m_rcToolTip.Left + 8, m_rcToolTip.Top + 3, m_colorCurToolTip );		
	
	// Flush all render text queue
	pDrawPort->EndTextEx();
}

void CUIRankingClassify::RenderList ()
{
	m_lbListContent.Render();
}

// ----------------------------------------------------------------------------
// Name : OpenHelp()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingClassify::Open()
{
	CUIManager::getSingleton()->RearrangeOrder( UI_RANKINGCLASSIFY, TRUE );
}

// ----------------------------------------------------------------------------
// Name : CloseHelp()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingClassify::Close()
{
	CUIManager::getSingleton()->RearrangeOrder( UI_RANKINGCLASSIFY, FALSE );
}


void CUIRankingClassify::AddListContents ( int iStringCode, CTString cstrName, COLOR cColor )
{
	m_lbListContent.AddString(0, _S(iStringCode, cstrName), cColor );
}

void CUIRankingClassify::AddTooltip ( int iStringCode, CTString cstrName, COLOR cColor )
{
	stToolTip Temp; 
	Temp.color		= cColor;
	Temp.ctszrName	= _S(iStringCode, cstrName);
	m_vecCtstringToolTip.push_back( Temp );
}

void CUIRankingClassify::ShowToolTip ( bool bShow,  int iIndex, int x, int y)
{
	static int	nOldToolTipID = -1;
	
	if( !bShow )
	{
		nOldToolTipID = -1;
		m_bShowToolTip = FALSE;
		return;
	}
	
	
	if( nOldToolTipID != iIndex && iIndex >= 0 && iIndex < m_lbListContent.GetCurItemCount(0) )
	{
		m_bShowToolTip = TRUE;

		int	nInfoX = 0, nInfoY = 0;
		
		nOldToolTipID		= iIndex;
		m_cszCurToolTip		= m_vecCtstringToolTip[iIndex].ctszrName;
		m_colorCurToolTip	= m_vecCtstringToolTip[iIndex].color;
		int	nInfoWidth		= _pUIFontTexMgr->GetFontSpacing() + m_cszCurToolTip.Length() *
							( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) + 14;
		int	nInfoHeight = _pUIFontTexMgr->GetFontHeight() + 8;
		
		nInfoX = x - (nInfoWidth/2);
		nInfoY = y - nInfoHeight - 5;

		CUIManager* pUIManager = CUIManager::getSingleton();

		if( nInfoX + nInfoWidth > pUIManager->GetMaxI() )
			nInfoX = pUIManager->GetMaxI() - nInfoWidth;
		
		m_rcToolTip.SetRect( nInfoX, nInfoY, nInfoX + nInfoWidth, nInfoY + nInfoHeight );
	}
}

void CUIRankingClassify::ShowSelectList ( int iIndex )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->GetRankingViewEx()->OpenUI((ERANKINGEX_SUB_TYPE) iIndex);
}


void CUIRankingClassify::ToggleVisible()
{
	if( IsVisible() )
	{
		Close();
		m_bVisible = FALSE;
		return;
	}
	Open();
	m_bVisible = TRUE;
}

const int CUIRankingClassify::RankingClietTypeToUIIndex ( int nType )
{
	return -1;
}

const int CUIRankingClassify::RankingServerTypeToUIIndex ( int nType )
{
	return -1;
}


const int CUIRankingClassify::RankingClientTypeToServer( int nType )
{
	switch(nType)
	{		
	case RANK_BEST:
		{
			return 	SERVER_RANK_BEST;			
		}
	case RANK_TITAN:
		{
			return SERVER_RANK_TITAN;
		}
	case RANK_KNIGHT:
		{
			return SERVER_RANK_KNIGHT;
		}
	case RANK_HEALER:
		{
			return SERVER_RANK_HEALER;			
		}
	case RANK_MAGE:
		{
			return SERVER_RANK_MAGE;				
		}
	case RANK_ROGUE:
		{
			return SERVER_RANK_ROGUE;			
		}
	case RANK_SORCERER:
		{
			return SERVER_RANK_SORCERER;				
		}
	case RANK_NIGHTSHADOW:
		{
			return SERVER_RANK_NIGHTSHADOW;	
		}
#ifdef CHAR_EX_ROGUE
	case RANK_EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
		{
			return SERVER_RANK_EX_ROGUE;	
		}
#endif
#ifdef CHAR_EX_MAGE		//2013/01/08 jeil EX메이지 추가 
	case RANK_EX_MAGE:
		{
			return SERVER_RANK_EX_MAGE;
		}
#endif
	case RANK_GUARDIAN:
		{
			return SERVER_RANK_GUARDIAN;	
		}
	default:
		return -1;
	}
}

const int CUIRankingClassify::RankingServerTypeToClient( int nType )
{
	switch(nType)
	{		
	case SERVER_RANK_BEST:
		{
			return 	RANK_BEST;			
		}
	case SERVER_RANK_TITAN:
		{
			return RANK_TITAN;
		}
	case SERVER_RANK_KNIGHT:
		{
			return RANK_KNIGHT;
		}
	case SERVER_RANK_HEALER:
		{
			return RANK_HEALER;			
		}
	case SERVER_RANK_MAGE:
		{
			return RANK_MAGE;				
		}
	case SERVER_RANK_ROGUE:
		{
			return RANK_ROGUE;			
		}
	case SERVER_RANK_SORCERER:
		{
			return RANK_SORCERER;				
		}
	case SERVER_RANK_NIGHTSHADOW:
		{
			return RANK_NIGHTSHADOW;	
		}
#ifdef CHAR_EX_ROGUE
	case SERVER_RANK_EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
		{
			return RANK_EX_ROGUE;	
		}
#endif
#ifdef CHAR_EX_MAGE		//2013/01/08 jeil EX메이지 추가 
	case SERVER_RANK_EX_MAGE:
		{
			return RANK_EX_MAGE;
		}
#endif
	case SERVER_RANK_GUARDIAN:
		{
			return RANK_GUARDIAN;	
		}
	default:
		return -1;
	}
}

const int CUIRankingClassify::RankingUIIndexToServer ( int nType )
{
	return -1;
}
