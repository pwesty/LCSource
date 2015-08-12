#include "stdh.h"
#include <dimm.h>
#include <imm.h>	// Date : 2005-03-05,   By Lee Ki-hwan
#include <Engine/Interface/UIEditBox.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Interface/UIIME.h>			// 이기환 추가 (11.16)

#include <Engine/Base/KeyNames.h>//ADD_CHAT_SPECIALCHAR_DISABLE_NA_20090210
#include <Engine/Base/Input.h>
#include <Engine/Help/UISupport.h>
#include <Engine/GameStageManager/StageMgr.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Interface/UIImage.h>

// External variables
extern HWND	_hwndMain;
extern INDEX g_iCountry;

// cpp2angel Edit (11.17)
// Static variables
BOOL		CUIEditBox::s_bShowReadingWindow;
// connie [2009/11/2] - 일본 아이디/패스워드 16자까지 
char		CUIEditBox::s_szReadingString[34];	
int			CUIEditBox::s_nReadingStringLength;
CCandList	CUIEditBox::s_CandList;
BOOL		bSecondComposition = false;
int			CUIEditBox::s_nRefCount = 0;

CUIEditBox*	CUIEditBox::s_FocusingEditBox = NULL;

CUIEditBox*	CUIEditBox::s_PrevFocus = NULL;

// cppangel Edit End

#if defined(G_RUSSIA)
	ENGINE_API extern CFontData *_pfdDefaultFont;
#endif	//	defined(G_RUSSIA)

// ----------------------------------------------------------------------------
// Name : CUIEditBox()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIEditBox::CUIEditBox()
	: m_pInputCallback(NULL)
	, m_pBackground(NULL)
	, m_nCursorSX(0)
{
	m_bIsPWEditBox = FALSE;
	m_pInputBuffer = NULL;
	m_pTempBuffer = NULL;
	m_pPWBuffer = NULL;
	m_b2ByteChar = FALSE;
	m_bOnComposition = FALSE;
	m_nMaxCharCount = 0;
	m_nCurCharCount = 0;
	m_nFirstShowChar = 0;
	m_nCursorIndex = 0;
	m_bShowCursor = TRUE;
	m_dElapsedTime = 0.0;
	m_dOldPromptTime = 0.0;
	m_bMsgInput = FALSE;
	m_InValidEditBox = FALSE;
	m_bOnlyInteger = FALSE;

	// [2011/11/02 : Sora] 커서 이동 불가플래그
	m_bMoveCursor = TRUE;
	// 이기환 수정 시작 (11. 15)
	//s_bShowReadingWindow = FALSE;
	//memset ( s_szReadingString, 0, sizeof( s_szReadingString ) );
	// 이기환 수정 끝 
	m_nTextColor = 0xF2F2F2FF;
	m_nShowCharCount = 0;

	m_bCopyAndPaste = false;
	m_pImgSelText = NULL;
	m_nCopyStartPos = -1;

	setType(eUI_CONTROL_EDIT);
}

// ----------------------------------------------------------------------------
// Name : ~CUIEditBox()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIEditBox::~CUIEditBox()
{
	Destroy();

	s_PrevFocus = NULL;
	s_FocusingEditBox = NULL;
}

void CUIEditBox::ResetShowCount()
{
	m_nShowCharCount = ( m_nWidth + _pUIFontTexMgr->GetFontSpacing() ) /
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
}


// ----------------------------------------------------------------------------
// Name : Destroy()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, int nMaxChar, BOOL bIsPWBox, int color )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_nTextSY = ( nHeight - _pUIFontTexMgr->GetFontHeight() ) / 2;
	m_nMaxCharCount = nMaxChar;
	m_nShowCharCount = ( nWidth + _pUIFontTexMgr->GetFontSpacing() ) /
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	// wooss 051002
	m_nShowCharSize = nWidth;
	
	// Allocate memory of input & temporary buffer
	if( nMaxChar > 0 )
	{
		m_pInputBuffer = new char[nMaxChar + 1];
		m_pInputBuffer[0] = NULL;
		m_pTempBuffer = new char[nMaxChar + 1];
		m_pTempBuffer[0] = NULL;

		if( bIsPWBox )
		{
			m_bIsPWEditBox = bIsPWBox;
			m_pPWBuffer = new char[m_nShowCharCount + 1];
			m_pPWBuffer[0] = NULL;
		}
	}
	m_nTextColor = color;
}

void CUIEditBox::initialize()
{
	m_nTextSY = ( m_nHeight - _pUIFontTexMgr->GetFontHeight() ) / 2;

	m_nShowCharCount = ( m_nWidth + _pUIFontTexMgr->GetFontSpacing() ) /
		( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	
	m_nShowCharSize = m_nWidth;

	if (m_nMaxCharCount > 0)
	{
		SAFE_ARRAY_DELETE(m_pInputBuffer);
		SAFE_ARRAY_DELETE(m_pTempBuffer);

		m_pInputBuffer = new char[m_nMaxCharCount + 1];
		m_pInputBuffer[0] = NULL;
		m_pTempBuffer = new char[m_nMaxCharCount + 1];
		m_pTempBuffer[0] = NULL;

		if (m_bIsPWEditBox)
		{
			SAFE_ARRAY_DELETE(m_pPWBuffer);
			m_pPWBuffer = new char[m_nShowCharCount + 1];
			m_pPWBuffer[0] = NULL;
		}
	}

#ifdef VER_TEST
	m_bCopyAndPaste = true;
#endif // VER_TEST

	m_pImgSelText = new CUIImage;
	addChild((CUIBase*)m_pImgSelText);
	
	m_pImgSelText->setTexString("CommonBtn.tex");
	UIRectUV uv;
	uv.SetUV(158, 59, 165, 66);
	m_pImgSelText->SetUV(uv);
	m_pImgSelText->SetColor(0x0000FFCC);
	m_pImgSelText->Hide(TRUE);
}

// ----------------------------------------------------------------------------
// Name : SetMaxChar()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::SetMaxChar( int nMaxChar )
{
	// If new size is equal to old size
	if( m_nMaxCharCount == nMaxChar )
		return;

	// Reset string
	ResetString();

	// Release memory of input & temporary buffer
	SAFE_ARRAY_DELETE(m_pInputBuffer);
	SAFE_ARRAY_DELETE(m_pTempBuffer);

	m_nMaxCharCount = nMaxChar;

	// Allocate memory of input & temporary buffer
	if( nMaxChar > 0 )
	{
		m_pInputBuffer = new char[nMaxChar + 1];
		m_pInputBuffer[0] = NULL;
		m_pTempBuffer = new char[nMaxChar + 1];
		m_pTempBuffer[0] = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : Destroy()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::Destroy()
{
	CUIWindow::Destroy();

	// Release memory of input & temporary buffer
	SAFE_ARRAY_DELETE( m_pInputBuffer );
	SAFE_ARRAY_DELETE( m_pTempBuffer );
	SAFE_ARRAY_DELETE( m_pPWBuffer );

	SAFE_DELETE(m_pInputCallback);

	SAFE_DELETE(m_pBackground);
}

// ----------------------------------------------------------------------------
// Name : SetReadingWindowUV()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::SetReadingWindowUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1,
										FLOAT fTexWidth, FLOAT fTexHeight )
{
#ifdef UI_TOOL
	m_ReadingWindowUV.U0 = fTx0; m_ReadingWindowUV.V0 = fTy0; m_ReadingWindowUV.U1 = fTx1; m_ReadingWindowUV.V1 = fTy1;
#endif // UI_TOOL
	m_rtReadWndL.SetUV( fTx0, fTy0, fTx0 + 6.0f, fTy1, fTexWidth, fTexHeight );
	m_rtReadWndM.SetUV( fTx0 + 7.0f, fTy0, fTx1 - 7.0f, fTy1, fTexWidth, fTexHeight );
	m_rtReadWndR.SetUV( fTx1 - 6.0f, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
}

// ----------------------------------------------------------------------------
// Name : SetCandidateUV()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::SetCandidateUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1,
									FLOAT fTexWidth, FLOAT fTexHeight )
{
#ifdef UI_TOOL
	m_CandidateUV.U0 = fTx0; m_CandidateUV.V0 = fTy0; m_CandidateUV.U1 = fTx1; m_CandidateUV.V1 = fTy1;
#endif // UI_TOOL
	m_rtCandUL.SetUV( fTx0, fTy0, fTx0 + 6.0f, fTy0 + 6.0f, fTexWidth, fTexHeight );
	m_rtCandUM.SetUV( fTx0 + 7.0f, fTy0, fTx1 - 7.0f, fTy0 + 6.0f, fTexWidth, fTexHeight );
	m_rtCandUR.SetUV( fTx1 - 6.0f, fTy0, fTx1, fTy0 + 6.0f, fTexWidth, fTexHeight );
	m_rtCandML.SetUV( fTx0, fTy0 + 7.0f, fTx0 + 6.0f, fTy1 - 7.0f, fTexWidth, fTexHeight );
	m_rtCandMM.SetUV( fTx0 + 7.0f, fTy0 + 7.0f, fTx1 - 7.0f, fTy1 - 7.0f, fTexWidth, fTexHeight );
	m_rtCandMR.SetUV( fTx1 - 6.0f, fTy0 + 7.0f, fTx1, fTy1 - 7.0f, fTexWidth, fTexHeight );
	m_rtCandLL.SetUV( fTx0, fTy1 - 6.0f, fTx0 + 6.0f, fTy1, fTexWidth, fTexHeight );
	m_rtCandLM.SetUV( fTx0 + 7.0f, fTy1 - 6.0f, fTx1 - 7.0f, fTy1, fTexWidth, fTexHeight );
	m_rtCandLR.SetUV( fTx1 - 6.0f, fTy1 - 6.0f, fTx1, fTy1, fTexWidth, fTexHeight );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::Render()
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );
	nY += m_nTextSY;

	// Length of shown characters
	int	nLength = m_nCurCharCount - m_nFirstShowChar;

	// wooss 051002
#if defined(G_THAI)
	int tv_Len=FindThaiLen(m_pInputBuffer,m_nFirstShowChar, m_nCursorIndex);
	if(((m_nShowCharCount)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing())) < tv_Len)
		m_nFirstShowChar++;
#else
	if( nLength > m_nShowCharCount )	nLength = m_nShowCharCount;
#endif

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Render text
	if( m_bIsPWEditBox )
	{
		int iChar;
		for( iChar = 0; iChar < nLength; iChar++ )
			m_pPWBuffer[iChar] = '*';
		m_pPWBuffer[iChar] = NULL;
		pDrawPort->PutTextCharEx( m_pPWBuffer, nLength, nX, nY );
	}
	else
	{
#ifdef G_RUSSIA
		int nShowCnt = UTIL_HELP()->CheckNoFixedLength(_pfdDefaultFont, &m_pInputBuffer[m_nFirstShowChar], m_nShowCharSize);
		std::string strTemp = &m_pInputBuffer[m_nFirstShowChar];
		strTemp = strTemp.substr(0, nShowCnt);
		pDrawPort->PutTextCharEx( (char*)strTemp.c_str(), nLength, nX + m_nCursorSX, nY );
#else // G_RUSSIA
		pDrawPort->PutTextCharEx( &m_pInputBuffer[m_nFirstShowChar], nLength, nX + m_nCursorSX, nY );
#endif // G_RUSSIA
		
	}

	// Render cursor
	if( IsFocused() )
	{
		if( m_pParent != NULL && !m_pParent->IsFocused() )		
			SetFocus( FALSE );

		DOUBLE	dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
		m_dElapsedTime += dCurTime - m_dOldPromptTime;
		m_dOldPromptTime = dCurTime;
		if( m_dElapsedTime > 0.5 )
		{
			m_bShowCursor = !m_bShowCursor;
			do
			{
				m_dElapsedTime -= 0.5;
			}
			while( m_dElapsedTime > 0.5 );
		}

		int nCursorX = 0;
#if defined(G_RUSSIA)
		std::string strTemp = m_pInputBuffer;
		strTemp = strTemp.substr(m_nFirstShowChar, m_nCursorIndex - m_nFirstShowChar);
		nCursorX = m_nCursorSX + nX + UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, (char*)strTemp.c_str());
#else
		nCursorX = m_nCursorSX + nX + ( m_nCursorIndex - m_nFirstShowChar ) *
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif
		

		// Date : 2005-03-09,   By Lee Ki-hwan
		/*
			!!문제 : 커서의 위치가 글자의 위치와 맞지 않음 

			문자내에 2Byte 문자가 몇개인 지 판단해서 여백 지워줌 
			CursorIndex는 2Byte문자를 2개의 1Byte문자로 판단해서 여백처리서 1개의 여백이 증가한다. 
			 
			* 정상처리 
				2Byte문자size (12) + 여백 (1) * 문자 갯수 = 13 * 문자 갯수 
			
			* CursorIndex의 경우 ( 1Byte문자2개로 처리) 
				1Byte 문자 Size ( 6 ) + 여백 (1) * 2 * 문자 갯수 = 14 * 문자 갯수 

			@해결방법 : 현재까지와 같은 방법으로 커서의 위치를 계산한 후 2Byte문자의 갯수를 파악해 
			갯수 만의 추가 적용된 여백을 빼준다.
		*/
#if defined(G_THAI)
		nCursorX = nX + FindThaiLen(m_pInputBuffer,m_nFirstShowChar, m_nCursorIndex);
#else
#if !defined(G_RUSSIA)
		int n2ByteChar = Get2ByteCharCount ( m_pInputBuffer, m_nCursorIndex );
		nCursorX -= n2ByteChar * _pUIFontTexMgr->GetFontSpacing(); 
#endif
#endif
		int	nCursorY = nY;

		if( m_bShowCursor )
		{
			if( m_bOnComposition )
			{
				if( _pUIFontTexMgr->GetLanguage() == FONT_KOREAN )
				{
					char	cCursor = 31;
					int		nOldFontWidth = _pUIFontTexMgr->GetFontWidth();
					_pUIFontTexMgr->SetFontWidth( _pUIFontTexMgr->GetFontWidth2Byte() );
					//pDrawPort->PutTextCharEx( &cCursor, 1, nCursorX, nCursorY, 0x00FF00FF ); // 폰트 테스트
					pDrawPort->PutTextCharEx( &cCursor, 1, nCursorX, nCursorY, m_nTextColor );
					_pUIFontTexMgr->SetFontWidth( nOldFontWidth );

					//pDrawPort->PutTextCharEx( &m_pInputBuffer[m_nCursorIndex], 2,	// 폰트 테스트
															//nCursorX, nCursorY, 0xFFFFFFFF );

					pDrawPort->PutTextCharEx( &m_pInputBuffer[m_nCursorIndex], 2,
															nCursorX, nCursorY, 0x000000FF );
				}
				else
				{
					char	cCursor = 30;
					pDrawPort->PutTextCharEx( &cCursor, 1, nCursorX - 1, nCursorY, 0xFFFFFFFF );
				}
			}
			else
			{
#if defined(G_RUSSIA)
					pDrawPort->PutTextEx( CTString("|"), nCursorX, nCursorY-2, m_nTextColor );
#else
					char	cCursor = 30;
					pDrawPort->PutTextCharEx( &cCursor, 1, nCursorX - 1, nCursorY, m_nTextColor );
#endif
			}
		}
	}
	
}

