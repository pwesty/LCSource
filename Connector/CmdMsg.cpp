#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "CmdMsg.h"

void LoginRepMsg(CNetMsg::SP& msg, MSG_CONN_ERRORCODE errcode, const char* id, CUser* user)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char)MSG_CONN_LOGIN_REP
				<< id
				<< (unsigned char)errcode;
	if (errcode != MSG_CONN_ERROR_SUCCESS)
		return ;
	RefMsg(msg) << user->m_index
				<< (unsigned char)user->m_usertype
				<< (unsigned char)user->m_paytype
				<< (unsigned char)user->m_location
				<< user->m_timeremain;

	RefMsg(msg) << user->m_userFlag;//0627

#ifdef CHARDEL_CHECKID
#ifndef JUMIN_DB_CRYPT
	RefMsg(msg) << user->m_identification;
#endif
#endif
	RefMsg(msg) << user->m_proSite;
}

void ConnCashItemBalanceRep(CNetMsg::SP& msg, int userindex, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalance)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_BALANCE_REP
				<< userindex
				<< (unsigned char) errorcode
				<< cashBalance;
}

void ConnCashItemPurchaseRep(CNetMsg::SP& msg, int userindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, int cash)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_PURCHASE_REP
				<< userindex
				<< (unsigned char) errorCode
				<< cash;
}

void ConnCashItemBringRep(CNetMsg::SP& msg, int charindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, bool bPresent, int count, int ctid[])
{
	msg->Init(MSG_CONN_REP);
	if( bPresent )
		RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_GIFT_RECV;
	else
		RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_BRING_REP;

	RefMsg(msg) << charindex
				<< (unsigned char) errorCode
				<< count;

	for(int i = 0; i < count; i++)
	{
		RefMsg(msg) << ctid[i];
	}
}

void ConnCashItemPurchaselistRep(CNetMsg::SP& msg, int charindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, int count, int idx[], int ctid[])
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_PURCHASELIST_REP
				<< charindex
				<< (unsigned char) errorCode
				<< count;

	for(int i = 0; i < count; i++)
	{
		RefMsg(msg) << idx[i]
					<< ctid[i];
	}
}

void ConnCashItemPurchaseHistoryRep(CNetMsg::SP& msg, int userindex, int charindex, int y, int m, int d)
{
	CLCString sql(4096);
	int ctid = -1, preCtid = -1, i;

	sql.Format("select * from t_purchase0%d where a_user_idx = %d and a_server = %d "
			   "and year(a_pdate) = %d and month(a_pdate) = %d and dayofmonth(a_pdate) = %d order by a_ctid ", userindex % 10, userindex, gserver.m_serverno, y, m, d);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	cmd.SetQuery(sql);

	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_PURCHASEHISTORY_REP;

	i = 0;
	if( !cmd.Open() )
	{
		RefMsg(msg) << charindex
					<< (unsigned char) MSG_EX_CASHITEM_ERROR_CONN << i;
		return;
	}

	if( !cmd.MoveFirst() )
	{
		RefMsg(msg) << charindex
					<< (unsigned char) MSG_EX_CASHITEM_ERROR_SUCCESS << i;
		return;
	}

	CNetMsg ctmsg;
	ctmsg.Init();
	int j = 1;

	do
	{
		cmd.GetRec("a_ctid", ctid);

		if( preCtid != -1 && preCtid != ctid )
		{
			ctmsg << i
				  << preCtid;
			i = 0;
			j++;
		}

		preCtid = ctid;
		i++;
	}
	while(cmd.MoveNext() );

	ctmsg << i << ctid;

	RefMsg(msg) << charindex
				<< (unsigned char) MSG_EX_CASHITEM_ERROR_SUCCESS << j << ctmsg;
}

