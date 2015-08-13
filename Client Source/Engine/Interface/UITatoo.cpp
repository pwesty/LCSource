#include "stdh.h"

// 헤더 정리. [12/3/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UITatoo.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Info/MyInfo.h>

const int ct_iColorShapeIndex[9][5] = { {26, 27, 28, 29, 30}, {31, 32, 33, 34, 35},
										{13, 22, 23, 24, 25}, {36, 37, 38, 39, 40},
										{41, 42, 43, 44, 45}, {46, 47, 48, 49, 50},	
										{51, 52, 53, 54, 55}, {11, 14, 15, 16, 17},
										{12, 18, 19, 20, 21} };


//------------------------------------------------------------------------------
// CUITatoo::CUITatoo()
// Explain:  
//------------------------------------------------------------------------------
CUITatoo::CUITatoo() : m_iSelectedColorIndex(-1), m_iSelectedShapeIndex(-1)
{
	m_ptdCommonBtnTexture = NULL;
}


//------------------------------------------------------------------------------
// CUITatoo::~CUITatoo()
// Explain:  
//------------------------------------------------------------------------------
CUITatoo::~CUITatoo()
{
	Clear ();

	Destroy();

	STOCK_RELEASE(m_ptdCommonBtnTexture);
	STOCK_RELEASE(m_ptdNewInventoryTexture);
}

//------------------------------------------------------------------------------
// CUITatoo::Clear
// Explain:  
//------------------------------------------------------------------------------
void CUITatoo::Clear()
{	
	m_iSelectedColorIndex = -1;
	m_iSelectedShapeIndex = -1;
}

//------------------------------------------------------------------------------
// CUITatoo::Create
// Explain:  
//------------------------------------------------------------------------------
void CUITatoo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_rcTitle.SetRect( 0, 0, TATOO_WIDTH, 22 );	

	m_ptdBaseTexture = CreateTexture( "Data\\Interface\\QuestBook.tex" );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtMain.SetUV(254, 609, 254+TATOO_WIDTH, 609+TATOO_HEIGHT, fTexWidth, fTexHeight);

	// Color buttons
	{
		AddColorButton(0, 0, 0xFF0000FF);	// 빨강
		AddColorButton(0, 1, 0xFFA500FF);	// 주황
		AddColorButton(1, 0, 0xFFFF00FF);	// 노랑
		AddColorButton(1, 1, 0x00FF00FF);   // 초록
		AddColorButton(2, 0, 0x0000FFFF);	// 파랑
		AddColorButton(2, 1, 0x000080FF);	// 남색
		AddColorButton(3, 0, 0x800080FF);	// 보라
		AddColorButton(3, 1, 0x000000FF);	// 검정
		AddColorButton(4, 0, 0xFFFFFFFF);	// 하양
	}

	// Shape buttons
	{
		AddShapeButton(0, 0, 287, 967);		// 하트
		AddShapeButton(0, 1, 319, 967);		// 클로버
		AddShapeButton(1, 0, 351, 967);		// 다이아
		AddShapeButton(1, 1, 383, 967);		// 스페이드
		AddShapeButton(2, 0, 255, 967);		// 해골
	}

	m_ptdNewInventoryTexture = CreateTexture(CTString("Data\\Interface\\NewInventory.tex"));
	m_ptdCommonBtnTexture = CreateTexture( "Data\\Interface\\CommonBtn.tex" );
	fTexWidth = m_ptdCommonBtnTexture->GetPixWidth();
	fTexHeight = m_ptdCommonBtnTexture->GetPixHeight();

	// OK button
	m_btnOK.Create( this, _S( 191, "확인" ), TATOO_WIDTH - 180, TATOO_HEIGHT - 30, 78, 22 );
	m_btnOK.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// cancel button
	m_btnCancel.Create( this, _S( 139, "취소" ), TATOO_WIDTH - 90, TATOO_HEIGHT - 30, 78, 22 );
	m_btnCancel.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// close button
	m_btnClose.Create( this, CTString( "" ), TATOO_WIDTH - 24, 4, 16, 16 );
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 227, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 229, 33, 245, 49, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );	
}


//------------------------------------------------------------------------------
// CUITatoo::Close
// Explain:  
//------------------------------------------------------------------------------
void CUITatoo::Close()
{
	if( IsVisible() )
	{
		CUIManager::getSingleton()->RearrangeOrder( UI_TATOO, FALSE );
		Clear();
	}
}

