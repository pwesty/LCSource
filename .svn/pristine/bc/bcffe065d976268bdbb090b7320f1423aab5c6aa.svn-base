#include "stdhdrs.h"

#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "dofuncAdmin.h"

#ifdef DURABILITY
extern void do_Durability(CPC* ch, CNetMsg::SP& msg);
#endif

CCmdList::CCmdList()
{
}

void CCmdList::AddMessage()
{
	// Chatting
	Add(MSG_CHAT, do_Chat, 0, true, true);			// 채팅

	// 이동 관련
	Add(MSG_MOVE, do_Move, 0, false, false);			// 이동
	Add(MSG_GO_ZONE, do_GoZone, 0, false, false);		// 존이동

//N1InPersonalDungeon : bw
	Add(MSG_PD_ATTACK, do_pd_Attack, 0, false, true);
	Add(MSG_PD_MOVE, do_pd_Move, 0, false, true);

	// 공격 관련
	Add(MSG_ATTACK, do_Attack, 0, false, false);

	// 아이템 관련
	Add(MSG_ITEM, do_Item, 0, false, false);

	// 교환 관련
	Add(MSG_EXCHANGE, do_Exchange, 0, false, false);

	// 장소 기억 관련
	Add(MSG_MEMPOS, do_MemPos, 0, false, false);

	// 운영자 관련
	Add(MSG_GM, do_GM, 1, true, true);

	// 파티 관련
	Add(MSG_PARTY, do_Party, 0, false, false);

	// 퀵슬롯관련
	Add(MSG_QUICKSLOT, do_QuickSlot, 0, false, true);

	// 싱글던전 - NPC 리젠
	Add(MSG_NPC_REGEN, do_NPCRegen, 0, false, true);

	// 스킬
	Add(MSG_SKILL, do_Skill, 0, false, false);

	// 부활메시지
	Add(MSG_PC_REBIRTH, do_Rebirth, 0, false, false);

	// 액션 메시지
	Add(MSG_ACTION, do_Action, 0, false, false);

	// 퀘스트
	Add(MSG_QUEST, do_Quest, 0, false, false);

	// 성장 포인트 관련
	Add(MSG_STATPOINT, do_StatPoint, 0, false, true);

	// 이동 장치 관련
	Add(MSG_WARP, do_Warp, 0, true, false);

	// 펄스 관련
	Add(MSG_PULSE, do_Pulse, 0, true, true);

	// 특수스킬 관련
	Add(MSG_SSKILL, do_SSkill, 0, false, false);

	Add(MSG_PK, do_PK, 0, false, false);

	Add(MSG_EVENT, do_Event, 0, false, false);

	Add(MSG_PERSONALSHOP, do_PersonalShop, 0, false, false);

	Add(MSG_STASH, do_Stash, 0, false, false);

	Add(MSG_GUILD, do_Guild, 0, false, false);

	Add(MSG_CHANGE, do_Change, 0, false, false);

	Add(MSG_UI, do_UI, 0, false, false);

	Add(MSG_TEACH,	do_Teach, 0, false, false);

	Add(MSG_CHANGEJOB, do_ChangeJob, 0, false, false);

	Add(MSG_FRIEND,	do_Friend, 0, false, false);

	Add(MSG_SELECT_PRODUCE, do_SelectProduce, 0, false, false);

	Add(MSG_EXTEND, do_Extend, 0, true, false);
	Add(MSG_RANDOM_PRODUCE, do_RandomProduce, 0, false, false);

	Add(MSG_TRADEAGENT, do_TradeAgent, 0, false, false);

	Add(MSG_EXPEDITION, do_Expedition, 0, false, false);

	Add(MSG_RAID, do_Raid, 0, false, false);

	Add(MSG_PET_STASH, do_PetStash, 0, false, false);

	Add(MSG_FACTORY, do_Factory, 0, false, false);

	Add(MSG_EXPRESS_SYSTEM, do_ExpressSystem, 0, false, false);
	Add(MSG_RESERVED_GM_COMMAND, do_ReservedGMCommand, 0, false, false);

	Add(MSG_RVR, do_RVR, 0, false, false);

#ifdef DURABILITY
	Add(MSG_DURABILITY, do_Durability, 0, false, false);
#endif
	Add(MSG_GPS, do_GPS, 0, false, false);
	Add(MSG_ITEM_COLLECTION, do_ItemCollection, 0, false, false);

#ifdef PREMIUM_CHAR
	Add(MSG_PREMIUM_CHAR, do_PremiumChar, 0, false, false);
#endif
}

