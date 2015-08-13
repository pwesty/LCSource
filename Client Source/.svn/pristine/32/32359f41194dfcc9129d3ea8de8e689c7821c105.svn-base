//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include "StdH.h"

#include <Engine/Math/Functions.h>
#include <Engine/Base/Types.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Ska/Render.h>
#include <Engine/ska/ModelInstance.h>
#include <Engine/Templates/Stock_CModelInstance.h>
#include <Engine/Graphics/DrawPort.h>
#include <algorithm>

#include "CParticleGroup.h"
#include "CParticleGroupManager.h"
#include "CTag.h"
#include "CParticlesProcessDynamicState.h"
#include "CParticleEffect.h"
#include "CGroupTag.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CParticleEffect);

CParticleEffect::CParticleEffect()
: m_pParticleGroup( NULL )
, m_eRotation( EOTT_ALWAYS )
, m_bOnlyStart( FALSE )
{
	CEffect::m_eType = ET_PARTICLE;
}

CParticleEffect::~CParticleEffect()
{
	if(m_pParticleGroup) CParticleGroupManager::Instance().Destroy(m_pParticleGroup);
}

BOOL CParticleEffect::SetParticleGroup(std::string name)
{
	//이미 있으면 뽀개고 초기화.
	if(!m_strParticleGroupName.empty() && m_pParticleGroup != NULL)
	{
		CParticleGroupManager::Instance().Destroy(m_pParticleGroup);
		m_pParticleGroup = NULL;
	}
	if(!CParticleGroupManager::Instance().IsRegistered(name)) return FALSE;

	m_pParticleGroup = CParticleGroupManager::Instance().Create(name);
/*
	m_fFadeInTime = 0;
	m_fFadeOutTime = 0;
	CParticleGroup *pPG = CParticleGroupManager::Instance().Create(name);
	for(UINT i=0; i<pPG->GetCommonProcessCount(); ++i)
	{
		if(pPG->GetCommonProcess(i)->GetType() == PCPT_DYNAMIC_STATE)
		{
			m_fFadeInTime = ((CParticlesProcessDynamicState*)pPG->GetCommonProcess(i))->GetFadeInTime();
			m_fFadeOutTime = ((CParticlesProcessDynamicState*)pPG->GetCommonProcess(i))->GetFadeOutTime();
		}
	}
	CParticleGroupManager::Instance().Destroy(pPG);
*/
	
	m_strParticleGroupName.swap(name);

	return TRUE;
}

CEffect *CParticleEffect::Copy()
{
	CParticleEffect *pRet = new CParticleEffect;
	if(pRet == NULL) return NULL;
	//CEffect의 content
	pRet->SetContent(this);
	//CParticleEffect의 content
	pRet->SetParticleGroup(m_strParticleGroupName);
	pRet->m_eRotation =	 m_eRotation;
	return pRet;
}

INDEX CParticleEffect::GetWantTagCount()
{
	if(m_pParticleGroup == NULL) return 0;

	INDEX wantTagCount = 0;
	for(INDEX i=0; i< m_pParticleGroup->GetCommonProcessCount(); ++i)
	{
		wantTagCount += m_pParticleGroup->GetCommonProcess(i)->GetWantTagCount();
	}
	return wantTagCount;
}

const char *CParticleEffect::GetWantTagName(INDEX index)
{
	if(m_pParticleGroup == NULL) return "";

	INDEX wantTagCount = 0;
	for(INDEX i=0; i< m_pParticleGroup->GetCommonProcessCount(); ++i)
	{
		wantTagCount += m_pParticleGroup->GetCommonProcess(i)->GetWantTagCount();
		if(wantTagCount >= index + 1)
		{
			return m_pParticleGroup->GetCommonProcess(i)->GetWantTagName(
				index - wantTagCount - m_pParticleGroup->GetCommonProcess(i)->GetWantTagCount()
				);
		}
	}
	return "";
}

void CParticleEffect::SetWantTag(INDEX index, ptr_tag tag)
{
	if(m_pParticleGroup == NULL) return;

	INDEX wantTagCount = 0;
	for(INDEX i=0; i< m_pParticleGroup->GetCommonProcessCount(); ++i)
	{
		wantTagCount += m_pParticleGroup->GetCommonProcess(i)->GetWantTagCount();
		if(wantTagCount >= index + 1)
		{
			m_pParticleGroup->GetCommonProcess(i)->SetWantTag(
				index - wantTagCount - m_pParticleGroup->GetCommonProcess(i)->GetWantTagCount()
				, tag
				);
		}
	}
}