void CUIEditBox::OnRender( CDrawPort* pDraw )
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );
	nY += m_nTextSY;

	if (m_pTexData != NULL)
	{
		// 텍스쳐가 있을 경우에만 배경을 그린다.
		pDraw->InitTextureData( m_pTexData );
		if( m_pBackground != NULL )
		{
			m_pBackground->SetPos(nX, nY);
			m_pBackground->RenderRectSurface( pDraw, DEF_UI_COLOR_WHITE );
	
			pDraw->FlushRenderingQueue();
		}
	}

	// Length of shown characters
	int	nLength = m_nCurCharCount - m_nFirstShowChar;

	// wooss 051002
#if defined(G_THAI)
	int tv_Len=FindThaiLen(m_pInputBuffer,m_nFirstShowChar, m_nCursorIndex);
	if(((m_nShowCharCount)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing())) < tv_Len)
		m_nFirstShowChar++;
#else
	if( nLength > m_nShowCharCount )	nLength = m_nShowCharCount;
#endif

	// Render text
	if ( m_bIsPWEditBox )
	{
		if( m_pPWBuffer != NULL )
		{
			int iChar;
			for( iChar = 0; iChar < nLength; iChar++ )
				m_pPWBuffer[iChar] = '*';
			m_pPWBuffer[iChar] = NULL;
			pDraw->PutTextCharEx( m_pPWBuffer, nLength, nX, nY );
		}
	}
	
	else
	{
		if (m_pInputBuffer != NULL)
		{
#ifdef G_RUSSIA
			int nShowCnt = UTIL_HELP()->CheckNoFixedLength(_pfdDefaultFont, &m_pInputBuffer[m_nFirstShowChar], m_nShowCharSize);
			std::string strTemp = &m_pInputBuffer[m_nFirstShowChar];
			strTemp = strTemp.substr(0, nShowCnt);
			pDraw->PutTextCharEx( (char*)strTemp.c_str(), nLength, nX + m_nCursorSX, nY );
#else // G_RUSSIA
			pDraw->PutTextCharEx( &m_pInputBuffer[m_nFirstShowChar], nLength, nX + m_nCursorSX, nY );
#endif // G_RUSSIA
		}
	}

	// Render cursor
	if (IsFocused())
	{
// 		if( m_pParent != NULL && !m_pParent->IsFocused() )		
// 			SetFocus( FALSE );

		DOUBLE	dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
		m_dElapsedTime += dCurTime - m_dOldPromptTime;
		m_dOldPromptTime = dCurTime;
		if( m_dElapsedTime > 0.5 )
		{
			m_bShowCursor = !m_bShowCursor;
			do
			{
				m_dElapsedTime -= 0.5;
			}
			while( m_dElapsedTime > 0.5 );
		}

		int nCursorX = 0;
#if defined(G_RUSSIA)
		std::string strTemp = m_pInputBuffer;

		if (m_bIsPWEditBox)
		{
			int nMax = strTemp.size();
			strTemp = "";
			
			for (int c = 0; c < nMax; ++c)
			{
				strTemp += "*";
			}
		}

		strTemp = strTemp.substr(m_nFirstShowChar, m_nCursorIndex - m_nFirstShowChar);
		nCursorX = m_nCursorSX + nX + UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, (char*)strTemp.c_str());
#else	// G_RUSSIA
		nCursorX = m_nCursorSX + nX + ( m_nCursorIndex - m_nFirstShowChar ) *
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif	// G_RUSSIA

#if defined(G_THAI)
		nCursorX = m_nCursorSX + nX + FindThaiLen(m_pInputBuffer,m_nFirstShowChar, m_nCursorIndex);
#else
#if !defined(G_RUSSIA)
		int n2ByteChar = Get2ByteCharCount ( m_pInputBuffer, m_nCursorIndex );
		nCursorX -= n2ByteChar * _pUIFontTexMgr->GetFontSpacing(); 
#endif
#endif
		int	nCursorY = nY;

		if( m_bShowCursor )
		{
			if( m_bOnComposition )
			{
				if( _pUIFontTexMgr->GetLanguage() == FONT_KOREAN )
				{
					char	cCursor = 31;
					int		nOldFontWidth = _pUIFontTexMgr->GetFontWidth();
					_pUIFontTexMgr->SetFontWidth( _pUIFontTexMgr->GetFontWidth2Byte() );
					//pDraw->PutTextCharEx( &cCursor, 1, nCursorX, nCursorY, 0x00FF00FF ); // 폰트 테스트
					pDraw->PutTextCharEx( &cCursor, 1, nCursorX, nCursorY, m_nTextColor );
					_pUIFontTexMgr->SetFontWidth( nOldFontWidth );

					//pDraw->PutTextCharEx( &m_pInputBuffer[m_nCursorIndex], 2,	// 폰트 테스트
															//nCursorX, nCursorY, 0xFFFFFFFF );

					pDraw->PutTextCharEx( &m_pInputBuffer[m_nCursorIndex], 2,
															nCursorX, nCursorY, 0x000000FF );
				}
				else
				{
					char	cCursor = 30;
					pDraw->PutTextCharEx( &cCursor, 1, nCursorX - 1, nCursorY, 0xFFFFFFFF );
					nLength += 1;
				}
			}
			else
			{
#if defined(G_RUSSIA)
					pDraw->PutTextEx( CTString("|"), nCursorX, nCursorY-2, m_nTextColor );
#else
					char	cCursor = 30;
					pDraw->PutTextCharEx( &cCursor, 1, nCursorX - 1, nCursorY, m_nTextColor );
					nLength += 1;
#endif
			}
		}
	}

	if (nLength > 0)
		pDraw->EndTextEx();

#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

