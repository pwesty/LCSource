#include "stdh.h"

#include <Engine/Sound/SoundDecoder.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Sound/SoundData.h>
#include <Engine/Sound/SoundObject.h>
#include <Engine/Base/Statistics_Internal.h>
#include <Engine/Base/Console.h>

// asm shortcuts
#define O offset
#define Q qword ptr
#define D dword ptr
#define W  word ptr
#define B  byte ptr

#define ASMOPT 1


// console variables for volume
extern FLOAT snd_fSoundVolume;
extern FLOAT snd_fMusicVolume;


// a bunch of local vars coming up

static SLONG slMixerBufferSampleRate;  // quality of destination buffer
static SLONG slMixerBufferSize;        // size in samples per channel of the destination buffers
static SWORD *pswMixerBuffer;          // pointer to the start of the destination buffers

static SWORD *pswSrcBuffer;
static SLONG slLeftVolume,  slRightVolume, slLeftFilter, slRightFilter;
static SLONG slLastLeftSample, slLastRightSample, slSoundBufferSize;
static FLOAT fSoundSampleRate, fPhase;
static FLOAT fOfsDelta, fStep, fLeftStep, fRightStep, fLeftOfs, fRightOfs;
static __int64 fixLeftOfs, fixRightOfs; // fixed integers 32:32
static __int64 mmLeftStep, mmRightStep, mmVolumeGain;
static BOOL bNotLoop, bEndOfSound;

static const FLOAT f65536 = 65536.0f;
static const FLOAT f4G    = 4294967296.0f;
static __int64 mmInvFactor = 0x00007FFF00007FFF;



// reset mixer buffer (wipes it with zeroes and remembers pointers in static mixer variables)
extern void ResetMixer( const SWORD *pswBuffer, const SLONG slBufferSize)
{
  // clamp master volumes
  snd_fSoundVolume = Clamp( snd_fSoundVolume, 0.0f, 1.0f);
  snd_fMusicVolume = Clamp( snd_fMusicVolume, 0.0f, 1.0f);

  // cache local variables
  ASSERT( slBufferSize%4==0);
  pswMixerBuffer = (SWORD*)pswBuffer;
  slMixerBufferSize = slBufferSize /2/2; // because it's stereo and 16-bit dst format
  slMixerBufferSampleRate = _pSound->sl_wfeFormat.nSamplesPerSec;

  // wipe destination mixer buffer
  __asm {
    cld
    xor     eax,eax
    mov     edi,D [pswMixerBuffer]
    mov     ecx,D [slMixerBufferSize]
    rep     stosd
  }
}


// copy mixer buffer to the output buffer(s)
extern void CopyMixerBuffer( const SLONG slSrcOffset, const void *pDstBuffer, const SLONG slBytes)
{
  ASSERT( pDstBuffer!=NULL);
  ASSERT( slBytes%4==0);
  if( slBytes<4) return;
  __asm {
    cld
    mov     esi,D [slSrcOffset]
    add     esi,D [pswMixerBuffer]
    mov     edi,D [pDstBuffer]
    mov     ecx,D [slBytes]
    shr     ecx,2   // bytes to samples per channel
    rep     movsd
  }
}


