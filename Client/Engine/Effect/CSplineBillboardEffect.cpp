//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
#include "StdH.h"

#include "CTag.h"
#include "CSplineBillboardEffect.h"
#include "CGroupTag.h"

#include <Engine/Graphics/Shader.h>
#include <Engine/Math/Projection.h>
#include <Engine/Math/Matrix12.h>
#include <Engine/Math/Geometry.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Math/Matrix12.h>
#include <Engine/Math/Functions.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Templates/Stock_CTextureData.h>

#include <d3d8.h>
#include <algorithm>
#include <Engine/Base/Memory.h>

extern BOOL RM_AreHardwareShadersAvailable(void);

static CDrawPort *s_pdp = NULL;
static CAnyProjection3D *s_pApr = NULL;

ULONG CSplineBillboardEffect::m_ulVertexProgramNoTex = 0;
ULONG CSplineBillboardEffect::m_ulPixelProgramNoTex = 0;
ULONG CSplineBillboardEffect::m_ulVertexProgramTex = 0;
ULONG CSplineBillboardEffect::m_ulPixelProgramTex = 0;

//NEW_DELETE_IMPLEMENT(CSplineBillboardEffect);

CSplineBillboardEffect::CSplineBillboardEffect()
: m_ptdTexture( NULL )
, m_colTraceBegin( 0 )
, m_colTraceEnd( 0 )
, m_fTraceTime( 0 )
, m_fTimeInterval( 0 )
, m_fTwinklePeriod( 0 )
, m_fWidth( 1 )
, m_bTwinTagMode( FALSE )
, m_fVelocity( 0 )
, m_eBlendType( PBT_ADDALPHA )
{
	CEffect::m_eType = ET_SPLINEBILLBOARD;
}

CSplineBillboardEffect::~CSplineBillboardEffect()
{
	if(m_ptdTexture) _pTextureStock->Release(m_ptdTexture);
}

BOOL CSplineBillboardEffect::SetTexture(const CTFileName &filename)
{
	ASSERT(m_ptdTexture == NULL);
	if(filename == m_strTextureFileName) return FALSE;
	m_strTextureFileName = filename;

	//텍스쳐를 생성하고 로드한다.
	try
	{
		m_ptdTexture = _pTextureStock->Obtain_t(m_strTextureFileName);
	}
	catch(char *)
	{
		m_ptdTexture = NULL;
	}
	return TRUE;
}

CEffect *CSplineBillboardEffect::Copy()
{
	CSplineBillboardEffect *pRet = new CSplineBillboardEffect;
	if(pRet == NULL) return NULL;
	//CEffect의 content
	pRet->SetContent(this);
	//CSplineBillboardEffect의 content
	pRet->m_colTraceBegin = m_colTraceBegin;
	pRet->m_colTraceEnd = m_colTraceEnd;
	pRet->m_fTraceTime = m_fTraceTime;
	pRet->m_fTimeInterval = m_fTimeInterval;
	pRet->m_fTwinklePeriod = m_fTwinklePeriod;
	pRet->m_fWidth = m_fWidth;
	pRet->SetTexture(m_strTextureFileName);
	pRet->m_fVelocity = m_fVelocity;
	pRet->m_eBlendType = m_eBlendType;
	return pRet;
}

void CSplineBillboardEffect::Start(FLOAT time, BOOL restart)
{
	if(!PreStart(time, restart)) return;
	if(m_eState != ES_NOT_STARTED) return;

	//reserve max spline points
	INDEX iTraceCount = (INDEX)ceilf(m_fTraceTime / m_fTimeInterval) + 1;
	if(iTraceCount < 2) iTraceCount = 2;
	m_ssSplinePoint = ss_float3d();

	m_vectorIndex.reserve( (iTraceCount - 1) * 6 );
	for(UWORD i=0; i<iTraceCount-1; ++i)
	{
		UWORD idx = i * 2;
		//백페이스 컬링 없음.
		m_vectorIndex.push_back( idx+0 );
		m_vectorIndex.push_back( idx+1 );
		m_vectorIndex.push_back( idx+2 );
		m_vectorIndex.push_back( idx+2 );
		m_vectorIndex.push_back( idx+1 );
		m_vectorIndex.push_back( idx+3 );
	}
	m_colTraceBeginInstance.Set( m_colTraceBegin );
	m_colTraceEndInstance.Set( m_colTraceEnd );

	m_ptrTag = m_ptrAttachTag;
	if(m_ptrAttachTag->GetType() == TT_GROUP)
	{
		const CGroupTag::my_vector &vec = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTags();
		m_bTwinTagMode = (2 == vec.size());
		if(m_bTwinTagMode)
		{
			m_ptrTag = vec[0];
			m_ptrTag2 = vec[1];
			m_ssSplinePoint2 = ss_float3d();
		}
	}

	PostStart();
}

