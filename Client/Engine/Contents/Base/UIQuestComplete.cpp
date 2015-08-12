#include "stdh.h"
#include "UIQuestComplete.h"
#include <Engine/GameDataManager/GameDataManager.h>
#include <vector>
#include <algorithm>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Contents/Base/UIQuestAccept.h>

//--------------------------------------------------------------
// 완료창 닫기
class CmdQuestCompleteClose : public Command
{
public:
	CmdQuestCompleteClose() : pUI_(NULL) {}
	void setData(CUIQuestComplete* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->close();
	}
private:
	CUIQuestComplete* pUI_;
};



//--------------------------------------------------------------
// 완료 버튼
class CmdQuestCompleteOk : public Command
{
public:
	CmdQuestCompleteOk() : pUI_(NULL) {}
	void setData(CUIQuestComplete* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
		{
			pUI_->PrizeQuest();
		}
	}
private:
	CUIQuestComplete* pUI_;
};



//--------------------------------------------------------------
// npc 메뉴로 돌아감
class CmdQuestCompleteReturnNpcMenu : public Command
{
public:
	CmdQuestCompleteReturnNpcMenu() : pUI_(NULL) {}
	void setData(CUIQuestComplete* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
		{
			pUI_->ReturnToNPCMenu();
		}
	}
private:
	CUIQuestComplete* pUI_;
};

//--------------------------------------------------------------
// 선택 보상 셋팅
class CmdQuestPrizeItemSet : public Command
{
public:
	CmdQuestPrizeItemSet() : pUI_(NULL), nIdx_(-1) {}
	void setData(CUIQuestComplete* pUI, int ItemIdx)	{ pUI_ = pUI; nIdx_ = ItemIdx;}
	void execute() {
		if (pUI_)
		{
			pUI_->SetPrizeItemIndex(nIdx_);
		}
	}
private:
	CUIQuestComplete* pUI_;
	int nIdx_;
};

//////////////////////////////////////////////////////////////////////////
// 퀘스트 완료창

