#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <../Common/CommonDef.h>
#include <../Common/Packet/ptype_tradeAgent_system.h>
#include "UIAuctionNew.h"

#include <Engine/Network/CNetwork.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Auction.h>
#include <Engine/Interface/UICommand.h>
#include <Engine/Util.h>
#include <Engine/Help/Util_Help.h>
#include "AuctionCmd.h"
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
//#include <Engine/Interface/UIWindowHeader.h>
//#include <Engine/Interface/UILayer.h>

//---------------------------------------------------------------------

#define AUCTION_NAS_MAX 999999999999	//입력가능 나스 최대 금액

//IMPLEMENT_CREATE_UI(CUIAuctionNew, eUI_NEWTYPE_AUCTION, eUILAYER_MIDDLE, auction.xml);

CUIAuctionNew::CUIAuctionNew()
	: m_pAuction(NULL)
	, m_nCurSelItem(-1)
	, m_fNpcX(FLT_MAX)
	, m_fNpcZ(FLT_MAX)	
	, m_pMainTab(NULL)
	, m_pTxtTitle(NULL)
	, m_nBuyIndex(-1)
	, m_pImgFavoriteItem(NULL)
	, m_pSearchTab(NULL)		// Search
	, m_nCurSearchSel(-1)
	, m_pListSearchRes(NULL)
	, m_pListSearchResEtc(NULL)
	, m_pPrev(NULL)
	, m_pNext(NULL)
	, m_pTxtNas(NULL)
	, m_pBtnRegFav(NULL)
	, m_pBtnBuy(NULL)
	, m_nDepositMoney(0)		// regist
	, m_nFee(0)
	, m_nQty(0)
	, m_nPrice(0)
	, m_pIconRegist(NULL)
	, m_pListRegist(NULL)
	, m_pListRegistResult(NULL)
	, m_pEditQuantity(NULL)
	, m_pEditPrice(NULL)
	, m_pTxtFee(NULL)
	, m_pTxtDeposit(NULL)
	, m_pTxtRegCount(NULL)
	, m_pBtnRegist(NULL)
	, m_pBtnRegCancel(NULL)
	, m_pTxtRegNas(NULL)
	, m_pListFavorite(NULL)		// Favorite
	, m_pTxtFavNas(NULL)
	, m_pTxtFavCount(NULL)
	, m_pBtnFavCancel(NULL)
	, m_pBtnFavBuy(NULL)
	, ox(0), oy(0)
	, m_bTitleClick(false)
	, m_pTxtPage(NULL)
	, m_pDropArea(NULL)
	, m_pTbPremium(NULL)
{
	setInherit(false);		// 창 안에 들어온 MsgProc 창에서 소화. 다른데 상속 안함

	int		type, i;

	for (type = 0; type < 2; ++type)
	{
		for (i = 0; i < eEDIT_TYPE_MAX; ++i)
		{
			m_pEdit[type][i] = NULL;
		}
	}

	for (type = 0; type < 2; ++type)
	{
		for (i = 0; i < eCOMBO_TYPE_MAX; ++i)
		{
			m_pCombo[type][i] = NULL;
		}
	}

	for (i = 0; i < eEDIT_ETC_TYPE_MAX; ++i)
		m_pEditEtc[i] = NULL;

	m_pAuction = GameDataManager::getSingleton()->GetAuction();
}

CUIAuctionNew::~CUIAuctionNew()
{
	Destroy();

	m_pAuction = NULL;		// GameDataManager에서 관리.
}

void CUIAuctionNew::initialize()
{
	Auction* pAuction = GameDataManager::getSingleton()->GetAuction();

	m_pMainTab = (CUITab*)findUI( "tab_" );
	{
		CmdChangeMainTab* pCmd = new CmdChangeMainTab;
		pCmd->setData(this);
		m_pMainTab->SetCommand(pCmd);
	}

	{
		// 닫기 버튼 명령 연결
		CUIButton* pButton = (CUIButton*)findUI("btn_close");
		if (pButton)
		{
			CmdAuctionClose* pCmd = new CmdAuctionClose;
			pCmd->setData(this);
			pButton->SetCommandUp( pCmd );
		}
	}

	m_pTxtTitle = (CUIText*)findUI("str_title");

	m_pImgFavoriteItem = (CUIImage*)findUI("img_favorite_item");

	initSearch();
	initRegist();
	initFavorite();
}

