#include "stdhdrs.h"
#include "Server.h"
#include "CmdMsg.h"
#include "DBCmd.h"


int test_count_open_query = 0;
int test_count_update_query = 0;
int test_count_open_session = 0;
int test_count_close_session = 0;


void PingDB()
{
	CDBCmd cmd;
	if (cmd.Init(&gserver.m_catalDB))
	{
#ifdef LC_TLD
		cmd.SetQuery("SELECT TOP 1 * FROM [bill_user]");
		cmd.Open();

		CDBCmd cmd;

		cmd.Init(&gserver.m_catalDB);

		{
			cmd.SetQuery("select a_index from t_catalog_1 limit 0, 1 ");
			cmd.Open();
		}
#else
		cmd.Open("SELECT TOP 1 * FROM [BILL_SECTION]");
#endif
	}
}

//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )
void ReadReserveInfo(int userindex, CUser& user)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
	user.reserve_found(false);
	user.reserve_billguild("");
	user.reserve_billvalue(0);
	user.reserve_billname("");

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CDBCmd selcmd;
	if (!selcmd.Init(&gserver.m_catalDB))
	{
#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		return ;
	}

	// 예약 얻기
	// SELECT [t1].[Bill_Guid], [t2].[Bill_Value], [t2].[Bill_ViewName]
	//  FROM [GAME_CATALOG] [t1] INNER JOIN [BILL_SECTION] [t2] ON [t1].[Bill_Guid] = [t2].[Bill_Guid]
	//  WHERE ([t1].[Game_Guid] = 1) AND ([t1].[Bill_Check] = 3)
	sprintf(g_buf,

			"SELECT [t1].[Bill_Guid], [t2].[Bill_Value], [t2].[Bill_ViewName]"
			" FROM [GAME_CATALOG] [t1] INNER JOIN [BILL_SECTION] [t2] ON [t1].[Bill_Guid] = [t2].[Bill_Guid]"
			" WHERE ([t1].[Game_Guid] = %d) AND ([t1].[Bill_Check] = 3)"

			, userindex);
#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << delim << test_count_open_session << delim << test_count_close_session << delim << test_count_open_query << delim << test_count_update_query << end;
	GAMELOG << init("SQL") << g_buf << end;
#endif

	if (selcmd.Open(g_buf))
	{
		if (selcmd.MoveFirst() == 1)
		{
			char rbguid[LEN_BILL_GUID];
			int rbvalue;
			char rbname[LEN_BILL_VIEWNAME];

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
			if (selcmd.GetRec(1, rbguid) && selcmd.GetRec(2, rbvalue) && selcmd.GetRec(3, rbname))
			{
				user.reserve_found(true);
				user.reserve_billguild(rbguid);
				user.reserve_billvalue(rbvalue);
				user.reserve_billname(rbname);
			}
		}
	}
#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
}


