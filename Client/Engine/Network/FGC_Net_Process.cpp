#include "StdH.h"
#include <Engine/Entities/Mail.h>
#include <Engine/Network/NetworkMessage.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Base/CTString.h>
#include "Engine/Base/Synchronization.h"
#include "include/F_CSocket_Event.h"
#include "FGC_Net_Process.h"

using namespace FGC_STATE;

FGC_Net_Process* FGC_Net_Process::g_FGC = NULL;	// static 변수 초기화

int		nConnectedCount = 0;

CTCriticalSection	g_LocalsFGC; // FGC 동기화를 위한 Criticalsection 핸들

namespace net
{
	DWORD WINAPI		RecvThreadFunc(LPVOID pParam)
	{
		F_Thread*	t_Thread = (F_Thread*)pParam;
		// 존이동중에는 서버와의 통신을 못하도록 Wait상태가 된다.
		// 존이동중에도, 서버로 부터 패킷을 받게 된다면, 미리 처리할 수 있는 사항이 많아 지지 않을까?
		// 나중에 검토 해 보자.
		while (t_Thread->Get_Exit())
		{
			t_Thread->Wait_Event();

			if (FGC != NULL)
			{
				switch(FGC->GetFGCState())
				{
				case SOC_TASK_WAIT:
					{
						if (FGC->IsConnectSubHelper())
						{
							if (!FGC->RecvPacket())
							{
								Sleep(1000);
							}
							//
							FGC->SubHelperRecvProcessing();
						}
						Sleep(1000);
						FGC->GetRecvThread()->Set_Event();
					}
					break;
				case SOC_MOVING_ZONE:
					{
						t_Thread->Wait_Event();
						FGC->GetRecvThread()->Set_Event();
					}
					break;
				case SOC_CLOSE:
					{
						t_Thread->Set_Exit();
					}
					break;
				}
			}
		}

		FGC->SetExitEvent(1);

		delete t_Thread;
		t_Thread = NULL;

		return 0;
	}
	// Send thread가 Main thread로 동작하도록 한다.
	DWORD WINAPI		SendThreadFunc(LPVOID pParam)
	{
		F_Thread*	t_Thread = (F_Thread*)pParam;

		while (t_Thread->Get_Exit())
		{
			t_Thread->Wait_Event();

			if (FGC != NULL)
			{
				switch(FGC->GetFGCState())
				{
				case SOC_DISCONNECTED: // 접속 시도
					{
						if (!FGC->IsConnectSubHelper())
						{
							if (!FGC->ConnectSubHelper())
							{ // 접속 실패!
								++nConnectedCount;
								if (nConnectedCount >= 3)
								{
									Sleep(5000); // 5초 후에 재 접속 시도
									nConnectedCount = 0;
								}
								FGC->GetSendThread()->Set_Event();
								break;
							}
						}
						Sleep(1000);
						FGC->SetFGCState(SOC_AUTH);
						FGC->GetSendThread()->Set_Event();
					}
					break;
				case SOC_AUTH:
					{
						//FGC->SendChannelAuth(_pNetwork->m_iServerCh, _pNetwork->MyCharacterInfo.index, _pNetwork->MyCharacterInfo.name);
						if (!FGC->IsConnectSubHelper())
						{
							Sleep(1000);
							FGC->SetFGCState(SOC_DISCONNECTED);
							FGC->GetSendThread()->Set_Event();
							break;
						}

						FGC->SendChannelAuth();
						Sleep(1000);
						FGC->SetFGCState(SOC_TASK_WAIT);
						FGC->GetRecvThread()->Set_Event();	// Recv 활성화
						FGC->GetSendThread()->Set_Event();
					}
					break;
				case SOC_TASK_WAIT:
					{
						if (FGC->IsConnectSubHelper())
						{
							if (FGC->SendPacket())
							{
								FGC->GetSendThread()->Set_Event();
								break;
							}
						}
						Sleep(1000); // 무조건 1초 지연
						FGC->GetSendThread()->Set_Event();
					}
					break;
				case SOC_MOVING_ZONE:
				case SOC_RESTART:
					{
						t_Thread->Wait_Event();
						FGC->GetSendThread()->Set_Event();
						FGC->GetRecvThread()->Set_Event();
					}
					break;
				case SOC_CLOSE:
					{
						t_Thread->Set_Exit();
					}
					break;
				default:
					{
						Sleep(1000);
						FGC->GetSendThread()->Set_Event();
					}
					break;
				}
			}
		}

		FGC->SetExitEvent(0);

		delete t_Thread;
		t_Thread = NULL;

		return 0;
	}

