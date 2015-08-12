#include "stdhdrs.h"

#include "PCList.h"
#include "CmdMsg.h"
#include "GPSDataManager.h"

#include <boost/format.hpp>


CGPSDataManager::CGPSDataManager()
{
}

CGPSDataManager::~CGPSDataManager()
{
}

CGPSDataManager* CGPSDataManager::instance()
{
	static CGPSDataManager __instance;
	return &__instance;
}

void CGPSDataManager::_insert( int charIndex, int targetIndex )
{
	_MAP::iterator it;
	it = gpsMap.find(targetIndex);
	
	if(it == gpsMap.end())
	{
		std::vector<int> _vec;
		_vec.push_back(charIndex);
		gpsMap.insert(std::pair< int,std::vector<int> >(targetIndex, _vec));
	}
	else
	{
		it->second.push_back(charIndex);
	}
}

void CGPSDataManager::_remove( int charIndex, int targetIndex )
{
	_MAP::iterator it_map;
		
	it_map = gpsMap.find(targetIndex);

	if(it_map == gpsMap.end())
		return;

	std::vector<int>::iterator it_vec = it_map->second.begin();
	std::vector<int>::iterator it_vec_end = it_map->second.end();

	for(; it_vec != it_vec_end; it_vec++)
	{
		if((*it_vec) == charIndex)
		{
			it_map->second.erase(it_vec);
			if(it_map->second.size() == 0)
			{
				gpsMap.erase(it_map);
			}
			return;
		}
	}
}

void CGPSDataManager::_update( CPC* targetPC )
{
	//날 타겟으로 하고 있는 유저들에게 내 포인터 정보를 준다.
	_MAP::iterator it;

	it = gpsMap.find(targetPC->m_index);

	if(it == gpsMap.end())
		return;

	std::vector<int>::iterator it_vec = it->second.begin();
	std::vector<int>::iterator it_vec_end = it->second.end();

	CPC* pc;

	for(; it_vec != it_vec_end; it_vec++)
	{
		pc = PCManager::instance()->getPlayerByCharIndex(*it_vec);
		
		if(pc == NULL)
			continue;
		
		pc->m_gpsManager.setGpsTarget(targetPC);
	}
}

