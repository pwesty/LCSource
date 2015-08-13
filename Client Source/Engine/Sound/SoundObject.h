#ifndef SE_INCL_SOUNDOBJECT_H
#define SE_INCL_SOUNDOBJECT_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/Lists.h>
#include <Engine/Base/Timer.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Functions.h>


// sound control values
#define SOF_LOOP       (1L<<0)   // looping sound
#define SOF_3D         (1L<<1)   // has 3d effects
#define SOF_VOLUMETRIC (1L<<2)   // attenuate 3d effects
#define SOF_LOCAL      (1L<<3)   // local to listener with same entity
#define SOF_MUSIC      (1L<<4)   // use music-volume master control instead of sound-volume
#define SOF_NONGAME    (1L<<5)   // game sounds are not mixed while the game is paused
#define SOF_VOICE      (1L<<6)   // someone's talking (attenuate other sounds a bit)
// (internal)
#define SOF_WAITING    (1L<<28)  // paused automatically by engine
#define SOF_PAUSED     (1L<<29)  // playing, but paused (manually)
#define SOF_LOADED     (1L<<30)  // sound just loaded
#define SOF_PLAYING    (1L<<31)  // currently playing


class ENGINE_API CSoundObject {
public:
  CListNode so_Node;          // for linking in list of pending sounds
  CSoundData *so_psdLink;     // linked on SoundData
  CEntity *so_penEntity;      // entity that owns this sound (may be null)
  DOUBLE so_tmPlayBegin;      // when request for playing was issued
  DOUBLE so_tmNotAudible;     // when sound has begun to have volume=0
  DOUBLE so_tmFadeBegin;      // when fade has begun
  INDEX so_iPlayingBuffer;    // for hardware: in which playing buffer this object is currently playing (-1=none)
  SLONG so_ulFlags;           // playing flags

  // fader
  TIME  so_tmFadeTime;        // how long the fade will take
  FLOAT so_fFadeVolume;       // volume to fade to

  // input parameters (set by application)
  FLOAT so_fVolume;     // maximum sound volume
  FLOAT so_fPitch;      // sound pitch 1=normal
  FLOAT so_fHotSpot;    // sound at maximum volume
  FLOAT so_fFallOff;    // distance when sound can't be heard any more

  // HW setup and for sorting
  FLOAT3D so_vCurrentPosition;
  FLOAT3D so_vCurrentSpeed;
  FLOAT so_fCurrentDistance;
  FLOAT so_fCurrentVolume;
  FLOAT so_fCurrentFilter;

  // internal mixer parameters
  FLOAT so_fLeftVolume;         // left channel volume  (0.0f-1.0f)
  FLOAT so_fRightVolume;        // right channel volume
  SLONG so_slLeftFilter;        // left channel bass enhance (32767-0, 0=max)
  SLONG so_slRightFilter;       // right channel bass enhance
  FLOAT so_fPitchShift;         // playing speed factor (>0, 1.0=normal)
  FLOAT so_fPhaseShift;         // right channel(!) delay in seconds (signed! fixint 16:16)
  FLOAT so_fLastLeftVolume;     // volume from previous mixing (for seamless transition)
  FLOAT so_fLastRightVolume;
  SWORD so_swLastLeftSample;    // samples from previous mixing (for filtering purposes)
  SWORD so_swLastRightSample;
  FLOAT so_fLeftOffset;         // current playing offset of left channel
  FLOAT so_fRightOffset;        // current playing offset of right channel
  FLOAT so_fOffsetDelta;        // difference between offsets in samples (for seamless transition between phases)

  // only for sounds that are mpx/ogg
  class CSoundDecoder *so_psdcDecoder;  

public:
  // Constructor
  CSoundObject();
  // Destructor
  ~CSoundObject();
  // copy from another object of same class
  void Copy( CSoundObject &soOther);

  // control
  void Play( CSoundData *psdLink, ULONG ulFlags);   // play sound
  void Stop( const BOOL bNetwork=TRUE);             // stop playing sound
  void Stop_internal( const BOOL bReleasePlayingBuffer); // (for convenience)
  void Fade( const FLOAT fVolume, const TIME tmPeriod, const BOOL bNetwork=TRUE);  // fade to fVolume, thru tmPeriod (stops the sound at end if volume is 0)
  void Pause(void);     // stop playing sound, but keep it linked to data
  void Resume(void);    // resume playing paused sound
  // update sound state
  void Update(void);

  // check if sound is playing, paused or hooked to list
  inline BOOL IsPlaying(void) const { return (so_ulFlags&SOF_PLAYING); };
  inline BOOL IsPaused( void) const { return (so_ulFlags&SOF_PAUSED);  };
  inline BOOL IsHooked( void) const { return so_Node.IsLinked(); };

  // set sound parameters
  void SetVolume(const FLOAT fVolume, BOOL bNetwork=TRUE);
  void SetPitch( const FLOAT fPitch,  BOOL bNetwork=TRUE); // 1.0 for normal (<1 = slower, >1 = faster playing)
  void SetRange( const FLOAT fHotSpot, const FLOAT fFallOff, BOOL bNetwork=TRUE);
  inline void SetOwner( CEntity*pen) { so_penEntity = pen; };

  // temp wrapper
  inline void Set3DParameters( FLOAT fFallOff, FLOAT fHotSpot, FLOAT fVolume, FLOAT fPitch, BOOL bNetwork=TRUE) {
    SetVolume( fVolume, bNetwork);
    SetPitch( fPitch, bNetwork);
    SetRange( fHotSpot, fFallOff, bNetwork);
  };

  // read/write functions
  void Read_t(  CTStream *pistr, BOOL bNetwork=FALSE);
  void Write_t( CTStream *postr, BOOL bNetwork=FALSE);

  // Obtain sound and play it for this object
  void Play_t( const CTFileName &fnmSound, ULONG ulFlags);

  // timer functions (iTimer: 0=start playing, 1=start silence, 2=fadeout)
  void ResetTimer( const INDEX iTimer);
  TIME GetElapsedTime( const INDEX iTimer);
};


#endif  /* include-once check. */