void CUIAuctionNew::reset()
{
	// 검색 목록은 지워야한다.
	int		i,	nMax = m_pListSearchRes->getListItemCount();
	CUIListItem* pItem;

	for (i = 0; i < nMax; ++i)
	{
		pItem = (CUIListItem*)m_pListSearchRes->GetListItem(i);
		if (pItem != NULL)
			pItem->Hide(TRUE);
	}

	nMax = m_pListSearchResEtc->getListItemCount();
	
	for (i = 0; i < nMax; ++i)
	{
		pItem = (CUIListItem*)m_pListSearchResEtc->GetListItem(i);
		if (pItem != NULL)
			pItem->Hide(TRUE);
	}
}

//---------------------------------------------------------------------

void CUIAuctionNew::OpenAuction( int nNpcIndex, FLOAT fNpcPosX, FLOAT fNpcPosZ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager == NULL)
	{
		ASSERT(false);
		return;
	}

	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_AUCTION ) || IsVisible() )	
		return;

	m_fNpcX = fNpcPosX;
	m_fNpcZ = fNpcPosZ;

	pUIManager->CreateMessageBoxL(_S(4287, "거래 대행 서비스"), UI_AUCTION, MSGLCMD_AUCTION);
	CTString strNpcName = CMobData::getData(nNpcIndex)->GetName();

	pUIManager->AddMessageBoxLString(MSGLCMD_AUCTION, TRUE, strNpcName, -1, 0xE18600FF);
	pUIManager->AddMessageBoxLString(MSGLCMD_AUCTION, TRUE, _S(4352, "상인의 후예 길드는 대륙에서 가장 안전하고 빠른 거래 대행 서비스를 소정의 수수료를 받고 제공하고 있다."), -1, 0xa3a1a3ff);
	pUIManager->AddMessageBoxLString(MSGLCMD_AUCTION, TRUE, _s(" "));
	pUIManager->AddMessageBoxLString(MSGLCMD_AUCTION, TRUE, _S(4353, "거래를 맡기고 싶은 물건이 있는 것인가?"), -1, 0xa3a1a3ff);
	pUIManager->AddMessageBoxLString(MSGLCMD_AUCTION, TRUE, _s(" "));

	CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_QUEST_REQ);

	pUIManager->AddMessageBoxLString(MSGLCMD_AUCTION, FALSE, _S(4354, "거래 대행 서비스를 이용한다."), eMSG_COMMAND_USE_AUCTION);
	pUIManager->AddMessageBoxLString(MSGLCMD_AUCTION, FALSE, _S( 1748, "NPC 안내" ), NPC_HELP);
	pUIManager->AddMessageBoxLString(MSGLCMD_AUCTION, FALSE, _S( 1220, "취소한다." ) );	
}

void CUIAuctionNew::UseAuction()
{
	if( IsVisible() == TRUE)
		return;

	m_bHide = FALSE;
	SetVisible(TRUE);

	// 처음이라면...
	if (m_pAuction != NULL)
	{
		if (m_pAuction->get1stRecvRegList() == false)
			m_pAuction->SendRegistListReq();

		if (m_pAuction->get1stRecvFavoriteList() == false)
			m_pAuction->SendFavoriteListReq();

		if (m_pAuction->getRegUpdate())
		{
			m_pAuction->setRegUpdate(false);
			updateRegistList();
		}
	}

	updateNas();

	CUIManager::getSingleton()->GetInventory()->Lock(TRUE, TRUE, LOCK_AUCTION);

	CUIManager::getSingleton()->RearrangeOrder(UI_AUCTION, TRUE);
}

void CUIAuctionNew::CloseAuction()
{
	CloseAuctionMessageBox();
	//m_ebSearch.SetFocus(FALSE);

	m_bHide = TRUE;
	SetVisible(FALSE);

	CUIManager::getSingleton()->RearrangeOrder(UI_AUCTION, FALSE);
	CUIManager::getSingleton()->GetInventory()->Lock(FALSE, FALSE, LOCK_AUCTION);

	CUIFocus::getSingleton()->setUI(NULL);	// 창을 닫을 때 포커스가 있다면, 초기화

	clearRegInput();

	m_pMainTab->OnChangeTab(0);
	m_pSearchTab->OnChangeTab(0);

	changeSearchnTab();

	clearInput();

	CUIBase::CloseProc();
}

BOOL CUIAuctionNew::CloseWindowByEsc()
{
	CloseAuction();
	CUITooltipMgr::getSingleton()->clearTooltip();

	return TRUE;
}

