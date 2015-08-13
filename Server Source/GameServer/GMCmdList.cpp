#include <boost/tokenizer.hpp>
#include "stdhdrs.h"

#include "Server.h"
#include "doFunc.h"
#include "Log.h"
#include "../ShareLib/gm_command.h"
#include "dofuncAdmin.h"
#include "GMCmdList.h"

typedef boost::tokenizer<boost::char_separator<char> > stokenizer;
static boost::char_separator<char> sep(" ", NULL, boost::drop_empty_tokens);

//////////////////////////////////////////////////////////////////////////
CGMCmd::CGMCmd(std::string command, int level, GMFunction fun)
	: m_command(command), m_level(level), m_function(fun)
{
}

CGMCmd::~CGMCmd(void)
{
}

void CGMCmd::run(CPC* pc, const char* arg, std::vector<std::string>& vec)
{
	m_function(pc, arg, vec);
}

std::string& CGMCmd::getCommand(void)
{
	return m_command;
}

int CGMCmd::getLevel(void) const
{
	return m_level;
}

GMFunction CGMCmd::getFunction(void) const
{
	return m_function;
}
//////////////////////////////////////////////////////////////////////////

CGMCmdList* CGMCmdList::m_instance = NULL;

CGMCmdList::CGMCmdList(void)
{
}

CGMCmdList::~CGMCmdList(void)
{
}

CGMCmdList* CGMCmdList::getInstance(void)
{
	if ( m_instance == NULL )
		m_instance = new CGMCmdList();

	return m_instance;
}

