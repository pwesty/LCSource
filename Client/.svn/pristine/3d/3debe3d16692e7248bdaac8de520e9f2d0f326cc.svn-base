603
%{
#include "StdH.h"
%}

// input parameter for spawning a blood spray
event ESpawnSpray {
  enum SprayParticlesType sptType, // type of particles
  FLOAT fDamagePower,              // factor saying how powerfull damage has been
  FLOAT fSizeMultiplier,           // stretch factor
  FLOAT3D vDirection,              // dammage direction  
  CEntityID eidOwner,              // who spawned the spray
  COLOR colCentralColor,           // central color of particles that is randomized a little
  FLOAT fLaunchPower,
  COLOR colBurnColor,              // burn color
};

class CBloodSpray: CRationalEntity {
name      "Blood Spray";
thumbnail "";
features  "CanBePredictable", "NotSentOverNet";

properties:

  1 enum SprayParticlesType m_sptType = SPT_NONE features(EPROPF_NETSEND),     // type of particles
  2 FLOAT m_tmStarted = 0.0f features(EPROPF_NETSEND),                         // time when spawned
  3 FLOAT3D m_vDirection = FLOAT3D(0,0,0) features(EPROPF_NETSEND),            // dammage direction
  5 CEntityPointer m_penOwner,                                                     // who spawned the spray
  6 FLOAT m_fDamagePower = 1.0f features(EPROPF_NETSEND),                      // power of inflicted damage
  8 FLOATaabbox3D m_boxSizedOwner = FLOATaabbox3D(FLOAT3D(0,0,0), 0.01f),          // bounding box of blood spray's owner
  9 FLOAT3D m_vGDir = FLOAT3D(0,0,0),                                              // gravity direction
  10 FLOAT m_fGA = 0.0f,                                                           // gravity strength
  11 FLOAT m_fLaunchPower = 1.0f features(EPROPF_NETSEND),
  12 COLOR m_colCentralColor = COLOR(C_WHITE|CT_OPAQUE) features(EPROPF_NETSEND),
  13 FLOATaabbox3D m_boxOriginalOwner = FLOATaabbox3D(FLOAT3D(0,0,0), 0.01f),
  14 COLOR m_colBurnColor = COLOR(C_WHITE|CT_OPAQUE) features(EPROPF_NETSEND),

  15 INDEX m_iOwner = -1 features(EPROPF_NETSEND), // owner's id


components:
  1 editor model   MODEL_MARKER     "Data\\Models\\Editor\\Axis.mdl",
  2 editor texture TEXTURE_MARKER   "Data\\Models\\Editor\\Vector.tex"

functions:

  void InitializeFromNet()
  {
    ESpawnSpray eSpawn;
    eSpawn.sptType = m_sptType;
    eSpawn.vDirection = m_vDirection;
    
    eSpawn.fDamagePower = m_fDamagePower;
    eSpawn.fLaunchPower = m_fLaunchPower;
    eSpawn.colBurnColor = m_colBurnColor;
    
    eSpawn.colCentralColor = m_colCentralColor;

    CEntity* penEntity;
    if (m_iOwner == -1) {
      eSpawn.eidOwner = (CEntity*)NULL; 
    } else {
      _pNetwork->ga_World.EntityExists(m_iOwner,penEntity);
      eSpawn.eidOwner = penEntity;
    }

    Initialize(eSpawn);
  }

  // particles
  void RenderParticles(void)
  {
/*
    switch( m_sptType)
    {
    case SPT_BLOOD:
    case SPT_BONES:
    case SPT_FEATHER:
    case SPT_STONES:
    case SPT_WOOD:
    case SPT_SLIME:
    case SPT_LAVA_STONES:
    case SPT_SMALL_LAVA_STONES:
    case SPT_BEAST_PROJECTILE_SPRAY:
    case SPT_AIRSPOUTS:
    case SPT_GOO:
    {
      Particles_BloodSpray(m_sptType, GetLerpedPlacement().pl_PositionVector, m_vGDir, m_fGA,
        m_boxSizedOwner, m_vDirection, m_tmStarted, m_fDamagePower, m_colBurnColor);
      break;
    }
    case SPT_COLOREDSTONE:
      {
        Particles_BloodSpray(m_sptType, GetLerpedPlacement().pl_PositionVector, m_vGDir, m_fGA,
          m_boxSizedOwner, m_vDirection, m_tmStarted, m_fDamagePower, MulColors(m_colCentralColor, m_colBurnColor) );
        break;
      }
    case SPT_TREE01:
      Particles_BloodSpray(SPT_WOOD, GetLerpedPlacement().pl_PositionVector, m_vGDir, m_fGA/1.5f,
        m_boxSizedOwner, m_vDirection, m_tmStarted, m_fDamagePower/2.0f, m_colBurnColor);
      Particles_Leaves(m_penOwner, m_boxOriginalOwner, GetLerpedPlacement().pl_PositionVector,
        m_fDamagePower, m_fDamagePower*m_fLaunchPower, m_vGDir,
        m_fGA/2.0f, m_tmStarted, MulColors(m_colCentralColor,m_colBurnColor));
      break;
    case SPT_PLASMA:
        Particles_BloodSpray(m_sptType, GetLerpedPlacement().pl_PositionVector, m_vGDir, m_fGA,
          m_boxSizedOwner, m_vDirection, m_tmStarted, m_fDamagePower);
        Particles_DamageSmoke(this, m_tmStarted, m_boxSizedOwner, m_fDamagePower);
        Particles_ElectricitySparks( this, m_tmStarted, 5.0f, 0.0f, 32);
      break;
    case SPT_ELECTRICITY_SPARKS:
      {
        Particles_MetalParts(this, m_tmStarted, m_boxSizedOwner, m_fDamagePower);
        Particles_DamageSmoke(this, m_tmStarted, m_boxSizedOwner, m_fDamagePower);
        Particles_BloodSpray(SPT_BLOOD, GetLerpedPlacement().pl_PositionVector, m_vGDir, m_fGA,
          m_boxSizedOwner, m_vDirection, m_tmStarted, m_fDamagePower/2.0f, C_WHITE|CT_OPAQUE);
        Particles_ElectricitySparks( this, m_tmStarted, 5.0f, 0.0f, 32);
        break;
      }
    case SPT_ELECTRICITY_SPARKS_NO_BLOOD:
      {
        Particles_MetalParts(this, m_tmStarted, m_boxSizedOwner, m_fDamagePower);
        Particles_DamageSmoke(this, m_tmStarted, m_boxSizedOwner, m_fDamagePower);
        Particles_ElectricitySparks( this, m_tmStarted, 5.0f, 0.0f, 32);        
      }
    }
	*/
  };

/************************************************************
 *                          MAIN                            *
 ************************************************************/

procedures:

