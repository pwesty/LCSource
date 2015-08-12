#ifndef _EXT_IPC_EVENT_H_
#define _EXT_IPC_EVENT_H_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <stdlib.h>
#include "SharedMemory_queue.h"

#define SME_SENDCMD_INTERVAL 10
#define SME_SENDCMD_TIMEOUT		(8*1000 / SME_SENDCMD_INTERVAL)

#define SME_SHAREMEMORY_CMD_NAME "SME_SHAREMEMORY_CMD_NAME"
#define SME_SHAREMEMORY_SEMAPHORE_CMD_NAME "SME_SHAREMEMORY_SEMAPHORE_CMD_NAME"


struct XSemaphore 
{
	XSemaphore(unsigned int initial_count, unsigned int max_count, const char *name, bool create_new);
	~XSemaphore();
	int P();
	int V();

	typedef HANDLE handle_t;

	handle_t sem_;

	bool owned;
	const char* name_;

private:
	XSemaphore(const XSemaphore&);
	XSemaphore& operator=(const XSemaphore&);
};

void SleepInMilliseconds(int msec);

//////////////////////////////////////////////////////////////////////////
#define SME_SHAREMEMORY_SEMAPHORE_NUM 1

struct IPCEventInfo 
{
	int SourceProcessID;
	int DestProcessID;
	int IPCEventStatus;
	int EventID;
	unsigned long ulParameter1;
	char ulParameter2[MAX_PATH];
};

typedef int (*SME_IPC_EVENT_CALLBACK_T)(void* pInEventInfo);
//////////////////////////////////////////////////////////////////////////
template <class Type>
class XExtIPCEvent
{
public:
	XExtIPCEvent();
	virtual ~XExtIPCEvent();

	enum {IPC_EVENT_BUFFER_SIZE = 1024};
	enum {IPC_QUEUE_NUMBER = 4};
	enum {IPC_QUEUE_RESOURCE = 0, IPC_QUEUE_SENDING = 1, IPC_QUEUE_POSTING = 3};

	// Four queues : empty, send queue, wait for sending, and post

	enum TmCommandStatus
	{
		EVENT_STATUS_UNUSED = 0, EVENT_STATUS_SENDING, EVENT_STATUS_POSTING, EVENT_STATUS_CONSUMED
	};

	//static XExtIPCEvent* GetInstance();

	int Create(int ProcessID, bool bIsMasterProcess = false);

	void Release (bool bIsMasterProcess = false);

	int SendIPCEvent(Type* pInEventInfo, Type* pOutEventInfo, int nTimeout = -1);
	int PostIPCEvent(Type* pInEventInfo);
	int QueryIPCEvent(Type* pOutEventInfo, SME_IPC_EVENT_CALLBACK_T pfnCallbak);

	void RecycleEvent(int idProcess);
	bool DumpInfo();

private:
	int Lock();
	int UnLock();

private:
	XSharedMemoryQueue* m_pSMQueue;
	XSemaphore* m_pSem;
	int m_idProcess;
};

//template<class Type> Type XSingleTon<Type>::m_pInstance = NULL;

//XExtIPCEvent ipc;

struct UUIDInfo 
{
	unsigned long uiParameter;
};

//////////////////////////////////////////////////////////////////////////
// Class XExtIPCEvent
template <class Type>
XExtIPCEvent<Type>::XExtIPCEvent() : m_pSMQueue(0), m_pSem(0), m_idProcess(-1)
{
}

template <class Type>
XExtIPCEvent<Type>::~XExtIPCEvent()
{
}

template <class Type>
int XExtIPCEvent<Type>::Lock()
{
	return m_pSem->P();
}

template <class Type>
int XExtIPCEvent<Type>::UnLock()
{
	return m_pSem->V();
}

/*XExtIPCEvent* XExtIPCEvent::GetInstance()
{
	return &ipc;
}*/

