#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Server.h"
#include "MessengerInSubHelper.h"
#include "ReservedGmCommandManager.h"
#include "../ShareLib/packetType/ptype_express_system.h"
#include "../ShareLib/packetType/ptype_reserved_gm_command.h"
#include "../ShareLib/LogInOutManager.h"

#define RECONNECT_TIMER_FOR_MESSENGER		(10 * 1000)

MessengerInSubHelperConnectTimer* MessengerInSubHelperConnectTimer::instance()
{
	static MessengerInSubHelperConnectTimer __instance;
	return &__instance;
}

void MessengerInSubHelperConnectTimer::operate( rnSocketIOService* service )
{
	// 현 타이머 삭제
	bnf::instance()->RemoveSession(service);

	// 메신져 서버에 접속
	MessengerInSubHelper::instance()->connect();
}

//////////////////////////////////////////////////////////////////////////
void MessengerInSubHelper::connect()
{
	// 메시젼 서버에 접속 시도
	bnf::instance()->CreateAsyncConnect(
		host_, port_, this, CONNECT_TYPE_FOR_CNETMSG);
}

void MessengerInSubHelper::connect(std::string host, std::string port)
{
	host_ = host;
	port_ = atoi(port.c_str());

	this->connect();
}

void MessengerInSubHelper::WriteToOutput( CNetMsg::SP& msg )
{
	if (msg->m_mtype == MSG_UNKNOWN)
		return;

	if (service_ == NULL)
		return;

	service_->deliver(msg);
}

void MessengerInSubHelper::operate( rnSocketIOService* service )
{
	CNetMsg::SP msg(service->GetMessage());

	switch (msg->m_mtype)
	{
	case MSG_EXPRESS_SYSTEM:
		process_ExpressSystem(msg);
		break;

	case MSG_RESERVED_GM_COMMAND:
		ReservedGmCommandManager::Instance()->process(msg);
		break;

	default:
		LOG_FATAL("Invalid packet type[%d]", msg->m_mtype);
		break;
	}
}

void MessengerInSubHelper::onClose( rnSocketIOService* service )
{
	bnf::instance()->RemoveSession(service);
	service_ = NULL;

	// 재접속 타이머 생성
	session_handle handle = bnf::instance()->CreateMSecTimer(RECONNECT_TIMER_FOR_MESSENGER,
							(void *)MessengerInSubHelperConnectTimer::instance());

	LOG_FATAL("%%%%%%%%%% Disconnected from Messenger Server (%s : %d) %%%%%%%%%%\n\n",
			  host_.c_str(), port_);
}

void MessengerInSubHelper::onConnect( rnSocketIOService* service )
{
	service_ = service;

	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_SUBHELPER_CONNECT);
	RefMsg(rmsg) << gserver.m_serverno;
	service_->deliver(rmsg);

	LOG_INFO("%%%%%%%%%% connected to Messenger server (%s : %d) %%%%%%%%%%\n\n",
			 host_.c_str(), port_);
}

void MessengerInSubHelper::onConnectFail( rnSocketIOService* service )
{
	LOG_FATAL("%%%%%%%%%% Can't connect to Messenger server (%s : %d) %%%%%%%%%%\n\n",
			  host_.c_str(), port_);
}

MessengerInSubHelper* MessengerInSubHelper::instance()
{
	static MessengerInSubHelper __instance;
	return &__instance;
}

void MessengerInSubHelper::process_ExpressSystem( CNetMsg::SP& msg )
{
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);
	switch (pBase->subType)
	{
	case MSG_SUB_EXPRESS_ONE_USER:
		{
			RequestGMTool::sendItemToOneUser* packet = reinterpret_cast<RequestGMTool::sendItemToOneUser*>(msg->m_buf);

			CDBCmd cmd;
			cmd.Init(&gserver.m_dbchar);

			// 유효한 유저인지 검사
			std::string qry = boost::str(boost::format(
											 "SELECT a_index FROM t_characters WHERE a_index=%1% LIMIT 1") % packet->char_index);
			cmd.SetQuery(qry.c_str());
			if(cmd.Open() == false || cmd.MoveFirst() == false)
			{
				LOG_FATAL("Not found cahr_index[%d] / query[%s]", packet->char_index, qry.c_str());
				return;
			}

			ExpressSystemItemInfo tItem;
			tItem.item_index = packet->item_index;
			tItem.item_count = packet->item_count;
			tItem.nas = packet->nas;
			tItem.send_type = EXPRESS_SEND_TYPE_GM;
			strcpy(tItem.sender, packet->sender);
			this->process_InputItemToExpress(packet->char_index, tItem);
		}
		break;

	default:
		LOG_FATAL("Invalid packet subtye[%d]", pBase->subType);
		break;
	}
}

