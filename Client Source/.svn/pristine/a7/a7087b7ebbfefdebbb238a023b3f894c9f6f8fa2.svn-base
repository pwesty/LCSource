#include "StdH.h"
#include "UIMultiText.h"
#include "UIManager.h"
#include "UITextureManager.h"

CUIMultiText::CUIMultiText(void)
	: m_nCurStringCount(0),
	m_nMaxRow(0)
{
}


CUIMultiText::~CUIMultiText(void)
{

}

void CUIMultiText::SetRenderRect()
{
	int x, y;
	GetPos(x, y);
	m_nRenderPosX = x;
	m_nRenderPosY = y;
	m_nRenderWidth = GetWidth();
	m_nMaxMsgStringChar = ( m_nRenderWidth - 14 ) / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
}

const int CUIMultiText::GetRenderHeight()	const		
{ 
	return (/*_pUIFontTexMgr->GetFontHeight()+*/_pUIFontTexMgr->GetLineHeight()) * m_nCurStringCount;	
}

void CUIMultiText::AddString(CTString &strMessage, COLOR colMessage /* = 0xF2F2F2FF */, TEXT_ALIGN eAlign /* = TEXT_LEFT  */)
{
	if( m_nCurStringCount >= MAX_MSGSTRING )
		return;

	// Get length of string
	INDEX	nLength = strMessage.Length();
	INDEX	limitstrpos = m_nMaxMsgStringChar;
	if( nLength == 0 )
		return;
#if defined(G_THAI)
	nLength = FindThaiLen(strMessage);
	limitstrpos = (m_nMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
#else
	nLength = strMessage.Length();
	limitstrpos = m_nMaxMsgStringChar;
#endif

	int iPos;
	// If length of string is less than max char
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
			m_msgList[m_nCurStringCount].strMessage = strMessage;
			m_msgList[m_nCurStringCount].colMessage = colMessage;
			m_msgList[m_nCurStringCount].taAlign = eAlign;
			m_msgList[m_nCurStringCount++].nStrPosY = m_nMaxRow * _pUIFontTexMgr->GetLineHeight();
			m_nMaxRow++;
		}
		else
		{
			// Split string
			CTString	strTemp;
			strMessage.Split( iPos, m_msgList[m_nCurStringCount].strMessage, strTemp );
			m_msgList[m_nCurStringCount].colMessage = colMessage;
			m_msgList[m_nCurStringCount].taAlign = eAlign;
			m_msgList[m_nCurStringCount++].nStrPosY = m_nMaxRow * _pUIFontTexMgr->GetLineHeight();
			m_nMaxRow++;

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddString( strTemp, colMessage, eAlign );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = limitstrpos;
#if defined(G_THAI)
		BOOL	b2ByteChar = FALSE;
		for( iPos = 0; iPos < strMessage.Length(); iPos++ )
		{
			if(nSplitPos < FindThaiLen(strMessage,0,iPos))
				break;
		}
		nSplitPos = iPos;
#else
		BOOL	b2ByteChar = FALSE;
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
			CTString	strTemp;
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
			strMessage.Split( iPos, m_msgList[m_nCurStringCount].strMessage, strTemp );
			m_msgList[m_nCurStringCount].colMessage = colMessage;
			m_msgList[m_nCurStringCount].taAlign = eAlign;
			m_msgList[m_nCurStringCount++].nStrPosY = m_nMaxRow * _pUIFontTexMgr->GetLineHeight();
			m_nMaxRow++;

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

			AddString( strTemp, colMessage, eAlign );
		}
		else
		{
			// Split string
			CTString	strTemp;
			strMessage.Split( iPos, m_msgList[m_nCurStringCount].strMessage, strTemp );
			m_msgList[m_nCurStringCount].colMessage = colMessage;
			m_msgList[m_nCurStringCount].taAlign = eAlign;
			m_msgList[m_nCurStringCount++].nStrPosY = m_nMaxRow * _pUIFontTexMgr->GetLineHeight();
			m_nMaxRow++;

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddString( strTemp, colMessage, eAlign );
		}
	}

}

void CUIMultiText::Clear()
{
	for ( int i = 0; i< m_nCurStringCount; ++i )
	{
		m_msgList[i].strMessage.Clear();
		m_msgList[i].colMessage = 0xF2F2F2FF;
		m_msgList[i].nStrPosX = MSGBOX_MESSAGE_OFFSETX;
		m_msgList[i].nStrPosY = -1;
		m_msgList[i].taAlign = TEXT_LEFT;
	}
	m_nCurStringCount = 0;
	m_nMaxRow =0;
	m_nRenderPosX = 0;
	m_nRenderPosY = 0;
	m_nRenderWidth = 0;
	m_nMaxMsgStringChar = 0;

}