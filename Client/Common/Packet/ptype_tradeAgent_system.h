#ifndef __PTYPE_TRADEAGENT_H__
#define __PTYPE_TRADEAGENT_H__

#include "ptype_base.h"

#ifdef WIN32
typedef unsigned int tradeAgentIndex_t;
#else
typedef unsigned int tradeAgentIndex_t;
#endif

#define MAX_REG_COUNT 10
#define MAX_LIKE_REG_COUNT 10

//Packet SubType Client <-> Server
enum
{
	MSG_TRADEAGENT_LIST,					// 상품 조회 리스트
	MSG_TRADEAGENT_REG_LIST,				// 상품 등록 리스트
	MSG_TRADEAGENT_REG,						// 상품 등록
	MSG_TRADEAGENT_REG_CANCEL,				// 상품 등록 취소
	MSG_TRADEAGENT_BUY,						// 상품 구매
	MSG_TRADEAGENT_LIKE_LIST,				// 관심 물품 리스트
	MSG_TRADEAGENT_LIKE_REG,				// 관심 물품 등록
	MSG_TRADEAGENT_LIKE_CANCEL,				// 관심 물품 취소
	MSG_TRADEAGENT_ERROR,					// 거래대행 에러

	MSG_TRADEAGENT_AFTER_SELL,				// 판매후 판매자에게 메시지 전송
	MSG_TRADEAGENT_AFTER_RETURNED,			// 반품 후 판매자에게 메시지 전송

	MSG_TRADEAGENT_CHANGE_FEE,				// 수수료 변경
	MSG_TRADEAGENT_CHANGE_DEPOSIT,			// 보증금 변경
};

// 서버만 사용함
enum
{
	TRADEAGENT_DB_MSG_TYPE_SELL,
	TRADEAGENT_DB_MSG_TYPE_RETURNED,
};

#pragma pack(push, 1)

struct TradeAgentItem
{
	tradeAgentIndex_t		index;			// 고유 번호
	int						item_index;
	unsigned char			plus;			// 강화 수치
	unsigned char			plus2;			// 추가 수치
	int						flag;
	char					serial[MAX_SERIAL_LENGTH + 1];
	int						item_jobclass;
	int						quantity;
	int						option_count;
	int						option_type[MAX_ITEM_OPTION];
	int						option_level[MAX_ITEM_OPTION];
	int						socketCount;
	int						socket[MAX_SOCKET_COUNT];
	int						item_origin[MAX_ORIGIN_OPTION];
	GoldType_t				nas;
	int						regist_date;
	int						expire_date;
	int						item_num0;				//아이템 물리공격력
	int						item_num1;				//아이템 마법공격력
	int						item_level;
	int						rareGrade;
	char					sell_charName[MAX_ID_NAME_LENGTH];  //MAX_ID_NAME_LENGTH = 64

#ifdef DURABILITY
	int						now_durability;
	int						max_durability;
#endif

	TradeAgentItem()
	{
		memset(this, 0x00, sizeof(*this));
		memset(this->socket, -1, sizeof(this->socket));

#ifdef DURABILITY
		now_durability = 0;
		max_durability = 0;;
#endif
	}
};

namespace RequestClient
{
	//거래대행 리스트 조회 요청
	struct TradeAgentSystemListBase : public pTypeBase
	{
		int charIndex;					//서버에서 찾아서 만들어 사용 client default = 0
		int pageNo;						// 1~
		char itemName[20];
		int itemTypeTab;				// 0~
		GoldType_t minPrice;
		GoldType_t maxPrice;
	};
	struct TradeAgentSystemList : public TradeAgentSystemListBase
	{
		int JobClass;
		int itemSubType;
		int itemGrade;
		int minItemPlus;
		int maxItemPlus;
		int minAttackValue;
		int maxAttackValue;
		int minMagicValue;
		int maxMagicValue;
		int minSocketCount;
		int maxSocketCount;
		int minLevel;
		int maxLevel;
	};

	//거래대행 아이템 구매 요청
	struct TradeAgentSystemBuy : public pTypeBase
	{
		int charIndex;					//서버에서 찾아서 만들어 사용 client default = 0
		int TradeAgentIndex;
		GoldType_t needNas;					//서버만 사용
	};

	//거래대행 아이템 등록 리스트 요청
	struct TradeAgentSystemRegList : public pTypeBase
	{
		int charIndex;					//서버에서 찾아서 만들어 사용 client default = 0
	};

	//거래대행 아이템 등록 요청
	struct TradeAgentSystemReg : public pTypeBase
	{
		int charIndex;					//서버에서 찾아서 만들어 사용 client default = 0
		unsigned short tab;
		unsigned short invenIndex;
		int itemIndex;
		int itemSerial;					//서버에서만 사용
		GoldType_t TotalMoney;
		int quantity;
	};

