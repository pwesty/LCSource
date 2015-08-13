//안태훈 수정 시작	//(Remake Effect)(0.1)
#include "stdH.h"

#include "CParticlesProcessPointGoal.h"
#include "CRandomTable.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CParticlesProcessPointGoal);

CParticlesProcessPointGoal::CParticlesProcessPointGoal()
: m_vPointGoal( 0,0,0 )
, m_fSpeed( 0 )
{
	m_eType = PCPT_POINT_GOAL;
}

CParticlesCommonProcess *CParticlesProcessPointGoal::Copy()
{
	CParticlesProcessPointGoal *pPPPG = new CParticlesProcessPointGoal;
	pPPPG->m_vPointGoal = m_vPointGoal;
	pPPPG->m_fSpeed = m_fSpeed;
	pPPPG->m_fLerpRatio = m_fLerpRatio;
	pPPPG->m_fLerpSpeed = m_fLerpSpeed;
	pPPPG->m_strGoalTagName = m_strGoalTagName;
	return pPPPG;
}

void CParticlesProcessPointGoal::Process(CParticles &particles, const FLOAT fDeltaTime)
{
	if(fDeltaTime == 0.0f) return;
	UINT cntParticle = particles.Size();
	FLOAT3D vPos = m_vCenter;
	if(m_ptrGoalTag.NotNull())
	{
		FLOAT3D vRotatedGoal = m_vPointGoal;
		RotateVector(vRotatedGoal, m_ptrGoalTag->CurrentTagInfo().m_qRot);
		vPos = vRotatedGoal + m_ptrGoalTag->CurrentTagInfo().m_vPos;
	}
	else
	{
		vPos += m_vPointGoal;
	}

	for(UINT iPtc=0; iPtc<cntParticle; ++iPtc)
	{
		CParticle &ptc = particles[iPtc];
		FLOAT3D dist = vPos - ptc.GetPosition();
		dist.Normalize();
		ptc.SetAcceleration(FLOAT3D(0,0,0));

		FLOAT3D vLerped = Lerp(ptc.GetVelocity(), dist * m_fSpeed, m_fLerpRatio);
		FLOAT3D velocity = Lerp(ptc.GetVelocity(), vLerped, fDeltaTime / m_fLerpSpeed);
		ptc.SetVelocity( velocity );
	}
}

#define WANT_TAG_COUNT 1
INDEX CParticlesProcessPointGoal::GetWantTagCount()
{
	if(!m_strGoalTagName.empty()) return WANT_TAG_COUNT;
	else return 0;
}

const char *CParticlesProcessPointGoal::GetWantTagName(INDEX index)
{
	return m_strGoalTagName.c_str();
}

void CParticlesProcessPointGoal::SetWantTag(INDEX index, ptr_tag tag)
{
	if(index < 0 || index >= WANT_TAG_COUNT) return;
	m_ptrGoalTag = tag;
}


#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 2

void CParticlesProcessPointGoal::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("PPPG");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_fLerpRatio;
		is >> m_fLerpSpeed;
		is >> m_fSpeed;
		is >> m_vPointGoal(1);
		is >> m_vPointGoal(2);
		is >> m_vPointGoal(3);
		CTString strTagName;
		is >> strTagName;
		m_strGoalTagName = strTagName.str_String;
	}
	//old version
	else if(ubVer == 1)
	{
		is >> m_fLerpRatio;
		is >> m_fLerpSpeed;
		is >> m_fSpeed;
		is >> m_vPointGoal(1);
		is >> m_vPointGoal(2);
		is >> m_vPointGoal(3);
	}
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticlesProcessPointGoal::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("PPPG");
	os << (UBYTE)CURRENT_VERSION;

	os << m_fLerpRatio;
	os << m_fLerpSpeed;
	os << m_fSpeed;
	os << m_vPointGoal(1);
	os << m_vPointGoal(2);
	os << m_vPointGoal(3);
	CTString strTagName = m_strGoalTagName.c_str();
	os << strTagName;
}

//안태훈 수정 끝	//(Remake Effect)(0.1)