void CUIAuctionNew::changeMainTab()
{
	int nSel = m_pMainTab->getCurSelTab();

	switch (nSel)
	{
	case 0:
		{
			m_pListSearchRes->SetItemShowNum(0);
			m_pListSearchRes->UpdateList();
			m_pListSearchResEtc->SetItemShowNum(0);
			m_pListSearchResEtc->UpdateList();	

			int	nSel = m_pSearchTab->getCurSelTab();
			clearSearch((eRESULT_TYPE)(eRESULT_TYPE_SEARCH_ARMOR + nSel));

			m_pListSearchRes->setCurSel(-1);
			m_pListSearchResEtc->setCurSel(-1);
			m_pBtnRegFav->SetEnable(FALSE);
			m_pBtnBuy->SetEnable(FALSE);

			clearInput();
		}
		break;
	case 1:
		{
			m_pBtnRegCancel->SetEnable(FALSE);
		}
		break;
	case 2:
		{
			m_pBtnFavCancel->SetEnable(FALSE);
			m_pBtnFavBuy->SetEnable(FALSE);
		}
		break;
	}	
}

void CUIAuctionNew::changeSearchnTab()
{
	int	nSel = m_pSearchTab->getCurSelTab();

	if (nSel < 2)
	{
		m_pListSearchRes->Hide(FALSE);
		m_pListSearchResEtc->Hide(TRUE);
	}
	else
	{
		m_pListSearchRes->Hide(TRUE);
		m_pListSearchResEtc->Hide(FALSE);
	}

	m_pListSearchRes->SetItemShowNum(0);
	m_pListSearchRes->UpdateList();
	m_pListSearchResEtc->SetItemShowNum(0);
	m_pListSearchResEtc->UpdateList();

	clearSearch((eRESULT_TYPE)(eRESULT_TYPE_SEARCH_ARMOR + nSel));

	m_pListSearchRes->setCurSel(-1);
	m_pListSearchResEtc->setCurSel(-1);
	m_pBtnRegFav->SetEnable(FALSE);
	m_pBtnBuy->SetEnable(FALSE);

	clearInput();
}

