#ifndef SE_INCL_SOUNDDATA_H
#define SE_INCL_SOUNDDATA_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <DSound.h>
#include <Engine/Base/Lists.h>
#include <Engine/Base/Serial.h>

#define SDF_ENCODED (1UL<<0) // this is ogg, mpX or wma compressed file

class ENGINE_API CSoundData : public CSerial {
#ifdef KALYDO
  static CTString strDefaultSoundPath;
#endif
public:
  struct {
    WAVEFORMATEX sd_wfeFormat;   // wave format info
    WORD sd_wSamplesPerBlock;    
  };
  LPDIRECTSOUNDBUFFER sd_pdsBuffer;  // pointer to direct sound buffer (not used in WaveOut mode!)
  SWORD *sd_pswSamples;              // pointer to wave data (not used in DirectSound mode!)
  DOUBLE sd_tmLength;         // sound length in seconds
  SLONG  sd_slBufferSize;     // buffer size in bytes
  INDEX  sd_ctSamples;        // buffer size in samples
  FLOAT  sd_fSoundMass;       // 'weight' of all sound objects currently playing this data (sum of volumes)
  ULONG  sd_ulFlags;          // flags
  UBYTE *sd_pubDecoderHeader; // header for sound decoder
  SLONG  sd_slHeaderSize;     // size of header for sound decoder

//private:
public:
  // free buffer (and all linked Objects)
  void ClearBuffer(void);
  // reference counting functions
  void AddReference(void);
  void RemReference(void);

public:
  CSoundData();
  ~CSoundData();
  // free allocated memory for sound and Sound in DXBuffer
  void Clear(void);

  // load wave-file - PCM will be auto-downsampled to MaxSampleRate
  void LoadWAV_t( CTStream *inFile, SLONG slMaxSampleRate);
  // read sound from file and convert it to the current sound format
  void Read_t( CTStream *inFile);
  // write sound to file (not implemented)
  void Write_t( CTStream *outFile);
 #ifdef KALYDO
  void Load_t(const CTFileName &fnFileName);
  void Load_Delay_t(const CTFileName &fnFileName);
  void Reload();
#endif

  // get sound length in seconds
  double GetSecondsLength(void);
  // get the description of this object
  CTString GetDescription(void);
  // get amount of memory used by this object
  SLONG GetUsedMemory(void);

  // check if this kind of objects is auto-freed
  virtual BOOL IsAutoFreed(void);
};


#endif  /* include-once check. */

