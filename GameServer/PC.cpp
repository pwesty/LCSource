/************************************
  수정사항
  수정한 사람: bs
  수정일: 2004-11-18 오후 3:42:52
  설명: PC의 보조 효과 적용 및 레벨업 버그 수정
 ************************************/

#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "Exp.h"
#include "doFunc.h"
#include "Guild.h"
#include "Battle.h"
#include "Friend.h"
#include "WarCastle.h"
#include "../ShareLib/DBCmd.h"
#include "Utils.h"
#include "TitleSystem.h"
#include "Artifact_Manager.h"

#include "Party_recall_manager.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"

#ifdef DURABILITY
#include "../ShareLib/packetType/ptype_durability.h"
#endif

#include "../ShareLib/packetType/ptype_old_do_action.h"
#include "../ShareLib/packetType/ptype_old_do_skill.h"
#include "../ShareLib/packetType/ptype_server_to_server.h"
#include "../ShareLib/packetType/ptype_old_do_friend.h"

// 순서대로 타이탄, 나이트, 힐러, 메이지, 로그, 소서러, 나이트쉐도우(dummy), EX 로그, EX 메이지
int student20LEVELGiftTable[JOBCOUNT][2] =
{
	{ 6105,  460},
	{ 6106,  427},
	{ 6107,  462},
	{ 6108,  461},
	{ 6109,  951},
	{ 6110, 1069},
#ifdef EX_ROGUE
	{    0,    0},
	{ 6109,  951},
#endif // EX_ROGUE
#ifdef EX_MAGE
	{ 6108,	 461},
#endif // EX_MAGE
};

// 순서대로 타이탄, 나이트, 힐러, 메이지, 로그, 소서러 1-20 레벨까지 기본 상승 수치
// 레벨업시 각 스탯 변화
// 스탯은 str, dex, int, con 순서
int levelup_stat[JOBCOUNT][4]	=
{
	{3, 1, 1, 3},
	{2, 2, 2, 2},
	{1, 3, 2, 2},
	{1, 3, 3, 1},
	{3, 3, 1, 1},
	{1, 2, 3, 2},
	{1, 3, 3, 2},
#ifdef EX_ROGUE
	{2, 3, 2, 1},
#endif // EX_ROGUE
#ifdef EX_MAGE
	{1, 3, 3, 1},
#endif // EX_MAGE
};
// 레벨업시 각 스탯당 hp, mp 증가량
int levelup_hp[JOBCOUNT][4]		=
{
	{4, 0, 0, 8},
	{4, 0, 0,11},
	{4, 0, 0, 7},
	{3, 0, 0, 8},
	{5, 0, 0,12},
	{5, 0, 0, 5},
	{3, 0, 0,11},
#ifdef EX_ROGUE
	{5, 0, 0,12},
#endif // EX_ROGUE
#ifdef EX_MAGE
	{3, 0, 0, 8},
#endif // EX_MAGE
};

int levelup_mp[JOBCOUNT][4]		=
{
	{0, 0, 8, 2},
	{0, 0, 6, 2},
	{0, 0, 9, 2},
	{0, 0,10, 1},
	{0, 0, 7, 2},
	{0, 0, 7, 4},
	{0, 0, 8, 1},
#ifdef EX_ROGUE
	{0, 0, 9, 2},
#endif // EX_ROGUE
#ifdef EX_MAGE
	{0, 0, 10, 1},
#endif // EX_MAGE
};

// 기본 능력치
int base_stat[JOBCOUNT][4]		=
{
	{50, 20, 20, 50},
	{38, 28, 28, 38},
	{20, 50, 38, 28},
	{20, 50, 50, 20},
	{38, 50, 20, 28},
	{20, 44, 44, 28},
	{10, 50, 50, 30},
#ifdef EX_ROGUE
	{38, 50, 20, 12},
#endif // EX_ROGUE
#ifdef EX_MAGE
	{20, 50, 50, 20},
#endif // EX_MAGE
};

#ifdef EX_ROGUE
#ifdef EX_MAGE
int base_hp[JOBCOUNT]				= {180, 150, 110,  90, 130, 110, 120, 130,  100};
int base_mp[JOBCOUNT]				= { 68,  86, 108, 122,  56, 128, 120,  56, 128};
#else
int base_hp[JOBCOUNT]				= {180, 150, 110,  90, 130, 110, 120, 130};
int base_mp[JOBCOUNT]				= { 68,  86, 108, 122,  56, 128, 120,  56};
#endif
#else
int base_hp[JOBCOUNT]				= {180, 150, 110,  90, 130, 110, 120};
int base_mp[JOBCOUNT]				= { 68,  86, 108, 122,  56, 128, 120};
#endif // EX_ROGUE

// 전직 기본 능력치
// 순서대로 하이랜더, 워마스터, 로열나이트, 템플나이트, 아처, 힐러, 위자드, 위치, 어쌔신, 레인저, 엘리멘탈리스트, 스페셜리스트, 나이트 쉐도우, EX 어쌔신, EX 레인저, EX 위자드, EX 위치
int base_stat2[JOB2COUNT][4]		=
{
	{167,  79,  79, 167},
	{167,  79,  79, 167},
	{126, 116, 116, 126},
	{126, 116, 116, 126},
	{ 79, 167, 126, 116},
	{ 79, 167, 126, 116},
	{ 79, 167, 167,  79},
	{ 79, 167, 167,  79},
	{155, 167,  79,  87},
	{155, 167,  79,  87},
	{ 79, 132, 161, 116},
	{ 79, 132, 161, 116},
	{ 69, 167, 167, 118},
#ifdef EX_ROGUE
	{126, 167, 126,  87},	// EX 어쌔신
	{126, 167, 126,  87},	// EX 레인저
#endif // EX_ROGUE
#ifdef EX_MAGE
	{ 79, 167, 167,  79},
	{ 79, 167, 167,  79},
#endif // EX_MAGE
};

#ifdef EX_ROGUE
#ifdef EX_MAGE
int base_hp2[JOB2COUNT] = {	1584, 1584,	1382, 1382, 962, 962, 739, 739,	1247, 1247,	1045, 1045, 1265, 962, 962, 839, 839 };
int base_mp2[JOB2COUNT]	= { 774, 774, 790, 790, 1076, 1076,	1351, 1351, 587, 587, 1299, 1299, 1134, 790, 790, 1351, 1351 };
#else
int base_hp2[JOB2COUNT] = {	1584, 1584,	1382, 1382, 962, 962, 739, 739,	1247, 1247,	1045, 1045, 1265, 962, 962 };
int base_mp2[JOB2COUNT]	= { 774, 774, 790, 790, 1076, 1076,	1351, 1351, 587, 587, 1299, 1299, 1134, 790, 790 };
#endif
#else
int base_hp2[JOB2COUNT] = {	1584, 1584,	1382, 1382, 962, 962, 739, 739,	1247, 1247,	1045, 1045, 1265 };
int base_mp2[JOB2COUNT]	= { 774, 774, 790, 790, 1076, 1076,	1351, 1351, 587, 587, 1299, 1299, 1134 };
#endif // EX_ROGUE

int levelup_stat2[JOB2COUNT][4]		=
{
	{3, 1, 1, 1},
	{2, 1, 1, 2},
	{2, 1, 2, 1},
	{2, 1, 1, 2},
	{2, 2, 1, 1},
	{2, 1, 2, 1},
	{1, 1, 3, 1},
	{1, 2, 2, 1},
	{1, 3, 1, 1},
	{2, 2, 1, 1},
	{1, 1, 2, 2},
	{1, 10, 4, 1},
	{1, 3, 3, 2},
#ifdef EX_ROGUE
	{1, 3, 2, 1},
	{2, 2, 2, 1},
#endif // EX_ROGUE
#ifdef EX_MAGE
	{1, 1, 3, 1},
	{1, 2, 2, 1},
#endif // EX_MAGE
};

int levelup_hp2[JOB2COUNT]				=
{
	10,
	10,
	4,
	4,
	2,
	2,
	1,
	1,
	3,
	3,
	2,
	3,
	2,
#ifdef EX_ROGUE
	3,
	3,
#endif // EX_ROGUE
#ifdef EX_MAGE
	1,
	1,
#endif // EX_MAGE
};

int levelup_mp2[JOB2COUNT]				=
{
	4,
	4,
	2,
	2,
	12,
	12,
	10,
	10,
	2,
	2,
	10,
	10,
	11,
#ifdef EX_ROGUE
	3,
	3,
#endif // EX_ROGUE
#ifdef EX_MAGE
	10,
	10,
#endif //EX_MAGE
};

// 이동 속도
#ifdef EX_ROGUE
#ifdef EX_MAGE
float run_speed[JOBCOUNT]			= {6.0f, 6.0f, 7.0f, 6.0f, 6.5f, 7.0f, 6.5f, 6.5f, 6.0f};
float walk_speed[JOBCOUNT]			= {1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f};
#else
float run_speed[JOBCOUNT]			= {6.0f, 6.0f, 7.0f, 6.0f, 6.5f, 7.0f, 6.5f, 6.5f };
float walk_speed[JOBCOUNT]			= {1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f };
#endif
#else // EX_ROGUE
float run_speed[JOBCOUNT]			= {6.0f, 6.0f, 7.0f, 6.0f, 6.5f, 7.0f, 6.5f};
float walk_speed[JOBCOUNT]			= {1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f};
#endif // EX_ROGUE

// 기본 명중도 : 직업-(근접,원거리,마법)
int nBaseHitrate[JOBCOUNT][3]	=
{
	{137, 137, 137},
	{130, 130, 130},
	{142, 142, 131},
	{142, 142, 142},
	{142, 142, 142},
	{137, 137, 137},
	{142, 142, 142},
#ifdef EX_ROGUE
	{142, 142, 142},
#endif // EX_ROGUE
#ifdef EX_MAGE
	{142, 142, 142},
#endif // EX_MAGE
};
// 기본 회피도 : 직업-(근접,원거리,마법)
int nBaseAvoid[JOBCOUNT][3]	=
{
	{20, 20, 20},
	{28, 28, 28},
	{50, 50, 38},
	{50, 50, 50},
	{50, 50, 20},
	{44, 44, 44},
	{50, 50, 50},
#ifdef EX_ROGUE
	{50, 50, 20},
#endif // EX_ROGUE
#ifdef EX_MAGE
	{50, 50, 50},
#endif // EX_MAGE
};
// 기본 공격 타입
// 물리/마법, 위크/노멀/스트롱/하드/크리/데들리
int nBaseHitTypeProb[2][6] =
{
	{ 50, 575, 125, 100,  90,  60},
	{ 50, 575, 125, 100,  90,  60},
};

/////////////
// CPC member
CPC::CPC()
	: m_nick(MAX_CHAR_NAME_LENGTH + 1)
	, m_inventory(this)
	, m_petStashManager(this)
	, m_wearInventory(this)
	, m_syndicateManager(this)
	, m_gpsManager(this)
	, m_itemCollectionManager(this)
	, m_attendanceManager(this)
#ifdef PREMIUM_CHAR
	, m_premiumChar(this)
#endif
	,m_firstSendData(true)
	,m_isWarSkillCheck(false)
	,m_arti_gpsManager(this)
{
	m_type = MSG_CHAR_PC;

	m_billReqTime = 0;
	m_listIdx = -1;
	m_bPlaying = false;
	m_bChangeStatus = false;
	m_bImmortal = false;
	m_bChatMonitor = false;
	m_bLoadChar = false;
	m_speedup = 0;

	m_bTradeAgentRegIng = false;	//등록 처리중인가(거래대행)
	m_bTradeAgentCalcIng = false;	//정산 처리중인가(거래대행)
	m_bTradeAgentBuyIng = false;	//구매 처리중인가(거래대행)
	m_nCalcPageNo		 = 0;		//정산 현재 페이지 번호

#ifdef GMTOOL
	m_bGmMonitor = false;
#endif // GMTOOL

	m_pd3Count = 0;
	m_pd3Time = 0;
	m_pd4Count = 0;
	m_pd4Time = 0;
	m_recentAtt |= MATT_UNWALKABLE;

#ifdef RANKER_NOTICE
	m_bRanker = false;
#endif

	m_autoSkillTime = 0;
	m_SkillTime_511 = 0;
	m_cashBalance = 0;		// 보유 캐쉬

	m_secretkey = 0;

	/////////////////////////////////////////////
	// BANGWALL : 2005-06-27 오후 8:52:59
	// Comment : pd4time
	m_pd4StartTime = 0;

	m_loseexp = 0;

	m_job = 0;
	m_job2 = 0;
	m_hairstyle = 0;
	m_facestyle = 0;
	m_admin = 0;

// 050401 : bs : plus에 따른 확률 변수 추가
	m_plusStrong = 0;
	m_plusHard = 0;
	m_plusWeak = 0;
// --- 050401 : bs : plus에 따른 확률 변수 추가

	m_statpt_remain = 0;
	m_statpt_str = 0;
	m_statpt_dex = 0;
	m_statpt_int = 0;
	m_statpt_con = 0;

	m_state = 0;
	m_pkPenalty = 0;
	m_pkCount = 0;
	m_pkRecoverPulse = 0;
	m_pkmodedelay = 0;
	m_pkPenaltyHP = 0;
	m_pkPenaltyMP = 0;
	m_pkRecoverNPCCount = 0;

#ifdef REFORM_PK_PENALTY_201108 // PK 패널티 리폼
	m_pkPenaltyReward = 0;
	m_pkDispositionRateValue = 0;
	m_pkDispositionAddValue = 0;
#endif

	m_dbStr = 0;
	m_dbDex = 0;
	m_dbInt = 0;
	m_dbCon = 0;

	m_dbHP = 0;
	m_dbMP = 0;
	m_tempHP = 0;
	m_tempMP = 0;
	m_opSturnLevel = 0;
	m_opBloodLevel = 0;
	m_opPoisonLevel = 0;
	m_opSlowLevel = 0;
	m_opMoveLevel = 0;

	m_opSturnIndex = 0;
	m_opBloodIndex = 0;
	m_opPoisonIndex = 0;
	m_opSlowIndex = 0;
	m_opMoveIndex = 0;

	m_bCreateMixItem = false;

	m_opIncreaseInven = 0;
	m_opMPSteal = 0;
	m_opHPSteal = 0;
	m_opAttackBlind = 0;
	m_opAttackPoison = 0;
	m_opAttackCritical = 0;
	m_opAttackDeadly = 0;

	m_opRecoverHP = 0;
	m_opRecoverMP = 0;
	m_opRecoverHP_NoRate = 0;
	m_opRecoverMP_NoRate = 0;
	m_opDecreaseSkillDelay = 0;
	m_opDecreaseSkillMP = 0;
	m_opResistStone = 0;
	m_opResistSturn = 0;
	m_opResistSilent = 0;
	m_opBlocking = 0;

	m_opStrong = 0;

	m_desc = NULL;

	m_silencePulse = gserver->m_pulse;
	m_lastProducePulse = 0;

	m_hackProduceCount = PULSE_HACK_ATTACK_COUNT;	// 샌상메세지 온 수 저장
	m_ProduceClientTime = 0; // 클라이언트가 보낸 생산시간

	// INVENTORY INITIALIZE
	m_bChangeStash = false;

	m_wearInventory.setCosWearSuitItem(NULL);

	m_exchange = NULL;
	m_party = NULL;
	m_bGiveItem = false;

	m_Exped				= NULL;

	m_nJoinInzone_ZoneNo = -1;			//현재 인존 존 번호
	m_nJoinInzone_RoomNo = -1;			//현재 인존 룸 번호
	m_nJoinInzone_AreaNo = -1;
	m_nRaidBoxItemTime = 0;

	m_recoverHPItemTime = 0;
	m_recoverMPItemTime = 0;
	m_recoverHPItemValue = 0;
	m_recoverMPItemValue = 0;

	m_reqWarpTime = 0;
	m_reqWarpType = -1;
	m_reqWarpData = -1;

	m_reqWarpTime_skill = 0;
	m_reqWarpType_skill = -1;
	m_reqWarpData_skill = -1;

	m_guildInfo = NULL;
	m_regGuild = 0;
	m_guildoutdate = 0;
	m_joinMerac = WCJF_NONE;

	m_raList = NULL;

	m_changePulse = 0;
	m_changeIndex = -1;

	m_plusEffect = 0;

	memset(m_teachIdx, -1, sizeof(m_teachIdx));
	memset(m_teachLevel, 0, sizeof(m_teachLevel));
	memset(m_teachJob, -1, sizeof(m_teachJob));
	memset(m_teachJob2, 0, sizeof(m_teachJob2));
	memset(m_teachName, 0, sizeof(m_teachName));
	memset(m_teachTime, 0, sizeof(m_teachTime));

	m_teachWait = -1;
	m_teachType = MSG_TEACH_NO_TYPE;
	m_bTeacher = false;

	m_fame = 0;
	//0627
	m_cntTeachingStudent = 0;
	m_cntCompleteStudent = 0;
	m_cntFailStudent = 0;
	m_superstone = 0;
	m_guardian   = 0;
	m_displayCanSstone = true;

	m_nMoonStoneSum = 0;
	m_nMoonStoneDigit = -1;

#ifdef LACARETTE_SYSTEM
	m_lacaretteCosArrNum = -1;				// 당천된 아이템 인덱스를 임시로 가지고 있자. ㅡㅡ;; // 이렇게 해야하나? ㅡㅡ;;
	m_lacaretteItemArrNum = -1;
	m_lacaretteTokenArrNum= -1;
	m_lacaretteRepeat = -1;
#endif

	m_nMSIndex	= 0;
	m_nMSGiftIndex	= 0;

	m_memposTime = 0;

	m_stashextTime = 0;

	m_nRegFriend = 0;
	m_nCondition = 0;
	m_Friend = NULL;

	m_etcEvent = 0;

	m_petList = NULL;
	m_pulseLastAttackSkill = 0;
	m_bNotMiss = false;

	m_pApetlist = NULL;

	m_pulseTakeEventXMas2005 = 0;
	m_nTimeEventNewYear2006 = 0;
	m_pulseEventNewYear2006 = 0;

	m_elementalList = NULL;
	memset(m_pulseElemental, -1, sizeof(m_pulseElemental));

	m_evocationIndex = EVOCATION_NONE;
	m_pulseEvocation[0] = 0;		// 헬운드 강신 남은 시간
	m_pulseEvocation[1] = 0;		// 엘레넨 강신 남은 시간

	m_nExpDamage = 0;

	m_nGoldenBallNoticeStatus = GOLDENBALL_STATUS_NOTHING;

	m_bProcDisconnect = false;

	m_nLastCollectRequest = 0;

#ifdef RESTRICT_PVP_SKILL
	m_nRestrictPvPSkillDelaySec = 0;
#endif // RESTRICT_PVP_SKILL

	m_nEventGomdori2007Win = 0;
	m_nEventGomdori2007Game = 0;
	m_nEventGomdori2007FirstLose = 0;
	m_nEventGomdori2007FirstWin = 0;
	m_nEventGomdori2007Total = 0;
	m_bEventGomdori2007Start = false;
	m_bEventGomdori2007CanSelect = false;
#if defined(LC_BILA)
	m_bEventGomdori2007Drop = gserver->m_pulse;			// 브라질은 pulse 값 저장
#else
	m_bEventGomdori2007Drop = true;
#endif // LC_BRZ

	m_bEventIndependenceDay2007drop = true;
	m_GuildExp	= 0;			// 길드포인트로 전환될 경험치
	m_GuildFame = 0;		// 길드 포인트로 전환될 명성치
	m_nRespondTime = 0;

#ifdef CHAT_BLOCK_USA
	m_shoutPulse = gserver->m_pulse;
#endif // CHAT_BLOCK_USA

	m_bPressCorps = 0;

	m_bPhoenix_Char = 2;

#ifdef NO_CHATTING
	m_nflag = 0;
#endif

	m_Owners_target = NULL;
	m_Slave_npc = NULL;
	m_bTaming_npc = false;
	m_nSoul_Count = 0;
	m_skillTargetCount = 0;
	m_targettype = NULL;
	m_targetindex = NULL;
	m_bCheckAppear = false;

	for (int i = 0; i < MAX_SEAL_TYPE_SKILL; ++i)
	{
		m_sealSkillExp[i].nSkillIdx = -1;
		m_sealSkillExp[i].pSkill = NULL;
		m_sealSkillExp[i].llExp = 0;
	}

	m_Npc_Portal_x	  = -1;
	m_Npc_Portal_z	  = -1;
	m_Npc_Portal_y	  = -1;

	m_nCurrentTitle = TITLE_SYSTEM_NO_TITLE;

	m_nAttendance_exp = 0;

#ifdef XTRAP
	m_xtrapCheckPulse = 1 * PULSE_REAL_SEC; // ITS-5545 XTRAP 접속 후 바로 체크하도록 체크 시간 초기설정을 20초에서 1초로 줄임.
#endif // xTRAP
	int i;
	for(i=0; i<SUMMON_NPC_MAX; i++)
	{
		m_pSummnonNpc[i] = NULL;
	}
	m_SummonOwners_target = NULL;
	m_nBombSummonCnt = 0;

// [110207:selo] 한 벌 의상
	m_suitWearingPos[0] = WEARING_HELMET;
	m_suitWearingPos[1] = WEARING_ARMOR_UP;
	m_suitWearingPos[2] = WEARING_ARMOR_DOWN;
	m_suitWearingPos[3] = WEARING_GLOVE;
	m_suitWearingPos[4] = WEARING_BOOTS;
	m_nKillCount = 0;
	m_nTotalKillCount = 0;
	m_nDeathCount = 0;
	m_nTotalDeathCount = 0;
	m_nWarGroundPoint = 0;
	m_nWarGroundAccPoint = 0;
	m_absorbPVPDamageRate = 0;
	m_decreaseDBufTimeRate = 0;

#ifdef STASH_PASSWORD
	m_stash_password = "";
	m_a_cid = "";
#endif

	m_bRockPaperScissorsFlag = false;
	m_first_inmap = false;

	m_targetPC = NULL;
	m_isSearchLife = false;
	holy_water_item = NULL;
	m_isNotCoolBoxItem = false;
}

CPC::~CPC()
{
	CParty_recall_manager *pInstance = CParty_recall_manager::Get_instance();
	pInstance->Erase_map(this->m_index);

	m_assist.ClearAssist(false, false, true, true, true);

	if (m_petList)
	{
		CPet* petNext = m_petList;
		CPet* pet;
		while ((pet = petNext))
		{
			petNext = pet->m_nextPet;
			DelAttackList(pet);
			delete pet;
		}
		m_petList = NULL;
	}

	if( m_pApetlist )
	{
		CAPet* next = m_pApetlist;
		CAPet* pTempPet;
		while( (pTempPet = next) )
		{
			next = pTempPet->m_pNextPet;
			delete pTempPet;
			pTempPet = NULL;
		}

		m_pApetlist = NULL;
	}

	while (m_elementalList)
		UnsummonElemental(m_elementalList);

	if( m_Friend )
	{
		delete m_Friend;
		m_Friend = NULL;
	}

	if(m_titleList.m_nCount > 0)
	{
		CTitleNode* temp = m_titleList.m_head;
		CTitleNode* nexttitle;
		for(int i = 0; i < m_titleList.m_nCount; i++)
		{
			nexttitle = temp->m_next;
			if (temp)
			{
				delete temp;
				temp = NULL;
				temp = nexttitle;
			}
		}
		m_titleList.m_nCount = 0;
	}

	if(getSyndicateType() == SYNDICATE::eSYNDICATE_KAILUX)
	{
		gserver->m_kailuxUser.erase(this);
	}
	else if(getSyndicateType() == SYNDICATE::eSYNDICATE_DEALERMOON)
	{
		gserver->m_dealermoonUser.erase(this);
	}

	Clean();
}

//#ifdef GER_LOG
//void CPC::SetLoginTime()
//{
//	struct tm loginTime = NOW();
//	m_loginTime = mktime( &loginTime );
//}
//
//void CPC::GetCalcInGameTime(char *_buf, const int _size)
//{
//		time_t logoutTime;
//		struct tm ti = NOW();
//		/*struct tm *ts;*/
//		char tmpbuf[128]="";
//		logoutTime = mktime( &ti );
//		double d_diff;
//
//		int tm_day, tm_hour, tm_min, tm_sec;
//
//		d_diff   = difftime( logoutTime, GetLoginTime() );
//		tm_day   = d_diff / ( 60 *60 * 24);
//		d_diff   = d_diff - ( tm_day *60 *60 *24);
//		tm_hour  = d_diff / ( 60 *60);
//		d_diff   = d_diff - ( tm_hour *60 *60);
//		tm_min   = d_diff / 60;
//		d_diff   = d_diff - ( tm_min *60);
//		tm_sec   = d_diff;
//
//		//time_t gameinTime = logoutTime - (GetLoginTime() );
//		//ts = localtime( &gameinTime );
//		IntCat0(tmpbuf, /*ts->tm_hour*/tm_hour, 2, false);
//		strcat(tmpbuf, ":");
//		IntCat0(tmpbuf, /*ts->tm_min*/tm_min, 2, false);
//		strcat(tmpbuf, ":");
//		IntCat0(tmpbuf, /*ts->tm_sec*/tm_sec, 2, false);
//
//		strncpy( _buf, tmpbuf, _size);
//}
//#endif // END : GER_LOG

void CPC::Clean()
{
	DelAttackList(this);
	DelRaList(this, true);
	CleanSummonNPC();
	CleanTaming();
	CleanExchange();
	CleanExtreamCube();
	CleanParty();
}

void CPC::CleanSummonNPC()
{
	SummonNpcRemoveAll(true);
}

void CPC::CleanTaming()
{
	set_npc_t::iterator it = m_listTaming.begin();
	set_npc_t::iterator endit = m_listTaming.end();
	for(; it != endit; ++it)
	{
		CNPC* p = *(it);

		p->SetOwner(NULL);
		p->m_assist.CureAssist(MST_ASSIST_TAMING, 99);
		DelAttackList(p);
	}
	m_listTaming.clear();

	m_bTaming_npc = false;
}

void CPC::CleanParty()
{
	if(IsParty())
	{
		m_party->SetMemberPCPtr(m_index,NULL);
		m_party = NULL;
	}
	else if(IsExped())
	{
		m_Exped->SetMemberPCPtr(m_index,NULL);
		m_Exped = NULL;
	}
}

void CPC::CleanExchange()
{
	if (m_exchange)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExchangeReqMsg(rmsg, MSG_EXCHANGE_REQ_REJECT, NULL, NULL);
		do_Exchange(this, rmsg);
	}
}

void CPC::CleanExtreamCube()
{
#ifdef EXTREME_CUBE
	if(m_pZone && m_pZone->IsExtremeCube())
	{
		CCubeMemList* memlist = NULL;
		if(gserver->m_extremeCube.IsGuildCubeTime())
		{
			if(m_guildInfo && m_guildInfo->guild())
			{
				memlist = gserver->m_extremeCube.FindMemList(m_guildInfo->guild());
			}
		}
		else
		{
			if(m_party)
			{
				memlist = gserver->m_extremeCube.FindMemList(m_party);
			}
			else
			{
				memlist = NULL;
			}
		}

		if(memlist)
		{
			memlist->DelPC(this);
			GAMELOG << init("CUBE MEMBER DELETE", this)
					<< end;
		}
		else
		{
			GAMELOG << init("CUBE MEMBER DELETE FAIL", this)
					<< end;
		}
	}
#endif // EXTREME_CUBE
}

void CPC::SendStatus()
{
	this->reviseHPMP();
	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatusMsg(rmsg, this);
		SEND_Q(rmsg, m_desc);
	}

	m_bChangeStatus = false;

	// 파티에 속해 있으면 파티원에게도 보낸다
	if (IsParty())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PartyInfoMsg(rmsg, this, true);
		m_party->SendToPCInSameZone(m_pZone->m_index, m_pArea->m_index, rmsg);
	}

	if (IsExped())
	{
		const CExpedMember* pMember = m_Exped->GetMemberByCharIndex(m_index);
		if(pMember)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExpedInfoMsg(rmsg, this, pMember->GetGroupType(), true);
			m_Exped->SendToPCInSameZone(m_pZone->m_index, m_pArea->m_index, rmsg);
		}
	}
}

void CPC::SendElementalStatus()
{
	// 소환수 상태 정보도
	CElemental* pElemental = m_elementalList;
	while (pElemental)
	{
		if (pElemental->m_bChangeStatus)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExElementalStatusMsg(rmsg, pElemental);
			SEND_Q(rmsg, m_desc);
			pElemental->m_bChangeStatus = false;
		}
		pElemental = pElemental->m_nextElemental;
	}
}

void CPC::SendQuickSlot()
{
	for (int i=0; i < QUICKSLOT_PAGE_NUM; i++)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		QuickSlotListMsg(rmsg, this, i);
		SEND_Q(rmsg, m_desc);
	}
}

void CPC::Send(CNetMsg::SP& msg)
{
	if (!m_bPlaying)	return ;
	if (m_desc && msg->m_size)
	{
		SEND_Q(msg, m_desc);
	}
}

