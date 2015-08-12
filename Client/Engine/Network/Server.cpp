#include "stdh.h"

#include <Engine/Build.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Network/PlayerSource.h>
#include <Engine/Network/PlayerTarget.h>
#include <Engine/Network/NetworkProfile.h>
#include <Engine/Network/ClientInterface.h>
#include <Engine/Network/CommunicationInterface.h>
#include <Engine/Network/Compression.h>
#include <Engine/Network/Diff.h>
//#include <Engine/Network/GameSpy.h>
#include <Engine/Network/CPacket.h>
#include <Engine/Network/EntityHashing.h>
#include <Engine/World/WorldSettings.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/Shell.h>
#include <Engine/Math/Functions.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Base/CRC.h>
#include <Engine/Base/ErrorTable.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/Timer.h>
#include <Engine/Network/MessageDefine.h>
                           
#include <Engine/Templates/StaticArray.cpp>


extern FLOAT ser_fAckDropRate;
extern FLOAT cli_fTickDropRate;
extern FLOAT net_tmDisconnectTimeout;
extern BOOL con_bCapture;
extern CTString con_strCapture;
extern CTString ser_strIPMask;
extern CTString ser_strNameMask;
extern INDEX ser_bInverseBanning;
extern BOOL MatchesBanMask(const CTString &strString, const CTString &strMask);
extern CClientInterface cm_aciClients[SERVER_CLIENTS];
extern UBYTE _aubCompData[SERVER_MAX_MESSAGE_SIZE] = {0};

CSessionSocket::CSessionSocket(void)
{
  sso_bActive = FALSE;
  sso_bVIP = FALSE;
  sso_bSendStream = FALSE;
  sso_bTrackData = FALSE;
  sso_iDisconnectedState = 0;
  sso_iLastSentSequence  = -1;
  sso_tvLastMessageSent.Clear();
  sso_tvLastPingSent.Clear();

  sso_tmLastTickAcknowledge = -1.0f;
  sso_tmNextAvailableTimeSlot = 0.0f;
}
CSessionSocket::~CSessionSocket(void)
{
  sso_bActive = FALSE;
  sso_bVIP = FALSE;
  sso_bSendStream = FALSE;
  sso_bTrackData = FALSE;
  sso_iLastSentSequence  = -1;
  sso_tvLastMessageSent.Clear();
  sso_tvLastPingSent.Clear();

  sso_tmLastTickAcknowledge = -1.0f;
  sso_tmNextAvailableTimeSlot = 0.0f;
}
void CSessionSocket::Clear(void)
{
  sso_bActive = FALSE;
  sso_bVIP = FALSE;
  sso_bSendStream = FALSE;
  sso_bTrackData = FALSE;
  sso_tvMessageReceived.Clear();
  sso_iLastSentSequence  = -1;
  sso_tvLastMessageSent.Clear();
  sso_tvLastPingSent.Clear();
  sso_nsBuffer.Clear();
  sso_iDisconnectedState = 0;
  sso_sspParams.Clear();

  sso_tmLastTickAcknowledge = -1.0f;
  sso_tmNextAvailableTimeSlot = 0.0f;
}

void CSessionSocket::Activate(void)
{

  ASSERT(!sso_bActive);
  sso_bActive = TRUE;
  sso_bVIP = FALSE;
  sso_bSendStream = FALSE;
  sso_tvMessageReceived.Clear();
  sso_iLastSentSequence  = -1;
  sso_tvLastMessageSent.Clear();
  sso_tvLastPingSent.Clear();
  sso_iDisconnectedState = 0;
  sso_sspParams.Clear();
//  sso_nsBuffer.Clear();

  sso_tmLastTickAcknowledge = -1.0f;
  sso_tmNextAvailableTimeSlot = 0.0f;
}

void CSessionSocket::Deactivate(void)
{
  sso_iDisconnectedState = 0;
  sso_iLastSentSequence  = -1;
  sso_tvLastMessageSent.Clear();
  sso_tvLastPingSent.Clear();
  sso_bActive = FALSE;
  sso_bSendStream = FALSE;
  sso_bTrackData = FALSE;
  sso_nsBuffer.Clear();
  sso_sspParams.Clear();

  sso_tmLastTickAcknowledge = -1.0f;
  sso_tmNextAvailableTimeSlot = 0.0f;
}
BOOL CSessionSocket::IsActive(void)
{
  return sso_bActive;
}

extern INDEX cli_iBufferActions;
extern INDEX cli_iMaxBPS;
extern INDEX cli_iMinBPS;

CSessionSocketParams::CSessionSocketParams(void)
{
  Clear();
}

void CSessionSocketParams::Clear(void)
{
  ssp_iBufferActions = 2;
  ssp_iMaxBPS = 4000;
  ssp_iMinBPS = 1000;
}

static void ClampParams(void)
{
  cli_iBufferActions = Clamp(cli_iBufferActions, INDEX(1), INDEX(20));
  cli_iMaxBPS = Clamp(cli_iMaxBPS, INDEX(100), INDEX(1000000));
  cli_iMinBPS = Clamp(cli_iMinBPS, INDEX(100), INDEX(1000000));
}

// check if up to date with current params
BOOL CSessionSocketParams::IsUpToDate(void)
{
  ClampParams();
  return 
    ssp_iBufferActions == cli_iBufferActions &&
    ssp_iMaxBPS == cli_iMaxBPS &&
    ssp_iMinBPS == cli_iMinBPS;
}

// update
void CSessionSocketParams::Update(void)
{
  ClampParams();
  ssp_iBufferActions = cli_iBufferActions;
  ssp_iMaxBPS = cli_iMaxBPS;
  ssp_iMinBPS = cli_iMinBPS;
}

// message operations
CNetworkMessage &operator<<(CNetworkMessage &nm, CSessionSocketParams &ssp)
{
  nm<<ssp.ssp_iBufferActions<<ssp.ssp_iMaxBPS<<ssp.ssp_iMinBPS;
  return nm;
}
CNetworkMessage &operator>>(CNetworkMessage &nm, CSessionSocketParams &ssp)
{
  nm>>ssp.ssp_iBufferActions>>ssp.ssp_iMaxBPS>>ssp.ssp_iMinBPS;
  return nm;
}

/*
 * Constructor.
 */
CServer::CServer(void) 
{
  TRACKMEM(Mem,"Network");

  srv_bActive = FALSE;
  srv_bSendPause = FALSE;
  srv_bPauseValue = FALSE;
  srv_bRunning = FALSE; 
  srv_tmNextAvailableDemoTimeSlot = -1.0f;

  srv_assoSessions.New(NET_MAXGAMECOMPUTERS);
  srv_apltPlayers.New(NET_MAXGAMEPLAYERS);

  // initialize player indices
  INDEX iPlayer = 0;
  FOREACHINSTATICARRAY(srv_apltPlayers, CPlayerTarget, itplt) {
    itplt->plt_Index = iPlayer;
    iPlayer++;
  }
 
  srv_embSendBuffer.Clear();
  srv_embOutgoingBuffer.Clear();
  // grow the buffer to the predefined size
  extern INDEX ser_iMaxBufferSize;
  if (srv_embSendBuffer.bu_slSize < ser_iMaxBufferSize) {
    srv_embSendBuffer.Expand(ser_iMaxBufferSize);
  }
  srv_embOutgoingBuffer.Expand(ser_iMaxBufferSize/3);
}

/*
 * Destructor.
 */
CServer::~CServer()
{
  srv_bActive = FALSE;
}

/*
 * Clear the object.
 */
void CServer::Stop(void)
{
  // stop gamespy
//#if _GAMESPY
//  GameSpy_ServerEnd();
//#endif

   srv_bRunning = FALSE;

  // tell all clients to disconnect
  INDEX ctClients = srv_assoSessions.sa_Count;
  INDEX iClient;
  for (iClient=0;iClient<ctClients;iClient++) {
    if (srv_assoSessions[iClient].sso_bActive) {
      SendDisconnectMessage(iClient,"Server stopped.");
    }
  }
  
  // run a few updates, so the message gets sent
  for (int ctUpdates=0;ctUpdates<10;ctUpdates++) {
    BOOL bFound = FALSE;
    for (iClient=0;iClient<ctClients;iClient++) {
      if (srv_assoSessions[iClient].sso_bActive) {
        bFound = TRUE;
        break;
      }
    }
    if (bFound == FALSE) {
      break;
    } else {
      _cmiComm.Server_Update();
      Sleep(100);
    }
  }


  // stop network driver server
  _cmiComm.Server_Close();
  // stop local client
  _cmiComm.Client_Close();

  {
    MEMTRACK_SETFLAGS(mem,MTF_NOSTACKTRACE);
    // clear all session
    srv_assoSessions.Clear();
    srv_assoSessions.New(NET_MAXGAMECOMPUTERS);
    // clear list of players
    srv_apltPlayers.Clear();
    srv_apltPlayers.New(NET_MAXGAMEPLAYERS);

  }

  // initialize player indices
  INDEX iPlayer = 0;
  {FOREACHINSTATICARRAY(srv_apltPlayers, CPlayerTarget, itplt) {
    itplt->plt_Index = iPlayer;
    iPlayer++;
  }}

  srv_embSendBuffer.Clear();


  srv_bActive = FALSE;
};

/*
 * Initialize server and start message handlers.
 */
void CServer::Start_t(void)
{
  // set up structures
  srv_tmLastProcessedTick = 0.0f;
  srv_iLastProcessedSequence = -1; // -1 so that next one will be 0
  srv_bActive = TRUE;
  srv_bRunning = FALSE;
  srv_bPause = FALSE;
  srv_bGameFinished = FALSE;
  srv_fServerStep = 0.0f;
  srv_bPauseValue = FALSE;

  // init network driver server
  _cmiComm.Server_Init_t();

  // clear list of players
  srv_apltPlayers.Clear();
  srv_apltPlayers.New(NET_MAXGAMEPLAYERS);

  // initialize player indices
  INDEX iPlayer = 0;
  {FOREACHINSTATICARRAY(srv_apltPlayers, CPlayerTarget, itplt) {
    itplt->plt_Index = iPlayer;
    iPlayer++;
  }}


//#if _GAMESPY
//  // init gamespy
//  if (_cmiComm.IsNetworkEnabled()) {
//    GameSpy_ServerInit();
//  }
//#endif
}

/*
 * Send disconnect message to some node.
 */
void CServer::SendDisconnectMessage(INDEX iClient, const char *strExplanation, BOOL bStream)
{
/* //0522 kwon 삭제.
  CSessionSocket &sso = srv_assoSessions[iClient];

  if (!bStream) {
    CNetworkMessage nmDisconnect(MSG_INF_DISCONNECTED);
    // compose message                                         
    nmDisconnect<<CTString(strExplanation);
    // send it
    _pNetwork->SendToClientReliable(iClient, nmDisconnect);
  } else {
    CTMemoryStream strmDisconnect;
    strmDisconnect<<INDEX(MSG_INF_DISCONNECTED);
    strmDisconnect<<CTString(strExplanation);

    // send the stream to the remote session state
    _pNetwork->SendToClientReliable(iClient, strmDisconnect);
  }
  // report that it has gone away
  CPrintF(TRANS("Client '%s' ordered to disconnect: %s\n"), 
    _cmiComm.Server_GetClientName(iClient), strExplanation);
  // if not disconnected before
  if (sso.sso_iDisconnectedState==0) {
    // mark the disconnection
    sso.sso_iDisconnectedState = 1;
  // if the client was already kicked before, but is still hanging here
  } else {
    // force the disconnection
    CPrintF(TRANS("Forcing client '%s' to disconnect\n"), 
      _cmiComm.Server_GetClientName(iClient));
    sso.sso_iDisconnectedState = 2;
  }
*/
}

