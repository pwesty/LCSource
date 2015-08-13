/*
#include "stdafx.h"
#include <assert.h>
#include "tcpipconnection.h"
#include "TxtQueue.h"
//#include "TcpIp.h"
#include <mmsystem.h>
#include <stdio.h>
*/
#include "stdh.h"
#include <Engine/Network/tcpipconnection.h>
#include <Engine/Network/TxtQueue.h>
#include <Engine/Network/CNetwork.h>


#define IS_NEWLINE(c)	(c == '\n' || c == '\r')

#define MAX_INPUT_LENGTH 1024
#define MAX_RAW_INPUT_LENGTH 512

extern BOOL g_bWorldLoading;
extern BOOL g_bCompressMsg;

char msg_buf[1024];


CTcpIpConnection _tcpip;

//DWORD WINAPI ThreadFunc( LPVOID lpParam ) ;

CTcpIpConnection::CTcpIpConnection() 
{
	Socket = INVALID_SOCKET;

	m_nSendingLen = 0;	

	m_nSecureMethod = 0;
	m_nSecureNum = 15;

	for(int i=0; i< MAX_SEND_BUF_LEN+200; i++)
		m_strSendingBuf[i] = '\0';

	m_bConnect = FALSE;

	m_pTxtQueue = new CTxtQ;
	m_Inbuf[0] = '\0';

}

CTcpIpConnection::~CTcpIpConnection()
{	
	if (Socket != INVALID_SOCKET) closesocket(Socket);

	SAFE_DELETE(m_pTxtQueue);
}

void CTcpIpConnection::ResetConnection()
{
	if (Socket != INVALID_SOCKET) closesocket(Socket);
	Socket = INVALID_SOCKET;
}

BOOL CTcpIpConnection::InitSocket()
{
	WORD      wVersionRequested;
    WSADATA   wsaData;

	wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
    {
		WSACleanup();
        return FALSE;
    }
	return TRUE;
}

BOOL CTcpIpConnection::InitConnection(SOCKET s)
{	
	Socket = s;
	BOOL bl = TRUE;
//	unsigned long blength = 256;
//	unsigned long bpr = 0;

	setsockopt(Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&bl, sizeof(BOOL));
	setsockopt(Socket, IPPROTO_TCP, SO_KEEPALIVE, (const char*)&bl, sizeof(BOOL));
	return TRUE;
}

//client part of the connection
void* CTcpIpConnection::GetConnectionMessage(DWORD portNo, DWORD& length)
{
	DWORD* t = new DWORD;
	*t = portNo;
	length = sizeof(DWORD);
	return (void*)t;
}


//서버에 연결하기.
BOOL CTcpIpConnection::ConnectToServer(char* serverName, int _port)
{
#ifndef _DEBUG
	try{
#endif
		ResetConnection();// 소켓을 닫는다.

		SOCKET ServerSock = INVALID_SOCKET;
		int tryCount = 3;
		BOOL bSuccessConnect = FALSE;
		while (tryCount > 0)
		{
			tryCount--;
			ServerSock = socket(AF_INET, SOCK_STREAM, 0);


			unsigned long val = 1;
			ioctlsocket(ServerSock, FIONBIO, &val);

			sockaddr_in sai;
			memset(&sai, 0, sizeof(sockaddr_in));
			sai.sin_port = htons(_port);
			sai.sin_family = AF_INET;
			sai.sin_addr.s_addr = inet_addr(serverName);
			if(INADDR_NONE == sai.sin_addr.s_addr)
			{

				hostent *Hostent = gethostbyname(serverName);
				if (Hostent == NULL)
				{
					closesocket(ServerSock);
					ServerSock = INVALID_SOCKET;
					continue ;
				}
				sai.sin_addr.s_addr = ((struct in_addr FAR *)(Hostent->h_addr))->s_addr;
			}
			int res = connect(ServerSock, (sockaddr*)&sai, sizeof(sockaddr_in));
			if (res!=0)
			{
				int err = WSAGetLastError();
			
				if (err != WSAEWOULDBLOCK) 
				{
					closesocket(ServerSock);
					ServerSock = INVALID_SOCKET;
					continue ;
				}
				else
				{
					fd_set wfds, efds;
					FD_ZERO(&wfds);
					FD_ZERO(&efds);
					FD_SET(ServerSock, &wfds);
					FD_SET(ServerSock, &efds);
					struct timeval tv;
					tv.tv_sec = 3;
					tv.tv_usec = 0;
					if (select(0, NULL, &wfds, &efds, &tv) <= 0 || FD_ISSET(ServerSock, &efds))
					{
						closesocket(ServerSock);
						ServerSock = INVALID_SOCKET;
						continue ;
					}
				}
			}
			bSuccessConnect = TRUE;
			break;
		}

		if (!bSuccessConnect)
		{
			return FALSE;
		}

//	Sleep(1000);

		InitConnection(ServerSock);	// 소켓 새로 생성및 초기화

		_pNetwork->resetSeq();

	return TRUE;
#ifndef _DEBUG
	}catch(...){ASSERT(FALSE);return FALSE;};
#endif
}

