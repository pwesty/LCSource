#ifndef __COMMUNITY_DEFINE__
#define __COMMUNITY_DEFINE__

#define	PARTY_MAX_MEMBER			7
#define EXPEDITION_GROUP_MAX		4
#define EXPEDITION_MEMBER_PER_GROUP 8

// Party type
enum PartyType
{
	PT_PEACEEVER = 0,	// 균등 분배
	PT_SURVIVAL = 1,	// 입수우선 
	PT_ATTACK = 2,		// 공격자경험치 우선
};

// 파티 인포
struct stPartyInfo
{
	SLONG				slIndice;			// Indices of members
	SLONG				slZone;				// Zone of members
	CTString			strName;			// Strings of member's name
	BOOL				bLeader;			// If this member is a leader or not
	SBYTE				sbJob;				// Jobs of members
	int					nLevel;				// Levels of members
	int					nHP;				// HPs of members
	int					nMaxHP;				// Maximum HPs of members
	int					nMP;				// MPs of members
	int					nMaxMP;				// Maximum MPs of members
	FLOAT				fPosX;				// Position x of members
	FLOAT				fPosZ;				// Position z of members
	SBYTE				sbLayer;			// Layer of members
	UBYTE				ubBuff;				// If buff is shown or not
	bool				bOnline;			// is online 
	ULONG				ulTargetID;			// 파티원이 공격하고 있는 타겟 ID
	SBYTE				sbTargetType;		// 파티원이 공격하고 있는 타겟 Type
};

//원정대 멤버 정보
struct stExpeditionMember
{
	SLONG		m_slIndex;								// 캐릭터 인덱스
	SLONG		m_slZone;								// 존번호
	CTString	m_strName;								// 이름
	SBYTE		m_nPosition;							// 직책
	SBYTE		m_sbJob;								// 직업
	int			m_nLevel;								// 레벨
	int			m_nHP;									// HP
	int			m_nMaxHP;								// MAX HP
	int			m_nMP;									// MP
	int			m_nMaxMP;								// MAX MP
	FLOAT		m_fPosX;								// 위치(X)
	FLOAT		m_fPosZ;								// 위치(Z)
	SBYTE		m_sbLayer;								// Layer
	FLOAT		m_fHPBarRate;							
	FLOAT		m_fMPBarRate;							
	BOOL		m_bOnline;								// 접속 상태

	stExpeditionMember()
	{
		Init();
	}

	void Init()
	{
		m_slIndex = -1;		
		m_slZone = -1;	
		m_strName = "";	
		m_nPosition = -1;
		m_sbJob = -1;	
		m_nLevel = -1;	
		m_nHP = -1;		
		m_nMaxHP = -1;	
		m_nMP = -1;		
		m_nMaxMP = -1;	
		m_fPosX = -1;	
		m_fPosZ = -1;	
		m_sbLayer = -1;	
		m_fHPBarRate = 0.0f;
		m_fMPBarRate = 0.0f;
		m_bOnline = FALSE;
	}

	BOOL IsEmpty()
	{
		return m_slIndex == -1 ? TRUE : FALSE;
	}

	//HP,MP 남은 비율 계산
	void UpdateBarRate()
	{
		FLOAT	fRatio = (FLOAT)m_nHP / (FLOAT)m_nMaxHP;
		if( fRatio > 1.0f ) fRatio = 1.0f;	

		m_fHPBarRate =  fRatio;

		fRatio = (FLOAT)m_nMP / (FLOAT)m_nMaxMP;
		if( fRatio > 1.0f ) fRatio = 1.0f;	

		m_fMPBarRate =  fRatio;
	}

	//주어진 길이를 통해 비례한 길이 계산
	int CalcHPBarWidth(int nTotalBarWidth)
	{
		return nTotalBarWidth * m_fHPBarRate;
	}

	int CalcMPBarWIdth(int nTotalBarWidth)
	{
		return nTotalBarWidth * m_fMPBarRate;
	}
};



#endif	//	__COMMUNITY_DEFINE__