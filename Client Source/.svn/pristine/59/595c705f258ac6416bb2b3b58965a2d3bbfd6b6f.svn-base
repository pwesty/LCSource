//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include "StdH.h"

#include "CTag.h"
#include "CEffect.h"
#include <Engine/Base/Memory.h>

CEffect::CEffect()
: m_eType( ET_NOTHING )
, m_fLastProcessTime( 0 )
, m_fLifeTime( 0 )
, m_fLeftTime( 0 )
, m_fStartTime( 0 )
, m_eState( ES_NOT_STARTED )
, m_slRepeat( 0 )
, m_bNotRenderAtThisFrame( FALSE )
, m_fFadeInTime( 0 )
, m_fFadeOutTime( 0 )
, m_fBoundingSphereRadius( 1 )
, m_fSpeedMul( 1 )
, m_penOwner( NULL )
, m_ERType(ER_NORMAL)
, m_ERSubType(ERS_NORMAL)
{
}

CEffect::~CEffect()
{
}

void CEffect::SetNotRenderAtThisFrame()
{
	m_bNotRenderAtThisFrame = TRUE;
}


BOOL CEffect::PreStart(FLOAT time, BOOL bRestart)
{
	if(bRestart)
	{
		m_eState = ES_NOT_STARTED;
		if(m_slRepeat > 0) --m_slRepeat;
		m_ptrAttachTag->Deactivate();
	}
	if( m_ptrAttachTag.IsNull() || m_eState != ES_NOT_STARTED )
	{
		m_eState = ES_PLAY_END;
		return FALSE;
	}

	m_ptrAttachTag->Activate();

	m_fStartTime = time;
	m_fLastProcessTime = time;
	m_fLeftTime = m_fLifeTime;

	return TRUE;
}

BOOL CEffect::PreProcess(FLOAT time, BOOL &bRetValue, BOOL &bRenderInThisFrame
						 , FLOAT &retDeltaTime, FLOAT &retProcessedTime)
{
	bRenderInThisFrame = FALSE;
	bRetValue = FALSE;
	if(m_eState == ES_PLAY_END) return FALSE;
	//활성화된 상태가 아니거나 Tag에 붙어있지 않다면
	//원칙적으로 이 루틴을 타면 안됨, 함수외부에서 처리.
	if(!Playing() || m_ptrAttachTag.IsNull()) return FALSE;
	if(!m_ptrAttachTag->Active()) return FALSE;
	bRenderInThisFrame = TRUE;

	//시간 경과 및 그에 따른 애니메이션, 상태의 처리
	FLOAT fDeltaTime = (time - m_fLastProcessTime) / m_fSpeedMul;
	FLOAT fProcessedTime = (time - m_fStartTime) / m_fSpeedMul;
	m_fLastProcessTime = time;
	m_fLeftTime -= fDeltaTime;

	if(m_fLeftTime <= 0.0f)
	{
		if(m_slRepeat != 0)
		{
			Start(time, TRUE);
			fProcessedTime = (time - m_fStartTime) / m_fSpeedMul;
		}
		else
		{
			m_ptrAttachTag->Deactivate();
			m_eState = ES_PLAY_END;
			return FALSE;
		}
	}
	retDeltaTime = fDeltaTime;
	retProcessedTime = fProcessedTime;
	if(retProcessedTime < 0)
	{
		bRenderInThisFrame = FALSE;
		bRetValue = TRUE;
		return FALSE;
	}
	return TRUE;
}

BOOL CEffect::PreRender()
{
	if( !Playing() || m_ptrAttachTag.IsNull() || !m_ptrAttachTag->Active() )
	{
		return FALSE;
	}
	if(m_bNotRenderAtThisFrame)
	{
		m_bNotRenderAtThisFrame = FALSE;
		return FALSE;
	}
	//tag를 그린다.
	//extern INDEX ska_bShowAttachedTag;
	//if(ska_bShowAttachedTag) m_ptrAttachTag->Render();
	return TRUE;
}

void CEffect::SetContent(const CEffect *pEffect)
{
	m_strName = pEffect->m_strName;
	m_fLifeTime = pEffect->m_fLifeTime;
	m_slRepeat = pEffect->m_slRepeat;
	m_fFadeInTime = pEffect->m_fFadeInTime;
	m_fFadeOutTime = pEffect->m_fFadeOutTime;
	m_fBoundingSphereRadius = pEffect->m_fBoundingSphereRadius;
}

void CEffect::Stop(FLOAT leftTime)
{
	m_slRepeat = 0;
	
	if(m_fLeftTime > leftTime)	//leftTime이 m_fLeftTime보다 클경우 할일 없음.
	{
		if(m_fLeftTime > m_fFadeOutTime)	//no fade
		{
			if(m_fFadeOutTime > leftTime)
			{
				m_fLeftTime = m_fFadeOutTime = leftTime;
			}
			else //m_fFadeOutTime <= leftTime
			{
				m_fLeftTime = leftTime;
			}
		}
		else //m_fLeftTime <= m_fFadeOutTime //fade 중
		{
			if(m_fFadeOutTime > leftTime)
			{
				FLOAT val = leftTime / m_fLeftTime;
				m_fLeftTime = leftTime;
				m_fFadeOutTime *= val;
			}
		}
	}
}

BOOL CEffect::BeginRender(CAnyProjection3D &apr, CDrawPort *pdp)
{
	return FALSE;
}

void CEffect::EndRender(BOOL bRestoreOrtho)
{
	ASSERTALWAYS("Don't call this function");
}


#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 2

void CEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;

	is.ExpectID_t("EFCT");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		CTString strTemp;
		is.GetLine_t(strTemp);
		SetName(strTemp.str_String);
		is >> m_fLifeTime;
		is >> m_slRepeat;
		is >> m_fFadeInTime;
		is >> m_fFadeOutTime;
		is >> m_fBoundingSphereRadius;
	}
	else if(ubVer == 1)
	{
		CTString strTemp;
		is.GetLine_t(strTemp);
		SetName(strTemp.str_String);
		is >> m_fLifeTime;
		UINT nRepeat;
		is >> (DWORD&)nRepeat;
		is >> m_fFadeInTime;
		is >> m_fFadeOutTime;
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;

	os.WriteID_t("EFCT");
	os << (UBYTE)CURRENT_VERSION;

	os.PutLine_t(m_strName.c_str());
	os << m_fLifeTime;
	os << m_slRepeat;
	os << m_fFadeInTime;
	os << m_fFadeOutTime;
	os << m_fBoundingSphereRadius;
}

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)