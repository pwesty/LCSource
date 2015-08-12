#ifndef _CHSMD_API_H_
#define _CHSMD_API_H_

#include <chsmd_def.h>
#include <server_msgq.h>
#include <server_mmcd.h>
#include <server_sts.h>

#ifdef DEBUG
void chkpacket();
void pprint2();
#endif

/* chsmd_db.c */
int dCheckTNS();
int dCheckTs();
int dDB_Compute();
int db_check();

/* chsmd_mask */
int GetMaskInfo();
int FillMaskInfo();
int UndateMaskFile();
int CreateMaskFile();

/* chsmd_init */
int dInit_msgq();
void SetUpSignal();
void UserControlledSignal(int sign);
void FinishProgram();
void IgnoreSignal(int sign);
int	LoadThreshold();
int SetFidbCnt();
int SetFidbInit();

/* chsmd_main */
int dSendToMerge();
int dPassToInfo();
int dInit_chsmd();
int dInit_load();
int IsRcvedMessage( pst_MsgQ pstMsg );

/* chsmd_mmc */
int dHandle_mmc(mml_msg *ml);
int dAct_prc(mml_msg *ml);
int dDact_prc(mml_msg *ml);
int dDis_prc(mml_msg *ml);
int mask_alm();
int dis_mask_alm();
int mask_svc();
int dis_mask_svc();
int dSendMess(mml_msg *ml,dbm_msg_t *smsg);

/* df_com */
int df_compute();

/* chsmd_fidb */
int	SendToInfo();
int SendToCond();
int SendToAlmd();
int dSend_Status_Info();
int FIDB_Handle_Proc( st_AlmStat *almsts );
int SetStatus( st_AlmStat *almsts, int idx, int sts, int AlmLevel );
int CompareLoad( int loadidx, int loadinfo, int *AlmLevel );
int SendToCondMsg( st_AlmStat *almsts );

/* check_sw */
char *name_of_process(int id);
int id_of_process(char *name);
int get_proc_id( char *name );
int auto_restart_the_process(int idx);
int check_software_status( int *re_run , int *pid);
void check_software();

/* cpu_check */
long percentages(int cnt, int *out, register long *new, register long *old, long *diffs);
int cpu_compute();
int init_cpu_usage(int interval);	/* SUN */
int init_mem_info();		/* SUN */
int cpu_usage_collector();	/* SUN */
void *start_cpu_usage_collector(void *arg);	/* SUN */
int finalize_cpu_usage();	/* SUN */
int cpu_usage_info( float *idle, float *user, float *kernel, float *wait );	/* SUN */

/* mem_check */
int mem_compute();

/* link_check */
int Link_Check();
int dEthCheck(char *szNetStr);	/* LINUX */

/* new MMCD add */
INT dProc_mmcd(pst_MsgQ pstMsgQ);
INT dDIS_PRC(UCHAR* szBody);
INT dACT_PRC(UCHAR* szOldData, UCHAR* szData);
INT dDACT_PRC(UCHAR* szOldData, UCHAR* szData);
INT dDIS_LOAD_STS(UCHAR* szData);

INT Read_CpuLoad( unsigned long *system, unsigned long *user, unsigned long *nice, unsigned long *idle, unsigned long *total );
INT Read_MemLoad( INT* ullTotal, INT* ullUsed, FLOAT* fPer);
INT dGetDF(st_SoldfList *stSoldfList);

#endif
