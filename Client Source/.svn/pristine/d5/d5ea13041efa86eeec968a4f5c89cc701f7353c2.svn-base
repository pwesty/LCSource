#include "stdh.h"
#include "UIQuestView.h"
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>

//--------------------------------------------------------------
// 미리보기창 닫기
class CmdQuestViewClose : public Command
{
public:
	CmdQuestViewClose() : pUI_(NULL) {}
	void setData(CUIQuestView* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->close();
	}
private:
	CUIQuestView* pUI_;
};

//--------------------------------------------------------------
// 미리보기창 다음 페이지
class CmdQuestViewNextPage : public Command
{
public:
	CmdQuestViewNextPage() : pUI_(NULL) {}
	void setData(CUIQuestView* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->NextPage();
	}
private:
	CUIQuestView* pUI_;
};

//--------------------------------------------------------------
// 미리보기창 이전 페이지
class CmdQuestViewPrevPage : public Command
{
public:
	CmdQuestViewPrevPage() : pUI_(NULL) {}
	void setData(CUIQuestView* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->PrevPage();
	}
private:
	CUIQuestView* pUI_;
};

//--------------------------------------------------------------
// 미리보기창 퀘스트 선택 해제
class CmdQuestViewUnSelect : public Command
{
public:
	CmdQuestViewUnSelect() : pUI_(NULL), nQuestIdx_(-1) {}
	void setData(CUIQuestView* pUI, int QuestIdx)	{ pUI_ = pUI; nQuestIdx_ = QuestIdx;}
	void execute() {
		if (pUI_)
			pUI_->UnSelectQuest(nQuestIdx_);
	}
private:
	CUIQuestView* pUI_;
	int nQuestIdx_;
};

//--------------------------------------------------------------
// 미리보기창 퀘스트 링크
class CmdQuestViewLink : public Command
{
public:
	CmdQuestViewLink() : pUI_(NULL), nQuestIdx_(-1) {}
	void setData(CUIQuestView* pUI, int QuestIdx)	{ pUI_ = pUI; nQuestIdx_ = QuestIdx;}
	void execute() {
		if (pUI_)
			pUI_->LinkQuestBook(nQuestIdx_);
	}
private:
	CUIQuestView* pUI_;
	int nQuestIdx_;
};

//--------------------------------------------------------------
// 미리보기창 네임 오버
class CmdQuestViewNameColor : public Command
{
public:
	CmdQuestViewNameColor() : pUI_(NULL), color_(0xFFFFFFFF) {}
	void setData(CUIText* pUI, COLOR color)	{ pUI_ = pUI; color_ = color;}
	void execute() {
		if (pUI_)
			pUI_->setFontColor(color_);
	}
private:
	CUIText* pUI_;
	COLOR color_;
};

