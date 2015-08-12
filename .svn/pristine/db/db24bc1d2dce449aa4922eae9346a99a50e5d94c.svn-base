/*******************************************************************************
				KTF IPAF Project

   Author	: joonhk
   Section	: 
   SCCS ID	: %W%
   Date		: %G%
   Revision History :
   	'03.  1.  6	Initial


   Description :

   Copyright (c) Infravalley 2003
*******************************************************************************/

/**A.1*	FILE INCLUSION ********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <server_shm.h>
#include <utillib.h>
#include <socklib.h>
#include <init_shm.h>
#include <server_define.h>
#include <server_msgq.h>
#include <server_sock.h>
#include <msgdef.h>

/**B.1*	DEFINITION OF NEW CONSTANTS *******************************************/

/**B.2*	DEFINITION OF NEW TYPE ************************************************/

/**C.1*	DECLARATION OF VARIABLES **********************************************/




int dInit_EpollSocket( int *pEFD, UINT uiMaxEvents )
{
    struct epoll_event events;

    *pEFD = epoll_create(uiMaxEvents);
    if( *pEFD < 0 ) {
        dAppLog( LOG_CRI, "%s --> epoll create failed err:%d<%s>", 
				__func__, errno, strerror(errno) );
        return ERR_SOCK_EPOLL_CREATE_FAIL;
    }

    dAppLog( LOG_CRI, "[EPOLL-START] epoll events set success for server" );

    return SOCK_SUCCESS;
}

void EPOLL_ADD( struct epoll_event *event, int *pEFD, int *pSocket )
{
#if 0
    struct epoll_event events;

    /* event control set for read event */
    memset((char *)&events, 0x00, sizeof( struct epoll_event));
    events.events = EPOLLIN | EPOLLERR;
    //events.data.fd = dSocket;
    events.data.ptr = pstCSOCK;
#endif
    
    if( epoll_ctl(*pEFD, EPOLL_CTL_ADD, *pSocket, event) < 0 ) {
    	dAppLog(LOG_DEBUG, "%s --> epoll control failed err:%d<%s>", 
				__func__, errno, strerror(errno));
    }
}

void EPOLL_MODIFY( struct epoll_event *event, int *pEFD, int *pSocket )
{
#if 0
    struct epoll_event events;

    events.events = EPOLLIN | EPOLLERR;
    events.data.ptr = pstCSOCK;
#endif

    if( epoll_ctl(*pEFD, EPOLL_CTL_MOD, *pSocket, event) < 0 ) {
    	dAppLog(LOG_DEBUG, "%s --> epoll control failed err:%d<%s>", 
				__func__, errno, strerror(errno));
	}
}

