// common headers for flesh entity classes
#include <Engine/GameState.h>

// yjpark |<--
#define LC_SURFACE_MARBLE_IN			0
#define LC_SURFACE_BLOCK_OUT			1
#define LC_SURFACE_WOOD_OUT				2
#define LC_SURFACE_SAND_IN				3
#define LC_SURFACE_WATER_SHALLOW_IN		4
// yjpark     -->|

#define SURFACE_SAND 9
#define SURFACE_WATER 12
#define SURFACE_RED_SAND 13
#define SURFACE_GRASS 17
#define SURFACE_GRASS_SLIDING 19
#define SURFACE_GRASS_NOIMPACT 20
#define SURFACE_WOOD 18
#define SURFACE_SNOW 21

// Max ammo
#define MAX_BULLETS INDEX(500)
#define MAX_SHELLS INDEX(100)
#define MAX_ROCKETS INDEX(50)
#define MAX_GRENADES INDEX(50)
#define MAX_NAPALM INDEX(500)
#define MAX_ELECTRICITY INDEX(400)
#define MAX_IRONBALLS INDEX(30)
//#define MAX_NUKEBALLS INDEX(3)
//#define MAX_SNIPERBULLETS INDEX(50)

// Bit shifters for ammo
#define AMMO_BULLETS       0
#define AMMO_SHELLS        1
#define AMMO_ROCKETS       2
#define AMMO_GRENADES      3
#define AMMO_NAPALM        4
#define AMMO_ELECTRICITY   5
//#define AMMO_NUKEBALLS   6
#define AMMO_IRONBALLS     7
//#define AMMO_SNIPERBULLETS 8

#define BLOOD_SPILL_RED RGBAToColor(250,20,20,255)
#define BLOOD_SPILL_GREEN RGBAToColor(0,250,0,255)

// Ammo mana Value
#define AV_SHELLS         INDEX(70)
#define AV_BULLETS        INDEX(10)
#define AV_ROCKETS        INDEX(150)
#define AV_GRENADES       INDEX(150)
#define AV_ELECTRICITY    INDEX(250)
#define AV_IRONBALLS      INDEX(700)
//#define AV_NUKEBALLS      INDEX(1800)
#define AV_NAPALM         INDEX(200)
//#define AV_SNIPERBULLETS  INDEX(200)

// used for invisibility powerup
#define INVISIBILITY_ALPHA_LOCAL  0x55
#define INVISIBILITY_ALPHA_REMOTE 0x11

enum EmptyShellType {
  ESL_NONE = -1,
  ESL_BULLET = 0,
  ESL_SHOTGUN = 1,
  ESL_BUBBLE = 2,
  ESL_BULLET_SMOKE = 3,
  ESL_SHOTGUN_SMOKE = 4,
  ESL_COLT_SMOKE = 5,
};
// empty shell launch info
#define MAX_FLYING_SHELLS 32
struct ShellLaunchData {
  FLOAT sld_fSize;              // size multiplier
  FLOAT3D sld_vPos;             // launch position
  FLOAT3D sld_vSpeed;           // launch speed
  FLOAT3D sld_vUp;              // up vector in moment of launch
  FLOAT sld_tmLaunch;           // time of launch
  EmptyShellType sld_estType;   // shell type
};
#define ShellLaunchData_array m_asldData[MAX_FLYING_SHELLS]

// player bullet spray fx list
#define MAX_BULLET_SPRAYS 32
struct BulletSprayLaunchData {
  INDEX bsld_iRndBase;              // random base
  FLOAT3D bsld_vPos;                // launch position
  FLOAT3D bsld_vG;                  // gravity vector
  EffectParticlesType bsld_eptType; // type
  FLOAT bsld_tmLaunch;              // time of launch
  FLOAT3D bsld_vStretch;            // stretch
};
#define BulletSprayLaunchData_array m_absldData[MAX_BULLET_SPRAYS]

#define MAX_GORE_SPRAYS 32
struct GoreSprayLaunchData {
  FLOAT3D gsld_vPos;                // launch position
  FLOAT3D gsld_v3rdPos;             // launch position for 3rd perspective
  FLOAT3D gsld_vG;                  // gravity vector
  FLOAT gsld_fGA;                   // gravity strength
  SprayParticlesType gsld_sptType;  // type
  FLOATaabbox3D gsld_boxHitted;     // box of hitted object
  FLOAT3D gsld_vSpilDirection;      // spill direction
  FLOAT gsld_fDamagePower;          // damage power
  FLOAT gsld_tmLaunch;              // time of launch
  COLOR gsld_colParticles;          // color of particles
};
#define GoreSprayLaunchData_array m_agsldData[MAX_GORE_SPRAYS]

