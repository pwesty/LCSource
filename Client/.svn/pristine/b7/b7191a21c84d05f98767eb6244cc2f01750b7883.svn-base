#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "ItemComposeUI.h"
#include "ItemCompose.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/UIMsgBoxMgr.h>
#include <Engine/Help/Util_Help.h>

#define DEF_END_TIME 2

class CmdItemComposeClose : public Command
{
public:
	CmdItemComposeClose() : m_pWnd(NULL) {}
	void setData( CItemComposeUI* pWnd ) { m_pWnd = pWnd; }
	void execute()
	{
		if( m_pWnd )
			m_pWnd->closeUI();
	}
private:
	CItemComposeUI* m_pWnd;
};

class CmdItemComposeOk : public Command
{
public:
	CmdItemComposeOk() : m_pWnd(NULL) {}
	void setData( CItemComposeUI* pWnd ) { m_pWnd = pWnd; }
	void execute()
	{
		if( m_pWnd )
			m_pWnd->PressOK();
	}
private:
	CItemComposeUI* m_pWnd;
};

CItemComposeUI::CItemComposeUI(void)
	: m_bDrag(false)
	, m_pDrag(NULL)
	, m_nOriX(0), m_nOriY(0)
	, m_pStrTitle(NULL)
	, m_pTbStateGuide(NULL)
	, m_pPgBar(NULL)
	, m_eState(eNONE)
	, m_llNas(0)
	, m_uStartTime(0)
	, m_uEndTime(DEF_END_TIME)
	, m_bProgress(FALSE)
{
	int i;
	for (i = 0; i < eICON_MAX; i++)
	{
		m_pIcon[i] = NULL;
		m_pIconBg[i] = NULL;
	}
	for (i = 0; i < eBTN_MAX; i++)
	{
		m_pBtn[i] = NULL;
	}
	
	setInherit(false);
}

void CItemComposeUI::openUI()
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	// If inventory is already locked
	if (pUIMgr->GetInventory()->IsLocked() == TRUE ||
		pUIMgr->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	if (IsVisible() != FALSE)
		return;

	SetVisible(TRUE);
	Hide(FALSE);

	SetPos( m_rcOriginPos.Left, m_rcOriginPos.Top );
	ClearIcon();
	SetState(eNONE);	
	
	pUIMgr->RearrangeOrder(UI_ITEM_COMPOSE, TRUE);
	pUIMgr->GetInventory()->Lock(TRUE, TRUE, LOCK_ITEM_COMPOSE);
}

void CItemComposeUI::closeUI()
{
	SetVisible(FALSE);
	Hide(TRUE);

	ClearIcon();

	m_bProgress = FALSE;
	m_uStartTime = 0;

	if (m_pPgBar != NULL)
		m_pPgBar->SetProgressPos(0);

	CUIManager* pUIMgr = CUIManager::getSingleton();
	pUIMgr->RearrangeOrder(UI_ITEM_COMPOSE, FALSE);
	pUIMgr->GetInventory()->Lock(FALSE, FALSE, LOCK_ITEM_COMPOSE);
}

void CItemComposeUI::UpdateResultIcon(int ItemIdx)
{
	CItemData* pItemData = CItemData::getData(ItemIdx);

	ClearIcon();

	if (pItemData == NULL || ItemIdx <= 0)
	{
		SetState(eNONE);
		ShowGuideString(eGUIDE_COMPOSE_FAIL);
		return;
	}

	SetState(eCOMPLETE);

	if (m_pIcon[eICON_RESULT] != NULL)
	{
		m_pIcon[eICON_RESULT]->setData(UBET_ITEM, ItemIdx);
		m_pIcon[eICON_RESULT]->Hide(FALSE);
	}
}

void CItemComposeUI::SendEvent()
{
	if (m_pIcon[eICON_MAT1] == NULL)
		return;

	CItemCompose* pData = GAMEDATAMGR()->GetItemCompose();

	if(pData == NULL)
		return;

	int i;
	int nMatIdx = m_pIcon[eICON_MAT1]->getIndex();
	stSendComposeInfo senddata;	

	for (i = 0; i < eICON_MAX - 1; ++i)
	{
		if (m_pIcon[i] == NULL)
			continue;

		CItems* pItem = m_pIcon[i]->getItems();

		if (pItem == NULL)
			continue;

		senddata.tab[i] = pItem->Item_Tab;
		senddata.invenIdx[i] = pItem->InvenIndex;
		senddata.virIdx[i] = pItem->Item_UniIndex;
	}

	// 서버로 합성 메시지 전송
	pData->SendComposeReq(nMatIdx, senddata);
}

