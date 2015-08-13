//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
#ifndef __CSPLINEEFFECT_H__
#define __CSPLINEEFFECT_H__

#include <Engine/Base/Memory.h>
#include <Engine/Base/Types.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/Gfx_wrapper.h>
#pragma warning(disable :4786)
#include <list>
#include <Engine/Base/FileName.h>
#include <Engine/Graphics/Vertex.h>
#include <vector>

#include "CEffect.h"
#include "CRefCountPtr.h"
#include "CSampleSpline.h"

class CTag;

class ENGINE_API CSplineBillboardEffect : public CEffect
{
public:
	typedef CSampleSpline< FLOAT3D, CCubicSplineInterpolate<FLOAT3D> >		ss_float3d;
	typedef std::vector< UWORD >		vector_index;
public:
	CSplineBillboardEffect();
	virtual ~CSplineBillboardEffect();

	//NEW_DELETE_DEFINITION(CSplineBillboardEffect);

	virtual void Read(CTStream *istrFile);
	virtual void Write(CTStream *ostrFile);
	
	virtual void Start(FLOAT time, BOOL restart = FALSE);
	virtual BOOL Process(FLOAT time);
	virtual CEffect *Copy();
	virtual void Render();

	BOOL SetTexture(const CTFileName &filename);
	const CTFileName &GetTexFileName()	{ return m_strTextureFileName;	}

	//acess function
	inline void SetTraceTime(FLOAT time)	{ m_fTraceTime = time; }
	inline FLOAT GetTraceTime()				{ return m_fTraceTime; }

	inline void SetTimeInterval(FLOAT time)	{ m_fTimeInterval = time; }
	inline FLOAT GetTimeInterval()			{ return m_fTimeInterval; }

	inline void SetTwinklePeriod(FLOAT twinklePeriod)	{ m_fTwinklePeriod = twinklePeriod;	}
	inline FLOAT GetTwinklePeriod()						{ return m_fTwinklePeriod;			}

	inline void SetColor(COLOR colBegin, COLOR colEnd)	{ m_colTraceBegin = colBegin; m_colTraceEnd = colEnd; }
	inline COLOR GetColorBegin()						{ return m_colTraceBegin;	}
	inline COLOR GetColorEnd()							{ return m_colTraceEnd;		}

	inline void SetWidth(FLOAT width)					{ m_fWidth = width;			}
	inline FLOAT GetWidth()								{ return m_fWidth;			}

	inline void SetVelocity(FLOAT vel)					{ m_fVelocity = vel;		}
	inline FLOAT GetVelocity()							{ return m_fVelocity;		}

	inline void SetBlendType(PredefinedBlendType blendType)	{ m_eBlendType = blendType; }
	inline PredefinedBlendType GetBlendType()				{ return m_eBlendType;		}

	virtual BOOL BeginRender(CAnyProjection3D &apr, CDrawPort *pdp);
	virtual void EndRender(BOOL bRestoreOrtho);

	static void InitializeShaders();
	static void FinalizeShaders();

protected:
	CSplineBillboardEffect( CSplineBillboardEffect &other ) {}
	CSplineBillboardEffect &operator =(CSplineBillboardEffect &other) {return *this;}

protected:
	//content variablen
	CTFileName			m_strTextureFileName;	//texture의 파일 이름
	COLOR				m_colTraceBegin;		//trace가 시작되는 부분의 색깔
	COLOR				m_colTraceEnd;			//trace가 끝나는 부분의 색깔
	FLOAT				m_fTraceTime;			//trace되는 시간
	FLOAT				m_fTimeInterval;		//trace되는 간격, 시작부분은 항상 됨.
	FLOAT				m_fTwinklePeriod;		//전체의 알파를 일정간격으로 조정.
	FLOAT				m_fWidth;				//두께.
	FLOAT				m_fVelocity;			//속도.
	PredefinedBlendType	m_eBlendType;
	//instance variable
	static ULONG		m_ulVertexProgramNoTex;
	static ULONG		m_ulPixelProgramNoTex;
	static ULONG		m_ulVertexProgramTex;
	static ULONG		m_ulPixelProgramTex;
	CTextureData		*m_ptdTexture;			//실제 로딩된 texture
	FLOAT				m_fTwinkleValue;		//반짝반짝값, 매프레임마다 계산됨.
	ss_float3d			m_ssSplinePoint;
	vector_index		m_vectorIndex;
	GFXColor			m_colTraceBeginInstance;
	GFXColor			m_colTraceEndInstance;
	UBYTE				m_ubFadeVal;
	BOOL				m_bTwinTagMode;
	ptr_tag				m_ptrTag;
	ptr_tag				m_ptrTag2;
	ss_float3d			m_ssSplinePoint2;
};

#endif //__CSPLINEEFFECT_H__
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