template<class T> inline void VectorPopFront(std::vector<T> &vec, INDEX cnt)
{
 	ASSERT(cnt > 0 && vec.size() > cnt);
	std::vector<T> vecRet;
	vecRet.reserve(vec.capacity());
	vecRet.assign( vec.begin() + cnt, vec.end() );
	vec = vecRet;
}

BOOL CSplineBillboardEffect::Process(FLOAT time)
{
	FLOAT fDeltaTime, fProcessedTime;
	BOOL bRet, bRender;
	if(!PreProcess(time, bRet, bRender, fDeltaTime, fProcessedTime))
	{
		if(!bRender) SetNotRenderAtThisFrame();
		return bRet;
	}

	m_ubFadeVal = UBYTE(255 * GetFadeValue(fProcessedTime));
	if(m_fLeftTime < m_fFadeOutTime)
	{
		m_ssSplinePoint.RemoveSample(0, time - m_fTraceTime * m_fLeftTime / m_fFadeOutTime);
		if(m_bTwinTagMode) m_ssSplinePoint2.RemoveSample(0, time - m_fTraceTime * m_fLeftTime / m_fFadeOutTime);
	}
	else
	{
		m_ssSplinePoint.RemoveSample(0, time - m_fTraceTime);
		if(m_bTwinTagMode) m_ssSplinePoint2.RemoveSample(0, time - m_fTraceTime);
	}

	m_ssSplinePoint.AddSample(time, m_ptrTag->CurrentTagInfo().m_vPos);
	if(m_bTwinTagMode)
	{
		m_ssSplinePoint2.AddSample(time, m_ptrTag2->CurrentTagInfo().m_vPos);
	}

	PostProcess();
	return TRUE;
}

#define FLOAT_SAME(f1, f2)	( fabs((f1)-(f2)) < (0.000001f) ? true : false )

inline void FLOAT3DToGFXVertex(GFXVertex &vtx, const FLOAT3D &v)
{
	vtx.x = v.vector[0];
	vtx.y = v.vector[1];
	vtx.z = v.vector[2];
}
inline GFXVertex FLOAT3DToGFXVertex(const FLOAT3D &v)
{
	GFXVertex vtx;
	vtx.x = v.vector[0];
	vtx.y = v.vector[1];
	vtx.z = v.vector[2];
	return vtx;
}
inline GFXColor LerpColor256(const GFXColor &col0, const GFXColor &col1, UBYTE lerpRatio, UBYTE fadeVal)
{
	GFXColor ret;
	ret.r = (col0.r * (256 - lerpRatio) + col1.r * lerpRatio) >> 8;
	ret.g = (col0.g * (256 - lerpRatio) + col1.g * lerpRatio) >> 8;
	ret.b = (col0.b * (256 - lerpRatio) + col1.b * lerpRatio) >> 8;
	ret.a = ((col0.a * (256 - lerpRatio) + col1.a * lerpRatio) * fadeVal) >> 16;
	return ret;
}

