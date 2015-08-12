//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include "stdH.h"

#include <Engine/Math/Functions.h>
#include <Engine/Base/Types.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Light/LightSource.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Math/Projection.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/MAth/Matrix12.h>
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Entities/Entity.h>
#include <Engine/Math/AdditionalFunction.h>
#include <vector>

#include "CTag.h"
#include "CTerrainEffect.h"
#include <Engine/Base/Memory.h>

CTerrain *CTerrainEffect::m_pTerrain = NULL;

static CDrawPort *s_pdp = NULL;
static CAnyProjection3D *s_pApr = NULL;

//NEW_DELETE_IMPLEMENT(CTerrainEffect);

CTerrainEffect::CTerrainEffect()
: m_colEffect( 0 )
, m_ptdTexture( NULL )
, m_eRotation( EOTT_NONE )
, m_ePosition( EOTT_START )
, m_vPos( 0,0,0 )
, m_qRot( 1,0,0,0 )
, m_eBlendType( PBT_ADDALPHA )
{
	CEffect::m_eType = ET_TERRAIN;
	m_rcArea = CRectFlt(0,0,0,0);
}

CTerrainEffect::~CTerrainEffect()
{
	if(m_ptdTexture) _pTextureStock->Release(m_ptdTexture);
}

BOOL CTerrainEffect::SetTexture(const CTFileName &filename)
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

CEffect *CTerrainEffect::Copy()
{
	CTerrainEffect *pRet = new CTerrainEffect;
	if(pRet == NULL) return NULL;
	//CEffect의 content
	pRet->SetContent(this);
	//CTerrainEffect의 content
	pRet->SetTexture(m_strTextureFileName);
	pRet->m_rcArea = m_rcArea;
	pRet->m_colEffect = m_colEffect;
	pRet->m_eBlendType = m_eBlendType;
	pRet->m_ePosition = m_ePosition;
	pRet->m_eRotation = m_eRotation;

	return pRet;
}

void CTerrainEffect::Start(FLOAT time, BOOL restart)
{
	if(!PreStart(time, restart)) return;
	if(m_ptdTexture == NULL) m_eState = ES_PLAY_END;
	if(m_pTerrain == NULL) m_eState = ES_PLAY_END;
	if(m_eState != ES_NOT_STARTED) return;

	PostStart();
}

BOOL CTerrainEffect::Process(FLOAT time)
{
	FLOAT fDeltaTime, fProcessedTime;
	BOOL bRet, bRender;
	if(!PreProcess(time, bRet, bRender, fDeltaTime, fProcessedTime))
	{
		if(!bRender) SetNotRenderAtThisFrame();
		return bRet;
	}
	//Alpha의 변화를 처리
	UBYTE ubColElement = NormFloatToByte( GetFadeValue(fProcessedTime) );
	COLOR col;
	if( m_eBlendType == PBT_BLEND || m_eBlendType == PBT_ADDALPHA )
	{
		col = 0xFFFFFF00 | ubColElement; //fade by alpha
	}
	else if( m_eBlendType == PBT_ADD )
	{
		col = (ubColElement << 24) | (ubColElement << 16) | (ubColElement << 8) | 0x000000FF; //fade by color
	}
	else col = 0xFFFFFFFF;	//fade 의미 없음.
	m_colCurrent = MulColors(col, m_colEffect);
	
	m_vPos = m_ptrAttachTag->CurrentTagInfo().m_vPos;
	if(m_eRotation != EOTT_NONE) m_qRot = m_ptrAttachTag->CurrentTagInfo().m_qRot;

	PostProcess();
	return TRUE;
}

