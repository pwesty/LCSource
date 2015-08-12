#ifndef SE_INCL_NETWORKMESSAGE_H
#define SE_INCL_NETWORKMESSAGE_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#ifndef NUMTOBUF
#define NUMTOBUF(b, n)	{ memcpy(b, &n, sizeof(n)); b += sizeof(n); }
#define BUFTONUM(n, b)	{ memcpy(&n, b, sizeof(n)); b += sizeof(n); }
#define HTONL(n)		{ n = htonl(n); }
#define HTONS(n)		{ n = htons(n); }
#define NTOHL(n)		{ n = ntohl(n); }
#define NTOHS(n)		{ n = ntohs(n); }
#define	HTONLL(n)		{ n	= htonll3(n);}
#define	NTOHLL(n)		{ n = ntohll3(n);}
#endif

#include <Engine/Base/Lists.h>
#include <Engine/Math/Vector.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/Translation.h>
  // maximum server buffer size is 256k
  #define SERVER_MAX_BUFFER_SIZE  256*1024
// maximum size of message the server is allowed to send
#define SERVER_MAX_MESSAGE_SIZE (SERVER_MAX_BUFFER_SIZE/3)
// EDIT : BS : 070413 : 신규 패킷 암호화
#define MAX_MESSAGE_DATA		(10000 - 1)

///////

/*
 * Holder for network message, can be read/written like a stream.
 */
class ENGINE_API CNetworkMessage {
public:
  UBYTE nm_mtType;                  // type of this message

#define MAX_NETWORKMESSAGE_SIZE (SERVER_MAX_MESSAGE_SIZE+1024)   // max. length of message buffer 
                                                                 // enough to hold max server message plus a safety margin
  UBYTE *nm_pubMessage;       // the message data itself
  SLONG nm_slMaxSize;         // size of message buffer

  UBYTE *nm_pubPointer;       // pointer for reading/writing message
  SLONG nm_slSize;            // size of message
  INDEX nm_iBit;              // next bit index to read/write (0 if not reading/writing bits)
  //0315 kwon
  INDEX nm_iIndex;
public:
  /* Constructor for empty message (for receiving). */
  CNetworkMessage(void);
  /* Constructor for initializing message that is to be sent. */
  CNetworkMessage(UBYTE mtType);
  /* Copying. */
  CNetworkMessage(const CNetworkMessage &nmOriginal);
  void operator=(const CNetworkMessage &nmOriginal);
  /* Destructor. */
  ~CNetworkMessage(void);

  void *GetBuffer()		{ return this->nm_pubMessage; }
  // Network message initialization
  void Initialize(void);
  void Initialize(const CNetworkMessage &nmOriginal);
  void Initialize(UBYTE mtType);

  // reinit a message that is to be sent (to write different contents)
  void Reinit(void);

  /* Ignore the contents of this message. */
  void IgnoreContents(void);
  // dump message to console
  void Dump(void);

  /* Get the type of this message. */
  inline UBYTE GetType(void) const { ASSERT(this!=NULL); return UBYTE(nm_mtType&0x7F); }; // sora 네트워크 메시지 처크시 0x3F를 사용하면 64이상의 값(64 & 0x3F = 0)에서 문제가 발생해서 범위 수정
  /* Check if end of message. */
  BOOL EndOfMessage(void);
  // rewind message to start, so that written message can be read again
  void Rewind(void);

  /* Pack a message to another message (message type is left untouched). */
  void Pack(CNetworkMessage &nmPacked, CCompressor &comp);
  void PackDefault(CNetworkMessage &nmPacked);
  /* Unpack a message to another message (message type is left untouched). */
  void Unpack(CNetworkMessage &nmUnpacked, CCompressor &comp);
  void UnpackDefault(CNetworkMessage &nmUnpacked);

