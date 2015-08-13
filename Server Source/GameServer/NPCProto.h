#ifndef __ANPCPROTO_H__
#define __ANPCPROTO_H__

#include <vector>
#include "../ShareLib/LCString.h"
#include "Skill.h"

class CNPCProtoList;

class CAffinityProto;

// t_npc_dropraid
typedef struct __tagRaidDropData
{
	int			itemindex;
	int			flag;
	int			count;
	int			prob;


#ifdef BUGFIX_SPECIALBOX_DROPRATE
	int			spec_itemindex[14];
#else
	int			spec_itemindex1;     // 스페셜 아이템
	int			spec_itemindex2;
	int			spec_itemindex3;
	int			spec_itemindex4;
	int			spec_itemindex5;
	int			spec_itemindex6;
	int			spec_itemindex7;
	int			spec_itemindex8;
	int			spec_itemindex9;
	int			spec_itemindex10;
	int			spec_itemindex11;
	int			spec_itemindex12;
	int			spec_itemindex13;
	int			spec_itemindex14;
#endif //BUGFIX_SPECIALBOX_DROPRATE
	int			spec_Min;
	int			spec_Max;
	int			spec_count;
	int			spec_flag;
	int			spec_prob;

} RAIDDROPDATA;
inline int Comp_RAIDDROPDATA(RAIDDROPDATA d1, RAIDDROPDATA d2)
{
	return d1.itemindex - d2.itemindex;
}

typedef std::vector<RAIDDROPDATA> vec_raiddropdata_t;
typedef std::vector<CAffinityProto*> vec_affinityList_t;

class CNPCProto
{
	friend class CNPCProtoList;

public:
	CNPCProto();
	~CNPCProto();

	/////////////////////////////////////
	// DB 에서 읽어오는 NPC에 대한 정보들
	int		m_index;			// 몹 번호
	CLCString m_name;			// 이름

	int		m_level;			// 렙
	int		m_price;			// 돈
	float	m_sight;			// 시야
	float	m_walkSpeed;		// 걷기이동속도
	float	m_runSpeed;			// 뛰기이동속도
	float	m_size;				// 크기(반지름)
	int		m_hp;				// 체력
	int		m_mp;				// 마나

	int		getExp()				{ return m_exp; }
	void	setExp(int exp)			{ m_exp = exp; }
	int		getSkillPoint()			{ return m_skillPoint; }
	void	setSkillPoint(int sp)	{ m_skillPoint = sp; }

#ifdef PREMIUM_CHAR
	int		getExpForPremiumChar(int premium_type);
	int		getSkillPointForPremiumChar(int premium_type);
#endif

private:
	int		m_exp;				// 경험치
	int		m_skillPoint;		// 스킬포인트

	int		m_flag;				// 속성
	int		m_flag1;			// 속성(확장)
	int		m_stateFlag;		// 상태이상 플래그

public:
	int		m_family;			// 동족번호 없으면 -1
	int		m_recoverHP;		// 회복HP
	int		m_recoverMP;		// 회복MP
	int		m_item[MAX_NPC_DROPITEM];			// 드롭아이템
	int		m_itemPercent[MAX_NPC_DROPITEM];	// 아이템 드롭 확률
	int		m_itemCount;		// 드롭 테이블의 수
	int		m_product[MAX_NPC_PRODUCT];		// 생산품

// 050303 : bs : 몬스터에게서 plus 붙은 아이템 만들기
	int		m_minplus;			// 최소 플러스 수치
	int		m_maxplus;			// 최대 플러스 수치
	int		m_probplus;			// 플러스 붙을 확률
// --- 050303 : bs : 몬스터에게서 plus 붙은 아이템 만들기

	int		m_str;				// 힘
	int		m_dex;
	int		m_int;
	int		m_con;

	int		m_attack;			// 공격
	int		m_magic;			// 마법
	int		m_defense;			// 방어
	int		m_resist;			// 마방

