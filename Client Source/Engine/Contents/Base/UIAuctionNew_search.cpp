#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Common/CommonDef.h>
#include <Common/Packet/ptype_tradeAgent_system.h>
#include "UIAuctionNew.h"

#include <Engine/LocalDefine.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Util.h>
#include "Auction.h"
#include "AuctionCmd.h"

const int wtype[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 
	11, 12, 13, 14, 15,
};

#define		DEF_ENCHANT_MAX		25


void CUIAuctionNew::initSearch()
{
	m_pSearchTab = (CUITab*)findUI("tab_type");
	{
		CmdChangeSearchTab* pCmd = new CmdChangeSearchTab;
		pCmd->setData(this);
		m_pSearchTab->SetCommand(pCmd);
	}

	// List 초기화
	{
		CUIList* pListArmor = (CUIList*)findUI( "list_armor" );	
		initInputTab(0, pListArmor);
	}

	{
		CUIList* pListShield = (CUIList*)findUI( "list_shield" );
		initInputTab(1, pListShield);
	}

	m_pEditEtc[eEDIT_ETC_TYPE_NAME] = (CUIEditBox*)findUI("edit_etc_name");
	m_pEditEtc[eEDIT_ETC_TYPE_PRICE1] = (CUIEditBox*)findUI("edit_etc_price1");
	m_pEditEtc[eEDIT_ETC_TYPE_PRICE2] = (CUIEditBox*)findUI("edit_etc_price2");	

	m_pListSearchRes = (CUIList*)findUI("list_search_res");
	{
		CmdSelectListSearch* pCmd = new CmdSelectListSearch;
		pCmd->setData(this);
		m_pListSearchRes->SetCommand(pCmd);

		CmdDblClickList* pCmdDBL = new CmdDblClickList;
		pCmdDBL->setData(this);
		m_pListSearchRes->SetCommandDBL(pCmdDBL);
	}

	m_pListSearchResEtc = (CUIList*)findUI("list_search_res2");
	{
		CmdSelectListSearch* pCmd = new CmdSelectListSearch;
		pCmd->setData(this);
		m_pListSearchResEtc->SetCommand(pCmd);

		CmdDblClickList* pCmdDBL = new CmdDblClickList;
		pCmdDBL->setData(this);
		m_pListSearchResEtc->SetCommandDBL(pCmdDBL);
	}

	m_pTxtPage = (CUIText*)findUI("str_page");

	{
		CUIButton* pBtn = (CUIButton*)findUI("btn_search");

		if (pBtn)
		{
			CmdSearch* pCmd = new CmdSearch;
			pCmd->setData(this);
			pBtn->SetCommandUp(pCmd);
		}
	}

	m_pBtnRegFav = (CUIButton*)findUI("btn_reg_favorite");

	if (m_pBtnRegFav)
	{
		CmdRegFavorite* pCmd = new CmdRegFavorite;
		pCmd->setData(this, true);
		m_pBtnRegFav->SetCommandUp(pCmd);
		m_pBtnRegFav->SetEnable(FALSE);
	}
	m_pBtnBuy = (CUIButton*)findUI("btn_buy");

	if (m_pBtnBuy)
	{
		CmdBuyItem* pCmd = new CmdBuyItem;
		pCmd->setData(this);
		m_pBtnBuy->SetCommandUp(pCmd);
		m_pBtnBuy->SetEnable(FALSE);
	}
	m_pPrev = (CUIButton*)findUI("btn_left");

	if (m_pPrev)
	{
		CmdChangePage* pCmd = new CmdChangePage;
		pCmd->setData(this, false);
		m_pPrev->SetCommandUp(pCmd);
	}

	m_pNext = (CUIButton*)findUI("btn_right");

	if (m_pNext)
	{
		CmdChangePage* pCmd = new CmdChangePage;
		pCmd->setData(this, true);
		m_pNext->SetCommandUp(pCmd);
	}

	m_pTxtNas = (CUIText*)findUI("str_search_nas");

	CTString strSign = _S(6028, "▶");
	strSign += " ";
	{
		CUIText* pText = (CUIText*)findUI("str_search_etc_name");
		if (pText != NULL)
			pText->SetText(strSign + getText(71));

		pText = (CUIText*)findUI("str_search_etc_price");
		if (pText != NULL)
			pText->SetText(strSign + getText(2397));
	}
}

