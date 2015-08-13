#ifndef __PTYPE_SYNDICATE_H__
#define __PTYPE_SYNDICATE_H__

#include "ptype_base.h"

//Packet SubType Client <-> Server
enum
{
	MSG_RVR_REMOVE_REQ,						//탈퇴 요청
	MSG_RVR_GIVE_JEWEL_REQ,					//보석 기부
	MSG_RVR_SYNDICATE_INFO_USER_REQ,		//개인 결사대 정보 요청
	MSG_RVR_SYNDICATE_INFO_SERVER_REQ,		//결사대 정보 요청

	MSG_RVR_ERROR_REP,						//응답 에러코드
	MSG_RVR_SYNDICATE_INFO_USER_REP,		//결사대 유저 정보 응답

	MSG_RVR_SYNDICATE_INFO_SERVER,			//결사대 정보 응답
	MSG_UPDATE_RVR_SYNDICATE_TYPE,			//결사대 정보 업데이트
	MSG_UPDATE_RVR_SYNDICATE_GRADE,			//결사대 등급 업데이트
	MSG_UPDATE_RVR_CONTRIBUTIONS_POINT,		//결사대 포인트 업데이트
	MSG_UPDATE_SYNDICATE_HISTORY_LOG,		//결사대 히스토리 로그 업데이트
	MSG_UPDATE_SYNDICATE_PERCENTAGE,		//결사대 점수 업데이트
	MSG_UPDATE_KINGNAME,					//왕 정보 업데이트

	SYSMSG_INCREASE_POINT,					//포인트 증가 시스템 메시지

	MSG_UPDATE_OUT_NEEDNAS,					//결사대 탈퇴시 필요한 나스정보
};

enum
{
	JOIN,			// 가입
	REMOVE,			// 탈퇴
	DEGRADE,		// 강등
	PROMOTION,		// 승급
	WIN,			// 승리
	LOSE,			// 지다
};

#pragma pack(push, 1)

namespace RequestClient
{
//탈퇴 요청
struct RVRRemoveRequest : public pTypeBase
{
	int	tab;
	int invenIndex;
	int npcIndex;		// 탈퇴 NPC
};

//창조의 보석 기부요청
struct RVRGiveJewel : public pTypeBase
{
	int				npcIndex;				// 창조의 보석 give NPC
	unsigned int	vIndexByCreateJewel;	// 창조의 보석 v-index
};

struct RVRServerInfoReq : public pTypeBase
{
	int				syndicateType;			// 서버간 통신에 사용(클라이언트는 보낼 필요 없음)
	int				grade;					// 서버간 통신에 사용(클라이언트는 보낼 필요 없음)
};

#ifndef _CLIENT_
inline void makeReqSyndicateServerInfoMsg(CNetMsg::SP& msg, int syndicateType, int grade)
{
	RequestClient::RVRServerInfoReq* packet = reinterpret_cast<RequestClient::RVRServerInfoReq*>(msg->m_buf);
	packet->type = MSG_HELPER_COMMAND_STRUCT;
	packet->subType = MSG_HELPER_RVR_RVR_INFO;
	packet->syndicateType = syndicateType;
	packet->grade = grade;
	msg->setSize(sizeof(RequestClient::RVRServerInfoReq));
}
#endif
}

namespace ResponseClient
{
enum
{
	ERROR_SYNDICATE_JOIN_SUCCESS,					// 결사대 가입 성공
	ERROR_SYNDICATE_REMOVE_SUCCESS,					// 결사대 탈퇴 성공
	ERROR_SYNICATE_GIVE_SUCCESS,					// 결사대 창조의 보석 기부 성공
	ERROR_SYNDICATE_REMOVE_FAIL_NOT_ENOUGH_MONEY,	// 탈퇴하기 위한 돈이 부족
	ERROR_SYNDICATE_REMOVE_FAIL_ALREADY_EQUIP,		// 결사대 전용 장비를 착용 중이다.
	ERROR_SYNDICATE_BUSY,							// 결사대 정보가 혼잡합니다.
};

//유저 결사대 정보
struct RVRSyndicateInfoOfUser : public pTypeBase
{
	int			syndicateType;								//결사대 가입 타입
	int			grade;										//등급
	LONGLONG	accumulateContributionPoint;				//pc의 누적 기여도
	LONGLONG	maxPoint;									//등급 별 최대 수치
};

//error response
struct SyndicateError  : public pTypeBase
{
	int result;
	int syndicateType;
};

#ifndef _CLIENT_
inline void makeSyndicateErrorMsg(CNetMsg::SP& msg, int errorCode, int syndicateType)
{
	ResponseClient::SyndicateError* packet = reinterpret_cast<ResponseClient::SyndicateError*>(msg->m_buf);
	packet->type			= MSG_RVR;
	packet->subType			= MSG_RVR_ERROR_REP;
	packet->result			= errorCode;
	packet->syndicateType	= syndicateType;
	msg->setSize(sizeof(ResponseClient::SyndicateError));
}
#endif
}

