#ifndef UI_CALENDAR_H_
#define UI_CALENDAR_H_

#define DAY_MAX	(42)

struct reservedGMCommandElement;
class CUIArray;
class Calendar;
class CUIText;


class CUICalendar : public CUIWindow
{
public:
	CUICalendar();
	~CUICalendar();

	enum	eTEX_STATE
	{
		eTEX_STATE_CALENDAR1 = 0,
		eTEX_STATE_CALENDAR2,
		eTEX_STATE_MAX
	};

	void	initialize();

	void	CloseCalendar();
	void	OpenCalendar();

	void	SetTime();
	void	SetCalendar();
	void	SetBorder(int nYear, int nMonth);

	int		GetWeek()		{ return m_nCurrentWeek;	}
	int		GetYear()		{ return m_nCurrentYear;	}
	int		GetMonth()		{ return m_nCurrentMonth;	}

	bool	CheckLeapyear( int year );
	
	int		getNumDay(int year, int month);

	void	NextMonth();
	void	PrevMonth();

	BOOL	CloseWindowByEsc()	{ CloseCalendar(); return TRUE; }

public:
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	// Msg Proc
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT	OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);

private:
	void	Clear();
	
	std::multimap<int,reservedGMCommandElement*>	m_mapEvent;
	Calendar*		m_pCalendar;

	CUIArray*		m_pDayArray;
	CUIText*		m_pBorderMonth;
	CUIText*		m_pBorderYear;
	CUIText*		m_pDrag;
	UIRect			m_rcOriginPos;
	int				m_nEventCnt[DAY_MAX];
	int				m_nCurrentMonth;
	int				m_nCurrentYear;
	int				m_nCurrentWeek;	// 현재 달의 시작 요일. 0 == 일요일
	int				m_nTotalDate;
	bool			m_bDrag;
	int				m_nOriX, m_nOriY;

};
#endif // UI_CALENDAR_H_