/* Send one regular batch of sequences to a client. */
void CServer::SendGameStreamBlocks(INDEX iClient)
{
  // get corresponding session socket
  CSessionSocket &sso = srv_assoSessions[iClient];

  // gather needed data to decide what to send
  INDEX iLastSent = sso.sso_iLastSentSequence;
  INDEX ctMinBytes = sso.sso_sspParams.ssp_iMinBPS/20;
  INDEX ctMaxBytes = sso.sso_sspParams.ssp_iMaxBPS/20;
  // make sure outgoing message doesn't overflow UDP size
  ctMinBytes = Clamp(ctMinBytes, 0L, 1000L);
  ctMaxBytes = Clamp(ctMaxBytes, 0L, 1000L);
  // limit the clients BPS by server's local settings
  extern INDEX ser_iMaxAllowedBPS;
  ctMinBytes = ClampUp(ctMinBytes, (INDEX) ( ser_iMaxAllowedBPS/20L - MAX_HEADER_SIZE));
  ctMaxBytes = ClampUp(ctMaxBytes, (INDEX) (ser_iMaxAllowedBPS/20L - MAX_HEADER_SIZE));

  // prevent server/singleplayer from flooding itself
  if (iClient==0) {
    ctMinBytes = 0;
    ctMaxBytes = 1E6;
  }

//  CPrintF("Send%d(%d, %d, %d): ", iClient, iLastSent, ctMinBytes, ctMaxBytes);

  // start after last sequence that was sent and go upwards
  INDEX iSequence = iLastSent+1;
  INDEX iStep = +1;
//  CPrintF("last=%d -- ", iLastSent);

  // initialize the message that is to be sent
  CNetworkMessage nmGameStreamBlocks((UBYTE)MSG_GAMESTREAMBLOCKS);
  // get one message for last compressed message of valid size
  CNetworkMessage nmPackedBlocks((UBYTE)MSG_GAMESTREAMBLOCKS);
  CNetworkMessage nmPackedBlocksNew((UBYTE)MSG_GAMESTREAMBLOCKS);

  // repeat for max 100 sequences
  INDEX iBlocksOk = 0;
  INDEX iMaxSent = -1;
  for(INDEX i=0; i<100; i++) {
    if (iStep<0 && iBlocksOk>=3) {
//      break;
    }
    // get the stream block with current sequence
//    CPrintF("%d: ", iSequence);
    CNetworkStreamBlock *pnsbBlock;
    CNetworkStream::Result res = sso.sso_nsBuffer.GetBlockBySequence(iSequence, pnsbBlock);
    // if it is not found
    if (res!=CNetworkStream::R_OK) {
      // if going upward
      if (iStep>0 ) {
//        // if this block is missing
//        && res==CNetworkStream::R_BLOCKMISSING
        // if none sent so far
        if (iBlocksOk<=0) {
          // give up
//          CPrintF("giving up\n");
          break; 
        }
//        CPrintF("rewind ", iSequence);
        // rewind and continue downward
        if (iSequence == iLastSent+1) {
          iSequence = iLastSent-1;
        } else {
          iSequence = iLastSent;
        }
        iStep = -1;
        // retry
        continue;
      // otherwise
      } else {
        // stop adding more blocks
        break;
      }
    }
    // if uncompressed message would overflow
    if (nmGameStreamBlocks.nm_slSize+pnsbBlock->nm_slSize+32>MAX_NETWORKMESSAGE_SIZE) {
//      CPrintF("overflow ");
      break;
    }

    // add this block to the message and pack it
    pnsbBlock->WriteToMessage(nmGameStreamBlocks);
    nmPackedBlocksNew.Reinit();
    nmGameStreamBlocks.PackDefault(nmPackedBlocksNew);
    // if some blocks written already and the batch is too large
    if (iBlocksOk>0) {
      if (iStep>0 && nmPackedBlocksNew.nm_slSize>=ctMaxBytes ||
          iStep<0 && nmPackedBlocksNew.nm_slSize>=ctMinBytes ) {
        // stop
//        CPrintF("toomuch ");
        break;
      }
    }
    // use new pack
//    CPrintF("added ");
    nmPackedBlocks = nmPackedBlocksNew;
    iMaxSent = Max(iMaxSent, iSequence);
    iSequence+= iStep;
    iBlocksOk++;
  }
  
  // if no blocks to write
  if (iBlocksOk<=0) {
	  /*
    // if not sent anything for some time
    CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
    extern FLOAT ser_tmKeepAlive;
 //0522 kwon 삭제.
    if ((tvNow-sso.sso_tvLastMessageSent).GetSeconds()>ser_tmKeepAlive) {
      // send keepalive
      CNetworkMessage nmKeepalive(MSG_KEEPALIVE);
      _pNetwork->SendToClient(iClient, nmKeepalive);
      sso.sso_tvLastMessageSent = tvNow;
    }
*/
    //    CPrintF("nothing\n");
    return;
  }

  // send the message to the client
//  CPrintF("sent: %d=%dB\n", iBlocksOk, nmPackedBlocks.nm_slSize);
  _pNetwork->SendToClient(iClient, nmPackedBlocks);
  sso.sso_iLastSentSequence = Max(sso.sso_iLastSentSequence, iMaxSent);
  sso.sso_tvLastMessageSent = _pTimer->GetHighPrecisionTimer();

  // remove the block(s) that fall out of the buffer
  extern INDEX ser_iRememberBehind;
  sso.sso_nsBuffer.RemoveOlderBlocksBySequence(srv_iLastProcessedSequence-ser_iRememberBehind);

/* //0522 kwon 삭제.
  // if haven't sent pings for some time
  CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
  extern FLOAT ser_tmPingUpdate;
  if ((tvNow-sso.sso_tvLastPingSent).GetSeconds()>ser_tmPingUpdate) {
    // send ping info
    CNetworkMessage nmPings(MSG_INF_PINGS);
    for(INDEX i=0; i<NET_MAXGAMEPLAYERS; i++) {
      CPlayerTarget &plt = srv_apltPlayers[i];
      if (plt.IsActive()) {
        BOOL b = 1;
        nmPings.WriteBits(&b, 1);
        nmPings.WriteBits(&plt.plt_iPing, 10);
      } else {
        BOOL b = 0;
        nmPings.WriteBits(&b, 1);
      }
    }
    _pNetwork->SendToClient(iClient, nmPings);
    sso.sso_tvLastPingSent = tvNow;
  }
  */
}

/* Resend a batch of game stream blocks to a client. */
void CServer::ResendGameStreamBlocks(INDEX iClient, INDEX iSequence0, INDEX ctSequences)
{
  extern INDEX net_bReportMiscErrors;
  if (net_bReportMiscErrors) {
    CPrintF(TRANS("Server: Resending sequences %d-%d(%d) to '%s'..."), 
      iSequence0, iSequence0+ctSequences-1, ctSequences, _cmiComm.Server_GetClientName(iClient));
  }

  // get corresponding session socket
  CSessionSocket &sso = srv_assoSessions[iClient];

  // create a package message
  CNetworkMessage nmGameStreamBlocks((UBYTE)MSG_GAMESTREAMBLOCKS);
  CNetworkMessage nmPackedBlocks((UBYTE)MSG_GAMESTREAMBLOCKS);

  // for each sequence
  INDEX iSequence;
  for(iSequence = iSequence0; iSequence < iSequence0 + ctSequences; iSequence++) {
    // get the stream block with that sequence
    CNetworkStreamBlock *pnsbBlock;
    CNetworkStream::Result res = sso.sso_nsBuffer.GetBlockBySequence(iSequence, pnsbBlock);
    // if it is not found
    if (res!=CNetworkStream::R_OK) {
      // tell the requesting session state to disconnect
      SendDisconnectMessage(iClient, TRANS("Gamestream synchronization lost"));
      return;
    }

    CNetworkMessage nmPackedBlocksNew((UBYTE)MSG_GAMESTREAMBLOCKS);
    // pack it in the batch
    pnsbBlock->WriteToMessage(nmGameStreamBlocks);
    nmGameStreamBlocks.PackDefault(nmPackedBlocksNew);
    // if the batch is too large
    if (nmPackedBlocksNew.nm_slSize>512) {
      // stop
      break;
    }
    // use new pack
    nmPackedBlocks = nmPackedBlocksNew;
  }

  // send the last batch of valid size
  _pfNetworkProfile.IncrementCounter(CNetworkProfile::PCI_GAMESTREAMRESENDS);
  _pNetwork->SendToClient(iClient, nmPackedBlocks);
  extern INDEX net_bReportMiscErrors;
  if (net_bReportMiscErrors) {
    CPrintF(TRANS(" sent %d-%d(%d - %db)\n"), 
      iSequence0, iSequence, iSequence-iSequence0-1, nmPackedBlocks.nm_slSize);
  }
}

/* Get number of active players. */
INDEX CServer::GetPlayersCount(void)
{
  INDEX ctPlayers = 0;
  FOREACHINSTATICARRAY(srv_apltPlayers, CPlayerTarget, itplt) {
    if (itplt->IsActive()) {
      ctPlayers++;
    }
  }
  return ctPlayers;
}
/* Get number of active vip players. */
INDEX CServer::GetVIPPlayersCount(void)
{
  INDEX ctPlayers = 0;
  FOREACHINSTATICARRAY(srv_apltPlayers, CPlayerTarget, itplt) {
    if (itplt->IsActive() && srv_assoSessions[itplt->plt_iClient].sso_bVIP) {
      ctPlayers++;
    }
  }
  return ctPlayers;
}
/* Get total number of active clients. */
INDEX CServer::GetClientsCount(void)
{
  INDEX ctClients = 0;
  // for each active session
  for(INDEX iSession=0; iSession<srv_assoSessions.Count(); iSession++) {
    CSessionSocket &sso = srv_assoSessions[iSession];
    if (iSession>0 && !sso.IsActive()) {
      continue;
    }
    ctClients++;
  }

  return ctClients;

}
/* Get number of active vip clients. */
INDEX CServer::GetVIPClientsCount(void)
{
  INDEX ctClients = 0;
  // for each active session
  for(INDEX iSession=0; iSession<srv_assoSessions.Count(); iSession++) {
    CSessionSocket &sso = srv_assoSessions[iSession];
    if (iSession>0 && !sso.IsActive()) {
      continue;
    }
    if (sso.sso_bVIP) {
      ctClients++;
    }
  }

  return ctClients;
}
/* Get number of active observers. */
INDEX CServer::GetObserversCount(void)
{
  INDEX ctClients = 0;
  // for each active session
  for(INDEX iSession=0; iSession<srv_assoSessions.Count(); iSession++) {
    CSessionSocket &sso = srv_assoSessions[iSession];
    if (iSession>0 && !sso.IsActive()) {
      continue;
    }
    if (sso.sso_ctLocalPlayers == 0) {
      ctClients++;
    }
  }

  return ctClients;
}

/* Get number of active players on one client. */
INDEX CServer::GetPlayersCountForClient(INDEX iClient)
{
  INDEX ctPlayers = 0;
  FOREACHINSTATICARRAY(srv_apltPlayers, CPlayerTarget, itplt) {
    if (itplt->IsActive() && itplt->plt_iClient==iClient) {
      ctPlayers++;
    }
  }
  return ctPlayers;
}

/*
 * Find first inactive client.
 */
CPlayerTarget *CServer::FirstInactivePlayer(void)
{
  // for all players in game
  FOREACHINSTATICARRAY(srv_apltPlayers, CPlayerTarget, itplt) {
    // if player is not active
    if (!itplt->IsActive()) {
      // return it
      return &itplt.Current();
    }
  }
  // if no inactive players found, return error
  return NULL;
}

/*
 * Check if some character already exists in this session.
 */
BOOL CServer::CharacterNameIsUsed(CPlayerCharacter &pcCharacter)
{
  // for all players in game
  FOREACHINSTATICARRAY(srv_apltPlayers, CPlayerTarget, itplt) {
    // if it is active and has same character as this one
    if (itplt->IsActive() && itplt->plt_pcCharacter == pcCharacter) {
      // it exists
      return TRUE;
    }
  }
  // otherwise, it doesn't exist
  return FALSE;
}

// find a mask of all players on a certain client
ULONG CServer::MaskOfPlayersOnClient(INDEX iClient)
{
  ULONG ulClientPlayers = 0;
  for(INDEX ipl=0; ipl<srv_apltPlayers.Count(); ipl++) {
    CPlayerTarget &plt = srv_apltPlayers[ipl];
    if (plt.IsActive() && plt.plt_iClient==iClient) {
      ulClientPlayers |= 1UL<<ipl;
    }
  }
  return ulClientPlayers;
}

//! 서버의 메인loop. 메시지 처리후 로컬 클라이언트로 넘김.(맞나?ㅡㅡ;)
/*
 * Handle network messages.
 */
void CServer::ServerLoop(void)
{


  // if not started
  if (!srv_bActive) {
    ASSERTALWAYS("Running server loop for before starting server!");
    // do not gather/send actions
    return;
  }

  _pfNetworkProfile.StartTimer(CNetworkProfile::PTI_SERVER_LOOP);

  //! 모든 클라이언트의 메시지 핸들링.
  // handle all incoming messages
  HandleAll();
  
  if(_pNetwork->IsReadyStart()){//0311 kwon 
	  INDEX iSpeed = 1;
	  extern INDEX ser_bWaitFirstPlayer;
	  //! srv_bPause는 서버가 뜬후에 접속자가 없을때 멈춰놓는다."Pause game by local machine"
	  // if the local session is keeping up with time and not paused
	  BOOL bPaused = srv_bPause || _pNetwork->ga_bLocalPause || _pNetwork->IsWaitingForPlayers() || srv_bGameFinished;
	  if (_pNetwork->ga_sesSessionState.ses_bKeepingUpWithTime && !bPaused ) {
		  
		  //! 시간 경과. ga_sesSessionState는 로컬 세션이다.초기값은 srv_fServerStep=1.0f
		  // advance time
		  srv_fServerStep += _pNetwork->ga_fGameRealTimeFactor*_pNetwork->ga_sesSessionState.ses_fRealTimeFactor;
		  //! 다음 tick에 이르면,
		  // if stepped to next tick
		  if (srv_fServerStep>=1.0f) {
			  // find how many ticks were stepped
			  INDEX iSpeed = ClampDn(INDEX(srv_fServerStep), 1L);
			  srv_fServerStep -= iSpeed;
			  //! 각각의 tick에 대하여,	
			  // for each tick
			  for( INDEX i=0; i<iSpeed; i++) {
				  // increment tick counter and processed sequence
				  if (!ser_bWaitFirstPlayer) {
					  //플레이어가 들어왔을때 실행된다.그전에는 0.
					  srv_tmLastProcessedTick += _pTimer->TickQuantum; //마지막으로 모든 액션을 재전송한 시간.
				  }
				  srv_iLastProcessedSequence++; //! 마지막으로 보낸 게임 스트림 블록의 시퀀스.
				  // make allaction messages for one tick - this in effect advances the session timer
				  //! 모든 액션 메시지를 생성하고, 서버시간을 쓴다.
				  // create all-actions message and write server time into it
				  CNetworkStreamBlock nsbAllActions(MSG_SEQ_ALLACTIONS, srv_iLastProcessedSequence);
				  nsbAllActions<<srv_tmLastProcessedTick;
				  //! 로컬 세션에 블록을 추가한다.
				  // add the block to the buffer
				  srv_assoSessions[0].sso_nsBuffer.AddBlock(nsbAllActions);
				  
			  } 
		  }
	  }
	  //! 게임 스트림 블록을 로컬 클라이언트로 보낸다.
	  // send one regular batch of sequences to the client
	  SendGameStreamBlocks(0);
	  
	  _pfNetworkProfile.StopTimer(CNetworkProfile::PTI_SERVER_LOOP);
  }
}