bool CPC::AddExpSP(LONGLONG exp, int sp, bool bUseEvent, bool IsQuestExp, bool bNoEffect, bool bArtifact)
{
	if (exp < 0 )
	{
		exp = 0;
	}

	if( sp < 0)
	{
		sp = 0;
	}

	if(this->m_party != NULL && m_assist.FindBySkillIndex(SKILL_GUILD_PARTY_EXP) != 0)
		return false;

	LONGLONG lsp = sp;

	if(gserver->m_bDungeonTimeToggle == true && gserver->m_bIsTime == true)
	{
		// 던전타임은 해당 던전의 npc 기본(original) 경험치가 1.5배가 된다. 그외 지역에선 1배가 계산되므로 경험치 증가에 영향이 없다.
		// 경험치 증가율은 기본 1.5배이지만 운영자의 설정으로 최대 6배가 될 수도 있다.
		if(IsQuestExp == false)
			exp = exp * gserver->m_iZoneExp[this->m_pZone->m_index] / 100;
	}

	// 경험치 증가 제한
	LONGLONG nOrgExp = exp;
	// SP 증가 제한
	LONGLONG nOrgSP = lsp;

#ifdef EVENT_PCBANG_2ND	// 넷띠모 피씨방 경험치 150%
	if(m_desc->m_location == BILL_LOCATION_PCBANG)
	{
		exp = exp * 130 / 100;
	}
#endif // EVENT_PCBANG_2ND

	if (bUseEvent)
	{
#ifdef EVENT_DOUBLE
		// 더블 이벤트
		if (gserver->m_bDoubleEvent)
		{
#ifdef NEW_DOUBLE_EVENT_AUTO
			if(!gserver->m_bDoubleEventAutoOn)
#endif // NEW_DOUBLE_EVENT_AUTO
			{
#ifdef NEW_DOUBLE_GM_ZONE
				if(gserver->m_bDoubleEventZone == -1 || (gserver->m_bDoubleEventZone != -1 && gserver->m_bDoubleEventZone == m_pZone->m_index) )
#endif // NEW_DOUBLE_GM_ZONE
				{
					exp = exp * gserver->m_bDoubleExpPercent / 100;
					lsp = lsp * gserver->m_bDoubleSpPercent / 100;
				}
			}
		}
#endif

#ifdef NEW_DOUBLE_EVENT_AUTO
		if(gserver->m_bDoubleEventAutoOn && m_pZone->m_index == ZONE_COMBO_DUNGEON)
		{
			LONGLONG oldexp;
			LONGLONG oldsp;

			oldexp = exp;
			oldsp = lsp;

			exp *= 2;
			lsp *= 2;

			if(m_admin == 10)
			{
				GAMELOG << init("DoubleEventAuto", this)
						<< "oldexp" << delim
						<< oldexp << delim
						<< "oldsp" << delim
						<< oldsp << delim
						<< "exp" << delim
						<< exp << delim
						<< "sp" << delim
						<< lsp << end;
			}
		}
#endif // NEW_DOUBLE_EVENT_AUTO

		// 경험치 더블 이벤트
		if (gserver->m_bDoubleExpEvent)
		{
			exp = (exp * gserver->m_expPercent ) / 100;
		}

		// 떡국
		if (m_assist.m_avAddition.hcExpPlus)
			exp = exp * NEWYEAR_EVENT_RICESOUP_UP_EXP / 10;

		// 떡만두국
		if (m_assist.m_avAddition.hcSPPlus)
			lsp = lsp * NEWYEAR_EVENT_RICEMANDO_UP_SP / 10;

		// 수박
		if (m_assist.m_avAddition.hcExpPlus_836)
			exp = exp * 3 / 2;

		// 참외
		if (m_assist.m_avAddition.hcSPPlus_837)
			lsp = lsp * 2;

		// 경험의스크롤
		if (m_assist.m_avAddition.hcScrollExp)
			exp = exp * 3 / 2;

		// 노력의스크롤
		if (m_assist.m_avAddition.hcScrollSP)
			lsp = lsp * 3 / 2;

		if( !IsQuestExp )
		{
#if defined( LC_GAMIGO ) || defined(LC_USA)
			// GAMIGO는 경험치 업데이트 안함
#else
			exp = exp + ( exp * m_nAttendance_exp / 100 );
#endif

#if defined( LC_GAMIGO ) || defined(LC_USA) || defined(LC_TLD)
			lsp	= lsp + ( lsp * m_nAttendance_exp / 100 );
#else
			// 숙련도는 GAMIGO 이외의 국가만 적용
			// [110106: selo] 홍콩은 경험치, 숙련도 모두 적용
			// [111226: swkwon] 태국은 경험치, 숙련도 모두 적용
#endif // LC_GAMIGO

		}

		// 060227 : bs : 유료 경험치 향상제
		if (m_assist.m_avRate.hcCashExpUp > 0)
			exp += exp * m_assist.m_avRate.hcCashExpUp / SKILL_RATE_UNIT;

		// 경험치 포션 : 1.5배
		if (m_assist.m_avAddition.hcExpPlus_S354)
			exp = exp * 3 / 2;

		// 추천서버포션 경험치 상승
		if (m_assist.m_avAddition.hcExpPlus_S355)
			exp = exp * 3 / 2;

		// 추천서버포션 SP 상승
		if (m_assist.m_avAddition.hcSPPlus_S356)
			lsp = lsp * 3 / 2;

		// 경험치 상승 % 단위 누적 적용
		if (m_assist.m_avAddition.hcExpPlusPer100 > 0)
			exp += exp * m_assist.m_avAddition.hcExpPlusPer100 / 100;

		// 숙련도 상승 %단위 누적 적용
		if (m_assist.m_avAddition.hcSPPlusPer100 > 0)
			lsp += lsp * m_assist.m_avAddition.hcSPPlusPer100 / 100;

		// 말레이시아 경험치 포션 : 유료 아이템 : 1840, 1841, 1842 : 단위는 %
		if (m_assist.m_avAddition.hcExpPlus_398 > 0)
			exp += exp * m_assist.m_avAddition.hcExpPlus_398 / 100;

		// 케이크 : 2006 크리스마스
		if (m_assist.m_avAddition.hcExpPlus_1975 > 0)
			exp += exp * m_assist.m_avAddition.hcExpPlus_1975 / 100;

		// 눈사람 : 2006 크리스마스
		if (m_assist.m_avAddition.hcExpPlus_1976 > 0)
			exp += exp * m_assist.m_avAddition.hcExpPlus_1976 / 100;

		if (m_assist.m_avAddition.hcExpSPPlus_2354)
		{
			exp += exp * 50/100;
			lsp += lsp * 50/100;
		}

		if( m_assist.m_avAddition.hcExpPlus_2388 > 0 )
			exp += exp * m_assist.m_avAddition.hcExpPlus_2388 / 100;

		if( m_assist.m_avAddition.hcExpPlus_2389 > 0 )
			exp += exp * m_assist.m_avAddition.hcExpPlus_2389 / 100;

		if( m_assist.m_avAddition.hcSPPlus_2390 > 0 )
			lsp += lsp * m_assist.m_avAddition.hcSPPlus_2390 / 100;

#ifdef NON_PVPPANALTY_EXPSPUP
#ifdef MAL_DISABLE_PKPENALTY
		if(gserver->m_bDisablePKPaenalty)
		{
			exp += exp * 10/100;
			lsp += lsp * 10/100;
		}
#endif // MAL_DISABLE_PKPENALTY
#endif // NON_PVPPANALTY_EXPSPUP
	}

	if (m_assist.FindBySkillIndex(654))
	{
		if (m_level < 60)
		{
			exp *= 2;
			lsp  *= 2;
		}
	}

	if( 0 != m_opExpRate )
		exp += (exp * m_opExpRate / 100);
	if( 0 != m_opSPRate )
		lsp += (lsp * m_opSPRate / 100);

	exp += m_assist.m_avAddition.exp;
	lsp += m_assist.m_avAddition.sp;

	if( 0 != m_assist.m_avAddition.exp_rate )
		exp = exp * m_assist.m_avAddition.exp_rate / 100;
	if( 0 != m_assist.m_avAddition.sp_rate )
		lsp = lsp * m_assist.m_avAddition.sp_rate / 100;

#ifdef NEW_DOUBLE_EVENT_AUTO
	if(gserver->m_bDoubleEventAutoOn && m_pZone->IsComboZone())
	{
		if(exp >= nOrgExp * 4)
			exp = nOrgExp * 4;
		if(lsp >= nOrgSP * 4)
			lsp = nOrgSP * 4;
		if(m_admin == 10)
		{
			GAMELOG << init("MonsterCombo Event 400 Limit", this)
					<< "OrgEXP" << delim
					<< nOrgExp << delim
					<< "OrgSP" << delim
					<< nOrgSP << delim
					<< "EXP" << delim
					<< exp << delim
					<< "SP" << delim
					<< lsp << end;
		}
	}
	else
#endif // NEW_DOUBLE_EVENT_AUTO

	{
		LONGLONG max_limitEXP = 0;
		LONGLONG max_limitSP  = 0;
		max_limitEXP = MAX( gserver->m_nExpLimit , m_assist.m_avAddition.hcLimitEXP ) + m_assist.m_avAddition.hcExplimitPlus;
		max_limitSP = MAX( gserver->m_nSpLimit , m_assist.m_avAddition.hcLimitSP );

		if (exp >= nOrgExp * (max_limitEXP)/100)
			exp = nOrgExp * (max_limitEXP)/100;
		if (lsp >= nOrgSP * max_limitSP / 100)
			lsp = nOrgSP * max_limitSP / 100;

		if( max_limitEXP < 300 )
		{
			if( this->m_assist.FindByItemIndex(1416) || this->m_assist.FindByItemIndex(2658)
					|| this->m_assist.FindByItemIndex(5085) || this->m_assist.FindByItemIndex(5086) || this->m_assist.FindByItemIndex(5087)
					|| this->m_assist.FindByItemIndex(4966)
			  )
			{
				if (m_assist.m_avRate.hcCashExpUp > 0)
					exp = nOrgExp + nOrgExp * m_assist.m_avRate.hcCashExpUp / SKILL_RATE_UNIT;
			}
		}

		if( max_limitSP < 300 )
		{
			// 플래티늄 숙련의 묘약 사용때만 300% 리미트
			if(this->m_assist.FindByItemIndex(2453) || this->m_assist.FindByItemIndex(5088) ||
					this->m_assist.FindByItemIndex(5089) || this->m_assist.FindByItemIndex(5090)||
					this->m_assist.FindByItemIndex(7611) )
			{
				if(m_assist.m_avAddition.hcSPPlusPer100 > 0)
				{
					lsp = nOrgSP + nOrgSP * m_assist.m_avAddition.hcSPPlusPer100 / SKILL_RATE_UNIT;
				}
			}
		}
	}

	if(IsQuestExp)
	{
		if(exp > nOrgExp * 3)
			exp = nOrgExp * 3;
		if(lsp > nOrgSP * 3)
			lsp = nOrgSP * 3;
	}
	else
	{
		LONGLONG nExtraExp = 0;
		// 아이리스의 열정은 경험치 제한및 모든 계산이 끝난 후에 nOrgExp 에 30% 만큼 exp 를 증가시켜 준다
		if( m_assist.m_avAddition.hcIrisFervor == true )
		{
			m_assist.CureByItemIndex(7345);
			m_assist.CureByItemIndex(7346);
			// org 에 30% 추가
			nExtraExp = nOrgExp / 10 * 3;
		}
		exp = exp + nExtraExp;
	}

	if(bNoEffect)
	{
		exp = nOrgExp;
		lsp = nOrgSP;
	}

#ifdef IMP_SPEED_SERVER
	// 최종에 최종 LIMIT 무시
	if( gserver->m_bSpeedServer )
	{
		LONGLONG	_ZoneExp = exp * m_pZone->GetZoneExp() / 100 ;
		LONGLONG	_ZoneSp  = lsp * m_pZone->GetZoneSp() / 100;

		if( _ZoneExp > exp )
			exp = _ZoneExp;
		if( _ZoneSp > lsp )
			lsp = _ZoneSp;
	}
#endif //IMP_SPEED_SERVER

	sp = (int)lsp;

	//유물 아이템 효과적용을 하기전에 경험치 증폭제, 또는 퀘스트 보상일 경우에는 적용하지 않는다.
	if(IsQuestExp == false
		&& bArtifact == false
		&& m_assist.m_avAddition.hcExpPlus_836 == false
		&& m_assist.FindByItemIndex(4939) == false)
	{
		exp = exp + exp * this->m_artiExp / 100;
	}

	if (m_level <= MAX_LEVEL)
	{
		m_exp += exp;
		m_exp_weekly += exp;
	}
	if (m_level >= MAX_LEVEL && m_exp > GetLevelupExp(m_level))
		m_exp = GetLevelupExp(m_level);

	if (m_skillPoint + sp > 0)
		m_skillPoint += sp;

	CAPet *apet = GetAPet();
	if( apet )
	{
		apet->AddAccExp( exp, GetLevelupExp(m_level) );
	}
#if defined (LC_TLD)
	if(exp == 1)
	{
		GAMELOG << init("GET EXP", this) << "EXP" << delim << exp << delim << "SP" << delim << sp << end;
	}
#endif // LC_TLD

	{
		// Exp, SP 메시지 전송
		CNetMsg::SP rmsg(new CNetMsg);
		ExpSPMsg(rmsg, exp, sp);
		SEND_Q(rmsg, m_desc);
	}

	if (m_skillPoint > MAX_SKILLPOINT)
		m_skillPoint = MAX_SKILLPOINT;

	int zoneindex = this->m_pZone->m_index;

	CalcLevelup();

	if( zoneindex != this->m_pZone->m_index)
		return true;

	return false;
}

void CPC::AddExpSP_Direct( LONGLONG exp, LONGLONG sp )
{
	m_exp += exp;
	m_skillPoint += sp;

	if (m_level >= MAX_LEVEL && m_exp > GetLevelupExp(m_level))
		m_exp = GetLevelupExp(m_level);

	if (m_skillPoint > MAX_SKILLPOINT)
		m_skillPoint = MAX_SKILLPOINT;

	CalcLevelup();
}
void CPC::CalcLevelup()
{
	if (GetLevelupExp(m_level) <= m_exp && m_level < MAX_LEVEL)
	{
		do
		{
#ifdef GER_LOG
			GAMELOGGEM << init( 0, "CHAR_LEVEL_UP")
					   << LOG_VAL("account-id", this->m_desc->m_idname ) << blank
					   << LOG_VAL("character-id", this->m_desc->m_pChar->m_name ) << blank
					   << LOG_VAL("level", m_level+1 ) << blank
					   << endGer;
#endif // GER_LOG
			GAMELOG << init("LEVEL UP", this)
					<< m_level
					<< " ==> "
					<< m_level + 1
					<< end;
			m_exp -= GetLevelupExp(m_level);
			m_level++;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				SubHelperLevelup(rmsg, this->m_index);
				SEND_Q(rmsg, gserver->m_subHelper);
			}

			// stat point 증가
			if (m_job2)
			{
				m_statpt_remain += 3;
				if ((m_level - JOB_2ND_LEVEL) % 2 == 0)
					m_statpt_remain += 1;
			}
			else
				m_statpt_remain++;

			{
				// STAT POINT
				CNetMsg::SP rmsg(new CNetMsg);
				StatPointRemainMsg(rmsg, this);
				SEND_Q(rmsg, m_desc);
			}

			// 레벨업 효과
			LevelUp(true);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperPartyMatchMemberChangeInfoMsg(rmsg, m_index, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL, "", m_level, 0);
				SEND_Q(rmsg, gserver->m_helper);
			}
		}
		while (GetLevelupExp(m_level) <= m_exp);

		CalcStatus(true);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::makeLevelupInfo(rmsg, m_level);
			SEND_Q(rmsg, m_desc);
		}

		Restore();
	}
	return;
}

void CPC::Restore()
{
	m_hp = m_maxHP;
	m_mp = m_maxMP;

	m_bChangeStatus = true;
}

void CPC::InitStat()
{
	int i;
	int level;
	int upstr, updex, upint, upcon;

	if (m_job2 == 0)
	{
		switch (m_job)
		{
		case JOB_TITAN:
		case JOB_KNIGHT:
		case JOB_HEALER:
		case JOB_MAGE:
		case JOB_ROGUE:
		case JOB_SORCERER:
		case JOB_NIGHTSHADOW:
#ifdef EX_ROGUE
		case JOB_EX_ROGUE:
#endif // EX_ROGUE
#ifdef EX_MAGE
		case JOB_EX_MAGE:
#endif // EX_MAGE
			level = m_level - 1;
			i = m_job;

			upstr = levelup_stat[i][0] * level + m_statpt_str;
			updex = levelup_stat[i][1] * level + m_statpt_dex;
			upint = levelup_stat[i][2] * level + m_statpt_int;
			upcon = levelup_stat[i][3] * level + m_statpt_con;

			m_str		= m_dbStr		= base_stat[i][0] + upstr;
			m_dex		= m_dbDex		= base_stat[i][1] + updex;
			m_int		= m_dbInt		= base_stat[i][2] + upint;
			m_con		= m_dbCon		= base_stat[i][3] + upcon;

			m_maxHP		= m_dbHP		= base_hp[i]
										  + upstr * levelup_hp[i][0]
										  + updex * levelup_hp[i][1]
										  + upint * levelup_hp[i][2]
										  + upcon * levelup_hp[i][3];
			m_maxMP		= m_dbMP		= base_mp[i]
										  + upstr * levelup_mp[i][0]
										  + updex * levelup_mp[i][1]
										  + upint * levelup_mp[i][2]
										  + upcon * levelup_mp[i][3];

			m_walkSpeed = walk_speed[i];
			//m_runSpeed = run_speed[i];
			SetRunSpeed(run_speed[i]);
			m_flySpeed = 15.0f;		// 기본 나는 속도
			break;
		default:
			{
				return ;
			}
			break;
		}
	}
	else
	{
		switch (m_job)
		{
		case JOB_TITAN:
			switch (m_job2)
			{
			case JOB_2ND_HIGHLANDER:
				i = 0;
				break;
			case JOB_2ND_WARMASTER:
				i = 1;
				break;
			default:
				{
					return ;
				}
				break;
			}
			break;
		case JOB_KNIGHT:
			switch (m_job2)
			{
			case JOB_2ND_ROYALKNIGHT:
				i = 2;
				break;
			case JOB_2ND_TEMPLEKNIGHT:
				i = 3;
				break;
			default:
				{
					return ;
				}
				break;
			}
			break;
		case JOB_HEALER:
			switch (m_job2)
			{
			case JOB_2ND_ARCHER:
				i = 4;
				break;
			case JOB_2ND_CLERIC:
				i = 5;
				break;
			default:
				{
					return ;
				}
				break;
			}
			break;
		case JOB_MAGE:
			switch (m_job2)
			{
			case JOB_2ND_WIZARD:
				i = 6;
				break;
			case JOB_2ND_WITCH:
				i = 7;
				break;
			default:
				{
					return ;
				}
				break;
			}
			break;
		case JOB_ROGUE:
			switch (m_job2)
			{
			case JOB_2ND_ASSASSIN:
				i = 8;
				break;
			case JOB_2ND_RANGER:
				i = 9;
				break;
			default:
				{
					return ;
				}
				break;
			}
			break;
		case JOB_SORCERER:
			switch (m_job2)
			{
			case JOB_2ND_ELEMENTALLIST:
				i = 10;
				break;
			case JOB_2ND_SPECIALLIST:
				i = 11;
				break;
			default:
				{
					return ;
				}
				break;
			}
			break;
		case JOB_NIGHTSHADOW:
			i = 12;			// JOB_2ND_NIGHTSHADOW
			break;
#ifdef EX_ROGUE
		case JOB_EX_ROGUE:
			switch (m_job2)
			{
			case JOB_2ND_EX_ASSASSIN:
				i = 13;
				break;
			case JOB_2ND_EX_RANGER:
				i = 14;
				break;
			}
			break;
#endif // EX_ROGUE
#ifdef EX_MAGE
		case JOB_EX_MAGE:
			switch (m_job2)
			{
			case JOB_2ND_EX_WIZARD:
				i = 15;
				break;
			case JOB_2ND_EX_WITCH:
				i = 16;
				break;
			}
			break;
#endif // EX_MAGE
		default:
			{
				return ;
			}
			break;
		}

		level = m_level - JOB_2ND_LEVEL;
		if( level < 0 )
			level=0;

		upstr = levelup_stat2[i][0] * level;
		updex = levelup_stat2[i][1] * level;
		upint = levelup_stat2[i][2] * level;
		upcon = levelup_stat2[i][3] * level;

		m_str		= m_dbStr		= base_stat2[i][0] + upstr + m_statpt_str;
		m_dex		= m_dbDex		= base_stat2[i][1] + updex + m_statpt_dex;
		m_int		= m_dbInt		= base_stat2[i][2] + upint + m_statpt_int;
		m_con		= m_dbCon		= base_stat2[i][3] + upcon + m_statpt_con;

		switch (m_job)
		{
		case JOB_KNIGHT:
			m_maxHP		= m_dbHP		= base_hp2[i] + upcon * levelup_hp2[i] + m_statpt_con * 3 + level * 29;
			m_maxMP		= m_dbMP		= base_mp2[i] + upcon * levelup_mp2[i] + m_statpt_con * 3 + level * 10;
			break;
		case JOB_ROGUE:
#ifdef EX_ROGUE
		case JOB_EX_ROGUE:
#endif // EX_ROGUE
			m_maxHP		= m_dbHP		= base_hp2[i] + upcon * levelup_hp2[i] + m_statpt_con * 3 + level * 31;
			m_maxMP		= m_dbMP		= base_mp2[i] + upcon * levelup_mp2[i] + m_statpt_con * 3 + level * 10;
			break;
		case JOB_SORCERER:
			if (m_job2 == JOB_2ND_ELEMENTALLIST)
				m_maxHP	= m_dbHP		= base_hp2[i] + upcon * levelup_hp2[i] + m_statpt_con * 3 + level * 10;
			else
				m_maxHP	= m_dbHP		= base_hp2[i] + upcon * levelup_hp2[i] + m_statpt_con * 3 + level * 32;
			m_maxMP		= m_dbMP		= base_mp2[i] + upcon * levelup_mp2[i] + m_statpt_con * 3 + level * 10;
			break;
		case JOB_NIGHTSHADOW:
			m_maxHP		= m_dbHP		= base_hp2[i] + (26*level) + (m_statpt_con*2 ) + (upcon * 2) ;
			m_maxMP		= m_dbMP		= base_mp2[i] + (10*level) + (m_statpt_con*5 ) + (upcon * 5) ;
			break;
		default:
			m_maxHP		= m_dbHP		= base_hp2[i] + upcon * levelup_hp2[i] + m_statpt_con * 3 + level * 26;
			m_maxMP		= m_dbMP		= base_mp2[i] + upcon * levelup_mp2[i] + m_statpt_con * 3 + level * 10;
			break;
		}

		m_walkSpeed = walk_speed[(int)m_job];
		//m_runSpeed = run_speed[(int)m_job];
		SetRunSpeed(run_speed[(int)m_job]);
		m_flySpeed = 15.0f;
	}
}

/*
	Description
		create item and give it to pc

	Param
		IN		int			itemIndex		// item db index
		IN		int			plus			// item plus
		IN		int			flag			// item flag
		IN		LONGLONG	itemNum			// item count
		IN		bool		ndrop			// can drop field
		IN		bool		nOtion			// can plus option

	Return
		type bool
		if item create and give success, return true, otherwise return false
*/
bool CPC::GiveItem(int itemIndex, int plus, int flag, LONGLONG itemNum, bool bdrop, bool bOption)
{
	CItem* item = NULL;
	item = gserver->m_itemProtoList.CreateItem(itemIndex, -1, plus, flag, itemNum);

	if (!item)
	{
		//////////////////////////////
		// item 생성 실패
//		GAMELOG << init("CREATE_AND_GIVE_ITEM_ERROR CANNOT CREATE", this)
//					<< itemIndex << delim
//					<< itemNum << delim
//					<< end;
		return false;
	}

	bool bIsRare = false;

	if (item->IsRareItem())
	{
		bOption = false;
		bIsRare = true;
	}

	if (item->IsOriginItem())
	{
		bOption = false;
		bIsRare = false;
	}

	if( bOption )
	{
		const static int Option_table[5][4] =
		{
			{ 22, 12 ,13, 18 },		// 근력의 반지 옵션 타입 a_type
			{ 23, 14, 15, 18 },		// 민첩의 목걸이 옵션 타입 a_type
			{ 7, 11, 18, 20 },		// 지혜의 귀걸이 옵션 타입 a_type
			{ 20, 8, 11, 18 },		// 생명의 목걸이 옵션 타입 a_type
			{ 0, 1, 2, 3}
		};		// 영혼의 반지 옵션 타입 a_type

		int Option_type = 0;
		switch( itemIndex )
		{
		case 1823:
			break;
		case 1824:
			Option_type = 1;
			break;
		case 1825:
			Option_type = 2;
			break;
		case 1826:
			Option_type = 3;
			break;
		case 1827:
			Option_type = 4;
			break;
		}

		for (int i=0; i < MAX_ITEM_OPTION - 1; i++)
		{
			if ( Option_table[Option_type][i] < 0 || Option_table[Option_type][i] >= MAX_NUM_OPTION )
				continue;

			item->m_option[i].m_proto = gserver->m_optionProtoList.FindProto(Option_table[Option_type][i]);

			if (!item->m_option[i].m_proto)
				continue;

			item->m_option[item->m_nOption].m_type = Option_table[Option_type][i];
			item->m_option[item->m_nOption].m_level = 4;
			item->m_option[item->m_nOption].m_value = item->m_option[i].m_proto->m_levelValue[item->m_option[i].m_level - 1];
			item->m_option[item->m_nOption].SetDBValue();
			++item->m_nOption;
		}
	}
	else if (!bIsRare)
	{
		if( item->IsAccessary()  && item->m_nOption < 1)
		{
			OptionSettingItem( this, item );
		}
	}

	if (this->m_inventory.addItem(item) == false)
	{
		if( !bdrop )
		{

			CNetMsg::SP rmsg(new CNetMsg);
			SysFullInventoryMsg(rmsg, 0);
			SEND_Q(rmsg, this->m_desc);
			delete item;
			return false;
		}

		// 인벤토리 가득참
		item = this->m_pArea->DropItem(item, this);
		if (!item)
			return false;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg( rmsg, MSG_SYS_FULL_INVENTORY_AND_DROP );
			SEND_Q( rmsg, this->m_desc );
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			item->m_preferenceIndex = this->m_index;
			ItemDropMsg(rmsg, this, item);
			this->m_pArea->SendToCell(rmsg, this, true);
		}
	}

	return true;
}

