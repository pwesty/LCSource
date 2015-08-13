#ifndef __DOFUNCTION_H__
#define __DOFUNCTION_H__

// 채팅 관련
void do_Chat(CPC* ch, CNetMsg::SP& msg);

// 이동 관련
void do_Move(CPC* ch, CNetMsg::SP& msg);
void do_GoZone(CPC* ch, CNetMsg::SP& msg);
void do_Warp(CPC* ch, CNetMsg::SP& msg);

// 공격 관련
void do_Attack(CPC* ch, CNetMsg::SP& msg);

// N:1 Attack  & move in Personal dungeon
//N1InPersonalDungeon BW

void do_pd_Attack(CPC* ch, CNetMsg::SP& msg);
void do_pd_Move(CPC* ch, CNetMsg::SP& msg);

// 아이템 관련
void do_Item(CPC* ch, CNetMsg::SP& msg);
void do_ItemUse(CPC* ch, CNetMsg::SP& msg, bool bprolong = false);
void do_ItemTake(CPC* ch, CNetMsg::SP& msg);
void do_ItemThrow(CPC* ch, CNetMsg::SP& msg);
void do_ItemDelete(CPC* ch, CNetMsg::SP& msg);
void do_ItemWear(CPC* ch, CNetMsg::SP& msg);
void do_ItemWearTakeOff(CPC* ch, CNetMsg::SP& msg);
void do_ItemSwap(CPC* ch, CNetMsg::SP& msg);
void do_ItemBuy(CPC* ch, CNetMsg::SP& msg);
void do_ItemSell(CPC* ch, CNetMsg::SP& msg);
void do_ItemUpgradeReq(CPC* ch, CNetMsg::SP& msg);
void do_ItemRefinReq(CPC* ch, CNetMsg::SP& msg);
void do_ItemOptionAddReq(CPC* ch, CNetMsg::SP& msg);
void do_ItemOptionDelReq(CPC* ch, CNetMsg::SP& msg);
void do_ItemProcessReq(CPC* ch, CNetMsg::SP& msg);
void do_ItemMakeReq(CPC* ch, CNetMsg::SP& msg);
void do_ItemMixReq(CPC* ch, CNetMsg::SP& msg);
void do_ItemArcaneReq(CPC* ch, CNetMsg::SP& msg);
void do_ItemChangeWeaponReq(CPC* ch, CNetMsg::SP& msg);
void do_ItemProcessNPC(CPC* ch, CNetMsg::SP& msg);
void do_ItemAddBooster(CPC* ch, CNetMsg::SP& msg);
void do_ItemMixWarItem(CPC* ch, CNetMsg::SP& msg);

void do_itemUse_PressCorps(CPC* ch, CNetMsg::SP& msg);

void do_itemCheck_Composition(CPC* ch, CNetMsg::SP& msg);

void do_ItemTarget(CPC* ch, CNetMsg::SP& msg);

void do_ItemWear_Costume(CPC* ch, CNetMsg::SP& msg);
void do_Itemwear_Costume_TakeOff(CPC* ch, CNetMsg::SP& msg);

// [110207:selo] 한 벌 의상
void do_ItemWear_Costume_Suit(CPC* ch, CNetMsg::SP& msg);
void do_ItemWear_Costuem_Suit_TakeOff(CPC* ch, CNetMsg::SP& msg);
void do_Item_Make_Fortune(CPC* ch, CNetMsg::SP& msg);

void do_ItemUse_MasterStone_USA(CPC* ch, CNetMsg::SP& msg);

void do_Ex_CashItem(CPC* ch, CNetMsg::SP& msg);
int usePeriodItem(int* useTime, bool* bprolong);
int usePeriodItem_7(int* useTime, bool* bprolong);
// 상품 가져오기와, 선물 가져오기, 같은 프로세스를 사용함,
// bPresent가 트루이면 선물
void CashItemBringProcess(CPC* ch, CNetMsg::SP& msg, bool bPresent);

void CashItemListProcess(CPC* ch, int SearchKey, int ReqPage);
void CashItemRecommandProcess(CPC* ch, int category);

