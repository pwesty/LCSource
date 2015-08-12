// UIChildQuickSlot.cpp: implementation of the UIChildQuickSlot class.
//
//////////////////////////////////////////////////////////////////////
#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIChildQuickSlot.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIMessenger.h>


// UI 퀵슬롯 확장 개편 작업 확장 슬롯 위치 [12/18/2012 Ranma]
extern INDEX g_iXPosQuickSlotEX1;
extern INDEX g_iYPosQuickSlotEX1;
extern INDEX g_iXPosQuickSlotEX2;
extern INDEX g_iYPosQuickSlotEX2;

extern INDEX g_bQuickSlot1HorOrVer;
extern INDEX g_bQuickSlot1Lock;
extern INDEX g_bQuickSlot2HorOrVer;
extern INDEX g_bQuickSlot2Lock;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIChildQuickSlot::CUIChildQuickSlot(int nid)
: m_abtnItems(NULL), m_bIsInside(FALSE)
{
	m_nID = nid;
	m_bRotation[0] = &g_bQuickSlot1HorOrVer;
	m_bRotation[1] = &g_bQuickSlot2HorOrVer;
	m_bLock[0] = &g_bQuickSlot1Lock;
	m_bLock[1] = &g_bQuickSlot2Lock;
	
	if (m_nID == UI_QUICKSLOT2)
		m_nCurSlot = 0;
	else
		m_nCurSlot = 1;
	
	m_bEXSlot1On = FALSE;
	m_bEXSlot2On = FALSE;
	
	m_nSelSlotBtnID = -1;
	
	m_nStartX = 0;
	m_nStartY = 0;
}

CUIChildQuickSlot::~CUIChildQuickSlot()
{
	Destroy();
}