void CUIAuctionNew::initInputTab( int idx, CUIList* pList )
{
	if (pList == NULL)
		return;

	int		i;
	CUIText* pText = NULL;
	
	CTString strSign = _S(6028, "▶");
	strSign += " ";

	CTString strAll = _S(6038, "모두");

	{
		// UIText 이름 설정
		const int nStr[] = {
			71,		// 이름
			141,	// 직업
			5964,	// 종류
			6027,	// 등급
			73,		// 레벨
			5980,	// 인챈트
			4178,	// 물리 공격력
			86,		// 마법 공격력
			6066,	// 소켓 수
			2397,	// 가격
		};

		int nMax = pList->getChildCount();
		CUIBase* pChild;

		for (i = 0; i < nMax; ++i)
		{
			pChild = pList->getChildAt(i);

			if (pChild)
			{
				pText = (CUIText*)pChild->findUI("str_title");
				if (pText)
				{
					if (idx > 0 && i == 6)
					{
						pText->SetText( strSign + getText(87));
					}
					else if (idx > 0 && i == 7)
					{
						pText->SetText( strSign + getText(88));
					}
					else
					{
						pText->SetText( strSign + getText(nStr[i]));
					}
				}
			}

		}

		pList->UpdateList();
	}
	
	{
		// Edit 설정
		const char* strEdit[] = {
			"edit_name",	
			"edit_lv1",		"edit_lv2",
			"edit_pp1",		"edit_pp2",		
			"edit_mp1",		"edit_mp2",
			"edit_price1",	"edit_price2",
		};

		for (i = 0; i < eEDIT_TYPE_MAX; ++i)
		{
			m_pEdit[idx][i] = (CUIEditBox*)pList->findUI(strEdit[i]);
		}
	}

	{
		// Combo 설정
		const char* strCombo[] = {
			"cb_class",		"cb_type",		"cb_grade",
			"cb_enc1",		"cb_enc2",		
			"cb_socket1",	"cb_socket2",
		};

		for (i = 0; i < eCOMBO_TYPE_MAX; ++i)
		{
			m_pCombo[idx][i] = (CUIComboBox*)pList->findUI(strCombo[i]);
			ASSERT(m_pCombo[i]);
		}

		m_pCombo[idx][eCOMBO_TYPE_CLASS]->AddString( strAll );

		UtilHelp* pHelp = UtilHelp::getSingleton();
		for (i = 0; i < TOTAL_JOB; ++i)
		{
			m_pCombo[idx][eCOMBO_TYPE_CLASS]->AddString( pHelp->GetClassName((eJob)i) );
		}

		// 종류 설정
		m_pCombo[idx][eCOMBO_TYPE_TYPE]->AddString( strAll );

		if (idx == 0)
		{
			// 무기 타입		
			for (i = 0; i < 13; ++i)
			{
				m_pCombo[idx][eCOMBO_TYPE_TYPE]->AddString( pHelp->GetWeaponName((CItemData::ITEMWEAPONTYPE)wtype[i]) );
			}
		}
		else
		{
			// 방어구 타입
			for (i = 0; i <= CItemData::ITEM_SHIELD_ONE_SUIT; ++i)
			{
				m_pCombo[idx][eCOMBO_TYPE_TYPE]->AddString( pHelp->GetSheildName((CItemData::ITEMSHIELDTYPE)i) );
			}		

		}

		// 등급 설정
		for (i = 0; i < 9; ++i)
		{
			m_pCombo[idx][eCOMBO_TYPE_GRADE]->AddString( getGradeString(i) );
		}

		CTString strTmp;	

		// 인챈트
		{
			m_pCombo[idx][eCOMBO_TYPE_ENC_S]->AddString( strAll );
			m_pCombo[idx][eCOMBO_TYPE_ENC_L]->AddString( strAll );

			for (i = 1; i <= DEF_ENCHANT_MAX; ++i)
			{
				strTmp.PrintF("+%d", i);
				m_pCombo[idx][eCOMBO_TYPE_ENC_S]->AddString( strTmp );
				m_pCombo[idx][eCOMBO_TYPE_ENC_L]->AddString( strTmp );
			}
		}

		m_pCombo[idx][eCOMBO_TYPE_SOCKET_S]->AddString( strAll );
		m_pCombo[idx][eCOMBO_TYPE_SOCKET_L]->AddString( strAll );

		for (i = 1; i <= 7; ++i)
		{
			strTmp.PrintF("%d", i);
			m_pCombo[idx][eCOMBO_TYPE_SOCKET_S]->AddString( strTmp );
			m_pCombo[idx][eCOMBO_TYPE_SOCKET_L]->AddString( strTmp );
		}
	}
}