void CPC::LevelUp(bool bSendEffect)
{
	if(m_level > 45)	// 45레벨 초과시 초보자용 목걸이 벗기기
	{
		int newbie_item[9] =
		{
			2669,	// 타이탄 목걸이
			2670,	// 나이트 목걸이
			2671,	// 힐러 목걸이
			2672,	// 메이지 목걸이
			2673,	// 로그 목걸이
			2674,	// 소서러 목걸이
			-1,
			2673,
			2672,
		};

		int wear_position = -1;
		for(int i=WEARING_ACCESSORY1; i <= WEARING_ACCESSORY3; i++)
		{
			if(m_wearInventory.wearItemInfo[i] != NULL
					&& m_wearInventory.wearItemInfo[i]->m_itemProto != NULL
					&& m_wearInventory.wearItemInfo[i]->m_itemProto->getItemIndex() == newbie_item[m_job])
			{
				wear_position = i;
				break;
			}
		}

		if(wear_position != -1)
		{
			CItem * pItem = m_wearInventory.wearItemInfo[wear_position];
			this->m_wearInventory.DelNormalItem(wear_position);
			{
				CNetMsg::SP rmsg(new CNetMsg);
				WearingMsg(rmsg, this, wear_position, -1, 0);
				m_pArea->SendToCell(rmsg, this, false);
			}
		}
	}

	InitStat();
	CalcStatus(true);

	// 레벨업 효과 메시지 전송
	if (bSendEffect)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EffectEtcMsg(rmsg, this, MSG_EFFECT_ETC_LEVELUP);
		this->m_pArea->SendToCell(rmsg, this, true);
	}

	// 사제 시스템 체크
	if ( MSG_TEACH_STUDENT_TYPE == m_teachType )
	{
		CPC* tch = PCManager::instance()->getPlayerByCharIndex(m_teachIdx[0]);

		// 우선 버프부터 줘보장. 무기 그리고 마지막 호칭과 명성치.
		int skillLevel = -1;
		int Fame = 0;
		switch( m_level )
		{
		case 10:
			skillLevel = 1;
			break;	// exp + 50% 버프 (24시간)
		case 20:
			{
				// + 15무기 지급
				int i=0;
				for(i=0; i<2; i++)
				{
					if( false == GiveItem(student20LEVELGiftTable[m_job][i], 15, 0, 1, true, false))
					{
						GAMELOG << init("STUDENT_LEVELUP_ITEM_GIVE_FAIL", this)
								<< student20LEVELGiftTable[m_job][i] << end;
					}
				}
			}
			skillLevel = 2; 	// sp + 50% 버프 (24시간)
			break;
		case 30:
			skillLevel = 3;
			break; // exp + 100% 버프 (24시간)
		case 35:
			Fame = 2;
			break;	// 후견인 명성치 2
		case 40:
			skillLevel = 4;		// sp + 100% 버프 (24시간)
			Fame = 3;
			break;  // 후견인 명성치 3
		case 45:
			skillLevel = 5;		// exp,sp + 100% 버프 (24시간)
			Fame = 5;
			break;  // 후견인 명성치 5
		case 50:
			{
				// 견습생 클래스별 아이템 지급 ( 기존 5953 포함 )
#ifdef EX_ROGUE
#ifdef EX_MAGE
				const int nGiveItem[JOBCOUNT] = { 6878, 6879, 6880, 6881, 6882, 6883, NAS_ITEM_DB_INDEX, 6882, 6881 };
#else
				const int nGiveItem[JOBCOUNT] = { 6878, 6879, 6880, 6881, 6882, 6883, NAS_ITEM_DB_INDEX, 6882 };
#endif
#else
				const int nGiveItem[JOBCOUNT] = { 6878, 6879, 6880, 6881, 6882, 6883, NAS_ITEM_DB_INDEX };
#endif // EX_ROGUE
				if( false == GiveItem(nGiveItem[(int)this->m_job], 0, 0, 1, true, false))
				{
					GAMELOG << init("STUDENT_LEVELUP_ITEM_GIVE_FAIL", this)	<< nGiveItem[(int)this->m_job] << end;
				}

				// 후견인 리뉴얼 이후 보상에서 제외 되었던 호칭은 가미고에서는 지급하기로 결정

#if defined(LC_GAMIGO) // 120222 가미고 요청으로 호칭 아이템 지급
				if( false == GiveItem(5953, 0, 0, 1, true, false))
				{
					GAMELOG << init("STUDENT_LEVELUP_ITEM_GIVE_FAIL", this)	<< 5953 << end;
				}
#endif // LC_GAMIGO 120222 가미고 요청으로 호칭 아이템 지급

				// 후견인은 접속중인가?
				if( this->m_teachIdx[0] == -1 )
					break;

				bool bHelper = false;
				CPC * tch = PCManager::instance()->getPlayerByCharIndex(this->m_teachIdx[0]);
				if( tch ) // 후견인이 접속중이면 (5952)후견인의 증표 지급.
				{
					tch->m_cntCompleteStudent++;
					tch->m_cntTeachingStudent--;
					if( tch->m_cntTeachingStudent < 0)
						tch->m_cntTeachingStudent = 0;
					if( false == tch->GiveItem(5952, 0, 0, 1, false, false))
					{
						bHelper = true;
					}
				}
				else
				{
					bHelper = true;
				}

				// 후견인이 접속중이지 않으면 DB에 기록해서 나중에 받아가게끔 처리하장.
				if( bHelper )
				{
					if(gserver->isRunHelper())
					{
						CNetMsg::SP rmsg(new CNetMsg);
						HelperTeacherGiftAddReqMsg( rmsg, this->m_teachIdx[0] );
						SEND_Q(rmsg, gserver->m_helper);
					}
					else
					{
						GAMELOG << init("TEACH SUC GIFT GIVE ERROR", this )
								<< "teacher index " << delim << this->m_teachIdx[0] << end;
					}
				}

				// 그외 기타 이벤트용 지급

#ifdef EVENT_CHILDERN_DAY
				if(true == GiveItem(NAS_ITEM_DB_INDEX, 0, 0, 500000, true))
				{
					GAMELOG << init("EVENT_CHILDERN_DAY", this)
							<< NAS_ITEM_DB_INDEX
							<< end;
				}
#endif // EVENT_CHILDERN_DAY

				// 지급이 완료 되었다면 완료 메시지를 보내장~

				{
					CNetMsg::SP rmsg(new CNetMsg);
					TeachEndMsg(rmsg, m_teachIdx[0], m_teachName[0], m_index, this->GetName(), MSG_TEACH_END_SUCCESS);
					if( tch )
					{
						RefMsg(rmsg) << tch->m_cntTeachingStudent
									 << tch->m_cntCompleteStudent
									 << tch->m_cntFailStudent;

						SEND_Q(rmsg, tch->m_desc);

						int i;
						for(i=0; i< TEACH_MAX_STUDENTS; i++)
						{
							if( tch->m_teachIdx[i] == this->m_index )
								tch->m_teachIdx[i] = -1;
						}
					}
					SEND_Q(rmsg, m_desc);
				}

				// DB를 깨끗이 세팅 하자 ㅡㅡ;;
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperTeachStudentSucMsg(rmsg, this->m_index, m_teachIdx[0]);
					SEND_Q(rmsg, gserver->m_helper);
				}

				if( !tch )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperStuentListRefresh( rmsg, this->m_teachIdx[0] ); // 리스트를 갱신하도록 메시지를 보낸다
					SEND_Q(rmsg, gserver->m_helper);
				}

				// 혼자 셋팅 해제
				memset(m_teachIdx, -1, sizeof(int)*TEACH_MAX_STUDENTS );
				m_teachJob[0] = -1;
				m_teachJob2[0] = 0;
				m_teachLevel[0] = 0;
				m_teachName[0][0]= '\0';
				m_teachTime[0] = 0;
				m_teachType = MSG_TEACH_NO_TYPE;
			}
			break;
		}

		// 스킬 레벨이 있다면 적용 한다.
		if( skillLevel != -1)
		{
			// 1061 후견인의 축복
			CSkill * pSkill = gserver->m_skillProtoList.Create(1061, skillLevel);
			if( pSkill )
			{
				bool bApply;
				ApplySkill(this, this, pSkill, -1, bApply );
				delete pSkill;
				pSkill = NULL;
			}
		}

		//if( (m_level % 5) == 0 ) // 5레벨 단위로 레벨업 했다는 메시지를 보낸다.
		{
			CNetMsg::SP rmsg(new CNetMsg);
			TeachStudentLevelUPMsg(rmsg, this);
			SEND_Q(rmsg, m_desc);
			if( tch )
				SEND_Q(rmsg, tch->m_desc);
		}

		if( Fame > 0)
		{
			if(gserver->isRunHelper())
			{
#if defined (EVENT_TEACH) || defined ( EVENT_CHILDERN_DAY )
				Fame = Fame * 2;
#endif

				/*
				길드 마스터가 길드워의 명성치 상납 정도를 설정해두면,
				그 이후부터 길드원이 획득하는 명성치의 일부가 아래와 같이 계산되어 자동으로 길드포인트로 전환된다.

				명성치의 길드 포인트 전환 비율 = 상납될 명성치 10 당 GP 1

				길드에 가입한 캐릭터는 상압 정도가 설정된 이후부터 획득한 명성치 중 상납될 명성치가 따로 기억되어야 하며,
				상납될 명성치가 쌓여 10이 넘을 때 마다 자동으로 GP 1로 변환하여 길드포인트로 등록된다.

				Ex. 나의 명성치 상납 % -> 50%
				견습생 2명을 졸업 시킬시 획득하는 명성치가 20 이라고 가정
				실제 캐릭터가 획득하는 명성치 = 20 * 0.5 =10
				길드 포인트로 상납될 경험치 = 20 * 0.5 = 10 따라서 GP 1

				해당 캐릭터가 길드에서 탈퇴해도 이미 변환되어 등록된 GP는 회수되지 않으며,
				10이 되지 않아 변환 되지 않고 남아 있는 상납 명성치는 초기화 되지 않고 그대로 보관된다.
				*/
				if(tch && NULL != tch->m_guildInfo
						&& NULL != tch->m_guildInfo->guild())
				{
					// 길드가 있는 경우
					// 1-1. 길마가 정한 상납 명성치 %가 존재
					if(0 != tch->m_guildInfo->GetcontributeFame())
					{
						int guildindex = tch->m_guildInfo->guild()->index();
						CGuild * guild = gserver->m_guildlist.findguild(guildindex);

						// 길드에 상납하는 명성치
						int GuildFame = Fame * tch->m_guildInfo->GetcontributeFame() / 100;
						// 캐릭터가 받는 명성치
						Fame = Fame - GuildFame;

						// 상납될 GP
						tch->m_GuildFame += GuildFame;
						int GP = tch->m_GuildFame / 10;
						// 상납 변환 대기 명성치
						tch->m_GuildFame = tch->m_GuildFame % 10;

						// 상납될 GP가 있는 경우
						if(GP > 0 )
						{
							guild->AddGuildPoint( GP );				// 길드에 GP 추가
							tch->m_guildInfo->AddCumulatePoint( GP );	// 자신이 바친 GP 총합

							{
								CNetMsg::SP rmsg(new CNetMsg);
								HelperNewGuildPointUpdate( rmsg, tch->m_index, guild->index(), GP );
								SEND_Q(rmsg, gserver->m_helper);
							}

							{
								CNetMsg::SP rmsg(new CNetMsg);
								HelperSaveGuildMemberPointMsg( rmsg, guild->index(), tch->m_index, GP );
								SEND_Q( rmsg, gserver->m_helper );
							}
						}	// end if 상납될 GP가 있는 경우
					}	// end if 길마가 정한 상납 명성치 %가 존재
				}	// end if 길드가 있는 경우

				if( gserver->isRunHelper())
				{
					GAMELOG << init("TEACH_FAME_UP", this)
							<< "frame : " << Fame << end;

					CNetMsg::SP rmsg(new CNetMsg);
					HelperTeachFameUpReqMsg(rmsg, m_teachIdx[0], this->m_index, this->GetName(), Fame);
					SEND_Q(rmsg, gserver->m_helper);
				}
				else
				{
					GAMELOG << init("TEACH_FAME_UP_NO_HELPER", this)
							<< "frame : " << Fame << end;
				}
			}
		}
	}

	if((m_teachType == MSG_TEACH_STUDENT_TYPE
			|| m_teachType == MSG_TEACH_NO_STUDENT_TYPE)
			&& m_level >= TEACH_LEVEL_SUCCESS)
	{
		m_teachType = MSG_TEACH_NO_TYPE;
	}

	// 레벨업으로 시작하는 퀘스트 검색
	int i;
	int count = 0;
	for (i=0; i < gserver->m_questProtoList.m_nCount; i++)
	{
		if (gserver->m_questProtoList.m_proto[i].m_startType == QSTART_LEVEL)
		{
			// 필요 레벨 조건
			if (gserver->m_questProtoList.m_proto[i].m_needMinLevel > m_level || gserver->m_questProtoList.m_proto[i].m_needMaxLevel < m_level)
				continue;

			// 필요 직업 조건
			if (gserver->m_questProtoList.m_proto[i].m_needJob != -1 && gserver->m_questProtoList.m_proto[i].m_needJob != m_job)
				continue;

			// 하드코딩 : 퍼스널 던전 퀘스트 , 튜토리얼 퀘스트 제외
			if (gserver->m_questProtoList.m_proto[i].m_index == 42 || gserver->m_questProtoList.m_proto[i].m_index == 45)
				continue;

			if( m_questList.FindQuest( gserver->m_questProtoList.m_proto[i].m_index, QUEST_STATE_ALL) )
				continue;

			count++;
		}
	}

	if (count > 0)
	{
		int* qIndex;
		qIndex = new int [count];
		int idx = 0;
		for (i=0; i < gserver->m_questProtoList.m_nCount; i++)
		{
			if (gserver->m_questProtoList.m_proto[i].m_startType == QSTART_LEVEL)
			{
				// 필요 레벨 조건
				if (gserver->m_questProtoList.m_proto[i].m_needMinLevel > m_level || gserver->m_questProtoList.m_proto[i].m_needMaxLevel < m_level)
					continue;

				// 필요 직업 조건
				if (gserver->m_questProtoList.m_proto[i].m_needJob != -1 && gserver->m_questProtoList.m_proto[i].m_needJob != m_job)
					continue;

				// 하드코딩 : 퍼스널 던전 퀘스트 , 튜토리얼 퀘스트 제외
				if (gserver->m_questProtoList.m_proto[i].m_index == 42 || gserver->m_questProtoList.m_proto[i].m_index == 45)
					continue;

				// 가지고 있거나 포기했거나 완료 했을때는 보내지 않는다.
				if( m_questList.FindQuest( gserver->m_questProtoList.m_proto[i].m_index, QUEST_STATE_ALL) )
					continue;

				qIndex[idx] = gserver->m_questProtoList.m_proto[i].m_index;
				idx++;
			}
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			QuestCreateListMsg(rmsg, count, qIndex);
			SEND_Q(rmsg, m_desc);
		}
	}

	// 레벨이 4이상 나는 퀘스트는 포기시킨다.
	CQuest* pQuest;
	CQuest* pQuestNext = m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
	while ((pQuest = pQuestNext))
	{
		pQuestNext = m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		if(!pQuest->IsCompleted() && pQuest->GetQuestProto()->m_needMaxLevel + 4 < m_level)
		{
			do_QuestGiveUp(this, pQuest);
		}
	}

	// 싱글던전입장권 제공
	if (m_level >= S2_TICKET_LEVEL_START && m_level <= S2_TICKET_LEVEL_END)
	{
		CItem* item = gserver->m_itemProtoList.CreateItem(gserver->m_itemProtoList.m_sTicketItem2->getItemIndex(), -1, 0 ,0, 1);

		if (!item)
			return;

		if (this->m_inventory.addItem(item) == false)
		{
			// 인젠토리 꽉차서 못 받을 때 Drop
			item = m_pArea->DropItem(item, this);
			if (!item)
				return;

			CNetMsg::SP rmsg(new CNetMsg);
			item->m_preferenceIndex = m_index;
			ItemDropMsg(rmsg, this, item);
			m_pArea->SendToCell(rmsg, this, true);
		}

		// Item LOG
		GAMELOG << init("ITEM_PICK_LEVELUP", this)
				<< itemlog(item)
				<< end;
	}

#ifdef HANARO_EVENT
#if defined(LC_BILA)// 멕시코 하나로 이벤트
	{} // 멕시코는 목표 레벨 달성 시 아이템을 지급하지 않는다.
#else
	if (m_level == 31 || m_level == 60)
	{
		int* pItemIndex = NULL;
		int  nItemIndex = 0;
		int  nItemCount = 7;
		int  nItemPlus = 0;

		static int nTitanItem[2][7] =
		{
			{321, 313, 315, 317, 319, 608, 310},			// 31 레벨 달성
			{1349, 1350, 1351, 1352, 1353, 1333, 1334}		// 60 레벨 달성
		};

		static int nNightItem[2][8] =
		{
			{336, 328, 330, 332, 334, 617, 325, 393},
			{1343, 1344, 1345, 1346, 1347, 1348, 1332, 1331}
		};

		static int nHealerItem[2][7] =
		{
			{355, 347, 349, 351, 353, 626, 344},
			{1354, 1355, 1356, 1357, 1358, 1336, 1335}
		};

		static int nMageItem[2][7] =
		{
			{390, 370, 375, 380, 385, 635, 364},
			{1359, 1360, 1361, 1362, 1363, 1337, 1338}
		};

		static int nRogueItem[2][7] =
		{
			{569, 571, 573, 575, 577, 538, 643},
			{1364, 1365, 1366, 1367, 1368, 1339, 1340}
		};

		static int nSorcererItem[2][7] =
		{
			{1046, 1006, 1016, 1026, 1036, 996, 984},
			{1374, 1375, 1376, 1377, 1378, 1341, 1342}
		};

		if (m_level == 31)
		{
			nItemPlus = 10;
			nItemIndex = 0;
		}
		else if (m_level == 60)
		{
			nItemPlus = 5;
			nItemIndex = 1;
		}

		switch (m_job)
		{
		case JOB_TITAN:
			pItemIndex = nTitanItem[nItemIndex];
			break;
		case JOB_KNIGHT:
			pItemIndex = nNightItem[nItemIndex];
			nItemCount = 8;
			break;
		case JOB_HEALER:
			pItemIndex = nHealerItem[nItemIndex];
			break;
		case JOB_MAGE:
#ifdef EX_MAGE
		case JOB_EX_MAGE:
#endif // EX_MAGE
			pItemIndex = nMageItem[nItemIndex];
			break;
		case JOB_ROGUE:
#ifdef EX_ROGUE
		case JOB_EX_ROGUE:
#endif // EX_ROGUE
			pItemIndex = nRogueItem[nItemIndex];
			break;
		case JOB_SORCERER:
			pItemIndex = nSorcererItem[nItemIndex];
			break;
		default:
			nItemCount = 0;
		}

		int i = 0;
		for (i = 0; i < nItemCount; i++)
		{
			// 아이템 지급
			if (!GiveItem(pItemIndex[i], nItemPlus, 0, 1, false, false))
			{
				GAMELOG << init("HANARO_EVENT ERROR", this) << "ITEM" << delim << pItemIndex[i] << end;

				// 롤백
				for( ; i > 0; i--)
				{
					CItem* item = this->m_inventory.FindByDBIndex(pItemIndex[i], 0, 0);
					if (item)
					{
						this->m_inventory.decreaseItemCount(item, 1);
					}
				}// 롤백 end
				break;
			}

			GAMELOG << init("HANARO_EVENT", this) << "ITEM" << delim << pItemIndex[i] << end;
		} // for
	}
#endif // #if defined(LC_BILA)// 멕시코 하나로 이벤트
#endif // #ifdef HANARO_EVENT

	if( this->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY) )
	{
		this->GetSummonNpc(SUMMON_NPC_TYPE_MERCENARY)->ResetStat();
	}
}

void CPC::RemoveItemFromQuickSlot(CItem* item)
{
	if(item == NULL)
		return;

	CItem* quickSlotItem = NULL;
	int quickSlotType = QUICKSLOT_TYPE_EMPTY;

	//아이템을 장착하였을때는 퀵슬롯에서 제거하지 않고 퀵슬롯 정보를 수정한다.
	if(item->getWearPos() != WEARING_NONE)
	{
		quickSlotItem = item;
		quickSlotType = QUICKSLOT_TYPE_ITEM_WEAR;
	}

	for (int i=0; i < QUICKSLOT_PAGE_NUM; i++)
	{
		for (int j=0; j < QUICKSLOT_MAXSLOT; j++)
		{
			if (item == m_quickSlot[i].m_item[j])
			{
				m_quickSlot[i].m_item[j] = quickSlotItem;
				m_quickSlot[i].m_slotType[j] = quickSlotType;
				CNetMsg::SP rmsg(new CNetMsg);
				QuickSlotAddMsg(rmsg, this, (char)i, (char)j);
				SEND_Q(rmsg, m_desc);
			}
		}
	}
}

void CPC::RemoveSkillFromQuickSlot(CSkill* pSkill)
{
	int i, j;
	for (i=0; i < QUICKSLOT_PAGE_NUM; i++)
	{
		for (j=0; j < QUICKSLOT_MAXSLOT; j++)
		{
			if (pSkill->m_proto->m_index == m_quickSlot[i].m_skillType[j])
			{
				m_quickSlot[i].m_skillType[j] = -1;
				m_quickSlot[i].m_slotType[j] = QUICKSLOT_TYPE_EMPTY;

				CNetMsg::SP rmsg(new CNetMsg);
				QuickSlotAddMsg(rmsg, this, (char)i, (char)j);
				SEND_Q(rmsg, m_desc);
			}
		}
	}
}

void CPC::RecoverPC()
{
	if (DEAD(this))
		return ;

	if (m_hp == m_maxHP && m_mp == m_maxMP)
		return ;

	int rhp = m_recoverHP;
	int rmp = m_recoverMP;

	if (IsSetPlayerState(PLAYER_STATE_MOVING) || IsCombatMode() || m_currentSkill != NULL)
	{
		// 이동/공격
		rhp = 1;
		rmp = 1;
	}
	else if (!IsSetPlayerState(PLAYER_STATE_SITDOWN) && !(m_assist.m_state & AST_FAKEDEATH))
	{
		// 앉기 외에
		rhp = rhp * 25 / 100;
		rmp = rmp * 25 / 100;
	}
	rhp += m_avPassiveAddition.recoverhp;
	rhp += rhp * m_avPassiveRate.recoverhp / SKILL_RATE_UNIT;
	rmp += m_avPassiveAddition.recovermp;
	rmp += rmp * m_avPassiveRate.recovermp / SKILL_RATE_UNIT;
	rhp += m_assist.m_avAddition.recoverhp;
	rhp += rhp * m_assist.m_avRate.recoverhp / SKILL_RATE_UNIT;
	rmp += m_assist.m_avAddition.recovermp;
	rmp += rmp * m_assist.m_avRate.recovermp / SKILL_RATE_UNIT;

	if (m_opRecoverHP > 0)
		rhp += rhp * m_opRecoverHP / 100;
	if (m_opRecoverMP > 0)
		rmp += rmp * m_opRecoverMP / 100;
	if(m_opRecoverHP_NoRate > 0)
		rhp += m_opRecoverHP_NoRate;
	if(m_opRecoverMP_NoRate > 0)
		rmp += m_opRecoverMP_NoRate;

	// 쥬노, 피스존이면 회복량 10배
	if(m_pZone && m_pZone->m_index == ZONE_START)
	{
		if(GetMapAttr() & MATT_PEACE)
		{
			rhp *= 10;
			rmp *= 10;
		}
	}

	m_hp += rhp;
	m_mp += rmp;

	if (m_hp > m_maxHP || m_hp <= 0)
		m_hp = m_maxHP;
	if (m_mp > m_maxMP || m_mp <= 0)
		m_mp = m_maxMP;

	m_bChangeStatus = true;

	// 소환수 회복
	CElemental* pElemental = m_elementalList;
	while (pElemental)
	{
		if (pElemental->m_hp < pElemental->m_maxHP)
		{
			pElemental->m_hp += 1;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				CharStatusMsg(rmsg, pElemental, 0);
				pElemental->m_pArea->SendToCell(rmsg, pElemental, true);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExElementalStatusMsg(rmsg, pElemental);
				SEND_Q(rmsg, m_desc);
			}
		}
		pElemental = pElemental->m_nextElemental;
	}
}

void CPC::GoNpcPortalScroll()
{
	if(m_Npc_Portal_x<0  || m_Npc_Portal_z<0 )
		return;

	int zone = m_pArea->m_zone->m_index;
	//char ylayer = m_pos.m_yLayer;

	GoZone(this, zone, m_Npc_Portal_y, m_Npc_Portal_x, m_Npc_Portal_z);
}


void CPC::GoMemPos(int slot)
{
	if (m_mempos.m_data[slot] == NULL)
		return ;

	int zone = m_mempos.m_data[slot]->m_zone;
	char ylayer = m_mempos.m_data[slot]->m_ylayer;
	float x = m_mempos.m_data[slot]->m_x;
	float z = m_mempos.m_data[slot]->m_z;

	GoZone(this, zone, ylayer, x, z);
}

void CPC::CalcStatus(bool bSend)
{
	// 원상태로
	m_maxHP = m_dbHP;
	m_maxMP = m_dbMP;
	m_str = m_dbStr;
	m_dex = m_dbDex;
	m_int = m_dbInt;
	m_con = m_dbCon;
	//m_runSpeed = run_speed[(int)m_job];
	SetRunSpeed(run_speed[(int)m_job]);
	m_walkSpeed = walk_speed[(int)m_job];
	m_flySpeed = 15.0f;

	m_attackSpeed = 12;
	m_magicSpeed = 0;
	m_skillSpeed = 0;
	m_statPall = 0;
	// << 071226 kjban add
	m_statPall_per = 0;
	// >>
	m_skillCooltime = 0; // 0 ~ 100%

	CAPet *aPet = GetAPet(0);
	m_absorbPVPDamageRate = 0;
	m_decreaseDBufTimeRate = 0;
	if(aPet)
	{
		aPet->m_nJewelAttack = 0;
		aPet->m_nJewelDefence = 0;
		aPet->m_nJewelMagicAttack = 0;
		aPet->m_nJewelMagicDefence = 0;
	}

	m_opJewelElementPetHPUP = 0;
	m_opJewelElementAttUP = 0;
	m_opJewelElementMagicAttUP = 0;

	switch (m_job)
	{
	case JOB_TITAN:
		m_attackRange = ATTACK_RANGE_TITAN;
		m_recoverHP = ((80 * 3 / 2) + m_level) / 10;
		m_recoverMP = ((120 * 1) + m_level) / 10;
		break;
	case JOB_KNIGHT:
		m_attackRange = ATTACK_RANGE_KNIGHT;
		m_recoverHP = ((80 * 3 / 2) + m_level) / 10;
		m_recoverMP = ((120 * 1) + m_level) / 10;
		break;
	case JOB_HEALER:
		m_attackRange = ATTACK_RANGE_HEALER;
		m_recoverHP = ((80 * 3 / 2) + m_level) / 10;
		m_recoverMP = ((120 * 3 / 2) + m_level) / 10;
		break;
	case JOB_MAGE:
#ifdef EX_MAGE
	case JOB_EX_MAGE:
#endif // EX_MAGE
		m_attackRange = ATTACK_RANGE_MAGE;
		m_recoverHP = ((80 * 1) + m_level) / 10;
		m_recoverMP = ((120 * 2) + m_level) / 10;
		break;
	case JOB_ROGUE:
#ifdef EX_ROGUE
	case JOB_EX_ROGUE:
#endif // EX_ROGUE
		if (m_wearInventory.wearItemInfo[WEARING_WEAPON] && m_wearInventory.wearItemInfo[WEARING_WEAPON]->IsRangeWeapon())
			m_attackRange = ATTACK_RANGE_ROGUE_2;
		else
			m_attackRange = ATTACK_RANGE_ROGUE_1;
		m_recoverHP = ((80 * 3 / 2) + m_level) / 10;
		m_recoverMP = ((120 * 1) + m_level) / 10;
		break;
	case JOB_SORCERER:
		m_attackRange = ATTACK_RANGE_SORCERER;
		switch (m_evocationIndex)
		{
		case EVOCATION_HELLOUND:
			m_attackRange = ATTACK_RANGE_SORCERER_HELLOUND;
			break;
		case EVOCATION_ELENEN:
			m_attackRange = ATTACK_RANGE_SORCERER_ELENEN;
			break;
		}
		m_recoverHP = ((80 * 1) + m_level) / 10;
		m_recoverMP = ((120 * 1) + m_level) / 10;
		break;
	case JOB_NIGHTSHADOW :
		{
			m_attackRange = ATTACK_RANGE_NIGHTSHADOW ;
			m_recoverHP = ((80 * 1) + m_level) / 10;
			m_recoverMP = ((120 * 2) + m_level) / 10;
		}
		break;
	}

	m_artiStr = 0;
	m_artiDex = 0;
	m_artiInt = 0;
	m_artiCon = 0;
	m_artiExp = 0;
	m_artiGold = 0;

	m_opStr = 0;
	m_opDex = 0;
	m_opInt = 0;
	m_opCon = 0;

	m_opMelee = 0;
	m_opRange = 0;
	m_opMeleeHitRate = 0;
	m_opRangeHitRate =0;

	m_opDMelee = 0;
	m_opDRange = 0;
	m_opMeleeAvoid = 0;
	m_opRangeAvoid =0;

	m_opMagic = 0;
	m_opMagicHitRate = 0;
	m_opResist = 0;
	m_opResistAvoid = 0;

	m_opExpRate = 0;
	m_opSPRate = 0;

	m_opSturnLevel = 0;
	m_opBloodLevel = 0;
	m_opPoisonLevel = 0;
	m_opSlowLevel = 0;
	m_opMoveLevel = 0;

	m_opSturnIndex = 0;
	m_opBloodIndex = 0;
	m_opPoisonIndex = 0;
	m_opSlowIndex = 0;
	m_opMoveIndex = 0;

	m_opIncreaseInven = 0;
	m_opMPSteal = 0;
	m_opHPSteal = 0;
	m_opAttackBlind = 0;
	m_opAttackPoison = 0;
	m_opAttackCritical = 0;
	m_opAttackDeadly = 0;
	m_opRecoverHP = 0;
	m_opRecoverMP = 0;
	m_opRecoverHP_NoRate = 0;
	m_opRecoverMP_NoRate = 0;
	m_opDecreaseSkillDelay = 0;
	m_opDecreaseSkillMP = 0;
	m_opResistStone = 0;
	m_opResistSturn = 0;
	m_opResistSilent = 0;
	m_opBlocking = 0;
	m_opStrong = 0;

	m_plusStrong = 0;
	m_plusHard = 0;
	m_plusWeak = 0;

	// 아이템 적용
	ApplyItemValue(bSend);

	// 패시브 스킬 적용
	ApplyPassive();

	ApplyTitle();

	ArtifactManager::instance()->ApplyArtifact(this);

	// runspeed
	m_runSpeed_org = GetRunSpeed();
	// attackspeed
	m_attackSpeed_org = m_attackSpeed;
	// hp, mp
	m_maxHP_org = m_maxHP;
	m_maxMP_org = m_maxMP;

	m_assist.Apply();

	if(m_statPall)
	{
		if(m_job2 == 0)
		{
			m_maxHP += m_statPall * levelup_hp[(int)m_job][0]; //힘
			m_maxMP += m_statPall * levelup_mp[(int)m_job][2]; //지혜
			m_maxHP += m_statPall * levelup_hp[(int)m_job][3]; //체질
			m_maxMP += m_statPall * levelup_mp[(int)m_job][3]; //체질
		}
		else
		{
			m_maxHP += m_statPall * levelup_hp2[ (int)m_job * JOB_2ND_SPLIT_INDEX + (int)m_job2 -1 ]; //체질
			m_maxMP += m_statPall * levelup_mp2[ (int)m_job * JOB_2ND_SPLIT_INDEX + (int)m_job2 -1 ]; //체질
		}

		if(m_maxHP < 1)
			m_maxHP = 1;
		if(m_maxMP < 1)
			m_maxMP = 1;

		m_opStr += m_statPall;
		m_opDex += m_statPall;
		m_opInt += m_statPall;
		m_opCon += m_statPall;
	}
	// << 071226 kjban add
	if(m_statPall_per)
	{
		if(m_job2 == 0)
		{
			m_maxHP += levelup_hp[(int)m_job][0] * m_str*m_statPall_per/100; //힘
			m_maxMP += levelup_mp[(int)m_job][2] * m_int*m_statPall_per/100; //지혜
			m_maxHP += levelup_hp[(int)m_job][3] * m_con*m_statPall_per/100; //체질
			m_maxMP += levelup_mp[(int)m_job][3] * m_con*m_statPall_per/100; //체질
		}
		else
		{
			m_maxHP += levelup_hp2[ (int)m_job* JOB_2ND_SPLIT_INDEX + (int)m_job2 -1 ] * m_con*m_statPall_per/100; //체질
			m_maxMP += levelup_mp2[ (int)m_job* JOB_2ND_SPLIT_INDEX + (int)m_job2 -1 ] * m_con*m_statPall_per/100; //체질
		}

		m_opStr += m_str*m_statPall_per/100;
		m_opDex += m_dex*m_statPall_per/100;
		m_opInt += m_int*m_statPall_per/100;
		m_opCon += m_con*m_statPall_per/100;
	}
	// 강신 적용 : HP
	switch (m_evocationIndex)
	{
	case EVOCATION_HELLOUND:
		m_maxHP = m_maxHP + 2 * m_con;
		break;
	case EVOCATION_ELENEN:
		m_maxHP = m_maxHP + m_con * 15 / 10;
		break;
	}
	// 위의 똑같은 구문을 주석 처리하고 아래에 넣었습니다. 그 이유는 CAssistList::DecreaseTime(CCharacter* ch, LONGLONG* changestate) 때문입니다. m_hp, m_mp가 max보다 클때 스킬을 취소하는 문제가 있습니다.
	// CPC의 recovery pulse가 DecreaseTime보다 먼저 실행 될 경우 문제가 없으나 그렇지 않을 경우 문제가 생깁니다.
	if(m_maxHP < 1)
		m_maxHP = 1;
	if(m_maxMP < 1)
		m_maxMP = 1;

	if(m_hp > m_maxHP)
		m_hp = m_maxHP;
	if(m_mp > m_maxMP)
		m_mp = m_maxMP;
	// 페시브 스킬 스텟 계산
// 	m_opStr = ( m_opStr + m_avPassiveAddition.stat_str ) + ( m_statpt_str * m_avPassiveRate.stat_str / 100 );
// 	m_opDex = ( m_opStr + m_avPassiveAddition.stat_dex ) + ( m_statpt_dex * m_avPassiveRate.stat_dex / 100 );
// 	m_opInt = ( m_opStr + m_avPassiveAddition.stat_int ) + ( m_statpt_int * m_avPassiveRate.stat_int / 100 );
//	m_opCon = ( m_opStr + m_avPassiveAddition.stat_con ) + ( m_statpt_con * m_avPassiveRate.stat_con / 100 );

	// 맨손은 원래 속도로
	if (m_wearInventory.wearItemInfo[WEARING_WEAPON] == NULL)
	{
		m_attackSpeed = 12;
	}

	if ( m_wearInventory.wearItemInfo[WEARING_PET] )
	{
		CAPet* apet = this->GetAPet();

		if( apet && apet->IsMount() )
		{
			//m_runSpeed = apet->m_runSpeed;
			SetRunSpeed(apet->GetRunSpeed());
		}
	}

	// TODO : 하드코딩
	if (m_admin > 0 && m_speedup != 0)
	{
		//m_runSpeed = m_speedup / 10.0f;
		SetRunSpeed( m_speedup / 10.0f );
	}
	// --- TODO : 하드코딩

	m_bChangeStatus = true;

	if (GetMapAttr() & MATT_FREEPKZONE && m_level > PKMODE_LIMIT_LEVEL && !IsSetPlayerState(PLAYER_STATE_PKMODEDELAY | PLAYER_STATE_PKMODE))
	{
		ResetPlayerState(PLAYER_STATE_PKMODEDELAY | PLAYER_STATE_PKMODE);
		SetPlayerState(PLAYER_STATE_PKMODE);

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeAction(rmsg, this, ACTION_GENERAL, AGT_PKMODE);
		m_pArea->SendToCell(rmsg, this, true);
	}
}

