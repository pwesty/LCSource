#include "StdH.h"
#include "CShockWaveEffect.h"

#include <Engine/Math/AdditionalFunction.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Graphics/Shader.h>
#include <Engine/Math/Projection.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Math/Matrix12.h>
#include <Engine/Math/Functions.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Graphics/GfxLibrary.h>

extern BOOL RM_AreHardwareShadersAvailable(void);

static CDrawPort *s_pdp = NULL;
static CAnyProjection3D *s_pApr = NULL;

ULONG CShockWaveEffect::m_ulVertexProgramNoTex = 0;
ULONG CShockWaveEffect::m_ulPixelProgramNoTex = 0;
ULONG CShockWaveEffect::m_ulVertexProgramTex = 0;
ULONG CShockWaveEffect::m_ulPixelProgramTex = 0;

//NEW_DELETE_IMPLEMENT(CShockWaveEffect);

CShockWaveEffect::CShockWaveEffect()
: m_iSplitCount( 24 )
, m_ptdTexture( NULL )
, m_eRotation( EOTT_NONE )
, m_ePosition( EOTT_START )
, m_eBlendType( PBT_ADDALPHA )
{
	CEffect::m_eType = ET_SHOCKWAVE;
}

CShockWaveEffect::~CShockWaveEffect()
{
	if(m_ptdTexture) _pTextureStock->Release(m_ptdTexture);

	ClearInstance();
}

void CShockWaveEffect::ClearInstance()
{
	m_vectorMoveVector.clear();
	m_vectorGFXVertex.clear();
	m_vectorTexCoord.clear();
}

void CShockWaveEffect::Start(FLOAT time, BOOL restart)
{
	INDEX	i;
	if(!PreStart(time, restart)) return;
	if(m_eState != ES_NOT_STARTED) return;

	ASSERT(m_iSplitCount >= 8);

	ClearInstance();

	//vertex index, once
	m_vectorIndex.reserve(m_iSplitCount * 2 * 3);
	for( i=0; i<m_iSplitCount-1; ++i)
	{
		m_vectorIndex.push_back(2 * i + 0);
		m_vectorIndex.push_back(2 * i + 2);
		m_vectorIndex.push_back(2 * i + 1);
		m_vectorIndex.push_back(2 * i + 1);
		m_vectorIndex.push_back(2 * i + 2);
		m_vectorIndex.push_back(2 * i + 3);
	}
	m_vectorIndex.push_back(2 * (m_iSplitCount-1) + 0);
	m_vectorIndex.push_back(0);
	m_vectorIndex.push_back(2 * (m_iSplitCount-1) + 1);
	m_vectorIndex.push_back(2 * (m_iSplitCount-1) + 1);
	m_vectorIndex.push_back(0);
	m_vectorIndex.push_back(1); 

	//move vector fill, once
	m_vectorMoveVector.reserve(m_iSplitCount * 2);
	const FLOAT angleUnit = 2 * PI / m_iSplitCount;
	FLOAT angle = 0;
	for( i = 0; i < m_iSplitCount; ++i)
	{
		m_vectorMoveVector.push_back( FLOAT3D( sinf(angle), 0, cosf(angle) ) );
		m_vectorMoveVector.push_back( FLOAT3D( sinf(angle), 0, cosf(angle) ) );
		angle += angleUnit;
	}

	//tex coord fill, once
	m_vectorTexCoord.reserve(m_iSplitCount * 2);
	GFXTexCoord texcrd1, texcrd2, texcrd3, texcrd4;
	texcrd1.s = 0.05f;
	texcrd1.t = 0.05f;
	texcrd2.s = 0.05f;
	texcrd2.t = 0.95f;
	texcrd3.s = 0.95f;
	texcrd3.t = 0.05f;
	texcrd4.s = 0.95f;
	texcrd4.t = 0.95f;
	for( i = 0; i < m_iSplitCount; ++i)
	{
		if(i % 2 == 0)
		{
			m_vectorTexCoord.push_back(texcrd1);
			m_vectorTexCoord.push_back(texcrd2);
		}
		else
		{
			m_vectorTexCoord.push_back(texcrd3);
			m_vectorTexCoord.push_back(texcrd4);
		}
	}

	m_ssRadius.Prepare();
	m_ssWidth.Prepare();
	m_ssHeight.Prepare();

	//first state gfx vertex filll, fir frame
	FLOAT height = m_ssHeight.Value(0);
	FLOAT radiusInner = 0, radiusOuter = 0;
	if(m_bInnerBasis)
	{
		radiusInner = m_ssRadius.Value(0);
		radiusOuter = radiusInner + m_ssWidth.Value(0);
	}
	else
	{
		radiusOuter = m_ssRadius.Value(0);
		radiusInner = radiusOuter - m_ssWidth.Value(0);
	}

	m_vectorGFXVertex.clear();
	m_vectorGFXVertex.reserve(m_iSplitCount * 2);

	//tag의 정보
	m_vTagPos = m_ptrAttachTag->CurrentTagInfo().m_vPos;
	if(m_eRotation != EOTT_NONE) m_qTagRot = m_ptrAttachTag->CurrentTagInfo().m_qRot;

	//gfx vertex fill, every frame
/*
	GFXVertex vtx;
	FLOAT3D pos;
	angle = 0;
	for(i=0; i<m_iSplitCount; ++i)
	{
		pos = m_vectorMoveVector[2*i+0] * radiusOuter;
		RotateVector(pos, rotTag);
		pos += posTag;
		vtx.x = pos(1);	vtx.y = pos(2);	vtx.z = pos(3);
		m_vectorGFXVertex.push_back(vtx);

		pos = m_vectorMoveVector[2*i+1] * radiusInner;
		RotateVector(pos, rotTag);
		pos += posTag;
		vtx.x = pos(1);	vtx.y = pos(2);	vtx.z = pos(3);
		m_vectorGFXVertex.push_back(vtx);
		angle += angleUnit;
	}

	//gfx color fill, once
	GFXColor col;
	for(i=0; i<m_iSplitCount; ++i)
	{
		col.Set(0xFFFFFFFF);
		m_vectorGFXColor.push_back(col);
		m_vectorGFXColor.push_back(col);
	}
*/
	m_bColorWhite = TRUE;

	PostStart();
}