void CSplineBillboardEffect::Render()
{
	if(!PreRender())
	{
		return;
	}
	if(s_pApr == NULL) return;
	if(m_ssSplinePoint.GetSampleCount() < 2) return;
	
	if (GetOwner() != NULL)
	{ // HIDDEN 속성의 NPC의 이펙트를 보기 위해서는 캐릭터가 ENF_SHOWHIDDEN을 가지고 있어야 한다.
		if (GetOwner()->IsFlagOn(ENF_HIDDEN) && (CEntity::GetPlayerEntity(0)->IsFlagOff(ENF_SHOWHIDDEN) ||
			(CEntity::GetPlayerEntity(0)->IsFlagOn(ENF_SHOWHIDDEN)&&!GetOwner()->IsEnemy())))//ENF_SHOWHIDDEN이면 npc effect는 볼 수 있다.
			return;
	}

	m_ssSplinePoint.Prepare();
	if(m_bTwinTagMode) m_ssSplinePoint2.Prepare();
	
	INDEX cnt = INDEX( (m_ssSplinePoint.GetKey( m_ssSplinePoint.GetSampleCount() - 1 )
				- m_ssSplinePoint.GetKey(0)) / m_fTimeInterval ) + 2;
	if(cnt < 2) return;
	std::vector<GFXVertex> vectorPos;
	std::vector<GFXColor> vectorColor;
	std::vector<GFXTexCoord> vectorTexCoord;

	FLOAT3D vViewNear = -(*s_pApr)->pr_mDirectionRotation.GetRow(3);
	
	FLOAT keyTime = m_ssSplinePoint.GetKey( m_ssSplinePoint.GetSampleCount() - 1 ) - cnt * m_fTimeInterval;
	FLOAT ratio = 0.0f;
	FLOAT3D posOld = m_ssSplinePoint.GetValue(0);

	FLOAT ratioUnit = 1.0f / FLOAT( (0 == (cnt - 2)) + (cnt - 2) * (0 != (cnt - 2)) );

	if(!m_bTwinTagMode)
	{
		--cnt;
		if(cnt < 2) return;
		vectorPos.resize(cnt*2);
		vectorColor.resize(cnt*2);
		vectorTexCoord.resize(cnt*2);

		keyTime += m_fTimeInterval;
		posOld = m_ssSplinePoint.Value(keyTime);

		for(INDEX i=0; i<cnt*2; i+=2)
		{
			//position
			keyTime += m_fTimeInterval;

			FLOAT3D pos = m_ssSplinePoint.Value(keyTime);
			//if(pos(1) == 0 && pos(2) ==0 && pos(3) == 0) continue;
			FLOATplane3D plane( (pos - posOld), 0);
			FLOAT3D vFront = plane.ProjectPoint((*s_pApr)->pr_vViewerPosition - pos);//.SafeNormalize();
			FLOAT3D vSide = (vFront * plane).SafeNormalize();
			FLOAT3D temp = posOld + vSide * (m_fWidth * 0.5f);
			if(temp(1) < -FLT_MAX * 0.5f || temp(1) > FLT_MAX * 0.5f) return;
			if(temp(2) < -FLT_MAX * 0.5f || temp(2) > FLT_MAX * 0.5f) return;
			if(temp(3) < -FLT_MAX * 0.5f || temp(3) > FLT_MAX * 0.5f) return;
			FLOAT3DToGFXVertex(vectorPos[i], posOld + vSide * (m_fWidth * 0.5f));
			FLOAT3DToGFXVertex(vectorPos[i+1], posOld + vSide * (m_fWidth * -0.5f));
			posOld = pos;

			//color
			//COLOR col = LerpColorInline(m_colTraceEnd, m_colTraceBegin, ratio);
			vectorColor[i] = vectorColor[i+1]
				= LerpColor256(m_colTraceEndInstance, m_colTraceBeginInstance
								, UBYTE(ratio * 255), m_ubFadeVal);

			//tex coord
			vectorTexCoord[i].s = vectorTexCoord[i+1].s = 1.0f - ratio;
			vectorTexCoord[i].t = 0.0f;
			vectorTexCoord[i+1].t = 1.0f;
			if(i > 0) ratio += ratioUnit;//ratio += ratioUnit * (i != 0);
			if(ratio > 1.0f) ratio = 1.0f;//ratio = (ratio <= 1.0f) * ratio + (ratio > 1.0f);
		}
	}
	else
	{
		vectorPos.resize(cnt*2);
		vectorColor.resize(cnt*2);
		vectorTexCoord.resize(cnt*2);

		for(INDEX i=0; i<cnt*2; i+=2)
		{
			//position
			keyTime += m_fTimeInterval;

			if(m_fVelocity != 0)
			{
				FLOAT3D v1 = m_ssSplinePoint.Value(keyTime);
				FLOAT3D v2 = m_ssSplinePoint2.Value(keyTime);
				FLOAT3D vDir = (v1 - v2);//.SafeNormalize();
				FLOAT3DToGFXVertex(vectorPos[i], v1 + vDir * m_fVelocity * (m_ssSplinePoint.GetKey( m_ssSplinePoint.GetSampleCount()-1 ) - keyTime));
				FLOAT3DToGFXVertex(vectorPos[i+1], v2 + vDir * m_fVelocity * (m_ssSplinePoint2.GetKey( m_ssSplinePoint.GetSampleCount()-1 ) - keyTime));
			}
			else
			{
				FLOAT3DToGFXVertex(vectorPos[i], m_ssSplinePoint.Value(keyTime));
				FLOAT3DToGFXVertex(vectorPos[i+1], m_ssSplinePoint2.Value(keyTime));
			}

			//color
			vectorColor[i] =  vectorColor[i+1]
				= LerpColor256(m_colTraceEndInstance, m_colTraceBeginInstance
								, UBYTE(ratio * 255), m_ubFadeVal);

			//tex coord
			vectorTexCoord[i].s = vectorTexCoord[i+1].s = 1.0f - ratio;
			vectorTexCoord[i].t = 0.0f;
			vectorTexCoord[i+1].t = 1.0f;
			if(i > 0) ratio += ratioUnit;//ratio += ratioUnit * (i != 0);
			if(ratio > 1.0f) ratio = 1.0f;//ratio = (ratio <= 1.0f) * ratio + (ratio > 1.0f);
		}
	}

	gfxSetBlendType(m_eBlendType);
#ifdef _DEBUG
	static bool bDebugValue = FALSE;
	if(bDebugValue) gfxPolygonMode(GFX_LINE);
#endif //_DEBUG
	//렌더링 세팅 및 렌더링
	if(m_ptdTexture != NULL)
	{
		gfxSetTextureUnit(0);
		m_ptdTexture->SetAsCurrent();
		if(RM_AreHardwareShadersAvailable())
		{
			gfxSetVertexProgram( m_ulVertexProgramTex );
			gfxSetPixelProgram( m_ulPixelProgramTex );
		}
		else
		{
			gfxSetTextureModulation(1);
		}
	}
	else
	{
		if(RM_AreHardwareShadersAvailable())
		{
			gfxSetVertexProgram( m_ulVertexProgramNoTex );
			gfxSetPixelProgram( m_ulPixelProgramNoTex );
		}
		else
		{
			//gfxSetTextureSelectArg(0, GFX_TA_DIFFUSE, GFX_TA_DIFFUSE);
		}
	}
	gfxSetVertexArray( &vectorPos[0], vectorPos.size());
	gfxSetColorArray( &vectorColor[0] );
	if(m_ptdTexture != NULL) gfxSetTexCoordArray( &vectorTexCoord[0], FALSE );
	gfxDrawElements( (vectorPos.size()-2)*3, &m_vectorIndex[0]);
	
	if(!RM_AreHardwareShadersAvailable())
	{
		//gfxSetTextureSelectArg(0, GFX_TA_TEXTURE, GFX_TA_TEXTURE);
		gfxSetTextureModulation(1);
	}
	PostRender();
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 3

void CSplineBillboardEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CEffect::Read(pIS);

	is.ExpectID_t("EFSB");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_fWidth;
		is >> m_strTextureFileName;
		is >> m_colTraceBegin;
		is >> m_colTraceEnd;
		is >> m_fTraceTime;
		is >> m_fTimeInterval;
		is >> m_fTwinklePeriod;
		is >> m_fVelocity;
		is >> (INDEX&)m_eBlendType;
	}
	else if(ubVer == 2)
	{
		is >> m_fWidth;
		is >> m_strTextureFileName;
		is >> m_colTraceBegin;
		is >> m_colTraceEnd;
		is >> m_fTraceTime;
		is >> m_fTimeInterval;
		is >> m_fTwinklePeriod;
		is >> m_fVelocity;
	}
	else if(ubVer == 1)
	{
		is >> m_fWidth;
		is >> m_strTextureFileName;
		is >> m_colTraceBegin;
		is >> m_colTraceEnd;
		is >> m_fTraceTime;
		is >> m_fTimeInterval;
		is >> m_fTwinklePeriod;
		m_fVelocity = 0;
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CSplineBillboardEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CEffect::Write(pOS);

	os.WriteID_t("EFSB");
	os << (UBYTE)CURRENT_VERSION;

	os << m_fWidth;
	os << m_strTextureFileName;
//	CPrintF("%s\n", m_strTextureFileName);	//log
	os << m_colTraceBegin;
	os << m_colTraceEnd;
	os << m_fTraceTime;
	os << m_fTimeInterval;
	os << m_fTwinklePeriod;
	os << m_fVelocity;
	os << (INDEX)m_eBlendType;
}