void CUIAuctionNew::clearInput()
{
	int		i, type;

	for (type = 0; type < 2; ++type)
	{
		for (i = 0; i < eEDIT_TYPE_MAX; ++i)
		{
			if (m_pEdit[type][i] != NULL)
				m_pEdit[type][i]->ClearInputBuffer();
		}
	}

	for (type = 0; type < 2; ++type)
	{
		for (i = 0; i < eCOMBO_TYPE_MAX; ++i)
		{
			if (m_pCombo[type][i] != NULL)
				m_pCombo[type][i]->SetCurSel(0);
		}
	}

	for (i = 0; i < eEDIT_ETC_TYPE_MAX; ++i)
	{
		if (m_pEditEtc[i] != NULL)
			m_pEditEtc[i]->ClearInputBuffer();
	}
}

CTString CUIAuctionNew::getGradeString( int idx )
{
// 	5971	일반 // 	5972	기본 // 	5973	고급
// 	5974	귀한 // 	5975	특별 // 	5976	영웅
// 	5977	전설 // 	5978	미감정
	CTString	strRet;
	// 
	switch (idx)
	{
	case 0:
		strRet = _S(6038, "모두");
		break;
	case 1:	case 2:	case 3:	case 4: 
	case 5: case 6: case 7:	case 8:
		strRet = _S(5970 + idx, "");
		break;
	}

	return strRet;
}

//-------------------------------------------------------------------

int CUIAuctionNew::CheckSearchInput(RequestClient::TradeAgentSystemListBase* pData)
{	
	int cur_tab = m_pSearchTab->getCurSelTab();

	if (cur_tab < 2)
	{
		RequestClient::TradeAgentSystemList* pList = 
			reinterpret_cast<RequestClient::TradeAgentSystemList*>(pData);

		// 이름 유효성 검사.
		if (UTIL_HELP()->IsSpecialChar(pList->itemName) == true)
			return eSEARCH_TYPE_NAME;
		// 레벨
		if (pList->maxLevel != 0 && 
			pList->minLevel > pList->maxLevel)
			return eSEARCH_TYPE_LEVEL;
		// 인챈트
		if (pList->maxItemPlus != 0 && 
			pList->minItemPlus > pList->maxItemPlus)
			return eSEARCH_TYPE_ENCHANT;
		// 공/방
		if (pList->maxAttackValue != 0 && 
			pList->minAttackValue > pList->maxAttackValue)
			return eSEARCH_TYPE_BATTLE;
		// 마법 공/방
		if (pList->maxMagicValue != 0 && 
			pList->minMagicValue > pList->maxMagicValue)
			return eSEARCH_TYPE_MAGIC;
		// 소켓
		if (pList->maxSocketCount != 0 && 
			pList->minSocketCount > pList->maxSocketCount)
			return eSEARCH_TYPE_SOCKET;
		// 가격
		if (pList->maxPrice != 0 && 
			pList->minPrice > pList->maxPrice)
			return eSEARCH_TYPE_PRICE;

	}
	else
	{
		// 가격
		if (pData->maxPrice != 0 && 
			pData->minPrice > pData->maxPrice)
			return eSEARCH_TYPE_PRICE;
	}

	return -1;
}

