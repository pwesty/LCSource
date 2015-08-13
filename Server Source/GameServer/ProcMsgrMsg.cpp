#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Log.h"
#include "Exp.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "ServerTimer.h"
#include "../ShareLib/PrintExcuteInfo.h"
#include "DescManager.h"
#include "Notice.h"
#include "DBManager.h"
#include "../ShareLib/packetType/ptype_notice.h"

void ProcessMessengerMessage_CharLord(CNetMsg::SP& msg);
void ProcessMessengerMessage_Observer(CNetMsg::SP& msg);
void ProcessMessengerMessage_EndGameReq();
void ProcessMessengerMessage_RebootReq(CNetMsg::SP& msg);
void ProcessMessengerMessage_WhisperReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_NoticeChat(CNetMsg::SP& msg, int nZone);
void ProcessMessengerMessage_UpdateNoticeRev(CNetMsg::SP& msg);
void ProcessMessengerMessage_LogoutReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_PlayerCountReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_DoubleEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_DoubleExpEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);

void ProcessMessengerMessage_ChanceEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);

void ProcessMessengerMessage_LattoEventReq(CNetMsg::SP& msg);
void ProcessMessengerMessage_NewyearEventReq(CNetMsg::SP& msg);
void ProcessMessengerMessage_ValentineEventReq(CNetMsg::SP& msg);
void ProcessMessengerMessage_WhitedayEventReq(CNetMsg::SP& msg);
void ProcessMessengerMessage_MessengerChatReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);

void ProcessMessengerMessage_WhisperRep(CNetMsg::SP& msg);
void ProcessMessengerMessage_WhisperNotfound(CNetMsg::SP& msg);
void ProcessMessengerMessage_PlayerCountRep(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_LogoutRep(CNetMsg::SP& msg);
void ProcessMessengerMessage_DoubleExpEventRep(CNetMsg::SP& msg);
void ProcessMessengerMessage_MessengerChatRep(CNetMsg::SP& msg);
void ProcessMessengerMessage_MessengerchatNotfound(CNetMsg::SP& msg);

void ProcessMessengerMessage_MessengerReq( unsigned char subtype, CNetMsg::SP& msg);

void ProcessMessengerMessage_MessengerChatOneReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);


void ProcessEventOXMessage(int server, int subno, CNetMsg::SP& msg);

#ifdef NEW_DOUBLE_GM_ZONE
void ProcessMessengerMessage_DoubleEventRep(CNetMsg::SP& msg, int zone);
#else
void ProcessMessengerMessage_DoubleEventRep(CNetMsg::SP& msg);
#endif // NEW_DOUBLE_GM_ZONE

void ProcessMessengerMessage_Recommend(CNetMsg::SP& msg);
void ProcessMessengerMessage_RecommendMoonstone(CNetMsg::SP& msg);

void ProcessMessengerMessage_EventGoldenball(CNetMsg::SP& msg);

void ProcessMessengerMessage_ChannelChat(CNetMsg::SP& msg);

#ifdef GMTOOL
void ProcessMessengerMessage_GmToolKickIDReq(CNetMsg::SP& msg, int seq, int serverno, int subno, int zone);
void ProcessMessengerMessage_GmToolKickIDRep(CNetMsg::SP& msg);
void ProcessMessengerMessage_GmToolChatMonitorReq(CNetMsg::SP& msg, int seq, int serverno, int subno, int zone);
void ProcessMessengerMessage_GmToolChatMonitorRep(CNetMsg::SP& msg);
void ProcessMessengerMessage_GmToolChatMonitorNotice(CNetMsg::SP& msg);
void ProcessMessengerMessage_GmToolChatMonitorOff(CNetMsg::SP& msg);
void ProcessMessengerMessage_GmToolChatMonitorNoticeParty(CNetMsg::SP& msg);
void ProcessMessengerMessage_GmToolChatMonitorPartyOff(CNetMsg::SP& msg);
void ProcessMessengerMessage_GmToolChatMonitorNoticeGuild(CNetMsg::SP& msg);
void ProcessMessengerMessage_GmToolChatMonitorGuildOff(CNetMsg::SP& msg);
void ProcessMessengerMessage_GmToolWhisperRep(CNetMsg::SP& msg);
void ProcessMessengerMessage_GmWhisperReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_GmToolWhisperReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_GmToolChatMonitorNoticeWhisper(CNetMsg::SP& msg);
void ProcessMessengerMessage_GMLoopCountReq(CNetMsg::SP& msg);
void ProcessMessengerMessage_GMLoopCountRep(CNetMsg::SP& msg);
void ProcessMessengerMessage_GMReloadgmip(CNetMsg::SP& msg);
void ProcessMessengerMessage_GmToolSilenceReq(CNetMsg::SP& msg, int seq, int server, int subno, int zone);
void ProcessMessengerMessage_GmToolSilenceRep(CNetMsg::SP& msg);
void ProcessMessengerMessage_GmToolWhisperNotfound(CNetMsg::SP& msg);
void ProcessMessengerMessage_GMWhisperNotfound(CNetMsg::SP& msg);
void ProcessMessengerMessage_GMToolUserWhisperRep(CNetMsg::SP& msg);
void ProcessMessengerMessage_GmToolKickReq(CNetMsg::SP& msg, int seq, int serverno, int subno, int zone);
void ProcessMessengerMessage_GmToolKickRep(CNetMsg::SP& msg);
void ProcessMessengerMessage_GMToolShutdown(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
#endif // GMTOOL

#ifdef DOUBLE_ITEM_DROP
void ProcessMessengerMessage_DoubleItemEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_DoubleItemEventRep(CNetMsg::SP& msg);
#endif // DOUBLE_ITEM_DROP

#ifdef DOUBLE_PET_EXP
void ProcessMessengerMessage_DoublePetExpEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_DoublePetExpEventRep(CNetMsg::SP& msg);
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
void ProcessMessengerMessage_DoubleAttackEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_DoubleAttackEventRep(CNetMsg::SP& msg);
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
void ProcessMessengerMessage_EventDropItemReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_EventDropItemRep(CNetMsg::SP& msg);
#endif // EVENT_DROPITEM

void ProcessMessengerMessage_SetExpLimit( CNetMsg::SP& msg);
void ProcessMessengerMessage_DoubleEventAutoReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_DoubleEventAutoRep( CNetMsg::SP& msg );
void ProcessMessengerMessage_UpgradeEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone);
void ProcessMessengerMessage_UpgradeEventRep( CNetMsg::SP& msg );

void ProcessMessengerMessage_DungeonTimeTimeSetReq(CNetMsg::SP& msg);
void ProcessMessengerMessage_DungeonTimeExpSetReq(CNetMsg::SP& msg);
void ProcessMessengerMessage_DungeonTimeIntervalSetReq(CNetMsg::SP& msg);
void ProcessMessengerMessage_DungeonTimeToggleSetReq(CNetMsg::SP& msg);

void ProcessMessengerMessage_ReloadCatalog( CNetMsg::SP& msg );
void ProcessMessengerMessage_ModifyCashShop( CNetMsg::SP& msg );

void ProcessMessengerMessage_NpcEcho( CNetMsg::SP& msg, int nZone );
void ProcessMessengerMessage_ResetRaidNow(CNetMsg::SP& msg);
void ProcessMessengerMessage_ResetRaidSet(CNetMsg::SP& msg);

void ProcessMessengerMessage_Notice(CNetMsg::SP& msg);
void ProcessMessengerMessage_Winner(CNetMsg::SP& msg);

#ifdef SYSTEM_TREASURE_MAP
void ProcMgrMsgTreasureMap( CNetMsg::SP& msg) ;
#endif

void ProcessMsgrEventActiveUpdate(CNetMsg::SP& msg);

void ProcessMsgrRoyalRumbleSetting(CNetMsg::SP& msg);
void ProcessMsgrSendMsgToChar(CNetMsg::SP& msg);

void ProcessMessengerMessage_PopupNotice(CNetMsg::SP& msg);
void ProcessMessengerMessage_PopupNotice_Close(CNetMsg::SP& msg);
void ProcessMessengerMessage_PopupNotice_Clear(CNetMsg::SP& msg);

extern void process_expressInputItem(CNetMsg::SP& msg);

