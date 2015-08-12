#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Exp.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"
#include "DBManager.h"
#include "../ShareLib/DBCmd.h"
#include "DratanCastle.h"
#include "Party_recall_manager.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"
#include "../ShareLib/packetType/ptype_syndicate.h"
#include "../ShareLib/packetType/ptype_old_do_changejob.h"
#include "../ShareLib/packetType/ptype_old_do_monstercombo.h"
#include "../ShareLib/packetType/ptype_server_to_server_kick.h"
#include "../ShareLib/packetType/ptype_old_do_friend.h"
#include "../ShareLib/packetType/ptype_old_do_guild.h"
#include "DescManager.h"

#ifdef PREMIUM_CHAR
#include "../ShareLib/packetType/ptype_premium_char.h"
#endif

void ProcHelperWhisperReq(CNetMsg::SP& msg, int seq, int server, int subno, int zone);
void ProcHelperWhisperRep(CNetMsg::SP& msg, int seq, int server, int subno, int zone);
void ProcHelperWhisperNotfound(CNetMsg::SP& msg, int seq, int server, int subno, int zone);

void ProcHelperWarNoticeTime(CNetMsg::SP& msg);
void ProcHelperWarNoticeTimeRemain(CNetMsg::SP& msg);
void ProcHelperWarNoticeStart(CNetMsg::SP& msg);
void ProcHelperWarJoinAttackGuild(CNetMsg::SP& msg);
void ProcHelperWarJoinDefenseGuild(CNetMsg::SP& msg);
void ProcHelperWarNoticeStartAttackCastle(CNetMsg::SP& msg);
void ProcHelperWarNoticeRemainFieldTime(CNetMsg::SP& msg);
void ProcHelperWarNoticeEnd(CNetMsg::SP& msg);

void ProcHelperGuildStashHistoryRep(CNetMsg::SP& msg);
void ProcHelperGuildStashViewRep(CNetMsg::SP& msg);
void ProcHelperGuildStashTakeRep(CNetMsg::SP& msg);
void ProcHelperGuildStashSaveTaxRep(CNetMsg::SP& msg);

void ProcHelperPartyMemberChangeJob(CNetMsg::SP& msg);
void ProcHelperPartyChat(CNetMsg::SP& msg);
void ProcHelperPartyRecallPrompt(CNetMsg::SP& msg);
void ProcHelperPartyRecallConfirm(CNetMsg::SP& msg);
void ProcHelperPartyRecallProc(CNetMsg::SP& msg);

void ProcHelperFameupRep(CNetMsg::SP& msg);
void ProcHelperTeacherLoadRep(CNetMsg::SP& msg);

void ProcHelperGuildCreateRep(CNetMsg::SP& msg);
void ProcHelperGuildCreateNotify(CNetMsg::SP& msg);
void ProcHelperGuildOnlineNotify(CNetMsg::SP& msg);
void ProcHelperGuildMemberList(CNetMsg::SP& msg);
void ProcHelperGuildLoadRep(CNetMsg::SP& msg);
void ProcHelperGuildLevelUpRep(CNetMsg::SP& msg);
void ProcHelperGuildLevelUpNotify(CNetMsg::SP& msg);
void ProcHelperGuildBreakUpRep(CNetMsg::SP& msg);
void ProcHelperGuildBreakUpNotify(CNetMsg::SP& msg);
void ProcHelperGuildMemberAddNotify(CNetMsg::SP& msg);
void ProcHelperGuildMemberAddRep(CNetMsg::SP& msg);
void ProcHelperGuildMemberOutNotify(CNetMsg::SP& msg);
void ProcHelperGuildMemberOutRep(CNetMsg::SP& msg);
void ProcHelperGuildMemberKickRep(CNetMsg::SP& msg);
void ProcHelperGuildMemberKickNotify(CNetMsg::SP& msg);
void ProcHelperGuildChangeBossNotify(CNetMsg::SP& msg);
void ProcHelperGuildChangeBossRep(CNetMsg::SP& msg);
void ProcHelperGuildAppointOfficerRep(CNetMsg::SP& msg);
void ProcHelperGuildAppointOfficerNotify(CNetMsg::SP& msg);
void ProcHelperGuildChat(CNetMsg::SP& msg);
void ProcHelperGuildFireOfficerRep(CNetMsg::SP& msg);
void ProcHelperGuildFireOfficerNotify(CNetMsg::SP& msg);
void ProcHelperGuildLoadNotify(CNetMsg::SP& msg);

void ProcHelperGuildBattleRep(CNetMsg::SP& msg);
void ProcHelperGuildBattleStart(CNetMsg::SP& msg);
void ProcHelperGuildBattleStopRep(CNetMsg::SP& msg);
void ProcHelperGuildBattleStatus(CNetMsg::SP& msg);
void ProcHelperGuildBattlePeaceRep(CNetMsg::SP& msg);

void ProcHelperEventMoonStoneLoad(CNetMsg::SP& msg);
void ProcHelperEventMoonStoneUpdate(CNetMsg::SP& msg);
void ProcHelperEventMoonStoneJackpot(CNetMsg::SP& msg);

void ProcHelperGuildMarkTable(CNetMsg::SP& msg);

void ProcHelper_FriendAddRep(CNetMsg::SP& msg);
void ProcHelper_FriendDelRep(CNetMsg::SP& msg);
void ProcHelper_FriendSetConditionRep(CNetMsg::SP& msg);
void ProcHelper_BlockPCRep(CNetMsg::SP& msg);

void ProcHelper_GiftRecvChar(CNetMsg::SP& msg);

void ProcHelperPetCreateRep(CNetMsg::SP& msg);
void ProcHelperPetDeleteRep(CNetMsg::SP& msg);
void ProcHelperPetChangeName( CNetMsg::SP& msg );

///////////////////////
// pd4 helper rep : bw

void ProcHelperPD4RankViewRep(CNetMsg::SP& msg);
void ProcHelperPD4RewardViewRep(CNetMsg::SP& msg);
void ProcHelperPD4RewardRep(CNetMsg::SP& msg);

void ProcHelperNameChange(CNetMsg::SP& msg);

void ProcHelperPartyInviteReq(CNetMsg::SP& msg);
void ProcHelperPartyInviteRep(CNetMsg::SP& msg);
void ProcHelperPartyAllowRep(CNetMsg::SP& msg);
void ProcHelperPartyRejectRep(CNetMsg::SP& msg);
void ProcHelperPartyQuitRep(CNetMsg::SP& msg);
void ProcHelperPartyKickRep(CNetMsg::SP& msg);
void ProcHelperPartyChangeBossRep(CNetMsg::SP& msg);
void ProcHelperPartyChangeTypeRep(CNetMsg::SP& msg);

void ProcHelperPartyEndPartyRep(CNetMsg::SP& msg);

void ProcHelperPartyMatchRegMemberRep(CNetMsg::SP& msg);
void ProcHelperPartyMatchRegPartyRep(CNetMsg::SP& msg);
void ProcHelperPartyMatchDelRep(CNetMsg::SP& msg);
void ProcHelperPartyMatchInviteRep(CNetMsg::SP& msg);
void ProcHelperPartyMatchJoinRep(CNetMsg::SP& msg);
void ProcHelperPartyMatchJoinAllowRep(CNetMsg::SP& msg);
void ProcHelperPartyMatchJoinRejectRep(CNetMsg::SP& msg);
void ProcHelperPartyMatchMemberChangeInfo(CNetMsg::SP& msg);

void ProcHelperPartyInfoParty(CNetMsg::SP& msg);
void ProchelperPartyInfoPartyMatchMember(CNetMsg::SP& msg);
void ProchelperPartyInfoPartyMatchParty(CNetMsg::SP& msg);

void ProcHelperPartyInfoEnd(CNetMsg::SP& msg);

void ProcHelperGuildInclineEstablish( CNetMsg::SP& msg );
void ProcHelperGuildMemberAdjust( CNetMsg::SP& msg );
void ProcHelperGuildInfoNotice( CNetMsg::SP& msg );
void ProcHelperNewGuildMemberListRep( CNetMsg::SP& msg );
void ProcHelperNewGuilManageRep( CNetMsg::SP& msg );
void ProcHelperNewGuildNotifyRep( CNetMsg::SP& msg );
void ProcHelperNewGuildNotifyUpdateReP( CNetMsg::SP& msg );
void ProcHelperNewGuildNotifyTrans( CNetMsg::SP& msg );
void ProcHelperNewGuildNotifyTransRepMsg( CNetMsg::SP& msg );
void ProcHelperNewGuildSkillListRepMsg( CNetMsg::SP& msg );
void ProcHelperNewGuildLoadNTF( CNetMsg::SP& msg );
void ProcHelperNewGuildMemberList( CNetMsg::SP& msg );
void ProcHelperNewGuildInfoRepMsg( CNetMsg::SP& msg );
void ProcHelperNewGuildPointUpdateMsg( CNetMsg::SP& msg );
void ProcHelperNewGuildNotice( CNetMsg::SP& msg );
void ProcHelperNewGuildMemberPointSaveMsg( CNetMsg::SP& msg );
void ProcHelperNewGuildSkillLearnSendMember( CNetMsg::SP& msg );

void ProcHelperHalloween2007( CNetMsg::SP& msg );

void ProcHelperDVDRateChange(CNetMsg::SP& msg);
void ProcHelperDVDNormalChangeNotice(CNetMsg::SP& msg);

void ProcHelperComboGotoWaitRoomPrompt( CNetMsg::SP& msg );
void ProcHelperComboRecallToWaitRoomPrompt(CNetMsg::SP& msg);

void ProcHelperAttackPet( CNetMsg::SP& msg );

#ifdef EXTREME_CUBE
void ProcHelperCubeStateRep(CNetMsg::SP& msg);
void ProcHelperCubeStatePersonalRep(CNetMsg::SP& msg);
void ProcHelperGuildCubeNotice(CNetMsg::SP& msg);
#ifdef EXTREME_CUBE_VER2
void ProcHelperCubeRewardPersonalRep(CNetMsg::SP& msg);
void ProcHelperExtremeCubeError(CNetMsg::SP& msg);
void ProcHelperCubeRewardGuildPointRep(CNetMsg::SP& msg);
#endif // EXTREME_CUBE_VER2
#endif // EXTREME_CUBE

void ProcHelperEventPhoenix( CNetMsg::SP& msg);

void ProcHelperExpedErrorRep( CNetMsg::SP& msg);

void ProcHelperExped_CreateRep( CNetMsg::SP& msg);
void ProcHelperExped_InviteRep( CNetMsg::SP& msg);
void ProcHelperExped_AllowRep( CNetMsg::SP& msg);
void ProcHelperExped_RejectRep( CNetMsg::SP& msg);
void ProcHelperExped_QuitRep( CNetMsg::SP& msg);
void ProcHelperExped_KickRep( CNetMsg::SP& msg);
void ProcHelperExped_ChangeTypeRep( CNetMsg::SP& msg);
void ProcHelperExped_ChangeBossRep( CNetMsg::SP& msg);
void ProcHelperExped_SetMBossRep( CNetMsg::SP& msg);
void ProcHelperExped_ResetMBossRep( CNetMsg::SP& msg);
void ProcHelperExped_EndExpedRep( CNetMsg::SP& msg);
void ProcHelperExped_MoveGroupRep( CNetMsg::SP& msg);

void ProcHelperRaidErrorRep(CNetMsg::SP& msg);

void ProcHelperInzoneGetRoomnoRep(CNetMsg::SP& msg);

void ProcHelperDeleteRaidCharacterRep(CNetMsg::SP& msg);
void ProcHelperRaidInfoRep(CNetMsg::SP& msg);

void ProcHelperNSCreateCard(CNetMsg::SP& msg);

#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE)
void ProcHelperWorldcup2010(CNetMsg::SP& msg);
#endif // defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE)

void ProcHelperTeachRenewer(CNetMsg::SP& msg);
void ProcHelperTeachRenewerTimeLimit(CNetMsg::SP& msg);
void ProcHelperTeachRenewerGift(CNetMsg::SP& msg);

void ProcHelperFaceOff(CNetMsg::SP& msg);

#ifdef EVENT_CHAR_BIRTHDAY
void ProcHelperCharBirthday(CNetMsg::SP& msg);
#endif

#ifdef DEV_GUILD_MARK
void ProcHelperGuildMarkRegist(CNetMsg::SP& msg);
void ProcHelperGuildMarkExpire(CNetMsg::SP& msg);
#endif

#ifdef WARCASTLE_STATE_CHANNEL_CYNC
void ProcHelperCastleStateCync(CNetMsg::SP& msg);
#endif

#ifdef DEV_GUILD_STASH
void ProcHelperGuildStashList(CNetMsg::SP& msg);
void ProcHelperGuildStashKeep(CNetMsg::SP& msg);
void ProcHelperGuildStashTake(CNetMsg::SP& msg);
void ProcHelperGuildStashLog(CNetMsg::SP& msg);
void ProcHelperGuildStashError(CNetMsg::SP& msg);
void LogGuildStash(CItem* pItem, const LONGLONG count, const unsigned char type, CPC* pc);
#endif // DEV_GUILD_STASH

void ProcHelperStudentListRefresh(CNetMsg::SP& msg);
void ProcHelperTeacherStudentLsitCync(CNetMsg::SP& msg);
void ProcHelperTeacherInfoSet(CPC* pc, CNetMsg::SP& msg );

void ProcHelperGuildMasterKickRep(CNetMsg::SP& msg);
void ProcHelperGuildMasterKickCancelRep(CNetMsg::SP& msg);
void ProcHelperGuildMasterKickStatus(CNetMsg::SP& msg);
void ProcHelperGuildMasaterKickReset(CNetMsg::SP& msg);

void ProcHelperRvrUpdateCreateJewelPoint(CNetMsg::SP& msg);
void ProcHelperRvrUpdateJoinCount(CNetMsg::SP& msg);
void ProcHelperRvRUpdateKingInfo(CNetMsg::SP& msg);
void ProcHelperRVRInfoUpdate(CNetMsg::SP& msg);
void ProcHelperRvRKingDownGrade( CNetMsg::SP& msg );

void ProcHelperKickUser(CNetMsg::SP& msg);
void ProcHelperKickUserAnswer(CNetMsg::SP& msg);
void ProcHelperKickUserByCharIndexAnswer(CNetMsg::SP& msg);
void ProcHelperKickUserByUserIndexAnswer(CNetMsg::SP& msg);
void ProcHelperKickUserByUserIdAnswer(CNetMsg::SP& msg);


void CServer::ProcessHelperMessage(CNetMsg::SP& msg)
{
	msg->MoveFirst();

	switch (msg->m_mtype)
	{
	case MSG_HELPER_COMMAND:
		{
			int subtype;
			RefMsg(msg) >> subtype;
			//LOG_INFO("DEBUG_FUNC : START : type : %d, subtype : %d", msg->m_mtype, subtype);
			switch (subtype)
			{
			case MSG_HELPER_PARTY_INFO_PARTY:
				ProcHelperPartyInfoParty(msg);
				break;

			case MSG_HELPER_PARTY_INFO_PARTY_MATCH_MEMBER:
				ProchelperPartyInfoPartyMatchMember(msg);
				break;

			case MSG_HELPER_PARTY_INFO_PARTY_MATCH_PARTY:
				ProchelperPartyInfoPartyMatchParty(msg);
				break;

			case MSG_HELPER_PARTY_INFO_END:
				ProcHelperPartyInfoEnd(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO:
				ProcHelperPartyMatchMemberChangeInfo(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_REG_MEMBER_REP:
				ProcHelperPartyMatchRegMemberRep(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_REG_PARTY_REP:
				ProcHelperPartyMatchRegPartyRep(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_DEL_REP:
				ProcHelperPartyMatchDelRep(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_INVITE_REP:
				ProcHelperPartyMatchInviteRep(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_JOIN_REP:
				ProcHelperPartyMatchJoinRep(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_JOIN_ALLOW_REP:
				ProcHelperPartyMatchJoinAllowRep(msg);
				break;

			case MSG_HELPER_PARTY_MATCH_JOIN_REJECT_REP:
				ProcHelperPartyMatchJoinRejectRep(msg);
				break;

			case MSG_HELPER_PARTY_CHANGE_BOSS_REP:
				ProcHelperPartyChangeBossRep(msg);
				break;

			case MSG_HELPER_PARTY_CHANGE_TYPE_REP:
				ProcHelperPartyChangeTypeRep(msg);
				break;

			case MSG_HELPER_PARTY_END_PARTY_REP:
				ProcHelperPartyEndPartyRep(msg);
				break;

			case MSG_HELPER_PARTY_KICK_REP:
				ProcHelperPartyKickRep(msg);
				break;

			case MSG_HELPER_PARTY_QUIT_REP:
				ProcHelperPartyQuitRep(msg);
				break;

			case MSG_HELPER_PARTY_REJECT_REP:
				ProcHelperPartyRejectRep(msg);
				break;

			case MSG_HELPER_PARTY_ALLOW_REP:
				ProcHelperPartyAllowRep(msg);
				break;

			case MSG_HELPER_PARTY_INVITE_REP:
				ProcHelperPartyInviteRep(msg);
				break;

			case MSG_HELPER_PARTY_INVITE_REQ:
				ProcHelperPartyInviteReq(msg);
				break;

			case MSG_HELPER_GUILD_CREATE_REP:
				ProcHelperGuildCreateRep(msg);
				break;

			case MSG_HELPER_GUILD_CREATE_NTF:
				ProcHelperGuildCreateNotify(msg);
				break;

			case MSG_HELPER_GUILD_ONLINE_NTF:
				ProcHelperGuildOnlineNotify(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_LIST:
				ProcHelperGuildMemberList(msg);
				break;

			case MSG_HELPER_GUILD_LOAD_REP:
				ProcHelperGuildLoadRep(msg);
				break;

			case MSG_HELPER_GUILD_LEVELUP_REP:
				ProcHelperGuildLevelUpRep(msg);
				break;

			case MSG_HELPER_GUILD_LEVELUP_NTF:
				ProcHelperGuildLevelUpNotify(msg);
				break;

			case MSG_HELPER_GUILD_BREAKUP_REP:
				ProcHelperGuildBreakUpRep(msg);
				break;

			case MSG_HELPER_GUILD_BREAKUP_NTF:
				ProcHelperGuildBreakUpNotify(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_ADD_NTF:		// 길드 멤버 추가 알림
				ProcHelperGuildMemberAddNotify(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_ADD_REP:
				ProcHelperGuildMemberAddRep(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_OUT_NTF:
				ProcHelperGuildMemberOutNotify(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_OUT_REP:
				ProcHelperGuildMemberOutRep(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_KICK_REP:
				ProcHelperGuildMemberKickRep(msg);
				break;

			case MSG_HELPER_GUILD_MEMBER_KICK_NTF:
				ProcHelperGuildMemberKickNotify(msg);
				break;

			case MSG_HELPER_GUILD_CHANGE_BOSS_REP:
				ProcHelperGuildChangeBossRep(msg);
				break;

			case MSG_HELPER_GUILD_CHANGE_BOSS_NTF:
				ProcHelperGuildChangeBossNotify(msg);
				break;

			case MSG_HELPER_GUILD_APPOINT_OFFICER_REP:
				ProcHelperGuildAppointOfficerRep(msg);
				break;

			case MSG_HELPER_GUILD_APPOINT_OFFICER_NTF:
				ProcHelperGuildAppointOfficerNotify(msg);
				break;

			case MSG_HELPER_GUILD_CHAT:
				ProcHelperGuildChat(msg);
				break;

			case MSG_HELPER_GUILD_FIRE_OFFICER_REP:
				ProcHelperGuildFireOfficerRep(msg);
				break;

			case MSG_HELPER_GUILD_FIRE_OFFICER_NTF:
				ProcHelperGuildFireOfficerNotify(msg);
				break;

			case MSG_HELPER_GUILD_LOAD_NTF:
				ProcHelperGuildLoadNotify(msg);
				break;

			case MSG_HELPER_GUILD_BATTLE_REP:
				ProcHelperGuildBattleRep(msg);
				break;

			case MSG_HELPER_GUILD_BATTLE_START:
				ProcHelperGuildBattleStart(msg);
				break;

			case MSG_HELPER_GUILD_BATTLE_STOP_REP:
				ProcHelperGuildBattleStopRep(msg);
				break;

			case MSG_HELPER_GUILD_BATTLE_STATUS:
				ProcHelperGuildBattleStatus(msg);
				break;

			case MSG_HELPER_GUILD_BATTLE_PEACE_REP:
				ProcHelperGuildBattlePeaceRep(msg);
				break;

			case MSG_HELPER_GUILD_MARK_TABLE:
				ProcHelperGuildMarkTable(msg);
				break;
//0503 kwon
			case MSG_HELPER_EVENT_MOONSTONE_LOAD:
			case MSG_HELPER_EVENT_MOONSTONE_UPDATE_REP:
				ProcHelperEventMoonStoneLoad(msg);
				break;
			case MSG_HELPER_EVENT_MOONSTONE_JACKPOT_REP:
				ProcHelperEventMoonStoneJackpot(msg);
				break;
			case MSG_HELPER_FRIEND_SET_CONDITION_NOTIFY:
				ProcHelper_FriendSetConditionRep(msg);
				break;

////////////////////////
// pd4 헬퍼 rep 처리 : bw

			case MSG_HELPER_PD4_RANK_VIEW_REP:
				ProcHelperPD4RankViewRep(msg);
				break;
			case MSG_HELPER_PD4_REWARD_VIEW_REP:
				ProcHelperPD4RewardViewRep(msg);
				break;
			case MSG_HELPER_PD4_REWARD_REP:
				ProcHelperPD4RewardRep(msg);
				break;
			case MSG_HELPER_NAME_CHANGE_REP:
				ProcHelperNameChange(msg);
				break;

// 애완동물
			case MSG_HELPER_PET_CREATE_REP:
				ProcHelperPetCreateRep(msg);
				break;

			case MSG_HELPER_PET_DELETE_REP:
				ProcHelperPetDeleteRep(msg);
				break;

			case MSG_HELPER_PET_NAME_CHANGE:
				ProcHelperPetChangeName( msg );
				break;

			case MSG_HELPER_GUILD_STASH_HISTORY_REP:
				ProcHelperGuildStashHistoryRep(msg);
				break;
			case MSG_HELPER_GUILD_STASH_VIEW_REP:
				ProcHelperGuildStashViewRep(msg);
				break;
			case MSG_HELPER_GUILD_STASH_TAKE_REP:
				ProcHelperGuildStashTakeRep(msg);
				break;
			case MSG_HELPER_GUILD_STASH_SAVE_TAX_REP:
				ProcHelperGuildStashSaveTaxRep(msg);
				break;
			case MSG_HELPER_TEACHER_FAMEUP_REP:
				ProcHelperFameupRep(msg);
				break;
			case MSG_HELPER_TEACHER_LOAD_REP:
				ProcHelperTeacherLoadRep(msg);
				break;
			case MSG_HELPER_BLOCKPC_REP:
				ProcHelper_BlockPCRep(msg);
				break;

			case MSG_HELPER_GIFT_RECVCHARNAME_REP:
				ProcHelper_GiftRecvChar(msg);
				break;

			case MSG_HELPER_GUILD_INCLINE_ESTABLISH_REP:
				ProcHelperGuildInclineEstablish( msg );
				break;
			case MSG_HELPER_GUILD_MEMBER_ADJUST_REP:
				ProcHelperGuildMemberAdjust( msg );
				break;
			case MSG_HELPER_NEW_GUILD_INFO_NTF:
				ProcHelperGuildInfoNotice( msg );
				break;
			case MSG_HELPER_NEW_GUILD_MEMBERLIST_REP:
				ProcHelperNewGuildMemberListRep( msg );
				break;
			case MSG_HELPER_NEW_GUILD_MANAGE_REP:
				ProcHelperNewGuilManageRep( msg );
				break;
			case MSG_HELPER_NEW_GUILD_NOTICE_REP:
				ProcHelperNewGuildNotifyRep( msg );
				break;
			case MSG_HELPER_NEW_GUILD_NOTICE_UPDATE_REP:
				ProcHelperNewGuildNotifyUpdateReP( msg );
				break;
			case MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_NTF:
				ProcHelperNewGuildNotifyTrans( msg );
				break;
			case MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_REP:
				ProcHelperNewGuildNotifyTransRepMsg( msg );
				break;
			case MSG_HELPER_NEW_GUILD_NOTICE_SKILLLIST_REP:
				ProcHelperNewGuildSkillListRepMsg( msg );
				break;
			case MSG_HELPER_NEW_GUILD_LOAD_NTF:
				ProcHelperNewGuildLoadNTF( msg );
				break;
			case MSG_HELPER_NEW_GUILD_MEMBER_LIST:
				ProcHelperNewGuildMemberList( msg );
				break;

			case MSG_HELPER_NEW_GUILD_INFO_REP:
				ProcHelperNewGuildInfoRepMsg( msg );
				break;
			case MSG_HELPER_UPDATE_GUILD_POINT:
				ProcHelperNewGuildPointUpdateMsg( msg );
				break;
			case MSG_HELPER_GUILD_NOTICE:
				ProcHelperNewGuildNotice( msg );
				break;
			case MSG_HELPER_SAVE_GUILD_MEMBER_POINT:
				ProcHelperNewGuildMemberPointSaveMsg( msg );
				break;

			case MSG_HELPER_GUILD_SKILL_LEARN_SEND_MEMBER:
				ProcHelperNewGuildSkillLearnSendMember( msg );
				break;

			case MSG_HELPER_HALLOWEEN_2007:
				ProcHelperHalloween2007( msg );
				break;

			case MSG_HELPER_DVD_RATE_CHANGE:
				ProcHelperDVDRateChange( msg );
				break;

			case MSG_HELPER_DVD_NORMAL_CHANGE_NOTICE:
				ProcHelperDVDNormalChangeNotice( msg );
				break;

			case MSG_HELPER_APET :
				ProcHelperAttackPet(msg);
				break;
#ifdef EXTREME_CUBE
			case MSG_HELPER_CUBESTATE_REP:
				ProcHelperCubeStateRep(msg);
				break;
			case MSG_HELPER_CUBESTATE_PERSONAL_REP:
				ProcHelperCubeStatePersonalRep(msg);
				break;
#ifdef EXTREME_CUBE_VER2
			case MSG_HELPER_CUBEREWARD_GUILDPOINT_REP:
				ProcHelperCubeRewardGuildPointRep(msg);
				break;
			case MSG_HELPER_CUBEREWARD_PERSONAL_REP:
				ProcHelperCubeRewardPersonalRep(msg);
				break;
			case MSG_HELPER_EXTREME_CUBE_ERROR:
				ProcHelperExtremeCubeError(msg);
				break;
#endif // EXTREME_CUBE_VER2
#endif // EXTREME_CUBE

			case MSG_HELPER_EVENT_PHOENIX:
				ProcHelperEventPhoenix(msg);
				break;

			case MSG_HELPER_EXPED_ERROR:
				ProcHelperExpedErrorRep(msg);
				break;
			case MSG_HELPER_EXPED_CREATE_REP:
				ProcHelperExped_CreateRep(msg);
				break;
			case MSG_HELPER_EXPED_INVITE_REP:
				ProcHelperExped_InviteRep(msg);
				break;
			case MSG_HELPER_EXPED_ALLOW_REP:
				ProcHelperExped_AllowRep(msg);
				break;
			case MSG_HELPER_EXPED_REJECT_REP:
				ProcHelperExped_RejectRep(msg);
				break;
			case MSG_HELPER_EXPED_QUIT_REP:
				ProcHelperExped_QuitRep(msg);
				break;
			case MSG_HELPER_EXPED_KICK_REP:
				ProcHelperExped_KickRep(msg);
				break;
			case MSG_HELPER_EXPED_CHANGETYPE_REP:
				ProcHelperExped_ChangeTypeRep(msg);
				break;
			case MSG_HELPER_EXPED_CHANGEBOSS_REP:
				ProcHelperExped_ChangeBossRep(msg);
				break;
			case MSG_HELPER_EXPED_SETMBOSS_REP:
				ProcHelperExped_SetMBossRep(msg);
				break;
			case MSG_HELPER_EXPED_RESETMBOSS_REP:
				ProcHelperExped_ResetMBossRep(msg);
				break;
			case MSG_HELPER_EXPED_ENDEXPED_REP:
				ProcHelperExped_EndExpedRep(msg);
				break;
			case MSG_HELPER_EXPED_MOVEGROUP_REP:
				ProcHelperExped_MoveGroupRep(msg);
				break;

			case MSG_HELPER_RAID_ERROR:
				ProcHelperRaidErrorRep(msg);
				break;
			case MSG_HELPER_INZONE_GET_ROOMNO_REP:
				ProcHelperInzoneGetRoomnoRep(msg);
				break;

			case MSG_HELPER_DELETE_RAID_CHAR:
				ProcHelperDeleteRaidCharacterRep(msg);
				break;

			case MSG_HELPER_RAID_INFO:
				ProcHelperRaidInfoRep(msg);
				break;

			case MSG_HELPER_NS_CARD:
				ProcHelperNSCreateCard(msg);
				break;

#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) ||defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)
			case MSG_HELPER_EVENT_WORLDCUP2010:
				ProcHelperWorldcup2010(msg);
				break;
#endif // defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)

			case MSG_HELPER_TEACHER_SYSTEM_RENEWER:
				ProcHelperTeachRenewer(msg);
				break;

			case MSG_HELPER_FACEOFF_REP:
				ProcHelperFaceOff(msg);
				break;

#ifdef EVENT_CHAR_BIRTHDAY
			case MSG_HELPER_CHAR_BIRTHDAY:
				ProcHelperCharBirthday(msg);
				break;
#endif
#ifdef DEV_GUILD_MARK
			case MSG_HELPER_NEW_GUILD_MARK_REGIST:
				ProcHelperGuildMarkRegist(msg);
				break;
			case MSG_HELPER_NEW_GUILD_MARK_EXPIRE:
				ProcHelperGuildMarkExpire(msg);
				break;
#endif
#ifdef DEV_GUILD_STASH
			case MSG_HELPER_GUILD_STASH_LIST :
				ProcHelperGuildStashList(msg);
				break;
			case MSG_HELPER_GUILD_STASH_KEEP :
				ProcHelperGuildStashKeep(msg);
				break;
			case MSG_HELPER_GUILD_STASH_TAKE :
				ProcHelperGuildStashTake(msg);
				break;
			case MSG_HELPER_GUILD_STASH_LOG :
				ProcHelperGuildStashLog(msg);
				break;
			case MSG_HELPER_GUILD_STASH_ERROR :
				ProcHelperGuildStashError(msg);
				break;
			/*case MSG_HELPER_GUILD_STASH_LIST_MONEY:
				ProcHelperGuildStashList(msg);
				break;*/
#endif // DEV_GUILD_STASH
			case MSG_HELPER_TEACHER_STUDENT_LIST_CYNC_REP:
				ProcHelperTeacherStudentLsitCync(msg);
				break;
			case MSG_HELPER_GUILD_MASTER_KICK_REP:
				ProcHelperGuildMasterKickRep(msg);
				break;
			case MSG_HELPER_GUILD_MASTER_KICK_CANCEL_REP:
				ProcHelperGuildMasterKickCancelRep(msg);
				break;
			case MSG_HELPER_GUILD_MASTER_KICK_STATUS:
				ProcHelperGuildMasterKickStatus(msg);
				break;
			case MSG_HELPER_GUILD_MASTER_KICK_RESET:
				ProcHelperGuildMasaterKickReset(msg);
				break;

			case MSG_HELPER_SELECT_CHARACTER:
				{
					int userIndex;
					LONGLONG seq;
					int charIndex;
					RefMsg(msg) >> userIndex >> seq >> charIndex;

					CDescriptor* desc = DescManager::instance()->getDescByUserIndex(userIndex);
					if (desc == NULL)
						break;

					if (desc->m_seq_index != seq)
						break;

					CGuildMember* member = gserver->m_guildlist.findmember(charIndex);
					if (member)
					{
						if (member->guild() == NULL)
							member = NULL;
					}

					desc->request_start_game_flag_ = false;
					DBManager::instance()->PushSelectCharacter(desc, charIndex, member);
				}
				break;
			}
			//LOG_INFO("DEBUG_FUNC : END : type : %d, subtype : %d", msg->m_mtype, subtype);
		}
		break;

	case MSG_HELPER_REQ:
		{
			int seq, server, subno, zone;
			unsigned char subtype;

			RefMsg(msg) >> seq
				>> server >> subno >> zone
				>> subtype;

			if (server != -1 && (server != gserver->m_serverno || (subno != -1 && subno != gserver->m_subno)))
				return ;

			if (zone != -1)
			{
				CZone* pZone = gserver->FindZone(zone);
				if (pZone == NULL)
					return ;
			}

			//LOG_INFO("DEBUG_FUNC : START : type : %d, subtype : %d", msg->m_mtype, subtype);
			switch (subtype)
			{
			case MSG_HELPER_PARTY_MEMBER_CHANGE_JOB:
				ProcHelperPartyMemberChangeJob(msg);
				break;

			case MSG_HELPER_PARTY_CHAT:
				ProcHelperPartyChat(msg);
				break;

			case MSG_HELPER_PARTY_RECALL_PROMPT:
				ProcHelperPartyRecallPrompt(msg);
				break;

			case MSG_HELPER_PARTY_RECALL_CONFIRM:
				ProcHelperPartyRecallConfirm(msg);
				break;

			case MSG_HELPER_PARTY_RECALL_PROC:
				ProcHelperPartyRecallProc(msg);
				break;

			case MSG_HELPER_WAR_NOTICE_END:
				ProcHelperWarNoticeEnd(msg);
				break;

			case MSG_HELPER_WAR_NOTICE_REMAIN_FIELD_TIME:
				ProcHelperWarNoticeRemainFieldTime(msg);
				break;

			case MSG_HELPER_WAR_NOTICE_START_ATTACK_CASTLE:
				ProcHelperWarNoticeStartAttackCastle(msg);
				break;

			case MSG_HELPER_WAR_JOIN_ATTACK_GUILD:
				ProcHelperWarJoinAttackGuild(msg);
				break;

			case MSG_HELPER_WAR_JOIN_DEFENSE_GUILD:
				ProcHelperWarJoinDefenseGuild(msg);
				break;

			case MSG_HELPER_WAR_NOTICE_START:
				ProcHelperWarNoticeStart(msg);
				break;

			case MSG_HELPER_WAR_NOTICE_TIME_REMAIN:
				ProcHelperWarNoticeTimeRemain(msg);
				break;

			case MSG_HELPER_WAR_NOTICE_TIME:
				ProcHelperWarNoticeTime(msg);
				break;

			case MSG_HELPER_WHISPER_REQ:
				ProcHelperWhisperReq(msg, seq, server, subno, zone);
				break;
			case MSG_HELPER_COMBO_GOTO_COMBO_PROMPT:
				ProcHelperComboGotoWaitRoomPrompt(msg);
				break;
			case MSG_HELPER_COMBO_RECALL_TO_COMBO_PROMPT:
				ProcHelperComboRecallToWaitRoomPrompt(msg);
				break;
#ifdef EXTREME_CUBE
			case MSG_HELPER_GUILDCUBETIME_NOTICE:
				ProcHelperGuildCubeNotice(msg);
				break;
#endif // EXTREME_CUBE
#ifdef WARCASTLE_STATE_CHANNEL_CYNC
			case MSG_HELPER_WARCASTLE_STATE_CYNC:
				ProcHelperCastleStateCync(msg);
				break;

#endif

			case MSG_HELPER_TEACH_STUDENT_LIST_SYCN:
				ProcHelperStudentListRefresh(msg);
				break;
			}
			//LOG_INFO("DEBUG_FUNC : END : type : %d, subtype : %d", msg->m_mtype, subtype);
		}
		break;

	case MSG_HELPER_REP:
		{
			int seq, server, subno, zone;
			unsigned char subtype;

			RefMsg(msg) >> seq
				>> server >> subno >> zone
				>> subtype;

			if (server != -1 && (server != gserver->m_serverno || (subno != -1 && subno != gserver->m_subno)))
				return ;

			if (zone != -1)
			{
				CZone* pZone = gserver->FindZone(zone);
				if (pZone == NULL)
					return ;
			}

			//LOG_INFO("DEBUG_FUNC : START : type : %d, subtype : %d", msg->m_mtype, subtype);
			switch (subtype)
			{
			case MSG_HELPER_WHISPER_REP:
				ProcHelperWhisperRep(msg, seq, server, subno, zone);
				break;

			case MSG_HELPER_WHISPER_NOTFOUND:
				ProcHelperWhisperNotfound(msg, seq, server, subno, zone);
				break;
			}
			//LOG_INFO("DEBUG_FUNC : END : type : %d, subtype : %d", msg->m_mtype, subtype);
		}
		break;
	case MSG_HELPER_COMMAND_STRUCT:
		{
			pTypeServer* p = reinterpret_cast<pTypeServer*>(msg->m_buf);
			
			int subtype = p->subType;
			//LOG_INFO("DEBUG_FUNC : START : type : %d, subtype : %d", msg->m_mtype, subtype);
			
			switch(p->subType)
			{
			case MSG_HELPER_RVR_RVR_INFO:
				ProcHelperRVRInfoUpdate(msg);
				break;
			case MSG_HELPER_KING_INFO_UPDATE_ALL:
				ProcHelperRvRUpdateKingInfo(msg);
				break;
			case MSG_HELPER_DOWN_GRADE:
				ProcHelperRvRKingDownGrade(msg);
				break;
			case MSG_HELPER_FRIEND_MEMBER_ADD_REP:
				ProcHelper_FriendAddRep(msg);
				break;
			case MSG_HELPER_FRIEND_MEMBER_DEL_REP:
				ProcHelper_FriendDelRep(msg);
				break;
			}
			//LOG_INFO("DEBUG_FUNC : END : type : %d, subtype : %d", msg->m_mtype, subtype);
		}
		break;

	case MSG_SERVER_TO_SERVER:
		{
			pTypeBase* pType = reinterpret_cast<pTypeBase*>(msg->m_buf);
			
			int subtype = pType->subType;
			//LOG_INFO("DEBUG_FUNC : START : type : %d, subtype : %d", msg->m_mtype, subtype);

			switch (pType->subType)
			{
			case MSG_SUB_SERVERTOSERVER_KICK_REQ:
				ProcHelperKickUser(msg);
				break;

			case MSG_SUB_SERVERTOSERVER_KICK_ANSER:
				ProcHelperKickUserAnswer(msg);
				break;

			case MSG_SUB_SERVERTOSERVER_KICK_BY_CHAR_INDEX_ANSWER:
				ProcHelperKickUserByCharIndexAnswer(msg);
				break;

			case MSG_SUB_SERVERTOSERVER_KICK_BY_USER_INDEX_ANSWER:
				ProcHelperKickUserByUserIndexAnswer(msg);
				break;

			case MSG_SUB_SERVERTOSERVER_KICK_BY_USER_ID_ANSER:
				ProcHelperKickUserByUserIdAnswer(msg);
				break;

			default:
				LOG_FATAL("not found subtype[%d]", pType->subType);
				break;
			}
			//LOG_INFO("DEBUG_FUNC : END : type : %d, subtype : %d", msg->m_mtype, subtype);
		}
		break;

#ifdef PREMIUM_CHAR
	case MSG_PREMIUM_CHAR:
		{
			pTypeBase* pType = reinterpret_cast<pTypeBase*>(msg->m_buf);
			int subtype = pType->subType;
			switch (subtype)
			{
			case MSG_SUB_PREMIUM_CHAR_RESET_JUMP_COUNT_TIME:
				{
					ServerToServerPacket::premiumCharResetJumpCountTime* packet = reinterpret_cast<ServerToServerPacket::premiumCharResetJumpCountTime*>(msg->m_buf);
					PremiumChar::setResetJumpCountTime(packet->resetTime);
				}
				break;

			case MSG_SUB_PREMIUM_CHAR_RESET_JUMP_COUNT:
				{
					PCManager::instance()->resetPremiumCharJumpCount();

					// Helper로 부터 따로 받지 않고, 게임서버가 바로 처리함
					PremiumChar::setResetJumpCountTime(gserver->getNowSecond());
				}
				break;

			default:
				LOG_ERROR("invalid subtype - mtype : %d : subtype : %d", pType->type, pType->subType);
				break;
			}
		}
		break;
#endif
	default:
		break;
	}
}

void ProcHelperNameChange(CNetMsg::SP& msg)
{
	// TODO : BS : 길드와 캐릭터명의 길이가 달라지면 바꿔야함
	char bguild;
	char error;
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	int index;

	RefMsg(msg) >> bguild
		>> index
		>> name
		>> error;
	CPC* pc = NULL;

	if(bguild == 1)
	{
		 pc = gserver->m_guildlist.findguild(index)->boss()->GetPC();

		 if( !pc )
		{
			GAMELOG << init("CASH_ITEM_USE_ERROR-notPC")
					<< bguild << delim
					<< index << delim
					<< 843 << end;
			return;
		}

		if(error == MSG_EX_NAMECHANGE_ERROR_SUCCESS)
		{
			// 아이템 삭제 / 서버 변수 변경 // 길드 개명카드 삭제

			// 아이템 검사
			CItem* item = pc->m_inventory.FindByDBIndex(843, 0, 0);
			if (item == NULL)
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					NameChangeRepMsg(rmsg, MSG_EX_NAMECHANGE_ERROR_HELPER, name, bguild);
					SEND_Q(rmsg, pc->m_desc);
				}

				{
					//롤백
					CNetMsg::SP rmsg(new CNetMsg);
					HelperNameChangeReqMsg(rmsg, 3, pc->m_guildInfo->guild()->index(), pc->m_guildInfo->guild()->name());
					SEND_Q(rmsg, gserver->m_helper);
				}

				GAMELOG << init("CASH_ITEM_USE_ERROR-notItem")
						<< bguild << delim
						<< index << delim
						<< 843 << end;
				return;
			}

			if( (item->m_itemProto->getItemFlag() & ITEM_FLAG_CASH) )
			{
				GAMELOG << init("CASH_ITEM_USE", pc)
						<< itemlog(item) << delim
						<< pc->m_guildInfo->guild()->name() << delim
						<< name << delim
						<< end;
			}

			pc->m_guildInfo->guild()->changeName(name);

			pc->m_inventory.decreaseItemCount(item, 1);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperNameChangeReqMsg(rmsg, 5, pc->m_guildInfo->guild()->index(), name);
				SEND_Q(rmsg, gserver->m_helper);
			}

			return;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			NameChangeRepMsg(rmsg, (MSG_EX_NAMECHANGE_ERROR_TYPE) error, name, bguild);
			SEND_Q(rmsg, pc->m_desc);
		}
	}
	else if( bguild == 5)
	{
		CGuild* guild = gserver->m_guildlist.findguild(index);
		if( guild == NULL) return;

		guild->changeName(name);
		CNetMsg::SP rmsg(new CNetMsg);
		NameChangeRepMsg(rmsg, (MSG_EX_NAMECHANGE_ERROR_TYPE) error, name, bguild);
		guild->SendToAll(rmsg, true);
		return;
	}
	else
	{
		pc = PCManager::instance()->getPlayerByCharIndex(index);
		if( !pc )
		{
			GAMELOG << init("CASH_ITEM_USE_ERROR-notPC")
					<< bguild << delim
					<< index << delim
					<< 842 << end;
			return;
		}
		// 에러코드에 따른 메세지 작성

		if(error == MSG_EX_NAMECHANGE_ERROR_SUCCESS)
		{
			// 아이템 삭제/ 서버 변수 변경 // 개명카드 삭제
			// 아이템 검사
			CItem* item = pc->m_inventory.FindByDBIndex(842, 0, 0);
			if (item == NULL)
			{
				item = pc->m_inventory.FindByDBIndex(1120, 0, 0);
			}

			if (item == NULL)
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					NameChangeRepMsg(rmsg, MSG_EX_NAMECHANGE_ERROR_HELPER, name, bguild);
					SEND_Q(rmsg, pc->m_desc);
				}

				{
					//롤백
					CNetMsg::SP rmsg(new CNetMsg);
					HelperNameChangeReqMsg(rmsg, 4, pc->m_index, pc->GetName());
					SEND_Q(rmsg, gserver->m_helper);
				}

				GAMELOG << init("CASH_ITEM_USE_ERROR-notItem")
						<< bguild << delim
						<< index << delim
						<< 842 << end;
				return;
			}

			if( (item->m_itemProto->getItemFlag() & ITEM_FLAG_CASH) )
			{
				GAMELOG << init("CASH_ITEM_USE", pc)
						<< itemlog(item) << delim
						<< pc->GetName() << delim
						<< name << delim
						<< end;
			}

			pc->m_inventory.decreaseItemCount(item, 1);

			pc->ChangeName(name);

			return;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			NameChangeRepMsg(rmsg, (MSG_EX_NAMECHANGE_ERROR_TYPE) error, name, bguild);
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}

void ProcHelperPD4RankViewRep(CNetMsg::SP& msg)
{
	//////////////////////////////////
	// 받은 데이터를 클라이언트에게 보냄 : BW
	//MSG_HELPER_PD4_RANK_VIEW_REQ,		// : charindex(n) charjob(c)
	//MSG_HELPER_PD4_RANK_VIEW_REP : charindex(n) multicount(c), charname(str), bclear(c), deadmon(n), ctime(ll);
	int charindex;
	char multicount;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	char bclear;
	int deadmon;
	LONGLONG ctime;
#ifdef NPC_CHECK
	int npcIndex;
#endif

	if( !msg->CanRead() )
	{
		return;
	}

	RefMsg(msg) >> charindex
#ifdef NPC_CHECK
		>> npcIndex
#endif
		>> multicount;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(charindex);

	if( !ch )
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_QUEST);
		RefMsg(rmsg) << (unsigned char) MSG_QUEST_PD4_RANK_VIEW_REP
#ifdef NPC_CHECK
			<< npcIndex
#endif
			<< multicount;

		if(multicount == 0)
		{
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		for(int i = 0; i < multicount; i++)
		{
			RefMsg(msg) >> charname
				>> bclear
				>> deadmon
				>> ctime;

			RefMsg(rmsg) << charname
				<< bclear
				<< deadmon
				<< ctime;
		}
		SEND_Q(rmsg, ch->m_desc);
	}

	//MSG_QUEST_PD4_RANK_VIEW_REP,			// 랭크 뷰응답	: multicount(c) charname(str) bclear(c) deadmon(n) time(ll)
}
void ProcHelperPD4RewardViewRep(CNetMsg::SP& msg)
{
	//////////////////////////////////
	// 받은 데이터를 클라이언트에게 보냄
	//MSG_HELPER_PD4_REWARD_VIEW_REQ,		// : charindex(n) charjob(c)
	//MSG_HELPER_PD4_REWARD_VIEW_REP : charindex(n) multicount(c), charname(str) breward(c)
	int charindex;
	char multicount;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	int itemindex[5] = { 85, 85, 85, 673, 673 };
	char itemnum[5] = { 3, 2, 1, 3, 1 };
	char breward;
#ifdef NPC_CHECK
	int npcIndex;
#endif

	if( !msg->CanRead() )
	{
		return;
	}

#ifdef NPC_CHECK
	RefMsg(msg) >> charindex
		>> npcIndex
		>> multicount;
#else
	RefMsg(msg) >> charindex
		>> multicount;
#endif

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(charindex);
	if( !ch )
		return;

	if(multicount == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_QUEST);
		RefMsg(rmsg) << (unsigned char) MSG_QUEST_PD4_RANK_REWARD_RANK_REP
#ifdef NPC_CHECK
			 << npcIndex
#endif
			 << (char) 0;

		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_QUEST);
		RefMsg(rmsg) << (unsigned char) MSG_QUEST_PD4_RANK_REWARD_RANK_REP
#ifdef NPC_CHECK
			<< npcIndex
#endif
			<< multicount;

		for(int i = 0; i < multicount; i++)
		{
			RefMsg(msg) >> charname
				>> breward;

			RefMsg(rmsg) << charname
				<< itemindex[i]
			<< itemnum[i]
			<< breward;
		}

		SEND_Q(rmsg, ch->m_desc);
	}

	//MSG_QUEST_PD4_RANK_REWARD_RANK_REP,	// 보상 뷰응답	: multicount(c) charname(str) itemindex(n) itemnum(c) breward(c)
}
void ProcHelperPD4RewardRep(CNetMsg::SP& msg)
{
	//////////////////////////////////
	// 받은 데이터를 클라이언트에게 보냄
	//MSG_HELPER_PD4_REWARD_REQ : charindex(n)
	//MSG_HELPER_PD4_REWARD_REP : charindex(n) charrank(c) breward(c)
	int itemindex[5] = { 85, 85, 85, 673, 673 };
	char itemnum[5] = { 3, 2, 1, 3, 1 };

	int charindex;
	char charrank;
	char breward;
	int npcIndex;

	if(!msg->CanRead())
	{
		GAMELOG << init("Quest reward Error");
		return;
	}

	RefMsg(msg) >> charindex
		>> charrank
		>> breward
		>> npcIndex;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(charindex);

	if( !ch)
	{
		GAMELOG << init("PD4_REWARD_ERROR")
				<< charindex << delim
				<< charrank << delim
				<< breward << end;
		return;
	}

	if(!charrank)
	{
		//////////////////////////
		// 랭커가 아니다
		CNetMsg::SP rmsg(new CNetMsg);
		QuestPD4ErrorMsg(rmsg, MSG_QUEST_ERR_REWARD_NOT_RANK);
		GAMELOG << init("QuestPD4Error Reward Not Rank", ch)
				<< end;

		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(breward)
	{
		////////////////////////////////////////////
		// 이미 보상 받았으
		CNetMsg::SP rmsg(new CNetMsg);
		QuestPD4ErrorMsg(rmsg, MSG_QUEST_ERR_BREWARD);
		GAMELOG << init("QuestPD4Error Reward Not Rank", ch)
				<< end;

		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItem* item = gserver->m_itemProtoList.CreateItem(itemindex[charrank - 1], -1, 0, 0, itemnum[charrank - 1]);

	if (!item)
	{
		//////////////////////////////
		// item 생성 실패
		return;
	}

	if(item->IsAccessary() && item->m_nOption < 1)
	{
		OptionSettingItem(ch, item);
	}

	if (ch->m_inventory.addItem(item) == false)
	{
		// 인젠토리 꽉차서 못 받을 때
		// Quest Error Log
		GAMELOG << init("QUEST ERROR", ch)
				<< 105 << delim
				<< (int)MSG_QUEST_ERR_PRIZE_FULL
				<< end;

		// 인벤토리 가득참
		item = ch->m_pArea->DropItem(item, ch);
		if (!item)
		{
			/////////////////
			// item drop 실패
			return;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			item->m_preferenceIndex = ch->m_index;
			ItemDropMsg(rmsg, ch, item);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			QuestErrorMsg(rmsg, MSG_QUEST_ERR_PRIZE_FULL);
			SEND_Q(rmsg, ch->m_desc);
		}

		return;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_QUEST);
		RefMsg(rmsg) << (unsigned char) MSG_QUEST_PD4_REWARD;
		RefMsg(rmsg) << npcIndex;
		SEND_Q(rmsg, ch->m_desc);
	}

	// Item LOG
	GAMELOG << init("ITEM_PICK_REWARD_PD4", ch)
			<< itemlog(item)
			<< end;

	// Quest Prize Log
	GAMELOG << init("QUEST PRIZE", ch)
			<< 105 << delim
			<< QPRIZE_ITEM << delim
			<< itemindex[charrank-1] << delim
			<< itemnum[charrank-1]
			<< end;

	//MSG_QUEST_PD4_REWARD_REP,					// 보상 요청 : charindex(n)
}

void ProcHelperWhisperReq(CNetMsg::SP& msg, int seq, int server, int subno, int zone)
{
	int sidx;
	CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString chat(1000);
	RefMsg(msg) >> sidx >> sname
		>> rname
		>> chat;

	CPC* receiver = PCManager::instance()->getPlayerByName(rname, true);
	if (receiver == NULL)
	{
		// 수신자가 없을 때
		CNetMsg::SP rmsg(new CNetMsg);
		HelperRepWhisperNotFoundMsg(rmsg, seq, server, subno, zone, sidx, sname);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		{
			// 수신자가 있을 때
			CNetMsg::SP rmsg(new CNetMsg);
			ChatWhisperMsg(rmsg, sidx, sname, receiver->GetName(), chat);
			SEND_Q(rmsg, receiver->m_desc);
		}

		{
			// 메신저에 응답 보내기
			CNetMsg::SP rmsg(new CNetMsg);
			HelperRepWhisperRepMsg(rmsg, seq, server, subno, zone, sidx, sname, receiver->GetName() , chat);
			SEND_Q(rmsg, gserver->m_helper);
		}
	}
}

void ProcHelperWhisperRep(CNetMsg::SP& msg, int seq, int server, int subno, int zone)
{
	int sidx, ridx;
	CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString chat(1000);
	RefMsg(msg) >> sidx >> sname
		>> ridx >> rname
		>> chat;

	CPC* sender = PCManager::instance()->getPlayerByCharIndex(sidx);
	if (sender != NULL)
	{
		// 송신자가 있으면
		CNetMsg::SP rmsg(new CNetMsg);
		ChatWhisperMsg(rmsg, sidx, sname, rname, chat);
		SEND_Q(rmsg, sender->m_desc);
	}
}

void ProcHelperWhisperNotfound(CNetMsg::SP& msg, int seq, int server, int subno, int zone)
{
	int sidx;
	CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	RefMsg(msg) >> sidx >> sname;

	CPC* sender = PCManager::instance()->getPlayerByCharIndex(sidx);
	if (sender != NULL)
	{
		// 송신자가 있으면 오류 알림
		CNetMsg::SP rmsg(new CNetMsg);
		SysWhisperNotFoundMsg(rmsg);
		SEND_Q(rmsg, sender->m_desc);
	}
}

void ProcHelperGuildCreateRep(CNetMsg::SP& msg)
{
	int charindex;
	unsigned char errcode;
	RefMsg(msg) >> charindex >> errcode;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (!pc)
	{
		return ;
	}

	if (errcode == MSG_GUILD_ERROR_OK)
	{
		if (pc->m_inventory.getMoney() >= GUILD_LEVEL1_NEED_MONEY)
		{
			pc->m_inventory.decreaseMoney(GUILD_LEVEL1_NEED_MONEY);
		}
		pc->m_skillPoint -= GUILD_LEVEL1_NEED_SP * 10000;

		pc->CalcStatus(true);
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CREATE_OK);
			SEND_Q(rmsg, pc->m_desc);
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);
		SEND_Q(rmsg, pc->m_desc);
	}
}

void ProcHelperGuildCreateNotify(CNetMsg::SP& msg)
{
	int guildindex;
	int guildlevel;
	CLCString guildname(MAX_GUILD_NAME_LENGTH + 1);
	int bossindex;
	CLCString bossname(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> guildindex
		>> guildlevel
		>> guildname
		>> bossindex
		>> bossname;

	CGuild* guild = gserver->m_guildlist.create(guildindex, guildname, bossindex, bossname);
	if (!guild)
	{
		return ;
	}

	guild->level(guildlevel);
	guild->setMaxmember( GUILD_LEVEL1_MAX_MEMBER );
	guild->incline( 0 );
	guild->guildPointRanking( 0 );
	gserver->m_guildlist.add(guild);

	// 보스가 있으면 알리기
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(bossindex);
	if (pc)
	{
		guild->findmember(bossindex)->online(1);
		guild->findmember(bossindex)->SetPC(pc);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildInfoMsg(rmsg, pc);
			SEND_Q(rmsg, pc->m_desc);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildListMsg(rmsg, pc);
			SEND_Q(rmsg, pc->m_desc);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildInfoChangeMsg(rmsg, pc->m_index, guildindex, guildname, MSG_GUILD_POSITION_BOSS, pc);
			pc->m_pArea->SendToCell(rmsg, pc, false);
		}
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildPointRankingMsg( rmsg, pc->m_index, guildindex, guild->GetGuildPointRanking() );
			SEND_Q(rmsg, pc->m_desc);
			pc->m_pArea->SendToCell( rmsg,pc, false );
		}
	}

	GAMELOG << init("GUILD CREATE")
			<< "INDEX" << delim
			<< guild->index() << delim
			<< "NAME" << delim
			<< guild->name() << delim
			<< "BOSS" << delim
			<< guild->boss()->charindex() << delim
			<< guild->boss()->GetName()
			<< end;
}

void ProcHelperGuildOnlineNotify(CNetMsg::SP& msg)
{
	int guildindex;
	int charindex;
	char online;
	RefMsg(msg) >> guildindex
		>> charindex
		>> online;

	int zoneindex;
	RefMsg(msg) >> zoneindex;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
		return ;
	CGuildMember* member = guild->findmember(charindex);
	if (!member)
		return ;
	member->online(online);

	member->zoneindex( zoneindex );

	member->SetPC(NULL);
	if (online)
	{
		CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
		if (pc)
		{
			member->SetPC(pc);
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildLoadReq(rmsg, charindex );
			SEND_Q(rmsg, gserver->m_helper);
		}
	}

	// 길드원에게 알려주기
	// TODO : GUILD : 트리 구조에 전달하게 될 경우 변경
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildOnlineMsg(rmsg, member);
		guild->SendToAll(rmsg);
	}
}

void ProcHelperGuildMemberList(CNetMsg::SP& msg)
{
	int guildindex;
	int count;
	int charindex;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	int pos;
	char online;

	RefMsg(msg) >> guildindex
		>> count;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	int i;
	for (i = 0; i < count; i++)
	{
		RefMsg(msg) >> charindex
			>> charname
			>> pos
			>> online;

		int listindex = guild->addmember(charindex, charname);
		if (listindex == -1)
			continue ;

		switch (pos)
		{
		case MSG_GUILD_POSITION_BOSS:
			guild->changeboss(listindex);
			break;
		case MSG_GUILD_POSITION_OFFICER:
			guild->appoint(listindex);
			break;
		}

		CGuildMember* member = guild->member(listindex);
		if (!member)
			continue ;
		member->online(online);

		switch( pos )
		{
		case MSG_GUILD_POSITION_RUSH_CAPTAIN:			// 돌격조 대장
		case MSG_GUILD_POSITION_SUPPORT_CAPTAIN:			// 지원조 대장
		case MSG_GUILD_POSITION_RECON_CAPTAIN:			// 정찰조 대장
		case MSG_GUILD_POSITION_RUSH_MEMBER:				// 돌격조원
		case MSG_GUILD_POSITION_SUPPORT_MEMBER:			// 지원조원
		case MSG_GUILD_POSITION_RECON_MEMBER:			// 정찰조원
		case MSG_GUILD_POSITION_MEMBER:				// 일반 길드원
			member->pos(pos);
			break;
		}
		guild->AddGradeExPosCount(pos);

		CPC* pc = PCManager::instance()->getPlayerByCharIndex(member->charindex());
		if (pc)
		{
			member->online(1);
			member->SetPC(pc);

			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildOnline(rmsg, member);
			RefMsg(rmsg) << member->GetZoneNum();
			SEND_Q(rmsg, gserver->m_helper);
		}
		else
			member->SetPC(NULL);
	}
}

void ProcHelperGuildLoadRep(CNetMsg::SP& msg)
{
	CLCString idname(MAX_ID_NAME_LENGTH + 1);
	int charindex;
	unsigned char errcode;
	RefMsg(msg) >> idname >> charindex >> errcode;
}

void ProcHelperGuildLevelUpRep(CNetMsg::SP& msg)
{
	int guildindex;
	int charindex;
	unsigned char errcode;

	RefMsg(msg) >> guildindex >> charindex >> errcode;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (!guild)
	{
		return ;
	}

	if (errcode == MSG_GUILD_ERROR_OK)
	{
		int needlevel = 0x7fffffff;
		LONGLONG needmoney = 0x7fffffff;
		int needsp = 0x7fffffff;
		int needgp = 0x7fffffff;
		if( guild->level() < GUILD_EXTEND_BASE_LEVEL )
		{
			switch (guild->level())
			{
			case 2:
				needlevel = GUILD_LEVEL2_NEED_LEVEL;
				needmoney = GUILD_LEVEL2_NEED_MONEY;
				needsp = GUILD_LEVEL2_NEED_SP;
				break;

			case 3:
				needlevel = GUILD_LEVEL3_NEED_LEVEL;
				needmoney = GUILD_LEVEL3_NEED_MONEY;
				needsp = GUILD_LEVEL3_NEED_SP;
				break;

			case 4:
				needlevel = GUILD_LEVEL4_NEED_LEVEL;
				needmoney = GUILD_LEVEL4_NEED_MONEY;
				needsp = GUILD_LEVEL4_NEED_SP;
				break;

			case 5:
				needlevel = GUILD_LEVEL5_NEED_LEVEL;
				needmoney = GUILD_LEVEL5_NEED_MONEY;
				needsp = GUILD_LEVEL5_NEED_SP;
				break;

			case 6:
				needlevel = GUILD_LEVEL6_NEED_LEVEL;
				needmoney = GUILD_LEVEL6_NEED_MONEY;
				needsp = GUILD_LEVEL6_NEED_SP;
				break;
			}
		}
		else if( guild->level() >= GUILD_EXTEND_BASE_LEVEL )
		{
			if( guild->level() > 6 && guild->level() < 11 )
				needlevel = GUILD_LEVEL7_NEED_LEVEL;
			else if( guild->level() > 10 && guild->level() < 20 )
				needlevel = GUILD_LEVEL7_NEED_LEVEL + 10;
			else if( guild->level() > 19 && guild->level() < 30 )
				needlevel = GUILD_LEVEL7_NEED_LEVEL + 20;
			else if( guild->level() > 29 && guild->level() < 40 )
				needlevel = GUILD_LEVEL7_NEED_LEVEL + 30;
			else if( guild->level() > 39 && guild->level() < 50 )
				needlevel = GUILD_LEVEL7_NEED_LEVEL + 40;
			else
				needlevel = GUILD_LEVEL7_NEED_LEVEL + 50;

			int	gap = guild->level() - GUILD_EXTEND_BASE_LEVEL;  // 이미렙업한 상태
			if( gap >= 0 )
			{
				needmoney	= (LONGLONG)( GUILD_LEVEL7_NEED_MONEY * pow(1.09, gap) );
				needsp		= (int)( GUILD_LEVEL7_NEED_SP * pow(1.09, gap) );
				needgp		= (int)( GUILD_LEVEL7_NEED_GP * pow(1.09, gap) );
			}
			else
			{
				needmoney	= GUILD_LEVEL7_NEED_MONEY;
				needsp		= GUILD_LEVEL7_NEED_SP;
				needgp		= GUILD_LEVEL7_NEED_GP;
			}
		}

		int guildmaxmember = 0;
		switch( guild->level())
			{
			case 2: guildmaxmember = GUILD_LEVEL2_MAX_MEMBER; break;
			case 3: guildmaxmember = GUILD_LEVEL3_MAX_MEMBER; break;
			case 4: guildmaxmember = GUILD_LEVEL4_MAX_MEMBER; break;
			case 5: guildmaxmember = GUILD_LEVEL5_MAX_MEMBER; break;
			case 6: guildmaxmember = GUILD_LEVEL6_MAX_MEMBER; break;
			default :
				{
					int gap = guild->level() - GUILD_EXTEND_BASE_LEVEL;
					if( gap >= 0 )
					{
						if( guild->m_passiveSkillList.Find( 444 ) )
						{
							CSkill* skill = guild->m_passiveSkillList.Find( 444 );
							if(skill->m_level <= 5)
								guildmaxmember = GUILD_LEVEL6_MAX_MEMBER + (5 * skill->m_level );
							else
								guildmaxmember = 55 + 9 * (skill->m_level - 5);
						}
						else
						{
							guildmaxmember = GUILD_LEVEL6_MAX_MEMBER;
						}
					}
				}
				break;
			}

		guild->setMaxmember( guildmaxmember );

		if( guild->GetGuildPoint() > needgp )
		{
			guild->AddGuildPoint( -needgp );
		}

		GAMELOG << init("GUILD LEVEL UP")
				<< "Guild Max Member" << delim
				<< guild->maxmember() << delim
				<< "Guild Point" << delim
				<< guild->GetGuildPoint() << end;

		if(pc)
		{
			if (pc->m_inventory.getMoney() >= needmoney)
			{
				pc->m_inventory.decreaseMoney(needmoney);
			}
			pc->m_skillPoint -= needsp * 10000;

			pc->CalcStatus(true);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildErrorMsg(rmsg, MSG_GUILD_ERROR_LEVELUP_OK);

				GAMELOG << init("GUILD LEVEL UP ", pc )
					<< "Guild Index" << delim << guild->index() << delim
					<< "NAS" << delim << needmoney  << delim
					<< "SP" << delim << pc->m_skillPoint << delim
					<< "Need SP" << delim << needsp * 10000 << delim
					<< "Guild Point" << delim << guild->GetGuildPoint() << delim
					<< "Need GP" << delim << needgp << delim
					<< end;

				SEND_Q(rmsg, pc->m_desc);
			}
		}
	}
	else
	{
		if(pc)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}

void ProcHelperGuildLevelUpNotify(CNetMsg::SP& msg)
{
	int guildindex;
	int guildlevel;

	RefMsg(msg) >> guildindex >> guildlevel;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (guild)
	{
		guild->level(guildlevel);

		// TODO : GUILD : 트리 구성시 전체에 알리기
		CNetMsg::SP rmsg(new CNetMsg);
		GuildLevelInfoMsg(rmsg, guildindex, guildlevel);
		guild->SendToAll(rmsg);

		GAMELOG << init("GUILD LEVELUP")
				<< "INDEX" << delim
				<< guild->index() << delim
				<< "NAME" << delim
				<< guild->name() << delim
				<< "LEVEL" << delim
				<< guild->level()
				<< end;
	}
}

void ProcHelperGuildBreakUpRep(CNetMsg::SP& msg)
{
	int charindex;
	unsigned char errcode;
	RefMsg(msg) >> charindex >> errcode;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (pc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if (errcode == MSG_GUILD_ERROR_OK)
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_BREAKUP_OK);
		else
			GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);
		SEND_Q(rmsg, pc->m_desc);
	}
}

void ProcHelperGuildBreakUpNotify(CNetMsg::SP& msg)
{
	int guildindex;
	RefMsg(msg) >> guildindex;

	// 길드 삭제시 소유 성 정보 초기화 및 용병신청 초기화
	CWarCastle::DeleteGuild(guildindex);

	// TODO : GUILD : 트리 안에 모두 알리기

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
		return ;
	guild->guildPointRanking( 0 );

	GAMELOG << init("GUILD BREAKUP")
			<< "INDEX" << delim
			<< guild->index() << delim
			<< "NAME" << delim
			<< guild->name()
			<< end;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBreakUpNotifyMsg(rmsg, guild->index(), guild->name());
		PCManager::instance()->sendToAll(rmsg);
	}

	std::vector<CPC*> _vec;
	
	for (int i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		if (guild->m_member[i] != NULL)
		{
			if (guild->m_member[i]->GetPC() != NULL
				&& guild->m_member[i]->online() == 1				// 온라인 상태인 경우
				&& guild->m_member[i]->GetPC()->m_desc != NULL)	// 디스크립터가 존재 하는 경우
			{
				_vec.push_back(guild->m_member[i]->GetPC());
			}
		}
	}

#ifdef EXTREME_CUBE
	if(guild->m_cubeUniqueIdx != -1)
	{
		CCubeMemList* memlist;
		memlist = gserver->m_extremeCube.FindMemList(guild);
		if(memlist)
		{
			((CGuildCubeMemList*)memlist)->SetGuild(NULL);
		}
	}
#endif // EXTREME_CUBE

	gserver->m_guildlist.Remove(guild);

	std::vector<CPC*>::iterator it = _vec.begin();
	std::vector<CPC*>::iterator it_end = _vec.end();

	for(; it != it_end; it++)
	{
		(*it)->CalcStatus(true);
	}
}

void ProcHelperGuildMemberAddNotify(CNetMsg::SP& msg)
{
	int guildindex;
	int charindex;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	int pos;

	RefMsg(msg) >> guildindex
		>> charindex
		>> charname
		>> pos;

	int zoneindex;
	RefMsg(msg) >> zoneindex;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if(!guild)
	{
		return;
	}

	int listindex = guild->addmember(charindex, charname);
	if(listindex == -1)
	{
		return;
	}

	CGuildMember * member = guild->member(listindex);
	if(!member)
	{
		return;
	}
	member->online(0);

	member->zoneindex(zoneindex);

	{
		// 길드원 전부에게 추가 메세지   // 071210 kjban edit
		CNetMsg::SP rmsg(new CNetMsg);
		GuildMemberAddMsg(rmsg, guildindex, charindex, charname);
		guild->SendToAll(rmsg);
	}

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(!pc)
	{	// 혀해당 게임 채널에 캐릭이 없는 경우 아래 과정 생략
		return;
	}

	member->online(1);
	member->SetPC(pc);

	pc->m_guildoutdate = 0;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildInfoMsg(rmsg, pc);
		SEND_Q(rmsg, pc->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildListMsg(rmsg, pc);
		SEND_Q(rmsg, pc->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildOnline(rmsg, member);
		RefMsg(rmsg) << zoneindex;
		SEND_Q(rmsg, gserver->m_helper);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildInfoChangeMsg(rmsg, charindex, guildindex, guild->name(), member->pos(), pc);
		pc->m_pArea->SendToCell(rmsg, pc, false);
	}

#ifdef GMTOOL
	if(pc->m_bGmMonitor)
	{
		guild->m_bGmtarget = true;
	}
#endif // GMTOOL

	GAMELOG << init("GUILD MEMBER ADD")
		<< "GUILD" << delim
		<< guild->index() << delim
		<< guild->name() << delim
		<< "MEMBER" << delim
		<< member->charindex() << delim
		<< member->GetName() << delim
		<< end;
}

void ProcHelperGuildMemberAddRep(CNetMsg::SP& msg)
{
	int guildindex;
	int bossindex;
	int charindex;
	unsigned char errcode;

	RefMsg(msg) >> guildindex
		>> bossindex
		>> charindex
		>> errcode;

	CPC* boss = PCManager::instance()->getPlayerByCharIndex(bossindex);
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);

	if (pc)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (errcode == MSG_GUILD_ERROR_OK)
				GuildErrorMsg(rmsg, MSG_GUILD_ERROR_REGIST_OK);
			else
				GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);
			SEND_Q(rmsg, pc->m_desc);
		}

		pc->m_regGuild = 0;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildNameColorStateMsg( rmsg, pc );
			SEND_Q( rmsg , pc->m_desc );
		}

		// 가입한 길드에 공성전 정보가 있을경우 캐릭터에 공성정보 저장
		CDratanCastle * pCastle = CDratanCastle::CreateInstance();
		if( pCastle->IsAttackGuild(guildindex) )
		{
			pc->SetJoinFlag(ZONE_DRATAN, WCJF_ATTACK_GUILD);
		}
		else if( pCastle->IsDefenseGuild(guildindex) )
		{
			pc->SetJoinFlag(ZONE_DRATAN, WCJF_DEFENSE_GUILD);
		}
		else if( pCastle->GetOwnerGuildIndex() == guildindex )
		{
			pc->SetJoinFlag(ZONE_DRATAN, WCJF_OWNER);
		}
	}
	if (boss)
	{
		boss->m_regGuild = 0;
	}

	if(pc != NULL)
		pc->CalcStatus(true);
}

void ProcHelperGuildMemberOutNotify(CNetMsg::SP& msg)
{
	int guildindex;
	int charindex;
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> guildindex
		>> charindex
		>> name;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildMemberOutMsg(rmsg, guildindex, charindex, member->GetName());
		guild->SendToAll(rmsg);
	}

	if (member->GetPC())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildInfoChangeMsg(rmsg, member->charindex(), -1, "", MSG_GUILD_POSITION_UNKNOWN, member->GetPC());
		member->GetPC()->m_pArea->SendToCell(rmsg, member->GetPC(), false);
		time_t t;
		member->GetPC()->m_guildoutdate = time(&t) / 60 / 60 / 24;
	}

#ifdef GMTOOL
	if(member->GetPC() && member->GetPC()->m_bGmMonitor)
	{
		guild->m_bGmtarget = false;
	}
#endif // GMTOOL

	guild->DelGradeExPosCount( member->pos() );
	if( member->GetPC() )
		member->GetPC()->CureGuildGradeSkill();

	if(member->GetPC())
	{
		member->GetPC()->CastllanTitleDelete( -1, true  );
		member->GetPC()->m_inventory.CastllanItemRemove(-1, false, true); // 옷만 벚긴다.
	}

/*#ifdef EXTREME_CUBE
	if(member->GetPC() && member->GetPC()->m_pZone != NULL  && member->GetPC()->m_pZone->IsExtremeCube())
	{
		CCubeMemList* memlist;
		memlist = gserver->m_extremeCube.FindMemList(guild);
		if(memlist)
		{
			int zone = ZONE_MERAC;
			int extra = 0;

			int i;
			CZone* pZone;

			i = gserver->FindZone(zone);
			pZone = gserver->m_zones + i;

			if(member->GetPC())
			{
				CPC* pc = member->GetPC();
				GoZone(pc, pZone->m_index,
					pZone->m_zonePos[extra][0],														// ylayer
					GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
					GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
			}
		}
	}
#endif // EXTREME_CUBE*/

	GAMELOG << init("GUILD MEMBER OUT")
			<< "GUILD" << delim
			<< guild->index() << delim
			<< guild->name() << delim
			<< "MEMBER" << delim
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

	CPC* pc = member->GetPC();
	guild->removemember(member);

	if(pc != NULL)
		pc->CalcStatus(true);
}

void ProcHelperGuildMemberOutRep(CNetMsg::SP& msg)
{
	int guildindex;
	int charindex;
	unsigned char errcode;

	RefMsg(msg) >> guildindex
		>> charindex
		>> errcode;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (pc)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (errcode == MSG_GUILD_ERROR_OK)
			{
				CGuild * guild = gserver->m_guildlist.findguild(guildindex);
				GuildErrorMsg(rmsg, MSG_GUILD_ERROR_OUT_OK, guild);
			}
			else
			{
				GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);
			}
			SEND_Q(rmsg, pc->m_desc);
		}

		// 캐릭터에 공성정보 삭제
		pc->SetJoinFlag(ZONE_DRATAN, WCJF_NONE);
		///=== 메라크?
#ifdef EXTREME_CUBE
		if(pc && pc->m_pZone != NULL && pc->m_pZone->IsExtremeCube())
		{
#ifdef BUGFIX_CUBE_DELETE_MEMLIST
			CCubeMemList* memlist;
			if(gserver->m_extremeCube.IsGuildCubeTime())
			{
				CGuild * guild = gserver->m_guildlist.findguild(guildindex);
				if(guild)
				{
					memlist = gserver->m_extremeCube.FindMemList(guild);
					if(memlist)
					{
						((CGuildCubeMemList*)memlist)->DelPC(pc);
						GAMELOG << init("CUBE MEMBER DELETE", pc) << end;
					}
				}
			}
#endif // BUGFIX_CUBE_DELETE_MEMLIST
			CZone* pZone = gserver->FindZone(ZONE_MERAC);
			if (pZone == NULL)
				return;

			int extra = 0;
			GoZone(pc, pZone->m_index,
					pZone->m_zonePos[extra][0],														// ylayer
					GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
					GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
		}
#endif // EXTREME_CUBE

		if(pc && pc->m_pZone != NULL && pc->m_pZone->IsGuildRoom())
		{
			CZone* pZone = gserver->FindZone(ZONE_START);
			if (pZone == NULL)
				return;

			GoZone(pc, ZONE_START,
					pZone->m_zonePos[0][0],														// ylayer
					GetRandom(pZone->m_zonePos[0][1], pZone->m_zonePos[0][3]) / 2.0f,		// x
					GetRandom(pZone->m_zonePos[0][2], pZone->m_zonePos[0][4]) / 2.0f);		// z
		}
	}
}

void ProcHelperGuildMemberKickRep(CNetMsg::SP& msg)
{
	int guildindex;
	int bossindex;
	int charindex;
	unsigned char errcode;

	RefMsg(msg) >> guildindex
		>> bossindex
		>> charindex
		>> errcode;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(bossindex);
	if (pc)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (errcode == MSG_GUILD_ERROR_OK)
			{
				CGuild * guild = gserver->m_guildlist.findguild(guildindex);
				GuildErrorMsg(rmsg, MSG_GUILD_ERROR_KICK_OK, guild);
			}
			else
				GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);

			SEND_Q(rmsg, pc->m_desc);
		}

#ifdef EXTREME_CUBE
		if(pc && pc->m_pZone != NULL && pc->m_pZone->IsExtremeCube())
		{
#ifdef BUGFIX_CUBE_DELETE_MEMLIST
			CCubeMemList* memlist;
			if(gserver->m_extremeCube.IsGuildCubeTime())
			{
				CGuild * guild = gserver->m_guildlist.findguild(guildindex);
				if(guild)
				{
					memlist = gserver->m_extremeCube.FindMemList(guild);
					if(memlist)
					{
						((CGuildCubeMemList*)memlist)->DelPC(pc);
						GAMELOG << init("CUBE MEMBER DELETE", pc) << end;
					}
				}
			}
#endif // BUGFIX_CUBE_DELETE_MEMLIST
			CZone* pZone = gserver->FindZone(ZONE_MERAC);
			if (pZone == NULL)
				return;

			int extra = 0;
			GoZone(pc, pZone->m_index,
					pZone->m_zonePos[extra][0],														// ylayer
					GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
					GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
		}
#endif // EXTREME_CUBE

		CPC* kickpc = PCManager::instance()->getPlayerByCharIndex(charindex);
		if(kickpc && kickpc->m_pZone != NULL && kickpc->m_pZone->IsGuildRoom())
		{
			CZone* pZone = gserver->FindZone(ZONE_START);
			if (pZone == NULL)
				return;

			GoZone(kickpc, ZONE_START,
					pZone->m_zonePos[0][0],														// ylayer
					GetRandom(pZone->m_zonePos[0][1], pZone->m_zonePos[0][3]) / 2.0f,		// x
					GetRandom(pZone->m_zonePos[0][2], pZone->m_zonePos[0][4]) / 2.0f);		// z
		}
	}
}

void ProcHelperGuildMemberKickNotify(CNetMsg::SP& msg)
{
	int guildindex;
	int bossindex;
	int charindex;

	RefMsg(msg) >> guildindex
		>> bossindex
		>> charindex;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildMemberKickMsg(rmsg, guildindex, bossindex, charindex, member->GetName());
		guild->SendToAll(rmsg);
	}

	GAMELOG << init("GUILD MEMBER KICK")
			<< "GUILD" << delim
			<< guild->index() << delim
			<< guild->name() << delim
			<< "MEMBER" << delim
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (pc)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildInfoChangeMsg(rmsg, charindex, -1, "", MSG_GUILD_POSITION_UNKNOWN, pc);
			pc->m_pArea->SendToCell(rmsg, pc, false);
		}

#ifdef GUILD_MEMBER_KICK_JOIN_DELAY
		time_t t;
		pc->m_guildoutdate = time(&t) / 60 / 60 / 24;
#endif

#ifdef GMTOOL
		if(pc->m_bGmMonitor)
		{
			guild->m_bGmtarget = false;
		}
#endif // GMTOOL

		if( guild->level() > MANAGE_NEED_LEVEL )
		{
			if( gserver->isRunHelper() )
			{
				if(pc->m_guildInfo && pc->m_guildInfo->guild())
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperNewGuildManage( rmsg, pc );
					SEND_Q( rmsg, gserver->m_helper );
				}
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
				SEND_Q(rmsg, pc->m_desc);
			}
		}

		// 캐릭터에 공성정보 삭제
		pc->SetJoinFlag(ZONE_DRATAN, WCJF_NONE);
		///=== 메라크?

#ifdef EXTREME_CUBE
		if(pc->m_pZone != NULL && pc->m_pZone->IsExtremeCube())
		{
			CCubeMemList* memlist;
			memlist = gserver->m_extremeCube.FindMemList(guild);
			if(memlist)
			{
				CZone* pZone = gserver->FindZone(ZONE_MERAC);
				if (pZone == NULL)
					return;

				int extra = 0;
				GoZone(pc, pZone->m_index,
					pZone->m_zonePos[extra][0],														// ylayer
					GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
					GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
			}
		}
#endif // EXTREME_CUBE
	}
#ifdef GUILD_MEMBER_KICK_JOIN_DELAY
	// 핼퍼를 통해서 업데이트 해주자.
	else if(!pc)
	{
		time_t t;
		int outdate = time(&t) / 60 / 60 / 24;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildKickOutDateUpdateReqMsg(rmsg, charindex, outdate);
		SEND_Q(rmsg, gserver->m_helper);
	}
#endif // GUILD_MEMBER_KICK_JOIN_DELAY

	guild->DelGradeExPosCount( member->pos() );
	if( member->GetPC() )
		member->GetPC()->CureGuildGradeSkill();

	if(member->GetPC())
	{
		member->GetPC()->CastllanTitleDelete( -1, true );
		member->GetPC()->m_inventory.CastllanItemRemove(-1, false, true); // 옷만 벚긴다.
	}

	guild->removemember(member);

	if(pc != NULL)
	{
		pc->CalcStatus(true);
	}
}

void ProcHelperGuildChangeBossNotify(CNetMsg::SP& msg)
{
	int guildindex;
	int current;
	int change;

	RefMsg(msg) >> guildindex
		>> current
		>> change;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	CGuildMember* member = guild->findmember(change);
	if (!member)
		return ;
// << kjban
// 	GAMELOG << init("GUILD CHANGE BOSS")
// 			<< "GUILD" << delim
// 			<< guild->index() << delim
// 			<< guild->name() << delim
// 			<< "PRE BOSS" << delim;
// 	if( guild->boss() )
// 	{
// 			GAMELOG << guild->boss()->charindex() << delim
// 					<< guild->boss()->GetName();
// 	}
// 	GAMELOG	<< "NEW BOSS" << delim
// 			<< member->charindex() << delim
// 			<< member->GetName()
// 			<< end;
	if(guild->boss() == NULL)
	{
		GAMELOG << init("GUILD CHANGE BOSS")
			<< "GUILD" << delim
			<< guild->index() << delim
			<< guild->name() << delim
			<< "NEW BOSS" << delim
			<< member->charindex() << delim
			<< member->GetName() << end;
	}
	else
	{
		GAMELOG << init("GUILD CHANGE BOSS")
			<< "GUILD" << delim
			<< guild->index() << delim
			<< guild->name() << delim
			<< "PRE BOSS" << delim
			<< guild->boss()->charindex() << delim
			<< guild->boss()->GetName() << delim
			<< "NEW BOSS" << delim
			<< member->charindex() << delim
			<< member->GetName() << end;
	}
// >>

	guild->changeboss(member->listindex());

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildChangeBossMsg(rmsg, guildindex, current, change);
		guild->SendToAll(rmsg);
	}

	if (member->GetPC())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildInfoChangeMsg(rmsg, change, guildindex, guild->name(), MSG_GUILD_POSITION_BOSS, member->GetPC());
		member->GetPC()->m_pArea->SendToCell(rmsg, member->GetPC(), false);
	}

	CDratanCastle* pDratanCastle = CDratanCastle::CreateInstance();
	if( pDratanCastle->GetOwnerCharIndex() == current )
	{
		CPC* pc = PCManager::instance()->getPlayerByCharIndex(current);
		if( pc )
		{
			pc->CastllanTitleDelete(-1 , true);
			pc->m_inventory.CastllanItemRemove(-1, false, true); // 옷만 벚긴다.
		}
	}

	// 성주 길드라면 성주변경
	CWarCastle * cast = CWarCastle::GetCastleObject( CWarCastle::GetCurSubServerCastleZoneIndex() );
	if( cast && cast->GetOwnerGuildIndex() == guild->index() )
	{
		// 이전 보스에 무기 뺏고,
		CPC* preboss = PCManager::instance()->getPlayerByCharIndex(current);
		if( preboss )
		{
			BOOST_FOREACH(CItemProto* flagload_item, gserver->m_itemProtoList.m_flagload)
			{
				// 해당하는 모든 아이템을 검색한다.
				item_search_t vec;
				int sc = preboss->m_inventory.searchItemByCondition(flagload_item->getItemIndex(), 0, 0, vec);
				if (sc == 0)
					continue;

				item_search_t::iterator it = vec.begin();
				item_search_t::iterator endit = vec.end();
				for (; it != endit; ++it)
				{
					CItem* item = (*it).pItem;

					if (item->getWearPos() != WEARING_NONE)
					{
						if (item->getWearPos() >= WEARING_SHOW_START && item->getWearPos() <= WEARING_SHOW_END && preboss->m_pArea)
						{
							CNetMsg::SP rmsg(new CNetMsg);
							WearingMsg(rmsg, preboss, item->getWearPos(), -1, 0);
							preboss->m_pArea->SendToCell(rmsg, preboss, true);
						}
						preboss->m_wearInventory.RemoveItem(item->getWearPos());
					}
				}				
			}
			preboss->CastllanTitleDelete(-1, true );
			preboss->m_inventory.CastllanItemRemove(-1, false, true); // 옷만 벚긴다.
		}

		cast->SetOwner(guild);
		CDBCmd cmd;
		cmd.Init(&gserver->m_dbcastle);

		std::string update_castle_query = boost::str(boost::format("UPDATE t_castle SET a_owner_charindex = %d, a_owner_char_name = '%s' WHERE a_zone_index = %d ") % member->charindex()% member->GetName()% cast->GetZoneIndex() );
		cmd.SetQuery(update_castle_query);
		cmd.Update();
		GAMELOG << init("WARCASTLE BOSS CHANGE")
				<< member->charindex() << delim
				<< member->GetName() << end;

		// 현재 보스에 무기 지급
		CPC* boss = PCManager::instance()->getPlayerByCharIndex(change);
		if (boss && boss->m_guildInfo && boss->m_guildInfo->guild())
		{
			int ownZoneCount;
			int* ownZoneIndex = CWarCastle::GetOwnCastle(boss->m_guildInfo->guild()->index(), &ownZoneCount);
			if (ownZoneIndex)
			{
				time_t curtime;
				time(&curtime);

				int i;
				for (i = 0; i < ownZoneCount; i++)
				{
					CWarCastle* castle = CWarCastle::GetCastleObject(ownZoneIndex[i]);
					if (castle)
					{
						// 060116 : BS : BEGIN : 공성 시작해도 칼 회수 안하게
	//					int nexttime = castle->GetNextWarTime();

						if (castle->GetOwnerCharIndex() == boss->m_index)
						{
							// 060116 : BS : BEGIN : 공성 시작해도 칼 회수 안하게
	//						// 공성 시작 5분전에서 공성 진행중이면 회수
	//						if (castle->GetState() != WCSF_NORMAL || curtime + 5 * 60 >= nexttime)
	//						{
	//							castle->TakeLordItem(d->m_pChar);
	//						}
	//
	//						// 일반 상태에서 없으면 지급
	//						else
	//						{
								castle->GiveLordItem(boss);
	//						}
							// 060116 : BS : END : 공성 시작해도 칼 회수 안하게
						}
					}
				}
				delete [] ownZoneIndex;
			}
			boss->CastllanTitleDelete(-1, true);
			boss->m_inventory.CastllanItemRemove(-1, false, true); // 옷만 벚긴다.
		}
	}

	CGuildMember* boss = guild->findmember(current);
	if (!boss)
		return ;

	if (boss->GetPC())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildInfoChangeMsg(rmsg, current, guildindex, guild->name(), MSG_GUILD_POSITION_MEMBER, boss->GetPC());
		boss->GetPC()->m_pArea->SendToCell(rmsg, boss->GetPC(), false);
	}

	if( guild->level() > MANAGE_NEED_LEVEL )
	{
		CPC* ch = PCManager::instance()->getPlayerByCharIndex(current);
		CPC* ch1 = PCManager::instance()->getPlayerByCharIndex(change);

		if( ch && ch1 )
		{
			if( gserver->isRunHelper() )
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperNewGuildManage( rmsg, ch );
					SEND_Q( rmsg, gserver->m_helper );
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperNewGuildManage( rmsg, ch1 );
					SEND_Q( rmsg, gserver->m_helper );
				}
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
				SEND_Q(rmsg, ch->m_desc);
				SEND_Q(rmsg, ch1->m_desc);
			}
		}
	}

	// 길마가 바뀌었을 때, GP정보 쏴주기
	// current와 change PC로 발송.
	CGuildMember* pPrevBoss = NULL;
	CGuildMember* pChangeBoss = NULL;
	pPrevBoss = guild->findmember(current);
	pChangeBoss = guild->findmember(change);
	if(pPrevBoss->GetPC())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildPointInfo(rmsg, 0);
		SEND_Q(rmsg, pPrevBoss->GetPC()->m_desc);
	}
	if(pChangeBoss->GetPC())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildPointInfo(rmsg, guild->GetGuildPoint());
		SEND_Q(rmsg, pChangeBoss->GetPC()->m_desc);
	}
}

void ProcHelperGuildChangeBossRep(CNetMsg::SP& msg)
{
	int guildindex;
	int current;
	int change;
	unsigned char errcode;

	RefMsg(msg) >> guildindex
		>> current
		>> change
		>> errcode;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(current);
	if (pc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if (errcode == MSG_GUILD_ERROR_OK)
		{
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CHANGE_BOSS_OK);
		}
		else
			GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);

		SEND_Q(rmsg, pc->m_desc);
	}
}

void ProcHelperGuildAppointOfficerRep(CNetMsg::SP& msg)
{
	int guildindex;
	int bossindex;
	int charindex;
	unsigned char errcode;

	RefMsg(msg) >> guildindex
		>> bossindex
		>> charindex
		>> errcode;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(bossindex);
	if (pc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if (errcode == MSG_GUILD_ERROR_OK)
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_APPOINT_OFFICER_OK);
		else
			GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);

		SEND_Q(rmsg, pc->m_desc);
	}
}

void ProcHelperGuildAppointOfficerNotify(CNetMsg::SP& msg)
{
	int guildindex;
	int bossindex;
	int charindex;

	RefMsg(msg) >> guildindex
		>> bossindex
		>> charindex;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
		return ;

	guild->appoint(member->listindex());

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildAppointOfficerMsg(rmsg, guildindex, charindex);
		guild->SendToAll(rmsg);
	}

	if (member->GetPC())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildInfoChangeMsg(rmsg, charindex, guildindex, guild->name(), MSG_GUILD_POSITION_OFFICER, member->GetPC());
		member->GetPC()->m_pArea->SendToCell(rmsg, member->GetPC(), false);
	}

	GAMELOG << init("GUILD OFFICER APPOINT")
			<< "GUILD" << delim
			<< guild->index() << delim
			<< guild->name() << delim
			<< "NEW OFFICER" << delim
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

	if( guild->level() > MANAGE_NEED_LEVEL )
	{
		CPC* ch = PCManager::instance()->getPlayerByCharIndex(bossindex);
		if( ch )
		{
			if( gserver->isRunHelper() )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperNewGuildManage( rmsg, ch );
				SEND_Q( rmsg, gserver->m_helper );
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}
}

void ProcHelperGuildChat(CNetMsg::SP& msg)
{
	int guildindex;
	int charindex;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString chat(1000);

	RefMsg(msg) >> guildindex
		>> charindex
		>> charname
		>> chat;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (guild)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GUILD, charindex, charname, "", chat);
			guild->SendToAll(rmsg);
		}
#ifdef GMTOOL
		if(guild->m_bGmtarget)
		{
			bool bSend = false;
			for(int i = 0; i < GUILD_MAX_MEMBER; ++i)
			{
				CGuildMember* member = guild->member(i);
				if(member && member->GetPC() && member->GetPC()->m_bGmMonitor)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					MsgrNoticeGmChatMonitorGuildMsg(rmsg, -1, 1, 1, 0, gserver->m_serverno, gserver->m_subno, -1, chat, guildindex, charname, member->GetPC()->m_index, member->GetPC()->m_name);
					SEND_Q(rmsg, gserver->m_messenger);

					bSend = true;
				}
			}

			if(!bSend)
			{
				guild->m_bGmtarget = false;
			}
		}
#endif // GMTOOL
	}
}

void ProcHelperGuildFireOfficerRep(CNetMsg::SP& msg)
{
	int guildindex;
	int bossindex;
	int charindex;
	unsigned char errcode;

	RefMsg(msg) >> guildindex
		>> bossindex
		>> charindex
		>> errcode;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(bossindex);
	if (pc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if (errcode == MSG_GUILD_ERROR_OK)
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_FIRE_OFFICER_OK);
		else
			GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)errcode);

		SEND_Q(rmsg, pc->m_desc);
	}
}

void ProcHelperGuildFireOfficerNotify(CNetMsg::SP& msg)
{
	int guildindex;
	int bossindex;
	int charindex;

	RefMsg(msg) >> guildindex
		>> bossindex
		>> charindex;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
		return ;

	guild->fire(member->listindex());

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildFireOfficerMsg(rmsg, guildindex, charindex);
		guild->SendToAll(rmsg);
	}

	if (member->GetPC())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildInfoChangeMsg(rmsg, charindex, guildindex, guild->name(), MSG_GUILD_POSITION_MEMBER, member->GetPC());
		member->GetPC()->m_pArea->SendToCell(rmsg, member->GetPC(), false);
	}

	GAMELOG << init("GUILD OFFICER FIRE")
			<< "GUILD" << delim
			<< guild->index() << delim
			<< guild->name() << delim
			<< "OFFICER" << delim
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

	if( guild->level() > MANAGE_NEED_LEVEL )
	{
		CPC* ch = PCManager::instance()->getPlayerByCharIndex(bossindex);
		if( ch )
		{
			if( gserver->isRunHelper() )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperNewGuildManage( rmsg, ch );
				SEND_Q( rmsg, gserver->m_helper );
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}
}

void ProcHelperGuildLoadNotify(CNetMsg::SP& msg)
{
	int guildindex;
	int guildlevel;
	CLCString guildname(MAX_GUILD_NAME_LENGTH + 1);
	int bossindex;
	CLCString bossname(MAX_CHAR_NAME_LENGTH + 1);
	int battleIndex;

#ifdef DEV_GUILD_MARK
	char gm_row;
	char gm_col;
	char bg_row;
	char bg_col;
	int markTime;
#endif

	RefMsg(msg) >> guildindex
		>> guildlevel
		>> guildname
		>> bossindex
		>> bossname
		>> battleIndex;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);

	if (!guild)
	{
		guild = gserver->m_guildlist.create(guildindex, guildname, bossindex, bossname);
		if (!guild)
			return;

		guild->level(guildlevel);

		gserver->m_guildlist.add(guild);
	}

	int battlePrize;
	int battleTime;
	int battleZone;
	int killCount;
	char battleState;
	int maxmember;

	RefMsg(msg) >> battlePrize
		>> battleTime
		>> battleZone
		>> killCount
		>> battleState
		>> maxmember;
	guild->setMaxmember( maxmember );

	if (battleIndex != -1)
	{
		guild->BattleSet(battleIndex, battlePrize, battleZone);
		guild->BattleTime(battleTime);
		guild->KillCount(killCount);
		guild->BattleState(battleState);
	}
#ifdef DEV_GUILD_MARK
	RefMsg(msg) >> gm_row >> gm_col >> bg_row >> bg_col >> markTime;
	guild->SetGuildMark(gm_row, gm_col, bg_row, bg_col, markTime);
#endif
	int _kickStatus = GMKS_NORMAL, _requestChar = 0, _requestTime = 0, _lastTimeBossChange = 0;
	RefMsg(msg) >> _kickStatus
		>> _requestChar
		>> _requestTime
		>> _lastTimeBossChange;
	guild->getGuildKick()->setKickStatus(_kickStatus);
	guild->getGuildKick()->setKickReuestChar(_requestChar);
	guild->getGuildKick()->setKickRequestTime(_requestTime);

	/*
	CGuild* guild = gserver->m_guildlist.create(guildindex, guildname, bossindex, bossname);
	if (!guild)
	{
		return ;
	}

	guild->level(guildlevel);
	gserver->m_guildlist.add(guild);
	*/
}

void ProcHelperGuildBattleRep(CNetMsg::SP& msg)
{
	int guildindex1;
	CLCString guildname1(MAX_GUILD_NAME_LENGTH + 1);
	int guildindex2;
	CLCString guildname2(MAX_GUILD_NAME_LENGTH + 1);
	int prize;
	int zone;
	int time;

	RefMsg(msg) >> guildindex1
		>> guildname1
		>> guildindex2
		>> guildname2
		>> prize
		>> zone
		>> time;

	CGuild* g1 = gserver->m_guildlist.findguild(guildindex1);
	CGuild* g2 = gserver->m_guildlist.findguild(guildindex2);

	CPC* boss1 = NULL;
	CPC* boss2 = NULL;

	if (g1)
		boss1 = g1->boss()->GetPC();

	if (g2)
		boss2 = g2->boss()->GetPC();

	if (!g1 || !g2 || !boss1 || !boss2)
		return ;

	if (boss1)
	{
		if (boss1->m_inventory.getMoney())
		{
			if (boss1->m_inventory.getMoney() >= (LONGLONG)prize)
			{
				boss1->m_inventory.decreaseMoney(prize);
			}
		}
	}

	if (boss2)
	{
		if (boss2->m_inventory.getMoney())
		{
			if (boss2->m_inventory.getMoney() >= (LONGLONG)prize)
			{
				boss2->m_inventory.decreaseMoney(prize);
			}
		}
	}

	// 세부 셋팅
	g1->BattleSet(g2->index(), prize * 2 * 95 / 100, g1->battleZone());
	g1->BattleTime(time);
	g1->BattleState(GUILD_BATTLE_STATE_WAIT);

	g2->BattleSet(g1->index(), prize * 2 * 95 / 100, g1->battleZone());
	g2->BattleTime(time);
	g2->BattleState(GUILD_BATTLE_STATE_WAIT);

	{
		// 길전이 성립되다!
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleReqAccpetMsg(rmsg, guildindex1, guildname1, guildindex2, guildname2, prize * 2 * 95 / 100 / 10000, zone, time);
		g1->SendToAll(rmsg);
		g2->SendToAll(rmsg);
	}
}

void ProcHelperGuildBattleStart(CNetMsg::SP& msg)
{
	int guildindex1;
	CLCString guildname1(MAX_GUILD_NAME_LENGTH + 1);
	int guildindex2;
	CLCString guildname2(MAX_GUILD_NAME_LENGTH + 1);
	int prize;
	int zone;
	int time;
	CNetMsg::SP rmsg(new CNetMsg);

	RefMsg(msg) >> guildindex1
		>> guildname1
		>> guildindex2
		>> guildname2
		>> prize
		>> zone
		>> time;

	CGuild* g1 = gserver->m_guildlist.findguild(guildindex1);
	CGuild* g2 = gserver->m_guildlist.findguild(guildindex2);
	if (g1)
	{
		g1->BattleState(GUILD_BATTLE_STATE_ING);
		g1->BattleTime(time);
	}

	if (g2)
	{
		g2->BattleState(GUILD_BATTLE_STATE_ING);
		g2->BattleTime(time);
	}
}

void ProcHelperGuildBattleStopRep(CNetMsg::SP& msg)
{
	int winner_index;
	int guildindex1;
	CLCString guildname1(MAX_GUILD_NAME_LENGTH + 1);
	int guildindex2;
	CLCString guildname2(MAX_GUILD_NAME_LENGTH + 1);
	int prize;
	int zone;
	int guildkill1;
	int guildkill2;

	RefMsg(msg) >> winner_index
		>> guildindex1
		>> guildname1
		>> guildkill1
		>> guildindex2
		>> guildname2
		>> guildkill2
		>> prize
		>> zone;

	CGuild* g1 = gserver->m_guildlist.findguild(guildindex1);
	CGuild* g2 = gserver->m_guildlist.findguild(guildindex2);

	CPC* boss = NULL;
	bool bHaveMoney = false;

	// 동점이면 상금이 반
	if (winner_index == -1)
	{
		prize /= 2;

		if (g1)
			g1->BattleSet(guildindex2, prize, zone);
		if (g2)
			g2->BattleSet(guildindex1, prize, zone);
	}

	// 있는 길드 우선 셋팅 해제
	if (g1)
	{
		g1->BattleState(GUILD_BATTLE_STATE_PRIZE);

		// 동점이거나 승리길드일때 길드장 찾기
		if (winner_index == -1 || winner_index == g1->index())
		{
			boss = g1->boss()->GetPC();

			// 길드장이 있으면 상금 주고 길드 상태 해제 요청
			if (boss)
			{
				boss->m_inventory.increaseMoney(prize);

				// 길드전 해제 요청
				if (gserver->isRunHelper())
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperGuildBattlePeaceReqMsg(rmsg, g1);
					SEND_Q(rmsg, gserver->m_helper);
				}
				else
				{
					return ;
				}

				g1->BattleSet(-1, 0, -1);
				g1->BattleTime(0);
				g1->KillCount(0);
				g1->BattleState(GUILD_BATTLE_STATE_PEACE);
			}
		}
		else
		{
			g1->BattleSet(-1, 0, -1);
			g1->BattleTime(0);
			g1->KillCount(0);
			g1->BattleState(GUILD_BATTLE_STATE_PEACE);
		}
	}

	boss = NULL;
	bHaveMoney = false;

	if (g2)
	{
		g2->BattleState(GUILD_BATTLE_STATE_PRIZE);

		// 동점이거나 승리길드일때 길드장 찾기
		if (winner_index == -1 || winner_index == g2->index())
		{
			boss = g2->boss()->GetPC();

			// 길드장이 있으면 상금 주고 길드 상태 해제 요청
			if (boss)
			{
				boss->m_inventory.increaseMoney(prize);

				// 길드전 해제 요청
				if (gserver->isRunHelper())
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperGuildBattlePeaceReqMsg(rmsg, g2);
					SEND_Q(rmsg, gserver->m_helper);
				}
				else
				{
					return ;
				}

				g2->BattleSet(-1, 0, -1);
				g2->BattleTime(0);
				g2->KillCount(0);
				g2->BattleState(GUILD_BATTLE_STATE_PEACE);
			}
		}
		else
		{
			g2->BattleSet(-1, 0, -1);
			g2->BattleTime(0);
			g2->KillCount(0);
			g2->BattleState(GUILD_BATTLE_STATE_PEACE);
		}
	}

	{
		// 길드전 종료 메시지
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleEndMsg(rmsg, winner_index, guildindex1, guildname1, guildindex2, guildname2, prize);
		if (g1)
			g1->SendToAll(rmsg);
		if (g2)
			g2->SendToAll(rmsg);
	}
}

void ProcHelperGuildBattleStatus(CNetMsg::SP& msg)
{
	int guildindex1;
	CLCString guildname1(MAX_GUILD_NAME_LENGTH + 1);
	int killCount1;
	int guildindex2;
	CLCString guildname2(MAX_GUILD_NAME_LENGTH + 1);
	int killCount2;
	int battleTime;
	int battleZone;

	RefMsg(msg) >> guildindex1
		>> guildname1
		>> killCount1
		>> guildindex2
		>> guildname2
		>> killCount2
		>> battleTime
		>> battleZone;

	CGuild* g1 = gserver->m_guildlist.findguild(guildindex1);
	CGuild* g2 = gserver->m_guildlist.findguild(guildindex2);

	if (g1)
	{
		g1->KillCount(killCount1);
		g1->BattleTime(battleTime);
	}
	if (g2)
	{
		g2->KillCount(killCount2);
		g2->BattleTime(battleTime);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);

		GuildBattleStatusMsg(rmsg, guildindex1, guildname1, killCount1, guildindex2, guildname2, killCount2, battleTime, battleZone);
		if (g1)
			g1->SendToAll(rmsg);
		if (g2)
			g2->SendToAll(rmsg);
	}
}

void ProcHelperGuildBattlePeaceRep(CNetMsg::SP& msg)
{
	int guildindex;

	RefMsg(msg) >> guildindex;

	CGuild* g = gserver->m_guildlist.findguild(guildindex);

	if (!g)
		return;

	g->BattleSet(-1, 0, -1);
	g->BattleTime(0);
	g->KillCount(0);
	g->BattleState(GUILD_BATTLE_STATE_PEACE);
}

void ProcHelperGuildMarkTable(CNetMsg::SP& msg)
{
	for(int i=0; i < 3; i++)
	{
		RefMsg(msg) >> gserver->m_nGuildMarkTable[i];
	}
}

void ProcHelperEventMoonStoneLoad(CNetMsg::SP& msg)
{
	int moonstone_nas;

	RefMsg(msg) >> moonstone_nas;

	if(moonstone_nas >= 0)
	{
		gserver->m_nMoonStoneNas = moonstone_nas;
	}
}

void ProcHelperEventMoonStoneJackpot(CNetMsg::SP& msg)
{
	int moonstone_nas;
	int chaindex;

	RefMsg(msg) >> moonstone_nas
		>> chaindex;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(chaindex);

	if(!ch)
		return;

	ch->m_inventory.decreaseMoney(moonstone_nas);

	GAMELOG << init("CHANGE_MOONSTONE", ch)
		<< "NAS is " << moonstone_nas << end;
}

void ProcHelper_FriendAddRep(CNetMsg::SP& msg)
{
	UpdateServer::doFriendAddRepToHelper *packet = reinterpret_cast<UpdateServer::doFriendAddRepToHelper*>(msg->m_buf);

	CPC* appPc = PCManager::instance()->getPlayerByCharIndex(packet->approvalindex);
	CPC* reqPc = PCManager::instance()->getPlayerByCharIndex(packet->requesterindex);

	if (!appPc || !reqPc)
	{
		return;
	}

	if((MSG_FRIEND_ERROR_TYPE)packet->errorCode == MSG_FRIEND_ERROR_GAMESERVER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, (MSG_FRIEND_ERROR_TYPE)packet->errorCode);//요청자에게 성공여부를 알린다.
		SEND_Q(rmsg, reqPc->m_desc);
		reqPc->m_nRegFriend = 0;
		appPc->m_nRegFriend = 0;
	}
	else
	{
		if (reqPc)//동시에 접속해 있어야 한다.
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::FriendErrorMsg(rmsg, (MSG_FRIEND_ERROR_TYPE)packet->errorCode);//요청자에게 성공여부를 알린다.
				SEND_Q(rmsg, reqPc->m_desc);
			}
			reqPc->m_nRegFriend = 0;

			reqPc->m_Friend->AddFriend(packet->approvalindex, packet->appname, packet->appjob, MSG_FRIEND_CONDITION_NORMAL, 0);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::FriendAddNotify(rmsg, appPc->m_index, appPc->GetName(), appPc->m_job, appPc->m_nCondition); //승낙자에게 멤버가 추가됨을 알림
				SEND_Q(rmsg, reqPc->m_desc);
			}
		}

		if(appPc)
		{
			appPc->m_Friend->AddFriend(packet->requesterindex, packet->reqname, packet->reqjob, MSG_FRIEND_CONDITION_NORMAL, 0);
			appPc->m_nRegFriend = 0;

			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::FriendAddNotify(rmsg, reqPc->m_index, reqPc->GetName(), reqPc->m_job, reqPc->m_nCondition); //요청자에게 멤버가 추가됨을 알림.
			SEND_Q(rmsg, appPc->m_desc);
		}
	}
}

void ProcHelper_FriendDelRep(CNetMsg::SP& msg)
{
	UpdateServer::doFriendDelMemberRepToGameServer *packet = reinterpret_cast<UpdateServer::doFriendDelMemberRepToGameServer*>(msg->m_buf);
	
	//삭제 요청자...
	CPC* pPc = PCManager::instance()->getPlayerByCharIndex(packet->removerIndex);

	if(pPc)
	{
		pPc->m_Friend->RemoveFriend(packet->deleteIndex);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::FriendErrorMsg(rmsg, (MSG_FRIEND_ERROR_TYPE)packet->errorCode);
			SEND_Q(rmsg, pPc->m_desc);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::FriendDelNotify(rmsg, packet->deleteIndex);
			SEND_Q(rmsg, pPc->m_desc);
		}
	}

	//삭제 당한자...
	CPC* pPc2 = PCManager::instance()->getPlayerByCharIndex(packet->deleteIndex);

	if(pPc2)//접속해있다면...
	{
		pPc2->m_Friend->RemoveFriend(packet->removerIndex);

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendDelNotify(rmsg, packet->removerIndex);
		SEND_Q(rmsg, pPc2->m_desc);
	}
}

void ProcHelper_BlockPCRep(CNetMsg::SP& msg)
{
	int reqindex;
	int blockIndex;
	CLCString blockName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> reqindex
		>> blockIndex
		>> blockName;

	//삭제 요청자...
	CPC* pPc = PCManager::instance()->getPlayerByCharIndex(reqindex);

	if(pPc)
	{
		if( blockIndex == -1 )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			BlockPCRepMsg(rmsg, MSG_EX_MESSENGER_BLOCK_NOTCHAR, blockIndex, blockName);
			SEND_Q(rmsg, pPc->m_desc);
			return;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			if( pPc->AddBlockPC(blockIndex, blockName) )
			{
				// 블럭 요청이 성공하였다는 메세지 : 할일
				// GS->C			: errcode(c) charIndex(n) name(str)
				BlockPCRepMsg(rmsg, MSG_EX_MESSENGER_SUCCESS, blockIndex, blockName);
			}
			else
				BlockPCRepMsg(rmsg, MSG_EX_MESSENGER_ALREADY_BLOCK, blockIndex, blockName);

			SEND_Q(rmsg, pPc->m_desc);
		}
	}

	CPC* pPc2 = PCManager::instance()->getPlayerByCharIndex(blockIndex);

	if(pPc2)//접속해있다면...
	{
		CFriendMember* fMember = pPc2->m_Friend->FindFriendMember(reqindex);
		if( fMember )
		{
			fMember->SetCondition(MSG_FRIEND_CONDITION_OFFLINE);
		}

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendSetConditionMsg(rmsg, reqindex, MSG_FRIEND_CONDITION_OFFLINE);
		SEND_Q(rmsg, pPc2->m_desc);
	}
}

void ProcHelper_GiftRecvChar(CNetMsg::SP& msg)
{
	int count = 0;
	int idx[10], ctid[10], i;
	int sendUserIdx, sendCharIdx;
	int recvuserIndex, recvcharIndex;

	CLCString recvcharName( MAX_CHAR_NAME_LENGTH + 1);
	CLCString strMsg( MAX_GIFT_MESSAGE_LENGTH + 1);

	//int sendUserIdx, int sendCharIdx, int recvUserIdx, int recvCharIdx, const char* recvCharName, const char* sendMsg, int count, int idx[], int ctid[] )
	RefMsg(msg) >> sendUserIdx
		>> sendCharIdx
		>> recvuserIndex
		>> recvcharIndex
		>> recvcharName
		>> strMsg
		>> count;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(sendCharIdx);

	if( !ch )
		return;

	if( recvcharIndex == -1 )
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;

		CNetMsg::SP rmsg(new CNetMsg);
		CashItemGiftSendRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_GIFT_WRONGCHAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( count < 1 )
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;

		CNetMsg::SP rmsg(new CNetMsg);
		CashItemGiftSendRepMsg(rmsg, MSG_EX_CASHITEM_ERROR_GIFT_NOCTID);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	for( i = 0; i < count; i++ )
	{
		RefMsg(msg) >> idx[i]
			>> ctid[i];
	}

	if( gserver->isRunConnector() )
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnCashItemGiftReqMsg(rmsg, ch->m_desc->m_index, ch->m_index, ch->GetName(), strMsg, recvuserIndex, recvcharIndex, recvcharName, count, idx, ctid);
			SEND_Q(rmsg, gserver->m_connector);
		}

		GAMELOG << init("CASH_ITEM_BRING_GIFT_SEND_REQ", ch)
				<< "RECEIVER" << delim
				<< "USRIDX"  << delim
				<< recvuserIndex << delim
				<< "CHARIDX" << delim
				<< recvcharIndex << delim
				<< "CHARNAME" << delim
				<< recvcharName << delim
				<< count << delim;

		CCatalog* catalog = NULL;
		for(i = 0; i < count; i++ )
		{
			GAMELOG << idx[i] << delim
					<< ctid[i] << delim;
			catalog = gserver->m_catalogList.Find( ctid[i] );

			if( catalog )
				GAMELOG << catalog->GetName() << delim;
		}

		GAMELOG << end;
	}
	else
	{
		ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
		ch->m_billReqTime = 0;
	}
}


//상태가 바뀐걸 모든 친구에게 보낸다.
void ProcHelper_FriendSetConditionRep(CNetMsg::SP& msg)
{
	int chaindex;
	int condition;
	int reply;

	CFriendMember * pMember = NULL;

	RefMsg(msg) >> chaindex
		>> condition
		>> reply;
	if(reply == -1)
	{
		int sum, index;
		RefMsg(msg) >> sum;

		for(int i =0; i < sum; i++)
		{
			RefMsg(msg) >> index;

			CPC* pPc = PCManager::instance()->getPlayerByCharIndex(index);

			if(pPc)
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::FriendSetConditionMsg(rmsg, chaindex, condition);
					SEND_Q(rmsg, pPc->m_desc);
				}

				// 친구 리스트 업데이트
				if (pPc->m_Friend)
				{
					pMember = pPc->m_Friend->FindFriendMember( chaindex );
					if( pMember != NULL )
					{
						pMember->SetCondition( condition );
					}

					{
						//접속한 사람에게 내 자신의 상태도 보내줘야지..
						CNetMsg::SP rmsg(new CNetMsg);
						HelperFriendSetConditionMsg(rmsg, index, pPc->m_nCondition, chaindex, NULL);
						SEND_Q(rmsg, gserver->m_helper);
					}
				}
			}
		}
	}
	else
	{
		CPC* pPc = PCManager::instance()->getPlayerByCharIndex(reply);
		if(pPc)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::FriendSetConditionMsg(rmsg, chaindex, condition);
			SEND_Q(rmsg, pPc->m_desc);

			// 친구 리스트 업데이트
			pMember = pPc->m_Friend->FindFriendMember( chaindex );
			if( pMember != NULL )
			{
				pMember->SetCondition( condition );
			}
		}
	}
}

void ProcHelperWarNoticeTime(CNetMsg::SP& msg)
{
	int zoneindex;
	char month;
	char day;
	char hour;
	char min;
	RefMsg(msg) >> zoneindex
		>> month
		>> day
		>> hour
		>> min;
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarNoticeTimeMsg(rmsg, zoneindex, month, day, hour, min);
		PCManager::instance()->sendToAll(rmsg);
	}
}

void ProcHelperWarNoticeTimeRemain(CNetMsg::SP& msg)
{
	int zoneindex;
	char remain;
	RefMsg(msg) >> zoneindex
		>> remain;

	CNetMsg::SP rmsg(new CNetMsg);
	GuildWarNoticeTimeRemainMsg(rmsg, zoneindex, remain);
	PCManager::instance()->sendToAll(rmsg);
}

void ProcHelperWarNoticeStart(CNetMsg::SP& msg)
{
	int zoneindex;
	int remainSec;
	RefMsg(msg) >> zoneindex
		>> remainSec;

	CNetMsg::SP rmsg(new CNetMsg);
	GuildWarNoticeStartMsg(rmsg, zoneindex, remainSec);
	PCManager::instance()->sendToAll(rmsg);

	// 공성 상태 갱신 : 진행중인 서브 서버는 직접 바꾸므로 스킵
	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);
	if (castle && !castle->CheckSubServer())
	{
		castle->SetState(WCSF_WAR_FIELD);
	}
}

void ProcHelperWarJoinAttackGuild(CNetMsg::SP& msg)
{
	int zoneindex;
	int guildindex;

	RefMsg(msg) >> zoneindex
		>> guildindex;

	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);
	if (!castle)
		return ;

	if (castle->IsAttackGuild(guildindex))
		return ;

	if (castle->GetOwnerGuildIndex() == guildindex)
		return ;

	if (castle->IsDefenseGuild(guildindex))
		castle->RemoveDefenseGuild(guildindex);

	castle->AddAttackGuild(guildindex);

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	int i;
	int guildMaxMember = guild->maxmember();
	for (i = 0; i < guildMaxMember; i++)
	{
		if (guild->member(i))
		{
			if (castle->IsAttackChar(guild->member(i)->charindex()))
				castle->RemoveAttackChar(guild->member(i)->charindex());
			if (castle->IsDefenseChar(guild->member(i)->charindex()))
				castle->RemoveDefenseChar(guild->member(i)->charindex());

			if (guild->member(i)->GetPC())
			{
				guild->member(i)->GetPC()->SetJoinFlag(zoneindex, WCJF_ATTACK_GUILD);
				guild->member(i)->GetPC()->m_bChangeStatus = true;
			}
		}
	}
}

void ProcHelperWarJoinDefenseGuild(CNetMsg::SP& msg)
{
	int zoneindex;
	int guildindex;

	RefMsg(msg) >> zoneindex
		>> guildindex;

	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);
	if (!castle)
		return ;

	if (castle->IsDefenseGuild(guildindex))
		return ;

	if (castle->GetOwnerGuildIndex() == guildindex)
		return ;

	if (castle->IsAttackGuild(guildindex))
		return ;

	castle->AddDefenseGuild(guildindex);

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	int i;
	int guildMaxMember = guild->maxmember();
	for (i = 0; i < guildMaxMember; i++)
	{
		if (guild->member(i))
		{
			if (castle->IsAttackChar(guild->member(i)->charindex()))
				castle->RemoveAttackChar(guild->member(i)->charindex());
			if (castle->IsDefenseChar(guild->member(i)->charindex()))
				castle->RemoveDefenseChar(guild->member(i)->charindex());

			if (guild->member(i)->GetPC())
			{
				guild->member(i)->GetPC()->SetJoinFlag(zoneindex, WCJF_DEFENSE_GUILD);
				guild->member(i)->GetPC()->m_bChangeStatus = true;
			}
		}
	}
}

void ProcHelperWarNoticeStartAttackCastle(CNetMsg::SP& msg)
{
	int zoneindex;
	int remainSec;
	int guildindex1 = -1;
	int guildindex2 = -1;
	int guildindex3 = -1;
	CLCString guildname1(MAX_GUILD_NAME_LENGTH + 1);
	CLCString guildname2(MAX_GUILD_NAME_LENGTH + 1);
	CLCString guildname3(MAX_GUILD_NAME_LENGTH + 1);

	RefMsg(msg) >> zoneindex
		>> remainSec
		>> guildindex1
		>> guildname1
		>> guildindex2
		>> guildname2
		>> guildindex3
		>> guildname3;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarNoticeStartCastleMsg(rmsg, zoneindex, remainSec, guildindex1, guildname1, guildindex2, guildname2, guildindex3, guildname3);
		PCManager::instance()->sendToAll(rmsg);
	}

	// 공성 상태 갱신 : 진행중인 서브 서버는 직접 바꾸므로 스킵
	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);
	if (castle && !castle->CheckSubServer())
	{
		castle->SetState(WCSF_WAR_CASTLE);
	}

	// 포인트 변경 알림
	if  (castle != NULL)
	{
		PCManager::instance()->sendGuildWarPointMsg(castle, zoneindex);
		castle->SetGuildGradeSkillTime( gserver->getNowSecond() );
	}

}

void ProcHelperWarNoticeRemainFieldTime(CNetMsg::SP& msg)
{
	int zoneindex;
	int remainSec;

	RefMsg(msg) >> zoneindex
		>> remainSec;

	CNetMsg::SP rmsg(new CNetMsg);
	GuildWarNoticeRemainFieldTimeMsg(rmsg, zoneindex, remainSec);
	PCManager::instance()->sendToWarJoinUser(rmsg, zoneindex, false);
}

void ProcHelperWarNoticeEnd(CNetMsg::SP& msg)
{
	int zoneindex;
	char bWinDefense;
	int ownerguildindex;
	CLCString ownerguildname(MAX_GUILD_NAME_LENGTH + 1);
	int ownercharindex;
	CLCString ownercharname(MAX_CHAR_NAME_LENGTH + 1);
	int nextmonth;
	int nextday;
	int nexthour;
	int nextwday;

	RefMsg(msg) >> zoneindex
		>> bWinDefense
		>> ownerguildindex
		>> ownerguildname
		>> ownercharindex
		>> ownercharname
		>> nextmonth
		>> nextday
		>> nexthour
		>> nextwday;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarEndMsg(rmsg, zoneindex, bWinDefense, ownerguildindex, ownerguildname, ownercharindex, ownercharname, nextmonth, nextday, nexthour, nextwday);
		PCManager::instance()->sendToAll(rmsg);
	}

	PCManager::map_t& playerMap			= PCManager::instance()->getPlayerMap();
	PCManager::map_t::iterator iter		= playerMap.begin();
	PCManager::map_t::iterator endIter	= playerMap.end();
	for (; iter != endIter; ++iter)
	{
		CPC* pc = (*iter).pPlayer;
		if (pc)
		{
			// 바로 보내기
			CNetMsg::SP rmsg(new CNetMsg);
			StatusMsg(rmsg, pc);
			SEND_Q(rmsg, pc->m_desc );
		}
	}
	// 공성 상태 갱신 : 진행중인 서브 서버는 직접 바꾸므로 스킵
	CWarCastle* castle = CWarCastle::GetCastleObject(zoneindex);

	if(castle != NULL)
	{
		if( zoneindex == ZONE_DRATAN )
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				CastletowerQuartersListMsg( rmsg, (CDratanCastle*)castle );
				castle->GetArea()->SendToAllClient(rmsg);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildWarGateStateMsg(rmsg, 0xffffffff, 0xffffffff);
				castle->GetArea()->SendToAllClient(rmsg);
			}
		}


		if (!castle->CheckSubServer()
			&& zoneindex == ZONE_MERAC)
		{
			// 상태 변경
			castle->SetState(WCSF_NORMAL);

			// 시간 설정
			castle->m_lastWarTime = castle->GetNextWarTime() + WCT_WAR;
			castle->SetNextWarTime(0);

			// 성주 설정
			if (!bWinDefense)
			{
				if (ownerguildindex > 0)
				{
					castle->m_ownerGuildIndex = ownerguildindex;
					castle->m_ownerGuildName = ownerguildname;
					castle->m_ownerCharIndex = ownercharindex;
					castle->m_ownerCharName = ownercharname;
				}
				else
					castle->ResetOwner();
			}

			// 기타
			castle->SetNoticeWarTime(false);
			castle->SetNoticeRemain(9999);
			castle->m_defensePoint = 0;
			castle->m_gateState = 0;

			// 참여정보 초기화
			castle->ResetJoinFlag();

			// 참여 리스트 초기화
			castle->RemoveAllJoinList();
		}

		castle->EndWarRegenPoint();
	}

	// 공성 상태 갱신 : 진행중인 서브 서버는 직접 바꾸므로 스킵
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (!pCastle->CheckSubServer() && zoneindex == ZONE_DRATAN)
	{
		// 상태 변경
		pCastle->SetState(WCSF_NORMAL);

		// 시간 설정
		pCastle->m_lastWarTime = pCastle->GetNextWarTime() + WCT_WAR;
		pCastle->SetNextWarTime(0);

		// 성주 설정
		if (!bWinDefense)
		{
			int i;
			CGuild* guild;
			int prevOwnerGuildIndex = pCastle->m_ownerGuildIndex;

			if (ownerguildindex > 0)
			{
				pCastle->m_ownerGuildIndex = ownerguildindex;
				pCastle->m_ownerGuildName = ownerguildname;
				pCastle->m_ownerCharIndex = ownercharindex;
				pCastle->m_ownerCharName = ownercharname;
			}
			else
				pCastle->ResetOwner();

			// 성주가 바꿔서 길드명색을 다시 보내줌
			guild = gserver->m_guildlist.findguild(prevOwnerGuildIndex);
			if(guild)
			{
				int guildMaxMember = guild->maxmember();
				for(i=0; i<guildMaxMember; i++)
				{
					if(guild->member(i) && guild->member(i)->GetPC())
					{
						GuildNameColorStateMsg(msg, guild->member(i)->GetPC() );
						SEND_Q(msg , guild->member(i)->GetPC()->m_desc);
					}
				}
			}
			guild = gserver->m_guildlist.findguild(pCastle->m_ownerGuildIndex);
			if(guild)
			{
				int guildMaxMember = guild->maxmember();
				for(i=0; i<guildMaxMember; i++)
				{
					if(guild->member(i) && guild->member(i)->GetPC())
					{
						GuildNameColorStateMsg(msg, guild->member(i)->GetPC() );
						SEND_Q(msg , guild->member(i)->GetPC()->m_desc);
					}
				}
			}
		}

		// 기타
		pCastle->SetNoticeWarTime(false);
		pCastle->SetNoticeRemain(9999);
		pCastle->m_gateState = 0;

		// 참여정보 초기화
		pCastle->ResetJoinFlag();

		// 참여 리스트 초기화
		pCastle->RemoveAllJoinList();
	}
}

void ProcHelperPetCreateRep(CNetMsg::SP& msg)
{
	int index;
	int owner;
	char typeGrade;

	RefMsg(msg) >> index
		>> owner
		>> typeGrade;

	bool bSuccess = false;
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(owner);
	if (pc)
	{
		int itemdbindex = 0;

		const char* petTypeName;
		switch (typeGrade & PET_TYPE_MASK)
		{
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
		default:
			return ;
		}

		// Helper의 펫 지급 실패시 펫 아이템을 삭제하고 피리지급
		if( index == -1 )
		{
			CItem* item = pc->m_inventory.FindByDBIndex(itemdbindex, 0, 0);
			if( item )
			{
				pc->m_inventory.decreaseItemCount(item, 1);

				int egg_index = 0;
				switch(itemdbindex)
				{
				case PET_HORSE_ITEM_INDEX:
					egg_index = PET_HORSE_EGG_INDEX;
					break;
				case PET_BLUE_HORSE_ITEM_INDEX:
					egg_index = PET_BLUE_HORSE_EGG_INDEX;
					break;
				case PET_UNKOWN_HORSE_ITEM_INDEX:
					egg_index = PET_UNKOWN_HORSE_EGG_INDEX;
					break;
				case PET_DRAGON_ITEM_INDEX:
					egg_index = PET_DRAGON_EGG_INDEX;
					break;
				case PET_PINK_DRAGON_ITEM_INDEX:
					egg_index = PET_PINK_DRAGON_EGG_INDEX;
					break;
				case PET_UNKOWN_DRAGON_ITEM_INDEX:
					egg_index = PET_UNKOWN_DRAGON_EGG_INDEX;
					break;
				}
				pc->GiveItem(egg_index, 0, 0, 1, true);
			}

			return;
		}

		CPet* pet = new CPet(pc, index, typeGrade, 1);
		if (pet)
		{
			// 펫의 이미 지급한 아이템의 옵션을 변경한닷
			CItem* item = pc->m_inventory.FindByDBIndex(itemdbindex, 0, 0);
			if( item )
			{
				item->unWearPos();
				item->setPlus(index);
				item->setFlag(0);

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPetStatusMsg(rmsg, pet);
					SEND_Q(rmsg, pc->m_desc);
				}
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPetSkillListMsg(rmsg, pet);
					SEND_Q(rmsg, pc->m_desc);
				}

				pc->m_inventory.sendOneItemInfo(item);

				ADD_TO_BILIST(pet, pc->m_petList, m_prevPet, m_nextPet);
				bSuccess = true;

				// TODO : petlog
				GAMELOG << init("PET CREATE", pc)
						<< "TYPE" << delim
						<< petTypeName
						<< end;

			}

			if (!bSuccess)
			{
				if (item)
					delete item;
				delete pet;
			}
		}
	}
}

void ProcHelperPetDeleteRep(CNetMsg::SP& msg)
{
	int index;
	int owner;

	RefMsg(msg) >> index
		>> owner;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(owner);
	if (pc)
	{
		pc->DelPet(index);
	}
}


void ProcHelperGuildStashHistoryRep(CNetMsg::SP& msg)
{
	int charindex;
	int errcode;
	int month[7] = {0, 0, 0, 0, 0, 0, 0};
	int day[7] = {0, 0, 0, 0, 0, 0, 0};
	LONGLONG money[7] = {0, 0, 0, 0, 0, 0, 0};

	RefMsg(msg) >> charindex
		>> errcode;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (!pc)
		return ;

	if (errcode == MSG_HELPER_GUILD_STASH_ERROR_OK)
	{
		int i;
		for (i = 0; i < 7; i++)
		{
			RefMsg(msg) >> month[i]
				>> day[i]
				>> money[i];
		}
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildStashHistoryRepMsg(rmsg, (MSG_GUILD_STASH_ERROR_TYPE)errcode, month, day, money);
		SEND_Q(rmsg, pc->m_desc);
	}
}

void ProcHelperGuildStashViewRep(CNetMsg::SP& msg)
{
	int charindex;
	int errcode;
	LONGLONG money = 0;

	RefMsg(msg) >> charindex
		>> errcode
		>> money;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (!pc)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildStashViewRepMsg(rmsg, (MSG_GUILD_STASH_ERROR_TYPE)errcode, money);
		SEND_Q(rmsg, pc->m_desc);
	}
}

void ProcHelperGuildStashTakeRep(CNetMsg::SP& msg)
{
	int guildindex;
	int charindex;
	int errcode;
	LONGLONG money = 0;
	LONGLONG balance = 0;

	RefMsg(msg) >> guildindex
		>> charindex
		>> errcode
		>> money
		>> balance;

	bool bRollback = false;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (!pc)
	{
		// 찾으려던 사람이 없어서 롤백
		bRollback = true;
	}
	else
	{
		if (errcode == MSG_HELPER_GUILD_STASH_ERROR_OK)
		{
			// 돈을 생성 지급
			pc->m_inventory.increaseMoney(money);

			{
				// 성공 메시지 전달
				CNetMsg::SP rmsg(new CNetMsg);
				GuildStashTakeRepMsg(rmsg, MSG_GUILD_STASH_ERROR_OK);
				SEND_Q(rmsg, pc->m_desc);
			}

			GAMELOG << init("GUILD STASH TAKE MONEY", pc)
				<< "GUILD" << delim
				<< guildindex << delim
				<< "MONEY" << delim
				<< money << delim
				<< "BALANCE" << delim
				<< balance
				<< end;
		}
		else
		{
			// 실패 알림
			GAMELOG << init("GUILD STASH TAKE MONEY FAIL", pc)
					<< "GUILD" << delim
					<< guildindex << delim
					<< "ERROR" << delim
					<< errcode
					<< end;

			CNetMsg::SP rmsg(new CNetMsg);
			GuildStashTakeRepMsg(rmsg, (MSG_GUILD_STASH_ERROR_TYPE)errcode);
			SEND_Q(rmsg, pc->m_desc);
		}
	}

	if (bRollback && errcode == MSG_HELPER_GUILD_STASH_ERROR_OK)
	{
		if (gserver->isRunHelper())
		{
			if (pc)
				GAMELOG << init("GUILD STASH ROLLBACK TAKE MONEY", pc);
			else
				GAMELOG << init("GUILD STASH ROLLBACK TAKE MONEY") << "CHARINDEX" << delim << charindex << delim;
			GAMELOG << "GUILD" << delim
					<< guildindex << delim
					<< "MONEY" << delim
					<< money << delim
					<< "BALANCE" << delim
					<< balance
					<< end;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperGuildStashRollbackMsg(rmsg, guildindex, money);
				SEND_Q(rmsg, gserver->m_helper);
			}
		}
		else
		{
			if (pc)
				GAMELOG << init("GUILD STASH FAIL ROLLBACK TAKE MONEY", pc);
			else
				GAMELOG << init("GUILD STASH FAIL ROLLBACK TAKE MONEY") << "CHARINDEX" << delim << charindex << delim;
			GAMELOG << "GUILD" << delim
					<< guildindex << delim
					<< "MONEY" << delim
					<< money << delim
					<< "BALANCE" << delim
					<< balance
					<< end;
		}
	}
}

void ProcHelperGuildStashSaveTaxRep(CNetMsg::SP& msg)
{
	int errcode;
	int guildindex = 0;
	int zoneindex = 0;
	LONGLONG taxItem;
	LONGLONG taxProduce;

	RefMsg(msg) >> errcode
		>> guildindex
		>> zoneindex
		>> taxItem
		>> taxProduce;

	switch (errcode)
	{
	case MSG_HELPER_GUILD_STASH_ERROR_OK:
		{
			CDBCmd cmd;
			cmd.Init(&gserver->m_dbcastle);
			struct tm tmCur = NOW();
			std::string update_castle_query = boost::str(boost::format(
				"UPDATE t_castle SET a_tax_wday=%1%, a_tax_item = a_tax_item - %2%, a_tax_produce = a_tax_produce - %3% WHERE a_zone_index = %4%") % tmCur.tm_wday % taxItem % taxProduce % zoneindex);
			cmd.SetQuery(update_castle_query);
			if (!cmd.Update())
			{
				GAMELOG << init("TAX RESET FAIL")
						<< "ZONE" << delim
						<< zoneindex << delim
						<< "GUILD" << delim
						<< guildindex << delim
						<< "TAX ITEM" << delim
						<< taxItem << delim
						<< "TAX PRODUCT" << delim
						<< taxProduce
						<< end;
			}
			else
			{
				GAMELOG << init("TAX RESET SUCCESS")
						<< "ZONE" << delim
						<< zoneindex << delim
						<< "GUILD" << delim
						<< guildindex << delim
						<< "TAX ITEM" << delim
						<< taxItem << delim
						<< "TAX PRODUCT" << delim
						<< taxProduce
						<< end;
			}
		}
		break;

	default:
		GAMELOG << init("TAX SAVE FAIL")
				<< "ZONE" << delim
				<< zoneindex << delim
				<< "GUILD" << delim
				<< guildindex << delim
				<< "TAX ITEM" << delim
				<< taxItem << delim
				<< "TAX PRODUCT" << delim
				<< taxProduce
				<< end;
		break;
	}
}

void ProcHelperTeacherLoadRep(CNetMsg::SP& msg)
{
	CLCString idname(MAX_ID_NAME_LENGTH + 1);
	int charindex;

	RefMsg(msg) >> idname
		>> charindex;

	CDescriptor* d = DescManager::instance()->getDescById(idname);
	if( d && d->m_pChar && d->m_pChar->m_index )
	{
		char bteacher;
		RefMsg(msg) >> d->m_pChar->m_cntTeachingStudent
			 >> d->m_pChar->m_guardian
			 >> d->m_pChar->m_superstone
			 >> d->m_pChar->m_cntFailStudent
			 >> d->m_pChar->m_cntCompleteStudent
			 >> d->m_pChar->m_teachType
			 >> d->m_pChar->m_fame
			 >> bteacher;
		if(bteacher)
			d->m_pChar->m_bTeacher = true;
		else
			d->m_pChar->m_bTeacher = false;

		if( d->m_pChar->m_teachType == MSG_TEACH_NO_STUDENT_TYPE)
			return;

		if( d->m_pChar->m_teachType == MSG_TEACH_TEACHER_TYPE )
		{
			RefMsg(msg) >> d->m_pChar->m_teachTime[0]
				>> d->m_pChar->m_teachTime[1];
			CLCString teachName(MAX_CHAR_NAME_LENGTH + 1);
			for(int i = 0; i < TEACH_MAX_STUDENTS; i++)
			{
				RefMsg(msg) >> d->m_pChar->m_teachIdx[i]
					>> d->m_pChar->m_teachJob[i]
					>> d->m_pChar->m_teachJob2[i]
					>> d->m_pChar->m_teachLevel[i]
					>> teachName;
				strcpy(d->m_pChar->m_teachName[i], teachName);

				if( d->m_pChar->m_teachIdx[i] != -1 )
				{
					CPC* student = PCManager::instance()->getPlayerByCharIndex(d->m_pChar->m_teachIdx[i]);
					if( student )
					{
						d->m_pChar->m_teachIdx[i] = student->m_index;
						d->m_pChar->m_teachJob[i] = student->m_job;
						d->m_pChar->m_teachJob2[i] = student->m_job2;
						d->m_pChar->m_teachLevel[i] = student->m_level;
						strcpy(d->m_pChar->m_teachName[i], student->GetName());

						{
							// 접속해 있으면 로그인 했다고 보내주자.
							CNetMsg::SP rmsg(new CNetMsg);
							TeachLoginMsg(rmsg, d->m_pChar);
							SEND_Q(rmsg, student->m_desc);
						}
					}
				}
			}
		}
		else if( d->m_pChar->m_teachType == MSG_TEACH_STUDENT_TYPE ) // 학생
		{
			CLCString teachName(MAX_CHAR_NAME_LENGTH + 1);
			RefMsg(msg) >> d->m_pChar->m_teachIdx[0]
				>> d->m_pChar->m_teachJob[0]
				>> d->m_pChar->m_teachJob2[0]
				>> d->m_pChar->m_teachLevel[0]
				>> teachName
				>> d->m_pChar->m_teachTime[0];
			strcpy(d->m_pChar->m_teachName[0], teachName);

			// 자신이 학생이면 idx[1]을 선생의 명성수치로, idx[2]를 선생의 양성중이 초보로, time[1]을 완료인원, time[2]을 실패인원으로 쓴다.
			RefMsg(msg) >> d->m_pChar->m_teachIdx[1]
				>> d->m_pChar->m_teachIdx[2]
				>> d->m_pChar->m_teachTime[1]
				>> d->m_pChar->m_teachTime[2];

			if( d->m_pChar->m_teachIdx[0] != -1 )
			{
				CPC* teacher = PCManager::instance()->getPlayerByCharIndex(d->m_pChar->m_teachIdx[0]);
				if( teacher )
				{
					d->m_pChar->m_teachIdx[0] = teacher->m_index;
					d->m_pChar->m_teachJob[0] = teacher->m_job;
					d->m_pChar->m_teachJob2[0] = teacher->m_job2;
					d->m_pChar->m_teachLevel[0] = teacher->m_level;
					strcpy(d->m_pChar->m_teachName[0], teacher->GetName());

					{
						// 접속해 있으면 로그인 했다고 보내주자.
						CNetMsg::SP rmsg(new CNetMsg);
						TeachLoginMsg(rmsg, d->m_pChar);
						SEND_Q(rmsg, teacher->m_desc);
					}
				}
			}
		}
		else // MSG_TEACH_LIMITE_DAY_FAIL || MSG_TEACH_NO_TYPE,
		{
			RefMsg(msg) >> d->m_pChar->m_teachTime[0];
		}
	}
}

void ProcHelperFameupRep(CNetMsg::SP& msg)
{
	int teachidx;
	int fameup;

	RefMsg(msg) >> teachidx
		>> fameup;

	CPC* tch = PCManager::instance()->getPlayerByCharIndex(teachidx);
	if(tch)
	{
		GAMELOG << init("TEACH_FAME_UP")
				<< tch->m_name << delim
				<< tch->m_nick << delim
				<< tch->m_desc->m_idname << delim
				<< tch->m_fame << end;

#if defined( EVENT_TEACH ) || defined ( EVENT_CHILDERN_DAY )
		tch->m_fame = tch->m_fame + fameup * 2;
#else
		tch->m_fame = tch->m_fame + fameup;
#endif

		//0627
		if( tch->m_displayCanSstone
			&& ((tch->m_fame >= 200  && tch->m_superstone < 1 )
			|| (tch->m_fame >= 300 && tch->m_superstone < 2 )
			|| (tch->m_fame >= 500 && tch->m_superstone < 3)
			|| (tch->m_fame >= 800 && tch->m_superstone < 4)
			|| (tch->m_fame >= 1200 && tch->m_superstone < 5)
			|| (tch->m_fame >= 1700 && tch->m_superstone < 6)
			|| (tch->m_fame >= 2300 && tch->m_superstone < 7)) )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysEnableSuperStoneMsg(rmsg, tch->m_fame);
			SEND_Q(rmsg, tch->m_desc);
			tch->m_displayCanSstone = false;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			StatusMsg(rmsg, tch);
			SEND_Q(rmsg, tch->m_desc);
		}
	}
	else
	{
		GAMELOG << init("TEACH_FAME_UP_HELPER_REP-notTeacher")
				<< teachidx << delim
				<< fameup << end;
	}
}

void ProcHelperPartyMemberChangeJob(CNetMsg::SP& msg)
{
	int nBossIndex;
	int nCharIndex;
	char job1;
	char job2;

	RefMsg(msg) >> nBossIndex
		>> nCharIndex
		>> job1
		>> job2;

	CParty* pParty = gserver->FindPartyByBossIndex(nBossIndex);
	if (pParty)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJob(rmsg, nCharIndex, job1, job2);
		pParty->SendToAllPC(rmsg, nCharIndex);
	}
}

void ProcHelperPartyChat(CNetMsg::SP& msg)
{
	int nBossIndex;
	int nCharIndex;
	CLCString strName(MAX_CHAR_NAME_LENGTH + 1);
	CLCString strChat(1000);

	RefMsg(msg) >> nBossIndex
		>> nCharIndex
		>> strName
		>> strChat;

	CParty* pParty = gserver->FindPartyByBossIndex(nBossIndex);
	if (pParty)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_PARTY, nCharIndex, strName, "", strChat);
			pParty->SendToAllPC(rmsg);
		}
#ifdef GMTOOL
		if(pParty->m_bGmtarget)
		{
			bool bSend = false;
			for(int i = 0; i < MAX_PARTY_MEMBER; ++i)
			{
				const CPartyMember* member = pParty->GetMemberByListIndex(i);
				if(member && member->GetMemberPCPtr() && member->GetMemberPCPtr()->m_bGmMonitor)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					MsgrNoticeGmChatMonitorPartyMsg(rmsg, -1, 1, 1, 0, gserver->m_serverno, gserver->m_subno, -1, strChat, nBossIndex, strName, member->GetMemberPCPtr()->m_index, member->GetMemberPCPtr()->m_name);
					SEND_Q(rmsg, gserver->m_messenger);

					bSend = true;
				}
			}

			if(!bSend)
			{
				pParty->m_bGmtarget = false;
			}
		}
#endif // GMTOOL
	}
}

void ProcHelperPartyRecallPrompt(CNetMsg::SP& msg)
{
	int nBossIndex;
	int nReqIndex;
	CLCString strReqName(MAX_CHAR_NAME_LENGTH + 1);
	char cIsInCastle = 0;
	int nGuildIndex = 0;
	char bUseContinent = false;
	int zoneID = -1;
	CPos pos(0, 0, 0, 0, 0);

	RefMsg(msg) >> nBossIndex
		>> nReqIndex
		>> strReqName
		>> cIsInCastle
		>> nGuildIndex
		>> bUseContinent
		>> zoneID
		>> pos.m_x
		>> pos.m_z
		>> pos.m_yLayer;

	CParty* pParty = gserver->FindPartyByBossIndex(nBossIndex);
	if (pParty == NULL)
	{
		return;
	}
		
	pParty->SetPartyRecallUseInfo(zoneID, pos, nGuildIndex, cIsInCastle, nReqIndex);

	CNetMsg::SP rmsg(new CNetMsg);
	ExPartyRecallPromptMsg(rmsg, nReqIndex, strReqName);
#ifdef CONTINENT_PARTY_RECALL
	if(bUseContinent == false)
	{
		pParty->SendToPartyRecallPC(rmsg, nReqIndex);
	}
	else
	{
		pParty->SendToSameContinentPC(rmsg, nReqIndex);
	}
#else

	pParty->SendToPartyRecallPC(rmsg, nReqIndex);

#endif
}

void ProcHelperPartyRecallConfirm(CNetMsg::SP& msg)
{
	int nBossIndex;
	int reqindex;
	int repindex;
	char yesno;
	CLCString strReqName(MAX_CHAR_NAME_LENGTH + 1);
	CLCString strRepName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> nBossIndex
		>> reqindex
		>> strReqName
		>> repindex
		>> strRepName
		>> yesno;

	CPC* pReqPC = PCManager::instance()->getPlayerByCharIndex(reqindex);
	CPC* pResPC = PCManager::instance()->getPlayerByCharIndex(repindex);

	if (pResPC == NULL || pResPC->m_party == NULL)
		return;

	
	CPartyMember* pPartyMember = pResPC->m_party->GetMemberByCharIndex(reqindex);
	if (pPartyMember == NULL)
	{
		// 취소 명령 전달
		CNetMsg::SP rmsg(new CNetMsg);
		WarpErrorMsg(rmsg, MSG_WARP_ERROR_NOTCHAR, strReqName);
		SEND_Q(rmsg, pResPC->m_desc);
		pResPC->getPartyRecallInfo()->resetPartyRecallInfo();
		return;
	}

	int moveZoneID = pResPC->getPartyRecallInfo()->getZoneID();
	CPos* pos = pResPC->getPartyRecallInfo()->getPos();
	int guildIndex	= pResPC->getPartyRecallInfo()->getGuildIndex();
	char cIsInCastle = pResPC->getPartyRecallInfo()->getIsInCastle();

	if (yesno)
	{
		if (moveZoneID == ZONE_MONDSHINE)
		{
			if( !(pResPC->m_questList.FindQuest(249, QUEST_STATE_DONE) || pResPC->m_questList.FindQuest(249, QUEST_STATE_RUN)) )
			{
				if (pReqPC != NULL)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPartyRecallCancelMsg(rmsg, reqindex, strReqName, repindex, strRepName);
					SEND_Q(rmsg, pReqPC->m_desc);
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					WarpCancelMsg(rmsg, pResPC);
					SEND_Q(rmsg, pResPC->m_desc);
				}
				pResPC->getPartyRecallInfo()->resetPartyRecallInfo();
				return ;
			}
		}

		if( moveZoneID != -1 )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyRecallProcMsg(rmsg, nBossIndex, repindex, moveZoneID, pos, cIsInCastle, guildIndex);
			SEND_Q(rmsg, gserver->m_helper);
			pResPC->getPartyRecallInfo()->resetPartyRecallInfo();
		}
		else
		{
			// 존을 못찾아 거절로 전송
			if (pReqPC != NULL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyRecallCancelMsg(rmsg, reqindex, strReqName, repindex, strRepName);
				SEND_Q(rmsg, pReqPC->m_desc);
			}

			{
				pResPC->getPartyRecallInfo()->resetPartyRecallInfo();
				CNetMsg::SP rmsg(new CNetMsg);
				HelperPartyRecallConfirmFail(rmsg);
				SEND_Q(rmsg, pResPC->m_desc);
			}

		}
	}
	else
	{
		if (pReqPC != NULL)
		{
			// 존을 못찾아 거절로 전송
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyRecallCancelMsg(rmsg, reqindex, strReqName, repindex, strRepName);
				SEND_Q(rmsg, pReqPC->m_desc);
			}
			{
				pResPC->getPartyRecallInfo()->resetPartyRecallInfo();
				CNetMsg::SP rmsg(new CNetMsg);
				HelperPartyRecallConfirmFail(rmsg);
				SEND_Q(rmsg, pResPC->m_desc);
			}
		}
	}

	pResPC->m_party->deletePartyRecallMember(repindex);
}

void ProcHelperPartyRecallProc(CNetMsg::SP& msg)
{
	int nBossIndex;
	int nRepIndex;
	int nZone;
	float x;
	float z;
	char nYlayer;
	char cIsInCastle = 0;
	int nGuildIndex = 0;

	RefMsg(msg) >> nBossIndex
		>> nRepIndex
		>> nZone
		>> x
		>> z
		>> nYlayer
		>> cIsInCastle
		>> nGuildIndex;

	CPC* pRepPC =  PCManager::instance()->getPlayerByCharIndex(nRepIndex);
	if (pRepPC)
	{
		if (pRepPC->IsParty() && pRepPC->m_party->GetBossIndex() == nBossIndex)
		{
			if (cIsInCastle)
			{
				if (pRepPC->m_guildInfo && pRepPC->m_guildInfo->guild() && pRepPC->m_guildInfo->guild()->index() == nGuildIndex)
				{
					if((pRepPC->m_pZone->m_index != nZone))
					{
						pRepPC->SetPlayerState(PLAYER_STATE_CASH_ZONE_MOVE);
					}

					if(!GoZone(pRepPC, nZone, nYlayer, x, z))
					{
						if(pRepPC->IsSetPlayerState(PLAYER_STATE_CASH_ZONE_MOVE))
							pRepPC->ResetPlayerState(PLAYER_STATE_CASH_ZONE_MOVE);
					}
				}
				else
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperPartyRecallConfirmFail(rmsg);
					SEND_Q(rmsg, pRepPC->m_desc);
					return ;
				}
			}
			else
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					WarpStartMsg(rmsg, pRepPC);
					SEND_Q(rmsg, pRepPC->m_desc);
				}
				
				if((pRepPC->m_pZone->m_index != nZone))
				{
					pRepPC->SetPlayerState(PLAYER_STATE_CASH_ZONE_MOVE);
				}

				if(!GoZone(pRepPC, nZone, nYlayer, x, z))
				{
					if(pRepPC->IsSetPlayerState(PLAYER_STATE_CASH_ZONE_MOVE))
						pRepPC->ResetPlayerState(PLAYER_STATE_CASH_ZONE_MOVE);
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					WarpEndMsg(rmsg, pRepPC);
					SEND_Q(rmsg, pRepPC->m_desc);
				}
			}
		}
	}
}

void ProcHelperPartyInviteReq(CNetMsg::SP& msg)
{
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nBossLevel;
	int			nTargetIndex;
	char		nPartyType;

	RefMsg(msg) >> nBossIndex
		>> strBossName
		>> nBossLevel
		>> nTargetIndex
		>> nPartyType;

	GAMELOG << init("PARTY DEBUG HELPER INVITE REQ")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "strBossName" << delim << strBossName << delim
			<< "nBossLevel" << delim << nBossLevel << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "nPartyType" << delim << nPartyType
			<< end;

	CPC* pTargetPC = PCManager::instance()->getPlayerByCharIndex(nTargetIndex);

	if (!pTargetPC)
		return ;

	CPC* pBossPC = PCManager::instance()->getPlayerByCharIndex(nBossIndex);
	if (!pBossPC)
		return ;

	if (pTargetPC->m_Exped || pBossPC->m_Exped )	// 원정대 상태에서 파티초대 불가
		return;

#if defined (LC_USA)

	if(pBossPC->IsSetPlayerState(PLAYER_STATE_PKMODE) || pTargetPC->IsSetPlayerState(PLAYER_STATE_PKMODE))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, pTargetPC->GetName(), nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_PVP, pTargetPC->m_level);
		SEND_Q(rmsg, gserver->m_helper);
		return ;
	}
#endif	// #if defined (LC_USA)

	// 파티 타입이 2이면 tch와 ch의 레벨 차이를 검사
	// 차이가 +- 10이상이면 ERROR;
	if (nPartyType == MSG_PARTY_TYPE_BATTLE)
	{
		if (ABS(pTargetPC->m_level - nBossLevel) > 10)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, pTargetPC->GetName(), nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_LEVEL, pTargetPC->m_level);
			SEND_Q(rmsg, gserver->m_helper);
			return ;
		}
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, pTargetPC->GetName(), nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_OK, pTargetPC->m_level);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void ProcHelperPartyInviteRep(CNetMsg::SP& msg)
{
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);
	char		cPartyType1, cPartyType2, cPartyType3;
	int			nErrorCode;

	RefMsg(msg) >> nBossIndex
		>> strBossName
		>> nTargetIndex
		>> strTargetName
		>> cPartyType1
		>> cPartyType2
		>> cPartyType3
		>> nErrorCode;

	GAMELOG << init("PARTY DEBUG HELPER INVITE REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "strBossName" << delim << strBossName << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "strTargetName" << delim << strTargetName << delim
			<< "nPartyType1" << delim << cPartyType1 << delim
			<< "nErrorCode" << delim << nErrorCode
			<< end;

	CPC* pBossPC =  PCManager::instance()->getPlayerByCharIndex(nBossIndex);
	CPC* pTargetPC =  PCManager::instance()->getPlayerByCharIndex(nTargetIndex);
	CParty* pParty = NULL;

	// 파티 관련 서버 다운되는 현상 방지
	if(!pBossPC || !pTargetPC)
	{
		GAMELOG << init("PARTY Error : NULL Check 1") << end;
		return;
	}

	switch (nErrorCode)
	{
	case MSG_HELPER_PARTY_ERROR_INVITE_OK:
		{
			pParty = gserver->FindPartyByBossIndex(nBossIndex);
			if (pParty)
			{
				// 기존 파티에 초대 상태로
				pParty->SetRequest(nTargetIndex, strTargetName);
				if (pTargetPC)
					pTargetPC->m_party = pParty;
			}
			else
			{
				// 신규 파티
#ifdef PARTY_ONLINE
				pParty = new CParty(cPartyType1, nBossIndex, strBossName, pBossPC, nTargetIndex, strTargetName, pBossPC->m_level );
#else
				pParty = new CParty(cPartyType1, nBossIndex, strBossName, pBossPC, nTargetIndex, strTargetName);
#endif // PARTY_ONLINE

				if (pBossPC)		pBossPC->m_party = pParty;
				if (pTargetPC)		pTargetPC->m_party = pParty;

				gserver->m_listParty.insert(map_listparty_t::value_type(pParty->GetBossIndex(), pParty));
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				PartyInviteMsg(rmsg, cPartyType1, cPartyType2, cPartyType3, nBossIndex, strBossName);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
				if (pTargetPC)		SEND_Q(rmsg, pTargetPC->m_desc);
			}
		}
		break;

	case MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_ME:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PartyErrorMsg(rmsg, MSG_PARTY_ERROR_ALREADY_ME);
			if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		}
		break;

	case MSG_HELPER_PARTY_ERROR_INVITE_DIFF_TYPE:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PartyErrorMsg(rmsg, MSG_PARTY_ERROR_DIFF_TYPE);
			if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		}
		break;

	case MSG_HELPER_PARTY_ERROR_INVITE_LEVEL:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PartyErrorMsg(rmsg, MSG_PARTY_ERROR_INVALID_LEVEL);
			if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		}
		break;

	case MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_TARGET:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PartyErrorMsg(rmsg, MSG_PARTY_ERROR_ALREADY_TARGET);
			if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		}
		break;

	case MSG_HELPER_PARTY_ERROR_INVITE_FULL:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PartyErrorMsg(rmsg, MSG_PARTY_ERROR_FULL);
			if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		}
		break;

	case MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_REQUEST:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PartyErrorMsg(rmsg, MSG_PARTY_ERROR_ALREADY_REQUEST);
			if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		}
		break;
#if defined (LC_USA)
	case MSG_HELPER_PARTY_ERROR_INVITE_PVP:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PartyErrorMsg(rmsg, MSG_PARTY_ERROR_PVP);
			if (pBossPC) SEND_Q(rmsg, pBossPC->m_desc);
		}
		break;
#endif	// #if defined (LC_USA)
	}
}

void ProcHelperPartyAllowRep(CNetMsg::SP& msg)
{
	int			nBossIndex;
	int			nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);
	int			nErrorCode;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex
		>> strTargetName
		>> nErrorCode;

	GAMELOG << init("PARTY DEBUG HELPER ALLOW REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "strTargetName" << delim << strTargetName << delim
			<< "nErrorCode" << delim << nErrorCode
			<< end;

	CPC* pTargetPC = PCManager::instance()->getPlayerByCharIndex(nTargetIndex);

	// 파티 관련 서버 다운되는 현상 방지
	if(!pTargetPC)
	{
		GAMELOG << init("PARTY Error : NULL Check ALLOW REP 1") << end;
		return;
	}

	CParty* pParty = gserver->FindPartyByBossIndex(nBossIndex);
	if (pParty == NULL)
	{
		GAMELOG << init("PARTY DEBUG HELPER ALLOW REP : NOT FOUND PARTY") << end;
		return ;
	}

	int nListIndex = pParty->JoinRequest(strTargetName, pTargetPC);
	if (nListIndex == -1)
	{
		GAMELOG << init("PARTY DEBUG HELPER ALLOW REP : CANNOT JOIN") << end;
		return ;
	}

	// 파티매칭에도 현재 인원을 업데이트 해줍니다.
	CPartyMatchParty * pPartyMatch = gserver->FindPartyMatchPartyByBossIndex( pParty->GetBossIndex() );
	if( pPartyMatch )
	{
		pPartyMatch->SetMemberCount( pParty->GetMemberCount() );
	}

	{
		// 파티원들에게 결과 알리기
		CNetMsg::SP addmsg(new CNetMsg);
		PartyAddMsg(addmsg, nTargetIndex, strTargetName, pTargetPC, ((pParty->GetBossIndex() == nTargetIndex) ? 1 : 0));
		for (int i = 0; i < MAX_PARTY_MEMBER; i++)
		{
			const CPartyMember* pMember = pParty->GetMemberByListIndex(i);
			if (pMember && i != nListIndex)
			{
				// 타인에게 target을 추가하고
				if (pMember->GetMemberPCPtr())
					SEND_Q(addmsg, pMember->GetMemberPCPtr()->m_desc)

					// target에게는 타인을 추가
					if (pTargetPC)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						PartyAddMsg(rmsg, pMember->GetCharIndex(), pMember->GetCharName(), pMember->GetMemberPCPtr(), ((pParty->GetBossIndex() == pMember->GetCharIndex()) ? 1 : 0));
						SEND_Q(rmsg, pTargetPC->m_desc);
					}
			}
		}
	}
#ifdef GMTOOL
	CPC* boss = PCManager::instance()->getPlayerByCharIndex(nBossIndex);
	if((pTargetPC && pTargetPC->m_bGmMonitor) || (boss && boss->m_bGmMonitor))
	{
		pParty->m_bGmtarget = true;
	}
#endif // GMTOOL
}

void ProcHelperPartyRejectRep(CNetMsg::SP& msg)
{
	int			nBossIndex;
	int			nTargetIndex;
	int			nErrorCode;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex
		>> nErrorCode;

	GAMELOG << init("PARTY DEBUG HELPER REJECT REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "nErrorCode" << delim << nErrorCode
			<< end;

	CParty* pParty = gserver->FindPartyByBossIndex(nBossIndex);
	if (!pParty)
	{
		GAMELOG << init("PARTY DEBUG HELPER REJECT REP : NOT FOUND PARTY") << end;
		return ;
	}
	if (pParty->GetRequestIndex() < 1)
	{
		GAMELOG << init("PARTY DEBUG HELPER REJECT REP : NOT FOUND REQUEST") << end;
		return ;
	}

	CPC* pBossPC = pParty->GetMemberByListIndex(0)->GetMemberPCPtr();
	CPC* pRequestPC = PCManager::instance()->getPlayerByCharIndex(pParty->GetRequestIndex());

	if (nErrorCode == MSG_HELPER_PARTY_ERROR_REJECT_OK)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (pParty->GetRequestIndex() == nTargetIndex)
				PartyMsg(rmsg, MSG_PARTY_REJECT_DEST);
			else if (pParty->GetBossIndex() == nTargetIndex)
				PartyMsg(rmsg, MSG_PARTY_REJECT_SRC);
			else
			{
				GAMELOG << init("PARTY DEBUG HELPER REJECT REP : NO MATCH CHAR") << end;
				return ;
			}

			pParty->SetMemberPCPtr(pParty->GetRequestIndex(), NULL);

			if (pBossPC)
			{
				SEND_Q(rmsg, pBossPC->m_desc);
			}

			if (pRequestPC)
			{
				SEND_Q(rmsg, pRequestPC->m_desc);
				pRequestPC->m_party = NULL;
			}
		}

		pParty->SetRequest(-1, "");
		if (pParty->GetMemberCount() < 2)
		{
			// 파티 종료
#ifdef EXTREME_CUBE
			if(pParty->m_cubeUniqueIdx != -1)
			{
				CCubeMemList* memlist = gserver->m_extremeCube.FindMemList(pParty);
				if(memlist && memlist->IsPartyCubeMemList())
				{
					((CPartyCubeMemList*)memlist)->SetParty(NULL);
				}
			}
#endif // EXTREME_CUBE

			// 파티 지우기
			gserver->m_listParty.erase(pParty->GetBossIndex());
			pParty->SetEndParty();
			delete pParty;
		}
	}
}

void ProcHelperPartyQuitRep(CNetMsg::SP& msg)
{
	int			nBossIndex;
	int			nTargetIndex;
	int			nErrorCode;
	int			nNewBossIndex;
	CLCString	strNewBossName(MAX_CHAR_NAME_LENGTH + 1);
	CLCString	strOldBossName(MAX_CHAR_NAME_LENGTH + 1);

	bool		bEndMonsterCombo = false;
	RefMsg(msg) >> nBossIndex
		>> nTargetIndex
		>> nErrorCode
		>> nNewBossIndex
		>> strNewBossName;

	GAMELOG << init("PARTY DEBUG HELPER QUIT REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "nNewBossIndex" << delim << nNewBossIndex << delim
			<< "strNewBossName" << delim << strNewBossName
			<< end;

	CParty* pParty = gserver->FindPartyByBossIndex(nBossIndex);
	if (!pParty)
	{
		GAMELOG << init("PARTY DEBUG HELPER QUIT REP : NOT FOUND PARTY") << end;
		return ;
	}

	strOldBossName = pParty->GetMemberByListIndex(0)->GetCharName();

	CPC* pTargetPC = PCManager::instance()->getPlayerByCharIndex(nTargetIndex);
	if (pTargetPC)
	{
		// 콤보 area에 있는 pc 파티 탈퇴하면 콤보존 나감
		if(pTargetPC->m_pZone->IsComboZone())
		{
			CZone* pZone = gserver->FindZone(ZONE_DRATAN);
			if (pZone == NULL)
				return;

			GAMELOG << init("GET OUT COMBO AREA BY PARTY QUIT", pTargetPC)
				<< "AREA INDEX: "
				<< pTargetPC->m_pArea->m_index
				<< "COMBO INDEX: "
				<< pTargetPC->m_pArea->m_monsterCombo->m_nIndex
				<< end;

			int extra = 0;
			GoZoneForce(pTargetPC, pZone->m_index,
				pZone->m_zonePos[extra][0],														// ylayer
				GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
				GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z

			bEndMonsterCombo = true;
		}

#ifdef EXTREME_CUBE
		// 큐브존에 있는 PC 파티 탈퇴하면 큐브존 나감
		if(pTargetPC->m_pZone)
		{
			if(pTargetPC->m_pZone->IsExtremeCube())
			{
				CCubeMemList* memlist;
				memlist = gserver->m_extremeCube.FindMemList(pParty);
				if(memlist)
				{
					CZone* pZone = gserver->FindZone(ZONE_MERAC);
					if (pZone == NULL)
						return;

					int extra = 0;
					GoZoneForce(pTargetPC, pZone->m_index,
						pZone->m_zonePos[extra][0],														// ylayer
						GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
						GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z

					GAMELOG << init("GET OUT CUBE BY PARTY QUIT", pTargetPC)
							<< pTargetPC->m_pZone->m_index
							<< end;

					//	memlist->DelPC(pTargetPC);
				}
			}
		}
		else
		{
			GAMELOG << init("GET OUT CUBE BY PARTY QUIT", pTargetPC)
					<< "ZONE (NULL)"
					<< end;
		}
#endif // EXTREME_CUBE

		// 인존에 있는 PC 파티 탈퇴하면 인존을 나감(멤버)
		if(pTargetPC->m_pZone !=NULL && (pTargetPC->m_pZone->IsExpedRaidZone() || pTargetPC->m_pZone->IsPartyRaidZone()) &&
			pTargetPC->m_nJoinInzone_ZoneNo >=0 && pTargetPC->m_nJoinInzone_RoomNo >= 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidInzoneQuitReq(rmsg,1,0);
			do_Raid(pTargetPC, rmsg);
		}

		if( pTargetPC->m_party && pTargetPC->m_party != pParty)
		{
			// 가입되있는 파티와 탈퇴하는 파티가 다른 경우는
			// 파티 초기화 제외
		}
		else
		{
			pTargetPC->CancelDamageLink();
			pTargetPC->m_party = NULL;
		}
	}

	pParty->SetMemberPCPtr(nTargetIndex, NULL);
	pParty->DeleteMember(nTargetIndex);

#ifdef GMTOOL
	if( pTargetPC )
	{
		if(pTargetPC->m_bGmMonitor)
		{
			pParty->m_bGmtarget = false;
		}
	}
#endif // GMTOOL

	if (nErrorCode == MSG_HELPER_PARTY_ERROR_QUIT_END || pParty->GetBossIndex() != nNewBossIndex)
	{
		if (pTargetPC)
		{
			// 파티 해체 알림
			CNetMsg::SP rmsg(new CNetMsg);
			PartyMsg(rmsg, MSG_PARTY_END);
			SEND_Q(rmsg, pTargetPC->m_desc);
		}

		if(pParty->m_comboAreaIndex != -1 && bEndMonsterCombo)
		{
			int i;
			CArea* area;
			i = gserver->m_comboZone->FindComboArea(pParty->m_comboAreaIndex);
			if(i != -1)
			{
				area = gserver->m_comboZone->m_area + i;

				GAMELOG << init("CLOSE COMBO AREA")
						<< "BY Party End"
						<< "AREA INDEX: " << area->m_index
						<< "COMBO INDEX: " << area->m_monsterCombo->m_nIndex
						<< end;

				area->CloseComboZone();
			}
		}

#ifdef EXTREME_CUBE
		if(pParty->m_cubeUniqueIdx != -1)
		{
			CCubeMemList* memlist = gserver->m_extremeCube.FindMemList(pParty);
			if(memlist && memlist->IsPartyCubeMemList())
			{
				((CPartyCubeMemList*)memlist)->SetParty(NULL);
			}
		}
#endif // EXTREME_CUBE

		// 파티 지우기
		gserver->m_listParty.erase(pParty->GetBossIndex());
		pParty->SetEndParty();
		delete pParty;
	}
	else
	{
		{
			// 파티원 나감
			CNetMsg::SP rmsg(new CNetMsg);
			PartyDelMsg(rmsg, false, nTargetIndex);

			if (pTargetPC)
				SEND_Q(rmsg, pTargetPC->m_desc);

			pParty->SendToAllPC(rmsg, nTargetIndex);
		}

		// 파티매칭에도 현재 인원을 업데이트 해줍니다.
		CPartyMatchParty * pMatchParty = gserver->FindPartyMatchPartyByBossIndex( nBossIndex );

		// 보스 변경 알림
		if (nBossIndex != nNewBossIndex)
		{
			if (pMatchParty)
			{
				pMatchParty->SetBossIndex(pParty->GetBossIndex());
				pMatchParty->SetBossName(pParty->GetMemberByListIndex(0)->GetCharName());
				gserver->m_listPartyMatchParty.erase(nBossIndex);
				gserver->m_listPartyMatchParty.insert(map_listPartyMatchParty_t::value_type(pParty->GetBossIndex(), pMatchParty));
			}
			{
				CNetMsg::SP rmsg(new CNetMsg);
				PartyChangeBoss(rmsg, strOldBossName, nNewBossIndex, strNewBossName, false);
				pParty->SendToAllPC(rmsg);
			}
		}

		// 파티매칭에도 현재 인원을 업데이트 해줍니다.
		if( pMatchParty )
		{
			pMatchParty->SetMemberCount( pParty->GetMemberCount() );
		}
	}
}

void ProcHelperPartyKickRep(CNetMsg::SP& msg)
{
	int			nBossIndex;
	int			nTargetIndex;
	int			nErrorCode;

	bool		bEndMonsterCombo = false;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex
		>> nErrorCode;

	GAMELOG << init("PARTY DEBUG HELPER KICK REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "nErrorCode" << delim << nErrorCode
			<< end;

	CParty* pParty = gserver->FindPartyByBossIndex(nBossIndex);
	if (!pParty)
	{
		GAMELOG << init("PARTY DEBUG HELPER KICK REP : NOT FOUND PARTY") << end;
		return ;
	}

	CPC* pTargetPC =  PCManager::instance()->getPlayerByCharIndex(nTargetIndex);
	if (pTargetPC && pTargetPC->m_party && pTargetPC->m_party == pParty)
	{
		// 콤보 area에 있는 pc 파티 탈퇴하면 콤보존 나감
		if(pTargetPC->m_pZone->IsComboZone())
		{
			CZone* pZone = gserver->FindZone(ZONE_DRATAN);
			if (pZone == NULL)
				return;

			GAMELOG << init("GET OUT COMBO AREA BY PARTY KICK", pTargetPC)
				<< "AREA INDEX: "
				<< pTargetPC->m_pArea->m_index
				<< "COMBO INDEX: "
				<< pTargetPC->m_pArea->m_monsterCombo->m_nIndex
				<< end;

			int extra = 0;
			GoZoneForce(pTargetPC, pZone->m_index,
				pZone->m_zonePos[extra][0],														// ylayer
				GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
				GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z

			bEndMonsterCombo = true;
		}

#ifdef EXTREME_CUBE
		// 큐브존에 있는 PC 파티 탈퇴하면 큐브존 나감
		if(pTargetPC->m_pZone)
		{
			if(pTargetPC->m_pZone->IsExtremeCube())
			{
				CCubeMemList* memlist;
				memlist = gserver->m_extremeCube.FindMemList(pParty);
				if(memlist)
				{
					CZone* pZone = gserver->FindZone(ZONE_MERAC);
					if (pZone == NULL)
						return;

					int extra = 0;
					GoZoneForce(pTargetPC, pZone->m_index,
						pZone->m_zonePos[extra][0],														// ylayer
						GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
						GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z

					GAMELOG << init("GET OUT CUBE BY PARTY KICK", pTargetPC)
							<< pTargetPC->m_pZone->m_index
							<< end;

					//	memlist->DelPC(pTargetPC);
				}
			}
		}
		else
		{
			GAMELOG << init("GET OUT CUBE BY PARTY KICK", pTargetPC)
					<< "ZONE (NULL)"
					<< end;
		}
#endif // EXTREME_CUBE

		// 인존에 있는 PC 파티 탈퇴하면 인존을 나감(멤버)
		if(pTargetPC->m_pZone !=NULL && (pTargetPC->m_pZone->IsExpedRaidZone() || pTargetPC->m_pZone->IsPartyRaidZone()) &&
			pTargetPC->m_nJoinInzone_ZoneNo >=0 && pTargetPC->m_nJoinInzone_RoomNo >= 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidInzoneQuitReq(rmsg,1,0);
			do_Raid(pTargetPC, rmsg);
		}

		pTargetPC->CancelDamageLink();
		pTargetPC->m_party = NULL;
	}

	pParty->SetMemberPCPtr(nTargetIndex, NULL);
	pParty->DeleteMember(nTargetIndex);

	if (nErrorCode == MSG_HELPER_PARTY_ERROR_KICK_END)
	{
		{
			// 파티 해체 알림
			CNetMsg::SP rmsg(new CNetMsg);
			PartyMsg(rmsg, MSG_PARTY_END);

			if (pTargetPC)
				SEND_Q(rmsg, pTargetPC->m_desc);

			CPC* pBossPC = PCManager::instance()->getPlayerByCharIndex(nBossIndex);
			if(pBossPC)
				SEND_Q(rmsg, pBossPC->m_desc);
		}

		if(pParty->m_comboAreaIndex != -1 && bEndMonsterCombo)
		{
			int i;
			CArea* area;
			i = gserver->m_comboZone->FindComboArea(pParty->m_comboAreaIndex);
			if(i != -1)
			{
				area = gserver->m_comboZone->m_area + i;

				GAMELOG << init("CLOSE COMBO AREA")
						<< "BY Party End"
						<< "AREA INDEX: " << area->m_index
						<< "COMBO INDEX: " << area->m_monsterCombo->m_nIndex
						<< end;

				area->CloseComboZone();
			}
		}

#ifdef EXTREME_CUBE
		if(pParty->m_cubeUniqueIdx != -1)
		{
			CCubeMemList* memlist = gserver->m_extremeCube.FindMemList(pParty);
			if(memlist && memlist->IsPartyCubeMemList())
			{
				((CPartyCubeMemList*)memlist)->SetParty(NULL);
			}
		}
#endif // EXTREME_CUBE

		// 파티 지우기
		gserver->m_listParty.erase(pParty->GetBossIndex());
		pParty->SetEndParty();
		delete pParty;
	}
	else
	{
		{
			// 파티원 나감
			CNetMsg::SP rmsg(new CNetMsg);
			PartyDelMsg(rmsg, false, nTargetIndex);

			if (pTargetPC)
				SEND_Q(rmsg, pTargetPC->m_desc);

			pParty->SendToAllPC(rmsg, nTargetIndex);
		}

		// 파티매칭에도 현재 인원을 업데이트 해줍니다.
		CPartyMatchParty * pPartyMatch = gserver->FindPartyMatchPartyByBossIndex( pParty->GetBossIndex() );
		if( pPartyMatch )
		{
			pPartyMatch->SetMemberCount( pParty->GetMemberCount() );
		}
	}
}

void ProcHelperPartyChangeBossRep(CNetMsg::SP& msg)
{
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nNewBossIndex;
	CLCString	strNewBossName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> nBossIndex
		>> strBossName
		>> nNewBossIndex
		>> strNewBossName;

	GAMELOG << init("PARTY DEBUG HELPER CHANGE BOSS REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "strBossName" << delim << strBossName << delim
			<< "nNewBossIndex" << delim << nNewBossIndex << delim
			<< "strNewBossName" << delim << strNewBossName
			<< end;

	CParty* pParty = gserver->FindPartyByBossIndex(nBossIndex);
	if (!pParty)
	{
		GAMELOG << init("PARTY DEBUG HELPER CHANGE BOSS REP : NOT FOUND PARTY") << end;
		return ;
	}

	if(!pParty->ChangeBoss(strNewBossName))
	{
		GAMELOG << init("PARTY DEBUG HELPER CHANGE BOSS REP : FAILED")
				<< "nBossIndex" << delim << nBossIndex << delim
				<< "strBossName" << delim << strBossName << delim
				<< "nNewBossIndex" << delim << nNewBossIndex << delim
				<< "strNewBossName" << delim << strNewBossName
				<< end;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyBreakReqMsg(rmsg, nNewBossIndex);
			SEND_Q(rmsg, gserver->m_helper);
		}

		gserver->m_listParty.erase(pParty->GetBossIndex());
		pParty->SetEndParty();
		delete pParty;
		return ;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		PartyChangeBoss(rmsg, strBossName, nNewBossIndex, strNewBossName, true);
		pParty->SendToAllPC(rmsg);
	}

	CPartyMatchParty* pMatchParty = gserver->FindPartyMatchPartyByBossIndex(nBossIndex);
	if (pMatchParty)
	{
		pMatchParty->SetBossIndex(nNewBossIndex);
		pMatchParty->SetBossName(strNewBossName);
		gserver->m_listPartyMatchParty.erase(nBossIndex);
		gserver->m_listPartyMatchParty.insert(map_listPartyMatchParty_t::value_type(nNewBossIndex, pMatchParty));

			CPC* pBossPC = PCManager::instance()->getPlayerByCharIndex(nNewBossIndex);
			if (pBossPC && pBossPC->m_pZone)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperPartyMatchMemberChangeInfoMsg(rmsg, nNewBossIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_ZONE, "", pBossPC->m_level, pBossPC->m_pZone->m_index);
				SEND_Q(rmsg, gserver->m_helper);
			}
	}

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(nBossIndex);
	if(pc)
		pc->m_listSaveComboData.clear();
}

void ProcHelperPartyChangeTypeRep(CNetMsg::SP& msg)
{
	int nBossIndex;
	char cPartyType;
	char cDiviType, cAllOneSet;

	RefMsg(msg) >> nBossIndex
		>> cPartyType
		>> cDiviType
		>> cAllOneSet;

	GAMELOG << init("PARTY DEBUG HELPER CHANGE TYPE REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "PartyType" << delim << cPartyType
			<< "cDiviType" << delim << cDiviType
			<< "cAllOneSet" << delim << cAllOneSet
			<< end;

	CParty* pParty = gserver->FindPartyByBossIndex(nBossIndex);
	if (!pParty)
	{
		GAMELOG << init("PARTY DEBUG HELPER CHANGE TYPE REP : NOT FOUND PARTY") << end;
		return ;
	}

	if(cAllOneSet==MSG_PARTY_SETALLONE_ALL)	//대표 타입 설정
	{
		pParty->SetPartyTypeAll(cPartyType);
	}else if(cAllOneSet==MSG_PARTY_SETALLONE_ONE)	 //세분화 1개 설정
	{
		pParty->SetPartyType(cDiviType,cPartyType);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		PartyChangeType(rmsg, cPartyType, cDiviType, cAllOneSet);
		pParty->SendToAllPC(rmsg);
	}
}

//파티 해체
void ProcHelperPartyEndPartyRep(CNetMsg::SP& msg)
{
	int nBossIndex;

	RefMsg(msg) >> nBossIndex;

	CParty* pParty = gserver->FindPartyByBossIndex(nBossIndex);
	if (!pParty)
		return ;

	// 파티 해체(리스트 삭제)
	gserver->m_listParty.erase(pParty->GetBossIndex());
	pParty->SetEndParty();
	delete pParty;
}

void ProcHelperPartyMatchRegMemberRep(CNetMsg::SP& msg)
{
	int				nErrorCode;
	int				nCharIndex;
	CLCString		strCharName(MAX_CHAR_NAME_LENGTH + 1);
	int				nCharLevel;
	int				nZone;
	char			nCharJob;
	char			nPartyType;
	CPC*			pPC;

	RefMsg(msg) >> nErrorCode
		>> nCharIndex
		>> strCharName
		>> nCharLevel
		>> nZone
		>> nCharJob
		>> nPartyType;

	GAMELOG << init("PARTY DEBUG HELPER MATCH REG MEMBER REP")
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "nCharIndex" << delim << nCharIndex << delim
			<< "strCharName" << delim << strCharName << delim
			<< "nCharLevel" << delim << nCharLevel << delim
			<< "nZone" << delim << nZone << delim
			<< "nCharJob" << delim << nCharJob << delim
			<< "nPartyType" << delim << nPartyType
			<< end;

	pPC = PCManager::instance()->getPlayerByCharIndex(nCharIndex);

	switch (nErrorCode)
	{
	case MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_OK:
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_PARTY:
		if (pPC)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExPartyMatchRegMemberRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_PARTY);
			SEND_Q(rmsg, pPC->m_desc);
#ifdef _DEBUG
{
			CLCString chat(1000);
			chat = "REG MEMBER REPLY: ALREADY PARTY";
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			SEND_Q(rmsg, pPC->m_desc);
}
#endif // _DEBUG
		}
		return ;

	case MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_REG:
		if (pPC)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExPartyMatchRegMemberRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_REG);
			SEND_Q(rmsg, pPC->m_desc);
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "REG MEMBER REPLY: ALREADY REGIST";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				SEND_Q(rmsg, pPC->m_desc);
			}
#endif // _DEBUG
		}
		return ;

	default:
		return ;
	}

	CPartyMatchMember* pMatchMember = new CPartyMatchMember(nCharIndex, strCharName, nCharLevel, nZone, nCharJob, nPartyType);
	gserver->m_listPartyMatchMember.insert(map_listPartyMatchMember_t::value_type(pMatchMember->GetIndex(), pMatchMember));
	if (pPC)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPartyMatchRegMemberRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_MEMBER_OK);
		SEND_Q(rmsg, pPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "REG MEMBER REPLY: OK";
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			SEND_Q(rmsg, pPC->m_desc);
		}
#endif // _DEBUG
	}
}

void ProcHelperPartyMatchRegPartyRep(CNetMsg::SP& msg)
{
	int				nErrorCode;
	int				nBossIndex;
	CLCString		strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int				nBossLevel;
	int				nZone;
	int				nJobFlag;
	char			cLimitLevel;
	char			cPartyType;
	int				nMemberCount;
	CLCString		strComment(PARTY_MATCHING_COMMENT + 1);

	RefMsg(msg) >> nErrorCode
		>> nBossIndex
		>> strBossName
		>> nBossLevel
		>> nZone
		>> nMemberCount
		>> nJobFlag
		>> cLimitLevel
		>> cPartyType
		>> strComment;

	GAMELOG << init("PARTY DEBUG HELPER MATCH REG PARTY REP")
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "strBossName" << delim << strBossName << delim
			<< "nBossLevel" << delim << nBossLevel << delim
			<< "nZone" << delim << nZone << delim
			<< "nJobFlag" << delim << nJobFlag << delim
			<< "cLimitLevel" << delim << cLimitLevel << delim
			<< "cPartyType" << delim << cPartyType
			<< end;

	CPC* pBossPC = PCManager::instance()->getPlayerByCharIndex(nBossIndex);

	switch (nErrorCode)
	{
	case MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_OK:
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_NO_BOSS:
		if (pBossPC)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExPartyMatchRegPartyRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_NO_BOSS);
			SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "REG PARTY REPLY: NO BOSS";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				SEND_Q(rmsg, pBossPC->m_desc);
			}
#endif // _DEBUG
		}
		return ;

	case MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_ALREADY_REG:
		if (pBossPC)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExPartyMatchRegPartyRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_ALREADY_REG);
			SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "REG PARTY REPLY: ALREADY REGIST";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				SEND_Q(rmsg, pBossPC->m_desc);
			}
#endif // _DEBUG
		}
		return ;

	case MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_FULL:
		if (pBossPC)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExPartyMatchRegPartyRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_FULL);
			SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "REG PARTY REPLY: FULL PARTY";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				SEND_Q(rmsg, pBossPC->m_desc);
			}
#endif // _DEBUG
		}
		return ;

	default:
		return ;
	}

	CPartyMatchParty* pMatchParty = new CPartyMatchParty(nBossIndex, strBossName, nBossLevel, nZone, nJobFlag, cPartyType, (cLimitLevel) ? true : false, strComment);
	pMatchParty->SetMemberCount(nMemberCount);
	gserver->m_listPartyMatchParty.insert(map_listPartyMatchParty_t::value_type(pMatchParty->GetBossIndex(), pMatchParty));
	if (pBossPC)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPartyMatchRegPartyRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_OK);
		SEND_Q(rmsg, pBossPC->m_desc);
#ifdef _DEBUG
		{
			CLCString chat(1000);
			chat = "REG PARTY REPLY: OK";
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			SEND_Q(rmsg, pBossPC->m_desc);
		}
#endif // _DEBUG
	}
}

void ProcHelperPartyMatchDelRep(CNetMsg::SP& msg)
{
	int		nCharIndex;
	int		nErrorCode;

	RefMsg(msg) >> nCharIndex
		>> nErrorCode;

	GAMELOG << init("PARTY DEBUG HELPER MATCH DEL REP")
			<< "nCharIndex" << delim << nCharIndex << delim
			<< "nErrorCode" << delim << nErrorCode
			<< end;

	CPartyMatchMember*	pMatchMember = NULL;
	CPartyMatchParty*	pMatchParty = NULL;

	CPC* pPC = PCManager::instance()->getPlayerByCharIndex(nCharIndex);

	switch (nErrorCode)
	{
	case MSG_HELPER_PARTY_MATCH_ERROR_DEL_MEMBER:
		pMatchMember = gserver->FindPartyMatchMemberByCharIndex(nCharIndex);
		if (pMatchMember)
		{
			gserver->m_listPartyMatchMember.erase(pMatchMember->GetIndex());
			delete pMatchMember;
			if (pPC)
			{
				if (!pPC->m_party)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPartyMatchDelRepMsg(rmsg);
					SEND_Q(rmsg, pPC->m_desc);
				}
#ifdef _DEBUG
				{
					CLCString chat(1000);
					chat = "DEL REPLY: DEL MEMBER";
					CNetMsg::SP rmsg(new CNetMsg);
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
					SEND_Q(rmsg, pPC->m_desc);
				}
#endif // _DEBUG
			}
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_DEL_PARTY:
		pMatchParty = gserver->FindPartyMatchPartyByBossIndex(nCharIndex);
		if (pMatchParty)
		{
			gserver->m_listPartyMatchParty.erase(pMatchParty->GetBossIndex());
			delete pMatchParty;
			if (pPC)
			{
				if (pPC->IsParty() && pPC->m_party->GetMemberCount() < MAX_PARTY_MEMBER)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPartyMatchDelRepMsg(rmsg);
					SEND_Q(rmsg, pPC->m_desc);
				}
#ifdef _DEBUG
				{
					CLCString chat(1000);
					chat = "DEL REPLY: DEL PARTY";
					CNetMsg::SP rmsg(new CNetMsg);
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
					SEND_Q(rmsg, pPC->m_desc);
				}
#endif // _DEBUG
			}
		}
		break;
	}
}

void ProcHelperPartyMatchInviteRep(CNetMsg::SP& msg)
{
	int			nErrorCode;
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nCharIndex;
	CLCString	strCharName(MAX_CHAR_NAME_LENGTH + 1);
	char		cPartyType;

	RefMsg(msg) >> nErrorCode
		>> nBossIndex
		>> strBossName
		>> nCharIndex
		>> strCharName
		>> cPartyType;

	GAMELOG << init("PARTY DEBUG HELPER MATCH INVITE REP")
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "strBossName" << delim << strBossName << delim
			<< "nCharIndex" << delim << nCharIndex << delim
			<< "strCharName" << delim << strCharName << delim
			<< "cPartyType" << delim << cPartyType
			<< end;

	CPC* pBossPC = PCManager::instance()->getPlayerByCharIndex(nBossIndex);
	CPC* pTargetPC = PCManager::instance()->getPlayerByCharIndex(nCharIndex);
	CParty* pParty = NULL;

	// 파티 관련 서버 다운되는 현상 방지
	if(!pBossPC || !pTargetPC)
	{
		GAMELOG << init("PARTY Error : NULL Check 2") << end;
		//return;
	}
/*
	// 워프 중이면 취소
	if (pBossPC->CanWarp() || pTargetPC->CanWarp())
	{
		GAMELOG << init("PARTY Error : NULL Check 4") << end;
		return;
	}
*/

	switch (nErrorCode)
	{
	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_OK:
		{
			pParty = gserver->FindPartyByBossIndex(nBossIndex);
			if (pParty)
			{
				// 기존 파티에 초대 상태로
				pParty->SetRequest(nCharIndex, strCharName);
				if (pTargetPC)
					pTargetPC->m_party = pParty;
			}
			else
			{
				// 신규 파티
				pParty = new CParty(cPartyType, nBossIndex, strBossName, pBossPC, nCharIndex, strCharName);

				if (pBossPC)		pBossPC->m_party = pParty;
				if (pTargetPC)		pTargetPC->m_party = pParty;

				gserver->m_listParty.insert(map_listparty_t::value_type(pParty->GetBossIndex(), pParty));
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_OK, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
				if (pTargetPC)		SEND_Q(rmsg, pTargetPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat.Format("INVITE REP: TYPE: %d, BOSS: %d, TARGET: %d", cPartyType, nBossIndex, nCharIndex);
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
				if (pTargetPC)		SEND_Q(rmsg, pTargetPC->m_desc);
			}
#endif // _DEBUG
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_NOT_FOUND:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_NOT_FOUND, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "INVITE REP: NOT FOUND TARGET";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#endif // _DEBUG
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_LEVEL:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_LEVEL, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "INVITE REP: LEVEL";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#endif // _DEBUG
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_NOBOSS:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_NOBOSS, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "INVITE REP: NO BOSS";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#endif // _DEBUG
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_DIFF_TYPE:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_DIFF_TYPE, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "INVITE REP: DIFF TYPE";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#endif // _DEBUG
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_ALREADY_TARGET:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_ALREADY_TARGET, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "INVITE REP: ALREADY TARGET";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#endif // _DEBUG
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_FULL:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_FULL, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "INVITE REP: FULL";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#endif // _DEBUG
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_INVITE_ALREADY_REQUEST:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchInviteRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_INVITE_ALREADY_REQUEST, cPartyType, nBossIndex, strBossName, nCharIndex, strCharName);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "INVITE REP: ALREADY REQUEST";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
			}
#endif // _DEBUG
		}
		break;
	}
}

void ProcHelperPartyMatchJoinRep(CNetMsg::SP& msg)
{
	int			nErrorCode;
	char		cPartyType;
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nReqCharIndex;
	CLCString	strReqCharName(MAX_CHAR_NAME_LENGTH + 1);
	char		cReqCharJob;

	RefMsg(msg) >> nErrorCode
		>> cPartyType
		>> nBossIndex
		>> strBossName
		>> nReqCharIndex
		>> strReqCharName
		>> cReqCharJob;

	CPC* pBossPC = PCManager::instance()->getPlayerByCharIndex(nBossIndex);
	CPC* pReqPC = PCManager::instance()->getPlayerByCharIndex(nReqCharIndex);
	CParty* pParty = NULL;
///////////////파티오토매칭 시 전투파티로 파티타입 전환///////////////
	if(strReqCharName == "" || cReqCharJob == -1)
	{
		if(pReqPC)
		{
			strReqCharName = pReqPC->m_nick;
			cReqCharJob = pReqPC->m_job;
		}
		else // 요청 플레이어를 못찾을 경우 (게임에서 나갔을때나...)이름과 직업의 로그를 찍지 못하기에 임시로 string을 넣음.
		{
			strReqCharName = "Reference The Charindex(CANT FIND PC)";
			cReqCharJob = -1;
		}
	}
///////////////파티오토매칭 시 전투파티로 파티타입 전환///////////////
	GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN REP")
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "cPartyType" << delim << cPartyType << delim
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "strBossName" << delim << strBossName << delim
			<< "nReqCharIndex" << delim << nReqCharIndex << delim
			<< "strReqCharName" << delim << strReqCharName << delim
			<< "cReqCharJob" << delim << cReqCharJob
			<< end;

	// 파티 관련 서버 다운되는 현상 방지
	if(!pBossPC || !pReqPC)
	{
		GAMELOG << init("PARTY Error : NULL Check MATCH JOIN REP 1") << end;
		//return;
	}
/*
	// 워프 중이면 취소
	if (pBossPC->CanWarp() || pReqPC->CanWarp())
	{
		GAMELOG << init("PARTY Error : NULL Check MATCH JOIN REP 2") << end;
		return;
	}
*/

	switch (nErrorCode)
	{
	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_OK:
		{
			pParty = gserver->FindPartyByBossIndex(nBossIndex);
			if (pParty)
			{
				// 기존 파티의 초대 상태로
				pParty->SetRequest(nReqCharIndex, strReqCharName);
				if (pReqPC)
					pReqPC->m_party = pParty;
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_OK, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
					if (pBossPC)	SEND_Q(rmsg, pBossPC->m_desc);
					if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
				}
#ifdef _DEBUG
				{
					CLCString chat(1000);
					chat.Format("JOIN REPLY: TYPE: %d, BOSS: %d, REQ: %d, JOB: %d", cPartyType, nBossIndex, nReqCharIndex, cReqCharJob);
					CNetMsg::SP rmsg(new CNetMsg);
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
					if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
					if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
				}
#endif // _DEBUG
			}
			else
			{
				GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN REP : NOT FOUND PARTY") << end;
			}
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALREADY_TARGET:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_ALREADY_TARGET, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
				if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "JOIN REPLY: ALREADY TARGET";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
			}
#endif // _DEBUG
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_NOT_FOUND:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_NOT_FOUND, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
				if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "JOIN REPLY: NOT FOUND";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
			}
#endif // _DEBUG
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_LEVEL:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_LEVEL, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
				if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
				if (pBossPC)	SEND_Q(rmsg, pBossPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "JOIN REPLY: LEVEL";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
			}
#endif // _DEBUG
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_LIMIT_LEVEL:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_LIMIT_LEVEL, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
				if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "JOIN REPLY: LIMIT LEVEL";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
			}
#endif // _DEBUG
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_JOB:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_JOB, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
				if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "JOIN REPLY: JOB";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
			}
#endif // _DEBUG
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_FULL:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_FULL, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
				if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "JOIN REPLY: FULL";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
			}
#endif // _DEBUG
		}
		break;

	case MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALREADY_REQUEST:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyMatchJoinRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_JOIN_ALREADY_REQUEST, cPartyType, nBossIndex, strBossName, nReqCharIndex, strReqCharName, cReqCharJob);
				if (pReqPC)		SEND_Q(rmsg, pReqPC->m_desc);
			}
#ifdef _DEBUG
			{
				CLCString chat(1000);
				chat = "JOIN REPLY: ALREADY REQUEST";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
				if (pReqPC)			SEND_Q(rmsg, pReqPC->m_desc);
			}
#endif // _DEBUG
		}
		break;
	}
}

void ProcHelperPartyMatchJoinAllowRep(CNetMsg::SP& msg)
{
	int			nErrorCode;
	int			nBossIndex;
	int			nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> nErrorCode
		>> nBossIndex
		>> nTargetIndex
		>> strTargetName;

	GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN ALLOW REP")
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "strTargetName" << delim << strTargetName
			<< end;

	CPC* pTargetPC = PCManager::instance()->getPlayerByCharIndex(nTargetIndex);

	// 파티 관련 서버 다운되는 현상 방지
	if(!pTargetPC)
	{
		GAMELOG << init("PARTY Error : NULL Check MATCH JOIN ALLOW REP 1") << end;
		//return;
	}
/*
	// 워프 중이면 취소
	if (pTargetPC->CanWarp())
	{
		GAMELOG << init("PARTY Error : NULL Check MATCH JOIN ALLOW REP 2") << end;
		return;
	}
*/

	CParty* pParty = gserver->FindPartyByBossIndex(nBossIndex);
	if (pParty == NULL)
	{
		GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN ALLOW REP : NOT FOUND PARTY") << end;
		return ;
	}

	int nListIndex = pParty->JoinRequest(strTargetName, pTargetPC);
	if (nListIndex == -1)
	{
		GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN ALLOW REP : NOT FOUND REQUEST") << end;
		return ;
	}

	// 파티매칭에도 현재 인원을 업데이트 해줍니다.
	CPartyMatchParty * pPartyMatch = gserver->FindPartyMatchPartyByBossIndex( pParty->GetBossIndex() );
	if( pPartyMatch )
	{
		pPartyMatch->SetMemberCount( pParty->GetMemberCount() );
	}

	// 파티원들에게 결과 알리기
	{
		CNetMsg::SP addmsg(new CNetMsg);
		PartyAddMsg(addmsg, nTargetIndex, strTargetName, pTargetPC, ((pParty->GetBossIndex() == nTargetIndex) ? 1 : 0));
		for (int i = 0; i < MAX_PARTY_MEMBER; i++)
		{
			const CPartyMember* pMember = pParty->GetMemberByListIndex(i);
			if (pMember && i != nListIndex)
			{
				// 타인에게 target을 추가하고
				if (pMember->GetMemberPCPtr())
					SEND_Q(addmsg, pMember->GetMemberPCPtr()->m_desc)

					// target에게는 타인을 추가
					if (pTargetPC)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						PartyAddMsg(rmsg, pMember->GetCharIndex(), pMember->GetCharName(), pMember->GetMemberPCPtr(), ((pParty->GetBossIndex() == pMember->GetCharIndex()) ? 1 : 0));
						SEND_Q(rmsg, pTargetPC->m_desc);
					}
			}
		}
	}
}

void ProcHelperPartyMatchJoinRejectRep(CNetMsg::SP& msg)
{
	int			nErrorCode;
	int			nJoinCharIndex;
	int			nRejectCharIndex;

	RefMsg(msg) >> nErrorCode
		>> nJoinCharIndex
		>> nRejectCharIndex;

	GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN REJECT REP")
			<< "nErrorCode" << delim << nErrorCode << delim
			<< "nJoinCharIndex" << delim << nJoinCharIndex << delim
			<< "nRejectCharIndex" << delim << nRejectCharIndex
			<< end;

	// nJoinCharIndex로 파티 검색
	CParty* pParty = gserver->FindPartyByMemberIndex(nJoinCharIndex, true);
	if (!pParty)
	{
		GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN REJECT REP : NOT FOUND PARTY") << end;
		return ;
	}

	// 각각의 CPC 구하기
	CPC* pBossPC = pParty->GetMemberByListIndex(0)->GetMemberPCPtr();
	CPC* pRequestPC = PCManager::instance()->getPlayerByCharIndex(pParty->GetRequestIndex());

	if (nErrorCode == MSG_HELPER_PARTY_MATCH_ERROR_JOIN_REJECT_OK)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (pParty->GetRequestIndex() == nRejectCharIndex)
				PartyMsg(rmsg, MSG_PARTY_REJECT_DEST);
			else if (pParty->GetBossIndex() == nRejectCharIndex)
				PartyMsg(rmsg, MSG_PARTY_REJECT_SRC);
			else
			{
				GAMELOG << init("PARTY DEBUG HELPER MATCH JOIN REJECT REP : NOT FOUND CHAR") << end;
				return ;
			}

			pParty->SetMemberPCPtr(pParty->GetRequestIndex(), NULL);

			if (pBossPC)
				SEND_Q(rmsg, pBossPC->m_desc);
			if (pRequestPC)
			{
				SEND_Q(rmsg, pRequestPC->m_desc);
				pRequestPC->m_party = NULL;
			}
		}

		pParty->SetRequest(-1, "");
		if (pParty->GetMemberCount() < 2)
		{
			// 파티 종료
#ifdef EXTREME_CUBE
			if(pParty->m_cubeUniqueIdx != -1)
			{
				CCubeMemList* memlist = gserver->m_extremeCube.FindMemList(pParty);
				if(memlist && memlist->IsPartyCubeMemList())
				{
					((CPartyCubeMemList*)memlist)->SetParty(NULL);
				}
			}
#endif // EXTREME_CUBE

			// 파티 지우기
			gserver->m_listParty.erase(pParty->GetBossIndex());
			pParty->SetEndParty();
			delete pParty;
		}
	}
}

void ProcHelperPartyMatchMemberChangeInfo(CNetMsg::SP& msg)
{
	int			nCharIndex;
	int			nType;
	CLCString	strCharName(MAX_CHAR_NAME_LENGTH + 1);
	int			nLevel;
	int			nZone;

	RefMsg(msg) >> nCharIndex
		>> nType;

	CPartyMatchMember* pMatchMember = gserver->FindPartyMatchMemberByCharIndex(nCharIndex);
	CPartyMatchParty* pMatchParty = gserver->FindPartyMatchPartyByBossIndex(nCharIndex);

	switch (nType)
	{
	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_NAME:
		RefMsg(msg) >> strCharName;
		if (pMatchMember)		pMatchMember->SetName(strCharName);
		if (pMatchParty)		pMatchParty->SetBossName(strCharName);
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL:
		RefMsg(msg) >> nLevel;
		if (pMatchMember)		pMatchMember->SetLevel(nLevel);
		if (pMatchParty)		pMatchParty->SetBossLevel(nLevel);
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_ZONE:
		RefMsg(msg) >> nLevel
			>> nZone;
		if (pMatchMember)
		{
			pMatchMember->SetLevel(nLevel);
			pMatchMember->SetZone(nZone);
		}
		if (pMatchParty)
		{
			pMatchParty->SetBossLevel(nLevel);
			pMatchParty->SetZone(nZone);
		}
		break;
	}
}

void ProcHelperPartyInfoEnd(CNetMsg::SP& msg)
{
	// 헬퍼로부터 파티, 파티 매칭 정보를 모두 읽어 들이면 서버는 클라이언트 처리를 시작한다.
	gserver->m_bLoadPartyInfo = true;
}

void ProcHelperPartyInfoParty(CNetMsg::SP& msg)
{
	char		cPartyType;
	int			nRequestIndex;
	CLCString	strRequestName(MAX_CHAR_NAME_LENGTH + 1);
	int			nMemberCount;
	int			nBossIndex;
	int			nCharIndex;
	CLCString	strCharName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> cPartyType
		>> nRequestIndex
		>> strRequestName
		>> nMemberCount
		>> nBossIndex
		>> strCharName;

	CParty* pParty = new CParty(cPartyType, nBossIndex, strCharName, NULL, nRequestIndex, strRequestName);

	int i;
	for (i = 1; i < nMemberCount; i++)
	{
		RefMsg(msg) >> nCharIndex
			>> strCharName;

		pParty->m_listMember[i] = new CPartyMember(nCharIndex, strCharName, NULL);
		pParty->m_nCount++;
	}

	gserver->m_listParty.insert(map_listparty_t::value_type(pParty->GetBossIndex(), pParty));

	// 초대 중인 것이 있었으면 자동으로 거절 처리하도록 함
	if (nRequestIndex > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyRejectReqMsg(rmsg, nBossIndex, nRequestIndex);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void ProchelperPartyInfoPartyMatchMember(CNetMsg::SP& msg)
{
	int			nCharIndex;
	CLCString	strCharName(MAX_CHAR_NAME_LENGTH + 1);
	int			nLevel;
	int			nZone;
	char		cJob;
	char		cPartyType;

	RefMsg(msg) >> nCharIndex
		>> strCharName
		>> nLevel
		>> nZone
		>> cJob
		>> cPartyType;

	CPartyMatchMember* pMatchMember = new CPartyMatchMember(nCharIndex, strCharName, nLevel, nZone, cJob, cPartyType);
	gserver->m_listPartyMatchMember.insert(map_listPartyMatchMember_t::value_type(pMatchMember->GetIndex(), pMatchMember));
}

void ProchelperPartyInfoPartyMatchParty(CNetMsg::SP& msg)
{
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nBossLevel;
	int			nZone;
	int			nJobFlag;
	char		cPartyType;
	char		cLimitLevel;
	CLCString	strComment(PARTY_MATCHING_COMMENT + 1);

	RefMsg(msg) >> nBossIndex
		>> strBossName
		>> nBossLevel
		>> nZone
		>> nJobFlag
		>> cPartyType
		>> cLimitLevel
		>> strComment;

	CPartyMatchParty* pMatchParty = new CPartyMatchParty(nBossIndex, strBossName, nBossLevel, nZone, nJobFlag, cPartyType, (cLimitLevel) ? true : false, strComment);
	gserver->m_listPartyMatchParty.insert(map_listPartyMatchParty_t::value_type(pMatchParty->GetBossIndex(), pMatchParty));
}

void ProcHelperGuildInclineEstablish( CNetMsg::SP& msg )
{
	int		guildindex;
	int		charindex;
	char	guildincline;
	int		errorcode;

	RefMsg(msg) >> guildindex
		>> charindex
		>> guildincline
		>> errorcode;

	LONGLONG needMoney;
	int needGP;
	RefMsg(msg) >> needMoney
		>> needGP;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex( charindex );
	CGuild* guild = gserver->m_guildlist.findguild( guildindex );

	if( !pc || !guild )
	{
		return;
	}

	if( guild )
	{
		if( errorcode == MSG_NEW_GUILD_ERROR_INCLINE_OK )
		{
			guild->incline( guildincline );
			guild->AddGuildPoint( -needGP );

			if (pc->m_inventory.getMoney() >= needMoney)
			{
				pc->m_inventory.decreaseMoney(needMoney);
			}
			pc->CalcStatus(true);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildInclineEstablishMsg( rmsg, guildincline );
				guild->SendToAll( rmsg );
			}
		}
	}
}
void ProcHelperGuildMemberAdjust( CNetMsg::SP& msg )
{
	int guildindex;
	int ownerindexindex;
	int charindex;
	CLCString strPositionName(GUILD_POSITION_NAME+1);
	int contributeExp;
	int contributeFame;
	int errorcode;
	int pos;

	RefMsg(msg) >> guildindex
		>> ownerindexindex
		>> charindex
		>> strPositionName
		>> contributeExp
		>> contributeFame
		>> errorcode;
	RefMsg(msg) >> pos;

	CPC* guildOwner = PCManager::instance()->getPlayerByCharIndex( ownerindexindex );
	CGuildMember* guildMember = gserver->m_guildlist.findmember( charindex);

	CGuild* guild = gserver->m_guildlist.findguild( guildindex );

	if( !guild )
		return;

	if( errorcode == MSG_NEW_GUILD_ERROR_ADJUST_OK && guildMember )
	{
	//	guildMember->m_guildInfo->positionName( strPositionName );
	//	guildMember->m_guildInfo->contributeExp( contributeExp );
	//	guildMember->m_guildInfo->contributeFame( contributeFame );
		guildMember->positionName( strPositionName );
		guildMember->contributeExp( contributeExp );
		guildMember->contributeFame( contributeFame );
		guild->ChangeGradeExPosCount(guildMember->pos(), pos);
		guildMember->pos(pos);
		int needGP = guild->GetGradeExPosNeedGuilPoint( pos ) ;
		if( needGP >= guild->GetGuildPoint() )
			guild->guildPoint( 0 );
		else if( needGP > 0)
			guild->guildPoint(guild->GetGuildPoint() - needGP );
	}
	if( guildOwner )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )errorcode, guild );
		SEND_Q( rmsg, guildOwner->m_desc );
	}
}

void ProcHelperGuildInfoNotice( CNetMsg::SP& msg )
{
	int charindex;
	int guildindex;
	int avelevel;
	int usepoint;
	int errorcode;

	RefMsg(msg) >> charindex
		>> guildindex
		>> avelevel
		>> usepoint
		>> errorcode;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex( charindex );
	CGuild* guild = gserver->m_guildlist.findguild( guildindex );

	if( !pc || !guild )
		return;

	if( errorcode == MSG_NEW_GUILD_INFO_ERROR_OK )
	{
		if( guild )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildNewInfo( rmsg, pc, avelevel, guild->GetGuildPoint(), usepoint );
			SEND_Q( rmsg, pc->m_desc );
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )errorcode );
		SEND_Q( rmsg, pc->m_desc );
	}
}

void ProcHelperNewGuildMemberListRep( CNetMsg::SP& msg )
{
	int charindex;
	int guildindex;
	int errorcode;
	int membercount;
	int membercharindex[GUILD_MAX_MEMBER];
	int cumulatePoint[GUILD_MAX_MEMBER];
	char CharName[GUILD_MAX_MEMBER][MAX_CHAR_NAME_LENGTH + 1];
	char PositionName[GUILD_MAX_MEMBER][GUILD_POSITION_NAME+1];
	CLCString strCharName( MAX_CHAR_NAME_LENGTH  + 1 );
	CLCString strPositionName( GUILD_POSITION_NAME +1);
	char job[GUILD_MAX_MEMBER];
	char job2[GUILD_MAX_MEMBER];
	int level[GUILD_MAX_MEMBER];
	int position[GUILD_MAX_MEMBER];

	RefMsg(msg) >> charindex
		>> guildindex
		>> errorcode
		>> membercount;
	CPC* pc = PCManager::instance()->getPlayerByCharIndex( charindex );
	CGuild* guild = gserver->m_guildlist.findguild( guildindex );
	if( !pc || !guild )
		return;

	if( errorcode == MSG_NEW_GUILD_MEMBERLIST_ERROR_OK )
	{
		for( int i = 0; i < membercount; i++ )
		{
			RefMsg(msg) >> membercharindex[i]
				>> cumulatePoint[i]
				>> strCharName
				>> strPositionName
				>> job[i]
				>> job2[i]
				>> level[i]
				>> position[i];
			strcpy( CharName[i], strCharName );
			strcpy( PositionName[i], strPositionName );
			guild->AddGradeExPosCount(position[i]);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildMemberListRep( rmsg, membercount, membercharindex, cumulatePoint, CharName, PositionName, job, job2, level, position, guild );
			SEND_Q( rmsg, pc->m_desc );
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )errorcode );
		SEND_Q( rmsg, pc->m_desc );
	}
}

void ProcHelperNewGuilManageRep( CNetMsg::SP& msg )
{
	int charindex;
	int guildindex;
	int errorcode;
	int membercount;
	int membercharindex[GUILD_MAX_MEMBER];
	int contributeExp[GUILD_MAX_MEMBER];
	int contributeFame[GUILD_MAX_MEMBER];

	int level[GUILD_MAX_MEMBER];
	char charName[GUILD_MAX_MEMBER][MAX_CHAR_NAME_LENGTH  + 1];
	char positionName[GUILD_MAX_MEMBER][GUILD_POSITION_NAME+1];
	CLCString strCharName( MAX_CHAR_NAME_LENGTH  + 1 );
	CLCString strPositionName( GUILD_POSITION_NAME+1 );
	int position[GUILD_MAX_MEMBER];
#ifdef DEV_GUILD_STASH
	char	cStashAuth[GUILD_MAX_MEMBER] = {0,};
#endif //DEV_GUILD_STASH
	char first;

	RefMsg(msg) >> first
		>> charindex
		>> guildindex
		>> errorcode
		>> membercount;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex( charindex );
	CGuild* guild = gserver->m_guildlist.findguild( guildindex );
	if( !pc || !guild )
		return;

	if( errorcode == MSG_NEW_GUILD_MANAGE_ERROR_OK )
	{
		for( int i = 0; i < membercount; i++ )
		{
			RefMsg(msg) >> membercharindex[i]
				>> contributeExp[i]
				>> contributeFame[i]
				>> strCharName
				>> strPositionName
				>> level[i]
				>> position[i];
#ifdef DEV_GUILD_STASH
			RefMsg(msg) >> cStashAuth[i];
#endif //DEV_GUILD_STASH
			strcpy( charName[i], strCharName );
			strcpy( positionName[i], strPositionName );
			guild->AddGradeExPosCount(position[i]);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
#ifdef DEV_GUILD_STASH
			GuildNewManageRep( rmsg, membercount, membercharindex, contributeExp, contributeFame, charName, positionName, level, position, cStashAuth, guild, first );
#else
			GuildNewManageRep( rmsg, membercount, membercharindex, contributeExp, contributeFame, charName, positionName, level, position, guild, first );
#endif //DEV_GUILD_STASH
			SEND_Q( rmsg, pc->m_desc );
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )errorcode );
		SEND_Q( rmsg, pc->m_desc );
	}
}
void ProcHelperNewGuildNotifyRep( CNetMsg::SP& msg )
{
	int charindex;
	int guildindex;

	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );

	RefMsg(msg) >> charindex
		>> guildindex
		>> title
		>> text;
	CPC* pc = PCManager::instance()->getPlayerByCharIndex( charindex );
	CGuild* guild = gserver->m_guildlist.findguild( guildindex );
	if( !pc || !guild )
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildNewNotify( rmsg, title, text );
		SEND_Q( rmsg, pc->m_desc );
	}
}

void ProcHelperNewGuildNotifyUpdateReP( CNetMsg::SP& msg )
{
	int charindex;
	int guildindex;
	int errorcode;

	RefMsg(msg) >> charindex
		>> guildindex
		>> errorcode;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex( charindex );
	CGuild* guild = gserver->m_guildlist.findguild( guildindex );

	if( !pc || !guild )
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )errorcode );
		SEND_Q( rmsg, pc->m_desc );
	}
}

void ProcHelperNewGuildNotifyTrans( CNetMsg::SP& msg )
{
	int	guildindex;
	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );

	RefMsg(msg) >> guildindex
		>> title
		>> text;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildNewNotifyTrans(rmsg, guild->name(), title, text);
		guild->SendToAll(rmsg);
	}
}

void ProcHelperNewGuildNotifyTransRepMsg( CNetMsg::SP& msg )
{
	int	guildindex;
	int charindex;
	int error;

	RefMsg(msg) >> guildindex
		>> charindex
		>> error;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex( charindex );
	CGuild* guild = gserver->m_guildlist.findguild( guildindex );

	if( !pc || !guild )
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )error );
		SEND_Q( rmsg, pc->m_desc );
	}
}

void ProcHelperNewGuildSkillListRepMsg( CNetMsg::SP& msg )
{
	int i = 0;
	int charindex = 0;
	int guildindex = 0;
	int error;
	int checkflag = 0;
	int active_skillcount = 0;
	int passive_skillcount = 0;
	int etc_skillcount = 0;
	boost::scoped_array< int > active_skill_index;
	boost::scoped_array< int > passive_skill_index;
	boost::scoped_array< int > etc_skill_index;
	boost::scoped_array< int > active_skill_level;
	boost::scoped_array< int > passive_skill_level;
	boost::scoped_array< int > etc_skill_level;
	boost::scoped_array< int > active_skill_cooltime;
	bool endloop = true;

	RefMsg(msg) >> charindex
		>> guildindex
		>> error;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex( charindex );
	CGuild* guild = gserver->m_guildlist.findguild( guildindex );

	if( !pc || !guild )
		return;
	if(error == MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
		SEND_Q( rmsg, pc->m_desc );
		return ;
	}
	while (endloop)
	{
		RefMsg(msg) >> checkflag;
		switch(checkflag)
		{
		case GUILD_ACTIVE_SKILL_START:
			{
				RefMsg(msg) >> active_skillcount;
				active_skill_index.reset(new int[active_skillcount]);
				active_skill_level.reset(new int[active_skillcount]);
				active_skill_cooltime.reset(new int[active_skillcount]);
				for(i = 0; i < active_skillcount; i++)
				{
					RefMsg(msg) >> active_skill_index[i]
						>> active_skill_level[i];
				}
				if( error == MSG_NEW_GUILD_SKILL_ERROR_OK )
				{
					for(i = 0; i < active_skillcount; i++)
					{
						CSkill* skill = guild->m_activeSkillList.Find(active_skill_index[i]);
						if(!skill)
						{
							GAMELOG << init("NEW_GUILD_SKILL_LIST", pc)
									<< "guild " << delim << guildindex << delim
									<< "error " << delim << error << delim
									<< "skill count " << delim << active_skillcount << delim
									<< " skill index " << delim << active_skill_index[i] << end;

							return;
						}
						if(skill->m_proto->m_index == active_skill_index[i])
						{
							if(active_skill_level[i] != skill->m_level)
								active_skill_level[i] = skill->m_level;

							std::map<int, int>::iterator it = skill->m_gskill_usetime.find(pc->m_index);

							if(it != skill->m_gskill_usetime.end())
							{
								active_skill_cooltime[i] = int(gserver->m_nowseconds) - ((gserver->m_pulse - it->second) / 10);
							}
							else
							{
								active_skill_cooltime[i] = 0;
							}
						}
					}
				}
			}
			break;
		case GUILD_PASSIVE_SKILL_START:
			{
				RefMsg(msg) >> passive_skillcount;
				passive_skill_index.reset(new int[passive_skillcount]);
				passive_skill_level.reset(new int[passive_skillcount]);
				for(i = 0; i < passive_skillcount; i++)
				{
					RefMsg(msg) >> passive_skill_index[i]
						>> passive_skill_level[i];
				}
				if( error == MSG_NEW_GUILD_SKILL_ERROR_OK )
				{
					for(i = 0; i < passive_skillcount; i++)
					{
						CSkill* skill = guild->m_passiveSkillList.Find(passive_skill_index[i]);
						if(!skill)
						{
							GAMELOG << init("NEW_GUILD_SKILL_LIST", pc)
									<< "guild " << delim << guildindex << delim
									<< "error " << delim << error << delim
									<< "skill count " << delim << passive_skillcount << delim
									<< " skill index " << delim << passive_skill_index[i] << end;

							return;
						}
						if(skill->m_proto->m_index == passive_skill_index[i])
						{
							if(passive_skill_level[i] != skill->m_level)
								passive_skill_level[i] = skill->m_level;
						}
					}
				}
			}
			break;
		case GUILD_ETC_SKILL_START:
			{
				RefMsg(msg) >> etc_skillcount;
				etc_skill_index.reset(new int[etc_skillcount]);
				etc_skill_level.reset(new int[etc_skillcount]);
				for(i = 0; i < etc_skillcount; i++)
				{
					RefMsg(msg) >> etc_skill_index[i]
						>> etc_skill_level[i];
				}
				if( error == MSG_NEW_GUILD_SKILL_ERROR_OK )
				{
					for(i = 0; i < etc_skillcount; i++)
					{
						CSkill* skill = guild->m_etcSkillList.Find(etc_skill_index[i]);
						if(!skill)
						{
							GAMELOG << init("NEW_GUILD_SKILL_LIST", pc)
									<< "guild " << delim << guildindex << delim
									<< "error " << delim << error << delim
									<< "skill count " << delim << etc_skillcount << delim
									<< " skill index " << delim << etc_skill_index[i] << end;

							return;
						}
						if(skill->m_proto->m_index == etc_skill_index[i])
						{
							if(etc_skill_level[i] != skill->m_level)
								etc_skill_level[i] = skill->m_level;
						}
					}
				}
			}
			break;
		}

		if(checkflag == GUILD_ETC_SKILL_START)
			endloop = false;
	}
	// 메세지 전송 시작
	if( error == MSG_NEW_GUILD_SKILL_ERROR_OK )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildSkillListRepMsg(rmsg, active_skillcount, active_skill_index.get(), active_skill_level.get(), active_skill_cooltime.get(),
								passive_skillcount, passive_skill_index.get(), passive_skill_level.get(),
								etc_skillcount, etc_skill_index.get(), etc_skill_level.get());
		SEND_Q(rmsg, pc->m_desc);
	}
	// 메세지 전송 끝

	return ;
}

void ProcHelperNewGuildLoadNTF( CNetMsg::SP& msg )
{
	int guildindex;
	int guildpoint;
	char guildincline;
	int guildmaxmeber;
	int landcount;
	int land[256];
	int skillcount;
	int skillIndex[256];
	int skillLevel[256];
	memset( land, -1, sizeof( land) );
	int skilltype;
	RefMsg(msg) >> guildindex
		>> guildpoint
		>> guildincline
		>> guildmaxmeber
		>> landcount;
	if( landcount != 0 )
	{
		for( int i = 0 ; i < landcount; i++ )
		{
			RefMsg(msg) >> land[i];
		}
	}
	else
		RefMsg(msg) >> land[0];

	RefMsg(msg) >> skillcount;
	int i;
	for( i = 0; i < skillcount; i++ )
	{
		RefMsg(msg) >> skillIndex[i]
			>> skillLevel[i];
	}
	RefMsg(msg) >> skilltype;

	CGuild* guild = gserver->m_guildlist.findguild( guildindex );

	if( !guild )
		return;

	guild->guildPoint( guildpoint );
	guild->incline( guildincline );
	guild->landcount( landcount );
	guild->setMaxmember( guildmaxmeber );
	guild->land( landcount, land );

	if( skillcount > 0 )
	{
		for( i = 0; i < skillcount; i++ )
		{
			CSkill* skill = gserver->m_skillProtoList.Create( skillIndex[i], skillLevel[i] );
			if( skill )
			{
				switch(skilltype)
				{
				case 0:
					guild->m_activeSkillList.Add(skill);
					break;
				case 1:
					guild->m_passiveSkillList.Add( skill );
					break;
				case 2:
					guild->m_etcSkillList.Add(skill);
					break;
				default:
					break;
				}
			}
		}
	}

	guild->SendToAllStatusMsg();
}

void ProcHelperNewGuildMemberList( CNetMsg::SP& msg )
{
	int guildindex;
	int guildmembercount;
	int charindex;
	int contributeExp;
	int contributeFame;
	int cumulatePoint;
	int channel;
	int zoneNum;

	CLCString positionName( GUILD_POSITION_NAME + 1 );
	RefMsg(msg) >> guildindex
		>> guildmembercount;

	CGuild* guild = gserver->m_guildlist.findguild( guildindex );
	if( !guild )
		return;

	int i;
	for( i = 0; i < guildmembercount; i++ )
	{
		RefMsg(msg) >> charindex
			>> contributeExp
			>> contributeFame
			>> cumulatePoint
			>> positionName
			>> channel
			>> zoneNum;
		CGuildMember* member = guild->findmember( charindex );
		if( !member )
			continue;

			member->contributeExp( contributeExp );
			member->contributeFame( contributeFame );
			member->cumulatePoint( cumulatePoint );
			member->positionName( positionName );
			member->channel( channel );
			member->zoneindex( zoneNum );
	}
}

void ProcHelperNewGuildInfoRepMsg( CNetMsg::SP& msg )
{
	int charindex;
	int errorcode;

	RefMsg(msg) >> charindex
		>> errorcode;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex( charindex );

	if( !pc )
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, (MSG_GUILD_ERROR_TYPE )errorcode );
		SEND_Q( rmsg, pc->m_desc );
	}
}

void ProcHelperNewGuildPointUpdateMsg( CNetMsg::SP& msg )
{
	int charindex;
	int guildindex;
	int guildpoint;
	RefMsg(msg) >> charindex
		>> guildindex
		>> guildpoint;
	CPC* pc = PCManager::instance()->getPlayerByCharIndex( charindex );
	CGuild* guild = gserver->m_guildlist.findguild( guildindex );
	if( !pc && !guild )
		return;

	guild->guildPoint( guildpoint );
}

void ProcHelperNewGuildNotice( CNetMsg::SP& msg )
{
	int charindex;
	int	guildindex;
	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );

	RefMsg(msg) >> charindex
		>> guildindex
		>> title
		>> text;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex( charindex );
	if( !pc )
		return;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
		return ;

	if( strlen(title) != 0 && strlen(text) != 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildNewNotifyTrans(rmsg, guild->name(), title, text);
		SEND_Q( rmsg, pc->m_desc );
	}
}

void ProcHelperNewGuildMemberPointSaveMsg(CNetMsg::SP& msg)
{
	int nCharIdx;
	int nGuildIdx;
	int nMemberPoint;

	RefMsg(msg) >> nGuildIdx
		>> nCharIdx
		>> nMemberPoint;

	CPC* pPC = PCManager::instance()->getPlayerByCharIndex(nCharIdx);
	CGuildMember* pMember = gserver->m_guildlist.findmember(nCharIdx);
	if(!pPC || !pMember)
		return;

	pMember->cumulatePoint(nMemberPoint);
}

void ProcHelperNewGuildSkillLearnSendMember(CNetMsg::SP& msg)
{
	int guild_index;
	int skill_type, skill_index, skill_level;

	RefMsg(msg) >> guild_index
		>> skill_type
		>> skill_index
		>> skill_level;

	CGuild* guild = gserver->m_guildlist.findguild(guild_index);

	if(guild == NULL)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makenewGuildSkillLearnToMemberMsg(rmsg, skill_type, skill_index, skill_level);
		guild->SendToAll(rmsg, false);
	}
}

void ProcHelperPetChangeName( CNetMsg::SP& msg )
{
	int err;
	int charidx;
	int petidx;

	CLCString PetName( MAX_CHAR_NAME_LENGTH + 1 );

	CPC* pc = NULL;

	RefMsg(msg) >> err
		>> charidx
		>> petidx
		>> PetName;
	pc = PCManager::instance()->getPlayerByCharIndex( charidx );

	if( err == MSG_EX_PET_CHANGE_NAME_ERROR_OK )
	{
		if( pc == NULL )
		{
			GAMELOG << init( "CASH_ITEM_USE_ERROR-notPC" )
					<< charidx << delim
					<< 2360 << end;
			// Helper로 롤백 처리
			PetName = "";
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPetNameChange( rmsg, charidx, petidx, PetName );
			SEND_Q( rmsg, gserver->m_helper );
/*
			PetNameChange( rmsg, (MSG_EX_PET_CHANGE_NAME_ERROR_TYPE)err, petidx, PetName );
			SEND_Q( rmsg, pc->m_desc );
*/
			return;
		}
		else
		{
			CItem* pItem = pc->m_inventory.FindByDBIndex(2360, 0, 0);
			if (pItem == NULL)
			{
				GAMELOG << init( "CASH_ITEM_USE_ERROR-notPC" )
						<< charidx << delim
						<< 2360 << end;

				{
					// Helper로 롤백 처리
					PetName = "";
					CNetMsg::SP rmsg(new CNetMsg);
					HelperPetNameChange( rmsg, charidx, petidx, PetName );
					SEND_Q( rmsg, gserver->m_helper );
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					PetNameChange( rmsg, (MSG_EX_PET_CHANGE_NAME_ERROR_TYPE)err, petidx, PetName );
					SEND_Q( rmsg, pc->m_desc );
				}

				return;
			}

			CPet* pet = pc->GetPet( petidx );

			if( (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_CASH ) )
			{
				GAMELOG << init( "CASH_ITEM_USE", pc )
						<< itemlog( pItem ) << delim
						<< pet->m_name << delim << end;
			}

			{
				//Pet 이름 변경
				pet->m_name = PetName;
				CNetMsg::SP rmsg(new CNetMsg);
				PetNameChange( rmsg, (MSG_EX_PET_CHANGE_NAME_ERROR_TYPE)err, petidx, PetName );
				SEND_Q( rmsg, pc->m_desc );
				pet->m_pArea->SendToCell( rmsg, pet, true );
			}

			{
				// item 수량 변경
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::ItemUseMsg( rmsg, pItem->tab(), pItem->getInvenIndex(), pItem->m_itemProto->getItemIndex(), 0 );
				SEND_Q( rmsg, pc->m_desc );
			}

			pc->m_inventory.decreaseItemCount(pItem, 1);

			return;
		}
	}
	else
	{
		if( pc )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			PetNameChange( rmsg, (MSG_EX_PET_CHANGE_NAME_ERROR_TYPE)err, petidx, PetName );
			SEND_Q( rmsg, pc->m_desc );
			return;
		}
	}
}

void ProcHelperHalloween2007( CNetMsg::SP& msg )
{
	int charIndex;
	unsigned char error;
#ifdef EX_ROGUE
#ifdef EX_MAGE
	int nPriceItem[JOBCOUNT] = { 7544, 7544, 7544, 7544, 7544, 7544, 7544, 7544, 7544 };
#else
	int nPriceItem[JOBCOUNT] = { 7544, 7544, 7544, 7544, 7544, 7544, 7544, 7544 };
#endif
#else
	int nPriceItem[7] = { 7544, 7544, 7544, 7544, 7544, 7544, 7544 };
#endif // EX_ROGUE
	// JOB_TITAN 0, JOB_KNIGHT 1, JOB_HEALER 2, JOB_MAGE 3, JOB_ROGUE 4, JOB_SORCERER 5, JOB_NIGHTSHADOW 6

	RefMsg(msg) >> charIndex >> error ;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(charIndex);
	if( ch == NULL )
		return; // 해당유저 없음

	if( error == 0 ) // 아이템 지급
	{
		int nRow, nCol;
		CItem* pPriceItem = gserver->m_itemProtoList.CreateItem( nPriceItem[ch->m_job] , -1, 0, 0, 10 );
		if( !pPriceItem )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}

		// 아이템 지급
		if (ch->m_inventory.addItem(pPriceItem) == false)
		{
			delete pPriceItem;

			CNetMsg::SP rmsg(new CNetMsg);
			EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN ); // FULL INVEN
			SEND_Q( rmsg, ch->m_desc );
			return;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventHalloween2007Msg( rmsg, MSG_EVENT_TAKE_DEVILHEAIR, 0 ); // Sucess
			SEND_Q( rmsg, ch->m_desc );
		}
	}
	else	// 이미 받은적 있음
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventHalloween2007Msg( rmsg, MSG_EVENT_TAKE_DEVILHEAIR, 1 );
		SEND_Q( rmsg, ch->m_desc );
	}
}

void ProcHelperDVDRateChange( CNetMsg::SP& msg )
{
	unsigned char	subtype;
	int				nRate;

	RefMsg(msg) >> subtype >> nRate;

#if defined(LC_GAMIGO)
	GAMELOG << init("DRATAN DYNAMIC DUNGEON")
		    << subtype << delim
			<< nRate << delim;
#endif // LC_GAMIGO

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if(pCastle->m_dvd.GetZone() != NULL )
	{
		switch( subtype )
		{
		case MSG_MANAGEMENT_TAX_CHANGE :
			pCastle->m_dvd.SetFeeRate(nRate);
#if defined(LC_GAMIGO)
			GAMELOG << "TAX CHANGE";
#endif // LC_GAMIGO
			break;
		case MSG_MANAGEMENT_HUNTER_TAX_CHANGE:
			pCastle->m_dvd.SetHuntRate(nRate);
#if defined(LC_GAMIGO)
			GAMELOG << "HUNTER TAX CHANGE";
#endif // LC_GAMIGO
			break;
		case MSG_MANAGEMENT_ENV_CHANGE:
			pCastle->m_dvd.SetEnvRate(nRate);
			pCastle->m_dvd.SetChangeTime(gserver->m_pulse);
			pCastle->m_dvd.ChangeSetting();
#if defined(LC_GAMIGO)
			GAMELOG << "ENV CHANGE";
#endif // LC_GAMIGO
			break;
		case MSG_MANAGEMENT_STATE_CHANGE:
			pCastle->m_dvd.SetMobRate(nRate);
			pCastle->m_dvd.SetChangeTime(gserver->m_pulse);
			pCastle->m_dvd.ChangeSetting();
#if defined(LC_GAMIGO)
			GAMELOG << "STATE CHANGE";
#endif // LC_GAMIGO
			break;
		}
	}

#if defined(LC_GAMIGO)
	GAMELOG << end;
#endif // LC_GAMIGO
}

void ProcHelperDVDNormalChangeNotice (CNetMsg::SP& msg)
{
	CNetMsg::SP rmsg(new CNetMsg);
	DVDDungeonNormalChangeNoticeMsg(rmsg);
	PCManager::instance()->sendToAll(rmsg);

	//드라탄 성주를 찾아보고 있으면 패킷 전달.
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	int char_index = pCastle->GetOwnerCharIndex();

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(char_index);
	
	if(pc != NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		DVDDungeonChangeToOwnerMsg(rmsg);
		SEND_Q(rmsg, pc->m_desc);
	}
}

void ProcHelperComboGotoWaitRoomPrompt( CNetMsg::SP& msg )
{
	CLCString bossname(256);
	int		bossindex;
	int		nas;

	RefMsg(msg) >> bossindex
		>> bossname;

	CParty* party = gserver->FindPartyByBossIndex(bossindex);
	if(party)
	{
		int i = gserver->m_comboZone->FindComboArea(party->m_comboAreaIndex);
		if(i == -1)
			return;

		CArea* area = gserver->m_comboZone->m_area + i;
		nas = area->m_monsterCombo->GetTotalNas();

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeMCGotoWaitRoomPrompt(rmsg, bossindex, nas);

		for(i = 0 ; i < MAX_PARTY_MEMBER; ++i)
		{
			if( party->GetMemberByListIndex(i) &&
				party->GetMemberByListIndex(i)->GetMemberPCPtr() &&
				party->GetMemberByListIndex(i)->GetCharIndex() != bossindex)
			{
				SEND_Q(rmsg, party->GetMemberByListIndex(i)->GetMemberPCPtr()->m_desc);
			}
		}
	}
}

void ProcHelperComboRecallToWaitRoomPrompt(CNetMsg::SP& msg)
{
	int bossindex;
	int nas;

	RefMsg(msg) >> bossindex;

	CParty* party = gserver->FindPartyByBossIndex(bossindex);
	if(party)
	{
		// 파티가 던전에 있는지
		int ret;
		ret = gserver->m_comboZone->FindComboArea(party->m_comboAreaIndex);
		if(ret == -1)
			return ;

		CArea* area = gserver->m_comboZone->m_area + ret;
		nas = area->m_monsterCombo->GetTotalNas();

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeMCGotoWaitRoomPrompt(rmsg, bossindex, nas);
		for(int i = 0 ; i < MAX_PARTY_MEMBER; ++i)
		{
			if( party->GetMemberByListIndex(i) &&
				party->GetMemberByListIndex(i)->GetMemberPCPtr())
			{
				CPC* pc = party->GetMemberByListIndex(i)->GetMemberPCPtr();
				if(pc->m_pZone->m_index != ZONE_COMBO_DUNGEON)
				{
					SEND_Q(rmsg, party->GetMemberByListIndex(i)->GetMemberPCPtr()->m_desc);
				}
			}
		}
	}
}

void ProcHelperAttackPet( CNetMsg::SP& msg )
{
	unsigned char type;
	int charIndex;

	RefMsg(msg) >> type
		>> charIndex;

	CPC* pCh = PCManager::instance()->getPlayerByCharIndex(charIndex);
	if( !pCh )
		return;

	switch( type )
	{
	case MSG_HELPER_APET_CREATE_OK:
		{
			int pet_dbIndex, pet_protoIndex;
			RefMsg(msg) >> pet_dbIndex
				>> pet_protoIndex;

			if(pCh->m_inventory.getEmptyCount() < 1)
			{
				// 최소 인벤 1칸
				return;
			}

			// 펫 생성
			CAPet* apet = gserver->m_pApetlist.Create( pCh, pet_dbIndex , pet_protoIndex );
			if( !apet )
				return;
			ADD_TO_BILIST(apet,pCh->m_pApetlist,m_pPrevPet,m_pNextPet);
			// 펫 아이템 지급
			CAPetProto* proto = gserver->m_pApetlist.FindProto( pet_protoIndex );
			if( proto )
			{
				CItem*petItem =  gserver->m_itemProtoList.CreateItem( proto->m_nItemIndex, -1, pet_dbIndex, 0, 1 );
				if (pCh->m_inventory.addItem(petItem))
				{
					GAMELOG << init("CREATE APET", pCh) << delim
							<< "APET : " <<  pet_dbIndex << delim << pet_protoIndex
							<< end;

					{
						CNetMsg::SP rmsg(new CNetMsg);
						HelperAttackPetMsg( rmsg , MSG_HELPER_APET_ACCENPT_REQ, charIndex );
						RefMsg(rmsg) << pet_dbIndex;
						SEND_Q(rmsg, gserver->m_helper );
					}
				}
				else
				{
					delete petItem;

					// 실패
					GAMELOG << init("CREATE APET FAIL", pCh) << delim
							<< "APET : " <<  pet_dbIndex << delim << pet_protoIndex
							<< end;
				}
			}
		}
		break;
	case MSG_HELPER_APET_CREATE_FAILED:
		{
			// 아이템 제거
		}
		break;

	case MSG_HELPER_APET_ACCENPT_OK :
		{
			// 펫 마지막 처리 성공
		}
		break;

	case MSG_HELPER_APET_ACCENPT_FAILED :
		{
			// 메모리에서 펫 찾고 삭제
			// 헬퍼 에 dbindex 삭제 요청
			int pet_dbIndex, item_index ;
			RefMsg(msg) >> pet_dbIndex
				>> item_index ;

			CItem* item = pCh->m_inventory.FindByDBIndex(item_index, pet_dbIndex, 0);
			if (item)
			{
				pCh->m_inventory.decreaseItemCount(item, 1);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperAttackPetMsg( rmsg , MSG_HELPER_APET_DELETE_REQ, charIndex );
				RefMsg(rmsg) << pet_dbIndex
					<< (char) 3;
				SEND_Q(rmsg, gserver->m_helper);
			}
		}break;
	}
}

#ifdef EXTREME_CUBE
#ifdef EXTREME_CUBE_VER2
void ProcHelperCubeStateRep(CNetMsg::SP& msg)
{
	int charindex;
	int cubepoint;
	int selfpoint;
	char i, count, rank;
	CPC* ch;
	CLCString guildname(MAX_GUILD_NAME_LENGTH + 1), guildmaster(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> charindex >> selfpoint >> count;

	ch = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(!ch)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_EXTREME_CUBE
			<< (char)MSG_EX_EXTREME_CUBE_STATE_REP
			<< selfpoint
			<< count;

		for(i=0;i<count;i++)
		{
			RefMsg(msg) >> rank >> guildname >> guildmaster >> cubepoint;
			RefMsg(rmsg) << rank << guildname << guildmaster << cubepoint;
		}

		RefMsg(msg) >> count;
		RefMsg(rmsg) << count;
		for(i=0;i<count;i++)
		{
			RefMsg(msg) >> rank >> guildname >> guildmaster >> cubepoint;
			RefMsg(rmsg) << rank << guildname << guildmaster << cubepoint;
		}

		SEND_Q(rmsg, ch->m_desc);
	}
}

void ProcHelperCubeStatePersonalRep(CNetMsg::SP& msg)
{
	int charindex;
	int cubepoint, selfpoint;
	char i, count, rank;

	CPC* ch;
	CLCString guildname(MAX_GUILD_NAME_LENGTH + 1), charname(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> charindex >> selfpoint >>count;

	ch = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(!ch)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_EXTREME_CUBE
			<< (char)MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REP
			<< selfpoint
			<< count;

		for(i = 0 ; i < count; ++i)
		{
			RefMsg(msg) >> rank
				>> guildname
				>> charname
				>> cubepoint;

			RefMsg(rmsg) << rank
				<< guildname
				<< charname
				<< cubepoint;
		}

		RefMsg(msg) >> count;

		RefMsg(rmsg) << count;
		for(i=0;i<count;++i)
		{
			RefMsg(msg) >> rank
				>> guildname
				>> charname
				>> cubepoint;

			RefMsg(rmsg) << rank
				<< guildname
				<< charname
				<< cubepoint;
		}

		SEND_Q(rmsg, ch->m_desc);
	}
}

void ProcHelperGuildCubeNotice(CNetMsg::SP& msg)
{
	int type;
	RefMsg(msg) >> type;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		switch(type)
		{
		case 0:
			ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_START_NOTICE);
			break;
		case 1:
			ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_END_NOTICE);
			break;
		case 2:
			ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_START_REMAINTIME);
			break;
		case 3:
			ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_END_REMAINTIME);
			break;
		default:
			return ;
		}
		PCManager::instance()->sendToAll(rmsg);
	}
}

void ProcHelperCubeRewardPersonalRep(CNetMsg::SP& msg)
{
	int charindex, updatetime;
	char rank;
	CPC* pc;
	RefMsg(msg) >> updatetime
		>> charindex
		>> rank;

	pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(!pc)
	{
		if (gserver->isRunHelper())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperCubeRewardPersonalRollBackMsg(rmsg, updatetime, charindex);
			SEND_Q(rmsg, gserver->m_helper);

			GAMELOG << init("EXTREME_CUBE_REWARD_PERSONAL_ROLLBACK")
					<< "UPDATE TIME" << delim << updatetime << delim
					<< "CHAR INDEX" << delim << charindex << delim
					<< "RANK" << delim << rank << end;
		}
		return ;
	}

	static const int rewarditem[][4] =
	{
		{6076, 6077, 6078, 6079},
		{6076, 6078,},
		{6077, 6079,},
	};

	int itemkind;
//	int remainWeight, sumWeight;
	switch(rank)
	{
	case 1:
		itemkind=4;
		break;
	case 2:
		itemkind=2;
		break;
	case 3:
		itemkind=2;
		break;
	default:
		itemkind=0;
		return ;
	}
/*
	sumWeight = 0;
	remainWeight = pc->m_maxWeight * 3 / 2 - pc->m_weight;

	CItemProto* itemproto;
	for(i=0; i<itemcount; i++)
	{
		itemproto = gserver->m_itemProtoList.FindIndex(rewarditem[rank-1][i]);
		if(itemproto)
			sumWeight += itemproto->m_weight;
	}
*/
	if(/*sumWeight > remainWeight || */ itemkind > pc->m_inventory.getEmptyCount())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperCubeRewardPersonalRollBackMsg(rmsg, updatetime, charindex);
		SEND_Q(rmsg, gserver->m_helper);

		GAMELOG << init("EXTREME_CUBE_REWARD_PERSONAL_ROLLBACK", pc)
				<< "UPDATE TIME" << delim << updatetime << delim
				<< "RANK" << delim << rank << end;
		return ;
	}

	for (int i = 0; i < itemkind; i++)
	{
		CItem* item = gserver->m_itemProtoList.CreateItem(rewarditem[rank-1][i], -1, 0, 0, 1);
		if(item)
		{
			if (pc->m_inventory.addItem(item) == false)
			{
				delete item;
				continue;
			}
		}
	}

	GAMELOG << init("EXTREME_CUBE_REWARD_PERSONAL_OK", pc)
			<< "UPDATE TIME" << delim << updatetime << delim
			<< "RANK" << delim << rank << end;
}

void ProcHelperExtremeCubeError(CNetMsg::SP& msg)
{
	char errortype;
	int charindex;

	RefMsg(msg) >> charindex
		>> errortype;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(!pc)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		switch(errortype)
		{
		case MSG_HELPER_EXTREME_CUBE_ERROR_REWARD_PERSONAL_CANNOT:
			ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_REWARD_PERSONAL_CANNOT);
			break;
		case MSG_HELPER_EXTREME_CUBE_ERROR_REWARD_PERSONAL_ALREADY:
			ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_REWARD_PERSONAL_ALREADY);
			break;
		default:
			return ;
		}

		SEND_Q(rmsg, pc->m_desc);
	}
}

void ProcHelperCubeRewardGuildPointRep(CNetMsg::SP& msg)
{
	int guildidx, guildpoint;
	char rank;

	RefMsg(msg) >> guildidx
		>> rank
		>> guildpoint;

	CGuild* guild = gserver->m_guildlist.findguild(guildidx);
	if(guild != NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_EXTREME_CUBE
			 << (char)MSG_EX_EXTREME_CUBE_REWARD_GUILDPOINT_REP
			 << rank
			 << guildpoint;

		guild->SendToAll(rmsg);
	}
}
#else
void ProcHelperCubeStateRep(CNetMsg::SP& msg)
{
	int charindex;
	int cubepoint;
	int selfpoint;
	char i, count, rank;
	CPC* ch;
	CLCString guildname(MAX_GUILD_NAME_LENGTH + 1), guildmaster(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> charindex >> selfpoint >> count;

	ch = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(!ch)
		return ;

	if(count == 0)
	{
		// 순위 없음
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_EXTREME_CUBE
			 << (char)MSG_EX_EXTREME_CUBE_STATE_REP
			 << selfpoint
			 << (int)0;

		SEND_Q( rmsg, ch->m_desc );
		return ;
	}

	if(count > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_EXTREME_CUBE
			 << (char)MSG_EX_EXTREME_CUBE_STATE_REP
			 << selfpoint
			 << count;

		for(i = 0; i < count; ++i)
		{
			RefMsg(msg) >> rank >> guildname >> guildmaster >> cubepoint;

			RefMsg(rmsg) << rank << guildname << guildmaster << cubepoint;
		}
		SEND_Q( rmsg, ch->m_desc );
	}
}

void ProcHelperCubeStatePersonalRep(CNetMsg::SP& msg)
{
	int charindex;
	int cubepoint, selfpoint;
	char i, count, rank;

	CPC* ch;
	CLCString guildname(MAX_GUILD_NAME_LENGTH + 1), charname(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> charindex >> selfpoint >>count;

	ch = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(!ch)
		return ;

	if(count == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_EXTREME_CUBE
			 << (char)MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REP
			 << selfpoint
			 << (int)0;

		SEND_Q(rmsg, ch->m_desc );
		return ;
	}

	if(count > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_EXTREME_CUBE
			 << (char)MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REP
			 << selfpoint
			 << count;

		for(i = 0 ; i < count; ++i)
		{
			RefMsg(msg) >> rank
				>> guildname
				>> charname
				>> cubepoint;

			RefMsg(rmsg) << rank
				 << guildname
				 << charname
				 << cubepoint;
		}
		SEND_Q(rmsg, ch->m_desc );
	}
}

void ProcHelperGuildCubeNotice(CNetMsg::SP& msg)
{
	int type;
	RefMsg(msg) >> type;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		switch(type)
		{
		case 0:
			ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_START_NOTICE);
			break;
		case 1:
			ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_END_NOTICE);
			break;
		case 2:
			ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_START_REMAINTIME);
			break;
		case 3:
			ExtremeCubeErrorMsg(rmsg, MSG_EX_EXTREME_CUBE_ERROR_END_REMAINTIME);
			break;
		default:
			return ;
		}
		PCManager::instance()->sendToAll(rmsg);
	}
}
#endif // EXTREME_CUBE_VER2
#endif // EXTREME_CUBE

void ProcHelperEventPhoenix( CNetMsg::SP& msg )
{
	unsigned char type;
	int charindex = 0;

	RefMsg(msg) >> type >> charindex;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(charindex);
	if( !ch )
	{
		return;
	}

	// 케릭터 디비에서 검색했을 때 피닉스 이벤트가 가능하다는 응답이 왔을 경우
	if ( (MSG_HELPER_EVENT_PHOENIX_ERROR_TYPE)type == MSG_HELPER_EVENT_PHOENIX_OK )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_OK);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 케릭터 디비에서 검색했을 때 레벨이 100을 넘는 것이 없어서 피닉스 이벤트 자격이 안된다는 응답
	else if ( (MSG_HELPER_EVENT_PHOENIX_ERROR_TYPE)type == MSG_HELPER_EVENT_PHOENIX_ERROR_REQUIREMENT )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_REQUIREMENT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
}

//에러
void ProcHelperExpedErrorRep(CNetMsg::SP& msg)
{
	unsigned char errorType;
	int nCharIndex = -1;

	RefMsg(msg) >> errorType
		>> nCharIndex;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(nCharIndex);
	if(ch == NULL)
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, errorType);
		SEND_Q(rmsg, ch->m_desc);
	}
}
//생성 (파티 전환)
void ProcHelperExped_CreateRep(CNetMsg::SP& msg)
{
	char cExpedType1,cExpedType2,cExpedType3, cExpedType4, cExpedType5;
	int nMemberCount, nBossIndex;
	int nResult=0;	//처리 결과
	int i, j;

	CLCString BossName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> cExpedType1 >> cExpedType2 >> cExpedType3 >> cExpedType4 >> cExpedType5
		>> nBossIndex >> BossName
		>> nMemberCount;

	GAMELOG << init("EXPED DEBUG HELPER CREATE REP")
			<< "cExpedType1" << delim << cExpedType1 << delim
			<< "cExpedType2" << delim << cExpedType2 << delim
			<< "cExpedType3" << delim << cExpedType3 << delim
			<< "cExpedType4" << delim << cExpedType4 << delim
			<< "cExpedType5" << delim << cExpedType5<< delim
			<< "nMemberCount" << delim << nMemberCount << delim
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "BossName" << delim << BossName
			<< end;

	int* pCharIndex		 = new int[nMemberCount];			// 캐릭터 인덱스
	CLCString* pCharName = new CLCString[nMemberCount];		// 캐릭터 이름
	int* pGroupType		 = new int[nMemberCount];			// 그룹 타입
	int* pMemberType	 = new int[nMemberCount];			// 멤버 타입
	int* pSetLabelType	 = new int[nMemberCount];			// 표식
	int* pQuitType		 = new int[nMemberCount];			// 정상 ,비정상

	for( i=0; i < nMemberCount; i++)
	{
		RefMsg(msg) >> pCharIndex[i]
			>> pCharName[i]
			>> pGroupType[i]
			>> pMemberType[i]
			>> pSetLabelType[i]
			>> pQuitType[i];

	GAMELOG << init("EXPED DEBUG HELPER CREATE REP")
			<< "CharIndex[" << i << "]"  << delim << pCharIndex[i] << delim
			<< "CharName[" << i << "]"  << delim << pCharName[i] << delim
			<< "GroupType[" << i << "]"  << delim << pGroupType[i] << delim
			<< "MemberType[" << i << "]"  << delim << pMemberType[i] << delim
			<< "SetLabelType[" << i << "]"  << delim << pSetLabelType[i] << delim
			<< "QuitType[" << i << "]"  << delim << pQuitType[i]
			<< end;
	}

	CExpedition* pExped = NULL;
	int nJoinCnt = 0;

	CPC* pBoss = PCManager::instance()->getPlayerByCharIndex(nBossIndex);

#ifdef EXTREME_CUBE
	CParty* party = gserver->FindPartyByBossIndex(nBossIndex);
	if(party && party->m_cubeUniqueIdx > 0)
	{
		nResult = 0;	//실패
		goto SKIP_EXPEDCREATEREP;
	}
#endif

	if(!pBoss || pExped )
	{
		nResult = 0;	//실패
		goto SKIP_EXPEDCREATEREP;
	}

	// 신규 원정대 생성
	pExped = new CExpedition(cExpedType1, cExpedType2, cExpedType3, cExpedType4, cExpedType5, nBossIndex,(const char*)BossName,MSG_EXPED_GROUP_1,MSG_EXPED_MEMBERTYPE_BOSS,pBoss);
	if(pExped)
	{
		if(pBoss)
		{
			//원정대 포인터 지정(보스)
			pExped->SetMemberPCPtr(pBoss->m_index, pBoss);
			pBoss->m_Exped = pExped;
		}

		CExpedMember* pExpMember = NULL;
		for( i=0; i < nMemberCount; i++)
		{
			CPC* pMember = PCManager::instance()->getPlayerByCharIndex(pCharIndex[i]);
			if(pMember)
			{
				if(nBossIndex != pCharIndex[i])
				{
					//대원 추가
					pExpMember = (CExpedMember*) pExped->Join(pCharIndex[i],pCharName[i],MSG_EXPED_MEMBERTYPE_NORMAL,pMember);
					if(pExpMember)
					{
						//원정대 포인터 지정(대원)
						pExped->SetMemberPCPtr(pMember->m_index, pMember);
						pMember->m_Exped = pExped;

						pExpMember->SetLabelType(pSetLabelType[i]);
						pExpMember->SetQuitType(pQuitType[i]);

						nJoinCnt++;
					}
				}
			}else  //로그아웃 캐릭터 조인 처리
			{
				//대원 추가
				pExpMember = (CExpedMember*) pExped->Join(pCharIndex[i],pCharName[i],MSG_EXPED_MEMBERTYPE_NORMAL,pMember);
				if(pExpMember)
				{
					pExpMember->SetLabelType(pSetLabelType[i]);
					pExpMember->SetQuitType(pQuitType[i]);

					nJoinCnt++;
				}
			}
		}

		if((pExped->GetMemberCount() - 1) == nJoinCnt )
		{
			// 기존 파티 리스트 제거
			CParty* pParty = gserver->FindPartyByBossIndex(nBossIndex);
			if (pParty)
			{
				// 파티 지우기
				gserver->m_listParty.erase(pParty->GetBossIndex());
				pParty->SetEndParty();
				delete pParty;
			}

			{
				//결과 전송(원정대원 모두)
				CNetMsg::SP rmsg(new CNetMsg);
				ExpedCreateRepMsg(rmsg,cExpedType1,cExpedType2,cExpedType3,nBossIndex,BossName,nMemberCount,pCharIndex,pCharName,pGroupType,pMemberType,pSetLabelType,pQuitType);
				pExped->SendToAllPC(rmsg);
			}

			// 성공	(리스트 추가)
			gserver->m_listExped.insert(map_listexped_t::value_type(pExped->GetBossIndex(), pExped));

			for( i = 0; i < MAX_EXPED_GROUP; i++)
			{
				for( j = 0; j < MAX_EXPED_GMEMBER; j++)
				{
					const CExpedMember* pMember = pExped->GetMemberByListIndex(i,j);
					// 모두 전송
					if (pMember)
					{
						//대원들 정보 ==> 나에게 전송
						CNetMsg::SP rmsg(new CNetMsg);
						ExpedAddMsg(rmsg, pMember->GetCharIndex(), pMember->GetCharName(), i, pMember->GetMemberType(), pMember->GetListIndex(), pMember->GetMemberPCPtr());
						pExped->SendToAllPC( rmsg, -1 );

						if(pMember->GetMemberPCPtr() != NULL)
						{
							pMember->GetMemberPCPtr()->CalcStatus(true);
						}
					}
				}
			}

			pExped->SetMemberRegister_AfterGoZone(pBoss);

			nResult = 1;	//성공
		}else
		{
			nResult = 0;	//실패
		}
	}else
	{
		nResult = 0;	//실패
		//에러
	}

SKIP_EXPEDCREATEREP:
	// 성공후 서버로 요청 결과(파티 제거 Helper)(미구현)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedCreateResultMsg(rmsg, nResult, nBossIndex);
		SEND_Q(rmsg, gserver->m_helper);
	}

	delete[] pCharIndex;
	delete[] pCharName;
	delete[] pGroupType;
	delete[] pMemberType;
	delete[] pSetLabelType;
	delete[] pQuitType;
}

//초대
void ProcHelperExped_InviteRep(CNetMsg::SP& msg)
{
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);
	char		cExpedType1;
	char		cExpedType2;
	char		cExpedType3;

	RefMsg(msg) >> nBossIndex
		>> strBossName
		>> nTargetIndex
		>> strTargetName
		>> cExpedType1
		>> cExpedType2
		>> cExpedType3;

	GAMELOG << init("EXPED DEBUG HELPER INVITE REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "strBossName" << delim << strBossName << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "strTargetName" << delim << strTargetName << delim
			<< "cExpedType1" << delim << cExpedType1<< delim
			<< "cExpedType2" << delim << cExpedType2 << delim
			<< "cExpedType3" << delim << cExpedType3
			<< end;

	CPC* pBossPC = PCManager::instance()->getPlayerByCharIndex(nBossIndex);
	CPC* pTargetPC = PCManager::instance()->getPlayerByCharIndex(nTargetIndex);

	if( pTargetPC && ( pTargetPC->IsParty() || pTargetPC->IsExped() ) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		PartyErrorMsg(rmsg, MSG_PARTY_ERROR_ALREADY_TARGET);
		if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
		return;
	}

	CExpedition* pExped = gserver->FindExpedByBossIndex(nBossIndex);
	if (pExped)
	{
		// 기존 원정대 초대 상태로
		pExped->SetRequest(nTargetIndex, strTargetName);
		if (pTargetPC)
			pTargetPC->m_Exped = pExped;
	}

	{
		//초대 메세지
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedInviteRepMsg(rmsg, cExpedType1, cExpedType2, cExpedType3, nBossIndex, strBossName);
		if (pBossPC)		SEND_Q(rmsg, pBossPC->m_desc);
		if (pTargetPC)		SEND_Q(rmsg, pTargetPC->m_desc);
	}
}

//초대 수락
void ProcHelperExped_AllowRep(CNetMsg::SP& msg)
{
	int			nBossIndex;
	int			nTargetIndex,nTargetGroup,nTargetMember,nTargetListIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);
	int			nErrorCode;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex
		>> strTargetName
		>> nTargetGroup
		>> nTargetMember
	    >> nTargetListIndex
		>> nErrorCode;

	GAMELOG << init("EXPED DEBUG HELPER INVITE ALLOW REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "strTargetName" << delim << strTargetName << delim
			<< "nTargetGroup" << delim << nTargetGroup << delim
			<< "nTargetMember" << delim << nTargetMember << delim
			<< "nTargetListIndex" << delim << nTargetListIndex << delim
			<< "nErrorCode" << delim << nErrorCode << delim
			<< end;

	CPC* pTargetPC = PCManager::instance()->getPlayerByCharIndex(nTargetIndex);
	if( !pTargetPC )
	{
		return;
	}

	if( pTargetPC->IsParty() || pTargetPC->IsExped() )
	{
		return;
	}

	CExpedition* pExped = gserver->FindExpedByBossIndex(nBossIndex);
	if (pExped == NULL )
	{
		return ;
	}

	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*)pExped->JoinRequest(strTargetName, nTargetMember, pTargetPC);
	if (pMember==NULL)
	{
		return;
	}

	int i,j;

	if(nErrorCode == MSG_EXPED_ERROR_ALLOW_OK)
	{
		//새로운 타겟 => 원정대원
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedAddMsg(rmsg, nTargetIndex, strTargetName, nTargetGroup, nTargetMember, nTargetListIndex, pTargetPC);
		CNetMsg::SP addmsg(new CNetMsg);
		ExpedAddSysMsg(addmsg, strTargetName);
		for ( i = 0; i < MAX_EXPED_GROUP; i++)
		{
			for( j=0; j < MAX_EXPED_GMEMBER; j++)
			{
				const CExpedMember* pMember = pExped->GetMemberByListIndex(i,j);
				if (pMember)
				{
					if (pMember->GetMemberPCPtr())
					{
						SEND_Q(rmsg, pMember->GetMemberPCPtr()->m_desc);
						SEND_Q(addmsg, pMember->GetMemberPCPtr()->m_desc);
					}
				}
			}
		}

		//원정대원 정보  => 새로운 타겟
		for ( i = 0; i < MAX_EXPED_GROUP; i++)
		{
			for( j=0; j < MAX_EXPED_GMEMBER; j++)
			{
				const CExpedMember* pMember = pExped->GetMemberByListIndex(i,j);
				if (pMember)
				{
					// target에게는 타인을 추가
					CNetMsg::SP rmsg(new CNetMsg);
					ExpedAddMsg(rmsg, pMember->GetCharIndex(), pMember->GetCharName(), pMember->GetGroupType(), pMember->GetMemberType(),pMember->GetListIndex(), pMember->GetMemberPCPtr());
					SEND_Q(rmsg, pTargetPC->m_desc);
				}
			}
		}
#ifdef BUGFIX_ALLOW_EXPED_TYPE_SET
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExpedChangeType(rmsg, 0, pExped->GetExpedType(0));
			SEND_Q(rmsg, pTargetPC->m_desc);
		}
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExpedChangeType(rmsg, 1, pExped->GetExpedType(1));
			SEND_Q(rmsg, pTargetPC->m_desc);
		}
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExpedChangeType(rmsg, 2, pExped->GetExpedType(2));
			SEND_Q(rmsg, pTargetPC->m_desc);
		}
#endif
	}
}

//초대 거절
void ProcHelperExped_RejectRep(CNetMsg::SP& msg)
{
	int			nBossIndex;
	int			nTargetIndex;
	int			nErrorCode;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex
		>> nErrorCode;

	GAMELOG << init("EXPED DEBUG HELPER INVITE REJECT REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "nErrorCode" << delim << nErrorCode
			<< end;

	CExpedition* pExped = gserver->FindExpedByBossIndex(nBossIndex);
	if (!pExped)
	{
		return ;
	}
	if (pExped->GetRequestIndex() < 1)
	{
		return ;
	}

	CExpedMember	*member= pExped->GetMemberByCharIndex( pExped->GetBossIndex() );
	if( !member )
	{
		return;
	}
	CPC* pBossPC = member->GetMemberPCPtr();
	CPC* pRequestPC = PCManager::instance()->getPlayerByCharIndex(pExped->GetRequestIndex());

	if(nErrorCode == MSG_EXPED_ERROR_REJECT_OK)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (pExped->GetRequestIndex() == nTargetIndex)
				ExpedMsg(rmsg, MSG_REJECT_DEST);
			else if (pExped->GetBossIndex() == nTargetIndex)
				ExpedMsg(rmsg, MSG_REJECT_SRC);
			else
			{
				return ;
			}

			if (pBossPC)
			{
				SEND_Q(rmsg, pBossPC->m_desc);
			}

			if (pRequestPC)
			{
				SEND_Q(rmsg, pRequestPC->m_desc);
				pRequestPC->m_Exped = NULL;
			}
		}

		pExped->SetRequest(-1, "");
		if (pExped->GetMemberCount() < 2)
		{
			// 원정대 종료
			gserver->m_listExped.erase(pExped->GetBossIndex());
			pExped->SetEndExped();
			delete pExped;
		}
	}
}

//탈퇴(나감)
void ProcHelperExped_QuitRep(CNetMsg::SP& msg)
{
	int			nBossIndex;
	int			nTargetIndex;
	int			nErrorCode;
	int			nQuitMode;			// 정상,비정상 구분
	CLCString	strOldBossName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex
		>> nQuitMode
		>> nErrorCode;

	GAMELOG << init("EXPED DEBUG HELPER QUIT REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "nQuitMode" << delim << nQuitMode << delim
			<< "nErrorCode" << delim << nErrorCode
			<< end;

	CExpedition* pExped = gserver->FindExpedByBossIndex(nBossIndex);
	if (!pExped)
		return;

	if (nErrorCode == MSG_EXPED_ERROR_QUIT_END)
	{
		// 원정대 종료
		gserver->m_listExped.erase(pExped->GetBossIndex());
		pExped->SetEndExped();
		delete pExped;
	}
	else
	{
		{
			// 원정대원 나감
			CNetMsg::SP rmsg(new CNetMsg);
			ExpedQuitRepMsg(rmsg, nTargetIndex, nQuitMode);
			pExped->SendToAllPC(rmsg);
		}

		if(nQuitMode == MSG_EXPED_QUITMODE_NORMAL)
		{
			pExped->SetMemberPCPtr(nTargetIndex, NULL);
			pExped->DeleteMember(nTargetIndex);
		}

		// 보스 변경 알림(보스 변경 처리:자동 위임 처리 필요)
		if (nBossIndex == nTargetIndex)
		{
			//나가는 캐릭터가 보스이면 위임할 보스를 획득 함
			CExpedMember* pNewBossMember = (CExpedMember*) pExped->FindNextBoss();

			if(pNewBossMember)
			{
				pNewBossMember->SetMemberType(MSG_EXPED_MEMBERTYPE_BOSS);
				gserver->m_listExped.erase(nBossIndex);
				gserver->m_listExped.insert(map_listexped_t::value_type(pNewBossMember->GetCharIndex(), pExped));

				if( pExped->m_nRaidAreaNum > -1 )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperRaidInzoneSetNo( rmsg , pNewBossMember->GetCharIndex() , nBossIndex, pExped->m_nRaidZoneNum, MSG_JOINTYPE_EXPED, pExped->m_nDifficulty );
					SEND_Q(rmsg, gserver->m_helper);
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExpedChangeBoss(rmsg, nBossIndex , pNewBossMember->GetCharIndex(),MSG_EXPED_CHANGEBOSS_AUTO);
					pExped->SendToAllPC(rmsg);
				}
			}
		}
	}
}

//강퇴
void ProcHelperExped_KickRep(CNetMsg::SP& msg)
{
	int	nBossIndex;
	int	nTargetIndex;
	int	nErrorCode;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex
		>> nErrorCode;

	GAMELOG << init("EXPED DEBUG HELPER KICK REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nTargetIndex" << delim << nTargetIndex << delim
			<< "nErrorCode" << delim << nErrorCode
			<< end;

	CExpedition* pExped = gserver->FindExpedByBossIndex(nBossIndex);
	if (!pExped)
	{
		return ;
	}

	CPC* pTargetPC = PCManager::instance()->getPlayerByCharIndex(nTargetIndex);
	if (pTargetPC && pTargetPC->m_Exped && pTargetPC->m_Exped == pExped)
	{
		pTargetPC->CancelDamageLink();
		pTargetPC->m_Exped = NULL;

		// 인존에 있는 PC 파티 탈퇴하면 인존을 나감(멤버)
		if(pTargetPC->m_nJoinInzone_ZoneNo >=0 && pTargetPC->m_nJoinInzone_RoomNo >= 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidInzoneQuitReq(rmsg,1,0);
			do_Raid(pTargetPC, rmsg);
		}
	}

	pExped->SetMemberPCPtr(nTargetIndex, NULL);
	pExped->DeleteMember(nTargetIndex);
	if(pTargetPC)
		pTargetPC->SetExpedLabel(-1); // 라벨을 초기화 해준다.

	bool bAllMsgSend= true;

	if (nErrorCode == MSG_EXPED_ERROR_KICK_END)
	{
		// 원정대 종료
		gserver->m_listExped.erase(pExped->GetBossIndex());
		pExped->SetEndExped();
		delete pExped;
		bAllMsgSend= false;
	}

	{
		// 파티원 나감
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedKickMsg(rmsg, nTargetIndex);

		if (pTargetPC)
			SEND_Q(rmsg, pTargetPC->m_desc);

		if(	bAllMsgSend )
			pExped->SendToAllPC(rmsg, nTargetIndex);
	}
}

//대장 위임
void ProcHelperExped_ChangeBossRep(CNetMsg::SP& msg)
{
	int	nBossIndex;
	int	nNewBossIndex,nChangeMode;

	RefMsg(msg) >> nBossIndex
		>> nNewBossIndex
		>> nChangeMode;

	GAMELOG << init("EXPED DEBUG HELPER CHANGEBOSS REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nNewBossIndex" << delim << nNewBossIndex << delim
			<< "nChangeMode" << delim << nChangeMode
			<< end;

	CExpedition* pExped = gserver->FindExpedByBossIndex(nBossIndex);
	if(!pExped)
	{
		return ;
	}

	pExped->ChangeBoss(nNewBossIndex,nBossIndex);

	if( pExped->m_nRaidAreaNum > -1 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperRaidInzoneSetNo( rmsg , nNewBossIndex , nBossIndex,  pExped->m_nRaidZoneNum, MSG_JOINTYPE_EXPED, pExped->m_nDifficulty );
		SEND_Q(rmsg, gserver->m_helper);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedChangeBoss(rmsg, nBossIndex, nNewBossIndex, nChangeMode);
		pExped->SendToAllPC(rmsg);
	}
}

//타입변경
void ProcHelperExped_ChangeTypeRep(CNetMsg::SP& msg)
{
	int nBossIndex;
	char cDiviType,cExpedType;
	RefMsg(msg) >> nBossIndex
		>> cExpedType
		>> cDiviType;

	GAMELOG << init("EXPED DEBUG HELPER CHANGETYPE REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "cExpedType" << delim << cExpedType << delim
			<< "cDiviType" << delim << cDiviType
			<< end;

	CExpedition* pExped = gserver->FindExpedByBossIndex(nBossIndex);
	if (!pExped)
	{
		return ;
	}

//	pExped->SetExpedType(cDiviType,cExpedType);
	pExped->SetExpedType(cExpedType,cDiviType);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedChangeType(rmsg, cExpedType, cDiviType);
		pExped->SendToAllPC(rmsg);
	}
}

//부대장 임명
void ProcHelperExped_SetMBossRep(CNetMsg::SP& msg)
{
	int	nBossIndex, nNewMBossIndex;

	RefMsg(msg) >> nBossIndex
		>> nNewMBossIndex;

	GAMELOG << init("EXPED DEBUG HELPER SET MBOSS REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nNewMBossIndex" << delim << nNewMBossIndex
			<< end;

	CExpedition* pExped = gserver->FindExpedByBossIndex(nBossIndex);
	if(!pExped)
		return ;

	CExpedMember* pMember = NULL;
	int nGroup, nListindex;
	nGroup = pExped->FindMemberGroupIndex(nNewMBossIndex);
	nListindex = pExped->FindMemberListIndex(nNewMBossIndex);

	if(nGroup < 0 || nListindex < 0 )
		return;

	pMember =  (CExpedMember*)pExped->GetMemberByListIndex(nGroup, nListindex);
	if(pMember)
	{
		//부대장 수 체크
		if(pExped->GetGroupMembertypeCount(pMember->GetGroupType(),MSG_EXPED_MEMBERTYPE_MBOSS) >= 1)
		{
			//에러:한 그룹에 부대장 수 초과
			return;
		}

		if (pExped->SetMBoss(nNewMBossIndex))
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExpedSetMbossMsg(rmsg, nNewMBossIndex);
			pExped->SendToAllPC(rmsg);
		}
	}
}

//부대장 해임
void ProcHelperExped_ResetMBossRep(CNetMsg::SP& msg)
{
	int	nBossIndex, nNewMBossIndex;

	RefMsg(msg) >> nBossIndex
		>> nNewMBossIndex;

	GAMELOG << init("EXPED DEBUG HELPER RESET MBOSS REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nNewMBossIndex" << delim << nNewMBossIndex
			<< end;

	CExpedition* pExped = gserver->FindExpedByBossIndex(nBossIndex);
	if(!pExped)
		return ;

	CExpedMember* pMember = NULL;
	int nGroup, nListindex;
	nGroup = pExped->FindMemberGroupIndex(nNewMBossIndex);
	nListindex = pExped->FindMemberListIndex(nNewMBossIndex);

	if(nGroup < 0 || nListindex < 0 )
		return;

	pMember =  (CExpedMember*)pExped->GetMemberByListIndex(nGroup, nListindex);
	if(pMember)
	{
		if(pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_MBOSS)
		{
			//에러: 부대장아 아님
			return;
		}

		if (pExped->ResetMBoss(nNewMBossIndex))
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExpedResetMbossMsg(rmsg, nNewMBossIndex);
			pExped->SendToAllPC(rmsg);
		}
	}
}

//원정대 해체
void ProcHelperExped_EndExpedRep(CNetMsg::SP& msg)
{
	int	nBossIndex;

	RefMsg(msg) >> nBossIndex;

	GAMELOG << init("EXPED DEBUG HELPER ENDEXPED REP")
			<< "nBossIndex" << delim << nBossIndex
			<< end;

	CExpedition* pExped = gserver->FindExpedByBossIndex(nBossIndex);
	if (!pExped)
		return;

	//삭제
	gserver->m_listExped.erase(pExped->GetBossIndex());
	pExped->SetEndExped();
	delete pExped;
}

//그룹 이동
void ProcHelperExped_MoveGroupRep(CNetMsg::SP& msg)
{
	int nBossIndex, nSourceGroup, nMoveCharIndex, nTargetGroup, nTargetListindex;

	RefMsg(msg) >> nBossIndex
		>> nSourceGroup
		>> nMoveCharIndex
		>> nTargetGroup
		>> nTargetListindex;

	GAMELOG << init("EXPED DEBUG HELPER MOVEGROUP REP")
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nSourceGroup" << delim << nSourceGroup << delim
			<< "nMoveCharIndex" << delim << nMoveCharIndex << delim
			<< "nTargetGroup" << delim << nTargetGroup << delim
			<< "nTargetListindex" << delim << nTargetListindex << delim
			<< end;

	CExpedition* pExped = gserver->FindExpedByBossIndex( nBossIndex);
	if (!pExped)
		return ;

	if(pExped->MoveGroup(nSourceGroup,nMoveCharIndex,nTargetGroup,nTargetListindex))
	{
		//성공
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedMoveGroupRepMsg(rmsg, nSourceGroup, nMoveCharIndex, nTargetGroup, nTargetListindex);
		pExped->SendToAllPC(rmsg, -1);
	}
}


//에러
void ProcHelperRaidErrorRep(CNetMsg::SP& msg)
{
	unsigned char errorType;
	int nCharIndex = -1;
	int nErrData1 = -1;
	int nErrData2 = -1;

	RefMsg(msg) >> errorType
		>> nCharIndex
		>> nErrData1
		>> nErrData2;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(nCharIndex);

	if(!ch)	return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		RaidErrorMsg(rmsg, errorType, nErrData1, nErrData2);
		SEND_Q(rmsg, ch->m_desc);
	}
}

//룸번호 획득 후 처리(개별적 처리)
void ProcHelperInzoneGetRoomnoRep( CNetMsg::SP& msg)
{
	int nCharIndex, nZoneNo, nRoomNo, nBossIndex, nBossRoomNo;

	RefMsg(msg) >> nCharIndex
		>> nZoneNo
		>> nRoomNo
		>> nBossIndex	  //파티 원정대 보스 인덱스
		>> nBossRoomNo;
	int nDifficulty = -1;
	RefMsg(msg) >> nDifficulty;
	GAMELOG << init("RAID DEBUG HELPER GET INZONE ROOMNO REP")
			<< "nCharIndex" << delim << nCharIndex << delim
			<< "nZoneNo" << delim << nZoneNo << delim
			<< "nRoomNo" << delim << nRoomNo << delim
			<< "nBossIndex" << delim << nBossIndex << delim
			<< "nDifficulty" << delim << nDifficulty << delim
			<< end;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(nCharIndex);
	if(!ch)
		return;

	if(nZoneNo < 0)
		return;

	CZone* pZone=NULL;
	//int nCurrentZoneNo = ch->m_nJoinInzone_ZoneNo; 34번존에서 33번존으로 이동 시 입구로 이동시킴.
	int nCurrentZoneNo = ch->m_pZone->m_index;

	if((nZoneNo==ZONE_ALTER_OF_DARK || nZoneNo==ZONE_AKAN_TEMPLE
			|| nZoneNo == ZONE_TARIAN_DUNGEON
		) && nRoomNo != nBossRoomNo)
	{
		//에러: 귀속된 인존 정보가 원정대장 달라 접속 불가
		CNetMsg::SP rmsg(new CNetMsg);
		RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_NOTSAMEBOSSROOMNO);
		SEND_Q(rmsg, ch->m_desc);
		goto ERROR_RETURN;
	}

	pZone = gserver->FindZone(nZoneNo);

	if(pZone && pZone->m_area && pZone->m_zonePos)
	{
		CArea* area = NULL;

		// 원정대를 찾는다.
		if (ch->IsExped())
		{
			// 들어가려는 존의 정보가 기존 레이드 정보의 존이 아니면 에러메시지를 출력
			if(ch->m_Exped->m_nRaidZoneNum != -1 && ch->m_Exped->m_nRaidZoneNum != nZoneNo)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_NOTSAMEBOSSROOMNO);
				SEND_Q(rmsg, ch->m_desc);
				goto ERROR_RETURN;
			}

			// 다른 맴버가 이미 들어가 있으면 따라 간다.
			if(ch->m_Exped->m_nRaidAreaNum != -1)
			{
				area = pZone->m_area + ch->m_Exped->m_nRaidAreaNum;
				if(area->m_nRaidRoomNo != nRoomNo)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_NOTSAMEBOSSROOMNO);
					SEND_Q(rmsg, ch->m_desc);
					goto ERROR_RETURN;
				}
			}
		}

		// 파티 레이드이면 파티를 찾는다.
		else if(ch->IsParty())
		{
			// 다른 맴버가 이미 들어가 있으면 따라 간다.
			if(ch->m_party->m_nRaidAreaNum != -1)
				area = pZone->m_area + ch->m_party->m_nRaidAreaNum;
		}

		// 파티와 레이드 중 아무것도 아니면 에러 메세지
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_NOTJOIN_PARTYOREXPED);
			SEND_Q(rmsg, ch->m_desc);
			goto ERROR_RETURN;
		}

		// area에 값을 못 넣었으면, 처음 입장하는 것이므로 빈 area로 간다.
		if(!area)
		{
			int a = pZone->FindEmptyArea();
			if(a == -1)
			{
				//에러: 현재 인스턴스 너무 많아 더이상 생성 불가
				CNetMsg::SP rmsg(new CNetMsg);
				RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_ROOMCOUNT_FULL);
				SEND_Q(rmsg, ch->m_desc);
				goto ERROR_RETURN;
			}

			area = pZone->m_area + a;
			if(!area)
				goto ERROR_RETURN;

			// 다른 맴버들을 위해 area 넘버를 저장해둔다.
			if(ch->IsParty())
			{
				ch->m_party->m_nRaidZoneNum = nZoneNo;
				ch->m_party->m_nRaidAreaNum = a;
			}
			else if(ch->IsExped())
			{
				ch->m_Exped->m_nRaidAreaNum = a;
			}
		}

		if(ch->IsExped() && ch->m_index == nBossIndex)
		{
			ch->m_Exped->m_nRaidZoneNum = nZoneNo;
			ch->m_Exped->m_nDifficulty = nDifficulty;
		}

		int extra = 0;

		if(ch->IsExped() && pZone->IsExpedRaidZone())
		{
			//현재 들어온 존, 룸번호 획득
			ch->m_nJoinInzone_ZoneNo = nZoneNo;
			ch->m_nJoinInzone_RoomNo = nRoomNo;
			ch->m_nJoinInzone_AreaNo = area->m_index;

			if( (area->m_zone->m_index == ZONE_AKAN_TEMPLE
					|| area->m_zone->m_index == ZONE_TARIAN_DUNGEON
				)
				&& area->GetPCCount() >= 8)		
			{
				CNetMsg::SP rmsg(new CNetMsg);
				RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_MEMBER_FULL);
				SEND_Q(rmsg, ch->m_desc);
				goto ERROR_RETURN;
			}

			GoZone(ch,
				nZoneNo,
				pZone->m_zonePos[extra][0],
				GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,
				GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);

			GAMELOG << init("RAID DEBUG HELPER GET INZONE ROOMNO REP  - STEP 1")
					<< "JOIN INZONE SUCESS"
					<< end;
		}

		// 원정대 레이드 존이 아니면 파티 레이드이다.
		else if(ch->IsParty() && pZone->IsPartyRaidZone())
		{
			if(nCurrentZoneNo == ZONE_CAPPELLA_2 && nZoneNo == ZONE_CAPPELLA_1)
			{
				extra = 2;
			}

			//현재 들어온 존, 룸번호 획득
			ch->m_nJoinInzone_ZoneNo = nZoneNo;
			ch->m_nJoinInzone_RoomNo = nRoomNo;
			ch->m_nJoinInzone_AreaNo = area->m_index;

			GoZone(ch,
				nZoneNo,
				pZone->m_zonePos[extra][0],
				GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,
				GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);

			GAMELOG << init("RAID DEBUG HELPER GET INZONE ROOMNO REP  - STEP 1")
					<< "JOIN INZONE SUCESS"
					<< end;
		}

		else
		{
			//에러
			GAMELOG << init("RAID DEBUG HELPER GET INZONE ROOMNO REP  - STEP 2")
					<< "JOIN INZONE FAIL"
					<< end;
			goto ERROR_RETURN;
		}
	} // if

ERROR_RETURN:
	ch->ResetPlayerState(PLAYER_STATE_WARP);
	return;
}


void ProcHelperDeleteRaidCharacterRep(CNetMsg::SP& msg)
{
	int nCharIndex = 0, nSuccess = 0;
	RefMsg(msg) >> nCharIndex
		>> nSuccess;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(nCharIndex);
	if(!ch || !ch->m_desc)
		return;

	// DB 삭제 실패 시
	if (nSuccess == 0)
		return;

	// DB 삭제 성공 시
	else if (nSuccess == 1)
	{
		{
			// 귀속된 레이드 존 정보 창 비어있는 것으로 보여준다.
			CNetMsg::SP rmsg(new CNetMsg);
			RaidInfoMsg(rmsg, 0, NULL, NULL);
			SEND_Q(rmsg, ch->m_desc);
		}

		// 레이드 초기화 아이템을 삭제한다.
		CItem* item = ch->m_inventory.FindByDBIndex(4911, 0, 0);
		if( item )
		{
			ch->m_inventory.decreaseItemCount(item, 1);
		}
	}

	return;
}

void ProcHelperRaidInfoRep(CNetMsg::SP& msg)
{
	int nCharIndex = 0, nRaidCount = 0;
	RefMsg(msg) >> nCharIndex
		>> nRaidCount;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(nCharIndex);
	if(!ch || !ch->m_desc)
		return;

	int* nZoneNum = new int[nRaidCount];
	int* nRoomNum = new int[nRaidCount];

	for (int i=0; i < nRaidCount; i++)
	{
		RefMsg(msg) >> nZoneNum[i]
			>> nRoomNum[i];
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		if (nRaidCount >= 1)
			RaidInfoMsg(rmsg, nRaidCount, nZoneNum, nRoomNum);
		else
			RaidInfoMsg(rmsg, nRaidCount, NULL, NULL);
		SEND_Q(rmsg, ch->m_desc);
	}

	nZoneNum = NULL;
	nRoomNum = NULL;
	return;
}

void ProcHelperNSCreateCard(CNetMsg::SP& msg)
{
	CDescriptor * desc;
	unsigned char subtype;
	int userindex, charindex, itemindex;

	RefMsg(msg) >> subtype
		>> userindex
		>> charindex;

	desc = DescManager::instance()->getDescByUserIndex(userindex);
	if(!desc)
	{
		if(subtype == MSG_HELPER_NS_CARD_ERROR_USE_OK)
		{
			// 아이템 사용 취소 메시지 보내기
			GAMELOG << init("NS_CREATE_CARD_ERROR DESC NOTFOUND")
					<< "USERINDEX" << delim
					<< userindex << delim
					<< "CHARINDEX" << delim
					<< charindex
					<< end;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperNSCreateCardUse(rmsg, MSG_HELPER_NS_CARD_USE_CANCEL, userindex, charindex);
				SEND_Q(rmsg, gserver->m_helper);
			}
		}
		return ;
	}

	switch(subtype)
	{
	case MSG_HELPER_NS_CARD_ERROR_USE_OK:
		{
			RefMsg(msg) >> itemindex;

			CItem* item;
			int row, col;
			CPC* pc = desc->m_pChar;
			if(!pc)
			{
				// 아이템 사용 취소 메시지 보내기
				GAMELOG << init("NS_CREATE_CARD_ERROR PC NOTFOUND")
						<< "USERINDEX" << delim
						<< userindex << delim
						<< "CHARINDEX" << delim
						<< charindex
						<< end;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperNSCreateCardUse(rmsg, MSG_HELPER_NS_CARD_USE_CANCEL, userindex, charindex);
					SEND_Q(rmsg, gserver->m_helper);
				}
				return ;
			}

			// 인벤에서 아이템 하나 제거시키기
			item = pc->m_inventory.FindByVirtualIndex(itemindex);
			if (item == NULL)
			{
				GAMELOG << init("NS_CREATE_CARD_ERROR ITEM NOTFOUND")
						<< "USERINDEX" << delim
						<< userindex << delim
						<< "CHARINDEX" << delim
						<< charindex
						<< end;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperNSCreateCardUse(rmsg, MSG_HELPER_NS_CARD_USE_CANCEL, userindex, charindex);
					SEND_Q(rmsg, gserver->m_helper);
				}
				return ;
			}

			pc->m_inventory.decreaseItemCount(item, 1);

			desc->m_bIsNSCreateCardUse = true;

			{
				// 클라이언트로 메시지 보내기
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_NS_CARD_USE_OK);
				SEND_Q(rmsg, pc->m_desc);
			}
		}
		break;
	case MSG_HELPER_NS_CARD_ERROR_CREATED_NS:
	case MSG_HELPER_NS_CARD_ERROR_90LV:
	case MSG_HELPER_NS_CARD_ERROR_ALREADY:
	case MSG_HELPER_NS_CARD_ERROR_ETC:
		{
			CPC* pc = desc->m_pChar;
			if(pc)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_NS_CARD_INSUFF);
				SEND_Q(rmsg, pc->m_desc);
			}
		}
		break;
	default:
		break;
	}
}

#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE)
void ProcHelperWorldcup2010(CNetMsg::SP& msg)
{
	unsigned char nSubType;
	unsigned char nErrType;
	int charIndex;
	RefMsg(msg) >> nSubType >> nErrType >> charIndex;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(charIndex);
	if( ch == NULL)
		return;

	CNetMsg::SP rmsg(new CNetMsg);

	switch(nSubType)
	{
	case MSG_HELPER_WORLDCUP2010_TOTO_REP:
		switch(nErrType)
		{
		case MSG_HELPER_WORLDCUP2010_TOTO_ERROR_SUC:
			{
				int itemidx;
				RefMsg(msg) >> itemidx;

				EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_REP, MSG_EVENT_WORLDCUP2010_TOTO_ERROR_SUC);
			}
			break;
		case MSG_HELPER_WORLDCUP2010_TOTO_ERROR_FAIL:
			{
				// DB 기록이 실패했당 지운 국기를 다시 지급하자
				int Countryitemidx;
				RefMsg(msg) >> Countryitemidx;

				CItem * pItem = gserver->m_itemProtoList.CreateItem(Countryitemidx, -1, 0, 0, 1);

				if( pItem )
				{
					if( ch->m_inventory.addItem(pItem) == false)
					{	// 쓰바 지웠던 국기 지급이 안되었넹. 이를 어쩐다... ㅡㅡ;;
						// 로고는 찍고 FAIL을 날린다.
						GAMELOG << init("EVENT_WORLDCUP_2010 : TOTO ERROR", ch)
							<< itemlog(pItem)
							<< end;
					}
				}
			}
			EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_REP, MSG_EVENT_WORLDCUP2010_TOTO_ERROR_FAIL);
			break;
		}
		break;

	case MSG_HELPER_WORLDCUP2010_TOTO_STATUS_REP:
		{
			int resulttype=-1, itemindex;
			RefMsg(msg) >> resulttype >> itemindex;
			switch(nErrType)
			{
			case MSG_HELPER_WORLDCUP2010_TOTO_STATUS_ERROR_SUC:
				EventWorldcup2010TOTOStatusErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_STATUS_ERROR_SUC, resulttype, itemindex);
				break;
			case MSG_HELPER_WORLDCUP2010_TOTO_STATUS_ERROR_FAIL:
				EventWorldcup2010TOTOStatusErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_STATUS_ERROR_FAIL, resulttype);
				break;
			}
		}
		break;

	case MSG_HELPER_WORLDCUP2010_TOTO_GIFT_REP:
		switch(nErrType)
		{
		case MSG_HELPER_WORLDCUP2010_TOTO_GIFT_ERROR_SUC: // 선물만 주고 끝~
			{
				int nTableCount = 7;
				int giftTable[7][2]=
				{	// item_idx,  count
					{ 974, 1}, // 1. 행운의 제련석
					{4968, 1}, // 2. (이벤트용)악마의 룬 아르
					{2849, 1}, // 3. 힘   상승 물약(소)
					{2850, 1}, // 4. 민첩 상승 물약(소)
					{2851, 1}, // 5, 체질 상승 물약(소)
					{2852, 1}, // 6, 지혜 상승 물약(소)
					{2853, 1}  // 7, 버서커 포션(소)
				};
				if( ch->m_inventory.getEmptyCount() < nTableCount ) // 인벤 검사
				{
					HelperWorldcup2010TOTOGiftRepMsg(rmsg, ch->m_index, 1);
					SEND_Q(rmsg, gserver->m_helper);
					EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_REP, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_ERROR_INVEN);
					break;
				}

				CItem ** pItem = NULL;
				pItem = new CItem*[nTableCount];

				int i=0;
				for(i=0; i<nTableCount; i++) // 지급항 아이템을 다 생성하자.
				{
					pItem[i] = NULL;
					pItem[i] = gserver->m_itemProtoList.CreateItem( giftTable[i][0], -1, 0, 0, giftTable[i][1]);
					if( pItem[i] == NULL )
					{
						int j;
						for( j=0; j<i; j++ )
						{
							if( pItem[j] )
							{
								delete [] pItem[j];
								pItem[j] = NULL;
							}
						}
						delete [] pItem;

						EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_REP, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_ERROR_FAIL);
						HelperWorldcup2010TOTOGiftRepMsg(msg, ch->m_index, 1);
						break;
					}
				}

				for(i=0; i<nTableCount; i++)
				{
					if( ch->m_inventory.addItem(pItem[i]) == false )
					{
						GAMELOG << init("EVENT_WORLDCUP_2010 : TOTO GIFT ADD ERROR", ch)
							<< itemlog(pItem[i])
							<< end;

						int j=0;
						for(j=0; j<i; j++)
						{
							GAMELOG << init("EVENT_WORLDCUP_2010 : TOTO GIFT DEL", ch)
								<< itemlog(pItem[j])
								<< end;

							ch->m_inventory.decreaseItemCount(pItem[j], 1);
							delete pItem[j];
							pItem[j] = NULL;
						}

						delete [] pItem;

						HelperWorldcup2010TOTOGiftRepMsg(rmsg, ch->m_index, 1);
						SEND_Q(rmsg, gserver->m_helper);
						EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_REP, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_ERROR_FAIL);
						break;
					}
				}

				EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_REP, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_ERROR_SUC);
			}
			break;

		case MSG_HELPER_WORLDCUP2010_TOTO_GIFT_ERROR_FAIL:
			EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_REP, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_ERROR_FAIL);
			break;
		case MSG_HELPER_WORLDCUP2010_TOTO_GIFT_ERROR_AREADY:
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_ALREADY_GIVE);
			break;
		}
		break;

	case MSG_HELPER_WORLDCUP2010_ATTENDANCE_REP:
		switch(nErrType)
		{
		case MSG_HELPER_WORLDCUP2010_ATTENDANCE_ERROR_SUC: // 응원 카드를 지급하자
			{
				int supportIndex = 19;

				CItem * pItem = gserver->m_itemProtoList.CreateItem( supportIndex, -1, 0, 0, 1);
				if( pItem == NULL )
				{
					EventWorldcup2010ErrorMsg(rmsg,MSG_EVENT_WORLDCUP2010_TOTO_GIFT_REP,  MSG_EVENT_WORLDCUP2010_TOTO_GIFT_ERROR_FAIL);
					SEND_Q(rmsg, ch->m_desc);
					break;
				}

				// 인벤 검사
				if ( ch->m_inventory.getEmptyCount() < 1 )
				{
					EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_REP, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_ERROR_INVEN);
					break;
				}
				if( ch->m_inventory.addItem(pItem) == false )
				{
					EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_REP, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_ERROR_FAIL);
					break;
				}
			}
			EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_ATTENDANCE_REP, MSG_EVENT_WORLDCUP2010_ATTENDANCE_ERROR_SUC);
			break;
		case MSG_HELPER_WORLDCUP2010_ATTENDANCE_ERROR_ALREADY:
			EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_ATTENDANCE_REP, MSG_EVENT_WORLDCUP2010_ATTENDANCE_ERROR_ALREADY);
			break;
		case MSG_HELPER_WORLDCUP2010_ATTENDANCE_ERROR_FAIL:
			EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_ATTENDANCE_REP, MSG_EVENT_WORLDCUP2010_ATTENDANCE_ERROR_FAIL);
			break;
		}
		break;

	case MSG_HELPER_WORLDCUP2010_KOREA_REP:
		{
			int item[7][5] = {
				{5819, 5820, 5821, 5822, 5823},		// 타이탄
				{5824, 5825, 5826, 5827, 5828},		// 나이트
				{5804, 5805, 5806, 5807, 5808},		// 힐러
				{5809, 5810, 5811, 5812, 5813},		// 메이지
				{5814, 5815, 5816, 5817, 5818},		// 로그
				{5829, 5830, 5831, 5832, 5833},		// 소서러
				{5834, 5835, 5836, 5837, 5838}		// 나이트쉐도우
			};
			int i, j;

			switch(nErrType)
			{
			case MSG_HELPER_WORLDCUP2010_KOREA_ERROR_EMPTY:
				{
					// 일반 슬롯 체크
					if( ch->m_inventory.getEmptyCount() < 5)
					{
						EventErrorMsg(rmsg, MSG_EVENT_ERROR_FULLINVENTORY);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					HelperWorldcup2010KoreaRepMsg(rmsg, ch->m_index, 1);	// 지급 완료 요청
					SEND_Q(rmsg, gserver->m_helper);
					return;
				}
				break;
			case MSG_HELPER_WORLDCUP2010_KOREA_ERROR_SUC:
				{
					CItem* pItem[5];

					for( i = 0; i < 5; i++ ) // 지급할 아이템 모두 생성
					{
						pItem[i] = gserver->m_itemProtoList.CreateItem(item[ch->m_job][i], -1, 0, 0, 1);
						if( pItem[i] == NULL )
						{
							for( j = 0; j < i; j++ )
							{
								if( pItem[j] )
								{
									delete pItem[j];
									pItem[j] = NULL;
								}
							}

							EventErrorMsg(rmsg, MSG_EVENT_ERROR_INSUFF_CONDITION);
							SEND_Q(rmsg, ch->m_desc);
							return;
						}
					}

					for( i = 0; i < 5; i++ )
					{
						if( ch->m_inventory.addItem(pItem[i]) == false)
						{
							GAMELOG << init("EVENT_WORLDCUP_2010 : KOREA 16 GIFT ADD ERROR", ch)
								<< itemlog(pItem[i])
								<< end;
							
							HelperWorldcup2010KoreaRepMsg(rmsg, ch->m_index, 2);	// 지급 실패 요청
							SEND_Q(rmsg, gserver->m_helper);
							return;
						}
						else
						{
							GAMELOG << init("EVENT_WORLDCUP_2010 : KOREA 16 GIFT ADD", ch)
								<< itemlog(pItem[i])
								<< end;
						}
					}

					EventWorldcup2010KoreaErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_KOREA_ERROR_SUC);
				}
				break;
			case MSG_HELPER_WORLDCUP2010_KOREA_ERROR_ALREADY:
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_ALREADY_GIVE);
				break;
			case MSG_HELPER_WORLDCUP2010_KOREA_ERROR_FAIL:
				EventErrorMsg(rmsg, MSG_EVENT_ERROR_INSUFF_CONDITION);
				break;
			} break;
		}
	}
	SEND_Q(rmsg, ch->m_desc);
}
#endif // defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE)


void ProcHelperTeachRenewer(CNetMsg::SP& msg)
{
	unsigned char subtype;
	RefMsg(msg) >> subtype;

	switch( subtype )
	{
	case MSG_HELPER_TEACH_RENEWER_TIME_LIMIT_REP:
		ProcHelperTeachRenewerTimeLimit(msg);
		break;
	case MSG_HELPER_TEACH_RENEWER_TEACHER_GIFT_REP:
		ProcHelperTeachRenewerGift(msg);
		break;
	}
}

void ProcHelperTeachRenewerTimeLimit(CNetMsg::SP& msg)
{
	int teacherIDX;
	int cnt, i;
	int studentIDX;

	RefMsg(msg) >> teacherIDX
		>> cnt;

	CPC * ch = PCManager::instance()->getPlayerByCharIndex(teacherIDX);
	if( ch == NULL )
		return;

	for(i=0; i<cnt; i++)
	{
		RefMsg(msg) >> studentIDX;

		int j;
		for(j=0; j<TEACH_MAX_STUDENTS; j++)
		{
			if( ch->m_teachIdx[j] != -1 && studentIDX == ch->m_teachIdx[j] )
			{
				CPC * pStudentPc = PCManager::instance()->getPlayerByCharIndex(studentIDX);
				if( pStudentPc )
					continue;

				ch->m_cntFailStudent++;
				ch->m_cntTeachingStudent--;
				if( ch->m_cntTeachingStudent < 0)
					ch->m_cntTeachingStudent = 0;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					TeachEndMsg(rmsg, ch->m_index, ch->GetName(), ch->m_teachIdx[j], ch->m_teachName[j], MSG_TEACH_END_FAIL);
					RefMsg(rmsg) << ch->m_cntTeachingStudent
						<< ch->m_cntCompleteStudent
						<< ch->m_cntFailStudent;
					SEND_Q(rmsg, ch->m_desc);
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperTeachTimeover(rmsg, CANCELTEACHER | CANCELSTUDENT , ch->m_index, ch->m_teachIdx[j]);
					SEND_Q(rmsg, gserver->m_helper);
				}

				GAMELOG << init("TEACH_FAIL_TIMEOVER")
					<< "STU_INDEX"  << delim
					<< ch->m_teachIdx[j] << delim
					<< "STU_NAME" << delim
					<< ch->m_teachName[j] << delim
					<< ch->m_name << delim
					<< ch->m_nick << delim
					<< ch->m_desc->m_idname << delim
					<< ch->m_fame << end;

			// 혼자 셋팅해제
				ch->m_teachIdx[j] = -1;
				ch->m_teachJob[j] = -1;
				ch->m_teachJob2[j] = 0;
				ch->m_teachLevel[j] = 0;
				ch->m_teachName[j][0] = '\0';

				//0627
			}
		}

		// teachType 검사
		bool bTeacher = false;

		int i;
		for (i=0; i<TEACH_MAX_STUDENTS; i++)
		{
			if (ch->m_teachIdx[i] != -1)
			{
				bTeacher = true;
				break;
			}
		}
		if (!bTeacher)
			ch->m_teachType = MSG_TEACH_NO_TYPE;
	}
}

void ProcHelperTeachRenewerGift(CNetMsg::SP& msg)
{
	unsigned char errorType;
	int charindex;
	RefMsg(msg) >> errorType
		>> charindex;

	CPC * ch = PCManager::instance()->getPlayerByCharIndex(charindex);
	if( ch == NULL )
		return;

	switch( errorType )
	{
	case MSG_HELPER_TEACH_RENEWER_GIFT_ERROR:
	case MSG_HELPER_TEACH_RENEWER_GIFT_DB:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			TeachTeacherGiftMsg(rmsg, MSG_TEACH_GIFT_ERROR, ch);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case MSG_HELPER_TEACH_RENEWER_GIFT_NOMORE:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			TeachTeacherGiftMsg(rmsg, MSG_TEACH_GIFT_NOMORE, ch);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case MSG_HELPER_TEACH_RENEWER_GIFT_SUC:
		{
			int nCount;
			RefMsg(msg) >> nCount;
			int itemIndex = 5952; // 후견인의 증표
			if( false == ch->GiveItem(itemIndex, 0, 0, (LONGLONG)nCount, false, false) )
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperTeacherGiftAddReqMsg(rmsg, ch->m_index, nCount);
					SEND_Q(rmsg, gserver->m_helper);
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					TeachTeacherGiftMsg(rmsg, MSG_TEACH_GIFT_ERROR, ch);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				TeachTeacherGiftMsg(rmsg, MSG_TEACH_GIFT_SUC, ch, nCount);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;
	}
}

void ProcHelperFaceOff(CNetMsg::SP& msg)
{
	int charIndex;
	char hairStyle, faceStyle;
	unsigned char errorType;
	RefMsg(msg) >> errorType
		>> charIndex
		>> hairStyle
		>> faceStyle;

	switch( errorType )
	{
	case MSG_EX_FACEOFF_ERROR_SUC:
		GAMELOG << init("FACE OFF SUC - DB");
		break;
	case MSG_EX_FACEOFF_ERROR_FAIL:
		GAMELOG << init("FACE OFF FAIL - DB ");
		break;
	}
	GAMELOG << charIndex << delim << hairStyle << delim << faceStyle << end;
}

#ifdef EVENT_CHAR_BIRTHDAY
void ProcHelperCharBirthday(CNetMsg::SP& msg)
{
	unsigned char subtype;
	int charindex;;

	RefMsg(msg) >> subtype
		>> charindex;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);

	if( pc == NULL )
		return;

	switch( (int)subtype )
	{
	case MSG_EVENT_CHAR_BIRTHDAY_GIFT_REP:
		{
			unsigned char errortype;
			RefMsg(msg) >> errortype;
			switch( (MSG_EVENT_CHAR_BIRTHDAY_ERROR_TYPE)errortype )
			{
			case MSG_EVENT_CHAR_BIRTHDAY_ERROR_SUC:
				{
					int years;
					RefMsg(msg) >> years;
					if( pc->m_inventory.getEmptyCount() < 1 )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						SysFullInventoryMsg(rmsg, 0);
						SEND_Q(rmsg, pc->m_desc);
						return;
					}

					if( years > 10 ) // 10년이 넘어가면 10년 짜리 아이템으로 준다.
						years = 10;

					int giftindex= 6717 + years; // 보상품은 6718 ~ 6727 (1주년부터 10주년까지)
					if( !pc->GiveItem(giftindex, 0, 0, 1) )
					{
						GAMELOG << init("EVENT_CHAR_BIRTHDAY : ITEM GIVE FAIL", pc) << giftindex << end;
						CNetMsg::SP rmsg(new CNetMsg);
						EventCharBirthdayErrorMsg(rmsg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_FAIL );
						SEND_Q(rmsg, pc->m_desc);
						return;
					}

					{
						GAMELOG << init("EVENT_CHAR_BIRTHDAY : ITEM GIVE SUC", pc) << giftindex << end;
						CNetMsg::SP rmsg(new CNetMsg);
						EventCharBirthdayErrorMsg(rmsg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_SUC);
						SEND_Q(rmsg, pc->m_desc);
					}
				}
				break;

			case MSG_EVENT_CHAR_BIRTHDAY_ERROR_FAIL:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventCharBirthdayErrorMsg(rmsg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_FAIL);
					SEND_Q(rmsg, pc->m_desc);
				}
				break;
			}
		}
		break;

	case MSG_EVENT_CHAR_BIRTHDAY_BDAY_REP:
		{
			int yy;
			char mm, dd;

			RefMsg(msg) >> yy >> mm >> dd;

			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_EVENT);
			RefMsg(rmsg) << (unsigned char) MSG_EVENT_CHAR_BIRTHDAY
				 << (unsigned char) MSG_EVENT_CHAR_BIRTHDAY_BDAY_REP
				 << yy;

			if( yy > 0 )
			{
				RefMsg(rmsg) << mm << dd ;
			}
			SEND_Q(rmsg, pc->m_desc);
		}
		break;
	}
}
#endif
#ifdef DEV_GUILD_MARK
void ProcHelperGuildMarkExpire(CNetMsg::SP& msg)
{
	// helper에서 길드마크 시간을 계산하고 만료가 되었으면 게임서버가 받아서 클라이언트에게 뿌려준다.
	unsigned char command;
	int GuildIndex = 0;
	RefMsg(msg) >> command >> GuildIndex;
//	if(command == -1)
//		return ;
	if(GuildIndex == 0)
		return ;
	if(command == 0)
	{
		CGuild* pGuild = NULL;
		pGuild = gserver->m_guildlist.findguild(GuildIndex);
		if(pGuild)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			pGuild->SetGuildMark(-1, -1, -1, -1, -1);
			GuildMarkExpireMsg(rmsg);
			pGuild->SendToAll(rmsg, true);
		}
	}
}
void ProcHelperGuildMarkRegist(CNetMsg::SP& msg)
{
	int GuildIndex = 0;
	int CharIndex = 0;
	char command = -1;
	char gm_row = -1;
	char gm_col = -1;
	char bg_row = -1;
	char bg_col = -1;
	unsigned short tab = -1;
	unsigned short invenIndex = -1;
	int markTime = -1;
	CLCString serial;
	bool bRollback = false;

	RefMsg(msg) >> GuildIndex >> CharIndex >> command >> gm_row >> gm_col >> bg_row >> bg_col >> tab >> invenIndex >> markTime >> serial;

	CPC* pc = NULL;
	pc = PCManager::instance()->getPlayerByCharIndex(CharIndex);
	CGuild* pGuild = NULL;
	pGuild = gserver->m_guildlist.findguild(GuildIndex);

	switch((int)command)
	{
	case 0:
		{
			if(pGuild)
			{
				pGuild->SetGuildMark(gm_row, gm_col, bg_row, bg_col, markTime);
				if(pc)
				{
					CItem* pItem = pc->m_inventory.getItem(tab, invenIndex);
					if (pItem == NULL)
					{
						bRollback = true;
					}
					else if(pItem->m_serial != (const char *)serial)
					{
						bRollback = true;
					}

					else if(pItem->m_itemProto->getItemTypeIdx() == ITYPE_ETC && pItem->m_itemProto->getItemSubTypeIdx() == IETC_GUILD_MARK)
					{
						// 아이템이 있고, 찾았으니까..하나를 줄어들게 한다.
						pc->m_inventory.decreaseItemCount(pItem, 1);
					}
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					GuildMarkEditFinMsg(rmsg, pGuild->GetGuildMarkRow(), pGuild->GetGuildMarkCol(), pGuild->GetBackgroundRow(), pGuild->GetBackgroundCol(), pGuild->GetMarkTimeConvertGameTime());
					pGuild->SendToAll(rmsg, true);
				}

				if(bRollback == true)
				{
					// DB에 저장된 정보를 롤백 시킨다.
					CNetMsg::SP rmsg(new CNetMsg);
					HelperGuildMarkRegistReqMsg(rmsg, GuildIndex, CharIndex, -1, -1, -1, -1, -1, -1, -1, NULL);
					SEND_Q(rmsg, gserver->m_helper);
				}
			}
		}
		break;
	case 1:
		{
			if(pc)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
				SEND_Q(rmsg, pc->m_desc);
				return ;
			}
		}
		break;
	case 2: // MSG_NEW_GUILD_ERROR_SYSTEM
		{
			if(pc)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_SYSTEM);
				SEND_Q(rmsg, pc->m_desc);
				return ;
			}
		}
		break;
	case 3:
		{
			// 롤백 되었으니 모든 서버에 있는 길드원들에게 알려준다.
			if(pGuild)
			{
				pGuild->SetGuildMark(-1, -1, -1, -1, -1);
				CNetMsg::SP rmsg(new CNetMsg);
				GuildMarkEditFinMsg(rmsg, pGuild->GetGuildMarkRow(), pGuild->GetGuildMarkCol(), pGuild->GetBackgroundRow(), pGuild->GetBackgroundCol(), pGuild->GetMarkTimeConvertGameTime());
				pGuild->SendToAll(rmsg, true);
			}
		}
		break;
	default:
		break;
	}
}
#endif

#ifdef WARCASTLE_STATE_CHANNEL_CYNC
void ProcHelperCastleStateCync(CNetMsg::SP& msg)
{
	int mercState;
	RefMsg(msg) >> mercState;
	CWarCastle* pMercWar = CWarCastle::GetCastleObject(ZONE_MERAC);
	if( pMercWar )
	{
		if( gserver->m_subno != WAR_CASTLE_SUBNUMBER_MERAC) // 공성 채널은 갱신해줄 필요가 없다.
			pMercWar->SetStateChannelCync( mercState );
	}

	int dratanState;
	RefMsg(msg) >> dratanState;
	CWarCastle * pDratanWar = CWarCastle::GetCastleObject(ZONE_DRATAN);
	if( pDratanWar )
	{
		if( gserver->m_subno != WAR_CASTLE_SUBNUMBER_DRATAN) // 공성 채널은 갱신해줄 필요가 없다.
			pDratanWar->SetStateChannelCync(dratanState);
	}
}
#endif // WARCASTLE_STATE_CHANNEL_CYNC

#ifdef DEV_GUILD_STASH
void LogGuildStash(CItem* pItem, const LONGLONG count, const unsigned char type, CPC* pc)
{
	/*
		if type is 0 keep item to stash
		if type is 1 take item to stash
	*/

	char tmpBuf[MAX_STRING_LENGTH] = {0,};
	if ( type == GUILD_STASH_KEEP_LOG )
		strcpy(tmpBuf, "SUCCESS KEEP ITEM TO GUILD STASH");
	else if ( type == GUILD_STASH_TAKE_LOG )
		strcpy(tmpBuf, "SUCCESS TAKE ITEM TO GUILD STASH");
	else
		return ;

	GAMELOG << init(tmpBuf, pc);
	if (pItem->IsRareItem() == true
		&& pItem->m_pRareOptionProto != NULL
		&& pItem->m_pRareOptionProto->GetIndex() > 0)
	{
		GAMELOG << "[rare index: " << pItem->m_pRareOptionProto->GetIndex() << delim
				<< "rare bit: " << pItem->m_nRareOptionBit << "] " ;
	}
	GAMELOG	<< pItem->getDBIndex() << delim
		<< pItem->m_itemProto->getItemName() << delim
		<< pItem->m_serial.c_str() << delim
		<< pItem->getWearPos() << delim
		<< pItem->getPlus() << delim
		<< pItem->getFlag() << delim
		<< pItem->getUsed() << delim
		<< pItem->getUsed_2() << delim
		<< count << delim
		<< "OPTION" << delim
		<< pItem->m_nOption << delim;

	for (int i = 0; i < pItem->m_nOption; i++) {
		GAMELOG << pItem->m_option[i].m_type << delim
				<< pItem->m_option[i].m_value << delim;
	}

	GAMELOG << "SOCKET" << delim;
	for (int i = 0; i < MAX_SOCKET_COUNT; i++)
	{
		GAMELOG << pItem->m_socketList.GetJewelAt(i) << delim;
	}
	GAMELOG << end;
}

//XX 길드창고 - 리스트 (3)
void ProcHelperGuildStashList(CNetMsg::SP& msg)
{
	int charIndex = 0;
	int totalPage = 0;
	int curPage = 0;
	int itemCount = 0;
	int i = 0;
	int isNas = 0;
	GoldType_t money = 0;

	CLCString	timeStamp(32);
	int			capacity = 0;
	int subtype;

	GoldType_t moneyCount = 0;
	int nasStashIndex;

	msg->MoveFirst();

	RefMsg(msg) >> subtype
		>> charIndex
		>> timeStamp
		>> capacity
		>> money
		>> itemCount;

	CPC* pc = NULL;
	pc = PCManager::instance()->getPlayerByCharIndex(charIndex);
	if( pc == NULL )	// 그런사람 없습니다.
	{
		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	GuildStashListMsg( rmsg , timeStamp, capacity, money, itemCount );
	CItem** ppDummyItem = new CItem*[itemCount];
	for( i=0 ; i< itemCount ; i++ )
	{
		int			stashIndex = 0;
		int			index	= 0;
		int			plus	= 0;
		int			flag	= 0;
		int			used	= -1;
		int			used2	= -1;
		LONGLONG	count	= 0;
		short		option[MAX_ITEM_OPTION];
		CLCString	socket(1024);
		int			itemOrigin[MAX_VARIATION_COUNT];
		int			now_dur	= 0;
		int			max_dur = 0;
		RefMsg(msg) >> stashIndex
			>> index
			>> plus
			>> flag
			>> used
			>> used2
			>> now_dur
			>> max_dur
			>> count;
		for ( int idx = 0 ; idx < MAX_ITEM_OPTION ; ++idx )
			RefMsg(msg) >> option[idx];
		RefMsg(msg) >> socket ;
		RefMsg(msg) >> itemOrigin[0] >> itemOrigin[1] >> itemOrigin[2] >> itemOrigin[3] >> itemOrigin[4] >> itemOrigin[5] ;
		// 더미로 하나 만들고 MSG 만들어서 지워?
		ppDummyItem[i] = gserver->m_itemProtoList.CreateDBItem_2( index, -1, plus, flag, used, used2, "DUMMY", count, option, socket, itemOrigin );
		if( ppDummyItem[i] )
		{
			ppDummyItem[i]->setNowDurability(now_dur);
			ppDummyItem[i]->setMaxDurability(max_dur);
			
			RefMsg(rmsg) << stashIndex
				<< ppDummyItem[i]->getDBIndex()
				<< ppDummyItem[i]->getPlus()
				<< ppDummyItem[i]->getFlag()
				<< ppDummyItem[i]->getUsed()
				<< ppDummyItem[i]->getUsed_2()
				<< ppDummyItem[i]->getNowDurability()
				<< ppDummyItem[i]->getMaxDurability()
				<< ppDummyItem[i]->Count();
			ItemPublicMsg(rmsg, ppDummyItem[i]);
			ItemSocketMsg(rmsg, ppDummyItem[i]);
		}
		else
		{
			LOG_ERROR("Invalid Item Index. index[%d], count[%d]", index, count);
		}
	}	

	SEND_Q( rmsg, pc->m_desc );

	for( i = 0 ; i < itemCount ; ++i )
	{
		if( ppDummyItem[i] )
			delete ppDummyItem[i];
	}
	delete [] ppDummyItem;
}

//XX 길드창고 - 보관 (3)
void ProcHelperGuildStashKeep(CNetMsg::SP& msg)
{
	int		charIndex = 0;
	int		guildIndex = 0;
	char	error = 0;			// 0: 성공 , 1:길드 없음 , 2:인벤부족, 3:무게초과 4: 다른 길드원이 사용중

	RefMsg(msg) >> charIndex
		>> guildIndex
		>> error;

	// PC 찾고
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charIndex);
	if( !pc )
	{
		// Helper 로 UnLock 요청
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashKeepErrorMsg( rmsg, charIndex, guildIndex, 1 );
		SEND_Q( rmsg, gserver->m_helper );
		return;
	}

	// Helper 에서 온 error 처리
	// 1,2,3 인경우 에만 Helper 에 Unlock 하고, 4 인경우는 보내지 않음 // error 1,2,3,4 는 클라에 전달
	switch ( error )
	{
	case 0:		// 성공
		break;
	case 1: case 2: case 3:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashKeepErrorMsg( rmsg, charIndex, guildIndex, 1 );			// 1. 실패
			SEND_Q( rmsg, gserver->m_helper );
		}
	case 4:		// 다른 누군가 사용중
	case 9:		// 길드 창고 공간 없음
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_KEEP , error );
			SEND_Q( rmsg, pc->m_desc );
		}
		break;
	default :	// 무슨일이 생겼다!!! Unlock 해주자
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashKeepErrorMsg( rmsg, charIndex, guildIndex, 1 );			// 1. 실패
			SEND_Q( rmsg, gserver->m_helper );
		}
		break;
	}

	// Helper 에서 넘어온 에러가 있으면 여기까지
	if( error > 0 )
		return;

	// 혹시나 길드 확인
	if( !pc->m_guildInfo || !pc->m_guildInfo->guild() || pc->m_guildInfo->guild()->index() != guildIndex )
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashKeepErrorMsg( rmsg, charIndex, guildIndex, 1 );
			SEND_Q( rmsg, gserver->m_helper );
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_KEEP , 1 );
			SEND_Q( rmsg, pc->m_desc );
		}

		return;
	}

	// 아이템 및 돈 차감
	bool	bCanKeep = true;
	LONGLONG keepMoney = 0;
	int		nItemCount = 0;
	CLCString serial(MAX_SERIAL_LENGTH + 1);

	RefMsg(msg) >> keepMoney >> nItemCount;

	if (keepMoney > 0)
	{
		GoldType_t oldNas = pc->m_inventory.getMoney();

		pc->m_inventory.decreaseMoney(keepMoney);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::makeUpdateMoneyDescReason(rmsg, NAS_DESC_REASON_GUILD_KEEP, keepMoney);
			SEND_Q(rmsg, pc->m_desc);
		}

		GAMELOG << init("SUCCESS KEEP NAS TO GUILD STASH", pc)
			<< "CHAR INDEX : " << pc->m_index << delim
			<< "BEFORE NAS : " << oldNas << delim
			<< "KEEP NAS : " << keepMoney << delim
			<< "AFTER NAS : " << pc->m_inventory.getMoney() << end;
	}

	// Memory 할당후 Delete 하지 않고 Return 되지 않도록 주의
	CItem**		pKeepItem = new CItem*[nItemCount];
	LONGLONG*	pKeepItemCount = new LONGLONG[nItemCount];
	int dbIndex;
	int vIndex;

	// msg 뽑으면서 아이템을 확인
	for( int i = 0 ; i < nItemCount ; ++i )
	{
		RefMsg(msg) >> serial
			>> pKeepItemCount[i]
			>> dbIndex
			>> vIndex;

		pKeepItem[i] = pc->m_inventory.FindByVirtualIndex(vIndex);

		if(pKeepItem[i])
		{
			if( pKeepItem[i]->Count() >= pKeepItemCount[i] )
				continue;
			else{
				bCanKeep = false;
				break;
			}
		}
		else
		{
			bCanKeep = false;
		}
	}

	if( bCanKeep )
	{
		for( int i = 0 ; i < nItemCount ; ++i )
		{
			LogGuildStash(pKeepItem[i], pKeepItemCount[i], GUILD_STASH_KEEP_LOG, pc);

			pc->m_inventory.decreaseItemCount(pKeepItem[i], pKeepItemCount[i]);
			
			pKeepItem[i] = NULL;
			pKeepItemCount[i]	= 0;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashKeepErrorMsg( rmsg, charIndex, guildIndex, 0, keepMoney );
			SEND_Q( rmsg, gserver->m_helper );
		}

		{
			// 성공
			CNetMsg::SP rmsg(new CNetMsg);
			GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_KEEP , 0 );
			SEND_Q( rmsg, pc->m_desc );
		}

		// 실시간 저장
		DBManager::instance()->SaveCharacterInfo(pc->m_desc, false);
	}
	else
	{
		{
			// 실패
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashKeepErrorMsg( rmsg, charIndex, guildIndex, 1 );
			SEND_Q( rmsg, gserver->m_helper );
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_KEEP , 1 );
			SEND_Q( rmsg, pc->m_desc );
		}
	}

	delete [] pKeepItem;
	delete [] pKeepItemCount;

	return;
}

//XX 길드창고 - 찾기 (3)
void ProcHelperGuildStashTake(CNetMsg::SP& msg)
{
	int		charIndex = 0;
	int		guildIndex = 0;
	char	error = 0;

	RefMsg(msg) >> charIndex
		>> guildIndex
		>> error;			// 0: 성공 , 1:길드 없음 , 2:인벤부족, 3:무게초과 4: 다른 길드원이 사용중 5: 찾을수 없는 아이템

	// PC 찾고
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charIndex);
	if( !pc )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashKeepErrorMsg( rmsg, charIndex, guildIndex, 1 );				// Helper 로 UnLock 요청
		SEND_Q( rmsg, gserver->m_helper );
		return;
	}

	// Helper 에서 온 error 처리
	// 1,2,3,5 인경우 에만 Helper 에 Unlock 하고, 4 인경우는 보내지 않음 // error 1,2,3,4 는 클라에 전달
	switch ( error )
	{
	case 0:		// 성공
		break;
	case 1: case 2: case 3: case 5:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashKeepErrorMsg( rmsg, charIndex, guildIndex, 1 );			// 1. 실패
			SEND_Q( rmsg, gserver->m_helper );
		}
	case 4:		// 다른 누군가 사용중
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_TAKE , error );
			SEND_Q( rmsg, pc->m_desc );
		}
		break;
	default :	// 무슨일이 생겼다!!! Unlock 해주자
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashKeepErrorMsg( rmsg, charIndex, guildIndex, 1 );			// 1. 실패
			SEND_Q( rmsg, gserver->m_helper );
		}
		break;
	}

	// Helper 에서 넘어온 에러가 있으면 여기까지
	if( error > 0 )
		return;

	// 혹시나 길드 확인
	if( !pc->m_guildInfo || !pc->m_guildInfo->guild() || pc->m_guildInfo->guild()->index() != guildIndex )
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashTakeErrorMsg( rmsg, charIndex, guildIndex, 1 );
			SEND_Q( rmsg, gserver->m_helper );
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_TAKE , 1 );
			SEND_Q( rmsg, pc->m_desc );
		}

		return;
	}

	// 메세지 뽑고
	LONGLONG	takeMoney = 0;
	int			itemCount  = 0;
	int			index	= 0;
	int			plus	= 0;
	int			flag	= 0;
	int			used	= -1;
	int			used2	= -1;
	LONGLONG	count	= 0;
	short		option[MAX_ITEM_OPTION] = {0,};
	CLCString	serial(MAX_SERIAL_LENGTH + 1);
	CLCString	socket(1024);
	int			itemOrigin[MAX_VARIATION_COUNT] = {0,};
	RefMsg(msg) >> takeMoney >> itemCount;

	if( pc->m_inventory.getEmptyCount() < itemCount )	// 2: 인벤 부족
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashTakeErrorMsg( rmsg, charIndex, guildIndex, 2 );
			SEND_Q( rmsg , gserver->m_helper );
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_TAKE , 2 );
			SEND_Q( rmsg, pc->m_desc );
		}

		return;
	}

	bool	bGiveGuildItem = true;
	int		errorIndex = 0;
	int		now_dur = 0;
	int		max_dur = 0;

	// Memory 할당후 Delete 하지 않고 Return 되지 않도록 주의
	CItem** ppGuildItem = new CItem*[itemCount];
	for( int i=0 ; i<itemCount ; ++i )
	{
		RefMsg(msg) >> index >> plus >> flag >> used >> used2 >> now_dur >> max_dur
			>> count ;

		for ( int idx = 0 ; idx < MAX_ITEM_OPTION ; ++idx )
			RefMsg(msg) >> option[idx];

		RefMsg(msg) >> serial
			>> socket;
		RefMsg(msg) >> itemOrigin[0] >> itemOrigin[1] >> itemOrigin[2] >> itemOrigin[3] >> itemOrigin[4] >> itemOrigin[5];

		ppGuildItem[i] = gserver->m_itemProtoList.CreateDBItem_2( index, -1, plus, flag, used, used2, (const char*) serial, count, option, socket, itemOrigin );
		ppGuildItem[i]->setNowDurability(now_dur);
		ppGuildItem[i]->setMaxDurability(max_dur);

		// 아이템 생성 확인 및 무개 검사
		if( !ppGuildItem[i] )
		{
			// 하나라도 아이템 생성 실패
			bGiveGuildItem = false;
			errorIndex = i;
			break;
		}
	}

	if( bGiveGuildItem )	// 이정도 검사했는데 다 들어가겠지...
	{
		if (takeMoney > 0)
		{
			GoldType_t oldNas = pc->m_inventory.getMoney();

			pc->m_inventory.increaseMoney(takeMoney);

			GAMELOG << init("SUCCESS TAKE NAS TO GUILD STASH", pc)
				<< "CHAR INDEX : " << pc->m_index << delim
				<< "BEFORE NAS : " << oldNas << delim
				<< "TAKE NAS : " << takeMoney << delim
				<< "AFTER NAS : " << pc->m_inventory.getMoney() << end;
		}

		for( int i = 0 ; i < itemCount ; ++i )
		{
			if (pc->m_inventory.addItem(ppGuildItem[i]))
			{
				LogGuildStash(ppGuildItem[i], ppGuildItem[i]->Count(), GUILD_STASH_TAKE_LOG, pc);
			}
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashTakeErrorMsg( rmsg, charIndex, guildIndex, 0, takeMoney );			// 0: 정상처리
			SEND_Q( rmsg , gserver->m_helper );
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_TAKE , 0 );	 // 0: 정상처리
			SEND_Q( rmsg, pc->m_desc );
		}
	}
	else 	// 정리 합시다.
	{
		for( int delIdx=0 ; delIdx<errorIndex ; ++delIdx )
			delete ppGuildItem[delIdx];		// Create 한 Item 삭제

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashTakeErrorMsg( rmsg, charIndex, guildIndex, 3 );			// 3: 지급 실패 ( 무게 초과 )
			SEND_Q( rmsg , gserver->m_helper );
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_TAKE , 3 );	 // 3: 인벤 무게 초과
			SEND_Q( rmsg, pc->m_desc );
		}
	}

	if( ppGuildItem )
		delete [] ppGuildItem;

	return;
}

void ProcHelperGuildStashLog(CNetMsg::SP& msg)
{
	int		charIndex = 0;
	int		guildIndex = 0;
	int		logCount = 0;

	RefMsg(msg) >> charIndex
		>> guildIndex
		>> logCount;

	// PC 찾고
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charIndex);
	if( !pc )
		return;

	if( logCount < 1 )
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildStashLogMsg( rmsg, logCount );
		for( int i=0; i<logCount; ++i )
		{
			int logIndex = 0;
			CLCString date(32);
			CLCString nick(45);
			char	cState = -1;
			int		itemIndex = 0;
			int			itemPlus = 0;
			LONGLONG	itemCount = 0;

			RefMsg(msg) >> logIndex >> date >> nick >> cState >> itemIndex >> itemPlus >> itemCount ;
			int realPlus = (itemPlus & 0x0000ffff);
			RefMsg(rmsg) << logIndex << date << nick << cState << itemIndex << realPlus << itemCount;
		}
		SEND_Q( rmsg, pc->m_desc );
	}
}

// 길드 창고에 대한 전반적인 알림 에러 처리
void ProcHelperGuildStashError(CNetMsg::SP& msg)
{
	int		charIndex = 0;
	char	error = 0;

	RefMsg(msg) >> charIndex >> error;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charIndex);
	if( !pc )
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_ERROR, error );	 // 핼퍼에서 받은 그대로 보내주자
		SEND_Q( rmsg, pc->m_desc );
	}
}
#endif // DEV_GUILD_STASH

void ProcHelperStudentListRefresh(CNetMsg::SP& msg)
{
	int teacherIndex;
	RefMsg(msg) >> teacherIndex;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(teacherIndex);
	if( !pc )
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperTeacherStudentListCyncReq(rmsg, pc);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void ProcHelperTeacherStudentLsitCync(CNetMsg::SP& msg)
{
	CLCString idname(MAX_ID_NAME_LENGTH + 1);
	int teacherIndex;

	RefMsg(msg) >> idname
		>> teacherIndex;

	CPC * pc = PCManager::instance()->getPlayerByCharIndex( teacherIndex );

	if( !pc )
		return;

	char bteacher;
	RefMsg(msg) >> pc->m_cntTeachingStudent
		>> pc->m_guardian
		>> pc->m_superstone
		>> pc->m_cntFailStudent
		>> pc->m_cntCompleteStudent
		>> pc->m_teachType
		>> pc->m_fame
		>> bteacher;
	if( bteacher )
		pc->m_bTeacher = true;
	else
		pc->m_bTeacher = false;

	if( pc->m_teachType != MSG_TEACH_TEACHER_TYPE )
		return;

	ProcHelperTeacherInfoSet( pc, msg );

	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachInfoMsg(rmsg, pc);
		SEND_Q(rmsg, pc->m_desc);
	}
}

void ProcHelperTeacherInfoSet( CPC * pc, CNetMsg::SP& msg )
{
	RefMsg(msg) >> pc->m_teachTime[0]
		>> pc->m_teachTime[1];
	CLCString teachName(MAX_CHAR_NAME_LENGTH + 1);
	for(int i = 0; i < TEACH_MAX_STUDENTS; i++)
	{
		RefMsg(msg) >> pc->m_teachIdx[i]
			>> pc->m_teachJob[i]
			>> pc->m_teachJob2[i]
			>> pc->m_teachLevel[i]
			>> teachName;
		strcpy(pc->m_teachName[i], teachName);

		if( pc->m_teachIdx[i] != -1 )
		{
			CPC* student = PCManager::instance()->getPlayerByCharIndex(pc->m_teachIdx[i]);
			if( student )
			{
				pc->m_teachIdx[i] = student->m_index;
				pc->m_teachJob[i] = student->m_job;
				pc->m_teachJob2[i] = student->m_job2;
				pc->m_teachLevel[i] = student->m_level;
				strcpy(pc->m_teachName[i], student->GetName());
			}
		}
	}
}

//#endif

void ProcHelperGuildMasterKickRep(CNetMsg::SP& msg)
{
	int _guildIndex = -1, _requestCharIndex = -1, _result = -1, _requestTime;
	CLCString _currentBoss(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> _guildIndex
		>> _requestCharIndex
		>> _result
		>> _requestTime;

	CPC* _pc = PCManager::instance()->getPlayerByCharIndex(_requestCharIndex);

	// 결과 처리
	CGuild* _guild = gserver->m_guildlist.findguild(_guildIndex);
	if ( _guild )
	{
		_currentBoss = _guild->boss()->GetName();

		if ( _result == 0 )
		{	// 추방 예약 성공 : 길드원 전체에게 뿌린다
			CNetMsg::SP rmsg(new CNetMsg);
			GuildMasterKickRep(rmsg, _guildIndex, _currentBoss);
			_guild->SendToAll(rmsg);

			// 추방 관련 정보 설정
			_guild->getGuildKick()->setKickReuestChar(_requestCharIndex);
			_guild->getGuildKick()->setKickRequestTime(_requestTime);
		}
		else if ( _pc )
		{	// 추방 예약 실패 : 신청자에게만 뿌린다
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)_result);
			SEND_Q(rmsg, _pc->m_desc);
		}
	}
}

void ProcHelperGuildMasterKickCancelRep(CNetMsg::SP& msg)
{
	int _guildIndex = -1, _requestCharIndex = -1, _result = -1;
	RefMsg(msg) >> _guildIndex
		>> _requestCharIndex
		>> _result;

	CPC* _pc = PCManager::instance()->getPlayerByCharIndex(_requestCharIndex);

	// 결과 처리
	CGuild* _guild = gserver->m_guildlist.findguild(_guildIndex);
	if ( _guild )
	{
		if ( _result == 0 )
		{	// 추방 취소 성공 : 길드원 전체에 뿌린다
			CNetMsg::SP rmsg(new CNetMsg);
			GuildMasterKickCancelRep(rmsg, _guildIndex);
			_guild->SendToAll(rmsg);

			// 추방 관련 정보 설정
			_guild->getGuildKick()->setKickReuestChar(_requestCharIndex);
		}
		else if ( _pc )
		{	// 추방 취소 실패 : 신청자에게만 뿌린다
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)_result);
			SEND_Q(rmsg, _pc->m_desc);
		}
	}
}

void ProcHelperGuildMasterKickStatus(CNetMsg::SP& msg)
{
	int _guildIndex = -1, _status = -1;
	RefMsg(msg) >> _guildIndex
		>> _status;

	CGuild* _guild = gserver->m_guildlist.findguild(_guildIndex);
	if ( _guild )
	{
		_guild->getGuildKick()->setKickStatus(_status);
		CNetMsg::SP rmsg(new CNetMsg);
		GuildMasterKickStatus(rmsg, _guildIndex, _status);
		_guild->SendToAll(rmsg);
	}
}

void ProcHelperGuildMasaterKickReset(CNetMsg::SP& msg)
{
	int charIndex = -1, reset = -1;

	RefMsg(msg) >> charIndex
		>> reset;

	CPC* ch = PCManager::instance()->getPlayerByCharIndex(charIndex);
	if ( ch == NULL )
		return;

	CLCString message(128);
	if ( reset )
		message = "Reset Complete";
	else
		message = "Reset Fail, Try again";

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
		SEND_Q(rmsg, ch->m_desc);
	}
}

//RVR
void ProcHelperRVRInfoUpdate(CNetMsg::SP& msg)
{
	UpdateServer::SyndicateInfo* p = reinterpret_cast<UpdateServer::SyndicateInfo*>(msg->m_buf);
	SyndicateInfoDataManager::instance()->setJewelPoint(SYNDICATE::eSYNDICATE_KAILUX, p->jewelCount_k);
	SyndicateInfoDataManager::instance()->setJewelPoint(SYNDICATE::eSYNDICATE_DEALERMOON, p->jewelCount_d);
	SyndicateInfoDataManager::instance()->setJoinCount(SYNDICATE::eSYNDICATE_KAILUX, p->userCount_k);
	SyndicateInfoDataManager::instance()->setJoinCount(SYNDICATE::eSYNDICATE_DEALERMOON, p->userCount_d);
}

void ProcHelperRvRUpdateKingInfo( CNetMsg::SP& msg )
{
	UpdateServer::SyndicateUpdateKingInfoAll* p = reinterpret_cast<UpdateServer::SyndicateUpdateKingInfoAll*>(msg->m_buf);
	
	//왕이 바뀐 경우
	if(p->kingIndex_d != SyndicateInfoDataManager::instance()->getKingIndex(SYNDICATE::eSYNDICATE_DEALERMOON))
	{
		CPC* pChar = PCManager::instance()->getPlayerByCharIndex(p->kingIndex_d);

		if (pChar != NULL)
		{
			pChar->m_syndicateManager.setSyndicateGrade(SYNDICATE::eSYNDICATE_DEALERMOON, SYNDICATE::IPSISSIMUS);
			pChar->m_syndicateManager.historyManager_.promotionHistory(SYNDICATE::eSYNDICATE_DEALERMOON, SYNDICATE::IPSISSIMUS);
			pChar->m_syndicateManager.historyManager_.sendHistory(SYNDICATE::eSYNDICATE_DEALERMOON);
			pChar->m_syndicateManager.send();
			pChar->m_syndicateManager.applySyndicateSkill(SYNDICATE::eSYNDICATE_DEALERMOON);
		}
	}

	SyndicateInfoDataManager::instance()->setKingName(SYNDICATE::eSYNDICATE_DEALERMOON, p->kingName_d);
	SyndicateInfoDataManager::instance()->setKingPoint(SYNDICATE::eSYNDICATE_DEALERMOON, p->kingPoint_d);
	SyndicateInfoDataManager::instance()->setKingIndex(SYNDICATE::eSYNDICATE_DEALERMOON, p->kingIndex_d);
	
	//왕이 바뀐 경우
	if(p->kingIndex_k != SyndicateInfoDataManager::instance()->getKingIndex(SYNDICATE::eSYNDICATE_KAILUX))
	{
		CPC* pChar = PCManager::instance()->getPlayerByCharIndex(p->kingIndex_k);

		if(pChar != NULL)
		{
			pChar->m_syndicateManager.syndicateData_.syndicate_grade_k = SYNDICATE::PRINCIPE;
			pChar->m_syndicateManager.historyManager_.promotionHistory(SYNDICATE::eSYNDICATE_KAILUX, SYNDICATE::PRINCIPE);
			pChar->m_syndicateManager.historyManager_.sendHistory(SYNDICATE::eSYNDICATE_KAILUX);
			pChar->m_syndicateManager.send();
			pChar->m_syndicateManager.applySyndicateSkill(SYNDICATE::eSYNDICATE_KAILUX);
		}
	}

	SyndicateInfoDataManager::instance()->setKingName(SYNDICATE::eSYNDICATE_KAILUX, p->kingName_k);
	SyndicateInfoDataManager::instance()->setKingPoint(SYNDICATE::eSYNDICATE_KAILUX, p->kingPoint_k);
	SyndicateInfoDataManager::instance()->setKingIndex(SYNDICATE::eSYNDICATE_KAILUX, p->kingIndex_k);
}

void ProcHelperRvRKingDownGrade( CNetMsg::SP& msg )
{
	UpdateServer::SyndicateKingDownGrade* p = reinterpret_cast<UpdateServer::SyndicateKingDownGrade*>(msg->m_buf);
	
	CPC* pChar = PCManager::instance()->getPlayerByCharIndex(p->charIndex);
	int downgrade;
	
	if(pChar != NULL)
	{
		downgrade = SYNDICATE::getSyndicateGrade(p->syndicateType, pChar->m_syndicateManager.getSyndicatePoint(p->syndicateType));
		pChar->m_syndicateManager.setSyndicateGrade(p->syndicateType, downgrade);
		pChar->m_syndicateManager.historyManager_.degradeHistory(p->syndicateType, downgrade);
		pChar->m_syndicateManager.historyManager_.sendHistory(p->syndicateType);
		pChar->m_syndicateManager.send();
		pChar->m_syndicateManager.applySyndicateSkill(p->syndicateType);
	}
	else
	{
		if(p->syndicateType == SYNDICATE::eSYNDICATE_KAILUX)
			downgrade = SYNDICATE::DUKA;
		
		else if(p->syndicateType == SYNDICATE::eSYNDICATE_DEALERMOON)
			downgrade = SYNDICATE::MAGUS;

		//접속해 있지 않은 경우에는 쿼리문 저장
		std::string insertStr = "INSERT INTO t_syndicate_history VALUES";
		insertStr.reserve(1024);

		insertStr += boost::str(
			boost::format("(%1%, %2%, %3%, %4%, %5%, '', now())")
			% SyndicateInfoDataManager::instance()->getKingIndex(p->syndicateType) % p->syndicateType % SYNDICATE::DEGRADE % downgrade % SYNDICATE::eSYNDICATE_NONE
			);
		DBManager::instance()->pushQuery(0, insertStr);
	}
}
//RVR

void ProcHelperKickUser(CNetMsg::SP& msg)
{
	ServerToServerPacket::kickUser* packet = reinterpret_cast<ServerToServerPacket::kickUser*>(msg->m_buf);
	CPC* kickedch = PCManager::instance()->getPlayerByName(packet->kicked_charName, true);
	if (kickedch == NULL)
	{
		packet->subType = MSG_SUB_SERVERTOSERVER_KICK_ANSER;
		packet->result = false;
		SEND_Q(msg, gserver->m_helper);
		return;
	}
	
	LOG_INFO("KICK : kicked by %d", packet->req_charIndex);
	kickedch->m_desc->Close("kicked");

	{
		packet->subType = MSG_SUB_SERVERTOSERVER_KICK_ANSER;
		packet->result = true;
		SEND_Q(msg, gserver->m_helper);
	}
}

void ProcHelperKickUserAnswer(CNetMsg::SP& msg)
{
	ServerToServerPacket::kickUser* packet = reinterpret_cast<ServerToServerPacket::kickUser*>(msg->m_buf);
	std::string kickName = packet->kicked_charName;

	CPC* reqUser = PCManager::instance()->getPlayerByCharIndex(packet->req_charIndex);
	if (reqUser == NULL)
	{
		// 요청자(주로 GM)가 logout한 경우
		return;
	}

	std::string tstr = "";
	if (packet->result)
	{
		tstr = boost::str(boost::format(
			"KICK : User(%s) was kicked") % kickName);
	}
	else
	{
		tstr = boost::str(boost::format(
			"KICK : Not found User(%s)") % kickName);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tstr.c_str());
		SEND_Q(rmsg, reqUser->m_desc);
	}
}

void ProcHelperKickUserByCharIndexAnswer(CNetMsg::SP& msg)
{
	ServerToServerPacket::kickUserByCharIndex* packet = reinterpret_cast<ServerToServerPacket::kickUserByCharIndex*>(msg->m_buf);
	CPC* reqUser = PCManager::instance()->getPlayerByCharIndex(packet->req_charIndex);
	if (reqUser == NULL)
	{
		// 요청자(주로 GM)가 logout한 경우
		return;
	}

	std::string tstr = "";
	if (packet->result)
	{
		tstr = boost::str(boost::format(
			"KICK : User(charIndex: %d) was kicked") % packet->kicked_charIndex);
	}
	else
	{
		tstr = boost::str(boost::format(
			"KICK : Not found User(charIndex : %d)") % packet->kicked_charIndex);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tstr.c_str());
		SEND_Q(rmsg, reqUser->m_desc);
	}
}

void ProcHelperKickUserByUserIndexAnswer(CNetMsg::SP& msg)
{
	ServerToServerPacket::kickUserByUserIndex* packet = reinterpret_cast<ServerToServerPacket::kickUserByUserIndex*>(msg->m_buf);
	CPC* reqUser = PCManager::instance()->getPlayerByCharIndex(packet->req_charIndex);
	if (reqUser == NULL)
	{
		// 요청자(주로 GM)가 logout한 경우
		return;
	}

	std::string tstr = "";
	if (packet->result)
	{
		tstr = boost::str(boost::format(
			"KICK : User(userIndex : %d) was kicked") % packet->kicked_userIndex);
	}
	else
	{
		tstr = boost::str(boost::format(
			"KICK : Not found User(userIndex : %d)") % packet->kicked_userIndex);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tstr.c_str());
		SEND_Q(rmsg, reqUser->m_desc);
	}
}

void ProcHelperKickUserByUserIdAnswer(CNetMsg::SP& msg)
{
	ServerToServerPacket::kickUserByUserId* packet = reinterpret_cast<ServerToServerPacket::kickUserByUserId*>(msg->m_buf);
	CPC* reqUser = PCManager::instance()->getPlayerByCharIndex(packet->req_charIndex);
	std::string kickName = packet->kicked_userId;
	if (reqUser == NULL)
	{
		// 요청자(주로 GM)가 logout한 경우
		return;
	}

	std::string tstr = "";
	if (packet->result)
	{
		tstr = boost::str(boost::format(
			"KICK : User(userId : %s) was kicked") % kickName);
	}
	else
	{
		tstr = boost::str(boost::format(
			"KICK : Not found User(userId : %s)") % kickName);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tstr.c_str());
		SEND_Q(rmsg, reqUser->m_desc);
	}
}

