703
%{
#include "StdH.h"
%}

// input parameter for timer
event EReminderInit {
  CEntityID eidOwner,    // who owns it
  FLOAT fWaitTime,            // wait time
  INDEX iValue,               // reminder event value
};

class export CReminder : CRationalEntity {
name      "Reminder";
thumbnail "";

properties:
  1 CEntityPointer m_penOwner,    // entity which owns it
  2 FLOAT m_fWaitTime = 0.0f features(EPROPF_NETSEND),      // wait time
  3 INDEX m_iValue = 0  features(EPROPF_NETSEND),           // reminder event value
  4 INDEX m_iOwner = -1 features(EPROPF_NETSEND),           // ID of the owner

components:
functions:

  void InitializeFromNet() {
    EReminderInit eRInit;
    eRInit.eidOwner = m_iOwner;
    eRInit.fWaitTime = m_fWaitTime;
    eRInit.iValue = m_iValue;
    Initialize(eRInit);
  }

procedures:
  Main(EReminderInit eri) {
    // remember the initial parameters
    ASSERT(((CEntity*)eri.eidOwner)!=NULL);
    // prevent crashes in release...
    if (((CEntity*)eri.eidOwner)==NULL) {
      Destroy(FALSE);
      return;
    }
    m_penOwner = (CEntity*)eri.eidOwner;
    m_fWaitTime = eri.fWaitTime;
    m_iValue = eri.iValue;
    m_iOwner = ((CEntity*)eri.eidOwner)->en_ulID;
    
    // init as nothing
    InitAsVoid();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);
    SetFlagOn(ENF_CLIENTHANDLING);

    // wait
    if (m_fWaitTime > 0.0f) {
      autowait(m_fWaitTime);
      EReminder er;
      er.iValue = m_iValue;
      if (m_penOwner!=NULL) {
        m_penOwner->SendEvent(er);
      }
    }

    // cease to exist
    Destroy(FALSE);

    return;
  };
};