void CServer::ProcessMessengerMessage(CNetMsg::SP& msg)
{
	msg->MoveFirst();
	
	switch (msg->m_mtype)
	{
	case MSG_EXPRESS_SYSTEM:
		//LOG_INFO("DEBUG_FUNC : START : type : %d", msg->m_mtype);
		process_expressInputItem(msg);
		//LOG_INFO("DEBUG_FUNC : END : type : %d", msg->m_mtype);
		break;

	case MSG_MSGR_REQ:
		{
			int seq, server, subno, zone;
			unsigned char subtype;

			RefMsg(msg) >> seq
						>> server >> subno >> zone
						>> subtype;

			//성주 메시지 차단됨(검토)
			if (server != -1 && (server != gserver->m_serverno || (subno != -1 && subno != gserver->m_subno)))
				return ;

			if(zone != -1)
			{
				CZone* pZone = gserver->FindZone(zone);
				if (pZone ==  NULL || pZone->m_index != zone)
					return ;
			}

			//LOG_INFO("DEBUG_FUNC : START : type : %d, subtype : %d", msg->m_mtype, subtype);
			switch (subtype)
			{
			case MSG_MSGR_EVENT_GOLDENBALL:
				ProcessMessengerMessage_EventGoldenball(msg);
				break;
			case MSG_MSGR_CHANNEL_CHAT:
				ProcessMessengerMessage_ChannelChat(msg);
				break;

			case MSG_MSGR_RECOMMEND:
				ProcessMessengerMessage_Recommend(msg);
				break;

			case MSG_MSGR_RECOMMEND_MOONSTONE:
				ProcessMessengerMessage_RecommendMoonstone(msg);
				break;

			case MSG_MSGR_EVENT_OX:
				ProcessEventOXMessage(server, subno, msg);
				break;

			case MSG_MSGR_CHAT_LORD:
				ProcessMessengerMessage_CharLord(msg);
				break;

			case MSG_MSGR_EVENT_REQ:
				ProcessEventMessage(msg);
				break;

			case MSG_MSGR_OBSERVER:
				ProcessMessengerMessage_Observer(msg);
				break;

			case MSG_MSGR_ENDGAME_REQ:
				ProcessMessengerMessage_EndGameReq();
				break;

			case MSG_MSGR_REBOOT_REQ:
				ProcessMessengerMessage_RebootReq(msg);
				break;

			case MSG_MSGR_WHISPER_REQ:
				ProcessMessengerMessage_WhisperReq(msg, seq, server, subno, zone);
				break;

			case MSG_MSGR_NOTICECHAT:
				ProcessMessengerMessage_NoticeChat(msg, zone);
				break;

			case MSG_MSGR_UPDATE_REV:
				ProcessMessengerMessage_UpdateNoticeRev(msg);
				break;

			case MSG_MSGR_LOGOUT_REQ:
				ProcessMessengerMessage_LogoutReq(msg, seq, server, subno, zone);
				break;

			case MSG_MSGR_PLAYER_COUNT_REQ:
				ProcessMessengerMessage_PlayerCountReq(msg, seq, server, subno, zone);
				break;

			case MSG_MSGR_DOUBLE_EVENT_REQ:
				ProcessMessengerMessage_DoubleEventReq(msg, seq, server, subno, zone);
				break;

			case MSG_MSGR_DOUBLE_EXP_EVENT_REQ:
				ProcessMessengerMessage_DoubleExpEventReq(msg, seq, server, subno, zone);
				break;

			case MSG_MSGR_CHANCE_EVENT_REQ:
				ProcessMessengerMessage_ChanceEventReq(msg, seq, server, subno, zone);
				break;

			case MSG_MSGR_LATTO_EVENT_REQ:
				ProcessMessengerMessage_LattoEventReq(msg);
				break;

			case MSG_MSGR_NEWYEAR_EVENT_REQ:
				ProcessMessengerMessage_NewyearEventReq(msg);
				break;

			case MSG_MSGR_VALENTINE_EVENT_REQ:
				ProcessMessengerMessage_ValentineEventReq(msg);
				break;

			case MSG_MSGR_WHITEDAY_EVENT_REQ:
				ProcessMessengerMessage_WhitedayEventReq(msg);
				break;

			case MSG_MSGR_MESSENGERCHAT_REQ:
				ProcessMessengerMessage_MessengerChatReq(msg, seq, server, subno, zone);
				break;
			case MSG_MSGR_MESSENGER_ADD_CONFIRM:
			case MSG_MSGR_MESSENGER_ADD:
			case MSG_MSGR_MESSENGER_DEL:
			case MSG_MSGR_MESSENGER_CHAT:
				ProcessMessengerMessage_MessengerReq(subtype, msg);
				break;
			case MSG_MSGR_MESSENGER_ONE_VS_ONE:
				ProcessMessengerMessage_MessengerChatOneReq(msg, seq, server, subno, zone);
				break;

#ifdef GMTOOL
			case MSG_MSGR_GMTOOL_KICKID_REQ:
				ProcessMessengerMessage_GmToolKickIDReq(msg, seq, server, subno, zone);
				break;
			case MSG_MSGR_GMTOOL_KICK_REQ:
				ProcessMessengerMessage_GmToolKickReq(msg, seq, server, subno, zone);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_REQ:
				ProcessMessengerMessage_GmToolChatMonitorReq(msg, seq, server, subno, zone);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE:
				ProcessMessengerMessage_GmToolChatMonitorNotice(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_PARTY:
				ProcessMessengerMessage_GmToolChatMonitorNoticeParty(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_OFF:
				ProcessMessengerMessage_GmToolChatMonitorOff(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_PARTY_OFF:
				ProcessMessengerMessage_GmToolChatMonitorPartyOff(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_GUILD:
				ProcessMessengerMessage_GmToolChatMonitorNoticeGuild(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_GUILD_OFF:
				ProcessMessengerMessage_GmToolChatMonitorGuildOff(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_NOTICE_WHISPER:
				ProcessMessengerMessage_GmToolChatMonitorNoticeWhisper(msg);
				break;
			case MSG_MSGR_GMTOOL_SILENCE_REQ:
				ProcessMessengerMessage_GmToolSilenceReq(msg, seq, server, subno, zone);
				break;
			case MSG_MSGR_GM_LOOPCOUNT_REQ:
				ProcessMessengerMessage_GMLoopCountReq(msg);
				break;
			case MSG_MSGR_GM_LOOPCOUNT_REP:
				ProcessMessengerMessage_GMLoopCountRep(msg);
				break;
			case MSG_MSGR_GM_WHISPER_REQ:
				ProcessMessengerMessage_GmWhisperReq(msg, seq, server, subno, zone);
				break;
			case MSG_MSGR_GMTOOL_WHIPER_REQ:
				ProcessMessengerMessage_GmToolWhisperReq(msg, seq, server, subno, zone);
				break;
			case MSG_MSGR_GMTOOL_SHUTDOWN:
				ProcessMessengerMessage_GMToolShutdown(msg, seq, server, subno, zone);
				break;
#endif // GMTOOL

#ifdef DOUBLE_ITEM_DROP
			case MSG_MSGR_DOUBLE_ITEM_EVENT_REQ:
				ProcessMessengerMessage_DoubleItemEventReq(msg, seq, server, subno, zone);
				break;

#endif // DOUBLE_ITEM_DROP

#ifdef DOUBLE_PET_EXP
			case MSG_MSGR_DOUBLE_PET_EXP_EVENT_REQ:
				ProcessMessengerMessage_DoublePetExpEventReq(msg, seq, server, subno, zone);
				break;
#endif // DOUBLE_PET_EXP
#ifdef DOUBLE_ATTACK
			case MSG_MSGR_DOUBLE_ATTACK_EVENT_REQ:
				ProcessMessengerMessage_DoubleAttackEventReq(msg, seq, server, subno, zone);
				break;
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
			case MSG_MSGR_EVENT_DROPITEM_REQ:
				ProcessMessengerMessage_EventDropItemReq(msg, seq, server, subno, zone);
				break;
#endif // EVENT_DROPITEM

			case MSG_MSGR_SET_EXPLIMIT :
				ProcessMessengerMessage_SetExpLimit( msg );
				break;
#ifdef NEW_DOUBLE_EVENT_AUTO
			case MSG_MSGR_DOUBLE_EVENT_AUTO_REQ:
				ProcessMessengerMessage_DoubleEventAutoReq(msg, seq, server, subno, zone);
				break;
#endif // NEW_DOUBLE_EVENT_AUTO

			case MSG_MSGR_UPGRADE_EVENT_REQ:
				ProcessMessengerMessage_UpgradeEventReq(msg, seq, server, subno, zone);
				break;

			case MSG_MSRG_DUNGEON_TIME_TIMESET:
				ProcessMessengerMessage_DungeonTimeTimeSetReq(msg);
				break;
			case MSG_MSRG_DUNGEON_TIME_EXPSET:
				ProcessMessengerMessage_DungeonTimeExpSetReq(msg);
				break;
			case MSG_MSRG_DUNGEON_TIME_INTERVAL:
				ProcessMessengerMessage_DungeonTimeIntervalSetReq(msg);
				break;
			case MSG_MSRG_DUNGEON_TIME_TOGGLE:
				ProcessMessengerMessage_DungeonTimeToggleSetReq(msg);
				break;

			case MSG_MSGR_RELOAD_CATALOG :
				ProcessMessengerMessage_ReloadCatalog(msg);
				break;
			case MSG_MSGR_MODIFY_CASHSHOP:
				ProcessMessengerMessage_ModifyCashShop(msg);
				break;

			case MSG_MSGR_NPC_NOTICECHAT:
				ProcessMessengerMessage_NpcEcho(msg , zone );
				break;
			case MSG_MSGR_RESET_RAID_NOW:
				ProcessMessengerMessage_ResetRaidNow(msg);
				break;
			case MSG_MSGR_RESET_RAID_SET:
				ProcessMessengerMessage_ResetRaidSet(msg);
				break;
			case MSG_MSGR_WARGROUND_NOTICE:
				ProcessMessengerMessage_Notice(msg);
				break;
			case MSG_MSGR_WARGROUND_WINNER:
				ProcessMessengerMessage_Winner(msg);
				break;
#ifdef SYSTEM_TREASURE_MAP
			case MSG_MSGR_SET_TREASURE_MAP:
				ProcMgrMsgTreasureMap(msg);
				break;
#endif
			case MSG_MSGR_EVENT_AUTOMATION:
				ProcessMsgrEventActiveUpdate(msg);
				break;

			case MSG_MSGR_ROYAL_RUMBLE_SETTING:
				ProcessMsgrRoyalRumbleSetting(msg);
				break;
			case MSG_MSGR_SEND_MSG_TO_CHAR:
				ProcessMsgrSendMsgToChar(msg);
				break;

			case MSG_MSGR_POPUP_NOTICE:
				ProcessMessengerMessage_PopupNotice(msg);
				break;

			case MSG_MSGR_POPUP_NOTICE_CLOSE:
				ProcessMessengerMessage_PopupNotice_Close(msg);
				break;

			case MSG_MSGR_POPUP_NOTICE_CLEAR:
				ProcessMessengerMessage_PopupNotice_Clear(msg);
				break;
			}
			//LOG_INFO("DEBUG_FUNC : END : type : %d, subtype : %d", msg->m_mtype, subtype);
		}
		break;

	case MSG_MSGR_REP:
		{
			int seq, server, subno, zone;
			unsigned char subtype;

			RefMsg(msg) >> seq
						>> server >> subno >> zone
						>> subtype;

			if (server != -1 && (server != gserver->m_serverno || (subno != -1 && subno != gserver->m_subno)))
				return ;

			if ( zone != -1 )
			{
				CZone* pZone = gserver->FindZone(zone);
				if (pZone == NULL || pZone->m_index != zone)
					return ;
			}

			//LOG_INFO("DEBUG_FUNC : START : type : %d, subtype : %d", msg->m_mtype, subtype);
			switch (subtype)
			{
			case MSG_MSGR_WHISPER_REP:
				ProcessMessengerMessage_WhisperRep(msg);
				break;

			case MSG_MSGR_WHISPER_NOTFOUND:
				ProcessMessengerMessage_WhisperNotfound(msg);
				break;

			case MSG_MSGR_PLAYER_COUNT_REP:
				ProcessMessengerMessage_PlayerCountRep(msg, seq, server, subno, zone);
				break;

			case MSG_MSGR_LOGOUT_REP:
				ProcessMessengerMessage_LogoutRep(msg);
				break;

			case MSG_MSGR_DOUBLE_EVENT_REP:
#ifdef NEW_DOUBLE_GM_ZONE
				ProcessMessengerMessage_DoubleEventRep(msg, zone);
#else
				ProcessMessengerMessage_DoubleEventRep(msg);
#endif // NEW_DOUBLE_GM_ZONE
				break;

			case MSG_MSGR_DOUBLE_EXP_EVENT_REP:
				ProcessMessengerMessage_DoubleExpEventRep(msg);
				break;

			case MSG_MSGR_MESSENGERCHAT_REP:
				ProcessMessengerMessage_MessengerChatRep(msg);
				break;

			case MSG_MSGR_MESSENGERCHAT_NOTFOUND:
				ProcessMessengerMessage_MessengerchatNotfound(msg);
				break;
#ifdef GMTOOL
			case MSG_MSGR_GMTOOL_KICKID_REP:
				ProcessMessengerMessage_GmToolKickIDRep(msg);
				break;
			case MSG_MSGR_GMTOOL_KICK_REP:
				ProcessMessengerMessage_GmToolKickRep(msg);
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_REP:
				ProcessMessengerMessage_GmToolChatMonitorRep(msg);
				break;
			case MSG_MSGR_GM_WHISPER_REP:
				ProcessMessengerMessage_GmToolWhisperRep(msg);
				break;
			case MSG_MSGR_GMTOOL_SILENCE_REP:
				ProcessMessengerMessage_GmToolSilenceRep(msg);
				break;
			case MSG_MSGR_GMTOOL_WHISPER_NOTFOUND:
				ProcessMessengerMessage_GmToolWhisperNotfound(msg);
				break;
			case MSG_MSGR_GM_WHISPER_NOTFOUND:
				ProcessMessengerMessage_GMWhisperNotfound(msg);
				break;
			case MSG_MSGR_GM_RELOADGMIP_REP:
				ProcessMessengerMessage_GMReloadgmip(msg);
				break;
			case MSG_MSGR_GMTOOL_WHISPER_REP:
				ProcessMessengerMessage_GMToolUserWhisperRep(msg);
				break;
#endif // GMTOOL

#ifdef DOUBLE_ITEM_DROP
			case MSG_MSGR_DOUBLE_ITEM_EVENT_REP:
				ProcessMessengerMessage_DoubleItemEventRep(msg);
				break;
#endif	// DOUBLE_ITEM_DROP

#ifdef DOUBLE_PET_EXP
			case MSG_MSGR_DOUBLE_PET_EXP_EVENT_REP:
				ProcessMessengerMessage_DoublePetExpEventRep(msg);
				break;
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
			case MSG_MSGR_DOUBLE_ATTACK_EVENT_REP:
				ProcessMessengerMessage_DoubleAttackEventRep(msg);
				break;
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
			case MSG_MSGR_EVENT_DROPITEM_REP:
				ProcessMessengerMessage_EventDropItemRep(msg);
				break;
#endif // EVENT_DROPITEM

#ifdef NEW_DOUBLE_EVENT_AUTO
			case MSG_MSGR_DOUBLE_EVENT_AUTO_REP:
				ProcessMessengerMessage_DoubleEventAutoRep(msg);
				break;
#endif // NEW_DOUBLE_EVENT_AUTO

			case MSG_MSGR_UPGRADE_EVENT_REP:
				ProcessMessengerMessage_UpgradeEventRep(msg);
				break;
			}
			//LOG_INFO("DEBUG_FUNC : END : type : %d, subtype : %d", msg->m_mtype, subtype);
		}
		break;

	default:
		break;
	}
}

void ProcessEventOXMessage(int server, int subno, CNetMsg::SP& msg)
{
	int			nSubType;

	int			nGMCharIndex;
	CPC*		pGMPC;
	int			nYear;
	int			nMonth;
	int			nDay;
	int			nHour;
	int			nMin;

	int			nQuizNo;
	int			nSec;
	void*		pos;

	COXQuizData*	qdFind;

	RefMsg(msg) >> nSubType;

	switch (nSubType)
	{
	case MSG_MSGR_EVENT_OX_QUIZ:
		{
			RefMsg(msg) >> nGMCharIndex
						>> nQuizNo
						>> nSec;

			map_listOXQuiz_t::iterator it = gserver->m_listOXQuiz.find(nQuizNo);
			if (it != gserver->m_listOXQuiz.end())
			{
				gserver->m_nOXQuizRealNo = nQuizNo;
				time(&gserver->m_timeOXQuizLimit);
				gserver->m_timeOXQuizLimit += nSec;
				gserver->m_nOXQuizGMCharIndex = nGMCharIndex;

				qdFind = it->second;

				GAMELOG << init("EVENT OX QUIZ QUESTION")
						<< "BY" << delim
						<< nGMCharIndex << delim
						<< "QUIZ" << delim
						<< nQuizNo << delim
						<< qdFind->GetQuizString()
						<< end;

				pGMPC = PCManager::instance()->getPlayerByCharIndex(nGMCharIndex);
				if (pGMPC)
				{
					CLCString chat(1000);
					chat.Format("QUIZ: [%d]%s(%d sec)", nQuizNo, qdFind->GetQuizString(), nSec);
					CNetMsg::SP rmsg(new CNetMsg);
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
					SEND_Q(rmsg, pGMPC->m_desc);
				}

				CZone* pZone = gserver->FindZone(ZONE_OXQUIZROOM);
				if (pZone)
				{
					if (pZone->m_bRemote == false)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						EventOXQuizQuizMsg(rmsg, gserver->m_nOXQuizSeqNo, nSec, qdFind->GetQuizString());
						pZone->m_area[0].SendToAllClient(rmsg);
					}
				}
			}
		}
		break;

	case MSG_MSGR_EVENT_OX_END:
		RefMsg(msg) >> nGMCharIndex;
		gserver->m_bEventOX = false;
		GAMELOG << init("EVENT OX QUIZ SET END")
				<< " BY " << nGMCharIndex
				<< end;
		pGMPC = PCManager::instance()->getPlayerByCharIndex(nGMCharIndex);
		if (pGMPC)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "OX Quiz End");
			SEND_Q(rmsg, pGMPC->m_desc);
		}
		break;

	case MSG_MSGR_EVENT_OX_SET_START:
		RefMsg(msg) >> nGMCharIndex
					>> nYear
					>> nMonth
					>> nDay
					>> nHour
					>> nMin;

		gserver->m_bEventOX = true;
		gserver->m_bOXQuizNoticeStart = false;
		gserver->m_nOXQuizSeqNo = 1;
		gserver->m_nOXQuizRealNo = -1;

// 브라질은 시간이 안맞으므로 다른 방법을 사용함...   yhj
#if defined(LC_RUS) ||defined(LC_BILA)
		struct tm	tmStart = NOW();
		tmStart.tm_year = nYear - 1900;
		tmStart.tm_mon = nMonth - 1;
		tmStart.tm_mday = nDay;
		tmStart.tm_hour = nHour;
		tmStart.tm_min = nMin;
		tmStart.tm_isdst = -1;
		gserver->m_timeEventOXStart = mktime(&tmStart);
#else
		struct tm	tmStart;
		memset(&tmStart, 0, sizeof(tmStart));
		tmStart.tm_year = nYear - 1900;
		tmStart.tm_mon = nMonth - 1;
		tmStart.tm_mday = nDay;
		tmStart.tm_hour = nHour;
		tmStart.tm_min = nMin;
		tmStart.tm_isdst = -1;
		gserver->m_timeEventOXStart = mktime(&tmStart);
#endif

		GAMELOG << init("EVENT OX QUIZ SET START")
				<< "START TIME" << delim
				<< nYear << delim
				<< nMonth << delim
				<< nDay << delim
				<< nHour << delim
				<< nMin << delim
				<< " BY " << nGMCharIndex
				<< end;

		pGMPC = PCManager::instance()->getPlayerByCharIndex(nGMCharIndex);
		if (pGMPC)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			CLCString chat(1000);
			chat.Format("OX Quiz set start time: %04d-%02d-%02d %02d:%02d", nYear, nMonth, nDay, nHour, nMin);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			SEND_Q(rmsg, pGMPC->m_desc);
		}
		break;
	}
}

void ProcessMessengerMessage_CharLord(CNetMsg::SP& msg)
{
	int sindex;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	RefMsg(msg) >> sindex
				>> sname
				>> chat;

	GAMELOG << init("CHATLOAD CHATMSG SEND TO CLIENT:")
			<< " sindex"  << delim <<  sindex
			<< " sname"  << delim <<  sname
			<< " chat"  << delim <<  chat
			<< end;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_LORD, sindex, sname, "", chat);
		PCManager::instance()->sendToAll(rmsg);
	}
}

void ProcessMessengerMessage_Observer(CNetMsg::SP& msg)
{
	static CLCString chat(1000);
	if (gserver->m_observer)
	{
		RefMsg(msg) >> chat;
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		SEND_Q(rmsg, gserver->m_observer);
	}
}

//XX endgame - 3
void ProcessMessengerMessage_EndGameReq()
{
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ConnRebootReqMsg(rmsg);
		SEND_Q(rmsg, gserver->m_connector);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperShutdownMsg(rmsg);
		SEND_Q(rmsg, gserver->m_helper);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperShutdownMsg(rmsg);
		SEND_Q(rmsg, gserver->m_subHelper);
	}

	gserver->m_bShutdown = true;

	PrintExcuteInfo::Instance()->SetStopMessage("endgame");
	bnf::instance()->Stop();
}

void ProcessMessengerMessage_RebootReq(CNetMsg::SP& msg)
{
	if (RebootTimer::instance()->isRun())
		return;

	int sec;
	static CLCString chat(1000);

	RefMsg(msg) >> sec
				>> chat;

	if (chat.Length() > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		PCManager::instance()->sendToAll(rmsg);
	}

	RebootTimer::instance()->Init(sec);
}

void ProcessMessengerMessage_WhisperReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int sidx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	RefMsg(msg) >> sidx >> sname
				>> rname
				>> chat;

	CPC* receiver = PCManager::instance()->getPlayerByName(rname, true );
	if (receiver == NULL)
	{
		// 수신자가 없을 때
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrRepWhisperNotFoundMsg(rmsg, nSeq, nServer, nSubNo, nZone, sidx, sname);
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else
	{
		{
			// 수신자가 있을 때
			CNetMsg::SP rmsg(new CNetMsg);
			ChatWhisperMsg(rmsg, sidx, sname, receiver->GetName(), chat);
			if( !receiver->IsBlockPC(sidx, sname) )
				SEND_Q(rmsg, receiver->m_desc);
		}

		{
			// 메신저에 응답 보내기
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrRepWhisperRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, sidx, sname, receiver->GetName(), chat);
			SEND_Q(rmsg, gserver->m_messenger);
		}

#ifdef GMTOOL
		if(receiver->m_bGmMonitor)
		{
			// 메신저를 통해서 받은 내용을 툴로 보낸다.
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrNoticeGmChatMonitorWhisperMsg(rmsg, -1, 1, 1, 0, gserver->m_serverno, gserver->m_subno, -1, chat, receiver->m_index, receiver->m_name, (unsigned char)MSG_CHAT_WHISPER, sname);
			SEND_Q(rmsg, gserver->m_messenger);
		}
#endif // GMTOOL
	}
}

void ProcessMessengerMessage_NoticeChat(CNetMsg::SP& msg, int nZone)
{
	static CLCString chat(107);

	RefMsg(msg) >> chat;

	CNetMsg::SP rmsg(new CNetMsg);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);

	PCManager::instance()->sendToZone(rmsg, nZone);
}

void ProcessMessengerMessage_UpdateNoticeRev(CNetMsg::SP& msg)
{
	int rev;
	static CLCString chat(107);
	int date;

	RefMsg(msg) >> rev
		>> chat
		>> date;

	Notice::instance()->setLastRev(rev);

	CNetMsg::SP rmsg(new CNetMsg);
	CheckNoticeMsg(rmsg, rev);
	PCManager::instance()->sendToAll(rmsg);

	NoticeStr* st = new NoticeStr();
	st->date = date;
	st->index = rev;
	strncpy(st->notice_string, chat, 107);
	Notice::instance()->_insert(st);
}

void ProcessMessengerMessage_LogoutReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	static CLCString id(MAX_ID_NAME_LENGTH + 1);

	RefMsg(msg) >> id;

	char success = 1;
	CDescriptor* desc = DescManager::instance()->getDescById(id);
	if (desc)
	{
		desc->Close("ProcessMessengerMessage_LogoutReq()");
	}
	else
	{
		success = 0;
		if (nSeq != -1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrLogoutRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, (char)success, id);
			SEND_Q(rmsg, gserver->m_messenger);
		}
	}
}