  // read/write functions
  inline void Read(void *pvBuffer, SLONG slSize);
  inline void Write(const void *pvBuffer, SLONG slSize);
  void ReadBits(void *pvBuffer, INDEX ctBits);
  void WriteBits(const void *pvBuffer, INDEX ctBits);

  inline void setSize(int size) { this->nm_slSize = size; }
 // LONGLONG ntohll(LONGLONG n);

  inline LONGLONG ntohll3(LONGLONG n)
  {
#ifdef BIG_ENDIAN
	  return n;
#else
	  char* p = (char*)&n;
	  char ch;
	  ch = p[0];	p[0] = p[7];	p[7] = ch;
	  ch = p[1];	p[1] = p[6];	p[6] = ch;
	  ch = p[2];	p[2] = p[5];	p[5] = ch;
	  ch = p[3];	p[3] = p[4];	p[4] = ch;
	  return n;
#endif
  }

  inline LONGLONG htonll3(LONGLONG n)
  {
#ifdef BIG_ENDIAN
	  return n;
#else
	  char* p = (char*)&n;
	  char ch;
	  ch = p[0];	p[0] = p[7];	p[7] = ch;
	  ch = p[1];	p[1] = p[6];	p[6] = ch;
	  ch = p[2];	p[2] = p[5];	p[5] = ch;
	  ch = p[3];	p[3] = p[4];	p[4] = ch;
	  return n;
#endif
  }
 
  /* Read an object from message. */
  inline CNetworkMessage &operator>>(float  &f) { Read(&f,  sizeof(f)); return *this;}
  inline CNetworkMessage &operator>>(double &d) { Read(&d,  sizeof(d)); return *this; }
  inline CNetworkMessage &operator>>(int &n) { Read(&n, sizeof(n)); NTOHL(n); return *this; }
  inline CNetworkMessage &operator>>(ULONG &ul) { Read(&ul, sizeof(ul)); NTOHL(ul);  return *this; }
  inline CNetworkMessage &operator>>(UWORD &uw) { Read(&uw, sizeof(uw)); NTOHS(uw); return *this; }
  inline CNetworkMessage &operator>>(UBYTE &ub) { Read(&ub, sizeof(ub)); return *this; }
  inline CNetworkMessage &operator>>(SLONG &sl) { Read(&sl, sizeof(sl)); NTOHL(sl); return *this; }
  inline CNetworkMessage &operator>>(SWORD &sw) { Read(&sw, sizeof(sw)); NTOHS(sw); return *this; }
  inline CNetworkMessage &operator>>(SBYTE &sb) { Read(&sb, sizeof(sb)); return *this; }
  //inline CNetworkMessage &operator>>(UBYTE &mt) { Read(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator>>(MSG_LOGIN_TYPE &mt) { Read(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator>>(MSG_FAIL_TYPE &mt) { Read(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator>>(MSG_MOVE_TYPE &mt) { Read(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator>>(MSG_DB_TYPE &mt) { Read(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator>>(MSG_MENU_TYPE &mt) { Read(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator>>(MSG_CHAT_TYPE &mt) { Read(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator>>(MSG_CHAR_TYPE &mt) { Read(&mt, sizeof(mt)); return *this; }
  inline CNetworkMessage &operator>>(SQUAD &ll) { Read(&ll, sizeof(ll)); NTOHLL(ll); return *this; }  	  	 	    
  CNetworkMessage &operator>>(CTString &str); 
  /* Write an object into message. */
  inline CNetworkMessage &operator<<(const float  &f) { Write(&f,  sizeof(f)); return *this; }
  inline CNetworkMessage &operator<<(const double &d) { Write(&d,  sizeof(d)); return *this; }
  inline CNetworkMessage &operator<<(ULONG ul) { HTONL(ul); Write(&ul, sizeof(ul)); return *this; }
  inline CNetworkMessage &operator<<(UWORD uw) { HTONS(uw); Write(&uw, sizeof(uw)); return *this; }
  inline CNetworkMessage &operator<<(const UBYTE &ub) { Write(&ub, sizeof(ub)); return *this; }
  inline CNetworkMessage &operator<<(SLONG sl) { HTONL(sl); Write(&sl, sizeof(sl)); return *this; }
  inline CNetworkMessage &operator<<(SWORD sw) { HTONS(sw); Write(&sw, sizeof(sw)); return *this; }
  inline CNetworkMessage &operator<<(const SBYTE &sb) { Write(&sb, sizeof(sb)); return *this; }
  //inline CNetworkMessage &operator<<(const UBYTE &mt) { Write(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator<<(const MSG_LOGIN_TYPE &mt) { Write(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator<<(const MSG_FAIL_TYPE &mt) { Write(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator<<(const MSG_MOVE_TYPE &mt) { Write(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator<<(const MSG_DB_TYPE &mt) { Write(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator<<(const MSG_MENU_TYPE &mt) { Write(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator<<(const MSG_CHAT_TYPE &mt) { Write(&mt, sizeof(mt)); return *this; }
  //inline CNetworkMessage &operator<<(const MSG_CHAR_TYPE &mt) { Write(&mt, sizeof(mt)); return *this; }
  inline CNetworkMessage &operator<<(SQUAD ll) { HTONLL(ll); Write(&ll, sizeof(ll)); return *this; }  
  CNetworkMessage &operator<<(const CTString &str);