void CGMCmdList::load(void)
{
	add(new CGMCmd(GM_CMD_HPMP, 1, do_GM_hpmp));
	add(new CGMCmd(GM_CMD_HP, 1, do_GM_hp));
	add(new CGMCmd(GM_CMD_MP, 1, do_GM_mp));
	add(new CGMCmd(GM_CMD_GUILD_POINT, 1, do_GM_guildPoint));
	add(new CGMCmd(GM_CMD_EVENT_COLLECT, 1, do_GM_eventCollect));
	add(new CGMCmd(GM_CMD_BLOCK, 1, do_GM_block));
	add(new CGMCmd(GM_CMD_RELEASE, 1, do_GM_release));
	add(new CGMCmd(GM_CMD_COUPON, 1, do_GM_coupon));
	add(new CGMCmd(GM_CMD_PROMOTION, 1, do_GM_promotion));
	add(new CGMCmd(GM_CMD_SETWARTIME, 1, do_GM_setWarTime));
	add(new CGMCmd(GM_CMD_SETWARTIME2, 1, do_GM_setWarTime2));
	add(new CGMCmd(GM_CMD_SETWARTIMEEND, 1, do_GM_setWarTimeEnd));
	add(new CGMCmd(GM_CMD_SETWARTIME2END, 1, do_GM_setWarTime2End));
	add(new CGMCmd(GM_CMD_LOADHP5, 1, do_GM_loadhp5));
	add(new CGMCmd(GM_CMD_GUILDCUBESTART, 1, do_GM_guildCubeStart));
	add(new CGMCmd(GM_CMD_GUILDCUBEEND, 1, do_GM_guildCubeEnd));
	add(new CGMCmd(GM_CMD_GUILDCUBESTATE, 1, do_GM_guildCubeState));
	add(new CGMCmd(GM_CMD_GUILDCUBETIMECHECK, 1, do_GM_guildCubeTimeCheck));
	add(new CGMCmd(GM_CMD_SETOWNER, 1, do_GM_setOwner));
	add(new CGMCmd(GM_CMD_LORDCHAT, 1, do_GM_lordChat));
	add(new CGMCmd(GM_CMD_CREATEPET, 1, do_GM_createPet));
	add(new CGMCmd(GM_CMD_APETSTAT, 1, do_GM_apetStat));
	add(new CGMCmd(GM_CMD_APETHPMP, 1, do_GM_apetHPMP));
	add(new CGMCmd(GM_CMD_APETEXP, 1, do_GM_apetExp));
	add(new CGMCmd(GM_CMD_APETLEVELUP, 1, do_GM_apetLevelUp));
	add(new CGMCmd(GM_CMD_APETUSE, 1, do_GM_apetUse));
	add(new CGMCmd(GM_CMD_APETCOOLTIME, 1, do_GM_apetCoolTime));
	add(new CGMCmd(GM_CMD_APETACCEXP, 1, do_GM_apetAccExp));
	add(new CGMCmd(GM_CMD_SVRINFO, 1, do_GM_svrInfo));
	add(new CGMCmd(GM_CMD_EP, 1, do_GM_ep));
	add(new CGMCmd(GM_CMD_RANKREFRESH0831, 1, do_GM_rankRefresh0831));
	add(new CGMCmd(GM_CMD_PETLEVELUP, 1, do_GM_petLevelUp));
	add(new CGMCmd(GM_CMD_PETDEADTIME, 1, do_GM_petDeadTime));
	add(new CGMCmd(GM_CMD_PETRESET, 1, do_GM_petReset));
	add(new CGMCmd(GM_CMD_PETEXPUP, 1, do_GM_petExpUp));
	add(new CGMCmd(GM_CMD_PETMOUNTRESET, 1, do_GM_petMountReset));
	add(new CGMCmd(GM_CMD_PETCHANGENAME, 1, do_GM_petChangeName));
	add(new CGMCmd(GM_CMD_PKCOUNT, 1, do_GM_pkCount));
	add(new CGMCmd(GM_CMD_PKPENALTY, 1, do_GM_pkPenalty));
	add(new CGMCmd(GM_CMD_SHOWHP, 1, do_GM_showHP));
	add(new CGMCmd(GM_CMD_GOPOS, 1, do_GM_goPOS));
	add(new CGMCmd(GM_CMD_PDNPC, 1, do_GM_pdNPC));
	add(new CGMCmd(GM_CMD_CASHBALANCE, 1, do_GM_cashBalance));
	add(new CGMCmd(GM_CMD_CASHPURCHASE, 1, do_GM_cashPurchase));
	add(new CGMCmd(GM_CMD_CASHPURCHASELIST, 1, do_GM_cashPurchaseList));
	add(new CGMCmd(GM_CMD_CASHPURCHASEHISTORY, 1, do_GM_cashPurchaseHistory));

	add(new CGMCmd(GM_CMD_ECHO, GM_LEVEL_ECHO, do_GMEcho));
	add(new CGMCmd(GM_CMD_GO_ZONE, GM_LEVEL_GO_ZONE, do_GMGoZone));
	add(new CGMCmd(GM_CMD_GOTO, GM_LEVEL_GOTO, do_GMGoto));
	add(new CGMCmd(GM_CMD_ITEMDROP, GM_LEVEL_ITEMDROP, do_GMItemDrop));
	add(new CGMCmd(GM_CMD_COUNT_PC, GM_LEVEL_COUNT_PC, do_GMCountPC));
	add(new CGMCmd(GM_CMD_LOOPCOUNT, GM_LEVEL_LOOPCOUNT, do_GMLoopCount));
	add(new CGMCmd(GM_CMD_IMMORTAL, GM_LEVEL_IMMORTAL, do_GMImmortal));
	add(new CGMCmd(GM_CMD_SHUTDOWN, GM_LEVEL_SHUTDOWN, do_GMShutdown));
	add(new CGMCmd(GM_CMD_REBOOT, GM_LEVEL_REBOOT, do_GMReboot));
	add(new CGMCmd(GM_CMD_COUNT_NPC, GM_LEVEL_COUNT_NPC, do_GMCountNPC));
	add(new CGMCmd(GM_CMD_VISIBLE, GM_LEVEL_VISIBLE, do_GMVisible));
	add(new CGMCmd(GM_CMD_GO_PC, GM_LEVEL_GO_PC, do_GMGoPC));
	add(new CGMCmd(GM_CMD_GO_NPC, GM_LEVEL_GO_NPC, do_GMGoNPC));
	add(new CGMCmd(GM_CMD_LEVELUP, GM_LEVEL_LEVELUP, do_GMLevelUP));
	add(new CGMCmd(GM_CMD_KICK, GM_LEVEL_KICK, do_GMKICK));
	add(new CGMCmd(GM_CMD_COMON, GM_LEVEL_COMON, do_GMComon));
	add(new CGMCmd(GM_CMD_SUMMON, GM_LEVEL_SUMMON, do_GMSummon));
	add(new CGMCmd(GM_CMD_SILENCE, GM_LEVEL_SILENCE, do_GMSilence));
	add(new CGMCmd(GM_CMD_WHERE_AM_I, GM_LEVEL_WHERE_AM_I, do_GMWhereAmI));
	add(new CGMCmd(GM_CMD_QUEST_COMPLETE, GM_LEVEL_QUEST_COMPLETE, do_GMQuestComplete));
	add(new CGMCmd(GM_CMD_STATPOINT, GM_LEVEL_STATPOINT, do_GMStatPoint));
	add(new CGMCmd(GM_CMD_SKILLPOINT, GM_LEVEL_SKILLPOINT, do_GMSkillPoint));
	add(new CGMCmd(GM_CMD_SET_REGENSEC, GM_LEVEL_SET_REGENSEC, do_GMSetRegenSec));
	add(new CGMCmd(GM_CMD_NPC_REGEN, GM_LEVEL_NPC_REGEN, do_GMNPCRegen));
	add(new CGMCmd(GM_CMD_KILL_NPC, GM_LEVEL_KILL_NPC, do_GMKillNPC));
	add(new CGMCmd(GM_CMD_EXPUP, GM_LEVEL_EXPUP, do_GMExpUp));
	add(new CGMCmd(GM_CMD_BLOODPOINT, GM_LEVEL_BLOODPOINT, do_GMBloodPoint));
	add(new CGMCmd(GM_CMD_ENDGAME, GM_LEVEL_ENDGAME, do_GMEndGame));
	add(new CGMCmd(GM_CMD_OBSERVE, GM_LEVEL_OBSERVE, do_GMObserve));
	add(new CGMCmd(GM_CMD_CHATMONITOR, GM_LEVEL_CHATMONITOR, do_GMChatMonitor));
	add(new CGMCmd(GM_CMD_DOUBLE_EVENT, GM_LEVEL_DOUBLE_EVENT, do_GMDoubleEvent));
	add(new CGMCmd(GM_CMD_LATTO_EVENT, GM_LEVEL_LATTO_EVENT, do_GMLattoEvent));
	add(new CGMCmd(GM_CMD_DOUBLE_EXP_EVENT, GM_LEVEL_DOUBLE_EXP_EVENT, do_GMDoubleExpEvent));
	add(new CGMCmd(GM_CMD_NEWYEAR_EVENT, GM_LEVEL_NEWYEAR_EVENT, do_GMNewYearEvent));
	add(new CGMCmd(GM_CMD_VALENTINE_EVENT, GM_LEVEL_VALENTINE_EVENT, do_GMValentineEvent));
	add(new CGMCmd(GM_CMD_WHITEDAY_EVENT, GM_LEVEL_WHITEDAY_EVENT, do_GMWhiteDayEvent));
	add(new CGMCmd(GM_CMD_RELOAD_GMIP, GM_LEVEL_RELOAD_GMIP, do_GMReloadGMIP));
	add(new CGMCmd(GM_CMD_SPEEDUP, GM_LEVEL_SPEEDUP, do_GMSpeedUp));
	add(new CGMCmd(GM_CMD_FAMEUP, GM_LEVEL_FAMEUP, do_GMFameUp));
	add(new CGMCmd(GM_CMD_EVENT, GM_LEVEL_EVENT, do_GMEvent));
	add(new CGMCmd(GM_CMD_PD4, GM_LEVEL_PD4, do_GMPD4));
	add(new CGMCmd(GM_CMD_KICK_ID, GM_LEVEL_KICK_ID, do_GMKickID));
	add(new CGMCmd(GM_CMD_RESURRECTION, GM_LEVEL_RESURRECTION, do_GMResurrection));
	add(new CGMCmd(GM_CMD_RECOMMEND, GM_LEVEL_RECOMMEND, do_GMRecommend));
	add(new CGMCmd(GM_CMD_SETLIMIT, GM_LEVEL_SETLIMIT, do_GMSetLimit));
	add(new CGMCmd(GM_CMD_DISGUISE, GM_LEVEL_DISGUISE, do_GMDisguise));
	add(new CGMCmd(GM_CMD_LOGINSERVER, GM_LEVEL_LOGINSERVER, do_GMLoginServer));
	add(new CGMCmd(GM_CMD_KICK_CHAR_INDEX, GM_LEVEL_KICK_CHAR_INDEX, do_GMKickCharIndex));
	add(new CGMCmd(GM_GMD_EVENTSHOW, GM_LEVEL_EVENTSHOW, do_GMEventShow));
	add(new CGMCmd(GM_CMD_EVENTSETTING, GM_LEVEL_EVENTSETTING, do_GMEventSetting));
	add(new CGMCmd(GM_CMD_CHANCE_EVENT, GM_LEVEL_CHANCE_EVENT, do_GMChanceEvent));
	add(new CGMCmd(GM_CMD_GMCHATMONITOR, GM_LEVEL_GMCHATMONITOR, do_GMToolChatMonitor));
	add(new CGMCmd(GM_CMD_DOUBLE_ITEM_DROP, GM_LEVEL_DOUBLE_ITEM_EVENT, do_GMDoubleItemEvent));
	add(new CGMCmd(GM_CMD_DOUBLE_PET_EXP, GM_LEVEL_DOUBLE_PET_EXP_EVENT, do_GMDoublePetExpEvent));
	add(new CGMCmd(GM_CMD_DOUBLE_ATTACK, GM_LEVEL_DOUBLE_ATTACK_EVENT, do_GMDoubleAttackEvent));
	add(new CGMCmd(GM_CMD_EVENT_DROPITEM, GM_LEVEL_EVENT_DROPITEM, do_GMDropItemEvent));
	add(new CGMCmd(GM_CMD_KICK_PC_MULTI, GM_LEVEL_KICK_PC_MULRI, do_GMKickMulti));
	add(new CGMCmd(GM_CMD_SILENCE_PC_MULTI, GM_LEVEL_SILENCE_PC_MULTI, do_GMSilenceMulti));
	add(new CGMCmd(GM_CMD_SET_EXPLIMIIT, GM_LEVEL_SET_EXPLIMIT, do_GMExpLimit));
	add(new CGMCmd(GM_CMD_DOUBLE_EVENT_AUTO, GM_LEVEL_DOUBLE_EVENT_AUTO, do_GMDoubleEventAuto));
	add(new CGMCmd(GM_CMD_UPGRADE_EVENT, GM_LEVEL_UPGRADE_EVENT, do_GMUpgradeEvent));
	add(new CGMCmd(GM_CMD_KICK_GUILDMEMBER, GM_LEVEL_KICK_GUILDMEMBER, do_GMKickGuildMember));
	add(new CGMCmd(GM_CMD_APPLY_SKILL, GM_LEVEL_APPLY_SKILL, do_GMApplySkill));
	add(new CGMCmd(GM_CMD_STATE_FLAG, GM_LEVEL_STATE_FLAG, do_GMStateFlag));
	add(new CGMCmd(GM_CMD_DATE, GM_LEVEL_DATE, do_GMDate));
	add(new CGMCmd(GM_CMD_PLAY_TRIGGER, GM_LEVEL_PLAY_TRIGGER, do_GMPlayTrigger));
	add(new CGMCmd(GM_CMD_BATTLE_START, GM_LEVEL_BATTLE_START, do_GMBattleStart));
	add(new CGMCmd(GM_CMD_BATTLE_STOP, GM_LEVEL_BATTLE_STOP, do_GMBattleStop));
	add(new CGMCmd(GM_CMD_RESET_RAID, GM_LEVEL_RESET_RAID, do_GMResetRaid));
	add(new CGMCmd(GM_CMD_SET_SEAL_EXP, GM_LEVEL_SET_SEAL_EXP, do_GMSetSealExp));
	add(new CGMCmd(GM_CMD_END_SUBHELPER, GM_LEVEL_END_SUBHELPER, do_GMEndSubHelper));
	add(new CGMCmd(GM_CMD_MY_ADMIN, GM_LEVEL_MY_ADMIN, do_GMMyAdmin));
	add(new CGMCmd(GM_CMD_DT_TIMESET, GM_LEVEL_DT_TIMESET, do_GMDTTimeSet));
	add(new CGMCmd(GM_CMD_DT_EXPSET, GM_LEVEL_DT_EXPSET, do_GMDTExpSet));
	add(new CGMCmd(GM_CMD_DT_TIMEINTERVAL, GM_LEVEL_DT_TIMEINTERVAL, do_GMDTTimeInterval));
	add(new CGMCmd(GM_CMD_DT_TOGGLE, GM_LEVEL_DT_TOGGLE, do_GMDTToggle));
	add(new CGMCmd(GM_CMD_RELOAD_CATALOG, GM_LEVEL_RELOAD_CATALOG, do_GMReloadCatalog));
	add(new CGMCmd(GM_CMD_NPC_DROP, GM_LEVEL_NPC_DROP, do_GMNPCDrop));
	add(new CGMCmd(GM_CMD_GODBLESS, GM_LEVEL_GODBLESS, do_GMGodBless));
	add(new CGMCmd(GM_CMD_ENCHANT, GM_LEVEL_ENCHANT, do_GMEnchant));
	add(new CGMCmd(GM_CMD_MODIFY_CASHSHOP, GM_LEVEL_MODIFY_CASHSHOP, do_GMModifyCashShop));
	add(new CGMCmd(GM_CMD_LCBALL_RESET, GM_LEVEL_LCBALL_RESET, do_GMLCBallReset));
	add(new CGMCmd(GM_CMD_ATTEVENT_2011, GM_LEVEL_ATTEVENT_2011, do_GMAttEvent));
	add(new CGMCmd(GM_CMD_FUNC, GM_LEVEL_FUNC, do_GMFunc));
	add(new CGMCmd(GM_CMD_ECHONPC, GM_LEVEL_ECHONPC, do_GMEchoNpc));
	add(new CGMCmd(GM_CMD_CASTLE_STATE_CYNC, GM_LEVEL_CASTLE_STATE_CYNC, do_GMCastleStateCync));
	add(new CGMCmd(GM_CMD_TREASURE_MAP_DROP, GM_LEVEL_TREASURE_MAP_DROP, do_GMTreasureMapDrop));
	add(new CGMCmd(GM_CMD_DEATH_EVENT, GM_LEVEL_DEATH_EVENT, do_GMDeathEvent));
	add(new CGMCmd(GM_CMD_ATTENDANCE_CHANGE, GM_LEVEL_ATTENDANCE_CHANGE, do_GMAttendanceChange));
	add(new CGMCmd(GM_CMD_SET, GM_LEVEL_SET, do_GMSet));
	add(new CGMCmd(GM_CMD_CRAFTPOINT, GM_LEVEL_CRAFTPOINT, do_CraftPoint));
	add(new CGMCmd(GM_CMD_EVENTAOUTOMATON, GM_LEVEL_EVENTAOUTOMATON, do_GMEventAutomation));
	add(new CGMCmd(GM_CMD_SKILLLEARN, GM_LEVEL_SKILLLEARN, do_GMSkillLearn));
	add(new CGMCmd(GM_CMD_APET_HUNGRY, GM_LEVEL_APET_HUNGRY, do_GMApetHungry));
	add(new CGMCmd(GM_CMD_APET_FAITH, GM_LEVEL_APET_FAITH, do_GMApetFaith));
	add(new CGMCmd(GM_CMD_COMPARE_BOXITEM, GM_LEVEL_COMPARE_BOXITEM, do_GMCompareBoxItem));
	add(new CGMCmd(GM_CMD_GUILDMASTER_KICK_RESET, GM_LEVEL_GUILD_MASTER_KICK_RESET, do_GMGuildMasterKickReset));
	add(new CGMCmd(GM_CMD_GUILDMASTER_CHANGE, GM_LEVEL_GUILD_MASTER_CHANGE, do_GMGuildChangeBoss));
	add(new CGMCmd(GM_CMD_JOBRESET, GM_LEVEL_JOBRESET, do_GMJobReset));
	add(new CGMCmd(GM_CMD_JOBCHANGE, GM_LEVEL_JOBCHANGE, do_GMJobChange));
	add(new CGMCmd(GM_CMD_ITEMGET, GM_LEVEL_ITEMGET, do_GMItemGet));
	add(new CGMCmd(GM_CMD_ITEMGETRANGE, GM_LEVEL_ITEMGETRANGE, do_GMItemGetRange));
	add(new CGMCmd(GM_CMD_XMAS_PUZZLE, GM_LEVEL_XMAS_PUZZLE, do_GMXMASPuzzle));
	add(new CGMCmd(GM_CMD_ROYAL_RUMBLE_SETTING, GM_LEVEL_ROYAL_RUMBLE_SETTING, doGMRoyalRumbleSetting));
	add(new CGMCmd(GM_CMD_AFFINITY_SET, GM_LEVEL_AFFINITY_SET, do_GMAffinitySet));
	add(new CGMCmd(GM_CMD_AFFINITY_REWARD_INIT, GM_LEVEL_AFFINITY_SET, do_GMAffinityRewardInit));
	add(new CGMCmd(GM_CMD_NAS_SET, GM_LEVEL_NAS_SET, do_GMNasSet));
	add(new CGMCmd(GM_CMD_JEWELRYE, GM_LEVEL_JEWEL_EVENT, do_GMJewelEvent));
#ifdef LC_KOR
	add(new CGMCmd(GM_CMD_CRASH, GM_LEVEL_CRASH, do_Crash));
#endif
	add(new CGMCmd(GM_CMD_NOT_GM, GM_LEVEL_NOT_GM, do_NotGM));
	add(new CGMCmd(GM_CMD_TRADEAGENT_FEE, GM_LEVEL_TRADEAGENT_FEE, do_TradeAgent_Fee));
	add(new CGMCmd(GM_CMD_TRADEAGENT_DEPOSIT, GM_LEVEL_TRADEAGENT_DEPOSIT, do_TradeAgent_Deposit));
	add(new CGMCmd(GM_CMD_GO_ZONE_FINDNPC, 10, do_FindNpcAndGoZone));

	add(new CGMCmd(GM_CMD_STASHGETRANGE, 10, do_StashGetRange));
	add(new CGMCmd(GM_CMD_STASHDEL, 10, do_StashDel));

	add(new CGMCmd(GM_CMD_TIMERITEM_SET, 10, do_GMTimerItemSet));
	add(new CGMCmd(GM_CMD_PICKUP_ITEM, 10, do_PickupItem));
	add(new CGMCmd(GM_CMD_MONSTER_MERCEARY, 10, do_MonsterCard));
	add(new CGMCmd(GM_CMD_VIEW_ALL_SHOP_LIST, 10, do_ViewAllShopList));
	add(new CGMCmd(GM_CMD_DEL_CHARACTER, 10, do_DelCharacter));
	add(new CGMCmd(GM_CMD_LOGOUT, 10, do_Logout));
	add(new CGMCmd(GM_CMD_EXP, 10, do_Exp));
	add(new CGMCmd(GM_CMD_GO_NEXT_PC, 8, do_GoNextPC));
	add(new CGMCmd(GM_CMD_VIRTUAL_REFORM, 10, do_VirtualReform));
	add(new CGMCmd(GM_CMD_ROCK_PAPER_SCISSORS_WIN, 10, do_RockPaperScissorsWin));

	add(new CGMCmd(GM_CMD_RVR_INCREASE_POINT, 10, do_GMIncreaseRVRPoint));
	add(new CGMCmd(GM_CMD_RVR_DECREASE_POINT, 10, do_GMDecreaseRVRPoint));

	add(new CGMCmd(GM_CMD_THEOS_CHECK_REWARD_ITEM, 10, do_GMTheosCheckTime));
	add(new CGMCmd(GM_CMD_THEOS_CHECK_POLLUTION, 10, do_GMTheosChangePollution));
	add(new CGMCmd(GM_CMD_THEOS_CHECK_DIFFICULTY, 10, do_GMTheosChangeDifficulty));

#ifdef DURABILITY
	add(new CGMCmd(GM_CMD_DURABILITY, 10, do_GM_Durability));
#endif
	add(new CGMCmd(GM_CMD_RVR_APPLYSKILL, 10, do_GMRVRApplySkill));
	add(new CGMCmd(GM_CMD_RVR_SHOWJEWELPOINT, 10, do_GMRVRShowJewelPoint));
	add(new CGMCmd(GM_CMD_RVR_SET_USERCOUNT, 10, do_GMRVRSetUserCount));
	add(new CGMCmd(GM_CMD_RVR_SHOW_USERCOUNT, 10, do_GMRVRShowUserCount));
	add(new CGMCmd(GM_CMD_ALL_ITEM_TIME_SET, 10, do_GM_ItemUsed_Set));
	add(new CGMCmd(GM_CMD_GET_EXCHANGE_ITEM, 10, do_GM_GetExchangeItem));
	add(new CGMCmd(GM_CMD_NO_SPECIAL_BOX_COOLTIME, 10, do_GM_NoCoolBoxItem));

#ifdef PREMIUM_CHAR
	add(new CGMCmd(GM_CMD_PRIMIUM_CHAR_RESET, 10, do_GM_PremiumChar_Reset));
	add(new CGMCmd(GM_CMD_PRIMIUM_CHAR_SET_EXPIRE_TIME, 10, do_GM_PremiumChar_set_expire_time));
	add(new CGMCmd(GM_CMD_PRIMIUM_CHAR_SET_JUMP_COUNT, 10, do_GM_PremiumChar_set_jump_count));
	add(new CGMCmd(GM_CMD_PRIMIUM_CHAR_SET_TYPE, 10, do_GM_PremiumChar_set_type));
#endif
	add(new CGMCmd(GM_CMD_ATTENDANCE_ASSURE_EXPIRE_TIME, 10, do_GM_Attendance_assure_set_expire_time));
	add(new CGMCmd(GM_CMD_RAID_CHANGE_MAX_AREA_COUNT, 10, do_raid_changeMaxAreaCount));
	add(new CGMCmd(GM_CMD_POPUP_NOTICE, 9, do_popup_notice));
	add(new CGMCmd(GM_CMD_POPUP_NOTICEOFF, 9, do_popup_noticeoff));

	add(new CGMCmd(GM_CMD_TEST_COMMAND, 9, do_test));

	add(new CGMCmd(GM_CMD_TOGGLE_BUFF, GM_LEVEL_TOGGLE_BUFF, do_buff_system));
}

