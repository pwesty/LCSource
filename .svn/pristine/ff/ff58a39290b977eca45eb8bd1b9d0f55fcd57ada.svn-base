#ifndef __ADESCRIPTOR_H__
#define __ADESCRIPTOR_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"
#include "../ShareLib/MemoryPoolBase.h"

class CDescriptor : public rnSocketIOHandler, public MemoryPoolBase<CDescriptor>
{
public:
	explicit CDescriptor(rnSocketIOService* service);
	~CDescriptor();

	////////////
	// 소켓 관련
	rnSocketIOService*	service_;

	////////////
	// 상태 관련
	int			m_connected;				// CON_ 상수 값을 저장 : 접속 상태
	int			m_connectreq;				// 접속 요청 틱 카운트
	int			m_descnum;					// unique num assigned to desc
	int			m_playmode;					// login mode or play mode
	bool		m_bclosed;
	bool		m_bStartServer;				// 초기 접속용 서버 여부(존0을 가지고 있을때)
	CLCString	m_ipAddr;					// 서버의 주소
	int			m_portNumber;				// 서버의 포트 번호
	bool		m_bLoginServer;				// 로그인 서버인지 판별
	int			m_subno;					// 서브 번호
	int			m_waitbill;					// 빌링에 요청 결과 기다림
	int			m_billtimeout;				// 빌링 대기시간

	void WaitBillReply(int serial)
	{
		if (serial)
		{
			m_waitbill = serial;
			m_billtimeout = 0;
		}
		else
		{
			m_waitbill = 0;
		}
	}

	// 존 관련
	int			m_nCountZone;				// 해당 서버에 존재하는 존 수
	int*		m_nZoneIndex;				// 해당 서버에 존재하는 존

	////////////
	// 링크 관련
	CDescriptor* m_pPrev;					// 리스트 링크
	CDescriptor* m_pNext;

	void WriteToOutput(CNetMsg::SP& msg);	// 쓰기
	bool GetLogin(CNetMsg::SP& msg);			// 버전 아이디 비번 검사

	virtual void operate(rnSocketIOService* service);
	virtual void onClose(rnSocketIOService* service);
};

#endif
//
