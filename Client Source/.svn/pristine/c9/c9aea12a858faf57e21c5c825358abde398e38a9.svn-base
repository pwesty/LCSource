//안태훈 수정 시작	//(Remake Effect)(0.1)
#include "stdH.h"

#include "CParticlesProcessControl.h"
#include "CRandomTable.h"
#include <Engine/Math/AdditionalFunction.h>
#include <Engine/Math/Functions.h>
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CParticlesProcessControl);

CParticlesProcessControl::CParticlesProcessControl()
: m_eControlType( CT_NONE )
, m_bUseParticlePos( TRUE )
, m_fAngleSpeed( 0 )
, m_fHeightSpeed( 0 )
, m_bDependRadius( FALSE )
{
	m_eType = PCPT_CONTROL;
}

CParticlesCommonProcess *CParticlesProcessControl::Copy()
{
	CParticlesProcessControl *pPPC = new CParticlesProcessControl;
	switch(m_eControlType)
	{
	case CT_CIRCLE:
		{
			pPPC->SetCircleInfo(m_bUseParticlePos, m_fAngleSpeed);
			pPPC->SetDependRadius(m_bDependRadius);
		} break;
	case CT_SPIRAL:
		{
			pPPC->SetSpiralInfo(m_bUseParticlePos, m_fAngleSpeed, m_fHeightSpeed);
			pPPC->SetDependRadius(m_bDependRadius);
		} break;
	default:
		{
			pPPC->m_eControlType = CT_NONE;
		} break;
	}
	return pPPC;
}

void CParticlesProcessControl::SetCircleInfo(BOOL usePtcPos, FLOAT angleSpeed)
{
	m_bUseParticlePos = usePtcPos;
	m_eControlType = CT_CIRCLE;
	m_fAngleSpeed = angleSpeed;
}

void CParticlesProcessControl::SetSpiralInfo(BOOL usePtcPos, FLOAT angleSpeed, FLOAT heightSpeed)
{
	m_bUseParticlePos = usePtcPos;
	m_eControlType = CT_SPIRAL;
	m_fAngleSpeed = angleSpeed;
	m_fHeightSpeed = heightSpeed;
}

void CParticlesProcessControl::Process(CParticles &particles, const FLOAT fDeltaTime)
{
	if(fDeltaTime == 0.0f) return;
	UINT cntParticle = particles.Size();
	//FLOAT area = m_fAreaRadius * m_fAreaRadius;

	FLOAT3D vAxis = VectorOnlyYRotate( 1, m_quatDir ).Normalize();
	FLOATquat3D quatDelta;
	quatDelta.FromAxisAngle(vAxis, RadAngle(fDeltaTime * m_fAngleSpeed));

	switch(m_eControlType)
	{
	case CT_CIRCLE:
		{
			for(UINT iPtc=0; iPtc<cntParticle; ++iPtc)
			{
				CParticle &ptc = particles[iPtc];
				FLOAT3D relPos = ptc.GetPosition() - m_vCenter;
				if(m_bUseParticlePos)
				{
					if(m_bDependRadius)
					{
					}
					else
					{
						relPos = VectorRotate(relPos, quatDelta);
						ptc.SetPosition(relPos + m_vCenter);
					}
				}
				else
				{
					if(m_bDependRadius)
					{
					}
					else
					{
						FLOAT3D newPos = VectorRotate(relPos, quatDelta);
						ptc.SetVelocity( (newPos - relPos) / fDeltaTime );
					}
				}
			}
		} break;
	case CT_SPIRAL:
		{
			for(UINT iPtc=0; iPtc<cntParticle; ++iPtc)
			{
				CParticle &ptc = particles[iPtc];
				FLOAT3D relPos = ptc.GetPosition() - m_vCenter;
				if(m_bUseParticlePos)
				{
					if(m_bDependRadius)
					{
					}
					else
					{
						relPos = VectorRotate(relPos, quatDelta);
						relPos += vAxis * (m_fHeightSpeed * fDeltaTime);
						ptc.SetPosition(relPos + m_vCenter);
					}
				}
				else
				{
					if(m_bDependRadius)
					{
					}
					else
					{
						FLOAT3D newPos = VectorRotate(relPos, quatDelta);
						newPos += vAxis * (m_fHeightSpeed * fDeltaTime);
						ptc.SetVelocity( (newPos - relPos) / fDeltaTime );
					}
				}
			}
		} break;
	default: {} break;
	}
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 2

void CParticlesProcessControl::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("PTPC");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> (DWORD&)m_eControlType;
		is >> m_bUseParticlePos;
		is >> m_fAngleSpeed;
		is >> m_fHeightSpeed;
		is >> m_bDependRadius;
	}
	else if(ubVer == 1)
	{
		is >> (DWORD&)m_eControlType;
		is >> m_bUseParticlePos;
		is >> m_fAngleSpeed;
		is >> m_fHeightSpeed;
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticlesProcessControl::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("PTPC");
	os << (UBYTE)CURRENT_VERSION;

	os << (DWORD)m_eControlType;
	os << m_bUseParticlePos;
	os << m_fAngleSpeed;
	os << m_fHeightSpeed;
	os << m_bDependRadius;
}

//안태훈 수정 끝	//(Remake Effect)(0.1)