void CCmdList::AddExMessage()
{
	Add(MSG_EX_PET_CALL, do_ExPetCall, 0, false, false);
	Add(MSG_EX_PET_LEARN, do_ExPetLearn, 0, false, false);
	Add(MSG_EX_PET_RESET_SKILL, do_ExPetResetSkill, 0, false, false);
	Add(MSG_EX_PET_CHANGE_MOUNT, do_ExPetChangeMount, 0, false, false);
	Add(MSG_EX_PET_COMMAND, do_ExPetCommand, 0, false, false);
	Add(MSG_EX_PET_MIX_ITEM, do_ExPetMixItem, 0, false, false);
	Add(MSG_EX_PET_WARPTOWN, do_ExPetWarpTown, 0, false, false);
	Add(MSG_EX_PET_CHANGE_ITEM, do_ExPetChangeItem, 0, false, false);
	Add(MSG_EX_PET_REBIRTH, do_ExPetRebirth, 0, false, false);
	Add( MSG_EX_PET_CHANGE_NAME, do_PetNameChange, 0, false, false );

	Add(MSG_EX_NAMECHANGE,	do_Ex_NameChange, 0, false, false);		// 이름 변경 아이템	: tab_idx(uc) row_idx(uc) col_idx(uc) item_idx(n) name(str)
	Add(MSG_EX_CASHITEM,	do_Ex_CashItem, 0, false, false);

	Add(MSG_EX_CASTLE_MAP_RECENT, do_ExCastleMapRecent, 0, false, false);
	Add(MSG_EX_CASTLE_MAP_SIGNAL, do_ExCastleMapSignal, 0, false, false);

	Add(MSG_EX_PARTY_RECALL, do_ExPartyRecall, 0, false, false);

	Add(MSG_EX_PARTY_MATCH, do_ExPartyMatch, 0, false, false);

	Add(MSG_EX_MESSENGER, do_ExMessenger, 0, false, false);

	Add(MSG_EX_CASTLE_WAR, do_ExCastleWar, 0, false, false);
	Add(MSG_EX_DVD, do_DVD, 0, false, false);

	Add( MSG_EX_MONSTERCOMBO, do_MonsterCombo, 0, false, false );

	Add( MSG_EX_RESTART, do_ClientRestart, 0 , false, false );

#ifdef CHAOSBALL
	Add( MSG_EX_CHAOSBALL, do_ChaosBall, 0, false, false);
#endif

	Add( MSG_EX_ATTACK_PET, do_AttackPet, 0, false, false );

#ifdef EXTREME_CUBE
	Add( MSG_EX_EXTREME_CUBE, do_ExExtremeCube, 0, false, false );
#endif // EXTREME_CUBE

	Add( MSG_EX_INIT_SSKILL, do_ExInitSSkill, 0, false, false );

#ifdef DISCONNECT_HACK_CHARACTER
	// 펄스 관련
	Add(MSG_EX_DISCONNECT_HACK_CHARACTER, do_Disconnect_Hack_Character, 0, true, true);
#endif

	Add(MSG_EX_TRIGGER_EVENT, do_TriggerEvent, 0, false, false);

	Add(MSG_EX_AFFINITY, do_Affinity, 0, false, false);

	Add(MSG_EX_RAID_INFO, do_ExRaidInfo, 0, false, false);

	Add(MSG_EX_NPC_PORTAL_SCROLL, do_NpcPortalScroll, 0, false, false);

	Add(MSG_EX_TITLE_SYSTEM, do_ExTitleSytem, 0, false, false);

#ifdef LACARETTE_SYSTEM
	Add(MSG_EX_LACARETTE_SYSTEM, do_ExLacaretteSystem, 0, false, false);
#endif

	Add( MSG_EX_TAKE_AGAIN_QUEST_ITEM, do_ExTakeAgainQuestItem, 0, false, false);

	Add( MSG_EX_SOCKET, do_FuncExSocket, 0, false, false);

	Add( MSG_EX_ATTENDANCE_EXP_SYSTEM, do_FuncAttendanceExp, 0, false, false );

#ifdef XTRAP
	Add( MSG_EX_XTRAP, do_FuncXtrap, 0, false, false );
#endif // XTRAP

	Add( MSG_EX_RAID_SCENE, do_ExRaidScene, 0, false, false);

	Add( MSG_EX_TUTORIAL_RENEWER_2010, do_ExFuncTutorialRenewer, 0, false, false);

#ifdef ENABLE_SUBJOB
	Add( MSG_EX_SUBJOB , do_ExFuncSubJob, 0, false, false );
#endif //ENABLE_SUBJOB

	Add( MSG_EX_CASTLLAN , do_ExFuncCastllan, 0, false, false );

	Add( MSG_EX_FACEOFF, do_ExFaceOff, 0, false, false );

	Add( MSG_EX_LCBALL, do_ExLCBall, 0, false, false );

#ifdef JUNO_RENEWAL_MESSAGEBOX
	Add( MSG_EX_MSGBOX, do_ExMsgBox, 0, false, false );
#endif // JUNO_RENEWAL_MESSAGEBOX

	Add( MSG_EX_ROYAL_RUMBLE, do_ExRoyalRumble, 0, false, false );

#ifdef GAMIGO_CASH_AUTHENTICATION_CODE_USE
	Add( MSG_EX_CASH_AUTHENTICATION_CODE, do_ExCashAuthenticationCode, 0, false, false );
#endif

#ifdef SYSTEM_TREASURE_MAP
#ifdef SYSTEM_TREASURE_MAP_GAMIGO
	Add(MSG_EX_TREASURE_MAP_SYSTEM_KEY_EXCHANGE, do_ExTreasureMapKeyExchange, 0, false, false );
#endif
#endif

	Add(MSG_EX_RANKING_SYSTEM_EX, do_rankingSystem, 0, false, false );

#ifdef REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼
	Add(MSG_EX_PKPENALTY_REFORM, do_ExPKPeneltyReform, 0, false, false);
#endif

#ifdef NOTICE_SYSTEM
	Add(MSG_EX_USER_NOTICE, do_UserNotice, 0, false, false);
#endif
	Add(MSG_EX_REFORMER_SYSTEM, do_ReformSystem, 0, false, false);

	Add(MSG_EX_SERVER_TIME, do_serverTime, 0, false, false);

	Add(MSG_EX_SET_TARGET, do_setTarget, 0, false, false);
	Add(MSG_EX_NOTICE, do_noticeExtend, 0, false, false);
}

