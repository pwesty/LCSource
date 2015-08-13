#ifndef __ACHARACTER_H__
#define __ACHARACTER_H__

#include <math.h>
#include <map>
#include <vector>
#include <set>

#include "../ShareLib/MemoryPoolBase.h"
#include "NPCProto.h"

#include "APetProto.h"

#include "NPCRegenInfo.h"
#include "Item.h"
#include "CPos.h"
#include "ExchangeItems.h"
#include "MemPos.h"
#include "Party.h"
#include "Skill.h"
#include "Assist.h"
#include "Quest.h"
#include "SSkill.h"
#include "PersonalShop.h"
#include "Expedition.h"
#include "TimerItemManager.h"
#include "InventoryManager.h"
#include "WearInvenManager.h"
#include "GPSManager.h"

#include "Factory.h"            // 제작 시스템

#include "Affinity.h"

#include "TitleSystem.h"
#include "MonsterMercenary.h"

#include "RockPaperScissorsInfo.h"

#include "PetStashManager.h"

#include "SyndicateManager.h"
#include "ItemCollection.h"
#include "attendanceManager.h"

#ifdef PREMIUM_CHAR
#include "PremiumChar.h"
#endif

typedef struct __tagAutoGiveData
{
	int         nIndex;
	int         nPlus;
	int         nUsed;
	int         nFlag;
	LONGLONG    nCount;
	short       nOption[MAX_ITEM_OPTION];
	char        strSerial[MAX_SERIAL_LENGTH + 1];
} AUTO_GIVE_DATA;

typedef struct __tagBloodBugData
{
	int         nIndex;
	int         nFlag;
	int         nOptionCount;
	short       nOption[MAX_ITEM_OPTION];
	char        strSerial[MAX_SERIAL_LENGTH + 1];
} BLOOD_BUG_DATA;

typedef struct __tagMonsterComboData
{
	int         nIndex;
	int         nStep;
} MONSTER_COMBO_DATA;

typedef struct __tagSealSkillData
{
	int         nSkillIdx;
	CSkill *    pSkill;
	LONGLONG    llExp;
} SEAL_SKILL_DATA;

typedef std::set<CNPC*>	set_npc_t;
typedef std::vector<MONSTER_COMBO_DATA> vec_monster_combo_data_t;

class CDescriptor;
class CZone;
class CArea;
class CNetMsg;
class CGuild;
class CGuildMember;
class CWarCastle;
class CPet;
class CElemental;

class CAPet;

class CFriend;
class CFriendMember;

extern void DelAttackList(CCharacter* ch);
extern void DelRaList(CPC* pc, bool bForce = false);

#define SUMMON_NPC_MERCENARY_MAX	1		// 몬스터 용병
#define SUMMON_NPC_TOTEM_MAX		1		// 토텝형
#define SUMMON_NPC_BOMB_MAX			3		// 트랩형, 자폭형 같이 사용한다.
#define SUMMON_NPC_ITEM_TOTEM_MAX	1		// 아이템 토템형
#define SUMMON_NPC_MAX				SUMMON_NPC_TOTEM_MAX + SUMMON_NPC_MERCENARY_MAX + SUMMON_NPC_BOMB_MAX + SUMMON_NPC_ITEM_TOTEM_MAX

typedef enum _tagSummonNpcType
{
	SUMMON_NPC_TYPE_MERCENARY = 0,
	SUMMON_NPC_TYPE_TOTEM,
	SUMMON_NPC_TYPE_BOMB_1,	// 트랩, 자폭형.
	SUMMON_NPC_TYPE_BOMB_2,	// 트랩, 자폭형.
	SUMMON_NPC_TYPE_BOMB_3,	// 트랩, 자폭형.
	SUMMON_NPC_TYPE_TOTEM_ITEM,	//아이템을 이용한 소환 토템
} SUMMON_NPC_TYPE;

class CBlockPC
{
	int m_index;
	CLCString m_name;

public:
	CBlockPC(int index, const char* name) : m_name(MAX_CHAR_NAME_LENGTH +1)
	{
		m_index = index;
		m_name = name;
	}
	~CBlockPC() {};

	int GetIndex()
	{
		return m_index;
	}
	const char* GetName()
	{
		return m_name;
	}
	void SetName(const char* name)
	{
		m_name = name;
	}
};

class CQuickSlot
{
public:
	CQuickSlot()
	{
		clear();
	}

	// 슬롯 타입 : 액션, 스킬, 아이템 중 하나
	int     m_slotType[QUICKSLOT_MAXSLOT];

	// 퀵슬롯 내용
	int     m_skillType[QUICKSLOT_MAXSLOT]; // 스킬 타입
	int     m_actionType[QUICKSLOT_MAXSLOT];// 액션 타입
	CItem*  m_item[QUICKSLOT_MAXSLOT];      // 아이템 종류

	void clear()
	{
		memset(m_slotType, -1, sizeof(m_slotType));
		memset(m_skillType, -1, sizeof(m_skillType));
		memset(m_actionType, -1, sizeof(m_actionType));
		memset(m_item, 0, sizeof(m_item));
	}
};

// Attack List Class
class CAttackChar
{
public:
	CAttackChar()
	{
		ch = NULL;
		pc = NULL;
		npc = NULL;

		m_next = NULL;
		m_prev = NULL;

		m_damage = 0;
		m_targetHate = 0;
		m_targetPulse = 0;

		m_bGiveExpSP = false;
		m_bFirstAttack = false;
	}
	~CAttackChar()
	{
	}

	// pc
	int         m_damage;       // 데미지
	int         m_targetHate;   // 타겟 수치 : NPC가 공격시 참조 값
	// 타겟을 놓아주 : 멀어지거나 공격안하면 줄어

	int         m_targetPulse;  // 타겟 펄스 : 3분동안 캐릭터가 때리지 않으면 어택리스트에서 삭제
	bool        m_bGiveExpSP;   // 경험치와 SP를 받았는가
	bool        m_bFirstAttack; // ch가 먼저 때렸나?

	union
	{
		CCharacter* ch;
		CPC*        pc;
		CNPC*       npc;
	};

	CAttackChar*    m_next;
	CAttackChar*    m_prev;
};

// 정당방위 List Class
class CRaChar
{
public:
	CRaChar()
	{
		m_raTarget = NULL;
		m_raPulse = 0;
		m_bAttacker = false;
	}
	~CRaChar()
	{
	}

	CPC*        m_raTarget;
	int         m_raPulse;
	bool        m_bAttacker;    // 가해자 : true 피해자 false

	CRaChar*    m_next;
	CRaChar*    m_prev;
};

//////////////////
// Character class
class CCharacter
{
private:

	float   m_nFinalHitrate;            // 계산된 명중율  ( 반드시 계산완료가 보장되었을 때 사용 )
	float   m_nFinalAvoidrate;          // 계산된 회피율  ( 반드시 계산완료가 보장되었을 때 사용 )

public:
	typedef std::map<int/*index*/, GoldType_t/*nas*/> tradestash_t;

	CCharacter();
	virtual ~CCharacter();

	MSG_CHAR_TYPE   m_type;     // 종류

	// NPC, PC 공통 데이터
	int         m_index;        // 고유번호
	CLCString   m_name;         // 이름
	int         m_level;        // 렙

	LONGLONG    m_exp;          // 경험치

	LONGLONG	m_skillPoint;   // 스킬포인트

	int         m_hp;           // 체력
	int         m_maxHP;
	int         m_mp;           // 마나
	int         m_maxMP;

	int         m_str;          // 힘
	int         m_dex;          // 민첩
	int         m_int;          // 지혜
	int         m_con;          // 체질

	int         m_eqMelee;      // 무기 수치    : 근거리    : NUM0
	int         m_eqRange;      // 무기 수치    : 원거리    : NUM0 : IWEAPON_BOW
	int         m_eqMagic;      // 무기 수치    : 마법      : NUM1
	int         m_eqDefense;    // 방어구 수치  : 물리      : NUM0
	int         m_eqResist;     // 방어구 수치  : 마법      : NUM1

	char        m_attackSpeed;  // 공격속도 : 무기의 값에 따름 : 0.1초 단위 : 공격당 소요되는 시간 : +면 시간이 감소
	char        m_magicSpeed;   // 마법속도 : 마법 재사용 시간 줄고 늠: 0.1초 단위 : -면 시간이 감소
	int         m_skillSpeed;   // 스킬속도 : 스킬 시전 속도가 줄고 늠
	int			m_skillCooltime; // 0 ~ 100%
#ifdef BUGFIX_DEAD_AGAIN_DEAD
	time_t		m_deadTime;
#endif
	int         m_statPall;
	// << 071226 kjban add
	int         m_statPall_per;
	// >>

	float       m_walkSpeed;    // 걷기 속도
private:
	float       m_runSpeed;     // 뛰기 속도
public:
	float       m_flySpeed;     // 날기 속도
	float       m_attackRange;  // 공격거리

	int         m_recoverHP;    // 체력 회복
	int         m_recoverMP;    // 마나 회복
	int         m_recoverPulse; // 상태회복 pulse : HP, MP

	CPos        m_pos;          // 좌표 (x, z, h, r, yLayer)

	int         m_cellX;        // 셀 위치
	int         m_cellZ;

	CAssist     m_assist;       // 보조효과
	ASSISTVALUE m_avPassiveAddition;    // 패시브 효과 : add
	ASSISTVALUE m_avPassiveRate;        // 패시브 효과 : rate

	CAttackChar* m_attackList;  //  어택 리스트

	CCharacter* m_pCellPrev;    // 셀 리스트 링크
	CCharacter* m_pCellNext;
	CZone*      m_pZone;        // 존
	CArea*      m_pArea;        // 영역

	bool        m_bVisible;     // 보이고 안보이고

	unsigned char m_attratt;
	unsigned char m_attrdef;

	unsigned char m_attratt_by_item;	//아이템에 의한 속성
	unsigned char m_attrdef_by_item;	//아이템에 의한 속성

	CSkill*     m_currentSkill;     // 현재 사용중인 스킬
	CCharacter* m_linkSource;   // 나한테 링크 건 캐릭
	CCharacter* m_linkTarget;   // 나가 링크 건 캐릭

	////////////////////////////////
	// Artifact Value
	int			m_artiStr;			// 힘 증가
	int			m_artiDex;			// 민첩 증가
	int			m_artiInt;			// 지능 증가
	int			m_artiCon;			// 체질 증가
	int			m_artiExp;			// 경험치 증가
	int			m_artiGold;			// 골드획득 증가

	////////////////////////////////
	// Option Value
	int         m_opStr;            // 힘 증가
	int         m_opDex;            // 민첩 증가
	int         m_opInt;            // 지혜 증가
	int         m_opCon;            // 체질 증가

