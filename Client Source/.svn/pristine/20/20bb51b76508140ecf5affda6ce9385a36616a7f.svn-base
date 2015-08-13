#include "StdH.h"

// 헤더 정리. [12/1/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Interface/UIBingoBox.h>

const int ctnBgNum = 8;
const __int64 tmLimitHighlight = 200; // 0.2초

extern INDEX g_iCountry;

int BingoOfCase[ctnBgNum][3] =
{
	{0,1,2}, //0
	{3,4,5}, //1
	{6,7,8}, //2
	{0,3,6}, //3
	{1,4,7}, //4
	{2,5,8}, //5
	{0,4,8}, //6
	{2,4,6}  //7
};

//============================================================================================================
// Name : CUIBingoBox()
// Desc :
//============================================================================================================
CUIBingoBox::CUIBingoBox()
	: m_pIconTemp(NULL)
{
	m_nSelectItem = -1;
	m_nTempItemArray = -1;
	m_nTempItemIndex = -1;
	m_bSelectLock = FALSE;
	
	int i;

	for (i = 0; i < 9; ++i)
	{
		m_pIconInsectItem[i] = NULL;
	}

	for ( i=0; i<ctnBgNum; i++ )
	{
		m_BingoBtnInfo[i].bBingoEnable =FALSE;
		m_BingoBtnInfo[i].bHighlight = FALSE;
		m_BingoBtnInfo[i].tmBingoStart = 0;
	}

	m_BingoOfCase[0].vecBingo.push_back(0);	m_BingoOfCase[0].vecBingo.push_back(3);
	m_BingoOfCase[0].vecBingo.push_back(6); // 0 3 6

	m_BingoOfCase[1].vecBingo.push_back(0);	m_BingoOfCase[1].vecBingo.push_back(4); // 0 4

	m_BingoOfCase[2].vecBingo.push_back(0);	m_BingoOfCase[2].vecBingo.push_back(5);
	m_BingoOfCase[2].vecBingo.push_back(7); //0 5 7

	m_BingoOfCase[3].vecBingo.push_back(1); m_BingoOfCase[3].vecBingo.push_back(3); // 1 3

	m_BingoOfCase[4].vecBingo.push_back(1); m_BingoOfCase[4].vecBingo.push_back(4);
	m_BingoOfCase[4].vecBingo.push_back(6); m_BingoOfCase[4].vecBingo.push_back(7); // 1 4 6 7

	m_BingoOfCase[5].vecBingo.push_back(1); m_BingoOfCase[5].vecBingo.push_back(5); // 1 5

	m_BingoOfCase[6].vecBingo.push_back(2); m_BingoOfCase[6].vecBingo.push_back(3);
	m_BingoOfCase[6].vecBingo.push_back(7); // 2 3 7

	m_BingoOfCase[7].vecBingo.push_back(2); m_BingoOfCase[7].vecBingo.push_back(4); // 2 4

	m_BingoOfCase[8].vecBingo.push_back(2); m_BingoOfCase[8].vecBingo.push_back(5);
	m_BingoOfCase[8].vecBingo.push_back(6); // 2 5 6

	m_pIconTemp = new CUIIcon;
	m_pIconTemp->initialize();
}
//============================================================================================================
// Name : ~CUIBingoBox()
// Desc :
//============================================================================================================
CUIBingoBox::~CUIBingoBox()
{
	Destroy();

	int i;

	for (i = 0; i < 9; ++i)
	{
		SAFE_DELETE(m_pIconInsectItem[i]);
	}

	SAFE_DELETE(m_pIconTemp);
}