void CTerrainEffect::Render()
{
	if(!PreRender())
	{
		return;
	}
	if(m_pTerrain == NULL) return;
	
	if (GetOwner() != NULL)
	{ // HIDDEN 속성의 NPC의 이펙트를 보기 위해서는 캐릭터가 ENF_SHOWHIDDEN을 가지고 있어야 한다.
		if (GetOwner()->IsFlagOn(ENF_HIDDEN) && (CEntity::GetPlayerEntity(0)->IsFlagOff(ENF_SHOWHIDDEN) ||
			(CEntity::GetPlayerEntity(0)->IsFlagOn(ENF_SHOWHIDDEN)&&!GetOwner()->IsEnemy())))//ENF_SHOWHIDDEN이면 npc effect는 볼 수 있다.
			return;
	}

	gfxSetBlendType(m_eBlendType);

	gfxSetTextureUnit(0);
	m_ptdTexture->SetAsCurrent();

	CTerrainImp	*ptr = m_pTerrain->tr_ptrTerrain;
	FLOAT3D	*pavVertices;
	UWORD	*puwIndices;
	INDEX	ctVertices, ctIndices;
	INDEX	ctvtxX, ctvtxZ, ctvx;

	FLOAT3D	vStretch = ptr->tr_vStretch;
	FLOAT	fInvStretchX = 1.0f / vStretch( 1 );
	FLOAT	fInvStretchZ = 1.0f / vStretch( 3 );
	const CTRect	rcTerrain = CTRect( CTPoint( 0, 0 ),
										CTPoint( ptr->tr_pixHeightMapWidth - 1, ptr->tr_pixHeightMapHeight - 1 ) );
	// Get light information
	if(m_ePosition == EOTT_ALWAYS) m_vPos = m_ptrAttachTag->CurrentTagInfo().m_vPos;
	if(m_eRotation == EOTT_ALWAYS) m_qRot = m_ptrAttachTag->CurrentTagInfo().m_qRot;
	FLOAT	fRadius = m_rcArea.GetRadius();

	// Test sphere in frustum
	FLOAT3D	vViewPosition;
	(*s_pApr)->PreClip( m_vPos, vViewPosition );
	if( (*s_pApr)->TestSphereToFrustum( vViewPosition, fRadius ) == -1 )
		return;

	// Prepare terrain effect
	ANGLE head = -RadAngle( GetEulerAngleFromQuaternion(m_qRot)(1) );
	FLOATquat3D qRot;
	qRot.FromAxisAngle(FLOAT3D(0,1,0), head);
	FLOAT3D pos[4];	//좌상, 우상, 좌하, 우하
	pos[0] = FLOAT3D(m_rcArea.m_fLeft, 0, m_rcArea.m_fTop);
	pos[1] = FLOAT3D(m_rcArea.m_fRight, 0, m_rcArea.m_fTop);
	pos[2] = FLOAT3D(m_rcArea.m_fLeft, 0, m_rcArea.m_fBottom);
	pos[3] = FLOAT3D(m_rcArea.m_fRight, 0, m_rcArea.m_fBottom);
	RotateVector(pos[0], qRot);
	RotateVector(pos[1], qRot);
	RotateVector(pos[2], qRot);
	RotateVector(pos[3], qRot);
	CRectFlt rcRotRect;
	rcRotRect.m_fLeft = Min( Min(pos[0](1), pos[1](1)), Min(pos[2](1), pos[3](1)) );
	rcRotRect.m_fRight = Max( Max(pos[0](1), pos[1](1)), Max(pos[2](1), pos[3](1)) );
	rcRotRect.m_fTop = Min( Min(pos[0](3), pos[1](3)), Min(pos[2](3), pos[3](3)) );
	rcRotRect.m_fBottom = Max( Max(pos[0](3), pos[1](3)), Max(pos[2](3), pos[3](3)) );

	CTRect rcEffect;
	rcEffect.rc_slLeft = ( m_vPos( 1 ) + rcRotRect.m_fLeft ) * fInvStretchX;
	rcEffect.rc_slTop = ( m_vPos( 3 ) + rcRotRect.m_fTop ) * fInvStretchZ;
	rcEffect.rc_slRight = ( m_vPos( 1 ) + rcRotRect.m_fRight ) * fInvStretchX + 1;
	rcEffect.rc_slBottom = ( m_vPos( 3 ) + rcRotRect.m_fBottom ) * fInvStretchZ + 1;
	rcEffect = ClampRect( rcEffect, rcTerrain );

	TR_ExtractPoligonsInRect( m_pTerrain, rcEffect, &pavVertices, ctVertices, &puwIndices, ctIndices );
	if( ctVertices <= 0 || ctIndices <= 0 || ctVertices > 65536 || ctVertices > 65536 )
		return;

	ctvtxX = rcEffect.GetWidth() + 1;
	ctvtxZ = rcEffect.GetHeight() + 1;
	ctvx = ctvtxX * ctvtxZ;

	std::vector<GFXTexCoord> vectorTexCoord;
	std::vector<GFXColor> vectorColor;
	vectorTexCoord.reserve(ctvx);
	vectorColor.reserve(ctvx);

	//texture 좌표 생성.
	FLOAT aa = cos(head) / m_rcArea.GetWidth();
	FLOAT bb = sin(head) / m_rcArea.GetWidth();
	FLOAT cc = sin(head) / m_rcArea.GetHeight();
	FLOAT dd = -cos(head) / m_rcArea.GetHeight();
	FLOAT ee = m_vPos(1) + pos[0].vector[0];
	FLOAT ff = m_vPos(3) - pos[0].vector[2];

	GFXTexCoord	tc;
	for( INDEX ivx = 0; ivx < ctvx; ++ivx )
	{
		FLOAT3D	&vx = pavVertices[ivx];
		tc.u = (vx(1) - ee) * aa + (vx(3) - ff) * bb;
		tc.v = (vx(1) - ee) * cc + (vx(3) - ff) * dd;
		vectorTexCoord.push_back(tc);
		vectorColor.push_back(m_colCurrent);
	}


/*
	FLOAT	fOffsetX, fOffsetZ, fSizeX, fSizeY;
	fOffsetX = m_vPos( 1 ) - m_rcArea.GetWidth() * 0.5f;
	fOffsetZ = m_vPos( 3 ) - m_rcArea.GetHeight() * 0.5f;
	fSizeX = m_rcArea.GetWidth();
	fSizeY = m_rcArea.GetHeight();
	for( INDEX ivx = 0; ivx < ctvx; ++ivx )
	{
		FLOAT3D		&vx = pavVertices[ivx];
		GFXTexCoord	tc;
		//if(vx(1) <= vPosition(1))	tc.u = (vx(1) - vPosition(1) + m_rcArea.rc_slLeft) / m_rcArea.GetWidth();
		//else						tc.u = (vx(1) - vPosition(1) + m_rcArea.rc_slRight) / m_rcArea.GetWidth();
		//if(vx(3) <= vPosition(3))	tc.v = (vx(3) - vPosition(3) + m_rcArea.rc_slTop) / m_rcArea.GetHeight();
		//else						tc.v = (vx(3) - vPosition(3) + m_rcArea.rc_slBottom) / m_rcArea.GetHeight();
		tc.u = ( vx( 1 ) - fOffsetX ) / fSizeX;
		tc.v = ( vx( 3 ) - fOffsetZ ) / fSizeY;
		vectorTexCoord.push_back(tc);
		vectorColor.push_back(m_colEffect);
	}
*/

	// Render terrain effect
	gfxSetVertexArray( (GFXVertex *)&pavVertices[0], ctVertices );
	gfxSetColorArray( &vectorColor[0] );
	gfxSetTexCoordArray( &vectorTexCoord[0], FALSE );
	gfxDrawElements( ctIndices, puwIndices );

	PostRender();
}