CUIQuestView::CUIQuestView()
	: m_pmoveArea(NULL)
	, m_pbtnClose(NULL)
	, m_pbtnLeft(NULL)
	, m_pbtnRight(NULL)
	, m_pstrViewCount(NULL)
	, m_pDesign(NULL)
	, m_plistContent(NULL)
	, m_pisBack(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
{
}

CUIQuestView::~CUIQuestView()
{
	SAFE_DELETE(m_pDesign);
	Destroy();
}

void CUIQuestView::LinkQuestBook( int QuestIdx )
{
	UIMGR()->GetQuestBookList()->open(QuestIdx);
}

void CUIQuestView::NextPage()
{
	if (GAMEDATAMGR()->GetQuest()->NextQuestViewPage() == false)
	{
		// 에러 메시지
	}
}

void CUIQuestView::PrevPage()
{
	if (GAMEDATAMGR()->GetQuest()->PrevQuestViewPage() == false)
	{
		// 에러 메시지
	}
}

void CUIQuestView::UnSelectQuest(int QuestIdx)
{
	GAMEDATAMGR()->GetQuest()->RemoveSelectedQuest(QuestIdx);
}

BOOL CUIQuestView::UpdateContent()
{
	Quest* pQuest = GAMEDATAMGR()->GetQuest();

	if (pQuest == NULL)
		return FALSE;

	CTString strTemp;

	int nSelectCount = pQuest->GetSelectedCount();
	int nMaxPage = nSelectCount / DEF_PAGE_VIEW_COUNT;
	int nAddCount = nSelectCount % DEF_PAGE_VIEW_COUNT;

	if (nSelectCount <= 0)
	{
		close();
		return FALSE;
	}
	
	if (nAddCount > 0)
		nMaxPage++;
	
	int nCurrPage = pQuest->GetCurrViewPage();
	
	// 페이지 갯수
	if (m_pstrViewCount != NULL)
	{
		strTemp.PrintF("%d / %d", nCurrPage + 1, nMaxPage);
		m_pstrViewCount->SetText(strTemp);
	}

	// 리스트 내용
	if (m_plistContent == NULL)
		return FALSE;

	if (m_pDesign == NULL)
		return FALSE;

	m_plistContent->DeleteAllListItem();

	if (nSelectCount <= 0)
		return FALSE;

	int nStartIdx = nCurrPage * DEF_PAGE_VIEW_COUNT;
	int nEndIdx = (nCurrPage + 1) * DEF_PAGE_VIEW_COUNT;
	CUIListItem* pTempItem = (CUIListItem*)m_pDesign->CloneListViewItem();

	if (pTempItem == NULL)
		return FALSE;

	CUIText* pText = NULL;
	CUIButton* pBtn = NULL;
	CUIText* pCopyText = NULL;
	CUIButton* pCopyBtn = NULL;
	CUIImage* pImg = NULL;

	BOOL bOpen = FALSE;

	for (int i = nStartIdx; i < nEndIdx; i++)
	{
		int nQuestIdx = pQuest->GetSelectedQuestIndex(i);

		if (nQuestIdx <= 0)
			continue;

		CQuestDynamicData* pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex( nQuestIdx );
		CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(nQuestIdx));

		if (pQuest->isRaidMessage(nQuestIdx) == true)
		{
			pQuestDD = &qdd;
		}

		if (pQuestDD == NULL)
			continue;

		bOpen = TRUE;
		
		CUIListItem* pItem = new CUIListItem;
		int nItemTotalHeight = 0;
		// 버튼 셋팅
		pCopyBtn = (CUIButton*)pTempItem->findUI("btn_view");

		if (pCopyBtn != NULL)
			pItem->addChild(pCopyBtn->Clone());
		
		pBtn = (CUIButton*)pItem->findUI("btn_view");
		
		if (pBtn)
		{
			CmdQuestViewUnSelect* pCmd = new CmdQuestViewUnSelect;
			pCmd->setData(this, nQuestIdx);
			pBtn->SetCommandUp(pCmd);
		}

		// 퀘스트 이름
		pCopyText = (CUIText*)pTempItem->findUI("str_name");

		if (pCopyText)
		{
			pItem->addChild(pCopyText->Clone());
			nItemTotalHeight += pCopyText->GetHeight();
		}

		pText = (CUIText*)pItem->findUI("str_name");

		if (pText)
		{
			strTemp.PrintF("[%d] %s", pQuestDD->GetNeedMinLevel(), pQuestDD->GetName());
			strTemp = UtilHelp::getSingleton()->GetCalcStringEllipsis(strTemp, pText->GetWidth(), CTString("..."));
			pText->SetText(strTemp);
			
			CmdQuestViewLink* pCmd = new CmdQuestViewLink;
			pCmd->setData(this, nQuestIdx);
			pText->SetCommand(pCmd);

			CmdQuestViewNameColor* pCmdOver = new CmdQuestViewNameColor;
			pCmdOver->setData(pText, 0xffff00ff);
			pText->SetCommandEnter(pCmdOver);

			CmdQuestViewNameColor* pCmdLeave = new CmdQuestViewNameColor;
			pCmdLeave->setData(pText, 0xffffffff);
			pText->SetCommandLeave(pCmdLeave);
		}

		// 완료 상태
		if (pQuest->IsCompleteQuest(nQuestIdx) == true)
		{
			pImg = (CUIImage*)pTempItem->findUI("img_complete");

			if (pImg)
				pItem->addChild(pImg->Clone());
			
			pImg = (CUIImage*)pItem->findUI("img_complete");

			if (pImg)
			{
				CTString strTemp2;
				strTemp2.PrintF("%s", _S(429, "완료"));
				strTemp.PrintF("[%s]", strTemp2);
				pImg->setTooltip(strTemp);
			}
		}

		// 진행 상태
		int Statuscount = pQuestDD->GetCountStatusDesc();
		CTString strPurpose[6] = { "str_purpose1", "str_count1", "str_purpose2", "str_count2", "str_purpose3", "str_count3"};

		for (int index = 0; index < Statuscount; index++)
		{
			pCopyText = (CUIText*)pTempItem->findUI(strPurpose[index]);
						
			if (pCopyText != NULL)
			{
				pItem->addChild(pCopyText->Clone());
				nItemTotalHeight += pCopyText->GetHeight();
			}

			pText = (CUIText*)pItem->findUI(strPurpose[index]);
			
			if (pText != NULL)
			{
				strTemp.PrintF("%s", pQuestDD->GetStatusDesc(index));
				strTemp = UtilHelp::getSingleton()->GetCalcStringEllipsis(strTemp, pText->GetWidth(), CTString("..."));
				pText->SetText(strTemp);
			}			
		}	

		pItem->InitPos(3, 2, 280, nItemTotalHeight);
		m_plistContent->AddListItem(pItem);
	}

	SAFE_DELETE(pTempItem);
	
	m_plistContent->UpdateList();
	
	if (m_pisBack != NULL)
	{
		m_pisBack->SetHeight(m_plistContent->GetItemTotalHeight());
		m_pisBack->UpdateSplit();
	}

	return bOpen;
}

