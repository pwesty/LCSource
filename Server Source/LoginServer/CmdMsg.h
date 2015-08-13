#ifndef __ACMDMSG_H__
#define __ACMDMSG_H__

#include "Descriptor.h"

void FailMsg(CNetMsg::SP& msg, MSG_FAIL_TYPE failtype);
void LogoutReqMsg(CNetMsg::SP& msg, int server, int subnum, int zone, CDescriptor* d);
void PlayerReqMsg(CNetMsg::SP& msg);
void PlayerNumMsg(CNetMsg::SP& msg, int recentServer, int recentSubNum, int idx);

#endif
//
