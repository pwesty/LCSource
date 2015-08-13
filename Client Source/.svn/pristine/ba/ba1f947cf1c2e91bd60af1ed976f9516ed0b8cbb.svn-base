608
%{
#include "StdH.h"
#include "EntitiesMP/Effector.h"
#include <EntitiesMP/BloodSpray.h>
%}

enum EffectorEffectType {
  0 ET_NONE                  "None",                      // no particles
  1 ET_DESTROY_OBELISK       "Destroy obelisk",           // effect of obelisk destroying
  2 ET_DESTROY_PYLON         "Destroy pylon",             // effect of pylon destroying
  3 ET_HIT_GROUND            "Hit ground",                // effect of hitting ground
  4 ET_LIGHTNING             "Lightning",                 // lightning effect
  5 ET_SIZING_BIG_BLUE_FLARE "Sizing big blue flare",     // sizing big blue flare with reflections effect
  6 ET_SIZING_RING_FLARE     "Sizing ring flare",         // sizing ringed flare effect
  7 ET_MOVING_RING           "Moving ring",               // moving ring effect
  8 ET_PORTAL_LIGHTNING      "Portal lightnings",         // lightnings between portal vertices
  9 ET_MORPH_MODELS          "Morph two models",          // morph wto models
 10 ET_DISAPPEAR_MODEL       "Disappear model",           // disappear model
 11 ET_APPEAR_MODEL          "Appear model",              // appear model
 12 ET_DISAPPEAR_MODEL_NOW   "Disappear model now",       // disappear model now
 13 ET_APPEAR_MODEL_NOW      "Appear model now",          // appear model now
};

// input parameter for spawning effector
event ESpawnEffector {
  enum EffectorEffectType eetType, // type of particles
  FLOAT3D vDamageDir,              // direction of damage
  FLOAT3D vDestination,            // FX Destination
  FLOAT tmLifeTime,                // FX's life period
  FLOAT fSize,                     // misc size
  INDEX ctCount,                   // misc count
  CEntityID eidModel,         // ptr to model object used in this effect
  CEntityID eidModel2,        // ptr to second model object used in this effect
};

event ETriggerEffector{
};

%{
void CEffector_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) 
{
  switch ((EffectorEffectType)iUser)
  {
  case ET_MOVING_RING           :
    pdec->PrecacheModel(MODEL_POWER_RING);
    pdec->PrecacheTexture(TEXTURE_POWER_RING);
    break;
  case ET_DESTROY_OBELISK       :
  case ET_DESTROY_PYLON         :
  case ET_HIT_GROUND            :
  case ET_LIGHTNING             :
  case ET_SIZING_BIG_BLUE_FLARE :
  case ET_SIZING_RING_FLARE     :
  case ET_PORTAL_LIGHTNING      :
  case ET_MORPH_MODELS          :
  case ET_DISAPPEAR_MODEL       :
  case ET_APPEAR_MODEL          :
  case ET_DISAPPEAR_MODEL_NOW   :
  case ET_APPEAR_MODEL_NOW      :
    // no precaching needed
    break;
  default:
    ASSERT(FALSE);
  }
}

// array for model vertices in absolute space
CStaticStackArray<FLOAT3D> avModelFXVertices;
%}

class CEffector: CMovableModelEntity {
name      "Effector";
thumbnail "";
features "ImplementsOnPrecache";
properties:

  1 enum EffectorEffectType m_eetType = ET_NONE features(EPROPF_NETSEND), // type of effect
  2 FLOAT m_tmStarted = 0.0f,                                        // time when spawned
  3 FLOAT3D m_vDamageDir = FLOAT3D(0,0,0)  features(EPROPF_NETSEND),     // direction of damage
  4 FLOAT3D m_vFXDestination = FLOAT3D(0,0,0) features(EPROPF_NETSEND),  // FX destination
  5 FLOAT m_tmLifeTime = 5.0f features(EPROPF_NETSEND),                 // how long effect lives
  6 FLOAT m_fSize = 1.0f features(EPROPF_NETSEND),                      // effect's stretcher
  8 INDEX m_ctCount = 0 features(EPROPF_NETSEND),                                             // misc count
 10 BOOL m_bLightSource = FALSE,                                     // effect is also light source
 11 CAnimObject m_aoLightAnimation,                                  // light animation object
 12 INDEX m_iLightAnimation = -1,                                    // light animation index
 13 BOOL m_bAlive = TRUE,                                            // if effector is still alive
 14 CEntityPointer m_penModel,                                       // ptr to model object used in this effect
 15 CEntityPointer m_penModel2,                                      // ptr to second model object used in this effect
 16 BOOL m_bWaitTrigger = FALSE,                                     // if effect is activated using trigger
 
