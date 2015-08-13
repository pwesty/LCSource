#ifndef __WAR_CASTLE_H__
#define __WAR_CASTLE_H__

#define WAR_CASTLE_SUBNUMBER_DRATAN WAR_CASTLE_SUBNUMBER_MERAC

#define CHECK_SUBSERVER(castle)		if (!castle->CheckSubServer()) return

// 공성 상태
#define WCSF_NORMAL			0		// 일반
#define WCSF_WAR_FIELD		1		// 야전
#define WCSF_WAR_CASTLE		2		// 공성

// 공성 참여 상태
#define WCJF_NONE			0		// 비참여
#define WCJF_OWNER			1		// 성주 길드
#define WCJF_DEFENSE_GUILD	2		// 수성 길드
#define WCJF_DEFENSE_CHAR	3		// 수성측 용병
#define WCJF_ATTACK_GUILD	4		// 공성 길드
#define WCJF_ATTACK_CHAR	5		// 공성측 용병

#define IS_DEFENSE_TEAM(joinflag)		( (joinflag == WCJF_OWNER || joinflag == WCJF_DEFENSE_GUILD || joinflag == WCJF_DEFENSE_CHAR) ? true : false )
#define IS_ATTACK_TEAM(joinflag)		( (joinflag == WCJF_ATTACK_GUILD || joinflag == WCJF_ATTACK_CHAR) ? true : false )

// 야전에서 상대방을 물리칠 때 증가되는 포인트
#define WCFP_CASTLE_GUARD	5		// 수호병 잡으면 증가되는 포인트
#define WCFP_PLAYER			1		// 플레이어 잡으면 증가되는 포인트

// 공성 필드 전투 시간 : 초단위
#define WCT_FIELD			(15 * 60)		// 필드전 진행 시간 : 초
#define WCT_WAR				(60 * 60)		// 필드전 포함 전체 공성 시간 : 초

#define	APPLY_GUILD_GRADE_SKILL_TIME 10	// 길드 직위 편성 버프 시간 (초)

// 공성 가능 길드 조건
#define WCGF_ATTACK_LEVEL	5
#define WCGF_DEFENSE_LEVEL	4
#define WCGF_CHAR_LEVEL		15
#define WCGF_ITEM			759
#define WCGF_MEMBERCOUNT	20
#define WCGF_MONEY			100000

// 공성 신청인 클래스
class CWarCastleJoin
{
	friend class CWarCastle;

	int					m_index;
	char				m_bAttack;
	int					m_point;
	CWarCastleJoin*		m_prev;
	CWarCastleJoin*		m_next;

//	CWarCastleJoin(int index, char bAttack) { m_index = index; m_bAttack = bAttack; m_prev = m_next = NULL; m_point = 0; }

	void IncreasePoint(int val)
	{
		m_point += val;
	}

public:
	CWarCastleJoin(int index, char bAttack)
	{
		m_index = index;
		m_bAttack = bAttack;
		m_prev = m_next = NULL;
		m_point = 0;
	}

	int GetIndex()
	{
		return m_index;
	}
	char IsAttack()
	{
		return m_bAttack;
	}
	int GetPoint()
	{
		return m_point;
	}
	CWarCastleJoin* GetNext()
	{
		return m_next;
	}
	void SetPoint(int p)
	{
		m_point = p;
	}
	void SetPrev(CWarCastleJoin * n)
	{
		m_prev = n;
	}
	void SetNext(CWarCastleJoin * n)
	{
		m_next = n;
	}
};

// 공성 클래스
class CWarCastle
{
	// 공성 상태 반영을 위해 추가
	friend void ProcHelperWarNoticeStart(CNetMsg::SP& msg);
	friend void ProcHelperWarNoticeStartAttackCastle(CNetMsg::SP& msg);
	friend void ProcHelperWarNoticeEnd(CNetMsg::SP& msg);

protected:
	int					m_state;							// 공성 상태
	CNPC*				m_castleNPC;						// 공성 신청 NPC
	CNPC*				m_lordSymbol;						// 권좌
	bool				m_bRegenShop;						// 상점 리젠 여부
	int					m_lastWarTime;						// 마지막 공성 시간
	int					m_nextWarTime;						// 다음 공성 시간
	int					m_ownerGuildIndex;					// 성주 길드 인덱스
	CLCString			m_ownerGuildName;					// 성주 길드 이름
	int					m_ownerCharIndex;					// 성주 캐릭터 인덱스
	CLCString			m_ownerCharName;					// 성주 캐릭터 이름
	bool				m_bNoticeWarTime;					// 공성 시간을 알려줬는지 여부
	int					m_noticeRemain;						// 공성 시작까지 남은 시간 알림 메시지 플래그
	int					m_defensePoint;						// 수성측 포인트
	int					m_gateState;						// 성문 정보
	bool				m_bGiveLordWeapon;					// 성주 칼 지급여부
	bool				m_bMoveToRegen;						// 10분전 강제 이동 여부
	bool				m_bWarCastleReady;					// 공성전 시작 1분전 준비상태

