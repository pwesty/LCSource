//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CPARTICLE_H__
#define __CPARTICLE_H__

#include <Engine/Base/Memory.h>
#include <Engine/Base/Types.h>

#include "CRandomTable.h"
#include <Engine/Graphics/Color.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/AdditionalFunction.h>
#include <Engine/Graphics/DrawPort.h>
#include <math.h>
#include <Engine/Base/Stream.h>

class CParticlePrototype;

class CParticleSize
{
public:
	CParticleSize() {}
	CParticleSize(FLOAT width, FLOAT height) : m_fWidth( width), m_fHeight( height ) {}

	inline void SetWidth(FLOAT fWidth)			{ m_fWidth = fWidth;		}
	inline FLOAT GetWidth() const				{ return m_fWidth;			}
	inline void SetHeight(FLOAT fHeight)		{ m_fHeight = fHeight;		}
	inline FLOAT GetHeight() const				{ return m_fHeight;			}
	inline void SetSize(FLOAT fWidth, FLOAT fHeight)	{ m_fWidth = fWidth; m_fHeight = fHeight;	}
	inline void SetSize(FLOAT size)				{ m_fWidth = m_fHeight = size;	}

	friend class CParticleSizeLerp;

	// Stream operations
	friend inline CTStream &operator>>(CTStream &strm, CParticleSize &ps)
	{
		strm >> ps.m_fWidth;
		strm >> ps.m_fHeight;
		return strm;
	}
	friend inline CTStream &operator<<(CTStream &strm, const CParticleSize &ps)
	{
		strm << ps.m_fWidth;
		strm << ps.m_fHeight;
		return strm;
	}
protected:
	FLOAT	m_fWidth;
	FLOAT	m_fHeight;
};

class CParticleSizeLerp
{
public:
	enum { SAMPLE_COUNT = 2 };

	inline CParticleSize operator()( const CParticleSize &size1, const CParticleSize &size2, float ratio)
	{
		return CParticleSize( Lerp(size1.m_fWidth,  size2.m_fWidth,  ratio)
							, Lerp(size1.m_fHeight, size2.m_fHeight, ratio) );
	}
};

class CParticleTexPosLerp;

class CParticleTexPos
{
public:
	CParticleTexPos() {}
	CParticleTexPos(UBYTE row, UBYTE col) : m_ubRow(row), m_ubCol(col) {}

	inline void SetPos(UBYTE row, UBYTE col)	{ m_ubRow = row; m_ubCol = col; }
	inline UBYTE GetRow() const	{ return m_ubRow;	}
	inline UBYTE GetCol() const	{ return m_ubCol;	}

	friend class CParticleTexPosLerp;

	// Stream operations
	friend inline CTStream &operator>>(CTStream &strm, CParticleTexPos &ptp)
	{
		strm >> ptp.m_ubRow;
		strm >> ptp.m_ubCol;
		return strm;
	}
	friend inline CTStream &operator<<(CTStream &strm, const CParticleTexPos &ptp)
	{
		strm << ptp.m_ubRow;
		strm << ptp.m_ubCol;
		return strm;
	}
protected:
	UBYTE m_ubRow;
	UBYTE m_ubCol;
};

class CParticleTexPosLerp
{
public:
	inline CParticleTexPos operator()(CParticleTexPos &pos1, CParticleTexPos &pos2, float ratio)
	{
		if(ratio > 0.999999f) return pos2;
		return pos1;
	}
};

class CQuaternionSLerp
{
public:
	inline FLOATquat3D operator()(FLOATquat3D &quat1, FLOATquat3D &quat2, float ratio)
	{
		return Slerp(ratio, quat1, quat2);
	}
};

enum PARTICLE_RENDER_TYPE
{
	PRT_QUAD = 0,
	PRT_LINE = 1,
	PRT_TRAIL = 2,
	PRT_TRAIL_VELOCITY = 3,
	PRT_FORCE_DWORD = 0xFFFFFFFF,
};

//지속 상태 Flag	//Not use yet
#define PTF_STATIC_MASK			(0x0000FFFF)
#define PTF_QUAD				(1UL<<0)
#define PTF_LINE				(1UL<<1)
#define PTF_TRAIL				(1UL<<2)
#define PTF_TRAIL_VELOCITY		(1UL<<3)
//임시 상태 Flag	//Not use yet
#define PTF_DYNAMIC_MASK		(0xFFFF0000)
#define PTF_CHANGE_POSITION		(1UL<<16)
#define PTF_CHANGE_VELOCITY		(1UL<<17)
#define PTF_CHANGE_ACCELERATION	(1UL<<17)

