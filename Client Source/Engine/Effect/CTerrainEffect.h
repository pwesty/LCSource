//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CTERRAINEFFECT_H__
#define __CTERRAINEFFECT_H__

#pragma warning(disable :4786)

#include <Engine/Base/Memory.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Base/FileName.h>
#include <Engine/Base/Rect.h>
#include "CEffect.h"

class CTag;
class CAnyProjection3D;
class CDrawPort;
class CTerrain;

class ENGINE_API CTerrainEffect : public CEffect
{
public:
	CTerrainEffect();
	virtual ~CTerrainEffect();

	//NEW_DELETE_DEFINITION(CTerrainEffect);
	
//안태훈 수정 시작	//(Remake Effect)(0.1)
	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);
//안태훈 수정 끝	//(Remake Effect)(0.1)
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Render();
	
	BOOL SetTexture(const CTFileName &filename);
	const CTFileName &GetTexFileName()	{ return m_strTextureFileName;	}

	//acess function
	inline void SetArea(const CRectFlt &rect)		{ m_rcArea = rect; }
	inline CRectFlt GetArea()						{ return m_rcArea; }
	inline void SetColor(COLOR color)			{ m_colEffect = color; }
	inline COLOR GetColor()						{ return m_colEffect; }

	inline void SetBlendType(PredefinedBlendType blendType)	{ m_eBlendType = blendType; }
	inline PredefinedBlendType GetBlendType()				{ return m_eBlendType;		}

	inline void SetRotationType(EFFECT_OF_TAG_TYPE eott)	{ m_eRotation = eott;	}
	inline EFFECT_OF_TAG_TYPE GetRotationType()				{ return m_eRotation;	}

	inline void SetPositionType(EFFECT_OF_TAG_TYPE eott)	{ m_ePosition = eott;	}
	inline EFFECT_OF_TAG_TYPE GetPositionType()				{ return m_ePosition;	}

	static inline CTerrain *GetTerrain() { return m_pTerrain; }
	static inline void SetTerrain(CTerrain *pTerrain)	{ m_pTerrain = pTerrain; }

	virtual BOOL BeginRender(CAnyProjection3D &apr, CDrawPort *pdp);
	virtual void EndRender(BOOL bRestoreOrtho);
protected:
	//content variable
	CTFileName			m_strTextureFileName;	//texture의 파일 이름
	CRectFlt			m_rcArea;				//범위, Tag의 상대좌표계이용.
	COLOR				m_colEffect;			//색깔.
	PredefinedBlendType	m_eBlendType;
	EFFECT_OF_TAG_TYPE	m_eRotation;			//tag의 회전에 얼만큼 영향을 받을 것인가?
	EFFECT_OF_TAG_TYPE	m_ePosition;			//tag의 이동에 얼만큼 영향을 받을 것인가?
	//instance variable
	CTextureData		*m_ptdTexture;			//실제 로딩된 texture
	static CTerrain		*m_pTerrain;			//Effect가 그려질 Terrain, BSP는 고려없음. 
	FLOAT3D				m_vPos;
	FLOATquat3D			m_qRot;
	COLOR				m_colCurrent;
};

#endif //__CTERRAINEFFECT_H__
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)