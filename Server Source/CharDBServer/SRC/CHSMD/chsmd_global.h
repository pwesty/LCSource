#ifndef _CHSMD_GLOBAL_H_
#define _CHSMD_GLOBAL_H_

#include <chsmd_def.h>
#include <define.h>

int		SystemNo;

int 	dMyQid;
int 	dFstatdQid;
int 	dMmcdQid;
int 	dCSvcQid;
int 	dSSQid;
int		dCondQid;
int		dAlmdQid;
int		dGCISvcQid;
int		dGSISvcQid;
int		dSISVCQid;

time_t  tPastTime;
time_t  tStsTime;
long long  llBack_NID;

int     JiSTOPFlag;
int     FinishFlag;

int		dbflag;
int 	g_dProcCnt;
char	*szProcName[MAX_SW_COM];

char 	proc[MAX_SW_COM];
int 	lastpid[MAX_SW_COM];
char 	cpu[NUMOFCPU];
char 	net[NUMOFNET];
char	oldAlmLevel[MAX_STATUS_COM];

char    szDBTSName[MAX_DB_COM][32];
char 	szTafNet[NUMOFNET][1024];
char 	szTafDisk[NUMOFDISK][1024];
char 	szNet[NUMOFNET][1024];
char 	szDisk[NUMOFDISK][1024];
int 	dNet;
int 	dDisk;

char 	oldcpu[DEF_CPU_ALM_CNT];
long long 	llCurNID;

char    AlmClass[MAX_STATUS_COM];
char    AlmClassSW[MAX_STATUS_COM];
char    AlmClassHW[2];
char    AlmClassLOADCRI[4];
char    AlmClassLOADWARN[4];
char    AlmClassCHNL[2];

char 	SmsName[64];

#endif
