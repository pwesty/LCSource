//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CFORCE_H__
#define __CFORCE_H__

#include <Engine/Base/Memory.h>
enum ForceType
{
	FT_ZERO			= 0,	//zero 힘.
	FT_DIRECTION	= 1,	//일정한 힘, 특정 방향
	FT_POINT		= 2,	//일정한 힘, 특정 지점에서 발생,
	FT_GRAVITY_POINT= 3,	//거리에 따른 힘. 특정 지점.
	FT_INVERSE_GRAVITY_POINT	=4,
};

class CForce
{
public:
	void Read(CTStream *istrFile);
	void Write(CTStream *ostrFile);

	inline void SetZeroForce()
	{
		m_ftType = FT_ZERO;
		m_fPower = 0;
		m_vDirection(1) = 0;
		m_vDirection(2) = 0;
		m_vDirection(3) = 0;
		m_vPostion(1) = 0;
		m_vPostion(2) = 0;
		m_vPostion(3) = 0;
	}
	inline void SetDirectionalForce(FLOAT power, const FLOAT3D &vDir)
	{
		m_ftType = FT_DIRECTION;
		m_fPower = power;
		m_vDirection = vDir;
	}
	inline void SetPointForce(FLOAT power, const FLOAT3D &vPos)
	{
		m_ftType = FT_POINT;
		m_fPower = power;
		m_vPostion = vPos;
	}
	inline void SetGravityPointForce(FLOAT power, const FLOAT3D &vPos)
	{
		m_ftType = FT_GRAVITY_POINT;
		m_fPower = power;
		m_vPostion = vPos;
	}
	inline void SetInverseGravityPointForce(FLOAT power, const FLOAT3D &vPos)
	{
		m_ftType = FT_INVERSE_GRAVITY_POINT;
		m_fPower = power;
		m_vPostion = vPos;
	}
	inline const FLOAT3D GetForce(const FLOAT3D &vPos)
	{
		static FLOAT3D ZeroVector(0,0,0);
		switch(m_ftType)
		{
		case FT_DIRECTION:
			{
				return m_vDirection * m_fPower;
			} break;
		case FT_POINT:
			{
				return (vPos - m_vPostion).Normalize() * m_fPower;
			} break;
		case FT_GRAVITY_POINT:
			{
				FLOAT3D vDist = vPos - m_vPostion;
				FLOAT fValue = vDist.Magnitude();
				if(fValue < 0.000001f) return ZeroVector;
				return vDist * (m_fPower / fValue * 0.0001f);
			} break;
		case FT_INVERSE_GRAVITY_POINT:
			{
				FLOAT3D vDist = vPos - m_vPostion;
				FLOAT fValue = vDist.Magnitude();
				if(fValue < 0.000001f) return ZeroVector;
				return vDist * (m_fPower * fValue * 0.0001f);
			} break;
		default:
			{
				return ZeroVector;
			} break;
		}
	}

	inline ForceType GetType()		{ return m_ftType;				}
	inline bool IsZeroForce()			{ return m_ftType == FT_ZERO;	}
	inline FLOAT GetPower()			{ return m_fPower;				}
	inline const FLOAT3D &GetPosition()		{ return m_vPostion;	}
	inline const FLOAT3D &GetDirection()	{ return m_vDirection;	}
protected:
	ForceType	m_ftType;
	FLOAT		m_fPower;
	FLOAT3D		m_vPostion;		//point인 경우 방출이 power가 양일때의 방향이 된다.
	FLOAT3D		m_vDirection;
};

#endif //__CFORCE_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
