//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#ifndef __CLIGHTEFFECT_H__
#define __CLIGHTEFFECT_H__

#pragma warning(disable :4786)

#include <Engine/Base/Memory.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Graphics/Texture.h>
#include "CEffect.h"

class CTag;
class CAnyProjection3D;
class CDrawPort;
class CLightSource;
class CWorld;


class ENGINE_API CLightEffect : public CEffect
{
public:
	CLightEffect();
	virtual ~CLightEffect();

	//NEW_DELETE_DEFINITION(CLightEffect);
	
//안태훈 수정 시작	//(Remake Effect)(0.1)
	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);
//안태훈 수정 끝	//(Remake Effect)(0.1)
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Render();

	virtual BOOL BeginRender(CAnyProjection3D &apr, CDrawPort *pdp);
	virtual void EndRender(BOOL bRestoreOrtho);

	//acess function
	inline void SetAmbientMode(BOOL am)			{ m_bAmbientMode = am;		}
	inline BOOL GetAmbientMode()				{ return m_bAmbientMode;	}
	inline void SetMaxFallOff(FLOAT falloff)	{ m_fMaxFallOff = falloff;	}
	inline FLOAT GetMaxFallOff()				{ return m_fMaxFallOff;		}

	inline void SetLightColor(COLOR color)		{ m_colLight = color;		}
	inline COLOR GetLightColor()				{ return m_colLight;		}
	
	static inline void SetWorld(CWorld *pWorld)						{ m_pWorld = pWorld;			}
	static inline void SetTerrain(CTerrain *pTerrain)				{ m_pTerrain = pTerrain;		}
	static inline void SetLightTexture(CTextureData *ptdLightMap)	{ m_ptdLightMap = ptdLightMap;	}

protected:
	void SetupLightSource( CLightSource &lsNew);

protected:
	//content variable
	FLOAT				m_fMaxFallOff;	//빛의 범위
	COLOR				m_colLight;		//빛의 색깔, 현재는 Terrain에서는 무시됨. BSP에서는 먹는듯.
	BOOL				m_bAmbientMode;
	//instance variable
	CEntity				*m_penDummy;	//CLightSource가 위치값을 안갖고 있어서 CEntity가 필요하다.
	CLightSource		*m_plsLight;	//설정된 CLightSource로 Point라이트임.
	static CWorld		*m_pWorld;		//CEntity를 생성하는데 필요하다. Start에서만 쓰임.
	static CTerrain		*m_pTerrain;	//Rendering 하는데 필요하다. EndRender에서만 쓰임.
	static CTextureData	*m_ptdLightMap;	//CEntity를 생성하는데 필요하다. EndRender에서만 쓰임.
};

#endif //__CLIGHTEFFECT_H__
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)