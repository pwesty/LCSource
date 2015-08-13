#ifndef __ADESCRIPTOR_H__
#define __ADESCRIPTOR_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"
#include "../ShareLib/MemoryPoolBase.h"

#include "TimerItemManager.h"

#define AUTO_SAVE_SEQUENCT		(3)

class CDescriptor : public rnSocketIOHandler, public MemoryPoolBaseWithMutex<CDescriptor>
{
public:
	explicit CDescriptor(rnSocketIOService* service);

	~CDescriptor();

	////////////
	// 계정 관련
	time_t		m_connect_time;					// 세션이 연결된 시간

	LONGLONG	m_seq_index;
	bool		request_start_game_flag_;		// 클라리언트가 START_GAME 패킷을 보내
	int			m_index;						// 유저 인덱스
	CLCString	m_idname;						// 유저 아이디
	CLCString	m_passwd;						// 암호
	CLCString	m_ident;						// 주민번호 뒷자리
//	bool		m_bNovice;						//64시간 이내 초보자.//0627
	int			m_userFlag;
//0704
	int			m_notice[MAX_NOTICE];			//이벤트 공지 (최대 5개)

	bool		m_bCreate90Lv;					// 90Lv 이상 -> 100Lv 로 수정
	bool		m_bNotCreateNightShadow;			// NS 생성 여부
	bool		m_bIsNSCreateCardUse;

	CLCString		m_proSite;

#if defined(EVENT_PCBANG_2ND)
	int			m_location;
#endif

	////////////
	// 소켓 관련
	unsigned int m_nPrepareSeed;			// 처음 키를 생성하기 위한 Seed

	//////////
	// 핵 관련
	int			m_hackCheckPulse;			// 최근 핵 검사 수행한 타임
	int			m_hackServerPulse;			// 서버 펄스		: 최초 캐릭터 선택에서 설정되고, 이후 한시간에 한번 갱신
	int			m_hackClientPulse;			// 클라이언트 펄스	:
	int			m_hackLastClientDiff;		// 최근 클라이언트의 펄스 간격
	int			m_hackCount;				// 핵 시도 회수

	//////////
	// 틱 관련
	int			m_idletics;					// 입력없이 지나간 틱 수
	int			m_checktics;				// 1초당 명령 입력된 수를 리셋하기 위해 사용됨
	short int	m_commandcount;				// 1초당 명령 입력된 개수

	////////////
	// 상태 관련
	int			m_connected;				// CON_ 상수 값을 저장 : 접속 상태
	int			m_logined;					// 로그인 결과
	MSG_LOGIN_TYPE m_playmode;				// login mode or play mode
	bool		m_bclosed;
	bool		m_bGoZone;					// 존 이동시에 true로 설정
	bool		m_bGoZoneSent;				// 존 이동 메시지 전달 여부
	CNetMsg::SP		m_msgGoZone;				// 존 이동 메시지, m_bGoZoneSent를 false로 설정할 때 설정

	////////////
	// 링크 관련
	CPC*		m_pChar;					// linked to char
	CDescriptor* m_pPrev;					// 리스트 링크
	CDescriptor* m_pNext;

	//////////////
	// 이벤트 관련
#ifdef HANARO_EVENT
	CLCString m_hanaroCode;
#endif

	void WriteToOutput(CNetMsg::SP& msg);	// 쓰기

	bool GetLogin(CNetMsg::SP& msg);			// 버전 아이디 비번 검사

	bool StartGame(CNetMsg::SP& msg);		// 게임 시작
	bool CharNew(CNetMsg::SP& msg);			// 캐릭터 생성
	bool CharDel(CNetMsg::SP& msg);			// 캐릭터 삭제

	//////////
	// 핵 관련
	void SetHackCheckPulse();			// 핵 검사 펄스 값을 현재 펄스 값으로 설정
	bool CheckHackPulse(int clientPulse);	// 클라이언트의 펄스 값으로 핵펄스 검사
	bool IncreaseHackCount(int n);			// 핵 카운트 증가, 잦은 핵 시도시 접속 종료 처리

private:
	time_t m_tExtendCharacterSlotTime;
public:
	time_t getExtendCharacterSlotTime() const;
	void setExtendCharacterSlotTime(const time_t _time);

public:
	rnSocketIOService* service_;

	const char* getHostString()
	{
		return service_->ip().c_str();
	}
	void Close(std::string errmsg);
	virtual void operate(rnSocketIOService* service);
	virtual void onClose(rnSocketIOService* service);

public:
	void sendData_StartAndMoveZone();
	void returnToCharacterSelectMode();
	void sendStartData();					//접속시 최초 한번만 전달하는 데이터

#ifdef HARDCORE_SERVER
	void Make_1_Level();
#endif

	void sendSyndicateOutNeedNasInfo();

	// 자동 저장시 사용하는 변수
public:
	unsigned int m_autoSaveSeq;

public:
	static unsigned int static_auto_save_sequence;
	static std::set<CDescriptor*> auto_set[AUTO_SAVE_SEQUENCT];

	static void makeAutoSaveSequence(CDescriptor* desc);
};

#endif
