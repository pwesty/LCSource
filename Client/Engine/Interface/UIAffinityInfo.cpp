// UIAffinityInfo.cpp: implementation of the CUIAffinityInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <set>
#include <Engine/Interface/UIMultList.h> // 친화도 개편2 [2/6/2013 Ranma]
#include <Engine/Interface/UIAffinityInfo.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UIInventory.h>

// [2013/02/12] sykim70 미니맵 추가
inline int GetZoneByNpcIndex(int nNPCIndex)
{
	CNpcHelp::_map::iterator	iter = CNpcHelp::_mapdata.begin();
	CNpcHelp::_map::iterator	eiter = CNpcHelp::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CNpcHelp* pNpc = (*iter).second;

		if (pNpc == NULL)
			continue;

		if (pNpc->getNpcIndex() == nNPCIndex)
			return pNpc->zoneNum;
	}

	return -1;
}

inline int GetZoneByMobIndex(int nMobIndex)
{
	CMobHelp::_map::iterator	iter1 = CMobHelp::_mapdata.begin();
	CMobHelp::_map::iterator	eiter1 = CMobHelp::_mapdata.end();

	for (;iter1 != eiter1; ++iter1)
	{
		CMobHelp* pMob = (*iter1).second;

		if (pMob == NULL)
			continue;

		if (pMob->getNpcIndex() == nMobIndex)
			return pMob->zoneNum;
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
//  Name 	: CUIAffinityInfo()
//  Desc 	: Constructor
// ----------------------------------------------------------------------------
CUIAffinityInfo::CUIAffinityInfo() :
	m_ptdButtonTexture(NULL),
	m_ptdSignalTexture(NULL),
	m_bShowToolTip(TRUE),
	m_bPickTitle(FALSE)
{
}

// ----------------------------------------------------------------------------
//  Name 	: ~CUIAffinityInfo()
//  Desc 	: Destructor
// ----------------------------------------------------------------------------
CUIAffinityInfo::~CUIAffinityInfo()
{
	Destroy();

	STOCK_RELEASE(m_ptdSignalTexture);
	STOCK_RELEASE(m_ptdButtonTexture);
	STOCK_RELEASE(m_ptdBaseTexture);

	AffinityReset();
}

// ----------------------------------------------------------------------------
//  Name 	: Create()
//  Desc 	: initialize to ui position.
// ----------------------------------------------------------------------------
void	CUIAffinityInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Create Texture.
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Affinity.tex" ) );
	m_ptdButtonTexture = CreateTexture(CTString("Data\\Interface\\CommonBtn.tex"));
	
	// Get Texture size.
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();
	
	// Set size of UI on this texture.
	m_rtsBase.AddRectSurface(UIRect(0,0,nWidth,nHeight),UIRectUV(0,0,512,421,fTexWidth,fTexHeight));	// 틀.

	int nLineHeight = 40;
	// 친화도 개편2 친화도 아이콘 리스트 Create부분 [2/6/2013 Ranma]
	m_ListAffinityList.Create(this, 12, 44, 150 - 10, 359, nLineHeight, 0, 0, 2, TRUE, 2, 2);
	m_ListAffinityList.SetColumnType(0, TYPE_ICON);
	m_ListAffinityList.SetColumnPosX(1, nLineHeight + 4);
	m_ListAffinityList.SetColumnWidth(1, 90);
	m_ListAffinityList.SetOverColor(0xFEFEBAFF);
	m_ListAffinityList.SetSelectColor(0xFFEF19FF);

	int nWidthbox = 28;
	int nWidthGap = 0;
	// 친화도 개편2 친화도 보상 아이콘 리스트 Create부분 [2/6/2013 Ranma]
	for (int i = 0; i < AFFINITY_REWARD_MAX; i++)
	{
		nWidthGap = i * 29;
		m_rtAffinityRewardBox[i].Create( this, 297 + nWidthGap, 174, 28, 28);
		
		m_rtAffinityRewardBox[i].SetRenderRegion(0, 0, 28, 28);
	}
	m_nRewardCount = 0;
	
	// Set title.
	m_rtTitle.SetRect(0,0,nWidth - 20,37);
	// set Skill Area.
	m_rtArea.SetRect( 11, 43, 163, 405 );

	// 친화도 개편2 NPC 대사 나오는 부분 [2/7/2013 Ranma]
	m_rtNPC.SetRect( 167, 42, 501, 170 );

	// 친화도 개편2 친화도 탭 관련 렉트 [2/12/2013 Ranma]
	m_rtTab.SetRect( 286, 206, 500, 232 );
	m_rtTabArea.SetRect( 286, 249, 500, 405);


	// 친화도 개편2 아이템/몬스터/퀘스트 탭 [2/12/2013 Ranma]
	m_nCurrentTab = 0;
	// 친화도 개편2 아이템 탭
	m_AffinityTab[UI_TAB_ITEM].Create(this, 286, 248, 212 - 10, 155, 52, 0, 0, 2, FALSE, 0, 10);
	m_AffinityTab[UI_TAB_ITEM].SetColumnType(0, TYPE_ICON);
	m_AffinityTab[UI_TAB_ITEM].SetColumnPosX(1, 52 + 5);
	m_AffinityTab[UI_TAB_ITEM].SetColumnWidth(1, 140);
	
	m_btnAffinityTab[UI_TAB_ITEM].Create(this, _S( 4288, "아이템" ), 292, 208, 63, 21);
	m_btnAffinityTab[UI_TAB_ITEM].SetUV(UBS_IDLE,513, 263, 576, 284,fTexWidth,fTexHeight);
	m_btnAffinityTab[UI_TAB_ITEM].SetUV(UBS_ON, 576, 263, 639, 284,fTexWidth,fTexHeight);
	m_btnAffinityTab[UI_TAB_ITEM].SetUV(UBS_CLICK,639, 263, 702, 284,fTexWidth,fTexHeight);
	m_btnAffinityTab[UI_TAB_ITEM].CopyUV(UBS_IDLE,UBS_DISABLE);

	// 친화도 개편2 퀘스트 탭
	m_AffinityTab[UI_TAB_QUEST].Create(this, 286, 248, 212 - 10, 155, 52, 0, 0, 2, FALSE, 0, 10);
	m_AffinityTab[UI_TAB_QUEST].SetColumnType(0, TYPE_ICON);
	m_AffinityTab[UI_TAB_QUEST].SetColumnPosX(1, 52 + 5);
	m_AffinityTab[UI_TAB_QUEST].SetColumnWidth(1, 140);
	
	m_btnAffinityTab[UI_TAB_QUEST].Create(this, _S( 168, "퀘스트" ), 362, 208, 63, 21);
	m_btnAffinityTab[UI_TAB_QUEST].SetUV(UBS_IDLE,513, 263, 576, 284,fTexWidth,fTexHeight);
	m_btnAffinityTab[UI_TAB_QUEST].SetUV(UBS_ON, 576, 263, 639, 284,fTexWidth,fTexHeight);
	m_btnAffinityTab[UI_TAB_QUEST].SetUV(UBS_CLICK,639, 263, 702, 284,fTexWidth,fTexHeight);
	m_btnAffinityTab[UI_TAB_QUEST].CopyUV(UBS_IDLE,UBS_DISABLE);

	// 친화도 개편2 몬스터 탭
	m_AffinityTab[UI_TAB_MONSTER].Create(this, 286, 248, 212 - 10, 155, 52, 0, 0, 2, FALSE, 0, 10);
	m_AffinityTab[UI_TAB_MONSTER].SetColumnType(0, TYPE_ICON);
	m_AffinityTab[UI_TAB_MONSTER].SetColumnPosX(1, 52 + 5);
	m_AffinityTab[UI_TAB_MONSTER].SetColumnWidth(1, 140);
	
	m_btnAffinityTab[UI_TAB_MONSTER].Create(this, _S( 448, "몬스터" ), 432, 208, 63, 21);
	m_btnAffinityTab[UI_TAB_MONSTER].SetUV(UBS_IDLE,513, 263, 576, 284,fTexWidth,fTexHeight);
	m_btnAffinityTab[UI_TAB_MONSTER].SetUV(UBS_ON, 576, 263, 639, 284,fTexWidth,fTexHeight);
	m_btnAffinityTab[UI_TAB_MONSTER].SetUV(UBS_CLICK,639, 263, 702, 284,fTexWidth,fTexHeight);
	m_btnAffinityTab[UI_TAB_MONSTER].CopyUV(UBS_IDLE,UBS_DISABLE);

	// [2013/02/12] sykim70 미니맵 추가
	m_rtMiniMapView.SetRect(168, 206, 168+113, 206+113);

	// 친화도 개편2 선택 세력 대사 리스트 [2/7/2013 Ranma]
	// Get Button Texture Size.
	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();

	int _nMsgBoxLineHeight = _pUIFontTexMgr->GetFontHeight() + 4;

	// List box
	m_lbDescription.Create( this, 166, 40, 323, 126, _nMsgBoxLineHeight, 12, 8, 1, FALSE, 1, 0 );


	// close button.
	m_btnClose.Create(this,CTString(""),490,4,16,16);
	m_btnClose.SetUV(UBS_IDLE,211, 33, 227, 49,fTexWidth,fTexHeight);
	m_btnClose.SetUV(UBS_CLICK,229, 33, 245, 49,fTexWidth,fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE,UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE,UBS_DISABLE);

	// 친화도 개편2 친화도 수치 스트링 [2/12/2013 Ranma]
	m_strAffinityPoint.PrintF("%d/%d", 0, 0);	

	// [2013/02/12] sykim70 미니맵 추가
	m_bChangeMiniMap = false;
	m_nAffinityIndex = -1;
	m_nAdminNPCIndex = -1;
	m_nPositionNPCIndex = -1;
	m_nPositionZoneNo = -1;
	m_ptdMapTex = NULL;

	// [2013/02/13] sykim70 관리자 NPC 정보 표시
	m_ptdSignalTexture = CreateTexture(CTString("Data\\Interface\\Map.tex"));
	fTexWidth = m_ptdSignalTexture->GetPixWidth();
	fTexHeight = m_ptdSignalTexture->GetPixHeight();
	m_rtAdminNpcView.SetRect(168+113/2-6, 206+113/2-9, 168+113/2+6, 206+113/2+8);
	m_rtAdminNpc.SetUV(127, 64, 140, 82, fTexWidth, fTexHeight);
	m_rtAdminNpcNameView.SetRect(168, 325, 168+113, 325+46);
}

// ----------------------------------------------------------------------------
//  Name 	: Render()
//  Desc 	: rendering ui.
// ----------------------------------------------------------------------------
void	CUIAffinityInfo::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// info is view
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	m_rtsBase.SetPos( m_nPosX, m_nPosY );
	m_rtsBase.RenderRectSurface( pDrawPort, 0xFFFFFFFF );

	pDrawPort->FlushRenderingQueue();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// text setting
	CTString strTemp;
	strTemp.PrintF( _S(4480, "친화 ") );
	pDrawPort->PutTextExCX(strTemp, m_nPosX+NPCAFFINITYINFO_WIDTH/2, m_nPosY+15, 0xDED9A0FF);
	
	pDrawPort->EndTextEx();
	pDrawPort->FlushRenderingQueue();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 친화도 개편2 탭버튼 랜더 [2/12/2013 Ranma]
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	m_btnAffinityTab[m_nCurrentTab].SetBtnState(UBS_ON);

 	for (int i = 0; i < UI_TAB_END; i++)
 	{
		m_btnAffinityTab[i].Render();
	}
	
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData(m_ptdButtonTexture);

	m_btnClose.Render();
	pDrawPort->FlushRenderingQueue();

	// 친화도 개편2 친화도 세력 리스트 랜더 [2/7/2013 Ranma]
	m_ListAffinityList.Render();
	
	pDrawPort->EndTextEx();
	pDrawPort->FlushRenderingQueue();

	// 친화도 개편2 대사 랜더 [2/7/2013 Ranma]
	m_lbDescription.Render();
	pDrawPort->EndTextEx();
	pDrawPort->FlushRenderingQueue();

	// 친화도 개편2 친화도 수치 뿌리기 [2/5/2013 Ranma]
	strTemp.PrintF(_S(5861, "친화도 수치"));
	pDrawPort->PutTextEx(strTemp, m_nPosX + 175, m_nPosY + 173, 0xF5F505FF);
	pDrawPort->EndTextEx();
	
	pDrawPort->PutTextEx(m_strAffinityPoint, m_nPosX + 175, m_nPosY + 187, 0xF5F505FF);
	pDrawPort->EndTextEx();
	pDrawPort->FlushRenderingQueue();
	// Render all elements

	// [2013/02/12] sykim70 미니맵 추가
	RenderMiniMap();

	// 친화도 개편2 친화도 보상 아이콘 리스트 Render [2/6/2013 Ranma]
	for (int j = 0; j < m_nRewardCount; j++)
	{
		m_rtAffinityRewardBox[j].Render();
		m_rtAffinityRewardBox[j].PopupRender();
	}
	pDrawPort->FlushRenderingQueue();	

	// 친화도 개편2 탭 랜더 [2/13/2013 Ranma]
	m_AffinityTab[m_nCurrentTab].Render();
	pDrawPort->EndTextEx();
	pDrawPort->FlushRenderingQueue();

	m_AffinityTab[m_nCurrentTab].RenderIconPopup();
	pDrawPort->FlushRenderingQueue();
	
}

