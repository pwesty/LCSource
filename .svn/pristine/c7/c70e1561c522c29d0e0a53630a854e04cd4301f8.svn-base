#ifndef __DO_FUNCTION_H__
#define __DO_FUNCTION_H__

#include "../ShareLib/DBCmd.h"

void do_Request(CNetMsg::SP& msg, CDescriptor* dest);
void do_Reply(CNetMsg::SP& msg, CDescriptor* dest);
void do_Command(CNetMsg::SP& msg, CDescriptor* dest);
void do_Command_Struct(CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandShutdown(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildCreateReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildOnline(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildLoadReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildLevelUpReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildBreakUpReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildBreakUpReq_real(CDescriptor* dest, int bossindex, int guildindex);
void do_CommandGuildMemberAddReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildMemberOutReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildMemberOutReq_real(CDescriptor* dest, int guildindex, int charindex, bool bSendError);
void do_CommandGuildMemberKickReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildChangeBossReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildAppointOfficerReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildChat(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildFireOfficerReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandCharacterDelete(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildBattleReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildBattleStopReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildBattlePeaceReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildBattleKillReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandEventMoonStoneUpdateReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandEventMoonStoneJackPotReq(CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandFriendMemberAddReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandFriendMemberDelReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandFriendSetConditionReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandBlockCharReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGiftCharReq(CNetMsg::SP& msg, CDescriptor* dest);

// BW
void do_CommandPD4RankInit(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPD4RankAdd(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPD4RankEnd(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPD4RankView(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPD4RewardView(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPD4Reward(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPD4RewardInit(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPD4RewardEnd(CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandNameChange(CNetMsg::SP& msg, CDescriptor* dest);
// 애완동물
void do_CommandPetCreateReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPetDeleteReq(CNetMsg::SP& msg, CDescriptor* dest);

// 길드 나스 보관함
void do_CommandGuildStashHistoryReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildStashViewReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildStashTakeReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildStashRollback(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildStashSaveTaxReq(CNetMsg::SP& msg, CDescriptor* dest);

// 명성 관련
void do_CommandFameUp(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandTeacherRegister(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandTeacherGiveup(CNetMsg::SP& msg, CDescriptor* dest);
bool do_TeachCancel(CDBCmd cmd, int teacheridx, int studentidx, int cancelflag = CANCELTEACHER | CANCELSTUDENT, bool bTimeLimit = false);
//선생 리스트
void do_CommandTeacher(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandTeacherTimeover(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandTeacherSuperstoneRecieve(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandTeacherLoad(CNetMsg::SP& msg, CDescriptor* dest);

//휴면케릭
void do_CommandEventSearchFriendMemberAddReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandEventSearchFriendMemberSelectAddReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandEventSearchFriendMemberOneTimeCheckReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandEventSearchFriendMemberListGoodReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandEventSearchFriendMemberGoodReq(CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandPartyInviteReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyInviteRep(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyAllowReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyRejectReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyQuitReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyKickReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyChangeBossReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyInviteCancel(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyOffline( CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyOnline( CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandPartyChangeTypeReq(CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandPartyEndPartyReq(CNetMsg::SP& msg, CDescriptor* dest);
//void do_CommandPartyInzoneDataClearReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyBreakReq(CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandPartyMatchRegMemberReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyMatchRegPartyReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyMatchDelReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyMatchDelReq_real(int nSubNo, int nCharIndex);
void do_CommandPartyMatchInviteReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyMatchJoinReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyMatchJoinAllowReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyMatchJoinRejectReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPartyMatchMemberChangeInfoName(CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandGuildInclineEstablish( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandGuildMemberAdjust( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandNewGuildInfo( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandNewGuildMemberList( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandNewGuildManage( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandNewGuildNotice( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandNewGuildNoticeUpdate( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandNewGuildNoticeTrans( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandNewGuildSkillList( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandSaveGuildPoint(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandSaveGuildMemberPoint(CNetMsg::SP& msg, CDescriptor* dest);
///void do_CommandNewGuildSaveSkill(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandNewGuildUpdatePoint( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandGuildNotice( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandGuildSkillLearn( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandGuildSkillLearnSendMember( CNetMsg::SP& msg );

void do_InsertGuard(CNetMsg::SP& msg, CDescriptor * dest);
void do_CastleTowerAdd(CNetMsg::SP& msg, CDescriptor * dest);
void do_CastleTowerReinforce(CNetMsg::SP& msg, CDescriptor * dest);
void do_CastleQuartersInstall(CNetMsg::SP& msg, CDescriptor * dest);

void do_CommandPetChangeName( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandPetColorChange( CNetMsg::SP& msg, CDescriptor* dest );

void do_CommandHalloween2007( CNetMsg::SP& msg, CDescriptor* dest );

void do_CommandDVDRateChange( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandDVDNormalChangeNotice( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandDVDNormalChangeTime( CNetMsg::SP& msg, CDescriptor* dest );

void do_CommandPetTurnToNpc( CNetMsg::SP& msg, CDescriptor* dest );

void do_CommandCreateCombo( CNetMsg::SP& msg, CDescriptor* dest );

void do_CommandAttackPet( CNetMsg::SP& msg, CDescriptor* dest );

#ifdef EXTREME_CUBE
void do_CommandCubeStateReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandCubeStatePersonalReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandAddCubePoint( CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandAddCubePointPersonal( CNetMsg::SP& msg, CDescriptor* dest );
#ifdef EXTREME_CUBE_VER2
void do_CommandCubeRewardPersonalRollBack(CNetMsg::SP& msg, CDescriptor* dest );
void do_CommandCubeRewardPersonalReq(CNetMsg::SP& msg, CDescriptor* dest );
#endif // EXTREME_CUBE_VER2
#endif // EXTREME_CUBE

void do_CommandEventPhoenix( CNetMsg::SP& msg, CDescriptor* dest );

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
void do_Command_DisconnectHackCharacter( CNetMsg::SP& msg, CDescriptor* dest );
#endif // DISCONNECT_HACK_CHARACTER

void do_Command_Exped_CreateReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Exped_CreateResult( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Exped_InviteReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Exped_AllowReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Exped_RejectReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Exped_QuitReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Exped_KickReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Exped_MemberInfoReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Exped_ChangeTypeReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Exped_ChangeBossReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Exped_SetMBossReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Exped_ResetMBossReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Exped_EndExpedReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Exped_MoveGroupReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Expend_OnlineSystem( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Expend_OfflineSystem( CNetMsg::SP& msg, CDescriptor* dest );

void do_Command_Raid_InzoneGetRoomNoReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Raid_InzoneDataClearReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_Raid_InzoneSetRoomNo ( CNetMsg::SP& msg , CDescriptor* dest );

void do_CommandTriggerEvent( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_DeleteRaidCharacter( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_RaidInfo( CNetMsg::SP& msg, CDescriptor* dest );

void do_CommandNSCreateCard(CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandPartyDataInit(CNetMsg::SP& msg, CDescriptor* dest);

#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS)|| defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE)
void do_Worldcup2010Event(CNetMsg::SP& msg, CDescriptor* dest);
void do_Worldcup2010EventToto(CNetMsg::SP& msg, CDescriptor* dest);
void do_Worldcup2010EventTotoStatus(CNetMsg::SP& msg, CDescriptor* dest);
void do_Worldcup2010EventTotoGift(CNetMsg::SP& msg, CDescriptor* dest);
void do_Worldcup2010EventAttendance(CNetMsg::SP& msg, CDescriptor* dest);
void do_Worldcup2010EventKorea(CNetMsg::SP& msg, CDescriptor* dest);
#endif // defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)

void do_CommandUseGuildPoint(CNetMsg::SP& msg, CDescriptor* dest);

void do_TeachRenewer(CNetMsg::SP& msg, CDescriptor* dest);
void do_TeachRenewerLimitTime(CNetMsg::SP& msg, CDescriptor* dest);
void do_TeachRenewerTeacherGiftAdd(CNetMsg::SP& msg, CDescriptor* dest);
void do_TeachRenewerTeacherGift(CNetMsg::SP& msg, CDescriptor* dest);
void do_TeachRenewerStuedntSuc(CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandFaceOff(CNetMsg::SP& msg, CDescriptor* dest);

#ifdef EVENT_CHAR_BIRTHDAY
void do_commandEventCharBirthday(CNetMsg::SP& msg, CDescriptor* dest);
#endif

#ifdef DEV_GUILD_MARK
void do_CommandGuildMarkRegist(CNetMsg::SP& msg, CDescriptor* dest);
#endif

void do_CommandAutoResetRaid(CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandRoyalRumbleLog(CNetMsg::SP& msg, CDescriptor* dest);

#ifdef DEV_GUILD_STASH
void do_CommandGuildStashCreate(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildStashList(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildStashKeep(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildStashKeepError(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildStashTake(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildStashTakeError(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildStashLog(CNetMsg::SP& msg, CDescriptor* dest);
#endif // DEV_GUILD_STASH

#ifdef GUILD_MEMBER_KICK_JOIN_DELAY
void do_CommandGuildMemberKickOutDateUpdate(CNetMsg::SP& msg, CDescriptor* dest);
#endif

void do_CommandTeacherStudentListCync(CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandGuildMasterKickReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandGuildMasterKickCancelReq(CNetMsg::SP& msg, CDescriptor* dest);
bool do_GuildBossChange(int _guildIndex, int _newBoss);
void do_GuildStatusChange(int _guildIndex, int _status);
void do_CommandGuildMasterKickReset(CNetMsg::SP& msg, CDescriptor* dest);
void  do_CommandGuildMasterChange(CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandEventTableInit(CNetMsg::SP& msg, CDescriptor* dest);

void do_CommandRVRIncreaseJewel(CNetMsg::SP& msg);
void do_CommandRVRDecreaseJewel(CNetMsg::SP& msg);
void do_CommandRVRJoinReq(CNetMsg::SP& msg);
void do_CommandRVRLeaveReq(CNetMsg::SP& msg);
void do_CommandRVRInfo(CNetMsg::SP& msg);
void do_CommandRVRChangeKingInfo(CNetMsg::SP& msg);
void do_CommandRVRChangeKingInfoEach(CNetMsg::SP& msg);
void do_CommandRVRAdd2ndGradeUser(CNetMsg::SP& msg);
void do_CommandRVRDel2ndGradeUser(CNetMsg::SP& msg);
void do_CommandRVRUpdate2ndGradeUser(CNetMsg::SP& msg);
void do_CommandRVRSetUserCount(CNetMsg::SP& msg);

#endif //__DO_FUNCTION_H__
