/*
 * Entity that can move and obey physics.
 */
1
%{
#include "StdH.h"
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Math/Geometry.inl>
#include <Engine/Math/Float.h>
#include <Engine/Base/Stream.h>
#include <Engine/World/World.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Network/EntityHashing.h>
#include <Engine/Entities/EntityCollision.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/BSP.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/World/WorldSettings.h>
#include <Engine/World/WorldCollision.h>
#include <Engine/Math/Clipping.inl>
#include <Engine/Light/LightSource.h>
#include <Engine/Entities/LastPositions.h>
#include <Engine/Templates/StaticStackArray.cpp>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Base/Console.h>
#include <Engine/Base/CRC.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Terrain/Terrain.h>
#define CLEARMEM(var) memset(&var, 0, sizeof(var))

%}


%{

#define ANYEXCEPTION  ...
template CStaticStackArray<CBrushPolygon*>;

#define MAXCOLLISIONRETRIES 4*4
extern FLOAT phy_fCollisionCacheAhead;
extern FLOAT phy_fCollisionCacheAround;

//0211
#define AN_LOOPING              (1UL<<0) // looping animation
#define AN_NORESTART            (1UL<<1) // dont restart anim
#define AN_PAUSED				(1UL<<2)
#define AN_CLEAR                (1UL<<3) // do new clear state before adding animation

// force breakpoint (debug)
extern INDEX dbg_bBreak;
// must be in separate function to disable stupid optimizer
extern void Breakpoint(void); 

// add acceleration to velocity
static inline void AddAcceleration(
	FLOAT3D &vCurrentVelocity, const FLOAT3D &vDesiredVelocity, 
	FLOAT fAcceleration, FLOAT fDecceleration)
{
	// if desired velocity is smaller than current velocity
	if (vDesiredVelocity.Length()<vCurrentVelocity.Length()) {
		fAcceleration=fDecceleration;
	}
	// find difference between current and desired velocities
	FLOAT3D vDelta = vDesiredVelocity-vCurrentVelocity;
	// accelerate in the direction of the difference with given maximum acceleration
	FLOAT fDelta = vDelta.Length();
	if (fDelta>fAcceleration) {
		vCurrentVelocity += vDelta*(fAcceleration/fDelta);
	} else {
		vCurrentVelocity = vDesiredVelocity;
	}
}

// add gravity acceleration to velocity along an axis
static inline void AddGAcceleration(
	FLOAT3D &vCurrentVelocity, const FLOAT3D &vGDir, 
	FLOAT fGA, FLOAT fGV)
{
	// disassemble speed
	FLOAT3D vCurrentParallel, vCurrentOrthogonal;
	GetParallelAndNormalComponents(vCurrentVelocity, vGDir, vCurrentOrthogonal, vCurrentParallel);

	// if not already going down at max speed
	if (! (vCurrentOrthogonal%vGDir>=fGV)) {
		// add accelleration to parallel speed
		vCurrentOrthogonal+=vGDir*fGA;

		// if going down at max speed
		if (vCurrentOrthogonal%vGDir>=fGV) {
			// clamp
			vCurrentOrthogonal = vGDir*fGV;
		}
	}

	// assemble speed back
	vCurrentVelocity = vCurrentParallel+vCurrentOrthogonal;
}

// NOTE:
// this is pulled out into a separate function because, otherwise, VC6 generates
// invalid code when optimizing this. no clue why is that so.

#pragma inline_depth(0)
static void CheckAndAddGAcceleration(CMovableEntity *pen, FLOAT3D &vTranslationAbsolute, FLOAT fTickQuantum)
{
	// if there is forcefield involved
	if (pen->en_fForceA>0.01f) {
		// add force acceleration
		FLOAT fGV=pen->en_fForceV*fTickQuantum;
		FLOAT fGA=pen->en_fForceA*fTickQuantum*fTickQuantum;
		AddGAcceleration(vTranslationAbsolute, pen->en_vForceDir, fGA, fGV);
	}
}
#pragma inline_depth()  // see important note above


// add acceleration to velocity, but only along a plane
static inline void AddAccelerationOnPlane(
	FLOAT3D &vCurrentVelocity, const FLOAT3D &vDesiredVelocity, 
	FLOAT fAcceleration, FLOAT fDecceleration,
	const FLOAT3D &vPlaneNormal)
{
	FLOAT3D vCurrentParallel, vCurrentOrthogonal;
	GetParallelAndNormalComponents(vCurrentVelocity, vPlaneNormal, vCurrentOrthogonal, vCurrentParallel);
	FLOAT3D vDesiredParallel;
	GetNormalComponent(vDesiredVelocity, vPlaneNormal, vDesiredParallel);
	AddAcceleration(vCurrentParallel, vDesiredParallel, fAcceleration, fDecceleration);
	vCurrentVelocity = vCurrentParallel+vCurrentOrthogonal;
}

// add acceleration to velocity, for roller-coaster slope -- slow!
static inline void AddAccelerationOnPlane2(
	FLOAT3D &vCurrentVelocity, const FLOAT3D &vDesiredVelocity, 
	FLOAT fAcceleration, FLOAT fDecceleration,
	const FLOAT3D &vPlaneNormal, const FLOAT3D &vGravity)
{
	// get down and horizontal direction
	FLOAT3D vDn;
	GetNormalComponent(vGravity, vPlaneNormal, vDn);
	vDn.Normalize();
	FLOAT3D vRt = vPlaneNormal*vDn;
	vRt.Normalize();

	// add only horizontal acceleration
	FLOAT3D vCurrentParallel, vCurrentOrthogonal;
	GetParallelAndNormalComponents(vCurrentVelocity, vRt, vCurrentParallel, vCurrentOrthogonal);
	FLOAT3D vDesiredParallel;
	GetParallelComponent(vDesiredVelocity, vRt, vDesiredParallel);
	AddAcceleration(vCurrentParallel, vDesiredParallel, fAcceleration, fDecceleration);
	vCurrentVelocity = vCurrentParallel+vCurrentOrthogonal;
}

// max number of retries during movement
static INDEX _ctTryToMoveCheckCounter;
static INDEX _ctSliding;
static FLOAT3D _vSlideOffDir;   // move away direction for sliding
static FLOAT3D _vSlideDir;
static void InitTryToMove(void)
{
	_ctTryToMoveCheckCounter = MAXCOLLISIONRETRIES;
	_ctSliding = 0;
	_vSlideOffDir = FLOAT3D(0,0,0);
	_vSlideDir = FLOAT3D(0,0,0);
}

// array of forces for current entity
class CEntityForce {
public:
	CEntityPointer ef_penEntity;
	INDEX ef_iForceType;
	FLOAT ef_fRatio;    // how much of entity this force gets [0-1]
	inline void Clear(void) {
		ef_penEntity = NULL;
	};
	~CEntityForce(void) {
		Clear();
	};
};                                                            
static CStaticStackArray<CEntityForce> _aefForces;

void ClearMovableEntityCaches(void)
{
	_aefForces.Clear();
}

%}

class export CMovableEntity : CRationalEntity {
name      "MovableEntity";
thumbnail "";

properties:

	// NOTE: all properties that are not marked as 'adjustable' should be treated as read-only

	// translation and rotation speed that this entity would like to have (in relative system)
 1 FLOAT3D en_vDesiredTranslationRelative = FLOAT3D(0.0f,0.0f,0.0f),
 2 ANGLE3D en_aDesiredRotationRelative = ANGLE3D(0,0,0),

	// translation and rotation speed that this entity currently has in absolute system
 3 FLOAT3D en_vCurrentTranslationAbsolute = FLOAT3D(0.0f,0.0f,0.0f),
 4 ANGLE3D en_aCurrentRotationAbsolute = ANGLE3D(0,0,0),

 6 CEntityPointer en_penReference, // reference entity (for standing on)
 7 FLOAT3D en_vReferencePlane = FLOAT3D(0.0f,0.0f,0.0f),   // reference plane (only for standing on)
 8 INDEX en_iReferenceSurface = 0,     // surface on reference entity
 9 CEntityPointer en_penLastValidReference,  // last valid reference entity (for impact damage)
 15 CEntityPointer en_penLastValidReferenceTmp,  //0120
 14 FLOAT en_tmLastSignificantVerticalMovement = 0.0f,   // last time entity moved significantly up/down
	// swimming parameters
 10 FLOAT en_tmLastBreathed = 0,        // last time when entity took some air
 11 FLOAT en_tmMaxHoldBreath = 5.0f,    // how long can entity be without air (adjustable)
 12 FLOAT en_fDensity = 5000.0f,        // density of the body [kg/m3] - defines buoyancy (adjustable)
 13 FLOAT en_tmLastSwimDamage = 0,      // last time when entity was damaged by swimming
 // content immersion parameters
 20 INDEX en_iUpContent = 0,
 21 INDEX en_iDnContent = 0,
 22 FLOAT en_fImmersionFactor = 1.0f,
 // force parameters
 25 FLOAT3D en_vGravityDir = FLOAT3D(0,-1,0),
 26 FLOAT en_fGravityA = 0.0f,
 27 FLOAT en_fGravityV = 0.0f,
 66 FLOAT3D en_vForceDir = FLOAT3D(1,0,0),
 67 FLOAT en_fForceA = 0.0f,
 68 FLOAT en_fForceV = 0.0f,
 // jumping parameters
 30 FLOAT en_tmJumped = -1.0f,         // time when entity jumped
 31 FLOAT en_tmMaxJumpControl = 0.5f,  // how long after jump can have control in the air [s] (adjustable)
 32 FLOAT en_fJumpControlMultiplier = 0.5f,  // how good is control when jumping (adjustable)
 // movement parameters
 35 FLOAT en_fAcceleration = 200.0f,  // acc/decc [m/s2] in ideal situation (adjustable)
 36 FLOAT en_fDeceleration = 40.0f,

 37 FLOAT en_fStepUpHeight = 1.0f,        // how high can entity step upstairs (adjustable)
 42 FLOAT en_fStepDnHeight = -1.0f,       // how low can entity step (negative means don't check) (adjustable)
 38 FLOAT en_fBounceDampParallel = 0.5f,  // damping parallel to plane at each bounce (adjustable)
 39 FLOAT en_fBounceDampNormal   = 0.5f,  // damping normal to plane damping at each bounce (adjustable)
 // collision damage control
 40 FLOAT en_fCollisionSpeedLimit = 20.0f,      // max. collision speed without damage (adjustable)
 41 FLOAT en_fCollisionDamageFactor = 20.0f,    // collision damage ammount multiplier (adjustable)

 51 FLOATaabbox3D en_boxMovingEstimate = FLOATaabbox3D(FLOAT3D(0,0,0), 0.01f), // overestimate of movement in next few ticks
 52 FLOATaabbox3D en_boxNearCached = FLOATaabbox3D(FLOAT3D(0,0,0), 0.01f),     // box in which the polygons are cached

 // intended movement in this tick
 64  FLOAT3D en_vIntendedTranslation = FLOAT3D(0,0,0),  // can be read on receiving a touch event, holds last velocity before touch
 65  FLOATmatrix3D en_mIntendedRotation = FLOATmatrix3D(0),

 // entity time
 70 FLOAT en_tmEntityTime = 0.0f,

 // used for elevators
 75 BOOL m_bReferenceMovingInY = FALSE,
 76 BOOL m_bReferenceRotatingNonY = FALSE,
 77 INDEX m_iPlayerRefCounter = 0,
//0210
 78 FLOAT3D en_vDesiredTranslationFinal = FLOAT3D(0.0f,0.0f,0.0f),
 
 80 INDEX m_tmMoveStart = 0,
 81 FLOAT m_tickMoveStart = 0.0f,

 82 BOOL  m_bForceStop = FALSE,
 83 BOOL m_bBlockDown = FALSE,
 84 BOOL m_bBlockUp = FALSE,
 85 BOOL m_bStair = FALSE,
 86 BOOL m_bFallDownToFloor = FALSE,
{
// these are not saved via the property system
	CPlacement3D en_plLastPlacementTmp;  //0120

	CPlacement3D en_plLastPlacement;  // placement in last tick (used for lerping) (not saved)
	CListNode en_lnInMovers;          // node in list of moving entities  (saved as bool)

	CBrushPolygon *en_pbpoStandOn; // cached last polygon standing on, just for optimization
	// used for caching near polygons of zoning brushes for fast collision detection
	CStaticStackArray<CBrushPolygon *> en_apbpoNearPolygons;  // cached polygons

	// these are really temporary - should never be used across ticks
	// next placement for collision detection
	FLOAT3D en_vNextPosition;
	FLOATmatrix3D en_mNextRotation;

	// delta for this movement
	FLOAT3D en_vMoveTranslation;
	FLOATmatrix3D en_mMoveRotation;
	// aplied movement in this tick
	FLOAT3D en_vAppliedTranslation;
	FLOATmatrix3D en_mAppliedRotation;
}


components:


functions:

	TIME GetLerpedEntityTime() {    
		return en_tmEntityTime + _pTimer->GetLerpFactor()*_pTimer->TickQuantum;
	}
	// has an eaction been applied in this tick? overrides in player.es
	// this must be here, although it is used for player entities only
	virtual BOOL IsActionApplied() {
		return TRUE;
	}
	virtual void SetActionApplied() {};
	virtual void ClearActionApplied() {};

	// are we reapplying actions
	virtual BOOL IsReapplyingActions() {
		return FALSE;
	};
	virtual void SetReapplyingActions() {};
	virtual void ClearReapplyingActions() {};
	
	virtual void StopPlayer() {};

