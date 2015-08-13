#ifndef		CALENDAR_SYSTEM_H_
#define		CALENDAR_SYSTEM_H_

struct reservedGMCommandElement;
class  CNetworkMessage;

namespace RequestClient{
	struct reservedGMCommandList;
}

namespace ResponseClient{
	struct reservedGMCommandList;
}

struct stEvent
{
	int			    eType;	// 타잎에 의해 sort하기 위함.
	int				evtIdx;	// 타잎이 같다면 index에 의해 sort
	bool			bEvtStarted;	// 이벤트 시작일인지 아닌지 체크.
	reservedGMCommandElement* pData;
	bool operator < ( stEvent e )
	{
		if( e.eType == this->eType )
			return this->evtIdx < e.evtIdx;
		return this->eType < e.eType;
	}
};

class Calendar
{
public:
	Calendar();
	~Calendar();
	void	SendCalendarEventListReq(int nUserIdx, USHORT year, USHORT month);
	void	RecvCalendarMessage(CNetworkMessage* istr);

	void	SetData();
	
	int			GetEventInfoCount( int day );
	stEvent*	GetEventInfoAt( int day, int idx );
	int			GetNowTime()	{ return m_nNowTime;	}
	int			GetYear()		{ return m_nYear;		}
	int			GetMonth()		{ return m_nMonth;		}
	//bool		DataSort(vecEvtIter* first, vecEvtIter* end);

private:

	typedef std::vector<stEvent>	vecEvent;
	typedef vecEvent::iterator		vecEvtIter;
	typedef std::map<int,vecEvent>	mapEvent;
	typedef mapEvent::iterator		mapIter;

	void	ClearData();
	int		m_nCnt;

	reservedGMCommandElement*	m_pEventData;
	mapEvent	m_mapElement;
	int			m_nYear;
	int			m_nMonth;
	int			m_nNowTime;
};

#endif	// CALENDAR_SYSTEM_H_