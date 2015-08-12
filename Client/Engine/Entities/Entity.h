#ifndef SE_INCL_ENTITY_H
#define SE_INCL_ENTITY_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <Engine/Base/Lists.h>
#include <Engine/Base/Relations.h>
#include <Engine/Templates/StaticStackArray.h>
#include <Engine/Templates/Selection.h>
#include <Engine/Math/Matrix.h>
#include <Engine/Math/AABBox.h>
#include <Engine/Math/Placement.h>
#include <Engine/Entities/EntityEvent.h>
#include <Engine/Entities/EntityPointer.h>
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include <Engine/Effect/CTag.h>
#include <Engine/Effect/CEntityTag.h>
#include <Engine/Effect/CTagManager.h>
#include <Engine/Effect/CRefCountPtr.h>
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

#define WLD_AUTO_ENTITY_ID  -1


#define DUMPVECTOR(v) \
	strm.FPrintF_t(#v ":  %g,%g,%g %08x,%08x,%08x\n", \
(v)(1), (v)(2), (v)(3), (ULONG&)(v)(1), (ULONG&)(v)(2), (ULONG&)(v)(3))
#define DUMPVECTOR2(strDes, v) \
strm.FPrintF_t( "%s:  %g,%g,%g\n", strDes, (v)(1), (v)(2), (v)(3))
#define DUMPLONG(l) \
strm.FPrintF_t(#l ":  %08x\n", l)
#define DUMPPLACEMENT(plname, pl)        \
	strm.FPrintF_t("%s:\n", plname);       \
	DUMPVECTOR2( "Position", pl.pl_PositionVector);    \
DUMPVECTOR2( "Orientation", pl.pl_OrientationAngle);

// force infulence at a point in space
class CForceStrength {
public:
	FLOAT3D fs_vDirection;    // direction of the force (must be normalized)
	FLOAT fs_fAcceleration;   // acceleration of the force (m/s2) (along the direction)
	FLOAT fs_fVelocity;       // max. velocity that force can give (m/s) (along the direction)
};

typedef ULONG64		ZONEFLAGS;

#define DECL_DLL ENGINE_API
#include <Engine/Classes/BaseEvents.h>
#undef DECL_DLL

/*
* Flags determining whether some entity is active in some game type or difficulty level.
*/
// difficulty levels
#define SPF_EASY        (1L<<0)       // active at easy difficulty
#define SPF_NORMAL      (1L<<1)       // active at normal difficulty
#define SPF_HARD        (1L<<2)       // active at hard difficulty
#define SPF_EXTREME     (1L<<3)       // active at extreme difficulty
//#define SPF_TOURIST     (1L<<4)       // active at tourist difficulty

#define SPF_MASK_DIFFICULTY 0x0000FFFFL    // mask for difficulty level flags

// game types
#define SPF_SINGLEPLAYER  (1L<<16)    // active in single player mode
#define SPF_DEATHMATCH    (1L<<17)    // active in deathmatch mode
#define SPF_COOPERATIVE   (1L<<18)    // active in cooperative mode
#define SPF_FLYOVER       (1L<<19)    // active in flyover (camera) mode

#define SPF_MASK_GAMEMODE 0xFFFF0000L    // mask for game type flags

/*
* Various other entity flags
*/
#define ENF_SELECTED          (1L<< 0)  // set if selected
#define ENF_ZONING            (1L<< 1)  // brush that defines spatial classification
#define ENF_DELETED           (1L<< 2)  // set if the entity does not exist anymore
#define ENF_ALIVE             (1L<< 3)  // set if the entity is currently a living being
#define ENF_INRENDERING       (1L<< 4)  // set if the entity is currently active in rendering
#define ENF_VALIDSHADINGINFO  (1L<< 5)  // set if shading info is valid
#define ENF_SEETHROUGH        (1L<< 6)  // set if cast ray can see through
#define ENF_FOUNDINGRIDSEARCH (1L<< 7)  // set if the entity is already found in grid search
#define ENF_CLUSTERSHADOWS    (1L<< 8)  // model that has cluster shadows
#define ENF_BACKGROUND        (1L<< 9)  // brush or model that is used for background rendering
#define ENF_ANCHORED          (1L<<10)  // set if cannot be moved in editor without special allowance
#define ENF_CUSTOMSHADING     (1L<<11)  // entity has fully customized shading
#define ENF_INVISIBLE         (1L<<12)  // entity is invisible (for AI purposes) or brush is invisible (for trivial rejection)
#define ENF_DYNAMICSHADOWS    (1L<<13)  // moving brush that causes automatic shadow recalculation
#define ENF_NOTIFYLEVELCHANGE (1L<<14)  // entity is notified when level is changed
#define ENF_CROSSESLEVELS     (1L<<15)  // entity must be carried when level is changed
#define ENF_NOPARTICLES       (1L<<16)  // this entity doesn't have particles attached

#define ENF_PROJECTIONSHADOWS (1L<<17)  // model that has cluster shadows
#define ENF_ZONEMOVING		  (1L<<19)
// prediction was here!
//0607 kwon 추가.
#define ENF_ITEM            (1L<<20)  
#define ENF_HIDDEN            (1L<<21)  // set if the entity is hidden (for editing)
#define ENF_NOSHADINGINFO     (1L<<22)  // the entity doesn't need FindShadingInfo(), it will set its own shading
#define ENF_PROPSCHANGED      (1L<<23)  // entitiy's properties have changed during gameplay, it will be sent in gamestate
#define ENF_CLIENTHANDLING    (1l<<24)  // entity should be handled in HandleTimers() on the client side
#define ENF_PLACCHANGED       (1L<<25)  // entitiy's placement has changed during gameplay, it will be sent in gamestate
#define ENF_MARKDESTROY       (1L<<26)  // when this entity is destroyed, remember it so we can transport that over network
#define ENF_NONETCONNECT      (1L<<27)  // marks that this no update is required for this entity on game join (unless any of the two previous flags are set)
#define ENF_NICEWATER		  (1L<<28)
// FIXME : ENF_CHARACTER 플래그를 체크하는 방식이 아닌, IsCharacter() 검사로 해결 가능함.
// FIXME : IsOfClass나 기타 다른 함수로도 커버 가능함.
//#define ENF_CHARACTER		  (1L<<29)
#define	ENF_SHOWHIDDEN		  (1L<<30)	// show hidden npc (only show npc)
#define ENF_RENDERREFLECTION  (1L<<31)
//------------------------------------------------------------
// 주의!!! 더이상 플래그를 추가하지 말것.
// 4bytes를 모두 다 사용했음.
//------------------------------------------------------------
// First Extra Flags
#define ENF_EX1_TERRAINSHADOW	(1L<<0)
#define ENF_EX1_DECORATION		(1L<<1)
#define ENF_EX1_PEACEFUL		(1L<<4)
#define ENF_EX1_NPC				(1L<<5)
#define ENF_EX1_PRODUCTION		(1L<<6)
#define ENF_EX1_CURRENT_PET		(1L<<7)		// 내 애완동물인 경우...
#define ENF_EX1_CURRENT_SLAVE	(1L<<8)		// 내 소환수의 경우...
#define ENF_EX1_CURRENT_WILDPET	(1L<<9)		// 공혁형 팻인경
#define ENF_EX1_CLICK_OBJECT	(1L<<10)	// 클릭가능한 오브젝트인 경우(레이드 시스템용)
#define ENF_EX1_MONSTER_MERCENARY	(1L<<11)	// [2010/10/20 : Sora] 몬스터 용병 카드
#define ENF_EX1_TOTEM			(1L<<12)	// Totem
#define ENF_EX1_TRAP			(1L<<13)	// Trap
#define ENF_EX1_SUICIDE			(1L<<14)	// 자폭 기생충
#define ENF_EX1_TOTEM_ITEM		(1L<<15)	// Totem
// selections of entities
typedef CSelection<CEntity, ENF_SELECTED> CEntitySelection;

enum EntityComponentType {  // DO NOT RENUMBER!
	ECT_TEXTURE   = 1,    // texture data
	ECT_MODEL     = 2,    // model data
	ECT_CLASS     = 3,    // entity class
	ECT_SOUND     = 4,    // sound data
	ECT_SKAMODEL  = 5,    // ska model
};

/*
*  General structure of an entity instance.
*/
class ENGINE_API CEntity {
public:
	// EDIT : BS : BEGIN
	DWORD m_tickSendPDMove;
	// EDIT : BS : END

	// type of function pointer used as AI event handler
	typedef BOOL (CEntity::*pEventHandler)(const CEntityEvent &ee);
	
	enum RenderType {
		RT_ILLEGAL     = 1,
		RT_NONE        = 2,     // not rendered ever -- used internally
		RT_MODEL       = 3,     // drawn as model
		RT_BRUSH       = 4,     // rendered as brush
		RT_EDITORMODEL = 5,     // rendered as model, but only in editor
		RT_VOID        = 7,     // not rendered ever
		RT_FIELDBRUSH  = 8,     // brush used for field effects (like triggers, force fields etc.)
		RT_SKAMODEL    = 9,     // render as ska model
		RT_SKAEDITORMODEL = 10, // render as ska model, but only in editor
		RT_TERRAIN        = 11, // render as terrain
	};

	enum NickNameEffectType {
 		NICKNAME_ATTACK_EFFECT = 0,
 		NICKNAME_DAMAGE_EFFECT = 1,
		NICKNAME_ATTACH_EFFECT = 2,
	 };

	enum EntityUseType {
		EU_NORMAL	= 0,
		EU_DUMMY	= 1,		// Only using modelinstance
	};

	/* Entity physics flags. */
#define EPF_ORIENTEDBYGRAVITY     (1UL<<0) // set if gravity influences its orientation
#define EPF_TRANSLATEDBYGRAVITY   (1UL<<1) // set if gravity can move it
#define EPF_PUSHABLE              (1UL<<2) // set if can be pushed by other objects
#define EPF_STICKYFEET            (1UL<<3) // entity always falls to nearest surface
#define EPF_RT_SYNCHRONIZED       (1UL<<4) // set if rotation and translation are synchronized
#define EPF_ABSOLUTETRANSLATE     (1UL<<5) // set if entity is translated absolute and not relative to its position
#define EPF_NOACCELERATION        (1UL<<6) // set if entity can change its speed immediately
#define EPF_HASLUNGS              (1UL<<7) // set if entity has lungs
#define EPF_HASGILLS              (1UL<<8) // set if entity has gills
#define EPF_MOVABLE               (1UL<<9) // set if derived from CMovableEntity
#define EPF_NOIMPACT              (1UL<<10)// entities are not damaged when hitting this one
#define EPF_NOIMPACTTHISTICK      (1UL<<11)// this one is not damaged by impact this tick
#define EPF_CANFADESPINNING       (1UL<<12)// desired rotation can be reduced by contents (like water)
#define EPF_ONSTEEPSLOPE          (1UL<<13)// while sliding down a steep slope (valid only if entity has reference)
#define EPF_ORIENTINGTOGRAVITY    (1UL<<14)// while beeing re-oriented by gravity
#define EPF_FLOATING              (1UL<<15)// while bouyancy causes floating in fluid
#define EPF_FORCEADDED            (1UL<<16)// set if force-added to movers
	
	// what to do when colliding
#define EPF_ONBLOCK_MASK            (7UL<<29)
#define EPF_ONBLOCK_STOP            (0UL<<29)  // stop moving
#define EPF_ONBLOCK_SLIDE           (1UL<<29)  // slide along
#define EPF_ONBLOCK_CLIMBORSLIDE    (2UL<<29)  // clim up a stair or slide along
#define EPF_ONBLOCK_BOUNCE          (3UL<<29)  // bounce off
#define EPF_ONBLOCK_PUSH            (4UL<<29)  // push the obstacle
#define EPF_ONBLOCK_STOPEXACT       (5UL<<29)  // stop moving, but exactly at collision position
	
	// entity collision flags are divided in 3 groups
#define ECB_COUNT 10    // max number of flags per group
#define ECF_MASK  ((1<<ECB_COUNT)-1)  // mask for one group
	// what an entity is
#define ECB_IS      0
#define ECF_ISMASK  (ECF_MASK<<ECB_IS)
	// which other entities to test with
#define ECB_TEST    10
#define ECF_TESTMASK  (ECF_MASK<<ECB_TEST)
	// which tested entities to pass through
#define ECB_PASS    20
#define ECF_PASSMASK  (ECF_MASK<<ECB_PASS)
	
	// optimizations to completely ignore some types of entities
#define ECF_IGNOREBRUSHES  (1UL<<30)
#define ECF_IGNOREMODELS   (1UL<<31)
	
	// for ska: run animations on entitie's master model instance
#define ESKA_MASTER_MODEL_INSTANCE  -1
	// max quantity of data sent over network for any ska animation event 
#define ESKA_MAX_DATA_SIZE (3*sizeof(INDEX) + sizeof(ULONG) + 2*sizeof(FLOAT))
	// max quantity of data sent over network for any sound animation event 
#define ESOUND_MAX_DATA_SIZE 32
	
public:
	enum EntityUseType en_EntityUseType;	// how is this used type
	enum RenderType en_RenderType;  // how is it rendered
	ULONG en_ulPhysicsFlags;        // ways of interacting with environment
	ULONG en_ulCollisionFlags;      // which entities to collide with
	
	ULONG en_ulFlags;               // various flags
	ULONG en_ulExtraFlags1;			// Extra Flags 1
	ULONG en_ulExtraFlags2;			// Extra Flags 2
	ULONG en_ulSpawnFlags;          // in what game types is this entity active
	INDEX en_ctReferences;          // reference counter for delayed destruction
	ULONG en_ulID;                  // unique entity identifier
	LONG  en_lNetworkID;
	SBYTE en_sbNetworkType;
	FLOAT en_FallingValue;
	
	TIME en_tmLastPlacementTime;      // when did the client receive the last setplacement from the server
	CPlacement3D en_plLastPlacementReceived; // last posiion and orientation received from server
	
	CPlacement3D en_plPlacement;      // placement in world space
	FLOATmatrix3D en_mRotation;       // precalc. matrix for object rotation
	CEntityClass *en_pecClass;        // class used to construct this entity
	union {   // rendering object
		CBrush3D *en_pbrBrush;                // brush -- if brush entity
		CModelObject *en_pmoModelObject;      // model -- if model entity
		CModelInstance *en_pmiModelInstance;  // ska model -- if ska model entity
		CTerrain *en_ptrTerrain;              // terrain -- if terrain entity
	};
	CShadingInfo *en_psiShadingInfo;            // shading info for model entities
	CCollisionInfo *en_pciCollisionInfo;        // collision info for movable colliding entities
	FLOAT en_fSpatialClassificationRadius;      // radius for spatial classification
	FLOATaabbox3D en_boxSpatialClassification;  // box in object space for spatial classification
	CLastPositions *en_plpLastPositions;        // last positions of entity
	
	class CWorld *en_pwoWorld;      // the world this entity belongs to
	union 
	{
		class CMobTarget		*en_pMobTarget;
		class CCharacterTarget	*en_pCharacterTarget;
		class CPetTarget		*en_pPetTarget;
		class CSlaveTarget		*en_pSlaveTarget;
		class CWildPetTarget	*en_pWildPetTarget;
	};
	
	CTString en_strItemName;
	FLOAT3D	en_vDesiredDirection;
	
public: // imagine that this is private
	CRelationDst en_rdSectors;      // relation to sectors this entity is in
public:
	
	CEntity *en_penParent;        // parent entity
public: // these must be public for iteration
	CListNode en_lnInParent;      // node in the parent entity
	CListHead en_lhChildren;      // list of child entities
public:
	CPlacement3D en_plRelativeToParent;   // placement relative to parent placement
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	//CTagManager		*m_ptmTagManager;
	CLightSource	*m_plsLight;
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
	
public:
	// reference counting functions
	inline void AddReference(void);
	inline void RemReference(void);
	void DeleteSelf(void);
	
	
	/* Get pointer to entity property from its packed identifier. */
	class CEntityProperty *PropertyForTypeAndID(ULONG ulType, ULONG ulID);
	/* Helpers for writing/reading entity pointers. */
	void ReadEntityPointer_t(CTStream *istr, CEntityPointer &pen);
	void WriteEntityPointer_t(CTStream *ostr, CEntityPointer pen);
	/* Get pointer to entity component from its packed identifier. */
	class CEntityComponent *ComponentForTypeAndID(ULONG ulType, ULONG ulID);
	/* Get pointer to entity property from its name. */
	class CEntityProperty *PropertyForName(const CTString &strPropertyName);
	/* Copy one entity property from property of another entity. */
	void CopyOneProperty( CEntityProperty &epPropertySrc, CEntityProperty &epPropertyDest,
		CEntity &enOther, ULONG ulFlags);
	
	/* Read all properties from a stream. */
	void ReadProperties_t(CTStream &istrm,BOOL bNetwork);  // throw char *
	/* Write all properties to a stream. */
	void WriteProperties_t(CTStream &ostrm,BOOL bNetwork = FALSE); // throw char *
	/* Copy entity properties from another entity of same class. */
	void CopyEntityProperties(CEntity &enOther, ULONG ulFlags);
	
	/* Internal versions for entity reinitialization (do not discard shadows etc.). */
	void Initialize_internal(const CEntityEvent &eeInput);
	void End_internal(void);
	/* Reinitialize the entity. */
	void Reinitialize(void);
	
		// internal repositioning function
#define SPIF_NEAR                   (1UL<<0)    // optimize for small movements (used in physics)
#define SPIF_SKIPMOVEABLECHILDREN   (1UL<<0)    // don't pull movable children along (used in physics)
	virtual void SetPlacement_special(const CPlacement3D &plNew, const FLOATmatrix3D &mRotation,
		ULONG ulFlags);
	// Uncache shadows of each polygon in entity that has given gradient index
	void UncacheShadowsForGradient(INDEX iGradient);
	
	/* Find and remember shading info for this entity if invalid. */
	void FindShadingInfo(void);
	/* Find and remember collision info for this entity. */
	void FindCollisionInfo(void);
	// discard collision info for this entity
	void DiscardCollisionInfo(void);
	// copy collision info from some other entity
	void CopyCollisionInfo(CEntity &enOrg);
	/* Update range used for spatial clasification. */
	void UpdateSpatialRange(void);
	/* Find and remember all sectors that this entity is in. */
	void FindSectorsAroundEntity(void);
	void FindSectorsAroundEntityNear(void);
	
	// add entity to collision grid
	void AddToCollisionGrid(void);
		
	/* Copy entity from another entity of same class. (NOTE: this doesn't copy placement!) */
#define COPY_REMAP      (1UL<<0)  // remap pointers
#define COPY_REINIT     (1UL<<1)  // reinit entity
	virtual void Copy(CEntity &enOther, ULONG ulFlags);
	virtual CEntity &operator=(CEntity &enOther) {ASSERT(FALSE); return *this;};
	// find a pointer to another entity while copying
	static CEntity *FindRemappedEntityPointer(CEntity *penOriginal);
	/* Read from stream. */
	virtual void Read_t( CTStream *istr, BOOL bNetwork);  // throw char *
	/* Write to stream. */
	virtual void Write_t( CTStream *ostr, BOOL bNetwork); // throw char *
	/* Precache components that might be needed. */
	virtual void Precache(void);
	
	/* Handle all sent events. */
	static void HandleSentEvents(void);

	ANGLE GetRelativeHeading(const FLOAT3D &vDirection);
	BOOL CheckModelRangeDamage(CEntity &en, const FLOAT3D &vCenter, FLOAT &fMinD, FLOAT3D &vHitPos);		
	
	// find entities in a box (box must be around this entity)
	void FindEntitiesInRange(const FLOATaabbox3D &boxRange, CDynamicContainer<CEntity> &cen,
		BOOL bCollidingOnly);
	// find first entity touching a field (this entity must be a field brush)
	CEntity *TouchingEntity(BOOL (*ConsiderEntity)(CEntity *), CEntity *penHintMaybeInside);
	
public:
	// DLL class interface
	/* Initialize for being virtual entity that is not rendered. */
	void InitAsVoid(void);
	/* Initialize for beeing a model object. */
	void InitAsModel(void);
	void InitAsSkaModel(void);
	/* Initialize for beeing a terrain object. */
	void InitAsTerrain(void);
	
	/* Initialize for beeing an editor model object. */
	void InitAsEditorModel(void);
	void InitAsSkaEditorModel(void);
	/* Initialize for beeing a brush object. */
	void InitAsBrush(void);
	/* Initialize for beeing a field brush object. */
	void InitAsFieldBrush(void);
	/* Switch to Model/Editor model */
	void SwitchToModel(void);
	void SwitchToEditorModel(void);
	
	/* Set all properties to default values. - overridden by ecc */
	virtual void SetDefaultProperties(void);
	
	/* Get a filename for a component of given type and id. */
	const CTFileName &FileNameForComponent(SLONG slType, SLONG slID);
	// Get data for a texture component
	CTextureData *GetTextureDataForComponent(SLONG slID);
	// Get data for a model component
	CModelData *GetModelDataForComponent(SLONG slID);
	// Get data for a model instance component
	CModelInstance *GetModelInstanceForComponent(SLONG slID);
	
	// model manipulation functions -- only for RT_MODEL/RT_EDITORMODEL
	/* Set the model data for model entity. */
	void SetModel(const CTFileName &fnmModel);
	void SetModel(SLONG idModelComponent);
	/* Get the model data for model entity. */
	const CTFileName &GetModel(void);
	/* Start new animation for model entity. */
	void StartModelAnim(INDEX iNewModelAnim, ULONG ulFlags,BOOL bNetwork = TRUE);
	
	// Ska model manipulation functions -- only for RT_SKAMODEL/RT_SKAEDITORMODEL
	BOOL SetSkaModel(const CTString &fnmModel);
	void SetSkaModel_t(const CTString &fnmModel);
	void SetSkaModel(SLONG idSkaModelComponent);
	void SetSkaColisionInfo();

	//0601 kwon 함수 추가.
	CModelInstance* SetSkaModelItem(const CTString &fnmModel);
	CModelInstance* SetSkaModelItem_t(const CTString &fnmModel);
	
	/* Add a new animation to Ska amination stack */
	void AddAnimation(INDEX iAnimID, ULONG ulFlags, FLOAT fStrength, INDEX iGroupID, INDEX iModelInstanceID = ESKA_MASTER_MODEL_INSTANCE, FLOAT fSpeedMul = 1.0f);
	/* remove an animation from stack */
	void RemAnimation(INDEX iAnimID, INDEX iModelInstanceID = ESKA_MASTER_MODEL_INSTANCE);
	/* Remove all anims with GroupID */
	void RemAnimsWithID(INDEX iGroupID, INDEX iModelInstanceID = ESKA_MASTER_MODEL_INSTANCE);
	/* create new state, copy last state in it and give it a fade time */
	void NewClonedState(FLOAT fFadeTime, INDEX iModelInstanceID = ESKA_MASTER_MODEL_INSTANCE);
	/* create new cleared state and give it a fade time */
	void NewClearState(FLOAT fFadeTime, INDEX iModelInstanceID = ESKA_MASTER_MODEL_INSTANCE);
	/* Check if given animation is currently playing */
	BOOL IsAnimationPlaying(INDEX iAnimID, INDEX iModelInstanceID = ESKA_MASTER_MODEL_INSTANCE);
	/* Stretch model instance */
	void StretchModel(FLOAT3D &vStretch, INDEX iModelInstanceID = ESKA_MASTER_MODEL_INSTANCE);
	/* Stretch model instance without attachments */
	void StretchSingleModel(FLOAT3D &vStretch, INDEX iModelInstanceID = ESKA_MASTER_MODEL_INSTANCE);
	/* Offset all animations in anim queue */
	void OffSetAnimationQueue(TIME fOffsetTime, INDEX iModelInstanceID  = ESKA_MASTER_MODEL_INSTANCE);
	
	void ReadSKAQueue(CTStream *istr);
	void ReadSKAQueue(CNetworkMessage& nmMessage);
	void WriteSKAQueue(CTStream *ostr);
	void WriteSKAQueue(CNetworkMessage& nmMessage);
	
	/* Play a given sound object. */
	void PlaySound(CSoundObject &so, SLONG idSoundComponent, SLONG slPlayType, BOOL bNetwork = TRUE);
	void PlaySound(CSoundObject &so, const CTFileName &fnmSound, SLONG slPlayType);
	double GetSoundLength(SLONG idSoundComponent);
	
	// set/get model main blend color
	COLOR GetModelColor(void) const;
	void  SetModelColor( const COLOR colBlend);
	
	/* Set the main texture data for model entity. */
	void SetModelMainTexture(SLONG idTextureComponent);
	void SetModelMainTexture(const CTFileName &fnmTexture);
	/* Get the main texture data for model entity. */
	const CTFileName &GetModelMainTexture(void);
	/* Start new animation for main texture of model entity. */
	void StartModelMainTextureAnim(INDEX iNewTextureAnim);
	
	/* Set the reflection texture data for model entity. */
	void SetModelReflectionTexture(SLONG idTextureComponent);
	/* Set the specular texture data for model entity. */
	void SetModelSpecularTexture(SLONG idTextureComponent);
	
	/* Add attachment to model */
	void AddAttachment(INDEX iAttachment, ULONG ulIDModel, ULONG ulIDTexture);
	void AddAttachment(INDEX iAttachment, CTFileName fnModel, CTFileName fnTexture);
	/* Remove attachment from model */
	void RemoveAttachment(INDEX iAttachment);
	/* Set the reflection texture data for model attachment entity. */
	void SetModelAttachmentReflectionTexture(INDEX iAttachment, SLONG idTextureComponent);
	/* Set the specular texture data for model attachment entity. */
	void SetModelAttachmentSpecularTexture(INDEX iAttachment, SLONG idTextureComponent);
	
	// Get all vertices of model entity in absolute space
	void GetModelVerticesAbsolute( CStaticStackArray<FLOAT3D> &avVertices, FLOAT fNormalOffset, FLOAT fMipFactor);
	// Returns true if bone exists and sets two given vectors as start and end point of specified bone in abs space
	BOOL GetBoneAbsPosition(INDEX iBoneID, FLOAT3D &vStartPoint, FLOAT3D &vEndPoint);
	// Returns true if bone exists and sets two given vectors as start and end point of specified bone in relative space
	BOOL GetBoneRelPosition(INDEX iBoneID, FLOAT3D &vStartPoint, FLOAT3D &vEndPoint);
	// Returns true if bone exists and sets the bones rotation and translation
	BOOL GetBoneTransformations(INDEX iBoneID, FLOATmatrix3D &mRot, FLOAT3D &vTranslation);
	// Callback function for aditional bone adjustment
	virtual void AdjustBones();
	// Callback function for aditional shader params adjustment
	virtual void AdjustShaderParams(INDEX iSurfaceID,CShader *pShader,ShaderParams &spParams);
	
	// precache given component
	void PrecacheModel(SLONG slID);
	void PrecacheTexture(SLONG slID);
	void PrecacheSound(SLONG slID);
	void PrecacheClass(SLONG slID, INDEX iUser = -1);
	void PrecacheSkaModel(SLONG slID);

	void ReleaseChche(EntityComponentType eType, SLONG slID);
	
	/* Create a new entity of given class in this world. */
	CEntity *CreateEntity(const CPlacement3D &plPlacement, SLONG idModelComponent,ULONG ulEntityID = WLD_AUTO_ENTITY_ID,BOOL bNetwork = TRUE);
	
	/* Apply some damage directly to one entity. */
	virtual void InflictDirectDamage(CEntity *penToDamage, CEntity *penInflictor, enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection);
	/* Apply some damage to all entities in some range (this tests for obstacles). */
	virtual void InflictRangeDamage(CEntity *penInflictor, enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOAT3D &vCenter, FLOAT fHotSpotRange, FLOAT fFallOffRange);
	/* Apply some damage to all entities in a box (this doesn't test for obstacles). */
	virtual void InflictBoxDamage(CEntity *penInflictor, enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOATaabbox3D &box);
	
	// notify engine that gravity defined by this entity has changed
	void NotifyGravityChanged(void);
	// notify engine that collision of this entity was changed
	void NotifyCollisionChanged(void);
	
	// get a pseudo-random number (safe for network gaming)
	ULONG IRnd(void);   // [0x0000 , 0xFFFF]
	FLOAT FRnd(void);   // [0.0f , 1.0f]
	
	// DLL class overridables
	/* Called after creating and setting its properties. */
	virtual void OnInitialize(const CEntityEvent &eeInput);
	/* Called before releasing entity. */
	virtual void OnEnd(void);
	
	// these functions are dummy in CEntity, but are implemented in CRationalEntity
	/* Jump to a new state. */
	void Jump(SLONG slThisState, SLONG slTargetState, BOOL bOverride, const CEntityEvent &eeInput) {};
	/* Call a subautomaton. */
	void Call(SLONG slThisState, SLONG slTargetState, BOOL bOverride, const CEntityEvent &eeInput) {};
	/* Return from a subautomaton. */
	void Return(SLONG slThisState, const CEntityEvent &eeReturn) {};
	// print stack to debug output
	virtual const char *PrintStackDebug(void);
	
	void SetTimerAt(TIME timeAbsolute) {};
	void SetTimerAfter(TIME timeDelta) {};
	void UnsetTimer(void) {};
	
	// return opacity of the entity (1 is default)
	virtual FLOAT GetOpacity(void) { return 1.0f; };
	
	// returns ammount of memory used by entity
	virtual SLONG GetUsedMemory(void);

public:
	
	// construction/destruction
	/* Default constructor. */
	CEntity(void);
	/* Destructor. */
	virtual ~CEntity(void);
	/* Clear the object. */
	void Clear(void) {};
	
	// entities can be selected
	IMPLEMENT_SELECTING(en_ulFlags);
	
	// access functions
	/* Prepare entity (call after setting properties). */
	void Initialize(const CEntityEvent &eeInput = _eeVoid,BOOL bNetwork = TRUE);
	/* Initializes the entity with it's default event (call after setting properties). */
	virtual void InitializeFromNet();
	/* Clean-up entity. */
	void End(void);
	
	/* Destroy this entity (entity must not be targetable). Destroy message is sent through network by default */
	void Destroy(BOOL bNetwork = TRUE);
	
	/* Get state transition for given state and event code. */
	virtual CEntity::pEventHandler HandlerForStateAndEvent(SLONG slState, SLONG slEvent);
	/* Handle an event, return false if the event is not handled. */
	virtual BOOL HandleEvent(const CEntityEvent &ee);
	
	// get/set functions for use in WEd and/or entity class DLLs
	void SetPlacement(const CPlacement3D &plNew);     
	void FallDownToFloor(void);
	inline const CPlacement3D &GetPlacement(void) const { return en_plPlacement; };
	inline const FLOATmatrix3D &GetRotationMatrix(void) const { return en_mRotation; };
	// this one is used in rendering - gets lerped placement between ticks
	virtual CPlacement3D GetLerpedPlacement(void) const;
	/* Find first sector that entity is in */
	CBrushSector *GetFirstSector(void);
	/* Find first sector that entity is in (for UI purpuses) */
	CBrushSector *GetFirstSectorWithName(void);
	
	// teleport this entity to a new location -- takes care of telefrag damage
	void Teleport(const CPlacement3D &plNew, BOOL bTelefrag=TRUE);
	
	inline void SetFlagOn(ULONG ulFlag)  { en_ulFlags |= ulFlag; };
	inline void SetFlagOff(ULONG ulFlag) { en_ulFlags &= (~ulFlag); };
	inline ULONG IsFlagOn(ULONG ulFlag)  { return en_ulFlags & ulFlag; };
	inline ULONG IsFlagOff(ULONG ulFlag) { return (~en_ulFlags) & ulFlag; };  
	inline void SetFirstExtraFlagOn(ULONG ulFlag)  { en_ulExtraFlags1 |= ulFlag; };
	inline void SetFirstExtraFlagOff(ULONG ulFlag) { en_ulExtraFlags1 &= (~ulFlag); };
	inline ULONG IsFirstExtraFlagOn(ULONG ulFlag)  { return en_ulExtraFlags1 & ulFlag; };
	inline ULONG IsFirstExtraFlagOff(ULONG ulFlag) { return (~en_ulExtraFlags1) & ulFlag; };  

	inline void SetSecondExtraFlagOn(ULONG ulFlag)  { en_ulExtraFlags2 |= ulFlag; };
	inline void SetSecondExtraFlagOff(ULONG ulFlag) { en_ulExtraFlags2 &= (~ulFlag); };
	inline ULONG IsSecondExtraFlagOn(ULONG ulFlag)  { return en_ulExtraFlags2 & ulFlag; };
	inline ULONG IsSecondExtraFlagOff(ULONG ulFlag) { return (~en_ulExtraFlags2) & ulFlag; };  
	inline void	SetNetworkID(LONG lIndex)			{ en_lNetworkID = lIndex; }
	inline LONG GetNetworkID()						{ return en_lNetworkID; };	
	void SetFlags(ULONG ulFlags);
	inline ULONG GetFlags(void) const { return en_ulFlags; };
	inline ULONG GetFirstExFlags(void) const { return en_ulExtraFlags1; };
	inline ULONG GetSecondExFlags(void) const { return en_ulExtraFlags2; };
	inline void SetSpawnFlags(ULONG ulFlags) { en_ulSpawnFlags = ulFlags; }
	inline ULONG GetSpawnFlags(void) const { return en_ulSpawnFlags; };
	void SetPhysicsFlags(ULONG ulFlags);
	inline ULONG GetPhysicsFlags(void) const { return en_ulPhysicsFlags; };
	void SetCollisionFlags(ULONG ulFlags);
	inline ULONG GetCollisionFlags(void) const { return en_ulCollisionFlags; };
	
	inline enum RenderType GetRenderType(void) { return en_RenderType; };
	inline CEntityClass *GetClass(void) { return en_pecClass; };
	inline CWorld *GetWorld(void) { return en_pwoWorld; };
	inline CBrush3D *GetBrush(void) { return en_pbrBrush; };
	inline CModelObject *GetModelObject(void) { return en_pmoModelObject; };
	inline CModelInstance *GetModelInstance(void) { return en_pmiModelInstance; };
	inline CTerrain *GetTerrain(void) { return en_ptrTerrain; };
	inline CEntity *GetParent(void) { return en_penParent; };
	inline CMobTarget *GetMobTarget(void) { return en_pMobTarget; }
	inline CCharacterTarget *GetCharacterTarget(void) { return en_pCharacterTarget; }
	inline CPetTarget *GetPetTarget(void) { return en_pPetTarget; }
	inline CSlaveTarget *GetSlaveTarget(void) { return en_pSlaveTarget; }
	inline CWildPetTarget *GetWildPetTargetInfo(void) { return en_pWildPetTarget; }
	void SetParent(CEntity *penNewParent);
	
	// find first child of given class
	CEntity *GetChildOfClass(const char *strClass);
	
	/* Test if the entity is an empty brush. */
	BOOL IsEmptyBrush(void) const;
	/* Is this a player entity? (overriden in PlayerEntity.es) */
	virtual BOOL IsPlayer()				{return FALSE;};
	virtual BOOL IsCharacter() const	{return FALSE;};
	virtual BOOL	IsEnemy(void) const;
	virtual BOOL	IsEnemySpawner(void) const;
	virtual BOOL	IsPet(void) const;
	virtual BOOL	IsSlave(void) const;
	virtual BOOL	IsWildPet(void) const;
	virtual SBYTE	GetNetworkType()	const;
	// is available pet riding.
	virtual BOOL	IsAvailableRide(void);

	/* Return max Game Players */
	static INDEX GetMaxPlayers(void);
	/* Return Player Entity */
	static CEntity *GetPlayerEntity(INDEX iPlayer);
	
	/* Get bounding box of this entity - for AI purposes only. */
	void GetBoundingBox(FLOATaabbox3D &box);
	/* Get size of this entity - for UI purposes only. */
	void GetSize(FLOATaabbox3D &box);
	/* Get last positions structure for particles. */
	CLastPositions *GetLastPositions(INDEX ctPositions);
	/* Get nearest position of nearest brush polygon to this entity if available. */
	CBrushPolygon *GetNearestPolygon(FLOAT3D &vPoint, FLOATplane3D &plPlane, FLOAT &fDistanceToEdge);
	// BSP위에 그림자를 뿌려주기 위해서 필요한 함수.
	void GetNearPolygonsInSphere(FLOAT fRadius, CDynamicContainer<CBrushPolygon> &dcBrushPolygon);
	/* Get absolute position of point on entity given relative to its size. */
	void GetEntityPointRatio(const FLOAT3D &vRatio, FLOAT3D &vAbsPoint, BOOL bLerped=FALSE);
	/* Get absolute position of point on entity given in meters. */
	void GetEntityPointFixed(const FLOAT3D &vFixed, FLOAT3D &vAbsPoint);
	/* Get sector that given point is in - point must be inside this entity. */
	CBrushSector *GetSectorFromPoint(const FLOAT3D &vPointAbs);
	
	// map world polygon to/from indices
	CBrushPolygon *GetWorldPolygonPointer(INDEX ibpo);
	INDEX GetWorldPolygonIndex(CBrushPolygon *pbpo);
	
	// virtual functions that are overridden to implement class specific behavior
	/* Get name of this entity. */
	virtual const CTString &GetName(void) const;
	virtual const CTString &GetDescription(void) const; // name + some more verbose data
	/* Get first target of this entity. */
	virtual CEntity *GetTarget(void) const;
	/* Check if entity can be used as a target. */
	virtual BOOL IsTargetable(void) const;
	/* Check if entity is server only (not updated through network) */
	virtual BOOL IsSentOverNet(void) const;
	/* Check if entity is marker */
	virtual BOOL IsMarker(void) const;

	/* Check if entity is important */
	virtual BOOL IsImportant(void) const;
	/* Check if entity is moved on a route set up by its targets. */
	virtual BOOL MovesByTargetedRoute(CTString &strTargetProperty) const;
	/* Check if entity can drop marker for making linked route. */
	virtual BOOL DropsMarker(CTFileName &fnmMarkerClass, CTString &strTargetProperty) const;
	/* Get light source information - return NULL if not a light source. */
//안태훈 수정 시작	//(For Performance)(0.2)
	CLightSource *GetLightSource(void)
	{
		return m_plsLight;
	}
//안태훈 수정 끝	//(For Performance)(0.2)
	/* Is target valid. */
	virtual BOOL IsTargetValid(SLONG slPropertyOffset, CEntity *penTarget);
	
	/* Get force type name, return empty string if not used. */
	virtual const CTString &GetForceName(INDEX iForce);
	/* Get forces in given point. */
	virtual void GetForce(INDEX iForce, const FLOAT3D &vPoint,
		CForceStrength &fsGravity, CForceStrength &fsField);
	/* Get entity that controls the force, used for change notification checking. */
	virtual CEntity *GetForceController(INDEX iForce);
	
	/* Get fog type name, return empty string if not used. */
	virtual const CTString &GetFogName(INDEX iFog);
	/* Get fog, return FALSE for none. */
	virtual BOOL GetFog(INDEX iFog, class CFogParameters &fpFog);
	
	/* Get haze type name, return empty string if not used. */
	virtual const CTString &GetHazeName(INDEX iHaze);
	/* Get haze, return FALSE for none. */
	virtual BOOL GetHaze(INDEX iHaze, class CHazeParameters &hpHaze, FLOAT3D &vViewDir);
	
	/* Get mirror type name, return empty string if not used. */
	virtual const CTString &GetMirrorName(INDEX iMirror);
	/* Get mirror, return FALSE for none. */
	virtual BOOL GetMirror(INDEX iMirror, class CMirrorParameters &mpMirror);
	
	/* Get gradient type name, return empty string if not used. */
	virtual const CTString &GetGradientName(INDEX iGradient);
	/* Get gradient, return FALSE for none. */
	virtual BOOL GetGradient(INDEX iGradient, class CGradientParameters &gpGradient);
	
	/* Get classification box stretching vector. */
	virtual FLOAT3D GetClassificationBoxStretch(void);
	
	/* Get anim data for given animation property - return NULL for none. */
	virtual CAnimData *GetAnimData(SLONG slPropertyOffset);
	/* Adjust model shading parameters if needed - return TRUE if needs model shadows. */
	virtual BOOL AdjustShadingParameters(FLOAT3D &vLightDirection,
		COLOR &colLight, COLOR &colAmbient);
	/* Adjust model mip factor if needed. */
	virtual void AdjustMipFactor(FLOAT &fMipFactor);
	// get a different model object for rendering - so entity can change its appearance dynamically
	// NOTE: base model is always used for other things (physics, etc).
	virtual CModelObject *GetModelForRendering(void);
	virtual CModelInstance *GetModelInstanceForRendering(void);
	/* Get field information - return NULL if not a field. */
	virtual CFieldSettings *GetFieldSettings(void);
	/* Render particles made by this entity. */
	virtual void RenderParticles(void);
	/* Get current collision box index for this entity. */
	virtual INDEX GetCollisionBoxIndex(void);
	/* Get current collision box - override for custom collision boxes. */
	virtual void GetCollisionBoxParameters(INDEX iBox, FLOATaabbox3D &box, INDEX &iEquality);
	/* Render game view */
	virtual void RenderGameView(CDrawPort *pdp, void *pvUserData);
	virtual void RenderCursor(CDrawPort *pdp, void *pvUserData);
	// apply mirror and stretch to the entity if supported
	virtual void MirrorAndStretch(FLOAT fStretch, BOOL bMirrorX);
	// get offset for depth-sorting of alpha models (in meters, positive is nearer)
	virtual FLOAT GetDepthSortOffset(void);
	// get visibility tweaking bits
	virtual ULONG GetVisTweaks(void);
	
	/* Get max tessellation level. */
	virtual FLOAT GetMaxTessellationLevel(void);
	
	/* Send an event to this entity. */
	void SendEvent(const CEntityEvent &ee,BOOL bNetwork = FALSE);
	/* Send an event to all entities in a box (box must be around this entity). */
	void SendEventInRange(const CEntityEvent &ee, const FLOATaabbox3D &boxRange);
	
	/* apply some damage to the entity (see event EDamage for more info) */
	virtual void ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection);
	
	/* Receive item through event - for AI purposes only */
	//virtual BOOL ReceiveItem(const CEntityEvent &ee);
	/* Get entity info - for AI purposes only */
	virtual void *GetEntityInfo(void);
	/* Fill in entity statistics - for AI purposes only */
	virtual BOOL FillEntityStatistics(struct EntityStats *pes);
	
	/* 레이드 오브젝트 선언*/
	virtual BOOL GetRaidObject(void) const;
	virtual INDEX GetRaidEvent(void) const;
	virtual BOOL GetAlBackground(void) const; // ska 모델 랜더링 정렬용
	
	/* Model change notify */
	void ModelChangeNotify(void);
	/* Terrain change notify */ 
	void TerrainChangeNotify(void);
	
	// FIXME : 왜 비슷한 코드가 이렇게 많이 있는거지? ㅡ.ㅡ
	bool SetTargetInfo(float fMaxHealth, float fHealth, BOOL bMe, int level, int Pk = -1,int PkState = 0,int PkLegit=0,int dbIdx=-1);
	void SetTargetSyndicateInfo(int nType, int nGrade);
	void UpdateTargetInfo( float fMaxHealth, float fHealth ,int Pk = -1, int PkState = 0,int PkLegit=0);
	void DelTargetInfo();
	// WSS_GUILDMASTER 070517 ------------------>><<
	// 수정 -> 길드관련 내용 ct를 이용해 처리 
	// player.es의 관련 함수 호출 부분( charater 호출 부분 수정함)	nType == DBIndex
	void SetTargetInfoReal( float fMaxHealth, float fHealth, int level, int Pk = -1, int PkState = 0, SQUAD llCount = 0,int PkLegit=0,CCharacterTarget* ct =NULL, int nType = -1);
	void AddSkaModel(const CTString &fnmModel);
	void DelSkaModel(const CTString &fnmModel);
	bool HasSkaModel(const CTString &fnmModel);
	virtual void SetNickNameDamageEffect(INDEX iNickIndex, NickNameEffectType iType);
};