	/* *********************************************************************************
		ReleaseElement()
		Parameter로 전달된 Element를 Release한다.
	*********************************************************************************  */
	void lmbExtendQueue::ReleaseElement(LocalMsgBuffer* lmbInput)
	{
		if (lmbInput)
		{
			delete lmbInput;
			lmbInput = NULL;
		}
	}

	/* *********************************************************************************
		AddNetworkMessage()
		Queue 리스트에 CNetworkMessage 데이터를 LocalMsgBuffer 데이터로 변환하여 추가한다.
	*********************************************************************************  */
	void lmbExtendQueue::AddNetworkMessage(CNetworkMessage& istr)
	{
		LocalMsgBuffer* addBuff = new LocalMsgBuffer;
		addBuff->msgSize = istr.nm_slSize;
		addBuff->ubType = istr.nm_mtType;
		memcpy(addBuff->msgBuf, istr.nm_pubMessage, addBuff->msgSize);
		push_back(addBuff);
	}
	/* *********************************************************************************
		getFrontElement()
		Queue에서 최상단 Element를 가져온다.
	*********************************************************************************  */
	bool lmbExtendQueue::getFrontElement(CNetworkMessage& istr)
	{
		if (empty())
			return false;

		LocalMsgBuffer* addBuff = (*begin());
		//istr.nm_mtType = addBuff->ubType;
		istr.Initialize(addBuff->ubType);
		istr.nm_slSize = addBuff->msgSize;
		memcpy(istr.nm_pubMessage, addBuff->msgBuf, istr.nm_slSize);
		delete addBuff;
		addBuff =NULL;
		pop_front();
		return true;
	}
	/* *********************************************************************************
		ReleaseAll()
		Queue에서 가진 모든 Element를 Release한다.
	*********************************************************************************  */
	void lmbExtendQueue::ReleaseAll()
	{
		if (empty())
			return;

		lmbExtendQueue::iterator Itr = begin();
		LocalMsgBuffer* relaseElement;
		for (;Itr != end(); ++Itr)
		{
			relaseElement = (*Itr);

			delete relaseElement;
			relaseElement = NULL;
		}

		clear();
	}
}

/* *********************************************************************************
	FGC_Net_Process()
*********************************************************************************  */
FGC_Net_Process::FGC_Net_Process(
								 CONNECT_FUNC pConnect,
								 FUNC_INT_RETURN pDisConnect,
								 FUNC_INT_RETURN pIsConnect,
								 SEND_FUNC pSend,
								 RECV_FUNC pReceive) // 복사 생성자
{
	connectSocket = pConnect;
	disconnectSocket = pDisConnect;
	isConnectSocket = pIsConnect;
	sendToSocket = pSend;
	receiveSocket = pReceive;

	m_Port = 0;
	m_Sequence = 0;

	m_hExitTask[0] = NULL;
	m_hExitTask[1] = NULL;

	m_pRecvEventThread = new F_Thread;
	m_pSendEventThread = new F_Thread;

	if (m_pRecvEventThread)
	{
		m_pRecvEventThread->Create(net::RecvThreadFunc, m_pRecvEventThread);
		m_hExitTask[0] = CreateEvent(NULL,false,false,NULL);
	}

	if (m_pSendEventThread)
	{
		m_pSendEventThread->Create(net::SendThreadFunc, m_pSendEventThread);
		m_hExitTask[1] = CreateEvent(NULL,false,false,NULL);
	}

	m_FGCState = SOC_DISCONNECTED;
}

