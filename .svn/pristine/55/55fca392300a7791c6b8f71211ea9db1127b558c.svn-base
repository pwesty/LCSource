#ifndef _TIFB_GLOBAL_H_
#define _TIFB_GLOBAL_H_

#include <time.h>
#include <server_shm.h>


/* added by uamyd 050425 */
typedef struct
{
	int	dIdx;
	int dActFlag;
	int eflg;
	char szProcName[PROC_NAME_LEN];
} _st_SWInit;

_st_SWInit	st_SWInit[MAX_SW_COM];

time_t pid_stime;

int g_dProcCnt;


int SWPid[MAX_SW_COM];
char szSWPid[MAX_SW_COM][16];
char SWTime[MAX_SW_COM][64];

char szPid[16];
char szTimeStr[64];

char     mtime_str[32];

int killed, failed;
char   tmpbuf[128], errbuf[4096];

#endif
