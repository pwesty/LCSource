#include "stdh.h"
#include <Engine/Interface/UICheckButton.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UITextureManager.h>

// ----------------------------------------------------------------------------
// Name : CUICheckButton()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUICheckButton::CUICheckButton()
{
	m_bChecked = FALSE;
	m_strText = CTString( "" );
	m_bValidText = FALSE;
	m_colText[0] = 0xAAAAAAFF;
	m_colText[1] = 0xF2F2F2FF;

	m_nTextSY = 0;
	m_nTextArea = 0;
	m_eAlignText_h	= eALIGN_H_LEFT;
	m_bTextEdge	= false;
	m_colHighlight = DEF_UI_COLOR_WHITE;
	m_bHighlightOn = false;
	m_ulOldTime = 0;
	m_fHighlightTime = 0;
#ifdef UI_TOOL
	m_bLeft			= TRUE;
	m_nCheckRegion	= 0;
	m_nTempTextSX	= 0;
	m_stringIndex	= -1;
#endif //UI_TOOL
	setType(eUI_CONTROL_CHECKBOX);
}

CUIBase* CUICheckButton::Clone()
{
	CUICheckButton* pCheckBtn = new CUICheckButton(*this);

	if (pCheckBtn == NULL)
		return NULL;

	pCheckBtn->setTexString(getTexString());

	CUIBase::CloneChild(pCheckBtn);

	return (CUIBase*)pCheckBtn;
}
// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUICheckButton::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
								CTString &strText, BOOL bCheckRegionLeft, int nTextSX, int nCheckRegion )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	int	nLength = strText.Length();
	m_bValidText = nLength > 0;
	m_nTextSY = ( nHeight - _pUIFontTexMgr->GetFontHeight() ) / 2;

	if( m_bValidText )
		m_strText = strText;

	if( bCheckRegionLeft )
	{
		m_nTextSX = -nTextSX;
		m_rcCheckRegion.SetRect( -nCheckRegion, 0, nWidth, nHeight );
	}
	else
	{
		m_nTextSX = nTextSX;
		m_rcCheckRegion.SetRect( 0, 0, nWidth + nCheckRegion, nHeight );
	}
}

// ----------------------------------------------------------------------------
// Name : SetText()
// Desc :
// ----------------------------------------------------------------------------
void CUICheckButton::SetText( CTString &strText )
{
	int	nLength = strText.Length();
	m_bValidText = nLength > 0;

	if( m_bValidText )
		m_strText = strText;

	if( m_nTextSY == 0 )
		m_nTextSY = ( m_nHeight - _pUIFontTexMgr->GetFontHeight() ) / 2;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUICheckButton::Render()
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	// Add render regions
	int	nUVIndex;
	if( m_bChecked )
		nUVIndex = UCBS_CHECK;
	else
		nUVIndex = UCBS_NONE;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	if(m_bValidText)
	{
		if( IsEnabled() )
		{
			pDrawPort->PutTextEx( m_strText, nX + m_nTextSX, nY + m_nTextSY, m_colText[1] );
		}
		else
		{
			pDrawPort->PutTextEx( m_strText, nX + m_nTextSX, nY + m_nTextSY, m_colText[0] );
			nUVIndex += 2;
		}
	}

	pDrawPort->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight,
										m_rtUV[nUVIndex].U0, m_rtUV[nUVIndex].V0,
										m_rtUV[nUVIndex].U1, m_rtUV[nUVIndex].V1,
										0xFFFFFFFF );
}