void CItemComposeUI::PressOK()
{
	// Ok 버튼 락걸기
	if( SetBtnState(eBTN_OK, FALSE) == false )
		return;

	StartProgress();
	SetState(ePROGRESSING);
}

void CItemComposeUI::ClearIcon()
{
	for (int i = 0; i < eICON_MAX; ++i)
	{
		if (m_pIcon[i] != NULL)
		{
			m_pIcon[i]->clearIconData();
			m_pIcon[i]->Hide(TRUE);
		}

		if (m_pIconBg != NULL)
		{
			m_pIconBg[i]->Hide(TRUE);
		}
	}
}

void CItemComposeUI::ShowErrorPopup( int nErrorIdx )
{
	CTString strCaption = "";
	CTString strTemp = "";

	switch(nErrorIdx)
	{
	case eCOMPOSE_POPUP_DONT_IN_DUNGEON:
		strTemp.PrintF(_S(6410, "유물 아이템을 보유한 상태로 던전은 입장할 수 없습니다."));
		break;
	case eCOMPOSE_POPUP_NOT_ENOUGH_NAS:
		strTemp.PrintF(_S(306, "나스가 부족합니다."));
		break;
	case eCOMPOSE_POPUP_NOT_ENOUGH_MAT:
		strTemp.PrintF(_S(6431, "합성 재료가 부족합니다."));
		break;
	case eCOMPOSE_POPUP_HAVE_NOT_RELIC:
		strTemp.PrintF(_S(6412, "PVP가 불가능한 캐릭터는 유물 아이템을 획득 할 수 없습니다."));
		break;
	case eCOMPOSE_POPUP_NO_COMPOSE_ITEM:
		strTemp.PrintF(_S(6418, "합성 가능 아이템이 아닙니다."));
		break;
	case eCOMPOSE_POPUP_EQUIP_ITEM:
		strTemp.PrintF(_S(2898, "장착하지 않은 아이템을 올려주세요."));
		break;
	case eCOMPOSE_POPUP_COMPOSE_FAIL:
		strTemp.PrintF(_S(6434, "합성이 불가능한 조합입니다."));
		break;
	default:
		return;
	}
	strCaption.PrintF(_S(191,"확인"));

	MSGBOX_OK(strCaption, strTemp);
}

void CItemComposeUI::ShowGuideString(int nIdx)
{
	if (m_pTbStateGuide == NULL)
		return;

	CTString strTemp = "";

	switch(nIdx)
	{
	case eGUIDE_BASE:
		strTemp.PrintF(_S(6417, "합성 할 아이템을 올려주세요"));
		break;
	case eGUIDE_ALL_READY:
		strTemp.PrintF(_S(6430, "합성을 진행할 수 있습니다."));
		break;
	case eGUIDE_NOT_ENOUGH_NAS:
		strTemp.PrintF(_S(6432, "나스가 부족합니다."));
		break;
	case eGUIDE_COMPOSE_OK:
		strTemp.PrintF(_S(6433, "합성에 성공 하였습니다."));
		break;
	case eGUIDE_COMPOSE_FAIL:
		strTemp.PrintF(_S(6434, "합성에 실패 하였습니다."));
		break;
	default:
		break;
	}
	// 가이드 셋팅
	m_pTbStateGuide->SetText(strTemp);
}

void CItemComposeUI::initialize()
{
	// drag area
	m_pDrag = (CUIImage*)findUI("img_move");

	// title text
	m_pStrTitle = (CUIText*)findUI("str_title");
	
	// icon
	CTString strStringIcon;
	CTString strStringIconBG;

	int		i;

	for (i = 0; i < eICON_MAX; i++)
	{
		strStringIcon.PrintF("icon_slot0%d", i+1);
		strStringIconBG.PrintF("img_iconbg0%d", i+1);

		m_pIcon[i] = (CUIIcon*)findUI(strStringIcon);
		m_pIconBg[i] = (CUIImage*)findUI(strStringIconBG);

		if (m_pIcon[i] != NULL)
			m_pIcon[i]->clearIconData();
	}

	// guid
	m_pTbStateGuide = (CUITextBox*)findUI("tb_guide");

	// progress bar
	m_pPgBar = (CUIProgressBar*)findUI("pb_progress");

	if (m_pPgBar != NULL)
		m_pPgBar->OffsetProgressPos(0);

	// Btn
	CTString strStringBtn[eBTN_MAX] = { "btn_close", "btn_ok", "btn_cancel" };

	for (i = 0; i < eBTN_MAX; i++)
	{
		m_pBtn[i] = (CUIButton*)findUI(strStringBtn[i]);
	}

	if (m_pBtn[eBTN_CLOSE] != NULL)
	{
		CmdItemComposeClose* pCmd = new CmdItemComposeClose;
		pCmd->setData(this);
		m_pBtn[eBTN_CLOSE]->SetCommandUp(pCmd);
	}

	if (m_pBtn[eBTN_CANCEL] != NULL)
	{
		CmdItemComposeClose* pCmd = new CmdItemComposeClose;
		pCmd->setData(this);
		m_pBtn[eBTN_CANCEL]->SetCommandUp(pCmd);
	}

	if (m_pBtn[eBTN_OK] != NULL)
	{
		CmdItemComposeOk* pCmd = new CmdItemComposeOk;
		pCmd->setData(this);
		m_pBtn[eBTN_OK]->SetCommandUp(pCmd);
	}
}

void CItemComposeUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	CalcProgressTimer();
}

WMSG_RESULT CItemComposeUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pDrag && m_pDrag->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder(UI_ITEM_COMPOSE, TRUE);
	return WMSG_FAIL;
}

WMSG_RESULT CItemComposeUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager* pUIMgr = CUIManager::getSingleton();

	if (m_eState != ePROGRESSING && IsInside(x, y) == TRUE)
	{
		if (CUIIcon* pDrag = pUIMgr->GetDragIcon())
		{
			SlotRegistCheck();
			pUIMgr->ResetHoldBtn();
			return WMSG_SUCCESS;
		}
	}
	
	pUIMgr->ResetHoldBtn();
	return WMSG_FAIL;
}

WMSG_RESULT CItemComposeUI::OnRButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_eState != ePROGRESSING)
	{
		for (int i = 0; i < eICON_MAX - 1; ++i)
		{
			if (m_pIcon[i] != NULL && m_pIcon[i]->IsInside(x, y))
			{
				m_pIcon[i]->clearIconData();
				if (i == eICON_MAT1)
				{
					ClearIcon();
				}
				CheckOkBtn();
				UpdateIconBg();
				UpdateNasString();
				return WMSG_SUCCESS;
			}
		}
	}

	return WMSG_FAIL;
}

WMSG_RESULT CItemComposeUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if( m_bDrag && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int ndX = x - m_nOriX;
		int ndY = y - m_nOriY;

		m_nOriX = x;
		m_nOriY = y;

		Move( ndX, ndY );
		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	return WMSG_FAIL;
}

void CItemComposeUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
	m_rcOriginPos.SetRect(m_nPosX, m_nPosY, m_nWidth, m_nHeight);
}

void CItemComposeUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CItemComposeUI::SetState( eSTATE eState )
{
	int nGuideIdx = -1;
	m_llNas = CalcConsumNas();

	switch(eState)
	{
	case eNONE:
		{
			SetBtnState(eBTN_OK, FALSE);

			if (m_pIcon[eICON_RESULT] != NULL)
				m_pIcon[eICON_RESULT]->clearIconData();

			nGuideIdx = eGUIDE_BASE;
		}		
		break;

	case eREGIST:
		{
			SetBtnState(eBTN_OK, TRUE);
			nGuideIdx = eGUIDE_ALL_READY;
		}
		break;

	case eCOMPLETE:
		{
			nGuideIdx = eGUIDE_COMPOSE_OK;
		}
		break;
	}

	m_eState = eState;
	ShowGuideString(nGuideIdx);
	UpdateNasString();	
}

bool CItemComposeUI::SetBtnState( eBTN_TYPE eType, BOOL bEnable )
{
	if (eType < 0 || eType >= eBTN_MAX)
		return false;

	if (m_pBtn[eType] == NULL)
		return false;

	m_pBtn[eType]->SetEnable(bEnable);
	return true;
}

void CItemComposeUI::CalcProgressTimer()
{
	if( m_bProgress )
	{
		__int64 tmpTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		
		if( tmpTime - m_uStartTime > ( m_uEndTime * 1000 ) )
		{	
			// 이벤트 전송
			SendEvent();
			m_bProgress = FALSE;

			if (m_pPgBar != NULL)
				m_pPgBar->CompleteProgress();
		}
		else
		{
			if (m_pPgBar != NULL)
				m_pPgBar->OffsetProgressPos( (tmpTime - m_uStartTime) );	
		}
	}
}

