//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CPARTICLESCOMMONPROCESS_H__
#define __CPARTICLESCOMMONPROCESS_H__

#include <Engine/Base/Memory.h>
#include <Engine/Base/Types.h>
#include "CParticles.h"
#include "CRefCountPtr.h"
#include "CTag.h"
#include "FreeMemStack.h"

enum PARTICLES_COMMON_PROCESS_TYPE
{
	PCPT_NONE			= 0,
	PCPT_DYNAMIC_STATE	= 1,
	PCPT_FORCE			= 2,
	PCPT_POINT_GOAL		= 3,
	PCPT_CONTROL		= 4,
	PCPT_VELOCITY		= 5,
};

class ENGINE_API CParticlesCommonProcess// : public CParticlesProcess
{
public:
	CParticlesCommonProcess();
	virtual ~CParticlesCommonProcess() = 0;

	virtual void Read(CTStream *istrFile) = 0;
	virtual void Write(CTStream *ostrFile) = 0;

	inline PARTICLES_COMMON_PROCESS_TYPE GetType()		{ return m_eType; }
	
	inline void SetPosition(const FLOAT3D &center)	{ m_vCenter = center;		}
	inline const FLOAT3D &GetPosition()				{ return m_vCenter;			}
	inline void SetDirection(const FLOATquat3D &dir){ m_quatDir = dir;			}
	inline const FLOATquat3D &GetDirection()		{ return m_quatDir;			}

	virtual CParticlesCommonProcess *Copy() = 0;
	virtual void Process(CParticles &particles, const FLOAT fDeltaTime) = 0;
	
	virtual INDEX GetWantTagCount()						{ return 0;		}
	virtual const char *GetWantTagName(INDEX index)		{ return "";	}
	virtual void SetWantTag(INDEX index, ptr_tag tag)	{}
protected:
	//content
	PARTICLES_COMMON_PROCESS_TYPE m_eType;
	//instance
	FLOAT3D		m_vCenter;		//Process의 중심
	FLOATquat3D	m_quatDir;		//Process의 방향
};

typedef CRefCountPtr<CParticlesCommonProcess> ptr_common_process;

#endif //__CPARTICLESCOMMONPROCESS_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