void CUIChildQuickSlot::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	//============== RECT ==============
	m_rcDragArea.SetRect( 0, 0, 15, 37 );
	
	///< create UIQuickslot Texture
	m_ptdBaseTexture = CreateTexture(CTString("Data\\Interface\\TopUI.tex"));
	
	///< Texture size
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	//============== U . V ==============
	m_rtHorzBackground.SetUV(QSLOT_VER_BASE_TEXTURE_STARTX, QSLOT_VER_BASE_TEXTURE_STARTY, QSLOT_VER_BASE_TEXTURE_ENDX+1, QSLOT_VER_BASE_TEXTURE_ENDY+1, fTexWidth, fTexHeight);
	m_rtVerBackground.SetUV(QSLOT_HORZ_BASE_TEXTURE_STARTX, QSLOT_HORZ_BASE_TEXTURE_STARTY, QSLOT_HORZ_BASE_TEXTURE_ENDX+1, QSLOT_HORZ_BASE_TEXTURE_ENDY+1, fTexWidth, fTexHeight);
	
	
	switch(m_nID)
	{
	case UI_QUICKSLOT2:
		{
			m_rtHorzSlotNumber.SetUV(120, 574, 518, 581, fTexWidth, fTexHeight);
			m_rtVerSlotNumber.SetUV(80, 533, 93, 925, fTexWidth, fTexHeight);
		}
		break;
		
	case UI_QUICKSLOT3:
		{
			m_rtHorzSlotNumber.SetUV(120, 557, 526, 564, fTexWidth, fTexHeight);
			m_rtVerSlotNumber.SetUV(55, 533, 76, 925, fTexWidth, fTexHeight);
		}
		break;
	}
	
	// UV of rotation
	m_rtHorzOver.SetUV(680, 549, 697, 566, fTexWidth, fTexHeight);
	m_rtHorzClick.SetUV(698, 549, 715, 566, fTexWidth, fTexHeight);
	m_rtVerOver.SetUV(680, 567, 697, 584, fTexWidth, fTexHeight);
	m_rtVerClick.SetUV(698, 567, 715, 584, fTexWidth, fTexHeight);
	
	// rotation
	m_btnHorizontal.Create( this, CTString( "" ), CHILD_QUICKSLOT_WIDTH - QSLOT_OUTSIDE_BTN_SIZE - 1, 1, QSLOT_OUTSIDE_BTN_SIZE, QSLOT_OUTSIDE_BTN_SIZE );
	m_btnHorizontal.CopyUV( UBS_ON, m_rtHorzOver );
	m_btnHorizontal.CopyUV( UBS_CLICK, m_rtHorzClick );
	m_btnHorizontal.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	m_btnVertical.Create( this, CTString( "" ), QSLOT_OUTSIDE_BTN_SIZE + 2, CHILD_QUICKSLOT_WIDTH - QSLOT_OUTSIDE_BTN_SIZE - 1, QSLOT_OUTSIDE_BTN_SIZE, QSLOT_OUTSIDE_BTN_SIZE );
	m_btnVertical.CopyUV( UBS_ON, m_rtVerOver );
	m_btnVertical.CopyUV( UBS_CLICK, m_rtVerClick );
	m_btnVertical.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// UV of lock
	m_rtLockIdle.SetUV(662, 513, 662+17, 513+17, fTexWidth, fTexHeight);
	m_rtLockOver.SetUV(680, 513, 680+17, 513+17, fTexWidth, fTexHeight);
	m_rtLockClick.SetUV(698, 513, 698+17, 513+17, fTexWidth, fTexHeight);
	
	m_rtUnLockIdle.SetUV(662, 531, 662+17, 531+17, fTexWidth, fTexHeight);
	m_rtUnLockOver.SetUV(680, 531, 680+17, 531+17, fTexWidth, fTexHeight);
	m_rtUnLockClick.SetUV(698, 531, 698+17, 531+17, fTexWidth, fTexHeight);
	
	// lock
	m_btnLock.Create( this, CTString( "" ), CHILD_QUICKSLOT_WIDTH - QSLOT_OUTSIDE_BTN_SIZE - 1, 19, QSLOT_OUTSIDE_BTN_SIZE, QSLOT_OUTSIDE_BTN_SIZE );
	m_btnLock.CopyUV( UBS_ON, m_rtLockOver );
	m_btnLock.CopyUV( UBS_CLICK, m_rtLockClick );
	m_btnLock.CopyUV( UBS_IDLE, m_rtLockIdle );
	
	m_btnUnLock.Create( this, CTString( "" ), CHILD_QUICKSLOT_WIDTH - QSLOT_OUTSIDE_BTN_SIZE - 1, 19, QSLOT_OUTSIDE_BTN_SIZE, QSLOT_OUTSIDE_BTN_SIZE );
	m_btnUnLock.CopyUV( UBS_ON, m_rtUnLockOver );
	m_btnUnLock.CopyUV( UBS_CLICK, m_rtUnLockClick );
	m_btnUnLock.CopyUV( UBS_IDLE, m_rtUnLockIdle );
	//============== BUTTON ==============
	
	if( *(m_bRotation[m_nCurSlot]) == 1 ) // 1 가로 0 세로 [12/21/2012 Ranma]
	{	
		SetSize( CHILD_QUICKSLOT_WIDTH, CHILD_QUICKSLOT_HEIGHT );
		m_rcDragArea.SetRect( 0, 0, 15, 37 );
		m_rcSlot.SetRect(15, 0, CHILD_QUICKSLOT_RCSLOT_AREA , CHILD_QUICKSLOT_HEIGHT);
	}
	else if ( *(m_bRotation[m_nCurSlot]) == 0 )
	{
		SetSize( CHILD_QUICKSLOT_HEIGHT, CHILD_QUICKSLOT_WIDTH );
		m_rcDragArea.SetRect( 0, 0, 37, 15 );
		m_rcSlot.SetRect(0, 15, CHILD_QUICKSLOT_HEIGHT, CHILD_QUICKSLOT_RCSLOT_AREA);
	}
}
// ----------------------------------------------------------------------------
// Name : CheckOnOff()
// Desc :
// ----------------------------------------------------------------------------
void CUIChildQuickSlot::CheckOnOff()
{
	switch(m_nID)
	{
	case UI_QUICKSLOT2:
		{
			m_strPageNumber = CTString("2");
			
			if (g_bQuickSlotEX1)
				m_bEXSlot1On = TRUE;
			break;	
		}
		
	case UI_QUICKSLOT3:
		{
			m_strPageNumber = CTString("3");
			
			if (g_bQuickSlotEX2)
				m_bEXSlot2On = TRUE;
			break;	
		}
	}
	
}

