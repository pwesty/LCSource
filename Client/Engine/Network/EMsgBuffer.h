#ifndef SE_INCL_EMSGBUFFER_H
#define SE_INCL_EMSGBUFFER_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif


#include <Engine/Math/Placement.h>
#include <Engine/Network/Buffer.h>
#include <Engine/Network/NetworkMessage.h>

#define EM_SIZE 256

// data for how many ticks will be accumulated before an error is reported
  #define MAX_TICKS_KEPT 600 // 30 seconds on PC


// max 31 types of messages - stored in the lowest 4 bits of the em_uwType word 
// upper 12 bits contain event code, for create and copy messages
#define EMT_CREATE					     1  // client should create an entity (carries the id, class and placement)
#define	EMT_COPY						     2  // client should copy an entity (spawners)
#define	EMT_SETPLACEMENT		     3  // update entities placement and reset dead reckoning parameters
#define	EMT_DESTROY					     4  // destroy the entity
#define	EMT_EVENT						     5  // make the entity handle an event
#define EMT_INITIALIZE           6  // initialize an entity (use InitializeFromNet())
#define EMT_ADDPLAYER            7  // add a player to the game
#define EMT_STARTANIM            8  // start/change animation on an entity
#define EMT_PLAYERUPDATE         9  // update player's data
#define EMT_SETPLACEMENT_NOTIFY 10  // this is internal for the server, the entity just notifies the server that it has changed placement significantly 
#define EMT_SKA_ANIM            11  // a Ska animation command - add, remove animations...
#define EMT_PAUSE               12  // pause/unpause a game
#define EMT_REMPLAYER           13  // remove a player from the game
#define EMT_CHANGECHARACTER     14  // change a player character
#define EMT_SOUND               15  // play/stop/change 3D parameters of a sound
#define EMT_BASEPLACEMENT       16  // base placement used for compression/decompression of placements
#define EMT_ADDSKASTRING        17  // a new string is added to ska string table
#define EMT_PLAYERACTION        18  // used to record executed actions for demo recording/playback

#define	EMT_NUMBEROFTYPES		    18 	//	how many different types of messages there are

#define EM_SKA_ADD               1
#define EM_SKA_REM               2
#define EM_SKA_REMID             3
#define EM_SKA_NEWCLONED         4
#define EM_SKA_NEWCLEAR          5
#define EM_SKA_STRETCH           6
#define EM_SKA_STRETCHSINGLE     7


#define EM_SOUND_PLAY            1
#define EM_SOUND_STOP            2
#define EM_SOUND_SETVOLUME       3
#define EM_SOUND_SETPITCH        4
#define EM_SOUND_SETRANGE        5
#define EM_SOUND_FADE            6

// error messages
#define EMB_SUCCESS_OK			        0
#define EMB_ERR_MAX_TICKS		        1
#define EMB_ERR_NOT_IN_BUFFER       2
#define EMB_ERR_TICK_NOT_COMPLETE   4
#define EMB_ERR_BUFFER_TOO_SMALL    5
#define EMB_ERR_BUFFER_EMPTY        6


// this is a class that contains a single message
class ENGINE_API CEntityMessage {
public:
	ULONG em_ulType;              // message type - see above
	CNetworkMessage em_nmMessage; // messge data
	UWORD em_uwSize;              // message size
	ULONG em_ulEntityID;          // ID of the entity this message is for
  TIME  em_tmMessageTime;       // what tick is this message for

  // constructor and destructor - nothing to do
	CEntityMessage(){};
	~CEntityMessage(){};

  // write a placement message - record position, orientation, translation and rotation
	void WritePlacement(ULONG &ulEntityID,const CPlacement3D &plPlacement,const CPlacement3D &plSpeed);
  // read a placement message - position, orientation, translation and rotation
	void ReadPlacement(ULONG &ulEntityID,CPlacement3D &plPlacement,CPlacement3D &plSpeed);

  // write an entity create message - id, placement and class
	void WriteEntityCreate(ULONG &ulEntityID,const CPlacement3D &plPlacement,UWORD &uwEntityClassID);
  // read an entity create message - id, placement and class
	void ReadEntityCreate(ULONG &ulEntityID,CPlacement3D &plPlacement,UWORD &uwEntityClassID);

