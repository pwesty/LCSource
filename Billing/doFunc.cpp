#include "stdhdrs.h"
#include "Server.h"
#include "doFunc.h"
#include "CmdMsg.h"

//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )
bool ReadCurrentInfo(int userindex, CUser& user, bool bForce);
void ProcessLogout(CDescriptor* desc, int userindex);
bool ReadSectionInfo(const char* bguid, CSectionInfo& section);
bool PayBillGuid(CUser* user, const char* bguid);
bool ReserveSection(CUser* user, const char* bguid);
bool ReadSupItem(int userindex, CSupItemList &list, int serverGroup);
bool UpdateSupItem(CSupUpdateItemList& supUpdateItemList);



// 업데이트 처리
void do_SetItemReq(CNetMsg& inmsg, CDescriptor* desc)
{
	int userindex, charindex, count, index;

	inmsg >> userindex
		  >> charindex
		  >> count;

	CSupUpdateItemList updateItemList;

	for(int i = 0; i < count; i++)
	{
		inmsg >> index;

		updateItemList.Add(index);
	}

	if( !UpdateSupItem(updateItemList) )
	{
		GAMELOG << init("BillItem_Give_Fail-DB_ERROR")
				<< userindex
				<< charindex
				<< end;

		CNetMsg rmsg;
		SetItemRepMsg(rmsg, userindex, desc->m_serverNo, charindex, MSG_BBILLINFO_ERROR_BILL);
		SEND_Q(rmsg, desc);	
		return;
	}

	CNetMsg rmsg;
	SetItemRepMsg(rmsg, userindex, desc->m_serverNo, charindex, MSG_BBILLINFO_ERROR_SUCCESS);
	SEND_Q(rmsg, desc);	

}
void do_ItemReq(CNetMsg& msg, CDescriptor* desc)
{
		/////////////////////////////////////////////
	// BANGWALL : 2005-07-05 오후 9:17:54
	// Comment : item을 검색하여 있으면 커넥터로 보냄.
	// where userindex and date = '';

	int userindex, charindex;

	msg >> userindex
		>> charindex;

	CSupItemList supItemList;
	if(!ReadSupItem(userindex, supItemList, desc->m_serverNo - 10))
	{
		GAMELOG << init("supItemList Get Error - DATABASE LOADING") 
				<< userindex
				<< end;
		CNetMsg rmsg;
		SetItemRepMsg(rmsg, userindex, desc->m_serverNo, charindex, MSG_BBILLINFO_ERROR_BILL);
		SEND_Q(rmsg, desc);	
		return;
	}

	CSupItem * supItem = supItemList.head();
	if(!supItem)
	{
		GAMELOG << init("supItemList Get Error") 
				<< userindex
				<< end;

		CNetMsg rmsg;
		SetItemRepMsg(rmsg, userindex, desc->m_serverNo, charindex, MSG_BBILLINFO_ERROR_NOTLIST);
		SEND_Q(rmsg, desc);	
		return;
	}
	else
	{
		

		while(supItem)
		{
			CNetMsg rmsg;
			SupItemRepMsg(rmsg, charindex, (supItemList.head()->index() == supItem->index() ) ? true : false, &supItem);
			SEND_Q(rmsg, desc);
		}
	}
}

