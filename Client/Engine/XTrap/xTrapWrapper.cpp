#include "StdH.h"
#include <Engine/LocalDefine.h>

#ifdef XTRAP_SECURE_CKBANG_2010_07_20

#define XTRAP_ON
#include "xTrapWrapper.h"

CXTrapWrapper* g_pkXTrap = NULL;

#include ".\\include\\Xtrap_C_Interface.h"
#pragma comment (lib, "XTrap4Client_mtDll.lib")
#pragma comment (lib, "XTrap4Client_ClientPart_mtDll.lib")
#pragma comment (lib, "XTrap4Client_ServerPart_mtDll.lib")
#pragma comment (lib, "XTrap_Unipher_mtDll.lib")

// This value must be taken from X-Trap Security Solution Team.
// also must be matched to launcher's xtrap key value.
static const char szXTrapArg[] =
		"660970B49758DDDBD3216D9844CFEF62382756D6E840084BFEA47D7284A72C9D9451951349729A98D783C92DB69BB0D04332DFFEFCB54D3C6BF936299C6ED9E20F7D04245246E417A4610E56955373039C33CB438A28ECB56942138AB871F7742D1ADE6C";
//		"660970B49758DDDB41216D9844CFEF6232C26AFE536F21668C3E0EF59B7DA8DB5A73327147CA3E34D35126F8D794022A718F7C161F306F99B72A44BEF45FF7AD0F7D04245246E417A4610E56955373039C33CB438A28ECB56942138AB871F7742D1ADE6C"

//---------------------------------------------------------------------------
void CREATE_XTRAP_WRAPPER(const char* pcExecPath/* =NULL*/)
{
#ifdef XTRAP_ON
    g_pkXTrap = new CXTrapWrapper(pcExecPath);
#endif
}
//---------------------------------------------------------------------------
void DESTROY_XTRAP_WRAPPER()
{
#ifdef XTRAP_ON
    if (g_pkXTrap)
    {
        delete g_pkXTrap;
        g_pkXTrap = NULL;
    }
#endif
}
//---------------------------------------------------------------------------
DWORD WINAPI XTrapCallbackAliveThreadFunc(LPVOID lpParam)
{
    DWORD dwPeriod = 20*1000;   // 20sec
    
    while(TRUE)
    {
        Sleep(dwPeriod);
        XTrap_C_CallbackAlive(dwPeriod);
    }
    
    return 0;
}
//---------------------------------------------------------------------------
// for singletonize
CXTrapWrapper* CXTrapWrapper::m_spThis = NULL;
//---------------------------------------------------------------------------
CXTrapWrapper::CXTrapWrapper(const char* pcExecPath/* =NULL*/)
{
    m_spThis = this;
    ZeroMemory(m_szExecPath, MAX_PATH);
    if (pcExecPath)
        strcpy (m_szExecPath, pcExecPath);

#ifdef XTRAP_ON
    ::MessageBox(NULL, "XTRAP START", "Error", MB_OK);
    XTrap_C_Start((LPCSTR)szXTrapArg, NULL);
    ::MessageBox(NULL, "XTRAP FINISH", "Error", MB_OK);

    DWORD dwThreadID;
    ::CreateThread(NULL, 0, XTrapCallbackAliveThreadFunc, NULL, 0, &dwThreadID);
#endif
}
//---------------------------------------------------------------------------
void CXTrapWrapper::EngineConfirm()
{
#ifdef XTRAP_ON
    XTrap_C_KeepAlive();
#endif
}
//---------------------------------------------------------------------------
void CXTrapWrapper::SetUserInfo(const xTrapUserInfo_t& userInfo)
{
#ifdef XTRAP_ON
    XTrap_C_SetUserInfoEx(userInfo.lpUserID,
                          userInfo.lpServerName,
                          userInfo.lpCharacterName,
                          userInfo.lpCharacterClass,
                          userInfo.dwReserve,
                          userInfo.Socket);
#endif
}
//---------------------------------------------------------------------------
void CXTrapWrapper::ReplyFromServerSignal(LPVOID lpPacketBuf_IN,
                                          LPVOID lpPacketBuf_OUT)
{
#ifdef XTRAP_ON
    XTrap_CS_Step2(lpPacketBuf_IN, lpPacketBuf_OUT, XTRAP_PROTECT_PE|XTRAP_PROTECT_TEXT|XTRAP_PROTECT_EXCEPT_VIRUS);
#endif
}
//---------------------------------------------------------------------------

#endif
