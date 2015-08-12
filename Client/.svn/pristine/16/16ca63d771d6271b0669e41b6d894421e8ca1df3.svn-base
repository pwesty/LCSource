//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CMDLEFFECT_H__
#define __CMDLEFFECT_H__

#include <Engine/Base/Memory.h>
#pragma warning(disable :4786)
#include <list>
#include <Engine/Base/FileName.h>
#include <Engine/Models/ModelObject.h>

#include "CEffect.h"

class CTag;
class CAnyProjection3D;
class CDrawPort;
class CEffectControl;

class ENGINE_API CMdlEffect : public CEffect
{
public:
	CMdlEffect();
	virtual ~CMdlEffect();

	//NEW_DELETE_DEFINITION(CMdlEffect);

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

	inline void SetStretch(const FLOAT3D &stretch)	{ m_vStretch = stretch;	}
	inline const FLOAT3D &GetStretch()				{ return m_vStretch;	}

	inline void SetRotationType(EFFECT_OF_TAG_TYPE eott)	{ m_eRotation = eott;	}
	inline EFFECT_OF_TAG_TYPE GetRotationType()				{ return m_eRotation;	}
	inline void SetPositionType(EFFECT_OF_TAG_TYPE eott)	{ m_ePosition = eott;	}
	inline EFFECT_OF_TAG_TYPE GetPositionType()				{ return m_ePosition;	}

	void SetPosTag(ptr_tag &ptrTag)	{ if(ptrTag.NotNull()) m_ptrPosTag = ptrTag;	}
	ptr_tag GetPosTag()				{ return m_ptrPosTag;							}

	void SetEffectControl(const CEffectControl &effectControl);
	CEffectControl *GetEffectControl()	{ return m_pEffectControl;	}
	void EraseEffectControl();

	BOOL SetModelObject(const CTFileName &strMdlFileName, const CTFileName &strTexFileName);
	const CTFileName &GetMdlFileName()		{ return m_strMdlFileName;		}
	const CTFileName &GetMdlTexFileName()	{ return m_strTextureFileName;	}

	inline void SetOverDraw(BOOL overDraw)	{ m_bOverDraw = overDraw;	}
	inline BOOL GetOverDraw()				{ return m_bOverDraw;		}

	//나중엔 필요 없어질 것 같다.(GUI완성시)
	inline FLOAT GetMdlAnimLength() { if(m_pModelObject) return m_pModelObject->GetAnimLength(0); else return 0.0f; }

	virtual BOOL BeginRender(CAnyProjection3D &apr, CDrawPort *pdp);
	virtual void EndRender(BOOL bRestoreOrtho);

protected:
	CMdlEffect( CMdlEffect &other ) {}
	CMdlEffect &operator =(CMdlEffect &other) {return *this;}
	
protected:
	//content variable
	CTFileName			m_strMdlFileName;		//MDL파일이름
	CTFileName			m_strTextureFileName;	//MDL에서 쓰는 Texture의 파일이름
	COLOR				m_colModel;				//MDL의 색깔.
	FLOAT3D				m_vStretch;				//MDL의 크기.
	EFFECT_OF_TAG_TYPE	m_eRotation;			//tag의 회전에 얼만큼 영향을 받을 것인가?
	EFFECT_OF_TAG_TYPE	m_ePosition;			//tag의 이동에 얼만큼 영향을 받을 것인가?
	CEffectControl		*m_pEffectControl;		//effect control 없을때는 NULL
	BOOL				m_bOverDraw;			//Depth test를 끄고 그릴것인가?
	//instance variable
	CModelObject		*m_pModelObject;		//실제 로딩한 MDL
	CTextureData		*m_pModelTextureData;	//실제 로디안 Texture
	FLOAT3D				m_vStartPosition;		//시작시 위치
	FLOATmatrix3D		m_matStartRotation;		//시작시 방향
	FLOAT3D				m_vPostion;				//dynamic pos시 사용
	ANGLE3D				m_vAngle;				//dynamic angle시 사용
	ptr_tag				m_ptrPosTag;			//dynamic pos시 마지막에 따라갈 Tag, 없으면 제자리
	BOOL				m_bOnlyStart;
};

#endif //__CMDLEFFECT_H__
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