 20 INDEX m_iModel1ID = -1 features(EPROPF_NETSEND),
 21 INDEX m_iModel2ID = -1 features(EPROPF_NETSEND),

{
  CLightSource m_lsLightSource;
}

components:
  1 editor model   MODEL_MARKER              "Data\\Models\\Editor\\Axis.mdl",
  2 editor texture TEXTURE_MARKER            "Data\\Models\\Editor\\Vector.tex",
  3 model   MODEL_POWER_RING          "Data\\Models\\CutSequences\\SpaceShip\\PowerRing.mdl",
  4 texture TEXTURE_POWER_RING        "Data\\Models\\CutSequences\\SpaceShip\\PowerRing.tex"

functions:

  void InitializeFromNet() 
  {
    ESpawnEffector ese;
    ese.eetType = m_eetType;
    ese.vDamageDir = m_vDamageDir;
    ese.vDestination = m_vFXDestination;;
    ese.tmLifeTime = m_tmLifeTime;
    ese.fSize = m_fSize;
    ese.ctCount = m_ctCount;
    if (m_iModel1ID == -1) {
      ese.eidModel = (CEntity*)NULL;
    } else {
      ese.eidModel = _pNetwork->ga_World.EntityFromID(m_iModel1ID);      
    }
    if (m_iModel2ID == -1) {
      ese.eidModel2 = (CEntity*)NULL;
    } else {
      ese.eidModel2 = _pNetwork->ga_World.EntityFromID(m_iModel2ID);      
    }
    Initialize(ese);
  }

  // calculate life ratio
  FLOAT CalculateLifeRatio(FLOAT fFadeInRatio, FLOAT fFadeOutRatio)
  {
    TIME tmDelta = _pTimer->GetLerpedCurrentTick()-m_tmStarted;
    FLOAT fLifeRatio = CalculateRatio( tmDelta, 0, m_tmLifeTime, fFadeInRatio, fFadeOutRatio);
    return fLifeRatio;
  }

  void AdjustMipFactor(FLOAT &fMipFactor)
  {
    if (m_eetType==ET_DISAPPEAR_MODEL || m_eetType==ET_DISAPPEAR_MODEL_NOW && m_penModel!=NULL)
    {
      CModelObject *pmo = m_penModel->GetModelObject();
      TIME tmDelta = _pTimer->GetLerpedCurrentTick()-m_tmStarted;
      FLOAT fLifeRatio;
      if( m_tmStarted == -1)
      {
        fLifeRatio = 1.0f;
      }
      else if( tmDelta>=m_tmLifeTime)
      {
        fLifeRatio = 0.0f;
      }
      else 
      {
        fLifeRatio = CalculateLifeRatio(0.0f, 1.0f);
      }
      UBYTE ubAlpha = UBYTE(255.0f*fLifeRatio);
      COLOR col = C_WHITE|ubAlpha;
      pmo->mo_colBlendColor = col;
    }
    if (m_eetType==ET_APPEAR_MODEL || m_eetType==ET_APPEAR_MODEL_NOW && m_penModel!=NULL)
    {
      CModelObject *pmo = m_penModel->GetModelObject();
      TIME tmDelta = _pTimer->GetLerpedCurrentTick()-m_tmStarted;
      FLOAT fLifeRatio;
      if( m_tmStarted == -1)
      {
        fLifeRatio = 0.0f;
      }
      else if( tmDelta>=m_tmLifeTime)
      {
        fLifeRatio = 1.0f;
      }
      else 
      {
        fLifeRatio = CalculateLifeRatio(1.0f, 0.0f);
      }
      UBYTE ubAlpha = UBYTE(255.0f*fLifeRatio);
      COLOR col = C_WHITE|ubAlpha;
      pmo->mo_colBlendColor = col;
    }
    if (m_eetType==ET_MORPH_MODELS && m_penModel!=NULL && m_penModel2!=NULL)
    {
      TIME tmDelta = _pTimer->GetLerpedCurrentTick()-m_tmStarted;
      FLOAT fLifeRatio;
      if( m_tmStarted == -1)
      {
        fLifeRatio = 0.0f;
      }
      else if( tmDelta>=m_tmLifeTime)
      {
        fLifeRatio = 1.0f;
      }
      else 
      {
        fLifeRatio = CalculateLifeRatio(1.0f, 0.0f);
      }
      UBYTE ubAlpha1 = UBYTE(255.0f*(1-fLifeRatio));
      UBYTE ubAlpha2 = 255-ubAlpha1;
      COLOR col1 = C_WHITE|ubAlpha1;
      COLOR col2 = C_WHITE|ubAlpha2;
      if (m_penModel->en_RenderType==RT_MODEL || m_penModel->en_RenderType==RT_EDITORMODEL) {
        CModelObject *pmo = m_penModel->GetModelObject();
        pmo->mo_colBlendColor = col1;
      } else if (m_penModel->en_RenderType==RT_SKAMODEL || m_penModel->en_RenderType==RT_SKAEDITORMODEL) {
        CModelInstance *pmi = m_penModel->GetModelInstance();
        pmi->mi_colModelColor = col1;
      }
      if (m_penModel2->en_RenderType==RT_MODEL || m_penModel2->en_RenderType==RT_EDITORMODEL) {
        CModelObject *pmo = m_penModel2->GetModelObject();
        pmo->mo_colBlendColor = col2;
      } else if (m_penModel->en_RenderType==RT_SKAMODEL || m_penModel->en_RenderType==RT_SKAEDITORMODEL) {
        CModelInstance *pmi = m_penModel2->GetModelInstance();
        pmi->mi_colModelColor = col2;
      }    
    }
  }

