#include "stdh.h"
#include "initguid.h"

#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Base/Translation.h>

#include <Engine/Base/Shell.h>
#include <Engine/Base/Memory.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Base/Console.h>
#include <Engine/Base/Console_internal.h>
#include <Engine/Base/Statistics_Internal.h>
#include <Engine/Base/IFeel.h>

#include <Engine/Sound/SoundData.h>
#include <Engine/Sound/SoundObject.h>
#include <Engine/Sound/SoundDecoder.h>
#include <Engine/Network/CNetwork.h>

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/StaticStackArray.cpp>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/Stock_CSoundData.h>

#include <Engine/World/WorldSettings.h>

template CStaticArray<CSoundListener>;



	// on PC, we need GUIDs and multi-media library
	#pragma comment(lib, "winmm.lib")
	#include <Engine/Sound/EAX.h>

	// environment types for EAX
	static INDEX aiEnvTypes[] = {
		EAX_ENVIRONMENT_PADDEDCELL,
		EAX_ENVIRONMENT_GENERIC,
		EAX_ENVIRONMENT_LIVINGROOM,
		EAX_ENVIRONMENT_STONEROOM,
		EAX_ENVIRONMENT_AUDITORIUM,
		EAX_ENVIRONMENT_HALLWAY,
		EAX_ENVIRONMENT_ARENA,
		EAX_ENVIRONMENT_STONECORRIDOR,
		EAX_ENVIRONMENT_QUARRY,
		EAX_ENVIRONMENT_MOUNTAINS,
		EAX_ENVIRONMENT_PLAIN,
		EAX_ENVIRONMENT_CAVE,
		EAX_ENVIRONMENT_SEWERPIPE,
		EAX_ENVIRONMENT_HANGAR,
		EAX_ENVIRONMENT_FOREST,
		EAX_ENVIRONMENT_CONCERTHALL,
		EAX_ENVIRONMENT_UNDERWATER,
	};



// pointer to global sound library object
CSoundLibrary *_pSound = NULL;

// copy mixer buffer to the output buffer
extern void CopyMixerBuffer( const SLONG slSrcOffset, const void *pDstBuffer, const SLONG slBytes);


// debug printout
extern CTString _strDebug;
extern PIX _pixDebugStringX, _pixDebugStringY;
extern COLOR _colDebugString;


// console variables
static INDEX snd_iMaxVoices = 20;
static FLOAT snd_tmMixAhead = 0.2f;     // mix-ahead in seconds
extern FLOAT snd_fSoundVolume = 0.9f;   // master volume for sound playing [0..1]
extern FLOAT snd_fMusicVolume = 0.7f;   // master volume for music playing [0..1]
extern INDEX snd_bLinearVolume = TRUE;  // use form of attenuated logaritmic volume
extern INDEX snd_iMaxSameSounds = 5;    // to avoid clipping
extern INDEX snd_bUseHeadphones = FALSE;


ENGINE_API extern INDEX snd_iFormat = 3;
static INDEX snd_iDevice = -1;
static INDEX snd_iInterface = 1;             // 0=WaveOut, 1=DirectSound SW mixer (default), 2=DirectSound3D (HW mixer), 3=DS3D+EAX
static INDEX snd_bUseEnvironmentFX = FALSE;   // EAX, A3D, NV ...
static INDEX snd_iMaxOpenRetries = 3;
static INDEX snd_iMaxExtraChannels = 32;
static FLOAT snd_tmOpenFailDelay = 0.5f;

extern INDEX snd_iEmulationType = 0;
extern INDEX snd_bReportStereoWaves = FALSE;

extern HWND  _hwndMain; // global handle for application window
static HWND  _hwndCurrent = NULL;
static HINSTANCE _hInstDS = NULL;
static INDEX _iWriteOffset = 0;
static INDEX _iLastEnvType = 1234;
static FLOAT _fLastPanning = 1234;
static INDEX _bLastHeadphones = 1234;

// signal to each sound object whether to attenuate and/or mute volume of non-voice and/or all sounds
extern BOOL _bAttenuateVolume = FALSE;
extern BOOL _bMuteVolume = FALSE;


// TEMP! - for writing mixer buffer to file
static FILE *_filMixerBuffer;
static BOOL _bOpened = FALSE;


#define WAVEOUTBLOCKSIZE 1024
#define MINPAN (1.0f)
#define MAXPAN (9.0f)



/**
 * ----------------------------
 *    Sound Library functions
 * ----------------------------
**/

/*
 *  Construct uninitialized sound library.
 */
CSoundLibrary::CSoundLibrary(void)
{
	// clear sound format
	memset( &sl_wfeFormat, 0, sizeof(WAVEFORMATEX));
	sl_sfFormat = SF_NONE;

	// clear data
	_hInstDS = NULL;
	sl_pDS   = NULL;
	sl_pdsbStream = NULL;
	sl_pds3dListener = NULL;
	sl_ulFlags = NONE;
	sl_tmMuteUntil = 0;
	sl_hwoWaveOut = NULL;
	sl_pdsbPrimary = NULL;
	sl_pKSProperty = NULL; 
	sl_pubBuffersMemory = NULL;

	// reset buffer ptrs
	sl_pswMixerBuffer  = NULL;
	sl_pswDecodeBuffer = NULL;
	for( INDEX iPB=0; iPB<SND_MAXBUFFERS; iPB++) {
		sl_appbPlaying[iPB].pb_pso = NULL;
		sl_appbPlaying[iPB].pb_psd = NULL;
		sl_appbPlaying[iPB].pb_pdsb   = NULL;
		sl_appbPlaying[iPB].pb_pdsb3d = NULL;
		sl_appbPlaying[iPB].pb_pksps = NULL;
	}
}


/*
 *  Destruct (and clean up).
 */
CSoundLibrary::~CSoundLibrary(void)
{
	// clear sound enviroment
	Clear();
	// clear any installed sound decoders
	CSoundDecoder::EndPlugins();
}



// post sound console variables' functions

static FLOAT _tmLastMixAhead = 1234;
static INDEX _iLastInterface = 1234;
static INDEX _iLastFormat    = 1234;
static INDEX _iLastDevice    = 1234;
static INDEX _iLastEmulType  = 1234;

static void SndPostFunc(void *pvVar)
{
	// clamp variables
	snd_iFormat = Clamp( snd_iFormat, (INDEX)CSoundLibrary::SF_NONE, (INDEX)CSoundLibrary::SF_44100_16);
	snd_iDevice = Clamp( snd_iDevice, -1L, 15L);
	snd_iInterface = Clamp( snd_iInterface, 0L, 3L);
	snd_tmMixAhead = Clamp( snd_tmMixAhead, 0.1f, 0.4f);
	snd_iEmulationType = Clamp( snd_iEmulationType, 0L, 3L);

	// if any variable has been changed
	if( _tmLastMixAhead!=snd_tmMixAhead || _iLastFormat!=snd_iFormat || _iLastDevice!=snd_iDevice
	 || _iLastInterface!=snd_iInterface || _iLastEmulType!=snd_iEmulationType) {
		// reinit sound format
		_pSound->SetFormat( (enum CSoundLibrary::SoundFormat)snd_iFormat, TRUE);
	}
}



/*
 *  some internal functions
 */


// DirectSound shutdown procedure
void CSoundLibrary::ShutDown_dsound(void)
{
	// free direct sound buffer(s)
	sl_ulFlags &= ~(SLF_USINGDS3D|SLF_HASEFX);

	if( sl_pds3dListener != NULL) {
		sl_pds3dListener->Release();
		sl_pds3dListener = NULL;
	}

	if( sl_pdsbStream != NULL) {
		sl_pdsbStream->Stop();
		sl_pdsbStream->Release();
		sl_pdsbStream = NULL;
	}

	if( sl_pdsbPrimary!=NULL) {
		sl_pdsbPrimary->Stop();
		sl_pdsbPrimary->Release();
		sl_pdsbPrimary = NULL;
	}
	if( sl_pKSProperty != NULL) {
		sl_pKSProperty->Release();
		sl_pKSProperty = NULL;
	}

	// free direct sound object
	if( sl_pDS!=NULL) {
		// reset cooperative level
		if( _hwndCurrent!=NULL) sl_pDS->SetCooperativeLevel( _hwndCurrent, DSSCL_NORMAL);
		sl_pDS->Release();
		sl_pDS = NULL;
	}
	// free direct sound library
	if( _hInstDS != NULL) {
		FreeLibrary(_hInstDS);
		_hInstDS = NULL;
	}

	// free memory
	if( sl_pswMixerBuffer!=NULL) {
		FreeMemory( sl_pswMixerBuffer);
		sl_pswMixerBuffer = NULL;
	}
	if( sl_pswDecodeBuffer!=NULL) {
		FreeMemory( sl_pswDecodeBuffer);
		sl_pswDecodeBuffer = NULL;
	}
}


