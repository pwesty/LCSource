/************************************
  수정사항
  수정한 사람: bs
  수정일: 2004-11-18 오전 10:57:23
  설명: 스킬의 효과 적용에서 미리 계산된 확률 값 사용
 ************************************/

/************************************
  수정사항
  수정한 사람: bs
  수정일: 2004-11-17 오후 3:22:34
  설명: CAssistList에 Apply 추가 - 값 적용 함수
 ************************************/

#ifndef __ASSIST_H__
#define __ASSIST_H__

#include "../ShareLib/packetType/ptype_appear.h"

class CNetMsg;

typedef struct _tagAssistValue
{
	int	attack;
	int	defense;
	int	magic;
	int	resist;
	int	hitrate;
	int	avoid;
	int	critical;
	int	attackspeed;
	int	magicspeed;
	int	movespeed;
	int	recoverhp;
	int	recovermp;
	int	maxhp;
	int	maxmp;
	int	deadly;
	unsigned char attratt;
	unsigned char attrdef;
	unsigned char attratt_item;
	unsigned char attrdef_item;
	int magichitrate;
	int magicavoid;
	int attackdist;
	int reduceMelee;
	int reduceRange;
	int reduceMagic;
#ifdef ASSIST_REDUCE_SKILL
	int reduceSkill;
#endif
	int reduceMeleeProb;
	int reduceRangeProb;
	int reduceMagicProb;
#ifdef ASSIST_REDUCE_SKILL
	int reduceSkillProb;
#endif
	int attack_dam_melee;
	int attack_dam_range;
	int hitrate_skill;
	bool immune_blind;
	int attack80;
	int maxhp450;
	int	hard;
	/////////////////////////////////////////////
	// BANGWALL : 2005-07-18 오후 2:24:26
	// Comment : skill spd
	int skillspd;
	int statpall;
	// << 071226 kjban add
	int statpall_per;
	// >>
	int stronger_skill;
	int despair;
	int manascreen;
	int bless;
	int inverse_damage;
	int affinity;
	int affinity_rate;
	int exp;
	int sp;
	int exp_rate;
	int sp_rate;
#ifdef ASSIST_DECREASE_SKILL_MP
	int decreaseskillmp;
#endif

	int		npcAttack;					// NPC에만 물리/원거리 공격력 증가	/ PC공격시 미적용
	int		npcMagic;					// NPC에만 마법 공격력 증가		/ PC공격시 미적용
	int		SkillCooltime;

	// TODO : 하드코딩용
	bool hcExpPlus;
	bool hcSPPlus;
	int hcReflex;
	int hcReflexProb;			// addition에서만
	int hcDeathExpPlus;
	bool hcDeathSPPlus;
	MSG_CHAR_TYPE hcFearType;	// 피어
	int hcFearIndex;
	bool hcAttackTower;			// 석상 공격시 대미지 2배
	bool hcExpPlus_836;			// 수박
	bool hcSPPlus_837;			// 참외
	bool hcDropPlus_838;		// 자두
	bool hcSepDrop;				// 드롭률 상승
	bool hcSepExp;				// 경험치 상승
	bool hcSepSP;				// 스킬포인트 상승
	bool hcSepNas;				// 나스 상승
// >> [100823: selo] 유료 증폭제
	bool hcSepDrop_Cash;		// 드롭률 상승
	bool hcSepExp_Cash;			// 경험치 상승
	bool hcSepSP_Cash;			// 스킬포인트 상승
// << [100823: selo] 유료 증폭제
	bool hcScrollDrop;			// 행운의스크롤 : 드롭률 상승 // 강운의 스크롤과 변수를 같이 사용함
	bool hcScrollSP;			// 노력의스크롤 : SP 상승
	bool hcScrollExp;			// 경험의스크롤 : 경험치 상승
	bool hcExpPlus_S354;		// 경험치 포션
	bool hcScrollDrop_5081;
	int	hcDropPlus_Xmas2007;	// 크리스 마스 이벤트 드롭 상승률

	bool hcDropPlus_2141;		// 행운 주문서
	int hcCashExpUp;			// 060227 : bs : 유료아이템 경험치 상승
	int hcCashMaxHPUp;			// 060227 : bs : 유료아이템 HP 확장
	int hcCashMaxMPUp;			// 060227 : bs : 유료아이템 MP 확장

	bool hcExpPlus_S355;		// 추천서버포션 경험치 상승
	bool hcSPPlus_S356;			// 추천서버포션 SP 상승
	bool hcDropPlus_S360;		// 추천서버포션 드롭율 상승

	int hcExpPlusPer100;		// 경험치 상승 %단위 누적
	int hcSPPlusPer100;			// 숙련도 상승 %단위 누적
	int hcDropPlusPer100;		// 드롭율 상승 %단위 누적

	int hcExpPlus_398;			// 말레이시아 경험치 포션 : 유료 아이템 : 1840, 1841, 1842 : 단위는 %
	int hcExpPlus_1975;			// 케이크 : 2006 크리스마스
	int hcExpPlus_1976;			// 눈사람 : 2006 크리스마스
	int HitRate_2033;			// 럭키 소울 닉스  - 명중률 상승 20%
	int	Avoid_2034;				// 럭키 다지 피어 - 회피율 상승 10%

	bool hcCashPetExpUp_2358;	// 펫 경험의 묘약 - 전투 시간 3초당 2포인트 증가
	bool hcCashPetExpUp_2359;	// 플래티늄 펫 경험의 묘약 - 전투 시간 3초당 3포인트 증가

	bool bRorainOfLoveMagic;	// 로레인의 러브매직 :  일정 확률로 데미지 10배

	bool hcMPSteelPotion;		// MP 스틸 포션 : 일반 공격시 30% 확률로 상대방의 Max MP 중 10% 감소 나의 MP가 증가
	bool hcHPSteelPotion;		// HP 스틸 포션 : 일반 공격시 30% 확률로 상대방의 Max HP 중 5% 감소 나의 HP가 증가

	bool hcExpSPPlus_2354;

	int hcExpPlus_2388;
	int hcExpPlus_2389;
	int hcSPPlus_2390;
	bool hcDropPlus_2391;

	int hcEventHalloweenMaxHP;
	int hcEventHalloweenSpeed;

	int		charge_attack;
	int		charge_magic;
	bool	hcAttackBerserker;
	int		hcExplimitPlus;
	bool	hcPlatinumDrop;				// 드롭률 20배 상승
	bool	hcPlatinumScroll;			// 10분 동안 드롭률 4배상승

	int		hcLimitSP;					// 숙련도 개인 제한
	int		hcLimitEXP;					// 경험치 개인 제한
	int		hcRandomExpUp;				// 렌덤 경험치 4배 확률
	int		hcRandomSpUp;				// 렌덤 숙련도 4배 확률
	int		hcRandomDropUp;				// 렌덤 경험치 4배 확률
	// --- TODO : 하드코딩용
	bool	hcPetExpBoost;				// 펫 경험치 증폭
	bool	hcIrisFervor;				// 아이리스의 열정
#ifdef REFORM_PK_PENALTY_201108 // PK 패널티 리폼
	int		pkDispositionPointValue;	// pk 성향 수치
#endif
	int		tower_attack;

	int		war_reduce_melee_prob;			//공성전용 근접 데미지 흡수 확률
	int		war_reduce_range_prob;			//공성전용 원거리 데미지 흡수 확률
	int		war_reduce_magic_prob;			//공성전용 마법 데미지 흡수 확률

	int		war_reduce_melee;			//공성전용 근접 데미지 흡수
	int		war_reduce_range;			//공성전용 원거리 데미지 흡수
	int		war_reduce_magic;			//공성전용 마법 데미지 흡수
	int		war_max_hp;					//공성전용 최대 hp 증가
	int		war_defence;				//공성전용 방어력 증가
	int		war_resist;					//공성전용 마법 방어력 증가
	int		war_tower_attack;			//공성전용 공성 타워 공격력 일시증가

	int		money_buy;
	int		money_sell;
	int		money_nas;

	int		affinity_quest;
	int		affinity_monster;
	int		affinity_item;
	
	int		quest_exp;

	int		guild_party_exp;
	int		guild_party_sp;
} ASSISTVALUE;