void CUIAuctionNew::clearSearch(eRESULT_TYPE eType)
{
	int		i;
	CUIListItem* pListItem;

	if (eType == eRESULT_TYPE_SEARCH_ARMOR || 
		eType == eRESULT_TYPE_SEARCH_SHIELD)
	{
		for (i = 0; i < DEF_PAGE_PER_COUNT; ++i)
		{
			pListItem = (CUIListItem*)m_pListSearchRes->GetListItem(i);

			if (pListItem != NULL)
				clearResult(eType, pListItem);
		}
	}
	else if (eType == eRESULT_TYPE_SEARCH_ETC)
	{
		for (i = 0; i < DEF_PAGE_PER_COUNT; ++i)
		{
			pListItem = (CUIListItem*)m_pListSearchResEtc->GetListItem(i);

			if (pListItem != NULL)
				clearResult(eType, pListItem);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand(int nCommandCode, int nResult )
// Desc : 거래대행 npc 메시지처리 
// ----------------------------------------------------------------------------
void CUIAuctionNew::MsgBoxLCommand(int nCommandCode, int nResult )
{
	switch(nCommandCode)
	{
	case MSGLCMD_AUCTION:
		{
			if(nResult == eMSG_COMMAND_USE_AUCTION)
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				if( pUIManager->GetInventory()->IsLocked() == TRUE ||
					pUIManager->GetInventory()->IsLockedArrange() == TRUE )
				{
					pUIManager->GetInventory()->ShowLockErrorMessage();
					return;
				}

				UseAuction();
			}
			else if(nResult == NPC_HELP)
			{
				CUIManager::getSingleton()->RearrangeOrder( UI_NPCHELP, TRUE );
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
// Desc :
// ----------------------------------------------------------------------------
void CUIAuctionNew::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_AUCTION_CANCELREGISTER:	//등록 취소
		{
			if(bOK)
			{
				int nSel = m_pListRegistResult->getCurSel();
				m_pAuction->SendRegistCancelReq(nSel);
			}
		}
		break;

	case MSGCMD_AUCTION_BUYITEM:		//아이템 구매
		{
			if(bOK)
			{
				m_pAuction->SendBuyReq(m_nBuyIndex);
				m_nBuyIndex = -1;
			}
		}
		break;
	case MSGCMD_AUCTION_FAVORITE:
		{
			if (bOK == TRUE)
				CanRegFavorite(true);
		}
		break;
	case MSGCMD_AUCTION_FAVORITE_CANCEL:
		{
			if (bOK == TRUE)
				CanRegFavorite(false);
		}
		break;
	}
}

void CUIAuctionNew::CloseAuctionMessageBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->DoesMessageBoxExist(MSGCMD_AUCTION_BUYITEM))			//아이템 구매창
		pUIManager->CloseMessageBox(MSGCMD_AUCTION_BUYITEM);

	if(pUIManager->DoesMessageBoxExist(MSGCMD_AUCTION_CANCELREGISTER))		//아이템 등록취소창
		pUIManager->CloseMessageBox(MSGCMD_AUCTION_CANCELREGISTER);

	if(pUIManager->DoesMessageBoxExist(MSGCMD_AUCTION_INFO))				//등록완료, 취소등	
		pUIManager->CloseMessageBox(MSGCMD_AUCTION_INFO);

	if(pUIManager->DoesMessageBoxExist(MSGCMD_AUCTION_FAVORITE))
		pUIManager->CloseMessageBox(MSGCMD_AUCTION_FAVORITE);

	if(pUIManager->DoesMessageBoxExist(MSGCMD_AUCTION_FAVORITE_CANCEL))
		pUIManager->CloseMessageBox(MSGCMD_AUCTION_FAVORITE_CANCEL);
}

//---------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIAuctionNew::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}


// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIAuctionNew::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

BOOL CUIAuctionNew::IsFarNPC()
{
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
		return TRUE;

	return FALSE;
}

//---------------------------------------------------------------------

//---------------------------------------------------------------------



//---------------------------------------------------------------------
// PRIVATE

BOOL CUIAuctionNew::IsEditBoxFocused()
{
	int		i;

	int nMainTab = m_pMainTab->getCurSelTab();

	if (nMainTab == 0)
	{
		int	nSubTab = m_pSearchTab->getCurSelTab();

		if (nSubTab < 2)
		{
			for (i = 0; i < eEDIT_TYPE_MAX; ++i)
			{
				if (m_pEdit[nSubTab][i] && m_pEdit[nSubTab][i]->IsFocused())
					return TRUE;
			}
		}
		else
		{
			for (i = 0; i < eEDIT_ETC_TYPE_MAX; ++i)
			{
				if (m_pEditEtc[i] && m_pEditEtc[i]->IsFocused())
					return TRUE;
			}
		}
	}
	else if (nMainTab == 1)
	{
		if (m_pEditQuantity && m_pEditQuantity->IsFocused())
			return TRUE;

		if (m_pEditPrice && m_pEditPrice->IsFocused())
			return TRUE;
	}	

	return FALSE;
}

void CUIAuctionNew::updateNas()
{
	if (m_pTxtNas == NULL || m_pTxtRegNas == NULL || m_pTxtFavNas == NULL)	
		return;

	CTString strNas;
	COLOR	color;
	__int64 nas = _pNetwork->MyCharacterInfo.money;
	UtilHelp::getSingleton()->GetNasStringColor(nas, strNas, color);

	m_pTxtNas->SetText(strNas);
	m_pTxtRegNas->SetText(strNas);
	m_pTxtFavNas->SetText(strNas);
	m_pTxtNas->setFontColor(color);
	m_pTxtRegNas->setFontColor(color);
	m_pTxtFavNas->setFontColor(color);
}

void CUIAuctionNew::ShowError(int nError)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CUIMsgBox_Info MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(4302, "아이템 등록"), UMBS_OK, UI_NONE, MSGCMD_AUCTION_INFO );


	switch (nError)
	{
	case ResponseClient::TRADEAGENT_ERROR_NOT_EXIST_ITEM:
		MsgBoxInfo.AddString(_S(5708, "해당 아이템이 존재 하지 않습니다."));
		break;
	case ResponseClient::TRADEAGENT_ERROR_NOT_ENOUGH_MONEY:
		MsgBoxInfo.AddString(_S(306, "나스가 부족합니다."));
		break;
	case ResponseClient::TRADEAGENT_ERROR_NOT_TRADEITEM:
		MsgBoxInfo.AddString(_S(4346, "등록할 수 없는 아이템 입니다."));
		break;
	case ResponseClient::TRADEAGENT_ERROR_ITEM_REG_FULL:
	case ResponseClient::TRADEAGENT_ERROR_ITEM_LIKE_REG_FULL:
		MsgBoxInfo.AddString(_S(6029, "등록할 수 있는 아이템의 갯수가 꽉 찾습니다."));
		break;

	case ResponseClient::TRADEAGENT_ERROR_ITEM_BUY_ING:
		MsgBoxInfo.AddString(_S(4369, "이미 판매된 아이템입니다."));
		break;

	case ResponseClient::TRADEAGENT_ERROR_SEARCH_ERROR:
	case ResponseClient::TRADEAGENT_ERROR_INCORRECT_MONEY:
	case ResponseClient::TRADEAGENT_ERROR_ITEM_REG_FAIL:
	case ResponseClient::TRADEAGENT_ERROR_ITEM_REG_CANCEL_FAIL:
	case ResponseClient::TRADEAGENT_ERROR_ITEM_BUY_FAIL:
	case ResponseClient::TRADEAGENT_ERROR_ITEM_LIKE_LIST_FAIL:
	case ResponseClient::TRADEAGENT_ERROR_ITEM_LIKE_REG_FAIL:
	case ResponseClient::TRADEAGENT_ERROR_ITEM_LIKE_CANCEL_FAIL:
		{
			CTString	strError = _S(4364, "현재 거래대행 서비스가 혼잡한 상태입니다.");
			strError += CTString(0, " (%d)", nError);
			MsgBoxInfo.AddString( strError );
		}
		break;
		//-------------------------------------------------------------
	case eMSG_BOX_ERROR_REGIST:
		{
			CUIIcon* pIcon = pUIManager->GetDragIcon();

			if (pIcon == NULL)
				break;

			if (pIcon->getBtnType() == UBET_ITEM)
			{
				MsgBoxInfo.AddString(_S(4346, "등록할 수 없는 아이템입니다."));
			}
			else
			{
				MsgBoxInfo.AddString(_S(4347, "아이템만 등록할 수 있습니다."));
			}
		}
		break;
	case eMSG_BOX_ERROR_REG_EMPTY_CHAOS:
		MsgBoxInfo.AddString(_S(6069 , "카오스 보석 소켓이 있는 아이템만 거래대행에 등록할 수 있습니다."));
		break;

	case eMSG_BOX_ERROR_REG_COUNT:
		MsgBoxInfo.AddString(_S(6029, "더 이상 등록할 수 없습니다."));
		break;
	}


	pUIManager->CreateMessageBox(MsgBoxInfo);	
}

