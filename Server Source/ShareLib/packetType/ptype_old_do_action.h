#ifndef __PTYPE_OLD_DO_ACTION_H__
#define __PTYPE_OLD_DO_ACTION_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
struct action : public pTypeBase
{
	int		targetIndex;		// AGT_THROW_WATER 일때만 유효함
	char	type;
	char	index;
};
}

namespace ResponseClient
{
struct action : public pTypeBase
{
	int		charIndex;
	char	typeValue;
	char	actionValue;
	char	state;
};

#ifndef _CLIENT_
inline void makeAction(CNetMsg::SP& msg, CPC* ch, int typeValue, int actionValue)
{
	action* packet = reinterpret_cast<action*>(msg->m_buf);
	packet->type = MSG_ACTION;
	packet->subType = 0;
	packet->charIndex = ch->m_index;
	packet->typeValue = typeValue;
	packet->actionValue = actionValue;
	packet->state = ch->GetPlayerState();
	msg->setSize(sizeof(action));
}
#endif
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
