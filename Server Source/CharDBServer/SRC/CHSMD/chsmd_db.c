/*******************************************************************************
            ABLEX WATAS Project
    Author      :   
    Section     :   
    SCCS ID     :   %W%
    DATE        :   %G%

    Revision History :
        02. 01. 24      Initial

    Description:

    Copyright (C) ABLEX 2004
*******************************************************************************/

#include <chsmd_api.h>
#include <chsmd_extern.h>

#include <utillib.h>
#include <msgdef.h>
#include <define.h>
#include <server_shm.h>
#include <server_sts.h>


/*******************************************************************************

*******************************************************************************/
int dCheckTNS()
{
    FILE		*fp = NULL;
    char    	szBuf[256];
	char    	szTmp[1024];
    char		*szPnt;
    int     	dRet;

    dRet = 0;

	dAppLog(LOG_DEBUG, "-----------------------------");
    sprintf(szTmp, "tnsping localhost");

    fp = popen((const char *)(szTmp), "r");
    if(fp == NULL) {

        dAppLog(LOG_CRI, "dCheckTNS : %s failed in popen [%s]",
        szTmp, strerror(errno));
        return -1;
    }

    while(fgets(szBuf, 256, fp) != NULL) {

        szPnt = strstr(szBuf, "OK");
        if(szPnt != NULL) {

            dAppLog(LOG_DEBUG, "ALIVE LISTENER");
			dRet = 1;
        }

        szPnt = strstr(szBuf, "TNS-12541");
        if(szPnt != NULL) {

            dAppLog(LOG_CRI, "DEAD LISTENER");
			dRet = -1;
        }
    }

    pclose(fp);

    return dRet;
}


/*******************************************************************************

*******************************************************************************/
int dCheckTs(st_Ts_Info *stTs, int dCount)
{
    int     i;

    for(i = 0; i < dCount; i++)
        dAppLog( LOG_INFO, "CHECK_TS : TS[%s] TOTAL[%ld] USED[%ld] FREE[%ld] PER[%d]",
        					stTs->szTsName[i], stTs->lTotal[i], stTs->lUsed[i],
        					stTs->lFree[i], stTs->dPercent[i]);
    return 0;
}


/*******************************************************************************

*******************************************************************************/
int dDB_Compute( pst_Ts_Info pts, int cnt )
{
    st_AlmStat stAlm;
    int dRst;
    int i,j;

    stAlm.ucLocType = LOAD_DB_IDX;
    stAlm.ucStatus = LOAD_INFO;

    for( i = 0; i < fidb->cCurrCnt[LOAD_DB_IDX] ; i++ ){

		for( j = 0; j< cnt; j++ )
			if( strcmp(pts->szTsName[j],szDBTSName[i]) == 0 )
				break;

        dRst = ( ( pts->lUsed[j] * 100.0 ) / pts->lTotal[j] ) * 100;
        stAlm.ucLocIdx = i;
        stAlm.tWhen = time(NULL);
        sprintf( stAlm.Info,"%d",dRst );
        sprintf( &stAlm.Info[32],"%s",szDBTSName[i] );

/*  LOG FORMAT
    DB INFO( 0/20 ) : [ ABCD : 10.3% ]
    DB INFO( 1/20 ) : [ SERVICE : 90.3% ]
    DB INFO( 2/20 ) : [ MMCD : 50.3% ]
*/
		if( FIDB_Handle_Proc( &stAlm ) < 0 ){

			dAppLog( LOG_WARN, "DBTS Compute FAILED [ %s ] : [%d:%s]",
			szDBTSName[i],errno,strerror(errno) );
		}
		else{

			dAppLog( LOG_DEBUG, "DB INFO( %d/%d ) : [ %s : %d.%d%% ]",
			i+1, fidb->cCurrCnt[LOAD_DB_IDX],
			szDBTSName[i],
			dRst/100, dRst%100 );
		}
    }
	return 0;
#if 0
	long total;
	long used;
	int dRet;
	int i;

	for( i = 0, total = 0, used = 0 ; i < cnt ; i ++ ){

		total += pts->lTotal[i];	
		used += pts->lUsed[i];
	}

	dRet = ( ( used * 100.0 ) / total ) * 100;
	
	return dRet;
#endif
	
}


/*******************************************************************************

*******************************************************************************/

int db_check()
{
	st_Ts_Info	stTs;
	st_AlmStat stAlm;
	int			dRet = 0;

	dAppLog(LOG_INFO,"### DBMS LOAD ###" );

	/* re-try db connect */
	if( dbflag == 0 ){

		dRet = dESQLConnect();
		if(dRet < 0)
		{
			dAppLog(LOG_CRI, "DBCHK : FAILED IN dESQLConnect() dRet[%d]", dRet);
		}
		else
			dbflag = 1;
	}

	if( dbflag == 1 ){

		dRet = dGetTsInfo(&stTs);
		if(dRet < 0)
		{
			if( dRet == -3114 ){
				dAppLog(LOG_CRI,
				"FAILED : -3114 Error, NOT Connected to ORACLE " );
			}
			else if( dRet == -1012 ){
				dAppLog(LOG_CRI,
				"FAILED : -3114 Error, NOT Connected to ORACLE " );
			}
			else{
				dAppLog(LOG_CRI,
				"DBCHK : FAILED IN dGetTsInfo dRet[%d:%s]", dRet,strerror(errno));
			}
			dbflag = 0;
			
		}
		else{

			dDB_Compute(&stTs, dRet);

			/* for LOG */
			dCheckTs(&stTs, dRet);
			

			dRet = dCheckTNS();
			if(dRet < 0){
				dAppLog(LOG_CRI, "TNS FAIL RET[%d]", dRet);
				dbflag = 0;
			}
			else
				dAppLog(LOG_DEBUG, "TNS RESULT[%d]", dRet);
		}

	}

	/* DB CONNECT */
	if( fidb->fDBConnect != dbflag ){

		stAlm.ucLocType = DB_CONN_IDX;
		stAlm.ucLocIdx = 0;
		stAlm.tWhen = time(NULL);
		stAlm.ucStatus = dbflag;

		if( dbflag == DEAD ){

/* LOG FORMAT 
		DBCONNECT : ** DIS_CONNECT ALIVE->DEAD, TIME=[3939393]
*/

			dAppLog(LOG_INFO,
			"DBCONNECT : ** DIS_CONNECT ALIVE->DEAD, TIME=[%d]",
			stAlm.tWhen);

		}
		else{

/* LOG FORMAT 
		DBCONNECT : ## DIS_CONNECT DEAD->ALIVE, TIME=[3939393]
*/

		dAppLog(LOG_INFO,
		"DBCONNECT : ** DIS_CONNECT DEAD->ALIVE, TIME=[%d]",
		stAlm.tWhen);

		}

		dRet = FIDB_Handle_Proc(&stAlm);
		if( dRet < 0 ){
			dAppLog(LOG_WARN,"DBCHK : Failed To FIDB_Handle_Proc [%d:%s]",
			errno, strerror(errno) );
		}
	}
	else{
/* LOG FORMAT
		DBMS : CONNECT STATUS=[ DEAD ]
		DBMS : CONNECT STATUS=[ ALIVE ]
*/
		dAppLog(LOG_DEBUG,
		"DBMS : CONNECT STATUS=[ %s ]",
		( dbflag == 1 )? "ALIVE" : "DEAD" );
	}

	fidb->fDBConnect = dbflag;

	return dRet;
} /* end of main */