class ENGINE_API CParticle
{
public:
//안태훈 수정 시작	//(Remake Effect)(0.1)
	//void Read(CTStream *istrFile);
	//void Write(CTStream *ostrFile);
//안태훈 수정 끝	//(Remake Effect)(0.1)
	CParticle() { ResetZero(); }

	//access functions
	void SetFlag(DWORD flag)					{ m_dwFlag = flag;			}
	DWORD GetFlag()								{ return m_dwFlag;			}
	void SetLifeTime(FLOAT fLifeTime)			{ m_fLifeTime = fLifeTime;	}
	FLOAT GetLifeTime() const					{ return m_fLifeTime;		}
	void SetAge(FLOAT fAge)						{ m_fAge = fAge;			}
	FLOAT GetAge() const						{ return m_fAge;			}
	void SetSize(FLOAT fSize)					{ m_fWidth = m_fHeight = fSize;	}
	void SetWidth(FLOAT fWidth)					{ m_fWidth = fWidth;		}
	FLOAT GetWidth() const						{ return m_fWidth;			}
	void SetHeight(FLOAT fHeight)				{ m_fHeight = fHeight;		}
	FLOAT GetHeight() const						{ return m_fHeight;			}
	void SetTexPos(UBYTE row, UBYTE col)		{ m_ubRow = row; m_ubCol = col;	}
	UBYTE GetTexRow()							{ return m_ubRow;			}
	UBYTE GetTexCol()							{ return m_ubCol;			}
	void SetMass(FLOAT fMass)					{ m_fMass = fMass;			}
	FLOAT GetMass() const						{ return m_fMass;			}
	void SetColorNoAlpha(COLOR color)			{ m_colColor = (color & C_WHITE) | (m_colColor & CT_OPAQUE);	}
	COLOR GetColorNoAlpha()  const				{ return m_colColor & C_WHITE;		}
	void SetColorOnlyAlpha(COLOR color)			{ m_colColor = (m_colColor & C_WHITE) | (color & CT_OPAQUE);	}
	COLOR GetColorOnlyAlpha()  const			{ return m_colColor & CT_OPAQUE;	}
	void SetColor(COLOR color)					{ m_colColor = color;		}
	COLOR GetColor()  const						{ return m_colColor;		}
	void SetPosition(const FLOAT3D &pos)		{ m_vPosition = pos;		}
	const FLOAT3D &GetPosition() const			{ return m_vPosition;		}
	void SetDirection(const FLOATquat3D &quat)	{ m_quatDirection = quat;	}
	const FLOATquat3D &GetDirection()			{ return m_quatDirection;	}
	void SetVelocity(const FLOAT3D &velocity)	{ m_vVelocity = velocity;	}
	const FLOAT3D &GetVelocity() const			{ return m_vVelocity;		}
	void SetAcceleration(const FLOAT3D &acc)	{ m_vAcceleration = acc;	}
	const FLOAT3D &GetAcceleration() const		{ return m_vAcceleration;	}

	FLOAT GetProgressValue() const		{ return m_fAge / m_fLifeTime; }

	void ResetZero();