void ProcessMessengerMessage_PlayerCountReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int reqChar;

	RefMsg(msg) >> reqChar;

	int i;
	int countZone = 0;
	int* countPC = new int[gserver->m_numZone];
	int* countProduce = new int[gserver->m_numZone];
	int* countShop = new int[gserver->m_numZone];
	int* countPlay = new int[gserver->m_numZone];

	memset(countPC, 0, sizeof(int) * gserver->m_numZone);
	memset(countProduce, 0, sizeof(int) * gserver->m_numZone);
	memset(countShop, 0, sizeof(int) * gserver->m_numZone);
	memset(countPlay, 0, sizeof(int) * gserver->m_numZone);

	PCManager::map_t& playerMap			= PCManager::instance()->getPlayerMap();
	PCManager::map_t::iterator iter		= playerMap.begin();
	PCManager::map_t::iterator endIter	= playerMap.end();
	for (; iter != endIter; ++iter)
	{
		CPC* pc = (*iter).pPlayer;
		if (pc)
		{
			countPC[pc->m_pZone - gserver->m_zones]++;

			// 생산중인가 : pc의 AttackList 중 생산 npc가 있는가
			bool bProduce = false;
			CAttackChar* attackCh = pc->m_attackList;
			while (attackCh)
			{
				if (IS_NPC(attackCh->ch))
				{
					if (attackCh->npc->m_proto->CheckFlag(NPC_MINERAL | NPC_CROPS | NPC_ENERGY))
					{
						bProduce = true;
						break;
					}
				}
				attackCh = attackCh->m_next;
			}

			// 생산중인가
			if (bProduce)
				countProduce[pc->m_pZone - gserver->m_zones]++;
			// 개인상점을 열었는가
			else if (pc->m_personalshop)
				countShop[pc->m_pZone - gserver->m_zones]++;
			// 일반 플레이 유저
			else
				countPlay[pc->m_pZone - gserver->m_zones]++;
		}
	}

	// 리모트를 제외한 존의 개수를 구함
	for (i = 0; i < gserver->m_numZone; i++)
	{
		if (countPC[i] != 0)
			countZone++;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrPlayerCountRep(rmsg, nSeq, nServer, nSubNo, nZone, reqChar, countZone, countPC, countProduce, countShop, countPlay);
		SEND_Q(rmsg, gserver->m_messenger);
	}

	delete [] countPC;
	countPC = NULL;
	delete [] countProduce;
	countProduce = NULL;
	delete [] countShop;
	countShop = NULL;
	delete [] countPlay;
	countPlay = NULL;
}

void ProcessMessengerMessage_DoubleEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex, cmd;

	RefMsg(msg) >> charindex
				>> cmd;

	switch (cmd)
	{
	case 0:
		{
#ifdef NEW_DOUBLE_GM_ZONE
			gserver->m_bDoubleEventZone = nZone;
#endif // NEW_DOUBLE_GM_ZONE

			gserver->m_bDoubleEvent = true;
#ifdef NEW_DOUBLE_GM_AUTO
			int i;
			RefMsg(msg) >> gserver->m_bDoubleNasPercentBackup
						>> gserver->m_bDoubleNasGetPercentBackup
						>> gserver->m_bDoubleExpPercentBackup
						>> gserver->m_bDoubleSpPercentBackup
						>> gserver->m_bDoubleProducePercentBackup
						>> gserver->m_bDoubleProduceNumBackup;
			for(i = 0; i < 6; i++)
			{
				RefMsg(msg) >> gserver->m_iDoubleGMStart[i];
			}
			for(i = 0; i < 6; i++)
			{
				RefMsg(msg) >> gserver->m_iDoubleGMEnd[i];
			}
#else
			RefMsg(msg) >> gserver->m_bDoubleNasPercent
						>> gserver->m_bDoubleNasGetPercent
						>> gserver->m_bDoubleExpPercent
						>> gserver->m_bDoubleSpPercent
						>> gserver->m_bDoubleProducePercent
						>> gserver->m_bDoubleProduceNum;
#endif

#ifdef	TLD_DOUBLE_EVENT

			if ( gserver->m_bDoubleNasPercent <= 0
					|| gserver->m_bDoubleNasGetPercent  <= 0
					|| gserver->m_bDoubleExpPercent  <= 0
					|| gserver->m_bDoubleSpPercent  <= 0
					|| gserver->m_bDoubleProducePercent  <= 0
					|| gserver->m_bDoubleProduceNum  <= 0
					|| gserver->m_bDoubleNasPercent > TLD_DOUBLE_EVENT
					|| gserver->m_bDoubleNasGetPercent > TLD_DOUBLE_EVENT
					|| gserver->m_bDoubleExpPercent  > TLD_DOUBLE_EVENT
					|| gserver->m_bDoubleSpPercent  > TLD_DOUBLE_EVENT
					|| gserver->m_bDoubleProducePercent  > TLD_DOUBLE_EVENT
					|| gserver->m_bDoubleProduceNum  > 10 )
			{
				gserver->m_bDoubleNasPercent = DEFAULT_NAS_PERCENT;
				gserver->m_bDoubleNasGetPercent = DEFAULT_NAS_GET_PERCENT;
				gserver->m_bDoubleExpPercent = DEFAULT_EXP_PERCENT;
				gserver->m_bDoubleSpPercent = DEFAULT_SP_PERCENT;
				gserver->m_bDoubleProducePercent = DEFAULT_PRO_PERCENT;
				gserver->m_bDoubleProduceNum = DEFAULT_PRO_GET_NUMBER;
			}
#else
#ifdef NEW_DOUBLE_GM_AUTO
			if ( gserver->m_bDoubleNasPercentBackup <= 0
					|| gserver->m_bDoubleNasGetPercentBackup  <= 0
					|| gserver->m_bDoubleExpPercentBackup  <= 0
					|| gserver->m_bDoubleSpPercentBackup  <= 0
					|| gserver->m_bDoubleProducePercentBackup  <= 0
					|| gserver->m_bDoubleProduceNumBackup  <= 0
					|| gserver->m_bDoubleNasPercentBackup > 200
					|| gserver->m_bDoubleNasGetPercentBackup > 200
					|| gserver->m_bDoubleExpPercentBackup  > 200
					|| gserver->m_bDoubleSpPercentBackup  > 200
					|| gserver->m_bDoubleProducePercentBackup  > 200
					|| gserver->m_bDoubleProduceNumBackup  > 10 )
			{
				gserver->m_bDoubleNasPercentBackup = DEFAULT_NAS_PERCENT;
				gserver->m_bDoubleNasGetPercentBackup = DEFAULT_NAS_GET_PERCENT;
				gserver->m_bDoubleExpPercentBackup = DEFAULT_EXP_PERCENT;
				gserver->m_bDoubleSpPercentBackup = DEFAULT_SP_PERCENT;
				gserver->m_bDoubleProducePercentBackup = DEFAULT_PRO_PERCENT;
				gserver->m_bDoubleProduceNumBackup = DEFAULT_PRO_GET_NUMBER;
			}
#else
			if ( gserver->m_bDoubleNasPercent <= 0
					|| gserver->m_bDoubleNasGetPercent  <= 0
					|| gserver->m_bDoubleExpPercent  <= 0
					|| gserver->m_bDoubleSpPercent  <= 0
					|| gserver->m_bDoubleProducePercent  <= 0
					|| gserver->m_bDoubleProduceNum  <= 0
					|| gserver->m_bDoubleNasPercent > 200
					|| gserver->m_bDoubleNasGetPercent > 200
					|| gserver->m_bDoubleExpPercent  > 300
					|| gserver->m_bDoubleSpPercent  > 200
					|| gserver->m_bDoubleProducePercent  > 200
					|| gserver->m_bDoubleProduceNum  > 10 )
			{
				gserver->m_bDoubleNasPercent = DEFAULT_NAS_PERCENT;
				gserver->m_bDoubleNasGetPercent = DEFAULT_NAS_GET_PERCENT;
				gserver->m_bDoubleExpPercent = DEFAULT_EXP_PERCENT;
				gserver->m_bDoubleSpPercent = DEFAULT_SP_PERCENT;
				gserver->m_bDoubleProducePercent = DEFAULT_PRO_PERCENT;
				gserver->m_bDoubleProduceNum = DEFAULT_PRO_GET_NUMBER;
			}
#endif // NEW_DOUBLE_GM_AUTO

#endif	// TLD_DOUBLE_EVENT

			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrDoubleEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0);
				SEND_Q(rmsg, gserver->m_messenger);
			}

#ifdef NEW_DOUBLE_GM_SAVEFILE
#ifdef NEW_DOUBLE_GM_AUTO
			FILE* fpDoubleEvent = fopen(NEW_DOUBLE_GM_SAVEFILE, "wt");
			if (fpDoubleEvent)
			{
				int i;
				fprintf(fpDoubleEvent, "%d\n", gserver->m_bDoubleNasPercentBackup);
				fprintf(fpDoubleEvent, "%d\n", gserver->m_bDoubleNasGetPercentBackup);
				fprintf(fpDoubleEvent, "%d\n", gserver->m_bDoubleExpPercentBackup);
				fprintf(fpDoubleEvent, "%d\n", gserver->m_bDoubleSpPercentBackup);
				fprintf(fpDoubleEvent, "%d\n", gserver->m_bDoubleProducePercentBackup);
				fprintf(fpDoubleEvent, "%d\n", gserver->m_bDoubleProduceNumBackup);
				for(i = 0; i < 6; i++)
				{
					fprintf(fpDoubleEvent, "%d\n", gserver->m_iDoubleGMStart[i]);
				}
				for(i = 0; i < 6; i++)
				{
					fprintf(fpDoubleEvent, "%d\n", gserver->m_iDoubleGMEnd[i]);
				}

				fclose(fpDoubleEvent);
				fpDoubleEvent = NULL;
			}
#else
			FILE* fpDoubleEvent = fopen(NEW_DOUBLE_GM_SAVEFILE, "wt");
			if (fpDoubleEvent)
			{
				fprintf(fpDoubleEvent, "%d\n", gserver->m_bDoubleNasPercent);
				fprintf(fpDoubleEvent, "%d\n", gserver->m_bDoubleNasGetPercent);
				fprintf(fpDoubleEvent, "%d\n", gserver->m_bDoubleExpPercent);
				fprintf(fpDoubleEvent, "%d\n", gserver->m_bDoubleSpPercent);
				fprintf(fpDoubleEvent, "%d\n", gserver->m_bDoubleProducePercent);
				fprintf(fpDoubleEvent, "%d\n", gserver->m_bDoubleProduceNum);
				fclose(fpDoubleEvent);
				fpDoubleEvent = NULL;
			}
#endif // NEW_DOUBLE_GM_AUTO
#endif // NEW_DOUBLE_GM_SAVEFILE
		}
		break;
	case 1:
		{
			CNetMsg::SP rmsg(new CNetMsg);
#ifdef NEW_DOUBLE_GM_ZONE
			if (gserver->m_bDoubleEvent)
				MsgrDoubleEventRepMsg(rmsg, nSeq, nServer, nSubNo, gserver->m_bDoubleEventZone, charindex, 1);
			else
				MsgrDoubleEventRepMsg(rmsg, nSeq, nServer, nSubNo, gserver->m_bDoubleEventZone, charindex, 2);
#else
			if (gserver->m_bDoubleEvent)
				MsgrDoubleEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1);
			else
				MsgrDoubleEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2);
#endif // NEW_DOUBLE_GM_ZONE
			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;
	case 2:
		{
			gserver->m_bDoubleEvent = false;

#ifdef NEW_DOUBLE_GM_ZONE

			gserver->m_bDoubleEventZone = -1;
#endif // NEW_DOUBLE_GM_ZONE

			gserver->m_bDoubleNasPercent = DEFAULT_NAS_PERCENT;
			gserver->m_bDoubleNasGetPercent = DEFAULT_NAS_GET_PERCENT;
			gserver->m_bDoubleExpPercent = DEFAULT_EXP_PERCENT;
			gserver->m_bDoubleSpPercent = DEFAULT_SP_PERCENT;
			gserver->m_bDoubleProducePercent = DEFAULT_PRO_PERCENT;
			gserver->m_bDoubleProduceNum = DEFAULT_PRO_GET_NUMBER;
#ifdef NEW_DOUBLE_GM_AUTO
			gserver->m_bDoubleNasPercentBackup = DEFAULT_NAS_PERCENT;
			gserver->m_bDoubleNasGetPercentBackup = DEFAULT_NAS_GET_PERCENT;
			gserver->m_bDoubleExpPercentBackup = DEFAULT_EXP_PERCENT;
			gserver->m_bDoubleSpPercentBackup = DEFAULT_SP_PERCENT;
			gserver->m_bDoubleProducePercentBackup = DEFAULT_PRO_PERCENT;
			gserver->m_bDoubleProduceNumBackup = DEFAULT_PRO_GET_NUMBER;
#endif
			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrDoubleEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2);
				SEND_Q(rmsg, gserver->m_messenger);
			}

#ifdef NEW_DOUBLE_GM_SAVEFILE
			remove(NEW_DOUBLE_GM_SAVEFILE);
#endif // NEW_DOUBLE_GM_SAVEFILE
		}
		break;
	default:
		return;
	}
}

void ProcessMessengerMessage_ChanceEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
}

void ProcessMessengerMessage_DoubleExpEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex, cmd;

	RefMsg(msg) >> charindex
				>> cmd;

	switch (cmd)
	{
	case 0:			// start
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (gserver->m_bDoubleExpEvent)
				MsgrDoubleExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver->m_expPercent);
			else
			{
				gserver->m_bDoubleExpEvent = true;
				RefMsg(msg) >> gserver->m_expPercent;
				if( gserver->m_expPercent <= 0 || gserver->m_expPercent > 200 )
				{
					gserver->m_expPercent = 100;
				}

				MsgrDoubleExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, gserver->m_expPercent);
			}
			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;

	case 1:			// ing
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (gserver->m_bDoubleExpEvent)
				MsgrDoubleExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver->m_expPercent);
			else
				MsgrDoubleExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver->m_expPercent);
			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;

	case 2:			// stop
		{
			CNetMsg::SP rmsg(new CNetMsg);
			gserver->m_bDoubleExpEvent = false;
			MsgrDoubleExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver->m_expPercent);
			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;
	}
}

void ProcessMessengerMessage_LattoEventReq(CNetMsg::SP& msg)
{
	char bEventStart;
	int server, subno, charIndex;

	RefMsg(msg) >> bEventStart
				>> server
				>> subno
				>> charIndex;

	if (bEventStart)
		gserver->m_bLattoEvent= true;
	else
		gserver->m_bLattoEvent = false;

	if (gserver->m_serverno == server && gserver->m_subno == subno)
	{
		CPC* pPC = PCManager::instance()->getPlayerByCharIndex(charIndex);
		if (pPC)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (gserver->m_bLattoEvent)
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Latto Event Start!!");
			else
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Latto Event Stop!!");
			SEND_Q(rmsg, pPC->m_desc);
		}
	}
}

void ProcessMessengerMessage_NewyearEventReq(CNetMsg::SP& msg)
{
	char bEventStart;
	int server, subno, charIndex;

	RefMsg(msg) >> bEventStart
				>> server
				>> subno
				>> charIndex;

	if (bEventStart)
		gserver->m_bNewYearEvent= true;
	else
		gserver->m_bNewYearEvent = false;

	if (gserver->m_serverno == server && gserver->m_subno == subno)
	{
		CPC* pPC = PCManager::instance()->getPlayerByCharIndex(charIndex);
		if (pPC)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (gserver->m_bNewYearEvent)
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "NewYear Event Start!!");
			else
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "NewYear Event Stop!!");
			SEND_Q(rmsg, pPC->m_desc);
		}
	}
}

void ProcessMessengerMessage_ValentineEventReq(CNetMsg::SP& msg)
{
	char bEventStart;
	int server, subno, charIndex;

	RefMsg(msg) >> bEventStart
				>> server
				>> subno
				>> charIndex;

	if (bEventStart)
		gserver->m_bValentineEvent= true;
	else
		gserver->m_bValentineEvent = false;

	if (gserver->m_serverno == server && gserver->m_subno == subno)
	{
		CPC* pPC = PCManager::instance()->getPlayerByCharIndex(charIndex);
		if (pPC)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (gserver->m_bValentineEvent)
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Valentine Event Start!!");
			else
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Valentine Event Stop!!");
			SEND_Q(rmsg, pPC->m_desc);
		}
	}
}

void ProcessMessengerMessage_WhitedayEventReq(CNetMsg::SP& msg)
{
	char bEventStart;
	int server, subno, charIndex;

	RefMsg(msg) >> bEventStart
				>> server
				>> subno
				>> charIndex;

	if (bEventStart)
		gserver->m_bWhiteDayEvent= true;
	else
		gserver->m_bWhiteDayEvent = false;

	if (gserver->m_serverno == server && gserver->m_subno == subno)
	{
		CPC* pPC = PCManager::instance()->getPlayerByCharIndex(charIndex);
		if (pPC)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (gserver->m_bWhiteDayEvent)
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "WhiteDay Event Start!!");
			else
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "WhiteDay Event Stop!!");
			SEND_Q(rmsg, pPC->m_desc);
		}
	}
}

void ProcessMessengerMessage_MessengerReq(unsigned char subtype, CNetMsg::SP& msg)
{
	int makeCharIndex, chatIndex, chatColor, result, charIndex;
	CLCString chat(1000);
	CLCString charName(MAX_CHAR_NAME_LENGTH + 1 );

	RefMsg(msg) >> makeCharIndex
				>> chatIndex
				>> chatColor;

	if( subtype == MSG_MSGR_MESSENGER_ADD_CONFIRM )
		RefMsg(msg) >> charIndex;
	else
		RefMsg(msg) >> charName;

	RefMsg(msg) >> chat;

	switch(subtype)
	{
	case MSG_MSGR_MESSENGER_ADD_CONFIRM:
		{
			CPC* pc = PCManager::instance()->getPlayerByCharIndex(charIndex);
			if( pc )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_ADD, makeCharIndex, chatIndex, chatColor, pc->GetName() );
				SEND_Q(rmsg, gserver->m_messenger);
			}
		}
		break;
	case MSG_MSGR_MESSENGER_ADD:
		result = gserver->addChatGroup(makeCharIndex, chatIndex, charName);
		if( result == 1 )
			gserver->SendChatGroup( (MSG_EX_MESSENGER_TYPE) MSG_EX_MESSENGER_INVITE , makeCharIndex, chatIndex, chatColor, charName, "" );
		break;
	case MSG_MSGR_MESSENGER_DEL:
		gserver->SendChatGroup( (MSG_EX_MESSENGER_TYPE) MSG_EX_MESSENGER_OUT , makeCharIndex, chatIndex, chatColor, charName, "" );
		gserver->delChatGroup(makeCharIndex, chatIndex, charName);
		break;
	case MSG_MSGR_MESSENGER_CHAT:
		gserver->SendChatGroup( (MSG_EX_MESSENGER_TYPE) MSG_EX_MESSENGER_CHAT , makeCharIndex, chatIndex, chatColor, charName, chat);
		break;
	default:
		return;
	}
}