// world change
struct WorldChange {
  CTString strGroup;      // group name
  CPlacement3D plLink;    // link placement for relative change
  INDEX iType;            // change type
};
extern struct WorldChange _SwcWorldChange;

// entity info
struct EntityInfo {
  EntityInfoBodyType Eeibt;     // body type
  FLOAT fMass;                  // mass (in kg)
  FLOAT vSourceCenter[3];       // body point (offset from handle) when entity look another entity
  FLOAT vTargetCenter[3];       // body point (offset from handle) when entity is target of look
};

// entity info
struct EntityStats {
  CTString es_strName;
  INDEX es_ctCount;
  INDEX es_ctAmmount;
  FLOAT es_fValue;
  //INDEX es_iScore;
};

struct sSkillEffectInfo
{
	void InitForNormalAttack(CMobData* mob, INDEX aniID)
	{
		m_bSkillAttack = FALSE;
		szEffectNameCast = mob->GetFireEffect0();
		szEffectNameMissile = mob->GetFireEffect1();
		szEffectNameHit = mob->GetFireEffect2();
		iFireDelayCount = mob->GetDelayCount();
		fFireDelay[0] = mob->GetDelay(0);
		fFireDelay[1] = mob->GetDelay(1);
		fFireDelay[2] = mob->GetDelay(2);
		fFireDelay[3] = mob->GetDelay(3);
		iMissileType = mob->GetMissileType();
		fMissileSpeed = mob->GetMissileSpeed();
		iAnimatioID = aniID;
		dwValidValue = 0x00000000;
	}
	void InitForSkillAttack(CSkill &skill)
	{
		m_bSkillAttack = TRUE;
		szEffectNameCast = skill.GetFireEffect1(0);
		szEffectNameMissile = skill.GetFireEffect2(0);
		szEffectNameHit = skill.GetFireEffect3(0);
		iFireDelayCount = skill.GetDelayCount(0);
		fFireDelay[0] = skill.GetDelay(0,0);
		fFireDelay[1] = skill.GetDelay(1,0);
		fFireDelay[2] = skill.GetDelay(2,0);
		fFireDelay[3] = skill.GetDelay(3,0);
		iMissileType = skill.GetMissileType(0);
		fMissileSpeed = skill.GetMissileSpeed(0);
		iAnimatioID = skill.idPlayer_Anim_Skill[0][2];
		dwValidValue = 0x00000000;
	}
	
	BOOL		m_bSkillAttack;
	const char	*szEffectNameCast;
	const char	*szEffectNameMissile;
	const char	*szEffectNameHit;
	int			iFireDelayCount;
	FLOAT		fFireDelay[4];
	int			iMissileType;
	FLOAT		fMissileSpeed;
	INDEX		iAnimatioID;
	DWORD		dwValidValue;
};

extern INDEX dbg_bEnemyKillTest;
class EnemyKillData {
public:
  BOOL ekd_bKilled;
  BOOL ekd_bSpawner;
  INDEX ekd_ctToSpawn;
  SLONG ekd_ID;
  CTString ekd_strClass;
  CTString ekd_strSpawnClass;
  CTString ekd_strSpawnName;
  CTString ekd_strName;

  SLONG ekd_IDSpawner;
  CTString ekd_strNameSpawner;
  
  EnemyKillData(void)
  {
    ekd_bSpawner=FALSE;
    ekd_bKilled=FALSE;
    ekd_ID=-1;
    ekd_strClass="";
    ekd_strName="";
    ekd_IDSpawner=-1;
    ekd_strNameSpawner="Not spawned";
  }
};

void AddEnemyKillData(class CEnemyBase *penEnemy, class CEnemySpawner *penSpawner);
void AddSpawnerKillData(class CEnemySpawner *penSpawner, INDEX ctEnemies);
void MarkEnemyDead(class CEnemyBase *penEnemy);
void MarkEnemyDead(class CCharacterBase *penEnemy);
void MarkEnemyDead(class CPetBase *penEnemy);
void MarkEnemyDead(class CSlaveBase *penEnemy);
void MarkEnemyDead(class CWildPetBase *penEnemy);
void MarkSpawnerSpawned(class CEnemySpawner *penSpawner);
void DumpKillData(void);

// statistics data for player stats management
struct DECL_DLL PlayerStats {
  //INDEX ps_iScore;
  INDEX ps_iKills;
  INDEX ps_iDeaths;
  //INDEX ps_iSecrets;
  TIME  ps_tmTime;

  PlayerStats(void)
  {
    //ps_iScore = 0;
    ps_iKills = 0;
    ps_iDeaths = 0;
//    ps_iSecrets = 0;
    ps_tmTime = 0.0f;
  }
};

