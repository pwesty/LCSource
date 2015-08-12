#ifndef __ACHARACTER_H__
#define __ACHARACTER_H__

#define NULL 0
#include "Config.h"
#include "LCList.h"
#include "MessageType.h"
#include "Title.h"
#include "NPCProto.h"
#include "APetProto.h"
#include "NPCRegenInfo.h"
#include "Affinity.h"
#include "Item.h"
#include "CPos.h"
#include "Inventory.h"
#include "ExchangeItems.h"
#include "MemPos.h"
#include "Party.h"
#include "Zone.h"
#include "Skill.h"
#include "Assist.h"
#include "Quest.h"
#include "SSkill.h"
#include "PersonalShop.h"
#include "Stash.h"
#include "NetMsg.h"
#include "Expedition.h"
#include "Guild.h"
#include "Factory.h"			// 제작 시스템
#include <vector>
#include <set>

typedef struct __tagAutoGiveData
{
	int			nIndex;
	int			nPlus;
	int			nUsed;
	int			nFlag;
	LONGLONG	nCount;
	short		nOption[MAX_ITEM_OPTION];
	char		strSerial[MAX_SERIAL_LENGTH + 1];
} AUTO_GIVE_DATA;

typedef struct __tagBloodBugData
{
	int			nIndex;
	int			nFlag;
	int			nOptionCount;
	short		nOption[MAX_ITEM_OPTION];
	char		strSerial[MAX_SERIAL_LENGTH + 1];
} BLOOD_BUG_DATA;

typedef struct __tagMonsterComboData
{
	int			nIndex;
	int			nStep;
}MONSTER_COMBO_DATA;


class CDescriptor;
class CNetMsg;
class CGuild;
class CGuildMember;
class CWarCastle;
class CPet;
class CElemental;
class CItem;
class CItemProto;
#ifdef ATTACK_PET
class CAPet;
#endif //ATTACK_PET

class CFriend;
class CFriendMember;

class CNPC;

#ifdef ALTERNATE_MERCHANT
class CAlternateMerchant;
#endif // ALTERNATE_MERCHANT

extern void DelAttackList(CCharacter* ch);
extern void DelRaList(CPC* pc, bool bForce = false);

class CRockPaperScissorsInfo {
   private:
     char m_nWinCount;
     char m_nGameCount;
     bool m_bStart;
     int m_nTotalCount;
     bool m_bCanPlay;

   public:
     CRockPaperScissorsInfo(void);
     ~CRockPaperScissorsInfo();
     void SetWinCount(char);
     char GetWinCount(void);
     void IncreaseWinCount(void);
     void SetGameCount(char);
     char GetGameCount(void);
     void IncreaseGameCount(void);
     void SetStart(bool);
     bool IsStart(void);
     void SetTotalCount(int);
     int GetTotalCount(void);
     void IncreaseTotalCount(void);
     void SetCanPlay(bool);
     bool CanPlay(void);
 };

class CBlockPC
{
	int m_index;
	CLCString m_name;

public:
	CBlockPC(int index, const char* name) : m_name(MAX_CHAR_NAME_LENGTH +1) { m_index = index; m_name = name; }
	~CBlockPC() {};

	int GetIndex() { return m_index; }
	const char* GetName() { return m_name; }
	void SetName(const char* name) { m_name = name; }

	static int CompByIndex(CBlockPC* p1, CBlockPC* p2)
	{
		return p1->GetIndex() - p2->GetIndex();
	}
};

class CQuickSlot
{
public:
	CQuickSlot()
	{
		memset(m_slotType, -1, sizeof(int) * QUICKSLOT_MAXSLOT);
		memset(m_skillType, -1, sizeof(int) * QUICKSLOT_MAXSLOT);
		memset(m_actionType, -1, sizeof(int) * QUICKSLOT_MAXSLOT);
		memset(m_item, 0, sizeof(CItem*) * QUICKSLOT_MAXSLOT);
	}

	// 슬롯 타입 : 액션, 스킬, 아이템 중 하나
	int		m_slotType[QUICKSLOT_MAXSLOT];

	// 퀵슬롯 내용
	int		m_skillType[QUICKSLOT_MAXSLOT];	// 스킬 타입
	int		m_actionType[QUICKSLOT_MAXSLOT];// 액션 타입
	CItem*	m_item[QUICKSLOT_MAXSLOT];		// 아이템 종류
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
	int			m_damage;		// 데미지
	int			m_targetHate;	// 타겟 수치 : NPC가 공격시 참조 값
								// 타겟을 놓아주 : 멀어지거나 공격안하면 줄어

	int			m_targetPulse;	// 타겟 펄스 : 3분동안 캐릭터가 때리지 않으면 어택리스트에서 삭제
	bool		m_bGiveExpSP;	// 경험치와 SP를 받았는가
	bool		m_bFirstAttack;	// ch가 먼저 때렸나?

	union
	{
		CCharacter*	ch;
		CPC*		pc;
		CNPC*		npc;
	};

	CAttackChar*	m_next;
	CAttackChar*	m_prev;

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

	CPC*		m_raTarget;
	int			m_raPulse;
	bool		m_bAttacker;	// 가해자 : true 피해자 false

	CRaChar*	m_next;
	CRaChar*	m_prev;
};

class CAttributeData {
       private:
         int m_water;
         int m_wind;
         int m_earth;
         int m_fire;
         int m_dark;
         int m_light;

       public:
         CAttributeData(void);
         void Reset(void);
         void Add(int, int, bool);
         int GetValue(int) const;
     };

typedef struct _tagMercenaryNpcInfo {
     int index;
     float walkSpeed;
     float runSpeed;
     float size;
   public:
     _tagMercenaryNpcInfo(void);
 } MercenaryNpcInfo;

typedef struct _tagMonsterMercenaryInfo {
    int nClassIdx;
    int nProb;
    int nAttDef;
    int nHpMp;
    int nHit;
    int nDodge;
    int nBonusMini;
    int nBonusMax;
    int nNpcCount;
    MercenaryNpcInfo *npcInfo;
  public:
    _tagMonsterMercenaryInfo(void);
} MonsterMercenaryInfo;

typedef struct _tagMonsterMercenaryData {
       int nClassIdx;
       int nAttDef;
       int nHpMp;
       int nHit;
       int nDodge;
       int nBonusAtt;
       int nBonusDef;
       float nWalkSpeed;
       float nRunSpeed;
       float nSize;
     public:
       _tagMonsterMercenaryData(void);
   } MonsterMercenaryData; 

class CMonsterMercenary {
   public:
     int m_count;
   protected:
     MonsterMercenaryInfo *m_Data;

