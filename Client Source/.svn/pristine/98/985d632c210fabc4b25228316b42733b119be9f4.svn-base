#include "StdH.h"
#include "COrbitEffect.h"

#include "CWorldTag.h"
#include "CEffectManager.h"
#include <Engine/Math/AdditionalFunction.h>

//NEW_DELETE_IMPLEMENT(COrbitEffect);

COrbitEffect::COrbitEffect()
: m_ulFadeFlag( 0 )
, m_fFadeInDestRadiusMul( 1 )
, m_fFadeInDestPhaseMul( 1 )
, m_fFadeOutDestRadiusMul( 1 )
, m_fFadeOutDestPhaseMul( 1 )
{
	CEffect::m_eType = ET_ORBIT;
}

COrbitEffect::~COrbitEffect()
{
	ClearInstance();
}

void COrbitEffect::SetNotRenderAtThisFrame()
{
	for(INDEX i=0; i<m_vectorEffect.size(); ++i)
	{
		m_vectorEffect[i]->SetNotRenderAtThisFrame();
	}
}

void COrbitEffect::ClearInstance()
{
	INDEX cnt = m_vectorEffect.size();
	for(INDEX i=0; i<cnt; ++i)
	{
		CEffect *pEffect = m_vectorEffect[i];
		CEffectManager::Instance().Destroy(pEffect);
	}
	m_vectorEffect.clear();
	m_vectorTag.clear();
}

void COrbitEffect::Start(FLOAT time, BOOL restart)
{
	if(!PreStart(time, restart)) return;
	if(m_eState != ES_NOT_STARTED) return;
	if(restart)
	{
		PostStart();
		return;
	}

	ClearInstance();

	m_bFirstProcess = TRUE;
	m_vTagPos = FLOAT3D(0,0,0);
	m_qTagRot = FLOATquat3D(1,0,0,0);

	INDEX cnt = m_vectorOrbit.size();
	m_vectorEffect.reserve(cnt);
	for(INDEX i=0; i<cnt; ++i)
	{
		if(m_vectorOrbit[i].strEffectName == m_strName) continue;
		CEffect *pEffect = 
			CEffectManager::Instance().Create(m_vectorOrbit[i].strEffectName);
		if(pEffect == NULL) continue;
		m_vectorEffect.push_back( pEffect );
		tag_ptr ptrTag(new CWorldTag);
		ptrTag->SetOffsetPos(m_ptrAttachTag->CurrentTagInfo().m_vPos);
		ptrTag->SetOffsetRot(m_ptrAttachTag->CurrentTagInfo().m_qRot);
		pEffect->AttachToTag(ptrTag);
		m_vectorTag.push_back(ptrTag);
		pEffect->Start(time + m_vectorOrbit[i].fTimeBeforeStart, restart);
	}
	PostStart();
}

void COrbitEffect::Stop(FLOAT leftTime)
{
	CEffect::Stop(leftTime);
}