void do_LoginReq(CNetMsg& msg, CDescriptor* desc)
{

#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CNetMsg rmsg;

	msg.MoveFirst();

	int subno = -1;
	int userindex = -1;

	msg >> subno
		>> userindex;

	CUser data;

	if (!ReadCurrentInfo(userindex, data, true))
	{
		GAMELOG << init("LOGIN FAIL")
				<< "CANNOT READ CURRENT INFO" << delim
				<< "USERINDEX" << delim
				<< userindex
				<< end;
		LoginRepMsg(rmsg, BM_LOGIN_REP_ERROR_NOTFOUND, subno, userindex, NULL);
		SEND_Q(rmsg, desc);
		return ;
	}

	if (data.billsection() >= BILL_SECTION_FREE)
	{
		CSectionInfo section;
		if (!ReadSectionInfo(data.billguid(), section) || section.billevent() != 1)
		{
			GAMELOG << init("LOGIN FAIL")
					<< "NOT ALLOW FREE USER" << delim
					<< "USERINDEX" << delim
					<< userindex << delim
					<< "BILL_GUID" << delim
					<< data.billguid()
					<< end;
			LoginRepMsg(rmsg, BM_LOGIN_REP_ERROR_NOFREEEVENT, subno, userindex, NULL);
			SEND_Q(rmsg, desc);
			return ;
		}
	}

	CUser* user = desc->m_userlist.Add(subno, userindex, data.billguid(), data.billsection(), data.billviewname(), data.endtime(), data.surplus(), data.reserve_found(), data.reserve_billguild(), data.reserve_billvalue(), data.reserve_billname(), gserver.m_pulse);
	if (!user)
	{
		GAMELOG << init("LOGIN FAIL")
				<< "CANNOT ADD USER LIST" << delim
				<< "USERINDEX" << delim
				<< userindex
				<< end;
		LoginRepMsg(rmsg, BM_LOGIN_REP_ERROR_UNKNOWN, subno, userindex, NULL);
		SEND_Q(rmsg, desc);
		return ;
	}

	user->bSent3Day(data.bSent3Day());
	user->bSent1Day(data.bSent1Day());
	user->bSent60(data.bSent60());
	user->bSent30(data.bSent30());
	user->bSent10(data.bSent10());
	user->bSent5(data.bSent5());
	user->bSent1(data.bSent1());
	user->bSendTimeOut(data.bSendTimeOut());

	GAMELOG << init("LOGIN SUCCESS")
			<< "USERINDEX" << delim
			<< userindex << delim
			<< "BILL_GUID" << delim
			<< data.billguid()
			<< end;

	LoginRepMsg(rmsg, BM_LOGIN_REP_ERROR_OK, subno, userindex, user);
	SEND_Q(rmsg, desc);
}


void do_LogoutReq(CNetMsg& msg, CDescriptor* desc)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	msg.MoveFirst();

	int subno = -1;
	int userindex = -1;

	msg >> subno
		>> userindex;

	ProcessLogout(desc, userindex);
}


void do_UserInfoReq(CNetMsg& msg, CDescriptor* desc)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CNetMsg rmsg;

	msg.MoveFirst();

	int subno = -1;
	int userindex = -1;

	msg >> subno
		>> userindex;

	CUser data;

	CUser* user = FindUser(desc, subno, userindex);
	if (user)
	{
		if (!ReadCurrentInfo(userindex, *user, false))
		{
			GAMELOG << init("USERINFO FAIL")
					<< "CANNOT READ CURRENT INFO" << delim
					<< "USERINDEX" << delim
					<< userindex
					<< end;

			UserInfoRepMsg(rmsg, BM_USERINFO_ERROR_NOTFOUND, subno, userindex, user, 0, 0);
			SEND_Q(rmsg, desc);
			return ;
		}

		UserInfoRepMsg(rmsg, BM_USERINFO_ERROR_OK, subno, userindex, user, 0, 0);
		SEND_Q(rmsg, desc);
	}
	else
	{
		GAMELOG << init("USERINFO FAIL")
				<< "NOT FOUND USER" << delim
				<< "USERINDEX" << delim
				<< userindex
				<< end;

		UserInfoRepMsg(rmsg, BM_USERINFO_ERROR_NOTFOUND, subno, userindex, NULL, 0, 0);
		SEND_Q(rmsg, desc);
		return ;
	}

#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
}


void do_SectionReq(CNetMsg& msg, CDescriptor* desc)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CNetMsg rmsg;

	msg.MoveFirst();

	int subno = -1;
	int userindex = -1;

	msg >> subno
		>> userindex;

	SendSectionList(desc, subno, userindex);

#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
}


void do_UserRegReq(CNetMsg& msg, CDescriptor* desc)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CNetMsg rmsg;

	msg.MoveFirst();

	int subno = -1;
	int userindex = -1;

	msg >> subno
		>> userindex;

	if (FindUser(desc, subno, userindex) == NULL)
	{
		GAMELOG << init("USER REGIST")
				<< "NOT FOUND" << delim
				<< "USERINDEX" << delim
				<< userindex
				<< end;
		UserRegRepMsg(rmsg, subno, userindex, BM_USER_REG_ERROR_NOTFOUND);
	}
	else
	{
		GAMELOG << init("USER REGIST")
				<< "SUCCESS" << delim
				<< "USERINDEX" << delim
				<< userindex
				<< end;
		UserRegRepMsg(rmsg, subno, userindex, BM_USER_REG_ERROR_OK);
	}

	SEND_Q(rmsg, desc);
}


