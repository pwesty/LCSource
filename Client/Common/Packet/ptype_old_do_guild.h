#ifndef __PTYPE_OLD_DO_GUILD_H__
#define __PTYPE_OLD_DO_GUILD_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
// subtype - MSG_NEW_GUILD_MARK_EDIT_FIN
struct doNewGuildSkillMarkEditFinish : public pTypeBase
{
	char			gm_row;
	char			gm_col;
	char			bg_row;
	char			bg_col;
	unsigned short	tab;
	unsigned short	invenIndex;
};

// subtype - MSG_NEW_GUILD_STASH_KEEP
struct doNewGuildStashKeep : public pTypeBase
{
	LONGLONG			keepMoney;
	unsigned short		listCount;
	struct
	{
		unsigned short	tab;
		unsigned short	invenIndex;
		int				itemDBIndex;
		LONGLONG		count;
	} list[0];
};

struct doNewGuildStashTake : public pTypeBase
{
	int					guildIndex;
	LONGLONG			takeMoney;
	unsigned short		listCount;
	struct
	{
		int				stashIndex;
		int				itemCount;
	} list[0];
};
}

namespace UpdateClient
{
	struct doNewGuildSkillLearnToMemberMsg : public pTypeBase
	{
		unsigned char		skill_type;
		int					skill_index;
		char				skill_level;
	};

#ifndef _CLIENT_
	inline void makenewGuildSkillLearnToMemberMsg(CNetMsg::SP& msg, unsigned char skill_type, int skill_index, char skill_level)
	{
		doNewGuildSkillLearnToMemberMsg *packet = reinterpret_cast<doNewGuildSkillLearnToMemberMsg*>(msg->m_buf);
		packet->type = MSG_GUILD;
		packet->subType = MSG_NEW_GUILD_SKILL_LEARN;
		packet->skill_type = skill_type;
		packet->skill_index = skill_index;
		packet->skill_level = skill_level;
		msg->setSize(sizeof(doNewGuildSkillLearnToMemberMsg));
	}
#endif

}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