void CGMCmdList::add(CGMCmd* gmcmd)
{
	// 명령어 길이가 1보다 작으면 리턴
	if ( gmcmd->getCommand().length() < 1 )
		return;

	// 명령어 레벨이 0보다 작으면 리턴
	if ( gmcmd->getLevel() < 0 )
		return;

	// 함수 포인터가 NULL이면 리턴
	if ( gmcmd->getFunction() == NULL )
		return;

	// 등록
	m_GMCmdList.insert(map_t::value_type(gmcmd->getCommand(), gmcmd));
}

void CGMCmdList::run(CPC* pc, const char* arg)
{
	std::string str(arg);
	stokenizer token(str, sep);
	std::vector<std::string> vec;
	vec.assign(token.begin(), token.end());

	if (vec.empty())
		return;

	const char* p = arg;
	char command[512];
	p = AnyOneArg(p, command, true);

	// 존재하는지 검색
	CGMCmd* pGMCmd = find(vec[0]);
	if ( pGMCmd == NULL )
		return;

	// 실행할 수 있는지 검색
	if ( !canRun( pGMCmd, pc ) )
		return;

	GAMELOG << init("GM COMMAND")
			<< command << delim << p << delim
			<< pc->m_index << delim
			<< pc->m_name << delim
			<< pc->m_nick << delim
			<< end;

	// 실행
	vec.erase(vec.begin());
	pGMCmd->run(pc, p, vec);
}

CGMCmd* CGMCmdList::find(std::string& command)
{
	map_t::iterator it = m_GMCmdList.find(command);
	return (it != m_GMCmdList.end()) ? it->second : NULL;
}

bool CGMCmdList::canRun(CGMCmd* gmcmd, CPC* pc)
{
	if ( pc->m_admin < gmcmd->getLevel() )
		return false;

	return true;
}

