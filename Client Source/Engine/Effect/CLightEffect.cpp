//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include "stdH.h"

#include <Engine/Math/Functions.h>
#include <Engine/Base/Types.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Light/LightSource.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/World/World.h>
#include <Engine/Base/Memory.h>

#include "CTag.h"
#include "CLightEffect.h"

#define DummyEntityFile CTFILENAME("classes\\dummy.ecl")

CWorld			*CLightEffect::m_pWorld = NULL;
CTerrain		*CLightEffect::m_pTerrain = NULL;
CTextureData	*CLightEffect::m_ptdLightMap = NULL;

//NEW_DELETE_IMPLEMENT(CLightEffect);

CLightEffect::CLightEffect()
: m_plsLight( NULL )
, m_penDummy( NULL )
, m_fMaxFallOff( 0 )
, m_colLight( 0 )
, m_bAmbientMode( 0 )
{
	CEffect::m_eType = ET_LIGHT;
}

CLightEffect::~CLightEffect()
{
	if(m_penDummy) m_penDummy->Destroy(FALSE);
	if(m_plsLight)
	{
		delete m_plsLight;
	}
}

CEffect *CLightEffect::Copy()
{
	CLightEffect *pRet = new CLightEffect;
	if(pRet == NULL) return NULL;
	//CEffect의 content
	pRet->SetContent(this);
	//CLightEffect의 content
	pRet->m_fMaxFallOff = m_fMaxFallOff;
	pRet->m_colLight = m_colLight;
	pRet->m_bAmbientMode = m_bAmbientMode;

	return pRet;
}

void CLightEffect::Start(FLOAT time, BOOL restart)
{
	if(!PreStart(time, restart)) return;
	if(m_pWorld == NULL) m_eState = ES_PLAY_END;
	if(m_eState != ES_NOT_STARTED) return;

	if(m_plsLight == NULL) m_plsLight = new CLightSource;

	CPlacement3D place;
	place.pl_PositionVector = FLOAT3D(m_ptrAttachTag->CurrentTagInfo().m_vPos);
	place.pl_OrientationAngle = ANGLE3D(0,0,0);
	if(m_penDummy == NULL)
	{
		m_penDummy = m_pWorld->CreateEntity_t(place, DummyEntityFile);
		m_penDummy->Initialize();
		m_penDummy->m_plsLight = m_plsLight;
	}
	m_plsLight->ls_penEntity = m_penDummy;
	
	CLightSource &lsNew = *m_plsLight;
	this->SetupLightSource(lsNew);
	m_plsLight->SetLightSource(lsNew);
	
	PostStart();
}

BOOL CLightEffect::Process(FLOAT time)
{
	FLOAT fDeltaTime, fProcessedTime;
	BOOL bRet, bRender;
	if(!PreProcess(time, bRet, bRender, fDeltaTime, fProcessedTime))
	{
		if(m_plsLight) m_plsLight->ls_rFallOff = 0.0f;
		if(!bRender) SetNotRenderAtThisFrame();
		return bRet;
	}

	//Color의 변화를 처리
	FLOAT fFadeValue = GetFadeValue(fProcessedTime);
	ULONG byteValue = NormFloatToByte(fFadeValue);
	COLOR colLight = (byteValue << CT_RSHIFT) | (byteValue << CT_GSHIFT) | (byteValue << CT_BSHIFT);

	//light setting
	m_plsLight->ls_rFallOff = m_fMaxFallOff * fFadeValue;
	if(m_bAmbientMode)	m_plsLight->ls_colAmbient = MulColors(m_colLight, colLight);
	else				m_plsLight->ls_colColor = MulColors(m_colLight, colLight);

	//light position setting
	CPlacement3D place;
	place.pl_PositionVector = FLOAT3D(m_ptrAttachTag->CurrentTagInfo().m_vPos);
	place.pl_OrientationAngle = ANGLE3D(0,0,0);
	m_penDummy->SetPlacement(place);
	
	PostProcess();
	return TRUE;
}

BOOL CLightEffect::BeginRender(CAnyProjection3D &apr, CDrawPort *pdp)
{
	TR_BeginRenderingView(apr, pdp);
	return TRUE;
}

void CLightEffect::EndRender(BOOL bRestoreOrtho)
{
	if(m_pTerrain != NULL && m_ptdLightMap != NULL)
	TR_RenderTerrainLighting(m_pTerrain, m_ptdLightMap);
	TR_EndRenderingView();
}

void CLightEffect::Render()
{
	if(!PreRender())
	{
		m_plsLight->ls_rFallOff = 0.0f;
		return;
	}
	
	if (GetOwner() != NULL)
	{ // HIDDEN 속성의 NPC의 이펙트를 보기 위해서는 캐릭터가 ENF_SHOWHIDDEN을 가지고 있어야 한다.
		if (GetOwner()->IsFlagOn(ENF_HIDDEN) && (CEntity::GetPlayerEntity(0)->IsFlagOff(ENF_SHOWHIDDEN) ||
			(CEntity::GetPlayerEntity(0)->IsFlagOn(ENF_SHOWHIDDEN)&&!GetOwner()->IsEnemy())))//ENF_SHOWHIDDEN이면 npc effect는 볼 수 있다.
			return;
	}

	//CLightEffect::End()에서 실제로 그리는 루틴이 호출됨.
	//굳이 이렇게 한 이유는 CLightEffect외에도 light가 있을 수 있는데 이런 라이트들도 같이 처리하기 위해서이다.
	//다른 라이트의 처리 루틴과 따로 있을 경우 중복 처리가 될 가능성이 크다. 중복처리 방지 코드도 있어야 하고...

	//비어있는 함수인 Render()가 계속 불려지는데 이건 좀 문제가 될 수 있다.

	PostRender();
}

void CLightEffect::SetupLightSource( CLightSource &lsNew )
{
	lsNew.ls_ulFlags = LSF_CASTSHADOWS;
	lsNew.ls_ulFlags |= LSF_DYNAMIC;

	lsNew.ls_rHotSpot = 0;
	lsNew.ls_rFallOff = 0;
	lsNew.ls_fNearClipDistance = 0.1f;
	lsNew.ls_fFarClipDistance  = 0.01f;

	lsNew.ls_colColor   = C_BLACK;
	lsNew.ls_colAmbient = C_BLACK; // only directional lights are allowed to have ambient component
	lsNew.ls_ubPolygonalMask = (UBYTE) 0;
	lsNew.ls_plftLensFlare = NULL;

	lsNew.ls_paoLightAnimation = NULL;
	lsNew.ls_paoAmbientLightAnimation = NULL;
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CLightEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CEffect::Read(pIS);

	is.ExpectID_t("EFLT");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_fMaxFallOff;
		is >> m_colLight;
		is >> m_bAmbientMode;
	}
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CLightEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CEffect::Write(pOS);

	os.WriteID_t("EFLT");
	os << (UBYTE)CURRENT_VERSION;

	os << m_fMaxFallOff;
	os << m_colLight;
	os << m_bAmbientMode;
}

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)