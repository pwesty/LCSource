// TitleSystem.cpp: implementation of the CTitleProtoList class.
//
//////////////////////////////////////////////////////////////////////
#include "stdhdrs.h"

#include "../ShareLib/DBCmd.h"
#include "Server.h"
#include "Log.h"
#include "TitleSystem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitleProtoList::CTitleProtoList()
{
	m_proto = NULL;
}

CTitleProtoList::~CTitleProtoList()
{
	if( m_proto )
	{
		delete[] m_proto;
		m_proto = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// CTitleProto Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitleProto::CTitleProto()
{
	int i, j;
	for(i = 0; i < MAX_TITLE_OPTION; i++)
	{
		for(j = 0; j < 2; j++)
		{
			m_option[i][j] = -1;
		}
	}
}

CTitleProto::~CTitleProto()
{
}

//////////////////////////////////////////////////////////////////////
// CTitle Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitle::CTitle()
{
	m_endtime = -1;
	m_proto = NULL;
}

CTitle::~CTitle()
{
	m_proto = NULL;
}

bool CTitleProtoList::Load()
{
	int optCount;
	CDBCmd dbTitle;
	dbTitle.Init(&gserver->m_dbdata);
	dbTitle.SetQuery("SELECT * FROM t_title WHERE a_enable=1 ORDER BY a_index");

	if (!dbTitle.Open())
		return false;

	m_nCount = dbTitle.m_nrecords;
	m_proto = new CTitleProto[m_nCount];

	// 스킬별 설정

	if(!dbTitle.MoveFirst())
		return false;

	for (int i=0; i < m_nCount; i++)
	{
		optCount = 0;
		dbTitle.GetRec("a_index",			m_proto[i].m_index);
		dbTitle.GetRec("a_time",			m_proto[i].m_time);
		dbTitle.GetRec("a_item_index",		m_proto[i].m_itemidx);
		dbTitle.GetRec("a_castle_num" ,		m_proto[i].m_castleNum);
		char tmpBuf[MAX_STRING_LENGTH] = {0,};
		char tmpBuf1[MAX_STRING_LENGTH] = {0,};
		for(int a = 0; a < MAX_TITLE_OPTION; a++)
		{
			strcpy(tmpBuf, "a_option_index");
			IntCat(tmpBuf, a, false);
			dbTitle.GetRec(tmpBuf,	m_proto[i].m_option[a][TITLE_SYSTEM_OPTION_NUM]);

			strcpy(tmpBuf1, "a_option_level");
			IntCat(tmpBuf1, a, false);
			dbTitle.GetRec(tmpBuf1,	m_proto[i].m_option[a][TITLE_SYSTEM_OPTION_LEVEL]);

			if(m_proto[i].m_option[a][TITLE_SYSTEM_OPTION_NUM] != -1)
				optCount++;
		}
		m_proto[i].m_nOptionCount = optCount;
		dbTitle.MoveNext();

		map_.insert(map_t::value_type(m_proto[i].m_index, &m_proto[i]));
	}

	return true;
}

CTitleProto* CTitleProtoList::FindProto(int index)
{
	map_t::iterator it = map_.find(index);
	return (it != map_.end()) ? it->second : NULL;
}

CTitle* CTitleProtoList::Create(int index, int time)
{
	CTitleProto* tmp1;
	CTitle* tmp2;
	tmp1 = FindProto(index);
	if(tmp1 != NULL)
	{
		tmp2 = new CTitle;
		tmp2->m_proto = tmp1;
		tmp2->m_endtime = time;
		return tmp2;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// CTitleList Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitleList::CTitleList()
{
	m_nCount = 0;
	m_head = NULL;
}

CTitleList::~CTitleList()
{
}

//////////////////////////////////////////////////////////////////////
// CTitleNode Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitleNode::CTitleNode(CTitle* title)
{
	m_title = title;
	m_next = m_prev = NULL;
}

CTitleNode::~CTitleNode()
{
}

bool CTitleList::Add(CTitle *title)
{
	if (!title)
		return false;

	CTitleNode* node = new CTitleNode(title);
	ADD_TO_BILIST(node, m_head, m_prev, m_next);
	m_nCount++;

	return true;
}

bool CTitleList::Remove(CTitle *title)
{
	if (!title)
		return false;

	CTitleNode* node = m_head;
	while (node)
	{
		if (node->m_title->m_proto->m_index == title->m_proto->m_index)
		{
			REMOVE_FROM_BILIST(node, m_head, m_prev, m_next);
			m_nCount--;
			delete node;
			node = NULL;
			return true;
		}
		node = node->m_next;
	}

	return false;
}

CTitle* CTitleList::Find(int index)
{
	CTitleNode* node = m_head;
	while (node)
	{
		if (node->m_title->m_proto->m_index == index)
			return node->m_title;
		else
			node = node->m_next;
	}
	return NULL;
}

bool CTitleList::HaveTitle(int index)
{
	CTitleNode* node = m_head;
	while(node)
	{
		if(node->m_title->m_proto->m_index == index)
			return true;
		node = node->m_next;
	}
	return false;
}
//DEL void CTitleList::DeleteExpireTime(int gameTime)
//DEL {
//DEL 	CTitleNode* tmp, tmp2;
//DEL 	tmp = m_head;
//DEL 	while(tmp)
//DEL 	{
//DEL 		if(tmp->m_title->m_endtime > gameTime)
//DEL 		{
//DEL 			//삭제
//DEL 			tmp2 = tmp;
//DEL 			tmp = tmp->m_next;
//DEL 			Remove(tmp2->m_title);
//DEL 		}
//DEL 	}
//DEL }

bool CTitleList::CheckTitleTime(int CurrentTitle)
{
	if(CurrentTitle == TITLE_SYSTEM_NO_TITLE)
		return false;

	if(this->m_head == NULL)
		return false;

	CTitle* title;
	title = this->Find(CurrentTitle);
	if(title == NULL)
		return false;
	if((title->m_endtime < gserver->getNowSecond()) && (title->m_endtime != -1))
	{
		if(Remove(title))
			return true;
		else
			return false;
	}
	return false;
}
//