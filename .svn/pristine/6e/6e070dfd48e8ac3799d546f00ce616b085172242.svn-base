#ifndef __CMDMSG_H__
#define __CMDMSG_H__

#include "Descriptor.h"

void FailMsg(CNetMsg::SP& msg, MSG_FAIL_TYPE failtype);

void HelperWhisperNotfoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sidx, const char* sname);
void HelperWhisperRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sidx, const char* sname, const char* rname, const char* chat);
void HelperLogoutRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, const char* id);

void HelperGuildCreateRepMsg(CNetMsg::SP& msg, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildCreateNotifyMsg(CNetMsg::SP& msg, CGuild* guild);
void HelperGuildOnlineNotifyMsg(CNetMsg::SP& msg, CGuildMember* member);
void HelperGuildLoadRepMsg(CNetMsg::SP& msg, const char* idname, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildLevelUpRepMsg(CNetMsg::SP& msg, int guildindex, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildLevelUpNotifyMsg(CNetMsg::SP& msg, int guildindex, int level);
void HelperGuildBreakUpRepMsg(CNetMsg::SP& msg, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildBreakUpNotifyMsg(CNetMsg::SP& msg, int guildindex);
void HelperGuildMemberAddRepMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildMemberAddNotifyMsg(CNetMsg::SP& msg, int guildindex, int charindex, const char* name, MSG_GUILD_POSITION_TYPE pos);
void HelperGuildMemberOutRepMsg(CNetMsg::SP& msg, int guildindex, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildMemberOutNotifyMsg(CNetMsg::SP& msg, int guildindex, int charindex, const char* charname);
void HelperGuildMemberKickRepMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildMemberKickNotifyMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex);
void HelperGuildChangeBossRepMsg(CNetMsg::SP& msg, int guildindex, int current, int change, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildChangeBossNotifyMsg(CNetMsg::SP& msg, int guildindex, int current, int change);
void HelperGuildAppointOfficerRepMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildAppointOfficerNotifyMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex);
void HelperGuildFireOfficerRepMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode);
void HelperGuildFireOfficerNotifyMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex);
void HelperGuildLoadNotifyMsg(CNetMsg::SP& msg, CGuild* guild);

void HelperGuildBattleRepMsg(CNetMsg::SP& msg, CGuild* g1, CGuild* g2, int prize);
void HelperGuildBattleStartMsg(CNetMsg::SP& msg, CGuild* g1, CGuild* g2);
void HelperGuildBattleStopRepMsg(CNetMsg::SP& msg, int winner_index, CGuild* g1, CGuild* g2);
void HelperGuildBattleStatusMsg(CNetMsg::SP& msg, CGuild* g1, CGuild* g2);
void HelperGuildBattlePeaceRepMsg(CNetMsg::SP& msg, CGuild* g);

#ifdef ZONE_LIMITAIONS
void HelperGuildCommandBattleReqMsg(CNetMsg::SP& msg, CGuild* g1, CGuild* g2);
void HelperGuildCommandBattleStartMsg(CNetMsg::SP& msg, CGuild* g1, CGuild* g2);
void HelperGuildCommandBattleStopMsg(CNetMsg::SP& msg, int winner_index, CGuild* g1, CGuild* g2);
#endif

void HelperEventMoonStoneLoadMsg(CNetMsg::SP& mmsg, int moonstone);
void HelperEventMoonStoneUpdateRepMsg(CNetMsg::SP& msg, int moonstone);
void HelperEventMoonStoneJackPotRepMsg(CNetMsg::SP& msg, int moonstone, int chaindex);

void HelperFriendSetConditionNotifyMsg(CNetMsg::SP& msg, int chaindex, int condition, int reply, int sum, const int* index);
//MSG_HELPER_NAME_CHANGE_REP,	// 이름 변경카드 사용 : bguild(c) charindex(n) nickname(str) errorcode(c)
void HelperNameChangeRepMsg(CNetMsg::SP& msg, char bguild, int charindex, const char* name, MSG_EX_NAMECHANGE_ERROR_TYPE error);
void HelperBlockCharRepMsg(CNetMsg::SP& msg, int reqindex, int blockIndex, const char* blockName );

//MSG_HELPER_GIFT_RECVCHARNAME_REP,	//: sendCharIndex(n) recvUserIdx(n) recvCharIndex(n) revCharName(str) msg(str) count(n) idx(n) ctid(n)
void HelperGiftCharRepMsg(CNetMsg::SP& msg, int sendUserIdx, int sendCharIdx, int recvUserIdx, int recvCharIdx, const char* recvCharName, const char* sendMsg, int count, int idx[], int ctid[] );

// 애완동물
void HelperPetCreateRepMsg(CNetMsg::SP& msg, int index, int owner, char typegrade);
void HelperPetDeleteRepMsg(CNetMsg::SP& msg, int index, int owner);

// 길드 나스 보관함
void HelperGuildStashHistoryRepMsg(CNetMsg::SP& msg, int charindex, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, int month[7], int day[7], LONGLONG money[7]);
void HelperGuildStashViewRepMsg(CNetMsg::SP& msg, int charindex, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, LONGLONG money);
void HelperGuildStashTakeRepMsg(CNetMsg::SP& msg, int guildindex, int charindex, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, LONGLONG money, LONGLONG balance);
void HelperGuildStashSaveTaxRepMsg(CNetMsg::SP& msg, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, int guildindex, int zoneindex, LONGLONG taxItem, LONGLONG taxProduce);

void HelperFameupRepMsg(CNetMsg::SP& msg, int teachidx, int studentidx, const char* studentname, int fameup);
void HelperTeacherInfo(CNetMsg::SP& msg, const char* idname, int charindex, char m_teachType, int m_fame, char bteacher, int m_cntCompleteStudent,
					   int m_cntFailStudent, int m_superstone, int m_guardian, const char* strTeachIdx, const char* strTeachSec, int msgSubType = MSG_HELPER_TEACHER_LOAD_REP );

void HelperTeachGiftRepMsg( CNetMsg::SP& msg, MSG_HELPER_TEACH_RENEWER_GIFT_ERROR_TYPE type, int charindex, int count = -1 );

//등록한 케릭을 상태를 GameServer로 보냄.
void HelperEventSearchFriendMemberAddRepMsg(CNetMsg::SP& msg, int approvalindex, const char reqnick[MAX_CHAR_NAME_LENGTH + 1],  MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode);
//휴면케릭이 리스트에서 선택한 결과 상태를 GameServer로 보냄.
void HelperEventSearchFriendMemberSelectAddRepMsg(CNetMsg::SP& msg, int approvalindex,MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode);
//휴면케릭이 한시간 단위로 사냥한 시간을 저장하고 상태를 GameServer로 보냄.
void HelperEventSearchFriendMemberOneTimeCheckRepMsg(CNetMsg::SP& msg, int timesec, int appDormantindex, MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode);
//휴면케릭 이벤트 등록된 친구 보상가능 여부 데이터를 GameServer로 보냄.
void HelperEventSearchFriendMemberListGoodRepMsg(CNetMsg::SP& msg, int approvalindex, int* requestindex, const char requestnick[][MAX_CHAR_NAME_LENGTH + 1], int* itemgood, int request_listmember, int startindex, int nTotal);
//휴면케릭 보상지급 후 결과를 GameServer로 보냄.
void HelperEventSearchFriendMemberGoodRepMsg(CNetMsg::SP& msg, int approvalindex, MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TYPE errcode);

void HelperPartyInviteReqMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nTargetIndex, char nPartyType);

void HelperPartyInviteRepMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nTargetIndex, const char* strTargetName, char cPartyType1, char cPartyType2, char cPartyType3, MSG_HELPER_PARTY_ERROR_TYPE errcode);

void HelperPartyAllowRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, const char* strTargetName, MSG_HELPER_PARTY_ERROR_TYPE errcode);
void HelperPartyRejectRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, MSG_HELPER_PARTY_ERROR_TYPE errcode);
void HelperPartyQuitRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, MSG_HELPER_PARTY_ERROR_TYPE errcode, int nNewBossIndex, const char* strNewBossName);
void HelperPartyKickRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, MSG_HELPER_PARTY_ERROR_TYPE errcode);
void HelperPartyChangeBossRepMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nNewBossIndex, const char* strNewBossName);

void HelperPartyChangeTypeRepMsg(CNetMsg::SP& msg, int nBossIndex, char cPartyType, char cDiviType, char cAllOneSet);
void HelperPartyEndPartyRepMsg(CNetMsg::SP& msg, int nBossIndex);

void HelperPartyMatchRegMemberRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nCharIndex, const char* strCharName, int nCharLevel, int nZone, char cJob, char cPartyType);
void HelperPartyMatchRegPartyRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, const char* strBossName, int nBossLevel, int nZone, int nMemberCount, int nJobFlag, char cLimitLevel, char cPartyType, const char* strComment);
void HelperPartyMatchDelRepMsg(CNetMsg::SP& msg, int nCharIndex, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode);
void HelperPartyMatchInviteRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName, char cPartyType);
void HelperPartyMatchJoinRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, char cPartyType, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName, char cCharJob);
void HelperPartyMatchJoinAllowRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, int nCharIndex, const char* strCharName);
void HelperPartyMatchJoinRejectRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nJoinCharIndex, int nRejectCharIndex);
void HelperPartyMatchMemberChangeInfoMsg(CNetMsg::SP& msg, int nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_TYPE nType, const char* strCharName, int nLevel, int nZone);