void CItemComposeUI::StartProgress()
{
	m_bProgress = TRUE;
	
	if (m_pPgBar != NULL)
	{
		m_pPgBar->SetProgressPos( 0 );
		m_pPgBar->StartProgress();
	}

	if (m_pIcon[eICON_RESULT] != NULL)
	{
		m_pIcon[eICON_RESULT]->clearIconData();
	}

	m_uStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
}

void CItemComposeUI::SlotRegistCheck()
{
	CUIIcon* pDrag = UIMGR()->GetDragIcon();

	if (pDrag == NULL)
		return;

	int nItemIdx = pDrag->getIndex();

	if (pDrag->IsWearTab() == true)
	{
		ShowErrorPopup(eCOMPOSE_POPUP_EQUIP_ITEM);
		return;
	}

	if (IsComposeItem(nItemIdx) == false)
	{
		ShowErrorPopup(eCOMPOSE_POPUP_NO_COMPOSE_ITEM);
		return;
	}

	for(int i = 0; i < eICON_MAX; ++i)
	{
		if (m_pIcon[i] == NULL)
			return;

		// 빈슬롯을 찾고
		if (IsCheckIcon(i) == true)
		{
			// 등록가능한지 체크한다
			if (IsRegist(i, nItemIdx) == false)
				break;

			if (IsAlreadyRegist(pDrag->getItems()->Item_UniIndex) == false)
				break;

			// 등록 가능하면 등록한다.
			m_pIcon[i]->copyItem(pDrag);
			m_pIcon[i]->Hide(FALSE);
			UpdateIconBg();
			CheckOkBtn();
			break;
		}
	}
}

void CItemComposeUI::CheckOkBtn()
{
	int nFindIdx = 0;
	int nMatCount = 0;
	LONGLONG nComposeNas = 0;
	
	if (IsCheckIcon(eICON_MAT1) == true)
	{
		SetState(eNONE);
		return;
	}

	nFindIdx = m_pIcon[eICON_MAT1]->getIndex();

	// 등록된 아이템의 필요 갯수와 소모 나스 얻기
	if (nFindIdx > 0)
	{
		CItemCompose* pCompose = GAMEDATAMGR()->GetItemCompose();

		if (pCompose != NULL)
		{
			nMatCount = pCompose->GetComposeMatCount(nFindIdx);
			nComposeNas = pCompose->GetComposeNas(nFindIdx);
		}
	}
	else
	{
		return;
	}

	int nRegistCount = 0;

	// 재료 갯수 만큼 등록되어 있는지 검사
	for (int i = 0; i < eICON_MAX - 1; ++ i)
	{
		if (m_pIcon[i] == NULL)
			continue;

		if (m_pIcon[i]->IsEmpty() == true)
		{
			continue;
		}
		else
		{
			// 등록된 재료 검사
			if(nFindIdx == m_pIcon[i]->getIndex())
			{
				nRegistCount++;
			}
		}
	}

	if (nRegistCount == nMatCount)
	{
		if (_pNetwork->MyCharacterInfo.money >= nComposeNas)
		{
			SetState(eREGIST);
		}
		else
		{
			ShowGuideString(eGUIDE_NOT_ENOUGH_NAS);
		}
	}
	else
	{
		SetState(eNONE);
	}
}

