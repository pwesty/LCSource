#include "stdhdrs.h"
#include "Server.h"
#include "CmdMsg.h"


//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )
void SetItemRepMsg(CNetMsg& rmsg, int userindex, int subno, int charindex, MSG_BBILLINFO_ITEM_ERROR_TYPE subtype)
{
	rmsg.Init(BM_SET_ITEM_REP);
	rmsg << userindex
		 << subno
		 << charindex
		 << (char) subtype;
}
void SupItemRepMsg(CNetMsg& rmsg, int charindex, bool isstart, CSupItem ** supItem )
{
	// listmsg
	// listflag userindex charindex count a_index itemindex itemflag itemplus itemcount 

	int listflag = 0, userindex;

	if(isstart)
		listflag |= (1 << 0);

	userindex = (*supItem)->userId();
	rmsg.Init(BM_ITEM_REP);

	CNetMsg temp;
	temp.Init();

	for(int i = 0; i < 20 && (*supItem); i++)
	{
		temp << (*supItem)->index()
			 << (*supItem)->itemIndex()
			 << (*supItem)->itemFlag()
			 << (*supItem)->itemPlus()
			 << (*supItem)->itemCount();

		GAMELOG << init("BillItem_List");
		GAMELOG << (*supItem)->index() << delim
				<< (*supItem)->catalId() << delim
				<< (*supItem)->server() << delim
				<< (*supItem)->userId() << delim
				<< (*supItem)->itemIndex() << delim
				<< (*supItem)->itemFlag() << delim
				<< (*supItem)->itemPlus() << delim
				<< (*supItem)->itemCount() << delim
				<< i 
				<< end;

		(*supItem) = (*supItem)->next();
	}

	if( !(*supItem) ) listflag |= (1 << 1);

	rmsg << listflag
		 << userindex
		 << charindex;

	rmsg << i << temp;
}

void LoginRepMsg(CNetMsg& msg, BM_LOGIN_REP_ERROR_TYPE errcode, int subno, int userindex, CUser* user)
{
// TODO : BILLING 결과에 관계 없이 무조건 OK
//	time_t curtime;
//	time(&curtime);
//	// 1일 사용 가능으로 24 시간 * 60분 * 60초
//	curtime += 24 * 60 * 60;
//	struct tm* curtm;
//	curtm = localtime(&curtime);
//
//	msg.Init(BM_LOGIN_REP);
//	msg << subno
//		<< userindex
//		<< BM_LOGIN_REP_ERROR_OK
//		<< "FREEMODECONNECTED"
//		<< "FREEMODE"
//		<< 50
//		<< 0
//		<< curtm->tm_year + 1900
//		<< curtm->tm_mon + 1
//		<< curtm->tm_mday
//		<< curtm->tm_hour
//		<< curtm->tm_min
//		<< 24 * 60 * 60
//		<< 0
//		<< ""
//		<< 0
//		<< "";
// --- TODO : BILLING 결과에 관계 없이 무조건 OK

// 원래꺼
	msg.Init(BM_LOGIN_REP);
	msg << subno
		<< userindex
		<< errcode;
	if (user)
	{
		msg << user->billguid()
			<< user->billviewname()
			<< user->billsection()
			<< user->surplus()
			<< user->endtime_year()
			<< user->endtime_mon()
			<< user->endtime_day()
			<< user->endtime_hour()
			<< user->endtime_min()
			<< user->RemainTime();

		if (user->reserve_found())
		{
			msg << 1
				<< user->reserve_billguild()
				<< user->reserve_billvalue()
				<< user->reserve_billname();
		}
		else
		{
			msg << 0
				<< ""
				<< 0
				<< "";
		}
	}
	else
	{
		msg << ""
			<< ""
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< ""
			<< 0
			<< "";
	}
}