	int         m_opMelee;          // 근접 공격 증가
	int         m_opRange;          // 원격 공격 증가
	int         m_opMeleeHitRate;   // 근접 명중률 증가
	int         m_opRangeHitRate;   // 원격 명중률 증가

	int         m_opDMelee;         // 근접 공격 감소
	int         m_opDRange;         // 원격 공격 감소
	int         m_opMeleeAvoid;     // 근접 명중률 감소
	int         m_opRangeAvoid;     // 원격 명중률 감소

	int         m_opMagic;          // 마법 공격 증가
	int         m_opMagicHitRate;   // 마법 명중률 상승

	int         m_opResist;         // 마법 공격 감소
	int         m_opResistAvoid;    // 마법 명중률 감소

	int			m_opExpRate;		// EXP 증가
	int			m_opSPRate;			// SP 증가

	int			m_opStrong;			// 강인함 증가
	unsigned char m_opAttratt;
	unsigned char m_opAttrdef;

	// 공속 검사 관련
	int     m_hackAttackCount;      // 어택메세지 온 수 저장
	int     m_AttackServerTime;     // 공속에 따른 최대 어택 시간
	int     m_AttackClientTime;     // 클라이언트가 보낸 어택시간
	int     m_lastAttackPulse;      // 최근 공격 메시지 온 시간

	int     m_pulseDisable;         // 캐릭터 행동 불능 시간 설정 : 설정된 시간 이후로 행동 가능

#ifdef NEW_ACCERY_ADD
	////////////////////////////////////
	// 악세사리 경험치 sp 추가율
	LONGLONG    m_AddProb;
#endif //NEW_ACCERY_ADD
	int             cooltime_2142;          // 건강의 물약
	int             cooltime_2143;          // 지력의 물약

	int     m_cooltime_Competition; // 대전용 아이템

	int MobScroll;                      // 몹 소환서 사용 여부

	char    m_attacktype;
	char    m_attackcount;
	bool    ChekAttackType();           // 공격타입에따라서 스피드핵 검사 여부

	int		m_ep;						// attack Point
	int		m_maxEP;

	int		m_absorbPVPDamageRate;		// PVP시 데미지 흡수율
	int		m_decreaseDBufTimeRate;		// 디버프시간 감소율

	int		m_tradeAgentRegCount;		// 거래대행 등록 개수
	tradestash_t m_tradeAgentViewMap;	//거래대행 현재 페이지 정보
	tradestash_t m_tradeAgentViewMap2;	//거래대행 현재 페이지 정보

	bool		m_bFirstDoIt;			//무적모드에 대한 행동 여부 (true : 15초, false = 3분 으로 설정해서 사용)

	int			m_syndicateType;			// 결사대 인덱스

	void Move(float tx, float tz, float th, float tr, int ylayer)   // 이동
	{
		m_pos = CPos(tx, tz, th, tr, ylayer);
	}

	void InitPointsToMax()      // hp, mp, ep, st를 최대치로 설정
	{
		m_hp = m_maxHP;
		m_mp = m_maxMP;
	}

	void SetVisible();          // 보이고 안보이는 상태를 토글시킴

	/////////////////////
	// Function name    : SendDisappearAllInCell
	// Description      : 캐릭터 주변의 모든 캐릭터+아이템이 사라지도록 알림
	// Argument         : bool bSendOtherDisappear
	//                  : 다른 사람이 사라짐을 자신에게 알릴 것인지 여부
	void SendDisappearAllInCell(bool bSendOtherDisappear);

	//클라이언트 이펙트 출력을 하기위한 npc제거 패킷(소환 npc중 토템에서만 사용)
	void SendRemoveAllInCellForClientEffect(CPC* owner);

	////////////////////
	// Function name    : CanApplySkill
	// Description      : 스킬 적용 가능 검사
	bool CanApplySkill(const CSkillProto* proto, const CSkillLevelProto* levelproto);

	////////////////////
	// Function name    : CanMove
	// Description      : 이동 가능 검사
	virtual bool CanMove();

	////////////////////
	// Function name    : CanAttack
	// Description      : 공격 가능 검사
	virtual bool CanAttack();

	////////////////////
	// Function name    : CanSpell
	// Description      : 시전 가능 검사
	virtual bool CanSpell();

	////////////////////
	// Function name    : GetAttackLevel
	// Description      : 무기레벨 얻기
	virtual int GetAttackLevel() = 0;

	////////////////////
	// Function name    : GetDefenseLevel
	// Description      : 방어구 레벨 얻기
	virtual int GetDefenseLevel() = 0;

	////////////////////
	// Function name    : GetAttackType
	// Description      : 공격 타입 얻기, 스킬 프로토가 있으면 스킬의 타입, NULL이면 기본 타입
	virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto* proto) const = 0;

	////////////////////
	// Function name    : ApplyAssistData
	// Description      : assist value를 적용
	void ApplyAssistData(ASSISTVALUE* add, ASSISTVALUE* rate);

	////////////////////
	// Function name    : IsInPeaceZone
	// Description      : 피스존 안인지 검사, bBlockAsPeace이면 MAPATT_BLOCK을 피스존으로 간주
	bool IsInPeaceZone(bool bBlockAsPeace);

	////////////////////
	// Function name    : IsInRaidZone
	// Description      : 레이드 존인지 검사
	bool IsInRaidZone();

	////////////////////
	// Function name    : GetSize
	// Description      : 캐릭터 크기 반환
	virtual float GetSize() = 0;

	////////////////////
	// Function name    : GetUsingStat
	// Description      : 공격시 사용하는 스탯
	virtual int GetUsingStat() const = 0;

	bool IsInvisible()
	{
		return (m_assist.m_state & AST_INVISIBLE) ? true : false;
	}
	bool IsEnemy(CCharacter* tch);
	bool CanVisible(CCharacter* tch);

	void CancelInvisible();
	void CancelDamageLink();

	int GetDefenseLevelBonus() const
	{
		return (m_type == MSG_CHAR_PC) ? (m_level * 3) : 0;
	}
	virtual int GetResistLevelBonus() const
	{
		return (m_type == MSG_CHAR_PC) ? (m_level / 2) : 0;
	}
	int GetMeleeLevelBonus() const;
	int GetRangeLevelBonus() const;
	int GetMagicLevelBonus() const;

	void AddEP( int ep_ );

	bool IsBlind()
	{
		return (m_assist.m_state & AST_BLIND) ? true : false;
	}

	unsigned short GetMapAttr();

	virtual CSkill* FindSkill(int skillindex) = 0;

	// 패시브 스킬 적용
	virtual void ApplyPassive() = 0;
	void ApplyPassiveSkill(CSkill* skill, int param);

	// 스탯 재계산
	virtual void CalcStatus(bool bSend) = 0;

	// 공속 검사 : PC, 펫, Elemental
	// 핵으로 판단시 true
	bool CheckHackAttack(CPC* pPC);

	virtual float GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type) = 0;
	virtual float GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type) = 0;

	// 아래 함수는 회피 계산 후 사용 해야 합니다.  다른곳에 사용 금지...
	float   GetFinalHitRate()
	{
		return m_nFinalHitrate;
	}
	float   GetFinalAvoidRate()
	{
		return m_nFinalAvoidrate;
	}
	void    SetFinalHitRate( float FinalHitRate )
	{
		m_nFinalHitrate = (FinalHitRate<0 ? 0:FinalHitRate);
	}
	void    SetFinalAvoidRate( float FinalAvoidRate )
	{
		m_nFinalAvoidrate = (FinalAvoidRate<0 ? 0:FinalAvoidRate);
	}

	// 원정대 표식
	int		m_nExpedLabel;
	void	SetExpedLabel(int nType);
	int		GetExpedLabel();

	// disable 설정, 현재 설정이 더 오래가면 무시
	void SetDisableTime(int sec);
	// disable 상태 반환
	bool IsDisable();

	void SetRunSpeed(float speed)
	{
		( speed < PC_MIN_RUNSPEED )	? m_runSpeed = PC_MIN_RUNSPEED : m_runSpeed = speed;
	}
	float GetRunSpeed(void)
	{
		return m_runSpeed;
	}
	float GetHpPercentage(void)
	{
		if (m_maxHP == 0) return 0.0f;
		return ((float)m_hp / (float)m_maxHP) * 100;
	}

	void applyAuraSkill(CCharacter* ch, int& auraCount, int level, CAssistData* outData);

	void setSyndicateType(int syndicateType);
	int	getSyndicateType()
	{
		return m_syndicateType;
	}

	//아이템 스킬 쿨 타임 저장 컨테이너
private:
	std::map<int/*아이템 인덱스*/, int/*쿨 타임*/> m_itemCoolMap;
public:
	void addItemCoolTime(int item_index);
	bool checkItemCoolTime(int item_index, CSkill* skill);
};

///////////
// PC class
class PartyRecallInfo
{
public:
	PartyRecallInfo()
	{
		resetPartyRecallInfo();
	}
	~PartyRecallInfo() {};
	void	setPartyRecallInfo(int zoneID, CPos& pos, int guildIndex, char cIsInCastle)
	{
		zoneID_ = zoneID;
		pos_ = pos;
		guildIndex_ = guildIndex;
		cIsInCastle_ = cIsInCastle;
	}

	void	resetPartyRecallInfo()
	{
		zoneID_ = -1;
		pos_ = CPos(0, 0, 0, 0, 0);
		guildIndex_ = 0;
		cIsInCastle_ = 0;
	}
	int		getZoneID()
	{
		return zoneID_ ;
	}
	CPos*	getPos()
	{
		return &pos_;
	}
	int		getGuildIndex()
	{
		return guildIndex_;
	}
	char	getIsInCastle()
	{
		return cIsInCastle_;
	}

private:
	int		zoneID_;
	int		guildIndex_;
	char	cIsInCastle_;
	CPos	pos_;
};

typedef std::map<int, CBlockPC*> map_listblock_t;
class CPC : public CCharacter, public MemoryPoolBaseWithMutex<CPC>
{
	int             m_state;            // pc 상태

	int             m_joinMerac;        // 메라크 공성 참여여부

	int             m_joinDratan;

	int     m_pulseDisable;             // 캐릭터 행동 불능 시간 설정 : 설정된 시간 이후로 행동 가능

public:
	void reviseHPMP();
	bool AddExpUseGP(LONGLONG exp);
	void ApplyTitle();					// 호칭에 붙어 있는 옵션 적용 함수.
	void TitleCurrentSkillCure(int titleIndex = -1, bool bListDelete = true );  // 현제 적용 중인 타이틀 효과 제거
	void CastllanTitleDelete(int castleZoneindex, bool bExclude, char bCastellan = 0);

