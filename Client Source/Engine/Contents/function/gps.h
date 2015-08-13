#ifndef		GPS_FUNCTION_H_
#define		GPS_FUNCTION_H_

#include <Common/Packet/ptype_gps.h>
#include <string>
#include <map>

struct stFindItemInfo
{
	float fX;
	float fY;
	float fZ;
	int nCharIdx;
	char yLayer;

	stFindItemInfo()
	{
		fX = 0.f;
		fY = 0.f;
		fZ = 0.f;
		nCharIdx = -1;
		yLayer = 0;
	}
};

class GPS
{
public:
	GPS();

	void clear();

	void open(bool bGPS = true);
	void close();

	void findUser(const char* strName);
	void addTarget(int nIdx, const char* tarName);
	void setInfo(UpdateClient::GPSTargetMoveInfo& tarInfo);
	UpdateClient::GPSTargetMoveInfo* getInfo()		{ return &m_tarInfo; }
	bool IsPosition()	{ return m_tarInfo.isSameZone; }

	int getTargetID()			{ return m_nTargetIdx; }
	const char* getTarName()	{ return m_strTarget.c_str(); }

	// FindItem 인포 (유물)
	typedef std::map<int, stFindItemInfo> mapRelicPos;
	typedef mapRelicPos::iterator mapRelicPosIter;

	void SetFindItemInfo(UpdateClient::ArtifactGPSData& Info);
	int GetRelicPosCount() { return m_nRelicPosCnt; }
	bool IsValidPos(int nCharIndex)
	{
		m_relicIter = m_mapRelicPos.find(nCharIndex);
		mapRelicPosIter iterEnd = m_mapRelicPos.end();
		return m_relicIter != iterEnd ? true : false;
	}
	mapRelicPos* GetMapRelicPos() { return &m_mapRelicPos; }
	void RelicPosClear();

private:
	int			m_msgID;

	int			m_nTargetIdx;
	std::string m_strTarget;

	UpdateClient::GPSTargetMoveInfo	m_tarInfo;

	bool		m_bPreVisible;


	mapRelicPos			m_mapRelicPos;	// B1407 유물 나침반 사용시 아이템 인포
	mapRelicPosIter		m_relicIter;
	int					m_nRelicPosCnt;
};


#endif		// GPS_FUNCTION_H_