#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Base/Translation.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Math/Functions.h>
#include <Engine/Base/Lists.h>
#include <Engine/Base/Memory.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Network/CPacket.h>
#include <Engine/Base/ObjectRestore.h>

#include <Engine/Base/Listiterator.inl>
#include <Engine/Templates/ReusableContainer.cpp>

// should the packet transfers in/out of the buffer be reported to the console
extern INDEX net_bReportPackets;
extern INDEX net_iMaxSendRetries;
extern FLOAT net_fSendRetryWait;

#define MAX_RETRIES 10
#define RETRY_INTERVAL 3.0f

#define HTONL(n)		{ n = htonl(n); }
#define HTONS(n)		{ n = htons(n); }
#define NTOHL(n)		{ n = ntohl(n); }
#define NTOHS(n)		{ n = ntohs(n); }

extern INDEX net_iUDPBlockSize = MAX_UDP_BLOCK_SIZE;
extern INDEX net_iMaxPacketSize = net_iUDPBlockSize + MAX_HEADER_SIZE;

// array of packets to reduce packet allocation and dealocations
CReusableContainer<CPacket> _rcPacketContainer(32);

// make the address broadcast
void CAddress::MakeBroadcast(void)
{
  adr_ulAddress = INADDR_BROADCAST;
  extern INDEX net_iPort;
  adr_uwPort = net_iPort;
  adr_uwID = 0;
}



/*
*
*	CPacket class implementation
*
*/

// copy constructor
CPacket::CPacket(CPacket &paOriginal) 
{

	ASSERT(paOriginal.pa_pubPacketData != NULL && paOriginal.pa_slSize > 0);

	pa_slSize = paOriginal.pa_slSize;
	pa_slTransferSize = paOriginal.pa_slTransferSize;

	pa_ulSequence = paOriginal.pa_ulSequence;
	pa_uwReliable = paOriginal.pa_uwReliable;
	pa_tvSendWhen = paOriginal.pa_tvSendWhen;
	pa_ubRetryNumber = paOriginal.pa_ubRetryNumber;
	pa_adrAddress.adr_ulAddress = paOriginal.pa_adrAddress.adr_ulAddress;
	pa_adrAddress.adr_uwPort = paOriginal.pa_adrAddress.adr_uwPort;
	pa_adrAddress.adr_uwID = paOriginal.pa_adrAddress.adr_uwID;

	memcpy(pa_pubPacketData,paOriginal.pa_pubPacketData,pa_slSize);

};

// initialization of the packet - clear all data and remove the packet from any list (buffer) it is in
void CPacket::Clear() 
{

	pa_slSize = 0;
	pa_slTransferSize = 0;
	pa_uwReliable = 0;
	pa_ubRetryNumber = 0;

  pa_adrAddress.Clear();

  if (_pTimer!=NULL) {
	  pa_tvSendWhen = CTimerValue(0.0f);
  }
	if(pa_lnListNode.IsLinked()) pa_lnListNode.Remove();
};

void CPacket::operator delete(void *p)
{
  // delete must be called from DeletePacket function
  if(!_rcPacketContainer.rc_bNowDeleting) {
    ASSERTALWAYS("Packet must be deleted with DeletePacket function");
    // Halt release version if not this isn't final version
    extern INDEX dbg_bAtHome;
    if(dbg_bAtHome) {
      _asm int 3;
    }
  }
//안태훈 수정 시작	//(DevPartner Bug Fix)(2005-01-13)
  ::operator delete(p);
//안태훈 수정 끝	//(DevPartner Bug Fix)(2005-01-13)
};

