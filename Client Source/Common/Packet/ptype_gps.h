#ifndef __PTYPE_GPS_H__
#define __PTYPE_GPS_H__

#include "ptype_base.h"

//MessageType
enum
{
	//request
	MSG_SUB_GPS_ADD_TARGET_REQ,
	//response
	MSG_SUB_GPS_ERROR,
	//update
	MSG_SUB_GPS_TARGET_INFO,
	MSG_SUB_GPS_TARGET_MOVE_INFO,
	MSG_SUB_ARTIFACT_GPS_DATA,
};

#pragma pack(push, 1)

namespace RequestClient
{
	struct GPSAddTarget : public pTypeBase
	{
		short tab;
		int invenIndex;
		char UserName[MAX_CHAR_NAME_LENGTH];
	};
}

namespace ResponseClient
{
	enum
	{
		ERROR_GPS_ADD_FAIL,
		ERROR_GPS_NOT_FOUND_USER,
	};

	struct GPSError : public pTypeBase
	{
		int errorCode;
	};

#ifndef _CLIENT_
	inline void makeGPSErrorMsg(CNetMsg::SP& rmsg, int errorCode)
	{
		GPSError* packet = reinterpret_cast<GPSError*>(rmsg->m_buf);
		packet->type = MSG_GPS;
		packet->subType = MSG_SUB_GPS_ERROR;
		packet->errorCode = errorCode;
		rmsg->setSize(sizeof(GPSError));
	}
#endif
}

namespace UpdateClient
{
	struct GPSAddTarget : public pTypeBase
	{
		int charIndex;
		char targetName[MAX_CHAR_NAME_LENGTH];
	};

	struct GPSTargetMoveInfo : public pTypeBase
	{
		bool isSameZone;		//false ¸é ? Ã³¸®
		float x;
		float z;
		float h;
		char yLayer;
	};

	struct ArtifactUserInfo
	{
		int char_index;
		float x;
		float z;
		float h;
		char yLayer;
	};

	struct ArtifactGPSData : public pTypeBase
	{
		int _count;
		ArtifactUserInfo info[0];
	};

#ifndef _CLIENT_
	inline void makeGPSTargetInfoMsg(CNetMsg::SP& rmsg, int targetIndex, const char* targetName)
	{
		GPSAddTarget* packet = reinterpret_cast<GPSAddTarget*>(rmsg->m_buf);
		packet->type = MSG_GPS;
		packet->subType = MSG_SUB_GPS_TARGET_INFO;
		packet->charIndex = targetIndex;
		strcpy(packet->targetName, targetName);
		rmsg->setSize(sizeof(GPSAddTarget));
	}
	
	inline void makeGPSTargetMoveInfo(CNetMsg::SP& rmsg, bool isSameZone, float x, float z, float h, char yLayer)
	{
		GPSTargetMoveInfo *packet = reinterpret_cast<GPSTargetMoveInfo*>(rmsg->m_buf);
		packet->type = MSG_GPS;
		packet->subType = MSG_SUB_GPS_TARGET_MOVE_INFO;
		packet->isSameZone = isSameZone;
		packet->x = x;
		packet->z = z;
		packet->h = h;
		packet->yLayer = yLayer;
		rmsg->setSize(sizeof(GPSTargetMoveInfo));
	}
#endif
}

#pragma pack(pop)

#endif