	CPC();
	virtual ~CPC();

	////////////
	// 기본 정보
#ifdef STASH_PASSWORD
	std::string		m_stash_password;			// 창고 패스워드
	std::string		m_a_cid;					// 태국 a_cid 7글자만
#endif

	TimerItemManager	m_TimerItem;	// 기간제 아이템 관리
	int     m_listIdx;          // 리스트에서 인덱스
	bool    m_bPlaying;         // 플레이 여부
	bool    m_bChangeStatus;    // SendStatus()를 할지 안할지 결정
	bool    m_bImmortal;        // 몹에게 공격 당하는가? 운영자 명령어로 셋팅
	bool    m_bChatMonitor;     // 채팅 모니터링 여부
	bool    m_bLoadChar;        // 캐릭터 로딩 여부
	CLCString m_nick;           // 별명(이름 변경카드)
	int     m_billReqTime;      // 빌링 아이템 지급 타임

	int     m_speedup;          // 운영자 명령어 : speedup
	int     m_autoSkillTime;
	int     m_SkillTime_511;
	int     m_cashBalance;      // 보유 캐쉬

	unsigned short m_recentAtt;  // 최근 위치 속성맵 값

#ifdef NO_CHATTING
	int     m_nflag;            // 캐릭터 flag - 채팅금지
#endif

	bool            m_bTradeAgentRegIng;    //현재 등록 처리중인가(거래대행)
	bool            m_bTradeAgentCalcIng;   //현재 정산 처리중인가(거래대행)
	bool			m_bTradeAgentBuyIng;	//현재 구매 처리중인가(거래대행)
	int             m_nCalcPageNo;          //정산(리스트)현재 페이지 번호

#ifdef RANKER_NOTICE
	bool    m_bRanker;          // 랭커 인가.
#endif

	int     m_pd3Count;         // 퍼스널던전3 입장 수
	int     m_pd3Time;          // 퍼스널던전3 마지막 입장 시간
	int     m_pd4Count;         // 퍼스널던전4 입장 수
	int     m_pd4Time;          // 퍼스널던전4 마지막 입장 시간

	int m_etcEvent;

#ifdef GMTOOL
	bool    m_bGmMonitor;           // 지엠툴 채팅 모니터링 여부
#endif // GMTOOL

	int     m_secretkey;
	/////////////////////////////////////////////
	// BANGWALL : 2005-06-27 오후 7:10:37
	// Comment :
	// pd4 시작 타임
	int m_pd4StartTime;

	LONGLONG m_loseexp;         // 사망 경험치 저장
	LONGLONG m_losesp;          // 사망 sp 저장

	CFriend* m_Friend;
	int m_nRegFriend;
	int m_nCondition;
	map_listblock_t  m_listBlockPC;

	////////////
	// 외양 정보
	unsigned char    m_job;              // 직업
	unsigned char    m_job2;             // 2차 직업

#ifdef ENABLE_SUBJOB
	int	m_jobSub;			// 부직업
	bool			IsSubJob( int subjob )
	{
		return  m_jobSub & subjob ? true : false ;
	}
#endif //ENABLE_SUBJOB

	char    m_hairstyle;        // 머리모양
	// 1의 자리 : 원래 헤러
	// 10의 자리 : 산타 모자
	char    m_facestyle;        // 얼굴모양

#ifdef GER_LOG
	//private:
	//	time_t m_loginTime;				// 로그인한 시간.

	//public:
	//	time_t GetLoginTime()
	//	{
	//		return m_loginTime;
	//	}
	//	void SetLoginTime();
	//	void GetCalcInGameTime(char *_buf, const int _size);
#endif // END : GER_LOG
	////////////
	// 스킬 정보
	CSkillList  m_activeSkillList;
	CSkillList  m_passiveSkillList;
	CSkillList  m_etcSkillList;
	CSkillList	m_optionAttSkillList;
	CSkillList	m_optionDefSkillList;

	CSkillList          m_sealSkillList;                            // 제작 스킬
	SEAL_SKILL_DATA     m_sealSkillExp[MAX_SEAL_TYPE_SKILL];        // 제작 스킬 숙련도

	//////////////////
	// Special Skill
	CSSkillList m_sSkillList;   // Special Skill List
	//////////////
	// 적용되는 값
	int     m_admin;            // 관리자 등급

// 050401 : bs : plus에 따른 확률 변수 추가 : PC만 사용
	int     m_plusStrong;
	int     m_plusHard;
	int     m_plusWeak;
// --- 050401 : bs : plus에 따른 확률 변수 추가

	///////////////////
	// 스탯 포인트 관련
	int     m_statpt_remain;    // 남은 수
	int     m_statpt_str;       // 힘
	int     m_statpt_dex;       // 민첩
	int     m_statpt_int;       // 지혜
	int     m_statpt_con;       // 체질

	////////////////////
	// Blood Point
	int     m_bloodPoint;       // Blood Point

	////////////////////
	// PC 전용 옵션 관련
	int         m_opSturnLevel;     // 레벨 : 스턴
	int         m_opBloodLevel;     // 레벨 : 블러드
	int         m_opPoisonLevel;    // 레벨 : 포이즌
	int         m_opSlowLevel;      // 레벨 : 슬로우
	int         m_opMoveLevel;      // 레벨 : 무브

	int         m_opSturnIndex;     // 스킬 번호 : 스턴
	int         m_opBloodIndex;     // 스킬 번호 : 블러드
	int         m_opPoisonIndex;    // 스킬 번호 : 포이즌
	int         m_opSlowIndex;      // 스킬 번호 : 슬로우
	int         m_opMoveIndex;      // 스킬 번호 : 무브

	bool        m_bCreateMixItem;   // 공성조합 아이템 생성 여부
//#endif

	int         m_opIncreaseInven;      // 최대 무게 증가 : 단위 %
	int         m_opMPSteal;            // 마나 흡수 : 단위 %
	int         m_opHPSteal;            // 생명력 흡수 : 단위 %
	int         m_opAttackBlind;        // 암흑 공격 415 : 단위 스킬 레벨
	int         m_opAttackPoison;       // 독 공격 414 : 단위 스킬 레벨
	int         m_opAttackCritical;     // 크리티컬 확률 증가 : 단위 %
	int         m_opAttackDeadly;       // 데들리 확률 증가 : 단위 %
	int         m_opRecoverHP;          // 체력회복 증가 : 단위 %
	int         m_opRecoverMP;          // 체력회복 증가 : 단위 %
	int         m_opDecreaseSkillDelay; // 스킬 쿨타임 감소 : 단위 %
	int         m_opDecreaseSkillMP;    // MP 소모량 감소 : 단위 %
	int         m_opResistStone;        // 스톤 내성 증가 : 단위 %
	int         m_opResistSturn;        // 스톤 내성 증가 : 단위 %-
	int         m_opResistSilent;       // 침묵 내성 증가 : 단위 %
	int         m_opBlocking;           // 데미지 절반 감소 (스킬 제외) : 단위 %
	int			m_opRecoverHP_NoRate;	// 체력회복 정수단위
	int			m_opRecoverMP_NoRate;	// 마나회복 정수단위

	int			m_opJewelElementPetHPUP;		//보석 옵션 소환수 및 펫 HP증가
	int			m_opJewelElementAttUP;		//보석 옵션 소환수 공격력 증가
	int			m_opJewelElementMagicAttUP;	//보석 옵션 소환수 마법공격력 증가
	//////////////
	// 저장되는 값
	int     m_dbStr;                    // 힘
	int     m_dbDex;                    // 민첩
	int     m_dbInt;                    // 지혜
	int     m_dbCon;                    // 체질

	int     m_dbHP;                     // 체력
	int     m_dbMP;                     // 마나
	int		m_tempHP;
	int		m_tempMP;

	////////////
	// 소켓 관련
	CDescriptor* m_desc;                // 디스크립터

	////////////
	// 시간 관련
	int     m_silencePulse;             // 채팅 금지 시간 셋팅 변수
	int     m_lastProducePulse;         // 최근 생산 메시지 온 시간
	int     m_hackProduceCount;         // 샌상메세지 온 수 저장
	int     m_ProduceClientTime;        // 클라이언트가 보낸 생산시간

#ifdef CHAT_BLOCK_USA
	int     m_shoutPulse;
#endif // CHAT_BLOCK_USA

	bool    m_bProcDisconnect;          // 접속 종료시 ProcDisconnect()를 호출 했는지 여부

	//////////////
	// 아이템 관련
	InventoryManager	m_inventory;	// 가방 + 계정 창고를 관리하는 클래스
	CWearInvenManager	m_wearInventory;

	bool        m_bChangeStash;         // 창고 변경 사항 있는지 검사
	CNetMsg     m_stashMsg;             // 창고 요청 메시지

	std::vector<AUTO_GIVE_DATA>     m_listAutoGive;     // 자동 지급한 아이템 정보
	std::vector<BLOOD_BUG_DATA>     m_listBloodBug;     // 업그레이드 불가 아이템에 옵션 붙는 버그 수정에 의해 옵션 제거된 아이템 정보

	////////////
	// 교환 관련
	CExchangeItems*     m_exchange;

	// 상점 관련
	CPersonalShop::SP	m_personalshop;

	/////////////////
	// 장소 기억 관련
	CMemPos             m_mempos;               // 기억된 장소 리스트
	int                 m_reqWarpTime;          // 순간이동 남은 시간
	int                 m_reqWarpType;          // 이동 방법(귀환/장소기억)
	int                 m_reqWarpData;          // 귀환시 존번호/장소기억시 슬롯번호

	int                 m_reqWarpTime_skill;          // 순간이동 남은 시간
	int					m_reqWarpType_skill;	// 스킬을 이용한 이동 방법
	int					m_reqWarpData_skill;	// 스킬을 이용한 귀환시 존번호

	float	m_Npc_Portal_x;
	float	m_Npc_Portal_z;
	char	m_Npc_Portal_y;
	void GoNpcPortalScroll();

	////////////
	// 파티 정보
	CParty*             m_party;
	PartyRecallInfo		m_partyRecallInfo;

	PartyRecallInfo* getPartyRecallInfo()
	{
		return & m_partyRecallInfo;
	}

	// 원정대 정보
	CExpedition*        m_Exped;

	int m_nJoinInzone_ZoneNo;                       //현재 인존 존 번호
	int m_nJoinInzone_RoomNo;                       //현재 인존 룸 번호
	int m_nJoinInzone_AreaNo;                       //현재 인존 Area 번호