void CPacket::operator=(const CPacket &paOriginal)
{
	ASSERT(paOriginal.pa_pubPacketData != NULL && paOriginal.pa_slSize > 0);

	pa_slSize = paOriginal.pa_slSize;
  pa_slTransferSize = paOriginal.pa_slTransferSize;

	pa_ulSequence = paOriginal.pa_ulSequence;
	pa_uwReliable = paOriginal.pa_uwReliable;
	pa_tvSendWhen = paOriginal.pa_tvSendWhen;
	pa_ubRetryNumber = paOriginal.pa_ubRetryNumber;
	pa_adrAddress.adr_ulAddress = paOriginal.pa_adrAddress.adr_ulAddress;
	pa_adrAddress.adr_uwPort = paOriginal.pa_adrAddress.adr_uwPort;
	pa_adrAddress.adr_uwID = paOriginal.pa_adrAddress.adr_uwID;

	memcpy(pa_pubPacketData,paOriginal.pa_pubPacketData,pa_slSize);

};


// Takes data from a pointer, adds the packet header and copies the data to the packet
BOOL CPacket::WriteToPacket(void* pv,SLONG slSize,UWORD uwReliable,ULONG ulSequence,UWORD uwClientID,SLONG slTransferSize) 
{
	UBYTE* pubData;

	ASSERT(slSize <= net_iUDPBlockSize && slSize > 0);
	ASSERT(pv != NULL);
	ASSERT(slTransferSize >= slSize);

	// set packet properties to values received as parameters
	pa_uwReliable = uwReliable;
	pa_adrAddress.adr_uwID = uwClientID;
	pa_ulSequence = ulSequence;
	pa_slSize = slSize + MAX_HEADER_SIZE;
	pa_slTransferSize = slTransferSize;

	// insert packet header to the beginning of the packet data
	pubData = pa_pubPacketData;
	*(UWORD*)pubData = pa_uwReliable;
	pubData+=sizeof(pa_uwReliable);

	*(ULONG*)pubData = htonl(pa_ulSequence);
	pubData+=sizeof(pa_ulSequence);

	*(UWORD*)pubData = pa_adrAddress.adr_uwID;
	pubData+=sizeof(pa_adrAddress.adr_uwID);
  
	//0524 kwon
  *(SLONG*)pubData = htonl(pa_slTransferSize);
	pubData+=sizeof(pa_slTransferSize);

	// copy the data the packet is to contain
	memcpy(pubData,pv,slSize);

	

	return TRUE;

};


// Takes data from a pointer, reads the packet header and copies the data to the packet
BOOL CPacket::WriteToPacketRaw(void* pv,SLONG slSize) 
{
	UBYTE* pubData;

	ASSERT(slSize <= MAX_PACKET_SIZE && slSize > 0);
	if(slSize > MAX_PACKET_SIZE || slSize <= 0) return FALSE;
	ASSERT(pv != NULL);
	if(pv == NULL) return FALSE;

	// get the packet properties from the pointer, and set the values
	pubData = (UBYTE*)pv;
	pa_uwReliable = *(UWORD*)pubData;
	pubData+=sizeof(pa_uwReliable);
	pa_ulSequence = *(ULONG*)pubData;
	pubData+=sizeof(pa_ulSequence);
	pa_adrAddress.adr_uwID = *(UWORD*)pubData;
	pubData+=sizeof(pa_adrAddress.adr_uwID);
	pa_slTransferSize = *(SLONG*)pubData;
	pubData+=sizeof(pa_slTransferSize);
	
	NTOHS(pa_uwReliable);
	NTOHL(pa_ulSequence);
	NTOHS(pa_adrAddress.adr_uwID);
	NTOHL(pa_slTransferSize);

	pa_slSize = slSize;

	// transfer the data to the packet
	memcpy(pa_pubPacketData,pv,pa_slSize);

	return TRUE;

};


