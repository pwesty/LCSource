#ifndef __ADESCRIPTOR_H__
#define __ADESCRIPTOR_H__
#include "NetMsg.h"
#include "LCString.h"
#include "Sysdep.h"
#include "MessageType.h"
#include "Conf.h"
#include "Character.h"
#include "InputBuffer.h"
#include "InputQueue.h"
#include "OutputBuffer.h"

class CDescriptor {
   public:
     int m_index;
     CLCString m_idname;
     CLCString m_passwd;
     CLCString m_ident;
     int m_userFlag;
     bool m_tryBug;
     int m_notice[5];
     bool m_bCreate90Lv;
     bool m_bNotCreateNightShadow;
     bool m_bIsNSCreateCardUse;
     CLCString m_proSite;
     bool m_bwaitdb;
     int m_updatedb;
     CInputQueue m_dbOut;
     CMutexObject m_mutexobj;
     socket_t m_desc;
     CLCString m_host;
     CInputBuffer m_inBuf;
     CInputQueue m_inQ;
     COutputBuffer m_outBuf;
     bool m_bCryptNetMsg;
     LCDES3CONTEXT m_nCryptKey;
     LCDES3CONTEXT m_nCryptKeyClient;
     unsigned int m_nPrepareSeed;
     int m_hackCheckPulse;
     int m_hackServerPulse;
     int m_hackClientPulse;
     int m_hackLastClientDiff;
     int m_hackCount;
     int m_idletics;
     int m_checktics;
     short m_commandcount;
     int m_timestamp;
     int m_laststamp;
     int m_connected;
     int m_connectreq;
     int m_logined;
     MSG_LOGIN_TYPE m_playmode;
     bool m_quitsave;
     bool m_bclosed;
     bool m_dbrunning;
     bool m_bGoZone;
     bool m_bGoZoneSent;
     CNetMsg m_msgGoZone;
     bool m_waithelper;
     int m_helpertimeout;
     CPC *m_pChar;
     CDescriptor *m_pPrev;
     CDescriptor *m_pNext;
     int m_helperWaitType;
     CLCString m_hanaroCode;
   private:
     time_t m_tExtendCharacterSlotTime;

   public:
     CDescriptor(void);
     ~CDescriptor();
     void CloseSocket(void);
     void FlushQueues(void);
     void WriteToOutput(CNetMsg &);
     int ProcessOutput(void);
     static int WriteToDescriptor(int, const char *, int);
     int ProcessInput(int);
     static ssize_t PerformSocketRead(int, char *, unsigned int);
     bool GetLogin(CNetMsg &);
     bool StartGame(CNetMsg &);
     bool CharNew(CNetMsg &);
     bool CharDel(CNetMsg &);
     void SetHackCheckPulse(void);
     bool IsHackResponseTimeout(void);
     bool CheckHackPulse(int);
     bool IncreaseHackCount(int);
     void WaitHelperReply(bool, int);
     time_t getExtendCharacterSlotTime(void) const;
     void setExtendCharacterSlotTime(long);
 };
#endif