//주의 : 현재 orbit effect는 일단 create된 effect가 create된 순서대로 process된다고 가정하고 있다.
//추후 수정에 의해 이 가정이 깨지게 되면 정상동작을 보장할 수 없게 된다.
BOOL COrbitEffect::Process(FLOAT time)
{
	if(time - m_fLastProcessTime > 0.1f)
	{
		INDEX cnt = INDEX(ceilf((time - m_fLastProcessTime) * 20.0f));
		if(!this->Process(m_fLastProcessTime+0.05f)) return FALSE;
		for(FLOAT t=m_fLastProcessTime+0.10f; t<=time; t+=0.05f)
		{
			if(!this->Process(t)) break;
		}
		return TRUE;
	}
	FLOAT fDeltaTime, fProcessedTime;
	BOOL bRet, bRender;
	if(!PreProcess(time, bRet, bRender, fDeltaTime, fProcessedTime))
	{
		ClearInstance();
		if(!bRender) SetNotRenderAtThisFrame();
		return bRet;
	}
	FLOAT fadeValue = 0;
	FLOAT rX=1, rY=1, rZ=1, pX=1, pY=1, pZ=1, pOffset=1;
	//fade in, [0, fade-in time)
	if(fProcessedTime <= m_fFadeInTime)
	{
		fadeValue = (fProcessedTime / m_fFadeInTime);
		FLOAT fadeValueRadius = (m_fFadeInDestRadiusMul - 1) * (fadeValue - 1) * (fadeValue - 1) + 1;
		FLOAT fadeValuePhase = (1 - m_fFadeInDestPhaseMul) * (fadeValue - 1) + 1;
		if(m_ulFadeFlag & (1<<FADEIN_ORBIT_RADIUSX)) rX = fadeValueRadius;
		if(m_ulFadeFlag & (1<<FADEIN_ORBIT_RADIUSY)) rY = fadeValueRadius;
		if(m_ulFadeFlag & (1<<FADEIN_ORBIT_RADIUSZ)) rZ = fadeValueRadius;
		if(m_ulFadeFlag & (1<<FADEIN_ORBIT_PHASEMULX)) pX = fadeValuePhase;
		if(m_ulFadeFlag & (1<<FADEIN_ORBIT_PHASEMULY)) pY = fadeValuePhase;
		if(m_ulFadeFlag & (1<<FADEIN_ORBIT_PHASEMULZ)) pZ = fadeValuePhase;
		if(m_ulFadeFlag & (1<<FADEIN_ORBIT_OFFSET)) pOffset = fadeValue;
	}
	//fade out, [fade-in time, life time - fade-out time)
	else if(m_fLeftTime <= m_fFadeOutTime)
	{
		fadeValue = (m_fLeftTime - m_fFadeOutTime * 0.5f) / (m_fFadeOutTime * 0.5f);
		fadeValue = ClampDn(fadeValue, 0.01f);
		FLOAT fadeValueRadius = (m_fFadeOutDestRadiusMul - 1) * (fadeValue - 1) * (fadeValue - 1) + 1;
		FLOAT fadeValuePhase = (1 - m_fFadeOutDestPhaseMul) * (fadeValue - 1) + 1;
		if(m_ulFadeFlag & (1<<FADEOUT_ORBIT_RADIUSX)) rX = fadeValueRadius;
		if(m_ulFadeFlag & (1<<FADEOUT_ORBIT_RADIUSY)) rY = fadeValueRadius;
		if(m_ulFadeFlag & (1<<FADEOUT_ORBIT_RADIUSZ)) rZ = fadeValueRadius;
		if(m_ulFadeFlag & (1<<FADEOUT_ORBIT_PHASEMULX)) pX = fadeValuePhase;
		if(m_ulFadeFlag & (1<<FADEOUT_ORBIT_PHASEMULY)) pY = fadeValuePhase;
		if(m_ulFadeFlag & (1<<FADEOUT_ORBIT_PHASEMULZ)) pZ = fadeValuePhase;
		if(m_ulFadeFlag & (1<<FADEOUT_ORBIT_OFFSET)) pOffset = fadeValue;
	}

	//pos
	FLOAT ratio = ClampUp((fProcessedTime - m_fFadeInTime) / (m_fLifeTime - m_fFadeInTime - m_fFadeOutTime * 0.5f), 1.0f);
	if(m_ePosition == EOTT_ALWAYS || m_bFirstProcess)
	{
		if(m_ptrGoalTag.NotNull() && fProcessedTime > m_fFadeInTime)
		{
			m_vTagPos = Lerp(m_ptrAttachTag->CurrentTagInfo().m_vPos
				, m_ptrGoalTag->CurrentTagInfo().m_vPos
				, ratio);
		}
		else m_vTagPos = m_ptrAttachTag->CurrentTagInfo().m_vPos;
	}
	//rot
	if(m_eRotation == EOTT_ALWAYS || (m_bFirstProcess && m_eRotation == EOTT_START))
	{
		if(m_ptrGoalTag.NotNull() && fProcessedTime > m_fFadeInTime)
		{
			m_qTagRot = Slerp(
				ratio
				, m_ptrAttachTag->CurrentTagInfo().m_qRot
				, m_ptrGoalTag->CurrentTagInfo().m_qRot
				);
		}
		else m_qTagRot = m_ptrAttachTag->CurrentTagInfo().m_qRot;
	}

	INDEX cnt = m_vectorTag.size();
	std::vector<INDEX> vecDel;
	for(INDEX i=0; i<cnt; ++i)
	{
		tag_ptr ptrTag = m_vectorTag[i];
		static TagInfo tagInfo(0, FLOAT3D(0,0,0), FLOATquat3D(1,0,0,0));
		tagInfo.m_fTime = time;
		sOrbitInfo &orbit = m_vectorOrbit[i];
		tagInfo.m_vPos(1) = rX * orbit.fRadiusX
			* sinf(orbit.fPhaseMulX * pX * fProcessedTime + 2*PI * orbit.fPhaseOffsetX)
			+ orbit.fOffsetX * pOffset;
		tagInfo.m_vPos(2) = rY * orbit.fRadiusY
			* sinf(orbit.fPhaseMulY * pY * fProcessedTime + 2*PI * orbit.fPhaseOffsetY)
			+ orbit.fOffsetY * pOffset;
		tagInfo.m_vPos(3) = rZ * orbit.fRadiusZ
			* sinf(orbit.fPhaseMulZ * pZ * fProcessedTime + 2*PI * orbit.fPhaseOffsetZ)
			+ orbit.fOffsetZ * pOffset;
		tagInfo.m_vPos = VectorRotate(tagInfo.m_vPos, m_qTagRot);
		tagInfo.m_vPos += m_vTagPos;
		((CWorldTag*)ptrTag.GetNative())->AddNewTagInfo(tagInfo);

		if(!m_vectorEffect[i]->Process(time)) vecDel.push_back(i);
	}

	m_bFirstProcess = FALSE;
	PostProcess();
	return TRUE;
}

