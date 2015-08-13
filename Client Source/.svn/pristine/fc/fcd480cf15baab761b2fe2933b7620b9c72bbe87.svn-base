//안태훈 수정 시작	//(Remake Effect)(0.1)

#include "stdH.h"

#include "CParticlesEmitterCylinder.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CParticlesEmitterCylinder);

CParticlesEmitterCylinder::CParticlesEmitterCylinder()
{
	m_petType = PET_CYLINDER;
	m_bEmitAllDir = FALSE;
}

CParticlesEmitterCylinder::~CParticlesEmitterCylinder()
{
}

CParticlesEmitter *CParticlesEmitterCylinder::Copy()
{
	CParticlesEmitterCylinder *pEmitter = new CParticlesEmitterCylinder;
	//CParticleEmitter
	pEmitter->SetContent(*this);
	//CParticlesEmitterCylinder
	pEmitter->m_bUseShapePosition = m_bUseShapePosition;
	pEmitter->m_bUseShapeSpeed = m_bUseShapeSpeed;
	pEmitter->m_fSpeedLower = m_fSpeedLower;
	pEmitter->m_fSpeedUpper = m_fSpeedUpper;
	pEmitter->m_cdsShape = m_cdsShape;
	pEmitter->m_bEmitAllDir = m_bEmitAllDir;
	return pEmitter;
}

void CParticlesEmitterCylinder::Process(CParticles &particles, const FLOAT fDeltaTime)
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
		particle.SetPosition( m_vCenter );
//		particle.SetDirection( FLOATquat3D(1,0,0,0) );
		//파 티클 위치 결정
		FLOAT3D posNoRot(0,0,0);
		if(m_bUseShapePosition)
		{
			posNoRot = m_cdsShape.GetPointInThis();
			if(!m_bLocalType)
			{
				FLOAT3D pos = VectorRotate(posNoRot, m_quatDir);
				particle.SetPosition( pos + this->GetPosition() );
			}
			else
			{
				particle.SetPosition(posNoRot);
			}
		}
		//파티클 속도, 가속도 결정
		if(m_bUseShapeSpeed)
		{
			if(!m_bEmitAllDir)
			{
				FLOAT3D pos(0,1,0);
				if(!m_bLocalType) pos = VectorRotate(pos, m_quatDir);
				particle.SetVelocity( pos * (RndFactor() * (m_fSpeedUpper - m_fSpeedLower) + m_fSpeedLower) );
			}
			else
			{
				FLOAT3D center = FLOAT3D(0, m_cdsShape.GetHeight() * 0.5f, 0) + m_cdsShape.GetCenter();
				posNoRot -= center;
				if(m_bUseShapePosition)
				{
					posNoRot = VectorRotate(posNoRot, m_quatDir);
					posNoRot *= (RndFactor() * (m_fSpeedUpper - m_fSpeedLower) + m_fSpeedLower);
					particle.SetVelocity(posNoRot);
				}
				else
				{
					FLOAT3D dir = RndDir();
					dir *= (RndFactor() * (m_fSpeedUpper - m_fSpeedLower) + m_fSpeedLower);
					particle.SetVelocity(dir);
				}
			}
		}
	}
}

void CParticlesEmitterCylinder::SetShapeSize(FLOAT size)
{
	m_cdsShape.SetHeight(m_cdsShape.GetHeight() >= 0 ? size : -size);
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 2

void CParticlesEmitterCylinder::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CParticlesEmitter::Read(pIS);

	is.ExpectID_t("PECY");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_fSpeedLower;
		is >> m_fSpeedUpper;
		is >> m_bUseShapePosition;
		is >> m_bUseShapeSpeed;
		is >> m_bEmitAllDir;
		m_cdsShape.Read(&is);
	}
	else if(ubVer == 1)
	{
		is >> m_fSpeedLower;
		is >> m_fSpeedUpper;
		is >> m_bUseShapePosition;
		is >> m_bUseShapeSpeed;
		m_bEmitAllDir = FALSE;
		m_cdsShape.Read(&is);
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticlesEmitterCylinder::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CParticlesEmitter::Write(pOS);

	os.WriteID_t("PECY");
	os << (UBYTE)CURRENT_VERSION;

	os << m_fSpeedLower;
	os << m_fSpeedUpper;
	os << m_bUseShapePosition;
	os << m_bUseShapeSpeed;
	os << m_bEmitAllDir;
	m_cdsShape.Write(&os);
}
//안태훈 수정 끝	//(Remake Effect)(0.1)
