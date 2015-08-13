#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Common/Packet/ptype_reserved_gm_command.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Help/Util_Help.h>
#include "Calendar.h"
#include "UICalendarEventInfo.h"
#include "UICalendar.h"

#define	STANDARD_YEAR	(1900)
#define WEEK			(7)
#define	ONE_MIN			(60)
#define	ONE_HOUR		(ONE_MIN * 60)
#define	ONE_DAY			(ONE_HOUR * 24)

#define MONTH_MAX		(11)	// 0부터 시작

#define DEF_DATE_WIDTH	(90)
#define DEF_DATE_HEIGHT	(64)

class CmdCalendarClose : public Command
{
public:
	CmdCalendarClose() : m_pWnd(NULL) {}
	void setData( CUICalendar* pWnd ) { m_pWnd = pWnd; }
	void execute()
	{
		if( m_pWnd )
			m_pWnd->CloseCalendar();
	}
private:
	CUICalendar* m_pWnd;
};

class CmdMonthArrow : public Command
{
public:
	CmdMonthArrow() : m_pWnd(NULL)	{}
	void setData( CUICalendar* pWnd, bool bNextArrow ) { m_pWnd = pWnd; m_bNext = bNextArrow; }
	void execute()
	{
		if( m_pWnd == NULL )
			return;
		if( m_bNext )
			m_pWnd->NextMonth();
		else
			m_pWnd->PrevMonth();
	}
private:
	CUICalendar* m_pWnd;
	bool		 m_bNext;

};

CUICalendar::CUICalendar()
	:	m_pDayArray(NULL)
	,	m_pBorderMonth(NULL)
	,	m_pBorderYear(NULL)
	,	m_nCurrentYear(0)
	,	m_nCurrentMonth(0)
	,	m_pDrag(NULL)
	,	m_nOriX(0), m_nOriY(0)
	,	m_pCalendar(NULL)
	,	m_nTotalDate(0)
{
	setInherit(false);		// 창 안에 들어온 MsgProc 창에서 소화. 다른데 상속 안함
	int i;
	for( i = 0; i < DAY_MAX; i++ )
	{
		m_nEventCnt[i] = 0;
	}
}

CUICalendar::~CUICalendar()
{
	Destroy();
}

void CUICalendar::initialize()
{
	m_pCalendar = GameDataManager::getSingleton()->GetCalendarData();
 	m_pDayArray = (CUIArray*)findUI("day_array");

	// 버튼 연결.
	CUIButton* pCloseBtn = (CUIButton*)findUI("btn_close");
	if( pCloseBtn )
	{
		CmdCalendarClose* pCmd = new CmdCalendarClose;
		pCmd->setData(this);
		pCloseBtn->SetCommandUp( pCmd );
	}

	CUIButton* pNextBtn = (CUIButton*)findUI("btn_next_arrow");
	CUIButton* pPrevBtn = (CUIButton*)findUI("btn_prev_arrow");
	if( pNextBtn )
	{
		CmdMonthArrow* pCmd = new CmdMonthArrow;
		pCmd->setData(this, true);
		pNextBtn->SetCommandUp( pCmd );
	}
	if( pPrevBtn )
	{
		CmdMonthArrow* pCmd = new CmdMonthArrow;
		pCmd->setData(this, false);
		pPrevBtn->SetCommandUp( pCmd );
	}

	// 년, 달 보더 연결.
	m_pBorderYear = (CUIText*)findUI("text_border_year");
	m_pBorderMonth = (CUIText*)findUI("text_border_month");
	
	m_pDrag = (CUIText*)findUI("text_drag");
}

void CUICalendar::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
	m_rcOriginPos.SetRect(m_nPosX, m_nPosY, m_nWidth, m_nHeight);
}
void CUICalendar::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

bool CUICalendar::CheckLeapyear( int year )
{
	if( !((year % 4) && (year % 100)) || !(year % 400) )
		return true;

	return false;
}

void CUICalendar::SetTime()
{
	Clear();
	struct tm	calc_tm;
	time_t hTime;
	struct tm*	pTime;
	time(&hTime);
	pTime = localtime(&hTime);

	pTime->tm_mday;

	memset(&calc_tm, 0, sizeof(calc_tm));

	calc_tm.tm_year = m_pCalendar->GetYear() - STANDARD_YEAR;
	calc_tm.tm_mon = m_pCalendar->GetMonth() - 1;
	calc_tm.tm_mday = 1;
	calc_tm.tm_isdst = -1;
	time_t time = mktime( &calc_tm );

	m_nCurrentMonth  = calc_tm.tm_mon;
	m_nCurrentYear   = STANDARD_YEAR + calc_tm.tm_year;
	m_nCurrentWeek	 = calc_tm.tm_wday;
	SetCalendar();
}


