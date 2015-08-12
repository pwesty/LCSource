#include "StdH.h"

// 헤더 정리. [12/1/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <vector>
#include <Engine/Interface/UIFlowerTree.h>
#include <Engine/Ska/Render.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Contents/Login/UIServerSelect.h>

const int cFlowerTreeIndex = 342; // 꽃나무 npc Index

// 나무 메쉬
const CTString Event_Tree1 = CTString("Data\\Npc\\event_tree\\event_tree01.bm");// 1
const CTString Event_Tree2 = CTString("Data\\Npc\\event_tree\\event_tree02.bm");// 2
const CTString Event_Tree3 = CTString("Data\\Npc\\event_tree\\event_tree03.bm");// 3
const CTString Event_Tree3_1 = CTString("Data\\Npc\\event_tree\\event_tree03_1.bm"); // 4
const CTString Event_Tree4 = CTString("Data\\Npc\\event_tree\\event_tree04.bm"); // 5
const CTString Event_Tree5 = CTString("Data\\Npc\\event_tree\\event_tree05.bm"); // 6
const CTString Event_Tree6 = CTString("Data\\Npc\\event_tree\\event_tree06.bm"); // 7
const CTString Event_Tree6_1 = CTString("Data\\Npc\\event_tree\\event_tree06_1.bm"); // 8
const CTString Event_Tree6_2 = CTString("Data\\Npc\\event_tree\\event_tree06_2.bm"); // 9 
const CTString Event_Tree7 = CTString("Data\\Npc\\event_tree\\event_tree07.bm"); // 10
const CTString Event_Tree7_1 = CTString("Data\\Npc\\event_tree\\event_tree07_1.bm"); // 11
const CTString Event_Tree7_2 = CTString("Data\\Npc\\event_tree\\event_tree07_2.bm"); // 12
const CTString Event_Tree7_3 = CTString("Data\\Npc\\event_tree\\event_tree07_3.bm"); //13
// 나무 텍스처
const CTString Event_Tree1_Tex = CTString("Data\\Npc\\event_tree\\Texture\\event_fl01.tex");
const CTString Event_Tree2_Tex = CTString("Data\\Npc\\event_tree\\Texture\\event_fl02.tex");
const CTString Event_Tree3_Tex = CTString("Data\\Npc\\event_tree\\Texture\\event_fl03.tex");
const CTString Event_Tree3_1_Tex = CTString("Data\\Npc\\event_tree\\Texture\\event_leaf01.tex");
const CTString Event_Tree4_Tex = CTString("Data\\Npc\\event_tree\\Texture\\event_leaf01.tex");
const CTString Event_Tree5_Tex = CTString("Data\\Npc\\event_tree\\Texture\\event_leaf02.tex");
const CTString Event_Tree6_Tex = CTString("Data\\Npc\\event_tree\\Texture\\event_leaf03.tex");
const CTString Event_Tree6_1_Tex = CTString("Data\\Npc\\event_tree\\Texture\\event_leaf02.tex");
const CTString Event_Tree6_2_Tex = CTString("Data\\Npc\\event_tree\\Texture\\event_leaf03.tex");
const CTString Event_Tree7_Tex = CTString("Data\\Npc\\event_tree\\Texture\\event_leaf03.tex");
const CTString Event_Tree7_1_Tex = CTString("Data\\Npc\\event_tree\\Texture\\event_leaf02.tex");
const CTString Event_Tree7_2_Tex = CTString("Data\\Npc\\event_tree\\Texture\\event_leaf03.tex");
const CTString Event_Tree7_3_Tex = CTString("Data\\Npc\\event_tree\\Texture\\event_leaf03.tex");
// 나무 이펙트
const CTString Event_Tree8 = CTString("event_tree");
const CTString Event_Tree9_0 = CTString("event_tree_0");
const CTString Event_Tree9_1 = CTString("event_tree_1");
const CTString Event_Tree10_0 = CTString("event_tree_2");
const CTString Event_Tree10_1 = CTString("event_tree_3");

// X-MAS2007 트리 메쉬
const CTString Xmas_Tree1 = CTString("Data\\Npc\\Christmas_tree\\christmastree_2.bm"); // 2단계
const CTString Xmas_Tree2 = CTString("Data\\Npc\\Christmas_tree\\christmastree_deco.bm"); // 3단계
// X-MAS2007 트리 텍스처
const CTString Xmas_Tree1_Tex = CTString("Data\\Npc\\Christmas_tree\\Christmastree_tree.tex");
const CTString Xmas_Tree2_Tex = CTString("Data\\Npc\\Christmas_tree\\Christmastree_deco.tex");

