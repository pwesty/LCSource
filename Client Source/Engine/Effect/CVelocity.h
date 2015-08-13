//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CVELOCITY_H__
#define __CVELOCITY_H__

#include "CRandomTable.h"
#include <Engine/Base/Memory.h>
enum VelocityType
{
	VT_ZERO			= 0,	//움직이지 않음.
	VT_DIRECTION	= 1,	//일정한 방향
	VT_POINT		= 2,	//일정한 속도, 특정 지점에서 발생, 방사형
};

class CVelocity
{
public:
	void Read(CTStream *istrFile);
	void Write(CTStream *ostrFile);

	inline void SetZeroVelocity()
	{
	}
	inline void SetDirectionalVelocity(FLOAT power, const FLOAT3D &vDir)
	{
	}
	inline void SetPointVelocity(FLOAT power, const FLOAT3D &vPos)
	{
	}
	inline const FLOAT3D GetVelocity(const FLOAT3D &vPos)
	{
		static FLOAT3D ZeroVector(0,0,0);
		switch(m_ftType)
		{
		case VT_DIRECTION:
			{
			} break;
		case VT_POINT:
			{
			} break;
		default:
			{
				return ZeroVector;
			} break;
		}
	}

	inline ForceType GetType()		{ return m_vtType;				}
protected:
	VelocityType	m_vtType;
	FLOAT3D			m_vDirOrPos;
};

#endif //__CVELOCITY_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