void CUICalendar::SetCalendar()
{
	if( CUIManager::getSingleton()->isCreateVisible(UI_CALENDAR_EVENT_INFO) )
		CUIManager::getSingleton()->GetCalendarEventInfo()->Reset();	// 달력에 변화가 생기면 인포창 초기화.

	if( m_pDayArray == NULL )
		return;

	CUIArrayItem* pArrayItem;
	CUIArrayItem* pTmpItem;
	CUIText*	  pText;
	CUIImage*	  pImg;

	time_t tmt;
	struct tm *pToday;
	tmt = time(NULL);

	tmt    = m_pCalendar->GetNowTime();
	pToday = localtime(&tmt);

	SetBorder(m_pCalendar->GetYear(), m_pCalendar->GetMonth() - 1);		// 보더의 month는 0 == 1월

	int i, max = m_pDayArray->GetArrayChildCount();
	int dayNum = 1, nDays = getNumDay(m_pCalendar->GetYear(), m_pCalendar->GetMonth() - 1);	// day도 마찬가지.
	m_nTotalDate = nDays;
	pArrayItem	= m_pDayArray->GetArrayItemTemplate();
	for( i = 0; i < max; i++ )
	{
		pTmpItem = m_pDayArray->GetArrayItem(i);

		if( pTmpItem == NULL )
			continue;

		// 달력 배열이 42개 이므로 현재 달에 출력 되어야 할 날자가 아니면 보여주지 않는다.
		if( m_nCurrentWeek > i || nDays + m_nCurrentWeek <= i )
		{
			pTmpItem->Hide(TRUE);
			continue;
		}

		pText = (CUIText*)pTmpItem->findUI("text_day_number");
		if( pText != NULL )
		{
			CTString str;
			pTmpItem->Hide(FALSE);
			str.PrintF("%d", dayNum);
			pText->SetText(str);
			if( i % WEEK == 0 )
				pText->setFontColor(0xFF0000FF);
			else
				pText->setFontColor(DEF_UI_COLOR_WHITE);

			pText->setEdge(TRUE);
		}

		
		// 이벤트 정보는 1~31일까지의 데이타가 저장 되어있음.
		int nData = m_pCalendar->GetEventInfoCount(dayNum);	
		if( nData > 0 )
		{
			stEvent* event = m_pCalendar->GetEventInfoAt(dayNum, 0);
			pImg = (CUIImage*)pTmpItem->findUI("img_event");
			if( pImg != NULL && event != NULL )
			{
				pImg->ReleaseTexData();
				if( event->pData->a_image_type == 0 )
					pImg->setTexString("Calendar1.tex");
				else if( event->pData->a_image_type == 1 )
					pImg->setTexString("Calendar2.png");

				UIRectUV uv;

				uv.U0 = event->pData->a_image_x * DEF_DATE_WIDTH;
				uv.V0 = event->pData->a_image_y * DEF_DATE_HEIGHT;
				uv.U1 = uv.U0 + DEF_DATE_WIDTH;
				uv.V1 = uv.V0 + DEF_DATE_HEIGHT;

				pImg->SetUV(uv);
				pImg->Hide(FALSE);
			}

			pImg = (CUIImage*)pTmpItem->findUI("img_plus");
			if( nData >= 2 && pImg != NULL )
				pImg->Hide(FALSE);

			pText = (CUIText*)pTmpItem->findUI("text_evt_name");
			if( pText && event )
			{
				CTString strTitle;
				if( event->bEvtStarted == true )
					strTitle = UtilHelp::getSingleton()->GetCalcStringEllipsis(CTString(event->pData->a_start_title), pText->GetWidth(), CTString("..."));
				else
					strTitle = UtilHelp::getSingleton()->GetCalcStringEllipsis(CTString(event->pData->a_end_title), pText->GetWidth(), CTString("..."));

				pText->SetText(strTitle);
				pText->setEdge(TRUE);
				pText->Hide(FALSE);
			}
		}
		else
		{
			pImg = (CUIImage*)pTmpItem->findUI("img_event");
			if( pImg != NULL )
				pImg->Hide(TRUE);

			pImg = (CUIImage*)pTmpItem->findUI("img_plus");
			if( pImg != NULL )
				pImg->Hide(TRUE);

			pText = (CUIText*)pTmpItem->findUI("text_evt_name");
			if( pText  != NULL )
				pText->Hide(TRUE);
		}
		dayNum++;
	}

	pImg = (CUIImage*)m_pDayArray->findUI("img_today");
	if( pImg != NULL )
	{	// 서버에서 보내준 정보와 로컬 타임과 맞추기 위해서 연도에는 1900+, month는 -1
		if( pToday->tm_year + STANDARD_YEAR == m_pCalendar->GetYear() && pToday->tm_mon == m_pCalendar->GetMonth() - 1 )
		{
			int tmpToday = pToday->tm_mday + m_nCurrentWeek - 1;
			pTmpItem = m_pDayArray->GetArrayItem(tmpToday);
			
			if( pTmpItem )
			{
				pImg->Hide(FALSE);
				pImg->SetPos( pTmpItem->GetPosX() - 5, pTmpItem->GetPosY() -5 );
			}
		}
		else
		{
			pImg->Hide(TRUE);
		}
	}
}

