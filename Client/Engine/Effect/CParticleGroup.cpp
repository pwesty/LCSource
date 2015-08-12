//안태훈 수정 시작	//(Remake Effect)(0.1)

#include "stdh.h"
#include "CParticleGroup.h"
#include <Engine/Graphics/Color.h>
#include <Engine/Base/FileName.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Base/Console.h>
#include "CParticleGroup.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CParticleGroup);

CParticleGroup::CParticleGroup()
: m_pParticlesEmitter( NULL )
, m_pParticlesAbsorption( NULL )
, m_eBlendType( PBT_ADD )
, m_eRenderType( PRT_QUAD )
{
}

CParticleGroup::~CParticleGroup()
{
	for(UINT i=0; i<m_vectorCommonProcess.size(); ++i) delete m_vectorCommonProcess[i];
	if(m_pParticlesEmitter) delete m_pParticlesEmitter;
	if(m_pParticlesAbsorption) delete m_pParticlesAbsorption;
	m_particles.EraseLast(m_particles.Size());
}

CParticleGroup *CParticleGroup::Copy()
{
	CParticleGroup	*pParticleGroup = new CParticleGroup;
	pParticleGroup->m_strName = m_strName;
	pParticleGroup->SetTexture(CTFileName(m_fnTexture.c_str()));
	for(UINT i=0; i<m_vectorCommonProcess.size(); ++i)
	{
		pParticleGroup->AddCommonProcess(m_vectorCommonProcess[i]);
	}
	if(m_pParticlesEmitter) pParticleGroup->SetEmitter(m_pParticlesEmitter);
	if(m_pParticlesAbsorption) pParticleGroup->SetAbsorption(m_pParticlesAbsorption);
	pParticleGroup->SetRenderType(m_eRenderType);
	pParticleGroup->m_eBlendType = m_eBlendType;
	pParticleGroup->m_mexWidth = m_mexWidth;
	pParticleGroup->m_mexHeight = m_mexHeight;
	pParticleGroup->m_iCol = m_iCol;
	pParticleGroup->m_iRow = m_iRow;

	return pParticleGroup;
}

void CParticleGroup::SetTexture(const CTFileName &filename)
{
	m_toTexture.SetData_t(filename);
	m_fnTexture = filename.str_String;
}

BOOL CParticleGroup::Process( FLOAT fTimeDelta )
{
	if(m_pParticlesEmitter == NULL || m_pParticlesAbsorption == NULL) return FALSE;
	if(m_particles.Empty() && m_pParticlesEmitter->GetLeftCount() == 0) return FALSE;
	
	m_pParticlesEmitter->Process( m_particles, fTimeDelta );
	UINT count = m_vectorCommonProcess.size();
	for(UINT i=0; i<count; ++i)
	{
		m_vectorCommonProcess[i]->Process( m_particles, fTimeDelta );
	}
	m_pParticlesAbsorption->Process( m_particles, fTimeDelta );
	return TRUE;
}

void CParticleGroup::Stop()
{
	if(m_pParticlesEmitter != NULL)
	{
		m_pParticlesEmitter->SetTotalCount(0);
	}
}

void CParticleGroup::Render()
{
	// Prepare texture
	Particle_PrepareTexture( &m_toTexture, m_eBlendType );
	//Particle_SetTexturePart( m_mexWidth, m_mexHeight, m_iCol, m_iRow );

	switch(m_eRenderType)
	{
	case PRT_QUAD:	m_particles.RenderQuad(m_mexWidth, m_mexHeight);	break;
	case PRT_LINE:	m_particles.RenderLine(m_mexWidth, m_mexHeight);	break;
	case PRT_TRAIL:	m_particles.RenderTrail(m_mexWidth, m_mexHeight);	break;
	case PRT_TRAIL_VELOCITY:	m_particles.RenderTrailVelocity(m_mexWidth, m_mexHeight);	break;
	}

	// Flush
	Particle_Flush();
}

