#ifndef _KILLMC_API_H_
#define _KILLMC_API_H_

void normal_isr(int nSig);
void PrintResult(int killed, int failed);
void Send_Pid_Msg(int nIdx, int nPid);
void PrintOut(int nFlag, char *szbuf);
int KillOnlyOneBlock(char *bname);
int dRemove_Shm_All();
int dRemove_Sem_All();
int dRemove_Msgq_All();
int Remove_shm(int dKey, int dSize);
int Remove_msgq(int dKey);
int Remove_sem(int dKey);
/* added by uamyd 050426: for read resource from file */
int dGet_Resource_Info();



#endif
