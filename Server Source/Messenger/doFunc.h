#ifndef __DO_FUNCTION_H__
#define __DO_FUNCTION_H__

void do_Request(CNetMsg::SP& msg, CDescriptor* dest);
void do_Reply(CNetMsg::SP& msg, CDescriptor* dest);

void do_Request_Recommend(CNetMsg::SP& msg);

void do_Request_EventGoldenball(CNetMsg::SP& msg);

#endif //__DO_FUNCTION_H__