void CUIChildQuickSlot::FirstOpen()
{

	CheckOnOff();

	switch( m_nID )
	{
	case UI_QUICKSLOT2:
		{
			if (m_bEXSlot1On)
			{
				OpenUI();
				m_bEXSlot1On = FALSE;
			}
		}break;
		
	case UI_QUICKSLOT3:
		{
			if (m_bEXSlot2On)
			{
				OpenUI();
				m_bEXSlot2On = FALSE;
			}
		}break;
	}				
}
// ----------------------------------------------------------------------------
// Name : SetItem()
// Desc : 퀵슬롯과 아이템 연결
// ----------------------------------------------------------------------------
void CUIChildQuickSlot::SetItem(abtnItems* btnItems)
{
	m_abtnItems = btnItems;
}
// ----------------------------------------------------------------------------
// Name : SlotRender()
// Desc :
// ----------------------------------------------------------------------------
void CUIChildQuickSlot::SlotRender()
{
	if (!m_abtnItems)
		return;
	
	int	iBtn, nPosX, nPosY;
	
	if (*(m_bRotation[m_nCurSlot]) == 0) // 0 세로 일 때 버튼 위치 설정 [12/21/2012 Ranma]
	{
		nPosY = 16;
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nPosY += 35 )
		{
			(*m_abtnItems)[m_nCurSlot + 1][iBtn]->SetPos( m_nPosX + 3,m_nPosY + nPosY);
		}
	}
	else
	{
		nPosX = 16;
		for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nPosX += 35 )
		{
			(*m_abtnItems)[m_nCurSlot + 1][iBtn]->SetPos( m_nPosX + nPosX, m_nPosY + 3);
		}
	}	
	
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	// Skill buttons
	for( iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
	{
		if ((*m_abtnItems)[m_nCurSlot + 1][iBtn]->IsEmpty() == true)
			continue;
		
		(*m_abtnItems)[m_nCurSlot + 1][iBtn]->Render(pDrawPort);
	}
	
	// 슬롯 단축키 랜더 [12/24/2012 Ranma]
	SlotNumberRender();
}


void CUIChildQuickSlot::SlotNumberRender()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	
	///< texture init
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	if( *(m_bRotation[m_nCurSlot]) == 1 )
	{
		int	nX = m_nPosX;
		int	nY = m_nPosY;
		// 슬롯 번호 텍스춰
		nX = m_nPosX + 16;
		nY = m_nPosY + 4;
		switch(m_nID)
		{
		case UI_QUICKSLOT2:
			{
				pDrawPort->AddTexture(nX, nY, nX + 398, nY + 7
					, m_rtHorzSlotNumber.U0, m_rtHorzSlotNumber.V0
					, m_rtHorzSlotNumber.U1, m_rtHorzSlotNumber.V1
					, 0xFFFFFFFF);
				break;
			}
			
		case UI_QUICKSLOT3:
			{
				pDrawPort->AddTexture(nX, nY, nX + 406, nY + 7
					, m_rtHorzSlotNumber.U0, m_rtHorzSlotNumber.V0
					, m_rtHorzSlotNumber.U1, m_rtHorzSlotNumber.V1
					, 0xFFFFFFFF);
				break;
			}
		}
	}
	else if ( *(m_bRotation[m_nCurSlot]) == 0 )
	{
		int	nX = m_nPosX;
		int	nY = m_nPosY;
		// 슬롯 번호 텍스춰
		nX = m_nPosX + 4;
		nY = m_nPosY + 16;
		
		switch(m_nID)
		{
		case UI_QUICKSLOT2:
			{
				pDrawPort->AddTexture(nX, nY, nX + 13, nY + 392
					, m_rtVerSlotNumber.U0, m_rtVerSlotNumber.V0
					, m_rtVerSlotNumber.U1, m_rtVerSlotNumber.V1
					, 0xFFFFFFFF);
				break;
			}
			
		case UI_QUICKSLOT3:
			{
				pDrawPort->AddTexture(nX, nY, nX + 21, nY + 392
					, m_rtVerSlotNumber.U0, m_rtVerSlotNumber.V0
					, m_rtVerSlotNumber.U1, m_rtVerSlotNumber.V1
					, 0xFFFFFFFF);
				break;
			}
		}
	}
	
	pDrawPort->FlushRenderingQueue();
}

