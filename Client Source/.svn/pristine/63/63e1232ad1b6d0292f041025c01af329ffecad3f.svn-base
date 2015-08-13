//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include "StdH.h"

#include "CTag.h"
#include "CTraceEffect.h"
#include "CGroupTag.h"

#include <Engine/Graphics/Shader.h>
#include <Engine/Math/Projection.h>
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

ULONG CTraceEffect::m_ulVertexProgramNoTex = 0;
ULONG CTraceEffect::m_ulPixelProgramNoTex = 0;
ULONG CTraceEffect::m_ulVertexProgramTex = 0;
ULONG CTraceEffect::m_ulPixelProgramTex = 0;

//NEW_DELETE_IMPLEMENT(CTraceEffect);

CTraceEffect::CTraceEffect()
: m_iTraceCount( 0 )
, m_ptdTexture( NULL )
, m_colTraceBegin( 0 )
, m_colTraceEnd( 0 )
, m_fTraceTime( 0 )
, m_fTimeInterval( 0 )
, m_fTwinklePeriod( 0 )
, m_eBlendType( PBT_ADDALPHA )
{
	CEffect::m_eType = ET_TRACE;
}

CTraceEffect::~CTraceEffect()
{
	if(m_ptdTexture) _pTextureStock->Release(m_ptdTexture);
}

BOOL CTraceEffect::SetTexture(const CTFileName &filename)
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

CEffect *CTraceEffect::Copy()
{
	CTraceEffect *pRet = new CTraceEffect;
	if(pRet == NULL) return NULL;
	//CEffect의 content
	pRet->SetContent(this);
	//CTraceEffect의 content
	pRet->m_colTraceBegin = m_colTraceBegin;
	pRet->m_colTraceEnd = m_colTraceEnd;
	pRet->m_fTraceTime = m_fTraceTime;
	pRet->m_fTimeInterval = m_fTimeInterval;
	pRet->m_fTwinklePeriod = m_fTwinklePeriod;
	pRet->m_bCapEnd = m_bCapEnd;
	pRet->SetTexture(m_strTextureFileName);
	pRet->m_eBlendType = m_eBlendType;
	return pRet;
}

void CTraceEffect::Start(FLOAT time, BOOL restart)
{
	if(!PreStart(time, restart)) return;
	if(m_ptrAttachTag->GetType() != TT_GROUP)
		m_eState = ES_PLAY_END;
	m_iTagCount = ((CGroupTag*)m_ptrAttachTag.GetNative())->Count();
	if(m_iTagCount != 2 && m_iTagCount != 3) m_eState = ES_PLAY_END;
	if(m_eState != ES_NOT_STARTED) return;

	if(m_iTagCount == 2)
	{
		//필요한 것들 생성.인덱스는 고정
		m_iTraceCount = (INDEX)ceilf(m_fTraceTime / m_fTimeInterval);
		m_vectorIndex.reserve((m_iTraceCount+2)*6);
		for(UWORD i=0; i<m_iTraceCount+2; ++i)
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
		//메쉬 초기생성
		m_vectorPos.reserve((m_iTraceCount+2)*2);
		FLOAT3D pos1 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(0)->CurrentTagInfo().m_vPos;
		GFXVertex v1 = { pos1(1), pos1(2), pos1(3) };
		m_vectorPos.push_back(v1);
		FLOAT3D pos2 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(1)->CurrentTagInfo().m_vPos;
		GFXVertex v2 = { pos2(1), pos2(2), pos2(3) };
		m_vectorPos.push_back(v2);

		m_vectorColor.reserve((m_iTraceCount+2)*2);
		m_vectorTexCoord.reserve((m_iTraceCount+2)*2);
	}
	else if(m_iTagCount == 3)
	{
		//필요한 것들 생성.인덱스는 고정
		m_iTraceCount = (INDEX)ceilf(m_fTraceTime / m_fTimeInterval);
		m_vectorIndex.reserve((m_iTraceCount+9)*6*3);
		for(UWORD i=0; i<m_iTraceCount+9; ++i)
		{
			UWORD idx = i * 3;
			//백페이스 컬링 없음.
			m_vectorIndex.push_back( idx+0 );
			m_vectorIndex.push_back( idx+1 );
			m_vectorIndex.push_back( idx+3 );
			m_vectorIndex.push_back( idx+1 );
			m_vectorIndex.push_back( idx+4 );
			m_vectorIndex.push_back( idx+3 );

			m_vectorIndex.push_back( idx+0 );
			m_vectorIndex.push_back( idx+5 );
			m_vectorIndex.push_back( idx+2 );
			m_vectorIndex.push_back( idx+0 );
			m_vectorIndex.push_back( idx+3 );
			m_vectorIndex.push_back( idx+5 );

			m_vectorIndex.push_back( idx+1 );
			m_vectorIndex.push_back( idx+2 );
			m_vectorIndex.push_back( idx+5 );
			m_vectorIndex.push_back( idx+1 );
			m_vectorIndex.push_back( idx+5 );
			m_vectorIndex.push_back( idx+4 );
		}
		//메쉬 초기생성
		m_vectorPos.reserve((m_iTraceCount+2)*3);
		FLOAT3D pos1 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(0)->CurrentTagInfo().m_vPos;
		GFXVertex v1 = { pos1(1), pos1(2), pos1(3) };
		m_vectorPos.push_back(v1);
		FLOAT3D pos2 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(1)->CurrentTagInfo().m_vPos;
		GFXVertex v2 = { pos2(1), pos2(2), pos2(3) };
		m_vectorPos.push_back(v2);
		FLOAT3D pos3 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(2)->CurrentTagInfo().m_vPos;
		GFXVertex v3 = { pos3(1), pos3(2), pos3(3) };
		m_vectorPos.push_back(v3);

		m_vectorColor.reserve((m_iTraceCount+2)*3);
		m_vectorTexCoord.reserve((m_iTraceCount+2)*3);
	}
	m_vectorAddTime.reserve((m_iTraceCount+2)*2);
	m_vectorAddTime.push_back(time);

	PostStart();
}