// 가미고 10주년 이벤트 메쉬
const CTString Gamigo_10th_cake1 = CTString("Data\\Npc\\10th_cake\\10th_cake-3_1.bm"); // 2단계
const CTString Gamigo_10th_cake2 = CTString("Data\\Npc\\10th_cake\\10th_cake-3_2.bm"); // 3단계
// 가미고 10주년 이벤트 텍스처
const CTString Gamigo_10th_cake1_Tex = CTString("Data\\Npc\\10th_cake\\10th_cake01.tex");
const CTString Gamigo_10th_cake2_Tex = CTString("Data\\Npc\\10th_cake\\10th_cake01.tex");

const CTString Gamigo_10th_cake = CTString("10th_Cake");


/************************************************************************/
/* Name : CUIFlowerTree()                                               */
/************************************************************************/
CUIFlowerTree::CUIFlowerTree()
	: m_pMobFlowerTree(NULL)
{
	m_CurFlowerStep = OP_FLOWERSTEP0;
	
	m_FlowerMeshInfo[OP_FLOWERSTEP1].MeshBM = CTFileName(Event_Tree1);
	m_FlowerMeshInfo[OP_FLOWERSTEP1].MeshTex = CTFileName(Event_Tree1_Tex);

	m_FlowerMeshInfo[OP_FLOWERSTEP2].MeshBM = CTFileName(Event_Tree2);
	m_FlowerMeshInfo[OP_FLOWERSTEP2].MeshTex = CTFileName(Event_Tree2_Tex);

	m_FlowerMeshInfo[OP_FLOWERSTEP3].MeshBM = CTFileName(Event_Tree3);
	m_FlowerMeshInfo[OP_FLOWERSTEP3].MeshTex = CTFileName(Event_Tree3_Tex);

	m_FlowerMeshInfo[OP_FLOWERSTEP3+1].MeshBM = CTFileName(Event_Tree3_1);
	m_FlowerMeshInfo[OP_FLOWERSTEP3+1].MeshTex = CTFileName(Event_Tree3_1_Tex);

	m_FlowerMeshInfo[OP_FLOWERSTEP4].MeshBM = CTFileName(Event_Tree4);
	m_FlowerMeshInfo[OP_FLOWERSTEP4].MeshTex = CTFileName(Event_Tree4_Tex);

	m_FlowerMeshInfo[OP_FLOWERSTEP5].MeshBM = CTFileName(Event_Tree5);
	m_FlowerMeshInfo[OP_FLOWERSTEP5].MeshTex = CTFileName(Event_Tree5_Tex);

	m_FlowerMeshInfo[OP_FLOWERSTEP6].MeshBM = CTFileName(Event_Tree6);
	m_FlowerMeshInfo[OP_FLOWERSTEP6].MeshTex = CTFileName(Event_Tree6_Tex);

	m_FlowerMeshInfo[OP_FLOWERSTEP6+1].MeshBM = CTFileName(Event_Tree6_1);
	m_FlowerMeshInfo[OP_FLOWERSTEP6+1].MeshTex = CTFileName(Event_Tree6_1_Tex);

	m_FlowerMeshInfo[OP_FLOWERSTEP6+2].MeshBM = CTFileName(Event_Tree6_2);
	m_FlowerMeshInfo[OP_FLOWERSTEP6+2].MeshTex = CTFileName(Event_Tree6_2_Tex);

	m_FlowerMeshInfo[OP_FLOWERSTEP7].MeshBM = CTFileName(Event_Tree7);
	m_FlowerMeshInfo[OP_FLOWERSTEP7].MeshTex = CTFileName(Event_Tree7_Tex);

	m_FlowerMeshInfo[OP_FLOWERSTEP7+1].MeshBM = CTFileName(Event_Tree7_1);
	m_FlowerMeshInfo[OP_FLOWERSTEP7+1].MeshTex = CTFileName(Event_Tree7_1_Tex);

	m_FlowerMeshInfo[OP_FLOWERSTEP7+2].MeshBM = CTFileName(Event_Tree7_2);
	m_FlowerMeshInfo[OP_FLOWERSTEP7+2].MeshTex = CTFileName(Event_Tree7_2_Tex);

	m_FlowerMeshInfo[OP_FLOWERSTEP7+3].MeshBM = CTFileName(Event_Tree7_3);
	m_FlowerMeshInfo[OP_FLOWERSTEP7+3].MeshTex = CTFileName(Event_Tree7_3_Tex);

	m_pEG[0] = NULL;
	m_pEG[1] = NULL;
	m_pEG[2] = NULL;
	m_pEG[3] = NULL;
	m_pEG[4] = NULL;

	m_XMASTREEMeshInfo[0].MeshBM = CTFileName(Xmas_Tree1);
	m_XMASTREEMeshInfo[0].MeshTex = CTFileName(Xmas_Tree1_Tex);

	m_XMASTREEMeshInfo[1].MeshBM = CTFileName(Xmas_Tree2);
	m_XMASTREEMeshInfo[1].MeshTex = CTFileName(Xmas_Tree2_Tex);

	// [ldy1978220 2011/5/31] 가미고 10주년 케익
	m_GamigoCakeMeshInfo[0].MeshBM = CTFileName(Gamigo_10th_cake1);
	m_GamigoCakeMeshInfo[0].MeshTex = CTFileName(Gamigo_10th_cake1_Tex);

	m_GamigoCakeMeshInfo[1].MeshBM = CTFileName(Gamigo_10th_cake2);
	m_GamigoCakeMeshInfo[1].MeshTex = CTFileName(Gamigo_10th_cake2_Tex);
}