// month 는 0 ~ 11 로 한정
int CUICalendar::getNumDay(int year, int month)
{
	// check 
	int		ret;
	const int defDay[] 	= { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	ret = defDay[month];

	// 2월이면 윤년 계산식에 의해 날자 계산.
	if (month == 1)
	{
		if (CheckLeapyear(year))
			ret++;
	}

	return ret;
}

void CUICalendar::NextMonth( )
{
	if( m_nCurrentMonth < MONTH_MAX )
		m_nCurrentMonth++;
	else
	{
		m_nCurrentYear += 1;
		m_nCurrentMonth = 0;
	}
	// 서버에 보낼때는 실제 달을 보내야 하기때문에 +1을 더해준다.	localtime으로 하면 0부터 ~11, 서버에서는 1~12
	// 서버에서 받을때 실제 달을 받기 때문에 -1을 해서 세팅 해준다.
	GameDataManager::getSingleton()->GetCalendarData()->SendCalendarEventListReq(0,m_nCurrentYear, m_nCurrentMonth + 1);
}

void CUICalendar::PrevMonth( )
{
	if( m_nCurrentMonth > 0 )
		m_nCurrentMonth--;
	else
	{
		m_nCurrentYear -= 1;
		m_nCurrentMonth = MONTH_MAX;
	}
	GameDataManager::getSingleton()->GetCalendarData()->SendCalendarEventListReq(0,m_nCurrentYear, m_nCurrentMonth + 1);
}

void CUICalendar::SetBorder( int nYear, int nMonth )
{
	int   MonthIdx[] = { 5989, 5990, 5991, 5992, 5993, 5994, 5995, 5996, 5997, 5998, 5999, 6000 };
	if( m_pBorderYear )
	{
		CTString strYear;
		strYear.PrintF("%d", nYear);
		m_pBorderYear->SetText(strYear);
	}
	if( m_pBorderMonth )
	{
		CTString strMonth;
		strMonth = CTString(_S(MonthIdx[m_nCurrentMonth], "1~12월"));
		m_pBorderMonth->SetText(strMonth);
	}
}

void CUICalendar::OpenCalendar()
{
	if(IsVisible() == TRUE || GetHide() == FALSE )
		return;

	SetVisible(TRUE);
	Hide(FALSE);
	CUIManager::getSingleton()->RearrangeOrder(UI_CALENDAR, TRUE);
	SetPos( m_rcOriginPos.Left, m_rcOriginPos.Top );
	GameDataManager::getSingleton()->GetCalendarData()->SendCalendarEventListReq(0, m_nCurrentYear, m_nCurrentMonth);
}

void CUICalendar::CloseCalendar()
{
	Clear();
	Hide(TRUE);
	SetVisible(FALSE);
	CUIManager::getSingleton()->RearrangeOrder(UI_CALENDAR, FALSE);
}

void CUICalendar::Clear()
{
	int i;
	for( i = 0; i < DAY_MAX; i++ )
	{
		m_nEventCnt[i] = 0;
	}
	m_nCurrentWeek	= 0;
	m_nCurrentMonth = 0;
	m_nCurrentYear	= 0;
	m_nTotalDate	= 0;
}

WMSG_RESULT CUICalendar::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pDrag && m_pDrag->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder(UI_CALENDAR, TRUE);

	return CUIBase::OnLButtonDown(x, y);
}

WMSG_RESULT CUICalendar::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;
	
	if( m_pDayArray && m_pDayArray->IsInside(x, y) )
	{
		int nIdx = m_pDayArray->GetSelectIdx() - m_nCurrentWeek + 1;
		if( nIdx > 0 && nIdx <= m_nTotalDate )
			CUIManager::getSingleton()->GetCalendarEventInfo()->OpenUI(nIdx, m_nPosX + m_nWidth, m_nPosY);
	}

	return CUIBase::OnLButtonUp(x, y);
}

WMSG_RESULT CUICalendar::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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