//emitter
#include "CParticlesEmitterSphere.h"
#include "CParticlesEmitterCone.h"
#include "CParticlesEmitterCylinder.h"
inline CParticlesEmitter *CreateEmitter(PARTICLES_EMITTER_TYPE pet)
{
	switch(pet)
	{
	case PET_SPHERE:	return new CParticlesEmitterSphere;
	case PET_CONE:		return new CParticlesEmitterCone;
	case PET_CYLINDER:	return new CParticlesEmitterCylinder;
	default: ASSERTALWAYS("Unknown Emitter Type.");
	}
	return NULL;
}
//absorption
#include "CParticlesAbsorption.h"
#include "CParticlesAbsorptionDefault.h"
#include "CParticlesAbsorptionSphere.h"
inline CParticlesAbsorption *CreateAbsorption(PARTICLES_ABSORPTION_TYPE pat)
{
	switch(pat)
	{
	case PAT_DEFAULT: return new CParticlesAbsorptionDefault;
	case PAT_SPHERE: return new CParticlesAbsorptionSphere;
	default: ASSERTALWAYS("Unknown Absorption Type.");
	}
	return NULL;
}
//process
#include "CParticlesProcessDynamicState.h"
#include "CParticlesProcessForce.h"
#include "CParticlesProcessPointGoal.h"
#include "CParticlesProcessControl.h"
#include "CParticlesProcessVelocity.h"
inline CParticlesCommonProcess *CreateProcess(PARTICLES_COMMON_PROCESS_TYPE pcpt)
{
	switch(pcpt)
	{
	case PCPT_DYNAMIC_STATE: return new CParticlesProcessDynamicState;
	case PCPT_FORCE: return new CParticlesProcessForce;
	case PCPT_POINT_GOAL: return new CParticlesProcessPointGoal;
	case PCPT_CONTROL: return new CParticlesProcessControl;
	case PCPT_VELOCITY: return new CParticlesProcessVelocity;
	default: ASSERTALWAYS("Unknown Process Type.");
	}
	return NULL;
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CParticleGroup::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("PTGR");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		CTString strTemp;
		is.GetLine_t(strTemp);
		m_strName = strTemp.str_String;
		CTFileName fnTexture;
		is >> fnTexture;
		m_fnTexture = fnTexture.str_String;
		is >> (DWORD&)m_eRenderType;
		is >> (DWORD&)m_eBlendType;
		is >> m_mexWidth;
		is >> m_mexHeight;
		is >> m_iCol;
		is >> m_iRow;
		DWORD dwCount;
		is >> dwCount;
		DWORD dwTemp;
		for(DWORD i=0; i<dwCount; ++i)
		{
			is >> dwTemp;
			m_vectorCommonProcess.push_back( CreateProcess((PARTICLES_COMMON_PROCESS_TYPE)dwTemp) );
			m_vectorCommonProcess[i]->Read(&is);
		}
		BOOL bExist;
		is >> bExist;
		if(bExist)
		{
			is >> dwTemp;
			m_pParticlesEmitter = CreateEmitter((PARTICLES_EMITTER_TYPE)dwTemp);
			m_pParticlesEmitter->Read(&is);
		}
		is >> bExist;
		if(bExist)
		{
			is >> dwTemp;
			m_pParticlesAbsorption = CreateAbsorption((PARTICLES_ABSORPTION_TYPE)dwTemp);
			m_pParticlesAbsorption->Read(&is);
		}
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticleGroup::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("PTGR");
	os << (UBYTE)CURRENT_VERSION;

	os.PutLine_t(m_strName.c_str());
	os << CTFileName(m_fnTexture.c_str());
//	CPrintF("%s\n", m_fnTexture.c_str());
	os << (DWORD)m_eRenderType;
	os << (DWORD)m_eBlendType;
	os << m_mexWidth;
	os << m_mexHeight;
	os << m_iCol;
	os << m_iRow;
	os << (DWORD)m_vectorCommonProcess.size();
	for(DWORD i=0; i<m_vectorCommonProcess.size(); ++i)
	{
		os << (DWORD)m_vectorCommonProcess[i]->GetType();
		m_vectorCommonProcess[i]->Write(&os);
	}
	os << (BOOL)(m_pParticlesEmitter != NULL);
	if(m_pParticlesEmitter)
	{
		os << m_pParticlesEmitter->GetType();
		m_pParticlesEmitter->Write(&os);
	}
	os << (BOOL)(m_pParticlesAbsorption != NULL);
	if(m_pParticlesAbsorption)
	{
		os << m_pParticlesAbsorption->GetType();
		m_pParticlesAbsorption->Write(&os);
	}
}

//안태훈 수정 끝	//(Remake Effect)(0.1)
