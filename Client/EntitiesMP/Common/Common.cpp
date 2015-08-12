#include "StdH.h"
#include "EntitiesMP/Reminder.h"
#include "EntitiesMP/Flame.h"
#include "EntitiesMP/Debris.h"
#include "EntitiesMP/DebrisSka.h"
#include "EntitiesMP/Player.h"
#include "EntitiesMP/Bullet.h"
#include "EntitiesMP/BackgroundViewer.h"
#include "EntitiesMP/SoundHolder.h"
#include "EntitiesMP/MusicHolder.h"
#include "GameMP/PlayerSettings.h"
#include <Engine/Ska/SkaAssert.h>
#include "EntitiesMP/EnemyBase.h"
#include "EntitiesMP/PetBase.h"
#include "EntitiesMP/SlaveBase.h"
#include "EntitiesMP/CharacterBase.h"
#include "EntitiesMP/WildPetBase.h"
#include "EntitiesMP/EnemySpawner.h"
//#include "ModelsMP/Player/SeriousSam/Player.h"
//#include "ModelsMP/Player/SeriousSam/Body.h"
//#include "ModelsMP/Player/SeriousSam/Head.h"
//강동민 수정 시작 로그인 처리 작업	07.07
#include <Engine/GlobalDefinition.h>
#include <Engine/JobInfo.h>
//강동민 수정 끝 로그인 처리 작업		07.07
extern INDEX ent_bReportBrokenChains;

void CCompMessageID::Clear(void)
{
	cmi_fnmFileName.Clear();
	cmi_ulHash = 0;
}

void CCompMessageID::Read_t(CTStream &strm)    // throw char *
{
	strm>>cmi_fnmFileName;
	strm>>(INDEX&)cmi_cmtType;
	strm>>(INDEX&)cmi_bRead;
	cmi_ulHash = cmi_fnmFileName.GetHash();
}

void CCompMessageID::Write_t(CTStream &strm)   // throw char *
{
	strm<<cmi_fnmFileName;
	strm<<(INDEX&)cmi_cmtType;
	strm<<(INDEX&)cmi_bRead;
}

void CCompMessageID::NewMessage(const CTFileName &fnm)
{
	// remember filename
	cmi_fnmFileName = fnm;
	cmi_ulHash = cmi_fnmFileName.GetHash();

	// decode type from filename
	CTString strName = fnm;

	if (strName.Matches("*messages\\information*")) {
		cmi_cmtType = CMT_INFORMATION;
	} else if (strName.Matches("*messages\\weapons*")) {
		cmi_cmtType = CMT_WEAPONS;
	} else if (strName.Matches("*messages\\enemies*")) {
		cmi_cmtType = CMT_ENEMIES;
	} else if (strName.Matches("*messages\\background*")) {
		cmi_cmtType = CMT_BACKGROUND;
	} else if (strName.Matches("*messages\\statistics*")) {
		cmi_cmtType = CMT_STATISTICS;
	} else {
		CPrintF("Unknown message type: %s\n", (const CTString&) fnm);
		cmi_cmtType = CMT_INFORMATION;
	}
	// mark as unread
	cmi_bRead = FALSE;
}

/************************************************************
 *          COMMON FUNCTIONS FOR ENTITY CLASSES             *
 ************************************************************/
// world change
struct WorldChange _SwcWorldChange;

// get info position for entity
void GetEntityInfoPosition(CEntity *pen, FLOAT *pf, FLOAT3D &vPos) {
	ASSERT(pen!=NULL);

	vPos = pen->GetLerpedPlacement().pl_PositionVector;
	if (pf != NULL) {
		FLOATmatrix3D mRotation;
		MakeRotationMatrixFast(mRotation, pen->GetLerpedPlacement().pl_OrientationAngle);
		vPos += FLOAT3D(pf[0], pf[1], pf[2])*mRotation;
	}
};

// get source and target positions for ray cast
void GetPositionCastRay(CEntity *penSource, CEntity *penTarget, FLOAT3D &vSource, FLOAT3D &vTarget) {
	EntityInfo *peiSource = (EntityInfo*) (penSource->GetEntityInfo());
	EntityInfo *peiTarget = (EntityInfo*) (penTarget->GetEntityInfo());

//	ASSERT(peiSource!=NULL && peiTarget!=NULL);

	// source
	if (peiSource!=NULL) {
		GetEntityInfoPosition(penSource, peiSource->vSourceCenter, vSource);
	} else {
		vSource = penSource->GetPlacement().pl_PositionVector;
	}
	// target
	if (peiTarget!=NULL) {
		GetEntityInfoPosition(penTarget, peiTarget->vTargetCenter, vTarget);
	} else {
		vTarget = penTarget->GetPlacement().pl_PositionVector;
	}
};

// set bool from bool enum type
void SetBoolFromBoolEType(BOOL &bSet, BoolEType bet) {
	switch (bet) {
		case BET_TRUE:
			bSet = TRUE;
			break;
		case BET_FALSE:
			bSet = FALSE;
			break;
		//case BET_IGNORE:
			//bSet = bSet;
			// break
	}
};

// send event to target
void SendToTarget(CEntity *penSendEvent, EventEType eetEventType, CEntity *penCaused, CEntityEvent* peeEvent) {
	// if target is valid
	if (penSendEvent != NULL) {
		switch (eetEventType) {
			// send START event
			case EET_START: {
				EStart eStart;
				eStart.penCaused = penCaused;
				penSendEvent->SendEvent(eStart);
											} break;
			// send STOP event
			case EET_STOP:
				penSendEvent->SendEvent(EStop());
				break;
			// send TRIGGER event
			case EET_TRIGGER: {
				ETrigger eTrigger;
				eTrigger.penCaused = penCaused;
				penSendEvent->SendEvent(eTrigger);
												} break;
			// don't send event (IGNORE)
			case EET_IGNORE:
				break;
			// send ACTIVATE event
			case EET_ACTIVATE:
				penSendEvent->SendEvent(EActivate());
				break;
			// send DEACTIVATE event
			case EET_DEACTIVATE:
				penSendEvent->SendEvent(EDeactivate());
				break;
			// send ENVIRONMENTSTART event
			case EET_ENVIRONMENTSTART:
				penSendEvent->SendEvent(EEnvironmentStart());
				break;
			// send ENVIRONMENTSTOP event
			case EET_ENVIRONMENTSTOP:
				penSendEvent->SendEvent(EEnvironmentStop());
				break;
			// send STARTATTACK event
			case EET_STARTATTACK:
				penSendEvent->SendEvent(EStartAttack());
				break;
			// send STOPATTACK event
			case EET_STOPATTACK:
				penSendEvent->SendEvent(EStopAttack());
				break;
			case EET_STOPBLINDNESS:
				penSendEvent->SendEvent(EStopBlindness());
				break;
			case EET_STOPDEAFNESS:
				penSendEvent->SendEvent(EStopDeafness());
				break;
			case EET_TELEPORTMOVINGBRUSH:
				penSendEvent->SendEvent(ETeleportMovingBrush());
				break;
			case EET_ADDHOSTILE:
				penSendEvent->SendEvent(EAddHostile());
				break;
			case EET_DOORCONTROLL:
				{
					if (peeEvent)
					{
						penSendEvent->SendEvent((*peeEvent));
					}
				}
				break;
		}
	}
};

// send event in range
void SendInRange(CEntity *penSource, EventEType eetEventType, const FLOATaabbox3D &boxRange) {
	switch (eetEventType) {
		// send START event
		case EET_START:
			penSource->SendEventInRange(EStart(), boxRange);
			break;
		// send STOP event
		case EET_STOP:
			penSource->SendEventInRange(EStop(), boxRange);
			break;
		// send TRIGGER event
		case EET_TRIGGER:
			penSource->SendEventInRange(ETrigger(), boxRange);
			break;
		// don't send event (IGNORE)
		case EET_IGNORE:
			break;
		// send ACTIVATE event
		case EET_ACTIVATE:
			penSource->SendEventInRange(EActivate(), boxRange);
			break;
		// send DEACTIVATE event
		case EET_DEACTIVATE:
			penSource->SendEventInRange(EDeactivate(), boxRange);
			break;
		// send ENVIRONMENTSTART event
		case EET_ENVIRONMENTSTART:
			penSource->SendEventInRange(EEnvironmentStart(), boxRange);
			break;
		// send ENVIRONMENTSTOP event
		case EET_ENVIRONMENTSTOP:
			penSource->SendEventInRange(EEnvironmentStop(), boxRange);
			break;
		// send STARTATTACK event
		case EET_STARTATTACK:
			penSource->SendEventInRange(EStartAttack(), boxRange);
			break;
		// send STOPATTACK event
		case EET_STOPATTACK:
			penSource->SendEventInRange(EStopAttack(), boxRange);
			break;
		case EET_STOPBLINDNESS:
			penSource->SendEventInRange(EStopBlindness(), boxRange);
			break;
		case EET_STOPDEAFNESS:
			penSource->SendEventInRange(EStopDeafness(), boxRange);
			break;
	}
};

// spawn reminder
CEntityPointer SpawnReminder(CEntity *penOwner, FLOAT fWaitTime, INDEX iValue) {
	CEntityPointer penReminder;
	try {
		penReminder = penOwner->GetWorld()->CreateEntity_t
			(penOwner->GetPlacement(), CTFILENAME("Classes\\Reminder.ecl"));
	} catch (char *strError) {
		FatalError(TRANS("Cannot create reminder entity class: %s"), strError);
	}
	EReminderInit eri;
	eri.eidOwner = penOwner;
	eri.fWaitTime = fWaitTime;
	eri.iValue = iValue;
	penReminder->Initialize(eri);

	return penReminder;
};

EffectParticlesType GetParticleEffectTypeForSurface(INDEX iSurfaceType)
{
	EffectParticlesType eptType = EPT_BULLET_STONE;
	/*
	switch( iSurfaceType)
	{
		case SURFACE_SAND:     {eptType=EPT_BULLET_SAND; break;}
		case SURFACE_RED_SAND: {eptType=EPT_BULLET_RED_SAND; break;}
		case SURFACE_WATER:    {eptType=EPT_BULLET_WATER; break;}
		case SURFACE_GRASS:    
		case SURFACE_GRASS_SLIDING:
		case SURFACE_GRASS_NOIMPACT:
			{eptType=EPT_BULLET_GRASS; break;}
		case SURFACE_WOOD:     {eptType=EPT_BULLET_WOOD; break;}
		case SURFACE_SNOW:     {eptType=EPT_BULLET_SNOW; break;}
	}
	*/
	return eptType;
}

BulletHitType GetBulletHitTypeForSurface(INDEX iSurfaceType)
{
	BulletHitType bhtType = BHT_BRUSH_STONE;
	/*
	switch( iSurfaceType)
	{
		case SURFACE_SAND:     {bhtType=BHT_BRUSH_SAND; break;}
		case SURFACE_RED_SAND: {bhtType=BHT_BRUSH_RED_SAND; break;}
		case SURFACE_WATER:    {bhtType=BHT_BRUSH_WATER; break;}
		case SURFACE_GRASS:
		case SURFACE_GRASS_SLIDING:
		case SURFACE_GRASS_NOIMPACT:
			{bhtType=BHT_BRUSH_GRASS; break;}
		case SURFACE_WOOD:     {bhtType=BHT_BRUSH_WOOD; break;}
		case SURFACE_SNOW:     {bhtType=BHT_BRUSH_SNOW; break;}
	}
	*/
	return bhtType;
}

