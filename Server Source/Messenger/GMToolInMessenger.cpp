#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Server.h"
#include "SubHelperInMessenger.h"
#include "GMToolInMessenger.h"
#include "../ShareLib/packetType/ptype_express_system.h"
#include "../ShareLib/packetType/ptype_reserved_gm_command.h"
#include "../ShareLib/gm_command.h"

#define WAIT_TIMEOUT_FOR_GMTOOL			(6 * 10) // sec

bool GMToolInMessenger::Init()
{
	std::string bind_host = gserver.m_config.Find("Server", "IP");
	int bind_port = atoi(gserver.m_config.Find("Server", "GMPort"));
	session_handle handle = bnf::instance()->CreateListen(bind_host, bind_port, WAIT_TIMEOUT_FOR_GMTOOL, this);

	return (handle != SessionBase::INVALID_SESSION_HANDLE) ? true : false;
}

void GMToolInMessenger::operate( rnSocketIOService* service )
{
	if (this->isLogined())
	{
		LOG_ERROR("\n****************************\n"
				  "GM TOOL is already connected.\n"
				  "****************************\n");

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeAuthorize(rmsg, ResponseGMTool::ERR_ALREADY_CONNECT);
		service->deliver(rmsg);

		service->Close("GM TOOL is already connected");

		return;
	}

	GMToolProcessInMessenger* pGMTOOL = new GMToolProcessInMessenger(service);
	service->SetUserData(pGMTOOL);

	this->setFlag();

	LOG_INFO("\n****************************\n"
			 "GM TOOL is connected. / ip[%s]\n"
			 "****************************\n", service->ip().c_str());
}

GMToolInMessenger* GMToolInMessenger::instance()
{
	static GMToolInMessenger __instance;
	return &__instance;
}

//////////////////////////////////////////////////////////////////////////

void GMToolProcessInMessenger::onClose( rnSocketIOService* service )
{
	service_ = NULL;

	bnf::instance()->RemoveSession(service);
	GMToolInMessenger::instance()->resetFlag();

	delete this;

	LOG_INFO("\n****************************\n"
			 "GM TOOL is disconnected. / ip[%s]\n"
			 "****************************\n", service->ip().c_str());
}

void GMToolProcessInMessenger::operate( rnSocketIOService* service )
{
	CNetMsg::SP msg(service->GetMessage());

	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);

	switch (pBase->type)
	{
	case MSG_PULSE:
		{
			// alive packet
		}
		break;

	case MSG_EXPRESS_SYSTEM:
		{
			switch (pBase->subType)
			{
			case MSG_SUB_AUTHORIZE:
				break;

			case MSG_SUB_EXPRESS_ALL_USER:
				this->process_alluser(msg);
				break;
			case MSG_SUB_EXPRESS_SERVER_GROUP:
				this->process_server_group(msg);
				break;
			case MSG_SUB_EXPRESS_ZONE:
				this->process_zone(msg);
				break;
			case MSG_SUB_EXPRESS_ONE_USER:
				this->process_oneuser(msg);
				break;

			default:
				{
					std::string str = boost::str(boost::format("invalid subtype[%d].") % pBase->subType);
					service->Close(str);
				}
				break;
			}
		}
		break;

	case MSG_RESERVED_GM_COMMAND:
		{
			switch (pBase->subType)
			{
			case MSG_SUB_ADD_RESERVED_GM_COMMAND:
				this->process_reserved_gm_command_add(msg);
				break;

			case MSG_SUB_DELETE_RESERVED_GM_COMMAND:
				this->process_reserved_gm_command_delete(msg);
				break;

			default:
				{
					std::string str = boost::str(boost::format("invalid subtype[%d].") % pBase->subType);
					service->Close(str);
				}
				break;
			}
		}
		break;

	default:
		{
			std::string str = boost::str(boost::format("invalid packet type[%d].") % msg->m_mtype);
			service->Close(str);
		}
		break;
	}
}

