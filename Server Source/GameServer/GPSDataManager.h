#ifndef __GPS_DATA_MANAGER__
#define __GPS_DATA_MANAGER__

#include <vector>
#include <map>
class CPC;
class CGPSDataManager
{
public:
	CGPSDataManager();
	~CGPSDataManager();
	static CGPSDataManager* instance();

	typedef std::map< int, std::vector<int> > _MAP;
	_MAP gpsMap;

public:
	//Data
	void _insert(int charIndex, int targetIndex);	//gps 아이템을 사용하였을 때
	void _remove(int charIndex, int targetIndex);	//아이템 사용 시간이 지나서 데이터를 삭제해야되는 경우
	void _update(CPC* targetPc);				//타겟인 유저가 접속하였을 때
};

#endif