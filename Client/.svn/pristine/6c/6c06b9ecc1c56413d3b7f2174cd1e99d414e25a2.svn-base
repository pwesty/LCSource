#include "StdH.h"

#include <Engine/Interface/UIInternalClasses.h>
#include "NewsUI.h"

#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include "News.h"

class CmdNewsClose : public Command
{
public:
	CmdNewsClose() : m_pWnd(NULL) {}
	void setData(CNewsUI* pWnd)	{ m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->CloseUI();
	}

private:
	CNewsUI* m_pWnd;
};

class CmdNewsPageChange : public Command
{
public:
	CmdNewsPageChange() : m_pWnd(NULL), m_bNext(true) {}
	void setData(CNewsUI* pWnd, bool bNext)	{ m_pWnd = pWnd; m_bNext = bNext;	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->ChangePage(m_bNext);
		}
	}

private:
	CNewsUI* m_pWnd;
	bool	 m_bNext;
};

CNewsUI::CNewsUI()
	: m_pListNews(NULL)
	, m_pTextPage(NULL)
	, m_pDragArea(NULL)
	, m_bDrag(false)
	, m_nOriX(0)
	, m_nOriY(0)
{
	setInherit(false);
}

CNewsUI::~CNewsUI()
{

}

void CNewsUI::initialize()
{
	CUIButton* pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
	{
		CmdNewsClose* pCmd = new CmdNewsClose;
		pCmd->setData(this);
		pBtn->SetCommandUp(pCmd);
	}

	if (pBtn = (CUIButton*)findUI("btn_prev"))
	{
		CmdNewsPageChange* pCmd = new CmdNewsPageChange;
		pCmd->setData(this, false);
		pBtn->SetCommandUp(pCmd);
	}

	if (pBtn = (CUIButton*)findUI("btn_next"))
	{
		CmdNewsPageChange* pCmd = new CmdNewsPageChange;
		pCmd->setData(this, true);
		pBtn->SetCommandUp(pCmd);
	}

	m_pTextPage = (CUIText*)findUI("text_page");
	m_pListNews = (CUIList*)findUI("list_news");
	m_pDragArea = findUI("drag_area");
}

void CNewsUI::OpenUI()
{
	if (IsVisible() == TRUE || GetHide() == FALSE)
		return;

	GAMEDATAMGR()->GetNews()->ListReq();
}

void CNewsUI::CloseUI()
{
	SetVisible(FALSE);
	Hide(TRUE);
	UIMGR()->RearrangeOrder(UI_NEWS, FALSE);

	CNews* pNews = 	GAMEDATAMGR()->GetNews();

	if (pNews == NULL)
		return;

	pNews->ResetPage();
}

void CNewsUI::ChangePage( bool bNext )
{
	CNews* pNews = 	GAMEDATAMGR()->GetNews();

	if (pNews == NULL)
		return;

	int nCurPage = pNews->GetCurPage();

	if (pNews->GetNextPage() == true && bNext == true)
	{
		pNews->ListReq(nCurPage + 1);
	}
	else if (bNext == false && nCurPage > 0)
	{
		pNews->ListReq(nCurPage - 1);
	}
}

void CNewsUI::updatePage()
{
	if (m_pTextPage == NULL)
		return;

	CNews* pNews = 	GAMEDATAMGR()->GetNews();

	if (pNews == NULL)
		return;

	CTString strPage;
	strPage.PrintF("%d", pNews->GetCurPage() + 1);
	m_pTextPage->SetText(strPage);
}

void CNewsUI::SetNews()
{
	CNews* pNewsData = GAMEDATAMGR()->GetNews();
	ResponseClient::NoticeList* pNotiData = pNewsData->GetNotiList();

	if (m_pListNews == NULL ||  pNotiData == NULL)
		return;

	int i = 0;
	int nDataCount = pNotiData->count;
	int nListMax = m_pListNews->getListItemCount();
	CUIBase* pItem = NULL;
	CUIText* pText = NULL;
	CTString strTemp;
	tm* pTime;

// 	if (nDataCount > 0)
// 		m_nMaxPage = (nDataCount / 10);

	for (; i < nDataCount; ++i)
	{
		if (i >= nListMax)
		{
			m_pListNews->AddListItem(m_pListNews->GetListItemTemplate()->Clone());
		}

		pItem = m_pListNews->GetListItem(i);

		if (pItem == NULL)
			continue;

		pItem->Hide(FALSE);

		if (pText = (CUIText*)pItem->findUI("text_time"))
		{
			pTime = localtime((time_t*)&(pNotiData->data[i].date));
			strTemp.PrintF("%d/%02d/%02d %02d:%02d", pTime->tm_year + 1900, pTime->tm_mon + 1,
				pTime->tm_mday, pTime->tm_hour ,pTime->tm_min);

			pText->SetText(strTemp);
		}

		if (pText = (CUIText*)pItem->findUI("text_content"))
		{
			pText->SetText((CTString(pNotiData->data[i].noticeString)));
		}
	}

	for (i = nDataCount; i < nListMax; ++i)
	{
		pItem = m_pListNews->GetListItem(i);

		if (pItem == NULL)
			continue;

		pItem->Hide(TRUE);
	}

	updatePage();
	
	SetVisible(TRUE);
	Hide(FALSE);

	UIMGR()->RearrangeOrder(UI_NEWS, TRUE);
	UIMGR()->GetNotice()->SetIconBlink(Notice::NOTICE_NEWS_NORMAL, false);
}

WMSG_RESULT CNewsUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

	return WMSG_FAIL;
}

WMSG_RESULT CNewsUI::OnLButtonDown( UINT16 x, UINT16 y )
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

	CUIManager::getSingleton()->RearrangeOrder(UI_NEWS, TRUE);
	return WMSG_FAIL;
}

WMSG_RESULT CNewsUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	return WMSG_FAIL;
}
