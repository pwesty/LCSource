#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "../ShareLib/LogInOutManager.h"
#include "Log.h"
#include "Descriptor.h"
#include "Server.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"

CDescriptor::CDescriptor(rnSocketIOService* service, bool client_type)
	: service_(service)
	, client_type_(client_type)
{
	m_serverNo = -1;
	m_subNo = -1;
	m_zones = NULL;
	m_countZone = 0;

	m_connected = 0;
	m_uniqeKey = getUniqueKey();

	m_pPrev = NULL;
	m_pNext = NULL;
}

CDescriptor::~CDescriptor()
{
	if (m_zones)
		delete [] m_zones;
}

int CDescriptor::getUniqueKey()
{
	static int _key = 0;
	return ++_key;
}

void CDescriptor::WriteToOutput(CNetMsg::SP& msg)
{
	if (msg->m_mtype == MSG_UNKNOWN)
		return ;

	if (service_ == NULL)
		return;

	service_->deliver(msg);
}

bool CDescriptor::GetLogin(CNetMsg::SP& msg)
{
	msg->MoveFirst();

	if (msg->m_mtype != MSG_SUBHELPER_CONNECT)
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

void CDescriptor::onClose( rnSocketIOService* service )
{
	bnf::instance()->RemoveSession(service);

	// 이 세션에게 보낸 메시지 삭제
	gserver.m_msgList.RemoveServer(this);

	if (client_type_) // game server
	{
		LogInOutManager::Instance()->DeleteBySocketIO(this->service_);

		REMOVE_FROM_BILIST(this, gserver.m_desclist, m_pPrev, m_pNext);
	}
	else // user client
	{
	}

	service_ = NULL;

	delete this;
}

void CDescriptor::operate( rnSocketIOService* service )
{
	CNetMsg::SP msg(service->GetMessage());

	if (client_type_) // game server
	{
		if (STATE(this) != CON_GET_LOGIN)
		{
			gserver.CommandInterpreter(this, msg);
		}
		else
		{
			this->GetLogin(msg);
		}
	}
	else // user client
	{
	}
}