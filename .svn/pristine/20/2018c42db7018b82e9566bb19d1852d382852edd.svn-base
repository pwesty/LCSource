#ifndef __MSG_LIST_H__
#define __MSG_LIST_H__

class CMsgListNodeServerNode
{
public:
	CDescriptor*			m_server;
	bool					m_recieved;
	CMsgListNodeServerNode*	m_pPrev;
	CMsgListNodeServerNode*	m_pNext;

	CMsgListNodeServerNode(CDescriptor* server)
	{
		m_server = server;
		m_recieved = false;
		m_pPrev = NULL;
		m_pNext = NULL;
	}
};

class CMsgListNode
{
public:
	int							m_seq;
	bool						m_bReq;
	CNetMsg::SP					m_msg;
	bool						m_bSent;
	int							m_sendTime;
	CDescriptor*				m_reqServer;
	CMsgListNodeServerNode*		m_head;
	CMsgListNode*				m_pPrev;
	CMsgListNode*				m_pNext;

	CMsgListNode(int seq, bool bReq, CDescriptor* reqServer, CNetMsg::SP& msg)
	{
		m_seq = seq;
		m_bReq = bReq;
		m_msg = msg;
		m_bSent = false;
		m_sendTime = -1;
		m_reqServer = reqServer;
		m_head = NULL;
	}

	~CMsgListNode()
	{
		CMsgListNodeServerNode* p;
		while (m_head)
		{
			p = m_head->m_pNext;
			delete m_head;
			m_head = p;
		}
	}

	CMsgListNodeServerNode* Add(CDescriptor* server)
	{
		CMsgListNodeServerNode* node = new CMsgListNodeServerNode(server);
		ADD_TO_BILIST(node, m_head, m_pPrev, m_pNext);
		return node;
	}

	bool RemoveServer(CDescriptor* server)
	{
		CMsgListNodeServerNode* p;
		CMsgListNodeServerNode* p2 = m_head;

		while ((p = p2))
		{
			p2 = p->m_pNext;
			if (p->m_server == server)
			{
				REMOVE_FROM_BILIST(p, m_head, m_pPrev, m_pNext);
				delete p;
			}
		}

		if (m_head)
			return false;
		else
			return true;
	}
};

class CMsgList
{
public:
	CMsgListNode* m_head;

	CMsgList()
	{
		m_head = NULL;
	}

	~CMsgList()
	{
		CMsgListNode* p;
		while (m_head)
		{
			p = m_head->m_pNext;
			delete m_head;
			m_head = p;
		}
	}

	CMsgListNode* Add(int seq, bool bReq, CDescriptor* reqServer, CNetMsg::SP& msg)
	{
		//CNetMsg::SP tmsg(new CNetMsg(msg));
		msg->MoveFirst();

		//tmsg->MoveFirst();
		if (bReq)
			RefMsg(msg) << seq;
		else
			RefMsg(msg) << (int)-1;
		CMsgListNode* node = new CMsgListNode(seq, bReq, reqServer, msg);
		ADD_TO_BILIST(node, m_head, m_pPrev, m_pNext);
		return node;
	}

	void Remove(CMsgListNode* node)
	{
		REMOVE_FROM_BILIST(node, m_head, m_pPrev, m_pNext);
		delete node;
	}

	void RemoveServer(CDescriptor* server)
	{
		CMsgListNode* p;
		CMsgListNode* p2 = m_head;

		while ((p = p2))
		{
			p2 = p->m_pNext;
			if (p->RemoveServer(server))
			{
				REMOVE_FROM_BILIST(p, m_head, m_pPrev, m_pNext);
				delete p;
			}
		}
	}
};

#endif
//
