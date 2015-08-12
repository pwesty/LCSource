#include "StdH.h"

#include <Engine/Interface/UIInternalClasses.h>
#include "ItemCollectionUI.h"
#include "ItemCollectionCmd.h"

#include <Engine/Contents/function/ItemCollectionData.h>
#include <Engine/Contents/function/ItemCollection.h>

#include <Engine/GameDataManager/GameDataManager.h>

#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/Base/UIMsgBoxMgr.h>
#include <Engine//Help/Util_Help.h>

#define DEF_THEME_IMAGE_SIZE (60)
#define DEF_TREE_TEXT_COL	0xB48E5FFF

CItemCollectionUI::CItemCollectionUI()
	: m_pDragArea(NULL)
	, m_pImgSummaryMask(NULL)
	, m_pTreeDesign(NULL)
	, m_pTreeThemeList(NULL)
	, m_pTreeSearchList(NULL)
	, m_pEditSearch(NULL)
	, m_pCombo(NULL)
	, m_pListTopTheme(NULL)
	, m_pTab(NULL)
	, m_pTreeTab(NULL)
	, m_pOldTree(NULL)
	, m_nOldCat(-1)
	, m_nOldTheme(-1)
	, m_nOriX(0)
	, m_nOriY(0)
	, m_bDrag(false)
	, m_nCurPage(0)
	, m_nMaxPage(0)
	, m_nSelectTheme(0)
{
	setInherit(false);

	int		i;
	for (i = 0; i < eTAB_MAX; ++i)
		m_pTabPage[i] = NULL;

	for (i = 0; i < eCHECK_MAX; ++i)
		m_pCheck[i] = NULL;

	ClearNeed4ItemTempData();

	ArrangeTopThemeList();

	// 스트링툴에 입력된 스트링 인덱스.
	// 8100 ~ 8111은 카테고리 이름으로 사용.
	for (i = 0; i < DEF_CATEGORY_COUNT; ++i)
		m_nCatName[i] = 8100 + i;
}

CItemCollectionUI::~CItemCollectionUI()
{
	m_vecSearchList.clear();
	m_vecTopThemeList.clear();

	m_pDragArea = NULL;
	m_pImgSummaryMask = NULL;
	m_pTreeThemeList = NULL;
	m_pListTopTheme = NULL;
	m_pTab = NULL;
	m_pTreeTab = NULL;
	m_pOldTree = NULL;
	m_pTreeSearchList = NULL;
	m_pEditSearch = NULL;
	m_pCombo = NULL;

	int		i;
	for (i = 0; i < eTAB_MAX; ++i)
		m_pTabPage[i] = NULL;

	for (i = 0; i < eCHECK_MAX; ++i)
		m_pCheck[i] = NULL;

	SAFE_DELETE(m_pTreeDesign);
}

void CItemCollectionUI::initialize()
{
	m_pDragArea = findUI("drag_area");
	m_pImgSummaryMask = (CUIImage*)findUI("img_summary_mask");

	if (CUIButton* pClose = (CUIButton*)findUI("btn_close"))
	{
		CmdItemCollectionClose* pCmd = new CmdItemCollectionClose;
		pCmd->setData(this);
		pClose->SetCommandUp(pCmd);
	}
	
	if (m_pTab = (CUITab*)findUI("tab_summary_and_theme"))
	{
		if (CUICheckButton* pCheck = (CUICheckButton*)m_pTab->findUI("cb_summary"))
		{
			CmdItemCollectionChangeTab* pCmd = new CmdItemCollectionChangeTab;
			pCmd->setData(this, eTAB_SUMMARY, true);
			pCheck->SetCommand(pCmd);
		}
		if (CUICheckButton* pCheck = (CUICheckButton*)m_pTab->findUI("cb_theme"))
		{
			CmdItemCollectionChangeTab* pCmd = new CmdItemCollectionChangeTab;
			pCmd->setData(this, eTAB_THEME, true);
			pCheck->SetCommand(pCmd);
		}
	}
	if (m_pTreeTab = (CUITab*)findUI("tab_theme_and_search"))
	{
		if (CUICheckButton* pCheck = (CUICheckButton*)m_pTreeTab->findUI("cb_theme_list"))
		{
			CmdItemCollectionChangeTab* pCmd = new CmdItemCollectionChangeTab;
			pCmd->setData(this, eTAB_THEME_LIST, false);
			pCheck->SetCommand(pCmd);
		}
		if (CUICheckButton* pCheck = (CUICheckButton*)m_pTreeTab->findUI("cb_search_list"))
		{
			CmdItemCollectionChangeTab* pCmd = new CmdItemCollectionChangeTab;
			pCmd->setData(this, eTAB_SEARCH_LIST, false);
			pCheck->SetCommand(pCmd);
		}
	}

	m_pTabPage[eTAB_SUMMARY] = (CUITabPage*)findUI("tp_summary");
	m_pTabPage[eTAB_THEME] = (CUITabPage*)findUI("tp_theme");

	m_pTreeThemeList = (CUITree*)findUI("tree_theme_list");
	m_pTreeSearchList = (CUITree*)findUI("tree_search_list");

	CUIScrollBar* pScroll = (CUIScrollBar*)findUI("sc_tree_list");

	if (pScroll != NULL)
		m_pTreeThemeList->setScroll(pScroll);

	pScroll = (CUIScrollBar*)findUI("sc_tree_search");

	if (pScroll != NULL)
		m_pTreeSearchList->setScroll(pScroll);

	m_pTreeDesign = (CUITree*)m_pTreeThemeList->Clone();
	
	m_pCombo = (CUIComboBox*)findUI("cb_condition");
	m_pEditSearch = (CUIEditBox*)findUI("edit_search");
	m_pListTopTheme = (CUIList*)findUI("list_theme");

	if (m_pCheck[eCHECK_ING] = (CUICheckButton*)findUI("check_progress"))
	{
		CmdItemCollectionCheckState* pCmdUnCheck = new CmdItemCollectionCheckState;
		pCmdUnCheck->setData(this, m_pCheck[eCHECK_ING]);
		m_pCheck[eCHECK_ING]->SetCommand(pCmdUnCheck);
	}
	
	if (m_pCheck[eCHECK_COMPELTE] = (CUICheckButton*)findUI("check_complete"))
	{
		CmdItemCollectionCheckState* pCmdUnCheck = new CmdItemCollectionCheckState;
		pCmdUnCheck->setData(this, m_pCheck[eCHECK_COMPELTE]);
		m_pCheck[eCHECK_COMPELTE]->SetCommand(pCmdUnCheck);
	}

	if (CUIButton* pSearch = (CUIButton*)findUI("btn_search"))
	{
		CmdItemCollectionSearch* pCmd = new CmdItemCollectionSearch;
		pCmd->setData(this);
		pSearch->SetCommandUp(pCmd);
	}

	//=================================>>
	if (m_pCombo != NULL)
	{
		m_pCombo->AddString(_S(7046, "조건 아이템"));
		m_pCombo->AddString(_S(7047, "보상 아이템"));
	}
	//=================================<<
}

void CItemCollectionUI::OpenUI()
{
	if(IsVisible() == TRUE || GetHide() == FALSE )
		return;

	CUIManager* pUIMgr = CUIManager::getSingleton();

	if (pUIMgr->GetInventory()->IsLocked() == TRUE ||
		pUIMgr->GetInventory()->IsLockedArrange() == TRUE)
	{
		// 이미 Lock 인 창이 있을 경우 열지 못한다.
		pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	SetVisible(TRUE);
	Hide(FALSE);

	pUIMgr->GetInventory()->Lock(TRUE, TRUE, LOCK_ITEMCOLLECTION);
	pUIMgr->RearrangeOrder(UI_ITEMCOLLECTION, TRUE);

	m_nCurPage = 1;

	//=============================>>
	UpdateSummaryTab();
	UpdateThemeList();
	//=============================<<
}

void CItemCollectionUI::CloseUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	CUIManager* pUIMgr = CUIManager::getSingleton();

	pUIMgr->RearrangeOrder(UI_ITEMCOLLECTION, FALSE);
	pUIMgr->GetInventory()->Lock(FALSE, FALSE, LOCK_ITEMCOLLECTION);

	m_pTab->ResetTabPos();
	m_pTreeTab->ResetTabPos();
	ClearNeed4ItemTempData();
	m_nOldCat = -1;
	m_nOldTheme = -1;

	if (m_msgID > 0)
	{
		MSGBOXMGR()->Close(m_msgID);
	}

	if (m_pEditSearch != NULL)
		m_pEditSearch->ClearInputBuffer();

	if (m_pTreeSearchList != NULL)
	{
		m_pTreeSearchList->deleteChildAll();
		m_pTreeSearchList->clearChildList();
	}

	CUIBase::CloseProc();
}

