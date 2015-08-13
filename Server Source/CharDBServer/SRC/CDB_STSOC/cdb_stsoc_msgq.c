/* 마운트빌 소스코드 UTF-8 Encode */
#include <stdio.h>
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
#include <server_msgq.h>
#include <msgdef.h>
#include <define.h>

extern long long llRcvCnt;

int dIsRcvedMessage(pst_MsgQ pstMsgQ, int dQid)
{
    int     dRet;

    dRet = msgrcv(dQid, pstMsgQ, sizeof(st_MsgQ) - sizeof(long int), 0, IPC_NOWAIT | MSG_NOERROR);
    if(dRet < 0)
    {
        if(errno != EINTR && errno != ENOMSG)
        {
            dAppLog(LOG_DEBUG,"[FAIL:%d] MSGRCV MYQ : [%s]", errno, strerror(errno));
            return -1;      /* Error */
        }

        return 0;   /* Do Nothing */
    }
	#ifdef DEBUG
	else
		dAppLog(LOG_INFO, "Message Received");
	#endif

    return dRet;        /* Good */
} /*** end of dIsRcvedMessage ***/

int dSend_Message( pst_MsgQ pstMsgQ, int qid )
{
    int         isize, dRet;


    isize = sizeof(st_MsgQ) - MAX_MSGBODY_SIZE + pstMsgQ->usBodyLen - sizeof(long int);

    dRet = msgsnd(qid, pstMsgQ, isize, 0);
    if ( dRet < 0) {
        dAppLog( LOG_CRI, "[Qid = %d] ERROR SEND : %d[%s]", qid, errno, strerror(errno));
        return -1;
   	} 
#ifdef DEBUG
	else 
		dAppLog(LOG_INFO, "MESSAGE SEND SUCCESS");

	num++;
	if (old != time(0)) {
		fprintf(stderr, "SEND NUM %d\n", num);
		num = 0;
		old = time(0);
	}
#endif
	
    return 0;
}
