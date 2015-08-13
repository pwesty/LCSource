#include "stdhdrs.h"

#include "Descriptor.h"
#include "WaitPlayerList.h"

CWaitPlayerList::CWaitPlayerList()
{
}

CWaitPlayerList::~CWaitPlayerList()
{
	set_wait_player_t::iterator it = m_waitPlayerList.begin();
	set_wait_player_t::iterator endit = m_waitPlayerList.end();
	for (; it != endit; ++it)
	{
		delete (*it);
	}
	m_waitPlayerList.clear();
}

int CWaitPlayerList::Add(const char*id, const char* password, int server, int subno, const char* ip, char playmode, CDescriptor* desc )
{
	WAIT_PLAYER* newplayer = new WAIT_PLAYER;
	strcpy( newplayer->id , id );
	strcpy( newplayer->password , password );
	strcpy( newplayer->ip , ip );
	newplayer->server = server;
	newplayer->subno = subno;
	newplayer->playmode = playmode;
	newplayer->desc		= desc;
	newplayer->key		= (int)newplayer;
	m_waitPlayerList.insert( newplayer );
	return newplayer->key;
}

int CWaitPlayerList::Del( WAIT_PLAYER* delplayer )
{
	set_wait_player_t::iterator it = m_waitPlayerList.find(delplayer);
	if (it != m_waitPlayerList.end())
	{
		delete (*it);
		m_waitPlayerList.erase(it);
	}

	return m_waitPlayerList.size();
}

WAIT_PLAYER* CWaitPlayerList::Find( int key )
{
	set_wait_player_t::iterator it = m_waitPlayerList.begin();
	set_wait_player_t::iterator endit = m_waitPlayerList.end();
	for (; it != endit; ++it)
	{
		WAIT_PLAYER* p = *(it);
		if (p->key == key)
		{
			return p;
		}
	}

	return NULL;
}
//