// WaveOut shutdown procedure
void CSoundLibrary::ShutDown_waveout(void)
{
	// shut down wave out player buffers (if needed)
	if( sl_hwoWaveOut!=NULL) { 
		MMRESULT res;
		res = waveOutReset(sl_hwoWaveOut);
		ASSERT(res == MMSYSERR_NOERROR);
		 // clear buffers
		for( INDEX iBuffer = 0; iBuffer<sl_awhWOBuffers.Count(); iBuffer++) {
			res = waveOutUnprepareHeader( sl_hwoWaveOut, &sl_awhWOBuffers[iBuffer],
																		sizeof(sl_awhWOBuffers[iBuffer]));
			ASSERT(res == MMSYSERR_NOERROR);
		}
		sl_awhWOBuffers.Clear();
		// close waveout device
		res = waveOutClose( sl_hwoWaveOut);
		ASSERT(res == MMSYSERR_NOERROR);
		sl_hwoWaveOut = NULL;
	}

	// close extra taken channels
	for( INDEX iChannel=0; iChannel<sl_ahwoExtra.Count(); iChannel++) {
		MMRESULT res = waveOutClose( sl_ahwoExtra[iChannel]);
		ASSERT(res == MMSYSERR_NOERROR);
	}
	// free extra channel handles
	sl_ahwoExtra.PopAll();

	// free wave-out buffers memory
	if( sl_pubBuffersMemory!=NULL) {
		FreeMemory( sl_pubBuffersMemory);
		sl_pubBuffersMemory = NULL;
	}
}



/*
 *  Set wave format from library format
 */
static void SetWaveFormat( CSoundLibrary::SoundFormat esfFormat, WAVEFORMATEX &wfeFormat)
{
	// change Library Wave Format
	memset( &wfeFormat, 0, sizeof(WAVEFORMATEX));
	wfeFormat.wFormatTag = WAVE_FORMAT_PCM;
	wfeFormat.nChannels = 2;
	wfeFormat.wBitsPerSample = 16;
	switch( esfFormat) {
		case CSoundLibrary::SF_11025_16: wfeFormat.nSamplesPerSec = 11025;  break;
		case CSoundLibrary::SF_22050_16: wfeFormat.nSamplesPerSec = 22050;  break;
		case CSoundLibrary::SF_44100_16: wfeFormat.nSamplesPerSec = 44100;  break;
		case CSoundLibrary::SF_NONE: ASSERTALWAYS( "Can't set to NONE format"); break;
		default:                     ASSERTALWAYS( "Unknown Sound format");     break;
	}
	wfeFormat.nBlockAlign     = wfeFormat.nChannels * wfeFormat.wBitsPerSample/8;
	wfeFormat.nAvgBytesPerSec = wfeFormat.nSamplesPerSec * wfeFormat.nBlockAlign;
}


/*
 *  Set library format from wave format
 */
static void SetLibraryFormat( CSoundLibrary &sl)
{
	// if library format is none return
	if( sl.sl_sfFormat == CSoundLibrary::SF_NONE) return;

	// else check wave format to determine library format
	ULONG ulFormat = sl.sl_wfeFormat.nSamplesPerSec;
	// find format
	switch( ulFormat) {
		case 11025: sl.sl_sfFormat = CSoundLibrary::SF_11025_16; break;
		case 22050: sl.sl_sfFormat = CSoundLibrary::SF_22050_16; break;
		case 44100: sl.sl_sfFormat = CSoundLibrary::SF_44100_16; break;
		// unknown format
		default:
			ASSERTALWAYS( "Unknown sound format");
			FatalError( TRANS("Unknown sound format"));
			sl.sl_sfFormat = CSoundLibrary::SF_ILLEGAL;
	}
}


// common fail for direct-sound
BOOL CSoundLibrary::DSFail( char *strError) 
{
	CPrintF(strError);
	ShutDown_dsound();
	snd_iInterface = 0;   // if DirectSound has failed -> try WaveOut
	return FALSE;
}


// helper for locking buffer
BOOL CSoundLibrary::DSLockBuffer( LPDIRECTSOUNDBUFFER pBuffer, SLONG slSize, LPVOID &lpData, DWORD &dwSize)
{
	INDEX ctRetries = 1000;  // too much?
	FOREVER {
		HRESULT hr = pBuffer->Lock( 0, slSize, &lpData, &dwSize, NULL, NULL, 0);
		if( hr==DS_OK && slSize==dwSize) return TRUE;
		if( hr!=DSERR_BUFFERLOST) return DSFail( TRANS("  ! DirectSound error: Cannot lock sound buffer.\n"));
		if( ctRetries-- == 0) return DSFail( TRANS("  ! DirectSound error: Couldn't restore sound buffer.\n"));
		pBuffer->Restore();
	}
}
 

void CSoundLibrary::DSPlayMainBuffers(void)
{
	HRESULT hr;
	DWORD dwStatus;
	BOOL bInitiatePlay = FALSE;

	// check if buffers are already playing
	ASSERT( sl_pdsbStream!=NULL);
	hr = sl_pdsbStream->GetStatus(&dwStatus);
	ASSERT( hr==DS_OK);
	if( !(dwStatus&DSBSTATUS_PLAYING)) bInitiatePlay = TRUE;
	ASSERT( sl_pdsbPrimary!=NULL);
	hr = sl_pdsbPrimary->GetStatus(&dwStatus);
	ASSERT( hr==DS_OK);
	if( !(dwStatus&DSBSTATUS_PLAYING)) bInitiatePlay = TRUE;
	// done if both buffers are already playing
	if( !bInitiatePlay) return;

	// stop buffers (in case some buffers are playing)
	hr = sl_pdsbStream->Stop();
	ASSERT( hr==DS_OK);
	do sl_pdsbStream->GetStatus(&dwStatus);
	while( dwStatus&DSBSTATUS_PLAYING);
	hr = sl_pdsbPrimary->Stop();
	ASSERT( hr==DS_OK);
	do sl_pdsbPrimary->GetStatus(&dwStatus);
	while( dwStatus&DSBSTATUS_PLAYING);
	
	// check sound buffer lock and clear sound buffer(s) 
	LPVOID lpData;
	DWORD	 dwSize;
	if( !DSLockBuffer( sl_pdsbStream, sl_slMixerBufferSize, lpData, dwSize)) return;
	memset( lpData, 0, dwSize);
	sl_pdsbStream->Unlock( lpData, dwSize, NULL, 0);

	// start playing standard DirectSound buffers
	hr = sl_pdsbPrimary->Play( 0, 0, DSBPLAY_LOOPING);
	ASSERT(hr==DS_OK);
	hr = sl_pdsbStream->Play( 0, 0, DSBPLAY_LOOPING);
	ASSERT(hr==DS_OK);
	_iWriteOffset = 0;
}


// init and set DirectSound format (internal)