/////////////////////
// Class name	    : CAssistData
// Description	    : 보조효과데이터
class CAssistData
{
public:
	int						m_index;		// 아이템의 경우 아이템 인덱스, 그외는 -1
	const CSkillProto*		m_proto;		// 스킬
	int						m_level;		// 레벨
	int						m_remain;		// 남은시간
	int						m_remainCount;	// remain proc count
	bool					m_bHit[MAX_SKILL_MAGIC];	// 스킬의 효과 적용 여부

	MSG_CHAR_TYPE			m_spellerType;	// 스킬 걸은 캐릭터 타입
	int						m_spellerIndex;	// 스킬 걸은 캐릭터 인덱스

	CAssistData*			m_prev;			// 링크
	CAssistData*			m_next;			// 링크
#ifdef ASSIST_DECREASE_TIME_BUG_FIX
	int						m_prevtime;
#endif
	//속성 스킬에 대한 변수 데이터
	int						attr_rand;

	CAssistData(MSG_CHAR_TYPE spellertype, int spellerindex, int itemidx, const CSkillProto* proto, int level, int remain,
				int remainCount,
				bool bHit[MAX_SKILL_MAGIC]);
};

/////////////////////
// Class name	    : CAssistList
// Description	    : 보조효과리스트
class CAssistList
{
	friend class CAssist;

public:
	CAssistList();
	~CAssistList();

public:
	CAssistData* getHead()
	{
		return m_head;
	}

private:
	CAssistData*			m_head;			// 헤더
	CAssistData*			m_tail;			// 테일
	int						m_max;			// 최대 수
	int						m_count;		// 개수
	int						m_abscount;		// 절대시간 버프 개수

