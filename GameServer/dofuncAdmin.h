#ifndef __DO_FUNC_ADMIN_H__
#define __DO_FUNC_ADMIN_H__

void do_GMEcho(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMGoZone(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMGoto(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMItemDrop(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMCountPC(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMLoopCount(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMImmortal(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMShutdown(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMReboot(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMCountNPC(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMVisible(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMGoPC(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMGoNPC(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMLevelUP(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMKICK(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMComon(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMSummon(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMSilence(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMWhereAmI(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMQuestComplete(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMStatPoint(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMSkillPoint(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMSetRegenSec(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMNPCRegen(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMKillNPC(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMExpUp(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMBloodPoint(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMEndGame(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMObserve(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMChatMonitor(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMDoubleEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMDoubleExpEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMLattoEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMNewYearEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMValentineEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMWhiteDayEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMReloadGMIP(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMSpeedUp(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMFameUp(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMPD4(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMKickID(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMResurrection(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_GMRecommend(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_GMSetLimit(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMDisguise(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMLoginServer(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_OTAKUSpeedUp(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_OTAKUImmortal(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMCalTime(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMKickCharIndex(CPC* ch, const char* arg, std::vector<std::string>& vec );
void do_GMEventShow(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMEventSetting(CPC* ch, const char* arg, std::vector<std::string>& vec );
void do_GMChanceEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMToolChatMonitor(CPC* ch, const char* arg, std::vector<std::string>& vec );
void do_GMDoubleItemEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMDoublePetExpEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMDoubleAttackEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMDropItemEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMSilenceMulti(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMKickMulti(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMExpLimit(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMDoubleEventAuto(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMUpgradeEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMKickGuildMember(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMApplySkill(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMStateFlag(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMDate(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMPlayTrigger(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMBattleStart(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMBattleStop(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMResetRaid(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMSetSealExp(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMEndSubHelper(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_CraftPoint(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMEventAutomation(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMSkillLearn(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMApetHungry(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMApetFaith(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMCompareBoxItem(CPC* ch, const char* arg, std::vector<std::string>& vec);

void do_GMMyAdmin(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMDTTimeSet(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMDTExpSet(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMDTTimeInterval(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMDTToggle(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMReloadCatalog( CPC* ch, const char* arg, std::vector<std::string>& vec );
//void do_GMRaidTodo(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMNPCDrop(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMGodBless(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMEnchant(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMModifyCashShop(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMLCBallReset(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMAttEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMFunc(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMEchoNpc(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMCastleStateCync(CPC* ch, const char* arg, std::vector<std::string>& vec );
void do_GMGuildMasterKickReset(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMGuildChangeBoss(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMJobReset(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMJobChange(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMItemGet(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMItemGetRange(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMXMASPuzzle(CPC* ch, const char* arg, std::vector<std::string>& vec);
void doGMRoyalRumbleSetting(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMAffinitySet(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMAffinityRewardInit(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMQuestInit(CPC* cp, const char* arg, std::vector<std::string>& vec);
void do_GMNasSet(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_GMJewelEvent(CPC *ch, const char* arg, std::vector<std::string>& vec);
#ifdef LC_KOR
void do_Crash(CPC *ch, const char* arg, std::vector<std::string>& vec);
#endif
void do_NotGM(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_TradeAgent_Fee(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_TradeAgent_Deposit(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_FindNpcAndGoZone(CPC *ch, const char* arg, std::vector<std::string>& vec);

void do_StashGetRange(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_StashDel(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_GMTimerItemSet(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_PickupItem(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_MonsterCard(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_ViewAllShopList(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_DelCharacter(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_Logout(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_Exp(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_GoNextPC(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_VirtualReform(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_RockPaperScissorsWin(CPC *ch, const char* arg, std::vector<std::string>& vec);

void do_GMIncreaseRVRPoint(CPC *ch, const char* arg, std::vector<std::string>& vec);
void do_GMDecreaseRVRPoint(CPC *ch, const char* arg, std::vector<std::string>& vec);

//테오스 무덤 관련
void do_GMTheosCheckTime(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMTheosChangePollution(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMTheosChangeDifficulty(CPC* ch, const char* arg, std::vector<std::string>& vec);

// 이벤트
void do_GMEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);

void do_GMTreasureMapDrop(CPC* ch, const char* arg, std::vector<std::string>& vec );

void do_GMDeathEvent(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMAttendanceChange(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMSet(CPC* ch, const char* arg, std::vector<std::string>& vec);

#ifdef DURABILITY
void do_GM_Durability(CPC* ch, const char* arg, std::vector<std::string>& vec);
#endif

//////////////////////////////////////////////////////////////////////////
void do_GM_hpmp(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_hp(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_mp(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_guildPoint(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_eventCollect(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_block(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_release(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_coupon(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_promotion(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_setWarTime(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_setWarTime2(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_setWarTimeEnd(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_setWarTime2End(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_loadhp5(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_guildCubeStart(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_guildCubeEnd(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_guildCubeState(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_guildCubeTimeCheck(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_setOwner(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_lordChat(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_createPet(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_apetStat(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_apetHPMP(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_apetExp(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_apetLevelUp(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_apetUse(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_apetCoolTime(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_apetAccExp(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_svrInfo(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_ep(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_rankRefresh0831(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_petLevelUp(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_petDeadTime(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_petReset(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_petExpUp(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_petMountReset(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_petChangeName(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_pkCount(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_pkPenalty(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_showHP(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_goPOS(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_pdNPC(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_cashBalance(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_cashPurchase(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_cashPurchaseList(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_cashPurchaseHistory(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_skillLearn(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMRVRApplySkill(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMRVRShowJewelPoint(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMRVRSetUserCount(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GMRVRShowUserCount(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_ItemUsed_Set(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_GetExchangeItem(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_NoCoolBoxItem(CPC* ch, const char* arg, std::vector<std::string>& vec);

#ifdef PREMIUM_CHAR
void do_GM_PremiumChar_Reset(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_PremiumChar_set_expire_time(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_PremiumChar_set_jump_count(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_GM_PremiumChar_set_type(CPC* ch, const char* arg, std::vector<std::string>& vec);
#endif

void do_GM_Attendance_assure_set_expire_time(CPC* ch, const char* arg, std::vector<std::string>& vec);

void do_raid_changeMaxAreaCount(CPC* ch, const char* arg, std::vector<std::string>& vec);

void do_popup_notice(CPC* ch, const char* arg, std::vector<std::string>& vec);
void do_popup_noticeoff(CPC* ch, const char* arg, std::vector<std::string>& vec);

void do_test(CPC* ch, const char* arg, std::vector<std::string>& vec);

// More Illusionist stuff here
void do_buff_system( CPC* ch, const char* arg, std::vector<std::string>& vec );

#endif

