//안태훈 수정 시작	//(Remake Effect)(0.1)

#include "stdH.h"

#include "CParticlesAbsorptionSphere.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CParticlesAbsorptionSphere);

CParticlesAbsorptionSphere::CParticlesAbsorptionSphere()
{
	m_patType = PAT_SPHERE;
	m_sphere.SetRadius(FLOAT3D(0,0,0), 0.0f, 0.0f);
}

CParticlesAbsorptionSphere::~CParticlesAbsorptionSphere()
{
}

CParticlesAbsorption *CParticlesAbsorptionSphere::Copy()
{
	CParticlesAbsorptionSphere *ppas = new CParticlesAbsorptionSphere;
	ppas->SetContent(*this);
	ppas->SetSphere( this->GetSphere() );
	return ppas;
}

void CParticlesAbsorptionSphere::Process(CParticles &particles, const FLOAT fDeltaTime)
{
	if(fDeltaTime == 0.0f) return;
	const UINT count = particles.Size();
	UINT eraseCount = 0;
	for(UINT i=0; i<count-eraseCount; ++i)
	{
		MoveProcess( particles[i], fDeltaTime );
		if( !LifeProcess( particles[i], fDeltaTime )
		 || m_sphere.IsInThis(particles[i].GetPosition() - m_vCenter) )//죽었음.
		{
			//end를 앞으로 땡기면서 end앞의 값으로 현재값을 덮어씀.
			//이 자리의 값이 맨뒤의 값으로 바꼈으므로 여기에서 루프 다시 처리.
			++eraseCount;
			particles[i] = particles[count-eraseCount];
			--i;
		}
	}
	particles.EraseLast(eraseCount);
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CParticlesAbsorptionSphere::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CParticlesAbsorption::Read(pIS);

	is.ExpectID_t("PASP");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		m_sphere.Read(&is);
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticlesAbsorptionSphere::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CParticlesAbsorption::Write(pOS);

	os.WriteID_t("PASP");
	os << (UBYTE)CURRENT_VERSION;

	m_sphere.Write(&os);
}
//안태훈 수정 끝	//(Remake Effect)(0.1)