BOOL CShockWaveEffect::Process(FLOAT time)
{
	FLOAT fDeltaTime, fProcessedTime;
	BOOL bRet, bRender;
	if(!PreProcess(time, bRet, bRender, fDeltaTime, fProcessedTime))
	{
		if(!bRender) SetNotRenderAtThisFrame();
		return bRet;
	}

	//color 정보
	FLOAT fadeVal = this->GetFadeValue(fProcessedTime);
	if(fadeVal == 1.0f && m_bColorWhite)
	{
		//nothing
	}
	else
	{
		m_vectorGFXColor.clear();
		m_vectorGFXColor.reserve(m_iSplitCount*2);
		UBYTE ubColElement = UBYTE(255.0f * fadeVal);
		GFXColor col;
		if( m_eBlendType == PBT_BLEND || m_eBlendType == PBT_ADDALPHA )
		{
			col = 0xFFFFFF00 | ubColElement; //fade by alpha
		}
		else if( m_eBlendType == PBT_ADD )
		{
			col = (ubColElement << 24) | (ubColElement << 16) | (ubColElement << 8) | 0x000000FF; //fade by color
		}
		else col = 0xFFFFFFFF;	//fade 의미 없음.
		for(INDEX i=0; i<m_iSplitCount; ++i)
		{
			m_vectorGFXColor.push_back(col);
			m_vectorGFXColor.push_back(col);
		}
	}

	//wave 정보
	m_ssHeight.Prepare();
	FLOAT3D height(0, m_ssHeight.Value(fProcessedTime), 0);
	FLOAT radiusInner = 0, radiusOuter = 0;
	m_ssRadius.Prepare();
	m_ssWidth.Prepare();
	if(m_bInnerBasis)
	{
		radiusInner = m_ssRadius.Value(fProcessedTime);
		radiusOuter = radiusInner + m_ssWidth.Value(fProcessedTime);
	}
	else
	{
		radiusOuter = m_ssRadius.Value(fProcessedTime);
		radiusInner = radiusOuter - m_ssWidth.Value(fProcessedTime);
	}

	//tag의 정보
	if(m_ePosition == EOTT_ALWAYS) m_vTagPos = m_ptrAttachTag->CurrentTagInfo().m_vPos;
	if(m_eRotation == EOTT_ALWAYS) m_qTagRot = m_ptrAttachTag->CurrentTagInfo().m_qRot;

	//gfx vertex fill, every frame
	GFXVertex vtx;
	FLOAT3D pos;
	const FLOAT angleUnit = 2 * PI / m_iSplitCount;
	FLOAT angle = 0;
	INDEX index = 0;
	m_vectorGFXVertex.clear();
	m_vectorGFXColor.reserve(m_iSplitCount*2);
	for(INDEX i=0; i<m_iSplitCount; ++i)
	{
		index = 2*i+0;
		pos = m_vectorMoveVector[index] * radiusOuter + height;
		RotateVector(pos, m_qTagRot);
		pos += m_vTagPos;
		vtx.x = pos(1);	vtx.y = pos(2);	vtx.z = pos(3);
		m_vectorGFXVertex.push_back(vtx);

		++index;
		pos = m_vectorMoveVector[index] * radiusInner;
		RotateVector(pos, m_qTagRot);
		pos += m_vTagPos;
		vtx.x = pos(1);	vtx.y = pos(2);	vtx.z = pos(3);
		m_vectorGFXVertex.push_back(vtx);
		angle += angleUnit;
	}

	PostProcess();
	return TRUE;
}