//===========================================>>
//================ 요약 탭 ==================>>
//===========================================<<
void CItemCollectionUI::UpdateSummaryTab()
{
	UpdateSummaryCategory();

	UpdateSummaryTopTheme();
}

void CItemCollectionUI::UpdateSummaryCategory()
{
	if (m_pTabPage[eTAB_SUMMARY] == NULL)
		return;

	CUIBase* pParent = m_pTabPage[eTAB_SUMMARY]->findUI("img_top");

	if (pParent == NULL)
		return;

	int nCatCount = CItemCollectionData::GetCategoryCount();
	int nThemeCount = 0;
	int nThemeComplete = 0;

	int nTotalCount = 0;
	int nTotalComplete = 0;

	if (nCatCount <= 0)
		return;
	
	if (nCatCount > 0)
	{
		m_nMaxPage = nCatCount / 4;

		if (m_nMaxPage != 0 && nCatCount != 4)
			++m_nMaxPage;
	}

	CUIProgressBar* pProgress = NULL;
	CUIText* pText = NULL;
	CTString strTmp;

	// 한페이지에 4개 표시.
	int nShowCount = 4;

	for (int i = 0; i < nShowCount; ++i)
	{
		strTmp.PrintF("pb_category_%d", i);
		pProgress = (CUIProgressBar*)pParent->findUI(strTmp.str_String);

		if (pProgress == NULL)
			continue;

		// 1을 빼준다. 0부터 시작.
		int nCat = ((m_nCurPage - 1) * 4) + i;

		nThemeCount = CItemCollectionData::GetThemeCount(nCat);

		if (nThemeCount <= 0)
		{
			pProgress->Hide(TRUE);
			continue;
		}
		else
			pProgress->Hide(FALSE);

		nThemeComplete = CItemCollectionData::GetThemeCompleteCount(nCat);

		pProgress->SetProgressRange(0, nThemeCount);
		pProgress->OffsetProgressPos(nThemeComplete);

		CItemCollectionData* pData = CItemCollectionData::GetTheme(nCat, 0);
		pText = (CUIText*)pProgress->findUI("text_name");

		if (pData == NULL || pText == NULL)
			continue;
		
		// 테마 이름 세팅.
		pText->SetText(_S(m_nCatName[nCat], "카테고리 이름"));

		// 테마 카운트 세팅.
		pText = (CUIText*)pProgress->findUI("text_progress");

		if (pText == NULL)
			continue;

		strTmp.PrintF("%d / %d", nThemeComplete, nThemeCount);
		pText->SetText(strTmp);

		nTotalCount += nThemeCount;
		nTotalComplete += nThemeComplete;
	}

	pProgress = (CUIProgressBar*)pParent->findUI("pb_progress_top");

	if (pProgress != NULL)
	{
		pProgress->SetProgressRange(0, nTotalCount);
		pProgress->OffsetProgressPos(nTotalComplete);

		pText = (CUIText*)pProgress->findUI("text_progress");

		if (pText != NULL)
		{
			strTmp.PrintF("%d / %d", nTotalComplete, nTotalCount);
			pText->SetText(strTmp);
		}
			
	}

	pText = (CUIText*)pParent->findUI("text_count");

	if (pText != NULL)
	{
		strTmp.PrintF("%d / %d", m_nCurPage, m_nMaxPage);
		pText->SetText(strTmp);
	}

	CUIButton* pArrowL = (CUIButton*)pParent->findUI("btn_left_arrow");
	CUIButton* pArrowR = (CUIButton*)pParent->findUI("btn_right_arrow");

	if (pArrowL != NULL && pArrowR != NULL)
	{
		CmdItemCollectionNext_Prev* pCmdPrev = new CmdItemCollectionNext_Prev;
		pCmdPrev->setData(this, false);
		pArrowL->SetCommand(pCmdPrev);

		CmdItemCollectionNext_Prev* pCmdNext = new CmdItemCollectionNext_Prev;
		pCmdNext->setData(this, true);
		pArrowR->SetCommand(pCmdNext);
	}
}

void CItemCollectionUI::UpdateSummaryTopTheme()
{
	if (m_pListTopTheme == NULL)
		return;

	if (ArrangeTopThemeList() == false)
	{
		m_pListTopTheme->DeleteAllListItem();
		return;
	}

	int nCount = 3;

	if (m_vecTopThemeList.size() < nCount)
		nCount = m_vecTopThemeList.size();

	CUIListItem* pItemTmp = NULL;
	CUIListItem* pItem = m_pListTopTheme->GetListItemTemplate();
	CUIImage* pImg = NULL;
	CUIText* pText = NULL;
	CUIProgressBar* pProgress = NULL;
	CUIIcon* pIcon = NULL;
	CItemCollectionData* pData = NULL;
	CTString strTmp;

	int		i;

	for (i = 0; i < nCount; ++i)
	{
		if (m_pListTopTheme->getListItemCount() <= i)
			m_pListTopTheme->AddListItem(pItem->Clone());

		pItemTmp = (CUIListItem*)m_pListTopTheme->GetListItem(i);

		if (pItemTmp == NULL)
			continue;

		pItemTmp->Hide(FALSE);
		pData = m_vecTopThemeList[i];

		if (pData == NULL)
			continue;

		if (pImg = (CUIImage*)pItemTmp->findUI("img_theme_icon"))
		{
			SetThemeImageUV(pImg, pData->id, pData->row, pData->col);
		}

		if (pText = (CUIText*)pItemTmp->findUI("text_theme_name"))
		{
			strTmp = UtilHelp::getSingleton()->GetCalcStringEllipsis(pData->GetName(), 145, "...");
			pText->SetText(strTmp);
		}

		if (pIcon = (CUIIcon*)pItemTmp->findUI("icon_result"))
		{
			switch(pData->result_type)
			{
			case eETC_TYPE_ITEM:
				{
					pIcon->setData(UBET_ITEM, pData->result_index);
					pIcon->setCount(pData->result_num);
				}
				break;
			case eETC_TYPE_NAS: case eETC_TYPE_EXP: case eETC_TYPE_SP:
				{
					pIcon->setDataEtc((eUIBTN_ETC_TYPE)pData->result_type, 0, pData->result_num);
					pIcon->setCount(0);
				}
				break;
			}
		}

		if (pProgress = (CUIProgressBar*)pItemTmp->findUI("pb_theme"))
		{
			int nPossession = CalcPossessionTotal(pData);
			int nNeed = CalcNeedItemTotal(pData);

			pProgress->SetProgressRange(0, nNeed);
			pProgress->OffsetProgressPos(nPossession);

			if (pText = (CUIText*)pProgress->findUI("text_progress"))
			{
				strTmp.PrintF("%d / %d", nPossession, nNeed);

				pText->SetText(strTmp);
			}
		}

		CmdItemCollectionSelectTheme* pCmd = new CmdItemCollectionSelectTheme;

		int nCat = 0;
		int nTheme = 0;
		if (GetTreeIndex(pData, nCat, nTheme) == false)
			continue;

		pCmd->setData(this, pData->getindex(), nCat, nTheme, true);
		pItemTmp->SetCommand(pCmd);
	}

	m_pListTopTheme->UpdateList();

	for (i = nCount; i < 3; ++i)
	{
		pItemTmp = (CUIListItem*)m_pListTopTheme->GetListItem(i);

		if (pItemTmp == NULL)
			continue;

		pItemTmp->Hide(TRUE);
	}
}