// 교환 관련
void do_Exchange(CPC* ch, CNetMsg::SP& msg);
void do_ExchangeReq(CPC* ch, CNetMsg::SP& msg);
void do_ExchangeItem(CPC* ch, CNetMsg::SP& msg);
void do_ExchangeReqReq(CPC* ch, CNetMsg::SP& msg);
void do_ExchangeReqRep(CPC* ch, CNetMsg::SP& msg);
void do_ExchangeReqReject(CPC* ch, CNetMsg::SP& msg);
void do_ExchangeReqReady(CPC* ch, CNetMsg::SP& msg);
void do_ExchangeReqOK(CPC* ch, CNetMsg::SP& msg);
void do_ExchangeItemAdd(CPC* ch, CNetMsg::SP& msg);
void do_ExchangeItemDel(CPC* ch, CNetMsg::SP& msg);

// 장소 기억 관련
void do_MemPos(CPC* ch, CNetMsg::SP& msg);

// 펄스 관련
void do_Pulse(CPC* ch, CNetMsg::SP& msg);

#ifdef DISCONNECT_HACK_CHARACTER
void do_Disconnect_Hack_Character(CPC* ch, CNetMsg::SP& msg);
#endif // DISCONNECT_HACK_CHARACTER

// 운영자 명령 관련
void do_GM(CPC* ch, CNetMsg::SP& msg);

bool ParsingTime(const char*& arg, int start_dt[], int end_dt[]);

// 파티 관련
void do_Party(CPC* ch, CNetMsg::SP& msg);
void do_PartyInvite(CPC* ch, CNetMsg::SP& msg);
void do_PartyAllow(CPC* ch, CNetMsg::SP& msg);
void do_PartyReject(CPC* ch, CNetMsg::SP& msg);
void do_PartyQuit(CPC* ch, CNetMsg::SP& msg);
void do_PartyKick(CPC* ch, CNetMsg::SP& msg);
void do_PartyAssistInfo(CPC* ch, CNetMsg::SP& msg);
void do_PartyChangeBoss(CPC* ch, CNetMsg::SP& msg);
void do_PartyChangeType(CPC* ch, CNetMsg::SP& msg);

void do_PartyEndPartyReq(CPC* ch, CNetMsg::SP& msg);

void do_PartyOffline(CPC* ch, CNetMsg::SP& msg);

// 퀵슬롯 관련
void do_QuickSlot(CPC* ch, CNetMsg::SP& msg);

// 싱글던전 - NPC 리젠 관련
void do_NPCRegen(CPC* ch, CNetMsg::SP& msg);

// 스킬 관련
void do_Skill(CPC* ch, CNetMsg::SP& msg);

// 부활 메시지
void do_Rebirth(CPC* ch, CNetMsg::SP& msg);

// 액션 메시지
void do_Action(CPC* ch, CNetMsg::SP& msg);

// 퀘스트 관련
void do_Quest(CPC* ch, CNetMsg::SP& msg);
void do_QuestRequest(CPC* ch, CNetMsg::SP& msg);
bool do_QuestStart(CPC* ch, CNetMsg::SP& msg);
void do_QuestGiveUp(CPC* ch, CNetMsg::SP& msg);
void do_QuestPrize(CPC* ch, CNetMsg::SP& msg);
void do_QuestComplete(CPC* ch, CNetMsg::SP& msg);
void do_QuestCollect(CPC* ch, CNetMsg::SP& msg);
void do_QuestRestoreAbandon(CPC* ch, CNetMsg::SP& msg);
void do_QuestItemReq(CPC* ch, CNetMsg::SP& msg);
CItem* OptionSettingItem(CPC* ch, CItem* item);

void do_QuestPD4RankView(CPC* ch, CNetMsg::SP& msg);
void do_QuestPD4RankRewardView(CPC* ch, CNetMsg::SP& msg);
void do_QuestPD4RankReward(CPC* ch, CNetMsg::SP& msg);

// 특수스킬 관련
void do_SSkill(CPC* ch, CNetMsg::SP& msg);

// PK 관련
void do_PK(CPC* ch, CNetMsg::SP& msg);

// 이벤트 관련
void do_Event(CPC* ch, CNetMsg::SP& msg);

// 개인상점 관련
void do_PersonalShop(CPC* ch, CNetMsg::SP& msg);
void do_PersonalShopSellStart(CPC* ch, CNetMsg::SP& msg);
void do_PersonalShopSellList(CPC* ch, CNetMsg::SP& msg);
void do_PersonalShopChange(CPC* ch, CNetMsg::SP& msg);
void do_PersonalShopBuy(CPC* ch, CNetMsg::SP& msg);

