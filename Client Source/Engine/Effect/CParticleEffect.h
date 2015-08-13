//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CPARTICLEEFFECT_H__
#define __CPARTICLEEFFECT_H__

#include <Engine/Base/Memory.h>
#pragma warning(disable :4786)
#include <list>
#include <string>

#include "CEffect.h"
#include "FreeMemStack.h"

class CTag;
class CParticleGroup;

class ENGINE_API CParticleEffect : public CEffect
{
public:
	CParticleEffect();
	virtual ~CParticleEffect();

	//NEW_DELETE_DEFINITION(CParticleEffect);

//안태훈 수정 시작	//(Remake Effect)(0.1)
	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);
//안태훈 수정 끝	//(Remake Effect)(0.1)
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Render();
	virtual void Stop(FLOAT leftTime);

	virtual INDEX GetWantTagCount();
	virtual const char *GetWantTagName(INDEX index);
	virtual void SetWantTag(INDEX index, ptr_tag tag);

	BOOL SetParticleGroup(std::string name);
	const std::string &GetParticleGroup()	{ return m_strParticleGroupName;	}

	inline void SetRotationType(EFFECT_OF_TAG_TYPE eott)	{ m_eRotation = eott;	}
	inline EFFECT_OF_TAG_TYPE GetRotationType()				{ return m_eRotation;	}

	virtual BOOL BeginRender(CAnyProjection3D &apr, CDrawPort *pdp);
	virtual void EndRender(BOOL bRestoreOrtho);

protected:
	CParticleEffect( CParticleEffect &other ) {}
	CParticleEffect &operator =(CParticleEffect &other) {return *this;}
	
protected:
	//content variable
	std::string			m_strParticleGroupName;	//CParticleGroupManager에 등록된 파티클 그룹의 이름.
	EFFECT_OF_TAG_TYPE	m_eRotation;			//tag의 회전에 얼만큼 영향을 받을 것인가?
	//instance variable
	CParticleGroup		*m_pParticleGroup;		//실제 Create된 파티클 그룹.
	ptr_tag				m_ptrTag;
	BOOL				m_bOnlyStart;
	BOOL				m_bShapeSizeRecalc;
};

#endif //__CPARTICLEEFFECT_H__
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)