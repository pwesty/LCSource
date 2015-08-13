/* 마운트빌 소스코드 UTF-8 Encode */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

#include <server_names.h>
#include <server_msgq.h>
#include <server_sock.h>
#include <server_define.h>
#include <utillib.h>
#include <define.h>
#include <socklib.h>


extern int g_dMyQid, g_DcryptQid;
extern int JiSTOPFlag;

extern void SetUpSignal();
extern int dInit_IPC();
extern void FinishProgram();

extern int dIsRcvedMessage(st_MsgQ *stMsgQ, int dQid);
extern int dSend_Message(st_MsgQ *stMsgQ, int dQid);
extern int dProc_Message(st_MsgQ *stMsgQ);
extern int dSwitch_Message(st_MsgQ *stMsgQ);
extern int dCheck_SocketEvent();
extern void Check_ClientConnection();
extern int dHandle_WriteMessage(pst_MsgQ pstMsgQ);

extern int set_version(int dSeq, char *version);

int			dServerSock;
char 		*szTempBuf;

int			g_dIndex;

extern int 	g_epoll_fd;

char	*version = "R1.0.0";
int main()
{
	int 	dRet;
	st_MsgQ	stMsgQ;
	struct epoll_event event;
	time_t	tCurTime, tOldTime = 0;
	
	Init_logdebug(SEQ_PROC_CDB_STSOC, getpid(), "CDB_STSOC", "/home/SERVERAPP/LOG");

	dRet = Init_shm_common();
    if(dRet < 0) {
        dAppLog( LOG_CRI,"MAIN : Failed in Init_shm_common dRet[%d][%s]",
    	    dRet, strerror(errno));
		FinishProgram();
    }

    dRet = set_version(SEQ_PROC_CDB_STSOC, version);
    if( dRet < 0 )
		FinishProgram();

	SetUpSignal();

	dRet = dInit_IPC();
	if( dRet < 0 )
		FinishProgram();


	 /* init server LISTEN QUEUE : 2000 */
    dRet = dInit_ServerSocket( &dServerSock, S_PORT_CDB_STSOC, 0 );
    if( dRet < 0 ) {
        dAppLog(LOG_CRI, "Init Server Failed --> ret:%d", dRet);
        FinishProgram();
    }

    /* epoll initialize  MAX_EVENT:20000 */
    dRet = dInit_EpollSocket( &g_epoll_fd, 20000 );
    if( dRet < 0 ) {
        dAppLog(LOG_CRI, "Init Epoll Failed --> ret:%d", dRet);
        FinishProgram();
    }

	EPOLL_IN_FILL_INT( dServerSock, event );
	EPOLL_ADD( &event, &g_epoll_fd, &dServerSock );

	g_dIndex = 0 ;

	dRet = set_status_running(SEQ_PROC_CDB_STSOC);
    if(dRet < 0)
		FinishProgram();

	dAppLog(LOG_CRI, "CDB_STSOC PROCESS START!!!");

	while(JiSTOPFlag) {

		dRet = dIsRcvedMessage(&stMsgQ, g_dMyQid);
        if ( dRet > 0 )
            dRet = dHandle_WriteMessage(&stMsgQ);

        dRet = dCheck_SocketEvent();
        tCurTime = time(NULL);

	}

	FinishProgram();
	
	return 0;
}