void CUIChildQuickSlot::ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	g_iXPosQuickSlotEX1 = 0;
	g_iYPosQuickSlotEX1 = 0;
	g_iXPosQuickSlotEX2 = 0;
	g_iYPosQuickSlotEX2 = 0;

	g_bQuickSlot1HorOrVer = 1;
	g_bQuickSlot1Lock = 0;
	g_bQuickSlot2HorOrVer = 1;
	g_bQuickSlot2Lock = 0;

	RotationRectSet();
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );

	SetPos(m_nStartX, m_nStartY);
}


// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIChildQuickSlot::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	
	int nX = (pixMaxI - this->GetPosX()) - m_nWidth;
	int nY = (pixMaxJ - this->GetPosY()) - m_nHeight;
	int nBaseYGap = 0;
	BOOL bSave = TRUE;
	switch(m_nID)
	{
	case UI_QUICKSLOT2:
		{
			nBaseYGap = 37;
			if (g_iXPosQuickSlotEX1 == 0 && g_iYPosQuickSlotEX1 == 0)
			{
				m_nStartX = pixMaxI - m_nWidth;
				m_nStartY = pixMaxJ - m_nHeight - 44/*QUICKSLOT_HEIGHT*/;
				bSave = FALSE;
			}
			
		break;
		}
	case UI_QUICKSLOT3:
		{
			nBaseYGap = 74;
			if (g_iXPosQuickSlotEX2 == 0 && g_iYPosQuickSlotEX2 == 0)
			{
				m_nStartX = pixMaxI - m_nWidth;
				m_nStartY = pixMaxJ - m_nHeight - 44/*QUICKSLOT_HEIGHT*/ - CHILD_QUICKSLOT_HEIGHT;
				bSave = FALSE;
			}
		break;				
		}
	}

	if (nX > 0 && nY > 0)
	{
		return;
	}
	else if (nX < 0 && nY > 0)
	{
		SetPos(this->GetPosX() + nX, this->GetPosY() - nBaseYGap);
	}
	else if (nX > 0 && nY < 0)
	{
		SetPosY(this->GetPosY() + nY - nBaseYGap);
	}
	else
	{
		SetPos(this->GetPosX() + nX, this->GetPosY() + nY - nBaseYGap);
	}

	if (bSave)
	{
		SetPosEXQSlot();
	}
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIChildQuickSlot::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIChildQuickSlot::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	
	///< texture init
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	if( *(m_bRotation[m_nCurSlot]) == 1 )
	{
		int	nX = m_nPosX;
		int	nY = m_nPosY;
		// Quick slot base texture
		pDrawPort->AddTexture( nX, nY
			,nX + CHILD_QUICKSLOT_WIDTH, nY + CHILD_QUICKSLOT_HEIGHT
			,m_rtHorzBackground.U0, m_rtHorzBackground.V0
			,m_rtHorzBackground.U1, m_rtHorzBackground.V1
			,0xFFFFFFFF );
		
		// 퀵슬롯 페이지 출력 위치를 수동으로 계산 [12/20/2012 Ranma]
		pDrawPort->PutTextExCX(m_strPageNumber, m_nPosX + 7, m_nPosY + 11 );
		m_btnHorizontal.Render();
		m_btnLock.SetPos(CHILD_QUICKSLOT_WIDTH - QSLOT_OUTSIDE_BTN_SIZE - 1, 19);
		m_btnUnLock.SetPos(CHILD_QUICKSLOT_WIDTH - QSLOT_OUTSIDE_BTN_SIZE - 1, 19);
	}
	else if ( *(m_bRotation[m_nCurSlot]) == 0 )
	{
		int	nX = m_nPosX;
		int	nY = m_nPosY;
		// Quick slot base texture
		pDrawPort->AddTexture( nX, nY
			, nX + CHILD_QUICKSLOT_HEIGHT, nY + CHILD_QUICKSLOT_WIDTH
			, m_rtVerBackground.U0, m_rtVerBackground.V0
			, m_rtVerBackground.U1, m_rtVerBackground.V1
			, 0xFFFFFFFF );
		
		// 퀵슬롯 페이지 출력 위치를 수동으로 계산 [12/20/2012 Ranma]
		pDrawPort->PutTextExCX(m_strPageNumber, m_nPosX + 18, m_nPosY + 2 );
		m_btnVertical.Render();
		m_btnLock.SetPos(1, CHILD_QUICKSLOT_WIDTH - QSLOT_OUTSIDE_BTN_SIZE - 1);
		m_btnUnLock.SetPos(1, CHILD_QUICKSLOT_WIDTH - QSLOT_OUTSIDE_BTN_SIZE - 1);
	}
	
	if(*(m_bLock[m_nCurSlot]))
		m_btnLock.Render();
	else
		m_btnUnLock.Render();
	
	pDrawPort->FlushRenderingQueue();
	
	SlotRender();
	
	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : OpenUI()
