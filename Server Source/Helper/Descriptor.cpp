#include <boost/format.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "../ShareLib/LogInOutManager.h"
#include "Log.h"
#include "Descriptor.h"
#include "Server.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"

#ifdef PREMIUM_CHAR
#include "../ShareLib/packetType/ptype_premium_char.h"
#endif

CDescriptor::CDescriptor(rnSocketIOService* service)
	: service_(service)
{
	m_serverNo = -1;
	m_subNo = -1;
	m_zones = NULL;
	m_countZone = 0;

	m_connected = 0;

	m_pPrev = NULL;
	m_pNext = NULL;
}

CDescriptor::~CDescriptor()
{
	if (m_zones)
		delete [] m_zones;
}

void CDescriptor::WriteToOutput(CNetMsg::SP& msg)
{
	if (service_ == NULL)
		return;

	if (msg->m_mtype == MSG_UNKNOWN)
		return;

	service_->deliver(msg);
}

bool CDescriptor::GetLogin(CNetMsg::SP& msg)
{
	RefMsg(msg).MoveFirst();

	if (RefMsg(msg).m_mtype != MSG_HELPER_CONNECT)
		return false;

	int version, servernum, subnum, count_zone;

	RefMsg(msg) >> version
				>> servernum
				>> subnum
				>> count_zone;

	if (version < SERVER_MIN_VERSION || version > SERVER_MAX_VERSION)
		return false;

	m_serverNo = servernum;
	m_subNo = subnum;

	if (servernum != LOGIN_SERVER_NUM && servernum != CONNECTOR_SERVER_NUM)
	{
		if (count_zone < 1)
			return false;

		m_countZone = count_zone;
		m_zones = new int[m_countZone];

		int i;
		int zone_num;
		for (i = 0; i < m_countZone; i++)
		{
			RefMsg(msg) >> zone_num;
			m_zones[i] = zone_num;
		}
	}
	else
	{
		m_countZone = 0;
		m_zones = NULL;
	}

	GAMELOG << init("CONNECTED", service_->ip().c_str())
			<< version << delim
			<< m_serverNo << delim
			<< m_subNo << delim
			<< m_countZone
			<< end;

	STATE(this) = CON_PLAYING;

	CDBCmd cmd3;
	int moonstone_nas = 0;
	cmd3.Init(&gserver.m_dbchar);

	std::string select_event_dev_query = boost::str(boost::format("SELECT a_moonstone FROM t_event_dev WHERE a_group_index = %d") % servernum);
	cmd3.SetQuery(select_event_dev_query);

	if (!cmd3.Open())
	{
		GAMELOG << init("SYS_ERR")
				<< "Cannot load moonstone"
				<< end;
		return false;
	}
	if (cmd3.MoveNext())
	{
		cmd3.GetRec("a_moonstone", moonstone_nas);
		gserver.m_nMoonStoneNas = moonstone_nas;
	}
	else
	{
		gserver.m_nMoonStoneNas = 0;
	}

	CNetMsg::SP mmsg(new CNetMsg);
	HelperEventMoonStoneLoadMsg(mmsg, moonstone_nas);
	SEND_Q(mmsg, this);

	// 파티 정보 보내기
	SendPartyInfo();

#ifdef PREMIUM_CHAR
	// 게임 서버라면 프리미엄캐릭터 점프 카운트를 초기화 하는 시간을 전송
	if (version == SERVER_VERSION)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ServerToServerPacket::premiumCharResetJumpCountTime* packet = reinterpret_cast<ServerToServerPacket::premiumCharResetJumpCountTime*>(rmsg->m_buf);
		packet->type = MSG_PREMIUM_CHAR;
		packet->subType = MSG_SUB_PREMIUM_CHAR_RESET_JUMP_COUNT_TIME;
		packet->resetTime = gserver.m_premiumchar_reset_jump_count_time;
		rmsg->setSize(sizeof(ServerToServerPacket::premiumCharResetJumpCountTime));
		SEND_Q(rmsg, this);
	}
#endif

	return true;
}

int CDescriptor::FindZone(int zone)
{
	int i;
	for (i = 0; i < m_countZone; i++)
	{
		if (m_zones[i] == zone)
			return i;
	}

	return -1;
}

void CDescriptor::SendPartyInfo()
{
	if (m_serverNo != LOGIN_SERVER_NUM && m_serverNo != CONNECTOR_SERVER_NUM)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyInfoEndMsg(rmsg);
		SEND_Q(rmsg, this);
	}
}

void CDescriptor::onClose( rnSocketIOService* service )
{
	LOG_INFO("CLIENT DISCONNECTED... IP[%s]", service->ip().c_str());

	bnf::instance()->RemoveSession(service);

	LogInOutManager::Instance()->DeleteBySocketIO(this->service_);

	service_ = NULL;

	gserver.m_msgList.RemoveServer(this);

	REMOVE_FROM_BILIST(this, gserver.m_desclist, m_pPrev, m_pNext);
	gserver.m_msgList.RemoveServer(this);

	delete this;
}

void CDescriptor::operate( rnSocketIOService* service )
{
	CNetMsg::SP msg(service->GetMessage());

	if (STATE(this) != CON_GET_LOGIN)
	{
		gserver.CommandInterpreter(this, msg);
	}
	else
	{
		GetLogin(msg);

		gserver.m_syndicate.sendInfo();
		gserver.m_syndicate.sendKingInfo();
	}
}