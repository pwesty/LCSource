#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include <msgdef.h>

#include <tifb.h>
#include <tifb_api.h>
#include <tifb_global.h>

#include <startmc_api.h>

#include <server_msgq.h>
#include <server_define.h>
#include <utillib.h>

int dSet_ProcessPrepare(int proc_idx)
{
	st_FIDB *fidb;
	int dShmID;
	
	dShmID = shmget(S_SSHM_FIDB, sizeof(st_FIDB), 0666);
    if(dShmID < 0)
        return -1;

    fidb = (st_FIDB *)shmat(dShmID, 0, 0);
    if(fidb == (st_FIDB *)-1)
        return -2;

    fidb->stSWInfo[proc_idx].ucDoneFlag = 0;
    shmdt(fidb);

	return 0;
}

int dCheck_ProcessDone(int proc_idx)
{
	st_FIDB *fidb;
	int dShmID;

	dShmID = shmget(S_SSHM_FIDB, sizeof(st_FIDB), 0666);
    if(dShmID < 0)
        return -1;

    fidb = (st_FIDB *)shmat(dShmID, 0, 0);
    if(fidb == (st_FIDB *)-1)
        return -2;

    if(fidb->stSWInfo[proc_idx].ucDoneFlag == 0) {
        shmdt(fidb);
        return 1;
    }

    shmdt(fidb);

	return 0;
}

int dProc_ProcessPrepare(int proc_idx)
{
	int i;

#define MAX_PREPARE_TIME (10 * 60 * 60 *10)	/* 10 min, 1/10 sec */

	for(i = 0; i < MAX_PREPARE_TIME; i++) {
		if(dCheck_ProcessDone(proc_idx) == 0)
			return 0;

		usleep(100000);	/* sleep 1/10 sec */
	}

	return -1;
}

