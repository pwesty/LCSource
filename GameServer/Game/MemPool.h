#ifndef __MEMPOOL_H__
#define __MEMPOOL_H__

#include "Sysdep.h"
#include "Mutex.h"	// Added by ClassView

#define NULL    0
#define MAX_HEADER_SIZE ((long)(sizeof(unsigned short) + sizeof(unsigned long) + sizeof(unsigned short) + sizeof(unsigned long)))
#define MAX_INPUTBUFFER_SIZE		((MAX_HEADER_SIZE + MAX_MESSAGE_SIZE) * 30)
#define MAX_OUTPUTBUFFER_SIZE		((MAX_MESSAGE_SIZE + MAX_HEADER_SIZE) * 300)
#define MAX_OUTPUTBUFFER_OUTUNIT	4096
#define MAX_PACKET_HEAD		((int) (sizeof(short)) + (sizeof(int)) + (sizeof(short) ) )
#define MAX_PACKET_DATA		(MAX_PACKET_SIZE - MAX_PACKET_HEAD)
#define MAX_PACKET_SIZE 1000

// ¹öÆÛ°ü·Ã ±æÀÌ »ó¼ö
#define MAX_MESSAGE_TYPE		((int)(sizeof(unsigned char)))
#define MAX_MESSAGE_SIZE		1000
#define MAX_MESSAGE_DATA		(MAX_MESSAGE_SIZE - MAX_MESSAGE_TYPE)
template <class T>
class CMemPoolNode
{
public:
	T*				m_mem;
	CMemPoolNode*	m_next;

	CMemPoolNode(T* mem)
	{
		m_mem = mem;
		m_next = NULL;
	}
};

template <class T, const int size>
class CMemPool  
{
	CMemPoolNode<T>*	m_head;
	CMutexObject		m_mutex;
	int					m_count;

public:
	CMemPool()
	{
		m_head = NULL;
		m_count = 0;
	}

	~CMemPool()
	{
		while (m_head)
		{
			CMemPoolNode<T>* d = m_head;
			m_head = m_head->m_next;
			delete [] d->m_mem;
			d->m_mem = NULL;
			delete d;
		}
		m_count = 0;
	}

	T* Get(bool bInitial = true)
	{
		CMutex lock(&m_mutex);

		if (m_head)
		{
			CMemPoolNode<T>* d = m_head;
			m_head = m_head->m_next;
			T* ret = d->m_mem;
			delete d;
			if (bInitial)
				memset(ret, 0, sizeof(T) * size);
			m_count--;
			return ret;
		}
		else
		{
			T* ret = new T[size];
			if (bInitial)
				memset(ret, 0, sizeof(T) * size);
			return ret;
		}
	}

	void Set(T* mem)
	{
		CMutex lock(&m_mutex);

		if (m_count < 500)
		{
			CMemPoolNode<T>* d = new CMemPoolNode<T>(mem);
			d->m_next = m_head;
			m_head = d;
			m_count++;
		}
		else
		{
			delete [] mem;
		}
	}
};

typedef CMemPool<unsigned char, MAX_INPUTBUFFER_SIZE> CInputBufferPool;
typedef CMemPool<unsigned char, MAX_MESSAGE_SIZE> CNetMsgPool;
typedef CMemPool<unsigned char, MAX_OUTPUTBUFFER_SIZE> COutputBufferPool;

typedef CMemPool<unsigned char, MAX_PACKET_SIZE> CBPacketPool;

#endif 
