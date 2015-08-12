#ifndef __PTYPE_OLD_DO_MONSTER_COMBO_H__
#define __PTYPE_OLD_DO_MONSTER_COMBO_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
struct MCEditComplete : public pTypeThirdBase
{
	int			npcIndex;		// memory index
	int			count;
	int			list[0];
};

struct MCGotoCombo : public pTypeThirdBase
{
	int			npcIndex;
	int			zoneIndex;
	int			extra;
};

struct MCConfirm : public pTypeThirdBase
{
	char		yesno;
	int			bossIndex;
};

struct MCGiveup : public pTypeThirdBase
{
	int			npcIndex;
	int			zoneIndex;
	int			extra;
};
}

namespace ResponseClient
{
struct MCErrorMsg : public pTypeThirdBase
{
	char		errorCode;
};

struct MCEditContext : public pTypeThirdBase
{
	int			count;
	int			list[0];
};

struct MCMissionComplete : public pTypeThirdBase
{
	int			success;
	int			stage;
};

struct MCNoticeStage : public pTypeThirdBase
{
	int			stage;
};

struct MCGotoWaitRoomPrompt : public pTypeThirdBase
{
	int			boosIndex;
	LONGLONG	nas;
};

#ifndef _CLIENT_
inline void makeMBErrorMsg(CNetMsg::SP& msg, MSG_EX_MONSTERCOMBO_ERROR_TYPE errtype)
{
	MCErrorMsg* packet = reinterpret_cast<MCErrorMsg*>(msg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_MONSTERCOMBO;
	packet->thirdType = MSG_EX_MONSTERCOMBO_ERROR;
	packet->errorCode = errtype;
	msg->setSize(sizeof(MCErrorMsg));
}

inline void makeMBEditContext(CNetMsg::SP& msg, std::vector<int>& vec)
{
	MCEditContext* packet = reinterpret_cast<MCEditContext*>(msg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_MONSTERCOMBO;
	packet->thirdType = MSG_EX_MONSTERCOMBO_EDIT_CONTEXT_REP;
	packet->count = vec.size();
	int i = 0;
	BOOST_FOREACH(int index, vec)
	{
		packet->list[i] = vec[i];
		++i;
	}
	msg->setSize(sizeof(MCEditContext) + (packet->count * sizeof(int)));
}

inline void makeMCMissionComplete(CNetMsg::SP& msg, int success, int stage)
{
	MCMissionComplete* packet = reinterpret_cast<MCMissionComplete*>(msg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_MONSTERCOMBO;
	packet->thirdType = MSG_EX_MONSTERCOMBO_MISSION_COMPLETE;
	packet->success = success;
	packet->stage = stage;
	msg->setSize(sizeof(MCMissionComplete));
}

inline void makeMCNoticeStage(CNetMsg::SP& msg, int stage)
{
	MCNoticeStage* packet = reinterpret_cast<MCNoticeStage*>(msg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_MONSTERCOMBO;
	packet->thirdType = MSG_EX_MONSTERCOMBO_NOTICE_STAGE;
	packet->stage = stage;
	msg->setSize(sizeof(MCNoticeStage));
}

inline void makeMCGotoWaitRoomPrompt(CNetMsg::SP& msg, int bossIndex, LONGLONG nas)
{
	MCGotoWaitRoomPrompt* packet = reinterpret_cast<MCGotoWaitRoomPrompt*>(msg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_MONSTERCOMBO;
	packet->thirdType = MSG_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT;
	packet->boosIndex = bossIndex;
	packet->nas = nas;
	msg->setSize(sizeof(MCGotoWaitRoomPrompt));
}
#endif
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