//---------------------------------------------------------------------

WMSG_RESULT CUIAuctionNew::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pTxtTitle && m_pTxtTitle->IsInside(x, y))
	{
		m_bTitleClick = true;

		ox = x;
		oy = y;
	}

	CUIManager::getSingleton()->RearrangeOrder(UI_AUCTION, TRUE);	

	return CUIBase::OnLButtonDown(x, y);;
}

WMSG_RESULT CUIAuctionNew::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bTitleClick = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// D&D 검사
	{
		if (m_pIconRegist != NULL && pUIManager->GetDragIcon() != NULL)
		{
			if (m_pMainTab->getCurSelTab() == 1 && 
				(m_pIconRegist->IsInside(x, y) == TRUE ||
				m_pDropArea->IsInside(x, y) == TRUE) )
			{
				if (m_pAuction->getRegistCount() >= MAX_REG_COUNT)
				{
					ShowError(eMSG_BOX_ERROR_REG_COUNT);					
				}
				else if (CanRegister() == true)
				{
					CItems* pItems = pUIManager->GetDragIcon()->getItems();

					if (pItems == NULL)
						return WMSG_FAIL;

					CItemData*	pItemData = pItems->ItemData;
					// 소켓이 있는 경우 0 번째 카오스 슬롯이 뚫리지 않았을 경우 실패처리
					if (pItemData != NULL && pItemData->IsFlag(ITEM_FLAG_SOCKET) &&
						pItems->GetSocketCount() >= 1 && pItems->GetSocketJewelIndex(0) < 0)
					{
						pUIManager->ResetHoldBtn();
						ShowError(eMSG_BOX_ERROR_REG_EMPTY_CHAOS);
						return WMSG_FAIL;
					}

					tempRegist();
				}
				else
				{
					ShowError(eMSG_BOX_ERROR_REGIST);
				}
			}
		}
	}

	// 드래그 중이라면 메세지 중지.	
	if (pUIManager->GetDragIcon() != NULL)
	{
		pUIManager->ResetHoldBtn();
		return WMSG_SUCCESS;
	}


	return CUIBase::OnLButtonUp( x, y );
}

WMSG_RESULT CUIAuctionNew::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if( m_bTitleClick && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int	ndX = x - ox;
		int	ndY = y - oy;

		ox = x;
		oy = y;

		Move( ndX, ndY );

		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
	{
		m_bTitleClick = false;
		return WMSG_FAIL;
	}

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	return WMSG_FAIL;
}