// all standard smart pointer functions are here as inlines
inline CEntityPointer::CEntityPointer(void) : ep_pen(NULL) {};
inline CEntityPointer::~CEntityPointer(void) { ep_pen->RemReference(); };
inline CEntityPointer::CEntityPointer(const CEntityPointer &penOther) : ep_pen(penOther.ep_pen) {
	ep_pen->AddReference();
};
inline CEntityPointer::CEntityPointer(CEntity *pen) : ep_pen(pen) {
	ep_pen->AddReference();
};
inline const CEntityPointer &CEntityPointer::operator=(CEntity *pen) {
	pen->AddReference();    // must first add, then remove!
	ep_pen->RemReference();
	ep_pen = pen;
	return *this;
}
inline const CEntityPointer &CEntityPointer::operator=(const CEntityPointer &penOther) {
	penOther.ep_pen->AddReference();    // must first add, then remove!
	ep_pen->RemReference();
	ep_pen = penOther.ep_pen;
	return *this;
}
inline CEntity* CEntityPointer::operator->(void) const { return ep_pen; }
inline CEntityPointer::operator CEntity*(void) const { return ep_pen; }
inline CEntity& CEntityPointer::operator*(void) const { return *ep_pen; }


/////////////////////////////////////////////////////////////////////
// Reference counting functions
inline void CEntity::AddReference(void) { 
	if (this!=NULL) {
		ASSERT(en_ctReferences>=0);
		en_ctReferences++; 
	}
};
inline void CEntity::RemReference(void) { 
	if (this!=NULL) {
		ASSERT(en_ctReferences>0);
		en_ctReferences--;
		if(en_ctReferences==0) {
			DeleteSelf();
		}
	}
};

