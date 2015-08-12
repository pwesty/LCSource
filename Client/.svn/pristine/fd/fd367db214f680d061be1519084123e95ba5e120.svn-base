#ifndef SE_INCL_SERVER_H
#define SE_INCL_SERVER_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/Synchronization.h>
//#include <Engine/Network/NetworkMessage.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Templates/StaticArray.h>
#include <Engine/Network/Common.h>

/*
 * Server, manages game joining and similar, routes messages from PlayerSource to PlayerTarget
 */
class ENGINE_API CServer {
public:
	BOOL srv_bActive;                        // set if started

	CStaticArray<CSessionSocket> srv_assoSessions;  // client computers in game
	CStaticArray<CPlayerTarget> srv_apltPlayers;  // player buffers for all clients in game

	TIME srv_tmLastProcessedTick;            // last tick when all actions have been resent
	INDEX srv_iLastProcessedSequence;        // sequence of last sent game stream block
	TIME srv_tmNextAvailableDemoTimeSlot;    // used to regulate demo recording quality/size

	BOOL srv_bPause;        // set while game is paused
	BOOL srv_bSendPause;    // should the pause message be sent over network
	BOOL srv_bPauseValue;
	CTString srv_strPauser; // name of the player that initiated the pause 

	BOOL srv_bGameFinished; // set while game is finished
	FLOAT srv_fServerStep;  // counter for smooth time slowdown/speedup

	CEMsgBuffer     srv_embSendBuffer;      // main tick buffer for messages to be sent to the clients
	CEMsgBuffer     srv_embDemoBuffer;      // tick buffer used to record demos
	CEntityMessage  srv_emEntityMessage;    // a temp message 
	CEMsgBuffer     srv_embOutgoingBuffer;  // a temp buffer used to compile data for a client
	BOOL            srv_bRunning;           // is the message gathering/sending process active

public:
	/* Send disconnect message to some client. */
	void SendDisconnectMessage(INDEX iClient, const char *strExplanation, BOOL bStream = FALSE);
	/* Get total number of active players. */
	INDEX GetPlayersCount(void);
	/* Get number of active vip players. */
	INDEX GetVIPPlayersCount(void);
	/* Get total number of active clients. */
	INDEX GetClientsCount(void);
	/* Get number of active vip clients. */
	INDEX GetVIPClientsCount(void);
	/* Get number of active observers. */
	INDEX GetObserversCount(void);
	/* Get number of active players on one client. */
	INDEX GetPlayersCountForClient(INDEX iClient);
	/* Find first inactive player. */
	CPlayerTarget *FirstInactivePlayer(void);
	/* Check if some character name already exists in this session. */
	BOOL CharacterNameIsUsed(CPlayerCharacter &pcCharacter);

	/* Send initialization info to local client. */
	void ConnectLocalSessionState(INDEX iClient, CNetworkMessage &nm);
	/* Send initialization info to remote client. */
	void ConnectRemoteSessionState(INDEX iClient, CNetworkMessage &nm);
	/* Prepare gamestate data for sending to the client */
	void PrepareGameStateData_t(CTMemoryStream &strmStream,SLONG &slFullSize,SLONG &slSize);
	/* Send the state of the game to a remote client */
	void SendGameStateData(INDEX iClient);

	/* Send session state data to remote client. */
	void SendSessionStateData(INDEX iClient);

	/* Send one regular batch of sequences to a client. */
	void SendGameStreamBlocks(INDEX iClient);
	/* Resend a batch of game stream blocks to a client. */
	void ResendGameStreamBlocks(INDEX iClient, INDEX iSequence0, INDEX ctSequences);

	/* find a mask of all players on a certain client */
	ULONG MaskOfPlayersOnClient(INDEX iClient);

	/* How many active remote sessions are there? */
	INDEX ActiveRemoteSessions();

	/* buffer a message to the buffer */
	void SendMessage(CEntityMessage &emEntityMessage);
	/* put a new tick marker in the buffer */
	void StartNewTick(TIME tmTickTime);
	/* acknowledge a tick (and older ones) for a given client */
	void ReceiveAcknowledge(INDEX iClient,TIME tmTickTime);

	/* add a player update message to the outgoing buffer */
	//BOOL AddPlayerUpdate(INDEX iClient,FLOAT3D &vBasePosition,BOOL bForDemoRec = FALSE); 
	/* prepare a batch of data for a given client */
	//int  PrepareClientData(INDEX iClient);
	/* prepare and send a batch of data to a given client */
	void SendClientData(INDEX iClient);
	/* prepare a batch of data for demo recording */
	//BOOL PrepareDemoData(TIME tmTickTime);
	/* write data from demo buffer to the disk */
	//void WriteDemoData(TIME tmTickTime);

	/* remove a client from the message sending process */
	void CloseClient(INDEX iClient);
	/* drop some clients from the game to free buffer space */
	void DropLaggingClients();

	/* dump server stats to the console */
	void DumpStats();

	/* initialize message gathering/sending process */
	void InitServerNetProcess();
	/* stop message gathering/sending process and clear buffers */
	void StopNetProcess();
	/* run updates for all remote clients */
	void ServerNetProcess();

public:
	/* Constructor. */
	CServer(void);
	/* Destructor. */
	~CServer();

	/* Start server. */
	void Start_t(void);
	/* Stop server. */
	void Stop(void);
	/* Run server loop. */
	void ServerLoop(void);

	/* Handle incoming network messages. */
	void HandleAll();
	void HandleAllForAClient(INDEX iClient);
	void HandleClientDisconected(INDEX iClient);
	void Handle(INDEX iClient, CNetworkMessage &nm);

};



#endif  /* include-once check. */

