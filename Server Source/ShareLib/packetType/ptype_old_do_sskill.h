#ifndef __PTYPE_OLD_DO_SSKILL_H__
#define __PTYPE_OLD_DO_SSKILL_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
struct sskill : public pTypeBase
{
	int		npcIndex;		// memory index
	int		sskillIndex;
};
}

namespace ResponseClient
{
struct sskillList : public pTypeBase
{
	short listCount;
	struct tag_list
	{
		int		skillIndex;
		char	skillLevel;
	} list[0];
};

struct sskillLearnError : public pTypeBase
{
	char		errorCode;
};

struct sskillLearn : public pTypeBase
{
	int			skillIndex;
	char		skillLevel;
	char		flag;
};

#ifndef _CLIENT_
inline void makeSSkillList(CNetMsg::SP& msg, CPC* ch)
{
	sskillList* packet = reinterpret_cast<sskillList*>(msg->m_buf);
	packet->type = MSG_SSKILL;
	packet->subType = MSG_SSKILL_LIST;
	packet->listCount = ch->m_sSkillList.m_nCount;

	int i = 0;
	CSSkillNode* node = ch->m_sSkillList.m_head;
	while (node)
	{
		packet->list[i].skillIndex = node->m_sskill->m_proto->m_index;
		packet->list[i].skillLevel = node->m_sskill->m_level;
		++i;

		node = node->m_next;
	}

	msg->setSize(sizeof(sskillList) + (packet->listCount * sizeof(sskillList::tag_list)));
}

inline void makeSSkillLearnError(CNetMsg::SP& msg, int errorCode)
{
	sskillLearnError* packet = reinterpret_cast<sskillLearnError*>(msg->m_buf);
	packet->type = MSG_SSKILL;
	packet->subType = MSG_SSKILL_LEARN_ERROR;
	packet->errorCode = errorCode;
	msg->setSize(sizeof(sskillLearnError));
}

inline void makeSSkillLearn(CNetMsg::SP& msg, CSSkill* sskill)
{
	sskillLearn* packet = reinterpret_cast<sskillLearn*>(msg->m_buf);
	packet->type = MSG_SSKILL;
	packet->subType = MSG_SSKILL_LEARN;
	packet->skillIndex = sskill->m_proto->m_index;
	packet->skillLevel = sskill->m_level;
	packet->flag = (sskill->m_level == 1) ? 1 : 0;
	msg->setSize(sizeof(sskillLearn));
}
#endif
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