   public:
     CMonsterMercenary(void);
     ~CMonsterMercenary();
     void Init(void);
     void ReleaseAll(void);
     bool LoadData(void);
     int GetMercenaryIndex(int);
     MonsterMercenaryInfo * GetClassInfo(void);
     MonsterMercenaryInfo * GetClassInfo(int);
     MonsterMercenaryData * CreateClassData(int, int);
     bool SetMercenaryData(CItem *, int, int);
     void GiveLowLevelSupportItem(CPC *);
 };

//////////////////
// Character class
class CCharacter {
   private:
     float m_nFinalHitrate;
     float m_nFinalAvoidrate;
   public:
     MSG_CHAR_TYPE m_type;
     int m_index;
     CLCString m_name;
     int m_level;
     LONGLONG m_exp;
     LONGLONG m_skillPoint;
     int m_hp;
     int m_maxHP;
     int m_mp;
     int m_maxMP;
     int m_str;
     int m_dex;
     int m_int;
     int m_con;
     int m_eqMelee;
     int m_eqRange;
     int m_eqMagic;
     int m_eqDefense;
     int m_eqResist;
     char m_attackSpeed;
     char m_magicSpeed;
     int m_skillSpeed;
     char m_skillCooltime;
     int m_statPall;
     int m_statPall_per;
     float m_walkSpeed;
   private:
     float m_runSpeed;
   public:
     float m_flySpeed;
     float m_attackRange;
     int m_recoverHP;
     int m_recoverMP;
     int m_recoverPulse;
     CPos m_pos;
     int m_cellX;
     int m_cellZ;
     CAssist m_assist;
     ASSISTVALUE m_avPassiveAddition;
     ASSISTVALUE m_avPassiveRate;
     CAttackChar *m_attackList;
     CCharacter *m_pCellPrev;
     CCharacter *m_pCellNext;
     CZone *m_pZone;
     CArea *m_pArea;
     bool m_bVisible;
     unsigned char m_attratt;
     unsigned char m_attrdef;
     CSkill *m_currentSkill;
     CCharacter *m_linkSource;
     CCharacter *m_linkTarget;
     int m_opStr;
     int m_opDex;
     int m_opInt;
     int m_opCon;
     int m_opMelee;
     int m_opRange;
     int m_opMeleeHitRate;
     int m_opRangeHitRate;
     int m_opDMelee;
     int m_opDRange;
     int m_opMeleeAvoid;
     int m_opRangeAvoid;
     int m_opMagic;
     int m_opMagicHitRate;
     int m_opResist;
     int m_opResistAvoid;
     int m_opExpRate;
     int m_opSPRate;
     unsigned char m_opAttratt;
     unsigned char m_opAttrdef;
     int m_hackAttackCount;
     int m_AttackServerTime;
     int m_AttackClientTime;
     int m_lastAttackPulse;
     int m_pulseDisable;
     int cooltime_2142;
     int cooltime_2143;
     int m_cooltime_Competition;
     int MobScroll;
     char m_attacktype;
     char m_attackcount;
     int m_ep;
     int m_maxEP;
     int m_absorbPVPDamageRate;
     int m_decreaseDBufTimeRate;
     int m_nExpedLabel;

     CCharacter(void);
     ~CCharacter();
     bool ChekAttackType(void);
     void Move(float, float, float, float, int);
     void InitPointsToMax(void);
     void SetVisible(void);
     void SendDisappearAllInCell(bool);
     bool CanApplySkill(const CSkillProto *, const CSkillLevelProto *);
     virtual bool CanMove(void);
     virtual bool CanAttack(void);
     virtual bool CanSpell(void);
     /*virtual int GetAttackLevel(void) const;
     virtual int GetDefenseLevel(void) const;*/
     //virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto *) const;
     void ApplyAssistData(ASSISTVALUE *, ASSISTVALUE *);
     bool IsInPeaceZone(bool);
     bool IsInRaidZone(void);
     //virtual float GetSize(void);
     //virtual int GetUsingStat(void) const;
     bool IsInvisible(void);
     bool IsEnemy(CCharacter *);
     bool CanVisible(CCharacter *);
     void CancelInvisible(void);
     void CancelDamageLink(void);
     //int GetDefenseLevelBonus(void) const;
     //virtual int GetResistLevelBonus(void) const;
     //int GetMeleeLevelBonus(void) const;
     //int GetRangeLevelBonus(void) const;
     //int GetMagicLevelBonus(void) const;
     void AddEP(int);
     bool IsBlind(void);
     unsigned char GetMapAttr(void);
     //virtual CSkill * FindSkill(int);
     //virtual void ApplyPassive(void);
     void ApplyPassiveSkill(CSkill *, int);
     //virtual void CalcStatus(bool);
     bool CheckHackAttack(CPC *);
     //virtual float GetHitrate(CCharacter *, _tagDamageType);
     //virtual float GetAvoid(CCharacter *, _tagDamageType);
     float GetFinalHitRate(void);
     float GetFinalAvoidRate(void);
     void SetFinalHitRate(float);
     void SetFinalAvoidRate(float);
     void SetExpedLabel(int);
     int GetExpedLabel(void);
     void SetDisableTime(int);
     bool IsDisable(void);
     void SetRunSpeed(float);
     float GetRunSpeed(void);
     float GetHpPercentage(void);
 };

