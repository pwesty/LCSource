#include "stdh.h"

// 헤더 정리. [12/1/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <string>
#include <vector>
#include <Engine/Interface/UIFiltering.h>
#include <algorithm>
#include <Engine/Entities/InternalClasses.h>

CUIFiltering _UIFiltering;
CUIFiltering _UIFilteringCharacter;
CUIFiltering _UICharacterChatFilter;

extern INDEX g_iCountry;

#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))

// modified by D.K 
// since 2009-09-29
// comments : key change string to hexa code.
#pragma warning( disable: 4305)		// disable truncation warning.
#pragma warning( disable: 4309)		// disable truncation warning.
const char	szKey_brz[32] = {0x8D,0x8F,0x81,0x90,0x9D,0x8D,0x90,0x9D,0x81,0x81,0x9D,0x8F,0x8D,0x8D,0x81,0x90,0x81,0x8D,0x90,0x8F,
								0x8D,0x8F,0x9D,0x8F,0x9D,0x8D,0x90,0x9D,0x81,0x90,0x81,0x9D};

//------------------------------------------------------------------------------
// makeLowCase()
// Explain:  비교시 소문자로 변환 
// Date : 2006-04-03 wooss
//------------------------------------------------------------------------------
void makeLowCase(char* tmpChar)
{
	if( *tmpChar >= 65 && *tmpChar <= 90)
		*tmpChar += 32;
}

//------------------------------------------------------------------------------
// lessLength
// Explain:  문자열 비교함수 ( for std::sort )
// Date : 2005-01-28(오후 7:34:12) Lee Ki-hwan
//------------------------------------------------------------------------------
bool lessLength(const FILTER& f1, const FILTER& f2)
{
   return f1.strString < f2.strString;
}


//------------------------------------------------------------------------------
// CUIFiltering::Clear
// Explain:  
// Date : 2005-01-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIFiltering::Clear()
{
	m_vList.clear();

	memset ( m_strFileName, 0, sizeof ( m_strFileName ) );
	
	memset ( m_strRefineWord, 0, sizeof ( m_strRefineWord ) );

	strcpy ( m_strRefineWord, "!@d#$w35%^vb&s*(!@d#$w35%^vb&s*(" );
		
	m_nListCount = 0;
}


//------------------------------------------------------------------------------
// CUIFiltering::Create
// Explain:  
// Date : 2005-01-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CUIFiltering::Create( char* file_name , bool charfilter)
{
	
	FILE* fp;	
	
	char szReadBuffer[MAX_STR_LENGTH];
	char szEndCodingBuffer[MAX_STR_LENGTH];
	
	fp = fopen(file_name, "rb");
	
	if ( fp == NULL )
	{
		return FALSE;
	}
	
	// 파일 버젼 체크
	fread ( szReadBuffer, MAX_STR_LENGTH, 1, fp );
	EnDecoding ( szEndCodingBuffer, szReadBuffer );
	
	if ( strcmp ( szEndCodingBuffer, m_strRefineWord ) != 0 )
	{
		fclose(fp);
		return FALSE;	
	}

	// 파일 개수 체크 
	fread ( szReadBuffer, MAX_STR_LENGTH, 1, fp );
	m_nListCount = atoi ( szReadBuffer );
	
	int nRefindWordLen = strlen ( m_strRefineWord ) / 2;
	
	char szString[MAX_STR_LENGTH];
	char szRep[MAX_STR_LENGTH];

	for( int i = 0; i < m_nListCount; i++ ) 
	{	
		fread( szReadBuffer, MAX_STR_LENGTH, 1, fp );
		EnDecoding ( szString, szReadBuffer );
			 
		memcpy( szRep, m_strRefineWord+(rand()%nRefindWordLen), strlen( szString ) );		
			
		szRep[strlen( szString )] = NULL;
		FILTER	tempFilter( szString, szRep );
		m_vList.push_back ( tempFilter );
	}

	fclose(fp);			

	stable_sort ( m_vList.begin(), m_vList.end(), lessLength );

	m_bCharacterfilter = charfilter;
	
	//MySort_ForFilter ( m_vList, m_nListCount );			

	return TRUE;
}