void MessengerInSubHelper::process_InputItemToExpress( int charIndex, ExpressSystemItemInfo& item )
{
	if (item.nas > 0)
		item.item_index = -1;

	// t_express_system table에 데이터 기록
#ifdef DURABILITY
	std::string sql_q =
		boost::str(boost::format(
					   "INSERT INTO t_express_system(a_char_index, a_item_index, a_plus, a_plus2, a_flag, a_serial, a_item_count, a_option_count, "
					   "a_option_0_type, a_option_0_level, "
					   "a_option_1_type, a_option_1_level, "
					   "a_option_2_type, a_option_2_level, "
					   "a_option_3_type, a_option_3_level, "
					   "a_option_4_type, a_option_4_level, "
					   "a_socket0, a_socket1, a_socket2, a_socket3, a_socket4, a_socket5, a_socket6, "
					   "a_item_origin_var0, a_item_origin_var1, a_item_origin_var2, a_item_origin_var3, a_item_origin_var4, a_item_origin_var5, "
					   "a_nas, a_send_type, a_sender, a_registe_date, a_expire_date, a_now_dur, a_max_dur, a_tradeagent_nas, a_tradeagent_itemIndex, a_tradeagent_itemCount)"
					   "values ( %1%, %2%, %3%, %4%, %5%, '%6%', %7%, %8%, "
					   "%9%, %10%, "
					   "%11%, %12%, "
					   "%13%, %14%, "
					   "%15%, %16%, "
					   "%17%, %18%, "
					   "%19%, %20%, %21%, %22%, %23%, %24%, %25%, "
					   "%26%, %27%, %28%, %29%, %30%, %31%, "
					   "%32%, %33%, '%34%', now(), DATE_ADD(now(), INTERVAL %35% DAY), %36%, %37%, %38%, %39%, %40%)")
				   % charIndex % item.item_index % (int)item.plus %(int)item.plus2 % item.flag % item.serial % item.item_count % item.option_count
				   % item.option_type[0] % item.option_level[0]
				   % item.option_type[1] % item.option_level[1]
				   % item.option_type[2] % item.option_level[2]
				   % item.option_type[3] % item.option_level[3]
				   % item.option_type[4] % item.option_level[4]
				   % item.socket[0] % item.socket[1] % item.socket[2] % item.socket[3] % item.socket[4] % item.socket[5] % item.socket[6]
				   % item.item_origin[0] % item.item_origin[1] % item.item_origin[2] % item.item_origin[3] % item.item_origin[4] % item.item_origin[5]
				   % item.nas % item.send_type % item.sender % EXPRESS_SYSTEM_EXPIRE_DATE % item.now_durability % item.max_durability % item.tradeagent_nas % item.tradeagent_itemIndex % item.tradeagent_itemCount);
#else
	std::string sql_q =
		boost::str(boost::format(
					   "INSERT INTO t_express_system(a_char_index, a_item_index, a_plus, a_plus2, a_flag, a_serial, a_item_count, a_option_count, "
					   "a_option_0_type, a_option_0_level, "
					   "a_option_1_type, a_option_1_level, "
					   "a_option_2_type, a_option_2_level, "
					   "a_option_3_type, a_option_3_level, "
					   "a_option_4_type, a_option_4_level, "
					   "a_socket0, a_socket1, a_socket2, a_socket3, a_socket4, a_socket5, a_socket6, "
					   "a_item_origin_var0, a_item_origin_var1, a_item_origin_var2, a_item_origin_var3, a_item_origin_var4, a_item_origin_var5, "
					   "a_nas, a_send_type, a_sender, a_registe_date, a_expire_date, a_tradeagent_nas, a_tradeagent_itemIndex, a_tradeagent_itemCount)"
					   "values ( %1%, %2%, %3%, %4%, %5%, '%6%', %7%, %8%, "
					   "%9%, %10%, "
					   "%11%, %12%, "
					   "%13%, %14%, "
					   "%15%, %16%, "
					   "%17%, %18%, "
					   "%19%, %20%, %21%, %22%, %23%, %24%, %25%, "
					   "%26%, %27%, %28%, %29%, %30%, %31%, "
					   "%32%, %33%, '%34%', now(), DATE_ADD(now(), INTERVAL %35% DAY), %36%, %37%, %38%)"	)
				   % charIndex % item.item_index % (int)item.plus % item.flag % item.serial % item.item_count % item.option_count
				   % item.option_type[0] % item.option_level[0]
				   % item.option_type[1] % item.option_level[1]
				   % item.option_type[2] % item.option_level[2]
				   % item.option_type[3] % item.option_level[3]
				   % item.option_type[4] % item.option_level[4]
				   % item.socket[0] % item.socket[1] % item.socket[2] % item.socket[3] % item.socket[4] % item.socket[5] % item.socket[6]
				   % item.item_origin[0] % item.item_origin[1] % item.item_origin[2] % item.item_origin[3] % item.item_origin[4] % item.item_origin[5]
				   % item.nas % item.send_type % item.sender % EXPRESS_SYSTEM_EXPIRE_DATE % item.tradeagent_nas % item.tradeagent_itemIndex % item.tradeagent_itemCount);
#endif

	LOG_INFO("TRADEAGENT BUY. Query[%s]", sql_q.c_str());

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql_q.c_str());
	if (cmd.Update() == false)
	{
		LOG_FATAL("query error : query[%s]", sql_q.c_str());
		return;
	}

	// 접속한 유저에서 charIndex를 검색하여 이 패킷을 전달하여 실시간으로 창고NPC에 내용이 있음을 알린다.
	rnSocketIOService* pService = LogInOutManager::Instance()->getSocketIOByCharIndex(charIndex);
	if (pService)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		RequestGMTool::sendItemToOneUser* packet = reinterpret_cast<RequestGMTool::sendItemToOneUser*>(rmsg->m_buf);
		packet->type = MSG_EXPRESS_SYSTEM;
		packet->subType = MSG_SUB_EXPRESS_ONE_USER;
		packet->char_index = charIndex;
		packet->send_type = item.send_type;
		rmsg->setSize(sizeof(RequestGMTool::sendItemToOneUser));

		pService->deliver(rmsg);
	}
}
