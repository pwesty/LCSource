// UIReformSystem.cpp: implementation of the CUIReformSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIReformSystem.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Contents/Base/UIMsgBoxNumeric_only.h>
#include <Engine/Loading.h>
#include <Engine/Info/MyInfo.h>
// 프로그레스 바 추가 [9/12/2012 Ranma]
static BOOL	m_bProcessing = FALSE;

enum REFORM_MENU
{
	REFORM_HIGH = 1,
	REFORM_LOW,
	REFORM_END
};

#define	DEF_TOOLTIP_GAP		10

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

class CmdReformPutOnItem : public Command
{
public:
	CmdReformPutOnItem() : m_pWnd(NULL), m_bOk(false) {}
	void setData(CUIReformSystem* pWnd, bool bOk)	{	m_pWnd = pWnd; m_bOk = bOk;	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			if (m_bOk == true)
				m_pWnd->MagnifierPutOnCallBack();
			else
				m_pWnd->MagnifierPutOnCancelCallBack();
		}
	}
private:
	CUIReformSystem* m_pWnd;
	bool m_bOk;
};

CUIReformSystem::CUIReformSystem()
	: m_pItemsPrev(NULL)
{

}

CUIReformSystem::~CUIReformSystem()
{
	Destroy();

	STOCK_RELEASE(m_ptdButtonTexture);
	int		i;

	for (i = 0 ; i < eSLOT_MAX; i++)
		SAFE_DELETE(m_pIconsItem[i]);

	for (i = 0; i < 2; ++i)
		SAFE_DELETE(m_pTooltip[i]);

	SAFE_DELETE(m_pItemsPrev);
}

void CUIReformSystem::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_nOrgWidth = nWidth;
	m_nOrgHeight = nHeight;
	m_nWidth = 172;
	m_nHeight = 248;
	m_nMiddleGap = 30;

	// create texture.
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SocketSystem.tex" ) );
	m_ptdButtonTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );
	
	//texture size
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtTop.SetUV(0,0,172, 37, fTexWidth, fTexHeight);
	m_rtMiddle.SetUV(0,37, 172, 220, fTexWidth, fTexHeight);
	m_rtBottom.SetUV(0, 190, 172, 229, fTexWidth, fTexHeight );
	m_rtItemSlot.SetUV( 412, 172, 412 + BTN_SIZE+ 2, 172 + BTN_SIZE + 2, fTexWidth, fTexHeight );

	//set size of title.
	m_rcTitle.SetRect(0, 0, REFORM_UI_FRAME_WIDTH, REFORM_UI_FRAME_TITLE);

	//set rect size.
	SetUIRect();
	//리폼 UI texture 사이즈 설정
	m_rtsReform.AddRectSurface( UIRect(0,0,REFORM_UI_INNERFRAME_WIDTH, REFORM_UI_INNERFRAME_HEIGHT),
							    UIRectUV(REFORM_UI_TEX_CREATE_X, REFORM_UI_TEX_CREATE_Y,
										 REFORM_UI_TEX_CREATE_X+REFORM_UI_INNERFRAME_WIDTH,
										 REFORM_UI_TEX_CREATE_Y+REFORM_UI_INNERFRAME_HEIGHT,
										 fTexWidth,fTexHeight) );

	//set button texture.
	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();

	//close button
	m_btnClose.Create(this,CTString(""),148 ,4 ,16 ,16);
	// 버튼 상태
	m_btnClose.SetUV(UBS_IDLE,211, 33, 227, 49, fTexWidth, fTexHeight);
	m_btnClose.SetUV(UBS_CLICK,229, 33, 245, 49, fTexWidth, fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE,UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE,UBS_DISABLE);

	//set button pos
	UIRect rcLeft, rcMiddle, rcRight;
	UIRectUV3 rtIdleBtn, rtClickBtn;
	rcLeft.SetRect(0, 0, 14, 21);
	rcMiddle.SetRect(14, 0, 78, 21);
	rcRight.SetRect(78, 0, 92, 21);
	
	rtIdleBtn.SetUV3(113, 0, 127, 22, 127, 0, 168, 22, 168, 0, 183, 22, fTexWidth, fTexHeight );
	rtClickBtn.SetUV3(187, 0, 201, 22, 201, 0, 242, 22, 242, 0, 256, 22, fTexWidth, fTexHeight);
	
	m_btnConfirm.Create( this, _S(5763,"리폼" ),(REFORM_UI_FRAME_WIDTH/2)-46, m_nHeight - 60, 92, 21 );
	m_btnConfirm.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnConfirm.SetRTSurface( UBS_IDLE, rcMiddle, rtIdleBtn.rtM );
	m_btnConfirm.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnConfirm.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnConfirm.SetRTSurface( UBS_CLICK, rcMiddle, rtClickBtn.rtM );
	m_btnConfirm.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnConfirm.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnConfirm.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnConfirm.SetNewType(TRUE);

	int		i;

	for (i = 0 ; i < eSLOT_MAX; i++)
	{
		m_pIconsItem[i] = new CUIIcon;
		m_pIconsItem[i]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_REFORM_SYSTEM, UBET_ITEM);
	}

	m_btnConfirm.SetEnable(FALSE);

	m_nReformType = 2;

	//프로그레스 바 추가 [9/12/2012 Ranma]
	m_pbBar.Create( this, nX+6, nY+214, 172-6*2, 25);
 	m_pbBar.SetProgressRange( 0, 100 );
 	m_pbBar.SetProgressStep( 1 );
 	m_pbBar.OffsetProgressPos( 0 );
 	m_pbBar.SetAutoComplete( FALSE );

	// 5초에 해당하는 timegettime() 수치.
	m_uEndTime = 1;
	m_pbBar.SetProgressRange( 0, m_uEndTime* 1000 );
	m_bUseTimer = FALSE;

	for (i = 0; i < 2; ++i)
	{
		m_pTooltip[i] = new CUITooltip;
		m_pTooltip[i]->initialize();
		m_pTooltip[i]->setParent(this);
	}
}