int main (int argc, char **argv)
{
    int c, dRet;
    extern char *optarg;
    int vflg = 0;
    int bflg = 0;
    int dflg = 0;
    int errflg = 0;
	char *ProcName=NULL;
	int   pid, i;
	int sc, fc;
	char  fname[1024];
	char  errbuf[4096];


    dRet = dGetUserPermission();
    if( dRet == 0 )
    {
        sprintf(errbuf, "INAVLID USER PERMISSION\n" );
        PrintOut(TIFB_FAIL, errbuf);

        return 1;
    }


    while ((c = getopt(argc, argv, "b:v")) != EOF)
	{
    	switch (c) 
		{
      	case 'b':
			if( vflg )
				errflg++;
			else
				bflg++;

			ProcName = optarg;

        	break;

      	case 'v':
			if( bflg )
				errflg++;
			else
				vflg++;

        	break;
      	case '?':
			errflg++;
			break;
	
      	}
	}

/***
	if( errflg )
	{
		(void)fprintf(stderr, "usage : StartMC [-v] [-b <blockname>]...\n");
		exit(2);
	}

	if( vflg == 0 && bflg == 0 )
	{
		dflg = 1;	
		printf("Default All Process Starting...\n");
	}
***/

	if( vflg == 0 && bflg == 0 )
	{
		(void)fprintf(stderr, "usage : StartMC [-v] [-b <blockname>]...\n");
		exit(2);
	}
 
	GetMcInitInfo();
	ChkStatus();
	
	if( bflg )
	{
		/* Only One Process Start */
		pid = InitOnlyOneBlock(ProcName);

		if( pid > 0 )
		{
			//sleep(2);

			sprintf(errbuf, "NEW BLOCK : %s\n  PROCESS ID : %d\n",
				ProcName, pid );
			PrintOut(TIFB_SUCCESS, errbuf );

			for( i=0; i<g_dProcCnt; i++ )
			{

				if( !strcasecmp(ProcName, st_SWInit[i].szProcName) )
				{
					Send_Pid_Msg( i, pid );
					break;
				}
			}             

		}

		exit(0);
	}

	sc = fc = 0;
	for( i=0 ; i<MAX_SW_COM; i++ )
	{
		if( st_SWInit[i].dActFlag == 1 )
		{
			pid = GetProcessID(st_SWInit[i].szProcName);
			if( pid > 0 )
			{

            	/* 이미 실행중인 경우 */
				if( dflg )
				{
					/* changed by uamyd 050425: STR_TSW_COM[i] -> st_SWInit[i].szProcName */
            		fprintf(stderr, "\n\tBlock \"%s\" is already running.\n", 
						st_SWInit[i].szProcName ) ;
            		fprintf(stderr, "\tDo you want replace block \"%s\" (y/n) ? ", 
						st_SWInit[i].szProcName );

            		dRet = GetYorN();

            		if(dRet == _NO)
            		{
                		continue;
            		}
            		else
            		{
                		/* New Version of Killing */

                		dRet = kill(pid, SIGTERM);

                		if (dRet < 0)
                		{
                    		//sleep(2);
                    		if (kill(pid, SIGKILL) < 0)
                    		{
								/* changed by uamyd 050425: STR_TSW_COM[i] -> st_SWInit[i].szProcName */
								fprintf(stderr, "\t Can't kill process (%d)(%s)\n",	
									pid, st_SWInit[i].szProcName );
							
								exit(1);	
                    		}
						}
					}
				}
				else
				{
            		fprintf(stderr, "\nBlock \"%s\" is already running.\n", 
						st_SWInit[i].szProcName ) ;
            		fprintf(stderr, "Do you want replace block \"%s\" (y/n) ? ", 
						st_SWInit[i].szProcName );

					dRet = GetYorN();
					if(dRet == _NO){
						fprintf(stderr,"Can't kill process for User\n");
						fc++;
						continue;
					}
					else{
						kill(pid, SIGKILL);
					}
					
				}

//#define APP_HOME_BIN "/SERVERAPP/BIN/"
				dSet_ProcessPrepare(st_SWInit[i].dIdx);
			    sprintf(fname, "%s%s", APP_HOME_BIN, st_SWInit[i].szProcName );

               	pid = fork();

               	if(pid < 0)
               	{
               		sprintf(errbuf, "CAN'T CREATE A NEW PROCESS\n");
               		PrintOut(TIFB_FAIL, errbuf);
					fc++;
               		return -1;
               	}
               	else if(pid == 0)
               	{
					(void)freopen("/dev/null","w",stdout);
					(void)freopen("/dev/null","r",stdin);
					//usleep(100000);
               		/* 입력된 프로세스 실행 */
               		if (execl(fname, st_SWInit[i].szProcName, (char *)0) < 0)
               		{
						fc++;
                   		memset(errbuf, 0x0, MAX_ERRBUF_LEN);
                   		sprintf(errbuf, "CAN'T EXECUTE A NEW BLOCK : %s(%s)\n",
                       		st_SWInit[i].szProcName, fname);
                   		PrintOut(TIFB_FAIL, errbuf);
                   		exit(1);
               		}
                }
               	else
               	{
               		//sleep(2);
					dRet = dProc_ProcessPrepare(st_SWInit[i].dIdx);
					if(dRet != 0) {
						sprintf(errbuf, "PROCESS RUNNING WAIT TIMEOUT\n");
						PrintOut(TIFB_FAIL, errbuf);
						fc++;
						continue;
					}

					sprintf(errbuf, "NEW BLOCK : %s\n  PROCESS ID : %d\n",
						st_SWInit[i].szProcName, pid );
					PrintOut(TIFB_SUCCESS, errbuf );
					sc++;

               		Send_Pid_Msg(i, pid);
               	}
			}
			else
			{
				dSet_ProcessPrepare(st_SWInit[i].dIdx);
				if( dRet < 0 ) {
					fprintf(stderr, "Shared Memory Create or Attach Fail\n");
					exit(1);
				}

			    sprintf(fname, "%s%s", APP_HOME_BIN, st_SWInit[i].szProcName );
    			pid = fork();

    			if(pid < 0)
    			{
        			sprintf(errbuf, "CAN'T CREATE A NEW PROCESS\n");
        			PrintOut(TIFB_FAIL, errbuf);
					fc++;
        			return -1;
    			}
    			else if(pid == 0)
    			{
					(void)freopen("/dev/null","w",stdout);
					(void)freopen("/dev/null","r",stdin);
        			/* 입력된 프로세스 실행 */
					/* changed by uamyd 050425: STR_TSW_COM[i] -> st_SWInit[i].szProcName */
					//usleep(100000);
        			if (execl(fname, st_SWInit[i].szProcName, (char *)0) < 0)
        			{
						fc++;
            			memset(errbuf, 0x0, MAX_ERRBUF_LEN);
            			sprintf(errbuf, "CAN'T EXECUTE A NEW BLOCK : %s(%s)\n",
						st_SWInit[i].szProcName, fname);
            			PrintOut(TIFB_FAIL, errbuf);
            			exit(1);
        			}
    			}
    			else
				{
					//sleep(2);

					dRet = dProc_ProcessPrepare(st_SWInit[i].dIdx);
					if(dRet != 0) {
						sprintf(errbuf, "PROCESS RUNNING WAIT TIMEOUT \n");
						PrintOut(TIFB_FAIL, errbuf);
						fc++;
						continue;
					}

					sprintf(errbuf, "NEW BLOCK : %s\n  PROCESS ID : %d\n",
						st_SWInit[i].szProcName, pid );
					PrintOut(TIFB_SUCCESS, errbuf );
					sc++;

					Send_Pid_Msg(i, pid);
				}
			}

			
		}
		else{
			Send_Pid_Msg( i, -1 );
		}
	}
	PrintResult(sc,fc);

	return 0;
}


