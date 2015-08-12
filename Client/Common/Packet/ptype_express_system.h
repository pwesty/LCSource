#ifndef __PTYPE_EXPRESS_H__
#define __PTYPE_EXPRESS_H__

#include "ptype_base.h"

#define EXPRESS_SYSTEM_COUNT_PER_PAGE		(10)
#define EXPRESS_SYSTEM_EXPIRE_DATE			(15)

#define EXPRESS_SENDER_NAME					(16)		// 클라이언트 기준으로 맞춤

#ifdef WIN32
typedef unsigned int expressIndex_t;
#else
typedef unsigned int expressIndex_t;
#endif

// 창고NPC (Express system)
enum
{
	MSG_SUB_EXPRESS_EXIST,							// 창고 NPC에 아이템(또는 돈)이 있는지 0 - 없음, 1 - 있음
	MSG_SUB_EXPREES_LIST,							// 창고 NPC에서 리스트 요청 처리
	MSG_SUB_EXPRESS_TAKE,							// 창고 NPC에서 아이템(또는 돈)을 수령
	MSG_SUB_EXPRESS_DELETE,							// 창고 NPC에서 아이템(또는 돈)을 삭제

	MSG_SUB_AUTHORIZE = 100,						// 운영툴 인증 절차
	MSG_SUB_EXPRESS_ALL_USER,						// 접속한 모든 유저에게 아이템(또는 돈)지금 - Game Server에서 처리
	MSG_SUB_EXPRESS_SERVER_GROUP,					// 특정 서버 그룹 - Game Server에서 처리
	MSG_SUB_EXPRESS_ZONE,							// 특정 서버 그룹 / 특정 존 - Game Server에서 처리
	MSG_SUB_EXPRESS_ONE_USER,						// 특정 유저 - SubHelper에서 처리

	MSG_SUB_EXPRESS_SEARCH_NICKNAME,				// 수령인 이름 검색
	MSG_SUB_EXPRESS_SEND_ITEM_TO_NICKNAME,			// 수령인의 Express로 아이템 전달
};

enum
{
	EXPRESS_SEND_TYPE_NONE,			// 데이터가 없는 경우
	EXPRESS_SEND_TYPE_LOGIN,		// 로그인시 자동으로 보내는 경우
	EXPRESS_SEND_TYPE_GM,			// 운영툴
	EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_BUY,		// 판매 대행 시스템 (구매완료)
	EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_SELL,		// 판매 대행 시스템 (판매완료)
	EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_CANCEL,		// 판매 대행 시스템 (판매취소)
	EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_RETURN,	// 판매 대행 시스템 (반송)
	EXPRESS_SEND_TYPE_ETC,			// 기타
	EXPRESS_SEND_UNSEARCHABLE_STONESTATUS,		// 신비한 석상의 구슬
};

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
struct ExpressSystemItemInfo
{
	int				index;			// 고유 번호
	int				item_index;
	unsigned char	plus;			// 강화 수치
	unsigned char	plus2;
	int				flag;
	char			serial[MAX_SERIAL_LENGTH + 1];
	int				item_count;
	int				option_count;
	int				option_type[MAX_ITEM_OPTION];
	int				option_level[MAX_ITEM_OPTION];
	int				socket[MAX_SOCKET_COUNT];
	int				item_origin[MAX_ORIGIN_OPTION];
	LONGLONG		nas;
	int				registe_date;
	int				expire_date;
	short			send_type;
	char			sender[EXPRESS_SENDER_NAME + 1];

#ifdef DURABILITY
	int				now_durability;
	int				max_durability;
#endif

	LONGLONG		tradeagent_nas;				//default = -1
	int				tradeagent_itemIndex;		//default = -1
	int				tradeagent_itemCount;		//default = -1

	ExpressSystemItemInfo()
	{
		memset(this, 0x00, sizeof(*this));
		memset(this->socket, -1, sizeof(this->socket));

#ifdef DURABILITY
		now_durability = 0;
		max_durability = 0;
#endif
	}
};

