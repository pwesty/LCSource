#ifndef __PTYPE_OLD_DO_MOVE_H__
#define __PTYPE_OLD_DO_MOVE_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
struct moveForPernalDungeon : public pTypeBase
{
	unsigned char	moveType;
	unsigned char	multicount;
	struct tag_multi
	{
		int			multiIndex;
		float		x;
		float		z;
		float		h;
		float		r;
		char		ylayer;
	} list[0];
};

struct moveForNormal : public pTypeBase
{
	unsigned char	charType;
	unsigned char	moveType;
	char			ylayer;
	int				index;
	float			speed;
	float			x;
	float			z;
	float			h;
	float			r;
};

struct moveGoZone : public pTypeBase
{
	unsigned short	zone;
	unsigned short	extra;
	int				npcIndex;
};

#ifndef _CLIENT_
inline void makeGoZone(CNetMsg::SP& msg, int zone, int extra, int npcIndex)
{
	moveGoZone* packet = reinterpret_cast<moveGoZone*>(msg->m_buf);
	packet->type = MSG_GO_ZONE;
	packet->subType = 0;
	packet->zone = zone;
	packet->extra = extra;
	packet->npcIndex = npcIndex;
	msg->setSize(sizeof(moveGoZone));
}
#endif
}

namespace ResponseClient
{
struct moveGoZone : public pTypeBase
{
	int				zone;
	int				extra;
	char			ip[15 + 1];
	unsigned short	port;
};

struct moveMsg : public pTypeBase
{
	char			charType;
	char			moveType;
	int				charIndex;

	float			speed;

	float			x;
	float			z;
	float			h;
	float			r;
	char			cYlayer;

	unsigned short	mapAttr;
};

struct goMsg : public pTypeBase
{
	int				zoneIndex;
	float			x;
	float			z;
	float			h;
	float			r;
	char			cYlayer;
	char			hack;
};

#ifndef _CLIENT_
inline void makeGoZone(CNetMsg::SP& msg, int zone, int extra, const char* ip, int port)
{
	moveGoZone* packet = reinterpret_cast<moveGoZone*>(msg->m_buf);
	packet->type = MSG_GO_ZONE;
	packet->subType = 0;
	packet->zone = zone;
	packet->extra = extra;
	memcpy(packet->ip, ip, sizeof(packet->ip));
	packet->ip[15] = '\0';
	packet->port = port;
	msg->setSize(sizeof(moveGoZone));
}

inline void makeMoveMsg(CNetMsg::SP& msg, MSG_MOVE_TYPE movetype, CCharacter* ch)
{
	// 안 보이면 무시
	if (!ch->m_bVisible)
	{
		msg->Init();
		return ;
	}

	float walkSpeed = ch->m_walkSpeed;
	float runSpeed = ch->GetRunSpeed();
	float flySpeed = ch->m_flySpeed;

	if(ch->m_pZone->m_index == ZONE_DRATAN_CASTLE_DUNGEON && IS_PC(ch))
	{
		CDratanCastle * pCastle = CDratanCastle::CreateInstance();
		if(pCastle->m_dvd.GetZone() && pCastle->m_dvd.GetEnvRate() >= 90)
		{
			walkSpeed = walkSpeed + walkSpeed*0.1;
			runSpeed = runSpeed + runSpeed*0.1;
			flySpeed = flySpeed + flySpeed*0.1;
		}
	}

	if (IS_PC(ch) && TO_PC(ch)->m_wearInventory.wearItemInfo[WEARING_PET])
	{
		CPet* pet = TO_PC(ch)->GetPet();
		if (pet)
		{
			if (pet->IsMount())
			{
				walkSpeed = pet->m_walkSpeed;
				runSpeed = pet->GetRunSpeed();
			}
		}
	}

	moveMsg* packet = reinterpret_cast<moveMsg*>(msg->m_buf);
	packet->type = MSG_MOVE;
	packet->subType = 0;
	packet->charType = GET_CHAR_TYPE(ch);
	packet->moveType = movetype;
	packet->charIndex = ch->m_index;

	switch ( movetype )
	{
	case MSG_MOVE_WALK:
		packet->speed = walkSpeed;
		break;
	case MSG_MOVE_RUN:
		packet->speed = runSpeed;
		break;
	case MSG_MOVE_FLY:
		packet->speed = flySpeed;
		break;
	case MSG_MOVE_PLACE:
		packet->speed = runSpeed;
		break;
	default :
		packet->speed = 0.0f;
		break;
	}

	packet->x = GET_X(ch);
	packet->z = GET_Z(ch);
	packet->h = GET_H(ch);
	packet->r = GET_R(ch);
	packet->cYlayer = GET_YLAYER(ch);
	packet->mapAttr = ch->GetMapAttr();

	msg->setSize(sizeof(moveMsg));
}

inline void makeGoMsg(CNetMsg::SP& msg, CPC* ch, int bHack = 0)
{
	goMsg* packet = reinterpret_cast<goMsg*>(msg->m_buf);
	packet->type = MSG_GOTO;
	packet->subType = 0;
	packet->zoneIndex = (ch->m_pZone) ? ch->m_pZone->m_index : -1;
	packet->x = GET_X(ch);
	packet->z = GET_Z(ch);
	packet->h = GET_H(ch);
	packet->r = GET_R(ch);
	packet->cYlayer = GET_YLAYER(ch);
	packet->hack = bHack;
	msg->setSize(sizeof(goMsg));
}
#endif
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