//------------------------------------------------------------------------------
// CUIFiltering::Destroy
// Explain:  
// Date : 2005-01-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIFiltering::Destroy()
{

	/*for ( int i = 0; i < m_nListCount; i++ )
	{
		if ( m_vList[i].szString != NULL )
		{
			delete[] m_vList[i].szString;
			m_vList[i].szString = NULL;
		}


		if ( m_vList[i].szRep != NULL )
		{
			delete[] m_vList[i].szRep;	
			m_vList[i].szRep = NULL;
		}

	}
	*/
	Clear();
}


//------------------------------------------------------------------------------
// CUIFiltering::Filtering
// Explain:  입력된 단어를 필터링 하여 결과를 저장
// Date : 2005-01-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CUIFiltering::Filtering( char* syntax )
{
	int space_array[256];	// 띄어쓰기가 있는 위
	int space_cnt = 0;		// 띄어쓰기가 
	int i, pos;
	
	int len = strlen ( syntax );
	if ( len <= 0 )
	{
		return FALSE;		
	}

	// add new temp val 060419--------------------<<
	//char *tv_syntax = new(char[len+1]);
	//strcpy(tv_syntax,syntax);
	char chComp[256];
	memcpy(chComp, syntax, len);
	chComp[len] = '\0';
	// ------------------------------------------->>

	// space 를 모두 없애고, space 가 있던 
	for ( i = 0; i < len; i++ ) 
	{
		if ( is_space ( chComp[i] ) ) 
		{
			for ( pos = i; pos < len; pos++ ) 
			{
				chComp[pos] = chComp[pos+1];
			}

			space_array[space_cnt++] = i;
			len--;
			i--;
		}
		else {
			//[ttos_2009_8_6]: 채팅무시는 캐릭터이름으로 비교하므로 스트링 변환을 시키지 않는다
			// 영문자 소문자로 비교 060403
			if(!m_bCharacterfilter) 
				makeLowCase( &chComp[i] );
		}
	}

	// 필터링
	i = 0;
	BOOL bFount;
	int mid, left, right;
	BOOL bAllFount = FALSE;

	while ( i < len ) 
	{
		bFount = FALSE;
		left = 0;
		right =  m_nListCount - 1;	// Date : 2005-01-17,   By Lee Ki-hwan : 버그 수정

		while ( right >= left ) 
		{
			mid = ( right + left ) / 2;
			
			int Res;
			if (!m_bCharacterfilter)
			{
				Res = m_vList[mid].strString.compare ( 0, m_vList[mid].nString, chComp+i, 0, m_vList[mid].nString );
			}else
			{
				Res = m_vList[mid].strString.compare ( 0, m_vList[mid].nString, chComp+i, 0, len);
			}

			
			//strncmp ( m_vList[mid].strString, &syntax[i], m_vList[mid].nString );
					
			if ( Res < 0 ) 
			{
				left = mid + 1;	  // check_buffer이 더 크면 
			} 
			else if ( Res > 0 ) 
			{
				right = mid - 1; //  check_buffer이 더 작으면
			} 
			else // 찾았다.
			{
				if (!m_bCharacterfilter || (m_bCharacterfilter && m_vList[mid].nString == len))
				{
					memcpy ( &chComp[i], m_vList[mid].strReword.c_str(), m_vList[mid].nReword );
					bFount = TRUE;
					break;
				}
				
			}
		}


		if ( bFount ) 
		{
			i += m_vList[mid].nString;
			bAllFount = TRUE;
			break;
		} 
		else 
		{
			if ( chComp[i] < 0 || chComp[i] > 127 )  //2바이트
			{
				i += 2;
			} 
			else 
			{
				i++;
			}
		}

	} // while ( i < len ) 
/*
	// 띄어 쓰기 복구 
	for ( i = space_cnt - 1; i >= 0; i-- ) 
	{
		for ( pos = len; pos >= space_array[i]; pos-- ) 
		{
			syntax[pos+1] = syntax[pos];
		}

		syntax[space_array[i]] = ' ';
		len++;
	}
*/
	return bAllFount;
}