template <class Type>
int XExtIPCEvent<Type>::Create(int ProcessID, bool bIsMasterProcess /* = false */)
{
	m_pSMQueue = new XSharedMemoryQueue();

	if (m_pSMQueue == 0)
		return -1;

	if (!m_pSMQueue->Initialize(SME_SHAREMEMORY_CMD_NAME, sizeof(Type), IPC_EVENT_BUFFER_SIZE, IPC_QUEUE_NUMBER))
	{
		Release();
		return -1;
	}

	void* pCommandInfoPoint = 0;

	if (bIsMasterProcess)
		pCommandInfoPoint = m_pSMQueue->CreateSharedMemoryQueue();
	else
		pCommandInfoPoint = m_pSMQueue->OpenSharedMemoryQueue();

	if (pCommandInfoPoint == 0)
	{
		Release();
		return -1;
	}

	m_pSem = new XSemaphore(1, SME_SHAREMEMORY_SEMAPHORE_NUM, SME_SHAREMEMORY_SEMAPHORE_CMD_NAME, bIsMasterProcess);

	if (m_pSem == 0)
	{
		Release();
		return -1;
	}

	m_idProcess = ProcessID;
	return 0;
}

template <class Type>
void XExtIPCEvent<Type>::Release(bool bIsMasterProcess /* = false */)
{
	if (m_pSem)
	{
		delete m_pSem;
		m_pSem = NULL;
	}

	if (m_pSMQueue != 0)
	{
		m_pSMQueue->Release();

		if (bIsMasterProcess)
			m_pSMQueue->DeleteSharedMemoryQueue();
		else
			m_pSMQueue->CloseSharedMemoryQueue();


		delete m_pSMQueue;
		m_pSMQueue = 0;
	}
}

/*////////////////////////////////////////////////////////////////////////
Allocate a data block and move it to the event sending queue.
Sends the specified event to a process. This function does not return until the event status is consumed(EVENT_STATUS_CONSUMED).
////////////////////////////////////////////////////////////////////////*/
template <class Type>
int XExtIPCEvent<Type>::SendIPCEvent(Type* pInEventInfo, Type* pOutEventInfo, int nTimeout /* = -1 */)
{
	int err = 0;

	if (pInEventInfo == 0 || pOutEventInfo == 0 || m_pSMQueue == 0)
		return -1;
	else
		memset(pOutEventInfo, 0, sizeof(Type));

	pInEventInfo->IPCEventStatus = EVENT_STATUS_SENDING;

	if (-1 != m_idProcess)
		pInEventInfo->SourceProcessID = m_idProcess;

	// Get a block and set data
	int index = -1;
	
	if (Lock() < 0) return -1;
	
	Type* pTmpInfo = (Type*)m_pSMQueue->GetQueueHead(IPC_QUEUE_RESOURCE);

	if (pTmpInfo != 0 && pTmpInfo != pInEventInfo)
	{
		memcpy(pTmpInfo, pInEventInfo, sizeof(Type));
		m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_SENDING);
	}
	else
		err = -2;

	if (UnLock() < 0) return -1;

	if (err != 0) return err;

	const int timeout = (nTimeout<0 ? SME_SENDCMD_TIMEOUT : nTimeout/SME_SENDCMD_INTERVAL);

	// Get the result
	int Status;
	int i=0;

	for (i=0; i<timeout; i++)
	{
		if (Lock() < 0) return -1;
		Status = pTmpInfo->IPCEventStatus;
		if (UnLock() < 0) return -1;
		if (EVENT_STATUS_CONSUMED == Status) break;
		SleepInMilliseconds(SME_SENDCMD_INTERVAL);
	}

	// return the result
	if (i == timeout)
	{
		memset(pOutEventInfo, 0, sizeof(Type));
		err = -1;
	}
	else
	{
		if (Lock() < 0) return -1;
		memcpy(pOutEventInfo, pTmpInfo, sizeof(Type));
		m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_RESOURCE);
		if (UnLock() < 0) return -1;
	}

	return err;
}

