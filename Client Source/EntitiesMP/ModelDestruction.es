217
%{
#include "StdH.h"
%}

uses "EntitiesMP/ModelHolder2";
uses "EntitiesMP/ModelHolder3";
uses "EntitiesMP/BasicEffects";
uses "EntitiesMP/Debris";
uses "EntitiesMP/BloodSpray";
uses "EntitiesMP/SoundHolder";

// event sent to entities in range of model destroy
// (e.g light can turn off)
event ERangeModelDestruction {
};
// sent by models to themselves over network when they receive EDeath
event EModelDeath {  
};

// type of debris
enum DestructionDebrisType {
  1 DDT_STONE "Stone",
  2 DDT_WOOD  "Wood",
  3 DDT_PALM  "Palm",
  4 DDT_CHILDREN_CUSTOM "Custom (children)",
};

class CModelDestruction : CEntity {
name      "ModelDestruction";
thumbnail "Thumbnails\\ModelDestruction.tbn";
features  "HasName", "IsTargetable", "IsImportant";

properties:
  1 CTString m_strName          "Name" 'N' = "ModelDestruction",
  2 CTString m_strDescription = "",

 10 CEntityPointer m_penModel0  "Model 0" 'M' COLOR(C_RED|0x80),
 11 CEntityPointer m_penModel1  "Model 1" COLOR(C_RED|0x80),
 12 CEntityPointer m_penModel2  "Model 2" COLOR(C_RED|0x80),
 13 CEntityPointer m_penModel3  "Model 3" COLOR(C_RED|0x80),
 14 CEntityPointer m_penModel4  "Model 4" COLOR(C_RED|0x80),

 20 FLOAT m_fHealth "Health" 'H' = 50.0f,   // health of the model pointing to this
 22 enum DestructionDebrisType m_ddtDebris "Debris" 'D' = DDT_STONE,  // type of debris
 23 INDEX m_ctDebris "Debris Count" = 3,
 24 FLOAT m_fDebrisSize "Debris Size" = 1.0f,
 25 enum EntityInfoBodyType m_eibtBodyType "Body Type" = EIBT_ROCK,
 26 enum SprayParticlesType m_sptType "Particle Type" = SPT_NONE, // type of particles
 27 FLOAT m_fParticleSize "Particle Size" 'Z' = 1.0f, // size of particles
 28 BOOL m_bRequireExplosion "Requires Explosion" = FALSE,
 29 FLOAT m_fDebrisLaunchPower "CC: Debris Launch Power" 'L' = 1.0f, // launch power of debris
 30 enum DebrisParticlesType m_dptParticles "CC: Trail particles" = DPT_NONE,
 31 enum BasicEffectType m_betStain "CC: Leave stain" = BET_NONE,
 32 FLOAT m_fLaunchCone "CC: Launch cone" = 45.0f,
 33 FLOAT m_fRndRotH "CC: Rotation heading" = 720.0f,
 34 FLOAT m_fRndRotP "CC: Rotation pitch" = 720.0f,
 35 FLOAT m_fRndRotB "CC: Rotation banking" = 720.0f,
 36 FLOAT m_fParticleLaunchPower "Particle Launch Power" 'P' = 1.0f, // launch power of particles
 37 COLOR m_colParticles "Central Particle Color" 'C' = COLOR(C_WHITE|CT_OPAQUE),
 40 ANIMATION m_iStartAnim "Start anim" = -1,
 41 BOOL m_bDebrisImmaterialASAP "Immaterial ASAP" = TRUE,
 60 CTString m_strStartAnim "Start anim (for SKA)" = "",