/*
* Entity that is alive (has health).
*/
class ENGINE_API CLiveEntity : public CEntity {
public:
	FLOAT en_fHealth;            // health of the entity
	
	/* Copy entity from another entity of same class. */
	virtual void Copy(CEntity &enOther, ULONG ulFlags);
	/* Read from stream. */
	virtual void Read_t( CTStream *istr, BOOL bNetwork);  // throw char *
	/* Write to stream. */
	virtual void Write_t( CTStream *ostr, BOOL bNetwork); // throw char *
public:
	/* Set health of the entity. (Use only for initialization!) */
	void SetHealth(FLOAT fHealth) { en_fHealth = fHealth; };
	
public:
	/* Constructor. */
	CLiveEntity(void);
	
	/* Get health of the entity. */
	FLOAT GetHealth(void) const { return en_fHealth; };
	// apply some damage to the entity (see event EDamage for more info)
	virtual void ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection);
	
	// returns bytes of memory used by this object
	inline SLONG GetUsedMemory(void) {
		return( sizeof(CLiveEntity) - sizeof(CEntity) + CEntity::GetUsedMemory());
	};
};

// flag for entities that are not waiting for thinking
#define THINKTIME_NEVER (-1.f)
		
/*
* Entity that can percept things and make decisions (one that has its own AI).
*/
class ENGINE_API CRationalEntity : public CLiveEntity {
public:
	CListNode en_lnInTimers;    // node in list of waiting timers - sorted by wait time
public:
	TIME en_timeTimer;          // moment in time this entity waits for timer
	
