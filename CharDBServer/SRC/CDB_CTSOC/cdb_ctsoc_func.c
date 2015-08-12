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
#include <sys/select.h>
#include <sys/epoll.h>

#include <utillib.h>
#include <server_sock.h>
#include <server_define.h>
#include <server_shm.h>
#include <server_names.h>
#include <typedef.h>
#include <msgdef.h>
#include <server_msgq.h>
#include <socklib.h>
#include <libhashshm.h>


extern st_HashedShmHandle stServerHandle;
extern int g_epoll_fd;
extern int g_dSESSQid;

extern int dSend_Message(pst_MsgQ pstMsgQ, int dQid);

int dProc_Message( pst_MsgQ pstMsgQ )
{
	int				iRetval =0;
	pst_ServerInfo	pstSI;
	UINT64			ullKey;
	USHORT			usSvcID, usMsgID;
	struct epoll_event event;

	usSvcID = pstMsgQ->usSvcID;
	usMsgID = pstMsgQ->usMsgID;

	dAppLog(LOG_INFO, "%s --> SID:0x%x MID:0x%x LEN:%hu", __func__,
			usSvcID, usMsgID, pstMsgQ->usBodyLen);

	if( usSvcID == SID_GDMS_TEMS_CSMS ) {
		//all gdms send
		Browse_hashed_shm_clear( &stServerHandle );
		while( iRetval >=0  && iRetval != HASHED_SHM_SUCCESS_OK ) {
			pstSI = NULL;
			ullKey = 0;

        	iRetval = Browse_hashed_shm ( &stServerHandle, &ullKey, ( void ** ) &pstSI );
        	if ( iRetval < 0 ) {
            	if( iRetval != HASHED_SHM_ERR_NOT_FOUND ) {
                	dAppLog( LOG_CRI,"%s : Browse_hashed_shm: %s ",
                        __func__,
                        Msg_hashed_shm(iRetval) );
            	}

				usleep(USLEEP_TIME);
				break;
        	}

			if( !ullKey )
            	break;
			
			if( pstSI->dSendLen + pstMsgQ->usBodyLen >= __DEF_BUFFER_SIZE )
				continue;

			memcpy( pstSI->szSendBuf + pstSI->dSendLen, pstMsgQ->szBody, pstMsgQ->usBodyLen ); 
			pstSI->dSendLen += pstMsgQ->usBodyLen;
			EPOLL_OUT_FILL_STR( pstSI->stSSock, event)
			EPOLL_MODIFY( &event, &g_epoll_fd, &pstSI->stSSock.dSocket );
		}

	} else {
		;
	}

	
	return 0;
}

int dHandle_RecvMessage( UCHAR* szBody, USHORT usTotLen, pst_ServerInfo pstSI )
{
	pst_PktHdr      pstPH;
	int				dRet;
    st_MsgQ         stMsgQ;
	struct epoll_event event;

    pstPH = ( pst_PktHdr )&szBody[0];

	dAppLog( LOG_CRI, "MESSAGE RECEIVED --> SID:0x%x MID:0x%x LEN:%hu", 
		pstPH->st2ndPH.usSvcID, 
		pstPH->st2ndPH.usMsgID, pstPH->st1stPH.usTotLen );

	memcpy( stMsgQ.szBody, szBody, pstPH->st1stPH.usTotLen );
	stMsgQ.usBodyLen = pstPH->st1stPH.usTotLen;
	stMsgQ.usSvcID = pstPH->st2ndPH.usSvcID;
	stMsgQ.usMsgID = pstPH->st2ndPH.usMsgID;
	stMsgQ.usSystemNo = pstPH->st1stPH.usSerial;

	dSend_Message( &stMsgQ, g_dSESSQid );
	
	return 0;
}
