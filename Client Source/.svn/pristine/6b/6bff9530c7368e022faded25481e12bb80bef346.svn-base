#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Common/Packet/ptype_reserved_gm_command.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include "Calendar.h"
#include "UICalendarEventInfo.h"

class CmdCalendarEventInfoClose : public Command
{
public:
	CmdCalendarEventInfoClose() : m_pWnd(NULL)	{}
	void setData(CUICalendarEventInfo* pWnd) { m_pWnd = pWnd; }
	void execute()
	{
		if(m_pWnd)
			m_pWnd->CloseUI();
	}
private:
	CUICalendarEventInfo* m_pWnd;
};

class CmdEventListMouseEvent	: public Command
{
public:
	CmdEventListMouseEvent()	: m_pWnd(NULL), m_pItem(NULL), m_Color(DEF_UI_COLOR_WHITE)	{}
	void setData(CUICalendarEventInfo* pWnd, CUIListItem* pList, COLOR col)	
	{ m_pWnd = pWnd;	m_pItem = pList;	m_Color = col;	}
	void execute()
	{
		if(m_pWnd)
			m_pWnd->MouseEvent(m_pItem, m_Color);
	}
private:
	CUICalendarEventInfo*	m_pWnd;
	CUIListItem*			m_pItem;
	COLOR					m_Color;
};


CUICalendarEventInfo::CUICalendarEventInfo()
	:	m_pCalendar(NULL)
	,	m_pEventNameList(NULL)
	,	m_pTextBox(NULL)
	,	m_pEventName(NULL)
	,	m_pEventStartTime(NULL)
	,	m_pEventEndTime(NULL)
	,	m_pEventInfoTitle(NULL)
	,	m_bDrag(false)
	,	m_nOriX(0), m_nOriY(0)
	,	m_nSelectDay(-1)
{
	setInherit(false);		// 창 안에 들어온 MsgProc 창에서 소화. 다른데 상속 안함
}

CUICalendarEventInfo::~CUICalendarEventInfo()
{
	Destroy();
	//m_pEventNameList->DeleteAllListItem();
}
void CUICalendarEventInfo::Reset()
{
	SetEventInfo(-1);
	m_nSelectDay = -1;
	m_bSelected = false;
	m_pEventNameList->DeleteAllListItem();
}

void CUICalendarEventInfo::initialize()
{
	// 버튼 연결.
	CUIButton* pCloseBtn = (CUIButton*)findUI("btn_close");
	if( pCloseBtn )
	{
		CmdCalendarEventInfoClose* pCmd = new CmdCalendarEventInfoClose;
		pCmd->setData(this);
		pCloseBtn->SetCommandUp( pCmd );
	}

	m_pCalendar			= GameDataManager::getSingleton()->GetCalendarData();
	m_pEventNameList	= (CUIList*)findUI("list_event_name");
	m_pTextBox			= (CUITextBox*)findUI("tb_event_desc");
	m_pEventName		= (CUIText*)findUI("text_evt_name");
	m_pEventStartTime	= (CUIText*)findUI("text_evt_start_time");
	m_pEventEndTime		= (CUIText*)findUI("text_evt_end_time");
	m_pEventInfoTitle	= (CUIText*)findUI("text_title");
}

void CUICalendarEventInfo::AddEventList(int day)
{
	Reset();	// 인포창에 정보 셋팅 하기 전에 초기화.
	m_nSelectDay = day;
	if( day < 0 )
		return;

	SetEventInfo(0);
	SetEventList(day);
}

void CUICalendarEventInfo::OpenUI(int day, int sx, int sy)
{
	AddEventList(day);
	if(IsVisible() == TRUE || GetHide() == FALSE )
		return;

	SetVisible(TRUE);
	Hide(FALSE);
	SetPos(sx, sy);
	CUIManager::getSingleton()->RearrangeOrder(UI_CALENDAR_EVENT_INFO, TRUE);
}

void CUICalendarEventInfo::CloseUI()
{
	Hide(TRUE);
	SetVisible(FALSE);
	CUIManager::getSingleton()->RearrangeOrder(UI_CALENDAR_EVENT_INFO, FALSE);
}

