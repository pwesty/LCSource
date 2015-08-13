#include "stdh.h"
#include "UIQuestRestore.h"
#include <Engine/GameDataManager/GameDataManager.h>

//--------------------------------------------------------------
// 복구창 닫기
class CmdQuestRestoreClose : public Command
{
public:
	CmdQuestRestoreClose() : pUI_(NULL) {}
	void setData(CUIQuestRestore* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->close();
	}
private:
	CUIQuestRestore* pUI_;
};


CUIQuestRestore::CUIQuestRestore()
	: m_pDesign(NULL)
	, m_pmoveArea(NULL)
	, m_pbtnClose(NULL)
	, m_pbtnOk(NULL)
	, m_plistContent(NULL)
	, m_plistScroll(NULL)
	, m_pContent(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
{
	setInherit(false);
}
CUIQuestRestore::~CUIQuestRestore()
{
	SAFE_DELETE(m_pDesign);
	Destroy();
}

void CUIQuestRestore::UpdateContent()
{
	CTString strTemp;
	int nCount = GAMEDATAMGR()->GetQuest()->GetRestoreCount();
	int nNum = 1;
	COLOR color = 0xFFFFFFFF;
	// 안내
	if (m_pContent != NULL)
	{
		color = m_pContent->GetTextColor();
		strTemp.PrintF( _S( 4815 , "%d 개의 포기된 퀘스트가 모두 복구 되었습니다. NPC를 통해 퀘스트를 다시 받으시기 바랍니다."), nCount);
		m_pContent->SetText(strTemp, color);
	}

	// 리스트 내용
	if (m_plistContent == NULL)
		return;

	m_plistContent->DeleteAllListItem();

	if (m_plistScroll != NULL)
		m_plistScroll->SetScrollPos(0);

	std::vector<Quest::stRestoreInfo>::const_iterator iter = GAMEDATAMGR()->GetQuest()->GetRestoreQuestList().begin();
	std::vector<Quest::stRestoreInfo>::const_iterator iter_end = GAMEDATAMGR()->GetQuest()->GetRestoreQuestList().end();

	for (; iter != iter_end; ++iter)
	{
		CQuestStaticData* pQuestDD = CQuestStaticData::getData((*iter).iQuestIndex);
		if (pQuestDD == NULL)
			continue;
		// 퀘스트 이름
		CUITextBox* pTextBox = (CUITextBox*)m_pDesign->CloneRestoreQuest();

		if (pTextBox != NULL)
		{
			color = pTextBox->GetTextColor();
			strTemp.PrintF("%d. %s", nNum, pQuestDD->GetTitle());
			pTextBox->SetText(strTemp, color, TRUE);
			m_plistContent->AddListItem(pTextBox);
		}		

		// NPC 이름
		CUIText* pText = (CUIText*)m_pDesign->CloneRestoreNpc();

		if (pText != NULL)
		{
			// Name
			if (pQuestDD->startType == QSTART_NPC)
			{
				CMobData* pNpc = CMobData::getData(pQuestDD->startData);

				strTemp.PrintF("<%s>", pNpc->GetName());
			}
			else
			{
				CTString strTemp2;
				strTemp2.PrintF(_S(457, "시스템"));
				strTemp.PrintF("<%s>", strTemp2);				
			}

			pText->SetText(strTemp);
			m_plistContent->AddListItem(pText);
		}

		nNum++;
	}

	int nMaxItem = m_plistContent->getListItemCount();
	m_plistContent->UpdateScroll(nMaxItem);
	m_plistContent->UpdateList();

	int nShowCnt = m_plistContent->GetItemShowNum();

	if (m_plistScroll != NULL)
		m_plistScroll->SetItemsPerPage(nShowCnt);
}

void CUIQuestRestore::initialize()
{
	CUIManager* pUIMgr = CUIManager::getSingleton();
	// design
	m_pDesign = new CUIQuestDesign;
	pUIMgr->LoadXML("questDesign.xml", (CUIBase*)m_pDesign);

	m_pbtnClose		= (CUIButton*)findUI("btn_close");

	if (m_pbtnClose != NULL)
	{
		CmdQuestRestoreClose* pCmd = new CmdQuestRestoreClose;
		pCmd->setData(this);
		m_pbtnClose->SetCommandUp(pCmd);
	}

	m_pmoveArea		= (CUIText*)findUI("move_title");
	m_plistContent	= (CUIList*)findUI("list_info");
	m_plistScroll	= (CUIScrollBar*)findUI("list_scroll");
	m_pbtnOk		= (CUIButton*)findUI("btn_ok");
	m_pContent		= (CUITextBox*)findUI("tb_content");

	if (m_pbtnOk != NULL)
	{
		CmdQuestRestoreClose* pCmd = new CmdQuestRestoreClose;
		pCmd->setData(this);
		m_pbtnOk->SetCommandUp(pCmd);
	}
}

void CUIQuestRestore::open()
{
	SetVisible(TRUE);
	Hide(FALSE);
	UpdateContent();
	CUIManager::getSingleton()->RearrangeOrder( UI_QUEST_RESTORE, TRUE );
}

void CUIQuestRestore::close()
{
	SetVisible(FALSE);
	Hide(TRUE);
	CUIManager::getSingleton()->RearrangeOrder( UI_QUEST_RESTORE, FALSE );
}

void CUIQuestRestore::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2 , ( pixMaxJ + pixMinJ - GetHeight() ) / 2  );
}

void CUIQuestRestore::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

WMSG_RESULT CUIQuestRestore::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

WMSG_RESULT CUIQuestRestore::OnLButtonDown( UINT16 x, UINT16 y )
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

	CUIManager::getSingleton()->RearrangeOrder( UI_QUEST_RESTORE, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CUIQuestRestore::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

