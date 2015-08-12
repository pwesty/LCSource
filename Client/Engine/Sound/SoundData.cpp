#include "stdh.h"

#include <Engine/Sound/SoundData.h>

#include <Engine/Base/Memory.h>
#include <Engine/Base/Stream.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Sound/SoundDecoder.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Sound/SoundObject.h>

#include <Engine/Templates/Stock_CSoundData.h>
#include <Engine/Base/MemoryTracking.h>

#ifdef KALYDO
#include <Kalydo/KRFReadLib/Include/KRFReadLib.h>
CTString CSoundData::strDefaultSoundPath = "data\\defaults\\default.wav";
#endif

#define SD_ENCHEADERSIZE 16*1024 // header size for sound decoder

// Constructor
CSoundData::CSoundData()
{
  sd_pswSamples = NULL;
  sd_pdsBuffer  = NULL;
  sd_pubDecoderHeader = NULL;
  sd_slHeaderSize = 0;
  sd_ctSamples = 0;      
  sd_tmLength = 0.0;
  sd_ulFlags = NONE;      
  sd_fSoundMass = 0;
  sd_wSamplesPerBlock = 64;  // just in case
}

// Destructor
CSoundData::~CSoundData()
{
  Clear();
}


// Free Buffer (and all linked Objects)
void CSoundData::ClearBuffer(void)
{
  // cannot have both at same time
  ASSERT( sd_pswSamples==NULL || sd_pdsBuffer==NULL);

  // if this is decoded sound
  if(sd_pubDecoderHeader!=NULL) {
    // delete decoder header
    delete[] sd_pubDecoderHeader;
    sd_pubDecoderHeader = NULL;
  }
  // if sample buffer exist
  if( sd_pswSamples!=NULL) {
    // release it
    FreeMemory( sd_pswSamples);
    sd_pswSamples = NULL;
  }

  // if direct sound buffer exist
  if( sd_pdsBuffer!=NULL) {
    sd_pdsBuffer->Release();
    sd_pdsBuffer = NULL;
  }
}


// Get Sound Length in seconds
DOUBLE CSoundData::GetSecondsLength(void)
{
  // if not encoded
  if (!(sd_ulFlags&SDF_ENCODED) ) {
    // len is read from wave
    return sd_tmLength;
  // if encoded
  } else {
    // implement this!!!!
    ASSERT(FALSE);
    return 0;
  }
}


/* ====================================================
 *
 *  Input methods (load sound in SoundData class)
 */

// Read sound in memory
void CSoundData::Read_t( CTStream *inFile)  // throw char *
{
  ASSERT( sd_pswSamples==NULL && sd_pdsBuffer==NULL);
  sd_ulFlags = NONE;
  sd_tmLength   = 0;
  sd_ctSamples  = 0;      
  sd_fSoundMass = 0;

  // get filename
  CTFileName fnm = inFile->GetDescription();

  // if this is encoded file
  if( fnm.FileExt()==".ogg" || fnm.FileExt()==".mp3" || fnm.FileExt()==".wma") {
    // allocate decoder header
    ASSERT(sd_pubDecoderHeader==NULL);
    CSoundDecoder *pmpd = new CSoundDecoder(fnm);
    // if opened
    if(pmpd->IsOpen()) {
      // get decoder format
      pmpd->GetFormat(sd_wfeFormat);
      sd_slHeaderSize = SD_ENCHEADERSIZE;
      // precache decoder header
      sd_pubDecoderHeader = new UBYTE[sd_slHeaderSize];
      SLONG slReadHeader = pmpd->ReadHeader(sd_pubDecoderHeader, sd_slHeaderSize);
      if(slReadHeader==(-1)) {
        // delete header buffer
        delete[] sd_pubDecoderHeader;
        sd_pubDecoderHeader = NULL;
      } else {
        sd_slHeaderSize = slReadHeader;
      }
    }
    delete pmpd;
    // mark that this is streaming encoded file
    sd_ulFlags = SDF_ENCODED;
  }
  // if this is wave file
  else {
    // load sound data and eventually report anomalies (for debugging purposes only!)
    const SLONG slMaxSampleRate = _pSound->IsActive() ? _pSound->sl_wfeFormat.nSamplesPerSec : 0;
    LoadWAV_t( inFile, slMaxSampleRate);  // sample rate is valid only if library is active 
    extern INDEX snd_bReportStereoWaves;
    if( snd_bReportStereoWaves) {
      if( sd_wfeFormat.nChannels     !=1) CPrintF( "! Stereo wave: %s\n", fnm);
      if( sd_wfeFormat.wBitsPerSample==8) CPrintF( "! 8-bits wave: %s\n", fnm);
    }
    // determine length of sound in seconds
    sd_tmLength = sd_ctSamples / (DOUBLE)sd_wfeFormat.nSamplesPerSec;
    sd_wSamplesPerBlock = 64;  // for the sake of ADPCM 
  }
}