bool CUIAuctionNew::CanSearch()
{
	int cur_tab = m_pSearchTab->getCurSelTab();
	int nRet = 0;

	{
		m_nCurSearchSel = cur_tab;		
		
		if (m_nCurSearchSel < 2)
		{
			RequestClient::TradeAgentSystemList* pLast = (RequestClient::TradeAgentSystemList*)m_pAuction->getSearchPacket(cur_tab);

			strcpy( pLast->itemName, m_pEdit[m_nCurSearchSel][eEDIT_TYPE_NAME]->GetString() );

			pLast->JobClass = m_pCombo[m_nCurSearchSel][eCOMBO_TYPE_CLASS]->GetCurSel();

			if (m_nCurSearchSel == 0)
			{
				int nWeapon = m_pCombo[m_nCurSearchSel][eCOMBO_TYPE_TYPE]->GetCurSel();

				if (nWeapon > 0)
				{
					nWeapon = wtype[nWeapon - 1] + 1;
				}

				pLast->itemSubType = nWeapon;
			}
			else
				pLast->itemSubType = m_pCombo[m_nCurSearchSel][eCOMBO_TYPE_TYPE]->GetCurSel();
			
			pLast->itemGrade = m_pCombo[m_nCurSearchSel][eCOMBO_TYPE_GRADE]->GetCurSel();
			
			pLast->minItemPlus = m_pCombo[m_nCurSearchSel][eCOMBO_TYPE_ENC_S]->GetCurSel();
			pLast->maxItemPlus = m_pCombo[m_nCurSearchSel][eCOMBO_TYPE_ENC_L]->GetCurSel();

			pLast->minLevel = atoi(m_pEdit[m_nCurSearchSel][eEDIT_TYPE_LV_S]->GetString());
			pLast->maxLevel = atoi(m_pEdit[m_nCurSearchSel][eEDIT_TYPE_LV_L]->GetString());

			pLast->minAttackValue = atoi(m_pEdit[m_nCurSearchSel][eEDIT_TYPE_P1_S]->GetString());
			pLast->maxAttackValue = atoi(m_pEdit[m_nCurSearchSel][eEDIT_TYPE_P1_L]->GetString());

			pLast->minMagicValue = atoi(m_pEdit[m_nCurSearchSel][eEDIT_TYPE_P2_S]->GetString());
			pLast->maxMagicValue = atoi(m_pEdit[m_nCurSearchSel][eEDIT_TYPE_P2_L]->GetString());

			pLast->minSocketCount = m_pCombo[m_nCurSearchSel][eCOMBO_TYPE_SOCKET_S]->GetCurSel();
			pLast->maxSocketCount = m_pCombo[m_nCurSearchSel][eCOMBO_TYPE_SOCKET_L]->GetCurSel();

			pLast->minPrice = _atoi64(m_pEdit[m_nCurSearchSel][eEDIT_TYPE_PRICE_S]->GetString());
			pLast->maxPrice = _atoi64(m_pEdit[m_nCurSearchSel][eEDIT_TYPE_PRICE_L]->GetString());

			pLast->pageNo = 1;		// 버튼으로 찾기는 무조건 첫페이지 요청

			nRet = CheckSearchInput(pLast);
		}
		else
		{
			RequestClient::TradeAgentSystemListBase* pLast = m_pAuction->getSearchPacket(cur_tab);

			strcpy( pLast->itemName, m_pEditEtc[eEDIT_ETC_TYPE_NAME]->GetString() );

			pLast->minPrice = _atoi64(m_pEditEtc[eEDIT_ETC_TYPE_PRICE1]->GetString());
			pLast->maxPrice = _atoi64(m_pEditEtc[eEDIT_ETC_TYPE_PRICE2]->GetString());

			pLast->pageNo = 1;		// 버튼으로 찾기는 무조건 첫페이지 요청

			nRet = CheckSearchInput(pLast);
		}		
	}

	if (nRet >= 0)
	{
		ShowErrorSearch(nRet);
		return false;
	}

 	if (m_pAuction)
 		m_pAuction->SendSearchReq();

	return true;
}

