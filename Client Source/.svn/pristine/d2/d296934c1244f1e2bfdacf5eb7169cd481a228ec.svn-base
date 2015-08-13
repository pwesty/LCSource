#include "stdh.h"
#include <Engine/Interface/UIComboBox.h>
#include <Engine/Interface/UIDefine.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Interface/UICheckButton.h>
#include <Engine/Help/UISupport.h>


class CmdOnChangeComboScroll : public Command
{
public:
	CmdOnChangeComboScroll() : combo_(NULL) {}

	void setData(CUIComboBox* parent)	{ combo_ = parent; }
	void execute()
	{
		if (combo_)
		{
			combo_->OnUpdateDropList();
		}
	}

private:
	CUIComboBox* combo_;
};



// ----------------------------------------------------------------------------
// Name : CUIComboBox()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIComboBox::CUIComboBox()
	: m_pBackground(NULL)
	, m_pDropListSurface(NULL)
	, m_prsButton(NULL)
	, m_colorDrop(DEF_UI_COMBO_LIST)
	, m_colorDropSel(DEF_UI_COMBO_LIST_SEL)
	, m_pScroll(NULL)
	, m_nScrollPos(0)
	, m_eAlignText_h(eALIGN_H_LEFT)
{
	m_bScrollBarExist = FALSE;
	m_bShowScrollBar = FALSE;
	m_bDropList = FALSE;
	m_nSelectList = 0;
	m_nOverList = -1;
	m_nLineHeight = 0;
	m_nMaxLine = 0;
	m_nComboTextSY = 0;
	m_nTextSX = 0;
	m_nTextSY = 0;
	m_nSelectOffsetY = 0;
	m_nShowLine	= 0;

	m_nSpaceX = 0;
	m_nSpaceY = 0;
	setType(eUI_CONTROL_COMBOBOX);
}

CUIComboBox::~CUIComboBox()
{
	Destroy();

	SAFE_DELETE(m_pBackground);
	SAFE_DELETE(m_pDropListSurface);
	SAFE_DELETE(m_prsButton);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIComboBox::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight,
							int nBtnSX, int nBtnSY, int nBtnWidth, int nBtnHeight,
							int nMaxLine, int nLineHeight, int nSpaceX, int nSpaceY )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_nMaxLine = nMaxLine;
	m_nLineHeight = nLineHeight;

	m_rcBtn.Left = nBtnSX;
	m_rcBtn.Top = nBtnSY;
	m_rcBtn.Right = m_rcBtn.Left + nBtnWidth;
	m_rcBtn.Bottom = m_rcBtn.Top + nBtnHeight;

	m_rcDropList.Left = 0;
	m_rcDropList.Right = nWidth;
	m_rcDropList.Top = nHeight;

	m_nComboTextSY = ( m_nHeight - _pUIFontTexMgr->GetFontHeight() ) / 2;
	m_nTextSX = m_rcDropList.Left + 1 + nSpaceX;
	m_nTextSY = m_rcDropList.Top + 1 + nSpaceY;

	int	nOffset = nLineHeight - _pUIFontTexMgr->GetFontHeight();
	if( nOffset < 0 )
		nOffset = 0;
	m_nSelectOffsetY = nOffset / 2;
}

