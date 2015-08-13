#include "stdhdrs.h"

#include "CmdMsg.h"
#include "Server.h"

void FailMsg(CNetMsg::SP& msg, MSG_FAIL_TYPE failtype)
{
	msg->Init(MSG_FAIL);
	RefMsg(msg) << (unsigned char)failtype;
}

void MsgrWhisperNotfoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sidx, const char* sname)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server << subno << zone
				<< (unsigned char)MSG_MSGR_WHISPER_NOTFOUND
				<< sidx << sname;
}

void MsgrWhisperRep(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sidx, const char* sname, const char* rname, const char* chat)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_WHISPER_REP
				<< sidx << sname
				<< rname
				<< chat;
}

void MsgrLogoutRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, const char* id)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_LOGOUT_REP
				<< success
				<< id;
}

#ifdef NEW_DOUBLE_GM_AUTO
void MsgrDoubleEventRepMsg(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, int cmd, int nas, int nasget, int exp, int sp, int produce, int pronum, int start[], int end[])
#else
void MsgrDoubleEventRepMsg(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, int cmd, int nas, int nasget, int exp, int sp, int produce, int pronum)
#endif
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< serverno
				<< subno
				<< zoneno
				<< (unsigned char)MSG_MSGR_DOUBLE_EVENT_REP
				<< charindex
				<< tserver
				<< tsub
				<< cmd
				<< nas
				<< nasget
				<< exp
				<< sp
				<< produce
				<< pronum;
#ifdef NEW_DOUBLE_GM_AUTO
	int	i;
	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) << start[i];
	}
	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) << end[i];
	}
#endif // NEW_DOUBLE_GM_AUTO
}

#ifdef NEW_DOUBLE_EVENT_AUTO
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
void MsgrDoubleEventAutoRepMsg(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, char cmd, char state, int start[], int end[])
#else
void MsgrDoubleEventAutoRepMsg(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, char cmd, char state)
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< serverno
				<< subno
				<< zoneno
				<< (unsigned char)MSG_MSGR_DOUBLE_EVENT_AUTO_REP
				<< charindex
				<< tserver
				<< tsub
				<< cmd
				<< state;
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
	int i;
	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) << start[i];
	}
	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) << end[i];
	}
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
}
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef UPGRADE_EVENT_AUTO
void MsgrUpgradeEventRepMsg(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, char cmd, int prob, long start, long end)
#else
void MsgrUpgradeEventRepMsg(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int charindex, int tserver, int tsub, char cmd, int prob)
#endif
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< serverno
				<< subno
				<< zoneno
				<< (unsigned char)MSG_MSGR_UPGRADE_EVENT_REP
				<< charindex
				<< tserver
				<< tsub
				<< cmd
				<< prob;
#ifdef UPGRADE_EVENT_AUTO
	RefMsg(msg) << start
				<< end;
#endif
}

void MsgrDoubleExpEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int expPercent)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_DOUBLE_EXP_EVENT_REP
				<< charindex
				<< tserver
				<< tsub
				<< cmd
				<< expPercent;
}

void MsgrFriendChatNotfoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sidx, const char* sname)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server << subno << zone
				<< (unsigned char)MSG_MSGR_MESSENGERCHAT_NOTFOUND
				<< sidx << sname;
}

void MsgrFriendChatRep(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sidx, const char* sname, const char* rname, const char* chat)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_MESSENGERCHAT_REP
				<< sidx << sname
				<< rname
				<< chat;
}

void MsgrEventRepMsg(CNetMsg::SP& msg, unsigned char type, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int drop)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< type
				<< charindex
				<< tserver
				<< tsub
				<< cmd
				<< drop;
}

void MsgrRecommendMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << -1
				<< -1
				<< -1
				<< -1
				<< (unsigned char)MSG_MSGR_RECOMMEND
				<< 0
				<< gserver.m_nRecommendServer;
}

void MsgrEventGoldenballVoteMsg(CNetMsg::SP& msg, int nGMCharIndex, const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMin)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << -1
				<< -1
				<< -1
				<< -1
				<< (unsigned char)MSG_MSGR_EVENT_GOLDENBALL
				<< MSG_MSGR_EVENT_GOLDENBALL_VOTE
				<< nGMCharIndex
				<< strTeam1
				<< strTeam2
				<< nYear
				<< nMonth
				<< nDay
				<< nHour
				<< nMin;
}

