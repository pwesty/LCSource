/*
	FGC_Net_Process : Nksp에서 생성된 통신 모듈의 기능 처리 클래스.
	작성자 : 김영환 2011,12,05
	수정자 : 이언호 2012,04,13
	FGC : Forthen Game Client
	Net_Process : Network Modul Process

    통신처리 기능만 처리한다.
*/
#include "Engine/Network/CPacket.h"
#include "include/F_Thread.h"
#include <LCCrypt.h>
#include <list>

#pragma once

using namespace std;

// 소켓 클래스와 연결할 Interface
typedef int		(*SEND_FUNC) (char* p_Buffer, int iBuffer_Size);
typedef char*	(*RECV_FUNC) (int *p_Recv_Size);
typedef int		(*CONNECT_FUNC) (char *p_Ip, int p_Port);
typedef int		(*FUNC_INT_RETURN) ();
// 위 interface 함수는 암호화된 패킷을 사용한다.

namespace net
{
	DWORD	WINAPI		SendThreadFunc(LPVOID pParam);
	DWORD	WINAPI		RecvThreadFunc(LPVOID pParam);

	// 구조체를 클래스로 확장할 필요성을 생각해 보자~
	struct LocalMsgBuffer 
	{
		UBYTE	ubType;
		UBYTE	msgBuf[CNM_TEMP_BUFFER_LENGTH];
		int		msgSize;

		void Clear()
		{
			memset(msgBuf, 0, CNM_TEMP_BUFFER_LENGTH);
		}
	};

	class lmbExtendQueue : public list<LocalMsgBuffer*>
	{
	private:
		void ReleaseElement(LocalMsgBuffer* lmbInput); // pop() 요소를 Release한다.

	public:
		void				AddNetworkMessage(CNetworkMessage& istr); // push_back에 LocalMsgBuffer로 등록
		bool				getFrontElement(CNetworkMessage& istr);
		void				ReleaseAll();
	};
} // net namespace end

// 일단 send, recv 스레드를 두개 가져 갔다. (나중에 합쳐서 쓸 수도 있다.)
// ThreadState의 경우에 수는 나오는 대로 추가하자. (recv, send 동시 적용이다.)
namespace FGC_STATE
{
	typedef enum socketThreadState
	{
		SOC_DISCONNECTED,
		SOC_AUTH,
		SOC_TASK_WAIT,
		SOC_MOVING_ZONE,
		SOC_RESTART,
		SOC_CLOSE,
	}SOCKET_STATE;
}

// 1. singleton class를 만들자
// 2. Naming은 기존껄 따른다.

class ENGINE_API FGC_Net_Process
{
private:
	CTString				m_IpAddress;		// 접속 Ip 정보
	int						m_Port;				// 접속 포트 정보

	ULONG					m_Sequence;			// Send Sequence index
	
	// [2013/01/09] sykim70
	CLCCRC32 m_crc;

	// 사용할 스레드 변수 선언
	F_Thread*				m_pRecvEventThread;
	F_Thread*				m_pSendEventThread;

	HANDLE					m_hExitTask[2];	// send, recv 스레드 종료를 기다리는 이벤트 핸들

	// 사용할 메시지 버퍼 선언
	//list<LocalMsgBuffer*>	m_recvBuffer;
	//list<LocalMsgBuffer*>	m_sendBuffer;
	net::lmbExtendQueue		m_recvBuffer;
	net::lmbExtendQueue		m_sendBuffer;

	// 스레드 (Task) 상태 변수 선언
	FGC_STATE::SOCKET_STATE	m_FGCState;

	// linked functions
	CONNECT_FUNC			connectSocket;		// 연결 좀 합시다.
	FUNC_INT_RETURN			disconnectSocket;	// 연결 끊겠어요.
	FUNC_INT_RETURN			isConnectSocket;	// 연결 중인가요?
	SEND_FUNC				sendToSocket;		// 패킷을 보내요.
	RECV_FUNC				receiveSocket;		// 패킷을 받아요.

	// static single instance 선언
	static FGC_Net_Process*	g_FGC;

public:
	// Functions
	FGC_Net_Process();			// 기본 생성자
	FGC_Net_Process(
		CONNECT_FUNC pConnect,
		FUNC_INT_RETURN pDisConnect,
		FUNC_INT_RETURN pIsConnect,
		SEND_FUNC pSend,
		RECV_FUNC pReceive);	// 복사 생성자
	~FGC_Net_Process();			// 소멸자

	// Accessor function
	void			SetIpAddress(CTString strIp)		{ m_IpAddress = strIp; }
	const CTString	GetIpAddress()						{ return m_IpAddress; }

	void			SetPort(int port)					{ m_Port = port; }
	int				GetPort()							{ return m_Port; }

	void			IncSequence(void)					{ ++m_Sequence; }	// 무조건 시퀀스는 증가 처리
	ULONG			GetSequence(void)					{ return m_Sequence; }

	F_Thread*		GetSendThread(void)					{ return m_pSendEventThread; }
	F_Thread*		GetRecvThread(void)					{ return m_pRecvEventThread; }

	const FGC_STATE::SOCKET_STATE&	GetFGCState(void)									{ return m_FGCState; }
	void							SetFGCState(const FGC_STATE::SOCKET_STATE& state)	{ m_FGCState = state; }

	// Member function
	CPacket*		MakePacketHeader(const void *pvReceive, int packetSize); // Packet Header 구성
	BOOL			SendChannelAuth(void);//(int nChannel, int charIndex, CTString charName);
	BOOL			RecvPacket(void); // 패킷 받기
	BOOL			SendPacket(void); // 패킷 보내기(Main)
	BOOL			SendPacket(void *pvSend, int packetSize); // 패킷 보내기(sub)

	bool			MsgCryption(net::LocalMsgBuffer& lmbMsg);
	bool			MsgCryption(net::LocalMsgBuffer* lmbMsg);
	bool			MsgCryption(CNetworkMessage& iStr);
	bool			MsgDecryption(net::LocalMsgBuffer* lmbMsg);

	bool			AddSendBuffer(CNetworkMessage& iStr);
	bool			PopRecvBuffer(CNetworkMessage& iStr);

	bool			IsConnectSubHelper();
	bool			DisConnectSubHelper();
	bool			ConnectSubHelper();

	void			SubHelperRecvProcessing(void);

	void			SetExitEvent(int iValue) 
	{
		if (m_hExitTask[iValue])
			SetEvent(m_hExitTask[iValue]);
	}

public:
	// static functions
	static FGC_Net_Process* CreateFGC(
		CONNECT_FUNC pConn,
		FUNC_INT_RETURN pDisConn,
		FUNC_INT_RETURN pIsConn,
		SEND_FUNC pSend,
		RECV_FUNC pRecv)
	{
		if ( NULL == g_FGC)
		{
			g_FGC = new FGC_Net_Process(pConn, pDisConn, pIsConn, pSend, pRecv);
		}

		return g_FGC;
	}
	static void ReleaseFGC(void)
	{
		if (g_FGC)
		{
			delete g_FGC;
		}

		g_FGC = NULL;
	}
	static FGC_Net_Process* GetFGC(void)
	{
		return g_FGC;
	}
};

#define FGC		FGC_Net_Process::GetFGC()
