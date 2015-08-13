//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include "StdH.h"

#include "CTag.h"
#include "CSoundEffect.h"

#include <algorithm>
#include <Engine/Templates/Stock_CSoundData.h>
#include <Engine/Base/Memory.h>

BOOL g_bSndTestMode = FALSE;

//NEW_DELETE_IMPLEMENT(CSoundEffect);

CSoundEffect::CSoundEffect()
: m_pEffectSoundData( NULL )
, m_eFadeProcess( FADE_INVALID )
{
	CEffect::m_eType = ET_SOUND;
}

CSoundEffect::~CSoundEffect()
{
	if(m_pEffectSoundData) _pSoundStock->Release(m_pEffectSoundData);
}

BOOL CSoundEffect::SetSound(const CTFileName &filename)
{
	try
	{
		m_pEffectSoundData = _pSoundStock->Obtain_t(filename);
	}
	catch(char *)
	{
		m_pEffectSoundData = NULL;
		return FALSE;
	}
	m_strSoundFile = filename;
	return TRUE;
}

CEffect *CSoundEffect::Copy()
{
	CSoundEffect *pRet = new CSoundEffect;
	if(pRet == NULL) return NULL;
	//CEffect의 content
	pRet->SetContent(this);
	//CSoundEffect의 content
	pRet->m_ulSoundFlag = m_ulSoundFlag;
	pRet->m_fFallOff = m_fFallOff;
	pRet->m_fHotSpot = m_fHotSpot;
	pRet->m_fVolume = m_fVolume;
	pRet->m_fPitch = m_fPitch;
	if(m_strSoundFile != "") pRet->SetSound(m_strSoundFile);

	return pRet;
}

void CSoundEffect::Start(FLOAT time, BOOL restart)
{
	if(!PreStart(time, restart)) return;
	if(m_pEffectSoundData == NULL) m_eState = ES_PLAY_END;
	if(m_eState != ES_NOT_STARTED) return;

	if(m_fPitch <= 0) m_fPitch = 0.000001f;
	if(m_fHotSpot < 0) m_fHotSpot = 0;
	if(m_fFallOff <= m_fHotSpot) m_fFallOff = m_fHotSpot * 1.01f + 0.000001f;
	m_soEffect.Set3DParameters(m_fFallOff, m_fHotSpot, m_fVolume, m_fPitch);
	m_soEffect.SetOwner(m_penOwner);

	m_bStart = TRUE;
	m_eFadeProcess = FADE_IN;

	PostStart();
}

BOOL CSoundEffect::Process(FLOAT time)
{
	FLOAT fDeltaTime, fProcessedTime;
	BOOL bRet, bRender;
	if(!PreProcess(time, bRet, bRender, fDeltaTime, fProcessedTime))
	{
		//m_soEffect.Fade(0, 0.01f);
		if(!bRender) SetNotRenderAtThisFrame();
		return bRet;
	}

	//play sound
	if(m_bStart)
	{
		if(!g_bSndTestMode) m_soEffect.Play(m_pEffectSoundData, m_ulSoundFlag);
		else m_soEffect.Play(m_pEffectSoundData, (m_ulSoundFlag & ~SOF_3D) | SOF_NONGAME);
		m_bStart = FALSE;
	}
	//fade in, [0, fade-in time)
/*
	if(fProcessedTime <= m_fFadeInTime && m_eFadeProcess == FADE_IN)
	{
		m_eFadeProcess = FADE_NONE;
		FLOAT fadeValue = fProcessedTime / m_fFadeInTime;
		m_soEffect.Fade(m_fVolume, m_fFadeInTime);
	}
*/	//Fade in은 지원하지 않음.
	//no fade, [life time - fade-out time, life time)
	else if(fProcessedTime <= m_fLifeTime)
	{
		m_eFadeProcess = FADE_OUT;
	}
	//fade out, [fade-in time, life time - fade-out time)
	else if(m_fLeftTime <= m_fFadeOutTime && m_eFadeProcess == FADE_OUT)
	{
		m_eFadeProcess = FADE_IN;
		FLOAT fadeValue = m_fLeftTime / m_fFadeOutTime;
		//if(m_fFadeOutTime > 0) m_soEffect.Fade(0, m_fFadeOutTime);
		//else m_soEffect.Fade(0, 0.1f);
	}

	PostProcess();
	return m_soEffect.IsPlaying();
}

void CSoundEffect::Stop(FLOAT fLeftTime)
{
	m_fFadeOutTime = fLeftTime;
	CEffect::Stop(fLeftTime);
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CSoundEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CEffect::Read(pIS);

	is.ExpectID_t("EFSN");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_strSoundFile;
		is >> m_ulSoundFlag;
		is >> m_fFallOff;
		is >> m_fHotSpot;
		is >> m_fVolume;
		is >> m_fPitch;
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CSoundEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CEffect::Write(pOS);

	os.WriteID_t("EFSN");
	os << (UBYTE)CURRENT_VERSION;

	os << m_strSoundFile;
//	CPrintF("%s\n", m_strSoundFile);	//log
	os << m_ulSoundFlag;
	os << m_fFallOff;
	os << m_fHotSpot;
	os << m_fVolume;
	os << m_fPitch;
}

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)