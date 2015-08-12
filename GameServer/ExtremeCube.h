#ifndef __EXTREME_CUBE_H__
#define __EXTREME_CUBE_H__

#include "LCList.h"

#ifdef EXTREME_CUBE

#define MAX_CUBESPACE		30
#define BATTLECUBE_START	0
#define BOSSCUBE_START		25

#if defined(LC_BILA)
#define GUILDCUBE_TIME		7200		// 2시간
#else
#define GUILDCUBE_TIME		3600		// 1시간
#endif

#if defined (LC_GAMIGO) || defined (LC_BILA) || defined (LC_USA) || defined (LC_RUS)
#define EXTREME_CUBE_SUBSERVER 3
#else
#define EXTREME_CUBE_SUBSERVER 1
#endif

#define CUBEMASTER_RED		521
#define CUBEMASTER_BLUE		522
#define CUBEMASTER_YELLOW	523
#define CUBEMASTER_PURPLE	524
#define CUBEMASTER_ORANGE	525
#define CUBE_RED			526
#define CUBE_BLUE			527
#define CUBE_YELLOW			528
#define CUBE_PURPLE			529
#define CUBE_ORANGE			530

#define CRYSTALCOUNT		5

// 큐브상태
typedef enum _tagCubeState
{
	CUBESTATE_RED = 0,
	CUBESTATE_ORANGE,
	CUBESTATE_BLUE,
	CUBESTATE_PURPLE,
	CUBESTATE_YELLOW,
} CUBESTATE;

// 큐브멤버타입
typedef enum _tagCubeMemType
{
	CUBEMEM_PARTY,
	CUBEMEM_GUILD,
} CUBEMEMTYPE;

// 큐브공간타입
typedef enum _tagCubeSpaceType
{
	BATTLE_SPACE,
	BOSS_SPACE,
} CUBESPACETYPE;

// 몹리젠포인트
typedef struct _tagCubeRegenPoint
{
	float	regenX;
	float	regenZ;
	int	 	regenY;
	char	ylayer;
} CUBEREGENPOINT;

#if defined LC_KOR || defined (LC_GAMIGO)
typedef struct __tagRegDate
{
	int		nDay;
	int		nHour;
	int		nMin;
	int		nSec;
} REGDATE;
#endif

class CExtremeCube;

//////////////////////////////// 멤버리스트 ///////////////////////////////
class CCubeMemList
{
public:
	int GetPCCount()
	{
		return m_pcList.GetCount();
	}
	void AddPC(CPC* pc)
	{
		m_pcList.AddToTail(pc);
	}
	bool SetCubeNum(int idx)
	{
		if(idx >= 0 && idx < MAX_CUBESPACE)
		{
			m_cubenum = idx;
			return true;
		}
		m_cubenum = -1;
		return false;
	}

	bool GetCubeNum(int& idx)
	{
		if(m_cubenum >= 0 && m_cubenum < MAX_CUBESPACE)
		{
			idx = m_cubenum;
			return true;
		}
		idx = -1;

		return false;
	}

	// 큐브에 새로 참가
	bool JoinCube(CPC* pc, int extra);

	// 큐브내의 이동
	bool MoveToOtherCube(int cubeidx);
	int  GetUniqueIdx()
	{
		return m_index;
	}
	bool IsPartyCubeMemList()
	{
		return m_type == CUBEMEM_PARTY;
	}
	bool IsGuildCubeMemList()
	{
		return m_type == CUBEMEM_GUILD;
	}

#ifdef EXTREME_CUBE_VER2
	virtual void DelPC(CPC* pc) = 0;
	virtual void DelSummonNpc() = 0;
#else
	void DelPC(CPC* pc);
	void DelSummonNpc();
#endif // EXTREME_CUBE_VER2

	bool FindPC(CPC *pc);

	void GetOutAll();

	// 클리어 증가
	void IncreaseWinCount()
	{
		m_clearCount++;
	}
	int  GetWinCount()
	{
		return m_clearCount;
	}

	int GetPersonalCubePoint(CPC* pc, int level);
	int GetIndex()
	{
		return m_index;
	}
	bool IsEnable()
	{
		return m_bEnable;
	}

	virtual void Disable() = 0;
	virtual bool IsBreak() = 0;

#ifdef EXTREME_CUBE_VER2
	void SendStartCount();
	void ApplyCubeEffect();
#endif // EXTREME_CUBE_VER2

	CCubeMemList();
	virtual ~CCubeMemList();

protected:
	int		m_cubenum;
	char	m_type;
	CLCList<CPC*> m_pcList;
	char	m_clearCount;
	static int		m_uniqueidx;
	int		m_index;
	bool	m_bEnable;
};

class CPartyCubeMemList : public CCubeMemList
{
public:
	CPartyCubeMemList();
	~CPartyCubeMemList();

	CParty*		GetParty()
	{
		return m_party;
	}
	void		SetParty(CParty* party)
	{
		m_party = party;
	}
	void		Enable(CParty* party, int cubenum);
	void		Disable();
	bool		IsBreak();

#ifdef EXTREME_CUBE_VER2
	void		DelPC(CPC* pc);
#endif // EXTREME_CUBE_VER2

	void DelSummonNpc();

private:
	CParty*		m_party;
};

class CGuildCubeMemList : public CCubeMemList
{
public:
	CGuildCubeMemList();
	~CGuildCubeMemList();