	int		m_attacklevel;		// 공격레벨
	int		m_defenselevel;		// 방어레벨

	// 물리어택 류
	char	m_attackType;
	char	m_attackSpeed;		// 공격당 시간(0.1초)
	float	m_attackArea;		// 믈리공격 사정거리

	// 스킬 류
	int		m_nSkill;
	int		m_skillIndex[MAX_NPC_SKILL];
	int		m_skillLevel[MAX_NPC_SKILL];
	int		m_skillProb[MAX_NPC_SKILL];

	int		m_moveArea;			// 이동영역

	// ai 관련
	int		m_aitype;
	int		m_aiflag;
	int		m_aileader_flag;
	int		m_ai_summonHp;
	int		m_aileader_idx;
	int		m_aileader_count;

	int		m_nHit;
	int		m_nDodge;
	int		m_nMagicAvoid;

	unsigned char m_attrdef;
	unsigned char m_attratt;

	vec_raiddropdata_t		m_listRaidDrop;

#ifdef EXTREME_CUBE
	int		m_jobAttr;
#endif // EXTREME_CUBE

	vec_affinityList_t	m_affinityList;

	int		m_jewel[MAX_NPC_DROPITEM];			// 드롭아이템
	int		m_jewelPercent[MAX_NPC_DROPITEM];	// 아이템 드롭 확률
	int		m_jewelCount;
	int		m_jobdropitem[JOBCOUNT];
	int		m_jobdropitemprob[JOBCOUNT];

	int		m_dropallitem[MAX_NPC_DROPITEM];
	int		m_dropallitemprob[MAX_NPC_DROPITEM];

	int		m_summonIdx;

	LONGLONG		m_zone_flag;			//공간이동이 가능한 npc일경우의 이동시켜 줄 수 있는 존정보
	LONGLONG		m_extra_flag;			//공간이동이 가능한 npc의 이동가능한 존의 지역 정보

	int		m_rvr_value;
	int		m_rvr_grade;

	float	m_bound;
	
	int		m_lifetime;

	bool			m_bSydicateJoinGiveQuestNPC;	// 결사대 가입 퀘스트 부여 NPC
	// m_flag 검사
	int		CheckFlag(int mask) const
	{
		return m_flag  & mask;
	}
	int		CheckFlag1(int mask) const
	{
		return m_flag1 & mask;
	}
	int		CheckStateFlag(int mask) const
	{
		return m_stateFlag & mask;
	}

	int		GetFlag() const
	{
		return m_flag;
	}
	int		GetFlag1() const
	{
		return m_flag1;
	}
	int		GetStateFlag() const
	{
		return m_stateFlag;
	}

	// m_aiflag 검사 : bit번호
	int		CheckAIBit(int bit)
	{
		return m_aiflag & (1 << bit);
	}

	// m_aileader_flag 검사 : bit번호
	int		CheckLeaderBit(int bit)
	{
		return m_aileader_flag & (1 << bit);
	}
};

class CNPC;
class CNPCRegenInfo;

class CNPCProtoList
{
public:
	typedef std::map<int, CNPCProto*> map_t;

	CNPCProtoList();
	~CNPCProtoList();

	map_t		map_;
	CNPCProto*	m_npcProtoList;		// NPC 기본 정보 리스트
	int			m_nCount;			// 개수
	int			m_virtualIndex;		// 가상 인덱스

	bool Load();						// NPC 기본 정보 읽기
	CNPC* Create(int npcindex, CNPCRegenInfo* regenInfo);			// 해당 idx의 NPC 생성하기
	CNPCProto* FindProto(int npc_idx);	// 해당 idx의 정보 반환

private:
	std::vector<std::string> a_item_str;
	std::vector<std::string> a_item_percent_str;

	std::vector<std::string> a_jewel_str;
	std::vector<std::string> a_jewel_percent_str;

	std::vector<std::string> a_product_str;

	std::vector<std::string> a_skill_str;
};

#endif
//