// get info position for entity
DECL_DLL void GetEntityInfoPosition(CEntity *pen, FLOAT *pf, FLOAT3D &vPos);
// get source and target positions for ray cast
DECL_DLL void GetPositionCastRay(CEntity *penSource, CEntity *penTarget, FLOAT3D &vSource, FLOAT3D &vTarget);

// set bool from bool enum type
DECL_DLL void SetBoolFromBoolEType(BOOL &bSet, BoolEType bet);
// send event to target
DECL_DLL void SendToTarget(CEntity *penSendEvent, EventEType eetEventType, CEntity *penCaused = NULL, CEntityEvent* peeEvent = NULL);
// send event in range
DECL_DLL void SendInRange(CEntity *penSource, EventEType eetEventType, const FLOATaabbox3D &boxRange);

// spawn reminder
DECL_DLL CEntityPointer SpawnReminder(CEntity *penOwner, FLOAT fWaitTime, INDEX iValue);
// spawn flame
DECL_DLL CEntityPointer SpawnFlame(CEntity *penOwner, CEntity *penAttach, const FLOAT3D &vSource);

// Set components
DECL_DLL void SetComponents(CEntity *pen, CModelObject &mo, ULONG ulIDModel, ULONG ulIDTexture,
                   ULONG ulIDReflectionTexture, ULONG ulIDSpecularTexture, ULONG ulIDBumpTexture);
// Add attachment to model
DECL_DLL void AddAttachmentToModel(CEntity *pen, CModelObject &mo, INDEX iAttachment, ULONG ulIDModel, ULONG ulIDTexture,
                          ULONG ulIDReflectionTexture, ULONG ulIDSpecularTexture, ULONG ulIDBumpTexture);
// Remove attachment from model
DECL_DLL void RemoveAttachmentFromModel(CModelObject &mo, INDEX iAttachment);

// Kick entity
DECL_DLL void KickEntity(CEntity *penTarget, FLOAT3D vSpeed);

DECL_DLL void LoadSkaModel(CModelInstance &m_miModel, CEntity *penThis, INDEX iSkaModelComponentID);
DECL_DLL void StartSkaAnim(CEntity *pen, INDEX iAnimID, DWORD dwFlags, INDEX iGroupID, FLOAT fFadeTime);
DECL_DLL void StartSkaAnimStr(CModelInstance *m_miModel, CTString strAnimName, DWORD dwFlags, INDEX iGroupID, FLOAT fFadeTime);

// lens flare variables
extern CLensFlareType _lftStandard;
extern CLensFlareType _lftStandardReflections;
extern CLensFlareType _lftYellowStarRedRing;
extern CLensFlareType _lftYellowStarRedRingFar;
extern CLensFlareType _lftWhiteGlowStarRedRing;
extern CLensFlareType _lftWhiteGlowStar;
extern CLensFlareType _lftWhiteGlowStarNG;
extern CLensFlareType _lftWhiteStarRedRingStreaks;
extern CLensFlareType _lftWhiteStarRedReflections;
extern CLensFlareType _lftBlueStarBlueReflections;
extern CLensFlareType _lftProjectileStarGlow;
extern CLensFlareType _lftProjectileWhiteBubbleGlow;
extern CLensFlareType _lftProjectileYellowBubbleGlow;
//extern CLensFlareType _lftPVSpaceShipWindowFlare;
//extern CLensFlareType _lftCatmanFireGlow;
extern CLensFlareType _lftWhiteGlowFar;
// init lens flare effects
void InitLensFlares(void);
// close lens flares effects
void CloseLensFlares(void);

DECL_DLL BOOL SetPlayerAppearance(CModelObject *mo, CPlayerCharacter *ppc, CTString &strName, BOOL bPreview);
//0105
//SKA
DECL_DLL BOOL SetPlayerAppearanceSka(CModelInstance *pmiModel, CPlayerCharacter *ppc, CTString &strName, BOOL bPreview, ULONG type = 0);
//..
// debugging functions
DECL_DLL const char *PrintConsole(void);
DECL_DLL const char *PrintStack(CEntity *pen);
DECL_DLL void StartDebugTrigger(CEntity *penCaused);