template<class T> inline void VectorPopFront(std::vector<T> &vec, INDEX cnt)
{
 	ASSERT(cnt > 0 && vec.size() >= cnt);
	for(int i=0; i<cnt; ++i) vec.erase( vec.begin() );
}
BOOL CTraceEffect::Process(FLOAT time)
{
	FLOAT fDeltaTime, fProcessedTime;
	BOOL bRet, bRender;
	if(!PreProcess(time, bRet, bRender, fDeltaTime, fProcessedTime))
	{
		if(!bRender) SetNotRenderAtThisFrame();
		return bRet;
	}
	//vertex 추가, 삭제, Render함수안에 있어도 되는데 Process가 심심할까봐...-_-;
	if(fProcessedTime + m_fTraceTime > m_fLifeTime)
	{
		m_fTraceTime = m_fLifeTime - fProcessedTime;
	}
	FLOAT lastAddTime = 0;
	if(!m_vectorAddTime.empty()) lastAddTime = *m_vectorAddTime.rbegin();
	m_fTwinkleValue = 1.0f;
	if(m_fTwinklePeriod > 0.0001f)
	{
		int value = (int)floorf(fProcessedTime / m_fTwinklePeriod);
		if(value & 0x00000001) m_fTwinkleValue = ( fProcessedTime - m_fTwinklePeriod * value ) / m_fTwinklePeriod;
		else m_fTwinkleValue = ( m_fTwinklePeriod * (value + 1) - fProcessedTime ) / m_fTwinklePeriod;
	}
	
	if(m_iTagCount == 2)
	{
		if(m_fLastProcessTime - lastAddTime >= m_fTimeInterval)
		{
			FLOAT3D pos1 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(0)->CurrentTagInfo().m_vPos;
			GFXVertex v1 = { pos1(1), pos1(2), pos1(3) };
			m_vectorPos.push_back(v1);
			FLOAT3D pos2 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(1)->CurrentTagInfo().m_vPos;
			GFXVertex v2 = { pos2(1), pos2(2), pos2(3) };
			m_vectorPos.push_back(v2);
			m_vectorAddTime.push_back(m_fLastProcessTime);
			//color와 tex coord는 다시 만들어야 한다.
			RemakeColorsAndTexCoords(m_vectorPos.size());
		}
		FLOAT firstAddTime = m_vectorAddTime[0];
		if(m_fLastProcessTime - firstAddTime > m_fTraceTime)
		{
			VectorPopFront(m_vectorPos, 2);
			VectorPopFront(m_vectorAddTime, 1);
			//color와 tex coord는 다시 만들어야 한다.
			RemakeColorsAndTexCoords(m_vectorPos.size());
		}
	}
	else if(m_iTagCount == 3)
	{
		if(m_fLastProcessTime - lastAddTime >= m_fTimeInterval)
		{
			FLOAT3D pos1 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(0)->CurrentTagInfo().m_vPos;
			GFXVertex v1 = { pos1(1), pos1(2), pos1(3) };
			m_vectorPos.push_back(v1);
			FLOAT3D pos2 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(1)->CurrentTagInfo().m_vPos;
			GFXVertex v2 = { pos2(1), pos2(2), pos2(3) };
			m_vectorPos.push_back(v2);
			FLOAT3D pos3 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(2)->CurrentTagInfo().m_vPos;
			GFXVertex v3 = { pos3(1), pos3(2), pos3(3) };
			m_vectorPos.push_back(v3);
			m_vectorAddTime.push_back(m_fLastProcessTime);
			//color와 tex coord는 다시 만들어야 한다.
			RemakeColorsAndTexCoords(m_vectorPos.size());
		}
		FLOAT firstAddTime = m_vectorAddTime[0];
		if(m_fLastProcessTime - firstAddTime > m_fTraceTime)
		{
			VectorPopFront(m_vectorPos, 3);
			VectorPopFront(m_vectorAddTime, 1);
			//color와 tex coord는 다시 만들어야 한다.
			RemakeColorsAndTexCoords(m_vectorPos.size());
		}
	}

	PostProcess();
	return TRUE;
}