void HelperPartyInfoEndMsg(CNetMsg::SP& msg);

void HelperGuildInclineEstablishRepMsg( CNetMsg::SP& msg, int guildindex, int charindex, char guildincline, MSG_GUILD_ERROR_TYPE errcode );
void HelperGuildMemberAdjustRepMsg( CNetMsg::SP& msg, int guildindex, int ownerindex, int charindex, const char* strPositionName, int contributeExp, int contributeFame, int pos, MSG_GUILD_ERROR_TYPE errcode );
void HelperNewGuildInfoRepMsg( CNetMsg::SP& msg, int charindex, MSG_GUILD_ERROR_TYPE errorcode );
void HelperNewGuildInfoNotifyMsg( CNetMsg::SP& msg, int charindex, int guildindex, int avelevel, int usepoint, MSG_GUILD_ERROR_TYPE errorcode  );
void HelperNewGuildMemberListRepMsg( CNetMsg::SP& msg, int endcount, int & guildstart, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode,  int* membercharindex, int* cumulatePoint, const char CharName[][MAX_CHAR_NAME_LENGTH  + 1], const char positionName[][GUILD_POSITION_NAME+1], char* job, char* job2, int* level, int* position );
#ifdef DEV_GUILD_STASH
void HelperNewGuildManageRepMsg( CNetMsg::SP& msg, int endcount, int & guildstart, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode,  int* membercharindex, int* contributeExp, int* contributeFame, const char CharName[][MAX_CHAR_NAME_LENGTH + 1], const char positionName[][GUILD_POSITION_NAME+1], int* level, int* position, char* stashAuth, char first);
#else
void HelperNewGuildManageRepMsg( CNetMsg::SP& msg, int endcount, int & guildstart, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode,  int* membercharindex, int* contributeExp, int* contributeFame, const char CharName[][MAX_CHAR_NAME_LENGTH  + 1], const char positionName[][GUILD_POSITION_NAME+1], int* level, int* position, char first );
#endif //DEV_GUILD_STASH