	//움직임 함수(deltaTime내에서 값의 변화는 무시하고 현재값으로 일정하다고 봄)
	//속도변화가 크다면 오차도 커진다. (속도변화 * deltaTime * 0.5f만큼의 오차)
	inline void MoveVelocity(FLOAT deltaTime);
	inline void MoveAcceleration(FLOAT deltaTime);
	//회전함수
	inline void Rotate(FLOAT angle);
	inline void Rotate(FLOAT angle, const FLOAT3D &axis);	//젤 빠름.
	inline void Rotate(const FLOATquat3D &quat);
	//생명 감소 함수
	inline BOOL Live(FLOAT deltaTime)
	{
		m_fAge += deltaTime;
		if(m_fLifeTime <= m_fAge) return FALSE;
		else return TRUE;
	}
	//살아있는지 검사
	inline BOOL Live() const
	{
		if(m_fLifeTime < m_fAge) return FALSE;
		else return TRUE;
	}
	inline void RenderQuad()
	{
		//quatnion의 angle만을 사용한다.
		FLOAT angle = 0.0f;
		if( !FloatSame(m_quatDirection.q_w, 1.0f) && !FloatSame(m_quatDirection.q_w, -1.0f) )
			angle = AngleRad(2.0f * acos( m_quatDirection.q_w ));
		//Particle_RenderSquare( m_vPosition, m_fWidth, angle, m_colColor );
		Particle_RenderSquare( m_vFinalPos, m_fWidth, angle, m_colColor );
	}
	inline void RenderLine()
	{
		FLOAT3D vOtherPos = m_vPosition + VectorOnlyYRotate(m_fHeight, m_quatDirection);
		//Particle_RenderLine( m_vPosition, vOtherPos, m_fWidth, m_colColor );
		Particle_RenderLine( m_vFinalPos, vOtherPos, m_fWidth, m_colColor );
	}
	inline void RenderTrail()
	{
		FLOAT3D velocity = m_vVelocity;
		FLOAT3D vOtherPos = m_vPosition + velocity.Normalize() * (-m_fHeight);
		//Particle_RenderLine( vOtherPos, m_vPosition, m_fWidth, m_colColor );
		Particle_RenderLine( vOtherPos, m_vFinalPos, m_fWidth, m_colColor );
	}
	inline void RenderTrailVelocity()
	{
		FLOAT3D vOtherPos = m_vPosition + m_vVelocity * (-m_fHeight);
		//Particle_RenderLine( vOtherPos, m_vPosition, m_fWidth, m_colColor );
		Particle_RenderLine( vOtherPos, m_vFinalPos, m_fWidth, m_colColor );
	}
	friend class CParticlePrototype;
protected:
	DWORD		m_dwFlag;		//not use yet.
	FLOAT		m_fLifeTime;
	FLOAT		m_fAge;
	FLOAT		m_fWidth;
	FLOAT		m_fHeight;
	UBYTE		m_ubRow;
	UBYTE		m_ubCol;
	FLOAT		m_fMass;
	COLOR		m_colColor;
	FLOAT3D		m_vPosition;
	FLOATquat3D	m_quatDirection;
	FLOAT3D		m_vVelocity;
	FLOAT3D		m_vAcceleration;
public:
	FLOAT3D		m_vFinalPos;
/*
public:
	union	//공용으로 사용하기 위한 멤버변수
	{
		struct { BOOL			m_bValue[4];	};
		struct { SLONG			m_slValue[4];	};
		struct { ULONG			m_ulValue[4];	};
		struct { FLOAT			m_fValue[4];	};
		struct { COLOR			m_colValue[4];	};
		struct { FLOAT3D		m_vValue;		};
		struct { FLOATquat3D	m_quatValue;	};
	};
*/	//여러 Process에 걸쳐 사용될 수 없으므로 의미없다고 판단함.
	//Flag에서 현재 사용중인 Field를 표시할 수 있지 않을까?
};

inline void CParticle::ResetZero()
{
	m_dwFlag = 0;
	m_fLifeTime = 0;
	m_fAge = 0;
	m_fWidth = 1;
	m_fHeight = 1;
	m_ubRow = 0;
	m_ubCol = 0;
	m_fMass = 1;
	m_colColor = 0;
	m_quatDirection = FLOATquat3D(1,0,0,0);
	m_vPosition = FLOAT3D(0,0,0);
	m_vVelocity = FLOAT3D(0,0,0);
	m_vAcceleration = FLOAT3D(0,0,0);
}

inline void CParticle::MoveVelocity(FLOAT deltaTime)
{
	//s = v * t
	m_vPosition += m_vVelocity * deltaTime;
}

inline void CParticle::MoveAcceleration(FLOAT deltaTime)
{
	//s = v0 * t + 1/2 * a * t^2
	m_vPosition += m_vVelocity * deltaTime + m_vAcceleration * (0.5f * deltaTime * deltaTime);
	//v = v0 + a * t
	m_vVelocity += m_vAcceleration * deltaTime;
}

inline void CParticle::Rotate(FLOAT angle)
{
	FLOAT a;
	FLOAT3D axis;
	m_quatDirection.ToAxisAngle(axis, a);
	a += angle;
	m_quatDirection.FromAxisAngle(axis, a);
}