	void ApplyDeadReckoning()
	{
		 
		if ((IsPlayer() && (_pNetwork->IsPlayerLocal(this) || _pNetwork->IsPlayingDemo())) || _pNetwork->IsGameFinished()) { 
			return; 
		};
//0210
	FLOAT3D deltaPos =  (en_vDesiredTranslationFinal - en_plPlacement.pl_PositionVector);
	  if (deltaPos.Length() < 0.5) {
	en_vDesiredTranslationFinal = en_plPlacement.pl_PositionVector;
	en_vCurrentTranslationAbsolute = FLOAT3D(0.0f,0.0f,0.0f);  
	en_aCurrentRotationAbsolute =  FLOAT3D(0.0f,0.0f,0.0f);
	if (IsAnimationPlaying(53)) { //102는 달리기.
			return;
		}
 	NewClearState(0.4f);    
//	en_vCurrentTranslationAbsolute = FLOAT3D(0.0f,0.0f,0.0f);
//0211	  //0215 잠시 지움.
//	AddAnimation(53,AN_LOOPING|AN_NORESTART,1,1);
		return; 
	  }


		extern TIME _tmLocalTick;
		TIME tmLerpTime;

		if (_pNetwork->IsPlayingDemo()) {
			tmLerpTime = _pNetwork->ga_tmNextDemoTick - _pTimer->TickQuantum;
		} else {
	//0209
	  //tmLerpTime = _tmLocalTick;	
		tmLerpTime = _pTimer->CurrentTick();//_tmLocalTick;
		}

		CPlacement3D plSpeed;
		FLOAT fLerpFactor = (tmLerpTime - en_tmLastPlacementTime)/_pTimer->TickQuantum;
		plSpeed.pl_PositionVector = en_vCurrentTranslationAbsolute*_pTimer->TickQuantum;
		plSpeed.pl_OrientationAngle = en_aCurrentRotationAbsolute*_pTimer->TickQuantum;

		CPlacement3D plLerped;
		plLerped.Extrapolate(en_plLastPlacementReceived,plSpeed,fLerpFactor);
		for (int i=1;i<=3;i++) {
			plLerped.pl_OrientationAngle(i) = WrapAngle(plLerped.pl_OrientationAngle(i));
			en_plPlacement.pl_OrientationAngle(i) = WrapAngle(en_plPlacement.pl_OrientationAngle(i));
		}

		FLOAT3D delta = (plLerped.pl_OrientationAngle - en_plPlacement.pl_OrientationAngle) + (plLerped.pl_PositionVector - en_plPlacement.pl_PositionVector);
		if (delta.Length() > 0.01) {
			if (en_plpLastPositions!=NULL) {
				en_plpLastPositions->AddPosition(en_plPlacement.pl_PositionVector);
			}       
			SetPlacement(plLerped);
		}

		
		CPlacement3D plLerped2;
		plLerped2.Extrapolate(en_plLastPlacementReceived,plSpeed,fLerpFactor-1);
		en_plLastPlacement = plLerped2;
		
		en_vNextPosition = en_plPlacement.pl_PositionVector;        
		FLOATmatrix3D mRotation;
		MakeRotationMatrix(mRotation,en_plPlacement.pl_OrientationAngle);
		en_mNextRotation = mRotation;

		ClientCacheNearPolygons();

	}

	// receive new data
	void AdjustDeadReckoning(CPlacement3D plPlacement,CPlacement3D plSpeed,TIME tmNewTime)
	{

		en_vCurrentTranslationAbsolute = plSpeed.pl_PositionVector;
		en_aCurrentRotationAbsolute = plSpeed.pl_OrientationAngle;
		en_plLastPlacementReceived = plPlacement;
		en_tmLastPlacementTime    = tmNewTime;
		
		ApplyDeadReckoning();

	}
//0210
 // receive new data
	void AdjustDeadReckoningNew(CPlacement3D plPlacement,CPlacement3D plSpeed,TIME tmNewTime,FLOAT3D plPlacementFinal )
	{

		en_vCurrentTranslationAbsolute = plSpeed.pl_PositionVector;
		en_aCurrentRotationAbsolute = plSpeed.pl_OrientationAngle;
		en_plLastPlacementReceived = plPlacement;
		en_tmLastPlacementTime    = tmNewTime;
		
	en_vDesiredTranslationFinal = plPlacementFinal;
		ApplyDeadReckoning();

	}


	/* Constructor. */
	export void CMovableEntity(void)
	{
		en_pbpoStandOn = NULL;
		en_apbpoNearPolygons.SetAllocationStep(5);
	}

	export void ~CMovableEntity(void)
	{
	}

	/* Initialization. */
	export void OnInitialize(const CEntityEvent &eeInput)
	{
		CRationalEntity::OnInitialize(eeInput);
		ClearTemporaryData();
		en_vIntendedTranslation = FLOAT3D(0,0,0);
		en_mIntendedRotation.Diagonal(1.0f);
		en_boxNearCached = FLOATaabbox3D();
		en_boxMovingEstimate = FLOATaabbox3D();
		en_pbpoStandOn = NULL;

		// join the server hash table    
		en_pwoWorld->wo_ehEntityHashContainer.AddEntity(this);

	}

	/* Called before releasing entity. */
	export void OnEnd(void)
	{
		// remove from movers if active
		if (en_lnInMovers.IsLinked()) {
			en_lnInMovers.Remove();
		}
		ClearTemporaryData();
		en_boxNearCached = FLOATaabbox3D();
		en_boxMovingEstimate = FLOATaabbox3D();
		CRationalEntity::OnEnd();
	}

	export void Copy(CEntity &enOther, ULONG ulFlags)
	{
		CRationalEntity::Copy(enOther, ulFlags);
		CMovableEntity *pmenOther = (CMovableEntity *)(&enOther);

		ClearTemporaryData();
		en_boxNearCached = FLOATaabbox3D();
		en_boxMovingEstimate = FLOATaabbox3D();
		en_pbpoStandOn = NULL;

		en_plLastPlacement = pmenOther->en_plLastPlacement;
		if (pmenOther->en_lnInMovers.IsLinked()) {
			AddToMovers();
		}
	}

	void ClearTemporaryData(void)
	{
		en_plLastPlacement = en_plPlacement;
		// init moving parameters so that they are valid for collision if entity is not moving
		en_vNextPosition = en_plPlacement.pl_PositionVector;
		en_mNextRotation = en_mRotation;
///*!*/    en_vIntendedTranslation = FLOAT3D(0,0,0);
///*!*/    en_mIntendedRotation.Diagonal(1.0f);
		en_vAppliedTranslation = FLOAT3D(0,0,0);
		en_mAppliedRotation.Diagonal(1.0f);

// !!!! is this ok?
//    en_pbpoStandOn = NULL;
//    en_apbpoNearPolygons.Clear();
//    en_apbpoNearPolygons.SetAllocationStep(5);
	}

	/* Read from stream. */
	export void Read_t( CTStream *istr,BOOL bNetwork) // throw char *
	{
		CRationalEntity::Read_t(istr,bNetwork);
		// last placement is not saved to disk - it is not neccessary!
		ClearTemporaryData();

		// old version didn't load this
		if (istr->PeekID_t()==CChunkID("MENT")) { // 'movable entity'
			istr->ExpectID_t("MENT"); // 'movable entity'

			INDEX ibpo;
			(*istr)>>ibpo;
			en_pbpoStandOn = GetWorldPolygonPointer(ibpo);

			BOOL bAnyNULLs = FALSE;
			INDEX ctbpoNear;
			(*istr)>>ctbpoNear;
			if (ctbpoNear>0) {
				en_apbpoNearPolygons.PopAll();
				en_apbpoNearPolygons.Push(ctbpoNear);
				for(INDEX i=0; i<ctbpoNear; i++) {
					INDEX ibpo;
					(*istr)>>ibpo;
					 en_apbpoNearPolygons[i] = GetWorldPolygonPointer(ibpo);
					 if (en_apbpoNearPolygons[i]==NULL) {
						bAnyNULLs = TRUE;
					 }
				}
				if (bAnyNULLs) {
					CPrintF("NULL saved for near polygon!\n");
					en_apbpoNearPolygons.PopAll();
				}
			}
		}

		// if entity was in list of movers when saving
		BOOL bWasMoving;
		(*istr)>>bWasMoving;
		if (bWasMoving) {
			// add it to movers
			AddToMovers();
		}

		// join the server hash table    
		en_pwoWorld->wo_ehEntityHashContainer.AddEntity(this);


	}
	/* Write to stream. */
	export void Write_t( CTStream *ostr,BOOL bNetwork) // throw char *
	{
		CRationalEntity::Write_t(ostr,bNetwork);

		ostr->WriteID_t("MENT"); // 'movable entity'

		INDEX ibpo;
		ibpo = GetWorldPolygonIndex(en_pbpoStandOn);
		(*ostr)<<ibpo;

		INDEX ctbpoNear = en_apbpoNearPolygons.Count();
		(*ostr)<<ctbpoNear;
		for(INDEX i=0; i<ctbpoNear; i++) {
			INDEX ibpo;
			ibpo = GetWorldPolygonIndex(en_apbpoNearPolygons[i]);
			(*ostr)<<ibpo;
		}

		// last placement is not saved to disk - it is not neccessary!
		// save linked state in list of movers
		(*ostr)<<en_lnInMovers.IsLinked();
	}

	// this one is used in rendering - gets lerped placement between ticks 
	// this is, i.e. placement with DeadReckoning
	export CPlacement3D GetLerpedPlacement(void) const
	{
		// get the lerping factor
		FLOAT fLerpFactor;
		CPlacement3D plLerped;
		CPlacement3D plSpeed;    

		fLerpFactor = _pTimer->GetLerpFactor2();
	 
		plLerped.Lerp(en_plLastPlacement,en_plPlacement,fLerpFactor);   
	
		return plLerped;
	}

	/* Add yourself to list of movers. */
	export void AddToMovers(void)
	{
		if (!en_lnInMovers.IsLinked()) {
			en_pwoWorld->wo_lhMovers.AddTail(en_lnInMovers);

			// this means that the entity has started moving, so 
			// force sending an update to the client
			if (IsSentOverNet() && !IsPlayer()) {
				extern CEntityMessage _emEntityMessage;
				_emEntityMessage.WritePlacementNotify(en_ulID,TRUE);      
				_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
			}
		}
	}

	export void AddToMoversDuringMoving(void) // used for recursive adding
	{
		// if already added
		if (en_lnInMovers.IsLinked()) {
			// do nothing
			return;
		}
		// add it
		AddToMovers();
		// mark that it was forced to add
		en_ulPhysicsFlags|=EPF_FORCEADDED;
	}

	/* Set desired rotation speed of movable entity. */
	export void SetDesiredRotation(const ANGLE3D &aRotation) 
	{
		FLOAT fRotationDelta;
		fRotationDelta =   fabs(en_aDesiredRotationRelative(1) - aRotation(1))
										 + fabs(en_aDesiredRotationRelative(2) - aRotation(2)) 
										 + fabs(en_aDesiredRotationRelative(3) - aRotation(3));

		extern FLOAT ser_fOrientationTreshold;
		if (_pNetwork->IsServer() && IsSentOverNet() && !IsPlayer() && fRotationDelta >= ser_fOrientationTreshold) {
			extern CEntityMessage _emEntityMessage;
			_emEntityMessage.WritePlacementNotify(en_ulID,TRUE);
			_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
		}

		en_aDesiredRotationRelative = aRotation;
		AddToMovers();
	}
	export const ANGLE3D &GetDesiredRotation(void) const { return en_aDesiredRotationRelative; };

	/* Set desired translation speed of movable entity. */
	export void SetDesiredTranslation(const FLOAT3D &vTranslation) 
	{
		FLOAT fTranslationDelta;
		fTranslationDelta =   fabs(en_vDesiredTranslationRelative(1) - vTranslation(1))
												+ fabs(en_vDesiredTranslationRelative(2) - vTranslation(2)) 
												+ fabs(en_vDesiredTranslationRelative(3) - vTranslation(3));

		extern FLOAT ser_fPositionTreshold;
		if (_pNetwork->IsServer() && IsSentOverNet() && !IsPlayer()&& fTranslationDelta >= ser_fPositionTreshold) {
			extern CEntityMessage _emEntityMessage;
			_emEntityMessage.WritePlacementNotify(en_ulID,TRUE);
			_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
		}

		en_vDesiredTranslationRelative = vTranslation;
		AddToMovers();
	}
	export const FLOAT3D &GetDesiredTranslation(void) const { return en_vDesiredTranslationRelative; };

	/* Add an impulse to the current speed of the entity (used for instantaneous launching). */
	export void GiveImpulseTranslationRelative(const FLOAT3D &vImpulseSpeedRelative)
	{
		CPlacement3D plImpulseSpeedAbsolute( vImpulseSpeedRelative, ANGLE3D(0,0,0)); 
		plImpulseSpeedAbsolute.RelativeToAbsolute(
			CPlacement3D(FLOAT3D(0.0f,0.0f,0.0f), en_plPlacement.pl_OrientationAngle));
		en_vCurrentTranslationAbsolute += plImpulseSpeedAbsolute.pl_PositionVector;
		AddToMovers();
	}
	export void GiveImpulseTranslationAbsolute(const FLOAT3D &vImpulseSpeed)
	{
		en_vCurrentTranslationAbsolute += vImpulseSpeed;
		AddToMovers();
	}

	export void LaunchAsPropelledProjectile(const FLOAT3D &vImpulseSpeedRelative, 
		CMovableEntity *penLauncher)
	{
		en_vDesiredTranslationRelative = vImpulseSpeedRelative;
		en_vCurrentTranslationAbsolute += vImpulseSpeedRelative*en_mRotation;
//    en_vCurrentTranslationAbsolute += penLauncher->en_vCurrentTranslationAbsolute;
		AddToMovers();
	}
	export void LaunchAsFreeProjectile(const FLOAT3D &vImpulseSpeedRelative, 
		CMovableEntity *penLauncher)
	{
		en_vCurrentTranslationAbsolute += vImpulseSpeedRelative*en_mRotation;
//    en_vCurrentTranslationAbsolute += penLauncher->en_vCurrentTranslationAbsolute;
//    en_fAcceleration = en_fDeceleration = 0.0f;
		AddToMovers();
	}

	/* Stop all translation */
	export void ForceStopTranslation(void) {
		en_vDesiredTranslationRelative = FLOAT3D(0.0f,0.0f,0.0f);
		en_vCurrentTranslationAbsolute = FLOAT3D(0.0f,0.0f,0.0f);
		en_vAppliedTranslation = FLOAT3D(0.0f,0.0f,0.0f);
	}

	/* Stop all rotation */
	export void ForceStopRotation(void) {
		en_aDesiredRotationRelative = ANGLE3D(0,0,0);
		en_aCurrentRotationAbsolute = ANGLE3D(0,0,0);
		en_mAppliedRotation.Diagonal(1.0f);
	}

	/* Stop at once in place */
	export void ForceFullStop(void) {
/* //0311 경사면관련 Undo
		if(GetFlags() & ENF_ALIVE && !IsEnemy() && !(IsCharacter()))//플레이어일때.
		{
			StopPlayer();
		}
		*/
		ForceStopTranslation();
		ForceStopRotation();
	}