void Send_Pid_Msg(int nIdx, int nPid)
{
    int     nThsmd_qid;
    int     nRet;
	st_MsgQ  stSndMsg;
	size_t		dMsgLen;

    nThsmd_qid = msgget( S_MSGQ_CHSMD, 0666|IPC_CREAT );

    if( nThsmd_qid < 0) {
        fprintf(stderr, "\n  [TIFB] [FAIL] S_MSGQ_SMSSMD GET : %s \n",
            strerror(errno));
        exit(1);
    }

	memset( &stSndMsg, 0x00, sizeof(st_MsgQ));
	
	stSndMsg.usType = DEF_SYS;
	stSndMsg.usSvcID = SID_START;
	stSndMsg.usMsgID = 0;

	stSndMsg.ucProID = 0;
	stSndMsg.uiIndex = 1;
		
	/* changed by uamyd 050428: 8 -> 24 */
	stSndMsg.usBodyLen = 24 ;
		
	stSndMsg.usRetCode = 0;
	memcpy( stSndMsg.szBody, &st_SWInit[nIdx].dIdx, 4 );
	memcpy( &stSndMsg.szBody[4], &nPid, 4 );
	memcpy( &stSndMsg.szBody[8], st_SWInit[nIdx].szProcName, PROC_NAME_LEN );

	dMsgLen = DEF_MSGHEAD_LEN + stSndMsg.usBodyLen - sizeof(long);

	nRet = msgsnd(nThsmd_qid, &stSndMsg, dMsgLen, 0);
    if( nRet < 0 )
    {
        fprintf(stderr, "\n  [TIFB] [FAIL] S_MSGQ_CHSMD SEND : %s\n", strerror(errno));
        exit(1);
    }

    return;
}

void ChkStatus()
{
	int i;
	int pid;

	for( i=0 ; i<MAX_SW_COM; i++ )
	{
		if( st_SWInit[i].dActFlag == 1 )
		{
			pid = GetProcessID(st_SWInit[i].szProcName);
			if( pid > 0 ){
				st_SWInit[i].eflg = 0;
				SWPid[i] = pid;
			}
			else{
				st_SWInit[i].eflg = 1;
				SWPid[i] = -1;
			}
		}
	}
	return;
}
void PrintResult(int alived, int failed)
{
    char tmpbuf[128];

    sprintf(errbuf, "MAIN COMPUTER PROCESS INITIALIZATION ENDED\n");
    sprintf(tmpbuf, "  ALIVED PROCESS (CONFIRMED)     : %d\n", alived);
    strcat(errbuf, tmpbuf);
    sprintf(tmpbuf, "  START FAILED PROCESS           : %d\n", failed);
    strcat(errbuf, tmpbuf);

    PrintOut(TIFB_SUCCESS, errbuf);

} /* PrintResult */


