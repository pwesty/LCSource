//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CPARTICLESEMITTERCONE_H__
#define __CPARTICLESEMITTERCONE_H__

#include <Engine/Base/Memory.h>
#include <Engine/Base/Types.h>
#include "CParticles.h"
#include "CParticlesEmitter.h"
#include "CConeSpace.h"

class ENGINE_API CParticlesEmitterCone : public CParticlesEmitter
{
public:
	CParticlesEmitterCone();
	virtual ~CParticlesEmitterCone();

	virtual void Read(CTStream *pIS);
	virtual void Write(CTStream *pOS);

	//NEW_DELETE_DEFINITION(CParticlesEmitterCone);

	inline PARTICLES_EMITTER_TYPE GetType()		{ return m_petType; }

	inline void SetSpeed(FLOAT minSpeed, FLOAT maxSpeed)	{ m_fSpeedLower = minSpeed; m_fSpeedUpper = maxSpeed;	}
	inline FLOAT GetSpeedLower()	{ return m_fSpeedLower;	}
	inline FLOAT GetSpeedUpper()	{ return m_fSpeedUpper;	}

	inline void SetShape(const CConeDoubleSpace &cds)	{ m_cdsShape = cds;	}
	inline const CConeDoubleSpace &GetShape()	{ return m_cdsShape;	}

	inline void SetUsePos(BOOL bPos)	{ m_bUseShapePosition = bPos;	}
	inline BOOL GetUsePos()				{ return m_bUseShapePosition;	}

	inline void SetUseSpeed(BOOL bSpd)	{ m_buseShapeSpeed = bSpd;		}
	inline BOOL GetUseSpeed()			{ return m_buseShapeSpeed;		}

	virtual CParticlesEmitter *Copy();
	virtual void Process(CParticles &particles, const FLOAT fDeltaTime);
	virtual void SetShapeSize(FLOAT size);
protected:
	//content
	FLOAT	m_fSpeedLower, m_fSpeedUpper;
	BOOL	m_bUseShapePosition;
	BOOL	m_buseShapeSpeed;
	CConeDoubleSpace	m_cdsShape;
	//instance
};

#endif //__CPARTICLESEMITTERCONE_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