//------------------------------------------------------------------------------
// CUITatoo::SetFocus
// Explain:  
//------------------------------------------------------------------------------
void CUITatoo::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );
}

//------------------------------------------------------------------------------
// CUITatoo::Render
// Explain:  
//------------------------------------------------------------------------------
void CUITatoo::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + TATOO_WIDTH, m_nPosY + TATOO_HEIGHT,
										m_rtMain.U0, m_rtMain.V0, m_rtMain.U1, m_rtMain.V1,
										0xFFFFFFFF );
	// Render color and shape buttons
	RenderColorButtons();
	RenderShapeButtons();

	pDrawPort->FlushRenderingQueue();


	// render selected outline
	pDrawPort->InitTextureData(m_ptdNewInventoryTexture);

	RenderSelectedColorOutline();
	RenderSelectedShapeOutline();

	pDrawPort->FlushRenderingQueue();

	// render general buttons
	pDrawPort->InitTextureData( m_ptdCommonBtnTexture );	
	
	m_btnOK.Render();
	m_btnClose.Render();
	m_btnCancel.Render();

	pDrawPort->FlushRenderingQueue();

	// 타이틀 뿌린다
	pDrawPort->PutTextExCX( _S(4430, "문양 변경"), m_nPosX + 128, m_nPosY + 15, 0xDED9A0FF );
	// 색상 선택 (69, 47)
	pDrawPort->PutTextExCX( _S(4431, "색상 선택"), m_nPosX + 69, m_nPosY + 47, 0xDED9A0FF );
	// 문양 선택 (187, 47)
	pDrawPort->PutTextExCX( _S(4432, "문양 선택"), m_nPosX + 187, m_nPosY + 47, 0xDED9A0FF );

	pDrawPort->EndTextEx();
}

//==============================================================================
//  [7/10/2009 selo]
// Name : RenderColorButtons
// Desc : 색상 버튼들을 랜더한다
//==============================================================================
void CUITatoo::RenderColorButtons()
{
	int nCnt = 0;
	for( int i = 0; i < TATOO_TOTAL_ROW; ++i )
	{
		for( int j = 0; j < TATOO_TOTAL_COL; ++j )
		{
			if( nCnt > 8 )
				return;

			m_btnColor[i][j].Render();

			++nCnt;
		}
	}
}

//==============================================================================
//  [7/10/2009 selo]
// Name : RenderShapeButtons
// Desc : 문양 버튼들을 랜더한다
//==============================================================================
void CUITatoo::RenderShapeButtons()
{
	int nCnt = 0;
	for( int i = 0; i < TATOO_TOTAL_ROW; ++i )
	{
		for( int j = 0; j < TATOO_TOTAL_COL; ++j )
		{
			if( nCnt > 4 )
				return;

			if( m_btnShape[i][j].btnButton.IsEnabled() )
				m_btnShape[i][j].Render();

			++nCnt;
		}
	}
}

//
//
//
void CUITatoo::RenderSelectedColorOutline()
{
	if( 0 > m_iSelectedColorIndex || 8 < m_iSelectedColorIndex )
		return;

	int iX = 0;
	int iY = 0;

	m_btnColor[0][m_iSelectedColorIndex].btnButton.GetPos(iX, iY);

	FLOAT	fTexWidth = m_ptdNewInventoryTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdNewInventoryTexture->GetPixHeight();
	UIRectUV rtSelect(3,441,36,474,fTexWidth,fTexHeight);

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	pDrawPort->AddTexture( m_nPosX + iX, m_nPosY + iY, m_nPosX + iX+32, m_nPosY + iY+32, rtSelect.U0, rtSelect.V0, rtSelect.U1, rtSelect.V1, 0xFFFFFFFF);
}

//
//
//
void CUITatoo::RenderSelectedShapeOutline()
{
	if( 0 > m_iSelectedShapeIndex || 4 < m_iSelectedShapeIndex )
		return;

	int iX = 0;
	int iY = 0;

	m_btnShape[0][m_iSelectedShapeIndex].btnButton.GetPos(iX, iY);

	FLOAT	fTexWidth = m_ptdNewInventoryTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdNewInventoryTexture->GetPixHeight();
	UIRectUV rtSelect(3,441,36,474,fTexWidth,fTexHeight);

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	pDrawPort->AddTexture( m_nPosX + iX, m_nPosY + iY, m_nPosX + iX+32, m_nPosY + iY+32, rtSelect.U0, rtSelect.V0, rtSelect.U1, rtSelect.V1, 0xFFFFFFFF);
}