// Allocate a data block and move it to the event posting queue
template <class Type>
int XExtIPCEvent<Type>::PostIPCEvent(Type* pInEventInfo)
{
	int err = 0;

	if (pInEventInfo == 0 || m_pSMQueue == 0)
		return -1;

	pInEventInfo->IPCEventStatus = EVENT_STATUS_POSTING;

	if (-1 != m_idProcess)
		pInEventInfo->SourceProcessID = m_idProcess;

	// Get a block and set data
	if (Lock() < 0) return -1;

	Type* pTmpInfo = (Type*)m_pSMQueue->GetQueueHead(IPC_QUEUE_RESOURCE);

	if (pTmpInfo != 0)
	{
		memcpy(pTmpInfo, pInEventInfo, sizeof(Type));
		m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_POSTING);
	}
	else
		err = -2;

	if (UnLock() < 0) return -1;

	return err;
}

/*////////////////////////////////////////////////////////////////////////
// Return
// 1 : IPC event is availale
// 0 : No IPC event
// < 0 : Error
////////////////////////////////////////////////////////////////////////*/
template <class Type>
int XExtIPCEvent<Type>::QueryIPCEvent(Type* pOutEventInfo, SME_IPC_EVENT_CALLBACK_T pfnCallbak)
{
	int err = 0;

	if (pOutEventInfo == 0 || m_pSMQueue == 0)
		return -1;
	else
		memset(pOutEventInfo, 0, sizeof(Type));

	//////////////////////////////////////////////////////////////////////////
	// Get event from sending queue
	if (Lock() < 0) return -1;

	Type* pTmpFirstInfo = (Type*)m_pSMQueue->GetQueueHead(IPC_QUEUE_SENDING);
	Type* pTmpInfo = pTmpFirstInfo;

	while (pTmpInfo)
	{
		if (pTmpInfo->DestProcessID == m_idProcess)
		{
			memcpy(pOutEventInfo, pTmpInfo, sizeof(Type));
			m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_RESOURCE);
			err = 1; // available
			break;
		}

		pTmpInfo = (Type*)m_pSMQueue->GetNext(pTmpInfo);

		if (pTmpInfo == pTmpFirstInfo)
		{
			memset(pOutEventInfo, 0, sizeof(Type));
			err = -1;
			break;
		}
	}

	if (UnLock() < 0) return -1;

	if (err < 0)
		return err;

	if (pTmpInfo != 0 && err >= 0)
	{
		if (pfnCallbak && !pfnCallbak(pOutEventInfo))
			err = -3;

		if (Lock() < 0) return -1;
		pTmpInfo->ulParameter1 = pOutEventInfo->ulParameter1;
		memcpy(pTmpInfo->ulParameter2, pOutEventInfo->ulParameter2, sizeof(pOutEventInfo->ulParameter2)-1);
		pTmpInfo->IPCEventStatus = EVENT_STATUS_CONSUMED;
		if (UnLock() < 0) return -1;

		return err;
	}

	//////////////////////////////////////////////////////////////////////////
	// Get event from posting queue
	err = 0;

	if (Lock() < 0) return -1;

	pTmpFirstInfo = (Type*)m_pSMQueue->GetQueueHead(IPC_QUEUE_POSTING);
	pTmpInfo = pTmpFirstInfo;

	while (pTmpInfo)
	{
		if (pTmpInfo->DestProcessID == m_idProcess)
		{
			memcpy(pOutEventInfo, pTmpInfo, sizeof(Type));
			m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_RESOURCE);
			err = 1; // available
			break;
		}

		pTmpInfo = (Type*)m_pSMQueue->GetNext(pTmpInfo);

		if (pTmpInfo == pTmpFirstInfo)
		{
			memset(pOutEventInfo, 0, sizeof(Type));
			err = -1;
			break;
		}
	}

	if (UnLock() < 0) return -1;

	if (err < 0)
		return err;

	if (pTmpInfo != 0 && err >= 0)
	{
		if (pfnCallbak && !pfnCallbak(pOutEventInfo))
			err = -3;

		return err;
	}

	return err;
}