void CUIAuctionNew::updateSearchList()
{
 	if (m_pListSearchRes == NULL || m_pListSearchRes == NULL)
 		return;

	if (m_nCurSearchSel < 2)
	{
		m_pListSearchRes->Hide(FALSE);
		m_pListSearchResEtc->Hide(TRUE);
	}
	else
	{
		m_pListSearchRes->Hide(TRUE);
		m_pListSearchResEtc->Hide(FALSE);
	}


	int		i;
	int		nMax = m_pAuction->getSearchCount();

	TradeAgentItem* pItem;	
	int				nListItemCnt;	
	CUIListItem*	pListItem;
	CUIListItem*	pTmpItem;
	CUIImage*		pImg;
	
	if (m_nCurSearchSel < 2)
	{
		nListItemCnt = m_pListSearchRes->getListItemCount();
		m_pListSearchRes->SetItemShowNum(nMax);
	}
	else
	{
		nListItemCnt = m_pListSearchResEtc->getListItemCount();
		m_pListSearchResEtc->SetItemShowNum(nMax);
	}

	if (m_nCurSearchSel < 2)
		pListItem = m_pListSearchRes->GetListItemTemplate();
	else 
		pListItem = m_pListSearchResEtc->GetListItemTemplate();

	for (i = 0; i < DEF_PAGE_PER_COUNT; ++i)
	{
		if (i >= nListItemCnt)
		{
			if (m_nCurSearchSel < 2)
			{
				pTmpItem = (CUIListItem*)pListItem->Clone();
				pTmpItem->addChild(m_pImgFavoriteItem->Clone());
				m_pListSearchRes->AddListItem(pTmpItem);
			}
			else
			{
				pTmpItem = (CUIListItem*)pListItem->Clone();
				pTmpItem->addChild(m_pImgFavoriteItem->Clone());
				m_pListSearchResEtc->AddListItem(pTmpItem);
			}
		}

		if (m_nCurSearchSel < 2)
			pTmpItem = (CUIListItem*)m_pListSearchRes->GetListItem(i);
		else 
			pTmpItem = (CUIListItem*)m_pListSearchResEtc->GetListItem(i);

		if (pTmpItem == NULL)
			continue;
		
		if (i >= nMax)
		{
			clearResult((eRESULT_TYPE)(eRESULT_TYPE_SEARCH_ARMOR + m_nCurSearchSel), pTmpItem);
			continue;
		}

		pItem = m_pAuction->getSearchItem(i);

		if (pItem == NULL)
		{
			continue;
		}

		updateResult((eRESULT_TYPE)(eRESULT_TYPE_SEARCH_ARMOR + m_nCurSearchSel), pItem, pTmpItem);

		pImg = (CUIImage*)pTmpItem->findUI("img_favorite_item");
		if (pImg != NULL)
		{
			if (m_pAuction->findFavItem(pItem->index) == true)
				pImg->Hide(FALSE);
			else
				pImg->Hide(TRUE);
		}
	}

	if (m_nCurSearchSel < 2)
	{
		m_pListSearchRes->setCurSel(-1);
		m_pListSearchRes->UpdateList();
	}
	else
	{
		m_pListSearchResEtc->setCurSel(-1);
		m_pListSearchResEtc->UpdateList();
	}

	// Page 갱신
	m_pTxtPage->SetText( CTString(0, "%d", m_pAuction->getCurPage() ) );

	// 버튼 상태 갱신
	SelListSearch();
}

