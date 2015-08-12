#include "stdh.h"

// 헤더 정리. [12/3/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UITalk.h>
#include <Engine/Interface/UIMessenger.h>

// Definition


// Position
#define TALK_X					0
#define TALK_Y					0
#define TALK_WIDTH				300
#define TALK_HEIGHT				250	+19

// Define position
#define	TALK_TITLE_SX			25
#define	TALK_TITLE_SY			6

// Color
#define COLOR_TALK_LIST			0xe18600ff
#define COLOR_TALK_NAME			0xe2e2e2ff

// 좌우 크기 늘릴 때 사용한 한개의 너비
#define UNIT_SIZE_DEFAULT					40
#define UNIT_SIZE_GUIDLINE					40
#define UNIT_SIZE_RESIZE_BOTTOM				28

#define	TOP_HEIGHT							25		// 타이틀 바의 높이
#define TALKER_LIST							19		// 자신의 정보가 표시 되는 창의 높이 
#define BACK_MIDDLE_SCROLL_BOTTOM_HEIGHT	5		// 스크롤바가 포함되어 있는 중간 창의 높이 
#define BACK_BOTTOM_HEIGHT					13		// 아랫쪽 창의 높이
#define RESIZE_RANGE						10

#define TALK_INPUT_WIDTH					200

#define TALK_DESC_CHAR_WIDTH				270		
#define TALK_SYSTEM_COLOR					0xE18600FF

static int _iMaxMsgStringChar = 0;

#define COLOR_BUTTON_WIDTH					15
#define COLOR_BUTTON_HEIGHT					COLOR_BUTTON_WIDTH

//------------------------------------------------------------------------------
// CUITalk::CUITalk
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUITalk::CUITalk()
{
	Clear();
}


//------------------------------------------------------------------------------
// CUITalk
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUITalk::~CUITalk()
{
	

}

//------------------------------------------------------------------------------
// CUIMessenger::ResetPosition
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	int nOffset = -50;
	for( int iUI = UI_MESSENGER_TALK_START; iUI < UI_MESSENGER_TALK_END; iUI++ )
	{
		if( CUIManager::getSingleton()->GetTalk(iUI)->IsEnabled() )
		{
			nOffset += 10;
		}
	}

	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2 + nOffset, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 + nOffset );
}


//------------------------------------------------------------------------------
// CUIMessenger::AdjustPosition
// Explain:  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//------------------------------------------------------------------------------
// CUITalk::SetFocus
// Explain:  
// Date : 2005-05-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );
	m_ebInput.SetFocus( bVisible );
}


//------------------------------------------------------------------------------
// CUITalk::Clear
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::Clear()
{
	//m_btnClose;
	//m_ebInput;
	
	m_vecTarget.clear();

	m_vecTalkList.clear();

	m_tpTargetList.ResetAllStrings();

	m_nBoardHeight = 190;

	m_nMakeCharIndex	=-1;
	m_nChatIndex		=-1;

	//  [12/8/2006 KwonYongDae]
	m_nClientIndex		= 0;
	m_nTargetCharIndex	= -1;

	m_nColIndex			=0;

	m_ebInput.SetFocus(FALSE);
}


