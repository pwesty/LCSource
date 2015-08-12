#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Server.h"
#include "doFunc.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"
#include "Log.h"
#include "../ShareLib/packetType/ptype_tradeAgent_system.h"
#include "../ShareLib/packetType/ptype_express_system.h"
#include "MessengerInSubHelper.h"
#include "../ShareLib/LogInOutManager.h"

void getRTradeAgentItem(CDBCmd& cmd, TradeAgentItem& item);
void insertItemToLCE(TradeAgentItem& item, int charIndex, int sendType, LONGLONG tradeagent_nas = -1);
void insertNasToLCE(GoldType_t nas, int charIndex, const char*sender, int sendType, int tradeagent_itemIndex = -1, int tradeagent_itemCount = -1);
void sendMessageToUserSellOrReturned(int type, int charIndex, int itemIndex, int itemCount);
void do_command_TradeAgent_ChangeFee(CNetMsg::SP& msg, CDescriptor* desc);
void do_command_TradeAgent_ChangeDeposit(CNetMsg::SP& msg, CDescriptor* desc);

void do_TradeAgent(CNetMsg::SP& msg, CDescriptor* dest)
{
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);

	switch (pBase->subType)
	{
	case MSG_TRADEAGENT_LIST:
		do_command_TradeAgent_List(msg, dest);
		break;
	case MSG_TRADEAGENT_REG_LIST:
		do_command_TradeAgent_RegList(msg, dest);
		break;
	case MSG_TRADEAGENT_REG:
		do_command_TradeAgent_Reg(msg, dest);
		break;
	case MSG_TRADEAGENT_REG_CANCEL:
		do_command_TradeAgent_RegCancel(msg, dest);
		break;
	case MSG_TRADEAGENT_BUY:
		do_command_TradeAgent_Buy(msg, dest);
		break;
	case MSG_TRADEAGENT_LIKE_LIST:
		do_command_TradeAgent_LikeList(msg, dest);
		break;
	case MSG_TRADEAGENT_LIKE_REG:
		do_command_TradeAgent_LikeReg(msg, dest);
		break;
	case MSG_TRADEAGENT_LIKE_CANCEL:
		do_command_TradeAgent_LikeCancel(msg, dest);
		break;
	case MSG_TRADEAGENT_CHANGE_FEE:
		do_command_TradeAgent_ChangeFee(msg, dest);
		break;
	case MSG_TRADEAGENT_CHANGE_DEPOSIT:
		do_command_TradeAgent_ChangeDeposit(msg, dest);
		break;
	}
}

