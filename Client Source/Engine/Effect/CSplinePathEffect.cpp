#include "StdH.h"
#include "CSplinePathEffect.h"

#include "CWorldTag.h"
#include "CEffectManager.h"
#include <Engine/Math/AdditionalFunction.h>

//NEW_DELETE_IMPLEMENT(CSplinePathEffect);

CSplinePathEffect::CSplinePathEffect()
: m_ePosition( EOTT_START )
, m_eRotation( EOTT_NONE )
, m_qTagRot( 1,0,0,0 )
{
	CEffect::m_eType = ET_SPLINEPATH;
}

CSplinePathEffect::~CSplinePathEffect()
{
	ClearInstance();
}

void CSplinePathEffect::SetNotRenderAtThisFrame()
{
	for(INDEX i=0; i<m_vectorEffect.size(); ++i)
	{
		m_vectorEffect[i]->SetNotRenderAtThisFrame();
	}
}

void CSplinePathEffect::ClearInstance()
{
	INDEX cnt = m_vectorEffect.size();
	for(INDEX i=0; i<cnt; ++i)
	{
		CEffectManager::Instance().Destroy(m_vectorEffect[i]);
	}
	m_vectorEffect.clear();
	m_vectorTag.clear();
	m_vTagPos = FLOAT3D(0,0,0);
	m_qTagRot = FLOATquat3D(1,0,0,0);
}

void CSplinePathEffect::Start(FLOAT time, BOOL restart)
{
	if(!PreStart(time, restart)) return;
	if(m_eState != ES_NOT_STARTED) return;
	if(restart)
	{
		PostStart();
		return;
	}

	ClearInstance();

	m_vTagPos = m_ptrAttachTag->CurrentTagInfo().m_vPos;
	if(m_eRotation != EOTT_NONE) m_qTagRot = m_ptrAttachTag->CurrentTagInfo().m_qRot;

	INDEX cnt = m_vectorSplinePath.size();
	m_vectorEffect.reserve(cnt);
	for(INDEX i=0; i<cnt; ++i)
	{
		if(m_vectorSplinePath[i].strEffectName == m_strName) continue;
		CEffect *pEffect = 
			CEffectManager::Instance().Create(m_vectorSplinePath[i].strEffectName);
		if(pEffect == NULL) continue;

		pEffect->SetERType(m_ERType);
		pEffect->SetERSubType(m_ERSubType);
		m_vectorEffect.push_back( pEffect );
		tag_ptr ptrTag(new CWorldTag);
		ptrTag->SetOffsetPos(m_vTagPos + m_vectorSplinePath[i].ssPath.GetValue(0));
		//if(m_vectorSplinePath[i].eSplinePathDirType == SPDT_NONE) ptrTag->SetOffsetRot(m_qTagRot);
		//else ptrTag->SetOffsetRot(m_qTagRot + m_vectorSplinePath[i].ssDir.GetValue(0));
		pEffect->AttachToTag(ptrTag);
		m_vectorTag.push_back(ptrTag);
		pEffect->Start(time, restart);
	}
	PostStart();
}

void CSplinePathEffect::Stop(FLOAT leftTime)
{
	CEffect::Stop(leftTime);
	ClearInstance();
}

//주의 : 현재 CSplinePathEffect는 일단 create된 effect가 create된 순서대로 process된다고 가정하고 있다.
//추후 수정에 의해 이 가정이 깨지게 되면 정상동작을 보장할 수 없게 된다.
BOOL CSplinePathEffect::Process(FLOAT time)
{
	FLOAT fDeltaTime, fProcessedTime;
	BOOL bRet, bRender;
	if(!PreProcess(time, bRet, bRender, fDeltaTime, fProcessedTime))
	{
		ClearInstance();
		if(!bRender) SetNotRenderAtThisFrame();
		return bRet;
	}
	if(m_ePosition == EOTT_ALWAYS) m_vTagPos = m_ptrAttachTag->CurrentTagInfo().m_vPos;
	if(m_eRotation == EOTT_ALWAYS) m_qTagRot = m_ptrAttachTag->CurrentTagInfo().m_qRot;

	INDEX cnt = m_vectorTag.size();
	std::vector<INDEX> vecDel;
	for(INDEX i=0; i<cnt; ++i)
	{
		tag_ptr ptrTag = m_vectorTag[i];
		TagInfo tagInfo(time, m_vTagPos, m_qTagRot);
		
		sSplinePathInfo &spath = m_vectorSplinePath[i];
		FLOAT3D pos = spath.ssPath.Value(fProcessedTime);
		pos += m_vOffsetPos;
		RotateVector(pos, m_qTagRot);
		tagInfo.m_vPos += pos;
		//tagInfo.m_qRot += spath.ssDir.Value(fProcessedTime);
		((CWorldTag*)ptrTag.GetNative())->AddNewTagInfo(tagInfo);

		if(!m_vectorEffect[i]->Process(time)) vecDel.push_back(i);
	}
/*
	for(i=0; i<vecDel.size(); ++i)
	{
		CEffectManager::Instance().Destroy(m_vectorEffect[vecDel[i] - i]);
		m_vectorEffect.erase( m_vectorEffect.begin() + vecDel[i] - i );
	}
*/

	PostProcess();
	return TRUE;
}