namespace RequestClient
{
struct expressList : public pTypeBase
{
	unsigned short	npcIndex;
	unsigned short	pageIndex;
};

struct expressTake : public pTypeBase
{
	unsigned short	npcIndex;
	expressIndex_t	index;
};

struct expressDelete : public pTypeBase
{
	unsigned short	npcIndex;
	expressIndex_t	index;
};

// 신비한 석상의 구슬
struct express_unsearchable_stonestatue_searchName : public pTypeBase
{
	char nickName[MAX_CHAR_NAME_LENGTH + 1];
};

struct express_use_unsearchable_stonestatue : public pTypeBase
{
	unsigned short		tab;			// 신비한 석상의 구슬
	unsigned short		invenIndex;
	int					virualIndex;
	unsigned short		send_tab;		// 보낼 아이템
	unsigned short		send_invenIndex;
	int					send_virutalIndex;
	int					itemCount;
	char toNickName[MAX_CHAR_NAME_LENGTH + 1]; // 받는 사람
};
}

namespace ResponseClient
{
enum
{
	ERR_NO_ERROR,				// 정상
	ERR_DB_ERROR,				// 디비 에러
	ERR_NO_TAKE_ITEM,			// 찾을 물건이 없음
	ERR_CAN_NOT_CREATE_ITEM,	// 아이템을 생성할 수 업음
	ERR_INVALID_INFORMATION,	// 아이템(또는 돈)정보가 잘못된 정보
	ERR_FULL_INVENTORY,			// 가방이 꽉참
	ERR_NOT_FOUND_NPC,			// NPC를 찾을 수 없거나, 거리가 너무 멉니다.

	ERR_NOT_SEND_ITEM,			// 전달할수 없는 아이템
	ERR_NOT_ENOUGHT_COUNT,		// 전달할 아이템의 갯수가 모자름
	ERR_NOT_FOUND_NICKNAME_SYS_ERR,	// 전달받는 nick이 없어, 자신의 Express로 다시 넣어줌
};

struct ExpressExist : public pTypeBase
{
	unsigned char	flag;	// 0 - 없음, 1 - 있음
};

struct expressList : public pTypeBase
{
	unsigned short	nowPage;
	unsigned char	nextPage;			// 0 - 없음, 1 - 있음
	unsigned char	count;
	ExpressSystemItemInfo	list[0];
};

struct expressTake : public pTypeBase
{
	unsigned char	result;				// 0 - OK, 1 >= - ERROR
};

struct expressDelete : public pTypeBase
{
	unsigned char	result;				// 0 - OK, 1 >= - ERROR
};

// 신비한 석상의 구슬
struct express_unsearchable_stonestatus_searchName : public pTypeBase
{
	unsigned char	result;				// 0 - found, 1 - not found
};

struct express_use_unsearchable_stonestatus : public pTypeBase
{
	unsigned char	result;				// 0 - OK, 1 >= - ERROR
};

//////////////////////////////////////////////////////////////////////////
#ifndef _CLIENT_	// 클라이어트를 위한 처리
inline void makeExpressExist(CNetMsg::SP& msg, int flag)
{
	ExpressExist* packet = reinterpret_cast<ExpressExist*>(msg->m_buf);
	msg->m_mtype = packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPRESS_EXIST;
	packet->flag = flag;
	msg->setSize(sizeof(ExpressExist));
}

inline void makeExpressTake(CNetMsg::SP& msg, int result)
{
	expressTake* packet = reinterpret_cast<expressTake*>(msg->m_buf);
	msg->m_mtype = packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPRESS_TAKE;
	packet->result = result;
	msg->setSize(sizeof(expressTake));
}

inline void makeExpressDelete(CNetMsg::SP& msg, int result)
{
	expressTake* packet = reinterpret_cast<expressTake*>(msg->m_buf);
	msg->m_mtype = packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPRESS_DELETE;
	packet->result = result;
	msg->setSize(sizeof(expressTake));
}

inline void makeExpressSearchNickname(CNetMsg::SP& msg, int result)
{
	express_unsearchable_stonestatus_searchName* packet = reinterpret_cast<express_unsearchable_stonestatus_searchName*>(msg->m_buf);
	packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPRESS_SEARCH_NICKNAME;
	packet->result = result;
	msg->setSize(sizeof(express_unsearchable_stonestatus_searchName));
}

inline void makeExpressSendToNickName(CNetMsg::SP& msg, int result)
{
	express_use_unsearchable_stonestatus* packet = reinterpret_cast<express_use_unsearchable_stonestatus*>(msg->m_buf);
	packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPRESS_SEND_ITEM_TO_NICKNAME;
	packet->result = result;
	msg->setSize(sizeof(express_use_unsearchable_stonestatus));
}

#endif
}