void do_command_TradeAgent_List(CNetMsg::SP& msg, CDescriptor* desc)
{
	RequestClient::TradeAgentSystemList *p = reinterpret_cast<RequestClient::TradeAgentSystemList *>(msg->m_buf);

	char sql[1024];
	memset (sql, 0x00, sizeof(sql));
	sprintf(sql, "select t_tradeagent.*, t_tradeagent_iteminfo.*, UNIX_TIMESTAMP(a_regist_date) as regist_date, UNIX_TIMESTAMP(a_expire_date) as expire_date from t_tradeagent  LEFT JOIN t_tradeagent_iteminfo ON t_tradeagent.a_item_serial = t_tradeagent_iteminfo.a_index where ");
	char buf[256];

	//기타 아이템 검색 할 때
	if( p->itemTypeTab == 0 || p->itemTypeTab == 1 )
	{
		sprintf(buf, "t_tradeagent_iteminfo.a_item_type = %d ", p->itemTypeTab);
		strcat(sql, buf);

		if(p->minItemPlus > 0)
		{
			sprintf(buf, "and t_tradeagent.a_item_plus >= %d ", p->minItemPlus);
			strcat(sql, buf);
		}
		if(p->maxItemPlus > 0)
		{
			sprintf(buf, "and t_tradeagent.a_item_plus <= %d ", p->maxItemPlus);
			strcat(sql, buf);
		}
		if(p->itemSubType > 0)
		{
			sprintf(buf, "and t_tradeagent_iteminfo.a_item_subtype = %d ", p->itemSubType - 1);
			strcat(sql, buf);
		}
		if(p->JobClass > 0)
		{
			sprintf(buf, "and (t_tradeagent_iteminfo.a_item_job & (1 << %d)) ", p->JobClass - 1);
			strcat(sql, buf);
		}
		if(p->maxAttackValue > 0)
		{
			sprintf(buf, "and t_tradeagent_iteminfo.a_num0 <= %d ", p->maxAttackValue);
			strcat(sql, buf);
		}
		if(p->minAttackValue > 0)
		{
			sprintf(buf, "and t_tradeagent_iteminfo.a_num0 >= %d ", p->minAttackValue);
			strcat(sql, buf);
		}
		if(p->maxMagicValue > 0)
		{
			sprintf(buf, "and t_tradeagent_iteminfo.a_num1 <= %d ", p->maxMagicValue);
			strcat(sql, buf);
		}
		if(p->minMagicValue > 0)
		{
			sprintf(buf, "and t_tradeagent_iteminfo.a_num1 >= %d ", p->minMagicValue);
			strcat(sql, buf);
		}
		if(p->maxLevel > 0)
		{
			sprintf(buf, "and t_tradeagent_iteminfo.a_item_level <= %d ", p->maxLevel);
			strcat(sql, buf);
		}
		if(p->minLevel > 0)
		{
			sprintf(buf, "and t_tradeagent_iteminfo.a_item_level >= %d ", p->minLevel);
			strcat(sql, buf);
		}
		if(p->maxSocketCount > 0)
		{
			sprintf(buf, "and t_tradeagent.a_socketCount <= %d ", p->maxSocketCount);
			strcat(sql, buf);
		}
		if(p->minSocketCount > 0)
		{
			sprintf(buf, "and t_tradeagent.a_socketCount >= %d ", p->minSocketCount);
			strcat(sql, buf);
		}
		if(p->itemGrade < eITEM_GRADE_MAX)
		{
			sprintf(buf, "and t_tradeagent.a_rare_grade = %d ", p->itemGrade);
			strcat(sql, buf);
		}
	}
	else
	{
		sprintf(buf, " (t_tradeagent_iteminfo.a_item_type != 0 and t_tradeagent_iteminfo.a_item_type != 1) ");
		strcat(sql, buf);
	}

	if(strlen(p->itemName) > 1)
	{
		char tname[128];
		mysql_real_escape_string(&gserver.m_dbchar, tname, p->itemName, strlen(p->itemName));

		sprintf(buf, "and t_tradeagent_iteminfo.a_item_name like '%%%s%%' ", tname);
		strcat(sql, buf);
	}
	if(p->maxPrice > 0)
	{
#ifdef CIRCLE_WINDOWS
		sprintf(buf, "and t_tradeagent.a_totalmoney <= %I64d ", p->maxPrice);
#else
		sprintf(buf, "and t_tradeagent.a_totalmoney <= %lld ", p->maxPrice);
#endif
		strcat(sql, buf);
	}
	if(p->minPrice > 0)
	{
#ifdef CIRCLE_WINDOWS
		sprintf(buf, "and t_tradeagent.a_totalmoney >= %I64d ", p->minPrice);
#else
		sprintf(buf, "and t_tradeagent.a_totalmoney >= %lld ", p->minPrice);
#endif
		strcat(sql, buf);
	}

	sprintf(buf, "and t_tradeagent.a_sell_charindex != %d ", p->charIndex);
	strcat(sql, buf);

	int selectLimit = (p->pageNo - 1) * 10;
	sprintf(buf, "order by a_regist_date limit %d, 11", selectLimit);
	strcat ( sql, buf );

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	cmd.SetQuery(sql);

	if( cmd.Open() == false )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::TradeAgentSystemError *packet = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(rmsg->m_buf);
		makeTradeAgentError(rmsg, ResponseClient::TRADEAGENT_ERROR_NOT_EXIST_ITEM);
		packet->charIndex = p->charIndex;
		rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemError));
		SEND_Q(rmsg, desc);
		return;
	}
	cmd.MoveFirst();

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::TradeAgentSystemList *packet = reinterpret_cast<ResponseClient::TradeAgentSystemList*>(rmsg->m_buf);
	packet->type = MSG_TRADEAGENT;
	packet->subType = MSG_TRADEAGENT_LIST;
	packet->charIndex = p->charIndex;
	packet->pageNo = p->pageNo;
	packet->curServerTime = (int)time(NULL);

	if(cmd.m_nrecords == 11)
	{
		packet->nextPage = true;
		packet->count = 10;
	}
	else
	{
		packet->nextPage = false;
		packet->count = cmd.m_nrecords;
	}

	for(int i = 0; i<packet->count; i++)
	{
		cmd.GetRec("a_index", packet->list[i].index);
		cmd.GetRec( "a_item_serial", packet->list[i].item_index);
		cmd.GetRec( "a_item_plus", packet->list[i].plus);
		cmd.GetRec( "a_item_plus2", packet->list[i].plus2);
		cmd.GetRec( "a_item_flag", packet->list[i].flag);

		cmd.GetRec( "a_option_count", packet->list[i].option_count);
		cmd.GetRec( "a_option_0_type", packet->list[i].option_type[0]);
		cmd.GetRec( "a_option_0_level", packet->list[i].option_level[0]);
		cmd.GetRec( "a_option_1_type", packet->list[i].option_type[1]);
		cmd.GetRec( "a_option_1_level", packet->list[i].option_level[1]);
		cmd.GetRec( "a_option_2_type", packet->list[i].option_type[2]);
		cmd.GetRec( "a_option_2_level", packet->list[i].option_level[2]);
		cmd.GetRec( "a_option_3_type", packet->list[i].option_type[3]);
		cmd.GetRec( "a_option_3_level", packet->list[i].option_level[3]);
		cmd.GetRec( "a_option_4_type", packet->list[i].option_type[4]);
		cmd.GetRec( "a_option_4_level", packet->list[i].option_level[4]);
		cmd.GetRec( "a_quantity", packet->list[i].quantity);
		cmd.GetRec( "a_totalmoney", packet->list[i].nas);
		cmd.GetRec( "a_item_level", packet->list[i].item_level);
		cmd.GetRec("a_item_origin_0", packet->list[i].item_origin[0]);
		cmd.GetRec("a_item_origin_1", packet->list[i].item_origin[1]);
		cmd.GetRec("a_item_origin_2", packet->list[i].item_origin[2]);
		cmd.GetRec("a_item_origin_3", packet->list[i].item_origin[3]);
		cmd.GetRec("a_item_origin_4", packet->list[i].item_origin[4]);
		cmd.GetRec("a_item_origin_5", packet->list[i].item_origin[5]);
		cmd.GetRec( "a_socketCount", packet->list[i].socketCount);
		cmd.GetRec( "a_socket0", packet->list[i].socket[0]);
		cmd.GetRec( "a_socket1", packet->list[i].socket[1]);
		cmd.GetRec( "a_socket2", packet->list[i].socket[2]);
		cmd.GetRec( "a_socket3", packet->list[i].socket[3]);
		cmd.GetRec( "a_socket4", packet->list[i].socket[4]);
		cmd.GetRec( "a_socket5", packet->list[i].socket[5]);
		cmd.GetRec( "a_socket6", packet->list[i].socket[6]);
#ifdef DURABILITY
		cmd.GetRec( "a_now_dur", packet->list[i].now_durability);
		cmd.GetRec( "a_max_dur", packet->list[i].max_durability);
#endif
		cmd.GetRec( "a_num0", packet->list[i].item_num0);
		cmd.GetRec( "a_num1", packet->list[i].item_num1);
		cmd.GetRec( "regist_date", packet->list[i].regist_date);
		cmd.GetRec( "expire_date", packet->list[i].expire_date);
		cmd.MoveNext();
	}

	rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemList) + (packet->count * sizeof(TradeAgentItem)));
	SEND_Q(rmsg, desc);
}
void do_command_TradeAgent_RegList(CNetMsg::SP& msg, CDescriptor* desc)
{
	RequestClient::TradeAgentSystemRegList *p = reinterpret_cast<RequestClient::TradeAgentSystemRegList *>(msg->m_buf);

	char sql[1024];
	memset (sql, 0x00, sizeof(sql));

	sprintf(sql, "select *, UNIX_TIMESTAMP(a_regist_date) as regist_date, UNIX_TIMESTAMP(a_expire_date) as expire_date from t_tradeagent where a_sell_charindex = %d order by a_regist_date", p->charIndex);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);

	if( cmd.Open() == false )
	{
		return;
	}
	cmd.MoveFirst();

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::TradeAgentSystemRegList * packet = reinterpret_cast<ResponseClient::TradeAgentSystemRegList*>(rmsg->m_buf);
	packet->type = MSG_TRADEAGENT;
	packet->subType = MSG_TRADEAGENT_REG_LIST;

	packet->count = cmd.m_nrecords;
	packet->Guarantee = (packet->count + 1) * gserver.m_tradeAgentDeposit;
	packet->charIndex = p->charIndex;
	packet->tradeAgentFee = gserver.m_tradeAgentFee;
	packet->curServerTime = (int)time(NULL);

	for(int i = 0 ; i < packet->count; i++)
	{
		cmd.GetRec( "a_index"		, packet->list[i].index);
		cmd.GetRec( "a_item_serial"	, packet->list[i].item_index);
		cmd.GetRec( "a_item_plus"	, packet->list[i].plus);
		cmd.GetRec( "a_item_plus2"	, packet->list[i].plus2);
		cmd.GetRec( "a_item_flag"	, packet->list[i].flag);

		cmd.GetRec( "a_option_count"	, packet->list[i].option_count);
		cmd.GetRec( "a_option_0_type"	, packet->list[i].option_type[0]);
		cmd.GetRec( "a_option_0_level"	, packet->list[i].option_level[0]);
		cmd.GetRec( "a_option_1_type"	, packet->list[i].option_type[1]);
		cmd.GetRec( "a_option_1_level"	, packet->list[i].option_level[1]);
		cmd.GetRec( "a_option_2_type"	, packet->list[i].option_type[2]);
		cmd.GetRec( "a_option_2_level"	, packet->list[i].option_level[2]);
		cmd.GetRec( "a_option_3_type"	, packet->list[i].option_type[3]);
		cmd.GetRec( "a_option_3_level"	, packet->list[i].option_level[3]);
		cmd.GetRec( "a_option_4_type"	, packet->list[i].option_type[4]);
		cmd.GetRec( "a_option_4_level"	, packet->list[i].option_level[4]);

		cmd.GetRec( "a_quantity"	, packet->list[i].quantity);
		cmd.GetRec( "a_totalmoney"	, packet->list[i].nas);

		cmd.GetRec( "regist_date",  packet->list[i].regist_date);
		cmd.GetRec( "expire_date", packet->list[i].expire_date);

		cmd.GetRec("a_item_origin_0", packet->list[i].item_origin[0]);
		cmd.GetRec("a_item_origin_1", packet->list[i].item_origin[1]);
		cmd.GetRec("a_item_origin_2", packet->list[i].item_origin[2]);
		cmd.GetRec("a_item_origin_3", packet->list[i].item_origin[3]);
		cmd.GetRec("a_item_origin_4", packet->list[i].item_origin[4]);
		cmd.GetRec("a_item_origin_5", packet->list[i].item_origin[5]);

		cmd.GetRec("a_socketCount", packet->list[i].socketCount);
		cmd.GetRec("a_socket0", packet->list[i].socket[0]);
		cmd.GetRec("a_socket1", packet->list[i].socket[1]);
		cmd.GetRec("a_socket2", packet->list[i].socket[2]);
		cmd.GetRec("a_socket3", packet->list[i].socket[3]);
		cmd.GetRec("a_socket4", packet->list[i].socket[4]);
		cmd.GetRec("a_socket5", packet->list[i].socket[5]);
		cmd.GetRec("a_socket6", packet->list[i].socket[6]);

#ifdef DURABILITY
		cmd.GetRec("a_now_dur", packet->list[i].now_durability);
		cmd.GetRec("a_max_dur", packet->list[i].max_durability);
#endif
		cmd.MoveNext();
	}

	rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemRegList) + (packet->count * sizeof(TradeAgentItem)));
	SEND_Q(rmsg, desc);
}
void do_command_TradeAgent_Reg(CNetMsg::SP& msg, CDescriptor* desc)
{
	RequestClient::TradeAgentSystemRegServer *p = reinterpret_cast<RequestClient::TradeAgentSystemRegServer *>(msg->m_buf);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

#ifdef DURABILITY
	std::string sql = boost::str(boost::format("INSERT DELAYED INTO t_tradeagent( a_item_serial, a_item_serial2, a_item_plus, a_item_plus2, a_item_flag,"
								 " a_item_level, a_quantity, a_totalmoney, a_guaranty,"
								 " a_regist_date, a_expire_date, a_sell_charindex, a_sell_charname, a_option_count, a_option_0_type,"
								 " a_option_0_level,a_option_1_type, a_option_1_level,a_option_2_type, a_option_2_level,"
								 " a_option_3_type, a_option_3_level,a_option_4_type, a_option_4_level, a_item_origin_0, a_item_origin_1, a_item_origin_2, a_item_origin_3, a_item_origin_4, a_item_origin_5,"
								 " a_socketCount, a_socket0, a_socket1, a_socket2, a_socket3, a_socket4, a_socket5, a_socket6, a_now_dur, a_max_dur, a_rare_grade )"
								 "VALUES(%1%, '%2%', %3%, %4%, %5%, %6%, %7%, %8%, %9%, NOW(), DATE_ADD(now(), INTERVAL 5 DAY), %10%, '%11%', %12%, %13%, %14%, %15%, %16%,"
								 " %17%, %18%, %19%, %20%, %21%, %22%, %23%,"
								 " %24%, %25%, %26%, %27%, %28%, %29%, %30%, %31%, %32%, %33%, %34%, %35%, %36%, %37%, %38%, %39%)")
								 % p->item.item_index % p->item.serial % (int)p->item.plus % (int)p->item.plus2 % p->item.flag
								 % p->item.item_level % p->quantity % p->TotalMoney % p->Guarantee
								 % p->charIndex % p->charName
								 % p->item.option_count
								 % p->item.option_type[0] % p->item.option_level[0]
								 % p->item.option_type[1] % p->item.option_level[1]
								 % p->item.option_type[2] % p->item.option_level[2]
								 % p->item.option_type[3] % p->item.option_level[3]
								 % p->item.option_type[4] % p->item.option_level[4]
								 % p->item.item_origin[0] % p->item.item_origin[1] % p->item.item_origin[2] % p->item.item_origin[3] % p->item.item_origin[4] % p->item.item_origin[5]
								 % p->item.socketCount
								 % p->item.socket[0] % p->item.socket[1] % p->item.socket[2] % p->item.socket[3] % p->item.socket[4] % p->item.socket[5] % p->item.socket[6]
								 % p->item.now_durability % p->item.max_durability
								 % p->item.rareGrade);
#else
	std::string sql = boost::str(boost::format("INSERT DELAYED INTO t_tradeagent( a_item_serial, a_item_serial2, a_item_plus, a_item_plus2, a_item_flag,"
								 " a_item_level, a_quantity, a_totalmoney, a_guaranty,"
								 " a_regist_date, a_expire_date, a_sell_charindex, a_sell_charname, a_option_count, a_option_0_type,"
								 " a_option_0_level,a_option_1_type, a_option_1_level,a_option_2_type, a_option_2_level,"
								 " a_option_3_type, a_option_3_level,a_option_4_type, a_option_4_level, a_item_origin_0, a_item_origin_1, a_item_origin_2, a_item_origin_3, a_item_origin_4, a_item_origin_5,"
								 " a_socketCount, a_socket0, a_socket1, a_socket2, a_socket3, a_socket4, a_socket5, a_socket6, a_rare_grade )"
								 "VALUES(%1%, '%2%', %3%, %4%, %5%, %6%, %7%, %8%, %9%, NOW(), DATE_ADD(now(), INTERVAL 5 DAY), %10%, '%11%', %12%, %13%, %14%, %15%, %16%,"
								 " %17%, %18%, %19%, %20%, %21%, %22%, %23%,"
								 " %24%, %25%, %26%, %27%, %28%, %29%, %30%, %31%, %32%, %33%, %34%, %35%, %36%, %37%)")
								 % p->item.item_index % p->item.serial % (int)p->item.plus % (int)p->item.plus2 % p->item.flag
								 % p->item.item_level % p->quantity % p->TotalMoney % p->Guarantee
								 % p->charIndex % p->charName
								 % p->item.option_count
								 % p->item.option_type[0] % p->item.option_level[0]
								 % p->item.option_type[1] % p->item.option_level[1]
								 % p->item.option_type[2] % p->item.option_level[2]
								 % p->item.option_type[3] % p->item.option_level[3]
								 % p->item.option_type[4] % p->item.option_level[4]
								 % p->item.item_origin[0] % p->item.item_origin[1] % p->item.item_origin[2] % p->item.item_origin[3] % p->item.item_origin[4] % p->item.item_origin[5]
								 % p->item.socketCount
								 % p->item.socket[0] % p->item.socket[1] % p->item.socket[2] % p->item.socket[3] % p->item.socket[4] % p->item.socket[5] % p->item.socket[6]
								 % p->item.rareGrade);
#endif

	cmd.SetQuery(sql);
	if(cmd.Update() == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::TradeAgentSystemError *packet = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(rmsg->m_buf);
		makeTradeAgentError(rmsg, ResponseClient::TRADEAGENT_ERROR_ITEM_REG_FAIL);
		packet->charIndex = p->charIndex;
		rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemError));
		SEND_Q(rmsg, desc);
		int charindex = p->charIndex;
		int itemindex = p->itemIndex;
		LOG_INFO("TRADE_AGENT> REG FAIL.. charIndex : %d, itemIndex : %d", charindex, itemindex);
		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::TradeAgentSystemRegWithServer *packet = reinterpret_cast<ResponseClient::TradeAgentSystemRegWithServer *>(rmsg->m_buf);
	packet->type = MSG_TRADEAGENT;
	packet->subType = MSG_TRADEAGENT_REG;
	packet->charIndex = p->charIndex;
	packet->Guarantee = p->Guarantee;
	packet->itemIndex = p->itemIndex;
	packet->quantity = p->quantity;
	packet->itemCount = p->itemCount;
	packet->tab = p->tab;
	packet->invenIndex = p->invenIndex;
	strncpy(packet->serial, p->item.serial, sizeof(packet->serial));
	packet->TotalMoney = p->TotalMoney;

	rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemRegWithServer));
	SEND_Q(rmsg, desc);
}
void do_command_TradeAgent_RegCancel(CNetMsg::SP& msg, CDescriptor* desc)
{
	RequestClient::TradeAgentSystemRegCancel *p = reinterpret_cast<RequestClient::TradeAgentSystemRegCancel*>(msg->m_buf);

	//char sql[1024];
	//memset(sql, 0x00, sizeof(sql));
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	std::string sql = boost::str(boost::format(
									 "select * from t_tradeagent left join t_tradeagent_iteminfo on t_tradeagent.a_item_serial = t_tradeagent_iteminfo.a_index where a_sell_charindex = %d and t_tradeagent.a_index = %d")
								 % p->charIndex % p->index);
	cmd.SetQuery(sql.c_str());
	if(cmd.Open() == false || cmd.MoveFirst() == false)
	{
		CNetMsg::SP	rmsg(new CNetMsg);
		ResponseClient::TradeAgentSystemError *packet = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(rmsg->m_buf);
		makeTradeAgentError(rmsg, ResponseClient::TRADEAGENT_ERROR_ITEM_REG_CANCEL_FAIL);
		packet->charIndex = p->charIndex;
		rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemError));
		SEND_Q(rmsg, desc);
		int charindex = p->charIndex;
		int tradeAgentIndex = p->index;
		LOG_INFO("TRADE_AGENT> CANCEL FAIL.. charIndex : %d, tradeAgentIndex : %d", charindex, tradeAgentIndex);
		return;
	}

	TradeAgentItem item;
	getRTradeAgentItem(cmd, item);

	//거래대행 리스트에서 삭제하고
	sql = boost::str(boost::format(
						 "delete from t_tradeagent where a_index = %d and a_sell_charindex = %d")
					 % p->index % p->charIndex);

	cmd.SetQuery(sql.c_str());
	if(cmd.Update() == false)
	{
		CNetMsg::SP	rmsg(new CNetMsg);
		ResponseClient::TradeAgentSystemError *packet = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(rmsg->m_buf);
		makeTradeAgentError(rmsg, ResponseClient::TRADEAGENT_ERROR_ITEM_REG_CANCEL_FAIL);
		packet->charIndex = p->charIndex;
		rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemError));
		int charindex = p->charIndex;
		int tradeAgentIndex = p->index;
		LOG_INFO("TRADE_AGENT> CANCEL FAIL..NOT EXIST ITEM.. charIndex : %d, itemIndex : %d", charindex, tradeAgentIndex);
		SEND_Q(rmsg, desc);
		return;
	}

	//관심물품 리스트들에서 삭제를 하고
	sql = boost::str(boost::format(
						 "delete from t_tradeagent_like where a_tradeagent_index = %d")
					 % p->index);

	cmd.SetQuery(sql.c_str());
	cmd.Update();

	//성공
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::TradeAgentSystemRegCancelWithServer *packet = reinterpret_cast<ResponseClient::TradeAgentSystemRegCancelWithServer *>(rmsg->m_buf);
		packet->type = MSG_TRADEAGENT;
		packet->subType = MSG_TRADEAGENT_REG_CANCEL;
		packet->charIndex = p->charIndex;
		packet->itemIndex = item.item_index;
		packet->itemCount = item.quantity;
		strncpy(packet->serial, item.serial, sizeof(packet->serial));

		rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemRegCancelWithServer));
		SEND_Q(rmsg, desc);
	}

	//lce 에 아이템을 넣고 해당 클라이언트한테 메시지를 날려준다.
	insertItemToLCE(item, p->charIndex, EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_CANCEL);
}

