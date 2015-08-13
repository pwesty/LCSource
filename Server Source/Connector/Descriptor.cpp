#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Log.h"
#include "Descriptor.h"
#include "Server.h"
#include "CmdMsg.h"

CDescriptor::CDescriptor(rnSocketIOService* service)
	: service_(service)
	, m_ipAddr(HOST_LENGTH + 1)
{
	m_connected = 0;
	m_connectreq = 0;
	m_descnum	= 0;
	m_playmode = MSG_LOGIN_NEW;  // default play mode
	m_bclosed	= false;
	m_bStartServer = false;
	m_bLoginServer = false;
	m_subno = 0;

	m_waitbill = 0;					// 빌링에 요청 결과 기다림
	m_billtimeout = 0;				// 빌링 대기시간

	m_nCountZone = 0;
	m_nZoneIndex = NULL;

	m_pPrev = NULL;
	m_pNext = NULL;
	m_portNumber = 0;
}

CDescriptor::~CDescriptor()
{
	if (m_nZoneIndex)
		delete [] m_nZoneIndex;
	m_nZoneIndex = NULL;
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

	if (msg->m_mtype != MSG_CONN_CONNECT)
	{
		GAMELOG << init("BAD_CONNECTION", service_->ip().c_str())
				<< end;
		return false;
	}

	int version;
	int serverno, subnum, count_zone;
	int hardcore_flag;
	CLCString address(HOST_LENGTH + 1);
	int port;
	CLCString pw(MAX_PWD_LENGTH + 1);

	RefMsg(msg) >> version
				>> serverno
				>> subnum
				>> address
				>> port
				>> hardcore_flag
				>> count_zone;

	if (serverno == PLAYER_COUNT_NUM)
	{
		RefMsg(msg) >> pw;
		if (strcmp(pw, "tjwjddnjsehd") != 0)
		{
			GAMELOG << init("BAD_CONNECTION", service_->ip().c_str())
					<< version
					<< delim
					<< serverno
					<< delim
					<< subnum
					<< end;

			return false;
		}
	}

	// 버전이 맞는지, 서버 군이 맞는지, 혹은 요청한 녀석이 로그인,메신저 서버인지 검사
	if (version < SERVER_MIN_VERSION
			|| version > SERVER_MAX_VERSION
			|| ((serverno != gserver.m_serverno || subnum < 1 || subnum > gserver.m_maxSubServer) && serverno != LOGIN_SERVER_NUM && serverno != MESSENGER_SERVER_NUM && serverno != PLAYER_COUNT_NUM))
	{
		GAMELOG << init("BAD_CONNECTION", service_->ip().c_str())
				<< version
				<< delim
				<< serverno
				<< delim
				<< subnum
				<< end;
		return false;
	}

	if (serverno != LOGIN_SERVER_NUM && serverno!= PLAYER_COUNT_NUM && serverno != CONNECTOR_SERVER_NUM)
	{
		if (gserver.m_hardcore_flag_in_connector != hardcore_flag)
		{
			if (gserver.m_hardcore_flag_in_connector)
			{
				LOG_ERROR("This server-group is Hardcore But %d-%d Gameserver is non-hardcore.", serverno, subnum);
			}
			else
			{
				LOG_ERROR("This server-group is Non-Hardcore But %d-%d Gameserver is hardcore.", serverno, subnum);
			}

			this->service_->Close("invalid hardcore flag");

			return false;
		}

		if (subnum < 1 || subnum > gserver.m_maxSubServer)
			return false;

		if (count_zone < 1)
			return false;

		// 팔린 상품 갯수를 각 게임서버로 보냄
		CNetMsg::SP rmsg(new CNetMsg);
		LimitCatalogMsg(rmsg);
		SEND_Q(rmsg, this);

		m_nCountZone = count_zone;
		m_nZoneIndex = new int[m_nCountZone];

		int i;
		int zone_num;
		for (i = 0; i < count_zone; i++)
		{
			RefMsg(msg) >> zone_num;
			m_nZoneIndex[i] = zone_num;
			gserver.m_userList[subnum - 1].m_playersPerZone[zone_num] = 0;
			if (zone_num == ZONE_START)
				m_bStartServer = true;
		}

		m_subno = subnum;

		// 이전에 같은 서버 같은 존으로 등록한거 있으면 접속 불능
		CDescriptor* p;
		CDescriptor* pNext = gserver.m_desclist;
		bool bDuplicationZone = false;
		while ((p = pNext))
		{
			pNext = p->m_pNext;
			if (!p->m_bLoginServer && p->m_subno > 0 && p->m_subno == m_subno && this != p)
			{
				int thisZoneI;
				int pZoneI;
				for (thisZoneI = 0; !bDuplicationZone && thisZoneI < m_nCountZone; thisZoneI++)
				{
					for (pZoneI = 0; pZoneI < p->m_nCountZone; pZoneI++)
					{
						if (m_nZoneIndex[thisZoneI] == p->m_nZoneIndex[pZoneI])
						{
							bDuplicationZone = true;
							break;
						}
					}
				}
				if (bDuplicationZone)
				{
					return false;
				}
			}
		}
	}
	else if (serverno == LOGIN_SERVER_NUM)
	{
		m_bLoginServer = true;
	}

	m_ipAddr = address;
	m_portNumber = port;

	STATE(this) = CON_PLAYING;

	if(serverno == PLAYER_COUNT_NUM )
		return true;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		PlayerRepMsg(rmsg);

		CDescriptor* desc = gserver.m_desclist;
		while (desc)
		{
			if (desc->m_bLoginServer)
			{
				SEND_Q(rmsg, desc);
			}
			desc = desc->m_pNext;
		}
	}

	return true;
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
		this->GetLogin(msg);
	}
}

void CDescriptor::onClose( rnSocketIOService* service )
{
	bnf::instance()->RemoveSession(service);

	gserver.CloseSocket(this);
	service_ = NULL;

	delete this;
}