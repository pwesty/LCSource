//안태훈 수정 시작	//(Remake Effect)(0.1)
#ifndef __CPARTICLESPROCESSPOINTGOAL_H__
#define __CPARTICLESPROCESSPOINTGOAL_H__

#include <Engine/Base/Memory.h>
#include "CParticles.h"
#include "CParticlesCommonProcess.h"
#include "CTag.h"
#include <string>

class ENGINE_API CParticlesProcessPointGoal : public CParticlesCommonProcess
{
public:
	CParticlesProcessPointGoal();

	//NEW_DELETE_DEFINITION(CParticlesProcessPointGoal);

	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);

	inline void SetPointGoal(const FLOAT3D &point)	{ m_vPointGoal = point;	}
	inline const FLOAT3D &GetPointGoal()			{ return m_vPointGoal;	}

	inline void SetLerpValue(FLOAT ratio, FLOAT speed)
	{ m_fLerpRatio = ratio; m_fLerpSpeed = speed;	}
	inline FLOAT GetLerpRatio()	{ return m_fLerpRatio;	}
	inline FLOAT GetLerpSpeed()	{ return m_fLerpSpeed;	}

	inline void SetSpeed(FLOAT speed)				{ m_fSpeed = speed;	}
	inline FLOAT GetSpeed()							{ return m_fSpeed;	}

	inline void SetGoalTagName(const char *szTagName)	{ m_strGoalTagName = szTagName;		}
	inline const char *GetGoalTagName() const			{ return m_strGoalTagName.c_str();	}

	virtual INDEX GetWantTagCount();
	virtual const char *GetWantTagName(INDEX index);
	virtual void SetWantTag(INDEX index, ptr_tag tag);

	virtual CParticlesCommonProcess *Copy();
	virtual void Process(CParticles &particles, const FLOAT fDeltaTime);
protected:
	//content
	FLOAT3D	m_vPointGoal;
	FLOAT	m_fSpeed;
	FLOAT	m_fLerpRatio;
	FLOAT	m_fLerpSpeed;
	std::string m_strGoalTagName;
	//instance
	ptr_tag	m_ptrGoalTag;
};

#endif //__CPARTICLESPROCESSPOINTGOAL_H__
//안태훈 수정 끝	//(Remake Effect)(0.1)
