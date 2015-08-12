#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

#include <server_names.h>
#include <server_msgq.h>
#include <server_define.h>
#include <utillib.h>
#include <define.h>
#include <socklib.h>


extern int g_dMyQid, g_DcryptQid;
extern int JiSTOPFlag;
extern int g_epoll_fd;

extern void SetUpSignal();
extern int dInit_IPC();
extern void FinishProgram();

extern int dIsRcvedMessage(st_MsgQ *stMsgQ, int dQid);
extern int dSend_Message(st_MsgQ *stMsgQ, int dQid);
extern int dCheck_SocketEvent();
extern int dGet_Server_Connection_Info();
extern void Check_SockConnection_Status();
extern void Clear_SockConnection_Hash();

extern int set_version(int dSeq, char *version);


char	*version = "R1.0.0";


int packet_dump( UCHAR* pkt, int size )
{
    int i;

    for ( i = 0; i < size; i++ )
	{
        fprintf( stderr, "%02X ", *( pkt + i ) );
        if ( ( ( i + 1 ) % 16) == 0 )
			fprintf(stderr, "\n");
    }
    if ( ( ( i + 1 ) % 16 ) != 0 )
		fprintf( stderr, "\n" );
    
    return 0;
}


int main()
{
	int 	dRet, i, j;
	st_MsgQ stMsgQ;
	time_t	tCurTime, tLastTime = 0;

    Init_logdebug( SEQ_PROC_CDB_CTSOC, getpid(), "CDB_CTSOC", "/home/SERVERAPP/LOG" );

	dRet = Init_shm_common();
    if(dRet < 0) {
        dAppLog( LOG_CRI,"MAIN : Failed in Init_shm_common dRet[%d][%s]",
        dRet, strerror(errno));
        FinishProgram();
    }

    dRet = set_version(SEQ_PROC_CDB_CTSOC, version);
    if( dRet < 0 )
        FinishProgram();

	SetUpSignal();

	dRet = dInit_IPC();
	if( dRet < 0 )
		FinishProgram();

	Clear_SockConnection_Hash();

	dRet = dGet_Server_Connection_Info();
    if( dRet < 0 )
        FinishProgram();

	/* epoll initialize  MAX_EVENT:20000 */
    dRet = dInit_EpollSocket( &g_epoll_fd, 20000 );
    if( dRet < 0 ) {
        dAppLog(LOG_CRI, "Init Epoll Failed --> ret:%d", dRet);
        FinishProgram();
    }

	dRet = set_status_running(SEQ_PROC_CDB_CTSOC);
    if(dRet < 0)
		FinishProgram();

	dAppLog(LOG_CRI, "CDB_CTSOC PROCESS START!!!");
	
	while( JiSTOPFlag ) {

		tCurTime = time(NULL);
		if( tCurTime != tLastTime ) {
			Check_SockConnection_Status(tCurTime);
			tLastTime = tCurTime;
		}

#if 0
		for( j = 100 ; j ; j-- ) {
			dRet = dIsRcvedMessage( &stMsgQ, g_dMyQid );
			if( dRet > 0 ) {
				dProc_Message( &stMsgQ );
			} else {
				usleep(USLEEP_TIME);
				break;
			} 
		}

#endif
		if( !dCheck_SocketEvent() )
			usleep(USLEEP_TIME);

	}

	FinishProgram();
	
	return 0;
}


