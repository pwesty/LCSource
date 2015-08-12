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

ENGINE_API extern void CREATE_XTRAP_WRAPPER(const char* pcExecPath/* =NULL*/);
ENGINE_API extern void DESTROY_XTRAP_WRAPPER();

class CXTrapWrapper
{
public:
    /*
        ctor

        pcExecPath - execute file path can be NULL (to extract xtrap secure files)
    */
    CXTrapWrapper(const char* pcExecPath=NULL);

    static CXTrapWrapper* Get();

    ENGINE_API void    EngineConfirm();  // call to after RegisterClassEx() has been executed.
    ENGINE_API void    SetUserInfo(const xTrapUserInfo_t& userInfo);   // it's allow a number of calls but update all args whenever it calls.
    ENGINE_API void    ReplyFromServerSignal(LPVOID lpPacketBuf_IN, LPVOID lpPacketBuf_OUT);

private:
    static CXTrapWrapper* m_spThis;
    char m_szExecPath[MAX_PATH];
};

inline CXTrapWrapper* CXTrapWrapper::Get()
{
    ASSERT(m_spThis);
    return m_spThis;
}

ENGINE_API extern CXTrapWrapper* g_pkXTrap;
#define XTRAP() CXTrapWrapper::Get()
#define XTRAP_FAR() g_pkXTrap

#endif  // XTRAP_SECURE_CKBANG_2010_07_20

#endif  // _X_TRAP_WRAPPER_H_