	/////////////////////
	// Function name	: max
	// Description	    : 최대 수 설정
	void Max(int n);

	/////////////////////
	// Function name	: Add
	// Description	    : 리스트에 추가
	// Argument         : CCharacter* spellchar
	//                  : 스펠한 캐릭터
	// Argument         : int itemidx
	//                  : 아이템의 경우 인덱스, 그외는 -1
	// Argument         : const CSkillProto* proto
	//                  : 추가할 스킬
	// Argument         : int level
	//                  : 추가할 스킬의 레벨
	// Argument         : bool bHit[MAX_SKILL_MAGIC]
	//                  : 효과 적용 여부
	// Argument         : int& remain
	//                  : 남은 시간, -1은 스킬 자체 시간 사용
	// Argument         : int &remainCount
	//                  : remain proc trigger count. -1 use skill own count
	bool Add(CCharacter* spellchar, int itemidx, const CSkillProto* proto, int level, bool bHit[MAX_SKILL_MAGIC], int& remain,
			 int &remainCount,
			 int param, int nBlessAdd, int nBlessRate, int decreaseDBufTime);

	/////////////////////
	// Function name	: CanApply
	// Description	    : 스킬 적용 가능성 검사
	// Argument         : const CSkillProto* proto
	//                  : 스킬
	// Argument         : int level
	//                  : 레벨
	bool CanApply(const CSkillProto* proto, int level);

	/////////////////////
	// Function name	: DelDuplicate
	// Description	    : 중복되는 효과 제거
	// Argument         : const CSkillProto* proto
	//                  : 중복 검사 대상 스킬
	// Argument         : int level
	//                  : 레벨
	// Argument         : bool bSend
	//                  : 상태 변경을 메시지로 보낼것인지 결정
	// Argument         : bool bNoCancelType
	// 060317 : bs      : SF_NOCANCEL 검사 여부
	void DelDuplicate(const CSkillProto* proto, int level, bool bSend, CCharacter* ch, bool bNoCancelType);

	/////////////////////
	// Function name	: DecreaseTime
	// Description	    : 시간 감소
	// Return type		: bool
	//            		: 삭제되는 스킬이 있는지 여부
	bool DecreaseTime(CCharacter* ch, LONGLONG* changestate);

	////////////////////
	// Function name	: Apply
	// Description	    : 효과 적용
	// Argument         : ASSISTVALUE* addition
	//                  : 뎃셈으로 적용되는 수치
	// Argument         : ASSISTVALUE* rate
	//                  : 비율로 적용되는 수치
	// Argument         : int* state
	//                  : 상태 변화

	void Apply(CCharacter* ch, ASSISTVALUE* addition, ASSISTVALUE* rate, LONGLONG* state);

	/////////////////////
	// Function name	: DelAssist
	// Description	    : 보조효과 제거
	// Argument         : CAssistData* d
	//                  :
	// Argument         : bool bNoCancelType
	// 060317 : bs      : SF_NOCANCEL 검사 여부
	void DelAssist(CAssistData* d, bool bSend, CCharacter* ch, bool bNoCancelType, bool statusUpdate = true);

