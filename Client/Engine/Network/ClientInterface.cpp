#include "stdh.h"

#include <Engine/Base/CTString.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/ErrorTable.h>
#include <Engine/Base/Lists.h>
#include <Engine/Base/Stream.h>
#include <Engine/Base/Translation.h>
#include <Engine/Base/MemoryTracking.h>

#include <Engine/Network/ClientInterface.h>
#include <Engine/Network/CPacket.h>

#include <Engine/Base/Listiterator.inl>

#include <Engine/Network/CommunicationInterface.h>

#include <Engine/Network/CNetwork.h>
#include <Engine/GlobalDefinition.h>

// how many acknowledges can fit into one UDP packet
//#define MAX_ACKS_PER_PACKET (net_iUDPBlockSize/sizeof(ULONG))

extern FLOAT net_fDropPackets;
extern INDEX net_bReportPackets;
//0105
extern BOOL _bTcp4ClientInterface = FALSE;   // 클라이언트의 네트웍 인터페이스가 TCP인지 UDP인지 구분하는 인ㅏ.

CClientInterface::CClientInterface(void)
{
  TRACKMEM(Mem,"Network");
  Clear();
};

CClientInterface::~CClientInterface(void)
{
  Clear();
};


void CClientInterface::Clear(void) 
{
	ci_bUsed = FALSE;

	ci_bReliableComplete = FALSE;
	ci_pbInputBuffer.Clear();
	ci_pbOutputBuffer.Clear();
	ci_pbReliableInputBuffer.Clear();
	ci_pbWaitAckBuffer.Clear();

	ci_adrAddress.Clear();
	ci_strAddress = "";
	
	ci_pciOther = NULL;
	ci_ulSequence = 0;
};

// Get the size of all packets in the output buffers, including headers
ULONG CClientInterface::GetPendingDataSize()
{
  if (!ci_bUsed) {
    return 0;
  }
  return ci_pbOutputBuffer.GetTotalDataSize() + ci_pbWaitAckBuffer.GetTotalDataSize();
};

// mark the client interface as local for this computer
void CClientInterface::SetLocal(CClientInterface *pciOther)
{
  Clear();

	ci_bUsed = TRUE;

  ci_bClientLocal = TRUE;
  ci_pciOther = pciOther;
  if (pciOther!=NULL) {
    pciOther->ci_pciOther = this;
  }

	ci_adrAddress.Clear();
		
};

//0311 kwon함수추가. 마스터 버퍼로 직접 패킷 보내기.
// send a message through this client interface
void CClientInterface::SendNew(const void *pvSend, SLONG slSize)
{
	//0524 kwon 임시 삭제.
	//ASSERT (ci_bUsed);
	ASSERT(pvSend != NULL && slSize>0);
	
	UWORD uwPacketReliable;
	UBYTE* pubData;
	SLONG slSizeToSend;
	SLONG slTransferSize;
	ULONG ulSequence;
	CPacket* ppaNewPacket;
	ULONG ulHead,ulBody,ulTail;
	
	
    ulHead = UDP_PACKET_UNRELIABLE_HEAD;
    ulBody = UDP_PACKET_UNRELIABLE;
    ulTail = UDP_PACKET_UNRELIABLE_TAIL;
	
	
	// make sure the first packet is marked as a head of the message
	uwPacketReliable = ulBody | ulHead;
	if (slSize <= net_iUDPBlockSize) {
		uwPacketReliable |= ulTail;
	}
	
	
	pubData = (UBYTE*) pvSend;
	slSizeToSend = slSize;
	slTransferSize = slSizeToSend;
	
	
	//0707
#ifdef SECRET_KEY
	if(_pNetwork->MyCharacterInfo.secretkey != 0)
	{
		int seckey = ((_pNetwork->MyCharacterInfo.job + _pNetwork->MyCharacterInfo.secretkey) << 1);

		ci_adrAddress.adr_uwID = seckey;
	}
#endif

	// split large reliable messages into packets, and put them in the output buffer
	while (slSizeToSend>net_iUDPBlockSize) {
		ppaNewPacket = CreatePacket();
		
		// for each packet, increment the sequence (very important)
		ulSequence = (++ci_ulSequence);
		ppaNewPacket->WriteToPacket(pubData,net_iUDPBlockSize,uwPacketReliable,ulSequence,ci_adrAddress.adr_uwID,slTransferSize);
		ppaNewPacket->pa_adrAddress.adr_ulAddress = ci_adrAddress.adr_ulAddress;
		ppaNewPacket->pa_adrAddress.adr_uwPort = ci_adrAddress.adr_uwPort;
		ppaNewPacket->pa_adrAddress.adr_uwID = ci_adrAddress.adr_uwID;
		//0309 마스터버퍼로 바로 보낸다. send to real server
		if (_cmiComm.cci_pbMasterOutput.AppendPacket(*ppaNewPacket,TRUE,FALSE) == FALSE) {
			ASSERT(FALSE);
		}
		
		// turn off udp head flag, if exists (since we just put a packet in the output buffer, the next 
		// packet cannot be the head
		uwPacketReliable &= ulBody;
		
		slSizeToSend -=  net_iUDPBlockSize;
		pubData += net_iUDPBlockSize;
	}
	
	// what remains is a tail of a message, or an unreliable packet
	uwPacketReliable |= ulTail;
	
	// so send it
	ppaNewPacket = CreatePacket();
	
	ulSequence = _pNetwork->getSeq();
	ppaNewPacket->WriteToPacket(pubData,slSizeToSend,uwPacketReliable,ulSequence,ci_adrAddress.adr_uwID,slTransferSize);
	ppaNewPacket->pa_adrAddress.adr_ulAddress = ci_adrAddress.adr_ulAddress;
	ppaNewPacket->pa_adrAddress.adr_uwPort = ci_adrAddress.adr_uwPort;
	ppaNewPacket->pa_adrAddress.adr_uwID = ci_adrAddress.adr_uwID;
	if (_cmiComm.cci_pbMasterOutput.AppendPacket(*ppaNewPacket,TRUE,FALSE) == FALSE) {
		ASSERT(FALSE);
	}      
};