void ProcessMessengerMessage_MessengerChatOneReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int sCharIndex, tCharIndex, ChatColor;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	RefMsg(msg) >> sCharIndex
				>> tCharIndex
				>> ChatColor
				>> sname
				>> chat;

	CPC * receiver = PCManager::instance()->getPlayerByCharIndex( tCharIndex );
	if (receiver != NULL)
	{
		// 수신자가 있을 때
		CNetMsg::SP rmsg(new CNetMsg);
		ChatOneMessengerMsg( rmsg, sCharIndex, sname, tCharIndex, receiver->GetName(), ChatColor, chat );
		SEND_Q( rmsg, receiver->m_desc );
	}
}

void ProcessMessengerMessage_MessengerChatReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int sidx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	RefMsg(msg) >> sidx >> sname
				>> rname
				>> chat;

	CPC* receiver = PCManager::instance()->getPlayerByName(rname, true );
	if (receiver == NULL)
	{
		// 수신자가 없을 때
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrRepMessengerNotFoundMsg(rmsg, nSeq, nServer, nSubNo, nZone, sidx, sname);
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else
	{
		{
			// 수신자가 있을 때
			CNetMsg::SP rmsg(new CNetMsg);
			ChatMessengerMsg(rmsg, sidx, sname, receiver->GetName(), chat);
			SEND_Q(rmsg, receiver->m_desc);
		}

		{
			// 메신저에 응답 보내기
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrRepMessengerChatRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, sidx, sname, receiver->GetName() , chat);
			SEND_Q(rmsg, gserver->m_messenger);
		}
	}
}

void ProcessMessengerMessage_WhisperRep(CNetMsg::SP& msg)
{
	int sidx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	RefMsg(msg) >> sidx >> sname
				>> rname
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

void ProcessMessengerMessage_WhisperNotfound(CNetMsg::SP& msg)
{
	int sidx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);

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

void ProcessMessengerMessage_PlayerCountRep(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int reqChar;
	int countZone;

	RefMsg(msg) >> reqChar
				>> countZone;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(reqChar);
	if (pc)
	{
		int zone, count;
		int nProduce, nShop, nPlay;

		std::string message = "";
		for (int i = 0; i < countZone; i++)
		{
			RefMsg(msg) >> zone >> count
						>> nProduce >> nShop >> nPlay;

			// Server - SubNum - Zone - Player: n - m - l - count
			std::string message = "Server-SubNum-Zone-Player: ";
			if (nServer < 0)
			{
				message += "all";
			}
			else
			{
				message += boost::str(boost::format("%d") % nServer);
			}

			message += "-";

			if (nSubNo < 0)
			{
				message += "all";
			}
			else
			{
				message += boost::str(boost::format("%d") % nSubNo);
			}

			message += boost::str(boost::format("-%d-%d-%d-%d-%d") % zone % count % nProduce % nShop % nPlay);
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message.c_str());
				SEND_Q(rmsg, pc->m_desc);
			}
		}
	}
}

void ProcessMessengerMessage_LogoutRep(CNetMsg::SP& msg)
{
	char success;
	CLCString id(MAX_ID_NAME_LENGTH + 1);
	RefMsg(msg) >> success >> id;
}

#ifdef NEW_DOUBLE_GM_ZONE
void ProcessMessengerMessage_DoubleEventRep(CNetMsg::SP& msg, int zone)
#else
void ProcessMessengerMessage_DoubleEventRep(CNetMsg::SP& msg)
#endif // NEW_DOUBLE_GM_ZONE
{
	int charindex, tserver, tsub, cmd, nas, nasget, exp, sp, produce, pronum;

#ifdef NEW_DOUBLE_GM_AUTO
	int i;
	int start[6];
	int end[6];
	memset(start, -1, sizeof(start));
	memset(end, -1, sizeof(end));
#endif // NEW_DOUBLE_GM_AUTO
	RefMsg(msg) >> charindex
				>> tserver
				>> tsub
				>> cmd
				>> nas
				>> nasget
				>> exp
				>> sp
				>> produce
				>> pronum;
#ifdef NEW_DOUBLE_GM_AUTO
	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) >> start[i];
	}
	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) >> end[i];
	}
#endif // NEW_DOUBLE_GM_AUTO
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (pc)
	{
		std::string message = "";
#ifdef NEW_DOUBLE_GM_ZONE
		message = boost::str(boost::format(
								 "Double Event: Server %2d-%2d Zone %2d: ") % tserver % tsub % zone);
#else
		message = boost::str(boost::format(
								 "Double Event: Server %2d-%2d: ") % tserver % tsub);
#endif // NEW_DOUBLE_GM_ZONE
		switch (cmd)
		{
		case 0:
			message += "start";
			break;
		case 1:
			message += "ing";
			break;
		case 2:
			message += "stop";
			break;
		}

		message += boost::str(boost::format(
								  "Nas_Drop:%d Nas_Get:%d Exp:%d Sp:%d Produce:%d ProNum:%d ")
							  % nas % nasget % exp % sp % produce % pronum);

#ifdef NEW_DOUBLE_GM_AUTO
		message += boost::str(boost::format(
								  "Start:%d-%d-%d %d:%d:%d")
							  % start[0] % start[1] % start[2] % start[3] % start[4] % start[5]);

		message += boost::str(boost::format(
								  "End:%d-%d-%d %d:%d:%d")
							  % end[0] % end[1] % end[2] % end[3] % end[4] % end[5]);
#endif // NEW_DOUBLE_GM_AUTO

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message.c_str());
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}

void ProcessMessengerMessage_DoubleExpEventRep(CNetMsg::SP& msg)
{
	int charindex, tserver, tsub, cmd;
	RefMsg(msg) >> charindex
				>> tserver
				>> tsub
				>> cmd;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (pc)
	{
		std::string message = boost::str(boost::format("Double Exp Event: Server %2d-%2d: ") % tserver % tsub);
		switch (cmd)
		{
		case 0:
			message += "start";
			break;
		case 1:
			message += "ing";
			break;
		case 2:
			message += "stop";
			break;
		}

		message += boost::str(boost::format(" %d ") % gserver->m_expPercent);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message.c_str());
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}

void ProcessMessengerMessage_MessengerChatRep(CNetMsg::SP& msg)
{
	int sidx;//, ridx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	RefMsg(msg) >> sidx >> sname
				>> rname
				>> chat;

	CPC* sender = PCManager::instance()->getPlayerByCharIndex(sidx);
	if (sender != NULL)
	{
		// 송신자가 있으면
		CNetMsg::SP rmsg(new CNetMsg);
		ChatMessengerMsg(rmsg, sidx, sname, rname, chat);
		SEND_Q(rmsg, sender->m_desc);
	}
}

void ProcessMessengerMessage_MessengerchatNotfound(CNetMsg::SP& msg)
{
	int sidx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> sidx >> sname;

	CPC* sender = PCManager::instance()->getPlayerByCharIndex(sidx);
	if (sender != NULL)
	{
		// 송신자가 있으면 오류 알림
		CNetMsg::SP rmsg(new CNetMsg);
		SysFriendNotFoundMsg(rmsg);
		SEND_Q(rmsg, sender->m_desc);
	}
}

void ProcessMessengerMessage_Recommend(CNetMsg::SP& msg)
{
	int nGMCharIndex;
	int nRecommendServer;

	RefMsg(msg) >> nGMCharIndex
				>> nRecommendServer;

	GAMELOG << init("RECOMMEND SERVER")
			<< nRecommendServer << delim
			<< "BY" << delim
			<< nGMCharIndex
			<< end;

	if (nRecommendServer == gserver->m_serverno)
	{
		if (!gserver->m_bRecommend)
		{
			gserver->m_bRecommend = true;
			CNetMsg::SP rmsg(new CNetMsg);
			EventRecommendSetMsg(rmsg);
			PCManager::instance()->sendToAll(rmsg);
		}
	}
	else if (gserver->m_bRecommend)
	{
		gserver->m_bRecommend = false;
		CNetMsg::SP rmsg(new CNetMsg);
		EventRecommendSetMsg(rmsg);
		PCManager::instance()->sendToAll(rmsg);
	}

	CPC* pGMPC = PCManager::instance()->getPlayerByCharIndex(nGMCharIndex);
	if (pGMPC)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if (nRecommendServer > 0)
		{
			CLCString chat(100);
			chat.Format("Set Recommend Server: %d", nRecommendServer);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		}
		else
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Reset Recommend Server");
		SEND_Q(rmsg, pGMPC->m_desc);
	}
}

void ProcessMessengerMessage_RecommendMoonstone(CNetMsg::SP& msg)
{
	int nCharIndex;
	CLCString strCharName(MAX_CHAR_NAME_LENGTH + 1);
	RefMsg(msg) >> nCharIndex
				>> strCharName;
	CNetMsg::SP rmsg(new CNetMsg);
	EventRecommendMoonstoneMsg(rmsg, nCharIndex, strCharName);
	PCManager::instance()->sendToAll(rmsg);
}

void ProcessMessengerMessage_EventGoldenball(CNetMsg::SP& msg)
{
	int nSubType;
	RefMsg(msg) >> nSubType;

	int			nGMCharIndex;
	CLCString	strTeam1(GOLDENBALL_TEAM_NAME_LENGTH + 1);
	CLCString	strTeam2(GOLDENBALL_TEAM_NAME_LENGTH + 1);
	int			nTeam1Score;
	int			nTeam2Score;
	int			nEndYear;
	int			nEndMonth;
	int			nEndDay;
	int			nEndHour;
	int			nEndMin;
	int			nEndVoteTime;

	CPC*		pGMChar;
	CLCString	strGMMessage(1000);

	switch (nSubType)
	{
	case MSG_MSGR_EVENT_GOLDENBALL_VOTE:
		RefMsg(msg) >> nGMCharIndex
					>> strTeam1
					>> strTeam2
					>> nEndYear
					>> nEndMonth
					>> nEndDay
					>> nEndHour
					>> nEndMin;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventGoldenballVoteStartMsg(rmsg, strTeam1, strTeam2, nEndYear, nEndMonth, nEndDay, nEndHour, nEndMin);
			PCManager::instance()->sendToAll(rmsg);
		}

		gserver->m_clGoldenBall.Vote(strTeam1, strTeam2, nEndYear, nEndMonth, nEndDay, nEndHour, nEndMin);
		pGMChar = PCManager::instance()->getPlayerByCharIndex(nGMCharIndex);
		if (pGMChar)
		{
			strGMMessage.Format("GOLDEN BALL VOTE : [%s] vs [%s] : %04d-%02d-%02d %02d:%02d"
								, (const char*)strTeam1
								, (const char*)strTeam2
								, nEndYear
								, nEndMonth
								, nEndDay
								, nEndHour
								, nEndMin
							   );

			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", strGMMessage);
			SEND_Q(rmsg, pGMChar->m_desc);
		}
		GAMELOG << init("EVENT GOLDEN BALL VOTE SET")
				<< "BY" << delim
				<< nGMCharIndex << delim
				<< "TEAM1" << delim
				<< strTeam1 << delim
				<< "TEAM2" << delim
				<< strTeam2 << delim
				<< "END" << delim
				<< nEndYear << delim
				<< nEndMonth << delim
				<< nEndDay << delim
				<< nEndHour << delim
				<< nEndMin
				<< end;
		break;

	case MSG_MSGR_EVENT_GOLDENBALL_GIFT:
		RefMsg(msg) >> nGMCharIndex
					>> strTeam1
					>> nTeam1Score
					>> strTeam2
					>> nTeam2Score
					>> nEndYear
					>> nEndMonth
					>> nEndDay
					>> nEndHour
					>> nEndMin
					>> nEndVoteTime;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventGoldenballGiftStartMsg(rmsg, strTeam1, nTeam1Score, strTeam2, nTeam2Score, nEndYear, nEndMonth, nEndDay, nEndHour, nEndMin);
			PCManager::instance()->sendToAll(rmsg);
		}

		gserver->m_clGoldenBall.Gift(strTeam1, nTeam1Score, strTeam2, nTeam2Score, nEndYear, nEndMonth, nEndDay, nEndHour, nEndMin, (time_t)nEndVoteTime);
		pGMChar = PCManager::instance()->getPlayerByCharIndex(nGMCharIndex);
		if (pGMChar)
		{
			strGMMessage.Format("GOLDEN BALL GIFT : [%s][%d] vs [%s][%d] : %04d-%02d-%02d %02d:%02d"
								, gserver->m_clGoldenBall.GetTeam1Name()
								, nTeam1Score
								, gserver->m_clGoldenBall.GetTeam2Name()
								, nTeam2Score
								, nEndYear
								, nEndMonth
								, nEndDay
								, nEndHour
								, nEndMin
							   );

			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", strGMMessage);
			SEND_Q(rmsg, pGMChar->m_desc);
		}
		GAMELOG << init("EVENT GOLDEN BALL GIFT SET")
				<< "BY" << delim
				<< nGMCharIndex << delim
				<< "TEAM1" << delim
				<< strTeam1 << delim
				<< nTeam1Score << delim
				<< "TEAM2" << delim
				<< strTeam2 << delim
				<< nTeam2Score << delim
				<< "END" << delim
				<< nEndYear << delim
				<< nEndMonth << delim
				<< nEndDay << delim
				<< nEndHour << delim
				<< nEndMin << delim
				<< "END VOTE" << delim
				<< nEndVoteTime
				<< end;
		break;

	case MSG_MSGR_EVENT_GOLDENBALL_END:
		RefMsg(msg) >> nGMCharIndex;
		gserver->m_clGoldenBall.End();
		pGMChar = PCManager::instance()->getPlayerByCharIndex(nGMCharIndex);
		if (pGMChar)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "GOLDEN BALL END");
			SEND_Q(rmsg, pGMChar->m_desc);
		}
		GAMELOG << init("EVENT GOLDEN BALL END")
				<< "BY" << delim
				<< nGMCharIndex
				<< end;
		break;

	default:
		break;
	}
}

void ProcessMessengerMessage_ChannelChat( CNetMsg::SP& msg)
{
	int		sender_index = 0;
	unsigned char chatType, channel;
	CLCString name(MAX_CHAR_NAME_LENGTH+1);
	CLCString chat(1024);

	RefMsg(msg) >> sender_index
				>> name
				>> chat
				>> channel
				>> chatType;

	{
		//메세지 클라에 전송
		CNetMsg::SP rmsg(new CNetMsg);
		SayChannelMsg( rmsg, chatType, sender_index, name, channel, chat );
		PCManager::instance()->sendToAll(rmsg);
	}
}

void ProcessEventMessage_Valentine2006(int server, int subno, int charindex, int flag)
{
}