void CUIQuestView::initialize()
{
	CUIManager* pUIMgr = CUIManager::getSingleton();
	// design
	m_pDesign = new CUIQuestDesign;
	pUIMgr->LoadXML("questDesign.xml", (CUIBase*)m_pDesign);

	m_pisBack		= (CUIImageSplit*)findUI("is_back");
	m_pbtnClose		= (CUIButton*)findUI("btn_close");
	m_pstrViewCount = (CUIText*)findUI("str_count");

	if (m_pbtnClose != NULL)
	{
		CmdQuestViewClose* pCmd = new CmdQuestViewClose;
		pCmd->setData(this);
		m_pbtnClose->SetCommandUp(pCmd);
	}

	m_pmoveArea		= (CUIText*)findUI("move_title");
	m_pbtnLeft		= (CUIButton*)findUI("btn_left");

	if (m_pbtnLeft != NULL)
	{
		CmdQuestViewPrevPage* pCmd = new CmdQuestViewPrevPage;
		pCmd->setData(this);
		m_pbtnLeft->SetCommandUp(pCmd);
	}

	m_pbtnRight		= (CUIButton*)findUI("btn_right");

	if (m_pbtnRight != NULL)
	{
		CmdQuestViewNextPage* pCmd = new CmdQuestViewNextPage;
		pCmd->setData(this);
		m_pbtnRight->SetCommandUp(pCmd);
	}

	m_plistContent = (CUIList*)findUI("list_content");
}

void CUIQuestView::open()
{
	SetVisible(TRUE);
	Hide(FALSE);
	
	CUIManager::getSingleton()->RearrangeOrder( UI_QUEST_VIEW, UpdateContent() );
}

void CUIQuestView::close()
{
	SetVisible(FALSE);
	Hide(TRUE);

	GAMEDATAMGR()->GetQuest()->ClearSelectedQuestList();

	CUIManager::getSingleton()->RearrangeOrder( UI_QUEST_VIEW, FALSE );
}

void CUIQuestView::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI - GetWidth() ) - BTN_SIZE , 270);
}

void CUIQuestView::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

WMSG_RESULT CUIQuestView::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_nMouseX = x;
	m_nMouseY = y;

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

WMSG_RESULT CUIQuestView::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->RearrangeOrder( UI_QUEST_VIEW, TRUE );

	if( m_pmoveArea && m_pmoveArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
		return WMSG_SUCCESS; // 드래그 도중 캐릭터 이동을 막기 위하여
	}
	return WMSG_FAIL;
}

WMSG_RESULT CUIQuestView::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