// send a message through this client interface
void CClientInterface::Send(const void *pvSend, SLONG slSize,BOOL bReliable)
{
	ASSERT (ci_bUsed);
	ASSERT(pvSend != NULL && slSize>0);

  UWORD uwPacketReliable;
	UBYTE* pubData;
	SLONG slSizeToSend;
  SLONG slTransferSize;
	ULONG ulSequence;
	CPacket* ppaNewPacket;
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

	// make sure the first packet is marked as a head of the message
	uwPacketReliable = ulBody | ulHead;
	if (slSize <= net_iUDPBlockSize) {
		uwPacketReliable |= ulTail;
	}

 
	pubData = (UBYTE*) pvSend;
	slSizeToSend = slSize;
  slTransferSize = slSizeToSend;
	
	
	// split large reliable messages into packets, and put them in the output buffer
	while (slSizeToSend>net_iUDPBlockSize) {
		ppaNewPacket = CreatePacket();

		// for each packet, increment the sequence (very important)
		ulSequence = (++ci_ulSequence);
		ppaNewPacket->WriteToPacket(pubData,net_iUDPBlockSize,uwPacketReliable,ulSequence,ci_adrAddress.adr_uwID,slTransferSize);
		ppaNewPacket->pa_adrAddress.adr_ulAddress = ci_adrAddress.adr_ulAddress;
		ppaNewPacket->pa_adrAddress.adr_uwPort = ci_adrAddress.adr_uwPort;
		ppaNewPacket->pa_adrAddress.adr_uwID = ci_adrAddress.adr_uwID;
    if (ci_pbOutputBuffer.AppendPacket(*ppaNewPacket,TRUE,FALSE) == FALSE) {
      ASSERT(FALSE);
    }

		// turn off udp head flag, if exists (since we just put a packet in the output buffer, the next 
		// packet cannot be the head
		uwPacketReliable &= ulBody;

		slSizeToSend -=  net_iUDPBlockSize;
		pubData += net_iUDPBlockSize;
	}

	// what remains is a tail of a message, or an unreliable packet
  uwPacketReliable |= ulTail;
	
	// so send it
	ppaNewPacket = CreatePacket();

	ulSequence = (++ci_ulSequence);
	ppaNewPacket->WriteToPacket(pubData,slSizeToSend,uwPacketReliable,ulSequence,ci_adrAddress.adr_uwID,slTransferSize);
	ppaNewPacket->pa_adrAddress.adr_ulAddress = ci_adrAddress.adr_ulAddress;
	ppaNewPacket->pa_adrAddress.adr_uwPort = ci_adrAddress.adr_uwPort;
	ppaNewPacket->pa_adrAddress.adr_uwID = ci_adrAddress.adr_uwID;
  if (ci_pbOutputBuffer.AppendPacket(*ppaNewPacket,TRUE,FALSE) == FALSE) {
    ASSERT(FALSE);
  }    

  
};

