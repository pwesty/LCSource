//안태훈 수정 시작	//(Remake Effect)(0.1)

#include "stdH.h"

#include "CParticlesEmitterSphere.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CParticlesEmitterSphere);

CParticlesEmitterSphere::CParticlesEmitterSphere()
{
	m_petType = PET_SPHERE;
}

CParticlesEmitterSphere::~CParticlesEmitterSphere()
{
}

CParticlesEmitter *CParticlesEmitterSphere::Copy()
{
	CParticlesEmitterSphere *pEmitter = new CParticlesEmitterSphere;
	//CParticleEmitter
	pEmitter->SetContent(*this);
	//CParticlesEmitterSphere
	pEmitter->m_force = m_force;
	pEmitter->m_fDelayTime = m_fDelayTime;
	pEmitter->m_cdsShape = m_cdsShape;
	return pEmitter;
}

void CParticlesEmitterSphere::Process(CParticles &particles, const FLOAT fDeltaTime)
{
	if(fDeltaTime == 0.0f) return;

	const UINT cntOld = particles.Size();
	INDEX cntAdd = TimeProcessAndAddParticle(fDeltaTime, particles);
	if(cntAdd <= 0) return;

	for(UINT i=cntOld; i<cntOld+cntAdd; ++i)
	{
		CParticle &particle = particles[i];
		//파티클 생성.
		m_particlePrototype.CreateTo( particle );
		particle.SetAge((i-cntOld) * fDeltaTime / (FLOAT)cntAdd);
		particle.SetPosition( FLOAT3D(0,0,0) );
//		particle.SetDirection( FLOATquat3D(1,0,0,0) );
		//파티클 속도, 가속도 결정
		FLOAT3D vPtcPos = m_cdsShape.GetPointInThis();
		FLOAT3D force = m_force.GetForce( vPtcPos );
		RotateVector(force, this->m_quatDir);
		particle.SetAcceleration( force * m_fDelayTime );
		particle.MoveAcceleration( m_fDelayTime );
		//파티클 위치 결정
		if(!m_bLocalType)
		{
			RotateVector(vPtcPos, m_quatDir);
			particle.SetPosition( vPtcPos + this->GetPosition() );
		}
		else
		{
			particle.SetPosition( vPtcPos );
		}
	}
}

void CParticlesEmitterSphere::SetShapeSize(FLOAT size)
{
	m_cdsShape.SetRadius(m_cdsShape.GetCenter(), m_cdsShape.GetRadiusInner(), size);
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CParticlesEmitterSphere::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CParticlesEmitter::Read(pIS);

	is.ExpectID_t("PEMS");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		m_force.Read(&is);
		is >> m_fDelayTime;
		m_cdsShape.Read(&is);
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticlesEmitterSphere::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CParticlesEmitter::Write(pOS);

	os.WriteID_t("PEMS");
	os << (UBYTE)CURRENT_VERSION;

	m_force.Write(&os);
	os << m_fDelayTime;
	m_cdsShape.Write(&os);
}
//안태훈 수정 끝	//(Remake Effect)(0.1)