BOOL CSplineBillboardEffect::BeginRender(CAnyProjection3D &apr, CDrawPort *pdp)
{
	//프로젝션 설정
	s_pApr = &apr;
	apr->ObjectPlacementL() = CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0));
	apr->Prepare();
	// in case of mirror projection, move mirror clip plane a bit father from the mirrored models,
	// so we have less clipping (for instance, player feet)
	if( apr->pr_bMirror) apr->pr_plMirrorView.pl_distance -= 0.06f; // -0.06 is because entire projection is offseted by +0.05
	else if(apr->pr_bNiceWater) apr->pr_plNiceWaterView.pl_distance -= 0.06f;
	pdp->SetProjection( apr );
	s_pdp = pdp;
	Matrix12 mAbsToViewer;
	MatrixVectorToMatrix12(mAbsToViewer,
		apr->pr_ViewerRotationMatrix, 
		-apr->pr_vViewerPosition*apr->pr_ViewerRotationMatrix);

	//렌더링 상태 설정
	Matrix12 &mView12 = mAbsToViewer;
	FLOAT mView16[16];
	mView16[ 0] = mView12[ 0];  mView16[ 1] = mView12[ 4];  mView16[ 2] = mView12[ 8];  mView16[ 3] = 0;
	mView16[ 4] = mView12[ 1];  mView16[ 5] = mView12[ 5];  mView16[ 6] = mView12[ 9];  mView16[ 7] = 0;
	mView16[ 8] = mView12[ 2];  mView16[ 9] = mView12[ 6];  mView16[10] = mView12[10];  mView16[11] = 0;
	mView16[12] = mView12[ 3];  mView16[13] = mView12[ 7];  mView16[14] = mView12[11];  mView16[15] = 1;
	gfxSetViewMatrix(mView16);
	gfxSetTextureWrapping(GFX_CLAMP, GFX_CLAMP);
	gfxCullFace(GFX_NONE);
	if(RM_AreHardwareShadersAvailable())
	{
		gfxEnableVertexProgram();
		gfxEnablePixelProgram();

		shaSetVertexProgramConst_TransformMatrix();	//transform matrix setting
	}
	else
	{
		gfxDisableVertexProgram();
		gfxDisablePixelProgram();
	}
	gfxEnableColorArray();
	gfxEnableDepthTest();
	return TRUE;
}