// 모든 게임 서버에게 전달
void GMToolProcessInMessenger::process_alluser( CNetMsg::SP& msg )
{
	RequestGMTool::sendItemToPlayingUser* packet = reinterpret_cast<RequestGMTool::sendItemToPlayingUser*>(msg->m_buf);
	packet->sender[EXPRESS_SENDER_NAME] = '\0';

	LOG_INFO("All user : server_no[%d] zone_no[%d] item_index[%d] item_count[%d] nas[%d] sender[%s]",
			 packet->server_no, packet->zone_no, packet->item_index, packet->item_count, packet->nas, packet->sender);

	if (packet->server_no != -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_SERVER_NO);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->zone_no != -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_ZONE_NO);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->item_index == 0 || packet->item_index < -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_ITEM_INDEX);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->item_count < 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_ITEM_COUNT);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->nas < 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_NAS);
		this->service_->deliver(rmsg);
		return;
	}

	if (strlen(packet->sender) == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_SENDER_IS_EMPTY);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->minLevel < 1 || packet->maxLevel > 185)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_LEVEL);
		this->service_->deliver(rmsg);
		return;
	}

	CDescriptor* pDesc;
	CDescriptor* pNext = gserver.m_desclist;
	while ((pDesc = pNext))
	{
		pNext = pDesc->m_pNext;

		if (STATE(pDesc) != CON_PLAYING)
			continue ;

		SEND_Q(msg, pDesc);
	}

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_NO_ERROR);
	this->service_->deliver(rmsg);

	LOG_INFO("Express System (from GMTOOL to all server) : server_no[%d] / zone_no[%d] / item_index[%d] / item_count[%d] / nas[%d] / sender[%s]",
			 packet->server_no, packet->zone_no, packet->item_index, packet->item_count, packet->nas, packet->sender);
}

// 특정 서버군에게 전달
void GMToolProcessInMessenger::process_server_group( CNetMsg::SP& msg )
{
	RequestGMTool::sendItemToPlayingUser* packet = reinterpret_cast<RequestGMTool::sendItemToPlayingUser*>(msg->m_buf);
	packet->sender[EXPRESS_SENDER_NAME] = '\0';

	LOG_INFO("All user : server_no[%d] zone_no[%d] item_index[%d] item_count[%d] nas[%d] sender[%s]",
			 packet->server_no, packet->zone_no, packet->item_index, packet->item_count, packet->nas, packet->sender);

	if (packet->server_no <= 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_SERVER_NO);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->zone_no != -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_ZONE_NO);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->item_index == 0 || packet->item_index < -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_ITEM_INDEX);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->item_count < 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_ITEM_COUNT);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->nas < 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_NAS);
		this->service_->deliver(rmsg);
		return;
	}

	if (strlen(packet->sender) == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_SENDER_IS_EMPTY);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->minLevel < 1 || packet->maxLevel > 185)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_LEVEL);
		this->service_->deliver(rmsg);
		return;
	}

	CDescriptor* pDesc;
	CDescriptor* pNext = gserver.m_desclist;
	while ((pDesc = pNext))
	{
		pNext = pDesc->m_pNext;

		if (STATE(pDesc) != CON_PLAYING)
			continue ;

		if (pDesc->m_serverNo != packet->server_no)
			continue;

		SEND_Q(msg, pDesc);
	}

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseGMTool::makeSendToServerGroup(rmsg, ResponseGMTool::ERR_NO_ERROR);
	this->service_->deliver(rmsg);

	LOG_INFO("Express System (from GMTOOL to server) : server_no[%d] / zone_no[%d] / item_index[%d] / item_count[%d] / nas[%d] / sender[%s]",
			 packet->server_no, packet->zone_no, packet->item_index, packet->item_count, packet->nas, packet->sender);
}

// 특정 서버, 특정 존에게 자료를 전달
void GMToolProcessInMessenger::process_zone( CNetMsg::SP& msg )
{
	RequestGMTool::sendItemToPlayingUser* packet = reinterpret_cast<RequestGMTool::sendItemToPlayingUser*>(msg->m_buf);
	packet->sender[EXPRESS_SENDER_NAME] = '\0';

	LOG_INFO("All user : server_no[%d] zone_no[%d] item_index[%d] item_count[%d] nas[%d] sender[%s]",
			 packet->server_no, packet->zone_no, packet->item_index, packet->item_count, packet->nas, packet->sender);

	if (packet->zone_no < 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_ZONE_NO);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->item_index == 0 || packet->item_index < -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_ITEM_INDEX);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->item_count < 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_ITEM_COUNT);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->nas < 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_NAS);
		this->service_->deliver(rmsg);
		return;
	}

	if (strlen(packet->sender) == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_SENDER_IS_EMPTY);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->minLevel < 1 || packet->maxLevel > 185)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_LEVEL);
		this->service_->deliver(rmsg);
		return;
	}

	CDescriptor* pDesc;
	CDescriptor* pNext = gserver.m_desclist;
	while ((pDesc = pNext))
	{
		pNext = pDesc->m_pNext;

		if (STATE(pDesc) != CON_PLAYING)
			continue ;

		if ((packet->server_no != -1) && (pDesc->m_serverNo != packet->server_no))
			continue;

		for (int i = 0; i < pDesc->m_countZone; ++i)
		{
			if (packet->zone_no == pDesc->m_zones[i])
			{
				SEND_Q(msg, pDesc);
			}
		}
	}

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseGMTool::makeSendToZone(rmsg, ResponseGMTool::ERR_NO_ERROR);
	this->service_->deliver(rmsg);

	LOG_INFO("Express System (from GMTOOL to zone) : server_no[%d] / zone_no[%d] / item_index[%d] / item_count[%d] / nas[%d] / sender[%s]",
			 packet->server_no, packet->zone_no, packet->item_index, packet->item_count, packet->nas, packet->sender);
}