void CUIReformSystem::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
//	BOOL bVisible = !(IsVisible());

	if ( nResult >= 0)	// reform UI를 열려고 할때.
	{
		if (pUIManager->GetInventory()->IsLocked() == TRUE ||
			pUIManager->GetInventory()->IsLockedArrange() == TRUE)
		{
			// 이미 Lock 인 창이 있을 경우 열지 못한다.
			pUIManager->GetInventory()->ShowLockErrorMessage();
			return;
		}
	}

	switch(nCommandCode)
	{
	case MSGLCMD_REFORM_SYSTEM:
		{
			// cancle
			if (nResult > 0)
			{
				if(nResult == REFORM_HIGH)
				{
					m_nReformType = 1; // 상급 리폼
				}
				else if(nResult == REFORM_LOW)
				{
					m_nReformType = 0; // 중급 리폼
				
				}
				
				///< 인벤토리창 같이 띄워주기
				if(!pUIManager->GetInventory()->IsVisible())
					pUIManager->GetInventory()->ToggleVisible();
				SetItemButton();
				pUIManager->RearrangeOrder(UI_REFORM_SYSTEM, TRUE);			
				pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_REFORMSYSTEM );
				Render();
			}	
		}
		break;
	}
}
void CUIReformSystem::OpenReformSystem(int iMobIndex, FLOAT fX, FLOAT fZ)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	// 유저가 메뉴를 다시 클릭 하거나 이미 UI가 떠 있는 상태를 방지
	Close();

	if( pUIManager->DoesMessageBoxLExist(MSGLCMD_REFORM_SYSTEM) )
	{
		return;
	}

	CMobData* MD = CMobData::getData(iMobIndex);
	m_nNPCIndex = iMobIndex;
	m_fNPCPosX = fX;
	m_fNPCPosZ = fZ;

	if (CEntity* pEntity = INFO()->GetTargetEntity(eTARGET))
		m_nNPCVIdx = pEntity->GetNetworkID();

	pUIManager->CreateMessageBoxL(_S(5763,"리폼"), UI_REFORM_SYSTEM, MSGLCMD_REFORM_SYSTEM);

	CTString strNpcName = CMobData::getData(iMobIndex)->GetName();
	pUIManager->AddMessageBoxLString( MSGLCMD_REFORM_SYSTEM, TRUE, strNpcName, -1, 0xA3A1A3FF);
	pUIManager->AddMessageBoxLString( MSGLCMD_REFORM_SYSTEM, TRUE, _S(5764,"아이템에 붙어 있는 옵션을 리폼 할 수 있습니다."
																		"황금 돋보기는 1~100까지 넣을 수 있습니다."
																		"일부 저급 아이템은 리폼 할 수 없습니다."
																		"리폼 후에 옵션이 더 낮아질 수도 있습니다."), -1, 0xA3A1A3FF);
	CTString strMessage;
	strMessage.PrintF(_S(5765,"중급 리폼"));
	pUIManager->AddMessageBoxLString(MSGLCMD_REFORM_SYSTEM, FALSE, strMessage, REFORM_LOW);
	strMessage.PrintF(_S(5766,"상급 리폼"));
	pUIManager->AddMessageBoxLString(MSGLCMD_REFORM_SYSTEM, FALSE, strMessage, REFORM_HIGH);
	strMessage.PrintF(_S(1220,"취소한다."));
	pUIManager->AddMessageBoxLString(MSGLCMD_REFORM_SYSTEM, FALSE, strMessage);

	m_pbBar.SetIdleText( _S( 4962, "아이템을 올려주세요."), FALSE );
	m_pbBar.SetDoingText( _S( 5776, "리폼중"), TRUE );
	m_pbBar.SetCompleteText( _S( 4970, "강화완료"), TRUE );
}
void CUIReformSystem::CommandTimer(pReq req, UINT _second /* = 5  */)
{
	// 프로그레스 바 추가 [9/12/2012 Ranma]
	m_bProcessing = TRUE;
	this->m_fnReq = req;
	m_uEndTime = _second;
	m_bUseTimer = TRUE;
	m_pbBar.SetProgressPos( 0 );
	m_pbBar.StartProgress();
	m_uStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	CUIManager* pUIManager = CUIManager::getSingleton();
}

void CUIReformSystem::_calcCmdTimer()
{
	// 프로그레스 바 추가 [9/12/2012 Ranma]
	if( m_bUseTimer )
	{
		m_btnConfirm.SetEnable(FALSE);

		__int64 tmpTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		if( tmpTime - m_uStartTime > ( m_uEndTime * 1000 ) )
		{
			(this->*m_fnReq)();
			m_bUseTimer = FALSE;
		}
		else 
			m_pbBar.OffsetProgressPos( (tmpTime - m_uStartTime) ); 
	}
}

