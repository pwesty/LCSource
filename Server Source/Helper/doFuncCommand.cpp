#include <math.h>
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Server.h"
#include "doFunc.h"
#include "Guild.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"
#include "Log.h"
#include "../ShareLib/common_util.h"
#include "../ShareLib/PrintExcuteInfo.h"
#include "../ShareLib/packetType/ptype_char_status.h"
#include "../ShareLib/packetType/ptype_syndicate.h"
#include "../ShareLib//packetType/ptype_old_do_friend.h"

void do_Command(CNetMsg::SP& msg, CDescriptor* dest)
{
	int subtype = -1;
	msg->MoveFirst();
	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO:
		do_CommandPartyMatchMemberChangeInfoName(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_REG_MEMBER_REQ:
		do_CommandPartyMatchRegMemberReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_REG_PARTY_REQ:
		do_CommandPartyMatchRegPartyReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_DEL_REQ:
		do_CommandPartyMatchDelReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_INVITE_REQ:
		do_CommandPartyMatchInviteReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_JOIN_REQ:
		do_CommandPartyMatchJoinReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_JOIN_ALLOW_REQ:
		do_CommandPartyMatchJoinAllowReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_MATCH_JOIN_REJECT_REQ:
		do_CommandPartyMatchJoinRejectReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_CHANGE_BOSS_REQ:
		do_CommandPartyChangeBossReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_CHANGE_TYPE_REQ:
		do_CommandPartyChangeTypeReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_END_PARTY_REQ:
		do_CommandPartyEndPartyReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_BREAK_REQ:
		do_CommandPartyBreakReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_KICK_REQ:
		do_CommandPartyKickReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_QUIT_REQ:
		do_CommandPartyQuitReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_REJECT_REQ:
		do_CommandPartyRejectReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_ALLOW_REQ:
		do_CommandPartyAllowReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_INVITE_REP:
		do_CommandPartyInviteRep(msg, dest);
		break;

	case MSG_HELPER_PARTY_INVITE_REQ:
		do_CommandPartyInviteReq(msg, dest);
		break;

	case MSG_HELPER_PARTY_INVITE_CANCEL:
		do_CommandPartyInviteCancel(msg, dest);
		break;
	case MSG_HELPER_PARTY_OFFLINE:
		do_CommandPartyOffline( msg, dest );
		break;

	case MSG_HELPER_PARTY_ONLINE:
		do_CommandPartyOnline( msg, dest );
		break;

	case MSG_HELPER_SHUTDOWN:
		do_CommandShutdown(msg, dest);
		break;

	case MSG_HELPER_GUILD_CREATE_REQ:
		do_CommandGuildCreateReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_ONLINE:
		do_CommandGuildOnline(msg, dest);
		break;

	case MSG_HELPER_GUILD_LOAD_REQ:
		do_CommandGuildLoadReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_LEVELUP_REQ:
		do_CommandGuildLevelUpReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_BREAKUP_REQ:
		do_CommandGuildBreakUpReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_MEMBER_ADD_REQ:
		do_CommandGuildMemberAddReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_MEMBER_OUT_REQ:
		do_CommandGuildMemberOutReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_MEMBER_KICK_REQ:
		do_CommandGuildMemberKickReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_CHANGE_BOSS_REQ:
		do_CommandGuildChangeBossReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_APPOINT_OFFICER_REQ:
		do_CommandGuildAppointOfficerReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_CHAT:
		do_CommandGuildChat(msg, dest);
		break;

	case MSG_HELPER_GUILD_FIRE_OFFICER_REQ:
		do_CommandGuildFireOfficerReq(msg, dest);
		break;

	case MSG_HELPER_CHAR_DEL:
		do_CommandCharacterDelete(msg, dest);
		break;

	case MSG_HELPER_GUILD_BATTLE_REQ:
		do_CommandGuildBattleReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_BATTLE_STOP_REQ:
		do_CommandGuildBattleStopReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_BATTLE_PEACE_REQ:
		do_CommandGuildBattlePeaceReq(msg, dest);
		break;

	case MSG_HELPER_GUILD_BATTLE_KILL_REQ:
		do_CommandGuildBattleKillReq(msg, dest);
		break;
//0503 kwon
	case MSG_HELPER_EVENT_MOONSTONE_UPDATE_REQ:
		do_CommandEventMoonStoneUpdateReq(msg, dest);
		break;
	case MSG_HELPER_EVENT_MOONSTONE_JACKPOT_REQ:
		do_CommandEventMoonStoneJackPotReq(msg, dest);
		break;	
	case MSG_HELPER_FRIEND_SET_CONDITION_REQ:
		do_CommandFriendSetConditionReq(msg,dest);
		break;

//0616 bw
	case MSG_HELPER_PD4_RANK_INIT:
		do_CommandPD4RankInit(msg, dest);
		break;
	case MSG_HELPER_PD4_RANK_ADD:
		do_CommandPD4RankAdd(msg, dest);
		break;
	case MSG_HELPER_PD4_RANK_END:
		do_CommandPD4RankEnd(msg, dest);
		break;
	case MSG_HELPER_PD4_RANK_VIEW_REQ:
		do_CommandPD4RankView(msg, dest);
		break;
	case MSG_HELPER_PD4_REWARD_VIEW_REQ:
		do_CommandPD4RewardView(msg, dest);
		break;
	case MSG_HELPER_PD4_REWARD_REQ:
		do_CommandPD4Reward(msg, dest);
		break;
	case MSG_HELPER_PD4_REWARD_INIT:
		do_CommandPD4RewardInit(msg, dest);
		break;
	case MSG_HELPER_PD4_REWARD_END:
		do_CommandPD4RewardEnd(msg, dest);
		break;

	case MSG_HELPER_NAME_CHANGE_REQ:
		do_CommandNameChange(msg, dest);
		break;

// 애완동물
	case MSG_HELPER_PET_CREATE_REQ:
		do_CommandPetCreateReq(msg, dest);
		break;
//TODO : DELETE

	case MSG_HELPER_PET_DELETE_REQ:
		do_CommandPetDeleteReq(msg, dest);
		break;

// 길드 나스 보관함
	case MSG_HELPER_GUILD_STASH_HISTORY_REQ:
		do_CommandGuildStashHistoryReq(msg, dest);
		break;
	case MSG_HELPER_GUILD_STASH_VIEW_REQ:
		do_CommandGuildStashViewReq(msg, dest);
		break;
	case MSG_HELPER_GUILD_STASH_TAKE_REQ:
		do_CommandGuildStashTakeReq(msg, dest);
		break;
	case MSG_HELPER_GUILD_STASH_ROLLBACK:
		do_CommandGuildStashRollback(msg, dest);
		break;
	case MSG_HELPER_GUILD_STASH_SAVE_TAX_REQ:
		do_CommandGuildStashSaveTaxReq(msg, dest);
		break;

	case MSG_HELPER_TEACHER_FAMEUP_REQ:
		do_CommandFameUp(msg, dest);
		break;
	case MSG_HELPER_TEACHER_REGISTER_REQ:
		do_CommandTeacherRegister(msg, dest);
		break;
	case MSG_HELPER_TEACHER_GIVEUP_REQ:
		do_CommandTeacherGiveup(msg, dest);
		break;
	case MSG_HELPER_TEACHER_REQ:
		do_CommandTeacher(msg, dest);
		break;
	case MSG_HELPER_TEACHER_TIMEOVER_REQ:
		do_CommandTeacherTimeover(msg, dest);
		break;
	case MSG_HELPER_TEACHER_SUPERSTONERECIEVE:
		do_CommandTeacherSuperstoneRecieve(msg, dest);
		break;
	case MSG_HELPER_TEACHER_LOAD_REQ:
		do_CommandTeacherLoad(msg, dest);
		break;
		//휴면케릭 이벤트
	case MSG_HELPER_SEARCHFRIEND_ADD_REQ:			//휴면케릭 등록하는 부분
		do_CommandEventSearchFriendMemberAddReq(msg, dest);
		break;
	case MSG_HELPER_SEARCHFRIEND_SELECT_ADD_REQ:			// 케릭 등록 선택
		do_CommandEventSearchFriendMemberSelectAddReq(msg, dest);
		break;
	case MSG_HELPER_SEARCHFRIEND_ONETIME_ADD_REQ:			// 한시간 단위 체크
		do_CommandEventSearchFriendMemberOneTimeCheckReq(msg, dest);
		break;
	case MSG_HELPER_SEARCHFRIEND_LISTGOOD_REQ:			// 등록된 친구 보상가능여부
		do_CommandEventSearchFriendMemberListGoodReq(msg, dest);
		break;
	case MSG_HELPER_SEARCHFRIEND_GOOD_REQ:
		do_CommandEventSearchFriendMemberGoodReq(msg, dest);
		break;
	case MSG_HELPER_BLOCKPC_REQ:
		do_CommandBlockCharReq(msg, dest);
		break;
	case MSG_HELPER_GIFT_RECVCHARNAME_REQ:
		do_CommandGiftCharReq(msg, dest);
		break;
	case MSG_HELPER_GUILD_INCLINE_ESTABLISH_REQ:
		do_CommandGuildInclineEstablish( msg, dest );
		break;
	case MSG_HELPER_GUILD_MEMBER_ADJUST_REQ:
		do_CommandGuildMemberAdjust( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_INFO_REQ:
		do_CommandNewGuildInfo( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_MEMBERLIST_REQ:
		do_CommandNewGuildMemberList( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_MANAGE_REQ:
		do_CommandNewGuildManage( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_NOTICE_REQ:
		do_CommandNewGuildNotice( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_NOTICE_UPDATE_REQ:
		do_CommandNewGuildNoticeUpdate( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_REQ:
		do_CommandNewGuildNoticeTrans( msg, dest );
		break;
	case MSG_HELPER_NEW_GUILD_NOTICE_SKILLLIST_REQ:
		do_CommandNewGuildSkillList( msg, dest );
		break;
//	case MSG_HELPER_SAVE_GUILD_SKILL:
//		do_CommandNewGuildSaveSkill( msg, dest );
		break;
	case MSG_HELPER_UPDATE_GUILD_POINT:
		do_CommandNewGuildUpdatePoint( msg, dest );
		break;
	case MSG_HELPER_SAVE_GUILD_POINT:
		do_CommandSaveGuildPoint( msg, dest );
		break;
	case MSG_HELPER_SAVE_GUILD_MEMBER_POINT:
		do_CommandSaveGuildMemberPoint( msg, dest );
		break;
	case MSG_HELPER_GUILD_NOTICE :
		do_CommandGuildNotice( msg, dest );
		break;
	case MSG_HELPER_GUILD_SKILL_LEARN:
		do_CommandGuildSkillLearn( msg, dest );
		break;
	case MSG_HELPER_GUILD_SKILL_LEARN_SEND_MEMBER:
		do_CommandGuildSkillLearnSendMember( msg );
		break;

	case MSG_HELPER_INSERT_GUARD:
		do_InsertGuard(msg, dest);
		break;

	case MSG_HELPER_CASTLE_TOWER_ADD:
		do_CastleTowerAdd(msg, dest);
		break;

	case MSG_HELPER_CASTLE_TOWER_REINFORCE:
		do_CastleTowerReinforce(msg, dest);
		break;

	case MSG_HELPER_CASTLE_QUARTERS_INSTALL:
		do_CastleQuartersInstall(msg, dest);
		break;

	case MSG_HELPER_PET_NAME_CHANGE:
		do_CommandPetChangeName( msg, dest );
		break;
	case MSG_HELPER_PET_COLOR_CHANGE:
		do_CommandPetColorChange( msg, dest );
		break;

	case MSG_HELPER_HALLOWEEN_2007:
		do_CommandHalloween2007( msg, dest );
		break;

	case MSG_HELPER_DVD_RATE_CHANGE:
		do_CommandDVDRateChange( msg, dest );
		break;

	case MSG_HELPER_DVD_NORMAL_CHANGE_NOTICE:
		do_CommandDVDNormalChangeNotice( msg, dest );
		break;
		
	case MSG_HELPER_DVD_NORMAL_CHANGE_TIME:
		break;
	case MSG_HELPER_PET_TURNTO_NPC:
		do_CommandPetTurnToNpc( msg, dest );
		break;

	case MSG_HELPER_CREATE_COMBO:
		do_CommandCreateCombo( msg, dest );
		break;

	case MSG_HELPER_APET:
		do_CommandAttackPet( msg, dest );
		break;

#ifdef EXTREME_CUBE
	case MSG_HELPER_CUBESTATE_REQ:
		do_CommandCubeStateReq( msg, dest );
		break;
	case MSG_HELPER_CUBESTATE_PERSONAL_REQ:
		do_CommandCubeStatePersonalReq( msg, dest );
		break;
	case MSG_HELPER_ADDCUBEPOINT:
		do_CommandAddCubePoint( msg, dest );
		break;
	case MSG_HELPER_ADDCUBEPOINT_PERSONAL:
		do_CommandAddCubePointPersonal( msg, dest );
		break;
#ifdef EXTREME_CUBE_VER2
	case MSG_HELPER_CUBEREWARD_PERSONAL_REQ:
		do_CommandCubeRewardPersonalReq( msg, dest );
		break;
	case MSG_HELPER_CUBEREWARD_PERSONAL_ROLLBACK:
		do_CommandCubeRewardPersonalRollBack( msg, dest );
		break;
#endif // EXTREME_CUBE_VER2
#endif // EXTREME_CUBE

	case MSG_HELPER_EVENT_PHOENIX:
		do_CommandEventPhoenix( msg, dest );
		break;

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
	case MSG_HELPER_DISCONNECT_HACK_CHARACTER:
		do_Command_DisconnectHackCharacter( msg, dest );
		break;
#endif // DISCONNECT_HACK_CHARACTER

	case MSG_HELPER_EXPED_CREATE_REQ:
		do_Command_Exped_CreateReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_CREATE_RESULT:
		do_Command_Exped_CreateResult( msg, dest );
		break;
	case MSG_HELPER_EXPED_INVITE_REQ:
		do_Command_Exped_InviteReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_ALLOW_REQ:
		do_Command_Exped_AllowReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_REJECT_REQ:
		do_Command_Exped_RejectReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_QUIT_REQ:
		do_Command_Exped_QuitReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_KICK_REQ:
		do_Command_Exped_KickReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_CHANGETYPE_REQ:
		do_Command_Exped_ChangeTypeReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_CHANGEBOSS_REQ:
		do_Command_Exped_ChangeBossReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_SETMBOSS_REQ:
		do_Command_Exped_SetMBossReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_RESETMBOSS_REQ:
		do_Command_Exped_ResetMBossReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_ENDEXPED_REQ:
		do_Command_Exped_EndExpedReq( msg, dest );
		break;
	case MSG_HELPER_EXPED_MOVEGROUP_REQ:
		do_Command_Exped_MoveGroupReq( msg, dest );
		break;

	case MSG_HELPER_EXPEND_OFFLINE:
		do_Command_Expend_OfflineSystem(msg, dest);
		break;

	case MSG_HELPER_EXPEND_ONLINE:
		do_Command_Expend_OnlineSystem(msg, dest);
		break;

	case MSG_HELPER_INZONE_GET_ROOMNO_REQ:
		do_Command_Raid_InzoneGetRoomNoReq( msg, dest );
		break;
	case MSG_HELPER_INZONE_GAMEDATA_CLEAR_REQ:
		do_Command_Raid_InzoneDataClearReq( msg, dest );
		break;
	case MSG_HELPER_INZONE_SET_ROOMNO :
		do_Command_Raid_InzoneSetRoomNo( msg , dest );
		break;

	case MSG_HELPER_TRIGGER_EVENT:
		do_CommandTriggerEvent( msg, dest );
		break;

	case MSG_HELPER_DELETE_RAID_CHAR:
		do_Command_DeleteRaidCharacter( msg, dest );
		break;

	case MSG_HELPER_RAID_INFO:
		do_Command_RaidInfo( msg, dest );
		break;

	case MSG_HELPER_NS_CARD:
		do_CommandNSCreateCard(msg, dest);
		break;

	case MSG_HELPER_PARTY_INIT_REQ:
		do_CommandPartyDataInit(msg, dest);
		break;

#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)
	case MSG_HELPER_EVENT_WORLDCUP2010:
		do_Worldcup2010Event(msg, dest);
		break;
#endif

	case MSG_HELPER_GUILD_USE_GUILD_POINT:
		do_CommandUseGuildPoint(msg, dest);
		break;

	case MSG_HELPER_TEACHER_SYSTEM_RENEWER:
		do_TeachRenewer(msg, dest);
		break;

	case MSG_HELPER_FACEOFF_REQ:
		do_CommandFaceOff(msg, dest);
		break;

#ifdef EVENT_CHAR_BIRTHDAY
	case MSG_HELPER_CHAR_BIRTHDAY:
		do_commandEventCharBirthday(msg, dest);
		break;
#endif
#ifdef DEV_GUILD_MARK
	case MSG_HELPER_NEW_GUILD_MARK_REGIST:
		do_CommandGuildMarkRegist(msg, dest);
		break;
#endif
	case MSG_HELPER_AUTO_RESET_RAID:
		do_CommandAutoResetRaid(msg, dest);
		break;

	case MSG_HELPER_ROYAL_RUMBLE_WINNER:
		do_CommandRoyalRumbleLog(msg, dest);
		break;

#ifdef DEV_GUILD_STASH
	case MSG_HELPER_GUILD_STASH_CREATE:
		do_CommandGuildStashCreate( msg, dest );
		break;
	case MSG_HELPER_GUILD_STASH_LIST :
		do_CommandGuildStashList( msg, dest );
		break;
	case MSG_HELPER_GUILD_STASH_KEEP :
		do_CommandGuildStashKeep( msg, dest );
		break;
	case MSG_HELPER_GUILD_STASH_KEEP_ERROR :
		do_CommandGuildStashKeepError( msg, dest );
		break;
	case MSG_HELPER_GUILD_STASH_TAKE :
		do_CommandGuildStashTake( msg, dest );
		break;
	case MSG_HELPER_GUILD_STASH_TAKE_ERROR :
		do_CommandGuildStashTakeError( msg, dest );
		break;
	case MSG_HELPER_GUILD_STASH_LOG :
		do_CommandGuildStashLog( msg, dest );
		break;
	case MSG_HELPER_GUILD_STASH_LIST_MONEY:
		do_CommandGuildStashList( msg, dest );
#endif // DEV_GUILD_STASH
#ifdef GUILD_MEMBER_KICK_JOIN_DELAY
	case MSG_HELPER_GUILD_MEMBER_KICK_OUTDATE_UPDATE_REQ:
		do_CommandGuildMemberKickOutDateUpdate(msg, dest);
		break;
#endif
	case MSG_HELPER_TEACHER_STUDENT_LIST_CYNC_REQ:
		do_CommandTeacherStudentListCync(msg, dest);
		break;
	case MSG_HELPER_GUILD_MASTER_KICK_REQ:
		do_CommandGuildMasterKickReq(msg, dest);
		break;
	case MSG_HELPER_GUILD_MASTER_KICK_CANCEL_REQ:
		do_CommandGuildMasterKickCancelReq(msg, dest);
		break;
	case MSG_HELPER_GUILD_MASTER_KICK_RESET:
		do_CommandGuildMasterKickReset(msg, dest);
		break;

	case MSG_HELPER_GUILD_MASTER_CHANGE:
		do_CommandGuildMasterChange(msg, dest);
		break;

	case MSG_HELPER_EVENT_TABLE_INIT:
		do_CommandEventTableInit(msg, dest);
		break;

	case MSG_HELPER_SELECT_CHARACTER:
		SEND_Q(msg, dest);
		break;

	default:
		break;
	}
}

void do_Command_Struct(CNetMsg::SP& msg, CDescriptor* dest)
{
	pTypeServer* p = reinterpret_cast<pTypeServer*>(msg->m_buf);

	switch(p->subType)
	{
	case MSG_HELPER_RVR_INCREASE_JEWEL_REQ:
		do_CommandRVRIncreaseJewel(msg);
		break;

	case MSG_HELPER_RVR_DECREASE_JEWEL_REQ:
		do_CommandRVRDecreaseJewel(msg);
		break;

	case MSG_HELPER_RVR_JOIN_REQ:
		do_CommandRVRJoinReq(msg);
		break;

	case MSG_HELPER_RVR_LEAVE_REQ:
		do_CommandRVRLeaveReq(msg);
		break;

	case MSG_HELPER_KING_INFO_UPDATE_ALL:
		do_CommandRVRChangeKingInfo(msg);
		break;

	case MSG_HELPER_KING_INFO_UPDATE_EACH:
		do_CommandRVRChangeKingInfoEach(msg);
		break;

	case MSG_HELPER_ADD_2ND_USER:
		do_CommandRVRAdd2ndGradeUser(msg);
		break;
	case MSG_HELPER_DEL_2ND_USER:
		do_CommandRVRDel2ndGradeUser(msg);
		break;
	case MSG_HELPER_UPDATE_2ND_USER:
		do_CommandRVRUpdate2ndGradeUser(msg);
		break;

	case MSG_HELPER_SET_USER_COUNT:
		do_CommandRVRSetUserCount(msg);
		break;

	case MSG_HELPER_FRIEND_MEMBER_ADD_REQ:
		do_CommandFriendMemberAddReq(msg, dest);
		break;
	case MSG_HELPER_FRIEND_MEMBER_DEL_REQ:
		do_CommandFriendMemberDelReq(msg, dest);
		break;
	}
}

void do_CommandTeacherLoad(CNetMsg::SP& msg, CDescriptor* dest)
{
	CLCString idname(MAX_ID_NAME_LENGTH + 1);
	int charindex;

	int		m_guardian;
	int		m_superstone;
	int		m_cntFailStudent;
	int		m_cntCompleteStudent;
	char		m_teachType;
	int		m_fame;
	char	bteacher;
	CLCString strTeachIdx(256);
	CLCString strTeachSec(256);
	int subtype;

	msg->MoveFirst();
	RefMsg(msg) >> subtype;

	RefMsg(msg) >> idname
		>> charindex;

	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);

	std::string select_character_query = boost::str(boost::format(
		"SELECT a_teach_idx, a_teach_sec, a_teach_type, a_fame, a_teach_list, a_teach_complete, a_teach_fail, a_superstone, a_guardian "
		" FROM t_characters WHERE a_index = %d ") % charindex);
	dbChar.SetQuery(select_character_query);

	if( !dbChar.Open() || !dbChar.MoveFirst() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildLoadRepMsg(rmsg, idname, charindex, MSG_GUILD_ERROR_OK);
		SEND_Q(rmsg, dest);

		return;
	}

	dbChar.GetRec("a_teach_type",	m_teachType);
	dbChar.GetRec("a_fame",			m_fame);
	dbChar.GetRec("a_teach_list",	bteacher);
	dbChar.GetRec("a_teach_complete", m_cntCompleteStudent);
	dbChar.GetRec("a_teach_fail",	  m_cntFailStudent);
	dbChar.GetRec("a_superstone",	  m_superstone);
	dbChar.GetRec("a_guardian",		  m_guardian);
	dbChar.GetRec("a_teach_idx", strTeachIdx);
	dbChar.GetRec("a_teach_sec", strTeachSec);

	CNetMsg::SP rmsg(new CNetMsg);

	if( m_teachType != MSG_TEACH_NO_STUDENT_TYPE )
	{
		HelperTeacherInfo(rmsg, idname, charindex, m_teachType, m_fame, bteacher, m_cntCompleteStudent,
			m_cntFailStudent, m_superstone, m_guardian, strTeachIdx, strTeachSec);
	}
	else
	{
		rmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(rmsg) << MSG_HELPER_TEACHER_LOAD_REP
			 << idname
			 << charindex
			 << 0
			 << m_guardian
			 << m_superstone
			 << m_cntFailStudent
			 << m_cntCompleteStudent
			 << m_teachType
			 << m_fame
			 << bteacher;
	}
	SEND_Q(rmsg, dest);

	CNetMsg::SP tmsg(new CNetMsg);
	HelperGuildLoadRepMsg(tmsg, idname, charindex, MSG_GUILD_ERROR_OK);
	SEND_Q(tmsg, dest);
}

void do_CommandTeacherSuperstoneRecieve(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charindex;

	RefMsg(msg) >> charindex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	std::string update_characters_query = boost::str(boost::format("UPDATE t_characters SET a_superstone = a_superstone+1 WHERE a_index = %d ") % charindex);
	cmd.SetQuery(update_characters_query);

	if( !cmd.Update() )
	{
		GAMELOG << init("TEACH_SUPERSTONE_ERROR_NOTACCESSDB")
				<< charindex
				<< end;
		return;
	}
}

void do_CommandTeacherTimeover(CNetMsg::SP& msg, CDescriptor* dest)
{
	int flag=0;
	int teachidx=0;
	int studentidx=0;

	RefMsg(msg) >> flag
		>> teachidx
		>> studentidx;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.BeginTrans();

	if( !do_TeachCancel(cmd, teachidx, studentidx, flag, true) )
	{
		cmd.Rollback();
		GAMELOG << init("TEACH_TIMEOVER_ERROR_NOTACCESSDB")
				<< flag << delim
				<< teachidx << delim
				<< studentidx << delim
				<< end;
		return;
	}
	cmd.Commit();
}

void do_CommandTeacher(CNetMsg::SP& msg, CDescriptor* dest)
{
	int teacheridx;
	char bteacher;
	unsigned char noticeTime1, noticeTime2 ;
	RefMsg(msg) >> teacheridx
		>> bteacher;
	RefMsg(msg) >> noticeTime1
		>> noticeTime2;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	std::string update_characters_query = boost::str(boost::format("UPDATE t_characters SET a_teach_list = %d, a_teach_sec=' %d %d' where a_index = %d ")
				% (int)bteacher % (int)noticeTime1 % (int)noticeTime2 % teacheridx);
	cmd.SetQuery(update_characters_query);

	if( !cmd.Update() )
	{
		GAMELOG << init("TEACH_LIST_ERROR_NOTACCESSDB")
				<< teacheridx << delim
				<< bteacher << delim
				<< end;
		return;
	}
}

void do_CommandTeacherGiveup(CNetMsg::SP& msg, CDescriptor* dest)
{
	int teachidx = 0;
	int studentidx = 0;
	int teachType;
	int GiveupCnt;

	RefMsg(msg) >> teachidx
		>> studentidx;
	RefMsg(msg) >> teachType
		>> GiveupCnt;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	cmd.BeginTrans();
	if( teachidx != -1)
	{
		if( !do_TeachCancel(cmd, teachidx, studentidx) )
		{
			cmd.Rollback();
			return;
		}
	}

	// 여기서는 학생의 teach_type을 MSG_TEACH_NO_STUDENT_TYPE로 수정하고 do_TeachCancel를 호출한다.
	std::string update_characters_query = boost::str(boost::format("UPDATE t_characters SET a_teach_type = %d, a_teach_sec ='%d' WHERE a_index = %d") % teachType % GiveupCnt % studentidx);
	cmd.SetQuery(update_characters_query);
	if( !cmd.Update() )
	{
		cmd.Rollback();
		return;
	}

	if( teachidx == -1 )
	{
		cmd.Commit();
		return;
	}

	std::string update_characters_query_1 = boost::str(boost::format("update t_characters set a_teach_fail = a_teach_fail + 1 where a_index = %d ") % teachidx);
	cmd.SetQuery(update_characters_query_1);
	if( !cmd.Update() )
	{
		cmd.Rollback();
		return ;
	}
	cmd.Commit();
}

void do_CommandTeacherRegister(CNetMsg::SP& msg, CDescriptor* dest)
{
	int teachidx = 0;
	int studentidx = 0;
	char bTeacher = 0;
	CLCString strteach(256);
	int m_teachIdx[TEACH_MAX_STUDENTS];
	int m_teachTime[TEACH_MAX_STUDENTS];
	int i;
	time_t tCurTime;
	time(&tCurTime);

	RefMsg(msg) >> teachidx
		>> bTeacher
		>> studentidx;

	// 학생 데이터베이스
	// 사제 성립시간을 견습생은 giveup 카운트로 사용한다.
	std::string update_characters_query = boost::str(boost::format("UPDATE t_characters SET a_teach_idx =' %d -1 -1 -1 -1 -1 -1 -1 -1 -1', a_teach_type = %d WHERE a_index = %d ") 
		% teachidx % MSG_TEACH_STUDENT_TYPE % studentidx);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	cmd.BeginTrans();

	cmd.SetQuery(update_characters_query);
	if( !cmd.Update() )
	{
		//등록 실패
		cmd.Rollback();
		return;
	}

	// 선생 데이터베이스
	std::string select_characters_query = boost::str(boost::format("SELECT a_teach_idx, a_teach_sec FROM t_characters WHERE a_index=%d ") % teachidx);
	cmd.SetQuery(select_characters_query);
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		cmd.Rollback();
		return;
	}

	memset(m_teachIdx, -1, sizeof(m_teachIdx) );
	memset(m_teachTime, 0, sizeof(m_teachTime) );

	cmd.GetRec("a_teach_idx", strteach);

	int cnt =0;
	int count = 0;
	bool isFirst = true;

	const char* pTeach = strteach;
	while (*pTeach)
	{
		int idx;
		char tempBuf[100] = {0,};
		pTeach = AnyOneArg(pTeach, tempBuf);
		idx = atoi(tempBuf);

		if (idx == -1 && isFirst)
		{
			m_teachIdx[cnt] = studentidx;
			count = cnt;
			isFirst = false;
		}
		else
		{
			m_teachIdx[cnt] = idx;
		}
		cnt++;
	}

	std::string update_characters_query_1 = "UPDATE t_characters SET a_teach_idx = '";
	for(i = 0; i < TEACH_MAX_STUDENTS; i++)
	{
		update_characters_query_1 += boost::str(boost::format(" %d") % m_teachIdx[i]);
	}
	update_characters_query_1 += boost::str(boost::format("', a_teach_type = %d, a_teach_list = %d WHERE a_index = %d")
		% MSG_TEACH_TEACHER_TYPE % (int)bTeacher % teachidx);

	cmd.SetQuery(update_characters_query_1);
	if( !cmd.Update() )
	{
		//등록 실패
		cmd.Rollback();
		return;
	}
	cmd.Commit();
}

bool do_TeachCancel(CDBCmd cmd, int teacheridx, int studentidx, int cancelflag, bool bTimelimit)
{
	int studentList[TEACH_MAX_STUDENTS];
	int studentTime[TEACH_MAX_STUDENTS];
	CLCString str_studentList(256);
	CLCString str_studentTime(256);
	char temp[1024];
	bool bTeacher = false;
	int i;

	memset(studentList, -1, sizeof(studentList) );
	memset(studentTime, 0, sizeof(studentTime) );
	memset(temp, '\0', sizeof(temp) );

	if(cancelflag & CANCELTEACHER)
	{
		// 선생부분업데이트-시작
		std::string select_characters_query = boost::str(boost::format("SELECT a_teach_idx, a_teach_sec FROM t_characters WHERE a_index = %d") % teacheridx);
		cmd.SetQuery(select_characters_query);
		if( !cmd.Open() || !cmd.MoveFirst() )
		{
			GAMELOG << init("TEACH_SUCCESS_FAIL-do_TeachCancel")
					<< teacheridx << delim
					<< studentidx
					<< end;
			return false;
		}

		cmd.GetRec("a_teach_idx", str_studentList);
		const char* p_list = str_studentList;

		for(i = 0; i < TEACH_MAX_STUDENTS; i++)
		{
			p_list = AnyOneArg(p_list, temp);
			studentList[i] = atoi(temp);

			if(studentList[i] == studentidx)
			{
				studentList[i] = -1;
			}

			if( studentList[i] != -1)
				bTeacher = true;
		}

		std::string tmpStudentList = "";
		std::string tmpStudentTimeList = "";
		for(i = 0; i < TEACH_MAX_STUDENTS; i++)
		{
			tmpStudentList += boost::str(boost::format(" %d") % studentList[i]);
		}

		std::string update_characters_query = boost::str(boost::format("UPDATE t_characters SET a_teach_idx = '%s' WHERE a_index = %d") % tmpStudentList.c_str() % teacheridx);

		cmd.SetQuery(update_characters_query);
		if( !cmd.Update() )
		{
			GAMELOG << init("TEACH_SUCCESS_FAIL-do_TeachCancel")
					<< teacheridx << delim
					<< studentidx
					<< end;
			return false;
		}
	}

	if(cancelflag & CANCELSTUDENT)
	{
		// 학생일때는 제자가 없으므로 그냥 업데이트
		std::string select_characters_query = boost::str(boost::format("SELECT a_teach_idx, a_teach_sec FROM t_characters WHERE a_index = %d") % studentidx );

		cmd.SetQuery(select_characters_query);
		cmd.Open();
		cmd.MoveFirst();
		CLCString buf(256);
		cmd.GetRec("a_teach_sec", buf);
		int gCnt = atoi(buf) + 1;
		int teach_type = MSG_TEACH_NO_TYPE;

		if( gCnt >= 3)
		{
			teach_type = MSG_TEACH_NO_STUDENT_TYPE;
		}
		else if( bTimelimit)
		{
			teach_type = MSG_TEACH_LIMITE_DAY_FAIL;
		}

		std::string upate_characters_query = boost::str(boost::format("UPDATE t_characters SET a_teach_idx=' -1', a_teach_type = %d, a_teach_sec=' %d' where a_index = %d ")
				 % teach_type % gCnt % studentidx);

		cmd.SetQuery(upate_characters_query);

		if(!cmd.Update())
		{
			GAMELOG << init("TEACH_SUCCESS_FAIL-do_TeachCancel")
					<< teacheridx << delim
					<< studentidx
					<< end;
			return false;
		}
	}

	return true;
}

void do_CommandFameUp(CNetMsg::SP& msg, CDescriptor* dest)
{
	int teacheridx = 0;
	int fame = 0;

	RefMsg(msg) >> teacheridx
		>> fame;

	CDBCmd cmd;

	cmd.Init(&gserver.m_dbchar);

	cmd.BeginTrans();
	// 여기서는 선생의 명성만 올라간다. 견습셍 달성시 명성치 주지 않음.
	std::string update_characters_query = boost::str(boost::format("UPDATE t_characters SET a_fame = a_fame + %d where a_index = %d ") % fame % teacheridx);
	cmd.SetQuery(update_characters_query);

	if(!cmd.Update() )
	{
		cmd.Rollback();
		return;
	}
	cmd.Commit();

	// 전 서브서버에 거시기가 명성 업 했다는 것을 보냄
	CNetMsg::SP rmsg(new CNetMsg);
	HelperFameupRepMsg(rmsg, teacheridx, -1, NULL, fame);
	gserver.SendToAllGameServer(rmsg);

	GAMELOG << init("TEACH_SUCCESS_FAMEUP")
				<< teacheridx << delim
				<< fame
				<< end;
}

void do_CommandNameChange(CNetMsg::SP& msg, CDescriptor* dest)
{
	char bguild = 0;
	int index = 0;
	CLCString name(256);

	RefMsg(msg) >> bguild
		>> index
		>> name;

	CDBCmd cmd;

	cmd.Init(&gserver.m_dbchar);

	if(bguild == 1)
	{
		std::string update_guild_query = boost::str(boost::format("UPDATE t_guild SET a_name = '%s' WHERE a_index = %d") % (const char*)name % index);
		cmd.SetQuery(update_guild_query);
		if(!cmd.Update())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_DUPLICATE);
			SEND_Q(rmsg, dest);
			return;
		}

		CGuild *guild = gserver.m_guildlist.findguild(index);
		guild->changeName(name);
		// 변경되었다는 메세지 보냄
	}
	// 3, 4는 롤백
	else if(bguild == 3)
	{
		std::string update_guild_query = boost::str(boost::format("UPDATE t_guild SET a_name = '%s' WHERE a_index = %d") % (const char*)name % index);
		cmd.SetQuery(update_guild_query);
		cmd.Update();
		return;
	}
	else if(bguild == 4)
	{
		cmd.BeginTrans();
		std::string update_guild_query = boost::str(boost::format("UPDATE t_characters SET a_nick = '%s' WHERE a_index = %d") % (const char*)name % index);
		cmd.SetQuery(update_guild_query);
		cmd.Update();

		std::string update_guild_member_query = boost::str(boost::format("UPDATE t_guildmember SET a_char_name = '%s' WHERE a_char_index = %d") % (const char*)name % index);
		cmd.SetQuery(update_guild_member_query);
		if(!cmd.Update())
		{
			cmd.Rollback();
			return;
		}

		for(int j = 0; j < 10; j++)
		{
			CDBCmd cmd1;
			cmd1.Init(&gserver.m_dbchar);

			std::string select_friend_query = boost::str(boost::format("SELECT * FROM t_friend0%d WHERE a_friend_index = %d") % j % index);
			cmd1.SetQuery(select_friend_query);
			cmd1.Open();

			if( cmd1.m_nrecords < 1 )
				continue;

			std::string update_friend_query = boost::str(boost::format("UPDATE t_friend0%d SET a_friend_name = '%s' WHERE a_friend_index = %d") % j % (const char*)name % index);
			cmd.SetQuery(update_friend_query);
			if( !cmd.Update() )
			{
				cmd.Rollback();
				return;
			}
		}

		cmd.Commit();

		return;
	}
	else if(bguild == 5)
	{
		// 모든 서버에게 길드 이름이 바뀌었으므로 바꾸고 전 길드원에게 통보
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_SUCCESS);
		gserver.SendToAllGameServer(rmsg);
		return;
		//SEND_Q(rmsg, dest);
	}
	else
	{
		//sprintf(g_buf, "select a_name from t_characters where a_name = '%s' and a_nick = '%s' and a_index != %d ", (const char*)name, (const char*)name, index);
		std::string select_characters_query = boost::str(boost::format("SELECT a_name FROM t_characters WHERE a_name = '%s' AND a_index != %d ") % (const char*)name % index);
		cmd.SetQuery(select_characters_query);
		if( !cmd.Open() || (cmd.m_nrecords > 0))
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_DUPLICATE);
			SEND_Q(rmsg, dest);
			return;
		}

		std::string select_characters_query_1 = boost::str(boost::format("SELECT a_nick FROM t_characters WHERE a_nick = '%s' AND a_index != %d") % (const char*)name % index);
		cmd.SetQuery(select_characters_query_1);
		if( !cmd.Open() || (cmd.m_nrecords > 0))
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_DUPLICATE);
			SEND_Q(rmsg, dest);
			return;
		}

		cmd.BeginTrans();

		std::string update_characters_query = boost::str(boost::format("UPDATE t_characters SET a_nick = '%s' WHERE a_index = %d") % (const char*)name % index);
		cmd.SetQuery(update_characters_query);
		if(!cmd.Update())
		{
			cmd.Rollback();
			CNetMsg::SP rmsg(new CNetMsg);
			HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_DUPLICATE);
			SEND_Q(rmsg, dest);
			return;
		}

		std::string update_guild_member_query = boost::str(boost::format("UPDATE t_guildmember SET a_char_name = '%s' WHERE a_char_index = %d") % (const char*)name % index);
		cmd.SetQuery(update_guild_member_query);
		if(!cmd.Update())
		{
			cmd.Rollback();
			CNetMsg::SP rmsg(new CNetMsg);
			HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_DUPLICATE);
			SEND_Q(rmsg, dest);
			return;
		}

		for(int j = 0; j < 10; j++)
		{
			CDBCmd cmd1;
			cmd1.Init(&gserver.m_dbchar);

			std::string select_friend_query = boost::str(boost::format("SELECT * FROM t_friend0%d WHERE a_friend_index = %d") % j % index);
			cmd1.SetQuery(select_friend_query);
			cmd1.Open();

			if( cmd1.m_nrecords < 1 )
				continue;

			std::string updadte_friend_query = boost::str(boost::format("UPDATE t_friend0%d SET a_friend_name = '%s' WHERE a_friend_index = %d") % j % (const char*)name % index);
			cmd.SetQuery(updadte_friend_query);
			if( !cmd.Update() )
			{
				cmd.Rollback();
				CNetMsg::SP rmsg(new CNetMsg);
				HelperNameChangeRepMsg(rmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_HELPER);
				SEND_Q(rmsg, dest);
				return;
			}
		}
		cmd.Commit();

		CGuildMember* gMember = gserver.m_guildlist.findmember(index);

		if( gMember )
			gMember->name(name);
			//member nick부분 삭제 : bw 060403
			//gMember->nick(name);

		// 파티 매칭 업데이트
		CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(dest->m_subNo, index);
		if (pMatchMember)		pMatchMember->SetName(name);
		CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, index);
		if (pMatchParty)		pMatchParty->SetBossName(name);
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchMemberChangeInfoMsg(rmsg, index, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_NAME, name, 0, 0);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);

		// 변경되었다는 메세지 보냄
	}

	CNetMsg::SP tmsg(new CNetMsg);
	HelperNameChangeRepMsg(tmsg, bguild, index, name, MSG_EX_NAMECHANGE_ERROR_SUCCESS);
	SEND_Q(tmsg, dest);
}

///////////////////////////////////////////
// Rank DB control in personal dungeon 4(차원의 문) : bw
//
void do_CommandPD4RewardEnd(CNetMsg::SP& msg, CDescriptor* dest)
{
	CDBCmd cmd;

	// 보상 DB의 일주일 전 레코드를 삭제한다.
	cmd.Init(&gserver.m_dbchar);

	std::string delete_pd4reward_query = "DELETE FROM t_pd4reward WHERE date_add(a_adddate, interval 6 day) < curdate();";
	cmd.SetQuery(delete_pd4reward_query);

	if( !cmd.Update() )
	{
		GAMELOG << init("QUEST REWARD DB END FAIL");
		return;
	}
}
void do_CommandPD4RewardInit(CNetMsg::SP& msg, CDescriptor* dest)
{
	CDBCmd cmd;

	cmd.Init(&gserver.m_dbchar);

	std::string update_pd4reward_query = "UPDATE t_pd4reward SET a_breward = 0 WHERE a_breward = 2 AND date_add(a_adddate, interval 6 day) > curdate();";
	cmd.SetQuery(update_pd4reward_query);

	if( !cmd.Update() )
	{
		GAMELOG << init("QUEST REWARD DB INIT FAIL") << end ;
		return;
	}
}

void do_CommandPD4Reward(CNetMsg::SP& msg, CDescriptor* dest)
{
	//MSG_HELPER_PD4_REWARD_REQ : charindex(n)
	//MSG_HELPER_PD4_REWARD_REP : charindex(n) charrank(c) breward(c)
	char charrank = 0;
	char breward = 1;
	int charindex;

	RefMsg(msg) >> charindex;

	CDBCmd rewardDBcmd;
	rewardDBcmd.Init(&gserver.m_dbchar);

	std::string select_pd4reward_query = boost::str(boost::format(
		"SELECT a_rank, a_breward FROM t_pd4reward WHERE a_index =%d AND date_add(a_adddate, interval 6 day) > curdate()") % charindex);

	rewardDBcmd.SetQuery(select_pd4reward_query);

	if( !rewardDBcmd.Open() )
	{
		CNetMsg::SP pmsg(new CNetMsg);
		pmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(pmsg) << MSG_HELPER_PD4_REWARD_REP;
		SEND_Q(pmsg, dest);
		return;
	}
	if( !rewardDBcmd.MoveFirst() )
	{
		CNetMsg::SP pmsg(new CNetMsg);
		pmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(pmsg) << MSG_HELPER_PD4_REWARD_REP
			<< charindex
			<< (char) 0
			<< (char) 1;
		SEND_Q(pmsg, dest);
		return;
	}

	if( !rewardDBcmd.GetRec("a_rank", charrank)
		|| !rewardDBcmd.GetRec("a_breward", breward))
	{
		CNetMsg::SP pmsg(new CNetMsg);
		pmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(pmsg) << MSG_HELPER_PD4_REWARD_REP
			<< charindex
			<< charrank
			<< (char) 1;
		SEND_Q(pmsg, dest);
		return;
	}

	rewardDBcmd.Init(&gserver.m_dbchar);

	std::string update_pd4reward_query = boost::str(boost::format(
		"UPDATE t_pd4reward SET a_breward = 1 WHERE a_index= %d AND date_add(a_adddate, interval 6 day) > curdate();") % charindex);

	rewardDBcmd.SetQuery(update_pd4reward_query);
	if(!rewardDBcmd.Update())
	{
		return;
	}

	CNetMsg::SP pmsg(new CNetMsg);
	pmsg->Init(MSG_HELPER_COMMAND);
	RefMsg(pmsg) << MSG_HELPER_PD4_REWARD_REP
		<< charindex
		<< charrank
		<< breward;
	SEND_Q(pmsg, dest);
}
void do_CommandPD4RankView(CNetMsg::SP& msg, CDescriptor* dest)
{
	//MSG_HELPER_PD4_RANK_VIEW_REQ : charindex(n) charjob(c)
	//MSG_HELPER_PD4_RANK_VIEW_REP : charindex(n) multicount(c), charname(str), bclear(c), deadmon(n), ctime(ll);
	int charindex;
	char charjob;
	char multicount;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	char bclear;
	int deadmon;
	LONGLONG ctime;
#ifdef NPC_CHECK
	int npcIndex;

	RefMsg(msg) >> charindex
		>> charjob
		>> npcIndex;
#else
	RefMsg(msg) >> charindex
		>> charjob;
#endif

	CDBCmd cmd;

	cmd.Init(&gserver.m_dbchar);

	std::string select_pd4rank_query = boost::str(boost::format(
		"SELECT a_name, a_bclear, a_deadmon, a_ctime FROM t_pd4rank WHERE a_etime != '' AND a_job = %d AND a_stime < a_etime order by a_bclear desc, a_deadmon desc, a_ctime asc, a_level asc, a_exp asc, a_index asc LIMIT 5")
		% (int)charjob);

	cmd.SetQuery(select_pd4rank_query);
	if( !cmd.Open() )
	{
		CNetMsg::SP pmsg(new CNetMsg);
		pmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(pmsg) << MSG_HELPER_PD4_RANK_VIEW_REP
			<< charindex
#ifdef NPC_CHECK
			<< npcIndex
#endif
			<< (char) 0;

		SEND_Q(pmsg, dest);
		return;
	}
	if( !cmd.MoveFirst() )
	{
		CNetMsg::SP pmsg(new CNetMsg);
		pmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(pmsg) << MSG_HELPER_PD4_RANK_VIEW_REP
			<< charindex
#ifdef NPC_CHECK
			<< npcIndex
#endif
			<< (char) 0;
		SEND_Q(pmsg, dest);
		return;
	}

	multicount = 1;
	while(cmd.MoveNext()) multicount++;

	if( !cmd.MoveFirst() )
	{
		CNetMsg::SP pmsg(new CNetMsg);
		pmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(pmsg) << MSG_HELPER_PD4_RANK_VIEW_REP
			<< charindex
#ifdef NPC_CHECK
			<< npcIndex
#endif
			<< (char) 0;
		SEND_Q(pmsg, dest);
		return;
	}

	CNetMsg::SP pmsg(new CNetMsg);
	pmsg->Init(MSG_HELPER_COMMAND);
	RefMsg(pmsg) << MSG_HELPER_PD4_RANK_VIEW_REP
		<< charindex
#ifdef NPC_CHECK
		<< npcIndex
#endif
		<< multicount;

	for(int i = 0; i < multicount; i++)
	{
		if( !cmd.GetRec("a_name", charname)
			|| !cmd.GetRec("a_bclear", bclear)
			|| !cmd.GetRec("a_deadmon", deadmon)
			|| !cmd.GetRec("a_ctime", ctime) )
		{
			pmsg->Init(MSG_HELPER_COMMAND);
			RefMsg(pmsg) << MSG_HELPER_PD4_RANK_VIEW_REP
				<< charindex
#ifdef NPC_CHECK
				<< npcIndex
#endif
				<< (char) 0;
			SEND_Q(pmsg, dest);
			return;
		}

		RefMsg(pmsg) << charname
			<< bclear
			<< deadmon
			<< ctime;

		cmd.MoveNext();
	}
	SEND_Q(pmsg, dest);
}
void do_CommandPD4RewardView(CNetMsg::SP& msg, CDescriptor* dest)
{
	//MSG_HELPER_PD4_REWARD_VIEW_REQ : charindex(n) charjob(c)
	//MSG_HELPER_PD4_REWARD_VIEW_REP : charindex(n) multicount(c) charname(str) breward(c)
	char multicount;
	int charindex;
	char charjob;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	char breward;
#ifdef NPC_CHECK
	int npcIndex;

	RefMsg(msg) >> charindex
		>> charjob
		>> npcIndex;
#else
	RefMsg(msg) >> charindex
		>> charjob;
#endif

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	std::string select_pd4reward_query = boost::str(boost::format(
		"SELECT a_name, a_breward FROM t_pd4reward WHERE a_job = %d AND date_add(a_adddate, interval 6 day) > curdate() order by a_rank;") % charjob);
	cmd.SetQuery(select_pd4reward_query);

	if( !cmd.Open() )
	{
		CNetMsg::SP pmsg(new CNetMsg);
		pmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(pmsg) << MSG_HELPER_PD4_REWARD_VIEW_REP
			<< charindex
#ifdef NPC_CHECK
			<< npcIndex
#endif
			<< (char) 0;
		SEND_Q(pmsg, dest);
		return;
	}
	if( !cmd.MoveFirst() )
	{
		CNetMsg::SP pmsg(new CNetMsg);
		pmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(pmsg) << MSG_HELPER_PD4_REWARD_VIEW_REP
			<< charindex
#ifdef NPC_CHECK
			<< npcIndex
#endif
			<< (char) 0;
		SEND_Q(pmsg, dest);
		return;
	}

	multicount = 1;
	while(cmd.MoveNext()) multicount++;

	CNetMsg::SP pmsg(new CNetMsg);
	pmsg->Init(MSG_HELPER_COMMAND);
	RefMsg(pmsg) << MSG_HELPER_PD4_REWARD_VIEW_REP
		<< charindex
#ifdef NPC_CHECK
		<< npcIndex
#endif
		<< multicount;

	cmd.MoveFirst();

	for(int i = 0; i < multicount; i++)
	{
		if( !cmd.GetRec("a_name", charname)
			|| !cmd.GetRec("a_breward", breward) )
		{
			pmsg->Init(MSG_HELPER_COMMAND);
			RefMsg(pmsg) << MSG_HELPER_PD4_REWARD_VIEW_REP
				<< charindex
#ifdef NPC_CHECK
				<< npcIndex
#endif
				<< (char) 0;
			SEND_Q(pmsg, dest);
			return;
		}

		RefMsg(pmsg) << charname;

		if(!breward)
			RefMsg(pmsg) << breward;
		else
			RefMsg(pmsg) << (char) 1;

		cmd.MoveNext();
	}

	SEND_Q(pmsg, dest);
}

void do_CommandPD4RankInit(CNetMsg::SP& msg, CDescriptor* dest)
{
	//MSG_HELPER_PD4_RANK_INIT,			// pd4 rank DB init

	////////////////////////////////////////
	// 보상 DB로 백업 부분 추가
	//
	int charindex;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	char charjob = 0;

	CDBCmd rankCmd, rewardCmd;

	while(charjob < JOBCOUNT)
	{
		rankCmd.Init(&gserver.m_dbchar);

		//select a_name, a_bclear, a_deadmon, a_ctime from t_pd4rank where a_job = %d order by a_bclear desc, a_deadmon desc, a_ctime desc, a_level desc, a_exp desc, a_index desc LIMIT 5
		std::string select_pd4rank_query = boost::str(boost::format(
			"SELECT a_index, a_name FROM t_pd4rank WHERE a_etime != '' AND a_job = %d order by a_bclear desc, a_deadmon desc, a_ctime asc, a_level asc, a_exp asc, a_index asc LIMIT 5") % charjob);

		rankCmd.SetQuery(select_pd4rank_query);
		if( !rankCmd.Open() )
		{
			charjob++;
			continue;
		}

		if( !rankCmd.MoveFirst() )
		{
			charjob++;
			continue;
		}

		char rank = 0;

		do
		{
			rank++;

			if(!rankCmd.GetRec("a_index", charindex)
				|| !rankCmd.GetRec("a_name", charname))
			{
				continue;
			}
			rewardCmd.Init(&gserver.m_dbchar);
#ifdef LC_GAMIGO
			std::string select_pd4reward_query = boost::str(boost::format("SELECT * FROM t_pd4reward WHERE a_index = %d AND a_adddate = now()") % charindex);
			rewardCmd.SetQuery(select_pd4reward_query);
			if(rewardCmd.Open() && rewardCmd.m_nrecords > 0)
				continue;
#endif
			std::string insert_pd4reward_query = boost::str(boost::format(
				"INSERT INTO t_pd4reward (a_index, a_rank, a_name, a_job, a_breward, a_adddate ) VALUES ( %d, %d, '%s', %d, 2, now())") % charindex % rank % (const char*)charname % charjob);

			rewardCmd.SetQuery(insert_pd4reward_query);
			rewardCmd.Update();
			GAMELOG << init("PD4_RANK")
					<< rank << delim
					<< charindex << delim
					<< charname << delim
					<< charjob << end;
		} while(rankCmd.MoveNext());

		charjob++;
	}

	rankCmd.Init(&gserver.m_dbchar);
	
	std::string truncate_query = "TRUNCATE  TABLE t_pd4rank";
	rankCmd.SetQuery(truncate_query);
	if ( !rankCmd.Update() )
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB INIT FAILED") << end;
		return;
	}
}

void do_CommandPD4RankAdd(CNetMsg::SP& msg, CDescriptor* dest)
{
	/////////////////////////////////////////////
	// BANGWALL : 2005-06-23 오전 11:41:59
	// Comment : Quest Start time

	//MSG_HELPER_PD4_RANK_ADD,			// pd4 rank 시작	 : charIndex(n)
	// index, name, exp, stime, job, level
	int charindex;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString charnick(MAX_CHAR_NAME_LENGTH + 1);
	LONGLONG charexp;
	char charjob;
	int charlevel;

	RefMsg(msg) >> charindex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	std::string select_characters_query = boost::str(boost::format("SELECT a_name, a_nick, a_exp, a_job, a_level FROM t_characters WHERE a_index = %d and a_admin = 0") % charindex);

	cmd.SetQuery(select_characters_query);
	if( !cmd.Open() )
	{
		GAMELOG << init("PD4 RANK ADD ERROR")
				<< charindex;
		return;
	}
	if( !cmd.MoveFirst() )
	{
		GAMELOG << init("PD4 RANK ADD ERROR")
				<< charindex;
		return;
	}

	if( !cmd.GetRec("a_name", charname)
		||!cmd.GetRec("a_nick", charnick)
		||!cmd.GetRec("a_exp", charexp)
		||!cmd.GetRec("a_job", charjob)
		||!cmd.GetRec("a_level", charlevel) )
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB ADD FAILED")
				<< charname << end;
		return;
	}

	cmd.Init(&gserver.m_dbchar);

	std::string insert_pd4rank_query = "";
	if( strcmp(charnick, "") != 0 )
	{
		insert_pd4rank_query = boost::str(boost::format(
			"INSERT INTO t_pd4rank "
			"(a_index, a_name, a_exp, a_level, a_job, a_stime, a_ctime ) "
			" VALUES "
			" (%1%, '%2%', %3%, %4%, %5%, now(), 0 )")
			% charindex % (const char*)charnick % charexp % charlevel % charjob );
	}
	else
	{
		insert_pd4rank_query = boost::str(boost::format(
			 "INSERT INTO t_pd4rank "
			 " (a_index, a_name, a_exp, a_level, a_job, a_stime, a_ctime ) VALUES "
			 "(%1%, '%2%', %3%, %4%, %5%, now(), 0 )")
			 % charindex % (const char*)charname % charexp % charlevel % charjob );

	}

	cmd.SetQuery(insert_pd4rank_query);
	cmd.Update();

	if( cmd.m_nrecords < 1)
	{
		/////////////////
		// 초기화 후에 이미 디비에 있으면 update
		//
		std::string update_pd4randk = "";
		if( strcmp(charnick, "") != 0)
		{
			update_pd4randk = boost::str(boost::format(
				"UPDATE t_pd4rank set a_name='%1%', a_exp=%2%, a_level=%3%, a_job=%4%, a_stime=now() WHERE a_index=%5%")
				% (const char*)charnick % charexp % charlevel % charjob % charindex);
		}
		else
		{
			update_pd4randk = boost::str(boost::format(
				"UPDATE t_pd4rank SET a_name='%1%', a_exp=%2%, a_level=%3%, a_job=%4%, a_stime=now() where a_index=%5%")
				% (const char*)charname % charexp % charlevel % charjob % charindex);
		}

		cmd.SetQuery(update_pd4randk);
		if(!cmd.Update())
		{
			GAMELOG << init("PERSONAL DUNGEON 4 RANK DB ADD FAILED")
					<< charname << end;

			return;
		}
	}
}

void do_CommandPD4RankEnd(CNetMsg::SP& msg, CDescriptor* dest)
{
	//MSG_HELPER_PD4_RANK_END,			// pd4 종료			 : charIndex(n) deadmonNum(n) bclear(c)
	//index, etime, deadmonNum, bclear

	int charindex;
	int deadmonNum=0, predeadmonNum=0;
	char bclear=0, prebclear=0;
	LONGLONG ctime=0, prectime=0;

	// 이전과 비교하여 최대 기록이면 수정 그렇지 않으면 return;
	// bclear, ctime(stime), deadmon
	RefMsg(msg) >> charindex
		>> deadmonNum
		>> bclear;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	std::string update_pd4rank_query = boost::str(boost::format("UPDATE t_pd4rank SET a_etime = now() WHERE a_index = %d") % charindex);
	cmd.SetQuery(update_pd4rank_query);

	if(!cmd.Update())
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB END FAILED1")
				<< charindex << end;
		return;
	}

	cmd.Init(&gserver.m_dbchar);

	std::string select_pd4rank_query = boost::str(boost::format(
		"SELECT a_deadmon, a_bclear, a_ctime, (UNIX_TIMESTAMP(a_etime) - UNIX_TIMESTAMP(a_stime)) AS a_nctime FROM t_pd4rank WHERE a_index = %d") % charindex);
	cmd.SetQuery(select_pd4rank_query);

	if(!cmd.Open())
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB END FAILED2")
				<< charindex << end;
		return;
	}
	if(!cmd.MoveFirst())
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB END FAILED3")
				<< charindex << end;
		return;
	}

	if(!cmd.GetRec("a_deadmon", predeadmonNum)
		||!cmd.GetRec("a_bclear", prebclear)
		||!cmd.GetRec("a_ctime", prectime)
		||!cmd.GetRec("a_nctime", ctime))
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB END FAILED4")
				<< charindex << end;
		return;
	}

	GAMELOG << init("PD4 END")
			<< charindex << delim
			<< deadmonNum << delim
			<< bclear << delim
			<< ctime << end;

	if( (prebclear > bclear)
		|| ( (prebclear == bclear) && (predeadmonNum > deadmonNum) )
		|| ( (prebclear == bclear) && (predeadmonNum == deadmonNum) && (prectime != 0 && prectime <= ctime) ) )
	{
		return;
	}

	cmd.Close();

	cmd.Init(&gserver.m_dbchar);

	//UPDATE t_pd4rank SET a_etime= '2005-06-13 18:50:11', a_bclear= 1, a_deadmon= 210 WHERE a_index=23

	std::string update_pd4rank_query_1 = boost::str(boost::format(
		"UPDATE t_pd4rank SET a_bclear=%1%, a_deadmon=%2%, a_ctime=%3% WHERE a_index=%4%")
		% (int)bclear % deadmonNum % ctime % charindex);

	cmd.SetQuery(update_pd4rank_query_1);
	if( !cmd.Update())
	{
		GAMELOG << init("PERSONAL DUNGEON 4 RANK DB END FAILED5")
				<< charindex << end;
		return;
	}
}

void do_CommandGuildCreateReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charindex;
	CLCString charname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString name(MAX_GUILD_NAME_LENGTH + 1);
	RefMsg(msg) >> charindex
		>> charname
		>> name;

	int guildmaxmember;
	int zoneindex;
	RefMsg(msg) >> guildmaxmember
		>> zoneindex;
	// 기존 구성에서 검사
	CGuildMember* prev = gserver.m_guildlist.findmember(charindex);
	if (prev == NULL)
	{
		// 이전것 없으면 새로 독립길드 창설

		// DB 에 저장
#ifdef LC_GAMIGO
		std::string select_guild_query = boost::str(boost::format("SELECT a_name FROM t_guild WHERE a_name = '%s'") % (const char*)name);
		CDBCmd checkcmd;
		checkcmd.Init(&gserver.m_dbchar);
		checkcmd.SetQuery(select_guild_query);
		if(checkcmd.Open() && checkcmd.m_nrecords > 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_DUPNAME);
			SEND_Q(rmsg, dest);
			return ;
		}
#endif
		std::string insert_guild_query = boost::str(boost::format(
			"INSERT INTO t_guild (a_name,a_createdate,a_recentdate) VALUES ('%s',NOW(),NOW())") % (const char*)name);

		CDBCmd cmd;
		cmd.Init(&gserver.m_dbchar);
		cmd.BeginTrans();
		cmd.SetQuery(insert_guild_query);
		if (!cmd.Update())
		{
			cmd.Rollback();
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_DUPNAME);
			SEND_Q(rmsg, dest);
			return ;
		}
		int guildindex = cmd.insertid();
#ifdef LC_GAMIGO
		std::string select_guild_member_query = boost::str(boost::format(
			"SELECT a_guild_index, a_char_index FROM t_guildmember WHERE a_guild_index = %d AND a_char_index = %d") % guildindex % charindex);
		checkcmd.SetQuery(select_guild_member_query);
		if(checkcmd.Open() && checkcmd.m_nrecords > 0)
		{
			cmd.Rollback();
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);
			return ;
		}
#endif
		std::string insert_guild_member_query = boost::str(boost::format(
			"INSERT INTO t_guildmember (a_guild_index,a_char_index,a_char_name,a_pos,a_regdate) VALUES "
			" (%d, %d, '%s', %d, NOW())")
			% guildindex % charindex % (const char*)charname % MSG_GUILD_POSITION_BOSS);
		cmd.SetQuery(insert_guild_member_query);
		if (!cmd.Update())
		{
			cmd.Rollback();
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);
			return ;
		}
		int currentTime = (int)time(NULL);
#ifdef LC_GAMIGO
		std::string select_extend_guild = boost::str(boost::format("SELECT a_guild_index FROM t_extend_guild WHERE a_guild_index = %d") % guildindex);
		checkcmd.SetQuery(select_extend_guild);
		if (checkcmd.Open() && checkcmd.m_nrecords > 0)
		{
			cmd.Rollback();
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_DUPNAME);
			SEND_Q(rmsg, dest);
			return ;
		}
#endif
		std::string insert_extend_guild_query = boost::str(boost::format(
			"INSERT INTO t_extend_guild "
			" (a_guild_index, a_guild_point, a_guild_maxmember, a_guild_incline, a_lastdate_change_boss ) VALUES "
			"( %d, 0, %d, 0, %d )")
			% guildindex % guildmaxmember % currentTime );
		cmd.SetQuery(insert_extend_guild_query);
		if (!cmd.Update())
		{
			cmd.Rollback();
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_DUPNAME);
			SEND_Q(rmsg, dest);
			return ;
		}

#ifdef DEV_GUILD_STASH		// 길드 생성시 길드장은 창고이용가능
#ifdef LC_GAMIGO
		std::string select_extend_guild_member_query = boost::str(boost::format(
			"SELECT a_guild_index, a_char_index FROM t_extend_guildmember WHERE a_guild_index = %d AND a_char_index = %d")
			% guildindex % charindex);
		checkcmd.SetQuery(select_extend_guild_member_query);
		if (checkcmd.Open() && checkcmd.m_nrecords > 0)
		{
			cmd.Rollback();
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);
			return ;
		}
#endif
		std::string insert_extend_guild_member_query = boost::str(boost::format(
			"INSERT INTO t_extend_guildmember (a_guild_index, a_char_index , a_stash_auth ) VALUES "
			"(%d, %d, 1)")
			% guildindex % charindex );
#else
		std::string insert_extend_guild_member_query = boost::str(boost::format(
			"INSERT INTO t_extend_guildmember (a_guild_index, a_char_index ) VALUES "
			" (%d, %d )")
			% guildindex % charindex );
#endif //DEV_GUILD_STASH
		cmd.SetQuery(insert_extend_guild_member_query);
		if (!cmd.Update())
		{
			cmd.Rollback();
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);
			return ;
		}

		// 메모리에 만들기
		CGuild* nguild = gserver.m_guildlist.create(guildindex, name, charindex, charname);

		if (!nguild)
		{
			cmd.Rollback();
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);
			return ;
		}
		nguild->incline( 0 );
		nguild->setMaxmember(guildmaxmember);
		nguild->boss()->zoneindex( zoneindex );

		cmd.Commit();

		GAMELOG << init("GUILD CREATE")
				<< guildindex << delim
				<< name << delim
				<< "BOSS" << delim
				<< charindex << delim
				<< charname << delim
				<< end;

		// 리스트에 추가
		gserver.m_guildlist.add(nguild);
		// 멤버 온라인
		nguild->boss()->online(1);

		{
			// 리스트 추가 결과 알리기
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildCreateNotifyMsg(rmsg, nguild);
			gserver.SendToAllGameServer(rmsg);
		}

		{
			// 온라인 알리기
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildOnlineNotifyMsg(rmsg, nguild->boss());
			RefMsg(rmsg) << zoneindex;
			gserver.SendToAllGameServer(rmsg);
		}

		{
			// 성공 응답
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_OK);
			SEND_Q(rmsg, dest);
		}
	}
	else
	{
		// TODO : GUILD : 트리구성시
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildCreateRepMsg(rmsg, charindex, MSG_GUILD_ERROR_PACKET);
		SEND_Q(rmsg, dest);
	}
}

//XX endgame - 4 end
void do_CommandShutdown(CNetMsg::SP& msg, CDescriptor* dest)
{
	gserver.m_bshutdown = true;

	PrintExcuteInfo::Instance()->SetStopMessage("shutdown");
	bnf::instance()->Stop();
}

void do_CommandGuildOnline(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex;
	int charindex;
	char online;
	RefMsg(msg) >> guildindex
		>> charindex
		>> online;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (guild == NULL)
		return ;

	CGuildMember* member = guild->findmember(charindex);
	if (member)
	{
		member->online(online);
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildOnlineNotifyMsg(rmsg, member);
		int zoneNum ;
		RefMsg(msg) >> zoneNum;

		GAMELOG << init("do_CommandGuildOnline")
				<< dest->m_subNo << delim
				<< guild->index() << delim
				<< charindex << delim
				<< zoneNum << delim
				<< online << end;

		member->zoneindex( zoneNum );

		RefMsg(rmsg) << member->GetZoneNum();
		gserver.SendToAllGameServer(rmsg);
	}
}

void do_CommandGuildLoadReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charindex;
	CLCString idname(MAX_ID_NAME_LENGTH + 1);
	RefMsg(msg) >> idname >> charindex;

	int channel;
	int zoneindex;
	RefMsg(msg) >> channel >> zoneindex;

	CGuildMember* member = gserver.m_guildlist.findmember(charindex);
	if (member)
	{
		CGuild* guild = member->guild();

		if (!guild)
			return;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildLoadNotifyMsg(rmsg, guild);
			SEND_Q(rmsg, dest);
		}

		gserver.SendGuildMemberList(guild, dest);

		CDBCmd cmd;
		CLCString sql( 1204 );
		cmd.Init( &gserver.m_dbchar );
		for(int i = 0; i < 3; i++)
		{
			if(i == 0)
				sql.Format( "SELECT a_activeskill_index, a_activeskill_level FROM t_extend_guild WHERE a_guild_index = %d", guild->index() );
			else if(i == 1)
				sql.Format( "SELECT a_passiveskill_index, a_passiveskill_level FROM t_extend_guild WHERE a_guild_index = %d", guild->index() );
			else if(i == 2)
				sql.Format( "SELECT a_etcskill_index, a_etcskill_level FROM t_extend_guild WHERE a_guild_index = %d", guild->index() );

			cmd.SetQuery( sql );
			if( !cmd.Open() || !cmd.MoveFirst() )
			{
				return;
			}
			CLCString skillIndex(256);
			CLCString skillLevel(256);
			const char* pIndex = skillIndex;
			const char* pLevel = skillLevel;
			char tbuf[256];

			int sindex[256];
			int sLevel[256];
			if(i == 0)
			{
				cmd.GetRec( "a_activeskill_index", skillIndex );
				cmd.GetRec( "a_activeskill_level", skillLevel );
			}
			else if(i == 1)
			{
				cmd.GetRec( "a_passiveskill_index", skillIndex );
				cmd.GetRec( "a_passiveskill_level", skillLevel );
			}
			else if(i == 2)
			{
				cmd.GetRec( "a_etcskill_index", skillIndex );
				cmd.GetRec( "a_etcskill_level", skillLevel );
			}

			int roopcount = 0;
			while (*pIndex && *pLevel)
			{
				int i, l;
				pIndex = AnyOneArg(pIndex, tbuf);
				i = atoi(tbuf);
				pLevel = AnyOneArg(pLevel, tbuf);
				l = atoi(tbuf);

				sindex[roopcount] = i;
				sLevel[roopcount] = l;
				roopcount++;
			}

			member->channel( channel );
			member->zoneindex( zoneindex );

			GAMELOG << init("do_CommandGuildLoadReq")
					<< dest->m_subNo << delim
					<< guild->index() << delim
					<< charindex << delim
					<< zoneindex << end;

			CNetMsg::SP rmsg(new CNetMsg);
			HelperExtendGuildLoadNotifyMsg( rmsg, guild, roopcount, sindex, sLevel, i);
			SEND_Q( rmsg, dest );
		}

		gserver.SendExtendGuildMemberList(guild, dest);


		// 길드전 중인 길드이면
		if (guild->battleState() != GUILD_BATTLE_STATE_PEACE)
		{
			CGuild* g = gserver.m_guildlist.findguild(guild->battleIndex());
			if (g)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperGuildLoadNotifyMsg(rmsg, g);
				SEND_Q(rmsg, dest);
				gserver.SendGuildMemberList(g, dest);
			}
		}
	}

	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildMarkTableMsg(rmsg);
	SEND_Q(rmsg, dest);
}

void do_CommandGuildLevelUpReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex;
	int charindex;
	RefMsg(msg) >> guildindex
		>> charindex;

	int guildmaxmember;
	RefMsg(msg) >> guildmaxmember;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (guild->level() >= GUILD_MAX_GUILD_LEVEL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_MAXLEVEL);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (guild->boss()->charindex() != charindex)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	std::string update_guild_query = boost::str(boost::format(
		"UPDATE t_guild SET a_level=a_level+1, a_recentdate=NOW() WHERE a_index = %d") % guildindex);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.BeginTrans();
	cmd.SetQuery(update_guild_query);
	if (!cmd.Update())
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return ;
	}

	int gap = 0;
	int needGP = 0;
	if( guild->level() + 1 >= GUILD_EXTEND_BASE_LEVEL )
	{
		gap = guild->level() + 1 - GUILD_EXTEND_BASE_LEVEL;
		if( gap >= 0 )
		{
			needGP		= (int)(GUILD_LEVEL7_NEED_GP * pow(1.09, gap));
		}
		else
		{
			cmd.Rollback();
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);
			return ;
		}
		std::string update_extend_guild = boost::str(boost::format(
			"UPDATE t_extend_guild SET a_guild_point = a_guild_point - %d, a_guild_maxmember = %d WHERE a_guild_index = %d")
			% needGP % guildmaxmember % guildindex  );
		cmd.SetQuery( update_extend_guild );
		if( !cmd.Update() )
		{
			cmd.Rollback();
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);
			return ;
		}
	}
	else
	{
		std::string update_extend_guild = boost::str(boost::format(
			"UPDATE t_extend_guild SET a_guild_maxmember = %d WHERE a_guild_index = %d")
			% guildmaxmember % guildindex  );
		cmd.SetQuery( update_extend_guild );
		if( !cmd.Update() )
		{
			cmd.Rollback();
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);
			return ;
		}
	}
	guild->setMaxmember( guildmaxmember );
	guild->AddGuildPoint(-needGP);
	cmd.Commit();

	GAMELOG << init("GUILD LEVELUP")
			<< guild->index() << delim
			<< guild->name() << delim
			<< guild->level()
			<< end;

	guild->LevelUp();

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildLevelUpNotifyMsg(rmsg, guildindex, guild->level());
		gserver.SendToAllGameServer(rmsg);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildLevelUpRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_OK);
		gserver.SendToAllGameServer(rmsg);
	}
}

void do_CommandGuildBreakUpReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int bossindex;
	int guildindex;

	RefMsg(msg) >> bossindex
		>> guildindex;

	do_CommandGuildBreakUpReq_real(dest, bossindex, guildindex);
}

void do_CommandGuildMemberAddReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex;
	int bossindex;
	int charindex;
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> guildindex
		>> bossindex
		>> charindex
		>> name;
	int zoneindex;
	RefMsg(msg) >> zoneindex;

	CGuildMember* guildmember = gserver.m_guildlist.findmember(charindex);
	if(guildmember)
	{
		GAMELOG << init("GUILD MEMBER ADD FAILED")
				<< dest->m_subNo << delim
				<< guildmember->guild()->index() << delim
				<< guildindex << delim
				<< bossindex << delim
				<< charindex << delim
				<< name << delim
				<< zoneindex
				<< end;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (guild->maxmember() <= guild->membercount())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_FULLMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* boss = guild->findmember(bossindex);
	if (!boss)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_TARGET_NOBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (boss->pos() == MSG_GUILD_POSITION_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_TARGET_NOBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	// DB
#ifdef LC_GAMIGO
	std::string select_guild_member_query = boost::str(boost::format(
		"SELECT a_guild_index, a_char_index FROM t_guildmember WHERE a_guild_index = %d AND a_char_index = %d") % guildindex % charindex);
	CDBCmd checkcmd;
	checkcmd.Init(&gserver.m_dbchar);
	checkcmd.SetQuery(select_guild_member_query);
	if (checkcmd.Open() && checkcmd.m_nrecords > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return ;
	}
#endif
	std::string insert_guild_member_query = boost::str(boost::format(
		"INSERT INTO t_guildmember (a_guild_index,a_char_index,a_char_name,a_pos,a_regdate) VALUES "
		"(%d,%d,'%s',%d,NOW())")
		% guildindex % charindex % (const char*)name % MSG_GUILD_POSITION_MEMBER);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.BeginTrans();
	cmd.SetQuery(insert_guild_member_query);
	if (!cmd.Update())
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return ;
	}
#ifdef LC_GAMIGO
	std::string select_extend_guild_member_query = boost::str(boost::format(
		"SELECT a_guild_index, a_char_index FROM t_extend_guildmember WHERE a_guild_index = %d AND a_char_index = %d") % guildindex % charindex);
	checkcmd.SetQuery(select_extend_guild_member_query);
	if (checkcmd.Open() && checkcmd.m_nrecords > 0)
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_FULLMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
#endif
	std::string insert_extend_guild_member_query = boost::str(boost::format(
		"INSERT INTO t_extend_guildmember (a_guild_index, a_char_index ) VALUES (%d, %d )") % guildindex % charindex );

	cmd.SetQuery(insert_extend_guild_member_query);
	if (!cmd.Update())
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_FULLMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	cmd.Commit();

	int listindex = guild->addmember(charindex, name);
	if (listindex == -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_FULLMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	std::string update_guild_query = boost::str(boost::format("UPDATE t_guild SET a_recentdate=NOW() WHERE a_index = %d") % guildindex);
	cmd.SetQuery(update_guild_query);
	cmd.Update();

	GAMELOG << init("GUILD MEMBER ADD")
			<< guild->index() << delim
			<< guild->name() << delim
			<< "AGREE" << delim
			<< boss->charindex() << delim
			<< boss->GetName() << delim
			<< "CHARACTER" << delim
			<< charindex << delim
			<< name << delim
			<< end;

	{
		CGuildMember* member = guild->member(listindex);
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddNotifyMsg(rmsg, guildindex, charindex, name, member->pos());
		member->zoneindex( zoneindex );

	GAMELOG << init("do_CommandGuildMemberAddReq") << delim
			<< guild->index() << delim
			<< zoneindex 	<< end;

		RefMsg(rmsg) << zoneindex;
		gserver.SendToAllGameServer(rmsg);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_OK);
		SEND_Q(rmsg, dest);
	}
}

void do_CommandGuildMemberOutReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex;
	int charindex;
	RefMsg(msg) >> guildindex
		>> charindex;

	do_CommandGuildMemberOutReq_real(dest, guildindex, charindex, true);
}

void do_CommandGuildMemberKickReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex;
	int bossindex;
	int charindex;

	RefMsg(msg) >> guildindex
		>> bossindex
		>> charindex;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* boss = guild->findmember(bossindex);
	if (!boss)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (boss->pos() == MSG_GUILD_POSITION_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* kick = guild->findmember(charindex);
	if (!kick)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (kick->pos() == MSG_GUILD_POSITION_BOSS)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_CANTKICKBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (boss->pos() == MSG_GUILD_POSITION_OFFICER && kick->pos() == MSG_GUILD_POSITION_OFFICER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_CANTKICKBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (!gserver.DeleteGuildMember(guildindex, guild->name(), charindex, kick->GetName(), true))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberKickNotifyMsg(rmsg, guildindex, bossindex, charindex);
		gserver.SendToAllGameServer(rmsg);
	}

	guild->DelGradeExPosCount( kick->pos());
	guild->removemember(kick);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberKickRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_OK);
		SEND_Q(rmsg, dest);
	}
}

void do_CommandGuildChangeBossReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex;
	int current;
	int change;

	RefMsg(msg) >> guildindex
		>> current
		>> change;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* boss = guild->findmember(current);
	if (!boss)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (boss->pos() != MSG_GUILD_POSITION_BOSS)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* member = guild->findmember(change);
	if (!member)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (member->pos() != MSG_GUILD_POSITION_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_CANNOT_BOSSCHANGE_FAIL);
		SEND_Q(rmsg, dest);
		return ;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	std::string select_castle_query = boost::str(boost::format("SELECT * FROM t_castle WHERE a_owner_guild_index=%d;") % guildindex);
	cmd.SetQuery( select_castle_query );
	if( cmd.Open() && cmd.m_nrecords > 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOTFOUNDMEMBER);  // 성주는 안됨
		SEND_Q(rmsg, dest);
		return ;
	}

	cmd.BeginTrans();
	std::string update_guild_member_query = boost::str(boost::format(
		"UPDATE t_guildmember SET a_pos = %d WHERE a_guild_index = %d AND a_char_index = %d") % MSG_GUILD_POSITION_MEMBER % guildindex % current);
	cmd.SetQuery(update_guild_member_query);
	if (!cmd.Update())
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}

	std::string update_guild_member_query_1 = boost::str(boost::format(
		"UPDATE t_guildmember SET a_pos = %d WHERE a_guild_index = %d AND a_char_index = %d") % MSG_GUILD_POSITION_BOSS % guildindex % change);
	cmd.SetQuery(update_guild_member_query_1);
	if (!cmd.Update() || cmd.m_nrecords < 1)
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	cmd.Commit();

	std::string update_guild_query = boost::str(boost::format("UPDATE t_guild SET a_recentdate=NOW() WHERE a_index = %d") % guildindex);
	cmd.SetQuery(update_guild_query);
	cmd.Update();

	int currentTime = (int)time(NULL);
	char update_extend_guild[MAX_MESSAGE_SIZE] = {0,};
	sprintf(update_extend_guild, "UPDATE t_extend_guild SET a_lastdate_change_boss = %d WHERE a_guild_index=%d", currentTime, guildindex);
	cmd.SetQuery(update_extend_guild);
	if (!cmd.Update() || cmd.m_nrecords < 1)
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}

	GAMELOG << init("GUILD CHANGE BOSS")
			<< guild->index() << delim
			<< guild->name() << delim
			<< boss->charindex() << delim
			<< boss->GetName()
			<< " ==> "
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

	guild->changeboss(member->listindex());

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildChangeBossNotifyMsg(rmsg, guildindex, current, change);
		gserver.SendToAllGameServer(rmsg);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildChangeBossRepMsg(rmsg, guildindex, current, change, MSG_GUILD_ERROR_OK);
		SEND_Q(rmsg, dest);
	}
}

void do_CommandGuildAppointOfficerReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex;
	int bossindex;
	int charindex;

	RefMsg(msg) >> guildindex
		>> bossindex
		>> charindex;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* boss = guild->findmember(bossindex);
	if (!boss)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (boss->pos() != MSG_GUILD_POSITION_BOSS)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (member->pos() != MSG_GUILD_POSITION_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_CANTOFFICER);
		SEND_Q(rmsg, dest);
		return ;
	}

	int i;
	for (i = 0; i < GUILD_MAX_OFFICER; i++)
	{
		if (guild->officer(i) == NULL)
			break;
	}
	if (i == GUILD_MAX_OFFICER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_FULLOFFICER);
		SEND_Q(rmsg, dest);
		return ;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	std::string update_guild_member_query = boost::str(boost::format(
		"UPDATE t_guildmember SET a_pos = %d WHERE a_guild_index = %d AND a_char_index = %d") % MSG_GUILD_POSITION_OFFICER % guildindex % charindex);
	cmd.SetQuery(update_guild_member_query);
	if (!cmd.Update())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}

	std::string update_guild_query = boost::str(boost::format("UPDATE t_guild SET a_recentdate=NOW() WHERE a_index=%d") % guildindex);
	cmd.SetQuery(update_guild_query);
	cmd.Update();

	GAMELOG << init("GUILD APPOINT OFFICER")
			<< guild->index() << delim
			<< guild->name() << delim
			<< "OFFICER" << delim
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

	guild->appoint(member->listindex());

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildAppointOfficerNotifyMsg(rmsg, guildindex, bossindex, charindex);
		gserver.SendToAllGameServer(rmsg);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildAppointOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_OK);
		SEND_Q(rmsg, dest);
	}
}

void do_CommandGuildChat(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex;
	int charindex;

	RefMsg(msg) >> guildindex
		>> charindex;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (guild)
	{
		if (guild->findmember(charindex))
		{
			gserver.SendToAllGameServer(msg);
		}
	}
}

void do_CommandGuildFireOfficerReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex;
	int bossindex;
	int charindex;

	RefMsg(msg) >> guildindex
		>> bossindex
		>> charindex;

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* boss = guild->findmember(bossindex);
	if (!boss)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (boss->pos() != MSG_GUILD_POSITION_BOSS)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	if (member->pos() != MSG_GUILD_POSITION_OFFICER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTOFFICER);
		SEND_Q(rmsg, dest);
		return ;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	std::string update_guild_member_query = boost::str(boost::format(
		"UPDATE t_guildmember SET a_pos=%d WHERE a_guild_index=%d AND a_char_index=%d") % MSG_GUILD_POSITION_MEMBER % guildindex % charindex);
	cmd.SetQuery(update_guild_member_query);
	if (!cmd.Update())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, dest);
		return ;
	}
	std::string update_guild_query = boost::str(boost::format("UPDATE t_guild SET a_recentdate=NOW() WHERE a_index=%d") % guildindex);
	cmd.SetQuery(update_guild_query);
	cmd.Update();

	GAMELOG << init("GUILD FIRE OFFICER")
			<< guild->index() << delim
			<< guild->name() << delim
			<< "OFFICER" << delim
			<< member->charindex() << delim
			<< member->GetName()
			<< end;

	guild->fire(member->listindex());

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildFireOfficerNotifyMsg(rmsg, guildindex, bossindex, charindex);
		gserver.SendToAllGameServer(rmsg);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildFireOfficerRepMsg(rmsg, guildindex, bossindex, charindex, MSG_GUILD_ERROR_OK);
		SEND_Q(rmsg, dest);
	}
}

void do_CommandCharacterDelete(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charindex;
	RefMsg(msg) >> charindex;

	CGuildMember* member = gserver.m_guildlist.findmember(charindex);
	if (!member)
		return ;

	CGuild* guild = member->guild();
	if (guild->membercount() < 2)
	{
		// 1인 길드 삭제 : 멤버를 DB에서만 삭제
		gserver.DeleteGuildMember(guild->index(), guild->name(), charindex, member->GetName(), false);
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberOutNotifyMsg(rmsg, guild->index(), charindex, member->GetName());
		gserver.SendToAllGameServer(rmsg);
		guild->removemember(member);
		return;
	}
	else if (member->pos() == MSG_GUILD_POSITION_BOSS)
	{
		// 길드장 이임
		CGuildMember* newboss = NULL;

		// 부관1 검사
		if (guild->officer(0) != NULL)
			newboss = guild->officer(0);

		// 부관2 검사
		else if (guild->officer(1) != NULL)
			newboss = guild->officer(1);

		// 일반 길원 검사
		else
		{
			int i;
			int memberCount = guild->maxmember();
			for (i = 0; i < memberCount; i++)
			{
				newboss = guild->member(i);
				if (newboss && newboss != member)
					break;
			}
		}

		// 아무도 안되면 그냥 삭제
		if (newboss == NULL)
		{
			gserver.DeleteGuildMember(guild->index(), guild->name(), charindex, member->GetName(), false);
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildMemberOutNotifyMsg(rmsg, guild->index(), charindex, member->GetName());
			gserver.SendToAllGameServer(rmsg);
			guild->removemember(member);
			return ;
		}

		// 선택된 길원을 보스로 : 오류 발생시 길드멤버에서 삭제

		CDBCmd cmd;
		cmd.Init(&gserver.m_dbchar);
		cmd.BeginTrans();

		std::string update_guild_member_query = boost::str(boost::format(
			"UPDATE t_guildmember SET a_pos=%d WHERE a_guild_index=%d AND a_char_index=%d") % MSG_GUILD_POSITION_MEMBER % guild->index() % member->charindex());
		cmd.SetQuery(update_guild_member_query);
		if (!cmd.Update())
		{
			cmd.Rollback();
			gserver.DeleteGuildMember(guild->index(), guild->name(), charindex, member->GetName(), false);
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildMemberOutNotifyMsg(rmsg, guild->index(), charindex, member->GetName());
			gserver.SendToAllGameServer(rmsg);
			guild->removemember(member);
			return ;
		}

		std::string update_guild_member_query_1 = boost::str(boost::format(
			"UPDATE t_guildmember SET a_pos=%d WHERE a_guild_index=%d AND a_char_index=%d") % MSG_GUILD_POSITION_BOSS % guild->index() % newboss->charindex());
		cmd.SetQuery(update_guild_member_query_1);
		if (!cmd.Update() || cmd.m_nrecords < 1)
		{
			cmd.Rollback();
			gserver.DeleteGuildMember(guild->index(), guild->name(), charindex, member->GetName(), false);
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildMemberOutNotifyMsg(rmsg, guild->index(), charindex, member->GetName());
			gserver.SendToAllGameServer(rmsg);
			guild->removemember(member);
			return ;
		}
		cmd.Commit();

		std::string update_guild_query = boost::str(boost::format("UPDATE t_guild SET a_recentdate=NOW() WHERE a_index=%d") % guild->index());
		cmd.SetQuery(update_guild_query);
		cmd.Update();

		GAMELOG << init("GUILD CHANGE BOSS")
				<< guild->index() << delim
				<< guild->name() << delim
				<< member->charindex() << delim
				<< member->GetName()
				<< " ==> "
				<< newboss->charindex() << delim
				<< newboss->GetName()
				<< end;

		guild->changeboss(newboss->listindex());

		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildChangeBossNotifyMsg(rmsg, guild->index(), member->charindex(), newboss->charindex());
		gserver.SendToAllGameServer(rmsg);
	}
	// 길드 탈퇴
	do_CommandGuildMemberOutReq_real(dest, member->guild()->index(), member->charindex(), false);
}

void do_CommandGuildBreakUpReq_real(CDescriptor* dest, int bossindex, int guildindex)
{
	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if( !guild )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildBreakUpRepMsg(rmsg, bossindex, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, dest);
		return ;
	}

	if ( guild->boss()->charindex() != bossindex)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildBreakUpRepMsg(rmsg, bossindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 길드전 중
	if (guild->battleState() != GUILD_BATTLE_STATE_PEACE)
		return;

	// DB
	if (!gserver.DeleteGuild(guildindex))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildBreakUpRepMsg(rmsg, bossindex, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, dest);
		return ;
	}

	GAMELOG << init("GUILD BREAK")
			<< guild->index() << delim
			<< guild->name()
			<< end;

	// TODO : GUILD : 하위길드 독립

	gserver.m_guildlist.Remove(guild);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildBreakUpNotifyMsg(rmsg, guildindex);
		gserver.SendToAllGameServer(rmsg);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildBreakUpRepMsg(rmsg, bossindex, MSG_GUILD_ERROR_OK);
		SEND_Q(rmsg, dest);
	}
}

void do_CommandGuildMemberOutReq_real(CDescriptor* dest, int guildindex, int charindex, bool bSendError)
{
	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		if (bSendError)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildMemberOutRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_NOGUILD);
			SEND_Q(rmsg, dest);
		}
		return ;
	}

	if (guild->boss()->charindex() == charindex)
	{
		if (bSendError)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildMemberOutRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_OUTBOSS);
			SEND_Q(rmsg, dest);
		}
		return ;
	}

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
	{
		if (bSendError)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildMemberOutRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
			SEND_Q(rmsg, dest);
		}
		return ;
	}
	if (member->pos() == MSG_GUILD_POSITION_BOSS)
	{
		if (bSendError)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildMemberOutRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_OUTBOSS);
			SEND_Q(rmsg, dest);
		}
		return ;
	}

	if (!gserver.DeleteGuildMember(guildindex, guild->name(), charindex, member->GetName(), false))
	{
		if (bSendError)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildMemberOutRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
			SEND_Q(rmsg, dest);
		}
		return ;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberOutNotifyMsg(rmsg, guildindex, charindex, member->GetName());
		gserver.SendToAllGameServer(rmsg);
	}

	guild->DelGradeExPosCount(member->pos());
	guild->removemember(member);
	if (bSendError)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberOutRepMsg(rmsg, guildindex, charindex, MSG_GUILD_ERROR_OK);
		SEND_Q(rmsg, dest);
	}
}

void do_CommandGuildBattleReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex1;
	int guildindex2;
	int prize;
	int zone;
	int time;

	RefMsg(msg) >> guildindex1
		>> guildindex2
		>> prize
		>> zone
		>> time;

	// 헬퍼 2배
	time *= 2;

	CGuild* g1 = gserver.m_guildlist.findguild(guildindex1);
	CGuild* g2 = gserver.m_guildlist.findguild(guildindex2);

	if (!g1 || !g2)
		return;

	// DB UPDATE
	std::string update_guild_query = boost::str(boost::format(
		"UPDATE t_guild SET a_battle_index=%d, a_battle_prize=%d, a_battle_zone=%d, a_battle_time=%d, a_battle_state=%d WHERE a_index=%d")
		% guildindex2 %( prize * 2 * 95 / 100) % zone % time % GUILD_BATTLE_STATE_WAIT % guildindex1);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(update_guild_query);
	if (!cmd.Update())
	{
		return ;
	}

	std::string update_guild_query_1 = boost::str(boost::format(
		"UPDATE t_guild SET a_battle_index=%d, a_battle_prize=%d, a_battle_zone=%d, a_battle_time=%d, a_battle_state=%d WHERE a_index=%d")
		% guildindex1 % (prize * 2 * 95 / 100) % zone % time % GUILD_BATTLE_STATE_WAIT % guildindex2);
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(update_guild_query_1);
	if (!cmd.Update())
	{
		return ;
	}

	// 세부 셋팅
	g1->BattleSet(guildindex2, prize * 2 * 95 / 100, zone);
	g1->BattleTime(time);
	g1->BattleState(GUILD_BATTLE_STATE_WAIT);

	g2->BattleSet(guildindex1, prize * 2 * 95 / 100, zone);
	g2->BattleTime(time);
	g2->BattleState(GUILD_BATTLE_STATE_WAIT);

	// 시작 시간 셋팅
	g1->BattlePulse(gserver.m_pulse);
	g2->BattlePulse(gserver.m_pulse);

	GAMELOG << init("GUILD BATTLE INIT")
			<< g1->index() << delim
			<< g1->name() << delim
			<< "<==>" << delim
			<< g2->index() << delim
			<< g2->name() << delim
			<< g1->battlePrize() << delim
			<< g1->battleZone() << delim
			<< g1->battleTime()
			<< end;

	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildBattleRepMsg(rmsg, g1, g2, prize);
	gserver.SendToAllGameServer(rmsg);
}

void do_CommandGuildBattleStopReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex1;
	int guildindex2;

	RefMsg(msg) >> guildindex1
		>> guildindex2;

	CGuild* g1 = gserver.m_guildlist.findguild(guildindex1);
	CGuild* g2 = gserver.m_guildlist.findguild(guildindex2);

	if (!g1 || !g2)
		return;

	if (g1->battleState() != GUILD_BATTLE_STATE_WAIT &&
			g1->battleState() != GUILD_BATTLE_STATE_ING &&
			g1->battleState() != GUILD_BATTLE_STATE_STOP_WAIT)
	{
		return ;
	}

	if (g2->battleState() != GUILD_BATTLE_STATE_WAIT &&
			g2->battleState() != GUILD_BATTLE_STATE_ING &&
			g2->battleState() != GUILD_BATTLE_STATE_STOP_WAIT)
	{
		return ;
	}

	// 승리길드
	CGuild* winner;

	int prize = g1->battlePrize();
	int zone = g1->battleZone();

	// killcount 비교
	if (g1->killCount() > g2->killCount())
	{
		winner = g1;
	}
	else if (g1->killCount() < g2->killCount())
	{
		winner = g2;
	}
	else
	{
		winner = NULL;
	}
	// msg 부터 만든다
	CNetMsg::SP rmsg(new CNetMsg);
	if (winner)
		HelperGuildBattleStopRepMsg(rmsg, winner->index(), g1, g2);
	else
		HelperGuildBattleStopRepMsg(rmsg, -1, g1, g2);

	if (winner)
	{
		if (winner == g1)
		{
			g2->BattleSet(-1, 0, -1);
			g2->BattleTime(0);
			g2->KillCount(0);
			g2->BattleState(GUILD_BATTLE_STATE_PEACE);
		}
		else if (winner == g2)
		{
			g1->BattleSet(-1, 0, -1);
			g1->BattleTime(0);
			g1->KillCount(0);
			g1->BattleState(GUILD_BATTLE_STATE_PEACE);
		}

		winner->BattleState(GUILD_BATTLE_STATE_PRIZE);
	}
	else
	{
		g1->BattleSet(-1, prize / 2, -1);
		g1->BattleState(GUILD_BATTLE_STATE_PRIZE);
		g2->BattleSet(-1, prize / 2, -1);
		g2->BattleState(GUILD_BATTLE_STATE_PRIZE);
	}

	// DB UPDATE
	std::string update_guild_query = boost::str(boost::format(
		"UPDATE t_guild SET a_battle_index=%d, a_battle_prize=%d, a_battle_zone=%d, a_battle_time=%d, a_battle_killcount=%d, a_battle_state=%d WHERE a_index=%d")
		% g1->battleIndex() % g1->battlePrize() % g1->battleZone() % g1->battleTime() % g1->killCount() % (int)g1->battleState() % g1->index());
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(update_guild_query);
	if (!cmd.Update())
	{
		return ;
	}

	std::string update_guild_query_1 = boost::str(boost::format(
		"UPDATE t_guild SET a_battle_index=%d, a_battle_prize=%d, a_battle_zone=%d, a_battle_time=%d, a_battle_killcount=%d, a_battle_state=%d WHERE a_index=%d")
		% g2->battleIndex() % g2->battlePrize() % g2->battleZone() % g2->battleTime() % g2->killCount() % (int)g2->battleState() % g2->index());
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(update_guild_query_1);
	if (!cmd.Update())
	{
		return ;
	}

	GAMELOG << init("GUILD BATTLE END")
			<< g1->index() << delim
			<< g1->name() << delim
			<< "<==>" << delim
			<< g2->index() << delim
			<< g2->name() << delim
			<< prize << delim
			<< zone << delim
			<< "WINNER" << delim;

	if (winner)
		GAMELOG << winner->index();
	else
		GAMELOG << "-1";

	GAMELOG	<< end;
	gserver.SendToAllGameServer(rmsg);
}

void do_CommandGuildBattlePeaceReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex;

	RefMsg(msg) >> guildindex;

	CGuild* g = gserver.m_guildlist.findguild(guildindex);

	if (!g)
		return;

	if (g->battleState() != GUILD_BATTLE_STATE_PRIZE)
		return;

	g->BattleSet(-1, 0, -1);
	g->BattleTime(0);
	g->KillCount(0);
	g->BattleState(GUILD_BATTLE_STATE_PEACE);

	std::string update_guild_query = boost::str(boost::format(
		"UPDATE t_guild SET a_battle_index=%d, a_battle_prize=%d, a_battle_zone=%d, a_battle_time=%d, a_battle_killcount=%d, a_battle_state=%d WHERE a_index=%d")
		% -1 % 0 % -1 % 0 % 0 % -1 % g->index());

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(update_guild_query);
	if (!cmd.Update())
	{
		return;
	}

	GAMELOG << init("GUILD BATTLE GET PRIZE")
			<< g->index() << delim
			<< g->name()
			<< end;

	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildBattlePeaceRepMsg(rmsg, g);
	gserver.SendToAllGameServer(rmsg);
}

void do_CommandGuildBattleKillReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int of_guidlindex;
	int df_guildindex;

	RefMsg(msg) >> of_guidlindex
		>> df_guildindex;

	CGuild* g1 = gserver.m_guildlist.findguild(of_guidlindex);
	CGuild* g2 = gserver.m_guildlist.findguild(df_guildindex);

	if (!g1 || !g2)
		return;

	if (g1->battleIndex() != g2->index())
		return;

	if (g2->battleIndex() != g1->index())
		return;

	int killcount = g1->killCount();
	killcount++;

	std::string update_guild_query = boost::str(boost::format("UPDATE t_guild SET a_battle_killcount=%d WHERE a_index=%d") % killcount % g1->index());

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(update_guild_query);
	if (!cmd.Update())
	{
		return;
	}

	g1->KillCount(killcount);

	GAMELOG << init("GUILD BATTLE KILLCOUNT")
			<< g1->index() << delim
			<< g1->name() << delim
			<< g1->killCount()
			<< end;
}

//0503 kown
void do_CommandEventMoonStoneUpdateReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	gserver.m_nMoonStoneNas += 200;

	const static int maxNas = 10000000;
	if(gserver.m_nMoonStoneNas > maxNas)//1000만 이상 안모이도록 한다.
		gserver.m_nMoonStoneNas = maxNas;

	std::string select_event_dev_query = boost::str(boost::format(
		"SELECT a_event_type FROM t_event_dev WHERE a_group_index=%d AND a_subgroup_index=0 AND a_event_type=0") % gserver.m_serverno);
	cmd.SetQuery(select_event_dev_query);
	if (cmd.Open())
	{
		std::string update_event_dev_query = "";
		if (cmd.m_nrecords > 0) // update
		{
			update_event_dev_query = boost::str(boost::format(
				"UPDATE t_event_dev SET a_moonstone=%d WHERE a_group_index=%d AND a_subgroup_index=0 AND a_event_type=0") % gserver.m_nMoonStoneNas % gserver.m_serverno);
			cmd.SetQuery(update_event_dev_query);
			cmd.Update();
		}
		else // insert
		{
			update_event_dev_query = boost::str(boost::format(
				"INSERT INTO t_event_dev (a_group_index, a_subgroup_index, a_event_type, a_moonstone) VALUES (%d, 0, 0, %d)") % gserver.m_serverno % gserver.m_nMoonStoneNas);
			cmd.SetQuery(update_event_dev_query);
			cmd.Update();
		}
	}

	CNetMsg::SP rmsg(new CNetMsg);
	HelperEventMoonStoneUpdateRepMsg(rmsg, gserver.m_nMoonStoneNas);
	gserver.SendToAllGameServer(rmsg);
}

void do_CommandEventMoonStoneJackPotReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int chaindex;
	RefMsg(msg) >> chaindex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	std::string select_event_dev_query = boost::str(boost::format(
		"SELECT a_event_type FROM  t_event_dev WHERE a_group_index=%d AND a_subgroup_index=0 AND a_event_type=0") % gserver.m_serverno);
	cmd.SetQuery(select_event_dev_query);
	if (cmd.Open())
	{
		std::string update_event_dev_query = "";
		if (cmd.m_nrecords > 0) // update
		{
			update_event_dev_query = boost::str(boost::format(
				"UPDATE t_event_dev SET a_moonstone=0 WHERE a_group_index=%d AND a_subgroup_index=0 AND a_event_type=0") % gserver.m_serverno);
			cmd.SetQuery(update_event_dev_query);
			cmd.Update();
		}
		else // insert
		{
			update_event_dev_query = boost::str(boost::format(
				"INSERT INTO t_event_dev (a_group_index, a_subgroup_index, a_event_type, a_moonstone) VALUES (%d, 0, 0, 0)") % gserver.m_serverno);
			cmd.SetQuery(update_event_dev_query);
			cmd.Update();
		}
	}

	CNetMsg::SP rmsg(new CNetMsg);
	HelperEventMoonStoneJackPotRepMsg(rmsg, gserver.m_nMoonStoneNas, chaindex);
	SEND_Q(rmsg, dest);

	gserver.m_nMoonStoneNas = 0; //초기화.

	CNetMsg::SP hmsg(new CNetMsg);
	HelperEventMoonStoneUpdateRepMsg(hmsg, gserver.m_nMoonStoneNas);
	gserver.SendToAllGameServer(hmsg);
}

void do_CommandFriendMemberAddReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	UpdateServer::doFriendAddReqToHelper *packet = reinterpret_cast<UpdateServer::doFriendAddReqToHelper*>(msg->m_buf);
	
	//DB에 접속해서 처리..

	//먼저 친구등록 요청한 캐릭에 친구등록
	CLCString m_buf(1024);
#ifdef LC_GAMIGO
	std::string select_friend_query = boost::str(boost::format(
		"SELECT a_char_index, a_friend_index FROM t_friend%02d WHERE a_char_index = %d AND a_friend_index = %d") % (packet->requesterindex % 10) % packet->requesterindex % packet->approvalindex);
	CDBCmd checkcmd;
	checkcmd.Init(&gserver.m_dbchar);
	checkcmd.SetQuery(select_friend_query);
	if (checkcmd.Open() && checkcmd.m_nrecords > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateServer::HelperFriendAddRepMsg(rmsg, packet->approvalindex, packet->appname, packet->appjob, packet->requesterindex, packet->reqname, packet->reqjob, MSG_FRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return ;
	}
#endif
	m_buf.Format("INSERT INTO t_friend%02d (a_char_index,a_friend_index,a_friend_name,a_friend_job) VALUES ( %d,%d,'%s',%d)",
		packet->requesterindex % 10, packet->requesterindex, packet->approvalindex, (const char*)packet->appname, packet->appjob);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(m_buf);
	if (!cmd.Update())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateServer::HelperFriendAddRepMsg(rmsg, packet->approvalindex, packet->appname, packet->appjob, packet->requesterindex, packet->reqname, packet->reqjob, MSG_FRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return ;
	}

	//승인한 캐릭에도 친구등록
#ifdef LC_GAMIGO
	std::string select_friend_query_1 = boost::str(boost::format(
		"SELECT a_char_index, a_friend_index FROM t_friend%02d WHERE a_char_index = %d AND a_friend_index = %d") % (packet->approvalindex % 10) % packet->approvalindex % packet->requesterindex);
	checkcmd.SetQuery(select_friend_query_1);
	if (checkcmd.Open() && checkcmd.m_nrecords > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateServer::HelperFriendAddRepMsg(rmsg, packet->approvalindex, packet->appname, packet->appjob, packet->requesterindex, packet->reqname, packet->reqjob, MSG_FRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return ;
	}
#endif
	m_buf.Format("INSERT INTO t_friend%02d (a_char_index,a_friend_index,a_friend_name,a_friend_job) VALUES ( %d,%d,'%s',%d)",
		packet->approvalindex % 10, packet->approvalindex, packet->requesterindex, (const char*)packet->reqname, packet->reqjob);

	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(m_buf);
	if (!cmd.Update())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateServer::HelperFriendAddRepMsg(rmsg, packet->approvalindex, packet->appname, packet->appjob, packet->requesterindex, packet->reqname, packet->reqjob, MSG_FRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return ;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	UpdateServer::HelperFriendAddRepMsg(rmsg, packet->approvalindex, packet->appname, packet->appjob, packet->requesterindex, packet->reqname, packet->reqjob, MSG_FRIEND_ERROR_OK);
	SEND_Q(rmsg, dest);
}

void do_CommandFriendMemberDelReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	UpdateServer::doFriendDelMemberReqToHelper *packet = reinterpret_cast<UpdateServer::doFriendDelMemberReqToHelper*>(msg->m_buf);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	CLCString table(20);
	CLCString table2(20);
	table.Format("t_friend%02d", packet->approvalIndex % 10);
	table2.Format("t_friend%02d", packet->requesterIndex % 10);

	//삭제요청자 DB지우기.
	std::string delete_query = boost::str(boost::format("DELETE FROM %s WHERE a_char_index=%d AND a_friend_index=%d") % (const char*)table % packet->approvalIndex % packet->requesterIndex);

	cmd.SetQuery(delete_query);
	if (!cmd.Update())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateServer::HelperFriendDelRepMsg(rmsg, packet->approvalIndex, packet->requesterIndex, MSG_FRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return;
	}

	cmd.Init(&gserver.m_dbchar);
	//삭제 당한자 DB지우기.
	std::string delete_query_1 = boost::str(boost::format("DELETE FROM %s WHERE a_char_index=%d AND a_friend_index=%d") % (const char*)table2 % packet->requesterIndex % packet->approvalIndex);

	cmd.SetQuery(delete_query_1);
	if (!cmd.Update())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateServer::HelperFriendDelRepMsg(rmsg, packet->approvalIndex, packet->requesterIndex, MSG_FRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);
		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	UpdateServer::HelperFriendDelRepMsg(rmsg, packet->approvalIndex, packet->requesterIndex, MSG_FRIEND_ERROR_OK);
//0521 전체에 보내줘야 할듯..
//	SEND_Q(rmsg, dest);
	gserver.SendToAllGameServer(rmsg);
}

void do_CommandBlockCharReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int reqIndex;
	int blockIndex = -1;
	CLCString blockName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> reqIndex
		>> blockName;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	std::string select_characters_query = boost::str(boost::format(" SELECT a_index FROM t_characters WHERE a_nick = '%s' ")% (const char*) blockName);
	cmd.SetQuery(select_characters_query);
	if( !cmd.Open() || cmd.m_nrecords < 1 || !cmd.MoveFirst())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperBlockCharRepMsg(rmsg, reqIndex, -1, "" );
		SEND_Q(rmsg, dest);
		return;
	}

	cmd.GetRec("a_index", blockIndex);

	CNetMsg::SP rmsg(new CNetMsg);
	HelperBlockCharRepMsg(rmsg, reqIndex, blockIndex, blockName );
	gserver.SendToAllGameServer(rmsg);
}

void do_CommandGiftCharReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	//MSG_HELPER_GIFT_RECVCHARNAME_REQ,			// 핼퍼에 해당 케릭터가 있는 : sendCharIndex(n) recvCharName(str) msg(str) count(n) idx(n) ctid(n)
	//MSG_HELPER_GIFT_RECVCHARNAME_REP,			//							: sendCharIndex(n) recvCharIndex(n) revCharName(str) msg(str) count(n) idx(n) ctid(n)
	int sendCharIdx, sendUserIdx;
	CLCString recvCharName(MAX_CHAR_NAME_LENGTH + 1);
	CLCString sendMsg(MAX_GIFT_MESSAGE_LENGTH + 1);
	int count;
	int idx[MAX_PURCHASEITEM];
	int ctid[MAX_PURCHASEITEM];
	int i;
	int recvuserIndex, recvcharIndex;

	RefMsg(msg) >> sendUserIdx
		>> sendCharIdx
		>> recvCharName
		>> sendMsg
		>> count;

	for(i = 0; i < count; i++)
	{
		RefMsg(msg) >> idx[i]
			>> ctid[i];
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	std::string selelct_characters_query = boost::str(boost::format(
		"SELECT a_index, a_user_index FROM t_characters WHERE a_enable = 1 AND a_nick = '%s' AND a_user_index != %d ") % (const char*) recvCharName % sendUserIdx );
	cmd.SetQuery(selelct_characters_query);

	if( !cmd.Open() || cmd.m_nrecords < 1 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGiftCharRepMsg(rmsg, sendUserIdx, sendCharIdx, -1, -1, recvCharName, sendMsg, count, idx, ctid);
		SEND_Q(rmsg, dest);
		return;
	}

	if( !cmd.MoveFirst() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGiftCharRepMsg(rmsg, sendUserIdx, sendCharIdx, -1, -1, recvCharName, sendMsg, count, idx, ctid);
		SEND_Q(rmsg, dest);
		return;
	}

	if( !cmd.GetRec("a_user_index", recvuserIndex ) || !cmd.GetRec("a_index", recvcharIndex) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGiftCharRepMsg(rmsg, sendUserIdx, sendCharIdx, -1, -1, recvCharName, sendMsg, count, idx, ctid);
		SEND_Q(rmsg, dest);
		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	HelperGiftCharRepMsg(rmsg, sendUserIdx, sendCharIdx, recvuserIndex, recvcharIndex, recvCharName, sendMsg, count, idx, ctid);
	SEND_Q(rmsg, dest);
}

void do_CommandFriendSetConditionReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int chaindex;
	int condition;
	int sum;
	int index[FRIEND_MAX_MEMBER];
	int reply;

	RefMsg(msg) >> chaindex
		>> condition
		>> reply;

	if(reply == -1)
	{
		RefMsg(msg) >> sum;

		for(int i=0 ; i<sum ; i++)
		{
			RefMsg(msg) >> index[i];
		}

		//접속하고 있는 친구들에게 알려야한다..
		CNetMsg::SP rmsg(new CNetMsg);
		HelperFriendSetConditionNotifyMsg(rmsg, chaindex, condition, -1, sum, index);
		gserver.SendToAllGameServer(rmsg);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperFriendSetConditionNotifyMsg(rmsg, chaindex, condition, reply, 0, NULL);
		gserver.SendToAllGameServer(rmsg);
	}
}

void do_CommandPetCreateReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int owner;
	char typegrade;

	RefMsg(msg) >> owner
		>> typegrade;

	// 이미 펫을 가지고 있으면 생성하지 않고 페일 메세지 보낸닷
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	std::string select_pet_query = boost::str(boost::format(" SELECT a_index FROM t_pet WHERE a_owner = %d AND a_enable = 1") % owner);
	cmd.SetQuery(select_pet_query);
	cmd.Open();

	if( cmd.m_nrecords >= MAX_OWN_PET )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPetCreateRepMsg(rmsg, -1, owner, typegrade);
		SEND_Q(rmsg, dest);
		return;
	}

	std::string insert_pet_query = boost::str(boost::format("INSERT INTO t_pet (a_owner, a_enable, a_lastupdate, a_type) VALUES (%d, 2, NOW(), %d)") % owner % (int)typegrade);
	cmd.SetQuery(insert_pet_query);
	if (cmd.Update())
	{
		int index = cmd.insertid();

		CNetMsg::SP rmsg(new CNetMsg);
		HelperPetCreateRepMsg(rmsg, index, owner, typegrade);
		SEND_Q(rmsg, dest);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPetCreateRepMsg(rmsg, -1, owner, typegrade);
		SEND_Q(rmsg, dest);
	}
}

void do_CommandPetDeleteReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int index;
	int owner;

	RefMsg(msg) >> index
		>> owner;

	std::string delete_pet_query = boost::str(boost::format("DELETE FROM t_pet WHERE a_index=%d and a_owner = %d") % index % owner);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(delete_pet_query);
	if (cmd.Update())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPetDeleteRepMsg(rmsg, index, owner);
		SEND_Q(rmsg, dest);
	}
}

void do_CommandGuildStashHistoryReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	CNetMsg::SP rmsg(new CNetMsg);

	int guildindex = 0;
	int charindex = 0;
	int month[7] = {0, 0, 0, 0, 0, 0, 0};
	int day[7] = {0, 0, 0, 0, 0, 0, 0};
	LONGLONG money[7] = {0, 0, 0, 0, 0, 0, 0};

	RefMsg(msg) >> guildindex
		>> charindex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	// 7일보다 오래된 기록 삭제
	std::string delete_guild_tax_history = boost::str(boost::format(
		"DELETE FROM t_guild_tax_history WHERE a_guild_index = %d AND a_indate <= CAST(DATE_ADD(NOW(), INTERVAL -7 DAY) AS DATE)") % guildindex);
	cmd.SetQuery(delete_guild_tax_history);
	cmd.Update();

	// 시간순으로 나스 입금액 얻기
	std::string selete_guild_tax_history = boost::str(boost::format(
		"SELECT MONTH(a_indate) AS mm, DAYOFMONTH(a_indate) AS dd, a_money FROM t_guild_tax_history WHERE a_guild_index = %d ORDER BY a_indate") % guildindex);
	cmd.SetQuery(selete_guild_tax_history);
	if (cmd.Open() && cmd.m_nrecords)
	{
		int i = 0;
		while (cmd.MoveNext() && i < 7)
		{
			cmd.GetRec("mm",		month[i]);
			cmd.GetRec("dd",		day[i]);
			cmd.GetRec("a_money",	money[i]);
			i++;
		}
		HelperGuildStashHistoryRepMsg(rmsg, charindex, MSG_HELPER_GUILD_STASH_ERROR_OK, month, day, money);
	}
	else
	{
		// 없으면 없다고 오류
		HelperGuildStashHistoryRepMsg(rmsg, charindex, MSG_HELPER_GUILD_STASH_ERROR_NOHISTORY, month, day, money);
	}
	SEND_Q(rmsg, dest);
}

void do_CommandGuildStashViewReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex = 0;
	int charindex = 0;
	LONGLONG balance = 0;

	RefMsg(msg) >> guildindex
		>> charindex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	// 잔액 가져오기
	char sql[1024] = {0,};
	sprintf(sql, "SELECT a_balance FROM t_guild WHERE a_index = %d", guildindex);
	cmd.SetQuery(sql);
	if (cmd.Open() && cmd.MoveFirst())
	{
		cmd.GetRec("a_balance", balance);
	}

	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildStashViewRepMsg(rmsg, charindex, MSG_HELPER_GUILD_STASH_ERROR_OK, balance);
	SEND_Q(rmsg, dest);
}

void do_CommandGuildStashTakeReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex = 0;
	int charindex = 0;
	LONGLONG money = 0;
	LONGLONG balance = 0;

	RefMsg(msg) >> guildindex
		>> charindex
		>> money;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	if (money > 0)
	{
		std::string update_guild_query = boost::str(boost::format(
			"UPDATE t_guild SET a_balance = a_balance - %1% WHERE a_index = %2% AND a_balance >= %3%") % money % guildindex % money);
		cmd.SetQuery(update_guild_query);
		if (!cmd.Update() || cmd.m_nrecords < 1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashTakeRepMsg(rmsg, guildindex, charindex, MSG_HELPER_GUILD_STASH_ERROR_NOTENOUGH, 0, 0);
			SEND_Q(rmsg, dest);
			return ;
		}
	}

	// 잔액 가져오기
	char sql[1024] = {0,};
	sprintf(sql, "SELECT a_balance FROM t_guild WHERE a_index = %d", guildindex);
	cmd.SetQuery(sql);
	if (cmd.Open() && cmd.MoveFirst())
	{
		cmd.GetRec("a_balance", balance);
	}

	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildStashTakeRepMsg(rmsg, guildindex, charindex, MSG_HELPER_GUILD_STASH_ERROR_OK, money, balance);
	SEND_Q(rmsg, dest);
}

void do_CommandGuildStashRollback(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex = 0;
	LONGLONG money = 0;

	RefMsg(msg) >> guildindex
		>> money;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	std::string update_guild_query = "";
	if (money > 0)
	{
		std::string update_guild_query = boost::str(boost::format(
			"UPDATE t_guild SET a_balance = a_balance + %1% WHERE a_index = %2%") % money % guildindex);
	}
	cmd.SetQuery(update_guild_query);
	cmd.Update();
}

void do_CommandGuildStashSaveTaxReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex = 0;
	int zoneindex = 0;
	LONGLONG taxItem = 0;
	LONGLONG taxProduce = 0;

	RefMsg(msg) >> guildindex
		>> zoneindex
		>> taxItem
		>> taxProduce;

	if (guildindex < 1)
	{
		// 세금 받을 길드가 없으면 무조건 성공
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashSaveTaxRepMsg(rmsg, MSG_HELPER_GUILD_STASH_ERROR_OK, guildindex, zoneindex, taxItem, taxProduce);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 최소 금액 설정
	LONGLONG realTaxItem = 1000000;
	LONGLONG realTaxProduce = 500000;

	if (realTaxItem < taxItem)
		realTaxItem = taxItem;
	if (realTaxProduce < taxProduce)
		realTaxProduce = taxProduce;

	LONGLONG totalTax = realTaxItem + realTaxProduce;

	if (totalTax < 1)
	{
		taxItem = 0;
		taxProduce = 0;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashSaveTaxRepMsg(rmsg, MSG_HELPER_GUILD_STASH_ERROR_OK, guildindex, zoneindex, taxItem, taxProduce);
		SEND_Q(rmsg, dest);
		return ;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	std::string update_guild_query = boost::str(boost::format(
		"UPDATE t_guild SET a_balance = a_balance + %1% WHERE a_index = %2%") % totalTax % guildindex);
	
	cmd.SetQuery(update_guild_query);
	if (!cmd.Update() || cmd.m_nrecords < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashSaveTaxRepMsg(rmsg, MSG_HELPER_GUILD_STASH_ERROR_FAIL_DB, guildindex, zoneindex, taxItem, taxProduce);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 입금 내역 갱신
	std::string update_guild_tax_history = boost::str(boost::format(
		"UPDATE t_guild_tax_history SET a_money = a_money + %1% WHERE a_guild_index = %2% AND a_indate = CAST(NOW() AS DATE)") % totalTax % guildindex);

	cmd.SetQuery(update_guild_tax_history);
	if (!cmd.Update() || cmd.m_nrecords < 1)
	{
		// 오늘 날짜 입금 내역 갱신이 안되면 추가
		std::string insert_guild_tax_history = boost::str(boost::format(
			"INSERT INTO t_guild_tax_history (a_guild_index, a_indate, a_money) VALUES (%1%, CAST(NOW() AS DATE), %2%)") % guildindex % totalTax);

		cmd.SetQuery(insert_guild_tax_history);
		cmd.Update();
	}
	// 결과
	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildStashSaveTaxRepMsg(rmsg, MSG_HELPER_GUILD_STASH_ERROR_OK, guildindex, zoneindex, taxItem, taxProduce);
	SEND_Q(rmsg, dest);
}

//휴면케릭 이벤트 친구 등록부분 : GameServer에서 넘어온 부분.
void do_CommandEventSearchFriendMemberAddReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int approval_index, approval_job, approval_user_index, approval_server_old;
	CLCString approval_nick(MAX_CHAR_NAME_LENGTH + 1);

	int request_index , request_user_index, request_datestamp;
	CLCString request_nick(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> approval_index
		>> approval_nick
		>> approval_job
		>> approval_user_index
		>> approval_server_old
		>> request_nick;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	std::string select_characters_query = boost::str(boost::format(
		"SELECT a_index, a_user_index, a_nick, (UNIX_TIMESTAMP('2006-02-14 10:00:00') - UNIX_TIMESTAMP(a_datestamp)) as a_ctime FROM t_characters "
		"WHERE a_name = '%d%s'") % approval_server_old % request_nick);
	cmd.SetQuery(select_characters_query);

	//휴면 케릭 대상 서버군 케릭만 등록 할 수 있다.
	//등록한 케릭명 검색 우선 서버군 케릭 확인.
	//만약 동일 케릭명일 경우 (서버군 명과 비교)
	if (!(cmd.Open() && cmd.MoveFirst()))
	{
		//기존 서버군이 아닌 휴면케릭일 경우
		std::string select_characters_1 = boost::str(boost::format(
			"SELECT a_index, a_user_index, a_nick, (UNIX_TIMESTAMP('2006-02-14 10:00:00') - UNIX_TIMESTAMP(a_datestamp)) as a_ctime FROM t_characters "
			" WHERE a_nick ='%s'") % request_nick);
		cmd.SetQuery(select_characters_1);

		if (!(cmd.Open() && cmd.MoveFirst()))
		{
			// 등록 신청한 친구가 없습니다.
			CNetMsg::SP rmsg(new CNetMsg);
			HelperEventSearchFriendMemberAddRepMsg(rmsg
				, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_NOT_EXIST);
			SEND_Q(rmsg, dest);

			GAMELOG << init("EVENT SEARCH FRIEND _ERROR_NOT_EXIST")
					<< approval_nick << delim
					<< request_nick << delim
					<< end;
			return;
		}
	}
	//등록한 케릭명 검색 휴면케릭 자료
	cmd.GetRec("a_user_index", request_user_index);
	cmd.GetRec("a_index", request_index);
	cmd.GetRec("a_nick", request_nick);
	cmd.GetRec("a_ctime", request_datestamp);

	std::string select_event_search_friend_date_query = boost::str(boost::format(
		"SELECT (UNIX_TIMESTAMP('2006-02-14 10:00:00') - UNIX_TIMESTAMP(a_datestamp)) as a_ctime"
		"FROM t_event_searchfriend_date where a_index = %d") % request_index);
	cmd.SetQuery(select_event_search_friend_date_query);
	if (!(cmd.Open() && cmd.MoveFirst()))
	{
		//금요일 10일과 14일 사이에 생서된 케릭들일 경우....
		if ( request_datestamp < 2678400)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperEventSearchFriendMemberAddRepMsg(rmsg
				, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_GAMESERVER);
			SEND_Q(rmsg, dest);

			GAMELOG << init("EVENT SEARCH FRIEND ERROR_GAMESERVER ")
					<< approval_nick << delim
					<< request_nick << delim
					<< request_datestamp << delim
					<< end;
			return ;
		}
		else
		{   //혹시라도 t_event_searchfriend_date에서 전 날짜를 불러오지 못했을 경우..예외.
			GAMELOG << init("EVENT SEARCH FRIEND ERROR_GAMESERVER ")
					<< approval_nick << delim
					<< request_nick << delim
					<< request_datestamp << delim
					<< end;
			return ;
		}
	}
	else
		cmd.GetRec("a_ctime", request_datestamp);

	//휴면 케릭이 1개월 미만(31일 미만일 경우) 등록 불가.
	if ( request_datestamp < 2678400)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventSearchFriendMemberAddRepMsg(rmsg
			, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND ERROR_GAMESERVER ")
				<< approval_nick << delim
				<< request_nick << delim
				<< request_datestamp << delim
				<< end;
		return ;
	}

	//접속자가 같은 ID이면 불가능 함.
	if(request_user_index == approval_user_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventSearchFriendMemberAddRepMsg(rmsg
			, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_NOT_MATCH_INDEX);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND ERROR NOT MATCH INDEX")
				<< approval_nick << delim
				<< request_nick << delim
				<< approval_user_index << delim
				<< end;
		return;
	}
	//이미 등록이 되어 있는지..
	std::string select_event_search_friend_query = boost::str(boost::format(
		"SELECT count(a_char_index) AS nCount FROM t_event_searchfriend where a_dormant_index = %d and (a_char_index = %d or a_dormant_select = 1)")
		% request_index % approval_index);
	cmd.SetQuery(select_event_search_friend_query);

	int nCount = 0;
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		GAMELOG << init("EVENT SEARCH FRIEND ERROR ALREADY EXIST FAIL")
				<< approval_nick << delim
				<< request_nick << delim
				<< end;
		return;
	}

	cmd.GetRec("nCount", nCount);

	if(nCount >= 1 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventSearchFriendMemberAddRepMsg(rmsg
			, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_ALREADY_EXIST);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND ERROR ALREADY EXIST")
				<< approval_nick << delim
				<< request_nick << delim
				<< end;
		return;
	}

	//등록 케릭 50명 제한
	std::string select_event_search_friend_query_1 = boost::str(boost::format(
		"SELECT count(a_char_index) AS nCount FROM t_event_searchfriend where a_char_index = %d") % approval_index);
	cmd.SetQuery(select_event_search_friend_query_1);

	nCount = 0;
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		GAMELOG << init("EVENT SEARCH FRIEND ERROR FULLMEMBER FAIL")
				<< approval_nick << delim
				<< request_nick << delim
				<< end;
		return;
	}

	if( !cmd.GetRec("nCount", nCount) || nCount >= 50 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventSearchFriendMemberAddRepMsg(rmsg
			, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_FULLMEMBER);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND ERROR FULLMEMBER")
				<< approval_nick << delim
				<< request_nick << delim
				<< nCount << delim
				<< end;
		return;
	}

	//DB에 접속해서 처리..
	//먼저 친구등록 요청한 캐릭에 친구등록
#ifdef LC_GAMIGO
	std::string select_event_search_friend_query_2 = boost::str(boost::format(
	"SELECT a_char_index, a_dormant_index FROM t_event_searchfriend WHERE a_char_index = %d AND a_dormant_index = %d") % approval_index % request_index);
	CDBCmd checkcmd;
	checkcmd.Init(&gserver.m_dbchar);
	checkcmd.SetQuery(select_event_search_friend_query_2);
	if (checkcmd.Open() && checkcmd.m_nrecords > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventSearchFriendMemberAddRepMsg(rmsg
			, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_PACKET);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND ERROR PACKET")
			<< approval_nick << delim
			<< request_nick << delim
			<< end;
		return ;
	}
#endif
	std::string insert_event_search_friend_query = boost::str(boost::format(
		"INSERT INTO t_event_searchfriend (a_char_index, a_char_nick, a_char_good"
				",a_dormant_index,a_dormant_name,a_dormant_nick"
				",a_dormant_total_time, a_dormant_good) VALUES "
				"(%d,'%s',%d, %d,'%s','%s',%d, %d)")
				%approval_index % (const char*)approval_nick % -1 % request_index % (const char*)request_nick % (const char*)request_nick % 0 % -1);
	cmd.SetQuery(insert_event_search_friend_query);
	if (!cmd.Update())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventSearchFriendMemberAddRepMsg(rmsg
			, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_PACKET);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND ERROR PACKET")
				<< approval_nick << delim
				<< request_nick << delim
				<< end;
		return ;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	HelperEventSearchFriendMemberAddRepMsg(rmsg
		, approval_index, request_nick, MSG_EVENT_SEARCHFRIEND_ERROR_OK);
	SEND_Q(rmsg, dest);

	GAMELOG << init("EVENT SEARCH FRIEND OK")
			<< approval_nick << delim
			<< request_nick << delim
			<< end;
}

//휴면케릭 이벤트 등록 설정
void do_CommandEventSearchFriendMemberSelectAddReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int approval_index;
	int request_index;

	RefMsg(msg) >> approval_index
		>> request_index;

	//DB에 접속해서 처리..
	//먼저 친구등록 요청한 캐릭에 친구등록
	std::string update_event_search_friend_query = boost::str(boost::format(
		"UPDATE t_event_searchfriend SET a_dormant_select = 1, a_char_good = 0, a_dormant_good = 0 "
		"WHERE a_dormant_index= %d and a_char_index = %d")
		% approval_index % request_index);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.BeginTrans();
	cmd.SetQuery(update_event_search_friend_query);
	if (!cmd.Update())
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventSearchFriendMemberSelectAddRepMsg(rmsg
			, approval_index, MSG_EVENT_SEARCHFRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND")
				<< "ERROR HELPER DB " << delim
				<< "SELECT ADD CHAR" << delim
				<< approval_index << delim << request_index
				<< end;
		return ;
	}

	//휴면 이벤트 시간
	char	te_buf[1024];	// for temp string
	struct tm ti = NOW();
	te_buf[0] = '\0';
	IntCat0(te_buf, ti.tm_year + 1900, 4, false);
	strcat(te_buf, "/");
	IntCat0(te_buf, ti.tm_mon + 1, 2, false);
	strcat(te_buf, "/");
	IntCat0(te_buf, ti.tm_mday, 2, false);
	strcat(te_buf, " ");
	IntCat0(te_buf, ti.tm_hour, 2, false);
	strcat(te_buf, ":");
	IntCat0(te_buf, ti.tm_min, 2, false);
	strcat(te_buf, ":");
	IntCat0(te_buf, ti.tm_sec, 2, false);

	std::string update_event_search_friend_date_query = boost::str(boost::format(
		"UPDATE t_event_searchfriend_date SET a_datestamp='%s' WHERE a_index = %d")
		% te_buf % approval_index);
	cmd.SetQuery(update_event_search_friend_date_query);
	if (!cmd.Update())
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventSearchFriendMemberSelectAddRepMsg(rmsg
			, approval_index, MSG_EVENT_SEARCHFRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND")
				<< "ERROR HELPER DB Time " << delim
				<< "SELECT ADD CHAR" << delim
				<< approval_index << delim << request_index
				<< end;
		return;
	}

	std::string delete_event_search_friend_date_query = boost::str(boost::format(
		"DELETE FROM t_event_searchfriend WHERE a_dormant_index= %d and a_dormant_select = 0") % approval_index);
	cmd.SetQuery(delete_event_search_friend_date_query);
	if (!cmd.Update())
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventSearchFriendMemberSelectAddRepMsg(rmsg
			, approval_index, MSG_EVENT_SEARCHFRIEND_ERROR_GAMESERVER);
		SEND_Q(rmsg, dest);

		GAMELOG << init("EVENT SEARCH FRIEND")
				<< "ERROR HELPER DB_delete " << delim
				<< "SELECT ADD CHAR" << delim
				<< approval_index << delim << request_index
				<< end;
		return;
	}
	cmd.Commit();

	CNetMsg::SP rmsg(new CNetMsg);
	HelperEventSearchFriendMemberSelectAddRepMsg(rmsg
		, approval_index, MSG_EVENT_SEARCHFRIEND_ERROR_OK);
	SEND_Q(rmsg, dest);
}

//휴면케릭 사냥시간 한시간 단위로 업데이터 하는 부분
void do_CommandEventSearchFriendMemberOneTimeCheckReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int	timesec, appDormantindex;

	RefMsg(msg) >> timesec
		>> appDormantindex;

	//DB에 접속해서 처리..

	std::string update_event_search_friend_date_query = "";
	if(timesec == (60 * 60 * 30)) //30시간일때만
	{
		//30시간 채울 경우 아이템 보상 1점씩 올려줌.
		update_event_search_friend_date_query = boost::str(boost::format(
			"UPDATE t_event_searchfriend SET a_dormant_total_time=%d"
			", a_char_good = 1, a_dormant_good = 2 WHERE a_dormant_index=%d and a_dormant_select = 1")
			% timesec % appDormantindex);
	}
	else
	{
		//1시간당 체크 업데이터
		update_event_search_friend_date_query = boost::str(boost::format(
		"UPDATE t_event_searchfriend SET a_dormant_total_time=%d "
		"WHERE a_dormant_index=%d and a_dormant_select = 1")
		% timesec % appDormantindex);
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(update_event_search_friend_date_query);

	if(!cmd.Update())
	{
		GAMELOG << init("EVENT SEARCH FRIEND")
				<< "ERROR HELPER DB " << appDormantindex << delim
				<< "ONE TIME CHECK" << timesec
				<< end;
		return;
	}

	GAMELOG << init("EVENT SEARCH FRIEND")
			<< "ONE TIME CHECK " << delim
			<< timesec<< delim
			<< appDormantindex
			<< end;

	CNetMsg::SP rmsg(new CNetMsg);
	HelperEventSearchFriendMemberOneTimeCheckRepMsg(rmsg,timesec, appDormantindex, MSG_EVENT_SEARCHFRIEND_ERROR_OK);
	SEND_Q(rmsg, dest);
}

//휴면케릭 이벤트 등록된 친구 보상가능 여부
void do_CommandEventSearchFriendMemberListGoodReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int approval_index;
	RefMsg(msg) >> approval_index;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	std::string select_event_search_friend_query = boost::str(boost::format(
		"SELECT * FROM t_event_searchfriend WHERE (a_dormant_index = %d and a_dormant_select = 1) or a_char_index = %d") % approval_index % approval_index);
	cmd.SetQuery(select_event_search_friend_query);

	int ntemp = 0;
	int request_index[50];
	int item_good[50];
	char request_nick[50][MAX_CHAR_NAME_LENGTH + 1];

	if( cmd.Open() && cmd.MoveFirst())
	{
		CLCString rnick(MAX_CHAR_NAME_LENGTH + 1);
		do
		{
			cmd.GetRec("a_dormant_index", request_index[ntemp]);
			cmd.GetRec("a_dormant_nick", rnick);
			strcpy(request_nick[ntemp], rnick);

			if(request_index[ntemp] == approval_index)
				cmd.GetRec("a_dormant_good", item_good[ntemp]);
			else
				//이벤트 사냥에서 얻은 아이템 개수, 본인케릭과 등록해준 케릭의 아이템 얻은 양을 알아야된다.
				cmd.GetRec("a_char_good", item_good[ntemp]);
			ntemp++;
		}while (cmd.MoveNext());
	}

	if (ntemp <= 20)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventSearchFriendMemberListGoodRepMsg(rmsg, approval_index, request_index, request_nick, item_good, ntemp, 0, ntemp);
		SEND_Q(rmsg, dest);
	}
	else
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperEventSearchFriendMemberListGoodRepMsg(rmsg, approval_index, request_index, request_nick, item_good, 20, 0, ntemp);
			SEND_Q(rmsg, dest);
		}

		if (ntemp <= 40)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperEventSearchFriendMemberListGoodRepMsg(rmsg, approval_index, request_index + 20, request_nick + 20, item_good + 20, ntemp - 20, 20, ntemp);
			SEND_Q(rmsg, dest);
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperEventSearchFriendMemberListGoodRepMsg(rmsg, approval_index, request_index + 20, request_nick + 20, item_good + 20, 20, 20, ntemp);
			SEND_Q(rmsg, dest);

			CNetMsg::SP tmsg(new CNetMsg);
			HelperEventSearchFriendMemberListGoodRepMsg(tmsg, approval_index, request_index + 40, request_nick + 40, item_good + 40, ntemp - 40, 40, ntemp);
			SEND_Q(tmsg, dest);
		}
	}
}

//휴면케릭 이벤트 등록된 친구 보상 결과
void do_CommandEventSearchFriendMemberGoodReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int approval_index, ndormantindex, item_good;
	RefMsg(msg) >> approval_index
		>> ndormantindex
		>> item_good;

	//DB에 접속해서 처리..
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.BeginTrans();
	std::string update_event_searc_friend_query = "";
	if(ndormantindex == approval_index)
	{
		update_event_searc_friend_query = boost::str(boost::format(
			"UPDATE t_event_searchfriend SET a_dormant_good = 3 WHERE a_dormant_index=%d and a_dormant_select = 1")
			% approval_index);
	}
	else
	{
		update_event_searc_friend_query = boost::str(boost::format(
			"UPDATE t_event_searchfriend SET a_char_good = 3 WHERE a_dormant_index=%d and a_dormant_select = 1")
			% ndormantindex);
	}

	cmd.SetQuery(update_event_searc_friend_query);
	if (!cmd.Update())
	{
		cmd.Rollback();
		GAMELOG << init("EVENT SEARCH FRIEND")
				<< "ERROR HELPER DB " << delim
				<< "ITEM" << item_good << delim
				<< approval_index << delim << ndormantindex
				<< end;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventSearchFriendMemberGoodRepMsg(rmsg, approval_index, MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_HELPER);
		SEND_Q(rmsg, dest);
		return;
	}
	else
		cmd.Commit();

	CNetMsg::SP rmsg(new CNetMsg);
	HelperEventSearchFriendMemberGoodRepMsg(rmsg, approval_index, MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_OK);
	SEND_Q(rmsg, dest);
}

void do_CommandPartyInviteReq(CNetMsg::SP& msg, CDescriptor* dest)
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

	CNetMsg::SP rmsg(new CNetMsg);
	HelperPartyInviteReqMsg(rmsg, nBossIndex, strBossName, nBossLevel, nTargetIndex, nPartyType);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);
}

void do_CommandPartyInviteRep(CNetMsg::SP& msg, CDescriptor* dest)
{
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);
	char		nPartyType;
	int			nErrorCode;
	int			nBossLevel;

	RefMsg(msg) >> nBossIndex
		>> strBossName
		>> nTargetIndex
		>> strTargetName
		>> nPartyType
		>> nErrorCode;
	RefMsg(msg)	>> nBossLevel;

	if (nErrorCode == MSG_HELPER_PARTY_ERROR_INVITE_OK)
	{
		CParty* pPartyBoss = gserver.FindPartyByMemberIndex(dest->m_subNo, nBossIndex, true);
		CParty* pPartyTarget = gserver.FindPartyByMemberIndex(dest->m_subNo, nTargetIndex, true);

		// nBossIndex가 파티 초대가 가능한지 : 파티가 없거나 보스
		if (pPartyBoss && pPartyBoss->GetBossIndex() != nBossIndex)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, nPartyType, nPartyType, nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_ME);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
			return ;
		}

		// 파티 타입 검사
		if (pPartyBoss && pPartyBoss->GetPartyType(MSG_DIVITYPE_SP) != nPartyType)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, nPartyType, nPartyType, nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_DIFF_TYPE);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
			return ;
		}

		// 대상 파티 검사
		if (pPartyTarget)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, nPartyType, nPartyType, nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_TARGET);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
			return ;
		}

		// 파티 인원 검사
		if (pPartyBoss && pPartyBoss->GetMemberCount() >= MAX_PARTY_MEMBER)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, nPartyType, nPartyType, nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_FULL);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
			return ;
		}

		// 현재 요청자 유무 검사
		if (pPartyBoss && pPartyBoss->GetRequestIndex() > 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, nPartyType, nPartyType, nPartyType, MSG_HELPER_PARTY_ERROR_INVITE_ALREADY_REQUEST);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
			return ;
		}

		if (pPartyBoss)
		{
			// 기존 파티에 요청자로 설정
			pPartyBoss->SetRequest(nTargetIndex, strTargetName);
		}
		else
		{
			// 신규 파티 생성
			pPartyBoss = new CParty(dest->m_subNo, nPartyType, nBossIndex, strBossName, nTargetIndex, strTargetName , nBossLevel);
			LONGLONG key = MAKE_LONGLONG_KEY(dest->m_subNo, nBossIndex);
			gserver.m_listParty.insert(map_party_t::value_type(key, pPartyBoss));
		}

		// 결과 전달
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, pPartyBoss->GetPartyType(MSG_DIVITYPE_SP), pPartyBoss->GetPartyType(MSG_DIVITYPE_ITEM), pPartyBoss->GetPartyType(MSG_DIVITYPE_SPECIAL), MSG_HELPER_PARTY_ERROR_INVITE_OK);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
	}
	else
	{
		// 오류는 에코
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName, nPartyType, nPartyType, nPartyType, (MSG_HELPER_PARTY_ERROR_TYPE)nErrorCode);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
	}
}

void do_CommandPartyAllowReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int			nBossIndex;
	int			nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);
	int			nLevel;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex
		>> strTargetName;
	RefMsg(msg) >> nLevel;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);

	if (!pParty)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyAllowRepMsg(rmsg, nBossIndex, nTargetIndex, strTargetName, MSG_HELPER_PARTY_ERROR_ALLOW_NOT_PARTY);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}

	if (pParty->GetRequestIndex() != nTargetIndex)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyAllowRepMsg(rmsg, nBossIndex, nTargetIndex, strTargetName, MSG_HELPER_PARTY_ERROR_ALLOW_NOT_PARTY);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}

	// 파티원으로 등록
	pParty->JoinRequest(strTargetName, nLevel );

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyAllowRepMsg(rmsg, nBossIndex, nTargetIndex, strTargetName, MSG_HELPER_PARTY_ERROR_ALLOW_OK);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
	}

	// 파티에 참가하는 유저는 매칭 테이블에서 제거
	CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(dest->m_subNo, nTargetIndex);
	if (pMatchMember)
		do_CommandPartyMatchDelReq_real(dest->m_subNo, nTargetIndex);

	CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex( dest->m_subNo, nBossIndex );
	if( pMatchParty )
	{
		pMatchParty->SetMemberCount( pParty->GetMemberCount() );
	}
}

void do_CommandPartyRejectReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int			nBossIndex;
	int			nTargetIndex;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyRejectRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_REJECT_FAIL);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}

	if (pParty->GetRequestIndex() < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyRejectRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_REJECT_FAIL);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}

	// 요청자가 보스인지 일반인지 검사
	if (pParty->GetBossIndex() == nTargetIndex || pParty->GetRequestIndex() == nTargetIndex)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyRejectRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_REJECT_OK);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);

		pParty->SetRequest(-1, "");
		if (pParty->GetMemberCountOnline() < 2)
		{
			// 파티 종료
			LONGLONG key = MAKE_LONGLONG_KEY(pParty->GetSubNo(), pParty->GetBossIndex());
			gserver.m_listParty.erase(key);
			pParty->SetEndParty();
			delete pParty;
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyRejectRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_REJECT_FAIL);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}
}

void do_CommandPartyQuitReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int			nBossIndex;
	int			nTargetIndex;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
	{
#ifdef PARTY_BUG_GER
		GAMELOG << init("PARTY_BUG_GER QUIT: NOT_PARTY")
				<< "BOSSINDEX" << delim << nBossIndex << delim
				<< "TARGETINDEX" << delim << nTargetIndex << end;

		gserver.PrintPartyMemberList(dest->m_subNo, nBossIndex);
		gserver.PrintPartyMemberList(dest->m_subNo, nTargetIndex);
#endif // PARTY_BUG_GER
		return ;
	}
	pParty->DeleteMember(nTargetIndex);

	if (pParty->GetMemberCountOnline() < 2)
	{
		// 파티 해체
		LONGLONG key = MAKE_LONGLONG_KEY(pParty->GetSubNo(), pParty->GetBossIndex());
		gserver.m_listParty.erase(key);
		pParty->SetEndParty();
		delete pParty;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyQuitRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_QUIT_END, -1, "");
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		}

		// 제거 파티는 매칭 테이블에서 제거
		do_CommandPartyMatchDelReq_real(dest->m_subNo, nBossIndex);
	}
#ifdef BATTLE_PARTY_BOSS_CHANGE
	else if (pParty != NULL &&
		pParty->GetPartyType(0) == MSG_PARTY_TYPE_BATTLE &&
		pParty->GetPartyType(3) == MSG_PARTY_TYPE_BATTLE &&
		pParty->GetPartyType(4) == MSG_PARTY_TYPE_BATTLE &&
		nBossIndex == nTargetIndex)
	{
		// 전투파티장 후보 찾기
		CPartyMember* member;
		bool foundBossCandidate = false;
		int min = -1;
		int max = -1;
		int i;
		for(i = 0; i < MAX_PARTY_MEMBER; i++)
		{
			member = pParty->m_listMember[i];
			if(member) // 파티에서 레벨이 가장 낮은 사람, 가장 높은 사람 찾기
			{
				if(min == -1 && member->m_nLevel > 0)
					min = member->m_nLevel;
				else if(min > member->m_nLevel && member->m_nLevel > 0)
					min = member->m_nLevel;
				if(max == -1 && member->m_nLevel > 0)
					max = member->m_nLevel;
				else if(max < member->m_nLevel && member->m_nLevel > 0)
					max = member->m_nLevel;
			}
		}
		for(i = 0; i < MAX_PARTY_MEMBER; i++) // 파장 후보 찾기
		{
			member = pParty->m_listMember[i];
			if(member)
			{
				if(member->m_nLevel > 0)
				{
					if(ABS(max - member->m_nLevel) < 10 && ABS(min - member->m_nLevel) < 10)
					{
						foundBossCandidate = true;
						break;
					}
				}
			}
		}
		if(member == NULL) // member가 새로운 파티장이 될 거임.
			foundBossCandidate = false;

		if(foundBossCandidate == true)
		{
			// 보스 변경
			CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);
			CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
			strTargetName = member->GetCharName();
			strBossName = pParty->GetMemberByListIndex(0)->GetCharName();
			if (strTargetName == NULL)
			{
				// 파티해체
				LONGLONG key = MAKE_LONGLONG_KEY(pParty->GetSubNo(), pParty->GetBossIndex());
				gserver.m_listParty.erase(key);
				pParty->SetEndParty();
				delete pParty;
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperPartyQuitRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_QUIT_END, -1, "");
					gserver.SendToAllSubServer(rmsg, dest->m_subNo);
				}

					// 제거 파티는 매칭 테이블에서 제거
				do_CommandPartyMatchDelReq_real(dest->m_subNo, nBossIndex);
			}
			else
			{
				if(pParty)
				{
					if (pParty->ChangeBoss(strTargetName))
					{
						CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nBossIndex);
						if (pMatchParty)
						{
							pMatchParty->SetBossIndex(pParty->GetBossIndex());
							pMatchParty->SetBossName(pParty->GetMemberByListIndex(0)->GetCharName());

							LONGLONG key = MAKE_LONGLONG_KEY(dest->m_subNo, nBossIndex);
							gserver.m_listPartyMatchParty.erase(key);
							key = MAKE_LONGLONG_KEY(dest->m_subNo, pParty->GetBossIndex());
							gserver.m_listPartyMatchParty.insert(map_partymatchparty_t::value_type(key, pMatchParty));
						}

						CNetMsg::SP rmsg(new CNetMsg);
						HelperPartyQuitRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_QUIT_OK, pParty->GetBossIndex(), pParty->GetMemberByListIndex(0)->GetCharName());
						gserver.SendToAllSubServer(rmsg, dest->m_subNo);
					}
				}
			}
		}
		else if(foundBossCandidate == false)
		{
			// 파티해체
			LONGLONG key = MAKE_LONGLONG_KEY(pParty->GetSubNo(), pParty->GetBossIndex());
			gserver.m_listParty.erase(key);
			pParty->SetEndParty();
			delete pParty;

			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyQuitRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_QUIT_END, -1, "");
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);

			// 제거 파티는 매칭 테이블에서 제거
			do_CommandPartyMatchDelReq_real(dest->m_subNo, nBossIndex);
		}
	}
#endif // BATTLE_PARTY_BOSS_CHANGE
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);

		// 보스 변경 검사
		if (pParty->GetBossIndex() != nBossIndex)
		{
			CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nBossIndex);
			if (pMatchParty)
			{
				pMatchParty->SetBossIndex(pParty->GetBossIndex());
				pMatchParty->SetBossName(pParty->GetMemberByListIndex(0)->GetCharName());

				LONGLONG key = MAKE_LONGLONG_KEY(dest->m_subNo, nBossIndex);
				gserver.m_listPartyMatchParty.erase(key);
				key = MAKE_LONGLONG_KEY(dest->m_subNo, pParty->GetBossIndex());
				gserver.m_listPartyMatchParty.insert(map_partymatchparty_t::value_type(key, pMatchParty));
			}
			HelperPartyQuitRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_QUIT_OK, pParty->GetBossIndex(), pParty->GetMemberByListIndex(0)->GetCharName());
		}
		else
		{
			CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nBossIndex);
			if (pMatchParty)
			{
				pMatchParty->SetMemberCount( pParty->GetMemberCount() );
			}
			HelperPartyQuitRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_QUIT_OK, nBossIndex, pParty->GetMemberByListIndex(0)->GetCharName());
		}

		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
	}
}

void do_CommandPartyKickReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int			nBossIndex;
	int			nTargetIndex;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
	{
#ifdef PARTY_BUG_GER
		GAMELOG << init("PARTY_BUG_GER KICK: NOT_PARTY")
				<< "BOSSINDEX" << delim << nBossIndex << delim
				<< "TARGETINDEX" << delim << nTargetIndex << end;

		gserver.PrintPartyMemberList(dest->m_subNo, nBossIndex);
		gserver.PrintPartyMemberList(dest->m_subNo, nTargetIndex);
#endif // PARTY_BUG_GER
		return ;
	}

	pParty->DeleteMember(nTargetIndex);

	if (pParty->GetMemberCountOnline() < 2)
	{
		// 파티 해체
		LONGLONG key = MAKE_LONGLONG_KEY(pParty->GetSubNo(), pParty->GetBossIndex());
		gserver.m_listParty.erase(key);
		pParty->SetEndParty();
		delete pParty;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyKickRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_KICK_END);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);

		// 제거 파티는 매칭 테이블에서 제거
		do_CommandPartyMatchDelReq_real(dest->m_subNo, nBossIndex);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyKickRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_KICK_OK);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
	}
}

void do_CommandPartyChangeBossReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> nBossIndex
		>> strTargetName;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
	{
#ifdef PARTY_BUG_GER
		GAMELOG << init("PARTY_BUG_GER CHANGEBOSS: NOT_BOSS")
				<< "BOSS INDEX" << delim << nBossIndex << delim
				<< "TARGET NAME" << delim << strTargetName
				<< end;

		CParty* party = gserver.FindPartyByMemberIndex(dest->m_subNo, nBossIndex, true);
		if(party)
		{
			GAMELOG << init("PARTY_BUG_GER CHANGEBOSS: NOT_PARTY")
					<< "REAL BOSS INDEX" << delim << party->GetBossIndex()
					<< end;
		}
#endif // PARTY_BUG_GER
		return ;
	}

	strBossName = pParty->GetMemberByListIndex(0)->GetCharName();

	if (pParty->ChangeBoss(strTargetName))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyChangeBossRepMsg(rmsg, nBossIndex, strBossName, pParty->GetBossIndex(), strTargetName);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);

		CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nBossIndex);
		if (pMatchParty)
		{
			pMatchParty->SetBossIndex(pParty->GetBossIndex());
			pMatchParty->SetBossName(strTargetName);

			LONGLONG key = MAKE_LONGLONG_KEY(dest->m_subNo, nBossIndex);
			gserver.m_listPartyMatchParty.erase(key);
			key = MAKE_LONGLONG_KEY(dest->m_subNo, pParty->GetBossIndex());
			gserver.m_listPartyMatchParty.insert(map_partymatchparty_t::value_type(key, pMatchParty));
		}
	}
#ifdef PARTY_BUG_GER
	else
	{
		GAMELOG << init("PARTY_BUG_GER CHANGEBOSS: FAILED")
				<< "BOSS INDEX" << delim << nBossIndex << delim
				<< "TARGET NAME" << delim << strTargetName
				<< end;
	}
#endif // PARTY_BUG_GER
}

void do_CommandPartyInviteCancel(CNetMsg::SP& msg, CDescriptor* dest)
{
	int nTargetIndex;

	RefMsg(msg) >> nTargetIndex;

	CParty* pParty = gserver.FindPartyByMemberIndex(dest->m_subNo, nTargetIndex, true);
	if(!pParty)
	{
		return ;
	}

	if(pParty->GetRequestIndex() == nTargetIndex)
	{
		pParty->SetRequest(-1, "");
		if ( pParty->GetMemberCountOnline() < 2)
		{
			// 파티 종료
			LONGLONG key = MAKE_LONGLONG_KEY(pParty->GetSubNo(), pParty->GetBossIndex());
			gserver.m_listParty.erase(key);
			pParty->SetEndParty();
			delete pParty;
		}

		GAMELOG << init("PARTY INVITE CANCEL")
				<< "TARGET INDEX" << delim
				<< nTargetIndex
				<< end;
	}
}

void do_CommandPartyOffline( CNetMsg::SP& msg, CDescriptor* dest)
{
	int			nBossIndex;
	int			nTargetIndex;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex;

#ifdef PARTY_BUG_GER
	GAMELOG << init("PARTY_BUG_GER OFFLINE")
			<< "BOSS INDEX" << delim << nBossIndex << delim
			<< "TARGET INDEX" << delim << nTargetIndex
			<< end;
#endif // PARTY_BUG_GER

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
	{
#ifdef PARTY_BUG_GER
		GAMELOG << init("PARTY_BUG_GER OFFLINE: NOT_PARTY")
				<< "BOSSINDEX" << delim << nBossIndex << delim
				<< "TARGETINDEX" << delim << nTargetIndex
				<< end;

		gserver.PrintPartyMemberList(dest->m_subNo, nBossIndex);
		gserver.PrintPartyMemberList(dest->m_subNo, nTargetIndex);
#endif // #ifdef PARTY_BUG_GER
		return ;
	}

	int idx = pParty->FindMember(nTargetIndex);
	CPartyMember *member = pParty->GetMemberByListIndex( idx );
	if( member )
	{
		member->m_nLevel = 0;
	}
	else
	{
#ifdef PARTY_BUG_GER
		GAMELOG << init("PARTY_BUG_GER OFFLINE: TARGET MEMBER NOT FOUND")
				<< "TARGET INDEX" << delim << nTargetIndex
				<< end;
#endif // PARTY_BUG_GER
	}
	if (pParty->GetMemberCountOnline() < 2)
	{
#ifdef PARTY_BUG_GER
		GAMELOG << init("PARTY_BUG_GER OFFLINE: PARTY END")
				<< "BOSS INDEX" << delim << nBossIndex << end;
		gserver.PrintPartyMemberList(dest->m_subNo, nBossIndex);
#endif // PARTY_BUG_GER

		// 파티 해체
		LONGLONG key = MAKE_LONGLONG_KEY(pParty->GetSubNo(), pParty->GetBossIndex());
		gserver.m_listParty.erase(key);
		pParty->SetEndParty();
		delete pParty;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyQuitRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_QUIT_END, -1, "");
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);

		// 제거 파티는 매칭 테이블에서 제거
		do_CommandPartyMatchDelReq_real(dest->m_subNo, nBossIndex);
	}
#ifdef BATTLE_PARTY_BOSS_CHANGE
	else if (pParty != NULL &&
		pParty->GetPartyType(0) == MSG_PARTY_TYPE_BATTLE &&
		pParty->GetPartyType(3) == MSG_PARTY_TYPE_BATTLE &&
		pParty->GetPartyType(4) == MSG_PARTY_TYPE_BATTLE &&
		nBossIndex == nTargetIndex)
	{
		// 전투파티장 후보 찾기
		CPartyMember* member;
		bool foundBossCandidate = false;
		int min = -1;
		int max = -1;
		int i;
		for(i = 0; i < MAX_PARTY_MEMBER; i++)
		{
			member = pParty->m_listMember[i];
			if(member) // 파티에서 레벨이 가장 낮은 사람, 가장 높은 사람 찾기
			{
				if(min == -1 && member->m_nLevel > 0)
					min = member->m_nLevel;
				else if(min > member->m_nLevel && member->m_nLevel > 0)
					min = member->m_nLevel;
				if(max == -1 && member->m_nLevel > 0)
					max = member->m_nLevel;
				else if(max < member->m_nLevel && member->m_nLevel > 0)
					max = member->m_nLevel;
			}
		}
		for(i = 0; i < MAX_PARTY_MEMBER; i++) // 파장 후보 찾기
		{
			member = pParty->m_listMember[i];
			if(member)
			{
				if(member->m_nLevel > 0)
				{
					if(ABS(max - member->m_nLevel) < 10 && ABS(min - member->m_nLevel) < 10)
					{
						foundBossCandidate = true;
						break;
					}
				}
			}
		}
		if(member == NULL) // member가 새로운 파티장이 될 거임.
			foundBossCandidate = false;

		if(foundBossCandidate == true)
		{
			// 보스 변경
			CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
			CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);
			strTargetName = member->GetCharName();
			strBossName = pParty->GetMemberByListIndex(0)->GetCharName();
			if (strTargetName == NULL)
			{
				// 파티해체
				LONGLONG key = MAKE_LONGLONG_KEY(pParty->GetSubNo(), pParty->GetBossIndex());
				gserver.m_listParty.erase(key);
				pParty->SetEndParty();
				delete pParty;

				CNetMsg::SP rmsg(new CNetMsg);
				HelperPartyQuitRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_QUIT_END, -1, "");
				gserver.SendToAllSubServer(rmsg, dest->m_subNo);

				// 제거 파티는 매칭 테이블에서 제거
				do_CommandPartyMatchDelReq_real(dest->m_subNo, nBossIndex);
			}
			else
			{
				if(pParty)
				{
					if (pParty->ChangeBoss(strTargetName))
					{
						CNetMsg::SP rmsg(new CNetMsg);
						HelperPartyChangeBossRepMsg(rmsg, nBossIndex, strBossName, pParty->GetBossIndex(), strTargetName);
						gserver.SendToAllSubServer(rmsg, dest->m_subNo);
						CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nBossIndex);
						if (pMatchParty)
						{
							pMatchParty->SetBossIndex(pParty->GetBossIndex());
							pMatchParty->SetBossName(strTargetName);

							LONGLONG key = MAKE_LONGLONG_KEY(dest->m_subNo, nBossIndex);
							gserver.m_listPartyMatchParty.erase(key);
							key = MAKE_LONGLONG_KEY(dest->m_subNo, pParty->GetBossIndex());
							gserver.m_listPartyMatchParty.insert(map_partymatchparty_t::value_type(key, pMatchParty));
						}
					}
				}
			}
		}
		else if(foundBossCandidate == false)
		{
			// 파티해체
			LONGLONG key = MAKE_LONGLONG_KEY(pParty->GetSubNo(), pParty->GetBossIndex());
			gserver.m_listParty.erase(key);
			pParty->SetEndParty();
			delete pParty;

			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyQuitRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_QUIT_END, -1, "");
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);

			// 제거 파티는 매칭 테이블에서 제거
			do_CommandPartyMatchDelReq_real(dest->m_subNo, nBossIndex);
		}
	}
#endif // BATTLE_PARTY_BOSS_CHANGE
	else
	{
		// 보스 변경 검사
		if ( pParty->GetBossIndex() == nTargetIndex)
		{
			CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
			CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);
			CPartyMember *newBoss = NULL;

#ifdef PARTY_BUG_GER
			GAMELOG << init("PARTY_BUG_GER OFFLINE: CHANGEBOSS")
					<< "BOSS INDEX" << delim << nBossIndex
					<< end;
#endif // PARTY_BUG_GER

			for(int i=0; i<MAX_PARTY_MEMBER ; i++ )
			{
				newBoss = pParty->GetMemberByListIndex(i);
				if( newBoss && newBoss->m_nLevel > 0 )
				{
					strTargetName = newBoss->GetCharName();
					break;
				}
			}
			strBossName = pParty->GetMemberByListIndex(0)->GetCharName();

			if( strTargetName == NULL )
			{
				// 파티 해체
				LONGLONG key = MAKE_LONGLONG_KEY(pParty->GetSubNo(), pParty->GetBossIndex());
				gserver.m_listParty.erase(key);
				pParty->SetEndParty();
				delete pParty;

				CNetMsg::SP rmsg(new CNetMsg);
				HelperPartyQuitRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_HELPER_PARTY_ERROR_QUIT_END, -1, "");
				gserver.SendToAllSubServer(rmsg, dest->m_subNo);
				pParty = NULL;

				// 제거 파티는 매칭 테이블에서 제거
				do_CommandPartyMatchDelReq_real(dest->m_subNo, nBossIndex);
			}

			if(pParty)
			{
				if (pParty->ChangeBoss(strTargetName))
				{
#ifdef PARTY_BUG_GER
					GAMELOG << init("PARTY_BUG_GER OFFLINE: CHANGEBOSS")
							<< "Prev" << delim << nBossIndex << delim
							<< "After" << delim << pParty->GetBossIndex()
							<< end;
#endif // PARTY_BUG_GER

					CNetMsg::SP rmsg(new CNetMsg);
					HelperPartyChangeBossRepMsg(rmsg, nBossIndex, strBossName, pParty->GetBossIndex(), strTargetName);
					gserver.SendToAllSubServer(rmsg, dest->m_subNo);

					CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nBossIndex);
					if (pMatchParty)
					{
						pMatchParty->SetBossIndex(pParty->GetBossIndex());
						pMatchParty->SetBossName(strTargetName);

						LONGLONG key = MAKE_LONGLONG_KEY(dest->m_subNo, nBossIndex);
						gserver.m_listPartyMatchParty.erase(key);
						key = MAKE_LONGLONG_KEY(dest->m_subNo, pParty->GetBossIndex());
						gserver.m_listPartyMatchParty.insert(map_partymatchparty_t::value_type(key, pMatchParty));
					}
				}
			}
		}
	}
}

void do_CommandPartyOnline( CNetMsg::SP& msg, CDescriptor* dest)
{
	int			nBossIndex;
	int			nTargetIndex;
	int			nTargetLevel=0;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex
		>> nTargetLevel;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
	{
#ifdef PARTY_BUG_GER
		GAMELOG << init("PARTY_BUG_GER ONLINE: NOT_PARTY")
				<< "BOSSINDEX" << delim << nBossIndex << delim
				<< "TARGETINDEX" << delim << nTargetIndex
				<< end;
		gserver.PrintPartyMemberList(dest->m_subNo, nBossIndex);
		gserver.PrintPartyMemberList(dest->m_subNo, nTargetIndex);

#endif // #ifdef PARTY_BUG_GER
		return ;
	}

	int idx = pParty->FindMember(nTargetIndex);
	CPartyMember *member = pParty->GetMemberByListIndex( idx );
	if( member )
		member->m_nLevel = nTargetLevel;
}

void do_CommandPartyChangeTypeReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int nBossIndex;
	char cPartyType,cDiviType,cAllOneSet;

	RefMsg(msg) >> nBossIndex
		>> cPartyType
		>> cDiviType
		>> cAllOneSet;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
		return ;

	if(cAllOneSet==MSG_PARTY_SETALLONE_ALL)	//대표 타입 설정
	{
		pParty->SetPartyTypeAll(cPartyType);
	}else if(cAllOneSet==MSG_PARTY_SETALLONE_ONE)	 //세분화 1개 설정
	{
		pParty->SetPartyType(cDiviType,cPartyType);
	}

	CNetMsg::SP rmsg(new CNetMsg);
	HelperPartyChangeTypeRepMsg(rmsg, nBossIndex, cPartyType,cDiviType,cAllOneSet);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);
}

//파티 해체
void do_CommandPartyEndPartyReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int nBossIndex;
	RefMsg(msg) >> nBossIndex;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
		return ;

	// 파티 해체
	LONGLONG key = MAKE_LONGLONG_KEY(pParty->GetSubNo(), pParty->GetBossIndex());
	gserver.m_listParty.erase(key);
	pParty->SetEndParty();
	delete pParty;

	CNetMsg::SP rmsg(new CNetMsg);
	HelperPartyEndPartyRepMsg(rmsg, nBossIndex);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);
}

void do_CommandPartyBreakReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int nBossIndex;

	RefMsg(msg) >> nBossIndex;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty)
		return ;

	// 파티 해체
	LONGLONG key = MAKE_LONGLONG_KEY(pParty->GetSubNo(), pParty->GetBossIndex());
	gserver.m_listParty.erase(key);
	pParty->SetEndParty();
	delete pParty;

	GAMELOG << init( "PARTY BREAK REQ" )
			<< "BOSSINDEX" << delim
			<< nBossIndex
			<< end;
}

void do_CommandPartyMatchRegMemberReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int				nCharIndex;
	CLCString		strCharName(MAX_CHAR_NAME_LENGTH + 1);
	int				nCharLevel;
	int				nZone;
	char			cJob;
	char			cPartyType;

	RefMsg(msg) >> nCharIndex
		>> strCharName
		>> nCharLevel
		>> nZone
		>> cJob
		>> cPartyType;

	CParty* pParty = gserver.FindPartyByMemberIndex(dest->m_subNo, nCharIndex, true);
	if (pParty)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchRegMemberRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_PARTY, nCharIndex, strCharName, nCharLevel, nZone, cJob, cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(dest->m_subNo, nCharIndex);
	if (pMatchMember)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchRegMemberRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_ALREADY_REG, nCharIndex, strCharName, nCharLevel, nZone, cJob, cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	pMatchMember = new CPartyMatchMember(dest->m_subNo, nCharIndex, strCharName, nCharLevel, nZone, cJob, cPartyType);
	LONGLONG key = MAKE_LONGLONG_KEY(pMatchMember->GetSubNo(), pMatchMember->GetIndex());
	gserver.m_listPartyMatchMember.insert(map_partymatchmember_t::value_type(key, pMatchMember));

	CNetMsg::SP rmsg(new CNetMsg);
	HelperPartyMatchRegMemberRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_REG_MEMBER_OK, nCharIndex, strCharName, nCharLevel, nZone, cJob, cPartyType);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);
}

void do_CommandPartyMatchRegPartyReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int				nBossIndex;
	CLCString		strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int				nBossLevel;
	int				nZone;
	int				nJobFlag;
	char			cLimitLevel;
	CLCString		strComment(PARTY_MATCHING_COMMENT + 1);

	RefMsg(msg) >> nBossIndex
		>> strBossName
		>> nBossLevel
		>> nZone
		>> nJobFlag
		>> cLimitLevel
		>> strComment;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (!pParty || pParty->GetMemberCount() < 2)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchRegPartyRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_NO_BOSS, nBossIndex, strBossName, nBossLevel, nZone, 0, nJobFlag, cLimitLevel, 0, strComment);
		SEND_Q(rmsg, dest);
		return ;
	}

	CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (pMatchParty)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchRegPartyRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_ALREADY_REG, nBossIndex, strBossName, nBossLevel, nZone, 0, nJobFlag, cLimitLevel, 0, strComment);
		SEND_Q(rmsg, dest);
		return ;
	}

	pMatchParty = new CPartyMatchParty(dest->m_subNo, nBossIndex, strBossName, nBossLevel, nZone, nJobFlag, pParty->GetPartyType(MSG_DIVITYPE_EXP), (cLimitLevel == 1) ? true : false, strComment);

	pMatchParty->SetMemberCount(pParty->GetMemberCount());

	LONGLONG key = MAKE_LONGLONG_KEY(pMatchParty->GetSubNo(), pMatchParty->GetBossIndex());
	gserver.m_listPartyMatchParty.insert(map_partymatchparty_t::value_type(key, pMatchParty));

	CNetMsg::SP rmsg(new CNetMsg);
	HelperPartyMatchRegPartyRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_OK, nBossIndex, strBossName, nBossLevel, nZone, pMatchParty->GetMemberCount() , nJobFlag, cLimitLevel, pParty->GetPartyType(MSG_DIVITYPE_EXP), strComment);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);
}

void do_CommandPartyMatchDelReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int				nCharIndex;
	RefMsg(msg) >> nCharIndex;
	do_CommandPartyMatchDelReq_real(dest->m_subNo, nCharIndex);
}

void do_CommandPartyMatchDelReq_real(int nSubNo, int nCharIndex)
{
	// 멤버 리스트에서 찾기
	CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(nSubNo, nCharIndex);
	if (pMatchMember)
	{
		LONGLONG key = MAKE_LONGLONG_KEY(pMatchMember->GetSubNo(), pMatchMember->GetIndex());
		gserver.m_listPartyMatchMember.erase(key);
		delete pMatchMember;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchDelRepMsg(rmsg, nCharIndex, MSG_HELPER_PARTY_MATCH_ERROR_DEL_MEMBER);
		gserver.SendToAllSubServer(rmsg, nSubNo);
		return ;
	}

	// 파티 리스트에서 찾기
	CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(nSubNo, nCharIndex);
	if (pMatchParty)
	{
		gserver.m_listPartyMatchParty.erase(MAKE_LONGLONG_KEY(pMatchParty->GetSubNo(), pMatchParty->GetBossIndex()));
		delete pMatchParty;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchDelRepMsg(rmsg, nCharIndex, MSG_HELPER_PARTY_MATCH_ERROR_DEL_PARTY);
		gserver.SendToAllSubServer(rmsg, nSubNo);
		return ;
	}
}

void do_CommandPartyMatchInviteReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nBossLevel;
	int			nCharIndex;

	RefMsg(msg) >> nBossIndex
		>> strBossName
		>> nBossLevel
		>> nCharIndex;

	// 대상의 파티 매칭 등록을 찾는다
	CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(dest->m_subNo, nCharIndex);
	if (!pMatchMember)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_NOT_FOUND, nBossIndex, strBossName, nCharIndex, "", 0);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 대상자의 희망 파티 타입을 구한다
	char		cPartyType = pMatchMember->GetPartyType();

	// 파티 타입이 PARTY_TYPE_BATTLE이면 tch와 ch의 레벨 차이를 검사
	// 차이가 +- 10이상이면 ERROR;
	if (cPartyType == MSG_PARTY_TYPE_BATTLE)
	{
		if (ABS(pMatchMember->GetLevel() - nBossLevel) > 10)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_LEVEL, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
			SEND_Q(rmsg, dest);
			return ;
		}
	}

	// 기존 파티를 찾는다
	CParty* pPartyBoss = gserver.FindPartyByMemberIndex(dest->m_subNo, nBossIndex, true);
	CParty* pPartyTarget = gserver.FindPartyByMemberIndex(dest->m_subNo, nCharIndex, true);

	// nBossIndex가 파티 초대가 가능한지 : 파티가 없거나 보스
	if (pPartyBoss && pPartyBoss->GetBossIndex() != nBossIndex)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_NOBOSS, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 파티 타입 검사
	if (pPartyBoss && pPartyBoss->GetPartyType(MSG_DIVITYPE_EXP) != cPartyType)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_DIFF_TYPE, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 대상 파티 검사
	if (pPartyTarget)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_ALREADY_TARGET, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 파티 인원 검사
	if (pPartyBoss && pPartyBoss->GetMemberCount() >= MAX_PARTY_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_FULL, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 현재 요청자 유무 검사
	if (pPartyBoss && pPartyBoss->GetRequestIndex() > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_ALREADY_REQUEST, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
		SEND_Q(rmsg, dest);
		return ;
	}

	if (pPartyBoss)
	{
		// 기존 파티에 요청자로 설정
		pPartyBoss->SetRequest(nCharIndex, pMatchMember->GetName());
	}
	else
	{
		// 신규 파티 생성
		pPartyBoss = new CParty(dest->m_subNo, cPartyType, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), nBossLevel );
		LONGLONG key = MAKE_LONGLONG_KEY(pPartyBoss->GetSubNo(), pPartyBoss->GetBossIndex());
		gserver.m_listParty.insert(map_party_t::value_type(key, pPartyBoss));
	}

	// 결과 전달
	CNetMsg::SP rmsg(new CNetMsg);
	HelperPartyMatchInviteRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_INVITE_OK, nBossIndex, strBossName, nCharIndex, pMatchMember->GetName(), cPartyType);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);
}

void do_CommandPartyMatchJoinReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int			nBossIndex;
	int			nCharIndex;
	CLCString	strCharName(MAX_CHAR_NAME_LENGTH + 1);
	int			nCharLevel;
	char		cCharJob;

	RefMsg(msg) >> nBossIndex
		>> nCharIndex
		>> strCharName
		>> nCharLevel
		>> cCharJob;

	// 파티 매칭에 등록된 것을 찾고
	CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (pMatchParty == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_NOT_FOUND, 0, nBossIndex, "", nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 파티를 찾고
	CParty*		pPartyBoss = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (pPartyBoss == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_NOT_FOUND, 0, nBossIndex, "", nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 파티 타입을 구하고
	char cPartyType = pPartyBoss->GetPartyType(MSG_DIVITYPE_EXP);

	// 파티 타입이 PARTY_TYPE_BATTLE이면 레벨 검사
	if (cPartyType == MSG_PARTY_TYPE_BATTLE)
	{
		if (ABS(pMatchParty->GetBossLevel() - nCharLevel) > 10)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_LEVEL, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
			SEND_Q(rmsg, dest);
			return ;
		}
	}

	// 파티 매칭에 레벨 제한이 있으면 레벨 검사
	if (pMatchParty->IsLimitLevel() && ABS(pMatchParty->GetBossLevel() - nCharLevel) > PARTY_MATCHING_LIMIT_LEVEL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_LIMIT_LEVEL, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 파티 매칭에 직업 제한이 있으면 직업 검사
	if ((pMatchParty->GetJobFlag() & (1 << cCharJob)) == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_JOB, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 파티 인원 검사
	if (pPartyBoss->GetMemberCount() >= MAX_PARTY_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_FULL, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 현재 요청자 검사
	if (pPartyBoss->GetRequestIndex() > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALREADY_REQUEST, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 요청한 사람의 파티 검사
	CParty*		pPartyTarget = gserver.FindPartyByMemberIndex(dest->m_subNo, nCharIndex, true);
	if (pPartyTarget)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALREADY_TARGET, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
		SEND_Q(rmsg, dest);
		return ;
	}

	// 기존 파티에 요청자로 설정
	pPartyBoss->SetRequest(nCharIndex, strCharName);

	// 결과 전달
	CNetMsg::SP rmsg(new CNetMsg);
	HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_OK, cPartyType, nBossIndex, pMatchParty->GetBossName(), nCharIndex, strCharName, cCharJob);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);
}

void do_CommandPartyMatchJoinAllowReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int			nBossIndex;
	int			nReqCharIndex;
	int			nLevel;
	int			nReqCharLevel;

	RefMsg(msg) >> nBossIndex
		>> nReqCharIndex;
	RefMsg(msg) >> nLevel
		>> nReqCharLevel;

	CParty* pParty = gserver.FindPartyByBossIndex(dest->m_subNo, nBossIndex);

	if (!pParty)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinAllowRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALLOW_NOT_PARTY, nBossIndex, nReqCharIndex, "");
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}

	if (pParty->GetRequestIndex() != nReqCharIndex)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinAllowRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALLOW_NOT_PARTY, nBossIndex, nReqCharIndex, "");
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}
///////////////////////////////////////////오토매칭중 전투파티로 파티타입 변환///////////////////////////////////////////
//////권상욱 2010-01-27
/*
	if(ABS(nLevel - nReqCharLevel) > 10)
	{
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_LEVEL, cPartyType, nBossIndex, pMatchParty->GetBossName(), nReqCharIndex, "", -1);
		SEND_Q(rmsg, dest);
		return ;
	}
*/
	CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nBossIndex);
	if (pMatchParty == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_NOT_FOUND, 0, nBossIndex, "", nReqCharIndex, "", -1);
		SEND_Q(rmsg, dest);
		return ;
	}

	char cPartyType = pParty->GetPartyType(MSG_DIVITYPE_EXP);

	// 파티 타입이 PARTY_TYPE_BATTLE이면 레벨 검사
	if (cPartyType == MSG_PARTY_TYPE_BATTLE)
	{
		if (ABS(pMatchParty->GetBossLevel() - nReqCharLevel) > 10)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyMatchJoinRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_LEVEL, cPartyType, nBossIndex, pMatchParty->GetBossName(), nReqCharIndex, "", -1);
			SEND_Q(rmsg, dest);
			return ;
		}
	}
///////////////////////////////////////////오토매칭중 전투파티로 파티타입 변환///////////////////////////////////////////
	// 파티원으로 등록
	CLCString strReqName(MAX_CHAR_NAME_LENGTH + 1);
	strReqName = pParty->GetRequestName();
	pParty->JoinRequest(strReqName, nLevel );

	CNetMsg::SP rmsg(new CNetMsg);
	HelperPartyMatchJoinAllowRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_ALLOW_OK, nBossIndex, nReqCharIndex, strReqName);
	gserver.SendToAllSubServer(rmsg, dest->m_subNo);

	// 파티에 참가하는 유저는 매칭 테이블에서 제거
	CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(dest->m_subNo, nReqCharIndex);
	if (pMatchMember)
		do_CommandPartyMatchDelReq_real(dest->m_subNo, nReqCharIndex);
	pMatchParty->SetMemberCount( pParty->GetMemberCount() );
}

void do_CommandPartyMatchJoinRejectReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int			nJoinCharIndex;
	int			nRejectCharIndex;

	RefMsg(msg) >> nJoinCharIndex
		>> nRejectCharIndex;

	// nJoinCharIndex로 파티 검색
	CParty* pParty = gserver.FindPartyByMemberIndex(dest->m_subNo, nJoinCharIndex, true);
	if (!pParty)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinRejectRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_REJECT_FAIL, nJoinCharIndex, nRejectCharIndex);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}

	// 요청자가 보스인지 일반인지 검사
	if (pParty->GetBossIndex() == nRejectCharIndex || pParty->GetRequestIndex() == nRejectCharIndex)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinRejectRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_REJECT_OK, nJoinCharIndex, nRejectCharIndex);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);

		pParty->SetRequest(-1, "");
		if (pParty->GetMemberCount() < 2)
		{
			// 제거 파티는 매칭 테이블에서 제거
			do_CommandPartyMatchDelReq_real(dest->m_subNo, pParty->GetBossIndex());

			// 파티 종료
			LONGLONG key = MAKE_LONGLONG_KEY(pParty->GetSubNo(), pParty->GetBossIndex());
			gserver.m_listParty.erase(key);
			pParty->SetEndParty();
			delete pParty;
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinRejectRepMsg(rmsg, MSG_HELPER_PARTY_MATCH_ERROR_JOIN_REJECT_FAIL, nJoinCharIndex, nRejectCharIndex);
		gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		return ;
	}
}

void do_CommandPartyMatchMemberChangeInfoName(CNetMsg::SP& msg, CDescriptor* dest)
{
	int			nCharIndex;
	int			nType;
	CLCString	strCharName(MAX_CHAR_NAME_LENGTH + 1);
	int			nLevel;
	int			nZone;

	RefMsg(msg) >> nCharIndex
		>> nType;

	CPartyMatchMember* pMatchMember = gserver.FindPartyMatchMemberByCharIndex(dest->m_subNo, nCharIndex);
	CPartyMatchParty* pMatchParty = gserver.FindPartyMatchPartyByBossIndex(dest->m_subNo, nCharIndex);

	switch (nType)
	{
	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_NAME:
		RefMsg(msg) >> strCharName;
		if (pMatchMember)		pMatchMember->SetName(strCharName);
		if (pMatchParty)		pMatchParty->SetBossName(strCharName);
		if (pMatchMember || pMatchParty)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyMatchMemberChangeInfoMsg(rmsg, nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_NAME, strCharName, 0, 0);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		}
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL:
		RefMsg(msg) >> nLevel;
		if (pMatchMember)		pMatchMember->SetLevel(nLevel);
		if (pMatchParty)		pMatchParty->SetBossLevel(nLevel);
		if (pMatchMember || pMatchParty)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyMatchMemberChangeInfoMsg(rmsg, nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL, "", nLevel, 0);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		}
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
		if (pMatchMember || pMatchParty)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyMatchMemberChangeInfoMsg(rmsg, nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_ZONE, "", nLevel, nZone);
			gserver.SendToAllSubServer(rmsg, dest->m_subNo);
		}
		break;
	}
}

void do_CommandPetColorChange( CNetMsg::SP& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int nCharIndex;
	int	nPetIndex;
	char colorType;

	RefMsg(msg) >> nCharIndex
		>> nPetIndex
		>> colorType;

	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	if (colorType == 0)
	{
		sql.Format( "UPDATE t_pet SET a_color = '' WHERE a_index = %d AND a_owner = %d", nPetIndex, nCharIndex );
	}
	else
	{
		sql.Format( "UPDATE t_pet SET a_color = '%c' WHERE a_index = %d AND a_owner = %d", colorType, nPetIndex, nCharIndex );
	}
	cmd.SetQuery( sql );
	cmd.Update();
}

void do_CommandGuildInclineEstablish( CNetMsg::SP& msg, CDescriptor* dest )
{
	int guildindex;
	int charindex;
	char guildincline;
	LONGLONG needMoney;
	int needGP;

	RefMsg(msg) >> guildindex
		>> charindex
		>> guildincline;

	RefMsg(msg) >> needMoney
		>> needGP;

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	char update_sql[8192] = {0,};
	sprintf(update_sql, "update t_extend_guild set a_guild_incline= %d, a_guild_point = a_guild_point - %d where a_guild_index = %d", guildincline, needGP, guildindex);
	cmd.SetQuery(update_sql);
	if(!cmd.Update())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildInclineEstablishRepMsg(rmsg, guildindex, charindex, guildincline, MSG_NEW_GUILD_ERROR_CHANG_INCLINE );
		SEND_Q(rmsg, dest);
		return;
	}

	CGuild *guild = gserver.m_guildlist.findguild(guildindex);
	if( guild )
	{
		guild->incline(guildincline);
		guild->AddGuildPoint( -needGP );

		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildInclineEstablishRepMsg( rmsg, guildindex, charindex, guildincline, MSG_NEW_GUILD_ERROR_INCLINE_OK );
		RefMsg(rmsg) << needMoney << needGP;
		//SEND_Q(rmsg, dest);
		gserver.SendToAllGameServer(rmsg);
	}
}

void do_CommandGuildMemberAdjust( CNetMsg::SP& msg, CDescriptor* dest )
{
	int guildindex;
	int ownerindex;
	int charindex;
	CLCString strPositionName(GUILD_POSITION_NAME+1);
	int contributeExp;
	int contributeFame;
	int pos;
#ifdef DEV_GUILD_STASH
	char	cStashAuth = 0;
#endif //DEV_GUILD_STASH

	RefMsg(msg) >> guildindex
		>> ownerindex
		>> charindex
		>> strPositionName
		>> contributeExp
		>> contributeFame;
	RefMsg(msg) >> pos;
#ifdef DEV_GUILD_STASH
	RefMsg(msg) >> cStashAuth;
#endif //DEV_GUILD_STASH

	CGuildMember* gMember = gserver.m_guildlist.findmember( charindex );

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	cmd.BeginTrans();
	std::string update_extend_guild_member_query = "";
#ifdef DEV_GUILD_STASH
	update_extend_guild_member_query = boost::str(boost::format(
		"UPDATE t_extend_guildmember SET a_position_name = '%s', a_contribute_exp = %d, a_contribute_fame = %d, a_stash_auth = %d"
		" WHERE a_guild_index = %d AND a_char_index = %d")
		%(const char*)strPositionName % contributeExp % contributeFame % (int)cStashAuth % guildindex % charindex );
#else
	update_extend_guild_member_query = boost::str(boost::format(
		"UPDATE t_extend_guildmember SET a_position_name = '%s', a_contribute_exp = %d, a_contribute_fame = %d "
		" WHERE a_guild_index = %d AND a_char_index = %d ")
		%(const char*)strPositionName % contributeExp % contributeFame % guildindex % charindex );
#endif //DEV_GUILD_STASH

	cmd.SetQuery( update_extend_guild_member_query );

	if ( !cmd.Update() )
	{
		cmd.Rollback();
		GAMELOG << init( "GUILD_MEMBER_ADJUST_UPDATE_ERROR")
			<< mysql_error( &gserver.m_dbchar ) << end;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAdjustRepMsg( rmsg, guildindex, ownerindex, charindex, strPositionName, contributeExp, contributeFame, pos, MSG_NEW_GUILD_ERROR_ADJUST_FAIL );
		SEND_Q( rmsg, dest );
		return;
	}

	std::string update_guild_member_query = boost::str(boost::format(
		"UPDATE t_guildmember SET a_pos=%d WHERE a_guild_index = %d AND a_char_index = %d") % pos % guildindex % charindex );

	cmd.SetQuery( update_guild_member_query );
	if ( !cmd.Update() )
	{
		cmd.Rollback();
		GAMELOG << init( "GUILD_MEMBER_ADJUST_UPDATE_ERROR")
			<< mysql_error( &gserver.m_dbchar ) << end;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAdjustRepMsg( rmsg, guildindex, ownerindex, charindex, strPositionName, contributeExp, contributeFame, pos, MSG_NEW_GUILD_ERROR_ADJUST_FAIL );
		SEND_Q( rmsg, dest );
		return;
	}
	cmd.Commit();

	if( gMember )
	{
		gMember->positionName( strPositionName );
		gMember->contributeExp( contributeExp );
		gMember->contributeFame( contributeFame );
		gMember->guild()->ChangeGradeExPosCount(gMember->pos(), pos);
		gMember->pos(pos);

		int needGP = gMember->guild()->GetGradeExPosNeedGuilPoint( pos );
		if( needGP > 0 && needGP <= gMember->guild()->GetGuildPoint() )
		{
			int gp = gMember->guild()->GetGuildPoint();
			if(needGP >= gp)
				gp = 0;
			else
				gp = gp - needGP;

			cmd.Init(&gserver.m_dbchar);
			std::string update_extend_guild = boost::str(boost::format(
				"UPDATE t_extend_guild set a_guild_point = %d where a_guild_index = %d") % gp % guildindex);
			cmd.SetQuery(update_extend_guild);
			if(!cmd.Update())
			{
				GAMELOG << init("GUILD POINT UPDATE ERROR :: CHANGE GRADE")
						<< "GUILD INDEX" << delim << guildindex << delim
						<< "USE GUILD POINT" << delim << needGP
						<< end;
				return ;
			}
			else
			{
				GAMELOG << init("GUILD POINT UPDATE :: CHANGE GRADE")
						<< "GUILD INDEX" << delim
						<< guildindex << delim
						<< "USE GUILD POINT" << delim
						<< needGP << delim
						<< "TOTAL GUILD POINT" << delim
						<< gp
						<< end;
			}
			gMember->guild()->guildPoint( gp );
		}
	}

	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildMemberAdjustRepMsg( rmsg, guildindex, ownerindex, charindex, strPositionName, contributeExp, contributeFame, pos, MSG_NEW_GUILD_ERROR_ADJUST_OK );
	gserver.SendToAllGameServer(rmsg);
}

void do_CommandNewGuildInfo( CNetMsg::SP& msg, CDescriptor* dest )
{
	int guildindex;
	int charindex;

	RefMsg(msg) >> guildindex
		>> charindex;

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

//	SELECT g.a_index, g.a_name, g.a_level, eg.a_guild_point FROM t_guild as g, t_extend_guild as eg
//	WHERE g.a_index = eg.a_guild_index AND g.a_enable != 0
//	AND g.a_index = 1;
	char select_guild_query[8192] = {0,};
	sprintf( select_guild_query, "SELECT g.a_index, g.a_name, g.a_level, eg.a_guild_point FROM t_guild as g, t_extend_guild as eg "
		" WHERE g.a_index = eg.a_guild_index AND g.a_enable != 0 AND g.a_index = %d ", guildindex );

	cmd.SetQuery( select_guild_query );

	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildInfoRepMsg( rmsg, charindex, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, dest );
		return;
	}

//	int guildpoint;
//	cmd.GetRec( "a_guild_point", guildpoint );

	char select_guild_member_query[8192] = {0,};
	sprintf( select_guild_member_query, "SELECT g.a_guild_index, g.a_char_index, g.a_char_name, g.a_pos, eg.a_point, eg.a_position_name FROM t_guildmember as g, t_extend_guildmember as eg "
		" WHERE g.a_guild_index = eg.a_guild_index AND g.a_char_index = eg.a_char_index "
		" AND g.a_guild_index = %d AND g.a_char_index = %d ", guildindex, charindex );

	cmd.SetQuery( select_guild_member_query );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildInfoRepMsg( rmsg, charindex, MSG_GUILD_ERROR_NOTBOSS );
		SEND_Q( rmsg, dest );
		return;
	}
	int usepoint;
	CLCString positionname( GUILD_POSITION_NAME + 1 );
	cmd.GetRec( "a_point", usepoint );
	cmd.GetRec( "a_position_name", positionname );

	char select_guild_member_query_1[8192] = {0,};
	sprintf( select_guild_member_query_1, "SELECT avg( c.a_level ) as ave_level FROM t_guildmember as gm, t_characters as c "
		" WHERE gm.a_char_index = c.a_index AND gm.a_guild_index = %d ", guildindex );

	cmd.SetQuery( select_guild_member_query_1 );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildInfoRepMsg( rmsg, charindex, MSG_GUILD_ERROR_NOTBOSS );
		SEND_Q( rmsg, dest );
		return;
	}

	int avelevel;
	cmd.GetRec( "ave_level", avelevel );

	CNetMsg::SP rmsg(new CNetMsg);
	HelperNewGuildInfoNotifyMsg( rmsg, charindex, guildindex, avelevel, /* guildpoint,  */ usepoint,  MSG_NEW_GUILD_INFO_ERROR_OK );
	SEND_Q( rmsg, dest );
}

void do_CommandNewGuildMemberList( CNetMsg::SP& msg, CDescriptor* dest )
{
	int guildindex;
	int charindex;

	RefMsg(msg) >> guildindex
		>> charindex;

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	char select_guild_member_query[8192] = {0,};
	sprintf( select_guild_member_query, "SELECT g.a_guild_index as a_guild_index, g.a_char_index as a_char_index, g.a_char_name as a_char_name, g.a_pos as a_pos, eg.a_position_name as a_position_name, eg.a_point as a_point, c.a_job as a_job, c.a_job2 as a_job2, c.a_level as a_level  "
					" FROM t_guildmember as g LEFT JOIN t_extend_guildmember as eg ON g.a_guild_index = eg.a_guild_index "
					" LEFT JOIN t_characters as c ON g.a_char_index = c.a_index "
					" WHERE g.a_char_index = eg.a_char_index AND g.a_guild_index = %d AND c.a_enable = 1", guildindex );

	cmd.SetQuery( select_guild_member_query );

	int endcount = 0;
	int guildstart = 0;

	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildMemberListRepMsg( rmsg, endcount, guildstart, charindex, guildindex, MSG_GUILD_ERROR_NOTBOSS, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		SEND_Q( rmsg, dest );
		return;
	}
	int membercount		= 0;
	int membercharindex[GUILD_MAX_MEMBER];
	int cumulatePoint[GUILD_MAX_MEMBER];
	char charName[GUILD_MAX_MEMBER][MAX_CHAR_NAME_LENGTH  + 1];
	char positionName[GUILD_MAX_MEMBER][GUILD_POSITION_NAME+1];
	char job[GUILD_MAX_MEMBER];
	char job2[GUILD_MAX_MEMBER];
	int level[GUILD_MAX_MEMBER];
	int position[GUILD_MAX_MEMBER];
	CLCString strCharName( MAX_CHAR_NAME_LENGTH  + 1 );
	CLCString strPositionName( GUILD_POSITION_NAME+1 );

	membercount = cmd.m_nrecords;
	int i = 0;

	CGuild * guild = gserver.m_guildlist.findguild(guildindex);
	if( guild )
		guild->InitGradeExPosCount();

	do
	{
		cmd.GetRec( "a_char_index", membercharindex[i] );
		cmd.GetRec( "a_point", cumulatePoint[i] );
		cmd.GetRec( "a_char_name", strCharName );
		if( !cmd.GetRec( "a_position_name",strPositionName ) )
		{
			strPositionName = '\0';
		}
		cmd.GetRec( "a_job", job[i] );
		cmd.GetRec( "a_job2", job2[i] );
		cmd.GetRec( "a_level", level[i] );
		cmd.GetRec( "a_pos", position[i] );
		strcpy( charName[i], strCharName );
		strcpy( positionName[i], strPositionName );
		if( guild )
			guild->AddGradeExPosCount( position[i] );

		i++;
	}while( cmd.MoveNext());

	int loopcount = membercount / 5;

	for( i = 0; i < loopcount; i++ )
	{
		endcount = (i + 1) * 5;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildMemberListRepMsg( rmsg, endcount, guildstart, charindex, guildindex, MSG_NEW_GUILD_MEMBERLIST_ERROR_OK,
				 membercharindex, cumulatePoint, charName, positionName, job, job2, level, position );
		SEND_Q( rmsg, dest );
	}

	int remaincount = membercount - guildstart;
	if( remaincount > 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildMemberListRepMsg( rmsg, membercount, guildstart, charindex, guildindex, MSG_NEW_GUILD_MEMBERLIST_ERROR_OK,
				 membercharindex, cumulatePoint, charName, positionName, job, job2, level, position );
		SEND_Q( rmsg, dest );
	}

	return;
}

void do_CommandNewGuildManage( CNetMsg::SP& msg, CDescriptor* dest )
{
	int guildindex;
	int charindex;

	RefMsg(msg) >> guildindex
		>> charindex;

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	char select_guild_member_query[8192] = {0,};
#ifdef DEV_GUILD_STASH
	sprintf( select_guild_member_query, "SELECT g.a_guild_index as a_guild_index, g.a_char_index as a_char_index, g.a_char_name as a_char_name, g.a_pos as a_pos, eg.a_position_name as a_position_name, eg.a_contribute_exp as a_contribute_exp, eg.a_contribute_fame as a_contribute_fame, c.a_level as a_level, eg.a_stash_auth as a_stash_auth  "
#else
	sprintf( select_guild_member_query, "SELECT g.a_guild_index as a_guild_index, g.a_char_index as a_char_index, g.a_char_name as a_char_name, g.a_pos as a_pos, eg.a_position_name as a_position_name, eg.a_contribute_exp as a_contribute_exp, eg.a_contribute_fame as a_contribute_fame, c.a_level as a_level "
#endif //DEV_GUILD_STASH
			" FROM t_guildmember as g LEFT JOIN  t_extend_guildmember as eg ON g.a_guild_index = eg.a_guild_index "
			" LEFT JOIN t_characters as c ON g.a_char_index = c.a_index "
			" WHERE g.a_char_index = eg.a_char_index AND g.a_guild_index = %d AND c.a_enable = 1", guildindex );

	cmd.SetQuery( select_guild_member_query );

	int endcount = 0;
	int guildstart = 0;

	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
#ifdef DEV_GUILD_STASH
		HelperNewGuildManageRepMsg( rmsg, endcount, guildstart, charindex, guildindex, MSG_GUILD_ERROR_NOTBOSS, NULL, NULL, NULL, NULL , NULL, NULL, NULL, NULL, 1);
#else
		HelperNewGuildManageRepMsg( rmsg, endcount, guildstart, charindex, guildindex, MSG_GUILD_ERROR_NOTBOSS, NULL, NULL, NULL, NULL , NULL, NULL, NULL, 1);
#endif //DEV_GUILD_STASH
		SEND_Q( rmsg, dest );
		return;
	}
	int membercount		= 0;
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
	char stashAuth[GUILD_MAX_MEMBER];
#endif //DEV_GUILD_STASH

	membercount = cmd.m_nrecords;
	int i = 0;

	CGuild * guild = gserver.m_guildlist.findguild(guildindex);
	if( guild )
		guild->InitGradeExPosCount();

	do
	{
		cmd.GetRec( "a_char_index", membercharindex[i] );
		cmd.GetRec( "a_contribute_exp", contributeExp[i] );
		cmd.GetRec( "a_contribute_fame", contributeFame[i] );
		cmd.GetRec( "a_char_name", strCharName );
		if( !cmd.GetRec( "a_position_name",strPositionName ) )
		{
			strPositionName = '\0';
		}
		cmd.GetRec( "a_level", level[i] );
		cmd.GetRec( "a_pos", position[i] );
		strcpy( charName[i], strCharName );
		strcpy( positionName[i], strPositionName );
		if( guild )
			guild->AddGradeExPosCount( position[i] );

#ifdef DEV_GUILD_STASH
		cmd.GetRec( "a_stash_auth", stashAuth[i] );
#endif //DEV_GUILD_STASH
	i++;
	}while( cmd.MoveNext());

	int loopcount = membercount / 5;
	char first = 1;			// 1: 처음보냄, 2: 나중에 보냄

	for( i = 0; i < loopcount; i++ )
	{
		endcount = (i + 1) * 5;

		CNetMsg::SP rmsg(new CNetMsg);
#ifdef DEV_GUILD_STASH
		HelperNewGuildManageRepMsg( rmsg, endcount, guildstart, charindex, guildindex, MSG_NEW_GUILD_MANAGE_ERROR_OK,  membercharindex, contributeExp, contributeFame, charName, positionName, level, position, stashAuth, first );
#else
		HelperNewGuildManageRepMsg( rmsg, endcount, guildstart, charindex, guildindex, MSG_NEW_GUILD_MANAGE_ERROR_OK,  membercharindex, contributeExp, contributeFame, charName, positionName, level, position, first );
#endif //
		SEND_Q( rmsg, dest );

		first = 0;
	}

	int remaincount = membercount - guildstart;
	if( remaincount > 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
#ifdef DEV_GUILD_STASH
		HelperNewGuildManageRepMsg( rmsg, membercount, guildstart, charindex, guildindex, MSG_NEW_GUILD_MANAGE_ERROR_OK,  membercharindex, contributeExp, contributeFame, charName, positionName, level, position, stashAuth, first );
#else
		HelperNewGuildManageRepMsg( rmsg, membercount, guildstart, charindex, guildindex, MSG_NEW_GUILD_MANAGE_ERROR_OK,  membercharindex, contributeExp, contributeFame, charName, positionName, level, position, first );
#endif //DEV_GUILD_STASH
		SEND_Q( rmsg, dest );
	}

	return;
}

void do_CommandNewGuildNotice( CNetMsg::SP& msg, CDescriptor* dest )
{
	int guildindex;
	int charindex;

	RefMsg(msg) >> guildindex
		>> charindex;

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	// SELECT * FROM t_guild_notice WHERE a_guild_index = %d

	char select_guild_notice_query[8192] = {0,};
	sprintf( select_guild_notice_query, "SELECT * FROM t_guild_notice WHERE a_guild_index = %d", guildindex );

	cmd.SetQuery( select_guild_notice_query );

	if( !cmd.Open() || !cmd.MoveFirst()  )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildNoticeRepMsg( rmsg, charindex, guildindex, NULL, NULL );
		SEND_Q( rmsg, dest );
		return;
	}
	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );

	cmd.GetRec( "a_title", title );
	cmd.GetRec( "a_text", text );

	CNetMsg::SP rmsg(new CNetMsg);
	HelperNewGuildNoticeRepMsg( rmsg, charindex, guildindex, title, text );
	SEND_Q( rmsg, dest );
	return;
}

void do_CommandNewGuildNoticeUpdate( CNetMsg::SP& msg, CDescriptor* dest )
{
	static char t_title[GUILD_NOTICE_TITLE_MAX * 2 + 1];
	static char t_text[GUILD_NOTICE_TEXT_MAX * 2 + 1];

	int guildindex;
	int charindex;
	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );

	RefMsg(msg) >> guildindex
		>> charindex
		>> title
		>> text;

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	mysql_real_escape_string(&gserver.m_dbchar, t_title, title, title.Length());
	mysql_real_escape_string(&gserver.m_dbchar, t_text, text, text.Length());
	char replace_guild_notice_query[8192] = {0,};
	sprintf( replace_guild_notice_query, "REPLACE INTO t_guild_notice ( a_guild_index, a_title, a_text, a_date ) VALUES ( %d, '%s', '%s', NOW() )", guildindex, t_title, t_text );

	cmd.SetQuery( replace_guild_notice_query );

	if( !cmd.Update() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildNoticeUpdateRepMsg( rmsg, charindex, guildindex, MSG_NEW_GUILD_NOTICE_UPDATE_ERROR_FAIL );
		SEND_Q( rmsg, dest );
		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	HelperNewGuildNoticeUpdateRepMsg( rmsg, charindex, guildindex, MSG_NEW_GUILD_NOTICE_UPDATE_ERROR_OK );
	SEND_Q( rmsg, dest );
	return;
}
void do_CommandNewGuildNoticeTrans( CNetMsg::SP& msg, CDescriptor* dest )
{
	int guildindex;
	int charindex;

	RefMsg(msg) >> guildindex
		>> charindex;
//		>> title
//		>> text;
//

	CGuild* guild = gserver.m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildNoticeTransRep( rmsg, guildindex, charindex, MSS_NEW_GUILD_NOTICE_TRANSMISSION_ERROR_FAIL );
		SEND_Q(rmsg, dest);
		return ;
	}

	CGuildMember* boss = guild->findmember(charindex);
	if (!boss)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildNoticeTransRep( rmsg, guildindex, charindex, MSS_NEW_GUILD_NOTICE_TRANSMISSION_ERROR_FAIL );
		SEND_Q(rmsg, dest);
		return ;
	}

	char select_guild_notice_query[8192] = {0,};
	sprintf( select_guild_notice_query, "SELECT * FROM t_guild_notice WHERE a_guild_index = %d", guildindex );

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );
	cmd.SetQuery( select_guild_notice_query );

	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildNoticeTransRep( rmsg, guildindex, charindex, MSG_NEW_GUILD_NOTICE_ERROR_NOT_MESSAGE );
		SEND_Q( rmsg, dest );
		return;
	}
	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );

	cmd.GetRec( "a_title", title );
	cmd.GetRec( "a_text", text );

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildNoticeTransMsg( rmsg, guildindex, title, text );
		gserver.SendToAllGameServer(rmsg);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildNoticeTransRep( rmsg, guildindex, charindex, MSS_NEW_GUILD_NOTICE_TRANSMISSION_ERROR_OK );
		SEND_Q(rmsg, dest);
	}
}

void do_CommandNewGuildSkillList( CNetMsg::SP& msg, CDescriptor* dest )
{
	int guildindex;
	int charindex;

	RefMsg(msg) >> guildindex
		>> charindex;

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	CLCString skillIndex(256);
	CLCString skillLevel(256);
	CLCString skillIndex1(256);
	CLCString skillLevel1(256);
	CLCString skillIndex2(256);
	CLCString skillLevel2(256);
	const char* pIndex = skillIndex;
	const char* pLevel = skillLevel;
	const char* pIndex1 = skillIndex1;
	const char* pLevel1 = skillLevel1;
	const char* pIndex2 = skillIndex2;
	const char* pLevel2 = skillLevel2;
	int sActiveindex[256];
	int sActiveLevel[256];
	int sPassiveindex[256];
	int sPassiveLevel[256];
	int sEtcindex[256];
	int sEtcLevel[256];
	memset( sActiveindex, 0, sizeof( sActiveindex ) );
	memset( sActiveLevel, 0, sizeof( sActiveLevel ) );
	memset( sPassiveindex, 0, sizeof( sPassiveindex ) );
	memset( sActiveLevel, 0, sizeof( sPassiveLevel ) );
	memset( sEtcindex, 0, sizeof( sEtcindex ) );
	memset( sEtcLevel, 0, sizeof( sEtcLevel ) );
	char tbuf[256];
	int ActiveLoopCount = 0;
	int PassiveLoopCount = 0;
	int EtcLoopCount = 0;

	char select_extend_guild_query[8192] = {0,};
	sprintf(select_extend_guild_query, "SELECT a_activeskill_index, a_activeskill_level, a_passiveskill_index, a_passiveskill_level, a_etcskill_index, a_etcskill_level FROM t_extend_guild WHERE a_guild_index = %d", guildindex);
	cmd.SetQuery(select_extend_guild_query);
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildSkillListRepMsg(rmsg, charindex, guildindex, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST, 0, NULL, NULL, 0, NULL, NULL, 0, NULL, NULL);
		SEND_Q( rmsg, dest );
		return;
	}

	if(!cmd.GetRec("a_activeskill_index", skillIndex))
	{
		skillIndex = '\0';
	}

	if(!cmd.GetRec("a_activeskill_level", skillLevel))
	{
		skillLevel = '\0';
	}
	while (*pIndex && *pLevel)
	{
		int i, l;
		pIndex = AnyOneArg(pIndex, tbuf);
		i = atoi(tbuf);
		pLevel = AnyOneArg(pLevel, tbuf);
		l = atoi(tbuf);

		sActiveindex[ActiveLoopCount] = i;
		sActiveLevel[ActiveLoopCount] = l;
		ActiveLoopCount++;
	}
	if(!cmd.GetRec("a_passiveskill_index", skillIndex1))
	{
		skillIndex1 = '\0';
	}

	if(!cmd.GetRec("a_passiveskill_level", skillLevel1))
	{
		skillLevel1 = '\0';
	}

	while (*pIndex1 && *pLevel1)
	{
		int i, l;
		pIndex1 = AnyOneArg(pIndex1, tbuf);
		i = atoi(tbuf);
		pLevel1 = AnyOneArg(pLevel1, tbuf);
		l = atoi(tbuf);

		sPassiveindex[PassiveLoopCount] = i;
		sPassiveLevel[PassiveLoopCount] = l;
		PassiveLoopCount++;
	}
	if(!cmd.GetRec("a_etcskill_index", skillIndex2))
	{
		skillIndex2 = '\0';
	}

	if(!cmd.GetRec("a_etcskill_level", skillLevel2))
	{
		skillLevel2 = '\0';
	}
	while (*pIndex2 && *pLevel2)
	{
		int i, l;
		pIndex2 = AnyOneArg(pIndex2, tbuf);
		i = atoi(tbuf);
		pLevel2 = AnyOneArg(pLevel2, tbuf);
		l = atoi(tbuf);

		sEtcindex[EtcLoopCount] = i;
		sEtcLevel[EtcLoopCount] = l;
		EtcLoopCount++;
	}

	{
		// 여기서 메세지 구성 및 발송
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildSkillListRepMsg(rmsg, charindex, guildindex, MSG_NEW_GUILD_SKILL_ERROR_OK,
			ActiveLoopCount, sActiveindex, sActiveLevel,
			PassiveLoopCount, sPassiveindex, sPassiveLevel,
			EtcLoopCount, sEtcindex, sEtcLevel);
		SEND_Q( rmsg, dest );
	}

	return;
}

void do_CommandSaveGuildPoint(CNetMsg::SP& msg, CDescriptor* dest)
{
	int nGuildIndex;
	int nGuildPoint;

	RefMsg(msg) >> nGuildIndex
		>> nGuildPoint;

	CGuild* pGuild = gserver.m_guildlist.findguild( nGuildIndex );

	if( pGuild )
	{
		GAMELOG << init( "GUILD POINT SAVE" )
				<< "GUILD" << delim
				<< nGuildIndex << delim
				<< pGuild->name() << delim
				<< "POINT" << delim
				<< nGuildPoint
				<< end;

		CDBCmd cmd;
		cmd.Init( &gserver.m_dbchar );
		CLCString sql(256);
		sql.Format( "UPDATE t_extend_guild SET a_guild_point = %d WHERE a_guild_index = %d", nGuildPoint, nGuildIndex );
		cmd.SetQuery( sql );
		cmd.Update();
	}
}

void do_CommandSaveGuildMemberPoint(CNetMsg::SP& msg, CDescriptor* dest)
{
	int nGuildIndex;
	int nGuildMemberPoint;
	int nCharIndex;

	RefMsg(msg) >> nGuildIndex
		>> nCharIndex
		>> nGuildMemberPoint;

	CGuild* pGuild = gserver.m_guildlist.findguild( nGuildIndex );

	if( pGuild )
	{
//		GAMELOG << init( "GUILD MEMBER POINT SAVE" )
//				<< "GUILD" << delim
//				<< nGuildIndex << delim
//				<< pGuild->name() << delim
//				<< "CHAR" << delim
//				<< nCharIndex << delim
//				<< "POINT" << delim
//				<< nGuildMemberPoint
//				<< end;

		CDBCmd cmd;
		cmd.Init( &gserver.m_dbchar );
		CLCString sql(256);

		sql.Format( "UPDATE t_extend_guildmember SET a_point = a_point + %d WHERE a_guild_index = %d AND a_char_index = %d", nGuildMemberPoint, nGuildIndex, nCharIndex );
		cmd.SetQuery( sql );
		if (!cmd.Update())
		{
			GAMELOG << init( "GUILD MEMBER POINT SAVE" )
			<< "GUILD" << delim
			<< nGuildIndex << delim
			<< pGuild->name()
			<< end;
		}

		CGuildMember* pMember = gserver.m_guildlist.findmember(nCharIndex);
		if (!pMember) return;

		pMember->AddCumulatePoint(nGuildMemberPoint);

		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildMemberPointSaveMsg(rmsg, nCharIndex, nGuildIndex, pMember->GetcumulatePoint());
		SEND_Q(rmsg, dest);
		gserver.SendToAllGameServer(rmsg);
	}
}

void do_CommandNewGuildUpdatePoint( CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex;
	int charindex;
	int point;

	RefMsg(msg) >> charindex
		>> guildindex
		>> point;

	CGuild* pGuild = gserver.m_guildlist.findguild( guildindex );
	if( pGuild )
	{
		CDBCmd cmd;
		cmd.Init( &gserver.m_dbchar );
		CLCString sql( 1024);
		sql.Format( "UPDATE t_extend_guild SET a_guild_point = a_guild_point + %d WHERE a_guild_index = %d ", point, guildindex );
		cmd.SetQuery( sql );
		if( !cmd.Update() )
		{
			GAMELOG << init( "GUILD POINT UPDATE" )
			<< "GUILD" << delim
			<< guildindex << delim
			<< pGuild->name()
			<< end;
		}
		pGuild->AddGuildPoint( point );

		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildPointUpdateMsg( rmsg, charindex, guildindex, pGuild->GetGuildPoint() );
		SEND_Q( rmsg, dest );
		gserver.SendToAllGameServer(rmsg);
	}
}

void do_CommandGuildNotice( CNetMsg::SP& msg, CDescriptor* dest )
{
	int charindex;
	RefMsg(msg) >> charindex;

	CGuildMember* member = gserver.m_guildlist.findmember(charindex);
	if (member)
	{
		CGuild* guild = member->guild();

		if (!guild)
			return;
		CDBCmd cmd;
		cmd.Init( &gserver.m_dbchar );

		// SELECT * FROM t_guild_notice WHERE a_guild_index = %d

		char select_guild_notice_query[8192] = {0,};
		sprintf( select_guild_notice_query, "SELECT * FROM t_guild_notice WHERE a_guild_index = %d", guild->index() );

		cmd.SetQuery( select_guild_notice_query );

		if( !cmd.Open() || !cmd.MoveFirst() )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperNewGuildNotice( rmsg, charindex, guild->index(), NULL, NULL );
			SEND_Q( rmsg, dest );
			return;
		}
		CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
		CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );

		cmd.GetRec( "a_title", title );
		cmd.GetRec( "a_text", text );

		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildNotice( rmsg, charindex, guild->index(), title, text );
		SEND_Q( rmsg, dest );
	}
}

void do_CommandGuildSkillLearn( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nGuildIndex;
	int nGuildPoint;
	int nSkillType;
	CLCString  guildSkillIndex(1024);
	CLCString  guildSkillLevel(1024);

	const char* pskillIndex = guildSkillIndex;
	const char* pskillLevel = guildSkillLevel;
	char tbuf[256];
	int sindex[256];
	int sLevel[256];
	RefMsg(msg) >> nGuildIndex
		>> nGuildPoint
		>> nSkillType
		>> guildSkillIndex
		>> guildSkillLevel;

	CGuild* pGuild = gserver.m_guildlist.findguild( nGuildIndex );

	if( pGuild )
	{
		CDBCmd cmd;
		cmd.Init( &gserver.m_dbchar );
		CLCString sql( 1024);
		if(nSkillType == 0) // active
		{
			sql.Format( "UPDATE t_extend_guild SET a_activeskill_index ='%s', a_activeskill_level='%s', a_guild_point = a_guild_point +( %d ) WHERE a_guild_index = %d ", (const char*)guildSkillIndex, (const char*)guildSkillLevel, nGuildPoint, nGuildIndex  );
		}
		else if (nSkillType == 1) // passive
		{
			//sql.Format( "UPDATE t_extend_guild SET a_skill_index ='%s', a_skill_level='%s', a_guild_point = a_guild_point +( %d ) WHERE a_guild_index = %d ", (const char*)guildSkillIndex, (const char*)guildSkillLevel, nGuildPoint, nGuildIndex  );
			sql.Format( "UPDATE t_extend_guild SET a_passiveskill_index ='%s', a_passiveskill_level='%s', a_guild_point = a_guild_point +( %d ) WHERE a_guild_index = %d ", (const char*)guildSkillIndex, (const char*)guildSkillLevel, nGuildPoint, nGuildIndex  );
		}
		else if(nSkillType == 2) // etc
		{
			sql.Format( "UPDATE t_extend_guild SET a_etcskill_index ='%s', a_etcskill_level='%s', a_guild_point = a_guild_point +( %d ) WHERE a_guild_index = %d ", (const char*)guildSkillIndex, (const char*)guildSkillLevel, nGuildPoint, nGuildIndex  );
		}
		else
			return ;

		cmd.SetQuery( sql );
		if( !cmd.Update() )
		{
			GAMELOG << init( "GUILD SKILL SAVE" )
					<< "GUILD" << delim
					<< nGuildIndex << delim
					<< pGuild->name()
					<< end;
		}

		pGuild->AddGuildPoint( nGuildPoint );

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperNewGuildPointUpdateMsg( rmsg, pGuild->boss()->charindex(), nGuildIndex, pGuild->GetGuildPoint() );
			SEND_Q( rmsg, dest );
			gserver.SendToAllGameServer(rmsg);
		}

		int roopcount = 0;
		while( *pskillIndex && pskillLevel )
		{
			int i, l;
			pskillIndex = AnyOneArg( pskillIndex, tbuf );
			i = atoi( tbuf );
			pskillLevel = AnyOneArg( pskillLevel, tbuf );
			l = atoi( tbuf );

			sindex[roopcount] = i;
			sLevel[roopcount] = l;
			if( sindex[roopcount] == 444 )
			{
			//	int maxmember = pGuild->maxmember() + ( 5 * sLevel[roopcount] );

				int maxmember;
				if(sLevel[roopcount] <= 5)
					maxmember = GUILD_LEVEL6_MAX_MEMBER + (5 * sLevel[roopcount] );
				else
					maxmember = 55 + 9 * (sLevel[roopcount] - 5);
				pGuild->setMaxmember( maxmember );

				sql.Format( "UPDATE t_extend_guild SET a_guild_maxmember = %d WHERE a_guild_index = %d", maxmember, nGuildIndex  );
				cmd.SetQuery( sql );
				if( !cmd.Update() )
				{
					GAMELOG << init( "GUILD MEMBER UPDATE FAIL" )
							<< "GUILD" << delim
							<< nGuildIndex << end;
				}
			}
			roopcount++;
		}

		CNetMsg::SP rmsg(new CNetMsg);
		HelperExtendGuildLoadNotifyMsg( rmsg, pGuild, roopcount, sindex, sLevel, nSkillType);
		gserver.SendToAllGameServer( rmsg );
	}
}

void do_CommandGuildSkillLearnSendMember( CNetMsg::SP& msg )
{
	gserver.SendToAllGameServer( msg );
};
void do_InsertGuard(CNetMsg::SP& msg, CDescriptor * dest)
{
	int zoneindex, itemindex;
	float x, z, h;
	RefMsg(msg) >> zoneindex >> x >> z >> h >> itemindex;

	int mobindex;

	switch(itemindex)
	{
	case 2380:
		mobindex = 405;
		break;
	case 2381:
		mobindex = 406;
		break;

	default:
		mobindex = 0;
		break;
	}

	if(mobindex == 0)
	{
		return;
	}

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );
	CLCString sql( 1024);
	sql.Format( "INSERT INTO t_castle_guard VALUES(%d, %d, %f, %f, %f)", zoneindex, mobindex, x, z, h);
	cmd.SetQuery( sql );
	if( !cmd.Update() )
	{
		GAMELOG << init( "CASTLE GUARD INSERT FAILED" )
			<< zoneindex << delim
			<< mobindex << delim
			<< x << delim
			<< z << delim
			<< h << delim
			<< end;
	}
}

void do_CastleTowerAdd(CNetMsg::SP& msg, CDescriptor * dest)
{
	char status[7];
	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );
	CLCString sql(1024);
	int zoneidx = 0;

	RefMsg(msg) >> zoneidx;
	sql.Format( "DELETE FROM t_castle_tower WHERE a_zone_num=%d", zoneidx);
	cmd.SetQuery( sql );
	cmd.Update();

	for(int i=0; i<7;i++)
	{
		RefMsg(msg) >> status[i];
		sql.Format( "INSERT INTO t_castle_tower VALUES(%d, %d, %d)", zoneidx, i, (int)status[i]);
		cmd.SetQuery( sql );
		if( !cmd.Update() )
		{
			GAMELOG << init( "CASTLE TOWER ADD" )
				<< zoneidx << delim
				<< i << delim
				<< status[i] << delim
				<< end;
		}
	}
}

void do_CastleTowerReinforce(CNetMsg::SP& msg, CDescriptor * dest)
{
	int zoneidx = 0;
	char type = 0, step = 0;
	RefMsg(msg) >> zoneidx >> type >> step;

	// type : 0. 공격형 타워 1. 가드형 타워 2. 성문
	// step 1 - 3
	if (type <=0 || type > 3 || step <= 0 || step > 3)
	{
		return;
	}

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );
	CLCString sql(1024);

	sql.Format( "DELETE FROM t_castle_reinforce WHERE a_zone_num=%d and a_type=%d", zoneidx, type);
	cmd.SetQuery( sql );
	cmd.Update();

	sql.Format( "INSERT INTO t_castle_reinforce VALUES(%d, %d, %d)", zoneidx, type, step);
	cmd.SetQuery( sql );
	cmd.Update();
}

void do_CastleQuartersInstall(CNetMsg::SP& msg, CDescriptor * dest)
{
	int qindex = 0, gindex = 0;
	CLCString gname(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> qindex >> gindex >> gname;

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );
	CLCString sql(1024);

	sql.Format( "DELETE FROM t_castle_rebrith WHERE a_zone_num=%d and a_npc_idx=%d", ZONE_DRATAN, qindex);
	cmd.SetQuery( sql );
	cmd.Update();

	sql.Format( "INSERT INTO t_castle_rebrith VALUES(%d, %d, %d, '%s')", ZONE_DRATAN, qindex, gindex, (const char *) gname);
	cmd.SetQuery( sql );
	cmd.Update();
}

void do_CommandPetChangeName( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nCharIndex;
	int	nPetIndex;
	CLCString strPetName( 30 );

	RefMsg(msg) >> nCharIndex
		>> nPetIndex
		>> strPetName;

	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	sql.Format( "SELECT * FROM t_pet WHERE a_index = %d AND a_owner = %d ", nPetIndex, nCharIndex );
	cmd.SetQuery( sql );
	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPetNameChange(rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_NOPET, nCharIndex, nPetIndex, strPetName );
		SEND_Q(rmsg, dest);
		return;
	}

	sql.Format( "SELECT * FROM t_pet_name WHERE a_pet_name = '%s' ", (const char*)strPetName );
	cmd.SetQuery( sql );
	cmd.Open();
	if( cmd.m_nrecords > 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_DUPLICATE, nCharIndex, nPetIndex, strPetName );
		SEND_Q( rmsg, dest );
		return;
	}

	sql.Format( "REPLACE  INTO t_pet_name ( a_pet_index, a_pet_owner, a_pet_name ) VALUES ( %d, %d, '%s' )", nPetIndex, nCharIndex, (const char* )strPetName );
	cmd.SetQuery( sql );
	if( !cmd.Update() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_FAIL, nCharIndex, nPetIndex, strPetName );
		SEND_Q( rmsg, dest );
		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	HelperPetNameChange( rmsg, MSG_EX_PET_CHANGE_NAME_ERROR_OK, nCharIndex, nPetIndex, strPetName );
	SEND_Q( rmsg, dest );
	return;
}

void do_CommandHalloween2007( CNetMsg::SP& msg, CDescriptor* dest )
{
	int charIndex;

	RefMsg(msg) >> charIndex;

	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
#ifdef LC_GAMIGO
	char select_event_halloween_2007_query[8192] = {0,};
	sprintf(select_event_halloween_2007_query, "SELECT a_char_index FROM t_event_halloween2007 WHERE a_char_index = %d", charIndex);
	cmd.SetQuery(select_event_halloween_2007_query);
	if (cmd.Open() && cmd.m_nrecords > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperHalloween2007Msg( rmsg, charIndex , 1 );
		SEND_Q(rmsg, dest);
		return ;
	}
#endif
	sql.Format("INSERT INTO t_event_halloween2007 ( a_char_index , a_update_date ) VALUES (%d, NOW())", charIndex );

	cmd.SetQuery(sql);

	CNetMsg::SP rmsg(new CNetMsg);
	if(cmd.Update())
	{
		HelperHalloween2007Msg( rmsg, charIndex , 0 );
	}
	else
	{
		HelperHalloween2007Msg( rmsg, charIndex , 1 );
	}

	SEND_Q(rmsg, dest);
}

void do_CommandDVDRateChange( CNetMsg::SP& msg, CDescriptor* dest )
{
	// 모든 서버에 변경 subtype과 nRate 전달
	CDBCmd	cmd;
	unsigned char subtype;
	int		nRate, mode;
	RefMsg(msg) >> subtype >> nRate >> mode;

	// DB 에 저장
	int	nZoneIndex=0;
	CLCString sql(1024), strRateName(1024);

	switch( subtype )
	{
	case MSG_MANAGEMENT_ENV_CHANGE :	strRateName.Format("a_env_rate");			break;
	case MSG_MANAGEMENT_STATE_CHANGE:	strRateName.Format("a_mop_rate");			break;
	case MSG_MANAGEMENT_TAX_CHANGE :	strRateName.Format("a_tax_rate");			break;
	case MSG_MANAGEMENT_HUNTER_TAX_CHANGE :		strRateName.Format("a_hunt_rate");	break;
	}

	sql.Format("SELECT * FROM t_castle_dungeon WHERE a_zone_index=%d" , ZONE_DRATAN_CASTLE_DUNGEON );
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if( cmd.Open() && cmd.MoveFirst() )
	{
		cmd.GetRec("a_zone_index", nZoneIndex );
		cmd.GetRec("a_env_rate", gserver.m_castle_env_rate);
		cmd.GetRec("a_mop_rate", gserver.m_castle_mob_rate);
		cmd.GetRec("a_tax_rate", gserver.m_castle_tax_rate);
		cmd.GetRec("a_hunt_rate", gserver.m_castle_hunt_rate);
	}

	if(  nZoneIndex == ZONE_DRATAN_CASTLE_DUNGEON )
	{
		sql.Format("UPDATE t_castle_dungeon SET %s = %d, a_mode = %d WHERE a_zone_index=%d" , (const char*)strRateName, nRate, mode, ZONE_DRATAN_CASTLE_DUNGEON );
	}
	else
	{
		sql.Format("INSERT INTO t_castle_dungeon (  a_zone_index , %s, a_mode )  VALUES ( %d, %d, %d  ) " , (const char*)strRateName , ZONE_DRATAN_CASTLE_DUNGEON , nRate, mode) ;
	}
	cmd.SetQuery(sql);
	cmd.Update();

	CNetMsg::SP rmsg(new CNetMsg);
	HelperDVDRateChangeMsg( rmsg, subtype, nRate );
	gserver.SendToAllGameServer(rmsg);
}

void do_CommandDVDNormalChangeNotice( CNetMsg::SP& msg, CDescriptor* dest )
{
	gserver.SendToAllGameServer(msg);
}

void do_CommandDVDNormalChangeTime( CNetMsg::SP& msg, CDescriptor* dest )
{
	int		time;
	RefMsg(msg) >> time;
	
	CDBCmd	cmd;
	std::string query = boost::str(boost::format("UPDATE t_castle_dungeon SET a_change_normal_time = %d where a_zone_index = %d") % time % ZONE_DRATAN_CASTLE_DUNGEON);
	cmd.SetQuery(query);
	cmd.Update();
}

void do_CommandPetTurnToNpc( CNetMsg::SP& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int nCharIndex;
	int	nPetIndex;
	int nToNpc;
	int size;

	RefMsg(msg) >> nCharIndex
		>> nPetIndex
		>> nToNpc
		>> size;

	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
#ifdef PET_TURNTO_NPC_ITEM
	sql.Format( "UPDATE t_pet SET a_pet_turnto_npc=%d, a_pet_size=%d WHERE a_index = %d AND a_owner = %d", nToNpc, size, nPetIndex, nCharIndex );
#else
	sql.Format( "UPDATE t_pet SET a_pet_turnto_npc = %d WHERE a_index = %d AND a_owner = %d", nToNpc, nPetIndex, nCharIndex );
#endif // PET_TURNTO_NPC_ITEM
	cmd.SetQuery( sql );
	if( !cmd.Update() )
	{
		GAMELOG << init("FAILED SAVE PET CHANGE NPC")
				<< "CharIndex" << delim << nCharIndex << delim
				<< "PetIndex" << delim << nPetIndex << delim
				<< "NpcIndex" << delim << nToNpc << end;
	}
}

void do_CommandCreateCombo( CNetMsg::SP& msg, CDescriptor* dest )
{
	int subserver, charindex, areaindex, comboindex;
	CLCString sql(200);
	CDBCmd cmd;

	RefMsg(msg) >> subserver
		>> charindex
		>> areaindex
		>> comboindex;
	sql.Format("INSERT INTO t_create_combo (`a_date`, `a_server`, `a_charindex`, `a_areaindex`, `a_comboindex`) VALUES ( NOW() , %d, %d, %d, %d )",
		subserver, charindex, areaindex, comboindex);

	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	cmd.Update();
}

void do_CommandAttackPet( CNetMsg::SP& msg, CDescriptor* dest )
{
	char	subtype;
	int		ownerIndex;
	RefMsg(msg) >> subtype
		>> ownerIndex;

	CLCString sql( 1024 );
	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	switch( subtype )
	{
	case MSG_HELPER_APET_CREATE_REQ :
		{
			int protoIndex;

			RefMsg(msg) >> protoIndex;

			sql.Format( "INSERT INTO t_apets ( a_owner, a_enable, a_proto_index, a_create_date, a_lastupdate_date, a_level )"
				"VALUES ( %d, 3, %d, NOW(), NOW(), 1 )", ownerIndex, protoIndex );
			cmd.SetQuery( sql );
			if( cmd.Update() )
			{
				int petDbIndex = mysql_insert_id(&gserver.m_dbchar);

				CNetMsg::SP rmsg(new CNetMsg);
				HelperAttackPet( rmsg, MSG_HELPER_APET_CREATE_OK, ownerIndex );
				RefMsg(rmsg) << petDbIndex << protoIndex;
				SEND_Q( rmsg, dest);
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperAttackPet( rmsg, MSG_HELPER_APET_CREATE_FAILED, ownerIndex );
				RefMsg(rmsg) << protoIndex;
				SEND_Q( rmsg, dest);
			}
		}
		break;

	case MSG_HELPER_APET_ACCENPT_REQ :
		{
			int petIndex;
			RefMsg(msg) >> petIndex;

			sql.Format( "UPDATE t_apets SET a_enable=1 WHERE a_enable=3 AND a_index=%d AND a_owner=%d", petIndex, ownerIndex );
			cmd.SetQuery( sql );
			if( cmd.Update() )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperAttackPet( rmsg, MSG_HELPER_APET_ACCENPT_OK, ownerIndex );
				SEND_Q( rmsg, dest);
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperAttackPet( rmsg, MSG_HELPER_APET_ACCENPT_FAILED, ownerIndex );
				SEND_Q( rmsg, dest);
			}
		}break;
	case MSG_HELPER_APET_DELETE_REQ :
		{
			int		petIndex;
			char	enable;
			RefMsg(msg) >> petIndex
				>> enable;

			sql.Format( "DELETE FROM t_apets WHERE a_enable=%d AND a_index=%d AND a_owner=%d", enable, petIndex, ownerIndex );
			cmd.SetQuery( sql );
			if( cmd.Update() )
			{
			}
		}break;
	case MSG_HELPER_APET_DISABLE_REQ:
		{
			int		petIndex;
			RefMsg(msg) >> petIndex;
			sql.Format( "UPDATE t_apets SET a_enable=0  WHERE a_index=%d AND a_owner=%d", petIndex, ownerIndex );
			cmd.SetQuery( sql );
			if( cmd.Update() )
			{
				sql.Format( "DELETE FROM t_apets_inven WHERE a_apet_idx = %d ", petIndex );
				cmd.SetQuery( sql );
				cmd.Update();
			}
		}
	}
}

#ifdef EXTREME_CUBE
#ifdef EXTREME_CUBE_VER2
void do_CommandCubeStateReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int charindex;
	int selfpoint = 0;
	int guildidx, cubepoint;
	char rank;
	char guildname[MAX_GUILD_NAME_LENGTH + 1], guildboss[MAX_CHAR_NAME_LENGTH + 1];
	CGuild* guild;
	CGuildMember* guildmember;
	CLCString sql(1000);

	RefMsg(msg) >> charindex;

	// 길드큐브포인트
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	guildmember = gserver.m_guildlist.findmember(charindex);
	if(guildmember)
	{
		sql.Format("SELECT cp.* FROM t_cubepoint cp , t_guild g "
			"WHERE cp.a_week_date=%d and cp.a_guild_index=%d and cp.a_guild_index=g.a_index and g.a_enable=1",
			gserver.m_CubePointUpdateTime, guildmember->guild()->index());

		cmd.SetQuery(sql);
		if(cmd.Open() && cmd.MoveFirst())
		{
			cmd.GetRec("a_cubepoint", selfpoint);
		}
	}

	// `a_week_date`, `a_char_idx`, `a_cubepoint`, `a_update_date`
	sql.Format("SELECT cp.a_guild_index as a_guild_index, cp.a_cubepoint as a_cubepoint, cp.a_update_date, g.a_name as a_name FROM t_cubepoint cp, t_guild g "
		"WHERE cp.a_week_date=%d and cp.a_guild_index=g.a_index and g.a_enable=1 "
		"ORDER BY cp.a_cubepoint DESC, cp.a_update_date ASC LIMIT 5", gserver.m_CubePointUpdateTime);
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return ;

	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_HELPER_COMMAND);
	RefMsg(rmsg) << MSG_HELPER_CUBESTATE_REP
		 << charindex
		 << selfpoint
		 << (char)cmd.m_nrecords;

	rank = 0;
	CLCString strname(100);
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_guild_index", guildidx);
		cmd.GetRec("a_name", strname);
		cmd.GetRec("a_cubepoint", cubepoint);

		memset(guildname, 0, sizeof(guildname));
		memset(guildboss, 0, sizeof(guildboss));

		strcpy(guildname, (const char*)strname);
		guild = gserver.m_guildlist.findguild(guildidx);
		if(guild)
		{
			if(guild->boss())
				strcpy(guildboss, guild->boss()->GetName());
		}

		rank++;
		RefMsg(rmsg) << rank
			 << guildname
			 << guildboss
			 << cubepoint;
	}

	// a_insert_week, a_type, a_rank, a_typeidx, a_cubepoint 0
	sql.Format("SELECT c.a_rank as a_rank, c.a_typeidx as a_guild_index, c.a_cubepoint as a_cubepoint, g.a_name as a_name FROM t_cuberank c, t_guild g "
		"WHERE c.a_insert_week=%d AND c.a_type=0 AND g.a_index=c.a_typeidx ORDER BY c.a_rank ", gserver.m_CubePointUpdateTime);

	cmd.SetQuery(sql);
	if(!cmd.Open())
		return ;

	RefMsg(rmsg) << (char)cmd.m_nrecords;
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_guild_index", guildidx);
		cmd.GetRec("a_name", strname);
		cmd.GetRec("a_cubepoint", cubepoint);
		cmd.GetRec("a_name", strname);
		cmd.GetRec("a_rank", rank);

		memset(guildname, 0, sizeof(guildname));
		memset(guildboss, 0, sizeof(guildboss));

		strcpy(guildname, strname);
		guild = gserver.m_guildlist.findguild(guildidx);
		if(guild)
		{
			if(guild->boss())
				strcpy(guildboss, guild->boss()->GetName());
		}

		RefMsg(rmsg) << rank
			 << guildname
			 << guildboss
			 << cubepoint;
	}

	SEND_Q(rmsg, dest);
}

void do_CommandCubeStatePersonalReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int charindex;
	int selfpoint = 0;
	CLCString sql(1000);

	RefMsg(msg) >> charindex;

	// `a_week_date`, `a_char_idx`, `a_cubepoint`, `a_update_date`
	// 개인 큐브 점수 찾기
	sql.Format("SELECT cp.* FROM t_cubepoint_personal cp, t_characters c WHERE cp.a_week_date=%d and c.a_index=%d and cp.a_char_idx=c.a_index", gserver.m_CubePointUpdateTime, charindex);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return ;

	if(cmd.MoveFirst())
	{
		cmd.GetRec("a_cubepoint", selfpoint);
	}

	// 현재 개인 큐브 순위 1~3등까지 찾기
	sql.Format("SELECT cp.*, c.a_nick as a_nick FROM t_cubepoint_personal cp, t_characters c "
			   "WHERE cp.a_week_date=%d and cp.a_char_idx = c.a_index and c.a_enable = 1 ORDER BY cp.a_cubepoint DESC, cp.a_update_date ASC LIMIT 3", gserver.m_CubePointUpdateTime);
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return ;

	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_HELPER_COMMAND);
	RefMsg(rmsg) << MSG_HELPER_CUBESTATE_PERSONAL_REP
		 << charindex
		 << selfpoint
		 << (char)cmd.m_nrecords;

	int character, cubepoint;
	char guildname[MAX_GUILD_NAME_LENGTH + 1], charname[MAX_CHAR_NAME_LENGTH + 1];
	CLCString str(1000);
	CGuildMember* guildmember;
	char rank = 0;

	while(cmd.MoveNext())
	{
		cmd.GetRec("a_char_idx", character);
		cmd.GetRec("a_cubepoint", cubepoint);
		cmd.GetRec("a_nick", str);

		memset(guildname, 0, sizeof(guildname));
		memset(charname, 0, sizeof(charname));

		guildmember = gserver.m_guildlist.findmember(character);
		if(guildmember && guildmember->guild())
		{
			strcpy(guildname, guildmember->guild()->name());
		}

		strcpy(charname, (const char* )str);

		rank++;
		RefMsg(rmsg) << rank
			 << guildname
			 << charname
			 << cubepoint;
	}

	// 지난주 개인 큐브 순위 1~3등까지 찾기
	// a_insert_week, a_type, a_rank, a_typeidx, a_cubepoint
	rank = 0;

	sql.Format("SELECT cr.a_rank, cr.a_typeidx, cr.a_cubepoint, c.a_nick FROM t_cuberank cr, t_characters c "
		"WHERE cr.a_insert_week=%d AND cr.a_type=1 AND cr.a_typeidx=c.a_index ORDER BY cr.a_rank", gserver.m_CubePointUpdateTime);
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return ;

	RefMsg(rmsg) << (char)cmd.m_nrecords;

	while(cmd.MoveNext())
	{
		cmd.GetRec("a_rank", rank);
		cmd.GetRec("a_typeidx", character);
		cmd.GetRec("a_cubepoint", cubepoint);
		cmd.GetRec("a_nick", str);

		memset(guildname, 0, sizeof(guildname));
		memset(charname, 0, sizeof(charname));

		guildmember = gserver.m_guildlist.findmember(character);
		if(guildmember && guildmember->guild())
		{
			strcpy(guildname, guildmember->guild()->name());
		}

		strcpy(charname, (const char* )str);

		RefMsg(rmsg) << rank
			 << guildname
			 << charname
			 << cubepoint;
	}

	SEND_Q(rmsg, dest);
}

void do_CommandAddCubePoint( CNetMsg::SP& msg, CDescriptor* dest )
{
	CLCString sql(1000);
	int ofguildindex, ofcubepoint;
	RefMsg(msg) >> ofguildindex
		>> ofcubepoint;
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
#ifdef LC_GAMIGO
	char select_cubepoint_query[8192] = {0,};
	sprintf(select_cubepoint_query, "SELECT a_week_date, a_guild_index FROM t_cubepoint WHERE a_week_date = %d AND a_guild_index = %d", gserver.m_CubePointUpdateTime, ofguildindex);
	cmd.SetQuery(select_cubepoint_query);
	if (cmd.Open() && cmd.m_nrecords > 0)
	{
		sql.Format("UPDATE t_cubepoint SET a_cubepoint=a_cubepoint+%d, a_update_date=NOW() WHERE a_guild_index=%d and a_week_date=%d",
			ofcubepoint, ofguildindex, gserver.m_CubePointUpdateTime);
		cmd.SetQuery(sql);
		if(cmd.Update())
		{
			GAMELOG << init("Increase CubePoint")
				<< "Week" << delim << gserver.m_CubePointUpdateTime << delim
				<< "Guild Index" << delim << ofguildindex << delim
				<< "CubePoint" << delim << ofcubepoint << end;
		}
		else
		{
			GAMELOG << init("Failed Increase CubePoint")
				<< "Week" << delim << gserver.m_CubePointUpdateTime << delim
				<< "Guild Index" << delim << ofguildindex << delim
				<< "CubePoint" << delim << ofcubepoint << end;
		}
		return ;
	}
#endif
	sql.Format("INSERT INTO t_cubepoint (`a_week_date`, `a_guild_index`, `a_cubepoint`, `a_update_date` ) VALUES (%d, %d, %d, NOW())",
			gserver.m_CubePointUpdateTime, ofguildindex, ofcubepoint);
	cmd.SetQuery(sql);
	if(cmd.Update())
	{
		GAMELOG << init("Increase CubePoint")
				<< "Week" << delim << gserver.m_CubePointUpdateTime << delim
				<< "Guild Index" << delim << ofguildindex << delim
				<< "CubePoint" << delim << ofcubepoint << end;
	}
	else
	{
		sql.Format("UPDATE t_cubepoint SET a_cubepoint=a_cubepoint+%d, a_update_date=NOW() WHERE a_guild_index=%d and a_week_date=%d",
			ofcubepoint, ofguildindex, gserver.m_CubePointUpdateTime);
		cmd.SetQuery(sql);
		if(cmd.Update())
		{
			GAMELOG << init("Increase CubePoint")
					<< "Week" << delim << gserver.m_CubePointUpdateTime << delim
					<< "Guild Index" << delim << ofguildindex << delim
					<< "CubePoint" << delim << ofcubepoint << end;
		}
		else
		{
			GAMELOG << init("Failed Increase CubePoint")
					<< "Week" << delim << gserver.m_CubePointUpdateTime << delim
					<< "Guild Index" << delim << ofguildindex << delim
					<< "CubePoint" << delim << ofcubepoint << end;
		}
	}
}

void do_CommandAddCubePointPersonal( CNetMsg::SP& msg, CDescriptor* dest )
{
	CLCString sql(1000);
	int ofcharidx, ofcubepoint;
	RefMsg(msg) >> ofcharidx
		>> ofcubepoint;
#ifdef LC_GAMIGO
	char select_cubepoint_personal_query[1024] = {0,};
	sprintf(select_cubepoint_personal_query, "SELECT a_week_date, a_char_idx FROM t_cubepoint_personal WHERE a_week_date = %d AND a_char_idx = %d", gserver.m_CubePointUpdateTime, ofcharidx);
	CDBCmd checkcmd;
	checkcmd.Init(&gserver.m_dbchar);
	checkcmd.SetQuery(select_cubepoint_personal_query);
	if (checkcmd.Open() && checkcmd.m_nrecords > 0)
	{
		sql.Format("UPDATE t_cubepoint_personal SET a_cubepoint=a_cubepoint+%d, a_update_date=NOW() WHERE a_char_idx = %d AND a_week_date=%d",
			ofcubepoint, ofcharidx, gserver.m_CubePointUpdateTime);
		checkcmd.SetQuery(sql);
		if(checkcmd.Update())
		{
			GAMELOG << init("Increase CubePoint Personal")
				<< "Week" << delim << gserver.m_CubePointUpdateTime << delim
				<< "CharIdx" << delim << ofcharidx << delim
				<< "CubePoint" << delim	<< ofcubepoint << end;
		}
		else
		{
			// 실패하면 로그 찍기
			GAMELOG << init("Failed Increase CubePoint Personal")
				<< "Week" << delim << gserver.m_CubePointUpdateTime << delim
				<< "CharIdx" << delim << ofcharidx << delim
				<< "CubePoint" << delim	<< ofcubepoint << end;
		}
		return ;
	}
#endif
	sql.Format("INSERT INTO t_cubepoint_personal (`a_week_date`, `a_char_idx`, `a_cubepoint`, `a_update_date` ) VALUES (%d, %d, %d, NOW())",
			gserver.m_CubePointUpdateTime, ofcharidx, ofcubepoint);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(cmd.Update())
	{
		GAMELOG << init("Increase CubePoint Personal")
				<< "Week" << delim << gserver.m_CubePointUpdateTime << delim
				<< "CharIdx" << delim << ofcharidx << delim
				<< "CubePoint" << delim	<< ofcubepoint << end;
	}
	else
	{
		sql.Format("UPDATE t_cubepoint_personal SET a_cubepoint=a_cubepoint+%d, a_update_date=NOW() WHERE a_char_idx = %d AND a_week_date=%d",
			ofcubepoint, ofcharidx, gserver.m_CubePointUpdateTime);
		cmd.SetQuery(sql);
		if(cmd.Update())
		{
			GAMELOG << init("Increase CubePoint Personal")
					<< "Week" << delim << gserver.m_CubePointUpdateTime << delim
					<< "CharIdx" << delim << ofcharidx << delim
					<< "CubePoint" << delim	<< ofcubepoint << end;
		}
		else
		{
			// 실패하면 로그 찍기
			GAMELOG << init("Failed Increase CubePoint Personal")
					<< "Week" << delim << gserver.m_CubePointUpdateTime << delim
					<< "CharIdx" << delim << ofcharidx << delim
					<< "CubePoint" << delim	<< ofcubepoint << end;
		}
	}
}

void do_CommandCubeRewardPersonalReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int charidx;
	char rank, breward=0;

	RefMsg(msg) >> charidx;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	CLCString sql(1024);
	sql.Format("SELECT * FROM t_cuberank WHERE a_insert_week=%d AND a_type=1 AND a_typeidx=%d", gserver.m_CubePointUpdateTime, charidx);

	cmd.SetQuery(sql);
	if(!cmd.Open())
	{
		GAMELOG << init("EXTREME_CUBE_REWARD_REQ") << "t_cuberank OPEN FAIL" << delim << charidx << end;
		return ;
	}

	if(!cmd.MoveFirst())
	{
		// 보상받을 수 있는 캐릭터가 아니다.
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExtremeCubeErrorMsg(rmsg, MSG_HELPER_EXTREME_CUBE_ERROR_REWARD_PERSONAL_CANNOT, charidx);
		SEND_Q(rmsg, dest);
		return ;
	}

	cmd.GetRec("a_rank", rank);
	cmd.GetRec("a_reward", breward);
	if(breward == 1)
	{
		// 이미 보상을 받았음
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExtremeCubeErrorMsg(rmsg, MSG_HELPER_EXTREME_CUBE_ERROR_REWARD_PERSONAL_ALREADY, charidx);
		SEND_Q(rmsg, dest);
		return ;
	}

	sql.Format("UPDATE t_cuberank SET a_reward=1 WHERE a_insert_week=%d AND a_type=1 AND a_typeidx=%d", gserver.m_CubePointUpdateTime, charidx);
	cmd.SetQuery(sql);
	if(!cmd.Update())
	{
		GAMELOG << init("EXTREME_CUBE_REWARD_REQ") << "t_cuberank UPDATE FAIL" << charidx << end;
		return ;
	}

	GAMELOG << init("EXTREME_CUBE_PERSONAL_REWARD_OK") << "CharIndex" << delim << charidx << end;
	// 보상진행
	CNetMsg::SP rmsg(new CNetMsg);
	HelperCubeRewardPersonalRepMsg(rmsg, gserver.m_CubePointUpdateTime, charidx, rank);
	SEND_Q(rmsg, dest);
}

void do_CommandCubeRewardPersonalRollBack(CNetMsg::SP& msg, CDescriptor* dest )
{
	int charindex, updatetime;
	RefMsg(msg) >> updatetime
		>> charindex;

	CLCString sql(1024);
	sql.Format("UPDATE t_cuberank SET a_reward=0 WHERE a_insert_week=%d AND a_type=1 AND a_typeidx=%d", updatetime, charindex);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(cmd.Update())
	{
		// 롤백한 캐릭터 로그남김
		GAMELOG << init("EXTREME_CUBE_PERSONAL_REWARD_ROLLBACK") << "updatetime" << updatetime << "charindex" << charindex << end;
	}
	else
	{
		// 실패
		GAMELOG << init("EXTREME_CUBE_PERSONAL_REWARD_ROLLBACK_FAIL") << "updatetime" << updatetime << "charindex" << charindex << end;
	}
}
#else
void do_CommandCubeStateReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	CDBCmd cmd;
	int charindex;
	char nRemainSendCount;
	const char nMaxSendCount = 10;
	int selfpoint = 0;
	CLCString sql(1000);

	char SendCount;
	char TotalSendCount;
	int guildidx, cubepoint;
	char guildname[MAX_GUILD_NAME_LENGTH + 1], guildboss[MAX_CHAR_NAME_LENGTH + 1];
	CGuild* guild;
	CGuildMember* guildmember;
	char rank;

	RefMsg(msg) >> charindex;

	cmd.Init(&gserver.m_dbchar);

	guildmember = gserver.m_guildlist.findmember(charindex);
	if(guildmember)
	{
		sql.Format("SELECT cp.* FROM t_cubepoint cp , t_guild g WHERE cp.a_guild_index = %d and cp.a_guild_index = g.a_index and g.a_enable = 1", guildmember->guild()->index());
		cmd.SetQuery(sql);
		if(cmd.Open() && cmd.MoveFirst())
		{
			cmd.GetRec("a_cubepoint", selfpoint);
		}
	}
	cmd.SetQuery("SELECT cp.* FROM t_cubepoint cp, t_guild g WHERE cp.a_guild_index = g.a_index and g.a_enable = 1 ORDER BY cp.a_cubepoint DESC, cp.a_update_date ASC LIMIT 10");
	if(!cmd.Open())
	{
		GAMELOG << init("CUBESTATE OPEN FAILED") << end;
		return ;
	}

	if(!cmd.MoveFirst())
	{
		// count 0을 보낸다
		CNetMsg::SP rmsg(new CNetMsg);
		RefMsg(rmsg).Init(MSG_HELPER_COMMAND);
		RefMsg(rmsg) << MSG_HELPER_CUBESTATE_REP
			 << charindex
			 << selfpoint
			 << (char)0;

		SEND_Q(rmsg, dest);
		return;
	}

	rank = 0;
	nRemainSendCount = (char)cmd.m_nrecords;

	do
	{
		if(nMaxSendCount < nRemainSendCount)
			TotalSendCount = nMaxSendCount;

		TotalSendCount = nRemainSendCount;
		SendCount = 0;

		CNetMsg::SP rmsg(new CNetMsg);
		RefMsg(rmsg).Init(MSG_HELPER_COMMAND);
		RefMsg(rmsg) << MSG_HELPER_CUBESTATE_REP
			 << charindex
			 << selfpoint
			 << TotalSendCount;
		do
		{
			cmd.GetRec("a_guild_index", guildidx);
			cmd.GetRec("a_cubepoint", cubepoint);

			memset(guildname, 0, sizeof(guildname));
			memset(guildboss, 0, sizeof(guildboss));

			guild = gserver.m_guildlist.findguild(guildidx);
			if(guild)
			{
				strcpy(guildname, guild->name());
				if(guild->boss())
					strcpy(guildboss, guild->boss()->GetName());
			}

			rank++;
			RefMsg(rmsg) << rank
				 << guildname
				 << guildboss
				 << cubepoint;

			SendCount++;
		}while(cmd.MoveNext() && SendCount < TotalSendCount);

		if(TotalSendCount != SendCount)
			return ;

		SEND_Q(rmsg, dest);
		nRemainSendCount -= TotalSendCount;
	}while(nRemainSendCount > 0);
}

void do_CommandCubeStatePersonalReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	CDBCmd cmd;
	int charindex;
	char nRemainSendCount;
	const char nMaxSendCount = 10;
	int selfpoint = 0;
	CLCString sql(1000);

	RefMsg(msg) >> charindex;

	sql.Format("SELECT cp.* FROM t_cubepoint_personal cp, t_characters c WHERE c.a_index = %d and cp.a_char_idx = c.a_index", charindex);
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(!cmd.Open())
	{
		GAMELOG << init("CUBESTATE_PERSONAL OPEN FAILED") << "character index" << delim << charindex << end;
		return ;
	}

	if(cmd.MoveFirst())
	{
		cmd.GetRec("a_cubepoint", selfpoint);
	}

	sql.Format("SELECT cp.*, c.a_nick as a_nick FROM t_cubepoint_personal cp, t_characters c WHERE cp.a_char_idx = c.a_index and c.a_enable = 1 ORDER BY cp.a_cubepoint DESC, cp.a_update_date ASC LIMIT 10");
	cmd.SetQuery(sql);
	if(!cmd.Open())
	{
		GAMELOG << init("CUBESTATE_PERSONAL OPEN FAILED") << end;
		return ;
	}

	if(!cmd.MoveFirst())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		RefMsg(rmsg).Init(MSG_HELPER_COMMAND);
		RefMsg(rmsg) << MSG_HELPER_CUBESTATE_PERSONAL_REP
			 << charindex
			 << selfpoint
			 << (char)0;

		SEND_Q(rmsg, dest);
		return ;
	}

	nRemainSendCount = (char)cmd.m_nrecords;

	char SendCount;
	char TotalSendCount;
	int character, cubepoint;
	char guildname[MAX_GUILD_NAME_LENGTH + 1], charname[MAX_CHAR_NAME_LENGTH + 1];
	CLCString str(1000);
	CGuildMember* guildmember;
	char rank = 0;

	do
	{
		if(nMaxSendCount < nRemainSendCount)
			TotalSendCount = nMaxSendCount;

		TotalSendCount = nRemainSendCount;
		SendCount = 0;

		CNetMsg::SP rmsg(new CNetMsg);
		RefMsg(rmsg).Init(MSG_HELPER_COMMAND);
		RefMsg(rmsg) << MSG_HELPER_CUBESTATE_PERSONAL_REP
			 << charindex
			 << selfpoint
			 << TotalSendCount;

		do
		{
			cmd.GetRec("a_char_idx", character);
			cmd.GetRec("a_cubepoint", cubepoint);
			cmd.GetRec("a_nick", str);

			memset(guildname, 0, sizeof(guildname));
			memset(charname, 0, sizeof(charname));

			guildmember = gserver.m_guildlist.findmember(character);
			if(guildmember && guildmember->guild())
			{
				strcpy(guildname, guildmember->guild()->name());
			}

			strcpy(charname, (const char* )str);

			rank++;
			RefMsg(rmsg) << rank
				 << guildname
				 << charname
				 << cubepoint;

			SendCount++;
		}while(cmd.MoveNext() && SendCount < TotalSendCount);

		if(TotalSendCount != SendCount)
			return ;

		SEND_Q(rmsg, dest);
		nRemainSendCount -= TotalSendCount;
	}while(nRemainSendCount > 0);
}

void do_CommandAddCubePoint( CNetMsg::SP& msg, CDescriptor* dest )
{
	CLCString sql(1000);
	int ofguildindex, ofcubepoint;
	long time;
	RefMsg(msg) >> ofguildindex
		>> ofcubepoint
		>> time;

	sql.Format("INSERT INTO t_cubepoint ( `a_guild_index`, `a_cubepoint`, `a_update_date` ) VALUES (%d, %d, NOW())",
			ofguildindex, ofcubepoint);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(cmd.Update())
	{
		GAMELOG << init("Increase CubePoint")
				<< "Guild Index" << delim
				<< ofguildindex << delim
				<< "CubePoint" << delim
				<< ofcubepoint << end;
	}
	else
	{
		sql.Format("UPDATE t_cubepoint SET a_cubepoint = a_cubepoint + %d, a_update_date = NOW() WHERE a_guild_index = %d", ofcubepoint, ofguildindex);
		cmd.SetQuery(sql);
		if(cmd.Update())
		{
			GAMELOG << init("Increase CubePoint")
					<< "Guild Index" << delim
					<< ofguildindex << delim
					<< "CubePoint" << delim
					<< ofcubepoint << end;
		}
		else
		{
			// 실패하면 로그 찍기
			GAMELOG << init("Failed Increase CubePoint")
					<< "Guild Index" << delim
					<< ofguildindex << delim
					<< "CubePoint" << delim
					<< ofcubepoint << end;
		}
	}
}

void do_CommandAddCubePointPersonal( CNetMsg::SP& msg, CDescriptor* dest )
{
	CLCString sql(1000);
	int ofcharidx, ofcubepoint;
	long time;
	RefMsg(msg) >> ofcharidx
		>> ofcubepoint
		>> time;

	sql.Format("INSERT INTO t_cubepoint_personal ( `a_char_idx`, `a_cubepoint`, `a_update_date` ) VALUES (%d, %d, NOW())",
			ofcharidx, ofcubepoint);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(cmd.Update())
	{
		GAMELOG << init("Increase CubePoint Personal")
				<< "CharIdx" << delim
				<< ofcharidx << delim
				<< "CubePoint" << delim
				<< ofcubepoint << end;
	}
	else
	{
		sql.Format("UPDATE t_cubepoint_personal SET a_cubepoint = a_cubepoint + %d, a_update_date = NOW() WHERE a_char_idx = %d", ofcubepoint, ofcharidx);
		cmd.SetQuery(sql);
		if(cmd.Update())
		{
			GAMELOG << init("Increase CubePoint Personal")
					<< "CharIdx" << delim
					<< ofcharidx << delim
					<< "CubePoint" << delim
					<< ofcubepoint << end;
		}
		else
		{
			// 실패하면 로그 찍기
			GAMELOG << init("Failed Increase CubePoint Personal")
					<< "CharIdx" << delim
					<< ofcharidx << delim
					<< "CubePoint" << delim
					<< ofcubepoint << end;
		}
	}
}
#endif // EXTREME_CUBE_VER2
#endif // EXTREME_CUBE

void do_CommandEventPhoenix( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nCharIdx = 0, nUserIdx = 0;
	RefMsg(msg) >> nUserIdx
		>> nCharIdx;

	CDBCmd DB_cmd1;
	CDBCmd DB_cmd2;
	CDBCmd DB_cmd3;
	int query_count1 = 0;
	DB_cmd1.Init(&gserver.m_dbchar);
	DB_cmd2.Init(&gserver.m_dbchar);
	DB_cmd3.Init(&gserver.m_dbchar);

	// 100 레벨이상인 캐릭터가 있다는 것을 표시
	bool level_100_up = false;

	char select_characters_query[1024] = {0,};
	// 자신의 계정 내에 모든 케릭터의 인덱스를 가져온다.
	sprintf(select_characters_query, "SELECT a_index FROM t_characters WHERE a_user_index=%d ", nUserIdx );
	DB_cmd1.SetQuery(select_characters_query);
	if (!DB_cmd1.Open())
		return;

	query_count1 = DB_cmd1.m_nrecords;
	if (query_count1)
	{
		while (DB_cmd1.MoveNext())
		{
			int index;
			int level;

			DB_cmd1.GetRec("a_index", index);

			// 각 캐릭터의 레벨을 하나씩 가져온다.
			char select_characters_query_1[1024] = {0,};
			sprintf(select_characters_query_1, "SELECT a_level FROM t_characters WHERE a_index=%d ", index );

			DB_cmd2.SetQuery(select_characters_query_1);
			if (!DB_cmd2.Open())
				continue;

			DB_cmd2.MoveFirst();
			DB_cmd2.GetRec("a_level", level);

			// 피닉스 이벤트 승락, 레벨이 100 이상인 캐릭터가 하나라도 있으면
			if( level >= 100 )
			{
				level_100_up = true;
				break;
			}
		}
	}
	else
	{
		// 계정의 캐릭터를 불러오지 못해서 자격 미달임
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventPhoenixMsg(rmsg, MSG_HELPER_EVENT_PHOENIX_ERROR_REQUIREMENT, nCharIdx);
		SEND_Q(rmsg, dest);
		return;
	}

	// 100 레벨 캐릭터가 하나라도 있다면
	if ( level_100_up )
	{
		int phoenix;

		// 캐릭터가 피닉스 이벤트 이후 생성된 캐릭터 이라면 피닉스가 가능
		char select_characters_query_2[1024] = {0,};
		sprintf(select_characters_query_2, "SELECT a_phoenix FROM t_characters WHERE a_index=%d ", nCharIdx );
		DB_cmd3.SetQuery(select_characters_query_2);
		if (!DB_cmd3.Open())
		{
			// 데이터를 가져오지 못해서 자격 미달임
			CNetMsg::SP rmsg(new CNetMsg);
			HelperEventPhoenixMsg(rmsg, MSG_HELPER_EVENT_PHOENIX_ERROR_REQUIREMENT, nCharIdx);
			SEND_Q(rmsg, dest);
			return;
		}

		DB_cmd3.MoveFirst();
		DB_cmd3.GetRec("a_phoenix", phoenix);

		// 피닉스 이벤트 이후 생성된 캐릭터 이라면
		if ( phoenix == 0 )
		{
			// 자격이 있음
			CNetMsg::SP rmsg(new CNetMsg);
			HelperEventPhoenixMsg(rmsg, MSG_HELPER_EVENT_PHOENIX_OK, nCharIdx);
			SEND_Q(rmsg, dest);
			return;
		}
		else
		{
			// 기존의 캐릭터라서 피닉스 자격이 없음
			CNetMsg::SP rmsg(new CNetMsg);
			HelperEventPhoenixMsg(rmsg, MSG_HELPER_EVENT_PHOENIX_ERROR_REQUIREMENT, nCharIdx);
			SEND_Q(rmsg, dest);
			return;
		}
	}
	else
	{
		// 레벨이 100 이상인 캐릭터가 하나도 없어서 자격 미달임
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventPhoenixMsg(rmsg, MSG_HELPER_EVENT_PHOENIX_ERROR_REQUIREMENT, nCharIdx);
		SEND_Q(rmsg, dest);
		return;
	}
}

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
void do_Command_DisconnectHackCharacter( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nCharIdx = 0, nUserIdx = 0;
	int nHack_type=0;
	float delay;
	CLCString user_id(20 + 1);
	CLCString char_id(20 + 1);
	RefMsg(msg) >> nUserIdx
		>> user_id
		>> nCharIdx
		>> char_id
		>> nHack_type
		>> delay;

	CLCString strType_hack(20 + 1);
	if ( nHack_type == HACK_TYPE_PULSE )
	{
		strType_hack.Format("Pulse hack");
	}
	else if( nHack_type == HACK_TYPE_CLIENT )
	{
		strType_hack.Format("client hack check");
	}
	else
		return;

	CDBCmd DB_cmd1;

	DB_cmd1.Init(&gserver.m_dbchar);

	char insert_hack_characters_query[1024] = {0,};
	sprintf(insert_hack_characters_query, "insert into t_hack_characters (a_user_idx, a_user_id, a_char_idx, a_char_id, a_time, a_hacktype, a_delay) values (%d, '%s', %d, '%s', NOW(), '%s', %f)",
		nUserIdx, (const char*)user_id, nCharIdx, (const char*)char_id, (const char*)strType_hack, delay);
	DB_cmd1.SetQuery(insert_hack_characters_query);
	if (!DB_cmd1.Update())
		return;
}
#endif // DISCONNECT_HACK_CHARACTER

//생성(파티 전환)
void do_Command_Exped_CreateReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nBossIndex;
	CLCString BossName(MAX_CHAR_NAME_LENGTH + 1);
	CPartyMember* pPartyMember = NULL;
	CExpedMember* pExpedMember = NULL;

	RefMsg(msg) >> nBossIndex
		>> BossName;

	CExpedition* pExped= gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if(pExped)
	{
		GAMELOG << init("CREATE EXPED DUP")
				<< nBossIndex << end;
		return;
	}

	LONGLONG key = MAKE_LONGLONG_KEY(dest->m_subNo, nBossIndex);
	map_party_t::iterator it = gserver.m_listParty.find(key);
	if(it != gserver.m_listParty.end())
	{
		CParty* pPartyBoss = it->second;

		if(pPartyBoss->GetBossIndex() != nBossIndex)
		{
			// 에러
			CNetMsg::SP rmsg(new CNetMsg);
			HelperExpedErrorMsg( rmsg, MSG_EXPED_ERROR_NORMAL, nBossIndex );
			SEND_Q( rmsg, dest );
			return;
		}

		if (pPartyBoss->GetMemberCount() > MAX_PARTY_MEMBER)
		{
			// 에러
			CNetMsg::SP rmsg(new CNetMsg);
			HelperExpedErrorMsg( rmsg, MSG_EXPED_ERROR_NORMAL, nBossIndex );
			SEND_Q( rmsg, dest );
			return;
		}

		// 신규 원정대 생성
		CExpedition* pExped = NULL;
		int bossidx = pPartyBoss->FindMember(pPartyBoss->GetBossIndex());
		pPartyMember = pPartyBoss->GetMemberByListIndex( bossidx );
		if( !pPartyMember )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperExpedErrorMsg( rmsg, MSG_EXPED_ERROR_NORMAL, nBossIndex );
			SEND_Q( rmsg, dest );
			return;
		}
		pExped = new CExpedition(dest->m_subNo, pPartyBoss->GetPartyType(MSG_DIVITYPE_EXP), pPartyBoss->GetPartyType(MSG_DIVITYPE_ITEM), pPartyBoss->GetPartyType(MSG_DIVITYPE_SPECIAL), pPartyBoss->GetPartyType(MSG_DIVITYPE_SP), pPartyBoss->GetPartyType(MSG_DIVITYPE_MONEY),  nBossIndex,(const char*)BossName,MSG_EXPED_GROUP_1,MSG_EXPED_MEMBERTYPE_BOSS, pPartyMember->m_nLevel );

		if(pExped)
		{
			int nJoinCnt=0;
			for(int i=0; i < MAX_PARTY_MEMBER ; i++)
			{
				pPartyMember = (CPartyMember*)pPartyBoss->GetMemberByListIndex(i);
				if(pPartyMember)
				{
					if(nBossIndex != pPartyMember->GetCharIndex())
					{
						//대원 추가(일반)
						pExpedMember = (CExpedMember*)pExped->Join(pPartyMember->GetCharIndex(),(const char*)pPartyMember->GetCharName(),MSG_EXPED_MEMBERTYPE_NORMAL, pPartyMember->m_nLevel );
						if(pExpedMember)
							nJoinCnt++;
					}
				}
			}

			if((pPartyBoss->GetMemberCount() - 1) == nJoinCnt )
			{
				int nRealCount = pExped->GetMemberCount() - 1;	//원정대장 제외

				int* pCharIndex		 = new int[nRealCount];			// 캐릭터 인덱스
				CLCString* pCharName = new CLCString[nRealCount];	// 캐릭터 이름
				int*  pGroupType	 = new int[nRealCount];			// 그룹 타입
				int*  pMemberType	 = new int[nRealCount];			// 멤버 타입
				int*  pSetLabelType	 = new int[nRealCount];			// 표식
				int*  pQuitType		 = new int[nRealCount];			// 정상, 비정상

				int nCnt=0;
				pExpedMember = NULL;
				for(int i=0; i < MAX_EXPED_GMEMBER ; i++)
				{
					pExpedMember = (CExpedMember*)pExped->GetMemberByListIndex(MSG_EXPED_GROUP_1,i,MSG_EXPED_MEMBERTYPE_NORMAL);
					if(pExpedMember)
					{
						pCharIndex[nCnt]	= pExpedMember->GetCharIndex();
						pCharName[nCnt]		= pExpedMember->GetCharName();
						pGroupType[nCnt]	= pExpedMember->GetGroupType();
						pMemberType[nCnt]	= pExpedMember->GetMemberType();
						pSetLabelType[nCnt]	= pExpedMember->GetLabelType();
						pQuitType[nCnt]		= pExpedMember->GetQuitType();
						nCnt++;

						if(nCnt >= nRealCount) break;
					}
					pExpedMember = NULL;
				}

				// 결과 전달
				CNetMsg::SP rmsg(new CNetMsg);
				HelperExpedCreateRepMsg(rmsg,pExped->GetExpedType(MSG_DIVITYPE_EXP),pExped->GetExpedType(MSG_DIVITYPE_ITEM),pExped->GetExpedType(MSG_DIVITYPE_SPECIAL),pExped->GetExpedType(MSG_DIVITYPE_SP),pExped->GetExpedType(MSG_DIVITYPE_MONEY),pExped->GetBossIndex(),pExped->GetBossName(),nRealCount,pCharIndex,pCharName,pGroupType,pMemberType,pSetLabelType,pQuitType);
				SEND_Q( rmsg, dest );

				// 성공	(리스트 추가)
				LONGLONG key = MAKE_LONGLONG_KEY(pExped->GetSubNo(), pExped->GetBossIndex());
				gserver.m_listExped.insert(map_expedition_t::value_type(key, pExped));

				delete[] pCharIndex;
				delete[] pCharName;
				delete[] pGroupType;
				delete[] pMemberType;
				delete[] pSetLabelType;
				delete[] pQuitType;
			}else
			{
				if(pExped)
				{
					delete pExped;
					pExped = NULL;
				}

				// 에러
			}
		}else
		{
			// 에러
		}
	}
}

//생성(파티 전환 결과)
void do_Command_Exped_CreateResult( CNetMsg::SP& msg, CDescriptor* dest )
{
	CNetMsg	rmsg;
	int nResult,nBossIndex;

	RefMsg(msg) >> nResult
		>> nBossIndex;

	if(nResult == 1)  //성공
	{
		LONGLONG key = MAKE_LONGLONG_KEY(dest->m_subNo, nBossIndex);
		map_party_t::iterator it = gserver.m_listParty.find(key);
		if(it != gserver.m_listParty.end())
		{
			CParty* pParty = it->second;

			gserver.m_listParty.erase(it);
			pParty->SetEndParty();
			delete pParty;

		// 제거 파티는 매칭 테이블에서 제거
			do_CommandPartyMatchDelReq_real(dest->m_subNo, nBossIndex);
		}
	}else  //실패
	{
		LONGLONG key = MAKE_LONGLONG_KEY(dest->m_subNo, nBossIndex);
		map_expedition_t::iterator it = gserver.m_listExped.find(key);
		if(it != gserver.m_listExped.end())
		{
			// 원정대 삭제
			CExpedition* pExped = it->second;
			gserver.m_listExped.erase(it);
			pExped->SetEndExped();
			delete pExped;
		}
	}
}

//초대 요청
void do_Command_Exped_InviteReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int			nBossIndex;
	CLCString	strBossName(MAX_CHAR_NAME_LENGTH + 1);
	int			nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> nBossIndex
		>> strBossName
		>> nTargetIndex
		>> strTargetName;

	CExpedition* pExpedBoss = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	CExpedition* pExpedTarget = gserver.FindExpedByBossIndex(dest->m_subNo, nTargetIndex);

	// nBossIndex가 원정대 초대가 가능한지 : 원정대가 없거나 보스
	if (pExpedBoss == NULL)
	{
		return ;
	}

	if (pExpedBoss && pExpedBoss->GetBossIndex() != nBossIndex)
	{
		return ;
	}

	// 대상 원정대 검사
	if (pExpedTarget)
	{
		return ;
	}

	// 원정대 인원 검사
	if (pExpedBoss && pExpedBoss->GetMemberCount() >= MAX_EXPED_MEMBER)
	{
		return ;
	}

	// 현재 요청자 유무 검사
	if (pExpedBoss && pExpedBoss->GetRequestIndex() > 0)
	{
		return ;
	}

	if (pExpedBoss)
	{
		// 기존 원정대에 요청자로 설정
		pExpedBoss->SetRequest(nTargetIndex, strTargetName);
	}

	// 결과 전달
	CNetMsg::SP rmsg(new CNetMsg);
	HelperExpedInviteRepMsg(rmsg, nBossIndex, strBossName, nTargetIndex, strTargetName,pExpedBoss->GetExpedType(MSG_DIVITYPE_EXP),pExpedBoss->GetExpedType(MSG_DIVITYPE_ITEM),pExpedBoss->GetExpedType(MSG_DIVITYPE_SPECIAL));
	SEND_Q( rmsg, dest );
}

//초대 수락
void do_Command_Exped_AllowReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int	nBossIndex;
	int	nTargetIndex;
	CLCString	strTargetName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex
		>> strTargetName;
	int	nLevel;
	RefMsg(msg) >> nLevel;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);

	int nNewGroup=-1;

	if (!pExped)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedAllowRepMsg(rmsg, nBossIndex, nTargetIndex, strTargetName, nNewGroup, MSG_EXPED_MEMBERTYPE_NORMAL,-1, MSG_EXPED_ERROR_ALLOW_NOT_EXPED);
		SEND_Q( rmsg, dest );
		return ;
	}

	if (pExped->GetRequestIndex() != nTargetIndex)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedAllowRepMsg(rmsg, nBossIndex, nTargetIndex, strTargetName, nNewGroup, MSG_EXPED_MEMBERTYPE_NORMAL,-1, MSG_EXPED_ERROR_ALLOW_NOT_EXPED);
		SEND_Q( rmsg, dest );
		return ;
	}

	//빈슬룻 찾기(그룹 결정)
	nNewGroup = pExped->GetGroupBeEmptySlot();

	if(nNewGroup >= 0)
	{
		CExpedMember* pMember=NULL;
		// 파티원으로 등록(추후 수정)
		pMember = (CExpedMember*)pExped->JoinRequest(strTargetName, MSG_EXPED_MEMBERTYPE_NORMAL, nLevel );

		if(pMember)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperExpedAllowRepMsg(rmsg, nBossIndex, nTargetIndex, strTargetName, nNewGroup, MSG_EXPED_MEMBERTYPE_NORMAL,pMember->GetListIndex(),MSG_EXPED_ERROR_ALLOW_OK);
			SEND_Q( rmsg, dest );
		}
	}else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedAllowRepMsg(rmsg, nBossIndex, nTargetIndex, strTargetName, nNewGroup, MSG_EXPED_MEMBERTYPE_NORMAL,-1, MSG_EXPED_ERROR_ALLOW_NOT_EXPED);
		SEND_Q( rmsg, dest );
		return ;
	}
}

//초대 거절
void do_Command_Exped_RejectReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int	nBossIndex;
	int	nTargetIndex;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedRejectRepMsg(rmsg, nBossIndex, nTargetIndex,MSG_EXPED_ERROR_REJECT_FAIL);
		SEND_Q( rmsg, dest );
		return ;
	}

	if (pExped->GetRequestIndex() < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedRejectRepMsg(rmsg, nBossIndex, nTargetIndex,MSG_EXPED_ERROR_REJECT_FAIL);
		SEND_Q( rmsg, dest );
		return ;
	}

	// 요청자가 보스인지 일반인지 검사
	if (pExped->GetBossIndex() == nTargetIndex || pExped->GetRequestIndex() == nTargetIndex)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedRejectRepMsg(rmsg, nBossIndex, nTargetIndex,MSG_EXPED_ERROR_REJECT_OK);
		SEND_Q( rmsg, dest );

		pExped->SetRequest(-1, "");
		if (pExped->GetMemberCountOnline() < 2)
		{
			// 원정대 삭제
			LONGLONG key = MAKE_LONGLONG_KEY(pExped->GetSubNo(), pExped->GetBossIndex());
			gserver.m_listExped.erase(key);
			pExped->SetEndExped();
			delete pExped;
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedRejectRepMsg(rmsg, nBossIndex, nTargetIndex,MSG_EXPED_ERROR_REJECT_FAIL);
		SEND_Q( rmsg, dest );
		return ;
	}
}

//탈퇴
void do_Command_Exped_QuitReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int	nBossIndex,	nTargetIndex;
	int nQuitMode;						// 정상,비정성 구분

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex					// 나가는 캐릭터인덱스
	    >> nQuitMode;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;

	if(nQuitMode == MSG_EXPED_QUITMODE_NORMAL)
	{
		pExped->DeleteMember(nTargetIndex);
	}
	if (pExped->GetMemberCountOnline() < 2)
	{
		// 원정대 삭제
		int key_index;
		if (nBossIndex == nTargetIndex)
		{
			key_index = nBossIndex;
		}
		else
		{
			key_index = pExped->GetBossIndex();
		}
		LONGLONG key = MAKE_LONGLONG_KEY(pExped->GetSubNo(), key_index);
		gserver.m_listExped.erase(key);
		pExped->SetEndExped();
		delete pExped;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedQuitRepMsg(rmsg, nBossIndex, nTargetIndex, nQuitMode, MSG_EXPED_ERROR_QUIT_END);
		SEND_Q( rmsg, dest );
	}
	else
	{
		//나가는 캐릭터가 보스이면 위임할 보스를 획득 함
		if(nBossIndex == nTargetIndex)
		{
			CExpedMember* pNewBossMember = (CExpedMember*) pExped->FindNextBoss();
			if(pNewBossMember)
			{
				pNewBossMember->SetMemberType(MSG_EXPED_MEMBERTYPE_BOSS);
				LONGLONG key = MAKE_LONGLONG_KEY(pExped->GetSubNo(), nBossIndex);
				gserver.m_listExped.erase(key);
				key = MAKE_LONGLONG_KEY(pExped->GetSubNo(), pNewBossMember->GetCharIndex());
				gserver.m_listExped.insert(map_expedition_t::value_type(key, pExped));
			}
		}

		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedQuitRepMsg(rmsg, nBossIndex, nTargetIndex, nQuitMode, MSG_EXPED_ERROR_QUIT_OK);
		SEND_Q( rmsg, dest );
	}
}

//강퇴
void do_Command_Exped_KickReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int	nBossIndex;
	int	nTargetIndex;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;

	pExped->DeleteMember(nTargetIndex);

	if (pExped->GetMemberCountOnline() < 2)
	{
		// 원정대 삭제
		int key_index;
		if(nBossIndex == nTargetIndex)
		{
			key_index = nBossIndex;
		}
		else
		{
			key_index = pExped->GetBossIndex();
		}
		LONGLONG key = MAKE_LONGLONG_KEY(pExped->GetSubNo(), key_index);
		gserver.m_listExped.erase(key);
		pExped->SetEndExped();
		delete pExped;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedKickRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_EXPED_ERROR_KICK_END);
		SEND_Q( rmsg, dest );
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedKickRepMsg(rmsg, nBossIndex, nTargetIndex, MSG_EXPED_ERROR_KICK_OK);
		SEND_Q( rmsg, dest );
	}
}

//대장 위임
void do_Command_Exped_ChangeBossReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int	nBossIndex, nChangeMode, nNewBossIndex;

	RefMsg(msg) >> nBossIndex
		>> nChangeMode				// 수동,자동
		>> nNewBossIndex;           // 자동(-1)

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;

	// 자동위임 이면
	if(nChangeMode == MSG_EXPED_CHANGEBOSS_AUTO)
	{
		CExpedMember* pNewBossMember = (CExpedMember*) pExped->FindNextBoss();
		if(pNewBossMember)
		{
			if(pExped->ChangeBoss(pNewBossMember->GetCharIndex(),nBossIndex))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperExpedChangeBossRepMsg(rmsg, nBossIndex, pNewBossMember->GetCharIndex(), nChangeMode);
				SEND_Q( rmsg, dest );
			}
		}
	}else if(nChangeMode == MSG_EXPED_CHANGEBOSS_MANUAL)
	{
		if(pExped->ChangeBoss(nNewBossIndex,nBossIndex))
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperExpedChangeBossRepMsg(rmsg, nBossIndex, nNewBossIndex, nChangeMode);
			SEND_Q( rmsg, dest );
		}
	}
}

//타입 변경
void do_Command_Exped_ChangeTypeReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nBossIndex;
	char cDiviType,cExpedType;

	RefMsg(msg) >> nBossIndex
		>> cExpedType
		>> cDiviType;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;
#ifdef BUGFIX_ALLOW_EXPED_TYPE_SET
	pExped->SetExpedType(cExpedType, cDiviType);
#else
	pExped->SetExpedType(cDiviType,cExpedType);
#endif

	CNetMsg::SP rmsg(new CNetMsg);
	HelperExpedChangeTypeRepMsg(rmsg, nBossIndex, cExpedType, cDiviType);
	SEND_Q( rmsg, dest );
}

//부대장 임명
void do_Command_Exped_SetMBossReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nBossIndex, nNewMBossIndex;

	RefMsg(msg) >> nBossIndex
		>> nNewMBossIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
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
 			HelperExpedSetMBossRepMsg(rmsg, nBossIndex, nNewMBossIndex);
 			SEND_Q( rmsg, dest );
		}
	}
}

//부대장 임명 해제
void do_Command_Exped_ResetMBossReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nBossIndex, nNewMBossIndex;

	RefMsg(msg) >> nBossIndex
		>> nNewMBossIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
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
 			HelperExpedResetMBossRepMsg(rmsg, nBossIndex, nNewMBossIndex);
 			SEND_Q( rmsg, dest );
		}
	}
}

//원정대 해체
void do_Command_Exped_EndExpedReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int	nBossIndex;

	RefMsg(msg) >> nBossIndex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;

	// 원정대 삭제
	LONGLONG key = MAKE_LONGLONG_KEY(pExped->GetSubNo(), nBossIndex);
	gserver.m_listExped.erase(key);
	pExped->SetEndExped();
	delete pExped;

	CNetMsg::SP rmsg(new CNetMsg);
	HelperExpedEndExpedRepMsg(rmsg, nBossIndex);
	SEND_Q( rmsg, dest );
}

//그룹이동
void do_Command_Exped_MoveGroupReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nBossIndex, nSourceGroup, nMoveCharIndex, nTargetGroup, nTargetListindex;

	RefMsg(msg) >> nBossIndex
		>> nSourceGroup
		>> nMoveCharIndex
		>> nTargetGroup
		>> nTargetListindex;

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;

	if(pExped->MoveGroup(nSourceGroup,nMoveCharIndex,nTargetGroup,nTargetListindex))
	{
		//성공
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedMoveGroupRepMsg(rmsg, nBossIndex,nSourceGroup, nMoveCharIndex, nTargetGroup, nTargetListindex);
		SEND_Q( rmsg, dest );
	}else
	{
		//에러
	}
}

void do_Command_Expend_OfflineSystem( CNetMsg::SP& msg, CDescriptor* dest )
{
	int	nBossIndex,	nTargetIndex;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex	;				// 나가는 캐릭터인덱스

	CExpedition* pExped = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExped)
		return ;

	CExpedMember *member = pExped->GetMemberByCharIndex( nTargetIndex );
	if( member ) member->m_nLevel = 0;

	if (pExped->GetMemberCountOnline() < 2)
	{
		// 원정대 삭제
		LONGLONG key = MAKE_LONGLONG_KEY(pExped->GetSubNo(), nBossIndex);
		gserver.m_listExped.erase(key);
		pExped->SetEndExped();
		delete pExped;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedQuitRepMsg(rmsg, nBossIndex, nTargetIndex, 0, MSG_EXPED_ERROR_QUIT_END);
		SEND_Q( rmsg, dest );
	}
	else
	{
		//나가는 캐릭터가 보스이면 위임할 보스를 획득 함
		if(nBossIndex == nTargetIndex)
		{
			CExpedMember* pNewBossMember = (CExpedMember*) pExped->FindNextBoss();
			if(pNewBossMember)
			{
				if( pExped->ChangeBoss(pNewBossMember->GetCharIndex(),nBossIndex ) )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperExpedChangeBossRepMsg(rmsg, nBossIndex, pNewBossMember->GetCharIndex() , MSG_EXPED_CHANGEBOSS_AUTO );
					SEND_Q( rmsg, dest );
				}
			}
		}
	}
}

void do_Command_Expend_OnlineSystem( CNetMsg::SP& msg, CDescriptor* dest )
{
	int	nBossIndex,	nTargetIndex, nLevel = 0;;

	RefMsg(msg) >> nBossIndex
		>> nTargetIndex
		>> nLevel;

	CExpedition* pExpend = gserver.FindExpedByBossIndex(dest->m_subNo, nBossIndex);
	if (!pExpend)
		return ;

	CExpedMember *member = pExpend->GetMemberByCharIndex( nTargetIndex );
	if( member ) member->m_nLevel = nLevel;
}


//인존 들어갈 룸번호 획득(파티장 및 원정대장 기준)
#define MAX_INZONE_ROOM_COUNT 20
void do_Command_Raid_InzoneGetRoomNoReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nCharIndex, nZoneNo, nBossIndex, nJoinType;
	RefMsg(msg) >> nCharIndex
		>> nZoneNo
		>> nBossIndex
		>> nJoinType;
	int nDifficulty = -1;
	RefMsg(msg) >> nDifficulty;
	GAMELOG << init("RAID INZONE GET ROOMNO")
			<< "nCharIndex" << nCharIndex	<< delim
			<< "nZoneNo" << nZoneNo	<< delim
			<< "nBossIndex" << nBossIndex	<< delim
			<< "nJoinType" << nJoinType	<< delim
			<< "Difficulty" << nDifficulty << delim
			<< end;

	CDBCmd cmd;
	cmd.Init( &gserver.m_dbchar );

	CLCString sql(1024);

	int nRoomNo=0,nBossRoomNo=0;
	nRoomNo = -1, nBossRoomNo = -1;

	// 원정대 레이드만 처리
	if(nJoinType == MSG_JOINTYPE_EXPED)
	{
		// 레이드에 들어가는 캐릭터가 전에 원정대로 플레이를 했었는지 확인, roomNum 받아옴
		sql.Format("select a_roomno from t_inzonejoin where a_zoneno = %d and a_charindex = %d and a_difficulty = %d", nZoneNo, nCharIndex, nDifficulty);
		cmd.SetQuery(sql);
		if ( !cmd.Open() )
		{
			GAMELOG << init("do_Command_Raid_InzoneGetRoomNoReq - FAIL open()")
					<< "nCharIndex" << nCharIndex << delim
					<< "nZoneNo"	<< nZoneNo	  << delim
					<< end;
			return;
		}
		else if (cmd.m_nrecords > 0)
		{
			if (cmd.MoveFirst())
			{
				cmd.GetRec("a_roomno", nRoomNo);
			}
			else
			{
				GAMELOG << init("do_Command_Raid_InzoneGetRoomNoReq - FAIL MoveFirst()")
						<< "nCharIndex" << nCharIndex << delim
						<< "nZoneNo"	<< nZoneNo	  << delim
						<< end;
				return;
			}
		}

		// 원정대의 보스 캐릭터가 전에 원정대로 플레이를 했었는지 확인, roomNum 받아옴
		sql.Format("select a_roomno from t_inzonejoin where a_zoneno = %d and a_charindex = %d and a_difficulty = %d", nZoneNo, nBossIndex, nDifficulty);
		cmd.SetQuery(sql);
		if ( !cmd.Open() )
		{
			GAMELOG << init("do_Command_Raid_InzoneGetRoomNoReq - FAIL open()")
					<< "nBossIndex" << nBossIndex << delim
					<< "nZoneNo"	<< nZoneNo	  << delim
					<< end;
			return;
		}
		else if (cmd.m_nrecords > 0)
		{
			if (cmd.MoveFirst())
			{
				cmd.GetRec("a_roomno", nBossRoomNo);
			}
			else
			{
				GAMELOG << init("do_Command_Raid_InzoneGetRoomNoReq - FAIL MoveFirst()")
						<< "nBossIndex" << nBossIndex << delim
						<< "nZoneNo"	<< nZoneNo	  << delim
						<< end;
				return;
			}
		}

		// 원정대 보스의 룸번호가 0이면 사용되지 않은 룸검색
		// 원정대 레이드만 처리
		if(nRoomNo==-1 && nBossRoomNo==-1)
 		{
			// 위에서 검색한 최대 룸넘버의 다음 번호로 룸넘버를 부여한다.
			sql.Format("SELECT a_roomno FROM t_inzonejoin where a_zoneno = %d order by a_roomno desc LIMIT 1", nZoneNo);
			cmd.SetQuery(sql);
			if ( !cmd.Open() )
			{
				GAMELOG << init("do_Command_Raid_InzoneGetRoomNoReq - FAIL 1");
				return;
			}
			else if (cmd.m_nrecords > 0)
			{
				if (cmd.MoveFirst())
				{
					cmd.GetRec("a_roomno", nRoomNo);
					nRoomNo += 1;
					nBossRoomNo = nRoomNo;
				}
				else
				{
					GAMELOG << init("do_Command_Raid_InzoneGetRoomNoReq - FAIL 2");
					return;
				}
			}
			else if(cmd.m_nrecords == 0)
			{
				nRoomNo = 0;
				nBossRoomNo = nRoomNo;
			}
#ifdef LC_GAMIGO
			char select_inzonejoin_query[1024] = {0,};
			sprintf(select_inzonejoin_query, "SELECT a_zoneno, a_roomno, a_charindex FROM t_inzonejoin WHERE a_zoneno = %d AND a_roomno = %d AND a_charindex = %d", nZoneNo, nRoomNo, nCharIndex);
			CDBCmd checkcmd;
			checkcmd.Init(&gserver.m_dbchar);
			checkcmd.SetQuery(select_inzonejoin_query);
			if(checkcmd.Open() && checkcmd.m_nrecords == 0)
			{
				sql.Format("insert into t_inzonejoin (a_zoneno, a_roomno, a_charindex, a_difficulty) values (%d, %d, %d, %d)", nZoneNo, nRoomNo, nCharIndex, nDifficulty);
				cmd.SetQuery(sql);
				cmd.Update();
			}
#else
			sql.Format("insert into t_inzonejoin (a_zoneno, a_roomno, a_charindex, a_difficulty) values (%d, %d, %d, %d)", nZoneNo, nRoomNo, nCharIndex, nDifficulty);
			cmd.SetQuery(sql);
			cmd.Update();
#endif
		}

		// 원정대장은 룸넘버가 있고 캐릭터만 없으면, 원정대장의 룸넘버 부여
		else if(nRoomNo == -1 && nBossRoomNo > -1)
		{
			nRoomNo = nBossRoomNo;
#ifdef LC_GAMIGO
			char select_inzonejoin_query[1024] = {0,};
			sprintf(select_inzonejoin_query, "SELECT a_zoneno, a_roomno, a_charindex FROM t_inzonejoin WHERE a_zoneno = %d AND a_roomno = %d AND a_charindex = %d", nZoneNo, nRoomNo, nCharIndex);
			cmd.SetQuery(select_inzonejoin_query);
			if (cmd.Open() && cmd.m_nrecords == 0)
			{
				sql.Format("insert into t_inzonejoin (a_zoneno, a_roomno, a_charindex, a_difficulty) values (%d, %d, %d, %d)", nZoneNo, nRoomNo, nCharIndex, nDifficulty);
				cmd.SetQuery(sql);
				cmd.Update();
			}
#else
			sql.Format("insert into t_inzonejoin (a_zoneno, a_roomno, a_charindex, a_difficulty) values (%d, %d, %d, %d)", nZoneNo, nRoomNo, nCharIndex, nDifficulty);
			cmd.SetQuery(sql);
			cmd.Update();
#endif
		}
	} // if

	// 파티 레이드이면 일괄적으로 방번호를 1로 잡는다.
	else if (nJoinType == MSG_JOINTYPE_PARTY)
		nRoomNo = 1;

	// 게임 데이타가 없으면 nRoomNo 는 0 으로 결과 전달
	CNetMsg::SP rmsg(new CNetMsg);
	HelperRaidInzoneGetRoomNoRep(rmsg, nCharIndex, nZoneNo, nRoomNo, nBossIndex, nBossRoomNo);
	RefMsg(rmsg) << nDifficulty;
	SEND_Q( rmsg, dest );
	return;
}

//인존 데이타 리셋(존 전체)
void do_Command_Raid_InzoneDataClearReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nZoneNo;

	RefMsg(msg) >> nZoneNo;

	CDBCmd cmd;
	CLCString sql(1024);

	//인존 게임 데이타 삭제
	cmd.Init(&gserver.m_dbchar);

	//인존 조인 유저 삭제
	sql.Format("DELETE FROM t_inzonejoin WHERE a_zoneno = %d" , nZoneNo);
	cmd.SetQuery(sql);
	if( !cmd.Update() )
	{
		GAMELOG << init("RAID INZONE DB JOIN MEMBER CLEAR FAIL") << end;
		return;
	}
	if(nZoneNo == ZONE_AKAN_TEMPLE)
	{
		sql.Format("DELETE FROM t_trigger_saveinfo WHERE a_zone_index = %d", nZoneNo);
		cmd.SetQuery(sql);
		if(!cmd.Update())
		{
			GAMELOG << init("RAID INZONE DB JOIN MEMBER CLEAR FAIL") << "Zone No." << delim << nZoneNo << end;
			return;
		}
	}
	if(nZoneNo == ZONE_TARIAN_DUNGEON)
	{
		sql.Format("DELETE FROM t_trigger_saveinfo WHERE a_zone_index = %d", nZoneNo);
		cmd.SetQuery(sql);
		if(!cmd.Update())
		{
			GAMELOG << init("RAID INZONE DB JOIN MEMBER CLEAR FAIL") << "Zone No." << delim << nZoneNo << end;
			return;
		}
	}
}

void do_Command_Raid_InzoneSetRoomNo ( CNetMsg::SP& msg , CDescriptor* dest )
{
	int nNewBossIndex, nOldBossIndex, nJoinType, nZoneNo;

	RefMsg(msg) >> nNewBossIndex
		>> nOldBossIndex
		>> nZoneNo
		>> nJoinType;
	int nDifficulty = -1;
	RefMsg(msg) >> nDifficulty;
	CDBCmd cmd;
	CLCString sql(1024);

	cmd.Init( &gserver.m_dbchar );

	sql.Format("select a_roomno from t_inzonejoin where a_zoneno = %d and a_charindex = %d", nZoneNo, nNewBossIndex);
	cmd.SetQuery(sql);
	if ( cmd.Open() && cmd.MoveFirst() )
	{
		return;
	}
	else
	{
		sql.Format("insert into t_inzonejoin (a_zoneno, a_roomno, a_charindex, a_difficulty) SELECT  a_zoneno, a_roomno, %d, a_difficulty "
			"FROM t_inzonejoin WHERE a_zoneno = %d and a_charindex = %d and a_difficulty = %d", nNewBossIndex, nZoneNo, nOldBossIndex, nDifficulty );
		cmd.SetQuery(sql);
		cmd.Update();
		return;
	}
	return;
}


void do_CommandTriggerEvent( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nZoneIdx = 0, nRoomIdx = 0, nSaveInfo = 0;
	RefMsg(msg) >> nZoneIdx
		>> nRoomIdx
		>> nSaveInfo;

	CDBCmd DB_cmd;
	CLCString sql(256);
	DB_cmd.Init(&gserver.m_dbchar);

	// 암흑의 제단 지우는 요청이면
	if(nSaveInfo == TRIGGER_SAVE_ALTER_OF_DARK_NONE)
	{
		sql.Format("delete from t_trigger_saveinfo where a_zone_index=%d", nZoneIdx);
		DB_cmd.SetQuery(sql);
		if (!DB_cmd.Update())
		{
			GAMELOG << init("do_CommandTriggerEvent - DELETE FAIL")
					<< nZoneIdx		<< delim
					<< end;
		}
		else
			return;
	}

	sql.Format("SELECT * FROM t_trigger_saveinfo WHERE a_zone_index=%d AND a_room_index=%d ", nZoneIdx , nRoomIdx );
	DB_cmd.SetQuery(sql);
	if ( !DB_cmd.Open() )
	{
		GAMELOG << init("do_CommandTriggerEvent - SAVE FAIL 1")
				<< nZoneIdx		<< delim
				<< nRoomIdx		<< delim
				<< nSaveInfo
				<< end;
	}
	else if (DB_cmd.m_nrecords > 0)
	{
		sql.Format("update t_trigger_saveinfo set a_save_info=%d, a_save_time=NOW() where a_zone_index=%d AND a_room_index=%d ",
			nSaveInfo, nZoneIdx, nRoomIdx);
		DB_cmd.SetQuery(sql);
		if (!DB_cmd.Update())
		{
			GAMELOG << init("do_CommandTriggerEvent - SAVE FAIL 2")
					<< nZoneIdx		<< delim
					<< nRoomIdx		<< delim
					<< nSaveInfo
					<< end;
		}
	}
	else
	{
		sql.Format("insert into t_trigger_saveinfo (a_zone_index, a_room_index, a_save_info, a_save_time) values (%d, %d, %d, NOW())",
			nZoneIdx, nRoomIdx, nSaveInfo);
		DB_cmd.SetQuery(sql);
		if (!DB_cmd.Update())
		{
			GAMELOG << init("do_CommandTriggerEvent - SAVE FAIL 3")
					<< nZoneIdx		<< delim
					<< nRoomIdx		<< delim
					<< nSaveInfo
					<< end;
		}
	}
}

void do_Command_DeleteRaidCharacter( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nCharIndex = 0;
	int nSuccess = 0;		// 0:삭제 실패, 1:삭제 성공
	int nResult = 0;
	int i;
	int* zone;
	int* room;
	int* difficulty;
	CDBCmd cmd;

	RefMsg(msg) >> nCharIndex;

	cmd.Init(&gserver.m_dbchar);
	CLCString sql(1024);
	sql.Format("SELECT * FROM t_inzonejoin WHERE a_charindex = %d", nCharIndex);
	cmd.SetQuery(sql);

	if(!cmd.Open())
	{
		GAMELOG << init("do_Command_DeleteRaidCharacter - OPEN FAIL")
				<< nCharIndex		<< delim
				<< end;

		nSuccess = 0;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperDeleteRaidCharacterRep(rmsg, nCharIndex, nSuccess);
		SEND_Q(rmsg, dest);
		return ;
	}

	nResult = cmd.m_nrecords;
	if(!cmd.MoveFirst() || nResult == 0)
	{
		GAMELOG << init("do_Command_DeleteRaidCharacter - NO CHARACTER INFO")
				<< nCharIndex		<< delim
				<< end;

		nSuccess = 0;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperDeleteRaidCharacterRep(rmsg, nCharIndex, nSuccess);
		SEND_Q(rmsg, dest);
		return ;
	}
	zone = new int[nResult];
	room = new int[nResult];
	difficulty = new int[nResult];
	for(i = 0; i < nResult; i++)
	{
		zone[i] = -1;
		room[i] = -1;
		difficulty[i] = -1;
	}
	i = 0;
	do
	{
		cmd.GetRec("a_roomno", room[i]);
		cmd.GetRec("a_zoneno", zone[i]);
		cmd.GetRec("a_difficulty", difficulty[i]);
		i++;
	}
	while (cmd.MoveNext());

	sql.Format("DELETE FROM t_inzonejoin WHERE a_charindex = %d", nCharIndex);
	cmd.SetQuery(sql);
	if(!cmd.Update())
	{
		GAMELOG << init("do_Command_DeleteRaidCharacter - DELETE t_inzonejoin")
				<< nCharIndex		<< delim
				<< end;

		nSuccess = 0;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperDeleteRaidCharacterRep(rmsg, nCharIndex, nSuccess);
		SEND_Q(rmsg, dest);

		delete[] zone;
		delete[] room;
		delete[] difficulty;
		return ;
	}

	for(i = 0; i < nResult; i++)
	{
		sql.Format("SELECT * FROM t_inzonejoin WHERE a_zoneno = %d and a_roomno = %d and a_difficulty = %d", zone[i], room[i], difficulty[i]);
		cmd.SetQuery(sql);
		if(cmd.Open())
		{
			if(cmd.m_nrecords == 0)
			{
				sql.Format("DELETE FROM t_trigger_saveinfo WHERE a_zone_index = %d and a_room_index = %d and a_difficulty = %d", zone[i], room[i], difficulty[i]);
				cmd.SetQuery(sql);
				if(!cmd.Update())
				{
					GAMELOG << init("DELETE ERROR t_trigger_saveinfo") << sql << end;
				}
				else
				{
					GAMELOG << init("DELETE RAID SAVE INFO")
							<< "ZONE No." << delim << zone[i] << delim
							<< "ROOM No." << delim << room[i] << delim
							<< "Difficulty" << delim << difficulty[i] << delim
							<< end;
				}
			}
		}
	}

	nSuccess = 1;
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperDeleteRaidCharacterRep(rmsg, nCharIndex, nSuccess);
		SEND_Q(rmsg, dest);
	}

	delete[] zone;
	delete[] room;
	delete[] difficulty;

	return;
}

void do_Command_RaidInfo( CNetMsg::SP& msg, CDescriptor* dest )
{
	int nCharIndex = 0;
	RefMsg(msg) >> nCharIndex;

	int nRaidCount = 0;		// 귀속된 레이드 개수

	CDBCmd DB_cmd;
	CLCString sql(256);

	DB_cmd.Init(&gserver.m_dbchar);

	// 레이드에 귀속된 캐릭터 정보를 가지고 게임 서버로 간다.
	sql.Format("select a_zoneno, a_roomno from t_inzonejoin where a_charindex=%d", nCharIndex);
	DB_cmd.SetQuery(sql);
	if (!DB_cmd.Open())
	{
		GAMELOG << init("do_Command_DeleteRaidCharacter - FAIL Open")
				<< nCharIndex		<< delim
				<< end;
		return;
	}

	// 저장된 레이드의 종류 개수를 구한다.
	nRaidCount = DB_cmd.m_nrecords;
	int* nZoneNum = new int[nRaidCount];
	int* nRoomNum = new int[nRaidCount];
	int i = 0;

	if (nRaidCount)
	{
		while(DB_cmd.MoveNext())
		{
			DB_cmd.GetRec("a_zoneno", nZoneNum[i]);
			DB_cmd.GetRec("a_roomno", nRoomNum[i]);

			i++;
			if(i >= nRaidCount)
				break;
		}

		CNetMsg::SP rmsg(new CNetMsg);
		HelperRaidInfoRep(rmsg, nCharIndex, nRaidCount, nZoneNum, nRoomNum);
		SEND_Q(rmsg, dest);
	}

	// 저장된 레이드가 없으면 비었다는 정보를 보내준다.
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperRaidInfoRep(rmsg, nCharIndex, nRaidCount, NULL, NULL);
		SEND_Q(rmsg, dest);
	}

	nZoneNum = NULL;
	nRoomNum = NULL;
	return;
}

void do_CommandNSCreateCard(CNetMsg::SP& msg, CDescriptor* dest)
{
	int userindex, charindex;
	int itemindex;
	unsigned char subtype;
	RefMsg(msg) >> subtype
		>> userindex
		>> charindex;

	switch(subtype)
	{
	case MSG_HELPER_NS_CARD_USE:
		{
			RefMsg(msg) >> itemindex;
			CLCString sql(1000);
			sql.Format("SELECT a_index FROM t_characters WHERE  a_user_index=%d and a_enable=1 and a_job=6", userindex);

			CDBCmd cmd;
			cmd.Init(&gserver.m_dbchar);
			cmd.SetQuery(sql);
			if(!cmd.Open())
			{
				// 에러메시지
				GAMELOG << init("NS_CREATE_CARD t_characters_open_failed_ns")
						<< "USERINDEX" << delim
						<< userindex << delim
						<< "CHARINDEX" << delim
						<< charindex << end;

				CNetMsg::SP rmsg(new CNetMsg);
				HelperNSCreateCardErrorMsg(rmsg, MSG_HELPER_NS_CARD_ERROR_ETC, userindex, charindex);
				SEND_Q(rmsg, dest);
				return ;
			}

			if(cmd.MoveFirst())
			{
				// 나이트 쉐도우 이미 생성했다.
				GAMELOG << init("NS_CREATE_CARD ns_already_created")
						<< "USERINDEX" << delim
						<< userindex << delim
						<< "CHARINDEX" << delim
						<< charindex << end;

				CNetMsg::SP rmsg(new CNetMsg);
				HelperNSCreateCardErrorMsg(rmsg, MSG_HELPER_NS_CARD_ERROR_CREATED_NS, userindex, charindex);
				SEND_Q(rmsg, dest);
				return ;
			}

			sql.Format("SELECT count(a_index) as char90lvCount from t_characters where a_user_index=%d and a_enable=1 and a_level >= 90 and a_server = %d", userindex, gserver.m_serverno);

			cmd.SetQuery(sql);
			if(!cmd.Open() || !cmd.MoveFirst())
			{
				// 에러메시지
				GAMELOG << init("NS_CREATE_CARD t_characters_open_failed_90lv")
						<< "USERINDEX" << delim
						<< userindex << delim
						<< "CHARINDEX" << delim
						<< charindex << end;

				CNetMsg::SP rmsg(new CNetMsg);
				HelperNSCreateCardErrorMsg(rmsg, MSG_HELPER_NS_CARD_ERROR_ETC, userindex, charindex);
				SEND_Q(rmsg, dest);
				return ;
			}

			int char90lvCount;
			cmd.GetRec("char90lvCount", char90lvCount);
			if(char90lvCount > 0)
			{
				// 90 레벨 캐릭터 있다.
				GAMELOG << init("NS_CREATE_CARD 90lvChar_already_created")
						<< "USERINDEX" << delim
						<< userindex << delim
						<< "CHARINDEX" << delim
						<< charindex << end;
				CNetMsg::SP rmsg(new CNetMsg);
				HelperNSCreateCardErrorMsg(rmsg, MSG_HELPER_NS_CARD_ERROR_90LV, userindex, charindex);
				SEND_Q(rmsg, dest);
				return ;
			}

			sql.Format("INSERT INTO t_nscard (a_user_index, a_usetime) VALUES ( %d, NOW())",  userindex);
			cmd.SetQuery(sql);
			if(cmd.Update())
			{
				// 업데이트 성공
				GAMELOG << init("NS_CREATE_CARD itemuse_success")
						<< "USERINDEX" << delim
						<< userindex << delim
						<< "CHARINDEX" << delim
						<< charindex << end;

				CNetMsg::SP rmsg(new CNetMsg);
				HelperNSCreateCardErrorMsg(rmsg, MSG_HELPER_NS_CARD_ERROR_USE_OK, userindex, charindex);
				RefMsg(rmsg) << itemindex;
				SEND_Q(rmsg, dest);
			}
			else
			{
				// 이미 사용했음
				GAMELOG << init("NS_CREATE_CARD already_itemuse")
						<< "USERINDEX" << delim
						<< userindex << delim
						<< "CHARINDEX" << delim
						<< charindex << end;

				CNetMsg::SP rmsg(new CNetMsg);
				HelperNSCreateCardErrorMsg(rmsg, MSG_HELPER_NS_CARD_ERROR_ALREADY, userindex, charindex);
				RefMsg(rmsg) << itemindex;
				SEND_Q(rmsg, dest);
			}
		}
		break;
	case MSG_HELPER_NS_CARD_USE_CANCEL:
		{
			CLCString sql(1000);
			sql.Format("DELETE FROM t_nscard WHERE a_user_index=%d", userindex);

			CDBCmd cmd;
			cmd.Init(&gserver.m_dbchar);
			cmd.SetQuery(sql);
			if(cmd.Update())
			{
				GAMELOG << init("NS_CREATE_CARD itemuse_cancel_success")
						<< "USERINDEX" << delim
						<< userindex << delim
						<< "CHARINDEX" << delim
						<< charindex << end;
			}
			else
			{
				GAMELOG << init("NS_CREATE_CARD itemuse_cancel_failed")
						<< "USERINDEX" << delim
						<< userindex << delim
						<< "CHARINDEX" << delim
						<< charindex << end;
			}
		}
		break;
	case MSG_HELPER_NS_CARD_DELETE_DATA:
		{
			CLCString sql(1000);
			sql.Format("DELETE FROM t_nscard WHERE a_user_index=%d", userindex);

			CDBCmd cmd;
			cmd.Init(&gserver.m_dbchar);
			cmd.SetQuery(sql);
			if(cmd.Update())
			{
				GAMELOG << init("NS_CREATE_CARD delete_data_success")
						<< "USERINDEX" << delim
						<< userindex << delim
						<< "CHARINDEX" << delim
						<< charindex << end;
			}
			else
			{
				GAMELOG << init("NS_CREATE_CARD delete_data_failed")
						<< "USERINDEX" << delim
						<< userindex << delim
						<< "CHARINDEX" << delim
						<< charindex << end;
			}
		}
		break;
	default:
		break;
	}
}

void do_CommandPartyDataInit(CNetMsg::SP& msg, CDescriptor* dest)
{
	map_party_t::iterator it = gserver.m_listParty.begin();
	map_party_t::iterator endit = gserver.m_listParty.end();
	for( ; it != endit; )
	{
		CParty* pParty = it->second;

		if(pParty->m_nSubNo == dest->m_subNo)
		{
			int bossIndex = pParty->GetBossIndex();
			gserver.m_listParty.erase(it++);
			pParty->SetEndParty();
			delete pParty;
			GAMELOG << init("PARTY DATA INIT")
				<< "BOSS INDEX" << delim << bossIndex <<delim
				<< "GAME SERVER CHANNEL" << delim << dest->m_subNo << end;
			// 제거 파티는 매칭 테이블에서 제거
			do_CommandPartyMatchDelReq_real(dest->m_subNo, bossIndex);
		}
		else
		{
			++it;
		}
	}
}

#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)
void do_Worldcup2010Event(CNetMsg::SP& msg, CDescriptor* dest)
{
	unsigned char nSubType;
	RefMsg(msg) >> nSubType;

	switch( nSubType )
	{
	case MSG_HELPER_WORLDCUP2010_TOTO_REQ: // 토토 응모
		do_Worldcup2010EventToto(msg, dest);
		break;

	case MSG_HELPER_WORLDCUP2010_TOTO_STATUS_REQ:
		do_Worldcup2010EventTotoStatus(msg, dest);
		break;

	case MSG_HELPER_WORLDCUP2010_TOTO_GIFT_REQ:
		do_Worldcup2010EventTotoGift(msg, dest);
		break;

	case MSG_HELPER_WORLDCUP2010_ATTENDANCE_REQ:
		do_Worldcup2010EventAttendance(msg, dest);
		break;

	case MSG_HELPER_WORLDCUP2010_KOREA_REQ:
		do_Worldcup2010EventKorea(msg, dest);
		break;
	}
}

void do_Worldcup2010EventToto(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charIndex;
	int itemIndex;

	RefMsg(msg) >> charIndex >> itemIndex;

	CLCString sql(1000);
	sql.Format("SELECT a_char_idx FROM t_event_worldcup_toto WHERE a_char_idx = %d", charIndex);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(!cmd.Open())
	{
		GAMELOG << init("EVENT_WORLDCUP_2010 : TOTO DB OPEN FAIL") << charIndex << delim <<  itemIndex << end;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperWorldcup2010TOTOErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_TOTO_ERROR_FAIL, charIndex, itemIndex);
		SEND_Q(rmsg, dest);
		return;
	}

	if( !cmd.MoveFirst() ) // 등록된 내용이 없다.
	{
		sql.Format("Insert into t_event_worldcup_toto(a_char_idx, a_item_idx, a_date) values(%d, %d, NOW())", charIndex, itemIndex);
	}
	else
	{
		sql.Format("update t_event_worldcup_toto set a_item_idx=%d, a_date=NOW() where a_char_idx=%d", itemIndex, charIndex);
	}
	cmd.SetQuery(sql);

	CNetMsg::SP rmsg(new CNetMsg);
	if( !cmd.Update() )
	{
		GAMELOG << init("EVENT_WORLDCUP_2010 : TOTO DB FAIL") << charIndex << delim <<  itemIndex << end;
		HelperWorldcup2010TOTOErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_TOTO_ERROR_FAIL, charIndex, itemIndex);
	}
	else
	{
		GAMELOG << init("EVENT_WORLDCUP_2010 : TOTO DB SUC") << charIndex << delim <<  itemIndex << end;
		HelperWorldcup2010TOTOErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_TOTO_ERROR_SUC, charIndex, itemIndex);
	}
	SEND_Q(rmsg, dest);
}
void do_Worldcup2010EventTotoStatus(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charIndex;
	int resultType;

	RefMsg(msg) >> charIndex >> resultType;

	CLCString sql(1000);
	sql.Format("SELECT * FROM t_event_worldcup_toto WHERE a_char_idx=%d", charIndex);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(!cmd.Open() || !cmd.MoveFirst() ) // 없다.
	{
		GAMELOG << init("EVENT_WORLDCUP_2010 : TOTOSTATUS DB OPEN FAIL") << charIndex << delim <<  resultType << end;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperWorldcup2010TOTOStatusErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_TOTO_STATUS_ERROR_FAIL, charIndex, resultType);
		SEND_Q(rmsg, dest);
		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	int itemIndex;
	if(	!cmd.GetRec("a_item_idx"	, itemIndex) )
	{
		GAMELOG << init("EVENT_WORLDCUP_2010 : TOTOSTATUS DB FAIL") << charIndex << delim <<  resultType << delim << itemIndex << end;
		HelperWorldcup2010TOTOStatusErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_TOTO_STATUS_ERROR_FAIL, charIndex, resultType, itemIndex);
	}
	else
	{
		GAMELOG << init("EVENT_WORLDCUP_2010 : TOTOSTATUS DB SUC") << charIndex << delim <<  resultType << delim << itemIndex <<end;
		HelperWorldcup2010TOTOStatusErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_TOTO_STATUS_ERROR_SUC, charIndex, resultType, itemIndex);
	}
	SEND_Q(rmsg, dest);
}
void do_Worldcup2010EventTotoGift(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charIndex, restore;
	CLCString sql(1000);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	RefMsg(msg) >> charIndex >> restore;

	if( restore != 0)
	{
		sql.Format("update t_event_worldcup_toto set a_result=1 where a_char_idx=%d", charIndex);
		cmd.SetQuery(sql);
		if( !cmd.Update() )
		{
			GAMELOG << init("EVENT_WORLDCUP_2010 RESULT : TOTO GIFT RESULT RESTORE DB FAIL") << charIndex <<end;
			CNetMsg::SP rmsg(new CNetMsg);
			HelperWorldcup2010TOTOGiftErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_TOTO_GIFT_ERROR_FAIL, charIndex);
			return;
		}
		GAMELOG << init("EVENT_WORLDCUP_2010 RESULT : TOTO GIFT RESULT RESTORE DB SUC") << charIndex << delim << restore << end;
		return;
	}

	sql.Format("SELECT * FROM t_event_worldcup_toto WHERE a_char_idx=%d and a_result>0", charIndex);
	cmd.SetQuery(sql);
	if(!cmd.Open() || !cmd.MoveFirst() ) // 없다.
	{
		GAMELOG << init("EVENT_WORLDCUP_2010 : TOTO GIFT DB OPEN FAIL") << charIndex <<end;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperWorldcup2010TOTOGiftErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_TOTO_GIFT_ERROR_FAIL, charIndex);
		SEND_Q(rmsg, dest);
		return;
	}

	int result;
	cmd.GetRec("a_result", result);
	if( result == 2) // 이미 받은 상태
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperWorldcup2010TOTOGiftErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_TOTO_GIFT_ERROR_AREADY, charIndex);
		SEND_Q(rmsg, dest);
		return;
	}

	 // 0 응모중 , 1 당첨 아이템 지급, 2. 이미 지급 된 상태.
	// 지급된 상태로 바꾸고 지급 가능으로 보낸다.
	CNetMsg::SP rmsg(new CNetMsg);
	sql.Format("update t_event_worldcup_toto set a_result=2, a_date=NOW() where a_char_idx=%d", charIndex);
	cmd.SetQuery(sql);
	if( !cmd.Update() )
	{
		GAMELOG << init("EVENT_WORLDCUP_2010 : TOTO GIFT DB FAIL") << charIndex <<end;
		HelperWorldcup2010TOTOGiftErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_TOTO_GIFT_ERROR_FAIL, charIndex);
	}
	else
	{
		GAMELOG << init("EVENT_WORLDCUP_2010 : TOTO GIFT DB SUC") << charIndex <<end;
		HelperWorldcup2010TOTOGiftErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_TOTO_GIFT_ERROR_SUC, charIndex);
	}

	SEND_Q(rmsg, dest);
}
void do_Worldcup2010EventAttendance(CNetMsg::SP& msg, CDescriptor* dest)
{
	/*
	int charIndex;
	CNetMsg rmsg;

	msg >> charIndex;

	CLCString sql(1000);
	sql.Format("SELECT * FROM t_event_worldcup_attendance WHERE a_char_idx = %d", charIndex);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(!cmd.Open() )
	{
		HelperWorldcup2010AttendanceErrorMsg(msg, MSG_HELPER_WORLDCUP2010_ATTENDANCE_ERROR_FAIL, charIndex);
		SEND_Q(rmsg, dest);
		return;
	}

	if( !cmd.MoveFirst() )// 없으면 넣어준다.
	{
		sql.Format("Insert into t_event_worldcup_attendance(a_char_idx, a_date) values(%d, NOW())",	charIndex);
		cmd.SetQuery(sql);
		if( !cmd.Open() || ! cmd.Update() )
			HelperWorldcup2010AttendanceErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_ATTENDANCE_ERROR_FAIL, charIndex);
		else
			HelperWorldcup2010AttendanceErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_ATTENDANCE_ERROR_SUC, charIndex);
		SEND_Q(rmsg, dest);
		return;
	}

	// 있으면 날짜를 확인하자.
	sql.Format("SELECT * FROM t_event_worldcup_attendance WHERE a_char_idx = %d and a_date < NOW()", charIndex);

	cmd.Open();
	if( !cmd.MoveFirst() ) // 이미 지급된 상태
		HelperWorldcup2010AttendanceErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_ATTENDANCE_ERROR_ALREADY, charIndex);
	else
	{
		sql.Format("update t_event_worldcup_attendance set a_date=NOW() where a_char_idx=%d", charIndex);
		cmd.SetQuery(sql);
		if( !cmd.Update() )
			HelperWorldcup2010AttendanceErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_ATTENDANCE_ERROR_FAIL, charIndex);
		else
			HelperWorldcup2010AttendanceErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_ATTENDANCE_ERROR_SUC, charIndex);
	}

	SEND_Q(rmsg, dest);
	*/
}

void do_Worldcup2010EventKorea(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charIndex;
	int type;

	RefMsg(msg) >> charIndex;
	RefMsg(msg) >> type;		// 0 : 확인용, 1 : 지급, 2 : 지급 롤백

	CLCString sql(1000);
	sql.Format("SELECT * FROM t_event_worldcup_korea WHERE a_char_idx = %d", charIndex);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if( !cmd.Open() )
	{
		GAMELOG << init("EVENT_WORLDCUP_2010 : KOREA GIFT DB OPEN FAIL") << charIndex <<end;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperWorldcup2010KoreaErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_KOREA_ERROR_FAIL, charIndex);
		SEND_Q(rmsg, dest);
		return;
	}

	if( 0 == type )			// 확인 처리
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if( !cmd.MoveFirst() )
		{
			HelperWorldcup2010KoreaErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_KOREA_ERROR_EMPTY, charIndex);
		}
		else
		{
			HelperWorldcup2010KoreaErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_KOREA_ERROR_ALREADY, charIndex);
		}
		SEND_Q(rmsg, dest);
	}
	else if( 1 == type )	// 지급 처리
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if( !cmd.MoveFirst() )
		{
			sql.Format("Insert into t_event_worldcup_korea(a_char_idx, a_date) values(%d, NOW())",	charIndex);
			cmd.SetQuery(sql);
			if( !cmd.Update() )
			{
				GAMELOG << init("EVENT_WORLDCUP_2010 : KOREA GIFT DB INSERT FAIL") << charIndex <<end;
				HelperWorldcup2010KoreaErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_KOREA_ERROR_FAIL, charIndex);
			}
			else
			{
				GAMELOG << init("EVENT_WORLDCUP_2010 : KOREA GIFT DB SUC") << charIndex <<end;
				HelperWorldcup2010KoreaErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_KOREA_ERROR_SUC, charIndex);
			}
		}
		else
		{
			GAMELOG << init("EVENT_WORLDCUP_2010 : KOREA GIFT DB INSERT ALREADY") << charIndex <<end;
			HelperWorldcup2010KoreaErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_KOREA_ERROR_ALREADY, charIndex);
		}
		SEND_Q(rmsg, dest);
	}
	else if( 2 == type )	// 롤백 처리
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if( !cmd.MoveFirst() )
		{
			GAMELOG << init("EVENT_WORLDCUP_2010 : KOREA GIFT DB ROLLBACK NODATA") << charIndex <<end;
			HelperWorldcup2010KoreaErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_KOREA_ERROR_FAIL, charIndex);
		}
		else
		{
			sql.Format("DELETE FROM t_event_worldcup_korea WHERE a_char_idx=%d", charIndex);
			cmd.SetQuery(sql);
			if(cmd.Update())
			{
				GAMELOG << init("EVENT_WORLDCUP_2010 : KOREA GIFT DB ROLLBACK SUC") << charIndex <<end;
				HelperWorldcup2010KoreaErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_KOREA_ERROR_FAIL, charIndex);
			}
			else
			{
				GAMELOG << init("EVENT_WORLDCUP_2010 : KOREA GIFT DB ROLLBACK FAIL") << charIndex <<end;
				HelperWorldcup2010KoreaErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_KOREA_ERROR_FAIL, charIndex);
			}
		}
		SEND_Q(rmsg, dest);
	}
	else					// 이상한 값은 실패처리
	{
		GAMELOG << init("EVENT_WORLDCUP_2010 : KOREA GIFT DB STRANGE TYPE") << charIndex <<end;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperWorldcup2010KoreaErrorMsg(rmsg, MSG_HELPER_WORLDCUP2010_KOREA_ERROR_FAIL, charIndex);
		SEND_Q(rmsg, dest);
	}
}

#endif //#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)


void do_CommandUseGuildPoint(CNetMsg::SP& msg, CDescriptor* dest)
{
	int gindex;
	int needgp;
	int gp;
	RefMsg(msg) >> gindex
		>> needgp;
	CLCString sql(1024);
	CDBCmd cmd;
	CGuild* guild = gserver.m_guildlist.findguild(gindex);
	if(!guild)
	{
		// 길드가 없으면 어떻게 할지 고민좀 하자..
		return ;
	}
	gp = guild->GetGuildPoint();

	if(gp <= needgp)
		gp = 0;
	else
		gp = gp - needgp;

	cmd.Init(&gserver.m_dbchar);
	sql.Format("UPDATE t_extend_guild set a_guild_point = %d where a_guild_index = %d", gp, gindex);
	cmd.SetQuery(sql);
	if(!cmd.Update())
	{
		GAMELOG << init("GUILD POINT UPDATE ERROR")
				<< "GUILD INDEX" << delim << gindex << delim
				<< "USE GUILD POINT" << delim << needgp
				<< end;
		return ;
	}
	else
	{
		GAMELOG << init("GUILD POINT UPDATE")
				<< "GUILD INDEX" << delim
				<< gindex << delim
				<< "USE GUILD POINT" << delim
				<< needgp << delim
				<< "TOTAL GUILD POINT" << delim
				<< gp
				<< end;
	}
	guild->guildPoint(gp);
}

void do_TeachRenewer(CNetMsg::SP& msg, CDescriptor* dest)
{
	unsigned char subType;
	RefMsg(msg) >> subType;

	switch(subType)
	{
	case MSG_HELPER_TEACH_RENEWER_TIME_LIMIT_REQ:
		do_TeachRenewerLimitTime(msg, dest);
		break;

	case MSG_HELPER_TEACH_RENEWER_TEACHER_GIFT_REQ:
		{
			unsigned char errorType;
			RefMsg(msg) >> errorType;
			switch( errorType )
			{
			case MSG_HELPER_TEACH_RENEWER_GIFT_CNT_ADD:
				do_TeachRenewerTeacherGiftAdd(msg, dest);
				break;
			case MSG_HELPER_TEACH_RENEWER_GIFT:
				do_TeachRenewerTeacherGift(msg, dest);
				break;
			}
		}
		break;
	case MSG_HELPER_TEACH_RENEWER_STUDENT_SUC_REQ:
		do_TeachRenewerStuedntSuc(msg, dest);
		break;
	}
}

void do_TeachRenewerLimitTime(CNetMsg::SP& msg, CDescriptor* dest)
{
	int teacherIDX;
	RefMsg(msg) >> teacherIDX;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	// 선생 데이터베이스
	char select_characters_query[1024] = {0,};
	sprintf(select_characters_query, "select a_teach_idx from t_characters where a_index=%d ", teacherIDX);
	cmd.SetQuery(select_characters_query);
	if( !cmd.Open() || !cmd.MoveFirst() )
		return;

	int studentIDX[TEACH_MAX_STUDENTS];
	int reStudentIDX[TEACH_MAX_STUDENTS];

	memset(studentIDX, -1, sizeof(studentIDX) );
	memset(reStudentIDX, -1, sizeof(reStudentIDX) );

	CLCString strStudent(256);
	cmd.GetRec("a_teach_idx", strStudent);

	int cnt =0;
	const char* pStudent = strStudent;
	while (*pStudent)
	{
		int idx;
		char tmpBuf[100] = {0,};
		pStudent = AnyOneArg(pStudent, tmpBuf);
		idx = atoi(tmpBuf);
		if( idx != -1)
		{
			studentIDX[cnt] = idx;
			cnt++;
		}
	}

	if( cnt == 0 )
		return;

	int i, count=0;
	for (i=0; i<cnt; i++) // 등록된 후견인 중에 시간이 지난넘을 찾는다.
	{
		char select_characters_query_1[1024] = {0,};
		sprintf(select_characters_query_1, "select a_teach_idx, a_teach_sec from t_characters where a_datestamp < (now() - interval %d day ) and a_index =%d"
			, TEACH_LIMIT_DAY, studentIDX[i] );

		cmd.SetQuery(select_characters_query_1);
		if( !cmd.Open() )
			return;

		cmd.MoveFirst();
		if( cmd.m_nrecords == 1 ) // 10일이 지났다면 1이 나올 수 밖에 없다.
		{
			CLCString buf(256);
			int tIDX, gCnt;
			cmd.GetRec("a_teach_idx", buf);
			tIDX = atoi(buf);
			cmd.GetRec("a_teach_sec", buf);
			gCnt = atoi(buf);

			if( tIDX == teacherIDX)
			{
//				int teach_type = MSG_TEACH_LIMITE_DAY_FAIL;
//
//				if( gCnt >=3 )
//					teach_type = MSG_TEACH_NO_TYPE;
//
//				sprintf(g_buf, "update t_characters set a_teach_type=%d, a_teach_idx='-1', a_teach_sec='%d' where a_index =%d"
//				,teach_type, gCnt, studentIDX[i] );
//				cmd.SetQuery(g_buf);
//				cmd.Update();
				gCnt++;
				reStudentIDX[count]=studentIDX[i];
				count++;
			}
		}
	}

	// 모두 찾았다 GameServer로 보내자.
	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_HELPER_COMMAND);
	RefMsg(rmsg) << MSG_HELPER_TEACHER_SYSTEM_RENEWER
		 << (unsigned char) MSG_HELPER_TEACH_RENEWER_TIME_LIMIT_REP
		 << teacherIDX
		 << count;
	for(i=0; i<count; i++)
	{
		RefMsg(rmsg) << reStudentIDX[i];
	}

	SEND_Q(rmsg, dest);
}

void do_TeachRenewerTeacherGiftAdd(CNetMsg::SP& msg, CDescriptor* dest)
{
	int teacherIdx, count;
	RefMsg(msg) >> teacherIdx
		>> count;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	// 선생 데이터베이스
	char select_guardian_gift_query[1024] = {0,};
	sprintf(select_guardian_gift_query, "select a_gift_count from t_guardian_gift where a_char_idx=%d ", teacherIdx);

	cmd.SetQuery(select_guardian_gift_query);
	if( !cmd.Open() )
	{
		GAMELOG << init("GUARDIAN TEACHER GIFT ADD DB OPEN ERRER")
			<< "teacherIdx" << delim << teacherIdx << end;
		return;
	}

	if( !cmd.MoveFirst() ) // DB에 기록된게 없다면 새로 넣어주자.
	{
		char isnert_guardian_gift_query[1024] = {0,};

		if( count == -1 )
		{
			sprintf(isnert_guardian_gift_query, "insert into t_guardian_gift values( %d, 1)", teacherIdx);
		}
		else
		{
			sprintf(isnert_guardian_gift_query, "insert into t_guardian_gift values( %d, %d)", teacherIdx, count);
		}
		cmd.SetQuery(isnert_guardian_gift_query);
		if( !cmd.Update())
		{
			// 실패하면 로그 찍기
			GAMELOG << init("GUARDIAN TEACHER GIFT ADD DB ISNERT ERRER")
				<< "teacherIdx" << delim << teacherIdx ;
			if( count )
				GAMELOG << delim << "count" << delim << count ;

			GAMELOG << end;
		}
		return;
	}
	else
	{
		int giftCount;
		cmd.GetRec("a_gift_count", giftCount );
		char udpate_guardian_gift_query[1024] = {0,};

		if( count == -1 )
			sprintf(udpate_guardian_gift_query, "update t_guardian_gift set a_gift_count=%d where a_char_idx=%d ", giftCount+1, teacherIdx);
		else
			sprintf(udpate_guardian_gift_query, "update t_guardian_gift set a_gift_count=%d where a_char_idx=%d ", count, teacherIdx);
		cmd.SetQuery(udpate_guardian_gift_query);

		if( !cmd.Update())
		{
			GAMELOG << init("GUARDIAN TEACHER GIFT ADD DB UPDATE ERRER")
				<< "teacherIdx" << delim << teacherIdx;
			if( count == -1)
				GAMELOG << delim << "giftCount" << delim << giftCount+1;
			else
				GAMELOG << delim << "count" << delim << count;

			GAMELOG	<< end;
		}
		return;
	}
}

void do_TeachRenewerTeacherGift(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charindex;
	RefMsg(msg) >> charindex;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	// 선생 데이터베이스
	char select_guardian_gift_query[1024] = {0,};
	sprintf(select_guardian_gift_query, "select a_gift_count from t_guardian_gift where a_char_idx=%d ", charindex);

	cmd.SetQuery(select_guardian_gift_query);
	if( !cmd.Open() )
	{
		GAMELOG << init("GUARDIAN TEACHER GIFT DB OPEN ERRER")
					<< "charindex" << delim << charindex << end;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperTeachGiftRepMsg(rmsg, MSG_HELPER_TEACH_RENEWER_GIFT_DB, charindex);
		SEND_Q(rmsg, dest);
		return;
	}

	if( !cmd.MoveFirst() ) // 줄게 없다.
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperTeachGiftRepMsg(rmsg, MSG_HELPER_TEACH_RENEWER_GIFT_NOMORE, charindex);
		SEND_Q(rmsg, dest);
		return;
	}
	else
	{
		int giftCount;
		cmd.GetRec("a_gift_count", giftCount );

		if( giftCount < 1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperTeachGiftRepMsg(rmsg, MSG_HELPER_TEACH_RENEWER_GIFT_NOMORE, charindex);
			SEND_Q(rmsg, dest);
			return;
		}

		CNetMsg::SP rmsg(new CNetMsg);
		HelperTeachGiftRepMsg(rmsg, MSG_HELPER_TEACH_RENEWER_GIFT_SUC, charindex, giftCount);
		SEND_Q(rmsg, dest);

		char delete_guardian_gift_query[1024] = {0,};
		sprintf(delete_guardian_gift_query, "delete from t_guardian_gift where a_char_idx=%d ", charindex);
		cmd.SetQuery(delete_guardian_gift_query);

		if( !cmd.Update())
		{
			GAMELOG << init("GUARDIAN TEACHER GIFT DB DELETE ERRER")
					<< "charindex" << delim << charindex << delim
					<< "givecount" << delim << giftCount << end;
		}
		return;
	}
}

void do_TeachRenewerStuedntSuc(CNetMsg::SP& msg, CDescriptor* dest)
{
	int studentIdx, teacherIdx;
	RefMsg(msg) >> studentIdx
		>> teacherIdx;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	// 학생  데이터베이스는 깨끗이
	char udpate_characters_query[1024] = {0,};
	sprintf(udpate_characters_query, "update t_characters set a_teach_type=-1, a_teach_idx= ' -1', a_teach_sec=' 0 0' where a_index=%d ", studentIdx);

	cmd.SetQuery(udpate_characters_query);
	if( !cmd.Update())
	{
		GAMELOG << init("GUARDIAN STUDENT SUC DB UPDATE ERRER")
					<< "charindex" << delim << studentIdx <<  end;
	}

	//선생 DB 정리
	char select_characters_query[1024] = {0,};
	sprintf(select_characters_query, "select a_teach_idx from t_characters where a_index=%d ", teacherIdx);
	cmd.SetQuery(select_characters_query);

	if( !cmd.Open() || !cmd.MoveFirst())
	{
		GAMELOG << init("GUARDIAN TEACH SUC DB SELECT ERROR")
			<< "teacherIdx" << delim << teacherIdx << delim << "studentIdx" << studentIdx << delim << end;
	}
	else
	{
		int studentIDX[TEACH_MAX_STUDENTS];

		memset(studentIDX, -1, sizeof(studentIDX) );

		CLCString strStudent(256);
		cmd.GetRec("a_teach_idx", strStudent);

		int cnt =0;
		const char* pStudent = strStudent;
		while (*pStudent)
		{
			int idx;
			char tmpBuf[100] = {0,};
			pStudent = AnyOneArg(pStudent, tmpBuf);
			idx = atoi(tmpBuf);
			if( idx != -1 && idx != studentIdx )
			{
				studentIDX[cnt] = idx;
				cnt++;
			}
		}

		std::string udpate_characters_query_1 = "UPDATE t_characters SET a_teach_complete=a_teach_complete+1 , a_teach_idx='";
		for(int i=0; i<TEACH_MAX_STUDENTS; i++)
		{
			udpate_characters_query_1 += boost::str(boost::format(" %d") % studentIDX[i]);
		}
		udpate_characters_query_1 += boost::str(boost::format("' WHERE a_index = %d") % teacherIdx);
		cmd.SetQuery(udpate_characters_query_1);
		if( !cmd.Update() )
		{
			GAMELOG << init("GUARDIAN TEACH SUC DB UPDATE ERROR")
			<< "teacherIdx" << delim << teacherIdx << delim << "studentIdx" << studentIdx << delim << end;
		}
	}
}

void do_CommandFaceOff(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charIndex;
	char hairStyle, faceStyle;

	RefMsg(msg) >> charIndex
		>> hairStyle
		>> faceStyle;

	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_HELPER_COMMAND);
	RefMsg(rmsg) << MSG_HELPER_FACEOFF_REP;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	char update_characters_query[1024] = {0,};
	sprintf(update_characters_query, "UPDATE t_characters SET a_hair_style=%d, a_face_style=%d WHERE a_index=%d ", (int)hairStyle, (int)faceStyle, charIndex);
	cmd.SetQuery(update_characters_query);

	if( !cmd.Update() )
		RefMsg(rmsg) << (unsigned char) MSG_EX_FACEOFF_ERROR_FAIL;
	else
		RefMsg(rmsg) << (unsigned char) MSG_EX_FACEOFF_ERROR_SUC;

	RefMsg(rmsg) << charIndex
		 << hairStyle
		 << faceStyle;
	SEND_Q(rmsg, dest);
}

#ifdef EVENT_CHAR_BIRTHDAY
void do_commandEventCharBirthday(CNetMsg::SP& msg, CDescriptor* dest)
{
	unsigned char subtype;
	int charindex;
	RefMsg(msg) >> subtype
		>> charindex;

	switch( (int)subtype )
	{
	case MSG_EVENT_CHAR_BIRTHDAY_GIFT_REQ:
		{
			CDBCmd cmd;
			cmd.Init(&gserver.m_dbchar);
			char select_characters_query[8192] = {0,};
			sprintf(select_characters_query, "select YEAR(now()) as currYear,  YEAR( a_createdate ) as birthYear, "
				" MONTH(now()) as currMonth, MONTH (a_createdate ) as birthMonth, "
				" DAYOFMONTH(now()) as currDay, DAYOFMONTH(a_createdate) as birthDay "
				" from t_characters where a_index=%d" , charindex);

			cmd.SetQuery(select_characters_query);
			if( !cmd.Open() || !cmd.MoveFirst() )
			{
				// 캐릭터를 못찾겠다.
				GAMELOG << init("EVENT_CHAR_BIRTHDAY :: DB FAIL") << charindex << delim << __LINE__  << end;
				CNetMsg::SP rmsg(new CNetMsg);
				HelperEventCharBirthdayGiftRepMsg(rmsg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_FAIL, charindex);
				SEND_Q(rmsg, dest);
				return;
			}

			int currYear, currMonth, currDay;
			int birthYear, birthMonth, birthDay;

			cmd.GetRec("currYear",		currYear);
			cmd.GetRec("currMonth",		currMonth);
			cmd.GetRec("currDay",		currDay);

			cmd.GetRec("birthYear",		birthYear);
			cmd.GetRec("birthMonth",	birthMonth);
			cmd.GetRec("birthDay",		birthDay);

			int years	= currYear	- birthYear;
			int months	= currMonth	- birthMonth;
			int days	= currDay	- birthDay;

			if( years>1 )
			{
				if( months<0 || (months==0 && days<=0))
					years --;
			}
			else if( (years>0 && months>0) ||  (years>0 && months==0 && days>0) )
				years = 1;
			else
				years = 0;

			int bGift = years;

			if( bGift > 0 )
			{
				char select_event_char_birthday[8192] = {0,};
				sprintf(select_event_char_birthday,  "select a_count from t_event_char_birthday  where a_char_idx=%d " , charindex);

				cmd.SetQuery(select_event_char_birthday);
				if( !cmd.Open() )
				{
					// 테이블을 열지 못하넹...
					GAMELOG << init("EVENT_CHAR_BIRTHDAY :: DB FAIL") << charindex << delim << __LINE__  << end;
					CNetMsg::SP rmsg(new CNetMsg);
					HelperEventCharBirthdayGiftRepMsg(rmsg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_FAIL, charindex);
					SEND_Q(rmsg, dest);
					return;
				}

				if( !cmd.MoveFirst() )
				{
					// 네 정보가 없으니 보상 않받은 거다. 정보를 기록하고 보상을 주다.
					char insert_event_char_birthday[8192] = {0,};
					sprintf(insert_event_char_birthday, "Insert into t_event_char_birthday select a_index, a_createdate, NOW(), %d from t_characters where a_index=%d"
					,bGift 	,charindex );
					cmd.SetQuery(insert_event_char_birthday);

					if(!cmd.Update())
					{
						// 업데이트가 않되니 보상이고 뭐고 없다.
						GAMELOG << init("EVENT_CHAR_BIRTHDAY :: DB FAIL") << charindex << delim << __LINE__  << end;
						CNetMsg::SP rmsg(new CNetMsg);
						HelperEventCharBirthdayGiftRepMsg(rmsg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_FAIL, charindex);
						SEND_Q(rmsg, dest);
						return;
					}

					// 보상해주자.
					CNetMsg::SP rmsg(new CNetMsg);
					HelperEventCharBirthdayGiftRepMsg(rmsg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_SUC, charindex, bGift);
					SEND_Q(rmsg, dest);
					return;
				}

				int	giftCount;

				cmd.GetRec("a_count",	giftCount);

				if( bGift > giftCount )
				{	// 보상 받은 기록은 있지만 오래전이다. 보상을 주자.
					char update_event_char_birthday[8192] = {0,};
					sprintf(update_event_char_birthday, "update t_event_char_birthday set a_gift_date=now(), a_count=%d where a_char_idx=%d", bGift, charindex );
					cmd.SetQuery(update_event_char_birthday);
					if( !cmd.Update() )
					{
						GAMELOG << init("EVENT_CHAR_BIRTHDAY :: DB FAIL") << charindex << delim << __LINE__  << end;
						CNetMsg::SP rmsg(new CNetMsg);
						HelperEventCharBirthdayGiftRepMsg(rmsg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_FAIL, charindex);
						SEND_Q(rmsg, dest);
						return;
					}

					CNetMsg::SP rmsg(new CNetMsg);
					HelperEventCharBirthdayGiftRepMsg(rmsg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_SUC, charindex, bGift);
					SEND_Q(rmsg, dest);
					return;
				}
				else
				{	// 이미 받았다 다음년도에 받으삼.
					CNetMsg::SP rmsg(new CNetMsg);
					HelperEventCharBirthdayGiftRepMsg(rmsg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_FAIL, charindex);
					SEND_Q(rmsg, dest);
					return;
				}
			}
			else
			{
				// 캐릭터 생성한지 1년이 않넘었넹.
				CNetMsg::SP rmsg(new CNetMsg);
				HelperEventCharBirthdayGiftRepMsg(rmsg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_FAIL, charindex);
				SEND_Q(rmsg, dest);
				return;
			}
		}
		break;

	case MSG_EVENT_CHAR_BIRTHDAY_BDAY_REQ:
		{
			CDBCmd cmd;
			cmd.Init(&gserver.m_dbchar);
			char select_characters_query[8192] = {0,};
			sprintf(select_characters_query, "select YEAR(a_createdate) as YY, MONTH( a_createdate ) as MM, DAYOFMONTH( a_createdate ) as DD "
							"from t_characters "
							"where a_index=%d" , charindex);

			cmd.SetQuery(select_characters_query);
			if(!cmd.Open())
			{
				GAMELOG << init("EVENT_CHAR_BIRTHDAY :: DB FAIL") << charindex << delim << __LINE__  << end;
				CNetMsg::SP rmsg(new CNetMsg);
				HelperEventCharBirthdayBDayRepMsg(rmsg, charindex, 0, 0, 0);
				SEND_Q(rmsg, dest);
				return;
			}

			int yy;
			char mm, dd;
			yy = 0;
			mm = 0;
			dd = 0;
			if( cmd.MoveFirst() )
			{
				cmd.GetRec("YY", yy);
				cmd.GetRec("MM", mm);
				cmd.GetRec("DD", dd);
			}

			CNetMsg::SP rmsg(new CNetMsg);
			HelperEventCharBirthdayBDayRepMsg(rmsg, charindex, yy, mm, dd);
			SEND_Q(rmsg, dest);
		}
		break;
	}
}

#endif

#ifdef DEV_GUILD_MARK
void do_CommandGuildMarkRegist(CNetMsg::SP& msg, CDescriptor* dest)
{
	int GuildIndex = 0;
	int CharIndex = 0;
	char gm_row = -1;
	char gm_col = -1;
	char bg_row = -1;
	char bg_col = -1;
	unsigned short tab = -1;
	unsigned short invenIndex = -1;
	int markTime = -1;
	CLCString serial;

	RefMsg(msg) >> GuildIndex >> CharIndex >> gm_row >> gm_col >> bg_row >> bg_col >> tab >> invenIndex >> markTime >> serial;

	if( GuildIndex == 0
		|| CharIndex == 0
		//|| gm_row == -1
		//|| gm_col == -1
		//|| bg_row == -1
		//|| bg_col == -1
		//|| item_row == -1
		//|| item_col == -1
		//|| markTime == -1
		)
	{
		return ;
	}

	if(gm_row == -1 && gm_col == -1 && bg_row == -1 && bg_col == -1 && tab == -1 && invenIndex == -1 && markTime == -1)
	{
		// rollback부분 게임서버에서 롤백요청이 왔음.
		CGuild* pGuild = NULL;
		pGuild = gserver.m_guildlist.findguild(GuildIndex);
		if(pGuild)
		{
			pGuild->SetGuildMark(gm_row, gm_col, bg_row, bg_col, markTime);
			CDBCmd cmd;
			cmd.Init(&gserver.m_dbchar);
			CLCString sql(1024);
			sql.Format("UPDATE t_extend_guild SET a_gm_row = -1, a_gm_col = -1, a_bg_row = -1, a_bg_col = -1, a_markTime = -1 WHERE a_guild_index = %d", GuildIndex);
			cmd.SetQuery(sql);
			if(!cmd.Update())
			{
				// 롤백 실패
				GAMELOG << init("GUILD MARK ROLLBACK FAIL") << sql << end;
				return ;
			}
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildMarkRegistRepMsg(rmsg, GuildIndex, CharIndex, 3, gm_row, gm_col, bg_row, bg_col, tab, invenIndex, markTime, serial);
			gserver.SendToAllGameServer(rmsg);
			return ;
		}
		return ;
	}

	CGuild* pGuild = NULL;
	pGuild = gserver.m_guildlist.findguild(GuildIndex);
	if(!pGuild)
	{
		// 1: No Guild
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMarkRegistRepMsg(rmsg, GuildIndex, CharIndex, 1, gm_row, gm_col, bg_row, bg_col, tab, invenIndex, markTime, serial);
		SEND_Q(rmsg, dest);
		return ;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	CLCString sql(1024);
	sql.Format("UPDATE t_extend_guild SET a_gm_row = %d, a_gm_col = %d, a_bg_row = %d, a_bg_col = %d, a_markTime = %d WHERE a_guild_index = %d", (int)gm_row, (int)gm_col, (int)bg_row, (int)bg_col, markTime, GuildIndex);
	cmd.SetQuery(sql);
	if(!cmd.Update())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMarkRegistRepMsg(rmsg, GuildIndex, CharIndex, 2, gm_row, gm_col, bg_row, bg_col, tab, invenIndex, markTime, serial);
		return ;
	}

	pGuild->SetGuildMark(gm_row, gm_col, bg_row, bg_col, markTime);
	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildMarkRegistRepMsg(rmsg, GuildIndex, CharIndex, 0, gm_row, gm_col, bg_row, bg_col, tab, invenIndex, markTime, serial);
	gserver.SendToAllGameServer(rmsg);

	return ;
}
#endif

void do_CommandAutoResetRaid(CNetMsg::SP& msg, CDescriptor* dest)
{
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	std::string query = "DELETE FROM t_inzonejoin";
	cmd.SetQuery(query);
	cmd.Update();

	query = "DELETE FROM t_trigger_saveinfo";
	cmd.SetQuery(query);
	cmd.Update();

	//int i;
	//int zone;
	//int count;
	//int **area;
	//RefMsg(msg) >> zone >> count;

	//CDBCmd cmd;
	//cmd.Init(&gserver.m_dbchar);

	//CLCString sql(8192);
	//CLCString tmp(1024);

	//if(count == 0)
	//{
	//	// 다지워버리자
	//	sql.Format("DELETE FROM t_inzonejoin WHERE a_zoneno = %d", zone);
	//	cmd.SetQuery(sql);
	//	if(!cmd.Update())
	//	{
	//		GAMELOG << init("AUTO RESET RAID")
	//				<< "DELETE ERROR" << delim
	//				<< "Query" << delim
	//				<< sql << end;
	//		return ;
	//	}
	//	GAMELOG << init("AUTO RESET RAID")
	//			<< "DELETE" << delim
	//			<< "Query" << delim
	//			<< sql << end;

	//	sql.Format("DELETE FROM t_trigger_saveinfo WHERE a_zone_index = %d", zone);
	//	cmd.SetQuery(sql);
	//	if(!cmd.Update())
	//	{
	//		GAMELOG << init("AUTO RESET RAID")
	//				<< "DELETE ERROR" << delim
	//				<< "Query" << delim
	//				<< sql << end;
	//		return ;
	//	}
	//	GAMELOG << init("AUTO RESET RAID")
	//			<< "DELETE" << delim
	//			<< "Query" << delim
	//			<< sql << end;
	//}
	//else
	//{
	//	// 2차 동적배열 할당
	//	area = new int*[count];
	//	for(i = 0; i < count; i++)
	//		area[i] = new int[2];

	//	for(i = 0; i < count; i++)
	//	{
	//		// 0에는 방번호가, 1에는 difficulty정보가 있다.
	//		RefMsg(msg) >> area[i][0] >> area[i][1];
	//	}
	//	// 쿼리문을 만들자!!
	//	sql.Format("DELETE FROM t_inzonejoin WHERE ");
	//	for(i = 0; i < count; i++)
	//	{
	//		tmp.Format("!(a_zoneno = %d AND a_roomno = %d AND a_difficulty = %d)", zone, area[i][0], area[i][1]);
	//		sql += tmp;
	//		if(count - 1 != i)
	//		{
	//			tmp.Format(" AND ");
	//			sql += tmp;
	//		}
	//	}
	//	tmp.Format(" AND a_zoneno = %d", zone);
	//	sql += tmp;
	//	// 쿼리문 완료!!
	//	cmd.SetQuery(sql);
	//	if(!cmd.Update())
	//	{
	//		GAMELOG << init("AUTO RESET RAID")
	//				<< "DELETE ERROR" << delim
	//				<< "Query" << delim
	//				<< sql << end;
	//		return ;
	//	}
	//	GAMELOG << init("AUTO RESET RAID")
	//				<< "DELETE" << delim
	//				<< "Query" << delim
	//				<< sql << end;

	//	sql.Format("DELETE FROM t_trigger_saveinfo WHERE ");
	//	for(i = 0; i < count; i++)
	//	{
	//		tmp.Format("!(a_zone_index = %d AND a_room_index = %d AND a_difficulty = %d)", zone, area[i][0], area[i][1]);
	//		sql += tmp;
	//		if(count - 1 != i)
	//		{
	//			tmp.Format(" AND ");
	//			sql += tmp;
	//		}
	//	}

	//	tmp.Format(" AND a_zone_index = %d", zone);
	//	sql += tmp;

	//	cmd.SetQuery(sql);
	//	if(!cmd.Update())
	//	{
	//		GAMELOG << init("AUTO RESET RAID")
	//				<< "DELETE ERROR" << delim
	//				<< "Query" << delim
	//				<< sql << end;
	//		return ;
	//	}
	//	GAMELOG << init("AUTO RESET RAID")
	//				<< "DELETE" << delim
	//				<< "Query" << delim
	//				<< sql << end;
	//	// 2차 동적배열 해제
	//	for(i = 0; i < count; i++)
	//		delete[] area[i];
	//	delete[] area;
	//}
}

void do_CommandRoyalRumbleLog(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charindex;
	CLCString name;
	CLCString nick;
	int leveltype;
	RefMsg(msg) >> charindex >> name >> nick >> leveltype;

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	CLCString sql(1024);
	sql.Format("INSERT DELAYED INTO t_royalrumble_winner_log(a_char_index, a_name, a_nick, a_leveltype, a_date) VALUES (%d, '%s', '%s', %d, NOW())", charindex, (const char*)name, (const char*)nick, leveltype);
	cmd.SetQuery(sql);
	if(!cmd.Update())
	{
		GAMELOG << init("ROYAL RUMBLE WINNER LOG ERROR")
			<< charindex << delim
			<< name << delim
			<< nick << delim
			<< leveltype << end;
	}
}

#ifdef DEV_GUILD_STASH
void do_CommandGuildStashCreate( CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex = 0;
	int charindex = 0;

	RefMsg(msg) >> guildindex
		>> charindex;

	CDBCmd		cmd;
	CLCString	sql(1024);
	cmd.Init( &gserver.m_dbchar );

	sql.Format( "INSERT INTO t_guild_stash_info ( a_guild_idx, a_enable, a_limitdate, a_capacity ) VALUES ( %d, 1, NOW(), %d ); "
		,guildindex, CGuildStash::DefaultStashCapacity );
	cmd.SetQuery( sql );
	if( !cmd.Update() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashErrorMsg( rmsg, charindex, 8 );
		SEND_Q( rmsg, dest );
	}
	return;
}

//XX 길드창고 - 리스트 (2)
void do_CommandGuildStashList(CNetMsg::SP& msg, CDescriptor* dest)
{
	int guildindex	= 0;
	int	charindex	= 0;
	int subtype;

	enum { MAX_GS_LIST_ATONCE = 10, };

	msg->MoveFirst();
	RefMsg(msg) >> subtype
		>> guildindex
		>> charindex;

	CDBCmd	cmd;
	cmd.Init( &gserver.m_dbchar );

	// charIndex를 검색
	std::string sql = boost::str(boost::format(
		"SELECT gs.a_limitdate as a_limitdate, gs.a_capacity as a_capacity, gm.a_stash_auth as a_stash_auth FROM t_guild_stash_info as gs LEFT JOIN t_extend_guildmember as gm "
		" ON gs.a_enable=1 AND gs.a_guild_idx=%d AND gs.a_guild_idx = gm.a_guild_index WHERE gm.a_char_index = %d LIMIT 1") % guildindex % charindex);
	cmd.SetQuery( sql.c_str() );
	if( cmd.Open() == false || cmd.MoveFirst() == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashErrorMsg( rmsg, charindex, 6);
		SEND_Q( rmsg, dest );
		return;
	}

	int nEnable = 0;
	cmd.GetRec( "a_stash_auth", nEnable );
	if( nEnable == 0 )
	{
		 // 창고를 이용할 수 없음
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashErrorMsg( rmsg, charindex, 6 );
		SEND_Q( rmsg, dest );
		return;
	}

	int nCapacity = CGuildStash::DefaultStashCapacity;
	CLCString timeStamp(32);
	cmd.GetRec( "a_limitdate", timeStamp );
	cmd.GetRec( "a_capacity", nCapacity );

	// 돈 검색
	GoldType_t moneyCount = 0;
	sql = boost::str(boost::format("SELECT a_nas FROM t_guild_stash_info WHERE a_guild_idx = %d LIMIT 1") % guildindex);
	cmd.SetQuery( sql.c_str() );
	if( cmd.Open() && cmd.MoveFirst() )
	{
		cmd.GetRec( "a_nas" , moneyCount );
	}

	// 전송할 패킷 생성
	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildStashListMsg( rmsg , charindex );
	RefMsg(rmsg) << timeStamp
		<< nCapacity
		<< moneyCount;

	// 돈아이템을 제외한 아이템 정보
	sql = boost::str(boost::format("SELECT * FROM t_guild_stash WHERE a_guild_idx = %d LIMIT %d") % guildindex % nCapacity);
	cmd.SetQuery( sql.c_str() );
	if( cmd.Open() && cmd.MoveFirst() )
	{
		RefMsg(rmsg) << cmd.m_nrecords; // item count

		do
		{
			// 아이템 MSG 만들기
			int			stashIndex = 0;
			int			index	= 0;
			int			plus	= 0;
			int			flag	= 0;
			int			used	= -1;
			int			used2	= -1;
			LONGLONG	count	= 0;
			short		option[MAX_ITEM_OPTION];
			CLCString	socket(SOCKET_DB_LENGTH);
			int			now_dur;
			int			max_dur;

			cmd.GetRec( "a_index", stashIndex );
			cmd.GetRec( "a_item_idx" , index );
			cmd.GetRec( "a_plus" , plus );
			cmd.GetRec( "a_flag" , flag );
			cmd.GetRec( "a_used" , used );
			cmd.GetRec( "a_used_2" , used2 );
			cmd.GetRec( "a_count" , count );
			cmd.GetRec( "a_item_option0" , option[0] );
			cmd.GetRec( "a_item_option1" , option[1] );
			cmd.GetRec( "a_item_option2" , option[2] );
			cmd.GetRec( "a_item_option3" , option[3] );
			cmd.GetRec( "a_item_option4" , option[4] );
			cmd.GetRec( "a_socket" , socket );
			cmd.GetRec( "a_now_dur", now_dur);
			cmd.GetRec( "a_max_dur", max_dur);

			RefMsg(rmsg) << stashIndex
				<< index
				<< plus
				<< flag
				<< used
				<< used2
				<< now_dur
				<< max_dur
				<< count;
			for ( int idx = 0 ; idx < MAX_ITEM_OPTION ; ++idx )
				RefMsg(rmsg) << option[idx];

			RefMsg(rmsg) << socket;

			int			itemOrigin[MAX_VARIATION_COUNT];

			cmd.GetRec( "a_item_origin_0" , itemOrigin[0] );
			cmd.GetRec( "a_item_origin_1" , itemOrigin[1] );
			cmd.GetRec( "a_item_origin_2" , itemOrigin[2] );
			cmd.GetRec( "a_item_origin_3" , itemOrigin[3] );
			cmd.GetRec( "a_item_origin_4" , itemOrigin[4] );
			cmd.GetRec( "a_item_origin_5" , itemOrigin[5] );

			RefMsg(rmsg) << itemOrigin[0]
			<< itemOrigin[1]
			<< itemOrigin[2]
			<< itemOrigin[3]
			<< itemOrigin[4]
			<< itemOrigin[5];
		} while( cmd.MoveNext() );
	}
	else
	{
		RefMsg(rmsg) << 0; // itemcount = 0;
	}

	SEND_Q( rmsg, dest );
}

//XX 길드창고 - 보관 (2)
void do_CommandGuildStashKeep(CNetMsg::SP& msg, CDescriptor* dest)
{
	// 0: 성공 , 1:길드 없음 , 2:인벤부족, 3:무게초과 4: 다른 길드원이 사용중
	int guildindex	= 0;
	int charindex = 0;
	LONGLONG keepMoney = 0;
	int itemCount	= 0;

	RefMsg(msg) >> guildindex
		>> charindex
		>> keepMoney
		>> itemCount;

	CDBCmd	cmd;
	cmd.Init( &gserver.m_dbchar );
	CLCString	sql(1024);

	int nEnable = 0;
	sql.Format("SELECT gs.a_enable as a_enable, gm.a_stash_auth as a_stash_auth FROM t_guild_stash_info as gs LEFT JOIN t_extend_guildmember as gm "
		" ON gs.a_enable=1 AND gs.a_guild_idx=%d AND gs.a_guild_idx = gm.a_guild_index WHERE gm.a_char_index = %d ", guildindex, charindex );
	cmd.SetQuery( sql );
	if( cmd.Open() && cmd.MoveFirst() )
	{
		cmd.GetRec( "a_stash_auth", nEnable );
	}

	if( nEnable == 0 ) // 창고를 이용할 수 없습니다.
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashErrorMsg( rmsg, charindex, 6 );
		SEND_Q( rmsg, dest );
		return;
	}

	std::map<int,CGuildStash*>::iterator itr = gserver.m_guildstash.find( guildindex );
	if( itr != gserver.m_guildstash.end() )		// 찾았다
	{
		CGuildStash *ptmpGuildStash = itr->second ;
		if( ptmpGuildStash )
		{
			if( !ptmpGuildStash->GetLock(charindex) )
			{
				// 이미 락걸림
				CNetMsg::SP rmsg(new CNetMsg);
				HelperGuildStashKeepMsg( rmsg, charindex, guildindex, 3, 0, 0 );  // 사용중입니다.
				SEND_Q( rmsg, dest);
				return;
			}
		}
	}
	else		// 처음 로딩
	{
		CGuildStash* pGuildStash = new CGuildStash(guildindex);
		gserver.m_guildstash.insert( std::make_pair( guildindex, pGuildStash ) );
		pGuildStash->GetLock(charindex);		// 새로만들어서 100% 성공
		itr = gserver.m_guildstash.find( guildindex );
	}

	if( itr == gserver.m_guildstash.end() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashKeepMsg( rmsg, charindex, guildindex, 1, 0, 0 );  // 그런길드 없습니다.
		SEND_Q( rmsg, dest);

		return;
	}

	CGuildStash* pGuildStash = itr->second;
	if( !pGuildStash )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashKeepMsg( rmsg, charindex, guildindex, 1, 0, 0 );  // 그런길드 없습니다.
		SEND_Q( rmsg, dest);
		return; // 길드생성 실패
	}

	// 돈(NAS) 처리
	pGuildStash->setMoney(keepMoney);

	// 아이템 처리
	for (int i = 0; i < itemCount; ++i)
	{
		int			index	= 0;
		int			vindex  = 0;
		int			plus	= 0;
		int			flag	= 0;
		int			used	= -1;
		int			used2	= -1;
		LONGLONG	count	= 0;
		int			now_dur	= 0;
		int			max_dur	= 0;

		short		option[MAX_ITEM_OPTION] = {0,};
		CLCString	serial(MAX_SERIAL_LENGTH);
		CLCString	socket(SOCKET_DB_LENGTH);
		LONGLONG	proto_flag = 0;
		int			itemOrigin[MAX_VARIATION_COUNT] = {0,};

		RefMsg(msg) >> index
			>> vindex
			>> plus
			>> flag
			>> used
			>> used2
			>> now_dur
			>> max_dur
			>> count;

		for ( int idx = 0 ; idx < MAX_ITEM_OPTION ; ++idx )
			RefMsg(msg) >> option[idx];

		RefMsg(msg) >> serial
			>> socket
			>> proto_flag;
		for (int idx = 0; idx < MAX_VARIATION_COUNT; idx++) {
			RefMsg(msg) >> itemOrigin[idx];
		}
		pGuildStash->AddItem( 0, index , vindex, plus , flag, used, used2, count, option, serial, socket, itemOrigin, now_dur, max_dur );
		pGuildStash->SetItemProtoFlag( index , proto_flag );
	}

	if( !pGuildStash->CheckStashSpace(&gserver.m_dbchar) || !pGuildStash->KeepToDB(&gserver.m_dbchar) )		// 공간이 없거나 업데이트 실패
	{
		pGuildStash->RemoveItem();// 아이템 삭제
		if( !pGuildStash->UnLock(charindex) ) // Unlock 실패 Log 만찍기
		{
		}
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashKeepMsg( rmsg, charindex, guildindex, 9, 0, 0 ); // 길드 창고 공간 없음
		SEND_Q( rmsg, dest);
		return;
	}

	// 저장 완료 GameServer 로 Message 전송 ( ItemList 포함 ), 아직은 Lock 걸린 상태
	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildStashKeepMsg( rmsg, charindex, guildindex, 0, keepMoney, pGuildStash->m_listTempItem.size() );
	std::list<CStashItem*>::iterator itemItr;
	std::list<CStashItem*>::iterator itemItrEnd = pGuildStash->m_listTempItem.end();
	for ( itemItr = pGuildStash->m_listTempItem.begin() ; itemItr != itemItrEnd ; ++itemItr )
	{
		CStashItem* pItem = *itemItr;
		if( pItem )
		{
			RefMsg(rmsg) << pItem->m_serial
				<< pItem->m_count
				<< pItem->GetItemDBIndex()
				<< pItem->GetItemVIndex();
		}
	}

	SEND_Q( rmsg, dest);
}

void do_CommandGuildStashKeepError(CNetMsg::SP& msg, CDescriptor* dest)
{
	char error = -1;			// 0: 성공 , 1:실패
	int guildindex = 0;
	int charindex = 0;
	LONGLONG money = 0;
	RefMsg(msg) >> charindex
		>> guildindex
		>> error
		>> money;

	std::map<int,CGuildStash*>::iterator itr = gserver.m_guildstash.find( guildindex );
	if( itr != gserver.m_guildstash.end() )		// 찾았다
	{
		CGuildStash* pGuildStash = itr->second;
		if( !pGuildStash )
			return; // 길드생성 실패

		if( !pGuildStash->CanUnLock(charindex) )		// Lock 건사람이 아니면
			return;

		if( 0 != error )	// 창고에 저장은 했으나, 아이템 지급엔 실패
		{
			pGuildStash->TakeFromDB(&gserver.m_dbchar);		// DB 에 맡겨놨던 아이템을 temp 아이템을 기준으로 뺀따.
		}
		else
		{
			// 로그한번 남겨 봅시다.
			pGuildStash->SaveLog( &gserver.m_dbchar , CGuildStash::GUILD_STASH_KEEP, money );
		}
		// 메모리에 있는 아이템 제거
		pGuildStash->RemoveItem();// 아이템 삭제
		pGuildStash->UnLock(charindex);		//  UnLock
	}
}

//XX 길드창고 - 찾기 (2)
void do_CommandGuildStashTake(CNetMsg::SP& msg, CDescriptor* dest)
{
	// error 0: 성공 , 1:길드 없음 , 2:인벤부족, 3:무게초과 4: 다른 길드원이 사용중
	int			guildindex	= 0;
	int			charindex = 0;
	LONGLONG	takeMoney = 0;
	int			itemCount	= 0;
	int			stashIndex = 0;
	int			stashItemCount = 0;

	RefMsg(msg) >> guildindex
		>> charindex
		>> takeMoney
		>> itemCount;

	CDBCmd	cmd;
	cmd.Init( &gserver.m_dbchar );
	CLCString	sql(1024);

	int nEnable = 0;
	sql.Format("SELECT gs.a_enable as a_enable, gm.a_stash_auth as a_stash_auth FROM t_guild_stash_info as gs LEFT JOIN t_extend_guildmember as gm "
		" ON gs.a_enable=1 AND gs.a_guild_idx=%d AND gs.a_guild_idx = gm.a_guild_index WHERE gm.a_char_index = %d ", guildindex, charindex );
	cmd.SetQuery( sql );
	if( cmd.Open() && cmd.MoveFirst() )
	{
		cmd.GetRec( "a_stash_auth", nEnable );
	}

	if( nEnable == 0 ) // 창고를 이용할 수 없습니다.
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashErrorMsg( rmsg, charindex, 6 );
		SEND_Q( rmsg, dest );
		return;
	}

	std::map<int,CGuildStash*>::iterator itr = gserver.m_guildstash.find( guildindex );
	if( itr != gserver.m_guildstash.end() )		// 찾았다
	{
		CGuildStash *ptmpGuildStash = itr->second ;
		if( ptmpGuildStash )
		{
			if( !ptmpGuildStash->GetLock(charindex) )
			{
				// 이미 락걸림
				CNetMsg::SP rmsg(new CNetMsg);
				HelperGuildStashTakeMsg( rmsg, charindex, guildindex, 4, 0, 0 );  // 4: 다른길드원이 사용중입니다.
				SEND_Q( rmsg, dest);
				return;
			}
		}
	}
	else		// 처음 로딩
	{
		CGuildStash* pGuildStash = new CGuildStash(guildindex);
		gserver.m_guildstash.insert( std::make_pair( guildindex, pGuildStash ) );
		pGuildStash->GetLock(charindex);		// 새로만들어서 100% 성공
		itr = gserver.m_guildstash.find( guildindex );
	}

	if( itr == gserver.m_guildstash.end() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashTakeMsg( rmsg, charindex, guildindex, 1, 0, 0 );  // 그런길드 없습니다.
		SEND_Q( rmsg, dest);
		return;
	}

	CGuildStash* pGuildStash = itr->second;
	if( !pGuildStash )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashTakeMsg( rmsg, charindex, guildindex, 1, 0, 0 );  // 1: 그런길드 없습니다.
		SEND_Q( rmsg, dest);
		return; // 길드생성 실패
	}

	bool		bCanTake = true;
	int			itemDBIndex	= 0;
	int			plus	= 0;
	int			flag	= 0;
	int			used	= -1;
	int			used2	= -1;
	int			now_dur = 0;
	int			max_dur	= 0;
	short		option[MAX_ITEM_OPTION];
	CLCString	serial(MAX_SERIAL_LENGTH);
	CLCString	socket(SOCKET_DB_LENGTH);
	int itemOrigin[MAX_VARIATION_COUNT] = {0,};
	std::vector<int> stashIndexVerify;
	std::vector<int>::const_iterator ciVerify;

	{
		// 창고에서 돈을 뺄수 있는지 검사
		std::string str = boost::str(boost::format(
			"SELECT a_nas FROM t_guild_stash_info WHERE a_guild_idx = %1% LIMIT 1") % guildindex);
		cmd.SetQuery(str);
		if (cmd.Open() == false)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashTakeMsg( rmsg , charindex, guildindex, 5, 0, 0 ); // 5: 찾을수 없는 아이템
			SEND_Q( rmsg, dest );
			return;
		}
		
		if (cmd.MoveFirst() == false)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashTakeMsg( rmsg , charindex, guildindex, 5, 0, 0 ); // 5: 찾을수 없는 아이템
			SEND_Q( rmsg, dest );
			return;
		}

		LONGLONG curNas = 0;
		cmd.GetRec("a_nas", curNas);
		if (curNas < takeMoney)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashTakeMsg( rmsg , charindex, guildindex, 5, 0, 0 ); // 5: 찾을수 없는 아이템
			SEND_Q( rmsg, dest );
			return;
		}

		pGuildStash->setMoney(takeMoney);
	}

	// 수량 확인 과 동시에 temp 에 저장
	for( int i = 0; i < itemCount; ++i )
	{
		LONGLONG	dbCount = 0;
		RefMsg(msg) >> stashIndex >> stashItemCount;
		ciVerify = std::find(stashIndexVerify.begin(), stashIndexVerify.end(), stashIndex);
		if(ciVerify != stashIndexVerify.end())
		{
			GAMELOG << init("GUILD_STASH_WARN") << "CLIENT SEND TO STASH INDEX TWICE OR OVER, STASH INDEX : " << stashIndex << delim << "GuildIndex" << delim << guildindex << end;
		}
		else
		{
			stashIndexVerify.push_back(stashIndex);

			sql.Format("SELECT * FROM t_guild_stash WHERE a_guild_idx=%d AND a_index=%d" , guildindex, stashIndex );
			cmd.SetQuery(sql);
			if( cmd.Open() && cmd.MoveFirst() )
			{
				cmd.GetRec( "a_count", dbCount );
				if( dbCount < stashItemCount )		// 아오 갯수가 모잘라
				{
					bCanTake = false;
					break;
				}
				else
				{
					cmd.GetRec( "a_item_idx", itemDBIndex );
					cmd.GetRec( "a_serial", serial );
					cmd.GetRec( "a_plus", plus );
					cmd.GetRec( "a_flag", flag );
					cmd.GetRec( "a_used", used );
					cmd.GetRec( "a_used_2", used2 );
					cmd.GetRec( "a_item_option0", option[0] );
					cmd.GetRec( "a_item_option1", option[1] );
					cmd.GetRec( "a_item_option2", option[2] );
					cmd.GetRec( "a_item_option3", option[3] );
					cmd.GetRec( "a_item_option4", option[4] );
					cmd.GetRec( "a_socket", socket );
					cmd.GetRec("a_item_origin_0", itemOrigin[0]);
					cmd.GetRec("a_item_origin_1", itemOrigin[1]);
					cmd.GetRec("a_item_origin_2", itemOrigin[2]);
					cmd.GetRec("a_item_origin_3", itemOrigin[3]);
					cmd.GetRec("a_item_origin_4", itemOrigin[4]);
					cmd.GetRec("a_item_origin_5", itemOrigin[5]);
					cmd.GetRec("a_now_dur", now_dur);
					cmd.GetRec("a_max_dur", max_dur);
					pGuildStash->AddItem( stashIndex, itemDBIndex, 0, plus, flag, used, used2, stashItemCount, option, serial, socket, itemOrigin, now_dur, max_dur );
					if( dbCount > 1 || stashItemCount > 1 )
						pGuildStash->SetItemProtoFlag( itemDBIndex , 1);			// Count할수 있도록 플래그 설정
				}
			}
			else	// 없는 아이템이네..
			{
				bCanTake = false;
				break;
			}
		}
	}

	if (bCanTake == false)
	{
		pGuildStash->RemoveItem();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashTakeMsg( rmsg , charindex, guildindex, 5, 0, 0 ); // 5: 찾을수 없는 아이템
		SEND_Q( rmsg, dest );
		return;
	}

	if (pGuildStash->TakeFromDB(&gserver.m_dbchar) == false)
	{
		// DB 차감하다가 망했음
		pGuildStash->RemoveItem();
		pGuildStash->UnLock(charindex);		//  UnLock
		return;
	}

	// tempItem 에 있는 정보를 전달해 줍시다.
	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildStashTakeMsg( rmsg , charindex, guildindex, 0, takeMoney, pGuildStash->m_listTempItem.size() );
	std::list<CStashItem*>::iterator it =  pGuildStash->m_listTempItem.begin();
	std::list<CStashItem*>::iterator itrEnd = pGuildStash->m_listTempItem.end();
	for( ; it != itrEnd ; ++it )
	{
		CStashItem* pItem = *it;
		if( pItem )
		{
			RefMsg(rmsg) << pItem->GetItemDBIndex()
				<< pItem->m_plus
				<< pItem->m_flag
				<< pItem->m_used
				<< pItem->m_used2
				<< pItem->m_now_dur
				<< pItem->m_max_dur
				<< pItem->m_count;
			for ( int idx = 0 ; idx < MAX_ITEM_OPTION ; ++idx )
				RefMsg(rmsg) << pItem->m_option[idx];

			RefMsg(rmsg) << pItem->m_serial
				<< pItem->m_socket;
			//RefMsg(rmsg << itemOrigin[0]
			//	<< itemOrigin[1]
			//	<< itemOrigin[2]
			//	<< itemOrigin[3]
			//	<< itemOrigin[4]
			//	<< itemOrigin[5];
			RefMsg(rmsg) << pItem->m_itemOrigin[0]
			<< pItem->m_itemOrigin[1]
			<< pItem->m_itemOrigin[2]
			<< pItem->m_itemOrigin[3]
			<< pItem->m_itemOrigin[4]
			<< pItem->m_itemOrigin[5];
		}
		else
		{
			bCanTake = false;
			break; // 또 뭔가가 잘못 되었음
		}
	}

	if( !bCanTake )
	{
		// 뭔가 잘못되었다 error !!
		pGuildStash->KeepToDB(&gserver.m_dbchar); // dB 복구
		pGuildStash->RemoveItem();
		pGuildStash->UnLock(charindex);		//  UnLock
		return;
	}
	SEND_Q( rmsg, dest);		// 드디어 보냅니다.
}

//XX 길드창고 - 찾기 (4)
void do_CommandGuildStashTakeError(CNetMsg::SP& msg, CDescriptor* dest)
{
	// 실패하면 temp 에 있는 아이템 복원
	// 언락
	char error = -1;			// 0: 성공 , 1:실패
	int guildindex = 0;
	int charindex = 0;
	LONGLONG money = 0;
	RefMsg(msg) >> charindex
		>> guildindex
		>> error
		>> money;

	std::map<int,CGuildStash*>::iterator itr = gserver.m_guildstash.find( guildindex );
	if( itr != gserver.m_guildstash.end() )		// 찾았다
	{
		CGuildStash* pGuildStash = itr->second;
		if( !pGuildStash )
			return; // 길드생성 실패

		if( !pGuildStash->CanUnLock(charindex) )		// Lock 건사람이 아니면
			return;

		if( 0 != error )	// 창고에 저장은 했으나, 아이템 지급엔 실패
		{
			pGuildStash->KeepToDB(&gserver.m_dbchar);	// temp 에 있는 아이템 DB 로 저장
		}
		else
		{
			pGuildStash->SaveLog( &gserver.m_dbchar , CGuildStash::GUILD_STASH_TAKE, money );
		}
		// 메모리에 있는 아이템 제거
		pGuildStash->RemoveItem();// 아이템 삭제
		pGuildStash->UnLock(charindex);		//  UnLock
	}
}

void do_CommandGuildStashLog(CNetMsg::SP& msg, CDescriptor* dest)
{
	//10개 단위로 쪼개서 보냅시다.
	enum { MAX_GS_LOGLIST_ATONCE = 10, };

	int guildindex	= 0;
	int	charindex	= 0;
	int lastlog		= 0;

	RefMsg(msg) >> guildindex
		>> charindex
		>> lastlog;

	CDBCmd		cmd;
	CLCString	sql(1024);
	cmd.Init( &gserver.m_dbchar );

	int nEnable = 0;
	sql.Format("SELECT gs.a_enable as a_enable, gm.a_stash_auth as a_stash_auth FROM t_guild_stash_info as gs LEFT JOIN t_extend_guildmember as gm "
		" ON gs.a_enable=1 AND gs.a_guild_idx=%d AND gs.a_guild_idx = gm.a_guild_index WHERE gm.a_char_index = %d ", guildindex, charindex );
	cmd.SetQuery( sql );
	if( cmd.Open() && cmd.MoveFirst() )
	{
		cmd.GetRec( "a_stash_auth", nEnable );
	}

	if( nEnable == 0 ) // 창고를 이용할 수 없습니다.
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashErrorMsg( rmsg, charindex, 6 );
		SEND_Q( rmsg, dest );
		return;
	}

	// lastlog index > 0 ?
	if( lastlog == 0 )
		sql.Format( "SELECT l.a_index as a_index, l.a_date as a_date, c.a_nick as a_nick, l.a_state as a_state, l.a_item_idx as a_item_idx, l.a_plus as a_plus, l.a_count as a_count, l.a_nas as a_nas"
			" FROM t_guild_stash_log AS l LEFT JOIN t_characters AS c ON l.a_char_idx = c.a_index "
			" WHERE l.a_guild_idx = %d ORDER BY l.a_date DESC, l.a_index DESC limit %d " , guildindex , MAX_GS_LOGLIST_ATONCE);
	else
		sql.Format( "SELECT l.a_index as a_index, l.a_date as a_date, c.a_nick as a_nick, l.a_state as a_state, l.a_item_idx as a_item_idx, l.a_plus as a_plus, l.a_count as a_count, l.a_nas as a_nas"
			" FROM t_guild_stash_log AS l LEFT JOIN t_characters AS c ON l.a_char_idx = c.a_index "
			" WHERE l.a_guild_idx = %d AND l.a_index < %d ORDER BY l.a_date DESC, l.a_index DESC limit %d " , guildindex , lastlog, MAX_GS_LOGLIST_ATONCE );
	cmd.SetQuery(sql);
	if( cmd.Open() && cmd.MoveFirst() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashLogMsg( rmsg, charindex, guildindex, cmd.m_nrecords );

		CLCString	nick(45);
		CLCString	date(32);
		int logindex = 0;
		char		cState = 0;
		int			itemIndex = 0;
		int			itemPlus = 0;
		LONGLONG	count = 0;
		LONGLONG	money = 0;

		do {
			cmd.GetRec("a_index" , logindex );
			cmd.GetRec("a_date" , date );
			cmd.GetRec("a_nick" , nick );
			cmd.GetRec("a_state", cState );
			cmd.GetRec("a_item_idx", itemIndex );
			cmd.GetRec("a_plus", itemPlus );
			cmd.GetRec("a_count", count );
			cmd.GetRec("a_nas", money );

			if( itemIndex == -1 && money > 0)
			{
				RefMsg(rmsg) << logindex << date << nick << cState << NAS_ITEM_DB_INDEX << itemPlus << money;
			}
			else
			{
				RefMsg(rmsg) << logindex << date << nick << cState << itemIndex << itemPlus << count;
			}			
		} while ( cmd.MoveNext() );

		SEND_Q(rmsg, dest);
	}
}

#endif // DEV_GUILD_STASH

#ifdef GUILD_MEMBER_KICK_JOIN_DELAY
void do_CommandGuildMemberKickOutDateUpdate(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charindex;
	int outdate;

	RefMsg(msg) >> charindex
		>> outdate;

	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	sql.Format("update t_characters set a_guildoutdate=%d where a_index=%d", outdate, charindex );
	cmd.SetQuery(sql);
	if( !cmd.Update() )
	{
		GAMELOG << init("GUILD KICK OUTDATE DB UPDATE FAIL")
				<< "CharIndex : " << charindex << delim
				<< " GuildoutDate : " << outdate << end;
	}
}
#endif

void do_CommandTeacherStudentListCync(CNetMsg::SP& msg, CDescriptor* dest)
{
	CLCString idname(MAX_ID_NAME_LENGTH + 1);
	int charindex;

	int		m_guardian;
	int		m_superstone;
	int		m_cntFailStudent;
	int		m_cntCompleteStudent;
	char	m_teachType;
	int		m_fame;
	char	bteacher;
	CLCString strTeachIdx(256);
	CLCString strTeachSec(256);
	int subtype;

	msg->MoveFirst();
	RefMsg(msg) >> subtype;

	RefMsg(msg) >> idname
		>> charindex;

	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);

	char select_characters_query[8192] = {0,};
	sprintf(select_characters_query, "select a_teach_idx, a_teach_sec, a_teach_type, a_fame, a_teach_list, a_teach_complete, a_teach_fail, a_superstone, a_guardian "
		" from t_characters where a_index=%d ", charindex);
	dbChar.SetQuery(select_characters_query);

	if( !dbChar.Open() || !dbChar.MoveFirst() )
		return;

	dbChar.GetRec("a_teach_type",	m_teachType);
	dbChar.GetRec("a_fame",			m_fame);
	dbChar.GetRec("a_teach_list",	bteacher);
	dbChar.GetRec("a_teach_complete", m_cntCompleteStudent);
	dbChar.GetRec("a_teach_fail",	  m_cntFailStudent);
	dbChar.GetRec("a_superstone",	  m_superstone);
	dbChar.GetRec("a_guardian",		  m_guardian);
	dbChar.GetRec("a_teach_idx", strTeachIdx);
	dbChar.GetRec("a_teach_sec", strTeachSec);

	if( m_teachType == MSG_TEACH_TEACHER_TYPE )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperTeacherInfo(rmsg, idname, charindex, m_teachType, m_fame, bteacher, m_cntCompleteStudent,
			m_cntFailStudent, m_superstone, m_guardian, strTeachIdx, strTeachSec, MSG_HELPER_TEACHER_STUDENT_LIST_CYNC_REP);
		SEND_Q(rmsg, dest);
	}
}

void do_CommandGuildMasterKickReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int _guildIndex = 0, _requestChar = 0;
	RefMsg(msg) >> _guildIndex
		>> _requestChar;

	// 받은 데이터 검사
	if ( _guildIndex <= 0 || _requestChar <= 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickRep(rmsg, _guildIndex, _requestChar, MSG_NEW_GUILD_ERROR_MASTER_KICK_DATA);
		SEND_Q(rmsg, dest);
		return;
	}

	// 길드 검사
	CGuild* _guild = gserver.m_guildlist.findguild(_guildIndex);
	if ( !_guild )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickRep(rmsg, _guildIndex, _requestChar, MSG_NEW_GUILD_ERROR_MASTER_KICK_DATA);
		SEND_Q(rmsg, dest);
		return;
	}

	// 길드 상태 검사
	if ( _guild->getGuildKick()->getKickStatus() != GMKS_NORMAL )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickRep(rmsg, _guildIndex, _requestChar, MSG_NEW_GUILD_ERROR_MASTER_KICK_RESERVE);
		SEND_Q(rmsg, dest);
		return;
	}

	time_t t = time(NULL);
	CLCString kickReserveQuery(258);
	kickReserveQuery.Format("UPDATE t_extend_guild SET a_kick_status = %d, a_kick_request_char_index = %d, a_kick_request_time = %d "
		"WHERE a_guild_index = %d", GMKS_RESERVE, _requestChar, (int)t, _guildIndex);

	CDBCmd kickReserveCmd;
	kickReserveCmd.Init(&gserver.m_dbchar);
	kickReserveCmd.SetQuery(kickReserveQuery);
	if ( !kickReserveCmd.Update() )
	{
		GAMELOG << init("GUILD MASTER KICK")
				<< "RESERVE ERROR";
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickRep(rmsg, _guildIndex, _requestChar, MSG_NEW_GUILD_ERROR_MASTER_KICK_DATA);
		SEND_Q(rmsg, dest);

		return;
	}

	// 길드 상태 변경
	do_GuildStatusChange(_guildIndex, GMKS_RESERVE);

	// 길드마스터 추방 관련 데이터 설정
	_guild->getGuildKick()->setKickRequestTime(t);
	_guild->getGuildKick()->setKickReuestChar(_requestChar);

	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildMasterKickRep(rmsg, _guildIndex, _requestChar, 0, t);
	SEND_Q(rmsg, dest);
}

void do_CommandGuildMasterKickCancelReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int _guildIndex = 0, _requestChar = 0;
	RefMsg(msg) >> _guildIndex
		>> _requestChar;

	// 받은 데이터 검사
	if ( _guildIndex <= 0 || _requestChar <= 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickCancelRep(rmsg, _guildIndex, _requestChar, MSG_NEW_GUILD_ERROR_MASTER_KICK_DATA);
		SEND_Q(rmsg, dest);
		return;
	}

	// 길드 검사
	CGuild* _guild = gserver.m_guildlist.findguild(_guildIndex);
	if ( !_guild )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickCancelRep(rmsg, _guildIndex, _requestChar, MSG_NEW_GUILD_ERROR_MASTER_KICK_DATA);
		SEND_Q(rmsg, dest);
		return;
	}

	// 길드 상태 검사
	if ( _guild->getGuildKick()->getKickStatus() != GMKS_RESERVE )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickCancelRep(rmsg, _guildIndex, _requestChar, MSG_NEW_GUILD_ERROR_MASTER_KICK_CANCEL_NOT_RESERVE);
		SEND_Q(rmsg, dest);
		return;
	}

	time_t t = time(NULL);
	CLCString kickReserveQuery(256);
	kickReserveQuery.Format("UPDATE t_extend_guild SET a_kick_status = %d, a_kick_request_char_index = %d, a_kick_request_time = %d "
		"WHERE a_guild_index = %d", GMKS_NORMAL, _requestChar, (int)t, _guildIndex);

	CDBCmd kickReserveCmd;
	kickReserveCmd.Init(&gserver.m_dbchar);
	kickReserveCmd.SetQuery(kickReserveQuery);
	if ( !kickReserveCmd.Update() )
	{
		GAMELOG << init("GUILD MASTER KICK")
				<< "RESERVE CANCEL ERROR";
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickCancelRep(rmsg, _guildIndex, _requestChar, MSG_NEW_GUILD_ERROR_MASTER_KICK_DATA);
		SEND_Q(rmsg, dest);
		return;
	}

	// 길드 상태 변경
	do_GuildStatusChange(_guildIndex, GMKS_NORMAL);

	// 길드마스터 추방 관련 데이터 설정
	_guild->getGuildKick()->setKickRequestTime(t);
	_guild->getGuildKick()->setKickReuestChar(_requestChar);

	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildMasterKickCancelRep(rmsg, _guildIndex, _requestChar, 0);
	SEND_Q(rmsg, dest);
}

void do_CommandGuildMasterKickReset(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charIndex = 0, guildIndex = 0;
	const int RESET_SUCCESS = 1;
	const int RESET_FAIL = 0;

	RefMsg(msg) >> charIndex
		>> guildIndex;

	if ( charIndex <= 0 || guildIndex <= 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickReset(rmsg, charIndex, RESET_FAIL);
		SEND_Q(rmsg, dest);
		return;
	}

	CGuild* pGuild = gserver.m_guildlist.findguild(guildIndex);
	if (pGuild == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickReset(rmsg, charIndex, RESET_FAIL);
		SEND_Q(rmsg, dest);
		return;
	}

	if ( pGuild->boss()->charindex() == charIndex )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickReset(rmsg, charIndex, RESET_FAIL);
		SEND_Q(rmsg, dest);
		return;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.BeginTrans();

	CLCString message(512);
	message.Format("UPDATE t_extend_guild SET a_lastdate_change_boss = 0, a_kick_status = 0 WHERE a_guild_index = %d", guildIndex);
	cmd.SetQuery(message);
	if ( !cmd.Update() )
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickReset(rmsg, charIndex, RESET_FAIL);
		SEND_Q(rmsg, dest);
		return;
	}

	message.Format("UPDATE t_characters SET a_datestamp = DATE_SUB(NOW(), INTERVAL 7 DAY) WHERE a_index = %d", pGuild->boss()->charindex());
	cmd.SetQuery(message);
	if ( !cmd.Update() )
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickReset(rmsg, charIndex, RESET_FAIL);
		SEND_Q(rmsg, dest);
		return;
	}

	pGuild->getGuildKick()->setKickStatus(GMKS_NORMAL);
	HelperGuildMasterKickStatus(msg, guildIndex, GMKS_NORMAL);
	gserver.SendToAllGameServer(msg);

	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildMasterKickReset(rmsg, charIndex, RESET_SUCCESS);
	SEND_Q(rmsg, dest);
	cmd.Commit();
}

void  do_CommandGuildMasterChange(CNetMsg::SP& msg, CDescriptor* dest)
{
	int newBossIndex = -1, guildIndex = -1;

	RefMsg(msg) >> newBossIndex
		>> guildIndex;

	if ( do_GuildBossChange(guildIndex, newBossIndex) )
	{
		do_GuildStatusChange(guildIndex, GMKS_NORMAL);
	}
}

void ExecQuery(const char* p)
{
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(p);
	if(!cmd.Update())
		GAMELOG << init("DB ERROR") << p << end;
}

void do_CommandEventTableInit(CNetMsg::SP& msg, CDescriptor* dest)
{
	int eventIndex;
	RefMsg(msg) >> eventIndex;

	char sql[1024] = {0,};
	switch(eventIndex)
	{
	case A_EVENT_BEAR_DOLL:
		sprintf(sql, "DELETE FROM t_event_gomdori_2007");
		ExecQuery(sql);
		break;
	case A_EVENT_HALLOWEEN:
		sprintf(sql, "DELETE FROM t_event_halloween2007");
		ExecQuery(sql);
		break;
	case A_EVENT_XMAS:
		sprintf(sql, "UPDATE t_pet SET a_pet_turnto_npc = 0 WHERE a_pet_turnto_npc = 485");
		ExecQuery(sql);
		break;
	default:
		break;
	}
}

//RVR
void do_CommandRVRIncreaseJewel(CNetMsg::SP& msg)
{
	UpdateServer::SyndicateJewelCount* p = reinterpret_cast<UpdateServer::SyndicateJewelCount*>(msg->m_buf);
	gserver.m_syndicate.increaseJewelPoint(p->syndicateType, p->count);
}

void do_CommandRVRDecreaseJewel(CNetMsg::SP& msg)
{
	UpdateServer::SyndicateJewelCount* p = reinterpret_cast<UpdateServer::SyndicateJewelCount*>(msg->m_buf);
	gserver.m_syndicate.decreaseJewelPoint(p->syndicateType, p->count);
}

void do_CommandRVRJoinReq(CNetMsg::SP& msg)
{
	UpdateServer::SyndicateUser* p = reinterpret_cast<UpdateServer::SyndicateUser*>(msg->m_buf);
	gserver.m_syndicate.sydicateJoin(p->syndicateType);
}

void do_CommandRVRLeaveReq(CNetMsg::SP& msg)
{
	UpdateServer::SyndicateUser* p = reinterpret_cast<UpdateServer::SyndicateUser*>(msg->m_buf);
	gserver.m_syndicate.sydicateLeave(p->syndicateType);
}

void do_CommandRVRChangeKingInfo(CNetMsg::SP& msg)
{
	UpdateServer::SyndicateUpdateKingInfoAll* p = reinterpret_cast<UpdateServer::SyndicateUpdateKingInfoAll*>(msg->m_buf);
	
	gserver.m_syndicate.setKingIndex(SYNDICATE::eSYNDICATE_DEALERMOON, p->kingIndex_d);
	gserver.m_syndicate.setKingName(SYNDICATE::eSYNDICATE_DEALERMOON, p->kingName_d);
	gserver.m_syndicate.setKingPoint(SYNDICATE::eSYNDICATE_DEALERMOON, p->kingPoint_d);

	gserver.m_syndicate.setKingIndex(SYNDICATE::eSYNDICATE_KAILUX, p->kingIndex_k);
	gserver.m_syndicate.setKingName(SYNDICATE::eSYNDICATE_KAILUX, p->kingName_k);
	gserver.m_syndicate.setKingPoint(SYNDICATE::eSYNDICATE_KAILUX, p->kingPoint_k);

	gserver.m_syndicate.sendKingInfo();
}

void do_CommandRVRChangeKingInfoEach(CNetMsg::SP& msg)
{
	UpdateServer::SyndicateUpdateKingInfoEach* p = reinterpret_cast<UpdateServer::SyndicateUpdateKingInfoEach*>(msg->m_buf);

	gserver.m_syndicate.setKingData(p->syndicateType, p->kingIndex, p->kingPoint, p->kingName);
	gserver.m_syndicate.sendKingInfo();
}

void do_CommandRVRAdd2ndGradeUser(CNetMsg::SP& msg)
{
	UpdateServer::SyndicateAdd2ndGradeUser* p = reinterpret_cast<UpdateServer::SyndicateAdd2ndGradeUser*>(msg->m_buf);
	Syndicate2ndUser user;

	user.charIndex = p->charIndex;
	user.point = p->point;
	user.isKing = true;
	strncpy(user.userName, p->userName, MAX_CHAR_NAME_LENGTH);
	user.userName[MAX_CHAR_NAME_LENGTH - 1] = '\0';

	gserver.m_syndicate.add2ndUser(user, p->syndicateType);

	if( gserver.m_syndicate.checkKing(p->syndicateType, p->point) == true )
	{
		gserver.m_syndicate.setKingData(p->syndicateType, p->charIndex, p->point, p->userName);
		gserver.m_syndicate.sendKingInfo();
	}
}

void do_CommandRVRDel2ndGradeUser(CNetMsg::SP& msg)
{
	UpdateServer::SyndicateDel2ndGradeUser* p = reinterpret_cast<UpdateServer::SyndicateDel2ndGradeUser*>(msg->m_buf);
	gserver.m_syndicate.del2ndUser(p->charIndex, p->syndicateType);
}

void do_CommandRVRUpdate2ndGradeUser(CNetMsg::SP& msg)
{
	UpdateServer::SyndicateUpdate2ndGradeUser* p = reinterpret_cast<UpdateServer::SyndicateUpdate2ndGradeUser*>(msg->m_buf);

	std::vector<Syndicate2ndUser>* vec_ = gserver.m_syndicate.find2ndInfo(p->syndicateType);
	
	std::vector<Syndicate2ndUser>::iterator it_f = vec_->begin();
	std::vector<Syndicate2ndUser>::iterator it_f_end = vec_->end();

	
	int downGrade = 0;
	if(p->syndicateType == SYNDICATE::eSYNDICATE_KAILUX)
	{
		downGrade = SYNDICATE::DUKA;
	}
	else if(p->syndicateType == SYNDICATE::eSYNDICATE_DEALERMOON)
	{
		downGrade = SYNDICATE::MAGUS;
	}

	for(; it_f != it_f_end; it_f++)
	{
		//캐릭터를 찾고
		if(it_f->charIndex == p->charIndex)
		{
			//찾은 캐릭터가 왕인 경우
			if(it_f->isKing == true)
			{
				//기존에 갖고 있는 포인트보다 증가하였으면
				if(it_f->point < p->point)
				{
					//포인트를 세팅
					it_f->point = p->point;
					gserver.m_syndicate.setKingPoint(p->syndicateType, it_f->point);
				}
				//감소하였으면
				else
				{
					//나보다 많은 포인트를 갖고 있는 캐릭터가 있는지 검사
					std::vector<Syndicate2ndUser>::iterator it = vec_->begin();
					std::vector<Syndicate2ndUser>::iterator it_end = vec_->end();

					std::vector<Syndicate2ndUser>::iterator secound_it;

					secound_it = it;

					for(; it != it_end; it++)
					{
						if(secound_it->point < it->point)
							secound_it = it;
					}

					//있는 경우
					if(secound_it->charIndex != it_f->charIndex)
					{
						//기존 왕은 등급 다운 시켜라라는 패킷을 전달한다.
						it_f->isKing = false;
						gserver.m_syndicate.sendDownGrade(p->syndicateType, it_f->charIndex, downGrade, it_f->point);

						//왕 정보 갱신
						gserver.m_syndicate.setKingData(p->syndicateType, secound_it->charIndex, secound_it->point, secound_it->userName);
						secound_it->isKing = true;
						gserver.m_syndicate.sendKingInfo();
					}
					//없는 경우
					else
					{
						//등급 다운한 캐릭터의 등급이 공작이 아닌경우에는 제거
						int grade = SYNDICATE::getSyndicateGrade(p->syndicateType, p->point);
						if( (p->syndicateType == SYNDICATE::eSYNDICATE_DEALERMOON && grade < SYNDICATE::MAGUS) ||
							(p->syndicateType == SYNDICATE::eSYNDICATE_KAILUX && grade < SYNDICATE::DUKA) )
						{
							gserver.m_syndicate.sendDownGrade(p->syndicateType, it_f->charIndex, grade, p->point);
							//공작 리스트에서 제거
							vec_->erase(it_f);
							gserver.m_syndicate.setKingData(p->syndicateType, 0, 0, NULL);
							gserver.m_syndicate.sendKingInfo();
							return ;
						}
						else
						{
							//포인트 업데이트
							it_f->point = p->point;
							gserver.m_syndicate.setKingPoint(p->syndicateType, it_f->point);
						}						
					}
				}
			}
			//찾은 캐릭터가 왕이 아닌 경우
			else
			{
				//내 포인트가 왕보다 많다
				if(gserver.m_syndicate.checkKing(p->syndicateType, p->point) == true)
				{
					std::vector<Syndicate2ndUser>::iterator it = vec_->begin();
					std::vector<Syndicate2ndUser>::iterator it_end = vec_->end();

					for(; it != it_end; it++)
					{
						//왕을 찾고
						if(it->charIndex == gserver.m_syndicate.getKingIndex(p->syndicateType))
						{
							//등급 다운 시켜라라는 패킷을 전달한다.
							it->isKing = false;
							gserver.m_syndicate.sendDownGrade(p->syndicateType, it->charIndex, downGrade, it->point);
							break;
						}
					}

					//왕 정보 갱신
					gserver.m_syndicate.setKingData(p->syndicateType, p->charIndex, p->point, it_f->userName);				
					it_f->isKing = true;
					gserver.m_syndicate.sendKingInfo();
				}
				//적다
				else
				{
					//내 포인트 정보만 업데이트 시켜준다.
					it_f->point = p->point;
				}
			}
		}
	}
}

void do_CommandRVRSetUserCount(CNetMsg::SP& msg)
{
	UpdateServer::SyndicateUserCount* packet = reinterpret_cast<UpdateServer::SyndicateUserCount*>(msg->m_buf);
	int type = packet->syndicateType;
	int count = packet->userCount;

	gserver.m_syndicate.setUserCount(type, count);
	gserver.m_syndicate.sendInfo();
}
//RVR