	struct TradeAgentSystemRegServer : public TradeAgentSystemReg		//서버간 통신에 사용
	{
		int charIndex;
		char charName[MAX_CHAR_NAME_LENGTH +1];
		GoldType_t Guarantee;				//서버에서 계산하여 사용
		TradeAgentItem item;
		int itemCount;
	};

	//거래대행 아이템 등록 취소 요청
	struct TradeAgentSystemRegCancel : public pTypeBase
	{
		int charIndex;					//서버에서 찾아서 만들어 사용 client default = 0
		tradeAgentIndex_t index;
	};

	//거래대행 아이템 관심물품 리스트 요청
	struct TradeAgentSystemLikeList : public pTypeBase
	{
		int charIndex;					//서버에서 찾아서 만들어 사용 client default = 0
	};

	//거래대행 아이템 관심물품 등록 요청
	struct TradeAgentSystemLikeReg : public pTypeBase
	{
		int charIndex;					//서버에서 찾아서 만들어 사용 client default = 0
		tradeAgentIndex_t index;
	};

	//거래대행 아이템 관심물품 취소 요청
	struct TradeAgentSystemLikeCancel : public pTypeBase
	{
		int charIndex;					//서버에서 찾아서 만들어 사용 client default = 0
		tradeAgentIndex_t index;
	};
}

namespace ResponseClient
{
//Error code
enum
{
	TRADEAGENT_ERROR_NOT_EXIST_ITEM,			// 해당 아이템이 존재하지 않습니다.
	TRADEAGENT_ERROR_NOT_ENOUGH_MONEY,			// 돈이 부족합니다.
	TRADEAGENT_ERROR_SEARCH_ERROR,				// 조회 불가.
	TRADEAGENT_ERROR_INCORRECT_MONEY,			// 나스 데이터가 부정확 합니다.
	TRADEAGENT_ERROR_NOT_TRADEITEM,				// 거래 불가 아이템입니다.
	TRADEAGENT_ERROR_ITEM_REG_FAIL,				// 아이템 등록에 실패하였습니다.
	TRADEAGENT_ERROR_ITEM_REG_FULL,				// 등록할 수 있는 아이템의 갯수가 꽉 찾습니다.
	TRADEAGENT_ERROR_ITEM_LIKE_REG_FULL,		// 관심물품으로 등록 할 수 있는 아이템의 갯수가 꽉 찾습니다.
	TRADEAGENT_ERROR_ITEM_REG_CANCEL_FAIL,		// 판매 완료된 아이템
	TRADEAGENT_ERROR_ITEM_BUY_FAIL,				// 아이템 구매 실패
	TRADEAGENT_ERROR_ITEM_BUY_ING,				// 아이템이 판매 진행중입니다.
	TRADEAGENT_ERROR_ITEM_LIKE_LIST_FAIL,		// 아이템 관심물품 리스트 조회 실패(관심 물품이 없음)
	TRADEAGENT_ERROR_ITEM_LIKE_REG_FAIL,		// 아이템 관심물품 추가 실패
	TRADEAGENT_ERROR_ITEM_LIKE_CANCEL_FAIL,		// 아이템 관심물품 삭제 실패
};

	//거래대행 리스트 조회 응답
	struct TradeAgentSystemList : public pTypeBase
	{
		int charIndex;					//서버만 사용했던 값, client 무시해야 하는 데이터
		short pageNo;
		unsigned char nextPage;
		int count;
		int	curServerTime;
		TradeAgentItem list[0];
	};

	//거래대행 아이템 구매 응답
	struct TradeAgentSystemBuy : public pTypeBase
	{
		int charIndex;					//서버만 사용했던 값, client 무시해야 하는 데이터
		GoldType_t needNas;
	};

	struct TradeAgentSystemBuyWithServer : public TradeAgentSystemBuy 
	{
		int itemIndex;
		int itemCount;
		GoldType_t itemPrice;
		char itemSerial[MAX_SERIAL_LENGTH + 1];
		char sellerName[MAX_CHAR_NAME_LENGTH +1];
	};

	//거래대행 아이템 등록 리스트 응답
	struct TradeAgentSystemRegList : public pTypeBase
	{
		int charIndex;					//서버만 사용했던 값, client 무시해야 하는 데이터
		GoldType_t Guarantee;			//다음 등록시 필요한 수수료 값.
		float tradeAgentFee;				//수수료 %
		int count;
		int	curServerTime;
		TradeAgentItem list[0];
	};