void CUICheckButton::OnRender( CDrawPort* pDraw )
{
	if( m_pTexData == NULL )
	{
#ifdef UI_TOOL
		RenderBorder(pDraw);
#endif // UI_TOOL
		return;
	}

	// Add render regions
	int	nUVIndex;
	if( m_bChecked )
		nUVIndex = UCBS_CHECK;
	else
		nUVIndex = UCBS_NONE;

	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	pDraw->InitTextureData( m_pTexData );

	pDraw->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight,
		m_rtUV[nUVIndex].U0, m_rtUV[nUVIndex].V0,
		m_rtUV[nUVIndex].U1, m_rtUV[nUVIndex].V1,
		0xFFFFFFFF );

	pDraw->FlushRenderingQueue();

	if (m_bHighlightOn == true)
	{
		pDraw->InitTextureData(m_pTexData, FALSE, PBT_ADD);
		
		pDraw->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight,
			m_rtUV[nUVIndex].U0, m_rtUV[nUVIndex].V0,
			m_rtUV[nUVIndex].U1, m_rtUV[nUVIndex].V1,
			m_colHighlight );

		pDraw->FlushRenderingQueue();
	}

	int		outX = 0;

	if(m_bValidText)
	{
		if (m_eAlignText_h == eALIGN_H_CENTER)
		{
			int str_width = pDraw->GetTextWidth2(m_strText);
			outX += (m_nTextArea - str_width) / 2;
		}
		else if (m_eAlignText_h == eALIGN_H_RIGHT)
		{
			int str_width = pDraw->GetTextWidth2(m_strText);
			outX = m_nTextArea - str_width;
		}	

		if (m_bTextEdge == true)
		{
			pDraw->PutTextEx( m_strText, nX + m_nTextSX + outX-1, nY + m_nTextSY-1, DEF_UI_FONT_SHADOW_COLOR );
			pDraw->PutTextEx( m_strText, nX + m_nTextSX + outX+1, nY + m_nTextSY-1, DEF_UI_FONT_SHADOW_COLOR );
			pDraw->PutTextEx( m_strText, nX + m_nTextSX + outX+1, nY + m_nTextSY+1, DEF_UI_FONT_SHADOW_COLOR );
			pDraw->PutTextEx( m_strText, nX + m_nTextSX + outX-1, nY + m_nTextSY+1, DEF_UI_FONT_SHADOW_COLOR );
		}

		pDraw->PutTextEx( m_strText, nX + m_nTextSX + outX, nY + m_nTextSY, m_colText[nUVIndex] );

		pDraw->EndTextEx();
	}	
#ifdef UI_TOOL
	RenderBorder(pDraw);

	// Text Area
	if (m_bSelect)
		pDraw->DrawBorder( (PIX)(nX + m_nTextSX - 1), (PIX)(nY - 1), (PIX)m_nTextArea+1, (PIX)m_nHeight+1, 0xFFB90FFF );
#endif // UI_TOOL
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUICheckButton::MouseMessage( MSG *pMsg )
{
	// If check button is disabled
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	if( pMsg->message == WM_LBUTTONDOWN )
	{
		if( IsInsideRect( nX, nY, m_rcCheckRegion ) )
		{
			m_bChecked = !m_bChecked;
			return WMSG_SUCCESS;
		}
	}

	return WMSG_FAIL;
}

void CUICheckButton::SetCheckRegion( BOOL bLeft, int nTextSX /*= 0*/, int nCheckRegion /*= 0 */ )
{
	m_bLeft = bLeft;
	m_nCheckRegion = nCheckRegion;
	m_nTempTextSX = nTextSX;

	if( bLeft )
	{
		m_nTextSX = -nTextSX;
		m_rcCheckRegion.SetRect( -nCheckRegion, 0, m_nWidth, m_nHeight );
	}
	else
	{
		m_nTextSX = nTextSX;
		m_rcCheckRegion.SetRect( 0, 0, m_nWidth + nCheckRegion, m_nHeight );
	}
}

#ifdef UI_TOOL
UIRectUV CUICheckButton::GetUV( UICheckBtnState btnState )
{
	UIRectUV uv = m_rtUV[btnState];

	std::string strImg = getTexString();
	if(m_pTexData)
	{
		float fTexW, fTexH;
		fTexW = m_pTexData->GetPixWidth();
		fTexH = m_pTexData->GetPixHeight();

		uv.U0 = m_rtUV[btnState].U0 * fTexW;
		uv.V0 = m_rtUV[btnState].V0 * fTexH;
		uv.U1 = m_rtUV[btnState].U1 * fTexW;
		uv.V1 = m_rtUV[btnState].V1 * fTexH;
	}

	return uv; 
}

#endif //UI_TOOL

WMSG_RESULT CUICheckButton::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	m_bChecked = !m_bChecked;

	if (m_pCmd)
		m_pCmd->execute();

	return WMSG_SUCCESS;
}

void CUICheckButton::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (m_fHighlightTime > 0)
	{
		if (ElapsedTime > (m_ulOldTime + m_fHighlightTime))
		{
			m_ulOldTime = ElapsedTime;
			m_bHighlightOn = !m_bHighlightOn;
		}
	}
}