	/* Fake that the entity jumped (for jumppads) */
	export void FakeJump(const FLOAT3D &vOrgSpeed, const FLOAT3D &vDirection, FLOAT fStrength,
		FLOAT fParallelMultiplier, FLOAT fNormalMultiplier, FLOAT fMaxExitSpeed, TIME tmControl)
	{
		// fixup jump time for right control
		en_tmJumped = en_tmEntityTime-en_tmMaxJumpControl+tmControl;

		// apply parallel and normal component multipliers
		FLOAT3D vCurrentNormal;
		FLOAT3D vCurrentParallel;
		GetParallelAndNormalComponents(vOrgSpeed, vDirection, vCurrentParallel, vCurrentNormal);

		/*
		CPrintF( "\nCurrent translation absolute before: %g, %g, %g\n", 
			vOrgSpeed(1),
			vOrgSpeed(2),
			vOrgSpeed(3));*/

		// compile translation vector
		en_vCurrentTranslationAbsolute = 
			vCurrentParallel*fParallelMultiplier + 
			vCurrentNormal*fNormalMultiplier +
			vDirection*fStrength;

		// clamp translation speed
		FLOAT fLength = en_vCurrentTranslationAbsolute.Length();
		if( fLength > fMaxExitSpeed)
		{
			en_vCurrentTranslationAbsolute = 
				en_vCurrentTranslationAbsolute/fLength*fMaxExitSpeed;
		}

		/*CPrintF( "Current translation absolute after: %g, %g, %g\n\n", 
			en_vCurrentTranslationAbsolute(1),
			en_vCurrentTranslationAbsolute(2),
			en_vCurrentTranslationAbsolute(3));*/

		// no reference while bouncing
		en_penReference = NULL;
		en_pbpoStandOn = NULL;
		en_vReferencePlane = FLOAT3D(0.0f, 0.0f, 0.0f);
		en_iReferenceSurface = 0;

		// add to movers
		AddToMovers();
	}

	/* Get relative angles from direction angles. */
	export ANGLE GetRelativeHeading(const FLOAT3D &vDirection) {
		//ASSERT(Abs(vDirection.Length()-1)<0.001f); // must be normalized!
		// get front component of vector
		FLOAT fFront = 
			-vDirection(1)*en_mRotation(1,3)
			-vDirection(2)*en_mRotation(2,3)
			-vDirection(3)*en_mRotation(3,3);
		// get left component of vector
		FLOAT fLeft = 
			-vDirection(1)*en_mRotation(1,1)
			-vDirection(2)*en_mRotation(2,1)
			-vDirection(3)*en_mRotation(3,1);
		// relative heading is arctan of angle between front and left
		return ATan2(fLeft, fFront);
	}
	export ANGLE GetRelativePitch(const FLOAT3D &vDirection) {
		ASSERT(Abs(vDirection.Length()-1)<0.001f); // must be normalized!
		// get front component of vector
		FLOAT fFront = 
			-vDirection(1)*en_mRotation(1,3)
			-vDirection(2)*en_mRotation(2,3)
			-vDirection(3)*en_mRotation(3,3);
		// get up component of vector
		FLOAT fUp = 
			+vDirection(1)*en_mRotation(1,2)
			+vDirection(2)*en_mRotation(2,2)
			+vDirection(3)*en_mRotation(3,2);
		// relative pitch is arctan of angle between front and up
		return ATan2(fUp, fFront);
	}

	/* Get absolute direction for a heading relative to another direction. */
	export void GetReferenceHeadingDirection(const FLOAT3D &vReference, ANGLE aH, FLOAT3D &vDirection) {
		ASSERT(Abs(vReference.Length()-1)<0.001f); // must be normalized!
		FLOAT3D vY(en_mRotation(1,2), en_mRotation(2,2), en_mRotation(3,2));
		FLOAT3D vX = (vY*vReference).Normalize();
		FLOAT3D vMZ = vY*vX;
		vDirection = -vX*Sin(aH)+vMZ*Cos(aH);
	}

	/* Get absolute direction for a heading relative to current direction. */
	export void GetHeadingDirection(ANGLE aH, FLOAT3D &vDirection) {
		FLOAT3D vX(en_mRotation(1,1), en_mRotation(2,1), en_mRotation(3,1));
		FLOAT3D vZ(en_mRotation(1,3), en_mRotation(2,3), en_mRotation(3,3));
		vDirection = -vX*Sin(aH)-vZ*Cos(aH);
	}

	/* Get absolute direction for a pitch relative to current direction. */
	export void GetPitchDirection(ANGLE aH, FLOAT3D &vDirection) {
		FLOAT3D vY(en_mRotation(1,2), en_mRotation(2,2), en_mRotation(3,2));
		FLOAT3D vZ(en_mRotation(1,3), en_mRotation(2,3), en_mRotation(3,3));
		vDirection = -vZ*Cos(aH)+vY*Sin(aH);
	}

	// get a valid inflictor for misc damage (last brush or this)
	CEntity *MiscDamageInflictor(void)
	{
		// NOTE: must be damaged by some brush if possible, because enemies are set up so
		// that they cannot harm themselves.
		if (en_penLastValidReference!=NULL) {
			return en_penLastValidReference;
		} else {
			CBrushSector *pbsc = GetFirstSector();
			if (pbsc==NULL) {
				return this;
			} else {
				return pbsc->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity;
			}
		}
	}

	// add the sector force
	void UpdateOneSectorForce(CBrushSector &bsc, FLOAT fRatio)
	{
		// if not significantly
		if (fRatio<0.01f) {
			// just ignore it
			return;
		}
		INDEX iForceType = bsc.GetForceType();
		CEntity *penEntity = bsc.bsc_pbmBrushMip->bm_pbrBrush->br_penEntity;

/*
		BOOL bAnyIn = FALSE;
		CStaticArray<CMovingSphere> &absSpheres = en_pciCollisionInfo->ci_absSpheres;
		// for each sphere
		for(INDEX iSphere=0; iSphere<absSpheres.Count(); iSphere++) {
			CMovingSphere &ms = absSpheres[iSphere];
			// if the sphere is in field sector
			if (bsc.bsc_bspBSPTree.TestSphere(
				FLOATtoDOUBLE(ms.ms_vRelativeCenter0), ms.ms_fR)>=0) {
				bAnyIn = TRUE;
				break;
			}
		}
		if (!bAnyIn) {
			return;
		}*/

		// try to find the force in container
		CEntityForce *pef = NULL;
		for(INDEX iForce=0; iForce<_aefForces.Count(); iForce++) {
			if (penEntity ==_aefForces[iForce].ef_penEntity
				&&iForceType==_aefForces[iForce].ef_iForceType) {
				pef = &_aefForces[iForce];
				break;
			}
		}
	 
		// if field is not found
		if (pef==NULL) {
			// add a new one
			pef = _aefForces.Push(1);
			pef->ef_penEntity = penEntity;
			pef->ef_iForceType = iForceType;
			pef->ef_fRatio = 0.0f;
		}
		pef->ef_fRatio+=fRatio;
	}

	// test for field containment
	void TestFields(INDEX &iUpContent, INDEX &iDnContent, FLOAT &fImmersionFactor)
	{
		// this works only for models
		ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL || en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
		iUpContent = 0;
		iDnContent = 0;
		FLOAT fUp = 0.0f;
		FLOAT fDn = 0.0f;

		FLOAT3D &vOffset = en_plPlacement.pl_PositionVector;
		FLOATmatrix3D &mRotation = en_mRotation;
		// project height min/max in the entity to absolute space
		FLOAT3D vMin = FLOAT3D(0, en_pciCollisionInfo->ci_fMinHeight, 0);
		FLOAT3D vMax = FLOAT3D(0, en_pciCollisionInfo->ci_fMaxHeight, 0);
		vMin = vMin*mRotation+vOffset;
		vMax = vMax*mRotation+vOffset;
		// project all spheres in the entity to absolute space (for touch field testing)
		CStaticArray<CMovingSphere> &absSpheres = en_pciCollisionInfo->ci_absSpheres;
		FOREACHINSTATICARRAY(absSpheres, CMovingSphere, itms) {
			itms->ms_vRelativeCenter0 = itms->ms_vCenter*mRotation+vOffset;
		}

		// clear forces
		_aefForces.PopAll();
		// for each sector that this entity is in
		{FOREACHSRCOFDST(en_rdSectors, CBrushSector, bsc_rsEntities, pbsc)
			CBrushSector &bsc = *pbsc;
			// if this sector is not in first mip
			if (!bsc.bsc_pbmBrushMip->IsFirstMip()) {
				// skip it
				continue;
			}
			// get entity of the sector
			CEntity *penSector = bsc.bsc_pbmBrushMip->bm_pbrBrush->br_penEntity;

			// if not real brush
			if (penSector->en_RenderType!=RT_BRUSH) {
				// skip it
				continue;
			}

			// get min/max parameters of entity inside sector
			double dMin, dMax;
			bsc.bsc_bspBSPTree.FindLineMinMax(FLOATtoDOUBLE(vMin), FLOATtoDOUBLE(vMax), dMin, dMax);

			// if sector content is not default
			INDEX iContent = bsc.GetContentType();
			if (iContent!=0) {
				// if inside sector at all
				if (dMax>0.0f && dMin<1.0f) {
					//CPrintF("%s: %lf %lf    ", bsc.bsc_strName, dMin, dMax);
					// if minimum is small
					if (dMin<0.01f) {
						// update down content
						iDnContent = iContent;
						fDn = Max(fDn, FLOAT(dMax));
					}
					// if maximum is large
					if (dMax>0.99f) {
						// update up content
						iUpContent = iContent;
						fUp = Max(fUp, 1-FLOAT(dMin));
					}
				}
			}

			// add the sector force
			UpdateOneSectorForce(bsc, dMax-dMin);

		ENDFOR;}
		//CPrintF("%f %d %f %d\n", fDn, iDnContent, fUp, iUpContent);

		// if same contents
		if (iUpContent == iDnContent) {
			// trivial case
			fImmersionFactor = 1.0f;
		// if different contents
		} else {
			// calculate immersion factor
			if (iUpContent==0) {
				fImmersionFactor = fDn;
			} else if (iDnContent==0) {
				fImmersionFactor = 1-fUp;
			} else {
				fImmersionFactor = Max(fDn, 1-fUp);
			}
			// eliminate degenerate cases
			if (fImmersionFactor<0.01f) {
				fImmersionFactor = 1.0f;
				iDnContent = iUpContent;
			} else if (fImmersionFactor>0.99f) {
				fImmersionFactor = 1.0f;
				iUpContent = iDnContent;
			}
		}

		// clear force container and calculate average forces
		FLOAT3D vGravityA(0,0,0);
		FLOAT3D vGravityV(0,0,0);
		FLOAT3D vForceA(0,0,0);
		FLOAT3D vForceV(0,0,0);
		FLOAT fRatioSum = 0.0f;

		{for(INDEX iForce=0; iForce<_aefForces.Count(); iForce++) {
			CForceStrength fsGravity;
			CForceStrength fsField;
			_aefForces[iForce].ef_penEntity->GetForce(
				_aefForces[iForce].ef_iForceType, en_plPlacement.pl_PositionVector, 
				fsGravity, fsField);
			FLOAT fRatio = _aefForces[iForce].ef_fRatio;
			fRatioSum+=fRatio;
			vGravityA+=fsGravity.fs_vDirection*fsGravity.fs_fAcceleration*fRatio;
			vGravityV+=fsGravity.fs_vDirection*fsGravity.fs_fVelocity*fRatio;
			if (fsField.fs_fAcceleration>0) {
				vForceA+=fsField.fs_vDirection*fsField.fs_fAcceleration*fRatio;
				vForceV+=fsField.fs_vDirection*fsField.fs_fVelocity*fRatio;
			}
			_aefForces[iForce].Clear();
		}}
		if (fRatioSum>0) {
			vGravityA/=fRatioSum;
			vGravityV/=fRatioSum;
			vForceA/=fRatioSum;
			vForceV/=fRatioSum;
		}
		en_fGravityA = vGravityA.Length();
		if (en_fGravityA<0.01f) {
		 en_fGravityA = 0;
		} else {
		 en_fGravityV = vGravityV.Length();
		 en_vGravityDir = vGravityA/en_fGravityA;
		}
		en_fForceA = vForceA.Length();
		if (en_fForceA<0.01f) {
		 en_fForceA = 0;
		} else {
		 en_fForceV = vForceV.Length();
		 en_vForceDir = vForceA/en_fForceA;
		}
		_aefForces.PopAll();

		extern INDEX sys_iCPUMisc;
		if (!sys_iCPUMisc) {  // copy protection broken!
			// do some funny gravity tricks
			if (_pTimer->CurrentTick()>60.0f && 
				Abs(_pTimer->CurrentTick()-floor(_pTimer->CurrentTick())) <0.05f) {
				en_vGravityDir = -en_vGravityDir;
			}
		}
	}

	// test entity breathing
	void TestBreathing(CContentType &ctUp) 
	{
		// if this entity doesn't breathe
		if (!(en_ulPhysicsFlags&(EPF_HASLUNGS|EPF_HASGILLS))) {
			// do nothing
			return;
		}
		// find current breathing parameters
		BOOL bCanBreathe = 
			(ctUp.ct_ulFlags&CTF_BREATHABLE_LUNGS) && (en_ulPhysicsFlags&EPF_HASLUNGS) ||
			(ctUp.ct_ulFlags&CTF_BREATHABLE_GILLS) && (en_ulPhysicsFlags&EPF_HASGILLS);
		TIME tmNow = en_tmEntityTime;
		TIME tmBreathDelay = tmNow-en_tmLastBreathed;
		// if entity can breathe now
		if (bCanBreathe) {
			// update breathing time
			en_tmLastBreathed = tmNow;
			// if it was without air for some time
			if (tmBreathDelay>_pTimer->TickQuantum*2) {
				// notify entity that it has take air now
				ETakingBreath eTakingBreath;
				eTakingBreath.fBreathDelay = tmBreathDelay/en_tmMaxHoldBreath;
				SendEvent(eTakingBreath);
			}
		// if entity can not breathe now
		} else {
			// if it was without air for too long
			if (tmBreathDelay>en_tmMaxHoldBreath) {
				// inflict drowning damage 
				InflictDirectDamage(this, MiscDamageInflictor(), DMT_DROWNING, ctUp.ct_fDrowningDamageAmount, 
					en_plPlacement.pl_PositionVector, -en_vGravityDir);
				// prolongue breathing a bit, so not to come here every frame
				en_tmLastBreathed = tmNow-en_tmMaxHoldBreath+ctUp.ct_tmDrowningDamageDelay;
			}
		}
	}
	void TestContentDamage(CContentType &ctDn, FLOAT fImmersion)
	{
		// if the content can damage by swimming
		if (ctDn.ct_fSwimDamageAmount>0) {
			TIME tmNow = _pTimer->CurrentTick();
			// if there is a delay
			if (ctDn.ct_tmSwimDamageDelay>0) {
				// if not yet delayed
				if (tmNow-en_tmLastSwimDamage>ctDn.ct_tmSwimDamageDelay+_pTimer->TickQuantum) {
					// delay
					en_tmLastSwimDamage = tmNow+ctDn.ct_tmSwimDamageDelay;
					return;
				}
			}

			if (tmNow-en_tmLastSwimDamage>ctDn.ct_tmSwimDamageFrequency) {
				// inflict drowning damage 
				InflictDirectDamage(this, MiscDamageInflictor(),
					(DamageType)ctDn.ct_iSwimDamageType, ctDn.ct_fSwimDamageAmount*fImmersion, 
					en_plPlacement.pl_PositionVector, -en_vGravityDir);
				en_tmLastSwimDamage = tmNow;
			}
		}
		// if the content kills
		if (ctDn.ct_fKillImmersion>0 && fImmersion>=ctDn.ct_fKillImmersion
			&&(en_ulFlags&ENF_ALIVE)) {
			// inflict killing damage 
			InflictDirectDamage(this, MiscDamageInflictor(),
				(DamageType)ctDn.ct_iKillDamageType, GetHealth()*10.0f, 
				en_plPlacement.pl_PositionVector, -en_vGravityDir);
		}
	}

