#ifndef SE_INCL_SOUNDLIBRARY_H
#define SE_INCL_SOUNDLIBRARY_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif


#include <DSound.h>
#include <Engine/Base/Lists.h>
#include <Engine/Base/Synchronization.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Matrix.h>
#include <Engine/Templates/StaticArray.h>
#include <Engine/Templates/StaticStackArray.h>
#include <Engine/Templates/DynamicArray.h>

#ifdef PLATFORM_WIN32 /* rcg10042001 */

  #define DSBCAPS_GETPOS (DSBCAPS_GETCURRENTPOSITION2)
  #define DSBCAPS_2D (DSBCAPS_LOCDEFER|DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY|DSBCAPS_CTRLPAN)
  #define DSBCAPS_3D (DSBCAPS_LOCDEFER|DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLFREQUENCY|DSBCAPS_CTRL3D|DSBCAPS_MUTE3DATMAXDISTANCE)


#endif


// enums for environment effects
enum SndEnvFX
{
  SEFX_NORMAL      = 0,    // padded cell (or no-reverb)
  SEFX_GENERIC     = 1,    
  SEFX_LIVINGROOM  = 2,
  SEFX_STONEROOM   = 3,
  SEFX_AUDITORIUM  = 4,
  SEFX_HALLWAY     = 5,
  SEFX_ARENA       = 6,
  SEFX_STONEHALL   = 7,    // corridor
  SEFX_QUARRY      = 8,
  SEFX_MOUNTAINS   = 9,
  SEFX_PLAIN       = 10,
  SEFX_CAVE        = 11,
  SEFX_SEWERPIPE   = 12,
  SEFX_HANGAR      = 13,
  SEFX_FOREST      = 14,
  SEFX_CONCERTHALL = 15,
  SEFX_UNDERWATER  = 16,   // auto-set when player is in water
};

// buffers
#define SND_MINBUFFERS (5L)
#define SND_MAXBUFFERS (50L)

// flags
#define SLF_USINGWAVEOUT (1UL<<0)  // using WaveOut API (if not set, DirectSound is used)
#define SLF_USINGDS3D    (1UL<<1)  // using DirectSound3D mixer and positioning 
#define SLF_HASEFX       (1UL<<2)  // HW has environment effects (EAX and such)


/* !!! FIXME: rcg10042001 This is going to need OpenAL or SDL_audio... */


// holds the sound that is currently playing
struct PlayingBuffer
{
  CSoundData   *pb_psd;     // what sound data is playing
  CSoundObject *pb_pso;     // which sound object is playing
  LPDIRECTSOUNDBUFFER   pb_pdsb;
  LPDIRECTSOUND3DBUFFER pb_pdsb3d;  // (NULL if buffer is 2D)
  LPKSPROPERTYSET pb_pksps;  // for setting properties of EAX (NULL for 2D)
};


// main Sound Library class
class ENGINE_API CSoundLibrary {
public:
  enum SoundFormat {
    SF_NONE     = 0,
    SF_11025_16 = 1,
    SF_22050_16 = 2,
    SF_44100_16 = 3,
    SF_ILLEGAL  = 4
  };

/* rcg !!! FIXME: This needs to be abstracted. */
#ifdef PLATFORM_WIN32
  HWAVEOUT sl_hwoWaveOut;                   // wave out handle
  CStaticStackArray<HWAVEOUT> sl_ahwoExtra; // preventively taken channels
  CStaticArray<WAVEHDR> sl_awhWOBuffers;    // wave-out buffers
  INDEX sl_ctWaveDevices;                   // number of devices detected
  UBYTE *sl_pubBuffersMemory;               // memory for wave-out buffers
  LPDIRECTSOUNDBUFFER sl_pdsbPrimary;       // primary (for initialization on PC)
  LPKSPROPERTYSET sl_pKSProperty;           // for setting properties of EAX

  LPDIRECTSOUND sl_pDS;                     // direct sound 'handle'
  LPDIRECTSOUNDBUFFER sl_pdsbStream;        // streaming (for music)
  LPDIRECTSOUND3DLISTENER sl_pds3dListener; // for 3D positioning and effects

  CListHead sl_lhActiveListeners;   // active listeners for current frame of listening
  ULONG sl_ulFlags;                 // various flags (see #defines)

