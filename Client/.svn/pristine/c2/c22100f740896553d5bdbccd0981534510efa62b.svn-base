246
%{
#include "StdH.h"
%}

uses "EntitiesMP/MusicHolder";

event EMusicChangerTrigger {
};

%{
%}

class CMusicChanger : CRationalEntity {
name      "MusicChanger";
thumbnail "Thumbnails\\MusicChanger.tbn";
features "HasName", "HasDescription", "IsTargetable", "IsImportant";

properties:
  1 CTString m_strName   "Name" 'N' = "",
  2 CTString m_strDescription = "",
  3 CTFileName m_fnMusic "Music"   'M' = CTFILENAME(""),
  4 FLOAT m_fVolume "Volume" 'V' = 1.0f,
  5 enum MusicType m_mtType "Type" 'Y' = MT_EVENT,
  6 BOOL m_bForceStart "Force start" 'F' = TRUE,

  {
    CAutoPrecacheSound m_aps;
  }

components:
  1 editor model   MODEL_MARKER     "Data\\Models\\Editor\\MusicChanger.mdl",
  2 editor texture TEXTURE_MARKER   "Data\\Models\\Editor\\MusicChanger.tex"

functions:
  void Precache(void)
  {
    if(m_fnMusic!="") {
      m_aps.Precache(m_fnMusic);
    }
  }

procedures:
  // initialize music
  Main(EVoid) {

    // init as model
    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    // set appearance
    SetModel(MODEL_MARKER);
    SetModelMainTexture(TEXTURE_MARKER);

    m_strDescription.PrintF("%s: %s (%g)", 
      MusicType_enum.NameForValue((INDEX)m_mtType),
      (CTString&)m_fnMusic.FileName(),
      m_fVolume);

    // wait for game to start
    autowait(0.1f);

    // repeat forever
    wait() {
      // when triggered
      on (ETrigger) : {
        if (_pNetwork->IsServer()) {
          SendEvent(EMusicChangerTrigger(),TRUE);
        }
        resume;
      }
      on (EMusicChangerTrigger) : {
        // find music holder for this level
        CEntity *penMusicHolder = _pNetwork->GetEntityWithName("MusicHolder", 0);
        // if not existing
        if (penMusicHolder==NULL) {
          // error
          CPrintF("No MusicHolder on this level, cannot change music!\n");
        // if existing
        } else {
          // send event to change music
          EChangeMusic ecm;
          ecm.fnMusic = m_fnMusic;
          ecm.fVolume = m_fVolume;
          ecm.mtType  = m_mtType;
          ecm.bForceStart = m_bForceStart;
          penMusicHolder->SendEvent(ecm);
        }
        resume;
      };
    }

    return;
  }
};