//------------------------------------------------------------------------------
// CUIFiltering::Filtering
// Explain:   입력된 단어를 필터링 하여 결과를 저장
// Date : 2005-01-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CUIFiltering::Filtering( char* syntax, int* szReturn )
{
	int i, pos;

	int len = strlen ( syntax );
	if ( len <= 0 )
	{
		return FALSE;		
	}

	// add new temp val 060419--------------------<<
//	char *tv_syntax = new(char[len+1]);
//	strcpy(tv_syntax,syntax);
	char chComp[256];
	memcpy(chComp, syntax, len);
	chComp[len] = '\0';
	// ------------------------------------------->>

	// space 를 모두 없애고, space 가 있던 
	for ( i = 0; i < len; i++ ) 
	{
		if ( is_space ( chComp[i] ) ) 
		{
#if !defined(G_USA)
			{
				for ( pos = i; pos < len; pos++ ) 
				{
					chComp[pos] = chComp[pos+1];
				}

				len--;
				i--;
			}
#endif
		}
		else {
			//[ttos_2009_8_6]: 채팅무시는 캐릭터이름으로 비교하므로 스트링 변환을 시키지 않는다
			// 영문자 소문자로 비교 060403 
			if(!m_bCharacterfilter) 
				makeLowCase( &chComp[i] );
		}
	}

	// 필터링
	i = 0;
	BOOL bFount;
	int mid, left, right;
	BOOL bAllFount = FALSE;
	int nCountIndex = 1;

	while ( i < len ) 
	{
		bFount = FALSE;
		left = 0;
		right =  m_nListCount-1;
	

		while ( right >= left ) 
		{
			mid = ( right + left ) / 2;

			//int Res = strncmp ( m_vList[mid].strString.c_str(), &syntax[i], m_vList[mid].nString );
			int Res = m_vList[mid].strString.compare ( 0, m_vList[mid].nString, chComp+i, 0, m_vList[mid].nString );
				
			if ( Res < 0 ) 
			{
				left = mid + 1;	  // check_buffer이 더 크면 
			} 
			else if ( Res > 0 ) 
			{
				right = mid - 1; //  check_buffer이 더 작으면
			} 
			else // 찾았다.
			{ 
				szReturn[nCountIndex++] = mid;
				
				bFount = TRUE;
				break;
			}
		}


		if ( bFount ) 
		{
			i += m_vList[mid].nString;
			bAllFount = TRUE;
		} 
		else 
		{
			if ( chComp[i] < 0 || chComp[i] > 127 )  //2바이트
			{
				i += 2;
			} 
			else 
			{
				i++;
			}
		}

	} // while ( i < len ) 
	szReturn[0] = nCountIndex-1;

	return bAllFount;
}


//------------------------------------------------------------------------------
// CUIFiltering::GetString 
// Explain:  
// Date : 2005-01-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
char* CUIFiltering::GetString ( int nIndex ) const 
{
	return (char*)m_vList[nIndex].strString.c_str();
}



//------------------------------------------------------------------------------
// GetReFineWord
// Explain:  
// Date : 2005-01-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
char* CUIFiltering::GetKey() const
{
	return (char*)szKey_brz;
}


//------------------------------------------------------------------------------
// CUIFiltering::EnDecoding 
// Explain:  
// Date : 2005-01-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIFiltering::EnDecoding( char* szResult, const char* szData )
{
	char* TempKey = GetKey(); 
	int nKeyLeng = strlen ( TempKey);
	int nDataLen = strlen ( szData );
	int nKeyIndex = 0;

	int i;
	for ( i = 0; i < nDataLen; i++ )
	{
		szResult[i] = szData[i] ^ TempKey[nKeyIndex];
		if( ++nKeyIndex > nKeyLeng ) nKeyIndex = 0;
	}

	szResult[i] = NULL;
}


//------------------------------------------------------------------------------
// CUIFiltering::MySort_ForFilter
// Explain:  
// Date : 2005-01-14,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIFiltering::MySort_ForFilter( FILTER *ft, int n )
{
//	qsort(ft, n, sizeof(ft[0]), comp);	
}


//------------------------------------------------------------------------------
// CUIFiltering::GetRefineWord
// Explain:  
// Date : 2005-01-14,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
char* CUIFiltering::GetRefineWord() const
{
	return (char*) (m_strRefineWord);
}



//------------------------------------------------------------------------------
// CUIFiltering::SetFileName
// Explain:  
// Date : 2005-01-14,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIFiltering::SetFileName( char* szData )
{
	strcpy ( m_strFileName, szData );
}


//------------------------------------------------------------------------------
// CUIFiltering::GetFileName
// Explain:  
// Date : 2005-01-14,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
char* CUIFiltering::GetFileName() const
{
	return (char*) m_strFileName;
}

//[ttos_2009_7_17]: CHARATER_CHAT_FILTER 캐릭터 채팅 필터
//******************************************************
//---------------------------------------------------
// CUIChatFilter
//--------------------------------------------------

