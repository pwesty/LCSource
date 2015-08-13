211
%{
#include "StdH.h"
#include "EntitiesMP/WorldSettingsController.h"
%}

uses "EntitiesMP/Marker";

event ESetViewer {
};

class CBackgroundViewer: CMarker {
name      "Background Viewer";
thumbnail "Thumbnails\\BackgroundViewer.tbn";
features "IsImportant";

properties:
  1 BOOL m_bActive  "Active" 'A' =TRUE,   // set if active at beginning of game
  2 CEntityPointer m_penWorldSettingsController  "World settings controller" 'W',

components:
  1 editor model   MODEL_MARKER     "Data\\Models\\Editor\\Axis.mdl",
  2 editor texture TEXTURE_MARKER   "Data\\Models\\Editor\\Vector.tex"

functions:
  // Validate offered target for one property
  BOOL IsTargetValid(SLONG slPropertyOffset, CEntity *penTarget)
  {
    if(penTarget==NULL)
    {
      return FALSE;
    }
    // if gradient marker
    if( slPropertyOffset==offsetof(CBackgroundViewer, m_penWorldSettingsController))
    {
      return IsOfClass( penTarget, &CWorldSettingsController_DLLClass);
    }
    return TRUE;
  }

  BOOL HandleEvent(const CEntityEvent &ee) {
    if (ee.ee_slEvent == EVENTCODE_EStart) {
      if (_pNetwork->IsServer()) {
        SendEvent(ESetViewer(),TRUE);
      }
      return TRUE;
    } else if (ee.ee_slEvent == EVENTCODE_ESetViewer) {
      GetWorld()->SetBackgroundViewer(this);
      return TRUE;
    }

    return FALSE;
  };

procedures:
  Main()
  {
    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    // set appearance
    SetModel(MODEL_MARKER);
    SetModelMainTexture(TEXTURE_MARKER);

    // set name
    if (m_strName=="Marker") {
      m_strName = "Background Viewer";
    }

    if (m_bActive) {
      GetWorld()->SetBackgroundViewer(this);
    }
    return;
  }
};

