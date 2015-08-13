/* 마운트빌 소스코드 UTF-8 Encode */
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
#include <libhashshm.h>
#include <socklib.h>

extern int g_dipmngqid, g_dEQid[10];
extern st_HashedShmHandle   stClientHandle;
extern int g_epoll_fd;

int dProc_Message(UCHAR *szBody, USHORT usBodyLen, pst_CLIENT_INFO pstINFO);
int dSwitch_Message(pst_MsgQ pstMsgQ);

int dWrite_Message(int dSocket, unsigned char *buf, int dSize);
extern int dSend_Message( pst_MsgQ pstMsgQ, int qid );

extern char *szTempBuf;

int dProc_Message(UCHAR *szBody, USHORT usLength, pst_CLIENT_INFO pstINFO)
{
	pst_PktHdr		pstPH;
	int				dRet, dQid;
	st_MsgQ			stMsgQ;

		
	pstPH = ( pst_PktHdr )szBody;
	if( pstPH->st2ndPH.usSvcID == SID_SYS && pstPH->st2ndPH.usMsgID == MID_SYSTEM_INFO ) {
		pstINFO->ucSysNo = pstPH->st1stPH.usSerial;
		dAppLog( LOG_CRI, "CLIENT SYSTEM NUMBER SETTING : %d", pstINFO->ucSysNo );
		return 0;
	}

	//dQid = g_dipmngqid;

	//dRet = dSend_Message(&stMsgQ, dQid);

	return 0;
}

void MessageWrite_AllClient( unsigned char *szMsg,  USHORT usSendLen )
{
	int iRetval = 0;
	struct epoll_event event;
	pst_CLIENT_INFO	pstINFO;
	pst_CLIENT_SOCK	pstCSOCK;

    while (iRetval >= 0 && iRetval != HASHED_SHM_SUCCESS_OK) {
        unsigned long long ullKey = 0;
        pstINFO = NULL;
        iRetval = Browse_hashed_shm(&stClientHandle, &ullKey, (void **)&pstINFO);
        if( iRetval < 0 ) {
            if( iRetval != HASHED_SHM_ERR_NOT_FOUND ) {
                dAppLog( LOG_CRI, "Browse_hashed_shm: %s\n",
                    Msg_hashed_shm(iRetval) );
            }

            if( atomic_read(&(stClientHandle.pstHeader->aiEntityUsed)) != 0 ) {
                atomic_set( &(stClientHandle.pstHeader->aiEntityUsed), 0 );
            }

            usleep(0);
            break;
        }

        if( ullKey == 0 ) break;

		if( pstINFO->dSendLen + usSendLen < DEF_SENDBUF_SIZE ) {
			memcpy( &pstINFO->szSendBuf[pstINFO->dSendLen], szMsg, usSendLen );
			pstINFO->dSendLen += usSendLen;

			//memset( &event, 0, sizeof( struct epoll_event ) );
            //event.events = EPOLL_OUT_EVENT;
            //event.data.ptr = &pstINFO->stCSOCK;
			EPOLL_OUT_FILL_STR( pstINFO->stCSOCK, event );
			EPOLL_MODIFY( &event, &g_epoll_fd, &pstINFO->stCSOCK.dSocket );
    	} else {
        	dAppLog( LOG_CRI, "%s : NOT ENOUGH SEND BUFFER --> IP:%u MAX:%d CUR_POS:%d REC_SIZE:%hu",
            	__func__, pstINFO->stCSOCK.uiIPAddr, 
				DEF_SENDBUF_SIZE, pstINFO->dSendLen, usSendLen );
    	}
		usleep(USLEEP_TIME);
    }
}

