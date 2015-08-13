#ifndef GAMEGUARDINTERFACE_H
#define GAMEGUARDINTERFACE_H
#ifndef NO_GAMEGUARD
#include <NPGameLib.h>

typedef void (*GAME_GUARD_RECV_AUTH)(GG_AUTH_DATA auth);
typedef void (*GAME_GUARD_SEND_USERID)(char* pszName);

extern ENGINE_API GAME_GUARD_RECV_AUTH g_pGameGuardRecvAuth;
extern ENGINE_API GAME_GUARD_SEND_USERID g_pGameGuardSendUserID;

#endif
#endif