// Affinity.h: interface for the CAffinity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AFFINITY_H__CD36386E_C36B_445F_94E3_AD284737E3AE__INCLUDED_)
#define AFX_AFFINITY_H__CD36386E_C36B_445F_94E3_AD284737E3AE__INCLUDED_

#include <set>
#include <map>


#define MAX_ITEM_CONTRIBUTE			14
#define AF_CONNECT					(1<<0)
#define AF_CONTRIBUTE				(1<<1)
#define AF_SHOP						(1<<2)
#define AF_REWARD					(1<<3)

typedef struct __tagAffinityWork
{
	int		dataidx;		// 인덱스
	int		value;			// 기여도 값
} AFFINITYWORK;

typedef struct __tagRewardItem
{
	int		allowpoint;
	int		itemidx;
	int		itemflag;
	int		itemcount;
	int		exp;
	int		sp;
	int		needPCLevel;
	int		needItemIdx;
	int		needItemCount;
} REWARDITEM;

class CAffinityNPC
{
public:
	CAffinityNPC();
	~CAffinityNPC();

	int m_npcidx;
	int m_shoppoint;

	int m_totalreward;
	REWARDITEM*	m_reward;

	REWARDITEM* GetNextReward(int receivePoint);
	bool LoadRewardItem();
	int m_flag;
	int	GetFlag(int mark)
	{
		return (m_flag & mark);
	}
};

class CAffinityProto
{
public:
	CAffinityProto();
	virtual ~CAffinityProto();

	int				m_index;
	CLCString		m_name;
	int				m_maxvalue;
	int				m_nas;

	int				m_needAffinIdx;
	int				m_needAffinPoint;
	int				m_needPCLevel;
	int				m_needItemIdx;
	int				m_needItemCount;

	int				GetAffinityPointOfNPC(int npcidx);
	int				GetAffinityPointOfQuest(int npcidx);
	int				GetAffintiyPointOfItem(int itemidx);

	bool			LoadAffinityWork();
	bool			LoadAffinityNPC();

	CAffinityNPC*	FindAffinityNPC(int npcidx);

	int			m_npcWorkCount;
	int			m_itemWorkCount;
	int			m_questWorkCount;
	int			m_affinityNPCCount;

	typedef std::map<int, AFFINITYWORK*> map_AFFINITYWORK_t;

	AFFINITYWORK*			m_npcWork;
	map_AFFINITYWORK_t		map_npcWork_;

	AFFINITYWORK*			m_itemWork;
	map_AFFINITYWORK_t		map_itemWork_;

	AFFINITYWORK*			m_questWork;
	map_AFFINITYWORK_t		map_questWork_;

	typedef std::map<int, CAffinityNPC*> map_CAffinityNPC_t;
	CAffinityNPC*			m_affinityNPC;
	map_CAffinityNPC_t		map_affinityNPC_;
};

class CAffinityProtoList
{
public:
	CAffinityProtoList();
	virtual ~CAffinityProtoList();

	CAffinityProto* FindProtoByAffinityIdx(int idx);
	CAffinityProto* FindProtoByNPCIdx(int idx);
	bool LoadAffinity();

	typedef std::map<int, CAffinityProto*> map_t;

	int		m_AffinityTotal;
	CAffinityProto* m_affinity;
	map_t		map_;
};

// 현재 보유중인 보상정보
typedef struct __tagRewardStep
{
	int				affinitynpc;
	int				point;
} REWARDSTEP;

typedef std::set<REWARDSTEP *> set_rewardstep_t;

// 캐릭터정보
class CAffinity
{
public:
	CAffinity();
	virtual ~CAffinity();

	CAffinityProto*				m_proto;
	int							m_affinityidx;		// 보유친화도index
	int							m_point;			// point

	// 보상아이템 관련
	set_rewardstep_t			m_rewardstep;

	// npc에게 다음 보상받는 단계
	REWARDITEM*					GetNextReward(int npcidx);

	REWARDSTEP*					FindRewardInfo(int npcidx);

	int							AddPoint(int point, CPC* pc, int bonus = 0);

	void						UpdateRewardStep(int npcidx, int point);
};

typedef std::map<int,CAffinity*> map_affinitylist_t;

class CAffinityList
{
public:

	CAffinityList();
	~CAffinityList();

	map_affinitylist_t			m_list;

	bool AddAffinity(int affinityidx, int point);
	bool UpdateRewardPoint(int npcidx, int point);
	CAffinity* FindAffinity(int affinityidx);
	map_affinitylist_t* GetList()
	{
		return &m_list;
	}
	void clear();

	void						SendRewardNotice(CPC* pc);
};


#endif // !defined(AFX_AFFINITY_H__CD36386E_C36B_445F_94E3_AD284737E3AE__INCLUDED_)
//