void CUIReformSystem::SendReformReq()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	LONG lReformerUniIndex;
	LONG lGoldUniIndex;
	LONG lReformItemUniIndex;

	CItems*	pItems0 = m_pIconsItem[eSLOT_REFORMER]->getItems();
	CItems*	pItems1 = m_pIconsItem[eSLOT_MAGNIFIER]->getItems();
	CItems*	pItems2 = m_pIconsItem[eSLOT_ITEM]->getItems();

	if (pItems0 == NULL || pItems1 == NULL || pItems2 == NULL)
		return;
	
	// 리포머 유니 인덱스 얻기 [9/6/2012 Ranma]
	lReformerUniIndex = pUIManager->GetInventory()->GetItemUniIndex(pItems0->Item_Tab, 
		pItems0->InvenIndex);
	
	// 황금 돋보기 유니 인덱스 얻기 [9/6/2012 Ranma]
	lGoldUniIndex = pUIManager->GetInventory()->GetItemUniIndex(pItems1->Item_Tab, 
		pItems1->InvenIndex);
	
	// 리폼할 아이템 유니 인덱스 얻기 [9/6/2012 Ranma]
	lReformItemUniIndex = pUIManager->GetInventory()->GetItemUniIndex(pItems2->Item_Tab, 
		pItems2->InvenIndex);
	
	// 리폼 보내기 전에 아이템 확인 [9/6/2012 Ranma]
	if (lReformerUniIndex	!= pItems0->Item_UniIndex || 
		lGoldUniIndex		!= pItems1->Item_UniIndex ||
		lReformItemUniIndex != pItems2->Item_UniIndex)
	{
		// 리폼 메시지를보내기 전에 이동으로 인한 변화를 체크 하지만 없어졌을 경우를 대비하여 다시 체크
		Close();
		return;
	}
	
	// 리폼 메시지 보내기 [9/5/2012 Ranma]	
	_pNetwork->SendReformItemReq( m_nReformType, 
		pItems0->Item_Tab, pItems0->InvenIndex,
		pItems1->Item_Tab, pItems1->InvenIndex, pItems1->Item_Sum,
		pItems2->Item_Tab, pItems2->InvenIndex, m_nNPCVIdx);
}


void CUIReformSystem::SetUIRect(const int nHeight)
{
	m_nHeightGap = nHeight;
	m_nHeight -= m_nHeightGap;
	m_rcTop.SetRect(0,0,172,37);
	m_rcMiddle.SetRect(0, 37, 172, 220);
	m_rcBottom.SetRect(0, 220 ,172, 248);

	m_rcReformSystemRect.SetRect(	0, 	0,	172, 248 );
}

