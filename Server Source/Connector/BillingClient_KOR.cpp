#include "stdhdrs.h"

#include "../ShareLib/bnf.h"

#ifndef USE_TENTER_BILLING

#include "Server.h"
#include "Log.h"
#include "BillingClient.h"
#include "CmdMsg.h"
#include <boost/format.hpp>

CBillingClient::CBillingClient() : service_(NULL)
{
}

bool CBillingClient::Login(const char* name, const char* pw, const char* ip)
{
	return true;
}

bool CBillingClient::Logout(const char* name)
{
	return true;
}

void CBillingClient::operate(rnSocketIOService* service)
{
	CBPacket::SP msg(service->GetMessageForBilling());

	LOG_INFO("OOO (R) : type[%d] serial[%d] size[%d]", msg->m_mtype, msg->m_serial, msg->m_size);

	switch(msg->m_mtype)
	{
	case MSG_BILLITEM_CASHBALANCE_REP:
		{
			OnCashBalance(msg);
		}
		break;
	case MSG_BILLITEM_CANBUY_REP:
		{
			OnCanBuy(msg);
		}
		break;
	case MSG_BILLITEM_BUY_REP:
		{
			OnBuy(msg);
		}
		break;

	case MSG_BILLITEM_SERVERLIVE:	// 빌링 서버 접속 유지용 프로토콜
		GAMELOG << init("MSG_BILLITEM_SERVERLIVE") << end;
		break;

	default:
		{
			GAMELOG << init("UNKNOWN PACKET") << msg->m_mtype << end;
			return;
		}
		break;
	}
}

void CBillingClient::OnCashBalance(CBPacket::SP& msg)
{
	unsigned char returnCode = 0;
	int userindex;
	unsigned char cid = 0;
	int cashBalance = 0;
	int bonusPoint;
	CLCString idname(51);
	//char idname[MAX_ID_NAME_LENGTH + 1];
	msg->MoveFirst();
	RefMsg(msg) >> returnCode
				>> idname
				>> cid
				>> userindex
				>> cashBalance
				>> bonusPoint;

	CUser * user = NULL;
	for(int i = 0; i < gserver.m_maxSubServer; i++)
	{
		user = gserver.m_userList[i].FindByUserIndex(userindex);
		if( user )
			break;
	}

	if( !user )
	{
		GAMELOG << init("CASH_BALANCE_ERROR")
				<< userindex;
		return;
	}

	GAMELOG << init("CASH_BALANCE_REP")
			<< returnCode << delim
			<< idname << delim
			<< cid << delim
			<< userindex << delim
			<< cashBalance << delim
			<< bonusPoint << end;

	// MSG_CONN_CASHiTEM_BALANCE_REP
	CNetMsg::SP rmsg(new CNetMsg);
	ConnCashItemBalanceRep(rmsg, user->m_index, (MSG_EX_CASHITEM_ERROR_TYPE) returnCode, cashBalance);
	SEND_Q(rmsg, user->m_descserver);
}

