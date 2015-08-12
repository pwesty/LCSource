#ifndef		_LOADER_H_
#define		_LOADER_H_

#include <map>
#include <vector>

struct stSTRING
{
	std::vector<std::string>		_vecString;
};

class TableLoader
{
public:
	bool load_file(const char* str_path, int strCnt);
	void release();
	
	stSTRING& get_data(int index);
	int getCount()	{ return _map.size(); }

private:
	typedef std::map<int, stSTRING>::iterator	map_iter;
	std::map<int, stSTRING>	_map;
};


#endif		// _LOADER_H_