// mixes one mono 16-bit signed sound to destination buffer
inline void MixMono(void)
{
  __asm {
    // convert from floats to fixints 32:16
    fld     D [fLeftOfs]
    fmul    D [f65536]
    fld     D [fRightOfs]
    fmul    D [f65536]
    fld     D [fLeftStep]
    fmul    D [f65536]
    fld     D [fRightStep]
    fmul    D [f4G]
    fistp   Q [mmRightStep] // fixint 32:32
    fistp   Q [mmLeftStep]  // fixint 32:16
    fistp   Q [fixRightOfs] // fixint 32:16
    fistp   Q [fixLeftOfs]  // fixint 32:16

    // get last played sample (for filtering purposes)
    movzx   eax,W [slLastRightSample]
    movzx   edx,W [slLastLeftSample]
    shl     eax,16
    or      eax,edx
    movd    mm6,eax                       // MM6 = 0 | 0 || lastRightSample | lastLeftSample

    // get volume
    movd    mm5,D [slRightVolume]
    movd    mm0,D [slLeftVolume]
    psllq   mm5,32
    por     mm5,mm0                       // MM5 = rightVolume || leftVolume

    // get filter
    mov     eax,D [slRightFilter]
    mov     edx,D [slLeftFilter]
    shl     eax,16
    or      eax,edx
    movd    mm7,eax                       // MM7 = 0 | 0 || rightFilter | leftFilter

    // get offset of each channel inside sound and loop thru destination buffer
    mov     W [mmRightStep],0
    movzx   eax,W [fixLeftOfs]
    movzx   edx,W [fixRightOfs]
    shl     edx,16
    or      eax,edx                       // EAX = right ofs frac | left ofs frac
    mov     ebx,D [fixLeftOfs+2]          // EBX = left ofs int
    mov     edx,D [fixRightOfs+2]         // EDX = right ofs int
    mov     esi,D [pswSrcBuffer]          // ESI = source sound buffer start ptr
    mov     edi,D [pswMixerBuffer]        // EDI = mixer buffer ptr
    mov     ecx,D [slMixerBufferSize]     // ECX = samples counter

sampleLoop:
    // check if source offsets came to the end of source sound buffer
    cmp     ebx,D [slSoundBufferSize]
    jl      lNotEnd
    sub     ebx,D [slSoundBufferSize]
    movd    mm0,D [bNotLoop]
    movd    D [bEndOfSound],mm0
lNotEnd:
    // same for right channel
    cmp     edx,D [slSoundBufferSize]
    jl      rNotEnd
    sub     edx,D [slSoundBufferSize]
    movd    mm0,D [bNotLoop]
    movd    D [bEndOfSound],mm0
rNotEnd:
    // check end of sample
    cmp     D [bEndOfSound],FALSE
    jne     loopEnd

    // get sound samples
    movd    mm1,D [esi+ ebx*2]    // MM1 = 0 | 0 || nextLeftSample  | leftSample
    movd    mm2,D [esi+ edx*2]    // MM2 = 0 | 0 || nextRightSample | rightSample
    psllq   mm2,32
    por     mm1,mm2   // MM1 = nextRightSample | rightSample || nextLeftSample | leftSample

    // calc linear interpolation factor (strength)
    movd    mm3,eax   // MM3 = 0 | 0 || right frac | left frac
    punpcklwd mm3,mm3
    psrlw   mm3,1     // MM3 = rightFrac | rightFrac || leftFrac | leftFrac
    pxor    mm3,Q [mmInvFactor] // MM3 = rightFrac | 1-rightFrac || leftFrac | 1-leftFrac
    // apply linear interpolation
    pmaddwd mm1,mm3
    psrad   mm1,15
    packssdw mm1,mm1  // MM1 = ? | ? || linearRightSample | linearLeftSample

    // apply filter
    psubsw  mm1,mm6
    pmulhw  mm1,mm7
    psllw   mm1,1
    paddsw  mm1,mm6
    movq    mm6,mm1

    // apply volume adjustment
    movq    mm0,mm5
    psrad   mm0,16
    packssdw mm0,mm0
    pmulhw  mm1,mm0
    psllw   mm1,1
    paddd   mm5,Q [mmVolumeGain]   // modify volume

    // mix it into destination buffer
    movd    mm0,D [edi]
    paddsw  mm1,mm0
    movd    D [edi],mm1

    // advance to next samples in source sound
    add     eax,D [mmRightStep+0]
    adc     edx,D [mmRightStep+4]
    add      ax,W [mmLeftStep +0]
    adc     ebx,D [mmLeftStep +2]
    add     edi,4
    dec     ecx
    jnz     sampleLoop

loopEnd:
    // store modified asm local vars
    mov     D [fixLeftOfs +0],eax
    shr     eax,16
    mov     D [fixRightOfs+0],eax
    mov     D [fixLeftOfs +2],ebx
    mov     D [fixRightOfs+2],edx
    movd    eax,mm6
    mov     edx,eax
    and     eax,0x0000FFFF
    shr     edx,16
    mov     D [slLastLeftSample],eax
    mov     D [slLastRightSample],edx
    emms
  }
}


