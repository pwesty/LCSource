#include "stdh.h"
#include "UIQuestBookNew.h"
#include <Engine/GameDataManager/GameDataManager.h>
#include <vector>
#include <algorithm>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/Base/UIQuestAccept.h>
#include <Engine/Contents/Base/UIQuestComplete.h>

#define DEF_SELECT_QUEST_POSX 15
#define DEF_SELECT_QUEST_POSY -1

enum __tag_QUEST_TUTORIAL_RESPONSE
{
	RESPONSE_OK = 0,
	RESPONSE_CALCEL,
};

//--------------------------------------------------------------
// 퀘스트 북 선택 보상 셋팅
class CmdQuestBookPrizeItemSet : public Command
{
public:
	CmdQuestBookPrizeItemSet() : pUI_(NULL), nIdx_(-1) {}
	void setData(CUIQuestBook* pUI, int ItemIdx)	{ pUI_ = pUI; nIdx_ = ItemIdx;}
	void execute() {
		if (pUI_)
		{
			pUI_->SetPrizeItemIndex(nIdx_);
		}
	}
private:
	CUIQuestBook* pUI_;
	int nIdx_;
};

//--------------------------------------------------------------
// 퀘스트북 닫기
class CmdQuestBookClose : public Command
{
public:
	CmdQuestBookClose() : pUI_(NULL) {}
	void setData(CUIQuestBook* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->close();
	}
private:
	CUIQuestBook* pUI_;
};

//--------------------------------------------------------------
// 퀘스트북 보상
class CmdQuestBookOk : public Command
{
public:
	CmdQuestBookOk() : pUI_(NULL) {}
	void setData(CUIQuestBook* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->PrizeQuest();
	}
private:
	CUIQuestBook* pUI_;
};

//--------------------------------------------------------------
// 퀘스트북 포기
class CmdQuestBookGiveUp : public Command
{
public:
	CmdQuestBookGiveUp() : pUI_(NULL) {}
	void setData(CUIQuestBook* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->GiveUpPress();
	}
private:
	CUIQuestBook* pUI_;
};

//--------------------------------------------------------------
// Extend 연결
class CmdQuestBookExtend : public Command
{
public:
	CmdQuestBookExtend() : pUI_(NULL), eTreeType_(eTREE_NONE) {}
	void setData(CUITree* pUI, eTREE_TYPE eType)	{ pUI_ = pUI; eTreeType_ = eType; }
	void execute() {
		if (pUI_ && eTreeType_ >= 0 && eTreeType_ < eTREE_MAX)
		{
			BOOL bCurr = GAMEDATAMGR()->GetQuest()->GetExtend(eTreeType_);
			GAMEDATAMGR()->GetQuest()->SetExtend(!bCurr, eTreeType_);
			pUI_->updateCollapse();
		}
	}
private:
	CUITree* pUI_;
	eTREE_TYPE eTreeType_;
};

//--------------------------------------------------------------
// 퀘스트 선택
class CmdQuestBookSelect : public Command
{
public:
	CmdQuestBookSelect() : pUI_(NULL), QuestIdx_(-1) {}
	void setData(CUIQuestBook* pUI, INDEX Idx)	{ pUI_ = pUI; QuestIdx_ = Idx; }
	void execute() {
		if (pUI_)
		{
			pUI_->UpdateQuestContent(QuestIdx_);
		}
	}
private:
	CUIQuestBook* pUI_;
	INDEX QuestIdx_;
};

//--------------------------------------------------------------
// 미리보기 셋팅
class CmdQuestBookView : public Command
{
public:
	CmdQuestBookView() : pUI_(NULL), QuestIdx_(-1) {}
	void setData(CUICheckButton* pUI, INDEX Idx)	{ pUI_ = pUI; QuestIdx_ = Idx; }
	void execute() {
		if (pUI_)
		{			
			BOOL bCurr = pUI_->IsChecked();

			if (bCurr == TRUE)
			{
				if ( GAMEDATAMGR()->GetQuest()->GetSelectedCount() >= 6)
				{
					CUIMsgBox_Info	MsgBoxInfo;		
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );				
					MsgBoxInfo.AddString( _S(4694, "6개 이상 수행 리스트를 출력할 수 없습니다.") );
					CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );

					pUI_->SetCheck(FALSE);
					return;
				}

				GAMEDATAMGR()->GetQuest()->AddSelectedQuest(QuestIdx_);
			}
			else
				GAMEDATAMGR()->GetQuest()->RemoveSelectedQuest(QuestIdx_);
		}
	}
private:
	CUICheckButton* pUI_;
	INDEX QuestIdx_;
};