	int m_nRaidBoxItemTime;											//레이드 박스 획득 시간 체크
	void SetRaidBoxItemTime(int nTime)
	{
		m_nRaidBoxItemTime = nTime;
	};	//레이드 박스 획득 시간 설정
	int  GetRaidBoxItemTime()
	{
		return m_nRaidBoxItemTime;
	};				//레이드 박스 획득 시간 획득

	//////////////
	// Quick Slot
	CQuickSlot      m_quickSlot[QUICKSLOT_PAGE_NUM];    // 퀵슬록 3페이지 소유
	void recalcByItem(CItem* pItem);

	//////////////
	// pc 상테관련
	int             m_pkPenalty;                // PK 성향
	int             m_pkCount;                  // PK 수
	int             m_pkRecoverPulse;           // 성향 수치 회복 남은 시간
	int             m_pkmodedelay;              // PKMODE 전환 딜레이
	int             m_pkPenaltyHP;              // HP 감소
	int             m_pkPenaltyMP;              // MP 감소
	int             m_pkRecoverNPCCount;        // 사냥 수

#ifdef RESTRICT_PVP_SKILL
	int             m_nRestrictPvPSkillDelaySec;    // 몹에 의한 타게팅 유지 유효 시간
#endif // RESTRICT_PVP_SKILL

	// 정당방위 관련
	CRaChar*        m_raList;

	///////////////////
	// 아이템 사용 관련
	int             m_recoverHPItemTime;        // HP 회복 남은 시간
	int             m_recoverHPItemValue;       // HP 초당 회복량
	int             m_recoverMPItemTime;        // HP 회복 남은 시간
	int             m_recoverMPItemValue;       // MP 초당 회복량

	///////////////////
	// 퀘스트 관련
	CQuestList      m_questList;                // 퀘스트 리스트
	int             m_nLastCollectRequest;      // 마지막 MSG_QUEST_COLLECT 받은 시간
	int             m_nCollectRequestNPCIndex;  // 마지막 MSG_QUEST_COLLECT 받은 NPC 인덱스
	int             m_nCollectRequestCount;     // 수집퀘스트 누적 수

	////////////
	// 길드 관련
	CGuildMember*   m_guildInfo;                // 길드 정보, 상위길드원이면서 길드장인 경우 자신이 길드 장인 길드 정보
	int             m_regGuild;
	int             m_guildoutdate;             // 길드 탈퇴 일

	////////////////
	// 균등파티시 아이템을 받은적이 있는가?
	bool            m_bGiveItem;

	////////////////////
	// 변신 관련
	int             m_changePulse;          // 변신 시간
	int             m_changeIndex;          // 변신 몹 인덱스

	////////////////////
	// 아이템 이펙트 옵션
	char            m_plusEffect;           // 아이템 플러스 이펙트 옵션

	/////////////////////
	// 사제 시스템

	// 사제 필요 정보
	int     m_teachIdx[TEACH_MAX_STUDENTS];     // 사제 인덱스
	int     m_teachLevel[TEACH_MAX_STUDENTS];   // 사제 레벨
	char    m_teachJob[TEACH_MAX_STUDENTS];     // 사제 직업
	char    m_teachJob2[TEACH_MAX_STUDENTS];    // 사제 직업
	char    m_teachName[TEACH_MAX_STUDENTS][MAX_CHAR_NAME_LENGTH + 1];// 사제 이름
	int     m_teachTime[TEACH_MAX_STUDENTS];    // 사제 성립된 시간 : sec

	// 공통정보
	int     m_teachWait;                // 신청중인 캐릭터의 인덱스
	char    m_teachType;                // none -1 teacher 0 student 1
	bool    m_bTeacher;                 // 서버의 선생리스트에 등록 여부

	int     m_fame;                     // 명성치
	int     m_cntTeachingStudent;       //0627
	int     m_cntCompleteStudent;
	int     m_cntFailStudent;
	int     m_superstone;               //받은 초고제 갯수
	int     m_guardian;                 //후견인 시스템으로 성장한 캐릭터인지 체크.
	bool    m_displayCanSstone;         // 초고제를 받을수 있는지를 display했는가?

//0502 kwon
	int m_nMoonStoneDigit;              //가장 많은 갯수의 문스톤 숫자
	int m_nMoonStoneSum;                //문스톤 숫자 갯수

#ifdef LACARETTE_SYSTEM // 임시로 저장할 넘들... ㅡㅡ;; 뭐 다른 좋은 방법은 없을깡?
	int	m_lacaretteCosArrNum;
	int m_lacaretteItemArrNum;
	int	m_lacaretteTokenArrNum;
	int	m_lacaretteRepeat;
#endif
	int m_nMSIndex;
	int m_nMSGiftIndex;

	///////////
	// 애완동물
	CPet*   m_petList;
	int     m_pulseLastAttackSkill;

	// pc가 공격 했을때 미스가 아닐때만 경험치 추가
	bool    m_bNotMiss;

	CElemental*     m_elementalList;        // 소환수 리스트
	int             m_pulseElemental[4];    // 각 소환수별 소환 시간

	int             m_pulseEvocation[2];    // 강신한 시간 : 헬운드, 엘레넨
	int	            m_evocationIndex;        // 강신 타입

	int             m_memposTime;           // 메모리 스크롤 확장 사용시간
	int             m_stashextTime;         // 창과 확장 사용시간

	// 2005 크리스마스 이벤트 변수
	int     m_pulseTakeEventXMas2005;

	// 2006 신년 이벤트
	int     m_nTimeEventNewYear2006;        // 사냥시간
	int     m_pulseEventNewYear2006;        // 최근사냥시간

	// 경험치 분배관련 추가 변수 : 051228 : BS
	int				m_nExpDamage;           // 누적 대미지

	int     m_nGoldenBallNoticeStatus;                  // 골든볼 이벤트 공지 받은 상태

	char    m_nEventGomdori2007Win;             // 누적 승수
	char    m_nEventGomdori2007Game;            // 현재 게임 가위바위보 시도 수
	char    m_nEventGomdori2007FirstLose;       // 처음 패 보상 가능 여부
	char    m_nEventGomdori2007FirstWin;        // 처음 1승 보상 가능 여부
	bool    m_bEventGomdori2007Start;           // 게임 시작 여부
	int     m_nEventGomdori2007Total;           // 게임 누적 수
	bool    m_bEventGomdori2007CanSelect;       // 선택 가능 여부
	int     m_bEventGomdori2007Drop;            // 드롭 여부

	CRockPaperScissorsInfo m_RockPaperScissorsInfo;
	void StartRockPaperScissors();
	bool CanRockPaperScissors();

	int		m_nAttendance_exp;						// 출석 경험치

	int     m_bEventIndependenceDay2007drop;

	int     m_GuildExp;                                             // 길드포인트로 전환될 경험치
	int     m_GuildFame;                                            // 길드 포인트로 전환될 명성치

	long        m_nRespondTime;                                     // 교감 시간 저장

	CAPet*      m_pApetlist;

	float m_runSpeed_org;
	char m_attackSpeed_org;
	int m_maxHP_org;
	int m_maxMP_org;

	int m_bPhoenix_Char;

	GM_GOZONE_TYPE m_gmGoZoneType;

	CFactoryList    m_listFactory;                                  // 제작 가능 아이템 list

	bool        m_bTaming_npc;                                      // 테이밍 스킬을 사용하고 있는지 여부
	CNPC*		m_Slave_npc;				                        // 테이밍한 몬스터
	int         m_nSoul_Count;                                      // 나이트쉐도우 흡수한 영혼
	CCharacter* m_Owners_target;
	CCharacter* GetOwners_target()
	{
		return m_Owners_target;
	}
	void        SetOwners_target(CCharacter* target)
	{
		m_Owners_target = target;
	}
	int         GetSoul_Count()
	{
		return m_nSoul_Count;
	}
	void        SetSoul_Count(int count)
	{
		m_nSoul_Count = count;
	}

	// 지속이 되는 스킬에 대한 처리
	char		m_skillTargetCount;									// 지속시간이 긴 스킬의 타겟 숫자
	char*		m_targettype;										// 지속시간이 긴 스킬의 타겟 타입 배열
	int*		m_targetindex;										// 지속시간이 긴 스킬의 타겟 인덱스 배열

	// Appear매세지를 보낼 때 스킬 지속 중이면 skillfireMsg를 보내는거 체크
	bool		m_bCheckAppear;

	set_npc_t	m_listTaming;
	void DeleteSlave( CNPC* npc );
	bool AddSlave( CNPC* npc);
	bool IsTaming()
	{
		return m_bTaming_npc;
	}


	CAffinityList	m_affinityList;

	int m_nCurrentTitle;											// 현재 머리위에 달고 다니는 호칭 인덱스 저장 변수
	CTitleList m_titleList;											// 캐릭터가 보유하고 있는 호칭 리스트

#ifdef XTRAP
	int			m_xtrapCheckPulse;			// XTrap 확인 주기
#endif // XTRAP

	int			m_newtutoComplete;

	int m_exp_weekly;

	CNPC* m_pSummnonNpc[SUMMON_NPC_MAX];
	CCharacter* m_SummonOwners_target;
	int			m_nBombSummonCnt; // 같이 사용한다.

	void        SetSummonOwners_target(CCharacter* target)
	{
		m_SummonOwners_target = target;
	}
	CCharacter* GetSummonOwners_target()
	{
		return  m_SummonOwners_target;
	}
	void		SetSummonNpc(SUMMON_NPC_TYPE summonType, CNPC* pNPC);
	CNPC*		GetSummonNpc(int index, int idNum);
	CNPC*		GetSummonNpc(SUMMON_NPC_TYPE summonType);
	CNPC*		GetSummonNpc(CNPC* pNPC);
	void		SummonNpcRemove(CNPC* pNpc, bool bAutoDelete = true);
	void		SummonNpcRemove(SUMMON_NPC_TYPE summonType, bool bAutoDelete = true);
	void		SummonNpcRemoveAll(bool bAutoDelete = true);
	int			GetSummonNpcType(CNPC* pNpc);
	int			GetBombSummonCont()
	{
		return m_nBombSummonCnt;
	}
	bool		SetBombSummonNPC( CNPC* pNPC );
	///////////////////////////////////////////////////////////////////
	// 함수 정의

	///////////////////
	// 캐릭터 정보 전달
	void SendStatus();                                                  // 상태 보내기
	void Send(CNetMsg::SP& msg);                                            // 메시지 보내기
	void SendQuickSlot();                                               // 퀵슬롯 전달

	void Restore();                                                     // 상태 회복

	bool AddExpSP(LONGLONG exp, int sp, bool bUseEvent, bool IsQuestExp = false, bool bNoEffect = false, bool bArtifact = false);                // + sp, + exp