  // write a copy entity message - ids of source and target entities, etc...
	void WriteEntityCopy(ULONG &ulSourceEntityID,ULONG &ulTargetEntityID,const CPlacement3D &plPlacement,UBYTE ubWithDescentants);
  // read a copy entity message
	void ReadEntityCopy(ULONG &ulSourceEntityID,ULONG &ulTargetEntityID,CPlacement3D &plPlacement,UBYTE ubWithDescentants);

  // write a destroy entity message - just the id of the entity to be destroyed
	void WriteEntityDestroy(ULONG &ulEntityID);
  // read a destroy entity message - entitie's id
	void ReadEntityDestroy(ULONG &ulEntityID);

  // write an entity event message
	void WriteEntityEvent(ULONG &ulEntityID,SLONG &slEventCode,void* pvEventData,SLONG &slDataSize);
  // read an entity event message
	void ReadEntityEvent(ULONG &ulEntityID,SLONG &slEventCode,void* pvEventData,SLONG &slDataSize);

  // write an initialization message
  void WriteEntityInitialize(ULONG &ulEntityID,SLONG &slEventCode,void* pvEventData,SLONG &slDataSize);
  // read an initialization message
	void ReadEntityInitialize(ULONG &ulEntityID,SLONG &slEventCode,void* pvEventData,SLONG &slDataSize);

  // write an add player message - player idex, character info and specified weapons and animator ids
  void WriteAddPlayer(ULONG &ulEntityID,INDEX &iPlayerIndex,CPlayerCharacter &pcCharacter,ULONG &ulWeaponsID,ULONG &ulAnimatorID,INDEX iClient);
  // read an add player message - player idex, character info and specified weapons and animator ids
  void ReadAddPlayer(ULONG &ulEntityID,INDEX &iPlayerIndex,CPlayerCharacter &pcCharacter,ULONG &ulWeaponsID,ULONG &ulAnimatorID,INDEX &iClient);

  // write a start animation message
  void WriteStartAnim(ULONG &ulEntityID,INDEX &iNewModelAnim, ULONG &ulFlags);
  // read a start animation message
  void ReadStartAnim(ULONG &ulEntityID,INDEX &iNewModelAnim, ULONG &ulFlags);

  // write an update of player's data - data is prepared in advance by the server and is stored in 'ostr'
  void WritePlayerUpdate(ULONG &ulEntityID,CNetworkMessage &ostr);
  // read an update of player's data - data is not extracted from 'istr', player entity will do that for itself
  void ReadPlayerUpdate(ULONG &ulEntityID,CNetworkMessage &istr);

  // write a notification of changed placement
  void WritePlacementNotify(ULONG &ulEntityID,UBYTE bForceUpdate = FALSE);
  // read a notification of changed placement
  void ReadPlacementNotify(ULONG &ulEntityID,UBYTE &bForceUpdate);

  // write a Ska animation message 
  void WriteSka(ULONG &ulEntityID,UBYTE ubSkaCode,void* pvSkaData,SLONG &slDataSize);
  // read a Ska animation message
  void ReadSka(ULONG &ulEntityID,UBYTE &ubSkaCode,void* pvSkaData,SLONG &slDataSize);

  // write a pause message
	void WritePause(BOOL &bPaused,CTString &strPauser);
  // read a pause message
	void ReadPause(BOOL &bPaused,CTString &strPauser);

  // write a message to remove pa player from the game
  void WriteRemovePlayer(INDEX &iPlayer);
  // read a message to remove pa player from the game
  void ReadRemovePlayer(INDEX &iPlayer);

  // write a message to change a player's character
  void WriteCharacterChange(ULONG &ulEntityID,CPlayerCharacter &pcCharacter);
  // read a message to change a player's character
  void ReadcharacterChange(ULONG &ulEntityID,CPlayerCharacter &pcCharacter);

  // write a Ska animation message 
  void WriteSound(ULONG &ulEntityID,UBYTE ubSoundCode,void* pvSoundData,SLONG &slDataSize);
  // read a Ska animation message
  void ReadSound(ULONG &ulEntityID,UBYTE &ubSoundCode,void* pvSoundData,SLONG &slDataSize);

