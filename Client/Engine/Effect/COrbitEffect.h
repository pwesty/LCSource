#ifndef __CORBITEFFECT_H__
#define __CORBITEFFECT_H__

#include <string>
#include <vector>

#include "CEffect.h"

#define FADEIN_ORBIT_BEGIN			0
#define FADEIN_ORBIT_RADIUSX		0
#define FADEIN_ORBIT_PHASEMULX		1
#define FADEIN_ORBIT_RADIUSY		2
#define FADEIN_ORBIT_PHASEMULY		3
#define FADEIN_ORBIT_RADIUSZ		4
#define FADEIN_ORBIT_PHASEMULZ		5
#define FADEIN_ORBIT_OFFSET			6
#define FADEIN_MASK					0x0000007F

#define FADEOUT_ORBIT_BEGIN			16
#define FADEOUT_ORBIT_RADIUSX		16
#define FADEOUT_ORBIT_PHASEMULX		17
#define FADEOUT_ORBIT_RADIUSY		18
#define FADEOUT_ORBIT_PHASEMULY		19
#define FADEOUT_ORBIT_RADIUSZ		20
#define FADEOUT_ORBIT_PHASEMULZ		21
#define FADEOUT_ORBIT_OFFSET		22
#define FADEOUT_MASK				0x007F0000

class ENGINE_API COrbitEffect : public CEffect
{
public:
	struct sOrbitInfo
	{
		//contents
		// P = r * sin(pm * t + 2*PI*po)
		// P:좌표값, r:반지름, pm:PhaseMul, po:PhaseOffset, t:시간
		FLOAT fRadiusX;
		FLOAT fPhaseMulX;
		FLOAT fPhaseOffsetX;
		FLOAT fOffsetX;
		FLOAT fRadiusY;
		FLOAT fPhaseMulY;
		FLOAT fPhaseOffsetY;
		FLOAT fOffsetY;
		FLOAT fRadiusZ;
		FLOAT fPhaseMulZ;
		FLOAT fPhaseOffsetZ;
		FLOAT fOffsetZ;
		FLOAT fTimeBeforeStart;
		std::string strEffectName;
	};
	typedef std::vector<CEffect*>					effect_vector;
	typedef std::vector<sOrbitInfo>					orbit_vector;
	typedef CRefCountPtr<CTag>						tag_ptr;
	typedef std::vector<tag_ptr>					tag_vector;
public:
	COrbitEffect();
	virtual ~COrbitEffect();

	//NEW_DELETE_DEFINITION(COrbitEffect);

	virtual void Read(CTStream *pIS);
	virtual void Write(CTStream *pOS);
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Stop(FLOAT leftTime);

	virtual void SetNotRenderAtThisFrame();

	inline void SetGoalTagName(const char *szTagName)	{ m_strGoalTagName = szTagName;		}
	inline const char *GetGoalTagName() const			{ return m_strGoalTagName.c_str();	}

	virtual INDEX GetWantTagCount();
	virtual const char *GetWantTagName(INDEX index);
	virtual void SetWantTag(INDEX index, ptr_tag tag);

	inline void SetRotationType(EFFECT_OF_TAG_TYPE eott)	{ m_eRotation = eott;	}
	inline EFFECT_OF_TAG_TYPE GetRotationType()				{ return m_eRotation;	}

	inline void SetPositionType(EFFECT_OF_TAG_TYPE eott)	{ m_ePosition = eott;	}
	inline EFFECT_OF_TAG_TYPE GetPositionType()				{ return m_ePosition;	}

	INDEX AddOrbit(const sOrbitInfo &info);
	BOOL DeleteOrbit(INDEX index);
	sOrbitInfo *GetOrbit(INDEX index);
	INDEX GetOrbitCount()	{ return m_vectorOrbit.size();	}
	void ClearOrbit()		{ m_vectorOrbit.clear();		}

	void SetFadeInFlag(ULONG flags)		{ m_ulFadeFlag = (FADEIN_MASK & flags) | (FADEOUT_MASK & m_ulFadeFlag);	}
	void SetFadeOutFlag(ULONG flags)	{ m_ulFadeFlag = (FADEOUT_MASK & flags) | (FADEIN_MASK & m_ulFadeFlag);	}
	ULONG GetFadeInFlag()				{ return FADEIN_MASK & m_ulFadeFlag;	}
	ULONG GetFadeOutFlag()				{ return FADEOUT_MASK & m_ulFadeFlag;	}

	void SetFadeInDest(FLOAT destRadiusMul, FLOAT destPhaseMul)		{ m_fFadeInDestRadiusMul = destRadiusMul; m_fFadeInDestPhaseMul = destPhaseMul;		}
	void SetFadeOutDest(FLOAT destRadiusMul, FLOAT destPhaseMul)	{ m_fFadeOutDestRadiusMul = destRadiusMul; m_fFadeOutDestPhaseMul = destPhaseMul;	}
	FLOAT GetFadeInDestRadiusMul()		{ return m_fFadeInDestRadiusMul;	}
	FLOAT GetFadeInDestPhaseMul()		{ return m_fFadeInDestPhaseMul;		}
	FLOAT GetFadeOutDestRadiusMul()		{ return m_fFadeOutDestRadiusMul;	}
	FLOAT GetFadeOutDestPhaseMul()		{ return m_fFadeOutDestPhaseMul;	}
protected:
	COrbitEffect(const COrbitEffect& other) {}
	COrbitEffect &operator =(const COrbitEffect &other) { return *this; }

	void ClearInstance();
protected:
	//contents
	orbit_vector	m_vectorOrbit;
	ULONG			m_ulFadeFlag;
	FLOAT			m_fFadeInDestRadiusMul;
	FLOAT			m_fFadeInDestPhaseMul;
	FLOAT			m_fFadeOutDestRadiusMul;
	FLOAT			m_fFadeOutDestPhaseMul;
	std::string		m_strGoalTagName;
	EFFECT_OF_TAG_TYPE		m_eRotation;			//tag의 회전에 얼만큼 영향을 받을 것인가?
	EFFECT_OF_TAG_TYPE		m_ePosition;			//tag의 이동에 얼만큼 영향을 받을 것인가?
	//instance
	effect_vector	m_vectorEffect;
	tag_vector		m_vectorTag;
	ptr_tag			m_ptrGoalTag;
	BOOL			m_bFirstProcess;
	FLOAT3D			m_vTagPos;
	FLOATquat3D		m_qTagRot;
};

#endif //__CORBITEFFECT_H__