	void AddExpSP_Direct( LONGLONG exp, LONGLONG sp );						// 다른효과 적용없이 경험치 적용
	void CalcLevelup();													// 현재 있는 exp 로 레벨업 검사 루틴돌기

	void LevelUp(bool bSendEffect);                                     // 레벨업 효과
	void InitStat();                                                    // 스탯을 레벨에서 부터 초기화
	void CalcStatus(bool bSend);                                        // 아이템, 스킬에 의한 스탯 변화

	bool InitSkill(CSkillList* skillList, bool isPassive = false);      // 스킬 초기화
	bool InitSSkill();                                                  // 특수 스킬 초기화

	void resetCoolTime(CSkillList* skillList);

	void RecoverPC();                                                   // 자동회복, HP MP
	void ApplyItemValue(bool bSend);
	void ApplyItemPlusValue(CItem* pItem, int NormalPlus);                              // plus별로 아이템에 적용되는 수치 변화 및 보너스 능력

	int ItemUpgradeFuckingFunction( int itemPlus, int itemLevel, int itemAttack );	// 2011 마스터 스톤 대비 인첸트 변경 공식

	int CanChange();

	const char* GetName();
	//{
	//    if( IsNick() )
	//        return m_nick;
	//    return m_name;
	//}

	void ChangeName(const char* name);

	bool IsNick();
	//{
	//    if (m_nick != NULL && strcmp(m_nick, "") != 0)
	//        return true;
	//    else
	//        return false;
	//}

	////////////////////
	// Function name    : SettingItemOption
	// Description      : 착용 아이템 옵션 셋팅
	// Return type      : int
	//                  : 옵션 세팅된 파트 번호
	int SettingItemOption();

	//////////////////////////////
	// 퀵슬롯에서 특정 아이템 제거
	void RemoveItemFromQuickSlot(CItem* item);

	//////////////////////////////
	// 퀵슬롯에서 특정 스킬 제거
	void RemoveSkillFromQuickSlot(CSkill* pSkill);
	void RemovePetSkillFromQuickSlot();
	void RemoveApetSkillFromQuickSlot();
	void RemoveAllGuildSkillFromQuickSlot();

	void ChangeQuickSlot(CItem* item, int itemSlotType);

	/////////////////////
	// Function name    : GoMemPos
	// Description      : 기억된 장소로 이동
	// Argument         : int slot
	//                  : 이동할 장소 기억 위치
	void GoMemPos(int slot);

	/////////////////////
	// Function name    : UseStatPoint
	// Description      : 스탯 포인트 사용
	// Return type      : bool
	//                  : 성공 여부
	// Argument         : MSG_STATPOINT_USE_TYPE type
	//                  : 변경할 스탯
	// Argument         : int* val
	//                  : 변화된 스탯 수치
	bool UseStatPoint(MSG_STATPOINT_USE_TYPE type, int* val);

	/////////////////////
	// Function name    : CanWarp
	// Description      : 워프 아이템을 사용할 수 있는지 검사
	// Return type      : int
	//                  : 사용할 수 있으면 0, 아니면 시스템 메시지
	int CanWarp();

	/////////////////////
	// Function name    : FindSkill
	// Description      : 스킬 찾기
	CSkill* FindSkill(int index);

	////////////////////
	// Function name    : GetAttackLevel
	// Description      : 무기 레벨 구하기
	virtual int GetAttackLevel();

	////////////////////
	// Function name    : GetDefenseLevel
	// Description      : 방어구 평균 레벨 구하기
	virtual int GetDefenseLevel();

	////////////////////
	// Function name    : GetAttackType
	// Description      : 기본 공격의 근접 원거리 마법 종류를 가려냄
	virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto* proto) const;

	////////////////////
	// Function name    : GetPKName
	// Description      : 성향에 따른 칭호를 반환
	// Return type      : int
	//                  : -5 부터 +5까지 순서대로 카오스리전,네크로나이트,아웃로,피스캐스터,머더러,일반,어벤저,라이트포터,가디언,세인트나이트,디바인프로텍터
	int GetPKName();

	////////////////////
	// Function name    : IsSetPlayerState
	// Description      : 상태 필드가 셋 되어 있는지 검사
	bool IsSetPlayerState(int checkstate)
	{
		return ((m_state & checkstate) ? true : false);
	}

	////////////////////
	// Function name    : SetPlayerState
	// Description      : 상태 필드 세팅
	void SetPlayerState(int checkstate)
	{
		m_state |= checkstate;
	}

	////////////////////
	// Function name    : ResetPlayerState
	// Description      : 상태 필드 리셋
	void ResetPlayerState(int checkstate)
	{
		if(checkstate == ~0)
		{
			//리셋을 시킬때 비행, 다크니스 상태는 토글 초기화를 해준다.(초기화 되는 스킬이 있고 안되는 스킬이 있어 고의적인 하드코딩이 들어감. 수정필요)
			if(IsSetPlayerState(PLAYER_STATE_FLYING))
			{
				toggleOff(FLY_SKILL, TOGGLE_SKILL);
			}
			if(IsSetPlayerState(PLAYER_STATE_DARKNESS))
			{
				toggleOff(DARKNESS_SKILL, TOGGLE_SKILL);
			}
		}
		
		m_state &= ~checkstate;
		return;
	}

	////////////////////
	// Function name    : TogglePlayerState
	// Description      : 상태 필드 토글
	void TogglePlayerState(int checkstate)
	{
		m_state ^= checkstate;
	}

	////////////////////
	// Function name    : GetPlayerState
	// Description      : 상태 반환
	int GetPlayerState()
	{
		return m_state;
	}

	////////////////////
	// Function name    : IsCombatMode
	// Description      : 전투 상태 검사, 최근 공격후 일정 시간(PULSE_ATTACKMODE_DELAY)이 지났는가
	bool IsCombatMode();

	////////////////////
	// Function name    : CanMove
	// Description      : 이동 가능 검사
	virtual bool CanMove();

	////////////////////
	// Function name    : CanAttack
	// Description      : 공격 가능 검사
	virtual bool CanAttack();

	////////////////////
	// Function name    : CanSpell
	// Description      : 시전 가능 검사
	virtual bool CanSpell();

	////////////////////
	// FN : GiveItem
	// 아이템 생성 / 지급
	bool GiveItem(int itemIndex, int plus, int flag, LONGLONG itemNum, bool bdrop = false , bool bOption = false );

	////////////////////
	// Function name    : GetSize
	// Description      : 캐릭터 크기 반환
	virtual float GetSize()
	{
		return 0.0f;
	}

	bool IsParty()
	{
		return (m_party && m_party->GetRequestIndex() != m_index && m_party->GetMemberCount() > 1) ? true : false;
	}

	bool	IsExped()
	{
		return (m_Exped && m_Exped->GetRequestIndex() != m_index && m_Exped->GetMemberCount() > 1) ? true : false;
	}

	virtual int GetUsingStat() const
	{
		if (m_wearInventory.wearItemInfo[WEARING_WEAPON]) return m_wearInventory.wearItemInfo[WEARING_WEAPON]->GetUsingStat();
		else return USING_STR;
	}

	void SetMessengerCondition(int condition)
	{
		m_nCondition = condition;
	}
	bool AddBlockPC(int charIndex, const char* name);
	void ReleaseBlockPC(int charIndex, CLCString& name);
	// 리스트의 이름을 최신으로 유지할수 없으므로 이즈 블럭을 호출할때 최신으로 유지한다
	bool IsBlockPC(int charIndex, const char* name );
	void GetBlockListString(CLCString& blockIndexList, CLCString& blockNameList);

	void do_QuestGiveUp(CPC* ch, CQuest* quest);//레벨업에 의한 퀘스트 강제 포기.

	bool CheckDungeonData(int zone);

	virtual int GetResistLevelBonus()
	{
		switch (m_job)
		{
		case JOB_MAGE:
		case JOB_HEALER:
		case JOB_SORCERER:
		case JOB_NIGHTSHADOW:
			return m_level;
		default:
			return m_level / 2;
		}
	}

	// 공성 참여 플래그 구하기
	int GetJoinFlag(int zoneindex);

	// 공성 참여 플래그 설정
	void SetJoinFlag(int zoneindex, int value);

#ifdef REFORM_PK_PENALTY_201108 // PK 패널티 리폼
	int m_pkPenaltyReward;
	void SetPKPenaltyReward(int titleNum)
	{
		m_pkPenaltyReward |= ( 1 << titleNum );
	}
	int  CheckPKPenaltyReward(int titleNum)
	{
		return m_pkPenaltyReward & ( 1 << titleNum );
	}
	int	 GetPKPenaltyRewardNum();

	int	m_pkDispositionRateValue;	// PK 성향 수치 상승 증폭률 %
	int	m_pkDispositionAddValue;	// PK 성향 수치 상승 증폭률 %

	bool IsChaotic()
	{
		return (m_pkPenalty < 0 ) ? true : false;    // 카오틱 캐릭터 판별
	}
	void AddPkPenalty(int penalty);
#else
	bool IsChaotic()
	{
		return (m_pkPenalty < -9) ? true : false;    // 카오틱 캐릭터 판별
	}
#endif

	// 펫의 배고픔/교감도/경험치 값을 변경한다
	void UpdatePetValue();
	// 펫 리스트에서 특정 INDEX의 펫을 찾는다
	CPet* GetPet(int index = 0);
	// 펫 삭제
	void DelPet(int petIndex);
