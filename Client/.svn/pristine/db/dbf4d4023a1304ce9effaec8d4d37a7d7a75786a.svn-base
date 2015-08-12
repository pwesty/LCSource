//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include "StdH.h"

#include <Engine/Math/Functions.h>
#include <Engine/Base/Types.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Ska/Render.h>
#include <Engine/Models/ModelObject.h>
#include <Engine/Templates/Stock_CModelInstance.h>
#include <algorithm>
#include <Engine/Models/RenderModel.h>
#include <Engine/Models/ModelData.h>
#include <Engine/Templates/Stock_CModelData.h>
#include <Engine/Templates/Stock_CTextureData.h>

#include "CTag.h"
#include "CMdlEffect.h"
#include "CEffectControl.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CMdlEffect);

CMdlEffect::CMdlEffect()
: m_pModelObject( NULL )
, m_pModelTextureData( NULL )
, m_colModel( C_WHITE | CT_OPAQUE )
, m_vStretch( 1, 1, 1 )
, m_pEffectControl( NULL )
, m_vPostion( 0, 0, 0 )	
, m_vAngle( 0, 0, 0 )
, m_eRotation( EOTT_ALWAYS )
, m_ePosition( EOTT_START )
, m_bOverDraw( FALSE )
{
	CEffect::m_eType = ET_MDL;
}

CMdlEffect::~CMdlEffect()
{
	if(m_pModelObject) delete m_pModelObject;
	//if(m_pModelTextureData) delete m_pModelTextureData;
	if(m_pModelTextureData) _pTextureStock->Release(m_pModelTextureData);
	if(m_pEffectControl) delete m_pEffectControl;
}

BOOL CMdlEffect::SetModelObject(const CTFileName &strMdlFileName, const CTFileName &strTexFileName)
{
	ASSERT(m_pModelObject == NULL && m_pModelTextureData == NULL);
	if(strMdlFileName == m_strMdlFileName && strTexFileName == m_strTextureFileName) return FALSE;

	try
	{
		//모델을 생성하고 로드한다.
		m_pModelObject = new CModelObject;
		m_strMdlFileName = strMdlFileName;
		m_pModelObject->SetData_t(m_strMdlFileName);
		m_strTextureFileName = strTexFileName;
		//텍스쳐를 생성하고 로드한다.
		m_pModelTextureData = _pTextureStock->Obtain_t(m_strTextureFileName);
		//m_pModelTextureData = new CTextureData;
		//m_pModelTextureData->Load_t(m_strTextureFileName);
		m_pModelObject->SetTextureData(m_pModelTextureData);
	}
	catch(char *)
	{
		return FALSE;
	}
	return TRUE;
}

void CMdlEffect::SetEffectControl(const CEffectControl &effectControl)
{
	if(m_pEffectControl == NULL) m_pEffectControl = new CEffectControl;
	*m_pEffectControl = effectControl;
}

void CMdlEffect::EraseEffectControl()
{
	delete m_pEffectControl;
	m_pEffectControl = NULL;
}

CEffect *CMdlEffect::Copy()
{
	CMdlEffect *pRet = new CMdlEffect;
	if(pRet == NULL) return NULL;
	//CEffect의 content
	pRet->SetContent(this);
	//CMdlEffect의 content
	if(m_strMdlFileName!="" && m_strTextureFileName!="") pRet->SetModelObject(m_strMdlFileName, m_strTextureFileName);
	pRet->SetColor(this->GetColor());
	pRet->SetStretch(m_vStretch);
	pRet->SetRotationType(this->GetRotationType());
	pRet->SetPositionType(this->GetPositionType());
	if(m_pEffectControl) pRet->SetEffectControl(*m_pEffectControl);
	pRet->SetOverDraw(m_bOverDraw);
	return pRet;
}

