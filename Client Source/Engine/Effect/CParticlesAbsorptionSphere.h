//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CPARTICLESABSORPTIONSPHERE_H__
#define __CPARTICLESABSORPTIONSPHERE_H__

#include <Engine/Base/Types.h>
#include <Engine/Math/Functions.h>
#include "CParticlesAbsorption.h"
#include "CSphereSpace.h"

class ENGINE_API CParticlesAbsorptionSphere : public CParticlesAbsorption
{
public:
	CParticlesAbsorptionSphere();
	virtual ~CParticlesAbsorptionSphere();

	//NEW_DELETE_DEFINITION(CParticlesAbsorptionSphere);

	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);

	inline void SetSphere(const CSphereDoubleSpace &ss)	{ m_sphere = ss;	}
	inline const CSphereDoubleSpace &GetSphere()	{ return m_sphere;	}
	
	virtual CParticlesAbsorption *Copy();
	virtual void Process(CParticles &particles, const FLOAT fDeltaTime);
protected:
	//content
	CSphereDoubleSpace	m_sphere;
	//instance
};

#endif //__CPARTICLESABSORPTIONSPHERE_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
