#ifndef __CMDMSG_H__
#define __CMDMSG_H__

#include "Descriptor.h"

void FailMsg(CNetMsg::SP& msg, MSG_FAIL_TYPE failtype);
void MsgrWhisperNotfoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sidx, const char* sname);
void MsgrWhisperRep(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sidx, const char* sname, const char* rname, const char* chat);
void MsgrLogoutRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, const char* id);
#ifdef NEW_DOUBLE_GM_AUTO
void MsgrDoubleEventRepMsg(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, int cmd, int nas, int nasget, int exp, int sp, int produce, int pronum, int start[], int end[]);
#else
void MsgrDoubleEventRepMsg(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, int cmd, int nas, int nasget, int exp, int sp, int produce, int pronum);
#endif // NEW_DOUBLE_GM_AUTO
void MsgrDoubleExpEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int expPercent);

void MsgrFriendChatNotfoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sidx, const char* sname);
void MsgrFriendChatRep(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sidx, const char* sname, const char* rname, const char* chat);

void MsgrEventRepMsg(CNetMsg::SP& msg, unsigned char type, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int drop);

void MsgrRecommendMsg(CNetMsg::SP& msg);

void MsgrEventGoldenballVoteMsg(CNetMsg::SP& msg, int nGMCharIndex, const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMin);
void MsgrEventGoldenballGiftMsg(CNetMsg::SP& msg, int nGMCharIndex, const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMin, time_t timeEndVote);

#ifdef GMTOOL
void MsgrGmToolKickIDRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* id);
void MsgrGmToolChatMonitorRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, int gmcharindex, const char* id);
void MsgrGMWhisperRep(CNetMsg::SP& msg, int seq, int server, int sub, int zone, int charindex, const char* rname, const char* chat, int serverno, int subno, int sindex);
void MsgrGMWhisperNotfoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex);
void MsgrGMSilenceRep(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int server, int sub, int success, int gmcharindex, const char* name);
void MsgrGMToolWhisperRep(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrGmToolKickRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* name);
#endif // GMTOOL

#ifdef DOUBLE_ITEM_DROP
void MsgrDoubleItemEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int itemPercent);
#endif // DOUBLE_ITEM_DROP

#ifdef DOUBLE_PET_EXP
#ifdef DOUBLE_PET_EXP_AUTO
void MsgrDoublePetExpEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int PetExpPercent, long start = -1, long end = -1);
#else
void MsgrDoublePetExpEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int PetExpPercent);
#endif
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
void MsgrDoubleAttackEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int AttackPercent);
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
void MsgrEventDropItemRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char type, int npcidx, int itemidx, int prob, int thisServer, int thisSub);
#endif // EVENT_DROPITEM

#ifdef NEW_DOUBLE_EVENT_AUTO
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
void MsgrDoubleEventAutoRepMsg(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, char cmd, char state, int start[], int end[]);
#else
void MsgrDoubleEventAutoRepMsg(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, char cmd, char state);
#endif
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef UPGRADE_EVENT_AUTO
void MsgrUpgradeEventRepMsg(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, char cmd, int prob, long start = -1, long end = -1);
#else
void MsgrUpgradeEventRepMsg(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, char cmd, int prob);
#endif

#endif	// __CMDMSG_H__
//