WMSG_RESULT CUIAuctionNew::OnKeyMessage( MSG* pMsg )
{
	if( pMsg->wParam == VK_TAB )
	{
		OnTabKeyProc();
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

void CUIAuctionNew::OnTabKeyProc()
{
	int		i, curFocus;
	int		nCurTabMain = m_pMainTab->getCurSelTab();
	
	if (nCurTabMain == 0)
	{
		int		nCurTabSub = m_pSearchTab->getCurSelTab();

		if (nCurTabSub == 0 || nCurTabSub == 1)
		{
			curFocus = -1;

			for (i = 0; i < eEDIT_TYPE_MAX; ++i)
			{				
				if (m_pEdit[nCurTabSub][i]->IsFocused())
				{
					curFocus = i + 1;

					if (curFocus >= eEDIT_TYPE_MAX)
						curFocus = 0;

					m_pEdit[nCurTabSub][curFocus]->SetFocus(TRUE);

					break;
				}
			}

			// 포커스가 없다면
			if (curFocus < 0)
				m_pEdit[nCurTabSub][0]->SetFocus(TRUE);
		}
		else
		{
			curFocus = -1;
			for (i = 0; i < eEDIT_ETC_TYPE_MAX; ++i)
			{
				if (m_pEditEtc[i]->IsFocused())
				{
					curFocus = i + 1;

					if (curFocus >= eEDIT_ETC_TYPE_MAX)
						curFocus = 0;

					m_pEditEtc[curFocus]->SetFocus(TRUE);

					break;
				}
			}

			if (curFocus < 0)
				m_pEditEtc[0]->SetFocus(TRUE);
		}
	}
	else if (nCurTabMain == 1)
	{
		if (m_pEditQuantity->IsFocused())
		{
			m_pEditPrice->SetFocus(TRUE);
		}
		else
		{
			m_pEditQuantity->SetFocus(TRUE);
		}
	}
}

void CUIAuctionNew::ShowMsgBuy(__int64 nas)
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	if(pUIMgr->DoesMessageBoxExist(MSGCMD_AUCTION_BUYITEM))			//아이템 구매창
		pUIMgr->CloseMessageBox(MSGCMD_AUCTION_BUYITEM);

	CUIMsgBox_Info MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(4302, "아이템 등록"), UMBS_YESNO, UI_AUCTION, MSGCMD_AUCTION_BUYITEM );

	MsgBoxInfo.AddString(_S(5983, "아이템을 구매 하시겠습니까?"));
	MsgBoxInfo.AddString( CTString(0, _S(5984, "가격: %d"), nas) );

	pUIMgr->CreateMessageBox(MsgBoxInfo);
}