	////////////////////
	// Function name	: CheckApplyConditions
	// Description	    : 적용 조건 다시 검사
	void CheckApplyConditions(CCharacter* ch);
};

/////////////////////
// Class name	    : CAssist
// Description	    : 보조효과
class CAssist : public MemoryPoolBaseWithMutex<CAssist>
{
public:
	CCharacter*				m_ch;			// 효과적용하는 캐릭터
	CAssistList				m_help;			// 이로운 거
	CAssistList				m_curse;		// 해로운 거
	int						m_delaycheck;	// 검사 딜레이
	ASSISTVALUE				m_avAddition;	// 덧셈되는 효과 수치
	ASSISTVALUE				m_avRate;		// 비율로 적용되는 효과 수치

// 몹 상태 메시지
	LONGLONG				m_state;		// 상태 변화(스턴 등등)

	CAssist();

	/////////////////////
	// Function name	: Init
	// Description	    : 초기화
	void Init(CCharacter* ch);

	int GetAssistCount()
	{
		return m_help.m_count + m_curse.m_count + m_help.m_abscount;
	}

	int GetAssistHelpMax()
	{
		return m_help.m_max;
	}
	int GetAssistHelpCount()
	{
		return m_help.m_count;
	}

	int	GetAssistCurseCount()
	{
		return m_curse.m_count + m_curse.m_abscount;
	}

	/////////////////////
	// Function name	: Add
	// Description	    : 스킬 효과 추가
	// Argument         : CCharacter* spellchar
	//                  : 스펠한 캐릭터
	// Argument         : int itemidx
	//                  : 아이템의 경우 인덱스, 그외는 -1
	// Argument         : const CSkillProto* proto
	//                  : 스킬
	// Argument         : int level
	//                  : 레벨
	// Argument         : bool bHit[MAX_SKILL_MAGIC]
	//                  : 효과 적용 여부
	// Argument         : bool bSend
	//                  : 상태 변경을 메시지로 보낼것인지 결정
	// Argument         : int remain
	//                  : 남은 시간, -1은 스킬 자체 시간 사용
	// Argument         : int remainCount
	//                  : remain proc trigger count. -1 use skill own count
	bool Add(CCharacter* spellchar, int itemidx, const CSkillProto* proto, int level, bool bHit[MAX_SKILL_MAGIC], bool bSend, int remain,
			 int remainCount,
			 int param, int nBlessAdd, int nBlessRate);

	/////////////////////
	// Function name	: DecreaseTime
	// Description	    : 시간 감소
	bool DecreaseTime();

	/////////////////////
	// Function name	: Apply
	// Description	    : 효과 적용
	void Apply();

	////////////////////
	// Function name	: ClearAssist
	// Description	    : 효과 해제
	// Argument         : bool bNoCancelType
	// 060317 : bs      : SF_NOCANCEL 검사 여부
	void ClearAssist(bool bSend, bool bByStone, bool bHelp, bool bCurse, bool bNoCancelType);

	////////////////////
	// Function name	: Find
	// Description	    : 특정 효과가 적용되어 있는지 검사
	bool Find(int magicindex, int magiclevel);

	void GetListString(bool bHelp, char* item, char* index, char* level, char* remain,
					   char *remainCount,
					   char* hit0, char* hit1, char* hit2);

	void GetListString(bool bHelp, std::string& item, std::string& index, std::string& level, std::string& remain,
					   std::string& remainCount,
					   std::string& hit0, std::string& hit1, std::string& hit2);

	void AppendAssistToNetMsg(CNetMsg::SP& msg);
	void AppendAssistToNetStructMsg(struct tag_assistinfo& info);

