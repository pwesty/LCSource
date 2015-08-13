#include "stdafx.h"
//#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "SharedMemory_queue.h"
#include "Access_control.h"

//////////////////////////////////////////////////////////////////////////
XSharedMemoryQueue::XSharedMemoryQueue()
: m_strMapName(0), m_iNodeSize(0), m_iNodeCount(0), m_iQueueCount(0), m_pQueueArray(0), m_pMappedPointer(0)
{
}

XSharedMemoryQueue::~XSharedMemoryQueue()
{
	if (m_strMapName)
	{
		free(m_strMapName);
		m_strMapName = 0;
	}
}

bool XSharedMemoryQueue::Initialize(const char* strMapName, int iNodeSize, int iNodeCount, int iQueueCount)
{
	if (strMapName == 0 || iNodeSize <= 0 || iNodeCount <=0 || iQueueCount <= 0)
	{
		return false;
	}

	m_strMapName = strdup(strMapName); // strMapName의 문자열 만큼 메모리 할당한다. free()호출이 필요함.
	m_iNodeSize = iNodeSize + 2*sizeof(int); // Node, Previous Node Pointer, Next Node Pointer
	m_iNodeCount = iNodeCount;
	m_iQueueCount = iQueueCount;
	
	if (m_strMapName == 0)
	{
		Release();
		return false;
	}

	return true;
}

void XSharedMemoryQueue::Release()
{
	m_iNodeSize = 0;
	m_iNodeCount = 0;
	m_iQueueCount = 0;
}

// Link all nodes together as a double linked list: queue 0.
void* XSharedMemoryQueue::CreateSharedMemoryQueue()
{
	m_hMapMem = CreateFileMapping(INVALID_HANDLE_VALUE, XAccessControl::getLowPrivSA(), PAGE_READWRITE, 0,
		m_iNodeSize*m_iNodeCount + m_iQueueCount*sizeof(int),
		m_strMapName);

	if (m_hMapMem == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
		return 0;

	m_pMappedPointer = MapViewOfFile(m_hMapMem, FILE_MAP_WRITE | FILE_MAP_READ,
		0, 0, m_iNodeSize*m_iNodeCount + m_iQueueCount*sizeof(int));

	if (m_pMappedPointer == NULL)
		return 0;

	// Link all nodes together as a double linked list: queue 0
	int i=0;

	for (i=0; i<m_iNodeCount; i++)
	{
		void* pNode = (char*)m_pMappedPointer + i*m_iNodeSize;
		
		int *pNext = (int*)((char*)pNode + m_iNodeSize-sizeof(int));
		int *pPrev = (int*)((char*)pNext - sizeof(int));

		if (i == m_iNodeCount - 1)
			*pNext = 0;
		else
			*pNext = (i+1) * m_iNodeSize;

		if (i == 0)
			*pPrev = (m_iNodeCount-1) * m_iNodeSize;
		else
			*pPrev = (i - 1) * m_iNodeSize;
	}

	m_pQueueArray = (int*)((char*)m_pMappedPointer + m_iNodeSize*m_iNodeCount);

	for (i=0; i<m_iQueueCount; i++)
		m_pQueueArray[i] = -1;

	m_pQueueArray[0] = 0;

	return m_pMappedPointer;
}

void* XSharedMemoryQueue::OpenSharedMemoryQueue()
{
	m_hMapMem = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, TRUE, m_strMapName);

	if (m_hMapMem == NULL)
	{
		//printf(" Mapping File Opened fail... %d\n", GetLastError());
		return 0;
	}

	m_pMappedPointer = MapViewOfFile(m_hMapMem, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0,
		m_iNodeSize*m_iNodeCount + m_iQueueCount*sizeof(int));

	if (m_pMappedPointer == NULL)
	{
		//printf("Mapping File Access fail\n");
		return 0;
	}

	m_pQueueArray = (int*)((char*)m_pMappedPointer + m_iNodeSize*m_iNodeCount);

	return m_pMappedPointer;
}

