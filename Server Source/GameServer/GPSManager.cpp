#include "stdhdrs.h"

#include "PCList.h"
#include "CmdMsg.h"
#include "DBManager.h"
#include "GPSManager.h"
#include "Artifact_Manager.h"
#include "../ShareLib/packetType/ptype_gps.h"
#include "../ShareLib/packetType/ptype_artifact.h"

#include <boost/format.hpp>


CGPSManager::CGPSManager(CPC* pc)
	:_owner(pc), m_isGps(false), m_isGpsInterrupt(false), m_gpsTarget(NULL)
{
}

CGPSManager::~CGPSManager()
{
}

bool CGPSManager::isGps()
{
	return m_isGps;	
}

bool CGPSManager::isGpsInterrupt()
{
	return m_isGpsInterrupt;
}

void CGPSManager::sendGpsTargetMoveInfo()
{
	//gps 버프 체크
	if(this->isGps() == false)
		return;

	//캐릭터가 로그오프 했을 시에
	if(m_gpsTarget != NULL && m_gpsTarget->m_type == MSG_CHAR_UNKNOWN)
		m_gpsTarget = NULL;

	//캐릭터가 로그인 상태가 아니라면
	if(m_gpsTarget == NULL)
	{
		sendNotFound();
	}

	//캐릭터가 로그인 상태라면
	else
	{
		//위치 아이템 방해중이면
		if(m_gpsTarget->m_gpsManager.isGpsInterrupt() == true)
		{
			sendNotFound();
			return;
		}

		//같은 존에 있는지 검사
		if(m_gpsTarget->m_pZone->m_index == _owner->m_pZone->m_index)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::makeGPSTargetMoveInfo(rmsg, true, m_gpsTarget->m_pos.m_x, m_gpsTarget->m_pos.m_z, m_gpsTarget->m_pos.m_h, m_gpsTarget->m_pos.m_yLayer);
			SEND_Q(rmsg, _owner->m_desc);
			isNotFoundSend = false;
		}
		else
		{
			sendNotFound();
		}
	}
}

void CGPSManager::setGpsTarget( CPC* targetPC )
{
	this->m_gpsTarget = targetPC;
}

void CGPSManager::setIsGps( bool gps )
{
	this->m_isGps = gps;
}	

void CGPSManager::setIsGpsInterrupt( bool gpsInterrupt )
{
	this->m_isGpsInterrupt = gpsInterrupt;
}

CPC* CGPSManager::getGpsTarget()
{
	return m_gpsTarget;
}

int CGPSManager::getGpsTargetIndex()
{
	return m_gpsTargetIndex;
}

void CGPSManager::inputTargetInfo(int targetIndex, const char* targetName)
{
	CGPSDataManager::instance()->_remove(_owner->m_index, _owner->m_gpsManager.getGpsTargetIndex());
	
	CPC* targetPC = PCManager::instance()->getPlayerByCharIndex(targetIndex);
	CGPSDataManager::instance()->_insert(_owner->m_index, targetIndex);	
	
	if(targetPC != NULL)
	{
		setGpsTarget(targetPC);
	}

	m_gpsTargetIndex = targetIndex;
	m_gpsTargetName = targetName;
}


void CGPSManager::save(std::vector<std::string> & vec)
{
	std::string delete_query = boost::str(boost::format("DELETE FROM t_gps_data where a_pursuerIndex = %d") % _owner->m_index);
	vec.push_back(delete_query);
	if(isGps() == true)
	{
		std::string insert_query = boost::str(boost::format("INSERT INTO t_gps_data values(%d, %d, '%s')") % _owner->m_index % this->m_gpsTargetIndex % this->m_gpsTargetName.c_str());
		vec.push_back(insert_query);
	}
}

void CGPSManager::load(MYSQL* conn)
{
	CDBCmd dbChar;
	dbChar.Init(conn);
	std::string query = boost::str(boost::format("SELECT * FROM t_gps_data where a_pursuerIndex = %d") % _owner->m_index);

	dbChar.SetQuery(query);
	if( dbChar.Open() == false)
	{
		LOG_ERROR("query error : error[%s], query[%s]", mysql_error(&gserver->m_dbchar), query.c_str());
	}
		
	while(dbChar.MoveNext())
	{
		dbChar.GetRec("a_targetIndex", this->m_gpsTargetIndex);
		dbChar.GetRec("a_targetName", this->m_gpsTargetName);

		this->inputTargetInfo(this->m_gpsTargetIndex, this->m_gpsTargetName.c_str());

		CGPSDataManager::instance()->_insert(_owner->m_index, this->m_gpsTargetIndex);
	}
}

void CGPSManager::deleteTarget()
{
	CGPSDataManager::instance()->_remove(_owner->m_index, this->m_gpsTargetIndex);

	this->m_gpsTarget = NULL;
	this->m_gpsTargetIndex = -1;
	this->m_gpsTargetName = "";
}

void CGPSManager::sendNotFound()
{
	//한번이라도 보낸적 있으면 리턴
	if(isNotFoundSend == true)
		return;

	//물음표 send
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makeGPSTargetMoveInfo(rmsg, false, -1, -1, -1, -1);
	SEND_Q(rmsg, _owner->m_desc);

	isNotFoundSend = true;
}

void CGPSManager::sendTargetInfo()
{
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makeGPSTargetInfoMsg(rmsg, m_gpsTargetIndex, m_gpsTargetName.c_str());
	SEND_Q(rmsg, _owner->m_desc);
}

void ArtifactGPSManager::setIsGps( bool gps )
{
	this->m_isGps = gps;
}

bool ArtifactGPSManager::isGps()
{
	return m_isGps;
}

void ArtifactGPSManager::sendGpsTargetMoveInfo()
{
	if(this->isGps() == false)
		return;

	ArtifactManager::instance()->sendOwnerGPSData(_owner);
}

ArtifactGPSManager::ArtifactGPSManager( CPC* pc )
	: _owner(pc), m_isGps(false)
{
}

ArtifactGPSManager::~ArtifactGPSManager()
{
}

void ArtifactGPSManager::sendGpsOwnerCountInfo()
{
	if(isGps() == true)
	{
		int count = 0;
		count = ArtifactManager::instance()->getSameZoneCount(_owner);
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeArtifactFindUserCountMsg(rmsg, count);
		SEND_Q(rmsg, _owner->m_desc);
	}
}
