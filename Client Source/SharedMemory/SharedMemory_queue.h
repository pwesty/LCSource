#ifndef _SHAREDMEMORY_QUEUE_H_
#define _SHAREDMEMORY_QUEUE_H_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

typedef HANDLE shared_memory_t;

class XSharedMemoryQueue
{
public:
	XSharedMemoryQueue();
	virtual ~XSharedMemoryQueue();

	bool Initialize(const char* strMapName, int iNodeSize, int iNodeCount, int iQueueCount);

	void Release();

	void* CreateSharedMemoryQueue();
	void DeleteSharedMemoryQueue();
	void* OpenSharedMemoryQueue();
	void CloseSharedMemoryQueue();

	void* GetAt(int iIndex);
	int GetIndex(void* pNode);

	void* GetQueueHead(int iQueue);
	void* GetQueueTail(int iQueue);
	void* GetNext(void* pNode);
	void* GetPrev(void* pNode);

	bool MoveToDestQueueTail(void* pNode, int iDestQueue);
	int LocateQueue(void* pNode);

	void DumpQueue(int iQueue, bool bDumpNode);
	void DumpNode(void* pNode);

private:
	char* m_strMapName;
	int m_iNodeSize;		// the size of internal node which contains additional pointers besides user data.
	int m_iNodeCount;		// the node count
	int m_iQueueCount;		// the number of queues
	int* m_pQueueArray;		// the queue array
	shared_memory_t m_hMapMem;
	void* m_pMappedPointer;
};

#endif