//------------------------------------------------------------------------------
// CUITalk::Create
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::Create( CUIWindow *pParentWnd, int nWhichUI )
{

	_iMaxMsgStringChar = TALK_DESC_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#if defined G_RUSSIA
	_iMaxMsgStringChar = 34;
#endif

	m_nWhichUI = nWhichUI;
	m_strTitle = _S( 1640, "메신저 대화" ); 
	
	CUIWindow::Create(pParentWnd, TALK_X, TALK_Y, TALK_WIDTH, TALK_HEIGHT);

	// Set Rect
	m_rcTitle.SetRect( 0, 0, 512, 22 );
	//ReSizing( RESIZE_RANGE, m_nWidth, m_nHeight );
	
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// Set Background RectUV
	m_rt3BackT.rtL.SetUV( 0, 0, 40, 26, fTexWidth, fTexHeight );
	m_rt3BackT.rtM.SetUV( 40, 0, 176, 26, fTexWidth, fTexHeight );
	m_rt3BackT.rtR.SetUV( 176, 0, 216, 26, fTexWidth, fTexHeight );

	m_rt3BackMT.SetUV3( 36, 69, 76, 75, 76, 69, 212, 75, 212, 69, 252, 75, fTexWidth, fTexHeight );
	m_rt3BackMM.SetUV3( 36, 75, 76, 81, 76, 75, 212, 81, 212, 75, 252, 81, fTexWidth, fTexHeight );
	m_rt3BackMB.SetUV3( 36, 81, 76, 87, 76, 81, 212, 87, 212, 81, 252, 87, fTexWidth, fTexHeight );

	m_rt3BackMST.SetUV3( 36, 103, 76, 108, 76, 103, 212, 108, 212, 103, 252, 108, fTexWidth, fTexHeight );
	m_rt3BackMSM.SetUV3( 36, 108, 76, 111, 76, 108, 212, 111, 212, 108, 252, 111, fTexWidth, fTexHeight );
	m_rt3BackMSB.SetUV3( 36, 111, 76, 116, 76, 111, 212, 116, 212, 111, 252, 116, fTexWidth, fTexHeight );

	m_rt3BackM.SetUV3( 0, 35, 40, 37, 40, 35, 176, 37, 176, 35, 216, 37, fTexWidth, fTexHeight );
	m_rt3BackL.SetUV3( 73, 162, 101, 175, 101, 162, 129, 175, 129, 162, 157, 175, fTexWidth, fTexHeight );

	// Close Button 
	m_btnClose.Create( this, CTString( "" ), m_nWidth - 28, 4, 14, 14 );;
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE ); 

	// MemberList Button
	m_btnMember.Create( this, CTString( "" ), m_nWidth -23, 28, 13, 13 );
	m_btnMember.SetUV( UBS_IDLE, 230, 147, 243, 160, fTexWidth, fTexHeight );
	m_btnMember.SetUV( UBS_CLICK, 230, 161, 243, 174, fTexWidth, fTexHeight );
	m_btnMember.CopyUV( UBS_IDLE, UBS_ON );
	m_btnMember.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Color Button
	m_btnColor.Create( this, CTString( "" ), m_nWidth -22, m_nHeight -25, 13, 13 );
	m_btnColor.SetUV( UBS_IDLE, 230, 147, 243, 160, fTexWidth, fTexHeight );
	m_btnColor.SetUV( UBS_CLICK, 230, 161, 243, 174, fTexWidth, fTexHeight );
	//m_btnColor.SetUV( UBS_IDLE, 328+(m_nColIndex%3)*10, 177+(m_nColIndex/3)*10, 328+(m_nColIndex%3)*10 +9, 177+(m_nColIndex/3)*10 +9, fTexWidth, fTexHeight );
	//m_btnColor.SetUV( UBS_CLICK, 328+(m_nColIndex%3)*10, 177+(m_nColIndex/3)*10, 328+(m_nColIndex%3)*10 +9, 177+(m_nColIndex/3)*10 +9, fTexWidth, fTexHeight );
	m_btnColor.CopyUV( UBS_IDLE, UBS_ON );
	m_btnColor.CopyUV( UBS_IDLE, UBS_DISABLE );


	// Input box
	m_ebInput.Create( this, 10, m_nHeight - 25, m_nWidth - ( 10 + 12 +15), 13, 256 );
	m_ebInput.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebInput.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	
	int black = 74;
	m_rt3InputBox.SetUV3( 131+black, 46, 135+black, 59, 136+black, 46, 140+black, 59, 
						141 + black, 46, 145 + black, 59, fTexWidth, fTexHeight );
 
	// List box
	//m_lbTalkList
	m_lbTalkList.Create( this, 6, 25 +19, m_nWidth-(12+11), m_nBoardHeight, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, FALSE );
	m_lbTalkList.CreateScroll( TRUE, 0, 0, 9, m_nBoardHeight+3, 9, 7, 0, 0, 10 );
		
	// Up button
	m_lbTalkList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbTalkList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbTalkList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbTalkList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbTalkList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbTalkList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbTalkList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbTalkList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbTalkList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbTalkList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbTalkList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );


	CreateTrackPopup( fTexWidth, fTexHeight );

	//pParentWnd =&m_tpColor;
	for(int i=0; i<9; ++i)
	{
		m_btnColorList[i].Create( &m_tpColor, CTString( "" ), (i%3)*COLOR_BUTTON_WIDTH, (i/3)*COLOR_BUTTON_HEIGHT, COLOR_BUTTON_WIDTH, COLOR_BUTTON_HEIGHT );
		//m_btnColorList[i].SetUV( UBS_IDLE, 230, 147, 243, 160, fTexWidth, fTexHeight );
		//m_btnColorList[i].SetUV( UBS_CLICK, 230, 161, 243, 174, fTexWidth, fTexHeight );
		m_btnColorList[i].SetUV( UBS_IDLE, 328+(i%3)*10, 177+(i/3)*10, 328+(i%3)*10 +9, 177+(i/3)*10 +9, fTexWidth, fTexHeight );
		m_btnColorList[i].SetUV( UBS_CLICK, 328+(i%3)*10, 177+(i/3)*10, 328+(i%3)*10 +9, 177+(i/3)*10 +9, fTexWidth, fTexHeight );

		m_btnColorList[i].CopyUV( UBS_IDLE, UBS_ON );
		m_btnColorList[i].CopyUV( UBS_IDLE, UBS_DISABLE );

		//pParentWnd =&m_btnColorList[0];
	}
	//m_btnColorList[0].SetPos(m_nWidth-22, m_nHeight-5);
}