	// 060227 : bs : 절대시간 사용 리스트
	////////////////////
	// Function name	: GetABSTimeTypeList
	// Description	    : 절대 시간 타입의 버프 리스트를 가져온다
	// Return type		: int
	//                  : 총 개수
	// Argument         : int* nAssistABSItemIndex
	//                  : 아이템 인덱스 버퍼, NULL이면 데이터는 없이 개수만 구한다
	// Argument         : int* nAssistABSSkillIndex = NULL
	//                  : 스킬 인덱스 버퍼
	// Argument         : int* nAssistABSSkillLevel = NULL
	//                  : 스킬 레벨 버퍼
	// Argument         : char* nAssistABSHit0 = NULL
	//                  : 명중 0 버퍼
	// Argument         : char* nAssistABSHit1 = NULL
	//                  : 명중 1 버퍼
	// Argument         : char* nAssistABSHit2 = NULL
	//                  : 명중 2 버퍼
	// Argument         : int* nAssistABSEndTime = NULL
	//                  : 종료시간 버퍼
	int GetABSTimeTypeList(int* nAssistABSItemIndex, int* nAssistABSSkillIndex = NULL, int* nAssistABSSkillLevel = NULL, char* nAssistABSHit0 = NULL, char* nAssistABSHit1 = NULL, char* nAssistABSHit2 = NULL, int* nAssistABSEndTime = NULL);

	////////////////////
	// Function name	: CancelSleep
	// Description	    : 슬립상태 취소
	void CancelSleep();

	////////////////////
	// Function name	: CureAssist
	// Description	    : level이하의 type에 해당하는 스킬 취소
	void CureAssist(int type, int level);
	void CureAssist(int flag);
	void CureOtherAssist(int type, int subtype, int level);			// ASSIST말고 다른 버프도 삭제 가능

	////////////////////
	// Function name	: CancelInvisible
	// Description	    : 인비상태 풀기
	void CancelInvisible();

	////////////////////
	// Function name	: CheckApplyConditions
	// Description	    : 적용 조건 다시 검사
	void CheckApplyConditions();

	/////////////////////
	// Function name	: CanApply
	// Description	    : 스킬 적용 가능성 검사
	// Argument         : const CSkillProto* proto
	//                  : 스킬
	// Argument         : int level
	//                  : 레벨
	bool CanApply(const CSkillProto* proto, int level);

	////////////////////
	// Function name	: FindByType
	// Description	    : 특정 타입의 효과를 찾음
	// Return type		: int
	//                  : 찾은 효과의 레벨, 못찾으면 0
	// Argument         : int type
	//                  : 효과 타입
	// Argument         : int subtype
	//                  : 효과 서브 타입
	// Argument         : bool* outHelp = NULL
	//                  : help 리스트에서 찾아지면 true, 아니면 false
	// Argument         : CAssistData** outData = NULL
	//                  : 찾은 효과에 해당하는 CAssistdata*를 저장
	int FindByType(int type, int subtype, bool* outHelp = NULL, CAssistData** outData = NULL);
	int GetSummonNpcIndex(); // 각 레벨별 트렙과 페러사이트 소환 npc index 를 가져온다.

	////////////////////
	// Function name	: CancelFear
	// Description	    : MST_ASSIST_FEAR 해제
	void CancelFear();

	////////////////////
	// Function name	: CancelFakeDeath
	// Description	    : MST_ASSIST_FAKEDEATH 해제
	void CancelFakeDeath();

	// 특정 아이템 인덱스에 의한 버프를 취소
	void CureByItemIndex(int itemindex);

	// 특정 아이템 인덱스에 의한 버프가 있는지 찾음
	bool FindByItemIndex(int itemindex, CAssistData** outData = NULL);

	// 특정 스킬인덱스로 버프 찾기, 스킬이 없으면 0, 있으면 스킬레벨 반환
	int FindBySkillIndex(int skillindex);

	// 특정 스킬 인덱스에 의한 버프를 취소
	void CureBySkillIndex(int nSkillIndex);

	// pet, apet 에 의한 버프를 취소
	void CureByPetSkill();

	void CancelDespair();
	void CancelManaScreen();
	void CancelBless();
	void CancelMantle();
	void DecreaseCount(int type, int subtype);

	unsigned char getAttrAtt();
	unsigned char getAttrDef();

private:

	/////////////////////
	// Function name	: DelDuplicate
	// Description	    : 중복되는 효과 제거
	// Argument         : const CSkillProto* proto
	//                  : 중복 검사 대상 스킬
	// Argument         : int level
	//                  : 레벨
	// Argument         : bool bSend
	//                  : 상태 변경을 메시지로 보낼것인지 결정
	// Argument         : bool bNoCancelType
	// 060317 : bs      : SF_NOCANCEL 검사 여부
	void DelDuplicate(const CSkillProto* proto, int level, bool bSend, bool bNoCancelType);
};

#endif //__ASSIST_H__
//