namespace UpdateClient
{
struct SyndicateContributePoint : public pTypeBase
{
	int syndicateType;
	bool isFirstJoin;
	LONGLONG accumulateContributionPoint;	// pc의 누적 기여도
};

struct SyndicateKind : public pTypeBase
{
	int charIndex;
	int syndicateType;
	int grade;
};

struct SyndicateHistoryData : public pTypeBase
{
	struct historys
	{
		int syndicateType;
		int time;
		int grade;
		int type;
		int targetSyndicateType;
		char targetName[MAX_CHAR_NAME_LENGTH + 1];
	};

	int count;
	historys	data[0];
};

struct SyndicatePercentageData : public pTypeBase
{
	int syndicateType;
	float percentage;
};

struct RVRSyndicateInfoOfServer : public pTypeBase
{
	int syndicateType;
	char kingName[MAX_CHAR_NAME_LENGTH + 1];			//대빵 이름
	int skillIndex;									//받을 수 있는 버프 인덱스
	int skillLevel;									//받을 수 있는 버프 레벨
};

struct SyndicateKingNameUpdate : public pTypeBase
{
	int syndicateType;
	char kingName[MAX_CHAR_NAME_LENGTH + 1];
};

struct SyndicateIncreasePoint : public pTypeBase
{
	int syndicateType;
	LONGLONG point;
};

struct SyndicateToNeedNasOfOut : public pTypeBase
{
	GoldType_t needNas;
};

#ifndef _CLIENT_
inline void makeUpdateSyndicateContributePoint(CNetMsg::SP& msg, int syndicateType, bool isFirstJoin, LONGLONG point)
{
	UpdateClient::SyndicateContributePoint* packet = reinterpret_cast<UpdateClient::SyndicateContributePoint*>(msg->m_buf);
	packet->type			= MSG_RVR;
	packet->subType			= MSG_UPDATE_RVR_CONTRIBUTIONS_POINT;
	packet->syndicateType	= syndicateType;
	packet->isFirstJoin		= isFirstJoin;
	packet->accumulateContributionPoint = point;
	msg->setSize(sizeof(UpdateClient::SyndicateContributePoint));
}

inline void makeUpdateSyndicateKind(CNetMsg::SP& msg, int syndicateType, int grade, int charIndex)
{
	UpdateClient::SyndicateKind* packet = reinterpret_cast<UpdateClient::SyndicateKind*>(msg->m_buf);
	packet->type			= MSG_RVR;
	packet->subType			= MSG_UPDATE_RVR_SYNDICATE_GRADE;
	packet->charIndex		= charIndex;
	packet->syndicateType	= syndicateType;
	packet->grade			= grade;
	msg->setSize(sizeof(UpdateClient::SyndicateKind));
}

inline void makeUpdateSyndicateIncreaseSysMsg(CNetMsg::SP& msg, int syndicateType, LONGLONG point)
{
	UpdateClient::SyndicateIncreasePoint* packet = reinterpret_cast<UpdateClient::SyndicateIncreasePoint*>(msg->m_buf);
	packet->type	      = MSG_RVR;
	packet->subType		  = SYSMSG_INCREASE_POINT;
	packet->syndicateType = syndicateType;
	packet->point         = point;
	msg->setSize(sizeof(UpdateClient::SyndicateIncreasePoint));
}

inline void makeUpdateSyndicateOutNeedNas(CNetMsg::SP& msg, GoldType_t nas)
{
	UpdateClient::SyndicateToNeedNasOfOut* packet = reinterpret_cast<UpdateClient::SyndicateToNeedNasOfOut*>(msg->m_buf);
	packet->type    = MSG_RVR;
	packet->subType = MSG_UPDATE_OUT_NEEDNAS;
	packet->needNas = nas;
	msg->setSize(sizeof(UpdateClient::SyndicateToNeedNasOfOut));
}
#endif
}

