#include "StdH.h"

#include <Engine/Interface/UITextBox.h>
#include <Engine/Interface/UIDefine.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Help/UISupport.h>
#include <Engine/Help/Util_Help.h>

#define SCROLL_WIDTH 15

#if		defined(G_RUSSIA)
	extern CFontData *_pfdDefaultFont;
#endif

class CmdOnChangeTextBoxScroll : public Command
{
public:
	CmdOnChangeTextBoxScroll() : TextBox_(NULL) {}

	void setData(CUITextBox* parent)	{ TextBox_ = parent; }
	void execute()
	{
		if (TextBox_)
		{
			TextBox_->OnUpdateScrollPos();
		}
	}

private:
	CUITextBox* TextBox_;
};


CUITextBox::CUITextBox()
	: m_pBackground(NULL)
	, m_color(DEF_UI_FONT_COLOR)
	, m_pScroll(NULL)
	, m_nScrollPos(0)
	, m_eAlignText_h(eALIGN_H_LEFT)
	, m_eAlignText_v(eALIGN_V_TOP)
	, m_eSplitType(SPLIT_NONE)
	, m_bUpdateHeight(FALSE)
	, m_nBoxWidth(0)
#ifdef UI_TOOL
	, m_stringIndex(-1)
#endif // UI_TOOL
{
	m_nLineHeight = 0;
	m_nTextSX = 0;
	m_nTextSY = 0;
	m_nShowLine	= 0;
	m_nStartX = 0;
	m_nStartY = 0;
	m_nGapY   = 0;
	m_eType = eUI_CONTROL_TEXTBOX;
	m_bEllipsis = false;
	_ResetString();
	m_vecBoxString.clear();
	m_nCharMaxCount = 0;
}


CUITextBox::~CUITextBox()
{
	Destroy();

	SAFE_DELETE(m_pBackground);
}

CUIBase* CUITextBox::Clone()
{
	CUITextBox* pText = new CUITextBox(*this);

	if (m_pScroll != NULL)
	{
		pText->SetScroll( static_cast<CUIScrollBar*>( m_pScroll->Clone() ) );
	}

	CUIRectSurface* pRS = NULL;
	pRS = GetRectSurface();
	if( pRS != NULL )
	{
		{
			CUIRectSurface* pCopy = new CUIRectSurface;
			pCopy->CopyRectSurface(pRS);

			pText->SetRectSurface(pCopy);
		}		
	}

	CUIBase::CloneChild(pText);

	return pText;
}

void CUITextBox::initialize()
{

}

