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

#include "Config.h"
#include "Character.h"
class CNetMsg;


typedef struct _tagAssistValue {
       int attack;
       int defense;
       int magic;
       int resist;
       int hitrate;
       int avoid;
       int critical;
       int attackspeed;
       int magicspeed;
       int movespeed;
       int recoverhp;
       int recovermp;
       int maxhp;
       int maxmp;
       int deadly;
       int attfire;
       int attwater;
       int attearth;
       int attwind;
       int attdark;
       int attlight;
       int magichitrate;
       int magicavoid;
       int attackdist;
       int reduceMelee;
       int reduceRange;
       int reduceMagic;
       int reduceMeleeProb;
       int reduceRangeProb;
       int reduceMagicProb;
       int attack_dam_melee;
       int attack_dam_range;
       int hitrate_skill;
       bool immune_blind;
       int attack80;
       int maxhp450;
       int hard;
       int skillspd;
       int statpall;
       int statpall_per;
       int stronger_skill;
       int despair;
       int manascreen;
       int bless;
       int inverse_damage;
       int exp;
       int sp;
       int exp_rate;
       int sp_rate;
       int npcAttack;
       int npcMagic;
       int SkillCooltime;
       bool hcExpPlus;
       bool hcSPPlus;
       int hcReflex;
       int hcReflexProb;
       int hcDeathExpPlus;
       bool hcDeathSPPlus;
       MSG_CHAR_TYPE hcFearType;
       int hcFearIndex;
       bool hcAttackTower;
       bool hcExpPlus_836;
       bool hcSPPlus_837;
       bool hcDropPlus_838;
       bool hcSepDrop;
       bool hcSepExp;
       bool hcSepSP;
       bool hcSepNas;
       bool hcSepDrop_Cash;
       bool hcSepExp_Cash;
       bool hcSepSP_Cash;
       bool hcScrollDrop;
       bool hcScrollSP;
       bool hcScrollExp;
       bool hcExpPlus_S354;
       bool hcScrollDrop_5081;
       int hcDropPlus_Xmas2007;
       bool hcDropPlus_2141;
       int hcCashExpUp;
       int hcCashMaxHPUp;
       int hcCashMaxMPUp;
       bool hcExpPlus_S355;
       bool hcSPPlus_S356;
       bool hcDropPlus_S360;
       int hcExpPlusPer100;
       int hcSPPlusPer100;
       int hcDropPlusPer100;
       int hcExpPlus_398;
       int hcExpPlus_1975;
       int hcExpPlus_1976;
       int HitRate_2033;
       int Avoid_2034;
       bool hcCashPetExpUp_2358;
       bool hcCashPetExpUp_2359;
       bool bRorainOfLoveMagic;
       bool hcMPSteelPotion;
       bool hcHPSteelPotion;
       bool hcExpSPPlus_2354;
       int hcExpPlus_2388;
       int hcExpPlus_2389;
       int hcSPPlus_2390;
       bool hcDropPlus_2391;
       int hcEventHalloweenMaxHP;
       int hcEventHalloweenSpeed;
       int charge_attack;
       int charge_magic;
       bool hcAttackBerserker;
       int hcExplimitPlus;
       bool hcPlatinumDrop;
       bool hcPlatinumScroll;
       int hcLimitSP;
       int hcLimitEXP;
       int hcRandomExpUp;
       int hcRandomSpUp;
       int hcRandomDropUp;
       bool hcPetExpBoost;
       bool hcIrisFervor;
       int pkDispositionPointValue;
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
	bool					m_bHit[MAX_SKILL_MAGIC];	// 스킬의 효과 적용 여부

	MSG_CHAR_TYPE			m_spellerType;	// 스킬 걸은 캐릭터 타입
	int						m_spellerIndex;	// 스킬 걸은 캐릭터 인덱스

	CAssistData*			m_prev;			// 링크
	CAssistData*			m_next;			// 링크

	CAssistData(MSG_CHAR_TYPE spellertype, int spellerindex, int itemidx, const CSkillProto* proto, int level, int remain, bool bHit[MAX_SKILL_MAGIC]);
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
	bool Add(CCharacter* spellchar, int itemidx, const CSkillProto* proto, int level, bool bHit[MAX_SKILL_MAGIC], int& remain, int param, int nBlessAdd, int nBlessRate);


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
	bool DecreaseTime(CCharacter* ch, int* changestate);


	////////////////////
	// Function name	: Apply
	// Description	    : 효과 적용
	// Argument         : ASSISTVALUE* addition
	//                  : 뎃셈으로 적용되는 수치
	// Argument         : ASSISTVALUE* rate
	//                  : 비율로 적용되는 수치
	// Argument         : int* state
	//                  : 상태 변화
	void Apply(CCharacter* ch, ASSISTVALUE* addition, ASSISTVALUE* rate, int* state);


	/////////////////////
	// Function name	: DelAssist
	// Description	    : 보조효과 제거
	// Argument         : CAssistData* d
	//                  : 
	// Argument         : bool bNoCancelType
	// 060317 : bs      : SF_NOCANCEL 검사 여부
	void DelAssist(CAssistData* d, bool bSend, CCharacter* ch, bool bNoCancelType);


	////////////////////
	// Function name	: CheckApplyConditions
	// Description	    : 적용 조건 다시 검사
	void CheckApplyConditions(CCharacter* ch);
};


/////////////////////
// Class name	    : CAssist
// Description	    : 보조효과
class CAssist
{
public:
	CCharacter*				m_ch;			// 효과적용하는 캐릭터
	CAssistList				m_help;			// 이로운 거
	CAssistList				m_curse;		// 해로운 거
	int						m_delaycheck;	// 검사 딜레이
	ASSISTVALUE				m_avAddition;	// 덧셈되는 효과 수치
	ASSISTVALUE				m_avRate;		// 비율로 적용되는 효과 수치
	LONGLONG				m_state;		// 상태 변화(스턴 등등)


	CAssist();


	/////////////////////
	// Function name	: Init
	// Description	    : 초기화
	void Init(CCharacter* ch);

	int GetAssistCount()	{ return m_help.m_count + m_curse.m_count + m_help.m_abscount; }

	int GetAssistHelpMax()		{ return m_help.m_max; }
	int GetAssistHelpCount()	{ return m_help.m_count; }


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
	bool Add(CCharacter* spellchar, int itemidx, const CSkillProto* proto, int level, bool bHit[MAX_SKILL_MAGIC], bool bSend, int remain, int param, int nBlessAdd, int nBlessRate);


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

	void GetListString(bool bHelp, char* item, char* index, char* level, char* remain, char* hit0, char* hit1, char* hit2);

	CNetMsg& AppendAssistToNetMsg(CNetMsg& msg);



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

	void CancelDespair();
	void CancelManaScreen();
	void CancelBless();
#ifdef ADULT_SERVER_NEWITEM
	void CancelMantle();
#endif // ADULT_SERVER_NEWITEM

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
