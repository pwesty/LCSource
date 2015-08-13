#ifndef __BILL_MESSAGE_H__
#define __BILL_MESSAGE_H__

typedef enum __tagBillMessage
{
	MSG_BILL_UNKNOWN	= 0,						// 잘못된 메세지

	BM_CONNECT = 1100,

	MSG_BILLITEM_CASHBALANCE_REQ = 1700,	// 사용자 잔액요청 : gameId(uc) userId(str:32) cpId(n)
	MSG_BILLITEM_CASHBALANCE_REP = 2700,			// 응답	    	   : returnCode(uc) gameId(uc) userId(str:32) cpId(n) cashBalance(int)

	MSG_BILLITEM_CANBUY_REQ = 1720,			// 과금인증요청    : gmaeId(uc) userId(str:32) cpId(n) itemId(n)
	MSG_BILLITEM_CANBUY_REP = 2720,				// 응답			   : returnCode(uc) gameId(uc) userId(str:32) cpId(n) itemId(n) cashBalance(int)

	MSG_BILLITEM_BUY_REQ = 1740,			// 과금 요청	   : gmaeId(uc) userId(str:32) cpId(n) itemId(n)
	MSG_BILLITEM_BUY_REP = 2740,					// 응답			   : returnCode(uc) gameId(uc) userId(str:32) cpId(n) itemId(n) cashBalance(int)

	MSG_BILLITEM_COUPON_LIST_REQ = 1800,	// 쿠폰 리스트 요청 : gameId(uc) userId(str:50) cpId(c) userIndex(n)
	MSG_BILLITEM_COUPON_LIST_REP = 2800,		// 응답

	MSG_BILLITEM_BUYBYCOUPON_REQ = 1810,	// 쿠폰 사용 과금 요청 :
	MSG_BILLITEM_BUYBYCOUPON_REP = 2810,		// 응답

	MSG_BILLITEM_SERVERLIVE = 4400,				// 서버 얼라이브
} BILLMESSAGE_TYPE;

typedef enum __tagBillErrorMessage
{
	MSG_BILLITEM_ERROR_SUCCESS,				// 성공
	MSG_BILLITEM_ERROR_LACKCASH,			// 잔액부족
	MSG_BILLITEM_ERROR_NOTUSER,				// 해당유저 없음
	MSG_BILLITEM_ERROR_NOTITEM,				// 해당아이템 없음
	MSG_BILLITEM_ERROR_DATABASE,			// DB 오류
	MSG_BILLITEM_ERROR_PACKET,				// 패킷 오류
	MSG_BILLITEM_ERROR_ETC,					// 기타 오류
	MSG_BILLITEM_ERROR_NOTACCOUNT	= 12,			// 결재 권한 없음
} BILL_ERROR_MESSAGE_TYPE;

#endif // __BILL_MESSAGE_H__
//
