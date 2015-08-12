#ifndef __CSHOCKWAVEEFFECT_H__
#define __CSHOCKWAVEEFFECT_H__

#include "CEffect.h"
#include "CSampleSpline.h"
#include <vector>
#include <Engine/Graphics/Vertex.h>
#include <Engine/Base/FileName.h>
#include <Engine/Base/Types.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/Gfx_wrapper.h>

class ENGINE_API CShockWaveEffect : public CEffect
{
public:
public:
	CShockWaveEffect();
	virtual ~CShockWaveEffect();

	//NEW_DELETE_DEFINITION(CShockWaveEffect);

	virtual void Read(CTStream *pIS);
	virtual void Write(CTStream *pOS);
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Render();

	void SetInnerBasis(BOOL inner)	{ m_bInnerBasis = inner;	}
	BOOL GetInnerBasis()			{ return m_bInnerBasis;		}
	
	void SetSplitCount(INDEX cnt)	{ m_iSplitCount = cnt;		}
	INDEX GetSplitCount()			{ return m_iSplitCount;		}

	inline UINT AddRadius(FLOAT time, FLOAT radius)	{ return m_ssRadius.AddSample(time, radius);	}
	inline UINT RemoveRadius(SINT index)			{ return m_ssRadius.RemoveSample(index);		}
	inline FLOAT GetRadiusValue(UINT index)			{ return m_ssRadius.GetValue(index);			}
	inline FLOAT GetRadiusTime(UINT index)			{ return m_ssRadius.GetKey(index);				}
	inline UINT GetRadiusCount()					{ return m_ssRadius.GetSampleCount();			}

	inline UINT AddWidth(FLOAT time, FLOAT width)	{ return m_ssWidth.AddSample(time, width);		}
	inline UINT RemoveWidth(SINT index)				{ return m_ssWidth.RemoveSample(index);			}
	inline FLOAT GetWidthValue(UINT index)			{ return m_ssWidth.GetValue(index);				}
	inline FLOAT GetWidthTime(UINT index)			{ return m_ssWidth.GetKey(index);				}
	inline UINT GetWidthCount()						{ return m_ssWidth.GetSampleCount();			}

	inline UINT AddHeight(FLOAT time, FLOAT height)	{ return m_ssHeight.AddSample(time, height);	}
	inline UINT RemoveHeight(SINT index)			{ return m_ssHeight.RemoveSample(index);		}
	inline FLOAT GetHeightValue(UINT index)			{ return m_ssHeight.GetValue(index);			}
	inline FLOAT GetHeightTime(UINT index)			{ return m_ssHeight.GetKey(index);				}
	inline UINT GetHeightCount()					{ return m_ssHeight.GetSampleCount();			}

	inline void SetRotationType(EFFECT_OF_TAG_TYPE eott)	{ m_eRotation = eott;	}
	inline EFFECT_OF_TAG_TYPE GetRotationType()				{ return m_eRotation;	}

	inline void SetPositionType(EFFECT_OF_TAG_TYPE eott)	{ m_ePosition = eott;	}
	inline EFFECT_OF_TAG_TYPE GetPositionType()				{ return m_ePosition;	}

	BOOL SetTexture(const CTFileName &filename);
	const CTFileName &GetTexFileName()	{ return m_strTextureFileName;	}

	inline void SetBlendType(PredefinedBlendType blendType)	{ m_eBlendType = blendType; }
	inline PredefinedBlendType GetBlendType()				{ return m_eBlendType;		}

	static void InitializeShaders();
	static void FinalizeShaders();

	virtual BOOL BeginRender(CAnyProjection3D &apr, CDrawPort *pdp);
	virtual void EndRender(BOOL bRestoreOrtho);
protected:
	CShockWaveEffect(const CShockWaveEffect& other) {}
	CShockWaveEffect &operator =(const CShockWaveEffect &other) { return *this; }

	void ClearInstance();
protected:
	//contents
	CTFileName				m_strTextureFileName;	//texture의 파일 이름
	BOOL					m_bInnerBasis;
	INDEX					m_iSplitCount;
	CSampleSpline<FLOAT>	m_ssRadius;
	CSampleSpline<FLOAT>	m_ssWidth;
	CSampleSpline<FLOAT>	m_ssHeight;
	EFFECT_OF_TAG_TYPE		m_eRotation;			//tag의 회전에 얼만큼 영향을 받을 것인가?
	EFFECT_OF_TAG_TYPE		m_ePosition;			//tag의 이동에 얼만큼 영향을 받을 것인가?
	PredefinedBlendType		m_eBlendType;
	//instance
	static ULONG				m_ulVertexProgramNoTex;
	static ULONG				m_ulPixelProgramNoTex;
	static ULONG				m_ulVertexProgramTex;
	static ULONG				m_ulPixelProgramTex;
	CTextureData				*m_ptdTexture;			//실제 로딩된 texture
	std::vector<FLOAT3D>		m_vectorMoveVector;
	std::vector<GFXTexCoord>	m_vectorTexCoord;
	std::vector<WORD>			m_vectorIndex;
	std::vector<GFXVertex>		m_vectorGFXVertex;
	std::vector<GFXColor>		m_vectorGFXColor;
	BOOL						m_bColorWhite;
	FLOAT3D						m_vTagPos;
	FLOATquat3D					m_qTagRot;
};

#endif //__CSHOCKWAVEEFFECT_H__