// Copies the data from the packet to the location specified by the *pv. 
// packet header data is skipped
BOOL CPacket::ReadFromPacket(void* pv,SLONG &slExpectedSize) 
{
	UBYTE* pubData;

	ASSERT(slExpectedSize > 0);
	ASSERT(pv != NULL);
	ASSERT(pa_pubPacketData != NULL);

	if (slExpectedSize < (pa_slSize - MAX_HEADER_SIZE)) {
		return FALSE;
	}
	
	// how much data is actually returned
	slExpectedSize = pa_slSize - MAX_HEADER_SIZE;

	// skip the header data
	pubData = pa_pubPacketData + sizeof(pa_uwReliable) + sizeof(pa_ulSequence) + sizeof(pa_adrAddress.adr_uwID) + sizeof(pa_slTransferSize);
	
	memcpy(pv,pubData,slExpectedSize);

	return TRUE;

};

// is the packet reliable?
BOOL CPacket::IsReliable() 
{
	return ((pa_uwReliable & UDP_PACKET_RELIABLE) != 0);
};

// is the packet a head of a reliable stream
BOOL CPacket::IsReliableHead() 
{
	return pa_uwReliable & UDP_PACKET_RELIABLE_HEAD;
};

// is the packet a tail of a reliable stream
BOOL CPacket::IsReliableTail() 
{
	return pa_uwReliable & UDP_PACKET_RELIABLE_TAIL;
};

// is the packet reliable?
BOOL CPacket::IsUnreliable() 
{
	return ((pa_uwReliable & UDP_PACKET_UNRELIABLE) != 0);
};

// is the packet a head of a reliable stream
BOOL CPacket::IsUnreliableHead() 
{
	return ((pa_uwReliable & UDP_PACKET_UNRELIABLE_HEAD) != 0);
};

// is the packet a tail of a reliable stream
BOOL CPacket::IsUnreliableTail() 
{
	return ((pa_uwReliable & UDP_PACKET_UNRELIABLE_TAIL) != 0);
};

// return the sequence of a packet
ULONG CPacket::GetSequence()
{
	ASSERT(pa_uwReliable);
	return pa_ulSequence;
};

// return the retry status of a packet - can retry now, later or not at all 
UBYTE CPacket::CanRetry()
{
    //! 이 패킷의 전송 재시도가 최대치를 넘었다면, 보내지 말아야지.
	if (pa_ubRetryNumber >= net_iMaxSendRetries) {
		return RS_NOTATALL;
	}

	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
    //! 시간이 안되었으니 아직 보내면 안된다.
	if (tvNow < (pa_tvSendWhen + CTimerValue(net_fSendRetryWait * (pa_ubRetryNumber + 1)))) {
		return RS_NOTNOW;
	}

	return RS_NOW;

};

//drop the packet from a list (buffer)
void CPacket::Drop()
{
	if (pa_lnListNode.IsLinked()) {
		pa_lnListNode.Remove();
	}
};

SLONG CPacket::GetTransferSize() 
{
  return pa_slTransferSize;
};

BOOL CPacket::IsBroadcast() 
{
  if (pa_adrAddress.adr_uwID == '//' || pa_adrAddress.adr_uwID == 0) {
    return TRUE;
  }

  return FALSE;

};


/*
*
*
*	CPacketBufferStats Class implementation
*
*
*/

// this class is used for MaxBPS limitation (prevets flooding the client) and for bandwidth limit and latency emulation
void CPacketBufferStats::Clear(void)
{

	pbs_fBandwidthLimit = 0;
	pbs_fLatencyLimit = 0.0f;
	pbs_fLatencyVariation = 0.0f;
	pbs_tvTimeNextPacketStart = _pTimer->GetHighPrecisionTimer();

};