	//거래대행 아이템 등록 응답
	struct TradeAgentSystemReg : public pTypeBase
	{
		int charIndex;					// 서버만 사용했던 값, client 무시해야 하는 데이터
		int itemIndex;					// 등록한 아이템
		int quantity;				// 등록한 아이템 갯수 (서버에서만 사용)
		GoldType_t Guarantee;			// 지불한 게런티 (서버에서만 사용)
		int itemCount;					// 감소 메시지를 날리기 위한 아이템 카운트 (서버에서만 사용)
		unsigned short tab;						// 감소 시킬 아이템의 인벤 위치 (서버에서만 사용)
		unsigned short invenIndex;
	};

	struct TradeAgentSystemRegWithServer : public TradeAgentSystemReg 
	{
		char serial[MAX_SERIAL_LENGTH + 1];
		GoldType_t TotalMoney;
	};

	//거래대행 아이템 등록 취소 응답
	struct TradeAgentSystemRegCancel : public pTypeBase
	{
		int charIndex;					//서버만 사용했던 값, client 무시해야 하는 데이터
	};

	struct TradeAgentSystemRegCancelWithServer : public TradeAgentSystemRegCancel 
	{
		int itemIndex;
		int itemCount;
		char serial[MAX_SERIAL_LENGTH + 1];
	};

	//거래대행 아이템 관심물품 리스트 응답
	struct TradeAgentSystemLikeList : public pTypeBase
	{
		int charIndex;					//서버만 사용했던 값, client 무시해야 하는 데이터
		int count;
		int	curServerTime;
		TradeAgentItem list[0];
	};

	//거래대행 아이템 관심물품 등록 응답
	struct TradeAgentSystemLikeReg : public pTypeBase
	{
		int charIndex;					//서버만 사용했던 값, client 무시해야 하는 데이터
	};

	//거래대행 아이템 관심물품 취소 응답
	struct TradeAgentSystemLikeCancel : public pTypeBase
	{
		int charIndex;					//서버만 사용했던 값, client 무시해야 하는 데이터
	};

	struct TradeAgentSystemError : public pTypeBase
	{
		int errorCode;
		int charIndex;					//서버만 사용했던 값, client 무시해야 하는 데이터
		GoldType_t needNas;
		TradeAgentSystemError() : needNas(0) {}
	};

	struct TradeAgentAfterSell : public pTypeBase
	{
		int charIndex;
		int itemIndex;
		int itemCount;
	};

	struct TradeAgentAfterReturned : public pTypeBase
	{
		int charIndex;
		int itemIndex;
		int itemCount;
	};

	struct TradeAgentChangeFee : public pTypeBase
	{
		float fee;
	};

	struct TradeAgentChangeDeposit : public pTypeBase
	{
		unsigned int deposit;
	};
}

#ifndef _CLIENT_

	inline void makeTradeAgentError(CNetMsg::SP& msg, int errorCode)
	{
		ResponseClient::TradeAgentSystemError *packet = reinterpret_cast<ResponseClient::TradeAgentSystemError*>(msg->m_buf);
		packet->type = MSG_TRADEAGENT;
		packet->subType = MSG_TRADEAGENT_ERROR;
		packet->errorCode = errorCode;
		msg->setSize(sizeof(ResponseClient::TradeAgentSystemError));
	}

	inline void makeTradeAgentAfterSell(CNetMsg::SP& msg, int charIndex, int itemIndex, int itemCount)
	{
		ResponseClient::TradeAgentAfterSell* packet = reinterpret_cast<ResponseClient::TradeAgentAfterSell*>(msg->m_buf);
		packet->type = MSG_TRADEAGENT;
		packet->subType = MSG_TRADEAGENT_AFTER_SELL;
		packet->charIndex = charIndex;
		packet->itemIndex = itemIndex;
		packet->itemCount = itemCount;
		msg->setSize(sizeof(ResponseClient::TradeAgentAfterSell));
	}

	inline void makeTradeAgentAfterReturned(CNetMsg::SP& msg, int charIndex, int itemIndex, int itemCount)
	{
		ResponseClient::TradeAgentAfterReturned* packet = reinterpret_cast<ResponseClient::TradeAgentAfterReturned*>(msg->m_buf);
		packet->type = MSG_TRADEAGENT;
		packet->subType = MSG_TRADEAGENT_AFTER_RETURNED;
		packet->charIndex = charIndex;
		packet->itemIndex = itemIndex;
		packet->itemCount = itemCount;
		msg->setSize(sizeof(ResponseClient::TradeAgentAfterReturned));
	}

#endif

#pragma pack(pop)

#endif