void do_command_TradeAgent_Buy(CNetMsg::SP& msg, CDescriptor* desc)
{
	RequestClient::TradeAgentSystemBuy *p = reinterpret_cast<RequestClient::TradeAgentSystemBuy*>(msg->m_buf);

	std::list<int>::iterator it = gserver.m_tradeAgentBuyList.begin();
	std::list<int>::iterator it_end = gserver.m_tradeAgentBuyList.end();

	for(; it != it_end; it++)
	{
		if( *it == p->TradeAgentIndex )
		{
			// 다른 사람이 구매중입니다. 패킷 전송
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::TradeAgentSystemError *packet = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(rmsg->m_buf);
				makeTradeAgentError(rmsg, ResponseClient::TRADEAGENT_ERROR_ITEM_BUY_ING);
				packet->charIndex = p->charIndex;
				rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemError));
				SEND_Q(rmsg, desc);

			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				makeTradeAgentReturnMoney(rmsg, p->charIndex, p->needNas);
				SEND_Q(rmsg, desc);

				std::string tstr = boost::str(boost::format(
												  "TradeAgent Return Nas - SEND : charIndex : %1% : nas : %2%") % p->charIndex % p->needNas);
				LOG_INFO(tstr.c_str());
			}
			return;
		}
	}
	//구매중 리스트에 추가.
	gserver.m_tradeAgentBuyList.push_back(p->TradeAgentIndex);

	std::string sql = boost::str(boost::format("select * from t_tradeagent where a_index = %d") % p->TradeAgentIndex);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql.c_str());
	if(cmd.Open() == false || cmd.MoveFirst() == false)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::TradeAgentSystemError *packet = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(rmsg->m_buf);
			makeTradeAgentError(rmsg, ResponseClient::TRADEAGENT_ERROR_NOT_EXIST_ITEM);
			packet->charIndex = p->charIndex;
			rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemError));
			SEND_Q(rmsg, desc);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			makeTradeAgentReturnMoney(rmsg, p->charIndex, p->needNas);
			SEND_Q(rmsg, desc);

			std::string tstr = boost::str(boost::format(
											  "TradeAgent Return Nas - SEND : charIndex : %1% : nas : %2%") % p->charIndex % p->needNas);
			LOG_INFO(tstr.c_str());
		}

		gserver.m_tradeAgentBuyList.remove(p->TradeAgentIndex);
		int charindex = p->charIndex;
		int tradeAgentIndex = p->TradeAgentIndex;
		LOG_INFO("TRADE_AGENT> BUY FAIL.. charIndex : %d, itemIndex : %d", charindex, tradeAgentIndex);
		return;
	}
	// lce에 넣을 아이템 만들고
	TradeAgentItem item;
	getRTradeAgentItem(cmd, item);

	CLCString sell_charName(MAX_ID_NAME_LENGTH);
	cmd.GetRec( "a_sell_charname"	, sell_charName);
	strcpy(item.sell_charName, (const char*)sell_charName);
	int sell_charindex;
	cmd.GetRec("a_sell_charindex", sell_charindex);
	int guaranty;
	cmd.GetRec("a_guaranty", guaranty);
	GoldType_t totalPrice;
	cmd.GetRec("a_totalmoney", totalPrice);

	//거래대행 리스트에서 아이템 삭제하고
	sql = boost::str(boost::format(
						 "delete from t_tradeagent where a_index = %d LIMIT 1")
					 % p->TradeAgentIndex);

	cmd.SetQuery(sql.c_str());
	cmd.Update();

	//관심물품 리스트들에서 삭제를 하고
	sql = boost::str(boost::format(
						 "delete from t_tradeagent_like where a_tradeagent_index = %d")
					 % p->TradeAgentIndex);

	cmd.SetQuery(sql.c_str());
	cmd.Update();

	// LCE에 아이템 넣기
	insertItemToLCE(item, p->charIndex, EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_BUY, p->needNas);

	//2.판매자에게 돈 넣고, 판매 메시지 전송
	GoldType_t nas = (GoldType_t)(p->needNas - (p->needNas * gserver.m_tradeAgentFee / 100) + guaranty);

