#include <sys/param.h>
#include <string.h>
#include <sys/shm.h>

#include <tifb.h>
#include <tifb_api.h>
#include <tifb_global.h>

#include <dismc_api.h>
#include <server_define.h>
#include <server_msgq.h>

//st_FIDB *Fidb;

int main (int argc, char **argv)
{
    int 		bflg = 0;
    int 		dflg = 0;
    int 		errflg = 0;
	char 		*ProcName=NULL;
	char		path[64];
	int   		pid, i;
	int			dRet;
	int			sc,ac,dc,ec;
	int			mflg;
	struct stat	stfi;
	struct shmid_ds buf;

	dRet = dGetUserPermission();
	if( dRet == 0 )
	{
        sprintf(errbuf, "INAVLID USER PERMISSION\n" );
        PrintOut(TIFB_FAIL, errbuf);

        return 1;
    }

	/* Check Exist FIDB */
    dRet = shmget( S_SSHM_FIDB, sizeof( st_FIDB ), 0666|IPC_CREAT|IPC_EXCL );
    if( dRet < 0 ) {
        if( errno == EEXIST ){
            mflg = 1;
            dRet = shmget( S_SSHM_FIDB, sizeof( st_FIDB ), 0666|IPC_CREAT );
            if( dRet < 0 ) {

                sprintf(errbuf, "FAILED SHMGET_FIDB [%d:%s]\n", dRet, strerror(errno));
                PrintOut(TIFB_FAIL, errbuf);
                exit(-1);
            }

            if( (long)(fidb = (st_FIDB *)shmat( dRet, 0, 0 )) == -1 ){

                sprintf(errbuf, "FAILED SHMAT_FIDB [%d:%s]\n", errno, strerror(errno));
                PrintOut(TIFB_FAIL, errbuf);
                exit(-1);
            }
        } else {

            sprintf(errbuf, "FAILED SHMGET_FIDB [%d:%s]\n", errno, strerror(errno));
            PrintOut(TIFB_FAIL, errbuf);
            exit(-1);
        }

    } else {
        mflg = 0;
        dRet = shmctl( dRet, IPC_RMID, &buf );
        if( dRet != 0 ) {
            fprintf(stderr, "\n FIDB(0x%x) shmctl IPC_RMID fail (%s)\n", errno, strerror(errno));
            exit(-1);
        }
    }


	if( argc < 0 || argc == 2 || argc > 3 ) {

		errflg++;
	} else if( argc == 1 ) {

		dflg = 1;
		printf("Default All Process Display...\n");
	} else {

		if( argv[1][0] != '-' || argv[1][1] != 'b' )
			errflg++;
		else{
			bflg++;
			ProcName = argv[2];
		}
			 
	}


	if( errflg )
	{
		(void)fprintf(stderr, "usage : DisMC [-b <blockname>]...\n");
		exit(2);
	}

	GetMcInitInfo();

	if( bflg )
	{
		/* Only One Process Display */
		pid = DisOnlyOneBlock(ProcName, mflg);
		exit(0);
	}


	ec = sc = ac = dc =  0;
	for(i=0; i<g_dProcCnt; i++ )
	{
		st_SWInit[i].eflg = 0;
		sprintf( path, APP_HOME_BIN"%s", st_SWInit[i].szProcName );
		if( stat( path, &stfi ) != 0 ){
			st_SWInit[i].eflg = 1;
			ec++;
		}	

		pid = GetProcessID( st_SWInit[i].szProcName );
	
		if( pid >= 0 )
		{
			SWPid[i] = pid;
			sprintf( szSWPid[i], "%d", pid );
			sprintf( SWTime[i], "%s", PrnTime(pid_stime));	
			ac++;
		}	
		else
		{
			if( (mflg == 1) && (fidb->stStatus[st_SWInit[i].dIdx].stSts.stop == 1 ) ){

				SWPid[i] = 0;
				if( st_SWInit[i].dActFlag != 0 )
					sc++;
			}
			else{

				SWPid[i] = -1;
				if( st_SWInit[i].dActFlag != 0 )
					dc++;
			}

			sprintf( szSWPid[i], "    -    ");
			sprintf( SWTime[i], "    -    ");
		}
	}

	PrintResult(ac,sc,dc,ec,mflg);

	return 0;
}



/*
 *********************************************************
 * 프로세스 생성 과정을 출력한다.
 *********************************************************
*/

