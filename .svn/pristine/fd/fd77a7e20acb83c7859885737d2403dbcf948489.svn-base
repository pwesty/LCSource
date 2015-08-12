#ifndef __ADESCRIPTOR_H__
#define __ADESCRIPTOR_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"
#include "../ShareLib/MemoryPoolBase.h"

class CDescriptor : public rnSocketIOHandler, public MemoryPoolBase<CDescriptor>
{
public:
	explicit CDescriptor(rnSocketIOService* service, bool client_type);
	~CDescriptor();

	rnSocketIOService*	service_;
	bool		client_type_;				// 게임서버와 유저 클라이언트를 구분
	// true = 게임서버, false = 유저 클라이언트

	int			m_serverNo;					// 접속한 서버 번호
	int			m_subNo;					// 접속 서브 번호
	int*		m_zones;					// 소속 존 리스트
	int			m_countZone;				// 소속 존 개수

	////////////
	// 상태 관련
	int			m_connected;				// CON_ 상수 값을 저장 : 접속 상태
	int			m_uniqeKey;

	////////////
	// 링크 관련
	CDescriptor* m_pPrev;					// 리스트 링크
	CDescriptor* m_pNext;

	virtual void onClose(rnSocketIOService* service);
	virtual void operate(rnSocketIOService* service);

	void WriteToOutput(CNetMsg::SP& msg);	// 쓰기

	bool GetLogin(CNetMsg::SP& msg);		// 접속 요청 처리
	int FindZone(int zone);					// 해당 존 번호 찾기

	bool userAuth(CNetMsg::SP& msg);		// 유저 인증

private:
	static int getUniqueKey();
};

#endif
//
