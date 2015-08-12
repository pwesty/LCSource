#ifndef __LOGIN_SERVER_H__
#define __LOGIN_SERVER_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"
#include "ConfigFile.h"
#include "Descriptor.h"
#include "../ShareLib/DBCmd.h"
#include "WaitPlayerList.h"

#if defined (INTERGRATION_SERVER)
#include "IntergrationServer.h"
#endif

typedef struct _tagAutoBlock
{
	char	ip[HOST_LENGTH + 1];
	char	id[MAX_ID_NAME_LENGTH + 1];

	struct _tagAutoBlock* prev;
	struct _tagAutoBlock* next;
} AUTO_BLOCK_INFO;

//////////////////////////////////////////////////////////////////////////
class CServerTimer : public rnSocketIOHandler
{
public:
	CServerTimer() {}
	~CServerTimer() {}

	virtual void operate(rnSocketIOService* service);

	static CServerTimer* instance();
};
//////////////////////////////////////////////////////////////////////////
class CServerTimerPerMinute : public rnSocketIOHandler
{
public:
	CServerTimerPerMinute() {}
	~CServerTimerPerMinute() {}

	void Init();
	virtual void operate(rnSocketIOService* service);

	static CServerTimerPerMinute* instance();
};
//////////////////////////////////////////////////////////////////////////

class CServer : public rnSocketIOHandler
{
public:
	CServer();
	~CServer();

	// 컨넥터 관련
	CDescriptor** m_connector;		// 컨넥터 서버
	int			m_nConnector;		// 컨넥터 서버 수

	////////////
	// 서버 설정
	char*		m_serverpath;		// 모듈 경로
	CConfigFile	m_config;			// 서버 설정
	int			m_subno;			// 로그인 서버 서브 번호
	int			m_clientversionMin;	// 클라이언트 버전
	int			m_clientversionMax;	// 클라이언트 버전
	bool		m_bOnlyLocal;		// 외부 접속 허용 여부
	unsigned int m_nInternalIPFilter;	// 내부 접속용 IP 필터
	unsigned int m_nInternalSubnetMask;	// 내부 접속용 서브넷 마스크
	int			m_national;			// 국가

	int			m_maxplayers;		// 허용 가능 최대 플레이어 수
	bool		m_bshutdown;			// 리부트

#ifdef IP_BLOCK_AUTO
	/////////////////
	// 자동 블럭 관련
	AUTO_BLOCK_INFO*	m_autoBlockTable;	// 블럭 테이블
	int					m_autoBlockPulse;	// 최근 리셋한 시간
#endif

	int					m_nRecommendServer;

	//////////
	// 틱 관련
	int			m_pulse;			// 틱 카운터

#if defined (LC_USA)
	int			m_pulseSaveTableCount;
#endif

	//////////
	// DB 연결
	MYSQL		m_dbuser;			// 사용자 데이터
	MYSQL		m_dbdata;			// 데이터
	MYSQL		m_dbAuth;			// 2판4판 인증 유저 DB

#ifdef LOCAL_LOGIN_LOG
	MYSQL		m_dbLocalLog;		// 로그인 DB로그
#endif //LOCAL_LOGIN_LOG

	bool		ConnectDB();
	void		DisconnectDB();

	/////////////////
	// 서버 환경 관련
	char* GetServerPath();			// 경로 얻기
	bool LoadSettingFile();			// 설정 파일 읽기
	bool InitGame();				// 게임 초기화
	void CheckInternalIP(const char* strData);			// 서버 내부오픈 IP 파싱

	////////////
	// 컨넥터 관련
	void ProcConnector(CNetMsg::SP& msg);				// 컨넥터 메시지 처리

	void ProcMessenger(CNetMsg::SP& msg);				// 메신저 메시지 처리

#if defined (LC_USA)
	void WriteDBPlayerCount();
	void BlockIPLog( const char* user_ip );
#endif

#ifdef PASSWORD_ENCRYPT_SHA256
	char m_salt[1024];							// salt string
	char m_hash[65];							// hash string
#endif // PASSWORD_ENCRYPT_SHA256

#if defined (INTERGRATION_SERVER)
public:
	CIntergrationServer m_intergrationInfo;
#endif
//#if defined(BCRYPT_USA) && !defined(CIRCLE_WINDOWS)
//	bool m_bBcrypted;
//#endif
private:
	bool m_bHttps_Auth;	// 가미고 https 인증을 할 것인지에 대한 변수. newStobm.bin 파일에 HttpsAuth=FALSE를 입력하면 인증 절차를 생략한다.(QA서버에 적용)
public:
	bool DoHttpsAuth()
	{
		return m_bHttps_Auth;
	}

public:
	virtual void operate(rnSocketIOService* service);
};

#ifndef __LOGIN_SERVER_CPP__
extern CServer gserver;
#endif

#endif
