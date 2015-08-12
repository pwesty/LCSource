//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CCYLINDERSPACE_H__
#define __CCYLINDERSPACE_H__

#include <Engine/Base/Types.h>
#include "CRandomTable.h"
#include <Engine/Math/Functions.h>


class CCylinderSpace
{
public:
	void Read(CTStream *pIS);
	void Write(CTStream *pOS);

	inline void SetCenter(const FLOAT3D &center)	{ m_vCenter = center;	}
	inline const FLOAT3D &GetCenter()	{ return m_vCenter; }

	inline void SetRadius(FLOAT radius)	{ m_fRadius = radius;	}
	inline FLOAT GetRadius()			{ return m_fRadius;		}

	inline void SetHeight(FLOAT height)	{ m_fHeight = height;	}
	inline FLOAT GetHeight()			{ return m_fHeight;		}

	inline FLOAT3D GetPointInThis();
	inline FLOAT3D GetPointAtThis();
	inline bool IsInThis(const FLOAT3D &point);

protected:
	FLOAT	m_fRadius;
	FLOAT	m_fHeight;
	FLOAT3D	m_vCenter;
};

FLOAT3D CCylinderSpace::GetPointInThis()
{
	FLOAT3D point = RndCylinderPos();
	point(1) *= m_fRadius;
	point(3) *= m_fRadius;
	point(2) *= m_fHeight;
	return point + m_vCenter;
}

FLOAT3D CCylinderSpace::GetPointAtThis()
{
	FLOAT3D point = RndCylinderDir();
	point(1) *= m_fRadius;
	point(3) *= m_fRadius;
	point(2) *= m_fHeight;
	return point + m_vCenter;
}

bool CCylinderSpace::IsInThis(const FLOAT3D &point)
{
	if((point(2) - m_vCenter(2)) > m_fHeight) return FALSE;
	FLOAT3D point2 = point - m_vCenter;
	point2(2) = 0;
	return ( point2.Magnitude() <= (m_fRadius * m_fRadius) );
}

class CCylinderDoubleSpace
{
public:
	void Read(CTStream *pIS);
	void Write(CTStream *pOS);

	inline void SetCenter(const FLOAT3D &center)	{ m_vCenter = center;	}
	inline const FLOAT3D &GetCenter()	{ return m_vCenter; }

	inline void SetHeight(FLOAT height)	{ m_fHeight = height;	}
	inline FLOAT GetHeight()			{ return m_fHeight;		}

	inline void SetRadius(FLOAT rInner, FLOAT rOuter)
	{
		ASSERT(rInner <= rOuter && rInner >= 0);
		m_fRadiusInner = rInner;
		m_fRadiusOuter = rOuter;
	}
	inline FLOAT GetRadiusInner()			{ return m_fRadiusInner;	}
	inline FLOAT GetRadiusOuter()			{ return m_fRadiusOuter;	}

	inline FLOAT3D GetPointInThis();
	inline FLOAT3D GetPointAtThis();
	inline bool IsInThis(const FLOAT3D &point);

protected:
	FLOAT	m_fHeight;
	FLOAT	m_fRadiusInner;
	FLOAT	m_fRadiusOuter;
	FLOAT3D	m_vCenter;
};

FLOAT3D CCylinderDoubleSpace::GetPointInThis()
{
	FLOAT temp = RndFactor() * (m_fRadiusOuter - m_fRadiusInner) + m_fRadiusInner;
	FLOAT3D point = RndCylinderDir();
	point(1) *= temp;
	point(3) *= temp;
	point(2) *= m_fHeight;
	return point + m_vCenter;
}

FLOAT3D CCylinderDoubleSpace::GetPointAtThis()
{
	FLOAT temp;
	if(RndFactor() >= 0.5f) temp = m_fRadiusOuter;
	else temp = m_fRadiusInner;
	FLOAT3D point = RndCylinderDir();
	point(1) *= temp;
	point(3) *= temp;
	point(2) *= m_fHeight;
	return point + m_vCenter;
}

bool CCylinderDoubleSpace::IsInThis(const FLOAT3D &point)
{
	if((point(2) - m_vCenter(2)) > m_fHeight) return FALSE;
	FLOAT3D point2 = point - m_vCenter;
	point2(2) = 0;
	FLOAT mag = point2.Magnitude();
	return ( mag <= (m_fRadiusOuter * m_fRadiusOuter) && mag >= (m_fRadiusInner * m_fRadiusInner) );
}

#endif //__CCYLINDERSPACE_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