	void		SetGuild(CGuild* guild)
	{
		m_guild = guild;
	}
	CGuild*		GetGuild()
	{
		return m_guild;
	}
	void		Enable(CGuild* guild, int cubenum);
	void		Disable();
	bool		IsBreak();

#ifdef EXTREME_CUBE_VER2
	void		DelPC(CPC* pc);
#endif // EXTREME_CUBE_VER2

	void DelSummonNpc();

private:
	CGuild*		m_guild;
};

///////////////////////////////// 큐브 /////////////////////////////////////
class CCubeSpace
{
public:
	CCubeSpace();
	virtual ~CCubeSpace();

	virtual void CheckCubeState() = 0;

	virtual bool SetMemList(CCubeMemList* memlist) = 0;
	virtual bool DelMemList(CCubeMemList* memlist) = 0;

	virtual void DelMob(CNPC* npc) = 0;

	void ChageState(CUBESTATE state);

	virtual void Clean() = 0;

	virtual void RegenMob() = 0;

	bool IsBattleCubeSpace()
	{
		return m_type == BATTLE_SPACE;
	}
	bool IsBossCubeSpace()
	{
		return m_type == BOSS_SPACE;
	}

	void DelCrystal(bool bDisappear);

	CArea* m_area;
	const CExtremeCube*	m_extremeCube;
	int	m_extra;

	CNPC*   m_crystalkind[CRYSTALCOUNT];
	CNPC*	m_crystal;

	int m_waitTime;

	CUBESTATE GetState()
	{
		return m_state;
	}

protected:
	char			m_type;
	CUBESTATE		m_state;
};

// 일반큐브
class CBattleSpace : public CCubeSpace
{
public:
	CBattleSpace();
	virtual ~CBattleSpace();

	virtual void CheckCubeState();
	virtual bool SetMemList(CCubeMemList* memlist);
	virtual bool DelMemList(CCubeMemList* memlist);
	virtual void DelMob(CNPC* npc);
	virtual void Clean();
	virtual void RegenMob();

	bool			IsAvailableJoin(bool&);
	bool			IsAvailableJoinPersonal();

	CCubeMemList*	WinMemList()
	{
		if(m_memList1 != NULL && m_memList2 == NULL)
		{
			return m_memList1;
		}

		if(m_memList1 == NULL && m_memList2 != NULL)
		{
			return m_memList2;
		}

		return NULL;
	}

	CCubeMemList* GetMemList1()
	{
		return m_memList1;
	}
	CCubeMemList* GetMemList2()
	{
		return m_memList2;
	}

private:
	CLCList<CNPC*>	m_mobList;
	CCubeMemList*	m_memList1;
	CCubeMemList*	m_memList2;
};

class CBossSpace: public CCubeSpace
{
public:
	CBossSpace();
	virtual ~CBossSpace();

	virtual void CheckCubeState();
	virtual bool SetMemList(CCubeMemList* memlist);
	virtual void DelMob(CNPC* npc);
	virtual bool DelMemList(CCubeMemList* memlist);
	virtual void Clean();
	virtual void RegenMob();

	bool IsAvailableJoin();
	CCubeMemList* GetMemList()
	{
		return m_memList;
	}

private:
	CNPC*			m_boss;
	CCubeMemList*	m_memList;
};

class CExtremeCube
{
public:
	CExtremeCube();
	~CExtremeCube();

	void			CheckCubeSpace();

	int				GetGuildCubeTime()
	{
		return m_nextGuildCubeTime;
	}
	void			SetGuildCubeTime(long& time);
	bool			IsGuildCubeTime()
	{
		return m_bGuildCubeStart;
	}
	CCubeSpace*		GetExtremeCube(int i);

	bool			Load();
	CCubeMemList*	FindMemList(CGuild* guild);
	CCubeMemList*	FindMemList(CParty* party);
	CCubeMemList*	FindMemList(int uniqueidx);

	bool			Init();
	int				GetAvailableBattleCube(bool& bfirst);
	int				GetAvailableBossCube();
	bool			IsExtremeCubeServer();
	void			StartGuildCube();
	void			EndGuildCube();
	void			StartGuildCubeRemainTime();
	void			EndGuildCubeRemainTime();

	int				m_mobCount;
	int*			m_mobidx;
	int				m_regencount;
	CUBEREGENPOINT*	m_regenPoint;
	int				m_bossCount;
	int*			m_bossidx;
	time_t			m_nextGuildCubeTime;

	CPartyCubeMemList* GetAvailablePartyCubeMemList();
	CGuildCubeMemList* GetAvailableGuildCubeMemList();

private:
	void			GetOutAllMemList();
	bool			LoadCubeData();
	bool			CreateCubeSpace();
	bool			CheckCubeMasterNPC();
	bool			m_bGuildCubeStart;
	bool			m_bGuildCubeRemainStartNotice;
	bool			m_bGuildCubeRemainEndNotice;

	CCubeSpace*		m_cube[MAX_CUBESPACE];
	CPartyCubeMemList m_partyCubeMemList[MAX_CUBESPACE * 2];
	CGuildCubeMemList m_guildCubeMemList[MAX_CUBESPACE * 2];

#if defined (LC_KOR) || defined (LC_GAMIGO)
	int				m_regcount;
	REGDATE*		m_regdate;
#endif
};

#endif // EXTREME_CUBE

#endif // __EXTREME_CUBE_H__
//

//

//

//

//

//

//

//

//