void ProcessEventMessage_XMas2005(int server, int subno, int charindex, int flag)
{
}
#ifdef EVENT_ITEMDROP_AUTO
void ProcessEventMessage_Itemdrop(int server, int subno, int charindex, int rate, int start[], int end[])
#else
void ProcessEventMessage_Itemdrop(int server, int subno, int charindex, int rate)
#endif
{
	switch (rate)
	{
	case 0:		// stop
		{
#ifdef EVENT_ITEMDROP_AUTO
			gserver->m_bIsItemdrop = false;
			int i;
			for(i = 0; i < 6; i++)
			{
				gserver->m_iEventItemdropEnd[i] = -1;
			}
			FILE* fpItemdrop = fopen(EVENT_ITEMDROP_AUTO_SAVEFILE, "wt");
			if(fpItemdrop)
			{
				fprintf(fpItemdrop, "%d\n", gserver->m_nItemDropEventRateBackup);
				int i;
				for(i = 0; i < 6; i++)
				{
					fprintf(fpItemdrop, "%d\n", gserver->m_iEventItemdropStart[i]);
				}
				for(i = 0; i < 6; i++)
				{
					fprintf(fpItemdrop, "%d\n", gserver->m_iEventItemdropEnd[i]);
				}
				fclose(fpItemdrop);
				fpItemdrop = NULL;
			}
#endif
			gserver->m_nItemDropEventRate = 100;
			break;
		}

	case -1:	// ing
		{
			break;
		}
	default:	// start
		{
#ifdef EVENT_ITEMDROP_AUTO
			gserver->m_nItemDropEventRateBackup = rate;
			int i;
			for(i = 0; i < 6; i++)
			{
				gserver->m_iEventItemdropStart[i] = start[i];
			}
			for(i = 0; i < 6; i++)
			{
				gserver->m_iEventItemdropEnd[i] = end[i];
			}
			FILE* fpItemdrop = fopen(EVENT_ITEMDROP_AUTO_SAVEFILE, "wt");
			if(fpItemdrop)
			{
				fprintf(fpItemdrop, "%d\n", gserver->m_nItemDropEventRateBackup);
				int i;
				for(i = 0; i < 6; i++)
				{
					fprintf(fpItemdrop, "%d\n", gserver->m_iEventItemdropStart[i]);
				}
				for(i = 0; i < 6; i++)
				{
					fprintf(fpItemdrop, "%d\n", gserver->m_iEventItemdropEnd[i]);
				}
				fclose(fpItemdrop);
				fpItemdrop = NULL;
			}
#else
			gserver->m_nItemDropEventRate = rate;
#endif
			break;
		}
	}

	if (gserver->m_serverno == server && gserver->m_subno == subno)
	{
		CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
		if (pc)
		{
#ifdef EVENT_ITEMDROP_AUTO
			std::string message = "Event Itemdrop ";
			if (rate == 0)
			{
				message += "Stop!!";
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message.c_str());
				SEND_Q(rmsg, pc->m_desc);
			}
			else if(rate == -1)
			{
				message += boost::str(boost::format(
										  "Now rate:%d Event Rate:%d "
										  "Evnet Start time:%d-%d-%d %d:%d:%d "
										  "Event End time:%d-%d-%d %d:%d:%d")
									  % gserver->m_nItemDropEventRate % gserver->m_nItemDropEventRateBackup
									  % gserver->m_iEventItemdropStart[0] % gserver->m_iEventItemdropStart[1] % gserver->m_iEventItemdropStart[2] % gserver->m_iEventItemdropStart[3] % gserver->m_iEventItemdropStart[4] % gserver->m_iEventItemdropStart[5]
									  % gserver->m_iEventItemdropEnd[0] % gserver->m_iEventItemdropEnd[1] % gserver->m_iEventItemdropEnd[2] % gserver->m_iEventItemdropEnd[3] % gserver->m_iEventItemdropEnd[4] % gserver->m_iEventItemdropEnd[5]
									 );

				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message.c_str());
				SEND_Q(rmsg, pc->m_desc);
			}
			else
			{
				message += boost::str(boost::format(
										  "Start!! Start:"
										  "%d-%d-%d %d:%d:%d "
										  "End:"
										  "%d-%d-%d %d:%d:%d ")
									  % gserver->m_iEventItemdropStart[0] % gserver->m_iEventItemdropStart[1] % gserver->m_iEventItemdropStart[2] % gserver->m_iEventItemdropStart[3] % gserver->m_iEventItemdropStart[4] % gserver->m_iEventItemdropStart[5]
									  % gserver->m_iEventItemdropEnd[0] % gserver->m_iEventItemdropEnd[1] % gserver->m_iEventItemdropEnd[2] % gserver->m_iEventItemdropEnd[3] % gserver->m_iEventItemdropEnd[4] % gserver->m_iEventItemdropEnd[5]
									 );

				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message.c_str());
				SEND_Q(rmsg, pc->m_desc);
			}
#else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				if (gserver->m_nItemDropEventRate == 100)
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Event Itemdrop Stop!!");
				else
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Event Itemdrop Start!!");
				SEND_Q(rmsg, pc->m_desc);
			}
#endif // EVENT_ITEMDROP_AUTO
		}
	}
}

void CServer::ProcessEventMessage(CNetMsg::SP& msg)
{
	int server, subno, charIndex, flag;
	CLCString cmdstring(256);

	RefMsg(msg) >> server
				>> subno
				>> charIndex
				>> cmdstring
				>> flag;

	if (strcmp(cmdstring, "itemdrop") == 0)
	{
#ifdef EVENT_ITEMDROP_AUTO
		int i;
		int start[6];
		int end[6];
		memset(start, -1, sizeof(start));
		memset(end, -1, sizeof(end));
		for(i = 0; i < 6; i++)
		{
			RefMsg(msg) >> start[i];
		}
		for(i = 0; i < 6; i++)
		{
			RefMsg(msg) >> end[i];
		}
		ProcessEventMessage_Itemdrop(server, subno, charIndex, flag, start, end);
#else
		ProcessEventMessage_Itemdrop(server, subno, charIndex, flag);
#endif
	}

	else if (strcmp(cmdstring, "xmas2005") == 0)
		ProcessEventMessage_XMas2005(server, subno, charIndex, flag);

	else if (strcmp(cmdstring, "valentine2006") == 0)
		ProcessEventMessage_Valentine2006(server, subno, charIndex, flag);
}

#ifdef GMTOOL
void ProcessMessengerMessage_GmToolKickIDReq(CNetMsg::SP& msg, int seq, int serverno, int subno, int zone)
{
	static CLCString id(MAX_ID_NAME_LENGTH + 1);

	int server, sub, charindex;
	RefMsg(msg) >> server
				>> sub
				>> charindex
				>> id;

	char success = 0;
	CDescriptor* desc = DescManager::instance()->getDescById(id);
	if (desc)
	{
		desc->Close("ProcessMessengerMessage_GmToolKickIDReq");
		success = 1;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgGmKickIDCommandRepMsg(rmsg, seq, server, sub, zone, (char)success, charindex, id);
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void ProcessMessengerMessage_GmToolKickReq(CNetMsg::SP& msg, int seq, int serverno, int subno, int zone)
{
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	int server, sub, charindex;

	RefMsg(msg) >> server
				>> sub
				>> charindex
				>> name;

	char success = 0;
	CPC* pc = PCManager::instance()->getPlayerByName(name, true);
	if (pc)
	{
		pc->m_desc->Close("ProcessMessengerMessage_GmToolKickReq()");
		success = 1;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgGmKickCommandRepMsg(rmsg, seq, server, sub, zone, (char)success, charindex, name);
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void ProcessMessengerMessage_GmToolChatMonitorReq(CNetMsg::SP& msg, int seq, int serverno, int subno, int zone)
{
	CLCString name(MAX_ID_NAME_LENGTH + 1);
	CLCString onoff(100);
	int gmcharindex;
	RefMsg(msg) >> gmcharindex
				>> name
				>> onoff;

	char success = 0;
	CPC* pc = PCManager::instance()->getPlayerByName(name, false);
	if(pc)
	{
		if(strcmp((const char*)onoff, "on") == 0)
		{
			pc->m_bGmMonitor = true;

			CParty* party = pc->m_party;
			if(party)
			{
				party->m_bGmtarget = true;
			}
			CGuildMember* guildmember = gserver->m_guildlist.findmember(pc->m_index);
			if(guildmember)
			{
				CGuild* guild = guildmember->guild();
				if(guild)
				{
					guild->m_bGmtarget = true;
				}
			}
			success = 1;
		}
		else if(strcmp((const char*)onoff, "off") == 0)
		{
			pc->m_bGmMonitor = false;
			success = 2;
		}
	}
	else
	{
		success = 0;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrGmToolChatMonitorRepMsg(rmsg, seq, 1, 1, -1, success, gmcharindex, name);
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void ProcessMessengerMessage_GmToolKickIDRep(CNetMsg::SP& msg)
{
	char success;
	CLCString id(MAX_ID_NAME_LENGTH + 1);
	int charindex;
	RefMsg(msg) >> success >> charindex >> id;

	CPC* gmtool = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(gmtool)
	{
		char chat[100];
		sprintf(chat, "Kick %s ID: %s", (success ? "success" : "not found"), (const char*)id);

		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		SEND_Q(rmsg, gmtool->m_desc);
	}
}

void ProcessMessengerMessage_GmToolKickRep(CNetMsg::SP& msg)
{
	char success;
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	int charindex;
	RefMsg(msg) >> success >> charindex >> name;

	CPC* gmtool = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(gmtool)
	{
		char chat[100];
		sprintf(chat, "Kick %s NAME: %s", (success ? "success" : "not found"), (const char*)name);

		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		SEND_Q(rmsg, gmtool->m_desc);
	}
}

void ProcessMessengerMessage_GmToolChatMonitorRep(CNetMsg::SP& msg)
{
	char success;
	CLCString id(MAX_ID_NAME_LENGTH + 1);
	int gmcharindex;
	RefMsg(msg) >> success >> gmcharindex >> id;

	CPC* gmtool;
	gmtool = PCManager::instance()->getPlayerByCharIndex(gmcharindex);

	if(gmtool)
	{
		char chat[100];

		switch(success)
		{
		case 0:
			sprintf(chat, "GMTOOL CHATMONITOR NOT FOUND ID: %s", (const char*)id);
			break;
		case 1:
			sprintf(chat, "GMTOOL CHATMONITOR ID: %s ON", (const char*)id);
			break;
		case 2:
			sprintf(chat, "GMTOOL CHATMONITOR OFF ID: %s OFF", (const char*)id);
			break;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GMSayMsg(rmsg, 0, 0, 0, "", chat, MSG_CHAT_GM_SAY);
			SEND_Q(rmsg, gmtool->m_desc);
		}
	}
}

void ProcessMessengerMessage_GmToolChatMonitorNotice(CNetMsg::SP& msg)
{
	int server, sub, zone;
	int charindex;
	unsigned char chattype;
	CLCString chat(100);
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
//	CLCString recever(MAX_ID_NAME_LENGTH + 1)
	RefMsg(msg) >> server
				>> sub
				>> zone
				>> chat
				>> charindex
				>> name
				>> chattype;

	if(gserver->m_gmtool && gserver->m_nGMToolCount > 0)
	{
		bool bSend = false;
		for(int i = 0; i < gserver->m_nGMToolCount; ++i)
		{
			if(gserver->m_gmtool[i])
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GMSayMsg(rmsg, server, sub, charindex, name, chat, chattype);
				SEND_Q(rmsg, gserver->m_gmtool[i]);

				bSend = true;
			}
		}

		if(bSend)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnGMToolChatMonitor(rmsg, charindex, name, chattype, server, sub, chat);
			SEND_Q(rmsg, gserver->m_connector);
		}
		else
		{
			// 툴이 없으면 감시 해제한다.
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrGmToolChatMonitorOffMsg(rmsg, -1, server, sub, zone, charindex);
			SEND_Q(rmsg, gserver->m_messenger);
		}
	}
}

void ProcessMessengerMessage_GmToolChatMonitorNoticeWhisper(CNetMsg::SP& msg)
{
	int server, sub, zone, charindex;
	unsigned char chattype;
	CLCString chat(1000), sender(MAX_CHAR_NAME_LENGTH + 1), name(MAX_CHAR_NAME_LENGTH + 1);
	RefMsg(msg) >> server
				>> sub
				>> zone
				>> chat
				>> charindex
				>> name
				>> chattype
				>> sender;

	char gmchat[1000];
	memset(gmchat, 0, 1000);
	sprintf(gmchat, "WHISPER: %s ==> %s: %s", (const char*)sender,(const char*) name, (const char*)chat);

	if(gserver->m_gmtool && gserver->m_nGMToolCount > 0)
	{
		int i;
		bool bSend = false;
		for(i = 0 ; i < gserver->m_nGMToolCount; ++i)
		{
			if(gserver->m_gmtool[i])
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GMSayMsg(rmsg, server, sub, charindex, name, gmchat, chattype);
				SEND_Q(rmsg, gserver->m_gmtool[i]);

				bSend = true;
			}
		}

		if(bSend)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnGMToolChatMonitor(rmsg, charindex, name, chattype, server, sub, chat);
			SEND_Q(rmsg, gserver->m_connector);
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrGmToolChatMonitorOffMsg(rmsg, -1, server, sub, zone, charindex);
			SEND_Q(rmsg, gserver->m_messenger);
		}
	}
}

void ProcessMessengerMessage_GmToolChatMonitorNoticeParty(CNetMsg::SP& msg)
{
	int server, sub, zone;
	int bossindex;
	int charindex;
	CLCString chat(1000);
	CLCString name(MAX_CHAR_NAME_LENGTH + 1), charname(MAX_CHAR_NAME_LENGTH + 1);
	RefMsg(msg) >> server
				>> sub
				>> zone
				>> chat
				>> bossindex
				>> name
				>> charindex
				>> charname;

	char gmchat[1000];
	memset(gmchat, 0, 1000);
	sprintf(gmchat, "PARTYCHAT: %s: %s", (const char*)name, (const char*)chat);
	if(gserver->m_gmtool && gserver->m_nGMToolCount > 0)
	{
		int i;
		bool bSend = false;
		for(i = 0 ; i < gserver->m_nGMToolCount; ++i)
		{
			if(gserver->m_gmtool[i])
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GMSayMsg(rmsg, server, sub, charindex, name, gmchat, (unsigned char)MSG_CHAT_PARTY);
				SEND_Q(rmsg, gserver->m_gmtool[i]);

				bSend = true;
			}
		}

		if(bSend)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnGMToolChatMonitor(rmsg, charindex, name, (unsigned char)MSG_CHAT_PARTY, server, sub, gmchat);
			SEND_Q(rmsg, gserver->m_connector);
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrGmToolChatMonitorPartyOffMsg(rmsg, -1, server, sub, zone, bossindex);
			SEND_Q(rmsg, gserver->m_messenger);
		}
	}
}

void ProcessMessengerMessage_GmToolChatMonitorNoticeGuild(CNetMsg::SP& msg)
{
	int server, sub, zone;
	int guildindex;
	int charindex;
	CLCString chat(1000);
	CLCString name(MAX_CHAR_NAME_LENGTH + 1), charname(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> server
				>> sub
				>> zone
				>> chat
				>> guildindex
				>> name
				>> charindex
				>> charname;

	char gmchat[1000];
	memset(gmchat, 0, 1000);
	sprintf(gmchat, "GUILDCHAT: %s: %s", (const char*)name, (const char*)chat);

	if(gserver->m_gmtool && gserver->m_nGMToolCount > 0)
	{
		int i;
		bool bSend = false;
		for(i = 0; i < gserver->m_nGMToolCount; ++i)
		{
			if(gserver->m_gmtool[i])
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GMSayMsg(rmsg, server, sub, charindex, charname, gmchat, (unsigned char)MSG_CHAT_GUILD);
				SEND_Q(rmsg, gserver->m_gmtool[i]);
				bSend = true;
			}
		}

		if(bSend)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnGMToolChatMonitor(rmsg, charindex, name, (unsigned char)MSG_CHAT_GUILD, server, sub, gmchat);
			SEND_Q(rmsg, gserver->m_connector);
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrGmToolChatMonitorGuildOffMsg(rmsg, -1, server, sub, zone, guildindex);
			SEND_Q(rmsg, gserver->m_messenger);
		}
	}
}

void ProcessMessengerMessage_GmToolChatMonitorOff(CNetMsg::SP& msg)
{
	int charindex;
	RefMsg(msg) >> charindex;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(pc)
	{
		pc->m_bGmMonitor = false;

		CParty* party = pc->m_party;
		if(party)
		{
			party->m_bGmtarget = false;
		}
		CGuildMember* guildmember = gserver->m_guildlist.findmember(pc->m_index);
		if(guildmember)
		{
			CGuild* guild = guildmember->guild();
			if(guild)
			{
				guild->m_bGmtarget = false;
			}
		}
	}
}

void ProcessMessengerMessage_GmToolChatMonitorPartyOff(CNetMsg::SP& msg)
{
	int i;
	int bossindex;
	RefMsg(msg) >> bossindex;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(bossindex);
	if(pc)
	{
		if(pc->IsParty() && pc->m_party)
		{
			CParty* party = pc->m_party;
			party->m_bGmtarget = false;
			for(i = 0; i < MAX_PARTY_MEMBER; ++i)
			{
				if(party->GetMemberByListIndex(i) && party->GetMemberByListIndex(i)->GetMemberPCPtr())
				{
					party->GetMemberByListIndex(i)->GetMemberPCPtr()->m_bGmMonitor = false;
				}
			}
		}
	}
}

void ProcessMessengerMessage_GmToolChatMonitorGuildOff(CNetMsg::SP& msg)
{
	int i;
	int guildindex;
	RefMsg(msg) >> guildindex;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if(guild)
	{
		guild->m_bGmtarget = false;
		for(i = 0 ; i < GUILD_MAX_MEMBER; ++i)
		{
			if(guild->member(i) && guild->member(i)->GetPC())
			{
				guild->member(i)->GetPC()->m_bGmMonitor = false;
			}
		}
	}
}

void ProcessMessengerMessage_GmToolWhisperRep(CNetMsg::SP& msg)
{
	CLCString rname(MAX_ID_NAME_LENGTH + 1), sname(MAX_ID_NAME_LENGTH + 1);
	CLCString chat(1000);

	int rindex;
	int sindex;
	int server, sub;
	RefMsg(msg) >> rindex
				>> rname
				>> chat
				>> server
				>> sub
				>> sindex;

	CPC* gmtool = PCManager::instance()->getPlayerByCharIndex(sindex);

	if(gmtool)
	{
		char message[1000];
		memset(message, 0, 1000);
		sprintf(message, "WHISPER: [%d-%d] %s ==> %s: %s", server, sub, (const char*)gmtool->m_name, (const char*)rname, (const char*)chat);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GMSayMsg(rmsg,  server, sub, rindex, rname, message, MSG_CHAT_WHISPER);
			SEND_Q(rmsg, gmtool->m_desc);
		}
	}
}

