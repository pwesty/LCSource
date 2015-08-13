#ifndef __ADESCRIPTOR_H__
#define __ADESCRIPTOR_H__

#include "../ShareLib/rnsocketioservice.h"
#include "../ShareLib/rnsocketiohandler.h"
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

	rnSocketIOService* service_;

	////////////
	// 상태 관련
	int			m_connected;				// CON_ 상수 값을 저장 : 접속 상태
	int			m_logined;					// 로그인 결과

	////////////
	// 링크 관련
	CDescriptor* m_pPrev;					// 리스트 링크
	CDescriptor* m_pNext;

public:
	static CDescriptor* create(rnSocketIOService* service);

	void WriteToOutput(CNetMsg::SP& msg);		// 쓰기
	bool GetLogin(CNetMsg::SP& msg);			// 접속 요청 처리
	int FindZone(int zone);					// 해당 존 번호 찾기
	virtual void operate(rnSocketIOService* service);
	virtual void onClose(rnSocketIOService* service);
};

#endif
//
