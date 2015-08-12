//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CSKAEFFECT_H__
#define __CSKAEFFECT_H__

#include <Engine/Base/Memory.h>
#pragma warning(disable :4786)
#include <list>
#include <Engine/Base/FileName.h>
#include <Engine/Ska/ModelInstance.h>

#include "CEffect.h"
#include "CEffectControl.h"
#include <Engine/Base/Memory.h>
#include "FreeMemStack.h"

class CTag;
class CEffectControl;

class ENGINE_API CSkaEffect : public CEffect
{
public:
	CSkaEffect();
	virtual ~CSkaEffect();

	//NEW_DELETE_DEFINITION(CSkaEffect);

//안태훈 수정 시작	//(Remake Effect)(0.1)
	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);
//안태훈 수정 끝	//(Remake Effect)(0.1)
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Render();

	inline void SetColor(COLOR col)		{ m_colModel = col;		}
	inline COLOR GetColor()				{ return m_colModel;	}

	inline void SetRotationType(EFFECT_OF_TAG_TYPE eott)	{ m_eRotation = eott;	}
	inline EFFECT_OF_TAG_TYPE GetRotationType()				{ return m_eRotation;	}
	inline void SetPositionType(EFFECT_OF_TAG_TYPE eott)	{ m_ePosition = eott;	}
	inline EFFECT_OF_TAG_TYPE GetPositionType()				{ return m_ePosition;	}

	inline void SetStretch(const FLOAT3D &stretch)	{ m_vStretch = stretch;	}
	inline const FLOAT3D &GetStretch()				{ return m_vStretch;	}

	void SetPosTag(ptr_tag &ptrTag)	{ if(ptrTag.NotNull()) m_ptrPosTag = ptrTag;	}
	ptr_tag GetPosTag()				{ return m_ptrAttachTag;						}
	
	BOOL SetModelInstance(const CTFileName &filename);
	const CTFileName &GetSkaFileName()	{ return m_strModelFileName;	}
	CModelInstance *GetModelInstance() { return m_pCreatedModel; }

	void SetEffectControl(const CEffectControl &effectControl);
	CEffectControl *GetEffectControl()	{ return m_pEffectControl;	}
	void EraseEffectControl();

	FLOAT3D GetInstancePosition(FLOATmatrix3D &matRotTag = FLOATmatrix3D());
	FLOATquat3D GetInstanceRotation();

	//acess function
	virtual BOOL BeginRender(CAnyProjection3D &apr, CDrawPort *pdp);
	virtual void EndRender(BOOL bRestoreOrtho);

protected:
	CSkaEffect( CSkaEffect &other ) {}
	CSkaEffect &operator =(CSkaEffect &other) {return *this;}
	
protected:
	//content variable
	CTFileName			m_strModelFileName;		//Ska의 이름.
	COLOR				m_colModel;				//Ska의 색깔.
	FLOAT3D				m_vStretch;				//Ska의 크기.
	EFFECT_OF_TAG_TYPE	m_eRotation;			//tag의 회전에 얼만큼 영향을 받을 것인가?
	EFFECT_OF_TAG_TYPE	m_ePosition;			//tag의 이동에 얼만큼 영향을 받을 것인가?
	CEffectControl		*m_pEffectControl;		//effect control 없을때는 NULL
	//instance variable
	CModelInstance		*m_pCreatedModel;		//실제 로딩된 Ska
	FLOAT3D				m_vStartPosition;		//시작시 위치
	FLOATmatrix3D		m_matStartRotation;		//시작시 방향
	FLOAT3D				m_vPostion;				//dynamic pos시 사용
	ANGLE3D				m_vAngle;				//dynamic angle시 사용
	ptr_tag				m_ptrPosTag;			//dynamic pos시 마지막에 따라갈 Tag, 없으면 제자리
};

#endif //__CSKAEFFECT_H__
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)