int dHandle_WriteMessage(pst_MsgQ pstMsgQ)
{
	pst_PktHdr		pstPackHeader;
	pst_CLIENT_INFO	pstINFO;
	int				dRet, iRetval = 0;
	struct epoll_event		event;

	pstPackHeader = (pst_PktHdr)&pstMsgQ->szBody[0];

	dAppLog( LOG_INFO, "MESSAGE RECEIVED SVCID:0x%x MSGID:0x%x SESSKEY:%llx SNO:%u LEN:%hu HLEN:%hu",
		pstMsgQ->usSvcID, pstMsgQ->usMsgID,
		pstPackHeader->st1stPH.ullSessID, pstMsgQ->usSystemNo, pstMsgQ->usBodyLen, 
		pstPackHeader->st1stPH.usTotLen );


	if( pstPackHeader->st1stPH.usTotLen != pstMsgQ->usBodyLen ) {
		dAppLog( LOG_CRI, "NOT SAME LENGTH HEADER:%hu MSGQBODY:%hu", 
					pstPackHeader->st1stPH.usTotLen, pstMsgQ->usBodyLen );
		return -1;
	}

	pstPackHeader->st1stPH.usSerial = pstMsgQ->usSystemNo;

	Browse_hashed_shm_clear(&stClientHandle);
	while (iRetval >= 0 && iRetval != HASHED_SHM_SUCCESS_OK) {
        unsigned long long ullKey = 0;
        pstINFO = NULL;
        iRetval = Browse_hashed_shm(&stClientHandle, &ullKey, (void **)&pstINFO);
        if( iRetval < 0 ) {
            if( iRetval != HASHED_SHM_ERR_NOT_FOUND ) {
                dAppLog( LOG_CRI, "Browse_hashed_shm: %s\n",
                    Msg_hashed_shm(iRetval) );
            }

            if( atomic_read(&(stClientHandle.pstHeader->aiEntityUsed)) != 0 ) {
                atomic_set( &(stClientHandle.pstHeader->aiEntityUsed), 0 );
            }

            usleep(0);
			dAppLog(LOG_CRI, "Browse_hashed_shm error iRet:%d", iRetval);
            break;
        }

		//dAppLog(LOG_INFO, "KEY:%llu", ullKey);
        if( ullKey == 0 ) break;

		//dAppLog(LOG_INFO, "SYSNO:%d", pstINFO->ucSysNo);
		if( pstINFO->ucSysNo != pstMsgQ->usSystemNo )
			continue;

		//dAppLog( LOG_INFO, "FIND SERVER!! NO:%d", pstINFO->ucSysNo );
        if( pstINFO->dSendLen + pstMsgQ->usBodyLen < DEF_SENDBUF_SIZE ) {
            memcpy( &pstINFO->szSendBuf[pstINFO->dSendLen], pstMsgQ->szBody, pstMsgQ->usBodyLen );
            pstINFO->dSendLen += pstMsgQ->usBodyLen;

			//memset( &event, 0, sizeof( struct epoll_event ) );
			//event.events = EPOLL_OUT_EVENT;
        	//event.data.ptr = &pstINFO->stCSOCK;

            EPOLL_OUT_FILL_STR( pstINFO->stCSOCK, event );
            EPOLL_MODIFY( &event, &g_epoll_fd, &pstINFO->stCSOCK.dSocket );

        } else {
            dAppLog( LOG_CRI, "%s : NOT ENOUGH SEND BUFFER --> IP:%u MAX:%d CUR_POS:%d REC_SIZE:%hu",
                __func__, pstINFO->stCSOCK.uiIPAddr,
                DEF_SENDBUF_SIZE, pstINFO->dSendLen, pstMsgQ->usBodyLen );
        }

		break;
    }

	return 0;
}

int dWrite_Message(int dSocket, unsigned char *buf, int dSize)
{
    int n;

    if ((n = write(dSocket, buf, dSize)) <= 0) {
        if (n < 0) {
            if (errno == EAGAIN) {
                dAppLog(LOG_INFO, "[sfd]=[%d] [WRITE SOCKET ERROR, EAGAIN errno = %d]", dSocket, errno);
                return -1;
            } else {
               dAppLog(LOG_CRI, "[sfd]=[%d] [WRITE SOCKET ERROR errno = %d]", dSocket, errno);
               return -2;
            }
        }

        return -3;
    } else {
        if ( n != dSize )
            return dSize - n;
    }

    return 0;
}

int dProc_WriteEvent(pst_CLIENT_SOCK pstCSOCK)
{
	struct epoll_event event;
    pst_CLIENT_INFO     pstINFO;
    int dRet;


	EPOLL_IN_FILL_PTR( pstCSOCK, event );
	EPOLL_MODIFY( &event, &g_epoll_fd, &pstCSOCK->dSocket );

    dRet = Get_hashed_shm(&stClientHandle, pstCSOCK->llKey, (void **)&pstINFO);
    if( dRet < 0 ) {
        dAppLog(LOG_CRI, "%s:FAIL Get_hashed_shm --> KEY:%llx RET:%d\n",
                        __func__, pstCSOCK->llKey, dRet);
        return -1;
    }

	dRet = dWrite_Packet(pstCSOCK->dSocket,
                (CHAR*)pstINFO->szSendBuf, pstINFO->dSendLen);
    if( dRet < 0 ) {
        if( dRet != ERR_SOCK_EAGAIN ) {
        	dAppLog(LOG_CRI, "Failed In dWrite_Message LEN:%d RET:%d", pstINFO->dSendLen, dRet);
            pstINFO->dSendLen = 0;
        }
    } else if ( dRet > 0 ) {
		memmove( pstINFO->szSendBuf, pstINFO->szSendBuf + (pstINFO->dSendLen - dRet), dRet );
        pstINFO->dSendLen = dRet;

		event.events = EPOLL_OUT_EVENT;
     	EPOLL_MODIFY( &event, &g_epoll_fd, &pstCSOCK->dSocket );
		//dAppLog( LOG_CRI, "%s ==> Msg Send Success Len:%d", __func__, pstINFO->dSendLen - dRet );
    } else {
		//dAppLog( LOG_CRI, "%s ==> Msg Send Success Len:%d", __func__, pstINFO->dSendLen );
        pstINFO->dSendLen = 0;
    }


    return 0;
}
