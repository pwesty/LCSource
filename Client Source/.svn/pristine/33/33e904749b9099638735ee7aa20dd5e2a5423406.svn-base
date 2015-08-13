244
%{
#include "StdH.h"
%}

class CParticleCloudsMarker : CEntity {
name      "ParticleCloudsMarker";
thumbnail "Thumbnails\\ParticleCloudsMarker.tbn";
features "HasName", "HasDescription", "IsTargetable", "IsImportant";

properties:

  1 CTString m_strDescription                                                   = "",
  2 CTString m_strName                            "Name"                    'N' = "Clouds layer",
  3 INDEX m_ctCount                               "Count"                   'C' = 16,
  4 FLOAT m_fRadiusMin                            "Radius min"              'M' = 2.0f,
  5 FLOAT m_fRadiusMax                            "Radius max"              'X' = 8.0f,
  6 FLOAT m_fPitchMin                             "Pitch min"               'O' = 10.0f,
  7 FLOAT m_fPitchMax                             "Pitch max"               'P' = 20.0f,
  8 FLOAT m_fSize                                 "Size"                    'Z' = 1.0f,
  9 FLOAT m_fRotPeriod                            "Rotation period"         'R' = 600.0f,
 10 FLOAT m_fFramePeriod                          "Frame period"            'F' = 32.0f,
 11 FLOAT m_fExtraCurvature                       "Extra curvature"         'U' = 0.0f,
 12 FLOAT m_fRndSizingRatio                       "Cloud random size"       'D' = 0.5f,
 13 COLOR m_colMultiply1                          "Multiply color 1"        'Q' = COLOR(C_WHITE|CT_OPAQUE),
 14 COLOR m_colMultiply2                          "Multiply color 2"        'W' = COLOR(C_WHITE|CT_OPAQUE),
 15 INDEX m_iFrameStart                           "Frame no start"              = 0,
 16 INDEX m_iFrameEnd                             "Frame no end"                = 0,
 17 BOOL  m_bVisible                              "Visible"                 'V' = TRUE,
 20 CEntityPointer m_penNextParticleCloudsMarker  "Next Parameters marker"  'K',
 21 INDEX m_iRndBase=0,
 22 CEntityPointer m_penSun                       "Sun for streaks"         'J',
 23 COLOR m_colStreaksMin                         "Streaks color min"       'G' = COLOR(C_WHITE|CT_OPAQUE),
 24 COLOR m_colStreaksMax                         "Streaks color max"       'H' = COLOR(C_WHITE|CT_OPAQUE),
 25 FLOAT m_fStreaksConePercentage                "Streaks cone (%)"        'A' = 50.0f,
 26 FLOAT m_fStreakSize                           "Streaks size"            'S' = 1.0f,
 27 FLOAT m_fRndStreakSize                        "Streak random size"      'E' = 0.0f,
 28 BOOL m_bRandomize                             "Randomize"               'L' = FALSE,
 29 FLOAT m_fStreaksRndMovementRatio              "Streaks rnd movement ratio" 'I' = 1.0f,
 30 INDEX m_ctStreaksPerCloud                     "Streaks per cloud"       'B' = 1,
 31 FLOAT m_fStreakPulsatingRatio                 "Streak pulsating ratio"  'Y' = 0.5f,
 32 FLOAT m_fStreakPulsatingPeroid                "Streak pulsating period" 'T' = 32.0f,
 33 BOOL m_bRenderClouds                          "Render clouds" = TRUE,
 

components:

  1 model   MODEL_MARKER     "Data\\Models\\Editor\\Axis.mdl",
  2 texture TEXTURE_MARKER   "Data\\Models\\Editor\\Vector.tex"

functions:

procedures:
  Main()
  {
    if(m_bRandomize)
    {
      m_bRandomize=FALSE;
      m_iRndBase=rand();
    }

    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    // set appearance
    SetModel(MODEL_MARKER);
    SetModelMainTexture(TEXTURE_MARKER);

    return;
  }
};
