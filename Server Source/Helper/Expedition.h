#ifndef __EXPEDITION_H__
#define __EXPEDITION_H__

#define MAX_EXPED_MEMBER			32		// 원정대 최대 인원수
#define MAX_EXPED_GMEMBER			8		// 원정대 1개 그룹 최대 인원수
#define MAX_EXPED_GROUP				4		// 원정대내 그룹 수
#define MAX_EXPED_DIVITYPE			5		// 분배방법 수

class CNetMsg;

// 원정대 멤버 클래스
class CExpedMember
{
	int				m_nCharIndex;			// 캐릭터 인덱스
	CLCString		m_strCharName;			// 캐릭터 이름
	int				m_nGroupType;			// 그룹 타입
	int				m_nMemberType;			// 멤버 타입
	int				m_nLabelType;			// 표식
	int				m_nListIndex;			// 그룹내 배열 인덱스
	int				m_nQuitType;			// 정상,비정상 구분

public:
	CExpedMember(int nCharIndex, const char* strCharName, int nGroupType, int nMemberType, int nListIndex, int nLevel );
	~CExpedMember();

	int				m_nLevel;			// On/Off Line

	int GetCharIndex() const
	{
		return m_nCharIndex;
	}
	const char* GetCharName() const
	{
		return (const char*)m_strCharName;
	}
	int GetGroupType() const
	{
		return m_nGroupType;
	}
	int GetMemberType() const
	{
		return m_nMemberType;
	}
	int GetLabelType() const
	{
		return m_nLabelType;
	}
	int GetQuitType() const
	{
		return m_nQuitType;
	}
	int GetListIndex() const
	{
		return m_nListIndex;
	}

	void SetGroupType(int nGroupType);
	void SetListIndex(int nListIndex);
	void SetMemberType(int nMemberType);
	void SetLabelType(int nLabelType);
	void SetQuitType(int nQuitType);
};

//원정대 클래스
class CExpedition
{
	char			m_nExpedType[MAX_EXPED_DIVITYPE];					// 분배 방법(경험치,일반 아이템,스폐셜 아이템)
	int				m_nTotCount;										// 원정대 인원수
	int				m_nGTotCount[MAX_EXPED_GROUP];						// 그룹 인원수
	CExpedMember*	m_listMember[MAX_EXPED_GROUP][MAX_EXPED_GMEMBER];	// 4개 그룹, 8명
	int				m_nRequestIndex;									// 요청자 캐릭터 인덱스
	CLCString		m_strRequestName;									// 현재 요청자 캐릭터 이름

	int				m_nSubNo;

public:

	int				m_nRaidZoneNum;
	int				m_nRaidAreaNum;
	int				m_nSubNumber;										// 원정대 채널을 맴버 함수로 가지고 있는다. 해당 채널에서만 원정대 레이드 입장 가능
	int				m_nDifficulty;

	CExpedition(int nSubNo, char nExpeType1, char nExpeType2, char nExpeType3, char nExpeType4, char nExpeType5, int nBossIndex, const char* strBossName, int nGroupType, int nMemberType, int nBossLevel );

	~CExpedition();

	int GetRequestIndex() const
	{
		return m_nRequestIndex;
	}
	const char* GetRequestName() const
	{
		return (const char*)m_strRequestName;
	}
	int GetMemberCount() const
	{
		return m_nTotCount;
	}
	char GetExpedType(int nType);

	int GetMemberCountOnline();

	// boss 인덱스 반환
	int GetBossIndex();
	const char* GetBossName();

	// 해당 캐릭터 인덱스를 갖는 파티원의 리스트 인덱스를 구한다
	int FindMemberListIndex(int nCharIndex);
	int FindMemberGroupIndex(int nCharIndex);

	// 해당 리스트 인덱스의 멤버 Char index를 구한다.
	int GetMemberCharIndex(int nGroup, int nListIndex);

	void SetRequest(int nRequest, const char* strRequest);

	// request를 멤버로 참여
	CExpedMember* JoinRequest(const char* strRequestName, int nMemberType, int nLevel );

	// 멤버 참여
	CExpedMember* Join(int nCharIndex, const char* strCharName, int nMemberType, int nLevel );

	// 해당 캐릭터를 제거, 제거하거나 없으면 true, 제거에 실패하면 false
	void DeleteMember(int nCharIndex);

	// char index로 CExpeditionMember 얻기
	CExpedMember* GetMemberByCharIndex(int nCharIndex) const;

	// list index로 CExpeditionMember 얻기
	CExpedMember* GetMemberByListIndex(int nGroup, int nListIndex) const;

	CExpedMember* GetMemberByListIndex(int nGroup, int nListIndex, int nMemberType) const;

	bool ChangeBoss(int nTargetIndex, int nOldBossIndex );

	void SetExpedType(int nType,char ExpedType)
	{
		if(nType < 0 || nType >= MAX_EXPED_DIVITYPE) return;
		m_nExpedType[nType] = ExpedType;
	}

	int GetGroupMemberCount(int nGroupType);
	int GetGroupBeEmptySlot(void);

	int GetGroupMembertypeCount(int nGroupType, int nMemberType);
	bool SetMBoss(int nNewMBossIndex);
	bool ResetMBoss(int nNewMBossIndex);

	bool MoveGroup(int nSourceGroup, int nMoveCharIndex, int nTargetGroup, int nTargetListindex);
	const CExpedMember* FindNextBoss(void);

	// 원정대 종료 함수
	void SetEndExped();

	int GetSubNo() const
	{
		return m_nSubNo;
	}
};

#endif