void HelperNewGuildNoticeRepMsg( CNetMsg::SP& msg, int charindex, int guildindex, const char* title, const char* text);
void HelperNewGuildNoticeUpdateRepMsg( CNetMsg::SP& msg, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode );
void HelperNewGuildNoticeTransMsg( CNetMsg::SP& msg, int guildindex, const char* title, const char* text );
void HelperNewGuildNotice( CNetMsg::SP& msg, int charindex, int guildindex, const char* title, const char* text );
void HelperNewGuildNoticeTransRep( CNetMsg::SP& msg, int guildindex, int charindex, MSG_GUILD_ERROR_TYPE errcode );
void HelperExtendGuildLoadNotifyMsg( CNetMsg::SP& msg, CGuild* guild, int skillcount, int* skillindex, int* skillLevel, int skilltype );
void HelperNewGuildSkillListRepMsg( CNetMsg::SP& msg, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode, int Activeskillcount, int* Activeskillindex, int* ActiveskillLevel, int Passiveskillcount, int* Passiveskillindex, int* PassiveskillLevel, int Etcskillcount, int* Etcskillindex, int* EtcskillLevel );
void HelperNewGuildPointUpdateMsg( CNetMsg::SP& msg, int charindex, int guildindex, int guildpoint );
void HelperNewGuildMemberPointSaveMsg(CNetMsg::SP& msg, int charindex, int guildindex, int memberpoint);

void HelperPetNameChange( CNetMsg::SP& msg, MSG_EX_PET_CHANGE_NAME_ERROR_TYPE err,int charidx, int petidx = 0, const char* strPetName = NULL );
void HelperGuildMarkTableMsg(CNetMsg::SP& msg);

void HelperHalloween2007Msg( CNetMsg::SP& msg, int charIndex, unsigned char error );

void HelperDVDRateChangeMsg( CNetMsg::SP& msg, unsigned char subtype, int nRate );

void HelperAttackPet( CNetMsg::SP& msg, MSG_HELPER_APET_TYPE type, int charIndex );

void HelperEventPhoenixMsg(CNetMsg::SP& msg, MSG_HELPER_EVENT_PHOENIX_ERROR_TYPE nError, int nCharIdx );

void HelperExpedErrorMsg(CNetMsg::SP& msg, unsigned char errorType, int CharIndex);
void HelperExpedCreateRepMsg(CNetMsg::SP& msg, char nExpedType1,char nExpedType2,char nExpedType3,char nExpedType4, char nExpedType5, int nBossIndex,CLCString BossCharName,int nMemberCount,int *pCharIdex,CLCString* pCharName,int* pGroupType,int* pMemberType,int* pSetLabelType,int* pQuitType);
void HelperExpedInviteRepMsg(CNetMsg::SP& msg, int nBossIndex, CLCString strBossName, int nTargetIndex, CLCString strTargetName,char cExpedType1,char cExpedType2,char cExpedType3);
void HelperExpedAllowRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, CLCString strTargetName,int nTargetGroup, int nTargetMember,int nTargetListIndex, int nErrorCode);
void HelperExpedRejectRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex,int nErrorCode);
void HelperExpedQuitRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, int nQuitMode,int nErrorCode);
void HelperExpedKickRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, int errcode);
void HelperExpedChangeBossRepMsg(CNetMsg::SP& msg, int nBossIndex,  int nNewBossIndex, int nChangeMode);
void HelperExpedChangeTypeRepMsg(CNetMsg::SP& msg, int nBossIndex, char cExpedType, char cDiviType);
void HelperExpedEndExpedRepMsg(CNetMsg::SP& msg, int nBossIndex);
void HelperExpedSetMBossRepMsg(CNetMsg::SP& msg, int nBossIndex, int nNewMBossIndex);
void HelperExpedResetMBossRepMsg(CNetMsg::SP& msg, int nBossIndex, int nNewMBossIndex);
void HelperExpedMoveGroupRepMsg(CNetMsg::SP& msg,int nBossIndex,int nSourceGroup, int nMoveCharIndex, int nTargetGroup,int nTargetListindex);

