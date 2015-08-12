#ifndef __ASERVER_H__
#define __ASERVER_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"
#include "ConfigFile.h"
#include "Descriptor.h"
#include "MsgList.h"
#include "RankingSystem.h"
#include "../ShareLib/DBCmd.h"

struct CopyItem
{
	int a_index;
	int a_type_idx;
	int a_subtype_idx;
	int a_flag;
	int a_level;
	int a_job_flag;
	std::string a_name;
	int a_num0;
	int a_num1;
};

class CServerTimer : public rnSocketIOHandler
{
public:
	CServerTimer() {}
	~CServerTimer() {}

	void operate( rnSocketIOService* service );
	static CServerTimer* instance();
};

class CServerTimerPerMinute : public rnSocketIOHandler
{
public:
	CServerTimerPerMinute() {}
	~CServerTimerPerMinute() {}

	void Init();
	void operate( rnSocketIOService* service );
	static CServerTimerPerMinute* instance();
};

class CServer : public rnSocketIOHandler
{
public:
	CServer();
	~CServer();

	////////////
	// 소켓 관련
	CDescriptor* m_desclist;		// 연결된 소켓 리스트
	int			m_nDesc;			// 리스트의 소켓 개수

	////////////////////////////
	// 메시지 리스트
	CMsgList	m_msgList;
	int			m_seqGen;

	////////////
	// 서버 설정
	char*		m_serverpath;		// 모듈 경로
	CConfigFile	m_config;			// 서버 설정
	MYSQL		m_dbchar;
	int			m_serverno;

	int			m_maxplayers;		// 허용 가능 최대 플레이어 수
	bool		m_bshutdown;		// 서버 종료

	virtual void operate(rnSocketIOService* service);

	///////////////////
	// 메시지 번호 관련
	int GetNextSeqNo()
	{
		return m_seqGen++;
	}

	/////////////////
	// 서버 환경 관련
	char* GetServerPath();			// 경로 얻기
	bool LoadSettingFile();			// 설정 파일 읽기
	bool InitGame();				// 게임 초기화
	int GetMaxPlayers();			// 허용 가능 최대 플레이어 수 구하기

	bool ConnectDB();
	void DisconnectDB();
	bool LoadSettings();

	///////////////////////
	// 클라이언트 소켓 관련
	void SendToAllGameServer(CNetMsg::SP& msg);

	/////////////////
	// 명령 처리 관련
	void CommandInterpreter(CDescriptor* d, CNetMsg::SP& msg); // 명렁 처리

	void HeartBeat();

	CRankingSystemEX m_RankingSystem;

	MYSQL		m_dbData;
	std::list<int> m_tradeAgentBuyList;
	float		m_tradeAgentFee;			//거래대행 수수료
	int			m_tradeAgentDeposit;			//거래대행 보증금

	bool CopyToTradeAgentItemInfo();
};

#ifndef __SERVER_CPP__
extern CServer gserver;
#endif

//////////////////////////////////////////////////////////////////////////

class ClearMessageListTimer : public rnSocketIOHandler
{
public:
	ClearMessageListTimer() {}
	~ClearMessageListTimer() {}

	virtual void operate(rnSocketIOService* service);

	static ClearMessageListTimer* instance();
};

//////////////////////////////////////////////////////////////////////////
// 매 시
class CServerTimerPerHour : public rnSocketIOHandler
{
public:
	CServerTimerPerHour() {}
	~CServerTimerPerHour() {}

	void Init();
	virtual void operate(rnSocketIOService* service);

	static CServerTimerPerHour* instance();
};

#endif
//