//#endif

	// 펫의 배고픔/교감도/경험치 값을 변경한다
	void UpdateAPetValue();
	// 펫 리스트에서 특정 INDEX의 펫을 찾는다
	CAPet* GetAPet(int index = 0);          // 0 이면 현재 착용한 펫
	// 펫 삭제
	void DelAPet(int petIndex);

	// target에게 pvp 가능 여부 검사, 가능하하면 정당방위 설정
	bool CanPvP(CCharacter* target, bool bIgnoreInvisible);

	// 변신 해제
	void CancelChange();

	// 소환
	CElemental* SummonElemental(char elementalType);
	// 소환 해제
	void UnsummonElemental(CElemental* elemental);
	// 소환 가능 검사 : 중복, 동시소환, 소환시간
	bool CanSummonElemental(char newElementalType, char* duplicationType, int* nRemainTimeSummon, bool* errElementalType);
	// 소환수 얻기
	CElemental* GetElemental(int elementalindex);
	// 소환수 상태정보 보내기 : 변경된 것만
	void SendElementalStatus();

	// 강신
	void ChangeEvocationState(int skillIndex);
	// 강신 해제
	void Unevocation();

	void changeToggleState(int index, int toggle_type);
	void toggleOff(int index, int toggle_type);	
	void SendToggleState(int index, bool toggle, int toggle_type);

	void SendHolyWaterStateMsg(CItem* item);

	// 강신 남은 시간
	int GetRemainEvocation(bool bSec);
	// 패시브 스킬 적용
	void ApplyPassive();

	////////////////////
	// Function name    : ProcDisconnect
	// Description      : 존이동, 접속 종료 등에서 파티, 교환, 사제 등 처리
	// Argument         : bool bDisconnect
	//                  : 접속 종료 여부, 종료시 레이어 존에 있으면 시작 존으로 변경
	// Argument         : bool bGoto
	//                  : 같은 존내 이동에서 호출 된 경우
	void ProcDisconnect(bool bDisconnect, bool bGoto);

	// 자동 지급, 블러드옵션 버그 로그 남기기
	void OutputDBItemLog();

	virtual float GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type);
	virtual float GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type);

	void SetRespondTime(long time)
	{
		m_nRespondTime = time;
	}
	long GetRespondTime()
	{
		return m_nRespondTime;
	}

	vec_monster_combo_data_t	m_listSaveComboData;

	char m_bPressCorps;

	void ApplySetItemValue(CItem * pItem);
	void ApplyOriginOption(CItem * pItem);
	void RemoveSetItemValue(CItem * pItem);
	void CountSetItem(CItem* pItem, bool bPlus = true);

	int GetSealPos(int nSkillIdx)
	{
		for (int i = 0; i < MAX_SEAL_TYPE_SKILL; i++)
		{
			if (m_sealSkillExp[i].nSkillIdx == nSkillIdx)
				return i;
		}
		return -1;
	}

	LONGLONG GetSealExp(int nSkillIdx)
	{
		int nPos = GetSealPos(nSkillIdx);
		if (nPos > -1 && nPos < MAX_SEAL_TYPE_SKILL)
			return m_sealSkillExp[nPos].llExp;

		return 0;
	}

	void SetSealExp(int nSkillIdx, LONGLONG llExp)
	{
		int nPos = GetSealPos(nSkillIdx);
		if (nPos > -1 && nPos < MAX_SEAL_TYPE_SKILL)
			m_sealSkillExp[nPos].llExp = llExp;
	}

	void AddSealSkill(int nSkillIdx)
	{
		int nPos = -1;
		for (int i = 0; i < MAX_SEAL_TYPE_SKILL; i++)
		{
			if (m_sealSkillExp[i].nSkillIdx < 0)
			{
				nPos = i;
				break;
			}
		}

		if (nPos > -1 && nPos < MAX_SEAL_TYPE_SKILL)
		{
			m_sealSkillExp[nPos].nSkillIdx  = nSkillIdx;
			m_sealSkillExp[nPos].llExp      = 1;
		}
	}


// [101214: selo] 보상 아이템 드롭 수정
	bool CheckInvenForQuestPrize(const CQuestProto* pQuestProto, const int& optItemIndex);
	bool CheckInvenForProduceNoramlPrize(const int& itemdbIndex, const int& grade, const int& boost);
	bool CheckInvenForProduceRandomPrize();
	bool CheckInvenForProcessPrize(CItemProto* pItemProto);
	bool CheckInvenForTresureBoxPrize(CItem* pItem);
	bool CheckInvenForPrize(int nItemIndex, int plus, int flag, int count=1);
	bool CheckInvenForPrize(CItemProto* pItemProto, int plus, int flag, int count=1);

	void GuildGradeSkill( bool apply );
	void ApplyGuildGradeSkill()
	{
		GuildGradeSkill(true);
	}
	void CureGuildGradeSkill()
	{
		GuildGradeSkill(false);
	}

	bool	CheckCastellanType(int& zonenum, char& castellan);

// [110207:selo] 한 벌 의상
	const int GetSuitWearingPos(const int& pos)
	{
		return m_suitWearingPos[pos];
	}
	int m_suitWearingPos[COSTUME2_SUIT_MAX_WEARING];

private:
	void Clean();
	void CleanSummonNPC();
	void CleanTaming();
	void CleanParty();
	void CleanExchange();
	void CleanExtreamCube();

private:
	int m_nKillCount;
	int m_nTotalKillCount;
	int m_nDeathCount;
	int m_nTotalDeathCount;
	int m_nWarGroundPoint;
	int m_nWarGroundAccPoint;
public:
	int GetKillCount();
	int GetTotalKillCount();
	int GetDeathCount();
	int GetTotalDeathCount();
	int GetWarGroundPoint();
	int GetWarGroundAccPoint();

	void InitKillCount();
	void AddKillCount(int n);
	void SetTotalKillCount(int n);
	//void AddTotalKillCount(int n);
	void InitDeathCount();
	void AddDeathCount(int n);
	void SetTotalDeathCount(int n);
	//void AddTotalDeathCount(int n);
	void SetWarGroundPoint(int n);
	void AddWarGroundPoint(int n);
	void SetWarGroundAccPoint(int n);
	//void AddWarGroundAccPoint(int n);

public:
	CPetStashManager m_petStashManager;

public:
	bool m_bRockPaperScissorsFlag;
	void setRockPaperScissorsFlag();
	bool getRockPaperScissorsFlag();
#ifdef XTRAP
	char		m_xtrapSessionBuf[320];
	void xTrapSessionInit();
#endif

public:
	SyndicateManager  m_syndicateManager;
	bool isSyndicate();

#ifdef DURABILITY
	void calcDurability_for_weapon();
	void calcDurability_for_armor();
#endif

	bool m_first_inmap;
	
	typedef std::map<int, int> map_t;
	map_t itemSkill;

	CGPSManager m_gpsManager;
	ArtifactGPSManager m_arti_gpsManager;

	CPC* m_targetPC;				//내가 선택한 타겟의 인덱스

	//써치 라이프
private:
	bool m_isSearchLife;
public:
	void setSearchLife(bool isSearchLife) { m_isSearchLife = isSearchLife; }
	bool IsSearchLife() { return m_isSearchLife; }

public:
	
	CItem* holy_water_item;

	ItemCollectionManager m_itemCollectionManager;
	
	//운영자 명령어 변수
	bool	m_isNotCoolBoxItem;
	CAttendanceManager m_attendanceManager;

#ifdef PREMIUM_CHAR
public:
	PremiumChar m_premiumChar;
#endif

	//접속시 데이터 전송 유무
	bool m_firstSendData;
	//공성 스킬 체크 유무
	bool m_isWarSkillCheck;

	bool isWarZone();

	bool bMoveInsDunFinish;		// 인스턴스 던전 입장시 완전히 입장이 되었는지를 판단하기 위한 변수 (다시 만들지 않는 이상 필요함...)
};

////////////
// NPC class
class CNPC : public CCharacter, public MemoryPoolBaseWithMutex<CNPC>
{
public:
	CNPC();
	virtual ~CNPC();

	CNPCProto*  m_proto;        // DB 설정치

	int         m_idNum;        // 몹 아이디 번호(DB index)
	int         m_flag;         // Flag
	int         m_flag1;        // Flag1(확장 플래그)
	float       m_regenX;
	int         m_regenY;
	float       m_regenZ;
	int         m_disableTime;  // 리젠 이후 안 보이는 시간
	int         m_delay;        // 딜레이 공통

// 050322 : bs : 공격 전용 딜레이 변수 추가
	int         m_pulseAttack;  // 최근 공격 시간 : gserver->m_pulse와 직접 비교
// --- 050322 : bs : 공격 전용 딜레이 변수 추가

	char        m_attackType;   // 기본공격방법
	int         m_postregendelay;   // 리젠 이후 선공 하기 전까지의 시간 딜레이

	// 이동관련
	bool        m_bMoveRun;     // 뛰는가 true 걷는가 false
	int         m_nBlockRegen;  // 리젠위치로 가고 있는데 블럭당해서 못 간 경우 카운트
	// 전사의 축복에서 소유자 PC 인덱스
	bool        m_bStop;        // 서있는가
	int         m_moveArea;     // 이동 범위

	// AI 관련
	int         m_aipulse;      // NPC AI Pulse
	int         m_moveDir;      // 이동 방향 오른쪽 : 1 왼쪽 : -1 정지 : 0
	int         m_nBlockAI;     // AI 적용시 이동경로에서 블럭당한 수
	bool        m_bChangeDir;   // 이동뱡향이 바뀌어야 하는가
	bool        m_bFail45;      // 45도 꺾기가 실패인가

	int         m_quest[QUEST_MAX_NPC];         // 퀘스트 NPC 퀘스트 번호 저장
	int         m_nQuestCount;                  // 퀘스트 개수

	// 050221 : bs : NPC 스킬
	CSkill*     m_skills[MAX_NPC_SKILL];
	// --- 050221 : bs : NPC 스킬

	int         m_regenTime;	//#if defined(SYSTEM_TREASURE_MAP)

	int         m_regenTimeXmas2007;

	CNPC*       m_pNPCPrev;     // NPC 리스트 링크
	CNPC*       m_pNPCNext;     //

	CNPCRegenInfo* m_regenInfo;

	int         m_nRaidMoveTime;            // 마지막 전투 시간
	int         m_bRaidNPCSummon;           // 레이드 몹이 소환했는지 여부

	int			m_ctCount;					// 상태이상 면역 카운트 변수(보스와 준보스)
	int			m_ctTime;					// 상태이상 면역 회복 시간 변수

	int m_MobScrollType;
	int m_NextMobIndex;
	int m_UseCharIndex;

	int     m_coinidx;
	int     m_coincount;

#ifdef MONSTER_AI
	float   m_nOldDist;                     // 몬스터의 이동 전 위치
	bool    m_bMoveLock;                    // 몬스터 무적 상태
	int     m_pulseMoveLock;                // 무적 시간
	int     m_bMoveToRegen;                 // 리젠 위치로 이동 여부
#endif

	int     m_Mob_State;                    // 몬스터가 테이밍이 되었는가? 아니면 혼란인가?
	CPC*    m_owner;                        // 몬스터를 테이밍한 캐릭터
	int		m_pulseSoulRecover;				// 몬스터가 영혼을 빼앗긴 뒤 다시 생기는 딜레이
	int		m_lifetime;

	CPC*    GetOwner();
	void    SetOwner(CPC* pc)
	{
		m_owner = pc;    // 소유자 설정
	}

	int     Check_MobFlag(int mask) const
	{
		return m_Mob_State & mask;
	}
	void    Set_MobFlag(int state)
	{
		m_Mob_State |= state;
	}
	void    reSet_MobFlag(int state)
	{
		m_Mob_State &= ~state;
	}