// debris spawning
DECL_DLL void Debris_Begin(
  EntityInfoBodyType Eeibt, 
  enum DebrisParticlesType dptParticles,
  enum BasicEffectType  betStain,
  FLOAT fEntitySize,             // entity size in meters
  const FLOAT3D &vSpeed,
  const FLOAT3D &vSpawnerSpeed,  // how fast was the entity moving
  const FLOAT fConeSize,         // size multiplier for debris cone
  const FLOAT fSpeedUp,          // size multiplier for debris catapulting up (0-no multiply)
  const COLOR colDebris = C_WHITE, // multiply color
  const BOOL bCollision	= FALSE,
  const BOOL bFade	= TRUE
);
DECL_DLL CEntityPointer Debris_Spawn(
  CEntity *penSpawner,
  CEntity *penComponents,
  SLONG idModelComponent,
  SLONG idTextureComponent,
  SLONG idReflectionTextureComponent,
  SLONG idSpecularTextureComponent,
  SLONG idBumpTextureComponent,
  INDEX iModelAnim,
  FLOAT fSize,                // size relative to entity size (or 0 for absolute stretch of 1)
  const FLOAT3D &vPosRatio);
DECL_DLL CEntityPointer Debris_Spawn_Independent(
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
  ANGLE3D aRotation);
DECL_DLL CEntityPointer Debris_Spawn_Template(
  EntityInfoBodyType eibt,
  enum DebrisParticlesType dptParticles,
  enum BasicEffectType betStain,
  class CModelHolder2 *penmhDestroyed,
  CEntity *penComponents,
  class CModelHolder2 *penmh2,
  FLOAT3D vStretch,
  FLOAT fSize,
  CPlacement3D plAbsolutePlacement,
  FLOAT3D vLaunchSpeed,
  ANGLE3D aRotSpeed,
  BOOL bDebrisImmaterialASAP,
  FLOAT fDustStretch,
  COLOR colBurning);
// spawn a ModelHolder2 based debris from a ModelHolder2 parent
DECL_DLL CEntityPointer Debris_Spawn_MH2Template_FromMH2(
  EntityInfoBodyType eibt,
  enum DebrisParticlesType dptParticles,
  enum BasicEffectType betStain,
  class CModelHolder2 *penmhDestroyed,
  CEntity *penComponents,
  class CModelHolder2 *penmh2,
  FLOAT3D vStretch,
  FLOAT fSize,
  CPlacement3D plAbsolutePlacement,
  FLOAT3D vLaunchSpeed,
  ANGLE3D aRotSpeed,
  BOOL bDebrisImmaterialASAP,
  FLOAT fDustStretch,
  COLOR colBurning);
// spawn a ModelHolder2 based debris from a ModelHolder3 parent
DECL_DLL CEntityPointer Debris_Spawn_MH2Template_FromMH3(
  EntityInfoBodyType eibt,
  enum DebrisParticlesType dptParticles,
  enum BasicEffectType betStain,
  class CModelHolder3 *penmhDestroyed,
  CEntity *penComponents,
  class CModelHolder2 *penmh2,
  FLOAT3D vStretch,
  FLOAT fSize,
  CPlacement3D plAbsolutePlacement,
  FLOAT3D vLaunchSpeed,
  ANGLE3D aRotSpeed,
  BOOL bDebrisImmaterialASAP,
  FLOAT fDustStretch,
  COLOR colBurning);
// spawn a ModelHolder3 based debris from a ModelHolder2 parent
DECL_DLL CEntityPointer Debris_Spawn_MH3Template_FromMH2(
  EntityInfoBodyType eibt,
  enum DebrisParticlesType dptParticles,
  enum BasicEffectType betStain,
  class CModelHolder2 *penmhDestroyed,
  CEntity *penComponents,
  class CModelHolder3 *penmh3,
  FLOAT3D vStretch,
  FLOAT fSize,
  CPlacement3D plAbsolutePlacement,
  FLOAT3D vLaunchSpeed,
  ANGLE3D aRotSpeed,
  BOOL bDebrisImmaterialASAP,
  FLOAT fDustStretch,
  COLOR colBurning);
// spawn a ModelHolder3 based debris from a ModelHolder3 parent
DECL_DLL CEntityPointer Debris_Spawn_MH3Template_FromMH3(
  EntityInfoBodyType eibt,
  enum DebrisParticlesType dptParticles,
  enum BasicEffectType betStain,
  class CModelHolder3 *penmhDestroyed,
  CEntity *penComponents,
  class CModelHolder3 *penmh3,
  FLOAT3D vStretch,
  FLOAT fSize,
  CPlacement3D plAbsolutePlacement,
  FLOAT3D vLaunchSpeed,
  ANGLE3D aRotSpeed,
  BOOL bDebrisImmaterialASAP,
  FLOAT fDustStretch,
  COLOR colBurning);

DECL_DLL CEntityPointer DebrisSka_Spawn(CEntity *penSpawner,  INDEX iSkaModelComponentID,  
              FLOAT fSize, const FLOAT3D &vPosRatio);
DECL_DLL CEntityPointer DebrisSka_Spawn_Independent( CEntity *penSpawner, INDEX iSkaModelComponentID,
              FLOAT fSize, CPlacement3D plAbsolute, FLOAT3D vTranslation, ANGLE3D aRotation);

