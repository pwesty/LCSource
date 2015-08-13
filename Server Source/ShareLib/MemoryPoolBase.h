#ifndef __MEMORYPOOLBASE_H__
#define __MEMORYPOOLBASE_H__

#include <assert.h>
#include <boost/pool/pool.hpp>
#include <boost/thread/recursive_mutex.hpp>

template <class T>
class MemoryPoolBase
{
public:
	MemoryPoolBase(void) {}
	virtual ~MemoryPoolBase(void) {}

	void * operator new (size_t size)
	{
		(void)size;
		return (void *)(m_pool.malloc());
	}

	void * operator new (size_t size, void *p)
	{
		return p;
	}

	void operator delete (void *ptr)
	{
		m_pool.free( (T *)ptr );
	}

	void operator delete (void *ptr, T *p)
	{
	}

private:
	static boost::pool<>	m_pool;
};

template <class T>
boost::pool<>	MemoryPoolBase<T>::m_pool(sizeof(T), 32);

//////////////////////////////////////////////////////////////////////////

template <class T>
class MemoryPoolBaseWithMutex
{
public:
	MemoryPoolBaseWithMutex(void) {}
	virtual ~MemoryPoolBaseWithMutex(void) {}

	void * operator new (size_t size)
	{
		(void)size;

		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return (void *)(m_pool.malloc());
	}

	void * operator new (size_t size, void *p)
	{
		return p;
	}

	void operator delete (void *ptr)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		m_pool.free( (T *)ptr );
	}

	void operator delete (void *ptr, T *p)
	{
	}

private:
	static boost::pool<>	m_pool;
	static boost::recursive_mutex	m_mutex;
};

template <class T>
boost::pool<>	MemoryPoolBaseWithMutex<T>::m_pool(sizeof(T), 32);

template <class T>
boost::recursive_mutex	MemoryPoolBaseWithMutex<T>::m_mutex;

#endif