	void        DeleteNPC();    // npc의 타켓리스트의 어택리스트에서 스스로를 삭제하고 메모리 해제
	void        InitAIVar();    // AI 관련 변수 초기화
	void        ResetStat();    // 프로토의 값으로 수치를 초기화

	////////////////////
	// Function name    : CanMove
	// Description      : 이동 가능 검사
	virtual bool CanMove();

	////////////////////
	// Function name    : CanAttack
	// Description      : 공격 가능 검사
	virtual bool CanAttack();

	////////////////////
	// Function name    : GetAttackLevel
	// Description      : 무기 레벨 구하기
	virtual int GetAttackLevel();

	////////////////////
	// Function name    : GetDefenseLevel
	// Description      : 방어구 평균 레벨 구하기
	virtual int GetDefenseLevel();

	////////////////////
	// Function name    : GetAttackType
	// Description      : 기본 공격의 근접 원거리 마법 종류를 가려냄
	virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto* proto) const;

	////////////////////
	// Function name    : GetSize
	// Description      : 캐릭터 크기 반환
	virtual float GetSize()
	{
		if (m_proto)
			return m_proto->m_bound;
		return 0;
	}

	virtual int GetUsingStat() const
	{
		if (GetAttackType(NULL) == MSG_DAMAGE_RANGE) return USING_DEX;
		else return USING_STR;
	}

	CSkill* FindSkill(int idx);

	// 패시브 스킬 적용 : NPC는 없음
	void ApplyPassive() {}

	// 스탯 재계산
	void CalcStatus(bool bSend);

	// 자신을 공격한 대상들이 준 데미지 총합을 반환
	LONGLONG GetTotalDamage();

	virtual float GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type);
	virtual float GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type);

#ifdef EXTREME_CUBE
	bool m_bCubeRegen;
#endif // EXTREME_CUBE

#ifdef MONSTER_AI
	float GetDistToRegen();                 // 리젠위치와 거리
#endif

	MonsterMercenaryData * m_pMercenaryClassData;
	void SetMercenaryClassData(MonsterMercenaryData * pClass)
	{
		m_pMercenaryClassData=pClass;
	}
	MonsterMercenaryData * GetMercenaryClassData()
	{
		return m_pMercenaryClassData;
	}

	int IsFlag(int npcflag)
	{
		return m_flag & npcflag;
	}
	int IsFlag1(int npcflag)
	{
		return m_flag1 & npcflag;
	}
	int GetFlag()
	{
		return m_flag;
	}
};

class CPet : public CCharacter, public MemoryPoolBaseWithMutex<CPet>
{
	friend class DBManager;
	friend class DBProcess;

	CPC*    m_owner;            // 펫 주인
	char    m_petTypeGrade;     // 펫 종류 + 등급

	int     m_hungry;           // 배고픔
	int     m_pulseHungry;      // 배고픔 펄스
	int     m_sympathy;         // 교감도
	int     m_pulseSympathy;    // 교감도 펄스
	int     m_abilityPoint;     // 기술 포인트

	int     m_pulseExpUp;       // 경험치 획득 펄스

	char    m_petColorType;
	int     m_nTurnToNpcIndex;
	int     m_nTurnToNpcSize;

	bool    m_bSummon;          // 소환되었는지 여부
	bool    m_bMount;           // 타고 있는지 여부
	bool    m_wasnear;          // 050309: bw 20미터 내에 있는지 밖에 있는지의 변화

	// 060221 : bs : 펫 사망시 일정시간 착용불능으로
	int     m_nRemainRebirth;   // 부활 남은 시간

	CSkillList m_skillList;     // 스킬 리스트

public:

	CPet*   m_prevPet;
	CPet*   m_nextPet;
	time_t	m_petStashSeconds;	// 펫 창고 펄스
	time_t	m_petStashHungrySeconds; // 펫 창고 배고픔 증가

	CPet(CPC* owner, int index, char petTypeGrade, int level);
	~CPet();

	////////////
	// 기본 정보
	void resetStashTimer();

	// 소유자 반환
	CPC* GetOwner()
	{
		return m_owner;
	}

	// 소유자 설정
	void SetOwner(CPC* pc)
	{
		m_owner = pc;
	}

	// 펫 종류 구하기
	char GetPetType()
	{
		return (m_petTypeGrade & PET_TYPE_MASK);
	}

	// 펫 등급 구하기
	char GetPetGrade()
	{
		return (m_petTypeGrade & PET_GRADE_MASK);
	}

	// 펫을 탈수 있는지 검사
	bool IsMountType()
	{
		return (GetPetGrade() == PET_GRADE_MOUNT);
	}

	// 펫 종류 + 등급 구하기
	char GetPetTypeGrade()
	{
		return m_petTypeGrade;
	}

	// 스탯을 레벨에 맞게 초기화 : HP/MP 등 수치 최대치 설정
	void InitStat();

	// 현재 착용되어 있는지 검사
	bool IsWearing()
	{
		return (m_owner && m_owner->m_wearInventory.wearItemInfo[WEARING_PET] != NULL && m_owner->m_wearInventory.wearItemInfo[WEARING_PET]->getPlus() == m_index);
	}

	// 현재 소환 되어 있는지 검사
	bool IsSummon()
	{
		return m_bSummon;
	}

	// 현재 타고 있는지 검사
	bool IsMount()
	{
		return m_bMount;
	}

	// 소환/타기 상태 리셋
	void ResetSummonMountFlag()
	{
		m_bSummon = false;
		m_bMount = false;
	}

	// 애완동물을 나타나게 함
	void Appear(bool bIncludeOwner);

	// 애완동물을 사라지게 함
	void Disappear();

	// 타고 내기리
	void Mount(bool bMount);

	// 060221 : bs : 펫 부활 남은 시간 설정
	void SetRemainRebirthTime(int nRemainRebirth = -1);

	// 060221 : bs : 펫 부활 남은 시간 반환
	int GetRemainRebirthTime()
	{
		return m_nRemainRebirth;
	}

	// 060309 : bw : 펫의 오너의 거리 구함
	void SetWasNear(bool bnear)
	{
		m_wasnear = bnear;
	}
	bool WasNear()
	{
		return m_wasnear;
	}
	float GetDistanceFromOwner();
	void ChangePetType();

	//////////////
	// 배고픔 관련

	// 행동 불능 검사
	bool IsActive()
	{
		return (m_hungry > 0);
	}

	// 배고픔 수치 반환
	int GetHungryPoint()
	{
		return m_hungry;
	}

	// 배고픔 수치 변경, 수치 변경시 true 반환
	bool DecreaseHungryPoint();

	// 1회에 배고픔 수치 떨어지는 양 구하기
	int GetUnitForHungry()
	{
		return (m_owner->m_level < m_level) ? (m_level - m_owner->m_level) / 2 + 1 : 1;
	}

	// 배고픔 수치 증가, 수치 변경시 true 반환
	bool IncreaseHungryPoint(int val);

	//////////////
	// 교감도 관련

	// 교감도 수치 반환
	int GetSympathyPoint()
	{
		return m_sympathy;
	}

	// 교감도 수치 변경, 수치 변경시 true 반환, 050309: bw 근처에 있지 않으면 감소만함
	bool UpdateSympathyPoint(bool bnear = false);

	// 교감도 수치 올라가는 단위 시간 구하기
	int GetUnitForSympathy()
	{
		return (m_owner->m_level < m_level) ? (m_level - m_owner->m_level) / 2 + PULSE_PET_SYMPATHY_INCREASE : PULSE_PET_SYMPATHY_INCREASE;
	}

	// 교감도 상승
	bool IncreaseSympathyPoint(int val);

	////////////
	// 성장 관련

	// 레벨업 필요 경험치 반환
	LONGLONG GetNeedExp()
	{
		if( m_level <= 100 )
			return (LONGLONG)(600 * pow(1.09, m_level));
		else
			return (LONGLONG)(65000 * pow(1.04, m_level));
	}

	// 경험치 증가, 수치 변경/레벨업 시에 true
	bool AddExp();

	void SetPetColor( char colorType = 0 )
	{
		m_petColorType = colorType;
	}
	char GetPetColor( void )
	{
		return m_petColorType;
	}

	void SetPetTurnToNpc( int npcIndex =0 )
	{
		m_nTurnToNpcIndex = npcIndex;
	}
	int GetPetTurnToNpc( void )
	{
		return m_nTurnToNpcIndex;
	}
	void SetPetTurnToNpcSize( int size = 10 )
	{
		m_nTurnToNpcSize = size;
	}
	int GetPetTurnToNpcSize( void )
	{
		return m_nTurnToNpcSize;
	}

	// 레벨업 처리 : 레벨 변경이후 처리, 레벨/경험치/AP 등은 변경후 호출해야 함
	void LevelUp();

	// 기술 포인트 반환
	int GetAbilityPoint()
	{
		return m_abilityPoint;
	}
	void SetAbilityPoint(int value)
	{
		m_abilityPoint = value;
	}


	// 기술 포인트 감소
	int DecreaseAbilityPoint(int val)
	{
		m_abilityPoint -= val;
		if (m_abilityPoint < 0) m_abilityPoint = 0;
		return m_abilityPoint;
	}

	// 기술 포인트 감소
	int IncreaseAbilityPoint(int val)
	{
		m_abilityPoint += val;
		return m_abilityPoint;
	}

	// 마운트 타입으로 변경
	void ChangeToMount()
	{
		m_petTypeGrade = GetPetType() | PET_GRADE_MOUNT;
		InitStat();
	}

	// 펫 마운트 타입 취소
	void ResetMountType()
	{
		m_petTypeGrade = GetPetType() | PET_GRADE_ADULT;
		InitStat();
	}

	////////////
	// 스킬 관련

	// 특정 인덱스 스킬 구하기
	CSkill* FindSkill(int skillindex);

	// 스킬 추가
	void AddSkill(CSkill*& skill);

	// 스킬 리스트 반환
	CSkillList* GetSkillList()
	{
		return &m_skillList;
	}

	// 스킬 초기화
	bool ResetSkill();

	/////////////////////////////
	// virtual function implement
	virtual int GetAttackLevel()
	{
		return 1;
	}
	virtual int GetDefenseLevel()
	{
		return 1;
	}
	virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto* proto) const
	{
		return MSG_DAMAGE_MELEE;
	}
	virtual float GetSize()
	{
		return 0.5f;
	}
	virtual int GetUsingStat() const
	{
		return USING_STR;
	}

	// 패시브 스킬 적용 : Pet은 없음
	void ApplyPassive() {}

	// 스탯 재계산 : Pet은 없음
	void CalcStatus(bool bSend) {}

	virtual float GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type);
	virtual float GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type);
};