//============================================================================================================
// Name : Create()
// Desc :
//============================================================================================================
void CUIBingoBox::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_rcTitle.SetRect(0, 0, 143, 21);
	m_rcbtnItems.SetRect(20, 48, 123, 151);

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Shop.tex" ) );
	FLOAT fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtTitleL.SetUV(0, 0, 23, 21, fTexWidth, fTexHeight);
	m_rtTitleM.SetUV(36, 0, 117, 21, fTexWidth, fTexHeight);
	m_rtTitleR.SetUV(198, 0, 215, 21, fTexWidth, fTexHeight);

	m_rtBingoNumDescL.SetUV(22, 38, 31, 53, fTexWidth, fTexHeight);
	m_rtBingoNumDescM.SetUV(32, 38, 61, 53, fTexWidth, fTexHeight);
	m_rtBingoNumDescR.SetUV(92, 38, 101, 53, fTexWidth, fTexHeight);

	m_rtItemSlotL.SetUV(0, 136, 9, 239, fTexWidth, fTexHeight);
	m_rtItemSlotM.SetUV(10, 136, 113, 239, fTexWidth, fTexHeight);
	m_rtItemSlotR.SetUV(206, 136, 215, 239, fTexWidth, fTexHeight);

	m_rtGapL.SetUV(0, 131, 9, 135, fTexWidth, fTexHeight);
	m_rtGapM.SetUV(10, 131, 113, 135, fTexWidth, fTexHeight);
	m_rtGapR.SetUV(206, 131, 215, 135, fTexWidth, fTexHeight);
	m_rtTempGap.SetUV(4, 136, 9, 274, fTexWidth, fTexHeight);

	m_rtBottomL.SetUV(0, 94, 9, 96, fTexWidth, fTexHeight);
	m_rtBottomM.SetUV(10, 94, 113, 96, fTexWidth, fTexHeight);
	m_rtBottomR.SetUV(206, 94, 215, 96, fTexWidth, fTexHeight);

	m_rtBingoMark.SetUV(218, 163, 238, 183, fTexWidth, fTexHeight);

	m_rtInfoL.SetUV(0, 476, 2, 495, fTexWidth, fTexHeight);
	m_rtInfoM.SetUV(3, 476, 15, 495, fTexWidth, fTexHeight);
	m_rtInfoR.SetUV(16, 476, 19, 495, fTexWidth, fTexHeight);

	m_rtSelectOutline.SetUV(218, 86, 249,117, fTexWidth, fTexHeight);

	m_btnClose.Create(this, CTString( ""), 114, 4, 14, 14);
	m_btnClose.SetUV(UBS_IDLE, 217, 1, 230, 14, fTexWidth, fTexHeight);
	m_btnClose.SetUV(UBS_CLICK, 232, 1, 245, 14, fTexWidth, fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE, UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);

	int nRow, nCol;

	for ( nRow=0; nRow<3; nRow++ )
	for ( nCol=0; nCol<3; nCol++ )
	{
		//m_pIconInsectItem[nRow*3+nCol].Create(this, 21+35*nCol, 49+35*nRow, BTN_SIZE, BTN_SIZE, UI_BINGOBOX, UBET_ITEM);

		m_pIconInsectItem[nRow * 3 + nCol] = new CUIIcon();
		m_pIconInsectItem[nRow * 3 + nCol]->Create(this, 21 + 35 * nCol, 49 + 35 * nRow, BTN_SIZE, BTN_SIZE, UI_BINGOBOX, UBET_ITEM);
	}

	m_lbItemInfo.Create(this, 0, 0, 100, 100, _pUIFontTexMgr->GetLineHeight()+2, 6, 3, 1, FALSE);
	m_lbItemInfo.SetEnable(TRUE);
	m_lbItemInfo.SetVisible(FALSE);
}

//============================================================================================================
// Name : ResetPosition()
// Desc :
//============================================================================================================
void CUIBingoBox::ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
	SetPos((pixMaxI + pixMinI) / 2 - GetWidth(), (pixMaxJ + pixMinJ - GetHeight()) / 2);
}

//============================================================================================================
// Name : AdjustPosition()
// Desc :
//============================================================================================================
void CUIBingoBox::AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
	if ( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition(pixMinI, pixMinJ, pixMaxI, pixMaxJ);
}

//============================================================================================================
// Name : Init()
// Desc :
//============================================================================================================
void CUIBingoBox::Init()
{
	int i;

	for ( i=0; i<9; i++ )
	{
		m_pIconInsectItem[i]->clearIconData();
		m_BingoOfCase[i].bMultiply = FALSE;
	}

	for ( i=0; i<ctnBgNum; i++ )
	{
		m_BingoBtnInfo[i].bBingoEnable =FALSE;
		m_BingoBtnInfo[i].bHighlight = FALSE;
		m_BingoBtnInfo[i].tmBingoStart = 0;
	}

	m_bSelectLock = FALSE;
	m_nSelectItem = -1;
	m_nTab = -1;
	m_nInvenIdx = -1;

	m_nBingo = 0;
}

