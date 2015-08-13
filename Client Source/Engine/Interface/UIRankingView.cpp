#include "stdh.h"


#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIRankingView.h>
#include <Engine/Interface/UIRankingClassify.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIListBoxMultiList.h>
extern INDEX  cli_iCurrentFPS;

const UBYTE BOTTOM_GAP = 11;
const UBYTE MIDDLE_X_GAP = 4;
CUIRankingView::CUIRankingView( int nClassID ) : 
m_iImageIdx(-1),
m_bVisible(FALSE),
m_nTitleStringCode(-1),
m_iClassID(-1),
m_iPageIndex(0),
m_iListCount(0),
m_nListHight(0),
//m_nTitleHeight(28),
m_nTitleHeight(43),
m_nSubTitleHeight(25),
m_nBtnSpace(41), 
m_nTitleSubTitleGap(5),
//m_nBoarderListGap(49)
m_nBoarderListGap(191)
//m_ptdImageScroll(NULL)
{
	m_RankColor[0] = 0xff0000ff;
	m_RankColor[1] = 0x00ff00ff;
	m_RankColor[2] = 0xfa8800ff;
	m_RankColor[3] = 0x03aaaaff;
	m_RankColor[4] = 0xffff00ff;
}

CUIRankingView::~CUIRankingView ()
{
	m_mapSubTitle.clear();
	Destroy();
	/*
	if( m_ptdImageScroll )
	{
		_pTextureStock->Release( m_ptdImageScroll );
		m_ptdImageScroll = NULL;
	}
	/**/
}