class CElemental : public CCharacter, public MemoryPoolBaseWithMutex<CElemental>
{
	static int  m_nVirtualIndex;            // 가상 인덱스

	CPC*        m_owner;                    // 소유자

	char        m_elementalType;            // 소환수 종류

	int         m_defStr;                   // 기본 스탯
	int         m_defDex;
	int         m_defInt;
	int         m_defCon;

	int         m_defHP;                    // 기본 HP

	int         m_defAttackLevel;           // 기본 공방
	int         m_defDefenseLevel;

	int         m_pulseRemain;              // 소환 남은 시간

	CSkillList  m_skillList;                // 정령 스킬 리스트 (액티브/패시브)

public:
	bool        m_bChangeStatus;            // 상태변화여부

	CElemental* m_prevElemental;
	CElemental* m_nextElemental;

public:
	CElemental(CPC* owner, char elementalType);

	// 소유자 반환
	CPC* GetOwner()
	{
		return m_owner;
	}

	// 소환수 타입 반환
	char GetElementalType()
	{
		return m_elementalType;
	}

	// 소환수 남은 시간 반환
	int GetRemainTime()
	{
		return m_pulseRemain;
	}

	// 소환수 남은 시간 감소 : 소환 다되면 true 반환
	bool DecreaseRemainTime()
	{
		m_pulseRemain--;
		if (m_pulseRemain < 0) m_pulseRemain = 0;
		return (m_pulseRemain <= 0) ? true : false;
	}

	// 소환수 스킬 추가
	void AddSkill(int skillindex, int level);

	// 소환수 능력치 초기화 : 현재 hp 등은 유지
	void ResetStat();

	// 능력치 재계산
	void CalcStatus(bool bSend);

private:
	int GetVirtualIndex()
	{
		if (m_nVirtualIndex < 1) m_nVirtualIndex = 1;
		return CElemental::m_nVirtualIndex++;
	}

	// virtual functions
public:
	int GetAttackLevel()
	{
		return m_defAttackLevel;
	}
	int GetDefenseLevel()
	{
		return m_defDefenseLevel;
	}
	MSG_DAMAGE_TYPE GetAttackType(const CSkillProto* proto) const;
	float GetSize()
	{
		return 0.0f;
	}
	int GetUsingStat() const
	{
		return USING_DEX;
	}
	CSkill* FindSkill(int skillindex);
	void ApplyPassive();

	virtual float GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type);
	virtual float GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type);
};


typedef struct __tagApetAIData
{
	int     nItemNum;       // DB index
//  int     nItemIdx;       // Unique index
	CItem * pItem;
	int     nSkillIdx;
	int     nSkillType;     // skill - 0, action - 1
	char    cUse;
} APET_AI_DATA;

class CAPet : public CCharacter, public MemoryPoolBaseWithMutex<CAPet>
{
private:
	CPC* m_pOwner;

	int     m_nHpMpPulse;
	int     m_nStmPulse;
	int     m_nDStmPluse;
	int     m_nFaithPulse;
	int     m_nDFaithPulse;
	time_t	m_nPetStashSeconds;

	int m_nFaith;
	int m_nStm;

	LONGLONG	m_llAccExp;
	int			m_nRemainCooltime;

public:

	CAPet();
	~CAPet();

	// List
	CAPet*      m_pPrevPet;
	CAPet*      m_pNextPet;

	bool m_bSummon;         // 소환되었는지 여부

	int m_nSP;
	int m_nRemainStat;

	int m_nSeal;

	int m_nPlusStr;
	int m_nPlusCon;
	int m_nPlusDex;
	int m_nPlusInt;

	int m_nAttack;
	int m_nMagicAttack;
	int m_nDefence;
	int m_nMagicDefence;

	int m_nHitPoint;
	int m_nMagicHitPoint;
	int m_nAvoid;
	int m_nMagicAvoid;

	int m_nJewelAttack;
	int m_nJewelMagicAttack;
	int m_nJewelDefence;
	int m_nJewelMagicDefence;

	CAPetProto* m_pProto;
	CSkillList  m_skillList;            // Skill only  Active

	CSkillList m_passiveSkillList;

	CItem*      m_wearing[APET_WEARPOINT];

	char m_cAI;
	int  m_nAICount;
	int  m_nAISlot;
	APET_AI_DATA m_tAIData[APET_MAX_AI];

	int				m_nDelay;					// 재 착용 딜레이 남은시간
	unsigned int	m_nAccStemina;				// 레벨업할때 누적 채력
	unsigned int	m_nAccFaith;				// 레벨업할때 누적 충성도
	int				m_optTransEnd;				// 변신 끝나는 시점 조절 옵션값
	char			m_cTransSate;				// 변신 상태 0:일반 1:변신
	bool			m_bMount;					// 타기상태
	char			m_cSendTransMsg;			// 변신준비 메세지 알림 상황
	char			m_cForceTrans;				// 강제 변신 상태

	bool			TransFormationCheck();
	void			Mount( bool bMount );
	bool			IsMount()
	{
		return m_bMount;
	}

	// Function
	void	resetStashTimer();

	void    SetOwner(CPC* pc)
	{
		m_pOwner = pc;    // 소유자 설정
	}
	void    SetFaith(int faith)
	{
		m_nFaith = faith;    // 충성도 설정
	}
	void    SetStamina(int stm)
	{
		m_nStm = stm;    // 스테미너 설정
	}
	void    AddFaith(int faith); // 충성도 증가 ( 예외처리 )
	void    AddStamina(int stm); // 스테미너 증가 ( 예외처리 )

	CPC*    GetOwner()
	{
		return m_pOwner;    // 소유자 반환
	}
	int     GetFaith()
	{
		return m_nFaith;    // 충성도
	}
	int     GetStamina()
	{
		return m_nStm;    // 스테미너
	}
	int     GetStmLevel();                                  // 기아상태
	int     GetFaithLevel();                                // 충성도 상태

	bool    IsSummon()
	{
		return m_bSummon;    // 작용 소환 여부
	}

	CItem*  AddToWearItem( CItem* item );                   // Pet 방어구등 장착
	void    Appear(bool bIncludeOwner , bool bAction = false );
	void    Disappear();

	bool    IsWearing()
	{
		return ( m_pOwner && m_pOwner->m_wearInventory.wearItemInfo[WEARING_PET] != NULL && m_pOwner->m_wearInventory.wearItemInfo[WEARING_PET]->getPlus() == m_index);
	}
	bool    IsPetEquip()
	{
		return ( m_wearing[APET_WEAR_HEAD] || m_wearing[APET_WEAR_BODY] || m_wearing[APET_WEAR_WEAPON] || m_wearing[APET_WEAR_ACCE] )? true : false;
	}

	bool    IncreaseStat();         // 시간에 따라 증가되는 수치
	bool    DecreaseStat();         // 시간에 따라 감소되는 수치
	void    CheckItemTime();        // 펫장비 아이템 시간 확인

	void    UpdateStatPlus( int nStr, int nDex, int nInt , int nCon );

	LONGLONG    GetNeedExp()
	{
		if( m_level <= 100 )
			return (LONGLONG)(600 * pow(1.09, m_level));
		else
			return (LONGLONG)(65000 * pow(1.04, m_level));
	}

	bool        AddExpSP(LONGLONG exp, int sp, bool bDirect = false);
	void        LevelUp(bool bSendEffect);  // 레벨업 효과
	void		AddExpSP_Direct(LONGLONG _exp, int _sp);
	void		CalcLevelup();

	void        ApplyItemValue();           // 아이템 능력치 적용

	// Virtual Function
	////////////////////
	// Function name    : GetAttackLevel
	// Description      : 무기레벨 얻기.
	virtual int GetAttackLevel();

	////////////////////
	// Function name    : GetDefenseLevel
	// Description      : 방어구 레벨 얻기
	virtual int GetDefenseLevel();

	////////////////////
	// Function name    : GetAttackType
	// Description      : 공격 타입 얻기, 스킬 프로토가 있으면 스킬의 타입, NULL이면 기본 타입
	virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto* proto) const ;

	////////////////////
	// Function name    : GetSize
	// Description      : 캐릭터 크기 반환
	virtual float GetSize()
	{
		return 0;
	}

	////////////////////
	// Function name    : GetUsingStat
	// Description      : 공격시 사용하는 스탯
	virtual int GetUsingStat() const
	{
		if( m_wearing[WEARING_WEAPON]) return m_wearing[WEARING_WEAPON]->GetUsingStat();
		else return USING_STR;
	}

	virtual CSkill* FindSkill(int skillindex);
	// 패시브 스킬 적용
	void AddSkill( CSkill* pSkill );
	void ApplyPassive();
	// 스탯 재계산
	virtual void CalcStatus(bool bSend);

	// 진화 ( 계승할 부분 )
	void Evolution( CAPet* copy );

	void InitStat();

	virtual float GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type);
	virtual float GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type);

	bool IsAI()
	{
		return (m_cAI == '1') ? true:false;
	}
	bool IsEmptyAILine(int nIndex)
	{
		return (!m_tAIData[nIndex].pItem || m_tAIData[nIndex].nSkillIdx < 0) ? true:false;
	}

	bool DelAIList(CItem * pItem);
	void CheckAIList();

private:
	void		SendStatusMsg();

public:
	bool		IsProtoFlag(const int _flag);
	LONGLONG	GetAccExp();			// 현제 누적된 경험치
	void		SetAccExp(LONGLONG _accExp)
	{
		m_llAccExp = _accExp;    // 경험치 설정. 운영자 명령어에서만 사용
	}
	int			GetCooltime();				// 현제 남은 쿨타임 ( 초 )
	void		SetCooltime(int _min);		// 쿨타임 설정 ( 분 )
	void		ResetCooltime();

	double		GetAccRate(	int _petLevel );
	LONGLONG	GetMaxAccExp( int _petLevel, LONGLONG _charMaxExp );

	// 캐릭터가 먹는 최종 경험치, 캐릭터가 레벨업에 필요한 경험치
	void AddAccExp( LONGLONG _exp , LONGLONG _charLevelupExp );
	void UseAccExp( CPC* _pc );
	bool UseAccExp( CAPet* _apet, bool bDirect = false );

	// DB 에서 로딩하는 부분
	void	LoadCooltime(int _cooltime )
	{
		m_nRemainCooltime = _cooltime;
	}
	void	LoadAccExp(LONGLONG _accexp )
	{
		m_llAccExp = _accexp;
	}
};

void CheckQuestCondition(CPC* ch, CItem* item, LONGLONG itemSum);

#endif
//
