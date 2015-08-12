#ifndef _X_TRAP_WRAPPER_H_
#define _X_TRAP_WRAPPER_H_

#include <Engine/LocalDefine.h>

#ifdef XTRAP_SECURE_CKBANG_2010_07_20

typedef struct xTrapUserInfo_s
{
    LPCSTR      lpUserID;
    LPCSTR      lpServerName;
    LPCSTR      lpCharacterName;
    LPCSTR      lpCharacterClass;
    DWORD       dwReserve;
    SOCKET      Socket; // can be NULL.
} xTrapUserInfo_t;

void CREATE_XTRAP_WRAPPER(const char* pcExecPath/* =NULL*/);
void DESTROY_XTRAP_WRAPPER();
bool HexaDump4XTrap_V1(const TCHAR *szFileName, const void *Buf4Dump, const unsigned long BufSize, const TCHAR *szDescription);
class CXTrapWrapper
{
public:
    /*
        ctor

        pcExecPath - execute file path can be NULL (to extract xtrap secure files)
    */
    CXTrapWrapper(const char* pcExecPath=NULL);

    static CXTrapWrapper* Get();

    void    EngineConfirm();  // call to after RegisterClassEx() has been executed.
    void    SetUserInfo(const xTrapUserInfo_t& userInfo);   // it's allow a number of calls but update all args whenever it calls.
    void    ReplyFromServerSignal(LPVOID lpPacketBuf_IN, LPVOID lpPacketBuf_OUT);

private:
    static CXTrapWrapper* m_spThis;
    char m_szExecPath[MAX_PATH];
};

inline CXTrapWrapper* CXTrapWrapper::Get()
{
    ASSERT(m_spThis);
    return m_spThis;
}

#define XTRAP() CXTrapWrapper::Get()

#endif  // XTRAP_SECURE_CKBANG_2010_07_20

#endif  // _X_TRAP_WRAPPER_H_
