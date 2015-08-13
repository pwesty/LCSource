//안태훈 수정 시작	//(Remake Effect)(0.1)
#include "stdH.h"

#include "CParticlesProcessVelocity.h"
#include "CRandomTable.h"
#include <Engine/Base/Memory.h>
#include <Engine/Math/AdditionalFunction.h>

//NEW_DELETE_IMPLEMENT(CParticlesProcessVelocity);

CParticlesProcessVelocity::CParticlesProcessVelocity()
: m_vVelocityDir(0,0,0)
, m_fSpeed(0)
{
	m_eType = PCPT_VELOCITY;
}

CParticlesCommonProcess *CParticlesProcessVelocity::Copy()
{
	CParticlesProcessVelocity *pPPV = new CParticlesProcessVelocity;
	pPPV->SetVelocityDir(m_vVelocityDir);
	pPPV->SetVelocitySpd(m_fSpeed);
	return pPPV;
}

void CParticlesProcessVelocity::Process(CParticles &particles, const FLOAT fDeltaTime)
{
	if(fDeltaTime == 0.0f) return;
	UINT cntParticle = particles.Size();
	FLOAT3D velLocal = m_vVelocityDir.SafeNormalize();
	if(!IsNormalFloat(velLocal(1)) || !IsNormalFloat(velLocal(1)) || !IsNormalFloat(velLocal(1)))
	{
		velLocal(1) = velLocal(2) = velLocal(3) = 0;
	}
	RotateVector(velLocal, this->m_quatDir);

	for(UINT iPtr=0; iPtr<cntParticle; ++iPtr)
	{
		CParticle &ptc = particles[iPtr];
		if(ptc.GetVelocity()(1) == 0 && ptc.GetVelocity()(2) == 0 && ptc.GetVelocity()(3) == 0)
		{
			ptc.SetVelocity(velLocal * m_fSpeed);
		}
	}
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CParticlesProcessVelocity::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("PTPF");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_vVelocityDir;
		is >> m_fSpeed;
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticlesProcessVelocity::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("PTPF");
	os << (UBYTE)CURRENT_VERSION;
	
	os << m_vVelocityDir;
	os << m_fSpeed;
}

//안태훈 수정 끝	//(Remake Effect)(0.1)
