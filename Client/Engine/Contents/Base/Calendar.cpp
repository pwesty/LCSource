
#include "StdH.h"

#include <Engine/Interface/UIManager.h>
#include <Common/Packet/ptype_reserved_gm_command.h>
#include <Engine/Network/NetworkMessage.h>
#include <Engine/network/CNetwork.h>

#include "UICalendar.h"
#include "Calendar.h"

Calendar::Calendar()
	: m_pEventData(NULL)
	, m_nCnt(-1)
	, m_nYear(0)
	, m_nMonth(0)
	, m_nNowTime(0)
{
}

Calendar::~Calendar()
{
	ClearData();
}

void Calendar::SendCalendarEventListReq(int nUserIdx, USHORT year, USHORT month)
{
	CNetworkMessage nmEventList;
	RequestClient::reservedGMCommandList* packet = reinterpret_cast<RequestClient::reservedGMCommandList*>(nmEventList.nm_pubMessage);
	packet->type	= MSG_RESERVED_GM_COMMAND;
	packet->subType = MSG_SUB_RESERVED_GM_COMMAND_LIST;
	packet->m_Index = nUserIdx;
	packet->year	= year;
	packet->month	= month;
	nmEventList.setSize(sizeof(RequestClient::reservedGMCommandList));

	_pNetwork->SendToServerNew(nmEventList);
}

void Calendar::RecvCalendarMessage( CNetworkMessage* istr )
{
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(istr->GetBuffer());
	CUIManager* pMng = CUIManager::getSingleton();
	switch( pBase->subType )
	{
	case MSG_SUB_RESERVED_GM_COMMAND_LIST:
		{			
			ResponseClient::reservedGMCommandList* pData = reinterpret_cast<ResponseClient::reservedGMCommandList*>(istr->GetBuffer());

			m_nYear		 = pData->year;
			m_nMonth	 = pData->month;
			m_nNowTime	 = pData->nowTime;

			m_nCnt = pData->count;

			ClearData();
			if( pData->count > 0 )
			{
				m_pEventData = new reservedGMCommandElement[pData->count];
				
				if( m_pEventData == NULL )
					break;

				memcpy( m_pEventData, &pData->ele, sizeof(reservedGMCommandElement) * pData->count);
				SetData();
			}

			if( pMng->isCreateVisible(UI_CALENDAR) )
				pMng->GetCalendar()->SetTime();
		}
		break;
	}
}

void Calendar::ClearData()
{
	mapIter it = m_mapElement.begin();
	for( ; it != m_mapElement.end(); ++it )
	{
		it->second.clear();
	}
	m_mapElement.clear();
	SAFE_ARRAY_DELETE(m_pEventData)
}

void Calendar::SetData()
{
	int nDay, cnt;
	time_t tmt;
	struct tm *pStart, *pEnd;

	for( nDay = 1; nDay < 32; nDay++ )
	{
		vecEvent vecEvent_;
		for( cnt = 0; cnt < m_nCnt; cnt++ )
		{
			tmt = m_pEventData[cnt].a_startTime;
			pStart = localtime(&tmt);

			if( pStart->tm_mon + 1 == m_nMonth )	// 시작 하는 달과 현재 달이 같을 경우
			{
				if( pStart->tm_mday == nDay )
				{
					stEvent event;
					int type			 = m_pEventData[cnt].a_evnetType;
					event.eType			 = type;
					event.evtIdx		 = m_pEventData[cnt].a_Index;
					event.bEvtStarted	 = true;
					event.pData			 = &m_pEventData[cnt];
					vecEvent_.push_back(event);
				}
			}
			
			tmt = m_pEventData[cnt].a_endTime;
			pEnd = localtime(&tmt);
			if( pEnd->tm_mon + 1 == m_nMonth )	// 끝나는 달과 현재 달이 같을 경우.
			{
				if( pEnd->tm_mday == nDay )
				{
					stEvent event;
					int type			= m_pEventData[cnt].a_evnetType;
					event.eType			= type;
					event.evtIdx		= m_pEventData[cnt].a_Index;
					event.bEvtStarted	= false;
					event.pData			= &m_pEventData[cnt];
					vecEvent_.push_back(event);
				}
			}
		}
		if( vecEvent_.size() > 0 )
		{
			std::sort(vecEvent_.begin(), vecEvent_.end());
			m_mapElement.insert(std::pair<int,vecEvent>(nDay,vecEvent_));
		}
	}
}

int Calendar::GetEventInfoCount( int day )
{
	mapIter it = m_mapElement.find(day);

	if( it != m_mapElement.end() )
		return it->second.size();

	return 0;
}

stEvent* Calendar::GetEventInfoAt( int day, int idx )
{
	mapIter it = m_mapElement.find(day);

	if( it != m_mapElement.end() )
	{
		if( idx < 0 || idx >= it->second.size() )
			return NULL;

		return (&it->second[idx]);
	}
	return NULL;
}