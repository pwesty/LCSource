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


extern int g_dMyQid;

int dIsRcvedMessage( pst_MsgQ pstMsgQ, int dQid )
{
	int iRet;

	iRet = msgrcv( dQid, pstMsgQ, sizeof( st_MsgQ ) - sizeof( long int ), 0,
				IPC_NOWAIT | MSG_NOERROR );
	if ( iRet < 0 )
	{
		if ( errno != EINTR && errno != ENOMSG )
		{
			dAppLog( LOG_DEBUG, "[FAIL:%d] MSGRCV MYQ : [%s]",
					errno, strerror( errno ) );
			return -1;	/* Error */
		}

		return 0;		/* Do Nothing */
	}

	dAppLog(LOG_CRI, "RECV MESS: qid:%d size:%d", dQid, iRet);
	return iRet;		/* Good */
} /*** end of dIsRcvedMessage ***/

int dSend_Message( pst_MsgQ pstMsgQ, int qid )
{
	int isize, iRet;

	isize = sizeof( st_MsgQ ) - MAX_MSGBODY_SIZE + pstMsgQ->usBodyLen
		- sizeof( long int );

	iRet = msgsnd( qid, pstMsgQ, isize, 0 );
	if ( iRet < 0 )
	{
		dAppLog( LOG_CRI, "[Qid = %d] ERROR SEND : %d[%s]", qid, errno,
				strerror( errno ) );
		return -1;
	}

	return 0;
}