void ProcessMessengerMessage_GMLoopCountReq(CNetMsg::SP& msg)
{
	if(gserver->m_bLoop)
		return;

	gserver->m_bLoop = 2;
	gserver->m_nLoop = 0;
	GetTimeofDay(&gserver->m_tvLoop, NULL);
}

void ProcessMessengerMessage_GMLoopCountRep(CNetMsg::SP& msg)
{
	int serverno, subno;
	CLCString loopcount(100);
	RefMsg(msg) >> serverno
				>> subno
				>> loopcount;

	if(gserver->m_descLoop)
	{
		char tmpBuf[1024] = {0,};
		sprintf(tmpBuf, "[%d-%d] %s", serverno, subno, (const char*)loopcount);
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpBuf);
		SEND_Q(rmsg, gserver->m_descLoop);
		gserver->m_descLoop = NULL;
	}
}

void ProcessMessengerMessage_GMReloadgmip(CNetMsg::SP& msg)
{
}

void ProcessMessengerMessage_GmToolSilenceReq(CNetMsg::SP& msg, int seq, int server, int subno, int zone)
{
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	int sec;
	int gmcharindex;

	RefMsg(msg) >> gmcharindex
				>> name
				>> sec;

	if(name.Length() < 4 || sec < 1)
	{
		return;
	}

	int success = 0;
	CPC* pc = PCManager::instance()->getPlayerByName(name, true);
	if (pc == NULL)
		return;

	success = 1;
	pc->m_silencePulse = gserver->m_pulse + (sec * PULSE_REAL_SEC);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgGMSilenceRep(rmsg, seq, gserver->m_serverno, gserver->m_subno, success, gmcharindex, (const char* )name);
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void ProcessMessengerMessage_GmToolSilenceRep(CNetMsg::SP& msg)
{
	int server, sub;
	int success;
	int gmcharindex;
	CLCString name(100);
	RefMsg(msg) >> server
				>> sub
				>> success
				>> gmcharindex
				>> name;

	char tmp[256];
	memset(tmp, 0, 256);
	sprintf(tmp, "[%d-%d] Silence Name:%s %s", server, sub, (const char*)name, success ? "Success" : "Not Found");

	CPC* gmtool = PCManager::instance()->getPlayerByCharIndex(gmcharindex);

	if(gmtool)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmp);
		SEND_Q(rmsg, gmtool->m_desc);
	}
}

void ProcessMessengerMessage_GmToolWhisperNotfound(CNetMsg::SP& msg)
{
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	int charindex;
	RefMsg(msg) >> charindex
				>> name;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(pc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysWhisperNotFoundMsg(rmsg);
		SEND_Q(rmsg, pc->m_desc);
	}
}

void ProcessMessengerMessage_GmWhisperReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int sidx;
	static CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
	static CLCString chat(1000);

	RefMsg(msg) >> sidx >> sname
				>> rname
				>> chat;

	CPC* receiver = PCManager::instance()->getPlayerByName(rname, true );

	if (receiver == NULL)
	{
		// 수신자가 없을 때
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrRepGMWhisperNotFoundMsg(rmsg, nSeq, 1, 1, 0, sidx);
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else
	{
		{
			// 수신자가 있을 때
			CNetMsg::SP rmsg(new CNetMsg);
			ChatGMWhisperMsg(rmsg, sidx, /*sname*/ "GM", receiver->GetName(), chat);		// 툴이 유저에게 보내는 메시지 클라이언트와 협의
			SEND_Q(rmsg, receiver->m_desc);
		}

		{
			// 메신저에 응답 보내기
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrRepGMWhisperRepMsg(rmsg, nSeq, 1, 1, 0, receiver->m_index, receiver->GetName(), chat, gserver->m_serverno, gserver->m_subno, sidx);
			SEND_Q(rmsg, gserver->m_messenger);
		}
	}
}

void ProcessMessengerMessage_GmToolWhisperReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	CLCString chat(1000);
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);
	CLCString rname(MAX_CHAR_NAME_LENGTH + 1);

	int charindex, serverno, subno;
	RefMsg(msg) >> chat
				>> charindex
				>> name
				>> serverno
				>> subno
				>> rname;

	CPC* gmtool = PCManager::instance()->getPlayerByName(rname, false);

	if(gmtool)
	{
		char buf[1000];
		memset(&buf, 0, 1000);
		sprintf(buf, "WHISPER: %s ==> %s: %s", (const char*)name, (const char*)gmtool->m_name, (const char*)chat);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GMSayMsg(rmsg, serverno, subno, charindex, name, buf, MSG_CHAT_GM_WHISPER);
			SEND_Q(rmsg, gmtool->m_desc);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrRepGmToolWhisperRepMsg(rmsg, nSeq, serverno, subno, -1, charindex, name, gmtool->m_name, chat);
			SEND_Q(rmsg, gserver->m_messenger);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnGMToolChatting(rmsg, serverno, subno, gmtool->m_index, gmtool->m_name, name, buf);
			SEND_Q(rmsg, gserver->m_connector);
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrRepGMToolWhisperNotFoundMsg(rmsg, nSeq, nServer, nSubNo, -1, charindex, name);
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void ProcessMessengerMessage_GMWhisperNotfound(CNetMsg::SP& msg)
{
	int sindex;
	RefMsg(msg) >> sindex;
	CPC* gmtool = PCManager::instance()->getPlayerByCharIndex(sindex);
	if(gmtool)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysWhisperNotFoundMsg(rmsg);
		SEND_Q(rmsg, gmtool->m_desc);
	}
}

void ProcessMessengerMessage_GMToolUserWhisperRep(CNetMsg::SP& msg)
{
	int sindex;
	CLCString sname(MAX_CHAR_NAME_LENGTH + 1), rname(MAX_CHAR_NAME_LENGTH + 1);
	CLCString chat(1000);

	RefMsg(msg) >> sindex
				>> sname >> rname
				>> chat;

	CPC* sender = PCManager::instance()->getPlayerByName(sname, true );
	if(sender)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ChatGMWhisperMsg(rmsg, sindex, sname, rname, chat);
		SEND_Q(rmsg, sender->m_desc);
	}
}

void ProcessMessengerMessage_GMToolShutdown(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	if (ShutdownTimer::instance()->isRun())
		return;

	int remain;
	CLCString chat(1000);

	RefMsg(msg) >> remain
				>> chat;

	if (chat.Length() > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		PCManager::instance()->sendToAll(rmsg);
	}

	ShutdownTimer::instance()->Init(remain);
}
#endif // GMTOOL

#ifdef DOUBLE_ITEM_DROP
void ProcessMessengerMessage_DoubleItemEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex, cmd;
	RefMsg(msg) >> charindex
				>> cmd;

	switch (cmd)
	{
	case 0:
		{
			// start
			if (gserver->m_bDoubleItemEvent)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrDoubleItemEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver->m_bDoubleItemPercent );
				SEND_Q(rmsg, gserver->m_messenger);
			}
			else
			{
				gserver->m_bDoubleItemEvent = true;
				RefMsg(msg) >> gserver->m_bDoubleItemPercent;
				/*        /////// yhj    081014 ///////////////// 확률 제한해제
				if( gserver->m_bDoubleItemPercent < 0 || gserver->m_bDoubleItemPercent > 200 )
				{
					gserver->m_bDoubleItemPercent = 100;
				}
				*///////////////////////////////////////////////////

				CNetMsg::SP rmsg(new CNetMsg);
				MsgrDoubleItemEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, gserver->m_bDoubleItemPercent);
				SEND_Q(rmsg, gserver->m_messenger);
			}

#ifdef DOUBLE_ITEM_DROP_SAVEFILE
			FILE* fpDoubleItemEvent = fopen(DOUBLE_ITEM_DROP_SAVEFILE, "wt");
			if (fpDoubleItemEvent)
			{
				fprintf(fpDoubleItemEvent, "%d\n", gserver->m_bDoubleItemPercent);
				fclose(fpDoubleItemEvent);
				fpDoubleItemEvent = NULL;
			}
#endif // DOUBLE_ITEM_DROP_SAVEFILE
		}
		break;

	case 1:			// ing
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (gserver->m_bDoubleItemEvent)
				MsgrDoubleItemEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver->m_bDoubleItemPercent);
			else
				MsgrDoubleItemEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver->m_bDoubleItemPercent);
			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;

	case 2:			// stop
		{
			gserver->m_bDoubleItemEvent = false;

			CNetMsg::SP rmsg(new CNetMsg);
			MsgrDoubleItemEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver->m_bDoubleItemPercent);
			SEND_Q(rmsg, gserver->m_messenger);

#ifdef DOUBLE_ITEM_DROP_SAVEFILE
			remove(DOUBLE_ITEM_DROP_SAVEFILE);
#endif // DOUBLE_ITEM_DROP_SAVEFILE
		}
		break;
	}
}

void ProcessMessengerMessage_DoubleItemEventRep(CNetMsg::SP& msg)
{
	int charindex, tserver, tsub, cmd;
	RefMsg(msg) >> charindex
				>> tserver
				>> tsub
				>> cmd;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (pc)
	{
		std::string message = boost::str(boost::format("Double Exp Event: Server %2d-%2d: ") % tserver % tsub);

		switch (cmd)
		{
		case 0:
			message += "start";
			break;
		case 1:
			message += "ing";
			break;
		case 2:
			message += "stop";
			break;
		}

		message += boost::str(boost::format(" %d ") % gserver->m_bDoubleItemPercent);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message.c_str());
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}

#endif // DOUBLE_ITEM_DROP

#ifdef DOUBLE_PET_EXP
void ProcessMessengerMessage_DoublePetExpEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex, cmd;

	RefMsg(msg) >> charindex
				>> cmd;
#ifdef DOUBLE_PET_EXP_AUTO
	int percent = 100;
	long tStart = -1;
	long tEnd = -1;
	RefMsg(msg) >> percent
				>> tStart
				>> tEnd;
#endif

	switch (cmd)
	{
	case 0:			// start
		{
#ifdef DOUBLE_PET_EXP_AUTO
			if(gserver->m_PetExpPercent < 100 || gserver->m_PetExpPercent > 1000)
				gserver->m_PetExpPercent = 100;

			gserver->m_PetExpPercent = percent;
			gserver->m_tPetExpEventStart = tStart;
			gserver->m_tPetExpEventEnd = tEnd;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrDoublePetExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, gserver->m_PetExpPercent, tStart, tEnd);
				SEND_Q(rmsg, gserver->m_messenger);
			}

#ifdef DOUBLE_PET_EXP_SAVEFILE
			FILE* fDoublePetExp = fopen(DOUBLE_PET_EXP_SAVEFILE, "wt");
			if(fDoublePetExp)
			{
				fprintf(fDoublePetExp, "%ld\n", (long)gserver->m_tPetExpEventStart);
				fprintf(fDoublePetExp, "%ld\n", (long)gserver->m_tPetExpEventEnd);
				fprintf(fDoublePetExp, "%d\n", gserver->m_PetExpPercent);
				fclose(fDoublePetExp);
				fDoublePetExp = NULL;
			}
#endif // DOUBLE_PET_EXP_SAVEFILE
#else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				if (gserver->m_bDoublePetExpEvent)
					MsgrDoublePetExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver->m_PetExpPercent);
				else
				{
					gserver->m_bDoublePetExpEvent = true;
					RefMsg(msg) >> gserver->m_PetExpPercent;
					if( gserver->m_PetExpPercent <= 0 || gserver->m_PetExpPercent > 200 )
					{
						gserver->m_PetExpPercent = 100;
					}

					MsgrDoublePetExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, gserver->m_PetExpPercent);
				}
				SEND_Q(rmsg, gserver->m_messenger);
			}
#endif
		}
		break;

	case 1:			// ing
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if (gserver->m_bDoublePetExpEvent)
				MsgrDoublePetExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver->m_PetExpPercent);
			else
				MsgrDoublePetExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver->m_PetExpPercent);
			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;

	case 2:			// stop
		{
#ifdef DOUBLE_PET_EXP_AUTO
			gserver->m_tPetExpEventStart = -1;
			gserver->m_tPetExpEventEnd = -1;
			gserver->m_PetExpPercent = 100;
#ifdef DOUBLE_PET_EXP_SAVEFILE
			FILE* fDoublePetExp = fopen(DOUBLE_PET_EXP_SAVEFILE, "wt");
			if(fDoublePetExp)
			{
				fprintf(fDoublePetExp, "%ld\n", (long)gserver->m_tPetExpEventStart);
				fprintf(fDoublePetExp, "%ld\n", (long)gserver->m_tPetExpEventEnd);
				fprintf(fDoublePetExp, "%d\n", gserver->m_PetExpPercent);
				fclose(fDoublePetExp);
				fDoublePetExp = NULL;
			}
#endif
#endif
			gserver->m_bDoublePetExpEvent = false;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrDoublePetExpEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver->m_PetExpPercent);
				SEND_Q(rmsg, gserver->m_messenger);
			}
		}
		break;
	}
}

void ProcessMessengerMessage_DoublePetExpEventRep(CNetMsg::SP& msg)
{
	int charindex, tserver, tsub, cmd;
	RefMsg(msg) >> charindex
				>> tserver
				>> tsub
				>> cmd;
#ifdef DOUBLE_PET_EXP_AUTO
	int prob;
	long start = -1;
	long end = -1;
	RefMsg(msg) >> prob
				>> start
				>> end;
	struct tm tmStart;
	struct tm tmEnd;
	time_t tStart = (time_t)start;
	time_t tEnd = (time_t)end;
#endif
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (pc)
	{
		std::string message = boost::str(boost::format("Double Pet Exp Event: Server %2d-%2d: ") % tserver % tsub);

		switch (cmd)
		{
		case 0:
			{
				message += "start";


#ifdef DOUBLE_PET_EXP_AUTO
				if(tStart < 0 || tEnd < 0)
					return ;
				memcpy(&tmStart, localtime(&tStart), sizeof(tmStart));
				memcpy(&tmEnd, localtime(&tEnd), sizeof(tmEnd));

				message += boost::str(boost::format(
										  " Start Time : %d-%d-%d %d:%d:%d"
										  " End Time : %d-%d-%d %d:%d:%d")
									  % (tmStart.tm_year+1900) % (tmStart.tm_mon+1) % tmStart.tm_mday % tmStart.tm_hour % tmStart.tm_min % tmStart.tm_sec
									  % (tmEnd.tm_year+1900) % (tmEnd.tm_mon+1) % tmEnd.tm_mday % tmEnd.tm_hour % tmEnd.tm_min % tmEnd.tm_sec
									 );
#endif
				break;
			}

		case 1:
			{
				message += "ing";
				break;
			}
		case 2:
			message += "stop";
			break;
		}

		message += boost::str(boost::format(" %d ") % gserver->m_PetExpPercent);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message.c_str());
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
void ProcessMessengerMessage_DoubleAttackEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex, cmd;
	RefMsg(msg) >> charindex
				>> cmd;

	switch (cmd)
	{
	case 0:			// start
		{
			CNetMsg::SP rmsg(new CNetMsg);

			if (gserver->m_bDoubleAttackEvent)
				MsgrDoubleAttackEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver->m_AttackPercent);
			else
			{
				gserver->m_bDoubleAttackEvent = true;
				RefMsg(msg) >> gserver->m_AttackPercent;
				if( gserver->m_AttackPercent <= 0 || gserver->m_AttackPercent > 200 )
				{
					gserver->m_AttackPercent = 100;
				}

				MsgrDoubleAttackEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, gserver->m_AttackPercent);
			}

			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;

	case 1:			// ing
		{
			CNetMsg::SP rmsg(new CNetMsg);

			if (gserver->m_bDoubleAttackEvent)
				MsgrDoubleAttackEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, gserver->m_AttackPercent);
			else
				MsgrDoubleAttackEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver->m_AttackPercent);

			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;

	case 2:			// stop
		{
			gserver->m_bDoubleAttackEvent = false;

			CNetMsg::SP rmsg(new CNetMsg);
			MsgrDoubleAttackEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, gserver->m_AttackPercent);
			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;
	}
}