void GMToolProcessInMessenger::process_oneuser( CNetMsg::SP& msg )
{
	RequestGMTool::sendItemToOneUser* packet = reinterpret_cast<RequestGMTool::sendItemToOneUser*>(msg->m_buf);
	packet->sender[EXPRESS_SENDER_NAME] = '\0';

	LOG_INFO("One User : server_no[%d] char_index[%d] item_index[%d] item_count[%d] nas[%d] send_type[%d] sender[%s]",
			 packet->server_no, packet->char_index, packet->item_index, packet->item_count, packet->nas, packet->send_type, packet->sender);

	if (packet->item_index == 0 || packet->item_index < -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_ITEM_INDEX);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->item_count < 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_ITEM_COUNT);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->nas < 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_INVALID_NAS);
		this->service_->deliver(rmsg);
		return;
	}

	if (strlen(packet->sender) == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToAllUser(rmsg, ResponseGMTool::ERR_SENDER_IS_EMPTY);
		this->service_->deliver(rmsg);
		return;
	}

	bool retbool = SubHelperInMessenger::instance()->sendMsgServerGroup(packet->server_no, msg);
	if (retbool)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToOneUser(rmsg, ResponseGMTool::ERR_NO_ERROR);
		this->service_->deliver(rmsg);

		LOG_INFO("Express System (from GMTOOL to user) : server_no[%d] / char_index[%d] / item_index[%d] / item_count[%d] / nas[%d] / sender[%s]",
				 packet->server_no, packet->char_index, packet->item_index, packet->item_count, packet->nas, packet->sender);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeSendToOneUser(rmsg, ResponseGMTool::ERR_INVALID_SERVER_NO);
		this->service_->deliver(rmsg);
	}
}

