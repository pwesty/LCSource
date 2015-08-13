#ifndef __GUILD_H__
#define __GUILD_H__

#define GUILD_MAX_OFFICER			2
#define GUILD_MAX_MEMBER			200
#define GUILD_MAX_GUILD_LEVEL		50

#define GUILD_REG_DELAY				3

#define GUILD_LEVEL1_MAX_MEMBER		10
#define GUILD_LEVEL1_NEED_LEVEL		10
#define GUILD_LEVEL1_NEED_SP		10
#define GUILD_LEVEL1_NEED_MONEY		10000

#define GUILD_LEVEL2_MAX_MEMBER		15
#define GUILD_LEVEL2_NEED_LEVEL		15
#define GUILD_LEVEL2_NEED_SP		30
#define GUILD_LEVEL2_NEED_MONEY		50000

#define GUILD_LEVEL3_MAX_MEMBER		20
#define GUILD_LEVEL3_NEED_LEVEL		20
#define GUILD_LEVEL3_NEED_SP		60
#define GUILD_LEVEL3_NEED_MONEY		250000

#define GUILD_LEVEL4_MAX_MEMBER		25
#define GUILD_LEVEL4_NEED_LEVEL		25
#define GUILD_LEVEL4_NEED_SP		120
#define GUILD_LEVEL4_NEED_MONEY		500000

#define GUILD_LEVEL5_MAX_MEMBER		30
#define GUILD_LEVEL5_NEED_LEVEL		30
#define GUILD_LEVEL5_NEED_SP		240
#define GUILD_LEVEL5_NEED_MONEY		1000000

#define GUILD_LEVEL6_MAX_MEMBER		30
#define GUILD_LEVEL6_NEED_LEVEL		30
#define GUILD_LEVEL6_NEED_SP		500
#define GUILD_LEVEL6_NEED_MONEY		2000000

#define GUILD_LEVEL7_NEED_LEVEL		30
#define GUILD_LEVEL7_NEED_SP		550
#define GUILD_LEVEL7_NEED_GP		18000
#define GUILD_LEVEL7_NEED_MONEY		2500000

#define GUILD_EXTEND_BASE_LEVEL			7			// 레벨 확장 시 변화되는 레벨의 기본값
#define GUILD_POSITION_NAME		12
#define GUILD_NOTICE_TITLE_MAX	64
#define GUILD_NOTICE_TEXT_MAX	1024

#define GUILD_INCLINE_GENERAL		0		// 일반 길드
#define GUILD_INCLINE_CHAO			1		// 카오 길드
#define GUILD_INCLINE_HUNTER		2		// 헌터 길드

#define GUILD_BATTLE_MIN_MEMBER		10

#define GUILD_BATTLE_MIN_PRIZE		100000

#if defined(LC_GAMIGO)
#define GUILD_BATTLE_MAX_PRIZE		10000000
#else
#define GUILD_BATTLE_MAX_PRIZE		1000000
#endif

#define GUILD_BATTLE_MIN_TIME		6000		// 최소 10분
#define GUILD_BATTLE_MAX_TIME		180000		// 최대 300분

#define GUILD_BATTLE_WAIT_TIME		600		// 1분

#define GUILD_BATTLE_STATE_PEACE		-1		// 평화 상태
#define GUILD_BATTLE_STATE_START_WAIT	0		// 길드전 신청 중
#define	GUILD_BATTLE_STATE_WAIT			1		// 길드전 대기
#define	GUILD_BATTLE_STATE_ING			2		// 길드전 진행
#define GUILD_BATTLE_STATE_STOP_WAIT	3		// 길드전 중단 신청 중
#define	GUILD_BATTLE_STATE_PRIZE		4		// 길드전 후 상금 대기 모드

#define MANAGE_NEED_LEVEL				5

// 길드 시그널
typedef struct __tagGuildSignal
{
	MSG_GUILD_POSITION_TYPE		flag;
	int							senderIndex;
	float						x;
	float						z;
} GUILD_SIGNAL;

#include "GuildKick.h"

class CGuild;

class CGuildMember
{
	friend class CGuild;

	int				m_charindex;
	CLCString		m_name;
	//CLCString		m_nick;
	int				m_pos;
	CGuild*			m_guild;
	int				m_listindex;
	char			m_online;

	CPC*			m_pc;

	int				m_contributeExp;	// 경험치에서 길드 포인트로 기여할 %
	int				m_contributeFame;	// 명성치에서 길드 포인트로 기여할 %
	int				m_cumulatePoint;	// 기여한 누적 포인트
	CLCString		m_positionName;
	int				m_channel;			// 현재 접속한 채널
	int				m_zoneindex;			// 현재 접속한 위치

