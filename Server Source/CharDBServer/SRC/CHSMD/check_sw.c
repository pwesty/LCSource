/**********************************************************
            ABLEX DTAS Project
    Author      :   tundra 
    Section     :   LDGS CHSMD
    SCCS ID     :   %W%
    DATE        :   %G%

    Revision History :
        02. 01. 24      Initial

    Description:

    Copyright (C) ABLEX 2002
***********************************************************/

/**
    - changed by uamyd 050428: CURR_SW_COM -> g_dProcCnt
    - changed by uamyd 050428: STR_TSW_COM -> szProcName
**/

/**A.1*  File Inclusion ***********************************/
#include <dirent.h>
#include <unistd.h>

#include <chsmd_api.h>
#include <chsmd_extern.h>

#include <utillib.h>
#include <server_define.h>
#include <server_shm.h>


/* GET INDEX OF PROCESS WITH PROC_NAME */
int id_of_process(char *name)
{
    int     i;

    for(i = 0;i < g_dProcCnt;i++) 
	{
        if( !strcmp(name, szProcName[i]) )
		{
            return i;
		}
	}

    return -1;
}

/* 프로세스 이름을 가지고 pid을 찾아내는 방법 */

int get_proc_id( char *name )
{

    int fd;
    DIR 			*dirp;
    struct dirent 	*direntp;
    char 			pname[PROC_NAME_LEN];
	char        	tempbuf[128];

    if((dirp = opendir(PROC_PATH)) == (DIR *)NULL)
    {
        dAppLog(LOG_DEBUG,"get_proc_id : Can't Access Process Directory (%s)", PROC_PATH);
        exit(0);
    }

    while((direntp = readdir(dirp)) != NULL)
    {
        if(!strcmp(direntp->d_name, PARENT_PATH) ||
           !strcmp(direntp->d_name, HOME_PATH)) continue;

        if( !atoi(direntp->d_name) )
        {
            continue;
        }


		sprintf(tempbuf, "%s/%s/cmdline", PROC_PATH, direntp->d_name);

        if((fd = open(tempbuf, O_RDONLY)) < 0)
        {
            continue;
        }


        memset( pname, 0x00, PROC_NAME_LEN);
        if( read(fd, pname, PROC_NAME_LEN-1) < 0 )
        {
            close(fd);
            continue;
        }
        else
        {
            close(fd);
        }

        pname[PROC_NAME_LEN-1] = 0x00;

        if( !strcmp(name, pname) )
        {
            closedir(dirp);
            return atoi(direntp->d_name);
        }

    }

    closedir(dirp);

    return -1;
}


int auto_restart_the_process(int idx)
{
    int         pid;
    struct stat statbuf;
    char        pname[128];
	int 		ret ;

    if( idx < 1 || idx >= g_dProcCnt) return -1;

	pid = get_proc_id(szProcName[idx]);

   	if(pid > 0) 
	{
		dAppLog(LOG_DEBUG, "auto_restart_the_process : %s ALREADY ALIVE WITH %d", 
			szProcName[idx],pid);
		return pid;
	}

	sprintf(pname,"%s%s", APP_HOME_BIN, szProcName[idx]);


	if(stat(pname, &statbuf) < 0) return -1;

	signal(SIGCLD, SIG_IGN);
/**
	signal(SIGCHLD, SIG_IGN);
**/

    pid = fork();

    if(pid < 0) return -1;

    else if (pid == 0) 
	{

		dAppLog(LOG_DEBUG,"Child Process Invoked [%s]", pname);
		(void)freopen("/dev/null", "w", stdout);
		(void)freopen("/dev/null", "r", stdin);
		ret = execl(pname, szProcName[idx], (char *)0);

		exit(0);
    }
    else 
	{
		dAppLog(LOG_CRI," %s Start With %d", szProcName[idx], pid);
        return pid;
	}
} 

