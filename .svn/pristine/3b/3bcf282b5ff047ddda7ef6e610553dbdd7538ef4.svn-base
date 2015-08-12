/************************************************************************/
/* std::deque와 boost::recursive_mutex를 이용하여 thread safety하게 만듬 */
/* deque 전체를 lock 할경우                                              */
/* boost:;recursive_mutex &recursive_mutex = deque_ts<_Ty>::getmutex();            */
/*	  {                                                                 */
/*		  boost::recursive_mutex::scoped_lock lock(recursive_mutex);              */
/*	  }                                                                 */
/************************************************************************/
#ifndef __DEQUE_H__
#define __DEQUE_H__

#include <deque>
#include <boost/thread/recursive_mutex.hpp>

template <class _Ty>
class deque_ts : public std::deque<_Ty>
{
public:
	typedef typename std::deque<_Ty> deque;
	typedef typename std::deque<_Ty>::iterator iterator;
	typedef typename std::deque<_Ty>::reference reference;
	typedef typename std::deque<_Ty>::size_type size_type;

public:
	boost::recursive_mutex			m_mutex;

public:
	deque_ts() {}
	~deque_ts() { }

	boost::recursive_mutex &getmutex()
	{
		return m_mutex;
	}

	iterator begin()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return deque::begin();
	}

	iterator end()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return deque::end();
	}

	void resize(size_type _Newsize)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		deque::resize(_Newsize);
	}

	void resize(size_type _Newsize, _Ty _Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		deque::resize(_Newsize, _Val);
	}

	size_type size()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return deque::size();
	}

	size_type max_size()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return deque::max_size();
	}

	bool empty()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return deque::empty();
	}

	reference at(size_type _Pos)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return deque::at(_Pos);
	}

	reference operator[](size_type _Pos)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return deque::operator [](_Pos);
	}

	reference front()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return deque::front();
	}

	reference back()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return deque::back();
	}

	void push_front(const _Ty& _Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		deque::push_front(_Val);
	}

	void pop_front()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		deque::pop_front();
	}

	void push_back(const _Ty& _Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		deque::push_back(_Val);
	}

	void pop_back()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		deque::pop_back();
	}

	iterator insert(iterator _Where, const _Ty& _Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		deque::insert(_Where, _Val);
	}

	void insert(iterator _Where, size_type _Count, const _Ty& _Val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		deque::insert(_Where, _Count, _Val);
	}

	iterator erase(iterator _Where)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return deque::erase(_Where);
	}

	iterator erase(iterator _First, iterator _Last)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return deque::erase(_First, _Last);
	}

	void clear()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		deque::clear();
	}
};

#endif