  BOOL AdjustShadingParameters(FLOAT3D &vLightDirection, COLOR &colLight, COLOR &colAmbient)
  {
    if(m_eetType==ET_MOVING_RING)
    {
      FLOAT fLifeRatio = CalculateLifeRatio(0.2f, 0.1f);
      FLOAT fT = _pTimer->CurrentTick()-m_tmStarted;
      FLOAT fPulse = 1.0f;
      UBYTE ub = UBYTE (255.0f*fPulse*fLifeRatio);
      COLOR col = RGBAToColor(ub,ub,ub,ub);
      GetModelObject()->mo_colBlendColor = col;
    }

    return FALSE;
  }

  void RenderMovingLightnings(void)
  {
    // calculate life ratio
    FLOAT fLifeRatio = CalculateLifeRatio(0.1f, 0.1f);
    // fill array with absolute vertices of entity's model and its attached models
    m_penModel->GetModelVerticesAbsolute(avModelFXVertices, 0.05f, 0.0f); 
    const FLOATmatrix3D &m = m_penModel->GetRotationMatrix();
    FLOAT3D vOrigin = m_penModel->GetPlacement().pl_PositionVector;
    FLOAT fFXTime = 0.75f;
    FLOAT fMaxHeight = 6.0f;
    FLOAT fdh = 1.0f;
    FLOAT tmDelta = _pTimer->GetLerpedCurrentTick()-m_tmStarted;
    FLOAT fY0 = tmDelta;

    for(FLOAT fT=tmDelta; fT>0; fT-=fFXTime)
    {
      FLOAT fY = fT*2.0f;
      if( fY>fMaxHeight)
      {
        continue;
      }
      // calculate height ratio
      FLOAT fHeightRatio = CalculateRatio( fY, 0, fMaxHeight, 0.1f, 0.0f);
      FLOAT fMinY = 1e6f;
      FLOAT fMinY2 = -1e6f;
      INDEX iLower = -1;
      INDEX iUpper = -1;
      for( INDEX iVtx=0; iVtx<avModelFXVertices.Count(); iVtx++)
      {
        // get relative vertex coordinate
        FLOAT3D v = (avModelFXVertices[iVtx]-vOrigin)*!m;
        if( v(2)>fY && v(2)<fMinY && v(1)<0)
        {
          iLower = iVtx;
          fMinY = v(2);
        }
        if( v(2)<=fY && v(2)>fMinY2 && v(1)<0)
        {
          iUpper = iVtx;
          fMinY2 = v(2);
        }
      }
      // if we found valid vertex
      if( iLower!=-1 && iUpper!=-1)
      {
        FLOAT3D vRelHi = (avModelFXVertices[iUpper]-vOrigin)*!m;
        FLOAT3D vRelLow = (avModelFXVertices[iLower]-vOrigin)*!m;
        FLOAT fLerpFactor = (fY-vRelLow(2))/(vRelHi(2)-vRelLow(2));
        FLOAT3D vRel = Lerp(vRelLow, vRelHi, fLerpFactor);
        vRel(2) = fY;
        FLOAT3D vAbs1 = vOrigin+vRel*m;
        vRel(1)=-vRel(1);
        FLOAT3D vAbs2 = vOrigin+vRel*m;
        // render lightning
        Particles_Ghostbuster( vAbs1, vAbs2, 16, 0.325f, fHeightRatio*fLifeRatio, 5.0f);
      }
    }
    avModelFXVertices.Clear();
  }

