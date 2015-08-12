//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CSPHERESPACE_H__
#define __CSPHERESPACE_H__

#include <Engine/Base/Types.h>
#include "CRandomTable.h"

class CSphereSpace
{
public:
	void Read(CTStream *pIS);
	void Write(CTStream *pOS);

	inline void SetRadius(const FLOAT3D &center, FLOAT radius)	{ m_vCenter = center; m_fRadius = radius; }
	inline FLOAT GetRadius()			{ return m_fRadius; }
	inline const FLOAT3D &GetCenter()	{ return m_vCenter; }

	inline FLOAT3D GetPointInThis();
	inline FLOAT3D GetPointAtThis();
	inline bool IsInThis(const FLOAT3D &point);

protected:
	FLOAT	m_fRadius;
	FLOAT3D	m_vCenter;
};

FLOAT3D CSphereSpace::GetPointInThis()
{
	return (RndPos() * m_fRadius + m_vCenter);
}

FLOAT3D CSphereSpace::GetPointAtThis()
{
	return (RndDir() * m_fRadius + m_vCenter);
}

bool CSphereSpace::IsInThis(const FLOAT3D &point)
{
	return ( (point - m_vCenter).Magnitude() <= (m_fRadius * m_fRadius) );
}

class CSphereDoubleSpace
{
public:
	void Read(CTStream *pIS);
	void Write(CTStream *pOS);
	
	inline void SetRadius(const FLOAT3D &center, FLOAT rInner, FLOAT rOuter)
	{
		ASSERT(rInner <= rOuter && rInner >= 0);
		m_vCenter = center;
		m_fRadiusInner = rInner;
		m_fRadiusOuter = rOuter;
	}
	inline const FLOAT3D &GetCenter()	{ return m_vCenter;			}
	inline FLOAT GetRadiusOuter()		{ return m_fRadiusOuter;	}
	inline FLOAT GetRadiusInner()		{ return m_fRadiusInner;	}

	inline FLOAT3D GetPointInThis();
	inline FLOAT3D GetPointAtThis();
	inline bool IsInThis(const FLOAT3D &point);

protected:
	FLOAT	m_fRadiusOuter;
	FLOAT	m_fRadiusInner;
	FLOAT3D	m_vCenter;
};

FLOAT3D CSphereDoubleSpace::GetPointInThis()
{
	FLOAT temp = RndFactor() * (m_fRadiusOuter - m_fRadiusInner) + m_fRadiusInner;
	return (RndDir() * temp) + m_vCenter;
}

FLOAT3D CSphereDoubleSpace::GetPointAtThis()
{
	if(RndFactor() >= 0.5f)
	{
		return (RndDir() * m_fRadiusOuter + m_vCenter);
	}
	else
	{
		return (RndDir() * m_fRadiusInner + m_vCenter);
	}
}

bool CSphereDoubleSpace::IsInThis(const FLOAT3D &point)
{
	FLOAT mag = (point - m_vCenter).Magnitude();
	return ( mag <= (m_fRadiusOuter * m_fRadiusOuter) && mag >= (m_fRadiusInner * m_fRadiusInner) );
}

#endif //__CSPHERESPACE_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