bool ReadCurrentInfo(int userindex, CUser& user, bool bForce)
{
	if (!bForce && user.pulseRead() + PULSE_REAL_MIN * 5 > gserver.m_pulse)
		return true;
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	//SELECT [T1].[Bill_Guid], [T1].[Surplus_Point], [T1].[Bill_Section], [T3].[Bill_ViewName], [T1].[Increase_Days]
	//FROM [GAME_CURR_INFO] AS [T1], [BILL_SECTION] AS [T3]
	//WHERE [T1].[Game_Guid]=userindex AND [T1].[Bill_Guid]=[T3].[Bill_Guid]

	sprintf(g_buf,

			"SELECT [T1].[Bill_Guid], [T1].[Surplus_Point], [T1].[Bill_Section], [T3].[Bill_ViewName], [T1].[Increase_Days]"
			" FROM [GAME_CURR_INFO] AS [T1], [BILL_SECTION] AS [T3]"
			" WHERE [T1].[Game_Guid]=%d AND [T1].[Bill_Guid]=[T3].[Bill_Guid]",

			userindex);
#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CDBCmd selcmd;
	if (!selcmd.Init(&gserver.m_catalDB))
	{
#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		return false;
	}

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << delim << test_count_open_session << delim << test_count_close_session << delim << test_count_open_query << delim << test_count_update_query << end;
	GAMELOG << init("SQL") << g_buf << end;
#endif
	if (!selcmd.Open(g_buf))
	{
#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		return false;
	}

	if (selcmd.MoveFirst() != 1)
	{
#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		return false;
	}
#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	char billguid[LEN_BILL_GUID];
	int surpluspoint;
	DBTIMESTAMP increasedays;
	int billsection;
	char billviewname[LEN_BILL_VIEWNAME];

	try
	{
		DBCHECK(selcmd.GetRec(1, billguid));
		DBCHECK(selcmd.GetRec(2, surpluspoint));
		DBCHECK(selcmd.GetRec(3, billsection));
		DBCHECK(selcmd.GetRec(4, billviewname));

		if (!selcmd.IsNULL(5))
		{
			DBCHECK(selcmd.GetRec(5, increasedays));
		}
		else
		{
			SYSTEMTIME st;
			GetLocalTime(&st);
			increasedays.year = st.wYear;
			increasedays.month = st.wMonth;
			increasedays.day = st.wDay;
			increasedays.hour = st.wHour;
			increasedays.minute = st.wMinute;
		}

		user.index(userindex);
		user.billguid(billguid);
		user.surplus(surpluspoint);
		user.billsection(billsection);
		user.billviewname(billviewname);
		user.endtime(DT2TM(increasedays));

		user.bSent1(true);
		user.bSent5(true);
		user.bSent10(true);
		user.bSent30(true);
		user.bSent60(true);
		user.bSent1Day(true);
		user.bSent3Day(true);
		user.bSendTimeOut(true);

		if (user.billsection() < BILL_SECTION_FREE)
		{
			if (user.RemainTime() != 0)					user.bSendTimeOut(false);
			if (user.RemainTime() > 1 * 60)				user.bSent1(false);
			if (user.RemainTime() > 5 * 60)				user.bSent5(false);
			if (user.RemainTime() > 10 * 60)			user.bSent10(false);
			if (user.RemainTime() > 30 * 60)			user.bSent30(false);
			if (user.RemainTime() > 60 * 60)			user.bSent60(false);
			if (user.RemainTime() > 1 * 24 * 60 * 60)	user.bSent1Day(false);
			if (user.RemainTime() > 3 * 24 * 60 * 60)	user.bSent3Day(false);
		}

		// 예약 얻기
		ReadReserveInfo(userindex, user);
	}
	catch (...)
	{
#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		return false;
	}
#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	user.pulseRead(gserver.m_pulse);

	return true;
}


void SendSectionList(CDescriptor* desc, int subno, int userindex)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
	CNetMsg rmsg;

	if (gserver.m_sectionlist.m_head == NULL)
	{
		SectionRepMsg(rmsg, subno, userindex, BM_FLAG_START | BM_FLAG_END | BM_FLAG_EMPTY, NULL);
		SEND_Q(rmsg, desc);
		return ;
	}
	else
	{
		CSectionInfo* p;
		for (p = gserver.m_sectionlist.m_head; p; p = p->m_next)
		{
			int flag = 0;
			if (p->m_prev == NULL)
				flag |= BM_FLAG_START;
			if (p->m_next == NULL)
				flag |= BM_FLAG_END;
			SectionRepMsg(rmsg, subno, userindex, flag, p);
			SEND_Q(rmsg, desc);
		}
	}
#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
}


void CUserList::RefreshRemainTime()
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CUser* p;
	for (p = m_head; p; p = p->m_next)
		ReadCurrentInfo(p->index(), *p, true);
}


