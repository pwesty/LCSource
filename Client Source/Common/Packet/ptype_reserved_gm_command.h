#ifndef __PTYPE_RESERVED_GM_COMMDND_H__
#define __PTYPE_RESERVED_GM_COMMDND_H__

#include "ptype_base.h"

#define RESERVED_GM_COMMAND_MAX_STRING			(25)
#define RESERVED_GM_COMMAND_TITLE_MAX_STRING	(30)
#define RESERVED_GM_COMMAND_DESC_MAX_STRING		(100)

// sub type
enum
{
	MSG_SUB_RESERVED_GM_COMMAND_LIST,
	//////////////////////////////////////////////////////////////////////////
	MSG_SUB_ACTION_RESERVED_GM_COMMAND,
	MSG_SUB_ADD_RESERVED_GM_COMMAND,
	MSG_SUB_DELETE_RESERVED_GM_COMMAND,
};

enum
{
	eEVENT_TYPE_REGULAR, // 정기
	eEVENT_TYPE_FREQUENTLY, // 수시
	eEVENT_TYPE_OTHERS, // 임시

	eEVENT_TYPE_INFO = 99, // GM명령어를 포함하지 않는 단순 정보 처리
};

#pragma pack(push, 1)

struct reservedGMCommandElement
{
	int		a_Index;
	char	a_evnetType;
	int		a_startTime;
	char	a_startString[RESERVED_GM_COMMAND_MAX_STRING + 1];
	int		a_endTime;
	char	a_endString[RESERVED_GM_COMMAND_MAX_STRING + 1];
	char	a_image_type;
	short	a_image_x;
	short	a_image_y;
	char	a_title[RESERVED_GM_COMMAND_TITLE_MAX_STRING + 1];
	char	a_start_title[RESERVED_GM_COMMAND_TITLE_MAX_STRING + 1];
	char	a_end_title[RESERVED_GM_COMMAND_TITLE_MAX_STRING + 1];
	char	a_desc[RESERVED_GM_COMMAND_DESC_MAX_STRING + 1];
};

//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	// year == 0 && month == 0이면 현재 시간을 기준으로 리스트를 작성
	struct reservedGMCommandList : public pTypeBase
	{
		int m_Index;			// user_index
		unsigned short year;
		unsigned short month;
	};
}

namespace ResponseClient
{
	struct reservedGMCommandList : public pTypeBase
	{
		int m_Index;			// user_index
		int nowTime;			// unix time of today
		unsigned short year;
		unsigned short month;
		unsigned short count;
		reservedGMCommandElement ele[0];
	};
}
//////////////////////////////////////////////////////////////////////////
namespace RequestGMTool
{
	struct addReservedGMCommand : public pTypeBase
	{
		reservedGMCommandElement element;
	};

	struct deleteReservedGMCommand : public pTypeBase
	{
		int a_Index;
	};
}

namespace ResponseGMTool
{
	enum
	{
		ERR_INVALID_START_TIME = 100,			// GM TOOL에서 과거 시간으로 보낸 경우 발생
		ERR_INVALID_END_TIME,
		ERR_END_TIME_LESS_THAN_START_TIME,
		ERR_INVALID_START_PARAMETER,
		ERR_INVALID_END_PARAMETER,
		ERR_INVALID_IMAGE_TYPE,
		ERR_INVALID_EVENT_TYPE,
	};

	struct addReservedGMCommand : public pTypeBase
	{
		int result;
	};

#ifndef _CLIENT_
	inline void makeAddReservedGMCommand(CNetMsg::SP& msg, int result)
	{
		addReservedGMCommand* packet = reinterpret_cast<addReservedGMCommand*>(msg->m_buf);
		msg->m_mtype = packet->type = MSG_RESERVED_GM_COMMAND;
		packet->subType = MSG_SUB_ADD_RESERVED_GM_COMMAND;
		packet->result = result;
		msg->setSize(sizeof(addReservedGMCommand));
	}
#endif
}
//////////////////////////////////////////////////////////////////////////
namespace RequestGameServer
{
}

namespace ResposeGameServer
{
	struct addReservedGMCommand : public pTypeBase
	{
		char command[RESERVED_GM_COMMAND_MAX_STRING + 1];
	};

#ifndef _CLIENT_
	inline void makeAddReservedGMCommand(CNetMsg::SP& msg, std::string gmstr)
	{
		addReservedGMCommand* packet = reinterpret_cast<addReservedGMCommand*>(msg->m_buf);
		msg->m_mtype = packet->type = MSG_RESERVED_GM_COMMAND;
		packet->subType = MSG_SUB_ACTION_RESERVED_GM_COMMAND;
		strcpy(packet->command, gmstr.c_str());
		packet->command[RESERVED_GM_COMMAND_MAX_STRING] = '\0';
		msg->setSize(sizeof(addReservedGMCommand));
	}
#endif
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
