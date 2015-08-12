#define __SEVER_CPP__

#include "stdhdrs.h"

#include "Server.h"
#include "Cmd.h"
#include "CmdMsg.h"
#include "../ShareLib/packetType/ptype_server_to_server.h"
#include "../ShareLib/LogInOutManager.h"

CServer gserver;
CCmdList gcmdlist;

CServer::CServer()
{
	m_desclist = NULL;
	m_serverpath = GetServerPath();

	m_nRecommendServer = -1;

	memset(&m_sGoldenBallData, 0, sizeof(m_sGoldenBallData));
	m_sGoldenBallData.nStatus = GOLDENBALL_STATUS_NOTHING;
}

CServer::~CServer()
{
	if (m_serverpath)
		delete[] m_serverpath;
}

char* CServer::GetServerPath()
{
	char szBuffer[1024];
#ifdef CIRCLE_WINDOWS
	GetModuleFileName(::GetModuleHandle(NULL), szBuffer, 1000);
	int path_len = strlen(szBuffer);
	int i;

	for (i = path_len - 1; i >= 0; i-- )
	{
		if (szBuffer[i] == '\\')
		{
			szBuffer[i+1] = '\0';
			break;
		}
	}
	if (i < 0)
		return false;
#else
	getcwd (szBuffer, 512);
	strcat (szBuffer, "/");
#endif

	char* ret = new char[strlen(szBuffer) + 1];
	strcpy(ret, szBuffer);
	return ret;
}

bool CServer::LoadSettingFile()
{
	LOG_INFO("Load setting file....");

	CLCString strTemp(1024);

	strTemp.Format("%s%s", m_serverpath, "newStobm.bin");

	if (!m_config.Load(strTemp))
		return false;

	return true;
}

bool CServer::InitGame()
{
	FILE* fpRecommend = fopen(FILENAME_RECOMMEND, "rb");
	if (fpRecommend)
	{
		if (fread(&gserver.m_nRecommendServer, sizeof(gserver.m_nRecommendServer), 1, fpRecommend) != 1)
			gserver.m_nRecommendServer = -1;
		fclose(fpRecommend);
		fpRecommend = NULL;
	}
	LOG_INFO("SYSTEM > Load Recommend Server Data %d", gserver.m_nRecommendServer);

	LOG_INFO("SYSTEM > Load Golden Ball Data");
	FILE* fpGoldenBall = fopen(FILENAME_GOLDENBALL, "rb");
	if (fpGoldenBall)
	{
		if (fread(&gserver.m_sGoldenBallData, sizeof(gserver.m_sGoldenBallData), 1, fpGoldenBall) != 1)
		{
			memset(&gserver.m_sGoldenBallData, 0, sizeof(gserver.m_sGoldenBallData));
			gserver.m_sGoldenBallData.nStatus = GOLDENBALL_STATUS_NOTHING;
		}
		fclose(fpGoldenBall);
	}

	return true;
}

void CServer::CommandInterpreter(CDescriptor* d, CNetMsg::SP& msg)
{
	if (msg->m_mtype == MSG_SERVER_TO_SERVER)
	{
		pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);
		switch (pBase->subType)
		{
		case MSG_SUB_SERVERTOSERVER_USER_LOGIN:
// 			LogInOutManager::Instance()->Add(d->service_, msg);
			break;

		case MSG_SUB_SERVERTOSERVER_USER_LOGOUT:
// 			LogInOutManager::Instance()->Delete(msg);
			break;

		default:
			LOG_ERROR("SubHelper : invalid subtye[%d]", pBase->subType);
			break;
		}
	}
	else if (gcmdlist.Run(msg->m_mtype, msg, d) == false) // invalid command..
	{
		LOG_INFO("SYS_ERR > Invalid command: %d", msg->m_mtype);
	}
}

void CServer::operate( rnSocketIOService* service )
{
	// 새로운 클라이언트가 accept되었을때 처리하는 부분
	CDescriptor* newd = CDescriptor::create(service);
	service->SetUserData((void *)newd);

	STATE(newd) = CON_GET_LOGIN;

	ADD_TO_BILIST(newd, m_desclist, m_pPrev, m_pNext);

	LOG_INFO("CLIENT CONNECTED... IP[%s]", service->ip().c_str());
}

void CServer::ProcTimeout(CMsgListNode* msgNode)
{
	int seq, server, subno, zone;
	unsigned char subtype;

	CNetMsg::SP& msg = msgNode->m_msg;
	msg->MoveFirst();
	RefMsg(msg) >> seq
				>> server >> subno >> zone
				>> subtype;

	LOG_INFO("TIMEOUT server : %d : sub : %d : messege : %d : subtype %d",
			 server, subno, msg->m_mtype, subtype);

	// 요청했던 서버 찾기
	CDescriptor* d;
	CDescriptor* dNext = m_desclist;

	while ((d = dNext))
	{
		dNext = d->m_pNext;

		if (d == msgNode->m_reqServer)
			break;
	}

	if (d)
	{
		// 메시지에 따라 다른 처리
		switch (subtype)
		{
		case MSG_MSGR_WHISPER_REQ:
			{
				int sidx;
				CLCString sname(MAX_CHAR_NAME_LENGTH + 1);
				RefMsg(msg) >> sidx >> sname;

				CNetMsg::SP rmsg(new CNetMsg);
				MsgrWhisperNotfoundMsg(rmsg, seq, server, subno, zone, sidx, sname);
				SEND_Q(rmsg, msgNode->m_reqServer);
			}
			break;

		case MSG_MSGR_LOGOUT_REQ:
			{
				CLCString id(MAX_ID_NAME_LENGTH + 1);
				RefMsg(msg) >> id;

				CNetMsg::SP rmsg(new CNetMsg);
				MsgrLogoutRepMsg(rmsg, seq, server, subno, zone, (char)0, id);
				SEND_Q(rmsg, msgNode->m_reqServer);
			}
			break;
		}
	}

	// 노드 지우기
	REMOVE_FROM_BILIST(msgNode, m_msgList.m_head, m_pPrev, m_pNext);
	delete msgNode;
}

//////////////////////////////////////////////////////////////////////////
// 이 함수는 매 1초마다 호출됨
void ClearMessageListTimer::operate( rnSocketIOService* service )
{
	CMsgListNode* nowNode = NULL;
	CMsgListNode* nextNode = gserver.m_msgList.m_head;
	while (nowNode = nextNode)
	{
		nextNode = nowNode->m_pNext;

		++nowNode->m_sendTime;
		if (nowNode->m_sendTime > 5)
		{
			gserver.ProcTimeout(nowNode);
		}
	}
}

ClearMessageListTimer* ClearMessageListTimer::instance()
{
	static ClearMessageListTimer __instance;
	return &__instance;
}