// Sound can't be written to file
void CSoundData::Write_t( CTStream *outFile)
{
  ASSERTALWAYS("Cannot write sounds!");
  throw TRANS("Cannot write sounds!");
}


// get the description of this object
CTString CSoundData::GetDescription(void)
{
  CTString str;
  str.PrintF( "%dkHz %dbit %s %.2lfs",
    sd_wfeFormat.nSamplesPerSec/1000,
    sd_wfeFormat.wBitsPerSample,
    sd_wfeFormat.nChannels==1 ? "mono" : "stereo",
    sd_tmLength);
  return str;
}


/* ====================================================
 *
 *  Class CLEAR method
 */

// Free memory allocated for sound and Release DXBuffer
void CSoundData::Clear(void)
{
  // clear base class
  CSerial::Clear();
  // free buffer
  ClearBuffer();
}

#ifdef KALYDO
static void KCPSoundDownloaded(const char* fileName, TKResult result, void* id)
{
	switch (result)
	{
	case KR_OK:
		{
	// 		((CSerial*)id)->Clear();
	// 		((CSerial*)id)->Load_t( CTFileName( fileName ) );
			SLS* pSLS = new SLS();
			pSLS->pTarget = reinterpret_cast<CSerial*>(id);
			pSLS->pTargetFilePath = fileName;
			g_deqLoadData.push_back( pSLS );
		}
		break;
	case KR_DOWNLOAD_FAILED:
	case KR_FILE_CORRUPT:
		krfRequestKCPFile(fileName, &KCPSoundDownloaded, id);
	//default:
		// unknown error!
	}
}

void CSoundData::Load_t(const CTFileName &fnFileName)
{
  ASSERT(!IsUsed());
  // mark that you have changed
  MarkChanged();
  // 근데 이게 확실한가?? 호출 매커니즘의 정확한 해명이 필요할 거 같다.

  TKResult tkResult = krfRequestKCPFile( fnFileName, NULL, NULL );
  if( KR_OK == tkResult )
  {
	// open a stream
	CTFileStream istrFile;
	istrFile.Open_t(fnFileName);
	// read object from stream
	Read_t(&istrFile);
	// if still here (no exceptions raised)
	// remember filename
	ser_FileName = fnFileName;
  }
  else
  {
	CPrintF("Request file to kcp : %s\n", fnFileName );
	//?????????????
	CTFileStream istrFile;
	istrFile.Open_t( strDefaultSoundPath );
	Read_t(&istrFile);
	ser_FileName = fnFileName;
	// 위쪽에서 요청하면서 이미 처리됨.
	if( KR_FILE_NOT_AVAILABLE == tkResult )
	{
		MarkUsed();
	}
	tkResult = krfRequestKCPFile(fnFileName, &KCPSoundDownloaded, this);
	if( KR_FILE_NOT_FOUND == tkResult )
	{
		CPrintF("[Load_t] Sound File Not Found in kalydo...\n" );
	}
	else if( KR_IO_PENDING == tkResult )
	{
		CPrintF("[Load_t] Sound File already request...\n" );
	}
	else
	{
		;
	}
  }  
}

void CSoundData::Load_Delay_t(const CTFileName &fnFileName)
{
  // mark that you have changed
  MarkChanged();
  // 근데 이게 확실한가?? 호출 매커니즘의 정확한 해명이 필요할 거 같다.

  //if( kfileExists( fnFileName ) )
	// open a stream
	CTFileStream istrFile;
	istrFile.Open_t(fnFileName);
	// read object from stream
	Read_t(&istrFile);
	// if still here (no exceptions raised)
	// remember filename
	ser_FileName = fnFileName;
	MarkUnused();
}