bool CPC::UseStatPoint(MSG_STATPOINT_USE_TYPE type, int* val)
{
	// 남은 스탯 포인트 검사
	if (m_statpt_remain < 1)
		return false;

	// 스탯 포인트 증가
	int j;
	switch (type)
	{
	case MSG_STATPOINT_USE_STR:
		j = 0;
		m_statpt_str++;
		m_dbStr++;
		*val = 1;
		break;
	case MSG_STATPOINT_USE_DEX:
		j = 1;
		m_statpt_dex++;
		m_dbDex++;
		*val = 1;
		break;
	case MSG_STATPOINT_USE_INT:
		j = 2;
		m_statpt_int++;
		m_dbInt++;
		*val = 1;
		break;
	case MSG_STATPOINT_USE_CON:
		j = 3;
		m_statpt_con++;
		m_dbCon++;
		*val = 1;
		break;
	default:
		return false;
	}

	m_statpt_remain--;

	InitStat();

	CalcStatus(true);

	return true;
}

void CPC::ApplyItemValue(bool bSend)
{
	// TODO : 플러스 수치/옵션 고려
	CItem* item;
	int i, j;

	// 무기
	item = m_wearInventory.getCalcWear(WEARING_WEAPON);
	m_eqMelee = m_eqRange = m_eqMagic = 0;

	if(m_opAttratt)
		m_opAttratt = 0;

	if (item)
	{
		if (item->m_itemProto->getItemFlag() & ITEM_FLAG_LORD)
		{
			m_eqMelee = m_eqRange = m_eqMagic = 0;

			switch (item->m_itemProto->getItemSubTypeIdx())
			{
#if defined(LC_BILA)
			case IWEAPON_BIGSWORD:
				m_eqMelee = (int)(20 + ((m_level - 1 + 5) * 3) * pow(1.06, 12));
				break;

			case IWEAPON_AXE:
				m_eqMelee = (int)(23 + ((m_level - 1 + 5) * 3.3) * pow(1.06, 12));
				break;

			case IWEAPON_TWOSWORD:
				m_eqMelee = (int)(19 + ((m_level - 1 + 5) * 2.8) * pow(1.06, 12));
				break;

			case IWEAPON_NIGHT:
				m_eqMelee = (int)(16 + ((m_level - 1 + 5) * 2.4) * pow(1.06, 12));
				break;

			case IWEAPON_BOW:
				m_eqRange = (int)(14 + ((m_level - 1 + 5) * 2.5) * pow(1.06, 12));
				break;

			case IWEAPON_WAND:
				m_eqMagic = (int)(10 + ((m_level - 1 + 5) * 1.9) * pow(1.06, 12));
				break;

			case IWEAPON_STAFF:
				m_eqMagic = (int)(10 + ((m_level - 1 + 5) * 3.5) * pow(1.06, 12));
				break;

			case IWEAPON_SHORTSTAFF:
				m_eqMagic = (int)(10 + ((m_level - 1 + 5) * 3) * pow(1.06, 12));
				break;

			case IWEAPON_SHORTGUM:
				m_eqMelee = (int)(10 + ((m_level - 1 + 5) * 2.9) * pow(1.06, 12));
				break;

			case IWEAPON_CROSSBOW:
				m_eqRange = (int)(10 + ((m_level - 1 + 5) * 2.5) * pow(1.06, 12));
				break;

			case IWEAPON_SCYTHE:
				m_eqMagic = (int)(18 + ((m_level - 1 + 5) * 1.5) * pow(1.06, 12));
				break;

			case IWEAPON_POLEARM:
				m_eqMagic = (int)(20 + ((m_level - 1 + 5) * 2.1) * pow(1.06, 12));
				break;
			case IWEAPON_SOUL:
				m_eqMagic = (int)(20 + ((m_level - 1 + 5) * 2.1) * pow(1.06, 12));
				break;
#else
			case IWEAPON_BIGSWORD:
				m_eqMelee = (int)(20 + ((m_level - 1) * 3) * pow(1.06, 12));
				break;

			case IWEAPON_AXE:
				m_eqMelee = (int)(23 + ((m_level - 1) * 3.3) * pow(1.06, 12));
				break;

			case IWEAPON_TWOSWORD:
				m_eqMelee = (int)(19 + ((m_level - 1) * 2.8) * pow(1.06, 12));
				break;

			case IWEAPON_NIGHT:
				m_eqMelee = (int)(16 + ((m_level - 1) * 2.4) * pow(1.06, 12));
				break;

			case IWEAPON_BOW:
				m_eqRange = (int)(14 + ((m_level - 1) * 2.5) * pow(1.06, 12));
				break;

			case IWEAPON_WAND:
				m_eqMagic = (int)(10 + ((m_level - 1) * 1.9) * pow(1.06, 12));
				break;

			case IWEAPON_STAFF:
				m_eqMagic = (int)(10 + ((m_level - 1) * 3.5) * pow(1.06, 12));
				break;

			case IWEAPON_SHORTSTAFF:
				m_eqMagic = (int)(10 + ((m_level - 1) * 3) * pow(1.06, 12));
				break;

			case IWEAPON_SHORTGUM:
				m_eqMelee = (int)(10 + ((m_level - 1) * 2.9) * pow(1.06, 12));
				break;

			case IWEAPON_CROSSBOW:
				m_eqRange = (int)(10 + ((m_level - 1) * 2.5) * pow(1.06, 12));
				break;

			case IWEAPON_SCYTHE:
				m_eqMagic = (int)(18 + ((m_level - 1) * 1.5) * pow(1.06, 12));
				break;

			case IWEAPON_POLEARM:
				m_eqMagic = (int)(20 + ((m_level - 1) * 2.1) * pow(1.06, 12));
				break;

			case IWEAPON_SOUL:
				m_eqMagic = (int)(20 + ((m_level - 1) * 2.1) * pow(1.06, 12));
				break;
#endif // LC_BRZ
			}
			m_attackSpeed = item->m_itemProto->getItemNum2();
		}

		else
		{
			bool bNormalItem = true;
			CItem*	pItemNormal = item;
			int		nNormalPlus = item->getPlus();

			static const int nPetUniqueItemCount = 12;
			static const int nPetUniqueItemIndex[nPetUniqueItemCount] =
			{
				887,	// 한손검
				907,	// 이도류
				908,	// 대검
				909,	// 도끼
				910,	// 힐러완드
				911,	// 활
				912,	// 숏스테프
				913,	// 스테프
				914,	// 단검
				915,	// 석궁
				916,	// 사이드
				917		// 폴암
			};
			for (i = 0; i < nPetUniqueItemCount; i++)
			{
				if (item->getDBIndex() == nPetUniqueItemIndex[i])
					break;
			}
			if (i != nPetUniqueItemCount)
			{
				bNormalItem = false;
				m_eqMelee = m_eqRange = m_eqMagic = 0;
				m_attackSpeed = item->m_itemProto->getItemNum2();
				switch (item->m_itemProto->getItemSubTypeIdx())
				{
				case IWEAPON_BIGSWORD:
					m_eqMelee = (int)(20 + ((m_level - 1) * 3) * pow(1.06, 8));
					break;

				case IWEAPON_AXE:
					m_eqMelee = (int)(23 + ((m_level - 1) * 3.3) * pow(1.06, 8));
					break;

				case IWEAPON_TWOSWORD:
					m_eqMelee = (int)(19 + ((m_level - 1) * 2.8) * pow(1.06, 8));
					break;

				case IWEAPON_NIGHT:
					m_eqMelee = (int)(16 + ((m_level - 1) * 2.4) * pow(1.06, 8));
					break;

				case IWEAPON_BOW:
					m_eqRange = (int)(14 + ((m_level - 1) * 2.5) * pow(1.06, 8));
					break;

				case IWEAPON_WAND:
					m_eqMagic = (int)(10 + ((m_level - 1) * 1.9) * pow(1.06, 8));
					break;

				case IWEAPON_SHORTSTAFF:
					m_eqMagic = (int)(10 + ((m_level - 1) * 3) * pow(1.06, 8));
					break;

				case IWEAPON_CROSSBOW:
					m_eqRange = (int)(10 + ((m_level - 1) * 2.5) * pow(1.06, 8));
					break;

				case IWEAPON_SHORTGUM:
					m_eqMelee = (int)(10 + ((m_level - 1) * 2.9) * pow(1.06, 8));
					break;

#if defined ( LC_KOR ) || defined ( LC_TLD ) || defined (LC_GAMIGO) || defined (LC_RUS) || defined(LC_BILA)
				case IWEAPON_STAFF:
					m_eqMagic = (int)(14 + ((m_level - 1) * 3.5) * pow(1.06, 8));
					break;

				case IWEAPON_SCYTHE:
					m_eqMagic = (int)(20 + ((m_level - 1) * 2.9) * pow(1.06, 8));
					break;

				case IWEAPON_POLEARM:
					m_eqMagic = (int)(18 + ((m_level - 1) * 2.2) * pow(1.06, 8));
					break;
#else
				case IWEAPON_STAFF:
					m_eqMagic = (int)(10 + ((m_level - 1) * 3.5) * pow(1.06, 8));
					break;

				case IWEAPON_SCYTHE:
					m_eqMagic = (int)(18 + ((m_level - 1) * 1.5) * pow(1.06, 8));
					break;

				case IWEAPON_POLEARM:
					m_eqMagic = (int)(20 + ((m_level - 1) * 2.1) * pow(1.06, 8));
					break;
#endif // LC_KOR
				default:
					break;
				}
			}

			// 결합 아이템
			if (bNormalItem && (item->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (item->getFlag() & FLAG_ITEM_COMPOSITION))
			{
				CItem* pItemEquip = m_inventory.FindByVirtualIndex(item->m_nCompositeItem);
				if (pItemEquip)
				{
					pItemNormal = pItemEquip;
				}
				else
				{
					pItemNormal = m_wearInventory.FindByVirtualIndex(item->m_nCompositeItem);
				}
				if (nNormalPlus > MAX_UPGRADE_PLUS+1)
					nNormalPlus = MAX_UPGRADE_PLUS+1;
			}

			if (bNormalItem && pItemNormal)
			{
				if (pItemNormal->IsRangeWeapon())	// 활이면 원거리
				{
					int rareoption_damage = 0;
					if(pItemNormal->m_pRareOptionProto)
						rareoption_damage = pItemNormal->m_pRareOptionProto->GetDamageUp();
						
					m_eqRange = ItemUpgradeFuckingFunction( nNormalPlus, pItemNormal->GetItemLevel(), pItemNormal->m_itemProto->getItemNum0() + pItemNormal->getPlus_2() + rareoption_damage );
				}
				else	// 그외 무기는 모두 근접
				{
					int rareoption_damage = 0;
					if(pItemNormal->m_pRareOptionProto)
						rareoption_damage = pItemNormal->m_pRareOptionProto->GetDamageUp();

					m_eqMelee = ItemUpgradeFuckingFunction( nNormalPlus, pItemNormal->GetItemLevel(), pItemNormal->m_itemProto->getItemNum0() + pItemNormal->getPlus_2() + rareoption_damage);
				}
				m_eqMagic = ItemUpgradeFuckingFunction( nNormalPlus, pItemNormal->GetItemLevel(), pItemNormal->m_itemProto->getItemNum1() );
				m_attackSpeed = pItemNormal->m_itemProto->getItemNum2();

				if ( pItemNormal->IsRareItem() )
				{
					if (pItemNormal->m_pRareOptionProto)
					{
						for (i = 0; i < MAX_RARE_OPTION_SETTING; i++)
						{
							const RAREOPTIONDATA* rod = pItemNormal->m_pRareOptionProto->GetData(i);
							if (pItemNormal->m_nRareOptionBit & rod->rarebit)
							{
								COption::ApplyOptionValue(this, rod->optiontype, rod->optionval, pItemNormal);
							}
						}
						// 스탯 상승 적용
						COption::ApplyOptionValue(this, OPTION_DAMAGE_UP,  pItemNormal->m_pRareOptionProto->GetDamageUp(), NULL);
						COption::ApplyOptionValue(this, OPTION_DEFENSE_UP, pItemNormal->m_pRareOptionProto->GetDefenseUp(), NULL);
						COption::ApplyOptionValue(this, OPTION_MAGIC_UP,   pItemNormal->m_pRareOptionProto->GetMagicUp(), NULL);
						COption::ApplyOptionValue(this, OPTION_RESIST_UP,  pItemNormal->m_pRareOptionProto->GetResistUp(), NULL);
					}
				}
				else if (pItemNormal->IsOriginItem())
				{
					ApplyOriginOption(pItemNormal);
					pItemNormal->m_nSetOption = 0;
				}
				else
				{
					for (i=0; i < MAX_WEAPON_OPTION; i++)
					{
						if (pItemNormal->m_option[i].m_level <= 0)
							continue;
						pItemNormal->m_option[i].ApplyOptionValue(this, pItemNormal);
					}
				}

				ApplyItemPlusValue(pItemNormal, nNormalPlus );

				//m_optionAttSkillList.Clear();
				/*if(pItemNormal->m_socketList.GetSocketCount() > 0)
				{
				for(int i=0 ; i<MAX_SOCKET_COUNT; i++)
				{
				if(pItemNormal->m_socketList.m_socketpos[i].GetSkill())
				if(m_optionAttSkillList.count() > 0)
				m_optionAttSkillList.Remove(pItemNormal->m_socketList.m_socketpos[i].GetSkill());
				}
				}	*/
				pItemNormal->m_socketList.ApplyJewelOption(this, pItemNormal);
			}
		}
	}

	item = m_wearInventory.getCalcWear(WEARING_BACKWING);
	if( item )
	{
		m_flySpeed = (float)(item->m_itemProto->getItemNum2())  / 10 ;
	}

	// 방어구
	if(m_opAttrdef)
		m_opAttrdef = 0;
	m_eqDefense = 0;
	m_eqResist = 0;
	static int armors[] = {WEARING_HELMET, WEARING_ARMOR_UP, WEARING_ARMOR_DOWN, WEARING_SHIELD, WEARING_GLOVE, WEARING_BOOTS, WEARING_BACKWING };
	for (i = 0; i < sizeof(armors) / sizeof(armors[0]); i++)
	{
		item = m_wearInventory.getCalcWear(armors[i]);
		if (item && item->IsArmorType())
		{
			bool bNormalItem = true;
			CItem*	pItemNormal = item;
			int		nNormalPlus = item->getPlus();

			static const int nPetUniqueItemCount = 31;
			// itemIndex, f1, f2 : (f1 + (f2 * level)) * pow(1.06, 6)
			static const int nPetUniqueItemIndex[nPetUniqueItemCount][3] =
			{
				{918,  10, 20},			// 헬멧,타이탄
				{919,  16, 20},			// 상의,
				{920,  12, 20},			// 하의,
				{921,  10, 20},			// 부츠,
				{922,   8, 20},			// 장갑,
				{923,  14, 15},			// 헬멧,나이트
				{924,  20, 15},			// 상의,
				{925,  16, 15},			// 하의,
				{926,  14, 15},			// 부츠,
				{927,  14, 15},			// 장갑,
				{1280, 24, 30},			// 방패,
				{928,  10, 10},			// 헬멧,메이지
				{929,  12, 15},			// 상의,
				{930,  12, 15},			// 하의,
				{931,  10, 20},			// 부츠,
				{932,  10, 20},			// 장갑,
				{933,  14, 15},			// 헬멧,로그
				{934,  20, 20},			// 상의,
				{935,  16, 20},			// 하의,
				{936,  14, 15},			// 부츠,
				{937,  14, 15},			// 장갑,
				{938,  10, 10},			// 헬멧,힐러
				{939,  12, 10},			// 상의,
				{940,  12, 10},			// 하의,
				{941,  10, 10},			// 부츠,
				{942,  10, 10},			// 장갑,
				{943,  12, 16},			// 헬멧,소서러
				{944,  14, 18},			// 상의,
				{945,  14, 18},			// 하의,
				{946,  12, 16},			// 부츠,
				{947,  12, 16},			// 장갑,
			};
			int j;
			for (j = 0; j < nPetUniqueItemCount; j++)
			{
				if (item->getDBIndex() == nPetUniqueItemIndex[j][0])
					break;
			}
			if (j != nPetUniqueItemCount)
			{
				bNormalItem = false;
				m_eqDefense += (int)((nPetUniqueItemIndex[j][1] + (nPetUniqueItemIndex[j][2] * m_level / 10)) * pow(1.06, 6));
//				m_eqResist  += (int)((nPetUniqueItemIndex[j][1] + (nPetUniqueItemIndex[j][2] * m_level / 10)) * pow(1.06, 6));
			}

			// 결합 아이템
			if (bNormalItem && (item->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (item->getFlag() & FLAG_ITEM_COMPOSITION))
			{
				CItem* pItemEquip = m_inventory.FindByVirtualIndex(item->m_nCompositeItem);
				if (pItemEquip)
				{
					pItemNormal = pItemEquip;
				}
				if (nNormalPlus > MAX_UPGRADE_PLUS+1)
					nNormalPlus = MAX_UPGRADE_PLUS+1;
			}

			if (bNormalItem)
			{
				m_eqDefense += (int)(ItemUpgradeFuckingFunction(nNormalPlus, pItemNormal->GetItemLevel(), pItemNormal->m_itemProto->getItemNum0())) ;
				m_eqResist	+= (int)(ItemUpgradeFuckingFunction(nNormalPlus, pItemNormal->GetItemLevel(), pItemNormal->m_itemProto->getItemNum1())) ;

				bool bIsRare = false;
				if (pItemNormal->IsRareItem())
					bIsRare = true;

				if (bIsRare)
				{
					if (pItemNormal->m_pRareOptionProto)
					{
						for (j = 0; j < MAX_RARE_OPTION_SETTING; j++)
						{
							const RAREOPTIONDATA* rod = pItemNormal->m_pRareOptionProto->GetData(j);
							if (pItemNormal->m_nRareOptionBit & rod->rarebit)
							{
								COption::ApplyOptionValue(this, rod->optiontype, rod->optionval, pItemNormal);
							}
						}
						// 스탯 상승 적용
						COption::ApplyOptionValue(this, OPTION_DAMAGE_UP,  pItemNormal->m_pRareOptionProto->GetDamageUp(), NULL);
						COption::ApplyOptionValue(this, OPTION_DEFENSE_UP, pItemNormal->m_pRareOptionProto->GetDefenseUp(), NULL);
						COption::ApplyOptionValue(this, OPTION_MAGIC_UP,   pItemNormal->m_pRareOptionProto->GetMagicUp(), NULL);
						COption::ApplyOptionValue(this, OPTION_RESIST_UP,  pItemNormal->m_pRareOptionProto->GetResistUp(), NULL);
					}
				}
				else if (pItemNormal->IsOriginItem())
				{
					ApplyOriginOption(pItemNormal);
					pItemNormal->m_nSetOption = 0;
				}
				else
				{
					for (j=0; j < MAX_WEAPON_OPTION; j++)
					{
						if (pItemNormal->m_option[j].m_level <= 0)
							continue;
						pItemNormal->m_option[j].ApplyOptionValue(this, pItemNormal);
					}
				}
				ApplyItemPlusValue( pItemNormal, nNormalPlus );

				//m_optionDefSkillList.Clear();
				/*if(pItemNormal->m_socketList.GetSocketCount() > 0)
				{
					for(int i=0 ; i<MAX_SOCKET_COUNT; i++)
					{
						if(pItemNormal->m_socketList.m_socketpos[i].GetSkill())
							if(m_optionDefSkillList.count() > 0)
								m_optionDefSkillList.Remove(pItemNormal->m_socketList.m_socketpos[i].GetSkill());
					}
				}*/
				pItemNormal->m_socketList.ApplyJewelOption(this, pItemNormal);
			}
		}
	}

	static int accessory_item[] = { WEARING_ACCESSORY1, WEARING_ACCESSORY2, WEARING_ACCESSORY3 };
	// 악세사리
	for (i = 0; i < sizeof(accessory_item) / sizeof(accessory_item[0]); i++)
	{
		// 캐쉬 악세서리 능력치 중복 안되게
		int cashNotDup = 0;
		item = m_wearInventory.getCalcWear(accessory_item[i]);
		if (item)
		{
			// 060227 : bs : 악서서리
			time_t timeCur;
			time(&timeCur);
			struct tm tmCur = *localtime(&timeCur);
			int nCurTime = tmCur.tm_hour * 100 + tmCur.tm_min;

			switch (item->getDBIndex())
			{
			case 1301:		// 18:00 - 02:00, 명중 20%
				if ((nCurTime >= 1800 || nCurTime < 200) && !(cashNotDup & ( 1 << 1 )) )
				{
					m_opMeleeHitRate += (int)(m_opMeleeHitRate * 0.2);
					m_opRangeHitRate += (int)(m_opRangeHitRate * 0.2);
					m_opMagicHitRate += (int)(m_opMagicHitRate * 0.2);
					cashNotDup = cashNotDup | ( 1 << 1 );
				}
				break;

			case 1302:		// 10:00 - 18:00, 명중 20%
				if ((nCurTime >= 1000 && nCurTime < 1800) && !(cashNotDup & ( 1 << 2 )) )
				{
					m_opMeleeHitRate += (int)(m_opMeleeHitRate * 0.2);
					m_opRangeHitRate += (int)(m_opRangeHitRate * 0.2);
					m_opMagicHitRate += (int)(m_opMagicHitRate * 0.2);
					cashNotDup = cashNotDup | ( 1 << 2 );
				}
				break;

			case 1303:		// 02:00 - 10:00, 명중 20%
				if ((nCurTime >= 200 && nCurTime < 1000) && !(cashNotDup & ( 1 << 3 )) )
				{
					m_opMeleeHitRate += (int)(m_opMeleeHitRate * 0.2);
					m_opRangeHitRate += (int)(m_opRangeHitRate * 0.2);
					m_opMagicHitRate += (int)(m_opMagicHitRate * 0.2);
					cashNotDup = cashNotDup | ( 1 << 3 );
				}
				break;

			case 1304:		// 18:00 - 02:00, 회피 10%
				if ((nCurTime >= 1800 || nCurTime < 200) && !(cashNotDup & ( 1 << 4 )) )
				{
					m_opMeleeAvoid += (int)(m_opMeleeAvoid * 0.1);
					m_opRangeAvoid += (int)(m_opRangeAvoid * 0.1);
					m_opResistAvoid += (int)(m_opResistAvoid * 0.1);
					cashNotDup = cashNotDup | ( 1 << 4 );
				}
				break;

			case 1305:		// 10:00 - 18:00, 회피 10%
				if ((nCurTime >= 1000 && nCurTime < 1800) && !(cashNotDup & ( 1 << 5 )) )
				{
					m_opMeleeAvoid += (int)(m_opMeleeAvoid * 0.1);
					m_opRangeAvoid += (int)(m_opRangeAvoid * 0.1);
					m_opResistAvoid += (int)(m_opResistAvoid * 0.1);
					cashNotDup = cashNotDup | ( 1 << 5 );
				}
				break;

			case 1306:		// 02:00 - 10:00, 회피 10%
				if ( (nCurTime >= 200 && nCurTime < 1000) && !(cashNotDup & ( 1 << 6 )) )
				{
					m_opMeleeAvoid += (int)(m_opMeleeAvoid * 0.1);
					m_opRangeAvoid += (int)(m_opRangeAvoid * 0.1);
					m_opResistAvoid += (int)(m_opResistAvoid * 0.1);
					cashNotDup = cashNotDup | ( 1 << 6 );
				}
				break;

#ifdef NEW_ACCERY_ADD
			case 2039:		// 건강의 반지 HP 150증가
				if( !(cashNotDup & ( 1 << 7 )) )
				{
					m_maxHP += 150;
					cashNotDup = cashNotDup | ( 1 << 7 );
				}
				break;

			case 2040:		// 마나 잼  MP 150 증가
				if( !(cashNotDup & ( 1 << 8 )) )
				{
					m_maxMP += 150;
					cashNotDup = cashNotDup | ( 1 << 8 );
				}
				break;
			case 2041:		// 경험의 목걸이 획득한 경험치 5%증가
			case 2042:		// SP 부스터
				if( !(cashNotDup & ( 1 << 9 )) )
				{
					m_AddProb = 5;
					cashNotDup = cashNotDup | ( 1 << 9 );
				}
				break;
#endif // NEW_ACCERY_ADD
			default:
				{
					bool bIsRare = false;
					if (item->IsRareItem())
						bIsRare = true;

					if (bIsRare)
					{
						if (item->m_pRareOptionProto)
						{
							for (j = 0; j < MAX_RARE_OPTION_SETTING; j++)
							{
								const RAREOPTIONDATA* rod = item->m_pRareOptionProto->GetData(j);
								if (item->m_nRareOptionBit & rod->rarebit)
								{
									COption::ApplyOptionValue(this, rod->optiontype, rod->optionval, item);
								}
							}
							// 스탯 상승 적용
							COption::ApplyOptionValue(this, OPTION_DAMAGE_UP,  item->m_pRareOptionProto->GetDamageUp(), NULL);
							COption::ApplyOptionValue(this, OPTION_DEFENSE_UP, item->m_pRareOptionProto->GetDefenseUp(), NULL);
							COption::ApplyOptionValue(this, OPTION_MAGIC_UP,   item->m_pRareOptionProto->GetMagicUp(), NULL);
							COption::ApplyOptionValue(this, OPTION_RESIST_UP,  item->m_pRareOptionProto->GetResistUp(), NULL);
						}
					}
					else if (item->IsOriginItem())
					{
						ApplyOriginOption(item);
						item->m_nSetOption = 0;
					}
					else
					{
						for (j=0; j < MAX_WEAPON_OPTION; j++)
						{
							if (item->m_option[j].m_level <= 0)
								continue ;
							if (item->m_itemProto->getItemMaxUse() != -1 && item->getUsed() <= 0)
								continue;
							item->m_option[j].ApplyOptionValue(this, item);
						}
					}
				}
				break;
			}

			item->m_socketList.ApplyJewelOption(this, item);
		}
	}

	for (i = 0; i < MAX_WEARING; i++)
	{
		CItem * pItem = m_wearInventory.wearItemInfo[i];

		if(!pItem)
			continue;

		if((pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (pItem->getFlag() & FLAG_ITEM_COMPOSITION))
		{
			pItem = this->m_inventory.FindByVirtualIndex(pItem->m_nCompositeItem);
		}
		if ( !pItem )
			continue;

		if (!pItem->m_pSetItemProto)
			continue;

		if (!pItem->m_nSetOption)
		{
			ApplySetItemValue(pItem);
		}
	}

	for(i = COSTUME2_WEARING_START; i <= COSTUME2_WEARING_BACK; i++)
	{
		CItem* item = m_wearInventory.wearItemInfo[i];
		if(item != NULL)
		{
			if(item->IsOriginItem())
				ApplyOriginOption(item);
		}
	}

	if (bSend)
		m_bChangeStatus = true;
}

void CPC::ApplyItemPlusValue(CItem* pItem, int NomalPlus )
{
	int nNormalPlus = NomalPlus;  // 결합 관련 변경

	// 146레벨 이하
	if (pItem->GetItemLevel() < ITEM_UPGRADE_GENERAL_MAXLEVEL)
	{
		// 무기
		if (pItem->IsWeaponType())
		{
			// 공격력 상승 : 소서러 추가시 마공
			if (nNormalPlus >= 6)
			{
				switch (pItem->m_itemProto->getItemSubTypeIdx())
				{
				case IWEAPON_NIGHT:
				case IWEAPON_CROSSBOW:
				case IWEAPON_BIGSWORD:
				case IWEAPON_AXE:
				case IWEAPON_BOW:
				case IWEAPON_SHORTGUM:
				case IWEAPON_TWOSWORD:
					if (pItem->IsRangeWeapon())
						m_eqRange += 6;
					else
						m_eqMelee += 6;
					break;

				case IWEAPON_WAND:
				case IWEAPON_STAFF:
				case IWEAPON_SHORTSTAFF:
				case IWEAPON_SCYTHE:
				case IWEAPON_POLEARM:
				case IWEAPON_SOUL:
					m_eqMagic += 4;
					break;
				}
			}

			// 명중 or 사정거리 증가 : 소서러 추가시 사거리
			if (nNormalPlus >=8)
			{
				switch (pItem->m_itemProto->getItemSubTypeIdx())
				{
				case IWEAPON_NIGHT:
				case IWEAPON_BIGSWORD:
				case IWEAPON_AXE:
				case IWEAPON_SHORTGUM:
				case IWEAPON_TWOSWORD:
				case IWEAPON_SCYTHE:
				case IWEAPON_POLEARM:
					// 옵션의 확률 단위는 %
					m_opMeleeHitRate += 3;
					m_opRangeHitRate += 3;
					m_opMagicHitRate += 3;
					break;

				case IWEAPON_CROSSBOW:
				case IWEAPON_STAFF:
				case IWEAPON_SHORTSTAFF:
				case IWEAPON_BOW:
				case IWEAPON_WAND:
				case IWEAPON_SOUL:
					// 기본공격만 2m 증가
					m_attackRange += 2.0f;
					break;
				}
			}

			// 강공 확률 증가
			if (nNormalPlus >= 10)
				m_plusStrong += 300;

			// 매강공 확률 증가
			if (nNormalPlus >= 12)
				m_plusHard += 400;

			// +15이상은 추가 공격력
			if (nNormalPlus >= 15)
			{
				switch (pItem->m_itemProto->getItemSubTypeIdx())
				{
				case IWEAPON_NIGHT:
				case IWEAPON_CROSSBOW:
				case IWEAPON_BIGSWORD:
				case IWEAPON_AXE:
				case IWEAPON_BOW:
				case IWEAPON_SHORTGUM:
				case IWEAPON_TWOSWORD:
					if (pItem->IsRangeWeapon())
						m_eqRange += 75;
					else
						m_eqMelee += 75;
					break;

				case IWEAPON_WAND:
				case IWEAPON_STAFF:
				case IWEAPON_SHORTSTAFF:
				case IWEAPON_SCYTHE:
				case IWEAPON_POLEARM:
				case IWEAPON_SOUL:
					m_eqMagic += 50;
					break;
				}
			}
		}
		// 방어구
		else if (pItem->IsArmorType())
		{
			if (pItem->m_itemProto->getItemSubTypeIdx() != IWEAR_SHIELD)
			{
				// 추가 방어력 10
				if (nNormalPlus >=8)
					m_eqDefense += 10;

				// 추가 마방 10
				if (nNormalPlus >=10)
					m_eqResist += 10;

				// 위크 확률 5%
				if (nNormalPlus >=12)
					m_plusWeak += 500;
			}

			if (nNormalPlus >= 15)
			{
				m_eqDefense += 100;
				m_eqResist += 50;
			}
		}
	}
	// 146레벨 이상부터는 수치별로 다르게 적용..
	else
	{
		// 무기
		if (pItem->IsWeaponType())
		{
			if(nNormalPlus >= 6)
			{
				int	nAttackPoint = 0; //, nCriHitRate = 0, nDeadHitRate = 0;

				if(nNormalPlus <= 15)
				{
					if(nNormalPlus > 5)
					{
						int val = 0;
						val = nNormalPlus - 5;
						if(val > 10)
							val = 10;

						nAttackPoint = val * 45;
					}
				}
				else if(nNormalPlus <= 19)
				{
					nAttackPoint = 450;
				}
				else if(nNormalPlus <= 24)
				{
					nAttackPoint = 550;
				}
				else if(nNormalPlus <= 30)
				{
					nAttackPoint = 700;
				}

				if( nAttackPoint > 0 )
				{
					if (pItem->IsRangeWeapon())
						m_eqRange += nAttackPoint;
					else if( pItem->IsMagicWeapon() )
						m_eqMagic += nAttackPoint;
					else
						m_eqMelee += nAttackPoint;
				}
			}
		}
		// 방어구
		else if (pItem->IsArmorType())
		{
			if(nNormalPlus >= 6)
			{
				int nDef=0;
				int nHP = 100 * (nNormalPlus - 9);

				if(nNormalPlus <= 9)
				{
					nHP = 0;
					nDef = 25 * (nNormalPlus - 5);
				}
				else if(nNormalPlus <= 14)
				{
					nDef = 25 * (nNormalPlus - 5);
				}
				else if(nNormalPlus <= 19)
				{
					nDef = 250;
				}
				else if(nNormalPlus <= 24)
				{
					nDef = 300;
				}
				else if(nNormalPlus <= 30)
				{
					nDef = 400;
				}

				if( nDef > 0 )
					m_eqDefense += nDef;

				if( nHP > 0 )
					m_maxHP += nHP;
			}
		}
	}

	if( pItem->getPlus_2() > 0 )
	{
		if ( pItem->IsArmorType())
		{
			m_eqDefense += pItem->getPlus_2();
		}
		else if (pItem->IsWeaponType())
		{
			switch (pItem->m_itemProto->getItemSubTypeIdx())
			{
			case IWEAPON_NIGHT:
			case IWEAPON_CROSSBOW:
			case IWEAPON_BIGSWORD:
			case IWEAPON_AXE:
			case IWEAPON_BOW:
			case IWEAPON_SHORTGUM:
			case IWEAPON_TWOSWORD:
				if (pItem->IsRangeWeapon())
					m_eqRange += pItem->getPlus_2() ;
				else
					m_eqMelee += pItem->getPlus_2() ;
				break;

			case IWEAPON_WAND:
			case IWEAPON_STAFF:
			case IWEAPON_SHORTSTAFF:
			case IWEAPON_SCYTHE:
			case IWEAPON_POLEARM:
			case IWEAPON_SOUL:
				m_eqMagic += pItem->getPlus_2() ;
				break;
			}
		}
	}
}

// 착용 아이템 중 무기, 방어구에 옵션을 붙일 수 있도록 선택
int CPC::SettingItemOption()
{
	int enableIndex[WEARING_BOOTS + 1];	// 옵션 붙일 수 있는 아이템 인덱스
	int	nEnable = 0;					// 옵션 붙일 수 있는 아이템 몇개인가?

	int i;
	for (i = WEARING_HELMET; i <= WEARING_BOOTS; i++)
	{
		if (m_wearInventory.wearItemInfo[i])
		{
			// 무기 : 무기 최대 옵션 가능 초과 체크
			if (m_wearInventory.wearItemInfo[i]->IsWeaponType())
			{
				if (m_wearInventory.wearItemInfo[i]->m_nOption >= MAX_WEAPON_OPTION)
					continue;
				if (m_wearInventory.wearItemInfo[i]->m_itemProto->getItemSubTypeIdx() == IWEAPON_MINING ||
						m_wearInventory.wearItemInfo[i]->m_itemProto->getItemSubTypeIdx() == IWEAPON_GATHERING ||
						m_wearInventory.wearItemInfo[i]->m_itemProto->getItemSubTypeIdx() == IWEAPON_CHARGE)
					continue;
				// 성주 전용 아이템은 제외
				if (m_wearInventory.wearItemInfo[i]->m_itemProto->getItemFlag() & ITEM_FLAG_LORD)
					continue ;
			}
			// 방어구 : 방어구 최대 옵션 가능 초과 체크
			else if (m_wearInventory.wearItemInfo[i]->IsArmorType())
			{
				if (m_wearInventory.wearItemInfo[i]->m_nOption >= MAX_ARMOR_OPTION)
					continue;
			}

			// 업그레이드 불능은 블옵 불가
			if (!m_wearInventory.wearItemInfo[i]->CanBloodGem())
				continue ;

			// 이미 옵션 붙일 수 있는 상태인것은 제외
			if (m_wearInventory.wearItemInfo[i]->getFlag() & FLAG_ITEM_OPTION_ENABLE)
				continue;

			// 무기나 방어구 일때만
			if (m_wearInventory.wearItemInfo[i]->IsWeaponType() || m_wearInventory.wearItemInfo[i]->IsArmorType())
			{
				enableIndex[nEnable] = i;
				nEnable++;
			}
		}
	}

	// 옵션 붙일 수 있는 아이템 없음
	if (nEnable == 0)
		return -1;

	int where = GetRandom(0, nEnable - 1);

	if (!m_wearInventory.wearItemInfo[enableIndex[where]])
		return -1;

	// 아이템 옵션을 붙일 수 있는 상태로 flag 값 셋팅
	m_wearInventory.wearItemInfo[enableIndex[where]]->setFlag(m_wearInventory.wearItemInfo[enableIndex[where]]->getFlag() | FLAG_ITEM_OPTION_ENABLE);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysBloodItemMsg(rmsg, m_wearInventory.wearItemInfo[enableIndex[where]]->getDBIndex());
		SEND_Q(rmsg, m_desc);
	}

	this->m_wearInventory.sendOneItemInfo(enableIndex[where]);

	return where;
}

int CPC::CanWarp()
{
	// 죽었을때
	if (DEAD(this))
		return MSG_SYS_CANT_WARP_STATE;

	// 앉아 있을때
	if (m_state & PLAYER_STATE_SITDOWN)
		return MSG_SYS_CANT_WARP_STATE;

	// 다른 이동장치 사용중
	if (m_state & PLAYER_STATE_WARP)
		return MSG_SYS_CANT_WARP_OTHER;

	// 스킬 사용중
	if (m_currentSkill)
	{
		if( m_pZone->IsPersonalDungeon() ) // 퍼스널 던전에서 이동시에만 스킬을 해제하자
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeSkillCancelMsg(rmsg, this);
			m_pArea->SendToCell(rmsg, this, true);
			m_currentSkill->Cancel(this);
			m_currentSkill = NULL;
		}
		else
			return MSG_SYS_CANT_WARP_STATE;
	}

	// 교환 중
	if (m_exchange)
		return MSG_SYS_CANT_WARP_STATE;

#if defined (LC_KOR) || defined (LC_USA) || defined (LC_GAMIGO)
	if (m_party && m_party->GetRequestIndex() > 0)
		return MSG_SYS_CANT_WARP_STATE;
#else
	// 파티 신청중
	if (m_party && m_party->GetBossIndex() == m_index && m_party->GetRequestIndex() > 0)
		return MSG_SYS_CANT_WARP_STATE;
#endif // defined (LC_KOR) || defined (LC_USA)

	return 0;
}

CSkill* CPC::FindSkill(int index)
{
	CSkill* ret;
	ret = m_activeSkillList.Find(index);
	if (ret)
		return ret;
	ret = m_passiveSkillList.Find(index);
	if (ret)
		return ret;
	ret = m_etcSkillList.Find(index);
	if (ret)
		return ret;

	ret = m_sealSkillList.Find(index);
	if (ret)
		return ret;

	return NULL;
}

void CPC::ApplyPassive()
{
	// 초기화
	memset(&m_avPassiveAddition, 0, sizeof(m_avPassiveAddition));
	memset(&m_avPassiveRate, 0, sizeof(m_avPassiveRate));

	// 패시브 리스트에서 적용
	CSkill* pSkill;
	void* pos = m_passiveSkillList.GetHeadPosition();
	while (pos)
	{
		pSkill = m_passiveSkillList.GetNext(pos);

		if (pSkill && pSkill->m_proto)
		{
			bool bApply = true;
			// 소서러 플래그가 있으면 강신 상태에 따라 적용 결정
			if (pSkill->m_proto->CheckSorcererFlag(SSF_APP_ALL))
			{
				switch (m_evocationIndex)
				{
				case EVOCATION_NONE:
					if (!pSkill->m_proto->CheckSorcererFlag(SSF_APP_CHARACTER))
						bApply = false;
					break ;
				case EVOCATION_HELLOUND:
					if (!pSkill->m_proto->CheckSorcererFlag(SSF_APP_HELLOUND))
						bApply = false;
					break;
				case EVOCATION_ELENEN:
					if (!pSkill->m_proto->CheckSorcererFlag(SSF_APP_ELENEN))
						bApply = false;
					break;
				default:
					bApply = false;
					break;
				}
			}
			if (bApply)
				ApplyPassiveSkill(pSkill, 100);
		}
	}

	//길드스킬 리스트에서 적용
	if(this->m_guildInfo != NULL)
	{
		pos = this->m_guildInfo->guild()->m_passiveSkillList.GetHeadPosition();
		while (pos)
		{
			pSkill = this->m_guildInfo->guild()->m_passiveSkillList.GetNext(pos);

			if (pSkill && pSkill->m_proto)
			{
				bool bApply = true;
				if (bApply)
					ApplyPassiveSkill(pSkill, 100);
			}
		}
	}
	
	// 펫의 패시브 스킬
	if (m_wearInventory.wearItemInfo[WEARING_PET])
	{
		CPet* pet = GetPet();
		// 050309: bw 펫이 멀리 있으면 스킬 적용 안함 20미터
		if (pet && pet->GetDistanceFromOwner() < 20 && (pet->IsSummon() || pet->IsMount()))
		{
			pos = pet->GetSkillList()->GetHeadPosition();
			while (pos)
			{
				pSkill = pet->GetSkillList()->GetNext(pos);
				if (pSkill->m_proto->m_type == ST_PET_SKILL_PASSIVE)
					ApplyPassiveSkill(pSkill, pet->GetSympathyPoint() * 100 / PET_MAX_SYMPATHY);		// 교감도 비율로 적용
			}
		}
	}

	ApplyAssistData(&m_avPassiveAddition, &m_avPassiveRate);
}

int CPC::GetAttackLevel()
{
	int attacklevel = 0;

	if (m_wearInventory.wearItemInfo[WEARING_WEAPON])
	{
		CItem* pItem = m_wearInventory.wearItemInfo[WEARING_WEAPON];
		if ((pItem->getFlag() & FLAG_ITEM_COMPOSITION) && (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE))
		{
			CItem* composeItem = m_inventory.FindByVirtualIndex(pItem->m_nCompositeItem);
			if(composeItem == NULL)
				return 0;
#ifdef PREMIUM_CHAR
			attacklevel = composeItem->GetItemLevelForPremiumChar(this->m_premiumChar.getPremiumType());
#else
			attacklevel = composeItem->GetItemLevel();
#endif
		}
		else
		{
#ifdef PREMIUM_CHAR
			attacklevel = pItem->GetItemLevelForPremiumChar(this->m_premiumChar.getPremiumType());
#else
			attacklevel = pItem->GetItemLevel();
#endif
		}
	}

	return attacklevel;
}

int CPC::GetDefenseLevel()
{
	static const int armorscount = 6;
	static int armors[armorscount] = { WEARING_HELMET, WEARING_ARMOR_UP, WEARING_ARMOR_DOWN, WEARING_SHIELD, WEARING_GLOVE, WEARING_BOOTS };

	int defenselevel = 0;
	bool bWear = false;

	int i;

	for (i = 0; i < armorscount; i++)
	{
		if (m_wearInventory.wearItemInfo[armors[i]])
		{
			CItem* pItem = m_wearInventory.wearItemInfo[armors[i]];
			int nThisItemLevel = 0;
			if ((pItem->getFlag() & FLAG_ITEM_COMPOSITION) && (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE))
			{
				CItem* composeItem = m_inventory.FindByVirtualIndex(pItem->m_nCompositeItem);
				if(composeItem == NULL)
					continue;
#ifdef PREMIUM_CHAR
				nThisItemLevel = composeItem->GetItemLevelForPremiumChar(this->m_premiumChar.getPremiumType());
#else
				nThisItemLevel = composeItem->m_itemProto->getItemLevel();
#endif
			}
			else
			{
#ifdef PREMIUM_CHAR
				nThisItemLevel = pItem->GetItemLevelForPremiumChar(this->m_premiumChar.getPremiumType());
#else
				nThisItemLevel = pItem->GetItemLevel();
#endif
			}

			if (defenselevel < nThisItemLevel)
				defenselevel = nThisItemLevel;
			bWear = true;
		}
	}

	if (!bWear)
	{
		defenselevel = m_level * 2;
	}

	return defenselevel;
}

MSG_DAMAGE_TYPE CPC::GetAttackType(const CSkillProto* proto) const
{
	// 프로토가 MAGIC일때만 스킬 속성을 따르고
	// 그외에서는 착용 무기를 따른다
	if (proto)
	{
		switch (proto->m_type)
		{
		case ST_MAGIC:
			return MSG_DAMAGE_MAGIC;
		default:
			break;
		}
	}

	// 기본 공격이 MAGIC일 때 처리
	if (m_wearInventory.wearItemInfo[WEARING_WEAPON])
	{
		if (m_wearInventory.wearItemInfo[WEARING_WEAPON]->IsRangeWeapon())
			return MSG_DAMAGE_RANGE;
		else if (m_wearInventory.wearItemInfo[WEARING_WEAPON]->IsMagicWeapon())
			return MSG_DAMAGE_MAGIC;
		else
			return MSG_DAMAGE_MELEE;
	}

	return MSG_DAMAGE_MELEE;
}

int CPC::GetPKName()
{
	if (m_pkCount < -90)		return PK_NAME_CHAOSLEGION;
	else if (m_pkCount < -70)	return PK_NAME_NECROKNIGHT;
	else if (m_pkCount < -50)	return PK_NAME_OUTLAW;
	else if (m_pkCount < -30)	return PK_NAME_FEARCASTER;
	else if (m_pkCount < -10)	return PK_NAME_MURDERER;
	else if (m_pkCount <= 10)	return PK_NAME_NORMAL;
	else if (m_pkCount <= 30)	return PK_NAME_AVENGER;
	else if (m_pkCount <= 50)	return PK_NAME_LIGHTPORTER;
	else if (m_pkCount <= 70)	return PK_NAME_GUARDIAN;
	else if (m_pkCount <= 90)	return PK_NAME_SAINTKNIGHT;
	else						return PK_NAME_DIVINEPROTECTOR;
}

bool CPC::CanMove()
{
	if (IsSetPlayerState(PLAYER_STATE_SITDOWN))
		return false;
	if (IsSetPlayerState(PLAYER_STATE_WARP))
		return false;
	if (m_personalshop != NULL)
		return false;

	return CCharacter::CanMove();
}

bool CPC::CanAttack()
{
	if (IsSetPlayerState(PLAYER_STATE_SITDOWN))
		return false;
	if (IsSetPlayerState(PLAYER_STATE_WARP))
		return false;
	if (IsSetPlayerState(PLAYER_STATE_MOVING))
		return false;
	if (IsSetPlayerState(PLAYER_STATE_CHANGE))
		return false;

	return CCharacter::CanAttack();
}

bool CPC::CanSpell()
{
	if (IsSetPlayerState(PLAYER_STATE_SITDOWN))
		return false;
	if (IsSetPlayerState(PLAYER_STATE_WARP))
		return false;
// TODO : 여기 스펠 가능 여부 검사 부분 나중에 다시 부활하기
//	if (IsSetPlayerState(PLAYER_STATE_MOVING))
//		return false;

	return CCharacter::CanSpell();
}

int CPC::CanChange()
{
	// 죽었을때
	if (DEAD(this))
		return MSG_CHANGE_ERR_DEAD;

	// 앉아 있을때
	if (m_state & PLAYER_STATE_SITDOWN)
		return MSG_CHANGE_ERR_SITDOWN;

	// 다른 이동장치 사용중
	if (m_state & PLAYER_STATE_WARP)
		return MSG_CHANGE_ERR_WRAP;

	// 스킬 사용중
	if (m_currentSkill)
		return MSG_CHANGE_ERR_SKILL;

	// 교환 중
	if (m_exchange)
		return MSG_CHANGE_ERR_EXCHANGE;

	// 파티 신청중
	if (m_party && m_party->GetBossIndex() == m_index && m_party->GetRequestIndex() > 0)
		return MSG_CHANGE_ERR_PARTY;

	//PK중
#if defined ( FREE_PK_SYSTEM )
	if( !gserver->m_bFreePk )
	{
		if (m_state & PLAYER_STATE_PKMODE || m_state & PLAYER_STATE_RAMODE || m_state & PLAYER_STATE_PKMODEDELAY)
			return MSG_CHANGE_ERR_PK;
	}
#else
	if (m_state & PLAYER_STATE_PKMODE || m_state & PLAYER_STATE_RAMODE || m_state & PLAYER_STATE_PKMODEDELAY)
		return MSG_CHANGE_ERR_PK;
#endif // FREE_PK_SYSTEM

	// 전투중
	if (IsCombatMode())
		return MSG_CHANGE_ERR_BATTLE;

	// 개인상점중
	if (m_personalshop)
		return MSG_CHANGE_ERR_PERSONAL_SHOP;

	return -1;
}

void CPC::do_QuestGiveUp(CPC* ch, CQuest* quest)
{
	if (!quest)
		return;

	const CQuestProto* pQuestProto = quest->GetQuestProto();

	int i;

	switch (pQuestProto->m_type[0])
	{
	// 수집, 전달, 구출 퀘스트
	case QTYPE_KIND_COLLECTION:
	case QTYPE_KIND_DELIVERY:
	case QTYPE_KIND_SAVE:
	case QTYPE_KIND_MINING_EXPERIENCE:
	case QTYPE_KIND_GATHERING_EXPERIENCE:
	case QTYPE_KIND_CHARGE_EXPERIENCE:
	case QTYPE_KIND_PROCESS_EXPERIENCE :
	case QTYPE_KIND_MAKE_EXPERIENCE :
		{
			for (i=0; i < QUEST_MAX_CONDITION; i++)
			{
				if (pQuestProto->m_conditionType[i] == -1)
					continue;

				int nQuestValue = quest->GetQuestValue(i);

				switch (pQuestProto->m_conditionType[i])
				{
				case QCONDITION_ITEM:
					{
						item_search_t vec;
						int search_count = ch->m_inventory.searchItemByCondition(pQuestProto->m_conditionIndex[i], 0, 0, vec);
						if (search_count == 0 || search_count < nQuestValue)
							continue;

						ch->m_inventory.deleteItem(vec, nQuestValue);
					}
					break;
				default:
					break;
				}
			}
		}
		break;

	default:
		break;
	}

	bool bGoZone = false;

	if (pQuestProto->m_type[0] == QTYPE_KIND_DEFEAT || pQuestProto->m_type[0] == QTYPE_KIND_SAVE || pQuestProto->m_type[0] == QTYPE_KIND_TUTORIAL)
		bGoZone = true;

	// Quest GiveUp Log
	GAMELOG << init("QUEST FORCE GIVEUP", ch)
			<< pQuestProto->m_index
			<< end;

// 여기서 포기한 퀘스트 인덱스를 저장하여 다음에는 받지 못하도록 한다.
// 포기한 퀘스트를 저장한다.
	if (pQuestProto->m_type[1] == QTYPE_REPEAT_ONCE)
	{
		ch->m_questList.SetQuestState(quest, QUEST_STATE_ABANDON);
	}
	else
	{
		ch->m_questList.DelQuest(ch, quest);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		QuestForceGiveUpMsg(rmsg, quest);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 격파, 구출 테스트의 경우 존이동
	if (bGoZone)
	{
		// 가까운 마을로
		int nearZone;
		int nearZonePos;
		CZone* pZone = gserver->FindNearestZone(ch->m_pZone->m_index, GET_X(ch), GET_Z(ch), &nearZone, &nearZonePos);
		if (pZone == NULL)
			return ;

		GoZone(ch, nearZone,
			   pZone->m_zonePos[nearZonePos][0],															// ylayer
			   GetRandom(pZone->m_zonePos[nearZonePos][1], pZone->m_zonePos[nearZonePos][3]) / 2.0f,		// x
			   GetRandom(pZone->m_zonePos[nearZonePos][2], pZone->m_zonePos[nearZonePos][4]) / 2.0f);		// z
	}

	// 포기했으면 싱글던전 입장권 받을 기회 줄인다. 최소 0
	ch->m_questList.DecreaseQuestComepleteCount();
}

//0608
void CheckQuestCondition(CPC* ch, CItem* item, LONGLONG itemSum)
{
	if(item->m_itemProto->isQuuestGive() == true)
	{
		CQuest* pQuest;
		CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
		while ((pQuest = pQuestNext))
		{
			pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
			bool bSkip;
			switch (pQuest->GetQuestType0())
			{
			case QTYPE_KIND_REPEAT:
			case QTYPE_KIND_COLLECTION:
			case QTYPE_KIND_DELIVERY:
			case QTYPE_KIND_PROCESS_EXPERIENCE :
			case QTYPE_KIND_MAKE_EXPERIENCE :
				bSkip = false;
				break;
			default:
				bSkip = true;
				break;
			}
			if (!bSkip)
			{
				const CQuestProto* pQuestProto = pQuest->GetQuestProto();
				bool bComplete[QUEST_MAX_CONDITION];
				memset(bComplete, false, sizeof(bool) * QUEST_MAX_CONDITION);

				for(int j = 0; j < QUEST_MAX_CONDITION; j++)
				{
					if(pQuestProto->m_conditionType[j] == QCONDITION_ITEM_NORMAL )
					{
						if(pQuestProto->m_conditionIndex[j] == item->m_itemProto->getItemIndex())
						{
							// LONGLONG itemSum = item->Count();

							if (itemSum < pQuestProto->m_conditionNum[j])
							{
								bComplete[j] = false;
								pQuest->SetQuestValue(j, itemSum);
								if (pQuest->GetComplete2())
								{
									pQuest->SetComplete2(false);
								}

								CNetMsg::SP rmsg(new CNetMsg);
								QuestStatusMsg(rmsg, pQuest);
								SEND_Q(rmsg, ch->m_desc);
							}

							// 같으면 bComplete true 셋팅
							if (itemSum >= pQuestProto->m_conditionNum[j])
							{
								bComplete[j] = true;

								pQuest->SetQuestValue(j, itemSum);
								if(pQuest->GetComplete2())
								{
									pQuest->SetComplete2(false);
								}

								CNetMsg::SP rmsg(new CNetMsg);
								QuestStatusMsg(rmsg, pQuest);
								SEND_Q(rmsg, ch->m_desc);

								continue;
							}
							else if (itemSum < pQuestProto->m_conditionNum[j])
							{
								bComplete[j] = false;
							}
							else
								continue;
						}
						else if(pQuestProto->m_conditionType[j] == -1)
						{
							bComplete[j] = true;
							continue;
						}
						else if((pQuestProto->m_conditionType[j] == QCONDITION_ITEM
								 ||pQuestProto->m_conditionIndex[j] == QCONDITION_NPC) && pQuest->GetComplete1())
						{
							bComplete[j] = true;
							continue;
						}
						else if(pQuestProto->m_conditionType[j] == QCONDITION_ITEM_NORMAL
								&& (pQuestProto->m_conditionNum[j] <= pQuest->GetQuestValue(j)))
						{
							bComplete[j] = true;
							continue;
						}
						else
						{
							bComplete[j] = false;
							continue;
						}
					}
					else if ( pQuestProto->m_conditionType[j] == QCONDITION_ITEM )
					{
						if(pQuestProto->m_conditionIndex[j] == item->m_itemProto->getItemIndex())
						{
							if (itemSum >= pQuestProto->m_conditionNum[j])
							{
								bComplete[j] = true;

								pQuest->SetQuestValue(j, itemSum);
								pQuest->SetComplete1(true);
								if(pQuest->GetComplete2())
								{
									pQuest->SetComplete2(false);
								}

								CNetMsg::SP rmsg(new CNetMsg);
								QuestStatusMsg(rmsg, pQuest);
								SEND_Q(rmsg, ch->m_desc);

								continue;
							}
						}
						else if(pQuestProto->m_conditionNum[j] <= pQuest->GetQuestValue(j))
						{
							bComplete[j] = true;
							continue;
						}
					}
					else if ( pQuestProto->m_conditionType[j] == QCONDITION_ITEM_USE ) // 사용해야 완료하는 퀘스트이므로 패스
					{
						continue;
					}
					else
					{
						bComplete[j] = true;
					}
				}
				//pQuest->CheckComplete(ch); // 위 완료 체크 방식이 즈질 이다. 입시 방편으로 이넘으로 완료 체크 한다. [7/31/2009 derek]
				if(pQuest->IsCompleted())//이미 완료된 퀘스트는 skip
				{
					continue;
				}

				for (int k=0; k < QUEST_MAX_CONDITION; k++)
				{
					if (!bComplete[k])
						break;

					if(k == QUEST_MAX_CONDITION-1)
					{
						pQuest->SetComplete2(true);

						if(pQuest->GetComplete1())//퀘스트 아이템도 완료가 되었다면,
						{
							GAMELOG << init("QUEST COMPLETE (CheckQeustCondition)", ch)
									<< pQuestProto->m_index
									<< end;
//#ifdef GER_LOG
//							for( int rewardItemCount = 0 ; rewardItemCount < 5 ; rewardItemCount++ )
//							{
//								GAMELOGGEM << init( 0, "CHAR_QUEST_FINISHED")
//									<< LOG_VAL("PC.cpp",":6021") << blank
//									<< LOG_VAL("account-id", ch->m_desc->m_idname ) << blank
//									<< LOG_VAL("character-id", ch->m_desc->m_pChar->m_name ) << blank
//									<< LOG_VAL("quest-id", pQuest->GetQuestIndex() ) << blank
//									<< LOG_VAL("zone-id", ch->m_desc->m_pChar->m_pZone->m_index ) << blank
//									<< LOG_VAL("End-type", pQuest->GetQuestProto()->m_prizeNPC ) << blank
//									<< LOG_VAL("npc-id", pQuest->GetPrizeNPCIndex() ) << blank
//									<< LOG_VAL("item-id", pQuest->GetQuestProto()->m_prizeIndex[rewardItemCount] ) << blank
//									<< LOG_VAL("amount", pQuest->GetQuestProto()->m_prizeData[rewardItemCount] ) << blank
//									<< endGer;
//							}
//#endif //GER_LOG
							// 완료 메시지
							CNetMsg::SP rmsg(new CNetMsg);
							QuestCompleteMsg(rmsg, pQuest);
							SEND_Q(rmsg, ch->m_desc);
						}
					}
				}
			}
		}
	}
}

bool CPC::CheckDungeonData(int zone)
{
	CQuestProto* qproto = NULL;
	switch (zone)
	{
	case ZONE_SINGLE_DUNGEON3:
		qproto = gserver->m_questProtoList.FindProto(104);
		if (m_pd3Count >= 5)
			return false;
		break;

	case ZONE_SINGLE_DUNGEON4:
		qproto = gserver->m_questProtoList.FindProto(105);
		if (m_pd4Count >= 3)
			return false;
		break;
	}

	if( this->m_admin < 2 && qproto && ( this->m_level < qproto->m_needMinLevel || this->m_level > qproto->m_needMaxLevel ) )
		return false;

	return true;
}

int CPC::GetJoinFlag(int zoneindex)
{
	switch (zoneindex)
	{
	case ZONE_MERAC:
		return m_joinMerac;
	case ZONE_DRATAN:
		return m_joinDratan;
	default:
		return WCJF_NONE;
	}
//#else
//	return 0;
}

void CPC::SetJoinFlag(int zoneindex, int value)
{
	switch (zoneindex)
	{
	case ZONE_MERAC:
		m_joinMerac = value;
		return;
	case ZONE_DRATAN:
		m_joinDratan = value;
		return;
	default:
		return ;
	}
}

bool CPC::InitSkill(CSkillList *skillList, bool isPassive)
{
	CSkill *skill = NULL;
	LONGLONG skillPoint = 0;
	char skillLevel = 0;
	int skillCount = 0;

	if(!skillList)
	{
		// 초기화할 스킬이 없음을 메세지보냄
		return false;
	}

	skillCount = skillList->count();

	CSkillListNode* node = NULL;

	node = (CSkillListNode *) skillList->GetHeadPosition();

	if(!node)
	{
		// 초기화할 스킬이 없음을 메세지보냄
		return false;
	}
	skill = node->skill();
	for(int i = 0; i < skillCount; i ++)
	{
		skillLevel = skill->m_level;
		for(; skillLevel > 0; skillLevel--)
		{
			skillPoint += skill->m_proto->Level(skillLevel)->m_learnSP * 10000;
		}
		node = node->next();
		if(!node) break;
		skill = node->skill();
	}

	skillList->Clear();

	if( !isPassive )
	{
		switch (this->m_job)
		{
		case JOB_TITAN:
			skill = gserver->m_skillProtoList.Create(1);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;

		case JOB_KNIGHT:
			skill = gserver->m_skillProtoList.Create(14);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;

		case JOB_HEALER:
			skill = gserver->m_skillProtoList.Create(27);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			skill = gserver->m_skillProtoList.Create(31);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;

		case JOB_MAGE:
			skill = gserver->m_skillProtoList.Create(53);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;

		case JOB_ROGUE:
			skill = gserver->m_skillProtoList.Create(116);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;

		case JOB_SORCERER:
			skill = gserver->m_skillProtoList.Create(292);
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;
#ifdef EX_ROGUE
		case JOB_EX_ROGUE:
			skill = gserver->m_skillProtoList.Create(116);	// EXROGUE : 초기 스킬 인덱스 수정
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;
#endif // EX_ROGUE
#ifdef EX_MAGE
		case JOB_EX_MAGE:
			skill = gserver->m_skillProtoList.Create(1637);	// EXMAGE : 초기 스킬 인덱스 수정(?)
			skillList->Add(skill);
			skillPoint -= skill->m_proto->Level(1)->m_learnSP * 10000;
			break;
#endif // EX_MAGE
		default:
			break;
		}
	}
	this->m_skillPoint += skillPoint;

	skill = NULL;
	delete skill;

	return true;
}
bool CPC::InitSSkill()
{
	CSSkill *skill = NULL;
	CSSkillNode* node = NULL;
	LONGLONG skillPoint = 0;
	char skillLevel = 0;
	int skillCount = 0;

	skillCount = m_sSkillList.m_nCount;

	if(!skillCount) return false;

	node = m_sSkillList.m_head;

	skill = node->m_sskill;
	for(int i = 0; i < skillCount; i ++)
	{
		bool bAddSkillPoint = true;
		while(skill)
		{
			if(bAddSkillPoint)
			{
				skillLevel = skill->m_level - 1;
				for(; skillLevel >= 0; skillLevel--)
				{
					// TODO: 스킬 포인트 계산 및 SSkill포인트 구하기 위한 알고리즘 필요
					skillPoint += skill->m_proto->m_needSP[(int)skillLevel] * 10000;
				}
			}

			if(skill->m_proto->m_needSSkill != -1)
			{
				skill = gserver->m_sSkillProtoList.Create(skill->m_proto->m_needSSkill, skill->m_proto->m_needSSkillLevel);

				CSSkillNode* node2 = NULL;
				bAddSkillPoint = true;
				node2 = m_sSkillList.m_head;
				while(node2)
				{
					if(node2->m_sskill)
					{
						if(node2->m_sskill->m_proto->m_index == skill->m_proto->m_index)
						{
							bAddSkillPoint = false;
							break;
						}
					}
					node2 = node2->m_next;
				}
			}
			else
				break;
		}

		//m_sSkillList.Remove(node->m_sskill);
		node = node->m_next;

		if(!node) break;
		skill = node->m_sskill;
	}

	m_sSkillList.Init();

	skill = gserver->m_sSkillProtoList.Create(1);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;

	skill = gserver->m_sSkillProtoList.Create(5);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;

	skill = gserver->m_sSkillProtoList.Create(9);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;

	skill = gserver->m_sSkillProtoList.Create(13);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;

	skill = gserver->m_sSkillProtoList.Create(18);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;

	skill = gserver->m_sSkillProtoList.Create(23);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;

	skill = gserver->m_sSkillProtoList.Create(44);
	m_sSkillList.Add(skill);
	skillPoint -= skill->m_proto->m_needSP[0] * 10000;

	this->m_skillPoint += skillPoint;

	skill = NULL;
	node = NULL;
	delete skill;
	delete node;

	return true;
}

void CPC::resetCoolTime( CSkillList* skillList )
{
	//쿨타임 및 퀵슬롯 초기화 패킷 전송
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_SKILL);
		RefMsg(rmsg) << (unsigned char)MSG_SKILL_RESET_COOLTIME;
		SEND_Q(rmsg, this->m_desc);
	}

	CSkill *skill = NULL;
	int skillCount = 0;

	skillCount = skillList->count();

	CSkillListNode* node = NULL;

	node = (CSkillListNode *) skillList->GetHeadPosition();

	if(!node)
	{
		//초기 스킬 지급 없음
		return;
	}
	skill = node->skill();
	for(int i = 0; i < skillCount; i ++)
	{
		skill->m_usetime = 0;
		node = node->next();
		if(!node) break;
		skill = node->skill();
	}

	for (int page = 0; page < QUICKSLOT_PAGE_NUM; ++page)
	{
		for (int slot = 0; slot < QUICKSLOT_MAXSLOT; ++slot)
		{
			if (this->m_quickSlot[page].m_slotType[slot] != QUICKSLOT_TYPE_SKILL)
				continue;

			this->m_quickSlot[page].m_skillType[slot] = -1;
		}
	}	
}

