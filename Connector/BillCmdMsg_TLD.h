void BillConnectMsg(CNetMsg::SP& msg);

void BillBalanceReqMsg(CNetMsg::SP& msg, int userindex, const char* idname);
void BillCanbuyReqMsg(CNetMsg::SP& msg, int userindex, const char* idname, char count, int ctid[], int requestCash = -1);
void BillBuyReqMsg(CNetMsg::SP& msg, int userindex, const char* userid, int serial, char count, int ctid[], const char* ip);