 50 INDEX m_ctDustFall "Dusts Count" = 1,                                   // count of spawned dust falls
 51 FLOAT m_fMinDustFallHeightRatio "Dust Min Height Ratio" = 0.1f,         // min ratio of model height for dust
 52 FLOAT m_fMaxDustFallHeightRatio "Dust Max Height Ratio" = 0.6f,         // max ratio of model height for dust
 53 FLOAT m_fDustStretch "Dust Stretch" = 1.0f,                             // dust stretch
 54 FLOAT m_fDebrisDustRandom "Dust Debris Random" = 0.25f,                 // random for spawning dusts on debris fall
 55 FLOAT m_fDebrisDustStretch "Dust Debris Stretch" = 1.0f,                // size of spawned dust
 56 CEntityPointer m_penShake "Shake marker" 'A',

components:
  1 editor model   MODEL_MODELDESTRUCTION     "Data\\Models\\Editor\\ModelDestruction.mdl",
  2 editor texture TEXTURE_MODELDESTRUCTION   "Data\\Models\\Editor\\ModelDestruction.tex",
  3 class   CLASS_BASIC_EFFECT    "Classes\\BasicEffect.ecl",
  4 class   CLASS_SOUND_HOLDER    "Classes\\SoundHolder.ecl",

// ************** WOOD PARTS **************
 10 model     MODEL_WOOD        "Data\\Models\\Effects\\Debris\\Wood01\\Wood.mdl",
 11 texture   TEXTURE_WOOD      "Data\\Models\\Effects\\Debris\\Wood01\\Wood.tex",

 12 model     MODEL_BRANCH        "Data\\ModelsMP\\Effects\\Debris\\Tree\\Tree.mdl",
 13 texture   TEXTURE_BRANCH      "Data\\ModelsMP\\Plants\\Tree01\\Tree01.tex",

// ************** STONE PARTS **************
 14 model     MODEL_STONE        "Data\\Models\\Effects\\Debris\\Stone\\Stone.mdl",
 15 texture   TEXTURE_STONE      "Data\\Models\\Effects\\Debris\\Stone\\Stone.tex",

// *****  SOUNDS - ONLY TO BE PRECACHED *****
 // NOTE!!!! if you change these sounds, you MUST also apply the changes to the
 // same filenames lower in the code, in the function SpawnSoundHolder()
 50 sound     SOUND_DESTRUCTION_STONE  "SoundsMP\\Ambience\\StoneBreak.wav",
 51 sound     SOUND_DESTRUCTION_WOOD   "SoundsMP\\Ambience\\Crack.wav",
 52 sound     SOUND_DESTRUCTION_PALM   "SoundsMP\\Ambience\\BranchesFall.wav",  


functions:
  void Precache(void) {
    PrecacheClass(CLASS_BASIC_EFFECT, BET_EXPLOSIONSTAIN);
    PrecacheClass(CLASS_SOUND_HOLDER);
    PrecacheSound(SOUND_DESTRUCTION_STONE);
    PrecacheSound(SOUND_DESTRUCTION_WOOD);
    PrecacheSound(SOUND_DESTRUCTION_PALM);

    switch(m_ddtDebris) {
    case DDT_STONE: { 
      PrecacheModel(MODEL_STONE);
      PrecacheTexture(TEXTURE_STONE);
                          } break;
    case DDT_WOOD:   {  
      PrecacheModel(MODEL_WOOD);
      PrecacheTexture(TEXTURE_WOOD);
                          } break;
    case DDT_PALM:   {  
      PrecacheModel(MODEL_WOOD);
      PrecacheTexture(TEXTURE_WOOD);
                          } break;
    }
  };

  /* Get anim data for given animation property - return NULL for none. */
  CAnimData *GetAnimData(SLONG slPropertyOffset)
  {
    if( IsOfClass( GetModel(0), "CModelHolder2")) {
      if(slPropertyOffset==offsetof(CModelDestruction, m_iStartAnim)) {
        CModelHolder2 *pmh = (CModelHolder2 *)GetModel(0);
        if(pmh!=NULL) {

          return pmh->GetModelObject()->GetData();
        }
      }
    }
    return CEntity::GetAnimData(slPropertyOffset);
  }

  const CTString &GetDescription(void) const {
    INDEX ct = GetModelsCount();
    if(ct==0) {
      ((CTString&)m_strDescription).PrintF("(%g): no more", m_fHealth);
    } else if(ct==1) {
      ((CTString&)m_strDescription).PrintF("(%g): %s", m_fHealth, m_penModel0->GetName());
    } else if (TRUE) {
      ((CTString&)m_strDescription).PrintF("(%g): %s,...(%d)", m_fHealth, m_penModel0->GetName(), ct);
    }
    return m_strDescription;
  }