CUIChatFilter::CUIChatFilter()
{
	m_strSelName.Clear();
	m_nSelIndex = -1;

	m_lbCharName.ResetAllStrings();
	m_ptdButtonTexture = NULL;
}

CUIChatFilter::~CUIChatFilter()
{
	Destroy();

	STOCK_RELEASE(m_ptdButtonTexture);
}

static int	_nMsgBoxLineHeight = 0;
void CUIChatFilter::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	SetPos( nX, nY );
	SetSize( nWidth, nHeight );
	// Create Texture.
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\QuestBook.tex" ) );
	m_ptdButtonTexture = CreateTexture(CTString("Data\\Interface\\CommonBtn.tex"));

	_nMsgBoxLineHeight = _pUIFontTexMgr->GetFontHeight() + 4;

		// Get Texture size.
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// Set size of UI on this texture.
	m_rtsBase.AddRectSurface(UIRect(0,0,nWidth,nHeight),UIRectUV(0,386,250,717,fTexWidth,fTexHeight));	// 위에 틀.

	// Set title.
	m_rtTitle.SetRect(20, 0, nWidth - 40, 37);

		// Get Button Texture Size.
	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();

	// close button.
	m_btnClose.Create(this,CTString(""),227,4,16,16);
	m_btnClose.SetUV(UBS_IDLE,211, 33, 227, 49,fTexWidth,fTexHeight);
	m_btnClose.SetUV(UBS_CLICK,229, 33, 245, 49,fTexWidth,fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE,UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE,UBS_DISABLE);

	m_btnAddChar.Create(this,_S(550, "추가"),35,305,70,22);
	m_btnAddChar.SetUV(UBS_IDLE,113, 0, 182, 21,fTexWidth,fTexHeight);
	m_btnAddChar.SetUV(UBS_CLICK,186, 0, 255, 21,fTexWidth,fTexHeight);
	m_btnAddChar.CopyUV(UBS_IDLE,UBS_ON);
	m_btnAddChar.CopyUV(UBS_IDLE,UBS_DISABLE);

	m_btnDelChar.Create(this,_S(551, "삭제"),145,305,70,22);
	m_btnDelChar.SetUV(UBS_IDLE,113, 0, 182, 21,fTexWidth,fTexHeight);
	m_btnDelChar.SetUV(UBS_CLICK,186, 0, 255, 21,fTexWidth,fTexHeight);
	m_btnDelChar.CopyUV(UBS_IDLE,UBS_ON);
	m_btnDelChar.CopyUV(UBS_IDLE,UBS_DISABLE);

	// List box	
	m_lbCharName.Create( this, 20, 46, 205, 246, _nMsgBoxLineHeight, 8, 8, 2, TRUE );
	m_lbCharName.CreateScroll( TRUE, 0, 0, 10, 246, 10, 10, 0, 0, 10 );
	m_lbCharName.SetOverColor( 0xF0A769FF );
	m_lbCharName.SetSelectColor( 0xF0A769FF );
	m_lbCharName.SetColumnPosX( 1, 10 );
	// Up button
	m_lbCharName.SetScrollUpUV( UBS_IDLE, 156,33,165,42, fTexWidth, fTexHeight );
	m_lbCharName.SetScrollUpUV( UBS_CLICK, 168,33,177,42,  fTexWidth, fTexHeight );
	m_lbCharName.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbCharName.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbCharName.SetScrollDownUV( UBS_IDLE, 156, 45, 165, 54, fTexWidth, fTexHeight );
	m_lbCharName.SetScrollDownUV( UBS_CLICK, 168, 45, 177, 54, fTexWidth, fTexHeight );
	m_lbCharName.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbCharName.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbCharName.SetScrollBarTopUV( 185, 32, 194, 40, fTexWidth, fTexHeight );
	m_lbCharName.SetScrollBarMiddleUV( 185, 40, 194, 62, fTexWidth, fTexHeight );
	m_lbCharName.SetScrollBarBottomUV( 185, 62, 194, 70, fTexWidth, fTexHeight );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatFilter::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIChatFilter::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIChatFilter::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set shop texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	m_rtsBase.SetPos( m_nPosX, m_nPosY );
	m_rtsBase.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData( m_ptdButtonTexture );
	
	m_btnAddChar.Render();
	m_btnDelChar.Render();
	m_btnClose.Render();

	m_lbCharName.Render();

	pDrawPort->FlushRenderingQueue();
	CTString strTemp;
	strTemp.PrintF( _S( 4458, "채팅 차단" ) );
	pDrawPort->PutTextExCX( strTemp, m_nPosX + 125, m_nPosY + 17, 0xC5C5C5FF );
	pDrawPort->EndTextEx();	
	
}