static CDrawPort *s_pdp = NULL;
BOOL CTraceEffect::BeginRender(CAnyProjection3D &apr, CDrawPort *pdp)
{
	//프로젝션 설정
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
		//gfxDisableVertexProgram();
		//gfxDisablePixelProgram();
	}
	//gfxSetBlendType(PBT_ADDALPHA);
	//gfxEnableColorArray();
	gfxEnableDepthTest();
	return TRUE;
}
void CTraceEffect::EndRender(BOOL bRestoreOrtho)
{
	//렌더링 상태 설정
	if(RM_AreHardwareShadersAvailable())
	{
		gfxDisableVertexProgram();
		gfxDisablePixelProgram();
	}
	gfxDisableBlend();
	gfxEnableDepthWrite();
	//gfxDisableColorArray();

	// re-enable color mask (maybe occ test left it disabled)
	//gfxSetColorMask(CT_RMASK|CT_GMASK|CT_BMASK|CT_AMASK);
	// back to 2D projection?
	if( bRestoreOrtho ) s_pdp->SetOrtho();
}

void CTraceEffect::InitializeShaders()
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
								"texld r0,	t0"			"\n"
								//"mul r0, v0, t0"		"\n";
								"mul r0, r0, v0"		"\n";
	m_ulPixelProgramTex = gfxCreatePixelProgram(strPPTex);
}

void CTraceEffect::FinalizeShaders()
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

