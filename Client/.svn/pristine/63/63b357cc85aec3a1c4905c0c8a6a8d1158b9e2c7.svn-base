//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CPARTICLESPROCESSDYNAMICSTATE_H__
#define __CPARTICLESPROCESSDYNAMICSTATE_H__

#include <Engine/Base/Memory.h>
#include <Engine/Base/Types.h>
#include "CParticles.h"

#include <vector>
#include <algorithm>

#include <Engine/Graphics/Color.h>
#include "CSampleSpline.h"
#include "CParticlesCommonProcess.h"

class ENGINE_API CParticlesProcessDynamicState : public CParticlesCommonProcess
{
public:
	CParticlesProcessDynamicState();
	virtual ~CParticlesProcessDynamicState();

	//NEW_DELETE_DEFINITION(CParticlesProcessDynamicState);

	virtual void Read(CTStream *pIS);
	virtual void Write(CTStream *pOS);

	//다이나믹 관련
	inline void SetTwinklePeriod(FLOAT twiklePeriod)	{ m_fTwinklePeriod = twiklePeriod;	}
	inline FLOAT GetTwinklePeriod()						{ return m_fTwinklePeriod;			}
	inline void SetFadeTime(FLOAT fadeIn, FLOAT fadeOut)		{ m_fFadeInTime = fadeIn; m_fFadeOutTime = fadeOut;	}
	inline FLOAT GetFadeInTime()		{ return m_fFadeInTime;		}
	inline FLOAT GetFadeOutTime()		{ return m_fFadeOutTime;	}

	//color
	inline void EnableDynamicColor()	{ m_bDynamicColor = TRUE;	}
	inline void DisableDynamicColor()	{ m_bDynamicColor = FALSE;	}
	inline BOOL	IsDynamicColor()		{ return m_bDynamicColor;	}
	inline UINT AddColor(FLOAT time, COLOR color)	{ return m_ssColor.AddSample(time, color);	}
	inline UINT RemoveColor(SINT index)				{ return m_ssColor.RemoveSample(index);	}
	inline COLOR GetColorValue(UINT index)			{ return m_ssColor.GetValue(index);	}
	inline FLOAT GetColorTime(UINT index)			{ return m_ssColor.GetKey(index);	}
	inline UINT GetColorCount()						{ return m_ssColor.GetSampleCount();}
	
	//TexPos
	inline void EnableDynamicTexPos()	{ m_bDynamicTexPos = TRUE;	}
	inline void DisableDynamicTexPos()	{ m_bDynamicTexPos = FALSE;	}
	inline BOOL	IsDynamicTexPos()		{ return m_bDynamicTexPos;	}
	inline UINT AddTexPos(FLOAT time, UBYTE row, UBYTE col)	{ return m_ssTexPos.AddSample(time, CParticleTexPos(row, col));	}
	inline UINT RemoveTexPos(SINT index)					{ return m_ssTexPos.RemoveSample(index);		}
	inline CParticleTexPos GetTexPosValue(UINT index)		{ return m_ssTexPos.GetValue(index);	}
	inline FLOAT GetTexPosTime(UINT index)					{ return m_ssTexPos.GetKey(index);		}
	inline UINT GetTexPosCount()							{ return m_ssTexPos.GetSampleCount();	}

	//alpha
	inline void EnableDynamicAlpha()	{ m_bDynamicAlpha = TRUE;	}
	inline void DisableDynamicAlpha()	{ m_bDynamicAlpha = FALSE;	}
	inline BOOL	IsDynamicAlpha()		{ return m_bDynamicAlpha;	}
	inline UINT AddAlpha(FLOAT time, UBYTE alpha)	{ return m_ssAlpha.AddSample(time, alpha);	}
	inline UINT RemoveAlpha(SINT index)				{ return m_ssAlpha.RemoveSample(index);	}
	inline UBYTE GetAlphaValue(UINT index)			{ return m_ssAlpha.GetValue(index);	}
	inline FLOAT GetAlphaTime(UINT index)			{ return m_ssAlpha.GetKey(index);	}
	inline UINT GetAlphaCount()						{ return m_ssAlpha.GetSampleCount();}

	//size
	inline void EnableDynamicSize()		{ m_bDynamicSize = TRUE;	}
	inline void DisableDynamicSize()	{ m_bDynamicSize = FALSE;	}
	inline BOOL	IsDynamicSize()			{ return m_bDynamicSize;	}
	inline UINT AddSize(FLOAT time, FLOAT width, FLOAT height)	{ return m_ssSize.AddSample(time, CParticleSize(width, height));	}
	inline UINT RemoveSize(SINT index)				{return  m_ssSize.RemoveSample(index);		}
	inline CParticleSize GetSizeValue(UINT index)	{ return m_ssSize.GetValue(index);	}
	inline FLOAT GetSizeTime(UINT index)			{ return m_ssSize.GetKey(index);	}
	inline UINT GetSizeCount()						{ return m_ssSize.GetSampleCount();	}