	CGuildMember();

public:
	~CGuildMember();

	int charindex()
	{
		return m_charindex;
	}
	void charindex(int index)
	{
		m_charindex = index;
	}

	const char* GetName()
	{
		//member nick부분 삭제 : bw 060403
		//if(strcmp((const char*) m_nick, "") != 0)
		//	return (const char*)m_nick;
		return (const char*) m_name;
	}
//	const char* nick() { return (const char*)m_nick; }
	void name(const char* str)
	{
		if (str) m_name = str;
		else m_name = "";
	}
//	void nick(const char* str) { if (str) m_nick = str; else m_nick = ""; }

	MSG_GUILD_POSITION_TYPE pos()
	{
		return (MSG_GUILD_POSITION_TYPE)m_pos;
	}
	void pos(int p)
	{
		m_pos = p;
	}

	CGuild* guild()
	{
		return m_guild;
	}
	void guild(CGuild* g)
	{
		m_guild = g;
	}

	int listindex()
	{
		return m_listindex;
	}

	char online()
	{
		return m_online;
	}
	void online(char val)
	{
		m_online = val;
	}

	char GetGuildMark();

	void contributeExp( int contribute )
	{
		m_contributeExp = contribute;
	}
	void contributeFame( int contribute )
	{
		m_contributeFame = contribute;
	}
	void cumulatePoint( int point )
	{
		m_cumulatePoint = point;
	}
	void AddCumulatePoint( int point )
	{
		m_cumulatePoint += point;
	}
	//void positionName( const char* str ) { if( str ) m_positionName = str; else m_positionName = ""; }
	void positionName( CLCString& str )
	{
		m_positionName = "";
		if( str.Length() > 0 )
			m_positionName = str;
	}

	void channel( int channel )
	{
		m_channel = channel;
	}
	void zoneindex( int zoneNum )
	{
		m_zoneindex = zoneNum;
	}

	int	GetcontributeExp( )
	{
		return m_contributeExp;
	}
	int GetcontributeFame()
	{
		return m_contributeFame;
	}
	int GetcumulatePoint()
	{
		return m_cumulatePoint;
	}
	const char* GetPositionName( )
	{
		return m_positionName;
	}

	int	GetChannel()
	{
		return m_channel;
	}
	int GetZoneNum()
	{
		return m_zoneindex;
	}

	void SetPC(CPC* pc)
	{
		if (guild() == NULL)
		{
			m_pc = NULL;
			if (pc)
				pc->m_guildInfo = NULL;
			return ;
		}
		if (m_pc && pc == NULL)
			m_pc->m_guildInfo = NULL;
		m_pc = pc;
		if (pc)
			pc->m_guildInfo = this;
	}
	CPC* GetPC()
	{
		return m_pc;
	}

private:
	void listindex(int idx)
	{
		m_listindex = idx;
	}
};

class CGuild
{
	int				m_index;
	CLCString		m_name;
	int				m_level;
	CGuildMember*	m_boss;
	CGuildMember*	m_officer[GUILD_MAX_OFFICER];

	int				m_membercount;

	CGuild*			m_next;
	CGuild*			m_prev;

	// 길드전 -------------------
	int				m_battleIndex;
	int				m_battlePrize;
	int				m_battleZone;
	int				m_battleTime;
	int				m_killCount;
	char			m_battleState;
	int				m_battlePulse;
	// ---------------------------

	GUILD_SIGNAL	m_guildSignal;

	int				m_landcount;
	int				m_land[256];					// 소유지
	int				m_GuildPoint;			// 전체 길드 포인트
	char			m_incline;				// 길드 성향 -> 카오인지 헌터인지 구분
	int				m_maxmember;
	int				m_GuildPointRanking;		// 길드 포인트 랭킹
	// 길드 마크 추후 추가;
	// 길드 스킬
	////////////
	// 스킬 정보
public:
	CGuildMember*	m_member[GUILD_MAX_MEMBER];
	CSkillList	m_passiveSkillList;			// 길드 스킬 리스트
	CSkillList	m_activeSkillList;
	CSkillList	m_etcSkillList;
	CSkill* FindSkill(int index);
	void clearReadySkill();

	bool m_bRebirthPos;	// 부활 진지를 먹었는지 체크 하는 부분

public:
	CGuild(int guildindex, const char* name, int level, int battleIndex = -1, int battlePrize = 0, int battleTime = 0, int batttleZone = -1, int battleKillCount = 0, int battleState = -1);
	~CGuild();