BOOL CSoundLibrary::StartUp_dsound( BOOL bReport/*=TRUE*/)
{
	// startup
	sl_ulFlags = NONE;
	ASSERT( _hInstDS==NULL && sl_pDS==NULL);
	ASSERT( sl_pdsbStream==NULL);
	// update window handle (just in case)
	HRESULT (WINAPI *pDirectSoundCreate)(GUID FAR *lpGUID, LPDIRECTSOUND FAR *lplpDS, IUnknown FAR *pUnkOuter);
	
	// begin ...
	if( bReport) CPrintF(TRANS("Direct Sound initialization ...\n"));

	// PC has dynamic library
	ASSERT( _hInstDS==NULL);
	_hInstDS = LoadLibrary( "dsound.dll");
	if( _hInstDS==NULL) {
		CPrintF( TRANS("  ! DirectSound error: Cannot load 'DSOUND.DLL'.\n"));
		return FALSE;
	}
	// get main procedure address
	pDirectSoundCreate = (HRESULT(WINAPI *)(GUID FAR *, LPDIRECTSOUND FAR *, IUnknown FAR *))GetProcAddress( _hInstDS, "DirectSoundCreate");
	if( pDirectSoundCreate==NULL) return DSFail( TRANS("  ! DirectSound error: Cannot get procedure address.\n"));

	// init dsound
	HRESULT	hr;
	DSCAPS dsCaps;
	DSBUFFERDESC dsbDesc;
	WAVEFORMATEX &wfe = sl_wfeFormat;
	if( sl_pDS==NULL) {
		hr = pDirectSoundCreate( NULL, &sl_pDS, NULL);
		if( hr != DS_OK) return DSFail( TRANS("  ! DirectSound error: Cannot create object.\n"));
	}


	// get capabilities
	dsCaps.dwSize = sizeof(dsCaps);
	hr = sl_pDS->GetCaps(&dsCaps);
	if( hr!=DS_OK) return DSFail( TRANS("  ! DirectSound error: Cannot determine capabilities.\n"));
	// fail if in emulation mode
	if( dsCaps.dwFlags & DSCAPS_EMULDRIVER) {
		CPrintF( TRANS("  ! DirectSound error: No driver installed.\n"));
		ShutDown_dsound();
		return FALSE;
	} 
	// readout number of available HW buffers
	const INDEX ct2DVoicesHW = dsCaps.dwFreeHwMixingStaticBuffers;
	const INDEX ct3DVoicesHW = dsCaps.dwFreeHw3DStaticBuffers;

	// set cooperative level to priority
	_hwndCurrent = _hwndMain;
	hr = sl_pDS->SetCooperativeLevel( _hwndCurrent, DSSCL_PRIORITY);
	if( hr!=DS_OK) return DSFail( TRANS("  ! DirectSound error: Cannot set cooperative level.\n"));

	// create primary sound buffer
	memset( &dsbDesc, 0, sizeof(dsbDesc));
	dsbDesc.dwSize  = sizeof(dsbDesc);
	dsbDesc.dwBufferBytes = 0;
	dsbDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;
	dsbDesc.lpwfxFormat = NULL;
	hr = sl_pDS->CreateSoundBuffer( &dsbDesc, &sl_pdsbPrimary, NULL);
	if( hr!=DS_OK) return DSFail( TRANS("  ! DirectSound error: Cannot create primary sound buffer.\n"));
	// set primary buffer format
	hr  = sl_pdsbPrimary->SetFormat(&wfe);
	if( hr!=DS_OK) return DSFail( TRANS("  ! DirectSound error: Cannot set primary sound buffer format.\n"));
	// obtain listener
	hr = sl_pdsbPrimary->QueryInterface( IID_IDirectSound3DListener, (LPVOID*)&sl_pds3dListener);
	if( hr!=DS_OK) return DSFail( TRANS("  ! DirectSound3D error: Cannot obtain 3D listener.\n"));


	// check whether we can use DirectSound3D afterall
	snd_iEmulationType = Clamp( snd_iEmulationType, 0L, 3L);
	if( !(sl_ulFlags&SLF_USINGDS3D) && snd_iInterface>=2) {
		// allowed?
		if( ct3DVoicesHW>=SND_MINBUFFERS || snd_iEmulationType>0) {
			// good luck to driver
			sl_ulFlags |= SLF_USINGDS3D;
		} // disallowed?
		else {
			// just notify cvar
			snd_iInterface = 1;
		} 
	}

	// setup listener params as persistent
	const BOOL bUsingDS3D = sl_ulFlags & SLF_USINGDS3D;
	if( bUsingDS3D) {
		ASSERT( sl_pds3dListener!=NULL);
		hr = sl_pds3dListener->SetOrientation( 0,0,1, 0,1,0, DS3D_IMMEDIATE);  ASSERT( hr==DS_OK);
		hr = sl_pds3dListener->SetPosition( 0,0,0, DS3D_IMMEDIATE);  ASSERT( hr==DS_OK);
		hr = sl_pds3dListener->SetVelocity( 0,0,0, DS3D_IMMEDIATE);  ASSERT( hr==DS_OK);
		hr = sl_pds3dListener->SetRolloffFactor( 0.0f, DS3D_IMMEDIATE);  ASSERT( hr==DS_OK); // distance attenuation thru SetVolume()
		hr = sl_pds3dListener->SetDopplerFactor( 1.1f, DS3D_IMMEDIATE);  ASSERT( hr==DS_OK);
	}

	// determine size of buffers for streaming purposes
	ASSERT( wfe.wFormatTag==WAVE_FORMAT_PCM && wfe.wBitsPerSample==16 && wfe.nChannels==2);
	const SLONG slBufferSize = (SLONG)(ceil(snd_tmMixAhead*wfe.nSamplesPerSec) *16/8 *2);

	// create sound buffer for "streaming" (streaming is infact emulated in engine)
	memset( &dsbDesc, 0, sizeof(dsbDesc));
	dsbDesc.dwSize = sizeof(dsbDesc);
	dsbDesc.dwBufferBytes = slBufferSize;
	dsbDesc.dwFlags = DSBCAPS_2D | DSBCAPS_GETPOS;
	dsbDesc.lpwfxFormat = &wfe;
	hr = sl_pDS->CreateSoundBuffer( &dsbDesc, &sl_pdsbStream, NULL);
	if( hr!=DS_OK) return DSFail( TRANS("  ! DirectSound error: Cannot create streaming buffer.\n"));

	// query property interface to EAX if required
	if( snd_iInterface==3) {
		// create one dummy 3d sound buffer
		LPDIRECTSOUNDBUFFER pdsb;
		wfe.nChannels = 1;
		wfe.nBlockAlign /= 2;
		wfe.nAvgBytesPerSec /= 2;
		dsbDesc.dwSize = sizeof(dsbDesc);
		dsbDesc.dwBufferBytes = DSBSIZE_MIN;
		dsbDesc.dwFlags = DSBCAPS_CTRL3D;
		dsbDesc.lpwfxFormat = &wfe;
		hr = sl_pDS->CreateSoundBuffer( &dsbDesc, &pdsb, NULL);
		if( hr==DS_OK) {
			LPDIRECTSOUND3DBUFFER pdsb3d;
			hr = pdsb->QueryInterface( IID_IDirectSound3DBuffer8, (LPVOID*)&pdsb3d);
			if( hr==DS_OK) {
				CTString strErrEAX = TRANS("  ! EAX error: Cannot query property support.\n");
				hr = pdsb3d->QueryInterface( IID_IKsPropertySet, (LPVOID*)&sl_pKSProperty);
				if( hr==DS_OK) {
					// query support
					ULONG ulSup=0;
					hr = sl_pKSProperty->QuerySupport( DSPROPSETID_EAX_ListenerProperties, DSPROPERTY_EAXLISTENER_ENVIRONMENT, &ulSup);
					if( hr==DS_OK && (ulSup&KSPROPERTY_SUPPORT_SET)) {
						hr = sl_pKSProperty->QuerySupport( DSPROPSETID_EAX_BufferProperties, DSPROPERTY_EAXBUFFER_DIRECTHF, &ulSup);
						if( hr==DS_OK && (ulSup&KSPROPERTY_SUPPORT_SET)) {
							// made it - signal that EAX usage is on! :)
							sl_ulFlags |= SLF_HASEFX; 
						}
					}
				} // report eventual error and pull out
				if( !(sl_ulFlags&SLF_HASEFX)) CPrintF( strErrEAX);
				pdsb3d->Release();
			} // done with EAX init
			pdsb->Release();
		}
	}

	// mixer buffer size (decoder buffer always works at 44khz)
	sl_slMixerBufferSize  = slBufferSize;
	sl_slDecodeBufferSize = slBufferSize *44100.0f/wfe.nSamplesPerSec +32;
	// allocate mixing and decoding buffers
	sl_pswMixerBuffer  = (SWORD*)AllocMemory( sl_slMixerBufferSize);
	sl_pswDecodeBuffer = (SWORD*)AllocMemory( sl_slDecodeBufferSize+4); // (+4 because of linear interpolation of last samples)
	_iWriteOffset = 0;

	// report success
	if( bReport) {
		CTString strDevice = TRANS("default device");
		if( snd_iDevice>=0) strDevice.PrintF( TRANS("device %d"), snd_iDevice); 
		CPrintF( TRANS("  %dHz, %dbit, %s, mix-ahead: %gs\n"), wfe.nSamplesPerSec, wfe.wBitsPerSample, strDevice, snd_tmMixAhead); 
		CPrintF( TRANS("  hardware voices: %d (%d 3D)\n"), ct2DVoicesHW, ct3DVoicesHW);
		CPrintF( TRANS("  mixer buffer size:  %d KB\n"), sl_slMixerBufferSize /1024);
		CPrintF( TRANS("  decode buffer size: %d KB\n"), sl_slDecodeBufferSize/1024);
		// EAX?
		CTString strEAX;
		strEAX = (sl_ulFlags&SLF_HASEFX) ? TRANS("Enabled") : TRANS("Disabled");
		CPrintF( TRANS("  environment effects: %s\n"), strEAX);
	} 
	// done
	return TRUE;
}



// set WaveOut format (internal)