// when can a certian ammount of data be sent?
CTimerValue CPacketBufferStats::GetPacketSendTime(SLONG slSize) 
{
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();

  // calculate how much should the packet be delayed due to latency and due to bandwidth
  CTimerValue tvBandwidth;
  if (pbs_fBandwidthLimit<=0.0f) {
    tvBandwidth = CTimerValue(0.0);
  } else {
    tvBandwidth = CTimerValue(DOUBLE((slSize*8)/pbs_fBandwidthLimit));
  }
  CTimerValue tvLatency;
  if (pbs_fLatencyLimit<=0.0f && pbs_fLatencyVariation<=0.0f) {
   tvLatency = CTimerValue(0.0);
  } else {
   tvLatency = CTimerValue(DOUBLE(pbs_fLatencyLimit+(pbs_fLatencyVariation*rand())/RAND_MAX));
  }

  // time when the packet should be sent is max of
  CTimerValue tvStart(
    Max(
      // current time plus latency and
      (tvNow+tvLatency).tv_llValue,
      // next free point in time
      pbs_tvTimeNextPacketStart.tv_llValue));
  // remember next free time and return it
  pbs_tvTimeNextPacketStart = tvStart+tvBandwidth;
  return pbs_tvTimeNextPacketStart;

};




/*
*
*	CPacketBuffer Class implementation
*
*/


// Empty the packet buffer
void CPacketBuffer::Clear() 
{

  FORDELETELIST(CPacket,pa_lnListNode,pb_lhPacketStorage,litPacketIter) {
    CPacket* ppaPacket = litPacketIter;
    DeletePacket(ppaPacket);
  }

	pb_lhPacketStorage.Clear();
	
	pb_ulNumOfPackets = 0;
	pb_ulNumOfReliablePackets = 0;
	pb_ulTotalSize = 0;
	pb_ulLastSequenceOut = 0;

	pb_pbsLimits.Clear();

};


// Is the packet buffer empty?
BOOL CPacketBuffer::IsEmpty() 
{

	if (pb_ulNumOfPackets>0) {
		return FALSE;
	}

	return TRUE;
};

// Get the size of all packets in the buffer, including headers
ULONG CPacketBuffer::GetTotalDataSize()
{
  return pb_ulTotalSize + pb_ulNumOfPackets*MAX_HEADER_SIZE;
};


// Calculate when the packet can be output from the buffer
CTimerValue CPacketBuffer::GetPacketSendTime(SLONG slSize) {
	CTimerValue tvSendTime;
  CTimerValue tvBufferSendTime,tvClientSendTime,tvNewSendTime;
	
  tvSendTime = pb_pbsLimits.pbs_tvTimeNextPacketStart;

  	// if traffic emulation is in use, use the greater time
	if (pb_ppbsStats != NULL) {
    tvBufferSendTime = pb_ppbsStats->GetPacketSendTime(slSize);
    tvClientSendTime = pb_pbsLimits.GetPacketSendTime(slSize);
    if (tvBufferSendTime < tvClientSendTime) {
      tvNewSendTime = tvClientSendTime;
    } else {
      tvNewSendTime = tvBufferSendTime;
    }
		pb_pbsLimits.pbs_tvTimeNextPacketStart = tvNewSendTime;
		pb_ppbsStats->pbs_tvTimeNextPacketStart = tvNewSendTime;
	// else just use the MaxBPS control
	} else {
		pb_pbsLimits.GetPacketSendTime(slSize);
	}

	return tvSendTime;
};