CUIQuestBook::CUIQuestBook()
	: m_pbtnClose(NULL)
	, m_pmoveArea(NULL)
	, m_plistContent(NULL)
	, m_ptbName(NULL)
	, m_pbtnOk(NULL)
	, m_pbtnCancle(NULL)
	, m_pbtnGiveUp(NULL)
	, m_pDesign(NULL)
	, m_plistScroll(NULL)
	, m_pstrQuestCnt(NULL)
	, m_pTreeRoot(NULL)
	, m_pTreeDesign(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
	, m_nTargetIndex(0)
	, m_nSelectQuestIdx(-1)
	, m_nPrizeItemIndex(-1)
	, m_bLockQuestList(FALSE)
{
	int i;
	for (i = 0; i < eICON_MAX; i++)
	{
		m_pImageArr[i] = NULL;
	}

	for (i = 0; i < eTREE_MAX; i++)
	{
		m_pTree[i] = NULL;
	}

	setInherit(false);
}

CUIQuestBook::~CUIQuestBook()
{
	SAFE_DELETE(m_pDesign);
	SAFE_DELETE(m_pTreeDesign);
	Destroy();
}

void CUIQuestBook::RefreshQuestListNew()
{
	CTString strTemp;

	m_pTreeRoot->deleteChildAll();
	m_pTreeRoot->clearChildList();
	m_mapQuestSelect.clear();

	CUIBase* pBase = m_pTreeDesign->findUI("tree_root");
	CUIBase* pTemp = m_pTreeDesign->findUI("tree_1");
	CUIBase* pTreeQuest = m_pTreeDesign->findUI("tree_quest");
	CUIText* pText = (CUIText*)pTemp->findUI("str_treename");
	int i;
	
	if (pBase == NULL || pTemp == NULL || pTreeQuest == NULL || pText == NULL)
		return;

	Quest* pQuest = GAMEDATAMGR()->GetQuest();

	if (pQuest == NULL)
		return;

	int nProceedCount = pQuest->GetListProceedCount();
	int nCompleteCount = pQuest->GetListCompleteCount();
	int nRaidCount = pQuest->GetListRaidCount();

	if (m_pstrQuestCnt != NULL)
	{
		strTemp.PrintF("%d / %d", nProceedCount + nCompleteCount, DEF_MAX_QUEST);
		m_pstrQuestCnt->SetText(strTemp);
	}

	// 진행중
	m_pTree[eTREE_PROCEED] = (CUITree*)pBase->Clone();
	m_pTree[eTREE_PROCEED]->InitPos(0, 0, 260, 20);
	if (pText != NULL)
	{
		strTemp.PrintF(_S( 1707, "진행중인 퀘스트 (%d)" ), nProceedCount);
		pText->InitPos(20, 0, 260, 16);
		pText->SetText(strTemp);
	}

	m_pTree[eTREE_PROCEED]->addChild(pTemp->Clone());
	m_pTreeRoot->addTree(m_pTree[eTREE_PROCEED]);

	// 진행중 퀘스트 내용 표시
	for (i = 0; i < nProceedCount; i++)
	{
		CUITree* pProceedQuest = (CUITree*)pTreeQuest->Clone();

		if (pProceedQuest == NULL)
			continue;

		CUIImage* pImg = (CUIImage*)m_pDesign->CloneSelectQuest();

		if (pImg != NULL)
		{
			pImg->Hide(TRUE);
			pProceedQuest->addChild(pImg);
		}

		CUICheckButton* pCheck = (CUICheckButton*)pProceedQuest->findUI("quest_veiw");
		CUIText* pQuestText = (CUIText*)pProceedQuest->findUI("quest_name");
		int nQuestIndex = pQuest->GetListProceedIndex(i);

		pImg = (CUIImage*)pProceedQuest->findUI("img_selectMenu");

		if (pImg != NULL)
		{
			pImg->SetPos(DEF_SELECT_QUEST_POSX, DEF_SELECT_QUEST_POSY);
			m_mapQuestSelect.insert(std::make_pair(nQuestIndex, pImg));
		}

		if (pCheck != NULL)
		{
			CmdQuestBookView* pCmd = new CmdQuestBookView;
			pCmd->setData(pCheck, nQuestIndex);
			pCheck->SetCommand(pCmd);
			if (pQuest->IsSelectedQuest(nQuestIndex) == true)
				pCheck->SetCheck(TRUE);
		}

		if (pQuestText != NULL)
		{			
			CTString strQuestTitle = pQuest->GetListProceedTitle(i);
			CQuestDynamicData* pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex( nQuestIndex );

			if( pQuestDD )
			{
				strTemp.PrintF("[%d] %s", pQuestDD->GetNeedMinLevel(), strQuestTitle);
				strTemp = MakeTitleString(strTemp, 30);
			}
			else
			{
				strTemp = MakeTitleString(strQuestTitle, 30);			
			}

			pQuestText->SetText(strTemp);

			CmdQuestBookSelect* pCmd = new CmdQuestBookSelect;
			pCmd->setData(this, nQuestIndex);
			pQuestText->SetCommandUp(pCmd);
		}

		CUITree* pChildTree = (CUITree*)pBase->Clone();
		pChildTree->InitPos(0, 0, 260, 16);
		pChildTree->addChild(pProceedQuest);

		pProceedQuest->InitPos(15, 0, 220, 16);
		m_pTree[eTREE_PROCEED]->addTree(pChildTree, false);	
	}
	
	// 완료
	m_pTree[eTREE_COMPLETE] = (CUITree*)pBase->Clone();
	m_pTree[eTREE_COMPLETE]->InitPos(0, 25, 260, 20);
	
	if (pText != NULL)
	{
		strTemp.PrintF(_S( 1708, "완료된 퀘스트 (%d)" ), nCompleteCount);
		pText->InitPos(20, 0, 260, 16);
		pText->SetText(strTemp);
	}

	m_pTree[eTREE_COMPLETE]->addChild(pTemp->Clone());
	m_pTreeRoot->addTree(m_pTree[eTREE_COMPLETE]);

	// 완료된 퀘스트 내용 표시
	for (i = 0; i < nCompleteCount; i++)
	{
		CUITree* pCompleteQuest = (CUITree*)pTreeQuest->Clone();

		if (pCompleteQuest == NULL)
			continue;

		CUIImage* pImg = (CUIImage*)m_pDesign->CloneSelectQuest();

		if (pImg != NULL)
		{
			pImg->Hide(TRUE);
			pCompleteQuest->addChild(pImg);
		}

		CUICheckButton* pCheck = (CUICheckButton*)pCompleteQuest->findUI("quest_veiw");
		CUIText* pQuestText = (CUIText*)pCompleteQuest->findUI("quest_name");
		int nQuestIndex = pQuest->GetListCompleteIndex(i);

		pImg = (CUIImage*)pCompleteQuest->findUI("img_selectMenu");

		if (pImg != NULL)
		{
			pImg->SetPos(DEF_SELECT_QUEST_POSX, DEF_SELECT_QUEST_POSY);
			m_mapQuestSelect.insert(std::make_pair(nQuestIndex, pImg));
		}

		if (pCheck != NULL)
		{
			CmdQuestBookView* pCmd = new CmdQuestBookView;
			pCmd->setData(pCheck, nQuestIndex);
			pCheck->SetCommand(pCmd);

			if (pQuest->IsSelectedQuest(nQuestIndex) == true)
				pCheck->SetCheck(TRUE);
		}

		if (pQuestText != NULL)
		{
			CTString strQuestTitle = pQuest->GetListCompleteTitle(i);
			CQuestDynamicData* pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex( nQuestIndex );

			if( pQuestDD )
			{
				strTemp.PrintF("[%d] %s", pQuestDD->GetNeedMinLevel(), strQuestTitle);
				strTemp = MakeTitleString(strTemp, 30);
			}
			else
			{
				strTemp = MakeTitleString(strQuestTitle, 30);
			}

			pQuestText->SetText(strTemp);
			CmdQuestBookSelect* pCmd = new CmdQuestBookSelect;
			pCmd->setData(this, nQuestIndex);
			pQuestText->SetCommandUp(pCmd);
		}

		CUITree* pChildTree = (CUITree*)pBase->Clone();
		pChildTree->InitPos(0, 0, 260, 16);
		pChildTree->addChild(pCompleteQuest);

		pCompleteQuest->InitPos(15, 0, 220, 16);
		m_pTree[eTREE_COMPLETE]->addTree(pChildTree, false);
	}

	// 레이드
	m_pTree[eTREE_RAID] = (CUITree*)pBase->Clone();
	m_pTree[eTREE_RAID]->InitPos(0, 50, 260, 20);
	
	if (pText != NULL)
	{
		strTemp.PrintF(_S( 4429, "레이드 진행 메시지 (%d)" ), nRaidCount);
		pText->InitPos(20, 0, 260, 16);
		pText->SetText(strTemp);
	}

	m_pTree[eTREE_RAID]->addChild(pTemp->Clone());
	m_pTreeRoot->addTree(m_pTree[eTREE_RAID]);

	// 레이드 내용 표시
	for (i = 0; i < nRaidCount; i++)
	{
		CUITree* pRaidQuest = (CUITree*)pTreeQuest->Clone();

		if (pRaidQuest == NULL)
			continue;

		CUIImage* pImg = (CUIImage*)m_pDesign->CloneSelectQuest();

		if (pImg != NULL)
		{
			pImg->Hide(TRUE);
			pRaidQuest->addChild(pImg);
		}

		CUICheckButton* pCheck = (CUICheckButton*)pRaidQuest->findUI("quest_veiw");
		CUIText* pQuestText = (CUIText*)pRaidQuest->findUI("quest_name");
		int nQuestIndex = pQuest->GetListRaidIndex(i);

		pImg = (CUIImage*)pRaidQuest->findUI("img_selectMenu");

		if (pImg != NULL)
		{
			pImg->SetPos(DEF_SELECT_QUEST_POSX, DEF_SELECT_QUEST_POSY);
			m_mapQuestSelect.insert(std::make_pair(nQuestIndex, pImg));
		}

		if (pCheck != NULL)
		{
			pCheck->Hide(TRUE);
		}

		if (pQuestText != NULL)
		{
			CTString strQuestTitle = pQuest->GetListRaidTitle(i);
			CQuestDynamicData* pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex( nQuestIndex );

			if( pQuestDD )
				strTemp.PrintF("%s", strQuestTitle);

			pQuestText->SetText(strTemp);
			CmdQuestBookSelect* pCmd = new CmdQuestBookSelect;
			pCmd->setData(this, nQuestIndex);
			pQuestText->SetCommandUp(pCmd);
		}

		CUITree* pChildTree = (CUITree*)pBase->Clone();
		pChildTree->InitPos(0, 0, 260, 16);
		pChildTree->addChild(pRaidQuest);

		pRaidQuest->InitPos(15, 0, 220, 16);
		m_pTree[eTREE_RAID]->addTree(pChildTree, false);
	}
	
	for (i = 0; i < eTREE_MAX; i++)
	{
		if (m_pTree[i] != NULL)
		{
			m_pTree[i]->setUncollapse(pQuest->GetExtend((eTREE_TYPE)i));

			CUICheckButton* pCheck = m_pTree[i]->getCheck();

			if (pCheck != NULL)
			{
				CmdQuestBookExtend* pCmd = new CmdQuestBookExtend;
				pCmd->setData(m_pTree[i], (eTREE_TYPE)i);
				pCheck->SetCommand(pCmd);
			}
		}
	}
	
	m_pTreeRoot->updateTree();
}

void CUIQuestBook::initContent()
{
	for (int i = 0; i < eICON_MAX; i++)
	{
		if(m_pImageArr[i] != NULL)
			m_pImageArr[i]->Hide(TRUE);
	}

	if (m_ptbName != NULL)
		m_ptbName->Hide(TRUE);

	if (m_plistContent != NULL)
		m_plistContent->Hide(TRUE);

	if (m_pbtnOk != NULL)
		m_pbtnOk->SetEnable(FALSE);

	if (m_pbtnGiveUp != NULL)
		m_pbtnGiveUp->SetEnable(FALSE);

	m_vecOptionPrize.clear();
}

void CUIQuestBook::UpdateQuestContent( int nQuestIdx )
{
	CUITooltipMgr::getSingleton()->setData(NULL);

	m_nSelectQuestIdx = nQuestIdx;

	std::map<int, CUIImage*>::iterator iter = m_mapQuestSelect.begin();
	std::map<int, CUIImage*>::iterator iter_end = m_mapQuestSelect.end();

	for (; iter != iter_end; ++iter)
	{
		(*iter).second->Hide(TRUE);
	}

	if (m_nSelectQuestIdx <= 0)
	{
		initContent();
		return;
	}

	std::map<int, CUIImage*>::iterator it = m_mapQuestSelect.find(m_nSelectQuestIdx);

	if (it != iter_end)
	{
		(*it).second->Hide(FALSE);
	}

	CQuestDynamicData* pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex( m_nSelectQuestIdx );
	CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(m_nSelectQuestIdx));

	if (GAMEDATAMGR()->GetQuest()->isRaidMessage(m_nSelectQuestIdx) == true)
	{		
		pQuestDD = &qdd;
	}

	// 타입 아이콘
	int nType[eICON_MAX];
	int nToolTipIdx[eICON_MAX];
	nType[eQUEST_TYPE1] = pQuestDD->GetQuestType1();
	nType[eQUEST_TYPE2] = pQuestDD->GetQuestType2();
	nType[eQUEST_SCALE] = pQuestDD->GetQuestPartyScale();

	nToolTipIdx[eQUEST_TYPE1] = nType1StringIdx[nType[eQUEST_TYPE1]];
	nToolTipIdx[eQUEST_TYPE2] = nType2StringIdx[nType[eQUEST_TYPE2]];
	nToolTipIdx[eQUEST_SCALE] = nScaleStringIdx[nType[eQUEST_SCALE]];

	CTString strTooltip;
	COLOR	tempColor;
	int		i;

	for (i = 0; i < eICON_MAX; i++)
	{		
		if(m_pImageArr[i] != NULL)
		{
			m_pImageArr[i]->SetRenderIdx(nType[i]);
			strTooltip.PrintF(_S(nToolTipIdx[i], "타입 툴팁"));
			m_pImageArr[i]->setTooltip(strTooltip.str_String);
			m_pImageArr[i]->Hide(FALSE);
		}
	}	

	// 퀘스트 이름
	if (m_ptbName != NULL)
	{
		tempColor = m_ptbName->GetTextColor();
		m_ptbName->SetAlignTextV(eALIGN_V_CENTER);
		m_ptbName->SetText( CTString(pQuestDD->GetName()), tempColor );
		m_ptbName->StringEllipsis(2);

		if (m_ptbName->IsEllipsis() == true)
		{
			m_ptbName->setTooltip(CTString(pQuestDD->GetName()));
		}
		else
		{
			m_ptbName->setTooltip(CTString(""));
		}
		m_ptbName->Hide(FALSE);
	}

	// List 내용
	if (m_plistContent == NULL)
		return;

	m_plistContent->Hide(FALSE);
	m_plistContent->DeleteAllListItem();
	m_vecOptionPrize.clear();

	if (m_plistScroll)
		m_plistScroll->SetScrollPos(0);

	CUITextBox* pTextBox = NULL;
	CUITextBox* pTempTb = NULL;
	CUIText* pText = NULL;
	CUIImage* pImg = NULL;
	CUIIcon* pIcon = NULL;
	CUIListItem* plistItem = NULL;
	CTString strTemp;

	// 1. Summury
	pTextBox = (CUITextBox*)m_pDesign->CloneSummury();

	if (pTextBox != NULL)
	{		
		pTextBox->SetText(CTString(pQuestDD->GetDesc3()));
		int nCnt = pTextBox->GetLineCount();

		for (i = 0; i < nCnt; ++i)
		{			
			pText = (CUIText*)m_pDesign->CloneSummuryText();

			if(pText != NULL)
			{
				strTemp = pTextBox->GetLineString(i);
				pText->SetText(strTemp);
				m_plistContent->AddListItem(pText);
			}
		}

		SAFE_DELETE(pTextBox);
		pText = (CUIText*)m_pDesign->CloneSpace();
		if (pText != NULL)
		{
			m_plistContent->AddListItem(pText);
		}
	}

	// 2. 진행 상황
	plistItem = (CUIListItem*)m_pDesign->CloneTitle();

	if (plistItem != NULL)
	{
		strTemp.PrintF(_S(1703, "진행상황"));
		pText = (CUIText*)plistItem->findUI("str_copytitle");

		if (pText != NULL)
			pText->SetText(strTemp);

		m_plistContent->AddListItem(plistItem);
	}

	// 진행 상황 내용
	int count = pQuestDD->GetCountStatusDesc();

	for (int index = 0; index < count; index++)
	{
		pTextBox = (CUITextBox*)m_pDesign->CloneDesc();

		if (pTextBox != NULL)
		{
			if (index != 0 && index % 2 == 1)
				tempColor = 0xFF0000FF;
			else
				tempColor = pTextBox->GetTextColor();

			pTextBox->SetText(CTString(pQuestDD->GetStatusDesc(index)), tempColor, TRUE);
			int nCnt = pTextBox->GetLineCount();

			for (i = 0; i < nCnt; ++i)
			{			
				pTempTb = (CUITextBox*)m_pDesign->CloneDesc();
				if(pTempTb != NULL)
				{
					strTemp = pTextBox->GetLineString(i);
					pTempTb->SetText(strTemp, tempColor, TRUE);
					m_plistContent->AddListItem(pTempTb);
				}
			}
			SAFE_DELETE(pTextBox);
		}
	}

	pText = (CUIText*)m_pDesign->CloneSpace();
	if (pText != NULL)
	{
		m_plistContent->AddListItem(pText);
	}

	// 3. Npc
	plistItem = (CUIListItem*)m_pDesign->CloneTitle();

	if (plistItem != NULL)
	{
		int NpcIdx = pQuestDD->GetPrizeNPCIndex();
		// Name
		if (NpcIdx > 0)
		{
			CMobData* pNpc = CMobData::getData(NpcIdx);
			strTemp.PrintF("%s", pNpc->GetName());
		}
		else
		{
			strTemp.PrintF(_S(457, "시스템"));
		}		
			
		pText = (CUIText*)plistItem->findUI("str_copytitle");

		if (pText != NULL)
			pText->SetText(strTemp);

		m_plistContent->AddListItem(plistItem);
	}

	// 4. StartDesc
	pTextBox = (CUITextBox*)m_pDesign->CloneDesc();

	if (pTextBox != NULL)
	{
		pTextBox->SetText(CTString(pQuestDD->GetDesc()));

		int nCnt = pTextBox->GetLineCount();

		for (i = 0; i < nCnt; ++i)
		{			
			pText = (CUIText*)m_pDesign->CloneDescText();

			if(pText != NULL)
			{
				strTemp = pTextBox->GetLineString(i);
				pText->SetText(strTemp);
				m_plistContent->AddListItem(pText);
			}
		}
		
		SAFE_DELETE(pTextBox);
		pText = (CUIText*)m_pDesign->CloneSpace();
		if (pText != NULL)
		{
			m_plistContent->AddListItem(pText);
		}
	}

	// 5. 수행가능 레벨
	plistItem = (CUIListItem*)m_pDesign->CloneTitle();

	if (plistItem != NULL)
	{
		strTemp.PrintF(_S( 1704, "수행가능 레벨" ));

		pText = (CUIText*)plistItem->findUI("str_copytitle");

		if (pText != NULL)
			pText->SetText(strTemp);

		m_plistContent->AddListItem(plistItem);
	}

	// 6. 레벨 값	
	pTextBox = (CUITextBox*)m_pDesign->CloneDesc();

	if (pTextBox != NULL)
	{
		if(pQuestDD->GetNeedMinLevel() == pQuestDD->GetNeedMaxLevel())
		{
			strTemp.PrintF(_S( 1705, "레벨 %d" ), pQuestDD->GetNeedMinLevel());
		}
		else if(pQuestDD->GetNeedMaxLevel()==999)
		{
			strTemp.PrintF(_S( 5667, "레벨 : %d ~ MAX" ), pQuestDD->GetNeedMinLevel());
		}
		else
		{
			strTemp.PrintF(_S( 1706, "레벨 %d ~ 레벨 %d" ), pQuestDD->GetNeedMinLevel(), pQuestDD->GetNeedMaxLevel());
		}

		pTextBox->SetText(strTemp);
		int nCnt = pTextBox->GetLineCount();

		for (i = 0; i < nCnt; ++i)
		{			
			pText = (CUIText*)m_pDesign->CloneDescText();
			if(pText != NULL)
			{
				strTemp = pTextBox->GetLineString(i);
				pText->SetText(strTemp);
				m_plistContent->AddListItem(pText);
			}
		}
		SAFE_DELETE(pTextBox);
		pText = (CUIText*)m_pDesign->CloneSpace();
		if (pText != NULL)
		{
			m_plistContent->AddListItem(pText);
		}
	}

	// 7. 퀘스트 보상	
	// 고정보상
	if(pQuestDD->IsPrizeExist())
	{
		// 보상 타이틀
		plistItem = (CUIListItem*)m_pDesign->CloneTitle();

		if (plistItem != NULL)
		{
			pText = (CUIText*)plistItem->findUI("str_copytitle");

			if (pText != NULL)
				pText->SetText(pQuestDD->GetPrizeDesc( 0 ));

			m_plistContent->AddListItem(plistItem);
		}

		// 보상 아이템		
		CUIImageBox* pImgBox = new CUIImageBox;
		int Count = pQuestDD->GetCountPrizeDesc();

		for( INDEX i = 1; i < Count; ++i )
		{
			plistItem = (CUIListItem*)m_pDesign->CloneListItem();

			if (plistItem == NULL)
				continue;

			// 아이템 설명
			CTString strPrize = pQuestDD->GetPrizeDesc( i );
			bool bIcon = true;
			UIBtnExType eBtnType = UBET_ITEM;

			pTextBox = (CUITextBox*)plistItem->findUI("tb_copyComplete");

			if (pTextBox != NULL)
			{
				tempColor = pTextBox->GetTextColor();
				pTextBox->SetText(strPrize, tempColor, TRUE);
			}

			// 아이콘인지, 이미지 인지 결정
			switch (pQuestDD->GetPrizeType(i-1) )
			{
			case QPRIZE_ITEM:
				eBtnType = UBET_ITEM;
				break;
			case QPRIZE_SKILL:
			case QPRIZE_SSKILL:
				eBtnType = UBET_SKILL;
				break;
			default:
				bIcon = false;
				break;
			}

			pIcon = (CUIIcon*)plistItem->findUI("icon_copyItem");
			pImg = (CUIImage*)plistItem->findUI("img_copyitem");

			// 아이템
			if (bIcon == true)
			{			
				if (pIcon != NULL)
				{
					pIcon->setData(eBtnType, pQuestDD->GetPrizeIndex(i-1));
					pIcon->Hide(FALSE);
				}
			}
			else
			{
				if (pImg != NULL)
				{					
					pImgBox->SetImageByType(CUIImageBox::eImageType(pQuestDD->GetPrizeType(i-1)), pQuestDD->GetPrizeIndex(i-1), pQuestDD->GetSyndicateType());
					UIRectUV pUv = pImgBox->GetUV();
					pImg->setTexData( _pUIBtnTexMgr->GetTex( UIBtnExType(pImgBox->GetBtnType()), pImgBox->GetTextureID() ));					
					pImg->SetUV(pUv.U0, pUv.V0, pUv.U1, pUv.V1);
					pImg->Hide(FALSE);
				}
			}

			m_plistContent->AddListItem(plistItem);
		}

		if (pImgBox)
		{
			delete pImgBox;
			pImgBox = NULL;
		}
	}

	//선택 보상
	if(pQuestDD->IsOptionPrizeExist())
	{
		// 보상 타이틀
		plistItem = (CUIListItem*)m_pDesign->CloneTitle();

		if (plistItem != NULL)
		{
			pText = (CUIText*)plistItem->findUI("str_copytitle");

			if (pText != NULL)
				pText->SetText(pQuestDD->GetOptionPrizeDesc(0));

			m_plistContent->AddListItem(plistItem);
		}

		// 보상 아이템		
		CUIImageBox* pImgBox = new CUIImageBox;
		int Count = pQuestDD->GetCountOptionPrizeDesc();

		for( INDEX i = 1; i < Count; ++i )
		{
			plistItem = (CUIListItem*)m_pDesign->CloneListItem();

			if (plistItem == NULL)
				continue;

			// 아이템 설명
			CTString strPrize = pQuestDD->GetOptionPrizeDesc( i );
			bool bIcon = true;
			UIBtnExType eBtnType = UBET_ITEM;

			pTextBox = (CUITextBox*)plistItem->findUI("tb_copyComplete");

			if (pTextBox != NULL)
			{
				tempColor = pTextBox->GetTextColor();
				pTextBox->SetText(strPrize, tempColor, TRUE);
			}

			// 아이콘인지, 이미지 인지 결정
			switch (pQuestDD->GetOptionPrizeType(i-1) )
			{
			case QPRIZE_ITEM:
				eBtnType = UBET_ITEM;
				break;
			case QPRIZE_SKILL:
			case QPRIZE_SSKILL:
				eBtnType = UBET_SKILL;
				break;
			default:
				bIcon = false;
				break;
			}

			pIcon = (CUIIcon*)plistItem->findUI("icon_copyItem");
			pImg = (CUIImage*)plistItem->findUI("img_copyitem");

			// 아이템
			if (bIcon == true)
			{			
				if (pIcon != NULL)
				{
					pIcon->setData(eBtnType, pQuestDD->GetOptionPrizeIndex(i-1));
					pIcon->Hide(FALSE);
				}
			}
			else
			{
				if (pImg != NULL)
				{					
					pImgBox->SetImageByType(CUIImageBox::eImageType(pQuestDD->GetOptionPrizeType(i-1)), pQuestDD->GetOptionPrizeIndex(i-1), pQuestDD->GetSyndicateType());
					UIRectUV pUv = pImgBox->GetUV();
					pImg->setTexData( _pUIBtnTexMgr->GetTex( UIBtnExType(pImgBox->GetBtnType()), pImgBox->GetTextureID() ));					
					pImg->SetUV(pUv.U0, pUv.V0, pUv.U1, pUv.V1);
					pImg->Hide(FALSE);
				}
			}

			pImg = (CUIImage*)plistItem->findUI("img_selectItem");

			if (pImg != NULL)
				m_vecOptionPrize.push_back(pImg);

			CmdQuestBookPrizeItemSet* pCmd = new CmdQuestBookPrizeItemSet;
			pCmd->setData(this, i - 1);
			plistItem->SetCommandUp(pCmd);

			m_plistContent->AddListItem(plistItem);
		}

		if (pImgBox)
		{
			delete pImgBox;
			pImgBox = NULL;
		}
	}

	int nMaxItem = m_plistContent->getListItemCount();
	m_plistContent->UpdateScroll(nMaxItem);
	m_plistContent->UpdateList();

	int nShowCnt = m_plistContent->GetItemShowNum();

	if (m_plistScroll != NULL)
		m_plistScroll->SetItemsPerPage(nShowCnt);

	if (m_pbtnGiveUp != NULL)
		m_pbtnGiveUp->SetEnable(TRUE);

	if (pQuestDD->GetPrizeNPCIndex() == 0 && GAMEDATAMGR()->GetQuest()->IsCompleteQuest(pQuestDD->GetQuestIndex()))
	{
		if (m_pbtnOk != NULL)
			m_pbtnOk->SetEnable(TRUE);
	}
}