void CBillingClient::OnCanBuy(CBPacket::SP& msg)
{
	// MSG_BILLITEM_CANBUY_REP,
	// 응답	  : returnCode(uc) gameId(uc) userId(str:32) cpId(n) itemId(n) cashBalance(int)
	// returnCode에 따른 행동을 하시오
	unsigned char cpId, returnCode, count;
	int userindex;
	int  cashBalance, bonusPoint, ctId[MAX_PURCHASEITEM], i;
	CLCString idname(51);
	//char idname[MAX_ID_NAME_LENGTH + 1];

	RefMsg(msg) >> returnCode
				>> idname
				>> cpId
				>> userindex
				>> count;

	for(i = 0; i < MAX_PURCHASEITEM && i < count; i++)
	{
		RefMsg(msg) >> ctId[i];
	}

	RefMsg(msg) >> cashBalance
				>> bonusPoint;

	GAMELOG << init("CASH_ITEM_BUY");

	switch(returnCode)
	{
	case MSG_BILLITEM_ERROR_SUCCESS: // 성공
		GAMELOG << "MSG_BILLITEM_ERROR_SUCCESS" << delim;
		break;
	case MSG_BILLITEM_ERROR_LACKCASH: // 잔액부족
		GAMELOG << "MSG_BILLITEM_ERROR_LACKCASH" << delim;
		break;
	case MSG_BILLITEM_ERROR_NOTUSER: // 해당유저 없음
		GAMELOG << "MSG_BILLITEM_ERROR_NOTUSER" << delim;
		break;
	case MSG_BILLITEM_ERROR_NOTITEM: // 해당아이템 없음
		GAMELOG << "MSG_BILLITEM_ERROR_NOTITEM" << delim;
		break;
	case MSG_BILLITEM_ERROR_DATABASE: // DB 오류
		GAMELOG << "MSG_BILLITEM_ERROR_DATABASE" << delim;
		break;
	case MSG_BILLITEM_ERROR_PACKET: // 패킷 오류
		GAMELOG << "MSG_BILLITEM_ERROR_PACKET" << delim;
		break;
	case MSG_BILLITEM_ERROR_ETC: // 기타 오류
		GAMELOG << "MSG_BILLITEM_ERROR_ETC" << delim;
		break;
	case MSG_BILLITEM_ERROR_NOTACCOUNT: // 결재 권한 없음
		GAMELOG << "MSG_BILLITEM_ERROR_NOTACCOUNT" << delim;
		break;
	default:
		GAMELOG << "KNOWN_ERROR" << delim;
		break;
	}

	GAMELOG << idname << delim
			<< cpId << delim
			<< userindex << delim
			<< count << delim;

	for(i = 0; i < MAX_PURCHASEITEM && i < count; i++)
	{
		GAMELOG << ctId[i] << delim;
	}

	GAMELOG << cashBalance << delim
			<< bonusPoint << end;

	CUser* user = NULL;
	for(i = 0; i < gserver.m_maxSubServer; i++)
	{
		user = gserver.m_userList[i].FindByUserIndex(userindex);
		if(user) break;
	}

	if(!user)
	{
		GAMELOG << init("CASH_PURCHASE_REP_ERROR-NotUser")
				<< userindex << end;
		return;
	}

	if( returnCode == MSG_BILLITEM_ERROR_SUCCESS)
	{
		// 차감 요청을 함
		// DB의 구매필드를 추가, 시리얼 번호 설정후 거시기 해서 거시기 하시오

		CDBCmd cmd;
		cmd.Init(&gserver.m_dbuser);


		//		a_index,	a_user_idx,a_server,a_ctid,a_serial,a_pdate,a_use_char_idx, a_use_date
		std::string insertSql = boost::str(boost::format("INSERT INTO t_purchase0%d ( a_user_idx, a_server, a_ctid, a_serial, a_pdate, a_ip) VALUES ") % (userindex % 10));
		int i = 0;
		for(i = 0; i < count && i < MAX_PURCHASEITEM && ctId[i] != -1; i++)
		{
			insertSql += boost::str(boost::format(" (%d, %d, %d, -1, now(), '%s'), ") % user->m_index % gserver.m_serverno % ctId[i] % (const char*) user->m_ip );
		}

		int pos = insertSql.rfind(",");
		insertSql.erase(pos);

		cmd.BeginTrans();

		cmd.SetQuery(insertSql);
		if( !cmd.Update() )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnCashItemPurchaseRep(rmsg, userindex, MSG_EX_CASHITEM_ERROR_CONN, cashBalance);
			SEND_Q(rmsg, user->m_descserver);

			GAMELOG << init("CASH_PURCHASE_REP_ERROR-NotUpdateDB")
					<< mysql_error(&gserver.m_dbuser) << delim
					<< userindex << end;

			cmd.Rollback();
			return;
		}

		int serial = cmd.insertid() * 10 + userindex % 10;

		std::string updateQuery = boost::str(boost::format(
				"UPDATE t_purchase0%d SET a_serial = %d WHERE a_serial = -1 ")
											 % (userindex % 10) % serial);


		cmd.SetQuery(updateQuery);
		if( !cmd.Update() )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnCashItemPurchaseRep(rmsg, userindex, MSG_EX_CASHITEM_ERROR_CONN, cashBalance);
			SEND_Q(rmsg, user->m_descserver);

			GAMELOG << init("CASH_PURCHASE_REP_ERROR-NotUpdateDB")
					<< mysql_error(&gserver.m_dbuser) << delim
					<< userindex << end;

			cmd.Rollback();
			return;
		}

		cmd.Commit();

		if( gserver.m_billing.IsRunning() )
		{
			CBPacket::SP rmsg(new CBPacket);
			BillBuyReqMsg(rmsg, userindex, user->m_name, serial, ctId, user->m_ip, count);
			gserver.m_billing.WriteToOutput(rmsg);

			user->m_descserver->WaitBillReply(serial);

			return;
		}
		else
		{
			if(user)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ConnCashItemPurchaseRep(rmsg, userindex, MSG_EX_CASHITEM_ERROR_BILL, cashBalance);
				SEND_Q(rmsg, user->m_descserver);
			}
			else
			{
				GAMELOG << init("CASH_PURCHASE_REP_ERROR-NotUser")
						<< userindex << end;
			}

			// 빌링 서버가 돌지 않고 있으므로  롤백 : 해당 시리얼을 가진 아이템 삭제
			std::string deleteQuery = boost::str(boost::format(
					"DELETE FROM t_purchase0%d WHERE a_serial = %d")
												 % (userindex % 10) % serial);

			cmd.SetQuery(deleteQuery);

			if( !cmd.Update() )
			{
				GAMELOG << init("CASH_PURCHASE_REP_ERROR-RollBack-update fail")
						<< userindex << delim;

				for(i = 0; i < MAX_PURCHASEITEM; i++)
				{
					GAMELOG << ctId[i] << delim;
				}
				GAMELOG << end;
			}
		}

		return;
	}

	// 에러 메세지에 해당하는 리플라이
	CNetMsg::SP rmsg(new CNetMsg);
	ConnCashItemPurchaseRep(rmsg, userindex, (MSG_EX_CASHITEM_ERROR_TYPE) returnCode, cashBalance);
	SEND_Q(rmsg, user->m_descserver);
}