// send a message through this client interface, to the provided address
void CClientInterface::SendTo(const void *pvSend, SLONG slSize,const CAddress adrAdress,BOOL bReliable)
{
	ASSERT (ci_bUsed);
	ASSERT(pvSend != NULL && slSize>0);

	UWORD uwPacketReliable;
	UBYTE* pubData;
	SLONG slSizeToSend;
  SLONG slTransferSize;
	ULONG ulSequence;
	CPacket* ppaNewPacket;
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

	// make sure the first packet is marked as a head of the message
	uwPacketReliable = ulBody | ulHead;
	if (slSize <= net_iUDPBlockSize) {
		uwPacketReliable |= ulTail;
	}


	pubData = (UBYTE*) pvSend;
	slSizeToSend = slSize;
  slTransferSize = slSizeToSend;
	

	// split large reliable messages into packets, and put them in the output buffer
	while (slSizeToSend>net_iUDPBlockSize) {
		ppaNewPacket = CreatePacket();

		// for each packet, increment the sequence (very important)
		ulSequence = (++ci_ulSequence);
		ppaNewPacket->WriteToPacket(pubData,net_iUDPBlockSize,uwPacketReliable,ulSequence,adrAdress.adr_uwID,slTransferSize);
		ppaNewPacket->pa_adrAddress.adr_ulAddress = adrAdress.adr_ulAddress;
		ppaNewPacket->pa_adrAddress.adr_uwPort = adrAdress.adr_uwPort;
		ppaNewPacket->pa_adrAddress.adr_uwID = adrAdress.adr_uwID;
    if(ci_pbOutputBuffer.AppendPacket(*ppaNewPacket,TRUE,FALSE) == FALSE) {
      ASSERT(FALSE);
    }
		// turn off udp head flag, if exists (since we just put a packet in the output buffer, the next 
		// packet cannot be the head
		uwPacketReliable &= ulBody;

		slSizeToSend -=  net_iUDPBlockSize;
		pubData += net_iUDPBlockSize;
	}

	// what remains is a tail of a reliable message, or an unreliable packet
	uwPacketReliable |= ulTail;

	ppaNewPacket = CreatePacket();

	ulSequence = (++ci_ulSequence);
	ppaNewPacket->WriteToPacket(pubData,slSizeToSend,uwPacketReliable,ulSequence,adrAdress.adr_uwID,slTransferSize);
	ppaNewPacket->pa_adrAddress.adr_ulAddress = adrAdress.adr_ulAddress;
	ppaNewPacket->pa_adrAddress.adr_uwPort = adrAdress.adr_uwPort;
	ppaNewPacket->pa_adrAddress.adr_uwID = adrAdress.adr_uwID;
  if (ci_pbOutputBuffer.AppendPacket(*ppaNewPacket,TRUE,FALSE) == FALSE) {
    ASSERT(FALSE);
  }
};


// receive a message through the interface, discard originating address
BOOL CClientInterface::Receive(void *pvReceive, SLONG &slSize,BOOL bReliable)
{
	ASSERT (slSize>0);
	ASSERT (pvReceive != NULL);

	// we'll use the other receive procedure, and tell it to ignore the address
	return ReceiveFrom(pvReceive,slSize,NULL,bReliable);
};
//! 인터페이스를 통해 메시지를 받는다.
// receive a message through the interface, and fill in the originating address
BOOL CClientInterface::ReceiveFrom(void *pvReceive, SLONG &slSize, CAddress *padrAdress,BOOL bReliable)
{
	CPacket* ppaPacket;
	UBYTE* pubData = (UBYTE*) pvReceive;
	SLONG slDummySize;
	UWORD uwReliable;
	//! 릴라이어블 메시지라면,
	// if a reliable message is requested
	if (bReliable) {
		//! 완전한 릴라이어블 메시지인지 체크
		// if there is no complete reliable message ready
		if (ci_pbReliableInputBuffer.CheckSequence(slDummySize,TRUE) == FALSE) {
			return FALSE;
        //! 준비된 메시지가 예상된 사이즈보다 크다면 리턴 FALSE
		// if the ready message is longer than the expected size
		} else if ( GetCurrentReliableSize() > slSize) {
			return FALSE;
		// if everything is ok, compose the message and kill the packets
		} else {
			//! 브로드캐스트에서만 어드레스가 NULL이 아니다. 서버에선 안씀.
			//! 릴라이어블 input버퍼에서 첫번째 패킷을 읽어와서 주소,포트,아이디를 얻어낸다.
			// fill in the originating address (if necessary)
			if (padrAdress != NULL) {
				ppaPacket = ci_pbReliableInputBuffer.PeekFirstPacket();
				padrAdress->adr_ulAddress = ppaPacket->pa_adrAddress.adr_ulAddress;
				padrAdress->adr_uwPort = ppaPacket->pa_adrAddress.adr_uwPort;
				padrAdress->adr_uwID = ppaPacket->pa_adrAddress.adr_uwID;
			}

			slSize = 0;
			//! uwReliable플래그 값이 테일이 나올때까지 릴라이어블 input버퍼에서 패킷을 읽어와서
			//! pubData로 옮긴다.
			do {
				ppaPacket = ci_pbReliableInputBuffer.GetFirstPacket();
				uwReliable = ppaPacket->pa_uwReliable;
				slDummySize = ppaPacket->pa_slSize - MAX_HEADER_SIZE;
				ppaPacket->ReadFromPacket(pubData,slDummySize);
				pubData += slDummySize;
				slSize += slDummySize;
				DeletePacket(ppaPacket);
			} while (!(uwReliable & UDP_PACKET_RELIABLE_TAIL));
			return TRUE;
		}
	//! 언릴라이어블 메시지라면,
	// if an unreliable message is requested
	} else {
		// if there are no packets in the input buffer, return
		if (ci_pbInputBuffer.pb_ulNumOfPackets == 0) {
			return FALSE;
			//! 릴라이어블 input버퍼가 비어있지 않다면, input버퍼로부터 아무것도 받을수 없다.			
			// if the reliable buffer is not empty, nothing can be accepted from the input buffer
			// because it would be accepted out-of order (before earlier sequences have been read)
		} else if (ci_pbReliableInputBuffer.pb_ulNumOfPackets != 0) {
			return FALSE;
		}
		
		ppaPacket = ci_pbInputBuffer.PeekFirstPacket();
		
		//! input버퍼에 있는 첫번째 패킷이 언릴리어블이 아니라면 리턴 FALSE
		// if the first packet in the input buffer is not unreliable
		if (!ppaPacket->IsUnreliable()) {
			return FALSE;
			//! 완전치 않은 메시지라면 리턴 FALSE
			// if there is no complete message ready
		} else if (ci_pbInputBuffer.CheckSequence(slDummySize,FALSE) == FALSE) {
			return FALSE;
			// if the ready message is longer than the expected size
		} else if ( ppaPacket->pa_slTransferSize > slSize) {
			return FALSE;
			// if everything is ok, read the packet data, and kill the packet
		} else {
			// fill in the originating address (if necessary)
			if (padrAdress != NULL) {
				padrAdress->adr_ulAddress = ppaPacket->pa_adrAddress.adr_ulAddress;
				padrAdress->adr_uwPort = ppaPacket->pa_adrAddress.adr_uwPort;
				padrAdress->adr_uwID = ppaPacket->pa_adrAddress.adr_uwID;
			}
			
			slSize = 0;
			//! 테일이 나올때까지 패킷을 받아서 사이즈를 얻어낸다.
			do {
				ppaPacket = ci_pbInputBuffer.GetFirstPacket();
				uwReliable = ppaPacket->pa_uwReliable;
				slDummySize = ppaPacket->pa_slSize - MAX_HEADER_SIZE;
				ppaPacket->ReadFromPacket(pubData,slDummySize);
				pubData += slDummySize;
				slSize += slDummySize;
				DeletePacket(ppaPacket);
			} while (!(uwReliable & UDP_PACKET_UNRELIABLE_TAIL));
			
			return TRUE;
		}
		
	}

	return FALSE;
};