//! 로컬 클라이언트의 연결요청 처리.
/* Send initialization info to local client. */
void CServer::ConnectLocalSessionState(INDEX iClient, CNetworkMessage &nm)
{
  // find session of this client
  CSessionSocket &sso = srv_assoSessions[iClient];
  // activate it
  sso.Activate();
  // prepare his initialization message
  CNetworkMessage nmInitMainServer((UBYTE)MSG_REP_CONNECTLOCALSESSIONSTATE);
  nmInitMainServer<<srv_tmLastProcessedTick;
  nmInitMainServer<<srv_iLastProcessedSequence;
  sso.sso_ctLocalPlayers = -1;
  nm>>sso.sso_sspParams;

  // send him server session state initialization message
  _pNetwork->SendToClientReliable(iClient, nmInitMainServer);
}
//! 리모트 클라이언트의 접속요청 처리
/* Send initialization info to remote client. */
void CServer::ConnectRemoteSessionState(INDEX iClient, CNetworkMessage &nm)
{
  ASSERT(iClient>0);
  // find session of this client
  CSessionSocket &sso = srv_assoSessions[iClient];
  //! 허용되지 않는 아이피라면 접속 끊는다.
  // if the IP is banned
  if (!MatchesBanMask(_cmiComm.Server_GetClientName(iClient), ser_strIPMask) != !ser_bInverseBanning) {
    // disconnect the client
    SendDisconnectMessage(iClient, TRANS("You are banned from this server"), /*bStream=*/TRUE);
    return;
  }

  // read version info
  INDEX iTag, iMajor, iMinor;
  //! 버전을 읽고,
  nm>>iTag;
  if (iTag=='VTAG') {
    nm>>iMajor>>iMinor;
  } else {
    iMajor = 109;
    iMinor = 1;
  }
  // if wrong
  if (iMajor!=_SE_BUILD_MAJOR || iMinor!=_SE_BUILD_MINOR) {
    // disconnect the client
    CTString strExplanation;
    strExplanation.PrintF(TRANS(
      "This server runs version %d.%d, your version is %d.%d.\n"
      "Please visit http://www.croteam.com for information on version updating."),
      _SE_BUILD_MAJOR, _SE_BUILD_MINOR, iMajor, iMinor);
    SendDisconnectMessage(iClient, strExplanation, /*bStream=*/TRUE);
    return;
  }
  extern CTString net_strConnectPassword;
  extern CTString net_strVIPPassword;
  extern CTString net_strObserverPassword;
  extern INDEX net_iVIPReserve;
  extern INDEX net_iMaxObservers;
  extern INDEX net_iMaxClients;
  CTString strGivenMod;
  CTString strGivenPassword;
  nm>>strGivenMod>>strGivenPassword;
  INDEX ctWantedLocalPlayers;
  nm>>ctWantedLocalPlayers;
  // if wrong mod
  if (_strModName!=strGivenMod) {
    // disconnect the client
    // NOTE: DO NOT TRANSLATE THIS STRING!
    CTString strMod(0, "MOD:%s\\%s", _strModName, _strModURL);
    SendDisconnectMessage(iClient, strMod, /*bStream=*/TRUE);
    return;
  }
  //! 허용된 플레이어,클라이언트를 얻는다.
  // get counts of allowed players, clients, vips and  check for connection allowance
  INDEX ctMaxAllowedPlayers = _pNetwork->ga_sesSessionState.ses_ctMaxPlayers;
  INDEX ctMaxAllowedClients = ctMaxAllowedPlayers;
  if (net_iMaxClients>0) {
    ctMaxAllowedClients = ClampUp(net_iMaxClients, (INDEX)NET_MAXGAMECOMPUTERS);
  }
  INDEX ctMaxAllowedVIPPlayers = 0;
  INDEX ctMaxAllowedVIPClients = 0;
  if (net_iVIPReserve>0 && net_strVIPPassword!="") {
    ctMaxAllowedVIPPlayers = ClampDn(net_iVIPReserve-GetVIPPlayersCount(), 0L);
    ctMaxAllowedVIPClients = ClampDn(net_iVIPReserve-GetVIPClientsCount(), 0L);
  }
  INDEX ctMaxAllowedObservers = net_iMaxObservers;
  //! 현재 플레이하고 있는 사람수. 현재 활성화된 클라이언트 수.
  // get current counts
  INDEX ctCurrentPlayers = GetPlayersCount();
  INDEX ctCurrentClients = GetClientsCount();
  INDEX ctCurrentObservers = GetObserversCount();

  // check which passwords this client can satisfy
  BOOL bAutorizedAsVIP = FALSE;
  BOOL bAutorizedAsObserver = FALSE;
  BOOL bAutorizedAsPlayer = FALSE;
  if (net_strVIPPassword!="" && net_strVIPPassword==strGivenPassword) {
    bAutorizedAsVIP = TRUE;
    bAutorizedAsPlayer = TRUE;
    bAutorizedAsObserver = TRUE;
  }
  if (net_strConnectPassword=="" || net_strConnectPassword==strGivenPassword) {
    bAutorizedAsPlayer = TRUE;
  }
  if ((net_strObserverPassword==""&&bAutorizedAsPlayer) || net_strObserverPassword==strGivenPassword) {
    bAutorizedAsObserver = TRUE;
  }

  // if the user is not authorised as an VIP
  if (!bAutorizedAsVIP) {
    // artificially decrease allowed number of players and clients
    ctMaxAllowedPlayers = ClampDn(ctMaxAllowedPlayers-ctMaxAllowedVIPPlayers, 0L);
    ctMaxAllowedClients = ClampDn(ctMaxAllowedClients-ctMaxAllowedVIPClients, 0L);
  }

  //! 접속인원이 초과 되었다면,
  // if too many clients or players
  if (ctCurrentPlayers+ctWantedLocalPlayers>ctMaxAllowedPlayers
    ||ctCurrentClients+1>ctMaxAllowedClients) {
    // disconnect the client
    SendDisconnectMessage(iClient, TRANS("Server full!"), /*bStream=*/TRUE);
    return;
  }
  //! 옵저버로서 연결요청이라면,
  // if the user is trying to connect as observer
  if (ctWantedLocalPlayers==0) {
    // if too many observers already
    if (ctCurrentObservers>=ctMaxAllowedObservers && !bAutorizedAsVIP) {
      // disconnect the client
      SendDisconnectMessage(iClient, TRANS("Too many observers!"), /*bStream=*/TRUE);
      return;
    }
    // if observer password is wrong
    if (!bAutorizedAsObserver) {
      // disconnect the client
      if (strGivenPassword=="") {
        SendDisconnectMessage(iClient, TRANS("This server requires password for observers!"), /*bStream=*/TRUE);
      } else {
        SendDisconnectMessage(iClient, TRANS("Wrong observer password!"), /*bStream=*/TRUE);
      }
    }
  // if the user is trying to connect as player
  } else {
    // if player password is wrong
    if (!bAutorizedAsPlayer) {
      // disconnect the client
      if (strGivenPassword=="") {
        SendDisconnectMessage(iClient, TRANS("This server requires password to connect!"), /*bStream=*/TRUE);
      } else {
        SendDisconnectMessage(iClient, TRANS("Wrong password!"), /*bStream=*/TRUE);
      }
    }
    
  }

  // activate it
  sso.Activate();
  // load parameters for it
  sso.sso_ctLocalPlayers = ctWantedLocalPlayers;
  sso.sso_bVIP = bAutorizedAsVIP;
  sso.sso_ubNumLevelChanges = _pNetwork->ga_ubNumLevelChanges;
  nm>>sso.sso_sspParams;
/* //0522 kwon 삭제.
  // try to
  try {
    // create base info to be sent
    extern CTString ser_strMOTD;
    CTMemoryStream strmInfo;
    strmInfo<<INDEX(MSG_REP_CONNECTREMOTESESSIONSTATE);
    strmInfo<<_pNetwork->ga_ubNumLevelChanges;
    strmInfo<<ser_strMOTD;
    strmInfo<<_pNetwork->ga_World.wo_fnmFileName;
    strmInfo<<_pNetwork->ga_sesSessionState.ses_ulSpawnFlags;
    strmInfo.Write_t(_pNetwork->ga_aubProperties, NET_MAXSESSIONPROPERTIES);
    SLONG slSize = strmInfo.GetStreamSize();

    // send the stream to the remote session state
    _pNetwork->SendToClientReliable(iClient, strmInfo);
  
    CPrintF(TRANS("Server: Sent initialization info to '%s' (%dk)\n"),
      (const char*)_cmiComm.Server_GetClientName(iClient), slSize/1024);
  // if failed
  } catch (char *strError) {
    CloseClient(iClient);
    // deactivate it
    sso.Deactivate();

    // report error
    CPrintF(TRANS("Server: Cannot prepare connection data: %s\n"), strError);
  }
*/
}



//! 게임 스테이트 데이타 만들기.
void CServer::PrepareGameStateData_t(CTMemoryStream &strmStream,SLONG &slFullSize,SLONG &slSize)
{
/* //0522 kwon 삭제.
  // prepare files or memory streams for connection info
  CTFileStream strmGameStateFile; CTMemoryStream strmGameStateMem;
  CTStream *pstrmGameState; 
  extern INDEX net_bDumpConnectionInfo;
  if (net_bDumpConnectionInfo) {
    strmGameStateFile.Create_t(CTString("Temp\\GameState.bin"));
    pstrmGameState = &strmGameStateFile;
  } else {
    pstrmGameState = &strmGameStateMem;
  }

  (*pstrmGameState) << _pNetwork->ga_ubNumLevelChanges;
  // write main session state, for sending over network
  _pNetwork->ga_sesSessionState.Write_t(pstrmGameState,TRUE);

  //! crc 체크 
  // append CRC check data to the stream
  pstrmGameState->Write_t(_pNetwork->ga_pubCRCList, _pNetwork->ga_slCRCList);
  pstrmGameState->SetPos_t(0);
  slFullSize = pstrmGameState->GetStreamSize();

  // pack it up
  strmStream<<INDEX(MSG_REP_GAMESTATE);  
  //! 스트림 압축하기.
  CzlibCompressor comp;
  comp.PackStream_t(*pstrmGameState, strmStream);
  slSize = strmStream.GetStreamSize();
  */
 
};


//! 게임데이타 요청
/* Send the state of the game to a remote client */
void CServer::SendGameStateData(INDEX iClient)
{
  ASSERT(iClient>0);
  // find session of this client
  CSessionSocket &sso = srv_assoSessions[iClient];
  ASSERT(sso.IsActive());

  //! 서버의 레벨이 바뀌었다면 접속 끊는다.
  if (iClient!=0 && sso.sso_ubNumLevelChanges != _pNetwork->ga_ubNumLevelChanges) {
    CTString strMessage;
    strMessage.PrintF(TRANS("Server changed levels"));
    SendDisconnectMessage(iClient, strMessage);
    return;
  }
  //! 로컬 세션에서 복사해온다.
  // copy its buffer from local session state
  sso.sso_nsBuffer.Copy(srv_assoSessions[0].sso_nsBuffer);

   // try to
  try {
    
    SLONG slFullSize,slSize;
    CTMemoryStream strmInfo;
	//! 게임 데이타 만들기.
    PrepareGameStateData_t(strmInfo,slFullSize,slSize);

	//! 클라이언트로 보낸다.
    // send the stream to the remote session state
    _pNetwork->SendToClientReliable(iClient, strmInfo);
    
    CPrintF(TRANS("Server: Sent connection data to '%s' (%dk)->(%dk)\n"),
      (const char*)_cmiComm.Server_GetClientName(iClient), 
      slFullSize/1024,slSize/1024);
    extern INDEX net_bDumpConnectionInfo;
    if (net_bDumpConnectionInfo) {
      CPrintF(TRANS("Server: Connection data dumped.\n"));
    }

  // if failed
  } catch (char *strError) {
    CTString strMessage;
    strMessage.PrintF(TRANS("Server changed levels"));
    SendDisconnectMessage(iClient, strMessage);
    // report error
    CPrintF(TRANS("Server: Cannot prepare connection data: %s\n"), strError);
    return;
  }

  // start the client interface
  sso.sso_tmLastTickAcknowledge = _pNetwork->ga_sesSessionState.ses_tmLastProcessedTick - _pTimer->TickQuantum;
  sso.sso_bTrackData = TRUE;
  if (srv_bRunning) {
    srv_embSendBuffer.RequestTickAcknowledge(_pNetwork->ga_sesSessionState.ses_tmLastProcessedTick,1);  
  }
  //! 이 서버에 처음 접속하는 첫 리모트 세션만 한다.
  // do this only if this is the first remote session on this server
  InitServerNetProcess();
}

