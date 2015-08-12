#ifndef __PTYPE_OLD_MEMPOS_H__
#define __PTYPE_OLD_MEMPOS_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
struct memposWrite : public pTypeBase
{
	char		slot;
	char		comment[MEMPOS_COMMENT_LENGTH + 1];
};

struct memposMove : public pTypeBase
{
	char		slot;
};
}

namespace ResponseClient
{
struct memposElement
{
	unsigned char	index;
	int				zone;
	float			x;
	float			z;
	char		comment[MEMPOS_COMMENT_LENGTH + 1];
};

struct memposWrite : public pTypeBase
{
	memposElement	data;
};

struct memposList : public pTypeBase
{
	int					memposTime;
	unsigned short		listCount;
	memposElement		list[0];
};

#ifndef _CLIENT_
inline void makeMemposWrite(CNetMsg::SP& msg, CPC* ch, int slot)
{
	memposWrite* packet = reinterpret_cast<memposWrite*>(msg->m_buf);
	packet->type = MSG_MEMPOS;
	packet->subType = MSG_MEMPOS_WRITE;
	packet->data.index = slot;
	packet->data.zone = ch->m_mempos.m_data[slot]->m_zone;
	packet->data.x = ch->m_mempos.m_data[slot]->m_x;
	packet->data.z = ch->m_mempos.m_data[slot]->m_z;
	memcpy(packet->data.comment, ch->m_mempos.m_data[slot]->m_comment, MEMPOS_COMMENT_LENGTH);
	packet->data.comment[MEMPOS_COMMENT_LENGTH] = '\0';
	msg->setSize(sizeof(memposWrite));
}

inline void makeMemposList(CNetMsg::SP& msg, CPC* ch)
{
	memposList* packet = reinterpret_cast<memposList*>(msg->m_buf);
	packet->type = MSG_MEMPOS;
	packet->subType = MSG_MEMPOS_LIST;

	if(ch->m_memposTime - gserver->getNowSecond() <= 0 )
		ch->m_memposTime = 0;

	packet->memposTime = ch->m_memposTime;

	int count = 0;
	for (int i = 0; i < MAX_MEMPOS; ++i)
	{
		if (ch->m_mempos.m_data[i] == NULL)
			continue;

		packet->list[count].index = i;
		packet->list[count].zone = ch->m_mempos.m_data[i]->m_zone;
		packet->list[count].x = ch->m_mempos.m_data[i]->m_x;
		packet->list[count].z = ch->m_mempos.m_data[i]->m_z;
		memcpy(packet->list[count].comment, ch->m_mempos.m_data[i]->m_comment, MEMPOS_COMMENT_LENGTH);
		packet->list[count].comment[MEMPOS_COMMENT_LENGTH] = '\0';

		++count;
	}
	packet->listCount = count;

	msg->setSize(sizeof(memposList) + (count * sizeof(memposElement)));
}
#endif
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