// spawn effect from hit type
void SpawnHitTypeEffect(CEntity *pen, enum BulletHitType bhtType, BOOL bSound, FLOAT3D vHitNormal, FLOAT3D vHitPoint,
	FLOAT3D vIncommingBulletDir, FLOAT3D vDistance)
{
	switch (bhtType)
	{
		case BHT_BRUSH_STONE:
		case BHT_BRUSH_SAND:
		case BHT_BRUSH_RED_SAND:
		case BHT_BRUSH_WATER:
		case BHT_BRUSH_UNDER_WATER:
		case BHT_BRUSH_GRASS:
		case BHT_BRUSH_WOOD:
		case BHT_BRUSH_SNOW:
		{
			// bullet stain
			ESpawnEffect ese;
			if( bSound)
			{
				if( bhtType == BHT_BRUSH_STONE)         {ese.betType = BET_BULLETSTAINSTONE;};
				if( bhtType == BHT_BRUSH_SAND)          {ese.betType = BET_BULLETSTAINSAND;};
				if( bhtType == BHT_BRUSH_RED_SAND)      {ese.betType = BET_BULLETSTAINREDSAND;};
				if( bhtType == BHT_BRUSH_WATER)         {ese.betType = BET_BULLETSTAINWATER;};
				if( bhtType == BHT_BRUSH_UNDER_WATER)   {ese.betType = BET_BULLETSTAINUNDERWATER;};
				if( bhtType == BHT_BRUSH_GRASS)         {ese.betType = BET_BULLETSTAINGRASS;};
				if( bhtType == BHT_BRUSH_WOOD)          {ese.betType = BET_BULLETSTAINWOOD;};
				if( bhtType == BHT_BRUSH_SNOW)          {ese.betType = BET_BULLETSTAINSNOW;};
			}
			else
			{
				if( bhtType == BHT_BRUSH_STONE)         {ese.betType = BET_BULLETSTAINSTONENOSOUND;};
				if( bhtType == BHT_BRUSH_SAND)          {ese.betType = BET_BULLETSTAINSANDNOSOUND;};
				if( bhtType == BHT_BRUSH_RED_SAND)      {ese.betType = BET_BULLETSTAINREDSANDNOSOUND;};
				if( bhtType == BHT_BRUSH_WATER)         {ese.betType = BET_BULLETSTAINWATERNOSOUND;};
				if( bhtType == BHT_BRUSH_UNDER_WATER)   {ese.betType = BET_BULLETSTAINUNDERWATERNOSOUND;};
				if( bhtType == BHT_BRUSH_GRASS)         {ese.betType = BET_BULLETSTAINGRASSNOSOUND;};
				if( bhtType == BHT_BRUSH_WOOD)          {ese.betType = BET_BULLETSTAINWOODNOSOUND;};
				if( bhtType == BHT_BRUSH_SNOW)          {ese.betType = BET_BULLETSTAINSNOWNOSOUND;};
			}

			ese.vNormal = vHitNormal;
			ese.colMuliplier = C_WHITE|CT_OPAQUE;
			// reflect direction arround normal
			FLOAT fNx = vHitNormal(1);
			FLOAT fNy = vHitNormal(2);
			FLOAT fNz = vHitNormal(3);
			FLOAT fNV  = fNx*vIncommingBulletDir(1) + fNy*vIncommingBulletDir(2) + fNz*vIncommingBulletDir(3);
			FLOAT fRVx = vIncommingBulletDir(1) - 2*fNx*fNV;
			FLOAT fRVy = vIncommingBulletDir(2) - 2*fNy*fNV;
			FLOAT fRVz = vIncommingBulletDir(3) - 2*fNz*fNV;
			ese.vStretch = FLOAT3D( fRVx, fRVy, fRVz);

			
			try {
				// spawn effect
				CPlacement3D plHit = CPlacement3D( vHitPoint+ese.vNormal/50.0f*(pen->FRnd()+0.5f),
																					 pen->GetPlacement().pl_OrientationAngle);
				CEntityPointer penHit = pen->GetWorld()->CreateEntity_t( plHit, CTFILENAME("Classes\\BasicEffect.ecl"));
				penHit->Initialize(ese);
			}
			catch (char *strError) {

				FatalError(TRANS("Cannot create basic effect class: %s"), strError);
			}

			break;
		}
		case BHT_FLESH:
		case BHT_ACID:
		{
			// spawn bullet entry wound
			ESpawnEffect ese;
			ese.colMuliplier = C_WHITE|CT_OPAQUE;
			// if there is exit wound blood spill place
			FLOAT fDistance = vDistance.Length();
			if( fDistance>0.01f && !(pen->IRnd()%2) )
			{
				// spawn bullet exit wound blood patch
				ese.betType = BET_BLOODSPILL;
				if( bhtType == BHT_ACID)
				{
					ese.colMuliplier = BLOOD_SPILL_GREEN;
				}
				else
				{
					ese.colMuliplier = BLOOD_SPILL_RED;
				}
				ese.vNormal = vHitNormal;
				if (fDistance<25.0f)
				{
					GetNormalComponent( vDistance/fDistance, vHitNormal, ese.vDirection);
					FLOAT fLength = ese.vDirection.Length();
					fLength   = Clamp( fLength*3, 1.0f, 3.0f);
					fDistance = Clamp<FLOAT>( (FLOAT)log10(fDistance), 0.5f, 2.0f);
					ese.vStretch = FLOAT3D( fDistance, fLength*fDistance, 1.0f);
					try {

						// spawn effect
						CPlacement3D plHit = CPlacement3D( vHitPoint+ese.vNormal/50.0f*(pen->FRnd()+0.5f),
																							 pen->GetPlacement().pl_OrientationAngle);
						CEntityPointer penHit = pen->GetWorld()->CreateEntity_t( plHit, CTFILENAME("Classes\\BasicEffect.ecl"));
						penHit->Initialize(ese);
					}
					catch (char *strError) {

						FatalError(TRANS("Cannot create basic effect class: %s"), strError);
					}
				}
			}
			break;
		}
	}
}

// spawn flame
CEntityPointer SpawnFlame(CEntity *penOwner, CEntity *penAttach, const FLOAT3D &vSource)
{
	// owner can't flame himself
	if( penOwner==penAttach) return NULL;
	FLOAT3D vPos = vSource;
	// prepare flame event
	EFlame ef;
	ef.eidOwner = penOwner;
	ef.eidAttach = penAttach;

	CEntityPointer penFlame;

	// if the target entity is model
	if (penAttach->GetRenderType()==CEntity::RT_MODEL || 
			penAttach->GetRenderType()==CEntity::RT_SKAMODEL) {

		vPos = penAttach->GetPlacement().pl_PositionVector;
		// if the entity already has a flame attached
		penFlame = penAttach->GetChildOfClass("Flame");
		if (penFlame!=NULL) {
			// just send it the event
			penFlame->SendEvent(ef);
			return penFlame;
		}
	}

	CPlacement3D plFlame(vPos, ANGLE3D(0, 0, 0));
	// create new flame
	try {    
		penFlame = penAttach->GetWorld()->CreateEntity_t(plFlame, CTFILENAME("Classes\\Flame.ecl"),WLD_AUTO_ENTITY_ID,FALSE);
	} catch (char *strError) {
		FatalError(TRANS("Cannot create flame entity class: %s"), strError);
	}
	penFlame->Initialize(ef,FALSE);

	if (!_pNetwork->IsServer()) {
		// since it's created on the client, set it's dead reckoning parameters
		CPlacement3D plSpeed;  
		plSpeed.pl_OrientationAngle = ANGLE3D(0,0,0);
		plSpeed.pl_PositionVector   = FLOAT3D(0,0,0);
		((CMovableEntity*)((CEntity*)penFlame))->AdjustDeadReckoning(plFlame,plSpeed,_pTimer->CurrentTick());
	}

	return penFlame;
};

// Kick entity
void KickEntity(CEntity *penTarget, FLOAT3D vSpeed) {
	EntityInfo *peiTarget = (EntityInfo*) (penTarget->GetEntityInfo());
	if (penTarget->GetPhysicsFlags()&EPF_MOVABLE && peiTarget!=NULL) {
		// calc new speed acording to target mass
		vSpeed *= 100.0f/peiTarget->fMass;
		((CMovableEntity&)*penTarget).en_vCurrentTranslationAbsolute = vSpeed;
		((CMovableEntity&)*penTarget).AddToMovers();
	}
};



/************************************************************
 *                   SET MODEL AND ATTACHMENT               *
 ************************************************************/
	// Set components
	void SetComponents(CEntity *pen, CModelObject &mo, ULONG ulIDModel, ULONG ulIDTexture,
										 ULONG ulIDReflectionTexture, ULONG ulIDSpecularTexture, ULONG ulIDBumpTexture) {
		// model data
		mo.SetData(pen->GetModelDataForComponent(ulIDModel));
		// texture data
		mo.mo_toTexture.SetData(pen->GetTextureDataForComponent(ulIDTexture));
		// reflection texture data
		if (ulIDReflectionTexture>0) {
			mo.mo_toReflection.SetData(pen->GetTextureDataForComponent(ulIDReflectionTexture));
		} else {
			mo.mo_toReflection.SetData(NULL);
		}
		// specular texture data
		if (ulIDSpecularTexture>0) {
			mo.mo_toSpecular.SetData(pen->GetTextureDataForComponent(ulIDSpecularTexture));
		} else {
			mo.mo_toSpecular.SetData(NULL);
		}
		// bump texture data
		if (ulIDBumpTexture>0) {
			mo.mo_toBump.SetData(pen->GetTextureDataForComponent(ulIDBumpTexture));
		} else {
			mo.mo_toBump.SetData(NULL);
		}
	};

	// Add attachment to model
	void AddAttachmentToModel(CEntity *pen, CModelObject &mo, INDEX iAttachment, ULONG ulIDModel, ULONG ulIDTexture,
														ULONG ulIDReflectionTexture, ULONG ulIDSpecularTexture, ULONG ulIDBumpTexture) {
		SetComponents(pen, mo.AddAttachmentModel(iAttachment)->amo_moModelObject, ulIDModel,
									ulIDTexture, ulIDReflectionTexture, ulIDSpecularTexture, ulIDBumpTexture);
	};

	// Remove attachment from model
	void RemoveAttachmentFromModel(CModelObject &mo, INDEX iAttachment) {
		mo.RemoveAttachmentModel(iAttachment);
	};


/************************************************************
 *                       SET SKA MODEL                      *
 ************************************************************/
void LoadSkaModel(CModelInstance &m_miModel, CEntity *penThis, INDEX iSkaModelComponentID)
{
	try {
		CModelInstance *pmiComponent = penThis->GetModelInstanceForComponent(iSkaModelComponentID);
		ASSERT(pmiComponent!=NULL);
		m_miModel.Copy(*pmiComponent);

		/*
		CModelInstance *pmiTmp = NULL;
		pmiTmp = ObtainModelInstance_t(strSMCFile);
		// if loaded successfuly
		if (pmiTmp!=NULL) {
			m_miModel.Clear();
			m_miModel.Copy(*pmiTmp);
			pmiTmp->Clear();
			delete pmiTmp;
		}*/
	} catch (char *strError) {
		// report error
		const CTFileName &fnSMCFileName = penThis->FileNameForComponent(ECT_SKAMODEL,iSkaModelComponentID);
		FatalError(TRANS("Cannot load SMC model '%s' : %s\n"), (const char *)fnSMCFileName, strError);
	}
}

// starts a new anim, erasing any that are currently playing
// NOTE: be warned that this function uses animation power of 1 ALLWAYS
void StartSkaAnim(CEntity *pen, INDEX iAnimID, DWORD dwFlags, INDEX iGroupID, FLOAT fFadeTime)
{
	ASSERT(pen!=NULL && pen->GetModelInstance()!=NULL);
	pen->NewClearState(fFadeTime);
	pen->AddAnimation(iAnimID, dwFlags, 1, iGroupID);
}

void StartSkaAnimStr(CModelInstance *m_miModel, CTString strAnimName, DWORD dwFlags, INDEX iGroupID, FLOAT fFadeTime)
{
	ASSERT(m_miModel!=NULL);
	INDEX iAnimID = -1;
	iAnimID = ska_GetIDFromStringTable(strAnimName);
	if (iAnimID==-1) {    
		ASSERT(FALSE);
		CPrintF("Animation '%s' does not exist in model instance '%s'!\n",
						strAnimName, m_miModel->GetName());
		return;
	}
	m_miModel->NewClearState(fFadeTime);
	m_miModel->AddAnimation(iAnimID, dwFlags, 1, iGroupID);
}


/************************************************************
 *                          FLARES                          *
 ************************************************************/
// lens flare variables
CLensFlareType _lftStandard;
CLensFlareType _lftStandardReflections;
CLensFlareType _lftYellowStarRedRing;
CLensFlareType _lftYellowStarRedRingFar;
CLensFlareType _lftWhiteGlowStarRedRing;
CLensFlareType _lftWhiteGlowStar;
CLensFlareType _lftWhiteGlowStarNG;
CLensFlareType _lftWhiteStarRedRingStreaks;
CLensFlareType _lftWhiteStarRedReflections;
CLensFlareType _lftBlueStarBlueReflections;
CLensFlareType _lftProjectileStarGlow;
CLensFlareType _lftProjectileWhiteBubbleGlow;
CLensFlareType _lftProjectileYellowBubbleGlow;
//CLensFlareType _lftPVSpaceShipWindowFlare;
//CLensFlareType _lftCatmanFireGlow;
CLensFlareType _lftWhiteGlowFar;

#define FLARE_CREATE(flag,noof,tex,pos,rot,i,j,flags,amp,des,falloff)\
	flag.lft_aolfFlares.New(noof);\
	flag.lft_aolfFlares[0].olf_toTexture.SetData_t(CTFILENAME("Data\\Textures\\Effects\\Flares\\" tex));\
	flag.lft_aolfFlares[0].olf_fReflectionPosition = pos;\
	flag.lft_aolfFlares[0].olf_aRotationFactor = AngleDeg(rot);\
	flag.lft_aolfFlares[0].olf_fSizeIOverScreenSizeI = i;\
	flag.lft_aolfFlares[0].olf_fSizeJOverScreenSizeI = j;\
	flag.lft_aolfFlares[0].olf_ulFlags = flags;\
	flag.lft_aolfFlares[0].olf_fLightAmplification = amp;\
	flag.lft_aolfFlares[0].olf_fLightDesaturation = des;\
	flag.lft_aolfFlares[0].oft_fFallOffFactor = falloff;
#define FLARE_GLARE(flag,compression,intensity,des,falloff)\
	flag.lft_fGlareCompression = compression;\
	flag.lft_fGlareIntensity = intensity;\
	flag.lft_fGlareDesaturation = des;\
	flag.lft_fGlareFallOffFactor = falloff;
#define REFLECTION(flag,i,fnm,pos,size) \
	flag.lft_aolfFlares[i].olf_toTexture.SetData_t(CTFILENAME("Data\\Textures\\Effects\\Flares\\" fnm));\
	flag.lft_aolfFlares[i].olf_fReflectionPosition = pos;\
	flag.lft_aolfFlares[i].olf_aRotationFactor = AngleDeg(0.0f);\
	flag.lft_aolfFlares[i].olf_fSizeIOverScreenSizeI = size;\
	flag.lft_aolfFlares[i].olf_fSizeJOverScreenSizeI = size;\
	flag.lft_aolfFlares[i].olf_ulFlags = OLF_FADEINTENSITY|OLF_FADEOFCENTER;\
	flag.lft_aolfFlares[i].olf_fLightAmplification = 7.0f;\
	flag.lft_aolfFlares[i].olf_fLightDesaturation = 0.5f;\
	flag.lft_aolfFlares[i].oft_fFallOffFactor = 5.0f;