// Adds the packet to the end of the list
// bFullCheck is not set for internal and outgoing buffers, it should only
// be used for incoming buffers
BOOL CPacketBuffer::AppendPacket(CPacket &paPacket,BOOL bDelay,BOOL bFullCheck) 
{

	// bDelay regulates if the packet should be delayed because of the bandwidth limits or not
	// internal buffers (reliable, waitack and master buffers) do not pay attention to bandwidth limits
	if (bDelay) {
		paPacket.pa_tvSendWhen = GetPacketSendTime(paPacket.pa_slSize);
	} else {
		paPacket.pa_tvSendWhen = _pTimer->GetHighPrecisionTimer();
	}

  if (bFullCheck) {
    // if unreliable body or tail packet
    if (paPacket.IsUnreliable() && !paPacket.IsUnreliableHead()) {
      // if the buffer is empty, drop the packet
      if (pb_lhPacketStorage.Count() == 0) {
        if (net_bReportPackets) {
          CPrintF(TRANS("Unreliable packet out of order - packet rejected.\n"));
        }
        return FALSE;
      }

      // if it is not a direct successor of the last packet in the buffer (by sequence), drop it
      FOREACHINLIST_R(CPacket,pa_lnListNode,pb_lhPacketStorage,litPacketIter) {      
        CPacket* ppaPacket = litPacketIter;
        if ((paPacket.pa_ulSequence - ppaPacket->pa_ulSequence) != 1) {
          if (net_bReportPackets) {
            CPrintF(TRANS("Unreliable packet out of order - packet rejected.\n"));
          }
          return FALSE;
        }
        break;
      }
      // if it is anything else
    } else {
      if (!pb_lhPacketStorage.IsEmpty()) {
        // drop any unreliable body and head messages at the end of the buffer
        FORDELETELIST_R(CPacket,pa_lnListNode,pb_lhPacketStorage,litPacketIter) {
          CPacket* ppaPacket = litPacketIter;
          if (ppaPacket->IsUnreliable() && !ppaPacket->IsUnreliableTail()) {
            ppaPacket->pa_lnListNode.Remove();

            // update the packet count and total size of messages stored in the buffer
            pb_ulNumOfPackets--;
            if (litPacketIter->pa_uwReliable & UDP_PACKET_RELIABLE) {
              pb_ulNumOfReliablePackets--;
            }
            // update the total size of data stored in the buffer
            pb_ulTotalSize -= (litPacketIter->pa_slSize - MAX_HEADER_SIZE);
            if (net_bReportPackets) {
              CPrintF(TRANS("Packets received out of order - deleting uncompleted unreliable stream from buffer.\n"));
            }
            DeletePacket(ppaPacket);
          } else {
            break;
          }
        }
      }
    }
  }

	// Add the packet to the end of the list
	pb_lhPacketStorage.AddTail(paPacket.pa_lnListNode);
	pb_ulNumOfPackets++;

	// if the packet is reliable, bump up the number of reliable packets
	if (paPacket.pa_uwReliable & UDP_PACKET_RELIABLE) {
		pb_ulNumOfReliablePackets++;
	}

	// update the total size of data stored in the buffer
	pb_ulTotalSize += paPacket.pa_slSize - MAX_HEADER_SIZE;
	return TRUE;

};

// Inserts the packet in the buffer, according to it's sequence number
BOOL CPacketBuffer::InsertPacket(CPacket &paPacket,BOOL bDelay) 
{
	// find the right place to insert this packet (this is if this packet is out of sequence)
	FOREACHINLIST(CPacket,pa_lnListNode,pb_lhPacketStorage,litPacketIter) {
		// if there is a packet in the buffer with greater sequence number, insert this one before it
		if (paPacket.pa_ulSequence < litPacketIter->pa_ulSequence) {

			// bDelay regulates if the packet should be delayed because of the bandwidth limits or not
			// internal buffers (reliable, waitack and master buffers) do not pay attention to bandwidth limits
			if (bDelay) {
				paPacket.pa_tvSendWhen = GetPacketSendTime(paPacket.pa_slSize);
			} else {
				paPacket.pa_tvSendWhen = _pTimer->GetHighPrecisionTimer();
			}

			litPacketIter.InsertBeforeCurrent(paPacket.pa_lnListNode);
			pb_ulNumOfPackets++;

			// if the packet is reliable, bump up the number of reliable packets
			if (paPacket.pa_uwReliable & UDP_PACKET_RELIABLE) {
				pb_ulNumOfReliablePackets++;
			}

			// update the total size of data stored in the buffer
			pb_ulTotalSize += paPacket.pa_slSize - MAX_HEADER_SIZE;

      
			return TRUE;

		// if there already is a packet in the buffer with the same sequence, do nothing
		} else if (paPacket.pa_ulSequence == litPacketIter->pa_ulSequence) {
			return FALSE;
		}
	}

	// if this packet has the greatest sequence number so far, add it to the end of the list
	pb_lhPacketStorage.AddTail(paPacket.pa_lnListNode);
	pb_ulNumOfPackets++;

  
	// if the packet is reliable, bump up the number of reliable packets
	if (paPacket.pa_uwReliable & UDP_PACKET_RELIABLE) {
		pb_ulNumOfReliablePackets++;
	}

	pb_ulTotalSize += paPacket.pa_slSize - MAX_HEADER_SIZE;

	return TRUE;
 
};