CEffect *COrbitEffect::Copy()
{
	COrbitEffect *pRet = new COrbitEffect;
	if(pRet == NULL) return NULL;
	//CEffect의 content
	pRet->SetContent(this);
	//COrbitEffect의 content
	INDEX cnt = m_vectorOrbit.size();
	for(INDEX i=0; i<cnt; ++i)
	{
		pRet->AddOrbit( m_vectorOrbit[i] );
	}
	pRet->m_ulFadeFlag = m_ulFadeFlag;
	pRet->m_fFadeInDestRadiusMul = m_fFadeInDestRadiusMul;
	pRet->m_fFadeInDestPhaseMul = m_fFadeInDestPhaseMul;
	pRet->m_fFadeOutDestRadiusMul = m_fFadeOutDestRadiusMul;
	pRet->m_fFadeOutDestPhaseMul = m_fFadeOutDestPhaseMul;
	pRet->m_strGoalTagName = m_strGoalTagName;
	pRet->m_ePosition = m_ePosition;
	pRet->m_eRotation = m_eRotation;
	return pRet;
}

INDEX COrbitEffect::AddOrbit(const sOrbitInfo &info)
{
	m_vectorOrbit.push_back(info);
	return m_vectorOrbit.size() - 1;
}

BOOL COrbitEffect::DeleteOrbit(INDEX index)
{
	if(GetOrbit(index) == NULL) return FALSE;
	m_vectorOrbit.erase( m_vectorOrbit.begin() + index );
	return TRUE;
}