void XSharedMemoryQueue::DeleteSharedMemoryQueue()
{
	if (m_pMappedPointer)
	{
		UnmapViewOfFile(m_pMappedPointer);
		m_pMappedPointer = NULL;
	}

	if (m_hMapMem)
	{
		CloseHandle(m_hMapMem);
		m_hMapMem = NULL;
	}
}

void XSharedMemoryQueue::CloseSharedMemoryQueue()
{
	if (m_pMappedPointer)
	{
		UnmapViewOfFile (m_pMappedPointer);
		m_pMappedPointer = NULL;
	}
	
	if (m_hMapMem)
	{
		CloseHandle (m_hMapMem);
		m_hMapMem = NULL;
	}
}

void* XSharedMemoryQueue::GetAt(int iIndex)
{
	if (iIndex < 0 || iIndex >= m_iNodeCount) return 0;
	return (char*)m_pMappedPointer + m_iNodeSize * iIndex;
}

void* XSharedMemoryQueue::GetNext(void* pNode)
{
	if (pNode == 0) return 0;
	int *pNext = (int*)((char*)pNode + m_iNodeSize - sizeof(int));
	int *pPrev = (int*)((char*)pNext - sizeof(int));
	return (char*)m_pMappedPointer + *pNext;
}

void* XSharedMemoryQueue::GetPrev(void* pNode)
{
	if (pNode == 0) return 0;
	int *pNext = (int*)((char*)pNode + m_iNodeSize - sizeof(int));
	int *pPrev = (int*)((char*)pNext - sizeof(int));
	return (char*)m_pMappedPointer + *pPrev;
}

void* XSharedMemoryQueue::GetQueueHead(int iQueue)
{
	if (iQueue < 0 || iQueue >= m_iQueueCount) return 0;
	if (m_pQueueArray[iQueue] == -1) return 0;
	return (char*)m_pMappedPointer + m_pQueueArray[iQueue];
}

void* XSharedMemoryQueue::GetQueueTail(int iQueue)
{
	if (iQueue < 0 || iQueue >= m_iQueueCount) return 0;
	if (m_pQueueArray[iQueue] == -1) return 0;

	void *pNode = (char*)m_pMappedPointer + m_pQueueArray[iQueue];
	int *pNext = (int*)((char*)pNode + m_iNodeSize - sizeof(int));
	int *pPrev = (int*)((char*)pNext - sizeof(int));
	return (char*)m_pMappedPointer + *pPrev;
}