// init lens flare effects
void InitLensFlares(void) {
	// standard flare
	FLARE_CREATE(_lftStandard, 1, "01\\WhiteRedRing2.tex", 0.0f, 180.0f, 1/5.0f, 1/5.0f, OLF_FADESIZE, 7.0f, 0.5f, 5.0f);
	FLARE_GLARE(_lftStandard, 20.0f, 0.3f, 0.8f, 1.0f);

	// standard flare with huge reflections
	FLARE_CREATE(_lftStandardReflections, 15, "01\\WhiteRedRing2.tex", 0.0f, 180.0f, 1.36f, 1.36f, OLF_FADESIZE, 7.0f, 0.5f, 5.0f);
	FLARE_GLARE(_lftStandardReflections, 20.0f, 0.3f, 0.8f, 1.0f);
	REFLECTION(_lftStandardReflections, 1, "01\\WhiteRing.tex", -0.3f, 0.1f);
	REFLECTION(_lftStandardReflections, 2, "01\\BlueDisc.tex", 1-0.5f, 0.047f);
	REFLECTION(_lftStandardReflections, 3, "01\\BlueDisc.tex", 1-0.41f, 0.078f);
	REFLECTION(_lftStandardReflections, 4, "01\\BlueDiscWeak.tex", 1-0.45f, 0.15f);
	REFLECTION(_lftStandardReflections, 5, "01\\BrownDisc.tex", 1-0.2f, 0.05f);
	REFLECTION(_lftStandardReflections, 6, "01\\WhiteGradient.tex", 1-0.1f, 0.016f);
	REFLECTION(_lftStandardReflections, 7, "01\\WhiteGradient.tex", 1+0.29f, 0.027f);
	REFLECTION(_lftStandardReflections, 8, "01\\BrownDisc.tex", 1+0.5f, 0.05f);
	REFLECTION(_lftStandardReflections, 9, "01\\BrownDisc.tex", 1+0.43f, 0.11f);
	REFLECTION(_lftStandardReflections,10, "01\\BrownRing.tex", 1+0.49f, 0.19f);
	REFLECTION(_lftStandardReflections,11, "01\\BlueDisc.tex", 1+0.68f, 0.08f);
	REFLECTION(_lftStandardReflections,12, "01\\BlueGradient.tex", 1+0.7f, 0.043f);
	REFLECTION(_lftStandardReflections,13, "01\\GreenRing.tex", 1+1.04f, 0.27f);
	REFLECTION(_lftStandardReflections,14, "01\\RainbowRing.tex", 1+1.35f, 0.53f);

	// nice yellow star with red ring
	FLARE_CREATE(_lftYellowStarRedRing, 1, "02\\Flare05.tex", 0.0f, 180.0f, 1/5.0f, 1/5.0f, OLF_FADESIZE, 7.0f, 0.5f, 5.0f);
	FLARE_GLARE(_lftYellowStarRedRing, 20.0f, 0.3f, 0.8f, 1.0f);

	// same yellow star with red ring but visible from far away
	FLARE_CREATE(_lftYellowStarRedRingFar, 1, "02\\Flare05.tex", 0.0f, 180.0f, 1/12.0f, 1/12.0f, OLF_FADESIZE, 0.25f, 0.5f, 128.0f);
	FLARE_GLARE(_lftYellowStarRedRingFar, 20.0f, 0.3f, 0.8f, 1.0f);
	
	// nice yellow star with red ring
	FLARE_CREATE(_lftWhiteGlowStarRedRing, 1, "03\\Flare06.tex", 0.0f, 180.0f, 1/5.0f, 1/5.0f, OLF_FADESIZE, 7.0f, 0.5f, 5.0f);
	FLARE_GLARE(_lftWhiteGlowStarRedRing, 20.0f, 0.3f, 0.8f, 1.0f);
	
	FLARE_CREATE(_lftWhiteGlowStar, 1, "04\\Flare07.tex", 0.0f, 180.0f, 1/5.0f, 1/5.0f, OLF_FADESIZE, 7.0f, 0.5f, 5.0f);
	FLARE_GLARE(_lftWhiteGlowStar, 20.0f, 0.3f, 0.8f, 1.0f);
	
	FLARE_CREATE(_lftWhiteGlowStarNG, 1, "04\\Flare07.tex", 0.0f, 180.0f, 1/5.0f, 1/5.0f, OLF_FADESIZE, 7.0f, 0.5f, 5.0f);

	FLARE_CREATE(_lftWhiteStarRedRingStreaks, 1, "05\\Flare09.tex", 0.0f, 180.0f, 1/5.0f, 1/5.0f, OLF_FADESIZE, 7.0f, 0.5f, 5.0f);
	FLARE_GLARE(_lftWhiteStarRedRingStreaks, 20.0f, 0.3f, 0.8f, 1.0f);
	
	// white star flare with many red and brown hexagons
	FLARE_CREATE(_lftWhiteStarRedReflections, 12, "06\\WhiteStarManyStreaks.tex", 0.0f, 0.0f, 0.20625f, 0.20625f, OLF_FADESIZE, 7.0f, 0.5f, 5.0f);
	FLARE_GLARE(_lftWhiteStarRedReflections, 20.0f, 0.3f, 0.8f, 1.0f);
	REFLECTION(_lftWhiteStarRedReflections, 1, "06\\DarkRedPentagram.tex" ,-0.92424242f,0.06875f); 
	REFLECTION(_lftWhiteStarRedReflections, 2, "06\\LillaPentagram.tex"   ,0.28787879f,0.0296875f);
	REFLECTION(_lftWhiteStarRedReflections, 3, "06\\MagentaPentagram.tex" ,0.43939394f,0.05f);     
	REFLECTION(_lftWhiteStarRedReflections, 4, "06\\MagentaGlow.tex"      ,1.52272727f,0.009375f); 
	REFLECTION(_lftWhiteStarRedReflections, 5, "06\\DarkRedPentagram.tex" ,1.9469697f,0.06875f);   
	REFLECTION(_lftWhiteStarRedReflections, 6, "06\\MagentaGlow.tex"      ,1.96212121f,0.05f);     
	REFLECTION(_lftWhiteStarRedReflections, 7, "06\\DarkRedPentagram.tex" ,1.08333333f,0.06875f);  
	REFLECTION(_lftWhiteStarRedReflections, 8, "06\\DarkRedPentagram.tex" ,1.59848485f,0.06875f);  
	REFLECTION(_lftWhiteStarRedReflections, 9, "06\\DarkRedPentagram.tex" ,1.67424242f,0.06875f);  
	REFLECTION(_lftWhiteStarRedReflections,10, "06\\DarkRedPentagram.tex" ,-0.12878788f,0.03125f); 
	REFLECTION(_lftWhiteStarRedReflections,11, "06\\BrownPentagram.tex"   ,0.03030303f,0.021875f); 

	// blue star flare with many blue flares
	FLARE_CREATE(_lftBlueStarBlueReflections, 21, "07\\BlueStarManyStreaks.tex", 0.0f, 0.0f, 0.4f, 0.4f, OLF_FADESIZE, 7.0f, 0.5f, 5.0f);
	FLARE_GLARE(_lftBlueStarBlueReflections, 20.0f, 0.3f, 0.8f, 1.0f);
	REFLECTION(_lftBlueStarBlueReflections, 1, "07\\BlueGlow.tex", -0.5f,0.05f); 
	REFLECTION(_lftBlueStarBlueReflections, 2, "07\\BluePentagram.tex", -0.25f,0.03f); 
	REFLECTION(_lftBlueStarBlueReflections, 3, "07\\GreenGlow.tex", -0.05f,0.04f); 
	REFLECTION(_lftBlueStarBlueReflections, 4, "07\\GreenGlow.tex", 0.3f,0.02f); 
	REFLECTION(_lftBlueStarBlueReflections, 5, "07\\BluePentagram.tex", 0.5f,0.05f); 
	REFLECTION(_lftBlueStarBlueReflections, 6, "07\\DarkBluePentagram.tex", 0.8f,0.04f); 
	REFLECTION(_lftBlueStarBlueReflections, 7, "07\\LittleBluePentagram.tex", 1.2f,0.02f); 
	REFLECTION(_lftBlueStarBlueReflections, 8, "07\\MagentaPentagram.tex", 1.13f,0.08f); 
	REFLECTION(_lftBlueStarBlueReflections, 9, "07\\DarkBluePentagram.tex", 1.24f,0.03f); 
	REFLECTION(_lftBlueStarBlueReflections,10, "07\\BlueGlow.tex", 1.4f,0.06f); 
	REFLECTION(_lftBlueStarBlueReflections,11, "07\\GreenGlow.tex", 1.5f,0.02f); 
	REFLECTION(_lftBlueStarBlueReflections,12, "07\\BluePentagram.tex", 1.64f,0.05f); 
	REFLECTION(_lftBlueStarBlueReflections,13, "07\\LittleBluePentagram.tex", 1.7f,0.05f); 
	REFLECTION(_lftBlueStarBlueReflections,14, "07\\BluePentagram.tex", 1.8f,0.06f); 
	REFLECTION(_lftBlueStarBlueReflections,15, "07\\MagentaPentagram.tex", 2.0f,0.01f); 
	REFLECTION(_lftBlueStarBlueReflections,16, "07\\BlueGlow.tex", 2.0f,0.06f); 
	REFLECTION(_lftBlueStarBlueReflections,17, "07\\MagentaPentagram.tex", 2.0f,0.02f); 
	REFLECTION(_lftBlueStarBlueReflections,18, "07\\GreenGlow.tex", 2.1f,0.015f); 
	REFLECTION(_lftBlueStarBlueReflections,19, "07\\BluePentagram.tex", 2.4f,0.05f); 
	REFLECTION(_lftBlueStarBlueReflections,20, "07\\DarkBluePentagram.tex", 2.8f,0.03f); 

	FLARE_CREATE(_lftProjectileStarGlow, 1, "08\\FlarePower.tex", 0.0f, 180.0f, 1/5.0f, 1/5.0f, OLF_FADESIZE, 7.0f, 0.5f, 10.0f);
	FLARE_GLARE(_lftProjectileStarGlow, 20.0f, 0.3f, 0.8f, 1.0f);

	FLARE_CREATE(_lftProjectileWhiteBubbleGlow, 1, "09\\FlareWhiteBubble.tex", 0.0f, 180.0f, 1/5.0f, 1/5.0f, OLF_FADESIZE, 7.0f, 0.5f, 10.0f);
	FLARE_GLARE(_lftProjectileWhiteBubbleGlow, 20.0f, 0.3f, 0.8f, 1.0f);

	FLARE_CREATE(_lftProjectileYellowBubbleGlow, 1, "10\\FlareYellowBubble.tex", 0.0f, 180.0f, 1/10.0f, 1/10.0f, OLF_FADESIZE, 7.0f, 0.5f, 10.0f);
	FLARE_GLARE(_lftProjectileYellowBubbleGlow, 20.0f, 0.3f, 0.8f, 1.0f);

//	FLARE_CREATE(_lftPVSpaceShipWindowFlare, 1, "05\\Flare09.tex", 0.0f, 180.0f, 1/10.0f, 1/10.0f, OLF_FADESIZE, 1.0f, 0.0f, 10.0f);

	//FLARE_CREATE(_lftCatmanFireGlow, 1, "12\\Flare12.tex", 0.0f, 180.0f, 1/12.0f, 1/12.0f, OLF_FADESIZE, 7.0f, 0.5f, 128.0f);
	
	FLARE_CREATE(_lftWhiteGlowFar, 1, "13\\Flare13.tex", 0.0f, 180.0f, 1/16.0f, 1/16.0f, OLF_FADESIZE, 7.0f, 0.5f, 128.0f);
};

// close lens flares effects
void CloseLensFlares(void) {
	_lftStandard.lft_aolfFlares.Clear();
	_lftStandardReflections.lft_aolfFlares.Clear();
	_lftYellowStarRedRing.lft_aolfFlares.Clear();
	_lftYellowStarRedRingFar.lft_aolfFlares.Clear();
	_lftWhiteGlowStarRedRing.lft_aolfFlares.Clear();
	_lftWhiteGlowStar.lft_aolfFlares.Clear();
	_lftWhiteGlowStarNG.lft_aolfFlares.Clear();
	_lftWhiteStarRedRingStreaks.lft_aolfFlares.Clear();
	_lftWhiteStarRedReflections.lft_aolfFlares.Clear();
	_lftBlueStarBlueReflections.lft_aolfFlares.Clear();
	_lftProjectileStarGlow.lft_aolfFlares.Clear();
	_lftProjectileWhiteBubbleGlow.lft_aolfFlares.Clear();
	_lftProjectileYellowBubbleGlow.lft_aolfFlares.Clear();
//	_lftPVSpaceShipWindowFlare.lft_aolfFlares.Clear();
	//_lftCatmanFireGlow.lft_aolfFlares.Clear();
	_lftWhiteGlowFar.lft_aolfFlares.Clear();
};

static BOOL _bFatalChecks = FALSE;


/************************************************************
 *                      PLAYER APPEARANCE                   *
 ************************************************************/
/* Set the model data */
void SetModelData_t(CModelObject *pmo, const CTFileName &fnmModel) {
	ASSERT(pmo != NULL);
	pmo->SetData_t(fnmModel);   // load the new model data
};

/* Set the texture data */
void SetTextureData_t(CModelObject *pmo, const CTFileName &fnmTexture) {
	ASSERT(pmo != NULL);
	pmo->mo_toTexture.SetData_t(fnmTexture);    // load the texture data
};

/* Set model */
void SetModel_t(CModelObject *pmo, const CTFileName &fnmModel, const CTFileName &fnmTexture) {
	SetModelData_t(pmo, fnmModel);
	SetTextureData_t(pmo, fnmTexture);
};

/* Add attachment to model */
void ModelAddAttachment_t(CModelObject *pmo, INDEX iAttachment, 
												const CTFileName &fnmModel, const CTFileName &fnmTexture) {
	ASSERT(pmo != NULL);
	if (fnmModel==CTString("")) return;
	if (pmo==NULL) return;
	
	CAttachmentModelObject *pamo = pmo->AddAttachmentModel(iAttachment);
	SetModel_t(&(pamo->amo_moModelObject), fnmModel, fnmTexture);
};

CTString _strFile;
INDEX _ctLines;

CTString GetNonEmptyLine_t(CTStream &strm)
{
	FOREVER {
	 if(strm.AtEOF()) {
		 ThrowF_t(TRANS("Unexpected end of file"));
	 }
	 CTString str;
	 _ctLines++;
	 strm.GetLine_t(str);
	 str.TrimSpacesLeft();
	 if (str.RemovePrefix("//")) {  // skip comments
		 continue;
	 }
	 if (str!="") {
		 str.TrimSpacesRight();
		 return str;
	 }
	}
}

void FixupFileName_t(CTString &strFnm)
{
	strFnm.TrimSpacesLeft();
	if (!strFnm.RemovePrefix(CTString("TF") +"NM ")) {  // must not directly have ids in code
		ThrowF_t(TRANS("Expected %s%s before filename"), "TF", "NM");
	}
}

