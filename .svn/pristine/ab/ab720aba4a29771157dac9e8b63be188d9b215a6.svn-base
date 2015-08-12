#include "stdhdrs.h"

#include "CmdMsg.h"
#include "Server.h"
#include "../ShareLib/DBCmd.h"

void FailMsg(CNetMsg::SP& msg, MSG_FAIL_TYPE failtype)
{
	msg->Init(MSG_FAIL);
	RefMsg(msg) << (unsigned char)failtype;
}

void LogoutReqMsg(CNetMsg::SP& msg, int server, int subnum, int zone, CDescriptor* d)
{
	msg->Init(MSG_MSGR_REQ);

	RefMsg(msg) << 0
				<< server
				<< subnum
				<< zone
				<< (unsigned char)MSG_MSGR_LOGOUT_REQ
				<< d->m_idname
				<< d->m_passwd;
}

void PlayerReqMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_CONN_REQ);

	RefMsg(msg) << (unsigned char)MSG_CONN_PLAYER_REQ;
}

void PlayerNumMsg(CNetMsg::SP& msg, int recentServer, int recentSubNum, int idx)
{
	msg->Init(MSG_LOGINSERV_PLAYER);

	RefMsg(msg) << recentServer
				<< recentSubNum
				<< gserver.m_nConnector
				<< (idx + 1);

	if (gserver.m_connector[idx]->isRun())
	{
		RefMsg(msg) << gserver.m_connector[idx]->m_serverNo
					<< gserver.m_connector[idx]->m_nMaxServer;

		if (gserver.m_connector[idx]->m_hardcore_flag != 0)
		{
			RefMsg(msg) << (char)2;
		}
		else if (gserver.m_connector[idx]->m_serverNo == gserver.m_nRecommendServer)
			RefMsg(msg) << (char)1;
		else
			RefMsg(msg) << (char)0;

		int i;
		for (i=0; i < gserver.m_connector[idx]->m_nMaxServer; i++)
		{
			RefMsg(msg) << gserver.m_connector[idx]->m_serverSubNo[i]
						<< gserver.m_connector[idx]->m_playerNum[i] + 1999
						<< gserver.m_connector[idx]->m_ipAddr[i]
						<< gserver.m_connector[idx]->m_portNumber[i];
		}
	}
	else
	{
		char tmpBuf[100] = {0,};
		sprintf(tmpBuf, "Connector_%d", idx);
		int sno = atoi(gserver.m_config.Find(tmpBuf, "Number"));
		int max = atoi(gserver.m_config.Find(tmpBuf, "MaxSubNumber"));
		RefMsg(msg) << sno
					<< max;

		if (sno == gserver.m_nRecommendServer)
			RefMsg(msg) << (char)1;
		else
			RefMsg(msg) << (char)0;

		int i;
		for (i=0; i < gserver.m_connector[idx]->m_nMaxServer; i++)
		{
			int subno = i + 1;
			int playernum = -1;
			int port = 0;
			RefMsg(msg) << subno
						<< playernum + 1999
						<< ""
						<< port;
		}
	}
}
