231
%{
#include "StdH.h"
#include "EntitiesMP\Player.h"
extern INDEX ent_bReportBrokenChains;

// NOTE: GENDER_MALE has to be the same as the one in Player.es!!!!!!!!!!!!
#define GENDER_MALE 0
%}

class CVoiceHolder : CRationalEntity {
name      "VoiceHolder";
thumbnail "Thumbnails\\VoiceHolder.tbn";
features  "HasName", "IsTargetable";

properties:
  1 CTString m_strName          "Name" 'N' = "VoiceHolder",
  3 CTString m_strDescription = "",
  2 CTFileName m_fnmMessage  "Message" 'M' = CTString(""),
  5 BOOL m_bActive "Active" 'A' = TRUE,
  6 INDEX m_ctMaxTrigs            "Max trigs" 'X' = 1, // how many times could trig

  {
    CAutoPrecacheSound m_aps;
  }
components:
  1 editor model   MODEL_MARKER     "Data\\Models\\Editor\\VoiceHolder.mdl",
  2 editor texture TEXTURE_MARKER   "Data\\Models\\Editor\\VoiceHolder.tex"

functions:
  void Precache(void)
  {
    m_aps.Precache(m_fnmMessage);
  }
  const CTString &GetDescription(void) const {
    ((CTString&)m_strDescription).PrintF("%s", m_fnmMessage.FileName());
    return m_strDescription;
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
    wait() {
      on (ETrigger eTrigger): {
        if (!m_bActive) {
          resume;
        }
        CEntity *penCaused = FixupCausedToPlayer(this, eTrigger.penCaused);
        ASSERT( IsOfClass( penCaused, &CPlayer_DLLClass));
        if( IsOfClass( penCaused, &CPlayer_DLLClass)) {
          CPlayer *penPlayer = (CPlayer*)penCaused;
          // NOTE: GENDER_MALE has to be the same as the one in Player.es !
          if (penPlayer->m_iGender!=GENDER_MALE) {
            resume;
          }
        }
        EVoiceMessage eMsg;
        eMsg.fnmMessage = m_fnmMessage;
        penCaused->SendEvent(eMsg);
        // if max trig count is used for counting
        if(m_ctMaxTrigs > 0) {
          // decrease count
          m_ctMaxTrigs-=1;
          // if we trigged max times
          if( m_ctMaxTrigs <= 0) {
            // cease to exist
            Destroy();
            stop;
          }
        }
        resume;
      }
      on (EActivate): {
        m_bActive = TRUE;
        resume;
      }
      on (EDeactivate): {
        m_bActive = FALSE;
        resume;
      }
    }
    return;
  }
};