void CUIAuctionNew::updateResult(eRESULT_TYPE eType, TradeAgentItem* pItem, CUIListItem* pListItem)
{
	if (pItem == NULL || pListItem == NULL)
		return;

	int			nSocket;
	COLOR		color;
	CUIIcon*	pIcon;
	CUIText*	pText;
	CUIImageFont* pFont;
	std::string	strSocket;
	CTString	strNas;
	UtilHelp*	pHelp = UtilHelp::getSingleton();
	UINT		nServerT = m_pAuction->getServerTime();
	int			rareIdx = 0;


	{
		CItemData*	pData = _pNetwork->GetItemData(pItem->item_index);

		// 아이콘 세팅
		pIcon = (CUIIcon*)pListItem->findUI("item_icon");
		if (pIcon)
		{
			pIcon->clearIconData();

			CItems* pItems = new CItems;
			pItems->Item_UniIndex = pItem->index;
			pItems->Item_Index = pData->GetItemIndex();
			pItems->ItemData = pData;
			pItems->Item_Flag = pItem->flag;
			pItems->Item_Plus = pItem->plus;
			pItems->Item_Sum = pItem->quantity;
			pItems->Item_State_Plus = pItem->plus2;
#ifdef DURABILITY
			pItems->Item_durability_now = pItem->now_durability;
			pItems->Item_durability_max = pItem->max_durability;
#endif // DURABILITY

			if( pData->GetFlag() & ITEM_FLAG_RARE )
			{
				if (pItem->option_count == 0)
				{
					pItems->SetRareIndex(0);
				}
				else
				{
					LONG iRareIndex = pItem->option_level[0];

					pItems->SetRareIndex(iRareIndex);

					WORD iRareOption = pItem->option_level[1];
					WORD wCBit =1;
					SBYTE sbOption =-1;
					for(int iBit = 0; iBit < 10; ++iBit)
					{
						if(iRareOption & wCBit)
						{
							CItemRareOption* prItem = CItemRareOption::getData(iRareIndex);

							if (prItem == NULL)
								continue;

							if (prItem->GetIndex() < 0)
								continue;

							int OptionType = prItem->rareOption[iBit].OptionIdx;
							int OptionLevel = prItem->rareOption[iBit].OptionLevel;
							pItems->SetOptionData( ++sbOption, OptionType, OptionLevel, ORIGIN_VAR_DEFAULT );
						}
						wCBit <<=1;
					}
				}
			}
			else
			{
				if (pData->GetFlag() & ITEM_FLAG_ORIGIN)
				{
					int		OpCount;

					for (OpCount = 0; OpCount < MAX_ORIGIN_OPTION; OpCount++)
					{
						pItems->SetOptionData( OpCount, pData->GetOptionOriginType(OpCount), 
							pData->GetOptionOriginLevel(OpCount), pItem->item_origin[OpCount] );
					}

					// 아이템 스킬 세팅
  					for (OpCount = 0; OpCount < MAX_ITEM_SKILL; OpCount++)
  					{
  						pItems->SetItemSkill(OpCount, pData->GetOptionSkillType(OpCount), pData->GetOptionSkillLevel(OpCount));
  					}
				}
				else
				{
					for (int OpCount = 0; OpCount < pItem->option_count; OpCount++)
					{
						pItems->SetOptionData( OpCount, pItem->option_type[OpCount], 
							pItem->option_level[OpCount], ORIGIN_VAR_DEFAULT );
					}
				}
			}

			// 소켓 
			pItems->SetSocketCount(pItem->socketCount);

			for (nSocket = 0; nSocket < pItem->socketCount; ++nSocket)
			{
				pItems->SetSocketOption(nSocket, pItem->socket[nSocket]);
			}

			pIcon->setData(pItems, false);
			rareIdx = pItems->Item_RareIndex;
		}

		// 이름 등록 
		pText = (CUIText*)pListItem->findUI("item_name");
		if (pText)
		{
			CTString strName = pData->GetName();

			if (pData->IsFlag(ITEM_FLAG_RARE))
			{
				CItemRareOption* pRareOpt = CItemRareOption::getData(rareIdx);

				if (pRareOpt != NULL)
				{
					CTString strPrefix = pRareOpt->GetPrefix();
					if( strPrefix.Length() >0)
						strName.PrintF("%s %s", strPrefix, strName);
				}				
			}
			
			pText->SetText( strName );			

			color = pHelp->GetRareItemColor(rareIdx, pData->GetFlag(), pItem->option_count);

			pText->setFontColor(color);
		}

		if (eType == eRESULT_TYPE_SEARCH_ARMOR || eType == eRESULT_TYPE_SEARCH_SHIELD ||
			eType == eRESULT_TYPE_FAVORITE)
		{
			pText = (CUIText*)pListItem->findUI("item_enc");
			if (pText)
				pText->SetText( CTString(0, "+%d", pItem->plus) );
		}

		if (eType == eRESULT_TYPE_SEARCH_ARMOR || eType == eRESULT_TYPE_SEARCH_SHIELD ||
			eType == eRESULT_TYPE_FAVORITE)
		{
			// 공 / 방 은 무기, 방어구만 출력
			
			pText = (CUIText*)pListItem->findUI("item_power");			

			if (pText)
			{
				int		nPlusValue = 0;
				int		nBasePhysicalVal = 0;
				int		nBaseMagicVal = 0;
				int		nRareOptionPhysicalVal = 0;
				int		nRareOptionMagicVal = 0;

				if (pData->IsFlag(ITEM_FLAG_RARE))
				{
					CItemRareOption* rRareOp = CItemRareOption::getData(pItem->option_level[0]);

					if (rRareOp != NULL)
					{
						if (pData->GetType() == CItemData::ITEM_WEAPON)
						{
							nRareOptionPhysicalVal = rRareOp->GetPhysicalAttack();
							nRareOptionMagicVal = rRareOp->GetMagicAttack();
						}
						else if (pData->GetType() == CItemData::ITEM_SHIELD)
						{
							nRareOptionPhysicalVal = rRareOp->GetPhysicalDefence();
							nRareOptionMagicVal = rRareOp->GetMagicDefence();
						}
					}
				}

				if (pData->GetType() == CItemData::ITEM_WEAPON ||
					pData->GetType() == CItemData::ITEM_SHIELD)
				{
					if (pData->GetType() == CItemData::ITEM_WEAPON)
					{
						nBasePhysicalVal = pData->GetPhysicalAttack();
						nBaseMagicVal = pData->GetMagicAttack();
					}
					else if (pData->GetType() == CItemData::ITEM_SHIELD)
					{
						nBasePhysicalVal = pData->GetPhysicalDefence();
						nBaseMagicVal = pData->GetMagicDefence();
					}

					BOOL bRune = pData->GetLevel() >= 146 ? TRUE : FALSE;
					int nOutPutVal = 0;

					if (nBasePhysicalVal > 0)
					{
						nBasePhysicalVal += pItem->plus2 + nRareOptionPhysicalVal;
						nPlusValue = CItems::ItemUpgradeFuckingFunction( nBasePhysicalVal, pItem->plus, bRune );
						nOutPutVal = nBasePhysicalVal;
					}
					else if (nBaseMagicVal > 0)
					{
						nBaseMagicVal += pItem->plus2 + nRareOptionMagicVal;
						nPlusValue = CItems::ItemUpgradeFuckingFunction( nBaseMagicVal, pItem->plus, bRune );
						nOutPutVal = nBaseMagicVal;
					}

					if( pItem->plus > 0 )
					{
						pText->SetText( CTString(0, "%d", nOutPutVal + nPlusValue) );
					}
					else
					{
						pText->SetText( CTString(0, "%d", nOutPutVal) );
					}

				}
				else
				{
					pText->SetText(CTString(""));
				}
			}
		}

		if (eType == eRESULT_TYPE_SEARCH_ETC || eType == eRESULT_TYPE_FAVORITE ||
			eType == eRESULT_TYPE_REGIST)
		{
			pText = (CUIText*)pListItem->findUI("item_quantity");
			if (pText)
				pText->SetText( CTString(0, "%d", pItem->quantity) );
		}

		if (eType == eRESULT_TYPE_SEARCH_ARMOR || eType == eRESULT_TYPE_SEARCH_SHIELD || 
			eType == eRESULT_TYPE_FAVORITE)
		{
			pFont = (CUIImageFont*)pListItem->findUI("item_socket");

			if (pFont)
			{
				strSocket = "";

				for (nSocket = 0; nSocket < pItem->socketCount; ++nSocket)
				{
					if (nSocket == 0)
					{
						if (pItem->socket[nSocket] > 0)
							strSocket += "0";
						else
							strSocket += "1";
					}
					else
					{
						if (pItem->socket[nSocket] > 0)
							strSocket += "2";
						else
							strSocket += "3";
					}					
				}

				//if (strSocket.empty() == false)
				pFont->setString(strSocket.c_str());
			}
		}

		{
			pText = (CUIText*)pListItem->findUI("item_price");
			if (pText)
			{
				pHelp->GetNasStringColor(pItem->nas, strNas, color);		
				pText->SetText(strNas);
				pText->setFontColor(color);
			}
		}

		{
			pText = (CUIText*)pListItem->findUI("item_time");
			if (pText)
			{			
				CTString format;
				UINT	time = pItem->expire_date - nServerT;
				UINT	out;

				if (time >= DEF_DATE_ONE_DAY)
				{
					out = (time / DEF_DATE_ONE_DAY);
					if (time > (DEF_DATE_ONE_DAY * out))
						++out;
					format = _S(5970, "%d 일");
					pText->SetText( CTString(0, format, out) );
				}
				else if (time >= DEF_DATE_ONE_HOUR)
				{
					out = time / DEF_DATE_ONE_HOUR;
					if (time > (DEF_DATE_ONE_HOUR * out))
						++out;
					format = _S(5969, "%d 시");
					pText->SetText( CTString(0, format, out) );
				}
				else //if (pItem->expire_data_Unit == 2)
				{
					format = _S(5968, "%d 분");
					pText->SetText( CTString(0, format, (time / DEF_DATE_ONE_MIN)) );
				}
			}
		}
	}
}

