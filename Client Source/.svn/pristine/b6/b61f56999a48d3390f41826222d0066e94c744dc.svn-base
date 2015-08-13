#ifndef UI_CALENDAR_EVENT_INFO_H_
#define UI_CALENDAR_EVENT_INFO_H_

struct reservedGMCommandElement;
class CUIListItem;

class CUICalendarEventInfo : public CUIWindow
{
public :
	CUICalendarEventInfo();
	~CUICalendarEventInfo();

	void	initialize();
	void	AddEventList(int day);
	void	SetEventList(int day);
	void	SetEventInfo(int idx);
	void	OpenUI(int day, int sx, int sy);
	void	CloseUI();
	void	Reset();
	void	MouseEvent(CUIListItem* pListItem, COLOR col );

	WMSG_RESULT	OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT	OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);

	BOOL	CloseWindowByEsc()	{ CloseUI(); return TRUE; }

private:
	Calendar*	 m_pCalendar;
	CUIList*	 m_pEventNameList;
	CUIText*	 m_pEventName;
	CUIText*	 m_pEventStartTime;
	CUIText*	 m_pEventEndTime;
	CUIText*	 m_pEventInfoTitle;
	CUITextBox*	 m_pTextBox;
	bool		 m_bDrag;
	bool		 m_bSelected;
	bool		 m_bMouseOver;
	int			 m_nOriX, m_nOriY;
	int			 m_nSelectDay;


};

#endif // UI_CALENDAR_EVENT_INFO_H_