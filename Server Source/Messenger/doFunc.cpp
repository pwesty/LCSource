#include "stdhdrs.h"

#include "Server.h"
#include "doFunc.h"
#include "CmdMsg.h"

void do_Request(CNetMsg::SP& msg, CDescriptor* dest)
{
	msg->MoveFirst();

	int seq;
	int serverno, subno, zone;
	unsigned char subtype;

	RefMsg(msg) >> seq
				>> serverno >> subno >> zone
				>> subtype;

	switch (subtype)
	{
	case MSG_MSGR_RECOMMEND:
		do_Request_Recommend(msg);
		break;

	case MSG_MSGR_EVENT_GOLDENBALL:
		do_Request_EventGoldenball(msg);
		break;
	}

	CMsgListNode* node = NULL;
	if (seq == -1)
	{
		node = gserver.m_msgList.Add(-1, false, dest, msg);
	}
	else
	{
		node = gserver.m_msgList.Add(gserver.GetNextSeqNo(), true, dest, msg);
	}

	CDescriptor* pDesc;
	CDescriptor* pNext = gserver.m_desclist;
	while ((pDesc = pNext))
	{
		pNext = pDesc->m_pNext;

		if (STATE(pDesc) != CON_PLAYING)
			continue ;

		// serverno가 -1(모두)이거나
		// 해당 서버군 이고
		// 그 서버에 존재하는 존이거나
		// 모든 존이면 추가
		if (((serverno == -1/* && pDesc->m_serverNo != LOGIN_SERVER_NUM && pDesc->m_serverNo != CONNECTOR_SERVER_NUM*/)	// 컨넥터, 로그인서버 제외한
				|| pDesc->m_serverNo == serverno) && (subno == -1 || pDesc->m_subNo == subno) && (zone == -1 || pDesc->FindZone(zone) != -1))
		{
			// 세션에게 메시지 전송, 처리는 각 GameServer가 담당
			SEND_Q(msg, pDesc);

			if (node->m_bReq)
			{
				node->Add(pDesc);
			}
		}
	}

	// 응답이 필요없는 메시지의 경우 저장하지 않고 바로 지움
	if (node->m_bReq == false)
	{
		gserver.m_msgList.Remove(node);
	}
}

