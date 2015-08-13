#include "stdh.h"

// #include <Engine/Interface/UIWindow.h>
#include <vector>
#include <Engine/Interface/UIButton.h>
#
#include <Engine/Interface/UISpinButton.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Interface/UICommand.h>

#define SPIN_TITLE_GAP			3	// 제목과 DataBack사이
#define SPIN_DATABACK_GAP		5	// DataBack과 Data사이 Gap(오른쪽 기준)
#define SPIN_BUTTON_OFFSET_X	0	// Control Button의 상하위치
#define SPIN_BUTTON_OFFSET_Y	3	// Control Button의 상하위치

#define SPIN_BTN_UP				"btn_up"
#define SPIN_BTN_DOWN			"btn_down"


class CmdOnChange : public Command
{
public:
	CmdOnChange() : parent_(NULL) {}

	void setData(CUISpinButton* parent, bool bUp)	{ parent_ = parent; bUp_ = bUp; }
	void execute() 
	{
		if (parent_)
		{
			if (bUp_)
				parent_->MoveDataIndex(1);
			else
				parent_->MoveDataIndex(-1);
		}
	}

private:
	CUISpinButton* parent_;
	bool bUp_;
};

//------------------------------------------------------------------------------
// CUISpinButton::CUISpinButton
// Explain:  
// Date : 2005-05-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUISpinButton::CUISpinButton()
{
	m_bDataBack	= FALSE;
	m_strTitle	= "";	
	m_nSelectIndex = -1;
	m_bVertical = FALSE;
	m_nStrWidth	= 0;

	int i;
	m_nSpinBtnWidth		= 0;
	m_nSpinBtnHeight	= 0;
	m_nSpinBtnGap		= 0;
	m_nDataBackWidth	= 0;
	for( i = 0; i < eBTN_MAX; i++ )
		m_pSpinBtn[i] = NULL;

#ifdef UI_TOOL
	m_bBtnAutoInfo		= true;
	m_nAdjust			= 0;
#endif // UI_TOOL

	setType(eUI_CONTROL_SPINBUTTON);
}

CUIBase* CUISpinButton::Clone()
{
	CUISpinButton* pSpinBtn = new CUISpinButton(*this);

	CUIBase::CloneChild(pSpinBtn);

	return pSpinBtn;
}

//------------------------------------------------------------------------------
// CUISpinButton::Create
// Explain:  MultiList생성시 추가 작업 필요로 
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISpinButton::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, CTString strTitle, int nDataBackWidth )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_strTitle = strTitle;
	m_nDataBackWidth = nDataBackWidth;
#if defined (G_THAI)
	m_nStrWidth = FindThaiLen( m_strTitle ) + 3;
#else
	m_nStrWidth = ( m_strTitle.Length() ) * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) + 3;
#endif
}

//------------------------------------------------------------------------------
// CUISpinButton::CreateButtons
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISpinButton::CreateButtons( int nBtnWidth, int nBtnHeight, int nButtonGap, BOOL bVertical )
{
	m_bVertical = bVertical;
	
	int nX = m_nStrWidth + m_nDataBackWidth + SPIN_BUTTON_OFFSET_X;

	// Create buttons
	if( m_bVertical )
	{
		m_btnUp.Create( this, CTString( "" ), nX, SPIN_BUTTON_OFFSET_Y, nBtnWidth, nBtnHeight );
		m_btnDown.Create( this, CTString( "" ), nX, SPIN_BUTTON_OFFSET_Y + nBtnHeight - nButtonGap, nBtnWidth, nBtnHeight );
	}
	else
	{
		m_btnUp.Create( this, CTString( "" ), nX, SPIN_BUTTON_OFFSET_Y, nBtnWidth, nBtnHeight );
		m_btnDown.Create( this, CTString( "" ), nX + nBtnWidth + nButtonGap, SPIN_BUTTON_OFFSET_Y, nBtnWidth, nBtnHeight );
	}
}

