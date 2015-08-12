#ifndef SE_INCL_PLAYERTARGET_H
#define SE_INCL_PLAYERTARGET_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/Synchronization.h>
//#include <Engine/Network/NetworkMessage.h>
#include <Engine/Entities/PlayerCharacter.h>
#include <Engine/Network/ActionBuffer.h>

/*
 * Player target, located in each session state; receiving actions
 */
class CPlayerTarget {
public:
  BOOL plt_bActive;                     // set if this player exists
  CPlayerEntity *plt_penPlayerEntity;   // player entity used by this player
  CTCriticalSection plt_csAction;       // access to player action
  CPlayerAction plt_paPreLastAction;
  CPlayerAction plt_paLastAction;       // last action received

  INDEX plt_Index;
  INDEX plt_iClient;                    // client that controls this player
  INDEX plt_iPing;                      // ping in milliseconds
  CActionBuffer plt_abReceived;         // buffer of actions that were received, but not sent yet
  CPlayerCharacter plt_pcCharacter;     // this player's character data

public:

  /* Default constructor. */
  CPlayerTarget(void);
  /* Destructor. */
  ~CPlayerTarget(void);

  /* Activate player target for a new player. */
  void Activate(INDEX iClient);
  /* Deactivate player target for removed player. */
  void Deactivate(void);
  /* Check if this player is active. */
  BOOL IsActive(void) { return plt_bActive; };
	/* Attach an entity to this player. */
  void AttachEntity(CPlayerEntity *penClientEntity);
  /* Are there any actions buffered for this player */
  BOOL HasBufferedActions();

  /* Apply action packet to current actions. */
  void ApplyActionPacket(const CPlayerAction &paDelta);

  /* Read player information from a stream. */
  void Read_t(CTStream *pstr);   // throw char *
  /* Write player information into a stream. */
  void Write_t(CTStream *pstr);  // throw char *

  /* Receive action packet from player source. */
  void ReceiveActionPacket(CNetworkMessage *pnm, INDEX iMaxBuffer);
  /* Get the next acion packet that needs to be executed. */
  void GetActionPacket(CPlayerAction &paAction);

};


#endif  /* include-once check. */

