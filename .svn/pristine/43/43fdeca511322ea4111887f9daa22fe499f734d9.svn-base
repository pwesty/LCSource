#include <sys/ipc.h>
#include <sys/shm.h>

#include <chsmd_api.h>
#include <chsmd_extern.h>

#include <sys/time.h>
#include <utillib.h>
#include <server_define.h>
#include <server_shm.h>

st_SYS_ALMLOAD *sys_almload;

int dInit_msgq()
{

	/*** COMMON ***/
    dMyQid = msgget(S_MSGQ_CHSMD, 0666|IPC_CREAT);
    if(dMyQid < 0)
    {
        dAppLog(LOG_CRI,"FAILED IN msgget, QID=[%d],[%s]",
        	S_MSGQ_CHSMD,strerror(errno));
        return -1;
    }

    dMmcdQid = msgget(S_MSGQ_MMCD, 0666|IPC_CREAT);
    if(dMmcdQid < 0)
    {
        dAppLog(LOG_CRI,"FAILED IN msgget, QID=[%d],[%s]",
        	S_MSGQ_MMCD,strerror(errno));
        return -2;
    }

	dCondQid = msgget(S_MSGQ_COND, 0666|IPC_CREAT);
    if(dCondQid < 0)
    {
        dAppLog(LOG_CRI,"FAILED IN msgget, QID=[%d],[%s]",
        	S_MSGQ_COND,strerror(errno));
        return -3;
    }

    return 1;

}/* end of dInit_msgq */

int dInit_AlmLoad()
{
	int dRet;

	/* ALM LOAD SSHM INIT */
    dRet = shmget( S_SSHM_ALMLOAD, sizeof(st_SYS_ALMLOAD), 0666|IPC_CREAT|IPC_EXCL);
    if ( dRet < 0 )
    {
        if ( errno == EEXIST )
        {
            dRet = shmget( S_SSHM_ALMLOAD, sizeof(st_SYS_ALMLOAD), 0666|IPC_CREAT);
            if ( dRet < 0 )
            {
                dAppLog( LOG_CRI, "[dInit_IPC][shmget][FAIL][dRet:%d", dRet);
                return -6;
            }
            if ( (sys_almload = (st_SYS_ALMLOAD *)shmat( dRet, 0, 0)) == (st_SYS_ALMLOAD *)-1)
            {
                dAppLog( LOG_CRI, "[dInit_IPC][shmat][FAIL][dRet:%d", dRet);
                return -7;
            }
        }       
        else    
        {   
            dAppLog( LOG_CRI, "[dInit_IPC][shmget][FAIL][dRet:%d", dRet);
            return -4;
        }       
    }           
    else    
    {   
        if ( (sys_almload = (st_SYS_ALMLOAD *)shmat( dRet, 0, 0)) == (st_SYS_ALMLOAD *)-1)
        {
            dAppLog( LOG_CRI, "[dInit_IPC][shmat][FAIL SYS ALM LOAD SHM][dRet:%d", dRet);
            return -5;
        }
    }
	
	return 0;
}

void SetUpSignal()
{

	JiSTOPFlag = 1;

    /* WANTED SIGNALS   */
    signal(SIGTERM, UserControlledSignal);
    signal(SIGINT,  UserControlledSignal);
    signal(SIGQUIT, UserControlledSignal);

    /* UNWANTED SIGNALS */
    signal(SIGHUP,  IgnoreSignal);
    signal(SIGALRM, IgnoreSignal);
    signal(SIGPIPE, IgnoreSignal);
    signal(SIGPOLL, IgnoreSignal);
    signal(SIGPROF, IgnoreSignal);
    signal(SIGUSR1, IgnoreSignal);
    signal(SIGUSR2, IgnoreSignal);
    signal(SIGVTALRM, IgnoreSignal);
    signal(SIGCLD, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);

	dAppLog(LOG_INFO,"SIGNAL HANDLER WAS INSTALLED");

    return;
}

void UserControlledSignal(int sign)
{
    JiSTOPFlag = 0;
    FinishFlag = sign;
}


void FinishProgram()
{

    dAppLog(LOG_CRI, "PROGRAM IS NORMALLY TERMINATED, Cause = %d", FinishFlag);
	exit(0);
}

void IgnoreSignal(int sign)
{
    if (sign != SIGALRM)
        dAppLog(LOG_WARN,"UNWANTED SIGNAL IS RECEIVED, signal = %d", sign);


    signal(sign, IgnoreSignal);
}

int SetFidbCnt()
{

	fidb->dTotCompCnt = ATTRCNT;

	fidb->cCurrCnt[SW_COM_IDX] = MAX_SW_COM -1;
	fidb->cCurrCnt[LOAD_CPU_IDX] = CURR_LOAD_CPU;
	fidb->cCurrCnt[LOAD_MEM_IDX] = CURR_LOAD_MEM;
	fidb->cCurrCnt[LOAD_QUE_IDX] = CURR_QUE_COM;

	fidb->cCurrCnt[LOAD_DB_IDX] = 1;
	fidb->cCurrCnt[DB_CONN_IDX] = 1;

	return 0;

}

int SetFidbInit()
{
	int lastidx = 0;
	int  i, j;

	for(i = 0; i < ATTRCNT; i++)
	{
		for(j = 0; j < fidb->cCurrCnt[i]; j++)
		{
			if( i != SW_COM_IDX )
			{
				fidb->stStatus[lastidx].stSts.init = INIT;
			}
			lastidx++;
		}	
	}

	return 0;
}