// receive a message through the interface, discard originating address
BOOL CClientInterface::Receive(CTStream &strmReceive,UBYTE bReliable)
{
	CPacket* ppaPacket;
	UWORD uwReliable;
  SLONG slDummySize;

	// if a reliable message is requested
	if (bReliable) {
		// if there is no complete reliable message ready
		if (ci_pbReliableInputBuffer.CheckSequence(slDummySize,TRUE) == FALSE) {
			return FALSE;	
		// if everything is ok, compose the message and kill the packets
		} else {
			do {
				ppaPacket = ci_pbReliableInputBuffer.GetFirstPacket();
				uwReliable = ppaPacket->pa_uwReliable;
				strmReceive.Write_t(ppaPacket->pa_pubPacketData + MAX_HEADER_SIZE,ppaPacket->pa_slSize - MAX_HEADER_SIZE);
        if (ci_pbInputBuffer.pb_ulLastSequenceOut < ppaPacket->pa_ulSequence) {
          ci_pbInputBuffer.pb_ulLastSequenceOut = ppaPacket->pa_ulSequence;
        }
				DeletePacket(ppaPacket);
			} while (!(uwReliable & UDP_PACKET_RELIABLE_TAIL));
			return TRUE;
		}
	// if an unreliable message is requested
	} else {
    // if there are no packets in the input buffer, return
		if (ci_pbInputBuffer.pb_ulNumOfPackets == 0) {
			return FALSE;
    // if the reliable buffer is not empty, nothing can be accepted from the input buffer
	  // because it would be accepted out-of order (before earlier sequences have been read)
    } if (ci_pbReliableInputBuffer.pb_ulNumOfPackets != 0) {
			return FALSE;
    }

		ppaPacket = ci_pbInputBuffer.PeekFirstPacket();				
		// if the first packet in the input buffer is not unreliable
		if (!ppaPacket->IsUnreliable()) {
			return FALSE;
    // if there is no complete message ready
    } else if (ci_pbInputBuffer.CheckSequence(slDummySize,FALSE) == FALSE) {
			return FALSE;
		// if everything is ok, read the packet data, and kill the packet
		} else {
			do {
				ppaPacket = ci_pbInputBuffer.GetFirstPacket();
				uwReliable = ppaPacket->pa_uwReliable;
				strmReceive.Write_t(ppaPacket->pa_pubPacketData + MAX_HEADER_SIZE,ppaPacket->pa_slSize - MAX_HEADER_SIZE);
        if (ci_pbInputBuffer.pb_ulLastSequenceOut < ppaPacket->pa_ulSequence) {
          ci_pbInputBuffer.pb_ulLastSequenceOut = ppaPacket->pa_ulSequence;
        }
				DeletePacket(ppaPacket);
			} while (!(uwReliable & UDP_PACKET_UNRELIABLE_TAIL));
			return TRUE;
		}
		 
	}

	return FALSE;
};


