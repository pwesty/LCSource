#include "stdhdrs.h"
#include "Server.h"
#include "Log.h"
#include "BillingClient.h"

void BillConnectMsg(CNetMsg& msg)
{
	msg.Init(BM_CONNECT);
	msg << SERVER_VERSION
		<< CONNECTOR_SERVER_NAME
		<< gserver.m_serverno;
}

void BillingLoginReqMsg(CNetMsg& msg, int subno, int userindex)
{
	msg.Init(BM_LOGIN_REQ);
	msg << subno
		<< userindex;
}

void BillingLogoutReqMsg(CNetMsg& msg, int subno, int userindex)
{
	msg.Init(BM_LOGOUT_REQ);
	msg << subno
		<< userindex;
}

void BillingUserInfoReqMsg(CNetMsg& msg, int subno, int userindex)
{
	msg.Init(BM_USERINFO_REQ);
	msg << subno
		<< userindex;
}

void BillingSectionListReqMsg(CNetMsg& msg, int subno, int userindex)
{
	msg.Init(BM_SECTION_REQ);
	msg << subno
		<< userindex;
}

void BillingUserRegistReqMsg(CNetMsg& msg, int subno, int userindex)
{
	msg.Init(BM_USER_REG_REQ);
	msg << subno
		<< userindex;
}

void BillingPayReserveReqMsg(CNetMsg& msg, int subno, int userindex)
{
	msg.Init(BM_PAY_RESERVE_REQ);
	msg << subno
		<< userindex;
}

void BillingPayOtherReqMsg(CNetMsg& msg, int subno, int userindex, const char* bguid)
{
	msg.Init(BM_PAY_OTHER_REQ);
	msg << subno
		<< userindex
		<< bguid;
}

void BillInfoItemReqMsg(CNetMsg& msg, int userindex, int charindex)
{
	msg.Init(BM_ITEM_REQ);
	msg << userindex
		<< charindex;
}

void BillSetItemReqMsg(CNetMsg& rmsg, CNetMsg& inmsg)
{
	rmsg.Init(BM_SET_ITEM_REQ);
	
	int userindex, charindex, count, index;

	inmsg >> userindex
		  >> charindex
		  >> count;

	rmsg << userindex
		 << charindex
		 << count;

	GAMELOG << init("BillItem_Give")
			<< userindex << delim
			<< charindex << delim
			<< count << delim;
	for(int i = 0; i < count; i++)
	{
		inmsg >> index;
		rmsg << index;
		GAMELOG << index << delim;
	}
	GAMELOG << end;
}