#ifdef PREMIUM_CHAR
	{
//XX - 프리미엄캐릭터 : 수수료 0
		int expireTime = 0;
		std::string query = boost::str(boost::format(
			"SELECT a_expireTime FROM t_premiumchar WHERE a_charIndex = %1% LIMIT 1") % sell_charindex);
		cmd.SetQuery(query);
		if (cmd.Open() && cmd.m_nrecords > 0)
		{
			cmd.MoveNext();
			cmd.GetRec("a_expireTime", expireTime);
			if ((int)time(0) <= expireTime)
			{
				nas = (GoldType_t)(p->needNas + guaranty);
			}
		}
	}
#endif

	insertNasToLCE(nas, sell_charindex, "TradeAgent", EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_SELL, item.item_index, item.quantity);

	sendMessageToUserSellOrReturned(TRADEAGENT_DB_MSG_TYPE_SELL, sell_charindex,
									item.item_index, item.quantity);

	//게임서버로 패킷 전송 한다.
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::TradeAgentSystemBuyWithServer *packet = reinterpret_cast<ResponseClient::TradeAgentSystemBuyWithServer*>(rmsg->m_buf);
		packet->type = MSG_TRADEAGENT;
		packet->subType = MSG_TRADEAGENT_BUY;
		packet->needNas = p->needNas;
		packet->charIndex = p->charIndex;
		packet->itemIndex = item.item_index;
		packet->itemCount = item.quantity;
		packet->itemPrice = totalPrice;
		strncpy(packet->itemSerial, item.serial, sizeof(packet->itemSerial));
		strncpy(packet->sellerName, (const char *)sell_charName, sizeof(packet->sellerName));
		rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemBuyWithServer));
		SEND_Q(rmsg, desc);
	}

	//넣었으면 구매중 리스트에서 제거한다.
	gserver.m_tradeAgentBuyList.remove(p->TradeAgentIndex);

	return;
}
void do_command_TradeAgent_LikeList(CNetMsg::SP& msg, CDescriptor* desc)
{
	RequestClient::TradeAgentSystemLikeList *p = reinterpret_cast<RequestClient::TradeAgentSystemLikeList*>(msg->m_buf);

	std::string sql = boost::str(boost::format(
									 "select a.*, b.*, UNIX_TIMESTAMP(a_regist_date) as regist_date, UNIX_TIMESTAMP(a_expire_date) as expire_date  from t_tradeagent as a LEFT JOIN t_tradeagent_iteminfo as b ON (a.a_item_serial = b.a_index) LEFT JOIN t_tradeagent_like as c ON (a.a_index = c.a_tradeagent_index) where c.a_char_index = %d")
								 % p->charIndex);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql.c_str());

	if(cmd.Open() == false )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::TradeAgentSystemError *packet = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(rmsg->m_buf);
		makeTradeAgentError(rmsg, ResponseClient::TRADEAGENT_ERROR_ITEM_LIKE_LIST_FAIL);
		packet->charIndex = p->charIndex;
		rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemError));
		SEND_Q(rmsg, desc);
		return;
	}
	cmd.MoveFirst();

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::TradeAgentSystemLikeList *packet = reinterpret_cast<ResponseClient::TradeAgentSystemLikeList *>(rmsg->m_buf);
	packet->type = MSG_TRADEAGENT;
	packet->subType = MSG_TRADEAGENT_LIKE_LIST;

	packet->count = cmd.m_nrecords;
	packet->charIndex = p->charIndex;
	packet->curServerTime = (int)time(NULL);

	for(int i=0; i<packet->count; i++)
	{
		cmd.GetRec("a_index", packet->list[i].index);
		cmd.GetRec( "a_item_serial", packet->list[i].item_index);
		cmd.GetRec( "a_item_plus", packet->list[i].plus);
		cmd.GetRec( "a_item_plus2", packet->list[i].plus2);
		cmd.GetRec( "a_item_flag", packet->list[i].flag);

		cmd.GetRec( "a_option_count", packet->list[i].option_count);
		cmd.GetRec( "a_option_0_type", packet->list[i].option_type[0]);
		cmd.GetRec( "a_option_0_level", packet->list[i].option_level[0]);
		cmd.GetRec( "a_option_1_type", packet->list[i].option_type[1]);
		cmd.GetRec( "a_option_1_level", packet->list[i].option_level[1]);
		cmd.GetRec( "a_option_2_type", packet->list[i].option_type[2]);
		cmd.GetRec( "a_option_2_level", packet->list[i].option_level[2]);
		cmd.GetRec( "a_option_3_type", packet->list[i].option_type[3]);
		cmd.GetRec( "a_option_3_level", packet->list[i].option_level[3]);
		cmd.GetRec( "a_option_4_type", packet->list[i].option_type[4]);
		cmd.GetRec( "a_option_4_level", packet->list[i].option_level[4]);
		cmd.GetRec( "a_quantity", packet->list[i].quantity);
		cmd.GetRec( "a_totalmoney", packet->list[i].nas);
		cmd.GetRec( "a_item_level", packet->list[i].item_level);
		cmd.GetRec("a_item_origin_0", packet->list[i].item_origin[0]);
		cmd.GetRec("a_item_origin_1", packet->list[i].item_origin[1]);
		cmd.GetRec("a_item_origin_2", packet->list[i].item_origin[2]);
		cmd.GetRec("a_item_origin_3", packet->list[i].item_origin[3]);
		cmd.GetRec("a_item_origin_4", packet->list[i].item_origin[4]);
		cmd.GetRec("a_item_origin_5", packet->list[i].item_origin[5]);
		cmd.GetRec( "a_socketCount", packet->list[i].socketCount);
		cmd.GetRec( "a_socket0", packet->list[i].socket[0]);
		cmd.GetRec( "a_socket1", packet->list[i].socket[1]);
		cmd.GetRec( "a_socket2", packet->list[i].socket[2]);
		cmd.GetRec( "a_socket3", packet->list[i].socket[3]);
		cmd.GetRec( "a_socket4", packet->list[i].socket[4]);
		cmd.GetRec( "a_socket5", packet->list[i].socket[5]);
		cmd.GetRec( "a_socket6", packet->list[i].socket[6]);
		cmd.GetRec( "a_num0", packet->list[i].item_num0);
		cmd.GetRec( "a_num1", packet->list[i].item_num1);
		cmd.GetRec( "regist_date", packet->list[i].regist_date);
		cmd.GetRec( "expire_date", packet->list[i].expire_date);

#ifdef DURABILITY
		cmd.GetRec( "a_now_dur", packet->list[i].now_durability);
		cmd.GetRec( "a_max_dur", packet->list[i].max_durability);
#endif

		cmd.MoveNext();
	}
	rmsg->setSize( sizeof(ResponseClient::TradeAgentSystemLikeList) + ( sizeof(TradeAgentItem) * packet->count ));
	SEND_Q(rmsg, desc);
}
void do_command_TradeAgent_LikeReg(CNetMsg::SP& msg, CDescriptor* desc)
{
	RequestClient::TradeAgentSystemLikeReg *p = reinterpret_cast<RequestClient::TradeAgentSystemLikeReg*>(msg->m_buf);

	std::string sql = boost::str(boost::format(
									 "select * from t_tradeagent_like where a_char_index = %d") % p->charIndex);

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql.c_str());
	if( cmd.Open() == false )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::TradeAgentSystemError *packet = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(rmsg->m_buf);
		makeTradeAgentError(rmsg, ResponseClient::TRADEAGENT_ERROR_ITEM_LIKE_REG_FAIL);
		packet->charIndex = p->charIndex;
		rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemError));
		SEND_Q(rmsg, desc);
		return;
	}
	cmd.MoveFirst();

	if(cmd.m_nrecords >= 10)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::TradeAgentSystemError *packet = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(rmsg->m_buf);
		makeTradeAgentError(rmsg, ResponseClient::TRADEAGENT_ERROR_ITEM_LIKE_REG_FAIL);
		packet->charIndex = p->charIndex;
		rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemError));
		SEND_Q(rmsg, desc);
		return;
	}

	sql = boost::str(boost::format(
						 "insert into t_tradeagent_like values (%d, %d)") %p->charIndex %p->index);
	cmd.SetQuery(sql.c_str());
	if(cmd.Update() == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::TradeAgentSystemError *packet = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(rmsg->m_buf);
		makeTradeAgentError(rmsg, ResponseClient::TRADEAGENT_ERROR_ITEM_LIKE_REG_FAIL);
		packet->charIndex = p->charIndex;
		rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemError));
		SEND_Q(rmsg, desc);
		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::TradeAgentSystemLikeReg *packet = reinterpret_cast<ResponseClient::TradeAgentSystemLikeReg *>(rmsg->m_buf);
	packet->type = MSG_TRADEAGENT;
	packet->subType = MSG_TRADEAGENT_LIKE_REG;
	packet->charIndex = p->charIndex;

	rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemLikeReg));
	SEND_Q(rmsg, desc);
}
void do_command_TradeAgent_LikeCancel(CNetMsg::SP& msg, CDescriptor* desc)
{
	RequestClient::TradeAgentSystemLikeCancel *p = reinterpret_cast<RequestClient::TradeAgentSystemLikeCancel*>(msg->m_buf);

	std::string sql = boost::str(boost::format(
									 "delete from t_tradeagent_like where a_char_index = %d and a_tradeagent_index = %d") % p->charIndex % p->index );

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql.c_str());

	if(cmd.Update() == false)
	{
		CNetMsg::SP	rmsg(new CNetMsg);
		ResponseClient::TradeAgentSystemError *packet = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(rmsg->m_buf);
		makeTradeAgentError(rmsg, ResponseClient::TRADEAGENT_ERROR_ITEM_LIKE_CANCEL_FAIL);
		packet->charIndex = p->charIndex;
		rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemError));
		SEND_Q(rmsg, desc);
		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::TradeAgentSystemLikeCancel *packet = reinterpret_cast<ResponseClient::TradeAgentSystemLikeCancel *>(rmsg->m_buf);
	packet->type = MSG_TRADEAGENT;
	packet->subType = MSG_TRADEAGENT_LIKE_CANCEL;

	packet->charIndex = p->charIndex;
	rmsg->setSize(sizeof(ResponseClient::TradeAgentSystemLikeCancel));
	SEND_Q(rmsg, desc);
	return;
}