	int					m_countAttackGuild;					// 공성 길드 수
	CWarCastleJoin*		m_defenseGuildList;					// 수성 길드 리스트
	CWarCastleJoin*		m_attackGuildList;					// 공성 길드 리스트
	CWarCastleJoin*		m_defenseCharList;					// 수성측 용병 리스트
	CWarCastleJoin*		m_attackCharList;					// 공성측 용병 리스트

	CWarCastleJoin*		m_top3guild[3];						// 필드 전투 최상위 3개 길드
	int*				m_posRegenPoint;					// 해당 리젠 지역에 할당된 공성 길드 인덱스
	int					m_nRegenPoint;						// posRegenPoint의 개수

	char				m_nSendInterval;					// 메세지 보내는 카운트 : Hertbeat 로 1 초씩 카운트

	int					m_oldOwnerCharIndex;				// 공성 시작하면 바로 수성 성중의 인덱스 저장한다.

	int					m_guildGradeSkillTime;

public:

	CWarCastle();
	virtual ~CWarCastle();

	////////////////////
	// 공성 정보
	// 해당 존 공성 정보를 읽음
	void SetNoticeWarTime(bool value)
	{
		m_bNoticeWarTime = value;
	}
	void SetNoticeRemain(int value)
	{
		m_noticeRemain = value;
	}

	virtual bool CheckSubServer();

	// 상속 받은 클래스에서 오버라이딩 할 경우에도 반드시 호출 필요함
	virtual bool LoadCastleData();

	// 상태 반환
	int GetState()
	{
		return m_state;
	}

	// 공성 상태 변경
	void CheckWarCastle(bool forced_end_merac);

	// 해당 지역의 리젠 장소를 반환
	// ch는 NULL 일수 있음
	virtual int GetRegenPoint(int joinflag, CPC* ch) = 0;

	// 해당 존 번호를 반환
	virtual int GetZoneIndex() = 0;

	// 성문 상태 조사
	int GetGateState(int mask = 0xffffffff)
	{
		return m_gateState & mask;
	}

	// 수호병 사망시 성문 열림 설정
	virtual void UpdateGateState(CNPC* npc) = 0;

	// 권좌 반환
	CNPC* GetLordSymbol()
	{
		return m_lordSymbol;
	}

	// 열려있는 성문이 있는지 검사
	virtual bool IsOpenGate() = 0;

	// 권좌 HP 감소
	int DecreaseLordSymbolHP();

	// 리젠영역안에 있는지 검사
	bool IsInRegenPoint(CCharacter* ch, bool bAttack);

	// 성안을 구분하는 사각형 좌표 구하기
	virtual void GetInnerCastleRect(char* nYlayer, int* nX0x2, int* nZ0x2, int* nX1x2, int* nZ1x2) = 0;

	// 성안에 있는지 검사
	bool IsInInnerCastle(CCharacter* pChar);

	// 수성 워프 지역인지 검사
	virtual bool IsDefenseWarpPoint(int pos) = 0;

	// 수호탑 개체수
	virtual int GetGateNPCCount() = 0;

	// 수호탑 NPC 반환
	virtual CNPC* GetGateNPC(int idx) = 0;

	// 상점 리젠 여부
	bool IsRegenShop()
	{
		return m_bRegenShop;
	}

	////////////////////
	// 공성 시간 관련

	// 공성 시작 시간 설정
	virtual void SetNextWarTime(int nextWarTime) = 0;
	virtual bool SetNextWarTime(int wday, int hour) = 0;

	// 공성 시작 시간 얻기
	void GetNextWarTime(struct tm* nextWarTime, bool bHumanable);
	int GetNextWarTime()
	{
		return m_nextWarTime;
	}

	// 공성/수성 용병/길드 신청 가능 시간 검사
	virtual bool IsJoinTime() = 0;

	// 다음 공성 신청 가능 시간 구하기
	virtual void GetJoinTime(struct tm* joinTime, bool bHumanable) = 0;

	// 길드전 종료까지 남은 시간 구하기
	int GetRemainWarTime();

	////////////////////
	// 참여 유저 관련

	// 성주 정보 구하기
	int GetOwnerGuildIndex()
	{
		return m_ownerGuildIndex;
	}
	const char* GetOwnerGuildName()
	{
		return (const char*)m_ownerGuildName;
	}
	int GetOwnerCharIndex()
	{
		return m_ownerCharIndex;
	}
	const char* GetOwnerCharName()
	{
		return (const char*)m_ownerCharName;
	}