//===========================================>>
//================ 테마 탭 ==================>>
//===========================================<<
void CItemCollectionUI::UpdateThemeTab(int nThemeIdx)
{
	if (m_pTab != NULL && m_pTab->getCurSelTab() != eTAB_THEME)
		m_pTab->OnChangeTab(eTAB_THEME);

	SelectTheme(nThemeIdx);

	if (m_pTreeTab != NULL)
	{
		if (m_pTreeTab->getCurSelTab() == eTAB_THEME_LIST)
			UpdateThemeList();
		else
			UpdateSearchList();
	}

}

void CItemCollectionUI::UpdateSelectItem( int nThemeIdx )
{
	CItemCollectionData* pData = CItemCollectionData::getData(nThemeIdx);
	
	if (pData == NULL)
		return;

	int nCat = 0;
	int nTheme = 0;
	CUITree* pTree = NULL;

	if (m_pTreeTab->getCurSelTab() == eTAB_THEME_LIST)
	{
		GetTreeIndex(pData, nCat, nTheme);

		pTree = GetTreeItem(m_pTreeThemeList, nCat, nTheme);
		
	}
	// 검색 리스트에서 트리 인덱스 찾기.
	else if (m_pTreeTab->getCurSelTab() == eTAB_SEARCH_LIST)
	{
		for (int i = 0; i < m_vecSearchList.size(); ++i)
		{
			for (int j = 0; j < m_vecSearchList[i].size(); ++j)
			{
				if (m_vecSearchList[i][j] == pData)
				{
					pTree = GetTreeItem(m_pTreeSearchList, i, j);
				}
			}
		}
	}

	if (pTree == NULL)
		return;

	CUIText* pText = NULL;
	CTString strTmp, strTmp2;

	if (pData->IsCompleteTheme() == true)
	{
		strTmp2 = UtilHelp::getSingleton()->GetCalcStringEllipsis(pData->GetName(), 110, "...");
		strTmp.PrintF("%s %s", strTmp2, _S(7051, "완료"));
	}
	else
	{
		int nPer = CalcNeedItemPercentage(pData);

		CTString str;
		str.PrintF("(%d %s)", nPer, "%");

		strTmp2 = UtilHelp::getSingleton()->GetCalcStringEllipsis(pData->GetName(), 110, "...");
		strTmp.PrintF("%s %s", strTmp2, str);
	}

	pText = (CUIText*)pTree->findUI("text_name");

	if (pText == NULL)
		return;

	pText->SetText(strTmp);

	UpdateTheme();
	UpdateThemeNeed4ItemList();
}

void CItemCollectionUI::UpdateTheme()
{
	if (m_pTabPage[eTAB_THEME] == NULL)
		return;

	CUIBase* pParent = m_pTabPage[eTAB_THEME]->findUI("img_top");

	if (pParent == NULL)
		return;

	CUIText* pText = NULL;
	CUITextBox* pTextBox = NULL;
	CUIIcon* pIcon = NULL;
	CUIButton* pBtn = NULL;
	CUIProgressBar* pProgress = NULL;
	CUIImage* pImage = NULL;

	CTString strTmp;

	CItemCollectionData* pData = CItemCollectionData::getData(m_nSelectTheme);
	
	if (pData == NULL)
		return;

	pText = (CUIText*)pParent->findUI("text_theme_title");

	if (pText != NULL)
	{
		strTmp.PrintF("%s", pData->GetName());
		pText->SetText(strTmp);
	}

	pTextBox = (CUITextBox*)pParent->findUI("tb_desc");

	if (pTextBox != NULL)
	{
		strTmp.PrintF("%s", pData->GetDesc());
		pTextBox->SetText(strTmp, 0x4A2B17FF);
	}

	pIcon = (CUIIcon*)pParent->findUI("icon_result");

	if (pIcon != NULL)
	{
		switch(pData->result_type)
		{
		case eETC_TYPE_ITEM:
			{
				pIcon->setData(UBET_ITEM, pData->result_index);
				pIcon->setCount(pData->result_num);
			}
			break;
		case eETC_TYPE_NAS: case eETC_TYPE_EXP: case eETC_TYPE_SP:
			{
				pIcon->setDataEtc((eUIBTN_ETC_TYPE)pData->result_type, 0, pData->result_num);
				pIcon->setCount(0);
			}
			break;
		}
	}

	pImage = (CUIImage*)pParent->findUI("img_complete");

	if (pImage != NULL)
	{
		pImage->Hide(pData->IsCompleteTheme() ? FALSE : TRUE);
	}

	pImage = (CUIImage*)pParent->findUI("img_theme_icon");

	if (pImage != NULL)
	{
		SetThemeImageUV(pImage, pData->id, pData->row, pData->col);
	}

	pBtn = (CUIButton*)pParent->findUI("btn_amend");

	if (pBtn != NULL)
	{
		if (pData->IsCompleteTheme() == true)
		{
			pBtn->SetEnable(false);
		}
		else
		{
			bool bAmend = pData->GetNeedItemCompleteCount() >= pData->GetNeedItemCount();
	
			CmdItemCollectionOutputMsg* pCmd = new CmdItemCollectionOutputMsg;
			
			if (bAmend == true)
				pCmd->setData(this, OUTPUT_AMEND);
			else
				pCmd->setData(this, OUTPUT_GIVE);

			pBtn->SetCommand(pCmd);
	
			if (bAmend == true)
				pBtn->SetText(_S(1721, "보상받기"));
			else
				pBtn->SetText(_S(7054, "아이템 등록"));

			pBtn->SetEnable(true);
		}
	}

	if (pText = (CUIText*)pParent->findUI("text_complete_time"))
	{
		if (pData->IsCompleteTheme() == true)
		{
			int nCompleteTime = pData->GetCompleteTime();
			tm* pTime;
			pTime = localtime((time_t*)&nCompleteTime);

			strTmp.PrintF("%d.%02d.%02d", pTime->tm_year + 1900, pTime->tm_mon + 1, pTime->tm_mday);
			pText->SetText(strTmp);
		}

		pText->Hide(pData->IsCompleteTheme() ? FALSE : TRUE);
	}

	pProgress = (CUIProgressBar*)pParent->findUI("pb_progress");

	if (pProgress != NULL)
	{
		int nNeed = CalcNeedItemTotal(pData);
		int nPossession = CalcPossessionTotal(pData);

		pProgress->SetProgressRange(0, nNeed);
		pProgress->OffsetProgressPos(nPossession);

		pText = (CUIText*)pProgress->findUI("text_progress");
	
		if (pText != NULL)
		{
			if (pData->IsCompleteTheme() == true)
				strTmp = _S(7058, "테마 완료");
			else
				strTmp.PrintF("%d / %d", nPossession, nNeed);

			pText->SetText(strTmp);
		}
	}
}