// 로그인 서버와 연결이 되었는지 확인합니다.
BOOL CTcpIpConnection::IsConnected(SOCKET sock)
{	
	fd_set	fsExcept;	
	timeval tvTimeOut = {0,0};

	FD_ZERO(&fsExcept);
	FD_SET(sock, &fsExcept);
	
	if (select(1, NULL, NULL, &fsExcept, &tvTimeOut) <= 0)
	{
		// TIME_OUT
		return TRUE;
	}
	// 예외 상황이므로 소켓을 닫음.
	closesocket(sock);
	return FALSE;
}


int CTcpIpConnection::WhatErr()
{
	int err = WSAGetLastError();
	switch (err) {
	case WSANOTINITIALISED:		
	case WSAENETDOWN:		
	case WSAEFAULT:		
	case WSAENOTCONN:		
	case WSAEINTR:		
	case WSAEINPROGRESS:		
	case WSAENETRESET:		
	case WSAENOTSOCK:		
	case WSAEOPNOTSUPP:		
	case WSAESHUTDOWN:	
	case WSAEWOULDBLOCK:		
	case WSAEMSGSIZE:		
	case WSAEINVAL:		
	case WSAECONNABORTED:		
	case WSAETIMEDOUT:		
	case WSAECONNRESET:
		return err;
		break;
	default:
		return err;
		break;
	}
}

///////////////////////////////////////////////////////////////////

