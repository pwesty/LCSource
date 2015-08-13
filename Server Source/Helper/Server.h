#ifndef __ASERVER_H__
#define __ASERVER_H__

#include <map>
#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"

#define MAKE_LONGLONG_KEY(x, y)			(((LONGLONG)(x) << 32) | (y))

#ifdef DEV_GUILD_STASH
//#include <map>
#endif // DEV_GUILD_STASH
#include "Party.h"
#include "ConfigFile.h"
#include "Descriptor.h"
#include "MsgList.h"
#include "Guild.h"
#include "Expedition.h"
#include "Syndicate.h"
#include "../ShareLib/DBCmd.h"

#ifdef DEV_GUILD_STASH
#include "GuildStash.h"
#endif // DEV_GUILD_STASH

class timerPerSecond : public rnSocketIOHandler
{
public:
	timerPerSecond() {}
	~timerPerSecond() {}

	virtual void operate(rnSocketIOService* service);
	static timerPerSecond* instance();
};

class timerPerMinute : public rnSocketIOHandler
{
public:
	timerPerMinute() {}
	~timerPerMinute() {}

	void Init();
	virtual void operate(rnSocketIOService* service);
	static timerPerMinute* instance();
};

class timerPerHour : public rnSocketIOHandler
{
public:
	timerPerHour() {}
	~timerPerHour() {}

	void Init();
	virtual void operate(rnSocketIOService* service);
	static timerPerHour* instance();
};

typedef std::map<LONGLONG, CPartyMatchMember*> map_partymatchmember_t;
typedef std::map<LONGLONG, CPartyMatchParty*> map_partymatchparty_t;
typedef std::map<LONGLONG, CExpedition*> map_expedition_t;
typedef std::map<LONGLONG, CParty*> map_party_t;

class timerPerMidnight : public rnSocketIOHandler
{
public:
	timerPerMidnight() {}
	~timerPerMidnight() {}

	void Init();
	virtual void operate(rnSocketIOService* service);
	static timerPerMidnight* instance();
};

class CServer : public rnSocketIOHandler
{
public:
	CServer();
	~CServer();

#ifdef PREMIUM_CHAR
	int			m_premiumchar_reset_jump_count_time;		// unix time
#endif

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

	unsigned int m_pulse;			// 틱 카운터

	CGuildList	m_guildlist;

	int			m_nMoonStoneNas;
	int			m_nGuildMarkTable[3];
	///////////////////
	// 전체 파티 리스트
	map_party_t					m_listParty;	// 서버내 전체 파티 리스트

	// 전체 원정대 리스트
	map_expedition_t			m_listExped;	// 서버내 전체 원정대 리스트

	map_partymatchmember_t		m_listPartyMatchMember;
	map_partymatchparty_t		m_listPartyMatchParty;

#ifdef DEV_GUILD_STASH
	std::map<int,CGuildStash*>		m_guildstash;
#endif // DEV_GUILD_STASH

	Syndicate					m_syndicate;

	//헬퍼 테오스 무덤 관리 변수
	int							m_castle_env_rate;
	int							m_castle_mob_rate;
	int							m_castle_tax_rate;
	int							m_castle_hunt_rate;

	///////////////////
	// 메시지 번호 관련
	int GetNextSeqNo()
	{
		return m_seqGen++;
	}

	virtual void operate(rnSocketIOService* service);

	/////////////////
	// 서버 환경 관련
	char* GetServerPath();			// 경로 얻기
	bool LoadSettingFile();			// 설정 파일 읽기
	bool InitGame();				// 게임 초기화
	void initTime();				// Server::Run()에서 초기화 하던 부분
	int GetMaxPlayers();			// 허용 가능 최대 플레이어 수 구하기
	void SetServerTime();			// 서버 시간 설정 -> serverTime

	/////////////////
	// 소켓 설정 관련
	bool ConnectDB();
	void DisconnectDB();
	bool LoadSettings();

	void SendToAllGameServer(CNetMsg* msg)
	{
		SendToAllGameServer(CNetMsg::SP(msg));
	}
	void SendToAllGameServer(CNetMsg::SP msg);
	void SendToAllSubServer(CNetMsg* msg, int nSubNo)
	{
		SendToAllSubServer(CNetMsg::SP(msg), nSubNo);
	}
	void SendToAllSubServer(CNetMsg::SP msg, int nSubNo);

	/////////////////
	// 명령 처리 관련
	void CommandInterpreter(CDescriptor* d, CNetMsg::SP& msg); // 명렁 처리

	void HeartBeat();

	bool DeleteGuild(int guildindex);
	bool DeleteGuildMember(int guildindex, const char* guildname, int charindex, const char* charname, bool bKick);

	// 파티 리스트 노드 찾기 : 보스 인덱스 사용
	CParty* FindPartyByBossIndex(int nSubNo, int nBossIndex);

	// 파티 리스트 노드 찾기 : 멤버 인덱스 사용
	CParty* FindPartyByMemberIndex(int nSubNo, int nMemberIndex, bool bIncludeRequest);

	// 파티 리스트 노드 찾기 : 보스 인덱스 사용
	CExpedition* FindExpedByBossIndex(int nSubNo, int nBossIndex);

	// 파티 리스트 노드 찾기 : 멤버 인덱스 사용
	CExpedition* FindExpedByMemberIndex(int nSubNo, int nMemberIndex, bool bIncludeRequest);

	CPartyMatchMember* FindPartyMatchMemberByCharIndex(int nSubNo, int nCharIndex);
	CPartyMatchParty* FindPartyMatchPartyByBossIndex(int nSubNo, int nBossIndex);

	void SendGuildMemberList(CGuild* guild, CDescriptor* desc);
	void SendExtendGuildMemberList(CGuild* guild, CDescriptor* desc);

#ifdef PARTY_BUG_GER
	void PrintPartyMemberList(int nSubNo, int nFindCharIdx);
#endif // PARTY_BUG_GER

#ifdef EXTREME_CUBE_VER2
	int m_CubePointUpdateTime;
	void CubePointRanking(int RankingTime);
	void CheckCubeReward();
	int SetCurCubePointUpdateTime();
#endif // EXTREME_CUBE_VER2
#ifdef DEV_GUILD_MARK
#endif // DEV_GUILD_MARK
	void checkGuildProcess(time_t now);			// 전체 길드를 돌면서 체크해야 하는 것들을 모아둔 함수 : 루프 한번돌면서 다 체크하도록
	void checkGuildBattle(time_t, CGuild* p);	// 길드전 관련 체크
#ifdef DEV_GUILD_MARK
	void checkGuildMarkTime(time_t now, CGuild* p);			// 길드 마크 관련 체크
#endif // DEV_GUILD_MARK
	void checkGuildMasterAutoKick(time_t now, CGuild* p);	// 길드 마스터 자동 추방 관련 체크

	void saveCastleDungeonInfo();
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

#endif
//
