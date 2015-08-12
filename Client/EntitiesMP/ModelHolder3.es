242
%{
#include "StdH.h"
#include "EntitiesMP/WorldSettingsController.h"
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
#include <engine/Effect/CEffectGroup.h>
#include <engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CSkaTag.h>
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
%}

uses "EntitiesMP/ModelDestruction";
uses "EntitiesMP/AnimationChanger";
uses "EntitiesMP/BloodSpray";

// network model activate/deactivate
event EModel3Activate {
};
event EModel3Deactivate {
};
event EModel3Action {
};

%{
// #define MIPRATIO 0.003125f //(2*tan(90/2))/640
%}

class CModelHolder3 : CRationalEntity {
	name      "ModelHolder3";
	thumbnail "Thumbnails\\ModelHolder3.tbn";
	features "HasName", "HasDescription", "HasRaidObject", "RaidEvent", "AlignBackground";
	
properties:
/*** Export Data***/
	200 BOOL	m_bRaidObject	"This entity is RaidObject" = FALSE,		// 레이드 오브젝트 설정
	201 INDEX	m_RaidEventType		"Raid Event Type" = 0,			// 레이드 이벤트 타입
	202 BOOL	m_bObjectClick	"Object Click" = TRUE,	
/******************/
	1 CTFileName m_fnModel      "Model file (.smc)" 'M' = CTFILENAME("Data\\Models\\Editor\\Ska\\Axis.smc"), //("Data\\Models\\Editor\\Axis.mdl"),
	2 CTFileName m_fnTexture    "Texture" 'T' =CTFILENAME("Data\\Models\\Editor\\Vector.tex"),
	// 22 CTFileName m_fnReflection "Reflection" =CTString(""),
	// 23 CTFileName m_fnSpecular   "Specular" =CTString(""),
	// 24 CTFileName m_fnBump       "Bump" =CTString(""),
	3 FLOAT m_fStretchAll       "StretchAll" 'S' = 1.0f,
	4 ANGLE3D m_vStretchXYZ     "StretchXYZ" 'X' = FLOAT3D(1.0f, 1.0f, 1.0f),
	//5 FLOAT m_fStretchY         "StretchY"   'Y' = 1.0f,
	//6 FLOAT m_fStretchZ         "StretchZ"   'Z' = 1.0f,
	7 CTString m_strName        "Name" 'N' ="",
	12 CTString m_strDescription = "",
	8 BOOL m_bColliding       "Collision" 'L' = FALSE,    // set if model is not immatierial
	9 CTString m_strModelAnimation   "Model animation (SKA)" = "",
	// 10 ANIMATION m_iTextureAnimation "Texture animation" = 0,
	11 enum ShadowType m_stClusterShadows "Shadows" 'W' = ST_CLUSTER,   // set if model uses cluster shadows
	13 BOOL m_bBackground     "Background" 'B' = FALSE,   // set if model is rendered in background
	21 BOOL m_bTargetable     "Targetable" = FALSE, // st if model should be targetable
	
	// parameters for custom shading of a model (overrides automatic shading calculation)
	14 enum CustomShadingType m_cstCustomShading "Shading mode" 'H' = CST_NONE,
	15 ANGLE3D m_aShadingDirection "Shade. Light direction" 'D' = ANGLE3D( AngleDeg(45.0f),AngleDeg(45.0f),AngleDeg(45.0f)),
	16 COLOR m_colLight            "Shade. Light color" 'O' = C_WHITE,
	17 COLOR m_colAmbient          "Shade. Ambient color" 'A' = C_BLACK,
	18 FLOAT3D m_vShadingDirection = FLOAT3D(0,1,0),
	// 18 CTFileName m_fnmLightAnimation "Light animation file" = CTString(""),
	// 19 ANIMATION m_iLightAnimation "Light animation" = 0,
	// 20 CAnimObject m_aoLightAnimation,
	// 25 BOOL m_bAttachments      "Attachments" = TRUE, // set if model should auto load attachments*/
	26 BOOL m_bActive "Active" = TRUE features(EPROPF_NETSEND),
	28 COLOR m_colModelColor		"Model Color" = 0xFFFFFFFF,
	// 31 FLOAT m_fMipAdd "Mip Add" = 0.0f,
	// 32 FLOAT m_fMipMul "Mip Mul" = 1.0f,
	//33 FLOAT m_fMipFadeDist "Mip Fade Dist" = 0.0f,
	//34 FLOAT m_fMipFadeLen  "Mip Fade Len" = 0.0f,
	// 33 FLOAT m_fMipFadeDist = 0.0f,
	// 34 FLOAT m_fMipFadeLen  = 0.0f,
	35 RANGE m_rMipFadeDistMetric "Mip Fade Dist (Metric)" = -1.0f,
	36 FLOAT m_fMipFadeLenMetric  "Mip Fade Len (Metric)" = -1.0f,
		
	// random values variables
	50 BOOL m_bRandomStretch   "Apply RND stretch"   = FALSE, // apply random stretch
	52 ANGLE3D m_fStretchRndXYZ    "Stretch RND XYZ (%)"   =  FLOAT3D(0.2f, 0.2f, 0.2f), // random stretch width
	//51 FLOAT m_fStretchRndY    "Stretch RND Y (%)"   =  0.2f, // random stretch height
	//53 FLOAT m_fStretchRndZ    "Stretch RND Z (%)"   =  0.2f, // random stretch depth
	54 FLOAT m_fStretchRndAll  "Stretch RND All (%)" =  0.0f, // random stretch all
	55 FLOAT3D m_fStretchRandom = FLOAT3D(1, 1, 1),
	
	// destruction values
	60 CEntityPointer m_penDestruction "Destruction" 'Q' COLOR(C_BLACK|0x20),    // model destruction entity
	61 FLOAT3D m_vDamage = FLOAT3D(0,0,0),    // current damage impact
	62 FLOAT m_tmLastDamage = -1000.0f,
	63 CEntityPointer m_penDestroyTarget "Destruction Target" COLOR(C_WHITE|0xFF), // targeted when destroyed
	64 CEntityPointer m_penLastDamager,
	65 FLOAT m_tmSpraySpawned = 0.0f,   // time when damage has been applied
	66 FLOAT m_fSprayDamage = 0.0f,     // total ammount of damage
	67 CEntityPointer m_penSpray,       // the blood spray
	68 FLOAT m_fMaxDamageAmmount  = 0.0f, // max ammount of damage recived in in last xxx ticks
	
