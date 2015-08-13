//안태훈 수정 시작	//(Remake Effect)(0.1)
#include "stdH.h"

#include "CParticlesProcessForce.h"
#include "CRandomTable.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CParticlesProcessForce);

CParticlesProcessForce::CParticlesProcessForce()
{
	m_eType = PCPT_FORCE;
}

CParticlesCommonProcess *CParticlesProcessForce::Copy()
{
	CParticlesProcessForce *pPPF = new CParticlesProcessForce;
	pPPF->m_vectorForce = m_vectorForce;
	return pPPF;
}

void CParticlesProcessForce::Process(CParticles &particles, const FLOAT fDeltaTime)
{
	if(fDeltaTime == 0.0f) return;
	UINT cntParticle = particles.Size();
	UINT cntForce = m_vectorForce.size();

	for(UINT iPtr=0; iPtr<cntParticle; ++iPtr)
	{
		FLOAT3D vForce(0,0,0);
		CParticle &ptc = particles[iPtr];
		for(UINT iForce=0; iForce<cntForce; ++iForce)
		{
			CForce &force = m_vectorForce[iForce];
			vForce += force.GetForce( ptc.GetPosition() - m_vCenter );
		}
		//가속도는 누적하지 않는다.
		ptc.SetAcceleration( vForce / ptc.GetMass() );
	}
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 2

void CParticlesProcessForce::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("PTPF");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		DWORD dwCount;
		is >> dwCount;
		for(DWORD i=0; i<dwCount; ++i)
		{
			CForce force;
			force.Read(&is);
			m_vectorForce.push_back(force);
		}
	}
	else if(ubVer == 1)
	{
		DWORD dwCount;
		is >> dwCount;
		for(DWORD i=0; i<dwCount; ++i)
		{
			CForce force;
			force.Read(&is);
			m_vectorForce.push_back(force);
		}
		FLOAT fRandom;
		is >> fRandom;
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticlesProcessForce::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("PTPF");
	os << (UBYTE)CURRENT_VERSION;

	os << (DWORD)m_vectorForce.size();
	for(int i=0; i<m_vectorForce.size(); ++i)
	{
		m_vectorForce[i].Write(&os);
	}
}

//안태훈 수정 끝	//(Remake Effect)(0.1)
