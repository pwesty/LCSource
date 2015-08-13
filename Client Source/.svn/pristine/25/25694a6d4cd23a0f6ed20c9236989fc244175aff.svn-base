#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Base/CTString.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/ErrorTable.h>
#include <Engine/Base/Synchronization.h>
#include <Engine/Base/Translation.h>
#include <Engine/Base/MemoryTracking.h>

#include <Engine/Network/ClientInterface.h>
#include <Engine/Network/CommunicationInterface.h>
#include <Engine/Network/CPacket.h>
//#include <Engine/Network/GameSpy.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>

//0105
#include <Engine//Network/tcpipconnection.h>
//#include <Engine//Network/Inputbuffer.h> 
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMessageBox.h>
#include <vector>
#include <Engine/GameState.h>
#include <Engine/GameStageManager/StageMgr.h>
#include <Engine/Loading.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_LOCAL_CLIENT     0
// 강동민 수정 시작
#define MAX_LOGINSERVER			(5)
// 강동민 수정 끝
// application state variables by NSP
//extern BOOL g_bTcp;
// end of ..

#define USER_NUM_FULL		1000
#define USER_NUM_BUSY		300

extern INDEX net_iPort;
extern CTString net_strLocalHost;
extern INDEX net_bLookupHostNames;
extern INDEX net_bReportICMPErrors;
extern FLOAT net_fDropPackets;
extern FLOAT net_tmConnectionTimeout;
extern INDEX net_bReportPackets;

#ifdef EUROUPEAN_SERVER_LOGIN
extern INDEX g_iConnectEuroupean;
#endif

extern SOCKET g_hSocket;
extern INDEX	g_iCountry;

static struct ErrorCode ErrorCodes[] = {
	ERRORCODE(WSAEINTR          , "WSAEINTR"),
	ERRORCODE(WSAEBADF          , "WSAEBADF"),
	ERRORCODE(WSAEACCES         , "WSAEACCES"),
	ERRORCODE(WSAEFAULT         , "WSAEFAULT"),
	ERRORCODE(WSAEINVAL         , "WSAEINVAL"),
	ERRORCODE(WSAEMFILE         , "WSAEMFILE"),
	ERRORCODE(WSAEWOULDBLOCK    , "WSAEWOULDBLOCK"),
	ERRORCODE(WSAEINPROGRESS    , "WSAEINPROGRESS"),
	ERRORCODE(WSAEALREADY       , "WSAEALREADY"),
	ERRORCODE(WSAENOTSOCK       , "WSAENOTSOCK"),
	ERRORCODE(WSAEDESTADDRREQ   , "WSAEDESTADDRREQ"),
	ERRORCODE(WSAEMSGSIZE       , "WSAEMSGSIZE"),
	ERRORCODE(WSAEPROTOTYPE     , "WSAEPROTOTYPE"),
	ERRORCODE(WSAENOPROTOOPT    , "WSAENOPROTOOPT"),
	ERRORCODE(WSAEPROTONOSUPPORT, "WSAEPROTONOSUPPORT"),
	ERRORCODE(WSAESOCKTNOSUPPORT, "WSAESOCKTNOSUPPORT"),
	ERRORCODE(WSAEOPNOTSUPP     , "WSAEOPNOTSUPP"),
	ERRORCODE(WSAEPFNOSUPPORT   , "WSAEPFNOSUPPORT"),
	ERRORCODE(WSAEAFNOSUPPORT   , "WSAEAFNOSUPPORT"),
	ERRORCODE(WSAEADDRINUSE     , "WSAEADDRINUSE"),
	ERRORCODE(WSAEADDRNOTAVAIL  , "WSAEADDRNOTAVAIL"),
	ERRORCODE(WSAENETDOWN       , "WSAENETDOWN"),
	ERRORCODE(WSAENETUNREACH    , "WSAENETUNREACH"),
	ERRORCODE(WSAENETRESET      , "WSAENETRESET"),
	ERRORCODE(WSAECONNABORTED   , "WSAECONNABORTED"),
	ERRORCODE(WSAECONNRESET     , "WSAECONNRESET"),
	ERRORCODE(WSAENOBUFS        , "WSAENOBUFS"),
	ERRORCODE(WSAEISCONN        , "WSAEISCONN"),
	ERRORCODE(WSAENOTCONN       , "WSAENOTCONN"),
	ERRORCODE(WSAESHUTDOWN      , "WSAESHUTDOWN"),
	ERRORCODE(WSAETOOMANYREFS   , "WSAETOOMANYREFS"),
	ERRORCODE(WSAETIMEDOUT      , "WSAETIMEDOUT"),
	ERRORCODE(WSAECONNREFUSED   , "WSAECONNREFUSED"),
	ERRORCODE(WSAELOOP          , "WSAELOOP"),
	ERRORCODE(WSAENAMETOOLONG   , "WSAENAMETOOLONG"),
	ERRORCODE(WSAEHOSTDOWN      , "WSAEHOSTDOWN"),
	ERRORCODE(WSAEHOSTUNREACH   , "WSAEHOSTUNREACH"),
	ERRORCODE(WSASYSNOTREADY    , "WSASYSNOTREADY"),
	ERRORCODE(WSAVERNOTSUPPORTED, "WSAVERNOTSUPPORTED"),
	ERRORCODE(WSANOTINITIALISED , "WSANOTINITIALISED"),
	ERRORCODE(WSAEDISCON        , "WSAEDISCON"),
	ERRORCODE(WSAHOST_NOT_FOUND , "WSAHOST_NOT_FOUND"),
	ERRORCODE(WSATRY_AGAIN      , "WSATRY_AGAIN"),
	ERRORCODE(WSANO_RECOVERY    , "WSANO_RECOVERY"),
	ERRORCODE(WSANO_DATA        , "WSANO_DATA"),
};

struct LoginServer
{
	char	szName[100];
	char	szAddress[100];
	char	szPort[100];
	char	szFullUsers[50];
	char	szBusyUsers[50];
};

static struct ErrorTable SocketErrors = ERRORTABLE(ErrorCodes);

//structures used to emulate bandwidth and latency parameters - shared by all client interfaces
CPacketBufferStats _pbsSend;
CPacketBufferStats _pbsRecv;

ULONG				cm_ulLocalHost;			// configured local host address
CTString			cm_strAddress;			// local address
CTString			cm_strName;			// local address
unsigned char		cm_chSB1	= 0;
unsigned char		cm_chSB2	= 0;
unsigned char		cm_chSB3	= 0;
unsigned char		cm_chSB4	= 0;

CTCriticalSection cm_csComm;  // critical section for access to communication data

BOOL cm_bNetworkInitialized;

// index 0 is the server's local client, this is an array used by server only
CClientInterface cm_aciClients[SERVER_CLIENTS];

// Broadcast interface - i.e. interface for 'nonconnected' communication
CClientInterface cm_ciBroadcast;

// this is used by client only
CClientInterface cm_ciLocalClient;

// global communication interface object (there is only one for the entire engine)
CCommunicationInterface _cmiComm;

//0105 //0202
SOCKET client_socket;

int client_cnt;

//CInputBuffer m_inbuf;

/*
*
*	Two helper functions - conversion from IP to words
*
*/

// convert address to a printable string
CTString AddressToString(ULONG ulHost)
{
  ULONG ulHostNet = htonl(ulHost);


  // initially not converted
  struct hostent *hostentry = NULL;

  // if DNS lookup is allowed
  if (net_bLookupHostNames) {
    // lookup the host
	  hostentry = gethostbyaddr ((char *)&ulHostNet, sizeof(ulHostNet), AF_INET);
  }

  // if DNS lookup succeeded
	if (hostentry!=NULL) {
    // return its ascii name
    return (char *)hostentry->h_name;
  // if DNS lookup failed
  } else {
    // just convert to dotted number format
    return inet_ntoa((const in_addr &)ulHostNet);
  }
};

// convert string address to a number
ULONG StringToAddress(const CTString &strAddress)
{

  // first try to convert numeric address
  ULONG ulAddress = ntohl(inet_addr(strAddress));
  // if not a valid numeric address
  if (ulAddress==INADDR_NONE) {
    // lookup the host
    HOSTENT *phe = gethostbyname(strAddress);
    // if succeeded
    if (phe!=NULL) {
      // get that address
      ulAddress = ntohl(*(ULONG*)phe->h_addr_list[0]);
    }
  }

  // return what we got
  return ulAddress;
};



CCommunicationInterface::CCommunicationInterface(void)
{
  TRACKMEM(Mem,"Network");
  cm_csComm.cs_iIndex = -1;
  CTSingleLock slComm(&cm_csComm, TRUE);

  cci_bInitialized = FALSE;
  cci_bWinSockOpen = FALSE;

  cci_bServerInitialized = FALSE;
  cci_bClientInitialized = FALSE;
  cm_ciLocalClient.ci_bClientLocal = FALSE;

  cci_hSocket=INVALID_SOCKET;

  //0105
  m_bTcpIp =TRUE; //g_bTcp;//TRUE;//
	//0202
  client_socket = INVALID_SOCKET;
	/*
  for(int i=0;i<10;i++)
  {
	  client_socket[i] = INVALID_SOCKET;
  }
	*/
  client_cnt = 0;
};


// initialize
void CCommunicationInterface::Init(void) 
{
	CTSingleLock slComm(&cm_csComm, TRUE);
	
	cci_bWinSockOpen = FALSE;
	cci_bInitialized = TRUE;
	
// 강동민 수정 시작
	cci_szAddr[0]			= 0;
	cci_iPort				= -1;
	cci_iSelectedServerNum	= -1;
	cci_iFullUsers			= 0;
	cci_iBusyUsers			= 0;
// 강동민 수정 끝
	
	// mark as initialized
	cm_bNetworkInitialized = FALSE;
	
	cci_pbMasterInput.Clear();
	cci_pbMasterOutput.Clear();
	
	cci_bInitUpdateMasterBuffer = TRUE; //0616 updatemasterbuufers()의 초기화할수 있게 한다..

	SetIPFilter();
};

// close
void CCommunicationInterface::Close(void) 
{
  CTSingleLock slComm(&cm_csComm, TRUE);

  ASSERT(cci_bInitialized);
  if(cci_bClientInitialized){    
    Client_Close();
  }
  if(cci_bServerInitialized){
    Server_Close();
  };


  // mark as closed
  cm_bNetworkInitialized = FALSE;
  cci_bInitialized = FALSE;
	cm_ciLocalClient.ci_bClientLocal = FALSE;

	cci_pbMasterInput.Clear();
	cci_pbMasterOutput.Clear();
};

void CCommunicationInterface::InitWinsock(void)
{
	if (cci_bWinSockOpen) 
	{
    return;
  }

  // start winsock
  WSADATA	winsockdata;
  WORD wVersionRequested;
  wVersionRequested = MAKEWORD(2, 2);
  int iResult = WSAStartup(wVersionRequested, &winsockdata);
  // if winsock open ok
	if (iResult==0) 
	{
    // remember that
    cci_bWinSockOpen = TRUE;
    CPrintF(TRANS("  winsock opened ok\n"));
  }
	else
	{
		int iResult = WSACleanup();
		ASSERT(iResult==0);
	}
};