  Main( ESpawnSpray eSpawn)
  {
    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    // set appearance
    SetModel(MODEL_MARKER);
    SetModelMainTexture(TEXTURE_MARKER);
    
    // if owner doesn't exist (could be destroyed in initialization)
    if( ((CEntity*)eSpawn.eidOwner)==NULL || ((CEntity*)eSpawn.eidOwner)->en_pmoModelObject == NULL)
    {
      // don't do anything
      Destroy();
      return;
    }

    // setup variables
    m_sptType = eSpawn.sptType;
    m_vDirection = eSpawn.vDirection;
    m_penOwner = eSpawn.eidOwner;
    m_fDamagePower = eSpawn.fDamagePower;
    m_fLaunchPower = eSpawn.fLaunchPower;
    m_colBurnColor = eSpawn.colBurnColor;
    m_tmStarted = _pTimer->CurrentTick();
    m_colCentralColor = eSpawn.colCentralColor;

    m_iOwner = m_penOwner->en_ulID;

    if(((CEntity*)eSpawn.eidOwner)->en_RenderType == RT_SKAMODEL ||
       ((CEntity*)eSpawn.eidOwner)->en_RenderType == RT_SKAEDITORMODEL)
    {
      ((CEntity*)eSpawn.eidOwner)->GetSize( m_boxOriginalOwner);
      m_boxSizedOwner=m_boxOriginalOwner;
      FLOAT3D vStretchVector=FLOAT3D(eSpawn.fSizeMultiplier,eSpawn.fSizeMultiplier,eSpawn.fSizeMultiplier);
      m_boxSizedOwner.StretchByVector(vStretchVector);
    }
    else 
    {
      ((CEntity*)eSpawn.eidOwner)->en_pmoModelObject->GetCurrentFrameBBox( m_boxSizedOwner);
      m_boxOriginalOwner=m_boxSizedOwner;
      m_boxSizedOwner.StretchByVector(((CEntity*)eSpawn.eidOwner)->en_pmoModelObject->mo_Stretch*eSpawn.fSizeMultiplier);
      m_boxOriginalOwner.StretchByVector(((CEntity*)eSpawn.eidOwner)->en_pmoModelObject->mo_Stretch);
    }

    if (!_pNetwork->IsServer() && (m_penOwner->GetPhysicsFlags()&EPF_MOVABLE) && (m_penOwner->en_pciCollisionInfo!=NULL)) {
      CMovableEntity *penMovable = (CMovableEntity *)((CEntity*)m_penOwner);
      penMovable->TestFields(penMovable->en_iUpContent,penMovable->en_iDnContent,penMovable->en_fImmersionFactor);
    }

    if (m_penOwner->GetPhysicsFlags()&EPF_MOVABLE) {
      m_vGDir = ((CMovableEntity *)&*m_penOwner)->en_vGravityDir;
      m_fGA = ((CMovableEntity *)&*m_penOwner)->en_fGravityA;
    } else {
      FLOATmatrix3D &m = m_penOwner->en_mRotation;
      m_vGDir = FLOAT3D(-m(1,2), -m(2,2), -m(3,2));
      m_fGA = 30.0f;
    }

    FLOAT fWaitTime = 4.0f;
    if( m_sptType==SPT_ELECTRICITY_SPARKS || m_sptType==SPT_ELECTRICITY_SPARKS_NO_BLOOD)
    {
      fWaitTime = 4.0f;
    }
    autowait(fWaitTime);
    Destroy();

    return;
  }
};
