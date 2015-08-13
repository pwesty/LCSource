#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

#include <tifb.h>
#include <tifb_api.h>
#include <tifb_global.h>

#include <killmc_api.h>
#include <server_define.h>
#include <server_msgq.h>
#include <msgdef.h>
/*******************************************************************************
*	
*	- added new structure
*	- made by uamyd 050426
*	- Describe
*		temporary storage for readed resource data from file 'RsrcList'
*
*******************************************************************************/
int		g_qidx, g_midx, g_sidx;

typedef struct _st_Shm_Info{
	int dShmKey;
	int	dShmSize;
	/* char szShmName[24]; */

}st_Shm_Info;

typedef struct _st_MsgQ_Info{
	int	dMsgqKey;
	int	dReserved;
	/* char szMsgqName[24]; */
		
}st_MsgQ_Info;

typedef struct _st_Sem_Info{
	int dSemKey;
	int dReserved;
	/* char szSemName[24]; */
}st_Sem_Info;

st_Shm_Info		stShmInfo[500];
st_MsgQ_Info	stMsgqInfo[500];
st_Sem_Info		stSemInfo[500];


void normal_isr(int nSig)
{
    int err;

    printf("\n\tDou you want to stop finalization (y/n) ? ");
    err = GetYorN();

    if(err == _YES)
    {
        sprintf(errbuf, "FINALIZATION STOPPED IN PROGRESS\n");
        strcat(errbuf, "  BLOCKS ALREADY FINALIZED CAN'T AUTOMATICALLY CANCELLED\n"); 
		PrintOut(TIFB_FAIL, errbuf);

        exit(0);
    }

    signal(SIGINT, normal_isr);

    return;
}



int main (int argc, char **argv)
{
    int c, err;
    extern char *optarg;
    extern int optind;
    int vflg = 0;
    int bflg = 0;
    int dflg = 0;
    int errflg = 0;
	char *ProcName=NULL;
	int   pid,tpid, i;
	int		dRet;

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
		printf("Default All Process Stopping...\n");
	}
***/
	if( vflg == 0 && bflg == 0 )
	{
		(void)fprintf(stderr, "usage : KillMC [-v] [-b <blockname>]...\n");	
		exit(2);
	}

	GetMcInitInfo();	/* added by uamyd 050425: for get process list */

	if( bflg )
	{
		/* Only One Process Stop */
		pid = KillOnlyOneBlock(ProcName);
		exit(0);
	}


	if( dflg == 1 )
	{

    	printf("\n\tDo you want to finalize all blocks (y/n) ? ");

    	err = GetYorN();

    	if(err == _NO)
    	{
        	sprintf(errbuf, "STOPPED BY USER REQUEST\n");
        	PrintOut(TIFB_FAIL, errbuf);
        	exit(0);
    	}

    	printf("\n\tDo you really want to finalize all blocks (y/n) ? ");

    	err = GetYorN();

    	if(err == _NO)
    	{
        	sprintf(errbuf, "STOPPED BY USER REQUEST\n");
        	PrintOut(TIFB_FAIL, errbuf);
        	exit(0);
    	}

    	sprintf(errbuf, "MAIN COMPUTER PROCESS FINALIZATION STARTED\n");
    	PrintOut(TIFB_SUCCESS, errbuf);

    	signal(SIGINT, normal_isr);

		failed  = killed = 0;

		/* changed by uamyd 050425: CURR_SW_COM -> g_dProcCnt */
		for(i=0; i<g_dProcCnt; i++ )
		{
			/* changed by uamyd 050425: STR_TSW_COM-> st_SWInit[i].szProcName*/
			pid = GetProcessID( st_SWInit[i].szProcName );
		
			if( pid > 0 )
			{
				printf("\nDo you really want to kill %s blocks (y/n) ? ",st_SWInit[i].szProcName);
                err = GetYorN();
                if( err == _NO )
                {
                    sprintf(errbuf, "CAN'T KILL BLOCKS %d(%s) for User\n", pid, st_SWInit[i].szProcName);
                    PrintOut(TIFB_FAIL, errbuf);
                }
                else{
                    kill(pid, SIGTERM);
                }
				//sleep(1);

				/* changed by uamyd 050425: STR_TSW_COM-> st_SWInit[i].szProcName*/
				pid = GetProcessID( st_SWInit[i].szProcName );
				if( pid > 0 ){
					sprintf(errbuf, "CAN'T KILL PROCESS   PID : %d\n  BLOCK NAME : %s\n", 
						pid, st_SWInit[i].szProcName);
			  		PrintOut(TIFB_SUCCESS, errbuf);
					failed++;
				}
				else{
					sprintf(errbuf, "A PROCESS WAS KILLED\n  PROCESS ID : %d\n  BLOCK NAME : %s\n", 
						pid, st_SWInit[i].szProcName);
			  		PrintOut(TIFB_SUCCESS, errbuf);
					killed++;	
				}
			}
		}

		PrintResult(killed, failed);
	 	dGet_Resource_Info();
		dRemove_Shm_All();
		dRemove_Sem_All();
		dRemove_Msgq_All();
		exit(0);
	}