void CUIQuestBook::PrizeQuest()
{
	CQuestDynamicData *pDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex(m_nSelectQuestIdx);
	if(pDD == NULL)
	{
		ASSERT(0 && "Something wrong");
		return;
	}

	INDEX iItemIndex = -1;
	INDEX iItemPlus = 0;
	if(pDD->IsOptionPrizeExist())
	{
		if ( m_nPrizeItemIndex < 0)
		{
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OK, UI_QUESTBOOK_COMPLETE, MSGCMD_QUEST_NOTIFY );
			CTString strMessage;
			strMessage.PrintF( _S( 1711, "받고 싶은 보상을 선택하여 주십시오." ) );
			MsgBoxInfo.AddString( strMessage );
			CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
			return;
		}

		iItemIndex = pDD->GetOptionPrizeIndex(m_nPrizeItemIndex);
		if(iItemIndex == -1)
		{
			ASSERT(0 && "Something wrong");
			return;
		}
		iItemPlus = pDD->GetOptionPrizePlus(m_nPrizeItemIndex);
	}

	if (pDD->GetPrizeNPCIndex() == 0) // 퀘스트 북 스크롤 보상
		GAMEDATAMGR()->GetQuest()->SendQuestPrize(m_nSelectQuestIdx, pDD->GetPrizeNPCIndex(), iItemIndex, iItemPlus);
	else
		GAMEDATAMGR()->GetQuest()->SendQuestPrize(m_nSelectQuestIdx, m_nTargetIndex, iItemIndex, iItemPlus);
}

