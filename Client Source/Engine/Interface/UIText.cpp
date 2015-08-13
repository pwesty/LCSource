
#include "StdH.h"
#include "UIText.h"
#include "UIDefine.h"
#include "UIManager.h"
#include <Engine/Help/UISupport.h>
#include <Engine/Help/Util_Help.h>

CUIText::CUIText()
	: m_color(DEF_UI_FONT_COLOR)
	, m_bShadow(FALSE)
	, m_fZ(0.f)
	, m_colShadow(DEF_UI_FONT_SHADOW_COLOR)
	, m_bEdge(FALSE)
	, m_eAlignText_h(eALIGN_H_LEFT)
	, m_pBackground(NULL)
#ifdef UI_TOOL
	, m_stringIndex(-1)
#endif // UI_TOOL
	, m_strView("")
	, m_strEllipsis("")
{
	setType(eUI_CONTROL_TEXT);
}

CUIText::~CUIText()
{
	Destroy();

	SAFE_DELETE(m_pBackground);
}

CUIBase* CUIText::Clone()
{
	CUIText* pText = new CUIText(*this);
	pText->SetText(m_strOrigin, m_strEllipsis);
	pText->setBackGround(NULL);
	pText->setTexString(getTexString());

	// 현재 배경은 한장만
	if (m_pBackground != NULL && m_pBackground->GetCount() > 0)
	{
		pText->setBackGround(m_rcBackGround, m_uvBackGround);
	}	
	CUIBase::CloneChild(pText);

	return pText;
}

void CUIText::OnRender( CDrawPort* pDraw )
{
	if( m_bHide == TRUE )
		return;

	int		nX, nY;
	GetAbsPos( nX, nY );

	if (m_pTexData != NULL)
	{
		if (m_pBackground != NULL)
		{
			pDraw->InitTextureData( m_pTexData );		

			m_pBackground->SetPos(nX, nY);
			m_pBackground->RenderRectSurface( pDraw, DEF_UI_COLOR_WHITE );

			pDraw->FlushRenderingQueue();
		}
	}

	int		outX = nX;

	if (m_eAlignText_h == eALIGN_H_CENTER)
	{
		int str_width = pDraw->GetTextWidth2(m_strView);
		outX += (m_nWidth - str_width) / 2;
	}
	else if (m_eAlignText_h == eALIGN_H_RIGHT)
	{
		int str_width = pDraw->GetTextWidth2(m_strView);
		outX = outX + m_nWidth - str_width;
	}

	if (m_vecData.size() <= 0)
	{
		if (m_bShadow == FALSE && m_bEdge == TRUE)
		{
			pDraw->PutTextEx( m_strView, outX-1, nY-1, m_colShadow, m_fZ );
			pDraw->PutTextEx( m_strView, outX+1, nY-1, m_colShadow, m_fZ );
			pDraw->PutTextEx( m_strView, outX+1, nY+1, m_colShadow, m_fZ );
			pDraw->PutTextEx( m_strView, outX-1, nY+1, m_colShadow, m_fZ );
		}
			
		pDraw->PutTextEx( m_strView, outX, nY, m_color, m_fZ, m_bShadow, m_colShadow );
	}
	else
	{
		CTString str;
		COLOR col;

		for (int i = 0; i < m_vecData.size(); ++i)
		{
			str = m_vecData[i].str.c_str();
			col = m_vecData[i].col;

			if (i > 0)
				outX += pDraw->GetTextWidth2(m_vecData[i - 1].str.c_str());

			if (m_bShadow == FALSE && m_bEdge == TRUE)
			{
				pDraw->PutTextEx( str, outX-1, nY-1, m_colShadow, m_fZ );
				pDraw->PutTextEx( str, outX+1, nY-1, m_colShadow, m_fZ );
				pDraw->PutTextEx( str, outX+1, nY+1, m_colShadow, m_fZ );
				pDraw->PutTextEx( str, outX-1, nY+1, m_colShadow, m_fZ );
			}

			pDraw->PutTextEx( str, outX, nY, col, m_fZ, m_bShadow, m_colShadow );
		}
	}

	pDraw->EndTextEx();
#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

void CUIText::setBackGround( UIRect rect, UIRectUV uv )
{
	SAFE_DELETE(m_pBackground);
	m_pBackground = new CUIRectSurface;

	m_rcBackGround = rect;
	m_uvBackGround = uv;

	UISUPPORT()->DivideTexUV(m_pTexData, uv);

	m_pBackground->AddRectSurface(rect, uv);
}

WMSG_RESULT CUIText::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pCmdUp)
	{
		m_pCmdUp->execute();
		return WMSG_SUCCESS; // Text의 커맨드가 실행되는 경우 메시지 중단
	}

	return WMSG_FAIL;
}

WMSG_RESULT CUIText::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pCmd)
	{
		m_pCmd->execute();
		return WMSG_SUCCESS; // Text의 커맨드가 실행되는 경우 캐릭터 이동 막기
	}

	return WMSG_FAIL;
}

void CUIText::SetText( CTString& str, CTString strEllipsis /*= ""*/ )
{ 
	std::string strReplace = str.str_String;
	int nFind = strReplace.find("\r\n");
	
	while (nFind != std::string::npos)
	{
		strReplace.replace(nFind, 2, "");
		nFind = strReplace.find("\r\n");
	}
	
	nFind = strReplace.find("\n");
	
	while (nFind != std::string::npos)
	{
		strReplace.replace(nFind, 1, "");
		nFind = strReplace.find("\n");
	}
	
	nFind = strReplace.find("\t");
	
	while (nFind != std::string::npos)
	{
		strReplace.replace(nFind, 1, "");
		nFind = strReplace.find("\t");
	}

	m_strOrigin = strReplace.c_str();
	m_strEllipsis = strEllipsis;

	CTString strTemp = m_strOrigin;

	if (strEllipsis.IsEmpty() == false)
		strTemp = UtilHelp::getSingleton()->GetCalcStringEllipsis(strTemp, GetWidth(), strEllipsis);

	m_strView = strTemp;

	m_vecData.clear();
}

void CUIText::SetPointColor( char* str, COLOR col )
{
	if (m_strView.IsEmpty() == TRUE)
		return;

	m_vecData.clear();

	std::string strText = m_strView;
	std::string strTemp;

	int pos = strText.find(str);

	stCont _cont[3];
	int nPos = 0;

	if (pos != std::string::npos)
	{
		if (pos > 0)
		{
			strTemp = strText.substr(0, pos);
			if (strTemp.empty() == false)
			{
				_cont[nPos].str = strTemp;
				_cont[nPos].col = m_color;
				m_vecData.push_back(_cont[nPos++]);
			}
		}

		strTemp = strText.substr(pos, strlen(str));
		if (strTemp.empty() == false)
		{
			_cont[nPos].str = strTemp;
			_cont[nPos].col = col;
			m_vecData.push_back(_cont[nPos++]);
		}

		strTemp = strText.substr(strlen(str));
		if (strTemp.empty() == false)
		{
			_cont[nPos].str = strTemp;
			_cont[nPos].col = m_color;
			m_vecData.push_back(_cont[nPos]);
		}
	}
}
