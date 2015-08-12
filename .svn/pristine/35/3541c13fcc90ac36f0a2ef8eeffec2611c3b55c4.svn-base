#ifndef __NOTICE_SYSTEM_AGENT_H__
#define __NOTICE_SYSTEM_AGENT_H__

#include "../ShareLib/bnf.h"
#include "stdhdrs.h"

#if defined (NOTICE_SYSTEM)

#if !defined __NOTICESYSTEM_H__
#define __NOTICESYSTEM_H__

#include "UserNotice.h"

class CServerAgent;

class CNoticeSystemAgent : public rnSocketIOHandler
{
public:
	CNoticeSystemAgent(void);
	~CNoticeSystemAgent(void);

	virtual void operate(rnSocketIOService* service);

	void reg(int _characterIndex, CLCString _name, CLCString _contents);
	void initAgent(CServerAgent* _agent);

	static CNoticeSystemAgent* Instance();

private:
	void push_back( CUserNotice * _notice);
	void push_front( CUserNotice * _notice);
	CUserNotice* pop(void);
	void pop_front(void);
	int getSize(void);

private:
	std::deque<CUserNotice *> m_noticeContainer;
};

#endif // __NOTICESYSTEM_H__
#endif // NOTICE_SYSTEM

#endif