//------------------------------------------------------------------------------
// CUITatoo::OpenTatoo
// Explain:  
//------------------------------------------------------------------------------
void CUITatoo::OpenTatoo()
{
	if( IsVisible() )
		return;
	CUIManager::getSingleton()->RearrangeOrder( UI_TATOO, TRUE );

	m_iSelectedColorIndex = 0;
	CheckShowShape();
}

//------------------------------------------------------------------------------
// CUITatoo::ResetPosition
// Explain:  
//------------------------------------------------------------------------------
void CUITatoo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}


//------------------------------------------------------------------------------
// CUITatoo::AdjustPosition
// Explain:  
//------------------------------------------------------------------------------
void CUITatoo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//==============================================================================
//  [7/10/2009 selo]
// Name : AddColorButton
// Desc : 색상 버튼을 추가한다
//==============================================================================
void CUITatoo::AddColorButton(int iRow, int iCol, COLOR color)
{
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_btnColor[iRow][iCol].color = color;
	m_btnColor[iRow][iCol].exist = true;
	CUIButton& btnColor = m_btnColor[iRow][iCol].btnButton;
	
	// 0row 0col 좌표 (25, 72)
	// 0row 1col 좌표 (64, 72)	
	int iX = 25 + (iCol * 39);
	int iY = 72 + (iRow * 40);

	btnColor.Create(this, CTString(""), iX, iY, 32, 32);
	btnColor.SetUV( UBS_IDLE, 415, 967, 415+32, 967+32, fTexWidth, fTexHeight);
	btnColor.CopyUV( UBS_IDLE, UBS_CLICK );
	btnColor.CopyUV( UBS_IDLE, UBS_ON );
	btnColor.CopyUV( UBS_IDLE, UBS_DISABLE );
}

//==============================================================================
//  [7/10/2009 selo]
// Name : AddShapeButton
// Desc : 문양 버튼을 추가한다
//==============================================================================
void CUITatoo::AddShapeButton(int iRow, int iCol, int texLeft, int texTop)
{
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_btnShape[iRow][iCol].color = 0xFFFFFFFF;
	m_btnShape[iRow][iCol].exist = true;
	CUIButton& btnShape = m_btnShape[iRow][iCol].btnButton;

	// 0row 0col 좌표 (146, 72)
	int iX = 146 + (iCol * 39);
	int iY = 72 + (iRow * 40);

	btnShape.Create(this, CTString(""), iX, iY, 32, 32);
	btnShape.SetUV( UBS_IDLE, texLeft, texTop, texLeft+32, texTop+32, fTexWidth, fTexHeight);
	btnShape.CopyUV( UBS_IDLE, UBS_CLICK );
	btnShape.CopyUV( UBS_IDLE, UBS_ON );
	btnShape.CopyUV( UBS_IDLE, UBS_DISABLE );
}

//
//
//
void CUITatoo::CheckShowShape()
{
	int iSel = m_iSelectedColorIndex;
	BOOL bEnable = TRUE;

	ObjInfo* pInfo = ObjInfo::getSingleton();
	CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();

	int iType = pPetInfo->iType % 2;
	int iAge = pPetInfo->iAge;

	// [090831: selo] 타는 것 아니면 문양 선택 못하게 변경

	if( iAge < 2 )		// 타는거 아닐 때
	{
		bEnable = FALSE;
	}	
	else if( iAge >= 2 ) // 타는거 일 때
	{
		bEnable = TRUE;
	}

	bool bBreak = false;
	for( int i = 0; i < TATOO_TOTAL_ROW; ++i )
	{
		for( int j = 0; j < TATOO_TOTAL_COL; ++j )
		{
			if( false == m_btnShape[i][j].exist )
			{
				bBreak = true;
				break;
			}
			m_btnShape[i][j].btnButton.SetEnable(bEnable);
		}
		if( bBreak )
			break;
	}
}

//
//
//
void CUITatoo::SendItemUse()
{
	int tv_tab, tv_idx;
	CUIManager::getSingleton()->GetInventory()->GetUseItemSlotInfo(tv_tab,tv_idx);
	CItems& Items = _pNetwork->MySlotItem[tv_tab][tv_idx];
	int iItemIndex = Items.Item_UniIndex;
	_pNetwork->SendItemUse(tv_tab, tv_idx, iItemIndex, GetColorAndShapeIndex());
}

