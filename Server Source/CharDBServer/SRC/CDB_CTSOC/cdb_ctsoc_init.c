#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <utillib.h>
#include <server_sock.h>
#include <server_define.h>
#include <server_shm.h>
#include <server_names.h>
#include <init_shm.h>
#include <libhashshm.h>

st_HashedShmHandle stServerHandle;

int JiSTOPFlag, Finishflag;

int g_dSESSQid;

int g_dMySysNo;

void UserControlledSignal(int sign)
{
    JiSTOPFlag = 0;
    Finishflag = sign;
} /* end of UserControlledSignal */

void FinishProgram()
{
    dAppLog(LOG_CRI, "FinishProgram : PROGRAM IS NORMALLY TERMINATED, Cause = %d", Finishflag);
    exit(0);
} /* end of FinishProgram */

void IgnoreSignal(int sign)
{
    if (sign != SIGALRM)
        dAppLog(LOG_CRI,
        "IgnoreSignal : UNWANTED SIGNAL IS RECEIVED, signal = %d", sign);
    signal(sign, IgnoreSignal);
} /* end of IgnoreSignal */


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

    dAppLog(LOG_DEBUG, "SetUpSignal : SIGNAL HANDLER WAS INSTALLED[%d]", JiSTOPFlag);

    return;
} /* end of SetUpSignal */

int dInit_IPC()
{
	int iRet;

	iRet = Init_shm_common();
	if (iRet < 0) {
		printf("[FAIL] Init_shm_common\n");
		exit(-1);
	}

	Init_hashed_shm_handle( &stServerHandle, MAX_CLIENT_CNT, sizeof(st_ServerInfo),
            MAX_CLIENT_CNT, S_SSHM_CHARDBCSOCK );

    iRet = Init_hashed_shm( &stServerHandle );
    if ( iRet < 0 ) {
        dAppLog( LOG_CRI, "[FAIL] Init_hashed_shm(SI): %s", Msg_hashed_shm(iRet) );
        return -1;
    }

	if( (g_dSESSQid = msgget( S_MSGQ_CHAR_RELOCATE, 0666|IPC_CREAT )) < 0 ) // MyQid
    {
        dAppLog( LOG_CRI, "[init_ipcs] [FAIL:%d] MSGGET S_MSGQ_TGD_CTSOC : [%s]\n",
        	errno, strerror(errno) );
        return -1;
    }

	return 0;
}


int dGet_Server_Connection_Info()
{
    FILE    *fa;
    int     i = 0, dSysNo, dRet, dTryFlag;
    char    szBuf[1024];
    char    szSys[24], szIP[16];

	pst_ServerInfo	pstSI;
	UINT64			ullKey;
    
    fa = fopen( IPADDR_FILE,"r" );
    if(fa == NULL) {
        dAppLog( LOG_CRI,"%s : FILE OPEN FAILED (%s)", 
					__func__, IPADDR_FILE, strerror(errno) );
        return -1;
    }

    while( fgets( szBuf, 1024, fa ) != NULL ) {
        if( szBuf[0] != '#' ) {
            dAppLog( LOG_CRI,"%s : %s Format Error",
					__func__, IPADDR_FILE );
            fclose( fa );
            return -2;
        }

        if( szBuf[1] == '#' ) {
            continue;
        } else if( szBuf[1] == 'E' ) {
            break;
		} else if( szBuf[1] == 'S' ) {
			if(sscanf( &szBuf[2],"%d", &dSysNo) == 1 ) {	
				g_dMySysNo = dSysNo;
			}	
        } else if( szBuf[1] == '@' ) {
            if(sscanf( &szBuf[2],"%s %d %s %d", szSys, &dSysNo, szIP, &dTryFlag) == 4 ) {
                if( !strncasecmp( szSys, "CHARDB", 4 ) ) {

					Build_hashed_shm_key( &stServerHandle, 
							(INT64)dSysNo, (INT64)dSysNo, &ullKey );

					dRet = New_hashed_shm( &stServerHandle, ullKey, (void **)&pstSI );
                    if( dRet < 0 ) {
                        dAppLog( LOG_CRI, "ERROR ==> New_hashed_shm : %d", dRet );
                        if( dRet == -17 ) {
                            Get_hashed_shm( &stServerHandle, ullKey, (void **)&pstSI );
                            if ( dRet < 0 ) {
                                dAppLog( LOG_CRI, "%s:Get_hashed_shm KEY:%llx RET: %d msg:%s",
										__func__, ullKey, dRet, Msg_hashed_shm(dRet) );
                            }
                        } else {
							return -1;
                        }
                    }

					memset( pstSI, 0x00, sizeof(st_ServerInfo) );

					pstSI->stSSock.uiServerIPAddr 	= htonl( inet_addr(szIP) );
					pstSI->stSSock.usSvr_Key 		= dSysNo;
					pstSI->stSSock.ullKey 			= ullKey;
					pstSI->dConnTryFlag 			= dTryFlag;
					

					dAppLog( LOG_INFO, "%s ==> IP:%s SYSNO:%d", __func__, szIP, dSysNo );
                    i++;
                }

            }
        }
    }

    fclose( fa );

    if( !i )
    	return -1;

	return 0;
}

void Clear_SockConnection_Hash()
{
    int             iRetval = 0, dRet;

    Browse_hashed_shm_clear( &stServerHandle );

    while( iRetval >=0  && iRetval != HASHED_SHM_SUCCESS_OK ) {
        pst_ServerInfo  pstSI = NULL;
        unsigned long long ullKey = 0;

        iRetval = Browse_hashed_shm ( &stServerHandle, &ullKey, ( void ** ) &pstSI );
        if ( iRetval < 0 ) {
            if( iRetval != HASHED_SHM_ERR_NOT_FOUND ) {
                dAppLog( LOG_CRI,"%s : Browse_hashed_shm: %s ",
                        __func__, Msg_hashed_shm(iRetval) );
            }

            usleep(USLEEP_TIME);
            break;

        }

        if( !ullKey )
            break;

		dRet = Release_hashed_shm( &stServerHandle, pstSI->stSSock.ullKey, NULL );
		if( dRet < 0 ) {
			dAppLog( LOG_CRI, "%s: Ret:%d msg:%s", 	
					__func__, dRet, Msg_hashed_shm(dRet) );
		}
	}
}