void do_PayReserveReq(CNetMsg& msg, CDescriptor* desc)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CNetMsg rmsg;

	msg.MoveFirst();

	int subno = -1;
	int userindex = -1;

	msg >> subno
		>> userindex;

	CUser* user = FindUser(desc, subno, userindex);
	if (!user)
	{
		GAMELOG << init("PAYMENT FAIL")
				<< "NOT FOUND USER" << delim
				<< "USERINDEX" << delim
				<< userindex
				<< end;
		return ;
	}

	if (!ReadCurrentInfo(userindex, *user, true))
	{
		GAMELOG << init("PAYMENT FAIL")
				<< "CANNOT READ CURRENT INFO" << delim
				<< "USERINDEX" << delim
				<< userindex
				<< end;
		return ;
	}

	bool ret;
	bool isreserved = user->reserve_found();
	// 예약 -> 현재 시간제 -> 오류
	if (isreserved)
		ret = PayBillGuid(user, user->reserve_billguild());
	else if (user->billsection() == BILL_SECTION_HOUR)
		ret = PayBillGuid(user, user->billguid());
	else
		ret = false;

	if (ret)
	{
		if (isreserved)
		{
			GAMELOG << init("PAYMENT RESERVED SECTION")
					<< "USERINDEX" << delim
					<< userindex << delim
					<< "BILL GUID" << delim
					<< user->billguid()
					<< end;
		}
		else
		{
			GAMELOG << init("PAYMENT CURRENT SECTION(HOUR)")
					<< "USERINDEX" << delim
					<< userindex << delim
					<< "BILL GUID" << delim
					<< user->billguid()
					<< end;
		}
		PayRepMsg(rmsg, BM_PAY_ERROR_OK, subno, userindex, user, 0);
	}
	else
	{
		if (isreserved)
		{
			GAMELOG << init("PAYMENT RESERVED SECTION FAIL")
					<< "NO POINT (OR OTHER ERROR)" << delim
					<< "USERINDEX" << delim
					<< userindex << delim
					<< "BILL GUID" << delim
					<< user->reserve_billguild()
					<< end;
		}
		else
		{
			GAMELOG << init("PAYMENT CURRENT SECTION(HOUR) FAIL")
					<< "NO POINT (OR OTHER ERROR)" << delim
					<< "USERINDEX" << delim
					<< userindex << delim
					<< "BILL GUID" << delim
					<< user->billguid()
					<< end;
		}
		PayRepMsg(rmsg, BM_PAY_ERROR_NOPOINT, subno, userindex, user, 0);
	}

	SEND_Q(rmsg, desc);
}


void do_PayOtherReq(CNetMsg& msg, CDescriptor* desc)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif


	CNetMsg rmsg;

	msg.MoveFirst();

	int subno = -1;
	int userindex = -1;
	char bguid[LEN_BILL_GUID];

	msg >> subno
		>> userindex
		>> bguid;

	CUser* user = FindUser(desc, subno, userindex);
	if (!user)
	{
		GAMELOG << init("PAYMENT FAIL")
				<< "NOT FOUND USER" << delim
				<< "USERINDEX" << delim
				<< userindex
				<< end;
		return ;
	}

	if (!ReadCurrentInfo(userindex, *user, true))
	{
		GAMELOG << init("PAYMENT FAIL")
				<< "CANNOT READ CURRENT INFO" << delim
				<< "USERINDEX" << delim
				<< userindex
				<< end;
		return ;
	}

	CSectionInfo section;
	if (!ReadSectionInfo(bguid, section))
	{
		GAMELOG << init("PAYMENT FAIL")
				<< "CANNOT READ SECTION INFO" << delim
				<< "USERINDEX" << delim
				<< userindex << delim
				<< "BILL GUID" << delim
				<< bguid
				<< end;
		PayRepMsg(rmsg, BM_PAY_ERROR_NOPOINT, subno, userindex, user, 0);
		SEND_Q(rmsg, desc);
		return ;
	}

	bool isReserveType = false;

	if (user->RemainTime() > 0)
	{
		// 시간 남아 있고
		if (section.billsection() == BILL_SECTION_HOUR)
		{
			// 시간 남아서 시간제 선택은 예약
			isReserveType = true;
		}
	}

	if (isReserveType)
	{
		GAMELOG << init("RESERVE SECTION")
				<< "USERINDEX" << delim
				<< userindex << delim
				<< "BILL GUID" << delim
				<< bguid
				<< end;

		ReserveSection(user, bguid);
		UserInfoRepMsg(rmsg, BM_USERINFO_ERROR_OK, subno, userindex, user, 0, 0);
		SEND_Q(rmsg, desc);
	}
	else
	{
		bool ret;
		ret = PayBillGuid(user, bguid);

		if (ret)
		{
			GAMELOG << init("PAYMENT SUCCESS")
					<< "USERINDEX" << delim
					<< userindex << delim
					<< "BILL GUID" << delim
					<< bguid
					<< end;

			PayRepMsg(rmsg, BM_PAY_ERROR_OK, subno, userindex, user, 0);
		}
		else
		{
			GAMELOG << init("PAYMENT FAIL")
					<< "NO POINT (OR OTHER ERROR)" << delim
					<< "USERINDEX" << delim
					<< userindex << delim
					<< "BILL GUID" << delim
					<< bguid
					<< end;

			PayRepMsg(rmsg, BM_PAY_ERROR_NOPOINT, subno, userindex, user, 0);
		}

		SEND_Q(rmsg, desc);
	}
}