  // particles
  void RenderParticles(void)
  {
    // calculate ratio
    FLOAT fRatio;
    TIME tmNow = _pTimer->GetLerpedCurrentTick();
    TIME tmDelta = tmNow - m_tmStarted;
    FLOAT fLivingRatio = tmDelta/m_tmLifeTime;
    if(fLivingRatio<0.25f) {
      fRatio = Clamp( fLivingRatio/0.25f, 0.0f, 1.0f);
    } else if(fLivingRatio>0.75f) {
      fRatio = Clamp( (-fLivingRatio+1.0f)/0.25f, 0.0f, 1.0f);
    } else {
      fRatio = 1.0f;
    }

    switch( m_eetType)
    {
    case ET_DESTROY_OBELISK:
      Particles_DestroyingObelisk( this, m_tmStarted);
      break;
    case ET_DESTROY_PYLON:
      Particles_DestroyingPylon( this, m_vDamageDir, m_tmStarted);
      break;
    case ET_HIT_GROUND:
      Particles_HitGround( this, m_tmStarted, m_fSize);
      break;
    case ET_LIGHTNING:
      Particles_Ghostbuster(GetPlacement().pl_PositionVector, m_vFXDestination, m_ctCount, m_fSize, fRatio);
      break;
    case ET_PORTAL_LIGHTNING:
      RenderMovingLightnings();
      break;
    }
  };

  /* Read from stream. */
  void Read_t( CTStream *istr,BOOL bNetwork) // throw char *
  {
    CMovableModelEntity::Read_t(istr,bNetwork);
    // setup light source
    if( m_bLightSource) {
      SetupLightSource();
    }
  }

  /* Get static light source information. */
  CLightSource *GetLightSource(void)
  {
    if( m_bLightSource) {
      return &m_lsLightSource;
    } else {
      return NULL;
    }
  }

  // Setup light source
  void SetupLightSource(void)
  {
    if( m_iLightAnimation>=0)
    { // set light animation if available
      try {
        m_aoLightAnimation.SetData_t(CTFILENAME("Data\\Animations\\Effector.ani"));
      } catch (char *strError) {
        WarningMessage(TRANS("Cannot load Data\\Animations\\Effector.ani: %s"), strError);
      }
      // play light animation
      if (m_aoLightAnimation.GetData()!=NULL) {
        m_aoLightAnimation.PlayAnim(m_iLightAnimation, 0);
      }
    }

    // setup light source
    CLightSource lsNew;
    lsNew.ls_ulFlags = LSF_LENSFLAREONLY;
    lsNew.ls_rHotSpot = 0.0f;
    switch (m_eetType) {
      case ET_SIZING_RING_FLARE:
        lsNew.ls_colColor = C_WHITE|CT_OPAQUE;
        lsNew.ls_rHotSpot = 100.0f;
        lsNew.ls_rFallOff = 300.0f;
        lsNew.ls_plftLensFlare = &_lftWhiteGlowStarNG;
        break;

      case ET_SIZING_BIG_BLUE_FLARE:
        lsNew.ls_colColor = C_WHITE|CT_OPAQUE;
        lsNew.ls_rHotSpot = 500.0f*m_fSize;
        lsNew.ls_rFallOff = 1000.0f*m_fSize;
        lsNew.ls_plftLensFlare = &_lftBlueStarBlueReflections;
        break;

      default:
        ASSERTALWAYS("Unknown light source");
    }
    lsNew.ls_ubPolygonalMask = 0;
    lsNew.ls_paoLightAnimation = NULL;

    // setup light animation
    lsNew.ls_paoLightAnimation = NULL;
    if (m_aoLightAnimation.GetData()!=NULL) {
      lsNew.ls_paoLightAnimation = &m_aoLightAnimation;
    }

    m_lsLightSource.ls_penEntity = this;
    m_lsLightSource.SetLightSource(lsNew);
  }
/************************************************************
 *                          MAIN                            *
 ************************************************************/

procedures:

  Main( ESpawnEffector eSpawn)
  {
    // set appearance
    InitAsEditorModel();

    SetPhysicsFlags(EPF_MODEL_IMMATERIAL|EPF_MOVABLE);
    SetCollisionFlags(ECF_TOUCHMODEL);
    SetFlags( GetFlags()|ENF_SEETHROUGH);
    SetFlagOn(ENF_CLIENTHANDLING);

    SetModel(MODEL_MARKER);
    SetModelMainTexture(TEXTURE_MARKER);

    // setup variables
    m_eetType = eSpawn.eetType;
    m_vDamageDir = eSpawn.vDamageDir;
    m_tmStarted = _pTimer->CurrentTick();
    m_tmLifeTime = eSpawn.tmLifeTime;
    m_vFXDestination = eSpawn.vDestination;
    m_fSize = eSpawn.fSize;
    m_ctCount = eSpawn.ctCount;
    m_bAlive = TRUE;
    m_penModel  = (CEntity*)eSpawn.eidModel;
    m_penModel2 = (CEntity*)eSpawn.eidModel2;
    m_bWaitTrigger = FALSE;

    if ((CEntity*)m_penModel != NULL) {
      m_iModel1ID = ((CEntity*)m_penModel)->en_ulID;
    } else {
      m_iModel1ID = -1;
    }

    if ((CEntity*)m_penModel2 != NULL) {
      m_iModel2ID = ((CEntity*)m_penModel2)->en_ulID;
    } else {
      m_iModel2ID = -1;
    }

    autowait(0.1f);

    if(m_eetType==ET_MOVING_RING)
    {
      SetModel(MODEL_POWER_RING);
      SetModelMainTexture(TEXTURE_POWER_RING);
      en_fAcceleration = 1e6f;
      FLOAT fSpeed = 550.0f;
      SetDesiredTranslation(FLOAT3D(0,-fSpeed,0));
      FLOAT fPathLen = GetPlacement().pl_PositionVector(2)-m_vFXDestination(2);
      // t=s/v
      m_tmLifeTime = fPathLen/fSpeed;
      SwitchToModel();
      FLOAT fSize = 36.0f;
      FLOAT3D vStretch = FLOAT3D(fSize, fSize*2.0f, fSize);
      GetModelObject()->StretchModel(vStretch);
      ModelChangeNotify();
    }
    // spetial initializations
    if(m_eetType==ET_SIZING_RING_FLARE)
    {
      m_bLightSource = TRUE;
      m_iLightAnimation = 0;
    }
    if(m_eetType==ET_SIZING_BIG_BLUE_FLARE)
    {
      m_bLightSource = TRUE;
      m_iLightAnimation = 1;
    }
    if(m_eetType==ET_MORPH_MODELS || m_eetType==ET_DISAPPEAR_MODEL || m_eetType==ET_APPEAR_MODEL)
    {
      m_bWaitTrigger = TRUE;
      m_tmStarted = -1;
    }
    if(m_eetType==ET_DISAPPEAR_MODEL_NOW || m_eetType==ET_APPEAR_MODEL_NOW)
    {
      m_bWaitTrigger = FALSE;
      m_tmStarted = _pTimer->CurrentTick();
    }

    // setup light source
    if (m_bLightSource) { SetupLightSource(); }

    while(_pTimer->CurrentTick()<m_tmStarted+m_tmLifeTime && m_bAlive || m_bWaitTrigger)
    {
      wait( 0.25f)
      {
        on( EBegin):{ resume;}
        on( ETrigger):
        { 
          if (_pNetwork->IsServer()) {
              SendEvent(ETriggerEffector());
            }
          resume;
        }
        on (ETriggerEffector):
        {
          if(m_eetType==ET_MORPH_MODELS || m_eetType==ET_DISAPPEAR_MODEL || m_eetType==ET_APPEAR_MODEL)
          {
            m_tmStarted = _pTimer->CurrentTick();
            m_bWaitTrigger = FALSE;
            // live forever
            m_bAlive = TRUE;
          }
          resume;
        }
        on( ETimer):{ stop;}
      }
      // check if moving ring reached target position
      if(m_eetType==ET_MOVING_RING)
      {
        if( GetPlacement().pl_PositionVector(2) < m_vFXDestination(2))
        {
          m_bAlive = FALSE;
        }
      }
    }

    Destroy(FALSE);
    return;
  }
};