	void TestSurfaceDamage(CSurfaceType &stDn)
	{
		// if the surface can damage by walking
		if (stDn.st_fWalkDamageAmount>0) {
			TIME tmNow = _pTimer->CurrentTick();
			// if there is a delay
			if (stDn.st_tmWalkDamageDelay>0) {
				// if not yet delayed
				if (tmNow-en_tmLastSwimDamage>stDn.st_tmWalkDamageDelay+_pTimer->TickQuantum) {
					// delay
					en_tmLastSwimDamage = tmNow+stDn.st_tmWalkDamageDelay;
					return;
				}
			}

			if (tmNow-en_tmLastSwimDamage>stDn.st_tmWalkDamageFrequency) {
				// inflict walking damage 
				InflictDirectDamage(this, MiscDamageInflictor(),
					(DamageType)stDn.st_iWalkDamageType, stDn.st_fWalkDamageAmount, 
					en_plPlacement.pl_PositionVector, -en_vGravityDir);
				en_tmLastSwimDamage = tmNow;
			}
		}
	}

	// send touch event to this entity and touched entity
	void SendTouchEvent(const CClipMove &cmMove)
	{
		ETouch etouchThis;
		ETouch etouchOther;
		etouchThis.penOther = cmMove.cm_penHit;
		etouchThis.bThisMoved = FALSE;
		etouchThis.plCollision = cmMove.cm_plClippedPlane;
		etouchOther.penOther = this;
		etouchOther.bThisMoved = TRUE;
		etouchOther.plCollision = cmMove.cm_plClippedPlane;
		SendEvent(etouchThis);
		cmMove.cm_penHit->SendEvent(etouchOther);
	}

	// send block event to this entity
	void SendBlockEvent(CClipMove &cmMove)
	{
		EBlock eBlock;
		eBlock.penOther = cmMove.cm_penHit;
		eBlock.plCollision = cmMove.cm_plClippedPlane;
		SendEvent(eBlock);
		if (IsSentOverNet() && !IsPlayer()) {
			extern CEntityMessage _emEntityMessage;
			_emEntityMessage.WritePlacementNotify(en_ulID,TRUE);      
			_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
		}
	}

	BOOL IsStandingOnPolygon(CBrushPolygon *pbpo)
	{
		// if cannot optimize for standing on handle
		if (en_pciCollisionInfo==NULL 
			||!(en_pciCollisionInfo->ci_ulFlags&CIF_CANSTANDONHANDLE)) {
			// not standing on polygon
			return FALSE;
		}

		// if polygon is not valid for standing on any more (brush turned off collision)
		if (en_pbpoStandOn->bpo_pbscSector->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity->en_ulCollisionFlags==0) {
			// not standing on polygon
			return FALSE;
		}

		const FLOATplane3D &plPolygon = pbpo->bpo_pbplPlane->bpl_plAbsolute;
		// get stand-on handle
		FLOAT3D vHandle = en_plPlacement.pl_PositionVector;
		vHandle(1)+=en_pciCollisionInfo->ci_fHandleY*en_mRotation(1,2);
		vHandle(2)+=en_pciCollisionInfo->ci_fHandleY*en_mRotation(2,2);
		vHandle(3)+=en_pciCollisionInfo->ci_fHandleY*en_mRotation(3,2);
		vHandle-=((FLOAT3D&)plPolygon)*en_pciCollisionInfo->ci_fHandleR;

		// if handle is not on the plane
		if (plPolygon.PointDistance(vHandle)>0.01f) {
			// not standing on polygon
			return FALSE;
		}

		// find major axes of the polygon plane
		INDEX iMajorAxis1, iMajorAxis2;
		GetMajorAxesForPlane(plPolygon, iMajorAxis1, iMajorAxis2);

		// create an intersector
		CIntersector isIntersector(vHandle(iMajorAxis1), vHandle(iMajorAxis2));
		// for all edges in the polygon
		FOREACHINSTATICARRAY(pbpo->bpo_abpePolygonEdges, CBrushPolygonEdge, itbpePolygonEdge) {
			// get edge vertices (edge direction is irrelevant here!)
			const FLOAT3D &vVertex0 = itbpePolygonEdge->bpe_pbedEdge->bed_pbvxVertex0->bvx_vAbsolute;
			const FLOAT3D &vVertex1 = itbpePolygonEdge->bpe_pbedEdge->bed_pbvxVertex1->bvx_vAbsolute;
			// pass the edge to the intersector
			isIntersector.AddEdge(
				vVertex0(iMajorAxis1), vVertex0(iMajorAxis2),
				vVertex1(iMajorAxis1), vVertex1(iMajorAxis2));
		}

		// if the point is inside polygon
		if (isIntersector.IsIntersecting()) {
			// entity is standing on polygon
			return TRUE;
		// if the point is outside polygon
		} else {
			// entity is not standing on polygon
			return FALSE;
		}
	}

	// check whether a polygon is below given point, but not too far away
	BOOL IsPolygonBelowPoint(CBrushPolygon *pbpo, const FLOAT3D &vPoint, FLOAT fMaxDist)
	{
		// if passable or not allowed as ground
		if ((pbpo->bpo_ulFlags&BPOF_PASSABLE)
			||!AllowForGroundPolygon(pbpo)) {
			// it cannot be below
			return FALSE;
		}

		// get polygon plane
		const FLOATplane3D &plPolygon = pbpo->bpo_pbplPlane->bpl_plAbsolute;

		// determine polygon orientation relative to gravity
		FLOAT fCos = ((const FLOAT3D &)plPolygon)%en_vGravityDir;
		// if polygon is vertical or upside down
		if (fCos>-0.01f) {
			// it cannot be below
			return FALSE;
		}

		// if polygon's steepness is too high(높고 가파른)
		CSurfaceType &stReference = en_pwoWorld->wo_astSurfaceTypes[pbpo->bpo_bppProperties.bpp_ubSurfaceType];
		if (fCos>=-stReference.st_fClimbSlopeCos&&fCos<0
			||stReference.st_ulFlags&STF_SLIDEDOWNSLOPE) {
			// it cannot be below
			return FALSE;
		}

		// get distance from point to the plane
		FLOAT fD = plPolygon.PointDistance(vPoint);
		// if the point is behind the plane
		if (fD<-0.01f) {
			// it cannot be below
			return FALSE;
		}

		// find distance of point from the polygon along gravity vector
		FLOAT fDistance = -fD/fCos;

		// 높은 곳에서 뛰어내리면 에어워크 하던 것과 못 뛰어내리던 것을 수정함.
		/*
		// if too far away
		if (fDistance > fMaxDist) {
			// it cannot be below
			return FALSE;
		}
		*/
		// project point to the polygon along gravity vector
		FLOAT3D vProjected = vPoint + en_vGravityDir*fDistance;

		// find major axes of the polygon plane
		INDEX iMajorAxis1, iMajorAxis2;
		GetMajorAxesForPlane(plPolygon, iMajorAxis1, iMajorAxis2);

		// create an intersector
		CIntersector isIntersector(vProjected(iMajorAxis1), vProjected(iMajorAxis2));
		// for all edges in the polygon
		FOREACHINSTATICARRAY(pbpo->bpo_abpePolygonEdges, CBrushPolygonEdge, itbpePolygonEdge) {
			// get edge vertices (edge direction is irrelevant here!)
			const FLOAT3D &vVertex0 = itbpePolygonEdge->bpe_pbedEdge->bed_pbvxVertex0->bvx_vAbsolute;
			const FLOAT3D &vVertex1 = itbpePolygonEdge->bpe_pbedEdge->bed_pbvxVertex1->bvx_vAbsolute;
			// pass the edge to the intersector
			isIntersector.AddEdge(
				vVertex0(iMajorAxis1), vVertex0(iMajorAxis2),
				vVertex1(iMajorAxis1), vVertex1(iMajorAxis2));
		}

		// if the point is inside polygon
		if (isIntersector.IsIntersecting()) {
			// it is below
			return TRUE;
		// if the point is outside polygon
		} else {
			// it is not below
			return FALSE;
		}
	}

	// override this to make filtering for what can entity stand on
	export virtual BOOL AllowForGroundPolygon(CBrushPolygon *pbpo)
	{
		return TRUE;
	}

	// check whether any cached near polygon is below given point
	BOOL IsSomeNearPolygonBelowPoint(const FLOAT3D &vPoint, FLOAT fMaxDist)
	{
		// otherwise, there is none
		return FALSE;
	}

	// check whether any polygon in a sector is below given point
	BOOL IsSomeSectorPolygonBelowPoint(CBrushSector *pbsc, const FLOAT3D &vPoint, FLOAT fMaxDist)
	{
		// for each polygon in the sector
		FOREACHINSTATICARRAY(pbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) {
			CBrushPolygon *pbpo = itbpo;
			// if it is below
			if (IsPolygonBelowPoint(pbpo, vPoint, fMaxDist)) {
				// there is some
				return TRUE;
			}
		}
		// otherwise, there is none
		return FALSE;
	}

	// check whether entity would fall if standing on next position
	BOOL WouldFallInNextPosition(void)
	{
		// if entity doesn't care for falling
		if (en_fStepDnHeight<0) {
			// don't check
			return FALSE;
		}
	
		// if the stand-on polygon is near below
		if (en_pbpoStandOn!=NULL &&
			IsPolygonBelowPoint(en_pbpoStandOn, en_vNextPosition, en_fStepDnHeight)) {
			// it won't fall
			return FALSE;
		}

		// make empty list of extra sectors to check
		CListHead lhActiveSectors;

		CStaticStackArray<CBrushPolygon *> &apbpo = en_apbpoNearPolygons;
		// for each cached near polygon
		for(INDEX iPolygon=0; iPolygon<apbpo.Count(); iPolygon++) {
			CBrushPolygon *pbpo = apbpo[iPolygon];
			// if it is below
			if (IsPolygonBelowPoint(pbpo, en_vNextPosition, en_fStepDnHeight)) {
				// it won't fall
				lhActiveSectors.RemAll();
				return FALSE;
			}
			// if the polygon's sector is not added yet
			if (!pbpo->bpo_pbscSector->bsc_lnInActiveSectors.IsLinked()) {
				// add it
				lhActiveSectors.AddTail(pbpo->bpo_pbscSector->bsc_lnInActiveSectors);
			}
		}

		// NOTE: We add non-zoning reference first (if existing),
		// to speed up cases when standing on moving brushes.
		// if the reference is a non-zoning brush
		if (en_penReference!=NULL && en_penReference->en_RenderType==RT_BRUSH
			&&!(en_penReference->en_ulFlags&ENF_ZONING)
			&& en_penReference->en_pbrBrush!=NULL) {
			// get first mip of the brush
			CBrushMip *pbmMip = en_penReference->en_pbrBrush->GetFirstMip();
			// for each sector in the brush mip
			FOREACHINDYNAMICARRAY(pbmMip->bm_abscSectors, CBrushSector, itbsc) {
				// if it is not added yet
				if (!itbsc->bsc_lnInActiveSectors.IsLinked()) {
					// add it
					lhActiveSectors.AddTail(itbsc->bsc_lnInActiveSectors);
				}
			}
		}

		// for each zoning sector that this entity is in
		{FOREACHSRCOFDST(en_rdSectors, CBrushSector, bsc_rsEntities, pbsc);
			// if it is not added yet
			if (!pbsc->bsc_lnInActiveSectors.IsLinked()) {
				// add it
				lhActiveSectors.AddTail(pbsc->bsc_lnInActiveSectors);
			}
		ENDFOR;}

		// for each active sector
		BOOL bSupportFound = FALSE;
		FOREACHINLIST(CBrushSector, bsc_lnInActiveSectors, lhActiveSectors, itbsc) {
			CBrushSector *pbsc = itbsc;
			// if the sector is zoning
			if (pbsc->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity->en_ulFlags&ENF_ZONING) {
				// for non-zoning brush entities in the sector
				{FOREACHDSTOFSRC(pbsc->bsc_rsEntities, CEntity, en_rdSectors, pen);
					if (pen->en_RenderType==CEntity::RT_TERRAIN) {
						if (TR_IsTerrainBelowPoint(pen->en_ptrTerrain, en_vNextPosition, en_fStepDnHeight, en_vGravityDir)) {
							bSupportFound = TRUE;
							goto out;
						}
						continue;
					}
					if (pen->en_RenderType!=CEntity::RT_BRUSH&&
							pen->en_RenderType!=CEntity::RT_FIELDBRUSH) {
						break;  // brushes are sorted first in list
					}
					// get first mip of the brush
					CBrushMip *pbmMip = pen->en_pbrBrush->GetFirstMip();
					// for each sector in the brush mip
					FOREACHINDYNAMICARRAY(pbmMip->bm_abscSectors, CBrushSector, itbscInMip) {
						// if it is not added yet
						if (!itbscInMip->bsc_lnInActiveSectors.IsLinked()) {
							// add it
							lhActiveSectors.AddTail(itbscInMip->bsc_lnInActiveSectors);
						}
					}
				ENDFOR;}
			}
			// if there is a polygon below in that sector
			if (IsSomeSectorPolygonBelowPoint(itbsc, en_vNextPosition, en_fStepDnHeight)) {
				// it won't fall
				bSupportFound = TRUE;
				break;
			}
		}
out:;

		// clear list of active sectors
		lhActiveSectors.RemAll();

		// if no support, it surely would fall
		return !bSupportFound;
	}

