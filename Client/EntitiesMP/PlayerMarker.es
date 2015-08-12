404
%{
#include "StdH.h"
#include "EntitiesMP/MusicHolder.h"
#include "EntitiesMP/Player.h"
%}

uses "EntitiesMP/Marker";

event ETriggerPlayerMarker {
  CEntityID eidCaused,
};

%{
  extern void CPlayerWeapons_Precache(ULONG ulAvailable);
%}

class CPlayerMarker: CMarker {
name      "Player Marker";
thumbnail "Thumbnails\\PlayerMarker.tbn";
features "IsImportant";

properties:
  1 FLOAT m_fHealth          "Health" 'H' = 100.0f,
  2 FLOAT m_fShield          "Shield" 'S' = 0.0f,
  3 INDEX m_iGiveWeapons     "Give Weapons" 'W' = 0x1,
  4 INDEX m_iTakeWeapons     "Take Weapons"  = 0x0,
  5 CTString m_strGroup      "Group" 'G' = "",
  6 BOOL m_bQuickStart       "Quick start" 'Q' = FALSE,
  7 BOOL m_bStartInComputer  "Start in computer" 'C' = FALSE,
 30 BOOL m_bStartInComputerOnce "Start in computer only once" = TRUE,
  8 CEntityPointer m_penMessage  "Message" 'M',
  9 FLOAT m_fMaxAmmoRatio    "Max ammo ratio" 'A' = 0.0f,
 10 FLOAT m_tmLastSpawned = -1000.0f, // to avoid telefragging in deathmatch
 11 INDEX m_iTakeAmmo        "Take Ammo"  = 0x0,
 12 BOOL m_bNoRespawnInPlace "No Respawn In Place" 'R'  = FALSE,

components:
  1 editor model   MODEL_MARKER     "Data\\Models\\Editor\\PlayerStart.mdl",
  2 editor texture TEXTURE_MARKER   "Data\\Models\\Editor\\PlayerStart.tex",

functions:
  void Precache(void) {
    if (m_iGiveWeapons>1) {
      CPlayerWeapons_Precache(m_iGiveWeapons);
    }
  }
  BOOL HandleEvent(const CEntityEvent &ee) {
    if (ee.ee_slEvent == EVENTCODE_ETrigger) {
      CEntity *penMainMusicHolder = _pNetwork->GetEntityWithName("MusicHolder", 0);
      if( penMainMusicHolder==NULL || !IsOfClass( penMainMusicHolder, &CMusicHolder_DLLClass)) {
        return TRUE;
      }
      if (_pNetwork->IsServer()) {
        ETriggerPlayerMarker etpm; 
        etpm.eidCaused = ((ETrigger&)ee).penCaused;
        SendEvent(ETriggerPlayerMarker(),TRUE);
      }
      return TRUE;
    } else if (ee.ee_slEvent == EVENTCODE_ETriggerPlayerMarker) {
      CEntity *penMainMusicHolder = _pNetwork->GetEntityWithName("MusicHolder", 0);
      if( penMainMusicHolder==NULL || !IsOfClass( penMainMusicHolder, &CMusicHolder_DLLClass)) {
        return TRUE;
      }
      CMusicHolder *pmh = (CMusicHolder *)penMainMusicHolder;

      BOOL bNew = (pmh->m_iRespawnMarker!=en_ulID);
      pmh->m_iRespawnMarker = en_ulID;

      // if this is a new marker and we are in single player and the trigger originator is valid
      CEntity *penCaused = (CEntity*)(((ETriggerPlayerMarker&)ee).eidCaused);
      if( bNew && (GetSP()->sp_bSinglePlayer && GetSP()->sp_gmGameMode!=CSessionProperties::GM_FLYOVER)
       && penCaused!=NULL &&IsOfClass( penCaused, &CPlayer_DLLClass)) {
        // if the player wants auto-save
        CPlayerSettings *pps = (CPlayerSettings *) (((CPlayerEntity*)penCaused)->en_pcCharacter.pc_aubAppearance);
        /*
        if (pps->ps_ulFlags&PSF_AUTOSAVE) {
          // save now
         // _pShell->Execute("gam_bQuickSave=1;");
        }
        */
      }
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

    SetFlagOn(ENF_MARKDESTROY);
    SetFlagOn(ENF_NONETCONNECT);
    SetFlagOff(ENF_PROPSCHANGED);

    // set appearance
    SetModel(MODEL_MARKER);
    SetModelMainTexture(TEXTURE_MARKER);

    // set name
    if (m_bQuickStart) {
      m_strName.PrintF("Player Quick Start");
    } else {
      m_strName.PrintF("Player Start - %s", m_strGroup);
    }

    return;
  }
};