bool CPC::IsCombatMode()
{
	return ((gserver->m_pulse - m_lastAttackPulse) < PULSE_ATTACKMODE_DELAY);
}

void CPC::UpdatePetValue()
{
	// 060221 : bs : 피스존/공성지역에서는 경험치/교감도/배고픔 변화 없음
	if (IsInPeaceZone(true) || GetMapAttr() & MATT_WAR)
		return ;

	CPet* p = m_petList;
	while (p)
	{
		// 060221 : bs : 펫 봉인 상태 추가
		if (p->GetRemainRebirthTime() > 0)
		{
			// 공격중일때
			if (IsCombatMode() && (gserver->m_pulse - m_pulseLastAttackSkill) < PULSE_ATTACKMODE_DELAY)
			{
				p->SetRemainRebirthTime(p->GetRemainRebirthTime() - 1);
				if (p->GetRemainRebirthTime() < 1)
				{
					// TODO : petlog
					GAMELOG << init("PET REBIRTH AUTO", this)
							<< "PET" << delim
							<< p->m_index << delim
							<< "TYPE" << delim
							<< p->GetPetType() << delim
							<< "LEVEL" << delim
							<< p->m_level
							<< end;
					p->m_hp = 30;
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPetStatusMsg(rmsg, p);
					SEND_Q(rmsg, m_desc);
				}
			}
		}
		else
		{
			// 060221 : bs : 피스존/공성지역에서는 경험치/교감도/배고픔 변화 없음
			if (!p->IsSummon() || !(p->IsInPeaceZone(true) || p->GetMapAttr() & MATT_WAR))
			{
				// 050309: bw 펫이 20미터 이상 떨어지거나 안에 들어오는 순간 거시기 calcstatus호출
				// 배고픔 0이면 경험치 고정, 스킬 유지, 교감도 5분에 1포인트 하락
				bool bexp = false;
				bool bchangenear = false;
				bool bnear = false;

				if( p->GetDistanceFromOwner() < 20 )
				{
					if ( p->WasNear() )
						bchangenear = false;
					else
						bchangenear = true;
					bnear = true;
				}
				else
				{
					if( p->WasNear() )
						bchangenear = true;
					else
						bchangenear = false;
				}

				// 배고픔이 0 아니고 근처에 있으면
				if( p->IsActive() && bnear)
					bexp = p->AddExp();

				bool bsympathy = p->UpdateSympathyPoint(bnear);
				bool bhungry = p->DecreaseHungryPoint();
				//펫이 펫창고에 있는 경우에는 1시간마다
				if(p->GetOwner()->m_petStashManager.GetPetItemByPlus(p->m_index) != NULL)
				{
					if( p->m_petStashHungrySeconds + (60 * 60/*sec*/) < gserver->m_nowseconds)
					{
						p->m_petStashHungrySeconds = gserver->m_nowseconds;
						bhungry = p->IncreaseHungryPoint(PET_STASH_INCREASE_POINT);
						p->GetOwner()->m_petStashManager.UpdatePetData(p->m_index);
					}
				}

				if (bexp || bsympathy || bhungry)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPetStatusMsg(rmsg, p);
					SEND_Q(rmsg, m_desc);
				}

				if( bsympathy || bchangenear)
				{
					CPC* pc = p->GetOwner();
					pc->CalcStatus(true);
					p->SetWasNear(bnear);
				}
			}
		}

		p = p->m_nextPet;
	}
}