/* *********************************************************************************
	~FGC_Net_Process()
*********************************************************************************  */
FGC_Net_Process::~FGC_Net_Process()
{
	if (GetFGCState() != SOC_CLOSE)
	{
		if (m_pSendEventThread)
			m_pSendEventThread->Set_Exit();

		if (m_pRecvEventThread)
			m_pRecvEventThread->Set_Exit();

		m_FGCState = SOC_CLOSE;
	}

	// 사용중인 send, recv 스레드 종료를 기다린다.
	WaitForMultipleObjects(2, m_hExitTask, TRUE, INFINITE);

	m_pRecvEventThread = NULL;
	m_pSendEventThread = NULL;

	m_hExitTask[0] = NULL;
	m_hExitTask[1] = NULL;

	if (isConnectSocket() > 0)
	{
		disconnectSocket();
	}

	// 사용중인 버퍼 Release
	m_recvBuffer.ReleaseAll();
	m_sendBuffer.ReleaseAll();
}

// Member function
/* *********************************************************************************
	SendChannelAuth()
*********************************************************************************  */
BOOL FGC_Net_Process::SendChannelAuth(void)//(int nChannel, int charIndex, CTString charName)
{
	CTSingleLock slLocal(&g_LocalsFGC, TRUE);

	if (!isConnectSocket())
		return FALSE;

	//키 초기화
#ifdef CRYPT_NET_MSG
	CNM_InitKeyValue(&_pNetwork->subHelperKey);
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_InitKeyValue(&_pNetwork->subHelperKeyServer);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#else
	_pNetwork->subHelperKey = CNM_INIT_KEY;
#endif // #ifdef CRYPT_NET_MSG

	//시드 생성 -> 서버로 보낸다.
#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
	ULONG ulSeed = (ULONG)CNM_MakeSeedForClient(_pNetwork->MyCharacterInfo.name, _pNetwork->MyCharacterInfo.name, GetTickCount());

#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG
	
	// 키 생성 -> 내가 가진다.
	// EDIT : BS : 070413 : 신규 패킷 암호화

	CNetworkMessage nmPostsys(MSG_USERSERVER_AUTH);
	nmPostsys << (INDEX)_pNetwork->m_iServerCh;
	nmPostsys << (INDEX)_pNetwork->MyCharacterInfo.index;
	nmPostsys << _pNetwork->MyCharacterInfo.name;
	nmPostsys << ulSeed;
#ifdef XOR_ENCIPHERMENT
	Xor_enc(nmPostsys.nm_pubMessage, nmPostsys.nm_slSize);
	FGC->SendPacket(nmPostsys.nm_pubMessage, nmPostsys.nm_slSize);
#else
	MsgCryption(nmPostsys);
#endif // XOR_ENCIPHERMENT

#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_MakeKeyFromSeed(&_pNetwork->subHelperKey, (unsigned int)ulSeed);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG

	return TRUE;
}

/* *********************************************************************************
	MakePacketHeader()
*********************************************************************************  */
CPacket* FGC_Net_Process::MakePacketHeader(const void *pvReceive, int packetSize)
{
	CTSingleLock slLocal(&g_LocalsFGC, TRUE);
	ASSERT(pvReceive != NULL && packetSize > 0);

	UWORD uwPacketReliable = UDP_PACKET_UNRELIABLE_HEAD | UDP_PACKET_UNRELIABLE | UDP_PACKET_UNRELIABLE_TAIL;
	UBYTE* pubData;
	SLONG slSizeToSend;
	SLONG slTranserSize;
	ULONG ulSequence = GetSequence();
	CPacket* ppaNewPacket;
	UWORD adr_uwID = 0;

	pubData = (UBYTE*)pvReceive;
	slSizeToSend = packetSize;
	slTranserSize = packetSize;

	ppaNewPacket = CreatePacket();

	IncSequence();

	if (ppaNewPacket->WriteToPacket(
			pubData,
			slSizeToSend,
			uwPacketReliable,
			ulSequence,
			adr_uwID,
			slTranserSize))
	{
		CTString strAddress = GetIpAddress();
		ppaNewPacket->pa_adrAddress.adr_ulAddress = atoi(strAddress.str_String);
		ppaNewPacket->pa_adrAddress.adr_uwPort = GetPort();
		ppaNewPacket->pa_adrAddress.adr_uwID = adr_uwID;
		return ppaNewPacket;
	}

	return NULL;
}

