#ifndef __QUEUE_BOOST_H__
#define __QUEUE_BOOST_H__

#include <list>
#include <queue>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>

template <class _Ty>
class queue_ts : public std::queue<_Ty>
{
public:
	typedef typename std::queue<_Ty> queue;
	typedef typename std::queue<_Ty>::value_type value_type;
	typedef typename std::queue<_Ty>::size_type size_type;

private:
	boost::recursive_mutex		m_queue_mutex;
	boost::condition			m_cond;

	volatile					int m_iLockCount;

public:
	queue_ts() : m_iLockCount(0)
	{
	}

	~queue_ts()
	{
	}

	boost::recursive_mutex &getmutex()
	{
		return m_queue_mutex;
	}

	bool empty()
	{
		boost::recursive_mutex::scoped_lock lock(m_queue_mutex);
		return queue::empty();
	}

	size_type size()
	{
		boost::recursive_mutex::scoped_lock lock(m_queue_mutex);
		return queue::size();
	}

	value_type& front()
	{
		boost::recursive_mutex::scoped_lock lock(m_queue_mutex);
		return queue::front();
	}

// 	const value_type& front() const
// 	{
// 		boost::recursive_mutex::scoped_lock lock(m_queue_mutex);
// 		return queue::front();
// 	}

	value_type& back()
	{
		boost::recursive_mutex::scoped_lock lock(m_queue_mutex);
		return queue::back();
	}

// 	const value_type& back() const
// 	{
// 		boost::recursive_mutex::scoped_lock lock(m_queue_mutex);
// 		return queue::back();
// 	}

	void push(const value_type& _Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_queue_mutex);
		queue::push(_Val);
	}

	void pop()
	{
		boost::recursive_mutex::scoped_lock lock(m_queue_mutex);
		queue::pop();
	}

	//--------------------------------------------------------------------------
	void wait()
	{
		boost::recursive_mutex::scoped_lock	lock(m_queue_mutex);

		while( m_iLockCount == 0 )
		{
			m_cond.wait(lock);
		}
		--m_iLockCount;
	}

	void signal()
	{
		boost::recursive_mutex::scoped_lock	lock(m_queue_mutex);
// 		++m_iLockCount;
// 		m_cond.notify_one();
		if(++m_iLockCount == 1)
			m_cond.notify_one();
	}

	void push_signal(const value_type & _Val)
	{
		boost::recursive_mutex::scoped_lock	lock(m_queue_mutex);
		queue::push(_Val);
// 		++m_iLockCount;
// 		m_cond.notify_one();
		if(++m_iLockCount == 1)
			m_cond.notify_one();
	}

	_Ty front_pop()
	{
		boost::recursive_mutex::scoped_lock lock(m_queue_mutex);
		_Ty p = queue::front();
		queue::pop();

		return p;
	}

	_Ty wait_front_pop()
	{
		boost::recursive_mutex::scoped_lock	lock(m_queue_mutex);

		while( m_iLockCount == 0 )
		{
			m_cond.wait(lock);
		}
		--m_iLockCount;

		//////////////////////////////////////////////////////////////////////////
		_Ty p = queue::front();
		queue::pop();

		return p;
	}

	void clear()
	{
		boost::recursive_mutex::scoped_lock lock(m_queue_mutex);
		while (!queue::empty())
		{
			queue::pop();
		}
	}
};

#endif