  SoundFormat  sl_sfFormat;         // sound format (external)
  WAVEFORMATEX sl_wfeFormat;        // primary sound buffer format
  SWORD *sl_pswMixerBuffer;         // buffer for mixing sounds
  SWORD *sl_pswDecodeBuffer;        // buffer for decoding encoded sounds (ogg, mpg, wma...)
  SLONG  sl_slMixerBufferSize;      // mixer buffer size
  SLONG  sl_slDecodeBufferSize;     // decoder buffer size

  // sound buffers for currently playing sounds
  PlayingBuffer sl_appbPlaying[SND_MAXBUFFERS];
  CListHead sl_lhPendingSounds; // list of sounds scheduled for playing (first couple from list are actually playing)
  DOUBLE sl_tmMuteUntil;        // time to keep sounds muted

#endif // PLATFORM_WIN32

private:
  // internal stuff
  void SetFormat_internal( CSoundLibrary::SoundFormat esfNew, BOOL bReport);
  BOOL StartUp_waveout( BOOL bReport=TRUE);
  BOOL StartUp_dsound(  BOOL bReport=TRUE);
  void ShutDown_waveout(void);
  void ShutDown_dsound(void);
  void CopyMixerBuffer_waveout(void);
  void CopyMixerBuffer_dsound( SLONG slMixedSize);
 SLONG PrepareSoundBuffer_waveout(void);
 SLONG PrepareSoundBuffer_dsound( void);

  BOOL DSFail( char *strError);
  BOOL DSLockBuffer( LPDIRECTSOUNDBUFFER pBuffer, SLONG slSize, LPVOID &lpData, DWORD &dwSize);
  void DSPlayMainBuffers(void);

  // update dsound buffers (if using HW mixer)
  void PlayBuffers(void);
  // update SW mixer
  void MixSounds(void);
  // request a playing buffer (bWasUsed=TRUE if this buffer was in use before by some quieter sound)
  INDEX CommitPlayingBuffer( CSoundObject &so, BOOL &bWasUsed);

public:
  CSoundLibrary(void);
  ~CSoundLibrary(void);
  DECLARE_NOCOPYING(CSoundLibrary);

  // return library state (active <==> format <> NONE
  inline BOOL IsActive(void) {return sl_sfFormat != SF_NONE;};
  // clear Library WaveOut or DirectSound structures and free memory
  void ClearLibrary(void);

  // init/clear
  void Init(void);
  void Clear(void);

  // set/get format
  void SetFormat( SoundFormat esfNew, BOOL bReport=FALSE);
  inline SoundFormat GetFormat(void) { return sl_sfFormat; };

  // update states of all sounds
  void UpdateSounds(void);
  // mute output until next UpdateSounds() or for next tmSeconds
  void Mute( const TIME tmSeconds=0);
  // stop all playing buffers
  void Flush(void);

  // set listener enviroment properties (EAX)
  void SetEnvironment( enum SndEnvFX eEnvFX);

  // add/remove object in pending list (and eventually release playing buffer, too)
  void AddToPendingList( CSoundObject &so);
  void RemoveFromPendingList( CSoundObject &so, const BOOL bReleaseNow=FALSE);

  // remove references to sound object and eventaully release direct-sound playing buffer
  void ReleasePlayingBuffer( const INDEX iBuffer, const BOOL bImmediately=FALSE);  

  // listen from this listener this frame
  void Listen( CSoundListener &sli);
};


// pointer to global sound library object
ENGINE_API extern CSoundLibrary *_pSound;


// listener's perameters
class CSoundListener {
public:
  CListNode sli_lnInActiveListeners;  // for linking for current frame of listening

  FLOAT3D sli_vPosition;          // listener position
  FLOATmatrix3D sli_mRotation;    // listener rotation matrix
  FLOAT3D sli_vSpeed;             // speed of the listener
  FLOAT sli_fVolume;              // listener volume (i.e. deaf factor)
  FLOAT sli_fFilter;              // global filter for all sounds on this listener
  CEntity *sli_penEntity;         // listener entity (for listener local sounds)
  enum SndEnvFX sli_eEnvType;     // environment sound effect
};


#endif  /* include-once check. */