#ifndef TEST
	printf("\nDo you really want to kill all blocks (y/n) ? ");
	err = GetYorN();
	if( err == _NO ){
		printf("Stop to kill all blocks for User\n\n");
		exit(0);
	}
#endif

	failed  = killed = 0;

	for(i=0; i<g_dProcCnt; i++ )
	{
		pid = GetProcessID( st_SWInit[i].szProcName );
		
		if( pid > 0 )
		{
			kill(pid, SIGTERM);
			//usleep(500000);
			usleep(100000);
			tpid = GetProcessID( st_SWInit[i].szProcName );
			if( tpid > 0 ){
				sprintf(errbuf, "CAN'T KILL PROCESS %s(%d)\n", 
				st_SWInit[i].szProcName,tpid);
				PrintOut(TIFB_FAIL, errbuf);
				failed++;
			}
			else{
				sprintf(errbuf, "A PROCESS WAS KILLED\n  PROCESS ID : %d\n  BLOCK NAME : %s\n", 
				pid, st_SWInit[i].szProcName );
				PrintOut(TIFB_SUCCESS, errbuf);
				killed++;	
			}
		}
	}

	PrintResult(killed, failed);
	dGet_Resource_Info();
	dRemove_Shm_All();
	dRemove_Sem_All();
	dRemove_Msgq_All();
	
	return 0;
}

void PrintResult(int killed, int failed)
{
    char tmpbuf[128];

    sprintf(errbuf, "MAIN COMPUTER PROCESS FINALIZATION ENDED\n");
    sprintf(tmpbuf, "  KILLED PROCESS (CONFIRMED)     : %d\n", killed);
    strcat(errbuf, tmpbuf);
    sprintf(tmpbuf, "  KILLED PROCESS (NOT CONFIRMED)    : %d\n", failed);
    strcat(errbuf, tmpbuf);

    PrintOut(TIFB_SUCCESS, errbuf);

} /* PrintResult */

