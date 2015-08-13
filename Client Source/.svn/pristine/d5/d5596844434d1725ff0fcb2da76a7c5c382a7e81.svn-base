233
%{
#include "StdH.h"
#include "EntitiesMP/Player.h"
void HUD_SetEntityForStackDisplay(CRationalEntity *pren);
//void HUD_SetSkaDebug(BOOL bSkaDebug);
%}

enum DebugTargetType {
  0 DTT_MANUAL         "Manual target",
  1 DTT_PLAYER         "Player",
  2 DTT_PLAYERWEAPONS  "Player weapons",
  3 DTT_PLAYERANIMATOR "Player animator",
};

class CEntityStateDisplay : CRationalEntity {
name      "EntityStateDisplay";
thumbnail "Thumbnails\\EntityStateDisplay.tbn";
features  "HasName";

properties:
  1 CTString m_strName                 "Name" 'N' = "EntityStateDisplay",  
  2 CEntityPointer m_penManualTarget   "Manual Target" 'T' COLOR(C_dGREEN|0xFF), // entity which it points to
  3 enum DebugTargetType m_eTargetType "Target type" = DTT_MANUAL,
  4 BOOL m_bSkaStateDebug              "Show SKA states" = FALSE,


components:
  1 editor model   MODEL_MARKER     "Data\\Models\\Editor\\Debug_EntityStack.mdl",
  2 editor texture TEXTURE_MARKER   "Data\\Models\\Editor\\Debug_EntityStack.tex"

functions:
  void ~CEntityStateDisplay()
  {
    HUD_SetEntityForStackDisplay(NULL);
  }
  
  CPlayer *GetPlayer()
  {
    // find first available player
    for(INDEX iPlayer=0; iPlayer<GetMaxPlayers(); iPlayer++) {
      CEntity *pen = GetPlayerEntity(iPlayer);
      if (pen!=NULL) {
        if (_pNetwork->IsPlayerLocal(pen)) {
          return (CPlayer *)pen;
        }
      }
    }
    return NULL;
  }

  void ObtainTarget()
  {
    CEntity *penTarget = NULL;
    switch(m_eTargetType) {
    case DTT_MANUAL:
      penTarget = m_penManualTarget;
      break;
    case DTT_PLAYER:
      penTarget = (CEntity *)&*GetPlayer();
      break;
    case DTT_PLAYERWEAPONS:
      if (GetPlayer()==NULL) { return; }
      penTarget = (CEntity *)&*GetPlayer()->GetPlayerWeapons();
      break;
    case DTT_PLAYERANIMATOR:
      if (GetPlayer()==NULL) { return; }
      penTarget = (CEntity *)&*GetPlayer()->GetPlayerAnimator();
      break;
    }
      
    if (penTarget!=NULL) {
      HUD_SetEntityForStackDisplay((CRationalEntity *)penTarget);
      //HUD_SetSkaDebug(m_bSkaStateDebug);
    } else {
      HUD_SetEntityForStackDisplay(NULL);
      //HUD_SetSkaDebug(FALSE);
    }
  }

procedures:
  Main() {
    
     // init as nothing
    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    SetModel(MODEL_MARKER);
    SetModelMainTexture(TEXTURE_MARKER);

    GetModelObject()->StretchModel(FLOAT3D(0.4f, 0.4f, 0.4f));
    
    if (m_eTargetType != DTT_MANUAL) {
      m_penManualTarget = NULL;
    }

    // wait a bit before starting behaveour
    autowait(0.5f);

    // setup target for stack display every 1/5th of the second so
    // that after a reload or restart everything will work allright
    while(TRUE) {
      wait(0.2f)
      {
        on (EBegin) : {
          ObtainTarget();




          resume;
        }
        on (ETimer) : {
          stop;  
        }
      }
    }
    return;
  };
};
