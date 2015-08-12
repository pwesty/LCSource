/**********************************************************************
**********************************************************************/
#include <sys/sysinfo.h>

#include <chsmd_api.h>
#include <chsmd_extern.h>

#include <utillib.h>

int mem_compute()
{
	int				dRst;
	long long		llUsed;
	long long		llTotal;
	time_t			tTime;
	int				dRet;
	st_AlmStat		stAlm;
    char    szPath[20] = "/proc/meminfo";
    char    szName[20];
    char    buffer[2048];

    int     dTotal, dFree, dBuff, dCach;

    FILE *fp;
	dAppLog( LOG_INFO, "### MEM INFO ###" );

    fp = fopen( szPath, "r" );

    if( fp == NULL )
    {
        dAppLog(LOG_CRI, "%s file open error\n", szPath);
        return -1;
    }

    while( fgets( buffer, 2048, fp ) != NULL )
    {
		if( sscanf( buffer, "%s", szName ) == 1 ){

			if( strcasecmp( szName, "MemTotal:" ) == 0 ){

				if( sscanf( buffer, "%*s %d %*s", &dTotal ) != 1 ){
					dAppLog(LOG_CRI,"FAIL read MemTotal");
					return -1;
				}
			} 
			else if( strcasecmp( szName, "MemFree:" ) == 0 ){

				if( sscanf( buffer, "%*s %d %*s", &dFree) != 1 ){
					dAppLog(LOG_CRI,"FAIL read MemFree");
					return -1;
				}
			}
			else if( strcasecmp( szName, "Buffers:" ) == 0 ){

				if( sscanf( buffer, "%*s %d %*s", &dBuff) != 1 ){
					dAppLog(LOG_CRI,"FAIL read Buffer");
					return -1;
				}
			}
			else if( strcasecmp( szName, "Cached:" ) == 0 ){

				if( sscanf( buffer, "%*s %d %*s", &dCach) != 1 ){
					dAppLog(LOG_CRI,"FAIL read Cached");
					return -1;
				}
			}
		}
		continue;

    }

    fclose(fp);


	//llUsed = dTotal - dFree - dBuff - dCach;
	llUsed = dTotal - dFree;
	llTotal = dTotal;


	dRst = (((double)llUsed/(double)llTotal)*100.0) * 100.0;

	dAppLog(LOG_DEBUG, "MEM TOTAL[%llu] USED[%llu] PER[%3.2f%%]",
    llTotal, llUsed, dRst / 100.0);

    time(&tTime);
    stAlm.ucLocType = LOAD_MEM_IDX;
    stAlm.ucLocIdx = 0;
    stAlm.ucStatus = LOAD_INFO;
    sprintf(stAlm.Info,"%d",dRst);
    stAlm.tWhen = tTime;

    dRet = FIDB_Handle_Proc( &stAlm );
    if(dRet < 0)
    {
        dAppLog(LOG_CRI, "MEM : Failed in FIDB_Handle_Proc (%d:%s)", dRet, strerror(errno));
        return -1;
    }

    return 1;
}

int Read_MemLoad( INT* pdTotal, INT* pdUsed, FLOAT* fPer)
{
    char    szPath[20] = "/proc/meminfo";
    char    szName[20];
    char    buffer[2048];

    int     dTotal, dFree, dBuff, dCach;
    FILE *fp;

    fp = fopen( szPath, "r" );

    if( fp == NULL )
    {
        dAppLog(LOG_CRI, "%s file open error\n", szPath);
        return -1;
    }

    while( fgets( buffer, 2048, fp ) != NULL )
    {
		if( sscanf( buffer, "%s", szName ) == 1 ){

			if( strcasecmp( szName, "MemTotal:" ) == 0 ){

				if( sscanf( buffer, "%*s %d %*s", &dTotal ) != 1 ){
					dAppLog(LOG_CRI,"FAIL read MemTotal");
					return -1;
				}
			} 
			else if( strcasecmp( szName, "MemFree:" ) == 0 ){

				if( sscanf( buffer, "%*s %d %*s", &dFree) != 1 ){
					dAppLog(LOG_CRI,"FAIL read MemFree");
					return -1;
				}
			}
			else if( strcasecmp( szName, "Buffers:" ) == 0 ){

				if( sscanf( buffer, "%*s %d %*s", &dBuff) != 1 ){
					dAppLog(LOG_CRI,"FAIL read Buffer");
					return -1;
				}
			}
			else if( strcasecmp( szName, "Cached:" ) == 0 ){

				if( sscanf( buffer, "%*s %d %*s", &dCach) != 1 ){
					dAppLog(LOG_CRI,"FAIL read Cached");
					return -1;
				}
			}
		}
		continue;

    }

    fclose(fp);

	*pdTotal = dTotal;
	*pdUsed  = dTotal - dFree - dBuff - dCach;
	*fPer	  = ((double)(*pdUsed)/(double)dTotal)*100.0f;

	return 0;
}
