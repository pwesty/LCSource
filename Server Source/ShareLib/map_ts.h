#ifndef __MAP_BOOST_H__
#define __MAP_BOOST_H__

#include <map>
#include <boost/thread/recursive_mutex.hpp>

// TEMPLATE CLASS map
template <class _Kty, class _Ty>
class map_ts : public std::map<_Kty, _Ty>
{
public:
	typedef std::map<_Kty, _Ty> map;
	typedef _Kty key_type;
	typedef typename std::map<_Kty, _Ty>::size_type size_type;
	typedef typename std::map<_Kty, _Ty>::iterator iterator;
	typedef typename std::map<_Kty, _Ty>::value_type value_type;

private:
	boost::recursive_mutex				m_mutex;

public:
	map_ts() {}
	~map_ts() {}

	boost::recursive_mutex &getmutex()
	{
		return m_mutex;
	}

	std::pair<iterator, bool> insert(_Kty key, _Ty val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return map::insert(value_type( key, val ) );
	}

	void erase(iterator _Where)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		map::erase(_Where);
	}

	void erase(iterator _First, iterator _Last)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		map::erase(_First, _Last);
	}

	size_type erase(const key_type& _Keyval)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return map::erase(_Keyval);
	}

	void clear()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		map::clear();
	}

	iterator find(const key_type& _Keyval)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return map::find(_Keyval);
	};

	size_type count(const key_type& _Keyval)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return map::count(_Keyval);
	}

	size_type size()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return map::size();
	}

	bool empty()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return map::empty();
	}
};

// TEMPLATE CLASS multimap
template <class _Kty, class _Ty>
class multimap_ts : public std::multimap<_Kty, _Ty>
{
public:
	typedef std::multimap<_Kty, _Ty> _multimap;
	typedef typename _multimap::iterator _iterator;
	typedef typename _multimap::value_type _Tyue_type;
	typedef typename _multimap::_Pairii _Pairii;

private:
	boost::recursive_mutex				m_mutex;

public:
	multimap_ts() {}
	~multimap_ts() {}

	boost::recursive_mutex &getmutex()
	{
		return m_mutex;
	}

	void insert(_Kty key, _Ty val)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		_multimap::insert( _Tyue_type( key, val ) );
	}

	_iterator erase(_iterator _Where)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return _multimap::erase(_Where);
	}

	_iterator erase(_iterator _First, _iterator _Last)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return _multimap::erase(_First, _Last);
	}

	_iterator erase(_Kty key)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		_iterator it = find( key );
		return _multimap::erase( it );
	}

	_iterator erase(_Kty *_First,  _Kty *_Last)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return _multimap::erase(_First, _Last);
	}

	void clear()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		_multimap::clear();
	}

	int count(_Kty key)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return (int)_multimap::count(key);
	}

	_iterator find(_Kty key)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return _multimap::find(key);
	}

	_iterator lower_bound(_Kty key)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return _multimap::lower_bound(key);
	}

	_iterator upper_bound(_Kty key)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return _multimap::upper_bound(key);
	}

	_Pairii equal_range(_Kty key)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return _multimap::equal_range(key);
	}

	int size()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return (int)_multimap::size();
	}

	bool empty()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		return _multimap::empty();
	}
};

#endif
