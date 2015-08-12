#include "stdh.h"

#include <Engine/Base/Stream.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/CRC.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Functions.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Sound/SoundObject.h>
#include <Engine/Sound/SoundDecoder.h>
#include <Engine/Sound/SoundData.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Templates/Stock_CSoundData.h>

// console variables
extern FLOAT snd_fSoundVolume;
extern FLOAT snd_fMusicVolume;

// debug printout
extern CTString _strDebug;
extern PIX _pixDebugStringX, _pixDebugStringY;
extern COLOR _colDebugString;

// for network transport
static UBYTE _abSoundData[ESOUND_MAX_DATA_SIZE] = {0};
// for math
static FLOAT3D vOne = FLOAT3D(0,0,-1);


/*
 *  Constructor
 */
CSoundObject::CSoundObject()
{
	// clear sound settings
	so_psdLink     = NULL;
	so_penEntity   = NULL;
	so_psdcDecoder = NULL;
	so_tmPlayBegin  = 0;
	so_tmNotAudible = 0;
	so_tmFadeBegin  = 0;
	so_iPlayingBuffer = -1;
	so_ulFlags = NONE;

	so_fFadeVolume = 1;
	so_tmFadeTime  = 0;

	so_fVolume  = 1;
	so_fPitch   = 1;
	so_fHotSpot = 0;
	so_fFallOff = 0;
	so_fCurrentFilter = 0;
	so_fCurrentVolume = 0;
	so_fCurrentDistance = 10000;

	so_fLeftVolume  = 0;   
	so_fRightVolume = 0;  
	so_slLeftFilter  = 0x7FFF;  
	so_slRightFilter = 0x7FFF; 
	so_fPitchShift = 1;   
	so_fPhaseShift = 0;   

	so_fLastLeftVolume  = 0;
	so_fLastRightVolume = 0;
	so_swLastLeftSample  = 0;
	so_swLastRightSample = 0;
	so_fLeftOffset  = 0;
	so_fRightOffset = 0;
	so_fOffsetDelta = 0;
}


// destructor
CSoundObject::~CSoundObject()
{
	Stop_internal(TRUE);
}


// copy from another object of same class
void CSoundObject::Copy( CSoundObject &soOther)
{
	Stop_internal(TRUE);
	so_penEntity = NULL;
	so_ulFlags  = soOther.so_ulFlags;
	so_fVolume  = soOther.so_fVolume; 
	so_fPitch   = soOther.so_fPitch;  
	so_fHotSpot = soOther.so_fHotSpot;
	so_fFallOff = soOther.so_fFallOff;
	so_fFadeVolume = soOther.so_fFadeVolume;
	so_tmFadeTime  = soOther.so_tmFadeTime;
	so_fCurrentDistance = soOther.so_fCurrentDistance;
	so_fCurrentVolume = soOther.so_fCurrentVolume;
	so_fCurrentFilter = soOther.so_fCurrentFilter;
	so_fLeftVolume   = soOther.so_fLeftVolume;   
	so_fRightVolume  = soOther.so_fRightVolume;  
	so_slLeftFilter  = soOther.so_slLeftFilter;  
	so_slRightFilter = soOther.so_slRightFilter; 
	so_fPhaseShift   = soOther.so_fPhaseShift;   
	so_fPitchShift   = soOther.so_fPitchShift;   
	if( soOther.IsPlaying()) Play( soOther.so_psdLink, soOther.so_ulFlags);
}