	70 FLOAT m_fClassificationStretch  "Classification stretch" = 1.0f, // classification box multiplier
	80 COLOR m_colBurning = COLOR(C_WHITE|CT_OPAQUE), // color applied when burning
	
	90 enum DamageType m_dmtLastDamageType=DMT_CHAINSAW,
	91 FLOAT m_fChainSawCutDamage    "Chain saw cut dammage" 'C' = 300.0f,
	//93 INDEX m_iFirstRandomAnimation "First random animation" 'R' = -1,
	100 FLOAT m_fMaxTessellationLevel "Max tessellation level" = 0.0f,
	
	110 BOOL m_bDoNotLoopAnimation "Animation looping OFF" = FALSE,
	//115 BOOL m_bEnemy "Is Enemy?" = FALSE,
	116 BOOL	m_bRenderReflection	"Render reflection?"	= FALSE,
	117 BOOL		m_bTerrainShadow "Terrain Shadow?"		= FALSE,
	118 CTString	m_strEffectGroupNameEvent "Model Effect Name(Event)" = "",
	119 BOOL		m_bDecorationObject "Decoration Object" = FALSE,
	120 BOOL		m_bHasRootTag	"Has __ROOT & __TOP tag" = FALSE,
	121 CTString	m_strEffectGroupName "Model Effect Name" = "",
	122 BOOL		m_AlignBackground "Object align background" = FALSE,
	123 BOOL		m_bChangeAnimActive	"change animation event" = FALSE, // 시작할때 애니메이션이 바로 동작하는것을 스킵한다.(changeanim event사용)
	124 CTString	m_strActionAnim  "Action animation name" = "",
	125 BOOL		m_bFadeOut = FALSE,
	{
		CEffectGroup *m_pModelEffectGroupEvent;
		CEffectGroup *m_pModelEffectGroup;
		FLOAT m_fFadeStartTime;
		FLOAT m_fFadeTime;
		FLOAT m_fSpiritStartTime; // time when spirit effect has started
	}
	
/*{
	CTFileName m_fnOldModel;  // used for remembering last selected model (not saved at all)
}*/

components:
	1 class    CLASS_BLOOD_SPRAY "Classes\\BloodSpray.ecl",
	20 skamodel EDITOR_AXIS_MODEL "Data\\Models\\Editor\\Ska\\Axis.smc", // just used for safety precaching
		
functions:
	void Precache(void) {
		PrecacheSkaModel(EDITOR_AXIS_MODEL);
		PrecacheClass(CLASS_BLOOD_SPRAY, 0);
		if(en_pmiModelInstance) { en_pmiModelInstance->m_tmSkaTagManager.SetOwner(this); }
		if(en_pmiModelInstance->mi_pmisSerial
		&& en_pmiModelInstance->mi_pmisSerial->mis_pmiModelInstance
		&& en_pmiModelInstance->mi_pmisSerial->mis_pmiModelInstance->m_fnmAnimEffectFile != "")
		{
			en_pmiModelInstance->ReadAnimEffectFile(en_pmiModelInstance->mi_pmisSerial->mis_pmiModelInstance->m_fnmAnimEffectFile);
			//en_pmiModelInstance->m_vectorAnimEffect = ((CModelInstance*)en_pmiModelInstance->mi_pmisSerial->mis_pmiModelInstance)->m_vectorAnimEffect;
		}
		GetModelInstance()->RefreshTagManager();
		if(m_bHasRootTag && GetModelInstance())
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			FLOATaabbox3D aabb;
			GetModelInstance()->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2), 0);
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
		}
		if( m_strModelAnimation != "" && !m_bChangeAnimActive)
		{
			INDEX iAnim = ska_GetIDFromStringTable(m_strModelAnimation);
			INDEX iDummy1, iDummy2;
			if (GetModelInstance()->FindAnimationByID(iAnim, &iDummy1, &iDummy2)) { 
				NewClearState(0.05f);
				ULONG ulFlags = AN_LOOPING;
				if (m_bDoNotLoopAnimation) {
					ulFlags = 0;
				}
				AddAnimation(iAnim, ulFlags, 1, 1);            
				OffSetAnimationQueue(FRnd()*10.0f);        
			} else {
				CPrintF("WARNING! Animation '%s' not found in SKA entity '%s'!\n",
					m_strModelAnimation, GetName());
			}
		}
		if(GetModelInstance() && m_strEffectGroupName != "")
		{
			if(CEffectGroupManager::Instance().IsValidCreated(m_pModelEffectGroup))
			{
				m_pModelEffectGroup->Stop(2); // WSS_DRATAN_SEIGEWARFARE 2007/08/31 수정
			}
			m_pModelEffectGroup = StartEffectGroup(m_strEffectGroupName
												, &GetModelInstance()->m_tmSkaTagManager
												, _pTimer->GetLerpedCurrentTick() + rand() / RAND_MAX);
		}
		else
		{
			m_pModelEffectGroup = NULL;
		}

		if (m_bRaidObject && m_bObjectClick) // 레이드, 클릭 오브젝트는 타겟이 가능하다.(살려야 가능하게 된다.)
		{
			SetFlagOn(ENF_ALIVE);
			SetFirstExtraFlagOn(ENF_EX1_CLICK_OBJECT);
		}
		else
		{
			SetFlagOff(ENF_ALIVE);
			SetFirstExtraFlagOff(ENF_EX1_CLICK_OBJECT);
		}
	};

	// render particles
	void RenderParticles(void) 
	{
		// no particles when not existing
		if (GetRenderType()!=CEntity::RT_MODEL && GetRenderType()!=CEntity::RT_SKAMODEL) 
		{
			return;
		}
		// if is dead
		if ( m_fSpiritStartTime > 0.0f && m_bFadeOut)
		{
			// 죽었을때 파티클 렌더링...
			 const FLOAT tmNow = _pTimer->CurrentTick();
			// Particles_ModelGlow(this, tmNow + 20,PT_STAR08, 0.15f, 2, 0.03f, 0xff00ff00);
			Particles_Death(this, m_fSpiritStartTime);
		}
	}