// skip one block in pmc
void SkipBlock_t(CTStream &strm)
{
	CTString strLine;
	// expect to begin with an open bracket
	strLine = GetNonEmptyLine_t(strm);
	if (strLine!="{") {
		ThrowF_t(TRANS("Expected '{'"));
	}
	// start at level one
	INDEX ctLevel = 1;
	// repeat
	do {
		strLine = GetNonEmptyLine_t(strm);
		// count brackets
		if (strLine=="{") {
			ctLevel++;
		} else if (strLine=="}") {
			ctLevel--;
		}
	// until we close down all brackets
	} while(ctLevel>0);
}

void ParseAMC_t(CModelObject *pmo, CTStream &strm, BOOL bPreview)
{
	CTString strLine;
	// expect to begin with an open bracket
	strLine = GetNonEmptyLine_t(strm);
	if (strLine!="{") {
		ThrowF_t(TRANS("Expected '{'"));
	}

	// repeat
	FOREVER {
		// read one line
		strLine = GetNonEmptyLine_t(strm);
		
		// if closed bracket
		if (strLine == "}") {
			// finish parsing
			return;
		}


		// if a preview-only block
		if (strLine.RemovePrefix("PreviewOnly")) {
			// if this is a preview
			if (bPreview) {
				// keep parsing it
				ParseAMC_t(pmo, strm, bPreview);
			// if this is not a preview
			} else {
				// skip that block
				SkipBlock_t(strm);
			}
		// if include block
		} else if (strLine.RemovePrefix("Include:")) {
			// open the new file
			FixupFileName_t(strLine);
			CTFileStream strmIncluded;
			strmIncluded.Open_t(strLine);

			// include it
			INDEX ctLinesOld = _ctLines;
			CTString strFileOld = _strFile;
			_ctLines = 0;
			_strFile = strLine;
			ParseAMC_t(pmo, strmIncluded, bPreview);
			strmIncluded.Close();
			_ctLines = ctLinesOld;
			_strFile = strFileOld;

		// if setting the model
		} else if (strLine.RemovePrefix("Model:")) {
			// set the model
			FixupFileName_t(strLine);
			pmo->SetData_t(strLine);

		// if setting an anim for the model
		} else if (strLine.RemovePrefix("Animation:")) {
			// get animation number
			INDEX iAnim = -1;
			strLine.ScanF("%d", &iAnim);
			if (iAnim<0) {
				ThrowF_t(TRANS("Invalid animation number"));
			}
			// check it
			if (iAnim>=pmo->GetAnimsCt()) {
				ThrowF_t(TRANS("Animation %d does not exist in that model"), iAnim);
			};
			// set it
			pmo->PlayAnim(iAnim, AOF_LOOPING);

		// if texture
		} else if (strLine.RemovePrefix("Texture:")) {
			// set texture
			FixupFileName_t(strLine);
			pmo->mo_toTexture.SetData_t(strLine);

		// if specular
		} else if (strLine.RemovePrefix("Specular:")) {
			// set texture
			FixupFileName_t(strLine);
			pmo->mo_toSpecular.SetData_t(strLine);

		// if reflection
		} else if (strLine.RemovePrefix("Reflection:")) {
			// set texture
			FixupFileName_t(strLine);
			pmo->mo_toReflection.SetData_t(strLine);

		// if specular
		} else if (strLine.RemovePrefix("Bump:")) {
			// set texture
			FixupFileName_t(strLine);
			pmo->mo_toBump.SetData_t(strLine);

		// if attachment
		} else if (strLine.RemovePrefix("Attachment:")) {
			// get attachment number
			INDEX iAtt = -1;
			strLine.ScanF("%d", &iAtt);
			if (iAtt<0) {
				ThrowF_t(TRANS("Invalid attachment number"));
			}
			// create attachment
			CModelData *pmd = (CModelData*)pmo->GetData();
			if (iAtt>=pmd->md_aampAttachedPosition.Count()) {
				ThrowF_t(TRANS("Attachment %d does not exist in that model"), iAtt);
			};
			CAttachmentModelObject *pamo = pmo->AddAttachmentModel(iAtt);
			
			// recursively parse it
			ParseAMC_t(&pamo->amo_moModelObject, strm, bPreview);
		} else {
			ThrowF_t(TRANS("Expected texture or attachment"));
		}
	}
}

/* Set player appearance */
BOOL SetPlayerAppearance_internal(CModelObject *pmo, const CTFileName &fnmAMC, CTString &strName, BOOL bPreview)
{
	// try to
	try {
		// open the config file
		CTFileStream strm;
		strm.Open_t(fnmAMC);

		_ctLines = 0;
		_strFile = fnmAMC;

		// read the name
		CTString strLine = GetNonEmptyLine_t(strm);
		if (!strLine.RemovePrefix("Name: ")) {
			ThrowF_t(TRANS("Expected name"));
		}
		strName = strLine;
		strName.TrimSpacesLeft();

		// parse the file recursively starting at root model object and add everything
		ParseAMC_t(pmo, strm, bPreview);
		return TRUE;

	// if anything failed
	} catch (char *strError) {
		// report error
		CPrintF(TRANS("Cannot load player model:\n%s (%d) : %s\n"), 
			(const char*)_strFile, _ctLines, strError);
		return FALSE;
	}
}

BOOL SetPlayerAppearance(CModelObject *pmo, CPlayerCharacter *ppc, CTString &strName, BOOL bPreview)
{
	// first kill any existing model
	pmo->SetData(NULL);
	pmo->mo_toTexture.SetData(NULL);
	pmo->mo_toSpecular.SetData(NULL);
	pmo->mo_toReflection.SetData(NULL);
	pmo->mo_toBump.SetData(NULL);
	pmo->RemoveAllAttachmentModels();

	DECLARE_CTFILENAME(fnmDefault, "Data\\ModelsMP\\Player\\SeriousSam.amc");

	// if no character, or player models are disabled
	if (ppc==NULL) {
		// set default appearance
		BOOL bSucceeded = SetPlayerAppearance_internal(pmo, fnmDefault, strName, bPreview);
		if (!bSucceeded) {
			FatalError(TRANS("Cannot load default player model!"));
		}
		return FALSE;
	}

	// get filename from the settings
	CPlayerSettings *pps = (CPlayerSettings *)ppc->pc_aubAppearance;
	CTFileName fnmModelFile = pps->GetModelFilename();
	// if dummy (empty settings)
	if (fnmModelFile.FileName()=="") {
		// use default
		fnmModelFile = fnmDefault;
	}

	extern INDEX plr_bOnlySam;
	if (!plr_bOnlySam && SetPlayerAppearance_internal(pmo, fnmModelFile, strName, bPreview)) {
		return TRUE;
	} else if (SetPlayerAppearance_internal(pmo, fnmDefault, strName, bPreview)) {  // HAVE TO SET DEFAULT HERE!
		return TRUE;
	} else {
		return FALSE;
	}
}

//SKA
BOOL SetPlayerAppearance_internal(CModelInstance *pmiModel, const CTFileName &fnmSMC, CTString &strName, BOOL bPreview)
{
	// try to
	try {
		ASSERT(pmiModel!=NULL);
		if (pmiModel!=NULL) {
			ObtainModelInstance_t(*pmiModel, fnmSMC);
		}
		
		strName = pmiModel->GetName();

		return TRUE;

	// if anything failed
	} catch (char *strError) {
		// report error
		CPrintF(TRANS("Cannot load player model '%s':\n%s\n"), 
			fnmSMC, strError);
		return FALSE;
	}
}

// get filename for model
static char *astrModels[] = 
{
	"SeriousSam",
	"SeriousSammy",
	"GroovyGregory",
	"FastFingerFreddy",
	"PiratePete",
	"KingKim",
	"SantaSam",
	"StainlessSteve",
	"MeesteriousMia",
};
CTFileName GetModelFilename(INDEX iModel)
{
	iModel %= ARRAYCOUNT(astrModels);
	return "Data\\Models\\Player\\"+CTString(astrModels[iModel])+".smc";
}

//SKA
BOOL SetPlayerAppearance(CModelInstance *pmiModel, CPlayerCharacter *ppc, CTString &strName, BOOL bPreview)
{
	// first kill any existing model
	if (pmiModel!=NULL) {
		pmiModel->Clear();
	} 

	DECLARE_CTFILENAME(fnmDefault, "Data\\Models\\Player\\SeriousSam.smc");

	// if no character, or player models are disabled
	if (ppc==NULL) {
		// set default appearance
		BOOL bSucceeded = SetPlayerAppearance_internal(pmiModel, fnmDefault, strName, bPreview);
		if (!bSucceeded) {
			FatalError(TRANS("Cannot load default player model!"));
		}
		return FALSE;
	}

	// get filename from the settings
	CPlayerSettings *pps = (CPlayerSettings *)ppc->pc_aubAppearance;
	CTFileName fnmModelFile = GetModelFilename(pps->ps_iModel);
	// if dummy (empty settings)
	if (fnmModelFile.FileName()=="") {
		// use default
		fnmModelFile = fnmDefault;
	}

	extern INDEX plr_bOnlySam;
	if (!plr_bOnlySam && SetPlayerAppearance_internal(pmiModel, fnmModelFile, strName, bPreview)) {
		return TRUE;
	} else if (SetPlayerAppearance_internal(pmiModel, fnmDefault, strName, bPreview)) {  // HAVE TO SET DEFAULT HERE!
		return TRUE;
	} else {
		return FALSE;
	}
}

//0105
//SKA
BOOL SetPlayerAppearanceSka(CModelInstance *pmiModel, CPlayerCharacter *ppc, CTString &strName, BOOL bPreview, ULONG type)
{
	// first kill any existing model
	if (pmiModel == NULL)
		return FALSE;
	
	pmiModel->Clear();
	pmiModel->mi_bRenderShadow = TRUE;
	
	CTString fnmDefault;
	
	if( ppc != NULL )
	{
		fnmDefault = CJobInfo::getSingleton()->GetFileName( ppc->pc_iPlayerType );
	}
	
	// if no character, or player models are disabled
	if (ppc==NULL) 
	{
		// set default appearance
		
		if( type >= TITAN && type <= TOTAL_JOB )
		{
			fnmDefault = CJobInfo::getSingleton()->GetFileName( type );
		}
		else
		{
			CMobData* MD = CMobData::getData( type );
			fnmDefault = MD->GetMobSmcFileName();
		}
		
		BOOL bSucceeded = SetPlayerAppearance_internal(pmiModel, fnmDefault, strName, bPreview);
		if (!bSucceeded) 
		{
			FatalError(TRANS("Cannot load default player model!"));
		}
		return FALSE;
	}
	
	//0215
	CTString fnmModelFile;
	fnmModelFile = fnmDefault;	
	
	extern INDEX plr_bOnlySam;
	if (!plr_bOnlySam && SetPlayerAppearance_internal(pmiModel, fnmModelFile, strName, bPreview)) {
		return TRUE;
	} else if (SetPlayerAppearance_internal(pmiModel, fnmDefault, strName, bPreview)) {  // HAVE TO SET DEFAULT HERE!
		return TRUE;
	} else {
		return FALSE;
	}
}
//..

/************************************************************
 *                    DEBUGGING FUNCTIONS                   *
 ************************************************************/
// debugging functions
const char *PrintConsole(void)
{
	_RPT1(_CRT_WARN, "%s", CON_GetBuffer());
	return NULL;
}

const char *PrintStack(CEntity *pen)
{
	return pen->PrintStackDebug();
}

void StartDebugTrigger(CEntity *penCaused)
{
	// for each entity in the world
	FOREACHINDYNAMICCONTAINER(penCaused->GetWorld()->wo_cenEntities, CEntity, iten) 
	{
		CEntity *pen = iten;
		if (IsOfClass(pen, "Trigger") && pen->GetName()=="__DebugTrigger__")
		{
			ETrigger eTrigger;
			eTrigger.penCaused = penCaused;
			pen->SendEvent(eTrigger);
		}
	}
}

/************************************************************
 *                          DEBRIS                          *
 ************************************************************/
EntityInfoBodyType _Eeibt;
enum DebrisParticlesType _dptParticles;
enum BasicEffectType  _betStain;
FLOAT3D _vSpeed;
FLOAT3D _vSpawnerSpeed;
FLOAT _fEntitySize;
FLOAT _fConeSize;
FLOAT _fSpeedUp;
COLOR _colDebris;
BOOL  _bCollision;
BOOL  _bFade;

// debris spawning
void Debris_Begin(
	EntityInfoBodyType Eeibt, 
	enum DebrisParticlesType dptParticles,
	enum BasicEffectType  betStain,
	FLOAT fEntitySize,                  // entity size in meters
	const FLOAT3D &vSpeed,
	const FLOAT3D &vSpawnerSpeed,       // how fast was the entity moving
	const FLOAT fConeSize,              // size multiplier for debris cone
	const FLOAT fSpeedUp,               // size multiplier for debris catapulting up (0-no multiply)
	const COLOR colDebris, /*=C_WHITE*/  // multiply color
	const BOOL	bCollision,
	const BOOL	bFade
)
{
	_Eeibt          = Eeibt       ;
	_dptParticles   = dptParticles;
	_betStain       = betStain    ;
	_vSpeed         = vSpeed      ;
	_vSpawnerSpeed  = vSpawnerSpeed;
	_fEntitySize    = fEntitySize ;
	_fConeSize      = fConeSize   ;
	_fSpeedUp       = fSpeedUp    ;
	_colDebris      = colDebris   ;
	_bCollision		= bCollision;
	_bFade			= bFade;
}

