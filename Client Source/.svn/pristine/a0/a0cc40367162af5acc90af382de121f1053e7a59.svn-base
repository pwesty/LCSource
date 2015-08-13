//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CSOUNDEFFECT_H__
#define __CSOUNDEFFECT_H__

#include <Engine/Base/Memory.h>
#pragma warning(disable :4786)
#include <list>
#include <Engine/Base/FileName.h>
#include <Engine/Sound/SoundObject.h>
#include <Engine/Sound/SoundData.h>

#include "CEffect.h"
#include "FreeMemStack.h"

extern ENGINE_API BOOL g_bSndTestMode;

class CTag;

class ENGINE_API CSoundEffect : public CEffect
{
public:
	CSoundEffect();
	virtual ~CSoundEffect();

	//NEW_DELETE_DEFINITION(CSoundEffect);

//안태훈 수정 시작	//(Remake Effect)(0.1)
	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);
//안태훈 수정 끝	//(Remake Effect)(0.1)
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Stop(FLOAT fLeftTime);

	BOOL SetSound(const CTFileName &filename);
	const CTFileName &GetSoundFileName()	{ return m_strSoundFile; }
	void Set3DParameter(FLOAT fFallOff, FLOAT fHotSpot, FLOAT fVolume, FLOAT fPitch)
	{
		m_fFallOff = fFallOff;
		m_fHotSpot = fHotSpot;
		m_fVolume = fVolume;
		m_fPitch = fPitch;
	}
	FLOAT GetFallOff()	{ return m_fFallOff;	}
	FLOAT GetHotSpot()	{ return m_fHotSpot;	}
	FLOAT GetVolume()	{ return m_fVolume;		}
	FLOAT GetPitch()	{ return m_fPitch;		}
	void SetSoundPlayFlag(ULONG flag)	{ m_ulSoundFlag = flag; }
	ULONG GetSoundPlayFlag()			{ return m_ulSoundFlag;	}

protected:
	CSoundEffect( CSoundEffect &other ) {}
	CSoundEffect &operator =(CSoundEffect &other) {return *this;}
	
protected:
	//content variable
	CTFileName			m_strSoundFile;
	ULONG				m_ulSoundFlag;
	FLOAT				m_fFallOff;
	FLOAT				m_fHotSpot;
	FLOAT				m_fVolume;
	FLOAT				m_fPitch;
	//instance variable
	CSoundObject		m_soEffect;
	CSoundData			*m_pEffectSoundData;
	enum	FADE_PROCESS
	{
		FADE_INVALID = 0,
		FADE_IN = 1,
		FADE_NONE = 2,
		FADE_OUT = 3,
	};
	FADE_PROCESS		m_eFadeProcess;
	BOOL				m_bStart;
};

#endif //__CSOUNDEFFECT_H__
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)