// exchanges packets beetween this socket and it's local partner
// from output of this buffet to the input of the other and vice versa
void CClientInterface::ExchangeBuffers(void)
{
	ASSERT (ci_pciOther != NULL);
	CPacket* ppaPacket;
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();

	// take the output from this interface and give it to it's partner socket
	while (ci_pbOutputBuffer.pb_ulNumOfPackets > 0) {
		ppaPacket = ci_pbOutputBuffer.PeekFirstPacket();
		//if (ppaPacket->pa_tvSendWhen < tvNow) {
			ci_pbOutputBuffer.RemoveFirstPacket(FALSE);
			if (ci_pciOther->ci_pbInputBuffer.InsertPacket(*ppaPacket,FALSE) == FALSE) {
				DeletePacket(ppaPacket);
			}
		//} else {
//			break;
//		}
	}

	// and the other way around
	while (ci_pciOther->ci_pbOutputBuffer.pb_ulNumOfPackets > 0) {
		ppaPacket = ci_pciOther->ci_pbOutputBuffer.PeekFirstPacket();
//		if (ppaPacket->pa_tvSendWhen < tvNow) {
			ppaPacket = ci_pciOther->ci_pbOutputBuffer.GetFirstPacket();
			if (ci_pbInputBuffer.InsertPacket(*ppaPacket,FALSE) == FALSE) {
				DeletePacket(ppaPacket);
			};

//		}	else {
			//break;
//		}
	}

};