//============================================================================================================
// Name : OpenBingoBox()
// Desc :
//============================================================================================================
void CUIBingoBox::OpenBingoBox(int nTab, int inven_idx)
{
	if ( IsVisible() == TRUE )
		return;

	Init();
	
	m_nTab = nTab;
	m_nInvenIdx = inven_idx;

	UpDateItem();

	CUIManager::getSingleton()->RearrangeOrder(UI_BINGOBOX, TRUE);
}

//============================================================================================================
// Name : Render()
// Desc :
//============================================================================================================
void CUIBingoBox::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData(m_ptdBaseTexture);

	int nX, nY, nX1, nY1;

	nX1 = m_nPosX + 23;
	nY1 = m_nPosY + 22;

	pDrawPort->AddTexture(m_nPosX, m_nPosY, nX1, nY1,
		m_rtTitleL.U0, m_rtTitleL.V0, m_rtTitleL.U1, m_rtTitleL.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX1, m_nPosY, nX1+103, nY1,
		m_rtTitleM.U0, m_rtTitleM.V0, m_rtTitleM.U1, m_rtTitleM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(m_nPosX+126, m_nPosY, m_nPosX+143, nY1,
		m_rtTitleR.U0, m_rtTitleR.V0, m_rtTitleR.U1, m_rtTitleR.V1, 0xFFFFFFFF);

	nY = m_nPosY + 22;
	nY1 = nY + 26;
	nX1 = m_nPosX + 10;

	pDrawPort->AddTexture(m_nPosX, nY, nX1, nY1,
		m_rtGapL.U0, m_rtGapL.V0, m_rtGapL.U1, m_rtGapL.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX1, nY, nX1+124, nY1,
		m_rtGapM.U0, m_rtGapM.V0, m_rtGapM.U1, m_rtGapM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX1+124, nY, nX1+134, nY1,
		m_rtGapR.U0, m_rtGapR.V0, m_rtGapR.U1, m_rtGapR.V1, 0xFFFFFFFF);

	nY = m_nPosY + 48;
	nY1 = nY + 105;

	pDrawPort->AddTexture(m_nPosX, nY, nX1, nY1,
		m_rtItemSlotL.U0, m_rtItemSlotL.V0, m_rtItemSlotL.U1, m_rtItemSlotL.V1, 0xFFFFFFFF);

	pDrawPort->AddTexture(nX1, nY, nX1+10, nY1,
		m_rtTempGap.U0, m_rtTempGap.V0, m_rtTempGap.U1, m_rtTempGap.V1, 0xFFFFFFFF);

	pDrawPort->AddTexture(nX1+10, nY, nX1+114, nY1,
		m_rtItemSlotM.U0, m_rtItemSlotM.V0, m_rtItemSlotM.U1, m_rtItemSlotM.V1, 0xFFFFFFFF);

	pDrawPort->AddTexture(nX1+114, nY, nX1+124, nY1,
		m_rtTempGap.U0, m_rtTempGap.V0, m_rtTempGap.U1, m_rtTempGap.V1, 0xFFFFFFFF);

	pDrawPort->AddTexture(nX1+124, nY, nX1+134, nY1,
		m_rtItemSlotR.U0, m_rtItemSlotR.V0, m_rtItemSlotR.U1, m_rtItemSlotR.V1, 0xFFFFFFFF);

	nY = m_nPosY + 152;
	nY1 = nY + 5;
	
	pDrawPort->AddTexture(m_nPosX, nY, nX1, nY1,
		m_rtGapL.U0, m_rtGapL.V0, m_rtGapL.U1, m_rtGapL.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX1, nY, nX1+124, nY1,
		m_rtGapM.U0, m_rtGapM.V0, m_rtGapM.U1, m_rtGapM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX1+124, nY, nX1+134, nY1,
		m_rtGapR.U0, m_rtGapR.V0, m_rtGapR.U1, m_rtGapR.V1, 0xFFFFFFFF);

	nY = m_nPosY + 157;
	nY1 = nY + 3;

	pDrawPort->AddTexture(m_nPosX, nY, nX1, nY1,
		m_rtBottomL.U0, m_rtBottomL.V0, m_rtBottomL.U1, m_rtBottomL.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX1, nY, nX1+124, nY1,
		m_rtBottomM.U0, m_rtBottomM.V0, m_rtBottomM.U1, m_rtBottomM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX1+124, nY, nX1+134, nY1,
		m_rtBottomR.U0, m_rtBottomR.V0, m_rtBottomR.U1, m_rtBottomR.V1, 0xFFFFFFFF);

	nY = m_nPosY + 27;
	nX = m_nPosX + 40;
	nY1 = nY + 15;
	
	// 빙고 갯수
	pDrawPort->AddTexture(nX, nY, nX+9, nY1,
		m_rtBingoNumDescL.U0, m_rtBingoNumDescL.V0, m_rtBingoNumDescL.U1, m_rtBingoNumDescL.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX+9, nY, nX+54, nY1,
		m_rtBingoNumDescM.U0, m_rtBingoNumDescM.V0, m_rtBingoNumDescM.U1, m_rtBingoNumDescM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX+54, nY, nX+74, nY1,
		m_rtBingoNumDescR.U0, m_rtBingoNumDescR.V0, m_rtBingoNumDescR.U1, m_rtBingoNumDescR.V1, 0xFFFFFFFF);

	nY = m_nPosY + 24;
	nX = m_nPosX + 20;
	// 빙고 마크
	pDrawPort->AddTexture(nX, nY, nX+20, nY+20,
		m_rtBingoMark.U0, m_rtBingoMark.V0, m_rtBingoMark.U1, m_rtBingoMark.V1, 0xFFFFFFFF);

	m_btnClose.Render();

	// Render all elements;
	pDrawPort->FlushRenderingQueue();

	int i;

	for ( i=0; i<9; i++ )
	{
		if (m_pIconInsectItem[i]->IsEmpty()) continue;

		m_pIconInsectItem[i]->Render(pDrawPort);
	}

	if ( m_nSelectItem >= 0 )
	{
		int btnX, btnY;

		btnX = m_nPosX + m_pIconInsectItem[m_nSelectItem]->GetPosX();
		btnY = m_nPosY + m_pIconInsectItem[m_nSelectItem]->GetPosY();

		pDrawPort->AddTexture( btnX, btnY, btnX+BTN_SIZE, btnY+BTN_SIZE,
			m_rtSelectOutline.U0, m_rtSelectOutline.V0, m_rtSelectOutline.U1, m_rtSelectOutline.V1, 0xFFFFFFFF);
	}

	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

	pDrawPort->InitTextureData(m_ptdBaseTexture);

	__int64 tmTemp = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	for ( i=0; i<ctnBgNum; i++ )
	{
		if ( m_BingoBtnInfo[i].bHighlight && (tmTemp - m_BingoBtnInfo[i].tmBingoStart) <= tmLimitHighlight )
		{
//			m_pIconInsectItem[BingoOfCase[i][0]].RenderHighlight(0xFFFFFFFF);
//			m_pIconInsectItem[BingoOfCase[i][1]].RenderHighlight(0xFFFFFFFF);
//			m_pIconInsectItem[BingoOfCase[i][2]].RenderHighlight(0xFFFFFFFF);
		}
		else if ( m_BingoBtnInfo[i].bHighlight && (tmTemp - m_BingoBtnInfo[i].tmBingoStart) > tmLimitHighlight )
		{
			m_BingoBtnInfo[i].bBingoEnable = TRUE;
			m_BingoBtnInfo[i].bHighlight = FALSE;
			m_BingoBtnInfo[i].tmBingoStart = 0;
		}
		else if ( m_BingoBtnInfo[i].bBingoEnable )
		{
			if (!m_BingoOfCase[BingoOfCase[i][0]].bMultiply)
			{
//				m_pIconInsectItem[BingoOfCase[i][0]].RenderHighlight(0x808080FF, PBT_MULTIPLY);
				m_BingoOfCase[BingoOfCase[i][0]].bMultiply = TRUE;
			}

			if (!m_BingoOfCase[BingoOfCase[i][1]].bMultiply)
			{
//				m_pIconInsectItem[BingoOfCase[i][1]].RenderHighlight(0x808080FF, PBT_MULTIPLY);
				m_BingoOfCase[BingoOfCase[i][1]].bMultiply = TRUE;
			}

			if (!m_BingoOfCase[BingoOfCase[i][2]].bMultiply)
			{
//				m_pIconInsectItem[BingoOfCase[i][2]].RenderHighlight(0x808080FF, PBT_MULTIPLY);
				m_BingoOfCase[BingoOfCase[i][2]].bMultiply = TRUE;
			}
		}
	}

	// Render all elements;
	pDrawPort->FlushRenderingQueue();

	//일본 스트링으로 인한 자리 옮김
#if defined(G_JAPAN)
	pDrawPort->PutTextEx( m_strTitle, m_nPosX+15, m_nPosY+5, 0xFFFFFFFF ); 
#else
	pDrawPort->PutTextEx( m_strTitle, m_nPosX+30, m_nPosY+5, 0xFFFFFFFF ); 
#endif

	CTString strBingo;
	strBingo.PrintF( _S(3190, "점수:%d"), m_nBingo );

	pDrawPort->PutTextExCX( strBingo, m_nPosX+70, m_nPosY+28, 0x6BD2FFFF );

	// Flush all render text queue
	pDrawPort->EndTextEx();
	
	if ( m_lbItemInfo.IsVisible() )
		RenderInfo();

	for (i=0; i<9; i++)
	{
		m_BingoOfCase[i].bMultiply = FALSE;
	}
}

