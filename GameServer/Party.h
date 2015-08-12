#ifndef __PARTY_H__
#define __PARTY_H__

#define		MAX_PARTY_DIVITYPE		5		// 분배방법 수(경험치,아이템,스페셜 아이템)

class CNetMsg;
class CMonsterCombo;

#define PARTY_MATCHING_COMMENT			50		// 파티 매칭 파티 등록시 설명문구 길이
#define PARTY_MATCHING_DATA_PER_PAGE	10		// 파티 매칭 리스트에서 페이지당 표시할 데이터 수
#define PARTY_MATCHING_LIMIT_LEVEL		2		// 파티 매칭에서 레벨 제한 범위

class CPartyMatchMember
{
	int			m_nIndex;
	CLCString	m_strName;
	int			m_nLevel;
	int			m_nZone;
	char		m_nJob;
	char		m_nPartyType;

public:
	CPartyMatchMember(int nIndex, const char* strName, int nLevel, int nZone, char nJob, char nPartyType)
		: m_strName(MAX_CHAR_NAME_LENGTH + 1)
	{
		m_nIndex		= nIndex;
		m_strName		= strName;
		m_nLevel		= nLevel;
		m_nZone			= nZone;
		m_nJob			= nJob;
		m_nPartyType	= nPartyType;
	}

	int			GetIndex() const
	{
		return m_nIndex;
	}
	const char*	GetName() const
	{
		return (const char*)m_strName;
	}
	int			GetLevel() const
	{
		return m_nLevel;
	}
	int			GetZone() const
	{
		return m_nZone;
	}
	char		GetJob() const
	{
		return m_nJob;
	}
	char		GetPartyType() const
	{
		return m_nPartyType;
	}

	void		SetName(const char* strName)
	{
		m_strName = strName;
	}
	void		SetLevel(int nLevel)
	{
		m_nLevel = nLevel;
	}
	void		SetZone(int nZone)
	{
		m_nZone = nZone;
	}
};

class CPartyMatchParty
{
	int			m_nBossIndex;
	CLCString	m_strBossName;
	int			m_nBossLevel;
	int			m_nZone;
	int			m_nJobFlag;
	char		m_nPartyType;
	bool		m_bLimitLevel;
	CLCString	m_strComment;

	int			m_nMemberCnt;

public:
	CPartyMatchParty(int nBossIndex, const char* strBossName, int nBossLevel, int nZone, int nJobFlag, char nPartyType, bool bLimitLevel, const char* strComment)
		: m_strBossName(MAX_CHAR_NAME_LENGTH + 1)
		, m_strComment(PARTY_MATCHING_COMMENT + 1)
	{
		m_nBossIndex = nBossIndex;
		m_strBossName = strBossName;
		m_nBossLevel = nBossLevel;
		m_nZone = nZone;
		m_nJobFlag = nJobFlag;
		m_nPartyType = nPartyType;
		m_bLimitLevel = bLimitLevel;
		m_strComment = strComment;
	}

	int			GetBossIndex() const
	{
		return m_nBossIndex;
	}
	const char*	GetBossName() const
	{
		return (const char*)m_strBossName;
	}
	int			GetBossLevel() const
	{
		return m_nBossLevel;
	}
	int			GetZone() const
	{
		return m_nZone;
	}
	int			GetJobFlag() const
	{
		return m_nJobFlag;
	}
	char		GetPartyType() const
	{
		return m_nPartyType;
	}
	bool		IsLimitLevel() const
	{
		return m_bLimitLevel;
	}
	const char*	GetComment() const
	{
		return (const char*)m_strComment;
	}

	void		SetBossIndex(int nBossIndex)
	{
		m_nBossIndex = nBossIndex;
	}
	void		SetBossName(const char* strName)
	{
		m_strBossName = strName;
	}
	void		SetBossLevel(int nLevel)
	{
		m_nBossLevel = nLevel;
	}
	void		SetZone(int nZone)
	{
		m_nZone = nZone;
	}

	int			GetMemberCount() const
	{
		return m_nMemberCnt;
	}
	void		SetMemberCount( int nMemberCnt )
	{
		m_nMemberCnt = nMemberCnt;
	}
};

// 파티 멤버
class CPartyMember
{
	int				m_nCharIndex;
	CLCString		m_strCharName;

	CPC*			m_pChar;

public:
	CPartyMember(int nCharIndex, const char* strCharName, CPC* pc);
	~CPartyMember();

	int				m_nLevel;

	int GetCharIndex() const
	{
		return m_nCharIndex;
	}
	const char* GetCharName() const
	{
		return (const char*)m_strCharName;
	}

	CPC* GetMemberPCPtr() const;
	void SetMemberPCPtr(CPC* pc);
};

struct AftifactItemData
{
	CPC* pc;
	int item_index;

	AftifactItemData() : pc(NULL), item_index(-1) {}

	void _reset()
	{
		pc = NULL;
		item_index = -1;
	}

	void _insert(CPC* _pc, int _item_index)
	{
		pc = _pc;
		item_index = _item_index;
	}
};

