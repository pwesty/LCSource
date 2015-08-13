#include "stdhdrs.h"
#include "Log.h"
#include "UserList.h"


//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )
/////////////////////////////////////////////////////////////////////
// UserList

CUser* CUserList::Add(	int subno,
			int userindex,
			const char* billguid,
			int billsection,
			const char* billviewname,
			struct tm endtime,
			int surplus,
			bool reserve_found,
			const char* reserve_billguild,
			int reserve_billvalue,
			const char* reserve_billname,
			int pulseRead)
{

#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CUser* ret = Find(userindex);
	if (ret)
	{
		ret->billguid(billguid);
		ret->billsection(billsection);
		ret->billviewname(billviewname);
		ret->endtime(endtime);
		ret->surplus(surplus);
		ret->reserve_found(reserve_found);
		ret->reserve_billguild(reserve_billguild);
		ret->reserve_billvalue(reserve_billvalue);
		ret->reserve_billname(reserve_billname);

		ret->bSent3Day(false);
		ret->bSent1Day(false);
		ret->bSent60(false);
		ret->bSent30(false);
		ret->bSent10(false);
		ret->bSent5(false);
		ret->bSent1(false);
		ret->bSendTimeOut(false);

		ret->pulseRead(pulseRead);

		return ret;
	}
	ret = new CUser(subno, userindex, billguid, billsection, billviewname, endtime, surplus, reserve_found, reserve_billguild, reserve_billvalue, reserve_billname, pulseRead);
	ret->m_next = m_head;
	if (m_head)
		m_head->m_prev = ret;
	m_head = ret;

	return ret;
}

void CUserList::Remove(CUser* user)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	if (user == NULL)
		return ;
	if (user == m_head)
		m_head = user->m_next;
	if (user->m_prev)
		user->m_prev->m_next = user->m_next;
	if (user->m_next)
		user->m_next->m_prev = user->m_prev;
	user->m_prev = user->m_next = NULL;
	delete user;
#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
}

CUser* CUserList::Find(int userindex)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CUser* p = m_head;
	while (p)
	{
		if (p->m_index == userindex)
		{
#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
			return p;
		}
		else
			p = p->m_next;
	}
#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
	return NULL;
}

void CGamePointList::Reset()
{

#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CGamePoint* p;
	while (m_head)
	{
		p = m_head;
		m_head = m_head->m_next;
		delete p;
	}
	m_head = NULL;
	m_tail = NULL;
	m_total = 0;
}

void CGamePointList::Add(const char* itemguid, int surplus, const char* sessionguid)
{

#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CGamePoint* p = new CGamePoint(itemguid, surplus, sessionguid);

	if (m_head)
	{
		m_tail->m_next = p;
		p->m_prev = m_tail;
		m_tail = p;
	}
	else
	{
		m_head = m_tail = p;
	}
	m_total += surplus;
}

void CSupUpdateItemList::Reset()
{

#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CSupUpdateItem* p;
	while (m_head)
	{
		p = m_head;
		m_head = m_head->m_next;
		delete p;
	}
	m_head = NULL;
	m_tail = NULL;
	m_total = 0;
}

void CSupUpdateItemList::Add( int index	)
{

#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CSupUpdateItem* p = new CSupUpdateItem( index );
	if (m_head)
	{
		m_tail->m_next = p;
		p->m_prev = m_tail;
		m_tail = p;
	}
	else
	{
		m_head = m_tail = p;
	}
	m_total++;
}


void CSupItemList::Reset()
{

#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CSupItem* p;
	while (m_head)
	{
		p = m_head;
		m_head = m_head->m_next;
		delete p;
	}
	m_head = NULL;
	m_tail = NULL;
	m_total = 0;
}

void CSupItemList::Add( int index,
	char* catalog_id,
	int userindex,
	int server,
	int itemindex,
	int itemplus,
	int itemflag,
	int itemcount
	)
{

#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CSupItem* p = new CSupItem( index,
								catalog_id,
								userindex,
								server,
								itemindex,
								itemplus,
								itemflag,
								itemcount
								);

	if (m_head)
	{
		m_tail->m_next = p;
		p->m_prev = m_tail;
		m_tail = p;
	}
	else
	{
		m_head = m_tail = p;
	}
	m_total++;
}
#endif //#ifndef LC_TLD