void CUIRankingView::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, int iID )
{
	m_iClassID	= iID;
	m_pParent = (CUIBase*)pParentWnd;

	SetPos( nX, nY );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, nWidth, m_nTitleHeight );
	
	// Create web board texture		
	m_ptdBaseTexture	= CreateTexture( CTString( "Data\\Interface\\new_interface.tex" ) );	
	//m_ptdImageScroll	= CreateTexture( CTString( "Data\\Interface\\WebBoard.tex" ) );

	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();	

	// List box of list type
	int nColCountPerPage	= 10;
	int nSpaceY				= 2;
	int nLineHight			= _pUIFontTexMgr->GetFontHeight() + nSpaceY;
	m_nListHight			= (nLineHight) * nColCountPerPage;


	
	m_lbListContent.Create( this, 15, m_nTitleHeight + m_nSubTitleHeight, nWidth - 40 , m_nListHight, nLineHight , 0, nSpaceY, 10, 4, TRUE );	

	m_lbListContent.SetOverColor( 0x66B5FFFF );
	
	m_lbListContent.CreateScroll( TRUE, 2, 0, 9, m_nListHight, 9, 7, 0, 0, 5 );
	
	// Scroll bar of list box
	// Up button
	//m_lbListContent.SetScrollUpUV( UBS_IDLE, 46, 117, 55, 124, fTexWidth, fTexHeight );
	//m_lbListContent.SetScrollUpUV( UBS_CLICK, 56, 117, 65, 124, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollUpUV( UBS_IDLE, 134, 439, 144, 449, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollUpUV( UBS_CLICK, 146, 439, 156, 449, fTexWidth, fTexHeight );
	m_lbListContent.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbListContent.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	//m_lbListContent.SetScrollBarTopUV( 90, 43, 99, 53, fTexWidth, fTexHeight );
	//m_lbListContent.SetScrollBarMiddleUV( 90, 54, 99, 56, fTexWidth, fTexHeight );
	//m_lbListContent.SetScrollBarBottomUV( 90, 57, 99, 67, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollBarTopUV( 163, 438, 173, 446, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollBarMiddleUV( 163, 446, 173, 470, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollBarBottomUV( 163, 470, 173, 478, fTexWidth, fTexHeight );
	// Down button
	//m_lbListContent.SetScrollDownUV( UBS_IDLE, 66, 117, 75, 124, fTexWidth, fTexHeight );
	//m_lbListContent.SetScrollDownUV( UBS_CLICK, 76, 117, 85, 124, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollDownUV( UBS_IDLE, 134, 451, 144, 461, fTexWidth, fTexHeight );
	m_lbListContent.SetScrollDownUV( UBS_CLICK, 146, 451, 156, 461, fTexWidth, fTexHeight );
	m_lbListContent.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbListContent.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Adjust column position of list box
	//m_lbListContent.SetColumnPosX( 0, 5 , TEXT_LEFT );	
	
	int nTempHeight = m_nListHight + m_nSubTitleHeight + m_nTitleHeight + m_nBtnSpace + m_nTitleSubTitleGap;
	SetSize( nWidth, nTempHeight );
	

	// Close Button
	m_btnClose.Create( this, CTString( "" ), nWidth - 30, 3, 14, 14 );
	//m_btnClose.SetUV( UBS_IDLE, 108, 0, 122, 14, fTexWidth , fTexHeight );
	//m_btnClose.SetUV( UBS_CLICK, 108, 15, 122, 29, fTexWidth, fTexHeight );	
	m_btnClose.SetUV( UBS_IDLE, 268, 457, 284, 473, fTexWidth , fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 286, 457, 302, 473, fTexWidth, fTexHeight );	
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// More Button 
	CTString ctrMore =  _S( 5208, "순위 더 보기" );
	int nMoreSize = _pUIFontTexMgr->GetFontWidth() * ctrMore.Length();
	m_btnMore.Create( this, ctrMore, 30, nTempHeight - m_nBtnSpace+ 5, nMoreSize + 20, 21 );
	//m_btnMore.SetUV( UBS_IDLE, 0, 94, 62, 114, fTexWidth , fTexHeight );
	//m_btnMore.SetUV( UBS_CLICK, 64, 94, 126, 114, fTexWidth, fTexHeight );
	m_btnMore.SetUV( UBS_IDLE, 282, 244, 352, 266, fTexWidth , fTexHeight );
	m_btnMore.SetUV( UBS_CLICK, 364, 244, 434, 266, fTexWidth, fTexHeight );
	m_btnMore.CopyUV( UBS_IDLE, UBS_ON );
	m_btnMore.CopyUV( UBS_IDLE, UBS_DISABLE );	
	
	// 취소 버튼 	
	ctrMore =  _S( 870, "닫기" );
	nMoreSize = _pUIFontTexMgr->GetFontWidth() * ctrMore.Length();
	m_btnCancel.Create( this, _S( 870, "닫기" ), nWidth - 100, nTempHeight - m_nBtnSpace + 5, 63, 21 );
	//m_btnCancel.SetUV( UBS_IDLE, 0, 94, 62, 114, fTexWidth , fTexHeight );
	//m_btnCancel.SetUV( UBS_CLICK, 64, 94, 126, 114, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_IDLE, 282, 244, 352, 266, fTexWidth , fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 364, 244, 434, 266, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );	


	// UV of each part
	//m_rtTitleUL.SetUV( 0, 0, 49, 28, fTexWidth, fTexHeight );
	//m_rtTitleUM.SetUV( 50, 0, 56, 28, fTexWidth, fTexHeight );
	//m_rtTitleUR.SetUV( 40, 0, 89, 28, fTexWidth, fTexHeight );
	m_rtTitleUL.SetUV( 428, 0, 619, 43, fTexWidth, fTexHeight );
	m_rtTitleUM.SetUV( 619, 0, 752, 43, fTexWidth, fTexHeight );
	m_rtTitleUR.SetUV( 752, 0, 943, 43, fTexWidth, fTexHeight );
	
	//m_rtBackUL.SetUV( 0, 42, 49, 52, fTexWidth, fTexHeight );
	//m_rtBackUM.SetUV( 50, 42, 56, 52, fTexWidth, fTexHeight );
	//m_rtBackUR.SetUV( 40, 42, 89, 52, fTexWidth, fTexHeight );
	m_rtBackUL.SetUV( 428, 51, 432, 55, fTexWidth, fTexHeight );
	m_rtBackUM.SetUV( 440, 51, 450, 55, fTexWidth, fTexHeight );
	m_rtBackUR.SetUV( 939, 51, 943, 55, fTexWidth, fTexHeight );
	
	

	//m_rtBackML.SetUV( 0, 43, 49, 48, fTexWidth, fTexHeight );
	//m_rtBackMM.SetUV( 51, 44, 55, 47, fTexWidth, fTexHeight );
	//m_rtBackMR.SetUV( 40, 43, 89, 48, fTexWidth, fTexHeight );
	m_rtBackML.SetUV( 428, 51, 432, 55, fTexWidth, fTexHeight );
	m_rtBackMM.SetUV( 440, 51, 450, 55, fTexWidth, fTexHeight );
	m_rtBackMR.SetUV( 939, 51, 943, 55, fTexWidth, fTexHeight );

	
	//m_rtBackLL.SetUV( 0, 63, 49, 75, fTexWidth, fTexHeight );
	//m_rtBackLM.SetUV( 50, 63, 56, 75, fTexWidth, fTexHeight );
	//m_rtBackLR.SetUV( 40, 63, 89, 75, fTexWidth, fTexHeight );	
	m_rtBackLL.SetUV( 428, 149, 439, 160, fTexWidth, fTexHeight );
	m_rtBackLM.SetUV( 450, 149, 455, 160, fTexWidth, fTexHeight );
	m_rtBackLR.SetUV( 932, 149, 943, 160, fTexWidth, fTexHeight );	
	

	m_rtInnerRectML.SetUV( 447, 240, 617, 272, fTexWidth, fTexHeight );	
	//m_rtInnerRectML.SetUV( 2, 258, 234, 298, fTexWidth, fTexHeight );
	//m_rtInnerRectML.SetUV( 447, 173, 923, 225, fTexWidth, fTexHeight );
	
	m_rtHorizonLine.SetUV( 297, 286, 298, 287, fTexWidth, fTexHeight );
	m_rtVerticalLine.SetUV( 297, 286, 298, 287, fTexWidth, fTexHeight );
	
}