void CUIAuctionNew::clearResult( eRESULT_TYPE eType, CUIListItem* pListItem )
{
	CUIIcon* pIcon = (CUIIcon*)pListItem->findUI("item_icon");

	if (pIcon != NULL)
		pIcon->clearIconData();

	CUIText* pText = (CUIText*)pListItem->findUI("item_name");

	if (pText != NULL)
		pText->SetText(CTString(""));

	if (eType == eRESULT_TYPE_SEARCH_ARMOR || eType == eRESULT_TYPE_SEARCH_SHIELD ||
		eType == eRESULT_TYPE_FAVORITE)
	{
		pText = (CUIText*)pListItem->findUI("item_enc");

		if (pText != NULL)
			pText->SetText(CTString(""));

		pText = (CUIText*)pListItem->findUI("item_power");

		if (pText != NULL)
			pText->SetText(CTString(""));

		CUIImageFont* pFont = (CUIImageFont*)pListItem->findUI("item_socket");

		if (pFont != NULL)
			pFont->setString("");
	}

	if (eType == eRESULT_TYPE_SEARCH_ETC || eType == eRESULT_TYPE_FAVORITE ||
		eType == eRESULT_TYPE_REGIST)
	{
		pText = (CUIText*)pListItem->findUI("item_quantity");

		if (pText != NULL)
			pText->SetText(CTString(""));
	}

	pText = (CUIText*)pListItem->findUI("item_price");
	
	if (pText != NULL)
		pText->SetText(CTString(""));

	pText = (CUIText*)pListItem->findUI("item_time");

	if (pText != NULL)
		pText->SetText(CTString(""));

	CUIImage* pImg = (CUIImage*)pListItem->findUI("img_favorite_item");
	if (pImg != NULL)
		pImg->Hide(TRUE);
}