/*////////////////////////////////////////////////////////////////////////
Remove the given node from the source queue and this node to the destination queue.
If the destination queue is empty, add the node as the head of the queue.
If the destination queue is not empty, append the node to the destination queue.

The source and destination queues are double linked lists.
////////////////////////////////////////////////////////////////////////*/
bool XSharedMemoryQueue::MoveToDestQueueTail(void* pNode, int iDestQueue)
{
	if (iDestQueue < 0 || iDestQueue >= m_iQueueCount) return false;
	if (pNode == 0) return false;

	// Check if the pNode is the first node of a queue
	int i=0;

	for (i=0; i<m_iQueueCount; i++)
	{
		if (m_pQueueArray[i] != -1 && (char*)m_pMappedPointer+m_pQueueArray[i] == pNode)
			break;
	}

	void* pNewNode = pNode;
	int *pNodeNext = (int*)((char*)pNewNode + m_iNodeSize - sizeof(int));
	int *pNodePrev = (int*)((char*)pNodeNext - sizeof(int));

	if (i == m_iQueueCount)
	{// pNode가 연결에서 삭제 된다.
		// Not the first node of a queue, remove the node from the double linked queue.
		// Update the previous node's next node pointer
		*(int*)(((char*)m_pMappedPointer + *pNodePrev) + m_iNodeSize - sizeof(int)) = *pNodeNext;
		// Update the next node's previous node pointer
		*(int*)(((char*)m_pMappedPointer + *pNodeNext) + m_iNodeSize - 2*sizeof(int)) = *pNodePrev;
	}
	else
	{
		// The First node of a queue
		if (((char*)m_pMappedPointer + *pNodeNext) == pNewNode) // The node's next pointer is the itself
		{
			// Only one node in the original queue, becomes an empty queue.
			m_pQueueArray[i] = -1;
		}
		else
		{
			// Not only one node in the original queue, remove the node from the double linked queue.
			m_pQueueArray[i] = *pNodeNext;
			*(int*)(((char*)m_pMappedPointer + *pNodePrev) + m_iNodeSize - sizeof(int)) = *pNodeNext;
			*(int*)(((char*)m_pMappedPointer + *pNodeNext) + m_iNodeSize - 2*sizeof(int)) = *pNodePrev;
		}
	}

	if (m_pQueueArray[iDestQueue] == -1)
	{
		// Empty queue, become the first node.
		// The queue header, the node's next and previous pointer to itself.
		*pNodeNext = (char*)pNewNode - (char*)m_pMappedPointer; // The offset of the node
		*pNodePrev = *pNodeNext; // The offset of the node
		m_pQueueArray[iDestQueue] = *pNodeNext;
	}
	else
	{
		// Not empty queue, add the new node behind the head of the new queue.
		void* pHeader = (char*)m_pMappedPointer + m_pQueueArray[iDestQueue];
		int *pHeaderNext = (int*)((char*)pHeader + m_iNodeSize - sizeof(int));
		int *pHeaderPrev = (int*)((char*)pHeaderNext - sizeof(int));

		// Append the new node to the queue tail. Become the new tail of the queue.

		// Update the next pointer of the tail node to the new node
		*(int*)(((char*)m_pMappedPointer + *pHeaderPrev) + m_iNodeSize - sizeof(int)) = (char*)pNewNode - (char*)m_pMappedPointer;
		// Update the previous of the new node to the tail.
		*pNodePrev = *pHeaderPrev;
		// Update the old header's previous pointer to the new node (new tail).
		*pHeaderPrev = (char*)pNewNode - (char*)m_pMappedPointer;
		// Update the next pointer of the new to the head.
		*pNodeNext = m_pQueueArray[iDestQueue];
	}

	return true;
}

int XSharedMemoryQueue::GetIndex(void* pNode)
{
	return ((char*)pNode-(char*)m_pMappedPointer)/m_iNodeSize;
}

int XSharedMemoryQueue::LocateQueue(void* pNode)
{
	int i=0;

	for (i=0; i<m_iQueueCount; i++)
	{
		void* pFirstTmp = GetQueueHead(i);
		void* pTmp = pFirstTmp;

		while(pTmp)
		{
			if (pTmp==pNode) return i;
			pTmp = GetNext(pTmp);
			if (pTmp==pFirstTmp) break;
		}
	}

	return -1;
}

void XSharedMemoryQueue::DumpQueue(int iQueue, bool bDumpNode)
{
/*	int iCount = 0;
	void* pTmp = 0;
	void* pFirstTmp = GetQueueHead(iQueue);
	pTmp = pFirstTmp;
	//printf("Dump queue %d:\n", iQueue);

	while (pTmp)
	{
		iCount++;
		if (bDumpNode)
			printf("%08x ", pTmp);

		pTmp = GetNext(pTmp);
		if (pTmp==pFirstTmp) break;
	}

	//printf("\tnumber %d\n", iCount);*/
}

void XSharedMemoryQueue::DumpNode(void* pNode)
{
	void* pSrcNode = pNode;
	int *pSrcNext = (int*)((char*)pSrcNode + m_iNodeSize - sizeof(int));
	int *pSrcPrev = (int*)((char*)pSrcNext - sizeof(int));

//	printf("Dump Node %08x: Prev\t08x (%d), Next\t%08x (%d)\n", pNode,
//		(char*)m_pMappedPointer + *pSrcPrev, *pSrcPrev,
//		(char*)m_pMappedPointer + *pSrcNext, *pSrcNext);
}