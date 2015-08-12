#ifndef __ADBTHREAD_H__
#define __ADBTHREAD_H__

#include "Descriptor.h"
#include "Mutex.h"
#include "Server.h"

#define MAX_DB_Q  100000

class CCharDBQ
{
public:
	CCharDBQ();
	~CCharDBQ();

	CDescriptor*	m_desc[MAX_DB_Q];	// 요청 큐
	CMutexObject	m_mutexobj;			// 동기 객체

	int				m_head;				// 처음
	int				m_tail;				// 끝
	bool			m_dbend;				// 종료 여부
	bool			m_dbreqend;			// 종료 요청

	CDescriptor* GetFromQ();			// 큐에서 얻기
	bool AddToQ(CDescriptor* d);		// 큐에 넣기
	void EraseQ(CDescriptor* d);		// 큐 지우기

	bool IsFull()	{ return (m_head == (m_tail + 1) % MAX_DB_Q); }	// 가득 찼는지 검사
	bool IsEmpty()	{ return (m_head == m_tail); }					// 비었는지 검사
};

class CDBThread
{
public:
	CDBThread();
	~CDBThread();

	CCharDBQ		m_queue;			// 큐
	char			m_t_buf[8192];	// for temp query
	char			m_buf[8192];	// for main query
	char			m_te_buf[1024];	// for temp string

	bool CreateThread();

#ifdef CIRCLE_WINDOWS
	static void RunThread(void* p);
#else
	static void* RunThread(void* p);
#endif

	void Run();

	void Wait300Millis()
	{
#ifdef CIRCLE_WINDOWS
		Sleep(300);
#else
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 300000;
		select(0, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, &timeout);
#endif
	}

	void PingDB();
	bool UpdateChar(CDescriptor* d);		// 캐릭터 정보 갱신

	bool OnUserDB(CDescriptor* d);			// 캐릭터 정보를 모두 얻기 : for menu
	bool OnNewCharDB(CDescriptor* d);		// 캐릭터 생성
	bool OnDelCharDB(CDescriptor* d);		// 캐릭터 삭제
#ifdef ENABLE_CHARACTER_DELETE_DELAY
	bool OnDelCancelCharDB(CDescriptor* d);	// 캐릭터 삭제 취소
#endif // ENABLE_CHARACTER_DELETE_DELAY
	bool OnPlayCharDB(CDescriptor* d);		// 플레이할 캐릭터 얻기
	bool OnDisconnect(CDescriptor* d);		// 접종시 DB 행동

// 프로모션 사이트 가입자 아이템 지급 함수
	// 케릭터, 파트너아이디(4G : 0, NM : 1), 행운여부(300명중에 1명)
	void PromotionGive(CPC* pChar, int partner_id, bool lucky);

	////////////////
	// util function
	//static void BackSlash(char* des, const char* src);
	static void GetDateStr(char* buf);
	
	// 구현안된 생산 스킬 복구
	void CheckSSkill( int & index, int &level, int & totoal_recover_sp);
};

#endif
