#pragma once

class CSoundDecoder {
public:
  class CDecodeData_MPEG *sdc_pmpeg;
  class CDecodeData_OGG  *sdc_pogg;
  class CDecodeData_WMA  *sdc_pwma;

  // initialize/end the decoding support engine(s)
  static void InitPlugins(void);
  static void EndPlugins(void);

  // create a decoder that streams from file
  CSoundDecoder(const CTFileName &fnmStream, UBYTE *pubHeader=NULL, SLONG slHeaderSize=0);
  ~CSoundDecoder(void);
  void Clear(void);

  // check if a decoder is succefully opened
  BOOL IsOpen(void);
  // get wave format of the decoder (invaid if it is not open)
  void GetFormat(WAVEFORMATEX &wfe);
  // read decoder header if possible
  SLONG ReadHeader(void *pHeaderMemory, SLONG slHeaderSize);

  // decode a block of bytes
  INDEX Decode(void *pvDestBuffer, INDEX ctBytesToDecode);
  // reset decoder to start of sample
  void Reset(void);
};
