#include "stdhdrs.h"

#include "Cmd.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"

CCmd::CCmd()
{
}

CCmd::~CCmd()
{
}

CCmdList::CCmdList()
{
	Add(MSG_MSGR_REQ, do_Request, 0);
	Add(MSG_MSGR_REP, do_Reply, 0);
}

CCmdList::~CCmdList()
{
}

bool CCmdList::Add(int cmdType, void (*cmdFunc)(CNetMsg::SP&, CDescriptor*), int minlevel)
{
	CCmd p;
	p.m_cmdType = cmdType;
	p.m_cmdFunc = cmdFunc;
	p.m_minLevel = minlevel;

	map_.insert(map_t::value_type(cmdType, p));

	return true;
}

bool CCmdList::Run(int cmd_num, CNetMsg::SP& msg, CDescriptor* dest)
{
	map_t::iterator it = map_.find(cmd_num);
	if (it == map_.end())
		return false;

	(*(it->second).m_cmdFunc)(msg, dest);
	return true;
}
//