BOOL CSoundLibrary::StartUp_waveout( BOOL bReport/*=TRUE*/)
{


	// not using DirectSound (obviously)
	sl_ulFlags = SLF_USINGWAVEOUT;
	WAVEFORMATEX &wfe = sl_wfeFormat;  // shortcut
	if( bReport) CPrintF(TRANS("WaveOut initialization ...\n"));

	// set maximum total number of retries for device opening
	INDEX ctMaxRetries = snd_iMaxOpenRetries;
	INDEX ctChannelsOpened = 0;
	MMRESULT res;

	// repeat
	FOREVER {
		// try to open wave device
		HWAVEOUT hwo;
		res = waveOutOpen( &hwo, (snd_iDevice<0)?WAVE_MAPPER:snd_iDevice, &sl_wfeFormat, NULL, NULL, NONE);
		// if opened
		if( res == MMSYSERR_NOERROR) {
			ctChannelsOpened++;
			// remember first one as used waveout 
			if( ctChannelsOpened==1) sl_hwoWaveOut = hwo;
			// else remember under extra channel
			else sl_ahwoExtra.Push() = hwo;
			// done if no extra channels should be taken
			if( ctChannelsOpened >= snd_iMaxExtraChannels+1) break;
		// if cannot open
		} else {
			// decrement retry counter
			ctMaxRetries--;
			// quit if no more retries
			if( ctMaxRetries<0) break;
			// if more retries left wait a bit (probably sound-scheme is playing)
			else Sleep( int(snd_tmOpenFailDelay*1000));
		}
	}

	// report error if couldn't set format
	if( ctChannelsOpened==0 && res!=MMSYSERR_NOERROR) {
		CTString strError;
		switch (res) {
		case MMSYSERR_ALLOCATED:    strError = TRANS("Device already in use.");     break;
		case MMSYSERR_BADDEVICEID:  strError = TRANS("Bad device number.");         break;
		case MMSYSERR_NODRIVER:     strError = TRANS("No driver installed.");       break;
		case MMSYSERR_NOMEM:        strError = TRANS("Memory allocation problem."); break;
		case WAVERR_BADFORMAT:      strError = TRANS("Unsupported data format.");   break;
		case WAVERR_SYNC:           strError = TRANS("Wrong flag?");                break;
		default: strError.PrintF( "%d", res);
		};
		CPrintF( TRANS("  ! WaveOut error: %s\n"), strError);
		return FALSE;
	}

	// report success
	if( bReport) {
		// get waveout capabilities
		WAVEOUTCAPS woc;
		memset( &woc, 0, sizeof(woc));
		res = waveOutGetDevCaps((int)sl_hwoWaveOut, &woc, sizeof(woc));
		CTString strDevice = TRANS("default device");
		if( snd_iDevice>=0) strDevice.PrintF( TRANS("device %d"), snd_iDevice); 
		CPrintF( TRANS("  opened device: %s\n"), woc.szPname);
		CPrintF( TRANS("  %dHz, %dbit, %s\n"), wfe.nSamplesPerSec, wfe.wBitsPerSample, strDevice);
	}

	// determine whole mixer buffer size from mixahead console variable
	ASSERT( wfe.wFormatTag==WAVE_FORMAT_PCM && wfe.wBitsPerSample==16 && wfe.nChannels==2);
	sl_slMixerBufferSize = (SLONG)(ceil(snd_tmMixAhead*sl_wfeFormat.nSamplesPerSec) *16/8 *2);
	// align size to be next multiply of WAVEOUTBLOCKSIZE
	sl_slMixerBufferSize += WAVEOUTBLOCKSIZE - (sl_slMixerBufferSize % WAVEOUTBLOCKSIZE);
	// determine number of WaveOut buffers
	const INDEX ctWOBuffers = sl_slMixerBufferSize / WAVEOUTBLOCKSIZE;
	ASSERT( ctWOBuffers*WAVEOUTBLOCKSIZE == sl_slMixerBufferSize);
	// decoder buffer always works at 44khz
	sl_slDecodeBufferSize = sl_slMixerBufferSize * 44100.0f/wfe.nSamplesPerSec +32;

	// bla, bla ...
	if( bReport) {
		CPrintF(TRANS("  parameters: %d Hz, %d bit, stereo, mix-ahead: %gs\n"),
						sl_wfeFormat.nSamplesPerSec, sl_wfeFormat.wBitsPerSample, snd_tmMixAhead);
		CPrintF(TRANS("  output buffers: %d x %d bytes\n"), ctWOBuffers, WAVEOUTBLOCKSIZE),
		CPrintF(TRANS("  mpx decode: %d bytes\n"), sl_slDecodeBufferSize),
		CPrintF(TRANS("  extra sound channels taken: %d\n"), ctChannelsOpened-1);
	}

	// initialise waveout sound buffers
	sl_pubBuffersMemory = (UBYTE*)AllocMemory( sl_slMixerBufferSize);
	memset( sl_pubBuffersMemory, 0, sl_slMixerBufferSize);
	sl_awhWOBuffers.New(ctWOBuffers); 
	for( INDEX iBuffer = 0; iBuffer<ctWOBuffers; iBuffer++) {
		WAVEHDR &wh = sl_awhWOBuffers[iBuffer];
		wh.lpData = (char*)(sl_pubBuffersMemory + iBuffer*WAVEOUTBLOCKSIZE);
		wh.dwBufferLength = WAVEOUTBLOCKSIZE;
		wh.dwFlags = 0;
	}

	// initialize mixing and decoding buffer
	sl_pswMixerBuffer  = (SWORD*)AllocMemory( sl_slMixerBufferSize);
	sl_pswDecodeBuffer = (SWORD*)AllocMemory( sl_slDecodeBufferSize+4); // (+4 because of linear interpolation of last samples)
	return TRUE;
}




/*
 *  set sound format
 */
void CSoundLibrary::SetFormat_internal( CSoundLibrary::SoundFormat sfNew, BOOL bReport)
{
	// remember library format
	sl_sfFormat = sfNew;
	// release library
	ClearLibrary();

	// if none skip initialization
	if( bReport) CPrintF(TRANS("Setting sound format ...\n"));
	if( sl_sfFormat == CSoundLibrary::SF_NONE) {
		if( bReport) CPrintF(TRANS("  (no sound)\n"));
		return;
	}

	// set wave format from library format
	SetWaveFormat( sfNew, sl_wfeFormat);
	BOOL bStarted = FALSE;

	snd_iDevice    = Clamp( snd_iDevice, -1L, (INDEX)(sl_ctWaveDevices-1));
	snd_tmMixAhead = Clamp( snd_tmMixAhead, 0.1f, 0.4f);
	snd_iInterface = Clamp( snd_iInterface, 0L, 3L);

	if( snd_iInterface>0) {
		// if wanted, 1st try DirectSound
		bStarted = StartUp_dsound(bReport);  
	}
	// if DirectSound failed (or not wanted)
	if( !bStarted) { 
		// try waveout
		bStarted = StartUp_waveout(bReport); 
		if( bStarted) snd_iInterface = 0; // mark that DirectSound didn't make it
	}

	// if didn't make it by now
	if( bReport) CPrintF("\n");
	if( !bStarted) {
		// revert to none in case sound init was unsuccessful
		sl_sfFormat = CSoundLibrary::SF_NONE;
		return;
	} 

	// set library format from wave format
	SetLibraryFormat(*this);
}


/*
 *  Initialization
 */
void CSoundLibrary::Init(void)
{
	_pShell->DeclareSymbol( "void SndPostFunc(INDEX);", &SndPostFunc);

//안태훈 수정 시작	//(Open beta)(2004-12-30)
	extern FLOAT g_fMscVol;
	extern FLOAT g_fSndVol;
	_pShell->DeclareSymbol( "persistent user FLOAT snd_fSoundVolume;",  &g_fSndVol);
	_pShell->DeclareSymbol( "persistent user FLOAT snd_fMusicVolume;",  &g_fMscVol);
//안태훈 수정 끝	//(Open beta)(2004-12-30)
	_pShell->DeclareSymbol( "persistent user INDEX snd_bLinearVolume;", &snd_bLinearVolume);

	_pShell->DeclareSymbol( "persistent user FLOAT snd_tmMixAhead post:SndPostFunc;", &snd_tmMixAhead);
	_pShell->DeclareSymbol( "persistent user INDEX snd_iInterface post:SndPostFunc;", &snd_iInterface);
	_pShell->DeclareSymbol( "persistent user INDEX snd_iDevice post:SndPostFunc;", &snd_iDevice);
	_pShell->DeclareSymbol( "persistent user INDEX snd_iFormat post:SndPostFunc;", &snd_iFormat);
	_pShell->DeclareSymbol( "persistent user INDEX snd_iMaxExtraChannels;", &snd_iMaxExtraChannels);
	_pShell->DeclareSymbol( "persistent user FLOAT snd_tmOpenFailDelay;",   &snd_tmOpenFailDelay);

	_pShell->DeclareSymbol( "persistent user INDEX snd_iMaxVoices;", &snd_iMaxVoices);
	_pShell->DeclareSymbol( "persistent user INDEX snd_iMaxSameSounds;", &snd_iMaxSameSounds);
	_pShell->DeclareSymbol( "persistent user INDEX snd_bUseHeadphones;", &snd_bUseHeadphones);
	//_pShell->DeclareSymbol( "persistent user INDEX snd_bUseEnvironmentFX;", &snd_bUseEnvironmentFX);

	// for debugging purposes only
	_pShell->DeclareSymbol( "user INDEX snd_bReportStereoWaves;", &snd_bReportStereoWaves);

	// PC only snd cvars
	_pShell->DeclareSymbol( "persistent user INDEX snd_iEmulationType post:SndPostFunc;", &snd_iEmulationType);

	// print header
	CPrintF(TRANS("Initializing sound...\n"));

	// initialize sound library
	SetFormat(SF_NONE, FALSE);     

	// initialize any installed sound decoders
	CSoundDecoder::InitPlugins();

	// get number of devices
	INDEX ctDevices = waveOutGetNumDevs();
	CPrintF(TRANS("  Detected devices: %d\n"), ctDevices);
	sl_ctWaveDevices = ctDevices;

	// <-- ErrorLog.txt에 디스플레이 정보를 기록하기 위한 부분
	extern CTString	_strSoundDriver;
	// -->
	
	// for each device
	for(INDEX iDevice=0; iDevice<ctDevices; iDevice++) {
		// get description
		WAVEOUTCAPS woc;
		memset( &woc, 0, sizeof(woc));
		MMRESULT res = waveOutGetDevCaps(iDevice, &woc, sizeof(woc));
		CPrintF(TRANS("    device %d: %s\n"), 
			iDevice, woc.szPname);
		// <-- ErrorLog.txt에 디스플레이 정보를 기록하기 위한 부분
		_strSoundDriver = woc.szPname;
		// -->
		CPrintF(TRANS("      ver: %d, id: %d.%d\n"), 
			woc.vDriverVersion, woc.wMid, woc.wPid);
		CPrintF(TRANS("      form: 0x%08x, ch: %d, support: 0x%08x\n"), 
			woc.dwFormats, woc.wChannels, woc.dwSupport);
	}
	// done
	CPrintF("\n");
}



