#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/PlayerTarget.h>
#include <Engine/Base/Stream.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Templates/StaticArray.cpp>

extern INDEX cli_bLerpActions;
/*
 *  Constructor.
 */
CPlayerTarget::CPlayerTarget(void) {
  plt_bActive = FALSE;
  plt_penPlayerEntity = NULL;
  plt_csAction.cs_iIndex = -1;
  plt_paLastAction.Clear();
  plt_paPreLastAction.Clear();
  plt_abReceived.Clear();
  plt_iClient = -1;

}

/*
 *  Destructor.
 */
CPlayerTarget::~CPlayerTarget(void) 
{	
	if (plt_penPlayerEntity != NULL)
	{
		plt_penPlayerEntity->Destroy(FALSE);
		plt_penPlayerEntity = NULL;
	}
}

//! 스트림으로부터 플레이어 정보를 읽는다.
/*
 * Read player information from a stream.
 */
void CPlayerTarget::Read_t(CTStream *pstr) // throw char *
{
  ULONG ulEntityID;
  ULONG bActive;
  // synchronize access to actions
  CTSingleLock slActions(&plt_csAction, TRUE);

  // read activity flag
  (*pstr) >> bActive;
  (*pstr) >> plt_Index >> plt_iClient;

  //! 이 클라이언트가 활성화 되어있다면,
  // if client is active
  if (bActive) {
    // set it up
    Activate(plt_iClient);
	//! 엔티티 id, 마지막 액션,이전 마지막 액션을 읽어오고, id로 플레이어 엔티티를 찾아서 설정.
    // read data
    (*pstr)>>ulEntityID>>plt_paLastAction>>plt_paPreLastAction;
    CPlayerEntity *penPlayer = (CPlayerEntity *)_pNetwork->ga_World.EntityFromID(ulEntityID);

    ASSERT(penPlayer != NULL);
    AttachEntity(penPlayer);
  }
}

/*
 * Write client information into a stream.
 */
void CPlayerTarget::Write_t(CTStream *pstr) // throw char *
{
  ULONG ulEntityID;
  ULONG bActive = plt_bActive;
  // synchronize access to actions
  CTSingleLock slActions(&plt_csAction, TRUE);

  // write activity flag
  (*pstr) << bActive;
  (*pstr) << plt_Index << plt_iClient;

  // if client is active
  if (bActive) {
    // prepare its data
    ulEntityID = plt_penPlayerEntity->en_ulID;
    // write data
    (*pstr)<<ulEntityID<<plt_paLastAction<<plt_paPreLastAction;
  }
}

/*
 * Activate client data for a new client.
 */
void CPlayerTarget::Activate(INDEX iClient)
{
  ASSERT(!plt_bActive);
  plt_bActive = TRUE;
  plt_paPreLastAction.Clear();
  plt_paLastAction.Clear();
  plt_iClient = iClient;
  // make packets dummy before receiving something
  plt_abReceived.Clear();

}

/*
 * Deactivate client data for removed client.
 */
void CPlayerTarget::Deactivate(void)
{
  ASSERT(plt_bActive);
  plt_bActive = FALSE;
  plt_penPlayerEntity = NULL;
  plt_paPreLastAction.Clear();
  plt_paLastAction.Clear();
  plt_iClient = -1;
}

/*
 * Attach an entity to this client.
 */
void CPlayerTarget::AttachEntity(CPlayerEntity *penClientEntity)
{
  ASSERT(plt_bActive);
  plt_penPlayerEntity = penClientEntity;
}

/* 
 * Are there any actions buffered for this player 
 */
BOOL CPlayerTarget::HasBufferedActions() {
  return (plt_abReceived.GetCount()>0);
}


/*
 * Apply action packet to current actions.
 */
void CPlayerTarget::ApplyActionPacket(const CPlayerAction &paAction)
{
  ASSERT(plt_bActive);
  ASSERT(plt_penPlayerEntity != NULL);
  // synchronize access to actions
  CTSingleLock slActions(&plt_csAction, TRUE);

  plt_paLastAction = paAction;
   
  FLOAT fLatency = 0.0f;
  // if the player is local
  if (_pNetwork->IsPlayerLocal(plt_penPlayerEntity)) {
    // calculate latency
    ULONG ulmsNow = (ULONG)_pTimer->GetHighPrecisionTimer().GetMilliseconds();
    ULONG ulmsCreated = plt_paLastAction.pa_ulCreated;
    fLatency = FLOAT(DOUBLE(ulmsNow-ulmsCreated)/1000.0f);
    if (plt_paLastAction.pa_ulCreated==plt_paPreLastAction.pa_ulCreated) {
      _pNetwork->AddNetGraphValue(NGET_REPLICATEDACTION, fLatency);
    } else {
      _pNetwork->AddNetGraphValue(NGET_ACTION, fLatency);
    }
  }

  // if the entity is not deleted and this is the server
  if (!(plt_penPlayerEntity->en_ulFlags&ENF_DELETED) && _pNetwork->IsServer()) {
    // call the player DLL class to apply the new action to the entity
    plt_penPlayerEntity->ApplyAction(plt_paLastAction, fLatency);
  }

}


/*
 * Receive action packet from player source.
 */
void CPlayerTarget::ReceiveActionPacket(CNetworkMessage *pnm, INDEX iMaxBuffer)
{
  ASSERT(plt_bActive);
  // receive new action
  CPlayerAction pa;
  (*pnm)>>pa;
  
  // buffer it
  plt_abReceived.AddAction(pa);

  // read sendbehind 
  INDEX iSendBehind = 0;
  pnm->ReadBits(&iSendBehind, 2);
  // foreach resent action
  for(INDEX i=0; i<iSendBehind; i++) {
    CPlayerAction paOld;
    (*pnm)>>paOld;

    // if not already sent out back to the client
    if (paOld.pa_ulCreated>plt_paLastAction.pa_ulCreated) {
      // buffer it
      plt_abReceived.AddAction(paOld);
    }
  }

  // while there are too many actions buffered
  while(plt_abReceived.GetCount()>iMaxBuffer) {
    // purge the oldest one
    plt_abReceived.RemoveOldest();
  }
}

/* 
 * Get the next acion packet that needs to be executed. 
 */
void CPlayerTarget::GetActionPacket(CPlayerAction &paAction) {
  // there must be at least one action in the buffer
  ASSERT (plt_abReceived.GetCount()>0);

  // retrieve the oldest one
  plt_abReceived.GetActionByIndex(0, paAction);

  // remember the last sent action
  plt_paLastAction = paAction;

  // remove sent action from the buffer
  plt_abReceived.RemoveOldest();
}


