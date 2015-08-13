#ifndef __ASERVER_H__
#define __ASERVER_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"
#include "ConfigFile.h"
#include "Descriptor.h"
#include "MsgList.h"

#define FILENAME_RECOMMEND		".recommend"

#define FILENAME_GOLDENBALL		".goldenball"

typedef struct _tagGoldenBallData
{
	int		nStatus;
	char	strTeam1[GOLDENBALL_TEAM_NAME_LENGTH + 1];
	char	strTeam2[GOLDENBALL_TEAM_NAME_LENGTH + 1];
	int		nTeam1Score;
	int		nTeam2Score;
	int		nYear;
	int		nMonth;
	int		nDay;
	int		nHour;
	int		nMin;
	time_t	timeEndVote;
} GOLDEN_BALL_DATA;

class CServer : public rnSocketIOHandler
{
public:
	CServer();
	~CServer();

	////////////
	// 소켓 관련
	CDescriptor* m_desclist;		// 연결된 소켓 리스트

	////////////////////////////
	// 메시지 리스트
	CMsgList	m_msgList;
	int			m_seqGen;

	////////////
	// 서버 설정
	char*		m_serverpath;		// 모듈 경로
	CConfigFile	m_config;			// 서버 설정

	int			m_maxplayers;		// 허용 가능 최대 플레이어 수
	bool		m_nameserverisslow; // 왜 쓰는 것일까?

	int			m_nRecommendServer;	// 서버 추천 설정

	GOLDEN_BALL_DATA	m_sGoldenBallData;	// 골든볼 데이터

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

	/////////////////
	// 명령 처리 관련
	virtual void operate(rnSocketIOService* service);
	void CommandInterpreter(CDescriptor* d, CNetMsg::SP& msg); // 명렁 처리
	void ProcTimeout(CMsgListNode* msgNode);
};

#ifndef __SEVER_CPP__
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