void CUITextBox::OnRender( CDrawPort* pDraw )
{
	if( m_bHide == TRUE )
		return;

	int		nX, nY;
	GetAbsPos( nX, nY );

	if (m_pTexData != NULL && m_pBackground != NULL)
	{
		pDraw->InitTextureData( m_pTexData );

		m_pBackground->SetPos(nX, nY);
		m_pBackground->RenderRectSurface( pDraw, DEF_UI_COLOR_WHITE );

		pDraw->FlushRenderingQueue();
	}

	if ( m_pScroll )
		m_pScroll->OnRender( pDraw );

	_RenderString(pDraw, nX, nY);

#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

void CUITextBox::OnUpdateScrollPos()
{
	if ( m_pScroll )
	{
		m_nScrollPos = m_pScroll->GetScrollPos();
	}
}

void CUITextBox::_UpdateBoxSize()
{
	int nSize = ( _pUIFontTexMgr->GetFontHeight() + _pUIFontTexMgr->GetLineSpacing() + m_nGapY  );

	m_nMaxLine = m_vecBoxString.size();
	m_nLineHeight = nSize;
	m_nShowLine = (this->GetHeight() - m_nStartY) / nSize;

	m_nTextSX = m_nStartX + 1;
	m_nTextSY = m_nStartY + 1;

	if (m_pScroll)
	{
		m_pScroll->SetCurItemCount( m_nMaxLine );
		m_pScroll->SetItemsPerPage( m_nShowLine );
	}	
}

void CUITextBox::SetText( CTString& str, COLOR color, BOOL bUpdateHeight )
{
	_ResetString();
	m_color = color;
	m_Str = str;
	m_bUpdateHeight = bUpdateHeight;

	int nFind = m_Str.find("\r\n");

	while (nFind != std::string::npos)
	{
		m_Str.replace(nFind, 2, "");
		nFind = m_Str.find("\r\n");
	}

	nFind = m_Str.find("\n");

	while (nFind != std::string::npos)
	{
		m_Str.replace(nFind, 1, "");
		nFind = m_Str.find("\n");
	}

	nFind = m_Str.find("\t");

	while (nFind != std::string::npos)
	{
		m_Str.replace(nFind, 1, "");
		nFind = m_Str.find("\t");
	}

	OnUpdateBoxInfo();
}

void CUITextBox::SetScroll( CUIScrollBar* pScroll )
{
	if (pScroll != NULL)
	{
		m_pScroll = pScroll;

		CmdOnChangeTextBoxScroll* pCmd = new CmdOnChangeTextBoxScroll;
		pCmd->setData(this);
		m_pScroll->SetCommand(pCmd);

		m_pScroll->Hide(FALSE);

		m_pScroll->SetCurItemCount( m_nMaxLine );
		m_pScroll->SetItemsPerPage( m_nShowLine );

		OnUpdateBoxInfo();
	}
}

void CUITextBox::setBackGround( UIRect rect, UIRectUV uv )
{
	if( m_pBackground == NULL )
		m_pBackground = new CUIRectSurface;

#ifdef UI_TOOL
	m_rcBackGround = rect;
	m_uvBackGround = uv;
#endif // UI_TOOL
	UISUPPORT()->DivideTexUV(m_pTexData, uv);

	m_pBackground->AddRectSurface(rect, uv);
}

void CUITextBox::_RenderString( CDrawPort* pDraw , int nX, int nY)
{
	if(m_vecBoxString.size() <= 0)
		return;

	int	nTexX = nX + m_nTextSX;
	int	nTexY = nY + m_nTextSY;
	int	nRowE = m_nScrollPos + m_nShowLine;
	if( nRowE > m_vecBoxString.size() )
		nRowE = m_vecBoxString.size();

	for( int iString = m_nScrollPos; iString < nRowE; iString++)
	{
		int outX = 0;

 		if (m_eAlignText_h == eALIGN_H_CENTER)
 		{
 			int str_width = pDraw->GetTextWidth2(m_vecBoxString[iString]);
 			outX += (m_nWidth - str_width) / 2;
 		}
 		else if (m_eAlignText_h == eALIGN_H_RIGHT)
 		{
			int str_width = pDraw->GetTextWidth2(m_vecBoxString[iString]);
 			outX = outX + m_nWidth - str_width;
 		}

		int outY = 0;
		int str_height = _pUIFontTexMgr->GetFontHeight() * GetLineCount();

		if (m_eAlignText_v == eALIGN_V_CENTER)
		{
			outY += (m_nHeight - str_height) / 2;
		}
		else if (m_eAlignText_v == eALIGN_V_BOTTOM)
		{
			outY = outY + m_nHeight - str_height;
		}

		pDraw->PutTextEx( m_vecBoxString[iString], nTexX + outX, nTexY + outY, m_color );

		nTexY += m_nLineHeight;
	}		
	

	pDraw->EndTextEx();
}

void CUITextBox::_ResetString()
{
	m_Str = CTString("");
	m_color = 0xFFFFFFFF;
	m_nBoxWidth = 0;
}

void CUITextBox::_StringSplit()
{
	m_vecBoxString.clear();

	if (m_Str.empty() == true)
		return;

	// 문자열이 출력될 수 있는 크기.
	m_nBoxWidth = GetWidth() - m_nStartX * 2;		

	// 스크롤이 연결되어 있다면 스크롤바 width만큼 빼준다.
	if (m_pScroll != NULL)
		m_nBoxWidth -= m_pScroll->getThumbWidth();	

	// 폰트 하나의 width
	int nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();	

	// 박스 크기 / 폰트 하나의 크기 = 한라인에 차지하는 문자열.
	int nTextCnt = m_nBoxWidth / nFontWidth;

	switch (m_eSplitType)
	{
	case SPLIT_NONE:
		SplitNone(nTextCnt);
		break;
	case SPLIT_SPACE:
		SplitSpace(nTextCnt);
		break;
	case SPLIT_HYPHEN:
		SplitHyphen(nTextCnt);
		break;
	case SPLIT_MIXED:
		SplitMixed( nTextCnt);
		break;
	case SPLIT_CHAR_COUNT:
		SplitCharCount();
		break;
		
	}

	if (m_bUpdateHeight == TRUE)
	{
		SetHeight( GetTextTotalHeight() );
	}
	ASSERT(m_vecBoxString.size());
}

void CUITextBox::CheckSplit( int& nCurCnt, int& nSplitCnt,	int& nEnterCnt)
{
	std::string strTmp;
	int		pos;

	int len = m_Str.size();

	bool bDBSC = false;

#if defined(G_KOR) || defined(G_THAI)
	bDBSC = true;
#endif
	
	pos = m_Str.find("\\n", nCurCnt);

	if (pos != std::string::npos && (pos - nCurCnt + 1) <= nSplitCnt)
	{
		nSplitCnt = pos - nCurCnt;
		nEnterCnt = 2;
	}

	// 한글인지 검사.
	if (bDBSC == true)
 	{
		if ((nCurCnt + nSplitCnt) < len && IsDBCSLeadByte(m_Str[nCurCnt + nSplitCnt]))
		{
			bool bLead = false;

			for (int i = nCurCnt; i <= (nCurCnt + nSplitCnt); ++i)
			{
				if (IsDBCSLeadByte(m_Str[i]))
				{
					bLead = true;
					++i;

					if (i > (nCurCnt + nSplitCnt))
					{
						bLead = false;
					}
				}
				else
				{
					bLead = false;
				}
			}

			if (bLead == true)
				--nSplitCnt;
		}
	}

	if ((nCurCnt + nSplitCnt) > len)
		nSplitCnt = len - nCurCnt;
}

void CUITextBox::SplitNone(int nTextCnt )
{
	std::string strTmp;
	int nCurText = 0, tmpCnt = 0;
	int len = m_Str.size();

	while(1)
	{
		int nEnter = 0;
		tmpCnt = nTextCnt;

#if		defined(G_RUSSIA)
		std::string strNofixed;

		strNofixed = m_Str.substr(nCurText, (len - nCurText));
		tmpCnt = UTIL_HELP()->CheckNoFixedLength(_pfdDefaultFont, (char*)strNofixed.c_str(), m_nBoxWidth);
#endif	// G_RUSSIA
		
		CheckSplit(nCurText, tmpCnt, nEnter);

		strTmp = m_Str.substr(nCurText, tmpCnt);
		
		m_vecBoxString.push_back(strTmp.c_str());
		
		nCurText += tmpCnt + nEnter;

		if (nCurText >= len)
			break;
	}
}

void CUITextBox::SplitSpace( int nTextCnt )
{
	std::string strTmp;
	int nCurText = 0, tmpCnt = 0;
	int len = m_Str.size();

	while(1)
	{
		int		lastSpace = 0;

		int nEnter = 0;
		tmpCnt = nTextCnt;

#if		defined(G_RUSSIA)
		std::string strNofixed;

		strNofixed = m_Str.substr(nCurText, (len - nCurText));
		tmpCnt = UTIL_HELP()->CheckNoFixedLength(_pfdDefaultFont, (char*)strNofixed.c_str(), m_nBoxWidth);
#endif	// G_RUSSIA

		CheckSplit(nCurText, tmpCnt, nEnter);

		strTmp = m_Str.substr(nCurText, tmpCnt);

		lastSpace = strTmp.rfind(0x20, tmpCnt);

		// 잘라낸 문자열에 스페이스가 있는 경우를 검사.
		if (lastSpace != std::string::npos && nCurText + tmpCnt < len)
		{
			// 마지막 문자가 스페이스가 아니어야 한다.
			if (lastSpace != tmpCnt - 1)
			{
				strTmp = m_Str.substr(nCurText, ++lastSpace);
				tmpCnt = lastSpace;
			}
		}

		m_vecBoxString.push_back(strTmp.c_str());

		nCurText += tmpCnt + nEnter;

		if (nCurText >= len)
			break;
	}
}

void CUITextBox::SplitHyphen( int nTextCnt )
{
	std::string strTmp;
	int nCurText = 0, tmpCnt = 0;
	int len = m_Str.size();

	while(1)
	{
		int		pos = 0;

		int nEnter = 0;
		bool bHyphen = false;
		tmpCnt = nTextCnt;

#if		defined(G_RUSSIA)
		std::string strNofixed;

		strNofixed = m_Str.substr(nCurText, (len - nCurText));
		tmpCnt = UTIL_HELP()->CheckNoFixedLength(_pfdDefaultFont, (char*)strNofixed.c_str(), m_nBoxWidth);
#endif	// G_RUSSIA

		CheckSplit(nCurText, tmpCnt, nEnter);

		if (nCurText + tmpCnt < len)
		{
			char szPrev = m_Str[nCurText + tmpCnt - 2];
			char szCur  = m_Str[nCurText + tmpCnt - 1];
			char szNext = m_Str[nCurText + tmpCnt];
	
			// 문자열의 마지막이 영어고 그 다음 문자도 영어일 경우 마지막 문자 "위치"에 하이픈을 추가하고 개행.
			if ( ((szPrev >= 0x41 && szPrev <= 0x5A) || (szPrev >= 0x61 && szPrev <= 0x7a)) &&
				 ((szCur >= 0x41 && szCur <= 0x5A) || (szCur >= 0x61 && szCur <= 0x7a)) )//&&
				 //!((szNext == 0x20 || szNext == 0x2C || szNext == 0x2E)) )
			{
				bHyphen = true;
				--tmpCnt;
			}
			// ' ', ',' '.'
			else if ( ((szPrev == 0x20 || szPrev == 0x2C || szPrev == 0x2E)) && 
					  ((szNext >= 0x41 && szNext <= 0x5A) || (szNext >= 0x61 && szNext <= 0x7a)) )
			{
				--tmpCnt;
			}
		}

		strTmp = m_Str.substr(nCurText, tmpCnt);

		if (bHyphen == true)
			strTmp += '-';

		m_vecBoxString.push_back(strTmp.c_str());

		nCurText += tmpCnt + nEnter;

		if (nCurText >= len)
			break;
	}
}

void CUITextBox::SplitMixed( int nTextCnt )
{

}

void CUITextBox::SplitCharCount( )
{
	std::string strTmp;
	int nCurText = 0, tmpCnt = 0;
	int len = m_Str.size();

	while(1)
	{
		int nEnter = 0;
		tmpCnt = m_nCharMaxCount;

		CheckSplit(nCurText, tmpCnt, nEnter);

		strTmp = m_Str.substr(nCurText, tmpCnt);

		// 자른 문자열 시작 부분에 공백이나 줄바꿈 문자가 있을경우 제거.
		int i = 0;
		for ( ; i < strTmp.size(); ++i)
		{
			if (strTmp[i]  != ' ' && strTmp[i] != '\r' && strTmp[i] != '\n')
				break;
		}

		if (i > 0 && i != tmpCnt)
		{
			nCurText += i;
			CheckSplit(nCurText, tmpCnt, nEnter);
			strTmp = m_Str.substr(nCurText, tmpCnt);
		}

		if (strTmp.empty() == false)
			m_vecBoxString.push_back(strTmp.c_str());

		nCurText += tmpCnt + nEnter;

		if (nCurText >= len)
			break;
	}
}

void CUITextBox::OnUpdateBoxInfo()
{
	_StringSplit();
	_UpdateBoxSize();
	m_nScrollPos = 0;
}

void CUITextBox::StringEllipsis( int nCnt )
{
	if (nCnt <= 0 || m_vecBoxString.size() <= nCnt)
	{
		m_bEllipsis = false;
		return;
	}

	CTString strTemp;
	strTemp = GetLineString(nCnt - 1) + GetLineString(nCnt);
	strTemp = UtilHelp::getSingleton()->GetCalcStringEllipsis(strTemp, GetWidth(), CTString(".."));

	m_Str = CTString("");
	
	for (int i = 0; i < nCnt - 1; ++i)
	{
		m_Str += GetLineString(i);
	}

	m_Str += strTemp;

	_StringSplit();
	_UpdateBoxSize();
	m_bEllipsis = true;
}