// ----------------------------------------------------------------------------
// Name : RenderReadingWindow()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::RenderReadingWindow()
{
	// 이기환 수정 시작 (11.17)
	int	nX, nY;
	GetAbsPos( nX, nY );
	int	nCursorX = nX + ( m_nCursorIndex - m_nFirstShowChar ) *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	int	nCursorY = nY + m_nTextSY;

	// Space for outline of reading and candidate window
	nCursorX -= 6;
	nCursorY += 14;

	// Calculate window size
	int	nReadWndWidth = strlen( s_szReadingString ) *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) + 11;
	int	nCandWndWidth = s_CandList.nMaxLength *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) + 11;
	const static int	nReadWndHeight = 16;
	const static int	nCandWndHeight = 158;

	// Candidate window must be larger than reading window
	if( nCandWndWidth < nReadWndWidth )
		nCandWndWidth = nReadWndWidth;

	// Set region of reading and candidate window
	m_rcReadWnd.SetRect( nCursorX, nCursorY, nCursorX + nReadWndWidth, nCursorY + nReadWndHeight );
	m_rcCand.SetRect( nCursorX, nCursorY + nReadWndHeight,
						nCursorX + nCandWndWidth, nCursorY + nReadWndHeight + nCandWndHeight );

	// If candidate is only shown
	if( !s_bShowReadingWindow )
	{
		int	nReadHeight = m_rcReadWnd.GetHeight();
		m_rcCand.Top -= nReadHeight;
		m_rcCand.Bottom -= nReadHeight;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Adjust position of reading and candidate window
	if( m_rcCand.Left < pUIManager->GetMinI() )
	{
		int	nOffset = pUIManager->GetMinI() - m_rcCand.Left;
		m_rcReadWnd.Left += nOffset;		m_rcReadWnd.Right += nOffset;
		m_rcCand.Left += nOffset;			m_rcCand.Right += nOffset;
	}
	else if( m_rcCand.Right > pUIManager->GetMaxI() )
	{
		int	nOffset = pUIManager->GetMinI() - m_rcCand.Right;
		m_rcReadWnd.Left += nOffset;		m_rcReadWnd.Right += nOffset;
		m_rcCand.Left += nOffset;			m_rcCand.Right += nOffset;
	}

	if( m_rcCand.Bottom > pUIManager->GetMaxJ() )
	{
		nCursorY -= 17;
		m_rcReadWnd.SetRect( nCursorX, nCursorY - nReadWndHeight, nCursorX + nReadWndWidth, nCursorY );
		m_rcCand.SetRect( nCursorX, m_rcReadWnd.Top - nCandWndHeight, nCursorX + nCandWndWidth, m_rcReadWnd.Top );

		// If candidate is only shown
		if( !s_bShowReadingWindow )
		{
			int	nReadHeight = m_rcReadWnd.GetHeight();
			m_rcCand.Top += nReadHeight;
			m_rcCand.Bottom += nReadHeight;
		}
	}

	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Render reading window
	if( s_bShowReadingWindow )
	{
		// Left
		pDrawPort->AddTexture( m_rcReadWnd.Left, m_rcReadWnd.Top,
											m_rcReadWnd.Left + 6, m_rcReadWnd.Bottom,
											m_rtReadWndL.U0, m_rtReadWndL.V0,
											m_rtReadWndL.U1, m_rtReadWndL.V1,
											0xFFFFFFFF );
		// Middle
		pDrawPort->AddTexture( m_rcReadWnd.Left + 6, m_rcReadWnd.Top,
											m_rcReadWnd.Right - 6, m_rcReadWnd.Bottom,
											m_rtReadWndM.U0, m_rtReadWndM.V0,
											m_rtReadWndM.U1, m_rtReadWndM.V1,
											0xFFFFFFFF );
		// Right
		pDrawPort->AddTexture( m_rcReadWnd.Right - 6, m_rcReadWnd.Top,
											m_rcReadWnd.Right, m_rcReadWnd.Bottom,
											m_rtReadWndR.U0, m_rtReadWndR.V0,
											m_rtReadWndR.U1, m_rtReadWndR.V1,
											0xFFFFFFFF );

		nX = m_rcReadWnd.Left + 6;
		nY = m_rcReadWnd.Top + 3;

		pDrawPort->PutTextCharEx( s_szReadingString, 0, nX, nY);
	}

	// Render candidate window
	if( s_CandList.bShowWindow )
	{
		// Upper left
		pDrawPort->AddTexture( m_rcCand.Left, m_rcCand.Top,
											m_rcCand.Left + 6, m_rcCand.Top + 6,
											m_rtCandUL.U0, m_rtCandUL.V0,
											m_rtCandUL.U1, m_rtCandUL.V1,
											0xFFFFFFFF );
		// Upper middle
		pDrawPort->AddTexture( m_rcCand.Left + 6, m_rcCand.Top,
											m_rcCand.Right - 6, m_rcCand.Top + 6,
											m_rtCandUM.U0, m_rtCandUM.V0,
											m_rtCandUM.U1, m_rtCandUM.V1,
											0xFFFFFFFF );
		// Upper right
		pDrawPort->AddTexture( m_rcCand.Right - 6, m_rcCand.Top,
											m_rcCand.Right, m_rcCand.Top + 6,
											m_rtCandUR.U0, m_rtCandUR.V0,
											m_rtCandUR.U1, m_rtCandUR.V1,
											0xFFFFFFFF );
		// Middle left
		pDrawPort->AddTexture( m_rcCand.Left, m_rcCand.Top + 6,
											m_rcCand.Left + 6, m_rcCand.Bottom - 6,
											m_rtCandML.U0, m_rtCandML.V0,
											m_rtCandML.U1, m_rtCandML.V1,
											0xFFFFFFFF );
		// Middle middle
		pDrawPort->AddTexture( m_rcCand.Left + 6, m_rcCand.Top + 6,
											m_rcCand.Right - 6, m_rcCand.Bottom - 6,
											m_rtCandMM.U0, m_rtCandMM.V0,
											m_rtCandMM.U1, m_rtCandMM.V1,
											0xFFFFFFFF );
		// Middle right
		pDrawPort->AddTexture( m_rcCand.Right - 6, m_rcCand.Top + 6,
											m_rcCand.Right, m_rcCand.Bottom - 6,
											m_rtCandMR.U0, m_rtCandMR.V0,
											m_rtCandMR.U1, m_rtCandMR.V1,
											0xFFFFFFFF );
		// Lower left
		pDrawPort->AddTexture( m_rcCand.Left, m_rcCand.Bottom - 6,
											m_rcCand.Left + 6, m_rcCand.Bottom,
											m_rtCandLL.U0, m_rtCandLL.V0,
											m_rtCandLL.U1, m_rtCandLL.V1,
											0xFFFFFFFF );
		// Lower middle
		pDrawPort->AddTexture( m_rcCand.Left + 6, m_rcCand.Bottom - 6,
											m_rcCand.Right - 6, m_rcCand.Bottom,
											m_rtCandLM.U0, m_rtCandLM.V0,
											m_rtCandLM.U1, m_rtCandLM.V1,
											0xFFFFFFFF );
		// Lower right
		pDrawPort->AddTexture( m_rcCand.Right - 6, m_rcCand.Bottom - 6,
											m_rcCand.Right, m_rcCand.Bottom,
											m_rtCandLR.U0, m_rtCandLR.V0,
											m_rtCandLR.U1, m_rtCandLR.V1,
											0xFFFFFFFF );

		nX = m_rcCand.Left + 6;
		nY = m_rcCand.Top + 6;
		for( int i = 0; i < s_CandList.dwPageSize; i++ )
		{
			pDrawPort->PutTextCharEx( s_CandList.szCandidate[i], 0, nX, nY);
			nY += 15;
		}
	}
	// 이기환 수정 끝 
}

// ----------------------------------------------------------------------------
// Name : ResetString()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::ResetString()
{
	//m_pInputBuffer[0] = NULL;
	if( m_pInputBuffer )
		ZeroMemory(m_pInputBuffer, strlen(m_pInputBuffer)+1);

	m_nCurCharCount = 0;
	m_nCursorIndex = 0;
	m_nFirstShowChar = 0;
}

// ----------------------------------------------------------------------------
// Name : SetString()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::SetString( char *pcString )
{
	int	nLength = strlen( pcString );
	if( nLength <= 0 )
		return;

	// Resize buffer
	if( nLength > m_nMaxCharCount )
		SetMaxChar( nLength );

	// Copy string
	memcpy( m_pInputBuffer, pcString, nLength );

	if (nLength >= 2)
	{
		if (m_pInputBuffer[nLength-2] == '\r' && m_pInputBuffer[nLength-1] == '\n')
		{
			m_pInputBuffer[nLength-2] = '\0';
			m_pInputBuffer[nLength-1] = '\0';
			nLength -= 2;
		}
	}

	m_pInputBuffer[nLength] = NULL;
	m_nCurCharCount = nLength;
	m_nCursorIndex = m_nCurCharCount;

	if( m_nFirstShowChar + m_nShowCharCount < m_nCursorIndex )
	{
		m_nFirstShowChar = m_nCursorIndex - m_nShowCharCount;

		if( Is2ByteChar( m_nFirstShowChar ) )
			m_nFirstShowChar++;
	}

	// [091104] 보이는 문자수 다시 체크
#if defined(G_RUSSIA)
	m_nShowCharCount = UtilHelp::getSingleton()->CheckNoFixedLength(_pfdDefaultFont, pcString, m_nWidth);
#endif
}

void CUIEditBox::ClearInputBuffer()
{
	if (m_pInputBuffer == NULL)
		return;

	int	nLength = strlen( m_pInputBuffer );

	memset(m_pInputBuffer, 0, nLength);

	m_nCurCharCount = 0;
	m_nCursorIndex = m_nCurCharCount;
	m_nFirstShowChar = 0;
}

// ----------------------------------------------------------------------------
// Name : Is2ByteChar()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIEditBox::Is2ByteChar( int nCharIndex, int nFirstCheck )
{
	BOOL	b2ByteChar = FALSE;
	char	*pcTemp = &m_pInputBuffer[nFirstCheck];

	// wooss 070313 -------------------------------------------------------->><<
	// SingleByte CodeSet
	// connie [2009/9/11] - 
	if( g_iCountry ==THAILAND || g_iCountry == BRAZIL  || g_iCountry == GERMANY || g_iCountry == SPAIN
		|| g_iCountry == FRANCE || g_iCountry == POLAND || g_iCountry == RUSSIA || g_iCountry == TURKEY || g_iCountry == MEXICO
		|| g_iCountry == USA_SPAIN || g_iCountry == ITALY || g_iCountry == NETHERLANDS)//FRANCE_SPAIN_CLOSEBETA_NA_20081124
	{
		return b2ByteChar;
	}
	for( int iPos = nFirstCheck; iPos < nCharIndex; iPos++, pcTemp++ )
	{
		if( (*pcTemp & 0x80) && (g_iCountry !=THAILAND) ){
			if( _pUIFontTexMgr->GetLanguage() == FONT_JAPANESE && ((UCHAR)*pcTemp >= 0xa1 && (UCHAR)*pcTemp <= 0xdf) ) 
				b2ByteChar = FALSE;
			else b2ByteChar = !b2ByteChar;
		}
		else
			b2ByteChar = FALSE;
	}

	return b2ByteChar;
}