void getRTradeAgentItem(CDBCmd& cmd,TradeAgentItem& item)
{
	cmd.GetRec( "a_item_serial"		, item.item_index);
	CLCString serial(MAX_SERIAL_LENGTH + 1);
	cmd.GetRec( "a_item_serial2"	, serial);
	strcpy(item.serial, (const char*)serial);
	serial.CopyFrom(item.serial, serial.BufferSize());
	cmd.GetRec( "a_quantity"		, item.quantity);
	cmd.GetRec( "a_item_plus"		, item.plus);
	cmd.GetRec( "a_item_plus2"		, item.plus2);
	cmd.GetRec( "a_item_flag"		, item.flag);
	cmd.GetRec( "a_option_count"	, item.option_count);
	cmd.GetRec( "a_option_0_type"	, item.option_type[0]);
	cmd.GetRec( "a_option_0_level"	, item.option_level[0]);
	cmd.GetRec( "a_option_1_type"	, item.option_type[1]);
	cmd.GetRec( "a_option_1_level"	, item.option_level[1]);
	cmd.GetRec( "a_option_2_type"	, item.option_type[2]);
	cmd.GetRec( "a_option_2_level"	, item.option_level[2]);
	cmd.GetRec( "a_option_3_type"	, item.option_type[3]);
	cmd.GetRec( "a_option_3_level"	, item.option_level[3]);
	cmd.GetRec( "a_option_4_type"	, item.option_type[4]);
	cmd.GetRec( "a_option_4_level"	, item.option_level[4]);

	cmd.GetRec( "a_socketCount"		, item.socketCount);
	cmd.GetRec( "a_socket0"			, item.socket[0]);
	cmd.GetRec( "a_socket1"			, item.socket[1]);
	cmd.GetRec( "a_socket2"			, item.socket[2]);
	cmd.GetRec( "a_socket3"			, item.socket[3]);
	cmd.GetRec( "a_socket4"			, item.socket[4]);
	cmd.GetRec( "a_socket5"			, item.socket[5]);
	cmd.GetRec( "a_socket6"			, item.socket[6]);
	cmd.GetRec( "a_item_origin_0"	, item.item_origin[0]);
	cmd.GetRec( "a_item_origin_1"	, item.item_origin[1]);
	cmd.GetRec( "a_item_origin_2"	, item.item_origin[2]);
	cmd.GetRec( "a_item_origin_3"	, item.item_origin[3]);
	cmd.GetRec( "a_item_origin_4"	, item.item_origin[4]);
	cmd.GetRec( "a_item_origin_5"	, item.item_origin[5]);

#ifdef DURABILITY
	cmd.GetRec( "a_now_dur"			, item.now_durability);
	cmd.GetRec( "a_max_dur"			, item.max_durability);
#endif
}

