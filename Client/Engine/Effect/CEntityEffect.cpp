#include "StdH.h"
#include "CEntityEffect.h"

#include <Engine/Entities/Entity.h>
#include "CEffectControl.h"
#include <Engine/Ska/ModelInstance.h>

//NEW_DELETE_IMPLEMENT(CEntityEffect);

BOOL g_bEntityEffectTestMode = FALSE;

CEntityEffect::CEntityEffect()
: m_pEffectControl( NULL )
, m_pMIForTest( NULL )
{
	CEffect::m_eType = ET_ENTITY;
}

CEntityEffect::~CEntityEffect()
{
	if(m_pEffectControl) delete m_pEffectControl;
}

void CEntityEffect::SetEffectControl(const CEffectControl &effectControl)
{
	if(m_pEffectControl == NULL) m_pEffectControl = new CEffectControl;
	*m_pEffectControl = effectControl;
}

void CEntityEffect::EraseEffectControl()
{
	delete m_pEffectControl;
	m_pEffectControl = NULL;
}

void CEntityEffect::Start(FLOAT time, BOOL restart)
{
	if(!PreStart(time, restart)) return;
	if(m_eState != ES_NOT_STARTED) return;

	if(m_penOwner == NULL && !g_bEntityEffectTestMode) return;

	if(g_bEntityEffectTestMode)
	{
		if(m_pMIForTest == NULL) return;
		m_colModelOriginal = m_pMIForTest->mi_colModelColor;
		m_vStretchOriginal = m_pMIForTest->mi_vStretch;
	}
	else
	{
		if(m_penOwner->GetRenderType() == CEntity::RT_SKAMODEL
		&& m_penOwner->GetModelInstance())
		{
			m_colModelOriginal = m_penOwner->GetModelInstance()->mi_colModelColor;
			m_vStretchOriginal = m_penOwner->GetModelInstance()->mi_vStretch;
		}
		else return;
	}
	PostStart();
}

BOOL CEntityEffect::Process(FLOAT time)
{
	FLOAT fDeltaTime, fProcessedTime;
	BOOL bRet, bRender;
	if(!PreProcess(time, bRet, bRender, fDeltaTime, fProcessedTime))
	{
		if(m_pEffectControl)
		{
			CModelInstance *pMI = NULL;
			if(g_bEntityEffectTestMode) pMI = m_pMIForTest;
			else pMI = m_penOwner->GetModelInstance();
			if(pMI)
			{
				if(m_pEffectControl->GetAlphaCount())
				{
					UBYTE alpha = m_pEffectControl->GetAlphaValue(m_pEffectControl->GetAlphaCount()-1);
					pMI->mi_colModelColor = (pMI->mi_colModelColor & 0xFFFFFF00) | alpha;
				}
				if(m_pEffectControl->GetColorCount())
				{
					COLOR color = m_pEffectControl->GetColorValue(m_pEffectControl->GetColorCount()-1);
					pMI->mi_colModelColor = (color & 0xFFFFFF00) | (pMI->mi_colModelColor & 0x000000FF);
				}
				if(m_pEffectControl->GetStretchCount())
				{
					FLOAT3D stretch = m_pEffectControl->GetStretchValue(m_pEffectControl->GetStretchCount()-1);
					pMI->mi_vStretch = stretch;
				}
			}
		}
		if(!bRender) SetNotRenderAtThisFrame();
		return bRet;
	}

	COLOR colModel = C_WHITE;
	FLOAT3D vStretch(1,1,1), vPosition(0,0,0), vAngle(0,0,0);
	if(m_pEffectControl == NULL) return FALSE;
	else
	{
		m_pEffectControl->Process(&colModel, &vStretch, &vPosition, &vAngle
								, fProcessedTime, m_fLifeTime, fDeltaTime);
	}

	if(g_bEntityEffectTestMode)
	{
		if(m_pMIForTest == NULL) return FALSE;
		if(m_pEffectControl->GetAlphaCount())
		{
			UBYTE alpha = colModel & 0x000000FF;
			m_pMIForTest->mi_colModelColor = (m_pMIForTest->mi_colModelColor & 0xFFFFFF00) | alpha;
		}
		if(m_pEffectControl->GetColorCount())
		{
			COLOR color = colModel & 0xFFFFFF00;
			m_pMIForTest->mi_colModelColor = color | (m_pMIForTest->mi_colModelColor & 0x000000FF);
		}
		if(m_pEffectControl->GetStretchCount())
		{
			m_pMIForTest->mi_vStretch = vStretch;
		}
	}
	else
	{
		if(m_penOwner->GetRenderType() == CEntity::RT_SKAMODEL
		&& m_penOwner->GetModelInstance())
		{
			CModelInstance *pMI = m_penOwner->GetModelInstance();
			if( pMI )
			{
				if(m_pEffectControl->GetAlphaCount())
				{
					UBYTE alpha = colModel & 0x000000FF;
					pMI->mi_colModelColor = (pMI->mi_colModelColor & 0xFFFFFF00) | alpha;
				}
				if(m_pEffectControl->GetColorCount())
				{
					COLOR color = colModel & 0xFFFFFF00;
					pMI->mi_colModelColor = color | (pMI->mi_colModelColor & 0x000000FF);
				}
				if(m_pEffectControl->GetStretchCount())
				{
					pMI->mi_vStretch = vStretch;
				}
			}
		}
		else return FALSE;
	}
	PostProcess();
	return TRUE;
}

CEffect *CEntityEffect::Copy()
{
	CEntityEffect *pRet = new CEntityEffect;
	if(pRet == NULL) return NULL;
	//CEffectÀÇ content
	pRet->SetContent(this);
	//CEntityEffectÀÇ content
	if(m_pEffectControl) pRet->SetEffectControl(*m_pEffectControl);
	return pRet;
}

#include <Engine/Base/Stream.h>
#define CURRENT_VERSION 1

void CEntityEffect::Read(CTStream *pIS)
{
	ASSERT(pIS);
	CTStream &is = *pIS;
	CEffect::Read(pIS);

	is.ExpectID_t("EENT");
	UBYTE ubVer;
	is >> ubVer;

	if(ubVer == CURRENT_VERSION)
	{
		if(m_pEffectControl == NULL) m_pEffectControl = new CEffectControl;
		m_pEffectControl->Read(&is);
	}
	else
	{
		ASSERTALWAYS("Version Error, Effect");
	}
}

void CEntityEffect::Write(CTStream *pOS)
{
	ASSERT(pOS);
	CTStream &os = *pOS;
	CEffect::Write(pOS);

	os.WriteID_t("EENT");
	os << (UBYTE)CURRENT_VERSION;
	if(m_pEffectControl != NULL) m_pEffectControl->Write(&os);
}

