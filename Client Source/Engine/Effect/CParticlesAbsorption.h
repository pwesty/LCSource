//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CPARTICLESABSORPTION_H__
#define __CPARTICLESABSORPTION_H__

#include <Engine/Base/Memory.h>
#include <Engine/Base/Types.h>
#include <Engine/Math/Functions.h>
#include "CParticles.h"
#include "CRefCountPtr.h"
#include "FreeMemStack.h"

enum PARTICLES_ABSORPTION_TYPE
{
	PAT_NONE	= 0,
	PAT_DEFAULT,
	PAT_SPHERE,
};

enum PARTICLE_MOVE_TYPE
{
	PMT_NONMOVE			= 0,
	PMT_VELOCITY,
	PMT_ACCELERATION,
	PMT_LOCALPOS_NONMOVE,
	PMT_LOCALPOS_VELOCITY,
	PMT_LOCALPOS_ACCELERATION,
};

class ENGINE_API CParticlesAbsorption// : public CParticlesProcess
{
public:
	CParticlesAbsorption();
	virtual ~CParticlesAbsorption();

	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);

	inline PARTICLES_ABSORPTION_TYPE GetType()	{ return m_patType; }

	inline void SetMoveType(PARTICLE_MOVE_TYPE pmType)	{ m_pmtType = pmType;	}
	inline PARTICLE_MOVE_TYPE GetMoveType()				{ return m_pmtType;		}

	inline void SetDependLife(BOOL bDependLife)		{ m_bDependLife = bDependLife;	}
	inline BOOL GetDependLife()						{ return m_bDependLife;			}

	inline void SetPosition(const FLOAT3D &center)
	{
		m_vOldCenter = m_vCenter;
		m_vDeltaCenter = center - m_vCenter;
		m_vCenter = center;
	}
	inline void SetDirection(const FLOATquat3D &dir)
	{
		m_quatOldDir = m_quatDir;
		//m_quatDeltaDir = m_quatOldDir.Inv();
		m_quatDeltaDir = dir * m_quatOldDir.Inv();
		m_quatDir = dir;
	}
	inline const FLOAT3D &GetPosition()				{ return m_vCenter;			}
	inline const FLOATquat3D &GetDirection()		{ return m_quatDir;			}

	virtual CParticlesAbsorption *Copy() = 0;
	virtual void Process(CParticles &particles, const FLOAT fDeltaTime) = 0;
protected:
	inline void SetContent(CParticlesAbsorption &pa)
	{
		m_pmtType = pa.m_pmtType;
		m_bDependLife = pa.m_bDependLife;
	}
	void MoveProcess(CParticle &ptc, FLOAT deltaTime);
	inline BOOL LifeProcess(CParticle &ptc, FLOAT deltaTime)
	{
		if(m_bDependLife) return ptc.Live(deltaTime);
		return TRUE;
	}
protected:
	//content
	PARTICLES_ABSORPTION_TYPE	m_patType;		//타입.
	PARTICLE_MOVE_TYPE			m_pmtType;		//파티클 움직임 타입.
	BOOL						m_bDependLife;	//Life감소 처리 여부.
	//instance
	FLOAT3D		m_vCenter;		//Absorption의 중심
	FLOATquat3D	m_quatDir;		//Absorption의 방향
	FLOAT3D		m_vOldCenter;	//Absorption의 중심(old)
	FLOATquat3D	m_quatOldDir;	//Absorption의 방향(old)
	FLOAT3D		m_vDeltaCenter;	//Absorption의 중심의 변화
	FLOATquat3D	m_quatDeltaDir;	//Absorption의 방향의 변화
};

typedef CRefCountPtr<CParticlesAbsorption> ptr_absoprtion;

#endif //__CPARTICLESABSORPTION_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
