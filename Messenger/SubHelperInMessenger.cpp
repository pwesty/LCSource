#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Server.h"
#include "SubHelperInMessenger.h"

SubHelperInMessenger* SubHelperInMessenger::instance()
{
	static SubHelperInMessenger __instance;
	return &__instance;
}

bool SubHelperInMessenger::Init()
{
	std::string bind_host = gserver.m_config.Find("Server", "IP");
	int bind_port = atoi(gserver.m_config.Find("Server", "SubHelperPort"));
	session_handle handle = bnf::instance()->CreateListen(bind_host, bind_port, 0, this);

	return (handle != SessionBase::INVALID_SESSION_HANDLE) ? true : false;
}

void SubHelperInMessenger::insert(int serverno, SubHelperProcessInMessenger* sub)
{
	subhelper_map_.insert(map_t::value_type(serverno, sub));
}

void SubHelperInMessenger::erase(int serverno)
{
	subhelper_map_.erase(serverno);
}

bool SubHelperInMessenger::sendMsgServerGroup(int serverno, CNetMsg::SP& rmsg)
{
	map_t::iterator it = subhelper_map_.find(serverno);
	if (it == subhelper_map_.end())
		return false;

	SubHelperProcessInMessenger* p = it->second;
	p->WriteToOutput(rmsg);

	return true;
}

void SubHelperInMessenger::sendMsgToAll( CNetMsg::SP& rmsg )
{
	BOOST_FOREACH(map_t::value_type& p, subhelper_map_)
	{
		SubHelperProcessInMessenger* s = p.second;
		s->WriteToOutput(rmsg);
	}
}

void SubHelperInMessenger::operate( rnSocketIOService* service )
{
	SubHelperProcessInMessenger* p = new SubHelperProcessInMessenger(service);
	service->SetUserData(p);

	LOG_INFO("############################ SubHelper is connected. / ip[%s] ############################\n",
			 service->ip().c_str());
}

//////////////////////////////////////////////////////////////////////////
void SubHelperProcessInMessenger::WriteToOutput(CNetMsg::SP& msg)
{
	if (msg->m_mtype == MSG_UNKNOWN)
		return;

	if (service_ == NULL)
		return;

	service_->deliver(msg);
}

void SubHelperProcessInMessenger::onClose( rnSocketIOService* service )
{
	service_ = NULL;

	bnf::instance()->RemoveSession(service);
	SubHelperInMessenger::instance()->erase(this->server_no_);

	delete this;

	LOG_INFO("\n############################\n"
			 "SubHelper is disconnected. / ip[%s]\n"
			 "############################\n", service->ip().c_str());
}

void SubHelperProcessInMessenger::operate( rnSocketIOService* service )
{
	CNetMsg::SP msg(service->GetMessage());

	switch (msg->m_mtype)
	{
	case MSG_EXPRESS_SYSTEM:
		break;

	case MSG_SUBHELPER_CONNECT:
		{
			RefMsg(msg) >> this->server_no_;
			SubHelperInMessenger::instance()->insert(this->server_no_, this);
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