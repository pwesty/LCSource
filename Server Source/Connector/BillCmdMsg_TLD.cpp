#include "stdhdrs.h"

#ifdef USE_TENTER_BILLING

#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "CmdMsg.h"

void BillConnectMsg(CNetMsg::SP& msg)
{
	msg->Init(BM_CONNECT);

	//BM_CONNECT,								// 目池磐 立加 夸没: protocolversion(str:17) serverNum(uc) gameId(uc)
	RefMsg(msg) << BILL_SERVER_VERSION
				<< (unsigned char) gserver.m_serverno
				<< (unsigned char) GAMEID;
	//<< (unsigned char) 3;
}

void BillBalanceReqMsg(CNetMsg::SP& msg, int userindex, const char* idname)
{
	msg->Init(MSG_BILLITEM_CASHBALANCE_REQ);

	RefMsg(msg) << (unsigned char) GAMEID
				<< userindex
				<< idname
				<< (int) CPID;
}
void BillCanbuyReqMsg(CNetMsg::SP& msg, int userindex, const char* idname, char count, int ctid[], int requestCash)
{
	msg->Init(MSG_BILLITEM_CANBUY_REQ);
	RefMsg(msg) << (unsigned char) GAMEID
				<< userindex
				<< idname
				<< (int) CPID
				<< count;

	for(int i = 0; i < count && i < MAX_PURCHASEITEM; i++)
	{
		RefMsg(msg) << ctid[i];
	}
	RefMsg(msg) << requestCash;
}

void BillBuyReqMsg(CNetMsg::SP& msg, int userindex, const char* userid, int serial, char count, int ctid[], const char* ip)
{
	msg->Init(MSG_BILLITEM_BUY_REQ);
	RefMsg(msg) << serial
				<< (unsigned char) GAMEID
				<< userindex
				<< userid
				<< (int) CPID
				<< ip
				<< count;

	for(int i = 0; i < count && i < MAX_PURCHASEITEM; i++)
	{
		RefMsg(msg) << ctid[i];
	}
}

#endif