void ProcessMessengerMessage_DoubleAttackEventRep(CNetMsg::SP& msg)
{
	int charindex, tserver, tsub, cmd;
	RefMsg(msg) >> charindex
				>> tserver
				>> tsub
				>> cmd;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (pc)
	{
		std::string message = boost::str(boost::format("Double Attack Event: Server %2d-%2d: ") % tserver % tsub);

		switch (cmd)
		{
		case 0:
			message +=  "start";
			break;
		case 1:
			message +=  "ing";
			break;
		case 2:
			message +=  "stop";
			break;
		}

		message += boost::str(boost::format(" %d ") % gserver->m_AttackPercent);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message.c_str());
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
void ProcessMessengerMessage_EventDropItemReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex, npcidx, itemidx, prob, fromServer, fromSub;
	char type;

	RefMsg(msg) >> charindex
				>> type
				>> npcidx
				>> itemidx
				>> prob
				>> fromServer
				>> fromSub;

	switch(type)
	{
	case 0:		// start
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if(gserver->m_bDropItem)
			{
				MsgrEventDropItemRepMsg(rmsg, nSeq, fromServer, fromSub, -1, charindex, 1, gserver->m_bDropItemNpcIdx, gserver->m_bDropItemIdx,
										gserver->m_bDropProb, gserver->m_serverno, gserver->m_subno);
			}
			else
			{
				gserver->m_bDropItem = true;

				CNPCProto* npcproto = NULL;
				CItem* item = NULL;
				npcproto = gserver->m_npcProtoList.FindProto(npcidx);
				item = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 1);
				if(npcproto && item)
				{
					if(prob < 1 || prob > 100)
						prob = 1;

					gserver->m_bDropItemNpcIdx	= npcidx;
					gserver->m_bDropItemIdx		= itemidx;
					gserver->m_bDropProb			= prob;
					MsgrEventDropItemRepMsg(rmsg, nSeq, fromServer, fromSub, -1, charindex, 0, npcidx, itemidx, prob, gserver->m_serverno, gserver->m_subno);
				}
				else
				{
					MsgrEventDropItemRepMsg(rmsg, nSeq, fromServer, fromSub, -1, charindex, 4, -1, -1, 0, gserver->m_serverno, gserver->m_subno);
				}

				if(item)
				{
					delete item;
					item = NULL;
				}
			}
			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;
	case 1:		// ing
		{
			CNetMsg::SP rmsg(new CNetMsg);

			if(gserver->m_bDropItem)
				MsgrEventDropItemRepMsg(rmsg, nSeq, fromServer, fromSub, -1, charindex, 1, gserver->m_bDropItemNpcIdx, gserver->m_bDropItemIdx,
										gserver->m_bDropProb, gserver->m_serverno, gserver->m_subno);
			else
				MsgrEventDropItemRepMsg(rmsg, nSeq, fromServer, fromSub, -1, charindex, 2, gserver->m_bDropItemNpcIdx, gserver->m_bDropItemIdx,
										gserver->m_bDropProb, gserver->m_serverno, gserver->m_subno);

			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;
	case 2:		// stop
		{
			gserver->m_bDropItem = false;

			CNetMsg::SP rmsg(new CNetMsg);
			MsgrEventDropItemRepMsg(rmsg, nSeq, fromServer, fromSub, -1, charindex, 2, gserver->m_bDropItemNpcIdx, gserver->m_bDropItemIdx,
									gserver->m_bDropProb, gserver->m_serverno, gserver->m_subno);
			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;
	}
}

void ProcessMessengerMessage_EventDropItemRep(CNetMsg::SP& msg)
{
	int charindex, npcidx, itemidx, prob, tServer, tSub;
	char type;

	RefMsg(msg) >> charindex
				>> type
				>> npcidx
				>> itemidx
				>> prob
				>> tServer
				>> tSub;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (pc)
	{
		std::string message = boost::str(boost::format("Event NPC DropItem: Server %2d-%2d: ") % tServer % tSub);

		switch(type)
		{
		case 0:
			{
				message += "start";
			}
			break;
		case 1:
			{
				message += "ing";
			}
			break;
		case 2:
			{
				message += "stop";
			}
			break;
		case 3:		// 운영자명령어 실행실패
			{
				message += "Not Abailable";
			}
			break;
		}

		message += boost::str(boost::format(
								  "NPC: %d item: %d Prob: %d")
							  % npcidx % itemidx % prob);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message.c_str());
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}
#endif // EVENT_DROPITEM

void ProcessMessengerMessage_SetExpLimit( CNetMsg::SP& msg)
{
#ifdef GM_EXP_LIMIT_AUTO
	int charindex;
	int nLimitStart;
	int nLimitEnd;
	int i;

	RefMsg(msg) >> charindex
				>> nLimitStart
				>> nLimitEnd;

	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) >> gserver->m_nGMExpLimitStart[i];
	}
	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) >> gserver->m_nGMExpLimitEnd[i];
	}
	gserver->m_nExpLimitStart = nLimitStart;
	gserver->m_nExpLimitEnd = nLimitEnd;
	gserver->m_nSpLimitStart = nLimitStart;
	gserver->m_nSpLimitEnd = nLimitEnd;
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (pc)
	{
		char strMsg[1024];
		sprintf( strMsg, "Set Limit Start ( %d ) Time:%d-%d-%d %d:%d:%d End ( %d ) Time:%d-%d-%d %d:%d:%d",
				 nLimitStart,
				 gserver->m_nGMExpLimitStart[0],
				 gserver->m_nGMExpLimitStart[1],
				 gserver->m_nGMExpLimitStart[2],
				 gserver->m_nGMExpLimitStart[3],
				 gserver->m_nGMExpLimitStart[4],
				 gserver->m_nGMExpLimitStart[5],
				 nLimitEnd,
				 gserver->m_nGMExpLimitEnd[0],
				 gserver->m_nGMExpLimitEnd[1],
				 gserver->m_nGMExpLimitEnd[2],
				 gserver->m_nGMExpLimitEnd[3],
				 gserver->m_nGMExpLimitEnd[4],
				 gserver->m_nGMExpLimitEnd[5]
			   );

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", strMsg );
			SEND_Q(rmsg, pc->m_desc);
		}
	}
	// 파일 저장 할 것.
	FILE* fpExpLimit = fopen(GM_EXP_LIMIT_AUTO_SAVEFILE, "wt");
	if(fpExpLimit)
	{
		fprintf(fpExpLimit, "%d\n", nLimitStart);
		fprintf(fpExpLimit, "%d\n", nLimitEnd);
		for(i = 0; i < 6; i++)
		{
			fprintf(fpExpLimit, "%d\n", gserver->m_nGMExpLimitStart[i]);
		}
		for(i = 0; i < 6; i++)
		{
			fprintf(fpExpLimit, "%d\n", gserver->m_nGMExpLimitEnd[i]);
		}
		fclose(fpExpLimit);
		fpExpLimit = NULL;
	}
#else
	int charindex;
	int nLimit;

	RefMsg(msg) >> charindex
				>> nLimit;

	if( nLimit > 0 && nLimit <= 3000 )
	{
		gserver->m_nExpLimit = nLimit;
		gserver->m_nSpLimit = nLimit;
	}

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (pc)
	{
		char strMsg[64];
		sprintf( strMsg, " Set Limit Sucesses ( %d ) " , gserver->m_nExpLimit );
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", strMsg );
		SEND_Q(rmsg, pc->m_desc);
	}
#endif // GM_EXP_LIMIT_AUTO
}

void ProcessMessengerMessage_DoubleEventAutoReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
#ifdef NEW_DOUBLE_EVENT_AUTO
	int charindex;
	char cmd;
	RefMsg(msg) >> charindex
				>> cmd;

	switch(cmd)
	{
	case 0:
		{
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
			int i;
			for (i = 0; i < 6; i++)
			{
				RefMsg(msg) >> gserver->m_iDoubleEventStart[i];
			}
			for (i = 0; i < 6; i++)
			{
				RefMsg(msg) >> gserver->m_iDoubleEventEnd[i];
			}
			// member 변수에 저장

			// 파일에 시간 저장
			FILE* fpDoubleEvent = fopen(NEW_DOUBLE_EVENT_AUTO_SAVEFILE, "wt");
			if (fpDoubleEvent)
			{
				for(i = 0; i < 6; i++)
				{
					fprintf(fpDoubleEvent, "%d\n", gserver->m_iDoubleEventStart[i]);
				}
				for(i = 0; i < 6; i++)
				{
					fprintf(fpDoubleEvent, "%d\n", gserver->m_iDoubleEventEnd[i]);
				}

				fclose(fpDoubleEvent);
				fpDoubleEvent = NULL;
			}
			gserver->NewDoubleEventAuto();

			{
				CNetMsg::SP rmsg(new CNetMsg);
				if(gserver->m_bDoubleEventAutoOn)
					MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, 1, gserver->m_iDoubleEventStart, gserver->m_iDoubleEventEnd);
				else
					MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, 0, gserver->m_iDoubleEventStart, gserver->m_iDoubleEventEnd);
#else
			gserver->m_bDoubleEventAuto = true;
			gserver->NewDoubleEventAuto();
			if(gserver->m_bDoubleEventAutoOn)
				MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, 1);
			else
				MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, 0);
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
				SEND_Q(rmsg, gserver->m_messenger);
			}
		}
		break;

	case 1:
		{
			if(gserver->m_bDoubleEventAuto)
			{
				CNetMsg::SP rmsg(new CNetMsg);
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
				if(gserver->m_bDoubleEventAutoOn)
					MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, 1, gserver->m_iDoubleEventStart, gserver->m_iDoubleEventEnd);
				else
					MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, 0, gserver->m_iDoubleEventStart, gserver->m_iDoubleEventEnd);
#else
				if(gserver->m_bDoubleEventAutoOn)
					MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, 1);
				else
					MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1, 0);
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
				SEND_Q(rmsg, gserver->m_messenger);
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
				MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, 0, gserver->m_iDoubleEventStart, gserver->m_iDoubleEventEnd);
#else
				MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, 0);
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
				SEND_Q(rmsg, gserver->m_messenger);
			}
		}
		break;

	case 2:
		{
			gserver->m_bDoubleEventAuto = false;
			gserver->m_bDoubleEventAutoOn = false;

			CNetMsg::SP rmsg(new CNetMsg);
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
			memset(gserver->m_iDoubleEventEnd, -1, sizeof(gserver->m_iDoubleEventEnd));
			FILE* fpDoubleEvent = fopen(NEW_DOUBLE_EVENT_AUTO_SAVEFILE, "wt");
			if (fpDoubleEvent)
			{
				int i;
				for(i = 0; i < 6; i++)
				{
					fprintf(fpDoubleEvent, "%d\n", gserver->m_iDoubleEventStart[i]);
				}
				for(i = 0; i < 6; i++)
				{
					fprintf(fpDoubleEvent, "%d\n", gserver->m_iDoubleEventEnd[i]);
				}

				fclose(fpDoubleEvent);
				fpDoubleEvent = NULL;
			}
			MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, 0, gserver->m_iDoubleEventStart, gserver->m_iDoubleEventEnd);
#else
			MsgrDoubleEventAutoRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2, 0);
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;
	}
#endif // NEW_DOUBLE_EVENT_AUTO
}

void ProcessMessengerMessage_DoubleEventAutoRep( CNetMsg::SP& msg )
{
#ifdef NEW_DOUBLE_EVENT_AUTO
	int charindex, server, sub;
	int start[6];
	int end[6];
	memset(start, -1, sizeof(start));
	memset(end, -1, sizeof(end));
	char state, cmd;
	RefMsg(msg) >> charindex
				>> server
				>> sub
				>> cmd
				>> state;
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
	int i;
	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) >> start[i];
	}
	for(i = 0; i < 6; i++)
	{
		RefMsg(msg) >> end[i];
	}
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(pc)
	{
		std::string message = boost::str(boost::format("Double Event Auto: Server %d-%d: ") % server % sub);



		switch(cmd)
		{
		case 0:
			message += "Start ";

			if(state)
			{
				message += "On";
			}
			else
			{
				message += "OFF";
			}
			break;
		case 1:
			message += "ing ";

			if(state)
			{
				message += "On";
			}
			else
			{
				message += "OFF";
			}
			break;
		case 2:
			message += "stop";
			break;
		}
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
		message += boost::str(boost::format(
								  " Start:%d-%d-%d %d:%d:%d"
								  " End:%d-%d-%d %d:%d:%d")
							  % start[0] % start[1] % start[2] % start[3] % start[4] % start[5]
							  % end[0] % end[1] % end[2] % end[3] % end[4] % end[5]
							 );
#endif // NEW_DOUBLE_EVENT_AUTO_TIME

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message.c_str());
			SEND_Q(rmsg, pc->m_desc);
		}
	}
#endif // NEW_DOUBLE_EVENT_AUTO
}

void ProcessMessengerMessage_UpgradeEventReq(CNetMsg::SP& msg, int nSeq, int nServer, int nSubNo, int nZone)
{
	int charindex;
	char cmd;
	int prob;
#ifdef UPGRADE_EVENT_AUTO
	long start = -1;
	long end = -1;
#endif
	RefMsg(msg) >> charindex
				>> cmd
				>> prob;

	switch(cmd)
	{
	case 0:							// Start
		{
#ifdef UPGRADE_EVENT_AUTO
			RefMsg(msg) >> start
						>> end;

			gserver->m_tUpgradeEventStart = (time_t)start;
			gserver->m_tUpgradeEventEnd = (time_t)end;
			FILE* fUpgradeEvent = fopen(UPGRADE_EVENT_AUTO_SAVEFILE, "wt");
			if(fUpgradeEvent)
			{
				fprintf(fUpgradeEvent, "%ld\n", (long)start);
				fprintf(fUpgradeEvent, "%ld\n", (long)end);
				fprintf(fUpgradeEvent, "%d\n", prob);
				fclose(fUpgradeEvent);
				fUpgradeEvent = NULL;
			}
#else // UPGRADE_EVENT_AUTO
			g_bUpgradeEvent = true;
#endif // UPGRADE_EVENT_AUTO

			switch(prob)
			{
			case 100:
			case 125:
			case 150:
			case 175:
			case 200:
				break;
			default:
				prob = 100;
				return ;
			}
			g_nUpgradeProb = prob;

			{
				CNetMsg::SP rmsg(new CNetMsg);
#ifdef UPGRADE_EVENT_AUTO
				MsgrUpgradeEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0, start, end);
#else
				MsgrUpgradeEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 0);
#endif
				SEND_Q(rmsg, gserver->m_messenger);
			}
		}
		break;
	case 1:							// ing
		{
			CNetMsg::SP rmsg(new CNetMsg);

			if(g_bUpgradeEvent)
				MsgrUpgradeEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 1);
			else
				MsgrUpgradeEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2);

			SEND_Q(rmsg, gserver->m_messenger);
		}
		break;
	case 2:							// End
		{
			g_bUpgradeEvent = false;
			g_nUpgradeProb = 100;
#ifdef UPGRADE_EVENT_AUTO
			gserver->m_tUpgradeEventStart = -1;
			gserver->m_tUpgradeEventEnd = -1;
			FILE* fUpgradeEvent = fopen(UPGRADE_EVENT_AUTO_SAVEFILE, "wt");
			if(fUpgradeEvent)
			{
				fprintf(fUpgradeEvent, "%ld\n", (long)gserver->m_tUpgradeEventStart);
				fprintf(fUpgradeEvent, "%ld\n", (long)gserver->m_tUpgradeEventEnd);
				fprintf(fUpgradeEvent, "%d\n", g_nUpgradeProb);
				fclose(fUpgradeEvent);
				fUpgradeEvent = NULL;
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventEnchantRate(rmsg, 0, 100);
				PCManager::instance()->sendToAll(rmsg);
			}
#endif

			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrUpgradeEventRepMsg(rmsg, nSeq, nServer, nSubNo, nZone, charindex, 2);
				SEND_Q(rmsg, gserver->m_messenger);
			}
		}
		break;
	}
}

void ProcessMessengerMessage_UpgradeEventRep( CNetMsg::SP& msg )
{
	int charindex, tserver, tsub, prob;
#ifdef UPGRADE_EVENT_AUTO
	long start = -1;
	long end = -1;
	struct tm tmStart;
	struct tm tmEnd;
#endif // UPGRADE_EVENT_AUTO
	char cmd;

	RefMsg(msg) >> charindex
				>> tserver
				>> tsub
				>> cmd
				>> prob;
#ifdef UPGRADE_EVENT_AUTO
	RefMsg(msg) >> start
				>> end;
#endif // UPGRADE_EVENT_AUTO

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(pc)
	{
		std::string message = boost::str(boost::format("Upgrade Event: Server %2d-%2d: ") % tserver % tsub);

		switch (cmd)
		{
		case 0:
			{
#ifdef UPGRADE_EVENT_AUTO
				time_t tStart = (time_t)start;
				time_t tEnd = (time_t)end;
				memcpy(&tmStart, localtime(&tStart), sizeof(tmStart));
				memcpy(&tmEnd, localtime(&tEnd), sizeof(tmEnd));


				message += boost::str(boost::format(
										  "start time : "
										  "%d-%d-%d %d:%d:%d"
										  " end time : "
										  "%d-%d-%d %d:%d:%d")
									  % (tmStart.tm_year+1900) % (tmStart.tm_mon+1) % tmStart.tm_mday % tmStart.tm_hour % tmStart.tm_min % tmStart.tm_sec
									  % (tmEnd.tm_year+1900) % (tmEnd.tm_mon+1) % tmEnd.tm_mday % tmEnd.tm_hour % tmEnd.tm_min % tmEnd.tm_sec
									 );
#else // UPGRADE_EVENT_AUTO
				message += "start";
#endif // UPGRADE_EVENT_AUTO
			}
			break;
		case 1:
			message += "ing";
			break;
		case 2:
			message += "stop";
			break;
		}
		message += boost::str(boost::format(" Prob:%d ") % prob);
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message.c_str());
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}