//------------------------------------------------------------------------------
// CUISpinButton::Render
// Explain:  기본 루틴 그대로 가져옴.. 일부 수정
// Date : 2005-05-18,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISpinButton::Render()
{
	if( !IsVisible() ) return;

	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	// Data 배경 
	if( m_bDataBack )
	{
		//RenderRectUV3( 4, nX + m_nStrWidth, nY + m_nHeight - _pUIFontTexMgr->GetFontHeight(), m_nDataBackWidth, m_nHeight, m_rt3Data );
		RenderRectUV3( 4, nX + m_nStrWidth, nY +1, m_nDataBackWidth, m_nHeight, m_rt3Data );
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Title : 나중에 색정할수 있게 하면 좋겠다.
	pDrawPort->PutTextEx( m_strTitle, nX, nY + SPIN_TITLE_GAP );


	// Data 
	if( m_nSelectIndex >= 0 )
	{
		CTString strData = m_vecData[m_nSelectIndex];
		int nOffsetX, nOffsetY;

		nOffsetX = nX + m_nStrWidth + m_nDataBackWidth // 여기가 맨 왼쪽 
					- strData.Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - SPIN_DATABACK_GAP;

		
		nOffsetY = nY + 2;

		pDrawPort->PutTextEx( strData, nOffsetX, nOffsetY ); 
		
	}
	
	// Render UP/DOWN Button
	m_btnUp.Render();
	m_btnDown.Render();
}

//------------------------------------------------------------------------------
// CUISpinButton::MouseMessage
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUISpinButton::MouseMessage( MSG *pMsg )
{
	// If scroll bar is disabled
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Messages
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			m_btnUp.MouseMessage(pMsg);
			m_btnDown.MouseMessage(pMsg);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				// Up button
				if( m_btnUp.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Down button
				else if( m_btnDown.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			if( IsInside( nX, nY ) )
			{
				// Up button
				if( m_btnUp.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				//	MoveDataIndex( -1 );
					MoveDataIndex( +1 );
					return WMSG_COMMAND;
				}
				// Down button
				else if( m_btnDown.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				//	MoveDataIndex( +1 );
					MoveDataIndex( -1 );
					return WMSG_COMMAND;
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// Up button
				if( m_btnUp.MouseMessage( pMsg ) == WMSG_COMMAND )
				{
					MoveDataIndex( -1 );
					return WMSG_COMMAND;
				}
				// Down button
				else if( m_btnDown.MouseMessage( pMsg ) == WMSG_COMMAND )
				{
					MoveDataIndex( +1 );
					return WMSG_COMMAND;
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) || IsInsideRect( nX, nY, m_rcWheel ) )
			{
				SWORD	swDelta = SWORD( HIWORD( pMsg->wParam ) ) / WHEEL_DELTA;
				MoveDataIndex( -swDelta );

				return WMSG_SUCCESS;
			}
		}
		break;
	}	
	return WMSG_FAIL;
}

//------------------------------------------------------------------------------
// CUISpinButton::AddData
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISpinButton::AddData( CTString strData )
{
	m_nSelectIndex = 0;
	m_vecData.push_back( strData );
}

//------------------------------------------------------------------------------
// CUISpinButton::GetDataToNumber
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
int CUISpinButton::GetDataToNumber( int nIndex )
{
	if( nIndex < -1 || nIndex > (int)m_vecData.size() - 1 ) return -1;

	if( nIndex == -1 ) nIndex = m_nSelectIndex;

	int nData = atoi( m_vecData[nIndex].str_String );
	return nData;
}

//------------------------------------------------------------------------------
// CUISpinButton::GetDataToString
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CTString CUISpinButton::GetDataToString( int nIndex )
{
	if( nIndex < -1 || nIndex > (int)m_vecData.size() - 1 ) return "NULL";
	
	if( nIndex == -1 ) nIndex = m_nSelectIndex;

	return m_vecData[nIndex];
}

//------------------------------------------------------------------------------
// CUISpinButton::MoveDataIndex
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISpinButton::MoveDataIndex( int nIndexDelta )
{
	m_nSelectIndex += nIndexDelta;

	if( m_nSelectIndex < 0 ) m_nSelectIndex = 0;
	if( m_nSelectIndex > (int)m_vecData.size() - 1 ) m_nSelectIndex = (int)m_vecData.size() - 1;

}

//------------------------------------------------------------------------------
// CUISpinButton::RenderRectUV3
// Explain:  
// Date : 2005-06-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISpinButton::RenderRectUV3( int nUnitSize, int nX, int nY, int nWidth, int nHeight, UIRectUV3 rtRectUV3 )
{
	int nX2 = nX + nWidth;
	int nY2 = nY + nHeight;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->AddTexture( nX, nY, nX + nUnitSize, nY2,
										rtRectUV3.rtL.U0, rtRectUV3.rtL.V0, rtRectUV3.rtL.U1, rtRectUV3.rtL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX + nUnitSize, nY, nX2 - nUnitSize, nY2,
										rtRectUV3.rtM.U0, rtRectUV3.rtM.V0, rtRectUV3.rtM.U1, rtRectUV3.rtM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( nX2 - nUnitSize, nY, nX2, nY2,
										rtRectUV3.rtR.U0, rtRectUV3.rtR.V0, rtRectUV3.rtR.U1, rtRectUV3.rtR.V1,
										0xFFFFFFFF );
}

void CUISpinButton::SetTitle( CTString strTitle )
{ 
	m_strTitle =strTitle;
	if( m_strTitle.IsEmpty() )	// 타이틀 string이 없다면 0으로 초기화.
		m_nStrWidth = 0;
	else
		m_nStrWidth = ( m_strTitle.Length() ) * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) + 3;

	int nX = m_nStrWidth + m_nDataBackWidth + SPIN_BUTTON_OFFSET_X;
	
	m_btnUp.SetPosX( nX );
	m_btnDown.SetPosX( nX );

	// 툴 작업을 위해 새로 만든 버튼 세팅
	int i;
	for( i = 0; i < eBTN_MAX; i++ )
	{
		if( m_pSpinBtn[i] )
			m_pSpinBtn[i]->SetPosX(nX);
	}
}

void CUISpinButton::initialize()
{
	int i;
	for( i = 0; i < eBTN_MAX; i++ )
	{
		CmdOnChange* pCmd = new CmdOnChange;

		if( i == eBTN_UP )
		{
			m_pSpinBtn[i] = (CUIButton*)findUI(SPIN_BTN_UP);
			pCmd->setData(this, true);
		}
		else if( i == eBTN_DOWN )
		{
			m_pSpinBtn[i] = (CUIButton*)findUI(SPIN_BTN_DOWN);
			pCmd->setData(this, false);
		}

		m_pSpinBtn[i]->SetCommandUp(pCmd);
#ifdef UI_TOOL
		setBtnAutoInfo(m_bBtnAutoInfo);
#endif // UI_TOOL
	}
}

void CUISpinButton::OnRender( CDrawPort* pDraw )
{
	if( !IsVisible() ) return;

	if( m_bHide == TRUE || m_pTexData == NULL)
		return;

	// Get position
	int	nX, nY;
	GetAbsPos( nX, nY );

	pDraw->InitTextureData( m_pTexData );

	// Data 배경 
	if( m_bDataBack )
	{
		//RenderRectUV3( 4, nX + m_nStrWidth, nY + m_nHeight - _pUIFontTexMgr->GetFontHeight(), m_nDataBackWidth, m_nHeight, m_rt3Data );

		RenderRectUV3( 0, nX + m_nStrWidth, nY, m_nDataBackWidth, m_nHeight, m_rt3Data );
		pDraw->FlushRenderingQueue();
	}
	
	pDraw->PutTextEx( m_strTitle, nX, nY );
	pDraw->EndTextEx();
	// Data 
	if( m_nSelectIndex >= 0 )
	{
		CTString strData = m_vecData[m_nSelectIndex];
		int nOffsetX, nOffsetY;

		nOffsetX = nX + m_nStrWidth + 3;/* + m_nDataBackWidth // 여기가 맨 왼쪽 
			- strData.Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - SPIN_DATABACK_GAP;*/

		nOffsetY = nY /*+ 2*/;

		pDraw->PutTextEx( strData, nOffsetX, nOffsetY ); 
		pDraw->EndTextEx();
	}

	int i;
	for( i = 0; i < eBTN_MAX; i++ )
	{
		if( m_pSpinBtn[i] )
			m_pSpinBtn[i]->Render();
	}
#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

WMSG_RESULT CUISpinButton::OnLButtonUp( UINT16 x, UINT16 y )
{
	if( m_bHide == TRUE )
		return WMSG_FAIL;

	if( IsInside(x, y) == FALSE )
		return WMSG_FAIL;

	return WMSG_FAIL;
}

WMSG_RESULT CUISpinButton::OnMouseWheel( UINT16 x, UINT16 y, int wheel )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( IsInsideRect( x, y, m_rcWheel ) )
	{
		SWORD	swDelta = SWORD( wheel ) / WHEEL_DELTA;

		MoveDataIndex(swDelta);
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

void CUISpinButton::setUVDataBack( UIRectUV uv )
{
	m_bDataBack = TRUE;
	
#ifdef UI_TOOL
	m_uvDataBack = uv;
#endif // UI_TOOL
	UIRectUV uv_[3];
	FLOAT fTexW = 1.0f;
	FLOAT fTexH = 1.0f;
	
	uv_[0] = UIRectUV(uv.U0, uv.V0, uv.U0 + 4, uv.V1, fTexW, fTexH);
	uv_[1] = UIRectUV(uv_[0].U1, uv.V0, uv.U1 - 4, uv.V1, fTexW, fTexH);
	uv_[2] = UIRectUV(uv_[1].U1, uv.V0, uv.U1, uv.V1, fTexW, fTexH);

	if( m_pTexData )
	{
		fTexW = m_pTexData->GetWidth();
		fTexH = m_pTexData->GetHeight();
	}

	m_rt3Data.SetUV3(uv_[0].U0, uv_[0].V0, uv_[0].U0, uv_[0].V1
					, uv_[1].U0, uv_[1].V0, uv_[1].U0, uv_[1].V1
					, uv_[2].U0, uv_[2].V0, uv_[2].U0, uv_[2].V1, fTexW, fTexH);
}
#ifdef UI_TOOL
void CUISpinButton::SetSpinButton( int type, CUIButton* pButton )
{
	if( pButton == NULL )
		return;

	if( type < 0 && type > eBTN_MAX )
		return;

	m_pSpinBtn[(eBTN_STATE)type]	= pButton;
	this->addChild(m_pSpinBtn[(eBTN_STATE)type]);
}

void CUISpinButton::setBtnAutoInfo( bool bAuto )
{
	m_bBtnAutoInfo = bAuto;
	int i;
	for( i = 0; i < eBTN_MAX; i++ )
	{
		if( m_pSpinBtn[i] )
		{
			if( m_bBtnAutoInfo == false )	// Tool에서 m_bBtnAutoInfo 가 true라면 자동으로 버튼 세팅.
				continue;

			if( m_strTitle.IsEmpty() == false )
				m_nStrWidth = ( m_strTitle.Length() ) * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) + 3;

			int nX = m_nDataBackWidth + m_nStrWidth;

			if( i == eBTN_UP )
				m_pSpinBtn[i]->InitPos(nX, 0, m_nSpinBtnWidth, m_nSpinBtnHeight );
			else
				m_pSpinBtn[i]->InitPos(nX, m_nSpinBtnHeight + m_nSpinBtnGap, m_nSpinBtnWidth, m_nSpinBtnHeight );
		}
	}
}

#endif // UI_TOOL