int check_software_status( int *re_run , int *pid)
{
	int				dRet;
    int             i, fd, j;
    char            pname[PROC_NAME_LEN*4];
	time_t			now;
	int				status[MAX_SW_COM];
	char			tempbuf[256];
	time_t			tTime;
	st_AlmStat		stAlm;

/* UAMYD 1->0 =i */
	for( i=0; i< g_dProcCnt; i++ )
	{
		status[i] = DEAD;
        re_run[i] = 0;
        pid[i] = 0;
	}

/* UAMYD 1->0 =i */
	for( i=0; i<g_dProcCnt  ; i++ )
	{
		if(lastpid[i] < 3)
				continue;

		sprintf( tempbuf, "%s/%d/cmdline", PROC_PATH, lastpid[i] );

        fd = open(tempbuf, O_RDONLY);

        if(fd < 0)
        {
			dAppLog(LOG_CRI,"CHKSW : Open fail idx[%d] pid[%d]",
					 i, lastpid[i]);
			pid[i] = 0;
            continue;
        }
		else
		{

			memset(pname, 0x00, PROC_NAME_LEN);

	        if (read(fd, pname, PROC_NAME_LEN-1) < 0)
	        {
				dAppLog(LOG_CRI,"CHKSW : Read fail idx[%d] pid[%d]",
					 i, lastpid[i]);
	            close(fd);

				pid[i] = 0;
	            continue;
	        }
	        else
	        {
	            close(fd);

				pname[PROC_NAME_LEN-1] = 0x00;

				j = id_of_process(pname);
 
   	    		if (j >= 0 && j <g_dProcCnt) 
				{
					status[i] = ALIVE;
					pid[i] = lastpid[i];
				}
				else
                {
                    dAppLog(LOG_CRI,
					"CHKSW : Invalid block name[%s] idx[%d] pid[%d]",
					pname, i, lastpid[i]);

					status[i] = DEAD;
					pid[i] = 0;
					if(lastpid[i] > 0)
						kill(lastpid[i], SIGTERM);
                }
			}
		}
    }
			

	now = time(&now);

/* UAMYD 1->0 =i */
	if( now % 10 == 0 ) {
		for( i = 0; i< g_dProcCnt; i++ )
		{
			if((fidb->stStatus[i].stSts.init > 0) && (fidb->stSWInfo[i].ucActFlag > 0 )){
	
        		dAppLog(LOG_INFO, "[%-10s][%2d] INI:STP:STS[%d:%d:%d] PID[%10d] ",
                              	szProcName[i], i, fidb->stStatus[i].stSts.init,
                              	fidb->stStatus[i].stSts.stop, fidb->stStatus[i].stSts.sts,
                              	lastpid[i] );
			}
		}
	}

/* UAMYD 1->0 =i */
	for( i = 0; i< g_dProcCnt; i++ )
	{

		if((status[i] == DEAD) && (fidb->stStatus[i].stSts.stop != STOP)
			&& (lastpid[i] > 0) && (fidb->stStatus[i].stSts.init == INIT))
		{
			re_run[i] = 1;
			if(proc[i] != DEAD)
			{
				time(&tTime);		
				stAlm.ucLocType = SW_COM_IDX;
				stAlm.ucLocIdx = i;
				stAlm.ucStatus = DEAD;
				stAlm.tWhen = tTime;
				sprintf(stAlm.Info,"%d",pid[i]);
				sprintf(&stAlm.Info[32],"%s",szProcName[i]);

    			dAppLog(LOG_DEBUG,
				"time=[%ld],ucLocType=[SW_COM_IDX],ucLocIdx=[%d],ucStatus=[DEAD],Info=[%s]",
        			stAlm.tWhen,stAlm.ucLocIdx,stAlm.Info);

				dRet = FIDB_Handle_Proc( &stAlm );
				if( dRet < 0 ){
					dAppLog( LOG_CRI, "CHKSW : Failed in FIDB_Handle_Proc(%d:%s)",dRet,strerror(errno) );
				}
				proc[i] = DEAD;
			}	
		}
		else if((status[i] == ALIVE))
		{
			if(proc[i] != ALIVE)
			{
				time(&tTime);
				stAlm.ucLocType = SW_COM_IDX;
				stAlm.ucLocIdx = i;
				stAlm.ucStatus = ALIVE;
				stAlm.tWhen = tTime;
				sprintf(stAlm.Info,"%d",pid[i]);
				sprintf(&stAlm.Info[32],"%s",szProcName[i]);
				
				dAppLog(LOG_DEBUG,"time=[%ld],ucLocType=[SW_COM_IDX],ucLocIdx=[%d],ucStatus=[ALIVE],Info=[%s]",
            		stAlm.tWhen,stAlm.ucLocIdx,stAlm.Info);

				dRet = FIDB_Handle_Proc( &stAlm );	
				if( dRet < 0 ){
					dAppLog( LOG_CRI, "CHKSW : Failed in FIDB_Handle_Proc(%d:%s)",dRet,strerror(errno) );
				}

				proc[i] = ALIVE;
			}
		}
	}

	for(i=0;i<g_dProcCnt;i++)
	{
		lastpid[i] = pid[i];
	}

    return 1;
}





void check_software()
{
	int 	i, err;
	int		dRet;
	int 	re_run[MAX_SW_COM], pid[MAX_SW_COM];
	time_t	now;
	st_AlmStat	stAlm;

	if ((err = check_software_status( re_run, pid )) < 0) 
	{
		dAppLog(LOG_CRI,"CHKSW : Failed in check_software_status");
	}


	now = time(&now);

	/* 죽은 프로세스를 자동을 살린다 */
	for(i = 1; i < g_dProcCnt; i++) 
	{

		if( re_run[i] == 1 ) 
		{	/* 살려야 하는 프로세스 */
			if ((err = auto_restart_the_process(i)) <= 0)
			{
				dAppLog(LOG_CRI,"CHKSW : Failed in auto_restart_the_process [%s]",szProcName[i]);
			}
			else
			{
				dAppLog(LOG_CRI," ### RESTART PROCESS [%s] ##" ,szProcName[i]);		
				lastpid[i] = err;
				proc[i] = ALIVE;

                stAlm.ucLocType = SW_COM_IDX;
                stAlm.ucLocIdx = i;
                stAlm.ucStatus = ALIVE;
                stAlm.tWhen = time(NULL);
                sprintf(stAlm.Info,"%d",lastpid[i]);
				sprintf(&stAlm.Info[32],"%s",szProcName[i]);

                dAppLog(LOG_DEBUG,"CHKSW : time=[%ld],ucLocType=[SW_COM_IDX],ucLocIdx=[%d],ucStatus=[ALIVE],Info=[%s]",
                stAlm.tWhen,stAlm.ucLocIdx,stAlm.Info);

                dRet = FIDB_Handle_Proc(&stAlm);
                if(dRet < 0)
                {
                    dAppLog(LOG_CRI,"CHKSW : Failed in FIDB_Handle_Proc");
                }
			}
		}
	}

}
