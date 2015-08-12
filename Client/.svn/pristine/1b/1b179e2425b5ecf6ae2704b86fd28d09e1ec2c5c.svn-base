#ifndef __PTYPE_OLD_EXTEND_H__
#define __PTYPE_OLD_EXTEND_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	struct doSetTargetInfo : public pTypeThirdBase
	{
		int charIndex;			//PC의 인덱스 나머지는 -1,(NPC,PET,ELE...)
	};
}

namespace ResponseClient
{
// 캐릭터 선택창으로 돌아가기
// pTypeThirdBase:thirdType = 0 (by user), = 1 (by hardcore system)
struct exRestart : public pTypeThirdBase
{
};

struct ToggleMsg : public pTypeBase
{
	int toggle_type;
	int index;
	bool toggle;
};

#ifndef _CLIENT_
inline void makeExRestart(CNetMsg::SP& msg, char flag)
{
	exRestart* packet = reinterpret_cast<exRestart*>(msg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_RESTART;
	packet->thirdType = flag;
	msg->setSize(sizeof(exRestart));
}

inline void makeToggleMsg(CNetMsg::SP& msg, int index, bool toggle, int toggle_type)
{
	ToggleMsg * packet = reinterpret_cast<ToggleMsg*>(msg->m_buf);
	packet->type = MSG_SKILL;
	packet->subType = MSG_SKILL_TOGGLE;

	packet->index = index;
	packet->toggle = toggle;
	packet->toggle_type = toggle_type;
	msg->setSize(sizeof(ToggleMsg));
}
#endif
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