bool ReadSectionInfo(const char* bguid, CSectionInfo& section)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	// SELECT [Bill_Guid], [Bill_Section], [Bill_Hour], [Bill_Value], [Bill_ViewName], [Bill_Event]
	//  FROM [BILL_SECTION]
	//  WHERE [Bill_Guid]='billguid'

	sprintf(g_buf,

			"SELECT [Bill_Guid], [Bill_Section], [Bill_Hour], [Bill_Value], [Bill_ViewName], [Bill_Event]"
			" FROM [BILL_SECTION]"
			" WHERE [Bill_Guid]='%s'",

			bguid);

	CDBCmd selcmd;
	if (!selcmd.Init(&gserver.m_catalDB))
		return false;

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
	if (!selcmd.Open(g_buf))
		return false;

	if (selcmd.MoveFirst() != 1)
		return false;

	char			billguid[LEN_BILL_GUID];
	int				billsection;
	int				billhour;
	int				billvalue;
	char			billviewname[LEN_BILL_VIEWNAME];
	int				billevent;

	try
	{
		DBCHECK(selcmd.GetRec(1, billguid));
		DBCHECK(selcmd.GetRec(2, billsection));
		DBCHECK(selcmd.GetRec(3, billhour));
		DBCHECK(selcmd.GetRec(4, billvalue));
		DBCHECK(selcmd.GetRec(5, billviewname));
		DBCHECK(selcmd.GetRec(6, billevent));

		if (billsection == BILL_SECTION_BEGIN)
			return false;
		if (billsection >= BILL_SECTION_FREE && billevent != 1)
			return false;

		section.billguid(billguid);
		section.billsection(billsection);
		section.billhour(billhour);
		section.billvalue(billvalue);
		section.billviewname(billviewname);
		section.billevent(billevent);
	}
	catch (...)
	{
#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		return false;
	}

#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
	return true;
}


bool ReadGamePoint(int userindex, CGamePointList& list)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	list.Reset();

	CDBCmd selcmd;
	if (!selcmd.Init(&gserver.m_catalDB))
		return false;

	// SELECT [Item_Guid], [Item_Surplus], [Session_Guid] FROM [GAME_POINT] WHERE ([Game_Guid] = userindex) AND ([Item_Surplus] > 0) ORDER BY [Item_Guid]
	sprintf(g_buf,
		"SELECT [Item_Guid], [Item_Surplus], [Session_Guid] FROM [GAME_POINT] WHERE ([Game_Guid] = %d) AND ([Item_Surplus] > 0) ORDER BY [Item_Guid]",
		userindex);

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
	if (!selcmd.Open(g_buf))
		return false;

	char itemguid[LEN_ITEM_GUID];
	int surplus;
	char sessionguid[LEN_SESSION_GUID];

	while (selcmd.MoveNext() == 1)
	{
		if (selcmd.GetRec(1, itemguid) && selcmd.GetRec(2, surplus) && selcmd.GetRec(3, sessionguid))
		{
			list.Add(itemguid, surplus, sessionguid);
		}
	}

#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
	return true;
}

/////////////////////////////////////////////
// BANGWALL : 2005-07-05 오후 9:30:31
// Comment : read item history 읽어 리스트로
bool UpdateSupItem(CSupUpdateItemList &supUpdateItemList)
{
	// 리스트가 왓으니 이걸로 업데이트 하셈
	CDBCmd updatecmd;
	if (!updatecmd.Init(&gserver.m_catalDB) )
	{
		printf("디비 커맨더 생성 실패");
		return false;
	}

	try
	{
		if ( !updatecmd.BeginTrans() )
			//return false;
			throw 0;

		CSupUpdateItem *supUpdateItem;

		supUpdateItem = supUpdateItemList.head();
		if( !supUpdateItem ) throw 0;

		while(1)
		{
			sprintf( g_buf, 
				"UPDATE  t_item_history "
				"SET     a_bsupply = { fn NOW() } "
				"WHERE   (a_index = %d) and a_bsupply IS NULL "
				, supUpdateItem->index());

			if (!updatecmd.Update(g_buf) )
				//return false;
				throw 0;

			if( !(supUpdateItem = supUpdateItem->next()) ) break;
		}

		if( !updatecmd.CommitTrans() )
			throw 0;
			//return false;

		return true;
	
		
	}

	catch(...)
	{
		updatecmd.RollbackTrans();
		printf("rollback");
		return false;
	}
	
}

