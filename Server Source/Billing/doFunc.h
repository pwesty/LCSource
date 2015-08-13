#ifndef __DO_FUNCTION_H__
#define __DO_FUNCTION_H__

//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )

void do_LoginReq(CNetMsg& msg, CDescriptor* desc);
void do_LogoutReq(CNetMsg& msg, CDescriptor* desc);
void do_UserInfoReq(CNetMsg& msg, CDescriptor* desc);
void do_SectionReq(CNetMsg& msg, CDescriptor* desc);
void do_UserRegReq(CNetMsg& msg, CDescriptor* desc);
void do_PayReserveReq(CNetMsg& msg, CDescriptor* desc);
void do_PayOtherReq(CNetMsg& msg, CDescriptor* desc);
void do_ItemReq(CNetMsg& msg, CDescriptor* desc);
void do_SetItemReq(CNetMsg& msg, CDescriptor* desc);

#else // #ifndef LC_TLD

class CDBCmd;

int GetSumPrice(char count, const int itemID[], char gameID);
int GetBalance(int userindex, const char* idname);
int GetBalance(const char* idname);
int DeductBalance(CDBCmd *m_billCmd, int serial, char gameID, int userindex, const char* ip, int cpID, char count, int itemID[], int balance, int sumPrice, int* sumMileage);
void do_BalanceReq(CNetMsg& msg, CDescriptor* desc);
void do_CanbuyReq(CNetMsg& msg, CDescriptor* desc);
void do_BuyReq(CNetMsg& msg, CDescriptor* desc);

#endif // #ifndef LC_TLD


#endif