// mixes one stereo 16-bit signed sound to destination buffer
inline void MixStereo(void)
{
  __asm {
    // convert from floats to fixints 32:16
    fld     D [fLeftOfs]
    fmul    D [f65536]
    fld     D [fRightOfs]
    fmul    D [f65536]
    fld     D [fLeftStep]
    fmul    D [f65536]
    fld     D [fRightStep]
    fmul    D [f4G]
    fistp   Q [mmRightStep] // fixint 32:32
    fistp   Q [mmLeftStep]  // fixint 32:16
    fistp   Q [fixRightOfs] // fixint 32:16
    fistp   Q [fixLeftOfs]  // fixint 32:16

    // get last played sample (for filtering purposes)
    movzx   eax,W [slLastRightSample]
    movzx   edx,W [slLastLeftSample]
    shl     eax,16
    or      eax,edx
    movd    mm6,eax                       // MM6 = 0 | 0 || lastRightSample | lastLeftSample

    // get volume
    movd    mm5,D [slRightVolume]
    movd    mm0,D [slLeftVolume]
    psllq   mm5,32
    por     mm5,mm0                       // MM5 = rightVolume || leftVolume

    // get filter
    mov     eax,D [slRightFilter]
    mov     edx,D [slLeftFilter]
    shl     eax,16
    or      eax,edx
    movd    mm7,eax                       // MM7 = 0 | 0 || rightFilter | leftFilter

    // get offset of each channel inside sound and loop thru destination buffer
    mov     W [mmRightStep],0
    movzx   eax,W [fixLeftOfs]
    movzx   edx,W [fixRightOfs]
    shl     edx,16
    or      eax,edx                       // EAX = right ofs frac | left ofs frac
    mov     ebx,D [fixLeftOfs+2]          // EBX = left ofs int
    mov     edx,D [fixRightOfs+2]         // EDX = right ofs int
    mov     esi,D [pswSrcBuffer]          // ESI = source sound buffer start ptr
    mov     edi,D [pswMixerBuffer]        // EDI = mixer buffer ptr
    mov     ecx,D [slMixerBufferSize]     // ECX = samples counter

sampleLoop:
    // check if source offsets came to the end of source sound buffer
    cmp     ebx,D [slSoundBufferSize]
    jl      lNotEnd
    sub     ebx,D [slSoundBufferSize]
    movd    mm0,D [bNotLoop]
    movd    D [bEndOfSound],mm0
lNotEnd:
    // same for right channel
    cmp     edx,D [slSoundBufferSize]
    jl      rNotEnd
    sub     edx,D [slSoundBufferSize]
    movd    mm0,D [bNotLoop]
    movd    D [bEndOfSound],mm0
rNotEnd:
    // check end of sample
    cmp     D [bEndOfSound],FALSE
    jne     loopEnd

    // get sound samples
    movq    mm1,Q [esi+ ebx*4]
    movq    mm2,Q [esi+ edx*4]
    pslld   mm1,16
    psrad   mm1,16              // MM1 = 0 | nextLeftSample  || 0 | leftSample
    psrad   mm2,16              // MM2 = 0 | nextRightSample || 0 | rightSample
    packssdw mm1,mm2  // MM1 = nextRightSample | rightSample || nextLeftSample | leftSample

    // calc linear interpolation factor (strength)
    movd    mm3,eax   // MM3 = 0 | 0 || right frac | left frac
    punpcklwd mm3,mm3
    psrlw   mm3,1     // MM3 = rightFrac | rightFrac || leftFrac | leftFrac
    pxor    mm3,Q [mmInvFactor] // MM3 = rightFrac | 1-rightFrac || leftFrac | 1-leftFrac
    // apply linear interpolation
    pmaddwd mm1,mm3
    psrad   mm1,15
    packssdw mm1,mm1  // MM1 = ? | ? || linearRightSample | linearLeftSample

    // apply filter
    psubsw  mm1,mm6
    pmulhw  mm1,mm7
    psllw   mm1,1
    paddsw  mm1,mm6
    movq    mm6,mm1

    // apply volume adjustment
    movq    mm0,mm5
    psrad   mm0,16
    packssdw mm0,mm0
    pmulhw  mm1,mm0
    psllw   mm1,1
    paddd   mm5,Q [mmVolumeGain]   // modify volume

    // mix it into destination buffer
    movd    mm0,D [edi]
    paddsw  mm1,mm0
    movd    D [edi],mm1

    // advance to next samples in source sound
    add     eax,D [mmRightStep+0]
    adc     edx,D [mmRightStep+4]
    add      ax,W [mmLeftStep +0]
    adc     ebx,D [mmLeftStep +2]
    add     edi,4
    dec     ecx
    jnz     sampleLoop

loopEnd:
    // store modified asm local vars
    mov     D [fixLeftOfs +0],eax
    shr     eax,16
    mov     D [fixRightOfs+0],eax
    mov     D [fixLeftOfs +2],ebx
    mov     D [fixRightOfs+2],edx
    movd    eax,mm6
    mov     edx,eax
    and     eax,0x0000FFFF
    shr     edx,16
    mov     D [slLastLeftSample],eax
    mov     D [slLastRightSample],edx
    emms
  }
}


