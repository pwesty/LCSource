#include "StdH.h"
#include "Notice.h"

#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/function/News.h>

Notice::Notice(void)
{
	int i;
	
	for (i = 0; i < NOTICE_MAX; i++)
	{
		m_vecList[i].clear();
	}
}

Notice::~Notice(void)
{
	clear();
}

void Notice::AddToNoticeList( int iNoticeIndex, Notice::eNoticeType iNoticeType )
{
	if (iNoticeType < 0 || iNoticeType >= NOTICE_MAX)
		return;

	CUIManager* pUIMgr = CUIManager::getSingleton();
	bool bNewNotice = true;

	if (iNoticeIndex == 2030)
	{
		pUIMgr->m_JapanTempEvent = 1;
	}

	if (iNoticeType == NOTICE_QUEST)
	{
		CQuestDynamicData QuestDD(CQuestSystem::Instance().GetStaticData(iNoticeIndex));

		switch (QuestDD.GetQuestType2())
		{
		case QTYPE_REPEAT_ONCE:
		case QTYPE_REPEAT_DAY:
			iNoticeType = NOTICE_QUEST;
			break;
		case QTYPE_REPEAT_UNLIMITED:
			iNoticeType = NOTICE_QUEST_UNLIMITED;
			break;
		}
	}

	if (iNoticeType == NOTICE_NEWS_NORMAL)
	{
		bNewNotice = GAMEDATAMGR()->GetNews()->CheckRevision();
	}

	sNoticeInfo TempInfo;
	TempInfo.iIndex = iNoticeIndex;
	TempInfo.iType	= iNoticeType;

	std::vector<sNoticeInfo>* pTempList = NULL;
	pTempList = &m_vecList[iNoticeType];

	std::vector<sNoticeInfo>::iterator iter = 
		std::find_if(pTempList->begin(), pTempList->end(), FindNotice(TempInfo) );

	if( iter == pTempList->end() )
	{
		pTempList->push_back(TempInfo);
		m_bNewNotice[iNoticeType] = bNewNotice;
	}
	else
	{
		if (iNoticeType == NOTICE_NEWS_NORMAL || iNoticeType == NOTICE_NEWS_WEB)
			pUIMgr->GetNotice()->SetIconBlink(eNoticeType(iNoticeType), bNewNotice);
		return;
	}

	pUIMgr->GetNotice()->UpdateBtn();	
}

void Notice::DelFromNoticeList( int iNoticeIndex, Notice::eNoticeType iNoticeType )
{
	if( iNoticeIndex < 0 )
		return;

	if (iNoticeType < 0 || iNoticeType >= NOTICE_MAX)
		return;

	CUIManager* pUIMgr = CUIManager::getSingleton();

	if (iNoticeType == NOTICE_QUEST)
	{
		CQuestDynamicData *pQuestDD = NULL;
		CQuestDynamicData QuestDD(CQuestSystem::Instance().GetStaticData( iNoticeIndex ));
		pQuestDD = &QuestDD;

		switch (pQuestDD->GetQuestType2())
		{
		case QTYPE_REPEAT_ONCE:
		case QTYPE_REPEAT_DAY:
			iNoticeType = NOTICE_QUEST;
			break;
		case QTYPE_REPEAT_UNLIMITED:
			iNoticeType = NOTICE_QUEST_UNLIMITED;
			break;
		}
	}

	sNoticeInfo TempInfo;		
	TempInfo.iIndex = iNoticeIndex;
	TempInfo.iType	= iNoticeType;

	std::vector<sNoticeInfo>* pTempList = NULL;
	pTempList = &m_vecList[iNoticeType];

	std::vector<sNoticeInfo>::iterator iter = 
		std::find_if(pTempList->begin(), pTempList->end(), FindNotice(TempInfo) );

	if( iter != pTempList->end() )
	{
		pTempList->erase( iter );
		m_bNewNotice[iNoticeType] = false;
	}
	else
	{
		return;
	}	

	pUIMgr->GetNotice()->UpdateBtn();
}

std::vector<Notice::sNoticeInfo>* Notice::GetNoticeList( eNoticeType iNoticeType )
{
	if (iNoticeType < 0 || iNoticeType >= NOTICE_MAX)
		return NULL;

	return &m_vecList[iNoticeType];
}

bool Notice::GetNoticeBlink( eNoticeType iNoticeType )
{
	if (iNoticeType < 0 || iNoticeType >= NOTICE_MAX)
		return false;

	return m_bNewNotice[iNoticeType];
}

void Notice::SetNoticeBlink( eNoticeType iNoticeType, bool bOnOff )
{
	if (iNoticeType < 0 || iNoticeType >= NOTICE_MAX)
		return;

	m_bNewNotice[iNoticeType] = bOnOff;
}

void Notice::clear()
{
	int i;

	for (i = 0; i < NOTICE_MAX; i++)
	{
		m_vecList[i].clear();
		m_bNewNotice[i] = false;
	}
}
