#ifndef __PTYPE_SERVER_TO_SERVE_H__
#define __PTYPE_SERVER_TO_SERVE_H__

#include "ptype_base.h"

enum
{
	MSG_SUB_SERVERTOSERVER_USER_LOGIN,
	MSG_SUB_SERVERTOSERVER_USER_LOGOUT,
	MSG_SUB_SERVERTOSERVER_TRADEAGENT_FEE,

	MSG_SUB_SERVERTOSERVER_CHANGE_NICK_NAME,

	MSG_SUB_SERVERTOSERVER_KICK_REQ,
	MSG_SUB_SERVERTOSERVER_KICK_ANSER,
	MSG_SUB_SERVERTOSERVER_KICK_BY_CHAR_INDEX_REQ,
	MSG_SUB_SERVERTOSERVER_KICK_BY_CHAR_INDEX_ANSWER,
	MSG_SUB_SERVERTOSERVER_KICK_BY_USER_INDEX_REQ,
	MSG_SUB_SERVERTOSERVER_KICK_BY_USER_INDEX_ANSWER,
	MSG_SUB_SERVERTOSERVER_KICK_BY_USER_ID_REQ,
	MSG_SUB_SERVERTOSERVER_KICK_BY_USER_ID_ANSER,
};

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////

namespace ServerToServerPacket
{
struct LoginUserInfo : public pTypeBase
{
	int userIndex;
	int charIndex;
	int serverNo;
	int zoneNo;
	char userId[MAX_ID_NAME_LENGTH + 1];
	char charName[MAX_CHAR_NAME_LENGTH + 1];
};

struct LogoutUserInfo : public pTypeBase
{
	int userIndex;
};

struct changeNickName : public pTypeBase
{
	char	oldNickName[MAX_CHAR_NAME_LENGTH + 1];
	char	newNickName[MAX_CHAR_NAME_LENGTH + 1];
};

//////////////////////////////////////////////////////////////////////////
inline void makeLogoutUserInfo(CNetMsg::SP& msg, int userIndex)
{
	LogoutUserInfo* packet = reinterpret_cast<LogoutUserInfo*>(msg->m_buf);

	packet->type = MSG_SERVER_TO_SERVER;
	packet->subType = MSG_SUB_SERVERTOSERVER_USER_LOGOUT;
	packet->userIndex = userIndex;

	msg->setSize(sizeof(ServerToServerPacket::LogoutUserInfo));
}

inline void makeChangeNickName(CNetMsg::SP& msg, const char* oldnickname, const char* newnickname)
{
	changeNickName* packet = reinterpret_cast<changeNickName*>(msg->m_buf);
	packet->type = MSG_SERVER_TO_SERVER;
	packet->subType = MSG_SUB_SERVERTOSERVER_CHANGE_NICK_NAME;
	memcpy(packet->oldNickName, oldnickname, sizeof(packet->oldNickName));
	packet->oldNickName[MAX_CHAR_NAME_LENGTH] = '\0';
	memcpy(packet->newNickName, newnickname, sizeof(packet->newNickName));
	packet->newNickName[MAX_CHAR_NAME_LENGTH] = '\0';
	msg->setSize(sizeof(changeNickName));
}
//////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