// Desc :
// ----------------------------------------------------------------------------
void CUIChildQuickSlot::OpenUI()
{
	if (IsVisible()) // 중복 실행 방지
		return;
	
	switch( m_nID )
	{
	case UI_QUICKSLOT2:
		{
			g_bQuickSlotEX1 = 1;
			
			if (g_iXPosQuickSlotEX1 == 0 && g_iYPosQuickSlotEX1 == 0)
			{
				SetPos(m_nStartX, m_nStartY);
				break;
			}
			SetPos( g_iXPosQuickSlotEX1, g_iYPosQuickSlotEX1 );
			break;	
		}
	case UI_QUICKSLOT3:
		{
			g_bQuickSlotEX2 = 1;
			
			if (g_iXPosQuickSlotEX2 == 0 && g_iYPosQuickSlotEX2 == 0)
			{
				SetPos(m_nStartX, m_nStartY);
				break;
			}
			SetPos( g_iXPosQuickSlotEX2, g_iYPosQuickSlotEX2 );
			break;	
		}
	}				
	
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( m_nID, TRUE );
}

// ----------------------------------------------------------------------------
// Name : _CloseUI()
// Desc : Close the ChildQuickSlot
// ----------------------------------------------------------------------------
void CUIChildQuickSlot::_CloseUI()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	switch( m_nID )
	{
	case UI_QUICKSLOT2:
		g_bQuickSlotEX1 = 0;
		break;
		
	case UI_QUICKSLOT3:
		g_bQuickSlotEX2 = 0;
		break;
	}				
	pUIManager->RearrangeOrder( m_nID, FALSE );
}