// 창고 관련
void do_Stash(CPC* pc, CNetMsg::SP& msg);

/////////////////////////////////////////////
// BANGWALL : 2005-07-05 오후 2:37:20
// Comment : 테섭적용
void do_StashDeletePassword(CPC* ch, CNetMsg::SP& msg);
void do_StashCheckPassword(CPC* ch, CNetMsg::SP& msg);
void do_StashSetNewPassword(CPC* ch, CNetMsg::SP& msg);

// 길드 관련
void do_Guild(CPC* ch, CNetMsg::SP& msg);
void do_GuildCreate(CPC* ch, CNetMsg::SP& msg);
void do_GuildLevelUp(CPC* ch, CNetMsg::SP& msg);
void do_GuildBreakUp(CPC* ch, CNetMsg::SP& msg);
void do_GuildRegistReq(CPC* ch, CNetMsg::SP& msg);
void do_GuildRegistAllow(CPC* ch, CNetMsg::SP& msg);
void do_GuildRegistCancel(CPC* ch, CNetMsg::SP& msg);
void do_GuildOutReq(CPC* ch, CNetMsg::SP& msg);
void do_GuildKick(CPC* ch, CNetMsg::SP& msg);
void do_GuildChangeBoss(CPC* ch, CNetMsg::SP& msg);
void do_GuildAppointOfficer(CPC* ch, CNetMsg::SP& msg);
void do_GuildFireOfficer(CPC* ch, CNetMsg::SP& msg);
void do_GuildBattleReqReq(CPC* ch, CNetMsg::SP& msg);
void do_GuildBattleReqReject(CPC* ch, CNetMsg::SP& msg);
void do_GuildBattleReqAccept(CPC* ch, CNetMsg::SP& msg);
void do_GuildBattleStopReq(CPC* ch, CNetMsg::SP& msg);
void do_GuildBattleStopReject(CPC* ch, CNetMsg::SP& msg);
void do_GuildBattleStopAccept(CPC* ch, CNetMsg::SP& msg);
void do_GuildWarSetTimeReq(CPC* ch, CNetMsg::SP& msg);
void do_GuildWarGetTime(CPC* ch, CNetMsg::SP& msg);
void do_GuildWarJoinAttackGuild(CPC* ch, CNetMsg::SP& msg);
void do_GuildWarJoinDefenseGuild(CPC* ch, CNetMsg::SP& msg);
void do_GuildWarJoinAttackChar(CPC* ch, CNetMsg::SP& msg);
void do_GuildWarJoinDefenseChar(CPC* ch, CNetMsg::SP& msg);
void do_GuildWarAttackSymbol(CPC* ch, CNetMsg::SP& msg);
void do_GuildStashHistoryReq(CPC* ch, CNetMsg::SP& msg);
void do_GuildStashViewReq(CPC* ch, CNetMsg::SP& msg);
void do_GuildStashTakeReq(CPC* ch, CNetMsg::SP& msg);

void do_GuildInclineEstablist( CPC* ch, CNetMsg::SP& msg );
void do_GuildMemberAdjust( CPC* ch, CNetMsg::SP& msg );
void do_NewGuildInfo( CPC* ch, CNetMsg::SP& msg );
void do_NewGuildMemberList( CPC* ch, CNetMsg::SP& msg );
void do_NewGuildManage( CPC* ch, CNetMsg::SP& msg );
void do_NewGuildNotice( CPC* ch, CNetMsg::SP& msg );
void do_NewGuildNoticeUpdate( CPC* ch, CNetMsg::SP& msg );
void do_NewGuildNoticeTrans( CPC* ch, CNetMsg::SP& msg );
void do_NewGuildSkillList( CPC* ch, CNetMsg::SP& msg );
void do_NewGuildSkillLearn( CPC* ch, CNetMsg::SP& msg );

#ifdef DEV_GUILD_MARK
void do_newGuildSkillMarkEdit(CPC* ch, CNetMsg::SP& msg);
void do_newGuildSkillMarkEditFinish(CPC* ch, CNetMsg::SP& msg);
#endif