void HelperRaidInzoneGetRoomNoRep(CNetMsg::SP& msg, int nCharIndex, int nZoneNo, int nRoomNo, int nBossIndex, int nBossRoomNo);
void HelperDeleteRaidCharacterRep(CNetMsg::SP& msg, int nCharIndex, int nSuccess);
void HelperRaidInfoRep(CNetMsg::SP& msg, int nCharIndex, int nRaidCount, int* nZoneNum, int* nRoomNum);

void HelperNSCreateCardErrorMsg(CNetMsg::SP& msg, MSG_HELPER_NS_CARD_TYPE subtype, int userindex, int charindex);

#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE)
void HelperWorldcup2010TOTOErrorMsg(CNetMsg::SP& msg, MSG_HELPER_WORLDCUP2010_ERROR_TYPE subType, int charIndex, int itemidx=-1);
void HelperWorldcup2010TOTOStatusErrorMsg(CNetMsg::SP& msg, MSG_HELPER_WORLDCUP2010_ERROR_TYPE subType, int charIndex, int resultType=-1, int intemIndex = -1);
void HelperWorldcup2010TOTOGiftErrorMsg(CNetMsg::SP& msg, MSG_HELPER_WORLDCUP2010_ERROR_TYPE subType, int charIndex);
void HelperWorldcup2010AttendanceErrorMsg(CNetMsg::SP& msg, MSG_HELPER_WORLDCUP2010_ERROR_TYPE subType,  int charIndex);
void HelperWorldcup2010KoreaErrorMsg(CNetMsg::SP& msg, MSG_HELPER_WORLDCUP2010_ERROR_TYPE subType, int charIndex);
#endif // defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)

#ifdef EXTREME_CUBE_VER2
void HelperExtremeCubeErrorMsg(CNetMsg::SP& msg, HELPER_EXTREME_CUBE_ERROR_TYPE errortype, int charindex);
void HelperCubeRewardPersonalRepMsg(CNetMsg::SP& msg, int updatetime, int charindex, char rank);
void HelperCubeRewardGuildPointRepMsg(CNetMsg::SP& msg, int guildindex, char rank, int guildpoint);
#endif // EXTREME_CUBE_VER2

#ifdef EVENT_CHAR_BIRTHDAY
void HelperEventCharBirthdayGiftRepMsg(CNetMsg::SP& msg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_TYPE errortype, int charindex, int years = 0);
void HelperEventCharBirthdayBDayRepMsg(CNetMsg::SP& msg, int charindex, int year, char month, char day);
#endif
#ifdef DEV_GUILD_MARK
void HelperGuildMarkRegistRepMsg(CNetMsg::SP& msg, int GuildIndex, int CharIndex, char command, char gm_row, char gm_col, char bg_row, char bg_col, unsigned short tab, unsigned short invenIndex, int markTime, const char* serial);
void HelperGuildMarkExpireMsg(CNetMsg::SP& msg, unsigned char command, int GuildIndex);
#endif

#ifdef DEV_GUILD_STASH
void HelperGuildStashListMsg( CNetMsg::SP& msg , int charIndex );
void HelperGuildStashKeepMsg( CNetMsg::SP& msg , int charIndex, int GuildIndex, char error, LONGLONG keepMoney, int itemCount );
void HelperGuildStashTakeMsg( CNetMsg::SP& msg , int charIndex, int GuildIndex, char error, LONGLONG takeMoney, int itemCount );
void HelperGuildStashLogMsg( CNetMsg::SP& msg, int charIndex, int guildIndex, int logCount );
void HelperGuildStashErrorMsg( CNetMsg::SP& msg, int charIndex, char error );
#endif // DEV_GUILD_STASH

void HelperGuildMasterKickRep(CNetMsg::SP& msg, int _guildIndex, int _requestChar, int _result, int _requestTime = 0);
void HelperGuildMasterKickCancelRep(CNetMsg::SP& msg, int _guildIndex, int _requestChar, int _result);
void HelperGuildMasterKickStatus(CNetMsg::SP& msg, int _guildIndex, int _status);
void HelperGuildMasterKickReset(CNetMsg::SP& msg, int _charIndex, int _reset);
#endif	// __CMDMSG_H__