/*
	void Read_t(CTStream *istr, BOOL bNetwork)
	{
		CRationalEntity::Read_t(istr,bNetwork);
		if(m_bHasRootTag && GetModelInstance())
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			FLOATaabbox3D aabb;
			GetModelInstance()->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2), 0);
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
		}
		if( m_strModelAnimation != "")
		{
			INDEX iAnim = ska_GetIDFromStringTable(m_strModelAnimation);
			INDEX iDummy1, iDummy2;
			if (GetModelInstance()->FindAnimationByID(iAnim, &iDummy1, &iDummy2)) { 
				NewClearState(0.05f);
				ULONG ulFlags = AN_LOOPING;
				if (m_bDoNotLoopAnimation) {
					ulFlags = 0;
				}
				AddAnimation(iAnim, ulFlags, 1, 1);            
				OffSetAnimationQueue(FRnd()*10.0f);        
			} else {
				CPrintF("WARNING! Animation '%s' not found in SKA entity '%s'!\n",
					m_strModelAnimation, GetName());
			}
		}
	}
*/
	
	/* Fill in entity statistics - for AI purposes only */
	BOOL FillEntityStatistics(EntityStats *pes)
	{
		//pes->es_strName = m_fnModel.FileName()+", "+m_fnTexture.FileName();
		pes->es_strName = m_fnModel.FileName();
		
		pes->es_ctCount = 1;
		pes->es_ctAmmount = 1;
		if (m_penDestruction!=NULL) {
			pes->es_strName += " (destroyable)";
			pes->es_fValue = GetDestruction()->m_fHealth;
//			pes->es_iScore = 0;
		} else {
			pes->es_fValue = 0;
//			pes->es_iScore = 0;
		}
		return TRUE;
	}
	
	// classification box multiplier
	FLOAT3D GetClassificationBoxStretch(void)
	{
		return FLOAT3D( m_fClassificationStretch, m_fClassificationStretch, m_fClassificationStretch);
	}
	
	
	// maximum allowed tessellation level for this model (for Truform/N-Patches support)
	FLOAT GetMaxTessellationLevel(void)
	{
		return m_fMaxTessellationLevel;
	}
	
	
	/* Receive damage */
	void ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection) 
	{
		FLOAT fNewDamage = fDamageAmmount;
		
		// if not destroyable
		if (m_penDestruction==NULL) {
			// do nothing
			return;
		}
		if( dmtType==DMT_BURNING)
		{
			UBYTE ubR, ubG, ubB, ubA;
			ColorToRGBA(m_colBurning, ubR, ubG, ubB, ubA);
			ubR=ClampDn(ubR-4, 32);
			m_colBurning=RGBAToColor(ubR, ubR, ubR, ubA);
		}
		
		CModelDestruction *penDestruction = GetDestruction();
		// adjust damage
		fNewDamage *=DamageStrength(penDestruction->m_eibtBodyType, dmtType);
		// if no damage
		if (fNewDamage==0) {
			// do nothing
			return;
		}
		FLOAT fKickDamage = fNewDamage;
		if( (dmtType == DMT_EXPLOSION) || (dmtType == DMT_IMPACT)/* || (dmtType == DMT_CANNONBALL_EXPLOSION) */)
		{
			fKickDamage*=1.5f;
		}
		if (dmtType == DMT_CLOSERANGE) {
			fKickDamage=0.0f;
		}
		if (dmtType == DMT_CHAINSAW) {
			fKickDamage=0.0f;
		}    
		if((dmtType == DMT_BULLET || dmtType == DMT_SNIPER || dmtType == DMT_SHOTGUN)
			&& penDestruction->m_eibtBodyType==EIBT_ROCK) {
			fKickDamage=0.0f;
		}
		if( dmtType==DMT_BURNING)
		{
			fKickDamage=0.0f;
		}
		
		// get passed time since last damage
		TIME tmNow = _pTimer->CurrentTick();
		TIME tmDelta = tmNow-m_tmLastDamage;
		m_tmLastDamage = tmNow;
		
		// remember who damaged you
		m_penLastDamager = penInflictor;
		
		// fade damage out
		if (tmDelta>=_pTimer->TickQuantum*3) {
			m_vDamage=FLOAT3D(0,0,0);
		}
		// add new damage
		FLOAT3D vDirectionFixed;
		if (vDirection.ManhattanNorm()>0.5f) {
			vDirectionFixed = vDirection;
		} else {
			vDirectionFixed = FLOAT3D(0,1,0);
		}
		FLOAT3D vDamageOld = m_vDamage;
		m_vDamage += vDirectionFixed*fKickDamage;
		
		// NOTE: we don't receive damage here, but handle death differently
		if (m_vDamage.Length()>GetHealth()) {
			if (!penDestruction->m_bRequireExplosion || 
				dmtType==DMT_EXPLOSION/* || dmtType==DMT_CANNONBALL || dmtType==DMT_CANNONBALL_EXPLOSION*/)
			{
				EDeath eDeath;  // we don't need any extra parameters
				SendEvent(eDeath);
				//remember last dammage type
				m_dmtLastDamageType=dmtType;
			}
		}
		
		if( m_fMaxDamageAmmount<fDamageAmmount) {
			m_fMaxDamageAmmount = fDamageAmmount;
		}
		
		// if it has no spray, or if this damage overflows it
		if( (dmtType!=DMT_BURNING) && (dmtType!=DMT_CHAINSAW) &&
			(m_tmSpraySpawned<=_pTimer->CurrentTick()-_pTimer->TickQuantum*8 || 
			m_fSprayDamage+fNewDamage>50.0f))
		{
			// spawn blood spray
			CPlacement3D plSpray = CPlacement3D( vHitPoint, ANGLE3D(0, 0, 0));
			m_penSpray = CreateEntity( plSpray, CLASS_BLOOD_SPRAY);
			m_penSpray->SetParent( this);
			ESpawnSpray eSpawnSpray;
			
			// adjust spray power
			if( m_fMaxDamageAmmount > 10.0f) {
				eSpawnSpray.fDamagePower = 3.0f;
			} else if(m_fSprayDamage+fNewDamage>50.0f) {
				eSpawnSpray.fDamagePower = 2.0f;
			} else {
				eSpawnSpray.fDamagePower = 1.0f;
			}
			
			eSpawnSpray.sptType = penDestruction->m_sptType;
			eSpawnSpray.fSizeMultiplier = penDestruction->m_fParticleSize;
			
			// get your down vector (simulates gravity)
			FLOAT3D vDn(-en_mRotation(1,2), -en_mRotation(2,2), -en_mRotation(3,2));
			
			// setup direction of spray
			FLOAT3D vHitPointRelative = vHitPoint - GetPlacement().pl_PositionVector;
			FLOAT3D vReflectingNormal;
			GetNormalComponent( vHitPointRelative, vDn, vReflectingNormal);
			vReflectingNormal.Normalize();
			
			FLOAT3D vProjectedComponent = vReflectingNormal*(vDirection%vReflectingNormal);
			FLOAT3D vSpilDirection = vDirection-vProjectedComponent*2.0f-vDn*0.5f;
			
			eSpawnSpray.vDirection = vSpilDirection;
			eSpawnSpray.eidOwner = this;
			eSpawnSpray.colCentralColor=penDestruction->m_colParticles;
			eSpawnSpray.colBurnColor=m_colBurning;
			eSpawnSpray.fLaunchPower=penDestruction->m_fParticleLaunchPower;
			// initialize spray
			m_penSpray->Initialize( eSpawnSpray);
			m_tmSpraySpawned = _pTimer->CurrentTick();
			m_fSprayDamage = 0.0f;
			m_fMaxDamageAmmount = 0.0f;
		}
		
		if( dmtType==DMT_CHAINSAW && m_fChainSawCutDamage>0)
		{
			m_fChainSawCutDamage-=fDamageAmmount;
			if(m_fChainSawCutDamage<=0)
			{
				EDeath eDeath;  // we don't need any extra parameters
				SendEvent(eDeath);
				//remember last dammage type
				m_dmtLastDamageType=dmtType;
			}
		}
		
		m_fSprayDamage+=fNewDamage;
	};
	
	// Entity info
	void *GetEntityInfo(void) {
		CModelDestruction *pmd=GetDestruction();
		if( pmd!=NULL)
		{
			return GetStdEntityInfo(pmd->m_eibtBodyType);
		}
		return CEntity::GetEntityInfo();
	};
	
	class CModelDestruction *GetDestruction(void)
	{
		ASSERT( m_penDestruction==NULL || IsOfClass( m_penDestruction, &CModelDestruction_DLLClass));
		return (CModelDestruction*)&*m_penDestruction;
	}
	BOOL IsTargetable(void) const
	{
		return m_bTargetable;
	}
	
	/* Adjust model mip factor if needed. */
	void AdjustMipFactor(FLOAT &fMipFactor)
	{
		// if should fade last mip
		//if (m_rMipFadeDistMetric>0) {
		// this was changed due to an error in conversion of mipfactors <=0.0f
		// if it causes any problems, return to previous value
		if (m_rMipFadeDistMetric>0.5f) {
			CModelInstance *pmi = GetModelInstance();
			if(pmi==NULL) {
				return;
			}
			
			ENGINE_API extern FLOAT ska_fLODMul;
			ENGINE_API extern FLOAT ska_fLODAdd;
			
			// adjust for stretch
			FLOAT fMipDistNow = fMipFactor;
			FLOAT fMipFadeDist = m_rMipFadeDistMetric*ska_fLODMul+ska_fLODAdd;
			
			// if not visible
			if (fMipDistNow>fMipFadeDist) {
				// set mip factor so that model is never rendered
				fMipFactor = UpperLimit(0.0f);
				return;
			}
			
			// adjust fading
			FLOAT fFade = (fMipFadeDist-fMipDistNow);
			if (m_fMipFadeLenMetric>0) {
				fFade/=m_fMipFadeLenMetric;
			} else {
				if (fFade>0) {
					fFade = 1.0f;
				}
			}
			
			fFade = Clamp(fFade, 0.0f, 1.0f);
			// make it invisible
			pmi->SetModelColor((pmi->GetModelColor()&~255)|UBYTE(255*fFade));
		}
	}
	
	/* Adjust model shading parameters if needed. */
	BOOL AdjustShadingParameters(FLOAT3D &vLightDirection, COLOR &colLight, COLOR &colAmbient)
	{
		if (m_bFadeOut) 
		{
			FLOAT fTimeRemain = m_fFadeStartTime + m_fFadeTime - _pTimer->CurrentTick();
			if( fTimeRemain < 0.0f) { fTimeRemain = 0.0f; }
			COLOR colAlpha;
			if(en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) 
			{
				colAlpha = GetModelInstance()->GetModelColor();
				colAlpha = (colAlpha&0xFFFFFF00) + (COLOR(fTimeRemain/m_fFadeTime*0xFF)&0xFF);
				GetModelInstance()->SetModelColor(colAlpha);
			}
			else 
			{
				colAlpha = GetModelObject()->mo_colBlendColor;
				colAlpha = (colAlpha&0xFFFFFF00) + (COLOR(fTimeRemain/m_fFadeTime*0xFF)&0xFF);
				GetModelObject()->mo_colBlendColor = colAlpha;
			}
		}

		switch(m_cstCustomShading)
		{
		case CST_FULL_CUSTOMIZED:
			{
				colLight   = m_colLight;
				colAmbient = m_colAmbient;
				// obtain world settings controller
				CWorldSettingsController *pwsc = GetWSC(this);
				if( pwsc!=NULL && pwsc->m_bApplyShadingToModels) {
					// apply animating shading
					COLOR colShade = GetWorld()->wo_atbTextureBlendings[9].tb_colMultiply;
					colLight=MulColors(colLight, colShade);
					colAmbient=MulColors(colAmbient, colShade);
				}
				vLightDirection = m_vShadingDirection;
				SetFlagOn(ENF_CUSTOMSHADING); // signal to engine
				break;
			}
		case CST_CONSTANT_SHADING:
			{
				// combine colors with clamp
				UBYTE lR,lG,lB,aR,aG,aB,rR,rG,rB;
				ColorToRGB( colLight,   lR, lG, lB);
				ColorToRGB( colAmbient, aR, aG, aB);
				colLight = 0;
				rR = (UBYTE) Clamp( (ULONG)lR+aR, (ULONG)0, (ULONG)255);
				rG = (UBYTE) Clamp( (ULONG)lG+aG, (ULONG)0, (ULONG)255);
				rB = (UBYTE) Clamp( (ULONG)lB+aB, (ULONG)0, (ULONG)255);
				colAmbient = RGBToColor( rR, rG, rB);
				SetFlagOff(ENF_CUSTOMSHADING); // signal to engine
				break;
			}
		case CST_NONE:
			{
				SetFlagOff(ENF_CUSTOMSHADING); // signal to engine
				break;
			}
		}
		
		if( m_colBurning!=COLOR(C_WHITE|CT_OPAQUE)) {
			colAmbient = MulColors( colAmbient, m_colBurning);
			colLight   = MulColors( colLight,   m_colBurning);
			return TRUE;
		}
		// done
		return m_stClusterShadows!=ST_NONE;
	};
	
	
	// apply mirror and stretch to the entity
	void MirrorAndStretch(FLOAT fStretch, BOOL bMirrorX)
	{
		m_fStretchAll*=fStretch;
		if (bMirrorX) {
			m_vStretchXYZ(1)=-m_vStretchXYZ(1);
		}
	}
	
	
	// Stretch model
	void StretchModel(void) {
		// stretch factors must not have extreme values
		if (Abs(m_vStretchXYZ(1))  < 0.01f) { m_vStretchXYZ(1)   = 0.01f;  }
		if (Abs(m_vStretchXYZ(2))  < 0.01f) { m_vStretchXYZ(2)   = 0.01f;  }
		if (Abs(m_vStretchXYZ(3))  < 0.01f) { m_vStretchXYZ(3)   = 0.01f;  }
		if (m_fStretchAll< 0.01f) { m_fStretchAll = 0.01f;  }
		
		if (Abs(m_vStretchXYZ(1))  >1000.0f) { m_vStretchXYZ(1)   = 1000.0f*Sgn(m_vStretchXYZ(1)); }
		if (Abs(m_vStretchXYZ(2))  >1000.0f) { m_vStretchXYZ(2)   = 1000.0f*Sgn(m_vStretchXYZ(2)); }
		if (Abs(m_vStretchXYZ(3))  >1000.0f) { m_vStretchXYZ(3)   = 1000.0f*Sgn(m_vStretchXYZ(3)); }
		if (m_fStretchAll>1000.0f) { m_fStretchAll = 1000.0f; }
		
		if (m_bRandomStretch) {
			m_bRandomStretch = FALSE;
			// stretch
			m_fStretchRndXYZ = FLOAT3D( Clamp( m_fStretchRndXYZ(1), 0.0f, 1.0f),
				Clamp( m_fStretchRndXYZ(2), 0.0f, 1.0f),
				Clamp( m_fStretchRndXYZ(3), 0.0f, 1.0f));
			m_fStretchRndAll = Clamp( m_fStretchRndAll , 0.0f, 1.0f);
			
			m_fStretchRandom(1) = (FRnd()*m_fStretchRndXYZ(1)*2 - m_fStretchRndXYZ(1)) + 1;
			m_fStretchRandom(2) = (FRnd()*m_fStretchRndXYZ(2)*2 - m_fStretchRndXYZ(2)) + 1;
			m_fStretchRandom(3) = (FRnd()*m_fStretchRndXYZ(3)*2 - m_fStretchRndXYZ(3)) + 1;
			
			FLOAT fRNDAll = (FRnd()*m_fStretchRndAll*2 - m_fStretchRndAll) + 1;
			m_fStretchRandom(1) *= fRNDAll;
			m_fStretchRandom(2) *= fRNDAll;
			m_fStretchRandom(3) *= fRNDAll;
		}
//안태훈 수정 시작	//(Easy Use World Editor)(0.1)
		FLOAT3D vStretch;
		vStretch(1) = m_vStretchXYZ(1) * m_fStretchAll * m_fStretchRandom(1);
		vStretch(2) = m_vStretchXYZ(2) * m_fStretchAll * m_fStretchRandom(2);
		vStretch(3) = m_vStretchXYZ(3) * m_fStretchAll * m_fStretchRandom(3);
		
		GetModelInstance()->StretchModel( vStretch );
//안태훈 수정 끝	//(Easy Use World Editor)(0.1)
		ModelChangeNotify();
	};
	
	
	/* Init model holder*/
	void InitModelHolder(void) {
		
		// must not crash when model is removed
		if (m_fnModel=="") {
			m_fnModel=CTFILENAME("Data\\Models\\Editor\\Ska\\Axis.smc");
		}
		
		if (m_bActive) {
			InitAsSkaModel();
		} else {
			InitAsSkaEditorModel();
		}
		
		BOOL bLoadOK = TRUE;
		// try to load the model
		try {
			SetSkaModel_t(m_fnModel);
			// if failed
		} catch(char *) {            
			bLoadOK = FALSE;
			// set colision info for default model
			//SetSkaColisionInfo();
		}
		if (!bLoadOK) {
			SetSkaModel(CTFILENAME("Data\\Models\\Editor\\Ska\\Axis.smc"));
			WarningMessage(TRANS("Cannot load ska model '%s'"), (CTString&)m_fnModel);
		}
		
		// set model stretch
		StretchModel();
		ModelChangeNotify();
		
		if (m_bColliding&&m_bActive) {
			SetPhysicsFlags(EPF_MODEL_FIXED);
			SetCollisionFlags(ECF_MODEL_HOLDER);
		} else {
			SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
			SetCollisionFlags(ECF_IMMATERIAL);
		}
		
		switch(m_stClusterShadows) {
		case ST_NONE:
			{
				SetFlags(GetFlags()&~ENF_CLUSTERSHADOWS);
				//강동민 수정 시작
				SetFlags(GetFlags()&~ENF_PROJECTIONSHADOWS);
				//강동민 수정 끝
				//SetFlags(GetFlags()&~ENF_POLYGONALSHADOWS);
				break;
			}
		case ST_CLUSTER:
			{
				SetFlags(GetFlags()|ENF_CLUSTERSHADOWS);
				//강동민 수정 시작
				SetFlags(GetFlags()&~ENF_PROJECTIONSHADOWS);
				//강동민 수정 끝
				//SetFlags(GetFlags()&~ENF_POLYGONALSHADOWS);
				break;
			}
		case ST_POLYGONAL:
			{
				//SetFlags(GetFlags()|ENF_POLYGONALSHADOWS);
				//강동민 수정 시작
				SetFlags(GetFlags()&~ENF_PROJECTIONSHADOWS);
				//강동민 수정 끝
				SetFlags(GetFlags()&~ENF_CLUSTERSHADOWS);
				break;
			}
			//강동민 수정 시작
		case ST_PROJECTION:
			{
//안태훈 수정 시작	//(Add Sun Moon Entity and etc)(0.2)
				GetModelInstance()->mi_bRenderShadow = TRUE;
//안태훈 수정 끝	//(Add Sun Moon Entity and etc)(0.2)
				SetFlags(GetFlags()|ENF_PROJECTIONSHADOWS);
				break;
			}
			//강동민 수정 끝
		}
		
		if (m_bBackground) {
			SetFlags(GetFlags()|ENF_BACKGROUND);
		} else {
			SetFlags(GetFlags()&~ENF_BACKGROUND);
		}
		
		if (m_penDestruction==NULL) {
			m_strDescription.PrintF("%s undestroyable", (CTString&)m_fnModel.FileName());
		} else {
			m_strDescription.PrintF("%s -> %s", (CTString&)m_fnModel.FileName(), m_penDestruction->GetName());
		}
		
		m_strDescription.PrintF("%s", (CTString&)m_fnModel.FileName());
		
		AnglesToDirectionVector(m_aShadingDirection, m_vShadingDirection);
		m_vShadingDirection = -m_vShadingDirection;

		return;
	};
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	void SetModelColor(const COLOR color)
	{
		GetModelInstance()->SetModelColor(color);
	}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
	