void CUIQuestBook::GiveUpQuest()// 퀘스트 포기
{
	if(m_nSelectQuestIdx == -1) return;

	GAMEDATAMGR()->GetQuest()->SendQuestCancel(m_nSelectQuestIdx);
}

void CUIQuestBook::SetPrizeItemIndex( int nIndex )
{
	m_nPrizeItemIndex = nIndex;

	if ( m_nPrizeItemIndex < 0)
		return;

	if ( m_vecOptionPrize.size() <= m_nPrizeItemIndex )
		return;

	std::vector<CUIImage*>::iterator iter = m_vecOptionPrize.begin();
	std::vector<CUIImage*>::iterator iter_end = m_vecOptionPrize.end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Hide(TRUE);
	}

	m_vecOptionPrize[m_nPrizeItemIndex]->Hide(FALSE);
}

void CUIQuestBook::ToggleVisible()
{
	if( IsVisible() != FALSE)
		close();
	else
		open();
}

void CUIQuestBook::open(int nQuestIdx)
{
	SetVisible(TRUE);
	Hide(FALSE);

	RefreshQuestListNew();
	UpdateQuestContent(nQuestIdx);

	CQuestSystem::Instance().DeleteCurrentRequest();
	CUIManager::getSingleton()->RearrangeOrder( UI_QUESTBOOK_LIST, TRUE );
}