CEntityPointer Debris_Spawn(
	CEntity *penSpawner,
	CEntity *penComponents,
	SLONG idModelComponent,
	SLONG idTextureComponent,
	SLONG idReflectionTextureComponent,
	SLONG idSpecularTextureComponent,
	SLONG idBumpTextureComponent,
	INDEX iModelAnim,
	FLOAT fSize,
	const FLOAT3D &vPosRatio)
{
	// create debris at same world as spawner
	FLOAT3D vPos;
	FLOAT3D vStretch=FLOAT3D(1,1,1);
	if( (penSpawner->en_RenderType==CEntity::RT_MODEL ||
			 penSpawner->en_RenderType==CEntity::RT_EDITORMODEL) &&
			 penSpawner->GetModelObject()!=NULL)
	{
		vStretch=penSpawner->GetModelObject()->mo_Stretch;
	}
	penSpawner->GetEntityPointRatio(vPosRatio, vPos);
	CEntityPointer penDebris = penSpawner->GetWorld()->CreateEntity_t(
		CPlacement3D(vPos, ANGLE3D(0,0,0)), CTFILENAME("Classes\\Debris.ecl"),WLD_AUTO_ENTITY_ID,FALSE);
	// prepare parameters
	ESpawnDebris eSpawn;
	eSpawn.bImmaterialASAP=FALSE;
	eSpawn.bCustomShading=FALSE;
	eSpawn.Eeibt = _Eeibt;
	eSpawn.dptParticles = _dptParticles;
	eSpawn.betStain = _betStain;
	eSpawn.pmd = penComponents->GetModelDataForComponent(idModelComponent);
	eSpawn.ptd = penComponents->GetTextureDataForComponent(idTextureComponent);
	eSpawn.ptdRefl = penComponents->GetTextureDataForComponent(idReflectionTextureComponent);
	eSpawn.ptdSpec = penComponents->GetTextureDataForComponent(idSpecularTextureComponent);
	eSpawn.ptdBump = penComponents->GetTextureDataForComponent(idBumpTextureComponent);
	eSpawn.iModelAnim = iModelAnim;
	eSpawn.colDebris = _colDebris;
	eSpawn.vStretch = FLOAT3D(1,1,1);
	if (fSize==0) {
		eSpawn.fSize = 1.0f;
	} else {
		eSpawn.fSize = _fEntitySize*fSize;
	}
	
//강동민 수정 시작 시스템 싱글던젼 개선	10.05
	CDLLEntityClass *pdecDLLClass		= penDebris->GetClass()->ec_pdecDLLClass;	
	CEntityProperty &epDebrisCollision	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 201);
	ENTITYPROPERTY( &*penDebris, epDebrisCollision.ep_slOffset, BOOL) = _bCollision;
	CEntityProperty &epDebrisFade		= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 200);
	ENTITYPROPERTY( &*penDebris, epDebrisFade.ep_slOffset, BOOL) = _bFade;
//강동민 수정 끝 싱글던젼 개선		10.05

	// initialize it
	penDebris->Initialize(eSpawn,FALSE);

	FLOAT fCone = _fEntitySize*1.0f;
	if (_vSpeed.Length()==0) {
		fCone = 0;
	}
	FLOAT fRndX = (penSpawner->FRnd()*2-1)*fCone*_fConeSize;
	FLOAT fRndY = (penSpawner->FRnd()*2-1)*fCone*_fConeSize;
	FLOAT fRndZ = (penSpawner->FRnd()*2-1)*fCone*_fConeSize;

	FLOAT fRndH = penSpawner->FRnd();
	FLOAT fRndP = penSpawner->FRnd();
	FLOAT fRndB = penSpawner->FRnd();

	FLOAT3D vUp;
	const FLOATmatrix3D &m = penSpawner->GetRotationMatrix();
	vUp(1) = m(1,2);
	vUp(2) = m(2,2);
	vUp(3) = m(3,2);

	//FLOAT fStrength = _vSpeed.Length();

	// speed it up
	((CMovableEntity&)*penDebris).LaunchAsFreeProjectile(
		_vSpawnerSpeed+_vSpeed+FLOAT3D(fRndX, fRndY, fRndZ)+vUp*_fSpeedUp, (CMovableEntity*)penSpawner);
	((CMovableEntity&)*penDebris).SetDesiredRotation(
		ANGLE3D(fRndH*360.0f-180.0f, fRndP*360.0f-180.0f, fRndB*360.0f-180.0f));

	return penDebris;
}

CEntityPointer Debris_Spawn_Independent(
	CEntity *penSpawner,
	CEntity *penComponents,
	SLONG idModelComponent,
	SLONG idTextureComponent,
	SLONG idReflectionTextureComponent,
	SLONG idSpecularTextureComponent,
	SLONG idBumpTextureComponent,
	INDEX iModelAnim,
	FLOAT fSize,
	CPlacement3D plAbsolutePlacement,
	FLOAT3D vTranslation,
	ANGLE3D aRotation)
{
	// create debris at same world as spawner
	CEntityPointer penDebris = penSpawner->GetWorld()->CreateEntity_t(
		plAbsolutePlacement, CTFILENAME("Classes\\Debris.ecl"),WLD_AUTO_ENTITY_ID,FALSE);
	// prepare parameters
	ESpawnDebris eSpawn;
	eSpawn.bImmaterialASAP=FALSE;
	eSpawn.bCustomShading=FALSE;
	eSpawn.Eeibt = _Eeibt;
	eSpawn.dptParticles = _dptParticles;
	eSpawn.betStain = _betStain;
	eSpawn.pmd = penComponents->GetModelDataForComponent(idModelComponent);
	eSpawn.ptd = penComponents->GetTextureDataForComponent(idTextureComponent);
	eSpawn.ptdRefl = penComponents->GetTextureDataForComponent(idReflectionTextureComponent);
	eSpawn.ptdSpec = penComponents->GetTextureDataForComponent(idSpecularTextureComponent);
	eSpawn.ptdBump = penComponents->GetTextureDataForComponent(idBumpTextureComponent);
	eSpawn.iModelAnim = iModelAnim;
	eSpawn.colDebris = _colDebris;
	eSpawn.fSize = fSize;
	eSpawn.vStretch = FLOAT3D(1,1,1);
	
	// initialize it
	penDebris->Initialize(eSpawn,FALSE);

	// move it 
	((CMovableEntity&)*penDebris).LaunchAsFreeProjectile(
		vTranslation, (CMovableEntity*)penSpawner);
	((CMovableEntity&)*penDebris).SetDesiredRotation(aRotation);

	return penDebris;
}

CEntityPointer Debris_Spawn_Template(
	EntityInfoBodyType eibt,
	enum DebrisParticlesType dptParticles,
	enum BasicEffectType betStain,
	CModelHolder2 *penmhDestroyed,
	CEntity *penComponents,
	CModelHolder2 *penmhTemplate,
	FLOAT3D vStretch,
	FLOAT fSize,
	CPlacement3D plAbsolutePlacement,
	FLOAT3D vLaunchSpeed,
	ANGLE3D aRotSpeed,
	BOOL bDebrisImmaterialASAP,
	FLOAT fDustStretch,
	COLOR colBurning)
{
	if(penmhTemplate==NULL || penmhTemplate->GetModelObject()==NULL)
	{
		return NULL;
	}
	// create debris at same world as spawner
	CEntityPointer penDebris = penmhDestroyed->GetWorld()->CreateEntity_t(
		plAbsolutePlacement, CTFILENAME("Classes\\Debris.ecl"),WLD_AUTO_ENTITY_ID,FALSE);
	// prepare parameters
	ESpawnDebris eSpawn;
	eSpawn.bImmaterialASAP=bDebrisImmaterialASAP;
	eSpawn.fDustStretch=fDustStretch;
	eSpawn.Eeibt = eibt;
	eSpawn.dptParticles = dptParticles;
	eSpawn.betStain = betStain;
	CModelObject &mo=*penmhTemplate->GetModelObject();
	eSpawn.pmd = mo.GetData();
	eSpawn.ptd = (CTextureData*)mo.mo_toTexture.GetData();
	eSpawn.ptdRefl = (CTextureData*)mo.mo_toReflection.GetData();
	eSpawn.ptdSpec = (CTextureData*)mo.mo_toSpecular.GetData();
	eSpawn.ptdBump = (CTextureData*)mo.mo_toBump.GetData();
	eSpawn.iModelAnim = mo.GetAnim();
	eSpawn.colDebris = colBurning;
	eSpawn.fSize = 1.0f;
	eSpawn.vStretch = vStretch;
	eSpawn.bCustomShading=FALSE;
	eSpawn.penFallFXPapa=penmhTemplate;
	if( penmhDestroyed->m_cstCustomShading==CST_FULL_CUSTOMIZED)
	{
		eSpawn.bCustomShading=TRUE;
		eSpawn.aShadingDirection=penmhDestroyed->m_aShadingDirection;
		eSpawn.colCustomDiffuse=penmhDestroyed->m_colLight;
		eSpawn.colCustomAmbient=penmhDestroyed->m_colAmbient;
	}
	
	// initialize it
	penDebris->Initialize(eSpawn,FALSE);

	// move it 
	const FLOATmatrix3D &m = penDebris->GetRotationMatrix();
	((CMovableEntity&)*penDebris).LaunchAsFreeProjectile( vLaunchSpeed*!m, (CMovableEntity*)penmhDestroyed);
	((CMovableEntity&)*penDebris).SetDesiredRotation(aRotSpeed);

	return penDebris;
}

// this will spawn a ModelHolder2 based debris from a ModelHolder2 Parent
CEntityPointer Debris_Spawn_MH2Template_FromMH2(
	EntityInfoBodyType eibt,
	enum DebrisParticlesType dptParticles,
	enum BasicEffectType betStain,
	CModelHolder2 *penmhDestroyed,
	CEntity *penComponents,
	CModelHolder2 *penmhTemplate,
	FLOAT3D vStretch,
	FLOAT fSize,
	CPlacement3D plAbsolutePlacement,
	FLOAT3D vLaunchSpeed,
	ANGLE3D aRotSpeed,
	BOOL bDebrisImmaterialASAP,
	FLOAT fDustStretch,
	COLOR colBurning)
{
	if(penmhTemplate==NULL || penmhTemplate->GetModelObject()==NULL)
	{
		return NULL;
	}
	// create debris at same world as spawner
	CEntityPointer penDebris = penmhDestroyed->GetWorld()->CreateEntity_t(
		plAbsolutePlacement, CTFILENAME("Classes\\Debris.ecl"),WLD_AUTO_ENTITY_ID,FALSE);
	// prepare parameters
	ESpawnDebris eSpawn;
	eSpawn.bImmaterialASAP=bDebrisImmaterialASAP;
	eSpawn.fDustStretch=fDustStretch;
	eSpawn.Eeibt = eibt;
	eSpawn.dptParticles = dptParticles;
	eSpawn.betStain = betStain;
	CModelObject &mo=*penmhTemplate->GetModelObject();
	eSpawn.pmd = mo.GetData();
	eSpawn.ptd = (CTextureData*)mo.mo_toTexture.GetData();
	eSpawn.ptdRefl = (CTextureData*)mo.mo_toReflection.GetData();
	eSpawn.ptdSpec = (CTextureData*)mo.mo_toSpecular.GetData();
	eSpawn.ptdBump = (CTextureData*)mo.mo_toBump.GetData();
	eSpawn.iModelAnim = mo.GetAnim();
	eSpawn.colDebris = colBurning;
	eSpawn.fSize = 1.0f;
	eSpawn.vStretch = vStretch;
	eSpawn.bCustomShading=FALSE;
	eSpawn.penFallFXPapa=penmhTemplate;
	if( penmhDestroyed->m_cstCustomShading==CST_FULL_CUSTOMIZED)
	{
		eSpawn.bCustomShading=TRUE;
		eSpawn.aShadingDirection=penmhDestroyed->m_aShadingDirection;
		eSpawn.colCustomDiffuse=penmhDestroyed->m_colLight;
		eSpawn.colCustomAmbient=penmhDestroyed->m_colAmbient;
	}
	
	// initialize it
	penDebris->Initialize(eSpawn,FALSE);

	// move it 
	const FLOATmatrix3D &m = penDebris->GetRotationMatrix();
	((CMovableEntity&)*penDebris).LaunchAsFreeProjectile( vLaunchSpeed*!m, (CMovableEntity*)penmhDestroyed);
	((CMovableEntity&)*penDebris).SetDesiredRotation(aRotSpeed);

	return penDebris;
}

// this will spawn a ModelHolder2 based debris from a ModelHolder3 Parent
CEntityPointer Debris_Spawn_MH2Template_FromMH3(
	EntityInfoBodyType eibt,
	enum DebrisParticlesType dptParticles,
	enum BasicEffectType betStain,
	CModelHolder3 *penmhDestroyed,
	CEntity *penComponents,
	CModelHolder2 *penmhTemplate,
	FLOAT3D vStretch,
	FLOAT fSize,
	CPlacement3D plAbsolutePlacement,
	FLOAT3D vLaunchSpeed,
	ANGLE3D aRotSpeed,
	BOOL bDebrisImmaterialASAP,
	FLOAT fDustStretch,
	COLOR colBurning)
{
	if(penmhTemplate==NULL || penmhTemplate->GetModelObject()==NULL)
	{
		return NULL;
	}
	// create debris at same world as spawner
	CEntityPointer penDebris = penmhDestroyed->GetWorld()->CreateEntity_t(
		plAbsolutePlacement, CTFILENAME("Classes\\Debris.ecl"),WLD_AUTO_ENTITY_ID,FALSE);
	// prepare parameters
	ESpawnDebris eSpawn;
	eSpawn.bImmaterialASAP=bDebrisImmaterialASAP;
	eSpawn.fDustStretch=fDustStretch;
	eSpawn.Eeibt = eibt;
	eSpawn.dptParticles = dptParticles;
	eSpawn.betStain = betStain;
	CModelObject &mo=*penmhTemplate->GetModelObject();
	eSpawn.pmd = mo.GetData();
	eSpawn.ptd = (CTextureData*)mo.mo_toTexture.GetData();
	eSpawn.ptdRefl = (CTextureData*)mo.mo_toReflection.GetData();
	eSpawn.ptdSpec = (CTextureData*)mo.mo_toSpecular.GetData();
	eSpawn.ptdBump = (CTextureData*)mo.mo_toBump.GetData();
	eSpawn.iModelAnim = mo.GetAnim();
	eSpawn.colDebris = colBurning;
	eSpawn.fSize = 1.0f;
	eSpawn.vStretch = vStretch;
	eSpawn.bCustomShading=FALSE;
	eSpawn.penFallFXPapa=penmhTemplate;
	if( penmhDestroyed->m_cstCustomShading==CST_FULL_CUSTOMIZED)
	{
		eSpawn.bCustomShading=TRUE;
		eSpawn.aShadingDirection=penmhDestroyed->m_aShadingDirection;
		eSpawn.colCustomDiffuse=penmhDestroyed->m_colLight;
		eSpawn.colCustomAmbient=penmhDestroyed->m_colAmbient;
	}
	
	// initialize it
	penDebris->Initialize(eSpawn,FALSE);

	// move it 
	const FLOATmatrix3D &m = penDebris->GetRotationMatrix();
	((CMovableEntity&)*penDebris).LaunchAsFreeProjectile( vLaunchSpeed*!m, (CMovableEntity*)penmhDestroyed);
	((CMovableEntity&)*penDebris).SetDesiredRotation(aRotSpeed);

	return penDebris;
}