/* *********************************************************************************
	RecvPacket()
*********************************************************************************  */
BOOL FGC_Net_Process::RecvPacket()
{
	CTSingleLock slLocal(&g_LocalsFGC, TRUE);

	ASSERT(receiveSocket != NULL);

	int recvSize = 0;
	unsigned char* MsgBuffer = (unsigned char*)receiveSocket(&recvSize);

	if (MsgBuffer == NULL)
		return FALSE;

	unsigned char* p = MsgBuffer;

	while (p < MsgBuffer + recvSize && recvSize > 0)
	{
		unsigned short int reliable;
		unsigned long int seq;
		unsigned short int id;
		unsigned long int txSize;

		memcpy(&reliable, p, sizeof(reliable));		p += sizeof(reliable);
		memcpy(&seq, p, sizeof(seq));				p += sizeof(seq);
		memcpy(&id, p, sizeof(id));					p += sizeof(id);
		memcpy(&txSize, p, sizeof(txSize));			p += sizeof(txSize);
		NTOHL(txSize);

		if (txSize > MAX_UDP_BLOCK_SIZE) // txSize가 1KB이상일때가 있는가? 현재 msgbuffer는 한번에 1K이상의 처리를 할 수 없다.
		{
			p += txSize;
			// [2013/01/09] sykim70
			p += sizeof(unsigned int);	// for CRC
			continue;
		}
		else
		{
			net::LocalMsgBuffer* lmb = new net::LocalMsgBuffer;
			lmb->Clear();
			lmb->msgSize = txSize;
			memcpy(lmb->msgBuf, p, lmb->msgSize);
			MsgDecryption(lmb); // 복호화
			m_recvBuffer.push_back(lmb);

			p += txSize;
			// [2013/01/09] sykim70
			p += sizeof(unsigned int);	// for CRC
		}
	}

	free(MsgBuffer);

	return TRUE;
}

/* *********************************************************************************
	AddSendBuffer()
*********************************************************************************  */
bool FGC_Net_Process::AddSendBuffer(CNetworkMessage& iStr)
{
	m_sendBuffer.AddNetworkMessage(iStr);
	return true;
}

/* *********************************************************************************
	PopRecvBuffer()
*********************************************************************************  */
bool FGC_Net_Process::PopRecvBuffer(CNetworkMessage& iStr)
{
	return m_recvBuffer.getFrontElement(iStr);
}

/* *********************************************************************************
	IsConnectSubHelper()
*********************************************************************************  */
bool FGC_Net_Process::IsConnectSubHelper()
{
	ASSERT(isConnectSocket != NULL);
	CTSingleLock slComm(&g_LocalsFGC, TRUE);
	return (isConnectSocket() > 0);
}

/* *********************************************************************************
	ConnectSubHelper()
*********************************************************************************  */
bool FGC_Net_Process::ConnectSubHelper()
{
	ASSERT(connectSocket != NULL);
	CTSingleLock slComm(&g_LocalsFGC, TRUE);
	return (connectSocket(m_IpAddress.str_String, m_Port) > 0);
}

/* *********************************************************************************
	DisConnectSubHelper()
*********************************************************************************  */
bool FGC_Net_Process::DisConnectSubHelper()
{
	ASSERT(disconnectSocket != NULL);
	CTSingleLock slComm(&g_LocalsFGC, TRUE);
	if (!IsConnectSubHelper())
		return false;

	SetFGCState(FGC_STATE::SOC_DISCONNECTED); // 상태 전환
	return (disconnectSocket() > 0);
}