#ifndef _CLIENT_
namespace UpdateServer
{
struct SyndicateUpdateServerInfo : public pTypeServer
{
	LONGLONG	first;
	LONGLONG	second;
};

struct SyndicateJewelCount : public pTypeServer
{
	int syndicateType;
	int count;
};

struct SyndicateUser : public pTypeServer
{
	int syndicateType;
};

struct SyndicateInfo : public pTypeServer
{
	int	jewelCount_k;
	int jewelCount_d;
	int userCount_k;
	int userCount_d;
};

struct SyndicateUpdateKingInfoAll : public pTypeServer
{
	char kingName_d[MAX_CHAR_NAME_LENGTH + 1];
	int kingIndex_d;
	LONGLONG	kingPoint_d;
	char kingName_k[MAX_CHAR_NAME_LENGTH + 1];
	int kingIndex_k;
	LONGLONG kingPoint_k;
};

struct SyndicateUpdateKingInfoEach : public pTypeServer
{
	int syndicateType;
	int kingIndex;
	LONGLONG kingPoint;
	char kingName[MAX_CHAR_NAME_LENGTH + 1];
};

struct SyndicateAdd2ndGradeUser : public pTypeServer
{
	int syndicateType;
	int charIndex;
	LONGLONG point;
	char userName[MAX_CHAR_NAME_LENGTH + 1];
};

struct SyndicateDel2ndGradeUser : public pTypeServer
{
	int syndicateType;
	int charIndex;
};

struct SyndicateUpdate2ndGradeUser : public pTypeServer
{
	int syndicateType;
	int charIndex;
	LONGLONG point;
};

struct SyndicateKingDownGrade : public pTypeServer
{
	int syndicateType;
	int charIndex;
	LONGLONG point;
};

struct SyndicateUserCount : public pTypeServer
{
	int syndicateType;
	int userCount;
};

inline void HelperServerRVRAdd2ndUser(CNetMsg::SP& msg, int syndicateType, int charIndex, LONGLONG point, const char* name)
{
	UpdateServer::SyndicateAdd2ndGradeUser* packet = reinterpret_cast<UpdateServer::SyndicateAdd2ndGradeUser*>(msg->m_buf);
	packet->type = MSG_HELPER_COMMAND_STRUCT;
	packet->subType = MSG_HELPER_ADD_2ND_USER;
	packet->syndicateType = syndicateType;
	packet->charIndex = charIndex;
	packet->point = point;
	strncpy(packet->userName, name, MAX_CHAR_NAME_LENGTH);
	packet->userName[MAX_CHAR_NAME_LENGTH] = '\0';
	msg->setSize(sizeof(UpdateServer::SyndicateAdd2ndGradeUser));
}

inline void HelperServerRVRDel2ndUser(CNetMsg::SP& msg, int syndicateType, int charIndex)
{
	UpdateServer::SyndicateDel2ndGradeUser* packet = reinterpret_cast<UpdateServer::SyndicateDel2ndGradeUser*>(msg->m_buf);
	packet->type = MSG_HELPER_COMMAND_STRUCT;
	packet->subType = MSG_HELPER_DEL_2ND_USER;
	packet->syndicateType = syndicateType;
	packet->charIndex = charIndex;
	msg->setSize(sizeof(UpdateServer::SyndicateDel2ndGradeUser));
}

inline void HelperServerRVRUpdate2ndUser(CNetMsg::SP& msg, int syndicateType, int charIndex, LONGLONG point)
{
	UpdateServer::SyndicateUpdate2ndGradeUser* packet = reinterpret_cast<UpdateServer::SyndicateUpdate2ndGradeUser*>(msg->m_buf);
	packet->type = MSG_HELPER_COMMAND_STRUCT;
	packet->subType = MSG_HELPER_UPDATE_2ND_USER;
	packet->syndicateType = syndicateType;
	packet->charIndex = charIndex;
	packet->point = point;
	msg->setSize(sizeof(UpdateServer::SyndicateUpdate2ndGradeUser));
}

inline void HelperServerRVRKingDownGrade(CNetMsg::SP& msg, int syndicateType, int charIndex, LONGLONG point)
{
	UpdateServer::SyndicateKingDownGrade* packet = reinterpret_cast<UpdateServer::SyndicateKingDownGrade*>(msg->m_buf);
	packet->type = MSG_HELPER_COMMAND_STRUCT;
	packet->subType = MSG_HELPER_DOWN_GRADE;
	packet->syndicateType = syndicateType;
	packet->charIndex = charIndex;
	packet->point = point;
	msg->setSize(sizeof(UpdateServer::SyndicateKingDownGrade));
}

inline void HelperServerRVRIncreaseCreateJewel(CNetMsg::SP& msg, int syndicateType, int count)
{
	UpdateServer::SyndicateJewelCount* packet = reinterpret_cast<UpdateServer::SyndicateJewelCount*>(msg->m_buf);
	packet->type = MSG_HELPER_COMMAND_STRUCT;
	packet->subType = MSG_HELPER_RVR_INCREASE_JEWEL_REQ;
	packet->syndicateType = syndicateType;
	packet->count = count;
	msg->setSize(sizeof(UpdateServer::SyndicateJewelCount));
}

inline void HelperServerRVRDecreaseCreateJewel(CNetMsg::SP& msg, int syndicateType, int count)
{
	UpdateServer::SyndicateJewelCount* packet = reinterpret_cast<UpdateServer::SyndicateJewelCount*>(msg->m_buf);
	packet->type = MSG_HELPER_COMMAND_STRUCT;
	packet->subType = MSG_HELPER_RVR_DECREASE_JEWEL_REQ;
	packet->syndicateType = syndicateType;
	packet->count = count;
	msg->setSize(sizeof(UpdateServer::SyndicateJewelCount));
}

inline void HelperServerRVRJoinRequest(CNetMsg::SP& msg, int syndicateType)
{
	UpdateServer::SyndicateUser* packet = reinterpret_cast<UpdateServer::SyndicateUser*>(msg->m_buf);
	packet->type = MSG_HELPER_COMMAND_STRUCT;
	packet->subType = MSG_HELPER_RVR_JOIN_REQ;
	packet->syndicateType = syndicateType;
	msg->setSize(sizeof(UpdateServer::SyndicateUser));
}

inline void HelperServerRVRLeaveRequest(CNetMsg::SP& msg, int syndicateType)
{
	UpdateServer::SyndicateUser* packet = reinterpret_cast<UpdateServer::SyndicateUser*>(msg->m_buf);
	packet->type = MSG_HELPER_COMMAND_STRUCT;
	packet->subType = MSG_HELPER_RVR_LEAVE_REQ;
	packet->syndicateType = syndicateType;
	msg->setSize(sizeof(UpdateServer::SyndicateUser));
}

inline void HelperServerRVRChangeKing(CNetMsg::SP& msg, int syndicateType, const char* kingName, int kingIndex, LONGLONG kingPoint)
{
	UpdateServer::SyndicateUpdateKingInfoEach* packet = reinterpret_cast<UpdateServer::SyndicateUpdateKingInfoEach*>(msg->m_buf);
	packet->type = MSG_HELPER_COMMAND_STRUCT;
	packet->subType = MSG_HELPER_KING_INFO_UPDATE_EACH;
	packet->syndicateType = syndicateType;
	packet->kingIndex = kingIndex;
	packet->kingPoint = kingPoint;
	strncpy(packet->kingName, kingName, MAX_CHAR_NAME_LENGTH);
	packet->kingName[MAX_CHAR_NAME_LENGTH] = '\0';
	msg->setSize(sizeof(UpdateServer::SyndicateUpdateKingInfoEach));
}

inline void HelperServerRvRSetUserCount(CNetMsg::SP& msg, int syndicateType, int count)
{
	UpdateServer::SyndicateUserCount* packet = reinterpret_cast<UpdateServer::SyndicateUserCount*>(msg->m_buf);
	packet->type = MSG_HELPER_COMMAND_STRUCT;
	packet->subType = MSG_HELPER_SET_USER_COUNT;
	packet->syndicateType = syndicateType;
	packet->userCount = count;
	msg->setSize(sizeof(UpdateServer::SyndicateUserCount));
}
}
#endif
#pragma pack(pop)
#endif // __PTYPE_SYNDICATE__
