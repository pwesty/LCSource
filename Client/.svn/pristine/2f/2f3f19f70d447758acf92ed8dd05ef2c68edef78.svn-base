#include "StdH.h"
#include "CCameraEffect.h"
#include <Engine/Math/AdditionalFunction.h>
#include <Engine/Base/Stream.h>

ENGINE_API CCameraEffect::CCameraValue g_cvCameraShake;

//NEW_DELETE_IMPLEMENT(CFlameEffect);

CCameraEffect::CCameraEffect()
{
	CEffect::m_eType = ET_CAMERA;
}

CCameraEffect::~CCameraEffect()
{
	ClearInstance();
}

void CCameraEffect::ClearInstance()
{
	m_fSampleSplineLife = 0;
	m_cvCameraShakeSum = CCameraValue();
	m_cvOldCameraShakeSum = CCameraValue();
	m_iOnlyStartFadeOut = 0;
}

void CCameraEffect::Start(FLOAT time, BOOL restart)
{
	if(!PreStart(time, restart)) return;
	if(m_eState != ES_NOT_STARTED) return;

	ClearInstance();

	m_ssCameraShake.Prepare();
	m_fSampleSplineLife = m_ssCameraShake.GetKey(m_ssCameraShake.GetSampleCount()-1) - m_ssCameraShake.GetKey(0);

	PostStart();
}

void CCameraEffect::Stop(FLOAT leftTime)
{
	CEffect::Stop(leftTime);
	g_cvCameraShake += m_cvCameraShakeSum;
}

BOOL CCameraEffect::Process(FLOAT time)
{
	FLOAT fDeltaTime, fProcessedTime, fProcessedTimeOld = m_fLastProcessTime - m_fStartTime;
	BOOL bRet, bRender;
	if(!PreProcess(time, bRet, bRender, fDeltaTime, fProcessedTime))
	{
		g_cvCameraShake += m_cvCameraShakeSum;
		if(!bRender) SetNotRenderAtThisFrame();
		return bRet;
	}
/*
	CCameraValue cvShake, cvShakeOld;
	if(m_fLifeTime - fProcessedTime <= m_fFadeOutTime)
	{
		++m_iOnlyStartFadeOut;
		FLOAT ratio = (m_fLifeTime - fProcessedTime) / m_fFadeOutTime;
		cvShake = m_cvOldCameraShakeSum * (1-ratio);
		ratio = (m_fLifeTime - fProcessedTimeOld) / m_fFadeOutTime;
		cvShakeOld = m_cvOldCameraShakeSum * (1-ratio);
	}
	else
	{
		INDEX cnt = INDEX(fProcessedTime / m_fSampleSplineLife);
		if(m_ssCameraShake.GetSampleCount() <= 1) cnt = 0;
		cvShake = m_ssCameraShake.Value(fProcessedTime - cnt * m_fSampleSplineLife);
		cnt = INDEX(fProcessedTimeOld / m_fSampleSplineLife);
		if(m_ssCameraShake.GetSampleCount() <= 1) cnt = 0;
		cvShakeOld = m_ssCameraShake.Value(fProcessedTimeOld - cnt * m_fSampleSplineLife);
	}
	//process의 호출사이에 화면 회전이 될 수 있음.
	//처리는 각각의 카메라 부분에서 이루어짐.
	cvShakeOld.m_fHorizonal = 0;
	cvShakeOld.m_fVertical = 0;
	cvShakeOld.m_vMoveByCharCoord = FLOAT3D(0,0,0);

	cvShake -= cvShakeOld;
	m_cvCameraShakeSum -= cvShake;
	g_cvCameraShake += cvShake;
	if(m_iOnlyStartFadeOut == 1) m_cvOldCameraShakeSum = m_cvCameraShakeSum;
*/
	CCameraValue cvShake;
	INDEX cnt = INDEX(fProcessedTime / m_fSampleSplineLife);
	if(m_ssCameraShake.GetSampleCount() <= 1) cnt = 0;
	cvShake = m_ssCameraShake.Value(fProcessedTime - cnt * m_fSampleSplineLife);
	if(fProcessedTime <= m_fFadeInTime)
	{
		FLOAT ratio = fProcessedTime / m_fFadeInTime;
		cvShake *= ratio;
	}
	else if(m_fLifeTime - fProcessedTime <= m_fFadeOutTime)
	{
		FLOAT ratio = (m_fLifeTime - fProcessedTime) / m_fFadeOutTime;
		cvShake *= ratio;
	}
	g_cvCameraShake += cvShake;

	PostProcess();
	return TRUE;
}

CEffect *CCameraEffect::Copy()
{
	CCameraEffect *pRet = new CCameraEffect;
	if(pRet == NULL) return NULL;
	//CEffect의 content
	pRet->SetContent(this);
	//CCameraEffect content
	pRet->m_ssCameraShake = m_ssCameraShake;
	return pRet;
}

void CCameraEffect::Render()
{
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CCameraEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CEffect::Read(pIS);

	is.ExpectID_t("EFLM");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		ssReadFromStream(is, m_ssCameraShake, CCameraValue);
	}
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CCameraEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CEffect::Write(pOS);

	os.WriteID_t("EFLM");
	os << (UBYTE)CURRENT_VERSION;

	ssWriteToStream(os, m_ssCameraShake);
}