void ConnCashItemPresentHistoryRep(CNetMsg::SP& msg, int userindex, int charindex, int y, int m, int d, bool bSend)
{
	// 응답				: errorcode(uc) count(n) ctid(n) sendcharName(str)
	std::string sql = "";
	sql.reserve(10240);

	std::string charName = "";
	charName.reserve(MAX_CHAR_NAME_LENGTH + 1);


	if (bSend)
	{
		int i = 0;
		for (i = 0; i < 9; ++i)
		{
			sql += boost::str(boost::format(
								  "SELECT a_ctid, a_recv_char_name AS a_charName FROM t_gift%02d WHERE"
								  "a_send_user_idx = %d AND a_server = %d AND year(a_send_date) = %d AND month(a_send_date) = %d AND dayofmonth(a_send_date) = %d UNION ALL ")
							  % i % userindex % gserver.m_serverno % y % m % d);
		}
		sql += boost::str(boost::format(
							  "SELECT a_ctid, a_recv_char_name AS a_charName FROM t_gift%02d WHERE"
							  "a_send_user_idx = %d AND a_server = %d AND year(a_send_date) = %d AND month(a_send_date) = %d AND dayofmonth(a_send_date) = %d")
						  % i % userindex % gserver.m_serverno % y % m % d);
	}
	else
	{
		sql = boost::str(boost::format(
							 "SELECT a_ctid, a_send_char_name AS a_charName from t_gift%02d WHERE a_recv_user_idx = %d and a_server = %d "
							 "AND year(a_send_date) = %d AND month(a_send_date) = %d AND dayofmonth(a_send_date) = %d ORDER BY a_send_date ")
						 % (userindex % 10) % userindex % gserver.m_serverno % y % m % d);
	}




	CDBCmd cmd;
	cmd.Init(&gserver.m_dbuser);

	msg->Init(MSG_CONN_REP);

	if( bSend )
		RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_GIFT_SENDHISTORY;
	else
		RefMsg(msg) << (unsigned char) MSG_CONN_CASHITEM_GIFT_RECVHISTORY;

	RefMsg(msg) << charindex;

	cmd.SetQuery(sql);

	if( cmd.Open() && cmd.MoveFirst() )
	{
		RefMsg(msg) << (unsigned char) MSG_EX_CASHITEM_ERROR_SUCCESS;

		int count = cmd.m_nrecords;

		RefMsg(msg) << count;
		do
		{
			int ctid = -1;
			cmd.GetRec("a_ctid", ctid);
			cmd.GetRec("a_charName", charName);

			RefMsg(msg) << ctid
						<< charName.c_str();
		}
		while( cmd.MoveNext() );
	}
	else
	{
		RefMsg(msg) << (unsigned char) MSG_EX_CASHITEM_ERROR_SUCCESS;
		RefMsg(msg) << (int) 0;
	}
}

void LimitCatalogMsg(CNetMsg::SP& msg, CLimitSell* limit)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char) MSG_CONN_LIMIT_CATALOG;

	if( !limit )
	{
		int count = gserver.m_limitSell.size();
		RefMsg(msg) << count;

		CServer::map_limitsell_t::iterator it = gserver.m_limitSell.begin();
		CServer::map_limitsell_t::iterator endit = gserver.m_limitSell.end();
		for(; it != endit; ++it)
		{
			CLimitSell* sellList = it->second;
			RefMsg(msg) << sellList->GetIndex()
						<< sellList->GetSell();
		}
	}
	else
	{
		RefMsg(msg) << 1
					<< limit->GetIndex()
					<< limit->GetSell();
	}
}

void PlayerRepMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char)MSG_CONN_PLAYER_REP
				<< gserver.m_serverno
				<< gserver.m_maxSubServer
				<< gserver.m_hardcore_flag_in_connector;

	// zone 0번을 가지고 있는 넘을 찾는다
	CDescriptor* d = gserver.m_desclist;
	CDescriptor** result = new CDescriptor*[gserver.m_maxSubServer];
	memset(result, 0, sizeof(CDescriptor*) * gserver.m_maxSubServer);

	int i = 0;

	while (d)
	{
		if (d->m_bStartServer)
		{
			result[d->m_subno - 1] = d;
		}
		d = d->m_pNext;
	}

	for (i = 1; i <= gserver.m_maxSubServer; i++)
	{
		RefMsg(msg) << i;
		if (result[i - 1])
		{
			RefMsg(msg) << gserver.m_userList[result[i - 1]->m_subno - 1].m_count;

#ifdef SETTING_IF_INNER_IP_NEW // 외부 아이피와 내부 아이피가 나뉘면 사용, 새로운 버젼 ...yhj
			CLCString ex_ip(100);
			ex_ip.Format("EX_IP_%d", i);
			if(  strcmp(gserver.m_config.Find("GameServer", (const char*)ex_ip),"") != 0 )
			{
				result[i - 1]->m_ipAddr.Format("%s", gserver.m_config.Find("GameServer", (const char*)ex_ip));
				RefMsg(msg) << result[i - 1]->m_ipAddr;
			}
			else
				RefMsg(msg) << result[i - 1]->m_ipAddr;
#else
			RefMsg(msg) << result[i - 1]->m_ipAddr;
#endif // SETTING_IF_INNER_IP_NEW

			RefMsg(msg) << result[i - 1]->m_portNumber;
		}
		else
		{
			RefMsg(msg) << (int)-1
						<< ""
						<< (int)0;
		}
	}

	delete [] result;
}