  /* Insert a sub-message into this message. */
  void InsertSubMessage(const CNetworkMessage &nmSubMessage);
  /* Extract a sub-message from this message. */
  void ExtractSubMessage(CNetworkMessage &nmSubMessage);

  // shrink message buffer to exactly fit contents
  void Shrink(void);

  void SetLoginType( UBYTE LoginType);
};




// read/write functions
// memset and memcpy are intrinsinc functions and will be optimized by the compiler
inline void CNetworkMessage::Read(void *pvBuffer, SLONG slSize)
{
  if (nm_pubPointer+slSize > nm_pubMessage+nm_slSize) {
    CPrintF(TRANS("Warning: Message over-reading!\n"));
 //   ASSERT(FALSE);
    memset(pvBuffer, 0, slSize);
    return;
  }
  memcpy(pvBuffer, nm_pubPointer, slSize);
  nm_pubPointer += slSize;
  nm_iBit = 0;
}

inline void CNetworkMessage::Write(const void *pvBuffer, SLONG slSize)
{
  if (nm_pubPointer+slSize > nm_pubMessage+nm_slMaxSize) {
    CPrintF(TRANS("Warning: Message over-writing!\n"));
    ASSERT(FALSE);
    return;
  }
  memcpy(nm_pubPointer, pvBuffer, slSize);
  nm_pubPointer += slSize;
  nm_iBit = 0;
  nm_slSize += slSize;
}



/*
 * A message block used for streaming data across network.
 *
 * These can be received duplicated or misordered. They
 * are resequenced at the receive side as needed. Can be sent more than one
 * together as submessages in a message and duplicated across messages as a
 * compensation for eventual packet loss.
 */
class CNetworkStreamBlock : public CNetworkMessage {
public:
  CListNode nsb_lnInStream;     // node in list of blocks in stream
public:
  INDEX nsb_iSequenceNumber;    // index for sorting in list
public:
  /* Constructor for receiving -- uninitialized block. */
  CNetworkStreamBlock(void);
  /* Constructor for sending -- empty packet with given type and sequence. */
  CNetworkStreamBlock(UBYTE mtType, INDEX iSequenceNumber);

  /* Read a block from a received message. */
  void ReadFromMessage(CNetworkMessage &nmToRead);
  /* Add a block to a message to send. */
  void WriteToMessage(CNetworkMessage &nmToWrite);

  /* Remove the block from stream. */
  void RemoveFromStream(void);
  /* Clear block */
  void Clear(void);