//------------------------------------------------------------------------------
// CUITalk::Open
// Explain:  
// Date : 2005-05-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::Open( CTString strName)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetMessenger()->CloseAllMessageBox();

	Clear();
	m_ebInput.ResetString();
	m_lbTalkList.ResetAllStrings();
	
	AddTalkTarget(strName);
	
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	ResetPosition( pDrawPort->dp_MinI, pDrawPort->dp_MinJ, pDrawPort->dp_MaxI, pDrawPort->dp_MaxJ );

	SetVisible(TRUE);
	SetEnable(TRUE);
	m_ebInput.SetFocus(TRUE);
	pUIManager->ChangeOrder( m_nWhichUI, 1);

	AddTalkListString( "", _S( 1642, "대화내용을 입력하여주십시오."), true, TALK_SYSTEM_COLOR ); 
}




//------------------------------------------------------------------------------
// CUITalk::Open
// Explain:  
// Date : 2006-12-8,Author: KwonYongDae
//------------------------------------------------------------------------------
void CUITalk::Open(int clientIndex, const CMemberInfo targetInfo )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetMessenger()->CloseAllMessageBox();

	Clear();
	m_ebInput.ResetString();
	m_lbTalkList.ResetAllStrings();
	
	// SetMyInfo [12/8/2006 KwonYongDae]
	m_nClientIndex = clientIndex;
	m_vecTarget.push_back(pUIManager->GetMessenger()->GetMyInfo().m_strName);
	m_tpTargetList.AddMenuList(pUIManager->GetMessenger()->GetMyInfo().m_strName);

	m_nTargetCharIndex = targetInfo.m_nCharIndex;
	AddTalkTarget(targetInfo.m_strName);
	
	const CDrawPort	*pdp = pUIManager->GetDrawPort();
	ResetPosition( pdp->dp_MinI, pdp->dp_MinJ, pdp->dp_MaxI, pdp->dp_MaxJ );
	
	SetVisible(TRUE);
	SetEnable(TRUE);
	m_ebInput.SetFocus(TRUE);
	pUIManager->RearrangeOrder( m_nWhichUI, 1);		//  [12/19/2006 KwonYongDae]
	//pUIManager->ChangeOrder( m_nWhichUI, 1);

	AddTalkListString( "", _S( 1642, "대화내용을 입력하여주십시오."), true, TALK_SYSTEM_COLOR ); 
}

//------------------------------------------------------------------------------
// CUITalk::Close
// Explain: 대화창 닫기 모든 데이터 초기화 
// Date : 2005-05-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::Close()
{
	_pNetwork->MgrFriendOut( m_nMakeCharIndex, m_nChatIndex);

	Clear();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->RearrangeOrder( m_nWhichUI, FALSE );
	pUIManager->GetMessenger()->CloseTalk( m_nWhichUI );
}