/* *********************************************************************************
	SubHelperRecvProcessing()
*********************************************************************************  */
void FGC_Net_Process::SubHelperRecvProcessing()
{
	// 버퍼를 한번 소비를 한다.
	CTSingleLock slComm(&g_LocalsFGC, TRUE);

	if (m_recvBuffer.empty())
		return;

	net::lmbExtendQueue::iterator Itr = m_recvBuffer.begin();

	CNetworkMessage procRecvBuffer;

	while(PopRecvBuffer(procRecvBuffer))
	{
		PostSystem::DispatchMessage(&procRecvBuffer);
	}
}

/* *********************************************************************************
	SendPacket()
*********************************************************************************  */
BOOL FGC_Net_Process::SendPacket()
{
	CTSingleLock slComm(&g_LocalsFGC, TRUE);

	if (m_sendBuffer.empty())
		return FALSE;

	net::LocalMsgBuffer* SendMsg = (*m_sendBuffer.begin());
	m_sendBuffer.pop_front();

	//if (!SendPacket((void*)SendMsg->msgBuf, SendMsg->msgSize))
	if (!MsgCryption(SendMsg))
	{
		delete SendMsg;
		SendMsg = NULL;
		return FALSE;
	}

	delete SendMsg;
	SendMsg = NULL;

	return TRUE;
}

BOOL FGC_Net_Process::SendPacket(void *pvSend, int packetSize)
{
	ASSERT( sendToSocket != NULL && isConnectSocket != NULL );

	CTSingleLock slComm(&g_LocalsFGC, TRUE);

	SLONG slResult;

	CPacket* sendPacket = MakePacketHeader(pvSend, packetSize);

	if (sendPacket == NULL)
	{
		return FALSE;
	}

	if (isConnectSocket() /* send state는 상위에서 판단한다.*/)
	{
		// [2013/01/09] sykim70
		unsigned int res = htonl(m_crc.CalcCRC32(sendPacket->pa_pubPacketData+MAX_HEADER_SIZE, sendPacket->pa_slSize-MAX_HEADER_SIZE));
		memcpy(sendPacket->pa_pubPacketData+sendPacket->pa_slSize, &res, sizeof(unsigned int));
		slResult = sendToSocket((char*)sendPacket->pa_pubPacketData, sendPacket->pa_slSize+sizeof(unsigned int));

		if (slResult == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();

			if (iError == WSAEWOULDBLOCK)
				return FALSE;

			if (iError != WSAECONNRESET) // 접속 지연으로 연결이 끊어짐
			{

			}

			_pNetwork->m_bSendMessage = FALSE;
			disconnectSocket();
			SetFGCState(FGC_STATE::SOC_DISCONNECTED);
			return FALSE;
		}
	}

	return TRUE;
}

/* *********************************************************************************
	MsgCryption()
*********************************************************************************  */
bool FGC_Net_Process::MsgCryption(net::LocalMsgBuffer& lmbMsg)
{
	CTSingleLock slLocal(&g_LocalsFGC, TRUE);
	CNetworkMessage NMInput;
	NMInput.nm_slSize = lmbMsg.msgSize;
	NMInput.nm_mtType = lmbMsg.ubType;
	memcpy(NMInput.nm_pubMessage, lmbMsg.msgBuf, lmbMsg.msgSize);
	return MsgCryption(NMInput);
}

bool FGC_Net_Process::MsgCryption(net::LocalMsgBuffer* lmbMsg)
{
	CTSingleLock slLocal(&g_LocalsFGC, TRUE);
	CNetworkMessage NMInput;
	NMInput.nm_slSize = lmbMsg->msgSize;
	NMInput.nm_mtType = lmbMsg->ubType;
	memcpy(NMInput.nm_pubMessage, lmbMsg->msgBuf, lmbMsg->msgSize);
	return MsgCryption(NMInput);
}