void CMdlEffect::Start(FLOAT time, BOOL restart)
{
	if(!PreStart(time, restart)) return;
	if(m_pModelObject == NULL) m_eState = ES_PLAY_END;
	else if(m_pModelObject->GetAnimsCt() == 0) m_eState = ES_PLAY_END;
	if(m_eState != ES_NOT_STARTED) return;

	m_pModelObject->PlayAnim(0, AOF_LOOPING);
	m_pModelObject->mo_colBlendColor = m_colModel;
	m_pModelObject->StretchModel(m_vStretch);

	if(m_pEffectControl != NULL)
	{
		m_pEffectControl->SetFadeTime(m_fFadeInTime, m_fFadeOutTime);
	}

	m_bOnlyStart = TRUE;

	PostStart();
}

BOOL CMdlEffect::Process(FLOAT time)
{
	FLOAT fDeltaTime, fProcessedTime;
	BOOL bRet, bRender;
	if(!PreProcess(time, bRet, bRender, fDeltaTime, fProcessedTime))
	{
		m_pModelObject->PauseAnim();
		if(!bRender) SetNotRenderAtThisFrame();
		return bRet;
	}

	if(m_bOnlyStart)
	{
		m_vStartPosition = m_ptrAttachTag->CurrentTagInfo().m_vPos;
		m_ptrAttachTag->CurrentTagInfo().m_qRot.ToMatrix(m_matStartRotation);
		m_bOnlyStart = FALSE;
	}

	if(m_pEffectControl == NULL)
	{
		//Alpha의 변화를 처리
		COLOR colModel = m_colModel & C_WHITE;
		colModel |= ((m_colModel & 0x000000FF) * NormFloatToByte( GetFadeValue(fProcessedTime) )) >> 8; //model instance의 color
		//color setting
		m_pModelObject->mo_colBlendColor = colModel;
	}
	else
	{
		COLOR colModel = m_colModel;
		m_pEffectControl->Process(&colModel, &m_vStretch, &m_vPostion, &m_vAngle
								, fProcessedTime, m_fLifeTime, fDeltaTime);

		m_pModelObject->mo_colBlendColor = colModel;
		m_pModelObject->StretchModel(m_vStretch);
	}

	PostProcess();
	return TRUE;
}

BOOL CMdlEffect::BeginRender(CAnyProjection3D &apr, CDrawPort *pdp)
{
	gfxEnableBlend();
	BeginModelRenderingView(apr, pdp);
	return TRUE;
}

void CMdlEffect::EndRender(BOOL bRestoreOrtho)
{
	EndModelRenderingView(bRestoreOrtho);
}