//
//
//
int	CUITatoo::GetColorAndShapeIndex()
{
	if( 0 > m_iSelectedShapeIndex )
		return m_iSelectedColorIndex + 1;

	return ct_iColorShapeIndex[m_iSelectedColorIndex][m_iSelectedShapeIndex];	
}

//
//
//
void CUITatoo::MakeMessageBox()
{
	CTString strColor ="";
	CTString strShape =_S(4448, "민");

	switch(m_iSelectedColorIndex)
	{
	case 0:
		strColor = _S(4434, "빨강");
		break;
	case 1:
		strColor = _S(4435, "주황");
		break;
	case 2:
		strColor = _S(4436, "노랑");
		break;
	case 3:
		strColor = _S(4437, "초록");
		break;
	case 4:
		strColor = _S(4438, "파랑");
		break;
	case 5:
		strColor = _S(4439, "남색");
		break;
	case 6:
		strColor = _S(4440, "보라");
		break;
	case 7:
		strColor = _S(4441, "검정");
		break;
	case 8:
		strColor = _S(4442, "하양");
		break;
	}
	switch(m_iSelectedShapeIndex)
	{
	case 0:
		strShape = _S(4443, "하트");		
		break;
	case 1:
		strShape = _S(4445, "클로버");				
		break;
	case 2:		
		strShape = _S(4444, "다이아몬드");		
		break;
	case 3:
		strShape = _S(4446, "스페이드");		
		break;
	case 4:		
		strShape = _S(4447, "해골");
		break;
	}


	CUIMsgBox_Info	MsgBoxInfo;
	CTString		strMessage;
	strMessage.PrintF(_S(4433, "%s색상 %s문양을 선택하셨습니다. 이대로 진행하시겠습니까?"), strColor, strShape);
	MsgBoxInfo.SetMsgBoxInfo(_S(4430, "문양 변경"), UMBS_YESNO, UI_NONE, MSGCMD_PET_TATOOCHANGE_USE);
	MsgBoxInfo.AddString(strMessage);
	CUIManager::getSingleton()->CreateMessageBox(MsgBoxInfo);
}

//------------------------------------------------------------------------------
// CUITatoo::MouseMessage
// Explain:  
//------------------------------------------------------------------------------
WMSG_RESULT	CUITatoo::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL	bTitleBarClick = FALSE;	

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	CUIManager* pUIManager = CUIManager::getSingleton();
	
	if( pUIManager->DoesMessageBoxExist(MSGCMD_PET_TATOOCHANGE_USE) )
		return WMSG_SUCCESS;
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

			// If message box isn't focused
			if( !IsFocused() )
				return WMSG_FAIL;			

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

			for( int i = 0; i < TATOO_TOTAL_ROW; ++i )
			{
				for( int j = 0; j < TATOO_TOTAL_COL; ++j )
				{
					m_btnColor[i][j].btnButton.MouseMessage( pMsg );
					m_btnShape[i][j].btnButton.MouseMessage( pMsg );
				}					
			}
		}
		break;

		case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				// OK button
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}

				int nCnt = 0;
				for( int i = 0; i < TATOO_TOTAL_ROW; ++i )
				{
					for( int j = 0; j < TATOO_TOTAL_COL; ++j )
					{
						if( m_btnColor[i][j].exist )
						{
							if( m_btnColor[i][j].btnButton.MouseMessage( pMsg ) != WMSG_FAIL )
							{
								m_iSelectedColorIndex = nCnt;
								m_iSelectedShapeIndex = -1;

								CheckShowShape();
							}
						}
						if( m_btnShape[i][j].exist )
						{
							if( m_btnShape[i][j].btnButton.MouseMessage( pMsg ) != WMSG_FAIL )
							{
								m_iSelectedShapeIndex = nCnt;
							}
						}

						++nCnt;
					}					
				}

				pUIManager->RearrangeOrder( UI_TATOO, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

		case WM_LBUTTONUP:
		{
			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;
				
				// If message box isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// OK button
				if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						MakeMessageBox();					
					return WMSG_SUCCESS;
				}
				// Close button
				else if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						Close();					
					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						Close();
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}