void ProcessMessengerMessage_DungeonTimeTimeSetReq(CNetMsg::SP& msg)
{
	int arg1, arg2;
	bool toggle;
	RefMsg(msg) >> arg1 >> arg2;
	if (arg1 < 0 || arg1 > 24)
	{
		return ;
	}

	if (arg2 < 0 || arg2 > 1)
	{
		return ;
	}

	if(arg2 == 0)		toggle = false;
	else if(arg2 == 1)	toggle = true;

	if (arg1 == 24)
	{
		for(int i = 0; i < 24; i++)
		{
			gserver->m_bTimeTable[i] = toggle;
		}
	}

	else
	{
		gserver->m_bTimeTable[arg1] = toggle;
	}
	gserver->SaveDungeonTime(gserver->m_iZoneCount);
}
void ProcessMessengerMessage_DungeonTimeExpSetReq(CNetMsg::SP& msg)
{
	int arg1;
	RefMsg(msg) >> arg1;

	if(arg1 < 100 || arg1 > 600)
		return ;

	for(int i = 0; i < gserver->m_iZoneCount; i++)
	{
		switch(i)
		{
		case ZONE_DUNGEON1:
		case ZONE_DUNGEON2:
		case ZONE_DUNGEON3:
		case ZONE_DUNGEON4:
		case ZONE_EGEHA_DUNGEON_1:
		case ZONE_EGEHA_DUNGEON_8:
		case ZONE_EGEHA_DUNGEON_9:
		case ZONE_EGEHA_DUNGEON_10:
		case ZONE_DRATAN_CASTLE_DUNGEON:
		case ZONE_SPRIT_CAVE:
		case ZONE_QUANIAN_CAVE:
		case ZONE_GOLEM_CAVE:
		case ZONE_FLORAIM_CAVE:
		case ZONE_TRIVIA_CANYON:
		case ZONE_CAPPELLA_2:
		case ZONE_ALTER_OF_DARK:
		case ZONE_AKAN_TEMPLE:
		case ZONE_TARIAN_DUNGEON:
		case ZONE_MISTY_CANYON:
		case ZONE_EBONY_MINE:
			gserver->m_iZoneExp[i] = arg1;
			break;
		default:
			break;
		}
	}
	gserver->SaveDungeonTime(gserver->m_iZoneCount);
}
void ProcessMessengerMessage_DungeonTimeIntervalSetReq(CNetMsg::SP& msg)
{
	int arg1;
	RefMsg(msg) >> arg1;

	if(arg1 < 1 || arg1 > 12)
		return ;

	gserver->m_iTimeInterval = arg1;
	gserver->SaveDungeonTime(gserver->m_iZoneCount);
}
void ProcessMessengerMessage_DungeonTimeToggleSetReq(CNetMsg::SP& msg)
{
	int arg1;
	bool toggle;
	RefMsg(msg) >> arg1;

	if(arg1 < 0 || arg1 > 1)
		return ;

	if (arg1 == 1)		toggle = true;
	else if(arg1 == 0)	toggle = false;

	if(toggle == true)
	{
		if(gserver->m_bDungeonTimeToggle != toggle)
			gserver->ModifyDungeonTime(1);
	}
	else
	{
		if(gserver->m_bDungeonTimeToggle != toggle)
			gserver->ModifyDungeonTime(2);
	}
	gserver->m_bDungeonTimeToggle = toggle;
	gserver->SaveDungeonTime(gserver->m_iZoneCount);
}


void ProcessMessengerMessage_ReloadCatalog( CNetMsg::SP& msg )
{
	//캐쉬아이템 락
	//캐쉬아이템 리로드
	gserver->m_catalogList.Reload();
	//캐쉬아이템 언락
	return;
}
void ProcessMessengerMessage_ModifyCashShop( CNetMsg::SP& msg )
{
	int cmd;
	RefMsg(msg) >> cmd;
	switch(cmd)
	{
	case 0:	// Live 중 CashShop 수정 시작
		{
			// 캐쉬상점 락 클라로 발송
			CNetMsg::SP rmsg(new CNetMsg);
			gserver->m_bCashShopLock = true;
			MsgCashShopLock(rmsg, MSG_EX_CASHITEM_SHOP_LOCK);
			PCManager::instance()->sendToAll(rmsg);
			GAMELOG << init("CASH SHOP LOCK") << end;
		}
		break;
	case 1: // Live 중 CashShop 가격 리로딩
		{
			gserver->m_catalogList.ReloadPrice();
			GAMELOG << init("CATALOG RELOAD PRICE") << end;
		}
		break;
	case 2: // Live 중 CashShop 수정 끝
		{
			// 캐쉬아이템 언락
			CNetMsg::SP rmsg(new CNetMsg);
			gserver->m_bCashShopLock = false;
			MsgCashShopLock(rmsg, MSG_EX_CASHITEM_SHOP_UNLOCK);
			PCManager::instance()->sendToAll(rmsg);
			GAMELOG << init("CASH SHOP UNLOCK") << end;
		}
		break;
	default:
		break;
	}

	return ;
}

void ProcessMessengerMessage_NpcEcho( CNetMsg::SP& msg , int nZone )
{
	static CLCString chat(1000);

	int npcIndex = 0;
	RefMsg(msg) >> npcIndex
				>> chat;

	CNetMsg::SP rmsg(new CNetMsg);
	NpcNoticeMsg(rmsg, MSG_CHAT_NPCNOTICE, npcIndex, chat );

	PCManager::instance()->sendToZone(rmsg, nZone);
}

void ProcessMessengerMessage_ResetRaidNow(CNetMsg::SP& msg)
{
	gserver->m_tRaidResetProcess = mktime(&gserver->m_tRealSystemTime);
	// 첫번째 공지 시작
	// (5 - m_nProcess)
	GAMELOG << init("RAID AUTO RESET REMAIN") << (5-gserver->m_nProcess) << end;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysRaidZoneRemain(rmsg, (5-gserver->m_nProcess));
		PCManager::instance()->sendToAll(rmsg);
	}

	// 첫번째 공지 끝
	gserver->m_tRaidResetProcess += 60;
	gserver->m_nProcess++;
	gserver->m_bIsCheckTime = true;
	return ;
}
void ProcessMessengerMessage_ResetRaidSet(CNetMsg::SP& msg)
{
	int resetday = 0;
	int resethr = 0;
	int resetmin = 0;
	RefMsg(msg) >> resetday >> resethr >> resetmin;
	GAMELOG << init("RAID DUNGEON RESET TIME SET") << "Day" << delim << resetday << delim
			<< "Time " << resethr << delim << resetmin << end;
	// 이제 파일에다 쓰고..
	FILE* fp = fopen(RESET_RAID_SAVEFILE, "wt");
	if(fp)
	{
		fprintf(fp, "%d\n", resetday);
		fprintf(fp, "%d\n", resethr);
		fprintf(fp, "%d\n", resetmin);
		fclose(fp);
		fp = NULL;
		GAMELOG << init("WROTE FILE ABOUT RAID RESET INFORMATION") << end;
	}
	// 멤버변수를 변경해준다.
	gserver->m_nResetDay = resetday;
	gserver->m_nResethr = resethr;
	gserver->m_nResetmin = resetmin;
	return ;
}

void ProcessMessengerMessage_Notice(CNetMsg::SP& msg)
{
	// 클라이언트에게 참가하라는 메시지 보내기
	int cmd;
	RefMsg(msg) >> cmd;
	switch(cmd)
	{
	case 0:				// 참가시간 입니다.
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				RoyalRumbleNotice(rmsg, 0);
				PCManager::instance()->sendToAll(rmsg);
			}
			if(gserver->m_subno == WAR_GROUND_CHANNEL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				RoyalRumbleRegistMenu(rmsg, 1);
				PCManager::instance()->sendToAll(rmsg);
			}
		}
		break;
	case 1:				// 5분 남았습니다.
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RoyalRumbleNotice(rmsg, 1);
			PCManager::instance()->sendToAll(rmsg);
		}
		break;
	case 2:				// 종료되었습니다.
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				RoyalRumbleNotice(rmsg, 2);
				PCManager::instance()->sendToAll(rmsg);
			}
			if(gserver->m_subno == WAR_GROUND_CHANNEL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				RoyalRumbleRegistMenu(rmsg, 0);
				PCManager::instance()->sendToAll(rmsg);
			}
		}
		break;
	default:
		break;
	}
}
void ProcessMessengerMessage_Winner(CNetMsg::SP& msg)
{
	int leveltype;
	int charindex;
	CLCString nick;
	RefMsg(msg) >> leveltype >> charindex >> nick;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleWinner(rmsg, leveltype, charindex, nick);
		PCManager::instance()->sendToAll(rmsg);
	}

	GAMELOG << init("ROYAL RUMBLE WINNER NOTICE") << charindex << delim << nick << delim << "LEVEL TYPE" << delim << leveltype << end;
}

#ifdef SYSTEM_TREASURE_MAP
void ProcMgrMsgTreasureMap( CNetMsg::SP& msg)
{
	// 1. 던전 드랍 설정 [ on | off ]
	// 2. 맵 아이템 드랍 설정 [ on | 0ff ]
	// 3. 드랍할 존 인덱스  [ all | zoneindex .... ]

	int charindex;
	CLCString str(1000);
	RefMsg(msg) >> charindex >> str;

	const char* arg = str;
	char tmpBuf[MAX_STRING_LENGTH] = {0,};
	arg = AnyOneArg(arg, tmpBuf, true);

	if( strcmp(tmpBuf, "on" ) == 0 )
		gserver->bTreasureMapLinkZoneDrop = true;
	else if( strcmp(tmpBuf, "off" ) == 0 )
		gserver->bTreasureMapLinkZoneDrop = false;
	else
		return;

	arg = AnyOneArg(arg, tmpBuf, true);
	int bDrop = false;
	if( strcmp(tmpBuf, "on" ) == 0 )
		bDrop = true;
	else if( strcmp(tmpBuf, "off" ) == 0 )
		bDrop = false;
	else
		return;

	arg = AnyOneArg(arg, tmpBuf, true);

	if( strcmp(tmpBuf, "all" ) == 0 )
	{
		for(int i = 0; i < gserver->m_numZone; i++)
		{
			CZone * pZone = gserver->FindZone(i);
			if( pZone == NULL)
				continue;

			if( !pZone->IsTreasureDropZone() )
				continue;

			if( bDrop )
				pZone->SetTreasureDropFlag();
			else
				pZone->ResetTreasureDropFlag();
		}
	}
	else
	{
		while(1)
		{
			int zone = atoi(tmpBuf);
			CZone* pZone = gserver->FindZone(zone);
			if( pZone == NULL || pZone->IsTreasureDropZone() == false)
			{
				arg = AnyOneArg(arg, tmpBuf, true);
				continue;
			}

			if( bDrop )
				pZone->SetTreasureDropFlag();
			else
				pZone->ResetTreasureDropFlag();

			if (!arg || !(*arg))
				break;

			arg = AnyOneArg(arg, tmpBuf, true);
		}
	}

	gserver->writeTreasureMapSaveFile();
}
#endif

void ProcessMsgrEventActiveUpdate(CNetMsg::SP& msg)
{
	unsigned char  subType;
	int eventIndex;
	int notice;

	RefMsg(msg) >> subType >> eventIndex >> notice;

	int i;

	switch( (int)subType )
	{
	case MSG_EX_EVENT_AUTOMATION_ADD:
		gserver->addActiveEvent( eventIndex );
		gserver->getEventInfo(eventIndex)->setRegenEnable();

		for(i=0; i<5; i++)
		{
			if( gserver->m_aNotice[i] == 0 )
				gserver->m_aNotice[i] = eventIndex;
		}
		// 이벤트에 따라 이벤트 테이블 초기화 할 것.
		// 곰돌이 이벤트(35)		CharDB.t_event_gomdori_2007		UserDB.t_event_gomdori_count
		// 할로윈 이벤트(49)		CharDB.t_event_halloween2007
		// 크리스마스 이벤트(50)	UserDB.t_event_xmastree_point
		if(eventIndex == A_EVENT_BEAR_DOLL || eventIndex == A_EVENT_HALLOWEEN)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_HELPER_COMMAND);
			RefMsg(rmsg) << MSG_HELPER_EVENT_TABLE_INIT << eventIndex;
			SEND_Q(rmsg, gserver->m_helper);
		}
		if(eventIndex == A_EVENT_BEAR_DOLL || eventIndex == A_EVENT_XMAS)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_CONN_REQ);
			RefMsg(rmsg) << (unsigned char)MSG_CONN_EVENT_TABLE_INIT << eventIndex;
			SEND_Q(rmsg, gserver->m_connector);
		}
		break;

	case MSG_EX_EVENT_AUTOMATION_REMOVE:
		gserver->removeActiveEvent( eventIndex );
		gserver->getEventInfo(eventIndex)->setRegenDisable();

		for(i=0; i<5; i++)
		{
			if( gserver->m_aNotice[i] == eventIndex )
				gserver->m_aNotice[i] = 0;
		}

		if(eventIndex == A_EVENT_XMAS)
		{
			// 크리스마스 이벤트 CharDB.t_pet.a_pet_turnto_npc 업데이트 필요
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_HELPER_COMMAND);
			RefMsg(rmsg) << MSG_HELPER_EVENT_TABLE_INIT << eventIndex;
			SEND_Q(rmsg, gserver->m_helper);
		}
		break;
	}

	// EventActive up date Msg
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventActiveUpdateMsg(rmsg, subType, eventIndex);
		PCManager::instance()->sendToAll(rmsg);
	}

	if( notice )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		NoticeInfoMsg(rmsg, eventIndex);
		PCManager::instance()->sendToAll(rmsg);
	}
}

void ProcessMsgrRoyalRumbleSetting(CNetMsg::SP& msg)
{
	int min = 0, max = 0, startHour = 0, charindex = 0;
	RefMsg(msg) >> min >> max >> startHour >> charindex;
	CLCString str(1024);

	if(gserver->m_subno != WAR_GROUND_CHANNEL)
		return;

	if(gserver->m_RoyalRumble.GetRookiePlaying() || gserver->m_RoyalRumble.GetSeniorPlaying() || gserver->m_RoyalRumble.GetMasterPlaying() || gserver->m_RoyalRumble.GetRoyalRumbleTime() != 0)
	{
		str.Format("Royal rumble is processing or playing. Please set later.");
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrSendMsgToCharacterReq(rmsg, charindex, str);
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else
	{
		gserver->m_RoyalRumble.setMinPlayerCount(min);
		gserver->m_RoyalRumble.setMaxPlayerCount(max);
		gserver->m_RoyalRumble.setStartHour(startHour);
		CDBCmd cmd;
		cmd.Init(&gserver->m_dbdata);
		CLCString sql(1024);
		sql.Format("UPDATE t_royalrumble SET a_minPlayer = %d, a_maxPlayer = %d, a_startHour = %d", min, max, startHour);
		cmd.SetQuery(sql);
		if(!cmd.Update())
		{
			str.Format("Royal rumble is set completed. but cannot update table. check your table(t_royalrumble).");
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrSendMsgToCharacterReq(rmsg, charindex, str);
			SEND_Q(rmsg, gserver->m_messenger);

			return ;
		}
		else
		{
			str.Format("Royal rumble is set completed. Minimum Player Count:%d, Maximum Player Count:%d, Start Hour:%d", min, max, startHour);
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrSendMsgToCharacterReq(rmsg, charindex, str);
			SEND_Q(rmsg, gserver->m_messenger);
		}
	}
}

void ProcessMsgrSendMsgToChar(CNetMsg::SP& msg)
{
	int charindex = 0;
	CLCString str(1024);
	RefMsg(msg) >> charindex >> str;
	CPC* pc = NULL;
	pc = PCManager::instance()->getPlayerByCharIndex(charindex);
	if(pc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", str);
		SEND_Q(rmsg, pc->m_desc);
	}
}

void ProcessMessengerMessage_PopupNotice(CNetMsg::SP& msg)
{
	int html_num;
	int runtime;
	int start_time;

	RefMsg(msg) >> html_num
		>> runtime
		>> start_time;
	
	if(runtime == 0)
		return ;

	if(html_num == 0)
		return ;

	if(start_time < 0)
		return ;

	PopupNoticeData* data =  new PopupNoticeData();
	data->html_num = html_num;
	data->runtime = runtime;
	data->isSend = false;

	if(start_time == 0)
	{
		//now
		data->start_time = gserver->getNowSecond() + 60;
	}
	else if(start_time > gserver->getNowSecond())
	{
		data->start_time = start_time;
	}
	else
	{
		delete data;
		data = NULL;
		return ;
	}

	PopupNotice::instance()->_insert(data);
}

void ProcessMessengerMessage_PopupNotice_Close(CNetMsg::SP& msg)
{
	int html_num;

	RefMsg(msg) >> html_num;

	PopupNotice::instance()->_delete(html_num);

	CNetMsg::SP rmsg(new CNetMsg);
	ClosePopupNoticeMsg(rmsg, html_num);
	PCManager::instance()->sendToAll(rmsg);
}

void ProcessMessengerMessage_PopupNotice_Clear(CNetMsg::SP& msg)
{
	PopupNotice::instance()->_clear();

	//DB에서 모두 삭제
	{
		std::string query = boost::str(boost::format("delete from t_popup_notice where a_server = %d and a_subserver = %d") % gserver->m_serverno % gserver->m_subno);
		DBManager::instance()->pushQueryForDataDB(0, query);
	}
	
	CNetMsg::SP rmsg(new CNetMsg);
	PopupNoticeClearMsg(rmsg);
	PCManager::instance()->sendToAll(rmsg);
}