void CUIChatFilter::OpenChatFilter()
{
	ListUpdate();

	CUIManager::getSingleton()->RearrangeOrder( UI_CHAT_FILTER, TRUE );
}

void CUIChatFilter::CloseChatFilter()
{
	m_lbCharName.ResetAllStrings();
	m_strSelName.Clear();
	m_nSelIndex = -1;
	CUIManager::getSingleton()->RearrangeOrder( UI_CHAT_FILTER, FALSE );
}

void CUIChatFilter::ListUpdate()
{
	_UICharacterChatFilter.Clear();
	CTString	strFullPath = _fnmApplicationPath.FileDir();
	strFullPath += "Data\\CharacterChatFilter.dat"; 
	_UICharacterChatFilter.Create(strFullPath.str_String, true);
	m_lbCharName.ResetAllStrings();
	int count = _UICharacterChatFilter.GetListCount();
	CTString strTem;
	
	for (int i = 0; i < count; i++)
	{
		strTem.PrintF("%s",_UICharacterChatFilter.GetString(i)); 
		m_lbCharName.AddString(0, strTem);
	}
}

void CUIChatFilter::SaveFile()
{

	FILE* fp;
	CTString	strFullPath = _fnmApplicationPath.FileDir();
	strFullPath += "Data\\CharacterChatFilter.dat";

	fp = fopen ( strFullPath, "wb" );

	if ( fp == NULL )
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return;
	}
//	_UICharacterChatFilter.Clear();
	int nCount = m_lbCharName.GetCurItemCount(0);

	char szBuffer[MAX_STR_LENGTH]; 
	memset ( szBuffer, 0, sizeof ( szBuffer ) );
	
	_UICharacterChatFilter.EnDecoding ( szBuffer, _UICharacterChatFilter.GetRefineWord() );
	fwrite ( szBuffer, MAX_STR_LENGTH, 1, fp );


	wsprintf ( szBuffer, "%d", nCount );
	fwrite ( szBuffer, sizeof ( szBuffer ), 1, fp );	
	
	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
	{
		// save data...
		_UICharacterChatFilter.EnDecoding ( szBuffer, m_lbCharName.GetString(0, nIndex) );

		fwrite ( szBuffer, sizeof ( szBuffer ), 1, fp );	
	}

	fclose ( fp );

}

void CUIChatFilter::AddCharName(char* strCharName)
{
	int len = strlen ( strCharName );
	if ( len <= 0 )
	{
		return;		
	}
	CTString strTemp;
	strTemp.PrintF("%s",strCharName);
	m_lbCharName.AddString(0, strTemp);
	SaveFile();
	ListUpdate();
}

void CUIChatFilter::DelCharName()
{
	int len = strlen ( m_strSelName );
	if ( len <= 0 )
	{
		return;		
	}

	// 필터리스트에서 해제 되면 스패머 맵에서도 해제 added by sam 11/03/02
	CUIManager::getSingleton()->GetChattingUI()->SpamerLift( m_strSelName );

	m_lbCharName.RemoveString(m_nSelIndex,0);
	m_nSelIndex = -1;
	m_strSelName.Clear();
	SaveFile();
	ListUpdate();
}