void CItemCollectionUI::UpdateThemeNeed4ItemList()
{
	if (m_pTabPage[eTAB_THEME] == NULL)
		return;

	ClearNeed4ItemTempData();

	CItemCollectionData* pData = CItemCollectionData::getData(m_nSelectTheme);

	CUIArray* pArray = (CUIArray*)m_pTabPage[eTAB_THEME]->findUI("array_theme");

	if (pArray == NULL)
		return;

	CUIArrayItem* pItem = pArray->GetArrayItemTemplate();
	CUIArrayItem* pItemTmp = NULL;
	CUIText* pText = NULL;
	CUIButton* pBtn = NULL;
	CUIIcon* pIcon = NULL;
	CUIImage* pImage = NULL;
	CTString strTmp;
	bool bTheme = false;

	if (pData == NULL)
		return;

	int nNeedCount = pData->GetNeedItemCount();
	
	if (nNeedCount <= 0)
		return;

	int		i;

	for (i = 0; i < nNeedCount; ++i)
	{
		if (i >= pArray->GetArrayChildCount())
			pArray->AddArrayItem((CUIArrayItem*)pItem->Clone());

		pItemTmp = pArray->GetArrayItem(i);

		if (pItemTmp == NULL)
			continue;

		pItemTmp->Hide(FALSE);

		if (pData->need_item[i].need_type == eNEED_TYPE_THEME)
			bTheme = true;
		else
			bTheme = false;

		if (pText = (CUIText*)pItemTmp->findUI("text_need_name"))
		{
			pIcon = (CUIIcon*)pItemTmp->findUI("icon_need");

			if (pIcon != NULL)
				pIcon->clearIconData();

			if (bTheme == false)
			{
				CTString strItemName = CItemData::getData(pData->need_item[i].need_index)->GetName();
				strTmp = UtilHelp::getSingleton()->GetCalcStringEllipsis(strItemName, 110, "...");
				
				if (pIcon != NULL)
					pIcon->setData(UBET_ITEM, pData->need_item[i].need_index);
			}
			else
			{
				CItemCollectionData* pTheme = CItemCollectionData::GetTheme(pData->need_item[i].need_index);

				if (pTheme != NULL)
					strTmp = UtilHelp::getSingleton()->GetCalcStringEllipsis(pTheme->GetName(), 110, "...");

				if (pIcon != NULL)
				{
					pIcon->setDataEtc(eETC_TYPE_THEME, pData->need_item[i].need_index);
					pIcon->setCount(0);
				}
			}

			pText->SetText(strTmp);
		}

		if (pText = (CUIText*)pItemTmp->findUI("text_need_count"))
		{
			if (bTheme == false)
			{
				strTmp.PrintF("%d / %d", pData->GetNeedItemPossessionCount(i), pData->need_item[i].need_num);
				pText->SetText(strTmp);				
			}
			pText->Hide(bTheme ? TRUE : FALSE);
		}

		if (pText = (CUIText*)pItemTmp->findUI("text_need"))
		{
			pText->Hide(bTheme ? TRUE : FALSE);
		}

		if (pText = (CUIText*)pItemTmp->findUI("text_possession"))
		{
			pText->Hide(bTheme ? TRUE : FALSE);
		}

		if (pText = (CUIText*)pItemTmp->findUI("text_possession_count"))
		{
			if (bTheme == false)
			{
				int nPossession = UIMGR()->GetInventory()->GetItemCount(pData->need_item[i].need_index, false);
				strTmp.PrintF("%d", nPossession);
				pText->SetText(strTmp);
			}
			pText->Hide(bTheme ? TRUE : FALSE);
		}

		if (pText = (CUIText*)pItemTmp->findUI("text_need_theme_count"))
		{
			if (bTheme == true)
			{
				strTmp.PrintF("%d / %d", pData->GetNeedItemPossessionCount(i), pData->need_item[i].need_num);
				pText->SetText(strTmp);
			}
			pText->Hide(bTheme ? FALSE : TRUE);
		}

		if (pImage = (CUIImage*)pItemTmp->findUI("img_complete"))
		{
			if (pData->IsCompleteTheme() == true)
				pImage->Hide(FALSE);
			else
				pImage->Hide(pData->need_item[i].need_num <= pData->GetNeedItemPossessionCount(i) ? FALSE : TRUE);
		}

		if (pBtn = (CUIButton*)pItemTmp->findUI("btn_max"))
		{
			if (bTheme == false)
			{
				CmdItemCollectionMax* pCmd = new CmdItemCollectionMax;
				pCmd->setData(this, pItemTmp, pData->getindex(), i);
				pBtn->SetCommand(pCmd);
			}
			pBtn->Hide(bTheme ? TRUE : false);
		}

		if (pBtn = (CUIButton*)pItemTmp->findUI("btn_sub"))
		{
			if (bTheme == false)
			{
				CmdItemCollectionSub* pCmd = new CmdItemCollectionSub;
				pCmd->setData(this, pItemTmp, pData->getindex(), i);
				pBtn->SetCommand(pCmd);
			}
			pBtn->Hide(bTheme ? TRUE : false);
		}

		if (pBtn = (CUIButton*)pItemTmp->findUI("btn_sum"))
		{
			if (bTheme == false)
			{
				CmdItemCollectionSum* pCmd = new CmdItemCollectionSum;
				pCmd->setData(this, pItemTmp, pData->getindex(), i);
				pBtn->SetCommand(pCmd);
			}
			pBtn->Hide(bTheme ? TRUE : false);
		}
	}

	// 필요아이템이 6개미만이면 나머지를 안보이도록 처리.
	for (i = nNeedCount; i < DEF_NEED_ITEM_COUNT; ++i)
	{
		if (i >= pArray->GetArrayChildCount())
			pArray->AddArrayItem((CUIArrayItem*)pItem->Clone());

		pItemTmp = pArray->GetArrayItem(i);

		if (pItemTmp == NULL)
			continue;

		pItemTmp->Hide(TRUE);
	}

}

void CItemCollectionUI::NextCategory()
{
	if (m_nCurPage < m_nMaxPage)
		++m_nCurPage;
	else
		return;

	UpdateSummaryTab();
}

void CItemCollectionUI::PrevCategory()
{
	if (m_nCurPage > 1)
		--m_nCurPage;
	else
		return;

	UpdateSummaryTab();
}

void CItemCollectionUI::SendAmend()
{
	CItemCollectionData* pData = CItemCollectionData::getData(m_nSelectTheme);

	if (pData == NULL)
		return;

	GameDataManager::getSingleton()->GetItemCollection()->SendAmendItem(pData->getCategory(), pData->getTheme());
}

void CItemCollectionUI::SendGiveItem()
{
	CItemCollectionData* pData = CItemCollectionData::getData(m_nSelectTheme);

	if (pData == NULL)
		return;

	RequestClient::NEED_ITEM_DATA data[6];
	
	for (int i = 0; i < DEF_NEED_ITEM_COUNT; ++i)
	{
		if (IsCanRegistItem(pData->need_item[i].need_index) == false)
		{
			MSGBOX_OK(_S(7054, "아이템 등록"), _S(5988, "등록할 수 없는 아이템 입니다."));
			return;
		}

		data[i].need_num = m_nTmpPossessionItemCount[i];
		data[i].need_type = pData->need_item[i].need_type;

		if (m_nTmpPossessionItemCount[i] > 0)
			data[i].need_index = pData->need_item[i].need_index;
		else
			data[i].need_index = -1;
	}

	GameDataManager::getSingleton()->GetItemCollection()->SendGiveItem(pData->getCategory(), pData->getTheme(), data);
}

