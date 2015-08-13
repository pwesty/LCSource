#include "stdh.h"

#include <Engine/Base/Stream.h>
#include <Engine/Base/Console.h>
#include <Engine/Network/MessageDispatcher.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/NetworkProfile.h>
#include <Engine/Network/NetworkMessage.h>
#include <Engine/Network/CommunicationInterface.h>
#include <Engine/Base/ErrorTable.h>

#include <Engine/Base/ListIterator.inl>
#include <Engine/Network/MessageDefine.h>

#include <Engine/SEED_256_KISA/SEED_256_KISA.h>
#ifdef	VER_TEST
#include <Engine/Network/SessionState.h>
#endif	// VER_TEST

//////////////////////////////////////////////////////////////////////////
static DWORD pdwRoundKey[48];
static BYTE pbUserKey[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

void CMessageDispatcher::InitSEEDEncrypt()
{
	SeedRoundKey(pdwRoundKey, pbUserKey);
}
//////////////////////////////////////////////////////////////////////////

// define this to randomly drop messages (for debugging of packet-loss recovery)
//#define LOSEPACKETS_THRESHOLD (RAND_MAX/10)


extern INDEX net_bReportTraffic;
extern BOOL _bTcp4ClientInterface;				// by seo-40227 declared in ClientInterface.cpp

/////////////////////////////////////////////////////////////////////
// CMessageBuffer helper class+implementation

// class for holding received messages (thread locally)
class CMessageBuffer {
public:
  // size of message buffer for one message
  ULONG mb_ulMessageBufferSize;
  // pointer to message buffer for one message
  void *mb_pvMessageBuffer;

  void Allocate(void);
  void Free(void);
};

// the thread's local buffer
//static _declspec(thread) CMessageBuffer mbReceivedMessage = { 0,0 };
//static TLVar<CMessageBuffer> mbReceivedMessage = { 0,0 };

void CMessageBuffer::Allocate(void)
{
  if (mb_ulMessageBufferSize==0) {
    ASSERT(mb_pvMessageBuffer == NULL);
    // allocate message buffer
    mb_ulMessageBufferSize = 16000;
    mb_pvMessageBuffer = AllocMemory(mb_ulMessageBufferSize);
  }
}

void CMessageBuffer::Free(void)
{
  // if message buffer is allocated
  if (mb_ulMessageBufferSize != 0) {
    ASSERT(mb_pvMessageBuffer != NULL);
    // free it
    FreeMemory(mb_pvMessageBuffer);
    mb_ulMessageBufferSize = 0;
    mb_pvMessageBuffer = NULL;
  }
}

/////////////////////////////////////////////////////////////////////
// CNetworkProvider implementation

/*
 * Default constructor.
 */
CNetworkProvider::CNetworkProvider(void)
{
}

/*
 * Create a human description of driver.
 */
const CTString &CNetworkProvider::GetDescription(void) const
{
  return np_Description;
}


/////////////////////////////////////////////////////////////////////
// CNetworkSession implementation

/*
 * Default constructor.
 */
CNetworkSession::CNetworkSession(void)
{
}
/* Construct a session for connecting to certain server. */
CNetworkSession::CNetworkSession(const CTString &strAddress)
{
  ns_strAddress = strAddress;
}
void CNetworkSession::Copy(const CNetworkSession &nsOriginal)
{
  ns_strAddress     = nsOriginal.ns_strAddress     ;
  ns_strSession     = nsOriginal.ns_strSession     ;
  ns_strWorld       = nsOriginal.ns_strWorld       ;
  ns_tmPing         = nsOriginal.ns_tmPing         ;
  ns_ctPlayers      = nsOriginal.ns_ctPlayers      ;
  ns_ctMaxPlayers   = nsOriginal.ns_ctMaxPlayers   ;
  ns_strGameType    = nsOriginal.ns_strGameType    ;
  ns_strMod         = nsOriginal.ns_strMod         ;
  ns_strVer         = nsOriginal.ns_strVer         ;

  ns_ulAddress      = nsOriginal.ns_ulAddress      ;
  ns_uwPort         = nsOriginal.ns_uwPort         ;
  ns_tmLastSeen     = nsOriginal.ns_tmLastSeen     ;

}

/////////////////////////////////////////////////////////////////////
// CMessageDispatcher -- construction/destruction

/*
 * Default constructor.
 */
CMessageDispatcher::CMessageDispatcher(void) {
   _cmiComm.Init();

  // enumerate network providers
  EnumNetworkProviders_startup(md_lhProviders);
  IsTcpServer = FALSE;				// by seo-40225
  _bDelivery = true;
}

/*
 * Destructor.
 */
CMessageDispatcher::~CMessageDispatcher(void)
{
  _cmiComm.Close();

  // destroy the list of network providers
  FORDELETELIST(CNetworkProvider, np_Node, md_lhProviders, litProviders) {
    delete &*litProviders;
  }
}

/*
 * Initialize for a given game.
 */
void CMessageDispatcher::Init(const CTString &strGameID)
{
  md_strGameID = strGameID;
}

/////////////////////////////////////////////////////////////////////
// CMessageDispatcher -- network provider management

/*
 * Enumerate all providers at startup (later enumeration just copies this list).
 */
void CMessageDispatcher::EnumNetworkProviders_startup(CListHead &lh)
{
  // create local connection provider
  CNetworkProvider *pnpLocal = new CNetworkProvider;
  pnpLocal->np_Description = "Local";
  lh.AddTail(pnpLocal->np_Node);
  // create TCP/IP connection provider
  CNetworkProvider *pnpTCP = new CNetworkProvider;
  pnpTCP->np_Description = "TCP/IP Server";
  lh.AddTail(pnpTCP->np_Node);

  CNetworkProvider *pnpTCPCl = new CNetworkProvider;
  pnpTCPCl->np_Description = "TCP/IP Client";
  lh.AddTail(pnpTCPCl->np_Node);

  CNetworkProvider *pnpTCPMmo = new CNetworkProvider;	// by seo 40225
  pnpTCPMmo->np_Description = "TCP/IP Mmo";
  lh.AddTail(pnpTCPMmo->np_Node);

  CNetworkProvider *pnpTCPNEW = new CNetworkProvider; //0311 kwon
  pnpTCPNEW->np_Description = "TCP/IP NEW Client";
  lh.AddTail(pnpTCPNEW->np_Node);
}

/*
 * Enumerate all providers.
 */
void CMessageDispatcher::EnumNetworkProviders(CListHead &lh)
{
  // for each provider enumerated at startup
  FOREACHINLIST(CNetworkProvider, np_Node, md_lhProviders, litProvider) {
    // create a copy
    CNetworkProvider *pnpNew = new CNetworkProvider(*litProvider);
    // add the copy to the list
    lh.AddTail(pnpNew->np_Node);
  }
}

/*
 * Start using a service provider.
 */
void CMessageDispatcher::StartProvider_t(const CNetworkProvider &npProvider)
{
  extern INDEX net_bNetworkGame;
  extern INDEX net_bMmoGame;
  if (npProvider.np_Description=="Local") {
//강동민 수정 시작 클로즈 준비 작업	08.10
		//_cmiComm.PrepareForUse(FALSE, FALSE);
		//net_bNetworkGame = FALSE;
		_cmiComm.PrepareForUse(TRUE, FALSE);
		net_bNetworkGame = TRUE;
	//_bTcp4ClientInterface = FALSE;
//강동민 수정 끝 클로즈 준비 작업		08.10
  } else if (npProvider.np_Description=="TCP/IP Server") {
	 if (net_bMmoGame) {
		_cmiComm.PrepareForUse(TRUE, FALSE, TRUE);
		_bTcp4ClientInterface = TRUE;
	 } else {
	   _cmiComm.PrepareForUse(TRUE, FALSE, IsTcpServer);	// dedicated server를 띄울때 파라미터가 넘어가는 방법에 따라 bTcp가 이미 설정되어 있다.
	   _bTcp4ClientInterface = IsTcpServer;
	 }
    net_bNetworkGame = TRUE;	
  } else if (npProvider.np_Description=="TCP/IP Mmo") {			// by seo 40225
	_bTcp4ClientInterface = TRUE;
	net_bNetworkGame = TRUE;
	net_bMmoGame = TRUE;
	_cmiComm.PrepareForUse(/*bNetwork*/TRUE, /*bClient*/TRUE, /*bTcp*/TRUE);	
  } 
  else if (npProvider.np_Description=="TCP/IP New Client") { //0311 kwon
    _cmiComm.PrepareForUse(TRUE, FALSE);
    net_bNetworkGame = TRUE;
  }
  else {
	net_bNetworkGame = TRUE;
	_bTcp4ClientInterface = FALSE;
    _cmiComm.PrepareForUse(TRUE, TRUE);    
  }
}

/*
 * Stop using current service provider.
 */
void CMessageDispatcher::StopProvider(void)
{
  _cmiComm.Unprepare();
}

/////////////////////////////////////////////////////////////////////
// CMessageDispatcher -- network message management
static void UpdateSentMessageStats(const CNetworkMessage &nmMessage)
{
  // increment profile counters
  _pfNetworkProfile.IncrementCounter(CNetworkProfile::PCI_MESSAGESSENT);
  _pfNetworkProfile.IncrementCounter(CNetworkProfile::PCI_BYTESSENT, nmMessage.nm_slSize);
  switch (nmMessage.GetType()) {
  case MSG_GAMESTREAMBLOCKS:
    _pfNetworkProfile.IncrementCounter(CNetworkProfile::PCI_GAMESTREAM_BYTES_SENT, nmMessage.nm_slSize);
    break;
  case MSG_ACTIONS:
    _pfNetworkProfile.IncrementCounter(CNetworkProfile::PCI_ACTION_BYTES_SENT, nmMessage.nm_slSize);
    break;
  }
  if (net_bReportTraffic) {
    CPrintF("Sent: %d\n", nmMessage.nm_slSize);
  }
}
static void UpdateSentStreamStats(SLONG slSize)
{
  if (net_bReportTraffic) {
    CPrintF("STREAM Sent: %d\n", slSize);
  }
}
static void UpdateReceivedMessageStats(const CNetworkMessage &nmMessage)
{
  // increment profile counters
  _pfNetworkProfile.IncrementCounter(CNetworkProfile::PCI_MESSAGESRECEIVED);
  _pfNetworkProfile.IncrementCounter(CNetworkProfile::PCI_BYTESRECEIVED, nmMessage.nm_slSize);
  switch (nmMessage.GetType()) {
  case MSG_GAMESTREAMBLOCKS:
    _pfNetworkProfile.IncrementCounter(CNetworkProfile::PCI_GAMESTREAM_BYTES_RECEIVED, nmMessage.nm_slSize);
    break;
  case MSG_ACTIONS:
    _pfNetworkProfile.IncrementCounter(CNetworkProfile::PCI_ACTION_BYTES_RECEIVED, nmMessage.nm_slSize);
    break;
  }
  if (net_bReportTraffic) {
    CPrintF("Rcvd: %d\n", nmMessage.nm_slSize);
  }
}
static void UpdateReceivedStreamStats(SLONG slSize)
{
  if (net_bReportTraffic) {
    CPrintF("STREAM Rcvd: %d\n", slSize);
  }
}

/* Send a message from server to client. */
void CMessageDispatcher::SendToClient(INDEX iClient, const CNetworkMessage &nmMessage)
{
// if testing for packet-loss recovery
#ifdef LOSEPACKETS_THRESHOLD
  // every once a while
  if (rand()<LOSEPACKETS_THRESHOLD) {
    // don't send it
    return;
  }
#endif

  _pfNetworkProfile.StartTimer(CNetworkProfile::PTI_SENDMESSAGE);
  // send the message
  _cmiComm.Server_Send_Unreliable(iClient, (void*)nmMessage.nm_pubMessage, nmMessage.nm_slSize);
	
  UpdateSentMessageStats(nmMessage);
  _pfNetworkProfile.StopTimer(CNetworkProfile::PTI_SENDMESSAGE);
}
void CMessageDispatcher::SendToClientReliable(INDEX iClient, const CNetworkMessage &nmMessage)
{
  _pfNetworkProfile.StartTimer(CNetworkProfile::PTI_SENDMESSAGE);
	
  // send the message
  _cmiComm.Server_Send_Reliable(iClient, (void*)nmMessage.nm_pubMessage, nmMessage.nm_slSize);
  UpdateSentMessageStats(nmMessage);
  _pfNetworkProfile.StopTimer(CNetworkProfile::PTI_SENDMESSAGE);
}
void CMessageDispatcher::SendToClientReliable(INDEX iClient, CTMemoryStream &strmMessage)
{
  _pfNetworkProfile.StartTimer(CNetworkProfile::PTI_SENDMESSAGE);

  // get size and buffer from the stream
  void *pvBuffer;
  SLONG slSize;
  strmMessage.LockBuffer(&pvBuffer, &slSize);
  // send the message
  _cmiComm.Server_Send_Reliable(iClient, pvBuffer, slSize);
  strmMessage.UnlockBuffer();
  UpdateSentStreamStats(slSize);
  _pfNetworkProfile.StopTimer(CNetworkProfile::PTI_SENDMESSAGE);
}

/* Send a message from client to server. */
void CMessageDispatcher::SendToServer(const CNetworkMessage &nmMessage)
{
  _pfNetworkProfile.StartTimer(CNetworkProfile::PTI_SENDMESSAGE);
  // send the message
  _cmiComm.Client_Send_Unreliable((void*)nmMessage.nm_pubMessage, nmMessage.nm_slSize);
	UpdateSentMessageStats(nmMessage);
  _pfNetworkProfile.StopTimer(CNetworkProfile::PTI_SENDMESSAGE);
}

void CMessageDispatcher::SendToServerReliable(const CNetworkMessage &nmMessage)
{
  _pfNetworkProfile.StartTimer(CNetworkProfile::PTI_SENDMESSAGE);
  // send the message
  _cmiComm.Client_Send_Reliable((void*)nmMessage.nm_pubMessage, nmMessage.nm_slSize);
  UpdateSentMessageStats(nmMessage);
  _pfNetworkProfile.StopTimer(CNetworkProfile::PTI_SENDMESSAGE);
}

/* Receive next message from server to client. */
BOOL CMessageDispatcher::ReceiveFromServer(CNetworkMessage &nmMessage)
{
  _pfNetworkProfile.StartTimer(CNetworkProfile::PTI_RECEIVEMESSAGE);
  // receive message in static buffer
  nmMessage.nm_slSize = nmMessage.nm_slMaxSize;
  BOOL bReceived = _cmiComm.Client_Receive_Unreliable(
    (void*)nmMessage.nm_pubMessage, nmMessage.nm_slSize);

  // if there is message
  if (bReceived) {
    // init the message structure
    nmMessage.nm_pubPointer = nmMessage.nm_pubMessage;
    nmMessage.nm_iBit = 0;
    UBYTE ubType;
    nmMessage.Read(&ubType, sizeof(ubType));
    nmMessage.nm_mtType = (MESSAGETYPE)ubType;

    UpdateReceivedMessageStats(nmMessage);
  }
  _pfNetworkProfile.StopTimer(CNetworkProfile::PTI_RECEIVEMESSAGE);
  return bReceived;
}

//0311 kwon 함수추가. master input버퍼로부터 서버로부터온 메시지를 가져온다.
/* Receive next message from server to client. */
BOOL CMessageDispatcher::ReceiveFromServerNew(CNetworkMessage &nmMessage)
{		
  _pfNetworkProfile.StartTimer(CNetworkProfile::PTI_RECEIVEMESSAGE);
  // receive message in static buffer
  nmMessage.nm_slSize = nmMessage.nm_slMaxSize;
  BOOL bReceived = _cmiComm.Client_Receive_Unreliable_New(
    (void*)nmMessage.nm_pubMessage, nmMessage.nm_slSize, nmMessage.nm_iIndex);

  // if there is message
  if (bReceived) {
		// EDIT : BS : 패킷 암호화 : 복호화

	extern UINT g_uiEngineVersion;
//	if( g_uiEngineVersion >= 10000 )  // 060303 wooss : 본섭 암호화 적용
	{
#ifndef SEED_ENCIPHERMENT

#	ifdef CRYPT_NET_MSG
		// EDIT : BS : 070413 : 신규 패킷 암호화
		UBYTE pDecryptBuf[CNM_TEMP_BUFFER_LENGTH];
		UBYTE pDecryptBufTmp[CNM_TEMP_BUFFER_LENGTH];

#		ifdef	CRYPT_NET_MSG_LEVEL2
			extern unsigned char g_ubKey[];
			DecryptMem(nmMessage.nm_pubMessage, nmMessage.nm_slSize, g_ubKey, 14);		
#		endif //CRYPT_NET_MSG_LEVEL2

#		ifdef CRYPT_NET_MSG_MANUAL
			int nDecryptBuf = CNM_Decrypt(nmMessage.nm_pubMessage, nmMessage.nm_slSize, &_pNetwork->cnmKey, pDecryptBuf, pDecryptBufTmp);
#		else // CRYPT_NET_MSG_MANUAL
			int nDecryptBuf = CNM_Decrypt(&_pNetwork->cnmKeyServer, nmMessage.nm_slSize, nmMessage.nm_pubMessage, pDecryptBuf, pDecryptBufTmp);
#		endif // CRYPT_NET_MSG_MANUAL
#	else 
		UBYTE* pDecryptBuf = NULL;
		int nDecryptBuf = CNM_Decrypt(nmMessage.nm_pubMessage, nmMessage.nm_slSize, _pNetwork->cnmKey, &pDecryptBuf);
#	endif // CRYPT_NET_MSG

		if (nDecryptBuf < 0)
			bReceived = FALSE;
		else
		{
			memcpy(nmMessage.nm_pubMessage, pDecryptBuf, nDecryptBuf);
			nmMessage.nm_slSize = nDecryptBuf;
		}

#	ifndef CRYPT_NET_MSG
		if (pDecryptBuf)
			delete [] pDecryptBuf;
#	endif //CRYPT_NET_MSG

#endif // ifndef SEED_ENCIPHERMENT
	}
	// EDIT : BS : 070413 : 신규 패킷 암호화
#ifdef CRYPT_NET_MSG
#	ifndef CRYPT_NET_MSG_MANUAL
		CNM_NextKey(&_pNetwork->cnmKeyServer);
#	endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG
	
#ifdef SEED_ENCIPHERMENT
	int loopcount = nmMessage.nm_slSize / 16;
	for (int i = 0; i < loopcount; ++i)
	{
		SeedDecrypt(nmMessage.nm_pubMessage + (i * 16), pdwRoundKey);
	}
#endif //SEED_ENCIPHERMENT
    // init the message structure
    nmMessage.nm_pubPointer = nmMessage.nm_pubMessage;
    nmMessage.nm_iBit = 0;
    UBYTE ubType;
    nmMessage.Read(&ubType, sizeof(ubType));
    nmMessage.nm_mtType = (MESSAGETYPE)ubType;

	
    UpdateReceivedMessageStats(nmMessage);
  }
  _pfNetworkProfile.StopTimer(CNetworkProfile::PTI_RECEIVEMESSAGE);

  // 맵 이동 중 이라면 도착한 메세지를 버린다.
  if (_bDelivery == false)
	  return FALSE;

  return bReceived;
}

//0311 kwon 함수추가. 서버로 보낼 메시지를 master out buffer로 보낸다.
void CMessageDispatcher::SendToServerNew(const CNetworkMessage &nmMessage, BOOL bLogin)
{	
#ifdef	VER_TEST
	if (nmMessage.nm_pubMessage[0] != MSG_MOVE)
		LOG_DEBUG( "SendToServerNew : %s[%d] : %d", strMSG_MAIN[nmMessage.nm_pubMessage[0]], nmMessage.nm_pubMessage[0], nmMessage.nm_pubMessage[1]);
#endif	// VER_TEST


	if (_bDelivery == false)
	{
#ifdef	VER_TEST
		if (nmMessage.nm_pubMessage[0] != MSG_MOVE)
			LOG_DEBUG( "Ignore Send : %s[%d] : %d", strMSG_MAIN[nmMessage.nm_pubMessage[0]], nmMessage.nm_pubMessage[0], nmMessage.nm_pubMessage[1]);
#endif	// VER_TEST
		return;
	}

	//0707
	CNetworkMessage tmpMessage;
	tmpMessage = nmMessage;


	// EDIT : BS : 패킷 암호화 : 암호화 하기
	extern UINT g_uiEngineVersion;
//	if( g_uiEngineVersion >= 10000 ) // 060303 wooss : 본섭 암호화 적용	
	{
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SEED_ENCIPHERMENT

#	ifdef CRYPT_NET_MSG
		// EDIT : BS : 070413 : 신규 패킷 암호화
		UBYTE pCryptBuf[CNM_TEMP_BUFFER_LENGTH];
		UBYTE pCryptTmpBuf[CNM_TEMP_BUFFER_LENGTH];
		int nCryptBuf = CNM_Crypt(&_pNetwork->cnmKey, tmpMessage.nm_slSize, tmpMessage.nm_pubMessage, pCryptBuf, pCryptTmpBuf);
#	else
		UBYTE* pCryptBuf = NULL;
		int nCryptBuf = CNM_Crypt(tmpMessage.nm_pubMessage, tmpMessage.nm_slSize, _pNetwork->cnmKey, &pCryptBuf);
#	endif // CRYPT_NET_MSG

#	ifdef	CRYPT_NET_MSG_LEVEL2
		extern unsigned char g_ubKey[];
		CryptMem(pCryptBuf, nCryptBuf, g_ubKey, 14);
#	endif // CRYPT_NET_MSG_LEVEL2

		if (nCryptBuf < 0)
			return ;
		// EDIT : BS : 만약 메시지 타입이 UBYTE에서 다른 것으로 변경되면 여기도 변경해야함
		CNetworkMessage cryptMessage(pCryptBuf[0]);
		cryptMessage.Write(pCryptBuf + sizeof(UBYTE), nCryptBuf - sizeof(UBYTE));
		tmpMessage = cryptMessage;

#	ifndef CRYPT_NET_MSG
		if (pCryptBuf)
			delete [] pCryptBuf;
#	endif // #ifndef CRYPT_NET_MSG

#endif // SEED_ENCIPHERMENT
////////////////////////////////////////////////////////////////////////////////////////////////////
	}
#ifndef SEED_ENCIPHERMENT
	// EDIT : BS : 070413 : 신규 패킷 암호화
#	ifdef CRYPT_NET_MSG
#		ifndef CRYPT_NET_MSG_MANUAL
		CNM_NextKey(&_pNetwork->cnmKey);
#		endif // #ifndef CRYPT_NET_MSG_MANUAL
#	endif // #ifdef CRYPT_NET_MSG

#	ifdef COMPRESSION_MESSAGE
	
	CNetworkMessage nmPackMessage(tmpMessage.GetType());
	tmpMessage.PackDefault(nmPackMessage);
#	endif // COMPRESSION_MESSAGE

#endif // SEED_ENCIPHERMENT

	//	if(_cmiComm.cci_bBound){
	_pfNetworkProfile.StartTimer(CNetworkProfile::PTI_SENDMESSAGE);
	// send the message
#ifdef SEED_ENCIPHERMENT
	int loopcount = nmMessage.nm_slSize / 16;
	for (int i = 0; i < loopcount; ++i)
	{
		SeedEncrypt(nmMessage.nm_pubMessage + (i * 16), pdwRoundKey);
	}
	_cmiComm.Client_Send_Unreliable_New((void*)nmMessage.nm_pubMessage, nmMessage.nm_slSize, bLogin);
	UpdateSentMessageStats(nmMessage);	
#else
#	ifdef COMPRESSION_MESSAGE
	_cmiComm.Client_Send_Unreliable_New((void*)nmPackMessage.nm_pubMessage, nmPackMessage.nm_slSize, bLogin);
	UpdateSentMessageStats(nmPackMessage);
#	else

	Xor_enc(nmMessage.nm_pubMessage, nmMessage.nm_slSize);

	_cmiComm.Client_Send_Unreliable_New((void*)nmMessage.nm_pubMessage, nmMessage.nm_slSize, bLogin);
	UpdateSentMessageStats(nmMessage);
#	endif // COMPRESSION_MESSAGE
#endif // SEED_ENCIPHERMENT
	
	_pfNetworkProfile.StopTimer(CNetworkProfile::PTI_SENDMESSAGE);
}

//! 서버에서 클라이언트로 다음 메시지를 받는다.
BOOL CMessageDispatcher::ReceiveFromServerReliable(CNetworkMessage &nmMessage)
{
  _pfNetworkProfile.StartTimer(CNetworkProfile::PTI_RECEIVEMESSAGE);
  // receive message in static buffer
  nmMessage.nm_slSize = nmMessage.nm_slMaxSize;
  BOOL bReceived = _cmiComm.Client_Receive_Reliable(
    (void*)nmMessage.nm_pubMessage, nmMessage.nm_slSize);

  // if there is message
  if (bReceived) {
    // init the message structure
    nmMessage.nm_pubPointer = nmMessage.nm_pubMessage;
    nmMessage.nm_iBit = 0;
    UBYTE ubType;
    nmMessage.Read(&ubType, sizeof(ubType));
    nmMessage.nm_mtType = (MESSAGETYPE)ubType;
	UpdateReceivedMessageStats(nmMessage);

  }
  _pfNetworkProfile.StopTimer(CNetworkProfile::PTI_RECEIVEMESSAGE);
  return bReceived;
}

BOOL CMessageDispatcher::ReceiveFromServerReliable(CTMemoryStream &strmMessage)
{
  _pfNetworkProfile.StartTimer(CNetworkProfile::PTI_RECEIVEMESSAGE);
  // receive message in stream
  BOOL bReceived = _cmiComm.Client_Receive_Reliable(strmMessage);
	
  // if there is message
  if (bReceived) {
    try {
      UpdateReceivedStreamStats(strmMessage.GetPos_t());
    } catch (char *) {
    }
  }
  _pfNetworkProfile.StopTimer(CNetworkProfile::PTI_RECEIVEMESSAGE);
  return bReceived;
}

/* Receive next message from client to server. */
BOOL CMessageDispatcher::ReceiveFromClient(INDEX iClient, CNetworkMessage &nmMessage)
{
  _pfNetworkProfile.StartTimer(CNetworkProfile::PTI_RECEIVEMESSAGE);
  // receive message in static buffer
  nmMessage.nm_slSize = nmMessage.nm_slMaxSize;
  BOOL bReceived = _cmiComm.Server_Receive_Unreliable(iClient,
    (void*)nmMessage.nm_pubMessage, nmMessage.nm_slSize);

  // if there is message
  if (bReceived) {
    // init the message structure
    nmMessage.nm_pubPointer = nmMessage.nm_pubMessage;
    nmMessage.nm_iBit = 0;
    UBYTE ubType;
    nmMessage.Read(&ubType, sizeof(ubType));
    nmMessage.nm_mtType = (MESSAGETYPE)ubType;

    UpdateReceivedMessageStats(nmMessage);
  }
  _pfNetworkProfile.StopTimer(CNetworkProfile::PTI_RECEIVEMESSAGE);
  return bReceived;
}

BOOL CMessageDispatcher::ReceiveFromClientReliable(INDEX iClient, CNetworkMessage &nmMessage)
{
//  _pfNetworkProfile.StartTimer(CNetworkProfile::PTI_RECEIVEMESSAGE);  // profile this!!!!
  // receive message in static buffer
  nmMessage.nm_slSize = nmMessage.nm_slMaxSize;
  BOOL bReceived = _cmiComm.Server_Receive_Reliable(iClient,
    (void*)nmMessage.nm_pubMessage, nmMessage.nm_slSize);

  // if there is a message
  if (bReceived) {
    // init the message structure
    nmMessage.nm_pubPointer = nmMessage.nm_pubMessage;
    nmMessage.nm_iBit = 0;
    UBYTE ubType;
    nmMessage.Read(&ubType, sizeof(ubType));
    nmMessage.nm_mtType = (MESSAGETYPE)ubType;

    UpdateReceivedMessageStats(nmMessage);
  }
//  _pfNetworkProfile.StopTimer(CNetworkProfile::PTI_RECEIVEMESSAGE);
  return bReceived;
}

/* Send/receive broadcast messages. */
void CMessageDispatcher::SendBroadcast(const CNetworkMessage &nmMessage, ULONG ulAddr, UWORD uwPort)
{
  CAddress adrDestination;
  adrDestination.adr_ulAddress = ulAddr;
  adrDestination.adr_uwPort = uwPort;
  adrDestination.adr_uwID = '//';
  // send the message
  _cmiComm.Broadcast_Send((void*)nmMessage.nm_pubMessage, nmMessage.nm_slSize,adrDestination);

  UpdateSentMessageStats(nmMessage);
}

BOOL CMessageDispatcher::ReceiveBroadcast(CNetworkMessage &nmMessage, ULONG &ulAddr, UWORD &uwPort)
{
  CAddress adrSource = {0,0,0};
  // receive message in static buffer
  nmMessage.nm_slSize = nmMessage.nm_slMaxSize;
  BOOL bReceived = _cmiComm.Broadcast_Receive(
    (void*)nmMessage.nm_pubMessage, nmMessage.nm_slSize,adrSource);
//! 서버는 이밑으로 안들어가~
  // if there is message
  if (bReceived) {
    // init the message structure
    nmMessage.nm_pubPointer = nmMessage.nm_pubMessage;
    nmMessage.nm_iBit = 0;
    UBYTE ubType;
    nmMessage.Read(&ubType, sizeof(ubType));
    nmMessage.nm_mtType = (MESSAGETYPE)ubType;
    // read address
    ulAddr = adrSource.adr_ulAddress;
    uwPort = adrSource.adr_uwPort;
  }
//  _pfNetworkProfile.StopTimer(CNetworkProfile::PTI_RECEIVEMESSAGE);
  return bReceived;
}

void CMessageDispatcher::Encrypt( CNetworkMessage& nmMessage )
{
	int loopcount = nmMessage.nm_slSize / 16;
	for (int i = 0; i < loopcount; ++i)
	{
		SeedEncrypt(nmMessage.nm_pubMessage + (i * 16), pdwRoundKey);
	}
}