void CCommunicationInterface::EndWinsock(void)
{
	if (!cci_bWinSockOpen) 
	{
    return;
  }

  int iResult = WSACleanup();
  ASSERT(iResult==0);
  cci_bWinSockOpen = FALSE;
};

//강동민 수정 시작 시스템 마우스 작업	09.09
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CCommunicationInterface::Disconnect(void)
{
	if (cci_hSocket != INVALID_SOCKET) 
	{
		closesocket(cci_hSocket);
		cci_bWinSockOpen	= FALSE;
		cci_bSocketOpen		= FALSE;
		cci_hSocket			= INVALID_SOCKET;
		g_hSocket			= cci_hSocket;
		cci_bWinSockOpen	= FALSE;
		cm_ciBroadcast.Clear();
	}
	cci_pbMasterInput.Clear();
	cci_pbMasterOutput.Clear();	
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : strIP - 
//			ulPort - 
//-----------------------------------------------------------------------------
void CCommunicationInterface::Reconnect(CTString strIP, ULONG ulPort)
{
	//if(cci_bWinSockOpen)
	Disconnect();

	CreateSocket_t();
	InitWinsock();

	_tcpip.ConnectToServer(strIP.str_String, ulPort);
	cci_bWinSockOpen	= TRUE;
	cci_bSocketOpen		= TRUE;
	cci_hSocket			= _tcpip.Socket;
}
//강동민 수정 끝 시스템 마우스 작업		09.09


// prepares the comm interface for use - MUST be invoked before any data can be sent/received
BOOL CCommunicationInterface::PrepareForUse(BOOL bUseNetwork, BOOL bClient, BOOL bTcpIp)
{
	// clear the network conditions emulation data
	_pbsSend.Clear();
	_pbsRecv.Clear();
	
	// if the network is already initialized, shut it down before proceeding
	if (cm_bNetworkInitialized) 
	{
		// 소켓을 닫는 부분.
		Unprepare();
	}
	
	// make sure winsock is off (could be on if enumeration was triggered)
	//#if _GAMESPY
	//	GameSpy_EnumCancel();
	//#endif
	EndWinsock();
	
	if (bUseNetwork) 
	{
		CPrintF(TRANS("Initializing TCP/IP...\n"));
		if (bClient && bTcpIp) 
		{						// by seo-40225
			CPrintF(TRANS("  opening as mmo client\n"));
			m_bTcpIp = TRUE;
		} 
		else if (bClient && !bTcpIp) 
		{
			CPrintF(TRANS("  opening as client\n"));
		} 
		else if (!bClient && bTcpIp) 
		{
			CPrintF(TRANS("  opening as mmo test server\n"));
			_pNetwork->IsTcpServer = TRUE;
			m_bTcpIp = TRUE;
		} 
		else 
		{
			CPrintF(TRANS("  opening as server\n"));
			_pNetwork->IsTcpServer = FALSE;
		}
		
		// make sure winsock is on
		InitWinsock();
		
		// no address by default
		cm_ulLocalHost = 0;
		// if there is a desired local address
		if (net_strLocalHost!="") 
		{
			// 현재로서는 쓰이는 net_strLocalHost가 쓰이는 변수가 아닌 것같다. by seo 40225
			CPrintF(TRANS("  user forced local address: %s\n"), (const char*)net_strLocalHost);
			// use that address
			cm_strName = net_strLocalHost;
			cm_ulLocalHost = StringToAddress(cm_strName);
			// if invalid
			if (cm_ulLocalHost==0 || cm_ulLocalHost==-1) 
			{
				cm_ulLocalHost=0;
				// report it
				CPrintF(TRANS("  requested local address is invalid\n"));
			}
		}
		
		// if no valid desired local address
		CPrintF(TRANS("  getting local addresses\n"));

		// get default
		char hostname[256];
		gethostname(hostname, sizeof(hostname)-1);
		cm_strName = hostname;
		// lookup the host
		HOSTENT *phe = gethostbyname(cm_strName);
		
		// if succeeded
		if (phe!=NULL) 
		{
			// get the addresses
			cm_strAddress = "";
			for(INDEX i=0; phe->h_addr_list[i]!=NULL; i++) 
			{
				if (i>0)
				{
					cm_strAddress += ", ";
				}
				cm_strAddress	+= inet_ntoa(*(const in_addr *)phe->h_addr_list[i]);
				cm_chSB1		= ((const in_addr *)phe->h_addr_list[i])->S_un.S_un_b.s_b1;
				cm_chSB2		= ((const in_addr *)phe->h_addr_list[i])->S_un.S_un_b.s_b2;
				cm_chSB3		= ((const in_addr *)phe->h_addr_list[i])->S_un.S_un_b.s_b3;
				cm_chSB4		= ((const in_addr *)phe->h_addr_list[i])->S_un.S_un_b.s_b4;
			}
		}
		
		CPrintF(TRANS("  local addresses: %s (%s)\n"), cm_strName, cm_strAddress);
		CPrintF(TRANS("  port: %d\n"), net_iPort);
		
		// try to open master UDP socket
		try 
		{
			// 소켓을 생성하는 부분.
			OpenSocket_t(cm_ulLocalHost, bClient?0:net_iPort);
			cci_pbMasterInput.pb_ppbsStats = NULL;
			cci_pbMasterOutput.pb_ppbsStats = NULL;
			cm_ciBroadcast.SetLocal(NULL);
		} 
		catch (char *strError) 
		{
			CPrintF(TRANS("  cannot open UDP socket: %s\n"), strError);
			return FALSE;
		}
	}
	
	cm_bNetworkInitialized = cci_bWinSockOpen;
	return TRUE;
};

// shut down the communication interface
void CCommunicationInterface::Unprepare(void)
{
  // close winsock
	if (cci_bWinSockOpen) 
	{
		// if socket is open
		if (cci_hSocket != INVALID_SOCKET) 
		{
			// close it
			closesocket(cci_hSocket);
			cci_hSocket = INVALID_SOCKET;
			g_hSocket	= cci_hSocket;
		}
		
		cm_ciBroadcast.Clear();
		EndWinsock();
		cci_bBound=FALSE;
	}
	
	cci_pbMasterInput.Clear();
	cci_pbMasterOutput.Clear();
	
	cm_bNetworkInitialized = cci_bWinSockOpen;
};

BOOL CCommunicationInterface::IsNetworkEnabled(void)
{
	return cm_bNetworkInitialized;
};

// get address of local machine
void CCommunicationInterface::GetHostName(CTString &strName, CTString &strAddress)
{
	strName		= cm_strName;
	strAddress	= cm_strAddress;
};

/*
*
*
*	Socket functions - creating, binding...
*
*
*/
// create an inet-family socket
void CCommunicationInterface::CreateSocket_t()
{
	//0105 1line 지우기
	//  ASSERT(cci_hSocket==INVALID_SOCKET);
	// open the socket
	//0105
	if(m_bTcpIp)
	{
		cci_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	}
	else
	{
		cci_hSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
	cci_bBound = FALSE;
	if (cci_hSocket == INVALID_SOCKET) 
	{
		ThrowF_t(TRANS("Cannot open socket. %s"), (const char*)GetSocketError(WSAGetLastError()));
	}
};

// bind socket to the given address
void CCommunicationInterface::Bind_t(ULONG ulLocalHost, ULONG ulLocalPort)
{
	if (cci_hSocket==INVALID_SOCKET) {
		ASSERT(FALSE);
		return;
	}
	
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(ulLocalPort);
	sin.sin_addr.s_addr = htonl(ulLocalHost);
	
	// bind socket to server address/port
	if (bind(cci_hSocket, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR) {
		ThrowF_t(TRANS("Cannot bind socket. %s"), (const char*)GetSocketError(WSAGetLastError()));
	}
	
	cci_bBound = TRUE;
};


// set socket to non-blocking mode
void CCommunicationInterface::SetNonBlocking_t(void)
{
	if (cci_hSocket==INVALID_SOCKET) {
		ASSERT(FALSE);
		return;
	}
	
	ULONG ulArgNonBlocking = 1;
	if (ioctlsocket(cci_hSocket, FIONBIO, &ulArgNonBlocking) == SOCKET_ERROR) {
		ThrowF_t(TRANS("Cannot set socket to non-blocking mode. %s"), 
			(const char*)GetSocketError(WSAGetLastError()));
	}
	ULONG ulArgBroadcast = 1;
	if (setsockopt(cci_hSocket, SOL_SOCKET, SO_BROADCAST, (const char*)&ulArgBroadcast, sizeof(int) ) == SOCKET_ERROR) {
		ThrowF_t(TRANS("Cannot allow broadcast on a socket. %s"));
	}
	
};

// get generic socket error info string about last error
CTString CCommunicationInterface::GetSocketError(INDEX iError)
{
	CTString strError;
	strError.PrintF(TRANS("Socket %d, Error %d (%s)"),
		cci_hSocket, iError, ErrorDescription(&SocketErrors, iError));
	return strError;
};

BOOL CCommunicationInterface::CheckIPFilter()
{
	// 회사 내부가 아니라면 IP 필터링...
#if defined VER_TEST
	return FALSE;
#endif
	return TRUE;
}

BOOL CCommunicationInterface::IsValidIP()
{
	//-----------------------------------------------------------------
	BOOL bSatisfied = FALSE;

	hostent *Hostent = gethostbyname(cci_szAddr);
	if (Hostent != NULL)
	{
		// sl.dta에 있던 IP를 얻어옴.
		for( INDEX i = 0; Hostent->h_addr_list[i] != NULL; ++i )
		{
			// (chSB1.chSB2.chSB3.chSB4)
			const unsigned char chSB1 = ((const in_addr *)Hostent->h_addr_list[i])->S_un.S_un_b.s_b1;
			const unsigned char chSB2 = ((const in_addr *)Hostent->h_addr_list[i])->S_un.S_un_b.s_b2;
			const unsigned char chSB3 = ((const in_addr *)Hostent->h_addr_list[i])->S_un.S_un_b.s_b3;
			const unsigned char chSB4 = ((const in_addr *)Hostent->h_addr_list[i])->S_un.S_un_b.s_b4;

			for( std::vector<sIPFilter>::iterator iter = m_vecIPFilter.begin(); iter != m_vecIPFilter.end(); ++iter )
			{
				// 해당하는 나라의 허용 IP 목록들...
				if( g_iCountry == (*iter).iCountry )
				{
					// (chSB1.chSB2.chSB3.chSB4)
					const unsigned char chFilterSB1 = (*iter).chSB1;
					const unsigned char chFilterSB2 = (*iter).chSB2;
					const unsigned char chFilterSB3 = (*iter).chSB3;
					const unsigned char chFilterSB4 = (*iter).chSB4;

					// sl.dta에 있던 아이피와 하드 코딩되어있는 IP가 동일하다면, 조건을 만족함.
					if( chSB1 == chFilterSB1 && chSB2 == chFilterSB2 )
					{
						if( chFilterSB3 == (UCHAR)-1 || chFilterSB3 == chSB3 )
						{
							if( chFilterSB4 == (UCHAR)-1 || chFilterSB4 == chSB4)
							{
								// 조건 만족
								bSatisfied = TRUE;
								break;
							}
						}
					}
				}
			}
		}
	}

	return bSatisfied;
}

void CCommunicationInterface::SetIPFilter()
{
	// IP Filter 목록.
	sIPFilter	TempIP;

	//-------------------IP Filtering List------------------------------
	// NOTE : 기본적으로 앞에 두 클래스에 해당하는 주소는 체크합니다.
	// -1	: 해당 클래스의 모든 번호(?) 허용.
	
	// 중국 아이피를 임시로 열어줌
	for (int i=129; i<=190; ++i)
	{
		TempIP.SetIP( 123, 127, 98, i, KOREA );	// KOREA(119.205.XXX.XXX)
		m_vecIPFilter.push_back( TempIP );
	}

	// 개발 QA
	{
		TempIP.SetIP( 10, 1, 90, -1, g_iCountry );
		m_vecIPFilter.push_back( TempIP );
	}

	// Thai
	{
		TempIP.SetIP( 61, 90, -1, -1, THAILAND);		// THAILAND : MAIN
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 103, 251, -1, -1, THAILAND);		// THAILAND : TEST
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 202, 183, -1, -1, THAILAND);		// THAILAND : LIVE
		m_vecIPFilter.push_back( TempIP );
	}
	
	// Gamigo
	{
		TempIP.SetIP( 178, 19, 73, 77, GERMANY);	// GER_Live : 195.245.86.71
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 178, 19, 73, 92, GERMANY);	// GET_Test : 195.245.86.82
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 178, 19, 73, 91, GERMANY);	// GET_Test : 195.245.86.82
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 178, 19, 73, 80, ITALY);		// 이탈리아 라이브
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 178, 19, 73, 87, ITALY);		// 이탈리아 테섭
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 195, 245, 86, 130, ITALY);	// 이탈리아 테섭(구버전)
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 195, 245, 86, 157, ITALY);	// 이탈리아 테섭(EP2)
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 178, 19, 73, 85, SPAIN) ;		// Spain : test
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 178, 19, 73, 95, SPAIN) ;		// Spain : test
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 178, 19, 73, 78, SPAIN);		// Spain : Main
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 178, 19, 73, 86, FRANCE) ;	// France : test
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 178, 19, 73, 93, FRANCE) ;	// France : test
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 178, 19, 73, 79, FRANCE);		// France : Main
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 178, 19, 73, 89 , POLAND) ;	// Poland : test
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 178, 19, 73, 82, POLAND);		// Poland : Main
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 178, 19, 73, 94, POLAND);		// Poland : EP2
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 46, 253, 149, 249, ENGLAND);
		m_vecIPFilter.push_back( TempIP );
		
		// 유로피안 서버 추가 서버통합으로 인한 로그인 서버 변경 IP 필터링 추가 [11/19/2012 Ranma]
		// 독일
		TempIP.SetIP( 46, 253, 149, 77, GERMANY);	// GER_Live : 46.253.149.77
		m_vecIPFilter.push_back( TempIP );
		// 이탈리아
		TempIP.SetIP( 46, 253, 149, 80, ITALY);		// 이탈리아 라이브  : 46.253.149.80
		m_vecIPFilter.push_back( TempIP );
		// 스페인
		TempIP.SetIP( 46, 253, 149, 78, SPAIN);		// 스페인 라이브  : 46.253.149.78
		m_vecIPFilter.push_back( TempIP );
		// 프랑스
		TempIP.SetIP( 46, 253, 149, 79, FRANCE);	// 프랑스 라이브  : 46.253.149.79
		m_vecIPFilter.push_back( TempIP );
		// 폴란드
		TempIP.SetIP( 46, 253, 149, 82, POLAND);	// 폴란드 라이브  : 46.253.149.82
		m_vecIPFilter.push_back( TempIP );
		// 영국
		TempIP.SetIP( 46, 253, 149, 66, ENGLAND);	// 영국 라이브
		m_vecIPFilter.push_back( TempIP );

		if (FindHosNameIP("test2.it.lc.gamigo.com", TempIP, ITALY) == true)
			m_vecIPFilter.push_back( TempIP );

		// 가미고 로컬의 경우에 유러피안 서버를 추가
		if ( IsGamigo( g_iCountry ) )
		{
			TempIP.SetIP( 46, 253, 149, 200, g_iCountry);	// [2012/08/20 : Sora] 유러피안 서버 테섭 1
			m_vecIPFilter.push_back( TempIP );

			TempIP.SetIP( 46, 253, 149, 201, g_iCountry);	// [2012/08/20 : Sora] 유러피안 서버 테섭 2
			m_vecIPFilter.push_back( TempIP );

			TempIP.SetIP( 46, 253, 149, 112, g_iCountry);	// [11/19/2012 Ranma] 유러피안 라이브
			m_vecIPFilter.push_back( TempIP );

			if (FindHosNameIP("login.ea.lc.gamigo.com", TempIP, g_iCountry) == true)
				m_vecIPFilter.push_back( TempIP );
		}
	}

	{
		TempIP.SetIP( 46, 253, 149, 252, USA);
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 209, 239, 121, 42, USA);
		m_vecIPFilter.push_back( TempIP );
	}

	// Brz, Mex
	{
		TempIP.SetIP( 173, 224, 123, 83, BRAZIL );	// BRZ_가미고 라이브.
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 46, 253, 149, 250, BRAZIL );	// BRZ_가미고 테스트.
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 173, 224, 123, 76, MEXICO );	// MEX_가미고 라이브.
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 46, 253, 149, 251, MEXICO );	// MEX_가미고 테스트.
		m_vecIPFilter.push_back( TempIP );
	}

	// Russia
	{
		TempIP.SetIP( 88, 212, -1, -1, RUSSIA);	// RUS_Live : 88.212.221.201
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 188, 93, -1, -1, RUSSIA);	// RUS_Live : 88.212.221.201
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 86, 62, -1, -1, RUSSIA);		// RUS_Test : 86.62.121.33
		m_vecIPFilter.push_back( TempIP );

		TempIP.SetIP( 46, 253, -1, -1, RUSSIA);		// RUS_Test : 46.253.149.247
		m_vecIPFilter.push_back( TempIP );

		if (FindHosNameIP("login.ru.lc.gamigo.com", TempIP, RUSSIA) == true)
			m_vecIPFilter.push_back( TempIP );
	}

	std::sort( m_vecIPFilter.begin(), m_vecIPFilter.end() );
}

