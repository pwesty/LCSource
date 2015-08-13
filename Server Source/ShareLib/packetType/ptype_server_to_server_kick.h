#ifndef __PTYPE_SERVER_TO_SERVE_KICK_H__
#define __PTYPE_SERVER_TO_SERVE_KICK_H__

#include "ptype_server_to_server.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace ServerToServerPacket
{
struct kickUser : public pTypeBase
{
	int		req_charIndex;
	char	kicked_charName[MAX_ID_NAME_LENGTH + 1];
	bool	result;		// false - not found user, true - kick ok
};

struct kickUserByCharIndex : public pTypeBase
{
	int		req_charIndex;
	int		kicked_charIndex;
	bool	result;
};

struct kickUserByUserIndex : public pTypeBase
{
	int		req_charIndex;
	int		kicked_userIndex;
	bool	result;
};

struct kickUserByUserId : public pTypeBase
{
	int		req_charIndex;
	char	kicked_userId[MAX_ID_NAME_LENGTH + 1];
	bool	result;
};

inline void makeKickUser(CNetMsg::SP& msg, int req_charIndex, const char* kicked_charName)
{
	kickUser* packet = reinterpret_cast<kickUser*>(msg->m_buf);
	packet->type = MSG_SERVER_TO_SERVER;
	packet->subType = MSG_SUB_SERVERTOSERVER_KICK_REQ;
	packet->req_charIndex = req_charIndex;
	memcpy(packet->kicked_charName, kicked_charName, sizeof(packet->kicked_charName));
	packet->kicked_charName[MAX_ID_NAME_LENGTH] = '\0';
	msg->setSize(sizeof(kickUser));
}

inline void makeKickUserByCharIndex(CNetMsg::SP& msg, int req_charIndex, int kicked_charIndex)
{
	kickUserByCharIndex* packet = reinterpret_cast<kickUserByCharIndex*>(msg->m_buf);
	packet->type = MSG_SERVER_TO_SERVER;
	packet->subType = MSG_SUB_SERVERTOSERVER_KICK_BY_CHAR_INDEX_REQ;
	packet->req_charIndex = req_charIndex;
	packet->kicked_charIndex = kicked_charIndex;
	msg->setSize(sizeof(kickUserByCharIndex));
}

inline void makeKickUserByUserIndex(CNetMsg::SP& msg, int req_charIndex, int kicked_userIndex)
{
	kickUserByUserIndex* packet = reinterpret_cast<kickUserByUserIndex*>(msg->m_buf);
	packet->type = MSG_SERVER_TO_SERVER;
	packet->subType = MSG_SUB_SERVERTOSERVER_KICK_BY_USER_INDEX_REQ;
	packet->req_charIndex = req_charIndex;
	packet->kicked_userIndex = kicked_userIndex;
	msg->setSize(sizeof(kickUserByUserIndex));
}

inline void makeKickUserByUserId(CNetMsg::SP& msg, int req_charIndex, const char* kicked_userId)
{
	kickUserByUserId* packet = reinterpret_cast<kickUserByUserId*>(msg->m_buf);
	packet->type = MSG_SERVER_TO_SERVER;
	packet->subType = MSG_SUB_SERVERTOSERVER_KICK_BY_USER_ID_REQ;
	packet->req_charIndex = req_charIndex;
	memcpy(packet->kicked_userId, kicked_userId, sizeof(packet->kicked_userId));
	packet->kicked_userId[MAX_ID_NAME_LENGTH] = '\0';
	msg->setSize(sizeof(kickUserByUserId));
}
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
