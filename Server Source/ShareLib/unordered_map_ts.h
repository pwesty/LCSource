#ifndef __UNORDERED_MAP_TS_H__
#define __UNORDERED_MAP_TS_H__

#include <boost/thread/recursive_mutex.hpp>
#include <boost/unordered_map.hpp>

template<typename Key, typename T>
class unordered_map_ts : public boost::unordered_map<Key, T>
{
public:
	typedef boost::unordered_map<Key, T> unordered_map;

	typedef std::pair<const Key, T> value_type;

	typedef BOOST_DEDUCED_TYPENAME boost::unordered_map<Key, T>::iterator iterator;

	typedef BOOST_DEDUCED_TYPENAME boost::unordered_map<Key, T>::size_type size_type;

public:
	unordered_map_ts() {}
	~unordered_map_ts() {}

	boost::recursive_mutex &getmutex()
	{
		return m_mutex_;
	}

	std::pair<iterator, bool> insert(const Key _key, const T _t)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		return unordered_map::insert(value_type(_key, _t));
	}

	void erase(iterator _Where)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		unordered_map::erase(_Where);
	}

	void erase(iterator _First, iterator _Last)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		unordered_map::erase(_First, _Last);
	}

	size_type erase(const Key& _Keyval)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		return unordered_map::erase(_Keyval);
	}

	void clear()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		unordered_map::clear();
	}

	iterator find(const Key& _Keyval)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		return unordered_map::find(_Keyval);
	};

	size_type count(const Key& _Keyval)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		return unordered_map::count(_Keyval);
	}

	size_type size()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		return unordered_map::size();
	}

	bool empty()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		return unordered_map::empty();
	}

private:
	boost::recursive_mutex			m_mutex_;
};

template<typename Key, typename T>
class unordered_multimap_ts : public boost::unordered_multimap<Key, T>
{
public:
	typedef boost::unordered_multimap<Key, T> unordered_multimap;

	typedef std::pair<const Key, T> value_type;

	typedef BOOST_DEDUCED_TYPENAME boost::unordered_multimap<Key, T>::iterator iterator;

	typedef BOOST_DEDUCED_TYPENAME boost::unordered_map<Key, T>::size_type size_type;

public:
	unordered_multimap_ts() {}
	~unordered_multimap_ts() {}

	boost::recursive_mutex &getmutex()
	{
		return m_mutex_;
	}

	std::pair<iterator, bool> insert(const Key _key, const T _t)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		return unordered_multimap::insert(value_type(_key, _t));
	}

	void erase(iterator _Where)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		unordered_multimap::erase(_Where);
	}

	void erase(iterator _First, iterator _Last)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		unordered_multimap::erase(_First, _Last);
	}

	size_type erase(const Key& _Keyval)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		return unordered_multimap::erase(_Keyval);
	}

	void clear()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		unordered_multimap::clear();
	}

	iterator find(const Key& _Keyval)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		return unordered_multimap::find(_Keyval);
	};

	size_type count(const Key& _Keyval)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		return unordered_multimap::count(_Keyval);
	}

	size_type size()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		return unordered_multimap::size();
	}

	bool empty()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex_);
		return unordered_multimap::empty();
	}

private:
	boost::recursive_mutex			m_mutex_;
};

#endif