//============================================================================================================
// Name : RenderInfo()
// Desc :
//============================================================================================================
void CUIBingoBox::RenderInfo()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	int Left =m_lbItemInfo.GetAbsPosX();
	int Right =Left +m_lbItemInfo.GetWidth() + 10;
	int Top = m_lbItemInfo.GetAbsPosY();
	int Bottom = Top + 19;

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	pDrawPort->AddTexture( Left, Top,
										Left + 3, Bottom,
										m_rtInfoL.U0, m_rtInfoL.V0, m_rtInfoL.U1, m_rtInfoL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( Left + 3, Top,
										Right - 3, Bottom,
										m_rtInfoM.U0, m_rtInfoM.V0, m_rtInfoM.U1, m_rtInfoM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( Right - 3, Top,
										Right, Bottom,
										m_rtInfoR.U0, m_rtInfoR.V0, m_rtInfoR.U1, m_rtInfoR.V1,
										0xFFFFFFFF );

	pDrawPort->FlushRenderingQueue();
	
	m_lbItemInfo.Render();

	pDrawPort->EndTextEx();
}

//============================================================================================================
// Name : MouseMessage()
// Desc :
//============================================================================================================
WMSG_RESULT CUIBingoBox::MouseMessage(MSG *pMsg)
{
	WMSG_RESULT wmsg_Result;

	static BOOL bTitleBarClick = FALSE;
	static BOOL bLButtonDownInItem = FALSE;

	// Mouse Point
	static int nOldX, nOldY;
	int nX = LOWORD(pMsg->lParam);
	int nY = HIWORD(pMsg->lParam);
	int i;

	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			if (IsInside(nX, nY) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			int ndX = nX - nOldX;
			int ndY = nY - nOldY;

			if ( bTitleBarClick && (pMsg->wParam & MK_LBUTTON) )
			{
				nOldX = nX; nOldY = nY;
				Move(ndX, ndY);
				return WMSG_SUCCESS;
			}
			else if ( m_btnClose.MouseMessage(pMsg) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( IsInsideRect(nX, nY, m_rcbtnItems) )
			{
				for ( i=0; i<9; i++ )
				{
					if ( m_pIconInsectItem[i]->IsInside(nX, nY) )
					{
						if ( !m_bSelectLock ) 
						{ m_nSelectItem = i; }
						
						if (m_pIconInsectItem[i]->IsEmpty() == false)
						{
							m_lbItemInfo.ResetAllStrings();
							//m_lbItemInfo.AddString(0, m_pIconInsectItem[i]->.GetCashName(), 0xF2F2F2FF, FALSE);
							
							// [100204: selo] 아이템 이름 길이에 따라 Width 를 바꾼다
// 							INDEX width = 19 - _pUIFontTexMgr->GetFontSpacing() + m_pIconInsectItem[i].GetCashName().Length() *
// 								( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );							 					
// 							m_lbItemInfo.SetWidth(width);

// 							m_lbItemInfo.SetPos(m_pIconInsectItem[i]->GetPosX(),
// 							m_pIconInsectItem[i].GetPosY() - m_lbItemInfo.GetCurItemCount(0)*m_lbItemInfo.GetLineHeight() - 5);
// 						
// 							m_lbItemInfo.SetVisible(TRUE);
						}

						return WMSG_SUCCESS;
					}
				}
	
				m_lbItemInfo.SetVisible(FALSE);
				return WMSG_SUCCESS;
			}
			else
			{
				m_nSelectItem = -1;
				m_lbItemInfo.SetVisible(FALSE);
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if ( IsInside(nX, nY) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				nOldX = nX; nOldY = nY;

				if ( m_btnClose.MouseMessage(pMsg) != WMSG_FAIL )
				{

				}
				else if ( IsInsideRect(nX, nY, m_rcTitle) )
				{
					bTitleBarClick = TRUE;
				}
				else if ( IsInsideRect(nX, nY, m_rcbtnItems) )
				{
					for ( i=0; i<9; i++ )
					{
						if ( m_pIconInsectItem[i]->IsInside(nX, nY) )
						{
							bLButtonDownInItem = TRUE;
							pUIManager->RearrangeOrder(UI_BINGOBOX, TRUE);

							return WMSG_SUCCESS;
						}
					}
				}

				pUIManager->RearrangeOrder(UI_BINGOBOX, TRUE);
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			bLButtonDownInItem = FALSE;

			if (pUIManager->GetDragIcon() == NULL)
			{
				bTitleBarClick = FALSE;

				if ( !IsFocused() ) return WMSG_FAIL;

				if ( ( wmsg_Result = m_btnClose.MouseMessage(pMsg) ) != WMSG_FAIL && !m_bSelectLock )
				{
					if ( wmsg_Result == WMSG_COMMAND )
						pUIManager->RearrangeOrder(UI_BINGOBOX, FALSE);

					return WMSG_SUCCESS;
				}
				else if (IsInsideRect(nX, nY, m_rcbtnItems))
				{
					if (m_nSelectItem >= 0)
					{
						if (m_pIconInsectItem[m_nSelectItem]->MouseMessage( pMsg ) != WMSG_FAIL)
						{
							return WMSG_SUCCESS;
						}
					}
				}
			}
			else
			{
				if ( IsInsideRect(nX, nY, m_rcbtnItems) )
				{
					for ( i=0; i<9; i++ )
					{
						if (m_pIconInsectItem[i]->IsInside(nX, nY) && m_pIconInsectItem[i]->IsEmpty())
						{
							if ( pUIManager->GetDragIcon()->getBtnType() == UBET_ITEM &&
								pUIManager->GetDragIcon()->GetWhichUI() == UI_INVENTORY )
							{
								m_pIconTemp->setData(UBET_ITEM, pUIManager->GetDragIcon()->getIndex());
								m_nTempItemArray = m_nSelectItem;
								m_nSelectItem = i;
								m_bSelectLock = TRUE;

								CItemData* pItemData = _pNetwork->GetItemData(m_pIconTemp->getIndex());
								//m_pIconTemp.SetTextureID(pItemData->GetIconTexID());

								pUIManager->CloseMessageBox(MSGCMD_SAVE_BINGOITEM);
								CUIMsgBox_Info MsgBoxInfo;

								MsgBoxInfo.SetMsgBoxInfo(_S( 191, "확인"), UMBS_OKCANCEL, UI_BINGOBOX,	MSGCMD_SAVE_BINGOITEM);

								if (IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
									MsgBoxInfo.AddString(_S(3986, "복주머니를 한번 넣으면 위치를 바꿀수 없습니다. 정말로 복주머니를 상자에 넣겠습니까?"));
								else
									MsgBoxInfo.AddString(_S(3191, "초코렛을 한번 넣으면 위치를 바꿀수 없습니다. 정말로 초코렛을 상자에 넣겠습니까?"));

								pUIManager->CreateMessageBox(MsgBoxInfo);

								pUIManager->ResetHoldBtn();
								return WMSG_SUCCESS;
							}
						}
					}
				}
				else
				{
					if (pUIManager->GetDragIcon()->getBtnType() == UBET_ITEM &&
						pUIManager->GetDragIcon()->GetWhichUI() == UI_BINGOBOX)
					{
						pUIManager->ResetHoldBtn();
						return WMSG_SUCCESS;
					}
				}
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			return WMSG_SUCCESS;	
		}
		break;
	}

	return WMSG_FAIL;
}

//============================================================================================================
// Name : MsgBoxCommand()
// Desc :
//============================================================================================================
void CUIBingoBox::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput)
{
	switch ( nCommandCode )
	{
	case MSGCMD_SAVE_BINGOITEM:
		{
			m_bSelectLock = FALSE;

			if ( bOK )
			{
				_pNetwork->SendSaveBingoItem(m_nTempItemArray, m_pIconTemp->getIndex(), m_nTempItemIndex);
			}
			else
			{
				m_pIconTemp->clearIconData();
				m_nTempItemArray = -1;
			}
		}
		break;
	}
}

//============================================================================================================
// Name : SetBtnItem()
// Desc :
//============================================================================================================
void CUIBingoBox::SetBtnItem(int num, int nIndex)
{
	int nItemIndex; 

	if (IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
		nItemIndex = 2634;
	else
		nItemIndex = 2048;

	if ( num == 0 ) return;

	switch( num )
	{
	case 1:
		break;
	case 2:
		{
			nItemIndex += 1; // 2049 or 2644
		}
		break;
	case 4:
		{
			nItemIndex += 2; // 2050 or 2645
		}
		break;
	default :
		return;
	}

	CItemData* pItemData = _pNetwork->GetItemData(nItemIndex);

	m_pIconInsectItem[nIndex]->setData(UBET_ITEM, nItemIndex);
}

//============================================================================================================
// Name : UpDateFindBingo()
// Desc :
//============================================================================================================
void CUIBingoBox::FindBingo(int num, BOOL bAllSearch/*=FALSE*/)
{
	int i;

	if ( bAllSearch )
	{
		for ( i=0; i<ctnBgNum; i++ )
		{
			if ( m_pIconInsectItem[BingoOfCase[i][0]]->getIndex() == m_pIconInsectItem[BingoOfCase[i][1]]->getIndex() &&
				m_pIconInsectItem[BingoOfCase[i][0]]->getIndex() == m_pIconInsectItem[BingoOfCase[i][2]]->getIndex() )
			{ // 빙고
				if ( m_pIconInsectItem[BingoOfCase[i][0]]->getIndex() < 0 ) continue;

				m_BingoBtnInfo[i].bBingoEnable = TRUE;
			}
		}
	}
	else
	{
		if ( num < 0 ) return;

		__int64 tmTemp = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

		for (i=0; i<m_BingoOfCase[num].vecBingo.size(); i++)
		{
			int Temp0, Temp1, Temp2, Temp3;

			Temp3 = m_BingoOfCase[num].vecBingo[i];

			if (m_BingoBtnInfo[Temp3].bBingoEnable)	continue;

			Temp0 = BingoOfCase[Temp3][0];
			Temp1 = BingoOfCase[Temp3][1];
			Temp2 = BingoOfCase[Temp3][2];

		if (m_pIconInsectItem[Temp0]->getIndex() == m_pIconInsectItem[Temp1]->getIndex() &&
				m_pIconInsectItem[Temp0]->getIndex() == m_pIconInsectItem[Temp2]->getIndex() )
			{
				m_BingoBtnInfo[Temp3].bHighlight = TRUE;
				m_BingoBtnInfo[Temp3].tmBingoStart = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			}
		}
	}
}

//============================================================================================================
// Name : UpDateItem()
// Desc :
//============================================================================================================
void CUIBingoBox::UpDateItem(void)
{
	CItems &rItems = _pNetwork->MySlotItem[m_nTab][m_nInvenIdx];

	m_strTitle = rItems.ItemData->GetName();
	m_nTempItemIndex = rItems.ItemData->GetItemIndex();

	ULONG ulTemp;
	ULONG ulBit = rItems.Item_Plus;

	int i;

	for( i=0; i<9; i++ )
	{
		ulTemp = ((ulBit >> i*3) & 0x00000007);
		SetBtnItem(ulTemp, i);
	}

	m_nBingo = rItems.Item_Used;
	
	FindBingo(0, TRUE);
}

//=============================================================================================================
// Name : ErrorMessage()
// Desc :
//=============================================================================================================
void CUIBingoBox::ErrorMessage(CNetworkMessage *istr)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	ULONG ulErrorType;
	ULONG ulTemp;
	CUIMsgBox_Info MsgBoxInfo;
	CTString strMessage;

	MsgBoxInfo.SetMsgBoxInfo(_S( 191, "확인"), UMBS_OK, UI_NONE,	MSGCMD_NULL);

	(*istr) >> ulErrorType;

	switch ( ulErrorType )
	{
	case MSG_EVENT_VALENTINE_2007_PACKAGE_OK : // 초코렛 아이템 포장 성공
		{
			if(IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				strMessage = _S(3987, "복주머니가 노끈으로 묶였습니다. 묶인 복주머니는 소망상자의 수집 이 외의 용도로 사용할 수 없습니다.");
			else
				strMessage = _S(3192, "초코렛이 포장되었습니다. 포장된 초코렛은 초코상자의 수집 이외의 용도로 사용할 수 없습니다.");
		}
		break;
	case MSG_EVENT_VALENTINE_2007_PACKAGE_FULLINVEN : // 인벤이 가득 참
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S(116, "인벤토리 공간이 부족하여 보상을 받을 수 없습니다."), SYSMSG_ERROR );
			return;
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_OK : // 포장된 초코렛 저장 성공
		{
			(*istr) >> ulTemp;
			
			m_nBingo += ulTemp;
//			m_pIconInsectItem[m_nTempItemArray].Copy(m_pIconTemp);
			m_pIconInsectItem[m_nTempItemArray]->setData(UBET_ITEM, m_pIconTemp->getIndex());

			FindBingo(m_nTempItemArray);
			
			if(IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				strMessage = _S(3988, "묶인 복주머니가 새해 소망상자에 수집되었습니다.");
			else
				strMessage = _S(3193, "초코렛이 초코수집상자에 수집되었습니다.");

			pUIManager->GetChattingUI()->AddSysMessage(strMessage, SYSMSG_ERROR);
			return;
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_NOTPACKAGE : // 포장된 초코렛이 아님
		{
			if(IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				strMessage = _S(3989, "묵인 복주머니가 아닙니다.");
			else
				strMessage = _S(3194, "포장된 초코렛이 아닙니다.");

			pUIManager->GetChattingUI()->AddSysMessage(strMessage, SYSMSG_ERROR);
			return;
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_ALREADY : // 이미 저장된 자리
		{
			if(IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				strMessage = _S(3990, "이미 묶인 복주머니가 존재합니다.");
			else
				strMessage = _S(3195, "이미 포장된 초코렛이 존재합니다.");

			pUIManager->GetChattingUI()->AddSysMessage(strMessage, SYSMSG_ERROR);
			return;
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_GIFT_OK : // 상품 보상 성공
		{
			(*istr) >> ulTemp;
			
			if (IsVisible())
			{
				pUIManager->RearrangeOrder(UI_BINGOBOX, FALSE);
			}

			strMessage.PrintF(_S(3196, "축하합니다. %d 개의 빙고가 되었습니다.(점수:%d)"), ulTemp, m_nBingo);
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_GIFT_NOITEM : // 빙고가 하나도 없음
		{
			if(IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				strMessage = _S(3991, "소망상자에 빙고가 하나도 없습니다. 확인 후 다시 도전하세요");
			else
				strMessage = _S(3197, "초코상자에 빙고가 하나도 없습니다. 확인 후 다시 도전하세요");
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_ITEM_NOTFOUND : // 초코상자가 없음
		{
			if(IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				strMessage = _S(3992, "소망상자가 존재하지 않습니다.");
			else
				strMessage = _S(3198, "초코상자가 존재하지 않습니다.");
		}
		break;
	}

	MsgBoxInfo.AddString(strMessage);
	pUIManager->CreateMessageBox(MsgBoxInfo);
}