//이 함수는 속도가 느린 편인데 호출이 여기저기서 되서 최적화 목표 중 하나이다.
void CTraceEffect::RemakeColorsAndTexCoords(INDEX cntVtx)
{
	if(m_iTagCount == 2)
	{
		ASSERT((cntVtx & 0x00000001) == 0); //짝수보장.
		INDEX cntVtxD2 = cntVtx >> 1;
		m_vectorColor.clear();
		m_vectorTexCoord.clear();
		for(INDEX i=0; i<cntVtxD2; ++i)
		{
			GFXColor currentColor( LerpColorInline(m_colTraceEnd, m_colTraceBegin, m_fTwinkleValue * (FLOAT)i / (FLOAT)(cntVtxD2-1)) );
			m_vectorColor.push_back(currentColor);
			m_vectorColor.push_back(currentColor);
			if(m_ptdTexture != NULL)
			{
				GFXTexCoord currentTexCoord;
				currentTexCoord.s = Lerp(0.0f, 1.0f, (FLOAT)i/(FLOAT)(cntVtxD2-1));
				currentTexCoord.t = 0.0f;
				m_vectorTexCoord.push_back(currentTexCoord);
				currentTexCoord.t = 1.0f;
				m_vectorTexCoord.push_back(currentTexCoord);
			}
		}
	}
	else if(m_iTagCount == 3)
	{
		INDEX cntVtxD3 = cntVtx / 3;
		m_vectorColor.clear();
		m_vectorTexCoord.clear();
		for(INDEX i=0; i<cntVtxD3; ++i)
		{
			GFXColor currentColor( LerpColorInline(m_colTraceEnd, m_colTraceBegin, m_fTwinkleValue * (FLOAT)i / (FLOAT)(cntVtxD3-1)) );
			m_vectorColor.push_back(currentColor);
			m_vectorColor.push_back(currentColor);
			m_vectorColor.push_back(currentColor);
			if(m_ptdTexture != NULL)
			{
				GFXTexCoord currentTexCoord;
				currentTexCoord.s = 0.0f;
				currentTexCoord.t = Lerp(0.0f, 1.0f, (FLOAT)i/(FLOAT)(cntVtxD3-1));
				m_vectorTexCoord.push_back(currentTexCoord);
				currentTexCoord.s = 0.333333f;
				m_vectorTexCoord.push_back(currentTexCoord);
				currentTexCoord.s = 0.666666f;
				m_vectorTexCoord.push_back(currentTexCoord);
			}
		}
	}
}

#define FLOAT_SAME(f1, f2)	( fabs((f1)-(f2)) < (0.000001f) ? true : false )

