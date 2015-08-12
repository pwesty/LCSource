#ifndef __BILL_MESSAGE_H__
#define __BILL_MESSAGE_H__

typedef enum __tagBillMessage
{
	MSG_BILL_UNKNOWN	= 0,						// 잘못된 메세지

	BM_CONNECT,								// 커넥터 접속 요청: protocolversion(str:17) serverNum(uc) gameId(uc)
	// protocolversion = "TEnterBillV1.0"

	MSG_BILLITEM_CASHBALANCE_REQ,			// 사용자 잔액요청 : gameId(uc) userindex(n) userId(str:32) cpId(n)
	MSG_BILLITEM_CASHBALANCE_REP,			// 응답	    	   : returnCode(uc) gameId(uc) userindex(n) cpId(n) cashBalance(int)

	MSG_BILLITEM_CANBUY_REQ,				// 과금인증요청    : gameId(uc) userindex(n) userId(str:32) cpId(n) count(uc) itemId(n:count) requestCash(n)
	MSG_BILLITEM_CANBUY_REP,				// 응답			   : returnCode(uc) gameId(uc) userindex(n) cpId(n) count(uc) itemId(n:count) cashBalance(int)

	MSG_BILLITEM_BUY_REQ,					// 과금 요청	   : serial(n) gameId(uc) userindex(n) userId(str:32) cpId(n) ip(str:30) count(uc) itemId(n:count)
	MSG_BILLITEM_BUY_REP,					// 응답			   : serial(n) returnCode(uc) gameId(uc) userindex(n) cpId(n) count(uc) itemId(n:count) cashBalance(int)

	MSG_BILLITEM_SERVERLIVE,				// 서버 얼라이브

	// SOAPHelper 용
	MSG_SOAP_CONNECT_REQ,					// SOAPHelper에 접속 요청 : version(n) server_type(uc) server_no(n)
	// server_type: 1-로그인서버, 2-컨넥터
	MSG_SOAP_CONNECT_REP,					// SOAPHelper에 접속 요청 : errorcode(n)
	// errorcode: MSG_SOAP_LOGIN_ERROR_TYPE
	MSG_SOAP_USER_AUTH_CODE_REQ	, 			// LOGIN_SERVER->SOAP 유저 패스워드 해쉬 요청		: key(n) user_id(str)
	MSG_SOAP_USER_AUTH_CODE_REP	, 			// SOAP->LOGIN_SERVER 유저 패스워드 해쉬 응답		: key(n) password_hash(str)

	MSG_BILLITEM_RELOAD_CASH,				// 캐쉬아이템 가격 정보 리로드
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
	MSG_BILLITEM_ERROR_NOTACCOUNT	= 12,	// 결재 권한 없음
} BILL_ERROR_MESSAGE_TYPE;

typedef enum __tagMsgSOAPConnectErrorType
{
	MSG_SOAP_CONNECT_ERROR_OK				, // 정상
	MSG_SOAP_CONNECT_ERROR_VERSION			, // 버전 오류
	MSG_SOAP_CONNECT_ERROR_SERVER_TYPE		, // 서버 타입 오류
	MSG_SOAP_CONNECT_ERROR_SERVER_NO		, // 서버 번호 오류 (잘못된 값 또는 중복)
	MSG_SOAP_CONNECT_ERROR_ERROR			, // 그외 오류
} MSG_SOAP_CONNECT_ERROR_TYPE;

#endif // __BILL_MESSAGE_H__
//
