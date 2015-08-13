#include "stdhdrs.h"

#include "../ShareLib/LogInOutManager.h"
#include "Descriptor.h"
#include "Server.h"
#include "CmdMsg.h"

CDescriptor::CDescriptor(rnSocketIOService* service)
	: service_(service)
{
	m_serverNo = -1;
	m_subNo = -1;
	m_zones = NULL;
	m_countZone = 0;

	m_connected = 0;
	m_logined	= 0;

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
	if (msg->m_mtype == MSG_UNKNOWN)
		return;

	if (service_ == NULL)
		return;

	service_->deliver(msg);
}

bool CDescriptor::GetLogin(CNetMsg::SP& msg)
{
	msg->MoveFirst();

	if (msg->m_mtype != MSG_MSGR_CONNECT)
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

		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrRecommendMsg(rmsg);
			SEND_Q(rmsg, this);
		}

		switch (gserver.m_sGoldenBallData.nStatus)
		{
		case GOLDENBALL_STATUS_VOTE:
			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrEventGoldenballVoteMsg(rmsg, 0, gserver.m_sGoldenBallData.strTeam1, gserver.m_sGoldenBallData.strTeam2, gserver.m_sGoldenBallData.nYear, gserver.m_sGoldenBallData.nMonth, gserver.m_sGoldenBallData.nDay, gserver.m_sGoldenBallData.nHour, gserver.m_sGoldenBallData.nMin);
				SEND_Q(rmsg, this);
			}
			break;

		case GOLDENBALL_STATUS_GIFT:
			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrEventGoldenballGiftMsg(rmsg, 0, gserver.m_sGoldenBallData.strTeam1, gserver.m_sGoldenBallData.nTeam1Score, gserver.m_sGoldenBallData.strTeam2, gserver.m_sGoldenBallData.nTeam2Score, gserver.m_sGoldenBallData.nYear, gserver.m_sGoldenBallData.nMonth, gserver.m_sGoldenBallData.nDay, gserver.m_sGoldenBallData.nHour, gserver.m_sGoldenBallData.nMin, gserver.m_sGoldenBallData.timeEndVote);
				SEND_Q(rmsg, this);
			}
			break;
		}
	}
	else
	{
		m_countZone = 0;
		m_zones = NULL;

		if (servernum == LOGIN_SERVER_NUM)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrRecommendMsg(rmsg);
			SEND_Q(rmsg, this);
		}
	}

	LOG_INFO("CONNECTED %s %d %d %d %d", service_->ip().c_str(), version, m_serverNo, m_subNo, m_countZone);

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

CDescriptor* CDescriptor::create(rnSocketIOService* service)
{
	CDescriptor* p = new CDescriptor(service);
	return p;
}

void CDescriptor::onClose( rnSocketIOService* service )
{
	LOG_INFO("CLIENT DISCONNECTED... IP[%s]", service->ip().c_str());

// 	LogInOutManager::Instance()->DeleteBySocketIO(this->service_);

	service_ = NULL;
	REMOVE_FROM_BILIST(this, gserver.m_desclist, m_pPrev, m_pNext);

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
	}
}

//