procedures:
	Main()
	{
//강동민 수정 시작 접속 시퀀스 작업	05.20
/*
		if(m_bEnemy)
		{
			SetFlagOn(ENF_ENEMY);
			m_bActive = FALSE;	
		}
		*/
//강동민 수정 끝 접속 시퀀스 작업	05.20
		SetFlagOn(ENF_MARKDESTROY);
		SetFlagOn(ENF_NONETCONNECT);
		SetFlagOff(ENF_PROPSCHANGED);
//강동민 수정 시작 테스트 클라이언트 작업	06.30
		SetFlagOff(ENF_RENDERREFLECTION);
//강동민 수정 끝 테스트 클라이언트 작업		06.30
//강동민 수정 시작 로그인 처리 작업	07.20
		SetFirstExtraFlagOff(ENF_EX1_TERRAINSHADOW);
//강동민 수정 끝 로그인 처리 작업		07.20

		m_fFadeStartTime = 0.0f;
		m_fFadeTime = 0.0f;
		m_bFadeOut = FALSE;
		m_fSpiritStartTime = 0.0f; // time when spirit effect has started

		// initialize the model
		InitModelHolder();

//안태훈 수정 시작	//(For Performance)(0.1)
		SetFirstExtraFlagOff(ENF_EX1_DECORATION);
		if(m_bDecorationObject) { SetFirstExtraFlagOn(ENF_EX1_DECORATION); }
//안태훈 수정 끝	//(For Performance)(0.1)

//강동민 수정 시작 테스트 클라이언트 작업	06.30
		if(m_bRenderReflection)
		{
			SetFlagOn(ENF_RENDERREFLECTION);			
		}
//강동민 수정 끝 테스트 클라이언트 작업		06.30
		
//강동민 수정 시작 로그인 처리 작업	07.20
		if(m_bTerrainShadow)
		{
			SetFirstExtraFlagOn(ENF_EX1_TERRAINSHADOW);
		}
//강동민 수정 끝 로그인 처리 작업		07.20
		
		if (m_bRaidObject && m_bObjectClick) // 레이드, 클릭 오브젝트는 타겟이 가능하다.(살려야 가능하게 된다.)
		{
			SetFlagOn(ENF_ALIVE);
			SetFirstExtraFlagOn(ENF_EX1_CLICK_OBJECT);
		}
		
//안태훈 수정 시작	//(Add Sun Moon Entity and etc)(0.2)
		GetModelInstance()->SetModelColor(m_colModelColor);
//안태훈 수정 끝	//(Add Sun Moon Entity and etc)(0.2)

//안태훈 수정 시작	//(Open beta)(2004-12-13)
		if(m_bHasRootTag && GetModelInstance())
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			FLOATaabbox3D aabb;
			GetModelInstance()->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2), 0);
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
		}