// stop and/or release sound buffer
static void StopReleaseSoundBuffer( PlayingBuffer &pb, const BOOL bForceRelease)
{
	// skip already released
	if( pb.pb_pdsb==NULL) {
		ASSERT( pb.pb_pdsb3d==NULL);
		return;
	}
	// do it
	DWORD dwStatus;
	HRESULT hr = pb.pb_pdsb->GetStatus(&dwStatus);
	ASSERT( hr==DS_OK);
	const BOOL bPlaying = (dwStatus & DSBSTATUS_PLAYING);
	if( bPlaying) pb.pb_pdsb->Stop();
	if(!bPlaying || bForceRelease) {
		pb.pb_pdsb->Release();
		if( pb.pb_pdsb3d!=NULL) pb.pb_pdsb3d->Release();
		if( pb.pb_pksps !=NULL) pb.pb_pksps->Release();
		pb.pb_pksps = NULL;
		pb.pb_pdsb   = NULL;
		pb.pb_pdsb3d = NULL;
	}
	// empty!
	pb.pb_pso = NULL;
	pb.pb_psd = NULL;
}


/*
 *  Clear Sound Library
 */
void CSoundLibrary::Clear(void)
{                        
	// stop all playing and pending sounds
	Flush();
	ClearLibrary();
}


// clear library
void CSoundLibrary::ClearLibrary(void)
{
	// release playing buffers
	for( INDEX iPB=0; iPB<SND_MAXBUFFERS; iPB++) {
		// skip unused or already freed
		PlayingBuffer &pb = sl_appbPlaying[iPB];
		ASSERT( pb.pb_pso==NULL && pb.pb_psd==NULL);
		if( pb.pb_pdsb==NULL) ReleasePlayingBuffer( iPB, TRUE);
	}

	// shut down direct sound buffers (if needed)
	sl_tmMuteUntil = 0;
	ShutDown_dsound();
	ShutDown_waveout();

	// free mixer and decoder memory buffers
	if( sl_pswMixerBuffer!=NULL) {
		FreeMemory( sl_pswMixerBuffer);
		sl_pswMixerBuffer = NULL;
	}
	if( sl_pswDecodeBuffer!=NULL) {
		FreeMemory( sl_pswDecodeBuffer);
		sl_pswDecodeBuffer = NULL;
	}
}


// set listener enviroment properties (EAX)
void CSoundLibrary::SetEnvironment( enum SndEnvFX eEnvFX)
{
	// cast env type
	INDEX iEnvNo = (INDEX)eEnvFX;
	ASSERT( iEnvNo>=0 || iEnvNo<=16);
	if( iEnvNo<0 || iEnvNo>16) iEnvNo = 1;  // clamp just in case

	// force default environment if disabled and skip if same as last
	if( !snd_bUseEnvironmentFX) iEnvNo = SEFX_NORMAL;
	if( _iLastEnvType==iEnvNo) return;
	_iLastEnvType = iEnvNo;  // keep new property

	// set new environment
	if( !(sl_ulFlags&SLF_HASEFX)) return;
	INDEX iEnvEAX = aiEnvTypes[iEnvNo];
	HRESULT hr = sl_pKSProperty->Set( DSPROPSETID_EAX_ListenerProperties,
		DSPROPERTY_EAXLISTENER_ENVIRONMENT|DSPROPERTY_EAXLISTENER_DEFERRED, NULL, 0, &iEnvEAX, sizeof(DWORD));
	ASSERT( hr==DS_OK);
}


// mute all sounds (erase playing buffer(s) and supress mixer)
void CSoundLibrary::Mute( const TIME tmSeconds/*=0*/)
{
	// stop all IFeel effects
	IFeel_StopEffect(NULL);
	if( this==NULL) return;  // skip if there's no more sound library

	// set mute timer
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
	sl_tmMuteUntil = tvNow.GetSeconds() + tmSeconds;
	if( sl_ulFlags & SLF_USINGWAVEOUT) return;   // waveout will shut-up by itself

	// release all HW playing buffers 
	if( sl_ulFlags & SLF_USINGDS3D) {
		FOREACHINLIST( CSoundObject, so_Node, sl_lhPendingSounds, itso) {
			CSoundObject &so = *itso;
			if( so.so_iPlayingBuffer<0) continue;
			ReleasePlayingBuffer( so.so_iPlayingBuffer, TRUE);
			so.so_iPlayingBuffer = -1;
		}
	}

	// flush streaming buffer if exist
	if( sl_pdsbStream!=NULL) {
		LPVOID lpData;
		DWORD  dwSize;
		const BOOL bLocked = DSLockBuffer( sl_pdsbStream, sl_slMixerBufferSize, lpData, dwSize);
		if( bLocked) {
			memset( lpData, 0, dwSize);
			sl_pdsbStream->Unlock( lpData, dwSize, NULL, 0);
		}
	}
}


// stop all playing and pending sounds
void CSoundLibrary::Flush(void)
{
	// stop all IFeel effects
	IFeel_StopEffect(NULL);
	// stop direct sound buffers, but skip if there's no more sound library (waveout will shut-up by itself)
	sl_tmMuteUntil = 0;
	if( this==NULL || (sl_ulFlags&SLF_USINGWAVEOUT)) return;
	DWORD dwStatus;
	HRESULT hr;

	// stop sounds
	{FORDELETELIST( CSoundObject, so_Node, sl_lhPendingSounds, itso) {
		itso->Stop();
	}}
	// stop 'streaming' buffer(s)
	if( sl_pdsbStream!=NULL) {
		hr = sl_pdsbStream->Stop();
		ASSERT( hr==DS_OK);
		do sl_pdsbStream->GetStatus(&dwStatus);
		while( dwStatus&DSBSTATUS_PLAYING);
	}
	// on PC, stop primary buffer also
	if( sl_pdsbPrimary!=NULL) {
		hr = sl_pdsbPrimary->Stop();
		ASSERT( hr==DS_OK);
		do sl_pdsbPrimary->GetStatus(&dwStatus);
		while( dwStatus&DSBSTATUS_PLAYING);
	}
}


// set sound format
void CSoundLibrary::SetFormat( CSoundLibrary::SoundFormat sfNew, BOOL bReport/*=FALSE*/)
{
	// stop playing all pending sounds
	{FOREACHINLIST( CSoundObject, so_Node, sl_lhPendingSounds, itso) {
		itso->Pause();
	}}

	// unbind sounds
	if( _pSoundStock!=NULL) {
		FOREACHINDYNAMICCONTAINER( _pSoundStock->st_ctObjects, CSoundData, itsd) {
			if( !(itsd->sd_ulFlags&SDF_ENCODED)) itsd->ClearBuffer();
		}
	}
	/*
	extern BOOL	_bNTKernel;
	if( _bNTKernel )
	{
		snd_tmMixAhead = 0.2f;
		snd_iInterface = 2;
	}
	else
	{
		snd_tmMixAhead = 0.4f;
		snd_iInterface = 0;
	}
	*/
	//snd_tmMixAhead = 0.2f;
	//snd_iInterface = 1;

	// change format and keep console variable states
	SetFormat_internal( sfNew, bReport);
	_tmLastMixAhead = snd_tmMixAhead;
	_iLastInterface = snd_iInterface;
	_iLastFormat    = snd_iFormat;
	_iLastDevice    = snd_iDevice;
	_iLastEmulType  = snd_iEmulationType;

	// reload all non-streaming sounds 
	{FOREACHINDYNAMICCONTAINER( _pSoundStock->st_ctObjects, CSoundData, itsd) {
		if( !(itsd->sd_ulFlags&SDF_ENCODED)) itsd->Reload();
	}}

	// continue playing
	{FOREACHINLIST( CSoundObject, so_Node, sl_lhPendingSounds, itso) {
		itso->Resume();
	}}
}



// for sorting - compare two sound objects by current volume and distance from listener
int qsort_CompareSoundObjects( const void *ppso1, const void *ppso2)
{
	const CSoundObject &so1 = **(CSoundObject**)ppso1;
	const CSoundObject &so2 = **(CSoundObject**)ppso2;
	FLOAT fDelta;

	// if volumes are different
	if( so1.so_fCurrentVolume != so2.so_fCurrentVolume) {
		// sort by volumes
		fDelta = so2.so_fCurrentVolume - so1.so_fCurrentVolume;
	}
	// if volumes are the same
	else {
		// sort by distance
		fDelta = so1.so_fCurrentDistance - so2.so_fCurrentDistance;
	}
	// return sign
	return (SLONG&)fDelta;

	/*   if( so1.so_fCurrentVolume > so2.so_fCurrentVolume) return -1;
	else if( so1.so_fCurrentVolume < so2.so_fCurrentVolume) return +1;
	else if( so1.so_fCurrentDistance < so2.so_fCurrentDistance) return -1;
	else if( so1.so_fCurrentDistance > so2.so_fCurrentDistance) return +1;
	return 0; */
}