//------------------------------------------------------------------------------
// CUITalk::Render
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::Render()
{
	int	nX, nY;
	GetAbsPos( nX, nY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// 배경 
	RenderBackground( nX, nY );

	// 대화 목록 
	m_lbTalkList.Render();

	// 닫기 버튼 
	m_btnClose.Render();

	// !!Edit box
	RenderRectUV3( 4, nX + 7, nY + m_nHeight - 28, TALK_WIDTH-(10+3), 18, m_rt3InputBox );

	//멤버 리스트 툴팁 버튼
	m_btnMember.Render();

	//칼라 버튼
	m_btnColor.Render();

	//멤버 리스트
	m_tpTargetList.Render();

	//칼라 리스트
	m_tpColor.Render();
	if( m_tpColor.IsVisible() )
	{
		m_btnColorList[0].Render();m_btnColorList[1].Render();m_btnColorList[2].Render();
		m_btnColorList[3].Render();m_btnColorList[4].Render();m_btnColorList[5].Render();
		m_btnColorList[6].Render();m_btnColorList[7].Render();m_btnColorList[8].Render();

		//m_tpImoticon.Show();
		//m_tpImoticon.Render();
	}
	// bug fix : change texture priority [7/19/2010 rumist]
	// Edit Box 
	m_ebInput.Render();
	//else
	//	m_tpImoticon.Hide();


	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// 타이틀 바 ( 상대방의 대화명 추가  ~ ) : 다중 대화시에는 0번 대화상대만
	CTString strTitle;
	//strTitle.PrintF( "%s - %s", m_miTarget.m_strName, m_strTitle );	===>
	strTitle.PrintF( "%s", m_strTitle );		//타이틀만...
	
	pDrawPort->PutTextEx( strTitle, nX + TALK_TITLE_SX, nY + TALK_TITLE_SY );

	// 대화 인원 수 
	strTitle.PrintF( _S(3035, "대화 참가 인원 (%d)"), m_vecTarget.size());
	
	pDrawPort->PutTextEx( strTitle, nX + TALK_TITLE_SX, nY +TOP_HEIGHT +3 );

	// Flush all render text queue
	pDrawPort->EndTextEx();
	
	// Reading window
	if( m_ebInput.DoesShowReadingWindow() )
	{
		// Set texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		// Reading window
		m_ebInput.RenderReadingWindow();

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
}


//------------------------------------------------------------------------------
// CUIMessenger::RenderBack
// Explain: 창의 배경  
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::RenderBackground( int nX, int nY ) 
{
	int nBoardHeight = m_nBoardHeight;
	// Top
	RenderRectUV3( UNIT_SIZE_DEFAULT, nX, nY, m_nWidth, TOP_HEIGHT, m_rt3BackT ); nY += TOP_HEIGHT;

	// Member Info
	RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY, m_nWidth, 5, m_rt3BackMT ); nY += 5;
	RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY, m_nWidth, 9, m_rt3BackMM ); nY += 9;
	RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY, m_nWidth, 5, m_rt3BackMB ); nY += 5;

	// Render Talk List
	RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY, m_nWidth, 5, m_rt3BackMST ); nY += 5;
	RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY, m_nWidth, nBoardHeight - BACK_MIDDLE_SCROLL_BOTTOM_HEIGHT, m_rt3BackMSM ); 
	nY += nBoardHeight - BACK_MIDDLE_SCROLL_BOTTOM_HEIGHT;
	RenderRectUV3( UNIT_SIZE_GUIDLINE, nX, nY, m_nWidth, BACK_MIDDLE_SCROLL_BOTTOM_HEIGHT, m_rt3BackMSB ); 
	nY += BACK_MIDDLE_SCROLL_BOTTOM_HEIGHT;

	// Bottom
	RenderRectUV3( UNIT_SIZE_DEFAULT, nX, nY, m_nWidth, m_nPosY + m_nHeight - BACK_BOTTOM_HEIGHT - nY, m_rt3BackM ); 
	nY += m_nPosY + ( m_nHeight - BACK_BOTTOM_HEIGHT ) - nY; 
	RenderRectUV3( UNIT_SIZE_RESIZE_BOTTOM, nX, nY, m_nWidth, BACK_BOTTOM_HEIGHT, m_rt3BackL ); 

}



//------------------------------------------------------------------------------
// CUIMessenger::RenderRectUV3
// Explain:  
// Date : 2005-05-23,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::RenderRectUV3( int nUnitSize, int nX, int nY, int nWidth, int nHeight, UIRectUV3 rtRectUV3 )
{
	int nX2 = nX + nWidth;
	int nY2 = nY + nHeight;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->AddTexture( nX, nY, nX + nUnitSize, nY2,
										rtRectUV3.rtL.U0, rtRectUV3.rtL.V0, rtRectUV3.rtL.U1, rtRectUV3.rtL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX + nUnitSize, nY, nX2 - nUnitSize, nY2,
										rtRectUV3.rtM.U0, rtRectUV3.rtM.V0, rtRectUV3.rtM.U1, rtRectUV3.rtM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - nUnitSize, nY, nX2, nY2,
										rtRectUV3.rtR.U0, rtRectUV3.rtR.V0, rtRectUV3.rtR.U1, rtRectUV3.rtR.V1,
										0xFFFFFFFF );
}