#ifdef DEV_GUILD_STASH
void do_newGuildStashList( CPC* ch, CNetMsg::SP& msg );
void do_newGuildStashKeep( CPC* ch, CNetMsg::SP& msg );
void do_newGuildStashTake( CPC* ch, CNetMsg::SP& msg );
void do_newGuildStashLog( CPC* ch, CNetMsg::SP& msg );
void do_newGuildStashKeepMoney( CPC* ch, CNetMsg::SP& msg);
void do_newGuildStashTakeMoney( CPC* ch, CNetMsg::SP& msg);
void do_newGuildStashSearchMoneyRep( CPC* ch, CNetMsg::SP& msg);
void do_newGuildStashSearchMoneyReq( CPC* ch, CNetMsg::SP& msg);
#endif // DEV_GUILD_STASH

void do_GuildMasterKickReq( CPC* ch, CNetMsg::SP& msg );
void do_GuildMasterKickCancelReq( CPC* ch, CNetMsg::SP& msg );

#ifdef GUILD_REMOTE_INVITE
void do_GuildRemoteJoinReq( CPC* ch, CNetMsg::SP& msg );
void do_GuildRemoteJoinOK( CPC* ch, CNetMsg::SP& msg );
void do_GuildRemoteJoinNO( CPC* ch, CNetMsg::SP& msg );
int do_CheckGuildJoin(CPC* _guildMaster, CPC* _joinCharacter);
#endif // GUILD_REMOTE_INVITE

void do_GuildRecall(CPC* pc, CNetMsg::SP& rmsg);

// 변신 관련
void do_Change(CPC* ch, CNetMsg::SP& msg);

// 클라이언트 UI 관련
void do_UI(CPC* ch, CNetMsg::SP& msg);

// 사제 시스템 관련
void do_Teach(CPC* ch, CNetMsg::SP& msg);
void do_TeachTeacherList(CPC* ch, CNetMsg::SP& msg);
void do_TeachTeacherReqReq(CPC* ch, CNetMsg::SP& msg);
void do_TeachTeacherReqReject(CPC* ch, CNetMsg::SP& msg);
void do_TeachTeacherReqAccept(CPC* ch, CNetMsg::SP& msg);
void do_TeachInfo(CPC* ch, CNetMsg::SP& msg);
void do_TeachTeacherGiveUP(CPC* ch, CNetMsg::SP& msg);
void do_TeachSetCancel(CPC* teacher, CPC* student);
void do_TeachSetCancel(CPC* teacher, int studentidx);
void do_TeacherGift(CPC* ch);

// 전직 관련
void do_ChangeJob(CPC* ch, CNetMsg::SP& msg);

// 성장포인트 관련
void do_StatPoint(CPC* ch, CNetMsg::SP& msg);
void do_StatPointUse(CPC* ch, CNetMsg::SP& msg);
void do_StatPointReset(CPC* ch, CNetMsg::SP& msg);

void do_Friend(CPC* ch, CNetMsg::SP& msg);
void do_FriendSetCondition(CPC* ch, CNetMsg::SP& msg);
void do_FriendRegistRequest(CPC* ch, CNetMsg::SP& msg);
void do_FriendRegistAllow(CPC* ch, CNetMsg::SP& msg);
void do_FriendRegistRefuse(CPC* ch, CNetMsg::SP& msg);
void do_FriendDeleteMember(CPC* ch, CNetMsg::SP& msg);
void do_FriendDeleteBlock(CPC* ch, CNetMsg::SP& msg);

// 선택 가공
void do_SelectProduce(CPC* ch, CNetMsg::SP& msg);

// 확장 메시지 처리
void do_Extend(CPC* ch, CNetMsg::SP& msg);

void do_Ex_NameChange(CPC* ch, CNetMsg::SP& msg);