// Bumps up the retry count and time, and appends the packet to the buffer
BOOL CPacketBuffer::Retry(CPacket &paPacket) 
{
	paPacket.pa_ubRetryNumber++;

	if (net_bReportPackets)	{
		CPrintF("Retrying sequence: %d, reliable flag: 0x%X\n",paPacket.pa_ulSequence,paPacket.pa_uwReliable);
	}

	return AppendPacket(paPacket,TRUE,FALSE);
};

// Reads the data from the first packet in the bufffer, but does not remove it
CPacket* CPacketBuffer::PeekFirstPacket()
{
	ASSERT(pb_ulNumOfPackets != 0);
	return LIST_HEAD(pb_lhPacketStorage,CPacket,pa_lnListNode);
};

// Reads the first packet in the bufffer, and removes it from the buffer
CPacket* CPacketBuffer::GetFirstPacket()
{
	ASSERT(pb_ulNumOfPackets != 0);
	
	CPacket* ppaHead = LIST_HEAD(pb_lhPacketStorage,CPacket,pa_lnListNode);

	// remove the first packet from the start of the list
	pb_lhPacketStorage.RemHead();
	pb_ulNumOfPackets--;
	if (ppaHead->pa_uwReliable & UDP_PACKET_RELIABLE) {
		pb_ulNumOfReliablePackets--;
	}

	// update the total size of data stored in the buffer
	pb_ulTotalSize -= (ppaHead->pa_slSize - MAX_HEADER_SIZE);

	// mark the last packet sequence that was output from the buffer - helps to prevent problems wit duplicated packets	
	if (pb_ulLastSequenceOut < ppaHead->pa_ulSequence) {
		pb_ulLastSequenceOut = ppaHead->pa_ulSequence;		
	}

	return ppaHead;

};

// Reads the data from the packet with the requested sequence, but does not remove it
CPacket* CPacketBuffer::PeekPacket(ULONG ulSequence)
{
	FOREACHINLIST(CPacket,pa_lnListNode,pb_lhPacketStorage,litPacketIter) {
		if (litPacketIter->pa_ulSequence == ulSequence) {
			return litPacketIter;
		}
	}
	return NULL;
};

// Returns te packet with the matching sequence from the buffer
CPacket* CPacketBuffer::GetPacket(ULONG ulSequence)
{
	
	FOREACHINLIST(CPacket,pa_lnListNode,pb_lhPacketStorage,litPacketIter) {
		if (litPacketIter->pa_ulSequence == ulSequence) {
			litPacketIter->pa_lnListNode.Remove();

			pb_ulNumOfPackets--;
			if (litPacketIter->pa_uwReliable & UDP_PACKET_RELIABLE) {
				pb_ulNumOfReliablePackets--;
			}

			// update the total size of data stored in the buffer
			pb_ulTotalSize -= (litPacketIter->pa_slSize - MAX_HEADER_SIZE);

			return litPacketIter;
		}
	}
	return NULL;
};