//안태훈 수정 끝	//(Open beta)(2004-12-13)

		// check your destruction pointer
		if( m_penDestruction!=NULL && !IsOfClass( m_penDestruction, &CModelDestruction_DLLClass)) {
			WarningMessage("Destruction '%s' is wrong class!", m_penDestruction->GetName());
			m_penDestruction=NULL;
		}
		
		// warning! because of this auto wait domino effect destruction with chainsaw didn't work
		//autowait(0.05f);
		
		// start ska animations if needed
		if( m_strModelAnimation != "")
		{
			INDEX iAnim = ska_GetIDFromStringTable(m_strModelAnimation);
			INDEX iDummy1, iDummy2;
			if (GetModelInstance()->FindAnimationByID(iAnim, &iDummy1, &iDummy2)) { 
				NewClearState(0.05f);
				ULONG ulFlags = AN_LOOPING;
				if (m_bDoNotLoopAnimation) {
					ulFlags = 0;
				}
				AddAnimation(iAnim, ulFlags, 1, 1);            
				OffSetAnimationQueue(FRnd()*10.0f);        
			} else {
				CPrintF("WARNING! Animation '%s' not found in SKA entity '%s'!\n",
					m_strModelAnimation, GetName());
			}
		}
		
		// [080201: Su-won] 다른 이펙트가 존재하면 새로운 이펙트를 적용하기 전에 없앰.
		if( m_pModelEffectGroup )
		{
			DestroyEffectGroup(m_pModelEffectGroup);
		}

		if(GetModelInstance() && m_strEffectGroupName != "")
		{
			if(CEffectGroupManager::Instance().IsValidCreated(m_pModelEffectGroup))
			{
				m_pModelEffectGroup->Stop(0);
			}
			m_pModelEffectGroup = StartEffectGroup(m_strEffectGroupName
												, &GetModelInstance()->m_tmSkaTagManager
												, _pTimer->GetLerpedCurrentTick() + rand() / RAND_MAX);
		}
		else
		{
			m_pModelEffectGroup = NULL;
		}

		// wait forever
		wait() {
			// on the beginning
			on(EBegin): {
				// set your health
				if (m_penDestruction!=NULL) {
					SetHealth(GetDestruction()->m_fHealth);
				}
				resume;
			}
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
			on(EStart):
			{
				if(m_strEffectGroupNameEvent != "" && en_pmiModelInstance != NULL)
				{
					DestroyEffectGroup(m_pModelEffectGroupEvent);
					m_pModelEffectGroupEvent = StartEffectGroup(m_strEffectGroupNameEvent, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				resume;
			}
			on(EStop):
			{
				if(m_strEffectGroupNameEvent != "" && en_pmiModelInstance != NULL)
				{
					if(CEffectGroupManager::Instance().IsValidCreated(m_pModelEffectGroupEvent))
					{
						m_pModelEffectGroupEvent->Stop(0); // WSS_DRATAN_SEIGEWARFARE 2007/08/30 바로 사라지게 처리 10 -> 0
						m_pModelEffectGroupEvent = NULL;
					}
				}

				if (m_strEffectGroupName != "" && en_pmiModelInstance != NULL)
				{
					if(CEffectGroupManager::Instance().IsValidCreated(m_pModelEffectGroup))
					{
						m_pModelEffectGroup->Stop(0);
						m_pModelEffectGroup = NULL;
					}
				}

				resume;
			}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
			// activate/deactivate shows/hides model
			on (EActivate): {
				if (_pNetwork->IsServer()) {
					SendEvent(EModel3Activate(),TRUE);
				}
				resume;
			}
			on (EModel3Activate): {       
				SetFlagOn(ENF_PROPSCHANGED);
				SwitchToModel();
				m_bActive = TRUE;
				if (m_bColliding) {
					SetPhysicsFlags(EPF_MODEL_FIXED);
					SetCollisionFlags(ECF_MODEL_HOLDER);
				}
				resume;
			}
			on (EDeactivate): {
				if (_pNetwork->IsServer()) {
					SendEvent(EModel3Deactivate(),TRUE);
				}
				resume;
			}
			on (EModel3Deactivate): {
				SetFlagOn(ENF_PROPSCHANGED);
				SwitchToEditorModel();
				SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
				SetCollisionFlags(ECF_IMMATERIAL);
				m_bActive = FALSE;
				resume;
			}
			// when your parent is destroyed
			on(ERangeModelDestruction): {
				// for each child of this entity
				{FOREACHINLIST(CEntity, en_lnInParent, en_lhChildren, itenChild) {
					// send it destruction event
					itenChild->SendEvent(ERangeModelDestruction(),TRUE);
				}}
				// destroy yourself
				Destroy(FALSE);
				resume;
			}
			// when dead
			on(EDeath): {
				SendEvent(EModelDeath(),TRUE);
				if (_pNetwork->IsServer()) {
					if (m_penDestruction!=NULL) {
						jump Die();
					}
				}
				resume;
			}
			on(EModelDeath): {
				SendEvent(EModelDeath(),TRUE);
				if (!_pNetwork->IsServer()) {
					if (m_penDestruction!=NULL) {
						jump Die();
					}
				}
				resume;
			}
			// when animation should be changed
			on(EChangeAnim eChange): {

				if (eChange.AnimType == ACTION_NONE)
				{
					m_strModelAnimation = eChange.strAnimation;
					if( m_strModelAnimation != "")
					{
						INDEX iAnim = ska_GetIDFromStringTable(m_strModelAnimation);
						INDEX iDummy1, iDummy2;
						if (GetModelInstance()->FindAnimationByID(iAnim, &iDummy1, &iDummy2)) { 
							NewClearState(eChange.fBlendTime);
							ULONG ulFlags = 0;
							if (eChange.bModelLoop) {
								ulFlags = AN_LOOPING;
							}
							AddAnimation(iAnim, ulFlags, 1, 1);            
						} else {
							CPrintF("WARNING! Animation '%s' not found in SKA entity '%s'!\n",
								m_strModelAnimation, GetName());
						}
					} 
				}
				else if (eChange.AnimType == ACTION_SKILL)
				{
					m_strActionAnim = eChange.strAnimation;
					call ModelAction();
				}
				else if (eChange.AnimType == ACTION_DISAPPEAR)
				{
					m_strActionAnim = eChange.strAnimation;
					m_fSpiritStartTime = _pTimer->CurrentTick();
					m_fFadeStartTime = _pTimer->CurrentTick();
					m_fFadeTime = 4.0f;
					m_bFadeOut = TRUE;
					SetFlagOff(ENF_NOPARTICLES);
					call ModelAction();
				}
				else if (eChange.AnimType == ACTION_APPEAR)
				{
					if (m_bFadeOut)
					{
						m_bFadeOut = FALSE;
						SetFlagOn(ENF_NOPARTICLES);
						COLOR colAlpha;
						if(en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) 
						{
							colAlpha = GetModelInstance()->GetModelColor();
							colAlpha = (colAlpha|0x000000FF);
							GetModelInstance()->SetModelColor(colAlpha);
						}
						else 
						{
							colAlpha = GetModelObject()->mo_colBlendColor;
							colAlpha = (colAlpha|0x000000FF);
							GetModelObject()->mo_colBlendColor = colAlpha;
						}
					}
				}

				resume;
			}
			// received trigger event message from server
			on (ETrigger): {
				if (m_strActionAnim != "")
				{
					// toggle 처리
					if (m_bFadeOut)
					{
						m_bFadeOut = FALSE;
						SetFlagOn(ENF_NOPARTICLES);
						COLOR colAlpha;
						if(en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) 
						{
							colAlpha = GetModelInstance()->GetModelColor();
							colAlpha = (colAlpha|0x000000FF);
							GetModelInstance()->SetModelColor(colAlpha);
						}
						else 
						{
							colAlpha = GetModelObject()->mo_colBlendColor;
							colAlpha = (colAlpha|0x000000FF);
							GetModelObject()->mo_colBlendColor = colAlpha;
						}
					}
					else if (m_strActionAnim != "")
					{
						SendEvent(EModel3Action());
					}
				}
				else if (m_bChangeAnimActive)
				{ // 애니메이션을 동작하여 활성화 되는 모습을 연출한다. (이펙트는 모델애니메이션정보에 넣고, 애니메이션에서 이펙트를 활성화 한다.)

					if (m_strModelAnimation != "")
					{
						INDEX iAnim = ska_GetIDFromStringTable(m_strModelAnimation);
						INDEX iDummy1, iDummy2;
						if (GetModelInstance()->FindAnimationByID(iAnim, &iDummy1, &iDummy2)) { 
							NewClearState(0.05f);
							ULONG ulFlags = AN_LOOPING;
							if (m_bDoNotLoopAnimation) {
								ulFlags = 0;
							}
							AddAnimation(iAnim, ulFlags, 1, 1);            
							OffSetAnimationQueue(FRnd()*10.0f);        
						} else {
							CPrintF("WARNING! Animation '%s' not found in SKA entity '%s'!\n",
								m_strModelAnimation, GetName());
						}
					}

					SetFlagOff(ENF_ALIVE); // 비활성화

/*					if(m_strEffectGroupNameEvent != "" && en_pmiModelInstance != NULL)
					{
						DestroyEffectGroup(m_pModelEffectGroupEvent);
						m_pModelEffectGroupEvent = StartEffectGroup(m_strEffectGroupNameEvent, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
					}*/
					SendEvent(EStart()); // 이펙트 활성화
				}
				else
				{ // 활성화된 애니메이션 동작에서 비활성화되는 오브젝트로 연출한다.( 충돌처리 비활성화 및 이펙트 소거)
					if (m_bObjectClick)
					{
						SetFlagOff(ENF_ALIVE); // 총돌처리 비활성화
					}

					SendEvent(EStop()); // 이펙트 소거
				}

				resume;
			}
			on (EModel3Action): {
				/* test ///////////////////////////////////
				m_fSpiritStartTime = _pTimer->CurrentTick();
				m_fFadeStartTime = _pTimer->CurrentTick();
				m_fFadeTime = 4.0f;
				m_bFadeOut = TRUE;
				SetFlagOff(ENF_NOPARTICLES);
				/////////////////////////////////////////*/
				call ModelAction();
				resume;
			}
			/*on (EReturn): {
				SendEvent(EModel3Action());
				resume;
			}*/
			otherwise(): {
				resume;
			}
		};
	}

	Die()
	{
		// for each child of this entity
		{FOREACHINLIST(CEntity, en_lnInParent, en_lhChildren, itenChild) {
			// send it destruction event
			itenChild->SendEvent(ERangeModelDestruction(),TRUE);
		}}
		
		// spawn debris 
		CModelDestruction *pmd=GetDestruction();
		pmd->SpawnDebris(this);
		// if there is another phase in destruction
		CEntity *penNext = pmd->GetNextPhase();    
		if( penNext!=NULL) {
			// copy it here
			CEntity *penNew = GetWorld()->CopyEntityInWorld( *penNext, GetPlacement() );
			if( IsOfClass( penNew, &CModelHolder2_DLLClass)) {
				penNew->GetModelObject()->StretchModel(GetModelInstance()->mi_vStretch);
				penNew->ModelChangeNotify();
				((CModelHolder2 *)penNew)->m_colBurning=m_colBurning;
				((CModelHolder2 *)penNew)->m_fChainSawCutDamage=m_fChainSawCutDamage;
				
				if( pmd->m_iStartAnim!=-1)
				{
					penNew->GetModelObject()->PlayAnim(pmd->m_iStartAnim, 0);
				}
				
				// copy custom shading parameters
				CModelHolder2 &mhNew=*((CModelHolder2 *)penNew);
				mhNew.m_cstCustomShading=m_cstCustomShading;
				mhNew.m_colLight=m_colLight;
				mhNew.m_colAmbient=m_colAmbient;
				{ mhNew.m_fMipFadeDist = MipFactor_MetricToLog(m_rMipFadeDistMetric);
				mhNew.m_fMipFadeLen  = MipFactor_MetricToLog(m_fMipFadeLenMetric + m_rMipFadeDistMetric)
					- mhNew.m_fMipFadeDist;
				/*mhNew.m_fMipAdd = m_fMipAdd;*/
				/*mhNew.m_fMipMul = m_fMipMul;*/ }
				// domino death for chainsaw
				if(m_dmtLastDamageType==DMT_CHAINSAW)
				{
					EDeath eDeath;  // we don't need any extra parameters
					mhNew.m_fChainSawCutDamage=0.0f;
					mhNew.m_dmtLastDamageType=DMT_CHAINSAW;
					penNew->SendEvent(eDeath);
				}
			} else if( IsOfClass( penNew, &CModelHolder3_DLLClass)) {
				penNew->GetModelInstance()->StretchModel(GetModelInstance()->mi_vStretch);
				penNew->ModelChangeNotify();
				
				if( pmd->m_strStartAnim != "")
				{
					INDEX iAnim = ska_GetIDFromStringTable(pmd->m_strStartAnim);
					INDEX iDummy1, iDummy2;
					if (GetModelInstance()->FindAnimationByID(iAnim, &iDummy1, &iDummy2)) { 
						penNew->NewClearState(0.05f);
						penNew->AddAnimation(iAnim, 0, 1, 1);            
					}
				}
				
				// copy custom shading parameters
				CModelHolder3 &mhNew=*((CModelHolder3 *)penNew);
				mhNew.m_colBurning=m_colBurning;
				mhNew.m_fChainSawCutDamage=m_fChainSawCutDamage;
				mhNew.m_cstCustomShading=m_cstCustomShading;
				mhNew.m_colLight=m_colLight;
				mhNew.m_colAmbient=m_colAmbient;
				mhNew.m_rMipFadeDistMetric = m_rMipFadeDistMetric;
				mhNew.m_fMipFadeLenMetric  = m_fMipFadeLenMetric;        
				// domino death for chainsaw
				if(m_dmtLastDamageType==DMT_CHAINSAW)
				{
					EDeath eDeath;  // we don't need any extra parameters
					mhNew.m_fChainSawCutDamage=0.0f;
					mhNew.m_dmtLastDamageType=DMT_CHAINSAW;
					penNew->SendEvent(eDeath);
				}
			}      
		}
		
		/* currently, environment destruction does not yield score.
		update statistics, if score is re-enabled!
		// send score to who killed you
		if (m_penLastDamager!=NULL) {
		EReceiveScore eScore;
		eScore.fPoints = 10.0f;
		m_penLastDamager->SendEvent(eScore);
	}*/
		
		// if there is a destruction target
		if (m_penDestroyTarget!=NULL) {
			// notify it
			SendToTarget(m_penDestroyTarget, EET_TRIGGER, m_penLastDamager);
		}
		
		// destroy yourself
		Destroy(FALSE);
		return;
	}

	ModelAction(EVoid)
	{
		if (m_strActionAnim != "")
		{
			NewClearState(CLEAR_STATE_LENGTH);
			AddAnimation(ska_GetIDFromStringTable(m_strActionAnim), AN_NORESTART, 1, 1);
			autowait(GetModelInstance()->GetAnimLength(ska_GetIDFromStringTable(m_strActionAnim)));
		}

		INDEX iAnim = ska_GetIDFromStringTable(m_strModelAnimation);
		INDEX iDummy1, iDummy2;
		if (GetModelInstance()->FindAnimationByID(iAnim, &iDummy1, &iDummy2)) { 
			NewClearState(0.05f);
			ULONG ulFlags = AN_LOOPING;
			if (m_bDoNotLoopAnimation) {
				ulFlags = 0;
			}
			AddAnimation(iAnim, ulFlags, 1, 1);            
			OffSetAnimationQueue(FRnd()*10.0f);        
		} else {
			CPrintF("WARNING! Animation '%s' not found in SKA entity '%s'!\n",
				m_strModelAnimation, GetName());
		}

		return EReturn();
	}
};