#else //#ifndef LC_TLD

#include "DBCmd.h"

void do_BalanceReq(CNetMsg& msg, CDescriptor* desc)
{
	unsigned char gameID;
	int userindex;
	CLCString idname(ID_LENGTH+1);
	int cpID;
	int balance = 0;

	msg >> gameID
		>> userindex
		>> idname
		>> cpID;

	if( desc->m_gameId != gameID )
	{
		GAMELOG << init("NO_MATCHING_GAMEID")
				<< "GAMEID" << delim << desc->m_gameId << delim
				<< "SERVER" << delim << desc->m_serverNo
				<< end;

		STATE(desc) = CON_CLOSE;
		return;
	}

	balance = GetBalance(userindex, idname);

	CNetMsg rmsg;

	if( balance == -1 )
	{
		CashBalanceRepMsg(rmsg, MSG_BILLITEM_ERROR_NOTUSER, gameID, userindex, cpID, balance);
		SEND_Q(rmsg, desc);

		return;
	}

	GAMELOG << init("BALANCE_REQUEST_SUCCESS")
			<< gameID << delim
			<< cpID << delim
			<< userindex << delim
			<< idname << delim
			<< balance << end;

	CashBalanceRepMsg(rmsg, MSG_BILLITEM_ERROR_SUCCESS, gameID, userindex, cpID, balance);
	SEND_Q(rmsg, desc);

}
void do_CanbuyReq(CNetMsg& msg, CDescriptor* desc)
{
	char gameID;
	CLCString idname(ID_LENGTH+1);
	int userindex;
	int cpID;
	int itemID[MAX_PURCHASEITEM];
	int i = 0;
	int sumPrice = 0;
	int balance = 0;
	char count = 0;
	int requestCash = 0;

	msg >> gameID
		>> userindex
		>> idname
		>> cpID
		>> count;

	if( desc->m_gameId != gameID )
	{
		GAMELOG << init("NO_MATCHING_GAMEID")
				<< "GAMEID" << delim << desc->m_gameId << delim
				<< "SERVER" << delim << desc->m_serverNo
				<< end;

		STATE(desc) = CON_CLOSE;
		return;
	}

	for(i = 0; i < count; i++)
	{
		msg >> itemID[i];
	}

	msg >> requestCash;

	CNetMsg rmsg;
	// 현재 계정의 balance가져오기
	balance = GetBalance(userindex, idname);

	if( balance == -1 )
	{
		CanBuyRepMsg(rmsg, MSG_BILLITEM_ERROR_DATABASE, gameID, userindex, cpID, count, itemID, balance);
		SEND_Q(rmsg, desc);
		return;
	}

	// 사려는 아이템의 총 가격 가져오기 (유저디비에서)
	sumPrice = GetSumPrice(count, itemID, gameID);

	if( sumPrice == -1 || requestCash != sumPrice )
	{
		CanBuyRepMsg(rmsg, MSG_BILLITEM_ERROR_NOTITEM, gameID, userindex, cpID, count, itemID, balance);
		SEND_Q(rmsg, desc);
		GAMELOG << init("CANBUY_REQUEST_ERROR")
				<< gameID << delim
				<< cpID << delim
				<< userindex << delim
				<< idname << delim
				<< (int) MSG_BILLITEM_ERROR_NOTITEM << end;
		return;
	}
	

	if( balance < sumPrice )
	{
		// 잔액 부족
		CanBuyRepMsg(rmsg, MSG_BILLITEM_ERROR_LACKCASH, gameID, userindex, cpID, count, itemID, balance);
		SEND_Q(rmsg, desc);

		GAMELOG << init("CANBUY_REQUEST_ERROR")
				<< gameID << delim
				<< cpID << delim
				<< userindex << delim
				<< idname << delim
				<< (int) MSG_BILLITEM_ERROR_LACKCASH << end;
		return;
	}

	// balance성공, 아이템 총 가격 성공, balance >= sumPrice이면 성공
	CanBuyRepMsg(rmsg, MSG_BILLITEM_ERROR_SUCCESS, gameID, userindex, cpID, count, itemID, balance - sumPrice);
	SEND_Q(rmsg, desc);

}
void do_BuyReq(CNetMsg& msg, CDescriptor* desc)
{
	// 응답을 buy req로 처리 해야 함.
	char gameID;
	CLCString idname(ID_LENGTH+1);
	CLCString ip(HOST_LENGTH+1);
	int userindex;
	int cpID;
	int itemID[MAX_PURCHASEITEM];
	int i = 0;
	int serial;
	int sumPrice = 0;
	int balance = 0;
	char count = 0;

	msg >> serial
		>> gameID
		>> userindex
		>> idname
		>> cpID
		>> ip
		>> count;

	if( desc->m_gameId != gameID )
	{
		GAMELOG << init("NO_MATCHING_GAMEID")
				<< "GAMEID" << delim << desc->m_gameId << delim
				<< "SERVER" << delim << desc->m_serverNo
				<< end;

		STATE(desc) = CON_CLOSE;
		return;
	}

	for(i = 0; i < count; i++)
	{
		msg >> itemID[i];
	}

	// 캐쉬 차감, 구입 로그 남기기 
	CNetMsg rmsg;
	// 현재 계정의 balance가져오기
	balance = GetBalance(userindex, idname);

	if( balance == -1 )
	{
		BuyRepMsg(rmsg, MSG_BILLITEM_ERROR_DATABASE, serial, gameID, userindex, cpID, count, itemID, balance);
		SEND_Q(rmsg, desc);
		return;
	}

	// 사려는 아이템의 총 가격 가져오기 (유저디비에서)
	sumPrice = GetSumPrice(count, itemID, gameID);

	if( sumPrice == -1 )
	{
		BuyRepMsg(rmsg, MSG_BILLITEM_ERROR_NOTITEM, serial, gameID, userindex, cpID, count, itemID, balance);
		SEND_Q(rmsg, desc);

		GAMELOG << init("BUY_REQUEST_ERROR")
				<< gameID << delim
				<< cpID << delim
				<< userindex << delim
				<< idname << delim
				<< (int) MSG_BILLITEM_ERROR_NOTITEM << end;
		return;
	}
	

	if( balance < sumPrice )
	{
		// 잔액 부족
		BuyRepMsg(rmsg, MSG_BILLITEM_ERROR_LACKCASH, serial, gameID, userindex, cpID, count, itemID, balance);
		SEND_Q(rmsg, desc);

		GAMELOG << init("BUY_REQUEST_ERROR")
				<< gameID << delim
				<< cpID << delim
				<< userindex << delim
				<< idname << delim
				<< (int) MSG_BILLITEM_ERROR_LACKCASH << end;
		return;
	}

	// 차감요청( 기시기해서 로그를 남기고 성공하면 차감 
	CDBCmd cmd;
	if( !cmd.Init(&gserver.m_catalDB) )
	{
		BuyRepMsg(rmsg, MSG_BILLITEM_ERROR_DATABASE, serial, gameID, userindex, cpID, count, itemID, balance);
		SEND_Q(rmsg, desc);

		GAMELOG << init("BUY_REQUEST_ERROR")
				<< "DBCMD_INIT" << delim
				<< gameID << delim
				<< cpID << delim
				<< userindex << delim
				<< idname << end;

		return;
	}
	if( !cmd.BeginTrans() )
	{
		BuyRepMsg(rmsg, MSG_BILLITEM_ERROR_DATABASE, serial, gameID, userindex, cpID, count, itemID, balance);
		SEND_Q(rmsg, desc);

		GAMELOG << init("BUY_REQUEST_ERROR")
				<< "BEGIN_TRANS" << delim
				<< gameID << delim
				<< cpID << delim
				<< userindex << delim
				<< idname << end;
		return;
	}

	// bill_log 남기고 bill_game_item_buy 로그 남기고 bill_game_item 어쩌고 차감.

	int sumMileage;
	int errorCode;
	if( errorCode = DeductBalance(&cmd, serial, gameID, userindex, ip, cpID, count, itemID, balance, sumPrice, &sumMileage) )
	{
		GAMELOG << init("BUY_REQUEST_ERROR")
				<< "UPDATE_DB" << delim
				<< gameID << delim
				<< cpID << delim
				<< userindex << delim
				<< idname << end;


		BuyRepMsg(rmsg, (BILL_ERROR_MESSAGE_TYPE) errorCode, serial, gameID, userindex, cpID, count, itemID, balance);
		SEND_Q(rmsg, desc);

		return;
	}

	cmd.Commit();

	// balance성공, 아이템 총 가격 성공, balance >= sumPrice이면 성공
	BuyRepMsg(rmsg, MSG_BILLITEM_ERROR_SUCCESS, serial, gameID, userindex, cpID, count, itemID, balance - sumPrice + sumMileage);
	SEND_Q(rmsg, desc);

	GAMELOG << init("BUY_REQUEST_SUCCESS")
				<< gameID << delim
				<< cpID << delim
				<< userindex << delim
				<< idname << delim
				<< balance << delim
				<< sumPrice << end;

}
int DeductBalance(CDBCmd *m_billCmd, int serial, char gameID, int userindex, const char* ip, int cpID, char count, int itemID[], int balance, int sumPrice, int* sumMileage)
{
	// bill_game_item_buy와 bill_game_item 업데이트 
	int cash, mileage, i;
	*sumMileage = 0;
	CLCString ctname(50+1);

	// bill_log 남기기
	sprintf(g_buf, " insert into bill_log (log_user_index, log_date, log_gbuy_index, log_type, log_add_point, log_old_point, log_gitem_game) "
		"values (%d, getdate(), %d, %d, 0, %d, %d ) ", userindex, serial, 2, balance, gameID);
	
	m_billCmd->SetQuery(g_buf);
	if( !m_billCmd->Update() )
	{
		GAMELOG << init("DEDUCTBALANCE_ERROR_INSERT_BILL_LOG")
				<< gameID << delim
				<< userindex << delim
				<< cpID	<< delim
				<< balance << delim
				<< ip << delim
				<< serial << end;

		m_billCmd->Rollback();
		return MSG_BILLITEM_ERROR_DATABASE;
	}

	CDBCmd dataCmd;
	dataCmd.Init(&gserver.m_catalDB);

	for(i = 0; i < count && itemID[i] != -1 ; i++)
	{
		sprintf(g_buf, "select a_ctname, a_cash, a_mileage from t_catalog_%d where a_enable = 1 and a_ctid = %d ", gameID, itemID[i]);
		dataCmd.SetQuery(g_buf);

		if( !dataCmd.Open() || !dataCmd.MoveFirst() )
		{
			GAMELOG << init("DEDUCTBALANCE_ERROR")
					<< gameID << delim
					<< userindex << delim
					<< cpID	<< delim
					<< balance << delim
					<< itemID[i] << end;
			m_billCmd->Rollback();
			return MSG_BILLITEM_ERROR_NOTITEM;
		}

		if( !dataCmd.GetRec("a_ctname", ctname)
			|| !dataCmd.GetRec("a_cash", cash)
			|| !dataCmd.GetRec("a_mileage", mileage) )
		{
			GAMELOG << init("DEDUCTBALANCE_GET_ERROR")
					<< gameID << delim
					<< userindex << delim
					<< cpID	<< delim
					<< balance << delim
					<< itemID[i] << end;
			m_billCmd->Rollback();
			return MSG_BILLITEM_ERROR_DATABASE;
		}

		// bill_game_item 테이블에 레코드가 있으면 업데이트하고 없으면 인서트

#ifdef BWTEST
		sprintf(g_buf, "select gitem_index, gitem_game, gitem_name, gitem_count from bill_game_item where gitem_index = %d and gitem_game = %d", itemID[i], gameID);

		GAMELOG << init("CT_NAME") << ctname << delim << end;
#else
		sprintf(g_buf, "select gitem_index from bill_game_item where gitem_index = %d and gitem_game = %d", itemID[i], gameID);
#endif
		m_billCmd->SetQuery(g_buf);
		if( !m_billCmd->Open() )
		{
			GAMELOG << init("DEDUCTBALANCE_ERROR_OPEN_BILL_GAME_ITEM")
					<< gameID << delim
					<< userindex << delim
					<< cpID	<< delim
					<< balance << delim
					<< ip << delim
					<< itemID[i]  << delim
					 << end;

			m_billCmd->Rollback();
			return MSG_BILLITEM_ERROR_DATABASE;
		}


		if( m_billCmd->MoveFirst() != 1 )
		{
			// insert
#ifdef BWTEST
			sprintf(g_buf, "insert into bill_game_item (gitem_index, gitem_game, gitem_name, gitem_count) "
				"values "
				"(%d, %d, '%s ', 1) ", itemID[i], gameID, (const char*) ctname);
#else
			sprintf(g_buf, "insert into bill_game_item (gitem_index, gitem_game, gitem_name, gitem_count) "
				"values "
				"(%d, %d, '%s', 1) ", itemID[i], gameID, (const char*) ctname);
#endif

			m_billCmd->SetQuery(g_buf);
			if( !m_billCmd->Update() )
			{
				GAMELOG << init("DEDUCTBALANCE_ERROR_INSERT_BILL_GAME_ITEM")
						<< gameID << delim
						<< userindex << delim
						<< cpID	<< delim
						<< balance << delim
						<< ip << delim
						<< itemID[i]  << delim
						 << end;

				m_billCmd->Rollback();
				return MSG_BILLITEM_ERROR_DATABASE;
			}
		}
		else
		{

			//update
			
#ifdef BWTEST
			sprintf(g_buf, "update bill_game_item "
				"set gitem_count = gitem_count + 1, gitem_name = '%s ' "
				"where gitem_index = %d and gitem_game = %d",  (const char*) ctname, itemID[i], gameID);
#else
			sprintf(g_buf, "update bill_game_item "
				"set gitem_count = gitem_count + 1, gitem_name = '%s' "
				"where gitem_index = %d and gitem_game = %d", (const char*) ctname, itemID[i], gameID);
#endif

			m_billCmd->SetQuery(g_buf);
			if( !m_billCmd->Update() )
			{
				GAMELOG << init("DEDUCTBALANCE_ERROR_UPDATE_BILL_GAME_ITEM")
						<< gameID << delim
						<< userindex << delim
						<< cpID	<< delim
						<< balance << delim
						<< ip << delim
						<< itemID[i] << delim
						 << end;

				m_billCmd->Rollback();
				return MSG_BILLITEM_ERROR_DATABASE;
			}
		}

		
		
		sprintf(g_buf, "insert into bill_game_item_buy "
			" ( gbuy_index, gbuy_user_index, gbuy_user_ip, gbuy_date, gbuy_gitem_index, gbuy_gitem_point, gbuy_gitem_bonus, gbuy_gitem_game ) "
			" values "
			" ( %d, %d, '%s', getdate(), %d, %d, %d, %d) ", serial, userindex, ip, itemID[i], cash, mileage, gameID);

		m_billCmd->SetQuery(g_buf);
		if( !m_billCmd->Update() )
		{
			GAMELOG << init("DEDUCTBALANCE_ERROR_INSERT_ITEM_BUY_LOG")
					<< gameID << delim
					<< userindex << delim
					<< cpID	<< delim
					<< balance << delim
					<< ip << delim
					<< itemID[i] 
					 << end;

			m_billCmd->Rollback();

			return MSG_BILLITEM_ERROR_DATABASE;
		}
		
		
		*sumMileage += mileage;
	}

	//bill_log에 addpoint 업데이트
	sprintf(g_buf, "update bill_log set log_add_point = %d where log_gbuy_index = %d ", -sumPrice+*sumMileage, serial);
	m_billCmd->SetQuery(g_buf);
	if( !m_billCmd->Update() )
	{
		GAMELOG << init("DEDUCTBALANCE_ERROR_UPDATE_BILL_LOG")
				<< gameID << delim
				<< userindex << delim
				<< cpID	<< delim
				<< balance << delim
				<< ip << delim
				<< serial << end;

		m_billCmd->Rollback();
		return MSG_BILLITEM_ERROR_DATABASE;
	}

	// bill_user의 포인트 차감
	sprintf(g_buf, " update bill_user set user_point = %d, user_bonus = user_bonus + %d where user_index = %d ", balance-sumPrice+*sumMileage, *sumMileage, userindex);
	m_billCmd->SetQuery(g_buf);
	if( !m_billCmd->Update() )
	{
		GAMELOG << init("DEDUCTBALANCE_ERROR_UPDATE_BILL_USER")
				<< gameID << delim
				<< userindex << delim
				<< cpID	<< delim
				<< balance << delim
				<< sumPrice  << delim
				<< *sumMileage  << delim
				<< ip << delim
				<< serial << end;

		m_billCmd->Rollback();
		return MSG_BILLITEM_ERROR_DATABASE;
	}

	GAMELOG << init("DEDUCTBALANCE_SUCCESS_UPDATE_BILL_USER")
				<< gameID << delim
				<< userindex << delim
				<< cpID	<< delim
				<< balance << delim
				<< sumPrice  << delim
				<< *sumMileage  << delim
				<< ip << delim
				<< serial << end;

	return MSG_BILLITEM_ERROR_SUCCESS;
}