bool CCmdList::Add(int cmdType, LCCMDPROC cmdFunc, int minlevel, bool bCanDoWhileWarp, bool bCanDoWhileDisable)
{
	CCmd* p = new CCmd;
	p->m_cmdType = cmdType;
	p->m_cmdFunc = cmdFunc;
	p->m_minLevel = minlevel;
	p->m_bCanDoWhileWarp = bCanDoWhileWarp;
	p->m_bCanDoWhileDisable = bCanDoWhileDisable;

	bool ret = map_.insert(map_t::value_type(cmdType, p)).second;

	return ret;
}

void CCmdList::Run(int cmd_num, CPC* ch, CNetMsg::SP& msg)
{
	map_t::iterator it = map_.find(cmd_num);
	if (it == map_.end())
		return;

	CCmd* p = it->second;

	// 이동 장치 사용중에 불가능한지 검사
	if (!p->m_bCanDoWhileWarp && ch->IsSetPlayerState(PLAYER_STATE_WARP))
		return ;

	if( ch->IsSetPlayerState(PLAYER_STATE_CASHITEM) )
		return;

	if (!p->m_bCanDoWhileDisable && ch->IsDisable())
		return ;

	(*p->m_cmdFunc)(ch, msg);
}

CCmd::CCmd()
{
}

CCmd::~CCmd()
{
}

CCmdList::~CCmdList()
{
	map_t::iterator it = map_.begin();
	map_t::iterator endit = map_.end();
	for (; it != endit; ++it)
	{
		delete (it->second);
	}
	map_.clear();
}

bool CCmdList::Find(int cmdType)
{
	map_t::iterator it = map_.find(cmdType);
	return (it != map_.end()) ? true : false;
}
//