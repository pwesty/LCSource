#ifndef __ADESCRIPTOR_H__
#define __ADESCRIPTOR_H__

#include "../ShareLib/rnsocketioservice.h"
#include "../ShareLib/MemoryPoolBase.h"

class CDescriptor : public rnSocketIOHandler, public MemoryPoolBase<CDescriptor>
{
public:
	explicit CDescriptor(rnSocketIOService* service);
	~CDescriptor();

	int			m_serverNo;					// 접속한 서버 번호
	int			m_subNo;					// 접속 서브 번호
	int*		m_zones;					// 소속 존 리스트
	int			m_countZone;				// 소속 존 개수

	////////////
	// 소켓 관련
	rnSocketIOService*	service_;			// file descriptor for socket

	////////////
	// 상태 관련
	int			m_connected;				// CON_ 상수 값을 저장 : 접속 상태

	////////////
	// 링크 관련
	CDescriptor* m_pPrev;					// 리스트 링크
	CDescriptor* m_pNext;

	void WriteToOutput(CNetMsg::SP& msg);		// 쓰기
	bool GetLogin(CNetMsg::SP& msg);		// 접속 요청 처리
	int FindZone(int zone);					// 해당 존 번호 찾기
	void SendPartyInfo();					// 파티/파티 매칭 정보 전달

	virtual void onClose(rnSocketIOService* service);
	virtual void operate(rnSocketIOService* service);
};

#endif
//