// open an UDP socket at given port
void CCommunicationInterface::OpenSocket_t(ULONG ulLocalHost, ULONG ulLocalPort)
{
	// create the socket as UDP
	CreateSocket_t();
	
	// 서버로 동작시에...
	if(m_bTcpIp && _pNetwork->IsTcpServer)
	{	 
		//	 SetNonBlocking_t(); 
		ULONG ulArgBroadcast = 1;
		
		int i = MAX_PACKET_SIZE;
		if(setsockopt(cci_hSocket,  SOL_SOCKET, SO_SNDBUF, (char *) &i, sizeof(i))== SOCKET_ERROR) {	
			int err = WSAGetLastError();
		}
		
		BOOL bl = 0;
		
		if(setsockopt(cci_hSocket, SOL_SOCKET, SO_LINGER, (char *) &bl, sizeof(BOOL))== SOCKET_ERROR) {	
			int err = WSAGetLastError();
		}
		
		Bind_t(ulLocalHost, ulLocalPort); 
		
		ULONG ulArgNonBlocking = 1;
		if (ioctlsocket(cci_hSocket, FIONBIO, &ulArgNonBlocking) == SOCKET_ERROR) 
		{
			ThrowF_t(TRANS("Cannot set socket to non-blocking mode. %s"), 
				(const char*)GetSocketError(WSAGetLastError()));
		}
		
		// 서버로 동작시에.
		if(listen(cci_hSocket,57)== SOCKET_ERROR) 
		{			
			int err = WSAGetLastError();
		}
		
		cci_bBound = FALSE;
	}
	else if(m_bTcpIp)
	{
		LoginServer loginsvr[MAX_LOGINSERVER];
		memset(&loginsvr, 0, sizeof(LoginServer) * MAX_LOGINSERVER);

		int	ServerNum = ReadInfo(loginsvr);

#ifdef	VER_TEST
		{
			CUIManager::getSingleton()->setIPString(loginsvr[0].szAddress);
		}
#endif	// VER_TEST

		if (ServerNum <= 0)
			return;

		cci_iSelectedServerNum	= rand() % ServerNum;
		cci_iPort				= atoi(loginsvr[cci_iSelectedServerNum].szPort);
		cci_iBusyUsers			= atoi(loginsvr[cci_iSelectedServerNum].szBusyUsers);
		cci_iFullUsers			= atoi(loginsvr[cci_iSelectedServerNum].szFullUsers);

		if(cci_iBusyUsers <= 0)
		{
#if defined (G_HONGKONG)
			cci_iBusyUsers = USER_NUM_BUSY;
#else
			cci_iBusyUsers = USER_NUM_BUSY+100;
#endif
		}
		if(cci_iFullUsers <= 0)
		{
			cci_iFullUsers = USER_NUM_FULL;
		}
		strcpy(cci_szAddr, loginsvr[cci_iSelectedServerNum].szAddress);


		//-------------------------------------------------------------------------------
		//					### 서버 접속 부분 제한 ###
		//
		// NOTE : sl.dta의 앞에 두 자리와 vecIPFilter에 들어가있는 IP를 비교하여,
		// NOTE : 같다면 접속을 허용한다.
		// NOTE : 허용해야 할 IP가 더 추가된다면 vecIPFilter에 추가하고, 
		// NOTE : IP_FILTER를 체크할것.
		// NOTE : 주의!!! 회사 내부에서는 '서버 접속 부분 체크'를 하지 않음. !!!
		//-------------------------------------------------------------------------------
		// FIXME : IP_FILTER와 CLIENT_FILTER가 중복되는 부분이 많고, 따로 함수로 빼야 할것 같음.

		if (CheckIPFilter() == TRUE)
		{
			if (IsValidIP() == FALSE)
			{
				cci_iSelectedServerNum = -1;
				return;
			}
		}
		// 서버에 접속을 시도.
		_tcpip.ConnectToServer(cci_szAddr, cci_iPort); //서버가 띄워져있는 컴퓨터 아이피	

		cci_hSocket = _tcpip.Socket;
		g_hSocket	= cci_hSocket;
	}
	else
	{
		Bind_t(ulLocalHost, ulLocalPort); 	  
		// go non-blocking
		SetNonBlocking_t();
	}
	
	// mark as open
	cci_bSocketOpen = TRUE;
};