  // check if one model target is valid 
  void CheckOneModelTarget(CEntityPointer &pen)
  {
    if (pen!=NULL && !IsOfClass(pen, "ModelHolder2")) {
      WarningMessage("Model '%s' is not ModelHolder2!", pen->GetName());
      pen=NULL;
    }
  }

  // get next phase in destruction
  CEntity *GetNextPhase(void)
  {
    INDEX ct = GetModelsCount();
    // if not more models
    if (ct==0) {
      // return none
      return NULL;
    // if there are some
    } else {
      // choose by random
      return GetModel(IRnd()%ct);
    }
  }

  // get number of models set by user
  INDEX GetModelsCount(void) const
  {
    // note: only first N that are no NULL are used
    if (m_penModel0==NULL) { return 0; };
    if (m_penModel1==NULL) { return 1; };
    if (m_penModel2==NULL) { return 2; };
    if (m_penModel3==NULL) { return 3; };
    if (m_penModel4==NULL) { return 4; };
    return 5;
  }
  // get model by its index
  CEntity *GetModel(INDEX iModel)
  {
    ASSERT(iModel<=GetModelsCount());
    iModel = Clamp(iModel, INDEX(0), GetModelsCount());
    return (CModelHolder2 *)&*(&m_penModel0)[iModel];
  }

   void SpawnSoundHolder(DestructionDebrisType ddtDebris, CPlacement3D plModel)
  {
    CTFileName fnSound;
    if (ddtDebris==DDT_CHILDREN_CUSTOM) {
      return;
    }
    switch(ddtDebris) {
    case DDT_STONE:
      fnSound = CTString("Data\\Sounds\\Default.wav");			// 씨리어스 리소스 삭제로 인해 Default.wav로 대체
      break;
    case DDT_WOOD:
      fnSound = CTString("Data\\Sounds\\Default.wav");			// 씨리어스 리소스 삭제로 인해 Default.wav로 대체
      break;
    case DDT_PALM:
      fnSound = CTString("Data\\Sounds\\Default.wav");			// 씨리어스 리소스 삭제로 인해 Default.wav로 대체
      break;
    default: {
      return; }
      break;
    }
    // create it at the placement of destroyed model
    CEntityPointer penNew = CreateEntity(plModel, CLASS_SOUND_HOLDER, WLD_AUTO_ENTITY_ID, FALSE);
    CSoundHolder *penSoundH = (CSoundHolder *)&*penNew;
    penSoundH->Initialize(EVoid(), FALSE);
    penSoundH->m_fnSound       = fnSound;
    penSoundH->m_rFallOffRange = 50.0f;
    penSoundH->m_rHotSpotRange = 10.0f;
    penSoundH->m_fVolume       = 1.0f;
    penSoundH->m_fPitch        = 0.9f+0.2f*FRnd();
    penSoundH->SendEvent(EPlaySoundOnce());          
  }