CEffect *CSplinePathEffect::Copy()
{
	CSplinePathEffect *pRet = new CSplinePathEffect;
	if(pRet == NULL) return NULL;
	//CEffect의 content
	pRet->SetContent(this);
	//COrbitEffect의 content
	INDEX cnt = m_vectorSplinePath.size();
	for(INDEX i=0; i<cnt; ++i)
	{
		pRet->AddSplinePath( m_vectorSplinePath[i] );
	}
	pRet->m_ePosition = m_ePosition;
	pRet->m_eRotation = m_eRotation;
	pRet->m_vOffsetPos = m_vOffsetPos;
	return pRet;
}

INDEX CSplinePathEffect::AddSplinePath(const sSplinePathInfo &info)
{
	m_vectorSplinePath.push_back(info);
	return m_vectorSplinePath.size() - 1;
}

BOOL CSplinePathEffect::DeleteSplinePath(INDEX index)
{
	if(GetSplinePath(index) == NULL) return FALSE;
	m_vectorSplinePath.erase( m_vectorSplinePath.begin() + index );
	return TRUE;
}

CSplinePathEffect::sSplinePathInfo *CSplinePathEffect::GetSplinePath(INDEX index)
{
	if(index < 0 || index >= m_vectorSplinePath.size()) return NULL;
	return &m_vectorSplinePath[index];
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 2

void CSplinePathEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CEffect::Read(pIS);

	is.ExpectID_t("EFSP");
	UBYTE ubVer;
	is >> ubVer;

	is >> (INDEX&)m_eRotation;
	is >> (INDEX&)m_ePosition;

	if(ubVer == CURRENT_VERSION)
	{
		INDEX cnt = 0;
		is >> cnt;
		for(INDEX i=0; i<cnt; ++i)
		{
			sSplinePathInfo spath;
			ssReadFromStream(is, spath.ssPath, FLOAT3D);
			spath.ssPath.Prepare();
			CTString strTemp;
			is >> strTemp;
			spath.strEffectName = strTemp.str_String;
			m_vectorSplinePath.push_back(spath);
		}
		is >> m_vOffsetPos;
	}
	else if(ubVer == 1)
	{
		INDEX cnt = 0;
		is >> cnt;
		for(INDEX i=0; i<cnt; ++i)
		{
			sSplinePathInfo spath;
			ssReadFromStream(is, spath.ssPath, FLOAT3D);
			spath.ssPath.Prepare();
			CTString strTemp;
			is >> strTemp;
			spath.strEffectName = strTemp.str_String;
			m_vectorSplinePath.push_back(spath);
		}
		m_vOffsetPos = FLOAT3D(0,0,0);
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CSplinePathEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CEffect::Write(pOS);

	os.WriteID_t("EFSP");
	os << (UBYTE)CURRENT_VERSION;

	os << (INDEX)m_eRotation;
	os << (INDEX)m_ePosition;

	INDEX cnt = m_vectorSplinePath.size();
	os << cnt;
	for(INDEX i=0; i<cnt; ++i)
	{
		sSplinePathInfo &spath = m_vectorSplinePath[i];
		ssWriteToStream(os, spath.ssPath);
		os << CTString(spath.strEffectName.c_str());
	}
	os << m_vOffsetPos;
}
