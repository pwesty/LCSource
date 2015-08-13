#include <chsmd_api.h>
#include <chsmd_extern.h>
#include <utillib.h>

long percentages(int cnt, int *out, register long *new, register long *old, long *diffs)
{
    register int i;
    register long change;
    register long total_change;
    register long *dp;
    long half_total;

    /* initialization */
    total_change = 0;
    dp = diffs;

    /* calculate changes for each state and the overall change */
    for (i = 0; i < cnt; i++)
    {
    if ((change = *new - *old) < 0)
    {
        /* this only happens when the counter wraps */
        change = (int)
        ((unsigned long)*new-(unsigned long)*old);
    }
    total_change += (*dp++ = change);
    *old++ = *new++;
    }

    /* avoid divide by zero potential */
    if (total_change == 0)
    {
    total_change = 1;
    }

    /* calculate percentages based on overall change, rounding up */
    half_total = total_change / 2l;
    for (i = 0; i < cnt; i++)
    {
    *out++ = (int)((*diffs++ * 1000 + half_total) / total_change);
    }

    /* return the total in case the caller wants to use it */
    return(total_change);
}

int Read_CpuLoad( unsigned long *system, unsigned long *user, unsigned long *nice, unsigned long *idle, unsigned long *total )
{
    char path[20] = "/proc/stat" ;
    char buff[1024];
    int  cpustate[4];
    static long oldstate[4];
    static long newstate[4];
    static long diffstate[4];

    FILE *fp;


    fp = fopen( path, "r" );

    if( fp == NULL )
    {
        dAppLog(LOG_CRI, "CPU : %s File Open Error\n", path);
        return -1;
    }

    memset( buff, 0x00, sizeof(char)*1024);

    fgets( buff, 1024, fp );

    fclose(fp);

    sscanf( buff, "%s %lu %lu %lu %lu", path, &newstate[0], &newstate[1], &newstate[2], &newstate[3] );
    percentages( 4, cpustate, newstate, oldstate, diffstate );

    *user = cpustate[0];
    *nice = cpustate[1];
    *system = cpustate[2];
    *idle = cpustate[3];

    *total = *user + *nice + *system + *idle ;

    return 1;

}


int cpu_compute()
{
    unsigned long total, user, sys, idle, nice;
	int			dRst;
	time_t		tTime;
	st_AlmStat	stAlm;
	int			dRet;

	dAppLog( LOG_INFO, "### CPU INFO ###" );

    Read_CpuLoad( &sys, &user, &nice, &idle, &total );

	dRst = (100.0 - (double)idle/(double)total*100.0) * 100;
    time(&tTime);
    stAlm.ucLocType = LOAD_CPU_IDX;
    stAlm.ucLocIdx = 0;
    stAlm.ucStatus = LOAD_INFO;
    sprintf(stAlm.Info,"%d",dRst);

    stAlm.tWhen = tTime;

    dAppLog(LOG_DEBUG, 
	"CPU USER[%6.2f] SYSTEM[%6.2f] NICE[%6.2f] IDLE[%6.2f] PER[%d]",
    (double)user/(double)total*100,
    (double)sys/(double)total*100,
    (double)nice/(double)total*100,
    (double)idle/(double)total*100,
	dRst);

#if 0
	dAppLog(LOG_DEBUG,
    "CPU USER[%6.2f] SYSTEM[%6.2f] NICE[%6.2f] IDLE[%6.2f] PER[%d]",
    (double)user,
    (double)sys,
    (double)nice,
    (double)idle,
    dRst);
#endif


	dRet = FIDB_Handle_Proc( &stAlm );
    if(dRet < 0)
    {
  		dAppLog(LOG_CRI,"CPU : Failed in FIDB_Handle_Proc (%d:%s)", errno, strerror(errno));
    }

    return 1;
}