// get address of this host
void CCommunicationInterface::GetLocalAddress_t(ULONG &ulHost, ULONG &ulPort)
{
	ASSERT(cci_bBound);
	
	ulHost = 0;
	ulPort = 0;
	if (cci_hSocket==INVALID_SOCKET) 
	{
		ASSERT(FALSE);
		return;
	}
	
	// get socket local port and address
	sockaddr_in sin;
	int iSize = sizeof(sin);
	if (getsockname(cci_hSocket, (sockaddr*)&sin, &iSize) == SOCKET_ERROR) 
	{
		ThrowF_t(TRANS("Cannot get local address on socket. %s"), 
			(const char*)GetSocketError(WSAGetLastError()));
	}
	ulHost = ntohl(sin.sin_addr.S_un.S_addr);
	ulPort = ntohs(sin.sin_port);
}

// get address of the peer host connected to this socket
void CCommunicationInterface::GetRemoteAddress_t(ULONG &ulHost, ULONG &ulPort)
{
	ulHost = 0;
	ulPort = 0;
	if (cci_hSocket==INVALID_SOCKET) {
		ASSERT(FALSE);
		return;
	}
	
	// get socket local port
	sockaddr_in sin;
	int iSize = sizeof(sin);
	if (getpeername(cci_hSocket, (sockaddr*)&sin, &iSize) == SOCKET_ERROR) {
		ThrowF_t(TRANS("Cannot get remote address on socket. %s"), 
			(const char*)GetSocketError(WSAGetLastError()));
	}
	ulHost = ntohl(sin.sin_addr.S_un.S_addr);
	ulPort = ntohs(sin.sin_port);
}


/*
*  ---->>>>  BROADCAST INTERFACE   <<<<----
*/

// broadcast communication
void CCommunicationInterface::Broadcast_Send(const void *pvSend, SLONG slSendSize,CAddress &adrDestination)
{
	CTSingleLock slComm(&cm_csComm, TRUE);
	
	cm_ciBroadcast.ci_adrAddress.adr_ulAddress = adrDestination.adr_ulAddress;
	cm_ciBroadcast.ci_adrAddress.adr_uwPort = adrDestination.adr_uwPort;
	cm_ciBroadcast.ci_adrAddress.adr_uwID = adrDestination.adr_uwID;
	
	cm_ciBroadcast.Send(pvSend, slSendSize,FALSE);
}

BOOL CCommunicationInterface::Broadcast_Receive(void *pvReceive, SLONG &slReceiveSize,CAddress &adrAddress)
{
	CTSingleLock slComm(&cm_csComm, TRUE);
	return cm_ciBroadcast.ReceiveFrom(pvReceive, slReceiveSize,&adrAddress,FALSE);
}

//! 브로드캐스트 인버퍼 업데이트. 연결요청 처리.
// update the broadcast input buffer - handle any incoming connection requests
void CCommunicationInterface::Broadcast_Update_t() 
{
	CPacket* ppaConnectionRequest;
	BOOL bIsAlready;
	BOOL bFoundEmpty;
	ULONG iClient;
	UBYTE ubDummy=65;

	//! 인버퍼에 연결요청 패킷이 들어왔다면,
	// while there is a connection request packet in the input buffer
	while ((ppaConnectionRequest = cm_ciBroadcast.ci_pbReliableInputBuffer.GetConnectRequestPacket()) != NULL) {
		// see if there is a client already connected at that address and port
		bIsAlready = FALSE;
		for (iClient=1;iClient<SERVER_CLIENTS;iClient++) 
		{
			//! 이미 접속해있는 어드레스라면,
			if (cm_aciClients[iClient].ci_adrAddress.adr_ulAddress == ppaConnectionRequest->pa_adrAddress.adr_ulAddress &&
					cm_aciClients[iClient].ci_adrAddress.adr_uwPort == ppaConnectionRequest->pa_adrAddress.adr_uwPort) {
		   //! 그건 접속시도하다 실패한 것이니 클리어한다.
          // if it is, this client probably failed to connect the last time, so clear it
          _pNetwork->ga_srvServer.HandleClientDisconected(iClient);
			}
		}
		// if the client is already connected then just ignore the packet - else, connect it
		// find an empty client structure
		bFoundEmpty = FALSE;
		for (iClient=1;iClient<SERVER_CLIENTS;iClient++) {
			//! 사용하지 않는 array를 찾았다면, 그곳에 클라이언트 주소와 포트 저장.
			if (cm_aciClients[iClient].ci_bUsed == FALSE) {
				bFoundEmpty = TRUE;
				//0105 여기서 클라이언트 주소와 포트를 저장한다.
				if(m_bTcpIp&& _pNetwork->IsTcpServer){//서버
					int sockaddr_size = sizeof( SOCKADDR_IN );		
					SOCKADDR_IN clientsockaddr;	
					//0131	//0202				
					
					getpeername( client_socket, (LPSOCKADDR)&clientsockaddr, &sockaddr_size);
					
					
					cm_aciClients[iClient].ci_adrAddress.adr_ulAddress = ntohl(clientsockaddr.sin_addr.s_addr);
					cm_aciClients[iClient].ci_adrAddress.adr_uwPort = ntohs(clientsockaddr.sin_port);
					
				}else{
					// we have an empty slot, so fill it for the client
					cm_aciClients[iClient].ci_adrAddress.adr_ulAddress = ppaConnectionRequest->pa_adrAddress.adr_ulAddress;
					cm_aciClients[iClient].ci_adrAddress.adr_uwPort = ppaConnectionRequest->pa_adrAddress.adr_uwPort;
				}
				//! ID생성. 
				// generate the ID
				UWORD uwID = _pTimer->GetHighPrecisionTimer().tv_llValue&0x0FFF;
				if (uwID==0 || uwID=='//') {
					uwID+=1;
				}										
				cm_aciClients[iClient].ci_adrAddress.adr_uwID = (uwID<<4)+iClient;

				// form the connection response packet
				ppaConnectionRequest->pa_adrAddress.adr_uwID = '//';
				ppaConnectionRequest->pa_uwReliable = UDP_PACKET_RELIABLE | UDP_PACKET_RELIABLE_HEAD | UDP_PACKET_RELIABLE_TAIL | UDP_PACKET_CONNECT_RESPONSE;
				//! 클라이언트에 할당된 ID를 알려준다.
				// return it to the client
				ppaConnectionRequest->WriteToPacket(&(cm_aciClients[iClient].ci_adrAddress.adr_uwID),sizeof(cm_aciClients[iClient].ci_adrAddress.adr_uwID),ppaConnectionRequest->pa_uwReliable,cm_ciBroadcast.ci_ulSequence++,ppaConnectionRequest->pa_adrAddress.adr_uwID,sizeof(cm_aciClients[iClient].ci_adrAddress.adr_uwID));
				if (cm_ciBroadcast.ci_pbOutputBuffer.AppendPacket(*ppaConnectionRequest,TRUE,FALSE) == FALSE) {
					ASSERT (FALSE);
				}
				//! 이 클라이언트는 이제 사용되는거다.
				cm_aciClients[iClient].ci_bUsed = TRUE;
				//0131
				//				client_cnt++;

				return;
			}
		}

		// if none found
		if (!bFoundEmpty) {
			// error
			ThrowF_t(TRANS("Server: Cannot accept new clients, all slots used!\n"));
		}

	}

};


/*
*  ---->>>>  SERVER  <<<<----
*/

/*
*  Initialize server
*/
void CCommunicationInterface::Server_Init_t(void) 
{
  CTSingleLock slComm(&cm_csComm, TRUE);

  ASSERT(cci_bInitialized);
  ASSERT(!cci_bServerInitialized);
	


  // for each client
  for(INDEX iClient=0; iClient<SERVER_CLIENTS; iClient++) {
    // clear its status
    cm_aciClients[iClient].Clear();
		cm_aciClients[iClient].ci_pbOutputBuffer.pb_ppbsStats = &_pbsSend;
		cm_aciClients[iClient].ci_pbInputBuffer.pb_ppbsStats = &_pbsRecv;
  }

	// mark the server's instance of the local client as such
	cm_aciClients[SERVER_LOCAL_CLIENT].ci_bClientLocal = TRUE;
	cm_aciClients[SERVER_LOCAL_CLIENT].ci_bUsed = TRUE;

	// prepare the client part of the local client 
	cm_ciLocalClient.Clear();
	cm_ciLocalClient.ci_bUsed = TRUE;
	cm_ciLocalClient.ci_bClientLocal = TRUE;
	cm_ciLocalClient.ci_pbOutputBuffer.pb_ppbsStats = &_pbsSend;
	cm_ciLocalClient.ci_pbInputBuffer.pb_ppbsStats = &_pbsRecv;



  // mark that the server was initialized
  cci_bServerInitialized = TRUE;
};

/*
*  Close server
*/
void CCommunicationInterface::Server_Close(void) 
{
  CTSingleLock slComm(&cm_csComm, TRUE);

  // close all clients
  for (INDEX iClient=0; iClient<SERVER_CLIENTS; iClient++) {
    cm_aciClients[iClient].Clear();
  }

  // mark that the server is uninitialized
  cci_bServerInitialized = FALSE;
};

/*
*  Server clear client and prepare for new connection
*/
void CCommunicationInterface::Server_ClearClient(INDEX iClient)
{
  // synchronize access to communication data
  CTSingleLock slComm(&cm_csComm, TRUE);

  ASSERT(iClient>=0 && iClient<SERVER_CLIENTS);
  cm_aciClients[iClient].Clear();
};

BOOL CCommunicationInterface::Server_IsClientLocal(INDEX iClient)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  ASSERT(iClient>=0 && iClient<SERVER_CLIENTS);
  return iClient==SERVER_LOCAL_CLIENT;
};

BOOL CCommunicationInterface::Server_IsClientUsed(INDEX iClient)
{
  CTSingleLock slComm(&cm_csComm, TRUE);

  ASSERT(iClient>=0 && iClient<SERVER_CLIENTS);
  return cm_aciClients[iClient].ci_bUsed;
};

CTString CCommunicationInterface::Server_GetClientName(INDEX iClient)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  ASSERT(iClient>=0 && iClient<SERVER_CLIENTS);

  if (iClient==SERVER_LOCAL_CLIENT) {
    return TRANS("Local machine");
  }

  cm_aciClients[iClient].ci_strAddress = AddressToString(cm_aciClients[iClient].ci_adrAddress.adr_ulAddress);

  return cm_aciClients[iClient].ci_strAddress;
};


ULONG CCommunicationInterface::Server_GetClientPendingData(INDEX iClient)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  ASSERT(iClient>=0 && iClient<SERVER_CLIENTS);
  return cm_aciClients[iClient].GetPendingDataSize();
};

ULONG CCommunicationInterface::Server_GetMaxPendingData()
{
  CTSingleLock slComm(&cm_csComm, TRUE);

  ULONG ulMaxData = 0;
  ULONG ulData;
  for (INDEX iClient=0;iClient<SERVER_CLIENTS;iClient++) {
    ulData = cm_aciClients[iClient].GetPendingDataSize();
    if (ulData > ulMaxData) {
      ulMaxData = ulData;
    }
  }
  return ulMaxData;
};