	// 수성 용병 추가
	bool AddDefenseChar(int index);

	// 수성 길드 추가
	bool AddDefenseGuild(int index);

	// 공성 용병 추가
	bool AddAttackChar(int index);

	// 공성 길드 추가
	bool AddAttackGuild(int index);

	// 수성 용병 제거
	void RemoveDefenseChar(int index);

	// 수성 길드 제거
	void RemoveDefenseGuild(int index);

	// 공성 용병 제거
	void RemoveAttackChar(int index);

	// 공성 길드 제거
	void RemoveAttackGuild(int index);

	// 공성 길드인지 검사
	bool IsAttackGuild(int index);

	// 공성 용병인지 검사
	bool IsAttackChar(int index);

	// 수성 길드인지 검사
	bool IsDefenseGuild(int index);

	// 수성 용병인지 검사
	bool IsDefenseChar(int index);

	// 공성측 길드 수
	int GetCountAttackGuild()
	{
		return m_countAttackGuild;
	}

	// 최상위 공성길드 정보 얻기
	void GetTop3AttackGuild(int guildindex[3], char guildname[3][MAX_GUILD_NAME_LENGTH + 1], int guildpoint[3]);

	// Top3 길드중 하나인지 검사
	bool IsTop3Guild(int guildindex);

	bool IsWarCastleReady()
	{
		return m_bWarCastleReady;
	}

	////////////////////
	// 포인트 관련

	// 팀 점수를 얻기
	// guildindex가 0이면 수성측, 그외는 공성 길드
	int GetTeamPoint(int guildindex);

	// 팀 점수 증가
	// guildindex가 0이면 수성측, 그외는 공성 길드
	// 최종 값을 반환
	int IncreaseTeamPoint(int guildindex, int val);

	// 특정 길드 포인트 구하기
	// 야전에서는 킬포인트
	// 공성에서는 권좌 포인트
	// 길드 정보가 없거나, 수성측 권좌 포인트 요청등 잘못된 요청에서는 -1
	int GetGuildPoint(int guildindex);

	///////////////////
	// 성주 아이템 정보

	// 성주 아이템 지급
	virtual bool GiveLordItem(CPC* pc) = 0;

	// 성주 아이템 회수
	virtual bool TakeLordItem(CPC* pc) = 0;

	// 성주 아이템 인덱스 구하기
	virtual int GetLordItemIndex(char job1, char job2) = 0;

	// 공성 지역 중 수성 지역에서 수성 제외 유저 몰아내기
	void GetOutNonDefense();

	void SendCastleState( int zoneindex );	// 게이트 상태 전송

	void GuildGradeSkillAll( bool bApply);
	void ApplyGuildGradeSkillAll( )
	{
		GuildGradeSkillAll( true );
	}
	void CureGuildGradeSkillAll( )
	{
		GuildGradeSkillAll( false );
	}

#ifdef WARCASTLE_STATE_CHANNEL_CYNC
	void SetStateChannelCync( int state )
	{
		SetState( state);
	}
#endif

protected:

	////////////////////
	// 진행 상태 관련

	// 공성 시작 알림
	void CheckWarCastleNotice();

	// 야전 시작
	void CheckWarCastleStartWar();

	// 공성 시작
	void CheckWarCastleStartWarCastle();

	// 공성 끝 처리
	void CheckWarCastleEnd(bool forced_end_merac);

	// 공성 상태 변경
	void SetState(int state)
	{
		m_state = state;
	}

	////////////////////
	// 지역 정보 관련

	// 공성이 이루어 지는 Area를 가져온다
	CArea* GetArea();

	// 공성 지역으로 강제 리젠
	void MoveToRegenPoint();

	// 리젠 위치 초기화
	virtual void InitRegenPosition() = 0;

	////////////////////
	// MPC 관련

	// 공성 NPC를 리젠 시킨다
	void RegenCastleNPC();

	// 근위병 리젠
	void RegenGuardNPC();

	// 수호탑 리젠
	void RegenCastleTower();

	// 권좌 생성
	void RegenLordSymbol();

	// 워프게이트 생성
	void RegenWarpNPC();

	// 상인 생성/제거
	void RegenShop();

	// 공성 NPC를 제거한다
	void RemoveCastleNPC();

	// 수호병을 제거한다
	void RemoveGuardNPC();

	// 수호탑 제거
	void RemoveCastleTower();

	// 권좌 제거
	void RemoveLordSymbol();

	// 워프게이트 제거
	void RemoveWarpNPC();