// update interface's input buffer (transfer from input buffer to the reliable buffer...),
// for incoming acknowledge packets, remove acknowledged packets from the output buffers,
// and generate acknowledge messages for incoming reliable packets
BOOL CClientInterface::UpdateInputBuffers(void)
{
	BOOL bSomethingDone;
	INDEX iMaxAcks = 50;
//	ULONG pulGenAck[50];
	ULONG ulAckCount=0;
	CTimerValue tvNow;
	
	// if there are packets in the input buffer, process them
	FORDELETELIST(CPacket,pa_lnListNode,ci_pbInputBuffer.pb_lhPacketStorage,ppaPacket) {
		CPacket &paPacket = *ppaPacket;
		//! 아크패킷이라면,
		// if it's an acknowledge packet, remove the acknowledged packets from the wait acknowledge buffer
		if (ppaPacket->pa_uwReliable & UDP_PACKET_ACKNOWLEDGE) {
			ULONG *pulAck;
			SLONG slSize;
			ULONG ulSequence;
			
			slSize = ppaPacket->pa_slSize - MAX_HEADER_SIZE;
			// if slSize isn't rounded to the size of ulSequence, abort 
			ASSERT (slSize % sizeof(ULONG) == 0);
			
			// get the pointer to the start of acknowledged sequences
			pulAck = (ULONG*) (ppaPacket->pa_pubPacketData + MAX_HEADER_SIZE);
			// for each acknowledged sequence number
			while (slSize>0) {
				ulSequence = *pulAck;
				
				// report the packet info to the console
				if (net_bReportPackets) {
					tvNow = _pTimer->GetHighPrecisionTimer();
					CPrintF("%lu: Received acknowledge for packet sequence %d\n",(ULONG) tvNow.GetMilliseconds(),ulSequence);
				}
				//! 아크버퍼에서 해당 시퀀스 패킷을 삭제한다.
				// remove the matching packet from the wait acknowledge buffer
				ci_pbWaitAckBuffer.RemovePacket(ulSequence,TRUE);
				//! 아웃버퍼에서 재전송을 기다리고 있는 해당 시퀀스 패킷을 삭제한다.
				// if the packet is waiting to be resent it's in the outgoing buffer, so remove it
				ci_pbOutputBuffer.RemovePacket(ulSequence,TRUE);
				pulAck++;
				slSize -= sizeof(ULONG);
			}
			//! input버퍼의 그 패킷을 삭제한다.
			// take this packet out of the input buffer and kill it
			ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,FALSE);
			DeletePacket(ppaPacket);
			
			bSomethingDone = TRUE;
			//! 릴라이어블 패킷이라면,
			// if the packet is reliable
		} else if (ppaPacket->pa_uwReliable & UDP_PACKET_RELIABLE) {
			
			//! 이 패킷이 브로드캐스트 어드래스로 부터 온거라면 아크패킷을 생성하여 보낸다.
			// generate packet acknowledge
			// if the packet is from the broadcast address, send the acknowledge for that packet only
			if (ppaPacket->pa_adrAddress.adr_uwID == '//' || ppaPacket->pa_adrAddress.adr_uwID == 0) {
				//0105 ACK패킷 지우기
				if(!_bTcp4ClientInterface){
					CPacket *ppaAckPacket = CreatePacket();
					ppaAckPacket->pa_adrAddress.adr_ulAddress = ppaPacket->pa_adrAddress.adr_ulAddress;
					ppaAckPacket->pa_adrAddress.adr_uwPort = ppaPacket->pa_adrAddress.adr_uwPort;
					ppaAckPacket->WriteToPacket(&(ppaPacket->pa_ulSequence),sizeof(ULONG),UDP_PACKET_ACKNOWLEDGE,++ci_ulSequence,'//',sizeof(ULONG));				
					if (net_bReportPackets) {
						CPrintF("Acknowledging broadcast packet sequence %d\n",ppaPacket->pa_ulSequence);
					}
					if (ci_pbOutputBuffer.AppendPacket(*ppaAckPacket,TRUE,TRUE) == FALSE) {
						DeletePacket(ppaAckPacket);
					}
				}
				//..					
				ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,FALSE);          
			}	else {
/* //0311 삭제
				//! 아크패킷이 최대치50개로 가득찬다면,
				// if we have filled the packet to the maximum with acknowledges (an extremely rare event)
				// finish this packet and start the next one
				if (ulAckCount == iMaxAcks) {
					CPacket *ppaAckPacket = CreatePacket();
					ppaAckPacket->pa_adrAddress.adr_ulAddress = ci_adrAddress.adr_ulAddress;
					ppaAckPacket->pa_adrAddress.adr_uwPort = ci_adrAddress.adr_uwPort;
					ppaAckPacket->WriteToPacket(pulGenAck,ulAckCount*sizeof(ULONG),UDP_PACKET_ACKNOWLEDGE,++ci_ulSequence,ci_adrAddress.adr_uwID,ulAckCount*sizeof(ULONG));
					ulAckCount = 0;
					if (ci_pbOutputBuffer.AppendPacket(*ppaAckPacket,TRUE,TRUE) == FALSE) {
						DeletePacket(ppaAckPacket);
						break;
					}						
				}	

				//! 이패킷의 시퀀스를  저장하고,
				// add the acknowledge for this packet
				pulGenAck[ulAckCount] = ppaPacket->pa_ulSequence;
				
				// report the packet info to the console
				if (net_bReportPackets) {
					tvNow = _pTimer->GetHighPrecisionTimer();
					CPrintF("%lu: Acknowledging packet sequence %d\n",(ULONG) tvNow.GetMilliseconds(),ppaPacket->pa_ulSequence);
				}
				//! 아크카운트를 증가시킨다.
				ulAckCount++;
*/
			}
			
			//! 해당 패킷을 input버퍼에서 삭제한다.
			// take this packet out of the input buffer
			ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,FALSE);		
			
			//! 그 패킷이 아크패킷이거나
			//! 또는 연결확인 응답 패킷이고 클라이언트가 아직 연결이 되지 않았거나, 
			//! 로컬클라이언트라면,		
			// a packet can be accepted from the broadcast ID only if it is an acknowledge packet or 
			// if it is a connection confirmation response packet and the client isn't already connected
			if (ppaPacket->pa_adrAddress.adr_uwID == '//' || ppaPacket->pa_adrAddress.adr_uwID == 0) {
				if  (((!ci_bUsed) && (ppaPacket->pa_uwReliable & UDP_PACKET_CONNECT_RESPONSE)) ||
					(ppaPacket->pa_uwReliable & UDP_PACKET_ACKNOWLEDGE) || ci_bClientLocal) {
					//! 그 패킷을 릴라이어블 input버퍼에 삽입한다.
					ppaPacket->pa_ulSequence = 0;
					if (ci_pbReliableInputBuffer.InsertPacket(*ppaPacket,FALSE) == FALSE) {
						DeletePacket(ppaPacket);
					}
				}	else {
					DeletePacket(ppaPacket);
				}
				// reject duplicates
				//! 해당 패킷의 시퀀스가 릴라이어블 input버퍼에서 마지막으로 나간 시퀀스보다 크고,
				//! 릴라이어블 input버퍼안에 해당 시퀀스가 없다면, 릴라이어블 버퍼에 패킷을 삽입한다.
			} else if (ppaPacket->pa_ulSequence > ci_pbReliableInputBuffer.pb_ulLastSequenceOut &&
				!(ci_pbReliableInputBuffer.IsSequenceInBuffer(ppaPacket->pa_ulSequence))) {
				if (ci_pbReliableInputBuffer.InsertPacket(*ppaPacket,FALSE) == FALSE) {
					DeletePacket(ppaPacket);
				}				
			} else {
				DeletePacket(ppaPacket);
			}
			
			// if the packet is unreliable, leave it in the input buffer
			// when it is needed, the message will be pulled from there
		} else {
			//! 해당 패킷의 시퀀스가 input버퍼에서 마지막으로 나간 시퀀스보다 크다면,
			// reject duplicates			
			if (ppaPacket->pa_ulSequence > ci_pbInputBuffer.pb_ulLastSequenceOut) { 
				//! 해당 패킷의 시퀀스와 같은 패킷이 1개보다 많다면, 패킷 삭제.
				if (ci_pbReliableInputBuffer.GetSequenceRepeat(ppaPacket->pa_ulSequence) > 1) {										
					ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,TRUE);
				}				
			} else {
				ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,TRUE);
			}
			
		}
	}
