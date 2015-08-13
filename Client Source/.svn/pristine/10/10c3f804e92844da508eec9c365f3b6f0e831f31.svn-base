//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CPARTICLESPROCESSVELOCITY_H__
#define __CPARTICLESPROCESSVELOCITY_H__

#include <Engine/Base/Memory.h>
#include <vector>
#include <algorithm>

#include "CParticles.h"
#include "CParticlesCommonProcess.h"
#include "CSampleSpline.h"

class ENGINE_API CParticlesProcessVelocity : public CParticlesCommonProcess
{
public:
	CParticlesProcessVelocity();

	//NEW_DELETE_DEFINITION(CParticlesProcessVelocity);

	inline void SetVelocityDir(const FLOAT3D &velocity)	{ m_vVelocityDir = velocity;	}
	inline const FLOAT3D &GetVelocityDir()				{ return m_vVelocityDir;		}
	inline void SetVelocitySpd(const FLOAT speed)		{ m_fSpeed = speed;				}
	inline FLOAT GetVelocitySpd()						{ return m_fSpeed;				}

	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);

	virtual CParticlesCommonProcess *Copy();
	virtual void Process(CParticles &particles, const FLOAT fDeltaTime);
protected:
	FLOAT3D	m_vVelocityDir;
	FLOAT	m_fSpeed;
};

#endif //__CPARTICLESPROCESSVELOCITY_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