void CUIChildQuickSlot::ToggleVisible(const BOOL bResultView /* = false */)
{
	
	if( this->IsVisible() )
	{
		// 닫을지 말지 결정.
		if (bResultView)
			return;
		else
			_CloseUI();
	}
	else
	{
		if (bResultView)
		{
			OpenUI();
		}		
	}	
}
// ----------------------------------------------------------------------------
// Name : Rotation()
// Desc :
// ----------------------------------------------------------------------------
void CUIChildQuickSlot::Rotation()
{
	if( *(m_bRotation[m_nCurSlot]) == 1 ) // 1 가로 0 세로 [12/21/2012 Ranma]
	{
		int nGap = (CHILD_QUICKSLOT_WIDTH - CHILD_QUICKSLOT_HEIGHT);
		
		SetPos( GetPosX() - nGap, GetPosY() + nGap );
		SetPosEXQSlot();
	}
	else if ( *(m_bRotation[m_nCurSlot]) == 0 )
	{
		int nGap = (CHILD_QUICKSLOT_WIDTH - CHILD_QUICKSLOT_HEIGHT);
		
		SetPos( GetPosX() + nGap, GetPosY() - nGap );
		SetPosEXQSlot();
	}

	RotationRectSet();
}
void CUIChildQuickSlot::RotationRectSet()
{
	if( *(m_bRotation[m_nCurSlot]) == 1 ) // 1 가로 0 세로 [12/21/2012 Ranma]
	{
		SetSize( CHILD_QUICKSLOT_WIDTH, CHILD_QUICKSLOT_HEIGHT );
		m_rcSlot.SetRect(15, 0, CHILD_QUICKSLOT_WIDTH , CHILD_QUICKSLOT_HEIGHT);
		m_rcDragArea.SetRect( 0, 0, 15, 37 );
	}
	else if ( *(m_bRotation[m_nCurSlot]) == 0 )
	{
		SetSize( CHILD_QUICKSLOT_HEIGHT, CHILD_QUICKSLOT_WIDTH );
		m_rcSlot.SetRect(0, 15, CHILD_QUICKSLOT_HEIGHT, CHILD_QUICKSLOT_WIDTH);
		m_rcDragArea.SetRect( 0, 0, 37, 15 );
	}
	
}