void CUICalendarEventInfo::SetEventList( int day )
{
	CUIText* pText;
	CUIListItem* pListItem;
	CUIListItem* pTmpItem;
	
	pListItem = m_pEventNameList->GetListItemTemplate();

	int	nListItemCnt = m_pEventNameList->getListItemCount();

	int i, evtMax = m_pCalendar->GetEventInfoCount(day);

	for( i = 0; i < evtMax; i++ )
	{
		if( i >= nListItemCnt )
			m_pEventNameList->AddListItem(pListItem->Clone());

		pTmpItem = (CUIListItem*)m_pEventNameList->GetListItem(i);
		stEvent* event = m_pCalendar->GetEventInfoAt(day, i);

		if( pTmpItem == NULL || event == NULL )
			continue;

		CmdEventListMouseEvent*	pCmdOver = new CmdEventListMouseEvent;
		pCmdOver->setData(this, pTmpItem, 0xFBFB96FF);
		pTmpItem->SetCommandEnter(pCmdOver);

		CmdEventListMouseEvent*	pCmdLeave = new CmdEventListMouseEvent;
		pCmdLeave->setData(this, pTmpItem, DEF_UI_COLOR_WHITE);
		pTmpItem->SetCommandLeave(pCmdLeave);

		pText = (CUIText*)pTmpItem->findUI("text_evt_name");
		if(pText)
		{
			if( event->bEvtStarted == true )
				pText->SetText(CTString(event->pData->a_start_title));
			else
				pText->SetText(CTString(event->pData->a_end_title));

			if( i == 0 )
				pText->setFontColor(0xFFFF00FF);
			else
				pText->setFontColor(DEF_UI_COLOR_WHITE);
		}
	}
	m_pEventNameList->UpdateList();
	m_pEventNameList->UpdateScroll(evtMax);
	m_pEventNameList->setCurSel(0);
}

void CUICalendarEventInfo::SetEventInfo(int idx)
{
	if( m_pEventStartTime == NULL || m_pEventEndTime == NULL || m_pTextBox == NULL || m_pEventName == NULL )
		return;

	if( m_nSelectDay < 0 || idx < 0 )
	{
		CTString strTime;
		strTime.PrintF("%02d/%02d  %02d:%02d", 0,0, 0,0);
		m_pEventStartTime->SetText(strTime);

		strTime.PrintF("%02d/%02d  %02d:%02d", 0,0, 0,0);
		m_pEventEndTime->SetText(strTime);

		m_pEventName->Hide(TRUE);
		m_pTextBox->SetText(CTString(""));
		return;
	}

	stEvent* event = m_pCalendar->GetEventInfoAt(m_nSelectDay, idx);
	if( event == NULL )
		return;

	time_t	tmt;
	struct tm* pTime;
	tmt = time(NULL);
	tmt = event->pData->a_startTime;

	pTime = localtime(&tmt);

	CTString strTime;
	strTime.PrintF("%02d/%02d  %02d:%02d", pTime->tm_mon + 1, pTime->tm_mday, pTime->tm_hour, pTime->tm_min);
	m_pEventStartTime->SetText(strTime);

	tmt = event->pData->a_endTime;
	pTime = localtime(&tmt);

	strTime.PrintF("%02d/%02d  %02d:%02d", pTime->tm_mon + 1, pTime->tm_mday, pTime->tm_hour, pTime->tm_min);
	m_pEventEndTime->SetText(strTime);

	m_pTextBox->SetText(CTString(event->pData->a_desc));
	if( event->bEvtStarted == true )
		m_pEventName->SetText(CTString(event->pData->a_start_title));
	else
		m_pEventName->SetText(CTString(event->pData->a_end_title));

	m_pEventName->Hide(FALSE);

	int i, nMax = m_pEventNameList->getListItemCount();
	CUIListItem* pItem;
	CUIText*	 pText;

	for (i = 0; i < nMax; ++i)
	{
		pItem = (CUIListItem*)m_pEventNameList->GetListItem(i);

		if (pItem == NULL)
			continue;

		pText = (CUIText*)pItem->findUI("text_evt_name");
		if( pText == NULL )
			continue;
		
		if( i == m_pEventNameList->getCurSel() )
			pText->setFontColor(0xFFFF00FF);
		else
			pText->setFontColor(DEF_UI_COLOR_WHITE);
	}
}


WMSG_RESULT CUICalendarEventInfo::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pEventInfoTitle && m_pEventInfoTitle->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}
	CUIManager::getSingleton()->RearrangeOrder(UI_CALENDAR_EVENT_INFO, TRUE);

	return CUIBase::OnLButtonDown(x, y);
}

WMSG_RESULT CUICalendarEventInfo::OnLButtonUp( UINT16 x, UINT16 y )
{
	m_bDrag = false;
	m_bSelected = false;

	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pEventNameList->IsInside(x, y) == TRUE )
	{
		int i, nMax = m_pEventNameList->getListItemCount();
		CUIListItem* pItem;

		for (i = 0; i < nMax; ++i)
		{
			pItem = (CUIListItem*)m_pEventNameList->GetListItem(i);

			if (pItem != NULL)
			{
				if (pItem->IsInside(x, y) == TRUE)
				{
					m_bSelected = true;
					SetEventInfo(i);
					break;
				}
			}
		}
	}

	return CUIBase::OnLButtonUp(x, y);
}

WMSG_RESULT CUICalendarEventInfo::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

	return CUIBase::OnMouseMove(x, y, pMsg);
}

void CUICalendarEventInfo::MouseEvent( CUIListItem* pListItem, COLOR col )
{
	if( pListItem == NULL || m_pEventNameList == NULL )
		return;

	CUIText* pText;

	pText = (CUIText*)pListItem->findUI("text_evt_name");
	if( pText )
	{
		if( pListItem->getControlIndex() != m_pEventNameList->getCurSel() )
			pText->setFontColor(col);
	}
}