BOOL CTerrainEffect::BeginRender(CAnyProjection3D &apr, CDrawPort *pdp)
{
	TR_BeginRenderingView(apr, pdp);
	
	//프로젝션 설정
	apr->ObjectPlacementL() = CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0));
	apr->Prepare();
	// in case of mirror projection, move mirror clip plane a bit father from the mirrored models,
	// so we have less clipping (for instance, player feet)
	if( apr->pr_bMirror) apr->pr_plMirrorView.pl_distance -= 0.06f; // -0.06 is because entire projection is offseted by +0.05
	else if(apr->pr_bNiceWater) apr->pr_plNiceWaterView.pl_distance -= 0.06f;
	pdp->SetProjection( apr );
	s_pdp = pdp;
	s_pApr = &apr;
	Matrix12 mAbsToViewer;
	MatrixVectorToMatrix12(mAbsToViewer,
		apr->pr_ViewerRotationMatrix, 
		-apr->pr_vViewerPosition*apr->pr_ViewerRotationMatrix);

	FLOATmatrix3D m;
	Matrix12 mObjToAbs, mObjToView;
	if(m_pTerrain)
	{
		MakeRotationMatrixFast( m, m_pTerrain->tr_penEntity->en_plPlacement.pl_OrientationAngle);
		MatrixVectorToMatrix12(mObjToAbs,m, m_pTerrain->tr_penEntity->en_plPlacement.pl_PositionVector);
	}
	else
	{
		MakeRotationMatrixFast( m, ANGLE3D(0,0,0));
		MatrixVectorToMatrix12(mObjToAbs,m, FLOAT3D(0,0,0));
	}
	MatrixMultiply(mObjToView, mAbsToViewer, mObjToAbs);
	
	//렌더링 상태 설정
	Matrix12 &mView12 = mObjToView;
	FLOAT mView16[16];
	mView16[ 0] = mView12[ 0];  mView16[ 1] = mView12[ 4];  mView16[ 2] = mView12[ 8];  mView16[ 3] = 0;
	mView16[ 4] = mView12[ 1];  mView16[ 5] = mView12[ 5];  mView16[ 6] = mView12[ 9];  mView16[ 7] = 0;
	mView16[ 8] = mView12[ 2];  mView16[ 9] = mView12[ 6];  mView16[10] = mView12[10];  mView16[11] = 0;
	mView16[12] = mView12[ 3];  mView16[13] = mView12[ 7];  mView16[14] = mView12[11];  mView16[15] = 1;
	gfxSetViewMatrix(mView16);

	gfxCullFace(GFX_BACK);
	//gfxEnableVertexProgram();
	//gfxEnablePixelProgram();
	gfxEnableBlend();
	//gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA );
	//gfxBlendFunc( GFX_ONE, GFX_ONE );
	gfxDisableDepthWrite();
	gfxEnableDepthTest();
	gfxSetTextureWrapping( GFX_CLAMP, GFX_CLAMP );
	gfxSetTextureMatrix(NULL);
	gfxEnableDepthBias();
	gfxEnableColorArray();
	return TRUE;
}

