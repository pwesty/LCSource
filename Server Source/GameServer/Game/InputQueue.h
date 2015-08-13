// InputQueue.h: interface for the CInputQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUTQUEUE_H__D2841812_61A5_4982_9D9A_47205BF43165__INCLUDED_)
#define AFX_INPUTQUEUE_H__D2841812_61A5_4982_9D9A_47205BF43165__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "Mutex.h"

class CInputQueue  
{
	class CNode
	{
	public:
		CNetMsg		m_msg;
		CNode*		m_next;
		CNode(CNetMsg& msg)
		{
			m_msg.Init(msg);
			m_next = NULL;
		}
	};

	CMutexObject m_mutex;
	CNode* m_head;
	CNode* m_tail;
	int count;

public:
	CInputQueue()
	{
		m_head = NULL;
		m_tail = NULL;
		count = 0;
	}

	virtual ~CInputQueue()
	{
		Clear();
	}

	void Clear()
	{
		CMutex lock(&m_mutex);

		CNode* d = m_head;
		while (d)
		{
			m_head = d->m_next;
			delete d;
			d = m_head;
		}
		m_head = NULL;
		m_tail = NULL;
		count = 0;
	}

	bool Add(CNetMsg& msg, int maxcount)
	{
		CMutex lock(&m_mutex);

		CNode* node = new CNode(msg);
		if (!m_head)
			m_head = m_tail = node;
		else
		{
			m_tail->m_next = node;
			m_tail = node;
		}
		count++;
		// TODO : 나중에 슬롯수가 커지면 이것도 크게 잡아야 함
		if (count > maxcount)
		{
		//	GAMELOG << init("ISY : Data Count") 
	//				<< count << delim << maxcount << end;
			return false;
		}
		return true;
	}

	bool Get(CNetMsg& ret)
	{
		CMutex lock(&m_mutex);

		if (IsEmpty())
			return false;

		CNode* d = m_head;
		m_head = m_head->m_next;
		if (!m_head)
			m_tail = NULL;
		ret.Init(d->m_msg);
		delete d;
		d = NULL;
		ret.MoveFirst();
		count--;
		return true;
	}

	bool IsEmpty()
	{
		if (m_head)
			return false;
		else
			return true;
	}
};

#endif // !defined(AFX_INPUTQUEUE_H__D2841812_61A5_4982_9D9A_47205BF43165__INCLUDED_)
