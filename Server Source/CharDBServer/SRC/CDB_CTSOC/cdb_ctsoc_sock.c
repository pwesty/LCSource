#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <utillib.h>
#include <errno.h>
#include <fcntl.h>

#include <server_sock.h>
#include <typedef.h>
#include <init_shm.h>
#include <socklib.h>
#include <libhashshm.h>
#include <server_define.h>
#include <msgdef.h>

struct epoll_event g_events[MAX_EVENTS];

extern st_HashedShmHandle stServerHandle;

int 	g_dSockNum;
int		g_epoll_fd;

extern int g_dMySysNo;

extern int dHandle_RecvMessage( UCHAR *szRecvBuf, USHORT usTotLen, pst_ServerInfo pstSI );

void Send_SocketCheck_Message( pst_ServerInfo pstSI )
{
	st_PktHdr           stHeader;
    struct epoll_event  event;

    set_pheader_content( &stHeader, SID_SYS, MID_SOCKCHK_REQ, PACK_HEADER_LEN, 0, 0 )
	stHeader.st1stPH.usSerial = g_dMySysNo;

	if( pstSI->dSendLen + PACK_HEADER_LEN < DEF_SENDBUF_SIZE ) {

		memcpy( pstSI->szSendBuf + pstSI->dSendLen, &stHeader, PACK_HEADER_LEN );
		pstSI->dSendLen += PACK_HEADER_LEN;

    	EPOLL_OUT_FILL_STR( pstSI->stSSock, event );
    	EPOLL_MODIFY( &event, &g_epoll_fd, &pstSI->stSSock.dSocket );
	}
}

void Send_SystemInfo_Message( pst_ServerInfo pstSI )
{
    st_PktHdr           stHeader;
    struct epoll_event  event;

    set_pheader_content( &stHeader, SID_SYS, MID_SYSTEM_INFO, PACK_HEADER_LEN, 0, 0 )
    stHeader.st1stPH.usSerial = g_dMySysNo;

    if( pstSI->dSendLen + PACK_HEADER_LEN < DEF_SENDBUF_SIZE ) {

        memcpy( pstSI->szSendBuf + pstSI->dSendLen, &stHeader, PACK_HEADER_LEN );
        pstSI->dSendLen += PACK_HEADER_LEN;

        EPOLL_OUT_FILL_STR( pstSI->stSSock, event );
        EPOLL_MODIFY( &event, &g_epoll_fd, &pstSI->stSSock.dSocket );
    }
}

void Check_SockConnection_Status( time_t tCurTime )
{
    int             iRetval = 0, dRet;
    struct epoll_event event;

    Browse_hashed_shm_clear( &stServerHandle );

    while( iRetval >=0  && iRetval != HASHED_SHM_SUCCESS_OK ) {
        pst_ServerInfo  pstSI = NULL;
        unsigned long long ullKey = 0;

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

		if( !pstSI->dConnFlag && pstSI->dConnTryFlag && pstSI->tConnTryTime + 3 < tCurTime ) { 
			// disconnect && try mode && 3 seconds pass 
            dRet = dConnect_Server( &pstSI->stSSock.dSocket,
                        pstSI->stSSock.uiServerIPAddr, S_PORT_CDB_STSOC );
            pstSI->tConnTryTime = tCurTime;
            if( !dRet ) { // if 0 returned
                pstSI->dConnFlag = DEF_CONNECT;
                pstSI->dSendLen = pstSI->dRecvLen = 0;
				pstSI->tAliveChkTime = pstSI->tAliveRcvTime = tCurTime;

				pst_ServerSock pstSSock = &pstSI->stSSock;
                EPOLL_IN_FILL_PTR( pstSSock, event );
                EPOLL_ADD( &event, &g_epoll_fd, &pstSI->stSSock.dSocket );
            	Send_SystemInfo_Message( pstSI );
            }
        }

		if( pstSI->dConnFlag ) {

			if( pstSI->tAliveRcvTime + 60 <= tCurTime ) { // close socket
            	if( pstSI->stSSock.dSocket > 0 &&
                	(fcntl(pstSI->stSSock.dSocket, F_GETFL, 0) > 0) )
                	close( pstSI->stSSock.dSocket );
            	pstSI->dRecvLen = 0;
            	pstSI->dSendLen = 0;
            	pstSI->dConnFlag = DEF_DISCONNECT;
        	}

        	if( pstSI->tAliveChkTime + 10 <= tCurTime && pstSI->dConnFlag ) {
            	Send_SocketCheck_Message( pstSI );
				pstSI->tAliveChkTime = tCurTime;
				//dAppLog(LOG_INFO, "SOCK CHECK REQ SEND IP:%s", CVT_IPADDR(pstSI->stSSock.uiServerIPAddr));
			}

		}

    }
}

