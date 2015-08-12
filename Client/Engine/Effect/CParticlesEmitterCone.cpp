//안태훈 수정 시작	//(Remake Effect)(0.1)

#include "stdH.h"

#include "CParticlesEmitterCone.h"
#include <Engine/Math/AdditionalFunction.h>
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CParticlesEmitterCone);

CParticlesEmitterCone::CParticlesEmitterCone()
: m_bUseShapePosition( FALSE )
, m_buseShapeSpeed( TRUE )
{
	m_petType = PET_CONE;
}

CParticlesEmitterCone::~CParticlesEmitterCone()
{
}

CParticlesEmitter *CParticlesEmitterCone::Copy()
{
	CParticlesEmitterCone *pEmitter = new CParticlesEmitterCone;
	//CParticleEmitter
	pEmitter->SetContent(*this);
	//CParticlesEmitterCone
	pEmitter->m_bUseShapePosition = m_bUseShapePosition;
	pEmitter->m_buseShapeSpeed = m_buseShapeSpeed;
	pEmitter->m_fSpeedLower = m_fSpeedLower;
	pEmitter->m_fSpeedUpper = m_fSpeedUpper;
	pEmitter->m_cdsShape = m_cdsShape;
	return pEmitter;
}

void CParticlesEmitterCone::Process(CParticles &particles, const FLOAT fDeltaTime)
{
	if(fDeltaTime == 0.0f) return;

	const UINT cntOld = particles.Size();
	INDEX cntAdd = TimeProcessAndAddParticle(fDeltaTime, particles);
	if(cntAdd <= 0) return;

	FLOAT3D vSpaceCenter = VectorRotate(m_cdsShape.GetCenter(), m_quatDir);
	for(UINT i=cntOld; i<cntOld+cntAdd; ++i)
	{
		CParticle &particle = particles[i];
		//파티클 생성.
		m_particlePrototype.CreateTo( particle );
		particle.SetAge((i-cntOld) * fDeltaTime / (FLOAT)cntAdd);
		particle.SetPosition( m_vCenter + vSpaceCenter );
//		particle.SetDirection( FLOATquat3D(1,0,0,0) );
		//파티클 위치 결정
		FLOAT3D pos = VectorRotate(m_cdsShape.GetPointInThis() - m_cdsShape.GetCenter(), m_quatDir);
		if(m_bUseShapePosition) particle.SetPosition( pos + particle.GetPosition() );
		//파티클 속도, 가속도 결정
		if(m_buseShapeSpeed)
		{
			FLOAT3D nrmPos = pos.SafeNormalize();
			particle.SetVelocity( nrmPos * (RndFactor() * (m_fSpeedUpper - m_fSpeedLower) + m_fSpeedLower) );
			if(m_ePtcType == PRT_TRAIL)
				particle.SetPosition( particle.GetPosition() + nrmPos * particle.GetHeight() );
			else if(m_ePtcType == PRT_TRAIL_VELOCITY)
				particle.SetPosition( particle.GetPosition() + particle.GetVelocity() * particle.GetHeight() );
		}
	}
}

void CParticlesEmitterCone::SetShapeSize(FLOAT size)
{
	//NOTHING
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CParticlesEmitterCone::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CParticlesEmitter::Read(pIS);

	is.ExpectID_t("PECN");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_fSpeedLower;
		is >> m_fSpeedUpper;
		is >> m_bUseShapePosition;
		is >> m_buseShapeSpeed;
		m_cdsShape.Read(&is);
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticlesEmitterCone::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CParticlesEmitter::Write(pOS);

	os.WriteID_t("PECN");
	os << (UBYTE)CURRENT_VERSION;

	os << m_fSpeedLower;
	os << m_fSpeedUpper;
	os << m_bUseShapePosition;
	os << m_buseShapeSpeed;
	m_cdsShape.Write(&os);
}
//안태훈 수정 끝	//(Remake Effect)(0.1)