/* //0311 삭제
	// if there are any remaining unsent acknowldges, put them into a packet and send it
	//0105 ack패킷 없애기.	
	if(!_bTcp4ClientInterface){
		//! 아크 카운트가 있다면, 아크패킷을 생성하여 output버퍼에 넣는다. 
		//! 아크카운트가 여러개 있어도 한번만 보낸다.
		if (ulAckCount >0) {
			CPacket *ppaAckPacket = CreatePacket();
			ppaAckPacket->pa_adrAddress.adr_ulAddress = ci_adrAddress.adr_ulAddress;
			ppaAckPacket->pa_adrAddress.adr_uwPort = ci_adrAddress.adr_uwPort;
			ppaAckPacket->WriteToPacket(pulGenAck,ulAckCount*sizeof(ULONG),UDP_PACKET_ACKNOWLEDGE,++ci_ulSequence,ci_adrAddress.adr_uwID,ulAckCount*sizeof(ULONG));
			if (ci_pbOutputBuffer.AppendPacket(*ppaAckPacket,TRUE,TRUE) == FALSE) {
				DeletePacket(ppaAckPacket);
			}
		}	
	}
*/
	//..
	return TRUE;
	
};


// update socket input buffer (transfer from input buffer to the reliable buffer...),
// for incoming acknowledge packets, remove acknowledged packets from the output buffers,
// and generate acknowledge messages for incoming reliable packets
// this method is different than the previous becoause it sends acknowledges for each 
// packet separately, instead of grouping them together
BOOL CClientInterface::UpdateInputBuffersBroadcast(void)
{
	BOOL bSomethingDone;
	CTimerValue tvNow;
	//! 브로드캐스트의 Input버퍼에 패킷이 있다면,
	// if there are packets in the input buffer, process them
	FORDELETELIST(CPacket,pa_lnListNode,ci_pbInputBuffer.pb_lhPacketStorage,ppaPacket) {
		CPacket &paPacket = *ppaPacket;
			//! 그 패킷이 기다리는 아크패킷이라면 아크버퍼에 있는 해당패킷을 삭제한다.
			// if it's an acknowledge packet, remove the acknowledged packets from the wait acknowledge buffer
			if (ppaPacket->pa_uwReliable & UDP_PACKET_ACKNOWLEDGE) {
				ULONG *pulAck;
				SLONG slSize;
				ULONG ulSequence;

				slSize = ppaPacket->pa_slSize - MAX_HEADER_SIZE;
				// if slSize isn't rounded to the size of ulSequence, abort 
				ASSERT (slSize % sizeof(ULONG) == 0);
				//!아크패킷의 순서를 얻어낸다.
				// get the pointer to the start of acknowledged sequences
				pulAck = (ULONG*) (ppaPacket->pa_pubPacketData + MAX_HEADER_SIZE);
				// for each acknowledged sequence number
				while (slSize>0) {
					ulSequence = *pulAck;

					// report the packet info to the console
					if (net_bReportPackets) {
						tvNow = _pTimer->GetHighPrecisionTimer();
						CPrintF("%lu: Received acknowledge for broadcast packet sequence %d\n",(ULONG) tvNow.GetMilliseconds(),ulSequence);
					}
					//! 아크버퍼에서 그 패킷을 삭제한다.
					// remove the matching packet from the wait acknowledge buffer
					ci_pbWaitAckBuffer.RemovePacket(ulSequence,TRUE);
					//! 그 패킷이 재전송을 위해 아웃버퍼에 있다면 그 패킷도 삭제한다.
					// if the packet is waiting to be resent it's in the outgoing buffer, so remove it
					ci_pbOutputBuffer.RemovePacket(ulSequence,TRUE);
					pulAck++;
					slSize -= sizeof(ULONG);
				}
                //! input버퍼의 패킷 삭제한다.
				ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,FALSE);
				bSomethingDone = TRUE;
				DeletePacket(ppaPacket);
			//! 릴라이어블 패킷이라면,
			// if the packet is reliable
			} else if (ppaPacket->pa_uwReliable & UDP_PACKET_RELIABLE) {
//0105 Ack패킷 생성하지 않기 위해 잠시 지우기.
				if(!_bTcp4ClientInterface){				
					//! 아크 패킷을 생성하여 아웃버퍼로 보낸다.
					// generate packet acknowledge (each reliable broadcast packet is acknowledged separately 
					// because the broadcast interface can receive packets from any number of different addresses
					CPacket *ppaAckPacket = CreatePacket();
					ppaAckPacket->pa_adrAddress.adr_ulAddress = ppaPacket->pa_adrAddress.adr_ulAddress;
					ppaAckPacket->pa_adrAddress.adr_uwPort = ppaPacket->pa_adrAddress.adr_uwPort;
					ppaAckPacket->WriteToPacket(&(ppaPacket->pa_ulSequence),sizeof(ULONG),UDP_PACKET_ACKNOWLEDGE,ci_ulSequence++,ppaPacket->pa_adrAddress.adr_uwID,sizeof(ULONG));
					if (ci_pbOutputBuffer.AppendPacket(*ppaAckPacket,TRUE,TRUE) == FALSE) {
						DeletePacket(ppaAckPacket);
					}
				}
//..
				// report the packet info to the console
				if (net_bReportPackets) {
					tvNow = _pTimer->GetHighPrecisionTimer();
					CPrintF("%lu: Acknowledging broadcast packet sequence %d\n",(ULONG) tvNow.GetMilliseconds(),ppaPacket->pa_ulSequence);
				}
				//! 그런후에 input버퍼의 그 패킷을 삭제하고선,
				ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,FALSE);
				//! 릴라이어블 input버퍼에 넣는다.
				if (ci_pbReliableInputBuffer.InsertPacket(*ppaPacket,FALSE) == FALSE) {
					DeletePacket(ppaPacket);
				}				
			} else {
			//! 이 패킷이 언릴라이어블 패킷이라면 input 버퍼에 남겨놓는다.
			// if the packet is unreliable, leave it in the input buffer
			// when it is needed, the message will be pulled from there
			// have to check for duplicates	
			ci_pbInputBuffer.RemovePacket(ppaPacket->pa_ulSequence,FALSE);
			if (!(ci_pbReliableInputBuffer.IsSequenceInBuffer(ppaPacket->pa_ulSequence))) {						
				//! input 버퍼에 넣는다.
				if (ci_pbInputBuffer.InsertPacket(*ppaPacket,FALSE) == FALSE) {
					DeletePacket(ppaPacket);
				}
			} else {
				DeletePacket(ppaPacket);
			}

		}
	}

	return TRUE;
							
};