int dProc_WriteEvent(pst_ServerSock pstSSock)
{
	struct epoll_event	event;
	int dRet;

	pst_ServerInfo	pstINFO;


	dRet = Get_hashed_shm( &stServerHandle, pstSSock->ullKey, (void **)&pstINFO );
    if( dRet < 0 ) {
        dAppLog( LOG_CRI, "%s --> KEY:%llx RET:%d msg:%s",
						__func__, pstSSock->ullKey, dRet, Msg_hashed_shm(dRet) );
        return -1;
    }
	
	if( !pstINFO->dConnFlag ) { //NOT CONNECTED
		dAppLog(LOG_CRI, "%s --> CONNECTION CLOSED!!", __func__);
		pstINFO->dSendLen = 0;
		return -1;
	}

	EPOLL_IN_FILL_PTR(pstSSock, event);
    EPOLL_MODIFY( &event, &g_epoll_fd, &pstSSock->dSocket );


	dRet = dWrite_Packet( pstINFO->stSSock.dSocket, (CHAR*)pstINFO->szSendBuf, pstINFO->dSendLen );
    if( dRet < 0 ) {
        if( dRet != ERR_SOCK_EAGAIN ) {
            pstINFO->dSendLen = 0;
            dAppLog( LOG_CRI, "Failed In dWrite_Message LEN:%d RET:%d", pstINFO->dSendLen, dRet );
        }
    } else if ( dRet > 0 ) {
        memmove( pstINFO->szSendBuf, pstINFO->szSendBuf + (pstINFO->dSendLen - dRet), dRet );
        pstINFO->dSendLen = dRet;

		event.events = EPOLL_OUT_EVENT;
        EPOLL_MODIFY( &event, &g_epoll_fd, &pstSSock->dSocket );
    } else {
        pstINFO->dSendLen = 0;
    }

	return 0;
}

int dProc_ReadEvent( pst_ServerSock pstSSock )
{
    int             n, sp, dSize, dSocket, dRet;
	pst_ServerInfo	pstINFO;
	pst_PktHdr		pstHeader;
	

	dSocket = pstSSock->dSocket;

	dRet = Get_hashed_shm( &stServerHandle, pstSSock->ullKey, (void **)&pstINFO );
    if( dRet < 0 ) {
		dAppLog( LOG_CRI, "%s: Ret:%d Msg:%s\n", __func__, 
				dRet,  Msg_hashed_shm(dRet) );
        return -1;
	}

	dSize = pstINFO->dRecvLen;

	n = dRead_Packet( dSocket, (char *)pstINFO->szRecvBuf + dSize,
						__DEF_BUFFER_SIZE - dSize - 1 );
	
	if( n <= 0 ) {
		if( n != ERR_SOCK_EAGAIN ) {
			pstINFO->dSendLen = 0;
			pstINFO->dRecvLen = 0;
			pstINFO->dConnFlag = DEF_DISCONNECT;
			close( dSocket );
			return -1;
		}
		return 0;
	}

	if( dSize + n < PACK_HEADER_LEN ) {
        pstINFO->dRecvLen = dSize + n;
    } else {
        dSize += n;
        sp = 0;

        do {
            pstHeader = (pst_PktHdr)&pstINFO->szRecvBuf[sp];

            if( pstHeader->st1stPH.usTotLen < PACK_HEADER_LEN ) {
                dAppLog(LOG_INFO, "INVALID CASE ::: LESS THAN HEADER LENGTH [TOT]:[%hu]",
                            pstHeader->st1stPH.usTotLen);
                break;
            }

            if ( pstHeader->st1stPH.usTotLen <= dSize - sp ) {
				if( pstHeader->st2ndPH.usSvcID == SID_SYS &&
					pstHeader->st2ndPH.usMsgID == MID_SOCKCHK_RESP ) {
					pstINFO->tAliveRcvTime = time(NULL);
					//dAppLog(LOG_INFO, "SOCK CHECK RESP IP:%s", 
					//		CVT_IPADDR(pstINFO->stSSock.uiServerIPAddr));
				} else {
					dRet = dHandle_RecvMessage( (UCHAR *)(pstINFO->szRecvBuf + sp), 
							pstHeader->st1stPH.usTotLen, pstINFO );
				}

                sp += pstHeader->st1stPH.usTotLen;
            } else {
                break;
            }

        } while ( dSize >= sp + PACK_HEADER_LEN );

        /* Buffer set */
        if ( dSize > sp ) {
            pstINFO->dRecvLen = dSize - sp;
            memmove( &pstINFO->szRecvBuf[0], &pstINFO->szRecvBuf[sp], dSize - sp );
        } else {
            pstINFO->dRecvLen = 0;
        }
    }

    return 0;
}