// ----------------------------------------------------------------------------
// Name : CreateScroll()
// Desc :
// ----------------------------------------------------------------------------
void CUIComboBox::CreateScroll( BOOL bRight, int nBtnWidth, int nBtnHeight, int nBarWidthGap, int nBarHeightGap,
								int nBarVertEdgeSize )
{
	m_bScrollBarExist = TRUE;

	int	nWidth = m_rcDropList.GetWidth() - 2;
	int	nSpaceY = ( m_nTextSY - m_rcDropList.Top - 1 ) * 2 - ( m_nLineHeight - _pUIFontTexMgr->GetFontHeight() );
	int	nHeight = nSpaceY + m_nLineHeight * m_nMaxLine;

	if( bRight )
	{
		m_sbScrollBar.Create( this, m_rcDropList.Right - 1 - nBtnWidth, m_rcDropList.Top + 1, nBtnWidth, nHeight );
		m_sbScrollBar.SetWheelRect( -( nWidth - nBtnWidth ), 0, nWidth - nBtnWidth, nHeight );
	}
	else
	{
		m_sbScrollBar.Create( this, m_rcDropList.Left + 1, m_rcDropList.Top + 1, nBtnWidth, nHeight );
		m_sbScrollBar.SetWheelRect( nBtnWidth, 0, nWidth - nBtnWidth, nHeight );
	}

	m_sbScrollBar.CreateButtons( TRUE, nBtnWidth, nBtnHeight, nBarWidthGap, nBarHeightGap, nBarVertEdgeSize );
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetItemsPerPage( m_nMaxLine );
}

// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUIComboBox::AddString( CTString &strString )
{
#if		!defined(UI_TOOL)
	if( !m_bScrollBarExist && m_vecString.size() >= m_nMaxLine )
		return;
#endif	// UI_TOOL

	m_vecString.push_back( strString );
	int	nLines = m_vecString.size();

	if( m_sbScrollBar.GetCurItemCount() < nLines )
		m_sbScrollBar.SetCurItemCount( nLines );

	if( m_bScrollBarExist && nLines > m_nMaxLine )
	{
		m_bShowScrollBar = TRUE;
		return;
	}

	int	nSpaceY = ( m_nTextSY - m_rcDropList.Top - 1 ) * 2 - ( m_nLineHeight - _pUIFontTexMgr->GetFontHeight() ) + 2;
	m_rcDropList.Bottom = m_rcDropList.Top + nSpaceY + m_nLineHeight * nLines;

	m_rcDropArea.Bottom = m_rcDropArea.Top + ((m_nLineHeight + m_nSelectOffsetY) * nLines);
}