/************************************************************************/
/* Name : ~CUIFlowerTree()                                              */
/************************************************************************/
CUIFlowerTree::~CUIFlowerTree()
{
	ClearList();
}

/************************************************************************/
/* Name : Create()                                                      */
/************************************************************************/
void CUIFlowerTree::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_rcTitle.SetRect(0, 0, 216, 21);
	
	m_ptdBaseTexture = CreateTexture(CTString("Data\\Interface\\MessageBox.tex"));
	FLOAT fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtTitleL.SetUV(0, 0, 81, 21, fTexWidth, fTexHeight);
	m_rtTitleR.SetUV(134, 0, 215, 21, fTexWidth, fTexHeight);
	m_rtGapL.SetUV(0, 22, 81, 25, fTexWidth, fTexHeight);
	m_rtGapR.SetUV(134, 22, 215, 25, fTexWidth, fTexHeight);
	m_rtMiddleL.SetUV(0, 26, 81, 33, fTexWidth, fTexHeight);
	m_rtMiddleR.SetUV(134, 26, 215, 33, fTexWidth, fTexHeight);
	m_rtBtGapL.SetUV(0, 34, 81, 41, fTexWidth, fTexHeight);
	m_rtBtGapR.SetUV(134, 34, 215, 41, fTexWidth, fTexHeight);
	m_rtBottomL.SetUV(0, 42, 81, 44, fTexWidth, fTexHeight);
	m_rtBottomR.SetUV(134, 42, 215, 44, fTexWidth, fTexHeight);

	m_rtScoreL.SetUV(205, 46, 207, 58, fTexWidth, fTexHeight);
	m_rtScoreM.SetUV(208, 46, 214, 58, fTexWidth, fTexHeight);
	m_rtScoreR.SetUV(215, 46, 217, 58, fTexWidth, fTexHeight);

	m_btnClose.Create(this, CTString(""), nWidth-35, 4, 14, 14);
	m_btnClose.SetUV(UBS_IDLE, 219, 0, 232, 13, fTexWidth, fTexHeight);
	m_btnClose.SetUV(UBS_CLICK, 234, 0, 247, 13, fTexWidth, fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE, UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);
	
	m_btnOK.Create(this, _S(3485, "개화 돕기"), 7, 182, 63, 21);
	m_btnOK.SetUV(UBS_IDLE, 0, 46, 62, 66, fTexWidth, fTexHeight);
	m_btnOK.SetUV(UBS_CLICK, 66, 46, 128, 66, fTexWidth, fTexHeight);
	m_btnOK.CopyUV(UBS_IDLE, UBS_ON);
	m_btnOK.CopyUV(UBS_IDLE, UBS_DISABLE);

	m_btnRefresh.Create(this, _S(3449, "갱신"), 94, 182, 63, 21);
	m_btnRefresh.SetUV(UBS_IDLE, 0, 46, 62, 66, fTexWidth, fTexHeight);
	m_btnRefresh.SetUV(UBS_CLICK, 66, 46, 128, 66, fTexWidth, fTexHeight);
	m_btnRefresh.CopyUV(UBS_IDLE, UBS_ON);
	m_btnRefresh.CopyUV(UBS_IDLE, UBS_DISABLE);
	
}