void CTraceEffect::Render()
{
	if(!PreRender())
	{
		return;
	}
	
	if (GetOwner() != NULL)
	{ // HIDDEN 속성의 NPC의 이펙트를 보기 위해서는 캐릭터가 ENF_SHOWHIDDEN을 가지고 있어야 한다.
		if (GetOwner()->IsFlagOn(ENF_HIDDEN) && (CEntity::GetPlayerEntity(0)->IsFlagOff(ENF_SHOWHIDDEN) ||
			(CEntity::GetPlayerEntity(0)->IsFlagOn(ENF_SHOWHIDDEN)&&!GetOwner()->IsEnemy())))//ENF_SHOWHIDDEN이면 npc effect는 볼 수 있다.
			return;
	}

	//렌더링 세팅 및 렌더링
	if( (m_vectorPos.size() > 2 && m_iTagCount == 2) || (m_vectorPos.size() > 3 && m_iTagCount == 3) )
	{
		//이렇게 해주는 이유는 m_fTimeInterval이 frame당 시간보다 클 경우 Tag에 딱붙지않고 떨어지는데
		//그러면 대략 보기가 좋지 않고 Trace라는 것에도 맞지 않기 때문에 항상 붙을 수 있게 처리해준다.
		BOOL bAddSpare = FALSE;
		FLOAT lastAddTime = *m_vectorAddTime.rbegin();
		if(!FLOAT_SAME(lastAddTime, m_fLastProcessTime))
		{
			if(m_iTagCount == 2)
			{
				FLOAT3D pos1 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(0)->CurrentTagInfo().m_vPos;
				GFXVertex v1 = { pos1(1), pos1(2), pos1(3) };
				m_vectorPos.push_back(v1);
				FLOAT3D pos2 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(1)->CurrentTagInfo().m_vPos;
				GFXVertex v2 = { pos2(1), pos2(2), pos2(3) };
				m_vectorPos.push_back(v2);
			}
			else if(m_iTagCount == 3)
			{
				FLOAT3D pos1 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(0)->CurrentTagInfo().m_vPos;
				GFXVertex v1 = { pos1(1), pos1(2), pos1(3) };
				m_vectorPos.push_back(v1);
				FLOAT3D pos2 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(1)->CurrentTagInfo().m_vPos;
				GFXVertex v2 = { pos2(1), pos2(2), pos2(3) };
				m_vectorPos.push_back(v2);
				FLOAT3D pos3 = ((CGroupTag*)m_ptrAttachTag.GetNative())->GetTag(2)->CurrentTagInfo().m_vPos;
				GFXVertex v3 = { pos3(1), pos3(2), pos3(3) };
				m_vectorPos.push_back(v3);
			}
			//color와 tex coord는 다시 만들어야 한다.
			RemakeColorsAndTexCoords(m_vectorPos.size());
			bAddSpare = TRUE;
		}

		gfxSetBlendType(m_eBlendType);

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

		if(m_bCapEnd)
		{
			if(m_iTagCount == 2)
			{
				GFXVertex vtx;
				vtx.x = (m_vectorPos[0].x + m_vectorPos[1].x) * 0.5f;
				vtx.y = (m_vectorPos[0].y + m_vectorPos[1].y) * 0.5f;
				vtx.z = (m_vectorPos[0].z + m_vectorPos[1].z) * 0.5f;
				m_vectorPos[0] = m_vectorPos[1] = vtx;
			}
			else if(m_iTagCount == 3)
			{
				GFXVertex vtx;
				vtx.x = (m_vectorPos[0].x + m_vectorPos[1].x + m_vectorPos[2].x) / 3.0f;
				vtx.y = (m_vectorPos[0].y + m_vectorPos[1].y + m_vectorPos[2].y) / 3.0f;
				vtx.z = (m_vectorPos[0].z + m_vectorPos[1].z + m_vectorPos[2].z) / 3.0f;
				m_vectorPos[0] = m_vectorPos[1] = m_vectorPos[2] = vtx;
			}
		}

		gfxSetVertexArray( &m_vectorPos[0], m_vectorPos.size());
		gfxSetColorArray( &m_vectorColor[0] );
		if(m_ptdTexture != NULL) gfxSetTexCoordArray( &m_vectorTexCoord[0], FALSE );
		if(m_iTagCount == 2)
		{
			gfxDrawElements( (m_vectorPos.size()-2)*3, &m_vectorIndex[0]);
		}
		else if(m_iTagCount == 3)
		{
			gfxDrawElements( (m_vectorPos.size()-3)*2*3, &m_vectorIndex[0]);
		}

		if(bAddSpare)	//여분으로 추가되었던 vertex삭제.
		{
			if(m_iTagCount == 2)
			{
				m_vectorPos.pop_back();
				m_vectorPos.pop_back();
			}
			else if(m_iTagCount == 3)
			{
				m_vectorPos.pop_back();
				m_vectorPos.pop_back();
				m_vectorPos.pop_back();
			}
		}
	}
	
	if(!RM_AreHardwareShadersAvailable())
	{
		//gfxSetTextureSelectArg(0, GFX_TA_TEXTURE, GFX_TA_TEXTURE);
		gfxSetTextureModulation(1);
	}
	PostRender();
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 4

void CTraceEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CEffect::Read(pIS);

	is.ExpectID_t("EFTC");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_strTextureFileName;
		is >> m_colTraceBegin;
		is >> m_colTraceEnd;
		is >> m_fTraceTime;
		is >> m_fTimeInterval;
		is >> m_fTwinklePeriod;
		is >> m_bCapEnd;
		is >> (INDEX&)m_eBlendType;
	}
	else if(ubVer == 3)
	{
		is >> m_strTextureFileName;
		is >> m_colTraceBegin;
		is >> m_colTraceEnd;
		is >> m_fTraceTime;
		is >> m_fTimeInterval;
		is >> m_fTwinklePeriod;
		is >> m_bCapEnd;
	}
/*
	else if(ubVer == 2)
	{
		is >> m_strTextureFileName;
		is >> m_colTraceBegin;
		is >> m_colTraceEnd;
		is >> m_fTraceTime;
		is >> m_fTimeInterval;
		is >> m_fTwinklePeriod;
	}
	else if(ubVer == 1)
	{
		is >> m_strTextureFileName;
		is >> m_colTraceBegin;
		is >> m_colTraceEnd;
		is >> m_fTraceTime;
		is >> m_fTimeInterval;
	}
*/
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CTraceEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CEffect::Write(pOS);

	os.WriteID_t("EFTC");
	os << (UBYTE)CURRENT_VERSION;

	os << m_strTextureFileName;
//	CPrintF("%s\n", m_strTextureFileName);	//log
	os << m_colTraceBegin;
	os << m_colTraceEnd;
	os << m_fTraceTime;
	os << m_fTimeInterval;
	os << m_fTwinklePeriod;
	os << m_bCapEnd;
	os << m_eBlendType;
}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