void CUIReformSystem::Render()
{
	// 리폼 활성화시 일부 상호작용 거부 [9/25/2012 Ranma]
	InteractionMsgBoxReject();

	// 프로그레스바 추가 [9/12/2012 Ranma]
	_calcCmdTimer();

	m_btnConfirm.SetText( _S(5763, "리폼") );

	int nX, nY;
	GetAbsPos(nX, nY);

	CUIManager* pUiManager = CUIManager::getSingleton();

	if(m_fNPCPosX >= 0.0f && m_fNPCPosZ >= 0.0f)
	{
		FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNPCPosX;
		FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNPCPosZ;
		if( fDiffX * fDiffX + fDiffZ * fDiffZ > 100.0f )
		{
			Close();
		}
	}

	CDrawPort*	pDrawPort = pUiManager->GetDrawPort();

	pDrawPort->InitTextureData(m_ptdBaseTexture);
	pDrawPort->AddTexture( nX+m_rcTop.Left, nY+m_rcTop.Top, nX+m_rcTop.Right, nY+m_rcTop.Bottom,
		m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcMiddle.Left, nY+m_rcMiddle.Top, nX+m_rcMiddle.Right, nY+m_rcMiddle.Bottom,
		m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcBottom.Left, nY+m_rcBottom.Top, nX+m_rcBottom.Right, nY+m_rcBottom.Bottom,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1, 0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();

	// button render
	pDrawPort->InitTextureData(m_ptdButtonTexture);
	m_btnClose.Render();
	m_btnConfirm.Render();
//	m_btnConfirm.RenderHighlight(0x11111111);
	pDrawPort->FlushRenderingQueue();

	CTString strTemp;
	//COLOR	 colStrTemp;
	if(m_nReformType == 1)
	{
		strTemp =  _S(5766,"상급 리폼");
	}
	else if(m_nReformType == 0)
	{
		strTemp =  _S(5765,"중급 리폼");
	}
	pDrawPort->PutTextExCX(strTemp, m_nPosX+REFORM_UI_FRAME_WIDTH/2, m_nPosY+18, 0xDED9A0FF);
	pDrawPort->EndTextEx();
	
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	m_rtsReform.SetPos( m_nPosX + REFORM_UI_FRAME_BORDER, m_nPosY + REFORM_UI_FRAME_TITLE + REFORM_UI_FRAME_BORDER );
	m_rtsReform.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();

	// 아이템 버튼 백그라운드 회색 처리 [9/13/2012 Ranma]
	CItemData* pItemDataBotton0 = _pNetwork->GetItemData( 9005 );
	CItemData* pItemDataBotton1 = _pNetwork->GetItemData( 1839 );
	UIRectUV	rectBotton0UV, rectBotton1UV;

	rectBotton0UV.SetUV(pItemDataBotton0->GetIconTexCol() * BTN_SIZE,
		pItemDataBotton0->GetIconTexRow() * BTN_SIZE,
		(pItemDataBotton0->GetIconTexCol()+1) * BTN_SIZE,
		(pItemDataBotton0->GetIconTexRow()+1) * BTN_SIZE,
		_pUIBtnTexMgr->GetTexWidth( UBET_ITEM, pItemDataBotton0->GetIconTexID() ),
		_pUIBtnTexMgr->GetTexHeight( UBET_ITEM, pItemDataBotton0->GetIconTexID() ) );

	rectBotton1UV.SetUV(pItemDataBotton1->GetIconTexCol() * BTN_SIZE,
		pItemDataBotton1->GetIconTexRow() * BTN_SIZE,
		(pItemDataBotton1->GetIconTexCol()+1) * BTN_SIZE,
		(pItemDataBotton1->GetIconTexRow()+1) * BTN_SIZE,
		_pUIBtnTexMgr->GetTexWidth( UBET_ITEM, pItemDataBotton1->GetIconTexID() ),
		_pUIBtnTexMgr->GetTexHeight( UBET_ITEM, pItemDataBotton1->GetIconTexID() ) );

	pDrawPort->AddBtnTexture(	pItemDataBotton0->GetIconTexID(),
		nX + m_pIconsItem[eSLOT_REFORMER]->GetPosX() , nY + m_pIconsItem[eSLOT_REFORMER]->GetPosY(), 
		nX + m_pIconsItem[eSLOT_REFORMER]->GetPosX() + BTN_SIZE , nY + m_pIconsItem[eSLOT_REFORMER]->GetPosY() + BTN_SIZE,
		rectBotton0UV.U0, rectBotton0UV.V0,
		rectBotton0UV.U1, rectBotton0UV.V1, 0x404040FF );
	
	pDrawPort->AddBtnTexture(	pItemDataBotton1->GetIconTexID(),
		nX + m_pIconsItem[eSLOT_MAGNIFIER]->GetPosX() , nY + m_pIconsItem[eSLOT_MAGNIFIER]->GetPosY(), 
		nX + m_pIconsItem[eSLOT_MAGNIFIER]->GetPosX() + BTN_SIZE , nY + m_pIconsItem[eSLOT_MAGNIFIER]->GetPosY() + BTN_SIZE,
		rectBotton1UV.U0, rectBotton1UV.V0,
		rectBotton1UV.U1, rectBotton1UV.V1, 0x404040FF );
	
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );
	
	// 프로그레스 바 추가 [9/12/2012 Ranma]
	m_pbBar.Render();
	pDrawPort->FlushRenderingQueue();


	for(int i = 0; i < eSLOT_MAX; i++)
	{
		if(m_pIconsItem[i]->IsEmpty())
			continue;
		m_pIconsItem[i]->Render(pDrawPort);
	}

	m_pTooltip[0]->Render(pDrawPort);
	m_pTooltip[1]->Render(pDrawPort);
}
void CUIReformSystem::SetItemButton()
{
	for(int i = 0 ; i < eSLOT_MAX; i++)
	{
		m_pIconsItem[i]->clearIconData();
	}

	m_pIconsItem[eSLOT_REFORMER]->SetPos(REFORM_UI_FRAME_BORDER + 22, REFORM_UI_FRAME_TITLE + REFORM_UI_FRAME_BORDER + 6);
	m_pIconsItem[eSLOT_MAGNIFIER]->SetPos(REFORM_UI_FRAME_BORDER + 106, REFORM_UI_FRAME_TITLE + REFORM_UI_FRAME_BORDER + 6);
	m_pIconsItem[eSLOT_ITEM]->SetPos(REFORM_UI_FRAME_BORDER + 64, REFORM_UI_FRAME_TITLE + REFORM_UI_FRAME_BORDER + 77);

	m_pTooltip[0]->Hide(TRUE);
	m_pTooltip[1]->Hide(TRUE);
}
void CUIReformSystem::ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}
void CUIReformSystem::AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}
void CUIReformSystem::Close()
{
	m_bUseTimer = FALSE;
	m_fnReq = NULL;
	m_bProcessing = FALSE; // 프로그레스 바 추가 [9/12/2012 Ranma]
	m_fNPCPosX = 0.0f;
	m_fNPCPosZ = 0.0f;
	m_bPickTitle = FALSE;
	m_btnConfirm.SetEnable(FALSE);
	m_nReformType = 2;

	// 프로그레스바 추가 [9/12/2012 Ranma]
	m_pbBar.IdleProgress();
	m_pbBar.SetProgressPos( 0 );
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder(UI_REFORM_SYSTEM, FALSE);
	pUIManager->GetInventory()->Lock(FALSE, FALSE, LOCK_REFORMSYSTEM);
	pUIManager->GetMsgBoxNumOnly()->CloseBox();
}
WMSG_RESULT	CUIReformSystem::MouseMessage(MSG *pMsg )
{
	WMSG_RESULT wmsgResult;
	static BOOL bTitleBarClick = FALSE;
	static int nOldX, nOldY;
	int nX = LOWORD(pMsg->lParam);
	int nY = HIWORD(pMsg->lParam);

	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			if(IsInside(nX,nY))
				pUIManager->SetMouseCursorInsideUIs();

			int ndX = nX - nOldX;
			int ndY = nY - nOldY;

			if(m_bPickTitle && (pMsg->wParam & MK_LBUTTON))
			{
				nOldX = nX; nOldY = nY;
				Move(ndX,ndY);
				return WMSG_SUCCESS;
			}
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{

				if(IsInsideRect(nX,nY,m_rcReformSystemRect))
				{
					for(int i = 0; i < eSLOT_MAX; i++)
					{
						m_pIconsItem[i]->MouseMessage(pMsg);						
					}					

					return WMSG_SUCCESS;
				}

				
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			if(IsInside(nX,nY))
			{
				nOldX = nX;		nOldY = nY;
				
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				else if( m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				else if(IsInsideRect(nX,nY,m_rcTitle))
				{
					m_bPickTitle = TRUE;
				}

				pUIManager->RearrangeOrder(UI_REFORM_SYSTEM,TRUE);
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			m_bPickTitle = FALSE;
			
			if (pUIManager->GetDragIcon() == NULL)
			{
				if(!IsFocused())
					return WMSG_FAIL;
				if((wmsgResult = m_btnClose.MouseMessage(pMsg)) != WMSG_FAIL)
				{
					if(wmsgResult == WMSG_COMMAND)
					{
						Close();
					}
					return WMSG_SUCCESS;
				}
				else if((wmsgResult = m_btnConfirm.MouseMessage(pMsg)) != WMSG_FAIL)
				{
					if(wmsgResult == WMSG_COMMAND)
					{
						if(pUIManager->GetMsgBoxNumOnly()->IsOpen() == true)
						{
							// 수량 입력 박스를 띄운 채로 리폼을 실행 하는 경우를 막음 [9/5/2012 Ranma]
							return WMSG_SUCCESS;
						}
						// 프로그레스바 추가 [9/12/2012 Ranma]
						m_pbBar.IdleProgress();
						m_pbBar.SetProgressPos( 0 );
						CommandTimer(&CUIReformSystem::SendReformReq);
						setPrevData(m_pIconsItem[eSLOT_ITEM]->getItems());
						return WMSG_SUCCESS;
					}
				}
			}
			else
			{
				if(IsInside(nX,nY))
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();

					CItems* pItems = pDrag->getItems();

					if (pItems == NULL)
						return WMSG_FAIL;

					pUIManager->GetMessenger()->SetDragging(false);
					if(IsInsideRect(nX,nY,m_rcReformSystemRect))
					{
						if (pDrag->getBtnType() == UBET_ITEM)
						{
							if (m_pIconsItem[eSLOT_REFORMER]->IsInside(nX, nY) == TRUE) // 리포머 슬롯 start////
							{
								CItemData* pItemData = _pNetwork->GetItemData(pDrag->getIndex());

								if ( m_nReformType == 0 && pItemData->GetType() == CItemData::ITEM_ETC &&
									pItemData->GetSubType() == CItemData::ITEM_ETC_REFORMER && 
									pItemData->GetNum0() == CItemData::ITEM_ETC_REFORMER_MID_GRADE)
								{
									m_pIconsItem[eSLOT_REFORMER]->copyItem(pDrag);									
								}
								else if ( m_nReformType == 1 && pItemData->GetType() == CItemData::ITEM_ETC &&
									pItemData->GetSubType() == CItemData::ITEM_ETC_REFORMER && 
									pItemData->GetNum0() == CItemData::ITEM_ETC_REFORMER_HIGH_GRADE)
								{
									m_pIconsItem[eSLOT_REFORMER]->copyItem(pDrag);
								}
								else
								{
									// 에러 메시지 2번 [9/3/2012 Ranma]
									pUIManager->GetChattingUI()->AddSysMessage( _S(5757,"적절한 등급의 리포머가 아닙니다."), SYSMSG_ERROR );
									m_pIconsItem[eSLOT_REFORMER]->clearIconData();
									pUIManager->ResetHoldBtn();
									return WMSG_FAIL;
								}
								
								if(!m_pIconsItem[eSLOT_MAGNIFIER]->IsEmpty() && !m_pIconsItem[eSLOT_ITEM]->IsEmpty())
										m_btnConfirm.SetEnable(TRUE);
								
							}//// 리포머 슬롯 end

							else if (m_pIconsItem[eSLOT_MAGNIFIER]->IsInside(nX,nY)) // 황금 돋보기 슬롯 start////
							{
								CItemData* pItemData = pItems->ItemData;
								if(pItemData->GetItemIndex() == RARE_IDENTITY_ITEM2 &&
									pItemData->GetType() == CItemData::ITEM_ONCEUSE && pItemData->GetSubType() == CItemData::ITEM_SUB_ETC)
								{
									CUIMsgBoxNumericOnly* pMsgBoxNumOnly = pUIManager->GetMsgBoxNumOnly();

									if (pMsgBoxNumOnly->IsOpen() == true)
										pMsgBoxNumOnly->CloseBox();

									int nInitCount = pItems->Item_Sum;

									if (pItems->Item_Sum > 100)
										nInitCount = 100;

									CTString strMessage;
									strMessage.PrintF(_S(5756,"1회 리폼 시 몇 개의 황금 돋보기를 사용 하시겠습니까?"));

									CmdReformPutOnItem* pCmdOk = new CmdReformPutOnItem;
									pCmdOk->setData(this, true);

									CmdReformPutOnItem* pCmdCancel = new CmdReformPutOnItem;
									pCmdCancel->setData(this, false);

									pMsgBoxNumOnly->SetInfo(pCmdOk, _S(5763,"리폼"), strMessage, 1, (int)nInitCount, pCmdCancel);
									pMsgBoxNumOnly->InitData((int)nInitCount);

									{
										m_pIconsItem[eSLOT_MAGNIFIER]->cloneItem(pDrag);
										m_pIconsItem[eSLOT_MAGNIFIER]->setCount(nInitCount);
									}
								} 
								else
								{
									///< 에러 메시지
									// 황금 돋보기만 가능 [9/3/2012 Ranma]
									pUIManager->GetChattingUI()->AddSysMessage( _S(5758,"황금 돋보기가 아닙니다."), SYSMSG_ERROR );
									pUIManager->ResetHoldBtn();
								}
							}/// 황금 돋보기 슬롯 end//////

							else if(m_pIconsItem[eSLOT_ITEM]->IsInside(nX,nY)) // 장비 슬롯 start///
							{
								CTString strMessage;
								CUIMsgBox_Info msgBoxInfo;
								CItemData* pItemData = pItems->ItemData;

								// 장착 아이템 막기
								if (pDrag->IsWearTab() == true)
								{
									pUIManager->GetChattingUI()->AddSysMessage( _S(4981,"착용 중인 아이템 입니다."), SYSMSG_ERROR );
									pUIManager->ResetHoldBtn();
									return WMSG_FAIL;
								}
									// 가능 목록 무기. 방어구, 악세사리 [9/5/2012 Ranma]

								if(pItemData->GetFlag() & ITEM_FLAG_NOT_REFORM) // 리폼 가능 여부 체크 [9/11/2012 Ranma]
								{
									///< 에러 메시지 리폼 불가능 4번
									pUIManager->GetChattingUI()->AddSysMessage( _S(5759,"리폼 가능한 아이템이 아닙니다."), SYSMSG_ERROR );
									pUIManager->ResetHoldBtn();
									return WMSG_FAIL;
								}
								else
								{
									if( pItemData->GetType() == CItemData::ITEM_WEAPON
										|| pItemData->GetType() == CItemData::ITEM_SHIELD
										|| pItemData->GetType() == CItemData::ITEM_ACCESSORY )
									{
										
										if (m_nReformType == 0)
										{
											if (pItemData->IsFlag(ITEM_FLAG_RARE))
											{
												if (pItems->Item_RareIndex == 0) //미공개 레어아이템 막기
												{
													pUIManager->GetChattingUI()->AddSysMessage( _S(5771,"감정되지 않은 아이템 입니다."), SYSMSG_ERROR );
													m_pIconsItem[eSLOT_ITEM]->clearIconData();
													pUIManager->ResetHoldBtn();
													return WMSG_FAIL;
												}
											}										
											else 
											{
												pUIManager->GetChattingUI()->AddSysMessage( _S(5760,"중급 아이템이 아닙니다."), SYSMSG_ERROR );
												m_pIconsItem[eSLOT_ITEM]->clearIconData();
												pUIManager->ResetHoldBtn();
												return WMSG_FAIL;
											}
										}
										else if (m_nReformType == 1)
										{
											if (pItemData->IsFlag(ITEM_FLAG_ORIGIN))
											{
											}
											else
											{
												pUIManager->GetChattingUI()->AddSysMessage( _S(5761,"고급 아이템이 아닙니다."), SYSMSG_ERROR );
												m_pIconsItem[eSLOT_ITEM]->clearIconData();
												pUIManager->ResetHoldBtn();
												return WMSG_FAIL;
											}
										}
									}
									else
									{
										// 무기 방어구 악세서리를 제외한 다른 아이템 막기 [9/12/2012 Ranma]
										pUIManager->GetChattingUI()->AddSysMessage( _S(5759,"리폼 가능한 아이템이 아닙니다."), SYSMSG_ERROR );
										pUIManager->ResetHoldBtn();
										return WMSG_FAIL;
									}
									// 아이템 등록시
									m_pIconsItem[eSLOT_ITEM]->copyItem(pDrag);
									setPrevData(m_pIconsItem[eSLOT_ITEM]->getItems());

									if(!m_pIconsItem[eSLOT_MAGNIFIER]->IsEmpty() && !m_pIconsItem[eSLOT_REFORMER]->IsEmpty())
										m_btnConfirm.SetEnable(TRUE);
								}							
							}// 장비 슬롯 end///
						}
					}
				}
				pUIManager->ResetHoldBtn();
				return WMSG_SUCCESS;
			}
		}
	case WM_LBUTTONDBLCLK:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			if(IsInside(nX,nY))
			{
				pUIManager->RearrangeOrder(UI_REFORM_SYSTEM,TRUE);
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
		}
		break;
	}
	return WMSG_FAIL;
}