//! 안쓴다.
/* Send session state data to remote client. */
void CServer::SendSessionStateData(INDEX iClient)
{
  ASSERT(iClient>0);
  // find session of this client
  CSessionSocket &sso = srv_assoSessions[iClient];
  // copy its buffer from local session state
  sso.sso_nsBuffer.Copy(srv_assoSessions[0].sso_nsBuffer);

  // try to
  try {
    // prepare files or memory streams for connection info
    CTFileStream strmStateFile; CTMemoryStream strmStateMem;
    CTFileStream strmDeltaFile; CTMemoryStream strmDeltaMem;
    CTStream *pstrmState; CTStream *pstrmDelta;
    extern INDEX net_bDumpConnectionInfo;
    if (net_bDumpConnectionInfo) {
      strmStateFile.Create_t(CTString("Temp\\State.bin"));
      strmDeltaFile.Create_t(CTString("Temp\\Delta.bin"));
      pstrmState = &strmStateFile;
      pstrmDelta = &strmDeltaFile;
    } else {
      pstrmState = &strmStateMem;
      pstrmDelta = &strmDeltaMem;
    }

    // write main session state
    _pNetwork->ga_sesSessionState.Write_t(pstrmState);
    pstrmState->SetPos_t(0);
    SLONG slFullSize = pstrmState->GetStreamSize();

    CTMemoryStream strmInfo;
    //strmInfo<<INDEX(MSG_REP_STATEDELTA);

    // compress it to another one, using delta from original
    CTMemoryStream strmDefaultState;
    strmDefaultState.Write_t
      (_pNetwork->ga_pubDefaultState, _pNetwork->ga_slDefaultStateSize);
    strmDefaultState.SetPos_t(0);
    DIFF_Diff_t(&strmDefaultState, pstrmState, pstrmDelta);
    pstrmDelta->SetPos_t(0);
    SLONG slDeltaSize = pstrmDelta->GetStreamSize();
    CzlibCompressor comp;
    comp.PackStream_t(*pstrmDelta, strmInfo);

    SLONG slSize = strmInfo.GetStreamSize();

    // send the stream to the remote session state
    _pNetwork->SendToClientReliable(iClient, strmInfo);
  
    CPrintF(TRANS("Server: Sent connection data to '%s' (%dk->%dk->%dk)\n"),
      (const char*)_cmiComm.Server_GetClientName(iClient), 
      slFullSize/1024, slDeltaSize/1024, slSize/1024);
    if (net_bDumpConnectionInfo) {
      CPrintF(TRANS("Server: Connection data dumped.\n"));
    }

  // if failed
  } catch (char *strError) {
    // deactivate it
    sso.Deactivate();

    // report error
    CPrintF(TRANS("Server: Cannot prepare connection data: %s\n"), strError);
  }
}
//! 모든 메시지 핸들링.
/* Handle incoming network messages. */
void CServer::HandleAll()
{
  // clear last accepted client info
  INDEX iClient = -1;
/*  if (_cmiComm.GetLastAccepted(iClient)) {
    CPrintF(TRANS("Server: Accepted session connection by '%s'\n"),
      _cmiComm.Server_GetClientName(iClient));
  }
	*/
  //! 떠있는 모든 클라이언트에 대해서 메시지 핸들을 한다.
  // for each active client
  {for( INDEX iClient=0; iClient<NET_MAXGAMECOMPUTERS; iClient++) {
    // if the client is  connected, handle all of its messages
    if (_cmiComm.Server_IsClientUsed(iClient)) {
      HandleAllForAClient(iClient);
    }
  }}
}




void CServer::HandleAllForAClient(INDEX iClient)
{
  ASSERT(_cmiComm.Server_IsClientUsed(iClient));

	// update the client's max BPS limit from the session socket parameters
	cm_aciClients[iClient].ci_pbOutputBuffer.pb_pbsLimits.pbs_fBandwidthLimit = srv_assoSessions[iClient].sso_sspParams.ssp_iMaxBPS*8;

  // find session of this client
  CSessionSocket &sso = srv_assoSessions[iClient];
  
  if (iClient>0 && sso.sso_bActive && sso.sso_tvMessageReceived.tv_llValue>0 && // && sso.sso_bSendStream
    (_pTimer->GetHighPrecisionTimer()-sso.sso_tvMessageReceived).GetSeconds()>net_tmDisconnectTimeout) {
//0203
//    SendDisconnectMessage(iClient, TRANS("Connection timeout"));
  }

  // if the client is disconnected
  if (!_cmiComm.Server_IsClientUsed(iClient) || sso.sso_iDisconnectedState>1) {
    CPrintF(TRANS("Server: Client '%s' disconnected.\n"), _cmiComm.Server_GetClientName(iClient));
    // clear it
    _cmiComm.Server_ClearClient(iClient);
    // free all that data that was allocated for the client
    HandleClientDisconected(iClient);
  }

  CNetworkMessage nmReceived;
  
  // repeat
  FOREVER {
    // if there is some reliable message
    if (_pNetwork->ReceiveFromClientReliable(iClient, nmReceived)) {
      // process it
      Handle(iClient, nmReceived);

    // if there are no more messages
    } else {
      // skip to receiving unreliable
      break;
    }
  }

	// if the client has confirmed disconnect in this loop
  if (!_cmiComm.Server_IsClientUsed(iClient) || sso.sso_iDisconnectedState>1) {
    CPrintF(TRANS("Server: Client '%s' disconnected.\n"), _cmiComm.Server_GetClientName(iClient));
    // clear it
    _cmiComm.Server_ClearClient(iClient);
    // free all that data that was allocated for the client
    HandleClientDisconected(iClient);
  }

  // repeat
  FOREVER {
	  //! 언릴라이어블 메시지를 받은게 있다면,
    // if there is some unreliable message
    if (_pNetwork->ReceiveFromClient(iClient, nmReceived)) {
	  //! 처리.
      // process it
      Handle(iClient, nmReceived);
    // if there are no more messages
    } else {
      // finish with this client
      break;
    }
  }
}

void CServer::HandleClientDisconected(INDEX iClient)
{
  // find session of this client
  CSessionSocket &sso = srv_assoSessions[iClient];
  // remove it from ServerNetProcess
  CloseClient(iClient);
  // deactivate it
  sso.Deactivate();            
/* //0522 kwon 삭제.
  BOOL bIsTrackingData = FALSE;
  for (INDEX iSession=1;iSession < srv_assoSessions.Count();iSession++) {
    if (srv_assoSessions[iSession].sso_bTrackData) {
      ASSERT(srv_assoSessions[iSession].IsActive());
      bIsTrackingData = TRUE;
      break;
    }
  }
  
  INDEX iPlayer = 0;
  FOREACHINSTATICARRAY(srv_apltPlayers, CPlayerTarget, itplt) {
    // if player is on that client
    if (itplt->plt_iClient==iClient) {
      // create message for removing player from all session
      CNetworkStreamBlock nsbRemPlayerData(MSG_SEQ_REMPLAYER, ++srv_iLastProcessedSequence);
      nsbRemPlayerData<<iPlayer;      // player index
      // put the message in buffer
      srv_assoSessions[0].sso_nsBuffer.AddBlock(nsbRemPlayerData);

      // if the last remote client just got kicked, stop server tick processing
      if (!bIsTrackingData) {//!bIsTrackingData
        ASSERT(ActiveRemoteSessions()==0);
        StopNetProcess();
      } else {
        // else notify remaining clients that the player has left the building
        srv_emEntityMessage.WriteRemovePlayer(iPlayer);
        SendMessage(srv_emEntityMessage);
      }
    }
    iPlayer++;
  }
*/
}