bool ReadSupItem(int userindex, CSupItemList &list, int serverGroup)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	list.Reset();

	CDBCmd selcmd;
	if (!selcmd.Init(&gserver.m_catalDB))
		return false;

	//SELECT   a_catalog_id, a_server_group, a_item_index, a_item_plus, a_item_flag,  a_item_count
	//FROM      dbo.t_item_history
	//WHERE   (a_bsupply IS NULL) AND (a_user_id = 4)
	sprintf(g_buf,
		"SELECT   a_index, a_catalog_id, a_server_group, a_item_index, a_item_plus, a_item_flag, a_item_count "
		"FROM      t_item_history "
		"WHERE   (a_bsupply IS NULL) AND (a_user_index = %d) AND (a_server_group = %d)"
		"ORDER BY a_index ",
		userindex, serverGroup);

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
	GAMELOG << init("SQL") << g_buf << end;
#endif
	if (!selcmd.Open(g_buf))
		return false;

	int index;
	char catalogid[LEN_BILL_GUID];
	int server;
	int itemindex;
	int itemplus;
	int itemflag;
	int itemcount;

	while (selcmd.MoveNext() == 1)
	{
		if (selcmd.GetRec(1, index) 
			&& selcmd.GetRec(2, catalogid) 
			&& selcmd.GetRec(3, server) 
			&& selcmd.GetRec(4, itemindex)
			&& selcmd.GetRec(5, itemplus)
			&& selcmd.GetRec(6, itemflag)
			&& selcmd.GetRec(7, itemcount)
			)
		{
			list.Add( index, 
					catalogid,
					userindex,
					server,
					itemindex,
					itemplus,
					itemflag,
					itemcount);
		}
	}

#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
	return true;
}