void insertItemToLCE(TradeAgentItem& item, int charIndex, int sendType, LONGLONG tradeagent_nas)
{
	ExpressSystemItemInfo eItem;
	eItem.index = 0;
	eItem.item_index = item.item_index;
	eItem.plus = item.plus;
	eItem.plus2 = item.plus2;
	eItem.flag = item.flag;
	strncpy(eItem.serial, item.serial, sizeof(eItem.serial));
	eItem.item_count = item.quantity;
	eItem.option_count = item.option_count;
	memcpy(eItem.option_type, item.option_type, sizeof(eItem.option_type));
	memcpy(eItem.option_level, item.option_level, sizeof(eItem.option_level));
	memcpy(eItem.socket, item.socket, sizeof(eItem.socket));
	memcpy(eItem.item_origin, item.item_origin, sizeof(eItem.item_origin));
	eItem.nas = 0;
	eItem.send_type = sendType;
	strcpy(eItem.sender, item.sell_charName);
#ifdef DURABILITY
	eItem.now_durability = item.now_durability;
	eItem.max_durability = item.max_durability;
#endif
	eItem.tradeagent_nas = tradeagent_nas;
	eItem.tradeagent_itemIndex = -1;
	eItem.tradeagent_itemCount = -1;
	MessengerInSubHelper::instance()->process_InputItemToExpress(charIndex, eItem);
}