// get default entity info for given body type
DECL_DLL EntityInfo *GetStdEntityInfo(EntityInfoBodyType eibt);
// damage control functions
DECL_DLL FLOAT DamageStrength(EntityInfoBodyType eibtBody, enum DamageType dtDamage);

// Print center screen message
//DECL_DLL void PrintCenterMessage(CEntity *penThis, CEntity *penTarget, 
//  const CTString &strMessage, TIME tmLength, enum MessageSound mssSound);

// get name of a key item
//DECL_DLL const char *GetKeyName(enum KeyItemType kit);

// get session properties
DECL_DLL inline const CSessionProperties *GetSP(void)
{
  return (const CSessionProperties *)_pNetwork->GetSessionProperties();
}

// i.e. weapon sound when fireing or exploding
DECL_DLL void SpawnRangeSound( CEntity *penPlayer, CEntity *penPos, enum SoundType st, FLOAT fRange);

// get some player for trigger source if any is existing
DECL_DLL CEntity *FixupCausedToPlayer(CEntity *penThis, CEntity *penCaused, BOOL bWarning=TRUE);

// precisely lerp between two placement using quaternions
DECL_DLL CPlacement3D LerpPlacementsPrecise(const CPlacement3D &pl0, const CPlacement3D &pl1, FLOAT fRatio);

// obtain game extra damage per enemy and per player
DECL_DLL FLOAT GetGameDamageMultiplier(void);

// get entity's serious damage multiplier
DECL_DLL FLOAT GetSeriousDamageMultiplier( CEntity *pen);

// get current world settings controller
DECL_DLL class CWorldSettingsController *GetWSC(CEntity *pen);

// helper functions

// distance between two entities
DECL_DLL inline FLOAT DistanceTo(CEntity *penE1, CEntity *penE2) {
    return (penE1->GetPlacement().pl_PositionVector -
            penE2->GetPlacement().pl_PositionVector).Length();
}

BulletHitType GetBulletHitTypeForSurface(INDEX iSurfaceType);
EffectParticlesType GetParticleEffectTypeForSurface(INDEX iSurfaceType);
// spawn effect from hit type
void SpawnHitTypeEffect(CEntity *pen, enum BulletHitType bhtType, BOOL bSound, FLOAT3D vHitNormal, FLOAT3D vHitPoint,
  FLOAT3D vIncommingBulletDir, FLOAT3D vDistance);

#define FRndIn(a, b) (a + FRnd()*(b - a))


DECL_DLL inline FLOAT AngleBetweenNormVectors(FLOAT3D v1, FLOAT3D v2, INDEX iMaskH, INDEX iMaskP, INDEX iMaskB) {
  
  ASSERT(iMaskH || iMaskP || iMaskB);

  if (!iMaskH) {
    v1(1) = 0.0f;
    v2(1) = 0.0f;
  }
  if (!iMaskP) {
    v1(2) = 0.0f;
    v2(2) = 0.0f;
  }
  if (!iMaskB) {
    v1(3) = 0.0f;
    v2(3) = 0.0f;
  }
  if (!iMaskH || !iMaskP || !iMaskB) {
    v1.Normalize();
    v2.Normalize();
  }

  return (v1%v2);
}

DECL_DLL inline FLOAT AngleBetweenNormVectors(FLOAT3D v1, FLOAT3D v2) {  
  return (v1%v2);
}

DECL_DLL inline FLOAT AngleBetweenVectors(FLOAT3D v1, FLOAT3D v2, INDEX iMaskH, INDEX iMaskP, INDEX iMaskB) {
  
  if (iMaskH && iMaskP && iMaskB) {
    v1.Normalize();
    v2.Normalize();
  }

  return (AngleBetweenNormVectors(v1, v2, iMaskH, iMaskP, iMaskB));
}

DECL_DLL inline FLOAT AngleBetweenVectors(FLOAT3D v1, FLOAT3D v2) {
 
  v1.Normalize();
  v2.Normalize();
 
  return (AngleBetweenNormVectors(v1, v2));
}

//void SendScoreToEntity(CEntity *penTarget, INDEX iScore);