// Reads the first connection request packet from the buffer
CPacket* CPacketBuffer::GetConnectRequestPacket() {
		FOREACHINLIST(CPacket,pa_lnListNode,pb_lhPacketStorage,litPacketIter) {
		if (litPacketIter->pa_uwReliable & UDP_PACKET_CONNECT_REQUEST) {
			litPacketIter->pa_lnListNode.Remove();

			pb_ulNumOfPackets--;
			// connect request packets are allways reliable
			pb_ulNumOfReliablePackets--;

			// update the total size of data stored in the buffer
			pb_ulTotalSize -= (litPacketIter->pa_slSize - MAX_HEADER_SIZE);

			return litPacketIter;
		}
	}
	return NULL;
};

// Removes the first packet from the buffer
BOOL CPacketBuffer::RemoveFirstPacket(BOOL bDelete) {
	ASSERT(pb_ulNumOfPackets > 0);
	CPacket *lnHead = LIST_HEAD(pb_lhPacketStorage,CPacket,pa_lnListNode);

	pb_ulNumOfPackets--;
	if (lnHead->pa_uwReliable & UDP_PACKET_RELIABLE) {
		pb_ulNumOfReliablePackets--;
	}

	// update the total size of data stored in the buffer
	pb_ulTotalSize -= (lnHead->pa_slSize - MAX_HEADER_SIZE);

	if (pb_ulLastSequenceOut < lnHead->pa_ulSequence) {
		pb_ulLastSequenceOut = lnHead->pa_ulSequence;		
	}

	pb_lhPacketStorage.RemHead();
	if (bDelete) {
		DeletePacket(lnHead);
	}
	return TRUE;
};

// Removes the packet with the requested sequence from the buffer
BOOL CPacketBuffer::RemovePacket(ULONG ulSequence,BOOL bDelete)
{
//	ASSERT(pb_ulNumOfPackets > 0);
	FORDELETELIST(CPacket,pa_lnListNode,pb_lhPacketStorage,litPacketIter) {
		if (litPacketIter->pa_ulSequence == ulSequence) {
			litPacketIter->pa_lnListNode.Remove();
			
			pb_ulNumOfPackets--;
			if (litPacketIter->pa_uwReliable & UDP_PACKET_RELIABLE) {
				pb_ulNumOfReliablePackets--;
			}

			// update the total size of data stored in the buffer
			pb_ulTotalSize -= (litPacketIter->pa_slSize - MAX_HEADER_SIZE);

			if (bDelete) {
				DeletePacket(litPacketIter);
			}
		}
	}
	return FALSE;
};

// Remove connect response packets from the buffer
BOOL CPacketBuffer::RemoveConnectResponsePackets() {
		FORDELETELIST(CPacket,pa_lnListNode,pb_lhPacketStorage,litPacketIter) {
		if (litPacketIter->pa_uwReliable & UDP_PACKET_CONNECT_RESPONSE) {
			litPacketIter->pa_lnListNode.Remove();

			pb_ulNumOfPackets--;
			
			// connect request packets are allways reliable
			pb_ulNumOfReliablePackets--;

			// update the total size of data stored in the buffer
			pb_ulTotalSize -= (litPacketIter->pa_slSize - MAX_HEADER_SIZE);

			DeletePacket(litPacketIter);
		}
	}
	return NULL;
};


// Gets the number of packets in the buffer with a given sequence number
ULONG CPacketBuffer::GetSequenceRepeat(ULONG ulSequence)
{
  ULONG ulCount = 0;
	FOREACHINLIST(CPacket,pa_lnListNode,pb_lhPacketStorage,litPacketIter) {
		if (litPacketIter->pa_ulSequence == ulSequence) {
			ulCount++;
		}
	}
  return ulCount;
};

// Gets the sequence number of the first packet in the buffer
ULONG CPacketBuffer::GetFirstSequence()
{
	ASSERT(pb_ulNumOfPackets > 0);

	return LIST_HEAD(pb_lhPacketStorage,CPacket,pa_lnListNode)->pa_ulSequence;

};