//안태훈 수정 시작	//(5th Closed beta)(0.2)
FLOAT g_fSndVol = 50.0f;
FLOAT g_fMscVol = 50.0f;
//안태훈 수정 끝	//(5th Closed beta)(0.2)

// update all 3d effects and copy internal data
void CSoundLibrary::UpdateSounds(void)
{	
	// do nothing if no sound
	if( sl_sfFormat==SF_NONE) return;
	
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	extern ENGINE_API BOOL g_bNoPlaySnd;
	if(g_bNoPlaySnd)
	{
		snd_fSoundVolume = 0;
		snd_fMusicVolume = 0;
	}
	else
	{
		snd_fSoundVolume = g_fSndVol;
		snd_fMusicVolume = g_fMscVol;
	}
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	_sfStats.StartTimer(CStatForm::STI_SOUNDUPDATE);

	// see if we have valid handle for direct sound and eventually reinit sound
	if( !(sl_ulFlags&SLF_USINGWAVEOUT) && _hwndCurrent!=_hwndMain) {
		_hwndCurrent = _hwndMain;
		sl_pDS->SetCooperativeLevel( _hwndCurrent, DSSCL_PRIORITY);
		SetFormat( sl_sfFormat);
	}

	// clamp master volumes
	snd_fSoundVolume = Clamp( snd_fSoundVolume, 0.0f, 1.0f);
	snd_fMusicVolume = Clamp( snd_fMusicVolume, 0.0f, 1.0f);

	// reset volume if library is muted
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
	_bMuteVolume = sl_tmMuteUntil > tvNow.GetSeconds();

	// determine number of listeners and get listener
	INDEX ctListeners=0;
	CSoundListener *sli;
	{FOREACHINLIST( CSoundListener, sli_lnInActiveListeners, _pSound->sl_lhActiveListeners, itsli) {
		sli = itsli;
		ctListeners++;
	}}

	// update environment effect (multiple listeners are not supported!)
	const BOOL bDoEFX = (ctListeners==1);
	if( bDoEFX) SetEnvironment( sli->sli_eEnvType);
	else        SetEnvironment( SEFX_NORMAL);

	// check if we have a hearable voice pending (so we can attenuate volume)
	_bAttenuateVolume = FALSE;
	{FOREACHINLIST( CSoundObject, so_Node, sl_lhPendingSounds, itso) {
		CSoundObject &so = *itso;
		if( (so.so_ulFlags&SOF_VOICE) && so.so_fCurrentVolume>0.2f) _bAttenuateVolume = TRUE;
		// while we're at it, reset sound mass also
		if( so.so_psdLink!=NULL) so.so_psdLink->sd_fSoundMass = 0;
	}}

	// for each pending sound
	{FORDELETELIST( CSoundObject, so_Node, sl_lhPendingSounds, itso) {
		CSoundObject &so = *itso;
		// if this sound is playing
		if( so.so_ulFlags&SOF_PLAYING) { 
			// update sound parameters (updates 3D effects also)
			so.Update();
		 _sfStats.IncrementCounter(CStatForm::SCI_SOUNDSPENDING);  // count
		}
		// if it is not playing (anymore)
		if( !(so.so_ulFlags&SOF_PLAYING)) { 
			// just remove it from pending list
			RemoveFromPendingList(so);
		}
	}}

	// sort list of pending/playing sounds
	sl_lhPendingSounds.Sort( &qsort_CompareSoundObjects, offsetof(CSoundObject, so_Node));

	// clamp allowed buffer counts
	snd_iMaxVoices = Clamp( snd_iMaxVoices, SND_MINBUFFERS, SND_MAXBUFFERS);

	// make sure that main HW buffers are playing
	if( !(sl_ulFlags&SLF_USINGWAVEOUT)) DSPlayMainBuffers();

	// mixin the sounds via SW mixer (20 times per second should be enough)
	static CTimerValue tvLast = 0I64;
	const TIME tmElapsed = (tvNow-tvLast).GetSeconds();
	if( tmElapsed>0.05f) {
	//if( tmElapsed>0.2f) {		// tested by seo
	 _sfStats.StopTimer( CStatForm::STI_SOUNDUPDATE);
		tvLast = tvNow;
		MixSounds();
	 _sfStats.StartTimer(CStatForm::STI_SOUNDUPDATE);
	}

	// if using direct sound hardware
	if( sl_ulFlags & SLF_USINGDS3D)
	{
		// play HW sounds and apply HW parameters
		PlayBuffers();
		sl_pds3dListener->CommitDeferredSettings();
	}

	// remove all listeners
	{FORDELETELIST( CSoundListener, sli_lnInActiveListeners, sl_lhActiveListeners, itsli) {
		itsli->sli_lnInActiveListeners.Remove();
	}}

	// done (phew!:)
 _sfStats.StopTimer(CStatForm::STI_SOUNDUPDATE);
}




/*
 *  MIXER helper functions
 */


// copying of mixer buffer to sound buffer(s)

static LPVOID _lpData, _lpData2;
static DWORD  _dwSize, _dwSize2;

void CSoundLibrary::CopyMixerBuffer_dsound( SLONG slMixedSize)
{
	LPVOID lpData;
	DWORD dwSize;
	SLONG slPart1Size, slPart2Size;
	// lock stereo buffer and copy first part of block
	if( !DSLockBuffer( sl_pdsbStream, sl_slMixerBufferSize, lpData, dwSize)) return;
	slPart1Size = Min( sl_slMixerBufferSize-_iWriteOffset, slMixedSize);
	CopyMixerBuffer( 0, ((UBYTE*)lpData)+_iWriteOffset, slPart1Size);
	// copy second part of block
	slPart2Size = slMixedSize - slPart1Size;
	CopyMixerBuffer( slPart1Size, lpData, slPart2Size);
	_iWriteOffset += slMixedSize;
	if( _iWriteOffset>=sl_slMixerBufferSize) _iWriteOffset -= sl_slMixerBufferSize;
	ASSERT( _iWriteOffset>=0 && _iWriteOffset<sl_slMixerBufferSize);
	sl_pdsbStream->Unlock( lpData, dwSize, NULL, 0);
}


void CSoundLibrary::CopyMixerBuffer_waveout(void)
{
	MMRESULT res;
	SLONG slOffset = 0;
	for( INDEX iBuffer = 0; iBuffer<sl_awhWOBuffers.Count(); iBuffer++)
	{ // skip prepared buffer
		WAVEHDR &wh = sl_awhWOBuffers[iBuffer];
		if( wh.dwFlags&WHDR_PREPARED) continue;
		// copy part of a mixer buffer to wave buffer
		CopyMixerBuffer( slOffset, wh.lpData, WAVEOUTBLOCKSIZE);
		slOffset += WAVEOUTBLOCKSIZE;
		// write wave buffer (ready for playing)
		res = waveOutPrepareHeader( sl_hwoWaveOut, &wh, sizeof(wh));
		ASSERT( res==MMSYSERR_NOERROR);
		res = waveOutWrite( sl_hwoWaveOut, &wh, sizeof(wh));
		ASSERT( res==MMSYSERR_NOERROR);
	}
}


// finds room in sound buffer to copy in next crop of samples
SLONG CSoundLibrary::PrepareSoundBuffer_dsound(void)
{
	// determine writable block size (difference between write and play pointers)
	SLONG slDataToMix;
	ASSERT( sl_pdsbStream!=NULL);
	DWORD dwCurrentCursor;
	HRESULT hr = sl_pdsbStream->GetCurrentPosition( &dwCurrentCursor, NULL);
	if( hr!=DS_OK) return DSFail( TRANS("  ! DirectSound error: Cannot obtain sound buffer write position.\n"));

	// store pointer and wrapped block size
	slDataToMix = dwCurrentCursor - _iWriteOffset;
	if( slDataToMix<0) slDataToMix += sl_slMixerBufferSize;
	ASSERT( slDataToMix>=0 && slDataToMix<=sl_slMixerBufferSize);
	slDataToMix = Min( slDataToMix, sl_slMixerBufferSize); 

	// done
	return slDataToMix;
}


SLONG CSoundLibrary::PrepareSoundBuffer_waveout(void)
{

	// scan waveout buffers to find all that are ready to receive sound data (i.e. not playing)
	SLONG slDataToMix=0;
	for( INDEX iBuffer=0; iBuffer<sl_awhWOBuffers.Count(); iBuffer++)
	{ // if done playing
		WAVEHDR &wh = sl_awhWOBuffers[iBuffer];
		if( wh.dwFlags&WHDR_DONE) {
			// unprepare buffer
			MMRESULT res = waveOutUnprepareHeader( sl_hwoWaveOut, &wh, sizeof(wh));
			ASSERT( res == MMSYSERR_NOERROR);
		}
		// if unprepared
		if( !(wh.dwFlags&WHDR_PREPARED)) {
			// increase mix-in data size
			slDataToMix += WAVEOUTBLOCKSIZE;
		}
	}
	// done
	ASSERT( slDataToMix <= sl_slMixerBufferSize);
	return slDataToMix;

}

	
// listen from this listener this frame
void CSoundLibrary::Listen( CSoundListener &sli)
{
	// just add it to list
	if( sli.sli_lnInActiveListeners.IsLinked()) sli.sli_lnInActiveListeners.Remove();
	sl_lhActiveListeners.AddTail( sli.sli_lnInActiveListeners);
}