void CItemCollectionUI::UpdateThemeList()
{
	if (m_pTreeThemeList == NULL || m_pTreeDesign == NULL)
		return;

	m_pTreeThemeList->deleteChildAll();
	m_pTreeThemeList->clearChildList();

	m_pTreeThemeList->setRoot();

	CUIBase* pCatTmp = findUI("tree_group");
	CUIBase* pThemeTmp = findUI("tree_child");

	if (pCatTmp == NULL || pThemeTmp == NULL)
		return;

	CUIBase* pCategory = NULL;
	CUIBase* pTheme = NULL;
	CUIText* pText = NULL;
	CTString strTmp;
	CTString strTmp2;

	if (pCatTmp == NULL || pThemeTmp == NULL)
		return;

	int nCat = CItemCollectionData::GetCategoryCount();

	if (nCat <= 0)
		return;

	int nCmdCatIdx = 0;

	for (int i = 0; i < nCat; ++i)
	{
		CUITree* pGroupTree = (CUITree*)m_pTreeDesign->Clone();
		CItemCollectionData* pData = CItemCollectionData::GetTheme(i, 0);

		if (pData == NULL)
			continue;

		pCategory = pCatTmp->Clone();
		strTmp2.PrintF(_S(m_nCatName[i], "카테고리 이름"));
		strTmp  = UtilHelp::getSingleton()->GetCalcStringEllipsis(strTmp2, 170, "...");
		pText = (CUIText*)pCategory->findUI("text_name");

		if (pText == NULL)
			continue;

		pText->SetText(strTmp);

		pCategory->Hide(FALSE);
		pGroupTree->addChild(pCategory);
		pGroupTree->InitPos(5, 0, 192, 16);

		int nCmdThemeIdx = 0;

		for (int t = 0; t < CItemCollectionData::GetThemeCount(i); ++t)
		{
			pData = CItemCollectionData::GetTheme(i, t);
			CUITree* pChildTree = (CUITree*)m_pTreeDesign->Clone();

			if (pData == NULL)
				continue;

			if (m_pCheck[eCHECK_ING]->IsChecked() == TRUE)
			{
				if (pData->IsCompleteTheme() == true)
					continue;
				if (CalcPossessionTotal(pData) <= 0)
					continue;
			}
			else if (m_pCheck[eCHECK_COMPELTE]->IsChecked() == TRUE)
			{
				if (pData->IsCompleteTheme() == false)
					continue;
			}

			pTheme = pThemeTmp->Clone();

			if (CUIImage* pImage = (CUIImage*)pTheme->findUI("img_select"))
				pImage->Hide(TRUE);

			if (pData->IsCompleteTheme() == true)
			{
				strTmp2 = UtilHelp::getSingleton()->GetCalcStringEllipsis(pData->GetName(), 110, "...");
				strTmp.PrintF("%s %s", strTmp2, _S(7051, "완료"));
			}
			else
			{
				int nPer = CalcNeedItemPercentage(pData);

				CTString str;
				str.PrintF("(%d %s)", nPer, "%");

				strTmp2 = UtilHelp::getSingleton()->GetCalcStringEllipsis(pData->GetName(), 110, "...");
				strTmp.PrintF("%s %s", strTmp2, str);
			}

			pText = (CUIText*)pTheme->findUI("text_name");

			if (pText == NULL)
				continue;

			pText->SetText(strTmp);
			pText->setFontColor(DEF_TREE_TEXT_COL);

			pTheme->Hide(FALSE);
			pChildTree->addChild(pTheme);
			pChildTree->InitPos(0, 0, 178, 16);
			pGroupTree->addTree(pChildTree, false);

			CmdItemCollectionSelectTheme* pCmd = new CmdItemCollectionSelectTheme;
			pCmd->setData(this, pData->getindex(), nCmdCatIdx, nCmdThemeIdx);
			pTheme->SetCommand(pCmd);

 			CmdItemCollectionEnterColor* pCmdEnter = new CmdItemCollectionEnterColor;
			pCmdEnter->setData(this, nCmdCatIdx, nCmdThemeIdx, true);
			pTheme->SetCommandEnter(pCmdEnter);

			CmdItemCollectionEnterColor* pCmdLeave = new CmdItemCollectionEnterColor;
			pCmdLeave->setData(this, nCmdCatIdx, nCmdThemeIdx, false);
			pTheme->SetCommandLeave(pCmdLeave);

			nCmdThemeIdx++;
		}

		m_pTreeThemeList->addTree(pGroupTree);
		pGroupTree->setUncollapse(TRUE);
		pGroupTree->updateTree();
		nCmdCatIdx++;
	}

	m_pTreeThemeList->updateTree();
	m_pTreeThemeList->UpdateScroll();
}

void CItemCollectionUI::UpdateSearchList()
{
	if (m_pTreeSearchList == NULL || m_pTreeDesign == NULL)
		return;

	m_pTreeSearchList->deleteChildAll();
	m_pTreeSearchList->clearChildList();

	m_pTreeSearchList->setRoot();

	CUIBase* pCatTmp = findUI("tree_group");
	CUIBase* pThemeTmp = findUI("tree_child");

	if (pCatTmp == NULL || pThemeTmp == NULL)
		return;

	CUIBase* pCategory = NULL;
	CUIBase* pTheme = NULL;
	CUIText* pText = NULL;
	CTString strTmp;
	CTString strTmp2;

	bool bAddBtn = true;

	if (pCatTmp == NULL || pThemeTmp == NULL)
		return;

	int nCat = m_vecSearchList.size();

	if (nCat <= 0)
		return;

	int nCmdCatIdx = 0;

	for (int i = 0; i < nCat; ++i)
	{
		CUITree* pGroupTree = (CUITree*)m_pTreeDesign->Clone();

		if (m_vecSearchList[i].empty() == true)
			continue;

		CItemCollectionData* pData = m_vecSearchList[i][0];

		if (pData == NULL)
			continue;

		pCategory = pCatTmp->Clone();

		int nCatName = pData->getCategory() - 1;
		if (nCatName < 0 || nCatName >= DEF_CATEGORY_COUNT)
			continue;

		strTmp2.PrintF(_S(m_nCatName[nCatName], "카테고리 이름"));
		strTmp  = UtilHelp::getSingleton()->GetCalcStringEllipsis(strTmp2, 170, "...");

		pText = (CUIText*)pCategory->findUI("text_name");

		if (pText == NULL)
			continue;

		pText->SetText(strTmp);

		pCategory->Hide(FALSE);
		pGroupTree->addChild(pCategory);
		pGroupTree->InitPos(5, 0, 192, 16);

		int nThemeCount = m_vecSearchList[i].size();
		int nCmdThemeIdx = 0;

		for (int t = 0; t < nThemeCount; ++t)
		{
			CItemCollectionData* pData = m_vecSearchList[i][t];
			CUITree* pChildTree = (CUITree*)m_pTreeDesign->Clone();

			if (pData == NULL)
				continue;

			pTheme = pThemeTmp->Clone();

			if (CUIImage* pImage = (CUIImage*)pTheme->findUI("img_select"))
				pImage->Hide(TRUE);
			
			if (pData->IsCompleteTheme() == true)
			{
				strTmp2 = UtilHelp::getSingleton()->GetCalcStringEllipsis(pData->GetName(), 110, "...");
				strTmp.PrintF("%s %s", strTmp2, _S(7051, "완료"));
			}
			else
			{
				int nPer = CalcNeedItemPercentage(pData);

				CTString str;
				str.PrintF("(%d %s)", nPer, "%");

				strTmp2 = UtilHelp::getSingleton()->GetCalcStringEllipsis(pData->GetName(), 110, "...");
				strTmp.PrintF("%s %s", strTmp2, str);
			}

			pText = (CUIText*)pTheme->findUI("text_name");

			if (pText == NULL)
				continue;

			pText->SetText(strTmp);
			pText->setFontColor(DEF_TREE_TEXT_COL);

			pTheme->Hide(FALSE);
			pChildTree->addChild(pTheme);
			pChildTree->InitPos(0, 0, 178, 16);
			pGroupTree->addTree(pChildTree, false);

			CmdItemCollectionSelectTheme* pCmd = new CmdItemCollectionSelectTheme;
			pCmd->setData(this, pData->getindex(), nCmdCatIdx, nCmdThemeIdx);
			pText->SetCommand(pCmd);

			CmdItemCollectionEnterColor* pCmdEnter = new CmdItemCollectionEnterColor;
			pCmdEnter->setData(this, nCmdCatIdx, nCmdThemeIdx, true);
			pTheme->SetCommandEnter(pCmdEnter);

			CmdItemCollectionEnterColor* pCmdLeave = new CmdItemCollectionEnterColor;
			pCmdLeave->setData(this, nCmdCatIdx, nCmdThemeIdx, false);
			pTheme->SetCommandLeave(pCmdLeave);

			++nCmdThemeIdx;
		}

		pGroupTree->setUncollapse(TRUE);
		m_pTreeSearchList->addTree(pGroupTree);
		++nCmdCatIdx;
	}

	m_pTreeSearchList->updateTree();
	m_pTreeSearchList->UpdateScroll();

	if (m_pTreeTab != NULL)
		ChangeListTab(eTAB_SEARCH_LIST);
}

