
#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Common/CommonDef.h>
#include <Common/Packet/ptype_tradeAgent_system.h>
#include "UIAuctionNew.h"
#include "AuctionCmd.h"
#include <Engine/Contents/Base/Auction.h>
#include <Engine/Help/Util_Help.h>



void CUIAuctionNew::initFavorite()
{
	m_pListFavorite = (CUIList*)findUI("list_favorite");
	{
		CmdSelectListFav* pCmd = new CmdSelectListFav;
		pCmd->setData(this);
		m_pListFavorite->SetCommand(pCmd);

		CmdDblClickList* pCmdDBL = new CmdDblClickList;
		pCmdDBL->setData(this);
		m_pListFavorite->SetCommandDBL(pCmdDBL);
	}

	m_pBtnFavCancel = (CUIButton*)findUI("btn_fav_reg_cancel");

	if (m_pBtnFavCancel != NULL)
	{
		CmdRegFavorite* pCmd = new CmdRegFavorite;
		pCmd->setData(this, false);
		m_pBtnFavCancel->SetCommandUp(pCmd);
	}

	m_pBtnFavBuy = (CUIButton*)findUI("btn_fav_buy");

	if (m_pBtnFavBuy != NULL)
	{
		CmdBuyItem* pCmd = new CmdBuyItem;
		pCmd->setData(this);
		m_pBtnFavBuy->SetCommandUp(pCmd);
	}

	m_pTxtFavNas = (CUIText*)findUI("str_favorite_nas");

	m_pTxtFavCount = (CUIText*)findUI("str_fav_count");
	m_pTxtFavCount->SetText( CTString(0, "0 / %d", MAX_LIKE_REG_COUNT) );
}

bool CUIAuctionNew::CanRegFavorite(bool bRegist)
{
	TradeAgentItem* pItem;
	int		nSel, nSelMainTab = m_pMainTab->getCurSelTab();

	if (nSelMainTab == 0)
	{
		if (m_nCurSearchSel < 2)
			nSel = m_pListSearchRes->getCurSel();
		else
			nSel = m_pListSearchResEtc->getCurSel();

		pItem = m_pAuction->getSearchItem(nSel);
	}
	else if (nSelMainTab == 2)
	{
		nSel = m_pListFavorite->getCurSel();
		pItem = m_pAuction->getFavoriteItem(nSel);
	}
	else
	{
		return false;
	}

	if (pItem != NULL)
	{
		// 등록
		if (bRegist == true)
			m_pAuction->SendFavoriteRegReq(pItem->index);
		else
			m_pAuction->SendFavoriteCancelReq(pItem->index);

		return true;
	}

	return false;
}

void CUIAuctionNew::updateFavoriteList()
{
	if (m_pListFavorite == NULL)
		return;

	int		i;
	int		nMax = m_pAuction->getFavoriteCount();

	m_pTxtFavCount->SetText( CTString(0, "%d/%d", nMax, MAX_LIKE_REG_COUNT) );

	TradeAgentItem* pItem;	
	int			nListItemCnt;
	CUIManager* pUIMgr = CUIManager::getSingleton();	
	
	nListItemCnt = m_pListFavorite->getListItemCount();

	m_pListFavorite->SetItemShowNum(nMax);

	CUIListItem*	pListItem; 
	CUIListItem*	pTmpItem;

	pListItem = m_pListFavorite->GetListItemTemplate();

	for (i = 0; i < MAX_LIKE_REG_COUNT; ++i)
	{
		if (i >= nListItemCnt)
			m_pListFavorite->AddListItem((CUIListItem*)pListItem->Clone());

		pTmpItem = (CUIListItem*)m_pListFavorite->GetListItem(i);

		if (pTmpItem == NULL)
			continue;

		if (i >= nMax)
		{
			clearResult(eRESULT_TYPE_FAVORITE, pTmpItem);
			continue;
		}

		pItem = m_pAuction->getFavoriteItem(i);

		if (pItem == NULL)
		{
			continue;
		}

		updateResult(eRESULT_TYPE_FAVORITE, pItem, pTmpItem);
	}

	m_pListFavorite->setCurSel(-1);
	m_pListFavorite->UpdateList();

	//관심물품이 바뀌게 되면, 검색 갱신
	updateSearchList();

	if (nMax >= MAX_LIKE_REG_COUNT)
	{
		m_pBtnRegFav->SetEnable(FALSE);
	}
}

void CUIAuctionNew::SelListFavorite()
{
	int nSel = m_pListFavorite->getCurSel();

	if (nSel >= m_pAuction->getFavoriteCount())
		nSel = -1;

	if (nSel >= 0 )
	{
		m_pBtnFavCancel->SetEnable(TRUE);
		m_pBtnFavBuy->SetEnable(TRUE);
	}
	else
	{
		m_pBtnFavCancel->SetEnable(FALSE);
		m_pBtnFavBuy->SetEnable(FALSE);
	}
}

void CUIAuctionNew::showMsgFavorite( bool bRegist )
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	CUIMsgBox_Info MsgBoxInfo;	

	if (bRegist == true)
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(4302, "아이템 등록"), UMBS_YESNO, UI_AUCTION, MSGCMD_AUCTION_FAVORITE );
		MsgBoxInfo.AddString(_S(5985, "관심물품으로 등록 하시겠습니까?"));
		MsgBoxInfo.AddString( CTString(0, _S(5986, "남은 자리: %d개"), (MAX_LIKE_REG_COUNT - m_pAuction->getFavoriteCount())) );
	}
	else
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(4302, "아이템 등록"), UMBS_YESNO, UI_AUCTION, MSGCMD_AUCTION_FAVORITE_CANCEL );
		MsgBoxInfo.AddString(_S(5987, "아이템을 관심물품에서 제거 하시겠습니까?"));
	}

	pUIMgr->CreateMessageBox(MsgBoxInfo);
}