// add/remove object in pending list
void CSoundLibrary::AddToPendingList( CSoundObject &so)
{
	ASSERT( !so.IsHooked());
	sl_lhPendingSounds.AddTail(so.so_Node);
}


void CSoundLibrary::RemoveFromPendingList( CSoundObject &so, const BOOL bReleaseNow/*=FALSE*/)
{ 
	// if using playing buffer
	if( so.so_iPlayingBuffer>=0) {
		ReleasePlayingBuffer( so.so_iPlayingBuffer, bReleaseNow);
		so.so_iPlayingBuffer = -1;
	}
	// remove it from list
	if( so.IsHooked()) so.so_Node.Remove();
}


// request a playing buffer
INDEX CSoundLibrary::CommitPlayingBuffer( CSoundObject &so, BOOL &bWasUsed)
{
	// streaming sounds (music) should not commit playing buffers
	ASSERT( so.so_psdLink!=NULL);
	CSoundData &sd = *so.so_psdLink;
	ASSERT( sd.sd_pdsBuffer!=NULL || sd.sd_pswSamples!=NULL);
	const BOOL bMono  = (sd.sd_wfeFormat.nChannels==1);
	const BOOL bVoice = (so.so_ulFlags&SOF_VOICE);
	const BOOL bSet3D = (so.so_ulFlags&SOF_3D) && bMono && !bVoice;
	DWORD dwStatus;
	HRESULT hr;

	// find 1st non-playing buffer
	INDEX iBuffer = -1;
	FLOAT fMinVolume = MAX_FLOAT;
	INDEX iMinVolBuf = -1;
	for( INDEX iBuf=0; iBuf<snd_iMaxVoices; iBuf++) {
		// skip used
		PlayingBuffer &pb = sl_appbPlaying[iBuf];
		if( pb.pb_pso!=NULL) {
			// for occupied playing buffers, keep an eye on the one with lowest volume
			if( fMinVolume>pb.pb_pso->so_fCurrentVolume) {
				fMinVolume = pb.pb_pso->so_fCurrentVolume;
				iMinVolBuf = iBuf;
			} // move along, sir...
			continue;
		}
		iBuffer = iBuf; // remember found one
		// check if it's still playing
		if( pb.pb_pdsb==NULL) break;
		hr = pb.pb_pdsb->GetStatus(&dwStatus);
		ASSERT( hr==DS_OK);
		// if not playing - that's the one (like Neo in "The Matix"!)
		if( !(dwStatus&DSBSTATUS_PLAYING)) break;
	}

	// if no empty buffer is available
	bWasUsed = FALSE;
	if( iBuffer==-1) {
		// check if quiet sound can be replaced (current sound should be at least 1/3rd louder)
		const FLOAT fVolumeDelta = so.so_fCurrentVolume - fMinVolume;
		if( fVolumeDelta<0.333f) return -1;
		PlayingBuffer &pbMinVol = sl_appbPlaying[iMinVolBuf];
		ASSERT( fMinVolume==pbMinVol.pb_pso->so_fCurrentVolume);
		ASSERT( iMinVolBuf==pbMinVol.pb_pso->so_iPlayingBuffer);
		// remove quiet sound and put this one instead
		pbMinVol.pb_pso->so_iPlayingBuffer = -1;
		ReleasePlayingBuffer( iMinVolBuf, TRUE);
		iBuffer  = iMinVolBuf;
		bWasUsed = TRUE;
	}

	// clamp that playing buffer from HW
	PlayingBuffer &pb = sl_appbPlaying[iBuffer];


	// on PC, use duplicate sound buffer
	ASSERT( sd.sd_pdsBuffer!=NULL);
	if( pb.pb_pdsb!=NULL) pb.pb_pdsb->Release(); // eventually release old buffer
	pb.pb_pdsb = NULL;
	hr = sl_pDS->DuplicateSoundBuffer( sd.sd_pdsBuffer, &pb.pb_pdsb);
	if( hr!=DS_OK) return -1;

	if( bMono) { // (already loaded as 3D)
		// release old 3D control (and EAX properties)
		if( pb.pb_pdsb3d!=NULL) {
			pb.pb_pdsb3d->Release();
			pb.pb_pdsb3d = NULL;
			if( pb.pb_pksps!=NULL) {
				pb.pb_pksps->Release();
				pb.pb_pksps = NULL;
			}
		} // obtain 3D controls
		hr = pb.pb_pdsb->QueryInterface( IID_IDirectSound3DBuffer8, (LPVOID*)&pb.pb_pdsb3d);
		if( hr!=DS_OK) return -1; // didn't make it? :(
		// try to obtain control for EAX source properties
		ASSERT( pb.pb_pksps==NULL);
		const BOOL bHasEFX = sl_ulFlags & SLF_HASEFX;
		if( bSet3D && bHasEFX) pb.pb_pdsb3d->QueryInterface( IID_IKsPropertySet, (LPVOID*)&pb.pb_pksps);

		// set 3D mode as 2D (because on PC, sound is already loaded as 3D)
		const DWORD dwMode = bSet3D ? DS3DMODE_HEADRELATIVE : DS3DMODE_DISABLE;
		hr = pb.pb_pdsb3d->SetMode( dwMode, DS3D_IMMEDIATE);
		ASSERT( hr==DS_OK);
	}


	// mark, keep assignment and return commited buffer
	so.so_ulFlags |= SOF_WAITING;
	pb.pb_pso = &so;    
	pb.pb_psd = &sd;
	return iBuffer;
}


// remove references to sound object and data from playing buffer
void CSoundLibrary::ReleasePlayingBuffer( const INDEX iBuffer, const BOOL bImmediately/*=FALSE*/)
{
	ASSERT( iBuffer>=0 && iBuffer<SND_MAXBUFFERS);
	sl_appbPlaying[iBuffer].pb_pso = NULL;
	sl_appbPlaying[iBuffer].pb_psd = NULL;
	if( bImmediately) StopReleaseSoundBuffer( sl_appbPlaying[iBuffer], TRUE);
}



// update mixer
void CSoundLibrary::MixSounds(void)
{
	ASSERT( sl_sfFormat!=SF_NONE);
 _sfStats.StartTimer(CStatForm::STI_SOUNDMIXING);

	// seek available buffer(s) for next crop of samples
	SLONG slDataToMix;
	const BOOL bUsingDS3D    = sl_ulFlags & SLF_USINGDS3D;
	const BOOL bUsingWaveOut = sl_ulFlags & SLF_USINGWAVEOUT;
	if( bUsingWaveOut) {
		// using wave out 
		slDataToMix = PrepareSoundBuffer_waveout();
	} else {
		// using direct sound
		slDataToMix = PrepareSoundBuffer_dsound();
	}

	// skip mixing if all sound buffers are still busy playing
	ASSERT( slDataToMix>=0);
	if( slDataToMix<=0) {
		_sfStats.StopTimer(CStatForm::STI_SOUNDMIXING);
		return;
	}

	// prepare mixer buffer and readout game state
	extern void ResetMixer( const SWORD *pswBuffer, const SLONG slBufferSize);
	ResetMixer( sl_pswMixerBuffer, slDataToMix);
	const BOOL bGamePaused = _pNetwork==NULL || _pNetwork->IsPaused()
											 || (_pNetwork->IsServer() && _pNetwork->GetLocalPause());
	// for each pending sound
	INDEX ctAvailableVoices = snd_iMaxVoices; // countdown
	{FOREACHINLIST( CSoundObject, so_Node, sl_lhPendingSounds, itso)
	{
		// shortcut
		CSoundObject &so = *itso;

		// skip this sound if we don't have available mixer 'voices'
		if( ctAvailableVoices<=0 && so.so_psdcDecoder==NULL) continue;

		// skip if using HW and this is not encoded sound
		if( bUsingDS3D && so.so_psdcDecoder==NULL) continue;

		// if the sound is in-game sound, and the game is paused, or sound is paused or stopped
		BOOL bAttenuate = FALSE;
		if( (bGamePaused && !(so.so_ulFlags&SOF_NONGAME))
		 || (so.so_ulFlags&SOF_PAUSED) || !(so.so_ulFlags&SOF_PLAYING)) {
			// skip sound if already silent
			if( so.so_fLastLeftVolume==0 && so.so_fLastRightVolume==0) continue;
			// force volume attenuation to silence
			bAttenuate = TRUE;
		}
		// mix in this sound
		extern void MixSound( CSoundObject &so, BOOL bAttenuate);
		MixSound( so, bAttenuate);

		// decrease number of available SW mixer 'voices' for regular sounds
		if( !bUsingDS3D && so.so_psdcDecoder==NULL) ctAvailableVoices--;
	 _sfStats.IncrementCounter(CStatForm::SCI_SOUNDSPLAYING);  // count
	}}

	// copy mixer buffer to playing buffer
	if( bUsingWaveOut) {
		// using wave out 
		CopyMixerBuffer_waveout();
	} else {
		// using direct sound
		CopyMixerBuffer_dsound(slDataToMix);
	}

	// all done
	_sfStats.StopTimer(CStatForm::STI_SOUNDMIXING);
}