// ----------------------------------------------------------------------------
//  Name 	: RenderMiniMap()
//  Desc 	: [2013/02/12] sykim70 미니맵 추가
// ----------------------------------------------------------------------------
void CUIAffinityInfo::RenderMiniMap()
{
	if (m_nAffinityIndex == -1)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if (m_bChangeMiniMap)
	{
		if (m_ptdMapTex)
		{
			_pTextureStock->Release(m_ptdMapTex);
			m_ptdMapTex = NULL;
		}

		BOOL bWorldMap;
		int nDetail, nX, nZ = 0;

		MapData* md = pUIManager->GetMap()->GetCurMapData(m_nPositionZoneNo);
		for (int i = 0; i < md->vecNpc.size(); i++)
		{
			NpcData& npcData = md->vecNpc[i];
			if (npcData.nIndex != m_nPositionNPCIndex)
				continue;

			if (IS_EVENT_ON(A_EVENT_HOLLOWEEN) == 0)
			{
				if (npcData.nIndex == 454 ||
					npcData.nIndex == 455)
					continue;
			}

			nDetail = -1;
			for (int j = 0; j < md->vecDetail.size(); j++)
			{
				DetailMapData& dmd = md->vecDetail[j];
				nX = (npcData.fX - dmd.fOffsetX) * dmd.fRatio;
				nZ = (npcData.fZ - dmd.fOffsetZ) * dmd.fRatio;
				if (nX >= dmd.rcMapSize.Left && nX < dmd.rcMapSize.Right &&
					nZ >= dmd.rcMapSize.Top && nZ < dmd.rcMapSize.Bottom)
				{
					bWorldMap = FALSE;
					nDetail = j;
					break;
				}
			}
			if (nDetail == -1)
			{
				bWorldMap = TRUE;
				nX = (npcData.fX - md->World.lOffsetX) * md->World.fRatio;
				nZ = (npcData.fZ - md->World.lOffsetZ) * md->World.fRatio;
			}

			nX -= 113/2;
			nZ -= 113/2;

			// [2013/02/13] sykim70 관리자 NPC 정보 표시
			m_strAdminPosition.PrintF("%d, %d", (int)npcData.fX, (int)npcData.fZ);
			break;
		}

		CTString strFileName;
		if (bWorldMap)
			strFileName.PrintF("Data\\Interface\\Map_World%d0.tex", m_nPositionZoneNo);
		else
			strFileName.PrintF("Data\\Interface\\Map_World%d0_%d.tex", m_nPositionZoneNo, nDetail);
		m_ptdMapTex = CreateTexture(strFileName);
		
		FLOAT fDetailTexWidth = m_ptdMapTex->GetPixWidth();
		FLOAT fDetailTexHeight = m_ptdMapTex->GetPixHeight();
		m_rtMiniMap.SetUV(nX, nZ, nX+113, nZ+113, fDetailTexWidth, fDetailTexHeight);

		m_bChangeMiniMap = false;
	}

	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// render map
	pDrawPort->InitTextureData(m_ptdMapTex);
	pDrawPort->AddTexture(m_nPosX + m_rtMiniMapView.Left, m_nPosY + m_rtMiniMapView.Top,
						  m_nPosX + m_rtMiniMapView.Right, m_nPosY + m_rtMiniMapView.Bottom,
						  m_rtMiniMap.U0, m_rtMiniMap.V0, m_rtMiniMap.U1, m_rtMiniMap.V1,
						  0xFFFFFFFF);
	pDrawPort->FlushRenderingQueue();

	// [2013/02/13] sykim70 관리자 NPC 정보 표시
	pDrawPort->InitTextureData(m_ptdSignalTexture);
	pDrawPort->AddTexture(m_nPosX + m_rtAdminNpcView.Left, m_nPosY + m_rtAdminNpcView.Top,
						  m_nPosX + m_rtAdminNpcView.Right, m_nPosY + m_rtAdminNpcView.Bottom,
						  m_rtAdminNpc.U0, m_rtAdminNpc.V0, m_rtAdminNpc.U1, m_rtAdminNpc.V1,
						  0xFFFFFFFF);
	pDrawPort->FlushRenderingQueue();

	for (int i = 0; i < m_vecAdminNpcName.size(); i++)
		pDrawPort->PutTextExCX(m_vecAdminNpcName[i], m_nPosX+223, m_nPosY+331+i*20, 0xF2F2F2FF);
	pDrawPort->PutTextExCX(m_strAdminPosition, m_nPosX+223, m_nPosY+384, 0x00F200FF);
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
//  Name 	: ResetPosition()
//  Desc 	: 
// ----------------------------------------------------------------------------
void	CUIAffinityInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
//  Name 	: AdjustPosition()
//  Desc 	: 
// ----------------------------------------------------------------------------
void	CUIAffinityInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
//  Name 	: MouseMessage()
//  Desc 	: 
// ----------------------------------------------------------------------------
WMSG_RESULT		CUIAffinityInfo::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	static INDEX clickAffinitypos = -1;
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	// vecter interator.
	UIButton_vec::iterator iter;
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
			{
				if( IsInside( nX, nY ) )
					pUIManager->SetMouseCursorInsideUIs();

				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				
				// Hold item button
				if (pUIManager->GetDragIcon() == NULL && bLButtonDownInBtn && ( pMsg->wParam & MK_LBUTTON ) &&
					( ndX != 0 || ndY != 0 ) )
				{
					// Close message box of inventory
					pUIManager->CloseMessageBox( MSGCMD_DROPITEM );
				}

				// Move UI window.
				if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
				{
					nOldX = nX;	nOldY = nY;

					Move( ndX, ndY );

					return WMSG_SUCCESS;
				}
				else if ( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;	
				}
				else if( m_ListAffinityList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}					
				else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}			
				else if( m_btnAffinityTab[UI_TAB_ITEM].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				else if( m_btnAffinityTab[UI_TAB_QUEST].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				else if( m_btnAffinityTab[UI_TAB_MONSTER].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				else if( m_AffinityTab[m_nCurrentTab].MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				
				/////TEST CODE RANMA/////////
				for (int j = 0; j < AFFINITY_REWARD_MAX; j++)
				{
					m_rtAffinityRewardBox[j].MouseMessage( pMsg );
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
						// Nothing
					}
					else if( IsInsideRect( nX, nY, m_rtTitle ) )
					{
						m_bPickTitle = TRUE;
					}
					else if (IsInsideRect(nX, nY, m_rtAdminNpcNameView))
					{
						if (m_nAffinityIndex != -1)
						{
							// [2013/02/13] sykim70 맵 표시
							CUIManager* pUIManager = CUIManager::getSingleton();
							CUIMap *pUIMap = pUIManager->GetMap();
							pUIMap->SetCurrentWorldMap(m_nPositionZoneNo, 0);
							pUIManager->m_nHelpNpc_Index = m_nPositionNPCIndex;
							pUIManager->m_IsHelpMob = FALSE;
							pUIManager->m_IsInField = pUIMap->IsNpcInField(m_nPositionZoneNo, m_nPositionNPCIndex);
							pUIMap->ManipulateNpcScroll(TRUE);
							return WMSG_SUCCESS;
						}
					}
					else if( m_ListAffinityList.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						_SetAffinityListData();
						return WMSG_SUCCESS;
					}
					else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}			
					else if( m_btnAffinityTab[UI_TAB_ITEM].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
					else if( m_btnAffinityTab[UI_TAB_QUEST].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
					else if( m_btnAffinityTab[UI_TAB_MONSTER].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
					else if( m_AffinityTab[m_nCurrentTab].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}						
				
					CUIManager::getSingleton()->RearrangeOrder( UI_NPC_AFFINITYINFO, TRUE );
					return WMSG_SUCCESS;
				}
			}
		break;

		case WM_LBUTTONUP:
			{
				m_bPickTitle = FALSE;
				if( IsInside( nX, nY ) )
				{
					// Close button
					if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							CloseAffinityInfo();
						}
						return WMSG_SUCCESS;
					}
					else if( m_ListAffinityList.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}					
					else if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}			
					else if( m_btnAffinityTab[UI_TAB_ITEM].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_nCurrentTab = 0;
						return WMSG_SUCCESS;
					}
					else if( m_btnAffinityTab[UI_TAB_QUEST].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_nCurrentTab = 1;
						return WMSG_SUCCESS;
					}
					else if( m_btnAffinityTab[UI_TAB_MONSTER].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_nCurrentTab = 2;
						return WMSG_SUCCESS;
					}
					else if( m_AffinityTab[m_nCurrentTab].MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}
					return WMSG_SUCCESS;
				}
			}
		break;
		
		case WM_LBUTTONDBLCLK:
			{
				return WMSG_SUCCESS;
			}
		break;

		case WM_MOUSEWHEEL:
			{
				if( IsInside( nX, nY ) )
				{
					if( IsInsideRect( nX, nY, m_rtArea ) )
					{
						if( m_ListAffinityList.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}					
					}
					else if( IsInsideRect( nX, nY, m_rtNPC ) )
					{
						if( m_lbDescription.MouseMessage( pMsg ) != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}			
					}
					else if( IsInsideRect( nX, nY, m_rtTabArea ) )
					{
						if( m_AffinityTab[m_nCurrentTab].MouseMessage( pMsg ) != WMSG_FAIL )
						{
							return WMSG_SUCCESS;
						}						
					}
					return WMSG_SUCCESS;
				}
			}
		break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
//  Name 	: ToggleAffinityInfo()
//  Desc 	: // not used.
// ----------------------------------------------------------------------------
void			CUIAffinityInfo::_toggleVisible()
{
	BOOL bVisible = !( IsVisible() );

	if( bVisible )
	{
		OpenAffinityInfo();
	}
	else
	{
		CloseAffinityInfo();
	}
}

// ----------------------------------------------------------------------------
//  Name 	: OpenAffinityInfo()
//  Desc 	: 
// ----------------------------------------------------------------------------
void			CUIAffinityInfo::OpenAffinityInfo()
{
	//set rendering type.
	// if render type is TYPE_AFFINITY, setting affinity data. 
	// but type is TYPE_EXPERT, setting expert data.
	// reset button object.

	// if visible, clear button object and close UI.
	// but, is not visible. follow next operation.
	if( IsVisible() )
	{
		CloseAffinityInfo();
		return;
	}

	_setAffinityData();
	CUIManager::getSingleton()->RearrangeOrder( UI_NPC_AFFINITYINFO, TRUE );
}

// ----------------------------------------------------------------------------
//  Name 	: CloseAffinityInfo()
//  Desc 	: 
// ----------------------------------------------------------------------------
void			CUIAffinityInfo::CloseAffinityInfo()
{
	AffinityReset();
	CUIManager::getSingleton()->RearrangeOrder( UI_NPC_AFFINITYINFO, FALSE );
}

// ----------------------------------------------------------------------------
//  Name 	: AffinityReset()
//  Desc 	: 
// ----------------------------------------------------------------------------
void			CUIAffinityInfo::AffinityReset()
{	
	 for( UIButton_vec::iterator itr = m_btnvecAffinity.begin(); itr != m_btnvecAffinity.end(); ++itr )
	 {
		 delete (*itr);
	 }

	m_btnvecAffinity.clear();
	// 친화도 개편2 세력아이콘 리셋 [2/7/2013 Ranma]
	m_ListAffinityList.ResetAllData();
	

	// 친화도 개편2 세력 대사 리셋 [2/7/2013 Ranma]
	m_lbDescription.ResetAllData();
	m_AffinityList.clear();
	m_strAffinityPoint.PrintF("%d/%d", 0, 0);
	_ResetAffinityTab();

	// [2013/02/12] sykim70 미니맵 추가
	m_bChangeMiniMap = false;
	m_nAffinityIndex = -1;
	m_nAdminNPCIndex = -1;
	m_nPositionNPCIndex = -1;
	m_nPositionZoneNo = -1;
	if (m_ptdMapTex)
		_pTextureStock->Release(m_ptdMapTex);
	m_ptdMapTex = NULL;
}

// ----------------------------------------------------------------------------
//  Name 	: SetAffinityData()
//  Desc 	: 
// ----------------------------------------------------------------------------
void			CUIAffinityInfo::_setAffinityData()
{
	AffinityReset();

	int cnt = _pNetwork->GetAffinityPointer()->count;
	// 친화도 개편2 친화도 세력(맺음) 구분 컨테이너 [2/7/2013 Ranma]
	std::set<int> OnAffinityList;

	CNetworkLibrary::_AffinityInfo* pInfo = _pNetwork->GetAffinityPointer();
	CNetworkLibrary::_AffinityInfo::mapAffIter iter;

	for( iter = pInfo->mapAffinityList.begin(); iter != pInfo->mapAffinityList.end(); ++iter )
	{
		CUIIcon* pIcon = new CUIIcon();
		pIcon->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, _getUIType(), UBET_SKILL);
		pIcon->setAffinity((*iter).first);
//		pIcon->SetSkillAffinityInfo( (*iter).first );
//		pIcon->SetSkillAffinityIndex( (*iter).first );
//		pIcon->SetSkillAffinityCurrentPt( (*iter).second.current );
//		pIcon->SetSkillAffinityMaxPt( (*iter).second.max );
		m_btnvecAffinity.push_back( pIcon );

		// 친화도 개편2 친구로 맺은 세력들 셋팅 [2/7/2013 Ranma]
		OnAffinityList.insert( iter->first );
		// 친화도 개편2 친구로 맺은 세력들 부터 랜더링 [2/7/2013 Ranma]
		m_AffinityList.push_back( make_pair( iter->first, true) );
	}

	CAffinityData* pTempAffinityData = _pNetwork->GetAffinityData();

	// 맵은 키값으로 소팅이 되기 때문에 값이 뒤에서부터 들어가 있다. 그래서 뒤에서 부터 값을 가져도록 하자 최대한 근접하게 [2/7/2013 Ranma]
	CAffinityData::mapAffReverseIter itermapBegin = pTempAffinityData->mapAffinity_Data.rbegin();
	CAffinityData::mapAffReverseIter itermapEnd = pTempAffinityData->mapAffinity_Data.rend();
	CAffinityData::mapAffReverseIter itermapP = itermapBegin;

	for (; itermapP != itermapEnd; itermapP++)
	{
		int pInfoIndex = itermapP->first;

		if (OnAffinityList.find(pInfoIndex) == OnAffinityList.end())
		{
			// 친화도 개편2 친구로 맺지 못한 세력 셋팅 [2/7/2013 Ranma]
			m_AffinityList.push_back( make_pair( pInfoIndex, false ) );
		}
	}

	AffintyList_vec::iterator ListIterBegin = m_AffinityList.begin();
	AffintyList_vec::iterator ListIterEnd = m_AffinityList.end();
	AffintyList_vec::iterator ListIter = ListIterBegin;

	CTString szTextTitle;
	
	for (; ListIter != ListIterEnd; ListIter++)
	{
		m_ListAffinityList.AddIcon(0, CUIImageBox::IT_AFFINITY, ListIter->first );

		szTextTitle.PrintF( "%s", _pNetwork->GetAffinityData()->GetAffinityDataByIndex(ListIter->first)->GetName() );

		if (ListIter->second)
		{
			m_ListAffinityList.AddString(1, szTextTitle, 0xFFFFFFFF);
		}
		else
		{
			m_ListAffinityList.AddString(1, szTextTitle, 0x4C4C4CFF);
		}
		
	}

	// 친화도 개편2 세력이 하나라도 맺어져 있을 경우 가장 위의 세력으로 셋팅 [2/13/2013 Ranma]
	if (ListIterBegin->second)
	{
		_SetAffinityListData(ListIterBegin->first);
	}
	
}

// ----------------------------------------------------------------------------
//  Name 	: _SetAffinityTab(int affinityIdx)
//  Desc 	: 
// ----------------------------------------------------------------------------
// 친화도 개편2 대사, 리스트, 설정 [2/12/2013 Ranma]
void			CUIAffinityInfo::_SetAffinityListData(int affinityIdx)
{

	CUIManager* pUIManager = CUIManager::getSingleton();

	int nSel = m_ListAffinityList.GetCurSel();
	if ( (nSel >= 0 && nSel < m_AffinityList.size()) || affinityIdx )
	{
		//친화도 개편2
		//선택한 세력의 인덱스 얻기 -> 친구를 맺은 세력인지 확인 -> 맺은 세력이면 데이타 접근 -> 엔피씨 인덱스 얻기 [2/7/2013 Ranma]

		int nAffinityIndex = 0;
		
		if (nSel >= 0 && nSel < m_AffinityList.size())
			nAffinityIndex = m_AffinityList[nSel].first;

		if (affinityIdx)
			nAffinityIndex = affinityIdx;
	
		if (m_nAffinityIndex != nAffinityIndex)
		{
			m_lbDescription.ResetAllData();
			m_strAffinityPoint.PrintF("%d/%d", 0, 0);
			
			m_nAffinityIndex = nAffinityIndex;
			m_nAdminNPCIndex = _pNetwork->GetAffinityData()->GetAdminNPCIndex(m_nAffinityIndex);
		
			CTString strNpcName = "";
			// 친화도 개편2 NPC 이름 얻기 [2/5/2013 Ranma]
			strNpcName.PrintF("%s", CMobData::getData( m_nAdminNPCIndex )->GetName());
			m_lbDescription.AddString(0, strNpcName);
			// 친화도 개편2 NPC 대화 얻기 [2/5/2013 Ranma]
			extern void StringSplit(std::vector<CTString>& vecOutput, CTString strInput, ULONG ulColumnWidth);
			std::vector<CTString> vecOutput;
			//  친화도 개편2 글자 자르기 [2/12/2013 Ranma]
			StringSplit(vecOutput, pUIManager->GetString(_pNetwork->GetAffinityData()->GetAffinityStringIndex(m_nAdminNPCIndex)), 300);
			std::vector<CTString>::iterator b = vecOutput.begin();
			for (; b != vecOutput.end(); b++)
				m_lbDescription.AddString(0, *b);
			
			// 친화도 개편2 친화도[현재, 최대] 수치 얻어오기 [2/5/2013 Ranma]
			int nCur = 0;
			int nMax = 0;	
			CNetworkLibrary::_AffinityInfo::mapAffIter iterPoint = _pNetwork->AffinityInfo.mapAffinityList.find(m_nAffinityIndex);
			if (iterPoint != _pNetwork->AffinityInfo.mapAffinityList.end())
			{
				nCur = iterPoint->second.current;
				nMax = iterPoint->second.max;
			}
		
			m_strAffinityPoint.PrintF("%d/%d", nCur, nMax);
			
			_SetAffinityTab(m_nAffinityIndex);
			
			
			// [2013/02/12] sykim70 미니맵 추가
			if (m_nAffinityIndex == 14)	// 흑의 추적자연맹
				m_nPositionNPCIndex = 54;	// 공간술사 리안
			else
				m_nPositionNPCIndex = m_nAdminNPCIndex;
			m_nPositionZoneNo = GetZoneByNpcIndex(m_nPositionNPCIndex);
			m_bChangeMiniMap = true;
			
			// [2013/02/13] sykim70 관리자 NPC 정보 표시
			m_vecAdminNpcName.clear();
			if (m_nPositionNPCIndex != m_nAdminNPCIndex)
				strNpcName = CMobData::getData(m_nPositionNPCIndex)->GetName();
			StringSplit(m_vecAdminNpcName, strNpcName, 90);
			while (m_vecAdminNpcName.size() > 2)
									m_vecAdminNpcName.pop_back();
		}
		
	}
}

// ----------------------------------------------------------------------------
//  Name 	: _SetAffinityTab(int affinityIdx)
//  Desc 	: 
// ----------------------------------------------------------------------------
// 친화도 개편2 탭 설정 [2/12/2013 Ranma]
void			CUIAffinityInfo::_SetAffinityTab(int affinityIdx)
{

	CUIManager* pUIManager = CUIManager::getSingleton();
	_ResetAffinityTab();

	// 친화도 개편2 보상 정보 요청 [2/13/2013 Ranma]
	_pNetwork->SendAffinityInfoTabReq(affinityIdx);

	// 친화도 개편2 NPC 인덱스 얻기 [2/13/2013 Ranma]
	int nNPCIndex = _pNetwork->GetAffinityData()->GetAdminNPCIndex(affinityIdx);

	//////////////////// 기부 아이템 [2/13/2013 Ranma]/////////////////////////////////////////////////////
	int i = 0;
	
	CUIInventory* pInven = pUIManager->GetInventory();
	CTString textItem;
	COLOR color = 0xFFFFFFFF;
	std::vector<CAffinityData::_DonationItemInfo>::iterator b, e;
	b = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecDonateList.begin();
	e = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecDonateList.end();
	for (i = 0; b != e; b++, i++)
	{		
		textItem.PrintF(_S(5857, "%s, 친화도를 %d 상승 시켜줍니다."), _pNetwork->GetItemDesc(b->index), b->point);

		if (pInven->GetItemCount(b->index))
			color = 0x11FF11FF;
		else
			color = 0xFFFFFFFF;
			
		m_AffinityTab[UI_TAB_ITEM].AddIcon(0, CUIImageBox::IT_ITEM, b->index, TRUE, textItem);
		m_AffinityTab[UI_TAB_ITEM].AddString(1, CTString(_pNetwork->GetItemName(b->index)), color);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////// 퀘스트 목록 [2/13/2013 Ranma]/////////////////////////////////////////////////////
	CTString textQuest;
	std::vector<CAffinityData::_AffinityQuestInfo>::iterator b1, e1;
	b1 = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecQuestList.begin();
	e1 = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecQuestList.end();
	for (i = 0; b1 != e1; b1++, i++)
	{
 		textQuest.PrintF( _S(5858, "%s 세력 퀘스트, 완료시 친화도를  %d 상승 시켜줍니다."),
 			_pNetwork->GetAffinityData()->GetAffinityDataByIndex(affinityIdx)->GetName(), b1->point);
	
		m_AffinityTab[UI_TAB_QUEST].AddIcon(0, CUIImageBox::IT_ITEM, 6088, TRUE, textQuest);
 		m_AffinityTab[UI_TAB_QUEST].AddString(1, CTString(CQuestSystem::Instance().GetQuestName(b1->index)) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////// 몬스터 목록 [2/13/2013 Ranma]/////////////////////////////////////////////////////
	CTString textMonster;
	CTString textZonename;
	int nZoneNum = 0;
	std::vector<CAffinityData::_AffinityMonsterInfo>::iterator b2, e2;
	b2 = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecMonsterList.begin();
	e2 = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecMonsterList.end();
	for (i = 0; b2 != e2; b2++, i++)
	{
		nZoneNum = GetZoneByMobIndex(b2->index);
		
		if ( nZoneNum < 0 )
			textZonename.PrintF( _S(5892, "미공개") );
		else
			textZonename = CZoneInfo::getSingleton()->GetZoneName( nZoneNum );

		textMonster.PrintF( _S(5864, "[%s] %d Lv %s 처치 시 친화도를 %d 상승 시켜줍니다."),
		textZonename, CMobData::getData(b2->index)->GetLevel(),
		CTString(CMobData::getData(b2->index)->GetName()),
		b2->point );
		m_AffinityTab[UI_TAB_MONSTER].AddIcon(0, CUIImageBox::IT_AFFINITY_MONSTER, b2->index, TRUE, textMonster);
		m_AffinityTab[UI_TAB_MONSTER].AddString(1, CTString(CMobData::getData(b2->index)->GetName()));
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
}

// ----------------------------------------------------------------------------
//  Name 	: RecvAffinity_RewardInfo
//  Desc 	: 친화도 개편2 보상 정보 받기(친화도 인덱스, 다음 보상받을 포인트수치
// ----------------------------------------------------------------------------
void			CUIAffinityInfo::RecvAffinity_RewardInfo(CNetworkMessage* istr)
{
	// 친화도 개편2 친화도 보상 아이콘 리스트 Reset [2/6/2013 Ranma]
	for (int j = 0; j < AFFINITY_REWARD_MAX; j++)
	{
		m_rtAffinityRewardBox[j].ResetImageBox();
	}
	
	SLONG nAffinityIdx = 0;
	SLONG nAffinityNextRewardPoint = 0;
	SLONG nCheckSum = 0; //  [2/14/2013 Ranma]

	(*istr) >> nAffinityNextRewardPoint; // 다음 보상 받을 포인트 [2/13/2013 Ranma]
	(*istr) >> nAffinityIdx;
	(*istr) >> nCheckSum;

	//////////////////// 보상 목록 text [2/13/2013 Ranma]/////////////////////////////////////////////////////
	int nNPCIndex = _pNetwork->GetAffinityData()->GetAdminNPCIndex(nAffinityIdx);
	CTString textReward;
	int i = 0;
	std::vector<CAffinityData::_AffinityRewardInfo>::iterator b, e;
	b = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecRewardList.begin();
	e = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecRewardList.end();
	for (i = 0; b != e; b++, i++)
	{
		textReward.PrintF( _S(5866, "친화도 %d 달성 시 %s 아이템을 보상으로 받을 수 있습니다."), b->point, _pNetwork->GetItemName(b->index));
		m_rtAffinityRewardBox[i].SetPopupInfo(textReward, TRUE);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// 보유한 세력 포인트 [2/13/2013 Ranma]
	int nCur = 0;
	CNetworkLibrary::_AffinityInfo::mapAffIter iterPoint = _pNetwork->AffinityInfo.mapAffinityList.find(m_nAffinityIndex);
	if (iterPoint != _pNetwork->AffinityInfo.mapAffinityList.end())
	{
		nCur = iterPoint->second.current;
	}

	m_nRewardCount = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecRewardList.size();
	b = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecRewardList.begin();
	e = _pNetwork->GetAffinityData()->GetAffinityData(nNPCIndex)->vecRewardList.end();

	if (nCheckSum & (1<<0)) // 친화도 개편2 친화도 세력 친구가 아닐때 [2/14/2013 Ranma]
	{
		for (i = 0; b != e; b++, i++)
		{
			if ( i >= m_nRewardCount)
				break;
			// 모두 받을수 없음 으로 표시 [2/14/2013 Ranma]
			m_rtAffinityRewardBox[i].SetImageByType( CUIImageBox::IT_ITEM, 2555);			
		}
	}
	else if (nCheckSum & (1<<1)) // 모든 보상을 받았을 경우 [2/14/2013 Ranma]
	{
		for (i = 0; b != e; b++, i++)
		{
			if ( i >= m_nRewardCount)
				break;
			// 모든 보상을 받았음으로 표시 [2/14/2013 Ranma]
			m_rtAffinityRewardBox[i].SetImageByType( CUIImageBox::IT_ITEM, 497);
		}
	}
	else // 친화도 친구이면서 받을 보상이 있을 경우 [2/14/2013 Ranma]
	{
		for (i = 0; b != e; b++, i++)
		{
			if ( i >= m_nRewardCount)
				break;
			
			if ( b->point <= nCur )
			{
				if ( b->point < nAffinityNextRewardPoint ) 
				{
					// 받았음 [2/14/2013 Ranma]
					m_rtAffinityRewardBox[i].SetImageByType( CUIImageBox::IT_ITEM, 497);
				}
				else
				{
					// 받을수 있음 [2/14/2013 Ranma]
					m_rtAffinityRewardBox[i].SetImageByType( CUIImageBox::IT_ITEM, 6227);
				}
			}
			else
			{
				// 받을수 없음 [2/14/2013 Ranma]
				m_rtAffinityRewardBox[i].SetImageByType( CUIImageBox::IT_ITEM, 2555);
			}
		}
	}
}
// ----------------------------------------------------------------------------
//  Name 	: _ResetAffinityTab
//  Desc 	: 
// ----------------------------------------------------------------------------

void			CUIAffinityInfo::_ResetAffinityTab()
{
	for (int i = 0; i < UI_TAB_END; i++)
	{
		m_AffinityTab[i].ResetAllData();
	}

	for (int j = 0; j < AFFINITY_REWARD_MAX; j++)
	{
		m_rtAffinityRewardBox[j].ResetImageBox();
	}

	m_nRewardCount = 0;
}

// ----------------------------------------------------------------------------
//  Name 	: _getUIType()
//  Desc 	: 
// ----------------------------------------------------------------------------
const	ULONG	CUIAffinityInfo::_getUIType() const
{	
	return UI_NPC_AFFINITYINFO;	
}

void CUIAffinityInfo::initialize()
{
	// Set title.
	float fTexWidth, fTexHeight;
	m_rtTitle.SetRect(0,0,GetWidth()- 20,37);
	// set Skill Area.
	m_rtArea.SetRect( 11, 43, 163, 405 );

	// 친화도 개편2 NPC 대사 나오는 부분 [2/7/2013 Ranma]
	m_rtNPC.SetRect( 167, 42, 501, 170 );

	// 친화도 개편2 친화도 탭 관련 렉트 [2/12/2013 Ranma]
	m_rtTab.SetRect( 286, 206, 500, 232 );
	m_rtTabArea.SetRect( 286, 249, 500, 405);

	// [2013/02/12] sykim70 미니맵 추가
	m_rtMiniMapView.SetRect(168, 206, 168+113, 206+113);

	m_strAffinityPoint.PrintF("%d/%d", 0, 0);

	// [2013/02/12] sykim70 미니맵 추가
	m_bChangeMiniMap = false;
	m_nAffinityIndex = -1;
	m_nAdminNPCIndex = -1;
	m_nPositionNPCIndex = -1;
	m_nPositionZoneNo = -1;
	m_ptdMapTex = NULL;

	// [2013/02/13] sykim70 관리자 NPC 정보 표시
	m_ptdSignalTexture = CreateTexture(CTString("Data\\Interface\\Map.tex"));
	fTexWidth = m_ptdSignalTexture->GetPixWidth();
	fTexHeight = m_ptdSignalTexture->GetPixHeight();
	m_rtAdminNpcView.SetRect(168+113/2-6, 206+113/2-9, 168+113/2+6, 206+113/2+8);
	m_rtAdminNpcNameView.SetRect(168, 325, 168+113, 325+46);
}

void CUIAffinityInfo::OnUpdate( float fElapsedTime )
{

}
