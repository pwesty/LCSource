#ifndef SE_INCL_SESSIONSOCKET_H
#define SE_INCL_SESSIONSOCKET_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif
#include <Engine/Base/Timer.h>

// parameters for a client
class CSessionSocketParams {
public:
  INDEX ssp_iBufferActions;
  INDEX ssp_iMaxBPS;
  INDEX ssp_iMinBPS;

public:
  CSessionSocketParams(void);
  void Clear(void);

  // check if up to date with current params
  BOOL IsUpToDate(void);
  // update
  void Update(void);

  // message operations
  friend CNetworkMessage &operator<<(CNetworkMessage &nm, CSessionSocketParams &ssp);
  friend CNetworkMessage &operator>>(CNetworkMessage &nm, CSessionSocketParams &ssp);
};

// connection data for each session connected to a server
class CSessionSocket {
public:
  BOOL sso_bActive;
  BOOL sso_bSendStream;
  BOOL sso_bTrackData;
  CTimerValue sso_tvMessageReceived;
  INDEX sso_iDisconnectedState;
  INDEX sso_iLastSentSequence;
  CTimerValue sso_tvLastMessageSent;    // for sending keep-alive messages
  CTimerValue sso_tvLastPingSent;       // for sending ping
  CNetworkStream sso_nsBuffer;  // stream of blocks buffered for sending
  CSessionSocketParams sso_sspParams; // parameters that the client wants
  INDEX sso_ctLocalPlayers;     // number of players that this client will connect
  BOOL sso_bVIP;          // set if the client was successfully authorized as an VIP

  TIME sso_tmLastTickAcknowledge;   // time of the last acknowledged tick data block
  TIME sso_tmNextAvailableTimeSlot; // when can the next data block be sent
  UBYTE sso_ubNumLevelChanges;      // remember when did this session ask to connect

public:
  CSessionSocket(void);
  ~CSessionSocket(void);
  void Clear(void);
  void Activate(void);
  void Deactivate(void);
  BOOL IsActive(void);
};



#endif  /* include-once check. */

