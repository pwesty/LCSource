#ifndef __AMUTEX_H__
#define __AMUTEX_H__

#ifdef CIRCLE_WINDOWS

class CMutexObject
{
public:
	CMutexObject()
	{
		m_mutex = CreateMutex(NULL, 0, NULL);
		m_bfail = (m_mutex == NULL);
	}

	~CMutexObject()
	{
		if (!m_bfail)
		{
			Unlock();
			CloseHandle(m_mutex);
		}

		m_bfail = true;
	}

	DWORD Lock()
	{
		return WaitForSingleObject(m_mutex, INFINITE);
	}

	DWORD Try()
	{
		return WaitForSingleObject(m_mutex, 0);
	}

	void Unlock()
	{
		ReleaseMutex(m_mutex);
	}

	HANDLE m_mutex;
	bool m_bfail;
};

class CMutex  
{
public:
	CMutex(CMutexObject* o, bool bBlock = true)
	{
		m_obj = o;
		if (bBlock)
			m_ret = m_obj->Lock();
		else
			m_ret = m_obj->Try();
	}

	~CMutex()
	{
		if (m_ret == WAIT_OBJECT_0)
			m_obj->Unlock();
	}

	bool IsLock()
	{
		return (m_ret == WAIT_OBJECT_0);
	}

protected:
	CMutexObject* m_obj;
	DWORD m_ret;
};

#else
#include "Sysdep.h"
class CMutexObject
{
public:
	CMutexObject()
	{
		m_bfail = (pthread_mutex_init(&m_mutex, NULL)) ? true : false;
	}

	~CMutexObject()
	{
		if (!m_bfail)
		{
			Unlock();
			pthread_mutex_destroy(&m_mutex);
		}

		m_bfail = true;
	}

	int Lock()
	{
		return pthread_mutex_lock(&m_mutex);
	}

	int Try()
	{
		return pthread_mutex_trylock(&m_mutex);
	}

	void Unlock()
	{
		pthread_mutex_unlock(&m_mutex);
	}

	pthread_mutex_t m_mutex;
	bool m_bfail;
};

class CMutex  
{
public:
	CMutex(CMutexObject* o, bool bBlock = true)
	{
		m_obj = o;
		if (bBlock)
			m_ret = m_obj->Lock();
		else
			m_ret = m_obj->Try();
	}

	~CMutex()
	{
		if (m_ret == 0)
			m_obj->Unlock();
	}

	bool IsLock()
	{
		return (m_ret == 0);
	}

protected:
	CMutexObject* m_obj;
	int m_ret;
};
#endif

#endif