ULONG CCommunicationInterface::Server_GetTotalPendingData()
{
  CTSingleLock slComm(&cm_csComm, TRUE);

  ULONG ulData = 0;
  for (INDEX iClient=0;iClient<SERVER_CLIENTS;iClient++) {
    ulData += cm_aciClients[iClient].GetPendingDataSize();
  }
  return ulData;
};

/*
*  Server Send/Receive Reliable
*/
void CCommunicationInterface::Server_Send_Reliable(INDEX iClient, const void *pvSend, SLONG slSendSize)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  ASSERT(iClient>=0 && iClient<SERVER_CLIENTS);
  cm_aciClients[iClient].Send(pvSend, slSendSize,TRUE);
  Server_Update();
};

BOOL CCommunicationInterface::Server_Receive_Reliable(INDEX iClient, void *pvReceive, SLONG &slReceiveSize)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  ASSERT(iClient>=0 && iClient<SERVER_CLIENTS);
  return cm_aciClients[iClient].Receive(pvReceive, slReceiveSize,TRUE);
};

/*
*  Server Send/Receive Unreliable
*/
void CCommunicationInterface::Server_Send_Unreliable(INDEX iClient, const void *pvSend, SLONG slSendSize)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  ASSERT(iClient>=0 && iClient<SERVER_CLIENTS);
  cm_aciClients[iClient].Send(pvSend, slSendSize,FALSE);
  Server_Update();
};

BOOL CCommunicationInterface::Server_Receive_Unreliable(INDEX iClient, void *pvReceive, SLONG &slReceiveSize)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  ASSERT(iClient>=0 && iClient<SERVER_CLIENTS);
  return cm_aciClients[iClient].Receive(pvReceive, slReceiveSize,FALSE);
};

//! 서버 업데이트
BOOL CCommunicationInterface::Server_Update()
{

	CTSingleLock slComm(&cm_csComm, TRUE);
	//	CPacket *ppaPacket;
	//	CPacket *ppaPacketCopy;
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
	//	INDEX iClient;
	
	//! 로컬 클라이언트와 클라이언트 0번째 array의 버퍼를 맞교환 한다. input버퍼<->out버퍼
	// transfer packets for the local client
	if (cm_ciLocalClient.ci_bUsed && cm_ciLocalClient.ci_pciOther != NULL) {
		cm_ciLocalClient.ExchangeBuffers();
	};
	
	/* //0311 삭제
	//! out버퍼 업데이트.
	cm_aciClients[0].UpdateOutputBuffers();
	
	//! 싱클 플레이가 아니라면,
	// if not just playing single player
	if (cci_bServerInitialized) {
		try {
			//! 브로드 캐스트 업데이트.
			Broadcast_Update_t();
		} catch (char *strError) {
			CPrintF(strError);
		}
		
		//! 모든 클라이언트에 대하여 out버퍼에서 마스터 out버퍼로 패킷을 전송한다. 로컬 제외.
		// for each client transfer packets from the output buffer to the master output buffer
		for (iClient=1; iClient<SERVER_CLIENTS; iClient++) {
			CClientInterface &ci = cm_aciClients[iClient];
			//! 그 클라이언트가 연결되어 있지 않으면 skip
			// if not connected
			if (!ci.ci_bUsed) {
				// skip it
				continue;
			}
			//! out버퍼 업데이트.
			// update its buffers, if a reliable packet is overdue (has not been delivered too long)
			// disconnect the client			
			if (ci.UpdateOutputBuffers() != FALSE) {
				//! 만약 아웃버퍼의 첫번째패킷이 보내질 시간을 기다리는 패킷이 있다면,??
				//!패킷이 보내질 시간이라면 마스터 아웃버퍼로 전송한다.
				// transfer packets ready to be sent out to the master output buffer
				while (ci.ci_pbOutputBuffer.pb_ulNumOfPackets > 0) {
					ppaPacket = ci.ci_pbOutputBuffer.PeekFirstPacket();
					//! 보낼 시간이라면,
					if (ppaPacket->pa_tvSendWhen < tvNow) {
						ci.ci_pbOutputBuffer.RemoveFirstPacket(FALSE);
						//0105 ACK패킷버퍼 확인 하지 않기
						if(!(m_bTcpIp&& _pNetwork->IsTcpServer)){//서버
							//! 릴라이어블 패킷이라면 아크패킷 생성.
							if (ppaPacket->pa_uwReliable & UDP_PACKET_RELIABLE) {
								ppaPacketCopy = CreatePacket();
								*ppaPacketCopy = *ppaPacket;
								if (ci.ci_pbWaitAckBuffer.AppendPacket(*ppaPacketCopy,FALSE,FALSE) == FALSE) {
									ASSERT(FALSE);
								}
							}
						}
						//! 마스터 out버퍼에 넣는다.
						if (cci_pbMasterOutput.AppendPacket(*ppaPacket,FALSE,FALSE) == FALSE){
							ASSERT(FALSE);
						}
					} else {
						break;
					}
				}
			} else {
				CPrintF(TRANS("Unable to deliver data to client '%s', disconnecting.\n"),AddressToString(cm_aciClients[iClient].ci_adrAddress.adr_ulAddress));
				Server_ClearClient(iClient);
				_pNetwork->ga_srvServer.HandleClientDisconected(iClient);
				
			}
		}
		
		// update broadcast output buffers
		// update its buffers
		cm_ciBroadcast.UpdateOutputBuffers();
		//! 브로드캐스트 아웃버퍼에 패킷이 있다면,
		// transfer packets ready to be sent out to the master output buffer
		while (cm_ciBroadcast.ci_pbOutputBuffer.pb_ulNumOfPackets > 0) {
			ppaPacket = cm_ciBroadcast.ci_pbOutputBuffer.PeekFirstPacket();
			//! 보낼 시간이 되었다면, 
			if (ppaPacket->pa_tvSendWhen < tvNow) {
				cm_ciBroadcast.ci_pbOutputBuffer.RemoveFirstPacket(FALSE);
				if (cci_pbMasterOutput.AppendPacket(*ppaPacket,FALSE,FALSE) == FALSE) {
					ASSERT(FALSE);
				}
			} else {
				break;
			}
		}
	*/		
		// send/receive packets over the TCP/IP stack
		UpdateMasterBuffers();
	/* //0311 삭제
		//! 마스터 in버퍼에 패킷이 들어있다면,
		// dispatch all packets from the master input buffer to the clients' input buffers
		while (cci_pbMasterInput.pb_ulNumOfPackets > 0) {
			BOOL bClientFound;
			ppaPacket = cci_pbMasterInput.GetFirstPacket();
			bClientFound = FALSE;
			//! 접속허가 요청하는 브로트캐스트 패킷이라면,
			if (ppaPacket->pa_adrAddress.adr_uwID=='//' || ppaPacket->pa_adrAddress.adr_uwID==0) {
				if (cm_ciBroadcast.ci_pbInputBuffer.AppendPacket(*ppaPacket,FALSE,TRUE) == FALSE) {
					DeletePacket(ppaPacket);
				}
				bClientFound = TRUE;
			} else {
				for (iClient=0; iClient<SERVER_CLIENTS; iClient++) {
					//! 각 클라이언트 아이디 확인
					if (ppaPacket->pa_adrAddress.adr_uwID == cm_aciClients[iClient].ci_adrAddress.adr_uwID) {
						//! 해당 인버퍼에  패킷 넣기.
						if (cm_aciClients[iClient].ci_pbInputBuffer.AppendPacket(*ppaPacket,FALSE,TRUE) == FALSE) {
							DeletePacket(ppaPacket);
						}
						bClientFound = TRUE;
						break;
					}
				}
			}
			if (!bClientFound) {
				// warn about possible attack
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("WARNING: Invalid message from: %s\n"), AddressToString(ppaPacket->pa_adrAddress.adr_ulAddress));
				}
			}
		}
		//! 모든 클라이언트 인버퍼 업데이트
		for (iClient=1; iClient<SERVER_CLIENTS; iClient++) {
			cm_aciClients[iClient].UpdateInputBuffers();
		}
		
		
	}
	*/	
	cm_aciClients[0].UpdateInputBuffers();
	//! 로컬 인버퍼 업데이트
	cm_ciLocalClient.UpdateInputBuffers();
	/* //0311 삭제
	//! 브로드캐스트 인버퍼 업데이트
	cm_ciBroadcast.UpdateInputBuffersBroadcast();
	//! 브로드캐스트 업데이트
	Broadcast_Update_t();
	*/	
	return TRUE;
};

/*
*  ---->>>>  CLIENT  <<<<----
*/

/*
*  Initialize client
*/
//!클라이언트 초기화
void CCommunicationInterface::Client_Init_t(char* strServerName)
{
  CTSingleLock slComm(&cm_csComm, TRUE);

  ASSERT(cci_bInitialized);
  ASSERT(!cci_bClientInitialized);

  // retrieve server address from server name
  ULONG ulServerAddress = StringToAddress(strServerName);
  // if lookup failed
  if (ulServerAddress==INADDR_NONE) {
    ThrowF_t(TRANS("Host '%s' not found!\n"), strServerName);
  }

  // call client init with server address
  Client_Init_t(ulServerAddress);


};

void CCommunicationInterface::Client_Init_t(ULONG ulServerAddress)
{
  CTSingleLock slComm(&cm_csComm, TRUE);

  ASSERT(cci_bInitialized);
  ASSERT(!cci_bClientInitialized);
	//! 클라이언트 인터페이스 초기화.out버퍼,In버퍼 state 초기화.
  cm_ciLocalClient.Clear();
	cm_ciLocalClient.ci_pbOutputBuffer.pb_ppbsStats = &_pbsSend;
	cm_ciLocalClient.ci_pbInputBuffer.pb_ppbsStats = &_pbsRecv;
	//! 서버가 아니라면(클라이언트 초기화 하고 넘어온..)
  // if this computer is not the server
  if (!cci_bServerInitialized) {
    // open with connecting to remote server
    cm_ciLocalClient.ci_bClientLocal= FALSE;
	//! 여기 들어가서 서버 접속승인을 받는다.
    Client_OpenNet_t(ulServerAddress);

  // if this computer is server
  } else {
		//! 서버라면 로컬 클라이언트 설정.
    // open local client
    cm_ciLocalClient.ci_bClientLocal = TRUE;
    Client_OpenLocal();
  }
	//! 클라이언트 초기화 되었음.
  cci_bClientInitialized = TRUE;
};

/*
*  Close client
*/
void CCommunicationInterface::Client_Close(void)
{
  CTSingleLock slComm(&cm_csComm, TRUE);

  ASSERT(cci_bInitialized);

	// dispatch remaining packets (keep trying for half a second - 10 attempts)
  for(TIME tmWait=0; tmWait<500;
    Sleep(NET_WAITMESSAGE_DELAY), tmWait+=NET_WAITMESSAGE_DELAY) {
    // if all packets are successfully sent, exit loop
		if  ((cm_ciLocalClient.ci_pbOutputBuffer.pb_ulNumOfPackets == 0) 
			&& (cm_ciLocalClient.ci_pbWaitAckBuffer.pb_ulNumOfPackets == 0)) {
				break;
			}
    if (Client_Update() == FALSE) {
			break;
		}
	}

  cm_ciLocalClient.Clear();

  cm_ciLocalClient.ci_bClientLocal= FALSE;
  cci_bClientInitialized = FALSE;
};