	// 수호탑을 성문지기로 설정
	virtual void SetGateNPC(CNPC* npc) = 0;

	// 성문지기 초기화
	virtual void InitGateNPC() = 0;

	// m_state에 따라 공성 NPC를 리젠하거나 없앤다
	void CheckWarCastleNPC();

	////////////////////
	// 참여자 관련

	// 필드 전투 최상위 3개 길드 구하기
	void GetTop3AttackGuild();

	// 참여 정보 찾기
	CWarCastleJoin* FindAttackGuild(int index);
	CWarCastleJoin* FindAttackChar(int index);
	CWarCastleJoin* FindDefenseGuild(int index);
	CWarCastleJoin* FindDefenseChar(int index);

	// 참여자 정보 초기화
	void ResetJoinFlag();

	// 참여 리스트 초기화
	void RemoveAllJoinList();

	// 성 소유자 정보 초기화
	void ResetOwner();

public:

	// 공성 참여자 검사, WCJF_XXX 반환
	int CheckJoin(CPC* ch);

	// 성 소유자 설정
	void SetOwner(CGuild* guild);

	////////////////////
	// static 함수들

	// 해당 존의 공성 정보를 반환
	static CWarCastle* GetCastleObject(int zoneindex);

	// 대상이 공성 참여자인지 검사
	static void CheckJoinAll(CPC* ch);

	// 소유 성 반환
	// 호출한 곳에서 delete [] 를 해야함
	// 소유 성이 없으면 NULL 반환
	// 소유 성이 있으면 count에 성 개수 저장
	static int* GetOwnCastle(int guildindex, int* count);

	// 길드 삭제시 해당 정보를 공성 정보에 반영
	static void DeleteGuild(int guildindex);

	// 해당 PC가 서버 공지를 사용할 수 있는지 여부
	static bool CanLordChat(CPC* pc);

	// 현재 서브서버의 공성지역 인덱스 얻기, 없으면 -1
	static int GetCurSubServerCastleZoneIndex();
	void SetOldOwnerChar(int ownerIndex)
	{
		m_oldOwnerCharIndex = ownerIndex;
	}
	int  GetOldOwnerChar()
	{
		return m_oldOwnerCharIndex;
	}

	virtual int GetWarTime() = 0;

	int		GetGuildGradeSkillTime()
	{
		return m_guildGradeSkillTime;
	}
	void	SetGuildGradeSkillTime(int currenttime)
	{
		m_guildGradeSkillTime = currenttime;
	}

	void endWarCastleMerac()
	{
		CheckWarCastleEnd(true);
	}

	void EndWarRegenPoint();
};

// 메라크 공성 클래스
class CMeracCastle : public CWarCastle
{
	enum { COUNT_MERAC_GATE_NPC = 6 };

	CNPC*		m_gateNPC[COUNT_MERAC_GATE_NPC];		// 성문 지키는 수호상

public:
	CMeracCastle();
#ifdef WARCASTLE_SUBNUMBER_MEMBER_VALUE
	int					m_subNumber;						// 공성전을 하는 채널 넘버
#endif // WARCASTLE_SUBNUMBER_MEMBER_VALUE

// virtual function 구현
protected:
	void SetGateNPC(CNPC* npc);
	void InitGateNPC();
	void InitRegenPosition();

public:
	int GetZoneIndex()
	{
		return ZONE_MERAC;
	}
	void SetNextWarTime(int nextWarTime);
	bool SetNextWarTime(int wday, int hour);
	bool IsJoinTime();
	void GetJoinTime(struct tm* nextJoinTime, bool bHumanable);
	int GetRegenPoint(int joinflag, CPC* ch);
	void UpdateGateState(CNPC* npc);
	bool IsOpenGate();
	bool GiveLordItem(CPC* pc);
	bool TakeLordItem(CPC* pc);
	int GetLordItemIndex(char job1, char job2);
	bool IsDefenseWarpPoint(int pos);
	int GetGateNPCCount()
	{
		return (GetGateNPC(0) == NULL) ? 0 : COUNT_MERAC_GATE_NPC;
	}
	CNPC* GetGateNPC(int idx)
	{
		return (idx < 0 || idx >= COUNT_MERAC_GATE_NPC) ? NULL : m_gateNPC[idx];
	}
	void GetInnerCastleRect(char* nYlayer, int* nX0x2, int* nZ0x2, int* nX1x2, int* nZ1x2);
// --- virtual function 구현
	int GetWarTime()
	{
		return WCT_WAR;
	}
};

void CheckWarCastle();
bool LoadCastleData();
bool IsValidGuildCommandWithWarCastle(CPC* ch, unsigned char subtype);

extern CMeracCastle meracCastle;

#endif // __WAR_CASTLE_H__
//