	CStaticStackArray<SLONG> en_stslStateStack; // stack of states for entity AI
	
	/* Calculate physics for moving. */
	virtual void ClearMovingTemp(void);
	virtual void PreMoving(void);
	virtual void DoMoving(void);
	virtual void PostMoving(void);
	
	/* Copy entity from another entity of same class. */
	virtual void Copy(CEntity &enOther, ULONG ulFlags);
	/* Read from stream. */
	virtual void Read_t( CTStream *istr, BOOL bNetwork);  // throw char *
	/* Write to stream. */
	virtual void Write_t( CTStream *ostr, BOOL bNetwork); // throw char *
	
	/* Unwind stack to a given state. */
	void UnwindStack(SLONG slThisState);
	
public:
	
	/* Jump to a new state. */
	void Jump(SLONG slThisState, SLONG slTargetState, BOOL bOverride, const CEntityEvent &eeInput);
	/* Call a subautomaton. */
	void Call(SLONG slThisState, SLONG slTargetState, BOOL bOverride, const CEntityEvent &eeInput);
	/* Return from a subautomaton. */
	void Return(SLONG slThisState, const CEntityEvent &eeReturn);
	// print stack to debug output
	const char *PrintStackDebug(void);
	
	/* Set next timer event to occur at given moment time. */
	void SetTimerAt(TIME timeAbsolute);
	/* Set next timer event to occur after given time has elapsed. */
	void SetTimerAfter(TIME timeDelta);
	/* Cancel eventual pending timer. */
	void UnsetTimer(void);
	
	/* Called after creating and setting its properties. */
	virtual void OnInitialize(const CEntityEvent &eeInput);
	/* Called before releasing entity. */
	virtual void OnEnd(void);
public:
	/* Constructor. */
	CRationalEntity(void);
	
	/* Handle an event - return false if event was not handled. */
	virtual BOOL HandleEvent(const CEntityEvent &ee);
	
	// returns bytes of memory used by this object
	inline SLONG GetUsedMemory(void) {
		SLONG slUsedMemory = sizeof(CRationalEntity) - sizeof(CLiveEntity) + CLiveEntity::GetUsedMemory();
		slUsedMemory += en_stslStateStack.sa_Count * sizeof(SLONG);
		return slUsedMemory;
	};
};


ENGINE_API void EntityAdjustBonesCallback(void *pData);
ENGINE_API void EntityAdjustShaderParamsCallback(void *pData,INDEX iSurfaceID,CShader *pShader,ShaderParams &spParams);

extern "C" ENGINE_API class CDLLEntityClass CEntity_DLLClass;
extern "C" ENGINE_API class CDLLEntityClass CLiveEntity_DLLClass;
extern "C" ENGINE_API class CDLLEntityClass CRationalEntity_DLLClass;


#endif  /* include-once check. */
		