bool CItemCollectionUI::ArrangeSearchList()
{
	m_vecSearchList.clear();

	if (m_pEditSearch == NULL || m_pCombo == NULL)
		return false;

	std::string strSearch = m_pEditSearch->GetString();

	if (strSearch.empty() == true)
	{
		m_msgID = MSGBOXMGR()->CreateMsgBox(_S(7036, "아이리스 아이템 도감"), 
			_S(7048, "이름을 입력 하지 않아 검색 할 수 없습니다."), eMSG_OK);
		return false;
	}

	int nCatCount = CItemCollectionData::GetCategoryCount();
	int nSearchType = m_pCombo->GetCurSel();

	for (int i = 0; i < nCatCount; ++i)
	{
		int nThemeCount = CItemCollectionData::GetThemeCount(i);
		vecTheme vecT;

		for (int t = 0; t < nThemeCount; ++t)
		{
			CItemCollectionData* pItem = CItemCollectionData::GetTheme(i, t);

			if (pItem == NULL)
				continue;

			std::string str;

			switch(nSearchType)
			{
			case eSEARCH_TYPE_NEED_ITEM:
				{
					for (int i = 0; i < DEF_NEED_ITEM_COUNT; ++i)
					{
						if (pItem->need_item[i].need_index <= 0)
							continue;

						str = _pNetwork->GetItemName(pItem->need_item[i].need_index);

						if (str.empty() == true)
							continue;

						if (str.find(strSearch) == std::string::npos)
							continue;

						vecT.push_back(pItem);
						break;
					}
				}
				break;
			case eSEARCH_TYPE_RESULT_ITEM:
				{
					str = _pNetwork->GetItemName(pItem->result_index);

					if (str.empty() == true)
						break;

					if (str.find(strSearch) == std::string::npos)
						break;

					vecT.push_back(pItem);
				}
				break;
			}
			
		}

		if (vecT.empty() == true)
			continue;

		m_vecSearchList.push_back(vecT);
	}

	if (m_vecSearchList.empty() == true)
	{
		m_msgID = MSGBOXMGR()->CreateMsgBox(_S(7036, "아이리스 아이템 도감"), 
			_S(7049, "해당 하는 테마가 존재 하지 않습니다."), eMSG_OK);
		return false;
	}
	return true;
}

WMSG_RESULT CItemCollectionUI::OnKeyMessage( MSG* pMsg )
{
	if (IsEditBoxFocused())
	{
		if (pMsg->wParam == VK_RETURN)
		{
			if (ArrangeSearchList() == true)
				UpdateSearchList();
			return WMSG_SUCCESS;
		}
	}

	return WMSG_FAIL;
}

WMSG_RESULT CItemCollectionUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

