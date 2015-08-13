//안태훈 수정 시작	//(Remake Effect)(0.1)

#include "stdH.h"

#include "CParticlesEmitter.h"
#include <Engine/Base/Memory.h>

CParticlesEmitter::CParticlesEmitter()
: m_fProcessedTime( 0 )
, m_petType( PET_NONE )
, m_bLocalType( FALSE )
{
}

CParticlesEmitter::~CParticlesEmitter()
{
}

INDEX CParticlesEmitter::TimeProcessAndAddParticle(FLOAT deltaTime, CParticles &particles)
{
	m_fProcessedTime += deltaTime;
	if(m_ulLeftCount <= 0) return 0;

	INDEX cntAdd = 0;
	if(m_fCountPerSec == 999999.f)
	{
		cntAdd = INDEX(m_ulLeftCount);
	}
	else
	{
		INDEX cntExpectCreated = INDEX(m_fProcessedTime * m_fCountPerSec);
		INDEX cntRealCreated = (INDEX)m_ulTotalCount - (INDEX)m_ulLeftCount;
		if(cntRealCreated < 0) cntRealCreated = 0;
		cntAdd = cntExpectCreated - cntRealCreated;
	}

	if(cntAdd <= 0) return 0;
	if(m_ulLeftCount >= cntAdd)
	{
		m_ulLeftCount -= cntAdd;
	}
	else
	{
		cntAdd = m_ulLeftCount;
		m_ulLeftCount = 0;
	}
	return particles.AddNew(cntAdd);
}


#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 2

void CParticlesEmitter::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("PTEM");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_ulTotalCount;
		m_ulLeftCount = m_ulTotalCount;
		is >> m_fCountPerSec;
		m_particlePrototype.Read(&is);
		is >> m_bLocalType;
	}
	else if(ubVer == 1)
	{
		is >> m_ulTotalCount;
		m_ulLeftCount = m_ulTotalCount;
		is >> m_fCountPerSec;
		m_particlePrototype.Read(&is);
		m_bLocalType = FALSE;
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticlesEmitter::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("PTEM");
	os << (UBYTE)CURRENT_VERSION;

	os << m_ulTotalCount;
	os << m_fCountPerSec;
	m_particlePrototype.Write(&os);
	os << m_bLocalType;
}

void CParticlesEmitter::Restart()
{
	m_fProcessedTime = 0;
	m_ulLeftCount += m_ulTotalCount;
}
//안태훈 수정 끝	//(Remake Effect)(0.1)