class CPC : public CCharacter {
   private:
     int m_state;
     int m_joinMerac;
     int m_joinDratan;
     int m_pulseDisable;
   public:
     int m_listIdx;
     bool m_bPlaying;
     bool m_bChangeStatus;
     bool m_bImmortal;
     bool m_bChatMonitor;
     bool m_bLoadChar;
     CLCString m_nick;
     int m_billReqTime;
     int m_speedup;
     int m_autoSkillTime;
     int m_SkillTime_511;
     int m_cashBalance;
     unsigned char m_recentAtt;
     int m_nflag;
     bool m_bTradeAgentRegIng;
     bool m_bTradeAgentCalcIng;
     bool m_bTradeAgentBuyIng;
     int m_nCalcPageNo;
     int m_pd3Count;
     int m_pd3Time;
     int m_pd4Count;
     int m_pd4Time;
     int m_etcEvent;
     bool m_bGmMonitor;
     int m_secretkey;
     int m_pd4StartTime;
     LONGLONG m_loseexp;
     LONGLONG m_losesp;
     CFriend *m_Friend;
     int m_nRegFriend;
     int m_nCondition;
     std::map<int, CBlockPC*, std::less<int>, std::allocator<std::pair<int const, CBlockPC*> > > m_listBlockPC;
     unsigned char m_job;
     unsigned char m_job2;
     char m_hairstyle;
     char m_facestyle;
     CSkillList m_activeSkillList;
     CSkillList m_passiveSkillList;
     CSkillList m_etcSkillList;
     CSkillList m_optionAttSkillList;
     CSkillList m_optionDefSkillList;
     CSkillList m_sealSkillList;
     SEAL_SKILL_DATA m_sealSkillExp[10];
     CSSkillList m_sSkillList;
     int m_weight;
     int m_maxWeight;
     int m_admin;
     int m_plusStrong;
     int m_plusHard;
     int m_plusWeak;
     int m_statpt_remain;
     int m_statpt_str;
     int m_statpt_dex;
     int m_statpt_int;
     int m_statpt_con;
     int m_bloodPoint;
     int m_opSturnLevel;
     int m_opBloodLevel;
     int m_opPoisonLevel;
     int m_opSlowLevel;
     int m_opMoveLevel;
     int m_opSturnIndex;
     int m_opBloodIndex;
     int m_opPoisonIndex;
     int m_opSlowIndex;
     int m_opMoveIndex;
     bool m_bCreateMixItem;
     int m_opIncreaseInven;
     int m_opMPSteal;
     int m_opHPSteal;
     int m_opAttackBlind;
     int m_opAttackPoison;
     int m_opAttackCritical;
     int m_opAttackDeadly;
     int m_opRecoverHP;
     int m_opRecoverMP;
     int m_opDecreaseSkillDelay;
     int m_opDecreaseSkillMP;
     int m_opResistStone;
     int m_opResistSturn;
     int m_opResistSilent;
     int m_opBlocking;
     int m_opRecoverHP_NoRate;
     int m_opRecoverMP_NoRate;
     int m_opJewelElementHPUP;
     int m_opJewelElementAttUP;
     int m_opJewelElementMagicAttUP;
     int m_dbStr;
     int m_dbDex;
     int m_dbInt;
     int m_dbCon;
     int m_dbHP;
     int m_dbMP;
     int m_tempHP;
     int m_tempMP;
     CDescriptor *m_desc;
     int m_lastupdate;
     int m_autochknum[2];
     int m_silencePulse;
     int m_lastProducePulse;
     int m_hackProduceCount;
     int m_ProduceClientTime;
     int m_perSecondPulse;
     int m_shoutPulse;
     bool m_bProcDisconnect;
     CInventory m_invenNormal;
     CInventory m_invenQuest;
     CInventory m_invenEvent;
     CStash m_stash;
     bool m_bChangeStash;
     CNetMsg m_stashMsg;
     GoldType_t m_moneyItem;
     CItem *m_wearing[28];
     CItem *m_wearing_suit;
     std::vector<__tagAutoGiveData, std::allocator<__tagAutoGiveData> > m_listAutoGive;
     std::vector<__tagBloodBugData, std::allocator<__tagBloodBugData> > m_listBloodBug;
     CExchangeItems *m_exchange;
     CPersonalShop *m_personalshop;
     CMemPos m_mempos;
     int m_reqWarpTime;
     int m_reqWarpType;
     int m_reqWarpData;
     float m_Npc_Portal_x;
     float m_Npc_Portal_z;
     char m_Npc_Portal_y;
     CParty *m_party;
     CExpedition *m_Exped;
     int m_nJoinInzone_ZoneNo;
     int m_nJoinInzone_RoomNo;
     int m_nJoinInzone_AreaNo;
     int m_nRaidBoxItemTime;
     CQuickSlot m_quickSlot[3];
     int m_pkPenalty;
     int m_pkCount;
     int m_pkRecoverPulse;
     int m_pkmodedelay;
     int m_pkPenaltyHP;
     int m_pkPenaltyMP;
     int m_pkRecoverNPCCount;
     CRaChar *m_raList;
     int m_recoverHPItemTime;
     int m_recoverHPItemValue;
     int m_recoverMPItemTime;
     int m_recoverMPItemValue;
     CQuestList m_questList;
     int m_nLastCollectRequest;
     int m_nCollectRequestNPCIndex;
     int m_nCollectRequestCount;
     CGuildMember *m_guildInfo;
     int m_regGuild;
     int m_guildoutdate;
     bool m_bGiveItem;
     int m_changePulse;
     int m_changeIndex;
     char m_plusEffect;
     int m_teachIdx[10];
     int m_teachLevel[10];
     char m_teachJob[10];
     char m_teachJob2[10];
     char m_teachName[10][51];
     int m_teachTime[10];
     int m_teachWait;
     char m_teachType;
     bool m_bTeacher;
     int m_fame;
     int m_cntTeachingStudent;
     int m_cntCompleteStudent;
     int m_cntFailStudent;
     int m_superstone;
     int m_guardian;
     bool m_displayCanSstone;
     int m_nMoonStoneDigit;
     int m_nMoonStoneSum;
     int m_lacaretteCosArrNum;
     int m_lacaretteItemArrNum;
     int m_lacaretteTokenArrNum;
     int m_lacaretteRepeat;
     int m_nMSIndex;
     int m_nMSGiftIndex;
     CPet *m_petList;
     int m_pulseLastAttackSkill;
     bool m_bNotMiss;
     CElemental *m_elementalList;
     int m_pulseElemental[4];
     int m_pulseEvocation[2];
     char m_evocationType;
     int m_memposTime;
     int m_stashextTime;
     int m_pulseTakeEventXMas2005;
     int m_nTimeEventNewYear2006;
     int m_pulseEventNewYear2006;
     int m_nExpDamage;
     int m_nGoldenBallNoticeStatus;
     char m_nEventGomdori2007Win;
     char m_nEventGomdori2007Game;
     char m_nEventGomdori2007FirstLose;
     char m_nEventGomdori2007FirstWin;
     bool m_bEventGomdori2007Start;
     int m_nEventGomdori2007Total;
     bool m_bEventGomdori2007CanSelect;
     int m_bEventGomdori2007Drop;
     CRockPaperScissorsInfo m_RockPaperScissorsInfo;
     int m_nAttendance_exp;
     int m_bEventIndependenceDay2007drop;
     int m_GuildExp;
     int m_GuildFame;
     long m_nRespondTime;
     CAPet *m_pApetlist;
     float m_runSpeed_org;
     char m_attackSpeed_org;
     int m_maxHP_org;
     int m_maxMP_org;
     int m_bPhoenix_Char;
     GM_GOZONE_TYPE m_gmGoZoneType;
     CFactoryList m_listFactory;
     bool m_bTaming_npc;
     CNPC *m_Slave_npc;
     int m_nSoul_Count;
     CCharacter *m_Owners_target;
     char m_skillTargetCount;
     char *m_targettype;
     int *m_targetindex;
     bool m_bCheckAppear;
     std::set<CNPC*, std::less<CNPC*>, std::allocator<CNPC*> > set_npc_t;
     CAffinityList m_affinityList;
     int m_nCurrentTitle;
     CTitleList m_titleList;
     int m_newtutoComplete;
     int m_exp_weekly;
     CNPC *m_pSummnonNpc[5];
     CCharacter *m_SummonOwners_target;
     int m_nBombSummonCnt;
     int m_pkPenaltyReward;
     int m_pkDispositionRateValue;
     int m_pkDispositionAddValue;
     std::vector<__tagMonsterComboData, std::allocator<__tagMonsterComboData>> m_listSaveComboData;
     char m_bPressCorps;
     int m_suitWearingPos[5];
   private:
     int m_nKillCount;
     int m_nTotalKillCount;
     int m_nDeathCount;
     int m_nTotalDeathCount;
     int m_nWarGroundPoint;
     int m_nWarGroundAccPoint;