// Gets the sequence number of the last packet in the buffer
ULONG CPacketBuffer::GetLastSequence()
{
	ASSERT(pb_ulNumOfPackets > 0);

	return LIST_TAIL(pb_lhPacketStorage,CPacket,pa_lnListNode)->pa_ulSequence;

};

// Removes the packet with the requested sequence from the buffer
BOOL CPacketBuffer::IsSequenceInBuffer(ULONG ulSequence)
{
	FOREACHINLIST(CPacket,pa_lnListNode,pb_lhPacketStorage,litPacketIter) {
		if (litPacketIter->pa_ulSequence == ulSequence) {
			return TRUE;
		}
	}
	return FALSE;
};

//! 버퍼의 시작점에서 이 버퍼에 완전한 시퀀스의  (릴라이어블)패킷이 담겨있는지 체크한다.
//! 버퍼 시작점에 (릴라이어블) 패킷의 헤더가 있다면, 바디와 테일이 꼭 존재해야 한다.
// Check if the buffer contains a complete sequence of reliable packets	at the start of the buffer
BOOL CPacketBuffer::CheckSequence(SLONG &slSize,BOOL bReliable)
{

	CPacket* paPacket;
	ULONG ulSequence;
  ULONG ulHead,ulBody,ulTail;

  if (bReliable) {
    ulHead = UDP_PACKET_RELIABLE_HEAD;
    ulBody = UDP_PACKET_RELIABLE;
    ulTail = UDP_PACKET_RELIABLE_TAIL;
  } else {
    ulHead = UDP_PACKET_UNRELIABLE_HEAD;
    ulBody = UDP_PACKET_UNRELIABLE;
    ulTail = UDP_PACKET_UNRELIABLE_TAIL;
  }

  slSize=0;

	if (pb_ulNumOfPackets == 0) {  
		return FALSE;
	}

	paPacket = LIST_HEAD(pb_lhPacketStorage,CPacket,pa_lnListNode);
	//! 첫 패킷이 패킷 헤더가 아니라면 FALSE 리턴.
	// if the first packet is not the head of the reliable packet transfer
	if (!(paPacket->pa_uwReliable & ulHead)) {
		return FALSE;
	}

	ulSequence = paPacket->pa_ulSequence;
	//!헤더는 찾았으니 나머지를 버퍼안에서 찾아보자.
	//! head,body,tail의 시퀀스는 서로 연결되어있다.
	// for each packet in the buffer
	FOREACHINLIST(CPacket,pa_lnListNode,pb_lhPacketStorage,litPacketIter) {
		// if it's out of order (there is a gap in the reliable sequence), the message is not complete
		//! 첫패킷의 시퀀스가 다르다면 False.
		if (litPacketIter->pa_ulSequence != ulSequence) {
			return FALSE;
		}
		//! 잘못된 타입이라면,(바디가 아니라면) FALSE 리턴.
		// if it's of a wrong type (unreliable/reliable)
		if (!(litPacketIter->pa_uwReliable & ulBody)) {
			return FALSE;
		}
		//! 테일이 존재한다면 TRUE 리턴 
		// if it's a tail of the reliable sequence the message is complete (all packets so far
		// have been in order)
		if (litPacketIter->pa_uwReliable & ulTail) {
			return TRUE;
		}
		slSize += litPacketIter->pa_slSize - MAX_HEADER_SIZE;
		//! 시퀀스 증가.
		ulSequence++;
	}

	// if the function hasn't exited while in the loop, the message is not complete
	// (all the packets are in sequence, but there is no tail)
	return FALSE;
};

// Create new packet
extern CPacket *CreatePacket(void)
{
  TRACKMEM(Mem,"Network");
  return _rcPacketContainer.CreateObject();
}

// Delete packet
extern void DeletePacket(CPacket *ppaPacket)
{
  TRACKMEM(Mem,"Network");
  CObjectRestore<BOOL> or(_rcPacketContainer.rc_bNowDeleting,TRUE);
  _rcPacketContainer.DeleteObject(ppaPacket);
}