class CParty
{
public:
	char			m_nPartyType[MAX_PARTY_DIVITYPE];			// 경험치 분배 타입	: 0 - 균등, 1 - 입수우선, 2 - new파티
	int				m_nCount;
	CPartyMember*	m_listMember[MAX_PARTY_MEMBER];
	int				m_nRequest;
	CLCString		m_strRequest;

	int				m_nEndPartyTime;							// 파티 해체 시간

	int				m_nRaidZoneNum;
	int				m_nRaidAreaNum;
	int				m_nSubNumber;								// 원정대 채널을 맴버 함수로 가지고 있는다. 해당 채널에서만 원정대 레이드 입장 가능

	typedef std::map<int/*guild_index*/, int/*exp*/> GUILD_SKILL_EXP;
	GUILD_SKILL_EXP	m_guild_skill_exp;

	AftifactItemData arti_data;

	void deletePartyRecallMember( int memberIndex );
	std::set<int>	m_warPartyRecall;

	CParty(char nPartyType, int nBossIndex, const char* strBossName, CPC* pBoss, int nRequest, const char* strRequest);
	~CParty();

	int GetRequestIndex() const
	{
		return m_nRequest;
	}
	const char* GetRequestName() const
	{
		return (const char*)m_strRequest;
	}
	int GetMemberCount() const
	{
		return m_nCount;
	}
	char GetPartyType(int nType) const
	{
		if(nType < 0 || nType >= MAX_PARTY_DIVITYPE) return -1;
		return m_nPartyType[nType];
	}
	int	GetMemberCountOnline();

	// boss 인덱스 반환
	int GetBossIndex() const
	{
		return (m_listMember[0]) ? m_listMember[0]->GetCharIndex() : -1;
	}

	// 해당 캐릭터 인덱스를 갖는 파티원의 리스트 인덱스를 구한다
	int FindMember(int nCharIndex);

	// 해당 리스트 인덱스의 멤버 Char index를 구한다.
	int GetMemberCharIndex(int nListIndex);

	void SetRequest(int nRequest, const char* strRequest);

	// request를 멤버로 참여
	int JoinRequest(const char* strRequestName, CPC* pRequest);

	// 해당 캐릭터를 제거, 제거하거나 없으면 true, 제거에 실패하면 false
	void DeleteMember(int nCharIndex);

	// list index로 CPartyMember 얻기
	CPartyMember* GetMemberByListIndex(int nListIndex) const;
	CPartyMember* GetMemberByCharIndex(int charIndex) const;

	bool ChangeBoss(const char* strNewBossName);

	void SetPartyType(int nType,char PartyType)
	{
		if(nType < 0 || nType >= MAX_PARTY_DIVITYPE) return;
		m_nPartyType[nType] = PartyType;
	}
	void SetPartyTypeAll(char cPartyType);

	// 파티 종료시 내부 포인터 NULL 처리 함수
	void SetEndParty();

	int GetNearPartyMemberCount(CCharacter* pCenter);

	// pPC 가까운 곳에 있는 nListIndex번째 파티원을 구한다
	CPC* GetNearMember(CCharacter* pPC, int nListIndex);

	// 해당 캐릭터인덱스의 멤버의 PC 포인터를 설정한다
	void SetMemberPCPtr(int nCharIndex, CPC* pPC);

	// 현재 PC가 설정되어 있는 모든 파티원에게 메시지를 전달, nExcludeCharIndex 제외
	void SendToAllPC(CNetMsg::SP& msg, int nExcludeCharIndex = -1);
#ifdef CONTINENT_PARTY_RECALL
	void SendToSameContinentPC(CNetMsg::SP& msg, int nExcludeCharIndex = -1);
#endif

	void SendToPartyRecallPC(CNetMsg::SP& msg, int nReqIndex);

	// 같은 존에 있는 PC에게만 메시지 전달
	void SendToPCInSameZone(int nZone, int nArea, CNetMsg::SP& msg);

	// 파티 해체 시간 설정
	void SetEndPartyTime(int nNow);

	// 파티 해체 시간 획득
	int  GetEndPartyTime();

	// 존이동을 하면 파티에서 제외시키는데 존이동 실패시 다시 파티로 복귀 시켜주는 함수
	bool SetMemberRegister_AfterGoZone(CPC* pPC);

	// 파티 리콜 사용 위치 설정
	void SetPartyRecallUseInfo(int zoneID, CPos& pos, int guildIndex, char cIsInCastle, int nExcludeCharIndex);

	// 게임서버 FRIEND 함수
	friend void ProcHelperPartyInfoParty(CNetMsg::SP& msg);

	int getGuildMemberCountInParty(CPC* pc);

	void calcGuildSkillExp(CPC* pc, int exp);
	int getGuildSkillExp(CPC* pc);
	void clearGuildSkillExp(CPC* pc);

	void updateCalcMember();

#ifdef GMTOOL
	bool		m_bGmtarget;
#endif // GMTOOL

	int m_comboAreaIndex;

#ifdef EXTREME_CUBE
	int	m_cubeUniqueIdx;
#endif // EXTREME_CUBE
};

#endif
//
