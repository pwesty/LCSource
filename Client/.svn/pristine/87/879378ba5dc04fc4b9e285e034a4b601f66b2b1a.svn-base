//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CCONESPACE_H__
#define __CCONESPACE_H__

#include <Engine/Base/Types.h>
#include "CRandomTable.h"
#include <Engine/Math/Functions.h>

class CConeSpace
{
public:
	CConeSpace() : m_vPosSrc(FLT_MAX,FLT_MAX,FLT_MAX)
				, m_fCloseRange(0), m_fLerpRatio(0)		{}
	~CConeSpace()										{}

	void Read(CTStream *pIS);	//"CParticle.cpp"
	void Write(CTStream *pOS);	//"CParticle.cpp"

	inline void SetCenter(const FLOAT3D &center)	{ m_vCenter = center;	}
	inline const FLOAT3D &GetCenter()	{ return m_vCenter; }

	inline void SetHeight(FLOAT heightUpper, FLOAT heightLower)
	{
		heightLower > heightUpper ? heightLower = heightUpper: NULL ;
		m_fHeightUpper = heightUpper;
		m_fHeightLower = heightLower;
	}
	inline FLOAT GetHeightLower()	{ return m_fHeightLower;		}
	inline FLOAT GetHeightUpper()	{ return m_fHeightUpper;		}

	inline void SetAngle(FLOAT angle)	{ m_fAngle = angle;	}
	inline FLOAT GetAngle()				{ return m_fAngle;	}

	inline void SetGroupValue(FLOAT closeRange, FLOAT lerpRatio)
	{
		m_fCloseRange = closeRange;
		m_fLerpRatio = lerpRatio;
	}
	inline FLOAT GetCloseRange()	{ return m_fCloseRange;			}
	inline FLOAT GetLerpRatio()		{ return m_fLerpRatio;			}

	inline FLOAT3D GetPointInThis();
	inline bool IsInThis(const FLOAT3D &point);

protected:
	//contents
	FLOAT	m_fHeightUpper;
	FLOAT	m_fHeightLower;
	FLOAT	m_fAngle;
	FLOAT3D	m_vCenter;
	FLOAT	m_fCloseRange;
	FLOAT	m_fLerpRatio;
	//instance
	FLOAT3D	m_vPosSrc;
};

FLOAT3D CConeSpace::GetPointInThis()
{
	if(m_fCloseRange == 0.0f || m_fLerpRatio == 0.0f)
	{
		FLOAT height = RndFactor() * (m_fHeightUpper - m_fHeightLower) + m_fHeightLower;
		FLOAT radius = height * tan(RadAngle(m_fAngle));
		FLOAT3D point = RndCylinderPos() * radius;
		point(2) = height;
		return point + m_vCenter;
	}
	else
	{
		if(m_vPosSrc(1) == FLT_MAX && m_vPosSrc(2) == FLT_MAX && m_vPosSrc(3) == FLT_MAX)
		{
			m_vPosSrc = RndCylinderPos();
		}
		FLOAT height = RndFactor() * (m_fHeightUpper - m_fHeightLower) + m_fHeightLower;
		FLOAT radius = height * tan(RadAngle(m_fAngle));
		FLOAT3D posTest = RndCylinderPos();
		if((posTest - m_vPosSrc).Magnitude() < m_fCloseRange * m_fCloseRange)
		{
			posTest = Lerp(posTest, m_vPosSrc, m_fLerpRatio);
		}
		else
		{
			m_vPosSrc = posTest;
		}
		FLOAT3D point = posTest * radius;
		point(2) = height;
		return point + m_vCenter;
	}
}

bool CConeSpace::IsInThis(const FLOAT3D &point)
{
	if( (point(2) - m_vCenter(2)) > m_fHeightUpper
		|| (point(2) - m_vCenter(2)) < m_fHeightLower ) return FALSE;
	FLOAT radius = (point(2) - m_vCenter(2)) * tan(RadAngle(m_fAngle));
	FLOAT3D point2 = point - m_vCenter;
	point2(2) = 0;
	return ( point2.Magnitude() <= (radius * radius) );
}

class CConeDoubleSpace
{
public:
	CConeDoubleSpace() : m_vPosSrc(FLT_MAX,FLT_MAX,FLT_MAX)
					, m_fCloseRange(0), m_fLerpRatio(0)		{}
	~CConeDoubleSpace()										{}