COrbitEffect::sOrbitInfo *COrbitEffect::GetOrbit(INDEX index)
{
	if(index < 0 || index >= m_vectorOrbit.size()) return NULL;
	return &m_vectorOrbit[index];
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 6

void COrbitEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CEffect::Read(pIS);

	is.ExpectID_t("EFOB");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		INDEX cnt = 0;
		is >> cnt;
		for(INDEX i=0; i<cnt; ++i)
		{
			sOrbitInfo orbit;
			is >> orbit.fRadiusX;
			is >> orbit.fPhaseMulX;
			is >> orbit.fPhaseOffsetX;
			is >> orbit.fOffsetX;
			is >> orbit.fRadiusY;
			is >> orbit.fPhaseMulY;
			is >> orbit.fPhaseOffsetY;
			is >> orbit.fOffsetY;
			is >> orbit.fRadiusZ;
			is >> orbit.fPhaseMulZ;
			is >> orbit.fPhaseOffsetZ;
			is >> orbit.fOffsetZ;
			is >> orbit.fTimeBeforeStart;
			CTString strTemp;
			is.GetLine_t(strTemp);
			orbit.strEffectName = strTemp.str_String;
			AddOrbit(orbit);
		}
		is >> m_ulFadeFlag;
		is >> m_fFadeInDestRadiusMul;
		is >> m_fFadeInDestPhaseMul;
		is >> m_fFadeOutDestRadiusMul;
		is >> m_fFadeOutDestPhaseMul;
		CTString strTemp;
		is >> strTemp;
		m_strGoalTagName = strTemp.str_String;
		is >> (INDEX&)m_eRotation;
		is >> (INDEX&)m_ePosition;
	}
	//old version
	else if(ubVer == 5)
	{
		INDEX cnt = 0;
		is >> cnt;
		for(INDEX i=0; i<cnt; ++i)
		{
			sOrbitInfo orbit;
			is >> orbit.fRadiusX;
			is >> orbit.fPhaseMulX;
			is >> orbit.fPhaseOffsetX;
			is >> orbit.fOffsetX;
			is >> orbit.fRadiusY;
			is >> orbit.fPhaseMulY;
			is >> orbit.fPhaseOffsetY;
			is >> orbit.fOffsetY;
			is >> orbit.fRadiusZ;
			is >> orbit.fPhaseMulZ;
			is >> orbit.fPhaseOffsetZ;
			is >> orbit.fOffsetZ;
			is >> orbit.fTimeBeforeStart;
			CTString strTemp;
			is.GetLine_t(strTemp);
			orbit.strEffectName = strTemp.str_String;
			AddOrbit(orbit);
		}
		ULONG ulFadeInFlag, ulFadeOutFlag;
		is >> ulFadeInFlag;
		is >> ulFadeOutFlag;
		m_ulFadeFlag = (FADEIN_MASK & ulFadeInFlag) | (FADEOUT_MASK & ulFadeOutFlag);
		is >> m_fFadeInDestRadiusMul;
		is >> m_fFadeInDestPhaseMul;
		is >> m_fFadeOutDestRadiusMul;
		is >> m_fFadeOutDestPhaseMul;
		CTString strTemp;
		is >> strTemp;
		m_strGoalTagName = strTemp.str_String;
		is >> (INDEX&)m_eRotation;
		is >> (INDEX&)m_ePosition;
	}