  // write a base placement used for compression of entity placements
  void WriteBasePosition(FLOAT3D &vBasePosition);
  // read a base placement used for compression of entity placements
  void ReadBasePosition(FLOAT3D &vBasePosition);

  // write a message to add a new string to the ska string table
  void WriteAddSkaString(INDEX iIndex,const CTString &strString);
  // read a message to add a new string to the ska string table
  void ReadAddSkaString(INDEX &iIndex,CTString &strString);

  // write a PlayerAction - used for recording demos
  void WritePlayerAction(ULONG &ulEntityID,CPlayerAction &paAction);
  // read a PlayerAction - used for demo playback
  void ReadPlayerAction(ULONG &ulEntityID,CPlayerAction &paAction);

};

//! 이 구조체에는 Tick에 대한 정보가 담겨있다. 
// this struct contains information about a tick - marks it's starting position in the buffer, tick time
// for which it was creted, number of acknowledges it is to receive and the number of messages it contains
struct TickMarker {
	TIME  tm_tmTickTime; 
	SLONG tm_slTickOffset;
	UBYTE tm_ubAcknowledgesExpected; //아크패킷이 돌아오는데 예상되는 시간.
  UWORD tm_uwNumMessages; //! 메시지 갯수

  TickMarker() {
    tm_tmTickTime = -1;
    tm_slTickOffset = -1;
    tm_ubAcknowledgesExpected = 0;
    tm_uwNumMessages = 0;
  }
};



// Entity message buffer - stores entity messages - create, copy, destroy, set placement...
class CEMsgBuffer : public CBuffer {
public:
	TickMarker	emb_atmTickMarkers[MAX_TICKS_KEPT];		// a set of tick marker data
	UWORD		emb_uwNumTickMarkers;     // number of tick markers in the buffer
	INDEX		emb_iFirstTickMarker;     // index of the first tick
  INDEX		emb_iCurrentTickMarker;   // index of the current (last) tick

  // constructor and destructor
	CEMsgBuffer();
	~CEMsgBuffer();

  // initialization of the buffer (override for CBuffer::Clear())
  virtual void Clear(void);

  // write a message to the bufffer
	BOOL WriteMessage(CEntityMessage &emEntityMessage, BOOL bExpand = TRUE);
  // read a message from the bufffer
	int  ReadMessage(CEntityMessage &emEntityMessage);
  // peek at a message at a specified offset in the buffer - this does not advance the buffer beyond the read message
  int  PeekMessageAtOffset(CEntityMessage &emEntityMessage,SLONG &slTickOffset);

  // read data for one tick - does not advance the read offset
  int  ReadTick(float fTickTime,const void *pv, SLONG &slSize);
  // write tick data from source memory to the buffer
  void WriteTick(float fTickTime,const void *pv, SLONG slSize);
  // set the first buffered tick to the specified tick time
  int  MoveToStartOfTick(float fTickTime);  

  // mark the start of a new tick with a specified tick time
  int StartNewTick(float fTickTime);
  // get the marker index for specified time
  int GetTickIndex(float fTickTime,INDEX &iTickIndex);
  // get the buffer offset for a specified tick time
  int GetTickOffset(float fTickTime,SLONG &slTickOffset);
  // find the first tick time after some specified time
  int GetNextTickTime(float fTickTime,float &fNextTickTime);
  // mark that an acknowledge is required for this tick (i.e. reference counting)
  int RequestTickAcknowledge(float fTickTime,UBYTE ubNumAcknowledges);
  // acknowledge a specified tick time, and all older
  int ReceiveTickAcknowledge(float fLastAck, float fTickTime);

  

  // expand buffer to be given number of bytes in size (override for CBuffer::Expand())
  virtual void Expand(SLONG slNewSize);

	// write bytes to buffer (override for CBuffer::WriteBytes())
  virtual void WriteBytes(const void *pv, SLONG slSize);
  // peek a number of bytes from the data buffer (does not advance the buffer read offset)
	SLONG PeekBytes(const void *pv, SLONG slSize);
  // peek a number of bytes from the data buffer at a given offset
  SLONG PeekBytesAtOffset(const void *pv, SLONG slSize,SLONG &slTickOffset);



};

#endif