void RankerRepMsg(CNetMsg::SP& msg, int charindex)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char)MSG_CONN_RANKER_REP
				<< charindex;
}

#ifdef EVENT_PACKAGE_ITEM
void CouponConfirmMsg(CNetMsg::SP& msg, int charindex, MSG_EVENT_ERROR_TYPE subtype, int cIndex, int type)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char) MSG_CONN_COUPON_CONFIRM
				<< charindex
				<< (unsigned char) subtype
				<< cIndex
				<< type;
}

void CouponUseMsg(CNetMsg::SP& msg, int charindex, MSG_EVENT_ERROR_TYPE subtype, int type)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char) MSG_CONN_COUPON_USE
				<< charindex
				<< (unsigned char) subtype
				<< type;
}

#endif // EVENT_PACKAGE_ITEM

void MoveServerOKMsg(CNetMsg::SP& msg, int nUserIndex)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char)MSG_CONN_MOVESERVER_OK
				<< nUserIndex;
}

void ConnEventGomdori2007StatusMsg(CNetMsg::SP& msg, int nCharIndex, int nCount, int* nStatus)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char)MSG_CONN_EVENT_GOMDORI_2007
				<< (unsigned char)MSG_CONN_EVENT_GOMDORI_2007_STATUS
				<< nCharIndex
				<< nCount;
	int i;
	for (i = 0; i < nCount; i++)
		RefMsg(msg) << nStatus[i];
}

void ConnEventXmas2007Msg( CNetMsg::SP& msg, MSG_CONN_EVENT_XMAS_2007_TYPE subtype )
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char)MSG_CONN_EVENT_XMAS_2007
				<< (unsigned char)subtype;
}

#ifdef DEV_EVENT_PROMOTION2
void ConnPromotion2RepErrorMsg( CNetMsg::SP& msg, int charIndex, int errorType)
{
	msg->Init( MSG_CONN_REP);
	RefMsg(msg) << (unsigned char) MSG_CONN_EVENT_PROMOTION2
				<< charIndex
				<< (unsigned char) errorType;
}
#endif

void ConnWishlistRep(CNetMsg::SP& msg, int count, int ctid[], int useridx, int charidx)
{
	int i;
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char)MSG_CONN_WISHLIST
				<< (unsigned char)MSG_WISHLIST_TYPE_LIST_REP
				<< useridx
				<< charidx
				<< count;
	for(i = 0; i < count; i++)
	{
		RefMsg(msg) << ctid[i];
	}
}

void ConnWishlistSaveRep(CNetMsg::SP& msg, int useridx, int charidx, int errorcode, int ctid)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char)MSG_CONN_WISHLIST
				<< (unsigned char)MSG_WISHLIST_TYPE_SAVE_REP
				<< useridx
				<< charidx
				<< (unsigned char)errorcode
				<< ctid;
}

void ConnWishlistDelRep(CNetMsg::SP& msg, int useridx, int charidx, int errorcode, int ctid)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char)MSG_CONN_WISHLIST
				<< (unsigned char)MSG_WISHLIST_TYPE_DEL_REP
				<< useridx
				<< charidx
				<< (unsigned char)errorcode
				<< ctid;
}


#ifdef EVENT_USER_COMEBACK
void ConnEventUserComebackRepMsg( CNetMsg::SP& msg, char errortype, int userindex, int charindex)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char)MSG_CONN_EVENT_USER_COMEBACK
				<< errortype
				<< userindex
				<< charindex;
}
#endif
#ifdef EVENT_EUR2_PROMOTION
void ConnEUR2PromotionRepMsg(CNetMsg::SP& msg, MSG_EVENT_EUR2_PROMOTION_ERROR_TYPE type, int charindex)
{
	msg->Init(MSG_CONN_REP);
	RefMsg(msg) << (unsigned char)MSG_CONN_EVENT_EUR2_PROMOTION
				<< type
				<< charindex;
}
#endif
//