void CUIChildQuickSlot::SetPosEXQSlot()
{
	switch(m_nID)
	{
	case UI_QUICKSLOT2:
		{
			g_iXPosQuickSlotEX1 = GetPosX();
			g_iYPosQuickSlotEX1 = GetPosY();
			break;
		}
	case UI_QUICKSLOT3:
		{
			g_iXPosQuickSlotEX2 = GetPosX();
			g_iYPosQuickSlotEX2 = GetPosY();
			break;
		}
	}	
}
// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIChildQuickSlot::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	
	// Title bar
	static BOOL bDragAreaClick = FALSE;
	
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;
	
	// Mouse point
	static int	nOldX, nOldY;
	
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			
			m_bIsInside = IsInside( nX, nY );
			if (m_bIsInside)
			{
				if (*(m_bRotation[m_nCurSlot]) == 0) // 0 세로 일 때 버튼 위치 설정 [12/21/2012 Ranma]
				{
					int nPosY = 16;
					for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nPosY += 35 )
					{
						(*m_abtnItems)[m_nCurSlot + 1][iBtn]->SetPos( m_nPosX + 3,m_nPosY + nPosY);
					}
				}
				else
				{
					int nPosX = 16;
					for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nPosX += 35 )
					{
						(*m_abtnItems)[m_nCurSlot + 1][iBtn]->SetPos( m_nPosX + nPosX, m_nPosY + 3);
					}
				}
			}
			
			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();
			
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInBtn && 
				(pMsg->wParam & MK_LBUTTON) && (ndX != 0 || ndY != 0)&&  !* (m_bLock[m_nCurSlot]))
			{
				if( m_nSelSlotBtnID >= 0 )
				{
					ndX = abs( ndX );
					ndY = abs( ndY );
					
					static int nD = 7;
					
					CPrintF( "%d, %d", ndX, ndY );
					if( ndX > nD || ndY > nD )
					{
						int	nSelTab = m_nCurSlot + 1;
						int	nSelBtn = m_nSelSlotBtnID;
						
						pUIManager->SetHoldBtn( (*m_abtnItems)[nSelTab][nSelBtn] );

						bLButtonDownInBtn = FALSE;
					}
				}
				
			}
			
			// Move quick slot
			if( bDragAreaClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				Move( ndX, ndY );
				SetPosEXQSlot();
				return WMSG_SUCCESS;
			}
			else if( m_btnVertical.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
			else if( m_btnHorizontal.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
			
			if (*(m_bLock[m_nCurSlot]))
			{
				if( m_btnLock.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			else
			{
				if( m_btnUnLock.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			
			for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
			{
				if ((*m_abtnItems)[m_nCurSlot + 1][iBtn]->MouseMessage( pMsg ) != WMSG_FAIL)
					return WMSG_SUCCESS;
			}			
			
		}break;
	case WM_LBUTTONDOWN:
		{
			m_bIsInside = IsInside( nX, nY );

			m_nSelSlotBtnID = -1;

			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				
				nOldX = nX;		nOldY = nY;
				
				// DregArea
				if( IsInsideRect( nX, nY, m_rcDragArea ) )
				{
					bDragAreaClick = TRUE;
				}
				else if( m_btnVertical.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				else if( m_btnHorizontal.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				
				if (*(m_bLock[m_nCurSlot]))
					m_btnLock.MouseMessage( pMsg );
				else
					m_btnUnLock.MouseMessage( pMsg );
				
				for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
				{
					if( (*m_abtnItems)[m_nCurSlot + 1][iBtn]->MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Update selected button
						m_nSelSlotBtnID = iBtn;
						
						bLButtonDownInBtn = TRUE;
						
						pUIManager->RearrangeOrder( m_nID, TRUE );
						return WMSG_SUCCESS;
					}
				}
				
				pUIManager->RearrangeOrder( m_nID, TRUE );
				return WMSG_SUCCESS;
			}
		}break;
	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			
			bLButtonDownInBtn = FALSE;
			
			m_bIsInside = IsInside( nX, nY );
			if (m_bIsInside)
			{
				if (*(m_bRotation[m_nCurSlot]) == 0) // 0 세로 일 때 버튼 위치 설정 [12/21/2012 Ranma]
				{
					int nPosY = 16;
					for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nPosY += 35 )
					{
						(*m_abtnItems)[m_nCurSlot + 1][iBtn]->SetPos( m_nPosX + 3,m_nPosY + nPosY);
					}
				}
				else
				{
					int nPosX = 16;
					for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++, nPosX += 35 )
					{
						(*m_abtnItems)[m_nCurSlot + 1][iBtn]->SetPos( m_nPosX + nPosX, m_nPosY + 3);
					}
				}
			}
			
			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// If quick slot isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;
				
				bDragAreaClick = FALSE;
				if (*(m_bLock[m_nCurSlot]))
				{
					if( m_btnLock.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						*(m_bLock[m_nCurSlot]) = 0;
						return WMSG_SUCCESS;
					} 
				}
				else
				{
					if( m_btnUnLock.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						*(m_bLock[m_nCurSlot]) = 1;
						return WMSG_SUCCESS;
					}
				}	
				
				if( m_btnVertical.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					*(m_bRotation[m_nCurSlot]) = 1;
					Rotation();
					return WMSG_SUCCESS;
				}
				else if( m_btnHorizontal.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					*(m_bRotation[m_nCurSlot]) = 0;
					Rotation();
					return WMSG_SUCCESS;
				}
				else if( IsInsideRect( nX, nY, m_rcDragArea ) )
				{
					return WMSG_SUCCESS;
				}
				// Slot
				else if( IsInsideRect( nX, nY, m_rcSlot ) )
				{
					m_nSelSlotBtnID = -1;
					
					for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
					{
						if( ( wmsgResult = (*m_abtnItems)[m_nCurSlot + 1][iBtn]->MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
							pUIManager->GetQuickSlot()->UseQuickSlot( iBtn , m_nCurSlot + 1);
							return WMSG_SUCCESS;
						}
					}
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ))
				{
					pUIManager->GetMessenger()->SetDragging(false);
					
					CUIIcon* pDrag = pUIManager->GetDragIcon();								

					// If holding button comes from quick slot
					if (pDrag->GetWhichUI() == UI_QUICKSLOT)
					{
						if( IsInsideRect( nX, nY, m_rcSlot ) )
						{
							for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
							{
								if ((*m_abtnItems)[m_nCurSlot + 1][iBtn]->IsInside( nX, nY ) &&
									(*m_abtnItems)[m_nCurSlot + 1][iBtn]->GetQuickSlotID() != pDrag->GetQuickSlotID())
								{
									int nBtnID = pDrag->GetQuickSlotID();
									
									int	nPageIndex = nBtnID / QSLOT_BTN_COUNT;
									int	nSlotIndex = nBtnID % QSLOT_BTN_COUNT;

									if( !*(m_bLock[m_nCurSlot]) )
										pUIManager->GetQuickSlot()->SwapBtnsInQuickSlot( nPageIndex, nSlotIndex,  m_nCurSlot + 1, iBtn );
									break;
								}
							}
						}
					}
					else if ((pDrag->getBtnType() == UBET_SKILL &&
							 (pDrag->GetWhichUI() == UI_SKILL_NEW || pDrag->GetWhichUI() == UI_CHARACTERINFO)) ||
							(pDrag->getBtnType() == UBET_SKILL &&
							 pDrag->GetWhichUI() == UI_PETINFO) ||
							(pDrag->getBtnType() == UBET_ACTION &&
							 pDrag->GetWhichUI() == UI_CHARACTERINFO) ||
							(pDrag->getBtnType() == UBET_ITEM &&
							 pDrag->GetWhichUI() == UI_INVENTORY) ||
							(pDrag->getBtnType() == UBET_SKILL &&
							 pDrag->GetWhichUI() == UI_WILDPET_INFO) ||
							(pDrag->getBtnType() == UBET_SKILL &&
							 pDrag->GetWhichUI() == UI_GUILD))
					{
						if( IsInsideRect( nX, nY, m_rcSlot ) )
						{
							if (pDrag->getBtnType() == UBET_ITEM)
							{
								CItemData* pItemData = CItemData::getData(pDrag->getIndex());
								
								if( (pItemData->GetType() == CItemData::ITEM_ETC 					//Etc(x)
									// [070604: Su-won] 1차 펫 기능 개선
									// 생산품을 퀵슬롯에 넣을 수 있게...
									&&pItemData->GetSubType() !=CItemData::ITEM_ETC_PRODUCT
									) ||
									(pItemData->GetType() == CItemData::ITEM_ACCESSORY				// Accessory(x)
									// [070604: Su-won] 1차 펫 기능 개선
									// 펫을 퀵슬롯에 넣을 수 있게...
									&& pItemData->GetSubType() !=CItemData::ACCESSORY_PET
									&& pItemData->GetSubType() !=CItemData::ACCESSORY_WILDPET
									&& pItemData->GetSubType() != CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD
									) ||
									( pItemData->GetType() == CItemData::ITEM_BULLET &&				// Bullet - arrow(x)
									pItemData->GetSubType() == CItemData::ITEM_BULLET_ARROW )
									)
									
								{
									// Reset holding button
									pUIManager->ResetHoldBtn();
									
									return WMSG_SUCCESS;
								}
							}
							
							for( int iBtn = 0; iBtn < QSLOT_BTN_COUNT; iBtn++ )
							{
								if( (*m_abtnItems)[m_nCurSlot+1][iBtn]->IsInside( nX, nY ) )
								{
									if( !*(m_bLock[m_nCurSlot]) )
										pUIManager->GetQuickSlot()->AddBtnToQuickSlot( iBtn, m_nCurSlot + 1 );
									
									break;
								}
							}
						}
					}					
					// Reset holding button
					pUIManager->ResetHoldBtn();
					
					return WMSG_SUCCESS;
				}
			}
			
			
		}break;
		
	case WM_LBUTTONDBLCLK:
		{
			m_bIsInside = IsInside( nX, nY );
			if( IsInside( nX, nY ) )
			{
				return WMSG_SUCCESS;
			}
		}break;
	}
	return WMSG_FAIL;
}