/*
*  Open client local
*/
//! 서버에서 쓰는 로컬 클라이언트 오픈
void CCommunicationInterface::Client_OpenLocal(void)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  //! 서버상 0번째 클라이언트 테이블과 로컬 클라이언트 인터페이스를 연결한다.
  CClientInterface &ci0 = cm_ciLocalClient;
  CClientInterface &ci1 = cm_aciClients[SERVER_LOCAL_CLIENT];
  //! 서로 로컬로 지정.  
  ci0.ci_bUsed = TRUE;
  ci0.SetLocal(&ci1);
  ci1.ci_bUsed = TRUE;
  ci1.SetLocal(&ci0);
};


/*
*  Open client remote
*/
//! 클라이언트 오픈.
void CCommunicationInterface::Client_OpenNet_t(ULONG ulServerAddress)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
	CPacket* ppaInfoPacket;
	CPacket* ppaReadPacket;
	UBYTE ubDummy=65;
	UBYTE ubReliable;

  // check for reconnection
  static ULONG ulLastServerAddress = -1;
  BOOL bReconnecting = ulServerAddress == ulLastServerAddress;
  ulLastServerAddress = ulServerAddress;

  const INDEX iRefresh = 100; // (delay between retries in miliseconds)
  // determine connection timeout
  INDEX ctRetries = bReconnecting?(net_tmConnectionTimeout*1000/2/iRefresh):5;

  // start waiting for server's response
  if (ctRetries>1) {
    CallProgressHook_t(0.0f);
  }

	//!	연결 요청 패킷을 만든다.
	// form the connection request packet
	ppaInfoPacket = CreatePacket();
	ubReliable = UDP_PACKET_RELIABLE | UDP_PACKET_RELIABLE_HEAD | UDP_PACKET_RELIABLE_TAIL | UDP_PACKET_CONNECT_REQUEST;
	ppaInfoPacket->pa_adrAddress.adr_ulAddress = ulServerAddress;
	ppaInfoPacket->pa_adrAddress.adr_uwPort = net_iPort;
	ppaInfoPacket->pa_ubRetryNumber = 0;
	ppaInfoPacket->WriteToPacket(&ubDummy,1,ubReliable,cm_ciLocalClient.ci_ulSequence++,'//',1);
	//! 연결요청패킷을 아웃버퍼에 넣는다.
	if (cm_ciLocalClient.ci_pbOutputBuffer.AppendPacket(*ppaInfoPacket,TRUE,FALSE) == FALSE){
    ASSERT(FALSE);
  }
	//! 서버 주소와 포트 번호를 클라이언트 인터페이스에 셋팅
	// set client destination address to server address
	cm_ciLocalClient.ci_adrAddress.adr_ulAddress = ulServerAddress;
	cm_ciLocalClient.ci_adrAddress.adr_uwPort = net_iPort;
	
  // for each retry
	for(INDEX iRetry=0; iRetry<ctRetries; iRetry++) 
	{
	    //! 버퍼를 업데이트 하여 아웃버퍼안에 있는 접속요청패킷을 보내고, 접속승인 패킷을 기다린다.
		// send/receive and juggle the buffers
		if (Client_Update() == FALSE) 
		{
			break;
		}
		//! 릴라이어블 Input버퍼에 어떠한 패킷이 들어와 있다면,
		// if there is something in the input buffer
		if (cm_ciLocalClient.ci_pbReliableInputBuffer.pb_ulNumOfPackets > 0) 
		{
			//! 첫패킷을 얻어와서
			ppaReadPacket = cm_ciLocalClient.ci_pbReliableInputBuffer.GetFirstPacket();
			//! 그게 접속승인 패킷이라면 
			// and it is a connection confirmation
			if (ppaReadPacket->pa_uwReliable &  UDP_PACKET_CONNECT_RESPONSE) 
			{
				// the client has succedeed to connect, so read the uwID from the packet
				//! 보내온 패킷에서 adr_uwID을 얻는다.
				cm_ciLocalClient.ci_adrAddress.adr_ulAddress = ulServerAddress;
				cm_ciLocalClient.ci_adrAddress.adr_uwPort = net_iPort;
				cm_ciLocalClient.ci_adrAddress.adr_uwID = *((UWORD*) (ppaReadPacket->pa_pubPacketData + MAX_HEADER_SIZE));
				cm_ciLocalClient.ci_bUsed = TRUE;
				cm_ciLocalClient.ci_bClientLocal = FALSE;
				cm_ciLocalClient.ci_pciOther = NULL;
				//! 릴라이어블 input버퍼에서 그 패킷을 지운다.
				cm_ciLocalClient.ci_pbReliableInputBuffer.RemoveConnectResponsePackets();

				DeletePacket(ppaReadPacket);

				// finish waiting
				CallProgressHook_t(1.0f);		    
				return;
			}
		}

    Sleep(iRefresh);
    CallProgressHook_t(FLOAT(iRetry)/ctRetries);
	}
	
	cci_bBound = FALSE;
	ThrowF_t(TRANS("Client: Timeout receiving UDP port"));
};

/*
*  Clear local client
*/

void CCommunicationInterface::Client_Clear(void)
{
  // synchronize access to communication data
  CTSingleLock slComm(&cm_csComm, TRUE);

  cm_ciLocalClient.Clear();
};

/*
*  Client get status
*/
BOOL CCommunicationInterface::Client_IsConnected(void)
{
  // synchronize access to communication data
  CTSingleLock slComm(&cm_csComm, TRUE);

  return cm_ciLocalClient.ci_bUsed;
};

/*
*  Client Send/Receive Reliable
*/
void CCommunicationInterface::Client_Send_Reliable(const void *pvSend, SLONG slSendSize)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  //! output버퍼에 넣기.
  cm_ciLocalClient.Send(pvSend, slSendSize,TRUE);
  //! 로컬이라면 서버 업데이트.
  if (cci_bServerInitialized) {
    Server_Update();
  } else {
    Client_Update();
  }
};

BOOL CCommunicationInterface::Client_Receive_Reliable(void *pvReceive, SLONG &slReceiveSize)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  return cm_ciLocalClient.Receive(pvReceive, slReceiveSize,TRUE);
};

BOOL CCommunicationInterface::Client_Receive_Reliable(CTStream &strmReceive)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  return cm_ciLocalClient.Receive(strmReceive,TRUE);
};
//! 패킷에 적혀있는 전송사이즈(slExpectedSize)와 실제로 버퍼에 존재하는 릴라이어블 패킷의 사이즈(slReceivedSize)를 얻어낸다.
void CCommunicationInterface::Client_PeekSize_Reliable(SLONG &slExpectedSize,SLONG &slReceivedSize)
{
  slExpectedSize = cm_ciLocalClient.GetExpectedReliableSize();
  slReceivedSize = cm_ciLocalClient.GetCurrentReliableSize();
}

/*
*  Client Send/Receive Unreliable
*/
void CCommunicationInterface::Client_Send_Unreliable(const void *pvSend, SLONG slSendSize)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  cm_ciLocalClient.Send(pvSend, slSendSize,FALSE);

  if (cci_bServerInitialized) {
    Server_Update();
  } else {
    Client_Update();
  }
};

BOOL CCommunicationInterface::Client_Receive_Unreliable(void *pvReceive, SLONG &slReceiveSize)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  return cm_ciLocalClient.Receive(pvReceive, slReceiveSize,FALSE);
};

//0311 kwon 함수 추가
void CCommunicationInterface::Client_Send_Unreliable_New(const void *pvSend, SLONG slSendSize, BOOL bLogin)
{
  CTSingleLock slComm(&cm_csComm, TRUE);
  cm_ciLocalClient.SendNew(pvSend, slSendSize);
  
  //0524 kwon
  if(!bLogin){
	  if (cci_bServerInitialized) {
		  Server_Update();
	  } else {
		  Client_Update();
	  }
  }
};

//0311 kwon 함수추가. 여기서 마스터 input 버퍼에서 패킷 읽어들여 sessionstate에서 써야한다.
BOOL CCommunicationInterface::Client_Receive_Unreliable_New(void *pvReceive, SLONG &slReceiveSize, INDEX &index)
{
	CTSingleLock slComm(&cm_csComm, TRUE);
	
	ASSERT (slReceiveSize>0);
	ASSERT (pvReceive != NULL);
	
	
	CPacket* ppaPacket;
	UBYTE* pubData = (UBYTE*) pvReceive;
	SLONG slDummySize;
	UWORD uwReliable;
	
	// if there are no packets in the input buffer, return
	if (cci_pbMasterInput.pb_ulNumOfPackets == 0) {
		return FALSE;	
	} 
	ppaPacket = cci_pbMasterInput.PeekFirstPacket();

	// if the first packet in the input buffer is not unreliable
	if (!ppaPacket->IsUnreliable()) {
		return FALSE;
		// if there is no complete message ready
	} else if (cci_pbMasterInput.CheckSequence(slDummySize,FALSE) == FALSE) {
		return FALSE;
		// if the ready message is longer than the expected size
	} else if ( ppaPacket->pa_slTransferSize > slReceiveSize) {
		return FALSE;
		// if everything is ok, read the packet data, and kill the packet
	} else {
		
		slReceiveSize = 0;
		do {
			ppaPacket = cci_pbMasterInput.GetFirstPacket();
			uwReliable = ppaPacket->pa_uwReliable;
			slDummySize = ppaPacket->pa_slSize - MAX_HEADER_SIZE;
			ppaPacket->ReadFromPacket(pubData,slDummySize);
			pubData += slDummySize;
			slReceiveSize += slDummySize;
			//0322 kwon
			index = ppaPacket->pa_adrAddress.adr_uwID;
			DeletePacket(ppaPacket);
		} while (!(uwReliable & UDP_PACKET_UNRELIABLE_TAIL));
		
		return TRUE;
	}
	
	return FALSE;
};

