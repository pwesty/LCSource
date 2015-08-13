#ifndef __PTYPE_PREMIUM_CHAR_H__
#define __PTYPE_PREMIUM_CHAR_H__

#include "ptype_base.h"
#include "ptype_char_status.h"

#define PREMIUM_CHAR_JUMP_COUNT_MAX			(15)	// 최대 15회(일)
#define PREMIUM_CHAR_ITEM_INDEX				(10799)

enum
{
	PREMIUM_CHAR_TYPE_NONE,
	PREMIUM_CHAR_TYPE_FIRST,

	PREMIUM_CHAR_TYPE_MAX,
};

// main type = MSG_PREMIUM_CHAR
// 서브 패킷 타입
enum
{
	MSG_SUB_PREMIUM_CHAR_USE_ITEM,
	MSG_SUB_PREMIUM_CHAR_JUMP_TO_CHAR,

	// only server
	MSG_SUB_PREMIUM_CHAR_RESET_JUMP_COUNT_TIME = 100,
	MSG_SUB_PREMIUM_CHAR_RESET_JUMP_COUNT,
};

// 에러 코드
enum
{
	ERR_PREMIUM_CHAR_NONE,					//
	ERR_PREMIUM_CHAR_ALREADY_USED,			// 이미 사용중
	ERR_PREMIUM_CHAR_NOT_FOUNC_CHAR,		// 점핑 대상을 찾을 수 없음
	ERR_PREMIUM_CHAR_INVALID_JUMPING_POSITION,		// 점핑할 수 없는 위치
	ERR_PREMIUM_CHAR_MAX_JUMP_COUNT,		// 점핑수를 초과함
	ERR_PREMIUM_CHAR_NOT_PREMIUM_CHAR,		// 프리미엄 캐릭터 상태가 아님
	ERR_PREMIUM_CHAR_FRIEND_IS_OFFLINE,		// 친구가 offline상태
	ERR_PREMIUM_CHAR_NOT_JUMP_STATUS,		// 점프할 수 없는 상태
};

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	struct premiumChar_UseItem : public pTypeBase 
	{
		short		tab;
		short		invenIndex;
		int			virtualIndex;
	};

	struct premiumChar_JumpToChar : public pTypeBase 
	{
		char		toCharName[MAX_CHAR_NAME_LENGTH + 1];
	};
}

namespace ResponseClient
{
	struct premiumChar_UseItem : public pTypeBase 
	{
		char		result;
	};

	struct premiumChar_JumpToChar : public pTypeBase 
	{
		char		result;
	};

#ifndef _CLIENT_
	inline void makePremiumCharUseItem(CNetMsg::SP& msg, int result)
	{
		premiumChar_UseItem* packet = reinterpret_cast<premiumChar_UseItem*>(msg->m_buf);
		packet->type = MSG_PREMIUM_CHAR;
		packet->subType = MSG_SUB_PREMIUM_CHAR_USE_ITEM;
		packet->result = result;
		msg->setSize(sizeof(premiumChar_UseItem));
	}

	inline void makePremiumCharJumpToChar(CNetMsg::SP& msg, int result)
	{
		premiumChar_JumpToChar* packet = reinterpret_cast<premiumChar_JumpToChar*>(msg->m_buf);
		packet->type = MSG_PREMIUM_CHAR;
		packet->subType = MSG_SUB_PREMIUM_CHAR_JUMP_TO_CHAR;
		packet->result = result;
		msg->setSize(sizeof(premiumChar_JumpToChar));
	}
#endif
}

namespace UpdateClient
{
	struct premiumCharInfo : public pTypeBase 
	{
		char		premiumType;
		int			expireTime;
		char		jumpCount;
	};

	struct premiumCharJumpCount : public pTypeBase
	{
		char		jumpCount;
	};

	struct premiumCharEnd : public pTypeBase 
	{
	};

#ifndef _CLIENT_
	inline void makePremiumCharInfo(CNetMsg::SP& msg, int type, int expireTime, int jumpCount)
	{
		premiumCharInfo* packet = reinterpret_cast<premiumCharInfo*>(msg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_PREMIUM_CHAR_INFO;
		packet->premiumType = type;
		packet->expireTime = expireTime;
		packet->jumpCount = jumpCount;
		msg->setSize(sizeof(premiumCharInfo));
	}

	inline void makePremiumCharJumpCount(CNetMsg::SP& msg, int jumpCount)
	{
		premiumCharJumpCount* packet = reinterpret_cast<premiumCharJumpCount*>(msg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_PREMIUM_CHAR_JUMP_COUNT;
		packet->jumpCount = jumpCount;
		msg->setSize(sizeof(premiumCharJumpCount));
	}

	inline void makePremiumCharEnd(CNetMsg::SP& msg)
	{
		premiumCharEnd* packet = reinterpret_cast<premiumCharEnd*>(msg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_PREMIUM_CHAR_END;
		msg->setSize(sizeof(premiumCharEnd));
	}
#endif
}

// only server
namespace ServerToServerPacket
{
	struct premiumCharResetJumpCountTime : public pTypeBase 
	{
		int		resetTime;
	};

	struct premiumCharResetJumpCount : public pTypeBase
	{
	};
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)


#endif