   public:
     CPC(void);
     void reviseHPMP(void);
     bool AddExpUseGP(long long);
     void ApplyTitle(void);
     void TitleCurrentSkillCure(int, bool);
     void CastllanTitleDelete(int, bool, char);
     ~CPC();
     void GoNpcPortalScroll(void);
     void SetRaidBoxItemTime(int);
     int GetRaidBoxItemTime(void);
     GoldType_t GetMoney(void);
     void StartRockPaperScissors(void);
     bool CanRockPaperScissors(void);
     CCharacter * GetOwners_target(void);
     void SetOwners_target(CCharacter *);
     int GetSoul_Count(void);
     void SetSoul_Count(int);
     void DeleteSlave(CNPC *);
     bool AddSlave(CNPC *);
     bool IsTaming(void);
     void SetSummonOwners_target(CCharacter *);
     CCharacter * GetSummonOwners_target(void);
     void SetSummonNpc(_tagSummonNpcType, CNPC *);
     CNPC * GetSummonNpc(int, int);
     CNPC * GetSummonNpc(_tagSummonNpcType);
     CNPC * GetSummonNpc(CNPC *);
     void SummonNpcRemove(CNPC *, bool);
     void SummonNpcRemove(_tagSummonNpcType, bool);
     void SummonNpcRemoveAll(bool);
     int GetSummonNpcType(CNPC *);
     int GetBombSummonCont(void);
     bool SetBombSummonNPC(CNPC *);
     void SendStatus(void);
     void SendInventory(char);
     void Send(CNetMsg &);
     void SendQuickSlot(void);
     void Restore(void);
     bool IncreaseMoney(long long);
     bool DecreaseMoney(long long);
     bool AddExpSP(long long, int, bool, bool, bool);
     void AddExpSP_Direct(long long, long long);
     void CalcLevelup(void);
     void LevelUp(bool);
     void InitStat(void);
     virtual void CalcStatus(bool);
     bool InitSkill(CSkillList *, bool);
     bool InitSSkill(void);
     void RecoverPC(void);
     void ApplyItemValue(bool);
     void ApplyItemPlusValue(CItem *, int);
     int ItemUpgradeFuckingFunction(int, int, int);
     int CanChange(void);
     const char * GetName(void);
     void ChangeName(const char *);
     bool IsNick(void);
     int SettingItemOption(void);
     void RemoveItemFromQuickSlot(CItem *);
     void RemoveSkillFromQuickSlot(CSkill *);
     bool CheckWeight(long long, bool, bool, bool);
     void SetMaxWeight(bool);
     void GoMemPos(int);
     bool UseStatPoint(_tagStatPointUseType, int *);
     int CanWarp(void);
     virtual CSkill * FindSkill(int);
     //virtual int GetAttackLevel(void) const;
     //virtual int GetDefenseLevel(void) const;
     //virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto *) const;
     int GetPKName(void);
     bool IsSetPlayerState(int);
     void SetPlayerState(int);
     void ResetPlayerState(int);
     void TogglePlayerState(int);
     int GetPlayerState(void);
     bool IsCombatMode(void);
     /*virtual bool CanMove(void);
     virtual bool CanAttack(void);
     virtual bool CanSpell(void);*/
     bool GiveItem(int, int, int, long long, bool, bool);
     //virtual float GetSize(void);
     bool IsParty(void);
     void SaveNow(bool);
     bool IsExped(void);
     //virtual int GetUsingStat(void) const;
     void SetMessengerCondition(int);
     bool AddBlockPC(int, const char *);
     void ReleaseBlockPC(int, CLCString &);
     bool IsBlockPC(int, const char *);
     void GetBlockListString(CLCString &, CLCString &);
     void do_QuestGiveUp(CPC *, CQuest *);
     bool CheckDungeonData(int);
     //virtual int GetResistLevelBonus(void);
     int GetJoinFlag(int);
     void SetJoinFlag(int, int);
     void SetPKPenaltyReward(int);
     int CheckPKPenaltyReward(int);
     int GetPKPenaltyRewardNum(void);
     bool IsChaotic(void);
     void AddPkPenalty(int);
     void UpdatePetValue(void);
     CPet * GetPet(int);
     void DelPet(int);
     void UpdateAPetValue(int);
     CAPet * GetAPet(int);
     void DelAPet(int);
     bool CanPvP(CCharacter *, bool);
     void CancelChange(void);
     CElemental * SummonElemental(char);
     void UnsummonElemental(CElemental *);
     bool CanSummonElemental(char, char *, int *, bool *);
     CElemental * GetElemental(int);
     void SendElementalStatus(void);
     void Evocation(int);
     void Unevocation(void);
     int GetRemainEvocation(bool);
     //virtual void ApplyPassive(void);
     void ProcDisconnect(bool, bool);
     void OutputDBItemLog(void);
     /*virtual float GetHitrate(CCharacter *, _tagDamageType);
     virtual float GetAvoid(CCharacter *, _tagDamageType);*/
     void SetRespondTime(long);
     long GetRespondTime(void);
     void ApplySetItemValue(CItem *);
     void ApplyOriginOption(CItem *);
     void RemoveSetItemValue(CItem *);
     void CountSetItem(CItem *, bool);
     int GetSealPos(int);
     LONGLONG GetSealExp(int);
     void SetSealExp(int, long long);
     void AddSealSkill(int);
     bool CheckInvenForQuestPrize(const CQuestProto *, const int &);
     bool CheckInvenForProduceNoramlPrize(const int &, const int &, const int &);
     bool CheckInvenForProduceRandomPrize(void);
     bool CheckInvenForProcessPrize(CItemProto *);
     bool CheckInvenForTresureBoxPrize(CItem *);
     bool CheckInvenForTresureBoxRedPrize(int **, int);
     bool CheckInvenForPrize(int, int, int, int);
     bool CheckInvenForPrize(CItemProto *, int, int, int);
     void GuildGradeSkill(bool);
     void ApplyGuildGradeSkill(void);
     void CureGuildGradeSkill(void);
     bool CheckCastellanType(int &, char &);
     void SettingCostumeSuit(CItem *);
     void DressCostumeSuit(CItem *);
     void UndressCostumeSuit(void);
     void UndressCostumeAll(void);
     const int GetSuitWearingPos(const int &);
   private:
     void Clean(void);
     void CleanSummonNPC(void);
     void CleanTaming(void);
     void CleanParty(void);
     void CleanExchange(void);
     void CleanExtreamCube(void);
   public:
     int GetKillCount(void);
     int GetTotalKillCount(void);
     int GetDeathCount(void);
     int GetTotalDeathCount(void);
     int GetWarGroundPoint(void);
     int GetWarGroundAccPoint(void);
     void InitKillCount(void);
     void AddKillCount(int);
     void SetTotalKillCount(int);
     void InitDeathCount(void);
     void AddDeathCount(int);
     void SetTotalDeathCount(int);
     void SetWarGroundPoint(int);
     void AddWarGroundPoint(int);
     void SetWarGroundAccPoint(int);
     void writeDebugString(char *);
 };
 /*
class CPC : public CCharacter {
       private:
         int m_state;
         int m_joinMerac;
         int m_joinDratan;
         int m_pulseDisable;
       public:
         int m_listIdx;
         bool m_bPlaying;
         bool m_bChangeStatus;
         bool m_bImmortal;
         bool m_bChatMonitor;
         bool m_bLoadChar;
         CLCString m_nick;
         int m_billReqTime;
         int m_speedup;
         int m_autoSkillTime;
         int m_SkillTime_511;
         int m_cashBalance;
         unsigned char m_recentAtt;
         int m_nflag;
         bool m_bTradeAgentRegIng;
         bool m_bTradeAgentCalcIng;
         bool m_bTradeAgentBuyIng;
         int m_nCalcPageNo;
         int m_pd3Count;
         int m_pd3Time;
         int m_pd4Count;
         int m_pd4Time;
         int m_etcEvent;
         bool m_bGmMonitor;
         int m_secretkey;
         int m_pd4StartTime;
         LONGLONG m_loseexp;
         LONGLONG m_losesp;
         CFriend *m_Friend;
         int m_nRegFriend;
         int m_nCondition;
         //map_listblock_t m_listBlockPC;
		 std::map<int, CBlockPC*, std::less<int>, std::allocator<std::pair<int const, CBlockPC*> > > m_listBlockPC;
         unsigned char m_job;
         unsigned char m_job2;
         char m_hairstyle;
         char m_facestyle;
         CSkillList m_activeSkillList;
         CSkillList m_passiveSkillList;
         CSkillList m_etcSkillList;
         CSkillList m_optionAttSkillList;
         CSkillList m_optionDefSkillList;
         CSkillList m_sealSkillList;
         SEAL_SKILL_DATA m_sealSkillExp[10];
         CSSkillList m_sSkillList;
         int m_weight;
         int m_maxWeight;
         int m_admin;
         int m_plusStrong;
         int m_plusHard;
         int m_plusWeak;
         int m_statpt_remain;
         int m_statpt_str;
         int m_statpt_dex;
         int m_statpt_int;
         int m_statpt_con;
         int m_bloodPoint;
         int m_opSturnLevel;
         int m_opBloodLevel;
         int m_opPoisonLevel;
         int m_opSlowLevel;
         int m_opMoveLevel;
         int m_opSturnIndex;
         int m_opBloodIndex;
         int m_opPoisonIndex;
         int m_opSlowIndex;
         int m_opMoveIndex;
         bool m_bCreateMixItem;
         int m_opIncreaseInven;
         int m_opMPSteal;
         int m_opHPSteal;
         int m_opAttackBlind;
         int m_opAttackPoison;
         int m_opAttackCritical;
         int m_opAttackDeadly;
         int m_opRecoverHP;
         int m_opRecoverMP;
         int m_opDecreaseSkillDelay;
         int m_opDecreaseSkillMP;
         int m_opResistStone;
         int m_opResistSturn;
         int m_opResistSilent;
         int m_opBlocking;
         int m_opRecoverHP_NoRate;
         int m_opRecoverMP_NoRate;
         int m_opJewelElementHPUP;
         int m_opJewelElementAttUP;
         int m_opJewelElementMagicAttUP;
         int m_dbStr;
         int m_dbDex;
         int m_dbInt;
         int m_dbCon;
         int m_dbHP;
         int m_dbMP;
         int m_tempHP;
         int m_tempMP;
         CDescriptor *m_desc;
         int m_lastupdate;
         int m_autochknum[2];
         int m_silencePulse;
         int m_lastProducePulse;
         int m_hackProduceCount;
         int m_ProduceClientTime;
         int m_perSecondPulse;
         int m_shoutPulse;
         bool m_bProcDisconnect;
         CInventory m_invenNormal;
         CInventory m_invenQuest;
         CInventory m_invenEvent;
         CStash m_stash;
         bool m_bChangeStash;
         CNetMsg m_stashMsg;
         GoldType_t m_moneyItem;
         CItem *m_wearing[28];
         CItem *m_wearing_suit;
         std::vector<__tagAutoGiveData, std::allocator<__tagAutoGiveData> > m_listAutoGive;
         std::vector<__tagBloodBugData, std::allocator<__tagBloodBugData> > m_listBloodBug;
         CExchangeItems *m_exchange;
         CPersonalShop *m_personalshop;
         CMemPos m_mempos;
         int m_reqWarpTime;
         int m_reqWarpType;
         int m_reqWarpData;
         float m_Npc_Portal_x;
         float m_Npc_Portal_z;
         char m_Npc_Portal_y;
         CParty *m_party;
         CExpedition *m_Exped;
         int m_nJoinInzone_ZoneNo;
         int m_nJoinInzone_RoomNo;
         int m_nJoinInzone_AreaNo;
         int m_nRaidBoxItemTime;
         CQuickSlot m_quickSlot[3];
         int m_pkPenalty;
         int m_pkCount;
         int m_pkRecoverPulse;
         int m_pkmodedelay;
         int m_pkPenaltyHP;
         int m_pkPenaltyMP;
         int m_pkRecoverNPCCount;
         CRaChar *m_raList;
         int m_recoverHPItemTime;
         int m_recoverHPItemValue;
         int m_recoverMPItemTime;
         int m_recoverMPItemValue;
         CQuestList m_questList;
         int m_nLastCollectRequest;
         int m_nCollectRequestNPCIndex;
         int m_nCollectRequestCount;
         CGuildMember *m_guildInfo;
         int m_regGuild;
         int m_guildoutdate;
         bool m_bGiveItem;
         int m_changePulse;
         int m_changeIndex;
         char m_plusEffect;
         int m_teachIdx[10];
         int m_teachLevel[10];
         char m_teachJob[10];
         char m_teachJob2[10];
         char m_teachName[10][51];
         int m_teachTime[10];
         int m_teachWait;
         char m_teachType;
         bool m_bTeacher;
         int m_fame;
         int m_cntTeachingStudent;
         int m_cntCompleteStudent;
         int m_cntFailStudent;
         int m_superstone;
         int m_guardian;
         bool m_displayCanSstone;
         int m_nMoonStoneDigit;
         int m_nMoonStoneSum;
         int m_lacaretteCosArrNum;
         int m_lacaretteItemArrNum;
         int m_lacaretteTokenArrNum;
         int m_lacaretteRepeat;
         int m_nMSIndex;
         int m_nMSGiftIndex;
         CPet *m_petList;
         int m_pulseLastAttackSkill;
         bool m_bNotMiss;
         CElemental *m_elementalList;
         int m_pulseElemental[4];
         int m_pulseEvocation[2];
         char m_evocationType;
         int m_memposTime;
         int m_stashextTime;
         int m_pulseTakeEventXMas2005;
         int m_nTimeEventNewYear2006;
         int m_pulseEventNewYear2006;
         int m_nExpDamage;
         int m_nGoldenBallNoticeStatus;
         char m_nEventGomdori2007Win;
         char m_nEventGomdori2007Game;
         char m_nEventGomdori2007FirstLose;
         char m_nEventGomdori2007FirstWin;
         bool m_bEventGomdori2007Start;
         int m_nEventGomdori2007Total;
         bool m_bEventGomdori2007CanSelect;
         int m_bEventGomdori2007Drop;
         CRockPaperScissorsInfo m_RockPaperScissorsInfo;
         int m_nAttendance_exp;
         int m_bEventIndependenceDay2007drop;
         int m_GuildExp;
         int m_GuildFame;
         long m_nRespondTime;
         CAPet *m_pApetlist;
         float m_runSpeed_org;
         char m_attackSpeed_org;
         int m_maxHP_org;
         int m_maxMP_org;
         int m_bPhoenix_Char;
         GM_GOZONE_TYPE m_gmGoZoneType;
         CFactoryList m_listFactory;
         bool m_bTaming_npc;
         CNPC *m_Slave_npc;
         int m_nSoul_Count;
         CCharacter *m_Owners_target;
         char m_skillTargetCount;
         char *m_targettype;
         int *m_targetindex;
         bool m_bCheckAppear;
         //set_npc_t m_listTaming;
		 std::set<CNPC*, std::less<CNPC*>, std::allocator<CNPC*> > set_npc_t;
         CAffinityList m_affinityList;
         int m_nCurrentTitle;
         CTitleList m_titleList;
         int m_newtutoComplete;
         int m_exp_weekly;
         CNPC *m_pSummnonNpc[5];
         CCharacter *m_SummonOwners_target;
         int m_nBombSummonCnt;
         int m_pkPenaltyReward;
         int m_pkDispositionRateValue;
         int m_pkDispositionAddValue;
         std::vector<__tagMonsterComboData, std::allocator<__tagMonsterComboData>> m_listSaveComboData;
         char m_bPressCorps;
         int m_suitWearingPos[5];
       private:
         int m_nKillCount;
         int m_nTotalKillCount;
         int m_nDeathCount;
         int m_nTotalDeathCount;
         int m_nWarGroundPoint;
         int m_nWarGroundAccPoint;

       public:
         CPC(void);
         void reviseHPMP(void);
         bool AddExpUseGP(long long);
         void ApplyTitle(void);
         void TitleCurrentSkillCure(int, bool);
         void CastllanTitleDelete(int, bool, char);
         ~CPC();
         void GoNpcPortalScroll(void);
         void SetRaidBoxItemTime(int);
         int GetRaidBoxItemTime(void);
         GoldType_t GetMoney(void);
         void StartRockPaperScissors(void);
         bool CanRockPaperScissors(void);
         CCharacter * GetOwners_target(void);
         void SetOwners_target(CCharacter *);
         int GetSoul_Count(void);
         void SetSoul_Count(int);
         void DeleteSlave(CNPC *);
         bool AddSlave(CNPC *);
         bool IsTaming(void);
         void SetSummonOwners_target(CCharacter *);
         CCharacter * GetSummonOwners_target(void);
         void SetSummonNpc(_tagSummonNpcType, CNPC *);
         CNPC * GetSummonNpc(int, int);
         CNPC * GetSummonNpc(_tagSummonNpcType);
         CNPC * GetSummonNpc(CNPC *);
         void SummonNpcRemove(CNPC *, bool);
         void SummonNpcRemove(_tagSummonNpcType, bool);
         void SummonNpcRemoveAll(bool);
         int GetSummonNpcType(CNPC *);
         int GetBombSummonCont(void);
         bool SetBombSummonNPC(CNPC *);
         void SendStatus(void);
         void SendInventory(char);
         void Send(CNetMsg &);
         void SendQuickSlot(void);
         void Restore(void);
         bool IncreaseMoney(long long);
         bool DecreaseMoney(long long);
         bool AddExpSP(long long, int, bool, bool, bool);
         void AddExpSP_Direct(long long, long long);
         void CalcLevelup(void);
         void LevelUp(bool);
         void InitStat(void);
         virtual void CalcStatus(bool);
         bool InitSkill(CSkillList *, bool);
         bool InitSSkill(void);
         void RecoverPC(void);
         void ApplyItemValue(bool);
         void ApplyItemPlusValue(CItem *, int);
         int ItemUpgradeFuckingFunction(int, int, int);
         int CanChange(void);
         const char * GetName(void);
         void ChangeName(const char *);
         bool IsNick(void);
         int SettingItemOption(void);
         void RemoveItemFromQuickSlot(CItem *);
         void RemoveSkillFromQuickSlot(CSkill *);
         bool CheckWeight(long long, bool, bool, bool);
         void SetMaxWeight(bool);
         void GoMemPos(int);
         bool UseStatPoint(_tagStatPointUseType, int *);
         int CanWarp(void);
         virtual CSkill * FindSkill(int);
         //virtual int GetAttackLevel(void) const;
         //virtual int GetDefenseLevel(void) const;
         //virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto *) const;
         int GetPKName(void);
         bool IsSetPlayerState(int);
         void SetPlayerState(int);
         void ResetPlayerState(int);
         void TogglePlayerState(int);
         int GetPlayerState(void);
         bool IsCombatMode(void);
         /*virtual bool CanMove(void);
         virtual bool CanAttack(void);
         virtual bool CanSpell(void);
         bool GiveItem(int, int, int, long long, bool, bool);
         //virtual float GetSize(void);
         bool IsParty(void);
         void SaveNow(bool);
         bool IsExped(void);
         //virtual int GetUsingStat(void) const;
         void SetMessengerCondition(int);
         bool AddBlockPC(int, const char *);
         void ReleaseBlockPC(int, CLCString &);
         bool IsBlockPC(int, const char *);
         void GetBlockListString(CLCString &, CLCString &);
         void do_QuestGiveUp(CPC *, CQuest *);
         bool CheckDungeonData(int);
         //virtual int GetResistLevelBonus(void);
         int GetJoinFlag(int);
         void SetJoinFlag(int, int);
         void SetPKPenaltyReward(int);
         int CheckPKPenaltyReward(int);
         int GetPKPenaltyRewardNum(void);
         bool IsChaotic(void);
         void AddPkPenalty(int);
         void UpdatePetValue(void);
         CPet * GetPet(int);
         void DelPet(int);
         void UpdateAPetValue(int);
         CAPet * GetAPet(int);
         void DelAPet(int);
         bool CanPvP(CCharacter *, bool);
         void CancelChange(void);
         CElemental * SummonElemental(char);
         void UnsummonElemental(CElemental *);
         bool CanSummonElemental(char, char *, int *, bool *);
         CElemental * GetElemental(int);
         void SendElementalStatus(void);
         void Evocation(int);
         void Unevocation(void);
         int GetRemainEvocation(bool);
         //virtual void ApplyPassive(void);
         void ProcDisconnect(bool, bool);
         void OutputDBItemLog(void);
         //virtual float GetHitrate(CCharacter *, _tagDamageType);
         //virtual float GetAvoid(CCharacter *, _tagDamageType);
         void SetRespondTime(long);
         long GetRespondTime(void);
         void ApplySetItemValue(CItem *);
         void ApplyOriginOption(CItem *);
         void RemoveSetItemValue(CItem *);
         void CountSetItem(CItem *, bool);
         int GetSealPos(int);
         LONGLONG GetSealExp(int);
         void SetSealExp(int, long long);
         void AddSealSkill(int);
         bool CheckInvenForQuestPrize(const CQuestProto *, const int &);
         bool CheckInvenForProduceNoramlPrize(const int &, const int &, const int &);
         bool CheckInvenForProduceRandomPrize(void);
         bool CheckInvenForProcessPrize(CItemProto *);
         bool CheckInvenForTresureBoxPrize(CItem *);
         bool CheckInvenForTresureBoxRedPrize(int **, int);
         bool CheckInvenForPrize(int, int, int, int);
         bool CheckInvenForPrize(CItemProto *, int, int, int);
         void GuildGradeSkill(bool);
         void ApplyGuildGradeSkill(void);
         void CureGuildGradeSkill(void);
         bool CheckCastellanType(int &, char &);
         void SettingCostumeSuit(CItem *);
         void DressCostumeSuit(CItem *);
         void UndressCostumeSuit(void);
         void UndressCostumeAll(void);
         const int GetSuitWearingPos(const int &);
       private:
         void Clean(void);
         void CleanSummonNPC(void);
         void CleanTaming(void);
         void CleanParty(void);
         void CleanExchange(void);
         void CleanExtreamCube(void);
       public:
         int GetKillCount(void);
         int GetTotalKillCount(void);
         int GetDeathCount(void);
         int GetTotalDeathCount(void);
         int GetWarGroundPoint(void);
         int GetWarGroundAccPoint(void);
         void InitKillCount(void);
         void AddKillCount(int);
         void SetTotalKillCount(int);
         void InitDeathCount(void);
         void AddDeathCount(int);
         void SetTotalDeathCount(int);
         void SetWarGroundPoint(int);
         void AddWarGroundPoint(int);
         void SetWarGroundAccPoint(int);
         void writeDebugString(char *);
     };
*/
class CNPC : public CCharacter {
public:
         CNPCProto *m_proto;
         int m_idNum;
         int m_flag;
         int m_flag1;
         float m_regenX;
         int m_regenY;
         float m_regenZ;
         int m_disableTime;
         int m_delay;
         int m_pulseAttack;
         char m_attackType;
         int m_postregendelay;
         bool m_bMoveRun;
         int m_nBlockRegen;
         bool m_bStop;
         int m_moveArea;
         int m_aipulse;
         int m_moveDir;
         int m_nBlockAI;
         bool m_bChangeDir;
         bool m_bFail45;
         int m_quest[50];
         int m_nQuestCount;
         CSkill *m_skills[4];
         int m_regenTime;
         int m_regenTimeXmas2007;
         CNPC *m_pNPCPrev;
         CNPC *m_pNPCNext;
         CNPCRegenInfo *m_regenInfo;
         int m_nRaidMoveTime;
         int m_bRaidNPCSummon;
         int m_MobScrollType;
         int m_NextMobIndex;
         int m_UseCharIndex;
         int m_coinidx;
         int m_coincount;
         float m_nOldDist;
         bool m_bMoveLock;
         int m_pulseMoveLock;
         int m_bMoveToRegen;
         int m_Mob_State;
         CPC *m_owner;
         int m_pulseSoulRecover;
         bool m_bCubeRegen;
         MonsterMercenaryData *m_pMercenaryClassData;
       public:
         CNPC(void);
         ~CNPC();
         CPC * GetOwner(void);
         void SetOwner(CPC *);
         int Check_MobFlag(int) const;
         void Set_MobFlag(int);
         void reSet_MobFlag(int);
         void DeleteNPC(void);
         void InitAIVar(void);
         void ResetStat(void);
         virtual bool CanMove(void);
         virtual bool CanAttack(void);
         //virtual int GetAttackLevel(void) const;
         //virtual int GetDefenseLevel(void) const;
         //virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto *) const;
         //virtual float GetSize(void);
         //virtual int GetUsingStat(void) const;
         /*virtual CSkill * FindSkill(int);
         virtual void ApplyPassive(void);
         virtual void CalcStatus(bool);
         LONGLONG GetTotalDamage(void);
         virtual float GetHitrate(CCharacter *, _tagDamageType);
         virtual float GetAvoid(CCharacter *, _tagDamageType);
         float GetDistToRegen(void);
         void SetMercenaryClassData(MonsterMercenaryData *);*/
         MonsterMercenaryData * GetMercenaryClassData(void);
     };

