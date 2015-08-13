#include <unistd.h>

#include <chsmd_api.h>
#include <chsmd_global.h>
#include <server_names.h>
#include <server_define.h>
#include <server_mmcd.h>
#include <msgdef.h>
#include <utillib.h>
#include <server_shm.h>

extern int dInit_AlmLoad();

int set_version(int proc_idx, char *version);

char	*version = "R1.0.0";

int main()
{
	int 		dRet;
	int			idx;
	int         pmsg[2];
	time_t      tTime;
	
	st_MsgQ		stMsg;
	st_AlmStat	stAlm;

	Init_logdebug(SEQ_PROC_CHSMD, getpid(), "CHSMD", "/SERVERAPP/LOG");
	
	dRet = Init_shm_common();
	if(dRet < 0)
	{
		dAppLog( LOG_CRI,"MAIN : Failed in Init_shm_common dRet[%d][%s]", 
		dRet, strerror(errno));
		exit(1);
	}
	
	dRet = set_version(SEQ_PROC_CHSMD, version);
	if( dRet < 0 )
		exit(1);

	SetUpSignal();
	
	SetFidbCnt();

	dRet = dInit_load();
    if(dRet < 0)
    {
        dAppLog(LOG_CRI,"Main : Failed in dRead_load");
		return -1;
    }


	dRet = dInit_AlmLoad();
	if( dRet < 0 )
		return -2;

	/*** 4. Initialize chsmd ***/

	dAppLog(LOG_DEBUG,"[STARTING PROCESS]" );

	dRet = dInit_msgq();
	if(dRet < 0)
	{
		dAppLog(LOG_CRI,"MAIN : Failed in dInit_msgq");
		exit(1);
	}

    dInit_chsmd();

	dRet = set_status_running(SEQ_PROC_CHSMD);
    if(dRet < 0)
        exit(1);

	dAppLog(LOG_DEBUG,"[SUCCESS INITIALIZE]" );

	while(JiSTOPFlag)
	{
		
		dRet = IsRcvedMessage(&stMsg);

    	if(dRet < 0){
			dAppLog( LOG_CRI, "MAIN : Failed in IsRcvedMessage, MSQ=[%d:S_MSGQ_CHSMD]", S_MSGQ_CHSMD );
		}
		else if(dRet > 0){

			dAppLog( LOG_DEBUG, "[RCVMSG INFO]" );

			if((stMsg.usSvcID != SID_START) && (stMsg.usSvcID != SID_KILL))
			{
				dRet = dProc_mmcd(&stMsg);
				if(dRet < 0 ) dAppLog( LOG_CRI,"MAIN : Failed in dProc_mmcd");
			}
			else
			{
				memcpy(pmsg,(int *)&stMsg.szBody[0],sizeof(int)*2);
				idx = pmsg[0];
				memcpy( szProcName[idx], &stMsg.szBody[8], PROC_NAME_LEN );
				dAppLog( LOG_DEBUG, "MAIN : [TIFB] SVCID[%d] INDEX[%d] PID[%d] BLOCK:%s", 
						stMsg.usSvcID, pmsg[0], pmsg[1], &stMsg.szBody[8] );

				switch(stMsg.usSvcID)
				{
					case	SID_KILL :

						if( idx < 0 )
							dAppLog( LOG_WARN, "MAIN : Under min[0]" );
						else if( idx > MAX_SW_COM )
							dAppLog( LOG_WARN, "MAIN : Over MAX_SW_COM[32]" );
						else if( stMsg.usBodyLen != 24 )
							dAppLog( LOG_WARN, "MAIN : Unvalid body-len" );
						else{
							lastpid[idx] = 0;

							/** Not Pass **/
							stAlm.ucLocType = SW_COM_IDX;
							stAlm.ucLocIdx = ( char )idx;
							stAlm.ucStatus = SET_STOP;
							sprintf( stAlm.Info,"%d", 0 );
							sprintf( &stAlm.Info[32],"%s", szProcName[idx] );
							stAlm.tWhen = time(NULL);
							/** up to here **/
							
							FIDB_Handle_Proc( &stAlm ); 

							dAppLog( LOG_DEBUG, "MAIN : KillMC -b %s(%d)", szProcName[idx],pmsg[1]);
						}
						break;
		
					case	SID_START :

						if( idx < 0 ){
							dAppLog( LOG_WARN, "MAIN : Under min[0]" );
						}
						else if( idx > MAX_SW_COM ){
							dAppLog( LOG_WARN, "MAIN : Over MAX_SW_COM[32]" );
						}
						else if( stMsg.usBodyLen != 24 ){
							dAppLog( LOG_WARN, "MAIN : Unvalid body-len" );
						}
						else{

							/** Not Pass **/
							stAlm.ucLocType = SW_COM_IDX;
							stAlm.ucLocIdx = ( char )idx;
							stAlm.ucStatus = SET_START;
							sprintf( stAlm.Info,"%d", pmsg[1] );
							sprintf( &stAlm.Info[32],"%s", szProcName[idx] );
							stAlm.tWhen = time(NULL);
							/** up to here **/
							FIDB_Handle_Proc( &stAlm );

							dAppLog( LOG_DEBUG, "MAIN : StartMC -b %s(%d)", szProcName[idx],pmsg[1]);

							if( fidb->stSWInfo[idx].ucActFlag == 1 ){

							}
							lastpid[idx] = pmsg[1];
						}
						break;
				}
			}
    	}		

		check_software();
		dRet = Link_Check();
		if(dRet < 0)
		{
			dAppLog( LOG_CRI,"MAIN : Failed in Link_Check (%d:%s)",dRet, strerror(errno));
		}

		time(&tTime);	

		if(abs(tTime - tStsTime) >= STS_CHECK_LIMIT)
		{
			dAppLog( LOG_DEBUG,"############### STATUS INFO ###############"); 
			dRet = cpu_compute();
			if(dRet < 0)
			{
				dAppLog( LOG_CRI,"MAIN : Failed in cpu_compute");
			}

			dRet = mem_compute();
			if(dRet < 0)
			{
				dAppLog( LOG_CRI,"MAIN : Failed in mem_compute");
			}

			dRet = df_compute();
			if(dRet < 0)
			{
				dAppLog( LOG_CRI,"MAIN : Failed in df_compute");
			}

			dSend_Status_Info();

			tStsTime = tTime;
		}
	}
	FinishProgram();
	
	return 1;			
}/* end of main */