// this will spawn a ModelHolder3 based debris from a ModelHolder2 Parent
CEntityPointer Debris_Spawn_MH3Template_FromMH2(
	EntityInfoBodyType eibt,
	enum DebrisParticlesType dptParticles,
	enum BasicEffectType betStain,
	CModelHolder2 *penmhDestroyed,
	CEntity *penComponents,
	CModelHolder3 *penmhTemplate,
	FLOAT3D vStretch,
	FLOAT fSize,
	CPlacement3D plAbsolutePlacement,
	FLOAT3D vLaunchSpeed,
	ANGLE3D aRotSpeed,
	BOOL bDebrisImmaterialASAP,
	FLOAT fDustStretch,
	COLOR colBurning)
{
	if(penmhTemplate==NULL || penmhTemplate->GetModelInstance()==NULL)
	{
		return NULL;
	}
	// create debris at same world as spawner
	CEntityPointer penDebris = penmhDestroyed->GetWorld()->CreateEntity_t(
		plAbsolutePlacement, CTFILENAME("Classes\\DebrisSka.ecl"),WLD_AUTO_ENTITY_ID,FALSE);
	// prepare parameters
	ESpawnDebrisSka eSpawn;
	eSpawn.penSpawner = penmhDestroyed;
	eSpawn.bImmaterialASAP = bDebrisImmaterialASAP;
	eSpawn.fDustStretch=fDustStretch;
	eSpawn.Eeibt = eibt;
	eSpawn.dptParticles = dptParticles;
	eSpawn.betStain = betStain;  
	eSpawn.bFromModelHolder3 = TRUE;
	eSpawn.penModelHolder3 = penmhTemplate;
	eSpawn.iSkaModelComponentID = 0;
	eSpawn.colDebris = colBurning;
	eSpawn.vStretch = vStretch;  
	eSpawn.fSize = 1.0f;
	eSpawn.bCustomShading=FALSE;
	eSpawn.vStretch = vStretch;
	eSpawn.penFallFXPapa=penmhTemplate;
	if(penmhDestroyed->m_cstCustomShading==CST_FULL_CUSTOMIZED)
	{
		eSpawn.bCustomShading=TRUE;
		eSpawn.aShadingDirection=penmhDestroyed->m_aShadingDirection;
		eSpawn.colCustomDiffuse=penmhDestroyed->m_colLight;
		eSpawn.colCustomAmbient=penmhDestroyed->m_colAmbient;
	}
	// initialize it
	penDebris->Initialize(eSpawn,FALSE);

	// move it 
	const FLOATmatrix3D &m = penDebris->GetRotationMatrix();
	((CMovableEntity&)*penDebris).LaunchAsFreeProjectile( vLaunchSpeed*!m, (CMovableEntity*)penmhDestroyed);
	((CMovableEntity&)*penDebris).SetDesiredRotation(aRotSpeed);

	return penDebris;
}

// this will spawn a ModelHolder3 based debris from a ModelHolder3 Parent
CEntityPointer Debris_Spawn_MH3Template_FromMH3(
	EntityInfoBodyType eibt,
	enum DebrisParticlesType dptParticles,
	enum BasicEffectType betStain,
	CModelHolder3 *penmhDestroyed,
	CEntity *penComponents,
	CModelHolder3 *penmhTemplate,
	FLOAT3D vStretch,
	FLOAT fSize,
	CPlacement3D plAbsolutePlacement,
	FLOAT3D vLaunchSpeed,
	ANGLE3D aRotSpeed,
	BOOL bDebrisImmaterialASAP,
	FLOAT fDustStretch,
	COLOR colBurning)
{
	if(penmhTemplate==NULL || penmhTemplate->GetModelInstance()==NULL)
	{
		return NULL;
	}
	// create debris at same world as spawner
	CEntityPointer penDebris = penmhDestroyed->GetWorld()->CreateEntity_t(
		plAbsolutePlacement, CTFILENAME("Classes\\DebrisSka.ecl"),WLD_AUTO_ENTITY_ID,FALSE);
	// prepare parameters
	ESpawnDebrisSka eSpawn;
	eSpawn.penSpawner = penmhDestroyed;
	eSpawn.bImmaterialASAP = bDebrisImmaterialASAP;
	eSpawn.fDustStretch=fDustStretch;
	eSpawn.Eeibt = eibt;
	eSpawn.dptParticles = dptParticles;
	eSpawn.betStain = betStain;  
	eSpawn.bFromModelHolder3 = TRUE;
	eSpawn.penModelHolder3 = penmhTemplate;
	eSpawn.iSkaModelComponentID = 0;
	eSpawn.colDebris = colBurning;
	eSpawn.vStretch = vStretch;  
	eSpawn.fSize = 1.0f;
	eSpawn.bCustomShading=FALSE;
	eSpawn.vStretch = vStretch;
	eSpawn.penFallFXPapa=penmhTemplate;
	if(penmhDestroyed->m_cstCustomShading==CST_FULL_CUSTOMIZED)
	{
		eSpawn.bCustomShading=TRUE;
		eSpawn.aShadingDirection=penmhDestroyed->m_aShadingDirection;
		eSpawn.colCustomDiffuse=penmhDestroyed->m_colLight;
		eSpawn.colCustomAmbient=penmhDestroyed->m_colAmbient;
	}
	// initialize it
	penDebris->Initialize(eSpawn,FALSE);

	// move it 
	const FLOATmatrix3D &m = penDebris->GetRotationMatrix();
	((CMovableEntity&)*penDebris).LaunchAsFreeProjectile( vLaunchSpeed*!m, (CMovableEntity*)penmhDestroyed);
	((CMovableEntity&)*penDebris).SetDesiredRotation(aRotSpeed);

	return penDebris;
}

/****************/
/*  SKA DEBRIS  */
/****************/
CEntityPointer DebrisSka_Spawn(CEntity *penSpawner, INDEX iSkaModelComponentID,  
	FLOAT fSize, const FLOAT3D &vPosRatio)
{
	// create debris at same world as spawner
	FLOAT3D vPos;
	FLOAT3D vStretch=FLOAT3D(1,1,1);
	if( (penSpawner->en_RenderType==CEntity::RT_SKAMODEL ||
			 penSpawner->en_RenderType==CEntity::RT_SKAEDITORMODEL) &&
			 penSpawner->GetModelInstance()!=NULL)
	{
		vStretch=penSpawner->GetModelInstance()->mi_vStretch;
	}
	penSpawner->GetEntityPointRatio(vPosRatio, vPos);
	CEntityPointer penDebris = penSpawner->GetWorld()->CreateEntity_t(
		CPlacement3D(vPos, ANGLE3D(0,0,0)), CTFILENAME("Classes\\DebrisSka.ecl"),WLD_AUTO_ENTITY_ID,FALSE);
	// prepare parameters
	ESpawnDebrisSka eSpawn;

	eSpawn.penSpawner = penSpawner;
	eSpawn.iSkaModelComponentID = iSkaModelComponentID;
	eSpawn.bImmaterialASAP=FALSE;
	eSpawn.bCustomShading=FALSE;
	eSpawn.Eeibt = _Eeibt;
	eSpawn.dptParticles = _dptParticles;
	eSpawn.betStain = _betStain;
	eSpawn.colDebris = _colDebris;
	eSpawn.vStretch = FLOAT3D(1,1,1);
	eSpawn.bFromModelHolder3 = FALSE;
	if (fSize==0) {
		eSpawn.fSize = 1.0f;
	} else {
		eSpawn.fSize = _fEntitySize*fSize;
	}
	// initialize it
	penDebris->Initialize(eSpawn,FALSE);

	FLOAT fCone = _fEntitySize*1.0f;
	if (_vSpeed.Length()==0) {
		fCone = 0;
	}
	FLOAT fRndX = (penSpawner->FRnd()*2-1)*fCone*_fConeSize;
	FLOAT fRndY = (penSpawner->FRnd()*2-1)*fCone*_fConeSize;
	FLOAT fRndZ = (penSpawner->FRnd()*2-1)*fCone*_fConeSize;

	FLOAT fRndH = penSpawner->FRnd();
	FLOAT fRndP = penSpawner->FRnd();
	FLOAT fRndB = penSpawner->FRnd();

	FLOAT3D vUp;
	const FLOATmatrix3D &m = penSpawner->GetRotationMatrix();
	vUp(1) = m(1,2);
	vUp(2) = m(2,2);
	vUp(3) = m(3,2);

	//FLOAT fStrength = _vSpeed.Length();

	// speed it up
	((CMovableEntity&)*penDebris).LaunchAsFreeProjectile(
		_vSpawnerSpeed+_vSpeed+FLOAT3D(fRndX, fRndY, fRndZ)+vUp*_fSpeedUp, (CMovableEntity*)penSpawner);
	((CMovableEntity&)*penDebris).SetDesiredRotation(
		ANGLE3D(fRndH*360.0f-180.0f, fRndP*360.0f-180.0f, fRndB*360.0f-180.0f));

	return penDebris;
}

CEntityPointer DebrisSka_Spawn_Independent( CEntity *penSpawner, INDEX iSkaModelComponentID,
	FLOAT fSize, CPlacement3D plAbsolute, FLOAT3D vTranslation, ANGLE3D aRotation)
{
	// create debris at same world as spawner
	CEntityPointer penDebris = penSpawner->GetWorld()->CreateEntity_t(plAbsolute, CTFILENAME("Classes\\DebrisSka.ecl"),WLD_AUTO_ENTITY_ID,FALSE);
	// prepare parameters
	ESpawnDebrisSka eSpawn;
	eSpawn.penSpawner = penSpawner;
	eSpawn.iSkaModelComponentID = iSkaModelComponentID;
	eSpawn.bImmaterialASAP=FALSE;
	eSpawn.bCustomShading=FALSE;
	eSpawn.Eeibt = _Eeibt;
	eSpawn.dptParticles = _dptParticles;
	eSpawn.betStain = _betStain;
	eSpawn.colDebris = _colDebris;
	eSpawn.fSize = fSize;
	eSpawn.vStretch = FLOAT3D(1,1,1);
	eSpawn.bFromModelHolder3 = FALSE;

	// initialize it
	penDebris->Initialize(eSpawn,FALSE);

	// move it 
	((CMovableEntity&)*penDebris).LaunchAsFreeProjectile(
		vTranslation, (CMovableEntity*)penSpawner);
	((CMovableEntity&)*penDebris).SetDesiredRotation(aRotation);

	return penDebris;
}

// info structure
static EntityInfo eiFlesh = {EIBT_FLESH};
static EntityInfo eiWater = {EIBT_WATER};
static EntityInfo eiRock  = {EIBT_ROCK };
static EntityInfo eiFire  = {EIBT_FIRE };
static EntityInfo eiAir   = {EIBT_AIR  };
static EntityInfo eiBones = {EIBT_BONES};
static EntityInfo eiWood  = {EIBT_WOOD };
static EntityInfo eiMetal = {EIBT_METAL};
static EntityInfo eiRobot = {EIBT_ROBOT};
static EntityInfo eiIce   = {EIBT_ICE  };

// get default entity info for given body type
EntityInfo *GetStdEntityInfo(EntityInfoBodyType eibt)
{
	switch(eibt) {
	case EIBT_FLESH: {return &eiFlesh; } break;
	case EIBT_WATER: {return &eiWater; } break;
	case EIBT_ROCK : {return &eiRock ; } break;
	case EIBT_FIRE : {return &eiFire ; } break;
	case EIBT_AIR  : {return &eiAir  ; } break;
	case EIBT_BONES: {return &eiBones; } break;
	case EIBT_WOOD : {return &eiWood ; } break;
	case EIBT_METAL: {return &eiMetal; } break;
	case EIBT_ROBOT: {return &eiRobot; } break;
	case EIBT_ICE  : {return &eiIce  ; } break;
	default:    {return NULL;} break;
	};
}



/************************************************************
 *                 DAMAGE CONTROL FUNCTIONS                 *
 ************************************************************/
// damage control functions
FLOAT DamageStrength(EntityInfoBodyType eibtBody, enum DamageType dtDamage)
{
	switch(eibtBody) {
	case EIBT_FLESH:
		return 1.0f;
	case EIBT_WATER:
		switch(dtDamage) {
		case DMT_CLOSERANGE:  return 0.0f;
		case DMT_BURNING:  return 0.0f;
		case DMT_DROWNING: return 0.0f;
		}
		return 1.0f;
	case EIBT_ROCK :
		switch(dtDamage) {
		case DMT_CLOSERANGE:  return 0.0f;
		case DMT_BURNING:   return 0.0f;
		case DMT_FREEZING:  return 0.0f;
		}
		return 1.0f;
	case EIBT_ICE :
		switch(dtDamage) {
		case DMT_CLOSERANGE:  return 0.5f;
		case DMT_BURNING:  return 3.0f;
		case DMT_FREEZING:  return 0.0f;
		}
		return 1.0f;
	case EIBT_FIRE :
		switch(dtDamage) {
		case DMT_CLOSERANGE:  return 0.5f;
		case DMT_BURNING:   return 0.0f;
		}
		return 1.0f;
	case EIBT_AIR  :
		switch(dtDamage) {
		case DMT_CLOSERANGE:  return 0.0f;
		case DMT_BURNING:   return 0.5f;
		}
		return 1.0f;
	case EIBT_BONES:
		switch(dtDamage) {
		case DMT_FREEZING:  return 0.0f;
		}
		return 1.0f;
	case EIBT_WOOD :
		switch(dtDamage) {
		case DMT_FREEZING:  return 0.0f;
		}
		return 1.0f;
	case EIBT_METAL:
		switch(dtDamage) {
		case DMT_CLOSERANGE:  return 0.0f;
		case DMT_BURNING:   return 0.0f;
		case DMT_FREEZING:  return 0.0f;
		}
		return 1.0f;
	case EIBT_ROBOT:
		switch(dtDamage) {
		case DMT_CLOSERANGE:return 0.5f;
		case DMT_BURNING:   return 0.5f;
		case DMT_FREEZING:  return 0.5f;
		}
		return 1.0f;
	default:
		ASSERT(FALSE);
		return 1.0f;
	}
}

