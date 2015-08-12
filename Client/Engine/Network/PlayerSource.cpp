#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Network/PlayerSource.h>
#include <Engine/Network/PlayerTarget.h>
#include <Engine/Network/CommunicationInterface.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Math/Float.h>
#include <Engine/Network/MessageDefine.h>

extern FLOAT net_tmConnectionTimeout;
extern FLOAT net_tmProblemsTimeOut;

FLOAT _fLatency = 0;
/*
 *  Constructor.
 */
CPlayerSource::CPlayerSource(void) {
  pls_Active = FALSE;
  pls_Index = -2;
  pls_csAction.cs_iIndex = -1;
  // clear action packet
  pls_paAction.Clear();
}

/*
 *  Destructor.
 */
CPlayerSource::~CPlayerSource(void) {
}

/*
 * Activate a new player.
 */
void CPlayerSource::Start_t(CPlayerCharacter &pcCharacter) // throw char *
{
  ASSERT(!pls_Active);

  // set index to -1 what means that you are not yet registered properly
  pls_Index = -1;
  // copy the character data
  pls_pcCharacter = pcCharacter;
  // clear actions
  pls_paAction.Clear();;
  for(INDEX ipa=0; ipa<PLS_MAXLASTACTIONS; ipa++) {
    pls_apaLastActions[ipa].Clear();
  }

  // request player connection
  CNetworkMessage nmRegisterPlayer((UBYTE)MSG_REQ_CONNECTPLAYER);
  nmRegisterPlayer<<pls_pcCharacter;    // player's character data
  _pNetwork->SendToServerReliable(nmRegisterPlayer);

  TIME tmTimeout = net_tmConnectionTimeout;
  if (_pNetwork->IsServer()) {
    tmTimeout = 5.0f;
  }

  for(TIME tmWait=0; 
      tmWait<tmTimeout*1000; 
      Sleep(NET_WAITMESSAGE_DELAY), tmWait+=NET_WAITMESSAGE_DELAY) {


    if (_pNetwork->ga_IsServer) {
      // handle server messages
      _cmiComm.Server_Update();
      if (_pNetwork->ga_srvServer.srv_bActive) {
        // handle all incoming messages
        _pNetwork->ga_srvServer.HandleAll();
      }
      _pNetwork->ga_srvServer.ServerLoop();
      _cmiComm.Server_Update();
    }

    if (_cmiComm.Client_Update() == FALSE) {
			break;
		}
    // wait for message to come
    CNetworkMessage nmReceived;
    if (!_pNetwork->ReceiveFromServerReliable(nmReceived)) {
      continue;
    }

    // if this is the init message
    if (nmReceived.GetType() == MSG_REP_CONNECTPLAYER) {
      // get the number of server level changes
      UBYTE ubNumLevelChanges;
      nmReceived>>ubNumLevelChanges;
      // if it doesn't match the locally stored number
      if (_pNetwork->ga_sesSessionState.ses_ubNumLevelChanges != ubNumLevelChanges) {
        // bail out
        ThrowF_t(TRANS(" level change was in progress\n"));
      }
      // remember your index
      nmReceived>>pls_Index;
      pls_pcCharacter.pc_iPlayerIndex = pls_Index;
      // finish waiting
      pls_Active = TRUE;
//0311 kwon
	  _pNetwork->SetReadyStart(TRUE);
      return;
    // if this is disconnect message
    }
/* //0522 kwon 삭제.	
	else if (nmReceived.GetType() == MSG_INF_DISCONNECTED) {
			// confirm disconnect
			CNetworkMessage nmConfirmDisconnect(MSG_REP_DISCONNECTED);			
			_pNetwork->SendToServerReliable(nmConfirmDisconnect);

      // throw exception
      CTString strReason;
      nmReceived>>strReason;
      _pNetwork->ga_sesSessionState.ses_strDisconnected = strReason;
      ThrowF_t(TRANS("Cannot add player because: %s\n"), strReason);

    // otherwise
    } 
*/	
	  else {
      // it is invalid message
      ThrowF_t(TRANS("Invalid message while waiting for player registration"));
    }

    // if client is disconnected
    if (!_cmiComm.Client_IsConnected()) {
      // quit
      ThrowF_t(TRANS("Client disconnected"));
    }

  }
/* //0522 kwon 삭제.
		CNetworkMessage nmConfirmDisconnect(MSG_REP_DISCONNECTED);			
	_pNetwork->SendToServerReliable(nmConfirmDisconnect);
*/
  ThrowF_t(TRANS("Timeout while waiting for player registration"));
}