void CUIQuestBook::close()
{
	SetVisible(FALSE);
	Hide(TRUE);
	CUITooltipMgr::getSingleton()->setData(NULL);
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( UI_QUESTBOOK_LIST, FALSE );
}

void CUIQuestBook::initialize()
{
	CUIManager* pUIMgr = CUIManager::getSingleton();
	// design
	m_pDesign = new CUIQuestDesign;
	pUIMgr->LoadXML("questDesign.xml", (CUIBase*)m_pDesign);

	m_pbtnClose		= (CUIButton*)findUI("btn_close");

	if (m_pbtnClose != NULL)
	{
		CmdQuestBookClose* pCmd = new CmdQuestBookClose;
		pCmd->setData(this);
		m_pbtnClose->SetCommandUp(pCmd);
	}

	m_pmoveArea		= (CUIText*)findUI("move_title");
	m_ptbName		= (CUITextBox*)findUI("tb_name");
	m_plistContent	= (CUIList*)findUI("list_info");
	m_pbtnOk		= (CUIButton*)findUI("btn_ok");
	m_pstrQuestCnt	= (CUIText*)findUI("str_questCnt");

	if (m_pbtnOk != NULL)
	{
		CmdQuestBookOk* pCmd = new CmdQuestBookOk;
		pCmd->setData(this);
		m_pbtnOk->SetCommandUp(pCmd);
	}

	m_pbtnGiveUp		= (CUIButton*)findUI("btn_giveup");

	if (m_pbtnGiveUp != NULL)
	{
		CmdQuestBookGiveUp* pCmd = new CmdQuestBookGiveUp;
		pCmd->setData(this);
		m_pbtnGiveUp->SetCommandUp(pCmd);
	}

	m_pbtnCancle	= (CUIButton*)findUI("btn_cancle");

	if (m_pbtnCancle != NULL)
	{
		CmdQuestBookClose* pCmd = new CmdQuestBookClose;
		pCmd->setData(this);
		m_pbtnCancle->SetCommandUp(pCmd);
	}

	CTString strImageName[eICON_MAX] = { "iarr_type1", "iarr_type2", "iarr_scale"};

	for (int i = 0; i < eICON_MAX; i++)
	{
		m_pImageArr[i] = (CUIImageArray*)findUI(strImageName[i]);
	}

	m_plistScroll = (CUIScrollBar*)findUI("list_scroll");

	// tree
	m_pTreeDesign = new CUIBase;
	m_pTreeDesign->SetPos(0, 0);	
	pUIMgr->LoadXML("questTree.xml", m_pTreeDesign);
	CUITree* pBase = (CUITree*)m_pTreeDesign->findUI("tree_root");

	m_pTreeRoot = (CUITree*)pBase->Clone();
	m_pTreeRoot->setRoot();
	m_pTreeRoot->InitPos(25, 90, 265, 320);
	m_pTreeRoot->updateTree();
	addChild(m_pTreeRoot);
}