BOOL CCommunicationInterface::Client_Update(void)
{
	CTSingleLock slComm(&cm_csComm, TRUE);
	CPacket *ppaPacket;
	CPacket *ppaPacketCopy;
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();


    //! 클라이언트의 아웃버퍼 갱신
	// update local client's output buffers
	if (cm_ciLocalClient.UpdateOutputBuffers() == FALSE) {
		return FALSE;
	}

	// if not playing on the server (i.e. connected to a remote server)
	if (!cci_bServerInitialized) {
		// put all pending packets in the master output buffer
		while (cm_ciLocalClient.ci_pbOutputBuffer.pb_ulNumOfPackets > 0) {
			ppaPacket = cm_ciLocalClient.ci_pbOutputBuffer.PeekFirstPacket();
			//! pa_tvSendWhen는 재시도 간격
			if (ppaPacket->pa_tvSendWhen < tvNow) {
				//! 다시 보낼 시간이 되었다면 아웃버퍼에서 지우고,
				cm_ciLocalClient.ci_pbOutputBuffer.RemoveFirstPacket(FALSE);
				//! 릴라이어블 패킷이라면 아크버퍼에 패킷을 복사해 놓는다.
				if (ppaPacket->pa_uwReliable & UDP_PACKET_RELIABLE) {
					ppaPacketCopy = CreatePacket();
					*ppaPacketCopy = *ppaPacket;
					if (cm_ciLocalClient.ci_pbWaitAckBuffer.AppendPacket(*ppaPacketCopy,FALSE,FALSE) == FALSE){
						ASSERT(FALSE);
					}
				}
				//! 패킷을 마스터아웃버퍼에 넣는다.
				if (cci_pbMasterOutput.AppendPacket(*ppaPacket,FALSE,FALSE) == FALSE){
					ASSERT(FALSE);
				}

			} else {
				break;
			}
		}

		//! 브로드캐스트 클라이언트의 아웃버퍼 갱신
		// update broadcast output buffers
		// update its buffers
		cm_ciBroadcast.UpdateOutputBuffers();
        //! 브로드캐스트 클라이언트의 아웃버퍼에 마스터아웃버퍼로 보낼 패킷이 있다면 
		// transfer packets ready to be sent out to the master output buffer
		while (cm_ciBroadcast.ci_pbOutputBuffer.pb_ulNumOfPackets > 0) {
			ppaPacket = cm_ciBroadcast.ci_pbOutputBuffer.PeekFirstPacket();
			//! 보낼시간이 되었다면 마스터아웃버퍼로 보낸다.
			if (ppaPacket->pa_tvSendWhen < tvNow) {
				cm_ciBroadcast.ci_pbOutputBuffer.RemoveFirstPacket(FALSE);
				if (cci_pbMasterOutput.AppendPacket(*ppaPacket,FALSE,FALSE) == FALSE){
					ASSERT(FALSE);
				}
			} else {
				break;
			}
		}

		//! 마스터버퍼 갱신
		// send/receive packets over the TCP/IP stack
		UpdateMasterBuffers();

		//마스터 input 버퍼에 패킷이 있다면 
		// dispatch all packets from the master input buffer to the clients' input buffers
		while (cci_pbMasterInput.pb_ulNumOfPackets > 0) {
			BOOL bClientFound;
			ppaPacket = cci_pbMasterInput.GetFirstPacket();
			bClientFound = FALSE;
			
			//! 패킷주소가 브로드캐스트이고 언릴라어어블 전송이었다면, 그것을 브로드캐스트 input버퍼에 넣는다.
			// if the packet address is broadcast and it's an unreliable transfer, put it in the broadcast buffer
			if ((ppaPacket->pa_adrAddress.adr_uwID=='//' || ppaPacket->pa_adrAddress.adr_uwID==0) && 
				ppaPacket->IsUnreliable()) {
				if (cm_ciBroadcast.ci_pbInputBuffer.AppendPacket(*ppaPacket,FALSE,TRUE) == FALSE) {
					DeletePacket(ppaPacket);
				}
				bClientFound = TRUE;
				//! 이패킷의 adr_uwID와 이 클라이언트의 adr_uwID가 일치한다면 클라이언트 Input버퍼에 넣는다.
				// if the packet is for this client, accept it
			} else if ((ppaPacket->pa_adrAddress.adr_uwID == cm_ciLocalClient.ci_adrAddress.adr_uwID) || 
				ppaPacket->pa_adrAddress.adr_uwID=='//' || ppaPacket->pa_adrAddress.adr_uwID==0) { 
				
				if (cm_ciLocalClient.ci_pbInputBuffer.AppendPacket(*ppaPacket,FALSE,TRUE) == FALSE) {
					DeletePacket(ppaPacket);
				}
				bClientFound = TRUE;			
			}

			if (!bClientFound) {
				// warn about possible attack
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("WARNING: Invalid message from: %s\n"), AddressToString(ppaPacket->pa_adrAddress.adr_ulAddress));
				}
			}
		}

	}
    //! 클라이언트 input버퍼를 갱신.브로드캐스트 input버퍼 갱신
	cm_ciLocalClient.UpdateInputBuffers();
	cm_ciBroadcast.UpdateInputBuffersBroadcast();

	return TRUE;
};

// update master UDP socket and route its messages
void CCommunicationInterface::UpdateMasterBuffers() 
{
	//강동민 수정 시작 로그인 처리 작업	07.20
	// NOTE : 로그인 시 네트워크 메세지가 리턴을 하게되면 가지 않는다!!!
	// NOTE : Send, Recv를 하지 않기 때문에...
	extern BOOL _bWorldEditorApp;
	extern BOOL _bUseSocket;
	extern BOOL _bLoginProcess;
	//if((_bLoginProcess && !_bUseSocket) || _bWorldEditorApp)
	//{
	//	return;
	//}
	//강동민 수정 끝 로그인 처리 작업		07.20
	UBYTE aub[MAX_PACKET_SIZE];
	CAddress adrIncomingAddress;
	SOCKADDR_IN sa;
	int size = sizeof(sa);
	SLONG slSizeReceived;
	SLONG slSizeSent;
	BOOL bSomethingDone;
	CPacket* ppaNewPacket;
	CTimerValue tvNow;
	static TIME  tmLastBandwithRecorded;
	static ULONG ulBandwidth;
	static BOOL  bInit = TRUE;
	
	if ( cci_bInitUpdateMasterBuffer ) 
	{
		//0616 kwon 멤버변수로 수정.
		ulBandwidth = 0;
		tmLastBandwithRecorded = _pTimer->GetHighPrecisionTimer().GetSeconds();
		cci_bInitUpdateMasterBuffer = FALSE;

		//0311 kwon 임시로 넣은 바운드
		cci_bBound = TRUE;
	}
	
	//0105  
	if(m_bTcpIp && _pNetwork->IsTcpServer)
	{
		int sockaddr_size = sizeof( SOCKADDR_IN );		
		SOCKADDR_IN clientsockaddr;	
		SOCKET socket =  accept( cci_hSocket, (LPSOCKADDR)&clientsockaddr, &sockaddr_size);
		if(socket==INVALID_SOCKET)
		{
			//CPrintF(TRANS("Invalid Socket. %s\n"),(const char*)GetSocketError(socket));
		}
		else
		{
			CPrintF(TRANS("Client IP: %s  Connected\n"),inet_ntoa(clientsockaddr.sin_addr));				  
			adrIncomingAddress.adr_ulAddress = ntohl(clientsockaddr.sin_addr.s_addr);
			adrIncomingAddress.adr_uwPort = ntohs(clientsockaddr.sin_port);
			//0131	//0202	  
			
			client_socket = socket;
			client_cnt++;
			cci_bBound = TRUE;//열어보잣~
		}
	}
	//..	
	
	//0131
	if (cci_bBound) 
	{
		// read from the socket while there is incoming data
		do 
		{			
			// initially, nothing is done
			bSomethingDone = FALSE;
			//0105 1line 지우기
			//			slSizeReceived = recvfrom(cci_hSocket,(char*)aub,MAX_PACKET_SIZE,0,(SOCKADDR *)&sa,&size);
			//0105
			if(m_bTcpIp&& _pNetwork->IsTcpServer)
			{//서버
				//0131				
				slSizeReceived = recv(client_socket,(char*)aub,MAX_PACKET_SIZE,0);																
				
			}
			else if( m_bTcpIp )
			{//클라이언트
			//	slSizeReceived = recv(cci_hSocket,(char*)aub,MAX_PACKET_SIZE,0);
				slSizeReceived = recv(cci_hSocket,(char*)m_inbuf.GetPoint(), m_inbuf.GetRemain(),0);
			}
			else
			{			
				slSizeReceived = recvfrom(cci_hSocket,(char*)aub,MAX_PACKET_SIZE,0,(SOCKADDR *)&sa,&size);
			}
			//..
			tvNow = _pTimer->GetHighPrecisionTimer();
			//0105 2line 지우기
			//			adrIncomingAddress.adr_ulAddress = ntohl(sa.sin_addr.s_addr);
			//			adrIncomingAddress.adr_uwPort = ntohs(sa.sin_port);
			//0105
			if(!m_bTcpIp)
			{
				adrIncomingAddress.adr_ulAddress = ntohl(sa.sin_addr.s_addr);
				adrIncomingAddress.adr_uwPort = ntohs(sa.sin_port);
			}
			//..	
			//On error, report it to the console (if error is not a no data to read message)
			if (slSizeReceived == SOCKET_ERROR) 
			{
				int iResult = WSAGetLastError();
				if (iResult!=WSAEWOULDBLOCK) 
				{
					// report it
					if (iResult!=WSAECONNRESET || net_bReportICMPErrors) 
					{
						CPrintF(TRANS("Socket error during UDP receive. %s\n"), 
							(const char*)GetSocketError(iResult));
						cci_bBound = FALSE; //0311 kwon
						//0611 kwon 추가.
						//_pNetwork->ga_sesSessionState.ses_strDisconnected = TRANS("Try to Reconnect...");

						CUIManager* pUIManager = CUIManager::getSingleton();

						if( !_bLoginProcess || STAGEMGR()->GetCurStage() == eSTAGE_SELCHAR )
						{
							ShowDisconnMsg();
						}
						return;
					}
				}				
				// if block received
			} 
			else 
			{
				/* //kwon 0322 삭제
				// if there is not at least one byte more in the packet than the header size
				if (slSizeReceived <= MAX_HEADER_SIZE) {
					// the packet is in error
					extern INDEX net_bReportMiscErrors;          
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("WARNING: Bad UDP packet from '%s'\n"), AddressToString(adrIncomingAddress.adr_ulAddress));
					}
					// there might be more to do
					bSomethingDone = TRUE;
				} else if (net_fDropPackets <= 0  || (FLOAT(rand())/RAND_MAX) > net_fDropPackets) */{
					if ((tvNow.GetSeconds() - tmLastBandwithRecorded) <= 1.0f) {
						ulBandwidth += slSizeReceived;
					} else {
						_pNetwork->AddBandwidthGraphValue(ulBandwidth);
						double ctDelta = (tvNow.GetSeconds() - tmLastBandwithRecorded);
						tmLastBandwithRecorded += ctDelta;
						ulBandwidth = slSizeReceived;
					}

					//0322 kwon
					if (slSizeReceived == 0)
						continue;
					m_inbuf.SetPoint(slSizeReceived);
					/* //0322 kwon 삭제
					// if no packet drop emulation (or the packet is not dropped), form the packet 
					// and add it to the end of the UDP Master's input buffer
					ppaNewPacket = CreatePacket();
					ppaNewPacket->WriteToPacketRaw(aub,slSizeReceived);
					ppaNewPacket->pa_adrAddress.adr_ulAddress = adrIncomingAddress.adr_ulAddress;
					ppaNewPacket->pa_adrAddress.adr_uwPort = adrIncomingAddress.adr_uwPort;						
					*/
					if (net_bReportPackets) {
					//	CPrintF("%lu: Received sequence: %d from ID: %d, reliable flag: 0x%X, size: %d\n",(ULONG) tvNow.GetMilliseconds(),ppaNewPacket->pa_ulSequence,ppaNewPacket->pa_adrAddress.adr_uwID,ppaNewPacket->pa_uwReliable,ppaNewPacket->pa_slSize);
					}
					
					//0322 kwon
					while ((ppaNewPacket = m_inbuf.GetPacket()))
					{						
						if (cci_pbMasterInput.AppendPacket(*ppaNewPacket,FALSE,FALSE) == FALSE){
							ASSERT(FALSE);
						}
						// there might be more to do
						bSomethingDone = TRUE;
					}
					
				}
			}	
			
		} while (bSomethingDone);
	}
	
	// write from the output buffer to the socket
	while (cci_pbMasterOutput.pb_ulNumOfPackets > 0) 
	{
		ppaNewPacket		= cci_pbMasterOutput.PeekFirstPacket();		
		sa.sin_family		= AF_INET;
		sa.sin_addr.s_addr	= htonl(ppaNewPacket->pa_adrAddress.adr_ulAddress);
		sa.sin_port			= htons(ppaNewPacket->pa_adrAddress.adr_uwPort);
		//0105 1line 지우기		
		//    slSizeSent = sendto(cci_hSocket, (char*) ppaNewPacket->pa_pubPacketData, (int) ppaNewPacket->pa_slSize, 0, (SOCKADDR *)&sa, sizeof(sa));
		//0105		
		if( m_bTcpIp && _pNetwork->IsTcpServer )
		{
			//서버
			//0131			
			slSizeSent = send(client_socket, (char*) ppaNewPacket->pa_pubPacketData, (int) ppaNewPacket->pa_slSize, 0);		
			
		}
		else if( m_bTcpIp )
		{
			// [2013/01/09] sykim70
			unsigned int res = htonl(m_crc.CalcCRC32(ppaNewPacket->pa_pubPacketData+MAX_HEADER_SIZE, ppaNewPacket->pa_slSize-MAX_HEADER_SIZE));
			memcpy(ppaNewPacket->pa_pubPacketData+ppaNewPacket->pa_slSize, &res, sizeof(unsigned int));
			//클라이언트		
			slSizeSent = send(cci_hSocket, (char*) ppaNewPacket->pa_pubPacketData, (int) (ppaNewPacket->pa_slSize+sizeof(unsigned int)), 0);
			if( slSizeSent < 0 )
			{
// 				DWORD errcod = WSAGetLastError();
// 				CTString temp;
// 				temp.PrintF( "%d", errcod );
// 				MessageBox( NULL, temp, "", MB_OK );
			}

		}
		else
		{	
			slSizeSent = sendto(cci_hSocket, (char*) ppaNewPacket->pa_pubPacketData, (int) ppaNewPacket->pa_slSize, 0, (SOCKADDR *)&sa, sizeof(sa));
		}
		//..
		cci_bBound	= TRUE;   // UDP socket that did a send is considered bound
		tvNow		= _pTimer->GetHighPrecisionTimer();
		
		// if some error
		if ( slSizeSent == SOCKET_ERROR )
		{
			int iResult = WSAGetLastError();
			// if output UDP buffer full, stop sending
			if ( iResult == WSAEWOULDBLOCK ) 
			{
				return;
				// report it
			} 
			else if ( iResult != WSAECONNRESET || net_bReportICMPErrors ) 
			{
				// FIXME : 중복되는 코드가 많음.
				//extern BOOL _bLoginProcess;

				CUIManager* pUIManager = CUIManager::getSingleton();

				if( !_bLoginProcess || STAGEMGR()->GetCurStage() == eSTAGE_SELCHAR )
				{					
					ShowDisconnMsg();
				}

				cci_bBound = FALSE; //0311 kwon
			}
			else //0824
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				
				if( !_bLoginProcess || STAGEMGR()->GetCurStage() == eSTAGE_SELCHAR )
				{									
					ShowDisconnMsg();
				}
				cci_bBound = FALSE;
			}
			return;    
			// if all sent ok
		} 
		else 
		{
			if ((tvNow.GetSeconds() - tmLastBandwithRecorded) <= 1.0f) 
			{
				ulBandwidth += slSizeSent;
			} 
			else 
			{
				_pNetwork->AddBandwidthGraphValue(ulBandwidth);
				double ctDelta = (tvNow.GetSeconds() - tmLastBandwithRecorded);
				tmLastBandwithRecorded += ctDelta;
				ulBandwidth = slSizeSent;
			}
			if (net_bReportPackets)	
			{
				CPrintF("%lu: Sent sequence: %d to ID: %d, reliable flag: 0x%X,size: %d\n",(ULONG)tvNow.GetMilliseconds(),ppaNewPacket->pa_ulSequence,ppaNewPacket->pa_adrAddress.adr_uwID,ppaNewPacket->pa_uwReliable,ppaNewPacket->pa_slSize);
			}
			
			cci_pbMasterOutput.RemoveFirstPacket(TRUE);
			bSomethingDone=TRUE;
		}		
	}	
};