// set sound volume
void CSoundObject::SetVolume( const FLOAT fVolume, BOOL bNetwork/*=TRUE*/)
{
	//ASSERT( fVolume>=0 && fVolume<=1);
	const FLOAT fNewVolume = ClampUp( fVolume, 1.0f);  // fixup for now!
	const FLOAT fDiff = fabs(so_fVolume-fNewVolume);
	so_fVolume = fNewVolume;

	// reset fader
	so_fFadeVolume = 1;
	so_tmFadeTime  = 0;

	// send over network
	CEntity *penEntity = so_penEntity;
	if( fDiff>0.01f && _pNetwork!=NULL && _pNetwork->IsServer()
	 && penEntity!=NULL && penEntity->IsSentOverNet() && bNetwork) {
		const ULONG ulComponentOffset = ((UBYTE*)this) - ((UBYTE*)penEntity);
		const UBYTE ubVolume = so_fVolume *127;
		UBYTE *pubData = _abSoundData;
		*(ULONG*)pubData = ulComponentOffset;  pubData += sizeof(ulComponentOffset);
		*(UBYTE*)pubData = ubVolume;           pubData += sizeof(ubVolume);    
		extern CEntityMessage _emEntityMessage;
		SLONG slDataSize = sizeof(ulComponentOffset) + sizeof(ubVolume);
		_emEntityMessage.WriteSound( penEntity->en_ulID, EM_SOUND_SETVOLUME, _abSoundData, slDataSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	} 
}


// set sound pitch
void CSoundObject::SetPitch( const FLOAT fPitch, BOOL bNetwork/*=TRUE*/)
{
	ASSERT( fPitch>0);
	const FLOAT fDiff = fabs(so_fPitch-fPitch);
	so_fPitch = fPitch;

	// send over network
	CEntity *penEntity = so_penEntity;
	if( fDiff>0.01f && _pNetwork!=NULL && _pNetwork->IsServer()
	 && penEntity!=NULL && penEntity->IsSentOverNet() && bNetwork) {
		const ULONG ulComponentOffset = ((UBYTE*)this) - ((UBYTE*)penEntity);
		const UBYTE ubPitch = so_fPitch *63;
		UBYTE *pubData = _abSoundData;
		*(ULONG*)pubData = ulComponentOffset;  pubData += sizeof(ulComponentOffset);
		*(UBYTE*)pubData = ubPitch;            pubData += sizeof(ubPitch);    
		extern CEntityMessage _emEntityMessage;
		SLONG slDataSize = sizeof(ulComponentOffset) + sizeof(ubPitch);
		_emEntityMessage.WriteSound( penEntity->en_ulID, EM_SOUND_SETPITCH, _abSoundData, slDataSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	} 
}


// set range
void CSoundObject::SetRange( const FLOAT fHotSpot, const FLOAT fFallOff, BOOL bNetwork/*=TRUE*/)
{
	ASSERT( fHotSpot>=0 && fFallOff>fHotSpot);

	const FLOAT fDiff = fabs(so_fFallOff-fFallOff) + fabs(so_fHotSpot-fHotSpot);
	so_fHotSpot = fHotSpot;
	so_fFallOff = fFallOff;

	// send over network
	CEntity *penEntity = so_penEntity;
	if( fDiff>0.01f && _pNetwork!=NULL && _pNetwork->IsServer()
	 && penEntity!=NULL && penEntity->IsSentOverNet() && bNetwork) {
		const ULONG ulComponentOffset = ((UBYTE*)this) - ((UBYTE*)penEntity);
		const UWORD uwHotSpot = Clamp( so_fHotSpot*5, 0.0f, 65535.0f);
		const UWORD uwFallOff = Clamp( so_fFallOff*5, 0.0f, 65535.0f);
		UBYTE *pubData = _abSoundData;
		*(ULONG*)pubData = ulComponentOffset;  pubData += sizeof(ulComponentOffset);
		*(UWORD*)pubData = uwHotSpot;          pubData += sizeof(uwHotSpot);
		*(UWORD*)pubData = uwFallOff;          pubData += sizeof(uwFallOff);
		extern CEntityMessage _emEntityMessage;
		SLONG slDataSize = sizeof(ulComponentOffset) + sizeof(uwFallOff) + sizeof(uwHotSpot);
		_emEntityMessage.WriteSound( penEntity->en_ulID, EM_SOUND_SETRANGE, _abSoundData, slDataSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	} 
}



/*
 *  Play
 */
void CSoundObject::Play( CSoundData *psdLink, ULONG ulFlags)
{
	// check if should continue immediately with new sound
	const BOOL bPlaying = (so_ulFlags&SOF_PLAYING);
	// "stop" it just in case
	Stop_internal(FALSE);

	// mark new data as referenced once more (mark old data as referenced once less)
	psdLink->AddReference();    
	so_psdLink->RemReference(); 
	so_psdLink = psdLink;
	// add to pending list if not added yet
	if( !IsHooked()) _pSound->AddToPendingList(*this);

	// store flags and reset timers
	so_ulFlags = ulFlags | SOF_PLAYING;
	so_tmPlayBegin  = 0;
	so_tmNotAudible = 0;
	// reset fader
	so_fFadeVolume = 1;
	so_tmFadeTime  = 0;

	// create decoder if the sound data is encoded
	if( so_psdLink->sd_ulFlags&SDF_ENCODED) {
		so_psdcDecoder = new CSoundDecoder(so_psdLink->GetName(), so_psdLink->sd_pubDecoderHeader, so_psdLink->sd_slHeaderSize);
	}

	// done here if vars have been initialized
	if( ulFlags&SOF_LOADED) return;

	// reset some vars
	so_fLeftOffset  = 0.0f;
	so_fRightOffset = 0.0f;
	so_fOffsetDelta = 0.0f;
	// should play right now?
	if( bPlaying) {
		// adjust for master volume
		if( so_ulFlags&SOF_MUSIC) {
			so_fLeftVolume  *= snd_fMusicVolume/2;
			so_fRightVolume *= snd_fMusicVolume/2;
		} else {
			so_fLeftVolume  *= snd_fSoundVolume;
			so_fRightVolume *= snd_fSoundVolume;
		}
	} else {
		// reset totally
		so_fLastLeftVolume   = 0;
		so_fLastRightVolume  = 0;
		so_swLastLeftSample  = 0;
		so_swLastRightSample = 0;
	}
}


/*
 *  Stop
 */
void CSoundObject::Stop( const BOOL bNetwork/*=TRUE*/)
{
	// do network
	CEntity* penEntity = so_penEntity;
	if( _pNetwork!=NULL && _pNetwork->IsServer()
	 && penEntity!=NULL && penEntity->IsSentOverNet() && bNetwork) {
		ULONG ulComponentOffset = ((UBYTE*)this) - ((UBYTE*)penEntity);
		UBYTE *pubData = _abSoundData;
		*(ULONG*)pubData = ulComponentOffset;  pubData += sizeof(ulComponentOffset);
		SLONG slDataSize = sizeof(ulComponentOffset);
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteSound( penEntity->en_ulID, EM_SOUND_STOP, _abSoundData, slDataSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}
	// signal to stop
	Stop_internal(TRUE);
}


// really stop sound
void CSoundObject::Stop_internal( const BOOL bReleasePlayingBuffer)
{
	// sound is stoped
	so_ulFlags &= ~(SOF_PLAYING|SOF_PAUSED);

	// if required remove it from pending list
	if( _pSound!=NULL) _pSound->RemoveFromPendingList( *this, bReleasePlayingBuffer);

	// destroy decoder if exists
	if( so_psdcDecoder!=NULL) {
		delete so_psdcDecoder;
		so_psdcDecoder = NULL;
	}

	// remove reference from SoundData
	if( so_psdLink!=NULL) {
		so_psdLink->RemReference();
		so_psdLink = NULL;
	}
}


// fade to fVolume, thru tmPeriod and bStop at end
void CSoundObject::Fade( const FLOAT fVolume, const TIME tmPeriod, const BOOL bNetwork)
{
	ASSERT( tmPeriod>0);
	ASSERT( fVolume>=0 && fVolume<=1);

	// do network
	CEntity* penEntity = so_penEntity;
	if( _pNetwork!=NULL && _pNetwork->IsServer()
	 && penEntity!=NULL && penEntity->IsSentOverNet() && bNetwork) {
		ULONG ulComponentOffset = ((UBYTE*)this) - ((UBYTE*)penEntity);
		UBYTE *pubData = _abSoundData;
		*(ULONG*)pubData = ulComponentOffset;  pubData += sizeof(ulComponentOffset);
		SLONG slDataSize = sizeof(ulComponentOffset) + sizeof(UBYTE) + sizeof(tmPeriod);
		const UBYTE ubVolume = fVolume *127;
		*(UBYTE*)pubData = ubVolume;           pubData += sizeof(ubVolume);
		*(TIME*)pubData = tmPeriod;           pubData += sizeof(tmPeriod);
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteSound( penEntity->en_ulID, EM_SOUND_FADE, _abSoundData, slDataSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}

	so_fFadeVolume = fVolume;
	so_tmFadeTime  = tmPeriod;
	ResetTimer(2);
}



// update 3D effects
static void Update3D( CSoundObject &so, FLOAT3D &vRelPosition, FLOAT3D &vRelSpeed, 
											FLOAT &fRelVolume, FLOAT &fRelDistance, FLOAT &fRelFilter)
{
	// skip if not sound, not 3D or can't be 3D
	ASSERT( so.so_ulFlags&SOF_3D);
	vRelSpeed = FLOAT3D(0,0,0);
	vRelPosition = FLOAT3D(0,0,10000); // far away
	fRelDistance = 10000;
	fRelVolume = 0;
	fRelFilter = 0;
	if( so.so_psdLink==NULL || so.so_penEntity==NULL) return;

	// get listener's position and speed
	FLOAT3D vAbsPosition(0,0,0);
	FLOAT3D vAbsSpeed(0,0,0);
	vAbsPosition = so.so_penEntity->en_plPlacement.pl_PositionVector;
	if( so.so_penEntity->en_ulPhysicsFlags&EPF_MOVABLE) {
		CMovableEntity *penMovable = (CMovableEntity*)so.so_penEntity;
		vAbsSpeed = penMovable->en_vCurrentTranslationAbsolute;
	}

	// totals for all listeners
	FLOAT3D vTotalPosition(0,0,0);
	FLOAT3D vTotalSpeed(0,0,0);
	FLOAT fTotalDistance = 0;
	FLOAT fTotalStrength = 0;
	FLOAT fTotalVolume = 0;
	// for SW mixer
	FLOAT fTotalNonVol = 0;
	FLOAT fTotalLisFilter = 0;
	FLOAT fRelNonVol = 0; 

	// for each listener
	const BOOL bVolumetric = (so.so_ulFlags&SOF_VOLUMETRIC) || so.so_psdcDecoder!=NULL;
	{FOREACHINLIST( CSoundListener, sli_lnInActiveListeners, _pSound->sl_lhActiveListeners, itsli)
	{
		// don't add this listener if local, but not of this listener
		CSoundListener &sli = *itsli;
		if( (so.so_ulFlags&SOF_LOCAL) && so.so_penEntity!=sli.sli_penEntity) continue;

		// calculate distance from listener
		const FLOAT3D vDistance = vAbsPosition - sli.sli_vPosition;
		const FLOAT   fDistance = vDistance.Length();
		// don't add this listener if too far away
		if( fDistance>so.so_fFallOff) {
			// but keep the closest distance for sorting purposes
			if( fRelDistance>fDistance) fRelDistance = fDistance;  
			continue;  
		}

		// calculate distance falloff factor (how much this sound contributes to listeners)
		FLOAT fDistanceFactor = 1.0f;
		FLOAT fVolumetricBias = 0.0f;
		if( fDistance > so.so_fHotSpot) {
			const FLOAT fRange = so.so_fFallOff-so.so_fHotSpot;
			fDistanceFactor = (so.so_fFallOff-fDistance) / fRange;
			fVolumetricBias = (fDistance-so.so_fHotSpot) / fRange *3;   // 1/3rd of range should be just enough
			fVolumetricBias = ClampUp( fVolumetricBias, 1.0f);
			ASSERT( fDistanceFactor>0 && fDistanceFactor<=1);
		}

		// volumetric sound ?
		FLOAT3D vPosition, vSpeed;
		if( bVolumetric && fVolumetricBias<1) {
			// calculate volumetic influence only if outside hot-spot
			if( fVolumetricBias>0) {
				// calculate attenuated speed
				vSpeed = (vAbsSpeed - sli.sli_vSpeed) * !sli.sli_mRotation;
				vSpeed = vSpeed * fVolumetricBias;
				// calculate attenuated position
				FLOAT3D vPos = vDistance * !sli.sli_mRotation;
				vPos.Normalize();
				const FLOAT3D vPlane = (vPos * vOne).Normalize();
				const FLOAT fCosPos = vPos % vOne;
				const FLOAT3D vOrt2 = vOne * vPlane;
				const FLOAT3D vOrt1 = Lerp( vOrt2, FLOAT3D(0,1,0), ClampDn(-fCosPos,0.0f));
				// lerp angle and construct position vector
				ANGLE aLerpPos = ACos(fCosPos) * fVolumetricBias;
				vPosition = vOne*Cos(aLerpPos) + vOrt1*Sin(aLerpPos);
			} else {
				// inside hotspot - just set 'default' position and speed
				vPosition = FLOAT3D(0,0,-1);  // 'in front'
				vSpeed    = FLOAT3D(0,0,0);   // and static
			}
		} // non-volumetric sound !
		else {
			// just find position and speed of sound relative to viewer orientation
			vPosition = vDistance * !sli.sli_mRotation;   // relative to 'front'
			vSpeed    = (vAbsSpeed - sli.sli_vSpeed) * !sli.sli_mRotation;
		}

		// calculate volume attenuation
		const FLOAT fVolume = so.so_fVolume * sli.sli_fVolume * fDistanceFactor;
		const FLOAT fFilter =(so.so_ulFlags&(SOF_LOCAL|SOF_VOICE|SOF_MUSIC)) ? 0 : sli.sli_fFilter;

		// add to totals
		fTotalDistance += fDistance * fDistanceFactor;
		vTotalPosition += vPosition * fDistanceFactor;
		vTotalSpeed    += vSpeed    * fDistanceFactor;
		fTotalVolume   += fVolume   * fDistanceFactor;
		fTotalLisFilter+= fFilter   * fDistanceFactor;
		fTotalNonVol   += fDistanceFactor * fVolumetricBias;
		fTotalStrength += fDistanceFactor;
	}}

	// calculate final relative position, speed and volume (weightened avarage)
	if( fTotalStrength>0.001f) {
		fTotalStrength = 1.0f / fTotalStrength;
		vRelPosition = vTotalPosition *fTotalStrength;
		vRelSpeed    = vTotalSpeed    *fTotalStrength;
		fRelVolume   = ClampUp( fTotalVolume*fTotalStrength, 1.0f);
		fRelNonVol   = fTotalNonVol *fTotalStrength;
		fRelDistance = fTotalDistance  *fTotalStrength;
		fRelFilter   = fTotalLisFilter *fTotalStrength;
	}

	// done here if not using software mixer or decoder
	if( (_pSound->sl_ulFlags&SLF_USINGDS3D) && so.so_psdcDecoder==NULL) return;

	// find volumetric usage, doppler effect pitch shift
	const FLOAT fNonVolumetricMask = bVolumetric ? 0.0f : 1.0f;
	const FLOAT fDistance = vRelPosition.Length();
	const FLOAT3D vRelDirection = vRelPosition / fDistance;
	const FLOAT fRelSpeed = vRelSpeed % vRelDirection; // negative towards listener
	FLOAT fPitchShift = so.so_fPitch;
	if( Abs(fRelSpeed)>0.001f) {
		fPitchShift *= 300 / (300+fRelSpeed*fNonVolumetricMask);  // 300 = doppler sound speed
	}

	// find distances from left and right ear
	const FLOAT fLDistance  = (FLOAT3D(-0.5f*fNonVolumetricMask/2,0,0)-vRelPosition).Length();  // 0.5 = ears distance (big head!:)
	const FLOAT fRDistance  = (FLOAT3D(+0.5f*fNonVolumetricMask/2,0,0)-vRelPosition).Length();
	const FLOAT fPhaseShift = (fLDistance-fRDistance)/1000.0f;

	// calculate relative sound directions
	FLOAT fLRFactor = 0;  // positive right
	FLOAT fFBFactor = 0;  // positive front
	FLOAT fUDFactor = 0;  // positive up
	if( fDistance>0.01f) {
		fLRFactor = +vRelDirection(1);  ASSERT( fLRFactor>=-1.1 && fLRFactor<=+1.1);
		fUDFactor = +vRelDirection(2);  ASSERT( fUDFactor>=-1.1 && fUDFactor<=+1.1);
		fFBFactor = -vRelDirection(3);  ASSERT( fFBFactor>=-1.1 && fFBFactor<=+1.1);
	}

	// calculate panning influence factor
	const FLOAT fPanningFactor = fRelNonVol * fNonVolumetricMask * 0.9f;   // 0.9 = panning strength
	ASSERT( fPanningFactor>=0 && fPanningFactor<=+1);

	// calc volume for left and right channel
	FLOAT fLVolume, fRVolume;
	if( fLRFactor > 0) {
		fLVolume = (1-fLRFactor*fPanningFactor) * fRelVolume;
		fRVolume = fRelVolume;
	} else {
		fLVolume = fRelVolume;
		fRVolume = (1+fLRFactor*fPanningFactor) * fRelVolume;
	}

	// calculate filters
	FLOAT fLFilter, fRFilter;
	fLFilter = fRFilter = 1+ fRelFilter;
	// left-right
	if( fLRFactor>0) {
		fLFilter += 2 *fLRFactor *fNonVolumetricMask;   // 2 = left-right filter
	} else {
		fRFilter -= 2 *fLRFactor *fNonVolumetricMask;
	} // front-back
	if( fFBFactor<0) {
		fLFilter -= 3 *fFBFactor *fNonVolumetricMask;   // 3 = back filter
		fRFilter -= 3 *fFBFactor *fNonVolumetricMask;
	} // up-down
	if( fUDFactor>0) {
		fLFilter += 1 *fUDFactor *fNonVolumetricMask;   // 1 = up filter
		fRFilter += 1 *fUDFactor *fNonVolumetricMask;
	} else {
		fLFilter -= 5 *fUDFactor *fNonVolumetricMask;   // 5 = down filter
		fRFilter -= 5 *fUDFactor *fNonVolumetricMask;
	}

	// set sound parameters
	so.so_fLeftVolume   = Clamp( fLVolume, 0.0f, 1.0f);
	so.so_fRightVolume  = Clamp( fRVolume, 0.0f, 1.0f);
	so.so_slLeftFilter  = FloatToInt( 32767.0 / ClampDn(fLFilter,1.0f));
	so.so_slRightFilter = FloatToInt( 32767.0 / ClampDn(fRFilter,1.0f));
	so.so_fPitchShift   = ClampDn( fPitchShift, 0.01f);
	so.so_fPhaseShift   = Clamp( fPhaseShift, -1.0f, +1.0f);
}


// prepare sound for playing
void CSoundObject::Update(void)
{
	// determine master sound volume
	extern BOOL _bMuteVolume;
	extern BOOL _bAttenuateVolume;
	FLOAT fMasterVolume = 1;  // default
	if( _bMuteVolume) {
		// muted
		fMasterVolume = 0;
	} else if( _bAttenuateVolume && !(so_ulFlags&SOF_VOICE)) {
		// attenuated
		fMasterVolume = 0.5f;
	}

	// apply cvars
	const BOOL bMusic = (so_ulFlags & SOF_MUSIC);
	if( bMusic) fMasterVolume *= snd_fMusicVolume/2;
	else        fMasterVolume *= snd_fSoundVolume;
	ASSERT( snd_fSoundVolume>=0 && snd_fSoundVolume<=1);
	ASSERT( snd_fMusicVolume>=0 && snd_fMusicVolume<=1);

	// init flags
	const BOOL bNotPlaying  = (so_tmPlayBegin==0); // is this 1st update after play?
	const BOOL bSoundPaused = (so_ulFlags&SOF_PAUSED);
	const BOOL bGamePaused  = _pNetwork==NULL || _pNetwork->IsPaused()
												|| (_pNetwork->IsServer() && _pNetwork->GetLocalPause());

	// done here if sound is paused or no sound data
	if( bNotPlaying) ResetTimer(0);
	if( bSoundPaused || so_psdLink==NULL) return;

	// signal stop playing if finished more than a second ago - encoded sounds excluded!
	// (this will remove sound from pending list)
	const TIME tmPlaying = GetElapsedTime(0);
	if( !(so_psdcDecoder!=NULL) && !(so_ulFlags&SOF_LOOP) && tmPlaying>(so_psdLink->sd_tmLength+1)) {
		ASSERT( so_ulFlags & SOF_PLAYING);
		so_ulFlags &= ~SOF_PLAYING;
		return;
	}

	// if fading is set
	FLOAT fFadeLerp = 0;
	if( so_tmFadeTime>0) {
		// calculate fade ratio
		const TIME tmFading = GetElapsedTime(2);
		if( tmFading<so_tmFadeTime) {
			fFadeLerp = tmFading/so_tmFadeTime;
		} // if fading is done and end volume is silent
		else if( so_fFadeVolume==0) {
			// signal stop playing sound
			so_ulFlags &= ~SOF_PLAYING;
			so_tmFadeTime = 0;
			return;
		} // otherwise set fade complete lerp
		else {
			fFadeLerp = 1;
		}
	}

	// update 3D effects and volume
	FLOAT3D vSoundPos(  0,0,0);
	FLOAT3D vSoundSpeed(0,0,0);
	FLOAT fSoundDistance = 10000;
	FLOAT fSoundVolume = 0;
	FLOAT fSoundFilter = 0;
	const BOOL bIs3D = (so_ulFlags&SOF_3D);
	// for 3D sounds
	if( bIs3D) {
		// update 3D parameters
		Update3D( *this, vSoundPos, vSoundSpeed, fSoundVolume, fSoundDistance, fSoundFilter);
		fSoundVolume = fMasterVolume * Lerp( fSoundVolume, so_fFadeVolume, fFadeLerp);
		// for SW mixer and/or encoded sounds (music)
		so_fLeftVolume  = fMasterVolume * Lerp( so_fLeftVolume,  so_fFadeVolume, fFadeLerp);
		so_fRightVolume = fMasterVolume * Lerp( so_fRightVolume, so_fFadeVolume, fFadeLerp);
	}
	// for 2D sounds
	else {
		// update strait from 'input' parameters
		fSoundDistance = 0;
		fSoundVolume = fMasterVolume * Lerp( so_fVolume, so_fFadeVolume, fFadeLerp);
		// set relevant parameters for SW mixer right here
		so_fLeftVolume = so_fRightVolume = fSoundVolume;
		so_fPitchShift = so_fPitch;
	}

	// if not playing yet
	if( bNotPlaying) {
		// force initial volume set
		so_fLastLeftVolume  = so_fLeftVolume;
		so_fLastRightVolume = so_fRightVolume;
	}

	// check if this sound can be played regarding the "sound mass"
	// (number of times the same sound data is playing; 5 sounds reasonable)
	extern INDEX snd_iMaxSameSounds;
	snd_iMaxSameSounds = Clamp( snd_iMaxSameSounds, 1L, 99L);
	if( (so_psdLink->sd_fSoundMass+fSoundVolume) > snd_iMaxSameSounds) {
		// sound mass is too heavy - force volume of this sound to silent (temporary)
		fSoundVolume = 0;
	} else {
		// playable - just accumulate sound mass
		so_psdLink->sd_fSoundMass += fSoundVolume;
	}

	// keep parameters for latter HW setup
	ASSERT( fSoundVolume>=0 && fSoundVolume<=1);
	ASSERT( fSoundFilter>=0);
	so_vCurrentPosition = vSoundPos;
	so_vCurrentSpeed    = vSoundSpeed;
	so_fCurrentDistance = fSoundDistance;
	so_fCurrentVolume   = fSoundVolume;
	so_fCurrentFilter   = fSoundFilter;
}


// Obtain sound and play it for this object
void CSoundObject::Play_t( const CTFileName &fnmSound, ULONG ulFlags) // throw char *
{
	// obtain it (adds one reference)
	CSoundData *psd = _pSoundStock->Obtain_t(fnmSound);
	// set it as data (adds one more reference, and remove old reference)
	Play( psd, ulFlags);
	// release it (removes one reference)
	_pSoundStock->Release(psd);
	// total reference count +1+1-1 = +1 for new data -1 for old data
}


// stop playing sound but keep it linked to data
void CSoundObject::Pause(void)
{
	// do nothing if already paused
	if( so_ulFlags&SOF_PAUSED) {
		// must not be in HW buffer
		ASSERT( !(_pSound->sl_ulFlags&SLF_USINGDS3D) || so_iPlayingBuffer<0);
		return;
	}
	// mark
	so_ulFlags |= SOF_PAUSED;  

	// if HW playing buffer is used 
	if( (_pSound->sl_ulFlags&SLF_USINGDS3D) && so_iPlayingBuffer>=0) {
		// release it (must do that in case we're reseting sound library)
		_pSound->ReleasePlayingBuffer( so_iPlayingBuffer, TRUE);
		so_iPlayingBuffer = -1;
	}
}


// resume playing paused sound
void CSoundObject::Resume(void)
{
	// just unmark pause
	// (eventual HW playing buffer will be reclaimed at 'Update');
	so_ulFlags &= ~SOF_PAUSED;
}



// read/write functions
void CSoundObject::Read_t(CTStream *pistr, BOOL bNetwork/*=FALSE*/)  // throw char *
{
	// load file name
	CTFileName fnmSound;
	*pistr >> fnmSound;
	so_fFadeVolume = 1;
	so_tmFadeTime  = 0;
	so_fCurrentFilter = 0;
	so_fCurrentVolume = 0;
	so_fCurrentDistance = 10000;
	so_fLastLeftVolume  = 0;
	so_fLastRightVolume = 0;
	so_penEntity = NULL;

	ULONG ulDummy;
	if( !bNetwork) {
		// load object preferences
		*pistr >> ulDummy;
		*pistr >> so_ulFlags;

		*pistr >> so_fLeftVolume;
		*pistr >> so_fRightVolume;
		*pistr >> so_slLeftFilter;
		*pistr >> so_slRightFilter;
		*pistr >> so_fPitchShift;
		*pistr >> so_fPhaseShift;
		*pistr >> ulDummy;

		*pistr >> ulDummy;
		*pistr >> ulDummy;
		*pistr >> ulDummy;
		*pistr >> so_swLastLeftSample;
		*pistr >> so_swLastRightSample;
		*pistr >> so_fLeftOffset;
		*pistr >> so_fRightOffset;
		*pistr >> so_fOffsetDelta;

		*pistr >> so_fFallOff;
		*pistr >> so_fHotSpot;
		*pistr >> so_fVolume;
		*pistr >> so_fPitch;
	}
	else {
		UBYTE ubComp;
		*pistr >> so_ulFlags;

		*pistr >> ubComp;  so_fLeftVolume  = ((FLOAT)ubComp)/127; 
		*pistr >> ubComp;  so_fRightVolume = ((FLOAT)ubComp)/127; 
		*pistr >> so_slLeftFilter;
		*pistr >> so_slRightFilter;
		*pistr >> ubComp;  so_fPitchShift = ((FLOAT)ubComp)/63; 
		*pistr >> ubComp;  so_fPhaseShift = ((FLOAT)ubComp)/63; 
		*pistr >> ulDummy;

		*pistr >> ulDummy;
		*pistr >> ubComp;  so_fLastLeftVolume  = 0; 
		*pistr >> ubComp;  so_fLastRightVolume = 0; 
		*pistr >> so_swLastLeftSample;
		*pistr >> so_swLastRightSample;
		*pistr >> so_fLeftOffset;
		*pistr >> so_fRightOffset;
		*pistr >> so_fOffsetDelta;

		UWORD uwFallOff,uwHotspot;
		// save 3D parameters
		*pistr >> uwFallOff;  so_fFallOff = ((FLOAT)uwFallOff)/5;
		*pistr >> uwHotspot;  so_fHotSpot = ((FLOAT)uwHotspot)/5;
		*pistr >> ubComp;     so_fVolume  = ((FLOAT)ubComp)/127; 
		*pistr >> ubComp;     so_fPitch   = ((FLOAT)ubComp)/63; 
	}

	// Obtain and play object (sound)
	if( fnmSound!="" && (so_ulFlags&SOF_PLAYING)) {
		Play_t( fnmSound, so_ulFlags|SOF_LOADED);
	}
}


void CSoundObject::Write_t(CTStream *pistr, BOOL bNetwork/*=FALSE*/) // throw char *
{
	// save file name
	if( so_psdLink!=NULL) {
		*pistr << (so_psdLink->GetName());
	} else {
		*pistr << CTFILENAME("");
	}

	if( !bNetwork) {
		// save object preferences
		*pistr << 0L;
		*pistr << so_ulFlags;

		*pistr << so_fLeftVolume;
		*pistr << so_fRightVolume;
		*pistr << so_slLeftFilter;
		*pistr << so_slRightFilter;
		*pistr << so_fPitchShift;
		*pistr << so_fPhaseShift;
		*pistr << 0;

		*pistr << 0;
		*pistr << 0;
		*pistr << 0;
		*pistr << so_swLastLeftSample;
		*pistr << so_swLastRightSample;
		*pistr << so_fLeftOffset;
		*pistr << so_fRightOffset;
		*pistr << so_fOffsetDelta;

		*pistr << so_fFallOff;
		*pistr << so_fHotSpot;
		*pistr << so_fVolume;
		*pistr << so_fPitch;
	}
	else {
		ASSERT( so_fLeftVolume<=2 && so_fRightVolume<2);
		*pistr << so_ulFlags;

		*pistr << (UBYTE)(so_fLeftVolume *127); // volume is 0-4, this is packs it to UBYTE range 0-249 with 1/63 precission
		*pistr << (UBYTE)(so_fRightVolume*127);
		*pistr << so_slLeftFilter;
		*pistr << so_slRightFilter;
		*pistr << (UBYTE)(so_fPitchShift*63);
		*pistr << (UBYTE)(so_fPhaseShift*63);
		*pistr << 0L;

		*pistr << 0L;
		*pistr << (UBYTE)0;
		*pistr << (UBYTE)0;
		*pistr << so_swLastLeftSample;
		*pistr << so_swLastRightSample;
		*pistr << so_fLeftOffset;
		*pistr << so_fRightOffset;
		*pistr << so_fOffsetDelta;

		FLOAT fFallOff = Clamp(so_fFallOff*5, 0.0f, 65535.0f);
		FLOAT fHotspot = Clamp(so_fHotSpot*5, 0.0f, 65535.0f);
		// save 3D parameters
		*pistr << (UWORD)fFallOff;
		*pistr << (UWORD)fHotspot;
		*pistr << (UBYTE)(so_fVolume*127);
		*pistr << (UBYTE)(so_fPitch *63);
	}
}



// timer functions (iTimer: 0=start playing, 1=start silence, 2=fadeout)
void CSoundObject::ResetTimer( const INDEX iTimer)
{
	DOUBLE *ptm;
	ASSERT( iTimer>=0 && iTimer<=2);
	switch( iTimer) {
	case 1:  ptm = &so_tmNotAudible; break;
	case 2:  ptm = &so_tmFadeBegin;  break;
	default: ptm = &so_tmPlayBegin;  break;
	}

	if( so_ulFlags&SOF_NONGAME) {
		*ptm = _pTimer->GetHighPrecisionTimer().GetSeconds();
	} else {
		*ptm = _pTimer->GetLerpedCurrentTick();
	}
}


TIME CSoundObject::GetElapsedTime( const INDEX iTimer)
{
	DOUBLE *ptm;
	ASSERT( iTimer>=0 && iTimer<=2);
	switch( iTimer) {
	case 1:  ptm = &so_tmNotAudible; break;
	case 2:  ptm = &so_tmFadeBegin;  break;
	default: ptm = &so_tmPlayBegin;  break;
	}

	DOUBLE tmNow;
	if( so_ulFlags&SOF_NONGAME) {
		tmNow = _pTimer->GetHighPrecisionTimer().GetSeconds();
	} else {
		tmNow = _pTimer->GetLerpedCurrentTick();
	}
	return (tmNow - *ptm);
}
