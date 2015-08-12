#ifndef __PTYPE_ARTIFACT_H__
#define __PTYPE_ARTIFACT_H__

#include "ptype_base.h"

//systype
enum
{
	ARTIFACT_SYSMSG_TAKE,				//%s님이 %s지역에서 %s를 획득
	ARTIFACT_SYSMSG_COMPOSE,			//%s님이 %s지역에서 유물 아이템을 합성하여 %s를 만들었습니다.
	ARTIFACT_SYSMSG_DIE_TO_NPC,			//%s님이 몬스터에게 사망하여 보유중인 %s이(가) 사라졌습니다.
	ARTIFACT_SYSMSG_LOGOUT,				//%s님이 게임에서 이탈하여 보유 중인 %s이(가) 사라졌습니다.
	ARTIFACT_SYSMSG_MOVE_ZONE,			//%s님이 %s지역으로 이동하였습니다.
	ARTIFACT_SYSMSG_EXPIRE_TIME,		//%s님이 보유중인 %s의 사용시간이 다 되어 사라졌습니다.
};


#pragma pack(push, 1)


//request
namespace RequestClient
{
}

//response
namespace ResponseClient
{
}

//update
namespace UpdateClient
{
	struct Artifact_SysMsg : public pTypeBase
	{
		int item_index;
		int zone_index;
		char name[50];
		int systype;
	};

	struct ArtifactFindUserCountMsg : public pTypeBase
	{
		int count;
	};

#ifndef _CLIENT_
	inline void makeArtifact_SysMsg(CNetMsg::SP& rmsg, int item_index, int zone_index, const char* name, int systype)
	{
		Artifact_SysMsg* packet = reinterpret_cast<Artifact_SysMsg*>(rmsg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_ARTIFACT_SYSMSG;
		packet->item_index = item_index;
		packet->systype = systype;
		packet->zone_index = zone_index;
		strncpy(packet->name, name, 50);
		rmsg->setSize(sizeof(Artifact_SysMsg));
	}

	inline void makeArtifactFindUserCountMsg(CNetMsg::SP& rmsg, int count)
	{
		ArtifactFindUserCountMsg* packet = reinterpret_cast<ArtifactFindUserCountMsg*>(rmsg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_ARTIFACT_FIND_USER_COUNT_MSG;
		packet->count = count;
		
		rmsg->setSize(sizeof(ArtifactFindUserCountMsg));
	}
#endif
}

#pragma pack(pop)

#endif