/*
 *********************************************************
 * 프로세스 생성 과정을 출력한다.
 *********************************************************
*/

void PrintOut(int nFlag, char *szbuf)
{
    char pbuf[MAX_MSGBUF_LEN];

    sprintf(pbuf,"\n%s\n", mtime_tifb());

    strcat(pbuf, "M1062  INITILIZATION OF MAIN COMPUTER PROCESSES\n");

    /* 실행 결과에 따라 RESULT 생성 */
    if( nFlag == TIFB_FAIL)
        strcat(pbuf, "  RESULT = FAIL\n  REASON = ");
    else
        strcat(pbuf, "  RESULT = SUCCESS\n  ");


    /* 입력된 message buf를 추가한다. */

    strcat(pbuf, szbuf);

    strcat(pbuf, "COMPLETED\n");

    /* 출력 */
    fprintf(stderr, "%s\n", pbuf);

    return;
}



/*
 * 단지 개별 프로세스를 실행 시킬 때 이용
*/
int InitOnlyOneBlock(char *bname)
{
    int pid, idx, i, dRet;
    char fname[FILE_NAME_LEN];
    char errbuf[MAX_ERRBUF_LEN];
	char path[64];
	struct stat stfi;

    /* 등록된 프로세스인지 검사 */
	for( i = 0; i< MAX_SW_COM; i++ ){
		if( !strcasecmp( st_SWInit[i].szProcName, bname ) ){
			idx = i;
			break;
		}
		else{
			idx = -1;
		} 
	}

    if (idx < 0)
    {
        sprintf(errbuf, "NOT REGISTERED BLOCK\n");
        PrintOut(TIFB_FAIL, errbuf);
		exit(1);
    }


//#define MCINIT_FILE "/SERVERAPP/DATA/McInit"
	if( st_SWInit[idx].dActFlag == 0 )
	{
		sprintf( errbuf, "NOT STARTING BLOCK, %s FILE VIEW\n", MCINIT_FILE );
		PrintOut(TIFB_FAIL, errbuf);
		exit(1);
	}

	sprintf( path, APP_HOME_BIN"%s",st_SWInit[idx].szProcName );
	if( stat( path, &stfi ) != 0 ){

		sprintf(errbuf, "NOT EXIST BLOCK : %s\n", st_SWInit[idx].szProcName);
		PrintOut(TIFB_FAIL, errbuf);
		exit(1);
	}

    /* 이미 실행되고 있는지 여부 조사 */
    pid = GetProcessID(bname);

    if(pid >= 0)
    {
        sprintf(errbuf, "ALREADY RUNNING PROCESS\n");
        PrintOut(TIFB_FAIL, errbuf);
		exit(1);
    }

    sprintf(fname, "%s%s", APP_HOME_BIN, bname);

	dSet_ProcessPrepare(st_SWInit[idx].dIdx);

    pid = fork();

    if(pid < 0)
    {
        sprintf(errbuf, "CAN'T CREATE A NEW PROCESS\n");
        PrintOut(TIFB_FAIL, errbuf);
		exit(1);
    }
    else if(pid == 0)
    {
		(void)freopen("/dev/null","w",stdout);
		(void)freopen("/dev/null","r",stdin);
        /* 입력된 프로세스 실행 */
        if (execl(fname, bname, (char *)0) < 0)
        {
            memset(errbuf, 0x0, MAX_ERRBUF_LEN);
            sprintf(errbuf, "CAN'T EXECUTE A NEW BLOCK : %s(%s)\n", 
					bname, fname);
            PrintOut(TIFB_FAIL, errbuf);
            exit(1);
        }
    }
    else
    {
		dRet = dProc_ProcessPrepare(st_SWInit[idx].dIdx);
		if(dRet != 0) {
			sprintf(errbuf, "PROCESS RUNNING WAIT TIMEOUT \n");
			PrintOut(TIFB_FAIL, errbuf);
			exit(1);
		}
        return pid;
    }

	return 0;
}