//INDEX ska_CreateAnimIDFromString(CModelInstance *pmi, CTString strAnimName, BOOL bRecursive = FALSE);
//INDEX ska_CreateBoneIDFromString(CModelInstance *pmi, CTString strBoneName);
//INDEX ska_CreateCollisionIDFromString(CModelInstance *pmi, CTString strCollisionName);
//INDEX ska_CreateChildIDFromString(CModelInstance *pmi, CTString strChildName, BOOL bRecursive = FALSE);
void ska_CreateAnimIDFromString(INDEX *piID, CModelInstance *pmi, CTString strAnimName, BOOL bRecursive = FALSE);
void ska_CreateBoneIDFromString(INDEX *piID, CModelInstance *pmi, CTString strBoneName);
void ska_CreateCollisionIDFromString(INDEX *piID, CModelInstance *pmi, CTString strCollisionName);
void ska_CreateChildIDFromString(INDEX *piID, CModelInstance *pmi, CTString strChildName, BOOL bRecursive = FALSE);

#define FADE_VERYSHORT 0.10f
#define FADE_SHORT  0.20f
#define FADE_MEDIUM 0.35f
#define FADE_LONG   0.50f

INDEX CollisionBoxIndexFromID(CModelInstance *pmi, INDEX ID);

// call these only during AdjustBones() entity function !!!!!
BOOL RotateBone(INDEX idBone, ANGLE3D a);
BOOL OffsetBone(INDEX idBone, FLOAT3D v);

INDEX AddSkaChild(CEntity *penThis, CEntity *penParent, INDEX iSkaModelComponentID,
                  const CTString &strChildName, INDEX iParentBoneID);
INDEX AddSkaChild(CEntity *penThis, CModelInstance *pmiParent, INDEX iSkaModelComponentID,
                  const CTString &strChildName, INDEX iParentBoneID);
void  AddSkaChildFromMI(CModelInstance *pmiParent, CModelInstance *pmi,
                  const CTString &strChildName, INDEX iParentBoneID);
void  RemoveSkaChildren(CModelInstance *pmi);
BOOL  RemoveSkaChild(CEntity *penParent, INDEX iChildID);
BOOL  RemoveSkaChild(CEntity *penParent, CTString strChildName);
FLOAT MipFactor_LogToMetric(FLOAT fMipLog);
FLOAT MipFactor_MetricToLog(FLOAT fMipMetric);
void  AddToAutoAimables(CEntity *pen);
void  RemoveFromAutoAimables(CEntity *pen);
void  ScaleEntityInfo(EntityInfo *eiInfo, FLOAT fScale);

BOOL RotateSkaChild(CEntity *penParent, CTString strChildName);

extern BOOL plr_bSamForSequences;

class HUD_MLData 
{
public:
	HUD_MLData()
	{
		bEnable = FALSE;
		Init();
	}

	~HUD_MLData()
	{
		HUD_DeleteMI();
	}

public:
	//CModelInstance*	hud_MI;
	CEntity		hud_MI;
	CItemEffect	hud_ItemEffect;
	CEffectGroupManager::my_list listProcess;
	BOOL bEnable;
	INDEX iWearingItems[7]; // 0 Weapon // 날개는 패스~
	INDEX iAnim;
	

	void Init()
	{
		hud_MI.en_EntityUseType = CEntity::EU_DUMMY;
		hud_MI.InitAsSkaModel();
		hud_ItemEffect.SetItemERType(ER_IN_UI);
		HUD_CreateMI();

		for (int i=0; i<7; ++i)
		{
			iWearingItems[i] = 0;
		}
		iAnim = 0;
	}