int GetSumPrice(char count, const int itemID[], char gameID)
{
	int sumPrice = 0;
	int i;
	int price = 0;

	CDBCmd dataCmd;
	dataCmd.Init(&gserver.m_catalDB);

	for(i = 0; i < count && itemID[i] != -1; i++)
	{
		sprintf(g_buf, "select a_cash from t_catalog_%d where a_ctid = %d ", gameID, itemID[i]);
		dataCmd.SetQuery(g_buf);

		if( !dataCmd.Open() || !dataCmd.MoveFirst() )
		{
			// 사려는 아이템이 없음
			GAMELOG << init("CANNOT_FIND_CATALOG")
					<< itemID[i] << end;

			return -1;
		}
		if( !dataCmd.GetRec("a_cash", price) )
		{
			GAMELOG << init("CANNOT_FIND_CATALOG_PRICE")
					<< itemID[i] << end;
			return -1;
		}

		sumPrice += price;

		price = 0;
	}

	return sumPrice;
}

int GetBalance(const char* idname)
{
	int balance = -1;

	CDBCmd selcmd;
	if (!selcmd.Init(&gserver.m_catalDB))
	{
		GAMELOG << init("BALANCE_REQUEST_ERROR")
				<< "DATABASE_CONNECTION"  << delim
				<< idname  << delim << end;
		return -1;
	}

	sprintf(g_buf,
		"SELECT   user_point, user_id "
		"FROM      bill_user "
		"WHERE   user_index = %d ",
		idname);
	selcmd.SetQuery(g_buf);
	if (!selcmd.Open() || selcmd.MoveNext() != 1)
	{
		GAMELOG << init("BALANCE_REQUEST_ERROR")
				<< "DATABASE_OPEN"  << delim
				<< idname  << delim << end;
		return -1;
	}
	
	selcmd.GetRec("a_balance", balance);

	return balance;
}

int GetBalance(int userindex, const char *idname)
{
	CLCString name(50);
	int balance = -1;

	CDBCmd selcmd;
	if (!selcmd.Init(&gserver.m_catalDB))
	{
		GAMELOG << init("BALANCE_REQUEST_ERROR")
				<< "DATABASE_CONNECTION" << delim
				<< userindex  << delim
				<< end;
		return -1;
	}

	sprintf(g_buf,
		"SELECT   user_point, user_id "
		"FROM      bill_user "
		"WHERE   user_index = %d ",
		userindex);

	selcmd.SetQuery(g_buf);

	if (!selcmd.Open() || selcmd.MoveNext() != 1)
	{
		GAMELOG << init("BALANCE_REQUEST_ERROR")
				<< "DATABASE_OPEN"  << delim
				<< userindex  << delim
				<< end;
		return -1;
	}

	selcmd.GetRec("user_id", name, true);
	selcmd.GetRec("user_point", balance);

#ifdef USER_ID_CASE_SENSITIVE
	if (strcmp(name, idname) != 0)
		return -1;
#else
	if( strcmp2(name, idname) != 0 )
		return -1;
#endif // USER_ID_CASE_SENSITIVE

	return balance;
}
#endif //#ifndef LC_TLD