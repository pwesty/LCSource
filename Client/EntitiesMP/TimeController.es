613
%{
#include "StdH.h"
%}

class CTimeController: CRationalEntity {
name      "TimeController";
thumbnail "Thumbnails\\TimeController.tbn";
features  "IsTargetable", "HasName", "IsImportant";

properties:
  1 FLOAT m_fTimeStretch "Time speed" = 1.0f,
  2 FLOAT m_tmFadeIn     "Fade in time" = 0.25f,
  3 FLOAT m_tmInterval   "Auto clear stretch after..." = -1.0f,
  4 BOOL  m_bAbsolute    "Absolute" = TRUE,
  5 FLOAT m_fMyTimer=0.0f,
  6 FLOAT m_tmStretchChangeStart=0.0f,
  7 CTString m_strName   "Name" 'N' = "Time controller",
  8 FLOAT m_fOldTimeStretch=0.0f,
  9 FLOAT m_fNewTimeStretch=0.0f,

components:
  1 editor model   MODEL_TIME_CONTROLLER     "Data\\ModelsMP\\Editor\\TimeControler.mdl",
  2 editor texture TEXTURE_TIME_CONTROLLER   "Data\\ModelsMP\\Editor\\TimeController.tex"

functions:

procedures:
  
  Main(EVoid)
  {
    // set appearance
    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    // set appearance
    SetModel(MODEL_TIME_CONTROLLER);
    SetModelMainTexture(TEXTURE_TIME_CONTROLLER);
    
    // spawn in world editor
    autowait(0.1f);

    wait()
    {
      on (EBegin) :
      {
        resume;
      }
      // immediate
      on (EStart eStart) :
      {
        m_fOldTimeStretch=_pNetwork->GetRealTimeFactor();
        m_fNewTimeStretch=m_fTimeStretch;
        call ApplyTimeStretch();
        resume;
      }
      on (EStop) :
      {
        _pNetwork->SetRealTimeFactor(1.0f);
        resume;
      }
      on (EReturn) :
      {
        resume;
      }
    }
  }

  ChangeTimeStretch(EVoid)
  {
    m_fMyTimer=0.0f;
    while( m_fMyTimer<m_tmFadeIn-_pTimer->TickQuantum/2.0f)
    {
      autowait(_pTimer->TickQuantum);
      m_fMyTimer+=_pTimer->TickQuantum/_pNetwork->GetRealTimeFactor();
      FLOAT fNewStretch=Lerp(m_fOldTimeStretch, m_fNewTimeStretch, Clamp(m_fMyTimer/m_tmFadeIn, 0.0f, 1.0f));
      
      if (_pNetwork->ga_bDemoPlay) {
        _pNetwork->ga_fDemoRealTimeFactor = fNewStretch;
      } else {
        _pNetwork->SetRealTimeFactor(fNewStretch);
      }
    }
    if (_pNetwork->ga_bDemoPlay) {
      _pNetwork->ga_fDemoRealTimeFactor = m_fNewTimeStretch;
    } else {
      _pNetwork->SetRealTimeFactor(m_fNewTimeStretch);
    }
    return EReturn();  
  }

  ApplyTimeStretch(EVoid)
  {
    autocall ChangeTimeStretch() EReturn;
    if( m_tmInterval>0)
    {
      autowait(m_tmInterval);
      autocall ResetTimeStretch() EReturn;
    }
    return EReturn();
  }

  ResetTimeStretch(EVoid)
  {
    if(_pNetwork->GetRealTimeFactor()==1) {return EReturn(); };
    m_fOldTimeStretch=_pNetwork->GetRealTimeFactor();
    m_fNewTimeStretch=1.0f;
    autocall ChangeTimeStretch(EVoid()) EReturn;
    return EReturn();
  }
};