void CUIQuestBook::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2 - 44, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 - 44 );
}

void CUIQuestBook::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

CTString CUIQuestBook::MakeTitleString(CTString strTitle, int iSplitPos)
{
	if( strTitle.Length() <= iSplitPos )
		return strTitle;

	BOOL b2ByteChar = FALSE;

	for( int iPos = 0; iPos < iSplitPos; ++iPos )
	{
		if( strTitle[iPos] & 0x80 )
			b2ByteChar = !b2ByteChar;
		else
			b2ByteChar = FALSE;
	}

	if( b2ByteChar )
		iSplitPos--;

	CTString	strTemp, strTemp2;
	strTitle.Split( iSplitPos, strTemp, strTemp2 );

	strTemp += _s("...");

	return strTemp;
}

void CUIQuestBook::SelectQuestFromMessageBox(const int& iClassifierIndex)
{
	int iQuestIndex = iClassifierIndex - ciQuestClassifier;
	if( iQuestIndex <= 0 )
		return;

	SBYTE sbType = GetQuestType(iQuestIndex);	
	if( 1 == sbType )	// 수락
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		if( pUIManager->GetQuestBookList()->m_nTargetIndex != -1 )
		{
			pUIManager->GetQuestAccept()->SetPrevNpcIdx(pUIManager->GetQuestBookList()->m_nTargetIndex);
			pUIManager->GetQuestAccept()->SetPrevNpcUIType(pUIManager->GetQuestAccept()->GetNpcUIType());
		}
		else
		{
			pUIManager->GetQuestBookList()->m_nTargetIndex = pUIManager->GetQuestAccept()->GetPrevNpcIdx();			
		}

		pUIManager->GetQuestAccept()->open(iQuestIndex);
		pUIManager->GetQuestAccept()->SetTargetIndex(pUIManager->GetQuestBookList()->m_nTargetIndex);
		pUIManager->GetQuestBookList()->m_nTargetIndex = -1;		
	}
	else if( 2 == sbType ) // 보상
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		if( pUIManager->GetQuestBookList()->m_nTargetIndex != -1 )
		{
			pUIManager->GetQuestBookComplete()->SetPrevNpcIdx(pUIManager->GetQuestBookList()->m_nTargetIndex);
			pUIManager->GetQuestBookComplete()->SetPrevNpcUIType(pUIManager->GetQuestBookComplete()->GetNpcUIType());
			pUIManager->GetQuestAccept()->SetPrevNpcIdx(pUIManager->GetQuestBookList()->m_nTargetIndex);
			pUIManager->GetQuestAccept()->SetPrevNpcUIType(pUIManager->GetQuestAccept()->GetNpcUIType());
		}
		else
		{
			pUIManager->GetQuestBookList()->m_nTargetIndex = pUIManager->GetQuestBookComplete()->GetPrevNpcIdx();
		}

		pUIManager->GetQuestBookComplete()->open(iQuestIndex);
		pUIManager->GetQuestBookComplete()->SetTargetIndex(pUIManager->GetQuestBookList()->m_nTargetIndex);
		pUIManager->GetQuestBookList()->m_nTargetIndex = -1;
		pUIManager->GetQuestAccept()->SetTargetIndex(-1);
	}
}