//------------------------------------------------------------------------------
// CCommunicationInterface::Is2rdLocal
// Explain: 중국 Local 2번째 지역이 선택되었는지 확인 
// Date : 2005-05-02,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CCommunicationInterface::Is2rdLocal()
{
	if ( g_iCountry != CHINA )  // 중국 Local이 아니면 무조건 FALSE ( 현재 중국버전에만 적용됨 )
		return FALSE;

	return m_b2stLocal;
}

int CCommunicationInterface::ReadInfo( LoginServer* stLogin )
{
#ifdef	VER_TEST
	// 개발 버젼용 아이피 txt 가져오기
	{
		std::string	strPath = _fnmApplicationPath.FileDir();
		strPath += "sl.txt";

		FILE* fp = fopen(strPath.c_str(), "rt");
		if (fp != NULL)
		{
			fscanf(fp, "%s %s %s %s %s", 
				stLogin[0].szName, stLogin[0].szAddress, stLogin[0].szPort, stLogin[0].szFullUsers, stLogin[0].szBusyUsers);

			fclose(fp);

			return 1;
		}
	}
#endif	// VER_TEST

	CTFileName fnmServer = CTString("sl.dta");

#ifdef EUROUPEAN_SERVER_LOGIN
	if (g_iConnectEuroupean)
	{
		fnmServer = CTString("sl_Euro.dta");
	}
#endif	
		
	CTFileName fnmServer2 = CTString("sl2.dta");				// 중국쪽 두번째 유저디비군.
	CTFileName fnmSLD = CTString("sl_d.dta");
	//CTFileName fnmServer = CTString("server.txt");//0907
	CTFileStream strmFile;
		
	int ServerNum;
	
	{
		FILE *fp = NULL;
		FILE *fp_sld = NULL;
				
		char buf[256];
				
		// 서버 이름  , 주소
		int i;
		unsigned char key;
		int iKey, iKeyTemp;	
				
		CTString fnTemp;
		CTString fnSLD;		
		CTString strFullPath = _fnmApplicationPath.FileDir();
				
		// 읽어들일 sl?.dta 결정하기.
		fnSLD = strFullPath + fnmSLD;
		if (fp_sld = fopen(fnSLD, "rt")) {				// mdo용 sl_d.dta가 있으면
			int nRegion = -1;
			fscanf(fp_sld, "%d", &nRegion);
					
			if( nRegion == 1 ) // 2번째 지역
			{
				fnTemp = strFullPath + fnmServer2;		// sl2.dta :  region 1
				m_b2stLocal = TRUE;
			}
			else
			{
				fnTemp = strFullPath + fnmServer;		// default : region 0
				m_b2stLocal = FALSE;					
			}
			fclose(fp_sld);					
		} else {				// other country, i.e, nako, insrea ...
			fnTemp = strFullPath + fnmServer;
		}
				
		fp = fopen(fnTemp, "rb");
		if(fp == NULL) return 0;
				
		for (i = 0; i < 6; i++) 
		{
			fread(&key, 1, 1, fp);
		}
		fread(&iKey, sizeof(int), 1, fp);
		fread(&key, 1, 1, fp);
				
		fread(&ServerNum, sizeof(int), 1, fp);
				
		iKeyTemp	= ServerNum;
		ServerNum	-= iKey;
		iKey		= iKeyTemp;				
				
		for (i = 0; i < ServerNum; i++) 
		{
			// 첫번째 읽은줄을 buf에 넣는다.
			int count;
			unsigned char data;
			fread(&count, sizeof(int), 1, fp);
					
			iKeyTemp	= count;
			count		-= iKey;
			iKey		= iKeyTemp;
					
			for (int idx = 0; idx < count; idx++) 
			{
				fread(&data, 1, 1, fp);
				buf[idx] = data - key;
				key = data;
			}
			buf[count] = '\0';
					
			sscanf(buf, "%s %s %s %s %s", 
				stLogin[i].szName, stLogin[i].szAddress, stLogin[i].szPort, stLogin[i].szFullUsers, stLogin[i].szBusyUsers);
		}
				
		fclose(fp);
	}

	return ServerNum;
}

void CCommunicationInterface::ShowDisconnMsg()
{
	if ( _pGameState->IsRestartGame() == TRUE || _pGameState->GetGameMode() == CGameState::GM_RESTART )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();
	// FIXME : 중복되는 코드가 많음.
	pUIManager->CloseMessageBox(MSGCMD_DISCONNECT);
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK, UI_NONE, MSGCMD_DISCONNECT );
	MsgBoxInfo.AddString( _S( 300, "서버와의 연결이 끊어졌습니다." ) );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	cci_bInitUpdateMasterBuffer = FALSE;
	_pNetwork->m_bSendMessage = FALSE;
	_cmiComm.Disconnect();
}

void CCommunicationInterface::attachCRC( CPacket* packet )
{
	unsigned int res = htonl(m_crc.CalcCRC32(packet->pa_pubPacketData + MAX_HEADER_SIZE, packet->pa_slSize - MAX_HEADER_SIZE));
	memcpy(packet->pa_pubPacketData + packet->pa_slSize, &res, sizeof(unsigned int));
}

bool CCommunicationInterface::FindHosNameIP( const char* szHostName, sIPFilter &ip, INDEX iCountry )
{
	sockaddr_in sai;
	memset(&sai, 0, sizeof(sockaddr_in));
	sai.sin_addr.s_addr = inet_addr(szHostName);
	if(INADDR_NONE == sai.sin_addr.s_addr)
	{
		hostent *Hostent = gethostbyname(szHostName);

		if (Hostent == NULL)
			return false;

		sai.sin_addr.s_addr = ((struct in_addr FAR *)(Hostent->h_addr))->s_addr;
		ip.SetIP(sai.sin_addr.S_un.S_un_b.s_b1, sai.sin_addr.S_un.S_un_b.s_b2, sai.sin_addr.S_un.S_un_b.s_b3, sai.sin_addr.S_un.S_un_b.s_b4, iCountry);
	}

	return true;
}