class CPet : public CCharacter {
       private:
         CPC *m_owner;
         char m_petTypeGrade;
         int m_hungry;
         int m_pulseHungry;
         int m_sympathy;
         int m_pulseSympathy;
         int m_abilityPoint;
         int m_pulseExpUp;
         char m_petColorType;
         int m_nTurnToNpcIndex;
         int m_nTurnToNpcSize;
         bool m_bSummon;
         bool m_bMount;
         bool m_wasnear;
         int m_nRemainRebirth;
         CSkillList m_skillList;
       public:
         CPet *m_prevPet;
         CPet *m_nextPet;

         CPet(CPC *, int, char, int);
         ~CPet();
         CPC * GetOwner(void);
         void SetOwner(CPC *);
         char GetPetType(void);
         char GetPetGrade(void);
         bool IsMountType(void);
         char GetPetTypeGrade(void);
         void InitStat(void);
         bool IsWearing(void);
         bool IsSummon(void);
         bool IsMount(void);
         void ResetSummonMountFlag(void);
         void Appear(bool);
         void Disappear(void);
         void Mount(bool);
         void SetRemainRebirthTime(int);
         int GetRemainRebirthTime(void);
         void SetWasNear(bool);
         bool WasNear(void);
         float GetDistanceFromOwner(void);
         void ChangePetType(void);
         bool IsActive(void);
         int GetHungryPoint(void);
         bool DecreaseHungryPoint(void);
         int GetUnitForHungry(void);
         bool IncreaseHungryPoint(int);
         int GetSympathyPoint(void);
         bool UpdateSympathyPoint(bool);
         int GetUnitForSympathy(void);
         bool IncreaseSympathyPoint(int);
         LONGLONG GetNeedExp(void);
         bool AddExp(void);
         void SetPetColor(char);
         char GetPetColor(void);
         void SetPetTurnToNpc(int);
         int GetPetTurnToNpc(void);
         void SetPetTurnToNpcSize(int);
         int GetPetTurnToNpcSize(void);
         void LevelUp(void);
         int GetAbilityPoint(void);
         int DecreaseAbilityPoint(int);
         int IncreaseAbilityPoint(int);
         void ChangeToMount(void);
         void ResetMountType(void);
         virtual CSkill * FindSkill(int);
         void AddSkill(CSkill *&);
         CSkillList * GetSkillList(void);
         bool ResetSkill(void);
         //virtual int GetAttackLevel(void) const;
         //virtual int GetDefenseLevel(void) const;
         //virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto *) const;
         //virtual float GetSize(void);
         //virtual int GetUsingStat(void) const;
         virtual void ApplyPassive(void);
         virtual void CalcStatus(bool);
         virtual float GetHitrate(CCharacter *, _tagDamageType);
         virtual float GetAvoid(CCharacter *, _tagDamageType);
     };