void UserInfoRepMsg(CNetMsg& msg, BM_USERINFO_ERROR_TYPE errcode, int subno, int userindex, CUser* user, int iswarn, int istimeout)
{
// TODO : BILLING 결과에 관계 없이 무조건 OK
//	time_t curtime;
//	time(&curtime);
//	// 1일 사용 가능으로 24 시간 * 60분 * 60초
//	curtime += 24 * 60 * 60;
//	struct tm* curtm;
//	curtm = localtime(&curtime);
//
//	msg.Init(BM_USERINFO_REP);
//	msg << subno
//		<< userindex
//		<< BM_USERINFO_ERROR_OK
//		<< "FREEMODECONNECTED"
//		<< "FREEMODE"
//		<< 50
//		<< 0
//		<< curtm->tm_year + 1900
//		<< curtm->tm_mon + 1
//		<< curtm->tm_mday
//		<< curtm->tm_hour
//		<< curtm->tm_min
//		<< 24 * 60 * 60
//		<< 0
//		<< ""
//		<< 0
//		<< ""
//		<< 0
//		<< 0;
// --- TODO : BILLING 결과에 관계 없이 무조건 OK

// 원래꺼
	msg.Init(BM_USERINFO_REP);
	msg << subno
		<< userindex
		<< errcode;
	if (user)
	{
		msg << user->billguid()
			<< user->billviewname()
			<< user->billsection()
			<< user->surplus()
			<< user->endtime_year()
			<< user->endtime_mon()
			<< user->endtime_day()
			<< user->endtime_hour()
			<< user->endtime_min()
			<< user->RemainTime();

		if (user->reserve_found())
		{
			msg << 1
				<< user->reserve_billguild()
				<< user->reserve_billvalue()
				<< user->reserve_billname();
		}
		else
		{
			msg << 0
				<< ""
				<< 0
				<< "";
		}
	}
	else
	{
		msg << ""
			<< ""
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< ""
			<< 0
			<< "";
	}
	msg << iswarn
		<< istimeout;
}


void SectionRepMsg(CNetMsg& msg, int subno, int userindex, int flag, CSectionInfo* section)
{
	msg.Init(BM_SECTION_REP);
	msg << subno
		<< userindex
		<< flag;
	if (section)
	{
		msg << section->billguid()
			<< section->billsection()
			<< section->billhour()
			<< section->billvalue()
			<< section->billviewname();
	}
	else
	{
		msg << ""
			<< 0
			<< 0
			<< 0
			<< "";
	}
}


void PayRepMsg(CNetMsg& msg, BM_PAY_ERROR_TYPE errcode, int subno, int userindex, CUser* user, int isauto)
{
	msg.Init(BM_PAY_REP);
	msg << subno
		<< userindex
		<< errcode;
	if (user)
	{
		msg << user->billguid()
			<< user->billviewname()
			<< user->billsection()
			<< user->surplus()
			<< user->endtime_year()
			<< user->endtime_mon()
			<< user->endtime_day()
			<< user->endtime_hour()
			<< user->endtime_min()
			<< user->RemainTime();

		if (user->reserve_found())
		{
			msg << 1
				<< user->reserve_billguild()
				<< user->reserve_billvalue()
				<< user->reserve_billname();
		}
		else
		{
			msg << 0
				<< ""
				<< 0
				<< "";
		}
	}
	else
	{
		msg << ""
			<< ""
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< 0
			<< ""
			<< 0
			<< "";
	}
	msg << isauto;
}


void UserRegRepMsg(CNetMsg& msg, int subno, int userindex, BM_USER_REG_ERROR_TYPE errcode)
{
	msg.Init(BM_USER_REG_REP);
	msg << subno
		<< userindex
		<< errcode;
}

#else

void CashBalanceRepMsg(CNetMsg& msg, BILL_ERROR_MESSAGE_TYPE errcode, char gameID, int userindex, int cpID, int balance)
{
	msg.Init(MSG_BILLITEM_CASHBALANCE_REP);
	msg << (unsigned char) errcode
		<< gameID
		<< userindex
		<< cpID
		<< balance;
}

void CanBuyRepMsg(CNetMsg& msg, BILL_ERROR_MESSAGE_TYPE errcode, char gameID, int userindex, int cpID, char count, const int itemID[], int balance)
{
	msg.Init(MSG_BILLITEM_CANBUY_REP);
	msg << (unsigned char) errcode
		<< gameID
		<< userindex
		<< cpID
		<< count;

	for(int i = 0; i < count; i++)
		msg << itemID[i];
	
	msg << balance;
}

void BuyRepMsg(CNetMsg& msg, BILL_ERROR_MESSAGE_TYPE errcode, int serial, char gameID, int userindex, int cpID, char count, const int itemID[], int balance)
{
	msg.Init(MSG_BILLITEM_BUY_REP);
	msg << serial
		<< (unsigned char) errcode
		<< gameID
		<< userindex
		<< cpID
		<< count;

	for(int i = 0; i < count; i++)
		msg << itemID[i];
	
	msg << balance;
}

#endif // #ifndef LC_TLD