void CSoundData::Reload(void)
{
	/* if not found, */
	TRACKMEM(Mem, strrchr((const char*)ser_FileName, '.'));

	// mark that you have changed
	MarkChanged();

	CTFileName fnmOldName = ser_FileName;
	Clear();

	TKResult tkResult = krfRequestKCPFile( fnmOldName, NULL, NULL );
	if( KR_OK == tkResult )
	{
		// try to
		try {
		  // open a stream
		  CTFileStream istrFile;
		  istrFile.Open_t(fnmOldName);
		  // read object from stream
		  Read_t(&istrFile);

		// if there is some error while reloading
		} catch (char *strError) {
		  // quit the application with error explanation
		  FatalError(TRANS("Cannot reload file '%s':\n%s"), (CTString&)fnmOldName, strError);
		}

		// if still here (no exceptions raised)
		// remember filename
		ser_FileName = fnmOldName;
	}
	else
	{
		// it is already request in load_t()
		CPrintF("Request file to kcp : %s\n", fnmOldName );
		//?????????????
		CTFileStream istrFile;
		istrFile.Open_t( strDefaultSoundPath );
		Read_t(&istrFile);
		ser_FileName = fnmOldName;
	}
}

#endif


// check if this kind of objects is auto-freed
BOOL CSoundData::IsAutoFreed(void)
{
  return FALSE;
}


// get amount of memory used by this object
SLONG CSoundData::GetUsedMemory(void)
{
  SLONG slUsed = sizeof(*this);
  if( sd_pswSamples!=NULL || sd_pdsBuffer!=NULL) {
    ASSERT( sd_wfeFormat.nChannels==1 || sd_wfeFormat.nChannels==2);
    SLONG slSize = sd_ctSamples * sd_wfeFormat.nChannels *sd_wfeFormat.wBitsPerSample/8;
    slUsed += slSize; 
    if( sd_pdsBuffer!=NULL) slUsed += sizeof(sd_pdsBuffer);
  } 
  return slUsed;
}



// reference counting functions
void CSoundData::AddReference(void)
{
  if( this!=NULL) MarkUsed();
}

void CSoundData::RemReference(void)
{
  if( this!=NULL) _pSoundStock->Release(this);
}




// WAVE FILE LOADER ROUTINES