/*
 * Deactivate removed player.
 */
void CPlayerSource::Stop(void)
{
  ASSERT(pls_Active);
  pls_Active = FALSE;
  pls_Index = -2;
}

// request character change for a player
// NOTE: the request is asynchronious and possible failure cannot be detected
void CPlayerSource::ChangeCharacter(CPlayerCharacter &pcNew)
{
  // if the requested character has different guid
  if (!(pls_pcCharacter==pcNew)) {
    // fail
    CPrintF(TRANS("Cannot update character - different GUID\n"));
  }

  // just request the change
  CNetworkMessage nmChangeChar((UBYTE)MSG_REQ_CHARACTERCHANGE);
  nmChangeChar<<pls_Index<<pcNew;
  _pNetwork->SendToServerReliable(nmChangeChar);

  // remember new setting
  pls_pcCharacter = pcNew;
}

/*
 * Set player action.
 */
void CPlayerSource::SetAction(const CPlayerAction &paAction)
{
  // synchronize access to action
  CTSingleLock slAction(&pls_csAction, TRUE);
  // set action

  ULONG ulLast = pls_paAction.pa_ulCreated;
  pls_paAction = paAction;
  
  ULONG ulTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
  if(ulTime <= ulLast) {
    pls_paAction.pa_ulCreated++;
  } else {
    pls_paAction.pa_ulCreated = ulTime;
  }
  //CPrintF("%.2f - created: %d\n", _pTimer->GetRealTimeTick(), SLONG(pls_paAction.pa_llCreated));
}

// get mask of this player for chat messages
ULONG CPlayerSource::GetChatMask(void)
{
  return 1UL<<pls_Index;
}

/* Create action packet from current player commands and for sending to server. */
void CPlayerSource::WriteActionPacket(CNetworkMessage &nm)
{
  static ULONG ulCreatedLast = MAX_ULONG;

  // synchronize access to actions
  CTSingleLock slActions(&pls_csAction, TRUE);

  // check if active and registered
  CPlayerEntity *ppe = NULL;
  if (IsActive()) {
    ppe = (CPlayerEntity *)_pNetwork->GetLocalPlayerEntity(this);
  }
  // if not
  if (ppe==NULL) {
    // just write a dummy bit
    BOOL bActive = 0;
    nm.WriteBits(&bActive, 1);
    return;
  }

  if (ulCreatedLast >= pls_paAction.pa_ulCreated) {  
    pls_paAction.pa_ulCreated++;
  }

  ulCreatedLast = pls_paAction.pa_ulCreated;

  // normalize action (remove invalid floats like -0)
  pls_paAction.Normalize();

  ASSERT(pls_Active);
  ASSERT(pls_Index>=0);

  // determine ping
  FLOAT tmPing = ppe->en_tmPing;
  INDEX iPing = (INDEX)ceil(tmPing*1000);

  // write all in the message
  BOOL bActive = 1;
  nm.WriteBits(&bActive, 1);
  nm.WriteBits(&pls_Index, 4);  // your index
  nm.WriteBits(&iPing, 10);     // your ping
  nm<<pls_paAction;             // action
  //CPrintF("%.2f - written: %d\n", _pTimer->GetRealTimeTick(), SLONG(pls_paAction.pa_llCreated));

  // get sendbehind parameters
  extern INDEX cli_iSendBehind;
  cli_iSendBehind = Clamp(cli_iSendBehind, 0L, 3L);
  INDEX iSendBehind = cli_iSendBehind;

  // disable if server
  if (_pNetwork->IsServer()) {
    iSendBehind = 0;
  }

  // save sendbehind if needed
  nm.WriteBits(&iSendBehind, 2);
  for(INDEX i=0; i<iSendBehind; i++) {
    nm<<pls_apaLastActions[i];
  }
  
  // remember last action
  for(INDEX ipa=PLS_MAXLASTACTIONS-1; ipa>0; ipa--) {
    pls_apaLastActions[ipa] = pls_apaLastActions[ipa-1];
  }
  pls_apaLastActions[0] = pls_paAction;
//0128 클라이언트의 실질적 움직임은 이곳에서 된다.
  //if (!_pNetwork->IsServer()) 
  //{
  //  ApplyActionLocally(pls_paAction);
  //}

  // if not paused
  if (!_pNetwork->IsPaused() && !_pNetwork->GetLocalPause()) {
    // get the index of the player target in game state
    INDEX iPlayerTarget = pls_Index;
    // if player is added
    if (iPlayerTarget>=0) {
      // get the player target
      CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iPlayerTarget];
    }
  }
}