WMSG_RESULT CItemCollectionUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pTabPage[eTAB_SUMMARY] != NULL && m_pTabPage[eTAB_SUMMARY]->GetHide() == FALSE)
	{
		if (m_pImgSummaryMask != NULL && m_pImgSummaryMask->GetHide() == FALSE && m_pImgSummaryMask->IsInside(x, y))
			return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

WMSG_RESULT CItemCollectionUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pDragArea && m_pDragArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder(UI_ITEMCOLLECTION, TRUE);

	if (m_pTabPage[eTAB_SUMMARY] != NULL && m_pTabPage[eTAB_SUMMARY]->GetHide() == FALSE)
	{
		if (m_pImgSummaryMask != NULL && m_pImgSummaryMask->GetHide() == FALSE && m_pImgSummaryMask->IsInside(x, y))
			return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

void CItemCollectionUI::NeedItemCountSum( CUIArrayItem* pArrayItem, int nThemeIndex, int nItemIndex )
{
	if (pArrayItem == NULL)
		return;

	CItemCollectionData* pData = CItemCollectionData::getData(nThemeIndex);

	if (pData == NULL)
		return;

	int nPossessionCount = pData->GetNeedItemPossessionCount(nItemIndex);
	int nTmpCount = m_nTmpPossessionItemCount[nItemIndex];
	int nInvenItemCount = UIMGR()->GetInventory()->GetItemCount(pData->need_item[nItemIndex].need_index, false);
	int nItemMaxCount = pData->need_item[nItemIndex].need_num;

	if (nInvenItemCount <= 0)
		return;

	if (IsCanRegistItem(nItemIndex) == false)
	{
		MSGBOX_OK(_S(7054, "아이템 등록"), _S(5988, "등록할 수 없는 아이템 입니다."));
		return;
	}

	if (nPossessionCount >= nItemMaxCount || (nPossessionCount + nTmpCount) >= nItemMaxCount)
		return;

	if (m_nTmpInvenItemCount[nItemIndex] >= nInvenItemCount)
		return;

	++m_nTmpPossessionItemCount[nItemIndex];
	++m_nTmpInvenItemCount[nItemIndex];

	CUIText* pText = NULL;
	CTString strTmp;
	
	int nCount = 0;
	if (pText = (CUIText*)pArrayItem->findUI("text_need_count"))
	{
		nCount = m_nTmpPossessionItemCount[nItemIndex] + pData->GetNeedItemPossessionCount(nItemIndex);
		strTmp.PrintF("%d / %d", nCount, pData->need_item[nItemIndex].need_num);
		pText->SetText(strTmp);
	}

	if (pText = (CUIText*)pArrayItem->findUI("text_possession_count"))
	{
		nCount = nInvenItemCount - m_nTmpInvenItemCount[nItemIndex];
		strTmp.PrintF("%d", nCount);
		pText->SetText(strTmp);
	}
}

void CItemCollectionUI::NeedItemCountSub( CUIArrayItem* pArrayItem, int nThemeIndex, int nItemIndex )
{
	if (pArrayItem == NULL)
		return;

	CItemCollectionData* pData = CItemCollectionData::getData(nThemeIndex);

	if (pData == NULL)
		return;

	int nPossessionCount = m_nTmpPossessionItemCount[nItemIndex];
	int nInvenItemCount = UIMGR()->GetInventory()->GetItemCount(pData->need_item[nItemIndex].need_index, false);

	if (nPossessionCount <= 0)
		return;

	--m_nTmpPossessionItemCount[nItemIndex];
	--m_nTmpInvenItemCount[nItemIndex];

	CUIText* pText = NULL;
	CTString strTmp;

	int nCount = 0;
	if (pText = (CUIText*)pArrayItem->findUI("text_need_count"))
	{
		nCount = m_nTmpPossessionItemCount[nItemIndex] + pData->GetNeedItemPossessionCount(nItemIndex);
		strTmp.PrintF("%d / %d", nCount, pData->need_item[nItemIndex].need_num);
		pText->SetText(strTmp);
	}

	if (pText = (CUIText*)pArrayItem->findUI("text_possession_count"))
	{
		nCount = nInvenItemCount - m_nTmpInvenItemCount[nItemIndex];
		strTmp.PrintF("%d", nCount);
		pText->SetText(strTmp);
	}
}

void CItemCollectionUI::NeedItemCountMax( CUIArrayItem* pArrayItem, int nThemeIndex, int nItemIndex )
{
	if (pArrayItem == NULL)
		return;

	CItemCollectionData* pData = CItemCollectionData::getData(nThemeIndex);

	if (pData == NULL)
		return;

	int nPossessionCount = pData->GetNeedItemPossessionCount(nItemIndex);
	int nTmpPossessionCount = m_nTmpPossessionItemCount[nItemIndex];
	int nInvenItemCount = UIMGR()->GetInventory()->GetItemCount(pData->need_item[nItemIndex].need_index, false);
	int nItemMaxCount = pData->need_item[nItemIndex].need_num;

	if (nInvenItemCount <= 0)
		return;

	if (IsCanRegistItem(nItemIndex) == false)
	{
		MSGBOX_OK(_S(7054, "아이템 등록"), _S(5988, "등록할 수 없는 아이템 입니다."));
		return;
	}

	if (nPossessionCount >= nItemMaxCount || (nPossessionCount + nTmpPossessionCount) >= nItemMaxCount)
		return;

	int nTmpCount = 0;
	if (nInvenItemCount >= (nItemMaxCount - nPossessionCount))
	{
		m_nTmpPossessionItemCount[nItemIndex] = (nItemMaxCount - nPossessionCount);
		m_nTmpInvenItemCount[nItemIndex] = (nItemMaxCount - nPossessionCount);
	}
	else
	{
		m_nTmpPossessionItemCount[nItemIndex] = nInvenItemCount;
		m_nTmpInvenItemCount[nItemIndex] = nInvenItemCount;
	}

	CUIText* pText = NULL;
	CTString strTmp;

	if (pText = (CUIText*)pArrayItem->findUI("text_need_count"))
	{
		nTmpCount = m_nTmpPossessionItemCount[nItemIndex] + pData->GetNeedItemPossessionCount(nItemIndex);
		strTmp.PrintF("%d / %d", nTmpCount, pData->need_item[nItemIndex].need_num);
		pText->SetText(strTmp);
	}

	if (pText = (CUIText*)pArrayItem->findUI("text_possession_count"))
	{
		nTmpCount = nInvenItemCount - m_nTmpInvenItemCount[nItemIndex];
		strTmp.PrintF("%d", nTmpCount);
		pText->SetText(strTmp);
	}
}

void CItemCollectionUI::ClearNeed4ItemTempData()
{
	for (int i = 0; i < DEF_NEED_ITEM_COUNT; ++i)
	{
		m_nTmpPossessionItemCount[i] = 0;
		m_nTmpInvenItemCount[i] = 0;
	}
}

void CItemCollectionUI::SetThemeImageUV( CUIImage* pImg, int id, int row, int col )
{
	CTString strTex;
	strTex.PrintF("ItemCollectionBtn%d.tex", id);
	pImg->setTexString(strTex.str_String);

	int	nUVSX = DEF_THEME_IMAGE_SIZE;
	int	nUVSY = DEF_THEME_IMAGE_SIZE;

	nUVSX *= col;
	nUVSY *= row;

	const int nGap = 1;

	UIRectUV uv;
	uv.SetUV(nUVSX + nGap, nUVSY + nGap, nUVSX + DEF_THEME_IMAGE_SIZE - nGap, nUVSY + DEF_THEME_IMAGE_SIZE - nGap);
	pImg->SetUV(uv);
}

void CItemCollectionUI::UnCheck( CUICheckButton* pCheck )
{
	for (int i = 0; i < eCHECK_MAX; ++i)
	{
		if (pCheck != m_pCheck[i])
			m_pCheck[i]->SetCheck(FALSE);
	}

	UpdateThemeList();
	ChangeListTab(eTAB_THEME_LIST);
	UpdateScrollPos(0, 0);
}

void CItemCollectionUI::ChangeMainTab( int nTab )
{
	if (m_pTab == NULL)
		return;

	m_pTab->OnChangeTab(nTab);
	
	switch(nTab)
	{
	case eTAB_SUMMARY:
		{
			UpdateSummaryTab();
		}
		break;
	case eTAB_THEME:
		{
			ChangeListTab(eTAB_THEME_LIST);
		}
		break;
	}
}

void CItemCollectionUI::ChangeListTab( int nTab )
{
	if (m_pTreeTab == NULL)
		return;

	m_pTreeTab->OnChangeTab(nTab);

	switch(nTab)
	{
	case eTAB_THEME_LIST:
		{
			int nCat = 0;
			int nTheme = 0;

			if (GetTreeIndex(NULL, nCat, nTheme) == false)
				break;

			SelectTheme(CItemCollectionData::GetTheme(nCat, nTheme)->getindex());

			CItemCollectionData* pData = CItemCollectionData::GetTheme(nCat, nTheme);

			nCat = 0;
			nTheme = 0;
			if (GetTreeIndex(pData, nCat, nTheme) == false)
				break;

			SelectImage(nCat, nTheme);
		}
		break;
	case eTAB_SEARCH_LIST:
		{
			if (m_vecSearchList.empty() == false && m_vecSearchList[0].empty() == false)
			{
				SelectTheme(m_vecSearchList[0][0]->getindex());
				SelectImage(0, 0);
			}
		}
		break;
	}
}

CUITree* CItemCollectionUI::GetTreeItem(CUITree* pParent, int nCat, int nTheme )
{
	if (pParent == NULL)
		return NULL;

	int nCount = 0;

	nCount = pParent->getChildTreeCount();

	if (0 <= nCount && nCat >= nCount)
		return NULL;

	CUITree* pTree = pParent->getChildTree(nCat);

	if (pTree == NULL)
		return NULL;

	nCount = pTree->getChildTreeCount();

	if (0 <= nCount && nTheme >= nCount)
		return NULL;

	CUITree* pFind = pTree->getChildTree(nTheme);

	if (pFind != NULL)
		return pFind;
	return NULL;
}

void CItemCollectionUI::SelectImage( int nCat, int nTheme )
{
	int nTab = m_pTreeTab->getCurSelTab();
	CUITree* pTree = NULL;

	if (nTab == eTAB_THEME_LIST)
		pTree = GetTreeItem(m_pTreeThemeList, nCat, nTheme);
	else if (nTab == eTAB_SEARCH_LIST)
		pTree = GetTreeItem(m_pTreeSearchList, nCat, nTheme);

	if (pTree != NULL)
	{
		if (CUIImage* pImage = (CUIImage*)pTree->findUI("img_select"))
			pImage->Hide(FALSE);

		if (CUIText* pText = (CUIText*)pTree->findUI("text_name"))
			pText->setFontColor(0xFFEF9BFF);
	}

	CUITree* pOldTree = GetTreeItem(m_pOldTree, m_nOldCat, m_nOldTheme);

	if (nTab == eTAB_THEME_LIST)
		m_pOldTree = m_pTreeThemeList;
	else if (nTab == eTAB_SEARCH_LIST)
		m_pOldTree = m_pTreeSearchList;

	m_nOldCat = nCat;
	m_nOldTheme = nTheme;

	if (pOldTree == pTree)
		return;

	if (pOldTree != NULL)
	{
		if (CUIImage* pImage = (CUIImage*)pOldTree->findUI("img_select"))
			pImage->Hide(TRUE);

		if (CUIText* pText = (CUIText*)pOldTree->findUI("text_name"))
			pText->setFontColor(DEF_TREE_TEXT_COL);
	}
}

int CItemCollectionUI::CalcNeedItemPercentage( CItemCollectionData* pItem )
{
	if (pItem == NULL)
		return 0;

	int nNeed = 0;
	int nPossession = 0;
	for (int i = 0; i < DEF_NEED_ITEM_COUNT; ++i)
	{
		nNeed += pItem->need_item[i].need_num;
		nPossession += pItem->GetNeedItemPossessionCount(i);
	}

	if (nNeed > 0 && nPossession > 0)
		return ((float)nPossession / (float)nNeed) * 100.f;

	return 0;
}

int CItemCollectionUI::CalcNeedItemTotal( CItemCollectionData* pItem )
{
	if (pItem == NULL)
		return 0;
	
	int nNeed = 0;

	for (int i = 0; i < DEF_NEED_ITEM_COUNT; ++i)
		nNeed += pItem->need_item[i].need_num;

	return nNeed;
}

int CItemCollectionUI::CalcPossessionTotal( CItemCollectionData* pItem )
{
	if (pItem == NULL)
		return 0;

	int nPossession = 0;

	for (int i = 0; i < DEF_NEED_ITEM_COUNT; ++i)
		nPossession += pItem->GetNeedItemPossessionCount(i);

	return nPossession;
}

void CItemCollectionUI::OutputMessage( eOUTPUT_MSG eType )
{
	CTString strMsg;
	CTString strTitle;

	if (m_msgID > 0)
		MSGBOXMGR()->Close(m_msgID);

	switch(eType)
	{
	case OUTPUT_GIVE:
		{
			bool bEmpty = true;

			for (int i = 0; i < DEF_NEED_ITEM_COUNT; ++i)
			{
				if (m_nTmpPossessionItemCount[i] > 0)
					bEmpty = false;
			}

			if (bEmpty == true)
			{
				strTitle = _S(7054, "아이템 등록");
				strMsg = _S(7056, "등록 하려는 아이템이 없습니다.");
				m_msgID = MSGBOXMGR()->CreateMsgBox(strTitle, strMsg, eMSG_OK);
			}
			else
			{
				strTitle = _S(7054, "아이템 등록");
				strMsg = _S(7055, "등록 하신 아이템 수량 만큼 인벤토리에서 삭제 됩니다. 정말...");
				
				CmdItemCollectionAmend* pCmdOk = new CmdItemCollectionAmend;
				pCmdOk->setData(this, false);
				m_msgID = MSGBOXMGR()->CreateMsgBox(strTitle, strMsg, eMSG_YESNO, true, pCmdOk);
			}
		}
		break;
	case OUTPUT_AMEND:
		{
			CItemCollectionData* pData = CItemCollectionData::getData(m_nSelectTheme);

			if (pData == NULL)
				return;

			
			strTitle = _S(1721, "보상 받기");

			switch(pData->result_type)
			{
			case eETC_TYPE_ITEM:
				{
					CTString strName = _pNetwork->GetItemName(pData->result_index);
					strMsg.PrintF(_S(7057, "축하드립니다. 테마 완료 보상으로 아이템 %s 를 받을 수 있습니다. 지금 받으..."), strName.str_String);
				}
				break;
			case eETC_TYPE_NAS:
				strMsg = _S(7061, "축하드립니다. 테마 완료 보상으로 나스를 받을 수 있습니다. 지금 받으시겠습니까?");
				break;
			case eETC_TYPE_EXP:
				strMsg = _S(7063, "축하드립니다. 테마 완료 보상으로 경험치를 받을 수 있습니다. 지금 받으시겠습니까? ");
				break;
			case eETC_TYPE_SP:
				strMsg = _S(7062, "축하드립니다. 테마 완료 보상으로 숙련도를 받을 수 있습니다. 지금 받으시겠습니까?");
				break;
			}

			CmdItemCollectionAmend* pCmdOk = new CmdItemCollectionAmend;
			pCmdOk->setData(this, true);
			m_msgID = MSGBOXMGR()->CreateMsgBox(strTitle, strMsg, eMSG_YESNO, true, pCmdOk);
		}
		break;
	}
}

void CItemCollectionUI::SelectTheme( int nTheme )
{
	if (m_pTab != NULL && m_pTab->getCurSelTab() != eTAB_THEME)
		m_pTab->OnChangeTab(eTAB_THEME);

	m_nSelectTheme = nTheme;

	UpdateTheme();
	UpdateThemeNeed4ItemList();

	if (m_msgID > 0)
	{
		MSGBOXMGR()->Close(m_msgID);
	}
}

void CItemCollectionUI::ChangeTreeListEnterColor( int nCat, int nTheme, bool bEnter )
{
	if (m_nOldCat == nCat && m_nOldTheme == nTheme)
		return;

	int nTab = m_pTreeTab->getCurSelTab();
	CUITree* pTree = NULL;

	if (nTab == eTAB_THEME_LIST)
		pTree = GetTreeItem(m_pTreeThemeList, nCat, nTheme);
	else if (nTab == eTAB_SEARCH_LIST)
		pTree = GetTreeItem(m_pTreeSearchList, nCat, nTheme);

	if (pTree != NULL)
	{
		if (CUIText* pText = (CUIText*)pTree->findUI("text_name"))
		{
			if (bEnter == true)
				pText->setFontColor(0xDFB98FFF);
			else
				pText->setFontColor(DEF_TREE_TEXT_COL);
		}
	}
}

void CItemCollectionUI::UpdateScrollPos( int nCat, int nTheme )
{
	int nScrollPos = 0;
	CUITree* pTreeChild = NULL;

	for (int i = 0; i < nCat; ++i)
	{
		pTreeChild = m_pTreeThemeList->getChildTree(i);

		if (pTreeChild->getUncollapse() == FALSE)
			continue;

		if (pTreeChild == NULL)
			continue;

		nScrollPos += pTreeChild->getChildTreeCount() + 1;
	}

	pTreeChild = m_pTreeThemeList->getChildTree(nCat);

	if (pTreeChild != NULL && pTreeChild->getUncollapse() == FALSE)
	{
		pTreeChild->setUncollapse(TRUE);
	}

	nScrollPos += nTheme + nCat + 1;

	m_pTreeThemeList->SetScrollPos(nScrollPos);
}

bool CItemCollectionUI::GetTreeIndex(CItemCollectionData* pData, int& nCat, int& nTheme)
{
	if (m_pCheck[eCHECK_ING] == NULL || m_pCheck[eCHECK_COMPELTE] == NULL)
		return false;

	int nCatCount = CItemCollectionData::GetCategoryCount();
	nCat = 0;

	for (int i = 0; i < nCatCount; ++i)
	{
		int nThemeCount = CItemCollectionData::GetThemeCount(i);
		nTheme = 0;

		for (int j = 0; j < nThemeCount; ++j)
		{
			CItemCollectionData* pTheme = CItemCollectionData::GetTheme(i, j);

			if (m_pCheck[eCHECK_ING]->IsChecked() == TRUE)
			{
				if (pTheme->IsCompleteTheme() == true)
					continue;
				if (CalcPossessionTotal(pTheme) <= 0)
					continue;
			}
			else if (m_pCheck[eCHECK_COMPELTE]->IsChecked() == TRUE)
			{
				if (pTheme->IsCompleteTheme() == false)
					continue;
			}
			
			// 트리리스트의 첫번째 인덱스를 얻어오기 위해..
			if (pData == NULL)
			{
				nCat = i;
				nTheme = j;
				return true;
			}

			if (pTheme == pData)
				return true;

			++nTheme;
		}
		++nCat;	
	}

	return false;
}

bool compareTopList( CItemCollectionData* pSrc, CItemCollectionData* pDst )
{
	int nSrcPer = UIMGR()->GetItemCollection()->CalcNeedItemPercentage(pSrc);
	int nDstPer = UIMGR()->GetItemCollection()->CalcNeedItemPercentage(pDst);

	if (nSrcPer < nDstPer)
		return false;
	else if (nSrcPer == nDstPer)
	{
		int nSrcCat = 0;
		int nDstCat = 0;
		int nTempTheme = 0;

		CItemCollectionData::GetVectorIndex(pSrc, nSrcCat, nTempTheme);
		CItemCollectionData::GetVectorIndex(pDst, nDstCat, nTempTheme);

		int nSrcThemeComplete = CItemCollectionData::GetThemeCompleteCount(nSrcCat);
		int nDstThemeComplete = CItemCollectionData::GetThemeCompleteCount(nDstCat);

		if (nSrcThemeComplete < nDstThemeComplete)
			return false;
		else if (nSrcThemeComplete == nDstThemeComplete)
		{
			if (pSrc->getTheme() > pDst->getTheme())
				return false;
		}
	}

	return true;
}

bool CItemCollectionUI::ArrangeTopThemeList()
{
	m_vecTopThemeList.clear();

	int nCat = CItemCollectionData::GetCategoryCount();
	for (int i = 0; i < nCat; ++i)
	{
		int nTheme = CItemCollectionData::GetThemeCount(i);

		for (int j = 0; j < nTheme; ++j)
		{
			CItemCollectionData* pData = CItemCollectionData::GetTheme(i, j);

			if (pData == NULL)
				continue;

			if (pData->IsCompleteTheme())
				continue;

			m_vecTopThemeList.push_back(pData);
		}
	}

	if (m_vecTopThemeList.empty() == true)
		return false;

	std::sort(m_vecTopThemeList.begin(), m_vecTopThemeList.end(), compareTopList);

	return true;
}

bool CItemCollectionUI::IsCanRegistItem( int nItemIndex )
{
	CItemData* pItem = CItemData::getData(nItemIndex);

	if (pItem == NULL)
		return false;

	// 몬스터 용병카드
	if (pItem->GetType() == CItemData::ITEM_ETC && pItem->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD)
		return false;

	// 펫 협력 아이템
	if (pItem->GetType() == CItemData::ITEM_ETC && pItem->GetSubType() == CItemData::ITEM_PET_AI)
		return false;

	// 결합된 아이템외에 장비가 있는지 확인
	if (UIMGR()->GetInventory()->IsNotCompositionItem(nItemIndex) == false)
		return false;

	return true;
}
