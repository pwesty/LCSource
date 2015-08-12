#ifndef _TCPIPCONNECTION_H_
#define _TCPIPCONNECTION_H_


#include "winsock.h"


#define MAX_RECV_BUF_LEN	1024
#define MAX_SEND_BUF_LEN	4096

#define DEFAULT_FRAME		0x76453201

class CTxtQ;


class CTcpIpConnection
{

public:
	
	SOCKET Socket;	

	CTxtQ			 *m_pTxtQueue;
	
	int m_nSendingLen;		
	int m_nSecureMethod;
	int m_nSecureNum;
	
	BOOL m_bConnect;//connect가 되었는지...
	
	char m_Inbuf[1024]; //큐에 들어가는 버퍼
	char m_strSendingBuf[MAX_SEND_BUF_LEN+200];		

public:
	CTcpIpConnection();
	virtual ~CTcpIpConnection();
	
	virtual BOOL InitConnection(SOCKET socket);
	virtual void ResetConnection();	
	
	//sockets		
	SOCKET GetInSocket(){return Socket;};
	SOCKET GetOutSocket(){return Socket;};
	
	//client side of the connection
	virtual void* GetConnectionMessage(DWORD portNo, DWORD& length);
	virtual BOOL ConnectToServer(char* serverName, int port);	
	BOOL	IsConnected(SOCKET sock);
	
	BOOL SendNetMessage(void* msg, BOOL bForce = FALSE, BOOL bSecure = TRUE); //TCP/IP wrapper
	BOOL FlushSendingBuffer();
	void ResetSendingBuffer();	
	
	// 암호화를 위한 함수
	int ExpandData (char *des, char *src, int size);
	int CompressData (char *des, char * src, int size);	
	
	//소켓 초기화
	BOOL InitSocket();		
	
	//한줄씩 receive 하기.
	int PerformSocketRead(SOCKET desc, char *read_point, int space_left);
	int ProcessInput(); 
	
	int ReadMessage(char *buf, int buf_len, BOOL bSecure = TRUE);

	//에러체크
	int WhatErr();
	
};

extern ENGINE_API CTcpIpConnection _tcpip;

#endif