void insertNasToLCE(GoldType_t nas, int charIndex, const char* sender, int sendType, int tradeagent_itemIndex, int tradeagent_itemCount)
{
	ExpressSystemItemInfo eItem;
	eItem.index = 0;
	eItem.item_index = -1;
	eItem.nas = nas;
	eItem.send_type = sendType;
	eItem.tradeagent_nas = -1;
	eItem.tradeagent_itemIndex = tradeagent_itemIndex;
	eItem.tradeagent_itemCount = tradeagent_itemCount;
	strncpy(eItem.sender, sender, sizeof(eItem.sender));
	MessengerInSubHelper::instance()->process_InputItemToExpress(charIndex, eItem);
}

// 매 분마다 판매대행중 반품이 있는지 검사함, 한번에 검사하는 갯수는 한정함
void checkExpireItemInTradeAgent()
{
	std::string qry;
	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);
	dbChar.SetQuery("SELECT * FROM t_tradeagent WHERE a_expire_date <= now() LIMIT 30");
	if (dbChar.Open() == false)
		return;

	if (dbChar.m_nrecords == 0)
		return;

	int sell_charindex;
	int tradeagentIndex;
	int guaranty;
	TradeAgentItem item;
	CDBCmd tdb;
	tdb.Init(&gserver.m_dbchar);

	while (dbChar.MoveNext())
	{
		dbChar.GetRec("a_sell_charindex", sell_charindex);
		dbChar.GetRec("a_index", tradeagentIndex);
		dbChar.GetRec("a_guaranty", guaranty);

		//거래대행 리스트에서 아이템 삭제하고
		qry = boost::str(boost::format(
							 "delete from t_tradeagent where a_index = %d LIMIT 1")
						 % tradeagentIndex);
		tdb.SetQuery(qry.c_str());
		if(tdb.Update() == false)
			continue;

		//관심물품 리스트들에서 삭제를 하고
		qry = boost::str(boost::format(
							 "delete from t_tradeagent_like where a_tradeagent_index = %d")
						 % tradeagentIndex);

		tdb.SetQuery(qry.c_str());
		tdb.Update();

		// lce에 넣을 아이템 만들고
		getRTradeAgentItem(dbChar, item);

		// LCE에 아이템 / 돈 넣기
		insertItemToLCE(item, sell_charindex, EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_RETURN);
		insertNasToLCE(guaranty, sell_charindex, dbChar.GetRec("a_sell_charname"), EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_RETURN);

		sendMessageToUserSellOrReturned(TRADEAGENT_DB_MSG_TYPE_RETURNED, sell_charindex,
										item.item_index, item.quantity);
	}
}

