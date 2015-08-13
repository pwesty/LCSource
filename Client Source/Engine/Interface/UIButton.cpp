#include "stdh.h"
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Entities/InternalClasses.h>

#define DEF_BTNON_TIME 50
// ----------------------------------------------------------------------------
// Name : CUIButton()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIButton::CUIButton()
{
	m_bUseNewType = FALSE;
	m_bLButtonDown = FALSE;
	m_bsState = UBS_IDLE;
	m_strText = CTString( "" );
	m_bValidText = FALSE;
	m_bOnlyText = FALSE;
	m_colText[UBS_IDLE] = 0xF2F2F2FF;
	m_colText[UBS_ON] = 0xFFFFFFFF;
	m_colText[UBS_CLICK] = 0xFFFFFFFF;
	m_colText[UBS_DISABLE] = 0xB3B3B3FF;
	m_bVerticality = FALSE;
	m_bEdge = false;
	m_dwCurTime = 0;
#ifdef UI_TOOL
	m_strIndex	= -1;
#endif //UI_TOOL
	setType(eUI_CONTROL_BUTTON);

	setInherit(false);
}

CUIButton::~CUIButton()
{
	Destroy();
}

CUIBase* CUIButton::Clone()
{
	CUIButton* pBtn = NULL;
	pBtn = new CUIButton(*this);

	pBtn->setTexString(getTexString());

	CUIBase::CloneChild(pBtn);

	return pBtn;
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIButton::Create( CUIWindow *pParentWnd, CTString &strText, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	m_strText = strText;
	int	nLength = strText.Length();
	m_bValidText = nLength > 0;

	if( m_bValidText && nWidth == 0 && nHeight == 0 )
	{
		int	nTextWidth = nLength * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		int	nTextHeight = _pUIFontTexMgr->GetFontHeight();
		SetSize( nTextWidth, nTextHeight );
		m_bOnlyText = TRUE;
	}

	m_bMouseOver = FALSE;
}

// ----------------------------------------------------------------------------
// Name : SetText()
// Desc :
// ----------------------------------------------------------------------------
int CUIButton::SetText( CTString &strText, BOOL bVerticality, BOOL bOnlyText )
{
	m_strText = strText;
	m_bVerticality = bVerticality;
	int	nLength = strText.Length();
	m_bValidText = nLength > 0;

	if( m_bValidText )
	{
		m_bOnlyText = bOnlyText;
		if( m_bOnlyText )
		{
			int	nTextWidth = nLength * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
			int	nTextHeight = _pUIFontTexMgr->GetFontHeight();
			SetSize( nTextWidth, nTextHeight );
		}
	}
	else
		m_bOnlyText = FALSE;

	return nLength;
}

// ----------------------------------------------------------------------------
// Name : RenderHighlight()
// Desc :
// ----------------------------------------------------------------------------
void CUIButton::RenderHighlight( const COLOR colHighlight )
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Add render regions
	if (m_bUseNewType)
	{
		m_rtSurface[m_bsState].SetPos(nX, nY);
		m_rtSurface[m_bsState].RenderRectSurface(pDrawPort, colHighlight);
	}
	else
	{
		pDrawPort->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight,
												m_rtUV[m_bsState].U0, m_rtUV[m_bsState].V0,
												m_rtUV[m_bsState].U1, m_rtUV[m_bsState].V1,
												colHighlight );
	}
	
	// Render all button elements
	pDrawPort->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIButton::Render( COLOR textureColor )
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Add render regions
	if( !m_bOnlyText )
	{
		if (m_bUseNewType)
		{
			m_rtSurface[m_bsState].SetPos(nX, nY);
			m_rtSurface[m_bsState].RenderRectSurface(pDrawPort, textureColor);
		}
		else
		{
			pDrawPort->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight,
												m_rtUV[m_bsState].U0, m_rtUV[m_bsState].V0,
												m_rtUV[m_bsState].U1, m_rtUV[m_bsState].V1,
												textureColor );
		}
	}
	int nLength = m_strText.Length();
	int PosCont=0;
	for(int i = 0; i < nLength; i++)
	{
		if(m_strText[i]&0x80)
		{
			i++;
		}
		
		PosCont++;

	}
	

	// Text
	if( m_bValidText )
	{
		int countryXPosOffset = 3;
		int countryYPosOffset = 0;	
		int countryFontGapOffset = 14;	

		extern INDEX	g_iCountry;
#if defined(G_RUSSIA )
		{
			countryXPosOffset = 5;			
			countryYPosOffset = 6;
			countryFontGapOffset = 12;			
		}
#endif
		GetAbsPos( nX, nY );
		if( m_bOnlyText )
		{
			if(m_bVerticality)
			{
				int yPos = nY - ((12 * PosCont)/2 - countryYPosOffset);
				CTString temStr;
				int FontGap = 0;
				char chatext[3];
				for(int con = 0; con < nLength; con++)
				{
					if( m_strText[con] & 0x80 )
					{
						chatext[0] = m_strText[con];
						chatext[1] = m_strText[con+1];
						chatext[2] = '\0';
						temStr.PrintF("%s",chatext);
						con++;
					}else temStr.PrintF("%c",m_strText[con]);

					pDrawPort->PutTextEx( temStr, nX-countryXPosOffset, yPos+(FontGap*12), m_colText[m_bsState] );
					FontGap++;
					
				}
			}
			else pDrawPort->PutTextEx( m_strText, nX, nY, m_colText[m_bsState] );
		}
		else
		{
			nX += m_nWidth >> 1;
			nY += ( m_nHeight - _pUIFontTexMgr->GetFontHeight() ) >> 1;
			if( m_bsState == UBS_CLICK )
			{
				++nX;	++nY;
			}

			extern INDEX g_iCountry; 
			extern BOOL g_bIsMalEng;

			if(m_bVerticality)
			{
				int yPos = nY - ((12 * PosCont)/2 - countryYPosOffset);
				CTString temStr;
				int FontGap = 0;
				char chatext[3];
				for(int con = 0; con < nLength; con++)
				{

					if((m_strText[con] & 0x80 ))
					{
						chatext[0] = m_strText[con];
						chatext[1] = m_strText[con+1];
						chatext[2] = '\0';	
						temStr.PrintF("%s",chatext);
						con++;
					}else
					{
						temStr.PrintF("%c",m_strText[con]);
					}

					pDrawPort->PutTextEx( temStr, nX-countryXPosOffset, yPos+(FontGap*countryFontGapOffset), m_colText[m_bsState] );
					FontGap++;

				}
			}
			else pDrawPort->PutTextExCX( m_strText, nX, nY, m_colText[m_bsState] );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIButton::MouseMessage( MSG *pMsg )
{
	// If button is disabled
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( m_bsState == UBS_DISABLE )
				return WMSG_FAIL;

			if( IsInside( nX, nY ) )
			{
				if( !( pMsg->wParam & MK_LBUTTON ) )
					m_bLButtonDown = FALSE;

				if( m_bsState != UBS_CLICK )
				{
					if( m_bLButtonDown )
						m_bsState = UBS_CLICK;
					else
						m_bsState = UBS_ON;
				}

				m_bMouseOver = TRUE;

				return WMSG_SUCCESS;
			}

			m_bMouseOver = FALSE;
			m_bsState = UBS_IDLE;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_bsState == UBS_DISABLE )
					return WMSG_FAIL;

				if (m_bMouseOver == TRUE)
				{
					CEntity			*penPlEntity;
					CPlayerEntity	*penPlayerEntity;

					penPlEntity = CEntity::GetPlayerEntity( 0 );
					penPlayerEntity = (CPlayerEntity *)penPlEntity;
					penPlayerEntity->PlayButtonSound();

					m_bsState = UBS_CLICK;
					m_bLButtonDown = TRUE;
					m_dwCurTime = timeGetTime();
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			m_bLButtonDown = FALSE;

			if( m_bsState == UBS_DISABLE )
				return WMSG_FAIL;

			if( IsInside( nX, nY ) )
			{
				if( m_bsState == UBS_CLICK )
				{
					m_bsState = UBS_ON;
					return WMSG_COMMAND;
				}
			}

			m_bsState = UBS_IDLE;
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_bsState == UBS_DISABLE )
					return WMSG_FAIL;

				m_bsState = UBS_ON;
				return WMSG_COMMAND;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

void CUIButton::OnRender( CDrawPort* pDraw )
{

	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	if( m_pTexData )
		pDraw->InitTextureData( m_pTexData );

	// Add render regions
	if( !m_bOnlyText )
	{
		if (m_bUseNewType)
		{
			m_rtSurface[m_bsState].SetPos(nX, nY);
			m_rtSurface[m_bsState].RenderRectSurface(pDraw, 0xFFFFFFFF);
		}
		else
		{
			pDraw->AddTexture( nX, nY, nX + m_nWidth, nY + m_nHeight,
				m_rtUV[m_bsState].U0, m_rtUV[m_bsState].V0,
				m_rtUV[m_bsState].U1, m_rtUV[m_bsState].V1,
				0xFFFFFFFF );
		}
	}
	int nLength = m_strText.Length();
	int PosCont=0;
	for(int i = 0; i < nLength; i++)
	{
		if(m_strText[i]&0x80)
		{
			i++;
		}

		PosCont++;
	}

	pDraw->FlushRenderingQueue();


	// Text
	if( m_bValidText )
	{
		int countryXPosOffset = 3;
		int countryYPosOffset = 0;	
		int countryFontGapOffset = 14;	

		extern INDEX	g_iCountry;
#if defined(G_RUSSIA )
		{
			countryXPosOffset = 5;			
			countryYPosOffset = 6;
			countryFontGapOffset = 12;			
		}
#endif
		GetAbsPos( nX, nY );
		if( m_bOnlyText )
		{
			if(m_bVerticality)
			{
				int yPos = nY - ((12 * PosCont)/2 - countryYPosOffset);
				CTString temStr;
				int FontGap = 0;
				char chatext[3];
				for(int con = 0; con < nLength; con++)
				{
					if( m_strText[con] & 0x80 )
					{
						chatext[0] = m_strText[con];
						chatext[1] = m_strText[con+1];
						chatext[2] = '\0';
						temStr.PrintF("%s",chatext);
						con++;
					}else temStr.PrintF("%c",m_strText[con]);

					pDraw->PutTextEx( temStr, nX-countryXPosOffset, yPos+(FontGap*12), m_colText[m_bsState] );
					FontGap++;

				}
			}
			else
			{
				if (m_bEdge == true)
				{
					pDraw->PutTextEx( m_strText, nX-1, nY-1, DEF_UI_FONT_SHADOW_COLOR );
					pDraw->PutTextEx( m_strText, nX+1, nY-1, DEF_UI_FONT_SHADOW_COLOR );
					pDraw->PutTextEx( m_strText, nX+1, nY+1, DEF_UI_FONT_SHADOW_COLOR );
					pDraw->PutTextEx( m_strText, nX-1, nY+1, DEF_UI_FONT_SHADOW_COLOR );
				}
				pDraw->PutTextEx( m_strText, nX, nY, m_colText[m_bsState] );
			}
		}
		else
		{
			nX += m_nWidth >> 1;
			nY += ( m_nHeight - _pUIFontTexMgr->GetFontHeight() ) >> 1;
			if( m_bsState == UBS_CLICK )
			{
				++nX;	++nY;
			}

			extern INDEX g_iCountry; 
			extern BOOL g_bIsMalEng;

			if(m_bVerticality)
			{
				int yPos = nY - ((12 * PosCont)/2 - countryYPosOffset);
				CTString temStr;
				int FontGap = 0;
				char chatext[3];
				for(int con = 0; con < nLength; con++)
				{

					if((m_strText[con] & 0x80 ))
					{
						chatext[0] = m_strText[con];
						chatext[1] = m_strText[con+1];
						chatext[2] = '\0';	
						temStr.PrintF("%s",chatext);
						con++;
					}else
					{
						temStr.PrintF("%c",m_strText[con]);
					}

					pDraw->PutTextEx( temStr, nX-countryXPosOffset, yPos+(FontGap*countryFontGapOffset), m_colText[m_bsState] );
					FontGap++;

				}
			}
			else
			{
				if (m_bEdge == true)
				{
					pDraw->PutTextExCX( m_strText, nX-1, nY-1, DEF_UI_FONT_SHADOW_COLOR );
					pDraw->PutTextExCX( m_strText, nX+1, nY-1, DEF_UI_FONT_SHADOW_COLOR );
					pDraw->PutTextExCX( m_strText, nX+1, nY+1, DEF_UI_FONT_SHADOW_COLOR );
					pDraw->PutTextExCX( m_strText, nX-1, nY+1, DEF_UI_FONT_SHADOW_COLOR );
				}
				pDraw->PutTextExCX( m_strText, nX, nY, m_colText[m_bsState] );
			}
		}
	}

	pDraw->EndTextEx();	
#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

void CUIButton::SetUV( UIBtnState bsState, UIRectUV uv )
{
	if( m_pTexData )
	{
		FLOAT fW = m_pTexData->GetPixWidth();
		FLOAT fH = m_pTexData->GetPixHeight();
		uv.U0 /= fW;	uv.V0 /= fH;
		uv.U1 /= fW;	uv.V1 /= fH;
	}

	m_rtUV[bsState] = uv;
}

void CUIButton::SetRTSurfaceEx( UIBtnState bsState, UIRect rt, UIRectUV uv )
{
	if( bsState < UBS_IDLE || bsState >= UBS_TOTAL )
	{
		// 
		MessageBox( NULL, "type이 올바르지 않습니다.", "Error", MB_OK );
		return;
	}

	if( m_pTexData )
	{
		FLOAT fW = m_pTexData->GetPixWidth();
		FLOAT fH = m_pTexData->GetPixHeight();
		uv.U0 /= fW;	uv.V0 /= fH;
		uv.U1 /= fW;	uv.V1 /= fH;
	}

	m_rtSurface[bsState].AddRectSurface(rt, uv);
}

UIRectUV CUIButton::GetUV( UIBtnState bsState )
{
	UIRectUV uv;
#ifdef UI_TOOL
	uv = m_rtUV[bsState];
	if( m_pTexData )
	{
		FLOAT fW = m_pTexData->GetPixWidth();
		FLOAT fH = m_pTexData->GetPixHeight();
		uv.U0 *= fW;	uv.V0 *= fH;
		uv.U1 *= fW;	uv.V1 *= fH;
	}
#endif // UI_TOOL
	return uv;
}

WMSG_RESULT CUIButton::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (!(m_dwWndState & UWS_ENABLE))
		return WMSG_FAIL;

	if (m_pCmd)
		m_pCmd->execute();

	m_bsState = UBS_CLICK;

	return WMSG_SUCCESS;
}

WMSG_RESULT CUIButton::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (m_bsState == UBS_CLICK)
	{
		m_bsState = UBS_IDLE;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (!(m_dwWndState & UWS_ENABLE))
		return WMSG_FAIL;

	if (m_pCmdUp)
		m_pCmdUp->execute();

	return WMSG_SUCCESS;
}

void CUIButton::OnLeave( UINT16 x, UINT16 y )
{
	m_bEnter = false;

	if (IsInside(x, y) == FALSE)
	{
		if (!(m_dwWndState & UWS_ENABLE))
			return;

		m_bsState = UBS_IDLE;
	}

	CUIBase::OnLeave(x, y);
}

void CUIButton::OnEnter( UINT16 x, UINT16 y )
{
	m_bEnter = true;

	if (IsInside(x, y) == TRUE)
	{
		if (!(m_dwWndState & UWS_ENABLE))
			return;

		if (m_bsState == UBS_IDLE)
			m_bsState = UBS_ON;
	}

	CUIBase::OnEnter(x, y);
}

void CUIButton::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	DWORD dwTime = timeGetTime();

	if (m_bsState == UBS_CLICK)
	{
		if (dwTime > m_dwCurTime + DEF_BTNON_TIME)
		{
			if (m_pCmdOn)
				m_pCmdOn->execute();

			m_dwCurTime = dwTime;
		}
	}
}