bool FGC_Net_Process::MsgCryption(CNetworkMessage& iStr)
{
	CTSingleLock slLocal(&g_LocalsFGC, TRUE);
	CNetworkMessage tmpMessage(iStr.GetType());

#ifdef CRYPT_NET_MSG
	UBYTE pCryptBuf[CNM_TEMP_BUFFER_LENGTH];
	UBYTE pCryptTmpBuf[CNM_TEMP_BUFFER_LENGTH];
	int nCryptBuf = CNM_Crypt(&_pNetwork->subHelperKey,
						iStr.nm_slSize,
						iStr.nm_pubMessage,
						pCryptBuf,
						pCryptTmpBuf);
#else
	UBYTE* pCryptBuf = NULL;
	int nCryptBuf = CNM_Crypt(iStr.nm_pubMessage,
						iStr.nm_pubMessage,
						_pNetwork->subHelperKey,
						&pCryptBuf);
#endif

#ifdef CRYPT_NET_MSG_LEVEL2
	extern unsigned char g_ubKey[];
	CryptMem(pCryptBuf, nCryptBuf, g_ubKey, 14);
#endif

	if (nCryptBuf < 0)
	{
		return false;
	}

	CNetworkMessage cryptMessage(pCryptBuf[0]);
	cryptMessage.Write(pCryptBuf + sizeof(UBYTE), nCryptBuf - sizeof(UBYTE));
	tmpMessage = cryptMessage;

#ifndef CRYPT_NET_MSG
	if (pCryptBuf)
		delete [] pCryptBuf;
#endif

#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_NextKey(&_pNetwork->subHelperKey);
#endif
#endif

#ifdef COMPRESSION_MESSAGE
	CNetworkMessage nmPackMessage(tmpMessage.GetType());
	tmpMessage.PackDefault(nmPackMessage);
	FGC->SendPacket(nmPackMessage.nm_pubMessage, nmPackMessage.nm_slSize);
	return true;
#else
	FGC->SendPacket(tmpMessage.nm_pubMessage, tmpMessage.nm_slSize);
	return true;
#endif
}

/* *********************************************************************************
	MsgDecryption()
*********************************************************************************  */
bool FGC_Net_Process::MsgDecryption(net::LocalMsgBuffer* lmbMsg)
{
	CTSingleLock slLocal(&g_LocalsFGC, TRUE);
#ifdef CRYPT_NET_MSG
	UBYTE pDecryptBuf[CNM_TEMP_BUFFER_LENGTH];
	UBYTE pDecryptBufTmp[CNM_TEMP_BUFFER_LENGTH];

#ifdef CRYPT_NET_MSG_LEVEL2
	extern unsigned char g_ubKey[];
	DecryptMem(lmbMsg->msgBuf, lmbMsg->msgSize, g_ubKey, 14);
#endif

#ifdef CRYPT_NET_MSG_MANUAL
	int nDecryptBuf = CNM_Decrypt(lmbMsg->msgBuf,
						lmbMsg->msgSize,
						&_pNetwork->subHelperKey,
						pDecryptBuf,
						pDecryptBufTmp);
#else // CRYPT_NET_MSG_MANUAL
	int nDecryptBuf = CNM_Decrypt(&_pNetwork->subHelperKeyServer,
						lmbMsg->msgSize,
						lmbMsg->msgBuf,
						pDecryptBuf,
						pDecryptBufTmp);
#endif // CRYPT_NET_MSG_MANUAL

#else //CRYPT_NET_MSG
	UBYTE* pDecryptBuf = NULL;
	int nDecryptBuf = CNM_Decrypt(lmbMsg->msgBuf,
						lmbMsg->msgSize,
						_pNetwork->subHelperKey,
						&pDecryptBuf);
#endif // CRYPT_NET_MSG

	if (nDecryptBuf > 0)
	{
		lmbMsg->Clear();
		int iSize = sizeof(lmbMsg->ubType);
		memcpy(&lmbMsg->ubType, pDecryptBuf, iSize); // main type 가져오기
		memcpy(lmbMsg->msgBuf, pDecryptBuf, nDecryptBuf);
		lmbMsg->msgSize = nDecryptBuf;
	}
	else
	{
		return false;
	}

#ifndef CRYPT_NET_MSG
	if (pDecryptBuf)
	{
		delete [] pDecryptBuf;
	}
#endif

#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_NextKey(&_pNetwork->subHelperKeyServer);
#endif
#endif

	return true;
}
