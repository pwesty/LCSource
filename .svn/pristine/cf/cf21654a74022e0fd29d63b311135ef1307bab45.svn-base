#ifndef __ADESCRIPTOR_H__
#define __ADESCRIPTOR_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"
#include "../ShareLib/MemoryPoolBase.h"

enum
{
	CLIENT_TYPE_CLIENT,		// 유저용
	CLIENT_TYPE_CONNECTOR,	// Connector server 접속용
};

class CDescriptor;

//////////////////////////////////////////////////////////////////////////

class CDescriptorReconnectTimer : public rnSocketIOHandler
{
public:
	explicit CDescriptorReconnectTimer(CDescriptor* desc) : desc_(desc) {}
	~CDescriptorReconnectTimer() {}

	virtual void operate(rnSocketIOService* service);

private:
	CDescriptor*		desc_;
};

//////////////////////////////////////////////////////////////////////////

class CDescriptor : public rnSocketIOHandler
{
public:
	explicit CDescriptor(rnSocketIOService* service, int client_type);
	~CDescriptor();

	rnSocketIOService*		service_;
	int	client_type_;

	////////////
	// 계정 관련
	CLCString	m_idname;			// 유저 아이디
	CLCString	m_passwd;			// 암호

	//////////////////////////////////////////////////////////////////////////
	// 서버군 관련 (아래의 변수는 Connector server와 연결되는 세션에서만 사용됨)
	int			m_hardcore_flag;	// 가미고에서 사용하는 hardcore server, 0 - no, 1 - yes
	int			m_serverNo;			// 서버군 번호
	int			m_nMaxServer;		// 서버군 별 최대 서브 서버 수
	int*		m_serverSubNo;		// 서브 번호
	int*		m_playerNum;		// 서버군별 사용자 수
	char**		m_ipAddr;			// 서버의 주소
	int*		m_portNumber;		// 서버의 포트 번호

	std::string	connect_host_;			// 접속할 Connector server의 정보
	int			connect_port_;
	CDescriptorReconnectTimer reconnect_timer_;
	//////////////////////////////////////////////////////////////////////////

	////////////
	// 상태 관련
	int			m_connected;				// CON_ 상수 값을 저장 : 접속 상태
	bool		m_bclosed;

	////////////
	// 링크 관련
	CDescriptor* m_pPrev;					// 리스트 링크
	CDescriptor* m_pNext;

	void WriteToOutput(CNetMsg::SP& msg);	// 쓰기

	bool GetLogin(CNetMsg::SP& msg);		// 버전 아이디 비번 검사

#ifdef INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS_SPN
	bool IPBlockSouthAmerica(const char *u_IP);
#endif // INTERNATIONAL_LOCAL_ACCESS_RESTRICTIONS_SPN

public:
	bool isRun()
	{
		return (service_ != NULL);
	}

	virtual void operate(rnSocketIOService* service);
	virtual void onClose(rnSocketIOService* service);

	//////////////////////////////////////////////////////////////////////////
	// 아래의 함수는 Connector server와 연결되는 세션에서만 사용됨
	void setConnectInfo(std::string connect_host, int connect_port);
	void Connect();
	virtual void onConnect(rnSocketIOService* service);
	virtual void onConnectFail(rnSocketIOService* service);
};

#endif