// load wave file into SoundData class
void CSoundData::LoadWAV_t( CTStream *inFile, SLONG slMaxSampleRate)
{
  ULONG ulDummy;
  inFile->ExpectID_t(CChunkID("RIFF"));
  (*inFile) >> ulDummy;

  SLONG slFormatLength;
  inFile->ExpectID_t(CChunkID("WAVE"));
  inFile->ExpectID_t(CChunkID("fmt "));
  (*inFile) >> slFormatLength;     // Format Chunk length

  // read WAVE format
  WAVEFORMATEX &wfe = sd_wfeFormat;
  (*inFile) >> wfe.wFormatTag;
  (*inFile) >> wfe.nChannels;
  (*inFile) >> wfe.nSamplesPerSec;
  (*inFile) >> wfe.nAvgBytesPerSec;
  (*inFile) >> wfe.nBlockAlign;
  (*inFile) >> wfe.wBitsPerSample;
  wfe.cbSize = 0;   // for PCM

  // only for PCM Wave - skip extra information if exists
  if( slFormatLength>16) inFile->Seek_t( slFormatLength-16, CTStream::SD_CUR);

  // skip 'fact' and/or 'PAD ' chunks if exist
  CChunkID cID = inFile->GetID_t();
  if( cID == CChunkID("fact")) {
    SLONG slSkipSize;
    (*inFile) >> slSkipSize;
    inFile->Seek_t( slSkipSize, CTStream::SD_CUR);
    cID = inFile->GetID_t();
  }
  if( cID == CChunkID("PAD ")) {
    SLONG slSkipSize;
    (*inFile) >> slSkipSize;
    inFile->Seek_t( slSkipSize, CTStream::SD_CUR);
    cID = inFile->GetID_t();
  }
//안태훈 수정 시작	//(For New Snd Format)(0.1)
  if(cID == CChunkID("cue "))
  {
	  SLONG slSkipSize;
		(*inFile) >> slSkipSize;
		inFile->Seek_t(slSkipSize, CTStream::SD_CUR);
		cID = inFile->GetID_t();
  }
//안태훈 수정 끝	//(For New Snd Format)(0.1)

  // read data length (in bytes)
  ULONG ulDataLength;
  if( cID != CChunkID("data")) 
  {
	  throw "Wrong chunk ID ('data' expected)!";
  }
  (*inFile) >> ulDataLength;

  // check wave format
  if( wfe.wFormatTag != WAVE_FORMAT_PCM
  ) {
    throw "Unsupported format tag in wave file!";
  }
  if( wfe.wBitsPerSample != 4
   && wfe.wBitsPerSample != 8 
   && wfe.wBitsPerSample != 16) {
    throw "Unsupported BitsPerSample value in wave file!";
  }
  if( wfe.nChannels != 1
   && wfe.nChannels != 2) {
    throw "Unsupported number of channels in wave file!";
  }
  if( wfe.nSamplesPerSec != 11025
   && wfe.nSamplesPerSec != 22050
   && wfe.nSamplesPerSec != 44100) {
    throw "Unsupported frequency of wave file!";
  }

  // some booleans
  const BOOL bADPCM = (wfe.wFormatTag != WAVE_FORMAT_PCM);
  const BOOL b16Bit = (wfe.wBitsPerSample == 16);
  const BOOL bMono  = (wfe.nChannels == 1);

  // if library is not active yet
  if( slMaxSampleRate==0) {
    // just calc sound size
    if( bADPCM) {
      sd_wSamplesPerBlock = 64; 
      sd_ctSamples = ulDataLength * sd_wSamplesPerBlock / wfe.nBlockAlign;
    } else {
      sd_ctSamples = ulDataLength *8 / wfe.wBitsPerSample / wfe.nChannels;
    }
    // we're done here
    return;
  }

  // load samples
  DOUBLE dSampleRatio = 1;
  void *pBuffer = NULL;
  // ADPCM does not need conversion or shrinking
  if( bADPCM) {
    // must align memory for ADPCM to one block (36 bytes)
    sd_pswSamples = (SWORD*)AllocMemory( ulDataLength);
    inFile->Read_t( sd_pswSamples, ulDataLength);
    wfe.cbSize = 2;   // for ADPCM
    sd_wSamplesPerBlock = 64; 
    sd_ctSamples = ulDataLength * sd_wSamplesPerBlock / wfe.nBlockAlign;
  }
  // PCM might need conversion and/or shrinking
  else {
    dSampleRatio = Min( (DOUBLE)slMaxSampleRate/(DOUBLE)wfe.nSamplesPerSec, 1.0);
    const SLONG slMul = b16Bit ? 2 : 4;
    pBuffer = AllocMemory( ulDataLength*slMul);
    inFile->Read_t( pBuffer, ulDataLength);

    // convert to 32-bit
    if( slMul==2) {
      // from 16-bit
      sd_ctSamples = ulDataLength /2;
      for( INDEX i=sd_ctSamples-1; i>=0; i--) {
        ((ULONG*)pBuffer)[i] = (((SWORD*)pBuffer)[i] +0x8000) <<8;
      }
    } else {
      // from 8-bit
      sd_ctSamples = ulDataLength;
      for( INDEX i=sd_ctSamples-1; i>=0; i--) {
        ((ULONG*)pBuffer)[i] = ((UBYTE*)pBuffer)[i] <<16;
      } // update format
      wfe.wBitsPerSample   = 16;
      wfe.nAvgBytesPerSec *= 2;
      wfe.nBlockAlign     *= 2;
      ulDataLength *= 2;
    }
    // set samples in total (not per channel)
    if( !bMono) sd_ctSamples /= wfe.nChannels;
  }

  // shrink if needed
  if( dSampleRatio<1.0) {
    // prepare initial vars
    dSampleRatio  = 1.0 / dSampleRatio;
    ULONG *pulSrc = (ULONG*)pBuffer;
    ULONG *pulDst = pulSrc;
    DOUBLE dRatio = dSampleRatio;
    DOUBLE dInterDataL = 0.0;
    DOUBLE dInterDataR = 0.0;
    DOUBLE dTempDataL, dTempDataR;

    for( INDEX i=0; i<sd_ctSamples; i++)
    {
      // left (or mono) channel
      if( dRatio<1.0) { // partial sample
        dTempDataL = *pulSrc++;
        dInterDataL += dTempDataL*dRatio;
        *pulDst++ = (ULONG)(dInterDataL/dSampleRatio);
        // new intermediate value
        dRatio = 1 - dRatio;
        dInterDataL = dTempDataL*dRatio;
        dRatio = dSampleRatio - dRatio;
      } else { // complete sample
        dInterDataL += *pulSrc++;
        dRatio -= 1.0;
      }
      // done here if mono
      if( bMono) continue;

      // right channel
      if( dRatio<1.0) { // partial sample
        dTempDataR = *pulSrc++;
        dInterDataR += dTempDataR*dRatio;
        *pulDst++ = (ULONG)(dInterDataR/dSampleRatio);
        // new intermediate value
        dRatio = 1 - dRatio;
        dInterDataR = dTempDataR*dRatio;
        dRatio = dSampleRatio - dRatio;
      } else { // complete sample
        dInterDataR += *pulSrc++;
        dRatio -= 1.0;
      }
    }
    // last sample(s)
                *pulDst++ = (ULONG)(dInterDataL/(dSampleRatio-dRatio));
    if( !bMono) *pulDst++ = (ULONG)(dInterDataR/(dSampleRatio-dRatio));

    // update sample rate and sample count
    wfe.nSamplesPerSec  = slMaxSampleRate;
    wfe.nAvgBytesPerSec = slMaxSampleRate * wfe.nChannels *16/8;
    sd_ctSamples = sd_ctSamples /dSampleRatio;
    ulDataLength = ulDataLength /dSampleRatio;
  }

  // copy or convert to direct-sound buffer 
  const INDEX ctStereoSamples = sd_ctSamples * wfe.nChannels;
  sd_slBufferSize = ulDataLength;

  if( _pSound->sl_ulFlags&SLF_USINGDS3D)
  {
    // create direct sound buffer
    ASSERT( !bADPCM); // ADPCM is not supported on PC (yet:)
    HRESULT hr;
    DSBUFFERDESC dsbDesc;
    LPDIRECTSOUND pDS = _pSound->sl_pDS;
    ASSERT( sd_pdsBuffer==NULL && pDS!=NULL);
    memset( &dsbDesc, 0, sizeof(dsbDesc));
    dsbDesc.dwSize  = sizeof(DSBUFFERDESC);
    dsbDesc.dwFlags = bMono ? DSBCAPS_3D : DSBCAPS_2D;
    dsbDesc.dwBufferBytes = ulDataLength;
    dsbDesc.lpwfxFormat = &wfe;
    // determine which 3D-virtualzation algorithm to use
    dsbDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
    extern INDEX snd_iEmulationType;
    if( bMono && snd_iEmulationType) {
      switch( snd_iEmulationType) {
      case 3:  dsbDesc.guid3DAlgorithm = DS3DALG_HRTF_FULL;          break;
      case 2:  dsbDesc.guid3DAlgorithm = DS3DALG_HRTF_LIGHT;         break;
      case 1:  dsbDesc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;  break;
      default: snd_iEmulationType = 0;
      }
    } // create!
    hr = pDS->CreateSoundBuffer( &dsbDesc, &sd_pdsBuffer, NULL);
    ASSERT( !FAILED(hr) && sd_pdsBuffer!=NULL);

    // fill if with samples
    LPVOID pvAudioPtr1,   pvAudioPtr2;
    DWORD  dwAudioBytes1, dwAudioBytes2;
    hr = sd_pdsBuffer->Lock( 0, 0, &pvAudioPtr1,&dwAudioBytes1, &pvAudioPtr2,&dwAudioBytes2, DSBLOCK_ENTIREBUFFER);
    ASSERT( !FAILED(hr) && pvAudioPtr2==NULL);
    for( INDEX i=0; i<ctStereoSamples; i++) {
      // convert
      ((SWORD*)pvAudioPtr1)[i] = (((ULONG*)pBuffer)[i] >>8) -0x8000;
    }
    // filled
    hr = sd_pdsBuffer->Unlock( pvAudioPtr1,dwAudioBytes1, pvAudioPtr2,dwAudioBytes2);
    ASSERT( !FAILED(hr));
  }

  // convert to memory
  else
  {
    if( !bADPCM) {
      ASSERT( sd_pswSamples==NULL);
      ULONG ulAllocSize = ctStereoSamples*sizeof(SWORD);
      if( ulAllocSize&2) ulAllocSize += 2;  // align to 4
      // on PC, just use heap memory
      sd_pswSamples = (SWORD*)AllocMemory( ulAllocSize+8);
      for( INDEX i=0; i<ctStereoSamples; i++) {
        // convert
        sd_pswSamples[i] = (((ULONG*)pBuffer)[i] >>8) -0x8000;
      }
      // copy first sample to the last one (this is needed for linear interpolation)
      (ULONG&)(sd_pswSamples[ctStereoSamples]) = *(ULONG*)sd_pswSamples;
      // alignment must be at 4 bytes!
      ASSERT( (((SLONG)sd_pswSamples) %4) == 0);
    }
  }

  // eventually free temp buffer
  if( pBuffer!=NULL) FreeMemory(pBuffer);
}
