#ifndef __PTYPE_PET_STASH__
#define __PTYPE_PET_STASH__

#include "ptype_base.h"

#define MAX_PET_STASH_KEEP_COUNT	20
#define MAX_PET_PROXY_COUNT			1

#define PET							0
#define APET						1

//Packet SubType Client <-> Server
enum
{
	MSG_PET_STASH_LIST_UPDATE,
	MSG_PET_STASH_TAKE,
	MSG_PET_STASH_TAKE_UPDATE,
	MSG_PET_STASH_KEEP,
	MSG_PET_STASH_KEEP_UPDATE,
	MSG_PET_STASH_PROXY,
	MSG_PET_STASH_PROXY_UPDATE,
	MSG_PET_STASH_PROXY_CANCEL,
	MSG_PET_STASH_PROXY_CANCEL_UPDATE,
	MSG_PET_STASH_EFFECT,
	MSG_PET_STASH_EFFECT_BY_INMAP,
	MSG_PET_STASH_EFFECT_BROAD_CAST,
	MSG_PET_STASH_PET_UPDATE,
	MSG_PET_STASH_REQUEST_EFFECT_BROADCAST,
	MSG_PET_STASH_ERROR,
};

#pragma pack(push, 1)

namespace RequestClient
{
struct PetStashTake : public pTypeBase
{
	int charIndex;
	int petItemVIndex;								//펫 아이템 가상 인덱스
};

struct PetStashKeep : public pTypeBase
{
	int charIndex;
	int petItemVIndex;
};

struct PetStashProxyReg : public pTypeBase
{
	int petItemVIndex;
};

struct PetStashProxyCancel : public pTypeBase
{
	int petItemVIndex;
};

struct PetStashRequestEffectBroadcast : public pTypeBase
{
	int effectNo;
};
}


namespace ResponseClient
{
enum
{
	ERROR_PET_STASH_KEEP_SUCCESS,					//맡기기 성공
	ERROR_PET_STASH_TAKE_SUCCESS,					//찾기 성공
	ERROR_PET_STASH_PROXY_SUCCESS,					//대리육성 등록 성공
	ERROR_PET_STASH_PROXY_CANCEL_SUCCESS,			//대리육성 등록 취소 성공

	ERROR_PET_STASH_KEEP_FAIL_NOT_EXIST_PET,		//펫이 존재하지 않음
	ERROR_PET_STASH_KEEP_FAIL_TERMINATION_ITEM,		//펫 등록 아이템 사용기간 만료
	ERROR_PET_STASH_KEEP_FAIL_ALREADY_EQ_PET,		//펫을 착용중이다.
	ERROR_PET_STASH_KEEP_FAIL_PET_SEAL,				//펫이 봉인상태이다.
	ERROR_PET_STASH_KEEP_FAIL_FULL,					//펫 창고가 가득 참.

	ERROR_PET_STASH_TAKE_FAIL_NOT_EXIST_PET,		//펫이 존재하지 않음
	ERROR_PET_STASH_TAKE_FAIL,						//펫 찾기 실패

	ERROR_PET_STASH_PROXY_CANCEL_NOT_EXIST,			//대리육성으로 등록된 펫이 아니다.

	ERROR_PET_STASH_ERROR_NOT_USE_SYSTEM,			//펫 창고 시스템을 사용할 수 없습니다.

	ERROR_PET_STASH_FAIL_ETC,						//알수 없는 오류
};

struct PetStashTake : public pTypeBase
{
	int result;
};

struct PetStashKeep : public pTypeBase
{
	int result;
};

struct PetStashProxyReg : public pTypeBase
{
	int result;
};

struct PetStashProxyCancel : public pTypeBase
{
	int result;
};

struct PetStashError : public pTypeBase
{
	int errorCode;
};
}

namespace UpdateClient
{
struct PetStashData
{
	int			virtualIndex;		// 서버가 발행한 가상 index
	int			dbIndex;			// DB상의 Index
	int			isPetOrApet;		// 0 : 펫1, 1 : 펫2
	int			plus;
	int			petLevel;
	int			petHungryMax;
	int			petHungry;
	LONGLONG	petLevelupExp;
	LONGLONG	petExp;
	int			petFaithMax;
	int			petFaith;
	//펫만 사용
	int			hp;
	int			maxHp;
	int			abilityPoint;
	char		grade;
	//공격펫만 사용
	int			petStr;
	int			petInt;
	int			petDex;
	int			petCon;
	LONGLONG	petAccExp;
	int			remainTime;
	int			petPlusStr;
	int			petPlusInt;
	int			petPlusDex;
	int			petPlusCon;
	//
	char		petName[MAX_CHAR_NAME_LENGTH];
};

struct PetStashList : public pTypeBase
{
	int			proxyPetItemVIndex;	//대리육성 등록 펫 아이템 가상 인덱스
	short		count;
	PetStashData petStashData[0];
};

struct PetStashTake : public pTypeBase
{
	int petItemVIndex;								//펫 아이템 가상 인덱스
};

struct PetStashKeep : public pTypeBase
{
	PetStashData petStashData;
};

struct PetStashPetData : public pTypeBase
{
	PetStashData petStashData;
};

struct PetStashProxyReg : public pTypeBase
{
	int virtualIndex;
};

struct PetStashProxyCancel : public pTypeBase
{
	int virtualIndex;
};

struct PetStashEffect : public pTypeBase // 클라가 아이템을 썼을때
{
	int effectNo;
};

struct PetStashEffectByInMap : public pTypeBase // 인맵 or 펫창고 기간 만료일때
{
	int effectNo;
};

struct PetStashEffectBr : public pTypeBase
{
	int charIndex;
	int effectNo;
};
}