int dInit_ServerSocket( int *pServerSock, USHORT server_port, UINT uiListenQNum )
{
    struct sockaddr_in serv_addr;
    struct linger       ld;

    ld.l_onoff  = 0;
    ld.l_linger = 0;

    /* Open TCP Socket */
    if( (*pServerSock = socket(AF_INET,SOCK_STREAM,0)) < 0 ) {
        dAppLog(LOG_CRI, "%s --> can't open stream socket err:%d<%s>",
				__func__, errno, strerror(errno));
        return ERR_SOCK_OPEN_FAIL;
    }

    /* Set Socket Option  */
    int nSocketOpt = 1;
    if( setsockopt(*pServerSock, SOL_SOCKET, SO_REUSEADDR, 
					&nSocketOpt, sizeof(nSocketOpt)) < 0 ) {
        dAppLog(LOG_CRI, "%s --> can't set reuse address err:%d<%s>",
				__func__, errno, strerror(errno));
        close(*pServerSock);
        return ERR_SOCK_OPT_FAIL;
    }

    if( setsockopt(*pServerSock, SOL_SOCKET, SO_LINGER, (char *)&ld, sizeof(ld)) < 0 ) {
        dAppLog(LOG_CRI, "%s --> can't set linger err:%d<%s>",
				__func__, errno, strerror(errno));
        close( *pServerSock );
        return ERR_SOCK_OPT_FAIL;
    } /* end of if */

    if( fcntl(*pServerSock, F_SETFL, O_NDELAY) < 0 ) {
        dAppLog(LOG_CRI, "%s --> can't set nodelay err:%d<%s>",
				__func__, errno, strerror(errno));
        close( *pServerSock );
        return ERR_SOCK_OPT_FAIL;
    } /* end of if */

    /* Address Setting */
    memset( &serv_addr , 0 , sizeof(serv_addr) ) ;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(server_port);

    /* Bind Socket */
    if( bind(*pServerSock,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) {
        dAppLog(LOG_CRI, "%s --> can't bind local address err:%d<%s>",
				__func__, errno, strerror(errno));
        close(*pServerSock);
        return ERR_SOCK_BIND_FAIL;
    }

    /* Listening */
	if( uiListenQNum )
    	listen( *pServerSock, uiListenQNum );
	else
    	listen( *pServerSock, 2000 );

    dAppLog( LOG_CRI, "Now Server listening on port %d", server_port );
    return SOCK_SUCCESS;
}

#define DEF_CONN_TRY 3

int dConnect_Server( int *pMySocket, UINT uiServerIPAddr, USHORT server_port )
{
    INT dRet, dReUseAddr, flag;
    INT dBufSize, dConnTry;
    INT nSocketOpt = 1;
    struct sockaddr_in  stServerAddr;

    stServerAddr.sin_family         = AF_INET;
    stServerAddr.sin_addr.s_addr    = ntohl(uiServerIPAddr);
    stServerAddr.sin_port           = htons(server_port);

    if( (*pMySocket = socket(AF_INET, SOCK_STREAM, 0) ) < 0) {
        dAppLog( LOG_CRI, "%s --> can't open stream socket err:%d<%s>",
				__func__, errno, strerror(errno) );
        return ERR_SOCK_OPEN_FAIL;
    }

    if( (flag = fcntl(*pMySocket, F_GETFL, 0)) < 0 ) {
		dAppLog( LOG_CRI, "%s --> can't get FL err:%d<%s>",
				__func__, errno, strerror(errno) );
        close( *pMySocket );
        return ERR_SOCK_OPT_FAIL;
    }

    flag |= O_NONBLOCK;
    if( fcntl(*pMySocket, F_SETFL, flag) < 0 ) {
		dAppLog( LOG_CRI, "%s --> can't set nodelay err:%d<%s>",
				__func__, errno, strerror(errno) );
        close( *pMySocket );
        return ERR_SOCK_OPT_FAIL;
    }


    dReUseAddr = 1;
    if( setsockopt (*pMySocket, SOL_SOCKET, SO_REUSEADDR, 
				(void *)&dReUseAddr, sizeof(INT)) < 0 ) {
        dAppLog( LOG_CRI, "%s --> can't set reuse address err:%d<%s>",
				__func__, errno, strerror(errno) );
        close( *pMySocket );
        return ERR_SOCK_OPT_FAIL;
    }

    dBufSize = DEF_SENDBUF_SIZE;
    if( setsockopt(*pMySocket, SOL_SOCKET, SO_SNDBUF, (void *)&dBufSize, sizeof(dBufSize)) < 0 ) {
		dAppLog( LOG_CRI, "%s --> can't set sndbuf size err:%d<%s>",
				__func__, errno, strerror(errno) );
        close( *pMySocket );
		return ERR_SOCK_OPT_FAIL;
    }

    if( setsockopt(*pMySocket, IPPROTO_TCP, TCP_NODELAY, &nSocketOpt, sizeof(INT)) < 0 ) {
		dAppLog( LOG_CRI, "%s --> can't set nodelay err:%d<%s>",
				__func__, errno, strerror(errno) );
        close( *pMySocket );
        return ERR_SOCK_OPT_FAIL;
    }

    for( dConnTry = 0 ; dConnTry < DEF_CONN_TRY ; dConnTry++) {
        if( (dRet = connect(*pMySocket, (struct sockaddr *)&stServerAddr, 
					sizeof(stServerAddr))) < 0 ) {
            if( errno == EALREADY || errno == EINPROGRESS ) {
                usleep(USLEEP_TIME);
                continue;
            } else if( errno == EISCONN ) {
				dAppLog( LOG_DEBUG, "CONNECTED SERVER IP[%u]", 
						ntohl(stServerAddr.sin_addr.s_addr) );

                break;
            } else {
				dAppLog( LOG_CRI, "RECV CONNECT RET IP:[%s] PORT:[%hu] errno=[%d][%s]\n",
                    CVT_IPADDR(uiServerIPAddr), server_port, errno, strerror(errno) );

                close( *pMySocket );
                return -7;
            }
        } else {
			 dAppLog( LOG_DEBUG, "CONNECT SERVER SUCCESS!! IP:%s PORT:%hu",
                        CVT_IPADDR(uiServerIPAddr), server_port );

            break;
        }

    }

    if( dConnTry >= DEF_CONN_TRY ) {
		dAppLog( LOG_CRI, "CONNECTION RETRY COUNT OVER=[%d] CLOSE [IP]:[%s] [PORT]:[%hu]", 
			dConnTry, CVT_IPADDR(uiServerIPAddr), server_port );
        close( *pMySocket );
        return -8;
    }

    return 0;
}


int dSet_ClientSocket( int newsockfd, UINT uiMaxBuffSize )
{
    struct linger       new_ld;
    int                 flags;


	if( uiMaxBuffSize ) {
		if( setsockopt(newsockfd, SOL_SOCKET, SO_RCVBUF, (int*)&uiMaxBuffSize, sizeof(int)) < 0 ) {
        	dAppLog( LOG_CRI, "%s --> can't set recvbuf size err:%d<%s>",
					__func__, errno, strerror(errno) );
			close( newsockfd );
			return ERR_SOCK_OPT_FAIL;
		}

		if( setsockopt(newsockfd, SOL_SOCKET, SO_SNDBUF, (int*)&uiMaxBuffSize, sizeof(int)) < 0 ) {
        	dAppLog( LOG_CRI, "%s --> can't set sndbuf size err:%d<%s>",
					__func__, errno, strerror(errno) );
			close( newsockfd );
			return ERR_SOCK_OPT_FAIL;
		}
	}

    if( (flags = fcntl(newsockfd, F_GETFL, 0)) < 0 ) {
		dAppLog( LOG_CRI, "%s --> can't get FL err:%d<%s>",
				__func__, errno, strerror(errno) );
        close( newsockfd );
        return ERR_SOCK_OPT_FAIL;
    }

    flags |= O_NDELAY;
    if( fcntl(newsockfd, F_SETFL, flags) < 0 ) {
		dAppLog( LOG_CRI, "%s --> can't set nodelay err:%d<%s>",
				__func__, errno, strerror(errno) );
        close( newsockfd );
        return ERR_SOCK_OPT_FAIL;
    }

    new_ld.l_onoff = 0;
    new_ld.l_linger = 0;
    if( setsockopt(newsockfd, SOL_SOCKET, SO_LINGER, (char *)&new_ld, sizeof (new_ld)) < 0 ) {
		dAppLog( LOG_CRI, "%s --> can't set linger err:%d<%s>",
				__func__, errno, strerror(errno) );
        close( newsockfd );
        return ERR_SOCK_OPT_FAIL;
    } 

    return SOCK_SUCCESS;
}

int dRead_Packet( int dSocket, char *pSPos, UINT uiMaxReadLen )
{
	int n;

	if( !uiMaxReadLen )
		return 0;

	n = read( dSocket, pSPos, uiMaxReadLen );
    if(n <= 0) {
        if( n == 0 ) {
            dAppLog( LOG_CRI,"CLOSE MESSAGE RECEIVE FROM PEER" );
            return ERR_SOCK_PEER_CLOSED;
        } else if( errno != EAGAIN ) {
            dAppLog( LOG_CRI,"READ SOCKERROR, CLOSE SOCKET[%d][%s]", errno, strerror(errno) );
            return ERR_SOCK_RWFAIL;
        } else { //EAGAIN
            dAppLog( LOG_INFO, "EAGAIN!!!!" );
            return ERR_SOCK_EAGAIN;
        }
    }

	return n;
}

int dWrite_Packet( int dSocket, char *pSPos, UINT uiSendLen )
{
    int n;

	if( !uiSendLen )
		return 0;

    if( (n = write(dSocket, pSPos, uiSendLen)) <= 0 ) {
        if(n < 0) {
            if( errno == EAGAIN ) { //EAGAIN
                dAppLog( LOG_INFO, "[sfd]=[%d] [WRITE SOCKET ERROR, EAGAIN errno = %d]", 		
						dSocket, errno );
                return ERR_SOCK_EAGAIN;
            } else {
               dAppLog( LOG_CRI, "[sfd]=[%d] [WRITE SOCKET ERROR errno = %d]", 
						dSocket, errno, strerror(errno) );
               return ERR_SOCK_RWFAIL;
            }
        }

        return ERR_SOCK_WRITE_ZERO;
    } else {
        if ( n != uiSendLen )
            return uiSendLen - n;
    }

    return SOCK_SUCCESS;
}