inline void CParticle::Rotate(FLOAT angle, const FLOAT3D &axis)
{
	FLOATquat3D quat;
	quat.FromAxisAngle(axis, angle);
	m_quatDirection *= quat;
}

inline void CParticle::Rotate(const FLOATquat3D &quat)
{
	m_quatDirection *= quat;
}

class ENGINE_API CParticlePrototype
{
public:
//안태훈 수정 시작	//(Remake Effect)(0.1)
	void Read(CTStream *istrFile);
	void Write(CTStream *ostrFile);
//안태훈 수정 끝	//(Remake Effect)(0.1)

	inline void SetLower(const CParticle &particle) { m_particleLower = particle; }
	inline void SetUpper(const CParticle &particle) { m_particleUpper = particle; }
	inline void CreateTo(CParticle &particle);
	inline void CopyUpperTo(CParticle &particle) const	{ particle = m_particleUpper; }
	inline void CopyLowerTo(CParticle &particle) const	{ particle = m_particleLower; }
	
	inline FLOAT GetMaxLifeTime()
	{
		if(m_particleLower.m_fLifeTime < m_particleUpper.m_fLifeTime) return m_particleUpper.m_fLifeTime;
		else return m_particleLower.m_fLifeTime;
	}

	inline void SetPositionRange(const FLOAT3D &posLower, const FLOAT3D &posUpper)
	{
		m_particleLower.m_vPosition = posLower;
		m_particleUpper.m_vPosition = posUpper;
	}
	inline void SetVelocityRange(const FLOAT3D &velLower, const FLOAT3D &velUpper)
	{
		m_particleLower.m_vVelocity = velLower;
		m_particleUpper.m_vVelocity = velUpper;
	}
	inline void SetAccelerationRange(const FLOAT3D &accLower, const FLOAT3D &accUpper)
	{
		m_particleLower.m_vAcceleration = accLower;
		m_particleUpper.m_vAcceleration = accUpper;
	}
	inline void SetTexPos(UBYTE row, UBYTE col)
	{
		m_particleLower.m_ubRow = row;
		m_particleLower.m_ubCol = col;
	}
	inline void SetAngle(ANGLE3D angleLow, ANGLE3D anglehigh)
	{
		m_particleLower.m_quatDirection.FromEuler(angleLow);
		m_particleUpper.m_quatDirection.FromEuler(anglehigh);
	}
protected:
	//파티클값의 변화 가능 정도. [lower, upper] 범위임.
	CParticle	m_particleUpper;
	CParticle	m_particleLower;
};


inline void CParticlePrototype::CreateTo(CParticle &particle)
{
	particle.m_dwFlag = 0;
	particle.m_ubRow = m_particleLower.m_ubRow;
	particle.m_ubCol = m_particleLower.m_ubCol;
	particle.m_fLifeTime = Lerp(m_particleLower.m_fLifeTime, m_particleUpper.m_fLifeTime, RndFactor());
	particle.m_fAge = Lerp(m_particleLower.m_fAge, m_particleUpper.m_fAge, RndFactor());
	particle.m_fWidth = Lerp(m_particleLower.m_fWidth, m_particleUpper.m_fWidth, RndFactor());
	particle.m_fHeight = Lerp(m_particleLower.m_fHeight, m_particleUpper.m_fHeight, RndFactor());
	particle.m_fMass = Lerp(m_particleLower.m_fMass, m_particleUpper.m_fMass, RndFactor());
	particle.m_quatDirection = Slerp(RndFactor(), m_particleLower.m_quatDirection, m_particleUpper.m_quatDirection);
	particle.m_colColor = LerpColorInline(m_particleLower.m_colColor, m_particleUpper.m_colColor, RndFactor());
	//particle.m_vPosition = Lerp(m_particleLower.m_vPosition, m_particleUpper.m_vPosition, RndFactor());
	//particle.m_vVelocity = Lerp(m_particleLower.m_vVelocity, m_particleUpper.m_vVelocity, RndFactor());
	//particle.m_vAcceleration = Lerp(m_particleLower.m_vAcceleration, m_particleUpper.m_vAcceleration, RndFactor());
	particle.m_vPosition = FLOAT3D(0,0,0);
	particle.m_vVelocity = FLOAT3D(0,0,0);
	particle.m_vAcceleration = FLOAT3D(0,0,0);
}

#endif //__CPARTICLE_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