	// clear next position to current placement
	void ClearNextPosition(void)
	{
		en_vNextPosition = en_plPlacement.pl_PositionVector;
		en_mNextRotation = en_mRotation;
	}
	// set current placement from next position
	void SetPlacementFromNextPosition(void)
	{
		CPlacement3D plNew;
		plNew.pl_PositionVector = en_vNextPosition;
		DecomposeRotationMatrixNoSnap(plNew.pl_OrientationAngle, en_mNextRotation);
		FLOATmatrix3D mRotation;
		MakeRotationMatrixFast(mRotation, plNew.pl_OrientationAngle);

		SetPlacement_special(plNew, mRotation, SPIF_NEAR|SPIF_SKIPMOVEABLECHILDREN);

		// for each movable child of this entity
		{FOREACHINLIST(CEntity, en_lnInParent, en_lhChildren, itenChild) {
			if (!_pNetwork->IsServer() || !(itenChild->en_ulPhysicsFlags&EPF_MOVABLE)) {
				continue;
			}
			CMovableEntity *penChild = (CMovableEntity*)(CEntity*)itenChild;
			CPlacement3D plNew = penChild->en_plRelativeToParent;
			plNew.RelativeToAbsoluteSmooth(en_plPlacement);
			MakeRotationMatrixFast(penChild->en_mNextRotation, plNew.pl_OrientationAngle);
			penChild->en_vNextPosition = plNew.pl_PositionVector;

			// create movement towards the new placement
			CClipMove cmMove(penChild);
			en_pwoWorld->ClipMove(cmMove);

			// if the move passes
			if (cmMove.cm_fMovementFraction>1.0f) {
				penChild->SetPlacement(plNew);
			}

		}}
	}

	BOOL TryToGoUpstairs(const FLOAT3D &vTranslationAbsolute, const CSurfaceType &stHit,
		BOOL bHitStairsOrg)
	{
		// use only horizontal components of the movement
		FLOAT3D vTranslationHorizontal;
		GetNormalComponent(vTranslationAbsolute, en_vGravityDir, vTranslationHorizontal);

		//CPrintF("Trying: (%g) ", vTranslationHorizontal(3));
		// if the movement has no substantial value
		if(vTranslationHorizontal.Length()<0.001f) {
			//CPrintF("no value\n");
			// don't do it
			return FALSE;
		}
		FLOAT3D vTranslationHorizontalOrg = vTranslationHorizontal;
		// if the surface that is climbed on is not really stairs
		if (!bHitStairsOrg) {
			// keep minimum speed
			vTranslationHorizontal.Normalize();
			vTranslationHorizontal*=0.5f;
		}

		// remember original placement
		CPlacement3D plOriginal = en_plPlacement;

		// take stairs height
		FLOAT fStairsHeight = 0;
		if (stHit.st_fStairsHeight>0) {
			fStairsHeight = Max(stHit.st_fStairsHeight, en_fStepUpHeight);
		} else if (stHit.st_fStairsHeight<0) {
			fStairsHeight = Min(stHit.st_fStairsHeight, en_fStepUpHeight);
		}

		CContentType &ctDn = en_pwoWorld->wo_actContentTypes[en_iDnContent];
		CContentType &ctUp = en_pwoWorld->wo_actContentTypes[en_iUpContent];

		// if in partially in water
		BOOL bGettingOutOfWater = FALSE;
		if ((ctDn.ct_ulFlags&CTF_SWIMABLE) && !(ctUp.ct_ulFlags&CTF_SWIMABLE)
			&& en_fImmersionFactor>0.3f) {
			// add immersion height to up step
			if (en_pciCollisionInfo!=NULL) {
				fStairsHeight=fStairsHeight*2+en_fImmersionFactor*
					(en_pciCollisionInfo->ci_fMaxHeight-en_pciCollisionInfo->ci_fMinHeight);
				// remember that we are trying to get out of water
				bGettingOutOfWater = TRUE;
			}
		}

		// calculate the 3 translation directions (up, forward and down)
		FLOAT3D avTranslation[3];
		avTranslation[0] = en_vGravityDir*-fStairsHeight;
		avTranslation[1] = vTranslationHorizontal;
		avTranslation[2] = en_vGravityDir*fStairsHeight;

		// for each translation step
		for(INDEX iStep=0; iStep<3; iStep++) {
			BOOL bStepOK = TRUE;
			// create new placement with the translation step
			en_vNextPosition = en_plPlacement.pl_PositionVector+avTranslation[iStep];
			en_mNextRotation = en_mRotation;
			// clip the movement to the entity's world
			CClipMove cm(this);
			en_pwoWorld->ClipMove(cm);

			// if not passed
			if (cm.cm_fMovementFraction<1.0f) {
				// find hit surface
				INDEX iSurfaceHit = 0;
				BOOL bHitStairsNow = FALSE;
				if (cm.cm_pbpoHit!=NULL) {
					bHitStairsNow = cm.cm_pbpoHit->bpo_ulFlags&BPOF_STAIRS;
					iSurfaceHit = cm.cm_pbpoHit->bpo_bppProperties.bpp_ubSurfaceType;
				}
				CSurfaceType &stHit = en_pwoWorld->wo_astSurfaceTypes[iSurfaceHit];


				// check if hit a slope while climbing stairs
				const FLOAT3D &vHitPlane = cm.cm_plClippedPlane;
				FLOAT fPlaneDotG = vHitPlane%en_vGravityDir;
				FLOAT fPlaneDotGAbs = Abs(fPlaneDotG);

				BOOL bSlidingAllowed = (fPlaneDotGAbs>-0.01f && fPlaneDotGAbs<0.99f)&&bHitStairsOrg;

				BOOL bEarlyClipAllowed = 
					// going up or
					iStep==0 || 
					// going forward and hit stairs or
					iStep==1 && bHitStairsNow || 
					// going down and ends on something that is not high slope
					iStep==2 && 
						(vHitPlane%en_vGravityDir<-stHit.st_fClimbSlopeCos ||
						 bHitStairsNow);

				// if early clip is allowed
				if (bEarlyClipAllowed || bSlidingAllowed) {
					// try to go to where it is clipped (little bit before)
					en_vNextPosition = en_plPlacement.pl_PositionVector +
						avTranslation[iStep]*(cm.cm_fMovementFraction*0.98f);
					if (bSlidingAllowed && iStep!=2) {
						FLOAT3D vSliding = cm.cm_plClippedPlane.ProjectDirection(
									avTranslation[iStep]*(1.0f-cm.cm_fMovementFraction))+
						vHitPlane*(ClampUp(avTranslation[iStep].Length(), 0.5f)/100.0f);
						en_vNextPosition += vSliding;
					}
					CClipMove cm(this);
					en_pwoWorld->ClipMove(cm);
					// if it failed
					if (cm.cm_fMovementFraction<=1.0f) {
						// mark that this step is unsuccessful
						bStepOK = FALSE;
					}
				// if early clip is not allowed
				} else {
					// mark that this step is unsuccessful
					bStepOK = FALSE;
				}
			}

			// if the step is successful
			if (bStepOK) {
				// use that position
				SetPlacementFromNextPosition();
			// if the step failed
			} else {
				// restore original placement
				en_vNextPosition = plOriginal.pl_PositionVector;
				SetPlacementFromNextPosition();
				// move is unsuccessful
				//CPrintF("FAILED\n");
				return FALSE;
			}

		} // end of steps loop

		// all steps passed, use the final position

		// NOTE: must not keep the speed when getting out of water,
		// or the player gets launched out too fast
		if (!bGettingOutOfWater) {
			en_vAppliedTranslation += vTranslationHorizontalOrg;
		}
		// move is successful
		//CPrintF("done\n");
		return TRUE;
	}