int dInit_chsmd()
{
	int i;

	g_dProcCnt = MAX_SW_COM;
	
	for(i=0;i<MAX_SW_COM;i++)
	{
		proc[i] = -1;
		lastpid[i] = fidb->stSWInfo[i].uiPid;
	}	
	for(i=0;i<NUMOFCPU;i++)
		cpu[i] = -1;

	for(i=0;i<NUMOFNET;i++)
		net[i] = -1;

	for( i = 0; i < MAX_SW_COM; i++ )
		szProcName[i]=fidb->stSWInfo[i].szProcName;

	for( i = 0; i < MAX_STATUS_COM; i++ )
		oldAlmLevel[i]=0;
	fidb->cCurrCnt[SW_COM_IDX] = MAX_SW_COM;
	fidb->dTotCompCnt = ATTRCNT;	/* for fidb */

	return 1;

}/* end of dInit_chsmd */

int dInit_load()
{
	FILE *fa;
	char szBuf[1024];
	char szType[64];
	char szTmp[64];
	char szInfo[64];
	int i,j,k,l,m,n;	/* added by uamyd 050510 : l,m,n to used for fidb */

	fa = fopen("/home/SERVERAPP/DATA/sysconfig.dat","r");
	if(fa == NULL)
	{
		dAppLog( LOG_CRI,"dInit_load : %s FILE OPEN FAIL (%s)",
		"sysconfig.dat", strerror(errno));
		return -1;
	}

	i = 0;  /* line of FILE */
    j = 0;  /* count of DB Table Space Name */
    k = 0;  /* count of DISK PATH */
    l = 0;  /* count of NTAF DISK */
    m = 0;  /* count of Net Type */
    n = 0;  /* count of NTAF NET */
	
	while(fgets(szBuf,1024,fa) != NULL)
	{
		if(szBuf[0] != '#')
		{
			dAppLog( LOG_CRI,"dInit_load : %s File [%d] row format error",
			SYS_CONFIG_FILE, i);
			fclose(fa);
			return -1;
		}

		i++;

		if(szBuf[1] == '#')
			continue;
		else if(szBuf[1] == 'E')
			break;
		else if(szBuf[1] == '@') {
			dAppLog( LOG_DEBUG,"dInit_load : Read Data (%s)",&szBuf[2]);
   			if(sscanf(&szBuf[2],"%s %s %s",szType,szTmp,szInfo) == 3)
			{
				if(strcmp(szType,"NET") == 0)
				{
					if(strcmp(szTmp,"NUM") == 0)
					{
						dNet = atoi(szInfo);
						fidb->cCurrCnt[HW_ETH_IDX] = atoi(szInfo);
					}
	                else if(strcmp(szTmp,"TYPE") == 0)
                    {
                        sprintf(szNet[m], "%s", szInfo);
                        m++;
                    }				
				}
                if(strcmp(szType,"NTAFNET") == 0)
                {
                    if(strcmp(szTmp,"TYPE") == 0)
                    {
                        sprintf(szTafNet[n], "%s", szInfo);
                        n++;
                    }
                }
				else if(strcmp(szType,"DISK") == 0)
				{
					if(strcmp(szTmp,"NUM") == 0)
					{
						dDisk = atoi(szInfo);
						fidb->cCurrCnt[LOAD_DISK_IDX] =  dDisk;
					} else if(strcmp(szTmp,"PATH") == 0) {
                        sprintf(szDisk[k], "%s", szInfo);
                        k++;
                    }
				}
                else if(strcmp(szType,"NTAFDISK") == 0)
                {
                    if(strcmp(szTmp,"PATH") == 0)
                    {
                        sprintf(szTafDisk[l], "%s", szInfo);
                        l++;
                    }
                }
				else if(strcmp(szType,"DBMS") == 0){

                    if(strcmp(szTmp,"NUM") == 0){

                        fidb->cCurrCnt[LOAD_DB_IDX] = atoi(szInfo);
                    }
                    else if(strcmp(szTmp,"NAME") == 0){

                        sprintf(szDBTSName[j], "%s", szInfo);
                        j++;
                    }
                }
			}
		}
	}
	
	fclose(fa);

	return 1;
}/* end of dInit_load */

int IsRcvedMessage( pst_MsgQ pstMsg )
{
    int     dRet;


    alarm(1);

    dRet = msgrcv(dMyQid, pstMsg, sizeof( st_MsgQ ) - sizeof( long int ), 0, 0);

    alarm(0);

    if( dRet < 0 )
    {
        if( errno != EINTR && errno != ENOMSG )
        {
            dAppLog( LOG_CRI, "IsRcvedMessage : [SMCMD] [FAIL:%d] MSGGET MYQ : %s",
                errno, strerror(errno) );
            exit(0);
        }
        return 0;
    }

    return 1;
}

int Reload_Component_Info()
{
	int i;
	int idx=0;


	dAppLog(LOG_CRI, "########## SCP/SMC INFO CHANGE ########\n");

	SetFidbCnt();

	SetFidbInit();

	for(i=0; i<fidb->dTotCompCnt; i++ )
	{
		idx += fidb->cCurrCnt[i];
	}

	for(i = idx; i < MAX_STATUS_COM; i++)
	{
		memset(&fidb->stStatus[idx], 0x00, sizeof(un_Status));
		fidb->stStatusWhen[idx] = 0;
	}

	return 0;
}
