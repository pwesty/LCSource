
#include "StdH.h"
#include "News.h"

#include <Engine/Network/CNetwork.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/function/NewsUI.h>
#include <Engine/Contents/function/News_Web_UI.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Notice.h>
#include <Engine/Entities/InternalClasses.h>

CNews::CNews()
	: m_pNoticeList(NULL)
	, m_nSaveRevision(0)
	, m_nCurRevision(0)
	, m_nCurPage(0)
	, m_bNextPage(false)
{
	m_vecUrl.clear();
}

CNews::~CNews()
{
	ClearData();
}

void CNews::RecvMsg( CNetworkMessage* istr )
{
	RecvDataList(istr);
}

void CNews::UpdateRevision( CNetworkMessage* istr )
{
	UpdateClient::checkNotice* pPack = reinterpret_cast<UpdateClient::checkNotice*>(istr->GetBuffer());

	m_nCurRevision = pPack->revNum;

	Notice* pNotice = GAMEDATAMGR()->GetNotice();

	if (pNotice != NULL)
		pNotice->AddToNoticeList(NEWS_NORMAL, Notice::NOTICE_NEWS_NORMAL);
}

void CNews::UpdateWebNoti( CNetworkMessage* istr )
{
	UpdateClient::checkPopupNotice* pPack = reinterpret_cast<UpdateClient::checkPopupNotice*>(istr->GetBuffer());

	m_vecUrl.push_back(pPack->html_num);

	Notice* pNotice = GAMEDATAMGR()->GetNotice();

	if (pNotice != NULL)
		pNotice->AddToNoticeList(NEWS_WEB, Notice::NOTICE_NEWS_WEB);
}

void CNews::UpdateWebClose( CNetworkMessage* istr )
{
	UpdateClient::closePopupNotice* pPack = reinterpret_cast<UpdateClient::closePopupNotice*>(istr->GetBuffer());

	vecUrliter bIt = m_vecUrl.begin();
	vecUrliter eIt = m_vecUrl.end();

	for (; bIt != eIt; ++bIt)
	{
		if ((*bIt) == pPack->html_num)
		{
			m_vecUrl.erase(bIt);
			break;
		}
	}

	if (m_vecUrl.size() <= 0)
	{
		Notice* pNotice = GAMEDATAMGR()->GetNotice();
	
		if (pNotice != NULL)
			pNotice->DelFromNoticeList(NEWS_WEB, Notice::NOTICE_NEWS_WEB);
	}
}

void CNews::UpdateWebClear( CNetworkMessage* istr )
{
	m_vecUrl.clear();

	Notice* pNotice = GAMEDATAMGR()->GetNotice();

	if (pNotice != NULL)
		pNotice->DelFromNoticeList(NEWS_WEB, Notice::NOTICE_NEWS_WEB);
}

void CNews::RecvDataList( CNetworkMessage* istr )
{
	ResponseClient::NoticeList* pPack = reinterpret_cast<ResponseClient::NoticeList*>(istr->GetBuffer());
	m_pNoticeList = pPack;
	m_bNextPage = pPack->isNextPage;

	UIMGR()->GetNewsUI()->SetNews();
	m_nSaveRevision = m_nCurRevision;
}

void CNews::ListReq(int nPage /*=0*/)
{
	CNetworkMessage nmList;
	RequestClient::NoticeList* packet = reinterpret_cast<RequestClient::NoticeList*>(nmList.nm_pubMessage);

	m_nCurPage = nPage;
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_NOTICE;
	packet->thirdType = MSG_EX_NOTICE_LIST;
	packet->pageNum = m_nCurPage;

	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
}

int CNews::GetWebUrl( int nIdx )
{
	int size = m_vecUrl.size();

	if (nIdx < 0 || nIdx >= size)
		return -1;

	return m_vecUrl[nIdx];
}

void CNews::ClearData()
{
	m_vecUrl.clear();
	m_bNextPage = false;
	m_nCurPage = 0;
	m_pNoticeList = NULL;
}