bool PayBillGuid(CUser* user, const char* bguid)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	// 요금제 가져오기
	CSectionInfo section;
	if (!ReadSectionInfo(bguid, section))
		return false;

	CGamePointList gplist;
	if (!ReadGamePoint(user->index(), gplist))
		return false;
	if (gplist.total() < section.billvalue())
		return false;

	// 가능여부 조회
	if (user->billsection() >= BILL_SECTION_FREE)
		return false;

	if (user->surplus() < section.billvalue())
		return false;

	SYSTEMTIME curendtime;
	if (user->RemainTime() > 0)
	{
		// 남은 시간 있으면 그걸로 설정
		curendtime.wYear = user->endtime_year();
		curendtime.wMonth = user->endtime_mon();
		curendtime.wDay = user->endtime_day();
		curendtime.wHour = user->endtime_hour();
		curendtime.wMinute = user->endtime_min();
	}
	else
	{
		// 없으면 현재 시간으로
		GetLocalTime(&curendtime);
	}

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
	CDBCmd updatecmd;
	if (!updatecmd.Init(&gserver.m_catalDB))
		return false;

	try
	{
#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		// 트랜잭션 시작
		if (!updatecmd.BeginTrans())
			throw 0;

		// 만료데이터 정리
		// UPDATE GAME_CATALOG SET Bill_Check = 1 WHERE (Game_Guid = 1) AND (Bill_Check = 2) AND (End_Time <= CONVERT(SMALLDATETIME, CURRENT_TIMESTAMP))
		sprintf(g_buf,
			"UPDATE GAME_CATALOG SET Bill_Check = 1 WHERE (Game_Guid = 1) AND (Bill_Check = 2) AND (End_Time <= CONVERT(SMALLDATETIME, CURRENT_TIMESTAMP))"
			, user->index());

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		if (!updatecmd.Update(g_buf))
			throw 0;

		// UPDATE : GAME_CURR_INFO
		// UPDATE [GAME_CURR_INFO]
		//  SET [Bill_Guid] = 'billguid', [Surplus_Point] = [Surplus_Point] - billvalue, [Increase_Days] = dateadd(hour, billhour, [Increase_Days]), [Bill_Section] = billsection
		//  WHERE [Game_Guid] = userindex
		sprintf(g_buf,

			"UPDATE GAME_CURR_INFO"
			" SET [Bill_Guid] = '%s', [Surplus_Point] = [Surplus_Point] - %d, [Increase_Days] = dateadd(hour, %d, '%04d-%02d-%02d %02d:%02d'), [Bill_Section] = %d"
			" WHERE [Game_Guid] = %d"

			, section.billguid(), section.billvalue()
			, section.billhour(), curendtime.wYear, curendtime.wMonth, curendtime.wDay, curendtime.wHour, curendtime.wMinute
			, section.billsection(), user->index());

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		if (!updatecmd.Update(g_buf))
			throw 0;

		// 예약이 있고 결제 방법이 예약과 같으면 UPDATE 아니면 INSERT
		if (user->reserve_found() && strcmp(user->reserve_billguild(), bguid) == 0)
		{
			// UPDATE : GAME_CATALOG
			// UPDATE [GAME_CATALOG] SET [Bill_Check]=2, [End_Time]=dateadd(hour, %d, '%04d-%02d-%02d %02d:%02d') WHERE [Game_Guid]=%d AND [Bill_Check]=3
			sprintf(g_buf,
				"UPDATE [GAME_CATALOG] SET [Bill_Check]=2, [End_Time]=dateadd(hour, %d, '%04d-%02d-%02d %02d:%02d') WHERE [Game_Guid]=%d AND [Bill_Check]=3"
				, section.billhour(), curendtime.wYear, curendtime.wMonth, curendtime.wDay, curendtime.wHour, curendtime.wMinute
				, user->index());
		}
		else
		{
			// INSERT : GAME_CATALOG
			// INSERT INTO [GAME_CATALOG] ([Game_Guid], [Bill_Guid], [Bill_ViewName], [Bill_Section], [Bill_Hour], [Bill_Check], [End_Time]) VALUES (userindex, 'billguid', 'billname', billsection, billhour, billcheck, 'endtime')
			sprintf(g_buf,
				"INSERT INTO [GAME_CATALOG] ([Game_Guid], [Bill_Guid], [Bill_ViewName], [Bill_Section], [Bill_Hour], [Bill_Check], [End_Time]) VALUES"
				" (%d, '%s', '%s', %d, %d, 2, dateadd(hour, %d, '%04d-%02d-%02d %02d:%02d'))"
				, user->index(), section.billguid(), section.billviewname(), section.billsection(), section.billhour()
				, section.billhour(), curendtime.wYear, curendtime.wMonth, curendtime.wDay, curendtime.wHour, curendtime.wMinute);
		}

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		if (!updatecmd.Update(g_buf))
			throw 0;

		CGamePoint* gp = gplist.head();
		int remainpoint = section.billvalue();
		int dec;
		while (remainpoint > 0)
		{
			if (gp->surplus() < remainpoint)
				dec = gp->surplus();
			else
				dec = remainpoint;

			// UPDATE: GAME_POINT
			// UPDATE [GAME_POINT] SET [Item_Surplus] = [Item_Surplus] - value WHERE ([Game_Guid] = userindex) AND ([Item_Guid] = 'itemguid')
			sprintf(g_buf,
				"UPDATE [GAME_POINT] SET [Item_Surplus] = [Item_Surplus] - %d WHERE ([Game_Guid] = %d) AND ([Item_Guid] = '%s')",
				dec, user->index(), gp->itemguid());

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
			if (!updatecmd.Update(g_buf))
				throw 0;

			// INSERT: GAME_POINT_CATALOG
			// INSERT INTO [GAME_POINT_CATALOG] ([Item_Guid], [Item_Exp], [Game_Guid], [Session_Guid]) VALUES ('itemguid', value, userindex, 'session_guid')
			sprintf(g_buf,
				"INSERT INTO [GAME_POINT_CATALOG] ([Item_Guid], [Item_Exp], [Game_Guid], [Session_Guid]) VALUES ('%s', %d, %d, '%s')",
				gp->itemguid(), dec, user->index(), gp->sessionguid());

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
			if (!updatecmd.Update(g_buf))
				throw 0;

			remainpoint -= dec;
			gp = gp->next();
		}

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		// 트랜잭션 끝
		if (!updatecmd.CommitTrans())
			throw 0;

		ReadCurrentInfo(user->index(), *user, true);

#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		return true;
	}
	catch (...)
	{
#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		updatecmd.RollbackTrans();
#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		return false;
	}
}


