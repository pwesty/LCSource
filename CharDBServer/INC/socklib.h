#ifndef __SOCKLIB_HEADER_FILE___
#define __SOCKLIB_HEADER_FILE___

#include <typedef.h>
#include <sys/epoll.h>

#define EPOLL_IN_EVENT	EPOLLIN | EPOLLERR
#define EPOLL_OUT_EVENT	EPOLLIN | EPOLLOUT | EPOLLERR

#define SOCK_SUCCESS					0
#define ERR_SOCK_EAGAIN					-1
#define ERR_SOCK_RWFAIL					-2
#define ERR_SOCK_PEER_CLOSED			-3
#define ERR_SOCK_WRITE_ZERO				-4
#define ERR_SOCK_EPOLL_CREATE_FAIL		-5
#define ERR_SOCK_EPOLL_ADD_FAIL			-6
#define ERR_SOCK_OPEN_FAIL				-7
#define ERR_SOCK_OPT_FAIL				-8
#define ERR_SOCK_BIND_FAIL				-9

#define KEY_SRV_GDMS					1000
#define KEY_SRV_GDMS1					1001
#define KEY_SRV_GDMS2					1002

#define KEY_SRV_TEMS					2000
#define KEY_SRV_TEMS1					2001
#define KEY_SRV_TEMS2					2002
#define KEY_SRV_TEMS3					2003

#define KEY_SRV_GDBS					3000
#define KEY_SRV_GDBS1					3001

#define KEY_SVR_GSMS					4000
#define KEY_SVR_GSMS1					4001

#define KEY_SVR_SDBS					5000
#define KEY_SVR_SDBS1					5001



#define MAX_EVENTS			20000
#define DEF_CSOCKBUF_SIZE	1048576

#define EPOLL_IN_FILL_PTR( x, event ) \
		memset( &event, 0, sizeof( struct epoll_event ) ); \
		event.events = EPOLL_IN_EVENT; \
    	event.data.ptr = x; \
 
#define EPOLL_OUT_FILL_PTR( x, event ) \
		memset( &event, 0, sizeof( struct epoll_event ) ); \
		event.events = EPOLL_OUT_EVENT; \
    	event.data.ptr = x; \
 
#define EPOLL_IN_FILL_STR( x, event ) \
        memset( &event, 0, sizeof( struct epoll_event ) ); \
        event.events = EPOLL_IN_EVENT; \
        event.data.ptr = &x; \
 
#define EPOLL_OUT_FILL_STR( x, event ) \
        memset( &event, 0, sizeof( struct epoll_event ) ); \
        event.events = EPOLL_OUT_EVENT; \
        event.data.ptr = &x; \
 
#define EPOLL_IN_FILL_INT( x, event ) \
		memset( &event, 0, sizeof( struct epoll_event ) ); \
		event.events = EPOLL_IN_EVENT; \
    	event.data.fd = x; \
 
#define EPOLL_OUT_FILL_INT( x, event ) \
		memset( &event, 0, sizeof( struct epoll_event ) ); \
		event.events = EPOLL_OUT_EVENT; \
    	event.data.fd = x; \
 
#define EPOLL_IN_FILL_UINT( x, event ) \
		memset( &event, 0, sizeof( struct epoll_event ) ); \
		event.events = EPOLL_IN_EVENT; \
    	event.data.u32 = x; \
 
#define EPOLL_OUT_FILL_UINT( x, event ) \
		memset( &event, 0, sizeof( struct epoll_event ) ); \
		event.events = EPOLL_OUT_EVENT; \
    	event.data.u32 = x; \
 
int dInit_EpollSocket( int *pEFD, UINT uiMaxEvents );
void EPOLL_ADD( struct epoll_event *event, int *pEFD, int *pSocket );
void EPOLL_MODIFY( struct epoll_event *event, int *pEFD, int *pSocket );
int dInit_ServerSocket( int *pServerSock, USHORT server_port, UINT uiListenQNum );
int dSet_ClientSocket( int newsockfd, UINT uiMaxBuffSize );
int dRead_Packet( int dSocket, char *pSPos, UINT uiMaxReadLen );
int dWrite_Packet( int dSocket, char *pSPos, UINT uiSendLen );
int dConnect_Server(int *pMySocket, UINT uiServerIPAddr, USHORT server_port);


#endif




