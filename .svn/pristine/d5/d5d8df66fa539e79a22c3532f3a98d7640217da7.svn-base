#include "stdhdrs.h"

#include "Server.h"
#include "BillingClient.h"
#include "User.h"
#include "Log.h"
CUser::CUser(int index, const char* name, int server, int subnum, const char* ip, const char* ident)
	: m_name(MAX_ID_NAME_LENGTH + 1)
	, m_ip(HOST_LENGTH + 1)
	,m_proSite(3)
#ifdef CHARDEL_CHECKID
	, m_identification(8)
#endif
{
	m_index = index;
	m_name = name;
	m_server = server;
	m_subnum = subnum;
	m_zone = 0;
	m_ip = ip;
	m_state = CONN_WAIT_IDEN;
	m_listindex = -1;
	m_timestamp = 0;
	m_checkTics = 0;

	m_preopen_chk = 0;

	m_usertype = BILL_USER_FREE;
	m_timeremain = 0;
	m_paytype = BILL_PAY_FREE;
	m_location = BILL_LOCATION_HOME;
//0627
	m_userFlag = 0;

#ifdef CHARDEL_CHECKID
	if( ident )
		m_identification = ident;
#endif
}

CUserList::CUserList()
{
	m_last = 0;
	m_count = 0;
	m_users = new CUser*[MAX_PLAYING];
	memset(m_users, 0, sizeof(CUser*) * MAX_PLAYING);
	memset(m_playersPerZone, -1, sizeof(int) * MAX_ZONES);
}

CUserList::~CUserList()
{
	if (m_users)
	{
		for (int i = 0; i <= m_last; i++)
			if (m_users[i])
				delete m_users[i];
		delete [] m_users;
	}
}

CUser* CUserList::Add(CUser* user)
{
	if (m_count >= MAX_PLAYING)
		return NULL;

	int i;
	for (i = 0; i <= m_last; i++)
	{
		if (m_users[i] == NULL)
			break;
	}

	m_users[i] = user;
	user->m_listindex = i;
	m_count++;
	if (m_last < i)
		m_last = i;

	GAMELOG << init("PLAYER_ADD", m_users[i]->m_name)
			<< i
			<< end;

	return m_users[i];
}

bool CUserList::Remove(CUser* user, bool bFree)
{
	if (user->m_listindex < 0 || user->m_listindex >= MAX_PLAYING || m_users[user->m_listindex] != user)
		return false;

	GAMELOG << init("PLAYER_DEL", user->m_name)
			<< user->m_listindex
			<< end;

	m_users[user->m_listindex] = NULL;
	m_count--;
	if (bFree)
		delete user;

	return true;
}

CUser* CUserList::Find(const char* name)
{
	int i;
	for (i = 0; i <= m_last; i++)
	{
		if (m_users[i])
		{
			if (strcmp2(m_users[i]->m_name, name) == 0)
				return m_users[i];
		}
	}
	return NULL;
}

CUser* CUserList::FindByUserIndex(int userindex)
{
	int i;
	for (i = 0; i <= m_last; i++)
	{
		if (m_users[i])
		{
			if (m_users[i]->m_index == userindex)
				return m_users[i];
		}
	}
	return NULL;
}
//