	//mass
	inline void EnableDynamicMass()		{ m_bDynamicMass = TRUE;	}
	inline void DisableDynamicMass()	{ m_bDynamicMass = FALSE;	}
	inline BOOL	IsDynamicMass()			{ return m_bDynamicMass;	}
	inline UINT AddMass(FLOAT time, FLOAT mass)	{ return m_ssMass.AddSample(time, mass);	}
	inline UINT RemoveMass(SINT index)			{ return m_ssMass.RemoveSample(index);		}
	inline FLOAT GetMassValue(UINT index)		{ return m_ssMass.GetValue(index);	}
	inline FLOAT GetMassTime(UINT index)		{ return m_ssMass.GetKey(index);	}
	inline UINT GetMassCount()					{ return m_ssMass.GetSampleCount();	}

	//DeltaPos
	inline void EnableDynamicDeltaPos()		{ m_bDynamicDeltaPos = TRUE;	}
	inline void DisableDynamicDeltaPos()	{ m_bDynamicDeltaPos = FALSE;	}
	inline BOOL	IsDynamicDeltaPos()			{ return m_bDynamicDeltaPos;	}
	inline UINT AddDeltaPos(FLOAT time, FLOAT3D vDeltaPos)	{ return m_ssDeltaPos.AddSample(time, vDeltaPos);	}
	inline UINT RemoveDeltaPos(SINT index)					{ return m_ssDeltaPos.RemoveSample(index);		}
	inline FLOAT3D GetDeltaPosValue(UINT index)				{ return m_ssDeltaPos.GetValue(index);	}
	inline FLOAT GetDeltaPosTime(UINT index)				{ return m_ssDeltaPos.GetKey(index);	}
	inline UINT GetDeltaPosCount()							{ return m_ssDeltaPos.GetSampleCount();	}

	//Angle
	inline void EnableDynamicAngle()	{ m_bDynamicAngle = TRUE;	}
	inline void DisableDynamicAngle()	{ m_bDynamicAngle = FALSE;	}
	inline BOOL	IsDynamicAngle()		{ return m_bDynamicAngle;	}
	inline UINT AddAngle(FLOAT time, ANGLE3D angle)	{ return m_ssAngle.AddSample(time, angle);	}
	inline UINT RemoveAngle(SINT index)				{ return m_ssAngle.RemoveSample(index);		}
	inline ANGLE3D GetAngleValue(UINT index)		{ return m_ssAngle.GetValue(index);	}
	inline FLOAT GetAngleTime(UINT index)			{ return m_ssAngle.GetKey(index);			}
	inline UINT GetAngleCount()						{ return m_ssAngle.GetSampleCount();		}

	virtual CParticlesCommonProcess *Copy();
	virtual void Process(CParticles &particles, const FLOAT fDeltaTime);
protected:
	//content
	//알파의 특별한 경우인 fade, twinkle 처리.
	FLOAT	m_fTwinklePeriod;
	FLOAT	m_fFadeInTime;
	FLOAT	m_fFadeOutTime;
	//렌더링 관련(색, 알파, 텍스쳐), 절대값.
	BOOL	m_bDynamicColor;
	CSampleSpline<COLOR>	m_ssColor;
	BOOL	m_bDynamicAlpha;
	CSampleSpline<UBYTE>	m_ssAlpha;
	BOOL	m_bDynamicTexPos;
	CSampleSpline<CParticleTexPos, CParticleTexPosLerp>	m_ssTexPos;
	//물리적 성질 관련(크기, 무게), 절대값.
	BOOL	m_bDynamicSize;
	CSampleSpline<CParticleSize, CParticleSizeLerp>	m_ssSize;
	BOOL	m_bDynamicMass;
	CSampleSpline<FLOAT>	m_ssMass;
	//위치에 대한 Delta
	BOOL	m_bDynamicDeltaPos;
	CSampleSpline< FLOAT3D >	m_ssDeltaPos;
	//각도, 절대값.
	BOOL	m_bDynamicAngle;
	CSampleSpline< ANGLE3D >	m_ssAngle;

	//instance
};

#endif //__CPARTICLESPROCESSDYNAMICSTATE_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