void MsgrEventGoldenballGiftMsg(CNetMsg::SP& msg, int nGMCharIndex, const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMin, time_t timeEndVote)
{
	msg->Init(MSG_MSGR_REQ);
	RefMsg(msg) << -1
				<< -1
				<< -1
				<< -1
				<< (unsigned char)MSG_MSGR_EVENT_GOLDENBALL
				<< MSG_MSGR_EVENT_GOLDENBALL_GIFT
				<< nGMCharIndex
				<< strTeam1
				<< nTeam1Score
				<< strTeam2
				<< nTeam2Score
				<< nYear
				<< nMonth
				<< nDay
				<< nHour
				<< nMin
				<< (int)timeEndVote;
}

#ifdef GMTOOL
void MsgrGmToolKickIDRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* id)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GMTOOL_KICKID_REP
				<< success
				<< charindex
				<< id;
}

void MsgrGmToolKickRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* name)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GMTOOL_KICK_REP
				<< success
				<< charindex
				<< name;
}

void MsgrGmToolChatMonitorRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, int gmcharindex, const char* id)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_GMTOOL_CHATMONITOR_REP
				<< success
				<< gmcharindex
				<< id;
}

void MsgrGMWhisperNotfoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server << subno << zone
				<< (unsigned char)MSG_MSGR_GM_WHISPER_NOTFOUND
				<< sindex;
}

void MsgrGMSilenceRep(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int server, int sub, int success, int gmcharindex, const char* name)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq << serverno << subno << zoneno
				<< (unsigned char)MSG_MSGR_GMTOOL_SILENCE_REP
				<< server
				<< sub
				<< success
				<< gmcharindex
				<< name;
}

void MsgrGMWhisperRep(CNetMsg::SP& msg, int seq, int server, int sub, int zone, int charindex, const char* rname, const char* chat, int serverno, int subno, int sindex)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< sub
				<< zone
				<< (unsigned char)MSG_MSGR_GM_WHISPER_REP
				<< charindex
				<< rname
				<< chat
				<< serverno
				<< subno
				<< sindex;
}

void MsgrGMToolWhisperRep(CNetMsg::SP& msg, int seq, int serverno, int subno, int zoneno, int sindex, const char* sname, const char* rname, const char* chat)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< serverno
				<< subno
				<< zoneno
				<< (unsigned char)MSG_MSGR_GMTOOL_WHISPER_REP
				<< sindex << sname
				<< rname
				<< chat;
}

#endif // GMTOOL

#ifdef DOUBLE_ITEM_DROP
void MsgrDoubleItemEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int itemPercent)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_DOUBLE_ITEM_EVENT_REP
				<< charindex
				<< tserver
				<< tsub
				<< cmd
				<< itemPercent;
}
#endif // DOUBLE_ITEM_DROP

#ifdef DOUBLE_PET_EXP
#ifdef DOUBLE_PET_EXP_AUTO
void MsgrDoublePetExpEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int PetExpPercent, long start, long end)
#else
void MsgrDoublePetExpEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int PetExpPercent)
#endif
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_DOUBLE_PET_EXP_EVENT_REP
				<< charindex
				<< tserver
				<< tsub
				<< cmd
				<< PetExpPercent;
#ifdef DOUBLE_PET_EXP_AUTO
	RefMsg(msg) << start
				<< end;
#endif
}
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
void MsgrDoubleAttackEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int tserver, int tsub, int cmd, int AttackPercent)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char)MSG_MSGR_DOUBLE_ATTACK_EVENT_REP
				<< charindex
				<< tserver
				<< tsub
				<< cmd
				<< AttackPercent;
}
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
void MsgrEventDropItemRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char type, int npcidx, int itemidx, int prob, int thisServer, int thisSub)
{
	msg->Init(MSG_MSGR_REP);
	RefMsg(msg) << seq
				<< server
				<< subno
				<< zone
				<< (unsigned char) MSG_MSGR_EVENT_DROPITEM_REP
				<< charindex
				<< type
				<< npcidx
				<< itemidx
				<< prob
				<< thisServer
				<< thisSub;
}
#endif // EVENT_DROPITEM
//