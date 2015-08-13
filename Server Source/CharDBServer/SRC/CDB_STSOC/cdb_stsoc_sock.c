#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <utillib.h>
#include <errno.h>
#include <fcntl.h>

#include <server_sock.h>
#include <libhashshm.h>
#include <typedef.h>
#include <socklib.h>
#include <msgdef.h>
#include <define.h>

struct epoll_event g_events[MAX_EVENTS];

extern int 	dServerSock;
extern int 	dConnFlag;
int			dClientSock;


int 	g_dSockNum;
fd_set	g_dReadSet, g_dWriteSet;
int		g_epoll_fd;


extern st_HashedShmHandle	stClientHandle;

extern int dProc_Message( UCHAR *szRecvBuf, USHORT usTotLen, pst_CLIENT_INFO pstINFO );
extern int dProc_WriteEvent( pst_CLIENT_SOCK pstCSOCK );

int dProc_EpollinEvent( pst_CLIENT_SOCK pstCSOCK )
{
    int             n, dSize, sp, dRet;
	pst_PktHdr		pstHeader;
	int				dSockFd;

	pst_CLIENT_INFO		pstINFO;
	struct epoll_event event;
	
		
	dSockFd = pstCSOCK->dSocket;

	dRet = Get_hashed_shm( &stClientHandle, pstCSOCK->llKey, (void **)&pstINFO );
    if( dRet < 0 ) {
        dAppLog( LOG_CRI, "%s: Ret:%d Msg:%s\n", 
				__func__, dRet,  Msg_hashed_shm(dRet) );
        return -1;
    }

	dSize  = pstINFO->dRecvLen;

	n = dRead_Packet( dSockFd, (char *)(pstINFO->szRecvBuf + dSize), 
							DEF_RECVBUF_SIZE - dSize - 1 );
    if ( n < 0 ) {
        if( n != ERR_SOCK_EAGAIN )  { // if not eagain
            return -1;
        }
		return 0; // EAGAIN
    }

    if( dSize + n < PACK_HEADER_LEN ) {
		pstINFO->dRecvLen = dSize + n;
    } else {
		dSize += n;
		sp = 0;

		do { 
			pstHeader = (pst_PktHdr)(pstINFO->szRecvBuf + sp);

			if( pstHeader->st1stPH.usTotLen < PACK_HEADER_LEN ) {
				dAppLog( LOG_INFO, "INVALID CASE ::: LESS THAN HEADER LENGTH [TOT]:[%hu]", 
							pstHeader->st1stPH.usTotLen );
				break;
			}

            if ( (pstHeader->st1stPH.usTotLen <= dSize - sp) ) {
				if( pstHeader->st2ndPH.usSvcID == SID_SYS &&
                    pstHeader->st2ndPH.usMsgID == MID_SOCKCHK_REQ ) { //Check Alive
                    if( pstINFO->dRecvLen + PACK_HEADER_LEN < DEF_SENDBUF_SIZE ) {
						//pstINFO->ucSysNo = pstHeader->st1stPH.usSerial;
                        pstHeader->st2ndPH.usMsgID = MID_SOCKCHK_RESP;
                        memcpy( pstINFO->szSendBuf + pstINFO->dSendLen, pstHeader, PACK_HEADER_LEN );
                        pstINFO->dSendLen += PACK_HEADER_LEN;
                        //dAppLog(LOG_INFO, "SOCK CHECK REQ IP:%s", CVT_IPADDR(pstINFO->stCSOCK.uiIPAddr));

                        EPOLL_OUT_FILL_PTR( pstCSOCK, event );
                        EPOLL_MODIFY( &event, &g_epoll_fd, &pstCSOCK->dSocket );
                    }
                } else {
                    dRet = dProc_Message( (UCHAR *)(pstINFO->szRecvBuf + sp),
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
            memcpy( pstINFO->szRecvBuf, pstINFO->szRecvBuf + sp, dSize - sp );
        } else {
            pstINFO->dRecvLen = 0;
        }
	}

    return 1;
}


int dCheck_SocketEvent()
{
	struct sockaddr_in cli_addr;
    int i, nfds, dRet;
	int dcli_len = sizeof(cli_addr);
	unsigned long long llKey;
	pst_CLIENT_SOCK	pstCSOCK;
	pst_CLIENT_INFO	pstClientInfo;
	struct epoll_event events;

	nfds = epoll_wait( g_epoll_fd, g_events, MAX_EVENTS, USLEEP_TIME );
	if( nfds == 0 ) {
		return 0; /* no events */
	} else if( nfds < 0 ) {
		dAppLog( LOG_CRI, "EPOLL WATI ERROR OCCURED [%d:%s]", errno, strerror(errno) );
		return -1;
	} else {
		for( i = 0 ; i < nfds ; i++ ) {
			if( g_events[i].data.fd == dServerSock ) { // server socket event 
				dClientSock = accept( dServerSock, (struct sockaddr *)&cli_addr,(socklen_t *)&dcli_len );
				if( dClientSock < 0 ) { /* accept error */
                	if( errno != EAGAIN ) {
                    	dAppLog( LOG_CRI, "ACCEPT ERROR [%d][%s]", errno, strerror(errno) );
					}
                } else {
					dAppLog( LOG_DEBUG, "NEW CLIENT CONNECTION [IP:%s][PORT:%hu]", 
							inet_ntoa(cli_addr.sin_addr), htons(cli_addr.sin_port) );

					dRet = dSet_ClientSocket( dClientSock, DEF_CSOCKBUF_SIZE );
                	if( dRet < 0 ) {
                    	dAppLog(LOG_CRI, "FAILED IN dSet_ClientSocket RET:%d", dRet);
                    	continue;
                	}

					Build_hashed_shm_key( &stClientHandle, (INT64)cli_addr.sin_addr.s_addr, 
								(INT64)htons(cli_addr.sin_addr.s_addr), &llKey );
                
                	dRet = New_hashed_shm( &stClientHandle, llKey, (void **)&pstClientInfo );
                	if( dRet < 0 ) {   
                    	dAppLog( LOG_CRI, "ERROR ==> New_hashed_shm : %d", dRet );
						if( dRet == -17 ) {
							dRet = Get_hashed_shm( &stClientHandle, llKey, (void **)&pstClientInfo );
    						if ( dRet < 0 ) {
        						dAppLog( LOG_CRI, "Get_hashed_shm KEY:%llx RET: %d",
                						llKey, dRet );
                    			close( dClientSock );
    						}
						} else {
                    		close( dClientSock );
                    		continue;
						}
                	}
					
					memset( pstClientInfo, 0x00, sizeof(st_CLIENT_INFO) );

					pstClientInfo->stCSOCK.llKey 	= llKey;	
					pstClientInfo->stCSOCK.dSocket 	= dClientSock;

					pstClientInfo->stCSOCK.uiIPAddr = htonl(cli_addr.sin_addr.s_addr);
                    pstClientInfo->stCSOCK.usPort   = htons(cli_addr.sin_port);
					pstClientInfo->stCSOCK.usTemsNo = 0;
					pstClientInfo->dLoginStat 		= DEF_CONNECT;

					pstClientInfo->tAliveChkTime	= time(NULL);
					pstClientInfo->tAliveRcvTime	= time(NULL);

					pst_CLIENT_SOCK pstCSOCK = &pstClientInfo->stCSOCK;
					EPOLL_IN_FILL_PTR( pstCSOCK, events );
					EPOLL_ADD( &events, &g_epoll_fd, &dClientSock );
				}
               continue;
            }

			if( g_events[i].events & (EPOLLERR | EPOLLHUP) ) {	// error 
            	dAppLog( LOG_CRI, "EPOLL ERROR EVENT OCCURED!!" );

				pstCSOCK = g_events[i].data.ptr;

				dAppLog( LOG_CRI, "CLIENT DISCONNECTION [IP:%s][PORT:%hu]",
                        CVT_IPADDR(pstCSOCK->uiIPAddr), pstCSOCK->usPort );
	
            	close( pstCSOCK->dSocket );
				dRet = Release_hashed_shm( &stClientHandle, pstCSOCK->llKey, NULL );
    			if ( dRet < 0 ) {
        			dAppLog( LOG_CRI, "Release_hashed_shm: %d\n", dRet );
    			}


				return -2;
        	}

        	/* if not server socket , this socket is for client socket, so we read it */
        	if( g_events[i].events & EPOLLIN ) {
				pstCSOCK = g_events[i].data.ptr;

            	dRet = dProc_EpollinEvent( pstCSOCK );
				if( dRet < 0 ) {
					dAppLog( LOG_CRI, "CLIENT DISCONNECTION [IP:%s][PORT:%hu]",
                        CVT_IPADDR(pstCSOCK->uiIPAddr), pstCSOCK->usPort );

					close( pstCSOCK->dSocket );
					dRet = Release_hashed_shm( &stClientHandle, pstCSOCK->llKey, NULL );
    				if( dRet < 0 ) {
        				dAppLog( LOG_CRI, "Release_hashed_shm: %d\n", dRet );
    				}

				}
			}

			
        	if ( g_events[i].events & EPOLLOUT ) { // write event
            	pstCSOCK = g_events[i].data.ptr;

				//dAppLog(LOG_INFO, "EPOLL OUT EVENT OCCURED!");
				dRet = dProc_WriteEvent( pstCSOCK );
                if( dRet < 0 ) {
					close( pstCSOCK->dSocket );
                    dRet = Release_hashed_shm( &stClientHandle, pstCSOCK->llKey, NULL );
                    if( dRet < 0 ) {
                        dAppLog( LOG_CRI, "Release_hashed_shm: %d\n", dRet );
                    }
                }
        	}
		}
	}

    return 1;

}

void Check_ClientConnection( time_t tCurTime )
{
    int             	iRetval = 0, dCount = 0;
	pst_CLIENT_SOCK		pstCSOCK;
	struct epoll_event 	event;
	st_PktHdr			stPH;

	
    Browse_hashed_shm_clear( &stClientHandle );

    while( iRetval >=0  && iRetval != HASHED_SHM_SUCCESS_OK ) {
        pst_CLIENT_INFO  pstCI = NULL;
        unsigned long long ullKey = 0;

        iRetval = Browse_hashed_shm ( &stClientHandle, &ullKey, ( void ** ) &pstCI );
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

