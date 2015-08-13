#ifndef __ADESCRIPTOR_H__
#define __ADESCRIPTOR_H__

#include "InputBuffer.h"
#include "InputQueue.h"
#include "OutputBuffer.h"
#include "UserList.h"

class CDescriptor  
{
public:
	CDescriptor();
	~CDescriptor();

	int			m_serverNo;					// 접속한 서버 번호
	int			m_gameId;					// 접속한 서버의 게임 아이디 

	////////////
	// 소켓 관련
	socket_t	m_desc;						// file descriptor for socket
	char		m_host[HOST_LENGTH + 1];	// client ip
	CInputBuffer m_inBuf;					// Input Buffer
	CInputQueue m_inQ;						// Input Packet Queue
	COutputBuffer m_outBuf;					// Output Network Message Queue

	//////////
	// 틱 관련
	int			m_timestamp;				// player: total playing time,  mob: last attacked pulse

	////////////
	// 상태 관련
	int			m_connected;				// CON_ 상수 값을 저장 : 접속 상태
	int			m_connectreq;				// 접속 요청 틱 카운트
	int			m_logined;					// 로그인 결과
	int			m_descnum;					// unique num assigned to desc
	bool		m_bclosed;

//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )
	CUserList	m_userlist;
#endif
	
	////////////
	// 링크 관련
	CDescriptor* m_pPrev;					// 리스트 링크
	CDescriptor* m_pNext;


	void CloseSocket();						// 소켓 닫기
	void FlushQueues();						// 큐 비우기

	void WriteToOutput(CNetMsg& msg);		// 쓰기
	int ProcessOutput();					// 쓰기 수행
	static int WriteToDescriptor(socket_t desc, const char* buf, int length);

	int ProcessInput();						// 읽기 수행
	static ssize_t PerformSocketRead(socket_t desc, char* read_point, size_t space_left);

	bool GetLogin(CNetMsg& msg);			// 접속 요청 처리
};

#endif