void do_Reply(CNetMsg::SP& msg, CDescriptor* dest)
{
	msg->MoveFirst();

	int seq;
	int serverno, subno;
	int zoneno;
	unsigned char subtype;

	RefMsg(msg) >> seq
				>> serverno
				>> subno
				>> zoneno
				>> subtype;

	// seq를 찾고
	CMsgListNode* node;
	CMsgListNode* nodeNext = gserver.m_msgList.m_head;
	CDescriptor* pServer = NULL;
	while ((node = nodeNext))
	{
		nodeNext = node->m_pNext;

		if (node->m_seq == seq)
		{
			bool bAllReceived = true;
			// 서버를 찾고
			CMsgListNodeServerNode* snode;
			CMsgListNodeServerNode* snodeNext = node->m_head;
			while ((snode = snodeNext))
			{
				snodeNext = snode->m_pNext;
				if (snode->m_server->m_serverNo == dest->m_serverNo && snode->m_server->m_subNo == dest->m_subNo)
				{
					// 서버 응답 받음으로 갱신
					snode->m_recieved = true;
					pServer = snode->m_server;
					continue ;
				}

				// 모두 응답 받았는지 검사
				if (!snode->m_recieved)
					bAllReceived = false;
			}

			// 요청한 서버가 없으면
			if (!pServer)
				continue ;

			switch (subtype)
			{
			case MSG_MSGR_WHISPER_REP:
				{
					// 수신자가 있어서 제대로 받았다
					int sidx;
					CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
					CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
					CLCString chat(1000);

					RefMsg(msg)	>> sidx >> sname
								>> rname
								>> chat;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrWhisperRep(rmsg, seq, serverno, subno, zoneno, sidx, sname, rname, chat);
					SEND_Q(rmsg, node->m_reqServer);

					// 제대로 받았으니 지워버리자
					bAllReceived = true;
				}
				break ;

			case MSG_MSGR_WHISPER_NOTFOUND:
				if (bAllReceived)
				{
					// 다 받았는데 귓말 대상이 없었다
					int sidx;
					CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
					RefMsg(msg) >> sidx
								>> sname;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrWhisperNotfoundMsg(rmsg, seq, serverno, subno, zoneno, sidx, sname);
					SEND_Q(rmsg, node->m_reqServer);
				}
				break ;

			case MSG_MSGR_PLAYER_COUNT_REP:
				{
					// 받은 거 그대로 에코
					SEND_Q(msg, node->m_reqServer);
				}
				break;

			case MSG_MSGR_LOGOUT_REP:
				{
					char success;
					CLCString id(MAX_ID_NAME_LENGTH + 1);
					RefMsg(msg) >> success >> id;

					if (success)
					{
						// 로그아웃 한 서버가 있다면 성공한 것
						CNetMsg::SP rmsg(new CNetMsg);
						bAllReceived = true;
						MsgrLogoutRepMsg(rmsg, seq, serverno, subno, zoneno, (char)1, id);
						SEND_Q(rmsg, node->m_reqServer);
					}
					else if (bAllReceived)
					{
						// 다 받아도 없다...
						CNetMsg::SP rmsg(new CNetMsg);
						MsgrLogoutRepMsg(rmsg, seq, serverno, subno, zoneno, (char)0, id);
						SEND_Q(rmsg, node->m_reqServer);
					}
				}
				break ;

			case MSG_MSGR_DOUBLE_EVENT_REP:
				{
					int charindex, tserver, tsub, cmd, nas, nasget, exp, sp, produce, pronum;
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

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrDoubleEventRepMsg(rmsg, seq, serverno, subno, zoneno, charindex, tserver, tsub, cmd, nas, nasget, exp, sp, produce, pronum, start, end);
					SEND_Q(rmsg, node->m_reqServer);
#else // NEW_DOUBLE_GM_AUTO
					CNetMsg::SP rmsg(new CNetMsg);
					MsgrDoubleEventRepMsg(rmsg, seq, serverno, subno, zoneno, charindex, tserver, tsub, cmd, nas, nasget, exp, sp, produce, pronum);
					SEND_Q(rmsg, node->m_reqServer);
#endif // NEW_DOUBLE_GM_AUTO
				}
				break;
#ifdef NEW_DOUBLE_EVENT_AUTO
			case MSG_MSGR_DOUBLE_EVENT_AUTO_REP:
				{
					int charindex, tserver, tsub;
					char cmd, state;
					RefMsg(msg) >> charindex
								>> tserver
								>> tsub
								>> cmd
								>> state;
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
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

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrDoubleEventAutoRepMsg(rmsg, seq, serverno, subno, zoneno, charindex, tserver, tsub, cmd, state, start, end);
					SEND_Q(rmsg, node->m_reqServer);
#else
					CNetMsg::SP rmsg(new CNetMsg);
					MsgrDoubleEventAutoRepMsg(rmsg, seq, serverno, subno, zoneno, charindex, tserver, tsub, cmd, state);
					SEND_Q(rmsg, node->m_reqServer);
#endif
				}
				break;
#endif // NEW_DOUBLE_EVENT_AUTO

			case MSG_MSGR_UPGRADE_EVENT_REP:
				{
					int charindex, tserver, tsub, prob;
					char cmd;

					RefMsg(msg) >> charindex
								>> tserver
								>> tsub
								>> cmd
								>> prob;
#ifdef UPGRADE_EVENT_AUTO
					long start = -1;
					long end = -1;
					RefMsg(msg) >> start
								>> end;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrUpgradeEventRepMsg(rmsg, seq, serverno, subno, zoneno, charindex, tserver, tsub, cmd, prob, start, end);
					SEND_Q(rmsg, node->m_reqServer);
#else
					CNetMsg::SP rmsg(new CNetMsg);
					MsgrUpgradeEventRepMsg(rmsg, seq, serverno, subno, zoneno, charindex, tserver, tsub, cmd, prob);
					SEND_Q(rmsg, node->m_reqServer);
#endif
				}
				break;

			case MSG_MSGR_DOUBLE_EXP_EVENT_REP:
				{
					int charindex, tserver, tsub, cmd, expPercent;
					RefMsg(msg) >> charindex
								>> tserver
								>> tsub
								>> cmd
								>> expPercent;

					if( expPercent < 0 || expPercent > 200 )
						expPercent = 100;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrDoubleExpEventRepMsg(rmsg, seq, serverno, subno, zoneno, charindex, tserver, tsub, cmd, expPercent);
					SEND_Q(rmsg, node->m_reqServer);
				}
				break;
// 태국 오픈베타 메세지
			case MSG_MSGR_OPEN_BETA_EVENT_REP:
			case MSG_MSGR_FLOWER_EVENT_REP:
			case MSG_MSGR_MARGADUM_PVP_REP:
				{
					int charindex, tserver, tsub, cmd, drop;
					RefMsg(msg) >> charindex
								>> tserver
								>> tsub
								>> cmd
								>> drop;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrEventRepMsg(rmsg, subtype, seq, serverno, subno, zoneno, charindex, tserver, tsub, cmd, drop);
					SEND_Q(rmsg, node->m_reqServer);
				}
				break;

			case MSG_MSGR_MESSENGERCHAT_REP:
				{
					// 수신자가 있어서 제대로 받았다
					int sidx;
					CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
					CLCString rname(MAX_CHAR_NAME_LENGTH + 1);
					CLCString chat(1000);

					RefMsg(msg) >> sidx >> sname
								>> rname
								>> chat;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrFriendChatRep(rmsg, seq, serverno, subno, zoneno, sidx, sname, rname, chat);
					SEND_Q(rmsg, node->m_reqServer);

					// 제대로 받았으니 지워버리자
					bAllReceived = true;
				}
				break ;

			case MSG_MSGR_MESSENGERCHAT_NOTFOUND:
				if (bAllReceived)
				{
					// 다 받았는데 귓말 대상이 없었다
					int sidx;
					CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
					RefMsg(msg) >> sidx
								>> sname;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrFriendChatNotfoundMsg(rmsg, seq, serverno, subno, zoneno, sidx, sname);
					SEND_Q(rmsg, node->m_reqServer);
				}
				break ;
#ifdef GMTOOL
			case MSG_MSGR_GMTOOL_KICKID_REP:
				{
					char success;
					int charindex;
					CLCString id(MAX_ID_NAME_LENGTH + 1);
					RefMsg(msg) >> success >> charindex >> id;

					// 로그아웃 한 서버가 있다면 성공한 것
					bAllReceived = true;
					CNetMsg::SP rmsg(new CNetMsg);
					MsgrGmToolKickIDRepMsg(rmsg, seq, serverno, subno, zoneno, success, charindex, id);
					SEND_Q(rmsg, node->m_reqServer);
				}
				break;
			case MSG_MSGR_GMTOOL_KICK_REP:
				{
					char success;
					int charindex;
					CLCString name(MAX_CHAR_NAME_LENGTH + 1);
					RefMsg(msg) >> success >> charindex >> name;

					bAllReceived = true;
					CNetMsg::SP rmsg(new CNetMsg);
					MsgrGmToolKickRepMsg(rmsg, seq, serverno, subno, zoneno, success, charindex, name);
					SEND_Q(rmsg, node->m_reqServer);
				}
				break;
			case MSG_MSGR_GMTOOL_CHATMONITOR_REP:
				{
					char success;
					int gmcharindex;
					CLCString id(MAX_ID_NAME_LENGTH + 1);
					RefMsg(msg) >> success
								>> gmcharindex
								>> id;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrGmToolChatMonitorRepMsg(rmsg, seq, serverno, subno, zoneno, success, gmcharindex, id);
					SEND_Q(rmsg, node->m_reqServer);

					bAllReceived = true;
				}
				break;
			case MSG_MSGR_GM_WHISPER_REP:
				{
					CLCString rname(MAX_ID_NAME_LENGTH + 1);
					CLCString chat(1000);
					int charindex, serverno, subno, sindex;

					RefMsg(msg) >> charindex
								>> rname
								>> chat
								>> serverno
								>> subno
								>> sindex;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrGMWhisperRep(rmsg, seq, 1, 1, 0, charindex, rname, chat, serverno, subno, sindex);
					SEND_Q(rmsg, node->m_reqServer);

					bAllReceived = true;
				}
				break;
			case MSG_MSGR_GMTOOL_WHISPER_REP:
				{
					CLCString sname(MAX_ID_NAME_LENGTH + 1), rname(MAX_ID_NAME_LENGTH + 1);
					CLCString chat(1000);
					int sindex;

					RefMsg(msg) >> sindex
								>> sname
								>> rname
								>> chat;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrGMToolWhisperRep(rmsg, seq, serverno, subno, zoneno, sindex, sname, rname, chat);
					SEND_Q(rmsg, node->m_reqServer);
				}
				break;
			case MSG_MSGR_GM_WHISPER_NOTFOUND:
				{
					int sindex;
					RefMsg(msg) >> sindex;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrGMWhisperNotfoundMsg(rmsg, seq, serverno, subno, zoneno, sindex);
					SEND_Q(rmsg, node->m_reqServer);
				}
				break;
			case MSG_MSGR_GMTOOL_SILENCE_REP:
				{
					CLCString name(MAX_ID_NAME_LENGTH+1);
					int server, sub;
					int success;
					int gmcharindex;

					RefMsg(msg) >> server
								>> sub
								>> success
								>> gmcharindex
								>> name;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrGMSilenceRep(rmsg, seq, serverno, subno, zoneno, server, sub, success, gmcharindex, (const char*)name);
					SEND_Q(rmsg, node->m_reqServer);
				}
				break;
#endif // GMTOOL

#ifdef	DOUBLE_ITEM_DROP
			case MSG_MSGR_DOUBLE_ITEM_EVENT_REP :
				{
					int charindex, tserver, tsub, cmd, itemPercent;
					RefMsg(msg) >> charindex
								>> tserver
								>> tsub
								>> cmd
								>> itemPercent;

					if( itemPercent < 0 || itemPercent > 200 )
						itemPercent = 100;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrDoubleItemEventRepMsg(rmsg, seq, serverno, subno, zoneno, charindex, tserver, tsub, cmd, itemPercent);
					SEND_Q(rmsg, node->m_reqServer);
				}
				break;
#endif // DOUBLE_ITEM_DROP

#ifdef	DOUBLE_PET_EXP
			case MSG_MSGR_DOUBLE_PET_EXP_EVENT_REP:
				{
					int charindex, tserver, tsub, cmd, PetExpPercent;
#ifdef DOUBLE_PET_EXP_AUTO
					long start, end;
#endif
					RefMsg(msg) >> charindex
								>> tserver
								>> tsub
								>> cmd
								>> PetExpPercent;
#ifdef DOUBLE_PET_EXP_AUTO
					RefMsg(msg) >> start
								>> end;
					if( PetExpPercent < 100 || PetExpPercent > 1000 )
						PetExpPercent = 100;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrDoublePetExpEventRepMsg(rmsg, seq, serverno, subno, zoneno, charindex, tserver, tsub, cmd, PetExpPercent, start, end);
					SEND_Q(rmsg, node->m_reqServer);
#else
					if( PetExpPercent < 0 || PetExpPercent > 200 )
						PetExpPercent = 100;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrDoublePetExpEventRepMsg(rmsg, seq, serverno, subno, zoneno, charindex, tserver, tsub, cmd, PetExpPercent);
					SEND_Q(rmsg, node->m_reqServer);
#endif
				}
				break;
#endif // DOUBLE_PET_EXP
#ifdef DOUBLE_ATTACK
			case MSG_MSGR_DOUBLE_ATTACK_EVENT_REP:
				{
					int charindex, tserver, tsub, cmd, AttackPercent;
					RefMsg(msg) >> charindex
								>> tserver
								>> tsub
								>> cmd
								>> AttackPercent;

					if( AttackPercent < 0 || AttackPercent > 200 )
						AttackPercent = 100;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrDoubleAttackEventRepMsg(rmsg, seq, serverno, subno, zoneno, charindex, tserver, tsub, cmd, AttackPercent);
					SEND_Q(rmsg, node->m_reqServer);
				}
				break;
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
			case MSG_MSGR_EVENT_DROPITEM_REP:
				{
					int charindex, npcidx, itemidx, prob, thisServer, thisSub;
					char type;

					RefMsg(msg) >> charindex
								>> type
								>> npcidx
								>> itemidx
								>> prob
								>> thisServer
								>> thisSub;

					if( prob < 1 || prob > 100)
						prob = 1;

					CNetMsg::SP rmsg(new CNetMsg);
					MsgrEventDropItemRepMsg(rmsg, seq, serverno, subno, -1, charindex, type, npcidx, itemidx, prob, thisServer, thisSub);
					SEND_Q(rmsg, node->m_reqServer);
				}
				break;
#endif // EVENT_DROPITEM
			}

			// 다 받았으면 노드에서 제거
			if (bAllReceived)
			{
				gserver.m_msgList.Remove(node);
			}

			break;
		}
	}
}

