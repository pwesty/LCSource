#ifndef SE_INCL_COMMUNICATIONINTERFACE_H
#define SE_INCL_COMMUNICATIONINTERFACE_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif



#define SERVER_CLIENTS 1
#define DEMO_CLIENT SERVER_CLIENTS

#include <Engine/Network/CPacket.h>
#include <Engine/Network/Inputbuffer.h> 

struct LoginServer;

struct sIPFilter
{
	unsigned char	chSB1;
	unsigned char	chSB2;
	unsigned char	chSB3;
	unsigned char	chSB4;
	INDEX			iCountry;	// 국가

	sIPFilter()
	{
		chSB1		= -1;
		chSB2		= -1;
		chSB3		= -1;
		chSB4		= -1;
		iCountry	= -1;
	}

	void	SetIP( unsigned char SB1, unsigned char SB2, unsigned char SB3, unsigned char SB4, INDEX Country )
	{
		chSB1		= SB1;
		chSB2		= SB2;
		chSB3		= SB3;
		chSB4		= SB4;
		iCountry	= Country;
	}

	bool operator<(const sIPFilter &other) const
	{
		if( iCountry < other.iCountry )
			return true;
		return false;
	}
};

// Communication class
class ENGINE_API CCommunicationInterface 
{
public:
	// Login Server Information
	char	cci_szAddr[100];
	int		cci_iPort;
	int		cci_iSelectedServerNum;

	// 서버 선택 페이지에서 쓰기 위한 부분.
	// NOTE : sl.dta에서 읽어들이기 위해서 멤버 변수로 넣었음.
	int		cci_iFullUsers;
	int		cci_iBusyUsers;
public:
	BOOL cci_bSocketOpen;		// set if socket is open and working
	BOOL cci_bBound;				// set for udp sockets that have been explicitly or implicitly bound
	BOOL cci_bInitialized;	// is the communication interface initialized or not
	BOOL cci_bWinSockOpen;	// is the winsock API initialized
	BOOL cci_bServerInitialized;
	BOOL cci_bClientInitialized;
	
	CPacketBuffer cci_pbMasterOutput;					// master output buffer				 
	CPacketBuffer cci_pbMasterInput;					// master input buffer
	
	SOCKET cci_hSocket;						// the socket handle itself

	BOOL m_bTcpIp;
	BOOL cci_bInitUpdateMasterBuffer;

	CInputBuffer m_inbuf;

	// Date : 2005-05-02(오전 10:11:01), By Lee Ki-hwan
	BOOL	m_b2stLocal;				// 2번째 지역인지 확인 (중국 Local )

protected:
	// [2013/01/09] sykim70
	CLCCRC32 m_crc;

	// IP Filter 목록.
	std::vector<sIPFilter>	m_vecIPFilter;	
public:
	// client
	void Client_OpenLocal(void);
	void Client_OpenNet_t(ULONG ulServerAddress);
	// update master UDP socket and route its messages
	void UpdateMasterBuffers(void);
	
public:
	CCommunicationInterface(void);
	~CCommunicationInterface(void){};
	
	// start/stop protocols
	void Init(void);
	void Close(void);
	
//강동민 수정 시작 시스템 마우스 작업	09.09
	void Disconnect(void);
	void Reconnect(CTString strIP, ULONG ulPort);
//강동민 수정 끝 시스템 마우스 작업		09.09
	
	void InitWinsock(void);
	void EndWinsock(void);
	BOOL PrepareForUse(BOOL bUseNetwork, BOOL bClient, BOOL bTcpIp = FALSE);	// by seo 40225
	void Unprepare(void);
	BOOL IsNetworkEnabled(void);
	// get address of local machine
	void GetHostName(CTString &strName, CTString &strAddress);
	
