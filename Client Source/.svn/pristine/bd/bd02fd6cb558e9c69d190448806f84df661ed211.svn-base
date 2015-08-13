#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISelectWord.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Contents/Base/UIQuestNew.h>

#define BUTTON_HEIGHT		71
#define BUTTON_WIDTH		146
#define BUTTON_SPAN_H		2
#define BUTTON_SPAN_W		2
#define TITLEBAR_HEIGHT		26

// ----------------------------------------------------------------------------
// Name : CUISelectWord()
// Desc : Constructorreturn ;
// ----------------------------------------------------------------------------
CUISelectWord::CUISelectWord()
{	
	m_ptdNakoTitle = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUISelectWord()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISelectWord::~CUISelectWord()
{
	if( m_ptdNakoTitle )
	{
		_pTextureStock->Release( m_ptdNakoTitle );
		m_ptdNakoTitle = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectWord::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{	
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	// Region of each part
	m_rcTitle.SetRect( 0, 0, 236, 22 );
	
	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtBackMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtBackMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 290, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );	

	m_ptdNakoTitle = CreateTexture( CTString( "Data\\Interface\\Nako_Title.tex" ) );
	fTexWidth = m_ptdNakoTitle->GetPixWidth();
	fTexHeight = m_ptdNakoTitle->GetPixHeight();

	const int iStartX = 15;
	const int iStartY = 30;	

	int iCurPosX = iStartX;
	int iCurPosY = iStartY;
	
	int i = 0, j  = 0;
	
	int nStartX, nStartY, nEndX, nEndY;
	nStartY = 0;
	for( int nCount = 0; nCount < 6; nCount++ )
	{
		nStartX = 0;
	
		nEndX = nStartX + BUTTON_WIDTH;
		nEndY = nStartY + BUTTON_HEIGHT;

		CUIButton btnTemp;
		btnTemp.Create( this, CTString(""), iCurPosX, iCurPosY, BUTTON_WIDTH, BUTTON_HEIGHT );
		btnTemp.SetUV( UBS_IDLE, nStartX, nStartY, nEndX, nEndY, fTexWidth, fTexHeight );

		nStartX += ( BUTTON_WIDTH + 1 );
		nEndX = nStartX + BUTTON_WIDTH;

		btnTemp.SetUV( UBS_CLICK, nStartX, nStartY, nEndX, nEndY, fTexWidth, fTexHeight );
		btnTemp.CopyUV( UBS_IDLE, UBS_ON );
		btnTemp.CopyUV( UBS_IDLE, UBS_DISABLE );


		m_vectorWordList.push_back(btnTemp);
		
		iCurPosY += BUTTON_HEIGHT + BUTTON_SPAN_H;
		
		if( i >= 2 ) 
		{
			i = 0;
			j = 1;

			iCurPosX = iStartX + BUTTON_WIDTH + BUTTON_SPAN_W;
			iCurPosY = iStartY;
		}
		else 
		{
			i++;
		}	 
		nStartY += BUTTON_HEIGHT + 1;
		
				
	}

	nWidth = 2 * (BUTTON_WIDTH + BUTTON_SPAN_W) + 28;
	nHeight = 3 * (BUTTON_HEIGHT + BUTTON_SPAN_H) + TITLEBAR_HEIGHT + 14;
	SetSize( nWidth, nHeight );
	

}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectWord::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}
// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectWord::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenSelectResource()
// Desc : iType = 1 채굴, iType = 2 채집, iType = 3 차지
// ----------------------------------------------------------------------------
void CUISelectWord::OpenSelectWord()
{	
	if( IsVisible() )
	{
		ResetSelectWord();
		OpenSelectWord();
		return;
	}
	
	CUIManager::getSingleton()->RearrangeOrder( UI_SELECTWORD, TRUE );
}

// ----------------------------------------------------------------------------
// Name : ResetSelectWord()
// Desc : 
// ----------------------------------------------------------------------------
void CUISelectWord::ResetSelectWord()
{
	CUIManager::getSingleton()->RearrangeOrder( UI_SELECTWORD, FALSE );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISelectWord::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	// Background
	int nX = m_nPosX;
	int nY = m_nPosY;
	pDrawPort->AddTexture( nX, m_nPosY, nX + m_nWidth, nY + TITLEBAR_HEIGHT,
		m_rtBackTop.U0, m_rtBackTop.V0, 
		m_rtBackTop.U1, m_rtBackTop.V1,
		0xFFFFFFFF );
	
	nY += TITLEBAR_HEIGHT;	
	pDrawPort->AddTexture( nX, nY, nX + m_nWidth, m_nPosY + m_nHeight - 7,
		m_rtBackMiddle1.U0, m_rtBackMiddle1.V0, 
		m_rtBackMiddle1.U1, m_rtBackMiddle1.V1,
		0xFFFFFFFF );
	
	nY = m_nPosY + m_nHeight - 7;

	pDrawPort->AddTexture( nX, nY, nX + m_nWidth, m_nPosY + m_nHeight,
		m_rtBackBottom.U0, m_rtBackBottom.V0, 
		m_rtBackBottom.U1, m_rtBackBottom.V1,
		0xFFFFFFFF );	

	m_btnClose.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();	
	
	pDrawPort->InitTextureData( m_ptdNakoTitle );
	
	UIButton_vector::iterator itend = m_vectorWordList.end();
	UIButton_vector::iterator it = m_vectorWordList.begin();

	for(; it != itend; ++it)
	{
		(*it).Render();
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();	
	
	CTString strTitle = _S(2223, "게임 타이틀 선택" );
	
	pDrawPort->PutTextEx( strTitle, m_nPosX + SELECTREWORD_TITLE_TEXT_OFFSETX,	
		m_nPosY + SELECTREWORD_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	
	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUISelectWord::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	
	// Title bar
	static BOOL	bTitleBarClick = FALSE;
	
	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;
	
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
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
			
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			
			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}
			
			// Close Button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;
				
				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				else
				{
					UIButton_vector::iterator end = m_vectorWordList.end();
					UIButton_vector::iterator it = m_vectorWordList.begin();
					
					for(; it != end; ++it)
					{
						if( (*it).MouseMessage( pMsg ) != WMSG_FAIL )//button 누름
						{
							//int dist = std::distance(m_vectorWordList.begin(), it);
							//!! 서버에 메세지 보내기 
						
							return WMSG_SUCCESS;
						}
					}
				}
				
				CUIManager::getSingleton()->RearrangeOrder( UI_SELECTWORD, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;
			
			// Close button
			if( (wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if(wmsgResult == WMSG_COMMAND)
				{
					ResetSelectWord();
					CUIManager::getSingleton()->GetQuest()->MsgBoxLCommand( MSGLCMD_EVENT, QUEST_RENUAL_EVENT );

				}
				return WMSG_SUCCESS;
			}
			else
			{	
				for(int i = 0; i < m_vectorWordList.size(); ++i)
				{
					if( (wmsgResult = m_vectorWordList[i].MouseMessage( pMsg ) )!= WMSG_FAIL )
					{
						if(wmsgResult == WMSG_COMMAND)
						{
							_pNetwork->Send2p4pLetter( i );
						}
						return WMSG_SUCCESS;
					}
				}
			}
		}
		break;
	}
	return WMSG_FAIL;
}

//------------------------------------------------------------------------------
// CUISelectWord::MsgBoxCommand
// Explain:  
// Date : 2005-10-05,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISelectWord::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( nCommandCode == MSGCMD_SELECT_WORD_NOTIFY )
	{
		if( bOK )
		{
			if( _pNetwork->m_iLastError != MSG_EVENT_2PAN4PAN_WORD_OK )
				OpenSelectWord();
		}

	}
}

//------------------------------------------------------------------------------
// CUISelectWord::ResultProcess
// Explain:  
// Date : 2005-10-05,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISelectWord::ResultProcess( LONG lError )
{
	CTString strMessage;

	_pNetwork->m_iLastError = lError;
		 
	switch( lError )
	{
	case MSG_EVENT_2PAN4PAN_WORD_OK:
		strMessage = _S(2224, "이벤트 상품이 지급되었습니다." );
		ResetSelectWord();
		break;
	case MSG_EVENT_2PAN4PAN_WORD_NOITEM:
		strMessage = _S(2225, "요청한 타이틀과 소지한 아이템이 일치하지 않습니다. 다시 확인하여 주시기 바랍니다." );
		break;
	case MSG_EVENT_2PAN4PAN_WORD_ALREADY:
		strMessage = _S(2226, "이미 지급받은 상품은 다시 지급 받을 수 없습니다." );
		break;
	}

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(2209, "낱말 맞추기 이벤트" ), UMBS_OK, UI_SELECTWORD, MSGCMD_SELECT_WORD_NOTIFY );
	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
	
	ResetSelectWord();
}	