void do_Request_Recommend(CNetMsg::SP& msg)
{
	int nGMCharIndex;
	int nRecommendServer;
	RefMsg(msg) >> nGMCharIndex
				>> nRecommendServer;

	gserver.m_nRecommendServer = nRecommendServer;

	FILE* fp = fopen(FILENAME_RECOMMEND, "wb");
	if (fp)
	{
		fwrite(&gserver.m_nRecommendServer, sizeof(gserver.m_nRecommendServer), 1, fp);
		fclose(fp);
	}
}

void do_Request_EventGoldenball(CNetMsg::SP& msg)
{
	int			nSubtype;
	int			nGMCharIndex;
	CLCString	strTeam1(GOLDENBALL_TEAM_NAME_LENGTH + 1);
	CLCString	strTeam2(GOLDENBALL_TEAM_NAME_LENGTH + 1);
	int			nTeam1Score = 0;
	int			nTeam2Score = 0;
	int			nYear = 0;
	int			nMonth = 0;
	int			nDay = 0;
	int			nHour = 0;
	int			nMin = 0;
	int			nEndVoteTime = 0;

	struct tm	tmEndVoteTime;

	RefMsg(msg) >> nSubtype;

	switch (nSubtype)
	{
	case MSG_MSGR_EVENT_GOLDENBALL_VOTE:
		{
			// 골든볼 이벤트 응모 설정	: gmcharindex(n) team1(str) team2(str) year(n) month(n) day(n) hour(n) minute(n)
			RefMsg(msg) >> nGMCharIndex
						>> strTeam1
						>> strTeam2
						>> nYear
						>> nMonth
						>> nDay
						>> nHour
						>> nMin;
			gserver.m_sGoldenBallData.nStatus = GOLDENBALL_STATUS_VOTE;
			strcpy(gserver.m_sGoldenBallData.strTeam1, strTeam1);
			strcpy(gserver.m_sGoldenBallData.strTeam2, strTeam2);
			gserver.m_sGoldenBallData.nTeam1Score = 0;
			gserver.m_sGoldenBallData.nTeam2Score = 0;
			gserver.m_sGoldenBallData.nYear = nYear;
			gserver.m_sGoldenBallData.nMonth = nMonth;
			gserver.m_sGoldenBallData.nDay = nDay;
			gserver.m_sGoldenBallData.nHour = nHour;
			gserver.m_sGoldenBallData.nMin = nMin;

			memset(&tmEndVoteTime, 0, sizeof(tmEndVoteTime));
			tmEndVoteTime.tm_year	= nYear - 1900;
			tmEndVoteTime.tm_mon	= nMonth - 1;
			tmEndVoteTime.tm_mday	= nDay;
			tmEndVoteTime.tm_hour	= nHour;
			tmEndVoteTime.tm_min	= nMin;
			tmEndVoteTime.tm_isdst	= -1;
			gserver.m_sGoldenBallData.timeEndVote = mktime(&tmEndVoteTime);
		}
		break;

	case MSG_MSGR_EVENT_GOLDENBALL_GIFT:
		{
			// 골든볼 이벤트 보상 설정	: gmcharindex(n) team1(str) team1score(n) team2(str) team2score(n) year(n) month(n) day(n) hour(n) minute(n) endVoteTime(n)
			RefMsg(msg) >> nGMCharIndex
						>> strTeam1
						>> nTeam1Score
						>> strTeam2
						>> nTeam2Score
						>> nYear
						>> nMonth
						>> nDay
						>> nHour
						>> nMin
						>> nEndVoteTime;
			gserver.m_sGoldenBallData.nStatus = GOLDENBALL_STATUS_GIFT;
			strcpy(gserver.m_sGoldenBallData.strTeam1, strTeam1);
			strcpy(gserver.m_sGoldenBallData.strTeam2, strTeam2);
			gserver.m_sGoldenBallData.nTeam1Score = nTeam1Score;
			gserver.m_sGoldenBallData.nTeam2Score = nTeam2Score;
			gserver.m_sGoldenBallData.nYear = nYear;
			gserver.m_sGoldenBallData.nMonth = nMonth;
			gserver.m_sGoldenBallData.nDay = nDay;
			gserver.m_sGoldenBallData.nHour = nHour;
			gserver.m_sGoldenBallData.nMin = nMin;
		}
		break;

	case MSG_MSGR_EVENT_GOLDENBALL_END:
		{
			// 골든볼 이벤트 종료		: gmcharindex(n)
			RefMsg(msg) >> nGMCharIndex;
			memset(&gserver.m_sGoldenBallData, 0, sizeof(gserver.m_sGoldenBallData));
			gserver.m_sGoldenBallData.nStatus = GOLDENBALL_STATUS_NOTHING;
		}
		break;
	}

	FILE* fp = fopen(FILENAME_GOLDENBALL, "wb");
	if (fp)
	{
		fwrite(&gserver.m_sGoldenBallData, sizeof(gserver.m_sGoldenBallData), 1, fp);
		fclose(fp);
	}
}