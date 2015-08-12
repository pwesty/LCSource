#ifndef __MAP_FOR_FUNCTION_H__
#define __MAP_FOR_FUNCTION_H__

#include <boost/unordered_map.hpp>
#include <boost/function.hpp>

template<typename _Key, typename _Func>
class mapForFunction
{
public:
	typedef typename boost::function<_Func> function_t;
	typedef typename boost::unordered_map<_Key, function_t> map_t;
	typedef typename boost::unordered_map<_Key, function_t>::iterator map_t_iterator;
	typedef typename boost::unordered_map<_Key, function_t>::value_type map_t_value_type;

public:
	mapForFunction() {}
	~mapForFunction() {}

	void add(_Key key, function_t func)
	{
		map_.insert(map_t_value_type(key, func));
	}

	function_t* get(_Key key)
	{
		map_t_iterator it = map_.find(key);
		return (it != map_.end()) ? &(it->second) : NULL;
	}

private:
	map_t map_;
};

#endif