// split the rcon response string into lines and send one by one to the client
static void SendAdminResponse(INDEX iClient, const CTString &strResponse)
{
/* //0522 kwon 삭제
  CTString str = strResponse;

  while (str!="") {
    CTString strLine = str;
    strLine.OnlyFirstLine();
    str.RemovePrefix(strLine);
    str.DeleteChar(0);
    if (strLine.Length()>0) { 
      CNetworkMessage nm(MSG_ADMIN_RESPONSE);
      nm<<strLine;
      _pNetwork->SendToClientReliable(iClient, nm);
    }
  }
*/
}
//! 서버의 메시지 타입에 따른 메시지 처리.
void CServer::Handle(INDEX iClient, CNetworkMessage &nmMessage)
{
	CSessionSocket &sso = srv_assoSessions[iClient];
	sso.sso_tvMessageReceived = _pTimer->GetHighPrecisionTimer();
	
	/*
	// 애완동물이 활성화 되어있는 상태.
	if( INFO()->_PetTargetInfo.bIsActive && INFO()->_PetTargetInfo.pen_pEntity )
	{
		CEntity *pEntity = INFO()->_PetTargetInfo.pen_pEntity;
		if( pEntity->IsFirstExtraFlagOn  (ENF_EX1_CURRENT_PET ))
		{
			static unsigned int tmStartTime = timeGetTime();
			static CPlacement3D plOld = CPlacement3D(FLOAT3D(0.0f, 0.0f, 0.0f), ANGLE3D(0.0f, 0.0f, 0.0f));
			if(timeGetTime() - tmStartTime > 1000 )
			{
				FLOAT3D fDelta = pEntity->GetPlacement().pl_PositionVector - plOld.pl_PositionVector;				
				if( fDelta.Length() > 1.0f )
				{
					_pNetwork->SendMoveMessage(pEntity, pEntity->GetPlacement(), 1.0f);
				}
				else
				{
					_pNetwork->SendStopMessage(pEntity, pEntity->GetPlacement());
				}
				tmStartTime = timeGetTime();
				plOld = pEntity->GetPlacement();
			}
		}
	}
	*/
	
	switch (nmMessage.GetType()) {
	/* //0522 kwon 삭제.
	// if if it is just keepalive, ignore it
	case MSG_KEEPALIVE: break;
	case MSG_REP_DISCONNECTED: {
	CSessionSocket &sso = srv_assoSessions[iClient];
	sso.sso_iDisconnectedState=2;
	} break;
		*/
		//! 로컬 클라이언트의 연결요청
		// if local session state asks for registration
	case MSG_REQ_CONNECTLOCALSESSIONSTATE: {
		ConnectLocalSessionState(iClient, nmMessage);
										   } break;
		
										   /* //0311 kwon 삭제
										   //! 리모트 클라이언트의 연결요청.
										   // if remote server asks for registration
										   case MSG_REQ_CONNECTREMOTESESSIONSTATE: {
										   ConnectRemoteSessionState(iClient, nmMessage);
										   } break;
										   //! 게임 데이타 요청.
										   // if remote server asks for data
										   case MSG_REQ_GAMESTATE: {
										   CPrintF(TRANS("Sending game state response\n"));
										   SendGameStateData(iClient);
										   sso.sso_ubNumLevelChanges = _pNetwork->ga_ubNumLevelChanges;
										   } break;
										   //0130 서버 본격 시작.
										   case MSG_PLAYER_MOVE: {	
										   
											 ULONG x,y,z,anglex,angley,anglez;
											 UBYTE bRun;
											 
											   nmMessage>>x;
											   nmMessage>>y;
											   nmMessage>>z;
											   nmMessage>>bRun;
											   nmMessage>>anglex;
											   nmMessage>>angley;
											   nmMessage>>anglez;
											   
												 CNetworkMessage nmPlayerMove(MSG_PLAYER_MOVE);	
												 
												   //	CPrintF(TRANS("iClinet:  %-2d   \n"),iClient);
												   for (INDEX iPlayer=0;iPlayer<NET_MAXGAMEPLAYERS;iPlayer++) {
												   INDEX iSession = srv_apltPlayers[iPlayer].plt_iClient;
												   
													 //		 CPrintF(TRANS("%-2d iPlayer of iSession:  %-2d   \n"), iPlayer ,srv_apltPlayers[iPlayer].plt_iClient);
													 if (iSession == -1) {
													 continue;
													 }
													 if (srv_assoSessions[iSession].IsActive()) {
													 
													   CPlayerEntity *penPlayerEntity = _pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity;
													   if (iClient == iSession) {		//0203	
													   //				CNetworkMessage nmPlayerMove(MSG_PLAYER_MOVE);	
													   nmPlayerMove<<penPlayerEntity->en_ulID;
													   nmPlayerMove<<x;    
													   nmPlayerMove<<y;    
													   nmPlayerMove<<z;    
													   nmPlayerMove<<bRun;  
													   nmPlayerMove<<anglex;
													   nmPlayerMove<<angley;
													   nmPlayerMove<<anglez;
													   
														 //penPlayerEntity->SetDesiredTranslation(FLOAT3D(x,y,z));		
														 //0203
														 //	_pNetwork->SendToClient(iClient, nmPlayerMove);
														 //	_pNetwork->SendToClient(iSession, nmPlayerMove);
														 break;
														 
														   }
														   }
														   }
														   for (iPlayer=0;iPlayer<NET_MAXGAMEPLAYERS;iPlayer++) {
														   INDEX iSession = srv_apltPlayers[iPlayer].plt_iClient;
														   if (iSession == -1 || iSession==iClient) {//0206
														   continue;
														   }
														   if (srv_assoSessions[iSession].IsActive()) {
														   _pNetwork->SendToClient(iSession, nmPlayerMove);
														   }
														   }
														   
															 }break;
		*/
		// if player asks for registration
	case MSG_REQ_CONNECTPLAYER: {
		
		//0130 이놈의 변수땜에 내가 고생을...ㅡㅡ+
		CSessionSocket &sso = srv_assoSessions[iClient];
		sso.sso_bSendStream = TRUE;
		
		
		if (iClient!=0 && sso.sso_ubNumLevelChanges != _pNetwork->ga_ubNumLevelChanges) {
			CTString strMessage;
			strMessage.PrintF(TRANS("Server changed levels"));
			SendDisconnectMessage(iClient, strMessage);
			break;
		}
		// check that someone doesn't add too many players
		/* if (iClient>0 && GetPlayersCountForClient(iClient)>=sso.sso_ctLocalPlayers) {
		CTString strMessage;
		strMessage.PrintF(TRANS("Protocol violation"));
		SendDisconnectMessage(iClient, strMessage);
								}*/
		
		// read character data from the message
		CPlayerCharacter pcCharacter;
		nmMessage>>pcCharacter;
		
		// if the name is banned
		if (!MatchesBanMask(pcCharacter.GetName(), ser_strNameMask) != !ser_bInverseBanning) {
			// disconnect the client
			SendDisconnectMessage(iClient, TRANS("You are banned from this server"), /*bStream=*/TRUE);
			return;
		}
		
		CPlayerTarget *ppltNewPlayer;
		
		// find some inactive player
		if (_pNetwork->ga_sesSessionState.ses_ctMaxPlayers == 1) {
			ppltNewPlayer = &srv_apltPlayers[0];
		} else {
			ppltNewPlayer = FirstInactivePlayer();
		}
		if (ppltNewPlayer!=NULL) {
			// activate it
			//ppltNewClient->Activate(iClient);
			
			
			// get the player index
			INDEX iNewPlayer = ppltNewPlayer->plt_Index;
			pcCharacter.pc_iPlayerIndex = iNewPlayer;
			// reserve entity IDs for the player entity and it's weapons and animator entities
			ULONG ulEntityID = _pNetwork->ga_World.wo_ulNextEntityID++;
			ULONG ulWeaponsID = _pNetwork->ga_World.wo_ulNextEntityID++;
			ULONG ulAnimatorID = _pNetwork->ga_World.wo_ulNextEntityID++;
			
			// remember its character
			ppltNewPlayer->plt_pcCharacter = pcCharacter;      
			
			// create a create message to create this player on all clients      
			CEntityMessage emEntityMessage;
			emEntityMessage.WriteAddPlayer(ulEntityID,iNewPlayer,pcCharacter,ulWeaponsID,ulAnimatorID,iClient);
			SendMessage(emEntityMessage);    
			
			// create the player entity using above IDs 
			_pNetwork->ga_sesSessionState.AddPlayer(iNewPlayer,pcCharacter,ulEntityID,ulWeaponsID,ulAnimatorID,iClient);
			
			extern INDEX ser_bWaitFirstPlayer;
			ser_bWaitFirstPlayer = 0; // player is here don't wait any more
			
			// send client initialization message
			CNetworkMessage nmPlayerRegistered((UBYTE)MSG_REP_CONNECTPLAYER);
			nmPlayerRegistered<<_pNetwork->ga_ubNumLevelChanges;
			nmPlayerRegistered<<iNewPlayer;   // player index
			_pNetwork->SendToClientReliable(iClient, nmPlayerRegistered);
			
			//#if _GAMESPY
			// notify gamespy
			//      GameSpy_ServerStateChanged();
			//#endif
			// if refused
		} else {
			// send him refusal message
			SendDisconnectMessage(iClient, TRANS("Too many players in session."));
		}
								} break;
		
		// if client source wants to change character
	case MSG_REQ_CHARACTERCHANGE: {
		// read character data from the message
		INDEX iPlayer;
		CPlayerCharacter pcCharacter;
		nmMessage>>iPlayer>>pcCharacter;
		// first check if the request is valid
		if (iPlayer<0 || iPlayer>srv_apltPlayers.Count() ) {
			break;
		}
		CPlayerTarget &plt = srv_apltPlayers[iPlayer];
		if (plt.plt_iClient!=iClient || !(plt.plt_pcCharacter==pcCharacter) ) {
			break;
		}
		
		// if all was right, add that as change a sequence
		CNetworkStreamBlock nsbChangeChar(MSG_SEQ_CHARACTERCHANGE, ++srv_iLastProcessedSequence);
		nsbChangeChar<<iPlayer;
		nsbChangeChar<<pcCharacter;
		
		plt.plt_pcCharacter = pcCharacter;
		
		// put the message in buffer
		srv_assoSessions[0].sso_nsBuffer.AddBlock(nsbChangeChar);
		
								  } break;
		
		//! 이건 로컬클라이언트 만 쓴다.
		// if client source sends action packet
	case MSG_ACTIONS: 
	{
		CSessionSocket &sso = srv_assoSessions[iClient];
		
		// for each possible player on that client
		for(INDEX ipls=0; ipls<NET_MAXLOCALPLAYERS; ipls++) 
		{
			// see if saved in the message
			BOOL bSaved = 0;
			nmMessage.ReadBits(&bSaved, 1);
			// if saved
			if (bSaved) 
			{
				// read client index
				INDEX iPlayer = 0;
				nmMessage.ReadBits(&iPlayer, 4);
				CPlayerTarget &plt = srv_apltPlayers[iPlayer];
				// if the player is not on that client
				if (plt.plt_iClient!=iClient) {
					// consider the entire message invalid
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Player actions received with invalid client index.\n"));
					}
					break;
				}
				// read ping
				plt.plt_iPing = 0;
				nmMessage.ReadBits(&plt.plt_iPing, 10);
				// let the corresponding client buffer receive the message
				INDEX iMaxBuffer = sso.sso_sspParams.ssp_iBufferActions;
				plt.ReceiveActionPacket(&nmMessage, iMaxBuffer);
			}
		}
					  } break;
					  /* //0311 삭제
					  // if client sent a synchronization check
					  case MSG_SYNCCHECK: {
					  
						ASSERT(FALSE);
						} break;
						// if a client requests resend of some game stream packets
						case MSG_REQUESTGAMESTREAMRESEND: {
						// get the sequences from the block
						INDEX iSequence0, ctSequences;
						nmMessage>>iSequence0;
						nmMessage>>ctSequences;
						// resend the game stream blocks to the client
						ResendGameStreamBlocks(iClient, iSequence0, ctSequences);
						} break;
		*/
		// if a client wants to toggle pause
		/* //0522 kwon 삭제.
		case MSG_REQ_PAUSE: {
		// read the pause state from the message
		BOOL bWantPause;
		nmMessage>>(INDEX&)bWantPause;
		// if state is new
		if (!srv_bPause != !bWantPause) {
		
		  // if the client may pause
		  extern INDEX ser_bClientsMayPause;
		  if (_cmiComm.Server_IsClientLocal(iClient) || ser_bClientsMayPause) {
		  CTString strPauser = _cmiComm.Server_GetClientName(iClient);
		  // change it
		  if (!bWantPause) {
          srv_bPause = FALSE;
          // add the pause state block to the buffer to be sent to all clients
          CNetworkStreamBlock nsbPause(MSG_SEQ_PAUSE, ++srv_iLastProcessedSequence);
          nsbPause<<(INDEX&)srv_bPause;
          nsbPause<<strPauser;
          // put the message in buffer
          srv_assoSessions[0].sso_nsBuffer.AddBlock(nsbPause);
          srv_bPauseValue = FALSE;
          srv_bSendPause = FALSE;
          srv_emEntityMessage.WritePause(srv_bPause,strPauser);
          SendMessage(srv_emEntityMessage);
		  
			} else {
			srv_bSendPause = TRUE;
			srv_bPauseValue = TRUE;
			srv_strPauser = strPauser;
			}
			}
			}
			} break;
			// if a player wants to change its buffer settings
			case MSG_SET_CLIENTSETTINGS: {
			// read data
			CSessionSocket &sso = srv_assoSessions[iClient];
			nmMessage>>sso.sso_sspParams;
			} break;
			// if a chat message was sent
			case MSG_CHAT_IN: {
			// get it
			ULONG ulFrom, ulTo;
			CTString strMessage;
			nmMessage>>ulFrom>>ulTo>>strMessage;
			
			  // filter the from address by the client's players
			  ulFrom &= MaskOfPlayersOnClient(iClient);
			  // if the source has no players
			  if (ulFrom==0) {
			  // make it public message
			  ulTo = -1;
			  }
			  
				// make the outgoing message
				CNetworkMessage nmOut(MSG_CHAT_OUT);
				nmOut<<ulFrom;
				if (ulFrom==0) {
				CTString strFrom;
				if (iClient==0) {
				strFrom = TRANS("Server");
				} else {
				strFrom.PrintF(TRANS("Client %d"), iClient);
				}
				nmOut<<strFrom;
				}
				nmOut<<strMessage;
				
				  // for each active client
				  for(INDEX iSession=0; iSession<srv_assoSessions.Count(); iSession++) {
				  CSessionSocket &sso = srv_assoSessions[iSession];
				  if (iSession>0 && !sso.IsActive()) {
				  continue;
				  }
				  // if message is public or the client has some of destination players
				  if (ulTo==-1 || ulTo&MaskOfPlayersOnClient(iSession)) {
				  // send the message to that computer
				  _pNetwork->SendToClient(iSession, nmOut);
				  }
				  }
				  
					} break;
					// if a crc response is received
  case MSG_REP_CRCCHECK: {*/
//0130 지우기
/*
CSessionSocket &sso = srv_assoSessions[iClient];
if (iClient!=0 && sso.sso_ubNumLevelChanges != _pNetwork->ga_ubNumLevelChanges) {
CTString strMessage;
strMessage.PrintF(TRANS("Server changed levels"));
SendDisconnectMessage(iClient, strMessage);
break;
}

  // get it
  ULONG ulCRC;
  //INDEX iLastSequence;
  nmMessage>>ulCRC;//>>iLastSequence;
  // if not same
  if (_pNetwork->ga_ulCRC!=ulCRC) {
  // disconnect the client
  SendDisconnectMessage(iClient, TRANS("Wrong CRC check."));
  // if same
  } else {
  CPrintF(TRANS("Server: Client '%s', CRC check OK\n"), 
  (const char*)_cmiComm.Server_GetClientName(iClient));
  // use the piggybacked sequence number to initiate sending stream to it
  sso.sso_bSendStream = TRUE;
  }
*/
/* //0522 kwon 삭제.
//0130 2line추가. 현재 이쪽으로 안들어옴.
CSessionSocket &sso = srv_assoSessions[iClient];
sso.sso_bSendStream = TRUE;

  } break;
  // if a rcon request is received
  case MSG_ADMIN_COMMAND: {
  extern CTString net_strAdminPassword;
  // get it
  CTString strPassword, strCommand;
  nmMessage>>strPassword>>strCommand;
  if (net_strAdminPassword=="") {
  CNetworkMessage nmRes(MSG_ADMIN_RESPONSE);
  nmRes<<CTString(TRANS("Remote administration not allowed on this server.\n"));
  CPrintF(TRANS("Server: Client '%s', Tried to use remote administration.\n"), 
  (const char*)_cmiComm.Server_GetClientName(iClient));
  _pNetwork->SendToClientReliable(iClient, nmRes);
  } else if (net_strAdminPassword!=strPassword) {
  CPrintF(TRANS("Server: Client '%s', Wrong password for remote administration.\n"), 
  (const char*)_cmiComm.Server_GetClientName(iClient));
  SendDisconnectMessage(iClient, TRANS("Wrong admin password. The attempt was logged."));
  break;
  } else {
  
	CPrintF(TRANS("Server: Client '%s', Admin cmd: %s\n"), 
	(const char*)_cmiComm.Server_GetClientName(iClient), strCommand);
	
      con_bCapture = TRUE;
      con_strCapture = "";
      _pShell->Execute(strCommand+";");
	  
		CTString strResponse = CTString(">")+strCommand+"\n"+con_strCapture;
		SendAdminResponse(iClient, strResponse);
		con_bCapture = FALSE;
		con_strCapture = "";
		}
		} break;
*/
/* //0311 삭제
// otherwise
case MSG_TICK_ACKNOWLEDGE: {
float tmTickTime;
nmMessage >> tmTickTime;
ReceiveAcknowledge(iClient,tmTickTime);
} break;
*/
  default:
	  ASSERT(FALSE);
  }
}





// put a message in the outgoing buffer
void CServer::SendMessage(CEntityMessage &emEntityMessage) 
{																					 
	// set the message time tag to current tick time
	emEntityMessage.em_tmMessageTime = _pNetwork->ga_sesSessionState.ses_tmLastProcessedTick;
	
	/*
	// if recording a demo, put the message in that buffer too
	if (_pNetwork->ga_bDemoRec) 
	{
    srv_embDemoBuffer.WriteMessage(emEntityMessage);
	}
	*/
	
	// if there are no clients connected to the server, do nothing  
	if (!srv_bRunning) return;
	
	// player actions are written to demo buffer only
	if (emEntityMessage.em_ulType != EMT_PLAYERACTION) {
		// finally, put the message in the buffer
		srv_embSendBuffer.WriteMessage(emEntityMessage);
	}
    
};