void CSplineBillboardEffect::EndRender(BOOL bRestoreOrtho)
{
	//렌더링 상태 설정
	gfxDisableVertexProgram();
	gfxDisablePixelProgram();
	gfxDisableBlend();
	gfxEnableDepthWrite();
	gfxDisableColorArray();

	// re-enable color mask (maybe occ test left it disabled)
	gfxSetColorMask(CT_RMASK|CT_GMASK|CT_BMASK|CT_AMASK);
	// back to 2D projection?
	if( bRestoreOrtho ) s_pdp->SetOrtho();
}

void CSplineBillboardEffect::InitializeShaders()
{
	//Rednering에 사용할 Shader를 생성한다.
	static char *strVPNoTex =	"vs.1.1"				"\n"
								"m4x4 oPos, v0, c0"		"\n"
								"mov oD0, v4"			"\n";
	m_ulVertexProgramNoTex = gfxCreateVertexProgram(strVPNoTex, GFX_POSITION_STREAM | GFX_COLOR_STREAM);
	static char *strPPNoTex =	"ps.1.4"				"\n"
								"mov r0, v0"			"\n";
	m_ulPixelProgramNoTex = gfxCreatePixelProgram(strPPNoTex);
	static char *strVPTex =		"vs.1.1"				"\n"
								"m4x4 oPos, v0, c0"		"\n"
								"mov oT0, v5"			"\n"
								"mov oD0, v4"			"\n";
	m_ulVertexProgramTex = gfxCreateVertexProgram(strVPTex, GFX_POSITION_STREAM | GFX_COLOR_STREAM | GFX_TEXCOORD0);
	static char *strPPTex =		"ps.1.4"				"\n"
								//"tex t0"				"\n"
								"texld r0, t0"			"\n"
								//"mul r0, v0, t0"		"\n";
								"mul r0, r0, v0"		"\n";
	m_ulPixelProgramTex = gfxCreatePixelProgram(strPPTex);
}

void CSplineBillboardEffect::FinalizeShaders()
{
	//Rednering에 사용한 Shader를 지운다.
	if(m_ulVertexProgramNoTex) gfxDeleteVertexProgram(m_ulVertexProgramNoTex);
	m_ulVertexProgramNoTex = 0;
	if(m_ulPixelProgramNoTex) gfxDeletePixelProgram(m_ulPixelProgramNoTex);
	m_ulPixelProgramNoTex = 0;
	if(m_ulVertexProgramTex) gfxDeleteVertexProgram(m_ulVertexProgramTex);
	m_ulVertexProgramTex = 0;
	if(m_ulPixelProgramTex) gfxDeletePixelProgram(m_ulPixelProgramTex);
	m_ulPixelProgramTex = 0;
}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