void Send_Pid_Msg(int nIdx, int nPid)
{
    int     nThsmd_qid;
    int     nRet;
	st_MsgQ  stSndMsg;
	int		dMsgLen;

    nThsmd_qid = msgget( S_MSGQ_CHSMD, 0666|IPC_CREAT );

    if( nThsmd_qid < 0) {
        fprintf(stderr, "\n  [TIFB] [FAIL] S_MSGQ_SMSSMD GET : %s \n",
            strerror(errno));
        exit(1);
    }



	memset( &stSndMsg, 0x00, sizeof(st_MsgQ));
	
	stSndMsg.usType = DEF_SYS;
	stSndMsg.usSvcID = SID_KILL;
	stSndMsg.usMsgID = 0;

	stSndMsg.ucProID = 0;
	stSndMsg.uiIndex = 1;
		

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



void PrintOut(int nFlag, char *szbuf)
{
    char pbuf[MAX_MSGBUF_LEN];

    sprintf(pbuf,"\n%s\n", mtime_tifb());

    strcat(pbuf, "M1061  FINALIZATION OF MAIN COMPUTER PROCESSES\n");

    if( nFlag == TIFB_FAIL)
        strcat(pbuf, "  RESULT = FAIL\n  REASON = ");
    else
        strcat(pbuf, "  RESULT = SUCCESS\n  ");


    strcat(pbuf, szbuf);

    strcat(pbuf, "COMPLETED\n");

    fprintf(stderr, "%s\n", pbuf);

    return;
}



int KillOnlyOneBlock(char *bname)
{
    int pid, tpid;
	int idx;
	int  i, j = 0;
    char errbuf[MAX_ERRBUF_LEN];
	char path[64];
	struct stat stfi;

	for( i = 0; i< g_dProcCnt; i++ ){
		if( !(strcasecmp( bname, st_SWInit[i].szProcName) )){
			idx = i;
			break;
		}
		else{
			idx = -1;
		}
	}

    if ( idx < 0 )
    {
        sprintf(errbuf, "NOT REGISTERED BLOCK\n");
        PrintOut(TIFB_FAIL, errbuf);
        return -1;
    }

	if( st_SWInit[i].dActFlag != 1 ){

		sprintf(errbuf, "NOT STARTING BLOCK, %s FILE VIEW\n", MCINIT_FILE);
		PrintOut(TIFB_FAIL, errbuf);
		return -1;
	}

	sprintf( path, APP_HOME_BIN"%s", st_SWInit[i].szProcName );
	if( stat( path, &stfi ) != 0 ){

		sprintf(errbuf, "NOT EXIST BLOCK %s\n", st_SWInit[i].szProcName);
		PrintOut(TIFB_FAIL, errbuf);
		return -1;
	}
    pid = GetProcessID(bname);

    if(pid >= 0)
    {

		Send_Pid_Msg( idx, pid );
		//sleep(2);

		kill(pid, SIGTERM );

		for( i=0; i<5; i++ )
		{
			//sleep(1);
			tpid = GetProcessID(bname);
			if( tpid < 0 )
			{
				j = 1;
				break;
			}
		
			kill( tpid, SIGTERM );
		}

		if( j == 0 )
		{
			sprintf(errbuf, "CAN'T KILL PROCESS %d(%s)\n", pid, bname );
			PrintOut(TIFB_FAIL, errbuf);
			return -1;
		}
		else
		{
			sprintf(errbuf, "A PROCESS WAS KILLED\n  PROCESS ID : %d\n  BLOCK NAME : %s\n", pid, bname );
			PrintOut(TIFB_SUCCESS, errbuf);
			return 0;
		}
		 
    }


	sprintf(errbuf, "ALREADY STOPPING PROCESS\n");
	PrintOut(TIFB_FAIL, errbuf );

	return -1;
}

/*******************************************************************************
*
*	- new function 'dGet_Resource_Info'
*	- made by uamyd 050426
*	- Describe
*		- read shared memory - key from FILE ' RsrcList '
*
*******************************************************************************/
int dGet_Resource_Info()
{

	FILE	*fa;
	char	szBuf[1024];
	int		lidx;

	fa = fopen( "/home/SERVERAPP/DATA/RESOURCE_LIST.conf", "r" );
	if( fa == NULL ){
		printf( "%s FILE OPEN FAIL (%s)\n", "/SERVERAPP/DATA/RESOURCE_LIST.conf", strerror(errno));
		return -1;
	}
	
	g_sidx = g_midx = g_qidx = lidx = 0;

	while( fgets( szBuf, 1024, fa ) != NULL ){
		if( szBuf[0] != '#' ){
			printf("Error: [%s] File [%d] row format unsuitable\n",
				"/SERVERAPP/DATA/RESOURCE_LIST.conf", lidx+1 );
			fclose( fa );
			return -2;
		}
	
		if( szBuf[1] == '#' )
			continue;
		else if( szBuf[1] == 'E' )
			break;
		else if( szBuf[1] == 'Q' ){
			sscanf( &szBuf[2], "%*s %d", &stMsgqInfo[g_qidx].dMsgqKey ); 
			g_qidx++;
		}
		else if( szBuf[1] == 'M' ){
			sscanf( &szBuf[2], "%*s %d %d", 
				&stShmInfo[g_midx].dShmKey, &stShmInfo[g_midx].dShmSize );
			g_midx++;
		}
		else if( szBuf[1] == 'S' ){
			sscanf( &szBuf[2], "%*s %d", &stSemInfo[g_sidx].dSemKey );
			g_sidx++;
		}
		else{
			printf("Error: unvalid file format[%s]\n", "/SERVERAPP/RESOURCE_LIST.conf" );
			return -1;
		}
		lidx++;
	}

	return 0;
}/* read resource info from file */ 

/*******************************************************************************
*
*	- changed function 'dRemove_Shm_All', 'dRemove_Msgq_All'
*	- by uamyd 050426
*	- Describe
*		- Now, read Key( message queue key, shared memory key, -size..)
*		  from FILE ("RsrcList")
*
*******************************************************************************/
int dRemove_Shm_All()
{
	int i;

	for( i = 0; i< g_midx ; i++ ){
		/* changed by uamyd 050502: NONUSE size stShmInfo[i].dShmSize -> X */
		Remove_shm( stShmInfo[i].dShmKey, 0 );
	}
	return 0;
	
}/* remove shared memory */
int dRemove_Msgq_All()
{
	int i;

	for( i = 0; i< g_qidx ; i++ ){
    	Remove_msgq( stMsgqInfo[i].dMsgqKey );
	}
	return 0;

}/* remove message queue */

int dRemove_Sem_All()
{
	int i;

	for( i = 0; i< g_sidx ; i++ ){
		Remove_sem( stSemInfo[i].dSemKey );
	}
	return 0;

}

int Remove_shm(int dKey, int dSize)
{
    int dSid;
	int dRet;
	struct shmid_ds buf;

    dSid = shmget(dKey, dSize, 0666);

    if( dSid == -1)
	{
		if( errno != ENOENT )
			printf("FIDB(0x%x) shmget fail (%s)\n", dKey, strerror(errno));
		return -1;
	}

	dRet = shmctl( dSid, IPC_RMID, &buf ); 

	if( dRet != 0 )
	{
		printf("FIDB(0x%x) shmctl IPC_RMID fail(%s)\n", dKey, strerror(errno));
	}

	return 0;
}

int Remove_msgq(int dKey)
{
    int dSid;
	int dRet;
	struct msqid_ds buf;

   	dSid = msgget(dKey, 0666);

   	if( dSid == -1)
	{
		if( errno != ENOENT )
			printf("MSGQ(0x%x) msgget fail (%s)\n", dKey, strerror(errno));
		return -1;
	}

	dRet = msgctl( dSid, IPC_RMID, &buf ); 

	if( dRet != 0 )
	{
		printf("(0x%x) msgctl IPC_RMID fail(%s)\n", dKey, strerror(errno));
	}

	return 0;
}

int Remove_sem(int dKey)
{
	int dSid;
	int dRet;

	dSid = semget(dKey, 1, 0);
	if( dSid == -1){

			if( errno!= ENOENT )
					printf("SEM(0x%x) semget fail (%s)\n", dKey, strerror(errno));
			return -1;
	}

	dRet = semctl( dSid, 1, IPC_RMID, 1);
	if( dRet != 0 ){

			printf("SEM(0x%x) semctl IPC_RMID fail (%s)\n", dKey, strerror(errno));
	}

	return 0;
}