BOOL CTcpIpConnection::SendNetMessage(void* msg, BOOL bForce, BOOL bSecure)
{

#ifndef _DEBUG
	try{
#endif
	int msg_size = strlen((char *)msg);
	int l;
	int TickCount = 0;
	//char* tempbuf = (char*)msg; // 암호화 안한거...
	// 허접 암호화...
//	char msg_buf[1024];	
	char* tempbuf = msg_buf;
	for (int msg_idx = 0; msg_idx < msg_size; msg_idx++) {
		if (bSecure) {
			switch (m_nSecureMethod) {
			case 0:
				tempbuf[msg_idx] = ((char*) msg)[msg_idx] + m_nSecureNum;
				break;
			case 1:
				tempbuf[msg_idx] = ((char*) msg)[msg_idx] - m_nSecureNum;
				break;
			case 2:
				tempbuf[msg_idx] = (((char*) msg)[msg_idx] ^ 0xC3) + m_nSecureNum;
				break;
			case 3:
				tempbuf[msg_idx] = (((char*) msg)[msg_idx] ^ 0xC3) - m_nSecureNum;
				break;
			default:
				tempbuf[msg_idx] = ((char*) msg)[msg_idx];
				break;
			}
		} else {
			tempbuf[msg_idx] = ((char*) msg)[msg_idx];
		}
	}
	memcpy(m_strSendingBuf + m_nSendingLen, tempbuf, msg_size);	
	m_nSendingLen += msg_size;

	int sentl = 0;
	if (m_nSendingLen > MAX_SEND_BUF_LEN || bForce) {
		do
		{
			tempbuf = m_strSendingBuf;
			l = send(GetOutSocket(), tempbuf, m_nSendingLen-sentl, 0);
			if (l == SOCKET_ERROR) {
				m_nSendingLen = 0;
				return FALSE;
			}
			sentl+=l;
			tempbuf+=l;
			if (sentl<m_nSendingLen)
			{			
				Sleep(5);
				TickCount+=1;
			};
		}while (sentl < m_nSendingLen  && TickCount<1000);

		if (sentl!=m_nSendingLen) {
			if (m_nSendingLen - sentl < MAX_SEND_BUF_LEN) {
				memcpy(m_strSendingBuf, m_strSendingBuf+sentl, m_nSendingLen-sentl);
				m_nSendingLen -= sentl;
			} else {
				// 소켓 닫아버렷!
				m_nSendingLen = 0;
				return FALSE;
			}
		} else {
			// 시간은 걸렸지만, 다 보냈구먼...
			m_nSendingLen = 0;
		}
	} else {
		l = send(GetOutSocket(), m_strSendingBuf, m_nSendingLen, 0);
		if (l == SOCKET_ERROR) return FALSE;
		if (l < m_nSendingLen) {
			memcpy(m_strSendingBuf, m_strSendingBuf+l, m_nSendingLen-l);
			m_nSendingLen -= l;
		} else {
			m_nSendingLen = 0;
		}
	}
	return TRUE;
#ifndef _DEBUG
	}catch(...){ASSERT(FALSE);return FALSE;};
#endif
}

BOOL CTcpIpConnection::FlushSendingBuffer()
{
#ifndef _DEBUG
	try{
#endif
	if (m_nSendingLen <= 0)
		return TRUE;
	int l = send(GetOutSocket(), m_strSendingBuf, m_nSendingLen, 0);
	if (l == SOCKET_ERROR) return FALSE;
	if (l < m_nSendingLen) {
		memcpy(m_strSendingBuf, m_strSendingBuf+l, m_nSendingLen-l);
		m_nSendingLen -= l;
	} else {
		m_nSendingLen = 0;
	}
	return TRUE;
#ifndef _DEBUG
	}catch(...){ASSERT(FALSE);return FALSE;};
#endif
}

void CTcpIpConnection::ResetSendingBuffer()
{
	m_strSendingBuf[0] = '\0';
	m_nSendingLen = 0;
}