	CPC* GetNearMember(CCharacter* pPC, int nListIndex);

	void landcount( int landcount )
	{
		m_landcount = landcount;
	}
	void land( int count, int* land )
	{
		if( count != 0 )
		{
			for( int i = 0; i < count; i++ )
				m_land[i] = land[i];
		}
	}
	void guildPoint( int guildPoint )
	{
		m_GuildPoint = guildPoint;
	}
	void incline( char incline )
	{
		m_incline = incline;
	}
	void AddGuildPoint( int GuildPoint );

	int GetLandCount( )
	{
		return m_landcount;
	}
	int* GetLand()
	{
		return m_land;
	}
	int GetGuildPoint()
	{
		return m_GuildPoint;
	}
	char GetIncline()
	{
		return m_incline;
	}
	void guildPointRanking( int nRanking )
	{
		m_GuildPointRanking = nRanking;
	}
	int	GetGuildPointRanking( )
	{
		return m_GuildPointRanking;
	}

	bool IsRebrithPos( )
	{
		return m_bRebirthPos;
	}
	void SetRerithPos( bool bUse )
	{
		m_bRebirthPos = bUse;
	}

	int index()
	{
		return m_index;
	}

	void	changeName(const char* name)
	{
		m_name = name;
	}

	const char* name()
	{
		return (const char*)m_name;
	}

	int level()
	{
		return m_level;
	}
	void level(int n)
	{
		m_level = n;
	}

	void LevelUp()
	{
		if (m_level < GUILD_MAX_GUILD_LEVEL) m_level++;
	}

	CGuildMember* boss()
	{
		return m_boss;
	}

	CGuildMember* officer(int idx)
	{
		return (idx < 0 || idx >= GUILD_MAX_OFFICER) ? NULL : m_officer[idx];
	}

	CGuild* nextguild()
	{
		return m_next;
	}
	void nextguild(CGuild* g)
	{
		m_next = g;
	}

	CGuild* prevguild()
	{
		return m_prev;
	}
	void prevguild(CGuild* g)
	{
		m_prev = g;
	}

	int membercount()
	{
		return m_membercount;
	}

	CGuildMember* member(int idx)
	{
		return (idx < 0 || idx >= GUILD_MAX_MEMBER) ? NULL : m_member[idx];
	}

	// 길드전 추가

	int battleIndex()
	{
		return m_battleIndex;
	}

	int battlePrize()
	{
		return m_battlePrize;
	}

	int battleZone()
	{
		return m_battleZone;
	}

	void BattleSet (int i, int m, int z)
	{
		m_battleIndex = i;
		m_battlePrize = m;
		m_battleZone = z;
	}

	int battlePulse()
	{
		return m_battlePulse;
	}
	void BattlePulse(int p)
	{
		m_battlePulse = p;
	}

	int battleTime()
	{
		return m_battleTime;
	}
	void BattleTime(int t)
	{
		m_battleTime = t;
	}

	int killCount()
	{
		return m_killCount;
	}
	void KillCount(int c)
	{
		m_killCount = c;
	}

	char battleState()
	{
		return m_battleState;
	}
	void BattleState(char s)
	{
		m_battleState = s;
	}
	//---------------------------


	GUILD_SIGNAL* guildSignal()
	{
		return &m_guildSignal;
	}
	void guildSignal(MSG_GUILD_POSITION_TYPE flag, int senderIndex, float x, float z)
	{
		m_guildSignal.flag = flag;
		m_guildSignal.senderIndex = senderIndex;
		m_guildSignal.x = x;
		m_guildSignal.z = z;
	}

	int maxmember();
	void setMaxmember( int maxmember )
	{
		m_maxmember = maxmember;
	};

	////////////////////
	// Function name	: addmember
	// Description	    : 멤버 추가,
	// Return type		: int
	//                  : 성공: 추가된 멤버의 리스트 인덱스, 실패: -1
	int addmember(int charindex, const char* name);

	void removemember(CGuildMember* delmember);

	////////////////////
	// Function name	: changeboss
	// Description	    : 길드장 변경
	// Return type		: CGuildMember*
	//                  : 변경 이전 길드장 멤버 포인터
	CGuildMember* changeboss(int newbosslistindex);

	void appoint(int listindex);

	void fire(int listindex);

	////////////////////
	// Function name	: findmember
	// Description	    : 길드원을 찾는다.
	CGuildMember* findmember(int charindex);