/*
// Print center screen message
void PrintCenterMessage(CEntity *penThis, CEntity *penCaused, 
	const CTString &strMessage, TIME tmLength, enum MessageSound mssSound)
{
	penCaused = FixupCausedToPlayer(penThis, penCaused);

	ECenterMessage eMsg;
	eMsg.strMessage = strMessage;
	eMsg.tmLength = tmLength;
	eMsg.mssSound = mssSound;
	penCaused->SendEvent(eMsg);
}
*/


// i.e. weapon sound when fireing or exploding
void SpawnRangeSound( CEntity *penPlayer, CEntity *penPos, enum SoundType st, FLOAT fRange)
{
	// if not really player
	if (!IsDerivedFromClass(penPlayer, "Player")) {
		// do nothing
		return;
	}
	// sound event
	ESound eSound;
	eSound.EsndtSound = st;
	eSound.penTarget = penPlayer;
	penPos->SendEventInRange( eSound, FLOATaabbox3D(penPos->GetPlacement().pl_PositionVector, fRange));
}

// get some player for trigger source if any is existing
CEntity *FixupCausedToPlayer(CEntity *penThis, CEntity *penCaused, BOOL bWarning/*=TRUE*/)
{
	if (penCaused!=NULL && IsOfClass(penCaused, "Player")) {
		return penCaused;
	}

	if (bWarning && (ent_bReportBrokenChains || GetSP()->sp_bQuickTest)) {
		CPrintF(TRANS("WARNING: Triggering chain broken, entity: %s-%s(%s)\n"), 
			(const char*)penThis->GetName(),
			(const char*)penThis->GetDescription(),
			(const char*)penThis->GetClass()->GetName());
	}

	INDEX ctPlayers = penThis->GetMaxPlayers();
	if (ctPlayers==0) {
		return NULL;
	}

	CEntity *penClosestPlayer = NULL;
	FLOAT fClosestPlayer = UpperLimit(0.0f);

	// for all players
	for (INDEX iPlayer=0; iPlayer<penThis->GetMaxPlayers(); iPlayer++) {
		CEntity *penPlayer = penThis->GetPlayerEntity(iPlayer);
		// if player exists
		if (penPlayer!=NULL) {
			// calculate distance to player
			FLOAT fDistance = 
				(penPlayer->GetPlacement().pl_PositionVector-penThis->GetPlacement().pl_PositionVector).Length();
			// update if closer
			if (fDistance<fClosestPlayer) {
				fClosestPlayer = fDistance;
				penClosestPlayer = penPlayer;
			}
		}
	}
	return penClosestPlayer;
}

// precisely lerp between two placement using quaternions
CPlacement3D LerpPlacementsPrecise(const CPlacement3D &pl0, const CPlacement3D &pl1, FLOAT fRatio)
{
	CPlacement3D pl;

	FLOATquat3D q0; q0.FromEuler(pl0.pl_OrientationAngle);
	FLOATquat3D q1; q1.FromEuler(pl1.pl_OrientationAngle);
	FLOAT3D v0 = pl0.pl_PositionVector;
	FLOAT3D v1 = pl1.pl_PositionVector;

	FLOATquat3D q = Slerp(fRatio, q0, q1);
	FLOAT3D v = Lerp(v0, v1, fRatio);

	pl.pl_PositionVector = v;

	FLOATmatrix3D m;
	q.ToMatrix(m);
	DecomposeRotationMatrixNoSnap(pl.pl_OrientationAngle, m);

	return pl;
}

FLOAT GetGameDamageMultiplier(void)
{
	FLOAT fGameDamageMultiplier = 1.0f;
	FLOAT fExtraStrength = GetSP()->sp_fExtraEnemyStrength;
	if (fExtraStrength>0) {
		fGameDamageMultiplier*=1.0f/(1+fExtraStrength);
	}
	FLOAT fExtraStrengthPerPlayer = GetSP()->sp_fExtraEnemyStrengthPerPlayer;
	if (fExtraStrengthPerPlayer>0) {
		INDEX ctPlayers = _pNetwork->ga_sesSessionState.GetPlayersCount();
		fGameDamageMultiplier*=1.0f/(1+fExtraStrengthPerPlayer*ClampDn(ctPlayers-1L, 0L));
	}
	/*
	if (GetSP()->sp_gdGameDifficulty==CSessionProperties::GD_TOURIST) 
	{
		fGameDamageMultiplier *= 2.0f;
	}
	*/
	return fGameDamageMultiplier;
}


// get entity's serious damage multiplier
FLOAT GetSeriousDamageMultiplier( CEntity *pen)
{
	if( !IsOfClass(pen,"Player")) return 1.0f;
	const TIME tmNow = ((CPlayer*)pen)->en_tmEntityTime;
	const TIME tmDamage = ((CPlayer*)pen)->m_tmSeriousDamage;
	if( tmDamage>tmNow) return 4.0f;
	return 1.0f;
}

class CWorldSettingsController *GetWSC(CEntity *pen)
{
	CWorldSettingsController *pwsc = NULL;
	// obtain bcg viewer
	class CBackgroundViewer *penBcgViewer = (CBackgroundViewer *) pen->GetWorld()->GetBackgroundViewer();
	if( penBcgViewer != NULL) {
		// obtain world settings controller 
		pwsc = (CWorldSettingsController *) &*penBcgViewer->m_penWorldSettingsController;
	}
	return pwsc;
}

/*
void SendScoreToEntity(CEntity *penTarget, INDEX iScore)
{
	EReceiveScore ers;
	ers.iPoints = iScore;
	penTarget->SendEvent(ers);
}
*/

void ska_CreateAnimIDFromString(INDEX *piID, CModelInstance *pmi, CTString strAnimName, BOOL bRecursive /* = FALSE*/)
{
	// ignore velues that have allready been initialised
	if ((*piID)!=-1) {
		return;
	}

	INDEX iAnimID, iDummy1, iDummy2;    
	iAnimID = ska_GetIDFromStringTable(strAnimName);
	
	if (pmi->FindAnimationByID(iAnimID, &iDummy1, &iDummy2)) {
		*piID = iAnimID;
		return;
	} else if (bRecursive) {
		//test all children
		for(INDEX icmi=0; icmi<pmi->mi_cmiChildren.Count(); icmi++) {
			ska_CreateAnimIDFromString(&iAnimID, &(pmi->mi_cmiChildren[icmi]), strAnimName, TRUE);
			if (pmi->FindAnimationByID(iAnimID, &iDummy1, &iDummy2)) {
				*piID = iAnimID;
				return; 
			}
		}
	} else {
		CPrintF("WARNING: Animation set for model '%s' does not contain a required animation #%d('%s').\n\r\
Animation behaveour will be undefined.\n", pmi->GetName(), iAnimID, strAnimName);
		//untested!! - return any ID, just so that we don't crash
		iAnimID = pmi->FindFirstAnimationID();
		if (iAnimID == -1) {
			CPrintF("WARNING: No animations in ModelInstance '%s' ('%s').\n\r",
			pmi->GetName(), strAnimName);    
		}    
	}
	*piID = iAnimID;
}

void ska_CreateBoneIDFromString(INDEX *piID, CModelInstance *pmi, CTString strBoneName)
{
	// ignore velues that have allready been initialised
	if ((*piID)!=-1) {
		return;
	}

	INDEX iBoneID;
	iBoneID = ska_GetIDFromStringTable(strBoneName);
	
	// if the bone doesn't exist, inform the user and return -1 for iBoneID
	if (pmi->mi_psklSkeleton->FindBoneInLOD(iBoneID, 0)==-1)
	{
		WarningMessage("WARNING: Bone #%d('%s') not found in model '%s' skeleton.\n\r\
Skeleton behaveour will be undefined.\n", iBoneID, strBoneName, pmi->GetName());
		iBoneID = -1;
	}
	*piID = iBoneID;
	return;
}

void ska_CreateCollisionIDFromString(INDEX *piID, CModelInstance *pmi, CTString strCollisionName)
{
	// ignore velues that have allready been initialised
	if ((*piID)!=-1) {
		return;
	}

	INDEX iBoxID;
	iBoxID = ska_GetIDFromStringTable(strCollisionName);
	if (pmi->GetColisionBoxIndex(iBoxID)==-1)
	{
		WarningMessage("WARNING: Collision Box #%d('%s') not found in model '%s.\n\r\
Behaveour will be undefined.\n", iBoxID, strCollisionName, pmi->GetName());
		iBoxID = -1;
	}
	*piID = iBoxID;
	return;
}

void ska_CreateChildIDFromString(INDEX *piID, CModelInstance *pmi, CTString strChildName, BOOL bRecursive /*= FALSE*/)
{
	// ignore velues that have allready been initialised
	if ((*piID)!=-1) {
		return;
	}

	INDEX iChildID;
	iChildID = ska_GetIDFromStringTable(strChildName);

	extern BOOL _bWorldEditorApp;
	if (pmi->GetChild(iChildID, bRecursive)==NULL && (_pNetwork->IsServer()||_bWorldEditorApp))
	{
		WarningMessage("WARNING: Child #%d('%s') not found in model '%s'.\n\r\
Behaveour will be undefined.\n", iChildID, strChildName, pmi->GetName());
		ASSERT(FALSE);
		iChildID = -1;
	}
	*piID = iChildID;
	return;  
}

INDEX CollisionBoxIndexFromID(CModelInstance *pmi, INDEX ID)
{
	return pmi->GetColisionBoxIndex(ID);  
}

// call this only during AdjustBones() entity function !!!!!
BOOL RotateBone(INDEX idBone, ANGLE3D a)
{
	RenBone *rb = RM_FindRenBone(idBone);
	if(rb!=NULL) {
		FLOATquat3D quat;
		quat.FromEuler(a);
		rb->rb_arRot.ar_qRot = quat*rb->rb_arRot.ar_qRot;
		return TRUE;
	}
	/* this bone does not exist!!! */
	SKAASSERT(FALSE);
	return FALSE;
}

BOOL OffsetBone(INDEX idBone, FLOAT3D v)
{
	RenBone *rb = RM_FindRenBone(idBone);
	if(rb!=NULL) {
		rb->rb_apPos.ap_vPos += v;
		return TRUE;
	}
	/* this bone does not exist!!! */
	SKAASSERT(FALSE);
	return FALSE;
}

void RemoveSkaChildren(CModelInstance *pmi)
{
	//INDEX ctch = pmi->mi_cmiChildren.Count();
	while (TRUE)
	{
		INDEX ctch = pmi->mi_cmiChildren.Count();
		if (ctch==0) { 
			break;
		}
		// remove child
		CModelInstance *chmi = &pmi->mi_cmiChildren[ctch-1];
		pmi->DeleteChild(chmi);
	} 
}

INDEX AddSkaChild(CEntity *penThis, CEntity *penParent, INDEX iSkaModelComponentID,
									const CTString &strChildName, INDEX iParentBoneID) 
{
	ASSERT(penThis!=NULL);
	ASSERT(penParent!=NULL);  
	ASSERT(penParent->en_RenderType == CEntity::RT_SKAMODEL ||
				 penParent->en_RenderType == CEntity::RT_SKAEDITORMODEL);
	ASSERT(penParent->en_pmiModelInstance!=NULL);
	
	CModelInstance *pmiParent = penParent->en_pmiModelInstance;
	return AddSkaChild(penThis, pmiParent, iSkaModelComponentID, strChildName, iParentBoneID);
}

INDEX AddSkaChild(CEntity *penThis, CModelInstance *pmiParent, INDEX iSkaModelComponentID,
									const CTString &strChildName, INDEX iParentBoneID)
{
	ASSERT(penThis!=NULL);
	ASSERT(pmiParent!=NULL);
	CModelInstance *pmiChild = CreateModelInstance("");

	try {
		CModelInstance *pmiComponent = penThis->GetModelInstanceForComponent(iSkaModelComponentID);
		ASSERT(pmiComponent!=NULL);
		pmiChild->Copy(*pmiComponent);
	} catch (char *strError) {
		const CTFileName &fnSMCFileName = penThis->FileNameForComponent(ECT_SKAMODEL,iSkaModelComponentID);
		FatalError(TRANS("Cannot load SMC model '%s': %s"), fnSMCFileName, strError);
	}

	// if we want a different name then the one in .SMC
	if (strChildName!="") {
		pmiChild->SetName(strChildName);
	}

	INDEX iChildID = ska_GetIDFromStringTable(pmiChild->GetName());
	// for debugging purposes: don't allow two children with same names
#ifndef NDEBUG
	CModelInstance *pmiDebug = pmiParent->GetChild(iChildID);
	ASSERT(pmiDebug==NULL);     
#endif

	// Add new child to parent model 
	pmiParent->AddChild(pmiChild);

	// Set child parent bone ID
	if(iParentBoneID!=-1) {
		pmiChild->SetParentBone(iParentBoneID);  
	}  
	
	return iChildID;
}