void sendMessageToUserSellOrReturned(int type, int charIndex, int itemIndex, int itemCount)
{
	if (LogInOutManager::Instance()->isExistByCharIndex(charIndex))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		makeTradeAgentAfterSell(rmsg, charIndex, itemIndex, itemCount);
		rnSocketIOService* service = LogInOutManager::Instance()->getSocketIOByCharIndex(charIndex);
		service->deliver(rmsg);
	}
	else
	{
		// 현재 접속중이 아니라면 DB에 데이터 저장
		std::string qry = boost::str(boost::format(
										 "INSERT INTO t_tradeagent_after_sell_returned VALUES(%1%,%2%,%3%,%4%,now())")
									 % charIndex % type % itemIndex % itemCount);
		CDBCmd dbChar;
		dbChar.Init(&gserver.m_dbchar);
		dbChar.SetQuery(qry.c_str());
		dbChar.Update();
	}
}

void do_command_TradeAgent_ChangeFee(CNetMsg::SP& msg, CDescriptor* desc)
{
	ResponseClient::TradeAgentChangeFee* packet = reinterpret_cast<ResponseClient::TradeAgentChangeFee*>(msg->m_buf);
	gserver.m_tradeAgentFee = packet->fee;

	gserver.SendToAllGameServer(msg);
}

void do_command_TradeAgent_ChangeDeposit(CNetMsg::SP& msg, CDescriptor* desc)
{
	ResponseClient::TradeAgentChangeDeposit* packet = reinterpret_cast<ResponseClient::TradeAgentChangeDeposit*>(msg->m_buf);
	gserver.m_tradeAgentDeposit = packet->deposit;

	gserver.SendToAllGameServer(msg);
}