void CUIReformSystem::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
}

void CUIReformSystem::InteractionMsgBoxReject()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// 파티 초대[신청받음]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_PARTY_ALLOW))
	{
		pUIManager->GetMessageBox(MSGCMD_PARTY_ALLOW)->ReturnCommand(FALSE);
	}

	// 파티 초대[신청]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_PARTY_INVITE))
	{
		pUIManager->GetMessageBox(MSGCMD_PARTY_INVITE)->ReturnCommand(FALSE);
	}

	// 파티리콜[신청 받음]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_EX_PARTY_RECALL_PROMPT))
	{
		pUIManager->GetMessageBox(MSGCMD_EX_PARTY_RECALL_PROMPT)->ReturnCommand(FALSE);
	}

	// 교환 요청 [요청]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_EXCH_REQ_SRC))
	{
		pUIManager->GetMessageBox(MSGCMD_EXCH_REQ_SRC)->ReturnCommand(FALSE);
	}

	// 교환 요청 [요청받음]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_EXCH_REQ_DEST))
	{
		pUIManager->GetMessageBox(MSGCMD_EXCH_REQ_DEST)->ReturnCommand(FALSE);
	}

	// 길드 가입 요청 [요청받음]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_GUILD_JOIN))
	{
		pUIManager->GetMessageBox(MSGCMD_GUILD_JOIN)->ReturnCommand(FALSE);
	}

	// 길드 가입 요청 [요청]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_GUILD_JOIN_REQ))
	{
		pUIManager->GetMessageBox(MSGCMD_GUILD_JOIN_REQ)->ReturnCommand(FALSE);
	}

	// 길드 전투 요청
	if(pUIManager->DoesMessageBoxExist(MSGCMD_GUILD_BATTLE_REQ))
	{
		pUIManager->GetMessageBox(MSGCMD_GUILD_BATTLE_REQ)->ReturnCommand(FALSE);
	}

	// 길드 전투 신청 창
	if(pUIManager->DoesMessageBoxExist(MSGCMD_GUILD_BATTLE_MESSAGE))
	{
		pUIManager->GetMessageBox(MSGCMD_GUILD_BATTLE_MESSAGE)->ReturnCommand(FALSE);
	}
	
	// 길드 전투 수락창 [요청 받음]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_GUILD_BATTLE_ACCEPT))
	{
		pUIManager->GetMessageBox(MSGCMD_GUILD_BATTLE_ACCEPT)->ReturnCommand(FALSE);
	}

	// 순간이동 요청을 받았을 경우 sessionState에서 처리 
	if(pUIManager->DoesMessageBoxExist(MSGCMD_WARP_TO_REQ_CALL))
	{
		pUIManager->GetMessageBox(MSGCMD_WARP_TO_REQ_CALL)->ReturnCommand(FALSE);
	}

	// 소환 요청을 받았을 경우 sessionState에서 처리
	if(pUIManager->DoesMessageBoxExist(MSGCMD_WARP_TAKE_REQ_CALL))
	{
		pUIManager->GetMessageBox(MSGCMD_WARP_TAKE_REQ_CALL)->ReturnCommand(FALSE);
	}

	// 공성 지역 이동 확인
	if(pUIManager->DoesMessageBoxExist(MSGCMD_SIEGE_WARFARE_MOVEING_CONFIRM))
	{
		pUIManager->GetMessageBox(MSGCMD_SIEGE_WARFARE_MOVEING_CONFIRM)->ReturnCommand(FALSE);
	}

	// 후견인으로 등록 확인
	if(pUIManager->DoesMessageBoxExist(MSGCMD_HELPER_STUDENT_ACCEPT))
	{
		pUIManager->GetMessageBox(MSGCMD_HELPER_STUDENT_ACCEPT)->ReturnCommand(FALSE);
	}

	// go to royal rumble zone.
	if(pUIManager->DoesMessageBoxExist(MSGCMD_ROYALRUMBLE_GO_ZONE))
	{
		pUIManager->GetMessageBox(MSGCMD_ROYALRUMBLE_GO_ZONE)->ReturnCommand(FALSE);
	}
}