// take a look at the wait acknowledge buffer and resend any packets that heve reached the timeout
// if there is a packet that can't be sent sucessfully (RS_NOTATALL), signal it
BOOL CClientInterface::UpdateOutputBuffers(void)
{
	CPacket* ppaPacket;
	UBYTE ubRetry;

	//! 아크버퍼에 패킷이 있다면 
	// handle resends
	while (ci_pbWaitAckBuffer.pb_ulNumOfPackets > 0) {
		ppaPacket = ci_pbWaitAckBuffer.PeekFirstPacket();
		
		ubRetry = ppaPacket->CanRetry();
		switch (ubRetry) {
			//! 아크버퍼에서 패킷을 꺼내서 Output버퍼에 넣는다.
			// if it's time to retry sending the packet
			case RS_NOW: {	ci_pbWaitAckBuffer.RemoveFirstPacket(FALSE);
											ci_pbOutputBuffer.Retry(*ppaPacket);
											break;
									 }
			// if the packet cannot be sent now, no other packets can be sent, so exit
			case RS_NOTNOW: { return TRUE; }
			// if the packet has reached the retry limit - close the client's connection
			case RS_NOTATALL: { 
//0105 2line 지우기
//				Clear(); 
//									return FALSE;
//0105
				if(!_bTcp4ClientInterface){
					Clear(); // ACK 
					return FALSE;
				}else{
					//! 이제 수명을 다했으니 이 패킷은 아크버퍼에서 지워버린다.
					ci_pbWaitAckBuffer.Clear();
					return TRUE;
				}
//..
								
												}
			

		}
	}
	return TRUE;
};


// get the next available packet from the output buffer
CPacket* CClientInterface::GetPendingPacket(void)
{
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();

	if (ci_pbOutputBuffer.pb_ulNumOfPackets == 0) {
		return NULL;
	}

	CPacket* ppaPacket = ci_pbOutputBuffer.PeekFirstPacket();
	
	// if it's time to send the packet
	if (ppaPacket->pa_tvSendWhen <= tvNow) {
		ci_pbOutputBuffer.RemoveFirstPacket(FALSE);
		return ppaPacket;
	}

	return NULL;

};

//! 릴라이어블 input버퍼의 첫번째 패킷의 pa_slTransferSize를 리턴한다.
// reads the expected size of current realiable message in the reliable input buffer
SLONG CClientInterface::GetExpectedReliableSize(void)
{
  if (ci_pbReliableInputBuffer.pb_ulNumOfPackets == 0) {
    return 0;
  }
  CPacket* ppaPacket = ci_pbReliableInputBuffer.PeekFirstPacket();
	return ppaPacket->pa_slTransferSize;
};

//! 버퍼 시작점에 있는 릴라이어블 패킷의 총 사이즈를 리턴한다.
// reads the expected size of current realiable message in the reliable input buffer
SLONG CClientInterface::GetCurrentReliableSize(void)
{
  SLONG slSize;
  ci_pbReliableInputBuffer.CheckSequence(slSize,TRUE);
	return slSize;
};