/************************************************************************/
/* Name : Render()                                                      */
/************************************************************************/
void CUIFlowerTree::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData(m_ptdBaseTexture);

	int nY, nX1, nY1, nX2;

	nX1 = m_nPosX + (FLOWERTREE_WIDTH * 0.5);
	nX2 = nX1 + (FLOWERTREE_WIDTH * 0.5);
	nY1 = m_nPosY + 22;

	pDrawPort->AddTexture(m_nPosX, m_nPosY, nX1, nY1,
		m_rtTitleL.U0, m_rtTitleL.V0, m_rtTitleL.U1, m_rtTitleL.V1, 0xFFFFFFFF);

	pDrawPort->AddTexture(nX1, m_nPosY, nX2, nY1,
		m_rtTitleR.U0, m_rtTitleR.V0, m_rtTitleR.U1, m_rtTitleR.V1, 0xFFFFFFFF);

	nY = nY1;
	nY1 = nY + 4;

	pDrawPort->AddTexture(m_nPosX, nY, nX1, nY1,
		m_rtGapL.U0, m_rtGapL.V0, m_rtGapL.U1, m_rtGapL.V1, 0xFFFFFFFF);

	pDrawPort->AddTexture(nX1, nY, nX2, nY1,
		m_rtGapR.U0, m_rtGapR.V0, m_rtGapR.U1, m_rtGapR.V1, 0xFFFFFFFF);

	nY = nY1;
	nY1 = nY + 150;

	pDrawPort->AddTexture(m_nPosX, nY, nX1, nY1,
		m_rtMiddleL.U0, m_rtMiddleL.V0, m_rtMiddleL.U1, m_rtMiddleL.V1, 0xFFFFFFFF);

	pDrawPort->AddTexture(nX1, nY, nX2, nY1,
		m_rtMiddleR.U0, m_rtMiddleR.V0, m_rtMiddleR.U1, m_rtMiddleR.V1, 0xFFFFFFFF);

	nY = nY1;
	nY1 = nY + 34;

	pDrawPort->AddTexture(m_nPosX, nY, nX1, nY1,
		m_rtBtGapL.U0, m_rtBtGapL.V0, m_rtBtGapL.U1, m_rtBtGapL.V1, 0xFFFFFFFF);

	pDrawPort->AddTexture(nX1, nY, nX2, nY1,
		m_rtBtGapR.U0, m_rtBtGapR.V0, m_rtBtGapR.U1, m_rtBtGapR.V1, 0xFFFFFFFF);

	nY = nY1;
	nY1 = nY + 3;

	pDrawPort->AddTexture(m_nPosX, nY, nX1, nY1,
		m_rtBottomL.U0, m_rtBottomL.V0, m_rtBottomL.U1, m_rtBottomL.V1, 0xFFFFFFFF);

	pDrawPort->AddTexture(nX1, nY, nX2, nY1,
		m_rtBottomR.U0, m_rtBottomR.V0, m_rtBottomR.U1, m_rtBottomR.V1, 0xFFFFFFFF);

	std::vector<sServerScoreList>::iterator SvScItr;

	int i=0;
	int testY = m_nPosY + 30;
	int testX = m_nPosX - 10;

	if (!m_vecSvScoreList.empty())
	{
		for (SvScItr=m_vecSvScoreList.begin(); SvScItr!=m_vecSvScoreList.end(); SvScItr++)
		{
			pDrawPort->AddTexture(testX+101, testY, testX+104, testY+13,
				m_rtScoreL.U0, m_rtScoreL.V0, m_rtScoreL.U1, m_rtScoreL.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(testX+104, testY, testX+150, testY+13,
				m_rtScoreM.U0, m_rtScoreM.V0, m_rtScoreM.U1, m_rtScoreM.V1, 0xFFFFFFFF);
			pDrawPort->AddTexture(testX+150, testY, testX+153, testY+13,
				m_rtScoreR.U0, m_rtScoreR.V0, m_rtScoreR.U1, m_rtScoreR.V1, 0xFFFFFFFF);

			testY += 15;
		}
	}

	m_btnClose.Render();
	m_btnOK.Render();
	m_btnRefresh.Render();

	// Render all elements;
	pDrawPort->FlushRenderingQueue();
	// _pNetwork->m_iServerGroup
	CTString strScore;

	if (!m_vecSvScoreList.empty())
	{
		int nY = m_nPosY + 30;

		for (SvScItr=m_vecSvScoreList.begin(); SvScItr!=m_vecSvScoreList.end(); SvScItr++)
		{
			pDrawPort->PutTextEx( (*SvScItr).strServerName, m_nPosX+17, nY, 0xFFFFFFFF); // 서버 이름
			
			strScore.PrintF("%d", (*SvScItr).ulScore); 
			pDrawPort->PutTextExRX(strScore, testX+145, nY, 0xFFFFFFFF); // 서버 점수
			nY += 15;
		}
	}

	pDrawPort->PutTextEx( _S(3486, "접속 서버 :"), m_nPosX+17, m_nPosY+FLOWERTREE_HEIGHT-70, 0xFFFFFFFF);
	pDrawPort->PutTextExRX( pUIManager->GetServerSelect()->GetServerName(_pNetwork->m_iServerGroup), testX+155, m_nPosY+FLOWERTREE_HEIGHT-70, 0xFFFFFFFF);

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

/************************************************************************/
/* Name : OpenFlowerTree                                                */
/************************************************************************/
void CUIFlowerTree::OpenFlowerTree()
{
	if ( IsVisible() == TRUE )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->DoesMessageBoxExist(MSGCMD_FLOWERTREE_TICKET))
		pUIManager->CloseMessageBox(MSGCMD_FLOWERTREE_TICKET);

	if (pUIManager->DoesMessageBoxExist(MSGCMD_FLOWERTREE_TICKETEX))
		pUIManager->CloseMessageBox(MSGCMD_FLOWERTREE_TICKETEX);

	_pNetwork->SendFlowerTreeReq((UBYTE)MSG_EVENT_FLOWERTREE_2007_POINT_RANK);
	pUIManager->RearrangeOrder(UI_FLOWERTREE, TRUE);
}