#pragma pack(pop)

#ifndef _CLIENT_

inline void makePetStashErrorMsg(CNetMsg::SP &msg, int errorCode)
{
	ResponseClient::PetStashError *packet = reinterpret_cast<ResponseClient::PetStashError*>(msg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_ERROR;
	packet->errorCode = errorCode;
	msg->setSize(sizeof(ResponseClient::PetStashError));
}

inline void makePetStashTakeMsg(CNetMsg::SP& msg, int errorCode)
{
	ResponseClient::PetStashTake *packet = reinterpret_cast<ResponseClient::PetStashTake*>(msg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_TAKE;
	packet->result = errorCode;
	msg->setSize(sizeof(ResponseClient::PetStashTake));
}

inline void makePetStashTakeUpdateMsg(CNetMsg::SP& msg, int petVIndex)
{
	UpdateClient::PetStashTake *packet = reinterpret_cast<UpdateClient::PetStashTake*>(msg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_TAKE_UPDATE;
	packet->petItemVIndex = petVIndex;
	msg->setSize(sizeof(UpdateClient::PetStashTake));
}

inline void makePetStashKeepMsg(CNetMsg::SP& msg, int errorCode)
{
	ResponseClient::PetStashKeep *packet = reinterpret_cast<ResponseClient::PetStashKeep*>(msg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_KEEP;
	packet->result = errorCode;
	msg->setSize(sizeof(ResponseClient::PetStashKeep));
}

inline void makePetStashProxyRegMsg(CNetMsg::SP& msg, int errorCode)
{
	ResponseClient::PetStashProxyReg *packet = reinterpret_cast<ResponseClient::PetStashProxyReg*>(msg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_PROXY;
	packet->result = errorCode;
	msg->setSize(sizeof(ResponseClient::PetStashProxyReg));
}

inline void makePetStahsProxyRegUpdateMsg(CNetMsg::SP& msg, int petVIndex)
{
	UpdateClient::PetStashProxyReg* packet = reinterpret_cast<UpdateClient::PetStashProxyReg*>(msg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_PROXY_UPDATE;
	packet->virtualIndex = petVIndex;
	msg->setSize(sizeof(UpdateClient::PetStashProxyReg));
}

inline void makePetStashProxyCancelMsg(CNetMsg::SP& msg, int errorCode)
{
	ResponseClient::PetStashProxyCancel *packet = reinterpret_cast<ResponseClient::PetStashProxyCancel*>(msg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_PROXY_CANCEL;
	packet->result = errorCode;
	msg->setSize(sizeof(ResponseClient::PetStashProxyCancel));
}

inline void makePetStashProxyCancelUpdateMsg(CNetMsg::SP& msg, int petVIndex)
{
	UpdateClient::PetStashProxyCancel *packet = reinterpret_cast<UpdateClient::PetStashProxyCancel*>(msg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_PROXY_CANCEL_UPDATE;
	packet->virtualIndex = petVIndex;
	msg->setSize(sizeof(UpdateClient::PetStashProxyCancel));
}

inline void makePetStashEffectMsg(CNetMsg::SP& msg, int effectNo)
{
	UpdateClient::PetStashEffect *packet = reinterpret_cast<UpdateClient::PetStashEffect*>(msg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_EFFECT;
	packet->effectNo = effectNo;
	msg->setSize(sizeof(UpdateClient::PetStashEffect));
}

inline void makePetStashEffectMsgByInmap(CNetMsg::SP& msg, int effectNo)
{
	UpdateClient::PetStashEffectByInMap *packet = reinterpret_cast<UpdateClient::PetStashEffectByInMap*>(msg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_EFFECT_BY_INMAP;
	packet->effectNo = effectNo;
	msg->setSize(sizeof(UpdateClient::PetStashEffect));
}

inline void makePetStashEffectBrMsg(CNetMsg::SP& msg, int effectNo, int charIndex)
{
	UpdateClient::PetStashEffectBr *packet = reinterpret_cast<UpdateClient::PetStashEffectBr*>(msg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_EFFECT_BROAD_CAST;
	packet->charIndex = charIndex;
	packet->effectNo = effectNo;
	msg->setSize(sizeof(UpdateClient::PetStashEffectBr));
}

inline void makePetStashPetUpdateMsg(CNetMsg::SP& msg)
{
	UpdateClient::PetStashPetData *packet = reinterpret_cast<UpdateClient::PetStashPetData*>(msg->m_buf);
	packet->type = MSG_PET_STASH;
	packet->subType = MSG_PET_STASH_PET_UPDATE;
	msg->setSize(sizeof(UpdateClient::PetStashPetData));
}
#endif

#endif
