#ifndef __CMDMSG_H__
#define __CMDMSG_H__

//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )
void LoginRepMsg(CNetMsg& msg, BM_LOGIN_REP_ERROR_TYPE errcode, int subno, int userindex, CUser* user);
void UserInfoRepMsg(CNetMsg& msg, BM_USERINFO_ERROR_TYPE errcode, int subno, int userindex, CUser* user, int iswarn, int istimeout);
void SectionRepMsg(CNetMsg& msg, int subno, int userindex, int flag, CSectionInfo* section);
void PayRepMsg(CNetMsg& msg, BM_PAY_ERROR_TYPE errcode, int subno, int userindex, CUser* user, int isauto);
void UserRegRepMsg(CNetMsg& msg, int subno, int userindex, BM_USER_REG_ERROR_TYPE errcode);
void SupItemRepMsg(CNetMsg& rmsg, int charindex, bool isstart, CSupItem ** supItem);
void SetItemRepMsg(CNetMsg& rmsg, int userindex, int subno, int charindex, MSG_BBILLINFO_ITEM_ERROR_TYPE subtype);

#else

void CashBalanceRepMsg(CNetMsg& msg, BILL_ERROR_MESSAGE_TYPE errcode, char gameID, int userindex, int cpID, int balance);
void CanBuyRepMsg(CNetMsg& msg, BILL_ERROR_MESSAGE_TYPE errcode, char gameID, int userindex, int cpID, char count, const int itemID[], int balance);
void BuyRepMsg(CNetMsg& msg, BILL_ERROR_MESSAGE_TYPE errcode, int serial, char gameID, int userindex, int cpID, char count, const int itemID[], int balance);

#endif

#endif