  /* Read/write the block from file stream. */
  void Read_t(CTStream &strm); // throw char *
  void Write_t(CTStream &strm); // throw char *

  void operator delete(void *p);
};

/*
 * Stream of message blocks that can be sent across network.
 */
class CNetworkStream {
public:
  enum Result {
    R_OK = 1,
    R_BLOCKMISSING,           // block is missing in the stream
    R_BLOCKNOTRECEIVEDYET,    // block is not yet received
  };
public:
  CListHead ns_lhBlocks;   // list of blocks of this stream (higher sequences first)

  /* Add a block that is already allocated to the stream. */
  void AddAllocatedBlock(CNetworkStreamBlock *pnsbBlock);
public:
  /* Constructor. */
  CNetworkStream(void);
  /* Destructor. */
  ~CNetworkStream(void);
  /* Clear the object (remove all blocks). */
  void Clear(void);
  /* Copy from another network stream. */
  void Copy(CNetworkStream &nsOther);
  // get number of blocks used by this object
  INDEX GetUsedBlocks(void);
  // get amount of memory used by this object
  SLONG GetUsedMemory(void);
  // get index of newest sequence stored
  INDEX GetNewestSequence(void);

  /* Add a block to the stream (makes a copy of block). */
  void AddBlock(CNetworkStreamBlock &nsbBlock);
  /* Read a block as a submessage from a message and add it to the stream. */
  void ReadBlock(CNetworkMessage &nmMessage);
  /* Get a block from stream by its sequence number. */
  CNetworkStream::Result GetBlockBySequence(
    INDEX iSequenceNumber, CNetworkStreamBlock *&pnsbBlock);
  // find oldest block after given one (for batching missing sequences)
  INDEX GetOldestSequenceAfter(INDEX iSequenceNumber);

  /* Write given number of newest blocks to a message. */
  INDEX WriteBlocksToMessage(CNetworkMessage &nmMessage, INDEX ctBlocks);
  /* Remove all blocks but the given number of newest ones. */
  void RemoveOlderBlocks(INDEX ctBlocksToKeep);
  /* Remove all blocks with sequence older than given. */
  void RemoveOlderBlocksBySequence(INDEX iLastSequenceToKeep);
};

class ENGINE_API CPlayerAction {
public:
  // order is important for compression and normalization - do not reorder!
  FLOAT3D pa_vTranslation;
  ANGLE3D pa_aRotation;
  ANGLE3D pa_aViewRotation;
  ULONG pa_ulButtons;       // 32 bits for action buttons (application defined)
    // keep flags that are likely to be changed/set more often at lower bits,
    // so that better compression can be achieved for network transmission
  ULONG pa_ulCreated;     // when was created (for ping calc.) in ms

public:
  CPlayerAction(void);
  /* Clear the object (this sets up no actions). */
  void Clear(void);
  // normalize action (remove invalid floats like -0)
  void Normalize(void);

  void Lerp(const CPlayerAction &pa0, const CPlayerAction &pa1, FLOAT fFactor);

  /* Write an object into message. */
  friend CNetworkMessage &operator<<(CNetworkMessage &nm, const CPlayerAction &pa);
  /* Read an object from message. */
  friend CNetworkMessage &operator>>(CNetworkMessage &nm, CPlayerAction &pa);
  /* Write an object into stream. */
  friend CTStream &operator<<(CTStream &strm, const CPlayerAction &pa);
  /* Read an object from stream. */
  friend CTStream &operator>>(CTStream &strm, CPlayerAction &pa);
};

// Create new network stream block (use instead of new CNetworkStreamBlock)
extern CNetworkStreamBlock *CreateNetworkStreamBlock(void);
// Delete network stream block (use instead of delete CNetworkStreamBlock)
extern void DeleteNetworkStreamBlock(CNetworkStreamBlock *ppaPacket);

#endif  /* include-once check. */