void CTerrainEffect::EndRender(BOOL bRestoreOrtho)
{
	gfxDisableBlend();
	gfxEnableDepthWrite();
	gfxDisableDepthBias();

	// back to 2D projection?
	if( bRestoreOrtho ) s_pdp->SetOrtho();

	TR_EndRenderingView();
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 2

void CTerrainEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CEffect::Read(pIS);

	is.ExpectID_t("EFTR");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_strTextureFileName;
		is >> m_rcArea.m_fBottom;
		is >> m_rcArea.m_fLeft;
		is >> m_rcArea.m_fRight;
		is >> m_rcArea.m_fTop;
		is >> m_colEffect;
		is >> (DWORD&)m_eBlendType;
		is >> (DWORD&)m_ePosition;
		is >> (DWORD&)m_eRotation;
	}
	//old version
	else if(ubVer == 1)
	{
		is >> m_strTextureFileName;
		is >> m_rcArea.m_fBottom;
		is >> m_rcArea.m_fLeft;
		is >> m_rcArea.m_fRight;
		is >> m_rcArea.m_fTop;
		is >> m_colEffect;
		is >> (DWORD&)m_eBlendType;
		m_ePosition = EOTT_START;
		m_eRotation = EOTT_START;
	}
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CTerrainEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CEffect::Write(pOS);

	os.WriteID_t("EFTR");
	os << (UBYTE)CURRENT_VERSION;

	os << m_strTextureFileName;
//	CPrintF("%s\n", m_strTextureFileName);	//log
	os << m_rcArea.m_fBottom;
	os << m_rcArea.m_fLeft;
	os << m_rcArea.m_fRight;
	os << m_rcArea.m_fTop;
	os << m_colEffect;
	os << (DWORD)m_eBlendType;
	os << (DWORD)m_ePosition;
	os << (DWORD)m_eRotation;
}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)