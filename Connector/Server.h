#ifndef __ASERVER_H__
#define __ASERVER_H__

#include <map>
#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"
#include "ConfigFile.h"
#include "Descriptor.h"
#include "User.h"
#include "BillingClient.h"
#include "WaitPlayerList.h"

/*
typedef struct __tagLimitCtSell
{
	int			ct_id;
	int			sell;
} LIMIT_CT_CELL;
*/

class CLimitSell
{
	int m_ctid;
	int m_sell;
	bool m_bChange;

public:
	CLimitSell(int ctid, int sell, bool change = true)
	{
		m_ctid = ctid;
		m_sell = sell;
		m_bChange = change;
	}

	int GetIndex()
	{
		return m_ctid;
	}
	int GetSell()
	{
		return m_sell;
	}
	bool IsChange()
	{
		if( m_bChange )
		{
			m_bChange = false;
			return true;
		}
		return false;
	}
	void IncreaseSell()
	{
		m_sell++;
		m_bChange = true;
	}
	void moreSell(int sell)
	{
		m_sell = m_sell - sell;
		if( m_sell < 0 ) m_sell = 0;
	}

	static int CompByIndex(CLimitSell* p1, CLimitSell* p2)
	{
		return p1->GetIndex() - p2->GetIndex();
	}
};

class ServerAliveTimer : public rnSocketIOHandler
{
public:
	ServerAliveTimer() {}
	~ServerAliveTimer() {}

	virtual void operate(rnSocketIOService* service);
	static ServerAliveTimer* instance();
};

class HeartBeatTimer : public rnSocketIOHandler
{
public:
	HeartBeatTimer() {}
	~HeartBeatTimer() {}

	virtual void operate(rnSocketIOService* service);
	static HeartBeatTimer* instance();
};

class CServer : public rnSocketIOHandler
{
public:
	CServer();
	~CServer();

	////////////
	// 소켓 관련
	CDescriptor* m_desclist;		// 연결된 소켓 리스트

	////////////
	// 서버 설정
	int			m_hardcore_flag_in_connector;	// 가미고에서 사용하는 hardcore server, 0 - no, 1 - yes
	char*		m_serverpath;		// 모듈 경로
	CConfigFile	m_config;			// 서버 설정
	CBillingClient m_billing;		// 빌링 클라이언트
	int			m_serverno;			// 서버 번호
	int			m_maxSubServer;		// 하위 서버 최대 개수
	int			m_maxplayers;		// 허용 가능 최대 플레이어 수
	bool		m_bshutdown;		// 서버 정상 종료 여부

	typedef std::map<int, CLimitSell*> map_limitsell_t;
	map_limitsell_t m_limitSell;
	bool		m_changeLimit;		// 한정판매를 5분마다 디비에 저장할것인가.

	CWaitPlayerList	m_waitplayer;

	int			m_nEventGomdori2007LastUpdate;		// 마지막 업데이트 시간 : 단위 분
	int			m_nEventGomdori2007LastRead;		// 마지막 읽은 시간 : 단위 분
	int			m_nEventGomdori2007Count;			// 리스트 수
	int*		m_nEventGomdori2007Status;			// 데이터
	int			m_nEventGomdori2007StatusCurServer;	// 현재 서버군의 누적 수

	bool			m_bEventXmasTimerStart;				// 타이머 작동 여부
	int				m_nEventXmasTreeOldTime;			// 시간 차이를 구하기 위한 변수
	int				m_nEventXmasTreeNoticeTime;			// 트리 아이템 드랍 공지 시간 : 단위 분
	int				m_nEventXmasTreeLastUpdate;			// 마지막 업데이트 시간 : 단위 분
	int				m_nEventXmasTreeLastRead;			// 마지막 읽은 시간 : 단위 분
	unsigned int	m_unEventXmasTreePoint;				// 현재 서버의 나무 포인트

	/////////////////
	// 통계 주기 관련
	time_t		m_pulseSaveCount;	// t_connect_count 갱신 시간
	time_t		m_pulseSaveMax;		// t_connect_max 갱신 시간

	//////////
	// DB 연결
	MYSQL		m_dbuser;			// 사용자 데이터
	MYSQL		m_dbauth;			// 사용자 인증 데이터

	//////////////
	// User 리스트
	CUserList*	m_userList;			// 유저 리스트

	virtual void operate(rnSocketIOService* service);

	/////////////////
	// 서버 환경 관련
	char* GetServerPath();			// 경로 얻기
	bool LoadSettingFile();			// 설정 파일 읽기
	bool ConnectDB();				// DB 연결
	bool InitGame();				// 게임 초기화
	int GetMaxPlayers();			// 허용 가능 최대 플레이어 수 구하기

	void Close();										// 수행 종료
	void HeartBeat();									// 1초에 10번씩 매번 틱마다 수행되는 함수
	void SavePlayersCount();							// 플레이어 수를 데이터베이스에 저장

	void CloseSocket(CDescriptor* d);			// 디스크립터 종료
	void SendAllGameServer(CNetMsg::SP& msg);

	///////////////////////
	// 게임서버 와의 주기적 통신
	void TimerToGameServer();

	/////////////////
	// 명령 처리 관련
	void CommandInterpreter(CDescriptor* d, CNetMsg::SP& msg); // 명렁 처리

	void ProcessLogout(CUser* user);					// 로그아웃 처리
	void InitUserData();

#ifdef PASSWORD_ENCRYPT_SHA256
	char m_salt[1024];							// salt string
	char m_hash[65];							// hash string
#endif // PASSWORD_ENCRYPT_SHA256
//#if defined(BCRYPT_USA) && !defined(CIRCLE_WINDOWS)
//	bool m_bBcrypted;
//#endif
};

#ifndef __SERVER_CPP__
extern CServer gserver;
#endif

#endif
//