CPet* CPC::GetPet(int index)
{
	if (index == 0)
	{
		if (m_wearInventory.wearItemInfo[WEARING_PET])
		{
			index = m_wearInventory.wearItemInfo[WEARING_PET]->getPlus();
		}
		else
			return NULL;
	}

	CPet* p = m_petList;
	while (p)
	{
		if (p->m_index == index)
			return p;

		p = p->m_nextPet;
	}

	return NULL;
}

void CPC::DelPet(int petIndex)
{
	// 펫 리스트에서 삭제 대상을 찾아
	// 아이템 리스트에서 해당 악세 삭제
	// 펫을 리스트에서 제거
	// 펫 삭제

	CPet* petNext = m_petList;
	CPet* pet;
	while ((pet = petNext))
	{
		petNext = pet->m_nextPet;

		if (pet->m_index == petIndex)
		{
			int itemdbindex = 0;
			const char* petTypeName;
			switch (pet->GetPetType())
			{
			/*
			#define PET_HORSE_ITEM_INDEX			871				// 말 아이템 인덱스
			#define PET_DRAGON_ITEM_INDEX			872				// 용 아이템 인덱스
			#define PET_BLUE_HORSE_ITEM_INDEX		1710			// 말 아이템 인덱스
			#define PET_PINK_DRAGON_ITEM_INDEX		1711			// 용 아이템 인덱스
			#define PET_UNKOWN_HORSE_ITEM_INDEX		1713			// 말 아이템 인덱스
			#define PET_UNKOWN_DRAGON_ITEM_INDEX	1712			// 용 아이템 인덱스
			*/
			case PET_TYPE_HORSE:
				itemdbindex = PET_HORSE_ITEM_INDEX;
				petTypeName = "HORSE";
				break;
			case PET_TYPE_BLUE_HORSE:
				itemdbindex = PET_BLUE_HORSE_ITEM_INDEX;
				petTypeName = "BLUE_HORSE";
				break;
			case PET_TYPE_UNKOWN_HORSE:
				itemdbindex = PET_UNKOWN_HORSE_ITEM_INDEX;
				petTypeName = "UNKOWN_HORSE";
				break;
			case PET_TYPE_DRAGON:
				itemdbindex = PET_DRAGON_ITEM_INDEX;
				petTypeName = "DRAGON";
				break;
			case PET_TYPE_PINK_DRAGON:
				itemdbindex = PET_PINK_DRAGON_ITEM_INDEX;
				petTypeName = "PINK_DRAGON";
				break;
			case PET_TYPE_UNKOWN_DRAGON:
				itemdbindex = PET_UNKOWN_DRAGON_ITEM_INDEX;
				petTypeName = "UNKOWN_DRAGON";
				break;
			}

			CItem* item = NULL;

			// 소환되어 있는 펫은 셀에서 제거
			if (pet->IsSummon())
			{
				pet->Disappear();
				item = this->m_wearInventory.getWearItem(WEARING_PET);
			}
			// 타고 있으면 내리기
			else if (pet->IsMount())
			{
				pet->Mount(false);
				item = this->m_wearInventory.getWearItem(WEARING_PET);
			}
			else
			{
				item = this->m_inventory.FindByDBIndex(itemdbindex, petIndex);
			}

			if (item)
			{
				// 펫 장착 해제
				if (pet->IsWearing())
				{
					this->m_wearInventory.RemoveItem(item->getWearPos());
				}
				else
				{
					this->m_inventory.decreaseItemCount(item, 1);
				}
			}

			// TODO : petlog
			GAMELOG << init("PET DELETE", m_name, m_desc->m_idname)
					<< "TYPE" << delim
					<< petTypeName << delim
					<< "GRADE" << delim
					<< pet->GetPetGrade() << delim
					<< "LEVEL" << delim
					<< pet->m_level << delim
					<< "EXP" << delim
					<< pet->m_exp
					<< end;

			// 펫 리스트에서 삭제
			REMOVE_FROM_BILIST(pet, m_petList, m_prevPet, m_nextPet);
			DelAttackList(pet);
			delete pet;

			break;
		}
	}
}

bool CPC::CanPvP(CCharacter* target, bool bIgnoreInvisible)
{
	if (!target)								return false;

	CPC* tpc = NULL;
	switch (target->m_type)
	{
	case MSG_CHAR_PC:
		tpc = TO_PC(target);
		break;

	case MSG_CHAR_PET:
		tpc = TO_PET(target)->GetOwner();
		break;

	case MSG_CHAR_ELEMENTAL:
		tpc = TO_ELEMENTAL(target)->GetOwner();
		break;

	default:
		break;
	}

	if (!tpc)
		return true;

	// 사망검사
	if (DEAD(this))								return false;
	if (DEAD(target))							return false;

	// 운영자 검사
	if (m_bImmortal && m_bFirstDoIt == false)	return false;
	if (tpc->m_bImmortal)						return false;

	bool bSkipLevel = false;

#ifdef CHANGE_WARCASTLE_SETTING
	int nZoneIdx = -1;
	CWarCastle* castle = CWarCastle::GetCastleObject(ZONE_DRATAN);
	if (castle && castle->GetState() != WCSF_NORMAL)
	{
		nZoneIdx = ZONE_DRATAN;
	}
	else
	{
		castle = CWarCastle::GetCastleObject(ZONE_MERAC);
		if (castle && castle->GetState() != WCSF_NORMAL)
		{
			nZoneIdx = ZONE_MERAC;
		}
	}
	if ( nZoneIdx > 0 )
	{
		// 공,수성 관계일 경우 레벨 상관없이 공격 가능하다.
		if ( (IS_DEFENSE_TEAM(this->GetJoinFlag(nZoneIdx)) && IS_ATTACK_TEAM(tpc->GetJoinFlag(nZoneIdx))) ||
				( IS_ATTACK_TEAM(this->GetJoinFlag(nZoneIdx)) && tpc->GetJoinFlag(nZoneIdx) != WCJF_NONE) )
		{
			bSkipLevel = true;
		}
		// 공격자가 공수성이고, 타겟이 3세력이면 공성 전장 내에서 레벨 상관없이 공격 가능하다.
#ifdef CHECK_CASTLE_AREA
		if ((this->m_pZone->m_index == nZoneIdx && tpc->m_pZone->m_index == nZoneIdx) &&
				(this->GetMapAttr() & MATT_WAR || this->m_pZone->IsWarZone((int)this->m_pos.m_x, (int)this->m_pos.m_z)) &&
				(tpc->GetMapAttr() & MATT_WAR || tpc->m_pZone->IsWarZone((int)tpc->m_pos.m_x, (int)tpc->m_pos.m_z)) )
#else
		if ( this->m_pZone->m_index == nZoneIdx && tpc->m_pZone->m_index == nZoneIdx && this->GetMapAttr() == tpc->GetMapAttr() && this->GetMapAttr() & MATT_WAR )
#endif // CHECK_CASTLE_AREA
		{
			if ( (IS_DEFENSE_TEAM(this->GetJoinFlag(nZoneIdx)) && tpc->GetJoinFlag(nZoneIdx) == WCJF_NONE ) ||
					( IS_ATTACK_TEAM(this->GetJoinFlag(nZoneIdx)) && tpc->GetJoinFlag(nZoneIdx) == WCJF_NONE) )
				bSkipLevel = true;
		}
	}
#else // CHANGE_WARCASTLE_SETTING
#ifdef CHECK_CASTLE_AREA
	CWarCastle* castle = CWarCastle::GetCastleObject(m_pZone->m_index);
	if (castle && castle->GetState() != WCSF_NORMAL)
	{
		if( (m_pZone->m_index == target->m_pZone->m_index) &&
				(GetMapAttr() & MATT_WAR || m_pZone->IsWarZone((int)m_pos.m_x, (int)m_pos.m_z) ) &&
				( target->GetMapAttr() & MATT_WAR || target->m_pZone->IsWarZone((int)target->m_pos.m_x, (int)target->m_pos.m_z)) ) // 둘다 공성지역 안에 있어야 하고, 공격자가 공성에 참여해있어야 한다.
			bSkipLevel = true;
	}
#else
	// 공성전 지역에서 공성 진행시에는 레벨 검사 안함(BRZ 공성전 15레벨 이하 공격  안됨:에러 수정:2009/0702)
	if (m_pZone->m_index == target->m_pZone->m_index && (m_pZone->m_index == ZONE_DRATAN || m_pZone->m_index == ZONE_MERAC) &&
			GetMapAttr() == target->GetMapAttr() && GetMapAttr() & MATT_WAR)
//	if (m_pZone->m_index == target->m_pZone->m_index && m_pZone->m_index == CWarCastle::GetCurSubServerCastleZoneIndex() && GetMapAttr() == target->GetMapAttr() && GetMapAttr() == MAPATT_WARZONE) //에러
	{
		CWarCastle* castle = CWarCastle::GetCastleObject(m_pZone->m_index);
		if (castle && castle->GetState() != WCSF_NORMAL)
		{
			bSkipLevel = true;
		}
	}
#endif // CHECK_CASTLE_AREA
#endif // CHANGE_WARCASTLE_SETTING

#ifdef FREE_PK_SYSTEM
	bSkipLevel = true;
#endif // FREE_PK_SYSTEM

	if (target->m_pZone->isRVRZone())
	{
		bSkipLevel = true;
	}
		
	// 적으로 인식 가능?
	if (!IsEnemy(tpc))						return false;

	if (!bSkipLevel && tpc->m_level <= PKMODE_LIMIT_LEVEL)
	{
		// 대상 케릭의 레벨이 거시기허요 메세지 보냄
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_TARGET_PKLIMITLEVEL);
		SEND_Q(rmsg, this->m_desc);
		return false;
	}

	// 보이는지 검사
	if (!bIgnoreInvisible && !CanVisible(target))
		return false;

	// 레벨 검사
	if (!bSkipLevel && m_level <= PKMODE_LIMIT_LEVEL)	return false;

	// 행동 불능 검사
	if (IsDisable())							return false;
	if (tpc->IsDisable())						return false;

	// 피스존 검사 : 공격자는 속성맵 BLOCK 포함
	if (IsInPeaceZone(true))					return false;

	// 피스존 검사 : 방어자는 속성맵 BLOCK 미포함
	if (target->IsInPeaceZone(false))			return false;

	// PK로 시도시에만 정방
	if (IsSetPlayerState(PLAYER_STATE_PKMODE) && !tpc->IsSetPlayerState(PLAYER_STATE_PKMODE))
	{
		// 서로 리스트에 없을 때
		if (!IsRaList(this, tpc))
		{
			AddRaList(this, tpc);
		}
		else
		{
			CRaChar* raList = tpc->m_raList;

			while (raList)
			{
				if (raList->m_raTarget == this && raList->m_bAttacker)
				{
					raList->m_raPulse = RAMODE_USE_PULSE;
					break;
				}
				raList = raList->m_next;
			}
		}
	}

	return true;
}

void CPC::CancelChange()
{
	if (IsSetPlayerState(PLAYER_STATE_CHANGE))
	{
		m_changeIndex = -1;
		m_changePulse = 0;

		ResetPlayerState(PLAYER_STATE_CHANGE);

		if (m_pArea)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ChangeStopMsg(rmsg, this);
			m_pArea->SendToCell(rmsg, this, true);
		}
	}
}

CElemental* CPC::SummonElemental(char elementalType)
{
	switch (elementalType)
	{
	case ELEMENTAL_FIRE:
	case ELEMENTAL_WIND:
	case ELEMENTAL_EARTH:
	case ELEMENTAL_WATER:
		m_pulseElemental[(int)elementalType] = gserver->m_pulse;
		break;
	default:
		return NULL;
	}

	CElemental* elemental = new CElemental(this, elementalType);
	ADD_TO_BILIST(elemental, m_elementalList, m_prevElemental, m_nextElemental);

	// 좌표 지정
	GET_R(elemental) = GET_R(this);
	GET_YLAYER(elemental) = GET_YLAYER(this);

	// 적정 좌표 구하기
	float x, z;
	int nTry = 10;
	while (nTry--)
	{
		x = GET_X(this) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
		z = GET_Z(this) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
		if ( !(m_pArea->GetAttr(GET_YLAYER(this), x, z) & MATT_UNWALKABLE) )
			break;
	}

	GET_X(elemental) = x;
	GET_Z(elemental) = z;
	int cx, cz;
	m_pArea->PointToCellNum(x, z, &cx, &cz);
	m_pArea->CharToCell(elemental, GET_YLAYER(elemental), cx, cz);

	// 패시브 적용을 위해
	elemental->CalcStatus(true);
	elemental->m_maxHP += elemental->GetOwner()->m_opJewelElementPetHPUP;
	elemental->m_opMelee += elemental->GetOwner()->m_opJewelElementAttUP;
	elemental->m_opMagic += elemental->GetOwner()->m_opJewelElementMagicAttUP;

	elemental->m_hp = elemental->m_maxHP;
	elemental->m_mp = elemental->m_maxMP;

	{
		// 나타나기
		CNetMsg::SP rmsg(new CNetMsg);
		AppearMsg(rmsg, elemental, true);
		m_pArea->SendToCell(rmsg, GET_YLAYER(elemental), cx, cz);
	}

	{
		// 주인에게 소환 알리기
		CNetMsg::SP rmsg(new CNetMsg);
		ExElementalStatusMsg(rmsg, elemental);
		SEND_Q(rmsg, m_desc);
	}

	//소환수에게 주인의 결사대 타입을 저장
	elemental->setSyndicateType(getSyndicateType());

	return elemental;
}

void CPC::UnsummonElemental(CElemental* elemental)
{
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExElementalDeleteMsg(rmsg, elemental);
		SEND_Q(rmsg, m_desc);
	}

	CElemental* pElemental;
	CElemental* pElementalNext = m_elementalList;

	while ((pElemental = pElementalNext))
	{
		pElementalNext = pElemental->m_nextElemental;
		if (pElemental == elemental)
		{
			switch (elemental->GetElementalType())
			{
			case ELEMENTAL_FIRE:
			case ELEMENTAL_WIND:
			case ELEMENTAL_EARTH:
			case ELEMENTAL_WATER:
				m_pulseElemental[(int)elemental->GetElementalType()] = gserver->m_pulse;
				break;
			}

			DelAttackList(elemental);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				DisappearMsg(rmsg, elemental);
				elemental->m_pArea->SendToCell(rmsg, elemental, true);
			}

			elemental->m_pArea->CharFromCell(elemental, true);
			REMOVE_FROM_BILIST(elemental, m_elementalList, m_prevElemental, m_nextElemental);
			delete elemental;
			return ;
		}
	}
}

bool CPC::CanSummonElemental(char newElementalType, char* duplicationType, int* nRemainTimeSummon, bool* errElementalType)
{
	*duplicationType = -1;
	*nRemainTimeSummon = 0;
	*errElementalType = false;

	int pulseSummon = -1;
	char elementalType[3];		// { 소환하려는 타입, 중복 불가 1, 중복 불가 2 }
	elementalType[0] = newElementalType;

	switch (elementalType[0])
	{
	case ELEMENTAL_FIRE:
	case ELEMENTAL_WIND:
		// 대지, 물과는 불가능
		elementalType[1] = ELEMENTAL_EARTH;
		elementalType[2] = ELEMENTAL_WATER;
		break;

	case ELEMENTAL_EARTH:
	case ELEMENTAL_WATER:
		// 불, 바람과는 불가능
		elementalType[1] = ELEMENTAL_FIRE;
		elementalType[2] = ELEMENTAL_WIND;
		break;

	default:
		*errElementalType = true;
		return false;
	}

	// 중복 검사
	CElemental* elemental = m_elementalList;
	while (elemental)
	{
		int i;
		for (i = 0; i < 3; i++)
		{
			if (elemental->GetElementalType() == elementalType[i])
			{
				*duplicationType = elemental->GetElementalType();
				return false;
			}
		}
		elemental = elemental->m_nextElemental;
	}

	// 시간 검사
	pulseSummon = m_pulseElemental[(int)elementalType[0]];
	if (pulseSummon >= 0 && pulseSummon + SUMMON_DELAY >= gserver->m_pulse)
	{
		*nRemainTimeSummon = (pulseSummon + SUMMON_DELAY - gserver->m_pulse) / PULSE_REAL_SEC;
		return false;
	}

	return true;
}

CElemental* CPC::GetElemental(int elementalindex)
{
	CElemental* pElemental = m_elementalList;
	while (pElemental)
	{
		if (pElemental->m_index == elementalindex)
			return pElemental;
		pElemental = pElemental->m_nextElemental;
	}
	return NULL;
}

void CPC::ChangeEvocationState(int skillIndex)
{
	if (!m_pArea)
		return ;

	if( m_evocationIndex != EVOCATION_NONE )
	{
		//풀어라
		if(skillIndex != m_evocationIndex)
			return;

		m_evocationIndex = EVOCATION_NONE;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExEvocationStopMsg(rmsg, this);
			m_pArea->SendToCell(rmsg, this, true);
		}
	}
	else
	{
		switch (skillIndex)
		{
		case EVOCATION_HELLOUND:
			if (m_pulseEvocation[0] == 0)
				m_pulseEvocation[0] = gserver->m_pulse;
			m_evocationIndex = skillIndex;
			break;

		case EVOCATION_ELENEN:
			m_evocationIndex = skillIndex;
			if (m_pulseEvocation[1] == 0)
				m_pulseEvocation[1] = gserver->m_pulse;
			break;

		default:
			return ;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExEvocationStartMsg(rmsg, this);
			m_pArea->SendToCell(rmsg, this, true);
		}
	}

	CalcStatus(true);
}

void CPC::Unevocation()
{
	CalcStatus(true);

	if (!m_pArea)
		return ;

	this->toggleOff(m_evocationIndex, TOGGLE_SKILL);
	m_evocationIndex = EVOCATION_NONE;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExEvocationStopMsg(rmsg, this);
		m_pArea->SendToCell(rmsg, this, true);
	}
}

int CPC::GetRemainEvocation(bool bSec)
{
	int ret = 0;
	switch (m_evocationIndex)
	{
	case EVOCATION_HELLOUND:
		ret = (EVOCATION_DURATION - (gserver->m_pulse - m_pulseEvocation[0]));
		break;
	case EVOCATION_ELENEN:
		ret = (EVOCATION_DURATION - (gserver->m_pulse - m_pulseEvocation[1]));
		break;
	default:
		return 0;
	}
	if (bSec)
		return ret / 10;
	else
		return ret;
}

