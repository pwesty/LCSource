//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CEFFECTCONTROL_H__
#define __CEFFECTCONTROL_H__

#include <Engine/Base/Memory.h>
#include <Engine/Base/Types.h>
#include "CParticles.h"

#include <vector>
#include <algorithm>

#include <Engine/Graphics/Color.h>
#include "CSampleSpline.h"
#include "CParticlesCommonProcess.h"

class ENGINE_API CEffectControl
{
public:
	CEffectControl();
	virtual ~CEffectControl();

	//NEW_DELETE_DEFINITION(CEffectControl);

	void Read(CTStream *pIS);
	void Write(CTStream *pOS);
	
	//다이나믹 관련
	inline void SetTwinklePeriod(FLOAT twiklePeriod)	{ m_fTwinklePeriod = twiklePeriod;	}
	inline FLOAT GetTwinklePeriod()						{ return m_fTwinklePeriod;			}
	inline void SetFadeTime(FLOAT fadeIn, FLOAT fadeOut){ m_fFadeInTime = fadeIn; m_fFadeOutTime = fadeOut;	}
	inline FLOAT GetFadeInTime()		{ return m_fFadeInTime;		}
	inline FLOAT GetFadeOutTime()		{ return m_fFadeOutTime;	}

	//color
	inline void EnableDynamicColor()	{ m_bDynamicColor = TRUE;	}
	inline void DisableDynamicColor()	{ m_bDynamicColor = FALSE;	}
	inline BOOL	IsDynamicColor()		{ return m_bDynamicColor;	}
	inline UINT AddColor(FLOAT time, COLOR color)	{ return m_ssColor.AddSample(time, color);	}
	inline UINT RemoveColor(SINT index)				{ return m_ssColor.RemoveSample(index);		}
	inline COLOR GetColorValue(UINT index)			{ return m_ssColor.GetValue(index);			}
	inline FLOAT GetColorTime(UINT index)			{ return m_ssColor.GetKey(index);			}
	inline UINT GetColorCount()						{ return m_ssColor.GetSampleCount();		}
	
	//alpha
	inline void EnableDynamicAlpha()	{ m_bDynamicAlpha = TRUE;	}
	inline void DisableDynamicAlpha()	{ m_bDynamicAlpha = FALSE;	}
	inline BOOL	IsDynamicAlpha()		{ return m_bDynamicAlpha;	}
	inline UINT AddAlpha(FLOAT time, UBYTE alpha)	{ return m_ssAlpha.AddSample(time, alpha);	}
	inline UINT RemoveAlpha(SINT index)				{ return m_ssAlpha.RemoveSample(index);		}
	inline UBYTE GetAlphaValue(UINT index)			{ return m_ssAlpha.GetValue(index);			}
	inline FLOAT GetAlphaTime(UINT index)			{ return m_ssAlpha.GetKey(index);			}
	inline UINT GetAlphaCount()						{ return m_ssAlpha.GetSampleCount();		}

	//Stretch
	inline void EnableDynamicStretch()	{ m_bDynamicStretch = TRUE;	}
	inline void DisableDynamicStretch()	{ m_bDynamicStretch = FALSE;	}
	inline BOOL	IsDynamicStretch()		{ return m_bDynamicStretch;	}
	inline UINT AddStretch(FLOAT time, FLOAT3D vStretch)	{ return m_ssStretch.AddSample(time, vStretch);	}
	inline UINT RemoveStretch(SINT index)					{ return m_ssStretch.RemoveSample(index);		}
	inline FLOAT3D GetStretchValue(UINT index)				{ return m_ssStretch.GetValue(index);			}
	inline FLOAT GetStretchTime(UINT index)					{ return m_ssStretch.GetKey(index);				}
	inline UINT GetStretchCount()							{ return m_ssStretch.GetSampleCount();			}

	//Pos
	inline void EnableDynamicPos()	{ m_bDynamicPos = TRUE;	}
	inline void DisableDynamicPos()	{ m_bDynamicPos = FALSE;	}
	inline BOOL	IsDynamicPos()		{ return m_bDynamicPos;	}
	inline UINT AddPos(FLOAT time, FLOAT3D vPos)	{ return m_ssPos.AddSample(time, vPos);	}
	inline UINT RemovePos(SINT index)				{ return m_ssPos.RemoveSample(index);	}
	inline FLOAT3D GetPosValue(UINT index)			{ return m_ssPos.GetValue(index);		}
	inline FLOAT GetPosTime(UINT index)				{ return m_ssPos.GetKey(index);			}
	inline UINT GetPosCount()						{ return m_ssPos.GetSampleCount();		}
	inline CSampleSpline<FLOAT3D, CCubicSplineInterpolate<FLOAT3D> > &
				GetPosSpline()						{ return m_ssPos;						}

	//Angle(Euler)
	inline void EnableDynamicAngle()	{ m_bDynamicAngle = TRUE;	}
	inline void DisableDynamicAngle()	{ m_bDynamicAngle = FALSE;	}
	inline BOOL	IsDynamicAngle()		{ return m_bDynamicAngle;	}
	inline UINT AddAngle(FLOAT time, ANGLE3D vAngle)	{ return m_ssAngle.AddSample(time, vAngle);	}
	inline UINT RemoveAngle(SINT index)					{ return m_ssAngle.RemoveSample(index);		}
	inline ANGLE3D GetAngleValue(UINT index)			{ return m_ssAngle.GetValue(index);			}
	inline FLOAT GetAngleTime(UINT index)				{ return m_ssAngle.GetKey(index);			}
	inline UINT GetAngleCount()							{ return m_ssAngle.GetSampleCount();		}

	void Process( COLOR *pColor, FLOAT3D *pStretch, FLOAT3D *pPos, ANGLE3D *pAngle
				, const FLOAT fAge, const FLOAT fLifeTime, const FLOAT fDeltaTime );

	CEffectControl &operator=(const CEffectControl& other);
protected:
	//content
	//알파의 특별한 경우인 fade, twinkle 처리.
	FLOAT	m_fTwinklePeriod;
	FLOAT	m_fFadeInTime;
	FLOAT	m_fFadeOutTime;
	//기타 속성
	BOOL	m_bDynamicColor;
	CSampleSpline<COLOR>	m_ssColor;
	BOOL	m_bDynamicAlpha;
	CSampleSpline<UBYTE>	m_ssAlpha;
	BOOL	m_bDynamicStretch;
	CSampleSpline<FLOAT3D>	m_ssStretch;
	BOOL	m_bDynamicPos;
	CSampleSpline<FLOAT3D, CCubicSplineInterpolate<FLOAT3D> >	m_ssPos;
	BOOL	m_bDynamicAngle;
	CSampleSpline<ANGLE3D>	m_ssAngle;
	//instance
};

#endif //__CEFFECTCONTROL_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