/*
	else if(ubVer == 4)
	{
		INDEX cnt = 0;
		is >> cnt;
		for(INDEX i=0; i<cnt; ++i)
		{
			sOrbitInfo orbit;
			is >> orbit.fRadiusX;
			is >> orbit.fPhaseMulX;
			is >> orbit.fPhaseOffsetX;
			orbit.fOffsetX = 0;
			is >> orbit.fRadiusY;
			is >> orbit.fPhaseMulY;
			is >> orbit.fPhaseOffsetY;
			orbit.fOffsetY = 0;
			is >> orbit.fRadiusZ;
			is >> orbit.fPhaseMulZ;
			is >> orbit.fPhaseOffsetZ;
			orbit.fOffsetZ = 0;
			is >> orbit.fTimeBeforeStart;
			CTString strTemp;
			is.GetLine_t(strTemp);
			orbit.strEffectName = strTemp.str_String;
			AddOrbit(orbit);
		}
		ULONG ulFadeInFlag, ulFadeOutFlag;
		is >> ulFadeInFlag;
		is >> ulFadeOutFlag;
		m_ulFadeFlag = (FADEIN_MASK & ulFadeInFlag) | (FADEOUT_MASK & ulFadeOutFlag);
		is >> m_fFadeInDestRadiusMul;
		is >> m_fFadeInDestPhaseMul;
		is >> m_fFadeOutDestRadiusMul;
		is >> m_fFadeOutDestPhaseMul;
		CTString strTemp;
		is >> strTemp;
		m_strGoalTagName = strTemp.str_String;
		m_eRotation = EOTT_START;
		m_ePosition = EOTT_ALWAYS;
	}
	else if(ubVer == 2 || ubVer == 3)
	{
		INDEX cnt = 0;
		is >> cnt;
		for(INDEX i=0; i<cnt; ++i)
		{
			sOrbitInfo orbit;
			is >> orbit.fRadiusX;
			is >> orbit.fPhaseMulX;
			is >> orbit.fPhaseOffsetX;
			is >> orbit.fRadiusY;
			is >> orbit.fPhaseMulY;
			is >> orbit.fPhaseOffsetY;
			is >> orbit.fRadiusZ;
			is >> orbit.fPhaseMulZ;
			is >> orbit.fPhaseOffsetZ;
			is >> orbit.fTimeBeforeStart;
			CTString strTemp;
			is.GetLine_t(strTemp);
			orbit.strEffectName = strTemp.str_String;
			AddOrbit(orbit);
		}
		is >> m_ulFadeInFlag;
		is >> m_ulFadeOutFlag;
		if(ubVer == 3)
		{
			is >> m_fFadeInDestRadiusMul;
			is >> m_fFadeInDestPhaseMul;
			is >> m_fFadeOutDestRadiusMul;
			is >> m_fFadeOutDestPhaseMul;
		}
	}
*/	//별문제 없으면 삭제해도 되는 코드임.
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void COrbitEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CEffect::Write(pOS);

	os.WriteID_t("EFOB");
	os << (UBYTE)CURRENT_VERSION;

	INDEX cnt = m_vectorOrbit.size();
	os << cnt;
	for(INDEX i=0; i<cnt; ++i)
	{
		sOrbitInfo &orbit = m_vectorOrbit[i];
		os << orbit.fRadiusX;
		os << orbit.fPhaseMulX;
		os << orbit.fPhaseOffsetX;
		os << orbit.fOffsetX;
		os << orbit.fRadiusY;
		os << orbit.fPhaseMulY;
		os << orbit.fPhaseOffsetY;
		os << orbit.fOffsetY;
		os << orbit.fRadiusZ;
		os << orbit.fPhaseMulZ;
		os << orbit.fPhaseOffsetZ;
		os << orbit.fOffsetZ;
		os << orbit.fTimeBeforeStart;
		os.PutLine_t(orbit.strEffectName.c_str());
	}
	os << m_ulFadeFlag;
	os << m_fFadeInDestRadiusMul;
	os << m_fFadeInDestPhaseMul;
	os << m_fFadeOutDestRadiusMul;
	os << m_fFadeOutDestPhaseMul;
	os << CTString(m_strGoalTagName.c_str());
	os << (INDEX)m_eRotation;
	os << (INDEX)m_ePosition;
}

#define WANT_TAG_COUNT 1
INDEX COrbitEffect::GetWantTagCount()
{
	return WANT_TAG_COUNT;
}

const char *COrbitEffect::GetWantTagName(INDEX index)
{
	return m_strGoalTagName.c_str();
}

void COrbitEffect::SetWantTag(INDEX index, ptr_tag tag)
{
	if(index < 0 || index >= WANT_TAG_COUNT) return;
	m_ptrGoalTag = tag;
}

