#ifndef __PROC_CONNECTOR_MSG_H__
#define __PROC_CONNECTOR_MSG_H__

void ConnLogin(CDescriptor* d, CNetMsg::SP& msg);
void ConnLogout(CDescriptor* d, CNetMsg::SP& msg);
void ConnPlayerReq(CDescriptor* d, CNetMsg::SP& msg);
void ConnPlaying(CDescriptor* d, CNetMsg::SP& msg);

MSG_CONN_ERRORCODE ReadDB(const char* name, const char* pw, int server, int subno, const char* ip, CUser** user, char playmode);

/////////////////////
// Function name	: WriteDB
// Description	    : 유저 테이블에 데이터 저장
//                  : name이나 ip가 NULL 일 경우 t_connect_log는 저장하지 않는다
// Return type		: MSG_CONN_ERRORCODE
//            		: 에러코드
// Argument         : int index
//                  : 포털 인덱스
// Argument         : int server
//                  : 서버군 번호
// Argument         : int subno
//                  : 서브 번호
// Argument         : int zone
//                  : 존 번호
// Argument         : int timestamp
//                  : 플레이시간
// Argument         : const char* ip
//                  : 접속 아이피
// Argument         : const char* name
//                  : 아이디
// Argument         : bool bLogin
//                  : 로그인시 설정
MSG_CONN_ERRORCODE WriteDB(int index, int server, int subno, int zone, int playtime, const char* name, const char* ip, bool bLogin, int pcbangTime = 0);

/////////////////////
// Function name	: WriteDB
// Description	    : 유저 테이블에 데이터 저장 (로그아웃처리)
// Return type		: bool
//            		: 성공 실패 여부
// Argument         : const char* name
//                  : 접속 아이디
bool WriteDB(const char* name);

void ConnPreopenGift(CDescriptor* d, CNetMsg::SP& msg);
void ConnLevelupPoint(CDescriptor* d, CNetMsg::SP& msg);

void ConnEvent2pan4panBoxReq(CDescriptor* d, CNetMsg::SP& msg);
void ConnEvent2pan4panBoxCommit(CDescriptor* d, CNetMsg::SP& msg);

void ConnEvent2pan4panGoodsReq(CDescriptor* d, CNetMsg::SP& msg);
void ConnEvent2pan4panGoodsCommit(CDescriptor* d, CNetMsg::SP& msg);

void ConnEvent2pan4panMoneyReq(CDescriptor* d, CNetMsg::SP& msg);
void ConnRankerReq(CDescriptor* d, CNetMsg::SP& msg);

#ifdef EVENT_PACKAGE_ITEM
void ConnCouponConfirm(CDescriptor* d, CNetMsg::SP& msg);
void ConnCouponUse(CDescriptor* d, CNetMsg::SP& msg);
#endif

void ConnSetLimit(CDescriptor* d, CNetMsg::SP& msg);

void ConnEventGomdori2007(CDescriptor* d, CNetMsg::SP& msg);

#ifdef GMTOOL
void ConnGMToolCommand( CDescriptor* d, CNetMsg::SP& msg );
void ConnGMToolChatMonitor( CDescriptor* d, CNetMsg::SP& msg );
void ConnGMToolChatting( CDescriptor* d, CNetMsg::SP& msg );
#endif // GMTOOL

void ConnEventXmas2007( CDescriptor* d, CNetMsg::SP& msg );


#ifdef LACARETTE_SYSTEM
void ConnLacaretteSystemMsg( CDescriptor* d, CNetMsg::SP& msg);
unsigned char LacaretteToken(int userindex, int charindex, bool bWrite, bool bAdd=true);
unsigned char LacaretteRepeat(int userindex, int charindex, int &repeat, bool bWrite, bool bAdd=true);
#endif

#ifdef DEV_EVENT_PROMOTION2
void ConnEventPromotion2(CDescriptor *d, CNetMsg::SP& msg);
void EventPromotion2CouponUseReq(CDescriptor* d, CNetMsg::SP& msg);
void EventPromotion2CouponItemReq(CDescriptor* d, CNetMsg::SP& msg);
#endif

void ConnWishListMsg(CDescriptor* d, CNetMsg::SP& msg);
void ConnSOAPreloadCash(CDescriptor* d, CNetMsg::SP& msg);

#ifdef EVENT_USER_COMEBACK
void ConnEventUserComeback(CDescriptor* d, CNetMsg::SP& msg);
#endif

#ifdef EVENT_EUR2_PROMOTION
void ConnEventEUR2Promotion(CDescriptor* d, CNetMsg::SP& msg);
#endif

void ConnEventTableInit(CDescriptor* d, CNetMsg::SP& msg);

#if defined (USE_TENTER_BILLING)
#include "ProcConnMsg_TLD.h"
#else
#include "ProcConnMsg_KOR.h"
#endif

#endif
//