WMSG_RESULT	CUIChatFilter::MouseMessage( MSG *pMsg )
{
	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
			{
				if( IsInside( nX, nY ) )
					CUIManager::getSingleton()->SetMouseCursorInsideUIs();

				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				
				// Move UI window.
				if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
				{
					nOldX = nX;	nOldY = nY;

					Move( ndX, ndY );

					return WMSG_SUCCESS;
				}
				
				else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnAddChar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnDelChar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;						
				
			}
			break;

		case WM_LBUTTONDOWN:
			{
				if( IsInside( nX, nY ) )
				{
					nOldX = nX;		nOldY = nY;
					
					if (IsInsideRect(nX, nY, m_rtTitle))
					{
						bTitleBarClick = TRUE;
					}
					else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						//Nothing
					}
					else if( m_btnAddChar.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						//Nothing
					}
					else if( m_btnDelChar.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						//Nothing
					}
					else if (m_lbCharName.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						int nSelList = m_lbCharName.GetCurSel();
						if (nSelList != -1)
						{
							m_nSelIndex = nSelList;
							m_strSelName = m_lbCharName.GetString(0,m_nSelIndex);
						}
						return WMSG_SUCCESS;
					}
	
					CUIManager::getSingleton()->RearrangeOrder( UI_CHAT_FILTER, TRUE );
					return WMSG_SUCCESS;
				}
			}	
		break;

		case WM_LBUTTONUP:
			{
				bTitleBarClick = FALSE;
				if (IsInside(nX, nY))
				{
					// Close button
					if(m_btnClose.MouseMessage( pMsg )!= WMSG_FAIL )
					{
						CloseChatFilter();
						return WMSG_SUCCESS;
					}else if (m_btnAddChar.MouseMessage( pMsg) != WMSG_FAIL )
					{
						Message( MSGCMD_CHAT_BLOCK_REQ, _S(3004, "캐릭터 차단"), _S(3007, "차단할 캐릭명을 입력하여 주십시오." ), UMBS_OKCANCEL | UMBS_INPUTBOX );

						return WMSG_SUCCESS;
					}else if (m_btnDelChar.MouseMessage( pMsg ) != WMSG_FAIL)
					{
						if (m_nSelIndex != -1)
						{
							DelCharName();
						}
						return WMSG_SUCCESS;	
					}
				}

			}
		break;
		case WM_LBUTTONDBLCLK:
			{

			}
			break;
		case WM_MOUSEWHEEL:
			{
				if (IsInside(nX, nY))
				{
					return m_lbCharName.MouseMessage(pMsg);
				}
			}
		break;
	}

	return WMSG_FAIL;
}

void CUIChatFilter::Message( int nCommandCode, CTString strTitle, CTString strMessage, DWORD dwStyle )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( nCommandCode ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( strTitle, dwStyle, UI_CHAT_FILTER, nCommandCode ); 
	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CUIChatFilter::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch ( nCommandCode )
	{

	case MSGCMD_CHAT_BLOCK_REQ:
		{
			if( bOK )
			{
				// 차단의 순서를 바꿈. [3/9/2011 rumist]
// 				if( strInput.Length() < 2 || strInput.Length() > 16 )
// 				{
// 					Message( MSGCMD_CHAT_ERROR, _S(3008, "차단 에러"), _S(3009, "차단하고자 하는 이름이 올바르지 않습니다"), UMBS_OK);		
// 					return;
// 				}
// 				if( _UICharacterChatFilter.Filtering((char *)((const char *)strInput)))
// 				{
// 					Message( MSGCMD_CHAT_ERROR, _S(3008, "차단 에러"), _S(3011, "이미 차단된 캐릭입니다."), UMBS_OK);
// 					return;
// 				}
// 				
	
				CTString strMessage;
				strMessage.PrintF(_S(3005, "%s님을 차단하시겠습니까?"), strInput);
				m_strSelName = strInput;
				Message( MSGCMD_CHAT_BLOCK, _S(3004, "캐릭터 차단"), strMessage, UMBS_YESNO );		
			}
		}
		break;

	case MSGCMD_CHAT_BLOCK:
		{
			if( bOK)
			{
				if( m_strSelName.Length() < 2 || m_strSelName.Length() > 16 )
				{
					Message( MSGCMD_CHAT_ERROR, _S(3008, "차단 에러"), _S(3009, "차단하고자 하는 이름이 올바르지 않습니다"), UMBS_OK);		
					return;
				}
				if( _UICharacterChatFilter.Filtering((char *)((const char *)m_strSelName)))
				{
					Message( MSGCMD_CHAT_ERROR, _S(3008, "차단 에러"), _S(3011, "이미 차단된 캐릭입니다."), UMBS_OK);
					return;
				}
				AddCharName(m_strSelName.str_String);	
			}else
			{
				m_strSelName.Clear();
			}
		}
		break;

	case MSGCMD_CHAT_ERROR:
		{
			
		}
		break;

	}

}

// call by simple popup window.
void	CUIChatFilter::CharacterFilterInSimplePopup( CTString strCharName )
{
	CTString strMessage;
	strMessage.PrintF(_S(3005, "%s님을 차단하시겠습니까?"), strCharName);
	m_strSelName = strCharName;
	Message( MSGCMD_CHAT_BLOCK, _S(3004, "캐릭터 차단"), strMessage, UMBS_YESNO );	
}
//***********************************************************************************