	void SetWearingItems(INDEX iRef, INDEX iType, INDEX iJob, INDEX iItem)
	{
		CJobInfo* pInfo = CJobInfo::getSingleton();

		if (iWearingItems[iRef] > 0)
		{
			_pGameState->TakeOffArmorTest(hud_MI.GetModelInstance(),iWearingItems[iRef]);
			CItemData*	pItemData = _pNetwork->GetItemData(iWearingItems[iRef]);
			hud_ItemEffect.Change(iJob, pItemData, iType, -1, &hud_MI.GetModelInstance()->m_tmSkaTagManager, 1, pItemData->GetSubType());
		}
		else
		{
			if (iRef > 0)
			{
				_pGameState->DeleteDefaultArmor(hud_MI.GetModelInstance(), iType, iJob);
			}
		}

		if (iItem > 0)
		{
			_pGameState->WearingArmorTest(hud_MI.GetModelInstance(), iItem);
			CItemData* pItemData = _pNetwork->GetItemData(iItem);
			hud_ItemEffect.Change(iJob, pItemData, iType, 0, &hud_MI.GetModelInstance()->m_tmSkaTagManager, 1, pItemData->GetSubType());
			iWearingItems[iRef] = iItem;
			if (iWearingItems[0] > 0)
			{
				CItemData* pItemData = _pNetwork->GetItemData(iWearingItems[0]);

				if (pItemData->GetSubType() == pInfo->GetSkillWeponType(_pNetwork->MyCharacterInfo.job, 1))
				{
					hud_MI.AddAnimation(ska_GetIDFromStringTable(pInfo->GetAnimationName(_pNetwork->MyCharacterInfo.job, ANIM_EXT_ATTACK_IDLE)),
					AN_LOOPING|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
				}
				else
				{
					hud_MI.AddAnimation(ska_GetIDFromStringTable(pInfo->GetAnimationName(_pNetwork->MyCharacterInfo.job, ANIM_ATTACK_IDLE)),
					AN_LOOPING|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
				}
			}
			else
			{
				hud_MI.AddAnimation(ska_GetIDFromStringTable(pInfo->GetAnimationName(_pNetwork->MyCharacterInfo.job, ANIM_IDLE)),
				AN_LOOPING|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			}
		}
		else
		{
			if (iType == WEAR_WEAPON)
			{
				hud_MI.AddAnimation(ska_GetIDFromStringTable(pInfo->GetAnimationName(_pNetwork->MyCharacterInfo.job, ANIM_IDLE)),
				AN_LOOPING|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			}
			else
			{
				CTFileName fnFileName;
				MeshInstance *mi;

				int iWearPos = -1;
				switch( iType )
				{
				case WEAR_HELMET:
					iWearPos = HEAD;
					break;
					
				case WEAR_JACKET:
					iWearPos = BODYUP;
					break;
				case WEAR_PANTS:
					iWearPos = BODYDOWN;
					break;
				case WEAR_GLOVES:
					iWearPos = HAND;
					break;
				case WEAR_BOOTS:
					iWearPos = FOOT;
					break;
				}

				if( iWearPos == -1 )
				{
					return;
				}

				if( iType == WEAR_PANTS )
				{
					// Mesh
					fnFileName = pInfo->GetMeshName( iJob, SKIRT );
					if(strlen( fnFileName ) > 0)
					{			
						mi = hud_MI.GetModelInstance()->AddArmor( fnFileName );

						// Texture
						fnFileName = pInfo->GetTextureName( iJob, SKIRT );
						hud_MI.GetModelInstance()->AddTexture_t( fnFileName, fnFileName.FileName(), mi );	

						// NormalMap
						fnFileName = pInfo->GetTexNormalName( iJob, SKIRT );
						if(strcmp(fnFileName, ""))
						{					
							hud_MI.GetModelInstance()->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
						}
					}
				}

				if (iType == WEAR_HELMET )// 헬멧 이외 장비 처리
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh(hud_MI.GetModelInstance(), iJob, _pNetwork->MyCharacterInfo.hairStyle - 1);
				}else{									
					// Mesh
					fnFileName = pInfo->GetMeshName( iJob, iWearPos );
					mi = hud_MI.GetModelInstance()->AddArmor( fnFileName );

					// Texture
					fnFileName = pInfo->GetTextureName( iJob, iWearPos );
					hud_MI.GetModelInstance()->AddTexture_t( fnFileName, fnFileName.FileName(), mi );

					// NormalMap
					fnFileName = pInfo->GetTexNormalName( iJob, iWearPos );
					if(strcmp(fnFileName, ""))
					{
						hud_MI.GetModelInstance()->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
					}
				}
			}

			iWearingItems[iRef] = 0;
			hud_ItemEffect.Refresh(&hud_MI.GetModelInstance()->m_tmSkaTagManager, 1);
		}
	}

	void SetLoginWearingItems(INDEX iRef, INDEX iType, INDEX iJob, INDEX iItem)
	{
		CJobInfo* pInfo = CJobInfo::getSingleton();

		if (pInfo == NULL)
		{
			return;
		}

		if (iWearingItems[iRef] > 0)
		{
			_pGameState->TakeOffArmorTest(hud_MI.GetModelInstance(),iWearingItems[iRef]);
			CItemData*	pItemData = _pNetwork->GetItemData(iWearingItems[iRef]);
			hud_ItemEffect.Change(iJob, pItemData, iType, -1, &hud_MI.GetModelInstance()->m_tmSkaTagManager, 1, pItemData->GetSubType());
		}
		else
		{
			if (iRef > 0 && iItem > 0)
			{
				CItemData* pItemData = _pNetwork->GetItemData(iItem);
				CTString strItemSmc = pItemData->GetItemSmcFileName();

				if (strItemSmc == MODEL_TREASURE)
				{
					return;
				}

				_pGameState->DeleteDefaultArmor(hud_MI.GetModelInstance(), iType, iJob);
			}
		}

		if (iItem > 0)
		{
			_pGameState->WearingArmorTest(hud_MI.GetModelInstance(), iItem);
			CItemData* pItemData = _pNetwork->GetItemData(iItem);
			hud_ItemEffect.Change(iJob, pItemData, iType, 0, &hud_MI.GetModelInstance()->m_tmSkaTagManager, 1, pItemData->GetSubType());
			iWearingItems[iRef] = iItem;
		}
		else
		{
			if (iType != WEAR_WEAPON)
			{
				CTFileName fnFileName;
				MeshInstance *mi;

				int iWearPos = -1;
				switch( iType )
				{
				case WEAR_HELMET:
					iWearPos = HEAD;
					break;

				case WEAR_JACKET:
					iWearPos = BODYUP;
					break;
				case WEAR_PANTS:
					iWearPos = BODYDOWN;
					break;
				case WEAR_GLOVES:
					iWearPos = HAND;
					break;
				case WEAR_BOOTS:
					iWearPos = FOOT;
					break;
				}

				if( iWearPos == -1 )
				{
					return;
				}

				if( iType == WEAR_PANTS )
				{
					// Mesh
					fnFileName = pInfo->GetMeshName( iJob, SKIRT );
					if(strlen( fnFileName ) > 0)
					{			
						mi = hud_MI.GetModelInstance()->AddArmor( fnFileName );

						// Texture
						fnFileName = pInfo->GetTextureName( iJob, SKIRT );
						hud_MI.GetModelInstance()->AddTexture_t( fnFileName, fnFileName.FileName(), mi );	

						// NormalMap
						fnFileName = pInfo->GetTexNormalName( iJob, SKIRT );
						if(strcmp(fnFileName, ""))
						{					
							hud_MI.GetModelInstance()->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
						}
					}
				}

				if(iType != WEAR_HELMET )
				{									
					// Mesh
					fnFileName = pInfo->GetMeshName( iJob, iWearPos );
					mi = hud_MI.GetModelInstance()->AddArmor( fnFileName );

					// Texture
					fnFileName = pInfo->GetTextureName( iJob, iWearPos );
					hud_MI.GetModelInstance()->AddTexture_t( fnFileName, fnFileName.FileName(), mi );

					// NormalMap
					fnFileName = pInfo->GetTexNormalName( iJob, iWearPos );
					if(strcmp(fnFileName, ""))
					{
						hud_MI.GetModelInstance()->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
					}
				}
			}

			iWearingItems[iRef] = 0;
			hud_ItemEffect.Refresh(&hud_MI.GetModelInstance()->m_tmSkaTagManager, 1);
		}
	}
	
	void HUD_CreateMI()
	{
		hud_MI.en_pmiModelInstance = CreateModelInstance("");
		hud_MI.en_pmiModelInstance->mi_bDummyModel = TRUE;
	}

	void HUD_DeleteMI()
	{
		hud_MI.End();
		bEnable = FALSE;
	}

	void HUD_CopyMI(CModelInstance* copyMI)
	{
		hud_MI.GetModelInstance()->Copy((*copyMI));
		hud_MI.GetModelInstance()->m_tmSkaTagManager.SetOwner(&hud_MI);
		CSkaTag tag;
		tag.SetName("__ROOT");
		tag.SetOffsetRot(GetEulerAngleFromQuaternion(hud_MI.GetModelInstance()->mi_qvOffset.qRot));
		hud_MI.GetModelInstance()->m_tmSkaTagManager.Register(&tag);
		tag.SetName("__TOP");
		tag.SetOffsetRot(GetEulerAngleFromQuaternion(hud_MI.GetModelInstance()->mi_qvOffset.qRot));
		FLOATaabbox3D aabb;
		hud_MI.GetModelInstance()->GetAllFramesBBox(aabb);
		tag.SetOffsetPos(0, aabb.Size()(2) * hud_MI.GetModelInstance()->mi_vStretch(2), 0);
		hud_MI.GetModelInstance()->m_tmSkaTagManager.Register(&tag);
		bEnable = TRUE;
	}

	void HUD_CopyItemEffect(CItemEffect* copyItemEffect)
	{
		hud_ItemEffect.Clear();
		hud_ItemEffect = (*copyItemEffect);
		hud_ItemEffect.DeleteEffect(WEAR_WEAPON, TRUE);
		hud_ItemEffect.Refresh(&hud_MI.GetModelInstance()->m_tmSkaTagManager, 1);
	}

	void HUD_SetItemEffect()
	{
		hud_ItemEffect.Clear();
		hud_ItemEffect.DeleteEffect(WEAR_WEAPON, TRUE);
		hud_ItemEffect.AddLoginEffect(&hud_MI.GetModelInstance()->m_tmSkaTagManager);
		//hud_ItemEffect.AddPetStashEffect(0,&hud_MI.GetModelInstance()->m_tmSkaTagManager);
		hud_ItemEffect.Refresh(&hud_MI.GetModelInstance()->m_tmSkaTagManager, 1);
	}

};