	void Read(CTStream *pIS);	//"CParticle.cpp"
	void Write(CTStream *pOS);	//"CParticle.cpp"

	inline void SetCenter(const FLOAT3D &center)	{ m_vCenter = center;	}
	inline const FLOAT3D &GetCenter()	{ return m_vCenter; }

	inline void SetHeight(FLOAT heightUpper, FLOAT heightLower)
	{
		heightLower > heightUpper ? heightLower = heightUpper: NULL ;
		m_fHeightUpper = heightUpper;
		m_fHeightLower = heightLower;
	}
	inline FLOAT GetHeightLower()	{ return m_fHeightLower;		}
	inline FLOAT GetHeightUpper()	{ return m_fHeightUpper;		}

	inline void SetAngle(FLOAT hotspot, FLOAT falloff)
	{
		hotspot > falloff ? hotspot = falloff: NULL ;
		m_fHotspot = hotspot;
		m_fFalloff = falloff;
	}
	inline FLOAT GetHotspot()	{ return m_fHotspot;	}
	inline FLOAT GetFalloff()	{ return m_fFalloff;	}

	inline void SetGroupValue(FLOAT closeRange, FLOAT lerpRatio)
	{
		m_fCloseRange = closeRange;
		m_fLerpRatio = lerpRatio;
	}
	inline FLOAT GetCloseRange()	{ return m_fCloseRange;			}
	inline FLOAT GetLerpRatio()		{ return m_fLerpRatio;			}

	inline FLOAT3D GetPointInThis();
	inline bool IsInThis(const FLOAT3D &point);

protected:
	//contents
	FLOAT	m_fHeightLower;
	FLOAT	m_fHeightUpper;
	FLOAT	m_fHotspot;
	FLOAT	m_fFalloff;
	FLOAT3D	m_vCenter;
	FLOAT	m_fCloseRange;
	FLOAT	m_fLerpRatio;
	//instance
	FLOAT3D	m_vPosSrc;
};

FLOAT3D CConeDoubleSpace::GetPointInThis()
{
	if(m_fCloseRange == 0.0f || m_fLerpRatio == 0.0f)
	{
		FLOAT height = RndFactor() * (m_fHeightUpper - m_fHeightLower) + m_fHeightLower;
		FLOAT radiusHotspot = height * tan(RadAngle(m_fHotspot));
		FLOAT radiusFalloff = height * tan(RadAngle(m_fFalloff));
		FLOAT temp = RndFactor() * (radiusFalloff - radiusHotspot) + radiusHotspot;
		FLOAT3D point = RndCylinderDir() * temp;
		point(2) = height;
		return point + m_vCenter;
	}
	else
	{
		if(m_vPosSrc(1) == FLT_MAX && m_vPosSrc(2) == FLT_MAX && m_vPosSrc(3) == FLT_MAX)
		{
			m_vPosSrc = RndCylinderDir();
		}
		FLOAT height = RndFactor() * (m_fHeightUpper - m_fHeightLower) + m_fHeightLower;
		FLOAT radiusHotspot = height * tan(RadAngle(m_fHotspot));
		FLOAT radiusFalloff = height * tan(RadAngle(m_fFalloff));
		FLOAT temp = RndFactor() * (radiusFalloff - radiusHotspot) + radiusHotspot;
		FLOAT3D point = RndCylinderDir();
		FLOAT testVal = (point - m_vPosSrc).Magnitude();
		if(testVal < m_fCloseRange * m_fCloseRange)
		{
			point = Lerp(point, m_vPosSrc, m_fLerpRatio);
			point.SafeNormalize();
		}
		else
		{
			m_vPosSrc = point;
		}
		point *= temp;
		point(2) = height;
		return point + m_vCenter;
	}
}

bool CConeDoubleSpace::IsInThis(const FLOAT3D &point)
{
	if( (point(2) - m_vCenter(2)) > m_fHeightUpper
		|| (point(2) - m_vCenter(2)) < m_fHeightLower ) return FALSE;
	FLOAT3D point2 = point - m_vCenter;
	FLOAT radiusHotspot = point2(2) * tan(RadAngle(m_fHotspot));
	FLOAT radiusFalloff = point2(2) * tan(RadAngle(m_fFalloff));
	point2(2) = 0;
	return ( point2.Magnitude() <= (radiusFalloff * radiusFalloff)
		&& point2.Magnitude() >= (radiusHotspot * radiusHotspot) );
}

#endif //__CCONESPACE_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
