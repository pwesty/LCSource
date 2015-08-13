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

	CMsgListNode* node;
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
			case MSG_HELPER_WHISPER_REP:
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
					HelperWhisperRepMsg(rmsg, seq, serverno, subno, zoneno, sidx, sname, rname, chat);
					SEND_Q(rmsg, node->m_reqServer);

					// 제대로 받았으니 지워버리자
					bAllReceived = true;
				}
				break ;

			case MSG_HELPER_WHISPER_NOTFOUND:
				if (bAllReceived)
				{
					// 다 받았는데 귓말 대상이 없었다
					int sidx;
					CLCString sname(MAX_CHAR_NAME_LENGTH + 1);

					RefMsg(msg) >> sidx
								>> sname;

					CNetMsg::SP rmsg(new CNetMsg);
					HelperWhisperNotfoundMsg(rmsg, seq, serverno, subno, zoneno, sidx, sname);
					SEND_Q(rmsg, node->m_reqServer);
				}
				break ;
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
//