// ----------------------------------------------------------------------------
// Name : MoveCursor()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::MoveCursor( MoveDirection mdDirection )
{
	switch( mdDirection )
	{
	case MD_LEFT:
		if( m_nCursorIndex > 0 )
		{
			if( Is2ByteChar( m_nCursorIndex - 1 ) )
				m_nCursorIndex -= 2;
			else
				m_nCursorIndex--;

			// Adjust position of first shown character
			if( m_nFirstShowChar > m_nCursorIndex )
			{
				m_nFirstShowChar = m_nCursorIndex;

				if( Is2ByteChar( m_nFirstShowChar ) )
					m_nFirstShowChar++;
			}
		}
		break;

	case MD_RIGHT:
		if( m_nCursorIndex < m_nCurCharCount )
		{
			if( Is2ByteChar( m_nCursorIndex + 1, m_nFirstShowChar ) )
				m_nCursorIndex += 2;
			else
				m_nCursorIndex++;

			// Adjust position of first shown character
			if( m_nFirstShowChar + m_nShowCharCount < m_nCursorIndex )
			{
				m_nFirstShowChar = m_nCursorIndex - m_nShowCharCount;

				if( Is2ByteChar( m_nFirstShowChar ) )
					m_nFirstShowChar++;
			}
		}
		break;

	case MD_HOME:
		m_nCursorIndex = 0;
		m_nFirstShowChar = 0;
		break;

	case MD_END:
		if( m_nCurCharCount - m_nFirstShowChar > m_nShowCharCount )
		{
			m_nCursorIndex = m_nCurCharCount;
			m_nFirstShowChar = m_nCurCharCount - m_nShowCharCount;
		}
		else
		{
			m_nCursorIndex = m_nCurCharCount;
		}

		// Adjust position of first shown character
		if( Is2ByteChar( m_nFirstShowChar ) )
			m_nFirstShowChar++;
		
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : InsertChar()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::InsertChar( int nInsertPos, char cInsert )
{
	if (m_bOnlyInteger == TRUE && nInsertPos == 0)
	{
		if (cInsert == '0')
			return;
	}

	// If input buffer is not enough
	if( m_nCurCharCount >= m_nMaxCharCount )
		return;
#if defined(G_RUSSIA)
	if( nInsertPos >= m_nMaxCharCount )
		return;
#endif
	// If cursor locates in last position of input string
	if( nInsertPos == m_nCurCharCount )
	{
		// Insert character
		m_pInputBuffer[nInsertPos] = cInsert;
		m_pInputBuffer[nInsertPos + 1] = NULL;
	}
	else
	{
		// Split string
		int	nCopyLength = m_nCurCharCount - nInsertPos;
		memcpy( (void *)m_pTempBuffer, (void *)&(m_pInputBuffer[nInsertPos]), nCopyLength );
		m_pTempBuffer[nCopyLength] = NULL;

		// Insert character
		m_pInputBuffer[nInsertPos] = cInsert;

		// Combine strings
		memcpy( (void *)&(m_pInputBuffer[nInsertPos + 1]), (void *)m_pTempBuffer, nCopyLength + 1 );
	}

	// Increase cursor index and current character count
	m_nCursorIndex++;
	m_nCurCharCount++;

#if defined(G_RUSSIA)
	m_nShowCharCount = UtilHelp::getSingleton()->CheckNoFixedLength(_pfdDefaultFont, &m_pInputBuffer[m_nFirstShowChar], m_nWidth);
#endif
	// Adjust position of first shown character
	if( m_nCursorIndex > m_nFirstShowChar + m_nShowCharCount )
	{
		// wooss 051002
#if defined(G_THAI)
		int tv_Len=FindThaiLen(m_pInputBuffer,m_nFirstShowChar, m_nCursorIndex);
		if(((m_nShowCharCount)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing())) < tv_Len) {
			m_nFirstShowChar++;
		} 
#else
		m_nFirstShowChar = m_nCursorIndex - m_nShowCharCount;
#endif
		if( Is2ByteChar( m_nFirstShowChar ) )
			m_nFirstShowChar++;
	}
}

// ----------------------------------------------------------------------------
// Name : InsertChars()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::InsertChars( int nInsertPos, char *pcInsert )
{
	// Check length
	int	nLength = strlen( pcInsert );
	if( nLength < 1 )
		return;

	// If input buffer is not enough
	if( m_nCurCharCount + nLength > m_nMaxCharCount ) // edit by cpp2angel (11.19) : >= to > 
		return;

	// If cursor locates in last position of input string
	if( nInsertPos == m_nCurCharCount )
	{
		// Insert characters
		memcpy( (void *)&(m_pInputBuffer[nInsertPos]), (void *)pcInsert, nLength );
		m_pInputBuffer[nInsertPos + nLength] = NULL;
	}
	else
	{
		// Split string
		int	nCopyLength = m_nCurCharCount - nInsertPos;
		memcpy( (void *)m_pTempBuffer, (void *)&(m_pInputBuffer[nInsertPos]), nCopyLength );
		m_pTempBuffer[nCopyLength] = NULL;

		// Insert characters
		memcpy( (void *)&(m_pInputBuffer[nInsertPos]), (void *)pcInsert, nLength );

		// Combine strings
		memcpy( (void *)&(m_pInputBuffer[nInsertPos + nLength]), (void *)m_pTempBuffer, nCopyLength + 1 );
	}

	// Increase cursor index and current character count
	m_nCursorIndex += nLength;
	m_nCurCharCount += nLength;

#if defined(G_RUSSIA)
	m_nShowCharCount = UtilHelp::getSingleton()->CheckNoFixedLength(_pfdDefaultFont, &m_pInputBuffer[m_nFirstShowChar], m_nWidth);
#endif
	// Adjust position of first shown character
	if( m_nCursorIndex > m_nFirstShowChar + m_nShowCharCount )
	{
		m_nFirstShowChar = m_nCursorIndex - m_nShowCharCount;

		if( Is2ByteChar( m_nFirstShowChar ) )
			m_nFirstShowChar++;
	}
}

// ----------------------------------------------------------------------------
// Name : InsertIMEChar()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::InsertIMEChar( char *pcInsert, BOOL bOnComposition )
{
	// Start composition
	if( !m_bOnComposition && bOnComposition )
	{
		// If input buffer is not enough
		if( m_nCurCharCount >= m_nMaxCharCount - 1 )
			return;

		// If cursor locates in last position of input string
		if( m_nCursorIndex == m_nCurCharCount )
		{
			// Insert characters
			m_pInputBuffer[m_nCursorIndex] = pcInsert[0];
			m_pInputBuffer[m_nCursorIndex + 1] = pcInsert[1];
			m_pInputBuffer[m_nCursorIndex + 2] = NULL;
		}
		else
		{
			// Split string
			int	nCopyLength = m_nCurCharCount - m_nCursorIndex;

			if( nCopyLength > 0 )
			{
				memcpy( (void *)m_pTempBuffer, (void *)&(m_pInputBuffer[m_nCursorIndex]), nCopyLength );
				m_pTempBuffer[nCopyLength] = NULL;

				// Insert characters
				m_pInputBuffer[m_nCursorIndex] = pcInsert[0];
				m_pInputBuffer[m_nCursorIndex + 1] = pcInsert[1];

				// Combine strings
				memcpy( (void *)&(m_pInputBuffer[m_nCursorIndex + 2]), (void *)m_pTempBuffer, nCopyLength + 1 );
			}
			else
			{
				ResetString ();
				StopComposition ();
			}
		}

		// Increase current character count
		m_nCurCharCount += 2;

		// Adjust position of first shown character
		if( m_nCursorIndex + 2 > m_nFirstShowChar + m_nShowCharCount )
		{
			m_nFirstShowChar = m_nCursorIndex + 2 - m_nShowCharCount;

			if( Is2ByteChar( m_nFirstShowChar ) )
				m_nFirstShowChar++;
		}

		m_bOnComposition = TRUE;
	}
	// Composing now
	else if( m_bOnComposition && bOnComposition )
	{
		// If composing string is eliminated by backspace
		if( !pcInsert[0] && !pcInsert[1] )
		{
			int	nCopyLength = m_nCurCharCount - ( m_nCursorIndex + 2 );
			if( nCopyLength > 0 )
			{
				// Split string
				memcpy( (void *)m_pTempBuffer, (void *)&(m_pInputBuffer[m_nCursorIndex + 2]), nCopyLength );
				m_pTempBuffer[nCopyLength] = NULL;

				// Combine strings
				memcpy( (void *)&(m_pInputBuffer[m_nCursorIndex]), (void *)m_pTempBuffer, nCopyLength + 1 );
			}
			else
			{
				m_pInputBuffer[m_nCursorIndex] = NULL;
			}

			// Decrease current character count
			m_nCurCharCount -= 2;

			m_bOnComposition = FALSE;
			
		}
		else
		{
			// Insert characters
			m_pInputBuffer[m_nCursorIndex] = pcInsert[0];
			m_pInputBuffer[m_nCursorIndex + 1] = pcInsert[1];
		}
	}
	// Complete composition
	else if( m_bOnComposition && !bOnComposition )
	{
#if !defined(G_JAPAN)
		{
			// 한자를 필터링 한다.
			const unsigned char tmpChar = 0xC9;
			unsigned char cmpChar = static_cast<unsigned char>(pcInsert[0]);
			
			if( cmpChar >= tmpChar )
			{
				HWND hWnd = ImmGetDefaultIMEWnd( _hwndMain );
				HIMC hImc = ImmGetContext( hWnd );
				
				ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_COMPLETE, 0 );
				
				ImmReleaseContext ( hWnd, hImc );
				
				s_bShowReadingWindow = FALSE;
				ZeroMemory ( s_szReadingString, sizeof ( s_szReadingString ) );
				s_nReadingStringLength = 0;
				ZeroMemory ( &s_CandList, sizeof ( s_CandList ) );
				
				pcInsert[0] = 0;
				pcInsert[1] = 0;
				
				m_bOnComposition = FALSE;
				bSecondComposition = FALSE;
				bOnComposition = false;
			}
			else
			{
				// Insert characters
				m_pInputBuffer[m_nCursorIndex] = pcInsert[0];
				m_pInputBuffer[m_nCursorIndex + 1] = pcInsert[1];
				
				// Increase cursor index
				m_nCursorIndex += 2;
				
				m_bOnComposition = FALSE;
			}
		}
#else	// g_iCountry == JAPAN
		{
			// Insert characters
			m_pInputBuffer[m_nCursorIndex] = pcInsert[0];
			m_pInputBuffer[m_nCursorIndex + 1] = pcInsert[1];
			// Increase cursor index
			m_nCursorIndex += 2;
			m_bOnComposition = FALSE;
		}
#endif
	}
}

// ----------------------------------------------------------------------------
// Name : DeleteChar()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::DeleteChar( int nDeletePos )
{
	// If current character is 2 byte character
//	if(Is2ByteChar(nDeletePos))
	// connie [2009/9/11] - 
	if( (m_pInputBuffer[nDeletePos] & 0x80)  && (g_iCountry != THAILAND ) && (g_iCountry != BRAZIL ) && (g_iCountry != GERMANY ) 
		&& (g_iCountry != USA_SPAIN) && (g_iCountry != SPAIN) && (g_iCountry != MEXICO) && (g_iCountry != FRANCE) && (g_iCountry != POLAND)
		&& (g_iCountry != TURKEY) && (g_iCountry != RUSSIA) && (g_iCountry != ITALY) && (g_iCountry != NETHERLANDS)
		&& !(g_iCountry == JAPAN && ((UCHAR)m_pInputBuffer[nDeletePos] >= 0xa1 && (UCHAR)m_pInputBuffer[nDeletePos] <= 0xdf)) )
		{
			if( m_nCurCharCount < 2 || m_nCurCharCount == nDeletePos )
				return;

			int	nCopyLength = m_nCurCharCount - ( nDeletePos + 2 );
			if( nCopyLength > 0 )
			{
				// Split string
				memcpy( (void *)m_pTempBuffer, (void *)&(m_pInputBuffer[nDeletePos + 2]), nCopyLength );
				m_pTempBuffer[nCopyLength] = NULL;

				// Combine strings
				memcpy( (void *)&(m_pInputBuffer[nDeletePos]), (void *)m_pTempBuffer, nCopyLength + 1 );
			}
			else
			{
				m_pInputBuffer[nDeletePos] = NULL;
			}

			// Decrease cursor position and current character count
			m_nCurCharCount -= 2;
			if( nDeletePos < m_nCursorIndex )
				m_nCursorIndex -= 2;
		}
	
		else
		{
			if( m_nCurCharCount < 1 || m_nCurCharCount == nDeletePos )
				return;

			int	nCopyLength = m_nCurCharCount - ( nDeletePos + 1 );
			if( nCopyLength > 0 )
			{
				// Split string
				memcpy( (void *)m_pTempBuffer, (void *)&(m_pInputBuffer[nDeletePos + 1]), nCopyLength );
				m_pTempBuffer[nCopyLength] = NULL;

				// Combine strings
				memcpy( (void *)&(m_pInputBuffer[nDeletePos]), (void *)m_pTempBuffer, nCopyLength + 1 );
			}
			else
			{
				m_pInputBuffer[nDeletePos] = NULL;
			}

			// Decrease cursor position and current character count
			m_nCurCharCount--;
			if( nDeletePos < m_nCursorIndex )
				m_nCursorIndex--;
		}
	
}

