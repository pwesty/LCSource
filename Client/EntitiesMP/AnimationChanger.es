218
%{
#include "StdH.h"
#include <EntitiesMP/AnimationHub.h>
%}

uses "EntitiesMP/ModelHolder2";
uses "EntitiesMP/Light";

enum ChangeAnimType
{
	0 ACTION_NONE	"normal animation",
	1 ACTION_SKILL	"skill animation",
	2 ACTION_DISAPPEAR "disappear animation",
	3 ACTION_APPEAR "appear animation",
};

// event sent to the entity that should change animation
event EChangeAnim {
  INDEX iModelAnim,
  BOOL  bModelLoop,
  INDEX iTextureAnim,
  BOOL  bTextureLoop,
  INDEX iLightAnim,
  INDEX iAmbientLightAnim,
  BOOL  bLightLoop,
  BOOL  bAmbientLightLoop,
  COLOR colAmbient,
  COLOR colDiffuse,
  CTString strAnimation,
  FLOAT fBlendTime,
  ChangeAnimType AnimType,
};

class CAnimationChanger : CRationalEntity {
name      "AnimationChanger";
thumbnail "Thumbnails\\AnimationChanger.tbn";
features  "HasName", "HasTarget", "IsTargetable";

properties:
  1 CTString m_strName          "Name" 'N' = "Animation changer",
  2 CTString m_strDescription = "",
  3 CEntityPointer m_penTarget  "Target" 'T' COLOR(C_GREEN|0xFF),
  4 ANIMATION m_iModelAnim      "Model Animation" 'M' = 0,
  5 BOOL      m_bModelLoop      "Model Looping" = FALSE,
  6 ANIMATION m_iTextureAnim    "Texture Animation" 'X' = 0,
  7 BOOL      m_bTextureLoop    "Texture Looping" = FALSE,
  8 ANIMATION m_iLightAnim      "Diffuse Light Animation" 'L' = 0,
  9 BOOL      m_bLightLoop      "Diffuse Light Looping" = FALSE,
  10 ANIMATION m_iAmbientLightAnim "Ambient Light Animation" 'B' = 0,
  11 BOOL     m_bAmbientLightLoop  "Ambient Light Looping" = FALSE,
  12 COLOR m_colAmbient      "Ambient Light Color" 'A' = C_dBLUE,
  13 COLOR m_colDiffuse      "Diffuse Light Color" 'C' = C_GRAY,

  20 CTString m_strModelAnim    "Model Animation (SKA only)" = "",
  21 FLOAT m_fBlendTime         "Blend Time (SKA only)" = 0.05f,

components:
  1 editor model   MODEL_CHANGER     "Data\\Models\\Editor\\AnimationChanger.mdl",
  2 editor texture TEXTURE_CHANGER   "Data\\Models\\Editor\\AnimationChanger.tex"


functions:
  const CTString &GetDescription(void) const {
    ((CTString&)m_strDescription).PrintF("-><none>");
    if (m_penTarget!=NULL) {
      ((CTString&)m_strDescription).PrintF("->%s", m_penTarget->GetName());
    }
    return m_strDescription;
  }

  /* Get anim data for given animation property - return NULL for none. */
  CAnimData *GetAnimData(SLONG slPropertyOffset) 
  {
    CEntity *penTarget = m_penTarget;

    if (penTarget==NULL) {
      return NULL;
    }

    if (IsOfClass(penTarget, "AnimationHub")) {
      penTarget = ((CAnimationHub*)penTarget)->m_penTarget0;
    }

    if (penTarget==NULL) {
      return NULL;
    }

    // if modelholder
    if (IsOfClass(penTarget, "ModelHolder2")) {
      CModelHolder2 *penModel = (CModelHolder2*)&*penTarget;
      if (slPropertyOffset==offsetof(CAnimationChanger, m_iModelAnim)) {
        return penModel->GetModelObject()->GetData();
      } else if (slPropertyOffset==offsetof(CAnimationChanger, m_iTextureAnim)) {
        return penModel->GetModelObject()->mo_toTexture.GetData();
      } else if (slPropertyOffset==offsetof(CAnimationChanger, m_iLightAnim)) {
        return penModel->m_aoLightAnimation.GetData();
      }

    // if light
    } else if (IsOfClass(penTarget, "Light")) {
      CLight *penLight = (CLight*)&*penTarget;

      if (slPropertyOffset==offsetof(CAnimationChanger, m_iLightAnim))
      {
        return penLight->m_aoLightAnimation.GetData();
      }
      else if (slPropertyOffset==offsetof(CAnimationChanger, m_iAmbientLightAnim))
      {
        return penLight->m_aoAmbientLightAnimation.GetData();
      }
    }

    return CEntity::GetAnimData(slPropertyOffset);
  };

procedures:
  Main()
  {
    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    // set appearance
    SetModel(MODEL_CHANGER);
    SetModelMainTexture(TEXTURE_CHANGER);

    // check target type
    if (m_penTarget!=NULL && 
      !IsOfClass(m_penTarget, "AnimationHub") &&
      !IsOfClass(m_penTarget, "ModelHolder2") &&
      !IsOfClass(m_penTarget, "Light")) {
      WarningMessage("Target must be AnimationHub ModelHolder2 or Light!");
      m_penTarget=NULL;
    }
    if (m_penTarget==NULL) {
      return;
    }
    
    //main loop
    wait() {
      on (EBegin) : { resume; }
      on (EStart) : {
        EChangeAnim eChange;
        eChange.iModelAnim        =m_iModelAnim  ;
        eChange.iTextureAnim      =m_iTextureAnim;
        eChange.iLightAnim        =m_iLightAnim  ;
        eChange.iAmbientLightAnim =m_iAmbientLightAnim;
        eChange.bModelLoop        =m_bModelLoop  ;
        eChange.bTextureLoop      =m_bTextureLoop;
        eChange.bLightLoop        =m_bLightLoop  ;
        eChange.colAmbient        =m_colAmbient  ;
        eChange.colDiffuse        =m_colDiffuse  ;
        eChange.strAnimation      = m_strModelAnim;
        eChange.fBlendTime        = m_fBlendTime  ;
        m_penTarget->SendEvent(eChange,TRUE);
        resume;
      }
    }
    return;
  }
};