//////////////////////////////////////////////////////////////////////////
// Messenger server <---> GM TOOL
namespace RequestGMTool
{
struct authorize : public pTypeBase
{
};

/*
	server_no == -1 && zone_no == -1 : 모든 서버
	server_no != -1 && zone_no == -1 : 특정 서버군
	server_no == -1 && zone_no != -1 : 모든 서버군의 특정 존
*/
struct sendItemToPlayingUser : public pTypeBase
{
	int			server_no;
	int			zone_no;
	int			item_index;
	int			item_count;
	int			nas;
	int			minLevel;
	int			maxLevel;
	char		sender[EXPRESS_SENDER_NAME + 1];
};

struct sendItemToOneUser : public pTypeBase
{
	int			server_no;
	int			char_index;
	int			item_index;
	int			item_count;
	int			nas;
	short		send_type;
	char		sender[EXPRESS_SENDER_NAME + 1];
};
}

namespace ResponseGMTool
{
enum
{
	ERR_NO_ERROR,				// OK
	ERR_ALREADY_CONNECT,
	ERR_INVALID_SERVER_NO,
	ERR_INVALID_ZONE_NO,
	ERR_SENDER_IS_EMPTY,		// 보낸이가 없음
	ERR_INVALID_ITEM_INDEX,
	ERR_INVALID_ITEM_COUNT,
	ERR_INVALID_NAS,
	ERR_INVALID_LEVEL,
};

struct authorize : public pTypeBase
{
	int			result;
};

struct sendItemToPlayingUser : public pTypeBase
{
	int			result;
};

struct sendItemToOneUser : public pTypeBase
{
	int			result;
};

//////////////////////////////////////////////////////////////////////////
#ifndef _CLIENT_
inline void makeAuthorize(CNetMsg::SP& msg, int result)
{
	authorize* packet = reinterpret_cast<authorize*>(msg->m_buf);
	msg->m_mtype = packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_AUTHORIZE;
	packet->result = result;
	msg->setSize(sizeof(authorize));
}

inline void makeSendToAllUser(CNetMsg::SP& msg, int result)
{
	sendItemToPlayingUser* packet = reinterpret_cast<sendItemToPlayingUser*>(msg->m_buf);
	msg->m_mtype = packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPRESS_ALL_USER;
	packet->result = result;
	msg->setSize(sizeof(sendItemToPlayingUser));
}

inline void makeSendToServerGroup(CNetMsg::SP& msg, int result)
{
	sendItemToPlayingUser* packet = reinterpret_cast<sendItemToPlayingUser*>(msg->m_buf);
	msg->m_mtype = packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPRESS_SERVER_GROUP;
	packet->result = result;
	msg->setSize(sizeof(sendItemToPlayingUser));
}

inline void makeSendToZone(CNetMsg::SP& msg, int result)
{
	sendItemToPlayingUser* packet = reinterpret_cast<sendItemToPlayingUser*>(msg->m_buf);
	msg->m_mtype = packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPRESS_ZONE;
	packet->result = result;
	msg->setSize(sizeof(sendItemToPlayingUser));
}

inline void makeSendToOneUser(CNetMsg::SP& msg, int result)
{
	sendItemToOneUser* packet = reinterpret_cast<sendItemToOneUser*>(msg->m_buf);
	msg->m_mtype = packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPRESS_ONE_USER;
	packet->result = result;
	msg->setSize(sizeof(sendItemToOneUser));
}
#endif
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
