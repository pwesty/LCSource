#ifndef __CHAT_H_
#define __CHAT_H_

#include "LCString.h"
#include "LCList.h"
#include "MessageType.h"

struct CChatGroup {
   private:
     int m_makeCharIndex;
     int m_chatIndex;
   public:
     CLCList<CLCString> m_charNameList;

     CChatGroup(int, int);
     ~CChatGroup();
     int GetMakeCharIndex(void);
     int GetChatIndex(void);
     void SendGroup(__tagMsgExMessengerType, int, CLCString, CLCString);
     bool CheckSameName(CLCString);
     static int CompByIndex(CChatGroup *, CChatGroup *);
 };
#endif