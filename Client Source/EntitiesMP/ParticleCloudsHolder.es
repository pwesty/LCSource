243
%{
#include "StdH.h"
%}

uses "EntitiesMP/ParticleCloudsMarker";

enum StreaksBlendType {
  0 SBT_BLEND "Blend",
  1 SBT_ADD "Add",
  2 SBT_MULTIPLY "Multiply",
  3 SBT_ADDALPHA "Addalpha",
  4 SBT_FLEX "Flex",
  5 SBT_TRANSPARENT "Transparent",
};

class CParticleCloudsHolder : CMovableModelEntity {
name      "ParticleCloudsHolder";
thumbnail "Thumbnails\\ParticleCloudsHolder.tbn";
features "HasName", "HasDescription", "IsImportant";

properties:

  1 INDEX m_ctTilesPerX                       "Tiles per x"          'X' = 1,
  2 FLOAT m_ctTilesPerY                       "Tiles per y"          'Y' = 1,
  3 CTString m_strName                        "Name"                 'N' = "Clouds holder",
  4 CTString m_strDescription                                         = "",
  6 CModelObject m_moCloudsParticleTextureHolder,
  7 CTFileName m_fnCloudsParticleTexture      "Clouds texture"       'T' = CTString(""),
  8 CEntityPointer m_penParticleCloudsMarker  "Clouds marker"        'K',
  9 CTFileName m_fnStreaksParticleTexture     "Streaks texture"      'S' = CTString(""),
 10 CModelObject m_moStreaksParticleTextureHolder,
 11 enum StreaksBlendType m_sbtStreaksBelndType "Streaks blend type" 'A' = SBT_ADD,
 12 BOOL m_bBackground                          "Background"         'B' = TRUE,

components:

  1 model   MODEL_MARKER     "Data\\Models\\Editor\\MovingBrushMarker.mdl",
  2 texture TEXTURE_MARKER   "Data\\Models\\Editor\\GravityMarker.tex"

functions:

  // render particles
  void RenderParticles(void)
  {
    CTextureData *ptd=(CTextureData*)m_moCloudsParticleTextureHolder.mo_toTexture.GetData();
    if(ptd!=NULL)
    {
//      ParticlesClouds(this, m_moCloudsParticleTextureHolder.mo_toTexture, m_ctTilesPerX, m_ctTilesPerY);
    }
  }

procedures:
  Main()
  {
    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    // set appearance
    SetModel(MODEL_MARKER);
    SetModelMainTexture(TEXTURE_MARKER);

    // setup background rendering flag
    if (m_bBackground) {
      SetFlags(GetFlags()|ENF_BACKGROUND);
    } else {
      SetFlags(GetFlags()&~ENF_BACKGROUND);
    }

    if( m_fnCloudsParticleTexture != CTString(""))
    {
      try
      {
        m_moCloudsParticleTextureHolder.mo_toTexture.SetData_t(m_fnCloudsParticleTexture);
      } catch (char *strError) {
        WarningMessage(strError);
      }
    }
    if( m_fnStreaksParticleTexture != CTString(""))
    {
      try
      {
        m_moStreaksParticleTextureHolder.mo_toTexture.SetData_t(m_fnStreaksParticleTexture);
      } catch (char *strError) {
        WarningMessage(strError);
      }
    }

    autowait(0.1f);
    return;
  }
};