void CUIQuestBook::TalkWithNPC()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(GAMEDATAMGR()->GetQuest()->GetCompleteQuestCount())
	{
		// NPC 메뉴로 돌아갈 것을 대비해 NPC Index 를 저장해 둔다.
		if( pUIManager->GetQuestBookList()->m_nTargetIndex != -1 )
		{
			pUIManager->GetQuestBookComplete()->SetPrevNpcIdx(pUIManager->GetQuestBookList()->m_nTargetIndex);
			pUIManager->GetQuestBookComplete()->SetPrevNpcUIType(pUIManager->GetQuestBookComplete()->GetNpcUIType());
		}
		else
		{
			pUIManager->GetQuestBookList()->m_nTargetIndex = pUIManager->GetQuestBookComplete()->GetPrevNpcIdx();			
		}

		pUIManager->GetQuestBookComplete()->SetTargetIndex(pUIManager->GetQuestBookList()->m_nTargetIndex);
		pUIManager->GetQuestBookList()->m_nTargetIndex = -1;
		pUIManager->GetQuestAccept()->SetTargetIndex(-1);
	}
	else if(GAMEDATAMGR()->GetQuest()->GetProceedQuestCount())
	{
		// NPC 메뉴로 돌아갈 것을 대비해 NPC Index 를 저장해 둔다.
		if( pUIManager->GetQuestBookList()->m_nTargetIndex != -1 )
		{
			pUIManager->GetQuestAccept()->SetPrevNpcIdx(pUIManager->GetQuestBookList()->m_nTargetIndex);
			pUIManager->GetQuestAccept()->SetPrevNpcUIType(pUIManager->GetQuestAccept()->GetNpcUIType());
		}
		else
		{
			pUIManager->GetQuestBookList()->m_nTargetIndex = pUIManager->GetQuestAccept()->GetPrevNpcIdx();			
		}

		pUIManager->GetQuestAccept()->SetTargetIndex(pUIManager->GetQuestBookList()->m_nTargetIndex);
		pUIManager->GetQuestBookList()->m_nTargetIndex = -1;
	}
	else
	{
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OK, UI_QUEST_ACCEPT, MSGCMD_QUEST_NOTIFY );
		CTString strMessage;
		strMessage.PrintF( _S( 584, "수행할 수 있는 퀘스트가 없습니다." ) );					
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
}