void CUIReformSystem::BtnSetItemCheckforPos()
{
	m_pTooltip[1]->Hide(FALSE);
	m_pTooltip[1]->SetItemData(m_pIconsItem[eSLOT_ITEM]->getItems());
	m_pTooltip[1]->SetPosX(GetWidth() + DEF_TOOLTIP_GAP);
}

void CUIReformSystem::SendReformItemBtnInitUniIndex(int uniindex)
{
	for(int i = 0; i < eSLOT_MAX; ++i)
	{
		if (m_pIconsItem[i]->getItems() != NULL && 
			m_pIconsItem[i]->getItems()->Item_UniIndex == uniindex)
		{
			m_pIconsItem[i]->clearIconData();
			m_btnConfirm.SetEnable(FALSE);
		}
	}
}


void CUIReformSystem::RecvReformSystemResult(CNetworkMessage* istr)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	UCHAR type;
	
	(*istr) >> type;
	
	switch( type )
	{
	case MSG_EX_REFORMER_SYSTEM_NOT_GRADE_REP:
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S(5757,"적절한 등급의 리포머가 아닙니다."), SYSMSG_ERROR );
		}
		break;
	case MSG_EX_REFORMER_SYSTEM_NOT_GOLD_MAGNIFIER_REP:
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S(5758,"황금 돋보기가 아닙니다."), SYSMSG_ERROR );
		}
		break;
	case MSG_EX_REFORMER_SYSTEM_NOT_REFORM_ITEM_REP:
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S(5759,"리폼 가능한 아이템이 아닙니다."), SYSMSG_ERROR );
			m_pIconsItem[eSLOT_ITEM]->clearIconData();
		}
		break;
	case MSG_EX_REFORMER_SYSTEM_NOT_MID_ITEM_REP:
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S(5760,"중급 아이템이 아닙니다."), SYSMSG_ERROR );
			m_pIconsItem[eSLOT_ITEM]->clearIconData();
		}
		break;
	case MSG_EX_REFORMER_SYSTEM_NOT_HIGH_ITEM_REP:
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S(5761,"고급아이템이 아닙니다."), SYSMSG_ERROR );
			m_pIconsItem[eSLOT_ITEM]->clearIconData();
		}
		break;
	case MSG_EX_REFORMER_SYSTEM_EXCEPTION_ERROR_REP:
		{
			//pUIManager->GetChattingUI()->AddSysMessage( _s("적절한 등급의 리포머가 아닙니다."), SYSMSG_ERROR );
		}
		break;
	case MSG_EX_REFORMER_SYSTEM_REFORM_SUCCESS_REP:
		{
			SWORD nTab, nIndex;
			SLONG itemIndex, itemUniIndex;
			SQUAD itemCount = 0;
			
			(*istr) >> nTab;
			(*istr) >> nIndex;
			
			(*istr) >> itemUniIndex;
			(*istr) >> itemIndex;

			if (IsVisible() == FALSE)
				return;

			// 아이템버튼들 갱신
			
			// 리폼아이템 갱신
			m_pTooltip[1]->SetItemData(m_pIconsItem[eSLOT_ITEM]->getItems());
			
			// 리포머 갱신
			CItems*		pItems = pUIManager->GetInventory()->GetItems(m_pIconsItem[eSLOT_REFORMER]->getItems()->Item_UniIndex);
			
			if (pItems == NULL || pItems->IsEmptyItem())
			{
				m_pIconsItem[eSLOT_REFORMER]->clearIconData();
			}
			else
			{
				m_pIconsItem[eSLOT_REFORMER]->setCount(pItems->Item_Sum);
			}

			pItems = pUIManager->GetInventory()->GetItems(m_pIconsItem[eSLOT_MAGNIFIER]->getItems()->Item_UniIndex);
			
			// 황금 돋보기 갱신
			if (pItems == NULL || pItems->IsEmptyItem())
			{
				m_pIconsItem[eSLOT_MAGNIFIER]->clearIconData();
			}
			else
			{
				itemCount = pItems->Item_Sum;
				
				if (itemCount < m_pIconsItem[eSLOT_MAGNIFIER]->getItems()->Item_Sum)
				{
					m_pIconsItem[eSLOT_MAGNIFIER]->clearIconData();
				}
			}
			// 프로그레스바 추가 [9/12/2012 Ranma]
			CompleteProgress();
			m_bProcessing = FALSE;
			m_pTooltip[1]->Hide(FALSE);

			//팝업창 위치 셋팅 (세트 아이템 인지 체크) 
			BtnSetItemCheckforPos();

			// 리폼 버튼 활성화 여부 [9/5/2012 Ranma]
			if ( m_pIconsItem[eSLOT_REFORMER]->IsEmpty() || m_pIconsItem[eSLOT_MAGNIFIER]->IsEmpty() || m_pIconsItem[eSLOT_ITEM]->IsEmpty() )
			{
				// 리폼버튼 비활성 [9/5/2012 Ranma]
				m_btnConfirm.SetEnable(FALSE);
			}
			else
			{
				m_btnConfirm.SetEnable(TRUE);
			}
		}
		break;
	}
	
}

