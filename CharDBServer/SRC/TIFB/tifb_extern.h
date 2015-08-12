#ifndef _TIFB_EXTERN_H_
#define _TIFB_EXTERN_H_

#include <time.h>
#include "tifb_global.h"

extern time_t pid_stime;

/***	deleted by uamyd 050425: Don't be used
extern int SWInit[MAX_SW_COM];
***/

/* added by uamyd 050425 */
extern _st_SWInit st_SWInit[MAX_SW_COM];
extern int g_dProcCnt;

extern int SWPid[MAX_SW_COM];
extern char szSWPid[MAX_SW_COM][16];
extern char SWTime[MAX_SW_COM][64];

extern char szPid[16];
extern char szTimeStr[64];

extern char     mtime_str[32];



#endif