void CMdlEffect::Render()
{
	if(!PreRender())
	{
		return;
	}
	if(m_pModelObject == NULL) return;
	
	if (GetOwner() != NULL)
	{ // HIDDEN 속성의 NPC의 이펙트를 보기 위해서는 캐릭터가 ENF_SHOWHIDDEN을 가지고 있어야 한다.
		if (GetOwner()->IsFlagOn(ENF_HIDDEN) && (CEntity::GetPlayerEntity(0)->IsFlagOff(ENF_SHOWHIDDEN) ||
			(CEntity::GetPlayerEntity(0)->IsFlagOn(ENF_SHOWHIDDEN)&&!GetOwner()->IsEnemy())))//ENF_SHOWHIDDEN이면 npc effect는 볼 수 있다.
			return;
	}

	TagInfo &ti = m_ptrAttachTag->CurrentTagInfo();
	FLOATmatrix3D matRot;
	if(m_eRotation == EOTT_NONE)			matRot.Diagonal(1);
	else if(m_eRotation == EOTT_START)		matRot = m_matStartRotation;
	else if (m_eRotation == EOTT_ALWAYS)	ti.m_qRot.ToMatrix(matRot);
	if (m_ePosition == EOTT_ALWAYS)			m_vStartPosition = ti.m_vPos;

	RotateVector(m_vPostion, matRot);
	if(m_pEffectControl && m_pEffectControl->IsDynamicAngle())
	{
		FLOATmatrix3D matOffsetRot;
		MakeRotationMatrixFast(matOffsetRot, m_vAngle);
		matRot *= matOffsetRot;
	}

	CRenderModel rm;
	rm.rm_vLightDirection = FLOAT3D(0,1,0);
	rm.rm_fDistanceFactor = 0.0f;
	FLOAT3D vPos = m_vStartPosition;
	if(m_pEffectControl && m_pEffectControl->IsDynamicPos())
	{
		FLOAT lastDynamicPosTime = m_pEffectControl->GetPosTime(m_pEffectControl->GetPosCount()-1);
/*
		if( m_fLifeTime - m_fLeftTime >= lastDynamicPosTime )
		{
			vPos = Lerp( m_vStartPosition + m_vPostion, vPos, 1 - m_fLeftTime / (m_fLifeTime - lastDynamicPosTime) );
		}
		else
*/
		{
			vPos += m_vPostion;
		}
	}
	rm.SetObjectPlacement(vPos, matRot);
	rm.rm_ulFlags &= ~RMF_FOG;
	rm.rm_ulFlags &= ~RMF_HAZE;
	rm.rm_ulFlags &= ~RMF_MIRROR;
	//rm.rm_ulFlags |= RMF_INSIDE;
	rm.rm_ulFlags &= ~RMF_INSIDE;
	rm.rm_ulFlags &= ~RMF_INMIRROR;
	rm.rm_rtRenderType = 0;

	if(m_bOverDraw)
	{
		rm.rm_ulFlags |= RMF_OVERDRAW;
	}
	//일단 막아둠. z test를 안하면 성벽이나 다른 캐릭위에 이펙트가 나올수가 있음.

	// set tesselation level of models
	rm.rm_iTesselationLevel = 0.0f;
	// mask in the need for skipping the occlusion test
	m_pModelObject->mo_bSkipOcclusionTest = TRUE;
	// render model
	m_pModelObject->SetupModelRendering(rm);
	m_pModelObject->RenderModel(rm);

	PostRender();
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 4

void CMdlEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CEffect::Read(pIS);

	is.ExpectID_t("EFMD");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_strMdlFileName;
		is >> m_strTextureFileName;
		is >> m_colModel;
		is >> m_vStretch;
		is >> (DWORD&)m_eRotation;
		is >> (DWORD&)m_ePosition;
		BOOL bEffectControl;
		is >> bEffectControl;
		if(bEffectControl)
		{
			if(m_pEffectControl == NULL) m_pEffectControl = new CEffectControl;
			m_pEffectControl->Read(&is);
		}
		is >> m_bOverDraw;
	}
	else if(ubVer == 3)
	{
		is >> m_strMdlFileName;
		is >> m_strTextureFileName;
		is >> m_colModel;
		is >> m_vStretch;
		is >> (DWORD&)m_eRotation;
		BOOL bEffectControl;
		is >> bEffectControl;
		if(bEffectControl)
		{
			if(m_pEffectControl == NULL) m_pEffectControl = new CEffectControl;
			m_pEffectControl->Read(&is);
		}
		is >> m_bOverDraw;
	}
/*
	else if(ubVer == 2)
	{
		is >> m_strMdlFileName;
		is >> m_strTextureFileName;
		is >> m_colModel;
		is >> m_vStretch;
		is >> (DWORD&)m_eRotation;
		BOOL bEffectControl;
		is >> bEffectControl;
		if(bEffectControl)
		{
			if(m_pEffectControl == NULL) m_pEffectControl = new CEffectControl;
			m_pEffectControl->Read(&is);
		}
	}
*/
	//old version
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CMdlEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CEffect::Write(pOS);

	os.WriteID_t("EFMD");
	os << (UBYTE)CURRENT_VERSION;

	os << m_strMdlFileName;
//	CPrintF("%s\n", m_strMdlFileName);	//log
	os << m_strTextureFileName;
//	CPrintF("%s\n", m_strTextureFileName);	//log
	os << m_colModel;
	os << m_vStretch;
	os << (DWORD)m_eRotation;
	os << (DWORD)m_ePosition;
	os << (BOOL)(m_pEffectControl != NULL);
	if(m_pEffectControl != NULL) m_pEffectControl->Write(&os);
	os << m_bOverDraw;
}

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)