int dCheck_SocketEvent()
{
    int i, nfds, iRet, dIndex;
	pst_ServerInfo	pstSI;
	pst_ServerSock	pstSSock;

	nfds = epoll_wait( g_epoll_fd, g_events, MAX_EVENTS, 0 );
	if( nfds == 0 ) {
		return 0; /* no events */
	} else if( nfds < 0 ) {
		dAppLog( LOG_CRI, "EPOLL WATI ERROR OCCURED [%d:%s]", errno, strerror(errno) );
		return -1;
	} else {
		for( i = 0 ; i < nfds ; i++ ) {
			if( g_events[i].events & (EPOLLERR | EPOLLHUP) ) {
            	dAppLog(LOG_CRI, "EPOLL ERROR EVENT OCCURED!!");
				
				pstSSock = g_events[i].data.ptr;
				dAppLog( LOG_CRI, "CLOSE SOCKET [%s]", 
								CVT_IPADDR(pstSSock->uiServerIPAddr) );

            	close( pstSSock->dSocket );

				iRet = Get_hashed_shm( &stServerHandle, pstSSock->ullKey, (void **)&pstSI );
				if (iRet < 0) {
					dAppLog( LOG_CRI, "Get_hashed_shm KEY:%llx RET: %d",
							pstSSock->ullKey, iRet );
				} else {
					pstSI->dSendLen = 0;
					pstSI->dRecvLen = 0;
					pstSI->dConnFlag = DEF_DISCONNECT;
				}
				return -2;
        	}

        	/* if not server socket , this socket is for client socket, so we read it */
        	if( g_events[i].events & EPOLLIN ) {
				pstSSock = g_events[i].data.ptr;
				iRet = dProc_ReadEvent( pstSSock );
			}

			if( g_events[i].events & EPOLLOUT ) {
				pstSSock = g_events[i].data.ptr;

				iRet = dProc_WriteEvent( pstSSock );
			}
		}
	}

    return 0;

} 

#if 0
void Check_ClientConnection( time_t tCurTime )
{
    int                 iRetval = 0, dCount = 0;
    pst_CLIENT_SOCK     pstCSOCK;
    struct epoll_event  event;
    st_PktHdr           stPH;


    Browse_hashed_shm_clear( &stServerHandle );

    while( iRetval >=0  && iRetval != HASHED_SHM_SUCCESS_OK ) {
        pst_CLIENT_INFO  pstCI = NULL;
        unsigned long long ullKey = 0;

        iRetval = Browse_hashed_shm ( &stServerHandle, &ullKey, ( void ** ) &pstCI );
        if ( iRetval < 0 ) {
            if( iRetval != HASHED_SHM_ERR_NOT_FOUND ) {
                dAppLog( LOG_CRI,"%s : Browse_hashed_shm: %s ",
                        __func__,
                        Msg_hashed_shm(iRetval) );
            }

            usleep(0);
            break;

        }
        if( !ullKey )
            break;

        if( !pstCI->dLoginStat ) // not connected
            continue;

        if( pstCI->tAliveChkTime + 10 > tCurTime ) // check alive per 10 seconds
            continue;

        if( pstCI->tAliveRcvTime + 60 < tCurTime ) {
            // not receive alive response packet for 60 seconds, so close connection

            if( pstCI->stCSOCK.dSocket > 0 &&
                (fcntl(pstCI->stCSOCK.dSocket, F_GETFL, 0) > 0) )
                close(pstCI->stCSOCK.dSocket);

            pstCI->dLoginStat = DEF_DISCONNECT;
            pstCI->dSendLen = pstCI->dRecvLen = 0;
            continue;
        }

        if( !dCount ) { // first sending 
            memset( &stPH, 0x00, sizeof(st_PktHdr) );

            stPH.st1stPH.llMagicNumber = MAGIC_NUMBER;
            stPH.st2ndPH.usSvcID = SID_SYS;
            stPH.st2ndPH.usMsgID = MID_SOCKCHK_REQ;
            stPH.st1stPH.usTotLen = PACK_HEADER_LEN;
        }

        if( pstCI->dSendLen + PACK_HEADER_LEN >= DEF_SENDBUF_SIZE ) // buffer over flow
            continue;
        memcpy( pstCI->szSendBuf + pstCI->dSendLen, &stPH, PACK_HEADER_LEN );
        pstCI->dSendLen += PACK_HEADER_LEN;

        pstCI->tAliveChkTime = tCurTime;

        pstCSOCK = &pstCI->stCSOCK;
        EPOLL_OUT_FILL_PTR( pstCSOCK, event );
        EPOLL_MODIFY( &event, &g_epoll_fd, &pstCI->stCSOCK.dSocket );
        dAppLog(LOG_INFO, "CHECK CLIENT SOCKET");
        dCount++;
    }
}
#endif
