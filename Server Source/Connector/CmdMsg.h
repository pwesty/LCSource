#ifndef __CMD_MSG_H__
#define __CMD_MSG_H__

void LoginRepMsg(CNetMsg::SP& msg, MSG_CONN_ERRORCODE errcode, const char* id, CUser* user);

void ConnCashItemBalanceRep(CNetMsg::SP& msg, int userindex, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalance);
void ConnCashItemPurchaseRep(CNetMsg::SP& msg, int userindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, int cash);
void ConnCashItemBringRep(CNetMsg::SP& msg, int charindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, bool bPresent, int count, int ctid[]);
void ConnCashItemPurchaselistRep(CNetMsg::SP& msg, int charindex, MSG_EX_CASHITEM_ERROR_TYPE errorCode, int count, int idx[], int ctid[]);
void ConnCashItemPurchaseHistoryRep(CNetMsg::SP& msg, int userindex, int charindex, int y, int m, int d);

void ConnCashItemPresentHistoryRep(CNetMsg::SP& msg, int userindex, int charindex, int y, int m, int d, bool bSend);

void PlayerRepMsg(CNetMsg::SP& msg);


void RankerRepMsg(CNetMsg::SP& msg, int charindex);

#ifdef EVENT_PACKAGE_ITEM
void CouponConfirmMsg(CNetMsg::SP& msg, int charindex, MSG_EVENT_ERROR_TYPE subtype, int cIndex = -1, int type = -1);

void CouponUseMsg(CNetMsg::SP& msg, int charindex, MSG_EVENT_ERROR_TYPE subtype, int type = -1);

#endif

void MoveServerOKMsg(CNetMsg::SP& msg, int nUserIndex);

// 게임서버가 접속할때나 한정판매 아이템이 판매될때 모든 게임서버로 보내는 메세지
// 게임서버가 접속할때는 모든 한정판매 아이템 리스트와 모든 팔린 갯수를 보내고
// 한정판매 아이템이 팔릴때는 팔린 인덱스와 팔린 갯수(무조건 1이겟죠?)을 보낸다.
void LimitCatalogMsg(CNetMsg::SP& msg, CLimitSell* limit = NULL);


void ConnEventGomdori2007StatusMsg(CNetMsg::SP& msg, int nCharIndex, int nCount, int* nStatus);

void ConnEventXmas2007Msg( CNetMsg::SP& msg, MSG_CONN_EVENT_XMAS_2007_TYPE subtype );

#ifdef DEV_EVENT_PROMOTION2
void ConnPromotion2RepErrorMsg( CNetMsg::SP& msg,  int charIndex, int errorType );
#endif

void ConnWishlistRep(CNetMsg::SP& msg, int count, int ctid[], int useridx, int charidx);
void ConnWishlistSaveRep(CNetMsg::SP& msg, int useridx, int charidx, int errorcode, int ctid);
void ConnWishlistDelRep(CNetMsg::SP& msg, int useridx, int charidx, int errorcode, int ctid);

#ifdef EVENT_USER_COMEBACK
void ConnEventUserComebackRepMsg( CNetMsg::SP& msg, char errortype, int userindex, int charindex);
#endif
#ifdef EVENT_EUR2_PROMOTION
void ConnEUR2PromotionRepMsg(CNetMsg::SP& msg, MSG_EVENT_EUR2_PROMOTION_ERROR_TYPE type, int charindex);
#endif

#include "BillCmdMsg.h"

#endif
//
