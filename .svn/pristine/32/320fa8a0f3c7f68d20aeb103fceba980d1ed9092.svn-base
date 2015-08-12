#include "NoticeSystemAgent.h"

#if defined (NOTICE_SYSTEM)
#include "CmdMsg.h"
#include "Server.h"


CNoticeSystemAgent::CNoticeSystemAgent(void)
{
	m_noticeContainer.clear();
}

CNoticeSystemAgent::~CNoticeSystemAgent(void)
{
	std::deque<CUserNotice *>::iterator itr = m_noticeContainer.begin();
	std::deque<CUserNotice *>::iterator itrEnd = m_noticeContainer.end();
	for ( ; itr != itrEnd; ++itr )
	{
		delete (*itr);
	}

	m_noticeContainer.clear();
}

static CNoticeSystemAgent __instance;
CNoticeSystemAgent* CNoticeSystemAgent::Instance()
{
	return &__instance;
}

void CNoticeSystemAgent::reg(int _characterIndex, CLCString _name, CLCString _contents)
{
	CUserNotice * _notice = new CUserNotice;

	_notice->setCharIndex(_characterIndex);
	_notice->setName(_name);
	_notice->setContents(_contents);

	push_back(_notice);
}

void CNoticeSystemAgent::push_back( CUserNotice * _notice)
{
	m_noticeContainer.push_back(_notice);
}

void CNoticeSystemAgent::push_front( CUserNotice * _notice)
{
	m_noticeContainer.push_front(_notice);
}

CUserNotice* CNoticeSystemAgent::pop(void)
{
	std::deque<CUserNotice *>::iterator itr;

	itr = m_noticeContainer.begin();

	return (*itr);
}

void CNoticeSystemAgent::pop_front(void)
{
	m_noticeContainer.pop_front();
}

int CNoticeSystemAgent::getSize(void)
{
	return m_noticeContainer.size();
}

void CNoticeSystemAgent::operate( rnSocketIOService* service )
{
	// 매 11초마다

	if ( getSize() <= 0 )
		return;

	// 큐의 맨 앞 데이터 팝
	CUserNotice* pNotice = pop();

	// 큐의 맨 앞 데이터 삭제
	pop_front();

	// 뷰 카운터 증가
	pNotice->increaseVieCount();

	// 클라이언트로 보내기
	CNetMsg::SP msg(new CNetMsg);
	SubHelperUserNoticePrint(msg, pNotice->getName(), pNotice->getContents(), pNotice->getViewCount());
	gserver.SendToAllGameServer(msg);

	// 뷰 카운터가 3회 이상이면 메모리 삭제
	if ( pNotice->getViewCount() >= 3 )
	{
		delete pNotice;
	}
	// TODO :: 뷰 카운터가 3회 미만이면 푸시 백
	else
	{
		push_back(pNotice);
	}
}

#endif // NOTICE_SYSTEM
