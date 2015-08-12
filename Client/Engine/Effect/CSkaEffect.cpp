//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include "StdH.h"

#include <Engine/Ska/Render.h>
#include <algorithm>
#include <Engine/Templates/DynamicContainer.cpp>

#include "CTag.h"
#include "CSkaEffect.h"
#include "CEffectControl.h"
#include <Engine/Base/Memory.h>

//NEW_DELETE_IMPLEMENT(CSkaEffect);

CSkaEffect::CSkaEffect()
: m_pCreatedModel( NULL )
, m_colModel( C_WHITE | CT_OPAQUE )
, m_vStretch( 1, 1, 1 )
, m_pEffectControl( NULL )
, m_vPostion( 0, 0, 0 )	
, m_vAngle( 0, 0, 0 )
, m_eRotation( EOTT_ALWAYS )
, m_ePosition( EOTT_START )
{
	CEffect::m_eType = ET_SKA;
}

CSkaEffect::~CSkaEffect()
{
	if(m_pCreatedModel) DeleteModelInstance(m_pCreatedModel);
	EraseEffectControl();
}

BOOL CSkaEffect::SetModelInstance(const CTFileName &filename)
{
	if(m_pCreatedModel && m_strModelFileName != filename.str_String) DeleteModelInstance(m_pCreatedModel);

	m_strModelFileName = filename;
	m_pCreatedModel = ObtainModelInstance_t(filename);
	return TRUE;
}

void CSkaEffect::SetEffectControl(const CEffectControl &effectControl)
{
	if(m_pEffectControl == NULL) m_pEffectControl = new CEffectControl;
	*m_pEffectControl = effectControl;
}

void CSkaEffect::EraseEffectControl()
{
	delete m_pEffectControl;
	m_pEffectControl = NULL;
}

CEffect *CSkaEffect::Copy()
{
	CSkaEffect *pRet = new CSkaEffect;
	//CEffect의 content
	pRet->SetContent(this);
	//CSkaEffect의 content
	if(m_strModelFileName!="") pRet->SetModelInstance(m_strModelFileName);
	pRet->SetColor(this->GetColor());
	pRet->SetStretch(this->GetStretch());
	pRet->SetRotationType(this->GetRotationType());
	pRet->SetPositionType(this->GetPositionType());
	if(m_pEffectControl) pRet->SetEffectControl(*m_pEffectControl);
	return pRet;
}

void CSkaEffect::Start(FLOAT time, BOOL restart)
{
	if(!PreStart(time, restart)) return;

	if(m_pCreatedModel == NULL) m_eState = ES_PLAY_END;
	if(m_eState != ES_NOT_STARTED) return;
	
	//부모의 애니를 시작한다.
	INDEX effectAni = m_pCreatedModel->FindFirstAnimationID();
	if(effectAni != -1) m_pCreatedModel->AddAnimation(effectAni, AN_LOOPING|AN_CLEAR, 1, 0);
	m_pCreatedModel->StretchModel(m_vStretch);	//자식은 자동적용

	//자식들의 애니를 시작한다.
	INDEX ctmi = m_pCreatedModel->mi_cmiChildren.Count();
	for(INDEX i=0; i<ctmi; i++)
	{
		CModelInstance &mi = m_pCreatedModel->mi_cmiChildren[i];
		INDEX effectAni = mi.FindFirstAnimationID();
		if(effectAni == -1) continue;
		mi.AddAnimation(effectAni, AN_LOOPING|AN_CLEAR, 1, 0);
	}

	if(m_pEffectControl != NULL)
	{
		m_pEffectControl->SetFadeTime(m_fFadeInTime, m_fFadeOutTime);
	}

	m_ptrAttachTag->CurrentTagInfo().m_qRot.ToMatrix(m_matStartRotation);
	m_vStartPosition = m_ptrAttachTag->CurrentTagInfo().m_vPos;

	PostStart();
}

