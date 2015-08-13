#ifndef __GPS_MANAGER__
#define __GPS_MANAGER__

#include "mysql.h"

class CDBCmd;

class CGPSManager
{
public:
	explicit CGPSManager(CPC* pc);
	~CGPSManager();

private:
	CPC* _owner;
	CPC* m_gpsTarget;				//타겟 캐릭터
	std::string m_gpsTargetName;	//타켓 이름
	int m_gpsTargetIndex;			//타겟 캐릭터 인덱스
	bool m_isGps;					//gps 아이템 사용 여부
	bool m_isGpsInterrupt;			//gps 방해아이템 사용 여부

	bool isNotFoundSend;			//못찾고 있을때 패킷을 보낼 필요가 없으므로 관리해주는 변수가 필요 (한번이라도 보냈으면 true, 아니면 false)

	void sendNotFound();
public:
	//DataBase
	void save(std::vector<std::string> & vec);
	void load(MYSQL* conn);

	//Data
	void setGpsTarget(CPC* targetPC);
	void setIsGps(bool gps);
	void setIsGpsInterrupt(bool gpsInterrupt);

	CPC* getGpsTarget();
	int getGpsTargetIndex();

	bool isGps();
	bool isGpsInterrupt();

	void inputTargetInfo(int targetIndex, const char* targetName);
	void sendTargetInfo();
	void sendGpsTargetMoveInfo();
	void deleteTarget();
};

class ArtifactGPSManager
{
private:
	bool m_isGps;					//gps 아이템 사용 여부
	CPC* _owner;

public:
	explicit ArtifactGPSManager(CPC* pc);
	~ArtifactGPSManager();

	void setIsGps(bool gps);
	bool isGps();

	void sendGpsTargetMoveInfo();
	void sendGpsOwnerCountInfo();
};

#endif