	// create an inet-family socket
	void CreateSocket_t();
	// bind socket to the given address
	void Bind_t(ULONG ulLocalHost, ULONG ulLocalPort);
	// set socket to non-blocking mode
	void SetNonBlocking_t(void);
	// get generic socket error info string an last error
	CTString GetSocketError(INDEX iError);
	// open an UDP socket at given port 
	void OpenSocket_t(ULONG ulLocalHost, ULONG ulLocalPort);
	
	// get address of this host
	void GetLocalAddress_t(ULONG &ulHost, ULONG &ulPort);
	// get address of the peer host connected to this socket
	void GetRemoteAddress_t(ULONG &ulHost, ULONG &ulPort);
	
	// broadcast communication
	void Broadcast_Send(const void *pvSend, SLONG slSendSize,CAddress &adrDestination);
	BOOL Broadcast_Receive(void *pvReceive, SLONG &slReceiveSize,CAddress &adrAddress);
	// here we receive connect requests
	void Broadcast_Update_t(void);
	
	// Server
	void Server_Init_t(void);
	void Server_Close(void);
	
	void Server_ClearClient(INDEX iClient);
	BOOL Server_IsClientLocal(INDEX iClient);
	BOOL Server_IsClientUsed(INDEX iClient);
	CTString Server_GetClientName(INDEX iClient);
	ULONG Server_GetClientPendingData(INDEX iClient);
	ULONG Server_GetMaxPendingData();
	ULONG Server_GetTotalPendingData();
	
	void Server_Send_Reliable(INDEX iClient, const void *pvSend, SLONG slSendSize);
	BOOL Server_Receive_Reliable(INDEX iClient, void *pvReceive, SLONG &slReceiveSize);
	void Server_Send_Unreliable(INDEX iClient, const void *pvSend, SLONG slSendSize);
	BOOL Server_Receive_Unreliable(INDEX iClient, void *pvReceive, SLONG &slReceiveSize);
	
	void Client_Send_Unreliable_New(const void *pvSend, SLONG slSendSize, BOOL bLogin= FALSE);
	BOOL Client_Receive_Unreliable_New(void *pvReceive, SLONG &slReceiveSize, INDEX &iIndex);
	
	BOOL Server_Update(void);
	
	// Client
	void Client_Init_t(char* strServerName);
	void Client_Init_t(ULONG ulServerAddress);
	void Client_Close(void);
	
	void Client_Clear(void);
	BOOL Client_IsConnected(void);
	
	void Client_Send_Reliable(const void *pvSend, SLONG slSendSize);
	BOOL Client_Receive_Reliable(void *pvReceive, SLONG &slReceiveSize);
	BOOL Client_Receive_Reliable(CTStream &strmReceive);
	void Client_PeekSize_Reliable(SLONG &slExpectedSize,SLONG &slReceivedSize);
	void Client_Send_Unreliable(const void *pvSend, SLONG slSendSize);
	BOOL Client_Receive_Unreliable(void *pvReceive, SLONG &slReceiveSize);
	
	BOOL Client_Update(void);
	
	BOOL IsNetworkOn(){return cci_bBound;}
	void EnableInitUpdateMasterBuffer(){cci_bInitUpdateMasterBuffer = TRUE;}
	BOOL IsInitUpdateMasterBuffer(){return cci_bInitUpdateMasterBuffer;}

	BOOL Is2rdLocal(); // Date : 2005-05-02(오전 10:11:01), By Lee Ki-hwan	// 2번째 지역인지 확인 (중국 Local )
	
	void attachCRC(CPacket* packet);

protected:

	int ReadInfo( LoginServer* stLogin );
	void ShowDisconnMsg();
	void SetIPFilter();
	BOOL CheckIPFilter();
	BOOL IsValidIP();
	bool FindHosNameIP(const char* szHostName, sIPFilter &ip, INDEX iCountry);

};

extern ENGINE_API CCommunicationInterface _cmiComm;
extern CPacketBufferStats _pbsSend;
extern CPacketBufferStats _pbsRecv;

#endif  /* include-once check. */
