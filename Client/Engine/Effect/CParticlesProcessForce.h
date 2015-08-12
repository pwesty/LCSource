//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CPARTICLESPROCESSFORCE_H__
#define __CPARTICLESPROCESSFORCE_H__

#include <Engine/Base/Memory.h>
#include <vector>
#include <algorithm>

#include "CParticles.h"
#include "CParticlesCommonProcess.h"
#include "CForce.h"

class ENGINE_API CParticlesProcessForce : public CParticlesCommonProcess
{
public:
	CParticlesProcessForce();

	//NEW_DELETE_DEFINITION(CParticlesProcessForce);

	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);

	inline void AddForce(const CForce &force)	{ m_vectorForce.push_back( force );	}
	inline const CForce &GetForce(UINT index)	{ return m_vectorForce[index];		}
	inline void Removeforce(UINT index)			{ m_vectorForce.erase(m_vectorForce.begin() + index);	}
	inline UINT GetForceCount()					{ return m_vectorForce.size();		}

	virtual CParticlesCommonProcess *Copy();
	virtual void Process(CParticles &particles, const FLOAT fDeltaTime);
protected:
	std::vector<CForce> m_vectorForce;
};

#endif //__CPARTICLESPROCESSFORCE_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