// mixes one sound to destination buffer
void MixSound( CSoundObject &so, BOOL bAttenuate)
{
  // don't mix encoded sounds if they are not opened properly
  CSoundData &sd = *so.so_psdLink;
  if( (sd.sd_ulFlags&SDF_ENCODED) && (so.so_psdcDecoder==NULL || !so.so_psdcDecoder->IsOpen())) return;

  // check for supported sound formats
  const SLONG slChannels = sd.sd_wfeFormat.nChannels;
  const SLONG slBytes    = sd.sd_wfeFormat.wBitsPerSample/8;
  // unsupported sound formats will be ignored
  if( (slChannels!=1 && slChannels!=2) || slBytes!=2) return;

  // reach sound data and determine sound step, sound buffer and buffer size
  const FLOAT f1oMixerBufferSampleRate = 1.0f / slMixerBufferSampleRate;
  pswSrcBuffer = sd.sd_pswSamples;
  fSoundSampleRate = sd.sd_wfeFormat.nSamplesPerSec * so.so_fPitchShift;
  slSoundBufferSize = sd.sd_ctSamples;
  fStep = fSoundSampleRate * f1oMixerBufferSampleRate;
  fLeftStep = fRightStep = fStep;
  // eliminate potentional "puck" at the of sample that hasn't loop
  if( !(so.so_ulFlags&SOF_LOOP) && slSoundBufferSize>1) slSoundBufferSize--;

  // get old and new volumes
  const FLOAT fLeftVolume  = ClampDn( so.so_fLastLeftVolume,  0.0f);
  const FLOAT fRightVolume = ClampDn( so.so_fLastRightVolume, 0.0f);
  const FLOAT fClampVol = bAttenuate ? 0.0f : 1.0f; // apply forced volume attenuation
  FLOAT fNewLeftVolume  = Clamp( so.so_fLeftVolume,  0.0f, fClampVol);
  FLOAT fNewRightVolume = Clamp( so.so_fRightVolume, 0.0f, fClampVol);

  // if both channel volumes are too low
  if( fLeftVolume<0.001f && fRightVolume<0.001f && fNewLeftVolume<0.001f && fNewRightVolume<0.001f)
  {
    // if this is not an encoded sound
    if( !(sd.sd_ulFlags&SDF_ENCODED)) {
      // skip mixing of this sample segment
      fOfsDelta = fStep * slMixerBufferSize;
      so.so_fLeftOffset  += fOfsDelta;
      so.so_fRightOffset += fOfsDelta;
      const FLOAT fMinOfs = Min( so.so_fLeftOffset, so.so_fRightOffset);
      ASSERT( fMinOfs>=0);
      // if( fMinOfs<0) CPrintF( "BUG: negative offset (%.2g) encountered in sound: '%s' !\n", fMinOfs, (CTString&)sd.GetName());
      // if looping
      if( so.so_ulFlags & SOF_LOOP) {
        // adjust offset ptrs inside sound
        while( so.so_fLeftOffset  < 0) so.so_fLeftOffset  += slSoundBufferSize;
        while( so.so_fRightOffset < 0) so.so_fRightOffset += slSoundBufferSize;
        while( so.so_fLeftOffset  >= slSoundBufferSize) so.so_fLeftOffset  -= slSoundBufferSize;
        while( so.so_fRightOffset >= slSoundBufferSize) so.so_fRightOffset -= slSoundBufferSize;
      // if not looping
      } else {
        // no more playing
        so.so_ulFlags &= ~SOF_PLAYING;
      }
    }
    // reset last samples
    so.so_swLastLeftSample  = 0;
    so.so_swLastRightSample = 0;
    // update volume
    so.so_fLastLeftVolume  = fNewLeftVolume;
    so.so_fLastRightVolume = fNewRightVolume;
    return;
  }

  // cache sound object vars
  bNotLoop  = !(so.so_ulFlags&SOF_LOOP);
  fPhase    = so.so_fPhaseShift;
  fLeftOfs  = so.so_fLeftOffset;
  fRightOfs = so.so_fRightOffset;
  fOfsDelta = so.so_fOffsetDelta;
  slLeftVolume  = FloatToInt( fLeftVolume  * 65536*32767.0f);
  slRightVolume = FloatToInt( fRightVolume * 65536*32767.0f);
  const FLOAT fMixBufSize = 65536*32767.0f / slMixerBufferSize;
  const SLONG slLeftGain  = FloatToInt( (fNewLeftVolume -fLeftVolume)  *fMixBufSize);
  const SLONG slRightGain = FloatToInt( (fNewRightVolume-fRightVolume) *fMixBufSize);
  mmVolumeGain = ((__int64)(slRightGain)<<32) | ((__int64)(slLeftGain)&0xFFFFFFFF);
  // extrapolate back new volumes because of not enough precision in interpolation!
  // (otherwise we might hear occasional pucks)
  if( fNewLeftVolume >0.001f) fNewLeftVolume  = (slLeftVolume  + slLeftGain *slMixerBufferSize) /(65536*32767.0f);
  if( fNewRightVolume>0.001f) fNewRightVolume = (slRightVolume + slRightGain*slMixerBufferSize) /(65536*32767.0f);

  // if SW mixer
  if( !(_pSound->sl_ulFlags & SLF_USINGDS3D)) {
    // make some headroom to avoid clipping
    slLeftVolume  >>=1;
    slRightVolume >>=1;
  }

  // determine filtering
  slLeftFilter  = so.so_slLeftFilter;
  slRightFilter = so.so_slRightFilter;
  
  // if this is an encoded sound
  BOOL bDecodingFinished = FALSE;
  if( sd.sd_ulFlags&SDF_ENCODED) {
    // decode some samples from it
    SLONG slWantedBytes  = FloatToInt( slMixerBufferSize*fStep*sd.sd_wfeFormat.nChannels) *2;
    void *pvDecodeBuffer = _pSound->sl_pswDecodeBuffer;
    SLONG slDecodedBytes = so.so_psdcDecoder->Decode( pvDecodeBuffer, slWantedBytes);
    ASSERT( slWantedBytes  <= _pSound->sl_slDecodeBufferSize);
    ASSERT( slDecodedBytes <= slWantedBytes);
    // if it has a loop
    if( !bNotLoop) {
      // if sound is shorter than buffer
      while( slDecodedBytes<slWantedBytes) {
        // decode it again and again
        so.so_psdcDecoder->Reset();
        const SLONG slChunk = slWantedBytes-slDecodedBytes;
        slDecodedBytes += so.so_psdcDecoder->Decode( ((UBYTE*)pvDecodeBuffer)+slDecodedBytes, slChunk);
      }
    // if it doesn't have a loop
    } else {
      // if sound is shorter than buffer
      if( slDecodedBytes<slWantedBytes) {
        // mark that it is finished
        bDecodingFinished = TRUE;
      }
    }
    // copy first sample to the last one (this is needed for linear interpolation)
    (ULONG&)(((UBYTE*)pvDecodeBuffer)[slDecodedBytes]) = *(ULONG*)pvDecodeBuffer;
    // fix some mixer variables to play temporary decode buffer instead of real sound
    pswSrcBuffer = (SWORD*)pvDecodeBuffer;
    slSoundBufferSize = slDecodedBytes>>2;  // convert to samples
    fLeftOfs  = 0.0f;
    fRightOfs = 0.0f;
    fPhase    = 0.0f;
  }

  slLastLeftSample  = so.so_swLastLeftSample;
  slLastRightSample = so.so_swLastRightSample;

  // calculate eventual new offsets from phase shift
  FLOAT fLastPhase  = fOfsDelta / fSoundSampleRate;
  FLOAT fPhaseDelta = fPhase - fLastPhase;
  FLOAT fStepDelta  = Abs( fPhaseDelta*fSoundSampleRate / slMixerBufferSize);

  FLOAT fStepDeltaL, fStepDeltaR;
  if( fPhaseDelta>0) {
    fStepDeltaL = ClampUp( fStepDelta/2, fLeftStep/2);
    fStepDeltaL = -fStepDeltaL;
    fStepDeltaR =  fStepDelta + fStepDeltaL;
  } else {
    fStepDeltaR = ClampUp( fStepDelta/2, fRightStep/2);
    fStepDeltaR = -fStepDeltaR;
    fStepDeltaL =  fStepDelta + fStepDeltaR;
  }
  fLeftStep  += fStepDeltaL;
  fRightStep += fStepDeltaR;
  fStepDelta  = fStepDeltaR-fStepDeltaL;

  // if there is anything to mix (could be nothing when encoded file just finished)
  if( slSoundBufferSize>0) {
    // adjust offset ptrs inside sound to match those of phase shift
    ASSERT( fLeftOfs>=0 && fRightOfs>=0);
    while( fLeftOfs <0) fLeftOfs  += slSoundBufferSize;
    while( fRightOfs<0) fRightOfs += slSoundBufferSize;
    while( fLeftOfs >=slSoundBufferSize) fLeftOfs  -= slSoundBufferSize;
    while( fRightOfs>=slSoundBufferSize) fRightOfs -= slSoundBufferSize;

    // call corresponding mixer routine for current sound format
    bEndOfSound = FALSE;
    if( slChannels==2) {
      // mix as 16-bit stereo
      MixStereo();
    } else {
      // mix as 16-bit mono
      MixMono();
    }
  }

  // for encoded sounds, ignore mixing finished flag and use decoding finished flag
  if( sd.sd_ulFlags&SDF_ENCODED) bEndOfSound = bDecodingFinished;

  // if sound ended, not buffer
  if( bEndOfSound) {
    // reset sound
    slLastLeftSample  = 0;
    slLastRightSample = 0;
    so.so_ulFlags &= ~SOF_PLAYING;
  }

  // rememer last samples for the next mix in
  so.so_swLastLeftSample  = (SWORD)slLastLeftSample;
  so.so_swLastRightSample = (SWORD)slLastRightSample;
  // determine new phase shift offset
  so.so_fOffsetDelta += fStepDelta*slMixerBufferSize;
  // update play offset for the next mix iteration
  so.so_fLeftOffset  = fixLeftOfs  * (1.0f/65536.0f);
  so.so_fRightOffset = fixRightOfs * (1.0f/65536.0f);
  // update volume
  so.so_fLastLeftVolume  = fNewLeftVolume;
  so.so_fLastRightVolume = fNewRightVolume;
}