	/* Try to translate the entity. Slide, climb or push others if needed. */
	BOOL TryToMove(CMovableEntity *penPusher, BOOL bTranslate, BOOL bRotate)
	{
		//CPrintF("TryToMove(%d)\n", _ctTryToMoveCheckCounter);
		// decrement the recursion counter
		if (penPusher!=NULL) {
			_ctTryToMoveCheckCounter--;
		} else {
			_ctTryToMoveCheckCounter-=4;
		}
		// if recursing too deep
		if (_ctTryToMoveCheckCounter<0) {
			// fail the move
			return FALSE;
		}
		// create new placement with movement
		if (bTranslate) {
			en_vNextPosition = en_plPlacement.pl_PositionVector+en_vMoveTranslation;

		} else {
			en_vNextPosition = en_plPlacement.pl_PositionVector;
		}
		if (bRotate) {
//      CPrintF("  r:???\n");
			en_mNextRotation = en_mMoveRotation*en_mRotation;
		} else {
			en_mNextRotation = en_mRotation;
		}

		// test if rotation can be ignored
		ULONG ulCIFlags = en_pciCollisionInfo->ci_ulFlags;
		BOOL bIgnoreRotation = !bRotate ||
			((ulCIFlags&CIF_IGNOREROTATION)|| 
			( (ulCIFlags&CIF_IGNOREHEADING) && 
				(en_mMoveRotation(1,2)==0&&en_mMoveRotation(2,2)==1&&en_mMoveRotation(3,2)==0) ));

		// create movement towards the new placement
		CClipMove cmMove(this);
		// clip the movement to the entity's world
		if (!bTranslate && bIgnoreRotation) {
			cmMove.cm_fMovementFraction = 2.0f;
		} else {
			en_pwoWorld->ClipMove(cmMove);
		}

		// if the move passes
		if (cmMove.cm_fMovementFraction>1.0f) {
//      CPrintF("  passed\n");
			//CPrintF("%d\n", MAXCOLLISIONRETRIES-(_ctTryToMoveCheckCounter+1));
			// if entity is in walking control now, but it might fall of an edge
			if (bTranslate && en_penReference!=NULL && 
				 (en_ulPhysicsFlags&EPF_TRANSLATEDBYGRAVITY) &&
				 !(en_ulPhysicsFlags&(EPF_ONSTEEPSLOPE|EPF_ORIENTINGTOGRAVITY|EPF_FLOATING)) &&
				 penPusher==NULL && WouldFallInNextPosition()) {
				// fail the movement
				SendEvent(EWouldFall());
				//CPrintF("  wouldfall\n");
				return FALSE;
			}
			// make entity use its new placement
			SetPlacementFromNextPosition();

			if (bTranslate) {
				if (penPusher==NULL) {
					en_vAppliedTranslation += en_vMoveTranslation;
				} else if (m_iPlayerRefCounter==0) {
					en_vAppliedTranslation += en_vMoveTranslation;
				}
			}

			if (m_iPlayerRefCounter>0) {
				m_iPlayerRefCounter--;
			}

			if (bRotate) {
				en_mAppliedRotation = en_mMoveRotation*en_mAppliedRotation;
			}
			// move is successful
			//CPrintF("  successful\n");
			return TRUE;

		// if the move is clipped
		} else {
			if (m_bFallDownToFloor)
			{
				m_bFallDownToFloor = FALSE;
			}
			
			// if must not retry
			if (_ctTryToMoveCheckCounter<=0) {
				// fail
				return FALSE;
			}

			// if hit brush
			if (cmMove.cm_pbpoHit!=NULL) {
				// if polygon is stairs, and the entity can climb stairs
				if ((cmMove.cm_pbpoHit->bpo_ulFlags&BPOF_STAIRS)
					&&((en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_CLIMBORSLIDE)) {

					// adjust against sliding upwards
					cmMove.cm_plClippedPlane = FLOATplane3D(-en_vGravityDir, 0);
				}

				// if cannot be damaged by impact
				INDEX iSurface = cmMove.cm_pbpoHit->bpo_bppProperties.bpp_ubSurfaceType;
				if (en_pwoWorld->wo_astSurfaceTypes[iSurface].st_ulFlags&STF_NOIMPACT) {
					// remember that
					en_ulPhysicsFlags|=EPF_NOIMPACTTHISTICK;
				}
			}

			// if entity is translated by gravity and 
			// the hit plane is more orthogonal to the gravity than the last one found
			if ((en_ulPhysicsFlags&EPF_TRANSLATEDBYGRAVITY) && !(en_ulPhysicsFlags&EPF_FLOATING)
			 && (
			 ((en_vGravityDir%(FLOAT3D&)cmMove.cm_plClippedPlane)
			 <(en_vGravityDir%en_vReferencePlane)) ) ) {
				// remember touched entity as stand-on reference
				en_penReference = cmMove.cm_penHit;

//        CPrintF("    newreference id%08x\n", en_penReference->en_ulID);
				en_vReferencePlane = (FLOAT3D&)cmMove.cm_plClippedPlane;

				en_pbpoStandOn = cmMove.cm_pbpoHit;  // is NULL if not hit a brush

				if (cmMove.cm_pbpoHit==NULL) {
					en_iReferenceSurface = 0;
				} else {
					en_iReferenceSurface = cmMove.cm_pbpoHit->bpo_bppProperties.bpp_ubSurfaceType;
				}
			}

			// send touch event to this entity and to touched entity
//안태훈 수정 시작	//(For Performance)(0.1)
			//SendTouchEvent(cmMove);
//안태훈 수정 끝	//(For Performance)(0.1)

			// if cannot be damaged by impact
			if (cmMove.cm_penHit->en_ulPhysicsFlags&EPF_NOIMPACT) {
				// remember that
				en_ulPhysicsFlags|=EPF_NOIMPACTTHISTICK;
			}

			// if entity bounces when blocked
			FLOAT3D vBounce;
			BOOL bBounce = FALSE;
			if ( ((en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_BOUNCE) && bTranslate) {
				// create translation speed for bouncing off clipping plane
				FLOAT3D vParallel, vNormal;
				GetParallelAndNormalComponents(en_vMoveTranslation, cmMove.cm_plClippedPlane, 
					vNormal, vParallel);
				vNormal   *= -en_fBounceDampNormal;
				vParallel *= +en_fBounceDampParallel;
				vBounce = vNormal+vParallel;
				// if not too small bounce
				if (vNormal.Length()>0.1f) {
					// do bounce
					bBounce = TRUE;
				}
				// rotate slower
				en_aDesiredRotationRelative *= en_fBounceDampParallel;
				if (en_aDesiredRotationRelative.Length()<10) {
					en_aDesiredRotationRelative = ANGLE3D(0,0,0);
				}
			}
			
			// if entity pushes when blocked and the blocking entity is pushable
			if (penPusher!=NULL&&(cmMove.cm_penHit->en_ulPhysicsFlags&EPF_PUSHABLE)) {
				CMovableModelEntity *penBlocking = ((CMovableModelEntity *)cmMove.cm_penHit);
				// create push translation to account for rotating radius
				FLOAT3D vRadius = cmMove.cm_penHit->en_plPlacement.pl_PositionVector-
																 penPusher->en_plPlacement.pl_PositionVector;
				FLOAT3D vPush=(vRadius*penPusher->en_mMoveRotation-vRadius);
					//*(1.01f-cmMove.cm_fMovementFraction);
				vPush += penPusher->en_vMoveTranslation;
					//*(1.01f-cmMove.cm_fMovementFraction);

				penBlocking->en_vMoveTranslation = vPush;
				penBlocking->en_mMoveRotation = penPusher->en_mMoveRotation;

				// make sure it is added to the movers list
				penBlocking->AddToMoversDuringMoving();
				// push the blocking entity
				BOOL bUnblocked = penBlocking->TryToMove(penPusher, bTranslate, bRotate);
				// if it has removed itself
				if (bUnblocked) {
					// retry the movement
					ClearNextPosition();
					return TryToMove(penPusher, bTranslate, bRotate);
				} else {
					// move is unsuccessful
					SendBlockEvent(cmMove);     
					ClearNextPosition();
					return FALSE;
				}
			// if entity slides if blocked
			} else if (
				((en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_SLIDE)||
				((en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_BOUNCE)||
				((en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_CLIMBORSLIDE)||
				((en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_STOPEXACT) ){

				// if translating
				if (bTranslate) {
	
					// create translation for sliding along clipping plane
					FLOAT3D vSliding;
					// if sliding along one plane
					if (_ctSliding==0) {
						// remember sliding parameters from the plane
						_vSlideOffDir = cmMove.cm_plClippedPlane;
						// get sliding velocity
						vSliding = cmMove.cm_plClippedPlane.ProjectDirection(
								en_vMoveTranslation*(1.0f-cmMove.cm_fMovementFraction));
//						en_vMoveTranslation);//1210
						_ctSliding++;
					// if second plane
					} else if (_ctSliding==1) {
						// off direction is away from both planes
						_vSlideOffDir+=cmMove.cm_plClippedPlane;
						// sliding direction is along both planes
						_vSlideDir = _vSlideOffDir*(FLOAT3D&)cmMove.cm_plClippedPlane;
						if (_vSlideDir.Length()>0.001f) {
							_vSlideDir.Normalize();
						}
						_ctSliding++;
						// get sliding velocity
						GetParallelComponent(en_vMoveTranslation*(1.0f-cmMove.cm_fMovementFraction),
							_vSlideDir, vSliding);
					// if more than two planes
					} else {
						// off direction is away from all planes
						_vSlideOffDir+=cmMove.cm_plClippedPlane;
						// sliding direction is along all planes
						_vSlideDir = cmMove.cm_plClippedPlane.ProjectDirection(_vSlideDir);
						_ctSliding++;
						// get sliding velocity
						GetParallelComponent(en_vMoveTranslation*(1.0f-cmMove.cm_fMovementFraction),
							_vSlideDir, vSliding);
					}
					ASSERT(IsValidFloat(vSliding(1)));
					ASSERT(IsValidFloat(_vSlideDir(1)));
					ASSERT(IsValidFloat(_vSlideOffDir(1)));

					// if entity hit a brush polygon
					if (cmMove.cm_pbpoHit!=NULL) {
						CSurfaceType &stHit = en_pwoWorld->wo_astSurfaceTypes[
							cmMove.cm_pbpoHit->bpo_bppProperties.bpp_ubSurfaceType];
						// if it is not beeing pushed, and it can climb stairs
						if (penPusher==NULL
						 &&(en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_CLIMBORSLIDE) {
							// NOTE: originally, the polygon's plane was considered here.
							// due to sphere-polygon collision algo, it is possible for the collision
							// plane to be even orthogonal to the polygon plane.
							// considering polygon's plane prevented climbing up the stairs.
							// so now, the collision plane is considered.
							// if there are any further problems, i recommend choosing
							// the plane that is more orthogonal to the movement direction.
							FLOAT3D &vHitPlane = (FLOAT3D&)cmMove.cm_plClippedPlane;//cmMove.cm_pbpoHit->bpo_pbplPlane->bpl_plAbsolute;
							BOOL bHitStairs = cmMove.cm_pbpoHit->bpo_ulFlags&BPOF_STAIRS;

					
							// if the plane hit is steep enough to climb on it 
							// (cannot climb low slopes as if those were stairs)
							if ((vHitPlane%en_vGravityDir>-stHit.st_fClimbSlopeCos)
								||bHitStairs) {
								m_bStair = TRUE;
								// if sliding along it would be mostly horizontal 
								// (i.e. cannot climb up the slope)
								FLOAT fSlidingVertical2 = en_vMoveTranslation%en_vGravityDir;
								fSlidingVertical2*=fSlidingVertical2;
								FLOAT fSliding2 = en_vMoveTranslation%en_vMoveTranslation;
								if ((2*fSlidingVertical2<=fSliding2)
								// if can go upstairs
									&& TryToGoUpstairs(en_vMoveTranslation, stHit, bHitStairs)) {
									// movement is ok
									return FALSE;
								}
							}
							else
							{
								m_bStair = FALSE;
							}
						}
					}

					// entity shouldn't really slide
					if ((en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_STOPEXACT) {
						// kill sliding
						vSliding = FLOAT3D(0,0,0);
					}

					ASSERT(IsValidFloat(vSliding(1)));

					// add a component perpendicular to the sliding plane
					vSliding += _vSlideOffDir*
						(ClampUp(en_vMoveTranslation.Length(), 0.5f)/100.0f);

					// if initial movement has some substantial value
					if(en_vMoveTranslation.Length()>0.001f && cmMove.cm_fMovementFraction>0.002f) {
						// go to where it is clipped (little bit before)
						vSliding += en_vMoveTranslation*(cmMove.cm_fMovementFraction*0.98f);
					}

					// ignore extremely small sliding
					if (vSliding.ManhattanNorm()<0.03f) {
						return FALSE;
					}

					// recurse
					en_vMoveTranslation = vSliding;
					ClearNextPosition();
					TryToMove(penPusher, bTranslate, bRotate);
					// if bouncer
					if (bBounce) {
						// remember bouncing speed for next tick
						en_vAppliedTranslation = vBounce;
						// no reference while bouncing
						en_penReference = NULL;
						en_vReferencePlane = FLOAT3D(0.0f, 0.0f, 0.0f);
						en_iReferenceSurface = 0;
					}

					// move is not entirely successful
					return FALSE;

				// if rotating
				} else if (bRotate) {
					// if bouncing entity
					if ((en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_BOUNCE) {
						// rotate slower
						en_aDesiredRotationRelative *= en_fBounceDampParallel;
						if (en_aDesiredRotationRelative.Length()<10) {
							en_aDesiredRotationRelative = ANGLE3D(0,0,0);
						}
						// move is not successful
						return FALSE;
					}
					// create movement getting away from the collision point
					en_vMoveTranslation = cmMove.cm_vClippedLine*-1.2f;
					// recurse
					ClearNextPosition();
					TryToMove(penPusher, TRUE, bRotate);
					// move is not entirely successful
					return FALSE;
				}
				// not translating and not rotating? -  move is unsuccessful
				return FALSE;

			// if entity has some other behaviour when blocked
			} else {
				// move is unsuccessful (EPF_ONBLOCK_STOP is assumed)
				SendBlockEvent(cmMove);
				ClearNextPosition();
				return FALSE;
			}
		}
	}


	// clear eventual temporary variables that are not persistent
	export void ClearMovingTemp(void)
	{
//    return;
//    CLEARMEM(en_vIntendedTranslation);
//    CLEARMEM(en_mIntendedRotation);
			ClearNextPosition();
		CLEARMEM(en_vMoveTranslation);
		CLEARMEM(en_mMoveRotation);
		CLEARMEM(en_vAppliedTranslation);
		CLEARMEM(en_mAppliedRotation);
	}

	// prepare parameters for moving in this tick
	export void PreMoving(void)
	{
/* //0311 경사면관련 Undo
		if(GetFlags() & ENF_ALIVE )
		{			
			BOOL bMyCha = TRUE;
			en_ulPhysicsFlags = en_ulPhysicsFlags & ~EPF_ONBLOCK_BOUNCE;			
			en_ulPhysicsFlags = en_ulPhysicsFlags & ~EPF_ORIENTEDBYGRAVITY;
			en_ulPhysicsFlags = en_ulPhysicsFlags & ~EPF_PUSHABLE;
			en_ulPhysicsFlags = en_ulPhysicsFlags | EPF_ONBLOCK_CLIMBORSLIDE;
		}
*/
		if (en_pciCollisionInfo==NULL) {
			return;
		}
 


		//현재의 위치를 기억해놓는다.
		// remember old placement for lerping

		en_plLastPlacement = en_plPlacement;


		//차일드의 위치 기억
		// for each child of the mover 
		{FOREACHINLIST(CEntity, en_lnInParent, en_lhChildren, itenChild) {
			// if the child is movable, yet not in movers list
			if ((itenChild->en_ulPhysicsFlags&EPF_MOVABLE)
				&&!((CMovableEntity*)&*itenChild)->en_lnInMovers.IsLinked() || itenChild->IsPlayer()) {
				CMovableEntity *penChild = ((CMovableEntity*)&*itenChild);
				// remember old placement for lerping
				penChild->en_plLastPlacement = penChild->en_plPlacement;  
			}
		}}

		FLOAT fTickQuantum=_pTimer->TickQuantum; // used for normalizing from SI units to game ticks

		// trig break point if required
		if( dbg_bBreak) {
			dbg_bBreak = FALSE; // auto turn off breakpoint when triggered
			try { 
				Breakpoint();
			} catch (ANYEXCEPTION) {
				CPrintF("Breakpoint!\n");
			};
		}

		//속도의 한계 설정
		// NOTE: this limits maximum velocity of any entity in game.
		// it is absolutely neccessary in order to prevent extreme slowdowns in physics.
		// if you plan to increase this one radically, consider decreasing 
		// collision grid cell size!
		// currently limited to a bit less than speed of sound (not that it is any specificaly
		// relevant constant, but it is just handy)
		const FLOAT fMaxSpeed = 300.0f;
		en_vCurrentTranslationAbsolute(1)=Clamp(en_vCurrentTranslationAbsolute(1), -fMaxSpeed, +fMaxSpeed);
		en_vCurrentTranslationAbsolute(2)=Clamp(en_vCurrentTranslationAbsolute(2), -fMaxSpeed, +fMaxSpeed);
		en_vCurrentTranslationAbsolute(3)=Clamp(en_vCurrentTranslationAbsolute(3), -fMaxSpeed, +fMaxSpeed);

		// if the entity is a model
		if (en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL ||
				en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL) {
			// test for field containment
			TestFields(en_iUpContent, en_iDnContent, en_fImmersionFactor);
			// if entity has sticky feet
			if (en_ulPhysicsFlags & EPF_STICKYFEET) {
				// find gravity towards nearest polygon
				FLOAT3D vPoint;
				FLOATplane3D plPlane;
				FLOAT fDistanceToEdge;
				if (GetNearestPolygon(vPoint, plPlane, fDistanceToEdge)) {
					en_vGravityDir = -(FLOAT3D&)plPlane;
				}
			}
		}
		CContentType &ctDn = en_pwoWorld->wo_actContentTypes[en_iDnContent];
		CContentType &ctUp = en_pwoWorld->wo_actContentTypes[en_iUpContent];

		// test entity breathing
		TestBreathing(ctUp);
		// test content damage
		TestContentDamage(ctDn, en_fImmersionFactor);
		// test surface damage
		if (en_penReference!=NULL) {
			CSurfaceType &stReference = en_pwoWorld->wo_astSurfaceTypes[en_iReferenceSurface];
			TestSurfaceDamage(stReference);
		}
	 
		// calculate content fluid factors
		FLOAT fBouyancy = (1-
			(ctDn.ct_fDensity/en_fDensity)*en_fImmersionFactor-
			(ctUp.ct_fDensity/en_fDensity)*(1-en_fImmersionFactor));
		FLOAT fSpeedModifier = 
			ctDn.ct_fSpeedMultiplier*en_fImmersionFactor+
			ctUp.ct_fSpeedMultiplier*(1-en_fImmersionFactor);
		FLOAT fFluidFriction =
			ctDn.ct_fFluidFriction*en_fImmersionFactor+
			ctUp.ct_fFluidFriction*(1-en_fImmersionFactor);
		FLOAT fControlMultiplier =
			ctDn.ct_fControlMultiplier*en_fImmersionFactor+
			ctUp.ct_fControlMultiplier*(1-en_fImmersionFactor);
/*

		if(IsEnemy() || GetFlags() & ENF_CHARACTER)//1117
		{
			SLONG currentTime = GetTickCount()%1000;
			if(m_tmMoveStart!=0 && currentTime - m_tmMoveStart > 0.05)
			{
				en_vDesiredTranslationRelative = en_vDesiredTranslationRelative * ((FLOAT)(currentTime - m_tmMoveStart))/1000.0f/0.05f;				
			}
			
			m_tmMoveStart = currentTime;
		}
*/
		// transform relative desired translation into absolute
		FLOAT3D vDesiredTranslationAbsolute = en_vDesiredTranslationRelative;
		// relative absolute
		if (!(en_ulPhysicsFlags & EPF_ABSOLUTETRANSLATE)) {
//			vDesiredTranslationAbsolute *= en_mRotation; // EPF_ABSOLUTETRANSLATE 플래그가 적용이 안됨
		}
		//1tick동안 갈 거리.
		// transform translation and rotation into tick time units
		vDesiredTranslationAbsolute*=fTickQuantum;



		ANGLE3D aRotationRelative;
		aRotationRelative(1) = en_aDesiredRotationRelative(1)*fTickQuantum;
		aRotationRelative(2) = en_aDesiredRotationRelative(2)*fTickQuantum;
		aRotationRelative(3) = en_aDesiredRotationRelative(3)*fTickQuantum;
		// make absolute matrix rotation from relative angle rotation
		FLOATmatrix3D mRotationAbsolute;

		if ((en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_PUSH) {
			FLOATmatrix3D mNewRotation;
			MakeRotationMatrixFast(mNewRotation, en_plPlacement.pl_OrientationAngle+aRotationRelative);
			mRotationAbsolute = mNewRotation*!en_mRotation;

		} else {
			MakeRotationMatrixFast(mRotationAbsolute, aRotationRelative);
			mRotationAbsolute = en_mRotation*(mRotationAbsolute*!en_mRotation);
		}

		// modify desired speed for fluid parameters
		vDesiredTranslationAbsolute*=fSpeedModifier;

		// remember jumping strength (if any)
		FLOAT fJump = -en_mRotation.GetColumn(2)%vDesiredTranslationAbsolute;

		m_bReferenceMovingInY = FALSE;
		m_bReferenceRotatingNonY = FALSE;
		// if we have a CMovableEntity for a reference entity
	 if (en_penReference!=NULL && (en_penReference->en_ulPhysicsFlags&EPF_MOVABLE) &&
			((en_penReference->en_ulPhysicsFlags&EPF_ONBLOCK_MASK)!=EPF_ONBLOCK_BOUNCE)) {
			CMovableEntity *penReference = (CMovableEntity *)(CEntity*)en_penReference;
			// get reference deltas for this tick
			const FLOAT3D &vReferenceTranslation = penReference->en_vIntendedTranslation;
			const FLOATmatrix3D &mReferenceRotation = penReference->en_mIntendedRotation;
			// calculate radius of this entity relative to reference
			FLOAT3D vRadius = en_plPlacement.pl_PositionVector
					-penReference->en_plPlacement.pl_PositionVector;
			FLOAT3D vReferenceDelta = vReferenceTranslation + vRadius*mReferenceRotation - vRadius;
			// add the deltas to this entity
			//vDesiredTranslationAbsolute += vReferenceDelta;
			//mRotationAbsolute = mReferenceRotation*mRotationAbsolute;

			// remember if reference is moving in y
			m_bReferenceMovingInY = (vReferenceDelta%en_vGravityDir != 0.0f);
			m_bReferenceRotatingNonY = ((en_vGravityDir*mReferenceRotation)%en_vGravityDir)>0.01f;
		}

		FLOAT3D vTranslationAbsolute = en_vCurrentTranslationAbsolute*fTickQuantum;

		// initially not orienting
		en_ulPhysicsFlags&=~EPF_ORIENTINGTOGRAVITY;
		// if the entity is rotated by gravity
		if (en_ulPhysicsFlags&EPF_ORIENTEDBYGRAVITY) {
			// find entity's down vector
			FLOAT3D vDown;
			vDown(1) = -en_mRotation(1,2);
			vDown(2) = -en_mRotation(2,2);
			vDown(3) = -en_mRotation(3,2);

			// find angle entities down and gravity down
			FLOAT fCos = vDown%en_vGravityDir;
			// if substantial
			if (fCos<0.99999f) {
				// mark
				en_ulPhysicsFlags|=EPF_ORIENTINGTOGRAVITY;

				// limit the angle rotation
				ANGLE a = ACos(fCos);
				if (Abs(a)>20) {
					a = 20*Sgn(a);
				}
				FLOAT fRad =RadAngle(a);

				// make rotation axis
				FLOAT3D vAxis = vDown*en_vGravityDir;
				FLOAT fLen = vAxis.Length();
				if (fLen<0.01f) {
					vAxis(1) = en_mRotation(1,3);
					vAxis(2) = en_mRotation(2,3);
					vAxis(3) = en_mRotation(3,3);
				// NOTE: must have this patch for smooth rocking on moving brushes
				// (should infact do fRad/=fLen always)
				} else if (!m_bReferenceRotatingNonY) {
					fRad/=fLen;
				}
				vAxis*=fRad;

				// make rotation matrix
				FLOATmatrix3D mGRotation;
				mGRotation(1,1) =  1;        mGRotation(1,2) = -vAxis(3); mGRotation(1,3) =  vAxis(2);
				mGRotation(2,1) =  vAxis(3); mGRotation(2,2) =  1;        mGRotation(2,3) = -vAxis(1);
				mGRotation(3,1) = -vAxis(2); mGRotation(3,2) =  vAxis(1); mGRotation(3,3) = 1;
				OrthonormalizeRotationMatrix(mGRotation);

				// add the gravity rotation
				mRotationAbsolute = mGRotation*mRotationAbsolute;
			}
		}

		// initially not floating
		en_ulPhysicsFlags&=~EPF_FLOATING;

		en_fAcceleration = 10000.0f; 
		en_fDeceleration = 10000.0f;

		if (en_pMobTarget != NULL)
		{
			CMobData* MD = CMobData::getData(en_pMobTarget->m_nType);

			if((!(GetFlags() & ENF_ALIVE) && MD->IsMovable()) || (_pNetwork->m_bSingleMode && IsEnemy()))
			{
				en_fAcceleration = 200.0f; 
				en_fDeceleration = 40.0f;
			}
		}

		FLOAT ACC=en_fAcceleration*fTickQuantum*fTickQuantum;
		FLOAT DEC=en_fDeceleration*fTickQuantum*fTickQuantum;
		// if the entity is not affected by gravity
		if (!(en_ulPhysicsFlags&EPF_TRANSLATEDBYGRAVITY)) {
			// accellerate towards desired absolute translation
			if (en_ulPhysicsFlags&EPF_NOACCELERATION) {
				vTranslationAbsolute = vDesiredTranslationAbsolute;
			} else {
				AddAcceleration(vTranslationAbsolute, vDesiredTranslationAbsolute, 
					ACC*fControlMultiplier,
					DEC*fControlMultiplier);
			}
		// if swimming
		} else if ((fBouyancy*en_fGravityA<0.5f && (ctDn.ct_ulFlags&(CTF_SWIMABLE|CTF_FLYABLE)))) {
			// mark that
			en_ulPhysicsFlags|=EPF_FLOATING;
			// accellerate towards desired absolute translation
			if (en_ulPhysicsFlags&EPF_NOACCELERATION) {
				vTranslationAbsolute = vDesiredTranslationAbsolute;
			} else {
				AddAcceleration(vTranslationAbsolute, vDesiredTranslationAbsolute, 
					ACC*fControlMultiplier,
					DEC*fControlMultiplier);
			}

			// add gravity acceleration
			if (fBouyancy<-0.1f) {
				FLOAT fGV=en_fGravityV*fTickQuantum*fSpeedModifier;
				FLOAT fGA=(en_fGravityA*-fBouyancy)*fTickQuantum*fTickQuantum;
				AddAcceleration(vTranslationAbsolute, en_vGravityDir*-fGV, fGA, fGA);
			} else if (fBouyancy>+0.1f) {
				FLOAT fGV=en_fGravityV*fTickQuantum*fSpeedModifier;
				FLOAT fGA=(en_fGravityA*fBouyancy)*fTickQuantum*fTickQuantum;
				AddAcceleration(vTranslationAbsolute, en_vGravityDir*fGV, fGA, fGA);
			}

		// if the entity is affected by gravity
		} else {
			BOOL bGravityAlongPolygon = TRUE;
			// if there is no fixed remembered stand-on polygon or the entity is not on it anymore
			if (en_pbpoStandOn==NULL || !IsStandingOnPolygon(en_pbpoStandOn) || m_bReferenceMovingInY
				|| (en_ulPhysicsFlags&EPF_ORIENTINGTOGRAVITY)) {
				// clear the stand on polygon
				en_pbpoStandOn=NULL;
				if (en_penReference == NULL || m_bReferenceMovingInY) {
					bGravityAlongPolygon = FALSE;
				}
			}
	  
	  //0216 미끄러지 않게 하기.
	  bGravityAlongPolygon = FALSE;

			// if gravity can cause the entity to fall
			if (!bGravityAlongPolygon) {
				// add gravity acceleration
				FLOAT fGV=en_fGravityV*fTickQuantum*fSpeedModifier;//1210
				FLOAT fGA=(en_fGravityA*fBouyancy)*fTickQuantum*fTickQuantum;
/*
				if(en_penReference==NULL)//1210
				{
					vTranslationAbsolute(2) = -1.0f;
				}
*/
				AddGAcceleration(vTranslationAbsolute, en_vGravityDir, fGA, fGV);
			// if entity can only slide down its stand-on polygon
			} else {
				// disassemble gravity to parts parallel and normal to plane
				FLOAT3D vPolygonDir = -en_vReferencePlane;
				// NOTE: normal to plane=paralel to plane normal vector!
				FLOAT3D vGParallel, vGNormal;
				GetParallelAndNormalComponents(en_vGravityDir, vPolygonDir, vGNormal, vGParallel);
				// add gravity part parallel to plane
				FLOAT fFactor = vGParallel.Length();

				if (fFactor>0.001f) {
					FLOAT fGV=en_fGravityV*fTickQuantum*fSpeedModifier;
					FLOAT fGA=(en_fGravityA*fBouyancy)*fTickQuantum*fTickQuantum;
					AddGAcceleration(vTranslationAbsolute, vGParallel/fFactor, fGA*fFactor, fGV*fFactor);
				}

				// kill your normal-to-polygon speed if towards polygon and small
				FLOAT fPolyGA = (vPolygonDir%en_vGravityDir)*en_fGravityA;
				FLOAT fYSpeed = vPolygonDir%vTranslationAbsolute;
				if (fYSpeed>0 && fYSpeed < fPolyGA) {
					vTranslationAbsolute -= vPolygonDir*fYSpeed;
				}

				// if a bouncer
				if ((en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_BOUNCE) {
					// rotate slower
					en_aDesiredRotationRelative *= en_fJumpControlMultiplier;
					if (en_aDesiredRotationRelative.Length()<10) {
						en_aDesiredRotationRelative = ANGLE3D(0,0,0);
					}
				}
			}

			CSurfaceType &stReference = en_pwoWorld->wo_astSurfaceTypes[en_iReferenceSurface];

			// if it has a reference entity
			if (en_penReference!=NULL) {
				FLOAT fPlaneY = (en_vGravityDir%en_vReferencePlane);
				FLOAT fPlaneYAbs = Abs(fPlaneY);
				FLOAT fFriction = stReference.st_fFriction;
/* //0311 경사면관련 Undo		
				if((GetFlags() & ENF_ALIVE && !IsEnemy() && !IsCharacter())&& (en_vDesiredDirection.Length() != 0))
				{				
					FLOAT cosA;
					FLOAT3D _vReferencePlane = en_vReferencePlane;
					_vReferencePlane(2)=0.0f;
					cosA = en_vDesiredDirection%_vReferencePlane;

					m_bForceStop = FALSE;

					if(0>cosA && cosA >= -1)//올라가려고 시도.
					{					
							if (fPlaneY>=-stReference.st_fClimbSlopeCos&&fPlaneY<0)//올라가면 안된다.
							{
								
								if(!m_bBlockDown)//
								{													
									if(!m_bStair)
									{
									ClearMovingTemp();
									ForceFullStop();
									en_vIntendedTranslation = FLOAT3D(0,0,0);
									m_bForceStop = TRUE;
									m_bBlockUp = TRUE;
									return;
									}
								}
								else//내려가다가 걸린상태이므로 올라가야 한다.
								{
									m_bForceStop = FALSE;
								}
							}
							else
							{
								m_bBlockDown = FALSE;
								m_bForceStop = FALSE;
								m_bBlockUp = FALSE;							
							}
						
					}
					else if(cosA>=0 && cosA<=1)//내려가려고 시도.
					{					
							if (fPlaneY>=-stReference.st_fClimbSlopeCos&&fPlaneY<0)//내려가면 안된다.
							{
								if(!m_bBlockUp)//
								{														
									ClearMovingTemp();
									ForceFullStop();
									en_vIntendedTranslation = FLOAT3D(0,0,0);
									m_bForceStop = TRUE;
									m_bBlockDown = TRUE;
									return;
								}
								else//올라가다가 걸린상태이므로 내려가야 한다.
								{
									m_bForceStop = FALSE;
								}
							}
							else
							{							
								m_bBlockUp = FALSE;
								m_bForceStop = FALSE;
								m_bBlockDown = FALSE;
							}
					
					}
				}

				if((GetFlags() & ENF_ALIVE && !IsEnemy() && !IsCharacter()) && m_bForceStop)
				{
					return;
				}
*/

				// if on a steep slope
				if (fPlaneY>=-stReference.st_fClimbSlopeCos&&fPlaneY<0
					||(stReference.st_ulFlags&STF_SLIDEDOWNSLOPE)&&fPlaneY>-0.99f) {
					{

						if(GetFlags() & ENF_ALIVE && !IsEnemy())	
						{
						en_ulPhysicsFlags|=EPF_ONSTEEPSLOPE;
						// accellerate horizontaly towards desired absolute translation

						AddAccelerationOnPlane2(
							vTranslationAbsolute, 
							vDesiredTranslationAbsolute,
							ACC*fPlaneYAbs*fPlaneYAbs*fFriction*fControlMultiplier,
							DEC*fPlaneYAbs*fPlaneYAbs*fFriction*fControlMultiplier,
							en_vReferencePlane,
							en_vGravityDir);
						}
						else
						{
							en_ulPhysicsFlags&=~EPF_ONSTEEPSLOPE;
						// accellerate on plane towards desired absolute translation
						AddAccelerationOnPlane(
							vTranslationAbsolute, 
							vDesiredTranslationAbsolute,
							ACC*fPlaneYAbs*fPlaneYAbs*fFriction*fControlMultiplier,
							DEC*fPlaneYAbs*fPlaneYAbs*fFriction*fControlMultiplier,
							en_vReferencePlane);
						}

/*
						en_ulPhysicsFlags&=~EPF_ONSTEEPSLOPE;
					// accellerate on plane towards desired absolute translation
					AddAccelerationOnPlane(
						vTranslationAbsolute, 
						vDesiredTranslationAbsolute,
						ACC*fPlaneYAbs*fPlaneYAbs*fFriction*fControlMultiplier,
						DEC*fPlaneYAbs*fPlaneYAbs*fFriction*fControlMultiplier,
						en_vReferencePlane);
*/
					}

				// if not on a steep slope
				} else 

				{
					FLOAT3D vPreTranslationAbs = vTranslationAbsolute;
					en_plLastPlacementTmp = en_plLastPlacement;
					en_penLastValidReferenceTmp = en_penReference;
					m_bForceStop = FALSE;//1210
					en_ulPhysicsFlags&=~EPF_ONSTEEPSLOPE;
					// accellerate on plane towards desired absolute translation
					AddAccelerationOnPlane(
						vTranslationAbsolute, 
						vDesiredTranslationAbsolute,
						ACC*fPlaneYAbs*fPlaneYAbs*fFriction*fControlMultiplier,
						DEC*fPlaneYAbs*fPlaneYAbs*fFriction*fControlMultiplier,
						en_vReferencePlane);

					if (m_bFallDownToFloor)
					{
						vTranslationAbsolute(1) = 0.0f;
						vTranslationAbsolute(2) = vPreTranslationAbs(2);
						vTranslationAbsolute(3) = 0.0f;
					}
				}
				// if wants to jump and can jump
				if (fJump<-0.01f && (fPlaneY<-stReference.st_fJumpSlopeCos
					|| en_tmEntityTime>en_tmLastSignificantVerticalMovement+0.25f) ) {
					// jump
					vTranslationAbsolute += en_vGravityDir*fJump;
					en_tmJumped = en_tmEntityTime;
					en_pbpoStandOn = NULL;
				}

			// if it doesn't have a reference entity
			} else {//if (en_penReference==NULL) 

				m_bForceStop = FALSE;
				// if can control after jump
				if (en_tmEntityTime-en_tmJumped<en_tmMaxJumpControl) {
					// accellerate horizontaly, but slower
					AddAccelerationOnPlane(
						vTranslationAbsolute, 
						vDesiredTranslationAbsolute,
						ACC*fControlMultiplier*en_fJumpControlMultiplier,
						DEC*fControlMultiplier*en_fJumpControlMultiplier,
						FLOATplane3D(en_vGravityDir, 0));
				}

				// if wants to jump and can jump
				if (fJump<-0.01f && 
					en_tmEntityTime>en_tmLastSignificantVerticalMovement+0.25f) {
					// jump
					vTranslationAbsolute += en_vGravityDir*fJump;
					en_tmJumped = en_tmEntityTime;
					en_pbpoStandOn = NULL;
				}
			}
		}

		// check for force-field acceleration
		// NOTE: pulled out because of a bug in VC code generator, see function comments above
		CheckAndAddGAcceleration(this, vTranslationAbsolute, fTickQuantum);

		// if there is fluid friction involved
		if (fFluidFriction>0.01f) {
			// slow down
			AddAcceleration(vTranslationAbsolute, FLOAT3D(0.0f, 0.0f, 0.0f),
				0.0f, DEC*fFluidFriction);
		}

		// if may slow down spinning
		if ( (en_ulPhysicsFlags& EPF_CANFADESPINNING) &&
			( (ctDn.ct_ulFlags&CTF_FADESPINNING) || (ctUp.ct_ulFlags&CTF_FADESPINNING) ) ) {
			// reduce desired rotation
			en_aDesiredRotationRelative *= (1-fSpeedModifier*0.05f);
			if (en_aDesiredRotationRelative.Length()<10) {
				en_aDesiredRotationRelative = ANGLE3D(0,0,0);
			}
		}

		// discard reference entity (will be recalculated)
		if (en_pbpoStandOn==NULL && (vTranslationAbsolute.ManhattanNorm()>1E-5f || 
			en_vReferencePlane%en_vGravityDir<0.0f)) {
			en_penReference = NULL;
			en_vReferencePlane = FLOAT3D(0.0f, 0.0f, 0.0f);
			en_iReferenceSurface = 0;
		}

		en_vIntendedTranslation = vTranslationAbsolute;
		en_mIntendedRotation = mRotationAbsolute;

		//-- estimate future movements for collision caching

		// make box of the entity for its current rotation
		FLOATaabbox3D box;
		en_pciCollisionInfo->MakeBoxAtPlacement(FLOAT3D(0,0,0), en_mRotation, box);
		// if it is a light source
		{CLightSource *pls = GetLightSource();
		if (pls!=NULL && !(pls->ls_ulFlags&LSF_LENSFLAREONLY)) {
			// expand the box to be sure that it contains light range
			ASSERT(!(pls->ls_ulFlags&LSF_DIRECTIONAL));
			box |= FLOATaabbox3D(FLOAT3D(0,0,0), pls->ls_rFallOff);
		}}
		// add a bit around it
		box.ExpandByFactor( phy_fCollisionCacheAround-1.0f);
		// make box go few ticks ahead of the entity
		box += en_plPlacement.pl_PositionVector;
		en_boxMovingEstimate  = box;
		box += en_vIntendedTranslation*phy_fCollisionCacheAhead;
		en_boxMovingEstimate |= box;

		// clear applied movement to be updated during movement
		en_vAppliedTranslation = FLOAT3D(0.0f, 0.0f, 0.0f);
		en_mAppliedRotation.Diagonal(1.0f);
	}

	/* Calculate physics for moving. */
	export void DoMoving(void)
	{
		if (en_pciCollisionInfo==NULL || (en_ulPhysicsFlags&EPF_FORCEADDED)) {
			return;
		}

		if(m_bForceStop)//0120
		{
			return;
		}

/*
		if(en_vDesiredTranslationRelative.Length()==0)
		{
			return;
		}
*/
		FLOAT fTickQuantum=_pTimer->TickQuantum; // used for normalizing from SI units to game ticks
		// if rotation and translation are synchronized
		if (en_ulPhysicsFlags&EPF_RT_SYNCHRONIZED) {
			// move both in translation and rotation
			en_vMoveTranslation = en_vIntendedTranslation-en_vAppliedTranslation;
			en_mMoveRotation = en_mIntendedRotation*!en_mAppliedRotation;

			InitTryToMove();
			CMovableEntity *penPusher = NULL;
			if ((en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_PUSH) {
				penPusher = this;
			}
			TryToMove(penPusher, TRUE, TRUE);


		// if rotation and translation are asynchronious
		} else {
			ASSERT((en_ulPhysicsFlags&EPF_ONBLOCK_MASK)!=EPF_ONBLOCK_PUSH);
			// if there is no reference
			if (en_penReference == NULL) {
				// try to do simple move both in translation and rotation
				en_vMoveTranslation = en_vIntendedTranslation-en_vAppliedTranslation;
				en_mMoveRotation = en_mIntendedRotation*!en_mAppliedRotation;
				InitTryToMove();
				_ctTryToMoveCheckCounter = 4; // no retries
				BOOL bMoveSuccessfull = TryToMove(NULL, TRUE, TRUE);
				// if it passes
				if (bMoveSuccessfull) {
					// finish
					return;
				}
			}

			// translate
			en_vMoveTranslation = en_vIntendedTranslation-en_vAppliedTranslation;
			InitTryToMove();
			TryToMove(NULL, TRUE, FALSE);

			// rotate
			en_mMoveRotation = en_mIntendedRotation*!en_mAppliedRotation;
			if (
					(en_mMoveRotation(1,1)>1.0001f || en_mMoveRotation(1,1)<0.9999f) ||
					(en_mMoveRotation(1,2)>0.0f || en_mMoveRotation(1,2)<0.0f) ||
					(en_mMoveRotation(1,3)>0.0f || en_mMoveRotation(1,3)<0.0f) ||
					(en_mMoveRotation(2,1)>0.0f || en_mMoveRotation(2,1)<0.0f) ||
					(en_mMoveRotation(2,2)>1.0001f || en_mMoveRotation(2,2)<0.9999f) ||
					(en_mMoveRotation(2,3)>0.0f || en_mMoveRotation(2,3)<0.0f) ||
					(en_mMoveRotation(3,1)>0.0f || en_mMoveRotation(3,1)<0.0f) ||
					(en_mMoveRotation(3,2)>0.0f || en_mMoveRotation(3,2)<0.0f) ||
					(en_mMoveRotation(3,3)>1.0001f || en_mMoveRotation(3,3)<0.9999f) )
			{
				CTString tStr;
				tStr.PrintF("%f",en_mMoveRotation(3,3));
				InitTryToMove();
				TryToMove(NULL, FALSE, TRUE);
			}
		}
	}

	// calculate consequences of moving/not moving in this tick
	export void PostMoving(void) 
	{
		en_tmEntityTime += _pTimer->TickQuantum;

		if(m_bForceStop)//0120
		{
			return;
		}

		if (en_pciCollisionInfo==NULL) {
			// mark for removing from list of movers
			en_ulFlags |= ENF_INRENDERING;
			return;
		}

		if (en_ulPhysicsFlags&EPF_FORCEADDED) {
			en_ulPhysicsFlags&=~EPF_FORCEADDED;
			return;
		}

		// remember valid reference if valid
		if (en_penReference!=NULL) {
			en_penLastValidReference = en_penReference;
		}

		// remember original translation
		FLOAT3D vOldTranslation = en_vCurrentTranslationAbsolute;
		FLOAT fTickQuantum=_pTimer->TickQuantum; // used for normalizing from SI units to game ticks
		// calculate current velocity from movements applied in this tick
		en_vCurrentTranslationAbsolute = en_vAppliedTranslation/fTickQuantum;

		// remember significant movements
		if (Abs(en_vCurrentTranslationAbsolute%en_vGravityDir)>0.1f) {
			en_tmLastSignificantVerticalMovement = en_tmEntityTime;
		}

		ClearNextPosition();

		// calculate speed change between needed and possible (in m/s)
		FLOAT3D vSpeedDelta = en_vIntendedTranslation - en_vAppliedTranslation;
		FLOAT fSpeedDelta = vSpeedDelta.Length()/fTickQuantum;

/*
//강동민 수정 시작 싱글 던젼 작업	07.29
		// if it is large change and can be damaged by impact
		if (fSpeedDelta>en_fCollisionSpeedLimit &&
			 !(en_ulPhysicsFlags&EPF_NOIMPACTTHISTICK)) 
			 {
			// inflict impact damage 
			FLOAT fDamage = ((fSpeedDelta-en_fCollisionSpeedLimit)/en_fCollisionSpeedLimit)*en_fCollisionDamageFactor;
			InflictDirectDamage(this, MiscDamageInflictor(), DMT_IMPACT, fDamage, 
				en_plPlacement.pl_PositionVector, -vSpeedDelta.Normalize());
		}
//강동민 수정 끝 싱글 던젼 작업		07.29
*/
		en_ulPhysicsFlags&=~EPF_NOIMPACTTHISTICK;

		// remember old speed for Touch reactions
		en_vIntendedTranslation = vOldTranslation;

		// if not moving anymore
		if (en_vCurrentTranslationAbsolute.ManhattanNorm()<0.001f
			&&(en_vDesiredTranslationRelative.ManhattanNorm()==0 || en_fAcceleration==0)
			&&en_aDesiredRotationRelative.ManhattanNorm()==0) {

			// if there is a reference
			if (en_penReference!=NULL) {
				// it the reference is movable
				if (en_penReference->en_ulPhysicsFlags&EPF_MOVABLE) {
					CMovableEntity *penReference = (CMovableEntity *)(CEntity*)en_penReference;
					// if the reference is not in the list of movers
					if (!penReference->en_lnInMovers.IsLinked()) {
						// mark for removing from list of movers
						en_ulFlags |= ENF_INRENDERING;
					}
				// if the reference is not movable
				} else {
					// mark for removing from list of movers
					en_ulFlags |= ENF_INRENDERING;
				}

			// if there is no reference
			} else {
				// if no gravity and no forces can affect this entity
				if (
					(!(en_ulPhysicsFlags&(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY))
						|| en_fGravityA==0.0f || (en_ulPhysicsFlags&EPF_FLOATING))) { // !!!! test for forces also when implemented
					// mark for removing from list of movers
					en_ulFlags |= ENF_INRENDERING;
				}
			}

			// if should remove from movers list
			if (en_ulFlags&ENF_INRENDERING) {
				// clear last placement
				en_plLastPlacement = en_plPlacement;
			}
		}

		// remember new position for particles
		if (en_plpLastPositions!=NULL) {
			en_plpLastPositions->AddPosition(en_vNextPosition);
		}
		//CPrintF("\n%f", _pTimer->CurrentTick());
	}

	// call this if you move without collision
	export void CacheNearPolygons(void) 
	{
		CClipMove cm(this);
		cm.CacheNearPolygons();
	}


	// returns bytes of memory used by this object
	SLONG GetUsedMemory(void)
	{
		// init
		SLONG slUsedMemory = sizeof(CMovableEntity) - sizeof(CRationalEntity) + CRationalEntity::GetUsedMemory();
		// add some more
		slUsedMemory += en_apbpoNearPolygons.sa_Count * sizeof(CBrushPolygon*);
		return slUsedMemory;
	}


	// this is used on client side only
	void ClientCacheNearPolygons() {
		// make box of the entity for its current rotation
		FLOATaabbox3D box;
		if (en_pciCollisionInfo==NULL) {return;}
		en_pciCollisionInfo->MakeBoxAtPlacement(FLOAT3D(0,0,0), en_mRotation, box);
		// if it is a light source
		{CLightSource *pls = GetLightSource();
		if (pls!=NULL && !(pls->ls_ulFlags&LSF_LENSFLAREONLY)) {
			// expand the box to be sure that it contains light range
			ASSERT(!(pls->ls_ulFlags&LSF_DIRECTIONAL));
			box |= FLOATaabbox3D(FLOAT3D(0,0,0), pls->ls_rFallOff);
		}}
		// add a bit around it
		extern FLOAT phy_fCollisionCacheAround;
		box.ExpandByFactor( phy_fCollisionCacheAround-1.0f);
		// make box go few ticks ahead of the entity
		box += en_plPlacement.pl_PositionVector;
		en_boxMovingEstimate  = box;

		CClipMove cmClipMove(this);
		cmClipMove.cm_pwoWorld = &(_pNetwork->ga_World);

		// prepare flags masks for testing which entities collide with this
		cmClipMove.cm_ulTestMask1 = ((en_ulCollisionFlags&ECF_TESTMASK)>>ECB_TEST)<<ECB_IS;
		cmClipMove.cm_ulTestMask2 = ((en_ulCollisionFlags&ECF_ISMASK  )>>ECB_IS  )<<ECB_TEST;

		cmClipMove.cm_ulPassMaskA = ((en_ulCollisionFlags&ECF_PASSMASK)>>ECB_PASS)<<ECB_IS;
		cmClipMove.cm_ulPassMaskB = ((en_ulCollisionFlags&ECF_ISMASK  )>>ECB_IS  )<<ECB_PASS;

		cmClipMove.CacheNearPolygons();

	}


procedures:


	// must have at least one procedure per class
	Dummy() {};
};