template <class Type>
void XExtIPCEvent<Type>::RecycleEvent(int idProcess)
{
	if (Lock() < 0) return;

	for (int i=0; i<IPC_QUEUE_NUMBER; i++)
	{
		Type* pTmpFirstInfo = (Type*)m_pSMQueue->GetQueueHead(i);
		Type* pTmpInfo = pTmpFirstInfo;

		while (pTmpInfo)
		{
			Type* pTmpNextInfo = (Type*)m_pSMQueue->GetNext(pTmpInfo);

			if (pTmpInfo->DestProcessID == idProcess)
				m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_RESOURCE);
			if (pTmpInfo->SourceProcessID == idProcess)
				m_pSMQueue->MoveToDestQueueTail(pTmpInfo, IPC_QUEUE_RESOURCE);

			pTmpInfo = pTmpNextInfo;

			if (m_pSMQueue->GetQueueHead(i) == 0 || pTmpInfo == (Type*)m_pSMQueue->GetQueueHead(i)) break;
		}
	}

	if (UnLock() < 0) return;
}

template <class Type>
bool XExtIPCEvent<Type>::DumpInfo()
{
/*	if (Lock() < 0) return false;

	for (int i=0; m_pSMQueue!=0 && i<IPC_QUEUE_NUMBER; i++)
	{
		if (i != 0)
			m_pSMQueue->DumpQueue(i, true);
		else
			m_pSMQueue->DumpQueue(i, false);
	}

	if (UnLock() < 0) return false;
	printf("\n");
*/
	return true;
}
//////////////////////////////////////////////////////////////////////////
template <class Type>
class XSingle
{
public:
	XSingle() {}
	virtual ~XSingle() {}

	static bool CreateInstance()
	{
		if (!m_pInstance)
			m_pInstance = new XExtIPCEvent<Type>;
		
		if (m_pInstance) return true;

		return false;
	}

	static void ReleaseInstance()
	{
		if (m_pInstance) delete m_pInstance;
		m_pInstance = NULL;
	}

	static XExtIPCEvent<Type>* GetInstance()
	{
		return m_pInstance;
	}

protected:
	static XExtIPCEvent<Type>* m_pInstance;
};

template<class Type> XExtIPCEvent<Type>* XSingle<Type>::m_pInstance = NULL;

template <class Type>
class XExtIPCManager : public XSingle<Type>
{
public:
	XExtIPCManager() {}
	virtual ~XExtIPCManager() {}

	// Class XExtIPCEvent
	int XExtIPCEventCreate(int ProcessID, int bIsMasterProcess = 0)
	{
		if (CreateInstance())
			return GetInstance()->Create(ProcessID, bIsMasterProcess ? true : false);

		return -1;
	}

	void XExtIPCEventRelease(int bIsMasterProcess = 0)
	{
		GetInstance()->Release(bIsMasterProcess ? true : false);
		ReleaseInstance();
	}

	int XExtIPCEventSend(Type* pInEventInfo, Type* pOutEventInfo, int nTimeout = -1)
	{
		return GetInstance()->SendIPCEvent(pInEventInfo, pOutEventInfo, nTimeout);
	}

	int XExtIPCEventPost(Type* pInEventInfo)
	{
		return GetInstance()->PostIPCEvent(pInEventInfo);
	}

	int XExtIPCEventQueryIPCEvent(Type* pOutEventInfo, SME_IPC_EVENT_CALLBACK_T pfnCallbak)
	{
		return GetInstance()->QueryIPCEvent(pOutEventInfo, pfnCallbak);
	}
	
	void XExtIPCEventRecycleEvent(int idProcess)
	{
		GetInstance()->RecycleEvent(idProcess);
	}

	bool XExtIPCEventDumpInfo()
	{
		return GetInstance()->DumpInfo();
	}
};

#endif