void CBillingClient::OnBuy(CBPacket::SP& msg)
{
	//MSG_BILLITEM_BUY_REP,		// 응답	  : returnCode(uc) gameId(uc) userId(str:32) cpId(n) itemId(n) cashBalance(int)
	unsigned char cpId, returnCode, count;
	int userindex;
	int  cashBalance, ctId[MAX_PURCHASEITEM], i, serial, bonuspoint;
	CLCString idname(51);

	serial = msg->m_serial;

	RefMsg(msg) >> returnCode
				>> idname
				>> cpId
				>> userindex
				>> count;

	for(i = 0; i < MAX_PURCHASEITEM && i < count; i++)
	{
		RefMsg(msg) >> ctId[i];

		// 현재 구입 성공한 상품 인덱스로 커넥터가 가지고 있는 한정판매 리스트를 찾아서
		// 있으면 팔린 갯수를 증가 시키고 모든 게임서버로 메세지를 보낸다.
		CServer::map_limitsell_t::iterator it = gserver.m_limitSell.find(ctId[i]);
		if( it != gserver.m_limitSell.end() )
		{
			CLimitSell* limit = it->second;
			limit->IncreaseSell();
			CNetMsg::SP rmsg(new CNetMsg);
			LimitCatalogMsg(rmsg, limit);
			gserver.SendAllGameServer(rmsg);
			gserver.m_changeLimit = true;
		}
	}

	RefMsg(msg) >> cashBalance
				>> bonuspoint;

	CUser* user = NULL;
	for(i = 0; i < gserver.m_maxSubServer; i++)
	{
		user = gserver.m_userList[i].FindByUserIndex(userindex);
		if(user) break;
	}

	if(user)
	{
		user->m_descserver->WaitBillReply(0);
	}
	else
	{
		GAMELOG << init("CASH_PURCHASE_REP_ERROR-NotUser")
				<< userindex << end;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	if( returnCode == MSG_BILLITEM_ERROR_SUCCESS)
	{
		// 해당 시리얼을 가진 레코드를 컨펌한다.
		std::string updateQuery = boost::str(boost::format(
				"UPDATE t_purchase0%d SET a_confirm = 1 WHERE a_serial = %d ") % (userindex % 10) % serial);

		cmd.SetQuery(updateQuery);

		if( !cmd.Update() )
		{
			GAMELOG << init("Confirm Error")
					<< serial
					<< end;
		}

		if( user )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnCashItemPurchaseRep(rmsg, userindex, (MSG_EX_CASHITEM_ERROR_TYPE) returnCode, cashBalance);
			SEND_Q(rmsg, user->m_descserver);
		}
		return;
	}
	else
	{
		// 시리얼 값을 가진 t_purchase를 모두 롤백하고
		// 에러코드에 따른 값을 유저에게 전달한다

		std::string deleteQuery = boost::str(boost::format(
				"DELETE FROM t_purchase0%d WHERE a_serial = %d ") % (userindex % 10) % serial);

		cmd.SetQuery(deleteQuery);
		if( !cmd.Update() )
		{
			GAMELOG << init("CASH_PURCHASE_REP_ERROR-RollBack-update fail")
					<< userindex << delim;

			for(i = 0; i < MAX_PURCHASEITEM; i++)
			{
				GAMELOG << ctId[i] << delim;
			}
			GAMELOG << end;
		}

		if( user )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnCashItemPurchaseRep(rmsg, userindex, (MSG_EX_CASHITEM_ERROR_TYPE) returnCode, cashBalance);
			SEND_Q(rmsg, user->m_descserver);
		}
		return;
	}
}