BOOL CSkaEffect::Process(FLOAT time)
{
	FLOAT fDeltaTime, fProcessedTime;
	BOOL bRet, bRender;
	if(!PreProcess(time, bRet, bRender, fDeltaTime, fProcessedTime))
	{
		m_pCreatedModel->StopAllAnimations(0);
		if(!bRender) SetNotRenderAtThisFrame();
		return bRet;
	}

	m_vPostion = m_ptrAttachTag->CurrentTagInfo().m_vPos;
	m_vAngle = GetEulerAngleFromQuaternion(m_ptrAttachTag->CurrentTagInfo().m_qRot);
	if(m_pEffectControl == NULL)
	{
		//Alpha의 변화를 처리
		COLOR colModel = m_colModel & C_WHITE;
		colModel |= ((m_colModel & 0x000000FF) * NormFloatToByte( GetFadeValue(fProcessedTime) )) >> 8; //model instance의 color
		//color setting
		m_pCreatedModel->SetModelColor(colModel);
		INDEX ctmi = m_pCreatedModel->mi_cmiChildren.Count();
		for(INDEX i=0; i<ctmi; i++)
		{
			CModelInstance &mi = m_pCreatedModel->mi_cmiChildren[i];
			mi.SetModelColor(colModel);
		}
	}
	else
	{
		m_pEffectControl->Process(&m_colModel, &m_vStretch, &m_vPostion, &m_vAngle
								, fProcessedTime, m_fLifeTime, fDeltaTime);
		
		if (m_penOwner != NULL && m_eRotation == EOTT_NONE)
		{// ska effect모델의 방향이 시선 방향이 되도록 하기 위해 처리
			m_vAngle(1) = m_penOwner.ep_pen->GetPlacement().pl_OrientationAngle(1);
		}

		m_pCreatedModel->SetModelColor(m_colModel);
		INDEX ctmi = m_pCreatedModel->mi_cmiChildren.Count();
		for(INDEX i=0; i<ctmi; i++) m_pCreatedModel->mi_cmiChildren[i].SetModelColor(m_colModel);
		m_pCreatedModel->StretchModel(m_vStretch);
	}

	PostProcess();
	return TRUE;
}

FLOAT3D CSkaEffect::GetInstancePosition(FLOATmatrix3D &matRotTag)
{
	if(m_ptrAttachTag.IsNull()) return FLOAT3D(0,0,0);
	TagInfo &ti = m_ptrAttachTag->CurrentTagInfo();
	if(m_eRotation == EOTT_NONE)			matRotTag.Diagonal(1);
	else if(m_eRotation == EOTT_START)		matRotTag = m_matStartRotation;
	else if (m_eRotation == EOTT_ALWAYS)	ti.m_qRot.ToMatrix(matRotTag);
	if(m_ePosition == EOTT_ALWAYS)			m_vStartPosition = ti.m_vPos;

	RotateVector(m_vPostion, matRotTag);
	if(m_pEffectControl && m_pEffectControl->IsDynamicPos())
		return m_vPostion + m_vStartPosition;
	else
		return m_vStartPosition;
}

FLOATquat3D CSkaEffect::GetInstanceRotation()
{
	//TODO??
	return FLOATquat3D(1,0,0,0);
}

BOOL CSkaEffect::BeginRender(CAnyProjection3D &apr, CDrawPort *pdp)
{
	RM_BeginRenderingView(apr, pdp);
	RM_SetLightDirection(FLOAT3D(0,-1,0));
	RM_SetLightColor(0x80808080, 0x20202020);
	RM_SetMaxWeightsPerVertex( 4 );
	RM_SetCurrentDistance( 0.0f );	//always use first lod
	return TRUE;
}
void CSkaEffect::EndRender(BOOL bRestoreOrtho)
{
	RM_EndRenderingView(bRestoreOrtho);
}

void CSkaEffect::Render()
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

	if(m_ptrAttachTag.IsNull())
	{
		m_fLifeTime = 0;
		return;
	}
	FLOATmatrix3D matRot;
	FLOAT3D vPos = GetInstancePosition(matRot);
	if(m_pEffectControl && m_pEffectControl->IsDynamicAngle())
	{
		FLOATmatrix3D matOffsetRot;
		MakeRotationMatrixFast(matOffsetRot, m_vAngle);
		matRot *= matOffsetRot;
	}

	RM_SetObjectPlacement(matRot, vPos);
	RM_RenderSKA(*m_pCreatedModel);

	PostRender();
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 3

void CSkaEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CEffect::Read(pIS);

	is.ExpectID_t("EFSK");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		is >> m_strModelFileName;
		is >> m_colModel;
		is >> m_vStretch;
		is >> (DWORD&)m_ePosition;
		is >> (DWORD&)m_eRotation;
		BOOL bEffectControl;
		is >> bEffectControl;
		if(bEffectControl)
		{
			if(m_pEffectControl == NULL) m_pEffectControl = new CEffectControl;
			m_pEffectControl->Read(&is);
		}
	}
	else if(ubVer == 2)
	{
		is >> m_strModelFileName;
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
/*
	else if(ubVer == 1)
	{
		is >> m_strModelFileName;
		is >> m_colModel;
		is >> m_vStretch;
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

void CSkaEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CEffect::Write(pOS);

	os.WriteID_t("EFSK");
	os << (UBYTE)CURRENT_VERSION;

	os << m_strModelFileName;
//	CPrintF("%s\n", m_strModelFileName);	//log
	os << m_colModel;
	os << m_vStretch;
	os << (DWORD)m_ePosition;
	os << (DWORD)m_eRotation;
	os << (BOOL)(m_pEffectControl != NULL);
	if(m_pEffectControl != NULL) m_pEffectControl->Write(&os);
}

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)