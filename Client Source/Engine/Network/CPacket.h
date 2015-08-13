#ifndef SE_INCL_PACKET_H
#define SE_INCL_PACKET_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/Lists.h>
#include <Engine/Base/Timer.h>
#include <LCCRC32.h>


// The total size of the UDP packet should be below 1450 bytes, to reduce the 
// packet drop rate caused by routers dropping large UDP packets. UDP_BLOCK_SIZE is the
// maximum length of real data, not counting the packet header (ulPacketSequence, bReliable,uwID)
#define MAX_UDP_BLOCK_SIZE	10000
#define MAX_HEADER_SIZE (sizeof(UWORD) + sizeof(ULONG) + sizeof(UWORD) + sizeof(ULONG)) // pa_bReliable + pa_ulPacketSequence + pa_uwID + pa_ulTransferSize
#define MAX_PACKET_SIZE (MAX_UDP_BLOCK_SIZE + MAX_HEADER_SIZE)

extern INDEX net_iUDPBlockSize;
extern INDEX net_iMaxPacketSize;

// flags for different kinds of packets used by the netcode - note that the acknowledge packets are unreliable
#define UDP_PACKET_UNRELIABLE          (1 << 0)   //  unreliable packet
#define UDP_PACKET_RELIABLE            (1 << 1)   //  reliable packet
#define UDP_PACKET_RELIABLE_HEAD       (1 << 2)   //  first packet of a reliable stream
#define UDP_PACKET_RELIABLE_TAIL       (1 << 3)   //  last packet of a reliable stream
#define UDP_PACKET_ACKNOWLEDGE         (1 << 4)   //  packet containing acknowledges for reliable packets
#define UDP_PACKET_CONNECT_REQUEST     (1 << 5)   //  connection request (connection at communication interface level)
#define UDP_PACKET_CONNECT_RESPONSE    (1 << 6)   //  connection reaponse (connection at communication interface level)
#define UDP_PACKET_UNRELIABLE_HEAD		 (1 << 7)   //  first packet of an unreliable stream
#define UDP_PACKET_UNRELIABLE_TAIL		 (1 << 8)   //  last packet of an unreliable stream 

// constants for CPacket::CanRetry() function - they describe the retry state of the packet
#define RS_NOW			0		// the packet should be resent immediately
#define RS_NOTNOW		1		// the packet should be resent at a later time
#define RS_NOTATALL 2		// the packet has reached the maximum number of retries - give up

class CAddress {
public:
  ULONG adr_ulAddress;   // host address
  UWORD adr_uwPort;      // host port
  UWORD adr_uwID;        // host id
  void MakeBroadcast(void);
  void Clear(void) {
    adr_ulAddress = 0;
    adr_uwPort = 0;
    adr_uwID = 0;
  }
};

/*
 * A class that contains a single UDP packet. 
 */
class CPacket {
public:
	ULONG	pa_ulSequence;			// Sequence number of this packet
	UWORD	pa_uwReliable;			// Is packet reliable or not
	SLONG	pa_slSize;				// Number of data bytes in packet (without header)
	SLONG	pa_slTransferSize;      // Number of data bytes in a data transfer unit this packet belongs to

	UBYTE	pa_ubRetryNumber;			// How many retries so far for this packet
	CTimerValue pa_tvSendWhen;	// When to try sending this packet (includes latency bandwidth limitations 
															// as well as retry intervals)

	UBYTE pa_pubPacketData[MAX_PACKET_SIZE];		// Packet header + actual data contained in the packet

	CListNode pa_lnListNode;					// used to create a linked list of packets - buffer

	 CAddress pa_adrAddress;				// packet address, port and client ID
  																
	// Constructors/destructors
	CPacket() { Clear(); }					// Default Constructor
	CPacket(CPacket &paOriginal);		// Copy constructor
	~CPacket() { Clear(); }

	// Reset all packet data and free allocated memory
	void Clear();

	// Write data to the packet and add header data
	BOOL WriteToPacket(void* pv,SLONG slSize,UWORD uwReliable,ULONG ulSequence,UWORD uwClientID,SLONG slTransferSize);
	// Write raw data to the packet and extract header data from the data
	BOOL WriteToPacketRaw(void* pv,SLONG slSize);
	// Read data from the packet (no header data)
	BOOL ReadFromPacket(void* pv,SLONG &slExpectedSize);

