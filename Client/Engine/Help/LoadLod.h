#ifndef		_LOD_LOADER_H_
#define		_LOD_LOADER_H_

#include <map>
#include <vector>

//////////////////////////////////////////////////////////////////////////

template <typename T>
class LodLoader
{
public:
	typedef std::map<int, T*>	_map;
	typedef std::vector<T*>		_vec;

	static bool loadLod_file( const char* str_path )
	{
		FILE*	fp = NULL;
		
		fp = fopen(str_path, "rb");

		if (fp == NULL)
			return false;

		fread(&_nSize, sizeof(int), 1, fp);

		if (_nSize <= 0)
			return false;

		T* pdata = new T[_nSize];
		fread(pdata, sizeof(T) * _nSize, 1, fp);

		for (int i = 0; i < _nSize; i++)
		{
			T* ptmp = new T;
			memcpy(ptmp, &pdata[i], sizeof(T));
			_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp));
			_vecdata.push_back(ptmp);
		}

		fclose(fp);

		if (pdata != NULL)
		{
			delete[] pdata;
			pdata = NULL;
		}

		m_dummy = new T;

		return true;
	}
	static void release()
	{
		std::map<int, T*>::iterator it = _mapdata.begin();
		std::map<int, T*>::iterator it_end = _mapdata.end();

		for (; it != it_end; ++it)
		{
			if ((*it).second != NULL)
			{
				delete (*it).second;
				(*it).second = NULL;
			}
		}
		_mapdata.clear();
		_vecdata.clear();

		if(m_dummy)
		{
			delete m_dummy;
			m_dummy = NULL;
		}
	}
	static int getsize()	{ return (int)_mapdata.size(); }
	static T* getData(int Idx) 
	{
		std::map<int, T*>::iterator it = _mapdata.find(Idx);

		if (it != _mapdata.end())
			return (*it).second;

		return m_dummy; 
	}	

	static T* getDataSeq(int Idx)
	{
		if (Idx < 0 || Idx >= _vecdata.size())
			return m_dummy;

		return _vecdata[Idx];
	}

	static _map* getmapPtr() { return &_mapdata; } 
	
	static _map	_mapdata;
	static _vec _vecdata;
	static int	_nSize;
	static T*	m_dummy;
};

template <typename T>
T* LodLoader<T>::m_dummy;

template <typename T>
int LodLoader<T>::_nSize;

template <typename T>
std::map<int, T*> LodLoader<T>::_mapdata;

template <typename T>
std::vector< T* > LodLoader<T>::_vecdata;

#endif		// _LOD_LOADER_H_