void do_ExPetCall(CPC* ch, CNetMsg::SP& msg);
void do_ExPetLearn(CPC* ch, CNetMsg::SP& msg);
void do_ExPetResetSkill(CPC* ch, CNetMsg::SP& msg);
void do_ExPetChangeMount(CPC* ch, CNetMsg::SP& msg);
void do_ExPetCommand(CPC* ch, CNetMsg::SP& msg);
void do_ExPetMixItem(CPC* ch, CNetMsg::SP& msg);
void do_ExPetWarpTown(CPC* ch, CNetMsg::SP& msg);
void do_ExPetChangeItem(CPC* ch, CNetMsg::SP& msg);
void do_ExPetRebirth(CPC* ch, CNetMsg::SP& msg);
void do_PetNameChange( CPC* ch, CNetMsg::SP& msg );
void do_RandomProduce(CPC* ch, CNetMsg::SP& msg);
// 공성맵 관련
void do_ExCastleMapRecent(CPC* ch, CNetMsg::SP& msg);
void do_ExCastleMapSignal(CPC* ch, CNetMsg::SP& msg);

void do_ExPartyRecall(CPC* ch, CNetMsg::SP& msg);

void do_ExPartyMatch(CPC* ch, CNetMsg::SP& msg);

void do_ExCastleWar(CPC * ch, CNetMsg::SP& msg);
void do_ExCastleCristalRespondStart(CPC * ch, CNetMsg::SP& msg);
void do_ExCastleCristalRespondEnd(CPC * ch, CNetMsg::SP& msg);
void do_ExCastleTowerList(CPC * ch, CNetMsg::SP& msg);
void do_ExcastleTowerAdd(CPC * ch, CNetMsg::SP& msg);
void do_ExCastleTowerReinforce(CPC * ch, CNetMsg::SP& msg);
void do_ExCastleTowerReinforceList(CPC * ch, CNetMsg::SP& msg);
void do_ExCastleTowerRepaire(CPC * ch, CNetMsg::SP& msg);
void do_ExCastleTowerRepaireList(CPC * ch, CNetMsg::SP& msg);
void do_ExCastleTowerWarpList(CPC * ch, CNetMsg::SP& msg);
void do_ExCastleQuartersInstall(CPC * ch, CNetMsg::SP& msg);

void do_DVD(CPC * ch, CNetMsg::SP& msg);
void do_dvdInfo(CPC * ch, CNetMsg::SP& msg);
void do_dvdManagement( CPC * ch, CNetMsg::SP& msg );
void do_dvdDungenEnter( CPC *ch, CNetMsg::SP& msg );

void do_MonsterCombo(CPC* ch, CNetMsg::SP& msg);

void do_ClientRestart( CPC* ch, CNetMsg::SP& msg);

#ifdef CHAOSBALL
void do_ChaosBall(CPC* ch, CNetMsg::SP& msg );
#endif

void do_AttackPet(CPC* ch, CNetMsg::SP& msg );

#ifdef EXTREME_CUBE
void do_ExExtremeCube(CPC* ch, CNetMsg::SP& msg);
#endif // EXTREME_CUBE

void do_ExInitSSkill(CPC* ch, CNetMsg::SP& msg);

void do_TradeAgent(CPC* ch, CNetMsg::SP& msg);

void do_TradeAgent_RegListReq(CPC* ch, CNetMsg::SP& msg);
void do_TradeAgent_RegReq(CPC* ch, CNetMsg::SP& msg);
void do_TradeAgent_RegCancelReq(CPC* ch, CNetMsg::SP& msg);
void do_TradeAgent_SearchReq(CPC* ch, CNetMsg::SP& msg);
void do_TradeAgent_BuyReq(CPC* ch, CNetMsg::SP& msg);
void do_TradeAgent_LikeListReq(CPC* ch, CNetMsg::SP& msg);
void do_TradeAgent_LikeRegReq(CPC* ch, CNetMsg::SP& msg);
void do_TradeAgent_LikeCancel(CPC* ch, CNetMsg::SP& msg);
void do_TradeAgent_Error(CPC* ch, CNetMsg::SP& msg);

void do_Expedition(CPC* ch, CNetMsg::SP& msg);

void do_ExpedCreateReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedInviteReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedAllowReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedRejectReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedQuitReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedEndExpedReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedKickReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedChangeTypeReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedChangeBossReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedSetMBossReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedResetMBossReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedMoveGroupReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedAddMemberReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedViewDetailReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedSetLabelReq(CPC* ch, CNetMsg::SP& msg);
void do_ExpedSearchItemReq(CPC* ch, CNetMsg::SP& msg);
void SearchTriggerItem(CPC* FromChar, CPC* BossChar, bool& DidCollect, bool& UsualProcess);
void GiveTriggerItemMemberToBoss(CPC* FromChar, CPC* BossChar, item_search_pair_t& p, bool& DidCollect, bool& UsualProcess);
void do_ExpendOffline(CPC* ch, CNetMsg::SP& msg);