// begin writing a new tick in the output buffer
void CServer::StartNewTick(TIME tmTickTime)
{
  ASSERT (tmTickTime >= 0);
  int iErr;

  /*
  // if recording a demo, start a new tick in the demo buffer, and write data to disk
  if (_pNetwork->ga_bDemoRec) 
  {
    srv_embDemoBuffer.StartNewTick(tmTickTime);
    srv_embDemoBuffer.RequestTickAcknowledge(tmTickTime,1);
    WriteDemoData(tmTickTime-_pTimer->TickQuantum);
    srv_embDemoBuffer.ReceiveTickAcknowledge(tmTickTime-2*_pTimer->TickQuantum,tmTickTime-_pTimer->TickQuantum);
  }
  */

  if (!srv_bRunning) { return; }

  // get the index of last tick written to the buffer
  INDEX iLastTick = srv_embSendBuffer.emb_iCurrentTickMarker - 1;
  if (iLastTick < 0) iLastTick += MAX_TICKS_KEPT;

  // if time of this tick is earlier than the last written tick, just return
  if (tmTickTime < (srv_embSendBuffer.emb_atmTickMarkers[iLastTick].tm_tmTickTime + _pTimer->TickQuantum/2)) {
    return;
  }

  ULONG ulMaxPendingData = _cmiComm.Server_GetMaxPendingData();

  extern INDEX ser_iMaxBufferSize;
  // if the send buffer is getting too large, or some client is accumulating too much data in it's output buffers
  if ((ser_iMaxBufferSize<srv_embSendBuffer.GetUsedSize()) || (ulMaxPendingData>ser_iMaxBufferSize)) {
    if (ser_iMaxBufferSize<srv_embSendBuffer.GetUsedSize()) {
      CPrintF(TRANS("Kicking players because send buffer is too large.\n"));
    }
    if (ulMaxPendingData>ser_iMaxBufferSize) {
      CPrintF(TRANS("Kicking players because too much data is pending.\n"));
    }
    // make some free space by kicking some clients that have lagged to far behind
    DropLaggingClients();

  } 
//0115
// CPrintF(TRANS("srv_embSendBuffer.emb_iCurrentTickMarker : %d, iLastTick: %d\n ,tmTickTime: %5.2f\n"), srv_embSendBuffer.emb_iCurrentTickMarker, iLastTick,tmTickTime);
 
  // write a new tick marker
  iErr = srv_embSendBuffer.StartNewTick(tmTickTime);
  // if no more available tick markers in the buffer
  if (iErr == EMB_ERR_MAX_TICKS) {
//0206
/*
    CPrintF(TRANS("Kicking players because of too much tick markers.\n"));
    // make some free space by kicking some clients that have lagged to far behind
    DropLaggingClients();
    if (srv_bRunning) {
      // write a new tick marker
      srv_embSendBuffer.StartNewTick(tmTickTime);
    }
*/
    return;
  }
  
  return;
};


// receive acknowledge for some tick
void CServer::ReceiveAcknowledge(INDEX iClient,TIME tmTickTime)
{
  ASSERT (tmTickTime > -0.5);
  extern INDEX net_bReportServerTraffic;

  float fRandom = rand() / (float) RAND_MAX;
  // if the tick should not be kicked
  if (fRandom > ser_fAckDropRate) {
    // report to console  
    if (net_bReportServerTraffic) {
      CPrintF(TRANS("Acknowledge client : %d, time: %5.2f\n"), iClient, tmTickTime);
    }

    TIME tmLastAck = srv_assoSessions[iClient].sso_tmLastTickAcknowledge;

    // update the time of the tick last successfully sent to this client
    if (tmLastAck < (tmTickTime-0.01)) {
      // acknowledge the tick in the buffer
      srv_embSendBuffer.ReceiveTickAcknowledge(tmLastAck,tmTickTime);
      // report the oldest tick to the console
      if (net_bReportServerTraffic) {
        CPrintF(TRANS("New first tick time: %5.2f\n"),srv_embSendBuffer.emb_atmTickMarkers[srv_embSendBuffer.emb_iFirstTickMarker].tm_tmTickTime);
      }

      srv_assoSessions[iClient].sso_tmLastTickAcknowledge = tmTickTime;
    }
  }

};

/*
//! 활동하고 있는 모든 플레이어의 정보를 outgoing버퍼에 쓴다.
// write info on all active players to the outgoing buffer
BOOL CServer::AddPlayerUpdate(INDEX iClient,FLOAT3D &vBasePosition,BOOL bForDemoRec) 
{
  static CNetworkMessage stPlayerStream;
  BOOL bBasePositionSet = FALSE;
  stPlayerStream.Reinit();

  // for all active session sockets
  for (INDEX iPlayer=0;iPlayer<NET_MAXGAMEPLAYERS;iPlayer++) 
  {
    INDEX iSession = srv_apltPlayers[iPlayer].plt_iClient;
    if (iSession == -1) 
	{
      continue;
    }
    if (srv_assoSessions[iSession].IsActive()) 
	{
      // write it's player's data to the message and put it in the output buffer
      CPlayerEntity *penPlayerEntity = _pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity;
      stPlayerStream.Reinit();
      if (penPlayerEntity != NULL) 
	  {
		  //자신의 캐릭터는 풀정보를, 다른 캐릭터에 대한 것은 간략정보를 보낸다.
        // write full update only for the client we are sending this to, or for demo recording
		  if (iClient == iSession || bForDemoRec) 
		  {
			  if (!bBasePositionSet) 
			  {
				  bBasePositionSet = TRUE;
				  vBasePosition = penPlayerEntity->en_plPlacement.pl_PositionVector;
			  }
			  //! 플레이어의 정보를 쓴다.
			  penPlayerEntity->Write_net(&stPlayerStream);
		  } 
		  else 
		  {
			  penPlayerEntity->Write_net_brief(&stPlayerStream);
		  }
		//! EntityMessage로 정보를 옮긴다.
        srv_emEntityMessage.WritePlayerUpdate(penPlayerEntity->en_ulID,stPlayerStream);
        srv_emEntityMessage.em_tmMessageTime = _pNetwork->ga_sesSessionState.ses_tmLastProcessedTick - _pTimer->TickQuantum;
        //! Outgoing버퍼에 메시지를 쓴다.
        // if there is not enough free space, cancel this job
        if (!srv_embOutgoingBuffer.WriteMessage(srv_emEntityMessage,FALSE)) {
          return FALSE;
        };

      }
    }
  }
  
  if (!bBasePositionSet) {
    vBasePosition = FLOAT3D(0,0,0);
    bBasePositionSet = TRUE;
  }

  // write base placement message
  //! 기본위치를 EntityMessage에 쓴다.
  srv_emEntityMessage.WriteBasePosition(vBasePosition);
  srv_emEntityMessage.em_tmMessageTime = _pNetwork->ga_sesSessionState.ses_tmLastProcessedTick - _pTimer->TickQuantum;
  //! 또 Outgoing에 옮겨 쓴다.
  // if there is not enough free space, cancel this job
  if (!srv_embOutgoingBuffer.WriteMessage(srv_emEntityMessage,FALSE)) {
    return FALSE;
  };

  return TRUE;
};

//! 클라이언트로 보낼 tick데이타를 준비한다.
// prepare tick data to be sent to the client
BOOL CServer::PrepareClientData(INDEX iClient)
{
  TIME tmStartTime; // time of the earliest tick to be sent
  INDEX iStartIndex; // index of tick marker for this time

  //! 이 클라이언트에 마지막 TICK을 보낸 시간.
  TIME tmLastTickTime = srv_assoSessions[iClient].sso_tmLastTickAcknowledge; // time of the last tick sent to this client
  INDEX ctTick,iLastTick;       // number of ticks to process 
  INDEX ctPlaced,ctDestroyed;   // number of placement and destroy messages
  INDEX iTotalMsgs;             // total number of messages to process
  SLONG slStartOffset,slEndOffset,slCurrOffset;   
  INDEX iMsgs,iNumTicks;
  CStaticArray<SLONG> saMessageOffsets;     // array of message offsets
  CStaticArray<ULONG> saPlacedEntities;     // array of entites for which setplacement meessages were processed
  CStaticArray<ULONG> saDestroyedEntities;  // array of destroyed entities IDs

  // clear outgoing buffer and initialize it
  srv_embOutgoingBuffer.Clear();
  srv_embOutgoingBuffer.StartNewTick(0.0f);
  FLOAT3D vBasePosition;

  //! 모든 플레이어 정보를 outgoing버퍼에 넣는다.
  if (!AddPlayerUpdate(iClient,vBasePosition)) {
    return FALSE;
  }
  
  // get the index of the last tick in the buffer  
  iLastTick = srv_embSendBuffer.emb_iCurrentTickMarker - 1;
  if (iLastTick < 0) iLastTick += MAX_TICKS_KEPT;
             
  // get the time of the first tick not acknowledged by this client
  int iErr = srv_embSendBuffer.GetNextTickTime(tmLastTickTime,tmStartTime);
  // if there was an error (there is no such time in the buffer)
  if (iErr != EMB_SUCCESS_OK) {
    // send player updates only
    srv_embOutgoingBuffer.StartNewTick(_pTimer->TickQuantum);
    return TRUE;
  }
  
  // get the index in the buffer for the first tick marker
  iErr = srv_embSendBuffer.GetTickIndex(tmStartTime,iStartIndex); 

  // if first and last ticks are the same
  if (iStartIndex == iLastTick || iErr == EMB_ERR_NOT_IN_BUFFER) {
    // send player updates only
    srv_embOutgoingBuffer.StartNewTick(_pTimer->TickQuantum);
    return TRUE;
  }
  ASSERT (iErr == EMB_SUCCESS_OK); 

  // count total number of ticks and messages to process
  iTotalMsgs = 0;
  iNumTicks = 0;
  ctTick=iStartIndex;
  while (ctTick != iLastTick) {
    iTotalMsgs += srv_embSendBuffer.emb_atmTickMarkers[ctTick].tm_uwNumMessages;
    ctTick++;
    iNumTicks++;
    ctTick %= MAX_TICKS_KEPT;
  }
  
  // if no messages - all ticks are empty
  if (iTotalMsgs == 0) {
    // send player updates only
    srv_embOutgoingBuffer.StartNewTick(_pTimer->TickQuantum);
    return TRUE;
  }

  // get byte offsets in the buffer for first and last ticks
  srv_embSendBuffer.GetTickOffset(tmStartTime,slStartOffset);
  srv_embSendBuffer.GetTickOffset(srv_embSendBuffer.emb_atmTickMarkers[iLastTick].tm_tmTickTime,slEndOffset);

  // expand the arrays to contain all messages
  saMessageOffsets.Expand(iTotalMsgs);  
  saDestroyedEntities.Expand(iTotalMsgs);

  //! 이제 tick버퍼로부터 모든 메시지를 얻는다. 그리고 outgoing버퍼로 복사한다.
  //! SendPlacementNotify 메시지는 스킵한다. 모든 파괴된 엔티티를 열거하고, 
  //! 모든 위치 메시지에 대하여 오프셋을 표시한다.
  // now get all messages from the tick buffer and copy them to the outgoing buffer,
  // skip the SendPlacementNotify messages
  // in the process, enumerate all destroyed entities and mark offsets for all placement messages
  iMsgs=0;
  ctDestroyed=0;
  slCurrOffset = slStartOffset;
  while (slCurrOffset != slEndOffset) {    
    SLONG slOldOffset = slCurrOffset;
	//! srv_embSendBuffer에서 srv_emEntityMessage로 가져온다.
    srv_embSendBuffer.PeekMessageAtOffset(srv_emEntityMessage,slCurrOffset);
	//! 이 메시지가 위치 메시지가 아니라면, output 버퍼에 넣는다.
    // if it's not a placement message send, put it in the output buffer
    if (srv_emEntityMessage.em_ulType != EMT_SETPLACEMENT_NOTIFY) {        
      // if there is not enough free space, cancel this job
      if (!srv_embOutgoingBuffer.WriteMessage(srv_emEntityMessage,FALSE)) {
        return FALSE;
      };      
	  //! 만약 파괴 메시지라면, 그 엔티티에 대한 위치 메시지는 보내지 않는다.
      // if it's a destroy message, mark that entity as destroyed - we won't send placements for it
      if (srv_emEntityMessage.em_ulType == EMT_DESTROY) {
        saDestroyedEntities[ctDestroyed] = srv_emEntityMessage.em_ulEntityID;
        ctDestroyed++;  
      }
    } else {
	  // 위치 메시지라면 그 오프셋을 표시한다.
      // for placement notification messages, just mark it's offset in the message buffer
      saMessageOffsets[iMsgs] = slOldOffset;
      iMsgs++;
    }
  }

  // go through the placement notification messages, starting from the last one
  ctPlaced = 0;
  if (iMsgs > 0) {
    saPlacedEntities.Expand(iMsgs);
    for (INDEX iPlacements=iMsgs-1;iPlacements>=0;iPlacements--) {
	  //! 위치메시지.
      srv_embSendBuffer.PeekMessageAtOffset(srv_emEntityMessage,saMessageOffsets[iPlacements]);
      ASSERT (srv_emEntityMessage.em_ulType == EMT_SETPLACEMENT_NOTIFY);
      BOOL bFound = FALSE;
	  //만약에 이 엔티티에 대한 파괴 메시지가 있다면, skip.
      // if there was a destroy message for this entity, skip it
      for (int iEnt=0;iEnt<ctDestroyed;iEnt++) {
        if (saDestroyedEntities[iEnt] == srv_emEntityMessage.em_ulEntityID) {
          bFound = TRUE;
          break;
        }
      }
      if (!bFound) {
		//만약 이 엔티티에 대하여 더 최근의 위치메시지가 있다면, skip
        // if there was a more recent setplacement for this entity (we are going through the buffer
        // from end to begining), skip this message
        for (int iEnt=0;iEnt<ctPlaced;iEnt++) {
          if (saPlacedEntities[iEnt] == srv_emEntityMessage.em_ulEntityID) {
            bFound = TRUE;
            break;
          }
        }
        // else, mark the entity as placed, check if an update needs to be sent and send it
        if (!bFound) {
          saPlacedEntities[ctPlaced] = srv_emEntityMessage.em_ulEntityID;
          ctPlaced++;
          UBYTE ubForceUpdate;
          ULONG ulEntityID;
          srv_emEntityMessage.ReadPlacementNotify(ulEntityID,ubForceUpdate);

          // if an update is forced (example: when an entity stops moving)
          if (ubForceUpdate) {
            _pNetwork->ga_World.wo_ehEntityHashContainer.UpdateClient(iClient,srv_emEntityMessage.em_ulEntityID,srv_emEntityMessage,vBasePosition);
            srv_emEntityMessage.em_tmMessageTime = _pNetwork->ga_sesSessionState.ses_tmLastProcessedTick;
            // if there is not enough free space, cancel this job
            if (!srv_embOutgoingBuffer.WriteMessage(srv_emEntityMessage,FALSE)) {
              return FALSE;
            };      
          // if not forced and the dead reckoning error is larger than the allowed limit for that client, send a placement update
          } else if (_pNetwork->ga_World.wo_ehEntityHashContainer.ClientNeedsUpdate(iClient,_pTimer->CurrentTick(),srv_emEntityMessage.em_ulEntityID,srv_emEntityMessage,vBasePosition)) {
            srv_emEntityMessage.em_tmMessageTime = _pNetwork->ga_sesSessionState.ses_tmLastProcessedTick;
            // if there is not enough free space, cancel this job
            if (!srv_embOutgoingBuffer.WriteMessage(srv_emEntityMessage,FALSE)) {
              return FALSE;
            };      
          }
          
        }
      }
    }
  }
  
  // add player info to the buffer and return
  srv_embOutgoingBuffer.StartNewTick(_pTimer->TickQuantum);

  return TRUE;

};
*/