	CGuildMember* findmember(const char* charname);

	// bCell이 true이면 길드원 주위의 셀에 모두 dis, appear 메세지를 보낸다.
	void SendToAll(CNetMsg::SP& msg, bool bCell = false);
	void SendToAllInSameZone(CNetMsg::SP& msg, int zoneindex);
	void SendToAllStatusMsg();

#ifdef GMTOOL
	bool m_bGmtarget;
#endif // GMTOOL

#ifdef EXTREME_CUBE
	int	m_cubeUniqueIdx;
#endif // EXTREME_CUBE

#ifdef LC_GAMIGO
	int BossChangeNeedPCLevel();
#endif // LC_GAMIGO

	int				m_nCountRushCaptain;
	int				m_nCountSupportCaptain;
	int				m_nCountReconCaptain;
	int				m_nCountTotalGradeEx;

	void InitGradeExPosCount();
	bool CheckGradeExPosCount( int pos, int charIndex = 0 );
	void ChangeGradeExPosCount( int oldpos, int pos);
	void DelGradeExPosCount(int pos);
	void AddGradeExPosCount( int pos );
	int  GetGradeExPosNeedGuilPoint( int pos );

#ifdef DEV_GUILD_MARK
private:
	char m_GuildMark_row;
	char m_GuildMark_col;
	char m_Background_row;
	char m_Background_col;
	// 길드마크 시간은 실제 로컬 시간으로 저장되어 있지만 게임에서 사용할때는 게임시간을 클라이언트에 보내준다.
	int m_GuildMarkExpire;
public:
	void SetGuildMark(char gm_row, char gm_col, char bg_row, char bg_col, int markTime);
	char GetGuildMarkRow();
	char GetGuildMarkCol();
	char GetBackgroundRow();
	char GetBackgroundCol();
	int GetMarkTime();
	int GetMarkTimeConvertGameTime();
#endif // GUILD_MARK

private:
	CGuildKick m_guildKick;

	int getStandardTimeForKick(void);
	int getBossDateStamp(void);
	int getLastChansgeBossTime(void);
public:
	CGuildKick* getGuildKick(void);

	bool isPossibleKickMaster(void);		// 보스 추방이 가능한지?
	bool isPastLastChangeBoss(void);		// 최종 보스가 변경된 후 7일이 지났는지?
	int getNewBossByKick(void);				// 추방으로 인한 새로운 보스 구하기
};

class CGuildList
{
	CGuild*			m_head;
	int				m_count;

public:
	CGuildList();
	~CGuildList();

	////////////////////
	// Function name	: create
	// Description	    : 길드를 생성 -> 멤버에 boss추가 -> boss 설정 -> 1명짜리 온전한 독립 길드 생성
	static CGuild* create(int guildindex, const char* guildname, int charindex, const char* bossname);
	static CGuild* create(int guildindex, const char* guildname, int guildlevel, int battleIndex, int battlePrize, int battleTime, int batttleZone, int battleKillCount, int battleState);

	void add(CGuild* guild);
	void Remove(CGuild* guild);

	CGuild* findguild(int guildindex);

	CGuild* findguild(const char* guildname);

	////////////////////
	// Function name	: findmember
	// Description	    : 특정 캐릭터가 소속된 길드를 찾는다
	CGuildMember* findmember(int charindex);

	void Reset();

	CGuild* head()
	{
		return m_head;
	}
};

// 적대 길드 리스트
class CHostilityGuildList
{
	CGuild*		m_head;
	int			m_count;

public:
	CHostilityGuildList();
	~CHostilityGuildList();

	void AddGuild( CGuild* guild );
	void RemoveGuild( CGuild* guild );

	CGuild* FindGuild( int guildindx );

	////////////////////
	// Function name	: findmember
	// Description	    : 특정 캐릭터가 소속된 길드를 찾는다
	CGuildMember* findmember(int charindex);

	void Reset();

	CGuild* head()
	{
		return m_head;
	}
};

// 동맹 길드 리스트
class CAllyGuildList
{
	CGuild*		m_head;
	int			m_count;

public:
	CAllyGuildList();
	~CAllyGuildList();

	void AddGuild( CGuild* guild );
	void RemoveGuild( CGuild* guild );

	CGuild* FindGuild( int guildindx );

	////////////////////
	// Function name	: findmember
	// Description	    : 특정 캐릭터가 소속된 길드를 찾는다
	CGuildMember* findmember(int charindex);

	void Reset();

	CGuild* head()
	{
		return m_head;
	}
};

#endif
