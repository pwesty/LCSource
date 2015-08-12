#ifndef __LIST_BOOST_H__
#define __LIST_BOOST_H__

#include <list>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>

template <class _Ty>
class list_ts : public std::list<_Ty>
{
public:
	typedef typename std::list<_Ty> list;
	typedef typename std::list<_Ty>::iterator iterator;

public:
	boost::recursive_mutex				m_mutex;

public:
	list_ts() {}
	~list_ts() {}

	boost::recursive_mutex &getmutex()
	{
		return m_mutex;
	}

	iterator begin()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return list::begin();
	}

	iterator end()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return list::end();
	}

	iterator erase(iterator _Where)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return list::erase(_Where);
	}

	iterator erase(iterator _First, iterator _Last)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return list::erase(_First, _Last);
	}

	void clear()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::clear();
	}

	void remove(const _Ty& _Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::remove(_Val);
	}

	void unique()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::unique();
	}

	void sort()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::sort();
	}

	void reverse()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::reverse();
	}

	int size()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return (int)list::size();
	}

	bool empty()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return list::empty();
	}

	_Ty &front()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return (*list::begin());
	}

	_Ty &back()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return (*--(list::end()));
	}

	void push_front(const _Ty& _Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::insert(list::begin(), _Val);
	}

	void pop_front()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::erase(list::begin());
	}

	void push_back(const _Ty &_Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::push_back(_Val);
	}

	void pop_back()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::erase(--list::end());
	}
};

//////////////////////////////////////////////////////////////////////////
template <class _Ty>
class list_like_queue_ts : public std::list<_Ty>
{
public:
	typedef typename std::list<_Ty> list;
	typedef typename std::list<_Ty>::iterator iterator;

public:
	boost::recursive_mutex				m_mutex;
	boost::condition					m_cond;
	volatile int						m_iLockCount;

public:
	list_like_queue_ts() : m_iLockCount(0) {}
	~list_like_queue_ts() {}

	boost::recursive_mutex &getmutex()
	{
		return m_mutex;
	}

	iterator begin()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return list::begin();
	}

	iterator end()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return list::end();
	}

	iterator erase(iterator _Where)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return list::erase(_Where);
	}

	iterator erase(iterator _First, iterator _Last)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return list::erase(_First, _Last);
	}

	void clear()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::clear();
		m_iLockCount = 0;
	}

	void remove(const _Ty& _Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::remove(_Val);
	}

	void unique()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::unique();
	}

	void sort()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::sort();
	}

	void reverse()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::reverse();
	}

	int size()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return (int)list::size();
	}

	bool empty()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return list::empty();
	}

	_Ty &front()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return (*list::begin());
	}

	_Ty &back()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return (*--(list::end()));
	}

	void push_front(const _Ty& _Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::insert(list::begin(), _Val);
	}

	void pop_front()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::erase(list::begin());
	}

	void push_back(const _Ty &_Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::push_back(_Val);
	}

	void pop_back()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::erase(--list::end());
	}

	//////////////////////////////////////////////////////////////////////////
	void wait()
	{
		boost::recursive_mutex::scoped_lock	lock(m_mutex);

		while( m_iLockCount == 0 )
		{
			m_cond.wait(lock);
		}
		--m_iLockCount;
	}

	void signal()
	{
		boost::recursive_mutex::scoped_lock	lock(m_mutex);

		++m_iLockCount;
		if(m_iLockCount == 1)
			m_cond.notify_one();
	}

	void push_signal(const _Ty &_Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		list::push_back(_Val);

		++m_iLockCount;
		if(m_iLockCount == 1)
			m_cond.notify_one();
	}

	_Ty front_pop()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		_Ty p = *(list::begin());
		list::erase(list::begin());
		return p;
	}

	_Ty wait_front_pop()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		while( m_iLockCount == 0 )
		{
			m_cond.wait(lock);
		}
		--m_iLockCount;

		_Ty p = *(list::begin());
		list::erase(list::begin());
		return p;
	}

	void dec_wait_count()
	{
		--m_iLockCount;
	}
};

#endif