void CUIReformSystem::setPrevData( CItems* pItems )
{
	if (pItems == NULL)
		return;

	SAFE_DELETE(m_pItemsPrev);
	
	m_pItemsPrev = new CItems;
	memcpy(m_pItemsPrev, pItems, sizeof(CItems));

	m_pTooltip[0]->SetItemData(m_pItemsPrev);
	m_pTooltip[0]->SetPosX(-m_pTooltip[0]->GetWidth() - DEF_TOOLTIP_GAP);

	m_pTooltip[0]->Hide(FALSE);
	m_pTooltip[1]->Hide(TRUE);
}

void CUIReformSystem::MagnifierPutOnCallBack()
{
	SQUAD llData = UIMGR()->GetMsgBoxNumOnly()->GetData();

	if(llData && llData <= 100 && llData > 0)
	{
		if (llData <= m_pIconsItem[eSLOT_MAGNIFIER]->getItems()->Item_Sum)
		{
			// 유저가 입력한 갯수 받아서 표시
			m_pIconsItem[eSLOT_MAGNIFIER]->setCount(llData);
		}
	}
	else
	{
		///< 에러 처리
		UIMGR()->GetChattingUI()->AddSysMessage( _S(5762,"황금 돋보기 1회 사용 가능 수량은 1~100 입니다."), SYSMSG_ERROR );
		m_pIconsItem[eSLOT_MAGNIFIER]->clearIconData();
		return;
	}

	// 만약에 리포머 버튼과 장비 버튼이 채워져 있다면 리폼 버튼 활성화
	if(!m_pIconsItem[eSLOT_REFORMER]->IsEmpty() && !m_pIconsItem[eSLOT_ITEM]->IsEmpty())
		m_btnConfirm.SetEnable(TRUE);

	if (m_pIconsItem[eSLOT_MAGNIFIER]->IsEmpty())
		m_pIconsItem[eSLOT_MAGNIFIER]->setCount(llData);
}

void CUIReformSystem::MagnifierPutOnCancelCallBack()
{
	if (m_pIconsItem[eSLOT_MAGNIFIER]->IsEmpty() == true)
		return;

	m_pIconsItem[eSLOT_MAGNIFIER]->clearIconData();
	m_btnConfirm.SetEnable(FALSE);
}
