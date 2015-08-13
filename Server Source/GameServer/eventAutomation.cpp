#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/DBCmd.h"
#include "Server.h"
#include "Log.h"
#include "CmdMsg.h"
#include "EventAutomation.h"

void CEventInfo::setRegen( bool _bRegen)
{
	int i;
	int regenListSize = (int)m_npcRegenList.size();
	for(i=0; i<regenListSize; i++)
	{
		if( !m_npcRegenList[i] )
			continue;
		m_npcRegenList[i]->m_bRegen = _bRegen;
		m_npcRegenList[i]->m_lastDieTime = 10;
	}
}

bool CEventAutomationManager::loadEvent()
{
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery("SELECT * FROM t_event ORDER BY a_index");

	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		GAMELOG << init("EVENT_AUTOMATION_WARN") << "NO EXIST TABLE t_event" << end;
		return false;
	}

	if( cmd.m_nrecords < 1 )
	{
		GAMELOG << init("EVENT_AUTOMATION_WARN") << "EMPTY TABLE t_event" << end;
		return false;
	}

	int index;
	int enable, notice;
	int drop[4];
	CLCString npclist;
	do
	{
		if( !cmd.GetRec("a_index", index)
				|| !cmd.GetRec("a_enable", enable)
				|| !cmd.GetRec("a_notice", notice)
				|| !cmd.GetRec("a_npclist", npclist)
				|| !cmd.GetRec("a_drop_1", drop[0])
				|| !cmd.GetRec("a_drop_2", drop[1])
				|| !cmd.GetRec("a_drop_3", drop[2])
				|| !cmd.GetRec("a_drop_4", drop[3])
		  )
		{
			GAMELOG << init("EVENT_AUTOMATION_WARN") << "NOT MATCHING FIELD TABLE t_event" << end;
			return false;
		}

		CEventInfo EInfo;

		EInfo.setEventIndex( index );
		EInfo.setEnable( (enable!=0 ? true : false) );
		EInfo.setNotice( (notice!=0 ? true : false) );

		const char* pNpcIndex = npclist;
		char buf[8]= {0,};
		int npcIndex=0;
		while (*pNpcIndex)
		{
			pNpcIndex = AnyOneArg(pNpcIndex, buf);
			npcIndex=atoi(buf) ;
			if( npcIndex > 0 )
				EInfo.addNpc( npcIndex);
		}

		int i=0;
		for(i=0; i<4; i++)
		{
			if( drop[i] > 0 )
				EInfo.addDrop( drop[i] );
		}

		m_eventList.insert( std::map<int, CEventInfo>::value_type(index, EInfo) );
		if( enable )
		{
			std::map<int, CEventInfo>::iterator itr;
			itr = m_eventList.find( index );
			if( itr != m_eventList.end() )
				m_activeList.insert( std::map<int, CEventInfo*>::value_type(index, &itr->second) );
		}
	}
	while( cmd.MoveNext() );

	return true;
}

CEventInfo* CEventAutomationManager::getActiveEventInfo(int _eventIndex)
{
	if( m_eventList.size() == 0 )
		return NULL;
	std::map<int, CEventInfo*>::iterator itr;
	itr = m_activeList.find( _eventIndex );
	if( itr == m_activeList.end() )
		return NULL;

	return itr->second;
}

CEventInfo* CEventAutomationManager::getEventInfo(int _eventIndex)
{
	if( m_eventList.size() == 0 )
		return NULL;
	std::map<int, CEventInfo>::iterator itr;
	itr = m_eventList.find( _eventIndex );
	if( itr == m_eventList.end() )
		return NULL;

	return &itr->second;
}

bool CEventAutomationManager::dbUpdate(int _eventIndex, bool _enable)
{
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	std::string update_event_query = boost::str(boost::format("UPDATE t_event SET a_enable=%d WHERE a_index=%d" ) % (int)_enable % _eventIndex );
	cmd.SetQuery(update_event_query);

	if( !cmd.Update() )
	{
		GAMELOG << init("EVENT_AUTOMATION_WARN") << update_event_query.c_str() << end;
		return false;
	}
	return true;
}