  // spawn debris for given model
  void SpawnDebris(CEntity *penDestroyed)
  {
    FLOATaabbox3D box;
    penDestroyed->GetBoundingBox(box);
    FLOAT fEntitySize = box.Size().MaxNorm();
    switch(m_ddtDebris) {
    case DDT_STONE: {
      Debris_Begin(EIBT_ROCK, DPT_NONE, BET_NONE, fEntitySize, FLOAT3D(0,0,0), FLOAT3D(0,0,0), 1.0f, 0.0f);
      for(INDEX iDebris = 0; iDebris<m_ctDebris; iDebris++) {
        Debris_Spawn(penDestroyed, this, MODEL_STONE, TEXTURE_STONE, 0, 0, 0, IRnd()%4, m_fDebrisSize,
          FLOAT3D(FRnd()*0.8f+0.1f, FRnd()*0.8f+0.1f, FRnd()*0.8f+0.1f));
      }
                    } break;
    case DDT_WOOD:
    {
      Debris_Begin(EIBT_WOOD, DPT_NONE, BET_NONE, fEntitySize, FLOAT3D(0,0,0), FLOAT3D(0,0,0), 1.0f, 0.0f);
      for(INDEX iDebris = 0; iDebris<m_ctDebris; iDebris++)
      {
        Debris_Spawn(penDestroyed, this, MODEL_WOOD, TEXTURE_WOOD, 0, 0, 0, 0, m_fDebrisSize,
          FLOAT3D(0.5f, 0.5f, 0.5f));
      }
      break;
    }
    case DDT_CHILDREN_CUSTOM:
    {
      Debris_Begin(EIBT_WOOD, DPT_NONE, BET_NONE, 1.0f, FLOAT3D(10,10,10), FLOAT3D(0,0,0), 5.0f, 2.0f);

      BOOL bValidModel = TRUE;
      BOOL bMH2 = TRUE;
      FLOAT3D vOldStretch;
      if( IsOfClass( penDestroyed, "ModelHolder2")) {
        if (((CModelHolder2 *)penDestroyed)->GetModelObject()==NULL) { 
          bValidModel = FALSE; 
        }
        vOldStretch = penDestroyed->GetModelObject()->mo_Stretch;
      } else {
        if (((CModelHolder3 *)penDestroyed)->GetModelInstance()==NULL) { 
          bValidModel = FALSE; 
        }
        vOldStretch = penDestroyed->GetModelInstance()->mi_vStretch;
        bMH2 = FALSE;
      }

      // launch all children of model holder type
      FOREACHINLIST( CEntity, en_lnInParent, en_lhChildren, iten)
      {
        if( IsOfClass( &*iten, &CModelHolder2_DLLClass)) {
          CModelHolder2 &mhTemplate=(CModelHolder2 &)*iten;
          if( mhTemplate.GetModelObject()==NULL || !bValidModel) {
            continue;
          }
          CModelObject &moNew=*mhTemplate.GetModelObject();
          CPlacement3D plRel=mhTemplate.GetPlacement();
          plRel.AbsoluteToRelative(this->GetPlacement());
          CPlacement3D plLaunch=plRel;
          plLaunch.pl_PositionVector(1)=plLaunch.pl_PositionVector(1)*vOldStretch(1);
          plLaunch.pl_PositionVector(2)=plLaunch.pl_PositionVector(2)*vOldStretch(2);
          plLaunch.pl_PositionVector(3)=plLaunch.pl_PositionVector(3)*vOldStretch(3);
          plLaunch.RelativeToAbsolute(penDestroyed->GetPlacement());

          ANGLE3D angLaunch=ANGLE3D(FRnd()*360.0f,90.0f+m_fLaunchCone*(FRnd()-0.5f),0);
          FLOAT3D vLaunchDir;
          FLOAT3D vStretchTemplate=FLOAT3D(
            vOldStretch(1)*moNew.mo_Stretch(1),
            vOldStretch(2)*moNew.mo_Stretch(2),
            vOldStretch(3)*moNew.mo_Stretch(3));
          AnglesToDirectionVector(angLaunch, vLaunchDir);
          vLaunchDir.Normalize();
          vLaunchDir=vLaunchDir*m_fDebrisLaunchPower;
          ANGLE3D angRotSpeed=ANGLE3D(m_fRndRotH*2.0f*(FRnd()-0.5f),m_fRndRotP*(FRnd()-0.5f),m_fRndRotB*(FRnd()-0.5f));
          
          FLOAT fDustSize=0.0f;
          if( FRnd()<m_fDebrisDustRandom) {
            fDustSize=m_fDebrisDustStretch;
          }
          
          if (bMH2) {
            Debris_Spawn_MH2Template_FromMH2( m_eibtBodyType, m_dptParticles, m_betStain,
              (CModelHolder2*)penDestroyed, this, &mhTemplate, vStretchTemplate,
              mhTemplate.m_fStretchAll, plLaunch, vLaunchDir, angRotSpeed,
              m_bDebrisImmaterialASAP, fDustSize, ((CModelHolder2*)penDestroyed)->m_colBurning);
          } else {
            Debris_Spawn_MH2Template_FromMH3( m_eibtBodyType, m_dptParticles, m_betStain,
              (CModelHolder3*)penDestroyed, this, &mhTemplate, vStretchTemplate,
              mhTemplate.m_fStretchAll, plLaunch, vLaunchDir, angRotSpeed,
              m_bDebrisImmaterialASAP, fDustSize, ((CModelHolder3*)penDestroyed)->m_colBurning);
          }
        }
        if( IsOfClass( &*iten, &CModelHolder3_DLLClass)) 
        {
          //CPrintF("Spawning children debris of ModelHolder3 type not supported!\n");
          
          CModelHolder3 &mhTemplate=(CModelHolder3 &)*iten;
          if( mhTemplate.GetModelInstance()==NULL || !bValidModel)
          {
            continue;
          }
          CModelInstance &miNew=*mhTemplate.GetModelInstance();
          CPlacement3D plRel=mhTemplate.GetPlacement();

          plRel.AbsoluteToRelative(this->GetPlacement());
          CPlacement3D plLaunch=plRel;
          plLaunch.pl_PositionVector(1)=plLaunch.pl_PositionVector(1)*vOldStretch(1);
          plLaunch.pl_PositionVector(2)=plLaunch.pl_PositionVector(2)*vOldStretch(2);
          plLaunch.pl_PositionVector(3)=plLaunch.pl_PositionVector(3)*vOldStretch(3);
          plLaunch.RelativeToAbsolute(penDestroyed->GetPlacement());
          ANGLE3D angLaunch=ANGLE3D(FRnd()*360.0f,90.0f+m_fLaunchCone*(FRnd()-0.5f),0);
          FLOAT3D vLaunchDir;
          FLOAT3D vStretchTemplate=FLOAT3D(
            vOldStretch(1)*miNew.mi_vStretch(1),
            vOldStretch(2)*miNew.mi_vStretch(2),
            vOldStretch(3)*miNew.mi_vStretch(3));
          AnglesToDirectionVector(angLaunch, vLaunchDir);
          vLaunchDir.Normalize();
          vLaunchDir=vLaunchDir*m_fDebrisLaunchPower;
          ANGLE3D angRotSpeed=ANGLE3D(m_fRndRotH*2.0f*(FRnd()-0.5f),m_fRndRotP*(FRnd()-0.5f),m_fRndRotB*(FRnd()-0.5f));
          
          FLOAT fDustSize=0.0f;
          if( FRnd()<m_fDebrisDustRandom)
          {
            fDustSize=m_fDebrisDustStretch;
          }
          
          if (bMH2) {
          Debris_Spawn_MH3Template_FromMH2( m_eibtBodyType, m_dptParticles, m_betStain,
                                (CModelHolder2 *)penDestroyed,
                                this, &mhTemplate, vStretchTemplate,
                                mhTemplate.m_fStretchAll, plLaunch,
                                vLaunchDir, angRotSpeed, m_bDebrisImmaterialASAP,
                                fDustSize, ((CModelHolder3 *)penDestroyed)->m_colBurning);
          } else {
          Debris_Spawn_MH3Template_FromMH3( m_eibtBodyType, m_dptParticles, m_betStain,
                                (CModelHolder3 *)penDestroyed, 
                                this, &mhTemplate, vStretchTemplate,
                                mhTemplate.m_fStretchAll, plLaunch,
                                vLaunchDir, angRotSpeed, m_bDebrisImmaterialASAP,
                                fDustSize, ((CModelHolder3 *)penDestroyed)->m_colBurning);
          }
        }
        if( IsOfClass( &*iten, &CSoundHolder_DLLClass))
        {
          CSoundHolder &ensh=(CSoundHolder &)*iten;
          // copy it at the placement of destroyed model
          CEntity *penNewSH = GetWorld()->CopyEntityInWorld( ensh, penDestroyed->GetPlacement(),TRUE,WLD_AUTO_ENTITY_ID,FALSE);
          penNewSH->SetParent(NULL);
          penNewSH->SendEvent(EPlaySoundOnce());          
        }
      }      
      break;      
    }
    case DDT_PALM: {
        FLOAT3D vDamage = FLOAT3D(0.0f, 0.0f, 0.0f);
        if( IsOfClass( penDestroyed, "ModelHolder2")) {
          vDamage = ((CModelHolder2 *)penDestroyed)->m_vDamage;
        } else {
          vDamage = ((CModelHolder3 *)penDestroyed)->m_vDamage;
        }
        Debris_Begin(EIBT_WOOD, DPT_NONE, BET_NONE, fEntitySize, vDamage*0.3f, FLOAT3D(0,0,0), 1.0f, 0.0f);
        Debris_Spawn(penDestroyed, this, MODEL_WOOD, TEXTURE_WOOD, 0, 0, 0, 0, m_fDebrisSize,
          FLOAT3D(0.5f, 0.2f, 0.5f));
        Debris_Spawn(penDestroyed, this, MODEL_WOOD, TEXTURE_WOOD, 0, 0, 0, 1, m_fDebrisSize,
          FLOAT3D(0.5f, 0.3f, 0.5f));
        Debris_Spawn(penDestroyed, this, MODEL_WOOD, TEXTURE_WOOD, 0, 0, 0, 2, m_fDebrisSize,
          FLOAT3D(0.5f, 0.4f, 0.5f));
        Debris_Spawn(penDestroyed, this, MODEL_WOOD, TEXTURE_WOOD, 0, 0, 0, 3, m_fDebrisSize,
          FLOAT3D(0.5f, 0.5f, 0.5f));
        Debris_Spawn(penDestroyed, this, MODEL_WOOD, TEXTURE_WOOD, 0, 0, 0, 1, m_fDebrisSize,
          FLOAT3D(0.5f, 0.6f, 0.5f));
        Debris_Spawn(penDestroyed, this, MODEL_WOOD, TEXTURE_WOOD, 0, 0, 0, 2, m_fDebrisSize,
          FLOAT3D(0.5f, 0.8f, 0.5f));
        Debris_Spawn(penDestroyed, this, MODEL_WOOD, TEXTURE_WOOD, 0, 0, 0, 1, m_fDebrisSize,
          FLOAT3D(0.5f, 0.9f, 0.5f));
      }
      break;
    default: {} break;
    };

    SpawnSoundHolder(m_ddtDebris, penDestroyed->GetPlacement());

    if( m_ctDustFall>0)
    {
      FLOAT fHeight=box.Size()(2);
      FLOAT fMinHeight=fHeight*m_fMinDustFallHeightRatio;
      FLOAT fMaxHeight=fHeight*m_fMaxDustFallHeightRatio;
      FLOAT fHeightSteep=(fMaxHeight-fMinHeight)/m_ctDustFall;
      for(INDEX iDust=0; iDust<m_ctDustFall; iDust++)
      {
        FLOAT fY=fMinHeight+iDust*fHeightSteep;
        CPlacement3D plDust=penDestroyed->GetPlacement();
        plDust.pl_PositionVector=plDust.pl_PositionVector+FLOAT3D(0,fY,0);
        // spawn dust effect
        ESpawnEffect ese;
        ese.colMuliplier = C_WHITE|CT_OPAQUE;
        ese.vStretch = FLOAT3D(m_fDustStretch,m_fDustStretch,m_fDustStretch);
        ese.vNormal = FLOAT3D(0,1,0);
        ese.betType = BET_DUST_FALL;
        CEntityPointer penFX = CreateEntity(plDust, CLASS_BASIC_EFFECT);
        penFX->Initialize(ese);
      }
    }
  }

procedures:
  Main()
  {
    // must not allow invalid classes
    CheckOneModelTarget(m_penModel0);
    CheckOneModelTarget(m_penModel1);
    CheckOneModelTarget(m_penModel2);
    CheckOneModelTarget(m_penModel3);
    CheckOneModelTarget(m_penModel4);

    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    // set appearance
    SetModel(MODEL_MODELDESTRUCTION);
    SetModelMainTexture(TEXTURE_MODELDESTRUCTION);

    return;
  }
};

