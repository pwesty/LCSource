//안태훈 수정 시작	//(Remake Effect)(0.1)
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include "StdH.h"

#include <Engine/Base/Stream.h>

#include "CEffectManager.h"

#include "EffectCommon.h"
#include "CTag.h"
#include "CGroupTag.h"
#include "CEffect.h"
#include "CEffectGroup.h"
#include "CWorldTag.h"
#include "CLightEffect.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CEffectGroup);

CEffectGroup::CEffectGroup()
: m_strName( "" )
, m_bActive( FALSE )
, m_pTagManager( NULL )
, m_iNextEffectKeyValue( 0 )
, m_iNextTagKeyValue( 0 )
, m_pProjection( NULL )
, m_fSpeedMul( 1 )
, m_pWantTagManager( NULL )
, m_gERType(ER_NORMAL)
, m_gERSubType(ERS_NORMAL)
{
}

CEffectGroup::~CEffectGroup()
{
	for(INDEX i=0; i<m_vectorEffectKey.size(); ++i)
	{
		//if(m_vectorEffectKey[i].m_pCreatedEffect && CEffectManager::Instance().IsValidCreated(m_vectorEffectKey[i].m_pCreatedEffect))
		if(m_vectorEffectKey[i].m_pCreatedEffect)
		{
			CEffectManager::Instance().Destroy(m_vectorEffectKey[i].m_pCreatedEffect);
		}
	}
	m_vectorEffectKey.clear();
	m_vectorTagKey.clear();
}

void CEffectGroup::GetEffectKeyInfo(INDEX index, FLOAT &fStartTimeFromStart, std::string &effectName)
{
	ASSERT(index >= 0 && index < m_vectorEffectKey.size());
	fStartTimeFromStart = m_vectorEffectKey[index].m_fStartTime;
	effectName = m_vectorEffectKey[index].m_strEffectName;
}

void CEffectGroup::GetTagKeyInfo(INDEX index, FLOAT &fSettingTimeFromStart, INDEX &iAddedEffectKeyValue, std::string &TagName)
{
	ASSERT(index >= 0 && index < m_vectorTagKey.size());
	fSettingTimeFromStart = m_vectorTagKey[index].m_fSettingTime;
	iAddedEffectKeyValue = m_vectorTagKey[index].m_iEffectKeyValue;
	TagName = m_vectorTagKey[index].m_strTagName;
}

INDEX CEffectGroup::AddEffect(FLOAT fStartTimeFromStart, std::string strEffectName)
{
	ASSERT(fStartTimeFromStart >= 0.0f);
	ASSERT(!strEffectName.empty());
	//ASSERT(ptrInitTag.NotNull());

	if(!CEffectManager::Instance().IsRegistered(strEffectName)) return -1;

	//EFFECT_KEY를 만들어서 벡터에 추가한다.
	INDEX index = m_vectorEffectKey.size();
	m_vectorEffectKey.push_back(EffectKey());
	EffectKey &effectKey = m_vectorEffectKey[index];
	//프로토타입의 이름과 그것의 시작 시간을 저장한다.
	//시작 시간은 이 skill effect가 시작한 시간으로 부터의 오프셋 시간임.
	effectKey.m_iKeyValue = m_iNextEffectKeyValue++;
	effectKey.m_fStartTime = fStartTimeFromStart;
	effectKey.m_strEffectName = strEffectName;
	effectKey.m_pCreatedEffect = NULL;

	return effectKey.m_iKeyValue;
}

INDEX CEffectGroup::AddTagForEffect(FLOAT fSettingTimeFromStart, INDEX iAddedEffect, std::string tagName)
{
	ASSERT(fSettingTimeFromStart >= 0.0f);

	EffectKey* effectkey = FindEffectKey(iAddedEffect);
	if(effectkey == NULL) return -1;

	//TAG_KEY를 생성한다.
	INDEX index = m_vectorTagKey.size();
	m_vectorTagKey.push_back(TagKey());
	TagKey &tagKey = m_vectorTagKey[index];

	//TAG_KEY의 정보를 채운다.
	tagKey.m_iKeyValue = m_iNextTagKeyValue++;
	tagKey.m_fSettingTime = fSettingTimeFromStart;
	tagKey.m_iEffectKeyValue = iAddedEffect;
	tagKey.m_strTagName = tagName;

	return tagKey.m_iKeyValue;
}

