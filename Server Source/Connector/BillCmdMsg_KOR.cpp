#include "stdhdrs.h"

#ifndef USE_TENTER_BILLING

#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "CmdMsg.h"

void BillConnectMsg(CBPacket::SP& msg)
{
	CLCString protocolVersion(17);

	protocolVersion.Format("%s", BILL_SERVER_VERSION);

	msg->Init(BM_CONNECT);
	RefMsg(msg) << protocolVersion
				<< (unsigned char) gserver.m_serverno
				<< (unsigned char) GAMEID;
}

void BillBalanceReqMsg(CBPacket::SP& msg, int userindex, const char* idname)
{
	CLCString id(51);
	id.Format("%s", idname);

	msg->Init(MSG_BILLITEM_CASHBALANCE_REQ);

	RefMsg(msg) << id
				<< (unsigned char) CPID
				<< userindex;
}

void BillCanbuyReqMsg(CBPacket::SP& msg, int userindex, const char* idname, int ctid[], char count, const char* ip)
{
	CLCString id(51);
	id.Format("%s", idname);
	CLCString lcIP(16);
	lcIP.Format("%s", ip);

	msg->Init(MSG_BILLITEM_CANBUY_REQ);

	RefMsg(msg) << id
				<< (unsigned char) CPID
				<< userindex
				<< lcIP
				<< (unsigned char) count;

	for(int i = 0; i < MAX_PURCHASEITEM && i < count; i++)
	{
		RefMsg(msg) << ctid[i];
	}
}

void BillBuyReqMsg(CBPacket::SP& msg, int userindex, const char* userid, int serial, int ctid[], const char* ip, char count)
{
	msg->Init(MSG_BILLITEM_BUY_REQ);
	msg->m_serial = serial;

	CLCString id(51);
	id.Format("%s", userid);
	CLCString lcIP(16);
	lcIP.Format("%s", ip);

	RefMsg(msg) << id
				<< (unsigned char) CPID
				<< userindex
				<< lcIP
				<< (unsigned char) count;

	for(int i = 0; i < MAX_PURCHASEITEM && i < count; i++)
	{
		RefMsg(msg) << ctid[i];
	}
}

#endif