int CTcpIpConnection::ExpandData (char *des, char *src, int size)
{
	char set [16] = {0, ' ', '-', '.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '\n', 0};
	char ch;
	int count;
	int src_index;
	int des_index;
	int left, right;

	src_index = 0;
	des_index = 0;
	while (src_index < size) {
		ch = src[src_index++];
		if (ch == -1) continue;
		count = ch & 0x7f;
		if (ch & 0x80) { // it's compressed...
			while (count > 0) {
				ch = src[src_index++];
				left  = set[(ch & 0xf0) >> 4];
				right = set[ch & 0x0f];
				des [des_index ++] = left;
				if (right == 0) break;				
				des [des_index ++] = right;
				count -= 2;
			}
		}
		else {
			while (count > 0) {
				des [des_index++] = src[src_index++] ^ 0xff;
				count --;
			}
		}
	}
	return des_index;
}

int CTcpIpConnection::CompressData (char *des, char * src, int size)
{
	char subit[MAX_RECV_BUF_LEN + 1];
	int index;
	int count;
	bool compress;
	int ch;
	int p1, p2;
	bool left;

	if (size >= MAX_RECV_BUF_LEN) size = MAX_RECV_BUF_LEN;
	index = 0;
	count = 0;

	index = 0;
	memset (subit, '0', size);
	subit[size] = '\0';
	// 먼저 압축할 문자와 압축안할 문자를 나눈다..
	for (index=0; (ch=src[index]) != '\0'; index ++) {
		if ((ch >= '0' && ch <= '9') ||	ch=='-' || ch=='.' || ch==' ' || ch=='\n') 
			subit[index] = '1';
	}
		
	index = 0;
	// 압축 향상을 위해서 압축할 문자가 3개 이내이면 압축을 하지 않는걸로 선택한다.
	while (index < size) {
		while (subit[index] == '0')	index++;	// '0'이면 압축안함이므로 스킵..
		for (count=0; subit[index] == '1'; index++, count++)
			if (index >= size) break;
		if (count < 3) {
			while (count > 0) {
				subit[index - count] = '0';
				count --;
			}
		}
	}

	index = 0;
	p1 = 0;
	p2 = 0;
	compress = false;
	while (index < size) {
		// don't compress data...
		p1 = index;
		while (subit[index] == '0') index ++;
		if (index > p1) {
			count = index - p1;
			des[p2++] = count;
			while (count > 0) {
				des[p2++] = src[p1++] ^ 0xff;
				count --;
			}
		}
		if (index >= size) break;

		// for compress data...
		p1 = index;
		left = true;
		while (subit[index] == '1') index ++;
		if (index > p1) {
			count = index - p1;
			des[p2 ++] = count | 0x80;
			while (count > 0) {
				ch = src [p1++];
				if (ch >= '0' && ch <= '9')	ch = 0x04 + ch - '0';
				else if (ch == ' ')		ch = 0x01;
				else if (ch == '-')		ch = 0x02;
				else if (ch == '.')		ch = 0x03;
				else if (ch == '\n')	ch = 0x0e;
				else ch = 0;

				if (left) {
					des [p2] = ch << 4;
					left = false;
				}
				else {
					des [p2++] |= ch;
					left = true;
				}
				count --;
			}
			if (!left) p2++;
		}
	}
	des [p2] = -1;
	des [p2 + 1] = '\0';
	return (p2 + 2);
}


int CTcpIpConnection::PerformSocketRead(SOCKET desc, char *read_point, int space_left)
{
	int ret;

	BOOL bSecure = TRUE;
	
	ret = recv(desc, read_point, space_left, 0);
	
	/* Read was successful. */
	if (ret > 0){
		
		if (bSecure) {
			for (int buf_idx = 0; buf_idx < ret; buf_idx++) {
				read_point[buf_idx] = (read_point[buf_idx] - 15) ^ 0xC3;				
			}
		}
		
		return (ret);
	}
	
	/* read() returned 0, meaning we got an EOF. */
	if (ret == 0) {
		//Log("WARNING: EOF on socket read (connection broken by peer)");
		return (-1);
	}
	
	/*
	* read returned a value < 0: there was an error
	*/
	WhatErr();

	if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAEINTR)
		return (0);
		/*
		* We don't know what happened, cut them off. This qualifies for
		* a SYSERR because we have no idea what happened at this point.
	*/
	//perror("SYSERR: perform_socket_read: about to lose connection");
	return (-1);
}


