/*
 *  Sound Holder.
 */
204
%{
#include "StdH.h"
#include <Engine/Sound/SoundData.h>
%}

uses "EntitiesMP/ModelDestruction";

event EPlaySoundOnce { // when received, play sound only once
};

class CSoundHolder : CRationalEntity {
name      "SoundHolder";
thumbnail "Thumbnails\\SoundHolder.tbn";
features "HasName", "HasDescription", "IsTargetable";


properties:

  1 CTFileName m_fnSound   "Sound"    'S' = CTFILENAME("Sounds\\Default.wav"),    // sound
  2 RANGE m_rFallOffRange  "Fall-off" 'F' = 100.0f,
  3 RANGE m_rHotSpotRange  "Hot-spot" 'H' =  50.0f,
  4 FLOAT m_fVolume        "Volume"   'V' = 1.0f,
 16 FLOAT m_fPitch         "Pitch"    'P' = 1.0f,
  6 BOOL m_bLoop           "Looping"  'L' = TRUE,
  8 BOOL m_bVolumetric     "Volumetric" 'O' = TRUE,
 15 BOOL m_bMusic          "Music" 'M' = FALSE,
  9 CTString m_strName     "Name" 'N' = "",
 10 CTString m_strDescription = "",
 11 BOOL m_bAutoStart      "Auto start" 'A' = FALSE,    // auto start (environment sounds)
 12 INDEX m_iPlayType = 0,
 13 CSoundObject m_soSound,         // sound channel
 14 BOOL m_bDestroyable     "Destroyable" 'Q' = FALSE,

  {
    CAutoPrecacheSound m_aps;
  }


components:

  1 editor model   MODEL_MARKER     "Data\\Models\\Editor\\SoundHolder.mdl",
  2 editor texture TEXTURE_MARKER   "Data\\Models\\Editor\\SoundHolder.tex"


functions:

  void Precache(void)
  {
    m_aps.Precache(m_fnSound);
  }

  // apply mirror and stretch to the entity
  void MirrorAndStretch(FLOAT fStretch, BOOL bMirrorX)
  {
    // stretch its ranges
    m_rFallOffRange*=fStretch;
    m_rHotSpotRange*=fStretch;
    //(void)bMirrorX;  // no mirror for sounds
  }


  // returns bytes of memory used by this object
  SLONG GetUsedMemory(void)
  {
    // initial
    SLONG slUsedMemory = sizeof(CSoundHolder) - sizeof(CRationalEntity) + CRationalEntity::GetUsedMemory();
    // add some more
    slUsedMemory += m_strName.Length();
    slUsedMemory += m_strDescription.Length();
    slUsedMemory += m_fnSound.Length();
    slUsedMemory += 1* sizeof(CSoundObject);
    return slUsedMemory;
  }



procedures:
  Main(EVoid)
  {
    SetFlagOn(ENF_MARKDESTROY);
    SetFlagOn(ENF_NONETCONNECT);
    SetFlagOff(ENF_PROPSCHANGED);

    // fix parameters
    m_fVolume = Clamp( m_fVolume, 0.0f, 1.0f);
    m_fPitch  = Clamp( m_fPitch, 0.01f, 100.0f);
    m_rHotSpotRange = ClampDn( m_rHotSpotRange, 0.0f);
    m_rFallOffRange = ClampDn( m_rFallOffRange, m_rHotSpotRange+0.5f);

    // determine play type
    m_iPlayType = SOF_3D;
    if( m_bLoop) { m_iPlayType |= SOF_LOOP; }
    if( m_bMusic) { m_iPlayType |= SOF_MUSIC; }
    if( m_bVolumetric) { m_iPlayType |= SOF_VOLUMETRIC; }

    // init as model
    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    // set stretch factors - MUST BE DONE BEFORE SETTING MODEL!
    const FLOAT fFactor = ClampDn( Log2(m_rFallOffRange), 1.0f);
    
    GetModelObject()->mo_Stretch = FLOAT3D( fFactor, fFactor, fFactor);

    // set appearance
    SetModel(MODEL_MARKER);
    SetModelMainTexture(TEXTURE_MARKER);

    m_strDescription.PrintF("%s", (CTString&)m_fnSound.FileName());

    // wait for a while to play sound -> Sound Can Be Spawned 
    if( _pTimer->CurrentTick()<=0.1f) { autowait(0.5f); }

    wait() {
      // auto play sound
      on (EBegin) : {
        if (m_bAutoStart) {
          m_soSound.Set3DParameters(FLOAT(m_rFallOffRange), FLOAT(m_rHotSpotRange), m_fVolume, m_fPitch);
          PlaySound(m_soSound, m_fnSound, m_iPlayType);
        }
        resume;
      }
      // play sound
      on (EStart) : {
        m_soSound.Set3DParameters(FLOAT(m_rFallOffRange), FLOAT(m_rHotSpotRange), m_fVolume, m_fPitch);
        PlaySound(m_soSound, m_fnSound, m_iPlayType);
        resume;
      }
      // play sound only once
      on (EPlaySoundOnce) : {
        jump PlaySoundOnce();        
      }
      // stop playing sound
      on (EStop) : {
        m_soSound.Stop();
        resume;
      }
      // when someone in range is destroyed
      on (ERangeModelDestruction) : {
        // if range destruction is enabled
        if (m_bDestroyable) {
          // stop playing
          m_soSound.Stop();
        }
        return TRUE;
      }
      on (EEnd) : { stop; }
    }

    // cease to exist
    Destroy();
    return;
  }

  PlaySoundOnce() {
    m_soSound.Set3DParameters(FLOAT(m_rFallOffRange), FLOAT(m_rHotSpotRange), m_fVolume, m_fPitch, FALSE);
    PlaySound(m_soSound, m_fnSound, SOF_3D|SOF_VOLUMETRIC);
    FLOAT fSoundDuration = m_soSound.so_psdLink->GetSecondsLength();
    SetFlagOn(ENF_CLIENTHANDLING);
    autowait(fSoundDuration+0.05f);
    Destroy(FALSE);
    return;
  }
};