//------------------------------------------------------------------------------
// CUITalk::MouseMessage
// Explain:  
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUITalk::MouseMessage( MSG *pMsg )
{
	if( !IsEnabled() )
		return WMSG_FAIL;
	
	// Title bar
	static BOOL	bTitleBarClick = FALSE;

	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	switch( pMsg->message )
	{
		case WM_MOUSEMOVE :
			{
				if( m_tpTargetList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				else
				{
					if( m_tpTargetList.IsFocused() )
					{
						m_tpTargetList.SetFocus(false);
						m_tpTargetList.Hide();
					}
				}

				for(int i=0; i<9; ++i)
				{
					m_btnColorList[i].MouseMessage( pMsg );
				}

				if( m_tpColor.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				else
				{
					if( m_tpColor.IsFocused() )
					{
						m_tpColor.SetFocus(false);
						m_tpColor.Hide();
					}
				}

				if( IsInside( nX, nY ) )
				{
					CUIManager::getSingleton()->SetMouseCursorInsideUIs();
				}

				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;

				// Move shop
				if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
				{
					nOldX = nX;	nOldY = nY;
					Move( ndX, ndY );
					return WMSG_SUCCESS;
				}

				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				if( m_btnMember.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				if( m_btnColor.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				if( m_ebInput.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				if( m_lbTalkList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			break;
		case WM_LBUTTONDOWN :
			{
				if( m_tpTargetList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				
				for(int i=0; i<9; ++i)
				{
					m_btnColorList[i].MouseMessage( pMsg );			
				}

				if( m_tpColor.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;

				if( IsInside( nX, nY ) )
				{
					SetFocus( TRUE );
					CUIManager::getSingleton()->RearrangeOrder( m_nWhichUI, TRUE );

					nOldX = nX;		nOldY = nY;
				
					// Title bar
					if( IsInsideRect( nX, nY, m_rcTitle ) )
					{
						bTitleBarClick = TRUE;
					}

					if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					if( m_btnMember.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					if( m_btnColor.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					if( m_ebInput.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					if( m_lbTalkList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					
					return WMSG_SUCCESS;
				}	
			}
			break;
		case WM_LBUTTONUP:
			{
				if( m_tpTargetList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_tpTargetList.SetFocus(false);
					m_tpTargetList.Hide();
					return WMSG_SUCCESS;
				}
				else
				{
					m_tpTargetList.Hide();
					m_tpTargetList.SetFocus(false);
				}

				for(int i=0; i<9; ++i)
				{
					if( m_btnColorList[i].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_nColIndex =i;

						_pNetwork->MgrSetChatColor(m_nColIndex);

						m_tpColor.SetFocus(false);
						m_tpColor.Hide();

						return WMSG_SUCCESS;
					}
				}

				if( m_tpColor.MouseMessage( pMsg ) == WMSG_FAIL )
				{
					m_tpColor.Hide();
					m_tpColor.SetFocus(false);
				}

				CUIManager* pUIManager = CUIManager::getSingleton();

				if( IsInside( nX, nY ) )
				{
					SetFocus( TRUE );
					bTitleBarClick = FALSE;

					//메신저에서 캐릭을 드래그해서 채팅창에 떨궜을 때
					bool bDrop =pUIManager->GetMessenger()->IsDrop();
					if( bDrop )
					{
						pUIManager->RearrangeOrder( m_nWhichUI, TRUE );
						CMemberInfo miTemp = pUIManager->GetMessenger()->GetSelectMember();
						//대화중인 상대가 아니면...
						if( !IsExistTarget(miTemp.m_strName) )
						{
							if( miTemp.m_eCondition == OFFLINE)
								AddErrorTalk( _S(3037, "대화상대가 [오프라인] 상태이므로 초대할 수 없습니다." ) ); 
							else
							{
								//대화 상대추가
								if( m_nChatIndex < 0 ) // 1:1대화상태이면  자신의 대화상대도 초대[12/12/2006 KwonYongDae]
								{
									_pNetwork->MgrFriendInvite(m_nMakeCharIndex, m_nChatIndex<0? -1:m_nChatIndex, m_nTargetCharIndex );
									m_nTargetCharIndex = miTemp.m_nCharIndex;
								}
								else
								{
									_pNetwork->MgrFriendInvite(m_nMakeCharIndex, m_nChatIndex, miTemp.m_nCharIndex);
								}
								//AddTalkTarget(miTemp.m_strName);
							}
						}
						else
							AddErrorTalk( _S(3038, "이미 대화중인 상대입니다." ) ); 

						pUIManager->GetMessenger()->SetDrop(false);

						return WMSG_SUCCESS;
					}

					if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						Close();
						return WMSG_SUCCESS;
					}

					if( m_btnMember.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_tpTargetList.Show();
						return WMSG_SUCCESS;
					}

					if( m_btnColor.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_tpColor.Show();
						return WMSG_SUCCESS;
					}

					if( m_btnColor.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					if( m_ebInput.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					if( m_lbTalkList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
						
					return WMSG_SUCCESS;
				}
				else
				{
					pUIManager->GetMessenger()->SetDrop(false);
				}

			}
			break;
		case WM_MOUSEWHEEL:
			{
				if( IsInside( nX, nY ) )
				{
					return m_lbTalkList.MouseMessage ( pMsg );
				}
			}
			break;
		
	}

	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUITalk::KeyMessage
// Explain:  
// Date : 2005-05-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUITalk::KeyMessage( MSG *pMsg )
{
	if( !IsFocused() ) return WMSG_FAIL;
	WMSG_RESULT	wmsgResult;

	if( ( wmsgResult = m_ebInput.KeyMessage( pMsg ) ) != WMSG_FAIL )
	{
		if ( wmsgResult == WMSG_COMMAND )
		{
			// AddString 
			// test
			// AddTalkListString( m_miTarget.m_strName, m_ebInput.GetString() );

			// Message~
			// SendChatMessage함수에 금지어 필터링을 추가한 함수;
			SendChatMessageFiltering( m_ebInput.GetString() );
			m_ebInput.ResetString();
		}
		return WMSG_SUCCESS; 
	}
		
	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUITalk::IMEMessage
// Explain:  
// Date : 2005-05-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUITalk::IMEMessage( MSG *pMsg )
{
	if( !IsFocused() ) return WMSG_FAIL;
	
	return m_ebInput.IMEMessage( pMsg );
}


//------------------------------------------------------------------------------
// CUITalk::CharMessage
// Explain:  
// Date : 2005-05-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUITalk::CharMessage( MSG *pMsg )
{
	if( !IsFocused() ) return WMSG_FAIL;
	
	if( m_ebInput.CharMessage( pMsg ) != WMSG_FAIL )
	{
		return WMSG_SUCCESS; 
	}
	
	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUITalk::SetTargetCondition
// Explain:  현재 대화중인 상대의 상태를 변경한다.
// Date : 2005-05-30,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::SetTargetCondition( int nCharIndex, eCondition eState )
{
	/***
	for(int i=0; i<m_vecTarget.size(); ++i)
	{
		if( m_vecTarget[i].m_nCharIndex ==nCharIndex )
		{
			m_vecTarget[i].m_eCondition = eState;
			break;
		}
	}
	***/
	if( eState == OFFLINE )
	{
		AddErrorTalk( _S( 1643, "대화상대가 [오프라인] 상태이므로 응답하지 않을 수도 있습니다." ) ); 
	}
	else if( eState == LEFT_POSITION )
	{
		AddTalkListString( "", _S( 1644, "대화상대가 [자리비움] 상태이므로 응답하지 않을 수도 있습니다."), true, TALK_SYSTEM_COLOR ); 
	}
}


//------------------------------------------------------------------------------
// CUITalk::SendChatMessageFiltering
// Explain: SendChatMessage함수를 거치기 전에 금지어 필터링을 하는 함수
// Date : 2013-04-02, Author: 박 철희
//------------------------------------------------------------------------------
void CUITalk::SendChatMessageFiltering( CTString strMessage )
{
	// 운영자 인 경우에 필터링 안함
	if( _pNetwork->m_ubGMLevel > 1 ) {
		SendChatMessage(strMessage);
		return;
	}
	
	// 운영자 이외의 경우 불량 단어 필터링 
	char szBuffer[256];
	int nIndexBuffer[32];
	strcpy ( szBuffer, strMessage.str_String );
	
	// Filtering
	if ( _UIFiltering.Filtering ( szBuffer, nIndexBuffer ) == TRUE )
	{
		AddTalkListString( "", _S( 437, "문장에 금지된 단어가 포함되어 있습니다."), true, TALK_SYSTEM_COLOR ); 
		strMessage.Clear();
		
#ifndef FILTERING_WORD_VISIBLE_NA_20081013
		for ( int i = 1; i <= nIndexBuffer[0]; i++ ) {	
			strMessage += "[";
			strMessage += _UIFiltering.GetString( nIndexBuffer[i] );
			strMessage += "] ";
		}
		AddTalkListString( "", strMessage, true, TALK_SYSTEM_COLOR ); 
#endif
		
		return;
	}
	
	SendChatMessage(strMessage);
}


//------------------------------------------------------------------------------
// CUITalk::SendChatMessage
// Explain:  
// Date : 2005-05-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::SendChatMessage( CTString strMessage )
{
	if (UIMGR()->GetMessenger()->GetMemberCondition(m_nTargetCharIndex) == OFFLINE)
	{
		UIMGR()->GetMessenger()->ErrorMessage(_S(1638, "[오프라인] 상태인 대상과는 대화를 할 수 없습니다."));
		return;
	}

	if( m_nChatIndex < 0 )	/*서버와 연결이 되어있지 않은 상태*/
		_pNetwork->MgrOneVsOneConnect( CUIManager::getSingleton()->GetMessenger()->GetMyInfo().m_nCharIndex, m_nTargetCharIndex , strMessage );
	_pNetwork->MgrFriendChat(m_nMakeCharIndex, m_nChatIndex, strMessage);
}

//------------------------------------------------------------------------------
// CUITalk::AddTalkListString
// Explain:  
// Date : 2005-05-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::AddTalkListString( CTString strName, CTString strMessage, bool bFocus, COLOR colDesc )
{
	if( bFocus ) SetFocus( TRUE );

	CTString strTalk = strMessage;

	if( strName != "" )
		strTalk = strName + "> " + strMessage;

	m_vecTalkList.push_back( strTalk );

	AddTalkListString( strTalk, colDesc );
	
	if( m_lbTalkList.GetCurItemCount(0) > m_lbTalkList.GetLinePerPage() )
	{
		m_lbTalkList.SetScrollBarPos( m_lbTalkList.GetCurItemCount(0) - m_lbTalkList.GetLinePerPage() );
	}
	
}


//------------------------------------------------------------------------------
// CUITalk::ReloadTalkList
// Explain: 크기 변경시 크기에 맞게 스트링 다시 조절  
// Date : 2005-05-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::ReloadTalkList()
{
	std::vector<CTString>::iterator iterBegin = m_vecTalkList.begin();
	std::vector<CTString>::iterator iterEnd = m_vecTalkList.end();
	std::vector<CTString>::iterator iter;
	
	// 일단 다 지워
	m_lbTalkList.ResetAllStrings();

	// 다시 넣기 
	for( iter = iterBegin; iter != iterEnd; ++iter) 
	{
		AddTalkListString( *iter );	
	}

}


//------------------------------------------------------------------------------
// CUITalk::AddErrorTalk
// Explain:  
// Date : 2005-05-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::AddErrorTalk( CTString &strDesc, COLOR colDesc )
{
	AddTalkListString( "", strDesc, false, colDesc );
}


//------------------------------------------------------------------------------
// CUISkillLearn::AddTalkString
// Explain:  
// Date : 2005-05-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITalk::AddTalkListString( CTString &strDesc, COLOR colDesc )
{
	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;

	int		iPos;
	// wooss 051002
#if defined G_THAI
	// Get length of string
	INDEX	nThaiLen = FindThaiLen(strDesc);
	INDEX	nChatMax= (_iMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
	if( nLength == 0 )
		return;
	// If length of string is less than max char
	if( nThaiLen <= nChatMax )
	{
		// Check line character
	for( iPos = 0; iPos < nLength; iPos++ )
	{
		if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
			break;
	}

	// Not exist
	if( iPos == nLength )
	{
		m_lbTalkList.AddString( 0, strDesc, colDesc );
	}
	else
	{
		// Split string
		CTString	strTemp, strTemp2;
		strDesc.Split( iPos, strTemp2, strTemp );
		m_lbTalkList.AddString( 0, strTemp2, colDesc );

		// Trim line character
		if( strTemp[0] == '\r' && strTemp[1] == '\n' )
			strTemp.TrimLeft( strTemp.Length() - 2 );
		else
			strTemp.TrimLeft( strTemp.Length() - 1 );

		AddTalkListString( strTemp, colDesc );
	}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = _iMaxMsgStringChar;
		BOOL	b2ByteChar = FALSE;
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if(nChatMax < FindThaiLen(strDesc,0,iPos))
				break;
		}
		nSplitPos = iPos;

		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( nSplitPos, strTemp2, strTemp );
			m_lbTalkList.AddString( 0, strTemp2, colDesc );

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; iPos++ )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}

			AddTalkListString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbTalkList.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddTalkListString( strTemp, colDesc );
		}

	}
#else	
	// If length of string is less than max char
	if( nLength <= _iMaxMsgStringChar )
	{
		// Check line character
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nLength )
		{
			m_lbTalkList.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbTalkList.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddTalkListString( strTemp, colDesc );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = _iMaxMsgStringChar;
		BOOL	b2ByteChar = FALSE;
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( nSplitPos, strTemp2, strTemp );
			m_lbTalkList.AddString( 0, strTemp2, colDesc );

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; iPos++ )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}
#ifdef MODIFY_MESSENGER_CHAT_BLANK_NA_20081028
			strTemp = CTString("  ") + strTemp;
#endif
			AddTalkListString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbTalkList.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

#ifdef MODIFY_MESSENGER_CHAT_BLANK_NA_20081028
			strTemp = CTString("  ") + strTemp;
#endif
			AddTalkListString( strTemp, colDesc );
		}
	}
#endif
}

void CUITalk::CreateTrackPopup( float fTexWidth, float fTexHeight)
{
	// 채팅에 참가한 멤버 리스트
	m_tpTargetList.m_rtBackUL.SetUV( 164, 45, 171, 63, fTexWidth, fTexHeight );
	m_tpTargetList.m_rtBackUM.SetUV( 174, 45, 176, 63, fTexWidth, fTexHeight );
	m_tpTargetList.m_rtBackUR.SetUV( 179, 45, 186, 63, fTexWidth, fTexHeight );
	m_tpTargetList.m_rtBackML.SetUV( 164, 55, 171, 58, fTexWidth, fTexHeight );
	m_tpTargetList.m_rtBackMM.SetUV( 174, 55, 176, 58, fTexWidth, fTexHeight );
	m_tpTargetList.m_rtBackMR.SetUV( 179, 55, 186, 58, fTexWidth, fTexHeight );
	m_tpTargetList.m_rtBackLL.SetUV( 164, 60, 171, 68, fTexWidth, fTexHeight );
	m_tpTargetList.m_rtBackLM.SetUV( 174, 60, 176, 68, fTexWidth, fTexHeight );
	m_tpTargetList.m_rtBackLR.SetUV( 179, 60, 186, 68, fTexWidth, fTexHeight );

	m_tpTargetList.Create( this, m_nWidth, 25, 100, 1000, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, TRUE );
	m_tpTargetList.SetScrollBar( FALSE );
	m_tpTargetList.SetSelBar( 100, _pUIFontTexMgr->GetLineHeight()+2, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_tpTargetList.SetOverColor( 0xF8E1B5FF );
	m_tpTargetList.SetSelectColor( 0xF8E1B5FF );
	
	// 채팅 폰트 색
	m_tpColor.m_rtBackUL.SetUV( 164, 45, 171, 63, fTexWidth, fTexHeight );
	m_tpColor.m_rtBackUM.SetUV( 174, 45, 176, 63, fTexWidth, fTexHeight );
	m_tpColor.m_rtBackUR.SetUV( 179, 45, 186, 63, fTexWidth, fTexHeight );
	m_tpColor.m_rtBackML.SetUV( 164, 55, 171, 58, fTexWidth, fTexHeight );
	m_tpColor.m_rtBackMM.SetUV( 174, 55, 176, 58, fTexWidth, fTexHeight );
	m_tpColor.m_rtBackMR.SetUV( 179, 55, 186, 58, fTexWidth, fTexHeight );
	m_tpColor.m_rtBackLL.SetUV( 164, 60, 171, 68, fTexWidth, fTexHeight );
	m_tpColor.m_rtBackLM.SetUV( 174, 60, 176, 68, fTexWidth, fTexHeight );
	m_tpColor.m_rtBackLR.SetUV( 179, 60, 186, 68, fTexWidth, fTexHeight );

	m_tpColor.Create( this, m_nWidth-22, m_nHeight -5, COLOR_BUTTON_WIDTH*3, COLOR_BUTTON_HEIGHT*3, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, TRUE );
	m_tpColor.SetScrollBar( FALSE );
	m_tpColor.SetSelBar( 100, _pUIFontTexMgr->GetLineHeight()+2, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_tpColor.SetOverColor( 0xF8E1B5FF );
	m_tpColor.SetSelectColor( 0xF8E1B5FF );

	m_tpColor.Hide();
}

void CUITalk::AddTalkTarget( CTString strName)
{
	std::vector<CTString>::iterator iter;

	iter = find(m_vecTarget.begin(), m_vecTarget.end(), strName);

	if( iter ==m_vecTarget.end() )
	{
		m_vecTarget.push_back(strName);
		m_tpTargetList.AddMenuList(strName);

		if( strName != CUIManager::getSingleton()->GetMessenger()->GetMyInfo().m_strName)
		{
			CTString strMessage;
			strMessage.PrintF(_S( 3036,"%s님이 대화방에 들어오셨습니다."), strName );
			AddErrorTalk( strMessage ); 
		}
	}
}

bool CUITalk::IsExistTarget(CTString strName)
{
	std::vector<CTString>::iterator iter;
	iter =find(m_vecTarget.begin(), m_vecTarget.end(), strName );

	if( iter != m_vecTarget.end() )
		return true;
	else
		return false;
}

void CUITalk::DeleteTalkTarget(CTString strName)
{
	std::vector<CTString>::iterator iter;

	iter =find(m_vecTarget.begin(), m_vecTarget.end(), strName);

	//해당 캐릭이 존재하지 않음
	if( iter == m_vecTarget.end() )
		return;

	CTString strMessage;
	strMessage.PrintF(_S(3039,"%s 님이 대화방에서 나가셨습니다."), (*iter) );
	AddErrorTalk( strMessage ); 

	m_vecTarget.erase(iter);

	//채팅 멤버 리스트에서 지움.
	m_tpTargetList.DeleteMenuList(0, iter -m_vecTarget.begin());

	/***
	if( m_vecTarget.size() <= 1)
		Close();
	***/

}