WMSG_RESULT	 CUIRankingView::MouseMessage(MSG *pMsg)
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
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL || m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL  )
				return WMSG_SUCCESS;
			// List box
			else if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
			{				
				return WMSG_SUCCESS;			
			}			
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				SetFocus ( TRUE );
				CUIManager::getSingleton()->RearrangeOrder( m_iClassID, TRUE );

				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL || m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					//ToggleVisible();
				}
				else if ( m_btnMore.MouseMessage( pMsg ) != WMSG_FAIL )
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
						/*
						if ( m_pViewManager )
							m_pViewManager->SetSelectView( m_lbListContent.GetCurSel() );
							/**/
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
				WMSG_RESULT wmsgResult1 = m_btnCancel.MouseMessage( pMsg );
				wmsgResult = m_btnClose.MouseMessage( pMsg );
				if( wmsgResult != WMSG_FAIL ||  wmsgResult1 != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND || WMSG_COMMAND == wmsgResult1 )
						ToggleVisible();

					return WMSG_SUCCESS;
				}
				// List box
				else if( m_lbListContent.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if ( ( wmsgResult = m_btnMore.MouseMessage( pMsg )) != WMSG_FAIL )
				{
					if ( WMSG_COMMAND == wmsgResult )
					{
						SendNextPage();						
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

//#endif
	
	return WMSG_FAIL;
}


/*
WMSG_RESULT	CUIRankingView::KeyMessage( MSG *pMsg  )
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
void CUIRankingView::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingView::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIRankingView::Render(void)
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set web board texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	int	nX, nY, nX2, nY2;
	// Background
	
	nX = m_nPosX;
	nY = m_nPosY;
	nX2 = m_nPosX + m_nWidth;
	nY2 = m_nPosY + m_nTitleHeight;
	
	//타이틀 
	// Upper left
	pDrawPort->AddTexture( nX, nY, nX + m_nBoarderListGap, nY2,
		m_rtTitleUL.U0, m_rtTitleUL.V0, m_rtTitleUL.U1, m_rtTitleUL.V1,
		0xFFFFFFFF );
	// Upper middle
	pDrawPort->AddTexture( nX + m_nBoarderListGap, nY, nX2 - m_nBoarderListGap, nY2,
		m_rtTitleUM.U0, m_rtTitleUM.V0, m_rtTitleUM.U1, m_rtTitleUM.V1,
		0xFFFFFFFF );
	// Upper right
	pDrawPort->AddTexture( nX2 - m_nBoarderListGap, nY, nX2, nY2,
		m_rtTitleUR.U0, m_rtTitleUR.V0, m_rtTitleUR.U1, m_rtTitleUR.V1,
		0xFFFFFFFF );
	
	nY = nY2;
	nY2 = nY ;
	
	// 서브 타이틀 
	// upper left
	pDrawPort->AddTexture( nX, nY, nX + MIDDLE_X_GAP, nY2+1,
		m_rtBackUL.U0, m_rtBackUL.V0, m_rtBackUL.U1, m_rtBackUL.V1,
		0xFFFFFFFF );		
	// Upper middle
	pDrawPort->AddTexture( nX + m_nBoarderListGap, nY, nX2 - MIDDLE_X_GAP, nY2+1,
		m_rtBackUM.U0, m_rtBackUM.V0, m_rtBackUM.U1, m_rtBackUM.V1,
		0xFFFFFFFF );
	// Upper right
	pDrawPort->AddTexture( nX2 - MIDDLE_X_GAP, nY, nX2, nY2+1,
		m_rtBackUR.U0, m_rtBackUR.V0, m_rtBackUR.U1, m_rtBackUR.V1,
		0xFFFFFFFF );
	
	//중간 틀 리스트 
	// Middle left
	nY = nY2 + m_nSubTitleHeight + m_nListHight + m_nBtnSpace - BOTTOM_GAP;
	pDrawPort->AddTexture( nX, nY2, nX + MIDDLE_X_GAP, nY,
		m_rtBackML.U0, m_rtBackML.V0, m_rtBackML.U1, m_rtBackML.V1,
		0xFFFFFFFF );
	// Middle middle
	pDrawPort->AddTexture( nX + MIDDLE_X_GAP, nY2, nX2 - MIDDLE_X_GAP, nY,
		m_rtBackMM.U0, m_rtBackMM.V0, m_rtBackMM.U1, m_rtBackMM.V1,
		0xFFFFFFFF );
	// Middle right
	pDrawPort->AddTexture( nX2 - MIDDLE_X_GAP, nY2, nX2, nY,
		m_rtBackMR.U0, m_rtBackMR.V0, m_rtBackMR.U1, m_rtBackMR.V1,
		0xFFFFFFFF );
	
	//아랬쪽 틀
	// Lower left
	nY2 = nY + BOTTOM_GAP;
	pDrawPort->AddTexture( nX, nY, nX + BOTTOM_GAP, nY2,
		m_rtBackLL.U0, m_rtBackLL.V0, m_rtBackLL.U1, m_rtBackLL.V1,
		0xFFFFFFFF );
	// Lower middle
	pDrawPort->AddTexture( nX + BOTTOM_GAP, nY, nX2 - BOTTOM_GAP, nY2,
		m_rtBackLM.U0, m_rtBackLM.V0, m_rtBackLM.U1, m_rtBackLM.V1,
		0xFFFFFFFF );
	// Lower right
	pDrawPort->AddTexture( nX2 - BOTTOM_GAP, nY, nX2, nY2,
		m_rtBackLR.U0, m_rtBackLR.V0, m_rtBackLR.U1, m_rtBackLR.V1,
		0xFFFFFFFF );		
	
	
	int nXGap = 15;
	int nYGap = 5;
	
	nX = m_nPosX + 11;
	nX2 = m_nPosX + m_nWidth - 12;
	nY = m_nPosY + m_nTitleHeight; 
	nY2 = nY + m_nListHight + m_nSubTitleHeight;
	
	//내부 바탕 깔기 
	pDrawPort->AddTexture( nX , nY , nX + m_nGap1, nY2,
		m_rtInnerRectML.U0, m_rtInnerRectML.V0, m_rtInnerRectML.U1, m_rtInnerRectML.V1,
		0xFFFFFFFF );	
		pDrawPort->AddTexture( nX + m_nGap1, nY , nX + m_nGap2, nY2,
		m_rtInnerRectML.U0, m_rtInnerRectML.V0, m_rtInnerRectML.U1, m_rtInnerRectML.V1,
		0xFFFFFFFF );	
		pDrawPort->AddTexture( nX + m_nGap2, nY , nX + m_nGap3, nY2,
		m_rtInnerRectML.U0, m_rtInnerRectML.V0, m_rtInnerRectML.U1, m_rtInnerRectML.V1,
		0xFFFFFFFF );	
		pDrawPort->AddTexture( nX + m_nGap3 , nY , nX + m_nGap4, nY2,
		m_rtInnerRectML.U0, m_rtInnerRectML.V0, m_rtInnerRectML.U1, m_rtInnerRectML.V1,
		0xFFFFFFFF );	

	//칼럼 타이틀 리스트 구분선 
	//////////////////////////////////////////////////////////////////////////
	pDrawPort->AddTexture( nX , nY  , nX2, nY + 1 ,
		m_rtHorizonLine.U0, m_rtHorizonLine.V0, m_rtHorizonLine.U1, m_rtHorizonLine.V1,
		0xFFFFFFFF );

	//서브 타이틀 구분선 
	//////////////////////////////////////////////////////////////////////////
	pDrawPort->AddTexture( nX , nY + m_nSubTitleHeight , nX2, nY +m_nSubTitleHeight + 1 ,
		m_rtHorizonLine.U0, m_rtHorizonLine.V0, m_rtHorizonLine.U1, m_rtHorizonLine.V1,
		0xFFFFFFFF );
	//////////////////////////////////////////////////////////////////////////
	

	// 1, 칼럼 구분선 
	pDrawPort->AddTexture( nX , nY , nX + 1, nY2,
		m_rtVerticalLine.U0, m_rtVerticalLine.V0, m_rtVerticalLine.U1, m_rtVerticalLine.V1,
		0xFFFFFFFF );
	// 1,2 칼럼 구분선	 	
	pDrawPort->AddTexture( nX + m_nGap1, nY , nX + m_nGap1 + 1, nY2,
		m_rtVerticalLine.U0, m_rtVerticalLine.V0, m_rtVerticalLine.U1, m_rtVerticalLine.V1,
		0xFFFFFFFF );

		
	

		
	// 2,3 칼럼 구분선 	
	pDrawPort->AddTexture( nX + m_nGap2, nY , nX + m_nGap2 + 1, nY2,
		m_rtVerticalLine.U0, m_rtVerticalLine.V0, m_rtVerticalLine.U1, m_rtVerticalLine.V1,
		0xFFFFFFFF );		

	//3,4
	pDrawPort->AddTexture( nX + m_nGap3, nY , nX + m_nGap3 + 1, nY2,
		m_rtVerticalLine.U0, m_rtVerticalLine.V0, m_rtVerticalLine.U1, m_rtVerticalLine.V1,
		0xFFFFFFFF );
	//4 
	pDrawPort->AddTexture( nX + m_nGap4, nY , nX + m_nGap4 + 1 , nY2,
		m_rtVerticalLine.U0, m_rtVerticalLine.V0, m_rtVerticalLine.U1, m_rtVerticalLine.V1,
		0xFFFFFFFF );

	// sub back	
	pDrawPort->AddTexture( nX, nY2, nX2, nY2+1,
		m_rtHorizonLine.U0, m_rtHorizonLine.V0, m_rtHorizonLine.U1, m_rtHorizonLine.V1,
		0xFFFFFFFF );	



	// Close button
	m_btnClose.Render();
	m_btnCancel.Render();
	// More button
	m_btnMore.Render();
	
	// Title 글씨	
	pDrawPort->PutTextExCX( _S(m_nTitleStringCode , m_cszTitle ), GetWidth()/2+m_nPosX, m_nPosY + nYGap+ 10);	


	nY = m_nPosY + m_nTitleHeight;
	//캐릭터 명
	MAP_SUBTITLE_IT it = m_mapSubTitle.find(0);
	if ( it != m_mapSubTitle.end() )
		pDrawPort->PutTextExCX( _S( it->second.nStringCode, it->second.cszString ), m_nPosX + nXGap+ m_lbListContent.GetListRowWidth(0)/2, nY + nYGap, 0xFFFFFFFF );	
		

	// 종족
	it = m_mapSubTitle.find(1);
	if ( it != m_mapSubTitle.end() )
		pDrawPort->PutTextExCX( _S( it->second.nStringCode, it->second.cszString ), m_nPosX + nXGap + m_nGap1 + m_lbListContent.GetListRowWidth(1)/2,
			nY + nYGap, 0xFFFFFFFF );	

	// 직업
	it = m_mapSubTitle.find(2);
	if ( it != m_mapSubTitle.end() )
		pDrawPort->PutTextExCX( _S( it->second.nStringCode, it->second.cszString ), m_nPosX + nXGap + m_nGap2 + m_lbListContent.GetListRowWidth(2)/2,
			nY + nYGap, 0xFFFFFFFF );	

	// 달성날짜 
	it = m_mapSubTitle.find(3);
	if ( it != m_mapSubTitle.end() )
		pDrawPort->PutTextExCX( _S( it->second.nStringCode, it->second.cszString ), m_nPosX + nXGap + m_nGap3 + m_lbListContent.GetListRowWidth(3)/2,
			nY + nYGap, 0xFFFFFFFF );	
		/**/
	//////////////////////////////////////////////////////////////////////////
	
	RenderList();
	
	// Flush all render queue
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();	
	
}



void CUIRankingView::RenderList ()
{
	m_lbListContent.Render();
}

// ----------------------------------------------------------------------------
// Name : OpenHelp()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingView::Open()
{
	CUIManager::getSingleton()->RearrangeOrder( m_iClassID, TRUE );
	SetEnableNextPageBtn( TRUE );	
}

// ----------------------------------------------------------------------------
// Name : Close()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingView::Close()
{
	m_btnMore.SetBtnState( UBS_IDLE ) ;
	m_btnMore.SetEnable(TRUE);
	m_lbListContent.ResetAllStrings();
	m_iPageIndex = 0;
	m_iListCount = 0;
	CUIManager::getSingleton()->RearrangeOrder( m_iClassID , FALSE );
}


void CUIRankingView::AddListContents ( int icol, int iRow, CTString cstrName, COLOR cColor )
{
	m_lbListContent.AddString(icol, iRow, cstrName, cColor );
}

void CUIRankingView::SetColNum ( int iCount )
{

}

// Toggle visible of web board
void CUIRankingView::ToggleVisible()
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

void CUIRankingView::SetEachRowWidth ( int iRow, int iCharCount )
{	
	m_lbListContent.SetRowWidth(iRow, _pUIFontTexMgr->GetFontWidth() * iCharCount + 10);		
}

int	CUIRankingView::GetEachRowWIdth ( int nRow )
{
	return m_lbListContent.GetListRowWidth ( nRow );
}


// 제목
void CUIRankingView::SetTitleName ( CTString cszTitle, int iStringCode )
{	
	m_nTitleStringCode  = iStringCode;
	m_cszTitle			= cszTitle;
}

//각 칼럼의 제목
void CUIRankingView::SetEachRowTitleName ( int iRow, CTString cszTitle,  int iStringCode )
{
	stRankingViewSubTitle stTemp;
	stTemp.nRow			= iRow;
	stTemp.nStringCode	= iStringCode;
	stTemp.cszString	= cszTitle;

	m_mapSubTitle.insert(make_pair(iRow, stTemp));
	
	switch ( iRow )
	{
	case 0: 
	{
		m_nGap1 = m_lbListContent.GetListRowWidth( iRow );
		break;
	}
	case 1:
	{
		m_nGap2 = m_nGap1 + m_lbListContent.GetListRowWidth( iRow );
		break;
	}
	case 2:
	{
		m_nGap3 = m_nGap2 + m_lbListContent.GetListRowWidth( iRow );
		break;
	}
	case 3:
	{
		m_nGap4 = m_nGap3 + m_lbListContent.GetListRowWidth( iRow );
		break;
	}
	}
}

void CUIRankingView::SendNextPage ()
{
	if ( -1 == m_iPageIndex)
		return; 
	
	++m_iPageIndex;
	int nType = CUIManager::getSingleton()->GetRankingClassfy()->RankingUIIndexToServer( m_iClassID );
	if ( -1 != nType )
		_pNetwork->SendRankingLsit( nType, m_iPageIndex );
}


void CUIRankingView::SetRowAlign ( int nROW , TEXT_ALIGN align )
{
	m_lbListContent.SetRowAlign( nROW, align );
}