int CTcpIpConnection::ProcessInput()
{
	int buf_length, failed_subst;
	int bytes_read;
	int space_left;
	char *ptr, *read_point, *write_point, *nl_pos = NULL;
	char tmp[MAX_INPUT_LENGTH];

	/* first, find the point where we left off reading data */
	buf_length = strlen(m_Inbuf);
	read_point = m_Inbuf + buf_length;
	space_left = MAX_RAW_INPUT_LENGTH - buf_length - 1;

	do {
		if (space_left <= 0) {			
			return (-1);
		}

		bytes_read = PerformSocketRead(Socket, read_point, space_left);

		if (bytes_read < 0)	/* Error, disconnect them. */
			return (-1);
		else if (bytes_read == 0)	/* Just blocking, no problems. */
			return (0);
		else {
		//	char szMsg[80];
		//	sprintf( szMsg, "잘 돌아갑니다."); 
		//	MessageBox( NULL, szMsg, "메시지", MB_OK );
		}

		/* at this point, we know we got some data from the read */
		for (int read_idx = 0; read_idx < bytes_read; read_idx++) {
			*(read_point + read_idx) = *(read_point + read_idx);
		}


		*(read_point + bytes_read) = '\0';	/* terminate the string */

		/* search for a newline in the data we just read */
		for (ptr = read_point; *ptr && !nl_pos; ptr++)
			if ((*ptr == '\n'))
				nl_pos = ptr;

		read_point += bytes_read;
		space_left -= bytes_read;

		/*
		 * on some systems such as AIX, POSIX-standard nonblocking I/O is broken,
		 * causing the MUD to hang when it encounters input not terminated by a
		 * newline.  This was causing hangs at the Password: prompt, for example.
		 * I attempt to compensate by always returning after the _first_ read, instead
		 * of looping forever until a read returns -1.  This simulates non-blocking
		 * I/O because the result is we never call read unless we know from select()
		 * that data is ready (process_input is only called if select indicates that
		 * this descriptor is in the read set).  JE 2/23/95.
		 */
	} while (nl_pos == NULL);

/*
* okay, at this point we have at least one newline in the string; now we
* can copy the formatted data to a new array for further processing.
*/


	read_point = m_Inbuf;

	while (nl_pos != NULL) {
		write_point = tmp;
		space_left = MAX_INPUT_LENGTH - 1;

		/* The '> 1' reserves room for a '$ => $$' expansion. */
		for (ptr = read_point; (space_left > 1) && (ptr < nl_pos); ptr++) {
			if (*ptr == '\b' || *ptr == 127) { /* handle backspacing or delete key */
				if (write_point > tmp) {
					if (*(--write_point) == '$') {
						write_point--;
						space_left += 2;
					} else
						space_left++;
				}
			} else {
				*write_point++ = *ptr;
				space_left --;
			}
		}

		*write_point = '\0';
	
		failed_subst = 0;

		if (!failed_subst) {

			m_pTxtQueue->WriteToQ(tmp);		
		}
					
		/* find the end of this line */
		while (IS_NEWLINE(*nl_pos))
			nl_pos++;

		/* see if there's another newline in the input buffer */
		read_point = ptr = nl_pos;
		for (nl_pos = NULL; *ptr && !nl_pos; ptr++)
			if (IS_NEWLINE(*ptr))
				nl_pos = ptr;
			
			/* now move the rest of the buffer up to the beginning for the next pass */
	}
	write_point = m_Inbuf;
	while (*read_point)
		*(write_point++) = *(read_point++);
	*write_point = '\0';
	
	return (1);
}



char recv_buf[MAX_RECV_BUF_LEN];
int CTcpIpConnection::ReadMessage(char *buf, int buf_len, BOOL bSecure)
{
	int l;

	if (buf_len >= MAX_RECV_BUF_LEN)
		buf_len = MAX_RECV_BUF_LEN - 1;
	l = recv(GetInSocket(), buf, buf_len, 0);

	if (l==SOCKET_ERROR) 
	{
		int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK)
			return 0;
		return -1;
	};

	//  허접 암호화...
	if (bSecure) {
// main의 변수. 확인해야함.
	//	if (!g_bCompressMsg) {
			for (int buf_idx = 0; buf_idx < l; buf_idx++) {
				buf[buf_idx] = (buf[buf_idx] - 15) ^ 0xC3;
				//buf[buf_idx] = buf[buf_idx] - 17;
			}
	//	}
	}

	return l;
}