//! 새로운 tick데이타를 클라이언트에 보낸다.
// send new tick data to the client
void CServer::SendClientData(INDEX iClient)
{
  ASSERT (iClient >= 0 && iClient < SERVER_CLIENTS);
  ASSERT (srv_assoSessions[iClient].IsActive());
/* //0522 kwon 삭제.
  // if there are no ticks in the buffer, return
  if (srv_embSendBuffer.emb_uwNumTickMarkers <= 1) { 
    CNetworkMessage nmKeepalive(MSG_KEEPALIVE);
    _pNetwork->SendToClient(iClient, nmKeepalive);
    return;
  }
*/
/* //0522 kwon 삭제.
  CNetworkMessage nmMessage(MSG_TICK_DATA);
  SLONG slSize;

  // index of the last tick that will be sent to the client
  int iLastTick = srv_embSendBuffer.emb_iCurrentTickMarker - 2;
  if (iLastTick < 0) iLastTick += MAX_TICKS_KEPT;

  //! outgoing버퍼에 클라이언트로 보낼 데이타를 넣는다.
  // prepare the outgoing buffer for the client
  if (!PrepareClientData(iClient)) {
    return;
  }
  //! 데이타가 너무 크면 보내지 말아야한다.
  // if the size of prepared data is too large, don't send it
  if (srv_embOutgoingBuffer.GetUsedSize() > SERVER_MAX_MESSAGE_SIZE) {
    return;
  }

  // read the size of tick data and copy it into a local buffer
  slSize = srv_embOutgoingBuffer.GetUsedSize();
  ASSERT (srv_embOutgoingBuffer.bu_slReadOffset == 0);
   
  extern INDEX net_bReportServerTraffic;
  // report to console
//0108 //! 0.05초 즉, 1 Tick에 한번씩 캐릭터 정보를 보낸다.1 Tick에 2번, 2Tick에 1번 보낼때도 있군....
  if (net_bReportServerTraffic) {
    CPrintF(TRANS("SEND Client: %d, Time: %5.2f, Size: %ld\n"),iClient,srv_embSendBuffer.emb_atmTickMarkers[iLastTick].tm_tmTickTime,slSize);
	CPrintF(TRANS("iLastTick: %d, srv_embSendBuffer.emb_iCurrentTickMarker: %d\n"),iLastTick,srv_embSendBuffer.emb_iCurrentTickMarker);
  }

  // get the real time (must be a double because we need a good precision for large numbers - 0.001 precission, number order 10^4)
  double tmNow = _pTimer->GetHighPrecisionTimer().GetSeconds();
  // output the time of last tick in the message, real time and size of tick data
  nmMessage << srv_embSendBuffer.emb_atmTickMarkers[iLastTick].tm_tmTickTime; 
  nmMessage << tmNow;

  //! Tick 데이타 압축후에 클라이언트로 보낸다.
  // compress tick data
  CzlibCompressor comp;
  SLONG slCompressedSize = comp.NeededDestinationSize(slSize);  
  BOOL bOK = comp.Pack(srv_embOutgoingBuffer.bu_pubBuffer,slSize,_aubCompData,slCompressedSize);
  ASSERT(bOK);
  nmMessage << slCompressedSize;
  nmMessage.Write(_aubCompData,slCompressedSize);
  //CPrintF("SERVER: %6.2f: compressed tick data: (%ld)->(%ld) - 1:%5.3f\n",_pTimer->CurrentTick(),slSize,slCompressedSize,(FLOAT)slSize/slCompressedSize);

  // write tick data to the message and send it to client
  _pNetwork->SendToClient(iClient,nmMessage);

  // calculate the next available time slot
  CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
  TIME tmTransmitDone = tvNow.GetSeconds() + (float) slSize/srv_assoSessions[iClient].sso_sspParams.ssp_iMaxBPS;
  srv_assoSessions[iClient].sso_tmNextAvailableTimeSlot = tmTransmitDone;
  //CPrintF("NextTimeSlot: %7.4f   Delta: %7.4f\n",tmTransmitDone,(float) slSize/srv_assoSessions[iClient].sso_sspParams.ssp_iMaxBPS);

*/
  return;

};


/*
BOOL CServer::PrepareDemoData(TIME tmTickTime)
{
  TIME tmStartTime; // time of the earliest tick to be sent
  INDEX iStartIndex; // index of tick marker for this time

  TIME tmLastTickTime = tmTickTime-_pTimer->TickQuantum; // time of the last tick sent to this client
  INDEX ctTick,iLastTick;       // number of ticks to process 
  INDEX ctPlaced,ctDestroyed;   // number of placement and destroy messages
  INDEX iTotalMsgs;             // total number of messages to process
  SLONG slStartOffset,slEndOffset,slCurrOffset;   
  INDEX iMsgs,iNumTicks;
  CStaticArray<SLONG> saMessageOffsets;     // array of message offsets
  CStaticArray<ULONG> saPlacedEntities;     // array of entites for which setplacement meessages were processed
  CStaticArray<ULONG> saDestroyedEntities;  // array of destroyed entities IDs

  // clear outgoing buffer and initialize it
  srv_embOutgoingBuffer.Clear();
  srv_embOutgoingBuffer.StartNewTick(0.0f);
  FLOAT3D vBasePosition = FLOAT3D(0.0f,0.0f,0.0f);
  
  // get the index of the last tick in the buffer  
  iLastTick = srv_embDemoBuffer.emb_iCurrentTickMarker - 1;
  if (iLastTick < 0) iLastTick += MAX_TICKS_KEPT;
             
  // get the time of the first tick not acknowledged by this client
  int iErr = srv_embDemoBuffer.GetNextTickTime(tmLastTickTime,tmStartTime);
  // if there was an error (there is no such time in the buffer)
  if (iErr != EMB_SUCCESS_OK) {
    // send player updates only
    if (!AddPlayerUpdate(DEMO_CLIENT,vBasePosition,TRUE)) {
      return FALSE;
    }
    srv_embOutgoingBuffer.StartNewTick(_pTimer->TickQuantum);
    return TRUE;
  }
  
  // get the index in the buffer for the first tick marker
  iErr = srv_embDemoBuffer.GetTickIndex(tmStartTime,iStartIndex); 

  // if first and last ticks are the same
  if (iStartIndex == iLastTick || iErr == EMB_ERR_NOT_IN_BUFFER) {
    // send player updates only
    if (!AddPlayerUpdate(DEMO_CLIENT,vBasePosition,TRUE)) {
      return FALSE;
    }
    srv_embOutgoingBuffer.StartNewTick(_pTimer->TickQuantum);
    return TRUE;
  }
  ASSERT (iErr == EMB_SUCCESS_OK); 

  // count total number of ticks and messages to process
  iTotalMsgs = 0;
  iNumTicks = 0;
  ctTick=iStartIndex;
  while (ctTick != iLastTick) {
    iTotalMsgs += srv_embDemoBuffer.emb_atmTickMarkers[ctTick].tm_uwNumMessages;
    ctTick++;
    iNumTicks++;
    ctTick %= MAX_TICKS_KEPT;
  }
  
  // if no messages - all ticks are empty
  if (iTotalMsgs == 0) {
    // send player updates only
    srv_embOutgoingBuffer.StartNewTick(_pTimer->TickQuantum);
    if (!AddPlayerUpdate(DEMO_CLIENT,vBasePosition,TRUE)) {
      return FALSE;
    }
    return TRUE;
  }

  // get byte offsets in the buffer for first and last ticks
  srv_embDemoBuffer.GetTickOffset(tmStartTime,slStartOffset);
  srv_embDemoBuffer.GetTickOffset(srv_embDemoBuffer.emb_atmTickMarkers[iLastTick].tm_tmTickTime,slEndOffset);

  // expand the arrays to contain all messages
  saMessageOffsets.Expand(iTotalMsgs);  
  saDestroyedEntities.Expand(iTotalMsgs);

  // now get all messages from the tick buffer and copy them to the outgoing buffer,
  // skip the SendPlacementNotify messages
  // in the process, enumerate all destroyed entities and mark offsets for all placement messages
  iMsgs=0;
  ctDestroyed=0;
  slCurrOffset = slStartOffset;
  while (slCurrOffset != slEndOffset) {    
    SLONG slOldOffset = slCurrOffset;
    srv_embDemoBuffer.PeekMessageAtOffset(srv_emEntityMessage,slCurrOffset);
    // if it's not a placement message send, put it in the output buffer
    if (srv_emEntityMessage.em_ulType != EMT_SETPLACEMENT_NOTIFY) {        
      // if there is not enough free space, cancel this job
      if (!srv_embOutgoingBuffer.WriteMessage(srv_emEntityMessage,FALSE)) {
        return FALSE;
      };      
      // if it's a destroy message, mark that entity as destroyed - we won't send placements for it
      if (srv_emEntityMessage.em_ulType == EMT_DESTROY) {
        saDestroyedEntities[ctDestroyed] = srv_emEntityMessage.em_ulEntityID;
        ctDestroyed++;  
      }
    } else {
      // for placement notification messages, just mark it's offset in the message buffer
      saMessageOffsets[iMsgs] = slOldOffset;
      iMsgs++;
    }
  }

  // go through the placement notification messages, starting from the last one
  ctPlaced = 0;
  if (iMsgs > 0) {
    saPlacedEntities.Expand(iMsgs);
    for (INDEX iPlacements=iMsgs-1;iPlacements>=0;iPlacements--) {
      srv_embDemoBuffer.PeekMessageAtOffset(srv_emEntityMessage,saMessageOffsets[iPlacements]);
      ASSERT (srv_emEntityMessage.em_ulType == EMT_SETPLACEMENT_NOTIFY);
      BOOL bFound = FALSE;
      // if there was a destroy message for this entity, skip it
      for (int iEnt=0;iEnt<ctDestroyed;iEnt++) {
        if (saDestroyedEntities[iEnt] == srv_emEntityMessage.em_ulEntityID) {
          bFound = TRUE;
          break;
        }
      }
      if (!bFound) {
        // if there was a more recent setplacement for this entity (we are going through the buffer
        // from end to begining), skip this message
        for (int iEnt=0;iEnt<ctPlaced;iEnt++) {
          if (saPlacedEntities[iEnt] == srv_emEntityMessage.em_ulEntityID) {
            bFound = TRUE;
            break;
          }
        }
        // else, mark the entity as placed, check if an update needs to be sent and send it
        if (!bFound) {
          saPlacedEntities[ctPlaced] = srv_emEntityMessage.em_ulEntityID;
          ctPlaced++;
          UBYTE ubForceUpdate;
          ULONG ulEntityID;
          srv_emEntityMessage.ReadPlacementNotify(ulEntityID,ubForceUpdate);

          // if an update is forced (example: when an entity stops moving)
          if (ubForceUpdate) {
            _pNetwork->ga_World.wo_ehEntityHashContainer.UpdateClient(DEMO_CLIENT,srv_emEntityMessage.em_ulEntityID,srv_emEntityMessage,vBasePosition);
            srv_emEntityMessage.em_tmMessageTime = _pNetwork->ga_sesSessionState.ses_tmLastProcessedTick;
            // if there is not enough free space, cancel this job
            if (!srv_embOutgoingBuffer.WriteMessage(srv_emEntityMessage,FALSE)) {
              return FALSE;
            };      
          // if not forced and the dead reckoning error is larger than the allowed limit for that client, send a placement update
          } else if (_pNetwork->ga_World.wo_ehEntityHashContainer.ClientNeedsUpdate(DEMO_CLIENT,_pTimer->CurrentTick(),srv_emEntityMessage.em_ulEntityID,srv_emEntityMessage,vBasePosition)) {
            srv_emEntityMessage.em_tmMessageTime = _pNetwork->ga_sesSessionState.ses_tmLastProcessedTick;
            // if there is not enough free space, cancel this job
            if (!srv_embOutgoingBuffer.WriteMessage(srv_emEntityMessage,FALSE)) {
              return FALSE;
            }
          }          
        }
      }
    }
  }

  if (!AddPlayerUpdate(DEMO_CLIENT,vBasePosition,TRUE)) {
    return FALSE;
  }
  
  srv_embOutgoingBuffer.StartNewTick(_pTimer->TickQuantum);

  return TRUE;

};

void CServer::WriteDemoData(TIME tmTickTime) 
{
 //0522 kwon 삭제.
  if (srv_tmNextAvailableDemoTimeSlot > _pTimer->GetHighPrecisionTimer().GetSeconds()) {
    return;
  }

  ASSERT(_pNetwork->ga_bDemoRec);
  if (srv_embDemoBuffer.emb_uwNumTickMarkers <= 1) { 
    return;
  }

  CNetworkMessage nmMessage(MSG_TICK_DATA);
  SLONG slSize;

  // index of the last tick that will be sent to the client
  int iLastTick = srv_embDemoBuffer.emb_iCurrentTickMarker - 2;
  if (iLastTick < 0) iLastTick += MAX_TICKS_KEPT;

  // prepare the outgoing buffer for the client
  if (!PrepareDemoData(tmTickTime)) {
    return;
  }

  // read the size of tick data and copy it into a local buffer
  slSize = srv_embOutgoingBuffer.GetUsedSize();
  ASSERT (srv_embOutgoingBuffer.bu_slReadOffset == 0);
   
  extern INDEX net_bReportDemoTraffic;
  // report to console
  if (net_bReportDemoTraffic) {
    CPrintF(TRANS("DEMO tick prepared: Time: %5.2f, Size: %ld\n"),srv_embDemoBuffer.emb_atmTickMarkers[iLastTick].tm_tmTickTime,slSize);
  }

  // get the real time (must be a double because we need a good precision for large numbers - 0.001 precission, number order 10^4)
  double tmNow = _pTimer->GetHighPrecisionTimer().GetSeconds();
  // output the time of last tick in the message, real time and size of tick data
  //nmMessage << srv_embDemoBuffer.emb_atmTickMarkers[iLastTick].tm_tmTickTime; 
  //nmMessage << tmNow;

  // compress tick data
  CzlibCompressor comp;
  SLONG slCompressedSize = comp.NeededDestinationSize(slSize);  
  BOOL bOK = comp.Pack(srv_embOutgoingBuffer.bu_pubBuffer,slSize,_aubCompData,slCompressedSize);
  ASSERT(bOK);
  //CPrintF("SERVER: %6.2f: compressed tick data: (%ld)->(%ld) - 1:%5.3f\n",_pTimer->CurrentTick(),slSize,slCompressedSize,(FLOAT)slSize/slCompressedSize);

  // write tick data to the disk
  _pNetwork->ga_strmDemoRec.WriteID_t("DTTM");
  _pNetwork->ga_strmDemoRec << _pNetwork->ga_sesSessionState.ses_tmLastProcessedTick-_pTimer->TickQuantum; // srv_embDemoBuffer.emb_atmTickMarkers[iLastTick].tm_tmTickTime;
  
  _pNetwork->ga_strmDemoRec.WriteID_t("DTCK");
  _pNetwork->ga_strmDemoRec << slCompressedSize;
  _pNetwork->ga_strmDemoRec.Write_t(_aubCompData,slCompressedSize);

  extern INDEX net_bReportDemoTraffic;
  // report to console
  if (net_bReportDemoTraffic) {
    CPrintF("Recorded tick time: %6.3f\n",srv_embDemoBuffer.emb_atmTickMarkers[iLastTick].tm_tmTickTime);
  }

  extern INDEX net_iDemoQuality;
  net_iDemoQuality = Clamp(net_iDemoQuality,(INDEX)32,(INDEX)512);
  // kbps to Bps
  ULONG ulBPS = net_iDemoQuality*8*1024;
  // calculate the next available time slot
  CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
  TIME tmTransmitDone = tvNow.GetSeconds() + (float) slSize/ulBPS;
  srv_tmNextAvailableDemoTimeSlot = tmTransmitDone;
  return;

};
*/