void CBillingClient::WriteToOutput(CBPacket::SP msg)
{
	if (msg->m_mtype == -1)
		return;

	if (service_ == NULL)
		return;

	LOG_INFO("OOO (S): type[%d] serial[%d] size[%d]", msg->m_mtype, msg->m_serial, msg->m_size);

	service_->deliver(msg);
}

void CBillingClient::onClose(rnSocketIOService* service)
{
	bnf::instance()->RemoveSession(service);
	service_ = NULL;

	// 재접속 타이머 생성
	session_handle handle = bnf::instance()->CreateMSecTimer(2 * 1000, (void *)CBillingClientConnectTimer::instance());
}

void CBillingClient::onConnect(rnSocketIOService* service)
{
	service_ = service;

	CBPacket::SP rmsg(new CBPacket);
	BillConnectMsg(rmsg);
	WriteToOutput(rmsg);

	LOG_INFO("connected to billing server");
}

void CBillingClient::onConnectFail(rnSocketIOService* service)
{
	LOG_INFO("Can't connected to billing server : ip[%s] port[%s]",
			 gserver.m_config.Find("Billing Server", "IP"),
			 gserver.m_config.Find("Billing Server", "Port"));
}

void CBillingClient::connect()
{
	// 빌링 서버에 접속 시도
	std::string billing_host = gserver.m_config.Find("Billing Server", "IP");
	int billing_port = atoi(gserver.m_config.Find("Billing Server", "Port"));
	bnf::instance()->CreateAsyncConnect(billing_host, billing_port, &(gserver.m_billing), CONNECT_TYPE_FOR_CBILLING);
}

//////////////////////////////////////////////////////////////////////////
CBillingClientConnectTimer* CBillingClientConnectTimer::instance()
{
	static CBillingClientConnectTimer __instance;
	return &__instance;
}

void CBillingClientConnectTimer::operate(rnSocketIOService* service)
{
	// 현 타이머 삭제
	bnf::instance()->RemoveSession(service);

	// 빌링 서버에 재 접속
	gserver.m_billing.connect();
}

#endif