void PrintOut(int nFlag, char *szbuf)
{
    char pbuf[MAX_MSGBUF_LEN];

    sprintf(pbuf,"\n%s\n", mtime_tifb());

    strcat(pbuf, "M1060  DISPLAY OF MAIN COMPUTER PROCESSES\n");

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
int DisOnlyOneBlock(char *bname, int memflg)
{
    int pid;
	int idx, i;
    char errbuf[MAX_ERRBUF_LEN];
	char path[64];
	struct stat stfi;

    /* 등록된 프로세스인지 검사 */
    for( i=0 ; i< MAX_SW_COM; i++ ){
        if( !(strcasecmp( bname, st_SWInit[i].szProcName) )){
            idx = i;
            break;
        }
        else{
            idx = -1;
        }
    }

    if( idx < 0 )
    {
        sprintf(errbuf, "NOT REGISTERED BLOCK\n");
        PrintOut(TIFB_FAIL, errbuf);
        return -1;
    }

	sprintf( path, APP_HOME_BIN"%s", st_SWInit[i].szProcName);
	if( stat( path, &stfi ) != 0 ){

        sprintf(errbuf, "NOT EXIST FILE %s\n", st_SWInit[idx].szProcName );
        PrintOut(TIFB_FAIL, errbuf);
        return -1;
		
	}

	/* check already running */
    pid = GetProcessID(bname);

    if(pid > 0)
    {
		sprintf(errbuf, "PROCESS ID : %d\n  BLOCK NAME : %s\n\n", 
		pid, bname);
		PrintOut(TIFB_SUCCESS, errbuf);
		return 0;
    }
	else
	{
		if( (memflg== 1) && (fidb->stStatus[st_SWInit[i].dIdx].stSts.stop == 1) ){
            sprintf(errbuf, "STOPED PROCESS : %s\n", bname );
        } else{
            sprintf(errbuf, "UNEXECUTED PROCESS : %s\n", bname );
        }

		PrintOut(TIFB_FAIL, errbuf);
		return 0;
	}

}


void PrintResult(int ac, int sc, int dc, int ec, int memflg)
{
	int i;
	int nc;

	nc = g_dProcCnt -ac -sc -dc;

	printf("\n======================================================================\n");
	printf("%-16s %-10s %-10s %5s %9s %-10s\n",
		"BLOCKNAME", "STATUS", "VERSION", "INDEX", " PID  ", "START-TIME");
	printf("----------------------------------------------------------------------\n");
	
	for(i=0; i<g_dProcCnt; i++ )
	{

		if( SWPid[i] > 0 ){

			printf("%-16s %-10s %-10s %5d %9s %-10s\n",
			st_SWInit[i].szProcName, 
			"ALIVE", 
			((memflg>0) ? fidb->stSWInfo[st_SWInit[i].dIdx].szVersion: "  -  "),
			st_SWInit[i].dIdx,
			szSWPid[i], SWTime[i] );
		}
		else if( st_SWInit[i].dActFlag == 1 ){

			printf("%-16s %-10s %-10s %5d %9s %-10s\n",
			st_SWInit[i].szProcName, 
			(st_SWInit[i].eflg > 0) ? "NOTEXIST":"DEAD",
			((memflg>0) ? fidb->stSWInfo[st_SWInit[i].dIdx].szVersion: "  -  "),
			st_SWInit[i].dIdx,
			szSWPid[i], SWTime[i] );
	

			if( st_SWInit[i].eflg > 0 ){
				if( SWPid[i] < 0 )
					dc--;
				else
					sc--;
			}
				
		}
		else{

			printf("%-16s %-10s %-10s %5s %9s %-10s\n",
				st_SWInit[i].szProcName, 
				(st_SWInit[i].eflg > 0) ? "NOTEXIST":"NOTSTART", 
				"  -  ", "-","    -    ", "    -    " );

			if( st_SWInit[i].eflg > 0 )
				nc--;
		}
	}
	printf("----------------------------------------------------------------------\n");
	printf("TOTAL COUNT OF MAIN COMPUTER PROCESS 	%-2d\n",g_dProcCnt);
	printf("ALIVE PROCESS                        	%-2d\n",ac);
	printf("STOP  PROCESS                        	%-2d\n",sc);
	printf("DEAD  PROCESS                        	%-2d\n",dc);
	printf("NOTSTART  PROCESS                    	%-2d\n",nc);
	if( ec > 0 ){
		printf("NOTEXIST  PROCESS                    	%-2d\n",ec);
	}
	printf("======================================================================\n");
}

char *PrnPid(int pid)
{
	sprintf(szPid, "%d", pid );
	return szPid;
}

char *PrnTime(time_t tpid)
{
	strftime(szTimeStr, 32, "%m-%d %T", localtime((time_t*)&tpid));

	return szTimeStr;
}