// drop a client's connection
void CServer::CloseClient(INDEX iClient)
{ 
  // this can happen in rare cases when a client loses connection while connecting
  if (!srv_assoSessions[iClient].IsActive()) {return;}

  TIME tmLastAcknowledge;
  
  // get the index of the last tick in the buffer  
  INDEX iLastTick = srv_embSendBuffer.emb_iCurrentTickMarker - 1;
  if (iLastTick < 0) iLastTick += MAX_TICKS_KEPT;
  TIME tmLastTick = srv_embSendBuffer.emb_atmTickMarkers[iLastTick].tm_tmTickTime;

  // get the time of the last tick acknowledged by this client
  tmLastAcknowledge = srv_assoSessions[iClient].sso_tmLastTickAcknowledge;  
  if (tmLastAcknowledge <= -1) {
    tmLastAcknowledge = srv_embSendBuffer.emb_atmTickMarkers[srv_embSendBuffer.emb_iFirstTickMarker].tm_tmTickTime;
  } 
  
  // and acknowledge them
  srv_embSendBuffer.ReceiveTickAcknowledge(tmLastAcknowledge,tmLastTick);
  srv_assoSessions[iClient].sso_tmLastTickAcknowledge = -2;

  return; 
};




void CServer::DropLaggingClients()
{
  int iSession;
  float tmMinTime,tmNextTime;
  extern INDEX net_bReportServerTraffic;

  tmMinTime = (TIME) 1E32;

  // find the earliest acknowledged tick time
  for (iSession=1;iSession<SERVER_CLIENTS;iSession++) {
    if (srv_assoSessions[iSession].IsActive() && 
        (srv_assoSessions[iSession].sso_tmLastTickAcknowledge < tmMinTime)) {
      tmMinTime = srv_assoSessions[iSession].sso_tmLastTickAcknowledge;
    }
  }

  ASSERT(tmMinTime < 1E31);
                      
  tmNextTime = (float) 1E32;

  // now find the first greater acknowledged time  
  for (iSession=1;iSession<SERVER_CLIENTS;iSession++) {
    if (srv_assoSessions[iSession].IsActive() && 
        (srv_assoSessions[iSession].sso_tmLastTickAcknowledge > tmMinTime) && 
        (srv_assoSessions[iSession].sso_tmLastTickAcknowledge < tmNextTime)) {
      tmNextTime = srv_assoSessions[iSession].sso_tmLastTickAcknowledge;
    }
  }
 
  // if no such time found, drop all clients
  if (tmNextTime > 1E31) {
    // for all remote sessions
    for (iSession=1;iSession<SERVER_CLIENTS;iSession++) {
      // if this session is active, drop it
      if (srv_assoSessions[iSession].IsActive()) {
        // disconnect the client
        HandleClientDisconected(iSession);

        // report to console
        if (net_bReportServerTraffic) {
          CPrintF(TRANS("Unable to deliver game update to client '%s', disconnecting.\n"),AddressToString(cm_aciClients[iSession].ci_adrAddress.adr_ulAddress));
        }
        // clear communication data
        _cmiComm.Server_ClearClient(iSession);
      }
    }
    // report to console
    if (net_bReportServerTraffic) {
      CPrintF(TRANS("\nAll remote clients dropped!!!\n\n"));
    }
    StopNetProcess();
    return;
  }

  // remove clients that have lagged beyond tmNextTime
  srv_embSendBuffer.MoveToStartOfTick(tmNextTime);
  for (iSession=1;iSession<SERVER_CLIENTS;iSession++) {
    if (srv_assoSessions[iSession].IsActive() && 
        (srv_assoSessions[iSession].sso_tmLastTickAcknowledge < tmNextTime) && 
#pragma message(">> >> >> ??? fabs(srv_assoSessions[iSession].sso_tmLastTickAcknowledge < tmNextTime)")
		/* ocarina */
		//(fabs(srv_assoSessions[iSession].sso_tmLastTickAcknowledge < tmNextTime) > _pTimer->TickQuantum)) {
        (fabs(srv_assoSessions[iSession].sso_tmLastTickAcknowledge) > _pTimer->TickQuantum)) {
      // perform local disconnect
      HandleClientDisconected(iSession);
      // report to console
      if (net_bReportServerTraffic) {
        CPrintF(TRANS("Unable to deliver game update to client '%s', disconnecting.\n"),AddressToString(cm_aciClients[iSession].ci_adrAddress.adr_ulAddress));
      }
      // clear communication data
      _cmiComm.Server_ClearClient(iSession);
    }
  }


};


// dump server stats to file
void CServer::DumpStats() 
{ 
};


// start the client interface
void CServer::InitServerNetProcess() 
{
  extern INDEX net_bReportServerTraffic;
  if (srv_bRunning) { return; }
  // turn server process on
  srv_bRunning = TRUE;
  // clear tick buffer
  srv_embSendBuffer.Clear();
  // start writing a new tick
  StartNewTick(_pNetwork->ga_sesSessionState.ses_tmLastProcessedTick); 
  // report to console
  if (net_bReportServerTraffic) {
    CPrintF(TRANS("Initialize time: %5.2f\n"), _pNetwork->ga_sesSessionState.ses_tmLastProcessedTick);
  }
  // add a reference to the first tick in the buffer
  srv_embSendBuffer.RequestTickAcknowledge(_pNetwork->ga_sesSessionState.ses_tmLastProcessedTick,1);  
};


// get the count of active remote sessions
INDEX CServer::ActiveRemoteSessions() 
{
  INDEX ctNumActive=0;

  // if a session is active and sending of game data is enabled for it (client has completed
  // connection), count it in
  for (INDEX iSession=1;iSession < srv_assoSessions.Count();iSession++) {
    if (srv_assoSessions[iSession].IsActive() && srv_assoSessions[iSession].sso_bSendStream) {
      ctNumActive++;
    }
  }

  return ctNumActive;
};


// stop server net process
void CServer::StopNetProcess()
{
  ASSERT (srv_bRunning || ActiveRemoteSessions() == 0);
  // if the process isn't running, return
  if (srv_bRunning == FALSE) return;

  // clear server buffer
  srv_embSendBuffer.Clear();

  // mark that server tick process is stopped
  srv_bRunning = FALSE;
};

//! 모든 클라이언트에 대하여 업데이트를 한다.
// run updates for all clients
void CServer::ServerNetProcess()
{
  // if no clients connected, return
  if (!srv_bRunning) { return;}

  // this part is used to skip a certain number of updates, as set by the ser_iSkipTicks console variable
  static int iTick = 0;
  iTick++;
  extern INDEX ser_iSkipTicks;
  if (ser_iSkipTicks<0) ser_iSkipTicks=0;
  if ((iTick % (ser_iSkipTicks+1)) !=0) {
    return;
  };

  // string for debug output
  extern CTString _strDebug;
  extern PIX _pixDebugStringX, _pixDebugStringY;
  extern COLOR _colDebugString;
  
  extern INDEX ser_iMaxPendingData;
  extern INDEX ser_iMaxTotalData;
  
  if (ser_iMaxPendingData < _cmiComm.Server_GetMaxPendingData()) {
    ser_iMaxPendingData = _cmiComm.Server_GetMaxPendingData();
  }
  static ULONG ulMaxTotalData = 0;
  if (ser_iMaxTotalData < _cmiComm.Server_GetTotalPendingData()) {
    ser_iMaxTotalData = _cmiComm.Server_GetTotalPendingData();
  }

  // for each active client
  for (INDEX iSession=1;iSession<srv_assoSessions.Count();iSession++) {
    if (srv_assoSessions[iSession].IsActive() && srv_assoSessions[iSession].sso_bSendStream) {
      // if this client is ready and is available for send in this tick
      if (srv_assoSessions[iSession].sso_tmLastTickAcknowledge > -1.5 && srv_assoSessions[iSession].sso_tmNextAvailableTimeSlot <= _pTimer->GetHighPrecisionTimer().GetSeconds()) {
		//! 새로운 tick데이타를 클라이언트에 보낸다.
        SendClientData(iSession);
      }
    }
  }

};

