//안태훈 수정 시작	//(Remake Effect)(0.1)

#include "stdH.h"

#include "CParticlesAbsorptionDefault.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CParticlesAbsorptionDefault);

CParticlesAbsorptionDefault::CParticlesAbsorptionDefault()
{
	m_patType = PAT_DEFAULT;
}

CParticlesAbsorptionDefault::~CParticlesAbsorptionDefault()
{
}

CParticlesAbsorption *CParticlesAbsorptionDefault::Copy()
{
	CParticlesAbsorptionDefault *pRet = new CParticlesAbsorptionDefault;
	pRet->SetContent(*this);
	return pRet;
}

void CParticlesAbsorptionDefault::Process(CParticles &particles, const FLOAT fDeltaTime)
{
	if(fDeltaTime == 0.0f) return;
	const UINT count = particles.Size();
	UINT eraseCount = 0;
	for(UINT i=0; i<count-eraseCount; ++i)
	{
		MoveProcess( particles[i], fDeltaTime );
		if( !LifeProcess( particles[i], fDeltaTime ) )//죽었음.
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

void CParticlesAbsorptionDefault::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CParticlesAbsorption::Read(pIS);

	is.ExpectID_t("PADF");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error");
	}
}

void CParticlesAbsorptionDefault::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CParticlesAbsorption::Write(pOS);

	os.WriteID_t("PADF");
	os << (UBYTE)CURRENT_VERSION;
}
//안태훈 수정 끝	//(Remake Effect)(0.1)