/************************************************************************/
/* Name : CloseFlowerTree                                               */
/************************************************************************/
void CUIFlowerTree::CloseFlowerTree()
{
	if ( IsVisible() == FALSE )
		return;

	CUIManager::getSingleton()->RearrangeOrder(UI_FLOWERTREE, FALSE);
}

/************************************************************************/
/* Name : ResetPosition                                                 */
/************************************************************************/
void CUIFlowerTree::ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
	SetPos((pixMaxI + pixMinI) / 2 - GetWidth(), (pixMaxJ + pixMinJ - GetHeight()) / 2);
}

/************************************************************************/
/* Name : AdjustPosition                                                */
/************************************************************************/
void CUIFlowerTree::AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
	if ( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition(pixMinI, pixMinJ, pixMaxI, pixMaxJ);
}

/************************************************************************/
/* Name : MouseMessage                                                  */
/************************************************************************/
WMSG_RESULT CUIFlowerTree::MouseMessage(MSG *pMsg)
{
	WMSG_RESULT wmsg_Result;

	static BOOL bTitleBarClick = FALSE;
	static BOOL bLButtonDownInItem = FALSE;

	// Mouse Point
	static int nOldX, nOldY;
	int nX = LOWORD(pMsg->lParam);
	int nY = HIWORD(pMsg->lParam);

	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			if (IsInside(nX, nY))
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
			else if ( m_btnOK.MouseMessage(pMsg) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if ( m_btnRefresh.MouseMessage(pMsg) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if ( IsInside(nX, nY) )
			{
				nOldX = nX; nOldY = nY;

				if ( m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
				{

				}
				else if ( IsInsideRect(nX, nY, m_rcTitle) )
				{
					bTitleBarClick = TRUE;
				}
				else if ( m_btnOK.MouseMessage(pMsg) != WMSG_FAIL )
				{

				}
				else if ( m_btnRefresh.MouseMessage(pMsg) != WMSG_FAIL )
				{

				}

				CUIManager::getSingleton()->RearrangeOrder(UI_FLOWERTREE, TRUE);

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

				if ( (wmsg_Result = m_btnClose.MouseMessage(pMsg)) != WMSG_FAIL )
				{
					if ( wmsg_Result == WMSG_COMMAND )
						CloseFlowerTree();

					return WMSG_SUCCESS;
				}
				else if ( (wmsg_Result = m_btnOK.MouseMessage(pMsg)) != WMSG_FAIL )
				{
					if ( wmsg_Result == WMSG_COMMAND )
					{ // 개화 돕기
						if (!pUIManager->DoesMessageBoxExist(MSGCMD_FLOWERTREE_SUPPORT))
						{
							if (pUIManager->DoesMessageBoxExist(MSGCMD_FLOWERTREE_TICKET))
								pUIManager->CloseMessageBox(MSGCMD_FLOWERTREE_TICKET);

							if (pUIManager->DoesMessageBoxExist(MSGCMD_FLOWERTREE_TICKETEX))
								pUIManager->CloseMessageBox(MSGCMD_FLOWERTREE_TICKETEX);

							CUIMsgBox_Info MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo(_S(3485, "개화돕기"), UMBS_YESNO, UI_FLOWERTREE, MSGCMD_FLOWERTREE_SUPPORT);
							MsgBoxInfo.AddString(_S(3487, "개화를 도우면 가지고 있는 퇴비와 정화수가 모두 사라집니다. 그래도 개화를 돕겠습니까?"));
							pUIManager->CreateMessageBox(MsgBoxInfo);
						}
					}

					return WMSG_SUCCESS;
				}
				else if ( (wmsg_Result = m_btnRefresh.MouseMessage(pMsg)) != WMSG_FAIL )
				{
					if ( wmsg_Result == WMSG_COMMAND )
					{ // 서버 현황 갱신 Req
						_pNetwork->SendFlowerTreeReq((UBYTE)MSG_EVENT_FLOWERTREE_2007_POINT_RANK); 					
					}

					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}

/************************************************************************/
/* Name : MsgBoxCommand                                                 */
/************************************************************************/
void CUIFlowerTree::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput)
{
	switch (nCommandCode)
	{
	case MSGCMD_FLOWERTREE_SUPPORT: // 개화 돕기 Req
		{
			if (bOK)
			{
				_pNetwork->SendFlowerTreeReq((UBYTE)MSG_EVENT_FLOWERTREE_2007_GROWUP);
			}
		}
		break;
	case MSGCMD_FLOWERTREE_TICKET:
		{
			if (bOK)	// 교환권 받기
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				
				if (!pUIManager->DoesMessageBoxExist(MSGCMD_FLOWERTREE_TICKETEX))
				{
					CUIMsgBox_Info MsgBoxInfo;

					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_YESNO, UI_FLOWERTREE, MSGCMD_FLOWERTREE_TICKETEX);
					MsgBoxInfo.AddString(_S(3488, "누적된 교환권을 받으시겠습니까?"));
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}		
			}
			else
			{ // 교환권 확인
				_pNetwork->SendFlowerTreeReq((UBYTE)MSG_EVENT_FLOWERTREE_2007_OPEN_TICKET);
			}
		}
		break;
	case MSGCMD_FLOWERTREE_TICKETEX:
		{
			if (bOK)
				_pNetwork->SendFlowerTreeReq((UBYTE)MSG_EVENT_FLOWERTREE_2007_RECV_TICKET);
		}
	}
}

/************************************************************************/
/* Name : AddFlowerTreeMesh                                             */
/************************************************************************/
void CUIFlowerTree::AddFlowerTreeMesh(CModelInstance *pMI, int nFlowerStep)
{
	INDEX iMeshID;
	FlowerTreeMeshInfo* treeTemp = NULL;

	if (m_pMobFlowerTree != NULL && m_pMobFlowerTree->m_nType == MOB_FLOWERTREE_INDEX)
	{
		treeTemp = &m_FlowerMeshInfo[nFlowerStep];
	}
	else if(m_pMobFlowerTree != NULL && m_pMobFlowerTree->m_nType == GAMIGO_10TH_CAKE)
	{
		treeTemp = &m_GamigoCakeMeshInfo[nFlowerStep];
	}
	else
	{
		treeTemp = &m_XMASTREEMeshInfo[nFlowerStep];
	}

	iMeshID = ska_GetIDFromStringTable(treeTemp->MeshBM);

	MeshInstance* pMesh = pMI->FindMeshInstance(iMeshID);

	if (pMesh == NULL) // 메쉬가 없으면 추가
	{
/*		pMI->AddMesh_t(m_FlowerMeshInfo[nFlowerStep].MeshBM);
		m_FlowerMeshInfo[nFlowerStep].MeshID = iMeshID;

		pMesh = pMI->FindMeshInstance(iMeshID);
		pMI->AddTexture_t(m_FlowerMeshInfo[nFlowerStep].MeshTex,
			m_FlowerMeshInfo[nFlowerStep].MeshTex.FileName(), pMesh);*/

		pMI->AddMesh_t(treeTemp->MeshBM);
		treeTemp->MeshID = iMeshID;
		
		pMesh = pMI->FindMeshInstance(iMeshID);
		pMI->AddTexture_t(treeTemp->MeshTex, treeTemp->MeshTex.FileName(), pMesh);
	}
}

/************************************************************************/
/* Name : FlowerTreeUpdate                                              */
/************************************************************************/
void CUIFlowerTree::FlowerTreeUpdate(ULONG ulPoint)
{
	// 모델을 찾고 모델 정보를 먼저 얻는다.
	if ( m_pMobFlowerTree == NULL || m_pMobFlowerTree->m_nIdxClient < 0 ) return; // 꽃나무 정보가 없다.

	CEntity* pEntity;
	CModelInstance* pFlowerTreeMI;

	_pNetwork->ga_World.EntityExists(m_pMobFlowerTree->m_nIdxClient, pEntity);

	if ( pEntity == NULL )  return;	

	pFlowerTreeMI = pEntity->GetModelInstance();

	ASSERT( pFlowerTreeMI != NULL && "Invalid FlowerTree Model" );

	if (m_pMobFlowerTree->m_nType == MOB_FLOWERTREE_INDEX) // 꽃나무
	{
		eOpenFlowerStep eTempStep = m_CurFlowerStep;

		if (ulPoint >= 20000) { m_CurFlowerStep = OP_FLOWERSTEP10; }
		else if(ulPoint >= 9000) { m_CurFlowerStep = OP_FLOWERSTEP9; }
		else if(ulPoint >= 8000) { m_CurFlowerStep = OP_FLOWERSTEP8; }
		else if(ulPoint >= 7000) { m_CurFlowerStep = OP_FLOWERSTEP7; }
		else if(ulPoint >= 6000) { m_CurFlowerStep = OP_FLOWERSTEP6; }
		else if(ulPoint >= 5000) { m_CurFlowerStep = OP_FLOWERSTEP5; }
		else if(ulPoint >= 4000) { m_CurFlowerStep = OP_FLOWERSTEP4; }
		else if(ulPoint >= 3000) { m_CurFlowerStep = OP_FLOWERSTEP3; }
		else if(ulPoint >= 2000) { m_CurFlowerStep = OP_FLOWERSTEP2; }
		else if(ulPoint >= 100) { m_CurFlowerStep = OP_FLOWERSTEP1; }
		else { m_CurFlowerStep = OP_FLOWERSTEP0; }

		switch (m_CurFlowerStep) // 여기서는 break가 필요가 없다.
		{
		case OP_FLOWERSTEP10: // 주위에 요정들이 날아다님2 (effect)
			{
				if (m_pEG[3] == NULL)
					m_pEG[3] = StartEffectGroup( Event_Tree10_0, &pFlowerTreeMI->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

				if (m_pEG[4] == NULL)
					m_pEG[4] = StartEffectGroup( Event_Tree10_1, &pFlowerTreeMI->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
			}
		case OP_FLOWERSTEP9: // 주위에 요정들이 날아다님1 (effect)
			{
				if (m_pEG[1] == NULL)
					m_pEG[1] = StartEffectGroup( Event_Tree9_0, &pFlowerTreeMI->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

				if (m_pEG[2] == NULL)
					m_pEG[2] = StartEffectGroup( Event_Tree9_1, &pFlowerTreeMI->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
			}
		case OP_FLOWERSTEP8: // 7단계에서 환한 빛 (effect)
			{
				//DestroyEffectGroupIfValid(m_pEG[0]);
				if (m_pEG[0] == NULL)
					m_pEG[0] = StartEffectGroup( Event_Tree8, &pFlowerTreeMI->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
			}
		case OP_FLOWERSTEP7: // 꽃나무에 꽃이 만개함 
			{
				AddFlowerTreeMesh(pFlowerTreeMI, OP_FLOWERSTEP7);
				AddFlowerTreeMesh(pFlowerTreeMI, OP_FLOWERSTEP7+1);
				AddFlowerTreeMesh(pFlowerTreeMI, OP_FLOWERSTEP7+2);
				AddFlowerTreeMesh(pFlowerTreeMI, OP_FLOWERSTEP7+3);
			}
		case OP_FLOWERSTEP6: // 90 % 개화
			{
				AddFlowerTreeMesh(pFlowerTreeMI, OP_FLOWERSTEP6);
				AddFlowerTreeMesh(pFlowerTreeMI, OP_FLOWERSTEP6+1);
				AddFlowerTreeMesh(pFlowerTreeMI, OP_FLOWERSTEP6+2);
			}
		case OP_FLOWERSTEP5: // 60 % 개화
			{
				AddFlowerTreeMesh(pFlowerTreeMI, OP_FLOWERSTEP5);
			}
		case OP_FLOWERSTEP4: // 30 % 개화
			{
				AddFlowerTreeMesh(pFlowerTreeMI, OP_FLOWERSTEP4);
			}
		case OP_FLOWERSTEP3: // 드문 드문 꽃이 핌
			{
				AddFlowerTreeMesh(pFlowerTreeMI, OP_FLOWERSTEP3);
				AddFlowerTreeMesh(pFlowerTreeMI, OP_FLOWERSTEP3+1);
			}
		case OP_FLOWERSTEP2: // 꽃 봉오리 살짝 벌어짐
			{	
				AddFlowerTreeMesh(pFlowerTreeMI, OP_FLOWERSTEP2);
			}
		case OP_FLOWERSTEP1: // 꽃봉오리 생겨남
			{
				AddFlowerTreeMesh(pFlowerTreeMI, OP_FLOWERSTEP1);
			}
	//	case OP_FLOWERSTEP0: // 가지만 앙상
		}
	}
	else if (m_pMobFlowerTree->m_nType == GAMIGO_10TH_CAKE) // [ldy1978220 2011/5/31] 가미고 10주년 이벤트 케익 
	{
		int nStep = 0; // 1단계

		if (ulPoint >= 15000) { nStep = 2; } // 3단계
		else if (ulPoint >= 1000) { nStep = 1; } // 2단계

		switch(nStep)
		{
		case 2:
			{
				AddFlowerTreeMesh(pFlowerTreeMI, 1); // 3단계
			}
		case 1:
			{
				AddFlowerTreeMesh(pFlowerTreeMI, 0); // 2단계
			}

			// [ldy1978220 2011/6/2] 가미고 10주년 이벤트 케익관련 이펙트 적용
			// 메쉬를 추가 하면서 이펙트 테그가 Refresh 되기  때문에 메쉬 적용 이후에 이펙트 적용함
			if (nStep == 2)
			{
				if (m_pEG[0] == NULL)
					m_pEG[0] = StartEffectGroup( Gamigo_10th_cake, &pFlowerTreeMI->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

				if (m_pEG[3] == NULL)
					m_pEG[3] = StartEffectGroup( Event_Tree10_0, &pFlowerTreeMI->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

				if (m_pEG[4] == NULL)
					m_pEG[4] = StartEffectGroup( Event_Tree10_1, &pFlowerTreeMI->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());				
			}
		}
	}
	else
	{
		int nStep = 0; // 1단계

		if (ulPoint >= 15000) { nStep = 2; } // 3단계
		else if (ulPoint >= 1000) { nStep = 1; } // 2단계

		switch(nStep)
		{
		case 2:
			{
				AddFlowerTreeMesh(pFlowerTreeMI, 1); // 3단계
			}
		case 1:
			{
				AddFlowerTreeMesh(pFlowerTreeMI, 0); // 2단계
			}
		}
	}
}

/************************************************************************/
/* Name : AddServerNameList                                             */
/************************************************************************/
void CUIFlowerTree::AddServerNameList(ULONG ulScore, SLONG slGroup)
{
	std::vector<sServerScoreList>::iterator SvScItr;
	sServerScoreList SvSctmp;

	SvSctmp.slGroup = slGroup;

	SvScItr = std::find_if(m_vecSvScoreList.begin(), m_vecSvScoreList.end(), FindServerGroup(SvSctmp));

	if (SvScItr == m_vecSvScoreList.end())
	{
		SvSctmp.strServerName = CUIManager::getSingleton()->GetServerSelect()->GetServerName(slGroup);
		m_vecSvScoreList.push_back(SvSctmp);
		return; // 군정보만 추가하고 리턴
	}

	// 스코어 갱신
	(*SvScItr).ulScore = ulScore;

	if (_pNetwork->m_iServerGroup >= 1000 && slGroup < 1000)
	{// 테섭이 서버군 번호가 1000을 넘어서 처리
		slGroup += 1000;
	}

	if (_pNetwork->m_iServerGroup == slGroup)
	{ // 접속된 서버군일때
		FlowerTreeUpdate(ulScore);
	}
}

/************************************************************************/
/* Name : ClearList                                                     */
/************************************************************************/
void CUIFlowerTree::ClearList()
{
	m_pMobFlowerTree = NULL;

	m_vecSvScoreList.clear();

	m_CurFlowerStep = OP_FLOWERSTEP0;

	ClearEffect();
}

/************************************************************************/
/* Name : ClearEffect                                                   */
/************************************************************************/
void CUIFlowerTree::ClearEffect()
{
	int i;

	for(i=0; i<5; i++)
	{
		DestroyEffectGroupIfValid(m_pEG[i]);
	}
}

void CUIFlowerTree::initialize()
{
	m_rcTitle.SetRect(0, 0, 216, 21);

}