void GMToolProcessInMessenger::process_reserved_gm_command_add( CNetMsg::SP& msg )
{
	int nowTime = time(0);
	RequestGMTool::addReservedGMCommand* packet = reinterpret_cast<RequestGMTool::addReservedGMCommand*>(msg->m_buf);
	packet->element.a_startString[RESERVED_GM_COMMAND_MAX_STRING] = '\0';
	packet->element.a_endString[RESERVED_GM_COMMAND_MAX_STRING] = '\0';
	packet->element.a_title[RESERVED_GM_COMMAND_TITLE_MAX_STRING] = '\0';
	packet->element.a_start_title[RESERVED_GM_COMMAND_TITLE_MAX_STRING] = '\0';
	packet->element.a_end_title[RESERVED_GM_COMMAND_TITLE_MAX_STRING] = '\0';
	packet->element.a_desc[RESERVED_GM_COMMAND_DESC_MAX_STRING] = '\0';

	LOG_INFO("receive gm command : index[%d] eventType[%d] startTime[%d] startString[%s] endTime[%d] endString[%s] "
			 "imageType[%d] imageX[%d] imageY[%d] title[%s] startTitle[%s] endTitle[%s] desc[%s]",
			 packet->element.a_Index,
			 packet->element.a_evnetType,
			 packet->element.a_startTime,
			 packet->element.a_startString,
			 packet->element.a_endTime,
			 packet->element.a_endString,
			 packet->element.a_image_type,
			 packet->element.a_image_x,
			 packet->element.a_image_y,
			 packet->element.a_title,
			 packet->element.a_start_title,
			 packet->element.a_end_title,
			 packet->element.a_desc);

	if (packet->element.a_image_type != 0 && packet->element.a_image_type != 1)
	{
		LOG_ERROR("reserved gm command error : invalid image type[%d]", packet->element.a_image_type);

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeAddReservedGMCommand(rmsg, ResponseGMTool::ERR_INVALID_IMAGE_TYPE);
		this->service_->deliver(rmsg);
		return;
	}

	switch (packet->element.a_evnetType)
	{
	case eEVENT_TYPE_REGULAR:
	case eEVENT_TYPE_FREQUENTLY:
	case eEVENT_TYPE_OTHERS:
	case eEVENT_TYPE_INFO:
		break;
	default:
		{
			LOG_ERROR("reserved gm command error : invalid event type[%d]", packet->element.a_evnetType);

			CNetMsg::SP rmsg(new CNetMsg);
			ResponseGMTool::makeAddReservedGMCommand(rmsg, ResponseGMTool::ERR_INVALID_EVENT_TYPE);
			this->service_->deliver(rmsg);
		}
		return;
	}

	if (packet->element.a_startTime <= nowTime)
	{
		LOG_ERROR("reserved gm command error : start time[%d] less than now time[%d]", packet->element.a_startTime, nowTime);

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeAddReservedGMCommand(rmsg, ResponseGMTool::ERR_INVALID_START_TIME);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->element.a_endTime <= nowTime)
	{
		LOG_ERROR("reserved gm command error : end time[%d] less than now time[%d]", packet->element.a_endTime, nowTime);

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeAddReservedGMCommand(rmsg, ResponseGMTool::ERR_INVALID_END_TIME);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->element.a_endTime <= packet->element.a_startTime)
	{
		LOG_ERROR("reserved gm command error : start time[%d] less than end time[%d]", packet->element.a_startTime, packet->element.a_endTime);

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseGMTool::makeAddReservedGMCommand(rmsg, ResponseGMTool::ERR_END_TIME_LESS_THAN_START_TIME);
		this->service_->deliver(rmsg);
		return;
	}

	if (packet->element.a_evnetType != eEVENT_TYPE_INFO)
	{
		if (isValidGmCommand(packet->element.a_startString) == false)
		{
			LOG_ERROR("reserved gm command error : invalid start gm command [%s]", packet->element.a_startString);

			CNetMsg::SP rmsg(new CNetMsg);
			ResponseGMTool::makeAddReservedGMCommand(rmsg, ResponseGMTool::ERR_INVALID_START_PARAMETER);
			this->service_->deliver(rmsg);
			return;
		}

		if (isValidGmCommand(packet->element.a_endString) == false)
		{
			LOG_ERROR("reserved gm command error : invalid  end  gm command [%s]", packet->element.a_endString);

			CNetMsg::SP rmsg(new CNetMsg);
			ResponseGMTool::makeAddReservedGMCommand(rmsg, ResponseGMTool::ERR_INVALID_END_PARAMETER);
			this->service_->deliver(rmsg);
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 모든 유효성 검사가 끝났으므로 모든 SubHelper로 패킷 전달
	SubHelperInMessenger::instance()->sendMsgToAll(msg);

	LOG_INFO("OK gm command : index[%d] eventType[%d] startTime[%d] startString[%s] endTime[%d] endString[%s] "
			 "imageType[%d] imageX[%d] imageY[%d] title[%s] startTitle[%s] endTitle[%s] desc[%s]",
			 packet->element.a_Index,
			 packet->element.a_evnetType,
			 packet->element.a_startTime,
			 packet->element.a_startString,
			 packet->element.a_endTime,
			 packet->element.a_endString,
			 packet->element.a_image_type,
			 packet->element.a_image_x,
			 packet->element.a_image_y,
			 packet->element.a_title,
			 packet->element.a_start_title,
			 packet->element.a_end_title,
			 packet->element.a_desc);
}

bool GMToolProcessInMessenger::isValidGmCommand( std::string tstr )
{
	// 각 명령어의 유효성 검사
	typedef boost::tokenizer<boost::char_separator<char> > stokenizer;
	boost::char_separator<char> sep(" ", NULL, boost::drop_empty_tokens);
	stokenizer tok(tstr, sep);
	std::vector<std::string> vecstr(tok.begin(), tok.end());

	if (vecstr[0] == GM_CMD_JEWELRYE)
	{
		if (vecstr.size() != 3)
			return false;

		if (vecstr[1] != "0" && vecstr[1] != "1" && vecstr[1] != "2" && vecstr[1] != "3" && vecstr[1] != "4" && vecstr[1] != "all")
			return false;

		int percent = atoi(vecstr[2].c_str());
		if (percent < 1 || percent > 10000)
			return false;
	}
	else if(vecstr[0] == GM_CMD_EVENTAOUTOMATON)
	{
		if (vecstr.size() != 3)
			return false;

		if (vecstr[1] != "on" && vecstr[1] != "off")
			return false;
	}
	else
	{
		return false;
	}

	return true;
}

void GMToolProcessInMessenger::process_reserved_gm_command_delete( CNetMsg::SP& msg )
{
	SubHelperInMessenger::instance()->sendMsgToAll(msg);
}