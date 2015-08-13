#ifndef _STARTMC_API_H_
#define _STARTMC_API_H_


void Send_Pid_Msg(int nIdx, int nPid);
void PrintOut(int nFlag, char *szbuf);
void PrintResult(int alived,int failed);
void ChkStatus();
int InitOnlyOneBlock(char *bname);
int GetYorN();

#endif