// ----------------------------------------------------------------------------
// Name : ResetStrings()
// Desc :
// ----------------------------------------------------------------------------
void CUIComboBox::ResetStrings()
{
	m_vecString.clear();
	m_sbScrollBar.SetCurItemCount( 0 );
	m_sbScrollBar.SetScrollPos( 0 );
	m_bShowScrollBar = FALSE;
	m_nSelectList = 0;
	m_nOverList = -1;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIComboBox::Render()
{
	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	if( m_pBackground != NULL )
	{
		m_pBackground->SetPos(nX, nY);
		m_pBackground->RenderRectSurface(pDrawPort, 0xFFFFFFFF);
	}

	if( m_pDropListSurface != NULL && m_bDropList )
	{
		pDrawPort->AddTexture( nX + m_rcBtn.Left, nY + m_rcBtn.Top,
											nX + m_rcBtn.Right, nY + m_rcBtn.Bottom,
											m_rtUp.U0, m_rtUp.V0, m_rtUp.U1, m_rtUp.V1,
											0xFFFFFFFF );
		
		m_pDropListSurface->SetPos(nX, nY);

		m_pDropListSurface->m_RectSurfaceArray[0].m_RT.SetRect(m_rcDropList.Left,m_rcDropList.Top,
			m_rcDropList.Left+CMB_DROPLIST_HORI_GAP,m_rcDropList.Top+CMB_DROPLIST_VERT_GAP);
		m_pDropListSurface->m_RectSurfaceArray[1].m_RT.SetRect(m_rcDropList.Left+CMB_DROPLIST_HORI_GAP,m_rcDropList.Top,
			m_rcDropList.Right-CMB_DROPLIST_HORI_GAP,m_rcDropList.Top+CMB_DROPLIST_VERT_GAP);
		m_pDropListSurface->m_RectSurfaceArray[2].m_RT.SetRect(m_rcDropList.Right-CMB_DROPLIST_HORI_GAP,m_rcDropList.Top,
			m_rcDropList.Right,m_rcDropList.Top+CMB_DROPLIST_VERT_GAP);

		m_pDropListSurface->m_RectSurfaceArray[3].m_RT.SetRect(m_rcDropList.Left,m_rcDropList.Top+CMB_DROPLIST_VERT_GAP,
			m_rcDropList.Left+CMB_DROPLIST_HORI_GAP,m_rcDropList.Bottom-CMB_DROPLIST_VERT_GAP);
		m_pDropListSurface->m_RectSurfaceArray[4].m_RT.SetRect(m_rcDropList.Left+CMB_DROPLIST_HORI_GAP,m_rcDropList.Top+CMB_DROPLIST_VERT_GAP,
			m_rcDropList.Right-CMB_DROPLIST_HORI_GAP,m_rcDropList.Bottom-CMB_DROPLIST_VERT_GAP);
		m_pDropListSurface->m_RectSurfaceArray[5].m_RT.SetRect(m_rcDropList.Right-CMB_DROPLIST_HORI_GAP,m_rcDropList.Top+CMB_DROPLIST_VERT_GAP,
			m_rcDropList.Right,m_rcDropList.Bottom-CMB_DROPLIST_VERT_GAP);

		m_pDropListSurface->m_RectSurfaceArray[6].m_RT.SetRect(m_rcDropList.Left,m_rcDropList.Bottom-CMB_DROPLIST_VERT_GAP,
			m_rcDropList.Left+CMB_DROPLIST_HORI_GAP,m_rcDropList.Bottom);
		m_pDropListSurface->m_RectSurfaceArray[7].m_RT.SetRect(m_rcDropList.Left+CMB_DROPLIST_HORI_GAP,m_rcDropList.Bottom-CMB_DROPLIST_VERT_GAP,
			m_rcDropList.Right-CMB_DROPLIST_HORI_GAP,m_rcDropList.Bottom);
		m_pDropListSurface->m_RectSurfaceArray[8].m_RT.SetRect(m_rcDropList.Right-CMB_DROPLIST_HORI_GAP,m_rcDropList.Bottom-CMB_DROPLIST_VERT_GAP,
			m_rcDropList.Right,m_rcDropList.Bottom);

		m_pDropListSurface->RenderRectSurface(pDrawPort, 0xFFFFFFFF);

		// Scroll bar
		if( m_bShowScrollBar )
			m_sbScrollBar.Render();
	}
	else
	{
		pDrawPort->AddTexture( nX + m_rcBtn.Left, nY + m_rcBtn.Top,
											nX + m_rcBtn.Right, nY + m_rcBtn.Bottom,
											m_rtDown.U0, m_rtDown.V0, m_rtDown.U1, m_rtDown.V1,
											0xFFFFFFFF );
	}

	// Text
	if( m_vecString.size() > 0 )
		pDrawPort->PutTextEx( m_vecString[m_nSelectList],
											nX + m_nTextSX, nY + m_nComboTextSY, 0xF2F2F2FF );

	if( m_bDropList )
	{
		int	nTexX = nX + m_nTextSX;
		int	nTexY = nY + m_nTextSY;
		int	nRowS = m_sbScrollBar.GetScrollPos();
		int	nRowE = nRowS + m_nMaxLine;
		if( nRowE > m_vecString.size() )
			nRowE = m_vecString.size();

		for( int iString = nRowS; iString < nRowE; iString++ )
		{
			if( m_nOverList == iString )
				pDrawPort->PutTextEx( m_vecString[iString], nTexX, nTexY, 0xFF6107FF );
			else
				pDrawPort->PutTextEx( m_vecString[iString], nTexX, nTexY, 0xC5B083FF );

			nTexY += m_nLineHeight;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIComboBox::MouseMessage( MSG *pMsg )
{
	// If combo box is disabled
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
			if( m_bDropList && IsInsideRect( nX, nY, m_rcDropList ) )
			{
				if( m_bShowScrollBar )
				{
					if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}

				int	nAbsY = GetAbsPosY();
				m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight +
								m_sbScrollBar.GetScrollPos();

				return WMSG_SUCCESS;
			}

			m_nOverList = -1;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				m_bDropList = !m_bDropList;

				return WMSG_SUCCESS;
			}
			else if( m_bDropList && IsInsideRect( nX, nY, m_rcDropList ) )
			{
				if( m_bShowScrollBar )
				{
					if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}

				m_bDropList = FALSE;

				int	nAbsY = GetAbsPosY();
				m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight +
								m_sbScrollBar.GetScrollPos();
				if( m_nOverList >= 0 && m_nOverList < m_vecString.size() )
				{
					m_nSelectList = m_nOverList;
					return WMSG_COMMAND;
				}

				return WMSG_SUCCESS;
			}

			m_bDropList = FALSE;
		}
		break;

	case WM_LBUTTONUP:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
			else if( m_bDropList && IsInsideRect( nX, nY, m_rcDropList ) )
			{
				if( m_bShowScrollBar )
				{
					if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( m_bDropList && IsInsideRect( nX, nY, m_rcDropList ) )
			{
				if( m_bShowScrollBar )
				{
					if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( m_bDropList && IsInsideRect( nX, nY, m_rcDropList ) )
			{
				if( m_bShowScrollBar )
				{
					if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}

void CUIComboBox::SetDropList( BOOL bDrop )
{
	m_bDropList = bDrop;

	if (m_pScroll && m_bDropList == FALSE)
		m_pScroll->Hide(TRUE);
}

void CUIComboBox::initialize()
{
	m_nTextSX = m_rcDropArea.Left + 1 + m_nSpaceX;
	m_nTextSY = m_rcDropArea.Top + 1 + m_nSpaceY;

	int	nOffset = m_nLineHeight - _pUIFontTexMgr->GetFontHeight();
	if( nOffset < 0 )
		nOffset = 0;
	m_nSelectOffsetY = nOffset / 2;

	m_bScrollBarExist = TRUE;	// 스트링 추가가 안되기 때문에 임시로 설정.
}

void CUIComboBox::OnUpdate( float fElapsedTime )
{

}

void CUIComboBox::OnRender( CDrawPort* pDraw )
{
	if( m_pTexData == NULL )
		return;

	pDraw->InitTextureData( m_pTexData );

	int	nX, nY;
	GetAbsPos( nX, nY );

	if( m_pBackground != NULL )
	{
		m_pBackground->SetPos(nX, nY);
		m_pBackground->RenderRectSurface(pDraw, 0xFFFFFFFF);
	}

	if( m_prsButton != NULL )
	{
		m_prsButton->SetPos(nX, nY);
		if( m_prsButton->GetCount() > 0 )
		{
			if (m_bDropList == FALSE)
				m_prsButton->RenderRectSurface(pDraw, 0xFFFFFFFF, 0);
			else
				m_prsButton->RenderRectSurface(pDraw, 0xFFFFFFFF, 1);
		}
	}

	if( m_pDropListSurface != NULL && m_bDropList )
	{
		pDraw->AddTexture( nX + m_rcBtn.Left, nY + m_rcBtn.Top,
			nX + m_rcBtn.Right, nY + m_rcBtn.Bottom,
			m_rtUp.U0, m_rtUp.V0, m_rtUp.U1, m_rtUp.V1,
			0xFFFFFFFF );

		m_pDropListSurface->SetPos(nX, nY);
		m_pDropListSurface->RenderRectSurface(pDraw, 0xFFFFFFFF);

		// Scroll bar
		if( m_bShowScrollBar )
			m_sbScrollBar.Render();
	}

	pDraw->FlushRenderingQueue();

	// Text
	if( m_vecString.size() > 0 )
	{
		int outX = 0;

		if (m_eAlignText_h == eALIGN_H_CENTER)
		{
			int str_width = pDraw->GetTextWidth2(m_vecString[m_nSelectList]);
			outX += (m_nWidth - str_width) / 2;
		}
		else if (m_eAlignText_h == eALIGN_H_RIGHT)
		{
			int str_width = pDraw->GetTextWidth2(m_vecString[m_nSelectList]);
			outX = outX + m_nWidth - str_width;
		}	

		pDraw->PutTextEx( m_vecString[m_nSelectList],
		nX + m_nTextSX + outX, nY + m_nComboTextSY, DEF_UI_FONT_COLOR );
	}

	if( m_bDropList )
	{
		int	nTexX = nX + m_nTextSX;
		int	nTexY = nY + m_nTextSY;
		int	nRowE = m_nScrollPos + m_nShowLine;
		if( nRowE > m_vecString.size() )
			nRowE = m_vecString.size();

		for( int iString = m_nScrollPos; iString < nRowE; iString++ )
		{
			int outX = 0;

			if (m_eAlignText_h == eALIGN_H_CENTER)
			{
				int str_width = pDraw->GetTextWidth2(m_vecString[iString]);
				outX += (m_nWidth - str_width) / 2;
			}
			else if (m_eAlignText_h == eALIGN_H_RIGHT)
			{
				int str_width = pDraw->GetTextWidth2(m_vecString[iString]);
				outX = outX + m_nWidth - str_width;
			}

			if( (m_nOverList + m_nScrollPos) == iString )
				pDraw->PutTextEx( m_vecString[iString], nTexX + outX, nTexY, m_colorDropSel );
			else
				pDraw->PutTextEx( m_vecString[iString], nTexX + outX, nTexY, m_colorDrop );

			nTexY += m_nLineHeight;
		}		
	}

	pDraw->EndTextEx();

#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL	
}

void CUIComboBox::OnUpdateDropList()
{
	if (m_pScroll)
		m_nScrollPos = m_pScroll->GetScrollPos();
}

void CUIComboBox::SetDropListUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
{
	/*m_rtDropListTop.SetUV( fTx0, fTy0, fTx1, fTy0 + CMB_DROPLIST_VERT_GAP, fTexWidth, fTexHeight );
	m_rtDropListMiddle.SetUV( fTx0, fTy0 + CMB_DROPLIST_VERT_GAP, fTx1, fTy1 - CMB_DROPLIST_VERT_GAP, fTexWidth, fTexHeight );
	m_rtDropListBottom.SetUV( fTx0, fTy1 - CMB_DROPLIST_VERT_GAP, fTx1, fTy1, fTexWidth, fTexHeight );*/
	if( m_pDropListSurface == NULL )
		m_pDropListSurface = new CUIRectSurface;

	m_pDropListSurface->AddRectSurface(UIRect(0,0,0,0),
		UIRectUV(fTx0, fTy0, fTx0 + CMB_DROPLIST_HORI_GAP, fTy0 + CMB_DROPLIST_VERT_GAP, fTexWidth, fTexHeight)); // T_L
	m_pDropListSurface->AddRectSurface(UIRect(0,0,0,0),
		UIRectUV(fTx0 + CMB_DROPLIST_HORI_GAP, fTy0, fTx1 - CMB_DROPLIST_HORI_GAP, fTy0 + CMB_DROPLIST_VERT_GAP, fTexWidth, fTexHeight)); // T_M
	m_pDropListSurface->AddRectSurface(UIRect(0,0,0,0),
		UIRectUV(fTx1 - CMB_DROPLIST_HORI_GAP, fTy0, fTx1, fTy0 + CMB_DROPLIST_VERT_GAP, fTexWidth, fTexHeight)); // T_R
	m_pDropListSurface->AddRectSurface(UIRect(0,0,0,0),
		UIRectUV(fTx0, fTy0 + CMB_DROPLIST_VERT_GAP, fTx0 + CMB_DROPLIST_HORI_GAP, fTy1 - CMB_DROPLIST_VERT_GAP, fTexWidth, fTexHeight)); // M_L
	m_pDropListSurface->AddRectSurface(UIRect(0,0,0,0),
		UIRectUV(fTx0 + CMB_DROPLIST_HORI_GAP, fTy0 + CMB_DROPLIST_VERT_GAP, fTx1 - CMB_DROPLIST_HORI_GAP, fTy1 - CMB_DROPLIST_VERT_GAP, fTexWidth, fTexHeight)); // M_M
	m_pDropListSurface->AddRectSurface(UIRect(0,0,0,0),
		UIRectUV(fTx1 - CMB_DROPLIST_HORI_GAP, fTy0 + CMB_DROPLIST_VERT_GAP, fTx1, fTy1 - CMB_DROPLIST_VERT_GAP, fTexWidth, fTexHeight)); // M_R
	m_pDropListSurface->AddRectSurface(UIRect(0,0,0,0),
		UIRectUV(fTx0, fTy1 - CMB_DROPLIST_VERT_GAP, fTx0 + CMB_DROPLIST_HORI_GAP, fTy1, fTexWidth, fTexHeight)); // B_L
	m_pDropListSurface->AddRectSurface(UIRect(0,0,0,0),
		UIRectUV(fTx0 + CMB_DROPLIST_HORI_GAP, fTy1 - CMB_DROPLIST_VERT_GAP, fTx1 - CMB_DROPLIST_HORI_GAP, fTy1, fTexWidth, fTexHeight)); // B_M
	m_pDropListSurface->AddRectSurface(UIRect(0,0,0,0),
		UIRectUV(fTx1 - CMB_DROPLIST_HORI_GAP, fTy1 - CMB_DROPLIST_VERT_GAP, fTx1, fTy1, fTexWidth, fTexHeight)); // B_R
}

void CUIComboBox::setComboButton( UIRect rect, UIRectUV uv0, UIRectUV uv1 )
{
	// XML 에서 생성된 버튼을 넣는다.
	if( m_prsButton == NULL )
		m_prsButton = new CUIRectSurface;

	m_rcComboButton  = rect;
#ifdef UI_TOOL
	m_uvComboButton0 = uv0;
	m_uvComboButton1 = uv1;
#endif // UI_TOOL

	if( m_pTexData != NULL )
	{
		FLOAT fW = m_pTexData->GetPixWidth();
		FLOAT fH = m_pTexData->GetPixHeight();

		uv0.U0 /= fW;		uv0.V0 /= fH;
		uv0.U1 /= fW;		uv0.V1 /= fH;

		uv1.U0 /= fW;		uv1.V0 /= fH;
		uv1.U1 /= fW;		uv1.V1 /= fH;
	}

	// 현재 renderSurface 가 UV 개념으로 되어 있기때문에 가공.
	UIRect rc = rect;
	rc.Right += rc.Left;
	rc.Bottom += rc.Top;
	
	m_prsButton->AddRectSurface(rc, uv0);
	m_prsButton->AddRectSurface(rc, uv1);
}

WMSG_RESULT CUIComboBox::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (!(m_dwWndState & UWS_ENABLE))
		return WMSG_FAIL;

	// 콤보의 경우 DropList 영역내에서 메세지처리가 되기 때문에 스크롤은 별도 처리해야 한다.
	if (m_pScroll)
	{
		WMSG_RESULT wRet = m_pScroll->OnLButtonDown(x, y);
		if (wRet != WMSG_FAIL)
			return wRet;
	}

	if (IsInsideRect( x, y, m_rcComboBase ))
	{
		m_bDropList = !m_bDropList;

		if (m_bDropList)
			CUIFocus::getSingleton()->setUI(this);
		else
			CUIFocus::getSingleton()->setUI(NULL);

		if (m_pScroll)
			m_pScroll->Hide(!m_bDropList);

		return WMSG_SUCCESS;
	}

	if (m_bDropList && IsInsideRect( x, y, m_rcDropArea ))
	{
		int	nAbsY = GetAbsPosY();
		m_nOverList = ( y - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight +
			m_sbScrollBar.GetScrollPos();
		if( m_nOverList >= 0 && m_nOverList < m_vecString.size() )
		{
			m_nSelectList = m_nOverList + m_nScrollPos;
			m_bDropList = !m_bDropList;

			if (m_bDropList == false)
			{
				CUIFocus::getSingleton()->setUI(NULL);

				if (m_pScroll)
					m_pScroll->Hide(TRUE);
			}

			if (m_pCmdUp)
				m_pCmdUp->execute();

			return WMSG_SUCCESS;
		}
	}

	return WMSG_FAIL;
}

WMSG_RESULT CUIComboBox::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (!(m_dwWndState & UWS_ENABLE))
		return WMSG_FAIL;

	if (m_bDropList && IsInsideRect( x, y, m_rcDropArea ))
	{
		// Drop 영역에 들어왔다면 child에 명령을 전달하지 않는다.
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

WMSG_RESULT CUIComboBox::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pScroll)
	{
		WMSG_RESULT wRet = m_pScroll->OnMouseMove(x, y, pMsg);
		if (wRet != WMSG_FAIL)
			return wRet;
	}

	if( m_nLineHeight > 0 )	
	{
 		int	nAbsY = GetAbsPosY();

		int Over = ( y - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight +
			m_sbScrollBar.GetScrollPos();

		if (m_nOverList != Over)
		{
			m_nOverList = Over;
			return WMSG_SUCCESS;
		}
	}

	return WMSG_FAIL;
}

void CUIComboBox::setBackGround( UIRect rect, UIRectUV uv )
{
	if( m_pBackground == NULL )
		m_pBackground = new CUIRectSurface;

	m_rcComboBase = rect;
#ifdef UI_TOOL
	m_uvComboBase = uv;
#endif // UI_TOOL
	
	UISUPPORT()->DivideTexUV(m_pTexData, uv);

	m_pBackground->AddRectSurface(rect, uv);
	
	m_nComboTextSY = ( rect.GetHeight() - _pUIFontTexMgr->GetFontHeight() ) / 2;
}

void CUIComboBox::setDropBG( UIRect rect, UIRectUV uv, int unit )
{
	if( m_pDropListSurface == NULL )
		m_pDropListSurface = new CUIRectSurface;

	m_rcDropArea	= rect;
#ifdef UI_TOOL	
	m_uvDropArea	= uv;
	m_nDropAreaUnit = unit;
#endif // UI_TOOL
	UISUPPORT()->Split3PlaneVertical((*m_pDropListSurface), m_pTexData, rect, uv, unit);
}

void CUIComboBox::setScroll( CUIScrollBar* pScroll )
{
	if (pScroll != NULL)
	{
		m_pScroll = pScroll;

		CmdOnChangeComboScroll* pCmd = new CmdOnChangeComboScroll;
		pCmd->setData(this);
		m_pScroll->SetCommand(pCmd);

		m_pScroll->Hide(TRUE);
	}
}

CUIBase* CUIComboBox::Clone()
{
	CUIComboBox* pCombo = new CUIComboBox(*this);

	CUIRectSurface* pRSBack = NULL;
	pRSBack = GetRectSurfaceBack();

	if( pRSBack != NULL )
	{
		{
			CUIRectSurface* pCopy = new CUIRectSurface;
			pCopy->CopyRectSurface(pRSBack);

			pCombo->SetRectSurfaceBack(pCopy);
		}		
	}

	CUIRectSurface* pRSDropList = NULL;
	pRSDropList = GetRectSurfaceDropList();

	if( pRSDropList != NULL )
	{
		{
			CUIRectSurface* pCopy = new CUIRectSurface;
			pCopy->CopyRectSurface(pRSDropList);

			pCombo->SetRectSurfaceDropList(pCopy);
		}		
	}

	CUIRectSurface* pRSButton = NULL;
	pRSButton = GetRectSurfaceButton();

	if( pRSButton != NULL )
	{
		{
			CUIRectSurface* pCopy = new CUIRectSurface;
			pCopy->CopyRectSurface(pRSButton);

			pCombo->SetRectSurfaceButton(pCopy);
		}		
	}

	CUIBase::CloneChild(pCombo);

	return pCombo;
}