void CPC::ProcDisconnect(bool bDisconnect, bool bGoto)
{
	if(this->IsParty() == true)
	{
		this->m_party->deletePartyRecallMember(this->m_index);
	}

	if (m_bProcDisconnect)
		return ;
	if (bDisconnect)
		m_bProcDisconnect = true;

	//유물 아이템을 갖고 있는 유저라면 삭제
	if(bDisconnect == true)
	{
		ArtifactManager::instance()->disconnect(this);
	}

	CArea*		pCurArea = m_pArea;
	CZone*		pCurZone = m_pZone;

#ifdef EXTREME_CUBE
	CGuildMember*		pGuildInfo = NULL;
#endif // EXTREME_CUBE

	if (!m_bLoadChar)
		return ;

#if defined (EXTREME_CUBE)

	CParty* pParty = gserver->FindPartyByMemberIndex(m_index, false);

	if(pParty)
	{
		if( !(pParty->GetRequestIndex() != m_index && pParty->GetMemberCount() > 1) )
		{
			pParty = NULL;
		}
	}

	// 콤보존과 관련된 콤보스킬 모두 제거
	m_assist.CureAssist(SF_COMBO);
	// 콤보편집중 종료처리
	if(bDisconnect)
		m_listSaveComboData.clear();
#endif

#ifdef EXTREME_CUBE
	pGuildInfo = m_guildInfo;
#ifdef EXTREME_CUBE_VER2
	m_assist.CureAssist(SF_ZONE);
#endif // EXTREME_CUBE_VER2
#endif// EXTREME_CUBE

	// 길드 가입 신청 상태 해제
	if (m_regGuild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if (m_guildInfo)
			GuildRegistCancelMsg(rmsg, false);
		else
			GuildRegistCancelMsg(rmsg, true);
		do_Guild(this, rmsg);
	}

	// 길드장이고 길드전 관련 신청중이면 취소
	if (m_guildInfo && m_guildInfo->guild() && m_guildInfo->guild()->boss() && m_guildInfo->guild()->boss()->charindex() == m_index)
	{
		CGuild* guild = m_guildInfo->guild();
		if (guild && (guild->battleState() == GUILD_BATTLE_STATE_START_WAIT || guild->battleState() == GUILD_BATTLE_STATE_STOP_WAIT))
		{
			// 길드 신청중일때
			if (guild->battleState() == GUILD_BATTLE_STATE_START_WAIT)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildBattleReqRejectMsg(rmsg, m_index);
				do_Guild(this, rmsg);
			}
			// 길드 중단 신청 중일때
			else if (guild->battleState() == GUILD_BATTLE_STATE_STOP_WAIT)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildBattleStopRejectMsg(rmsg, m_index);
				do_Guild(this, rmsg);
			}
		}
	}

	if (m_guildInfo != NULL)
	{
		// 길드 스킬 시전시 Ready 상태인 경우 플래그를 clear함
		m_guildInfo->guild()->clearReadySkill();
	}

	// 교환 중이면 취소하기
	if (m_exchange)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExchangeReqMsg(rmsg, MSG_EXCHANGE_REQ_REJECT, NULL, NULL);
		do_Exchange(this, rmsg);
	}

	// 사제 신청중이면 취소하기
	if (m_teachWait != -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachTeacherReqRejectMsg(rmsg, this);
		do_Teach(this, rmsg);
	}

	// 개인 상점 취소
	m_personalshop.reset();

	if(m_nMoonStoneDigit != -1) //문스톤을 돌리는 중이었다..
	{
		gserver->MoonStoneEndProcess(this);
	}

	// 다른 존으로 이동하거나 접속 종료시에만 처리되는 루틴
	if (!bGoto)
	{
		// 테이밍 중이면 테이밍을 해제
		if( IsTaming() )
		{
			set_npc_t::iterator it = m_listTaming.begin();
			set_npc_t::iterator endit = m_listTaming.end();
			for(; it != endit; ++it)
			{
				CNPC* p = *(it);

				p->SetOwner(NULL);
				p->m_assist.CureAssist(MST_ASSIST_TAMING, 99);
				DelAttackList(p);
			}
			m_bTaming_npc = false;
			m_listTaming.clear();
		}

		if ( bDisconnect == true )
		{
			if (pCurZone->m_index == ZONE_ALTER_OF_DARK || pCurZone->m_index == ZONE_CAPPELLA_1 || pCurZone->m_index == ZONE_AKAN_TEMPLE
				|| pCurZone->m_index == ZONE_TARIAN_DUNGEON
				)
			{
				//감소를 시킬때는 bInCount 가 false인 상황에서만 감소를 시킨다. true는 완전히 이동이 된 상태이기때문에 제외한다.
				if(this->bMoveInsDunFinish == false)
					pCurArea->m_inCount--;
			}
		}

		SummonNpcRemoveAll(true);

		// 길드 가입자는 오프라인 알리기
		if (bDisconnect && m_guildInfo && m_guildInfo->GetPC() != NULL)
		{
			m_guildInfo->online(0);

			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildOnline(rmsg, m_guildInfo);
			RefMsg(rmsg) << (int)0;
			m_guildInfo->SetPC(NULL);
			SEND_Q(rmsg, gserver->m_helper);
		}

		// 펫 사라짐
		CPet* petNext = m_petList;
		CPet* pet;
		while ((pet = petNext))
		{
			if(!pet)
				break;

			petNext = pet->m_nextPet;
			if (pet->IsSummon())
			{
				pet->Disappear();
			}
		}

		CAPet* apetNext = m_pApetlist;
		CAPet* apet;
		while ((apet = apetNext))
		{
			if(!apet)
				break;

			apetNext = apet->m_pNextPet;
			if (apet->IsSummon())
			{
				apet->Disappear();
			}
		}

		// 소환수 제거
		while (m_elementalList)
			UnsummonElemental(m_elementalList);

		// 변신해제
		if (IsSetPlayerState(PLAYER_STATE_CHANGE) && m_changeIndex != -1)
			CancelChange();

		// 파티 처리
		// 접속 종료시에만 처리, 존이동 등은 GoZone()에서 처리함
		if (m_party == NULL && bDisconnect)
		{
			// 파티중이며, 존이동을 하다가 종료되었을 경우
			m_party = gserver->FindPartyByMemberIndex(m_index, false);
			if (m_party != NULL)
			{
				m_desc->m_bGoZone = false;
			}
		}

		if (m_party != NULL && bDisconnect)
		{
			// 초대하거나, 초대받은 사람일 경우 거절로 처리
			if (m_party->GetRequestIndex() > 0 && (m_party->GetBossIndex() == m_index || m_party->GetRequestIndex() == m_index))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PartyMsg(rmsg, MSG_PARTY_REJECT);
				do_Party(this, rmsg);
			}

			// 존이동이 아니면 파티 탈퇴로 처리
			if (!m_desc->m_bGoZone )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PartyMsg(rmsg, MSG_PARTY_OFFLINE);
				do_Party(this, rmsg);
			}

			// 자기 자신이 존재하므로 아무도 없으면 1명이다.
			// area가 disable되는 상황이면 파티의 areanum을 초기화 시킨다.
			if (pCurZone && (pCurZone->m_index == ZONE_ALTER_OF_DARK || pCurZone->m_index == ZONE_CAPPELLA_1)
					&& pCurArea && pCurArea->GetAllClientCount_Area() <= 1 )
			{
				if(IsParty())
					m_party->m_nRaidAreaNum = -1;
			}

			{
				//다른 파티원 로그아웃 알림(레벨:0)
				CNetMsg::SP rmsg(new CNetMsg);
				PartyInfoMsg(rmsg, this, false);
				m_party->SendToAllPC(rmsg, m_index);
			}
			
			m_party->updateCalcMember();

			// 존이동이면 파티 정보에서 포인터만 지운다
			m_party->SetMemberPCPtr(m_index, NULL);
			m_party = NULL;
		}

		// 원정대 처리
		// 접속 종료시에만 처리, 존이동 등은 GoZone()에서 처리함
		if (m_Exped == NULL && bDisconnect)
		{
			// 파티중이며, 존이동을 하다가 종료되었을 경우
			m_Exped = gserver->FindExpedByMemberIndex(m_index, false);
			if (m_Exped != NULL)
			{
				m_desc->m_bGoZone = false;
			}
		}
		if (IsExped() && bDisconnect)
		{
			if (m_Exped->GetRequestIndex() > 0 && (m_Exped->GetBossIndex() == m_index || m_Exped->GetRequestIndex() == m_index))
			{
				// 초대 거부
				CNetMsg::SP rmsg(new CNetMsg);
				ExpedRejectReqMsg(rmsg);
				do_Expedition(this, rmsg);
			}
			if (!m_desc->m_bGoZone)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExpendOfflineMsg(rmsg);
				do_Expedition(this, rmsg);
			}

			// 자기 자신이 존재하므로 아무도 없으면 1명이다.
			// area가 disable되는 상황이면 파티의 areanum을 초기화 시킨다.
			if (pCurZone && (pCurZone->m_index == ZONE_ALTER_OF_DARK || pCurZone->m_index == ZONE_CAPPELLA_1 || pCurZone->m_index == ZONE_AKAN_TEMPLE
							 || pCurZone->m_index == ZONE_TARIAN_DUNGEON
							)
					&& pCurArea && pCurArea->GetAllClientCount_Area() < 1)
			{
				if(IsExped())
				{
					m_Exped->m_nRaidAreaNum = -1;
					m_Exped->m_nDifficulty = -1;
				}
			}

			//다른 원정대원 로그아웃 알림(레벨:0)
			const CExpedMember* pMember = m_Exped->GetMemberByCharIndex(m_index);
			if(pMember)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExpedInfoMsg(rmsg, this, pMember->GetGroupType(), false);
				m_Exped->SendToAllPC(rmsg, m_index);
			}

			m_Exped->SendToExpedTargetClear(this);

			m_Exped->SetMemberPCPtr(m_index, NULL);
			m_Exped = NULL;			
		}

		if (bDisconnect && !m_desc->m_bGoZone)
		{
			// 파티 매칭에서 제거
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_EXTEND);
			RefMsg(rmsg) << MSG_EX_PARTY_MATCH
						 << MSG_EX_PARTY_MATCH_DEL_REQ;
			do_Extend(this, rmsg);
		}

		// 친구 처리
		if (m_Friend)
		{
			// 친구등록 요청중이거나 요청받고 있는주이었다면..
			if (m_nRegFriend != 0)
			{
				CPC* pRegFriendPC = PCManager::instance()->getPlayerByCharIndex(m_nRegFriend);

				if (pRegFriendPC)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::FriendRegistCancelMsg(rmsg);
					SEND_Q(rmsg, pRegFriendPC->m_desc);
					pRegFriendPC->m_nRegFriend = 0;
				}

				m_nRegFriend = 0;
			}

			if (bDisconnect)
			{
				m_nCondition = 0;

				CNetMsg::SP rmsg(new CNetMsg);
				HelperFriendSetConditionMsg(rmsg, m_index, m_nCondition, -1, this);
				SEND_Q(rmsg, gserver->m_helper);

				//delete m_Friend;
				//m_Friend = NULL;
			}
		}

		if (pCurZone && pCurArea)
		{
			// 길드룸에서 나가는거라면
			if (pCurZone->IsGuildRoom())
			{
				if (pCurArea->GetPCCount(0, 0, MAX(pCurArea->m_size[0], pCurArea->m_size[1])) < 1)
				{
					pCurArea->m_guildIndex = -1;
					pCurZone->SetDisableArea(pCurArea->m_index);
					pCurArea = NULL;
				}
			}

			// 퍼던이면
			else if (pCurZone->IsPersonalDungeon())
			{
				pCurZone->SetDisableArea(pCurArea->m_index);
				pCurArea = NULL;

				// 싱글전전 퀘스트를 수행중인 경우 존이동시
				// 퀘스트 취소 처리
				CQuest* pQuestNext = m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
				CQuest* pQuest;
				const CQuestProto* pQuestProto;
#if defined (QUEST_EXTEND_REPEAT_DAY)
				while ((pQuest = pQuestNext))
				{
					pQuestNext = m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);

					// 퀘스트를 하고 수행중이고 완료가 아니고
					if (!pQuest->IsCompleted())
					{
						pQuestProto = pQuest->GetQuestProto();
						if (pQuestProto->m_type[1] == QTYPE_REPEAT_DAY)
						{
							m_questList.SetQuestState(pQuest, QUEST_STATE_DONE);
							tm tmNext = NOW();
							tmNext.tm_mday = tmNext.tm_mday + 1;
							tmNext.tm_hour = 0;
							tmNext.tm_min = 0;
							tmNext.tm_sec = 0;
							tmNext.tm_isdst = -1;
							pQuest->SetCompleteTime(mktime(&tmNext));
						}
					}
				}
				pQuestNext = m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
#endif // QUEST_EXTEND_REPEAT_DAY
				while ((pQuest = pQuestNext))
				{
					pQuestNext = m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);

					// 퀘스트를 하고 수행중이고 완료가 아니고
					if (!pQuest->IsCompleted())
					{
						pQuestProto = pQuest->GetQuestProto();
						// 발록 격파 퀘스트
						if (pQuestProto->m_type[0] == QTYPE_KIND_DEFEAT)
						{
							// Quest Fail Log
							GAMELOG << init("QUEST FAIL", this)
									<< pQuestProto->m_index
									<< end;

							{
								// 없으면 바로 실패
								CNetMsg::SP rmsg(new CNetMsg);
								QuestFailMsg(rmsg, pQuest);
								if (!bDisconnect) SEND_Q(rmsg, m_desc);
							}

							m_questList.DelQuest(this, pQuest);
							goto EXIT_PROC_PD;
						}
						// 공주 구출 퀘스트
						else if (pQuestProto->m_type[0] == QTYPE_KIND_SAVE)
						{
							// 아이템 삭제
							pQuest->RemoeQuestItem(this);

							// Quest Fail Log
							GAMELOG << init("QUEST FAIL",  this)
									<< pQuestProto->m_index
									<< end;

							{
								CNetMsg::SP rmsg(new CNetMsg);
								QuestFailMsg(rmsg, pQuest);
								if (!bDisconnect) SEND_Q(rmsg, m_desc);
							}

							m_questList.DelQuest(this, pQuest);
							goto EXIT_PROC_PD;
						}
						else
							continue;
					}
				} // while ((pQuest = pQuestNext))

			} // 퍼던이면
EXIT_PROC_PD:
			;
		}

		if((bDisconnect == true || bGoto == false) && pCurZone && pCurZone->IsWarGroundZone())
		{
			CWaitPlayer* p = NULL;
			p = gserver->m_RoyalRumble.m_WaitPlayerList.GetNode(this->m_index);
			if(p)
			{
				int leveltype = p->GetLevelType();
				int leftcount = 0;
				CWaitPlayer* player = NULL;
				CWaitPlayer* playern = NULL;
				playern = gserver->m_RoyalRumble.m_WaitPlayerList.GetHead();
				while((player = playern))
				{
					playern = playern->GetNext();
					if( player->GetLevelType() == leveltype &&
							player->GetCheckIn() == true )
						leftcount++;
				}
				leftcount -= 2;
				CNetMsg::SP rmsg(new CNetMsg);
				RoyalRumbleLeftCount(rmsg, leftcount);
				CNetMsg::SP killmsg(new CNetMsg);
				RoyalRumbleKillPlayer(killmsg, this, this);
				switch(leveltype)
				{
				case LEVEL_TYPE_ROOKIE:
					{
						gserver->m_RoyalRumble.m_pRookieArea->SendToAllClient(rmsg);
						gserver->m_RoyalRumble.m_pRookieArea->SendToAllClient(killmsg);
					}
					break;
				case LEVEL_TYPE_SENIOR:
					{
						gserver->m_RoyalRumble.m_pSeniorArea->SendToAllClient(rmsg);
						gserver->m_RoyalRumble.m_pSeniorArea->SendToAllClient(killmsg);
					}
					break;
				case LEVEL_TYPE_MASTER:
					{
						gserver->m_RoyalRumble.m_pMasterArea->SendToAllClient(rmsg);
						gserver->m_RoyalRumble.m_pMasterArea->SendToAllClient(killmsg);
					}
					break;
				default:
					break;
				}
			}
			gserver->m_RoyalRumble.m_WaitPlayerList.DelNode(this->m_index);
		}
	} // 다른 존으로 이동하거나 접속 종료시에만 처리되는 루틴

	// Attack List 초기화
	DelAttackList(this);

	// 자신이 사라짐을 알림
	if (pCurArea && IS_IN_CELL(this))
	{
		if (bDisconnect)
			SendDisappearAllInCell(false);
		else
			SendDisappearAllInCell(true);

		// 셀에서 제거
		pCurArea->CharFromCell(this, true);
		if ( !bGoto && pCurZone->IsFieldZone() && pCurArea->GetAllClientCount_Area() < 1 )
		{
			pCurArea->SetFreeze(gserver->m_pulse);
		}
		// 연결이 끊기거나 다른 존으로 이동할 때 콤보존 처리
		if(!bGoto && pCurZone->IsComboZone())
		{
			// 콤보던전에 pc가 전혀 없으면 콤보존 해체
			if(pCurArea->GetPCCount(0, 0, 1) < 1)
			{
				GAMELOG << init("CLOSE COMBO AREA", this)
						<< "AREA INDEX: " << pCurArea->m_index
						<< "COMBO INDEX: " << pCurArea->m_monsterCombo->m_nIndex
						<< end;

				// 콤보던전 해체될 때 파티가 있으면
#if defined (EXTREME_CUBE)
				if(pParty)
					pParty->m_comboAreaIndex = -1;
#endif

				// 존에 있는 콤보정보 해제
				pCurArea->m_monsterCombo->InitAllVar();

				// 방문 닫음
				pCurZone->SetDisableArea(pCurArea->m_index);
				pCurArea = NULL;
			}
		}
		// 레이드에서 나가는 거라면
		if(pCurZone && !bGoto)
		{
			if (pCurZone->m_index == ZONE_AKAN_TEMPLE)
			{
				for(int loop = 0; loop < 8; loop++)
				{
					if((pCurArea->m_AkanPCList[loop] != NULL) && (pCurArea->m_AkanPCList[loop]->m_index == this->m_index))
					{
						pCurArea->m_AkanPCList[loop] = NULL;
					}
				}
				int nSavePoint = pCurArea->m_RaidInfoList[pCurArea->m_RaidDifficulty].GetSavePoint();
				if(nSavePoint == 1)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					RaidSceneMsg(rmsg, OBJECT_TYPE_TODO, TOUCH_FIELD, 2673, -1);
					do_ExRaidScene(this, rmsg);
				}
			}
			if (pCurZone->m_index == ZONE_TARIAN_DUNGEON)
			{
				for(int loop = 0; loop < 8; loop++)
				{
					if((pCurArea->m_AkanPCList[loop] != NULL) && (pCurArea->m_AkanPCList[loop]->m_index == this->m_index))
					{
						pCurArea->m_AkanPCList[loop] = NULL;
					}
				}
			}
			if (pCurZone->m_index == ZONE_ALTER_OF_DARK || pCurZone->m_index == ZONE_CAPPELLA_1 || pCurZone->m_index == ZONE_AKAN_TEMPLE
					|| pCurZone->m_index == ZONE_TARIAN_DUNGEON
			   )
			{
				if (pCurArea->GetAllClientCount_Area() < 1 && pCurArea->m_inCount == 0)
				{
					pCurZone->SetDisableArea(pCurArea->m_index);
					pCurArea = NULL;
				}
			}
			if (pCurZone->m_index == ZONE_DUNGEON4)
			{
				if(pCurArea->GetAllClientCount_Area() < 1)
				{
					pCurZone->SetDisableArea(pCurArea->m_index);
					pCurArea = NULL;
				}
			}
		}
	}
//#ifdef MONSTER_COMBO_BUGFIX
//	else
//	{
//		GAMELOG << init("ProcDisconnect Error", this)
//				<< this->m_cellX << delim;
//
//		if(pCurArea)
//			GAMELOG << "1" << end;
//		else
//			GAMELOG << "0" << end;
//	}
//#endif // MONSTER_COMBO_BUGFIX

#ifdef EXTREME_CUBE
	// CCMemList 에서 PC 삭제
	if(pCurZone != NULL && !bGoto && pCurZone->IsExtremeCube())
	{
		CCubeMemList* memlist = NULL;
		if(gserver->m_extremeCube.IsGuildCubeTime())
		{
			if(pGuildInfo && pGuildInfo->guild())
			{
				memlist = gserver->m_extremeCube.FindMemList(pGuildInfo->guild());
			}
		}
		else
		{
			if(pParty)
			{
				memlist = gserver->m_extremeCube.FindMemList(pParty);
			}
			else
			{
				memlist = NULL;
			}
		}

		if(memlist)
		{
			memlist->DelPC(this);
			GAMELOG << init("CUBE MEMBER DELETE", this)
					<< end;
		}
		else
		{
			GAMELOG << init("CUBE MEMBER DELETE FAIL", this)
					<< end;
		}
	}
#endif // EXTREME_CUBE

	gserver->discCharChatGroup(this->GetName() );

	// 다른 존으로 이동하거나 접속 종료시에만 처리되는 루틴
	if (!bGoto)
	{
		// 레이어 존에 있을 경우 스타트 존으로 변경, 존이동이 아닐때에만
		if (pCurZone && bDisconnect)
		{
			if (!m_desc->m_bGoZone && pCurZone->m_countArea > 1)
			{
				// 스타트존 지점
				m_pZone				= gserver->FindZone(ZONE_START);
				m_pArea				= NULL;
				GET_YLAYER(this)	= m_pZone->m_zonePos[0][0];
				GET_R(this)			= 0.0f;
				GET_X(this)			= GetRandom(m_pZone->m_zonePos[0][1], m_pZone->m_zonePos[0][3]) / 2.0f;
				GET_Z(this)			= GetRandom(m_pZone->m_zonePos[0][2], m_pZone->m_zonePos[0][4]) / 2.0f;
			}
		}

		// 접속 종료시만 처리
		if( bDisconnect )
		{
			m_assist.CureAssist(SF_LOGOUT);
			m_activeSkillList.DeleteInstantSkill(SF_INSTANT);
			m_passiveSkillList.DeleteInstantSkill(SF_INSTANT);
			m_etcSkillList.DeleteInstantSkill(SF_INSTANT);
		}
	} // 다른 존으로 이동하거나 접속 종료시에만 처리되는 루틴
}