void do_Factory(CPC* ch, CNetMsg::SP& msg);
void do_FactoryLearn(CPC* ch, CNetMsg::SP& msg, bool GMCommand=false);
void do_FactoryList(CPC* ch, CNetMsg::SP& msg);
void do_FactoryMakeReq(CPC* ch, CNetMsg::SP& msg);
void do_FactoryMake(CPC* ch, CNetMsg::SP& msg);

void do_TriggerEvent(CPC* ch, CNetMsg::SP& msg);
void do_TriggerEvent_ItemComposition(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);
void do_TriggerEvent_ItemDrop(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);
void do_TriggerEvent_OpenTheDoor(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);
void do_TriggerEvent_StartRaid(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);
void do_TriggerEvent_MessagePrint(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);
void do_TriggerEvent_SummonNpc(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);
void do_TriggerEvent_UseDebuf(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);
void do_TriggerEvent_ActiveObject(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);
void do_TriggerEvent_RemoveNpc(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);
void do_TriggerEvent_TimeLimit(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);
void do_TriggerEvent_QuestComplete(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);
void do_TriggerEvent_ItemGetInven(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);
void do_TriggerEvent_ConsecutiveDialogue(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);
void do_TriggerEvent_ActiveEffect(CPC* ch, CNetMsg::SP& msg, CTrigger* trigger);

void do_Raid(CPC* ch, CNetMsg::SP& msg);

void do_Raid_InzoneJoin(CPC* ch, CNetMsg::SP& msg);
void do_Raid_InzoneQuit(CPC* ch, CNetMsg::SP& msg);

void do_Affinity(CPC* ch, CNetMsg::SP& msg);

void do_NpcPortalScroll(CPC* ch, CNetMsg::SP& msg);
void do_NpcportalScrollGoNPC(CPC* ch, int index, int msgType);

void do_ExRaidInfo(CPC* ch, CNetMsg::SP& msg);

void do_ExTitleSytem(CPC* ch, CNetMsg::SP& msg);
void do_ExTitleSystemListReq(CPC* ch);
void do_ExTitleSystemTitleSelectReq(CPC* ch, CNetMsg::SP& msg);
void do_ExTitleSystemTitleCancelReq(CPC* ch, CNetMsg::SP& msg);
void do_ExTitleSystemTitleDeleteReq(CPC* ch, CNetMsg::SP& msg);

#ifdef LACARETTE_SYSTEM
void do_ExLacaretteSystem(CPC* ch, CNetMsg::SP& msg);
#endif

void do_ExTakeAgainQuestItem(CPC* ch, CNetMsg::SP& msg);

void do_FuncExSocket(CPC* ch, CNetMsg::SP& msg);
void do_ExSocketUpgradeJewelReq(CPC* ch, CNetMsg::SP& msg);
void do_ExSocketUpgradeChaosJewelReq(CPC* ch, CNetMsg::SP& msg);
void do_ExSocketMakeReq(CPC* ch, CNetMsg::SP& msg);
void do_ExSocketCombineJewelReq(CPC* ch, CNetMsg::SP& msg);
void do_ExSocketCleanJewel(CPC* ch, CNetMsg::SP& msg);
void do_ExSocketCleanOneJewel(CPC* ch, CNetMsg::SP& msg);

void do_FuncAttendanceExp ( CPC* ch, CNetMsg::SP& msg );

#ifdef XTRAP
void do_FuncXtrap( CPC* ch, CNetMsg::SP& msg );
#endif //XTRAP

void do_ExRaidScene(CPC* ch, CNetMsg::SP& msg);