void CItemComposeUI::UpdateIconBg()
{
	int		i;

	for (i = 0; i < eICON_MAX; ++i)
	{
		if (m_pIconBg[i] == NULL)
			return;

		m_pIconBg[i]->Hide(TRUE);
	}

	UtilHelp* pHelp = UTIL_HELP();

	if (pHelp == NULL)
		return;

	// 재료 아이템이 비었는지 검사
	bool bFirstEmpty = IsCheckIcon(eICON_MAT1);

	if (bFirstEmpty == true)
		return;

	int nItemIdx = m_pIcon[eICON_MAT1]->getIndex();
	CItemData* pMatItem = CItemData::getData(nItemIdx);

	if (pMatItem == NULL || nItemIdx <= 0)
		return;

	int nMatCount = 0;
	int nResultItemIdx = 0;
	// 등록된 아이템의 필요 갯수 얻기
	CItemCompose* pCompose = GAMEDATAMGR()->GetItemCompose();

	if (pCompose != NULL)
	{
		nMatCount = pCompose->GetComposeMatCount(nItemIdx);
		nResultItemIdx = pCompose->GetComposeResultItem(nItemIdx);
	}

	if (nMatCount <= 0 || nResultItemIdx <= 0)
		return;
	
	// 재료 실루엣
	COLOR cor = pHelp->GetColorAlpha(0.5f, DEF_UI_COLOR_WHITE);
	UIRectUV uv;
	int nTexID = pMatItem->GetIconTexID();
	pHelp->GetBtnTextureInfo(UBET_ITEM, nTexID, pMatItem->GetIconTexRow(), pMatItem->GetIconTexCol(), uv);
	
	for (i = 1; i < nMatCount; ++i)
	{
		m_pIconBg[i]->setTexData(_pUIBtnTexMgr->GetTex( UBET_ITEM, nTexID ));
		m_pIconBg[i]->SetUV(uv);
		m_pIconBg[i]->SetAlpha(cor);
		m_pIconBg[i]->Hide(FALSE);
	}

	CItemData* pResultItem = CItemData::getData(nResultItemIdx);

	if (pResultItem == NULL)
		return;

	// 결과 아이템 실루엣
	nTexID = pResultItem->GetIconTexID();
	pHelp->GetBtnTextureInfo(UBET_ITEM, nTexID, pResultItem->GetIconTexRow(), pResultItem->GetIconTexCol(), uv);

	m_pIconBg[eICON_RESULT]->setTexData(_pUIBtnTexMgr->GetTex( UBET_ITEM, nTexID ));
	m_pIconBg[eICON_RESULT]->SetUV(uv);
	m_pIconBg[eICON_RESULT]->SetAlpha(cor);
	m_pIconBg[eICON_RESULT]->Hide(FALSE);
}

void CItemComposeUI::UpdateNasString()
{
	CTString strTemp;
	COLOR color = 0xffffffff;

	strTemp.PrintF("%I64d", m_llNas);
	UIMGR()->InsertCommaToString( strTemp );

	if (m_llNas > _pNetwork->MyCharacterInfo.money)
	{
		color = 0xA0A0A0ff;
	}

	if (m_pPgBar != NULL)
	{
		if (m_eState == eREGIST || m_eState == eNONE)
		{
			m_pPgBar->IdleProgress();
			m_pPgBar->SetProgressPos(0);
		}

		m_pPgBar->setProgressText( PR_STAT_IDLE, strTemp, 0, color );
		m_pPgBar->setProgressText( PR_STAT_DOING, CTString("") );
		m_pPgBar->setProgressText( PR_STAT_COMPLETE, CTString("") );
	}
}

bool CItemComposeUI::IsCheckIcon(int nIndex)
{
	if (m_pIcon[nIndex] == NULL)
		return true;

	return m_pIcon[nIndex]->IsEmpty();
}

bool CItemComposeUI::IsComposeItem(int nIdx)
{
	CItemData* pItemData = CItemData::getData(nIdx);

	if (pItemData == NULL)
		return false;

	if ( !pItemData->IsFlag( ITEM_FLAG_COMPOSE ) )
		return false;

	return true;
}

bool CItemComposeUI::IsRegist( int nSlotIdx, int nItemIdx )
{
	if (nSlotIdx == eICON_MAT1)
	{
		return true;
	}
	else
	{
		if ( IsCheckIcon(eICON_MAT1) == true )
			return false;
	}
	
	int nFindIdx = m_pIcon[eICON_MAT1]->getIndex();

	if (nFindIdx <= 0 || nItemIdx != nFindIdx)
		return false;

	int nMatCount = 0;

	// 등록된 아이템의 필요 갯수 얻기
	CItemCompose* pCompose = GAMEDATAMGR()->GetItemCompose();

	if (pCompose != NULL)
		nMatCount = pCompose->GetComposeMatCount(nFindIdx);

	if (nMatCount <= 0 || nSlotIdx >= nMatCount)
		return false;

	return true;
}

LONGLONG CItemComposeUI::CalcConsumNas()
{
	if (m_pIcon[eICON_MAT1] == NULL)
		return 0;

	int nItemIdx = m_pIcon[eICON_MAT1]->getIndex();

	if (nItemIdx <= 0)
		return 0;

	return GAMEDATAMGR()->GetItemCompose()->GetComposeNas(nItemIdx);
}

bool CItemComposeUI::IsAlreadyRegist( int nVirIndex )
{
	for (int i = 0; i < eICON_MAX - 1; ++i)
	{
		if (IsCheckIcon(i) == false)
		{
			if (m_pIcon[i] == NULL)
				return false;

			CItems* pItem = m_pIcon[i]->getItems();

			if (pItem == NULL)
				return false;

			if (pItem->Item_UniIndex == nVirIndex)
				return false;
		}
		else
		{
			// 빈슬롯을 찾을때까지만 돈다.
			break;
		}
	}

	return true;
}