void CPC::OutputDBItemLog()
{
	{
		std::vector<AUTO_GIVE_DATA>::iterator it = m_listAutoGive.begin();
		std::vector<AUTO_GIVE_DATA>::iterator endit = m_listAutoGive.end();
		for (; it != endit; ++it)
		{
			AUTO_GIVE_DATA& agdItem = *(it);
			if (m_bLoadChar)
				GAMELOG << init("AUTO GIVE ITEM", this);
			else
				GAMELOG << init("AUTO GIVE ITEM FAIL", this);
			GAMELOG << "ITEM" << delim
					<< "INDEX" << delim
					<< agdItem.nIndex << delim
					<< "PLUS" << delim
					<< agdItem.nPlus << delim
					<< "USED" << delim
					<< agdItem.nUsed << delim
					<< "FLAG" << delim
					<< agdItem.nFlag << delim
					<< "COUNT" << delim
					<< agdItem.nCount << delim
					<< "OPTION" << delim
					<< ((agdItem.nOption[0] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
					<< (agdItem.nOption[0] & 0x00ff) << delim
					<< ((agdItem.nOption[1] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
					<< (agdItem.nOption[1] & 0x00ff) << delim
					<< ((agdItem.nOption[2] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
					<< (agdItem.nOption[2] & 0x00ff) << delim
					<< ((agdItem.nOption[3] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
					<< (agdItem.nOption[3] & 0x00ff) << delim
					<< ((agdItem.nOption[4] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
					<< (agdItem.nOption[4] & 0x00ff) << delim
					<< "SERIAL" << delim
					<< agdItem.strSerial
					<< end;
		}
		m_listAutoGive.clear();
	}

	{
		std::vector<BLOOD_BUG_DATA>::iterator it = m_listBloodBug.begin();
		std::vector<BLOOD_BUG_DATA>::iterator endit = m_listBloodBug.end();
		for (; it != endit; ++it)
		{
			BLOOD_BUG_DATA& bugdata = *(it);
			if (m_bLoadChar)
				GAMELOG << init("UNIQUE OPTION BUG FIX", this);
			else
				GAMELOG << init("UNIQUE OPTION BUG FIX FAIL", this);
			GAMELOG << "ITEM" << delim
					<< "INDEX" << delim
					<< bugdata.nIndex << delim
					<< "FLAG" << delim
					<< bugdata.nFlag << delim
					<< "OPTION" << delim
					<< bugdata.nOptionCount << delim
					<< ((bugdata.nOption[0] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
					<< (bugdata.nOption[0] & 0x00ff) << delim
					<< ((bugdata.nOption[1] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
					<< (bugdata.nOption[1] & 0x00ff) << delim
					<< ((bugdata.nOption[2] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
					<< (bugdata.nOption[2] & 0x00ff) << delim
					<< ((bugdata.nOption[3] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
					<< (bugdata.nOption[3] & 0x00ff) << delim
					<< ((bugdata.nOption[4] >> OPTION_VALUE_SHIFT) & 0x00ff) << delim
					<< (bugdata.nOption[4] & 0x00ff) << delim
					<< "SERIAL" << delim
					<< bugdata.strSerial
					<< end;
		}
		m_listBloodBug.clear();
	}
}

void CPC::ChangeName(const char* name)
{
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ServerToServerPacket::makeChangeNickName(rmsg, this->m_nick, name);
		SEND_Q(rmsg, gserver->m_helper);
		SEND_Q(rmsg, gserver->m_subHelper);
	}

	PCManager::instance()->changeNickName(this->m_nick, name);

	this->m_nick = name;

	int count = m_Friend->GetFriendCount();

	{
		CNetMsg::SP rmsg(new CNetMsg);
		NameChangeRepMsg(rmsg, MSG_EX_NAMECHANGE_ERROR_SUCCESS, name, 0);
		SEND_Q(rmsg, m_desc);
	}

	for(int i = 0; m_Friend != NULL && i < count; i++)
	{
		CFriendMember* m_fmember = m_Friend->GetFriendMember(i);

		if( m_fmember == NULL ) continue;

		CPC* fpc = PCManager::instance()->getPlayerByCharIndex(m_fmember->GetChaIndex());
		if( fpc)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			NameChangeFriendMsg(rmsg, m_index, GetName(), m_job );
			SEND_Q(rmsg, fpc->m_desc);
		}
	}

	// 길드원 전체에게 메세지 보냄
	if( m_guildInfo )
	{
		//member nick부분 삭제 : bw 060403
		//pc->m_guildInfo->nick(pc->GetName() );
		m_guildInfo->name(GetName() );

		CGuild* guild = m_guildInfo->guild();
		if( guild )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			NameChangeGuildMemberMsg(rmsg, m_index, GetName(), m_job);
			guild->SendToAll(rmsg);
		}
	}

	{
		// 자신의 주위의 모든 케릭터에게 어피어 메세지를 보낸다.
		CNetMsg::SP rmsg(new CNetMsg);
		DisappearMsg(rmsg, this);
		m_pArea->SendToCell(rmsg, this);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		AppearMsg(rmsg, this, true);
		m_pArea->SendToCell(rmsg, this);
	}

	if( m_guildInfo && m_guildInfo->guild() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildPointRankingMsg( rmsg, m_index, m_guildInfo->guild()->index(), m_guildInfo->guild()->GetGuildPointRanking() );
		m_pArea->SendToCell( rmsg, this );
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildNameColorStateMsg(rmsg, this );
		m_pArea->SendToCell( rmsg, this );
	}
}

bool CPC::AddBlockPC(int charIndex, const char* name)
{
	if( charIndex < 0 || strcmp(name, "" ) == 0 )
		return false;

	if( this->IsBlockPC(charIndex, name) )
		return false;

	CBlockPC* temp = new CBlockPC(charIndex, name);
	bool flag = m_listBlockPC.insert(map_listblock_t::value_type(charIndex, temp)).second;
	if (flag == false)
	{
		delete temp;
	}
	return flag;
}

void CPC::ReleaseBlockPC(int charIndex, CLCString& name)
{
	map_listblock_t::iterator it = m_listBlockPC.find(charIndex);
	if (it != m_listBlockPC.end())
	{
		CBlockPC* temp = it->second;
		name = temp->GetName();

		delete temp;
		m_listBlockPC.erase(it);
	}
}

bool CPC::IsBlockPC(int charIndex, const char* name)
{
	map_listblock_t::iterator it = m_listBlockPC.find(charIndex);
	if (it != m_listBlockPC.end())
	{
		CBlockPC* temp = it->second;
		temp->SetName(name);
		return true;
	}

	return false;
}

void CPC::GetBlockListString(CLCString& blockIndexList, CLCString& blockNameList)
{
	map_listblock_t::iterator it = m_listBlockPC.begin();
	map_listblock_t::iterator endit = m_listBlockPC.end();

	char tmpBuf[MAX_STRING_LENGTH] = {0,};
	for(; it != endit; ++it)
	{
		CBlockPC* bPc = it->second;
		sprintf(tmpBuf, "%d", bPc->GetIndex() );
		blockIndexList += tmpBuf;
		blockIndexList += " ";
		blockNameList += bPc->GetName();
		blockNameList += " ";
	}
}

float CPC::GetHitrate(CCharacter* df, MSG_DAMAGE_TYPE type)
{
	extern int nBaseHitrate[JOBCOUNT][3];
	extern int levelup_stat[JOBCOUNT][4];
	extern int levelup_stat2[JOB2COUNT][4];

	const float nHitratePerDex = 1.0f;
	const float nHitratePerInt = 1.0f;

	float fHitrate	= nBaseHitrate[m_job][type] + (m_level - df->m_level);

	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
		if (m_job2 < 1)
			fHitrate += nHitratePerDex * (levelup_stat[m_job][1] * (m_level - 1));
		else
		{
			fHitrate += nHitratePerDex * (levelup_stat2[m_job * JOB_2ND_SPLIT_INDEX + m_job2 -1 ][1] * (m_level - JOB_2ND_LEVEL));
			fHitrate += nHitratePerDex * (levelup_stat[m_job][1] * (JOB_2ND_LEVEL - 1));
		}
		break;
	case MSG_DAMAGE_MAGIC:
		if (m_job2 < 1)
			fHitrate += nHitratePerInt * (levelup_stat[m_job][2] * (m_level - 1));
		else
		{
			fHitrate += nHitratePerInt * (levelup_stat2[m_job * JOB_2ND_SPLIT_INDEX + m_job2 -1 ][2] * (m_level - JOB_2ND_LEVEL));

			fHitrate += nHitratePerInt * (levelup_stat[m_job][2] * (JOB_2ND_LEVEL - 1));
		}
		break;
	default:
		break;
	}

	if( fHitrate < 0 )
		fHitrate = 0;

	return fHitrate;
}

float CPC::GetAvoid(CCharacter* of, MSG_DAMAGE_TYPE type)
{
	extern int nBaseAvoid[JOBCOUNT][3];
	extern int levelup_stat[JOBCOUNT][4];
	extern int levelup_stat2[JOB2COUNT][4];

	const float nAvoidPerDex = 1.0f;
	const float nAvoidPerInt = 1.0f;

	float fAvoid	= nBaseAvoid[m_job][type] + ((m_level - of->m_level) * 10 ) ;

	switch (type)
	{
	case MSG_DAMAGE_MELEE:
	case MSG_DAMAGE_RANGE:
		if (m_job2 < 1)
			fAvoid += nAvoidPerDex * (levelup_stat[m_job][1] * (m_level - 1));
		else
		{
			fAvoid += nAvoidPerDex * (levelup_stat2[m_job * JOB_2ND_SPLIT_INDEX + m_job2 - 1][1] * (m_level - JOB_2ND_LEVEL));
			fAvoid += nAvoidPerDex * (levelup_stat[m_job][1] * (JOB_2ND_LEVEL - 1));
		}
		break;
	case MSG_DAMAGE_MAGIC:
		if (m_job2 < 1)
			fAvoid += nAvoidPerInt * (levelup_stat[m_job][2] * (m_level - 1));
		else
		{
			fAvoid += nAvoidPerInt * (levelup_stat2[m_job * JOB_2ND_SPLIT_INDEX + m_job2 - 1][2] * (m_level - JOB_2ND_LEVEL));
			fAvoid += nAvoidPerInt * (levelup_stat[m_job][2] * (JOB_2ND_LEVEL - 1));
		}
		break;
	default:
		break;
	}

	if( fAvoid < 0 )
		fAvoid = 0;

	return fAvoid;
}

void CPC::UpdateAPetValue()
{
	CAPet* p = m_pApetlist;
	while (p)
	{
		// 설정치 업데이트
		bool bUpdate = false;
		if( p->DecreaseStat() )
			bUpdate = true;

		if( p->IncreaseStat() )
			bUpdate = true;

		if( p->IsSummon() && p->TransFormationCheck() )
			bUpdate = true;

		if( bUpdate && p->m_hp > 0 && this->m_wearInventory.wearItemInfo[WEARING_PET])
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExAPetStatusMsg(rmsg, p);
			m_pArea->SendToCell(rmsg, this, true);
			SEND_Q(rmsg, m_desc);
		}

		p->CheckItemTime();

		p->m_assist.DecreaseTime();

		p = p->m_pNextPet;
	}
}

CAPet* CPC::GetAPet(int index)
{
	if (index == 0)
	{
		if (m_wearInventory.wearItemInfo[WEARING_PET])
		{
			index = m_wearInventory.wearItemInfo[WEARING_PET]->getPlus();
		}
		else
			return NULL;
	}

	CAPet* p = m_pApetlist;
	while (p)
	{
		if (p->m_index == index)
			return p;

		p = p->m_pNextPet;
	}

	return NULL;
}

void CPC::DelAPet(int petIndex)
{
	CAPet* apet = GetAPet(petIndex);

	if(!apet)	// 삭제할 펫 없음
		return;

	// 아이템을 입고있으면 불가
	for( int i=0; i<APET_WEARPOINT; ++i )
	{
		if( apet->m_wearing[i] )
			return;
	}

	// 착용한 아이템은 삭제 불가
	if(apet->IsSummon())
	{
		return;
	}

	CItem* item = this->m_inventory.FindByDBIndex(apet->m_pProto->m_nItemIndex, petIndex);
	if (item)
	{
		this->m_inventory.decreaseItemCount(item, 1);
	}

	// petIndex Helper로 보내서 disable
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperAttackPetMsg( rmsg , MSG_HELPER_APET_DISABLE_REQ, m_index );
		RefMsg(rmsg) << petIndex;
		SEND_Q(rmsg, gserver->m_helper);
	}

	GAMELOG << init("APET DELETE", m_name, m_desc->m_idname)
			<< "INDEX" << delim << apet->m_index << delim
			<< "TYPE" << delim
			<< apet->m_name << delim
			<< "LEVEL" << delim
			<< apet->m_level << delim
			<< "EXP" << delim
			<< apet->m_exp
			<< end;

	REMOVE_FROM_BILIST(apet, m_pApetlist, m_pPrevPet, m_pNextPet);
	DelAttackList(apet);
	delete apet;
	apet = NULL;
}

void CPC::ApplyOriginOption(CItem * pItem)
{
	// 0 : 귀속 설정, 1~6 : 옵션, 7~9 : 스킬
	int i = 0;
	for (i = 0; i < MAX_ORIGIN_OPTION; i++)
	{
		int nIndex = pItem->GetOriginIndex(i);
		if (nIndex < 0)
			continue;

		COptionProto* pOptionProto = gserver->m_optionProtoList.FindProto(nIndex);
		if (pOptionProto)
		{
			int nValue = 0;
			int nLevel = pItem->GetOriginLevel(i);
			if( nLevel > 0 && (int)pOptionProto->m_levelValue.size() >= nLevel )
			{
				if ( pOptionProto->m_levelValue[nLevel- 1])
					nValue = pOptionProto->m_levelValue[nLevel - 1];
			}

			COption::ApplyOptionValue(this, pOptionProto->m_type, nValue, pItem, i);
		}
	}

	for (i = MAX_ORIGIN_OPTION; i < MAX_ORIGIN; i++)
	{
		int nIndex = pItem->GetOriginIndex(i);
		if (nIndex <= 0)
			continue;

		CSkill * pSkill = gserver->m_skillProtoList.Create(nIndex, pItem->GetOriginLevel(i));
		if(pSkill)
		{
			if (pSkill->m_proto->IsActive())
			{
				{
					CPC::map_t::iterator it = this->itemSkill.find(pSkill->m_proto->m_index);
					if(it != this->itemSkill.end())
					{
						pSkill->m_usetime = it->second;
					}
				}
				m_activeSkillList.Add(pSkill);
			}
			else if (pSkill->m_proto->IsPassive())
			{
				m_passiveSkillList.Add(pSkill);
			}
			else
			{
				m_etcSkillList.Add(pSkill);
			}
		}
	}
}

void CPC::ApplySetItemValue(CItem * pItem)
{
	if (!pItem->m_pSetItemProto || pItem->m_nSetWearCnt <= 0)
		return;

	int i = 0;
	int optionCount = pItem->m_pSetItemProto->GetOptionCnt();
	for (i = 0; i < optionCount; i++)
	{
		const SETITEMOPTION* pSetOption = pItem->m_pSetItemProto->GetOption(i);

		if (!pSetOption)
			continue;

		if (pItem->m_nSetWearCnt < pSetOption->nWearCnt)
			continue;

		if (pSetOption->nType == SET_ITEM_OPTION)
		{
			COptionProto* pOptionProto = gserver->m_optionProtoList.FindProto(pSetOption->nIndex);
			if (pOptionProto)
			{
				int nValue = 0;
				int nLevel = pSetOption->nLevel;
				if (pOptionProto->m_levelValue[nLevel - 1])
					nValue = pOptionProto->m_levelValue[nLevel -1];

				COption::ApplyOptionValue(this, pOptionProto->m_type, nValue, pItem);
			}

			pItem->m_nSetOption = 1;
		}
		else if (pSetOption->nType == SET_ITEM_SKILL)
		{
			CSkill * pSkill = gserver->m_skillProtoList.Create(pSetOption->nIndex, pSetOption->nLevel);
			if(pSkill)
			{
				if (pSkill->m_proto->IsActive())
				{
					CPC::map_t::iterator it = this->itemSkill.find(pSkill->m_proto->m_index);
					if(it != this->itemSkill.end())
					{
						pSkill->m_usetime = it->second;
					}

					m_activeSkillList.Add(pSkill);
				}
				else if (pSkill->m_proto->IsPassive())
				{
					m_passiveSkillList.Add(pSkill);
				}
				else
				{
					m_etcSkillList.Add(pSkill);
				}
			}

			pItem->m_nSetOption = 2;
		}
	}

	if (pItem->m_nSetOption)
	{
		for (i = 0; i < MAX_WEARING; i++)
		{
			if (!m_wearInventory.wearItemInfo[i])
				continue;

			CItem* temp = m_wearInventory.wearItemInfo[i];
			if((temp->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (temp->getFlag() & FLAG_ITEM_COMPOSITION))
				temp = this->m_inventory.FindByVirtualIndex(temp->m_nCompositeItem);

			if (!temp || !temp->m_pSetItemProto)
				continue;

			if (pItem->m_itemProto->GetSetIndex() == temp->m_itemProto->GetSetIndex())
			{
				temp->m_nSetOption = pItem->m_nSetOption;
			}
		}
	}
}

void CPC::RemoveSetItemValue(CItem * pItem)
{
	if (!pItem->m_pSetItemProto || pItem->m_nSetWearCnt <= 0)
		return;

	int optionCount = pItem->m_pSetItemProto->GetOptionCnt();

	for (int i = 0; i < optionCount; i++)
	{
		const SETITEMOPTION* pSetOption = pItem->m_pSetItemProto->GetOption(i);

		if (!pSetOption)
			continue;

		if (pItem->m_nSetWearCnt <= pSetOption->nWearCnt && pSetOption->nType == SET_ITEM_SKILL)
		{
			CSkill * pSkill = gserver->m_skillProtoList.Create(pSetOption->nIndex, pSetOption->nLevel);
			if(pSkill)
			{
				if (pSkill->m_proto->IsActive())
				{
					//아이템 스킬 쿨 타임을 저장
					CSkill* skill = this->m_activeSkillList.Find(pSkill->m_proto->m_index);

					if(skill != NULL)
					{
						CPC::map_t::iterator it = this->itemSkill.find(pSkill->m_proto->m_index);						
						if(it != this->itemSkill.end())
						{
							it->second = skill->m_usetime;
						}
						else
						{
							this->itemSkill.insert(std::pair<int, int>(pSkill->m_proto->m_index, skill->m_usetime));
						}
					}						

					CNetMsg::SP rmsg(new CNetMsg);
					pSkill->Cancel(this);
					ResponseClient::makeSkillCancelMsg(rmsg, this);
					m_pArea->SendToCell(rmsg, this, true);

					RemoveSkillFromQuickSlot(pSkill);
					m_activeSkillList.Remove(pSkill);
				}
				else if (pSkill->m_proto->IsPassive())
				{
					m_passiveSkillList.Remove(pSkill);
				}
				else
				{
					m_etcSkillList.Remove(pSkill);
				}
				GAMELOG << init("ITEM SKILL DELETE", this) << delim << "SKILL INDEX" << delim
						<< pSkill->m_proto->m_index << itemlog(pItem) << end;
			}
		}
	}
}

void CPC::CountSetItem(CItem* pItem, bool bPlus /* = true */)
{
	CItem * pSameItem = NULL;

	for (int i = 0; i < MAX_WEARING; i++)
	{
		if (!m_wearInventory.wearItemInfo[i])
			continue;
		CItem *tempItem = m_wearInventory.wearItemInfo[i];
		if((tempItem->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE) && (tempItem->getFlag() & FLAG_ITEM_COMPOSITION))
		{
			tempItem = this->m_inventory.FindByVirtualIndex(tempItem->m_nCompositeItem);
		}

		if (!tempItem)
			continue;

		if (pItem->getDBIndex() == tempItem->getDBIndex() && bPlus == true)
			continue;

		if (pItem->m_itemProto->GetSetIndex() == tempItem->m_itemProto->GetSetIndex())
		{
			if (bPlus)
			{
				tempItem->m_nSetWearCnt += 1;
				if (!pSameItem)
					pSameItem = tempItem;
			}
			else
			{
				tempItem->m_nSetWearCnt -= 1;
			}
		}
	}

	if (bPlus)
	{
		if (pSameItem)
		{
			pItem->m_nSetWearCnt = pSameItem->m_nSetWearCnt;
		}
		else
		{
			pItem->m_nSetWearCnt = 1;
		}
	}
}

void CPC::SetSummonNpc(SUMMON_NPC_TYPE summonType, CNPC* pNPC)
{
	m_pSummnonNpc[summonType] = pNPC;
}

CNPC* CPC::GetSummonNpc(int index, int idNum)
{
	int i=0;
	for(i=0; i<SUMMON_NPC_MAX; i++)
	{
		if( m_pSummnonNpc[i] && m_pSummnonNpc[i]->m_index == index && m_pSummnonNpc[i]->m_idNum == idNum)
			return m_pSummnonNpc[i] ;
	}
	return NULL;
}

CNPC* CPC::GetSummonNpc(SUMMON_NPC_TYPE summonType)
{
	if (summonType >= SUMMON_NPC_MAX)
		return NULL;
	if( m_pSummnonNpc[summonType] )
		return m_pSummnonNpc[summonType];

	return NULL;
}

CNPC* CPC::GetSummonNpc(CNPC* pNpc)
{
	int i=0;
	for(i=0; i<SUMMON_NPC_MAX; i++)
	{
		if( m_pSummnonNpc[i] && m_pSummnonNpc[i] == pNpc)
			return m_pSummnonNpc[i] ;
	}
	return NULL;
}

void CPC::SummonNpcRemove(CNPC* pNpc, bool bAutoDelete)
{
	int i=0;
	for(i=0; i<SUMMON_NPC_MAX; i++)
	{
		if( m_pSummnonNpc[i] && m_pSummnonNpc[i]== pNpc )
		{
			GAMELOG << init("SUMMON NPC REMOVE")	<< delim
					<< "Owner Info"	<< delim
					<< GetName()	<< delim
					<< m_index	<< delim
					<< end;
			
			if(m_pSummnonNpc[i]->m_proto && m_pSummnonNpc[i]->m_proto->CheckFlag1(NPC1_TOTEM_ITEM))
			{
				//토템일 경우에는 클라이언트 이펙트 출력을 위해서 디스어피어 패킷을 보내지 않는다.
				if(m_pSummnonNpc[i]->GetOwner() != NULL)
					m_pSummnonNpc[i]->SendRemoveAllInCellForClientEffect(m_pSummnonNpc[i]->GetOwner());
			}
			else
			{
				m_pSummnonNpc[i]->SendDisappearAllInCell(true);
			}

			m_pSummnonNpc[i]->SetOwner( NULL);

			if( bAutoDelete )
			{
				m_pSummnonNpc[i]->m_hp = 0;
				if (m_pSummnonNpc[i]->m_pArea)
					m_pSummnonNpc[i]->m_pArea->m_nNPC_Killed_NPC ++;
			}
			m_pSummnonNpc[i] = NULL;

			if( i == SUMMON_NPC_TYPE_MERCENARY )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				MonsterMercenaryErrorMsg(rmsg, MSG_EX_MERCENARY_ERROR_MON_AUTO_DEL);
				SEND_Q(rmsg, this->m_desc );

				m_SummonOwners_target = NULL;
			}

			switch(i)
			{
			case SUMMON_NPC_TYPE_BOMB_1:
			case SUMMON_NPC_TYPE_BOMB_2:
			case SUMMON_NPC_TYPE_BOMB_3:
				m_nBombSummonCnt --;
			}
		}
	}
}

void CPC::SummonNpcRemove(SUMMON_NPC_TYPE summonType, bool bAutoDelete)
{
	if( m_pSummnonNpc[summonType] )
	{
		GAMELOG << init("SUMMON NPC REMOVE")	<< delim
				<< "Owner Info"	<< delim
				<< GetName()	<< delim
				<< m_index << delim
				<< end;
		
		if(m_pSummnonNpc[summonType]->m_proto && m_pSummnonNpc[summonType]->m_proto->CheckFlag1(NPC1_TOTEM_ITEM))
		{
			//토템일 경우에는 클라이언트 이펙트 출력을 위해서 디스어피어 패킷을 보내지 않는다.
			if(m_pSummnonNpc[summonType]->GetOwner() != NULL)
				m_pSummnonNpc[summonType]->SendRemoveAllInCellForClientEffect(m_pSummnonNpc[summonType]->GetOwner());
		}
		else
		{
			m_pSummnonNpc[summonType]->SendDisappearAllInCell(true);
		}

		m_pSummnonNpc[summonType]->SetOwner( NULL);
		
		if( bAutoDelete )
		{
			m_pSummnonNpc[summonType]->m_hp = 0;
			if (m_pSummnonNpc[summonType]->m_pArea)
				m_pSummnonNpc[summonType]->m_pArea->m_nNPC_Killed_NPC ++;
		}
		m_pSummnonNpc[summonType] = NULL;

		if( summonType == SUMMON_NPC_TYPE_MERCENARY )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MonsterMercenaryErrorMsg(rmsg, MSG_EX_MERCENARY_ERROR_MON_AUTO_DEL);
			SEND_Q(rmsg, this->m_desc );

			m_SummonOwners_target = NULL;
		}

		switch(summonType)
		{
		case SUMMON_NPC_TYPE_BOMB_1:
		case SUMMON_NPC_TYPE_BOMB_2:
		case SUMMON_NPC_TYPE_BOMB_3:
			m_nBombSummonCnt --;
			break;
		default:
			break;
		}
	}
}

void CPC::SummonNpcRemoveAll(bool bAutoDelete)
{
	int i=0;
	for(i=0; i<SUMMON_NPC_MAX; i++)
	{
		CNPC * summonNpc = m_pSummnonNpc[i];
		if( summonNpc )
		{
			GAMELOG << init("SUMMON NPC REMOVE")	<< delim
					<< "Owner Info"	<< delim
					<< GetName()	<< delim
					<< m_index	<< delim
					<< end;
			if(summonNpc->m_proto && summonNpc->m_proto->CheckFlag1(NPC1_TOTEM_ITEM))
			{
				if(summonNpc->GetOwner() != NULL)
				{
					summonNpc->SendRemoveAllInCellForClientEffect(summonNpc->GetOwner());
					//주인에게 사라짐을 알려준다.
					CNetMsg::SP rmsg(new CNetMsg);
					DisappearMsgForClientEffect(rmsg, summonNpc, summonNpc->GetOwner()->m_index);
					SEND_Q(rmsg, summonNpc->GetOwner()->m_desc);
				}
			}
			else
			{
				summonNpc->SendDisappearAllInCell(true);
			}
			
			summonNpc->SetOwner( NULL);
			if( bAutoDelete )
			{
				summonNpc->m_hp = 0;
				if (summonNpc->m_pArea)
					summonNpc->m_pArea->m_nNPC_Killed_NPC ++;
			}
			if( i == SUMMON_NPC_TYPE_MERCENARY )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				MonsterMercenaryErrorMsg(rmsg, MSG_EX_MERCENARY_ERROR_MON_AUTO_DEL);
				SEND_Q(rmsg, this->m_desc );
			}
			
			m_pSummnonNpc[i] = NULL;
		}
	}
	m_SummonOwners_target = NULL;
	m_nBombSummonCnt = 0;
}

int CPC::GetSummonNpcType(CNPC* pNpc)
{
	int i=0;
	for(i=0; i<SUMMON_NPC_MAX; i++)
	{
		if( m_pSummnonNpc[i] && m_pSummnonNpc[i] == pNpc)
			return i;
	}
	return -1;
}

bool CPC::SetBombSummonNPC( CNPC* pNPC )
{
	if( pNPC == NULL)
		return false;

	if( m_nBombSummonCnt >= 3)
		return false;

	int i=0;
	for(i=SUMMON_NPC_TYPE_BOMB_1; i<SUMMON_NPC_TYPE_BOMB_1+SUMMON_NPC_BOMB_MAX; i++)
	{
		if( m_pSummnonNpc[i] == NULL )
		{
			m_pSummnonNpc[i] = pNPC;
			m_nBombSummonCnt ++;
			return false;
		}
	}
	return false;
}

void CPC::DeleteSlave( CNPC* npc )
{
	m_listTaming.erase(npc);

	if( m_listTaming.empty() )
		m_bTaming_npc = false;
}

bool CPC::AddSlave( CNPC* npc)
{
	if( npc->m_owner)
		return false;

	set_npc_t::iterator it = m_listTaming.find(npc);
	if (it == m_listTaming.end())
		return false;

	m_listTaming.insert(npc);
	m_bTaming_npc = true;
	return true;
}

void CPC::GuildGradeSkill( bool apply )
{
	if( !m_guildInfo )
		return;

	int skillIndex = -1;

	switch( m_guildInfo->pos() )
	{
	case MSG_GUILD_POSITION_RUSH_CAPTAIN:
		skillIndex = 1177;
		break;	// 돌격조 대장
	case MSG_GUILD_POSITION_SUPPORT_CAPTAIN:
		skillIndex = 1181;
		break;	// 지원조 대장
	case MSG_GUILD_POSITION_RECON_CAPTAIN:
		skillIndex = 1179;
		break;	// 정찰조 대장
	case MSG_GUILD_POSITION_RUSH_MEMBER:
		skillIndex = 1178;
		break;	// 돌격조원
	case MSG_GUILD_POSITION_SUPPORT_MEMBER:
		skillIndex = 1182;
		break;	// 지원조원
	case MSG_GUILD_POSITION_RECON_MEMBER:
		skillIndex = 1180;
		break;	// 정찰조원
	default:
		return;
	}

	if( !apply )
	{
		m_assist.CureBySkillIndex( skillIndex );
		return;
	}

	CWarCastle* castle = CWarCastle::GetCastleObject(m_pZone->m_index);
	if( castle != NULL && castle->GetState() != WCSF_NORMAL && GetMapAttr() & MATT_WAR
			&& GetJoinFlag(castle->GetZoneIndex()) != WCJF_NONE )
	{
		CSkill * pSkill = gserver->m_skillProtoList.Create( skillIndex );
		if( pSkill == NULL )
			return;

		bool bApply;
		ApplySkill((CCharacter*)this, (CCharacter*)this, pSkill, -1, bApply);
		delete pSkill;
		pSkill = NULL;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			StatusMsg(rmsg, this);
			SEND_Q(rmsg, m_desc );
		}
	}
}

//Item Upgrade Fucking Function 2011
// 아이템 UPGRADE 무기 방어구 공식이 같음
int CPC::ItemUpgradeFuckingFunction( int itemPlus, int itemLevel, int itemAttack )
{
	// 플러스 0일때는 계산 안하고 공격력을 그냥 돌려줌
	if( itemPlus == 0 )
		return itemAttack;

	// 공통 상수
	double plusFactor = 1.06 ;

	if (itemLevel < ITEM_UPGRADE_GENERAL_MAXLEVEL )
	{
		if (itemPlus > 10)
			plusFactor = 1.07;
	}
	else
	{
		plusFactor = 1.188;
	}

	// 무기 / 방어구 공식이 같음
	const int itemUpgradeLimit = 17;

	// USA : plusFactor^27 , plusFactor^26 ETC: plusFactor^17 , plusFactor^16
	double func_value_1 = pow(plusFactor,itemUpgradeLimit);
	double func_value_2 = pow(plusFactor,itemUpgradeLimit-1);

	int nRetAttack = 0;

	if( itemLevel < ITEM_UPGRADE_GENERAL_MAXLEVEL )
	{
		if ( itemPlus > itemUpgradeLimit )	// 오바되는 인첸트 공식
		{
			nRetAttack = (int)( itemAttack * func_value_1 + (itemPlus-itemUpgradeLimit) * itemAttack * (func_value_1-func_value_2) ) ;
		}
		else	// 기본 공식
		{
			nRetAttack = (int)(itemAttack * pow(plusFactor, itemPlus));
		}
	}
	else
	{
		if ( itemPlus > itemUpgradeLimit )	// 오바되는 인첸트 공식
		{
			nRetAttack = (int)( itemAttack + itemAttack * (func_value_1/10) + (itemPlus-itemUpgradeLimit) * itemAttack * ((func_value_1-func_value_2)/10) );
		}
		else	// 기본 공식
		{
			nRetAttack = (int)(itemAttack + itemAttack * pow(plusFactor, itemPlus) / 10 );
		}
	}
	return nRetAttack;
}
//Item Upgrade Fucking Function 2011

int CPC::GetKillCount()
{
	return m_nKillCount;
}

int CPC::GetTotalKillCount()
{
	return m_nTotalKillCount;
}

int CPC::GetDeathCount()
{
	return m_nDeathCount;
}

int CPC::GetTotalDeathCount()
{
	return m_nTotalDeathCount;
}

int CPC::GetWarGroundPoint()
{
	return m_nWarGroundPoint;
}

int CPC::GetWarGroundAccPoint()
{
	return m_nWarGroundAccPoint;
}

void CPC::InitKillCount()
{
	m_nKillCount = 0;
	GAMELOG << init("WAR GROUND",this) << "INIT KILL COUNT" << end;
}

void CPC::AddKillCount(int n)
{
	if((m_nKillCount += n) < 0)
		m_nKillCount = 0;
	if(n > 0)
		m_nTotalKillCount += n;
	GAMELOG << init("WAR GROUND",this) << "ADD KILL COUNT" << delim << n << end;
}

void CPC::SetTotalKillCount(int n)
{
	if(n < 0)	n = 0;
	m_nTotalKillCount = n;
	GAMELOG << init("WAR GROUND",this) << "SET TOTAL KILL COUNT" << delim << n << end;
}
/*
void CPC::AddTotalKillCount(int n)
{
	if((m_nTotalKillCount += n) < 0)
		m_nTotalKillCount = 0;
}*/

void CPC::InitDeathCount()
{
	m_nDeathCount = 0;
	GAMELOG << init("WAR GROUND",this) << "INIT DEATH COUNT" << end;
}

void CPC::AddDeathCount(int n)
{
	if((m_nDeathCount += n) < 0)
		m_nDeathCount = 0;
	if(n > 0)
		m_nTotalDeathCount += n;
	GAMELOG << init("WAR GROUND",this) << "ADD DEATH COUNT" << delim << n << end;
}

void CPC::SetTotalDeathCount(int n)
{
	if(n < 0)	n = 0;
	m_nTotalDeathCount = n;
	GAMELOG << init("WAR GROUND",this) << "SET TOTAL DEATH COUNT" << delim << n << end;
}
/*
void CPC::AddTotalDeathCount(int n)
{
	if((m_nTotalDeathCount += n) < 0)
		m_nTotalDeathCount = 0;
}*/

void CPC::SetWarGroundPoint(int n)
{
	if(n < 0)	n = 0;
	m_nWarGroundPoint = n;
	GAMELOG << init("WAR GROUND",this) << "SET WAR GROUND POINT" << delim << n << end;
}

void CPC::AddWarGroundPoint(int n)
{
	if((m_nWarGroundPoint += n) < 0)
		m_nWarGroundPoint = 0;

	if(n > 0)
		m_nWarGroundAccPoint += n;

	if(m_nWarGroundPoint > WARGROUND_MAX_POINT)
		m_nWarGroundPoint = WARGROUND_MAX_POINT;

	if(m_nWarGroundAccPoint > WARGROUND_MAX_ACC_POINT)
		m_nWarGroundAccPoint = WARGROUND_MAX_ACC_POINT;
	GAMELOG << init("WAR GROUND",this) << "ADD WAR GROUND POINT" << delim << n << end;
}

void CPC::SetWarGroundAccPoint(int n)
{
	if(n < 0)	n = 0;
	m_nWarGroundAccPoint = n;
	GAMELOG << init("WAR GROUND",this) << "SET WAR GROUND ACC POINT" << delim << n << end;
}
/*
void CPC::AddWarGroundAccPoint(int n)
{
	if((m_nWarGroundAccPoint += n) < 0)
		m_nWarGroundAccPoint = 0;
}*/

#ifdef REFORM_PK_PENALTY_201108 // PK 패널티 리폼
int CPC::GetPKPenaltyRewardNum()
{
	if( m_pkPenalty <= PK_CHAOTIC_POINT_MAX )
		return 0; // 카오 :: 자포자기
	else if( m_pkPenalty > PK_CHAOTIC_POINT_MAX && m_pkPenalty <= PK_CHAOTIC_POINT_2 )
		return 1; // 카오 :: 좌절 금지
	else if( m_pkPenalty > PK_CHAOTIC_POINT_2 && m_pkPenalty <= PK_CHAOTIC_POINT_1 )
		return 2; // 카오 :: 눈치 보는 사냥꾼
	else if( m_pkPenalty >= PK_HUNTER_POINT_1 && m_pkPenalty < PK_HUNTER_POINT_2 )
		return 3; // 헌터 :: 얼떨떨한 사냥꾼
	else if( m_pkPenalty >= PK_HUNTER_POINT_2 && m_pkPenalty < PK_HUNTER_POINT_MAX )
		return 4; // 헌터 :: 자신감 만땅
	else if( m_pkPenalty >= PK_HUNTER_POINT_MAX )
		return 5; // 헌터 :: 독불장군

	return -1;
}
void CPC::AddPkPenalty(int penalty)
{
	m_pkPenalty += penalty;

	if( m_pkPenalty > PK_HUNTER_POINT_MAX )
		m_pkPenalty = PK_HUNTER_POINT_MAX;

	if( m_pkPenalty < PK_CHAOTIC_POINT_MAX )
		m_pkPenalty = PK_CHAOTIC_POINT_MAX;
}
#endif

void CPC::reviseHPMP()
{
	if(this->m_tempHP &&
			(	this->m_hp <= this->m_tempHP &&
				this->m_tempHP <= this->m_maxHP)	)
	{
		this->m_hp = this->m_tempHP;
		this->m_tempHP = 0;
	}

	if(this->m_tempMP &&
			(	this->m_mp <= this->m_tempMP &&
				this->m_tempMP <= this->m_maxMP)	)
	{
		this->m_mp = this->m_tempMP;
		this->m_tempMP = 0;
	}
}

void CPC::StartRockPaperScissors()
{
	m_RockPaperScissorsInfo.SetCanPlay(true);
	m_RockPaperScissorsInfo.SetStart(true);
	m_RockPaperScissorsInfo.SetWinCount(0);
	m_RockPaperScissorsInfo.SetGameCount(1);
	m_RockPaperScissorsInfo.IncreaseTotalCount();
}

bool CPC::CanRockPaperScissors()
{
	// 1. 가위바위보 게임을 시작하지 않은 상태,
	// 2.이긴 횟수가 총 스테이지보다 큰 상태,
	// 3. 선택을 할 없는 상태
	// 위 3가지 경우에는 가위바위보를 선택할 수 없다.
	if (!m_RockPaperScissorsInfo.IsStart() || m_RockPaperScissorsInfo.GetWinCount() >= gserver->m_XmasRockPaperScissorsGame->GetTotalStage() || !m_RockPaperScissorsInfo.CanPlay() )
		return false;

	return true;
}

void CPC::setRockPaperScissorsFlag()
{
	if (m_bRockPaperScissorsFlag)
	{
		m_bRockPaperScissorsFlag = false;
	}
	else
	{
		m_bRockPaperScissorsFlag = true;
	}
}

bool CPC::getRockPaperScissorsFlag()
{
	return m_bRockPaperScissorsFlag;
}

bool CPC::isSyndicate()
{
	return (m_syndicateManager.syndicateData_.syndicateType != 0);
}

#ifdef DURABILITY
void CPC::calcDurability_for_weapon()
{
	bool wear_inven_flag = true;
	CItem* item = this->m_wearInventory.getWearItem(WEARING_WEAPON);
	if (item == NULL)
		return;

	// 결합한 무기이면 결합된 아이템을 취함
	if ((item->getFlag() & FLAG_ITEM_COMPOSITION) && (item->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE))
	{
		item = this->m_inventory.FindByVirtualIndex(item->m_nCompositeItem);
		if (item == NULL)
			return;

		wear_inven_flag = false;
	}

	if ((item->m_itemProto->getItemFlag() & ITEM_FLAG_DURABILITY) == 0)
		return;

	if (item->getNowDurability() >= 10)
	{
		item->setNowDurability(item->getNowDurability() - 1);

		if (wear_inven_flag)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::makeItemDurabilityForWear(rmsg, WEARING_WEAPON, item->getNowDurability());
			SEND_Q(rmsg, this->m_desc);
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::makeItemDurabilityForInventory(rmsg, item->tab(), item->getInvenIndex(),
					item->getNowDurability(), item->getMaxDurability());
			SEND_Q(rmsg, this->m_desc);
		}

		// recalc
		if (item->getNowDurability() < 10)
			this->ApplyItemValue(true);
	}
}

void CPC::calcDurability_for_armor()
{
	static int loop[] =
	{
		WEARING_HELMET,
		WEARING_ARMOR_UP,
		WEARING_ARMOR_DOWN,
		WEARING_SHIELD,
		WEARING_GLOVE,
		WEARING_BOOTS,
		WEARING_ACCESSORY1,
		WEARING_ACCESSORY2,
		WEARING_ACCESSORY3,
		WEARING_BACKWING
	};

	int recalc_flag = false;
	CItem* item = NULL;

	for (int i = 0; i < sizeof(loop) / sizeof(loop[0]); ++i)
	{
		item = this->m_wearInventory.getWearItem(loop[i]);
		if (item == NULL)
			continue;

		bool wear_inven_flag = true;

		// 결합한 무기이면 결합된 아이템을 취함
		if ((item->getFlag() & FLAG_ITEM_COMPOSITION) && (item->m_itemProto->getItemFlag() & ITEM_FLAG_COMPOSITE))
		{
			item = this->m_inventory.FindByVirtualIndex(item->m_nCompositeItem);
			if (item == NULL)
				continue;

			wear_inven_flag = false;
		}

		if ((item->m_itemProto->getItemFlag() & ITEM_FLAG_DURABILITY) == 0)
			continue;

		if (item->getNowDurability() >= 10)
		{
			item->setNowDurability(item->getNowDurability() - 1);

			if (wear_inven_flag)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				UpdateClient::makeItemDurabilityForWear(rmsg, loop[i], item->getNowDurability());
				SEND_Q(rmsg, this->m_desc);
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				UpdateClient::makeItemDurabilityForInventory(rmsg, item->tab(), item->getInvenIndex(),
						item->getNowDurability(), item->getMaxDurability());
				SEND_Q(rmsg, this->m_desc);
			}

			if (item->getNowDurability() < 10)
				recalc_flag = true;
		}
	}

	// 내구도 상황에 따라 방어력 재 계산
	if (recalc_flag)
	{
		this->ApplyItemValue(true);
	}
}
#endif