// update dsound buffers (if using HW mixer)
void CSoundLibrary::PlayBuffers(void)
{
	HRESULT hr;
	DWORD dwStatus;
	ASSERT( sl_sfFormat!=SF_NONE && (sl_ulFlags&SLF_USINGDS3D));  // only in HW, of course
	const BOOL bGamePaused = _pNetwork==NULL || _pNetwork->IsPaused()
											 || (_pNetwork->IsServer() && _pNetwork->GetLocalPause());

	// stop or release empty playing buffers
	{for( INDEX iPB=0; iPB<SND_MAXBUFFERS; iPB++) {
		PlayingBuffer &pb = sl_appbPlaying[iPB];
		if( pb.pb_pso==NULL && pb.pb_pdsb!=NULL) {
			StopReleaseSoundBuffer( pb, FALSE);
		}
	}}

	// for each pending sound
	INDEX ctAvailableBuffers = snd_iMaxVoices; // countdown
	{FORDELETELIST( CSoundObject, so_Node, sl_lhPendingSounds, itso)
	{
		// shortcuts
		CSoundObject &so = *itso;
		if( so.so_psdLink==NULL || so.so_psdcDecoder!=NULL) continue; // skip empty or encoded sound objects
		const BOOL bIs3D = (so.so_ulFlags&SOF_3D);
		INDEX &iPB = so.so_iPlayingBuffer; 
		CSoundData &sd = *so.so_psdLink;
		WAVEFORMATEX &wfe = sd.sd_wfeFormat;

		// release mode?
		ASSERT( ctAvailableBuffers>=0);
		if( ctAvailableBuffers<=0) {
			// if HW buffer is used
			if( iPB>=0) {
				PlayingBuffer &pb = sl_appbPlaying[iPB];
				ASSERT( pb.pb_pdsb!=NULL);
				StopReleaseSoundBuffer( pb, TRUE);
				iPB = -1;
			} // no new commits allowed
			continue;
		}

		// commit mode! - check for silent sound
		const BOOL bIsSilent  = (so.so_fCurrentVolume < 0.005f);
		const BOOL bWasSilent = (so.so_tmNotAudible > 0);
		if(  bIsSilent && !bWasSilent) so.ResetTimer(1);
		if( !bIsSilent &&  bWasSilent) so.so_tmNotAudible = 0;   // reset if not silent any more
		ASSERT( (!bIsSilent && so.so_tmNotAudible==0) || (bIsSilent && so.so_tmNotAudible!=0));

		// commit playing buffer if not yet commited
		BOOL bInPlayBuffer = (iPB>=0);
		if( bInPlayBuffer) {
			// check if volume was too low for long time
			if( bIsSilent && bWasSilent) {
				const TIME tmSilent = so.GetElapsedTime(1);
				if( tmSilent>10.0f) { // seconds (should be enough)
					// release playing buffer
					ReleasePlayingBuffer(iPB); 
					iPB = -1;
					continue;
				}
			} // if not playing correct sound data
			PlayingBuffer &pb = _pSound->sl_appbPlaying[iPB];
			if( pb.pb_psd != &sd) {
				// release playing buffer
				_pSound->ReleasePlayingBuffer(iPB); 
				iPB = -1;
				bInPlayBuffer = FALSE;
			}
		}
		// if not already using playing buffer, sound is hearable and not paused
		BOOL bUsedBuffer = FALSE;
		const BOOL bSoundPaused = (so.so_ulFlags&SOF_PAUSED);
		if( !bInPlayBuffer && !bIsSilent && !bSoundPaused) {
			// commit one buffer
			iPB = CommitPlayingBuffer( so, bUsedBuffer);
		}

		// done here if didn't manage to commit play buffer
		if( iPB<0) continue;
		PlayingBuffer &pb = _pSound->sl_appbPlaying[iPB];
		ASSERT( pb.pb_pdsb!=NULL);
		// one buffer less available :(
		if( !bUsedBuffer) ctAvailableBuffers--;

		// if not already playing
		if( !bInPlayBuffer) {
			// determine starting position
			DWORD dwPlayPos = 0;
			TIME  tmPlaying = so.GetElapsedTime(0);
			if( tmPlaying>0.1f) {
				const SLONG slOffset = (SLONG)(tmPlaying * wfe.nSamplesPerSec) % sd.sd_ctSamples;
				DWORD dwPlayPos = slOffset * wfe.nChannels * wfe.wBitsPerSample / 8;
				if( wfe.wBitsPerSample==4) dwPlayPos -= dwPlayPos % wfe.nBlockAlign;
			} // set HW
			hr = pb.pb_pdsb->SetCurrentPosition(dwPlayPos);
			ASSERT( hr==DS_OK);
		} 

		// set 3D HW parameters if possible
		const BOOL bSet3D = bIs3D && wfe.nChannels==1 && !(so.so_ulFlags&SOF_VOICE);
		if( bSet3D) {
			ASSERT( pb.pb_pdsb3d!=NULL);
			hr = pb.pb_pdsb3d->SetPosition( so.so_vCurrentPosition(1), so.so_vCurrentPosition(2), -so.so_vCurrentPosition(3), DS3D_DEFERRED);  ASSERT( hr==DS_OK);
			hr = pb.pb_pdsb3d->SetVelocity( so.so_vCurrentSpeed(1),    so.so_vCurrentSpeed(2),    -so.so_vCurrentSpeed(3),    DS3D_DEFERRED);  ASSERT( hr==DS_OK);
		}

		// determine playing and pause state
		ASSERT( !bSoundPaused); // manually paused sound cannot be in playing buffer
		hr = pb.pb_pdsb->GetStatus(&dwStatus);  ASSERT( hr==DS_OK);
		const BOOL bShouldPause  = (bGamePaused && !(so.so_ulFlags&SOF_NONGAME));
		const BOOL bLoopingSound = (so.so_ulFlags&SOF_LOOP);
		BOOL bSoundPlaying = (dwStatus&DSBSTATUS_PLAYING);

		// set HW parameters
		if( !bShouldPause) {
			// volume
			DWORD dwVolume = DSBVOLUME_MIN;
			if( so.so_fCurrentVolume>0.005f) {
				const FLOAT fVolume = so.so_fCurrentVolume;
				const FLOAT fVolLog = log10(fVolume);
				if( snd_bLinearVolume) { // (X-1)*X + logX*(1-X)
					const FLOAT fVolLin = (fVolume-1);
					dwVolume = 3000 * (fVolLin*fVolume + fVolLog*(1-fVolume));     
					snd_bLinearVolume = TRUE; // clamp
				} else { // logX
					dwVolume = 3000 * fVolLog;
				}
			} // set it
			hr = pb.pb_pdsb->SetVolume(dwVolume);
			ASSERT( hr==DS_OK);
			DWORD dwFrequency = DSBFREQUENCY_ORIGINAL;
			if( so.so_fPitch!=1) dwFrequency = so.so_psdLink->sd_wfeFormat.nSamplesPerSec * so.so_fPitch;
			hr = pb.pb_pdsb->SetFrequency(dwFrequency);
			ASSERT( hr==DS_OK);
		 _sfStats.IncrementCounter(CStatForm::SCI_SOUNDSPLAYING);
		}

		// if playing but should of been paused
		if( bSoundPlaying && bShouldPause) {
			// mark and stop it now (current position has not been reset)
			so.so_ulFlags |= SOF_WAITING;
			hr = pb.pb_pdsb->Stop();
			ASSERT( hr==DS_OK);
			bSoundPlaying = FALSE;
		}
		// if stopped but maybe should playing
		else if( !bSoundPlaying && !bShouldPause) {
			// if not looping and done with playing
			const TIME tmDelta = so.GetElapsedTime(0);
			if( !bLoopingSound && (!(so.so_ulFlags&SOF_WAITING) || tmDelta>so.so_psdLink->sd_tmLength)) {
				// its done for good
				so.Stop();
			} else {
				// play it again, Sam! (from current position)
				const DWORD dwFlags = bLoopingSound ? DSBPLAY_LOOPING : NONE; 
				hr = pb.pb_pdsb->Play( 0,0, dwFlags);
				ASSERT( hr==DS_OK);
				bSoundPlaying = TRUE;
			}
			// unmark
			so.so_ulFlags &= ~SOF_WAITING;
		}

		// set EAX filter if possible (EAX source have to be set AFTER the sound begins playing)
		if( bSet3D && bSoundPlaying && pb.pb_pksps!=NULL) {
			INDEX iFilter = 0;
			if( snd_bUseEnvironmentFX) iFilter = Clamp( FloatToInt(-100*so.so_fCurrentFilter), -10000L, 0L);
			hr = pb.pb_pksps->Set( DSPROPSETID_EAX_BufferProperties, DSPROPERTY_EAXBUFFER_DIRECTHF|DSPROPERTY_EAXBUFFER_DEFERRED,
														 NULL, 0, &iFilter, sizeof(DWORD));
			ASSERT( hr==DS_OK);
		} 

		// move to next sound in pending list
		//_strDebug += "\n";
	}}
}