bool ReserveSection(CUser* user, const char* bguid)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	// 결제 정보 가져오기
	CSectionInfo section;
	if (!ReadSectionInfo(bguid, section))
		return false;

	// 시간제 검사
	if (section.billsection() != BILL_SECTION_HOUR)
		return false;

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
	CDBCmd updatecmd;
	if (!updatecmd.Init(&gserver.m_catalDB))
		return false;

	try
	{
#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		// 트랜잭션 시작
		if (!updatecmd.BeginTrans())
			throw 0;

		// 이전것 바꾸기
		// UPDATE [GAME_CATALOG] SET [Bill_Check] = 4 WHERE ([Game_Guid] = userindex) AND ([Bill_Check] = 3)
		sprintf(g_buf,
			"UPDATE [GAME_CATALOG] SET [Bill_Check] = 4 WHERE ([Game_Guid] = %d) AND ([Bill_Check] = 3)",
			user->index());

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		if (!updatecmd.Update(g_buf))
			throw 0;

		// 새로 넣기
		// INSERT INTO [GAME_CATALOG] ([Game_Guid], [Bill_Guid], [Bill_ViewName], [Bill_Section], [Bill_Hour], [Bill_Check], [End_Time]) VALUES (userindex, 'billguid', 'billname', billsection, billhour, 3, NULL)
		sprintf(g_buf,
			"INSERT INTO [GAME_CATALOG] ([Game_Guid], [Bill_Guid], [Bill_ViewName], [Bill_Section], [Bill_Hour], [Bill_Check], [End_Time]) VALUES"
			" (%d, '%s', '%s', %d, %d, 3, NULL)",
			user->index(), section.billguid(), section.billviewname(), section.billsection(), section.billhour());

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		if (!updatecmd.Update(g_buf))
			throw 0;

#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		// 트랜잭션 끝
		if (!updatecmd.CommitTrans())
			throw 0;

		ReadCurrentInfo(user->index(), *user, true);

#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		return true;
	}
	catch (...)
	{
#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		updatecmd.RollbackTrans();
#ifdef BSTEST
	GAMELOG << init("OUT", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
		return false;
	}
}


void ProcessLogout(CDescriptor* desc, int userindex)
{
#ifdef BSTEST
	GAMELOG << init("IN", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	// 카탈로그에서 시간 만료된거 처리
	// UPDATE [GAME_CATALOG]
	//  SET [Bill_Check]=1
	//  WHERE [Game_Guid]=userindex AND [Bill_Check]=2 AND [End_Time]<=CONVERT(SMALLDATETIME, CURRENT_TIMESTAMP)
	sprintf(g_buf,

		"UPDATE [GAME_CATALOG]"
		" SET [Bill_Check]=1"
		" WHERE [Game_Guid]=%d AND [Bill_Check]=2 AND [End_Time]<=CONVERT(SMALLDATETIME, CURRENT_TIMESTAMP)"

		, userindex);
#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif

	CDBCmd updatecmd;
	if (updatecmd.Init(&gserver.m_catalDB))
	{
#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << delim << test_count_open_session << delim << test_count_close_session << delim << test_count_open_query << delim << test_count_update_query << end;
	GAMELOG << init("SQL") << g_buf << end;
#endif
		updatecmd.Update(g_buf);
	}

#ifdef BSTEST
	sprintf(g_buf, "%08x", (unsigned int)desc);
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << delim << g_buf << end;
#endif

	desc->m_userlist.Remove(userindex);

	GAMELOG << init("LOGOUT")
			<< "USERINDEX" << delim
			<< userindex
			<< end;
}

#endif // #ifndef LC_TLD 