void CParticleEffect::Start(FLOAT time, BOOL restart)
{
	if(m_pParticleGroup == NULL) return;
	if(!PreStart(time, restart)) return;
	if(m_strParticleGroupName.empty()) m_eState = ES_PLAY_END;
	if(m_eState != ES_NOT_STARTED) return;

	if(restart) m_pParticleGroup->Restart();

	m_bOnlyStart = TRUE;

	PostStart();
}

BOOL CParticleEffect::Process(FLOAT time)
{
	if(m_pParticleGroup == NULL) return FALSE;
	FLOAT fDeltaTime, fProcessedTime;
	BOOL bRet, bRender;
	if(!PreProcess(time, bRet, bRender, fDeltaTime, fProcessedTime))
	{
		if(!bRender) SetNotRenderAtThisFrame();
		return bRet;
	}
	if(m_bOnlyStart)
	{
		if(m_ptrAttachTag->GetType() == TT_GROUP && ((CGroupTag*)m_ptrAttachTag.GetNative())->Count() > 1)
		{
			ptr_tag tag0 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(0);
			ptr_tag tag1 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(1);
			FLOAT3D vDistance = tag0->CurrentTagInfo().m_vPos - tag1->CurrentTagInfo().m_vPos;
			m_pParticleGroup->GetEmitter()->SetShapeSize(vDistance.Length());
			if(tag0->GetName().at(2) == '0') m_ptrTag = tag0;
			else m_ptrTag = tag1;
		}
		else 
		{
			m_ptrTag = m_ptrAttachTag;
		}
	}
	m_ptrAttachTag = m_ptrTag;

	m_pParticleGroup->SetPosition(m_ptrAttachTag->CurrentTagInfo().m_vPos);
	if(m_eRotation == EOTT_START && m_bOnlyStart) m_pParticleGroup->SetDirection(m_ptrAttachTag->CurrentTagInfo().m_qRot);
	else if(m_eRotation == EOTT_ALWAYS)	m_pParticleGroup->SetDirection(m_ptrAttachTag->CurrentTagInfo().m_qRot);
	else m_pParticleGroup->SetDirection(FLOATquat3D(1,0,0,0));
	m_bOnlyStart = FALSE;
	BOOL bLive = m_pParticleGroup->Process(fDeltaTime);

	//if(!bLive) m_fLeftTime = -0.000001f;
	if(m_fLeftTime < 0.0f && m_slRepeat == 0) m_pParticleGroup->Stop();

	PostProcess();
	return TRUE;
}

void CParticleEffect::Stop(FLOAT leftTime)
{
	if(m_pParticleGroup) m_pParticleGroup->GetEmitter()->SetTotalCount(0);
	else m_fLeftTime = 0;

	CEffect::Stop(leftTime);
}


BOOL CParticleEffect::BeginRender(CAnyProjection3D &apr, CDrawPort *pdp)
{
	gfxEnableBlend();
	Particle_PrepareSystem( pdp, apr );
	return TRUE;
}

void CParticleEffect::EndRender(BOOL bRestoreOrtho)
{
	Particle_EndSystem( bRestoreOrtho );
}

void CParticleEffect::Render()
{
	if(!PreRender())
	{
		return;
	}

	if (GetOwner() != NULL)
	{ // HIDDEN 속성의 NPC의 이펙트를 보기 위해서는 캐릭터가 ENF_SHOWHIDDEN을 가지고 있어야 한다.
		if (GetOwner()->IsFlagOn(ENF_HIDDEN) && (CEntity::GetPlayerEntity(0)->IsFlagOff(ENF_SHOWHIDDEN) ||
			(CEntity::GetPlayerEntity(0)->IsFlagOn(ENF_SHOWHIDDEN)&&!GetOwner()->IsEnemy())))//ENF_SHOWHIDDEN이면 npc effect는 볼 수 있다.
			return;
	}	

	//렌더링 
	m_pParticleGroup->Render();

	PostRender();
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 2

void CParticleEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CEffect::Read(pIS);

	is.ExpectID_t("EFPT");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		CTString strTemp;
		is.GetLine_t(strTemp);
		m_strParticleGroupName = strTemp.str_String;
		is >> (DWORD&)m_eRotation;
	}
	else if(ubVer == 1)
	{
		CTString strTemp;
		is.GetLine_t(strTemp);
		m_strParticleGroupName = strTemp.str_String;
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CParticleEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CEffect::Write(pOS);

	os.WriteID_t("EFPT");
	os << (UBYTE)CURRENT_VERSION;

	os.PutLine_t(m_strParticleGroupName.c_str());
	os << (DWORD)m_eRotation;
}

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)