bool CEventAutomationManager::addActiveEvent(int _eventIndex, bool _dbUpdate)
{
	if( m_eventList.size() == 0 )
	{
		GAMELOG << init("EVENT_AUTOMATION_WARN") << "EMPTY EVENT DATA" << end;
		return false;
	}

	std::map<int, CEventInfo*>::iterator pItr = m_activeList.find( _eventIndex );
	if( pItr != m_activeList.end() )
	{
		GAMELOG << init("EVENT_AUTOMATION_WARN") << _eventIndex << " IS ALREADY ENABLED" << end;
		return false;
	}

	std::map<int, CEventInfo>::iterator itr;
	itr = m_eventList.find( _eventIndex );
	if( itr == m_eventList.end() )
	{
		GAMELOG << init("EVENT_AUTOMATION_WARN") << "CANNOT FIND EVENT NO. " << _eventIndex << end;
		return false;
	}

	if( _dbUpdate )
	{
		if( !dbUpdate(_eventIndex, true ) )
		{
			GAMELOG << init("EVENT_AUTOMATION_WARN") << "DB UPDATE FAILED" << end;
			return false;
		}
	}

	itr->second.setEnable(true);
	m_activeList.insert( std::map<int, CEventInfo*>::value_type(_eventIndex, &itr->second) );

	return true;
}

bool CEventAutomationManager::removeActiveEvent(int _eventIndex, bool _dbUpdate)
{
	if( m_activeList.size() == 0 )
	{
		GAMELOG << init("EVENT_AUTOMATION_WARN") << "EMPTY EVENT DATA" << end;
		return false;
	}

	std::map<int, CEventInfo*>::iterator itr;
	itr = m_activeList.find( _eventIndex );
	if( itr == m_activeList.end() )
	{
		GAMELOG << init("EVENT_AUTOMATION_WARN") << _eventIndex << " IS ALREADY DISABLED" << end;
		return false;
	}

	if(_dbUpdate)
	{
		if( !dbUpdate(_eventIndex, false ) )
		{
			GAMELOG << init("EVENT_AUTOMATION_WARN") << "DB UPDATE FAILED" << end;
			return false;
		}
	}

	itr->second->setEnable(false);
	m_activeList.erase( _eventIndex );
	return true;
}

bool CEventAutomationManager::isActiveEvent(int _eventIndex)
{
	if( m_eventList.size() == 0 )
	{
//		GAMELOG << init("EVENT_AUTOMATION_WARN") << "EMPTY EVENT DATA" << end;
		return false;
	}

	std::map<int, CEventInfo*>::iterator itr;
	itr = m_activeList.find( _eventIndex );
	if( itr == m_activeList.end() )
	{
		//GAMELOG << init("EVENT_AUTOMATION_WARN") << "EVENT IS NOT ENABLED" << end;
		return false;
	}

	return true;
}

std::map<int, CEventInfo*> * CEventAutomationManager::getActiveEvenList()
{
	if( m_activeList.size() == 0 )
		return NULL;

	return  &m_activeList;
}

std::map<int, CEventInfo> * CEventAutomationManager::getEvenList()
{
	if( m_eventList.size() == 0 )
		return NULL;

	return  &m_eventList;
}

bool CEventAutomationManager::isEventNpc(int _npcDBIndex, int & _eventIndex, bool & _bEnable )
{
	bool bFind = false;
	_eventIndex = -1;
	if( m_eventList.size() == 0 )
		return false;

	std::map<int, CEventInfo>::iterator itr;
	std::map<int, CEventInfo>::iterator itrEnd = m_eventList.end();
	for( itr = m_eventList.begin(); itr != itrEnd; itr++ )
	{
		if( itr->second.getNpcCount() == 0 )
			continue;

		if( itr->second.isEventNpc(_npcDBIndex) )
		{
			_eventIndex = itr->first;
			_bEnable = itr->second.getEnable();
			if (_bEnable)
				return true;
			else
				bFind = true;
		}
	}
	return bFind;
}

bool CEventAutomationManager::isActiveEventNpc(int _npcDBIndex, int & _eventIndex )
{
	_eventIndex = -1;
	if( m_activeList.size() == 0 )
		return false;

	std::map<int, CEventInfo*>::iterator itr;
	std::map<int, CEventInfo*>::iterator itrEnd = m_activeList.end();
	for( itr = m_activeList.begin(); itr != itrEnd; itr++ )
	{
		if( itr->second->getNpcCount() == 0 )
			return false;

		if( itr->second->isEventNpc(_npcDBIndex) )
		{
			_eventIndex = itr->first;
			return true;
		}
	}
	return false;
}

void CEventAutomationManager::addNpcRegenInfo(CNPCRegenInfo* _pNpcRegenInfo)
{
	int eventIndex;

	if(isEventNpc(_pNpcRegenInfo->m_npcIdx, eventIndex, _pNpcRegenInfo->m_bRegen) )
		getEventInfo(eventIndex)->addNpcRegenInfo(_pNpcRegenInfo);
}