	// Is packet a part of a reliable stream?
	BOOL IsReliable();
	// Is packet a head of a reliable stream?
	BOOL IsReliableHead();
	// Is packet a tail of a reliable stream?
	BOOL IsReliableTail();
  // Is packet a part of an unreliable stream?
	BOOL IsUnreliable();
  // Is packet a head of an unreliable stream?
	BOOL IsUnreliableHead();
	// Is packet a tail of an unreliable stream?
	BOOL IsUnreliableTail();
  // Is  the packet from a broadcast address (pa_uwID not assigned)
  BOOL IsBroadcast();
	
	// Get the sequence number of the packet (must be reliable)
	ULONG GetSequence();
	// What is the current retry status?
	UBYTE CanRetry();
	// Drop the packet from the list
	void Drop();
	
  // get the size of data transfer unit this packet belongs to
  SLONG CPacket::GetTransferSize();

	// Copy operator
	void operator=(const CPacket &paOriginal);
  void operator delete(void *p);
};


// data used to limit bandwidth/lantency and calculate statistics in packet-buffers
class CPacketBufferStats {
public:
  FLOAT pbs_fLatencyLimit;    // minimum latency in seconds
  FLOAT pbs_fLatencyVariation;// additional latency variation
  FLOAT pbs_fBandwidthLimit;  // maximum bandwidth in bps (bits per second)
  CTimerValue pbs_tvTimeNextPacketStart; // next point in time free for data receiving


  void Clear(void);
  // get time when the packet will be allowed to leave the buffer
  CTimerValue GetPacketSendTime(SLONG slSize);
};


class CPacketBuffer {
public:
	ULONG pb_ulTotalSize;						// Total size of data in packets stored in this buffer (no headers)
	ULONG pb_ulLastSequenceOut;			// Sequence number of the last packet taken out of the buffer
	
	CListHead pb_lhPacketStorage;
	
	ULONG pb_ulNumOfPackets;					// Total number of packets currently in storage
	ULONG pb_ulNumOfReliablePackets;	// Number of reliable packets in storage (0 if no reliable stream in progress)

	CPacketBufferStats *pb_ppbsStats; // for bandwidth/latency emulation stats and limits
	CPacketBufferStats pb_pbsLimits;	// maximum output BPS for the buffer, to prevent client flooding

	CPacketBuffer() { Clear(); };
	~CPacketBuffer() { Clear(); };

	// Empty the packet buffer
	void Clear();
	// Is the packet buffer empty?
	BOOL IsEmpty();

	// Calculate when the packet can be output from the buffer
	CTimerValue GetPacketSendTime(SLONG slSize);

  // Get the size of all packets in the buffer, including headers
  ULONG GetTotalDataSize();
	// Adds a packet to the end of the packet buffer
	BOOL AppendPacket(CPacket &paPacket,BOOL bDelay,BOOL bFullCheck);
	// Inserts the packet in the buffer, according to it's sequence number
	BOOL InsertPacket(CPacket &paPacket,BOOL bDelay);
	// Bumps up the retry count and time, and appends the packet to the buffer
	BOOL Retry(CPacket &paPacket);
	// Reads the data from the first packet in the bufffer, but does not remove it
	CPacket* PeekFirstPacket();
	// Reads the first packet in the bufffer
	CPacket* GetFirstPacket();
	// Reads the data from the packet with the requested sequence, but does not remove it
	CPacket* PeekPacket(ULONG ulSequence);
	// Reads the packet with the requested sequence
	CPacket* GetPacket(ULONG ulSequence);
	// Reads the first connection request packet from the buffer
	CPacket* GetConnectRequestPacket();
	// Removes the first packet from the buffer
	BOOL RemoveFirstPacket(BOOL bDelete);
	// Removes the packet with the requested sequence from the buffer
	BOOL RemovePacket(ULONG ulSequence,BOOL bDelete);
	// Remove connect response packets from the buffer
  BOOL RemoveConnectResponsePackets();

  // Gets the number of packets in the buffer with a given sequence number
  ULONG GetSequenceRepeat(ULONG ulSequence);
	// Gets the sequence number of the first packet in the buffer
	ULONG GetFirstSequence();
	// Gets the sequence number of the last packet in the buffer
	ULONG GetLastSequence();
	// Is the packet with the given sequence in the buffer?
	BOOL IsSequenceInBuffer(ULONG ulSequence);
	// Check if the buffer contains a complete sequence of reliable packets	at the start of the buffer
	BOOL CheckSequence(SLONG &slSize,BOOL bReliable);
		
};

// Create new packet (use instead of new CPacket)
extern CPacket *CreatePacket(void);
// Delete packet (use instead of delete CPacket)
extern void DeletePacket(CPacket *ppaPacket);

#endif  /* include-once check. */
