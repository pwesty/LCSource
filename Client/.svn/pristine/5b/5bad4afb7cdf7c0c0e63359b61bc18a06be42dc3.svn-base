#include "StdH.h"
#include <Engine/LocalDefine.h>
#include <Engine/XTrapInterface/XTrapInterface.h>

#ifdef XTRAP_SECURE_CKBANG_2010_07_20

#include "xTrapWrapper.h"

CXTrapWrapper* g_pkXTrap = NULL;

#include ".\\XTrap\\Xtrap_C_Interface.h"
#pragma comment (lib, ".\\XTrap\\XTrap4Client_mtDll.lib")
#pragma comment (lib, ".\\XTrap\\XTrap4Client_ClientPart_mtDll.lib")
#pragma comment (lib, ".\\XTrap\\XTrap4Client_ServerPart_mtDll.lib")
#pragma comment (lib, ".\\XTrap\\XTrap_Unipher_mtDll.lib")

// This value must be taken from X-Trap Security Solution Team.
// also must be matched to launcher's xtrap key value.
#ifdef KALYDO
static const char szXTrapArg[] =
		"660970B49758DDDBD3216D9849CFEF62CA0541860179B8CDF6D2DA98673BF9B53ED04FF7E6DFC004661C5B7CEBE9C483F4626CE73207732A3E60172732DD043E0F7D04245246E417A4610E56955373039C33CB438A28ECB56942138AB871F7742D1ADE6C79058E7A8A96";
#else
static const char szXTrapArg[] =
		"660970B49758DDDBD3216D9844CFEF62382756D6E840084BFEA47D7284A72C9D9451951349729A98D783C92DB69BB0D04332DFFEFCB54D3C6BF936299C6ED9E20F7D04245246E417A4610E56955373039C33CB438A28ECB56942138AB871F7742D1ADE6C";
//		"660970B49758DDDB41216D9844CFEF6232C26AFE536F21668C3E0EF59B7DA8DB5A73327147CA3E34D35126F8D794022A718F7C161F306F99B72A44BEF45FF7AD0F7D04245246E417A4610E56955373039C33CB438A28ECB56942138AB871F7742D1ADE6C";
#endif


//---------------------------------------------------------------------------
void _XTrap_CS_Step2_Interface(LPVOID lpPacketBuf_IN, LPVOID lpPacketBuf_OUT)
{
    g_pkXTrap->ReplyFromServerSignal(lpPacketBuf_IN, lpPacketBuf_OUT);
}
//---------------------------------------------------------------------------
void _XTrap_C_SetUserInfoEx_Interface(LPCSTR    lpUserID,
                                      LPCSTR    lpServerName,
                                      LPCSTR	lpCharacterName,
                                      LPCSTR	lpCharacterClass,
                                      DWORD     dwReserve,
                                      SOCKET	Socket)
{
    xTrapUserInfo_t xTrapUserInfo;
    xTrapUserInfo.lpCharacterName = lpUserID;
    g_pkXTrap->SetUserInfo(xTrapUserInfo);
}
bool _Xtrap_C_HexaDump4XTrap_V1(const TCHAR *szFileName, const void *Buf4Dump, const unsigned long BufSize, const TCHAR *szDescription)
{
	return HexaDump4XTrap_V1(szFileName, Buf4Dump, BufSize, szDescription);
}
//---------------------------------------------------------------------------
void CREATE_XTRAP_WRAPPER(const char* pcExecPath/* =NULL*/)
{
    g_pkXTrap = new CXTrapWrapper(pcExecPath);
    g_pXTrap_CS_Step2Func = _XTrap_CS_Step2_Interface;
    g_pXTrap_C_SetUserInfoEx_Interface = _XTrap_C_SetUserInfoEx_Interface;
	g_pHexaDump4XTrap_V1 = _Xtrap_C_HexaDump4XTrap_V1;
}
//---------------------------------------------------------------------------
void DESTROY_XTRAP_WRAPPER()
{
    if (g_pkXTrap)
    {
        delete g_pkXTrap;
        g_pkXTrap = NULL;
    }
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
    XTrap_C_Start((LPCSTR)szXTrapArg, pcExecPath);
    if (pcExecPath)
        strcpy (m_szExecPath, pcExecPath);
}
//---------------------------------------------------------------------------
void CXTrapWrapper::EngineConfirm()
{
    XTrap_C_KeepAlive();

    DWORD dwThreadID;
    ::CreateThread(NULL, 0, XTrapCallbackAliveThreadFunc, NULL, 0, &dwThreadID);
}
//---------------------------------------------------------------------------
void CXTrapWrapper::SetUserInfo(const xTrapUserInfo_t& userInfo)
{
    XTrap_C_SetUserInfoEx("ssss",
                          "",
                          "",
                          "",
                          0,
                          0);
}
//---------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
//bool HexaDump4XTrap_V1(const TCHAR *szFileName, const void *Buf4Dump, const unsigned long BufSize, const TCHAR *szDescription);

void CXTrapWrapper::ReplyFromServerSignal(LPVOID lpPacketBuf_IN,
                                          LPVOID lpPacketBuf_OUT)
{
    DWORD dwResult = XTrap_CS_Step2(lpPacketBuf_IN, lpPacketBuf_OUT,
        XTRAP_PROTECT_PE|XTRAP_PROTECT_TEXT|XTRAP_PROTECT_EXCEPT_VIRUS);
}
//---------------------------------------------------------------------------

bool HexaDump4XTrap_V1(const TCHAR *szFileName, const void *Buf4Dump, const unsigned long BufSize, const TCHAR *szDescription)
{
    FILE		*fpDump		= NULL;
    char		*pSrc		= (char*)Buf4Dump;
    SYSTEMTIME	stLocal		= {0,};
    TCHAR		szTemp[MAX_PATH] = {0,};
    
    
#if _MSC_VER >= 1300
    _tfopen_s(&fpDump, szFileName, _T("a+t"));
#else
    fpDump = _tfopen( szFileName, _T("a+"));
#endif
    
    if( fpDump == NULL )
        return false;
    
    GetLocalTime(&stLocal);
#if _MSC_VER >= 1300
    _stprintf_s( szTemp, _countof(szTemp), _T("[%4d/%02d/%02d %02d:%02d:%02d.%03d] "), stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond, stLocal.wMilliseconds);
#else
    _stprintf( szTemp, _T("[%4d/%02d/%02d %02d:%02d:%02d.%03d] "), stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond, stLocal.wMilliseconds);
#endif
    
    _ftprintf(fpDump, szTemp);
    _ftprintf(fpDump, szDescription);
    _ftprintf(fpDump, _T(" :\t"));
    
    for( unsigned int uiLoop = 0; uiLoop < BufSize; uiLoop++ )
    {
        _ftprintf(fpDump, _T("%02X"), pSrc[uiLoop] & 0xff);
    }
    
    _ftprintf(fpDump, _T("\n"));
    
    fclose(fpDump);
    
    return true;
}

#endif
