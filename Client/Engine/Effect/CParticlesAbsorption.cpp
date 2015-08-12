//안태훈 수정 시작	//(Remake Effect)(0.1)

#include "stdH.h"

#include "CParticlesAbsorption.h"
#include <Engine/Base/Memory.h>

CParticlesAbsorption::CParticlesAbsorption()
: m_patType( PAT_NONE )
{
}

CParticlesAbsorption::~CParticlesAbsorption()
{
}

void CParticlesAbsorption::MoveProcess(CParticle &ptc, FLOAT deltaTime)
{
	switch(m_pmtType)
	{
	case PMT_NONMOVE:		{ ptc.m_vFinalPos = ptc.GetPosition();									} break;
	case PMT_VELOCITY:		{ ptc.MoveVelocity(deltaTime); ptc.m_vFinalPos = ptc.GetPosition();		} break;
	case PMT_ACCELERATION:	{ ptc.MoveAcceleration(deltaTime); ptc.m_vFinalPos = ptc.GetPosition();	} break;
	case PMT_LOCALPOS_NONMOVE:
		{
			ptc.m_vFinalPos = VectorRotate(ptc.GetPosition(), m_quatDir) + m_vCenter;
		} break;
	case PMT_LOCALPOS_VELOCITY:
		{
			ptc.MoveVelocity(deltaTime);
			ptc.m_vFinalPos = VectorRotate(ptc.GetPosition(), m_quatDir) + m_vCenter;
		} break;
	case PMT_LOCALPOS_ACCELERATION:
		{
			ptc.MoveAcceleration(deltaTime);
			ptc.m_vFinalPos = VectorRotate(ptc.GetPosition(), m_quatDir) + m_vCenter;
		} break;
	}
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CParticlesAbsorption::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("PTAB");
	UBYTE ubVer;
	is >> ubVer;
	is >> (DWORD&)m_pmtType;
	is >> m_bDependLife;

	if(ubVer == CURRENT_VERSION)
	{
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error");
	}
}

void CParticlesAbsorption::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("PTAB");
	os << (UBYTE)CURRENT_VERSION;
	os << (DWORD)m_pmtType;
	os << m_bDependLife;
}
//안태훈 수정 끝	//(Remake Effect)(0.1)