void AddSkaChildFromMI(CModelInstance *pmiParent, CModelInstance *pmi, const CTString &strChildName, INDEX iParentBoneID)
{
	ASSERT(pmiParent!=NULL);
	ASSERT(pmi!=NULL);

	CModelInstance *pmiChild = CreateModelInstance("");
	pmiChild->Copy(*pmi);  
	pmiChild->NewClearState(0.0f);
	// Add new child to parent model 
	pmiParent->AddChild(pmiChild);
	pmiChild->SetName(strChildName);
	
	if (iParentBoneID!=-1) {
		pmiChild->SetParentBone(iParentBoneID);  
	}  
}

BOOL RemoveSkaChild(CEntity *penParent, INDEX iChildID)
{
	ASSERT(penParent!=NULL);  
	ASSERT(penParent->en_RenderType == CEntity::RT_SKAMODEL ||
				 penParent->en_RenderType == CEntity::RT_SKAEDITORMODEL);
	
	CModelInstance *miChild = NULL;
	miChild = penParent->GetModelInstance()->GetChild(iChildID);
	if (miChild==NULL) {
		return FALSE;
	}
	penParent->GetModelInstance()->DeleteChild(miChild);
	return TRUE;
}

BOOL RemoveSkaChild(CEntity *penParent, CTString strChildName)
{
	ASSERT(penParent!=NULL);  
	ASSERT(penParent->en_RenderType == CEntity::RT_SKAMODEL ||
				 penParent->en_RenderType == CEntity::RT_SKAEDITORMODEL);
	
	CModelInstance *miChild = NULL;
	INDEX iChildID = ska_GetIDFromStringTable(strChildName);
	miChild = penParent->GetModelInstance()->GetChild(iChildID);
	if (miChild==NULL) {
		return FALSE;
	}
	penParent->GetModelInstance()->DeleteChild(miChild);
	return TRUE;
}


//0424
BOOL RotateSkaChild(CEntity *penParent, CTString strChildName)
{
	ASSERT(penParent!=NULL);  
	ASSERT(penParent->en_RenderType == CEntity::RT_SKAMODEL ||
				 penParent->en_RenderType == CEntity::RT_SKAEDITORMODEL);
	
	CModelInstance *miChild = NULL;
	INDEX iChildID = ska_GetIDFromStringTable(strChildName);
	miChild = penParent->GetModelInstance()->GetChild(iChildID);
	if (miChild==NULL) {
		return FALSE;
	}

	ANGLE3D CurrentAngle;
	CurrentAngle = miChild->GetOffsetRot();	
	CurrentAngle(1) = CurrentAngle(1) + 30.0f;
	miChild->SetOffsetRot(CurrentAngle);	
	
	//penParent->GetModelInstance()->DeleteChild(miChild);
	return TRUE;
}

// this is only correct for FOV of 90
FLOAT MipFactor_LogToMetric(FLOAT fMipLog)
{
	return pow(2, fMipLog)/3.2f;
}

// this is only correct for FOV of 90
FLOAT MipFactor_MetricToLog(FLOAT fMipMetric)
{
	return Log2(3.2f*fMipMetric);
}

void AddToAutoAimables(CEntity *pen)
{
	CEntity *penMainMusicHolder = _pNetwork->GetEntityOfClass("MusicHolder", 0);
	// add to list of enemies in music holder
	if (penMainMusicHolder!=NULL) {
		CStaticStackArray<ULONG> *paulAutoAimables = &(((CMusicHolder *)penMainMusicHolder)->m_aulAutoAimables);
		BOOL bEntityInArray = FALSE;
		for (INDEX i=0; i<paulAutoAimables->Count(); i++) {
			if ((*paulAutoAimables)[i]==pen->en_ulID)
			{
				bEntityInArray = TRUE;
				break;
			}
		}
		// if entity wasn't found, add it to the first empty position
		if (!bEntityInArray) {
			BOOL bAddedToEmpty = FALSE;
			for (INDEX i=0; i<paulAutoAimables->Count(); i++) {
				if ((*paulAutoAimables)[i]==-1)
				{
					(*paulAutoAimables)[i] = pen->en_ulID;
					bAddedToEmpty = TRUE;
					break;
				}
			}
			if (!bAddedToEmpty) {
				paulAutoAimables->Push() = pen->en_ulID;
			}      
		}
	}    
}

void RemoveFromAutoAimables(CEntity *pen) 
{
	// remove from list of enemies in music holder
	CEntity *penMainMusicHolder = _pNetwork->GetEntityOfClass("MusicHolder", 0);
	if (penMainMusicHolder!=NULL) {
		CStaticStackArray<ULONG> *paulAutoAimables = &(((CMusicHolder *)penMainMusicHolder)->m_aulAutoAimables);
		for (INDEX i=0; i<paulAutoAimables->Count(); i++) {
			if ((*paulAutoAimables)[i]==pen->en_ulID)
			{
				(*paulAutoAimables)[i] = -1;
			}
		}    
	}    
}

void ScaleEntityInfo(EntityInfo *eiInfo, FLOAT fScale) {
	eiInfo->vSourceCenter[0] *= fScale;
	eiInfo->vSourceCenter[1] *= fScale;
	eiInfo->vSourceCenter[2] *= fScale;
	eiInfo->vTargetCenter[0] *= fScale;
	eiInfo->vTargetCenter[1] *= fScale;
	eiInfo->vTargetCenter[2] *= fScale;
}

INDEX dbg_bEnemyKillTest = FALSE;
CDynamicContainer<EnemyKillData> _dcKillData;
void AddEnemyKillData(class CEnemyBase *penEnemy, class CEnemySpawner *penSpawner)
{
	if(!dbg_bEnemyKillTest) return;
	EnemyKillData *pekd=new EnemyKillData;
	pekd->ekd_bSpawner=FALSE;
	pekd->ekd_ID=penEnemy->en_ulID;
	pekd->ekd_strClass=penEnemy->GetClass()->ec_pdecDLLClass->dec_strName;
	pekd->ekd_strName=penEnemy->GetName();
	if(penSpawner!=NULL)
	{
		pekd->ekd_IDSpawner=penSpawner->en_ulID;
		pekd->ekd_strNameSpawner=penSpawner->GetName();
	}
	_dcKillData.Add(pekd);
}

void AddSpawnerKillData(class CEnemySpawner *penSpawner, INDEX ctEnemies)
{
	if(!dbg_bEnemyKillTest) return;
	EnemyKillData *pekd=new EnemyKillData;
	pekd->ekd_bSpawner=TRUE;
	pekd->ekd_ID=penSpawner->en_ulID;
	pekd->ekd_strName=penSpawner->GetName();
	pekd->ekd_ctToSpawn=ctEnemies;
	if(penSpawner->m_penTarget!=NULL)
	{
		pekd->ekd_strSpawnClass=penSpawner->m_penTarget->GetClass()->ec_pdecDLLClass->dec_strName;
		pekd->ekd_strSpawnName=penSpawner->m_penTarget->GetName();
	}
	_dcKillData.Add(pekd);
}

void MarkEnemyDead(class CEnemyBase *penEnemy)
{
	if(!dbg_bEnemyKillTest) return;
	for(INDEX iEnemy=0; iEnemy<_dcKillData.Count(); iEnemy++)
	{
		EnemyKillData *pekd=&_dcKillData[iEnemy];
		if(!pekd->ekd_bSpawner && pekd->ekd_ID==penEnemy->en_ulID)
		{
			if(pekd->ekd_bKilled)
			{
				if(pekd->ekd_IDSpawner!=-1)
				{
					CPrintF("!!! Warning !!! Enemy of class \"%s\" spawned by spawner with ID %d counted twice !!!\n",
						pekd->ekd_strName, pekd->ekd_IDSpawner);
				}
				else
				{
					CPrintF("!!! Warning !!! Enemy named \"%s\" of class \"%s\" with ID %d counted twice !!!\n",
						pekd->ekd_strName, pekd->ekd_strClass, pekd->ekd_ID);
				}
			}
			pekd->ekd_bKilled=TRUE;
			return;
		}
	}
}

void MarkEnemyDead(class CCharacterBase *penEnemy)
{
	if(!dbg_bEnemyKillTest) return;
	for(INDEX iEnemy=0; iEnemy<_dcKillData.Count(); iEnemy++)
	{
		EnemyKillData *pekd=&_dcKillData[iEnemy];
		if(!pekd->ekd_bSpawner && pekd->ekd_ID==penEnemy->en_ulID)
		{
			if(pekd->ekd_bKilled)
			{
				if(pekd->ekd_IDSpawner!=-1)
				{
					CPrintF("!!! Warning !!! Enemy of class \"%s\" spawned by spawner with ID %d counted twice !!!\n",
						pekd->ekd_strName, pekd->ekd_IDSpawner);
				}
				else
				{
					CPrintF("!!! Warning !!! Enemy named \"%s\" of class \"%s\" with ID %d counted twice !!!\n",
						pekd->ekd_strName, pekd->ekd_strClass, pekd->ekd_ID);
				}
			}
			pekd->ekd_bKilled=TRUE;
			return;
		}
	}
}

void MarkEnemyDead(class CPetBase *penEnemy)
{
	if(!dbg_bEnemyKillTest) return;
	for(INDEX iEnemy=0; iEnemy<_dcKillData.Count(); iEnemy++)
	{
		EnemyKillData *pekd=&_dcKillData[iEnemy];
		if(!pekd->ekd_bSpawner && pekd->ekd_ID==penEnemy->en_ulID)
		{
			if(pekd->ekd_bKilled)
			{
				if(pekd->ekd_IDSpawner!=-1)
				{
					CPrintF("!!! Warning !!! Enemy of class \"%s\" spawned by spawner with ID %d counted twice !!!\n",
						pekd->ekd_strName, pekd->ekd_IDSpawner);
				}
				else
				{
					CPrintF("!!! Warning !!! Enemy named \"%s\" of class \"%s\" with ID %d counted twice !!!\n",
						pekd->ekd_strName, pekd->ekd_strClass, pekd->ekd_ID);
				}
			}
			pekd->ekd_bKilled=TRUE;
			return;
		}
	}
}

void MarkEnemyDead(class CSlaveBase *penEnemy)
{
	if(!dbg_bEnemyKillTest) return;
	for(INDEX iEnemy=0; iEnemy<_dcKillData.Count(); iEnemy++)
	{
		EnemyKillData *pekd=&_dcKillData[iEnemy];
		if(!pekd->ekd_bSpawner && pekd->ekd_ID==penEnemy->en_ulID)
		{
			if(pekd->ekd_bKilled)
			{
				if(pekd->ekd_IDSpawner!=-1)
				{
					CPrintF("!!! Warning !!! Enemy of class \"%s\" spawned by spawner with ID %d counted twice !!!\n",
						pekd->ekd_strName, pekd->ekd_IDSpawner);
				}
				else
				{
					CPrintF("!!! Warning !!! Enemy named \"%s\" of class \"%s\" with ID %d counted twice !!!\n",
						pekd->ekd_strName, pekd->ekd_strClass, pekd->ekd_ID);
				}
			}
			pekd->ekd_bKilled=TRUE;
			return;
		}
	}
}

void MarkEnemyDead(class CWildPetBase *penEnemy)
{
	if(!dbg_bEnemyKillTest) return;
	for(INDEX iEnemy=0; iEnemy<_dcKillData.Count(); iEnemy++)
	{
		EnemyKillData *pekd=&_dcKillData[iEnemy];
		if(!pekd->ekd_bSpawner && pekd->ekd_ID==penEnemy->en_ulID)
		{
			if(pekd->ekd_bKilled)
			{
				if(pekd->ekd_IDSpawner!=-1)
				{
					CPrintF("!!! Warning !!! Enemy of class \"%s\" spawned by spawner with ID %d counted twice !!!\n",
						pekd->ekd_strName, pekd->ekd_IDSpawner);
				}
				else
				{
					CPrintF("!!! Warning !!! Enemy named \"%s\" of class \"%s\" with ID %d counted twice !!!\n",
						pekd->ekd_strName, pekd->ekd_strClass, pekd->ekd_ID);
				}
			}
			pekd->ekd_bKilled=TRUE;
			return;
		}
	}
}

void MarkSpawnerSpawned(class CEnemySpawner *penSpawner)
{
	if(!dbg_bEnemyKillTest) return;
	for(INDEX iSpawner=0; iSpawner<_dcKillData.Count(); iSpawner++)
	{
		EnemyKillData *pekd=&_dcKillData[iSpawner];
		if(pekd->ekd_bSpawner && pekd->ekd_ID==penSpawner->en_ulID)
		{
			if(pekd->ekd_ctToSpawn<=0)
			{
				CPrintF("!!! Warning !!! Spawner named \"%s\" with ID %d spawned more enemies than supposed !!!\n",
					pekd->ekd_strName, pekd->ekd_ID);
			}
			pekd->ekd_ctToSpawn--;
			return;
		}
	}
}

void DumpKillData(void)
{
	if(!dbg_bEnemyKillTest) return;
	CPrintF("-----------------------------------------------------------------------------------------\n");
	CPrintF("Zombie dump data:\n\n");
	for(INDEX iKD=0; iKD<_dcKillData.Count(); iKD++)
	{
		EnemyKillData *pekd=&_dcKillData[iKD];
		if(pekd->ekd_bSpawner)
		{
			if(pekd->ekd_ctToSpawn!=0)
			{
				CPrintF("Spawner named \"%s\" with ID %d: yet to spawn %d enemies of class \"%s\", named: \"%s\"\n",
					pekd->ekd_strName, pekd->ekd_ID, pekd->ekd_ctToSpawn, pekd->ekd_strSpawnClass, pekd->ekd_strSpawnName);
			}
		}
		else if( !pekd->ekd_bKilled)
		{
			if(pekd->ekd_IDSpawner!=-1)
			{
				CPrintF("Enemy spawned by spawner named: \"%s\" with ID %d , not counted as dead\n",
					pekd->ekd_strNameSpawner, pekd->ekd_IDSpawner);
			}
			else
			{
				CPrintF("Enemy named \"%s\" of class \"%s\" with ID %d not counted as dead\n",
					pekd->ekd_strName, pekd->ekd_strClass, pekd->ekd_ID);
			}
		}
	}
}