typedef struct __tagApetAIData {
    int nItemNum;
    CItem *pItem;
    int nSkillIdx;
    int nSkillType;
    char cUse;
} APET_AI_DATA;

class CAPet : public CCharacter {
   private:
     CPC *m_pOwner;
     int m_nHpMpPulse;
     int m_nStmPulse;
     int m_nDStmPluse;
     int m_nFaithPulse;
     int m_nDFaithPulse;
     int m_nFaith;
     int m_nStm;
     LONGLONG m_llAccExp;
     int m_nRemainCooltime;
   public:
     CAPet *m_pPrevPet;
     CAPet *m_pNextPet;
     bool m_bSummon;
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
     int m_nOpHP;
     int m_nOpMP;
     CAPetProto *m_pProto;
     CSkillList m_skillList;
     CSkillList m_passiveSkillList;
     CItem *m_wearing[4];
     char m_cAI;
     int m_nAICount;
     int m_nAISlot;
     APET_AI_DATA m_tAIData[6];
     int m_nDelay;
     unsigned int m_nAccStemina;
     unsigned int m_nAccFaith;
     int m_optTransEnd;
     char m_cTransSate;
     bool m_bMount;
     char m_cSendTransMsg;
     char m_cForceTrans;

     CAPet(void);
     ~CAPet();
     bool TransFormationCheck(void);
     void Mount(bool);
     bool IsMount(void);
     void SetOwner(CPC *);
     void SetFaith(int);
     void SetStamina(int);
     void AddFaith(int);
     void AddStamina(int);
     CPC * GetOwner(void);
     int GetFaith(void);
     int GetStamina(void);
     int GetStmLevel(void);
     int GetFaithLevel(void);
     bool IsSummon(void);
     CItem * AddToWearItem(CItem *);
     void Appear(bool, bool);
     void Disappear(void);
     bool IsWearing(void);
     bool IsPetEquip(void);
     bool IncreaseStat(void);
     bool DecreaseStat(void);
     void CheckItemTime(void);
     void UpdateStatPlus(int, int, int, int);
     LONGLONG GetNeedExp(void);
     bool AddExpSP(long long, int);
     void LevelUp(bool);
     void AddExpSP_Direct(long long, int);
     void CalcLevelup(void);
     void ApplyItemValue(void);
     //virtual int GetAttackLevel(void) const;
     //virtual int GetDefenseLevel(void) const;
     virtual MSG_DAMAGE_TYPE GetAttackType(const CSkillProto *) const;
     //virtual float GetSize(void);
     //virtual int GetUsingStat(void) const;
     virtual CSkill * FindSkill(int);
     void AddSkill(CSkill *);
     virtual void ApplyPassive(void);
     virtual void CalcStatus(bool);
     void Evolution(CAPet *);
     void InitStat(void);
     virtual float GetHitrate(CCharacter *, _tagDamageType);
     virtual float GetAvoid(CCharacter *, _tagDamageType);
     bool IsAI(void);
     bool IsEmptyAILine(int);
     bool DelAIList(CItem *);
     void CheckAIList(void);
   private:
     void SendStatusMsg(void);
   public:
     bool IsProtoFlag(int);
     LONGLONG GetAccExp(void);
     void SetAccExp(long long);
     int GetCooltime(void);
     void SetCooltime(int);
     void ResetCooltime(void);
     double GetAccRate(int);
     LONGLONG GetMaxAccExp(int, long long);
     void AddAccExp(long long, long long);
     void UseAccExp(CPC *);
     bool UseAccExp(CAPet *);
     void LoadCooltime(int);
     void LoadAccExp(long long);
 };
/*
bool AddToInventory(CPC* ch, CItem* item, bool bSendWarning, bool bCheckWeight, int row = -1, int col = -1);
bool DecreaseFromInventory(CPC* ch, CItem* item, LONGLONG count);
bool RemoveFromInventory(CPC* ch, int tab, int row, int col, bool bFreeMem, bool removeQuickSlot);
bool RemoveFromInventory(CPC* ch, CItem* item, bool bFreeMem, bool removeQuickSlot);
bool AddToPreInventory(CPC* ch, CItem* item, bool bSendWarning, bool bCheckWeight, int tab, int row = -1, int col = -1);
void CheckQuestCondition(CPC* ch, CItem* item, LONGLONG itemSum);*/


#endif
