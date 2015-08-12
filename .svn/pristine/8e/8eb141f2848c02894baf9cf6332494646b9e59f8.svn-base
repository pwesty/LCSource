/******************************************************r

 *******************************************************/

#include <chsmd_api.h>
#include <chsmd_extern.h>

#include <sys/statvfs.h>
#include <mntent.h>
#include <sys/vfs.h>
#include <utillib.h>

#ifdef FILE_NAME
#undef FILE_NAME
#define FILE_NAME "/etc/mtab"
#else
#define FILE_NAME "/etc/mtab"

#endif /* FILE_NAME */

int dGetDF(st_SoldfList *stSoldfList)
{
	int             dRet;
	int             j;
	FILE            *fp;

	struct mntent	*mp;
	struct statfs	buf;
	int				dDiskCount = 0;

	memset(stSoldfList, 0x00, sizeof(st_SoldfList));

	fp = fopen(FILE_NAME, "r");
	if(fp == NULL)
	{
		dAppLog(LOG_CRI, "%s file open fail(%s)\n",FILE_NAME,strerror(errno));
		exit(0);
	}
	dRet = 0;
	while(1)
	{
		mp = getmntent( fp );
		if( mp == NULL )
		{
			break;
		}

		for( j=0 ; j < dDisk ; j++ )
		{
			if( strcmp( szDisk[j], mp->mnt_dir ) == 0 ) {
				sprintf(stSoldfList->stSoldf[j].szSpecial, "%s", mp->mnt_fsname);
				sprintf(stSoldfList->stSoldf[j].szMountp, "%s", mp->mnt_dir);
				dDiskCount++;
			}
		}
	}

	fclose(fp);
	stSoldfList->dCount = dDiskCount;
	dRet = 0;

	for(j=0; j<stSoldfList->dCount; j++)
	{
		dRet = statfs(stSoldfList->stSoldf[j].szMountp, &buf);
		if(dRet < 0)
		{
			dAppLog(LOG_CRI,"STATVFS ERROR -> dRet = [%d][%s][%s]\n", dRet,strerror(errno), 
						stSoldfList->stSoldf[j].szMountp);
			exit(0);
		}

		stSoldfList->stSoldf[j].llUsed	= (buf.f_blocks - buf.f_bavail) * buf.f_bsize;
		stSoldfList->stSoldf[j].llTotal = (buf.f_blocks ) * buf.f_bsize;

		if(stSoldfList->stSoldf[j].llTotal == 0)
			stSoldfList->stSoldf[j].dPercent = 0;
		else
			stSoldfList->stSoldf[j].dPercent =
				100*stSoldfList->stSoldf[j].llUsed / stSoldfList->stSoldf[j].llTotal;
	}

	return 0;
}

int df_compute()
{
	int				i;
	int				dRet;
	st_SoldfList   	dflist;
	time_t			tTime;
	st_AlmStat		stAlm;
	int				dRst;	

	dAppLog(LOG_INFO,"### DISK INFO ###");
	dRet = dGetDF( &dflist );
	for(i = 0 ; i < dDisk ; i++)
	{
		time(&tTime);
		stAlm.ucLocType = LOAD_DISK_IDX;
		stAlm.ucLocIdx = (char)i;
		stAlm.ucStatus = LOAD_INFO;
		stAlm.tWhen = tTime;

		if(dflist.stSoldf[i].dPercent == 0)
			dRst = 0;
		else
			dRst = ((float)dflist.stSoldf[i].llUsed/(float)dflist.stSoldf[i].llTotal)*100.0 * 100;

		dAppLog(LOG_DEBUG, "DISK PATH[%10s] PER[%d.%d%%]", dflist.stSoldf[i].szMountp, dRst/100, dRst%100);

		sprintf(stAlm.Info, "%d", dRst);
		sprintf(&stAlm.Info[32], "%10s", dflist.stSoldf[i].szMountp);

		dRet = FIDB_Handle_Proc( &stAlm ); 
    	if(dRet < 0)
    	{
        	dAppLog(LOG_CRI,"DISK : Failed in FIDB_Handle_Proc (%d:%s)", dRet, strerror(errno));
        	return -1;
		}
    }	
	return 1;
}