void CUIAuctionNew::CanBuyItem()
{
	if (m_pAuction == NULL)
		return;

	if(m_pAuction->getNetWorkLock() == true)
		return;

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
		return;
	}

	if (pItem == NULL)
	{
		// error
		return;
	}

	ShowMsgBuy(pItem->nas);
	m_nBuyIndex = pItem->index;
}

void CUIAuctionNew::ChangePage( bool bNext )
{
	if (m_pAuction == NULL)
		return;

	m_pAuction->RequestNextPage(bNext);
}

void CUIAuctionNew::setButtonState( INT16 page, UINT8 next )
{
	if (page <= 1)
		m_pPrev->SetEnable(FALSE);
	else
		m_pPrev->SetEnable(TRUE);

	if (next > 0)
		m_pNext->SetEnable(TRUE);
	else
		m_pNext->SetEnable(FALSE);
}

void CUIAuctionNew::SelListSearch()
{
	int		nCurMainTab = m_pMainTab->getCurSelTab();

	if (nCurMainTab == 0)
	{
		int nCurSearchTab = m_pSearchTab->getCurSelTab();
		int nCurSelList = -1;

		if (nCurSearchTab < 2)
		{
			nCurSelList = m_pListSearchRes->getCurSel();
		}
		else
		{
			nCurSelList = m_pListSearchResEtc->getCurSel();
		}

		if (nCurSelList >= m_pAuction->getSearchCount())
			nCurSelList = -1;

		if (nCurSelList >= 0)
		{
			BOOL bFav = TRUE;
			m_pBtnBuy->SetEnable(TRUE);

			TradeAgentItem* pItem = m_pAuction->getSearchItem(nCurSelList);
			if (pItem != NULL)
			{
				if (m_pAuction->findFavItem(pItem->index) == true)
					bFav = FALSE;
			}

			if (m_pAuction->getFavoriteCount() >= MAX_LIKE_REG_COUNT)
				bFav = FALSE;

			m_pBtnRegFav->SetEnable(bFav);
		}
		else
		{
			m_pBtnRegFav->SetEnable(FALSE);
			m_pBtnBuy->SetEnable(FALSE);
		}
	}
}

void CUIAuctionNew::ShowErrorSearch( int nError )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CUIMsgBox_Info MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(6017, "거래대행"), UMBS_OK, UI_NONE, MSGCMD_AUCTION_INFO );

	const int nStr[] = {
		71,		// 이름
		141,	// 직업
		5964,	// 종류
		6027,	// 등급
		73,		// 레벨
		5980,	// 인챈트
		4178,	// 물리 공격력
		86,		// 마법 공격력
		6066,	// 소켓 수
		2397,	// 가격
	};

	switch (nError)
	{
	case eSEARCH_TYPE_NAME:
	case eSEARCH_TYPE_LEVEL:
	case eSEARCH_TYPE_ENCHANT:
	case eSEARCH_TYPE_BATTLE:
	case eSEARCH_TYPE_MAGIC:
	case eSEARCH_TYPE_SOCKET:
	case eSEARCH_TYPE_PRICE:
		{
			CTString str;
			str.PrintF( _S(6032, "%s의 검색조건이 올바르지 않습니다. 다시 확인해 주시길 바랍니다."), getText(nStr[nError]) );
			MsgBoxInfo.AddString(str);
		}
		break;

	default:
		return;
		break;
	}

	pUIManager->CreateMessageBox(MsgBoxInfo);
}