BOOL CShockWaveEffect::SetTexture(const CTFileName &filename)
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

CEffect *CShockWaveEffect::Copy()
{
	CShockWaveEffect *pRet = new CShockWaveEffect;
	if(pRet == NULL) return NULL;
	//CEffect의 content
	pRet->SetContent(this);
	//CShockWaveEffect의 content
	pRet->m_bInnerBasis = m_bInnerBasis;
	pRet->m_iSplitCount = m_iSplitCount;
	pRet->m_ssRadius = m_ssRadius;
	pRet->m_ssWidth = m_ssWidth;
	pRet->m_ssHeight = m_ssHeight;
	pRet->m_eRotation = m_eRotation;
	pRet->m_ePosition = m_ePosition;
	pRet->SetTexture(m_strTextureFileName);
	pRet->m_eBlendType = m_eBlendType;
	return pRet;
}

void CShockWaveEffect::Render()
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

	gfxSetVertexArray( &m_vectorGFXVertex[0], m_vectorGFXVertex.size() );
	gfxSetColorArray( &m_vectorGFXColor[0] );
	if(m_ptdTexture != NULL) gfxSetTexCoordArray( &m_vectorTexCoord[0], FALSE );
	gfxDrawElements( m_iSplitCount * 2 * 3, &m_vectorIndex[0]);

	if(!RM_AreHardwareShadersAvailable())
	{
		//gfxSetTextureSelectArg(0, GFX_TA_TEXTURE, GFX_TA_TEXTURE);
		gfxSetTextureModulation(1);
	}
	PostRender();
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 2

void CShockWaveEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CEffect::Read(pIS);

	is.ExpectID_t("SWVE");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_strTextureFileName;	//texture의 파일 이름
		is >> m_bInnerBasis;
		is >> m_iSplitCount;
		ssReadFromStream(is, m_ssRadius, FLOAT);
		ssReadFromStream(is, m_ssWidth, FLOAT);
		ssReadFromStream(is, m_ssHeight, FLOAT);
		is >> (INDEX&)m_eRotation;			//tag의 회전에 얼만큼 영향을 받을 것인가?
		is >> (INDEX&)m_ePosition;			//tag의 이동에 얼만큼 영향을 받을 것인가?
		is >> (INDEX&)m_eBlendType;
	}
	else if(ubVer == 1)
	{
		is >> m_strTextureFileName;	//texture의 파일 이름
		is >> m_bInnerBasis;
		is >> m_iSplitCount;
		ssReadFromStream(is, m_ssRadius, FLOAT);
		ssReadFromStream(is, m_ssWidth, FLOAT);
		ssReadFromStream(is, m_ssHeight, FLOAT);
		is >> (INDEX&)m_eRotation;			//tag의 회전에 얼만큼 영향을 받을 것인가?
		is >> (INDEX&)m_ePosition;			//tag의 이동에 얼만큼 영향을 받을 것인가?
		m_eBlendType = PBT_ADDALPHA;
	}
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CShockWaveEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CEffect::Write(pOS);

	os.WriteID_t("SWVE");
	os << (UBYTE)CURRENT_VERSION;

	os << m_strTextureFileName;	//texture의 파일 이름
	os << m_bInnerBasis;
	os << m_iSplitCount;
	ssWriteToStream(os, m_ssRadius);
	ssWriteToStream(os, m_ssWidth);
	ssWriteToStream(os, m_ssHeight);
	os << (INDEX)m_eRotation;			//tag의 회전에 얼만큼 영향을 받을 것인가?
	os << (INDEX)m_ePosition;			//tag의 이동에 얼만큼 영향을 받을 것인가?
	os << (INDEX)m_eBlendType;
}

BOOL CShockWaveEffect::BeginRender(CAnyProjection3D &apr, CDrawPort *pdp)
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
//	gfxPolygonMode(GFX_LINE);
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
void CShockWaveEffect::EndRender(BOOL bRestoreOrtho)
{
	//렌더링 상태 설정
	gfxDisableVertexProgram();
	gfxDisablePixelProgram();
	gfxDisableBlend();
	gfxEnableDepthWrite();
	gfxDisableColorArray();
//	gfxPolygonMode(GFX_FILL);

	// re-enable color mask (maybe occ test left it disabled)
	gfxSetColorMask(CT_RMASK|CT_GMASK|CT_BMASK|CT_AMASK);
	// back to 2D projection?
	if( bRestoreOrtho ) s_pdp->SetOrtho();
}

void CShockWaveEffect::InitializeShaders()
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

void CShockWaveEffect::FinalizeShaders()
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