void do_ExRaidSceneTodoTouchField(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneTodoKillNPC(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneTodoTakeItem(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneTodoAllInArea(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneTodoNoInArea(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneTodoMoveInArea(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneTodoMoveOutArea(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneTodoCountDown(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneTodoStartRaid(CPC* ch, CNetMsg::SP& msg);

void do_ExRaidSceneTouchField(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneSummonExtra(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneSummonDistrict(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneSummonMBoss(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneSummonBoss(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneGroundEffectOn(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneGroundEffectOff(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneToggleGate(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneTogglePortal(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneCreateItemObject(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneDebuffSkill(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneEndRaid(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidScenePauseArea(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneBuffSkill(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneToggleCamera(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneCountDownStart(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneSavePoint(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneSendObjectState(CPC* ch, int zone);
void do_ExRaidSceneModelHolder(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneNpcSay(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneApplySkill_TO_ch(CPC* ch, CNetMsg::SP& msg);
void do_ExRaidSceneCureSkill_TO_ch(CPC* ch, CNetMsg::SP& msg);

void do_ExFuncTutorialRenewer(CPC* ch, CNetMsg::SP& msg);

#ifdef ENABLE_SUBJOB
void do_ExFuncSubJob( CPC* ch, CNetMsg::SP& msg );
void do_ExSubJobRegist( CPC* ch, CNetMsg::SP& msg );
#endif //ENABLE_SUBJOB

void do_ExFuncCastllan( CPC* ch, CNetMsg::SP& msg );

void do_ExFaceOff(CPC* ch, CNetMsg::SP& msg);

void do_GuildWarSetTimeMenuReq(CPC* ch, CNetMsg::SP& msg);
void do_GuildWarGetTimeUniReq(CPC* ch, CNetMsg::SP& msg);
bool CheckOverlapWarCastle(int wday, int hour, int zone);

void do_ExLCBall( CPC* ch, CNetMsg::SP& msg );

#ifdef JUNO_RENEWAL_MESSAGEBOX
void do_ExMsgBox( CPC* ch, CNetMsg::SP& msg );
#endif // JUNO_RENEWAL_MESSAGEBOX

void do_ExRoyalRumble(CPC* ch, CNetMsg::SP& msg);
void do_ExRoyalRumblePlayerReq(CPC* ch, CNetMsg::SP& msg);
void do_ExRoyalRumblePlayerUnRegReq(CPC* ch, CNetMsg::SP& msg);
void do_ExRoyalRumbleRewardReq(CPC* ch, CNetMsg::SP& msg);
void do_ExRoyalRumblePointReq(CPC* ch, CNetMsg::SP& msg);
void do_ExRoyalRumbleTimeReq(CPC* ch, CNetMsg::SP& msg);
void do_ExRoyalRumbleGoZone(CPC* ch, CNetMsg::SP& msg);

#ifdef GAMIGO_CASH_AUTHENTICATION_CODE_USE
void do_ExCashAuthenticationCode(CPC* ch, CNetMsg::SP& msg);
#endif

#ifdef SYSTEM_TREASURE_MAP
#ifdef SYSTEM_TREASURE_MAP_GAMIGO
void do_ExTreasureMapKeyExchange(CPC* ch, CNetMsg::SP& msg);
#endif
#endif

void do_rankingSystem(CPC* ch, CNetMsg::SP& msg);

#ifdef REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼
void do_ExPKPeneltyReform(CPC* ch, CNetMsg::SP& msg);
#endif

#ifdef NOTICE_SYSTEM
void do_UserNotice(CPC* ch, CNetMsg::SP& msg);
#endif // NOTICE_SYSTEM

void do_ReformSystem(CPC *ch_, CNetMsg::SP& msg);
void do_serverTime(CPC * ch, CNetMsg::SP& msg);

void itemPickGameLog(CPC *pc, CItem* pItem, unsigned char char_type = 0);

void do_ExpressSystem(CPC* ch, CNetMsg::SP& msg);
void do_ReservedGMCommand(CPC* ch, CNetMsg::SP& msg);

void do_PetStash(CPC* ch, CNetMsg::SP& msg);

void do_RVR(CPC* ch, CNetMsg::SP& msg);

void do_GPS(CPC* ch, CNetMsg::SP& msg);
void do_ItemCollection(CPC* pc, CNetMsg::SP& msg);

void do_setTarget(CPC* ch, CNetMsg::SP& msg);

#ifdef PREMIUM_CHAR
void do_PremiumChar(CPC* ch, CNetMsg::SP& msg);
#endif

void do_noticeExtend(CPC* pc, CNetMsg::SP& rmsg);

void do_ReportForced(CPC* pc, CNetMsg::SP& msg);

#endif // #ifndef __DOFUNCTION_H__