CUIQuestComplete::CUIQuestComplete()
	: m_pbtnClose(NULL)
	, m_pmoveArea(NULL)
	, m_plistContent(NULL)
	, m_ptbName(NULL)
	, m_pbtnOk(NULL)
	, m_pbtnCancle(NULL)
	, m_pbtnPrev(NULL)
	, m_pDesign(NULL)
	, m_plistScroll(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
	, m_nTargetIndex(0)
	, m_nQuestIdx(-1)
	, m_nPrizeItemIndex(-1)
{
	for (int i = 0; i < eICON_MAX; i++)
	{
		m_pImageArr[i] = NULL;
	}

	setInherit(false);
}

CUIQuestComplete::~CUIQuestComplete()
{
	SAFE_DELETE(m_pDesign);
	Destroy();
}

void CUIQuestComplete::PrizeQuest()
{
	CQuestDynamicData *pDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex(m_nQuestIdx);
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
		GAMEDATAMGR()->GetQuest()->SendQuestPrize(m_nQuestIdx, pDD->GetPrizeNPCIndex(), iItemIndex, iItemPlus);
	else
		GAMEDATAMGR()->GetQuest()->SendQuestPrize(m_nQuestIdx, m_nTargetIndex, iItemIndex, iItemPlus);
}

void CUIQuestComplete::UpdateQuest()
{
	if(m_nQuestIdx == -1)
		return;

	CQuestDynamicData* pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex( m_nQuestIdx );

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

	m_plistContent->DeleteAllListItem();
	if (m_plistScroll)
		m_plistScroll->SetScrollPos(0);

	CUITextBox* pTextBox = NULL;
	CUITextBox* pTempTb = NULL;
	CUIText* pText = NULL;
	CUIImage* pImg = NULL;
	CUIIcon* pIcon = NULL;
	CUIListItem* plistItem = NULL;
	CTString strTemp;
		
	// 1. Npc
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

	// 2. PrizeDesc
	pTextBox = (CUITextBox*)m_pDesign->CloneDesc();

	if (pTextBox != NULL)
	{
		pTextBox->SetText(CTString(pQuestDD->GetDesc2()), tempColor, TRUE);

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
	
	// 3. 퀘스트 보상	
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
				pText->SetText(pQuestDD->GetOptionPrizeDesc( 0 ));

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

			CmdQuestPrizeItemSet* pCmd = new CmdQuestPrizeItemSet;
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
}

void CUIQuestComplete::SetPrizeItemIndex( int nIndex )
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

void CUIQuestComplete::open( int nQuestIdx)
{
	if (nQuestIdx == -1) return;

	SetVisible(TRUE);
	Hide(FALSE);

	m_nQuestIdx = nQuestIdx;
	
	BOOL bEnable = TRUE;

	if (71 == m_nPrevNPCIndex || m_nPrevNPCIndex < 0)
		bEnable = FALSE;
	
	if (m_pbtnPrev != NULL)
	{
		m_pbtnPrev->SetEnable(bEnable);
	}

	// NOTE : 차원의 문은 보상 인터페이스가 뜨면 안됩니다.
	if( nQuestIdx == 105 )
	{
		// FIXME : NPC 인덱스까지 하드 코딩...ㅡ.ㅡ
		m_nTargetIndex	= 71;
		PrizeQuest();
		CQuestSystem::Instance().DeleteCurrentRequest();
		return;
	}

	UpdateQuest();

	CQuestSystem::Instance().DeleteCurrentRequest();
	CUIManager::getSingleton()->RearrangeOrder( UI_QUESTBOOK_COMPLETE, TRUE );
}

void CUIQuestComplete::Clear()
{
	m_nQuestIdx = -1;
	m_nTargetIndex = -1;
	m_nTargetUIType = -1;
	m_nPrevNPCIndex = -1;
	m_nPrevNPCUIType = -1;
	m_nPrizeItemIndex = -1;

	m_vecOptionPrize.clear();
}

void CUIQuestComplete::close()
{
	SetVisible(FALSE);
	Hide(TRUE);

	Clear();
	CUITooltipMgr::getSingleton()->setData(NULL);
	CUIManager* pUIManager = CUIManager::getSingleton();
	GAMEDATAMGR()->GetQuest()->ClearQuest();
	pUIManager->RearrangeOrder( UI_QUESTBOOK_COMPLETE, FALSE );
}

void CUIQuestComplete::initialize()
{
	// design
	m_pDesign = new CUIQuestDesign;
	CUIManager::getSingleton()->LoadXML("questDesign.xml", (CUIBase*)m_pDesign);

	m_pbtnClose		= (CUIButton*)findUI("btn_close");

	if (m_pbtnClose != NULL)
	{
		CmdQuestCompleteClose* pCmd = new CmdQuestCompleteClose;
		pCmd->setData(this);
		m_pbtnClose->SetCommandUp(pCmd);
	}

	m_pmoveArea		= (CUIText*)findUI("move_title");
	m_ptbName		= (CUITextBox*)findUI("tb_name");
	m_plistContent	= (CUIList*)findUI("list_info");
	m_pbtnOk		= (CUIButton*)findUI("btn_ok");

	if (m_pbtnOk != NULL)
	{
		CmdQuestCompleteOk* pCmd = new CmdQuestCompleteOk;
		pCmd->setData(this);
		m_pbtnOk->SetCommandUp(pCmd);
	}

	m_pbtnPrev		= (CUIButton*)findUI("btn_giveup");

	if (m_pbtnPrev != NULL)
	{
		CmdQuestCompleteReturnNpcMenu* pCmd = new CmdQuestCompleteReturnNpcMenu;
		pCmd->setData(this);
		m_pbtnPrev->SetCommandUp(pCmd);
	}

	m_pbtnCancle	= (CUIButton*)findUI("btn_cancle");

	if (m_pbtnCancle != NULL)
	{
		CmdQuestCompleteClose* pCmd = new CmdQuestCompleteClose;
		pCmd->setData(this);
		m_pbtnCancle->SetCommandUp(pCmd);
	}

	CTString strImageName[eICON_MAX] = { "iarr_type1", "iarr_type2", "iarr_scale"};

	for (int i = 0; i < eICON_MAX; i++)
	{
		m_pImageArr[i] = (CUIImageArray*)findUI(strImageName[i]);
	}

	m_plistScroll = (CUIScrollBar*)findUI("list_scroll");
}

void CUIQuestComplete::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2);
}

void CUIQuestComplete::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

WMSG_RESULT CUIQuestComplete::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

WMSG_RESULT CUIQuestComplete::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pmoveArea && m_pmoveArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_QUESTBOOK_COMPLETE, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CUIQuestComplete::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

void CUIQuestComplete::ReturnToNPCMenu()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int nNpcIndex = -1;
	int nTargetUIType = -1;

	nNpcIndex = GetPrevNpcIdx();
	nTargetUIType = GetPrevNpcUIType();

	close();

	m_nTargetIndex = nNpcIndex;
	pUIManager->GetQuestAccept()->RequestQuest( nNpcIndex, m_nTargetVirIndex, nTargetUIType, m_fNpcX, m_fNpcZ );
}
