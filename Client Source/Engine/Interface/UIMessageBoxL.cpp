#include "stdh.h"
#include <Engine/Interface/UIMessageBoxL.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UITextureManager.h>


static int	_iMaxDescStringChar = 0;
static int	_iMaxSelStringChar = 0;
static int	_nMsgBoxLineHeight = 0;
//extern INDEX g_iCountry;
// ----------------------------------------------------------------------------
// Name : CUIMessageBoxL()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIMessageBoxL::CUIMessageBoxL()
{
	m_nWhichUI = -1;
	m_nCommandCode = -1;
	m_nSelStringCount = 0;
	m_nOverSelection = -1;
	m_bTitleBarClick = FALSE;

	// [sora] 메시지 박스실행 시간 제한
	m_nStartTime = 0;
	m_nTime = 0;
	m_nTimeOutBtnMessage = -1;
}

// ----------------------------------------------------------------------------
// Name : ~CUIMessageBoxL()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIMessageBoxL::~CUIMessageBoxL()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBoxL::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	_iMaxDescStringChar = 266 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	_iMaxSelStringChar = 275 / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	_nMsgBoxLineHeight = _pUIFontTexMgr->GetFontHeight() + 4;

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 311, 22 );
	m_rcSelection.SetRect( 11, 200, 291, 200 );

	// Create message box texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtTopL.SetUV( 0, 0, 40, 26, fTexWidth, fTexHeight );
	m_rtTopM.SetUV( 40, 0, 176, 26, fTexWidth, fTexHeight );
	m_rtTopR.SetUV( 176, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtMiddleScrollL.SetUV( 0, 27, 40, 29, fTexWidth, fTexHeight );
	m_rtMiddleScrollM.SetUV( 40, 27, 176, 29, fTexWidth, fTexHeight );
	m_rtMiddleScrollR.SetUV( 176, 27, 216, 29, fTexWidth, fTexHeight );
	m_rtMiddleL.SetUV( 0, 31, 40, 33, fTexWidth, fTexHeight );
	m_rtMiddleM.SetUV( 40, 31, 176, 33, fTexWidth, fTexHeight );
	m_rtMiddleR.SetUV( 176, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtMiddleGapL.SetUV( 0, 35, 40, 37, fTexWidth, fTexHeight );
	m_rtMiddleGapM.SetUV( 40, 35, 176, 37, fTexWidth, fTexHeight );
	m_rtMiddleGapR.SetUV( 176, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBottomL.SetUV( 0, 38, 40, 45, fTexWidth, fTexHeight );
	m_rtBottomM.SetUV( 40, 38, 176, 45, fTexWidth, fTexHeight );
	m_rtBottomR.SetUV( 176, 38, 216, 45, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 279, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// List box
	m_lbDescription.Create( this, 7, 26, 288, 164, _nMsgBoxLineHeight, 12, 8, 1, FALSE );
	m_lbDescription.CreateScroll( TRUE, 0, 0, 9, 164, 9, 7, 0, 0, 10 );
	// Up button
	m_lbDescription.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbDescription.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbDescription.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbDescription.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbDescription.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbDescription.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbDescription.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbDescription.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBoxL::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBoxL::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : CreateMessageBox()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBoxL::CreateMessageBox( CTString &strTitle, int nWhichUI, int nCommandCode, int nPosX, int nPosY )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Message box setting
	if( nPosX != -1 && nPosX != -1 )
	{
		SetPos( nPosX, nPosY );
	}
	else
	{
		int	nX = ( pUIManager->GetMaxI() - pUIManager->GetMinI() - GetWidth() ) / 2;
		int	nY = ( pUIManager->GetMaxJ() - pUIManager->GetMinJ() - GetHeight() ) / 2;
		SetPos( nX, nY );
	}
	m_nWhichUI = nWhichUI;
	m_nCommandCode = nCommandCode;
	m_strTitle = strTitle;
	m_nCurNum = 1;

#if defined G_KOR
	// [2011/04/11 : Sora] 메시지 박스실행 제한 시간 20초로 변경
	SetMsgBoxLTimer( 20, -1 );
#else
	// [sora] 메시지 박스실행 시간 제한
	m_nStartTime = 0;
	m_nTime = 0;
	m_nTimeOutBtnMessage = -1;
#endif

	pUIManager->RearrangeOrder( m_nUIIndex, TRUE );
}

// ----------------------------------------------------------------------------
// Name : InitMessageBox()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBoxL::InitMessageBox()
{
	m_nWhichUI = -1;
	m_nCommandCode = -1;
	m_nSelStringCount = 0;
	m_nOverSelection = -1;

	// [sora] 메시지 박스실행 시간 제한
	m_nStartTime = 0;
	m_nTime = 0;
	m_nTimeOutBtnMessage = -1;

	m_lbDescription.ResetAllStrings();
	m_vecStrSelMessage.clear();
	m_vecColSelMessage.clear();
	m_vecSelIndex.clear();

	CUIManager::getSingleton()->RearrangeOrder( m_nUIIndex, FALSE );
}

// ----------------------------------------------------------------------------
// Name : AddDescString()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBoxL::AddDescString( CTString &strMessage, const COLOR colDesc )
{
	// Get length of string
	INDEX	nLength = strMessage.Length();
	INDEX	limitstrpos = _iMaxDescStringChar;
	if( nLength == 0 )
		return;
#if defined(G_THAI)
	nLength = FindThaiLen(strMessage);
	limitstrpos = (_iMaxDescStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
#else
	nLength = strMessage.Length();
	limitstrpos = _iMaxDescStringChar;
#endif
	
	int		iPos;
	if( nLength <= limitstrpos )
	{
		// Check line character
		for( iPos = 0; iPos < strMessage.Length(); iPos++ )
		{
			if( strMessage[iPos] == '\n' || strMessage[iPos] == '\r' )
				break;	
		}
		
		// Not exist
		if( iPos == nLength )
		{
			m_lbDescription.AddString( 0, strMessage, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strMessage.Split( iPos, strTemp2, strTemp );
			
			// Date : 2005-03-16,   By Lee Ki-hwan : 버그 수정
			// m_lbDescription.AddString( 0, strMessage, colDesc ); 
			m_lbDescription.AddString( 0, strTemp2, colDesc ); 
			
			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );
			
			AddDescString( strTemp, colDesc );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = limitstrpos;
		BOOL	b2ByteChar = FALSE;
#if defined(G_THAI)
		for( iPos = 0; iPos < strMessage.Length(); iPos++ )
		{
			if(nSplitPos < FindThaiLen(strMessage,0,iPos))
				break;
		}
		nSplitPos = iPos;
#else
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strMessage[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}
		
		if( b2ByteChar )
			nSplitPos--;
#endif
		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strMessage[iPos] == '\n' || strMessage[iPos] == '\r' )
				break;
		}
		
		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
#ifdef LINE_CHANGE_BY_WORD
			if( strMessage[nSplitPos] != ' ' )
			{
				for(int i = iPos; i>=0; --i)
				{
					if( strMessage[i] == ' ' )
					{
						iPos = i;
						break;
					}
				}
			}
#endif
			strMessage.Split( iPos, strTemp2, strTemp );
			m_lbDescription.AddString( 0, strTemp2, colDesc );
			
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
			
			AddDescString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strMessage.Split( iPos, strTemp2, strTemp );
			m_lbDescription.AddString( 0, strTemp2, colDesc );
			
			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );
			
			AddDescString( strTemp, colDesc );
		}
		
	}
}

// ----------------------------------------------------------------------------
// Name : AddSelString()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBoxL::AddSelString( CTString &strMessage, COLOR colSel, int iValue )
{
	// Get length of string
	INDEX	nLength = strMessage.Length();
	INDEX	limitstrpos = _iMaxSelStringChar;
	if( nLength == 0 )
		return;
#if defined(G_THAI)
	nLength = FindThaiLen(strMessage);
	limitstrpos = (_iMaxSelStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
#else
	nLength = strMessage.Length();
	limitstrpos = _iMaxSelStringChar;
#endif
	if( nLength <= limitstrpos )
	{
		m_vecStrSelMessage.push_back( strMessage );
		m_vecColSelMessage.push_back( colSel );
		
		sSelTable TempSel;
		TempSel.iIndex	= m_nSelStringCount;
		TempSel.iValue	= iValue;
		m_vecSelIndex.push_back( TempSel );
		m_nSelStringCount++;
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = limitstrpos;
		BOOL	b2ByteChar = FALSE;
		int iPos = 0;
#if defined(G_THAI)
		for( iPos = 0; iPos < strMessage.Length(); iPos++ )
		{
			if(nSplitPos < FindThaiLen(strMessage,0,iPos))
				break;
		}
		nSplitPos = iPos;
#endif
		// Split string
		CTString	strTemp, strTemp2;
		strMessage.Split( nSplitPos, strTemp2, strTemp );
		m_vecStrSelMessage.push_back( strTemp2 );
		m_vecColSelMessage.push_back( colSel );
		
		sSelTable TempSel;
		TempSel.iIndex	= m_nSelStringCount;
		TempSel.iValue	= iValue;
		m_vecSelIndex.push_back( TempSel );
		
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
		
		// Add space
		strTemp = CTString( "   " ) + strTemp;
		
		AddSelString( strTemp, colSel, iValue );
		
	}
	m_nSelHeight = 14 + m_vecStrSelMessage.size() * _nMsgBoxLineHeight;
	m_rcSelection.Bottom = m_rcSelection.Top + m_vecStrSelMessage.size() * _nMsgBoxLineHeight;
	m_nHeight = MSGBOXL_TOP_HEIGHT + MSGBOXL_DESC_HEIGHT + MSGBOXL_GAP_HEIGHT +
				MSGBOXL_BOTTOM_HEIGHT + m_nSelHeight;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBoxL::Render()
{

	// [sora] 메시지 박스실행 시간 제한
	if(m_nTime > 0)
	{
		__int64 tmpTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

		// 설정한 시간이 지나가면 명령 실행
		if(tmpTime - m_nStartTime > (m_nTime * 1000))
		{
			ReturnCommand(m_nTimeOutBtnMessage);
		}
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2;
	nX2 = m_nPosX + m_nWidth;
	// Background
	// Top
	nY = m_nPosY + MSGBOXL_TOP_HEIGHT;
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + 40, nY,
										m_rtTopL.U0, m_rtTopL.V0, m_rtTopL.U1, m_rtTopL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 40, m_nPosY, nX2 - 40, nY,
										m_rtTopM.U0, m_rtTopM.V0, m_rtTopM.U1, m_rtTopM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 40, m_nPosY, nX2, nY,
										m_rtTopR.U0, m_rtTopR.V0, m_rtTopR.U1, m_rtTopR.V1,
										0xFFFFFFFF );

	// Desc middle
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 40, nY + MSGBOXL_DESC_HEIGHT,
										m_rtMiddleScrollL.U0, m_rtMiddleScrollL.V0,
										m_rtMiddleScrollL.U1, m_rtMiddleScrollL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY + MSGBOXL_DESC_HEIGHT,
										m_rtMiddleScrollM.U0, m_rtMiddleScrollM.V0,
										m_rtMiddleScrollM.U1, m_rtMiddleScrollM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 40, nY, nX2, nY + MSGBOXL_DESC_HEIGHT,
										m_rtMiddleScrollR.U0, m_rtMiddleScrollR.V0,
										m_rtMiddleScrollR.U1, m_rtMiddleScrollR.V1,
										0xFFFFFFFF );

	// Gap middle
	nY += MSGBOXL_DESC_HEIGHT;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 40, nY + MSGBOXL_GAP_HEIGHT,
										m_rtMiddleGapL.U0, m_rtMiddleGapL.V0, m_rtMiddleGapL.U1, m_rtMiddleGapL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY + MSGBOXL_GAP_HEIGHT,
										m_rtMiddleGapM.U0, m_rtMiddleGapM.V0, m_rtMiddleGapM.U1, m_rtMiddleGapM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 40, nY, nX2, nY + MSGBOXL_GAP_HEIGHT,
										m_rtMiddleGapR.U0, m_rtMiddleGapR.V0, m_rtMiddleGapR.U1, m_rtMiddleGapR.V1,
										0xFFFFFFFF );

	// Selection middle
	nY += MSGBOXL_GAP_HEIGHT;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 40, nY + m_nSelHeight,
										m_rtMiddleL.U0, m_rtMiddleL.V0, m_rtMiddleL.U1, m_rtMiddleL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY + m_nSelHeight,
										m_rtMiddleM.U0, m_rtMiddleM.V0, m_rtMiddleM.U1, m_rtMiddleM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 40, nY, nX2, nY + m_nSelHeight,
										m_rtMiddleR.U0, m_rtMiddleR.V0, m_rtMiddleR.U1, m_rtMiddleR.V1,
										0xFFFFFFFF );

	// Bottom
	nY += m_nSelHeight;
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + 40, nY + MSGBOXL_BOTTOM_HEIGHT,
										m_rtBottomL.U0, m_rtBottomL.V0, m_rtBottomL.U1, m_rtBottomL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX + 40, nY, nX2 - 40, nY + MSGBOXL_BOTTOM_HEIGHT,
										m_rtBottomM.U0, m_rtBottomM.V0, m_rtBottomM.U1, m_rtBottomM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - 40, nY, nX2, nY + MSGBOXL_BOTTOM_HEIGHT,
										m_rtBottomR.U0, m_rtBottomR.V0, m_rtBottomR.U1, m_rtBottomR.V1,
										0xFFFFFFFF );

	// Close box
	m_btnClose.Render();

	// List box
	m_lbDescription.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Title
	pDrawPort->PutTextEx( m_strTitle, m_nPosX + MSGBOXL_TITLE_TEXT_OFFSETX,
										m_nPosY + MSGBOXL_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	nX = m_nPosX + MSGBOXL_SEL_TEXT_SX;
	nY = m_nPosY + MSGBOXL_SEL_TEXT_SY;
	for( int iMsg = 0; iMsg < m_vecStrSelMessage.size(); iMsg++ )
	{
		if( m_nOverSelection != -1 && m_vecSelIndex[iMsg].iIndex == m_vecSelIndex[m_nOverSelection].iIndex )
			pDrawPort->PutTextEx( m_vecStrSelMessage[iMsg], nX, nY, 0xFF6107FF );
		else
			pDrawPort->PutTextEx( m_vecStrSelMessage[iMsg], nX, nY, m_vecColSelMessage[iMsg] );
		nY += _nMsgBoxLineHeight;
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : ReturnCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIMessageBoxL::ReturnCommand( int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( m_nWhichUI < 0 )
		pUIManager->MsgBoxLCommand( m_nCommandCode, nResult );
	else
		pUIManager->GetUI( m_nWhichUI )->MsgBoxLCommand( m_nCommandCode, nResult );

	InitMessageBox();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMessageBoxL::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

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

			// If message box isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;

			// Move message box
			if( m_bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
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
			else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Selection region
			else if( IsInsideRect( nX, nY, m_rcSelection ) )
			{
				int	nAbsY = GetAbsPosY();
				m_nOverSelection = ( nY - nAbsY - m_rcSelection.Top ) / _nMsgBoxLineHeight;
				if( m_nOverSelection < 0 || m_nOverSelection >= m_vecSelIndex.size() )
					m_nOverSelection = -1;

				return WMSG_SUCCESS;
			}

			m_nOverSelection = -1;
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
					m_bTitleBarClick = TRUE;
				}
				// List box
				else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Selection region
				else if( IsInsideRect( nX, nY, m_rcSelection ) )
				{
					int	nAbsY = GetAbsPosY();
					int	nResult = ( nY - nAbsY - m_rcSelection.Top ) / _nMsgBoxLineHeight;

					if( nResult >= 0 && nResult < m_vecSelIndex.size() )
						ReturnCommand( m_vecSelIndex[nResult].iValue );

					return WMSG_SUCCESS;
				}

				CUIManager::getSingleton()->RearrangeOrder( m_nUIIndex, TRUE );
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
				m_bTitleBarClick = FALSE;

				// If message box isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						ReturnCommand( -1 );

					return WMSG_SUCCESS;
				}
				// List box
				else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
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
				// List box
				if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// List box
				if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

WMSG_RESULT		CUIMessageBoxL::KeyMessage(MSG *pMsg )
{
	// If message box is not focused
	if( !IsFocused() )
		return WMSG_FAIL;

	if( pMsg->wParam == VK_ESCAPE )
	{
		ReturnCommand( -1 );
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}