void CUIQuestBook::AddQuestListToMessageBoxL(const int& iMessageBoxType)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	Quest* pQuest = GAMEDATAMGR()->GetQuest();

	if (pQuest == NULL)
		return;

	int iQuestIndex = -1;
	CTString strQuestTitle;
	int count;

	count = pQuest->GetCompleteQuestCount();

	for( int iComplete = 0; iComplete < count; ++iComplete )
	{
		iQuestIndex = pQuest->GetCompleteQuestIndex(iComplete);

		CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));				
		strQuestTitle = qdd.GetName();

		if( 0 == strQuestTitle.Length() )
		{
			if (_pNetwork->m_ubGMLevel > 1)
				strQuestTitle.PrintF("Index : %d", iQuestIndex);
			else
				continue;
		}

		pUIManager->AddMessageBoxLString( iMessageBoxType, FALSE, strQuestTitle, ciQuestClassifier + iQuestIndex, 0xF2F200FF, CTString("A") );
	}

	count = pQuest->GetProceedQuestCount();

	for( int iNew = 0; iNew < count; ++iNew )
	{
		iQuestIndex = pQuest->GetProceedQuestIndex(iNew);

		CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));				
		strQuestTitle = qdd.GetName();

		if( 0 == strQuestTitle.Length() )
		{
			if (_pNetwork->m_ubGMLevel > 1)
				strQuestTitle.PrintF("Index : %d", iQuestIndex);
			else
				continue;
		}

		pUIManager->AddMessageBoxLString( iMessageBoxType, FALSE, strQuestTitle, ciQuestClassifier + iQuestIndex, 0xF2F200FF, CTString("Q") );
	}
}

SBYTE CUIQuestBook::GetQuestType(const INDEX& iQuestIndex)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	Quest* pQuest = GAMEDATAMGR()->GetQuest();

	if (pQuest == NULL)
		return 0;

	int iTempIndex = -1;
	int count;

	count = pQuest->GetCompleteQuestCount();

	for( int iComplete = 0; iComplete < count; ++iComplete )
	{
		iTempIndex = pQuest->GetCompleteQuestIndex(iComplete);

		if( iQuestIndex == iTempIndex )
			return 2;
	}

	count = pQuest->GetProceedQuestCount();
	for( int iNew = 0; iNew < count; ++iNew )
	{
		iTempIndex = pQuest->GetProceedQuestIndex(iNew);

		if( iQuestIndex == iTempIndex )
			return 1;
	}

	return 0;
}

WMSG_RESULT CUIQuestBook::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (m_bLockQuestList == TRUE)
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

WMSG_RESULT CUIQuestBook::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (m_bLockQuestList == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pmoveArea && m_pmoveArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_QUESTBOOK_LIST, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CUIQuestBook::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (m_bLockQuestList == TRUE)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

//////////////////////////////////////////////////////////////////////////

void CUIQuestBook::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{	
	switch( nCommandCode )
	{
	case MSGCMD_QUEST_GIVEUP:		// 퀘스트 포기
		{
			if(bOK)
			{
				GiveUpQuest();
			}
			m_bLockQuestList = FALSE;
		}
		break;

	case MSGCMD_QUEST_TUTORIAL_START:
		{
			if( bOK == FALSE )
				OpenTutorialWindow();
		}
		break;

	case MSGCMD_QUEST_TUTORIAL_CANCEL:
		{
			if( bOK )
				GAMEDATAMGR()->GetQuest()->SendQuestCancel(45);
			else
				OpenTutorialWindow();
		}
		break;
	}
}
void CUIQuestBook::MsgBoxLCommand(int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_QUEST_TUTORIAL_OPEN:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( nResult == RESPONSE_OK )
			{
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 928, "튜토리얼" ), UMBS_OKCANCEL, UI_QUESTBOOK_LIST, MSGCMD_QUEST_TUTORIAL_START );
				MsgBoxInfo.AddString( _S( 5042, "튜토리얼을 진행하시겠습니까?" ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}

			if( nResult == RESPONSE_CALCEL )
			{
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 928, "튜토리얼" ), UMBS_OKCANCEL, UI_QUESTBOOK_LIST, MSGCMD_QUEST_TUTORIAL_CANCEL );
				MsgBoxInfo.AddString( _S( 5043, "튜토리얼에서 나가시겠습니까?" ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
		}
		break;
	}
}

void CUIQuestBook::OpenTutorialWindow()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is alreay exist.
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_QUEST_TUTORIAL_OPEN ) )
		return;

	pUIManager->CloseMessageBox(MSGCMD_QUEST_TUTORIAL_START);
	pUIManager->CloseMessageBox(MSGCMD_QUEST_TUTORIAL_CANCEL);

	// Create refine message box.
	pUIManager->CreateMessageBoxL( _S( 928,"튜토리얼" ), UI_QUESTBOOK_LIST, MSGLCMD_QUEST_TUTORIAL_OPEN );

	pUIManager->AddMessageBoxLString( MSGLCMD_QUEST_TUTORIAL_OPEN, TRUE, _S( 928, "튜토리얼" ), -1, 0xE18600FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_QUEST_TUTORIAL_OPEN, TRUE, _S( 5039, "라스트카오스에 오신것을 환영합니다.\n이곳은 처음 접속하시는 유저분들을 위해\n기본 조작 방법을 안내하는 곳입니다.\n\n앞에 있는 문을 열고 통로를 따라가시면 각종\n몬스터가 등장하고, 통로 끝에 존재하는 몬스터를\n사냥하면 쥬노 란돌 마을로 이동하실 수 있습니다." ), -1, 0xA3A1A3FF );

	CTString strMessage;

	// accept
	strMessage.PrintF( _S( 5040, "확인 (튜토리얼을 진행합니다.)" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_QUEST_TUTORIAL_OPEN, FALSE, strMessage, RESPONSE_OK );

	// cancel.
	strMessage.PrintF( _S( 5041, "나가기 (튜토리얼을 진행하지 않습니다.)") );
	pUIManager->AddMessageBoxLString( MSGLCMD_QUEST_TUTORIAL_OPEN, FALSE, strMessage, RESPONSE_CALCEL );
}

void CUIQuestBook::GiveUpPress()
{
	if (m_nSelectQuestIdx <= 0)
		return;

	CQuestStaticData* pQuestDD = CQuestStaticData::getData(m_nSelectQuestIdx);
	if (pQuestDD == NULL)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->CloseMessageBox(MSGCMD_QUEST_GIVEUP);
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;	
	MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OKCANCEL, UI_QUESTBOOK_LIST, MSGCMD_QUEST_GIVEUP);	
	strMessage.PrintF( "%s", pQuestDD->GetTitle() );
	MsgBoxInfo.AddString( strMessage );
	strMessage.PrintF( _S( 7002, "퀘스트를 포기 하시겠습니까?" ));
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	m_bLockQuestList = TRUE;
}
