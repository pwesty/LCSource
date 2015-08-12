#ifndef _XTRAP_INTERFACE_H_
#define _XTRAP_INTERFACE_H_

#include <Engine/LocalDefine.h>

#ifdef XTRAP_SECURE_CKBANG_2010_07_20
typedef void (*XTrap_CS_Step2_Interface)(LPVOID lpPacketBuf_IN, LPVOID lpPacketBuf_OUT);
typedef void (*XTrap_C_SetUserInfoEx_Interface)(LPCSTR	lpUserID,
                                                LPCSTR	lpServerName,
                                                LPCSTR	lpCharacterName,
                                                LPCSTR	lpCharacterClass,
                                                DWORD	dwReserve,
                                                SOCKET	Socket);
typedef bool (*pHexaDump4XTrap_V1)(const TCHAR *szFileName, const void *Buf4Dump, const unsigned long BufSize, const TCHAR *szDescription);
extern ENGINE_API XTrap_CS_Step2_Interface g_pXTrap_CS_Step2Func;
extern ENGINE_API XTrap_C_SetUserInfoEx_Interface g_pXTrap_C_SetUserInfoEx_Interface;
extern ENGINE_API pHexaDump4XTrap_V1 g_pHexaDump4XTrap_V1;
#endif

#endif