/* Apply an action to the player locally - for local player prediction */
void CPlayerSource::ApplyActionLocally(const CPlayerAction &paAction)
{
  ASSERT (!_pNetwork->IsServer());
  
  // if local prediction is turned off, do nothing
  extern INDEX net_bLocalPrediction;
  if (!net_bLocalPrediction || _pNetwork->IsGameFinished() || _pNetwork->IsPlayingDemo()) {
    return;
  }

  // get this player's player entity
  CPlayerEntity* penPlayerEntity = (CPlayerEntity*) _pNetwork->GetLocalPlayerEntity(this);
  ASSERT(penPlayerEntity != NULL);
  
  // apply this action
  penPlayerEntity->ApplyAction(paAction,_fLatency);
  // run all player updates
  penPlayerEntity->ClearMovingTemp();
  penPlayerEntity->PreMoving();
  penPlayerEntity->DoMoving(); 
  penPlayerEntity->PostMoving();
  penPlayerEntity->ClearMovingTemp();

  if (penPlayerEntity->en_penParent!=NULL && penPlayerEntity->en_penParent->GetPhysicsFlags()&EPF_MOVABLE) {
    penPlayerEntity->en_plLastPlacement.pl_PositionVector =     penPlayerEntity->en_plPlacement.pl_PositionVector - 
      ((CMovableEntity*)(penPlayerEntity->en_penParent))->en_vCurrentTranslationAbsolute*_pTimer->TickQuantum;
  }

}


void CPlayerSource::ApplyNewerActions(ULONG ulOldest)
{
  CPlayerAction paAction;

  // if local prediction is turned off, do nothing
  extern INDEX net_bLocalPrediction;
  if (!net_bLocalPrediction || _pNetwork->IsGameFinished() || _pNetwork->IsPlayingDemo()) {
    return;
  }
  _fLatency = (FLOAT)(((ULONG)_pTimer->GetHighPrecisionTimer().GetMilliseconds()) - ulOldest) / 1000;

  // get this player's player entity
  CPlayerEntity* penPlayerEntity = (CPlayerEntity*) _pNetwork->GetLocalPlayerEntity(this);
  ASSERT(penPlayerEntity != NULL);

  BOOL bFound = FALSE;

  _pNetwork->AddNetGraphValue(NGET_ACTION, _fLatency);

  for(INDEX ipa=PLS_MAXLASTACTIONS-1; ipa>=0; ipa--) {
    CPlayerAction &paAction =  pls_apaLastActions[ipa];   
    if (paAction.pa_ulCreated > ulOldest) {  
      if (!bFound) {
        bFound = TRUE;
      }
      penPlayerEntity->SetReapplyingActions();
      penPlayerEntity->ApplyAction(paAction,_fLatency);
      penPlayerEntity->ClearMovingTemp();
      penPlayerEntity->PreMoving();
      penPlayerEntity->DoMoving(); 
      penPlayerEntity->PostMoving();     
      penPlayerEntity->ClearMovingTemp();
    }
  }
  
  if (penPlayerEntity->en_penParent!=NULL && penPlayerEntity->en_penParent->GetPhysicsFlags()&EPF_MOVABLE) {
    penPlayerEntity->en_plLastPlacement.pl_PositionVector =     penPlayerEntity->en_plPlacement.pl_PositionVector - 
      ((CMovableEntity*)(penPlayerEntity->en_penParent))->en_vCurrentTranslationAbsolute*_pTimer->TickQuantum;
  } 

  //CPrintF("%7.4f\t%7.4f\t%7.4f\t",penPlayerEntity->en_plPlacement.pl_PositionVector(1),penPlayerEntity->en_plPlacement.pl_PositionVector(2),penPlayerEntity->en_plPlacement.pl_PositionVector(3));
  //CPrintF("%7.4f\t%7.4f\t%7.4f\n",penPlayerEntity->en_plLastPlacement.pl_PositionVector(1),penPlayerEntity->en_plLastPlacement.pl_PositionVector(2),penPlayerEntity->en_plLastPlacement.pl_PositionVector(3));

  penPlayerEntity->ClearReapplyingActions();
}