void CEffectGroup::AddTagForAllEffect(FLOAT fSettingTimeFromStart, std::string tagName)
{
	for(INDEX i=0; i<m_vectorEffectKey.size(); ++i)
	{
		INDEX index = m_vectorTagKey.size();
		//TAG_KEY를 생성한다.
		m_vectorTagKey.push_back(TagKey());
		TagKey &tagKey = m_vectorTagKey[index];
		//TAG_KEY의 정보를 채운다.
		tagKey.m_iKeyValue = m_iNextTagKeyValue++;
		tagKey.m_fSettingTime = fSettingTimeFromStart;
		tagKey.m_iEffectKeyValue = m_vectorEffectKey[i].m_iKeyValue;
		tagKey.m_strTagName = tagName;
	}
}

BOOL CEffectGroup::RemoveEffect(INDEX keyValue)
{
	//effect가 지워질때는 그에 해당하는 tag도 지움.
	INDEX cnt = m_vectorTagKey.size();
	for(INDEX i=0; i<cnt; ++i)
	{
		if(m_vectorTagKey[i].m_iEffectKeyValue == keyValue)
		{
			m_vectorTagKey.erase(m_vectorTagKey.begin() + i);
			--cnt;
			--i;
		}
	}
	//effect 지움.
	for(vector_effect_key::iterator iter=m_vectorEffectKey.begin(); iter!=m_vectorEffectKey.end(); ++iter)
	{
		if((*iter).m_iKeyValue == keyValue)
		{
			m_vectorEffectKey.erase(iter);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CEffectGroup::RemoveTag(INDEX keyValue)
{
	for(vector_tag_key::iterator iter=m_vectorTagKey.begin(); iter!=m_vectorTagKey.end(); ++iter)
	{
		if((*iter).m_iKeyValue == keyValue)
		{
			m_vectorTagKey.erase(iter);
			return TRUE;
		}
	}
	return FALSE;
}

CEffectGroup *CEffectGroup::Copy()
{
	CEffectGroup *pEffectGroup = new CEffectGroup;
	if(pEffectGroup == NULL) return NULL;
	pEffectGroup->m_strName = m_strName;
	pEffectGroup->m_gERType = m_gERType;
	pEffectGroup->m_gERSubType = m_gERSubType;
	INDEX i;
	for( i = 0; i < m_vectorEffectKey.size(); ++i )
	{
		EffectKey &effectKey = m_vectorEffectKey[i];
		pEffectGroup->AddEffect(effectKey.m_fStartTime, effectKey.m_strEffectName);
	}
	for( i = 0; i < m_vectorTagKey.size(); ++i )
	{
		TagKey &tagKey = m_vectorTagKey[i];
		pEffectGroup->AddTagForEffect(tagKey.m_fSettingTime, tagKey.m_iEffectKeyValue, tagKey.m_strTagName);
	}
	return pEffectGroup;
}

void CEffectGroup::Start(FLOAT time)
{
	if(m_bActive) return;
	if(m_pTagManager == NULL) return;	//tag manager가 없으면 의미 없음.
	if(m_vectorEffectKey.empty() || m_vectorTagKey.empty()) return;

	//effect를 생성한다.
	for(INDEX i=0; i<m_vectorEffectKey.size(); ++i)
	{
		CEffect *pEffect = CEffectManager::Instance().Create(m_vectorEffectKey[i].m_strEffectName);
		//if(pEffect == NULL) ASSERTALWAYS("이펙트 이름으로부터 이펙트의 생성에 실패했습니다.");
		if(pEffect == NULL) return;
		m_vectorEffectKey[i].m_pCreatedEffect = pEffect;

		pEffect->SetERType(m_gERType);
		pEffect->SetERSubType(m_gERSubType);

		if(m_pWantTagManager != NULL)
		{
			for(INDEX iwt=0; iwt<pEffect->GetWantTagCount(); ++iwt)
			{
				ptr_tag tag( m_pWantTagManager->Find( pEffect->GetWantTagName(iwt) ) );
				if(tag.IsNull()) tag = m_pTagManager->Find(pEffect->GetWantTagName(iwt));
				pEffect->SetWantTag(iwt, tag);
			}
		}
		else
		{
			for(INDEX iwt=0; iwt<pEffect->GetWantTagCount(); ++iwt)
			{
				ptr_tag tag( m_pTagManager->Find( pEffect->GetWantTagName(iwt) ) );
				pEffect->SetWantTag(iwt, tag);
			}
		}


		if(m_pTagManager)
		{
			CEntity* pEntity = m_pTagManager->GetOwner();
			if (pEntity != NULL)
				pEffect->SetOwner(pEntity);
		}
	}
	//모든 Tag를 찾는다.
	if(m_pTagManager != NULL)
	{
		for(INDEX i = 0; i < m_vectorTagKey.size(); ++i)
		{
			m_vectorTagKey[i].m_ptrReserveTag = NULL;
			CTag *pTag = m_pTagManager->Find(m_vectorTagKey[i].m_strTagName, TRUE);
			m_vectorTagKey[i].m_ptrReserveTag = pTag;
		}
	}

	m_bDirectTag = FALSE;
	m_fStartTime = time;
	m_bActive = TRUE;
}

//안태훈 수정 시작	//(Remake Effect)(0.1)
void CEffectGroup::Start(FLOAT time, const ptr_tag &ptrTag)
{
	if(m_bActive) return;
	if(m_vectorEffectKey.empty() || m_vectorTagKey.empty()) return;

	m_bDirectTag = TRUE;
	m_ptrDirectTag = ptrTag;

	//effect를 생성한다. 생성된 이펙트를 임시 Tag에 붙인다.
	for(INDEX i=0; i<m_vectorEffectKey.size(); ++i)
	{
		CEffect *pEffect = CEffectManager::Instance().Create(m_vectorEffectKey[i].m_strEffectName);
		//if(pEffect == NULL) ASSERTALWAYS("이펙트 이름으로부터 이펙트의 생성에 실패했습니다.");
		if(pEffect == NULL) return;
		if(m_pTagManager) pEffect->SetOwner(m_pTagManager->GetOwner());
		m_vectorEffectKey[i].m_pCreatedEffect = pEffect;
		pEffect->SetERType(m_gERType);
		pEffect->SetERSubType(m_gERSubType);
		pEffect->AttachToTag(ptrTag);
		pEffect->SetSpeedMul(m_fSpeedMul);
		
		if(m_pWantTagManager != NULL)
		{
			for(INDEX iwt=0; iwt<pEffect->GetWantTagCount(); ++iwt)
			{
				ptr_tag tag( m_pWantTagManager->Find( pEffect->GetWantTagName(iwt) ) );
				if(tag.IsNull()) tag = m_pTagManager->Find(pEffect->GetWantTagName(iwt));
				pEffect->SetWantTag(iwt, tag);
			}
		}
/*
		else
		{
			for(INDEX iwt=0; iwt<pEffect->GetWantTagCount(); ++iwt)
			{
				ptr_tag tag( m_pTagManager->Find( pEffect->GetWantTagName(iwt) ) );
				pEffect->SetWantTag(iwt, tag);
			}
		}
*/
	}
	m_fStartTime = time;
	m_fLastProcessedTime = time;
	m_bActive = TRUE;
}

void CEffectGroup::Stop(FLOAT leftTime)
{
	//CPrintF("Stop Effect: %s\n", m_strName.c_str());
	for(INDEX i=0; i<m_vectorEffectKey.size(); ++i)
	{
		if(m_vectorEffectKey[i].m_pCreatedEffect)
		{
			m_vectorEffectKey[i].m_pCreatedEffect->SetRepeatCount(0);
			if(m_vectorEffectKey[i].m_pCreatedEffect->Playing())
			{
				if(leftTime < 0) m_vectorEffectKey[i].m_pCreatedEffect->Stop( m_vectorEffectKey[i].m_pCreatedEffect->GetFadeOutTime() );
				else m_vectorEffectKey[i].m_pCreatedEffect->Stop(leftTime);
			}
			else
			{
				EffectKey &ek = m_vectorEffectKey[i];
				if(ek.m_fStartTime >= leftTime)
					ek.m_pCreatedEffect->SetLifeTime(0);
				else if(ek.m_fStartTime + ek.m_pCreatedEffect->GetLifeTime() > leftTime)
					ek.m_pCreatedEffect->SetLifeTime(leftTime - ek.m_fStartTime);
				else ;//NOTHING
			}
		}
	}
}

BOOL CEffectGroup::Process(FLOAT time)
{
	if(!m_bActive) return FALSE;

	// 1/50의 정확도를 갖는다.
	//if( time - m_fLastProcessedTime < 0.02f ) return TRUE;

	FLOAT fProcessedTime = time - m_fStartTime;
	m_fLastProcessedTime = time;

	if(!m_bDirectTag)
	{
		//tag change 처리
		for(INDEX i=0; i<m_vectorTagKey.size(); ++i)
		{
			if(m_vectorTagKey[i].m_fSettingTime * m_fSpeedMul > fProcessedTime) continue;

			//tag를 붙임. 매프레임마다 처리됨. (계속 붙음)
			EffectKey* effectkey = FindEffectKey(m_vectorTagKey[i].m_iEffectKeyValue);
			
			if (effectkey && effectkey->m_pCreatedEffect)
			{
				effectkey->m_pCreatedEffect->AttachToTag(m_vectorTagKey[i].m_ptrReserveTag);
			}
		}
	}
	//effect 처리
	CAnyProjection3D &apr = *m_pProjection;
	BOOL bTestCulling = m_pProjection != NULL && apr.ap_CurrentProjection != NULL;
	BOOL ret = FALSE;
	for(INDEX i=0; i<m_vectorEffectKey.size(); ++i)
	{
		EffectKey &effectKey = m_vectorEffectKey[ i ];

		//시작 시간을 지났으므로 시작한다. 시간은 정해진 시작시간으로 세팅한다.
		if(effectKey.m_pCreatedEffect->GetState() == ES_NOT_STARTED
			&& time >= effectKey.m_fStartTime * m_fSpeedMul + m_fStartTime)
		{
			effectKey.m_pCreatedEffect->Start( m_fStartTime + effectKey.m_fStartTime * m_fSpeedMul );
			if(effectKey.m_pCreatedEffect->GetTag().NotNull()
			&& !effectKey.m_pCreatedEffect->GetTag()->Active()
			&& effectKey.m_pCreatedEffect->Playing())
				effectKey.m_pCreatedEffect->SetNotRenderAtThisFrame();
		}

		//플레이 상태가 아니면 다음으로 넘어감
		if(effectKey.m_pCreatedEffect->Playing() == FALSE)
		{
			if(effectKey.m_pCreatedEffect->GetState() == ES_NOT_STARTED)
			{
				ret |= TRUE;
				continue;
			}
			else if(effectKey.m_pCreatedEffect->GetState() == ES_PLAY_END
				&& effectKey.m_pCreatedEffect->GetRepeatCount() != 0)
			{
				effectKey.m_pCreatedEffect->Start(time);

				if(effectKey.m_pCreatedEffect->Playing() == FALSE)
				{
					if(effectKey.m_pCreatedEffect->GetState() == ES_NOT_STARTED) ret |= TRUE;
					continue;
				}
			}
			else continue;
		}

		if(bTestCulling
			&& effectKey.m_pCreatedEffect->GetType() != ET_SOUND
			&& effectKey.m_pCreatedEffect->GetType() != ET_CAMERA)
		{
			BOOL bNoPassTest = FALSE;

			//거리 테스트, 먼저 한다.
			CTag *pTagTest = NULL;
			if(effectKey.m_pCreatedEffect->GetTag()->GetType() != TT_GROUP)
			{
				pTagTest = effectKey.m_pCreatedEffect->GetTag().GetNative();
			}
			else
			{
				pTagTest = ((CGroupTag*)effectKey.m_pCreatedEffect->GetTag().GetNative())->GetTag(0).GetNative();
			}
			bNoPassTest = (g_fEffectDistance < fabs(apr->GetDistance( pTagTest->CurrentTagInfo().m_vPos )));
			if(!bNoPassTest)
			{
				//Frustum 테스트
				FLOAT fPerspectiveFactor = 1.0f;
				if( apr.IsPerspective())
					fPerspectiveFactor = ((CPerspectiveProjection3D*)&*apr)->ppr_PerspectiveRatios(1);
				// project point to screen
				FLOAT3D vProjected;
				apr->PreClip( pTagTest->CurrentTagInfo().m_vPos, vProjected);
				// skip if too small
				const FLOAT fPixSize = effectKey.m_pCreatedEffect->GetBoundingSphereRadius() * fPerspectiveFactor / vProjected(3);
				if( fPixSize < 0.5f )
					bNoPassTest = TRUE;
				// skip if not in screen
				const INDEX iTested = apr->TestSphereToFrustum( vProjected, effectKey.m_pCreatedEffect->GetBoundingSphereRadius() );
				if( iTested < 0 )
					bNoPassTest = TRUE;
			}

			if(bNoPassTest)
			{
				effectKey.m_pCreatedEffect->SetNotRenderAtThisFrame();
				if(effectKey.m_pCreatedEffect->GetType() == ET_SPLINEBILLBOARD
				|| effectKey.m_pCreatedEffect->GetType() == ET_ORBIT)
				{
					effectKey.m_pCreatedEffect->Stop(0);
				}
			}
		}
		if(!effectKey.m_pCreatedEffect->IsNotRenderAtThisFrame())
		{
			//Processing, 내부적으로 상태가 변할 수 있음.
			BOOL processRet = effectKey.m_pCreatedEffect->Process(time);
			ret = ret || processRet;
		}
		else ret = TRUE;
	}
	return ret;
}
//안태훈 수정 끝	//(Remake Effect)(0.1)

CEffectGroup::EffectKey* CEffectGroup::FindEffectKey(INDEX keyValue)
{
	vector_effect_key::iterator FindItr = std::find_if(m_vectorEffectKey.begin(), m_vectorEffectKey.end(), FindKeyValue<EffectKey>(keyValue));
	if (FindItr != m_vectorEffectKey.end())
	{
		return &(*FindItr);
	}

	return NULL;
}
// 사용하지 않는다.
CEffectGroup::TagKey* CEffectGroup::FindTagKey(INDEX keyValue)
{
	return NULL;
}

#define CURRENT_VERSION 1

void CEffectGroup::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("EFGR");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		CTString strTemp;
		is.GetLine_t(strTemp);
		SetName(strTemp.str_String);

		//effect key를 read
		is.ExpectID_t("EGEK");
		DWORD count;
		is >> count;
		for(int iek=0; iek<count; ++iek)
		{
			FLOAT fStartTime;
			is >> fStartTime;
			is.GetLine_t(strTemp);

			AddEffect(fStartTime, strTemp.str_String);
		}
		//tag key를 read
		is.ExpectID_t("EGTK");
		is >> count;
		for(int itk=0; itk<count; ++itk)
		{
			FLOAT fSettingTime;
			INDEX iEffectKey;
			is >> fSettingTime;
			is >> iEffectKey;
			is.GetLine_t(strTemp);

			AddTagForEffect(fSettingTime, iEffectKey, strTemp.str_String);
		}
		is.ExpectID_t("EGEN");
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CEffectGroup::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("EFGR");
	os << (UBYTE)CURRENT_VERSION;

	os.PutLine_t(m_strName.c_str());

	//effect key를 write
	os.WriteID_t("EGEK");
	os << (DWORD)m_vectorEffectKey.size();
	for(int iek=0; iek<m_vectorEffectKey.size(); ++iek)
	{
		os << m_vectorEffectKey[iek].m_fStartTime;
		os.PutLine_t(m_vectorEffectKey[iek].m_strEffectName.c_str());
	}
	//tag key를 write
	os.WriteID_t("EGTK");
	os << (DWORD)m_vectorTagKey.size();
	for(int itk=0; itk<m_vectorTagKey.size(); ++itk)
	{
		os << m_vectorTagKey[itk].m_fSettingTime;
		os << m_vectorTagKey[itk].m_iEffectKeyValue;
		os.PutLine_t(m_vectorTagKey[itk].m_strTagName.c_str());
	}
	os.WriteID_t("EGEN");
}

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
//안태훈 수정 끝	//(Remake Effect)(0.1)