// ----------------------------------------------------------------------------
// Name : DeleteChars()
// Desc :
// ----------------------------------------------------------------------------
void CUIEditBox::DeleteChars( int nDeletePos, int nCharCount )
{
	if( m_nCurCharCount < nCharCount || m_nCurCharCount == nDeletePos )
		return;

	int	nCopyLength = m_nCurCharCount - ( nDeletePos + nCharCount );
	if( nCopyLength > 0 )
	{
		// Split string
		memcpy( (void *)m_pTempBuffer, (void *)&(m_pInputBuffer[nDeletePos + nCharCount]), nCopyLength );
		m_pTempBuffer[nCopyLength] = NULL;

		// Combine strings
		memcpy( (void *)&(m_pInputBuffer[nDeletePos]), (void *)m_pTempBuffer, nCopyLength + 1 );
	}
	else
	{
		m_pInputBuffer[nDeletePos] = NULL;
	}

	// Decrease cursor position and current character count
	m_nCurCharCount -= nCharCount;
	if( nDeletePos < m_nCursorIndex )
		m_nCursorIndex -= nCharCount;
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIEditBox::MouseMessage( MSG *pMsg )
{
	// If edit box is not enabled
	if( !IsEnabled() || m_InValidEditBox )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	if( pMsg->message == WM_LBUTTONDOWN )
	{
		if( IsInside( nX, nY ) )
		{
			SetFocus( TRUE );

			// [2011/11/02 : Sora] 커서 이동 불가플래그
			if( m_nCurCharCount > 0 && m_bMoveCursor )
			{
				int	nPos = 0;
#ifdef G_RUSSIA
				nPos = UTIL_HELP()->CheckNoFixedLength(_pfdDefaultFont, &m_pInputBuffer[m_nFirstShowChar], nX - GetAbsPosX());
#else
				int	nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
				int	nAbsX = GetAbsPosX() - nFontWidth / 2;
				nPos = ( nX - nAbsX ) / nFontWidth;
#endif // G_RUSSIA
				if( nPos >= 0 )
				{
					 if( nPos > m_nShowCharCount )
						 nPos = m_nShowCharCount;
					 if( nPos + m_nFirstShowChar > m_nCurCharCount )
						 nPos = m_nCurCharCount - m_nFirstShowChar;

					m_nCursorIndex = nPos + m_nFirstShowChar;
					if( Is2ByteChar( m_nCursorIndex, m_nFirstShowChar ) )
						m_nCursorIndex++;
				}
			}

			return WMSG_SUCCESS;
		}

		SetFocus( FALSE );

	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIEditBox::KeyMessage( MSG *pMsg )
{
	// If edit box is not focused
	if( !IsFocused() || m_InValidEditBox )
	{
		return WMSG_FAIL;
	}

	// [2011/11/02 : Sora] 커서 이동 불가플래그
	if( pMsg->wParam == VK_RETURN )
	{
		ResetSelArea();

		if( m_pInputBuffer[0] )
			return WMSG_COMMAND;
		else
			return WMSG_SUCCESS;
	}
	else if( pMsg->wParam == VK_LEFT )
	{
		if( m_bMoveCursor )
		{
			SetCopyStartPos();
			MoveCursor( MD_LEFT );
			SetCopyPos();
		}
		return WMSG_SUCCESS;
	}
	else if( pMsg->wParam == VK_RIGHT )
	{
		if( m_bMoveCursor )
		{
			SetCopyStartPos();
			MoveCursor( MD_RIGHT );
			SetCopyPos();
		}
		return WMSG_SUCCESS;
	}
	else if( pMsg->wParam == VK_HOME )
	{
		if( m_bMoveCursor )
		{
			SetCopyStartPos();
			MoveCursor( MD_HOME );
			SetCopyPos();
		}
		return WMSG_SUCCESS;
	}
	else if( pMsg->wParam == VK_END )
	{
		if( m_bMoveCursor )
		{
			SetCopyStartPos();
			MoveCursor( MD_END );	
			SetCopyPos();
		}
		return WMSG_SUCCESS;

	}
	else if( pMsg->wParam == VK_DELETE )
	{
		if( m_bMoveCursor )
		{
			if (m_bCopyAndPaste == false || m_nCopyStartPos < 0)
				DeleteChar( m_nCursorIndex );
			else
				DelChars();
		}

		ResetSelArea();
		return WMSG_SUCCESS;
	}
	else if( pMsg->wParam == VK_BACK )
	{
		if( m_nCursorIndex > 0 )
		{
			if (m_bCopyAndPaste == false || m_nCopyStartPos < 0)
			{
				MoveCursor( MD_LEFT );
				DeleteChar( m_nCursorIndex );
			}
			else
			{
				DelChars();
			}		
		}
		else
		{
			if (m_bCopyAndPaste == true && m_nCopyStartPos >= 0)
				DelChars();
		}

		SetCopyPos();
		return WMSG_SUCCESS;
	}

	if ((GetKeyState(VK_CONTROL) & 0x8000))
	{
		if (pMsg->wParam == 'V' || pMsg->wParam == 'v')
		{
			if (Paste(pMsg->hwnd) == true)
			{
				ResetSelArea();
				return WMSG_SUCCESS;
			}
		}
		else if (pMsg->wParam == 'C' || pMsg->wParam == 'c')
		{
			if (Copy(pMsg->hwnd) == true)
				return WMSG_SUCCESS;
		}
		else if (pMsg->wParam == 'X' || pMsg->wParam == 'x')
		{
			if (Copy(pMsg->hwnd) == true)
			{
				DelChars();
				ResetSelArea();
				return WMSG_SUCCESS;
			}
		}
	}

	// 이기환 수정 시작 (11. 15) : 한글 조합창 보이지 않도록
	/*else if ( pMsg->wParam == VK_PROCESSKEY )
	{
		return WMSG_SUCCESS;
	}
	*/
	// 이기환 수정 끝 (11.15)
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc : Only WM_CHAR
// ----------------------------------------------------------------------------
WMSG_RESULT CUIEditBox::CharMessage( MSG *pMsg )
{
	extern UBYTE	_abKeysPressed[256];

	// If editbox is not focused
	if( !IsFocused() || m_InValidEditBox )
		return WMSG_FAIL;

	if (m_bCopyAndPaste)
	{
		DelChars();
		ResetSelArea();
	}

	if(m_bOnlyInteger && (pMsg->wParam < 48 || pMsg->wParam > 57))
	{
		return WMSG_SUCCESS;
	}

	if (g_iCountry == THAILAND) {

		if(_abKeysPressed[KID_LCONTROL] || _abKeysPressed[KID_RCONTROL] || _abKeysPressed[KID_ESCAPE])
			return WMSG_FAIL;

		unsigned char cInsert = (unsigned char)(pMsg->wParam);
		unsigned char pre_cInsert = NULL;
		// wooss 051001
		// vowel display
		if(cInsert==0xd3){
			if(m_nCursorIndex==0) return WMSG_SUCCESS;
			pre_cInsert = m_pInputBuffer[m_nCursorIndex-1];
			if(!((pre_cInsert>=0xa1 && pre_cInsert <= 0xcf )||pre_cInsert == 0xed || pre_cInsert == 0xee)) return WMSG_SUCCESS;
		}else if(cInsert==0xed ||cInsert==0xd1 || (cInsert>=0xd4 &&cInsert<=0xda)) {
			if(m_nCursorIndex==0) return WMSG_SUCCESS;
			pre_cInsert = m_pInputBuffer[m_nCursorIndex-1];
			if(!((pre_cInsert>=0xa1 && pre_cInsert <= 0xcf)||
				(pre_cInsert>=0xe7 && pre_cInsert <= 0xee))) return WMSG_SUCCESS;
		} 
		// tone indication display 
		else if( cInsert>=0xe7 && cInsert<=0xec) {
			if(m_nCursorIndex==0) return WMSG_SUCCESS;
			pre_cInsert = m_pInputBuffer[m_nCursorIndex-1];
			if(!((pre_cInsert == 0xed) || 
				(pre_cInsert == 0xee) || 
				(pre_cInsert == 0xd1) || 
				(pre_cInsert>=0xd4 && pre_cInsert<=0xda) || 
				(pre_cInsert>=0xa1 && pre_cInsert <=0xcf))) return WMSG_SUCCESS; 
		}
		InsertChar( m_nCursorIndex, pMsg->wParam );
	}
	// connie [2009/9/11] - 
	else if(g_iCountry == BRAZIL || g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == MEXICO || g_iCountry == FRANCE || g_iCountry == POLAND
			|| g_iCountry == USA_SPAIN || g_iCountry == TURKEY || g_iCountry == ITALY || g_iCountry == NETHERLANDS)//FRANCE_SPAIN_CLOSEBETA_NA_20081124 
#ifdef ADD_CHAT_SPECIALCHAR_DISABLE_NA_20090210
	{
		if ( IsGamigo( g_iCountry ) )
		{
// 			if ( (_pInput->GetInputDevice(1)->id_aicControls[KID_LSHIFT].ic_fValue == 1.0f ) 
// 				|| (_pInput->GetInputDevice(1)->id_aicControls[KID_RSHIFT].ic_fValue == 1.0f) );
			
			INDEX	iScanCode = ( pMsg->lParam >> 16 ) & 0x1FF;
			if (( _abKeysPressed[KID_LSHIFT] && _abKeysPressed[KID_LCONTROL]) 
				|| (_abKeysPressed[KID_RSHIFT] && _abKeysPressed[KID_RCONTROL])
				|| (_abKeysPressed[KID_LSHIFT] && _abKeysPressed[KID_RCONTROL])
				|| (_abKeysPressed[KID_RSHIFT] && _abKeysPressed[KID_LCONTROL]) );
			else if( g_iCountry == ITALY )
			{
				if(_abKeysPressed[KID_LCONTROL] || _abKeysPressed[KID_RCONTROL])
				{
					if(iScanCode == 38) // iScanCode ==  'l'
					{
						InsertChar(m_nCursorIndex, (char)243);
					}
					else if(iScanCode == 36 || iScanCode == 37)
					{
						InsertChar(m_nCursorIndex, (char)(iScanCode+174));
					}
					else if(iScanCode == 50) // iScanCode == 'M'
					{
						InsertChar(m_nCursorIndex, (char)109);
					}
				}
				else
				{
					InsertChar(m_nCursorIndex, pMsg->wParam);
				}
			}
			else if ( g_iCountry == FRANCE && (_abKeysPressed[KID_LCONTROL] || _abKeysPressed[KID_RCONTROL]) && iScanCode == 39)
			{	// 프랑스는 m의 스캔코드 값이 39로 되어있음.
				InsertChar(m_nCursorIndex, (char)109);
			}
			else if ((_abKeysPressed[KID_LCONTROL] || _abKeysPressed[KID_RCONTROL]) && iScanCode == 50 ) // iScanCode == 'M'
			{
				InsertChar(m_nCursorIndex, (char)109);
			}
			else if ((_abKeysPressed[KID_LCONTROL] || _abKeysPressed[KID_RCONTROL]) && iScanCode == 36 ) // iScanCode == 'J'
			{
				InsertChar(m_nCursorIndex, (char)106);
			}		
			else
				InsertChar(m_nCursorIndex, pMsg->wParam);
		}
		else
			InsertChar(m_nCursorIndex, pMsg->wParam);
	}
#else
		if(g_iCountry == ITALY)
		{
			extern UBYTE	_abKeysPressed[256];

			if(_abKeysPressed[KID_LCONTROL])
			{
				INDEX	iScanCode = ( pMsg->lParam >> 16 ) & 0x1FF;
				if(iScanCode == 38) // iScanCode ==  'l'
				{
					InsertChar(m_nCursorIndex, (char)243);
				}
				else if(iScanCode == 36 || iScanCode == 37)
				{
					InsertChar(m_nCursorIndex, (char)(iScanCode+174));
				}
			}
			else
			{
				InsertChar(m_nCursorIndex, pMsg->wParam);
			}
		}
		else
		{
			InsertChar(m_nCursorIndex, pMsg->wParam);
		}
#endif
	else if(g_iCountry == JAPAN && ( pMsg->wParam >= 0xa1 && pMsg->wParam <=0xdf ))
		InsertChar( m_nCursorIndex, pMsg->wParam );
	else if( g_iCountry == RUSSIA )
	{
		if( ( pMsg->wParam >= 32 && pMsg->wParam < 128 ) || ( pMsg->wParam >= 192 && pMsg->wParam <= 255 ) || 
			( pMsg->wParam == 168 || pMsg->wParam == 184 ) )
		{
			InsertChar( m_nCursorIndex, pMsg->wParam );
		}
	}
	else if(pMsg->wParam & 0x80) m_b2ByteChar  = !m_b2ByteChar;
	else if( m_b2ByteChar ) m_b2ByteChar = FALSE;
	else if( pMsg->wParam >= 32 && pMsg->wParam < 128 )	InsertChar( m_nCursorIndex, pMsg->wParam );

	return WMSG_SUCCESS;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage() 
// Desc : 현재 설정된 국가별로 관련 함수 호출 
//		- 이기환 수정 (11. 15) - IME 관련 작업
// ----------------------------------------------------------------------------
WMSG_RESULT CUIEditBox::IMEMessage( MSG *pMsg )
{
	if (m_InValidEditBox) return WMSG_FAIL;

// 이기환 추가 (11.18) : 패스워드 및 EnterChatting 이 아닐 때 composition 중지
	if( m_bOnlyInteger || m_bIsPWEditBox && !m_bMsgInput )
	{
		SetEngMode( _hwndMain );	// Date : 2005-01-17,   By Lee Ki-hwan
		StopComposition();
		return WMSG_FAIL;
	}
	
		
	// 이기환 수정 끝 
	// _pUIFontTexMgr->GetLanguage() ==> g_iCountry로 수정 wooss 051107
	switch ( g_iCountry)
	{
	case KOREA : 
		return IMEMessageKOR ( pMsg );

	case TAIWAN :
	case TAIWAN2 :
	case HONGKONG :
		// If editbox is not focused
		// 이기환 주석 처리(05.01.04): 한국어 버젼에서 멀티 에디트 박스에서 중지 시켜 버리는 경우가 있음 
		//                           중국어 버젼 개발 시 참고....
		extern INDEX	g_iEnterChat;
		if( !IsFocused() && g_iEnterChat )
		{
			StopComposition ();	
			m_bOnComposition = FALSE;
			return WMSG_FAIL;
		}

		return IMEMessageCHT ( pMsg );

	case CHINA : 
		return IMEMessageCHS ( pMsg );
	
	case THAILAND :
		return IMEMessageTHAI ( pMsg );

	case JAPAN :
	/*	extern INDEX	g_iEnterChat;
		if( !IsFocused() && g_iEnterChat )
		{
			
			StopComposition ();	
			m_bOnComposition = FALSE;
			return WMSG_FAIL;
		}*/

	//	SendMessage ( _hwndMain, WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW, NULL );
		return IMEMessageJPN( pMsg );

	case MALAYSIA:
		// If editbox is not focused
		// 이기환 주석 처리(05.01.04): 한국푳E버젼에서 멀티 에디트 박스에서 중햨E시켜 버리는 경퓖E?있음 
		//                           중국푳E버젼 개발 시 혖E갋...
		extern INDEX	g_iEnterChat;
		if( !IsFocused() && g_iEnterChat )
		{
			
			StopComposition ();	
			m_bOnComposition = FALSE;
			return WMSG_FAIL;
		}

		return IMEMessageCHT ( pMsg );
	case USA:
		return IMEMessageUSA ( pMsg );	
	case BRAZIL:
	case GERMANY:
	case RUSSIA:
	case SPAIN://FRANCE_SPAIN_CLOSEBETA_NA_20081124
	case FRANCE:
	case POLAND:
	case TURKEY:
	case MEXICO:
	case ITALY:
	case USA_FRANCE:
	case USA_SPAIN:
	case NETHERLANDS:
		return IMEMessageBRZ ( pMsg );
	}
	

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc : 한국어 버전 IMEMessage 
//		- 이기환 수정 (11.15)
// ----------------------------------------------------------------------------
WMSG_RESULT CUIEditBox::IMEMessageKOR( MSG *pMsg )
{
	// If editbox is not focused
	if( !IsFocused() )
		return WMSG_FAIL;

	int		nLength;
	char	cString[5] = { 0 };
	
	static bool bOnComposition = false;

	switch( pMsg->message )
	{
	case WM_SYSKEYDOWN :
		if ( pMsg->wParam == VK_MENU && m_bOnComposition )
		{
			
			InsertIMEChar( cString );

			HWND hWnd = ImmGetDefaultIMEWnd( _hwndMain );
			HIMC hImc = ImmGetContext( hWnd );
		
			ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_COMPLETE, 0 );
		
			ImmReleaseContext ( hWnd, hImc );

			s_bShowReadingWindow = FALSE;
			ZeroMemory ( s_szReadingString, sizeof ( s_szReadingString ) );
			s_nReadingStringLength = 0;
			ZeroMemory ( &s_CandList, sizeof ( s_CandList ) );
		
			cString[0] = 0;
			cString[1] = 0;
	
		//	InsertIMEChar( cString );
			
			bSecondComposition = FALSE;
			bOnComposition = false;
		}
		return WMSG_FAIL;

/*
	case WM_KEYDOWN :
		return KeyMessage ( pMsg );
*/
	case WM_IME_STARTCOMPOSITION:
		return WMSG_SUCCESS;

	case WM_IME_CHAR:
		
		return WMSG_SUCCESS;

	case WM_IME_COMPOSITION:
		{
			static char cOldString[5] ="\n";

			HIMC	hImc = ImmGetContext( _hwndMain );
			// Complete composition
			if( pMsg->lParam & GCS_RESULTSTR ) 
			{
				if( ( nLength = ImmGetCompositionString( hImc, GCS_RESULTSTR, NULL, 0 ) ) > 0 )
				{
					ImmGetCompositionString( hImc, GCS_RESULTSTR, cString, nLength );
					cString[nLength] = NULL;
				
					// Date : 2005-06-03(오전 11:39:38), By Lee Ki-hwan
					/*	3벌식 숫자 입력 문제 처리 
						2Byte가 아닐때는 조합하지 말고 그냥 입력하시오
					*/
					if( nLength <= 1 ) 
					{
						InsertChars( m_nCursorIndex, cString );
					}
					else
					{
						InsertIMEChar( cString, FALSE );
						strcpy( cOldString, cString );
					}
					bSecondComposition = false;
					bOnComposition = false;

				}
			}
			// Composing now
			else if( pMsg->lParam & GCS_COMPSTR )
			{
				nLength = ImmGetCompositionString( hImc, GCS_COMPSTR, NULL, 0 );
				ImmGetCompositionString( hImc, GCS_COMPSTR, cString, nLength );
			
				if ( m_bOnComposition )
				{
					if ( bOnComposition )
					{
						if ( strcmp ( cOldString, cString ) == 0 ) 
						{
							bSecondComposition = false;
						}
						else 
						{
							bSecondComposition = true;
						}
					}
					else
					{
						bOnComposition = true;
					}
					
				}
				else 	
					strcpy ( cOldString, cString );

				
				InsertIMEChar( cString );
			}

			ImmReleaseContext( _hwndMain, hImc );
		}

		if (m_bCopyAndPaste)
		{
			DelChars();
			ResetSelArea();
		}

		return WMSG_SUCCESS;

	// 이기환 수정 시작 (11. 17) : 언어 변경 시 
	case WM_INPUTLANGCHANGE :	
	case WM_INPUTLANGCHANGEREQUEST :
		{
		//	CheckInputLocal ();		// 언어 체크
		}
		break;	
	// 이기환 수정 끝 (11. 17) 

	case WM_IME_ENDCOMPOSITION:	
		bSecondComposition = false;
		bOnComposition = false;
      	return WMSG_COMMAND;
	
	case WM_IME_NOTIFY:
		{
			switch( pMsg->wParam )
			{
				case NI_CHANGECANDIDATELIST:
				case IMN_OPENCANDIDATE:	
				case IMN_CHANGECANDIDATE:
					{
						CUIManager* pUIManager = CUIManager::getSingleton();

						// 개명카드 사용시에도 특수 문자를 넣지 못하게 수정한다. 060217 wooss
						if ( bSecondComposition || ( STAGEMGR()->GetCurStage() == eSTAGE_CREATECHAR )
								|| pUIManager->DoesMessageBoxExist(MSGCMD_USE_CHANGE_MY_NAME_ITEM) )
						{
							// 이기환 수정 시작 (11.23) : 한글 특수 문자 입력 막기 ( 글자 입력 취소됨 )
							HWND hWnd = ImmGetDefaultIMEWnd( _hwndMain );
							HIMC hImc = ImmGetContext( hWnd );
						
							ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_COMPLETE, 0 );
						
							ImmReleaseContext ( hWnd, hImc );

							s_bShowReadingWindow = FALSE;
							ZeroMemory ( s_szReadingString, sizeof ( s_szReadingString ) );
							s_nReadingStringLength = 0;
							ZeroMemory ( &s_CandList, sizeof ( s_CandList ) );

							cString[0] = 0;
							cString[1] = 0;

							InsertIMEChar( cString );
							
							bSecondComposition = FALSE;
							bOnComposition = false;
						}
						else
						{
							GetCand ();	
							SendMessage( _hwndMain, IMN_CLOSECANDIDATE, 0, 0 );
						}

						// 이기환 수정 완료 (11.23)
					}
					break;
				
				case IMN_CLOSECANDIDATE:
					{
						s_CandList.dwCount = 0;
						s_CandList.bShowWindow = false;
						//bSecondComposition = false;

						if( !s_bShowReadingWindow )
						{
							ZeroMemory( s_CandList.szCandidate, sizeof(s_CandList.szCandidate) );

							s_bShowReadingWindow = FALSE;
						}
					}
					break;
			} // switch( pMsg->wParam )
		}
		return WMSG_SUCCESS;

	}  // switch ( pMsg->message )

	return WMSG_FAIL;
}


// ----------------------------------------------------------------------------
// Name : IMEMessageCHT()
// Desc : 대만어 버젼 IMEMessage  
//		- 이기환 작성 (11.15)
// ----------------------------------------------------------------------------
WMSG_RESULT CUIEditBox::IMEMessageCHT( MSG *pMsg )
{	
	// If editbox is not focused
	if( !IsFocused() )
		return WMSG_FAIL;

	switch( pMsg->message )
	{
		case WM_IME_STARTCOMPOSITION :
			{
				m_bOnComposition = true;
			}
			return WMSG_SUCCESS;

		// 이기환 수정 시작 (11. 17) : 언어 변경 시 
		case WM_INPUTLANGCHANGE :	
		case WM_INPUTLANGCHANGEREQUEST :
			{
				s_bShowReadingWindow = false;
				s_CandList.dwCount = 0;
				s_CandList.bShowWindow = false;
				
				if( !s_bShowReadingWindow )
				{
					ZeroMemory( s_CandList.szCandidate, sizeof(s_CandList.szCandidate) );
				}
				//	CheckInputLocal ();		// 언어 체크
			}
			break;	
		// 이기환 수정 끝 (11. 17) 

		case WM_IME_COMPOSITION:
			{
				HIMC	hImc;
				char	cString[256] = { 0 };
				int		nLength;
				// hongkong new changjie IME 관련 버그 수정. [9/29/2010 rumist]
				/*	IME는 순서상 WM_IME_STARTCOMPOSITION -> WM_IME_COMPOSITION -> WM_IME_ENDCOMPOSITION
					로 처리되어야 하는데 new changjie의 경우 일반 ascii코드의 경우에도 WM_IME_COMPOSITION
					가 호출됨으로서 일반 ascii코드가 두번 입력되는 현상 발생. 이에 
					WM_IME_STARTCOMPOSITION의 호출 여부를 판단하여 현재 글이 IME인지 아닌지를 판단하도록 만듬. */
				if( !m_bOnComposition )
					return WMSG_FAIL;

				hImc = ImmGetContext( _hwndMain );
				
				if( pMsg->lParam & GCS_RESULTSTR )
				{
					if( ( nLength = ImmGetCompositionString( hImc, GCS_RESULTSTR, NULL, 0 ) ) > 0 )
					{
						ImmGetCompositionString( hImc, GCS_RESULTSTR, cString, nLength );
						cString[nLength] = NULL;
						
						InsertChars( m_nCursorIndex, cString );
						
						memset( s_szReadingString, 0, sizeof  s_szReadingString );
					}
				}

				// Composing now
				else if( pMsg->lParam & GCS_COMPSTR )
				{
					nLength = ImmGetCompositionString( hImc, GCS_COMPSTR, NULL, 0 );
					ImmGetCompositionString( hImc, GCS_COMPSTR, cString, nLength );
					cString[nLength] = 0;
					s_bShowReadingWindow = true;
					strcpy ( s_szReadingString, cString );
				}
				ImmReleaseContext( _hwndMain, hImc );
			}
			return WMSG_SUCCESS;
			
		case WM_IME_ENDCOMPOSITION:
			{
       			m_bOnComposition = false;
				s_bShowReadingWindow = false;
			}
			return WMSG_COMMAND;

		case WM_IME_NOTIFY:
			switch( pMsg->wParam )
			{
						
				// IME 선택 창
				case IMN_OPENCANDIDATE:	
				case IMN_CHANGECANDIDATE:
					{
						GetCand ();	
						SendMessage( _hwndMain, IMN_CLOSECANDIDATE, 0, 0 );
					}
					break;
					  
				case IMN_CLOSECANDIDATE:
					{
						s_CandList.dwCount = 0;
						s_CandList.bShowWindow = false;
						
						if( !s_bShowReadingWindow )
						{
							ZeroMemory( s_CandList.szCandidate, sizeof(s_CandList.szCandidate) );
						}
					}
					break;
				// for Test ...
				case IMN_CLOSESTATUSWINDOW :
						break;
				case IMN_OPENSTATUSWINDOW  :
						break;
				case IMN_SETCONVERSIONMODE :
						break;
				case IMN_SETSENTENCEMODE   :
						break;
				case IMN_SETOPENSTATUS     :
						break;
				case IMN_SETCANDIDATEPOS   :
						break;
				case IMN_SETCOMPOSITIONFONT:    
						break;
				case IMN_SETCOMPOSITIONWINDOW:
						break;
				case IMN_SETSTATUSWINDOWPOS:
						break;
				case IMN_GUIDELINE:
						break;
				case IMN_PRIVATE:
						break;
			}  
			return WMSG_SUCCESS;
	} // switch( pMsg->message )
	
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessageCHS()
// Desc : 중국어 버전 IMEMessage 
//		- 05. 2. 23 : 우선은 대만 버전과 동일하게 처리 
//		- 05. 3. 08 : 원도우에서 기본으로 지원하는 모든 IME 적용 
//					ReadingWindow 표기를 StartComposition과 EndComposition에서 
//					조정 ( 기존의 방식은 WM_IME_COMPOSITION에서 처리 )
// ----------------------------------------------------------------------------
WMSG_RESULT CUIEditBox::IMEMessageCHS( MSG *pMsg )
{
	// If editbox is not focused
	if( !IsFocused() )
		return WMSG_FAIL;

	switch( pMsg->message )
	{
		case WM_IME_STARTCOMPOSITION :
			{
				m_bOnComposition = true;
				s_bShowReadingWindow = true;	// Date : 2005-03-09,   By Lee Ki-hwan
			}
			return WMSG_SUCCESS;

		// 이기환 수정 시작 (11. 17) : 언어 변경 시 
		case WM_INPUTLANGCHANGE :	
		case WM_INPUTLANGCHANGEREQUEST :
			{
				s_bShowReadingWindow = false;
				s_CandList.dwCount = 0;
				s_CandList.bShowWindow = false;
				
				if( !s_bShowReadingWindow )
				{
					ZeroMemory( s_CandList.szCandidate, sizeof(s_CandList.szCandidate) );
				}
				//	CheckInputLocal ();		// 언어 체크
			}
			break;	
		// 이기환 수정 끝 (11. 17) 

		case WM_IME_COMPOSITION:
			{
				HIMC	hImc;
				char	cString[256] = { 0 };
				int		nLength;

				hImc = ImmGetContext( _hwndMain );
				
				if( pMsg->lParam & GCS_RESULTSTR )
				{
					if( ( nLength = ImmGetCompositionString( hImc, GCS_RESULTSTR, NULL, 0 ) ) > 0 )
					{
						ImmGetCompositionString( hImc, GCS_RESULTSTR, cString, nLength );
						cString[nLength] = NULL;
						
						InsertChars( m_nCursorIndex, cString );
						
						memset( s_szReadingString, 0, sizeof  s_szReadingString );
					}
				}

				// Composing now
				else if( pMsg->lParam & GCS_COMPSTR )
				{
					nLength = ImmGetCompositionString( hImc, GCS_COMPSTR, NULL, 0 );
					
					if ( nLength > 0 ) 
					{
						ImmGetCompositionString( hImc, GCS_COMPSTR, cString, nLength );
						cString[nLength] = 0;
						strcpy ( s_szReadingString, cString );
					}
				}
				ImmReleaseContext( _hwndMain, hImc );
			}
			return WMSG_SUCCESS;
			
		case WM_IME_ENDCOMPOSITION:
			{
       			m_bOnComposition = false;
				s_bShowReadingWindow = false;
			}
			return WMSG_COMMAND;

		case WM_IME_NOTIFY:
			switch( pMsg->wParam )
			{
						
				// IME 선택 창
				case IMN_OPENCANDIDATE:	
				case IMN_CHANGECANDIDATE:
					{
						GetCand ();	
						SendMessage( _hwndMain, IMN_CLOSECANDIDATE, 0, 0 );
					}
					break;
					  
				case IMN_CLOSECANDIDATE:
					{
						s_CandList.dwCount = 0;
						s_CandList.bShowWindow = false;
						
						if( !s_bShowReadingWindow )
						{
							ZeroMemory( s_CandList.szCandidate, sizeof(s_CandList.szCandidate) );
						//	s_bShowReadingWindow = false;
						}
						///s_bShowReadingWindow = false;
					}
					break;
				// for Test ...
				case IMN_CLOSESTATUSWINDOW :
						break;
				case IMN_OPENSTATUSWINDOW  :
						break;
				case IMN_SETCONVERSIONMODE :
						break;
				case IMN_SETSENTENCEMODE   :
						break;
				case IMN_SETOPENSTATUS     :
						break;
				case IMN_SETCANDIDATEPOS   :
						break;
				case IMN_SETCOMPOSITIONFONT:    
						break;
				case IMN_SETCOMPOSITIONWINDOW:
						break;
				case IMN_SETSTATUSWINDOWPOS:
						break;
				case IMN_GUIDELINE:
						break;
				case IMN_PRIVATE:
						break;

			}  
			return WMSG_SUCCESS;
	} // switch( pMsg->message )
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessageJPN()
// Desc : 일본어 버전 IMEMessage 
//	
// ----------------------------------------------------------------------------
WMSG_RESULT CUIEditBox::IMEMessageJPN( MSG *pMsg )
{
	// wooss 051107
	// If editbox is not focused
	if( !IsFocused() )

		return WMSG_FAIL;

	switch( pMsg->message )
	{
		case WM_IME_STARTCOMPOSITION :
			{
				m_bOnComposition = true;
			}
			return WMSG_SUCCESS;

		// 이기환 수정 시작 (11. 17) : 언어 변경 시 
		case WM_INPUTLANGCHANGE :	
		case WM_INPUTLANGCHANGEREQUEST :
			{
				s_bShowReadingWindow = false;
				s_CandList.dwCount = 0;
				s_CandList.bShowWindow = false;
				
				if( !s_bShowReadingWindow )
				{
					ZeroMemory( s_CandList.szCandidate, sizeof(s_CandList.szCandidate) );
				}
				//	CheckInputLocal ();		// 언어 체크
			}
			break;	
		// 이기환 수정 끝 (11. 17) 

		case WM_IME_COMPOSITION:
			{
				HIMC	hImc;
				char	cString[256] = { 0 };
				int		nLength;

				hImc = ImmGetContext( _hwndMain );
				
				if( pMsg->lParam & GCS_RESULTSTR )
				{
					if( ( nLength = ImmGetCompositionString( hImc, GCS_RESULTSTR, NULL, 0 ) ) > 0 )
					{
						ImmGetCompositionString( hImc, GCS_RESULTSTR, cString, nLength );
						cString[nLength] = NULL;
						
						InsertChars( m_nCursorIndex, cString );
						
						memset( s_szReadingString, 0, sizeof  s_szReadingString );
					}
				}

				// Composing now
				else if( pMsg->lParam & GCS_COMPSTR )
				{
					nLength = ImmGetCompositionString( hImc, GCS_COMPSTR, NULL, 0 );
					ImmGetCompositionString( hImc, GCS_COMPSTR, cString, nLength );
					cString[nLength] = 0;
					s_bShowReadingWindow = true;
					strcpy ( s_szReadingString, cString );
				}
				ImmReleaseContext( _hwndMain, hImc );
			}
			return WMSG_SUCCESS;
			
		case WM_IME_ENDCOMPOSITION:
			{
       			m_bOnComposition = false;
				s_bShowReadingWindow = false;
			}
			return WMSG_COMMAND;

		case WM_IME_NOTIFY:
			switch( pMsg->wParam )
			{
						
				// IME 선택 창
				case IMN_OPENCANDIDATE:	
				case IMN_CHANGECANDIDATE:
					{
						GetCand ();	
						SendMessage( _hwndMain, IMN_CLOSECANDIDATE, 0, 0 );
					}
					break;
					  
				case IMN_CLOSECANDIDATE:
					{
						s_CandList.dwCount = 0;
						s_CandList.bShowWindow = false;
						
						if( !s_bShowReadingWindow )
						{
							ZeroMemory( s_CandList.szCandidate, sizeof(s_CandList.szCandidate) );
						}
					}
					break;
				// for Test ...
				case IMN_CLOSESTATUSWINDOW :
						break;
				case IMN_OPENSTATUSWINDOW  :
						break;
				case IMN_SETCONVERSIONMODE :
						break;
				case IMN_SETSENTENCEMODE   :
						break;
				case IMN_SETOPENSTATUS     :
						break;
				case IMN_SETCANDIDATEPOS   :
						break;
				case IMN_SETCOMPOSITIONFONT:    
						break;
				case IMN_SETCOMPOSITIONWINDOW:
						break;
				case IMN_SETSTATUSWINDOWPOS:
						break;
				case IMN_GUIDELINE:
						break;
				case IMN_PRIVATE:
						break;
			}  
			return WMSG_SUCCESS;
	} // switch( pMsg->message )
	
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessageTHAI()
// Desc : 태국어 버전 IMEMessage 
// Date : 050914 wooss		
// ----------------------------------------------------------------------------
WMSG_RESULT CUIEditBox::IMEMessageTHAI( MSG *pMsg )
{
	// IME가 필요없음
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessageUSA()
// Desc : 미국 버전 IMEMessage 
// Date : 061017 eons
// ----------------------------------------------------------------------------
WMSG_RESULT CUIEditBox::IMEMessageUSA( MSG *pMsg )
{
	// IME가 필요없음
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessageBRZ()
// Desc : 브라질 버전 IMEMessage
// Date : 061102 eons
// ----------------------------------------------------------------------------
WMSG_RESULT CUIEditBox::IMEMessageBRZ( MSG *pMsg )
{
	// IME가 필요 없음
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : GetCand()
// Desc : 현재 Candidate의 정보를 얻어와 저장
//		- 이기환 작성 (11.15)
// ----------------------------------------------------------------------------
WMSG_RESULT CUIEditBox::GetCand()
{
	HIMC hImc;
	
	hImc = ImmGetContext( _hwndMain );

	LPCANDIDATELIST lpCandList = NULL;
    DWORD dwLenRequired;
	
	// s_bShowReadingWindow = false;
	
	dwLenRequired =	ImmGetCandidateList ( hImc, 0, lpCandList, 0 );

	if( dwLenRequired ) 
	{
		// 리스트의 길이 대로 메모리 할당
		lpCandList = (LPCANDIDATELIST) new char[dwLenRequired];
		dwLenRequired = ImmGetCandidateList ( hImc, 0, lpCandList, dwLenRequired );
	}
           
	if( lpCandList )
	{
		s_CandList.bShowWindow = true;

		// Update candidate list data
		s_CandList.dwSelection	= lpCandList->dwSelection;
		s_CandList.dwCount		= lpCandList->dwCount;

		int nPageTopIndex = 0;
        
		s_CandList.dwPageSize = min( lpCandList->dwPageSize, MAX_CANDLIST );
        
		{
            nPageTopIndex = lpCandList->dwPageStart;
		}

		s_CandList.dwSelection = ( GetPrimaryLang() == LANG_CHS ) ? (DWORD)-1
                                                 : s_CandList.dwSelection - nPageTopIndex;
		
		// edit by cpp2angel (050531) : abc-IME problem solution
		if( !nPageTopIndex )
			nPageTopIndex = s_CandList.dwSelection;
  
		ZeroMemory( s_CandList.szCandidate, sizeof(s_CandList.szCandidate) );

		s_CandList.nMaxLength = 0;

        for( UINT i = nPageTopIndex, j = 0;
            (DWORD)i < lpCandList->dwCount && j < s_CandList.dwPageSize;
            i++, j++ )
        {
            // Initialize the candidate list strings
            char* pwsz = s_CandList.szCandidate[j];

            // For every candidate string entry,
            // write [index] + Space + [String] if vertical,
            // write [index] + [String] + Space if horizontal.
//	        *pwsz++ = ( '0' + ( (j + 1) % 10 ) );  // Index displayed is 1 based
//			*pwsz++ = ( 176 );  // wooss 060222 delete Index display
	       // if( s_CandList.bVerticalCand )
		        *pwsz++ = ' ';
            char *szNewCand = (char*)( (LPBYTE)lpCandList + lpCandList->dwOffset[i] );

			while ( *szNewCand )
		        *pwsz++ = *szNewCand++;

	        if( !s_CandList.bVerticalCand )
		        *pwsz++ = ' ';

	        *pwsz = 0;  // Terminate

			int	nLength = strlen( s_CandList.szCandidate[j] );
			if( s_CandList.nMaxLength < nLength )
				s_CandList.nMaxLength = nLength;
        }

		s_CandList.dwCount = lpCandList->dwCount - lpCandList->dwPageStart;
        
		if( s_CandList.dwCount > lpCandList->dwPageSize )
            s_CandList.dwCount = lpCandList->dwPageSize;

        delete lpCandList;

		ImmReleaseContext ( _hwndMain, hImc );
		return WMSG_SUCCESS;
	
	} // if( lpCandList )
	
    ImmReleaseContext ( _hwndMain, hImc );

	return WMSG_FAIL;
}

void CUIEditBox::StopComposition ()
{
	HIMC hImc = ImmGetContext( _hwndMain );
	ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
	ImmReleaseContext ( _hwndMain, hImc );
	m_bOnComposition = FALSE;

	s_bShowReadingWindow = FALSE;
	ZeroMemory ( s_szReadingString, sizeof ( s_szReadingString ) );
	s_nReadingStringLength = 0;
	ZeroMemory ( &s_CandList, sizeof ( s_CandList ) );
}

void CUIEditBox::SetFocus( BOOL bVisible )
{
	if (m_InValidEditBox) return;

	if (s_PrevFocus != NULL && s_PrevFocus != this && bVisible == TRUE)
		s_PrevFocus->SetFocus(FALSE);

	CUIWindow::SetFocus( bVisible );

	extern INDEX	g_iEnterChat;
	
	if ( bVisible )
	{
		// Date : 2005-01-18,   By Lee Ki-hwan : 키 입력시 다른 UI에서 KillFocusEditBox()를 가 실행 되고 
		// SetFocus ( FALSE )를 호출 하기 때문에 계속 영문으로 바뀌는 현상이 일어 난다.
		// 그래서 SetFocus ( TRUE )일 때만 영문 모드 전환 한다.
		CUIEditBox::s_FocusingEditBox = this;
		if( m_bIsPWEditBox && m_bMsgInput )	// 영문 모드로 전환 
		{	
			SetEngMode( _hwndMain );	// Date : 2005-01-17,   By Lee Ki-hwan
		}

		s_PrevFocus = this;
	}
	else
	{
		if (CUIEditBox::s_FocusingEditBox == this)
			CUIEditBox::s_FocusingEditBox = NULL;
		if ( g_iEnterChat )
		{
			StopComposition ();
		}
	}
	//m_bOnComposition 는 포커스가 나가던 들어오던 엔드상태에서 시작끝나야 함.
	m_bOnComposition = false;

	m_dOldPromptTime = _pTimer->GetHighPrecisionTimer().GetSeconds();	
}



//------------------------------------------------------------------------------
// CUIEditBox::Get2ByteCharCount
// Explain:  
// Date : 2005-03-09(오후 2:27:04) Lee Ki-hwan
//------------------------------------------------------------------------------
int CUIEditBox::Get2ByteCharCount ( char* szBuffer, int nPos )
{
	int n2ByteCharCount = 0;
	int nLength = 0;
	
	if( nPos == -1 ) 
		nLength = strlen( szBuffer ) ;
	else
		nLength = nPos;

	for( int i = 0; i < nLength; i++ )
	{
      //if( isascii( szBuffer[i] ) == 0 )
		if( Is2ByteChar(i) )
        {
            n2ByteCharCount++; 
			i++;
        }
    }
   
	return n2ByteCharCount;
}


//------------------------------------------------------------------------------
// CUIEditBox::SetCursorIndex
// Explain:  
// Date : 2005-03-09(오후 2:28:59) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIEditBox::SetCursorIndex( int nIndex )
{
	int lengeth = strlen ( m_pInputBuffer );
	if ( lengeth <= nIndex )
	{
		m_nCursorIndex = lengeth;
		return;
	}
	
	m_nCursorIndex = nIndex;
}

//------------------------------------------------------------------------------
// CUIEditBox::IsFocused
// Explain:  
// Date : [11/23/2009 rumist]
//------------------------------------------------------------------------------
void CUIEditBox::SetOnlyIntegerMode(BOOL mode)
{
	m_bOnlyInteger = mode;

}
BOOL CUIEditBox::GetOnlyIntegerMode()
{
	return m_bOnlyInteger;
}
void CUIEditBox::SetTextColor(int color)
{
	m_nTextColor = color;

}
int  CUIEditBox::GetTextColor()
{
	return m_nTextColor;
}

void CUIEditBox::setBackGround( UIRect rect, UIRectUV uv )
{
	if( m_pBackground == NULL )
		m_pBackground = new CUIRectSurface;

	m_rcBackGround = rect;
	m_uvBackGround = uv;
	
	UISUPPORT()->DivideTexUV(m_pTexData, uv);

	m_pBackground->AddRectSurface(rect, uv);
}

CUIBase* CUIEditBox::Clone()
{
	CUIEditBox* pEdit = new CUIEditBox(*this);

	pEdit->initialize();

	CUIRectSurface* pRS = NULL;
	pRS = GetRectSurface();

	if( pRS != NULL )
	{
		{
			CUIRectSurface* pCopy = new CUIRectSurface;
			pCopy->CopyRectSurface(pRS);

			pEdit->SetRectSurface(pCopy);
		}		
	}

	CUIBase::CloneChild(pEdit);

	return pEdit;
}

WMSG_RESULT CUIEditBox::OnCharMessage( MSG* pMsg )
{
	WMSG_RESULT wmsg_ret = CharMessage(pMsg);

	if (wmsg_ret == WMSG_SUCCESS)
	{
		if (m_pInputCallback)
			m_pInputCallback->execute();
	}

	return wmsg_ret;
}

WMSG_RESULT CUIEditBox::OnIMEMessage( MSG* pMsg )
{
	WMSG_RESULT wmsg_ret = IMEMessage(pMsg);

	if (wmsg_ret == WMSG_COMMAND)
	{
		if (m_pInputCallback)
			m_pInputCallback->execute();
	}

	return wmsg_ret;
}

WMSG_RESULT CUIEditBox::OnLButtonDown( UINT16 x, UINT16 y )
{
	ResetSelArea();

	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	SetFocus( TRUE );

	// [2011/11/02 : Sora] 커서 이동 불가플래그
	if( m_nCurCharCount > 0 && m_bMoveCursor )
	{
		int	nPos = 0;
#ifdef G_RUSSIA
		nPos = UTIL_HELP()->CheckNoFixedLength(_pfdDefaultFont, &m_pInputBuffer[m_nFirstShowChar], x - GetAbsPosX());
#else
		int	nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
		int	nAbsX = GetAbsPosX() - nFontWidth / 2;
		nPos = ( x - nAbsX ) / nFontWidth;
#endif // G_RUSSIA
		if( nPos >= 0 )
		{
			if( nPos > m_nShowCharCount )
				nPos = m_nShowCharCount;
			if( nPos + m_nFirstShowChar > m_nCurCharCount )
				nPos = m_nCurCharCount - m_nFirstShowChar;

			m_nCursorIndex = nPos + m_nFirstShowChar;
			if( Is2ByteChar( m_nCursorIndex, m_nFirstShowChar ) )
				m_nCursorIndex++;
		}
	}

	return WMSG_SUCCESS;
}

WMSG_RESULT CUIEditBox::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (IsFocused() == FALSE)
		ResetSelArea();

	return WMSG_FAIL;
}

WMSG_RESULT CUIEditBox::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bCopyAndPaste == false || IsFocused() == FALSE)
		return WMSG_FAIL;

	if (!(GetKeyState(VK_LBUTTON) & 0x8000) || m_bHide)
		return WMSG_FAIL;

	// [2011/11/02 : Sora] 커서 이동 불가플래그
	if( m_nCurCharCount > 0 && m_bMoveCursor )
	{
		if ( IsInside(x, y) == FALSE )
		{
			int nMin, nMax;
			nMin = GetAbsPosX() + m_nCursorSX;
			nMax = nMin + m_nWidth;

			if (nMin > x && m_nCursorIndex > 0)
				MoveCursor(MD_LEFT);
			else if (nMax < x && m_nCursorIndex < m_nMaxCharCount)
				MoveCursor(MD_RIGHT);
		}

		int	nPos = 0;
#ifdef G_RUSSIA
		nPos = UTIL_HELP()->CheckNoFixedLength(_pfdDefaultFont, &m_pInputBuffer[m_nFirstShowChar], x - GetAbsPosX());
#else
		int	nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
		int	nAbsX = GetAbsPosX() - nFontWidth / 2;
		nPos = ( x - nAbsX ) / nFontWidth;
#endif // G_RUSSIA

		if( nPos >= 0 )
		{
			if( nPos > m_nShowCharCount )
				nPos = m_nShowCharCount;
			if( nPos + m_nFirstShowChar > m_nCurCharCount )
				nPos = m_nCurCharCount - m_nFirstShowChar;

			m_nCursorIndex = nPos + m_nFirstShowChar;
			if( Is2ByteChar( m_nCursorIndex, m_nFirstShowChar ) )
				m_nCursorIndex++;

			if (m_bCopyAndPaste == true && m_nCopyStartPos < 0)
				m_nCopyStartPos = m_nCursorIndex;
		}

		SetCopyPos();
	}

	return WMSG_SUCCESS;


}

WMSG_RESULT CUIEditBox::OnKeyMessage( MSG* pMsg )
{
	return KeyMessage(pMsg);
}

WMSG_RESULT CUIEditBox::OnCloseProc()
{
	if (s_PrevFocus != NULL && s_PrevFocus == this)
		s_PrevFocus->SetFocus(FALSE);

	// 다른 컨트롤에도 처리되도록 FAIL 리턴
	return WMSG_FAIL;
}

bool CUIEditBox::Paste( HWND hWnd )
{
	if (m_bCopyAndPaste == false || IsFocused() == FALSE)
		return false;

	if (::OpenClipboard(hWnd))
	{
		HGLOBAL hGlobal = ::GetClipboardData(CF_TEXT);
		char szData[1024];

		if (hGlobal)
		{
			DelChars();

			PTSTR pGlobal = (char*)::GlobalLock(hGlobal);
			lstrcpy(szData, pGlobal);

			::GlobalUnlock(hGlobal);

			if (strlen(szData) <= m_nMaxCharCount)
				InsertChars(m_nCursorIndex, szData);
		}

		::CloseClipboard();
	
		return true;
	}

	return false;
}

bool CUIEditBox::Copy( HWND hWnd )
{
	if (m_bCopyAndPaste == false || IsFocused() == FALSE)
		return false;

	if (m_nCopyStartPos < 0)
		return false;

	if (::OpenClipboard(hWnd))
	{
		int nStart = m_nCopyStartPos;
		int nSize = m_nCursorIndex - m_nCopyStartPos;

		if (nStart > m_nCursorIndex)
		{
			nStart = m_nCursorIndex;
			nSize = m_nCopyStartPos - m_nCursorIndex;
		}
		
		std::string strTemp = m_pInputBuffer;
		strTemp = strTemp.substr(nStart, nSize);

		if (strTemp.empty() == false)
		{
			HGLOBAL hGlobal = ::GlobalAlloc(GHND | GMEM_SHARE, nSize + 1);
	
			PTSTR pGlobal = (char*)::GlobalLock(hGlobal);
			lstrcpy(pGlobal, strTemp.c_str());
			GlobalUnlock(hGlobal);
	
			::EmptyClipboard();
			::SetClipboardData(CF_TEXT, hGlobal);
			::CloseClipboard();
	
			return true;
		}
	}

	return false;
}

void CUIEditBox::SetCopyPos()
{
	if (m_bCopyAndPaste == false || m_pImgSelText == NULL || IsFocused() == FALSE)
		return;

	if ((GetKeyState(VK_SHIFT) & 0x8000) || GetKeyState(VK_LBUTTON) & 0x8000)
	{
		m_pImgSelText->Hide(FALSE);
		
		int	nStartPos = m_nCursorSX + ( m_nCopyStartPos - m_nFirstShowChar ) *
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

		int	nEndPos = m_nCursorSX + ( m_nCursorIndex - m_nFirstShowChar ) *
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

		if (nStartPos < m_nCursorSX)
			nStartPos = m_nCursorSX;
		else if (nStartPos > m_nWidth)
			nStartPos = m_nWidth;

		if (nEndPos - nStartPos > m_nWidth)
			nEndPos = m_nWidth - m_nCursorSX;
		else if (nEndPos - nStartPos < -m_nWidth)
			nEndPos = -(m_nWidth - nEndPos);
		else
			nEndPos -= nStartPos;

		m_pImgSelText->SetPos(nStartPos, m_nTextSY);
		m_pImgSelText->SetSize(nEndPos, 12);
	}
	else
	{
		m_nCopyStartPos = -1;
		m_pImgSelText->Hide(TRUE);
	}
}

void CUIEditBox::DelChars()
{
	if (m_bCopyAndPaste == false || IsFocused() == FALSE)
		return;

	int nCount = 0;
	int nStart = 0;

	if (m_nCursorIndex < m_nCopyStartPos)
	{
		nCount = m_nCopyStartPos - m_nCursorIndex;
		nStart = m_nCursorIndex;
	}
	else
	{
		nCount = m_nCursorIndex - m_nCopyStartPos;
		nStart = m_nCopyStartPos;
	}

	if (nStart >= 0 && nCount > 0)
		DeleteChars(nStart, nCount);
}

void CUIEditBox::ResetSelArea()
{
	if (m_pImgSelText == NULL)
		return;

	m_nCopyStartPos = -1;
	m_pImgSelText->Hide(TRUE);
}

void CUIEditBox::SetCopyStartPos()
{
	if (m_nCopyStartPos < 0 && (GetKeyState(VK_SHIFT) & 0x8000))
	{
		if (m_bOnComposition)
			m_nCopyStartPos = m_nCursorIndex + 2;
		else
			m_nCopyStartPos = m_nCursorIndex;
	}
}
