#ifndef __PTYPE_OLD_DO_ACTION_H__
#define __PTYPE_OLD_DO_ACTION_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	struct action : public pTypeBase 
	{
		int		targetIndex;
		char	typevalue;
		char	index;
	};
}

namespace ResponseClient
{
	struct action : public pTypeBase
	{
		int		charIndex;
		char	typevalue;
		char	actionvalue;
		char	state;
	};

#ifndef _CLIENT_
	inline void makeAction(CNetMsg::SP& msg, int charIndex, int type, int action, int state)
	{
		action* packet = reinterpret_cast<action*>(msg->m_buf);
		packet->type = MSG_ACTION;
		packet->subType = 0;
		packet->charIndex = charIndex;
		packet->type = type;
		packet->action = action;
		packet->state = state;
		msg->setSize(sizeof(action));
	}
#endif
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
