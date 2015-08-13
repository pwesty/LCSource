401
%{
	
#include "StdH.h"
#include "GameMP/SEColors.h"
	
#include <Engine/Build.h>
#include <locale.h>
	
#include "ModelsMP/Player/SeriousSam/Player.h"
#include "ModelsMP/Player/SeriousSam/Body.h"
#include "ModelsMP/Player/SeriousSam/Head.h"
#include "EntitiesMP/Common/CharacterDefinition.h"

#include "EntitiesMP/PlayerMarker.h"
#include "EntitiesMP/PlayerAnimator.h"
#include "EntitiesMP/PlayerView.h"
#include "EntitiesMP/MovingBrush.h"
#include "EntitiesMP/Switch.h"
#include "EntitiesMP/Camera.h"
#include "EntitiesMP/WorldLink.h"
#include "EntitiesMP/Pet.h"
#include "EntitiesMP/WildPet.h"
#include "EntitiesMP/Slave.h"
#include "EntitiesMP/EnemySpawner.h"
#include "EntitiesMP/PlayerActionMarker.h"
#include "EntitiesMP/WorldSettingsController.h"
#include "EntitiesMP/HudPicHolder.h"
#include "EntitiesMP/DoorController.h"
#include "EntitiesMP/WorldBase.h"

//강동민 수정 시작 시스템 싱글던젼 개선	10.07
// 고정 시점 카메라 & 제한 영역
#include "EntitiesMP/Common/FixedCameraContainer.h"
//강동민 수정 끝 싱글던젼 개선		10.07
	
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)	
#include <Engine/Effect/CSkaEffect.h>
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
//안태훈 수정 시작	//(Zone Change System)(0.1)
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIManager.h>
//안태훈 수정 끝	//(Zone Change System)(0.1)
//안태훈 수정 시작	//(5th Closed beta)(0.2)
#define PICKING_EFFECT_REMAIN_TIME (0.0f)
//안태훈 수정 끝	//(5th Closed beta)(0.2)

#include <Engine/GlobalDefinition.h>
#include <Engine/Terrain/Terrain.h>

#include <Engine/Interface/UIAutoHelp.h> 
#include <Engine/Interface/UISignBoard.h> 
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include <Engine/Interface/UIFlowerTree.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Interface/UISiegeWarfareNew.h>
#include <Engine/Interface/UIGuildBattle.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Interface/UIReformSystem.h>
#include <Engine/Interface/UISelectResource.h>
#include <Engine/Interface/UIPersonalShop.h>
#include <Engine/Interface/UIGuildWarPortal.h>
#include <Engine/Interface/UIGuildStash.h>
#include <Engine/Interface/UIBilling.h>
#include <Engine/Contents/function/UIPortalNew.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/Base/UIChangeWeaponNew.h>
#include <Engine/Interface/UIResurrection.h>
#include <Engine/Interface/UIAffinity.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UIInitJob.h>
#include <Engine/Interface/UIWildPetInfo.h>
#include <Engine/Interface/UIGamble.h>
#include <Engine/Interface/UIMonsterMercenary.h>
#include <Engine/Interface/UISystemMenu.h>

#include <Engine/TransformInfo.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/ExpressSystem.h>
#include <Engine/Contents/Login/ServerSelect.h>
#include <Engine/Contents/Login/CharacterSelect.h>
#include <Engine/Contents/Login/UICharacterCreateNew.h>
#include <Engine/Contents/Base/Notice.h>
#include <Engine/LoginJobInfo.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Contents/Base/UISkillNew.h>
#include <Engine/Contents/Base/UIQuestAccept.h>
#include <Engine/Contents/Base/Party.h>
#include <algorithm>
#include <Engine/GameStageManager/StageMgr.h>
#include <Engine/Object/ActorMgr.h>

#include <Common/Packet/ptype_old_do_action.h>
#include <Common/Packet/ptype_old_do_skill.h>
#include <Common/Packet/ptype_old_do_move.h>
#include <Common/Packet/ptype_char_status.h>
#include <Common/Packet/ptype_appear.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Contents/function/SimplePlayerInfoUI.h>

// Date : 2005-09-23(오전 9:27:43), By Lee Ki-hwan
// 버튼 액션시 보내던 MOVE메세지에 대한 제한을 두기위한 플래그 
#define BUTTON_MOVE_ENABLE	0

//	extern void JumpFromBouncer(CEntity *penToBounce, CEntity *penBouncer);
	// from game
#define GRV_SHOWEXTRAS  (1L<<0)   // add extra stuff like console, weapon, pause
	
// FIXME : GENDER가 쓰이는 부분도 제거대상.
#define GENDER_MALE     0
#define GENDER_FEMALE   1
#define GENDEROFFSET    100   // sound components for genders are offset by this value

#define MAX_TARGETS		20

#define SOUND_BSP_COUNT 3		// yjpark
#define SOUND_TER_COUNT 4		// yjpark

#define		HITTYPE_MISS			(1 << 0)		// 회피
#define		HITTYPE_WEAK			(1 << 1)		// 약하게
#define		HITTYPE_NORMAL			(1 << 2)		// 보통
#define		HITTYPE_STRONG			(1 << 3)		// 강하게
#define		HITTYPE_HARD			(1 << 4)		// 매우 강하게
#define		HITTYPE_CRITICAL		(1 << 5)		// 크리티컬
#define		HITTYPE_DEADLY			(1 << 6)		// 죽도록 강하게

// NOTE : 심하게 밀리는 싱글던젼...
#define	EXTREAM_SINGLEDUNGEON		(10)
	
	//0105
	//INDEX idPlayerWhole_Animation[ANIM_TOTAL]	= {0};
	ENGINE_API extern INDEX idPlayerWhole_Animation[ANIM_TOTAL];

	//INDEX idPlayerWhole_Anim_Face1 = -1;
	//INDEX idPlayerWhole_Anim_Face2 = -1;
	INDEX idPlayerWhole_Anim_Face3 = -1;
	INDEX idPlayerWhole_Anim_Face4 = -1;

	// bone ID's
	// collision boxes
	INDEX idPlayer_Collision_Stand     = -1;

#define PLAYER_ANIM_GROUP_LEGS        0x01
#define PLAYER_ANIM_GROUP_TORSO       0x02
#define PLAYER_ANIM_GROUP_WHOLEBODY   0x03

#define AREA_CASTLE_X0	1015
#define AREA_CASTLE_X1	1340
#define AREA_CASTLE_Z0	815
#define AREA_CASTLE_Z1	1173

#define AREA_CASTLE_DRATAN_X0	1570
#define AREA_CASTLE_DRATAN_X1	2024
#define AREA_CASTLE_DRATAN_Z0	105
#define AREA_CASTLE_DRATAN_Z1	650

#define MUSIC_CASTLE_AREA		CTFILENAME("data\\sounds\\game\\BGM\\GB_pease.ogg")
#define MUSIC_FIELD_AREA		CTFILENAME("data\\sounds\\game\\BGM\\GB_field01.ogg")
#define MUSIC_FIELD_ENVIRON		CTFILENAME("data\\sounds\\game\\BGM\\GBE_field.ogg")

#define MUSIC_DRATAN_ENVIRON		CTFILENAME("data\\sounds\\game\\BGM\\GBE_ruin.ogg")

#define MUSIC_DUNGEON_AREA      CTFILENAME("data\\sounds\\game\\BGM\\GB_dungeon01.ogg")
#define MUSIC_DUNGEON_ENVIRON   CTFILENAME("data\\sounds\\game\\BGM\\GBE_dungeon.ogg")

#define MUSIC_CASTLE_BATTLE		CTFILENAME("Data\\Sounds\\Game\\BGM\\GB_battletheme.ogg")
#define MUSIC_MERAC_FIELD		CTFILENAME("Data\\Sounds\\Game\\BGM\\GB_meracpease.ogg")

// egeha
#define MUSIC_EGEHA_PEASE		CTFILENAME("Data\\Sounds\\Game\\BGM\\GB_egehapease.ogg")

#define MODEL_TREASURE	("Data\\Item\\Common\\ITEM_treasure02.smc")

// Check Attack Flagg
#define ENF_EX2_PVP				(1L<<0)
#define ENF_EX2_LEGIT			(1L<<1)
#define ENF_EX2_MYPARTY			(1L<<2)
#define ENF_EX2_MYGUILD			(1L<<3)
#define ENF_EX2_ENEMYGUILD		(1L<<4)
#define ENF_EX2_WAR_OURFORCE	(1L<<5)
#define ENF_EX2_WAR_ENEMY		(1L<<6)

#ifdef SIGEWAR_ATTACKRULE
	const BOOL bSigewar_Attackrule = TRUE;
#else
	const BOOL bSigewar_Attackrule = FALSE;
#endif
#ifdef CHAR_EX_ROGUE	// [2012/08/27 : Sora] EX로그 추가
	const BOOL charEXRogue = TRUE;
#else
	const BOOL charEXRogue = FALSE;
#endif
#ifdef CHAR_EX_MAGE	//2013/01/08 jeil EX메이지 추가 
	const BOOL charEXMage = TRUE;
#else
	const BOOL charEXMage = FALSE;
#endif

#ifdef HP_PERCENTAGE // 플레이어 es HP 뭐로 쓸지 [3/29/2013 Ranma]
	const BOOL bHp_Percentage = TRUE;
#else	// HP_PERCENTAGE
	const BOOL bHp_Percentage = FALSE;
#endif	// HP_PERCENTAGE

// enum으로 변경한다. 이유는 서로 겹치면 안되기 때문에 코드상 block처리함. [5/23/2011 rumist]
enum __tagRaidObjectNOutputType
{
	OBJECT_TOUCH_FIELD			= 0,				// same type : OBJECT_TOUCH_FIELD	= 0,
	OBJECT_GATE					= 1,
	OBJECT_PORTAL				= 2,
	OBJECT_ITEM					= 3,
	OBJECT_PADOX_MODEL			= 4,
	OUTPUT_TOGGLE_GATE			= 7,
	OUTPUT_TOGGLE_PORTAL		= 8,
	OUTPUT_PAUSE_AREA			= 12,
	OUTPUT_MODEL_HOLDER_PADOX	= 17,
	OUTPUT_MODEL_NPC_SAY		= 18,				// forgotten temple [5/23/2011 rumist]
};


#define JOINWAR_NONE		0; // 공성 참여 안한 그룹
#define JOINWAR_ATTACK		1; // 공성 공격 그룹
#define JOINWAR_DEFENSE		2; // 공성 수비 그룹

#define DEF_MOB_ID_DRATAN_TOWER		352
#define DEF_MOB_ID_MASTER_TOWER		399

// Raid Output Type과 Raid Object Type은 Number가 겹쳐서는 안된다.
// Raid Output Type
// #define __TOUCH_FIELD			0
// #define PAUSE_AREA				12
// #define TOGGLE_GATE				7
// #define MODEL_HOLDER_PADOX		17
// #define MODEL_NPC_SAY			18				// forgotten temple [5/23/2011 rumist]
// #define TOGGLE_PORTAL			8
// Raid Object Type
// #define OBJECT_TOUCH_FIELD		0
// #define OBJECT_GATE				1
// #define OBJECT_PORTAL			2
// #define OBJECT_ITEM				3
// #define OBJECT_PADOX_MODEL		4
%}
	
uses "EntitiesMP/AnimationChanger";
	
enum PlayerViewType {
	0 PVT_PLAYEREYES      "",
		1 PVT_PLAYERAUTOVIEW  "",
		2 PVT_SCENECAMERA     "",
		3 PVT_3RDPERSONVIEW   "",
};

enum PlayerState {
	0 PST_STAND     "",
//		1 PST_CROUCH    "",
		2 PST_SWIM      "",
		3 PST_DIVE      "",
		4 PST_FALL      "",
		5 PST_BEFORECLIMB	"",
		6 PST_CLIMBWALL		"",
		7 PST_AFTERCLIMB	"",
};

enum PlayerTeleportCause {  
	0 TPC_FIRSTINIT     "",
		1 TPC_WORLDCHANGED  "",
		2 TPC_OTHER         "",
};

enum NickNameEffectType {
	0 NICKNAME_ATTACK_EFFECT      "",
		1 NICKNAME_DAMAGE_EFFECT  "",
};

// event for starting cinematic camera sequence
event ECameraStart {
	CEntityPointer penCamera,   // the camera
};

// event for ending cinematic camera sequence
event ECameraStop {
	CEntityPointer penCamera,   // the camera
};

// 소환수가 뭔가를 죽였을때, 이벤트를 발생시키도록 함.
event ESlaveKilledEnemy {
};

event EAutoLogin { // 웹로그인 처리를 위한 자동 로그인 이벤트

};

//강동민 수정 시작 클로즈 준비 작업	08.10
// NOTE : 카메라 위치가 보간되서 움직이는 것을 정지합니다.
// NOTE : 로그인시 월드 보여주는 카메라에서 필요함.
event EStopLerp 
{
	BOOL bActive,
};
//강동민 수정 끝 클로즈 준비 작업		08.10

// sent when needs to rebirth
event ERebirth {
};

// sent over network
event EPlayerRebirth {
	INDEX iRespawnInPlace,
};

// sent when player was disconnected from game
event EDisconnected {
};

// starts automatic player actions
event EAutoAction {
	CEntityID eidFirstMarker,
};

event EPlayerKilledEnemy { // sent to player when enemy is killed
	CEntityID eidEnemy,  // ID of enemy that was killed  
		INDEX iScore,    // score that was sent to the killer
		enum CauseOfDeath eCauseOfDeath,
		enum KillType  eKillType,
};

event EPlayerDeath {
	CEntityID eidInflictor,    // entity that inflicted the damage
		FLOAT3D vDirection,             // where the damage came from (in absolute space)
		FLOAT3D vHitPoint,              // where the damage hit the entity (in absolute space)
		FLOAT fAmount,                  // amount of damage done
		enum DamageType dmtType,        // type of damage
};

event EShowConsole {
};

event EPlayerStats {
	INDEX iIndex,
		INDEX iFrags,
		INDEX iKills,
		INDEX iDeaths,
};

event EAddBulletSpray {
	FLOAT3D vPos,
		EffectParticlesType eptType,
		FLOAT3D vReflected,
};

event EAddGoreSpray {
	INDEX   iEntityHit,
		FLOAT3D vPos,
		FLOAT3D vHit,
		SprayParticlesType sptType,
		COLOR   colParticles,
};

%{
// ----------------------------------------------------------------<<
INDEX	m_timeCnt = 0; // 락체크(3초후 락을 풀어줌) wooss 060515
UBYTE	m_checkLock =0;
// ----------------------------------------------------------------->>
extern void HUD_DrawSkaModel(CDrawPort *pdpCurrent, HUD_MLData *hud_MLData,  FLOAT fMinX, FLOAT fMinY, FLOAT fMaxX, FLOAT fMaxY, FLOAT fDistance, FLOAT fHeight);
extern void DrawHUD( const CPlayer *penPlayerCurrent, CDrawPort *pdpCurrent, BOOL bSnooping, const CPlayer *penPlayerOwner);
extern void InitHUD(void);
extern void EndHUD(void);
extern void CameraEffect(const CPlacement3D &plPlr, CPlacement3D &plCam);

static CTimerValue _tvProbingLast;
static BOOL _bTripleHead = FALSE;
static BOOL _bSeparatedTripleHead = FALSE;

// used to render certain entities only for certain players (like picked items, etc.)
extern ULONG _ulPlayerRenderingMask = 0;
extern FLOAT _fAttackLengthMul = 1.0f; //0.8이된다. 애니메이션 길이는 짧아짐.
extern FLOAT _fAttackSpeedMul =  1.0f; //1.2이 된다.
// temporary BOOL used to discard calculating of 3rd view when calculating absolute view placement
BOOL _bDiscard3rdView=FALSE;

//#define NAME name

const FLOAT _fBlowUpAmmount = 70.0f;

//강동민 수정 시작 싱글던젼 스토리	10.12
// 고정 시점 카메라.
static BOOL _bInFixedCamera	= FALSE;
//강동민 수정 끝 싱글던젼 스토리	10.12

//강동민 수정 시작 벽타기	10.21
//static BOOL _bOldClimbWall = FALSE;
//강동민 수정 끝 벽타기	10.21

static BOOL _bPersonalShop = FALSE;

extern ENGINE_API INDEX g_iAutoAttack;
extern ENGINE_API INDEX g_iCountry;

// computer message adding flags
#define CMF_READ       (1L<<0)
#define CMF_ANALYZE    (1L<<1)

struct MarkerDistance {
public:
	FLOAT md_fMinD;
	CPlayerMarker *md_ppm;
	void Clear(void);
};

/*
#define MAX_DEATH	(100)
struct	DeathTable {

	CEntity*  m_penDeathEntity;
	FLOAT	DeathTime;
	BOOL	bActive;
};
*/

//DeathTable _DeathTable[MAX_DEATH];

// export current player projection
CAnyProjection3D prPlayerProjection;

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
static INDEX g_iPCMinBrightness = 32;
static INDEX g_iChaLightModifyValue = 255;
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

int qsort_CompareMarkerDistance(const void *pv0, const void *pv1)
{
	MarkerDistance &md0 = *(MarkerDistance*)pv0;
	MarkerDistance &md1 = *(MarkerDistance*)pv1;
	if(      md0.md_fMinD<md1.md_fMinD) return +1;
	else if( md0.md_fMinD>md1.md_fMinD) return -1;
	else                                return  0;
}

static inline FLOAT IntensityAtDistance( FLOAT fFallOff, FLOAT fHotSpot, FLOAT fDistance)
{
	// intensity is zero if further than fall-off range
	if( fDistance>fFallOff) return 0.0f;
	// intensity is maximum if closer than hot-spot range
	if( fDistance<fHotSpot) return 1.0f;
	// interpolate if between fall-off and hot-spot range
	return (fFallOff-fDistance)/(fFallOff-fHotSpot);
}

static CTString MakeEmptyString(INDEX ctLen, char ch=' ')
{
	char ach[2];
	ach[0] = ch;
	ach[1] = 0;
	CTString strSpaces;
	for (INDEX i=0; i<ctLen; i++) {
		strSpaces+=ach;
	}
	return strSpaces;
}

// take a two line string and align into one line of minimum given length
static INDEX _ctAlignWidth = 20;
static CTString AlignString(const CTString &strOrg)
{
	// split into two lines
	CTString strL = strOrg;
	strL.OnlyFirstLine();
	CTString strR = strOrg;
	strR.RemovePrefix(strL);
	strR.DeleteChar(0);
	
	// get their lengths
	INDEX iLenL = strL.LengthNaked();
	INDEX iLenR = strR.LengthNaked();
	
	// find number of spaces to insert
	INDEX ctSpaces = _ctAlignWidth-(iLenL+iLenR);
	if (ctSpaces<1) {
		ctSpaces=1;
	}
	
	// make aligned string
	return strL+MakeEmptyString(ctSpaces)+strR;
}

static CTString CenterString(const CTString &str)
{
	INDEX ctSpaces = (_ctAlignWidth-str.LengthNaked())/2;
	if (ctSpaces<0) {
		ctSpaces=0;
	}
	return MakeEmptyString(ctSpaces)+str;
}

static CTString PadStringRight(const CTString &str, INDEX iLen)
{
	INDEX ctSpaces = iLen-str.LengthNaked();
	if (ctSpaces<0) {
		ctSpaces=0;
	}
	return str+MakeEmptyString(ctSpaces);
}

static CTString PadStringLeft(const CTString &str, INDEX iLen)
{
	INDEX ctSpaces = iLen-str.LengthNaked();
	if (ctSpaces<0) {
		ctSpaces=0;
	}
	return MakeEmptyString(ctSpaces)+str;
}

static void DumpEnemies(void)
{
	// get the world pointer
	CWorld *pwo = (CWorld *)_pShell->GetINDEX("pwoCurrentWorld");
	// if there is no current world
	if (pwo==NULL) return;
	// for each entity in the world
	{FOREACHINDYNAMICCONTAINER(pwo->wo_cenEntities, CEntity, iten) {
		CEntity *pen = iten;
		if( IsDerivedFromClass( pen, &CEnemyBase_DLLClass) && !((CEnemyBase*)pen)->m_bTemplate) {
			FLOAT3D vPos=pen->GetPlacement().pl_PositionVector;
			CPrintF("Class: %s, Name: %s, ID: %d, Coordinates(%d,%d,%d)\n", 
				pen->GetClass()->ec_pdecDLLClass->dec_strName, pen->GetName(),
				pen->en_ulID, (INDEX)vPos(1), (INDEX)vPos(2), (INDEX)vPos(3));
		}
	}}
}

static void DumpSpawners(void)
{
	// get the world pointer
	CWorld *pwo = (CWorld *)_pShell->GetINDEX("pwoCurrentWorld");
	// if there is no current world
	if (pwo==NULL) return;
	// for each entity in the world
	{FOREACHINDYNAMICCONTAINER(pwo->wo_cenEntities, CEntity, iten) {
		CEntity *pen = iten;
		if( IsDerivedFromClass( pen, &CEnemySpawner_DLLClass)) {
			CEnemySpawner *penSpawner = (CEnemySpawner *)pen;
			FLOAT3D vPos=pen->GetPlacement().pl_PositionVector;
			CTString strEnemyClass="Unknown";
			CTString strEnemyName="Unknown";
			if(penSpawner->m_penTarget!=NULL)
			{
				strEnemyClass=penSpawner->m_penTarget->GetClass()->ec_pdecDLLClass->dec_strName+CTString("'s");
				strEnemyName=penSpawner->m_penTarget->GetName();
			}
			CPrintF("Spawner: %s (ID=%d), Pos=%d,%d,%d Not spawned yet %d enemies of class \"%s\", named: \"%s\")\n", 
				pen->GetName(), pen->en_ulID, (INDEX)vPos(1), (INDEX)vPos(2), (INDEX)vPos(3),
				penSpawner->m_ctTotal, strEnemyClass, strEnemyName);
		}
	}}
}

static int HP_FloatToInt(float fCurHp, int Percentage)
{
	int nHp = 0;
	if (fCurHp > 0)
	{
		nHp = fCurHp * Percentage;

		if (nHp <= 0)
			nHp = 1;
	}
	return nHp;
}

static void GoTo(FLOAT x, FLOAT y, FLOAT z, FLOAT r)
{
	CPlayer *pen=(CPlayer *)CEntity::GetPlayerEntity(0);
	if(pen!=NULL)
	{
		CPlacement3D plWhere=CPlacement3D(FLOAT3D(x,y,z),ANGLE3D(r,0,0));
		pen->GetPlayerWeapons()->m_penReservedTarget	= NULL;
		pen->GetPlayerWeapons()->m_penRayHitTmp			= NULL;
		pen->Teleport(plWhere, FALSE);
		((CMovableEntity*)pen)->m_bFallDownToFloor = TRUE;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 컨테이너 내의 모든 엔티티에 한번에 데미지를 줍니다.
// Input  : pEntity - 
//			sTI - 
//			vCenter - 
// Output : static void
//-----------------------------------------------------------------------------
void DamagedTargetsInRange(CEntity* pEntity, CSelectedEntities& dcEntities, enum DamageType dmtType, FLOAT fDamageAmmount, FLOAT3D vCenter, BOOL bSkill)
{	
	if(dcEntities.Count() <= 0)	return;

	for( ENTITIES_ITERATOR it = dcEntities.vectorSelectedEntities.begin();
		it != dcEntities.vectorSelectedEntities.end(); ++it )
	{
		if ((*it) == NULL)
		{
			continue;
		}

		CEntity &en = *(*it);
		
		if (en.en_RenderType != CEntity::RT_SKAMODEL || !(en.GetFlags() & ENF_ALIVE))
		{
			continue;
		}

		//CEntity *pEn = iten;
		FLOAT3D vHitPos;
		
		CModelInstance *pMI = en.GetModelInstance();
		if(!pMI)
		{
			continue;
		}

		//pEntity->InflictDirectDamage( (*it), pEntity, DMT_EXPLOSION,
		//	15.0f, vHitPos, (vHitPos-vCenter).Normalize() );

		if( _pNetwork->m_bSingleMode )
		{				
			// 10번 월드의 경우...
			if( g_slZone == EXTREAM_SINGLEDUNGEON )
			{
				//pEntity->CheckModelRangeDamage(en, vCenter, fMinD, vHitPos);
				pEntity->InflictDirectDamage( (*it), pEntity, dmtType, fDamageAmmount, vCenter, vCenter.Normalize() );
			}
			// 기타 다른 월드의 경우....
			else
			{
				pEntity->InflictDirectDamage( (*it), pEntity, dmtType, bSkill ? 30.0f : 15.0f, vCenter, vCenter.Normalize() );
			}
		}
		else
		{
			pEntity->InflictDirectDamage( (*it), pEntity, dmtType, fDamageAmmount, vCenter, vCenter.Normalize() );
		}
	}
};

#define HEADING_MAX      45.0f
#define PITCH_MAX        85.0f
#define BANKING_MAX      45.0f

// player flags
#define PLF_INITIALIZED           (1UL<<0)   // set when player entity is ready to function
#define PLF_VIEWROTATIONCHANGED   (1UL<<1)   // for adjusting view rotation separately from legs
#define PLF_JUMPALLOWED           (1UL<<2)   // if jumping is allowed
//#define PLF_SYNCWEAPON            (1UL<<3)   // weapon model needs to be synchronized before rendering
#define PLF_AUTOMOVEMENTS         (1UL<<4)   // complete automatic control of movements
#define PLF_DONTRENDER            (1UL<<5)   // don't render view (used at end of level)
#define PLF_CHANGINGLEVEL         (1UL<<6)   // mark that we next are to appear at start of new level
#define PLF_APPLIEDACTION         (1UL<<7)   // used to detect when player is not connected
#define PLF_NOTCONNECTED          (1UL<<8)   // set if the player is not connected
#define PLF_LEVELSTARTED          (1UL<<9)   // marks that level start time was recorded
#define PLF_ISZOOMING             (1UL<<10)  // marks that player is zoomed in with the sniper
#define PLF_RESPAWNINPLACE        (1UL<<11)  // don't move to marker when respawning (for current death only)
#define PLF_REAPPLYINGACTIONS     (1UL<<12)  // mark that we're reapplying actions (client local prediction)  

// defines representing flags used to fill player buttoned actions
#define PLACT_FIRE                (1L<<0)
//#define PLACT_RELOAD              (1L<<1)
//#define PLACT_WEAPON_NEXT         (1L<<2)
//#define PLACT_WEAPON_PREV         (1L<<3)
//#define PLACT_WEAPON_FLIP         (1L<<4)

// FIXME : 어느 순간부터 PLACT_USE 쓰는 부분이 다 막혀있음, 그래서 카메라 마커 동작중에 안먹는듯함...
#define PLACT_USE                 (1L<<5)
#define PLACT_3RD_PERSON_VIEW     (1L<<7)
#define PLACT_CENTER_VIEW         (1L<<8)
#define PLACT_USE_HELD            (1L<<9)
#define PLACT_SELECT_WEAPON_SHIFT (14)
#define PLACT_SELECT_WEAPON_MASK  (0x1FL<<PLACT_SELECT_WEAPON_SHIFT)

#define PICKEDREPORT_TIME   (2.0f)  // how long (picked-up) message stays on screen
// KALYDO
/*
struct PlayerControls 
{
	FLOAT fMoveForward;
	FLOAT fMoveBackward;
	FLOAT fMoveLeft;
	FLOAT fMoveRight;
	FLOAT fMoveUp;
	FLOAT fMoveDown;
	
	FLOAT fTurnLeft;
	FLOAT fTurnRight;
	FLOAT fTurnUp;
	FLOAT fTurnDown;
	FLOAT fTurnBankingLeft;
	FLOAT fTurnBankingRight;	
	
	FLOAT fLookLeft;
	FLOAT fLookRight;
	FLOAT fLookUp;
	FLOAT fLookDown;
	FLOAT fLookBankingLeft;
	FLOAT fLookBankingRight;
	
#define CTL_DIGITAL_FIRST bCenterView
	
	BOOL bCenterView;
	//BOOL bSelectWeapon[MAX_WEAPONS+1];
	//BOOL bWeaponNext;
	//BOOL bWeaponPrev;
	//BOOL bWeaponFlip;
	
	BOOL bWalk;
	BOOL bStrafe;
	BOOL bFire;	
	BOOL bUse;
	BOOL b3rdPersonView;	
};
*/

// KALYDO
//static struct PlayerControls pctlCurrent;


// cheats
static INDEX cht_iGoToMarker = -1;
static INDEX cht_bKillAll    = FALSE;
static INDEX cht_bKillAllAuto = FALSE;
static INDEX cht_bKillAllInLevel = FALSE;
static INDEX cht_bStartDebugTrigger = FALSE;
static INDEX cht_bGiveAll    = FALSE;
static INDEX cht_bOpen       = FALSE;
static INDEX cht_bAllMessages= FALSE;
static INDEX cht_bRefresh    = FALSE;
extern INDEX cht_bGod        = FALSE;
extern INDEX cht_bFly        = FALSE;
extern INDEX cht_bGhost      = FALSE;
extern INDEX cht_bInvisible  = FALSE;
extern FLOAT cht_fTranslationMultiplier = 1.0f;
extern INDEX cht_bEnable     = 0;   

// interface control
static INDEX hud_bShowAll	    = TRUE; // used internaly in menu/console
//extern INDEX hud_bShowWeapon  = TRUE;
//extern INDEX hud_bShowMessages = TRUE;
extern INDEX hud_bShowInfo    = TRUE;
extern INDEX hud_bShowLatency = FALSE;
extern INDEX hud_iShowPlayers = -1;   // auto
extern INDEX hud_iSortPlayers = -1;   // auto
extern FLOAT hud_fOpacity     = 0.9f;
extern FLOAT hud_fScaling     = 1.0f;
//extern FLOAT hud_tmWeaponsOnScreen = 3.0f;
extern FLOAT hud_tmLatencySnapshot = 1.0f;
//extern INDEX hud_bShowMatchInfo = TRUE;
//extern INDEX hud_bShowFloatingScore = TRUE;

extern FLOAT plr_fBreathingStrength = 0.0f;
extern FLOAT plr_tmSnoopingTime;
//extern INDEX cht_bKillFinalBoss = FALSE;
//INDEX cht_bDebugFinalBoss = FALSE;
//INDEX cht_bDumpFinalBossData = FALSE;
//INDEX cht_bDebugFinalBossAnimations = FALSE;
INDEX cht_bDumpPlayerShading = FALSE;

// misc
static FLOAT plr_fAcceleration  = 1000.0f;
static FLOAT plr_fDeceleration  = 600.0f;
/*static FLOAT plr_fSpeedForward  = 10.0f;
static FLOAT plr_fSpeedBackward = 10.0f;
static FLOAT plr_fSpeedSide     = 10.0f;*/
//0105 1line 삭제
//static FLOAT plr_fSpeed         = 10.0f;
//0105
static FLOAT plr_fSpeed         = 5.0f;
static FLOAT plr_fSpeedTmp         = 0.0f;
//..
static FLOAT plr_fSpeedUp       = 11.0f;
static FLOAT plr_fViewHeightStand  = 1.9f;
//static FLOAT plr_fViewHeightCrouch = 0.7f;
static FLOAT plr_fViewHeightSwim   = 0.4f;
static FLOAT plr_fViewHeightDive   = 0.0f;
extern FLOAT plr_fViewDampFactor        = 0.4f;
extern FLOAT plr_fViewDampLimitGroundUp = 0.1f;
extern FLOAT plr_fViewDampLimitGroundDn = 0.4f;
extern FLOAT plr_fViewDampLimitWater    = 0.1f;
static FLOAT plr_fFrontClipDistance = 0.25f;
static FLOAT plr_fFarClipDistance = 500.0f;
static FLOAT plr_fFOV = 90.0f;
static FLOAT gfx_fTripleHeadFOVBias = 1.4f;  // not too much

static FLOAT net_tmLatencyAvg;
extern INDEX plr_bRenderPicked = FALSE;
extern INDEX plr_bRenderPickedParticles = FALSE;
extern INDEX plr_bOnlySam = FALSE;
extern INDEX ent_bReportBrokenChains = FALSE;
extern FLOAT ent_tmMentalIn   = 0.5f;
extern FLOAT ent_tmMentalOut  = 0.75f;
extern FLOAT ent_tmMentalFade = 0.5f;
static INDEX plr_bRespawnInPlaceCoop = TRUE;

extern FLOAT gfx_fEnvParticlesDensity = 1.0f;
extern FLOAT gfx_fEnvParticlesRange   = 1.0f;

static FLOAT plr_fSwimSoundDelay = 0.8f;
static FLOAT plr_fDiveSoundDelay = 1.6f;
static FLOAT plr_fWalkSoundDelay = 0.6f;//0.5f; //0323
static FLOAT plr_fRunSoundDelay  = 0.415f;//0.35f;//0.3f;

//static FLOAT ctl_tmComputerDoubleClick = 0.5f; // double click delay for calling computer
//static FLOAT _tmLastUseOrCompPressed = -10.0f;  // for computer doubleclick

extern INDEX gam_iBlood = 2;     // 0=none, 1=green, 2=red, 3=hippie
extern INDEX gam_bGibs  = TRUE;

// speeds for button rotation
static FLOAT ctl_fButtonRotationSpeedH = 300.0f;
static FLOAT ctl_fButtonRotationSpeedP = 150.0f;
static FLOAT ctl_fButtonRotationSpeedB = 150.0f;
// modifier for axis strafing
static FLOAT ctl_fAxisStrafingModifier = 1.0f;

static BOOL plr_bSetRespawn = FALSE;

//0530 kwon 추가
static unsigned int tmStartTime = 0;

//0625 kwon
static ANGLE aWantedHeadingRelativeTmp = 0;
static BOOL g_bIsRotating = FALSE;
static FLOAT3D g_vMove = FLOAT3D(0,0,0);
static FLOAT3D g_vMoveDirection = FLOAT3D(0,0,0);
static BOOL g_bIsRotateEnd = FALSE;
static BOOL g_bFirstRotate = FALSE;
static FLOAT3D g_bTargetDirection = FLOAT3D(0,0,0);

//0722
static BOOL g_bPreSkill  = FALSE; 
static BOOL g_bDoSkill   = FALSE;
static BOOL g_bPostSkill = FALSE;
static FLOAT g_deltaX = 0;
static FLOAT g_deltaZ = 0;

extern INDEX dbg_bShowEnemyBodyPoint = FALSE;

extern ENGINE_API BOOL g_bHead_change; // 일본 헬멧 추가 관련 

// set when computer is called on level start
DECL_DLL extern BOOL cmp_bLevelStart = FALSE;
// set to play video before and after end of level
//DECL_DLL extern CTString cmp_strVideo1 = "";
//DECL_DLL extern CTString cmp_strVideo2 = "";
//DECL_DLL extern CTString cmp_strVideoInGame = "";

// !=NULL if some player wants to call computer
DECL_DLL extern class CPlayer *cmp_ppenPlayer = NULL;
// !=NULL for rendering computer on secondary display in dualhead
DECL_DLL extern class CPlayer *cmp_ppenDHPlayer = NULL;
// set to update current message in background mode (for dualhead)
DECL_DLL extern BOOL cmp_bUpdateInBackground = FALSE;
// set for initial calling computer without rendering game
DECL_DLL extern BOOL cmp_bInitialStart = FALSE;

// game sets this for player hud and statistics and hiscore sound playing
//DECL_DLL extern INDEX plr_iHiScore = 0.0f;

// these define address and size of player controls structure
DECL_DLL extern void *ctl_pvPlayerControls = &pctlCurrent;
DECL_DLL extern const SLONG ctl_slPlayerControlsSize = sizeof(pctlCurrent);

// called to compose action packet from current controls
DECL_DLL void ctl_ComposeActionPacket(const CPlayerCharacter &pc, CPlayerAction &paAction, BOOL bPreScan)
{
//	return;//0114

	CPlayerSettings *pps = (CPlayerSettings *)pc.pc_aubAppearance;
	
	// find local player, if any
	CPlayer *penThis = (CPlayer *)CEntity::GetPlayerEntity(pc.pc_iPlayerIndex);
	
	// if not found
	if (penThis==NULL) {
		// do nothing
		return;
	}
	
	// accumulate local rotation
	FLOAT3D &vRotation = penThis->m_aLocalRotation;
	FLOAT3D &vViewRotation = penThis->m_aLocalViewRotation;
	FLOAT3D &vTranslation  = penThis->m_vLocalTranslation;
//	FLOAT3D vHeadingforDir = GetHeadingDirection(penThis->m_aHeadingRotation);
	
	//  CPrintF("compose: prescan %d, x:%g\n", bPreScan, paAction.pa_aRotation(1));

/*	if (_pInput->GetInputDevice(1)->id_aicControls[KID_W].ic_fValue > 0.0f)
	{
		pctlCurrent.fMoveForward = _pInput->GetInputDevice(1)->id_aicControls[KID_W].ic_fValue;
	}

	if (_pInput->GetInputDevice(1)->id_aicControls[KID_S].ic_fValue > 0.0f)
	{
		pctlCurrent.fMoveBackward = _pInput->GetInputDevice(1)->id_aicControls[KID_S].ic_fValue;
	}

	if (_pInput->GetInputDevice(1)->id_aicControls[KID_A].ic_fValue > 0.0f)
	{
		pctlCurrent.fTurnLeft = _pInput->GetInputDevice(1)->id_aicControls[KID_A].ic_fValue;
	}

	if (_pInput->GetInputDevice(1)->id_aicControls[KID_D].ic_fValue > 0.0f)
	{
		pctlCurrent.fTurnRight = _pInput->GetInputDevice(1)->id_aicControls[KID_D].ic_fValue;
	}
*/
	// 	if (_pInput->GetInputDevice(1)->id_aicControls[KID_Q].ic_fValue > 0.0f)
// 	{
// 		pctlCurrent.fMoveLeft = _pInput->GetInputDevice(1)->id_aicControls[KID_Q].ic_fValue;
// 	}
// // 	if (_pInput->GetInputDevice(1)->id_aicControls[KID_E].ic_fValue > 0.0f)
// 	{
// 		pctlCurrent.fMoveRight = _pInput->GetInputDevice(1)->id_aicControls[KID_E].ic_fValue;
// 	}
	FLOAT fMoveLeft  = pctlCurrent.fMoveLeft;
	FLOAT fMoveRight = pctlCurrent.fMoveRight;
	if(pctlCurrent.bStrafe&&pctlCurrent.fTurnLeft>0.0f) 
	{
		fMoveLeft += pctlCurrent.fTurnLeft;
	}

	if(pctlCurrent.bStrafe&&pctlCurrent.fTurnRight>0.0f) 
	{
		fMoveRight += pctlCurrent.fTurnRight;
	}
	
	// no analogue controls on move up/down
	if (pctlCurrent.fMoveDown>0.0f) pctlCurrent.fMoveDown = 1.0f;
	//if (pctlCurrent.fMoveUp  >0.0f) pctlCurrent.fMoveUp   = 1.0f;	// 점프
	//pctlCurrent.fMoveUp   = 0.0f;	// 점프
	pctlCurrent.fMoveUp   = 0.0f;	// 점프

/*
	// NOTE : 제한 영역내에서만 점프하도록 하는 루틴임.
	//--------------------------------------------------------------------------
	if(!_bLoginProcess)
	{
		// 제한 영역내에 플레이어가 들어가있다면...
		CRestrictedField *pField = FindPlayerInRestrictedField(penThis);
		if(pField)
		{
			if (pctlCurrent.fMoveUp  >0.0f) pctlCurrent.fMoveUp   = 1.0f;	// 점프
			CPrintF("제한영역 내에 들어와있습니다.!\n");
		}
		else
		{
			pctlCurrent.fMoveUp   = 0.0f;	// 점프
		}
	}
	else
	{
		pctlCurrent.fMoveUp   = 0.0f;	// 점프
	}
	*/
	//--------------------------------------------------------------------------
	
	vTranslation(1) -= fMoveLeft  * plr_fSpeed;
	vTranslation(1) += fMoveRight * plr_fSpeed;
	vTranslation(2) -= pctlCurrent.fMoveDown * plr_fSpeedUp;
	vTranslation(2) += pctlCurrent.fMoveUp   * plr_fSpeedUp;
	vTranslation(3) -= pctlCurrent.fMoveForward  * plr_fSpeed;
	vTranslation(3) += pctlCurrent.fMoveBackward * plr_fSpeed;
	//0212
	//  CPrintF(TRANS("plr_fSpeed : %f\n"), plr_fSpeed);
	
	FLOAT fSensitivityH = ((FLOAT)(pps->ps_iSensitivity)/127.0f)*0.75f;
	FLOAT fSensitivityP = ((FLOAT)(pps->ps_iSensitivity)/127.0f)*0.50f;
	
	// if not strafing
	if(!pctlCurrent.bStrafe) 
	{
/*		if (pctlCurrent.fTurnLeft != 0.0f || pctlCurrent.fTurnRight != 0.0f)
		{
			CPrintF(TRANS("Left Turn : %f, Right Turn : %f\n"), pctlCurrent.fTurnLeft, pctlCurrent.fTurnRight);
		}*/
		vRotation(1)+= pctlCurrent.fTurnLeft*(1.0f+fSensitivityH);
		vRotation(1)-= pctlCurrent.fTurnRight*(1.0f+fSensitivityH);

		SE_Get_UIManagerPtr()->GetCreateChar()->SetCharaterAngle(pctlCurrent.fTurnLeft*(1.0f+fSensitivityH),pctlCurrent.fTurnRight*(1.0f+fSensitivityH));
	}
	
	BOOL bInvertLook = pps->ps_ulFlags&PSF_INVERTLOOK;
	FLOAT fInvert = bInvertLook ? -1.0f : 1.0f;      
	
	vRotation(2)+= fInvert*(pctlCurrent.fTurnUp  *(1.0f+fSensitivityP));
	vRotation(2)-= fInvert*(pctlCurrent.fTurnDown*(1.0f+fSensitivityP));
	vRotation(3)+= pctlCurrent.fTurnBankingLeft ;
	vRotation(3)-= pctlCurrent.fTurnBankingRight;
	
	memset(&pctlCurrent, 0, offsetof(PlayerControls, CTL_DIGITAL_FIRST));
	
	paAction.pa_vTranslation  = penThis->m_vLocalTranslation;
	penThis->m_vLocalTranslation  = FLOAT3D(0,0,0);
	
	// if prescanning
	if (bPreScan) 
	{
		// no button checking
		return;
	}
	
	// use current accumulated rotation
	paAction.pa_aRotation     = penThis->m_aLocalRotation;
	paAction.pa_aViewRotation = penThis->m_aLocalViewRotation;
	
	// reset all button actions
	paAction.pa_ulButtons = 0;
	
	/*
	// set weapon selection bits
	for(INDEX i=1; i<MAX_WEAPONS; i++) 
	{
		if(pctlCurrent.bSelectWeapon[i]) 
		{
			paAction.pa_ulButtons = i<<PLACT_SELECT_WEAPON_SHIFT;
			break;
		}
	}
	*/
	// set button pressed flags
	//if(pctlCurrent.bWeaponNext) paAction.pa_ulButtons |= PLACT_WEAPON_NEXT;
	//if(pctlCurrent.bWeaponPrev) paAction.pa_ulButtons |= PLACT_WEAPON_PREV;
	//if(pctlCurrent.bWeaponFlip) paAction.pa_ulButtons |= PLACT_WEAPON_FLIP;
//	if(pctlCurrent.bFire)       paAction.pa_ulButtons |= PLACT_FIRE;//0114
	//if(pctlCurrent.bReload)     paAction.pa_ulButtons |= PLACT_RELOAD;
//	if(pctlCurrent.bUse)        paAction.pa_ulButtons |= PLACT_USE|PLACT_USE_HELD|PLACT_SNIPER_USE;
	//if(pctlCurrent.bComputer)      paAction.pa_ulButtons |= PLACT_COMPUTER;
	if(pctlCurrent.b3rdPersonView) paAction.pa_ulButtons |= PLACT_3RD_PERSON_VIEW;
	if(pctlCurrent.bCenterView)    paAction.pa_ulButtons |= PLACT_CENTER_VIEW;
	// is 'use' being held?
	//if(pctlCurrent.bUseOrComputer) paAction.pa_ulButtons |= PLACT_USE_HELD|PLACT_SNIPER_USE;

	// FIXME : Sniper들어간 부분 모두 제거하기.
	//if(pctlCurrent.bSniperZoomIn)  paAction.pa_ulButtons |= PLACT_SNIPER_ZOOMIN;
	//if(pctlCurrent.bSniperZoomOut) paAction.pa_ulButtons |= PLACT_SNIPER_ZOOMOUT;
	//if(pctlCurrent.bFireBomb)      paAction.pa_ulButtons |= PLACT_FIREBOMB;
	
	// if userorcomp just pressed
	/*
	if(pctlCurrent.bUseOrComputer && !pctlCurrent.bUseOrComputerLast) 
	{
		// if double-click is off
		if (ctl_tmComputerDoubleClick==0 || (pps->ps_ulFlags&PSF_COMPSINGLECLICK)) 
		{
			// press both
			paAction.pa_ulButtons |= PLACT_USE|PLACT_COMPUTER;
			// if double-click is on
		} 
		else 
		{
			// if double click
			if (_pTimer->GetRealTimeTick()<=_tmLastUseOrCompPressed+ctl_tmComputerDoubleClick) 
			{
				// computer pressed
				paAction.pa_ulButtons |= PLACT_COMPUTER;
				// if single click
			} 
			else 
			{
				// use pressed
				paAction.pa_ulButtons |= PLACT_USE;
			}
		}
		_tmLastUseOrCompPressed = _pTimer->GetRealTimeTick();
	}
	// remember old userorcomp pressed state
	pctlCurrent.bUseOrComputerLast = pctlCurrent.bUseOrComputer;
	*/
};

extern UINT g_uiEntityVersion;
extern ENGINE_API INDEX g_bTestClient;
extern ENGINE_API HWND  _hwndMain;
ENGINE_API char *g_szExitError;


void CPlayer_Precache(void)
{
	CDLLEntityClass *pdec = &CPlayer_DLLClass;
	
	// precache view
	extern void CPlayerView_Precache(void);
	CPlayerView_Precache();

	pdec->PrecacheSound( SOUND_DEFAULT );

	// Bsp
	pdec->PrecacheSound( SOUND_WALK_MARBLE_IN_L );
	pdec->PrecacheSound( SOUND_WALK_MARBLE_IN_R );
	pdec->PrecacheSound( SOUND_WALK_BLOCK_OUT_L );
	pdec->PrecacheSound( SOUND_WALK_BLOCK_OUT_R );
	pdec->PrecacheSound( SOUND_WALK_WOOD_OUT_L );
	pdec->PrecacheSound( SOUND_WALK_WOOD_OUT_R );

	// Terrain
	pdec->PrecacheSound( SOUND_TER_WALK_DIRT_L );
	pdec->PrecacheSound( SOUND_TER_WALK_DIRT_R );
	pdec->PrecacheSound( SOUND_TER_WALK_GRASS_L );
	pdec->PrecacheSound( SOUND_TER_WALK_GRASS_R );
	pdec->PrecacheSound( SOUND_TER_WALK_ROCK_L );
	pdec->PrecacheSound( SOUND_TER_WALK_ROCK_R );
	pdec->PrecacheSound( SOUND_TER_WALK_BLOCK_L );
	pdec->PrecacheSound( SOUND_TER_WALK_BLOCK_R );
	pdec->PrecacheSound( SOUND_WATER_WALK_SHALLOW_L );
	pdec->PrecacheSound( SOUND_WATER_WALK_SHALLOW_R );

	// Effect
	pdec->PrecacheSound( SOUND_HEALER_HEAL_PRE );
	pdec->PrecacheSound( SOUND_HEALER_HEAL_DO );
	pdec->PrecacheSound( SOUND_HEALER_PARTY_HEAL_DO );
	pdec->PrecacheSound( SOUND_HEALER_DEFENCE_MANA_DO );

	// 강신체 사운드.
	pdec->PrecacheSound( SOUND_WALK_ELENEN_WALK		);
	pdec->PrecacheSound( SOUND_WALK_ELENEN_RUN		);
	pdec->PrecacheSound( SOUND_WALK_HELL_WALK		);
	pdec->PrecacheSound( SOUND_WALK_HELL_RUN		);

	pdec->PrecacheSound( SOUND_WALK_HORSE_WALK		);
	pdec->PrecacheSound( SOUND_WALK_HORSE_RUN		);
	pdec->PrecacheSound( SOUND_WALK_DRAGON_WALK		);
	//pdec->PrecacheSound( SOUND_WALK_DRAGON_RUN		);

	pdec->PrecacheSound( SOUND_HEALER_DAMAGE );
	pdec->PrecacheSound( SOUND_MAGE_DAMAGE );
	pdec->PrecacheSound( SOUND_TITAN_DAMAGE );
	pdec->PrecacheSound( SOUND_KNIGHT_DAMAGE );
	pdec->PrecacheSound( SOUND_ROGUE_DAMAGE );
	pdec->PrecacheSound( SOUND_SORCERER_DAMAGE );

	pdec->PrecacheSound( SOUND_HEALER_ATTACK );
	pdec->PrecacheSound( SOUND_MAGE_ATTACK );
	pdec->PrecacheSound( SOUND_TITAN_ATTACK );
	pdec->PrecacheSound( SOUND_KNIGHT_ATTACK );
	pdec->PrecacheSound( SOUND_ROGUE_ATTACK );
	pdec->PrecacheSound( SOUND_SORCERER_ATTACK );

	pdec->PrecacheSound( SOUND_SWORD_BLOW );
	pdec->PrecacheSound( SOUND_SWORD_CRITICAL );
	pdec->PrecacheSound( SOUND_BIGSWORD_BLOW  );
	pdec->PrecacheSound( SOUND_BIGSWORD_CRITICAL );
	pdec->PrecacheSound( SOUND_DROP_ITEM		);
	pdec->PrecacheSound( SOUND_DROP_MONEY		);
	pdec->PrecacheSound( SOUND_PICK_ITEM		);
	pdec->PrecacheSound( SOUND_BUTTON_CLICK		);

	pdec->PrecacheSound(  SOUND_POTION  );
	pdec->PrecacheSound(  SOUND_CRAK  );
	//pdec->PrecacheSound(  SOUND_MINE  );
	pdec->PrecacheSound(  SOUND_PROCESS	);
	pdec->PrecacheSound(  SOUND_PRODUCT	);
	pdec->PrecacheSound(  SOUND_POLYMOPH );

	pdec->PrecacheSound(  SOUND_DAGGER_BLOW);
	pdec->PrecacheSound(  SOUND_DAGGER_CRITICAL);
	pdec->PrecacheSound(  SOUND_FOOT_BLOW);
	pdec->PrecacheSound(  SOUND_FOOT_CRITICAL);
	pdec->PrecacheSound(  SOUND_AXE_BLOW  );
	pdec->PrecacheSound(  SOUND_AXE_CRITICAL );

	pdec->PrecacheSound( SOUND_DAIR_STRIKE_PRE);
	pdec->PrecacheSound( SOUND_DAIR_STRIKE_BLOW1);
	pdec->PrecacheSound( SOUND_DAIR_STRIKE_BLOW2);
	pdec->PrecacheSound( SOUND_DAIR_STRIKE_BLOW3);
	pdec->PrecacheSound( SOUND_A_DAIR_STRIKE_PRE);
	pdec->PrecacheSound( SOUND_A_DAIR_STRIKE_BLOW1);
	pdec->PrecacheSound( SOUND_A_DAIR_STRIKE_BLOW2);
	pdec->PrecacheSound( SOUND_A_DAIR_STRIKE_BLOW3);
}

DECL_DLL void CheckEntityVersion()
{
	if( !((g_bTestClient && g_uiEntityVersion >= 10000) || (!g_bTestClient && g_uiEntityVersion < 10000)) )
	{
		if(g_szExitError) delete[] g_szExitError;
		g_szExitError = new char[4096];
		strcpy(g_szExitError, _S( 864, "버전 정보가 맞지 않습니다." ));
		_pGameState->Running() = FALSE;
		_pGameState->QuitScreen() = FALSE;
	}	
}

/*
//0825
void Init_DeathTable(void)
{
	for ( int i = 0; i < MAX_DEATH; ++i )
	{
		_DeathTable[i].m_penDeathEntity = NULL;
		_DeathTable[i].DeathTime		= 0.0f;
		_DeathTable[i].bActive			= FALSE;
	}
}
*/

void TestEffect()
{
	CEntity *penTarget = ((CPlayer*)CEntity::GetPlayerEntity(0))->GetPlayerWeapons()->m_penRayHitTmp;
	if(penTarget == NULL) return;
	if(penTarget->GetRenderType() != CEntity::RT_SKAMODEL) return;
	if(penTarget->GetModelInstance() == NULL) return;
	if(!penTarget->IsEnemy()) return;
	void ShotConnect(CEntity *penShoter, CEntity *penTarget, FLOAT fWaitTime
					 , const char *szHitEffectName, const char *szConnectEffectName);
	ShotConnect(CEntity::GetPlayerEntity(0), penTarget
		, 3
		, "KN_TN_Dlnk_Hit"
		, "KN_TN_Dlnk_ms"
		);
}

void CPlayer_OnInitClass(void)
{
	// clear current player controls
	memset(&pctlCurrent, 0, sizeof(pctlCurrent));
	_pShell->DeclareSymbol("void TestEffect();", TestEffect);
	_pShell->DeclareSymbol("persistent user INDEX g_iPCMinBrightness;", &g_iPCMinBrightness);
	_pShell->DeclareSymbol("persistent user INDEX g_iChaLightModifyValue;", &g_iChaLightModifyValue);
	_pShell->DeclareSymbol("persistent user INDEX gam_iBlood;", &gam_iBlood);
	_pShell->DeclareSymbol("persistent user INDEX gam_bGibs;",  &gam_bGibs);
	// declare player control variables
	_pShell->DeclareSymbol("user FLOAT ctl_fMoveForward;",  &pctlCurrent.fMoveForward);
	_pShell->DeclareSymbol("user FLOAT ctl_fMoveBackward;", &pctlCurrent.fMoveBackward);
	_pShell->DeclareSymbol("user FLOAT ctl_fMoveLeft;",     &pctlCurrent.fMoveLeft);
	_pShell->DeclareSymbol("user FLOAT ctl_fMoveRight;",    &pctlCurrent.fMoveRight);
	_pShell->DeclareSymbol("user FLOAT ctl_fMoveUp;",       &pctlCurrent.fMoveUp);
	_pShell->DeclareSymbol("user FLOAT ctl_fMoveDown;",     &pctlCurrent.fMoveDown);
	_pShell->DeclareSymbol("user FLOAT ctl_fTurnLeft;",         &pctlCurrent.fTurnLeft);
	_pShell->DeclareSymbol("user FLOAT ctl_fTurnRight;",        &pctlCurrent.fTurnRight);
	_pShell->DeclareSymbol("user FLOAT ctl_fTurnUp;",           &pctlCurrent.fTurnUp);
	_pShell->DeclareSymbol("user FLOAT ctl_fTurnDown;",         &pctlCurrent.fTurnDown);
	_pShell->DeclareSymbol("user FLOAT ctl_fTurnBankingLeft;",  &pctlCurrent.fTurnBankingLeft);
	_pShell->DeclareSymbol("user FLOAT ctl_fTurnBankingRight;", &pctlCurrent.fTurnBankingRight);
	_pShell->DeclareSymbol("user INDEX ctl_bCenterView;",       &pctlCurrent.bCenterView);
	_pShell->DeclareSymbol("user INDEX ctl_fLookLeft;",         &pctlCurrent.fLookLeft);
	_pShell->DeclareSymbol("user INDEX ctl_fLookRight;",        &pctlCurrent.fLookRight);
	_pShell->DeclareSymbol("user INDEX ctl_fLookUp;",           &pctlCurrent.fLookUp);
	_pShell->DeclareSymbol("user INDEX ctl_fLookDown;",         &pctlCurrent.fLookDown);
	_pShell->DeclareSymbol("user INDEX ctl_fLookBankingLeft;",  &pctlCurrent.fLookBankingLeft);
	_pShell->DeclareSymbol("user INDEX ctl_fLookBankingRight;", &pctlCurrent.fLookBankingRight );
	_pShell->DeclareSymbol("user INDEX ctl_bWalk;",           &pctlCurrent.bWalk);
	_pShell->DeclareSymbol("user INDEX ctl_bStrafe;",         &pctlCurrent.bStrafe);
	_pShell->DeclareSymbol("user INDEX ctl_bFire;",           &pctlCurrent.bFire);
	//_pShell->DeclareSymbol("user INDEX ctl_bReload;",         &pctlCurrent.bReload);
	_pShell->DeclareSymbol("user INDEX ctl_bUse;",            &pctlCurrent.bUse);
	//_pShell->DeclareSymbol("user INDEX ctl_bComputer;",       &pctlCurrent.bComputer);
	//_pShell->DeclareSymbol("user INDEX ctl_bUseOrComputer;",  &pctlCurrent.bUseOrComputer);
	_pShell->DeclareSymbol("user INDEX ctl_b3rdPersonView;",  &pctlCurrent.b3rdPersonView);
	//_pShell->DeclareSymbol("user INDEX ctl_bWeaponNext;",         &pctlCurrent.bWeaponNext);
	//_pShell->DeclareSymbol("user INDEX ctl_bWeaponPrev;",         &pctlCurrent.bWeaponPrev);
	//_pShell->DeclareSymbol("user INDEX ctl_bWeaponFlip;",         &pctlCurrent.bWeaponFlip);
	//_pShell->DeclareSymbol("user INDEX ctl_bSelectWeapon[30+1];", &pctlCurrent.bSelectWeapon);
	//_pShell->DeclareSymbol("persistent user FLOAT ctl_tmComputerDoubleClick;", &ctl_tmComputerDoubleClick);
	_pShell->DeclareSymbol("persistent user FLOAT ctl_fButtonRotationSpeedH;", &ctl_fButtonRotationSpeedH);
	_pShell->DeclareSymbol("persistent user FLOAT ctl_fButtonRotationSpeedP;", &ctl_fButtonRotationSpeedP);
	_pShell->DeclareSymbol("persistent user FLOAT ctl_fButtonRotationSpeedB;", &ctl_fButtonRotationSpeedB);
	_pShell->DeclareSymbol("persistent user FLOAT ctl_fAxisStrafingModifier;", &ctl_fAxisStrafingModifier);
	//new
	//_pShell->DeclareSymbol("user INDEX ctl_bSniperZoomIn;",         &pctlCurrent.bSniperZoomIn);
	//_pShell->DeclareSymbol("user INDEX ctl_bSniperZoomOut;",        &pctlCurrent.bSniperZoomOut);
	//_pShell->DeclareSymbol("user INDEX ctl_bFireBomb;",             &pctlCurrent.bFireBomb);
	
	_pShell->DeclareSymbol("user FLOAT plr_fSwimSoundDelay;", &plr_fSwimSoundDelay);
	_pShell->DeclareSymbol("user FLOAT plr_fDiveSoundDelay;", &plr_fDiveSoundDelay);
	_pShell->DeclareSymbol("user FLOAT plr_fWalkSoundDelay;", &plr_fWalkSoundDelay);
	_pShell->DeclareSymbol("user FLOAT plr_fRunSoundDelay;",  &plr_fRunSoundDelay);
	
	_pShell->DeclareSymbol("     INDEX hud_bShowAll;",     &hud_bShowAll);
	_pShell->DeclareSymbol("user INDEX hud_bShowInfo;",    &hud_bShowInfo);
	_pShell->DeclareSymbol("user const FLOAT net_tmLatencyAvg;", &net_tmLatencyAvg);
	_pShell->DeclareSymbol("persistent user INDEX hud_bShowLatency;", &hud_bShowLatency);
	_pShell->DeclareSymbol("persistent user INDEX hud_iShowPlayers;", &hud_iShowPlayers);
	_pShell->DeclareSymbol("persistent user INDEX hud_iSortPlayers;", &hud_iSortPlayers);
	//_pShell->DeclareSymbol("persistent user INDEX hud_bShowWeapon;",  &hud_bShowWeapon);
	//_pShell->DeclareSymbol("persistent user INDEX hud_bShowMessages;",&hud_bShowMessages);
	_pShell->DeclareSymbol("persistent user FLOAT hud_fScaling;",     &hud_fScaling);
	_pShell->DeclareSymbol("persistent user FLOAT hud_fOpacity;",     &hud_fOpacity);
//	_pShell->DeclareSymbol("persistent user INDEX hud_bShowFloatingScore;", &hud_bShowFloatingScore);  
	//_pShell->DeclareSymbol("persistent user FLOAT hud_tmWeaponsOnScreen;",  &hud_tmWeaponsOnScreen);
	_pShell->DeclareSymbol("persistent user FLOAT hud_tmLatencySnapshot;",  &hud_tmLatencySnapshot);
	_pShell->DeclareSymbol("persistent user FLOAT plr_fBreathingStrength;", &plr_fBreathingStrength);
//	_pShell->DeclareSymbol("INDEX cht_bKillFinalBoss;",  &cht_bKillFinalBoss);
//	_pShell->DeclareSymbol("INDEX cht_bDebugFinalBoss;", &cht_bDebugFinalBoss);
//	_pShell->DeclareSymbol("INDEX cht_bDumpFinalBossData;", &cht_bDumpFinalBossData);
//	_pShell->DeclareSymbol("INDEX cht_bDebugFinalBossAnimations;", &cht_bDebugFinalBossAnimations);
	_pShell->DeclareSymbol("INDEX cht_bDumpPlayerShading;", &cht_bDumpPlayerShading);
	//_pShell->DeclareSymbol("persistent user INDEX hud_bShowMatchInfo;", &hud_bShowMatchInfo);
	
	// cheats
	_pShell->DeclareSymbol("user INDEX cht_bGod;",       &cht_bGod);
	_pShell->DeclareSymbol("user INDEX cht_bFly;",       &cht_bFly);
	_pShell->DeclareSymbol("user INDEX cht_bGhost;",     &cht_bGhost);
	_pShell->DeclareSymbol("user INDEX cht_bInvisible;", &cht_bInvisible);
	_pShell->DeclareSymbol("user INDEX cht_bGiveAll;",   &cht_bGiveAll);
	_pShell->DeclareSymbol("user INDEX cht_bKillAll;",   &cht_bKillAll);
	_pShell->DeclareSymbol("user INDEX cht_bKillAllAuto;",   &cht_bKillAllAuto);
	_pShell->DeclareSymbol("user INDEX cht_bKillAllInLevel;",   &cht_bKillAllInLevel);
	_pShell->DeclareSymbol("user INDEX cht_bStartDebugTrigger;", &cht_bStartDebugTrigger);
	_pShell->DeclareSymbol("user INDEX cht_bOpen;",      &cht_bOpen);
	_pShell->DeclareSymbol("user INDEX cht_bAllMessages;", &cht_bAllMessages);
	_pShell->DeclareSymbol("user FLOAT cht_fTranslationMultiplier ;", &cht_fTranslationMultiplier);
	_pShell->DeclareSymbol("user INDEX cht_bRefresh;", &cht_bRefresh);
	// this one is masqueraded cheat enable variable
	_pShell->DeclareSymbol("INDEX cht_bEnable;", &cht_bEnable);
	
	// this cheat is always enabled
	_pShell->DeclareSymbol("user INDEX cht_iGoToMarker;", &cht_iGoToMarker);
	
	// debug variables
	_pShell->DeclareSymbol("void DumpEnemies(void);", &DumpEnemies);
	_pShell->DeclareSymbol("void DumpSpawners(void);", &DumpSpawners);
	//_pShell->DeclareSymbol("void DumpZombies(void);", &DumpZombies);
	_pShell->DeclareSymbol("void GoTo(INDEX, INDEX, INDEX);", &GoTo);
	_pShell->DeclareSymbol("user INDEX dbg_bEnemyKillTest;", &dbg_bEnemyKillTest);  
	_pShell->DeclareSymbol("persistent user INDEX dbg_bShowEnemyBodyPoint;", &dbg_bShowEnemyBodyPoint);  
	
	// player speed and view parameters, not declared except in internal build
#if 1
	_pShell->DeclareSymbol("user FLOAT plr_fViewHeightStand;", &plr_fViewHeightStand);
	//_pShell->DeclareSymbol("user FLOAT plr_fViewHeightCrouch;",&plr_fViewHeightCrouch);
	_pShell->DeclareSymbol("user FLOAT plr_fViewHeightSwim;",  &plr_fViewHeightSwim);
	_pShell->DeclareSymbol("user FLOAT plr_fViewHeightDive;",  &plr_fViewHeightDive);
	_pShell->DeclareSymbol("user FLOAT plr_fViewDampFactor;",         &plr_fViewDampFactor);
	_pShell->DeclareSymbol("user FLOAT plr_fViewDampLimitGroundUp;",  &plr_fViewDampLimitGroundUp);
	_pShell->DeclareSymbol("user FLOAT plr_fViewDampLimitGroundDn;",  &plr_fViewDampLimitGroundDn);
	_pShell->DeclareSymbol("user FLOAT plr_fViewDampLimitWater;",     &plr_fViewDampLimitWater);
	_pShell->DeclareSymbol("user FLOAT plr_fAcceleration;",  &plr_fAcceleration);
	_pShell->DeclareSymbol("user FLOAT plr_fDeceleration;",  &plr_fDeceleration);
	/*_pShell->DeclareSymbol("user FLOAT plr_fSpeedForward;",  &plr_fSpeedForward);
	_pShell->DeclareSymbol("user FLOAT plr_fSpeedBackward;", &plr_fSpeedBackward);
	_pShell->DeclareSymbol("user FLOAT plr_fSpeedSide;",     &plr_fSpeedSide);*/
	_pShell->DeclareSymbol("user FLOAT plr_fSpeed;",         &plr_fSpeed);
	_pShell->DeclareSymbol("user FLOAT plr_fSpeedUp;",       &plr_fSpeedUp);
#endif
	_pShell->DeclareSymbol("persistent user FLOAT plr_fFOV;", &plr_fFOV);
	_pShell->DeclareSymbol("persistent user FLOAT plr_fFrontClipDistance;", &plr_fFrontClipDistance);
	_pShell->DeclareSymbol("persistent user FLOAT plr_fFarClipDistance;", &plr_fFarClipDistance);
	_pShell->DeclareSymbol("persistent user FLOAT gfx_fTripleHeadFOVBias;", &gfx_fTripleHeadFOVBias);
	_pShell->DeclareSymbol("persistent user INDEX plr_bRenderPicked;", &plr_bRenderPicked);
	_pShell->DeclareSymbol("persistent user INDEX plr_bRenderPickedParticles;", &plr_bRenderPickedParticles);
	_pShell->DeclareSymbol("persistent user INDEX plr_bOnlySam;", &plr_bOnlySam);
	_pShell->DeclareSymbol("persistent user INDEX ent_bReportBrokenChains;", &ent_bReportBrokenChains);
	_pShell->DeclareSymbol("persistent user FLOAT ent_tmMentalIn  ;", &ent_tmMentalIn  );
	_pShell->DeclareSymbol("persistent user FLOAT ent_tmMentalOut ;", &ent_tmMentalOut );
	_pShell->DeclareSymbol("persistent user FLOAT ent_tmMentalFade;", &ent_tmMentalFade);
	_pShell->DeclareSymbol("persistent user FLOAT gfx_fEnvParticlesDensity;", &gfx_fEnvParticlesDensity);
	_pShell->DeclareSymbol("persistent user FLOAT gfx_fEnvParticlesRange;", &gfx_fEnvParticlesRange);
	_pShell->DeclareSymbol("user INDEX plr_bRespawnInPlaceCoop;", &plr_bRespawnInPlaceCoop);
	
	// player appearance interface
	_pShell->DeclareSymbol("INDEX SetPlayerAppearance(INDEX, INDEX, INDEX, INDEX);", (void*)&SetPlayerAppearance);
	//0105
	_pShell->DeclareSymbol("INDEX SetPlayerAppearanceSka(INDEX, INDEX, INDEX, INDEX);", (void*)&SetPlayerAppearanceSka);
	//..
	// call player weapons persistant variable initialization
	extern void CPlayerWeapons_Init(void);
	CPlayerWeapons_Init();
	
	// initialize HUD
	InitHUD();	
	
	//0825
//	Init_DeathTable();	

	// precache
	CPlayer_Precache();
}

// clean up
void CPlayer_OnEndClass(void)
{
	EndHUD();
}

FLOAT TopHealth(void)
{
	if (GetSP()->sp_gdGameDifficulty<=CSessionProperties::GD_EASY) {
		return 200;
	} else {
		return 100;
	}
}

// info structure
static EntityInfo eiPlayerGround = {
	EIBT_FLESH, 80.0f,
	0.0f, 1.7f, 0.0f,     // source (eyes)
	0.0f, 1.0f, 0.0f,     // target (body)
};
/*
static EntityInfo eiPlayerCrouch = {
	EIBT_FLESH, 80.0f,
	0.0f, 1.2f, 0.0f,     // source (eyes)
	0.0f, 0.7f, 0.0f,     // target (body)
};
*/
static EntityInfo eiPlayerSwim = {
	EIBT_FLESH, 40.0f,
	0.0f, 0.0f, 0.0f,     // source (eyes)
	0.0f, 0.0f, 0.0f,     // target (body)
};

// animation light specific
//#define LIGHT_ANIM_MINIGUN 2
//#define LIGHT_ANIM_TOMMYGUN 3
//#define LIGHT_ANIM_COLT_SHOTGUN 4
#define LIGHT_ANIM_NONE 5

const char *NameForState(PlayerState pst)
{
	switch(pst) {
	case PST_STAND: return "stand";
//	case PST_CROUCH: return "crouch";
	case PST_FALL: return "fall";
	case PST_SWIM: return "swim";
	case PST_DIVE: return "dive";
	default: return "???";
	};
}

/*
// print explanation on how a player died
void PrintPlayerDeathMessage(CPlayer *ppl, const EPlayerDeath &ePLDeath)
{
	CTString strMyName = ppl->GetPlayerName();
	CEntity *penKiller = (CEntity*) ePLDeath.eidInflictor;
	// if killed by a valid entity
	if( penKiller!=NULL) {
		// if killed by a player
		if( IsOfClass( penKiller, &CPlayer_DLLClass)) {
			// if not self
			if( penKiller!=ppl) {
				CTString strKillerName = ((CPlayer*)penKiller)->GetPlayerName();
				
				if(ePLDeath.dmtType==DMT_TELEPORT) {
					CPrintF(TRANS("%s telefragged %s\n"), strKillerName, strMyName);
				} else if(ePLDeath.dmtType==DMT_CLOSERANGE) {
					CPrintF(TRANS("%s cut %s into pieces\n"), strKillerName, strMyName);
				} else if(ePLDeath.dmtType==DMT_CHAINSAW) {
					CPrintF(TRANS("%s cut %s into pieces\n"), strKillerName, strMyName);
				} else if(ePLDeath.dmtType==DMT_BULLET ||
					ePLDeath.dmtType==DMT_SHOTGUN ||
					ePLDeath.dmtType==DMT_SNIPER) {
					CPrintF(TRANS("%s poured lead into %s\n"), strKillerName, strMyName);
				} else if(ePLDeath.dmtType==DMT_PROJECTILE || ePLDeath.dmtType==DMT_EXPLOSION) {
					CPrintF(TRANS("%s blew %s away\n"), strKillerName, strMyName);
				} 
				else {
					CPrintF(TRANS("%s killed %s\n"), strKillerName, strMyName);
				}
			} else {
				// make message from damage type
				switch(ePLDeath.dmtType) {
				case DMT_DROWNING:  CPrintF(TRANS("%s drowned\n"), strMyName); break;
				case DMT_BURNING:   CPrintF(TRANS("%s burst into flames\n"), strMyName); break;
				case DMT_SPIKESTAB: CPrintF(TRANS("%s fell into a spike-hole\n"), strMyName); break;
				case DMT_FREEZING:  CPrintF(TRANS("%s has frozen\n"), strMyName); break;
				case DMT_ACID:      CPrintF(TRANS("%s dissolved\n"), strMyName); break;
				case DMT_PROJECTILE:
				case DMT_EXPLOSION:
					CPrintF(TRANS("%s was blown away\n"), strMyName); break;
				default:            CPrintF(TRANS("%s has committed suicide\n"), strMyName);
				}
			}
			// if killed by an enemy
		} else if( IsDerivedFromClass( penKiller, &CEnemyBase_DLLClass)) {
			// check for telefrag first
			if(ePLDeath.dmtType==DMT_TELEPORT) {
				CPrintF(TRANS("%s was telefragged\n"), strMyName);
				return;
			}
			
			// describe how this enemy killed player - conversion to eDeath is done to reduce changes to existing code
			EDeath eDeath;
			eDeath.eLastDamage.penInflictor = (CEntity*) ePLDeath.eidInflictor;
			eDeath.eLastDamage.vDirection   = ePLDeath.vDirection;
			eDeath.eLastDamage.vHitPoint    = ePLDeath.vHitPoint;
			eDeath.eLastDamage.fAmount      = ePLDeath.fAmount;
			eDeath.eLastDamage.dmtType      = ePLDeath.dmtType;
			CPrintF("%s\n", (const char*)((CEnemyBase*)penKiller)->GetPlayerKillDescription(strMyName, eDeath));
			
			// if killed by some other entity
		} else {
			// make message from damage type
			switch(ePLDeath.dmtType) {
			case DMT_SPIKESTAB: CPrintF(TRANS("%s was pierced\n"), strMyName); break;
			case DMT_BRUSH:     CPrintF(TRANS("%s was squashed\n"), strMyName); break;
			case DMT_ABYSS:     CPrintF(TRANS("%s went over the edge\n"), strMyName); break;
			case DMT_IMPACT:    CPrintF(TRANS("%s swashed\n"), strMyName); break;
			case DMT_HEAT:      CPrintF(TRANS("%s stood in the sun for too long\n"), strMyName); break;
			default:            CPrintF(TRANS("%s passed away\n"), strMyName);
			}
		}
		// if no entity pointer (shouldn't happen)
	} else {
		CPrintF(TRANS("%s is missing in action\n"), strMyName);
	}
}
*/

//안태훈 수정 시작	//(Open beta)(2004-12-08)
void ShotMissile(CEntity *penShoter, const char *szShotPosTagName
				 , CEntity *penTarget, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szArrowEffectName
				 , bool bCritical
				 , FLOAT fHorizonalOffset = 0.0f, FLOAT fVerticalOffset = 0.0f	//-1.0f ~ 1.0f
				 , INDEX iArrowType = 0, const char *szMissileModel = NULL, CSelectedEntities* dcEntities = NULL);
void ShotMagicContinued(CEntity *penShoter, FLOAT3D vStartPos, FLOATquat3D qStartRot
				 , CEntity *penTarget, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szMagicEffectName
				 , bool bCritical, INDEX iOrbitType, BOOL bDirectTag = FALSE);
void ShotGoWaitBack(CEntity *penShoter, FLOAT3D vStartPos, FLOATquat3D qStartRot
				 , CEntity *penTarget, FLOAT fMoveSpeed, FLOAT fWaitTime
				 , const char *szHitEffectName, const char *szMoveEffectName
				 , bool bCritical
				 , FLOAT fHorizonalOffset = 0.0f, FLOAT fVerticalOffset = 0.0f	//-1.0f ~ 1.0f
				 );
void ShotConnect(CEntity *penShoter, CEntity *penTarget, FLOAT fWaitTime
				 , const char *szHitEffectName, const char *szConnectEffectName);
//안태훈 수정 끝	//(Open beta)(2004-12-08)
/*
void ShotFallDown(FLOAT3D vStartPos, FLOAT3D vToTargetDir, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szFallObjEffectName
				 , bool bCritical);
*/
extern void GetTargetDirection(CEntity *penMe, CEntity *penTarget, FLOAT3D &vTargetPos, FLOAT3D &vDirection);
%}


class export CPlayer : CPlayerEntity {
	name      "Player";
	thumbnail "";
	features  "ImplementsOnInitClass", "ImplementsOnEndClass";
	
properties:
	1 CTString m_strName "Name" = "<unnamed player>",
	2 COLOR m_ulLastButtons = 0x0,              // buttons last pressed
	3 FLOAT m_fArmor = 0.0f,                    // armor
	4 CTString m_strGroup = "",                 // group name for world change
	5 INDEX m_ulKeys = 0,                       // mask for all picked-up keys
	6 FLOAT m_fMaxHealth = 1,                 // default health supply player can have
	7 INDEX m_ulFlags = 0,                      // various flags
	
	8 CEntityPointer m_penWeapons,              // player weapons
	9 CEntityPointer m_penAnimator,             // player animator
	10 CEntityPointer m_penView,                 // player view
	11 CEntityPointer m_pen3rdPersonView,        // player 3rd person view
	12 INDEX m_iViewState=PVT_PLAYEREYES,        // view state
	13 INDEX m_iLastViewState=PVT_PLAYEREYES,    // last view state
	
	14 CAnimObject m_aoLightAnimation,           // light animation object
	15 FLOAT m_fDamageAmmount = 0.0f,            // how much was last wound
	16 FLOAT m_tmWoundedTime  = 0.0f,            // when was last wound
	//17 FLOAT m_tmScreamTime   = 0.0f,            // when was last wound sound played
	
	// FiXME : m_iGender를 사용하는 부분을 모두 제거할것.
	18 INDEX m_iGender = GENDER_MALE,            // male/female offset in various tables
	19 enum PlayerState m_pstState = PST_STAND features(EPROPF_NETSEND),  // current player state
	20 FLOAT m_fFallTime = 0.0f features(EPROPF_NETSEND),                 // time passed when falling
	21 FLOAT m_fSwimTime = 0.0f features(EPROPF_NETSEND),                 // time when started swimming
	22 FLOAT m_tmOutOfWater = 0.0f features(EPROPF_NETSEND),              // time when got out of water last time
	23 FLOAT m_tmMoveSound = 0.0f,           // last time move sound was played
	24 BOOL  m_bMoveSoundLeft = TRUE,        // left or right walk channel is current
	25 FLOAT m_tmNextAmbientOnce = 0.0f,     // next time to play local ambient sound
	26 FLOAT m_tmMouthSoundLast = 0.0f,      // time last played some repeating mouth sound
	
	27 CEntityPointer m_penCamera,           // camera for current cinematic sequence, or null
	28 CTString m_strCenterMessage="",       // center message
	29 FLOAT m_tmCenterMessageEnd = 0.0f,    // last time to show centered message
	30 BOOL m_bPendingMessage = FALSE,   // message sound pending to be played
	31 FLOAT m_tmMessagePlay = 0.0f,     // when to play the message sound
	32 FLOAT m_tmAnalyseEnd = 0.0f,      // last time to show analysation
	33 BOOL m_bComputerInvoked = FALSE,  // set if computer was invoked at least once
	34 FLOAT m_tmAnimateInbox = -100.0f,      // show animation of inbox icon animation
	
	35 CEntityPointer m_penMainMusicHolder,
	
	36 FLOAT m_tmLastDamage = -1.0f,
	37 FLOAT m_fMaxDamageAmmount = 0.0f,
	38 FLOAT3D m_vDamage = FLOAT3D(0,0,0),
	39 FLOAT m_tmSpraySpawned = -1.0f,
	40 FLOAT m_fSprayDamage = 0.0f,
	41 CEntityPointer m_penSpray,
	
	// sounds
	// FIXME : 아랫부분도 필요없음
	42 CSoundObject m_soWeapon0,
	43 CSoundObject m_soWeapon1,
	44 CSoundObject m_soWeapon2,
	45 CSoundObject m_soWeapon3,
	46 CSoundObject m_soWeaponAmbient,
	47 CSoundObject m_soPowerUpBeep,
	
	50 CSoundObject m_soMouth,     // breathing, yelling etc.
	51 CSoundObject m_soFootL,     // walking etc.
	52 CSoundObject m_soFootR,
	53 CSoundObject m_soBody,          // splashing etc.
	54 CSoundObject m_soLocalAmbientLoop,  // local ambient that only this player hears
	55 CSoundObject m_soLocalAmbientOnce,  // local ambient that only this player hears
	56 CSoundObject m_soMessage,  // message sounds
//	57 CSoundObject m_soHighScore, // high score sound
//	58 CSoundObject m_soSpeech,    // for quotes
	//59 CSoundObject m_soSniperZoom, // for sniper zoom sound
	
	//61 INDEX m_iMana    = 0,        // current score worth for killed player
	//62 FLOAT m_fManaFraction = 0.0f,// fractional part of mana, for slow increase with time
	63 INDEX m_iHighScore = 0,      // internal hiscore for demo playing
	64 INDEX m_iBeatenHighScore = 0,    // hiscore that was beaten
	65 FLOAT m_tmLatency = 0.0f,               // player-server latency (in seconds)
	// for latency averaging
	66 FLOAT m_tmLatencyLastAvg = 0.0f, 
	67 FLOAT m_tmLatencyAvgSum = 0.0f, 
	68 INDEX m_ctLatencyAvg = 0, 
	
	69 BOOL  m_bEndOfLevel = FALSE,
	70 BOOL  m_bEndOfGame  = FALSE,
	71 INDEX m_iMayRespawn = 0,     // must get to 2 to be able to respawn
	72 FLOAT m_tmSpawned = 0.0f,   // when player was spawned
	73 FLOAT3D m_vDied = FLOAT3D(0,0,0),  // where player died (for respawn in-place)
	74 FLOAT3D m_aDied = FLOAT3D(0,0,0),
	
	// statistics
	75 FLOAT m_tmEstTime  = 0.0f,   // time estimated for this level
	76 INDEX m_iTimeScore = 0,
	77 INDEX m_iStartTime = 0,      // game start time (ansi c time_t type)
	78 INDEX m_iEndTime   = 0,      // game end time (ansi c time_t type)
	79 FLOAT m_tmLevelStarted = 0.0f,  // game time when level started
	80 CTString m_strLevelStats = "",  // detailed statistics for each level
	
	// auto action vars
	82 CEntityPointer m_penActionMarker,  // current marker for auto actions
	83 FLOAT m_fAutoSpeed = 0.0f, // speed to go towards the marker
	84 INDEX m_iAutoOrgWeapon = 0, // original weapon for autoactions
	85 FLOAT3D m_vAutoSpeed = FLOAT3D(0,0,0),
	86 FLOAT m_tmSpiritStart = 0.0f,
	87 FLOAT m_tmFadeStart = 0.0f,
	
	// 'picked up' display vars
	88 FLOAT m_tmLastPicked = -10000.0f,  // when something was last picked up
	89 CTString m_strPickedName = "",     // name of item picked
	90 FLOAT m_fPickedAmmount = 0.0f,     // total picked ammount
	91 FLOAT m_fPickedMana = 0.0f,        // total picked mana
	
	// shaker values
//	95 INDEX m_iLastHealth = 0,
//	96 INDEX m_iLastArmor  = 0,
//	97 INDEX m_iLastAmmo   = 0,
//	98 FLOAT m_tmHealthChanged = -9,
	//99 FLOAT m_tmArmorChanged  = -9,
	//100 FLOAT m_tmAmmoChanged   = -9,
	
//	101 FLOAT m_tmMinigunAutoFireStart = -1.0f,
	
	102 FLOAT3D m_vLastStain  = FLOAT3D(0,0,0), // where last stain was left
	
	// for mouse lag elimination via prescanning
	//151 ANGLE3D m_aLastRotation = FLOAT3D(0,0,0),
	//152 ANGLE3D m_aLastViewRotation = FLOAT3D(0,0,0),
	103 INDEX   m_iLastRotationH = 0,
	104 INDEX   m_iLastRotationP = 0,
	105 INDEX   m_iLastRotationB = 0,
	106 INDEX   m_iLastViewRotationH = 0,
	107 INDEX   m_iLastViewRotationP = 0,
	108 INDEX   m_iLastViewRotationB = 0,
	109 FLOAT3D m_vLastTranslation = FLOAT3D(0,0,0),
	110 ANGLE3D m_aLocalRotation = FLOAT3D(0,0,0),
	111 ANGLE3D m_aLocalViewRotation = FLOAT3D(0,0,0),
	112 FLOAT3D m_vLocalTranslation = FLOAT3D(0,0,0),
	
	// powerups (DO NOT CHANGE ORDER!) - needed by HUD.cpp
	113 FLOAT m_tmInvisibility    = 0.0f, 
	114 FLOAT m_tmInvulnerability = 0.0f, 
	115 FLOAT m_tmSeriousDamage   = 0.0f, 
	116 FLOAT m_tmSeriousSpeed    = 0.0f, 
	117 FLOAT m_tmInvisibilityMax    = 30.0f,
	118 FLOAT m_tmInvulnerabilityMax = 30.0f,
	119 FLOAT m_tmSeriousDamageMax   = 40.0f,
	120 FLOAT m_tmSeriousSpeedMax    = 20.0f,
	
	121 FLOAT m_tmChainShakeEnd = 0.0f, // used to determine when to stop shaking due to chainsaw damage
	122 FLOAT m_fChainShakeStrength = 1.0f, // strength of shaking
	123 FLOAT m_fChainShakeFreqMod = 1.0f,  // shaking frequency modifier
	124 FLOAT m_fChainsawShakeDX = 0.0f, 
	125 FLOAT m_fChainsawShakeDY = 0.0f,
	
//	126 INDEX m_iSeriousBombCount = 0,      // ammount of serious bombs player owns
//	127 INDEX m_iLastSeriousBombCount = 0,  // ammount of serious bombs player had before firing
//	128 FLOAT m_tmSeriousBombFired = -10.0f,  // when the bomb was last fired

	// [100107: selo] 트레이너를 피하기 위한 중요 데이터 es로 피신
	130 INDEX m_sbAttackspeed = 0, 
	131 INDEX m_sbMagicspeed = 0,
	132 FLOAT m_fAttackrange = 0.0f,
	133 FLOAT m_fRunspeed = 0.0f,
	134 FLOAT m_fWalkspeed = 0.0f,

	// data used to properly reintialize the player over the net
	141 INDEX m_iWeaponsID = -1  features(EPROPF_NETSEND),
	142 INDEX m_iAnimatorID = -1 features(EPROPF_NETSEND),
	143 BOOL  m_bInitializeOverNet = TRUE features(EPROPF_NETSEND),
	
	154 BOOL m_bDontLerpView=FALSE,
	//0105
	155 BOOL m_bMdl = FALSE,//TRUE,//
	156 FLOAT3D m_LocCurrent= FLOAT3D(0,0,0),
	157 FLOAT3D m_LocDesired= FLOAT3D(0,0,0), 
	159 FLOAT m_tmSpawnedForInv = -99.0f,   // when player was spawned, used to calculate invulnerability
	160 BOOL m_bIsMove = FALSE,
	161 BOOL m_bStartAttack = FALSE,
	
	162 FLOAT3D m_vDesiredPosition = FLOAT3D(0,0,0),
	163 ANGLE m_aRotateSpeed =  AngleDeg(1800.0f),
	164 FLOAT m_fMoveSpeed = 5.0f,
	165 FLOAT m_fMoveFrequency = 0.25f,
	166 BOOL m_bMoving = FALSE,
	
	167 FLOAT m_fBurnPower "Burn Power" = 1.0f,
	168 FLOAT m_fBurnRatio "Burn Ratio" = 1.0f,
	169 FLOAT m_fBurnTime "Burn Time" = 0.5f,
	170 FLOAT m_fBurnFadeInTime "Brun Fade-in Time" = 0.2f,
	171 FLOAT m_fBurnFadeOutTime "Brun Fade-out Time" = 0.2f,
	
	173 CEntityPointer m_penAttackingEnemy,
//	174 FLOAT m_fEnemyHealth = -1.0f,
	//176 INDEX m_IdWeapon = -1,
	//0611 kwon
	176 BOOL  m_bLockMove = FALSE,

	// m_bReserveMove -> LockMove가 걸린 상태에서, 어딘가로 이동하도록 클릭했을때... 켜지는 플래그.
	177 BOOL  m_bReserveMove = FALSE,
	178 BOOL  m_bForward = FALSE,
	179 BOOL  m_bKeyMove = FALSE,

	181   INDEX	m_nDesiredSkillNum = -1,
	182	BOOL 	m_bReserveSkill = FALSE,//안쓴다.
	183	BOOL	m_bSkilling = FALSE,//
	184	INDEX	m_nCurrentSkillNum = -1,
	185	INDEX	m_nReservedSkillNum = -1,
	//186	BOOL	m_bNeedSkillTarget = TRUE,//FALSE,
	187	FLOAT	m_fSkillDistance = 15.0f,
	188	INDEX	m_idCurrentSkillAnim = -1,
	189	FLOAT	m_tmSkillStartTime	= 0.0f,
	190	FLOAT   m_tmSkillAnimTime = 0.0f,
	191	FLOAT	m_fSkillAnimTime = 0.0f,
	192 BOOL	m_bWaitForSkillTarget = FALSE,
	193 ANGLE   m_aHeadingRotation = 0.0f,
	194 INDEX	m_nPreDesiredSkillNum = -1,
	195	BOOL	m_bWaitForSkillResponse = FALSE,
	196 FLOAT   m_fCameraAngle	=0.0f,

	197 BOOL	m_bPlayAction = FALSE,
//	198 BOOL	m_nPlayFace = -1,
	198 INDEX	m_nPlayActionNum = -1,

	199 CEntityPointer m_penWillDeathMob,
	200 BOOL m_bReservedCancel = FALSE,

	201 BOOL m_bNotRenderParticles = FALSE,
	202 BOOL m_bRcvAtMsg	= FALSE,
	//203 INDEX m_nMakerId	=0,
	204 BOOL m_bCriticalDamage = FALSE,
	205 BOOL m_bReadySendSkillMessage = TRUE,

	206 INDEX	m_idCurrentActionAnim = -1,	
	209 INDEX   m_nCurrentActionNum	= -1,
	210 INDEX   m_nDesiredActionNum = -1,
	211	BOOL	m_bAction = FALSE,
	212 BOOL	m_nActionSit = 0, // 1: 앉기, 2:앉기유지, 3: 서기.

	213 CTFileName m_fnMusic "Music"   'M' = CTFILENAME("data\\sounds\\BGM\\Another_Ballad.ogg"),
	214 FLOAT m_fVolume "Volume" 'V' = 10.0f,
	215 enum MusicType m_mtType "Type" 'Y' = MT_LIGHT,//MT_MEDIUM,
	//216 BOOL m_bForceStart "Force start" 'F' = TRUE,
	217 BOOL m_bFieldArea = FALSE,

	218 BOOL m_bDying = FALSE,
	219 BOOL m_bLockSkillCancel = FALSE,
	220 BOOL m_bConnectEffect = FALSE,
	//220 CEntityPointer penOldTarget,
	//..
	
	221 BOOL	m_bProduction = FALSE,
	222 INDEX	m_nProductionNum = -1,
	223 BOOL	m_bEnvironmentSound	= FALSE,

	224 BOOL	m_bStartPostSkill = TRUE,
	225 BOOL	m_bRunningMode = TRUE,

//안태훈 수정 시작	//(Open beta)(2004-12-23)
	226 INDEX	m_iSkillEffectStep = 0,
//안태훈 수정 끝	//(Open beta)(2004-12-23)

	227 BOOL	m_bCriticalDamageMe = FALSE,	
	229 FLOAT	m_tmSendSkillMessage = 0.0f,
	230 BOOL    m_bSpeedUp = FALSE,
	231 BOOL    m_bSendStopMessage = TRUE,

	232 FLOAT3D m_vMyPositionTmp = FLOAT3D(0.0f, 0.0f, 0.0f),
	233 BOOL	m_bMobChange = FALSE,
	234 INDEX	m_nLegit = FALSE,
	235 BOOL	m_bStuned = FALSE,
	236 BOOL	m_bDisappearWeapon = FALSE,
	237 BOOL    m_bChanging = FALSE,
	238 INDEX	m_nChangeMonsterId = 0,
	//239 BOOL	m_bRunningSelfSkill = FALSE,		// 타겟이 필요없는 셀프 스킬.
	239	INDEX	m_nProduceItemDBIndex = 0,
	240 BOOL	m_bHold = FALSE,	//이동을 제외한 동작은 가능.
	241 BOOL	m_bCannotUseSkill = FALSE, //아이템을 제외한 스킬 사용불가.
	242 BOOL	m_bRide = FALSE,	
	245 INDEX	m_iRideType	= -1,	// 0일 경우 말, 1일 경우 용	
	246	BOOL	m_bWildRide = FALSE,	// 임시타입. 나중에 시스템화 할때 변경할 것. [12/7/2010 rumist]
	248 BOOL	m_bDeathChk = FALSE, //죽었을 경우 서버외적으로 체크 해둠
	249 BOOL	m_bIsTransform = FALSE,		// 변신체의 경우.
	250 INDEX	m_iTransformType = 0,		// 변신체 타입.
	251 BOOL	m_bTransforming = FALSE,	// 강신중...
	252 BOOL	m_bOnlyRotate = FALSE,			// 회전
	253 BOOL	m_bClicked		= FALSE,		// 탈것 버그 수정. [9/4/2009 rumist]
	254 BOOL	m_bCanSkillCancel = TRUE,
		
	{
		ShellLaunchData ShellLaunchData_array;  // array of data describing flying empty shells
		INDEX m_iFirstEmptySLD;                         // index of last added empty shell
		
		BulletSprayLaunchData BulletSprayLaunchData_array;  // array of data describing flying bullet sprays
		INDEX m_iFirstEmptyBSLD;                            // index of last added bullet spray
		
		GoreSprayLaunchData GoreSprayLaunchData_array;   // array of data describing gore sprays
		INDEX m_iFirstEmptyGSLD;                         // index of last added gore spray
		
		ULONG ulButtonsNow;  
		ULONG ulButtonsBefore;
		ULONG ulNewButtons;
		ULONG ulReleasedButtons;
		
		// listener
		CSoundListener sliSound;
		// light
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
		//CLightSource m_lsLightSource;
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
		
		// all messages in the inbox
//강동민 수정 시작 다중 공격 작업	08.22
		//CDynamicContainer<CEntity>		m_dcEnemies;
		//CSelectedEntities				m_SelectedTargets;
		CSelectedEntities				m_dcEnemies;
//강동민 수정 끝 다중 공격 작업		08.22
		INDEX m_ctUnreadMessages;
		
		// statistics
		PlayerStats m_psLevelStats;
		PlayerStats m_psLevelTotal;
		PlayerStats m_psGameStats;
		PlayerStats m_psGameTotal;
		
		CModelObject m_moRender;                  // model object to render - this one can be customized
		//0105
		CModelInstance m_miRender;
		CAnyProjection3D m_apr;
		
		CValue<FLOAT> m_fBurnLeftTime;
		CValue<FLOAT> m_fLastBurnTime;
		//..
//안태훈 수정 시작	//(5th Closed beta)(0.2)
		CEffectGroup *m_pPickingEffectGroup;
		CEffectGroup *m_pSkillReadyEffect;
//안태훈 수정 끝	//(5th Closed beta)(0.2)
		CEffectGroup* m_pAttachPlayerEffect;
		CEffectGroup* m_pAttachEnemyEffect;
		CEffectGroup* m_pHolyWaterEffectL;
		CEffectGroup* m_pHolyWaterEffectR;
		CEntityPointer	m_penStillTarget;

		MeshInstance miWeapon;
		CStaticArray<FLOAT> m_afHardCodeValue;

		CModelInstance		*m_pRiderModelInstance;
		CModelInstance		*m_pRidePetModelInstance;
		CModelInstance		*m_pRideCharModelInstance;

		// 임시.
		QVect mi_qvOffset;      // current offset from parent model instance
		INDEX mi_iParentBoneID; // ID of parent bone in parent model instance

		CEntityPointer	m_penClickTarget; // 오브젝트 클릭시에 저장되는 변수(참조카운팅을 쓰지않게 직접 접근해서 사용)(변수를 NULL로 초기화하기위해 CEntityPointer사용)
		CTextureObject	m_pSkillTexture;
		
		INDEX iDeathMotion;		// 데스 모션 스킬 애니메이션 인덱스(애니메이션 중일때 이동 못하도록)
		
		BOOL m_bNoAniGuildSkill;
		// UI용 모델 데이터 
		HUD_MLData m_HUDMLData;
		HUD_MLData m_HUDItemData;

		INDEX		m_nWildPetType;			// 1 : human 2: beast 3 : deamon [3/21/2011 rumist]
		INDEX		m_iEnermyID;
	}
		
components:
	1 class   CLASS_PLAYER_WEAPONS  "Classes\\PlayerWeapons.ecl",
	2 class   CLASS_PLAYER_ANIMATOR "Classes\\PlayerAnimator.ecl",
	3 class   CLASS_PLAYER_VIEW     "Classes\\PlayerView.ecl",
	4 class   CLASS_BASIC_EFFECT    "Classes\\BasicEffect.ecl",
	5 class   CLASS_BLOOD_SPRAY     "Classes\\BloodSpray.ecl", 
	
	// gender specific sounds - make sure that offset is exactly 100
	// Serious
	10 sound SOUND_DEFAULT					"Data\\Sounds\\Default.wav",

	// Bsp
	15 sound SOUND_WALK_MARBLE_IN_L			"Data\\Sounds\\Character\\public\\C_run_in_L1.wav",
	16 sound SOUND_WALK_MARBLE_IN_R			"Data\\Sounds\\Character\\public\\C_run_in_R1.wav",
	17 sound SOUND_WALK_BLOCK_OUT_L			"Data\\Sounds\\Character\\public\\C_run_out_L5.wav",
	18 sound SOUND_WALK_BLOCK_OUT_R			"Data\\Sounds\\Character\\public\\C_run_out_R5.wav",
	19 sound SOUND_WALK_WOOD_OUT_L			"Data\\Sounds\\Character\\public\\C_run_out_L4.wav",
	20 sound SOUND_WALK_WOOD_OUT_R			"Data\\Sounds\\Character\\public\\C_run_out_R4.wav",	

	30 sound SOUND_WALK_ELENEN_WALK			"Data\\Sounds\\Character\\public\\CE_walk.wav",	
	31 sound SOUND_WALK_ELENEN_RUN			"Data\\Sounds\\Character\\public\\CE_run.wav",	
	32 sound SOUND_WALK_HELL_WALK			"Data\\Sounds\\Character\\public\\CH_walk.wav",	
	33 sound SOUND_WALK_HELL_RUN			"Data\\Sounds\\Character\\public\\CH_run.wav",	

	34 sound SOUND_WALK_HORSE_WALK			"Data\\Sounds\\Npc\\Pet\\pet_m_horse3_walk.wav",	
	35 sound SOUND_WALK_HORSE_RUN			"Data\\Sounds\\Npc\\Pet\\pet_m_horse3_run.wav",	
	36 sound SOUND_WALK_DRAGON_WALK			"Data\\Sounds\\Npc\\Pet\\pet_m_dragon3_walk.wav",	
	//37 sound SOUND_WALK_DRAGON_RUN			"Data\\Sounds\\Npc\\Pet\\pet_m_dragon3_run.wav",

	// Terrain
	110 sound SOUND_TER_WALK_DIRT_L			"Data\\Sounds\\Character\\public\\C_run_out_L6.wav",
	111 sound SOUND_TER_WALK_DIRT_R			"Data\\Sounds\\Character\\public\\C_run_out_R6.wav",
	112 sound SOUND_TER_WALK_GRASS_L		"Data\\Sounds\\Character\\public\\C_run_out_L1.wav",
	113 sound SOUND_TER_WALK_GRASS_R		"Data\\Sounds\\Character\\public\\C_run_out_R1.wav",
	114 sound SOUND_TER_WALK_ROCK_L			"Data\\Sounds\\Character\\public\\C_run_out_L6.wav",
	115 sound SOUND_TER_WALK_ROCK_R			"Data\\Sounds\\Character\\public\\C_run_out_R6.wav",
	116 sound SOUND_TER_WALK_BLOCK_L		"Data\\Sounds\\Character\\public\\C_run_out_L5.wav",
	117 sound SOUND_TER_WALK_BLOCK_R		"Data\\Sounds\\Character\\public\\C_run_out_R5.wav",

	120 sound SOUND_DAIR_STRIKE_PRE			"Data\\Sounds\\Character\\rogue\\skill\\CRs_skill_06.wav",
	121 sound SOUND_DAIR_STRIKE_BLOW1		"Data\\Sounds\\Character\\rogue\\skill\\CRs_skill_06_blow01.wav",
	122 sound SOUND_DAIR_STRIKE_BLOW2		"Data\\Sounds\\Character\\rogue\\skill\\CRs_skill_06_blow02.wav",
	123 sound SOUND_DAIR_STRIKE_BLOW3		"Data\\Sounds\\Character\\rogue\\skill\\CRs_skill_06_blow03.wav",
	124 sound SOUND_A_DAIR_STRIKE_PRE		"Data\\Sounds\\Character\\rogue\\skill\\CRs_a_skill_06.wav",
	125 sound SOUND_A_DAIR_STRIKE_BLOW1		"Data\\Sounds\\Character\\rogue\\skill\\CRs_a_skill_06_blow01.wav",
	126 sound SOUND_A_DAIR_STRIKE_BLOW2		"Data\\Sounds\\Character\\rogue\\skill\\CRs_a_skill_06_blow02.wav",
	127 sound SOUND_A_DAIR_STRIKE_BLOW3		"Data\\Sounds\\Character\\rogue\\skill\\CRs_a_skill_06_blow03.wav",

	// Water
	190	sound SOUND_WATER_WALK_SHALLOW_L	"Data\\Sounds\\Character\\public\\C_run_out_L3.wav",
	191	sound SOUND_WATER_WALK_SHALLOW_R	"Data\\Sounds\\Character\\public\\C_run_out_R3.wav",
	//192	sound SOUND_WATER_WALK_DEEP			"Data\\Sounds\\Character\\public\\C_run_out_R5.wav",		// 나중에 물에서 잠수하면..

	200 sound SOUND_SWORD_BLOW              "Data\\Sounds\\Character\\public\\C_blow_sword01.wav",
	201 sound SOUND_SWORD_CRITICAL          "Data\\Sounds\\Character\\public\\C_critical_blow_sword01.wav",
	202 sound SOUND_BIGSWORD_BLOW           "Data\\Sounds\\Character\\public\\C_blow_bsword01.wav",
	203 sound SOUND_BIGSWORD_CRITICAL       "Data\\Sounds\\Character\\public\\C_critical_blow_bsword01.wav",	
//	204 sound SOUND_BOW_BLOW                "Data\\Sounds\\Character\\public\\C_blow_bow01.wav",
//	205 sound SOUND_BOW_CRITICAL            "Data\\Sounds\\Character\\public\\C_critical_blow_bow01.wav",
	204 sound SOUND_POTION					"Data\\sounds\\character\\public\\C_potion.wav",
	205 sound SOUND_CRAK					"Data\\sounds\\game\\nature\\GN_production.wav",
//	206 sound SOUND_MINE					"Data\\sounds\\game\\nature\\GN_mine.wav",
	207 sound SOUND_PROCESS					"Data\\sounds\\game\\nature\\GN_process.wav",
	208 sound SOUND_PRODUCT					"Data\\sounds\\game\\nature\\GN_make.wav",
	209 sound SOUND_POLYMOPH				"Data\\sounds\\character\\public\\C_transformation.wav",

	210 sound SOUND_DAGGER_BLOW              "Data\\Sounds\\Character\\public\\C_blow_dagger01.wav",
	211 sound SOUND_DAGGER_CRITICAL          "Data\\Sounds\\Character\\public\\C_critical_blow_dagger01.wav",
	212 sound SOUND_FOOT_BLOW				 "Data\\Sounds\\Character\\public\\C_blow_foot01.wav",
	213 sound SOUND_FOOT_CRITICAL			 "Data\\Sounds\\Character\\public\\C_critical_blow_foot01.wav",	
	214 sound SOUND_AXE_BLOW                 "Data\\Sounds\\Character\\public\\C_blow_axe01.wav",
	215 sound SOUND_AXE_CRITICAL             "Data\\Sounds\\Character\\public\\C_critical_blow_axe01.wav",
	// Item
	230 sound SOUND_DROP_ITEM				"Data\\Sounds\\game\\nature\\GN_item_drop.wav",
	231 sound SOUND_DROP_MONEY				"Data\\Sounds\\game\\nature\\GN_money_drop.wav",
	232 sound SOUND_PICK_ITEM				"Data\\Sounds\\game\\nature\\GN_item_pick.wav",  	
	233 sound SOUND_BUTTON_CLICK			"Data\\sounds\\game\\system\\GS_click.wav",
	// UI
	500	sound SOUND_UI_JEWEL_DULL			"Data\\Sounds\\UI\\Jewel_Compos\\Jewel_Dull.wav",
	501	sound SOUND_UI_JEWEL_REFINEMENT		"Data\\Sounds\\UI\\Jewel_Compos\\Jewel_Refinement.wav",
	502	sound SOUND_UI_JEWEL_SHINY			"Data\\Sounds\\UI\\Jewel_Compos\\Jewel_Shiny.wav",

	503 sound SOUND_UI_PETSTASH_CARD		"Data\\Sounds\\UI\\Pet_Stash\\Card_move.wav",
	504 sound SOUND_UI_PETSTASH_CARD_SELECT	"Data\\Sounds\\UI\\Pet_Stash\\Card_select.wav",

	505 sound SOUND_UI_HOLYWATER			"Data\\Sounds\\UI\\HolyWater\\HolyWater_Activation.wav",

	// Effect //힐러스킬은 애니메이션을 공통적으로 쓰기때문에 사운드를 SKA에 붙일수가 없다.
	240 sound SOUND_HEALER_HEAL_PRE			"Data\\sounds\\character\\healer\\skill\\CHs_magic_before01.wav",
	241 sound SOUND_HEALER_HEAL_DO			"Data\\sounds\\character\\healer\\skill\\CHs_magic_start01.wav",
	242 sound SOUND_HEALER_PARTY_HEAL_PRE	"Data\\sounds\\character\\healer\\skill\\CHs_magic_before02.wav",
	243 sound SOUND_HEALER_PARTY_HEAL_DO	"Data\\sounds\\character\\healer\\skill\\CHs_magic_start01.wav",
	244 sound SOUND_HEALER_DEFENCE_MANA_PRE	"Data\\sounds\\character\\healer\\skill\\CHs_magic_before02.wav",
	245 sound SOUND_HEALER_DEFENCE_MANA_DO	"Data\\sounds\\character\\healer\\skill\\CHs_magic_start02.wav",


	247 sound SOUND_HEALER_DAMAGE			"Data\\sounds\\character\\healer\\voice\\CHv_critical_dam01.wav",
	249 sound SOUND_MAGE_DAMAGE				"Data\\sounds\\character\\mage\\voice\\CMv_critical_dam01.wav",
	250 sound SOUND_TITAN_DAMAGE			"Data\\sounds\\character\\titan\\voice\\CTv_critical_dam01.wav",
	251 sound SOUND_KNIGHT_DAMAGE			"Data\\sounds\\character\\knight\\voice\\CKv_critical_dam01.wav",
	181 sound SOUND_ROGUE_DAMAGE			"Data\\sounds\\character\\rogue\\voice\\CRv_critical_dam01.wav",
	182 sound SOUND_SORCERER_DAMAGE			"Data\\sounds\\character\\sorcerer\\voice\\CSv_dam01.wav",

	252 sound SOUND_HEALER_ATTACK			"Data\\sounds\\character\\healer\\voice\\CHv_critical_att01.wav",	
	253 sound SOUND_MAGE_ATTACK				"Data\\sounds\\character\\mage\\voice\\CMv_critical_att01.wav",
	254 sound SOUND_TITAN_ATTACK			"Data\\sounds\\character\\titan\\voice\\CTv_critical_att01.wav",
	255 sound SOUND_KNIGHT_ATTACK			"Data\\sounds\\character\\knight\\voice\\CKv_critical_att01.wav",
	184 sound SOUND_ROGUE_ATTACK			"Data\\sounds\\character\\rogue\\voice\\CRv_critical_att01.wav",
	185 sound SOUND_SORCERER_ATTACK			"Data\\sounds\\character\\sorcerer\\voice\\CSv_critical_att01.wav",

			
functions:
	void Precache(void) 
	{
		CPlayerEntity::Precache();
	};

	void LevelChangePlayerClear(void) 
	{
		// clear all possible FX text
		m_tmLastPicked      = -99.0f;
		m_tmInvisibility    = -99.0f;
		m_tmInvulnerability = -99.0f;
		m_tmSeriousDamage   = -99.0f;
		m_tmSeriousSpeed    = -99.0f;
		m_tmSpawned         = -99.0f;
		en_tmLastBreathed   = en_tmEntityTime;
		//0105
		m_tmSpawnedForInv   = -99.0f;
		//..
	}
	
	void OnEnd(void) 
	{    
		CPlayerEntity::OnEnd();        
	}
	
	/* Handle an event, return false if the event is not handled. */
	BOOL HandleEvent(const CEntityEvent &ee)
	{ 
		if (ee.ee_slEvent==EVENTCODE_EShowConsole) {
			if (!_pNetwork->IsServer()) {
				SetGameEnd();        
			} 
			return TRUE;
		} else if (ee.ee_slEvent==EVENTCODE_EPlayerStats) {
			if (!_pNetwork->IsServer()) {
				EPlayerStats epls = (EPlayerStats&) ee;
				CPlayer *ppl = (CPlayer*)_pNetwork->ga_srvServer.srv_apltPlayers[epls.iIndex].plt_penPlayerEntity;
				if (_pNetwork->ga_srvServer.srv_apltPlayers[epls.iIndex].IsActive() && ppl != NULL) {
//					ppl->m_psGameStats.ps_iScore = epls.iFrags;
					ppl->m_psGameStats.ps_iKills = epls.iKills;
					ppl->m_psGameStats.ps_iDeaths = epls.iDeaths;
				}          
			}
			return TRUE;
		} else if (ee.ee_slEvent==EVENTCODE_EAddBulletSpray) {
			if (!_pNetwork->IsServer()) {
				EAddBulletSpray &eabs = (EAddBulletSpray&)ee;
				AddBulletSpray(eabs.vPos,eabs.eptType,eabs.vReflected);
			}
			return TRUE;
		} else if (ee.ee_slEvent==EVENTCODE_EAddGoreSpray) {
			if (!_pNetwork->IsServer()) {
				EAddGoreSpray &eags = (EAddGoreSpray&)ee;
				CEntity* penEntity;
				if (_pNetwork->ga_World.EntityExists(eags.iEntityHit,penEntity)) {
					FLOAT3D vSpillDir = -en_vGravityDir*0.5f;
					FLOATaabbox3D boxCutted=FLOATaabbox3D(FLOAT3D(0,0,0),FLOAT3D(1,1,1));
					if (penEntity->GetRenderType()==RT_MODEL) {
						penEntity->en_pmoModelObject->GetCurrentFrameBBox( boxCutted);
					} else if (penEntity->GetRenderType()==RT_SKAMODEL) {
						INDEX iColBox = penEntity->GetModelInstance()->GetCurrentColisionBoxIndex();
						ColisionBox cb = penEntity->GetModelInstance()->GetColisionBox(iColBox);
						boxCutted = FLOATaabbox3D(cb.Min(),cb.Max());
					}
					FLOAT fPower=4.0f;
					/*
					if( IsOfClass( penEntity, "Gizmo")
						|| IsOfClass( penEntity, "Beast"))        {fPower=4.0f;}
						*/
					//if( IsOfClass( penEntity, "Boneman"))      {fPower=6.0f;}
					//if( IsOfClass( penEntity, "Woman"))        {fPower=3.0f;}
					//if( IsOfClass( penEntity, "Slave"))    {fPower=3.0f;}
					//if( IsOfClass( penEntity, "Walker"))       {fPower=30.0f;}
					//if( IsOfClass( penEntity, "AirSlave")) {fPower=6.0f;}
					AddGoreSpray( eags.vPos, eags.vHit, eags.sptType, vSpillDir, boxCutted, fPower, eags.colParticles);
				}
			}
			return TRUE;
		}
		return CPlayerEntity::HandleEvent(ee);
	}
	
	// override in player.es
	virtual void SetWeaponAndAnimator()
	{
		m_iWeaponsID = m_penWeapons->en_ulID;
		m_iAnimatorID = m_penAnimator->en_ulID;
	};

	// FIXME : 플레이어에서 MSG_CHAR_PC값 설정해야 할 부분이 정확하지 않아서,
	// FIXME : 일단은 이렇게 처리함.
	virtual SBYTE GetNetworkType()	const
	{
		return MSG_CHAR_PC;
	};
	
	void SetRotation(INDEX &iRotPacked,FLOAT fUnpacked) 
	{    
		iRotPacked = (UWORD)(fUnpacked*20);
	}
	
	FLOAT GetRotation(INDEX iRotPacked) 
	{
		return ((FLOAT) iRotPacked)/20;
	}
	
	FLOAT GetRotationDiff(INDEX iRotPacked,FLOAT fUnpacked)
	{
		UWORD uwRotPacked2 = (UWORD)(fUnpacked*20);
		UWORD uwDiff = uwRotPacked2 - ((UWORD)iRotPacked);
		return ((FLOAT) ((SWORD)uwDiff))/20;
	}
	
	void PlaySound(CSoundObject &so, SLONG idSoundComponent, SLONG slPlayType)
	{
		if (!IsReapplyingActions()) {
			CEntity::PlaySound(so, idSoundComponent, slPlayType);
		}
	};
	void PlaySound(CSoundObject &so, const CTFileName &fnmSound, SLONG slPlayType)
	{
		if (!IsReapplyingActions()) {
			CEntity::PlaySound(so, fnmSound, slPlayType);
		}
	};
	
	void InitializeFromNet()
	{
		EPlayerInit ePLInit;
		ePLInit.ulWeaponsID = m_iWeaponsID;
		ePLInit.ulAnimatorID = m_iAnimatorID;
		ePLInit.bCreate = m_bInitializeOverNet;
		Initialize(ePLInit,FALSE);
	}
	
	BOOL IsActionApplied() {
		return ((m_ulFlags & PLF_APPLIEDACTION) != 0);
	}
	
	virtual void SetActionApplied() {
		m_ulFlags |= PLF_APPLIEDACTION;
	};
	
	virtual void ClearActionApplied() {
		m_ulFlags&=~PLF_APPLIEDACTION;
	};	
	
	virtual BOOL IsReapplyingActions() {
		return ((m_ulFlags & PLF_REAPPLYINGACTIONS) != 0);
	}
	virtual void SetReapplyingActions() {
		m_ulFlags |= PLF_REAPPLYINGACTIONS;
	};
	virtual void ClearReapplyingActions() {
		m_ulFlags&=~PLF_REAPPLYINGACTIONS;
	};
	
	virtual void DumpToConsole() 
	{
		CPlayerWeapons* penWeapons = (CPlayerWeapons*)(CEntity*) m_penWeapons;
		CPrintF(TRANS("Entity ID: %X, Name: %s\n"),en_ulID,en_pcCharacter.pc_strName);
		CPrintF(TRANS("   Health: %d, Armor: %d\n"),(INDEX)en_fHealth,m_fArmor);
		//CTString strWeaponName;
		//UWORD uwAmmo = penWeapons->GetWeaponAmmo(penWeapons->m_iCurrentWeapon);
		//uwAmmo &= 0x07FF;
//		penWeapons->GetWeaponName(strWeaponName);
		
		CPrintF(TRANS("   PlayerFlags: 0x%08X\n"),m_ulFlags);
		CPrintF(TRANS("   Available weapons: 0x%X\n"),penWeapons->m_iAvailableWeapons);
		//CPrintF(TRANS("   Current weapon: %d - %s, Ammo: %d\n"),penWeapons->m_iCurrentWeapon,strWeaponName,uwAmmo);
		
		if (IsFlagOn(ENF_ALIVE)) {
			CPrintF(TRANS("   ENF_ALIVE = TRUE\n"));
		} else {
			CPrintF(TRANS("   ENF_ALIVE = FALSE\n"));
		}
		
		CPrintF(TRANS("   Placement:       x: %6.2f, y: %6.2f, z:%6.2f\n"),en_plPlacement.pl_PositionVector(1)
			,en_plPlacement.pl_PositionVector(2),en_plPlacement.pl_PositionVector(3));
		CPrintF(TRANS("   Last Placement:  x: %6.2f, y: %6.2f, z:%6.2f\n"),en_plLastPlacement.pl_PositionVector(1)
			,en_plLastPlacement.pl_PositionVector(2),en_plLastPlacement.pl_PositionVector(3));
		CPrintF(TRANS("   Translation:     x: %6.2f, y: %6.2f, z:%6.2f\n"),en_vCurrentTranslationAbsolute(1)
			,en_vCurrentTranslationAbsolute(2),en_vCurrentTranslationAbsolute(3));
		CPrintF(TRANS("   Rotation:        h: %6.2f, p: %6.2f, b:%6.2f\n"),en_aCurrentRotationAbsolute(1)
			,en_aCurrentRotationAbsolute(2),en_aCurrentRotationAbsolute(3));
		
		switch (en_RenderType) {
		case RT_ILLEGAL        : { CPrintF(TRANS("   RenderType: RT_ILLEGAL\n")); break;}
		case RT_NONE           : { CPrintF(TRANS("   RenderType: RT_NONE\n")); break;}
		case RT_MODEL          : { CPrintF(TRANS("   RenderType: RT_MODEL\n")); break;}
		case RT_BRUSH          : { CPrintF(TRANS("   RenderType: RT_BRUSH\n")); break;}
		case RT_EDITORMODEL    : { CPrintF(TRANS("   RenderType: RT_EDITORMODEL\n")); break;}
		case RT_VOID           : { CPrintF(TRANS("   RenderType: RT_VOID\n")); break;}
		case RT_FIELDBRUSH     : { CPrintF(TRANS("   RenderType: RT_FIELDBRUSH\n")); break;}
		case RT_SKAMODEL       : { CPrintF(TRANS("   RenderType: RT_SKAMODEL\n")); break;}
		case RT_SKAEDITORMODEL : { CPrintF(TRANS("   RenderType: RT_SKAEDITORMODEL\n")); break;}
		case RT_TERRAIN        : { CPrintF(TRANS("   RenderType: RT_TERRAIN\n")); break;}
		};
	};
	
	virtual void ProcessProduce(SBYTE sbChaType, SLONG slChaIndex, SBYTE sbTargetType, SLONG slTargetIndex, SLONG slTargetHP)
	{
		if( sbTargetType != MSG_CHAR_NPC )
		{
			return;
		}
		
		CEntity *penTargetEntity = NULL;

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, slChaIndex);

		if (pObject != NULL)
		{
			penTargetEntity = pObject->GetEntity();

			if(slTargetHP  <= 0)
			{
				if (penTargetEntity != NULL)
				{
					((CUnit*)penTargetEntity)->DeathNow();
				}

				if (_pNetwork->MyCharacterInfo.index == slChaIndex)
				{
					// 생산중 대상 npc가 hp=0일때 사라지면서 생산을 중단 시킨다.( 모든 pc )
					// ((CCharacterBase*)penCharacter)->StopProduce() -- 삭제 후 
					// CancelProduct()사용
					if (IsProduct())
					{
						CancelProduct(); 
					}
				}
				else
				{
					if (penTargetEntity != NULL)
					{
						((CCharacter*)penTargetEntity)->m_nPlayProduceNum = -1;
					}
				}
			}
		}
	}
	
	// 소환수나 애완동물의 HP를 갱신해줌.
	void UpdateUnitInfo( CEntity *pEntity, INDEX iTargetID, SLONG lTargetHP )
	{
		if( pEntity == NULL)	{ return; }
		ObjInfo* pInfo = ObjInfo::getSingleton();

		if( pEntity->IsFirstExtraFlagOn  (ENF_EX1_CURRENT_PET ) )
		{
			pInfo->GetMyPetInfo()->fHealth = lTargetHP;
		}else if( pEntity->IsFirstExtraFlagOn  (ENF_EX1_CURRENT_WILDPET ) )
		{
			pInfo->GetMyPetInfo()->fHealth = lTargetHP;
		}
		// 내 소환수의 경우 인터페이스의 HP를 갱신해줌.
		else if( pEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE) )
		{
			CUIManager* pUIManager = SE_Get_UIManagerPtr();
			
			for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
			{
				CUISummon* pUISummon = (CUISummon*)pUIManager->GetUI(i);
				if( pUISummon->GetSummonEntity() && pUISummon->GetSummonIndex() == iTargetID )
				{
					pInfo->GetMySlaveInfo(i - UI_SUMMON_START)->fHealth	= lTargetHP;											
				}
			}
		}
	}

	//HUD_DrawModel
	virtual void HUD_DrawModel(CDrawPort* pdp, FLOAT fMinX, FLOAT fMinY, FLOAT fMaxX, FLOAT fMaxY, INDEX iType, FLOAT fDistance, FLOAT fHeight)
	{
		if ( iType == HUD_CHARACTER_TYPE )
		{
			HUD_DrawSkaModel(pdp, &m_HUDMLData, fMinX, fMinY, fMaxX, fMaxY, fDistance, fHeight);
		}
		else if ( iType == HUD_ITEM_TYPE )
		{
			HUD_DrawSkaModel(pdp, &m_HUDItemData, fMinX, fMinY, fMaxX, fMaxY, -2.5f, -0.35f );
		}
	}

	virtual void HUD_SetCharModelData(INDEX iJob, INDEX iHairStyle, INDEX iFaceStyle)
	{
		if (iJob < 0 || iJob >= TOTAL_JOB)
		{
			return;
		}

		if (m_HUDMLData.bEnable)
		{
			m_HUDMLData.HUD_DeleteMI();
			m_HUDMLData.Init();
		}

		CEntity* pModel = CLoginJobInfo::getSingleton()->GetLoginModel( (eJob)iJob );

		//m_HUDMLData.hud_MI.SetSkaModel(LoginJobInfo().GetFileName(iJob));
		m_HUDMLData.HUD_CopyMI(pModel->GetModelInstance());
		//	김영환

		SetCharacterAppearance(m_HUDMLData.hud_MI.GetModelInstance(), iJob, iHairStyle, iFaceStyle);
		
		m_HUDMLData.hud_MI.GetModelInstance()->mi_ERType = ERS_NORMAL;
		m_HUDMLData.hud_ItemEffect.SetItemERSubType(ERS_NORMAL);
		m_HUDMLData.hud_MI.AddAnimation(ska_GetIDFromStringTable(CJobInfo::getSingleton()->GetAnimationName(iJob, ANIM_LOGIN_IDLE01)),
			AN_LOOPING|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);

		m_HUDMLData.hud_MI.GetModelInstance()->RefreshTagManager();
		m_HUDMLData.hud_MI.GetModelInstance()->m_tmSkaTagManager.SetOwner(&m_HUDMLData.hud_MI);
				
		m_HUDMLData.HUD_SetItemEffect();
		m_HUDMLData.bEnable = TRUE;
	}
	
	virtual void HUD_ChangeHairMesh(INDEX iJob, INDEX iHairStyle)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh(m_HUDMLData.hud_MI.GetModelInstance(), iJob, iHairStyle - 1);
	}

	virtual void HUD_ChangeFaceMesh(INDEX iJob, INDEX iFaceStyle)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeFaceMesh(m_HUDMLData.hud_MI.GetModelInstance(), iJob, iFaceStyle - 1);
	}

	virtual void HUD_PlayAnimation(INDEX iJob, INDEX AnimID)
	{
		m_HUDMLData.hud_MI.AddAnimation(ska_GetIDFromStringTable(CJobInfo::getSingleton()->GetAnimationName(iJob, AnimID)),
			AN_REMOVE_ON_END, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
	}

	virtual void HUD_PlayAnimation(CTString strAniName)
	{
		m_HUDMLData.hud_MI.AddAnimation(ska_GetIDFromStringTable(strAniName),
			AN_REMOVE_ON_END, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
	}

	virtual void HUD_SetModelColor(const COLOR ModelColor)
	{
		m_HUDMLData.hud_MI.SetModelColor(ModelColor);
	}

	virtual void HUD_SetModelData( INDEX iType, INDEX iIndex )
	{
		if ( iType == HUD_CHARACTER_TYPE )
		{
			if (m_HUDMLData.bEnable)
			{
				m_HUDMLData.HUD_DeleteMI();
				m_HUDMLData.Init();
			}
			
			m_HUDMLData.hud_MI.SetSkaModel(CJobInfo::getSingleton()->GetFileName(_pNetwork->MyCharacterInfo.job));
			
			//	김영환
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeFaceMesh(m_HUDMLData.hud_MI.GetModelInstance(), _pNetwork->Get_MyChar_Job(), _pNetwork->Get_MyChar_faceStyle() - 1);
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh(m_HUDMLData.hud_MI.GetModelInstance(), _pNetwork->Get_MyChar_Job(), _pNetwork->Get_MyChar_hairStyle() - 1);
			//((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeFaceMesh(m_HUDMLData.hud_MI.GetModelInstance(), _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.faceStyle - 1);
			//((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh(m_HUDMLData.hud_MI.GetModelInstance(), _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.hairStyle - 1);
			m_HUDMLData.hud_MI.GetModelInstance()->mi_ERType = ERS_CASHSHOP;
			m_HUDMLData.hud_ItemEffect.SetItemERSubType(ERS_CASHSHOP);
			m_HUDMLData.hud_MI.AddAnimation(ska_GetIDFromStringTable(CJobInfo::getSingleton()->GetAnimationName(_pNetwork->MyCharacterInfo.job, ANIM_IDLE)),
				AN_LOOPING|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);

			m_HUDMLData.HUD_CopyItemEffect(&_pNetwork->MyCharacterInfo.itemEffect);
			m_HUDMLData.bEnable = TRUE;
		}
		else if ( iType == HUD_ITEM_TYPE )
		{
			if ( iIndex <= 0 )
			{
				return;
			}
			
			CItemData* pItemData = _pNetwork->GetItemData( iIndex );
			CTString strSMC		= pItemData->GetItemSmcFileName();
			
			if (m_HUDItemData.bEnable)
			{
				m_HUDItemData.HUD_DeleteMI();
				m_HUDItemData.Init();
			}

			// real code
			CTString aniName = "matryoshka";
			INDEX aniIndex   = ska_StringToID(aniName);
			m_HUDItemData.hud_MI.SetSkaModel( strSMC );
			m_HUDItemData.hud_MI.StretchModel( FLOAT3D(1.5f,1.5f,1.5f) );
			m_HUDItemData.iAnim = aniIndex;

			m_HUDItemData.hud_MI.GetModelInstance()->mi_bDummyModel = TRUE;
			m_HUDItemData.hud_MI.GetModelInstance()->mi_ERType = ERS_MATRYOSHKA;
			m_HUDItemData.hud_MI.AddAnimation(aniIndex, AN_LOOPING|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_HUDItemData.bEnable = TRUE;
		}
		
	}

	virtual FLOAT GetAnimationTime()
	{
		//CTString aniName = JobInfo().GetAnimationName(_pNetwork->MyCharacterInfo.job, ANIM_ATTACK_1);
		CTString aniName = "matryoshka";
		INDEX aniIndex   = ska_StringToID(aniName);
		FLOAT animTime	 = m_HUDItemData.hud_MI.GetModelInstance()->GetAnimLength(aniIndex);
		//return m_HUDItemData.hud_MI.GetModelInstance()->GetAnimLength(ska_GetIDFromStringTable(JobInfo().GetAnimationName(_pNetwork->MyCharacterInfo.job, ANIM_ATTACK_1)));
		return animTime;
	}
	// iType == wearpos, iItem == itemIdx (0일경우 디폴트아머)
	virtual void HUD_WearingModel(INDEX iType,INDEX iJob, INDEX iItem, BOOL bLogin)
	{		
		INDEX iRef = -1;

		if (iType == WEAR_WEAPON){
			iRef = 0;
		}else if (iType == WEAR_HELMET){
			iRef = 1;
		}else if (iType == WEAR_JACKET){
			iRef = 2;
		}else if (iType == WEAR_PANTS){
			iRef = 3;
		}else if (iType == WEAR_GLOVES){
			iRef = 4;
		}else if (iType == WEAR_BOOTS){
			iRef = 5;
		}else if (iType == WEAR_SHIELD){
			iRef = 6;
		}else {
			return;
		}

		if (bLogin == TRUE)
		{
			m_HUDMLData.SetLoginWearingItems(iRef, iType, iJob, iItem);
		}
		else
		{
			m_HUDMLData.SetWearingItems(iRef, iType, iJob, iItem);
		}		
	}

	virtual CModelInstance* GetPlayerModelInstance( CEntity* pEntity )
	{
		if( pEntity )
		{
			if( pEntity->IsPlayer() )
			{
				return GetCurrentPlayerModelInstance();
			}
			else if( pEntity->IsCharacter() )
			{
				return ((CCharacter*)pEntity)->GetCurrentPlayerModelInstance();
			}			
		}
		return NULL;
	}

	// NOTE : 애완동물을 타고 있을때 문제가 되는 부분은 이 함수로 모델인스턴스를 얻어올것.
	CModelInstance* GetCurrentPlayerModelInstance()
	{
		CModelInstance *pMI = NULL;
		if( _pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide )
		{
			CModelInstance *pMITemp = GetModelInstance();

			INDEX ctmi = pMITemp->mi_cmiChildren.Count();
			if( ctmi > 0 )
			{
				pMI = &pMITemp->mi_cmiChildren[0];
			}
			else
			{
				pMI	= GetModelInstance();
			}
		}
		else
		{
			pMI	= GetModelInstance();
		}
		return pMI;
	};

	virtual void EnemyTargetSelected_InputTab()
	{
		CEntity* tmpEntity = NULL;
		
		if (GetPlayerWeapons()->m_penReservedTarget != NULL && !_pNetwork->ga_World.m_vectorTabTargetEntities.empty())
		{
			std::vector<CEntityPointer>::iterator Itr;
			Itr = std::find(_pNetwork->ga_World.m_vectorTabTargetEntities.begin(), _pNetwork->ga_World.m_vectorTabTargetEntities.end(), GetPlayerWeapons()->m_penReservedTarget);

			if (Itr != _pNetwork->ga_World.m_vectorTabTargetEntities.end())
			{ // 찾았으면
				for (++Itr;Itr != _pNetwork->ga_World.m_vectorTabTargetEntities.end(); ++Itr)
				{
					CEntityPointer ItrEntity = (*Itr);

					if (!(ItrEntity.ep_pen->GetFlags() & ENF_ALIVE) || ItrEntity.ep_pen->GetFlags() & ENF_INVISIBLE || ItrEntity.ep_pen->GetFlags() & ENF_HIDDEN)
					{
						continue;
					}

					if ( ( (CUnit*)ItrEntity.ep_pen )->m_nMobDBIndex == 1255 )
					{
						continue;
					}

					tmpEntity = ItrEntity.ep_pen;
					break;
				}
			}
		}

		if (tmpEntity == NULL)
		{
			_pNetwork->ga_World.m_vectorTabTargetEntities.clear();	
			_pNetwork->ga_World.SearchSectorAroundEntity(this, prPlayerProjection->ViewerPlacementR().pl_PositionVector, prPlayerProjection);
			if (!_pNetwork->ga_World.m_vectorTabTargetEntities.empty())
			{
				_pNetwork->ga_World.TargetSelectedEnemy_Sort();
				
				int nSize = _pNetwork->ga_World.m_vectorTabTargetEntities.size();
				for (int i = 0; i < nSize; i++)
				{
					CEntity* CheckEntity = NULL;
					CheckEntity = _pNetwork->ga_World.m_vectorTabTargetEntities[i];

					if (CheckEntity == NULL)
					{
						continue;
					}

					if (((CUnit*)CheckEntity)->m_nMobDBIndex == 1255)
					{
						continue;
					}

					tmpEntity = CheckEntity;
					break;
				}
			}
		}

		if (tmpEntity != NULL)
		{
			FLOAT maxHp = ((CUnit*)tmpEntity)->m_nMaxiHealth;
			FLOAT CurrentHealth = ((CUnit*)tmpEntity)->m_nCurrentHealth;
			int Level = ((CUnit*)tmpEntity)->m_nMobLevel;
			int mobIdx = ((CUnit*)tmpEntity)->m_nMobDBIndex;

			if (mobIdx == 1255) // 보물이 숨겨진 돌 NPC Tab 키로 타겟팅 되지 않도록 추가
			{
				return;
			}

			GetPlayerWeapons()->m_penReservedTarget = tmpEntity;
			bool bRet = tmpEntity->SetTargetInfo(maxHp, CurrentHealth, FALSE, Level, 0,0,0, mobIdx);

			if (bRet == true)
			{
				tmpEntity->SetTargetSyndicateInfo(tmpEntity->en_pMobTarget->mob_iSyndicateType, tmpEntity->en_pMobTarget->mob_iSyndicateGrade);
			}
		}
	}

	virtual void EvocationStart( LONG lIndex, int nSkillIndex )
	{
		int type = TransformInfo().GetType(nSkillIndex);

		if(lIndex == _pNetwork->MyCharacterInfo.index)
		{
			_pNetwork->MyCharacterInfo.nEvocationIndex = nSkillIndex;

			m_iTransformType	= type;
			m_bTransforming		= TRUE;
			m_bIsTransform		= FALSE;//변신상태라도 다시 변신해야 한다.	
			_pNetwork->Set_MyChar_MorphStatus_EVOCATION_CONVERTING();//MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_EVOCATION_CONVERTING;
			return;
		}

		SLONG chaID = _pNetwork->SearchClientChaIndex(lIndex);
		if(chaID != -1)
		{
			CEntity* penEntity;
			if (_pNetwork->ga_World.EntityExists(chaID, penEntity)) 
			{	
				((CCharacter*)penEntity)->m_bIsTransform	= FALSE;
				((CCharacter*)penEntity)->m_iTransformType	= type;				
				((CCharacter*)penEntity)->EvocateNow();
			}
		}
	}

	virtual void EvocationStop( LONG lIndex)
	{
		if(lIndex == _pNetwork->MyCharacterInfo.index)
		{
			_pNetwork->MyCharacterInfo.nEvocationIndex = 0;
			m_iTransformType	 = 0;
			const int iStopTransformItem = 522;
			//m_bTransforming		= TRUE;
			//_pNetwork->MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_END;

			ReturnSorcerer();

			CEntity			*penPlEntity;
			penPlEntity = CEntity::GetPlayerEntity( 0 );

			CModelInstance* pMI = penPlEntity->en_pmiModelInstance;

			_pNetwork->MyCharacterInfo.itemEffect.AddHolyWaterEffect(_pNetwork->MyCharacterInfo.iHitEffectType,
				&(pMI->m_tmSkaTagManager));
			_pNetwork->MyCharacterInfo.itemEffect.RefreshPetStashEffect(&(pMI->m_tmSkaTagManager));
			_pNetwork->MyCharacterInfo.itemEffect.RefreshRelicEffect(&(pMI->m_tmSkaTagManager));
			return;		
		}

		SLONG chaID = _pNetwork->SearchClientChaIndex(lIndex);
		if(chaID != -1)
		{
			CEntity* penEntity;
			if (_pNetwork->ga_World.EntityExists(chaID, penEntity)) 
			{	
				((CCharacter*)penEntity)->m_iTransformType	= 0;
				((CCharacter*)penEntity)->EvocateNow();				
			}
		}
	}

	//----------------------------------------------------------------	
	virtual void Read_net_Mob(int index, CNetworkMessage *istr) 
	{
		FLOAT3D vDesiredPosition;
		ANGLE3D DummyAngle;
		CEntity* penEntity;

		switch(index)
		{		
		// FIXME : 타겟만 정렬될 수 있다면.
		// FIXME : 하나로 묶일 수가 있는 부분임.
		// FIXME : MSG_MOVE와 관련되서 중복되는 부분이 엄청 많음.
		case MSG_MOVE:
			{
				ResponseClient::moveMsg* pPack = reinterpret_cast<ResponseClient::moveMsg*>(istr->GetBuffer());

				vDesiredPosition(1) = pPack->x;
				vDesiredPosition(2) = pPack->h+1;
				vDesiredPosition(3) = pPack->z;

				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, pPack->charIndex);

				if (pObject != NULL)
				{
					CMobTarget* pMT = static_cast< CMobTarget* >(pObject);					
										
					if (_pNetwork->ga_World.EntityExists(pMT->GetCIndex(), penEntity))
						{
						pMT->SetyLayer( pPack->cYlayer );

							if (pPack->moveType == MSG_MOVE_PLACE)
							{
								((CCharacterBase*)pMT->GetEntity())->StopNow();

								CPlacement3D place(vDesiredPosition, ANGLE3D(0, 0, 0));								
								((CUnit*)penEntity)->SetPlacement(place);
								return;
							}

							if( !(pPack->mapAttr & MATT_UNWALKABLE) )
							{
								pMT->mob_sbAttributePos = pPack->mapAttr;
							}

							((CUnit*)penEntity)->m_vDesiredPosition = vDesiredPosition;
							if(pPack->moveType==MSG_MOVE_RUN || pPack->moveType==MSG_MOVE_WALK)
							{
								//ttos_080422 : 이속 감소시 스피트값이 음수 값이 올 수 있으므로 스피드 제한 걸어줌 
								// 이렇게 안하면 MovableEntity.es에서 GetRelativeHeading()함수 내에서 계산시 flow 발생함
								if (pPack->speed < 0 )
								{
									pPack->speed = 0.1f;
								}

								if (pPack->moveType == MSG_MOVE_WALK)
								{
									((CUnit*)penEntity)->m_fMoveSpeed		= pPack->speed;
								}
								else
								{
									((CUnit*)penEntity)->m_fMoveSpeed		= pPack->speed;
								}

								// 혹시 몰라서, 싱글던젼에서만 속도가 적용되도록...
								if( _pNetwork->m_bSingleMode )
								{
									((CUnit*)penEntity)->m_fWalkSpeed		= pPack->speed;
									((CUnit*)penEntity)->m_fMoveSpeed		= pPack->speed;
									((CUnit*)penEntity)->m_fCloseRunSpeed	= pPack->speed;
									((CUnit*)penEntity)->m_fAttackRunSpeed	= pPack->speed;
								}								
							}
							else if(pPack->moveType == MSG_MOVE_STOP)
							{
								((CUnit*)penEntity)->m_fMoveSpeed = 5.0f;
								((CUnit*)penEntity)->StopandTeleport();//1117
							}

							((CUnit*)penEntity)->MoveNow();
						}
					}
				}
			break;	
	
		default: 
			break;
		}
	}

	virtual void Read_net_Pet(int index, CNetworkMessage *istr) 
	{
		FLOAT3D vDesiredPosition;		
		CEntity* penEntity = NULL;		
		CPlacement3D plPlacement;
		ObjInfo* pInfo = ObjInfo::getSingleton();

		switch(index)
		{
			// FIXME : 타겟만 정렬될 수 있다면.
			// FIXME : 하나로 묶일 수가 있는 부분임.
			case MSG_MOVE:
			{
				ResponseClient::moveMsg* pPack = reinterpret_cast<ResponseClient::moveMsg*>(istr->GetBuffer());
				
				vDesiredPosition(1) = pPack->x;
				vDesiredPosition(2) = pPack->h+1;
				vDesiredPosition(3) = pPack->z;

				// 내가 장착한 펫도 MSG_MOVE가 오는데,
				// 이 펫은 MSG_MOVE로 움직이면 안된다.
				if( pPack->charIndex != pInfo->GetMyPetInfo()->lIndex )
				{
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_PET, pPack->charIndex);

					if (pObject != NULL)
					{
						CPetTarget* pTarget = static_cast< CPetTarget* >(pObject);						

						penEntity = pTarget->GetEntity();

						if (penEntity != NULL)
						{
							pTarget->SetyLayer( pPack->cYlayer );
								
							if (pPack->moveType == MSG_MOVE_PLACE)
							{
								// 펫의 경우 따라 오기때문에 멈추면 안됨.
								//((CCharacterBase*)pTarget->pet_pEntity)->StopNow();
								((CUnit*)penEntity)->m_fMoveSpeed		= pPack->speed;

								CPlacement3D place(vDesiredPosition, ANGLE3D(0, 0, 0));								
								((CUnit*)penEntity)->SetPlacement(place);								
								return;
							}
							if ( !(pPack->mapAttr & MATT_UNWALKABLE) )
							{
								pTarget->pet_sbAttributePos = pPack->mapAttr;
							}

							// 국내의 경우에는 공성지역에서만 펫이 안보이도록.
							if( g_iCountry == KOREA )
							{
								if( pPack->mapAttr & MATT_WAR)
								{
									// 해당 엔티티가 사라져야 한다면...
									if( pInfo->GetTargetEntity(eTARGET) == penEntity )
									{
										pInfo->TargetClear(eTARGET);										
									}

									if( pInfo->GetTargetEntity(eTARGET_REAL) == penEntity )
									{
										pInfo->TargetClear(eTARGET_REAL);
									}

									penEntity->SetFlagOn(ENF_HIDDEN);
								}
								else
								{
									penEntity->SetFlagOff(ENF_HIDDEN);
								}
							}
							else
							{
								if( pPack->mapAttr & MATT_PEACE || pPack->mapAttr & MATT_WAR)
								{
									// 해당 엔티티가 사라져야 한다면...
									if( pInfo->GetTargetEntity(eTARGET) == penEntity )
									{
										pInfo->TargetClear(eTARGET);										
									}

									if( pInfo->GetTargetEntity(eTARGET_REAL) == penEntity )
									{
										pInfo->TargetClear(eTARGET_REAL);
									}

									penEntity->SetFlagOn(ENF_HIDDEN);
								}
								else
								{
									penEntity->SetFlagOff(ENF_HIDDEN);
								}
							}

							((CUnit*)penEntity)->m_vDesiredPosition = vDesiredPosition;
							if(pPack->moveType==MSG_MOVE_RUN || pPack->moveType==MSG_MOVE_WALK) //캐릭터 이동.
							{
								//ttos_080422 : 이속 감소시 스피트값이 음수 값이 올 수 있으므로 스피드 제한 걸어줌 
								// 이렇게 안하면 MovableEntity.es에서 GetRelativeHeading()함수 내에서 계산시 flow 발생함
								if (pPack->speed < 0 )
								{
									pPack->speed = 0.1f;
								}

								((CUnit*)penEntity)->m_fMoveSpeed		= pPack->speed;
								((CUnit*)penEntity)->m_fWalkSpeed		= pPack->speed;									
								((CUnit*)penEntity)->m_fCloseRunSpeed	= pPack->speed;
								((CUnit*)penEntity)->m_fAttackRunSpeed	= pPack->speed;
								//((CPetBase*)penEntity)->MoveNow();
							}
							else if(pPack->moveType == MSG_MOVE_STOP) //캐릭터 정지
							{								
								((CUnit*)penEntity)->m_fMoveSpeed = 5.0f;									
								((CUnit*)penEntity)->StopandTeleport();
							}
							((CUnit*)penEntity)->MoveNow();															
						}
					}
				}
				// 내 펫의 경우.
				else if( pPack->charIndex == pInfo->GetMyPetInfo()->lIndex )
				{
					if(pInfo->GetMyPetInfo()->pen_pEntity)
					{
						// 국내의 경우에는 공성지역에서만 펫이 안보이도록.
						if( g_iCountry == KOREA )
						{
							if( pPack->mapAttr & MATT_WAR)
							{
								// 해당 엔티티가 사라져야 한다면...
								if( pInfo->GetTargetEntity(eTARGET) == penEntity )
								{
									pInfo->TargetClear(eTARGET);										
								}

								if( pInfo->GetTargetEntity(eTARGET_REAL) == penEntity )
								{
									pInfo->TargetClear(eTARGET_REAL);
								}
								pInfo->GetMyPetInfo()->pen_pEntity->SetFlagOn(ENF_HIDDEN);
							}
							else
							{
								pInfo->GetMyPetInfo()->pen_pEntity->SetFlagOff(ENF_HIDDEN);
							}
						}
						else
						{
							if( pPack->mapAttr & MATT_PEACE || pPack->mapAttr & MATT_WAR)
							{
								// 해당 엔티티가 사라져야 한다면...
								if( pInfo->GetTargetEntity(eTARGET) == penEntity )
								{
									pInfo->TargetClear(eTARGET);										
								}

								if( pInfo->GetTargetEntity(eTARGET_REAL) == penEntity )
								{
									pInfo->TargetClear(eTARGET_REAL);
								}
								pInfo->GetMyPetInfo()->pen_pEntity->SetFlagOn(ENF_HIDDEN);
							}
							else
							{
								pInfo->GetMyPetInfo()->pen_pEntity->SetFlagOff(ENF_HIDDEN);
							}
						}
					}

					if (pPack->moveType == MSG_MOVE_PLACE)
					{
						//((CCharacterBase*)pInfo->_PetTargetInfo.pen_pEntity)->StopNow();
						((CUnit*)pInfo->GetMyPetInfo()->pen_pEntity)->m_fMoveSpeed		= pPack->speed;						

						CPlacement3D place(vDesiredPosition, ANGLE3D(0, 0, 0));
						((CUnit*)pInfo->GetMyPetInfo()->pen_pEntity)->SetPlacement(place);
						return;
					}
				}
				break;
			}			
			
			// 펫 사교동작
			case MSG_EX_PET_COMMAND:
			{
				LONG	lPetIndex;
				LONG	lCommandSkillIndex;
				SBYTE	sbTargetType;
				LONG	lTargetIndex;

				(*istr) >> lPetIndex;
				(*istr) >> lCommandSkillIndex;
				(*istr) >> sbTargetType;
				(*istr) >> lTargetIndex;

				CEntity *penEntity = NULL;
				if( _pNetwork->SearchEntityByNetworkID( lPetIndex, MSG_CHAR_PET, penEntity ) )
				{
					((CUnit*)penEntity)->m_nPlayActionNum = (SLONG)lCommandSkillIndex;
					if( lPetIndex != pInfo->GetMyPetInfo()->lIndex )
					{
						//((CPet*)penEntity)->GetModelInstance()->NewClearState(0.1f);
					}
					((CUnit*)penEntity)->ActionNow();
					CSkill &rSkillData = _pNetwork->GetSkillData( lCommandSkillIndex );
					PetStartEffectGroup( rSkillData.GetFireEffect1(0), lPetIndex );				
				}
			}
			break;
		}
	}

	virtual void Read_net_WildPet(int index, CNetworkMessage *istr) 
	{
		FLOAT3D vDesiredPosition;		
		CEntity* penEntity = NULL;		
		CPlacement3D plPlacement;
				
		switch(index)
		{
			case MSG_SUB_STAT:
				{
				}
				break;

			// FIXME : 타겟만 정렬될 수 있다면.
			// FIXME : 하나로 묶일 수가 있는 부분임.
			case MSG_MOVE:
			{
				ResponseClient::moveMsg* pPack = reinterpret_cast<ResponseClient::moveMsg*>(istr->GetBuffer());
				
				vDesiredPosition(1) = pPack->x; 
				vDesiredPosition(2) = pPack->h+1;
				vDesiredPosition(3) = pPack->z;

				// 내가 장착한 펫도 MSG_MOVE가 오는데,
				// 이 펫은 MSG_MOVE로 움직이면 안된다.
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_WILDPET, pPack->charIndex);

				if (pObject != NULL)
				{
					ObjInfo* pInfo = ObjInfo::getSingleton();
					CWildPetTarget* pTarget = static_cast< CWildPetTarget* >(pObject);
					penEntity = pTarget->GetEntity();
									
					if (pTarget->m_nOwnerIndex != _pNetwork->MyCharacterInfo.index)
					{
						if (penEntity != NULL && penEntity->IsFlagOff(ENF_DELETED))
						{
							pTarget->m_yLayer = pPack->cYlayer;
							
							if ( !(pPack->mapAttr & MATT_UNWALKABLE) )
							{
								pTarget->m_sbAttributePos = pPack->mapAttr;
							}

							// 국내의 경우에는 공성지역에서만 펫이 안보이도록.
							if( g_iCountry == KOREA )
							{
								if( pPack->mapAttr & MATT_WAR)
								{
									// 해당 엔티티가 사라져야 한다면...
									if( pInfo->GetTargetEntity(eTARGET) == penEntity )
									{
										pInfo->TargetClear(eTARGET);										
									}

									if( pInfo->GetTargetEntity(eTARGET_REAL) == penEntity )
									{
										pInfo->TargetClear(eTARGET_REAL);
									}
									penEntity->SetFlagOn(ENF_HIDDEN);
								}
								else
								{
									penEntity->SetFlagOff(ENF_HIDDEN);
								}
							}
							else
							{
								if( pPack->mapAttr & MATT_PEACE /*|| sbAttributePos == ATTC_WAR*/)
								{
										// 해당 엔티티가 사라져야 한다면...
									if( pInfo->GetTargetEntity(eTARGET) == penEntity )
									{
										pInfo->TargetClear(eTARGET);										
									}

									if( pInfo->GetTargetEntity(eTARGET_REAL) == penEntity )
									{
										pInfo->TargetClear(eTARGET_REAL);
									}
									penEntity->SetFlagOn(ENF_HIDDEN);
								}
								else
								{
									penEntity->SetFlagOff(ENF_HIDDEN);
								}
							}

							if (pPack->moveType == MSG_MOVE_PLACE)
							{
								//((CCharacterBase*)pTarget->pet_pEntity)->StopNow();
								((CUnit*)penEntity)->m_fMoveSpeed		= pPack->speed;

								CPlacement3D place(vDesiredPosition, ANGLE3D(0, 0, 0));								
								((CUnit*)penEntity)->SetPlacement(place);								
								return;
							}

							((CUnit*)penEntity)->m_vDesiredPosition = vDesiredPosition;
							if(pPack->moveType==MSG_MOVE_RUN || pPack->moveType==MSG_MOVE_WALK) //캐릭터 이동.
							{
								//ttos_080422 : 이속 감소시 스피트값이 음수 값이 올 수 있으므로 스피드 제한 걸어줌 
								// 이렇게 안하면 MovableEntity.es에서 GetRelativeHeading()함수 내에서 계산시 flow 발생함
								if (pPack->speed < 0 )
								{
									pPack->speed = 0.1f;
								}

								((CUnit*)penEntity)->m_fMoveSpeed		= pPack->speed;
								((CUnit*)penEntity)->m_fWalkSpeed		= pPack->speed;									
								((CUnit*)penEntity)->m_fCloseRunSpeed	= pPack->speed;
								((CUnit*)penEntity)->m_fAttackRunSpeed	= pPack->speed;
								//((CPetBase*)penEntity)->MoveNow();
							}
							else if(pPack->moveType == MSG_MOVE_STOP) //캐릭터 정지
							{								
								((CUnit*)penEntity)->m_fMoveSpeed = 5.0f;									
								((CUnit*)penEntity)->StopandTeleport();
							}
							((CUnit*)penEntity)->MoveNow();															
						}						
					}
					else if (pTarget->GetSIndex() == pPack->charIndex && pTarget->m_nOwnerIndex == _pNetwork->MyCharacterInfo.index)
					{
						if (pPack->speed < 0 )
						{
							pPack->speed = 0.1f;
						}

						if (penEntity == NULL)
						{
							return;
						}

						((CUnit*)penEntity)->m_fMoveSpeed		= pPack->speed;
						((CUnit*)penEntity)->m_fWalkSpeed		= pPack->speed;									
						((CUnit*)penEntity)->m_fCloseRunSpeed	= pPack->speed;
						((CUnit*)penEntity)->m_fAttackRunSpeed	= pPack->speed;
						// 국내의 경우에는 공성지역에서만 펫이 안보이도록.
						if( g_iCountry == KOREA )
						{
							if( pPack->mapAttr & MATT_WAR)
							{
								// 해당 엔티티가 사라져야 한다면...
								if( pInfo->GetTargetEntity(eTARGET) == penEntity )
								{
									pInfo->TargetClear(eTARGET);										
								}

								if( pInfo->GetTargetEntity(eTARGET_REAL) == penEntity )
								{
									pInfo->TargetClear(eTARGET_REAL);
								}
								penEntity->SetFlagOn(ENF_HIDDEN);
							}
							else
							{
								penEntity->SetFlagOff(ENF_HIDDEN);
							}
						}
						else
						{
							if( pPack->mapAttr & MATT_PEACE /*|| sbAttributePos == ATTC_WAR*/)
							{
								// 해당 엔티티가 사라져야 한다면...
								if( pInfo->GetTargetEntity(eTARGET) == penEntity )
								{
									pInfo->TargetClear(eTARGET);										
								}

								if( pInfo->GetTargetEntity(eTARGET_REAL) == penEntity )
								{
									pInfo->TargetClear(eTARGET_REAL);
								}
								penEntity->SetFlagOn(ENF_HIDDEN);
							}
							else
							{
								penEntity->SetFlagOff(ENF_HIDDEN);
							}
						}

					}

					if (pPack->moveType == MSG_MOVE_PLACE)
					{
						// 펫의 경우 나를 따라 오기때문에 멈추면 안됨.
						//((CCharacterBase*)pTarget->pet_pEntity)->StopNow();

						CPlacement3D place(vDesiredPosition, ANGLE3D(0, 0, 0));
						((CUnit*)penEntity)->SetPlacement(place);
						return;
					}
				}
			}
		}		
	}
	
	virtual void Read_net_StatusPC(CNetworkMessage *istr)
	{
		UpdateClient::charStatusPc* pPack = reinterpret_cast<UpdateClient::charStatusPc*>(istr->GetBuffer());
		ObjInfo* pInfo = ObjInfo::getSingleton();
		CEntity* penEntity;
		SQUAD	state = pPack->state;
		SLONG	pkstate = pPack->state2;
		SLONG	CurrentHp, FullHp;

		CurrentHp = pPack->hp;
		FullHp = pPack->maxHp;

		if(pPack->charIndex == _pNetwork->MyCharacterInfo.index)
		{
			_pNetwork->MyCharacterInfo.hp			= pPack->hp; 
			_pNetwork->MyCharacterInfo.maxHP		= pPack->maxHp;
			_pNetwork->MyCharacterInfo.mp			= pPack->mp; 
			_pNetwork->MyCharacterInfo.maxMP		= pPack->maxMp; 
			_pNetwork->MyCharacterInfo.pktitle		= pPack->pkName; 
			_pNetwork->MyCharacterInfo.pkpenalty	= pPack->pkPenalty; 


			// state와 state2 값을 이용해 이곳에서 마법 부여에 대한 이펙트를 추가한다.
			// 이펙트 관련 함수 하나 만들고, 그 안에서 state2에 해당하는 이펙트 추가한다.
			if(en_pmiModelInstance)
			{
				if(IS_FLAG_ON(state, EST_ASSIST_FAKEDEATH))
				{
					((CPlayerAnimator&)*m_penAnimator).m_bDisableAnimating = TRUE;
				}
				else
				{
					((CPlayerAnimator&)*m_penAnimator).m_bDisableAnimating = FALSE;
				}

				// NOTE : 상태 이펙트가 탈 것에는 안 먹으면 이상하지 않을까??? 한번 확인해볼것.
				//CModelInstance* pMI = GetCurrentPlayerModelInstance();
				CModelInstance* pMI = GetModelInstance();
				_pNetwork->MyCharacterInfo.statusEffect.ChangeStatus(&pMI->m_tmSkaTagManager, state, CStatusEffect::R_PARTY);
			}
			else
			{
				ASSERTALWAYS("Player must have ska model and tag manager.");
			}

			// NOTE : 플레이어의 버프 처리를 위한 부분.
			if( _pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_BLIND) )
			{
				LostTarget();
			}

			if(_pNetwork->MyCharacterInfo.statusEffect.IsSturn()
				|| _pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_STONE)
				|| _pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_SLEEP)
				|| _pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_FREEZE)
				)
			{
				StopMoveNoSendStopMsg();
				m_bStuned = TRUE;
			}
			else {m_bStuned = FALSE;}

			if(_pNetwork->MyCharacterInfo.statusEffect.IsHold())
			{
				StopMoveNoSendStopMsg();
				m_bHold = TRUE;
			}
			else {m_bHold = FALSE;}

			if(_pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_SILENT)) {m_bCannotUseSkill = TRUE;}
			else {m_bCannotUseSkill = FALSE;}

			return;
		}

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pPack->charIndex);

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			pTarget->SetChaPkState(pkstate);

			penEntity = pTarget->GetEntity();
			//if (_pNetwork->ga_World.EntityExists(ct.m_nIdxClient, penEntity)) 
			{
				((CUnit*)((CEntity*) penEntity))->m_nCurrentHealth = CurrentHp;
				if(pInfo->GetTargetEntity(eTARGET) == penEntity)//타겟팅이 되어있다면...
				{
					penEntity->UpdateTargetInfo(FullHp,CurrentHp,
						((CCharacter*)((CEntity*) penEntity))->m_nPkMode,
						((CCharacter*)((CEntity*) penEntity))->m_nPkState,
						((CCharacter*)((CEntity*) penEntity))->m_nLegit);
				}

				if(pInfo->GetTargetEntity(eTARGET_REAL) == penEntity)
				{
					penEntity->SetTargetInfoReal(FullHp,CurrentHp,
						0,
						((CCharacter*)((CEntity*) penEntity))->m_nPkMode, 
						((CCharacter*)((CEntity*) penEntity))->m_nPkState, 
						0,
						((CCharacter*)((CEntity*) penEntity))->m_nLegit, 
						pTarget
						);

					// 보이지 않는 스킬인 경우 타겟인포를 비활성시킨다 [11/10/13 trylord]
					if( IS_FLAG_ON(state, EST_ASSIST_INVISIBLE) )
					{
						pInfo->SetTargetActive(eTARGET_REAL, FALSE);
					}
					else
					{
						pInfo->SetTargetActive(eTARGET_REAL, TRUE);
					}
				}

				CUIManager* pUIManager = SE_Get_UIManagerPtr();

				GAMEDATAMGR()->GetPartyInfo()->UpdateMemberStatus( pPack->charIndex, CurrentHp, FullHp, pPack->mp, pPack->maxMp );

				if(pTarget->cha_statusEffect.IsState(EST_ASSIST_INVISIBLE) == FALSE
					&& IS_FLAG_ON(state, EST_ASSIST_INVISIBLE)
					&& _pNetwork->m_ubGMLevel < 2)
				{
					penEntity->SetFlags(penEntity->GetFlags() | ENF_HIDDEN);
					CPlayer *penPlr = (CPlayer*)CEntity::GetPlayerEntity(0);
					if(penPlr->IsSameTarget(penEntity))
					{
						if(penPlr->IsSkilling()) {penPlr->CancelSkill(TRUE, g_iAutoAttack,FALSE); penEntity->InflictDirectDamage(penEntity, penPlr, DMT_NONE, 0, FLOAT3D(0,0,0), ANGLE3D(0,0,0));}
						if(penPlr->IsAttacking()) {penPlr->StopAttack();}
						penPlr->SetTargetNull();
						penPlr->m_bLockMove = FALSE;
					}
					// [2010/08/31 : Sora] 인비저블 상태의 케릭터가 파티타겟이 되지 않도록 수정
					GAMEDATAMGR()->GetPartyInfo()->ClearPartyTarget(pPack->charIndex);	

				}
				else if(pTarget->cha_statusEffect.IsState(EST_ASSIST_INVISIBLE) && !IS_FLAG_ON(state, EST_ASSIST_INVISIBLE))
				{
					penEntity->SetFlags(penEntity->GetFlags()&~ENF_HIDDEN);
					((CCharacter*)penEntity)->SetDesiredRotation(ANGLE3D(0,0,0));
					if(((CCharacter*)penEntity)->m_bIdleAnim)
					{
						((CUnit*)penEntity)->ActionNow();
					}
				}

				if (pTarget->cha_statusEffect.IsState(EST_ASSIST_FAKEDEATH) == FALSE && 
					IS_FLAG_ON(state, EST_ASSIST_FAKEDEATH))
				{
					CPlayer *penPlr = (CPlayer*)CEntity::GetPlayerEntity(0);
					if(penPlr->IsSameTarget(penEntity))
					{
						if(penPlr->IsSkilling()) {penPlr->CancelSkill(TRUE, g_iAutoAttack, FALSE); penEntity->InflictDirectDamage(penEntity, penPlr, DMT_NONE, 0, FLOAT3D(0,0,0), ANGLE3D(0,0,0));}
						if(penPlr->IsAttacking()) {penPlr->StopAttack();}
						penPlr->SetTargetNull();
						penPlr->m_bLockMove = FALSE;
					}

					// [2010/08/31 : Sora] 데스모션 상태의 케릭터가 파티타겟이 되지 않도록 수정
					GAMEDATAMGR()->GetPartyInfo()->ClearPartyTarget(pPack->charIndex);	

				}

				// state와 state2 값을 이용해 이곳에서 마법 부여에 대한 이펙트를 추가한다.
				// 이펙트 관련 함수 하나 만들고, 그 안에서 state2에 해당하는 이펙트 추가한다.
				if(penEntity->en_pmiModelInstance)
				{
					CStatusEffect::eRelation rel = CStatusEffect::R_NONE;

					// 051203 아랫줄 주석 처리.  플래그로 처리.
					//if(pUIManager->GetParty()->IsPartyMember(ct.cha_Index)) {rel = CStatusEffect::R_PARTY;}
					if( pTarget->GetEntity()->IsSecondExtraFlagOn( ENF_EX2_MYPARTY ) ) {rel = CStatusEffect::R_PARTY;}
					pTarget->ChangeStatus(&penEntity->en_pmiModelInstance->m_tmSkaTagManager, state, rel);
				}
				else
				{
					ASSERTALWAYS("Character must have ska model and tag manager.");
				}
			}
		}
	}

	virtual void Read_net_StatusNPC(CNetworkMessage *istr)
	{
		UpdateClient::charStatusNpc* pPack = reinterpret_cast<UpdateClient::charStatusNpc*>(istr->GetBuffer());

		CUIManager* pUIManager = SE_Get_UIManagerPtr();
		ObjInfo* pInfo = ObjInfo::getSingleton();
		CEntity* penEntity;

		// EP2 적용시 각 국가 더할 것.
		BOOL bMercenaryDataRead = FALSE;
		if( g_iCountry != TURKEY && g_iCountry != ITALY)
		{
			// 몬스터 용병 카드
			bMercenaryDataRead = pUIManager->GetMonsterMercenary()->ReceiveMercenaryMessage(istr, pPack->hp, pPack->maxHp, pPack->mp, pPack->maxMp);
		}

		SLONG mob_ClientId = -1;			

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, pPack->charIndex);
		CMobTarget* pMT = NULL;

		if (pObject != NULL)
		{
			pMT = static_cast< CMobTarget* >(pObject);

			mob_ClientId = pMT->GetCIndex();

			// X-mas 2007 eons
			if (pMT->m_nType == MOB_XMAS_TREE_CENTER || pMT->m_nType == MOB_XMAS_TREE_DECO)
			{ //꽃나무, 크리스마스 트리(장식용, 광장용)
				pUIManager->GetFlowerTree()->SetMobFlowerTree(pMT); // 트리 MobTarget정보 저장
				pUIManager->GetFlowerTree()->FlowerTreeUpdate(pPack->hp);
			}
			if( bMercenaryDataRead )
			{
				_pNetwork->MonsterMercenaryInfo.index = pMT->m_nType;
			}
		}

		if(mob_ClientId != -1)
		{
			if (_pNetwork->ga_World.EntityExists(mob_ClientId,penEntity)) //공격자
			{
				((CUnit*)((CEntity*) penEntity))->m_nMaxiHealth = pPack->maxHp;
				((CUnit*)((CEntity*) penEntity))->m_nCurrentHealth = pPack->hp;
				if(pInfo->GetTargetEntity(eTARGET) == penEntity)//타겟팅이 되어있다면...
				{
					penEntity->UpdateTargetInfo(pPack->maxHp,pPack->hp);//1022
				}

				// state와 state2 값을 이용해 이곳에서 마법 부여에 대한 이펙트를 추가한다.
				// 이펙트 관련 함수 하나 만들고, 그 안에서 state2에 해당하는 이펙트 추가한다.
				if(penEntity->en_pmiModelInstance && pMT != NULL)
				{
					// get mob data.
					CMobData* MD = CMobData::getData( pMT->m_nType );
					pMT->ChangeStatus(&penEntity->en_pmiModelInstance->m_tmSkaTagManager, pPack->state);

					if( pMT->mob_statusEffect.IsSturn()
						|| pMT->mob_statusEffect.IsState(EST_ASSIST_STONE)
						|| pMT->mob_statusEffect.IsState(EST_ASSIST_SLEEP) 
						|| pMT->mob_statusEffect.IsState(EST_ASSIST_BLIND)
						|| pMT->mob_statusEffect.IsState(EST_ASSIST_FREEZE))
					{
						((CUnit*)penEntity)->m_bStuned = TRUE;
					}
					else {((CUnit*)penEntity)->m_bStuned = FALSE;}
					if(pMT->mob_statusEffect.IsHold())
					{								
						((CUnit*)penEntity)->m_bHold = TRUE;
					}
					else {((CUnit*)penEntity)->m_bHold = FALSE;}
					if(pMT->mob_statusEffect.IsState(EST_ASSIST_SILENT)) {((CUnit*)penEntity)->m_bCannotUseSkill = TRUE;}
					else {((CUnit*)penEntity)->m_bCannotUseSkill = FALSE;}

					if ( IS_FLAG_ON( pPack->state, EST_ASSIST_INVISIBLE ) )
					{	
						if ( pMT->mob_iOwnerIndex != _pNetwork->MyCharacterInfo.index )
						{
							penEntity->SetFlagOn(ENF_HIDDEN);
						}
					}
					else if ( !IS_FLAG_ON( pPack->state, EST_ASSIST_INVISIBLE ) )
					{
						if( !MD->IsShadowNPC() )
						{
							penEntity->SetFlagOff(ENF_HIDDEN);
						}
					}
				}
				else
				{
					ASSERTALWAYS("Mob must have ska model and tag manager.");
				}
			}
		}
	}

	virtual void Read_net_StatusPet(CNetworkMessage *istr)
	{
		UpdateClient::charStatusPet* pPack = reinterpret_cast<UpdateClient::charStatusPet*>(istr->GetBuffer());
	}
	
	virtual void Read_net_StatusElemental(CNetworkMessage *istr)
	{
		UpdateClient::charStatusElemental* pPack = reinterpret_cast<UpdateClient::charStatusElemental*>(istr->GetBuffer());

		SLONG	clientid;
		CEntity* penEntity;

		clientid = -1;
		CSlaveTarget *pMT = NULL;

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_SLAVE, pPack->charIndex);
		ObjInfo* pInfo = ObjInfo::getSingleton();

		if (pObject != NULL)
		{
			pMT = static_cast< CSlaveTarget* >(pObject);
			clientid = pMT->GetCIndex();
		}

		if(clientid != -1)
		{
			if (_pNetwork->ga_World.EntityExists(clientid,penEntity)) //공격자
			{	
				((CUnit*)((CEntity*) penEntity))->m_nMaxiHealth = pPack->maxHp;
				((CUnit*)((CEntity*) penEntity))->m_nCurrentHealth = pPack->hp;
				if(pInfo->GetTargetEntity(eTARGET) == penEntity)//타겟팅이 되어있다면...
				{
					penEntity->UpdateTargetInfo(pPack->maxHp,pPack->hp);//1022
				}

				// state와 state2 값을 이용해 이곳에서 마법 부여에 대한 이펙트를 추가한다.
				// 이펙트 관련 함수 하나 만들고, 그 안에서 state2에 해당하는 이펙트 추가한다.
				if(penEntity->en_pmiModelInstance && pMT != NULL)
				{
					pMT->ChangeStatus(&penEntity->en_pmiModelInstance->m_tmSkaTagManager, pPack->state);

					if( pMT->slave_statusEffect.IsSturn()
						|| pMT->slave_statusEffect.IsState(EST_ASSIST_STONE)
						|| pMT->slave_statusEffect.IsState(EST_ASSIST_SLEEP) 
						|| pMT->slave_statusEffect.IsState(EST_ASSIST_BLIND)
						|| pMT->slave_statusEffect.IsState(EST_ASSIST_FREEZE))
					{
						((CUnit*)penEntity)->m_bStuned = TRUE;								
					}
					else {((CUnit*)penEntity)->m_bStuned = FALSE;}
					if(pMT->slave_statusEffect.IsHold())
					{								
						((CUnit*)penEntity)->m_bHold = TRUE;
					}
					else {((CUnit*)penEntity)->m_bHold = FALSE;}
					if(pMT->slave_statusEffect.IsState(EST_ASSIST_SILENT)) {((CUnit*)penEntity)->m_bCannotUseSkill = TRUE;}
					else {((CUnit*)penEntity)->m_bCannotUseSkill = FALSE;}
				}
				else
				{
					ASSERTALWAYS("Mob must have ska model and tag manager.");
				}
			}
		}				

	}

	// FIXME : 내가 공격하는 경우만 제외하고, 나머지 경우는 하나로 다 묶을 수 있움.	
	// 데미지를 처리합니다.
	virtual void Read_net_Damage_Character(CNetworkMessage *istr)
	{
		UpdateClient::charDamage* pPack = reinterpret_cast<UpdateClient::charDamage*>(istr->GetBuffer());

		CUIManager* pUIManager = CUIManager::getSingleton();
		
		// 사용 아이템 검사
		int nType = 0;
		
		switch(pPack->holyitemIndex)
		{
		case DEF_HOLYWATER_STRONG:
			nType = 1;
			break;
		case DEF_HOLYWATER_MYSTERY:
			nType = 2;
			break;
		case DEF_HOLYWATER_DAMAGE:
			nType = 3;
			break;
		case DEF_HOLYWATER_SKILLDAMAGE:
			nType = 4;
			break;
		default:
			nType = 0;
			break;
		}

		if (_pNetwork->MyCharacterInfo.index == pPack->charIndex)
		{
			_pNetwork->MyCharacterInfo.iHitEffectType = nType;
		}
		else
		{
			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pPack->charIndex);

			if (pObject != NULL)
			{
				CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);
				pTarget->cha_iHitType = nType;
			}
		}	

		bool bMe = false;
		bMe = _pNetwork->IsAttackMe( pPack->targetIndex ); // TO.DO HP					
		
		if (_pGameState->IsRestartGame() == TRUE && bMe == true)
		{
			pUIManager->GetSystemMenu()->CancelRestart();
		}
		
		// 내가 공격하는 경우?
		if(pPack->charIndex == _pNetwork->MyCharacterInfo.index)
		{
			// 내가 NPC를 공격하는 경우.
			if(pPack->targetType == MSG_CHAR_NPC)
			{
				CEntity *penTargetEntity = NULL;
				if( _pNetwork->SearchEntityByNetworkID( pPack->targetIndex, pPack->targetType, penTargetEntity ) )
				{
					((CUnit*)penTargetEntity)->m_enAttackerID = pPack->charIndex;

					ShowAttackResult(penTargetEntity, (SLONG)pPack->damage, pPack->flag, pPack->skillIndex);

					if(pPack->flag != HITTYPE_MISS)
					{
						NickNameEffect(this, penTargetEntity, NICKNAME_ATTACK_EFFECT);
					}

					if (pUIManager->m_DamageLogInfo.GetStart())
					{
						CDamageLogInfo::LogDamage logDamage;
						logDamage.att_EntityInfo.ServerID = _pNetwork->MyCharacterInfo.index;
						logDamage.att_EntityInfo.ClientID = en_ulID;
						logDamage.att_EntityInfo.level = _pNetwork->MyCharacterInfo.level;
						logDamage.att_EntityInfo.db_Index = -1;
						strcpy(logDamage.att_EntityInfo.strName, _pNetwork->MyCharacterInfo.GetPlayerName());
						logDamage.att_EntityInfo.hp = _pNetwork->MyCharacterInfo.hp;
						logDamage.att_EntityInfo.mp = _pNetwork->MyCharacterInfo.mp;

						logDamage.tar_EntityInfo.ServerID = pPack->targetIndex;
						logDamage.tar_EntityInfo.ClientID = penTargetEntity->en_ulID;
						logDamage.tar_EntityInfo.level = penTargetEntity->en_pMobTarget->mob_iLevel;
						logDamage.tar_EntityInfo.db_Index = penTargetEntity->en_pMobTarget->m_nType;
						//logDamage.tar_EntityInfo.strName = penTargetEntity->en_pMobTarget->mob_Name;
						strcpy(logDamage.tar_EntityInfo.strName, penTargetEntity->GetName());
						logDamage.tar_EntityInfo.hp = pPack->targetHp;
						logDamage.tar_EntityInfo.mp = pPack->targetMp;
						logDamage.tar_EntityInfo.prehp = ((CUnit*)((CEntity*) penTargetEntity))->m_nCurrentHealth;
						logDamage.tar_EntityInfo.premp = pPack->targetMp;

						logDamage.dam_Type = pPack->flag;
						logDamage.dam_Amount = pPack->damage;

						pUIManager->m_DamageLogInfo.AddDamage(logDamage);
					}

					// 현재의 HP를 갱신함.
					// TO.DO HP
					if( bHp_Percentage )
					{
						if ( bMe == false )
						{
							((CUnit*)((CEntity*) penTargetEntity))->m_nPreHealth = pPack->targetHp;
							((CUnit*)((CEntity*) penTargetEntity))->m_nMaxiHealth = 1000;										}									
					}
					else
					{
						((CUnit*)((CEntity*) penTargetEntity))->m_nPreHealth = pPack->targetHp;
					}


					CPrintF("------%f--Damage Message\n", _pTimer->GetLerpedCurrentTick());

					if(pPack->targetHp <= 0)
					{
						_UIAutoHelp->SetInfo ( AU_MOB_KILL );
						_UIAutoHelp->SetKillMonIndex ( pPack->targetIndex );
						((CUnit*)penTargetEntity)->DeathNow();

						if(IsSameTarget(penTargetEntity))
						{
							INFO()->TargetClear(eTARGET);
						}

						if (pUIManager->m_DamageLogInfo.GetStart())
						{
							pUIManager->m_DamageLogInfo.EndDamageMode();
							_UIAutoHelp->SetGMNotice ( "데미지를 기록하는 모드를 종료합니다.", 0xFFAA33FF );
						}
					}
					return;
				}
				return;
			}
			// 내가 기타 다른 엔티티를 공격하는 경우.
			else
			{
				CEntity *penTargetEntity = NULL;
				if( _pNetwork->SearchEntityByNetworkID( pPack->targetIndex, pPack->targetType, penTargetEntity ) )
				{						
					((CUnit*)penTargetEntity)->m_enAttackerID = pPack->charIndex;
					// 타 캐릭터를 공격한 경우.
					if(pPack->targetType == MSG_CHAR_PC )//pvp징보는 운영자만 볼수있게 했다.
					{
						if( _pNetwork->m_ubGMLevel > 1 )
						{
							// 타겟의 데미지 정보를 표시함.
							ShowAttackResult(penTargetEntity, (SLONG)pPack->damage, pPack->flag, pPack->skillIndex);
						}

						if(pPack->flag != HITTYPE_MISS)
						{
							NickNameEffect(this, penTargetEntity, NICKNAME_ATTACK_EFFECT);
						}
					}
					else
					{
						// 타겟의 데미지 정보를 표시함.
						ShowAttackResult(penTargetEntity, (SLONG)pPack->damage, pPack->flag, pPack->skillIndex);

						if(pPack->flag != HITTYPE_MISS)
						{
							NickNameEffect(this, penTargetEntity, NICKNAME_ATTACK_EFFECT);
						}
					}

					// 현재의 HP를 갱신함.
					if( bHp_Percentage )
					{
						if( bMe == false ) // TO.DO HP
						{
							((CUnit*)((CEntity*) penTargetEntity))->m_nPreHealth = pPack->targetHp;
						}
					}
					else
					{
						((CUnit*)((CEntity*) penTargetEntity))->m_nPreHealth = pPack->targetHp;
					}

					if(pPack->targetHp <= 0)
					{							
						((CUnit*)penTargetEntity)->DeathNow();//1231
					}

					if( bMe == false)
					{
						UpdateUnitInfo( penTargetEntity, pPack->targetIndex, pPack->targetHp );
					}

					return;							
				}
				return;
			}
			return;
		}

		// FIXME : 이렇게 중첩되두 되는거야?ㅡ.ㅡ
		if(pPack->damageType != MSG_DAMAGE_REFLEX && pPack->damageType != MSG_DAMAGE_LINK)
		{
			CEntity *penEntity = NULL;
			
			m_iEnermyID = pPack->charIndex;

			if( _pNetwork->SearchEntityByNetworkID( pPack->charIndex, MSG_CHAR_PC, penEntity ) )
			{
				// 본인 캐릭의 경우.
				if(pPack->targetType == MSG_CHAR_PC)
				{
					//PVP일 경우
					if(pPack->targetIndex == _pNetwork->MyCharacterInfo.index)
					{
						pUIManager->AddDamageData( pPack->damage, pPack->flag, en_ulID, TRUE );
						if(!((CUnit*)penEntity)->m_bSkilling)							
						{
							((CUnit*)penEntity)->SetTargetEntity(this);
							((CUnit*)penEntity)->SetAttackSpeed(pPack->attackSpeed);
							((CUnit*)penEntity)->AttackNow();
						}
						// wooss 050928 PvP시 죽었다는 메시지를 받지 못하여 종료 못하는 문제로 추가
						if(pPack->targetHp <= 0)
						{
							DeathYou();
							return;
						}

						if(pPack->flag != HITTYPE_MISS)
						{
							NickNameEffect(this, penEntity, NICKNAME_DAMAGE_EFFECT);
						}

						if( ((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSitting() && pPack->flag != HITTYPE_MISS )
						{// 앉아 있을 경우 피공격시 일어남
							pUIManager->GetCharacterInfo()->UseAction( 3 );
						}

						return;
					}
				}

				// 플레이어 이외의 다른 것을 공격하는 경우.						
				CEntity *penTargetEntity = NULL;
				if( _pNetwork->SearchEntityByNetworkID( pPack->targetIndex, pPack->targetType, penTargetEntity ) )
				{
					((CUnit*)penTargetEntity)->m_enAttackerID = pPack->charIndex;

					// 타겟이 내 소환수인 경우.
					if( penTargetEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE) )
					{
						pUIManager->AddDamageData( pPack->damage, pPack->flag, penTargetEntity->en_ulID, TRUE );					
					}

					// 타겟이 내 소환수인 경우.
					if( penTargetEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_WILDPET) )
					{
						pUIManager->AddDamageData( pPack->damage, pPack->flag, penTargetEntity->en_ulID, TRUE );					
					}

					// 싱글 모드가 아닐때...
					if(!_pNetwork->m_bSingleMode)
					{
						if(!((CUnit*)penEntity)->m_bSkilling && pPack->skillIndex == -1)
						{
							((CUnit*)penEntity)->SetTargetEntity(penTargetEntity);
							((CUnit*)penEntity)->SetAttackSpeed(pPack->attackSpeed);
							((CUnit*)penEntity)->AttackNow();
							if (penEntity->IsCharacter() && pPack->flag != HITTYPE_MISS)
							{
								NickNameEffect(penEntity, penTargetEntity, NICKNAME_ATTACK_EFFECT);
								NickNameEffect(penEntity, penTargetEntity, NICKNAME_DAMAGE_EFFECT);
							}
							//파티원이 공격하고 있으면 그 파티원의 타겟을 저장...
							if( GAMEDATAMGR()->GetPartyInfo()->IsPartyMember(pPack->charIndex) )
							{
								GAMEDATAMGR()->GetPartyInfo()->SetPartyTarget(pPack->charIndex, pPack->targetIndex, pPack->targetType);
							}
						}
						else	// 스킬 사용 시
						{
							if (penEntity->IsCharacter() && pPack->flag != HITTYPE_MISS)
							{
								NickNameEffect(penEntity, penTargetEntity, NICKNAME_ATTACK_EFFECT);
								NickNameEffect(penEntity, penTargetEntity, NICKNAME_DAMAGE_EFFECT);
							}
						}
					}

					// 내 펫이 공격받는 경우.
					if( penTargetEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_PET) )
					{
						//	김영환 변경
						if(_pNetwork->Get_MyChar_Attack(pPack->targetIndex,pPack->targetHp))
						{
							pUIManager->GetPetInfo()->GetPetDesc();
						}
						//CNetworkLibrary::sPetInfo	TempPet;
						//TempPet.lIndex				= targetID;
						//std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
						//	std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
						//if( iter != _pNetwork->m_vectorPetList.end() )
						//{
						//	(*iter).lHP		= targetHP;
						//	pUIManager->GetPetInfo()->GetPetDesc();
						//}
					}
					if ( bHp_Percentage )
					{
						if( bMe == false ) // TO.DO HP
						{
							((CUnit*)penTargetEntity)->m_nPreHealth = pPack->targetHp;
						}
					}
					else
					{
						((CUnit*)penTargetEntity)->m_nPreHealth = pPack->targetHp;
					}

					if(pPack->targetHp  <= 0)
					{
						((CUnit*)penEntity)->m_bKillEnemy = TRUE;
						((CUnit*)penEntity)->SetKillTargetEntity(penTargetEntity);
						((CUnit*)penTargetEntity)->DeathNow();//0815
						penEntity->UpdateTargetInfo(((CUnit*)penTargetEntity)->m_nMaxiHealth, pPack->targetHp);

					}

					if( bMe == false)
					{
						UpdateUnitInfo( penTargetEntity, pPack->targetIndex, pPack->targetHp );
					}

					return;									
				}
				return; //피해자가 화면에 존재하지 않는다.
			}
			// 공격자가 화면내에 없지만, NPC나 PC가 HP가 0이 되어서 죽어야 하는경우...
			else
			{
				// 공격자가 화면에 없어도 HP가 0인 캐릭터는 죽어야 되기 때문에...
				if(pPack->targetType == MSG_CHAR_PC)
				{
					//PVP일 경우
					if(pPack->targetIndex == _pNetwork->MyCharacterInfo.index)
					{
						if( ((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSitting() && pPack->flag != HITTYPE_MISS )
						{// 앉아 있을 경우 피공격시 일어남
							pUIManager->GetCharacterInfo()->UseAction( 3 );
						}

						return;
					}
				}

				CEntity *penTargetEntity = NULL;
				if( _pNetwork->SearchEntityByNetworkID( pPack->targetIndex, pPack->targetType, penTargetEntity ) )
				{
					// 타겟이 내 소환수인 경우.
					if( penTargetEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE) )
					{
					}
					if ( bHp_Percentage )
					{
						if( bMe == false ) // TO.DO HP
						{
							((CUnit*)penTargetEntity)->m_nPreHealth = pPack->targetHp;
						}
					}
					else
					{
						((CUnit*)penTargetEntity)->m_nPreHealth = pPack->targetHp;	
					}

					if(pPack->targetHp  <= 0)
					{
						((CUnit*)penTargetEntity)->DeathNow();
					}

					if( bMe == false)
					{
						UpdateUnitInfo( penTargetEntity, pPack->targetIndex, pPack->targetHp );
					}
				}
				return;
			}
		}

		// 날 공격하는 경우.
		if(	pPack->targetType == MSG_CHAR_PC && 
			pPack->targetIndex == _pNetwork->MyCharacterInfo.index)
		{
			if( ((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSitting() && pPack->flag != HITTYPE_MISS )
			{// 앉아 있을 경우 피공격시 일어남
				pUIManager->GetCharacterInfo()->UseAction( 3 );
			}

			return;
		}

		// 날 공격하는 경우를 제외하고, 나머지 경우...
		CEntity *penTargetEntity = NULL;
		if( _pNetwork->SearchEntityByNetworkID( pPack->targetIndex, pPack->targetType, penTargetEntity ) )
		{
			((CUnit*)penTargetEntity)->m_enAttackerID = pPack->targetIndex;

			if ( bHp_Percentage )
			{
				if( bMe == false ) // TO.DO HP
				{
					((CUnit*)penTargetEntity)->m_nCurrentHealth = pPack->targetHp;
				}
			}
			else
			{
				((CUnit*)penTargetEntity)->m_nCurrentHealth = pPack->targetHp;						
			}

			// 타겟이 플레이어일 경우.
			if( pPack->targetType == MSG_CHAR_PC )
			{
				StartEffectGroup("Normal Hit", _pTimer->GetLerpedCurrentTick()
					, penTargetEntity->GetPlacement().pl_PositionVector+FLOAT3D(0,1,0.5f)
					, ANGLE3D(90,0,90));
			}
			// 타겟의 기타 다른 엔티티의 경우.
			else
			{
				FLOAT3D posOffset(0,1,0.5f);
				if(penTargetEntity->GetModelInstance())
				{
					FLOATaabbox3D aabb;
					penTargetEntity->GetModelInstance()->GetCurrentColisionBox(aabb);
					FLOAT3D size = aabb.Size()*0.5f;
					posOffset(2) += size(2);
					posOffset(3) += size(3);
				}
				StartEffectGroup("Normal Hit", _pTimer->GetLerpedCurrentTick()
					, penTargetEntity->GetPlacement().pl_PositionVector + posOffset
					, ANGLE3D(90,0,90));
			}

			if(pPack->targetHp <= 0)//0817
			{
				((CUnit*)penTargetEntity)->DeathNow();//0815									
			}

			if( bMe == false)
			{
				UpdateUnitInfo( penTargetEntity, pPack->targetIndex, pPack->targetHp );
			}
			return;					
		}
	}

	virtual void Read_net_DissappearEffect(CNetworkMessage* istr)
	{
		UpdateClient::charDisappearForClientEffect* pPack = reinterpret_cast<UpdateClient::charDisappearForClientEffect*>(istr->GetBuffer());
		ObjectBase* pObj = ACTORMGR()->GetObject(eOBJ_MOB, pPack->charIndex);

		if (pObj != NULL)
		{
			CEntity *penTargetEntity = pObj->GetEntity();

			if (penTargetEntity != NULL)
			{
				((CUnit*)penTargetEntity)->m_nCurrentHealth = 0;
				((CUnit*)penTargetEntity)->DeathNow();
			}
		}
	}
	virtual void Read_net_Damage( SBYTE sbType, CNetworkMessage *istr)
	{
		UpdateClient::charDamage* pPack = reinterpret_cast<UpdateClient::charDamage*>(istr->GetBuffer());
		
		CUIManager* pUIManager = SE_Get_UIManagerPtr();
		ObjInfo* pInfo = ObjInfo::getSingleton();

		bool bMe = false;
		bMe = _pNetwork->IsAttackMe( pPack->targetIndex ); // TO.DO HP					

		if (_pGameState->IsRestartGame() == TRUE && bMe == true)
		{
			pUIManager->GetSystemMenu()->CancelRestart();
		}
		
		CEntity* penEntity = NULL;
		// 공격자를 찾은 경우.
		if( _pNetwork->SearchEntityByNetworkID( pPack->charIndex, sbType, penEntity ) )
		{
			// 싱글던젼에서 몹이 몹을 공격할 경우에 대한 처리.
			if(sbType == MSG_CHAR_NPC && pPack->targetType == MSG_CHAR_NPC && _pNetwork->m_bSingleMode)
			{
				//몹이 몹을 공격 하는 경우.
				CTString strMobName = penEntity->GetName();
				CTString strSysMessage;

				CEntity *penTargetEntity = NULL; // 타겟 대상

				if( _pNetwork->SearchEntityByNetworkID( pPack->targetIndex, pPack->targetType, penTargetEntity ) )
				{ // 찾았다.
					switch(pPack->flag)
					{
					case HITTYPE_MISS:
						strSysMessage.PrintF(_S( 677, "구출 NPC가 공격을 회피했습니다." ));	
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );
						break;
					case HITTYPE_WEAK:
						strSysMessage.PrintF(_S( 678, "구출 NPC가 약한 공격을 받았습니다. " ));		
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );
						break;
					case HITTYPE_NORMAL:
						strSysMessage.PrintF(_S( 679, "구출 NPC가 일반 공격을 받았습니다." ));		
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );
						break;
					case HITTYPE_STRONG:// 강하게
						strSysMessage.PrintF(_S( 680, "구출 NPC가 강한 공격을 받았습니다." ));	
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );
						break;
					case HITTYPE_HARD:// 매우 강하게
						strSysMessage.PrintF(_S( 681, "구출 NPC가 매우 강한 공격을 받았습니다." ));	
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );
						break;
					case HITTYPE_CRITICAL:// 크리티컬
						strSysMessage.PrintF(_S( 682, "구출 NPC가 크리티컬 공격을 받았습니다." ));	
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );
						break;
					case HITTYPE_DEADLY:// 죽도록 강하게
						strSysMessage.PrintF(_S( 683, "구출 NPC가 데들리 공격을 받았습니다." ));	
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );
						break;
					}

					strSysMessage.PrintF( _S( 706, "구출 NPC의 현재 HP : %d" ), pPack->targetHp);	
					_pNetwork->ClientSystemMessage(strSysMessage);
					_pNetwork->UpdateRescueNPC(penTargetEntity, pPack->targetHp, pPack->targetMp);		
					((CUnit*)((CEntity*) penTargetEntity))->m_nPreHealth = pPack->targetHp;	
				}
				return;
			}

			// 플레이어를 공격함.		
			if(pPack->targetType == MSG_CHAR_PC)
			{
				if(_pNetwork->MyCharacterInfo.index == pPack->targetIndex)//나를 공격하는거라면,
				{
					// Date : 2005-11-16(오후 5:47:06), By Lee Ki-hwan
					pUIManager->AddDamageData( pPack->damage, pPack->flag, en_ulID, TRUE );

					ShowDamageResult( penEntity, pPack->damage, pPack->flag, pPack->skillIndex );

					// 생산중이라면 취소
					if(m_bProduction) 
					{	
						CancelProduct();
					}
				
					//0819 자신의 체력 갱신
					if ( !bHp_Percentage )
					{
						_pNetwork->MyCharacterInfo.hp = pPack->targetHp;
					}

					if(pPack->flag != HITTYPE_MISS)
					{
						NickNameEffect(this, penEntity, NICKNAME_DAMAGE_EFFECT);
					}

					if (pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->m_pEntity != NULL)
					{ // 나를 공격하는 녀석을 등록
						((CUnit*)pInfo->GetMyApetInfo()->m_pEntity)->SetEnemyEntity(penEntity, 0);
					}

					if(pPack->targetHp<=0 && !m_bDying)//0817 버그수정.자신의 캐릭터 죽기.
					{
						DeathYou();
					}

					if(!_pNetwork->m_bSingleMode)
					{
						((CUnit*)penEntity)->SetTargetEntity(this);

						if( (pPack->skillIndex == -1) && (pPack->damageType != MSG_DAMAGE_COMBO))
						{
							((CUnit*)penEntity)->AttackNow();
						}
						else //if (pPack->damageType == MSG_DAMAGE_COMBO)
						{ // 일단 Hit Effect없이 데미지만 표시하자!
							pUIManager->ShowDamage(en_ulID);
						}
					}
					return;
				}
			}
			
			// 내 플레이어 이외의 다른 것을 공격하는 경우.						
			CEntity *penTargetEntity = NULL;
			if( _pNetwork->SearchEntityByNetworkID( pPack->targetIndex, pPack->targetType, penTargetEntity ) )
			{
				((CUnit*)penTargetEntity)->m_enAttackerID = pPack->charIndex;

				// NOTE : 싱크가 안맞아도, 내 소환수가 공격할 경우 데미지가 표시가 안될때가 더 많기 때문에 어쩔수 없이 이렇게 처리함!
				// 데미지를 보여줘야 할 경우(내 소환수가 공격하거나, 공격받는 경우.)
				BOOL bShowDamage = FALSE;

				// 공격자가 내 소환수인 경우.
				if( penEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE) )
				{
					// Date : 2005-11-16(오후 5:47:06), By Lee Ki-hwan
					pUIManager->AddDamageData( pPack->damage, pPack->flag, penTargetEntity->en_ulID, FALSE );
					bShowDamage = TRUE;

					if( pPack->skillIndex != -1 )
					{
						CPrintF("소환수의 스킬 공격 [%d]타겟에게 : %ld의 데미지를 줌\n", pPack->targetIndex, pPack->damage);
					}
					else
					{
						CPrintF("소환수가 [%d]타겟에게 : %ld의 데미지를 줌\n", pPack->targetIndex, pPack->damage);
					}
				}

				// 타겟이 내 소환수인 경우.
				if( penTargetEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE) )
				{
					bShowDamage = TRUE;
					pUIManager->AddDamageData( pPack->damage, pPack->flag, penTargetEntity->en_ulID, TRUE );
				}

				if(penEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_WILDPET))
				{
					// 타겟의 데미지 정보를 표시함.
					pUIManager->AddDamageData( pPack->damage, pPack->flag, penTargetEntity->en_ulID, TRUE );
				}

				if(penTargetEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_WILDPET))
				{
					if (pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->m_pEntity != NULL)
					{ // 내 와일드 펫을 공격하는 녀석을 등록
						((CUnit*)pInfo->GetMyApetInfo()->m_pEntity)->SetEnemyEntity(penEntity, 1);
					}

					pUIManager->AddDamageData( pPack->damage, pPack->flag, penTargetEntity->en_ulID, TRUE );
				}

				if(!((CUnit*)penEntity)->m_bSkilling && pPack->skillIndex == -1 && (pPack->damageType != MSG_DAMAGE_COMBO))
				{
					((CUnit*)penEntity)->SetTargetEntity(penTargetEntity);
					((CUnit*)penEntity)->SetAttackSpeed(pPack->attackSpeed);
					((CUnit*)penEntity)->AttackNow();
					if (penTargetEntity->IsCharacter() && pPack->flag != HITTYPE_MISS)
					{
						NickNameEffect(penEntity, penTargetEntity, NICKNAME_DAMAGE_EFFECT);
					}
				}
				// 스킬 공격시.
				else
				{
					// FIXME : PreHealth가 왜 필요한지 모르겠음!!!
					// NOTE : 소환수가 스킬로 공격했을때, 데미지 이펙트가 안터지는 경우가 있는데,
					// NOTE : 이때 타겟팅을 했을때 데미지 갱신이 안되서 이렇게 처리함.
					if( bHp_Percentage)
					{
						if( bMe == false ) // TO.DO HP
						{
							((CUnit*)penTargetEntity)->m_nCurrentHealth = pPack->targetHp;
							((CUnit*)penTargetEntity)->m_fMaxHealth = 1000.0f;
						}
					}
					else
					{
						((CUnit*)penTargetEntity)->m_nCurrentHealth = pPack->targetHp;
					}

					// NOTE : 싱크가 안맞아도, 내 소환수가 공격할 경우 데미지가 표시가 안될때가 더 많기 때문에 어쩔수 없이 이렇게 처리함!
					if( bShowDamage )
					{
						pUIManager->ShowDamage( penTargetEntity->en_ulID );
					}
					if (penTargetEntity->IsCharacter() && pPack->flag != HITTYPE_MISS)
					{
						NickNameEffect(penEntity, penTargetEntity, NICKNAME_DAMAGE_EFFECT);
					}

					pUIManager->GetSimplePlayerInfo()->UpdateHPMP();
				}

				// 내 펫이 공격받는 경우.
				if( penTargetEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_PET) )
				{
					//	김영환 변경
					if(_pNetwork->Get_MyChar_Attack(pPack->targetIndex,pPack->targetHp))
					{
						pUIManager->GetPetInfo()->GetPetDesc();
					}
					//CNetworkLibrary::sPetInfo	TempPet;
					//TempPet.lIndex				= targetID;
					//std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
					//	std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
					//if( iter != _pNetwork->m_vectorPetList.end() )
					//{
					//	(*iter).lHP		= targetHP;
					//	pUIManager->GetPetInfo()->GetPetDesc();
					//}
				}
				if( bHp_Percentage)
				{
					if( bMe == false ) // TO.DO HP
					{
						((CUnit*)penTargetEntity)->m_nPreHealth = pPack->targetHp;
						((CUnit*)penTargetEntity)->m_fMaxHealth = 1000.0f;
					}
				}
				else
				{
					((CUnit*)penTargetEntity)->m_nPreHealth = pPack->targetHp;
				}
							
				if(pPack->targetHp  <= 0)
				{
					((CUnit*)penEntity)->m_bKillEnemy = TRUE;
					((CUnit*)penEntity)->SetKillTargetEntity(penTargetEntity);
					((CUnit*)penTargetEntity)->DeathNow();
					if ( pInfo->GetTargetEntity(eTARGET) == penTargetEntity)
					{
						ClearTargetInfo(penTargetEntity);
					}
				}

				if( bMe == false)
				{
					UpdateUnitInfo( penTargetEntity, pPack->targetIndex, pPack->targetHp );
				}
				
				return;									
			}
			return; //피해자가 화면에 존재하지 않는다.				
		}

		// 공격자가 화면에 없어도 HP가 0인 캐릭터는 죽어야 되기 때문에...
		if(pPack->targetType == MSG_CHAR_PC)
		{
			//PVP일 경우
			if(pPack->targetIndex == _pNetwork->MyCharacterInfo.index)
			{
				return;
			}
		}

		//공격자가 보이지 않을경우 피해자에게 데미지 주기.
		CEntity *penTargetEntity = NULL;
		if( _pNetwork->SearchEntityByNetworkID( pPack->targetIndex, pPack->targetType, penTargetEntity ) )
		{
			// 타겟이 내 소환수인 경우.
			if( penTargetEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE) )
			{
				// Date : 2005-11-16(오후 5:47:06), By Lee Ki-hwan
				pUIManager->AddDamageData( pPack->damage, pPack->flag, penTargetEntity->en_ulID, TRUE );					
			}
			else if( penTargetEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_WILDPET) )
			{
				// Date : 2005-11-16(오후 5:47:06), By Lee Ki-hwan
				pUIManager->AddDamageData( pPack->damage, pPack->flag, penTargetEntity->en_ulID, TRUE );					
			}

			if( bHp_Percentage)
			{
				if( bMe == false ) // TO.DO HP
				{
					((CUnit*)penTargetEntity)->m_nCurrentHealth = pPack->targetHp;
					((CUnit*)penTargetEntity)->m_fMaxHealth = 1000.0f;
				}
			}
			else
			{
				((CUnit*)penTargetEntity)->m_nCurrentHealth = pPack->targetHp;
			}
								
			if(pPack->targetHp  <= 0)
			{
				((CUnit*)penTargetEntity)->DeathNow();
			}

			if( bMe == false)
			{
				UpdateUnitInfo( penTargetEntity, pPack->targetIndex, pPack->targetHp );					
			}
			return;
		}		
	};

	virtual void Read_net_Summon(int index, CNetworkMessage* istr)
	{
		FLOAT3D vDesiredPosition;		
		CEntity* penEntity;		
		CPlacement3D plPlacement;
				
		switch(index)
		{
			// FIXME : 타겟만 정렬될 수 있다면.
			// FIXME : 하나로 묶일 수가 있는 부분임.
			case MSG_MOVE:
			{
				ResponseClient::moveMsg* pPack = reinterpret_cast<ResponseClient::moveMsg*>(istr->GetBuffer());
				
				vDesiredPosition(1) = pPack->x;
				vDesiredPosition(2) = pPack->h+1;
				vDesiredPosition(3) = pPack->z;

				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_SLAVE, pPack->charIndex);
				ObjInfo* pInfo = ObjInfo::getSingleton();

				if (pObject != NULL)
				{
					CSlaveTarget* pTarget = static_cast< CSlaveTarget* >(pObject);					

					//if (_pNetwork->ga_World.EntityExists(st.m_nIdxClient, penEntity)) 
					{
						penEntity = pTarget->GetEntity();

						pTarget->SetyLayer( pPack->cYlayer );

						if (pPack->moveType == MSG_MOVE_PLACE)
						{
							((CCharacterBase*)pTarget->GetEntity())->StopNow();

							CPlacement3D place(vDesiredPosition, ANGLE3D(0, 0, 0));								
							((CUnit*)penEntity)->SetPlacement(place);								
							return;
						}

						if ( !(pPack->mapAttr & MATT_UNWALKABLE) )
						{
							pTarget->slave_sbAttributePos = pPack->mapAttr;
						}
						
						if (g_iCountry == GERMANY || g_iCountry == SPAIN || g_iCountry == FRANCE || g_iCountry == POLAND
							|| g_iCountry == ITALY || g_iCountry == TURKEY || g_iCountry == NETHERLANDS)
						{
							//if ((g_iCountry == GERMANY) && (sbAttributePos == ATTC_PEACE))
							if (pPack->mapAttr & MATT_PEACE)
							{
								if (pInfo->GetTargetEntity(eTARGET) == penEntity)
								{
									pInfo->TargetClear(eTARGET);
								}
								if (pInfo->GetTargetEntity(eTARGET_REAL) == penEntity)
								{
									pInfo->TargetClear(eTARGET_REAL);
								}
								penEntity->SetFlagOn(ENF_HIDDEN);
							}
							else
							{
								penEntity->SetFlagOff(ENF_HIDDEN);
							}
						}

						((CUnit*)penEntity)->m_vDesiredPosition = vDesiredPosition;
						if(pPack->moveType==MSG_MOVE_RUN || pPack->moveType==MSG_MOVE_WALK) //캐릭터 이동.
						{
							//ttos_080422 : 이속 감소시 스피트값이 음수 값이 올 수 있으므로 스피드 제한 걸어줌 
							// 이렇게 안하면 MovableEntity.es에서 GetRelativeHeading()함수 내에서 계산시 flow 발생함
							if (pPack->speed < 0 )
							{
								pPack->speed = 0.1f;
							}

							((CUnit*)penEntity)->m_fWalkSpeed		= pPack->speed;
							((CUnit*)penEntity)->m_fMoveSpeed		= pPack->speed;
							((CUnit*)penEntity)->m_fCloseRunSpeed	= pPack->speed;
							((CUnit*)penEntity)->m_fAttackRunSpeed	= pPack->speed;
							if( !penEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE) )
							{
								((CUnit*)penEntity)->MoveNow();
							}
						}
						else if(pPack->moveType == MSG_MOVE_STOP) //캐릭터 정지
						{								
							((CUnit*)penEntity)->m_fMoveSpeed = 5.0f;

							if( !penEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE) )
							{
								((CUnit*)penEntity)->StopandTeleport();
							}
						}							
					}
				}
				break;
			}
		}
	}

	virtual void OpenGate( int iGateNumber, BOOL bOpenGate )
	{	
		// 메라크가 아니라면...
		if( g_slZone == 7 || g_slZone == 4) // WSS_DRATAN_SIEGEWARFARE 070723 드라탄 추가
		{
			CEntity *pDoor = NULL;			
			// DoorController엔티티에 열려있는지 아닌지 정보를 저장해야 할듯...
			// 그 정보에 따라서 문을 열지 닫을지를 판단함...
			// WSS_DRATAN_SIEGEWARFARE 2007/10/16
			if( g_slZone == 7 )
			{			
				switch (iGateNumber)
				{
					case 0:
						pDoor = _pNetwork->ga_World.EntityFromID(19182);
						break;
					case 1:
						pDoor = _pNetwork->ga_World.EntityFromID(19183);
						break;
					case 2:
						pDoor = _pNetwork->ga_World.EntityFromID(19184);
						break;
					default: 
					{ return; }
				}
			}
			else if( g_slZone == 4 )
			{
				// WSS_DRATAN_SIEGEWARFARE 070723 ----------------------->>
				switch (iGateNumber)
				{
					case 3:	// 외성 오른쪽
						//pDoor = _pNetwork->ga_World.EntityFromID(14230);
						pDoor = _pNetwork->ga_World.EntityFromID(15510);
						break;
					case 4: // 외성 왼쪽
						//pDoor = _pNetwork->ga_World.EntityFromID(14228);
						pDoor = _pNetwork->ga_World.EntityFromID(15753);
						break;
					case 5: // 외성 가운데
						//pDoor = _pNetwork->ga_World.EntityFromID(14229);	//
						pDoor = _pNetwork->ga_World.EntityFromID(15752);
						break;
					case 6: // 진입 왼쪽
						//pDoor = _pNetwork->ga_World.EntityFromID(14229);	//
						pDoor = _pNetwork->ga_World.EntityFromID(15517);
						break;
					case 7: // 진입 오른쪽
						//pDoor = _pNetwork->ga_World.EntityFromID(14228);
						pDoor = _pNetwork->ga_World.EntityFromID(15516);
						break;
					default: 
						{ return; }
				}
			}
				// ------------------------------------------------------<<
	

			if(pDoor)
			{
				if( iGateNumber < 3 ) // 0,1,2 메라크 세개의 문
				{			
	
					((CDoorController*)pDoor)->m_bOpened = bOpenGate;
					SendToTarget(pDoor, EET_TRIGGER, this);							
				}
				// WSS_DRATAN_SIEGEWARFARE 070723 ----------------------->>
				else if( iGateNumber < 8 ) // 3,4,5 드라탄 세개의 인비저블박스(문대신)
				{
					INDEX tIdx;
					switch(iGateNumber)
					{
					case 3:
						tIdx = 11003; break;
					case 4:
						tIdx = 15554; break;
					case 5:
						tIdx = 15565; break;
					case 6:
						tIdx = 15497; break;
					case 7:
						tIdx = 15496; break;
					}
					
					if(bOpenGate)
					{						
						((CWorldBase*)pDoor)->SetCollisionFlags(ECF_MODEL_NO_COLLISION);
						// 이펙트를 수동으로 지워준다.						
						if(pDoor=_pNetwork->ga_World.EntityFromID(tIdx))
						{									
							((CModelHolder3*)pDoor)->SendEvent(EStop());
						};
					}
					else 
					{
						// WSS_DRATAN_SEIGEWARFARE 2007/10/1
						if (_pNetwork->MyCharacterInfo.sbJoinFlagDratan != WCJF_OWNER &&
							_pNetwork->MyCharacterInfo.sbJoinFlagDratan != WCJF_DEFENSE_GUILD &&
							_pNetwork->MyCharacterInfo.sbJoinFlagDratan != WCJF_DEFENSE_CHAR)
						{
							((CWorldBase*)pDoor)->SetCollisionFlags(ECF_BRUSH);
						}
						else
						{
							((CWorldBase*)pDoor)->SetCollisionFlags(ECF_MODEL_NO_COLLISION);
						}

						// 이펙트를 수동으로 시작한다.						
						if(pDoor=_pNetwork->ga_World.EntityFromID(tIdx))
						{								
							((CModelHolder3*)pDoor)->SendEvent(EStart());
						};
					}

				}
				// ------------------------------------------------------<<
			}			
		}
	}

	virtual void StandDown( BOOL bStand )
	{
		if(m_nActionSit == 0)//서있는 상태.
		{
			m_nActionSit = 1;//앉는 모션 시작.
			if( _pNetwork->MyCharacterInfo.job == NIGHTSHADOW )
			{
				DeleteWearingWeapon(FALSE, FALSE);
			}
		}
		else if(m_nActionSit == 2)//앉아있는 상태.
		{
			m_nActionSit = 3;//서는 모션 시작.
		}	
		else
		{
			return;
		}	

		m_bPlayAction		= TRUE;
		m_nPlayActionNum	= ACTION_NUM_SELL;
		_bPersonalShop		= TRUE;
	}

	virtual void SetChaData(int index, int type, ULONG hp, ULONG maxHp, SBYTE hairStyle, SBYTE faceStyle, CTString& strName, UWORD state, SBYTE pkTitle, SLONG pkstate) 
	{
		CEntity* penEntity = NULL;
		if(_pNetwork->ga_World.EntityExists(index, penEntity))
		{
			ASSERT(penEntity != NULL && "Invalid Entity Pointer");		

			((CCharacter*)penEntity)->m_ChaList = type;
			penEntity->SetSkaModel(CJobInfo::getSingleton()->GetFileName(type));
			
			CModelInstance* pMI = penEntity->GetModelInstance();
			if(pMI)
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetCharacterAppearance(pMI, type, hairStyle, faceStyle);
			}			
			
			((CCharacter*)penEntity)->SetAnimation( type );
			
			((CCharacterBase*)penEntity)->m_strName		= strName;			
			((CUnit*)penEntity)->m_nMaxiHealth		= maxHp;
			((CUnit*)penEntity)->m_nCurrentHealth	= hp;
			((CCharacter*)penEntity)->m_nHairStyle		= hairStyle;
			((CCharacter*)penEntity)->m_nFaceStyle		= faceStyle;
			
			((CCharacter*)penEntity)->m_nPkTitle = pkTitle;
			((CCharacter*)penEntity)->m_nPkState = pkstate;
			((CCharacter*)penEntity)->m_ChaState = state;

			if (state & PLAYER_STATE_FLYING)
			{
				((CCharacter*)penEntity)->m_bDisappearWeapon = TRUE;
			}
						
			if(state & PLAYER_STATE_SITDOWN)//1111
			{
				((CCharacter*)penEntity)->m_nSitDown		= 2;//2가 앉기.
				((CUnit*)penEntity)->m_nPlayActionNum	= 3;
			}

			if((state & PLAYER_STATE_PKMODE) && (state & PLAYER_STATE_PKMODEDELAY))
			{
				((CCharacter*)penEntity)->m_nPkMode = 2;
			}
			else if(state & PLAYER_STATE_PKMODE)
			{
				((CCharacter*)penEntity)->m_nPkMode = 1;
			}
			else
			{
				((CCharacter*)penEntity)->m_nPkMode = 0;
			}

			if(hp <= 0)//1122 죽어있는 상태에서 appear했다..
			{
				//((CCharacterBase*)penEntity)->m_bDeath = TRUE;
				((CUnit*)penEntity)->DeathNow();
				((CCharacter*)penEntity)->AnimForDeath();
			}
			else
			{
				((CCharacterBase*)penEntity)->m_bDeath = FALSE;
			}

//안태훈 수정 시작	//(Open beta)(2004-12-14)
			if(penEntity->GetModelInstance())
			{
				CSkaTag tag;
				tag.SetName("__ROOT");
				tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
				GetModelInstance()->m_tmSkaTagManager.Register(&tag);
				tag.SetName("__TOP");
				tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
				FLOATaabbox3D aabb;
				GetModelInstance()->GetAllFramesBBox(aabb);
				tag.SetOffsetPos(0, aabb.Size()(2) * GetModelInstance()->mi_vStretch(2), 0);
				GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			}
//안태훈 수정 끝	//(Open beta)(2004-12-14)
		}
	}

	virtual void SetCharacterState(CEntity* cha_Entity, INDEX ch_state)
	{
		if (cha_Entity != NULL)
		{
			((CCharacter*)cha_Entity)->m_ChaState = ch_state;
		}
	}

	virtual void SetShopData( INDEX index, SBYTE sbShopType )
	{
		CEntity* penEntity = NULL;
		if(_pNetwork->ga_World.EntityExists(index, penEntity))
		{
			ASSERT(penEntity != NULL && "Invalid Entity Pointer");		
			if(sbShopType != PST_NOSHOP)
			{
				((CCharacter*)penEntity)->m_nSitDown = 2;//2가 앉기.
				((CUnit*)penEntity)->m_nPlayActionNum = ACTION_NUM_SELL;
			}
			else
			{
				((CCharacter*)penEntity)->m_nSitDown = 0;//서기.
				((CUnit*)penEntity)->m_nPlayActionNum = ACTION_NUM_SELL;		
			}			
			((CUnit*)penEntity)->ActionNow();
		}
	}

	// [100107: selo] 트레이너를 피하기 위한 중요 데이터 처리
	virtual void SetImportantValues( FLOAT fWalkspeed, FLOAT fRunspeed, FLOAT fAttackrange, SBYTE sbAttackspeed, SBYTE sbMagicspeed ) 
	{
		m_fWalkspeed = fWalkspeed;
		m_fRunspeed = fRunspeed;
		m_fAttackrange = fAttackrange;
		m_sbAttackspeed = sbAttackspeed;
		m_sbMagicspeed = sbMagicspeed;
	}
	virtual FLOAT GetWalkspeed() 
	{
		return m_fWalkspeed; 
	}
	virtual FLOAT GetRunspeed() 
	{
		return m_fRunspeed; 
	}
	virtual FLOAT GetAttackrange() 
	{
		return m_fAttackrange; 
	}
	virtual SBYTE GetAttackspeed() 
	{ 
		return m_sbAttackspeed; 
	}	
	virtual SBYTE GetMagicspeed() 
	{ 
		return m_sbMagicspeed;
	}

	virtual void SetSkillCancel( BOOL bCanSkillCancel )
	{
		m_bCanSkillCancel = bCanSkillCancel;
	}


	// 일반 공격을 검사합니다.
	virtual BOOL CheckNormalAttack( CEntity *pEntity, FLOAT fDist )
	{
		// 일반 공격의 경우.
		if( pEntity->IsCharacter() )
		{
			return CheckAttackCharacter( pEntity, fDist );
		}
		else if( pEntity->IsEnemy() )
		{
			return CheckAttackEnemy( pEntity, fDist );
		}
		else if( pEntity->IsPet() )
		{
			return CheckAttackPet( pEntity, fDist );
		}
		else if( pEntity->IsSlave() )
		{				
			return CheckAttackSlave( pEntity, fDist );
		}
		else if( pEntity->IsWildPet() )
		{
			return CheckAttackWildPet(pEntity, fDist);
		}
		return FALSE;
	};
	
	// FIXME : 코드 정리 필요함.
	virtual BOOL CheckAttackTarget( INDEX iSkillIndex, CEntity *pEntity, FLOAT fDist )
	{
		if( !pEntity )
		{
			return FALSE;
		}

		// 공격할수 없는 경우!!!
		if(SE_Get_UIManagerPtr()->IsCSFlagOn(CSF_CANNOT_ATTACK_MASK))
		{
			return FALSE;
		}

		// 생산품의 경우...
		if( pEntity->GetFirstExFlags() & ( ENF_EX1_PEACEFUL | ENF_EX1_PRODUCTION | ENF_EX1_NPC | ENF_EX1_CURRENT_PET | ENF_EX1_CURRENT_WILDPET | ENF_EX1_TOTEM ) )
		{
			return FALSE;
		}
		
		// 일반 공격은 죽은 넘한테는 사용할 수 없음.
		if( iSkillIndex == -1 && !(pEntity->GetFlags() & ENF_ALIVE ) )
		{
			return FALSE;
		}

		// 일반 공격의 경우 애완동물을 타고서는 할 수 없음.
		if( iSkillIndex == -1 && _pNetwork->MyCharacterInfo.bPetRide )
		{
			return FALSE;
		}

		// 일반 공격의 경우 와일드 펫을 타고서는 할 수 없음. [12/8/2010 rumist]
		if( iSkillIndex == -1 && _pNetwork->MyCharacterInfo.bWildPetRide )
		{
			return FALSE;
		}

		if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
		{ // 비행 모드에서는 공격할 수 없다.
			return FALSE;
		}
		
		
		if( iSkillIndex == -1 )
		{
			// 일반 공격 검사.
			return CheckNormalAttack( pEntity, fDist );
		}
		else
		{
			// 스킬 공격 검사.
			return _pNetwork->CheckSkillAttack( iSkillIndex, pEntity );
		}		
		return FALSE;
	}
	// AI 펫관련 타겟에 대해 행동이 가능한지 
	virtual BOOL CheckEntityOfTarget(CEntity *pEntity, CEntity *pTarget)
	{
		if (pEntity == NULL)
		{
			return FALSE;
		}

		return ((CUnit*)pEntity)->CheckTarget(pTarget);
	}
	
	BOOL CheckDratanWarInside(CCharacterTarget* pTarget)
	{
		if (pTarget == NULL)
		{
			return FALSE;
		}

		//[ttos_2010_5_25]:드라탄 공성 지역 체크
		if (_pNetwork->MyCharacterInfo.zoneNo == 4 &&
			(_pNetwork->MyCharacterInfo.x >= 72 && _pNetwork->MyCharacterInfo.x <= 696 &&
			_pNetwork->MyCharacterInfo.z >= 2344 && _pNetwork->MyCharacterInfo.z <= 3016))
		{
			FLOAT3D			vObjectPos;
			vObjectPos = pTarget->GetEntity()->en_plPlacement.pl_PositionVector;
			if( vObjectPos(1) >= 72 && vObjectPos(1) <= 696 &&
				vObjectPos(3) >= 2344 && vObjectPos(3) <= 3016)
			{
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL MyCheckDratanWarInside()
	{
		//[ttos_2010_5_25]:드라탄 공성 지역 체크
		if (_pNetwork->MyCharacterInfo.zoneNo == 4 &&
			(_pNetwork->MyCharacterInfo.x >= 72 && _pNetwork->MyCharacterInfo.x <= 696 &&
			_pNetwork->MyCharacterInfo.z >= 2344 && _pNetwork->MyCharacterInfo.z <= 3016))
		{
			return TRUE;
		}

		return FALSE;
	}

	INDEX WarTypeAttackGroup(SBYTE sbType)
	{
		if (sbType == WCJF_ATTACK_GUILD || sbType == WCJF_ATTACK_CHAR)
		{
			return JOINWAR_ATTACK;
		}
		else if (sbType == WCJF_OWNER || sbType == WCJF_DEFENSE_GUILD || sbType == WCJF_DEFENSE_CHAR)
		{
			return JOINWAR_DEFENSE;
		}

		return JOINWAR_NONE;
	}

	// FIXME : 코드 정리 필요함.
	// 캐릭터를 공격할 수 있는가?
	BOOL CheckAttackCharacter( CEntity *penEnt, FLOAT fLength )
	{
		BOOL	bIsPartyMember = FALSE;
		bIsPartyMember = penEnt->IsSecondExtraFlagOn( ENF_EX2_MYPARTY );

		if (fLength > _pNetwork->MyCharacterInfo.attackrange)
		{
			return FALSE;
		}
		
		// 일단 파티 멤버는 공격할 수 없음.
		if( bIsPartyMember )
		{
			return FALSE;
		}
		
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, penEnt->GetNetworkID());

		if (pObject == NULL)
		{
			return FALSE;
		}

		CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

		
		CUIManager* pUIManager = SE_Get_UIManagerPtr();

		// [sora] RAID_SYSTEM
		// 원정대 멤버이면 공격 불가
		if(GAMEDATAMGR()->GetPartyInfo()->IsExpedetionMember(pTarget->GetCIndex()))
		{
			return FALSE;
		}
		
		const UWORD sbAttributePos = _pNetwork->MyCharacterInfo.sbAttributePos;
		const UWORD sbCharAttributePos = pTarget->cha_sbAttributePos;
		
		const SBYTE sbJoinFlagMerac = _pNetwork->MyCharacterInfo.sbJoinFlagMerac;
		const SBYTE sbCharacterJoinFlagMerac = pTarget->cha_sbJoinFlagMerac;
		
		// WSS_DRATAN_SEIGEWARFARE 2007/08/30 --------------------------------------------------------------->>
		const SBYTE sbJoinFlagDratan = _pNetwork->MyCharacterInfo.sbJoinFlagDratan;
		const SBYTE sbCharacterJoinFlagDratan = pTarget->cha_sbJoinFlagDratan;
		// --------------------------------------------------------------------------------------------------<<
		const LONG lGuildIndex = pTarget->cha_lGuildIndex;

		const BOOL bIsDartanWar = pUIManager->GetSiegeWarfareNew()->GetWarState();
		const BOOL bIsMeracWar = _pUISWDoc->IsWar();
		
		const BOOL bIsCharacterInsideDratan = CheckDratanWarInside(pTarget);
		const INDEX iSyndicateType	= pTarget->cha_iSyndicateType; // 결사대(RVR) 시스템.

		if (sbAttributePos & MATT_FREEPKZONE && sbCharAttributePos & MATT_FREEPKZONE)
		{
			return TRUE;
		}

		//피스존에선 공격은 늘 안된다.
		if (sbAttributePos & MATT_PEACE || sbCharAttributePos & MATT_PEACE)
		{
			return FALSE;
		}

		if (_pNetwork->IsRvrZone() == TRUE)	// RVR존에 있다면.
		{
			if (_pNetwork->MyCharacterInfo.iSyndicateType != iSyndicateType) // 같은 소속이 아니라면 PK
			{
				return TRUE;
			}
		}
				
		// 먼저 같은 길드를 확인한다. 같은 길드는 공격 못한다.
		if (_pNetwork->MyCharacterInfo.lGuildIndex != -1 
			&& lGuildIndex == _pNetwork->MyCharacterInfo.lGuildIndex
			&& _pNetwork->MyCharacterInfo.EntranceType != CURRENT_ENTER_PARTYCUBE) // 파티 큐브 입장 상태 일때 같은 길드라도 공격
		{ // 예외 상황이다.
			return FALSE;
		}
		
		//SIGEWAR_ATTACKRULE
		if (bSigewar_Attackrule)
		{
			// 내가 진행 중인 공성전에 참여한 상태인가?
			if (bIsDartanWar && sbJoinFlagDratan != WCJF_NONE)
			{
				if (sbCharacterJoinFlagDratan != WCJF_NONE)
				{
					if (WarTypeAttackGroup(sbJoinFlagDratan) != WarTypeAttackGroup(sbCharacterJoinFlagDratan))
					{
						return TRUE;
					}
				}
				else // 타겟 캐릭터가 공성에 참여 안한 친구일 경우, 공성지역에 있으면 무조건 공격 대상
				{
					if (bIsCharacterInsideDratan)
					{
						return TRUE;
					}
				}
			}
			else if (bIsMeracWar && sbJoinFlagMerac != WCJF_NONE)
			{
				if (sbCharacterJoinFlagMerac != WCJF_NONE)
				{
					if (WarTypeAttackGroup(sbJoinFlagMerac) != WarTypeAttackGroup(sbCharacterJoinFlagMerac))
					{
						return TRUE;
					}
					else if (!_pUISWDoc->IsSelectBattle()) // 내성 전투의 경우
					{
						if ((WarTypeAttackGroup(sbJoinFlagMerac) && IsPvp()) || IsLegitTarget(penEnt)) // 공성측 끼리 pvp모드를 키면 공격이 가능
						{
							return TRUE;
						}
					}
				}
				else // 타겟 캐릭터가 공성에 참여 안한 친구일 경우, 공성지역에 있으면 무조건 공격 대상
				{
					if (sbAttributePos & MATT_WAR && sbCharAttributePos & MATT_WAR) // 메라크는 메라크 공성지역을 판단할 조건이 현재 없음
					{
						return TRUE;
					}
				}
			}
			// 일반적인 PVP룰 PVP모드 ON 정당방위 성립 혹은 길드전쟁의 적
			else if( IsPvp() || IsLegitTarget(penEnt) || pUIManager->GetGuildBattle()->IsEnemy( penEnt->en_ulID ) )
			{
				return TRUE;
			}
		}
		//else SIGEWAR_ATTACKRULE
		else
		{
			
			////////////////////////////////////////////////////////////////
			// FIXME : 아래 코드 너무함...ㅠ.ㅠ 수정하자!!!
			////////////////////////////////////////////////////////////////
			// 내가 공성 지역에 있고, 타 캐릭터도 공성 지역내에 있다면...
			
			if ((sbAttributePos & MATT_WAR && sbCharAttributePos & MATT_WAR) || 
				 CheckDratanWarInside(pTarget))
			{
				////////////////////////////////////////////////////////////////
				// 공성전 중이라면...
				
				// WSS_DRATAN_SEIGEWARFARE 2007/08/30 --------------------------------------------<<
				// 드라탄
				if ( pUIManager->GetSiegeWarfareNew()->GetWarState() ) // WSS_DRATAN_SEIGEWARFARE 2007/08/30 드라탄 공성중...
				{
					// 공성에 참여하지 않음...
					if(sbJoinFlagDratan == WCJF_NONE )
					{	
						return TRUE;
					}
					// 내가 수성측일때.
					// WSS_DRATAN_SEIGEWARFARE 2007/10/1
					else if(sbJoinFlagDratan == WCJF_OWNER ||
						sbJoinFlagDratan == WCJF_DEFENSE_GUILD)				
					{
						// 수성 길드 공격 불가.
						if( sbCharacterJoinFlagDratan == WCJF_OWNER ||
							sbCharacterJoinFlagDratan == WCJF_DEFENSE_GUILD )
						{
							return FALSE;
						}
						else
						{						
							return TRUE;					
						}
					}				
					// 내가 공성 길드일때...
					else if( sbJoinFlagDratan == WCJF_ATTACK_GUILD )
					{
						// 타겟이 공성 길드일때...
						if( sbCharacterJoinFlagDratan == WCJF_ATTACK_GUILD )
						{
							return TRUE;
						}
						else
						{							
							return TRUE;					
						}
					}
					
				}
				// -------------------------------------------------------------------------------<<
				
				// 메라크
				else if( _pUISWDoc->IsWar() && !_pUISWDoc->IsSelectBattle() ) 				
				{
					// 공성에 참여하지 않음...
					if(sbJoinFlagMerac == WCJF_NONE )
					{					
						return TRUE;				
					}
					// 내가 수성측일때.
					else if( sbJoinFlagMerac == WCJF_DEFENSE_CHAR || sbJoinFlagMerac == WCJF_OWNER || sbJoinFlagMerac == WCJF_DEFENSE_GUILD )				
					{
						// 수성 길드나 수성 용병은 공격 불가.
						if( sbCharacterJoinFlagMerac == WCJF_OWNER || 
							sbCharacterJoinFlagMerac == WCJF_DEFENSE_CHAR ||
							sbCharacterJoinFlagMerac == WCJF_DEFENSE_GUILD )
						{
							return FALSE;
						}
						else
						{						
							return TRUE;					
						}
					}				
					// 내가 공성용병이나 공성 길드일때...
					else if( sbJoinFlagMerac == WCJF_ATTACK_GUILD || 
						sbJoinFlagMerac == WCJF_ATTACK_CHAR )
					{
						// 타겟이 공성용병이나 공성 길드일때...
						if( sbCharacterJoinFlagMerac == WCJF_ATTACK_GUILD || 
							sbCharacterJoinFlagMerac == WCJF_ATTACK_CHAR )											
						{
							if( IsPvp() || IsLegitTarget(penEnt))
							{
								return TRUE;
							}
							return FALSE;
						}
						else
						{							
							return TRUE;					
						}
					}
				}
				////////////////////////////////////////////////////////////////
				// 야전전투!!!
				else if(_pUISWDoc->IsSelectBattle() )
				{
					// 공성에 참여하지 않음...
					if(sbJoinFlagMerac == WCJF_NONE )
					{
						return TRUE;					
					}
					// 내가 수성용병일때...
					else if( sbJoinFlagMerac == WCJF_DEFENSE_CHAR || sbJoinFlagMerac == WCJF_OWNER || sbJoinFlagMerac == WCJF_DEFENSE_GUILD )
					{
						// 수성 길드나 수성 용병은 공격 불가.
						if( sbCharacterJoinFlagMerac == WCJF_OWNER || 
							sbCharacterJoinFlagMerac == WCJF_DEFENSE_CHAR ||
							sbCharacterJoinFlagMerac == WCJF_DEFENSE_GUILD )
						{
							return FALSE;
						}
						else
						{						
							return TRUE;					
						}
					}
					// 내가 수성길드일때...
					else if( sbJoinFlagMerac == WCJF_OWNER || 
						sbJoinFlagMerac == WCJF_DEFENSE_GUILD )											
					{
						// 수성 길드나 수성 용병은 공격 불가.
						if( sbCharacterJoinFlagMerac == WCJF_OWNER || 
							sbCharacterJoinFlagMerac == WCJF_DEFENSE_CHAR ||
							sbCharacterJoinFlagMerac == WCJF_DEFENSE_GUILD )
						{
							return FALSE;
						}										
						else
						{					
							return TRUE;					
						}
					}
					// 내가 공성용병이나 공성 길드일때...
					else if( sbJoinFlagMerac == WCJF_ATTACK_GUILD || 
						sbJoinFlagMerac == WCJF_ATTACK_CHAR )
					{					
						return TRUE;				
					}
				}
				else
				{
					// 같은 파티나 길드 멤버가 아니고...
					if( IsPvp() || IsLegitTarget(penEnt) || pUIManager->GetGuildBattle()->IsEnemy( penEnt->en_ulID ) )
					{
						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
			}
			else
			{
				
				// 같은 파티나 길드 멤버가 아니고...
				if( IsPvp() || IsLegitTarget(penEnt) || pUIManager->GetGuildBattle()->IsEnemy( penEnt->en_ulID ) )
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
		}	//End SIGEWAR_ATTACKRULE
		return FALSE;
	}

	// FIXME : 코드 정리 필요함.
	// 애완동물을 공격할 수 있는가?
	BOOL CheckAttackPet( CEntity *pEntity, FLOAT fLength )
	{
		if( pEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_PET) )
		{
			return FALSE;
		}

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_PET, pEntity->GetNetworkID());

		if (pObject != NULL)
		{
			CPetTarget* pTarget = static_cast< CPetTarget* >(pObject);
			
			ObjectBase* pCharObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pTarget->pet_OwnerIndex);

			if (pCharObject != NULL)
			{
				// 애완동물 주인을 공격할 수 있으면, 애완동물도 공격 가능함.
				return CheckAttackCharacter(pCharObject->GetEntity(), fLength);
			}

			return FALSE;
			
		}		
		return TRUE;
	}
	BOOL CheckAttackWildPet(CEntity *pEntity, FLOAT fLength)
	{
		if( pEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_WILDPET) )
		{
			return FALSE;
		}

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_WILDPET, pEntity->GetNetworkID());

		if (pObject != NULL)
		{
			CWildPetTarget* pTarget = static_cast< CWildPetTarget* >(pObject);

			ObjectBase* pCharObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pTarget->m_nOwnerIndex);

			if (pCharObject != NULL)
			{
				CCharacterTarget* pCharTarget = static_cast< CCharacterTarget* >(pCharObject);

				if (pCharTarget->cha_statusEffect.IsState(EST_ASSIST_INVISIBLE))
				{
					return TRUE;
				}

				// 애완동물 주인을 공격할 수 있으면, 애완동물도 공격 가능함.
				return CheckAttackCharacter(pCharTarget->GetEntity(), fLength);
			}

			return FALSE;
		}		
		return TRUE;

	}

	// FIXME : 코드 정리 필요함.
	// 소환수를 공격할 수 있는가?
	BOOL CheckAttackSlave( CEntity *pEntity, FLOAT fLength )
	{
		if( pEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE) ) { return FALSE; }

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_SLAVE, pEntity->GetNetworkID());

		if (pObject != NULL)
		{
			CSlaveTarget* pTarget = static_cast< CSlaveTarget* >(pObject);

			ObjectBase* pCharObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pTarget->slave_OwnerIndex);

			if (pCharObject != NULL)
			{
				return CheckAttackCharacter(pCharObject->GetEntity(), fLength);
			}

			return FALSE;
		}

		return TRUE;
	}

	// FIXME : 코드 정리 필요함.
	// 몹을 공격할 수 있는가?
	BOOL CheckAttackEnemy( CEntity *pEntity, float fLength )
	{
		const INDEX iMobIndex = ((CEnemy*)((CEntity*)pEntity))->m_nMobDBIndex;										
//		const SBYTE sbJoinFlagMerac = _pNetwork->MyCharacterInfo.sbJoinFlagMerac;
		SBYTE sbJoinFlagMerac = _pNetwork->MyCharacterInfo.sbJoinFlagMerac;
		SBYTE sbJoinFlag = sbJoinFlagMerac;

		CMobData* MD = CMobData::getData(iMobIndex);
		FLOAT fScaledSize = MD->GetScaledSize();
		INDEX iSyndicateType = MD->GetSyndicateType();

		// rvr존이고 내 결사대 타입과 몬스터 결사대 타입이 같다면 공격할수 없다.
		if (_pNetwork->IsRvrZone() && iSyndicateType == _pNetwork->MyCharacterInfo.iSyndicateType)
		{
			return FALSE;
		}

		// WSS_DRATAN_SIEGEWARFARE 070720 ----------------------------------------------------------------------->>
		// TODO :: 현재 임시로 코드 처리 이후 드라탄 공성으로 수정해야함...
		//	       드라탄이나 메라크중 하나 이므로 기존 코드 그대도 사용 ^^;;	
		const SBYTE sbJoinFlagDratan = _pNetwork->MyCharacterInfo.sbJoinFlagDratan;
		ASSERT(sbJoinFlagMerac==WCJF_NONE||sbJoinFlagDratan==WCJF_NONE);
		//sbJoinFlag = (sbJoinFlagDratan == WCJF_NONE) ? sbJoinFlagMerac:sbJoinFlagDratan;
		sbJoinFlag = (_pNetwork->MyCharacterInfo.zoneNo == 7) ? sbJoinFlagMerac:sbJoinFlagDratan; //ttos 071121 : 참여 여부가 아니라 존으로 체크
		// ------------------------------------------------------------------------------------------------------<<
		
		// WSS_DRATAN_SEIGEWARFARE 2007/07/30 
		// 드라탄 공성 크리스탈은 공격하지 않는다....
		if( iMobIndex == DRATAN_LORD_SYMBOL_INDEX) 
		{ 
			return FALSE;
		}
		else if (fLength > (_pNetwork->MyCharacterInfo.attackrange + fScaledSize)) // 공격 거리 확인
		{
			return FALSE;
		}
		else if( iMobIndex == LORD_SYMBOL_INDEX )  // 성주의 권좌의 경우 길드장만 공격 가능.
		{
			
			if (_pNetwork->MyCharacterInfo.lGuildPosition < GUILD_MEMBER_MEMBER && //길드장,부장만
			sbJoinFlag == WCJF_ATTACK_GUILD && fLength <= 3.0f)
			{
				return TRUE;
			}

			return FALSE;
		}
		else
		{
			// 죽은 유닛을 왜 처리해야 하는가? (일단 살려두자)
			if(((CUnit*)pEntity)->m_nCurrentHealth <= 0)
			{
				CMobData* MD = CMobData::getData(iMobIndex);
				
				// 공성탑의 경우 죽었어도 공격할 수가 없음.
				if( MD->IsCastleTower() )
				{
					return FALSE;
				}
				else
				{
					if(MD->IsCastleGuard() || MD->IsLordSymbol())
					{
						// 수성 길드나 수성 용병은 공격 불가.
						if( sbJoinFlag == WCJF_OWNER || 
							sbJoinFlag == WCJF_DEFENSE_CHAR ||
							sbJoinFlag == WCJF_DEFENSE_GUILD ||
							sbJoinFlag == WCJF_NONE )
						{							
							return FALSE;
						}
						// WSS_DRATAN_SEIGEWARFARE 2007/07/31
						else if( sbJoinFlag == WCJF_ATTACK_GUILD &&  _pUISWDoc->IsWar() )
						{
							return TRUE;
						}						
					}
					else
					{
						return TRUE;
					}
				}
			}			
			else
			{
				// WSS_DRATAN_SEIGEWARFARE 2007/08/30
				CMobData* MD = CMobData::getData(iMobIndex);
				int tQtr = MD->IsMyQuarter();
				if( tQtr != QUARTER_NONE)
				{
					switch(tQtr)
					{
						case QUARTER_MINE:							
						case QUARTER_INSTALL:
							return FALSE;
							break;
						case QUARTER_ATTACK:
							return TRUE;
							break;
					}
				}
				else if(MD->IsCastleGuard() || MD->IsCastleTower() || MD->IsLordSymbol())
				{
					// 수성 길드나 수성 용병은 공격 불가.
					if( sbJoinFlag == WCJF_OWNER || 
						sbJoinFlag == WCJF_DEFENSE_CHAR ||
						sbJoinFlag == WCJF_DEFENSE_GUILD || 
						sbJoinFlag == WCJF_NONE )
//						!SE_Get_UIManagerPtr()->GetSiegeWarfareNew()->GetWarState() ) // WSS_DRATAN_SEIGEWARFARE 2007/09/12 드라탄 공성중이 아니면...
					{
						return FALSE;
					}
				}
			}
		}

		// [2010/10/20 : Sora] 몬스터 용병 카드
		INDEX ownerIndex = 0;

		ObjectBase* pObject = ACTORMGR()->GetObjectByCIndex(eOBJ_MOB, pEntity->en_ulID);

		if (pObject != NULL)
		{
			CMobTarget* pMT = static_cast< CMobTarget* >(pObject);
			ownerIndex = pMT->mob_iOwnerIndex;
		}
		
		if( ownerIndex > 0 )
		{
			if( _pNetwork->MyCharacterInfo.index == ownerIndex )
			{
				return FALSE;
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, ownerIndex);

				if (pObject != NULL)
				{
					// 주인을 공격할 수 있담 공격 가능
					// [2011/02/08 : Sora] 몬스터를 공격할 경우 공격 거리에 사이즈 에따라 값을 더해주므로
					// 일반 캐릭터 공격체크에 더해진 값을 보내면 맞지 않을 수 있다 
					return CheckAttackCharacter(pObject->GetEntity(), 0.f);
				}
			}
		}

		return TRUE;
	}

//강동민 수정 시작 싱글 던젼 작업	08.09
	//-----------------------------------------------------------------------------
	// Purpose: 
	// Input  : *pTarget - 
	// Output : 	virtual void
	//-----------------------------------------------------------------------------
	virtual void StartCamera(CEntity *pTarget, BOOL bRestart)
	{
		m_penCamera = pTarget;
		((CCamera*)pTarget)->m_penTarget = ((CCamera*)pTarget)->m_penTargetOld;
		SendToTarget(m_penCamera, EET_TRIGGER, this);
	}

	virtual void SetWideScreen(CEntity *pTarget, BOOL bWide, FLOAT fFov)
	{
		((CCamera*)pTarget)->m_bWideScreen = bWide;

		((CCamera*)pTarget)->m_fFOV = fFov;
		((CCamera*)pTarget)->m_fLastFOV = fFov;
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	// Input  : apr - 
	// Output : 	virtual void
	//-----------------------------------------------------------------------------
	virtual void GetProjection(CAnyProjection3D& apr)
	{
		apr = m_apr;
	}
//강동민 수정 끝 싱글 던젼 작업		08.09

	virtual void SetProduct(SLONG slChaIndex, SBYTE slProductType)
	{
		CEntity* penEntity;
		SLONG chaID = _pNetwork->SearchClientChaIndex(slChaIndex);
		if(chaID != -1)
		{
			if (_pNetwork->ga_World.EntityExists(chaID, penEntity)) 
			{	
				((CCharacter*)penEntity)->m_nPlayProduceNum = (SLONG)slProductType+1;
				((CCharacterBase*)penEntity)->ProduceNow();
			}
		}
	}

	// WSS_DRATAN_SEIGEWARFARE 2007/08/01
	virtual void SetConsensus(SLONG slChaIndex)
	{
		CEntity* penEntity;
		SLONG chaID = _pNetwork->SearchClientChaIndex(slChaIndex);
		if(chaID != -1)
		{
			if (_pNetwork->ga_World.EntityExists(chaID, penEntity)) 
			{	
				((CCharacterBase*)penEntity)->ConsensusNow();
				// WSS_DRATAN_SIEGEWARFARE 2007/10/12	
				// CModelInstance* pMI = penEntity->GetModelInstance();
				// WSS_DRATAN_SIEGEWARFARE 2007/10/14
				// StartEffectGroup("JEWEL",&pMI->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); 
				
			}
		}
	}

	// 소환수의 커맨드를 변경합니다.
	virtual void SetSummonCommand( CEntity* pEntity, INDEX iCommand )
	{
		if( !pEntity )
		{
			return;
		}

		((CSlave*)pEntity)->SetCurrentCommand(iCommand);		
	}

	virtual void WarpTeleport( FLOAT fX, FLOAT fZ, FLOAT fH, FLOAT fR, SBYTE sbLayer )
	{
		INFO()->TargetClear(eTARGET_REAL);
		
		_pNetwork->MyCharacterInfo.x		= fX;
		_pNetwork->MyCharacterInfo.z		= fZ;
		_pNetwork->MyCharacterInfo.h		= fH;
		_pNetwork->MyCharacterInfo.r		= fR;
		_pNetwork->MyCharacterInfo.yLayer	= sbLayer;

//		bug fix - wooss 060530		
//		_pNetwork->DeleteAllMob();		
		
		m_fCameraAngle			= 0.0f;
		
		m_bPlayAction			= FALSE;
		m_nPlayActionNum		= -1;
		m_nActionSit			= 0;
		
		//m_bRunningSelfSkill		= FALSE;	
		m_bDying				= FALSE;
		m_bRcvAtMsg				= TRUE;														
		m_bProduction			= FALSE;
		m_nProductionNum		= -1;

		PlayerInit(false);

		// stop moving
		//ForceFullStop();

		//CPlacement3D plPos( FLOAT3D( fX, fH, fZ ), ANGLE3D(fR, 0, 0) );
		//m_vDesiredPosition		= FLOAT3D( fX, fH, fZ );		
		//m_vMyPositionTmp		= FLOAT3D( fX, fH, fZ );
		StopMove();
		//StopRotating();
		//StopTranslating();
		
		GoTo( _pNetwork->MyCharacterInfo.x, _pNetwork->MyCharacterInfo.h + 0.5f, _pNetwork->MyCharacterInfo.z,
			_pNetwork->MyCharacterInfo.r );
	}
	// HairMesh의 파일이름 가져오기
	CTFileName GetHairMeshName(int iJob, SBYTE sbHairType)
	{
		CTFileName ctName = CTFileName("");

		if ( sbHairType >= HAIR_GREEN_CAP )
		{
			ctName = _afnHelmetMeshName[iJob][1];
		}
		else if ( sbHairType >= HAIR_RED_CAP )
		{ // 빨간 산타 모자, 녹색 산타 모자( 산타모자는 Mesh가 같다 )
			ctName = _afnHelmetMeshName[iJob][0];
		}
		else
		{// 일반 헤어
			ctName = _afnHairMeshName[iJob][sbHairType];
		}

		return ctName;
	}
	// Texture의 파일이름 가져오기
	CTFileName GetHairTextureName(int iJob, SBYTE sbHairType, SBYTE sbSubType)
	{
		CTFileName ctName = CTFileName("");

		if ( sbHairType >= HAIR_GREEN_CAP && sbSubType == 0 )
		{// 녹색 산타 모자
			ctName = _afnHelmetTextureName[iJob][1];
		}
		else if ( sbHairType >= HAIR_RED_CAP && sbSubType == 0 )
		{// 빨간 산타 모자
			ctName = _afnHelmetTextureName[iJob][0];
		}
		else
		{
			int nType = sbHairType;

			if ( nType >= HAIR_GREEN_CAP ) { nType -= HAIR_GREEN_CAP; }
			else if ( nType >= HAIR_RED_CAP ) { nType -= HAIR_RED_CAP; }
			
			ctName = _afnHairTextureName[iJob][nType+sbSubType];
		}

		return ctName;
	}

	void SetCharacterHairChange(INDEX nSIdx, SBYTE sbType, BOOL IsMe)
	{
		int nJob;

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, nSIdx);

		if (pObject == NULL)
		{
			return;		
		}

		CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

		CModelInstance *pMI = ((CCharacter*)pTarget->GetEntity())->GetCurrentPlayerModelInstance();
	
		if ( IsMe )
		{ // 자기 캐릭터
			nJob = _pNetwork->MyCharacterInfo.job;
		}
		else
		{ // 타 캐릭터
			nJob = pTarget->m_nType;
		}

		ChangeHairMesh(pMI, nJob, sbType-1);

		if ( IsMe ) 
		{ 
			_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
			_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
		}
		else
		{ 
			pTarget->cha_itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
			pTarget->cha_statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
		}
	}

	void SetCharacterAppearance(CModelInstance *pMI, int iJob, SBYTE sbHairType, SBYTE sbFaceType)
	{
		ChangeFaceMesh(pMI, iJob, sbFaceType - 1);
		ChangeHairMesh(pMI, iJob, sbHairType - 1);		
	}

	void ChangeFaceMesh(CModelInstance *pMI, int iJob, SBYTE sbType)		// 머리 바꾸기
	{
		ASSERT(pMI != NULL && "Invalid ModelInstance Pointer");
		
		INDEX iOldMeshID		= pMI->mi_iFaceMesh;
		INDEX iNewMeshID		= ska_GetIDFromStringTable(_afnFaceMeshName[iJob][sbType]);

		// 변함이 없는 메쉬.
//		if(iOldMeshID == iNewMeshID)
// 		{
// 			return;
//		}

		// [2012/08/27 : Sora] EX로그 추가
		if( IsEXRogue(iJob)  )
		{
			MeshInstance* tmpMesh = pMI->FindMeshInstance(ska_GetIDFromStringTable((CTString)_afnFaceMeshName[ROGUE][0]));
			
			if( tmpMesh )
			{
				pMI->DeleteMesh(tmpMesh->mi_pMesh->GetName());
			}
		}

		//2013/01/08 jeil EX메이지 추가
		if( IsEXMage(iJob)	)
		{
			MeshInstance* tmpMesh = pMI->FindMeshInstance(ska_GetIDFromStringTable((CTString)_afnFaceMeshName[MAGE][0]));
			
			if( tmpMesh )
			{
				pMI->DeleteMesh(tmpMesh->mi_pMesh->GetName());
			}
		}

		if(iOldMeshID == -1)
		{
			// Default Mesh
			iOldMeshID = ska_GetIDFromStringTable((CTString)_afnFaceMeshName[iJob][0]);
		}
		
		MeshInstance* pMESH = pMI->FindMeshInstance(iOldMeshID);
		if(pMESH)
		{
			pMI->DeleteMesh(pMESH->mi_pMesh->GetName());
		}
		if( iNewMeshID != -1 )
		{
			pMI->AddMesh_t(_afnFaceMeshName[iJob][sbType]);
			MeshInstance* pMESHNew = pMI->FindMeshInstance(iNewMeshID);	
			pMI->mi_iFaceMesh		= iNewMeshID;

			//pMI->mi_iFaceTexture = ska_GetIDFromStringTable(_afnFaceTextureName[iJob][sbType]);
			pMI->AddTexture_t(_afnFaceTextureName[iJob][sbType], _afnFaceTextureName[iJob][sbType].FileName(), pMESHNew);

			if(_afnFaceTextureName[iJob][sbType + TOTAL_HEAD].Length() > 0)
			{
				// FIXME : NormalMap Texture를 추가하도록 할것.
				pMI->AddTexture_t(_afnFaceTextureName[iJob][sbType + TOTAL_HEAD], _afnFaceTextureName[iJob][sbType + TOTAL_HEAD].FileName(), pMESHNew);
			}
		}
	};

	virtual void ChangeHairMesh(CModelInstance *pMI, int iJob, SBYTE sbType)		// 헤어 스타일 바꾸기
	{
		ASSERT(pMI != NULL && "Invalid ModelInstance Pointer");
		
		CTFileName ctHairMeshName = GetHairMeshName(iJob, sbType); // MeshName
		CTFileName ctHairTexture0 = GetHairTextureName(iJob, sbType, 0); // TextureName 0 : Default
		CTFileName ctHairTexture1 = GetHairTextureName(iJob, sbType, TOTAL_HAIR); // TextureName 1 : Normal Map
		CTFileName ctHairTexture2 = GetHairTextureName(iJob, sbType, TOTAL_HAIR*2); // TextureName 2 : Tail

		INDEX iOldMeshID		= pMI->mi_iHairMesh;
		INDEX iNewMeshID		= ska_GetIDFromStringTable(ctHairMeshName); // ctHairMeshName <== _afnHairMeshName[iJob][sbType]

		// 변함이 없는 메쉬.
		if(iOldMeshID == iNewMeshID)
		{
			return;
		}

		// [2012/08/27 : Sora] EX로그 추가
		if( IsEXRogue(iJob) )
		{
			MeshInstance* tmpMesh = pMI->FindMeshInstance(ska_GetIDFromStringTable((CTString)_afnHairMeshName[ROGUE][0]));
			
			if( tmpMesh )
			{
				pMI->DeleteMesh(tmpMesh->mi_pMesh->GetName());
			}
		}
		//2013/01/08 jeil EX메이지 추가
		if( IsEXMage(iJob)	)
		{
			MeshInstance* tmpMesh = pMI->FindMeshInstance(ska_GetIDFromStringTable((CTString)_afnHairMeshName[MAGE][0]));
			
			if( tmpMesh )
			{
				pMI->DeleteMesh(tmpMesh->mi_pMesh->GetName());
			}
		}

		if(iOldMeshID == -1)
		{
			// Default Mesh
			iOldMeshID = ska_GetIDFromStringTable((CTString)_afnHairMeshName[iJob][0]);

		}
		
		MeshInstance* pMESH = pMI->FindMeshInstance(iOldMeshID);
		if(pMESH)
		{
			pMI->DeleteMesh(pMESH->mi_pMesh->GetName());
		}
		if(iNewMeshID != -1)
		{
			pMI->AddMesh_t(ctHairMeshName);	// ctHairMeshName <== _afnHairMeshName[iJob][sbType]		
			MeshInstance* pMESHNew = pMI->FindMeshInstance(iNewMeshID);	
			pMI->mi_iHairMesh		= iNewMeshID;

			//pMI->mi_iHairTexture = ska_GetIDFromStringTable(_afnHairTextureName[iJob][sbType]);
			//pMI->AddTexture_t(_afnHairTextureName[iJob][sbType], _afnHairTextureName[iJob][sbType].FileName(), pMESHNew);
			// ctHairTexture0 <== _afnHairTextureName[iJob][sbType]
			pMI->AddTexture_t(ctHairTexture0, ctHairTexture0.FileName(), pMESHNew);

			if(ctHairTexture1.Length() > 0) // ctHairTexture1 <== _afnHairTextureName[iJob][sbType + 3]
			{
				// FIXME : NormalMap Texture를 추가하도록 할것.
				pMI->AddTexture_t(ctHairTexture1, ctHairTexture1.FileName(), pMESHNew);
			}
			if(ctHairTexture2.Length() > 0) // ctHairTexture2 <== _afnHairTextureName[iJob][sbType + 6]
			{
				// FIXME : NormalMap Texture를 추가하도록 할것.
				pMI->AddTexture_t(ctHairTexture2, ctHairTexture2.FileName(), pMESHNew);
			}
		}
	};

//0605 kwon
	virtual	void SetChaWearing(CCharacterTarget* pTarget, CEntity* penEntity, CNetworkMessage *istr) //msg_appear일때 쓰임.
	{
		int i;
		SLONG	wear_index;
		SLONG	item_plus;
		INDEX	cos_index;
		

		UpdateClient::appearPc* pPack = reinterpret_cast<UpdateClient::appearPc*>(istr->GetBuffer());

		for(i = 0; i < WEAR_COUNT; ++i)//1021//1105
		{
			BOOL	bNotWear = FALSE;

			if (i >= WEARING_SHOW_END + 1)
			{
				wear_index = pPack->backwing.dbIndex;
				item_plus = pPack->backwing.plus;
			}
			else
			{
				wear_index = pPack->wearingShowList[i].dbIndex;
				item_plus = pPack->wearingShowList[i].plus;
			}			

			cos_index = ((CCharacter*)penEntity)->IsThereCostumeWearing(i);
			CModelInstance *pMI = ((CCharacter*)penEntity)->GetCurrentPlayerModelInstance();
			
			if ( cos_index >= 0 )
			{
				bNotWear = TRUE;

				if (_pNetwork->GetItemData(cos_index)->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
				{
					if ( !( i == WEAR_HELMET && (CTString)_pNetwork->GetItemData(cos_index)->GetItemSmcFileName() == MODEL_TREASURE ) )
					{
						((CCharacter*)penEntity)->DeleteDefaultArmor(i);
						_pGameState->DeleteDefaultArmor( pMI, i, ((CCharacter*)penEntity)->m_ChaList );
						_pGameState->WearingArmor( pMI, *_pNetwork->GetItemData(cos_index) );
					}
				}
				else if (wear_index > 0)
				{
					((CCharacter*)penEntity)->WearingArmor(wear_index, _pNetwork->GetItemData(wear_index)->GetWearingPosition());
				}
				else
				{
					bNotWear = FALSE;
				}

				if ( wear_index >= 0 )
				{
					((CCharacter*)penEntity)->SetWearing(_pNetwork->GetItemData(wear_index)->GetWearingPosition(), wear_index);
				}
			}
			else if( wear_index > 0 ) //먼가 착용하고 있다면,
			{
				((CCharacter*)penEntity)->WearingArmor(wear_index, _pNetwork->GetItemData(wear_index)->GetWearingPosition());
				bNotWear = TRUE;
			}

			pTarget->cha_itemPlus[i] = item_plus;
			if(bNotWear && pMI)
			{
				int itemIdx = (pTarget->cha_CosWear[i] < 0) ? wear_index:pTarget->cha_CosWear[i];
				int subType = -1;
				CItemData* pWearItem = _pNetwork->GetItemData(itemIdx);
				if (pWearItem != NULL)
				{
					subType = pWearItem->GetSubType();
				}
				pTarget->cha_itemEffect.Change( 
					pTarget->m_nType
					, pWearItem
					, _pNetwork->GetItemData(wear_index)->GetWearingPosition()
					, pTarget->cha_itemPlus[i]
					, &pMI->m_tmSkaTagManager
					, (pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption
					, subType
					);
				pTarget->cha_statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
			}
		}

		for (i = WEAR_ACCESSORY_1; i <= WEAR_ACCESSORY_MAX; i++)
		{
			if ( pPack->accessory[i] > -1 )
			{
				CModelInstance *pMI = ((CCharacter*)penEntity)->GetCurrentPlayerModelInstance();
				if(pMI)
				{
					pTarget->cha_itemEffect.Change( 
						pTarget->m_nType
						, _pNetwork->GetItemData(pPack->accessory[i])
						, i
						, 0
						, &pMI->m_tmSkaTagManager
						, (pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption
						, _pNetwork->GetItemData(pPack->accessory[i])->GetSubType()
						);
				}
				
				pTarget->cha_itemAccessory[i] = pPack->accessory[i];

				int nRelicEffectType = -1;

				switch(pPack->accessory[i])
				{
				case 10951:	{nRelicEffectType = 0;}	break; // 천사의 유물
				case 10952: {nRelicEffectType = 1;}	break; // 대천사의 유물
				case 10953: {nRelicEffectType = 2;}	break; // 세라핌의 유물
				}

				if(nRelicEffectType != -1)
				{
					pTarget->cha_itemEffect.AddRelicEffect(nRelicEffectType, &pMI->m_tmSkaTagManager);
				}			
			}
		}

		//(*istr) >> state;//0824 상태.
	}
	// 코스튬2 아이템 착용 정보 ( msg_appear일때 쓰임. )
	// msg_appear에서만 사용
	virtual	void SetCharCostumeWearing(CCharacterTarget* pTarget, CEntity* penEntity, CNetworkMessage *istr)
	{
		if (pTarget == NULL)
		{
			return;
		}

		UpdateClient::appearPc* pPack = reinterpret_cast<UpdateClient::appearPc*>(istr->GetBuffer());
		
		for(int i = 0; i < WEAR_COSTUME_TOTAL; ++i)
		{			
			((CCharacter*)penEntity)->SetCostumeWearing(i, pPack->costume2[i]);
			pTarget->cha_CosWear[i] = pPack->costume2[i];
		}
	}

	// 코스튬2 아이템 입기/벗기 (자신이 아닌 다른 캐릭터)
	virtual void WearingCostumeArmor( CNetworkMessage *istr )
	{
		SLONG cha_index, item_index;
		SBYTE wear_pos;

		(*istr) >> cha_index;
		(*istr) >> wear_pos;
		(*istr) >> item_index; // -1 벗는것, 0 < 이면 입거나, 갈아 입거나

		CEntity *penEntity = NULL;

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, cha_index);

		if (pObject != NULL)
		{
			penEntity = pObject->GetEntity();

			CModelInstance *pMI = ((CCharacter*)penEntity)->GetCurrentPlayerModelInstance();
			INDEX wear_index = ((CCharacter*)penEntity)->IsThereWearing(wear_pos);
			INDEX realWearIndex = -1;
			int effect_plus = -1;

			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			realWearIndex = ((CCharacter*)penEntity)->IsThereCostumeWearing(wear_pos);

			if (realWearIndex > 0) // 벗기 (코스튬)
			{
				((CCharacter*)penEntity)->SetCostumeWearing(wear_pos, -1);

				if (_pNetwork->GetItemData(realWearIndex)->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
				{
					if ( (CTString)_pNetwork->GetItemData( realWearIndex )->GetItemSmcFileName() != MODEL_TREASURE )
					{
						((CCharacter*)penEntity)->DeleteCurrentArmor(realWearIndex);
					}
					else if ( wear_pos == WEAR_HELMET && (CTString)_pNetwork->GetItemData( realWearIndex )->GetItemSmcFileName() == MODEL_TREASURE )
					{
						((CCharacter*)penEntity)->DeleteDefaultArmor(wear_pos);
					}
				}
				else
				{
					if (wear_index > 0) // 벗기 (일반 장비)
					{
						((CCharacter*)penEntity)->DeleteCurrentArmor(wear_index);
					}
					else // 벗기 (Default)
					{
						((CCharacter*)penEntity)->DeleteDefaultArmor(wear_pos);
					}
				}				
			}
			else
			{
				if (item_index > 0 && _pNetwork->GetItemData(item_index)->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
				{
					if (wear_index > 0) // 벗기 (일반 장비)
					{
						((CCharacter*)penEntity)->DeleteCurrentArmor(wear_index);
					}
					else // 벗기 (Default)
					{
						((CCharacter*)penEntity)->DeleteDefaultArmor(wear_pos);
					}
				}
			}

			if (item_index > 0) // 입기(코스튬)
			{
				if (_pNetwork->GetItemData(item_index)->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
				{
					((CCharacter*)penEntity)->WearingArmor(item_index, wear_pos);
					realWearIndex = item_index;
				}
				((CCharacter*)penEntity)->SetCostumeWearing(wear_pos, item_index);
			}
			else if (wear_index > 0) // 입기 (일반 장비)
			{
				((CCharacter*)penEntity)->WearingArmor(wear_index, wear_pos);
				realWearIndex = wear_index;
			}
			else // 입기( default )
			{
				if (wear_pos != WEAR_COSTUME_HELMET)
				{
					((CCharacter*)penEntity)->WearingDefaultArmor(wear_pos);
				}
				else
				{
					ChangeHairMesh(pMI,pTarget->m_nType, ((CCharacter*)penEntity)->m_nHairStyle-1);
				}
				pTarget->cha_itemEffect.DeleteEffect(wear_pos);
				pTarget->cha_itemEffect.Refresh(&pMI->m_tmSkaTagManager, (pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption);
				realWearIndex = -1;
			}

			if (realWearIndex >= 0)
			{
				pTarget->cha_itemEffect.Change(
					pTarget->m_nType
					, _pNetwork->GetItemData(realWearIndex)
					, wear_pos
					, pTarget->cha_itemPlus[wear_pos]
					, &pMI->m_tmSkaTagManager
					, (pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption
					, _pNetwork->GetItemData(realWearIndex)->GetSubType()
					);

				if (pTarget->cha_state & PLAYER_STATE_SUPPORTER)
				{
					CTString strEffectName = CTString("Item_support");
					pTarget->cha_itemEffect.AddAccessoryEffect(strEffectName, &pMI->m_tmSkaTagManager);
				}
				pTarget->cha_itemEffect.Refresh(&pMI->m_tmSkaTagManager, (pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption);
				pTarget->cha_statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
			}
		}
	}

// added by sam 11/02/07 [SAM] 코스튬2 한벌의상 입기/벗기 (자신이 아닌 다른 캐릭터)
	virtual void WearingCostumeArmorOneSuit( CNetworkMessage *istr )
	{
		INDEX cha_index, item_index, slCount;
		INDEX wear_pos;

		(*istr) >> cha_index;
		(*istr) >> slCount;		
		for ( INDEX i = 0; i < slCount; ++i )
		{
			(*istr) >> wear_pos;
			(*istr) >> item_index; // -1 벗는것, 0 < 이면 입거나, 갈아 입거나			
			WearingCostumeArmorProcess ( cha_index, wear_pos, item_index );
		}		
	}

	virtual	BOOL IsHelmet(INDEX iCharIndex)	
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, iCharIndex);

		if (pObject != NULL)
		{
			if (((CCharacter*)(pObject->GetEntity()))->IsThereWearing(WEAR_HELMET) >= 0 ||
				((CCharacter*)(pObject->GetEntity()))->IsThereCostumeWearing(WEAR_COSTUME_HELMET) >= 0)
			{
				return TRUE;
			}
		}
		return FALSE;	
	}

// added by sam 11/02/07 [SAM] 코스튬2 한벌의상 입기/벗기 (자신이 아닌 다른 캐릭터) 실제 처리 함수 
	void WearingCostumeArmorProcess ( INDEX slCharIndex,  INDEX sbWwearPos, INDEX slItemIndex) // item_index -1 벗는것, 0 < 이면 입거나, 갈아 입거나)
	{
		CEntity *penEntity = NULL;

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, slCharIndex);

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			penEntity = pTarget->GetEntity();

			CModelInstance *pMI = ((CCharacter*)penEntity)->GetCurrentPlayerModelInstance();
			INDEX wear_index = ((CCharacter*)penEntity)->IsThereWearing(sbWwearPos);
			INDEX realWearIndex = -1;
			int effect_plus = -1;			

			realWearIndex = ((CCharacter*)penEntity)->IsThereCostumeWearing(sbWwearPos);

			if (realWearIndex > 0) // 벗기 (코스튬)
			{
				((CCharacter*)penEntity)->SetCostumeWearing(sbWwearPos, -1);

				if (_pNetwork->GetItemData( realWearIndex )->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
				{
					if ( (CTString)_pNetwork->GetItemData( realWearIndex )->GetItemSmcFileName() != MODEL_TREASURE )
					{
						((CCharacter*)penEntity)->DeleteCurrentArmor(realWearIndex);
					}
					else if ( sbWwearPos == WEAR_HELMET && (CTString)_pNetwork->GetItemData( realWearIndex )->GetItemSmcFileName() == MODEL_TREASURE )
					{	
						((CCharacter*)penEntity)->DeleteDefaultArmor(sbWwearPos);
					}
				}
				else
				{
					if (wear_index > 0) // 벗기 (일반 장비)
					{
						((CCharacter*)penEntity)->DeleteCurrentArmor(wear_index);
					}
					else // 벗기 (Default)
					{
						((CCharacter*)penEntity)->DeleteDefaultArmor(sbWwearPos);
					}
				}
			}
			else if (slItemIndex > 0 && 
				_pNetwork->GetItemData( slItemIndex )->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
			{
				if (wear_index > 0) // 벗기 (일반 장비)
				{
					((CCharacter*)penEntity)->DeleteCurrentArmor(wear_index);
				}
				else // 벗기 (Default)
				{
					((CCharacter*)penEntity)->DeleteDefaultArmor(sbWwearPos);
				}
			}

			if (slItemIndex > 0) // 입기(코스튬)
			{
				if (_pNetwork->GetItemData(slItemIndex)->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
				{
					((CCharacter*)penEntity)->WearingArmor(slItemIndex, sbWwearPos);
					realWearIndex = slItemIndex;
				}

				((CCharacter*)penEntity)->SetCostumeWearing(sbWwearPos, slItemIndex);
			}
			else if (wear_index > 0) // 입기 (일반 장비)
			{
				((CCharacter*)penEntity)->WearingArmor(wear_index, sbWwearPos);
				realWearIndex = wear_index;
			}
			else // 입기( default )
			{
				if (sbWwearPos != WEAR_COSTUME_HELMET)
				{
					((CCharacter*)penEntity)->WearingDefaultArmor(sbWwearPos);
				}
				else
				{
					ChangeHairMesh(pMI, pTarget->m_nType, ((CCharacter*)penEntity)->m_nHairStyle-1);
				}
				pTarget->cha_itemEffect.DeleteEffect(sbWwearPos);
				pTarget->cha_itemEffect.Refresh(&pMI->m_tmSkaTagManager, (pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption);
				realWearIndex = -1;
			}

			if (realWearIndex >= 0)
			{
				pTarget->cha_itemEffect.Change(
					pTarget->m_nType
					, _pNetwork->GetItemData(realWearIndex)
					, sbWwearPos
					, pTarget->cha_itemPlus[sbWwearPos]
					, &pMI->m_tmSkaTagManager
					, (pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption
					, _pNetwork->GetItemData(realWearIndex)->GetSubType()
					);

				if (pTarget->cha_state & PLAYER_STATE_SUPPORTER)
				{
					CTString strEffectName = CTString("Item_support");
					pTarget->cha_itemEffect.AddAccessoryEffect(strEffectName, &pMI->m_tmSkaTagManager);
				}
				pTarget->cha_itemEffect.Refresh(&pMI->m_tmSkaTagManager, (pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption);
				pTarget->cha_statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
			}
		}		
	}

	// 호칭 이펙트
	void NickNameEffect(CEntity *pAttackEn, CEntity *pTargetEn, NickNameEffectType iType)
	{
		CTString strNickDamageFile;
		INDEX iNickIndex;

		// 플레이어(나)일 경우
		if(pAttackEn == this)
		{
			iNickIndex = _pNetwork->MyCharacterInfo.iNickType;

			// 플레이어가 공격할 경우
			if(iType == NICKNAME_ATTACK_EFFECT)
			{
				pTargetEn->SetNickNameDamageEffect(iNickIndex, iType);

				if(pTargetEn->IsCharacter())	// 맞는 대상이 캐릭터라면 (NPC는 호칭이 없으므로 패스)
				{
					iNickIndex = pTargetEn->en_pCharacterTarget->cha_NickType;

					pTargetEn->SetNickNameDamageEffect(iNickIndex, iType);
				}
			}
			// 플레이어가 공격을 받을 경우
			else if(iType == NICKNAME_DAMAGE_EFFECT)
			{
				SetNickNameDamageEffect(iNickIndex, iType);

				if(pTargetEn->IsCharacter())	// 공격자가 캐릭터라면
				{
					iNickIndex = pTargetEn->en_pCharacterTarget->cha_NickType;
					
					SetNickNameDamageEffect(iNickIndex, iType);
				}
			}
			// 플레이어 호칭 이펙트일 경우
			else if(iType == NICKNAME_ATTACH_EFFECT)
			{
				SetNickNameDamageEffect(iNickIndex, iType);
			}
		}
		// 플레이어(내가)가 아닌 경우 : NPC, Character간의 공격 및 데미지 호칭 이펙트
		else
		{
			// 캐릭터라면
			if(pAttackEn->IsCharacter() && iType == NICKNAME_ATTACK_EFFECT)
			{
				iNickIndex = pAttackEn->en_pCharacterTarget->cha_NickType;
				
				pTargetEn->SetNickNameDamageEffect(iNickIndex, iType);
			}

			if(pTargetEn->IsCharacter() && iType == NICKNAME_DAMAGE_EFFECT)
			{
				iNickIndex = pTargetEn->en_pCharacterTarget->cha_NickType;
				
				pTargetEn->SetNickNameDamageEffect(iNickIndex, iType);
			}
		}
	}

	void SetNickNameDamageEffect(INDEX iNickIndex, NickNameEffectType iType)
	{
		CTString  strNickDamageFile;
		
		if(iNickIndex > 0 && iType == NICKNAME_ATTACK_EFFECT)	// 호칭이 있으면
		{
			strNickDamageFile = CNickNameData::getData(iNickIndex)->GetAttackEffectFile();
		}
		else if(iNickIndex > 0 && iType == NICKNAME_DAMAGE_EFFECT)
		{
			strNickDamageFile = CNickNameData::getData(iNickIndex)->GetDamageEffectFile();
		}
		else if(iNickIndex > 0 && iType == NICKNAME_ATTACH_EFFECT)
		{
			strNickDamageFile = CNickNameData::getData(iNickIndex)->GetEffectFile();
		}

		if( strNickDamageFile != CTString("") && iType != NICKNAME_ATTACH_EFFECT)
		{
			StartEffectGroup(strNickDamageFile
						, &this->en_pmiModelInstance->m_tmSkaTagManager
						, _pTimer->GetLerpedCurrentTick());
		}
		else if(strNickDamageFile != CTString("") && iType == NICKNAME_ATTACH_EFFECT)
		{
			_pNetwork->MyCharacterInfo.itemEffect.AddNickEffect(strNickDamageFile, &CEntity::GetPlayerEntity(0)->en_pmiModelInstance->m_tmSkaTagManager);
		}
	}

	virtual void SetDecoModeCamera( FLOAT fDistance, FLOAT fHeight )
	{
		// Camera Angle : 캐릭터 정면으로
		en_plViewpoint.pl_OrientationAngle(1) = GetPlacement().pl_OrientationAngle(1)/2+90;
		en_plViewpoint.pl_OrientationAngle(2) = 0.0f;

		// Camera Height, Distance : 높이 보정값, 거리 설정
		((CPlayerView&)*m_pen3rdPersonView).m_fCameraHeight = fHeight;
		((CPlayerView&)*m_pen3rdPersonView).m_fFaceChangeDistance = fDistance;

		// Get Model Height
// 		CModelInstance* pMI = GetModelInstance();
// 		FLOATaabbox3D boxModel;
// 		pMI->GetAllFramesBBox( boxModel );
// 		boxModel.StretchByVector( pMI->mi_vStretch );
// 		FLOAT fModelHeight = boxModel.maxvect(2) - boxModel.minvect(2);
// 
// 		// Player Position
// 		FLOAT3D vPlayerPosition = GetLerpedPlacement().pl_PositionVector;
// 
// 		// Camera Height
// 		if ( fModelHeight > 2.0f )
// 		{	// 캐릭터의 높이가 2.0 이상이면 카메라 위치도 높인다.
// 			fHeight += 0.2f;
// 		}
	}

	virtual void SetAppearanceData( int index, SBYTE hairstyle, SBYTE facestyle )
	{
		CEntity* penEntity = NULL;
		if(_pNetwork->ga_World.EntityExists(index, penEntity))
		{
			((CCharacter*)penEntity)->m_nHairStyle = hairstyle;
			((CCharacter*)penEntity)->m_nFaceStyle = facestyle;
		}
	}

	//2012/11/09 jeil 데미지 받았을대 여기서 카운트 감소 처리 
	//스킬로 받은 대미지가 0인 경우 [공격자]의 스킬 공격을 막았습니다. 메시지 출력처리 
	void ShowDamageResult(CEntity* pEntity, ULONG targetDamage, SBYTE targetFlag, SLONG	skillIndex)
	{			
		CTString strSysMessage;
		CTString strHp;
		CTString strName;
		strName = pEntity->GetName();

		CTString strSkillMessage;
 
		if( ((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsSitting() && targetFlag != HITTYPE_MISS )
		{// 앉아 있을 경우 피공격시 일어남
			SE_Get_UIManagerPtr()->GetCharacterInfo()->UseAction( 3 );
		}

		switch(targetFlag)
		{
		case HITTYPE_MISS:
			strSysMessage.PrintF( _S( 413, "%s의 공격을 피했습니다." ), strName );
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );
			break;
		case HITTYPE_WEAK:
			if( skillIndex != -1 )
			{
				//2012/11/12 jeil 스킬공격을 받았을때 대미지가 0이라면 스킬공격을 막았다는 메시지 출력 추가 -> String 필요
				if(targetDamage != 0){
					strSysMessage.PrintF( _S( 1390, "스킬 공격! %s에게 %d의 데미지를 받았습니다." ), strName, targetDamage );	
				}else{
					strSkillMessage.PrintF( _S( 5814, "%s의 스킬 공격을 막았습니다." ),strName );	
					_pNetwork->ClientSystemMessage( strSkillMessage);
				}
			}
			else
			{
				strSysMessage.PrintF( _S( 685, "약한 공격! %s에게 %d의 데미지를 받았습니다." ), strName, targetDamage );		
			}
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );				
			break;
		case HITTYPE_NORMAL:
			if( skillIndex != -1 )
			{
				//2012/11/12 jeil 스킬공격을 받았을때 대미지가 0이라면 스킬공격을 막았다는 메시지 출력 추가 -> String 필요
				if(targetDamage != 0){
					strSysMessage.PrintF( _S( 1390, "스킬 공격! %s에게 %d의 데미지를 받았습니다." ), strName, targetDamage );	
				}else{
					strSkillMessage.PrintF( _S( 5814, "%s의 스킬 공격을 막았습니다." ),strName );	
					_pNetwork->ClientSystemMessage( strSkillMessage);
				}
			}
			else
			{
				strSysMessage.PrintF( _S( 414, "일반공격! %s에게 %d의 데미지를 받았습니다." ), strName, targetDamage );
			}
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );
			break;
		case HITTYPE_STRONG:// 강하게
			if( skillIndex != -1 )
			{
				//2012/11/12 jeil 스킬공격을 받았을때 대미지가 0이라면 스킬공격을 막았다는 메시지 출력 추가 -> String 필요
				if(targetDamage != 0){
					strSysMessage.PrintF( _S( 1390, "스킬 공격! %s에게 %d의 데미지를 받았습니다." ), strName, targetDamage );	
				}else{
					strSkillMessage.PrintF( _S( 5814, "%s의 스킬 공격을 막았습니다." ),strName );	
					_pNetwork->ClientSystemMessage( strSkillMessage);
				}
			}
			else
			{
				strSysMessage.PrintF( _S( 686, "강한 공격! %s에게 %d의 데미지를 받았습니다." ), strName, targetDamage  );		
			}
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );				
			break;
		case HITTYPE_HARD:// 매우 강하게
			if( skillIndex != -1 )
			{
				//2012/11/12 jeil 스킬공격을 받았을때 대미지가 0이라면 스킬공격을 막았다는 메시지 출력 추가 -> String 필요
				if(targetDamage != 0){
					strSysMessage.PrintF( _S( 1390, "스킬 공격! %s에게 %d의 데미지를 받았습니다." ), strName, targetDamage );		
				}else{
					strSkillMessage.PrintF( _S( 5814, "%s의 스킬 공격을 막았습니다." ),strName );	
					_pNetwork->ClientSystemMessage( strSkillMessage);
				}
			}
			else
			{
				strSysMessage.PrintF( _S( 687, "매우 강한 공격! %s에게 %d의 데미지를 받았습니다." ), strName, targetDamage  );		
			}
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );				
			break;
		case HITTYPE_CRITICAL:// 크리티컬
			if( skillIndex != -1 )
			{
				//2012/11/12 jeil 스킬공격을 받았을때 대미지가 0이라면 스킬공격을 막았다는 메시지 출력 추가 -> String 필요
				if(targetDamage != 0){
					strSysMessage.PrintF( _S( 1390, "스킬 공격! %s에게 %d의 데미지를 받았습니다." ), strName, targetDamage );	
				}else{
					strSkillMessage.PrintF( _S( 5814, "%s의 스킬 공격을 막았습니다." ),strName );	
					_pNetwork->ClientSystemMessage( strSkillMessage);
				}
			}
			else
			{
				strSysMessage.PrintF( _S( 411, "크리티컬 히트! %s에게 %d의 데미지를 받았습니다." ), strName, targetDamage );
			}
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );
			m_bCriticalDamageMe = TRUE;
			break;
		case HITTYPE_DEADLY:// 죽도록 강하게
			if( skillIndex != -1 )
			{
				//2012/11/12 jeil 스킬공격을 받았을때 대미지가 0이라면 스킬공격을 막았다는 메시지 출력 추가 -> String 필요
				if(targetDamage != 0){
					strSysMessage.PrintF( _S( 1390, "스킬 공격! %s에게 %d의 데미지를 받았습니다." ), strName, targetDamage );	
				}else{
					strSkillMessage.PrintF( _S( 5814, "%s의 스킬 공격을 막았습니다." ),strName );	
					_pNetwork->ClientSystemMessage( strSkillMessage);
				}
			}
			else
			{
				strSysMessage.PrintF( _S( 688, "데들리 공격! %s에게 %d의 데미지를 받았습니다." ), strName, targetDamage );
			}
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACKED );				
			m_bCriticalDamageMe = TRUE;
			break;
		}
	}

	void ShowAttackResult(CEntity* penTargetEntity, ULONG targetDamage, SBYTE targetFlag, SLONG	skillIndex)
	{
		CTString strSysMessage;
		CTString strHp;
		CTString strMobName;
		strMobName = penTargetEntity->GetName();

		SLONG slCurrentWeight	= _pNetwork->MyCharacterInfo.weight;
		SLONG slMaxWeight		= _pNetwork->MyCharacterInfo.maxweight;
		SE_Get_UIManagerPtr()->AddDamageData( targetDamage, targetFlag, penTargetEntity->en_ulID, FALSE );		

		// FIXME : targetFlag를 꼭 체크해야 하나???
		switch(targetFlag)
		{
		case HITTYPE_MISS:
			strSysMessage.PrintF( _S2( 409, strMobName, "%s<가> 당신의 공격을 피했습니다." ), strMobName );
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACK );
			break;
		case HITTYPE_WEAK:
			strSysMessage.PrintF( _S( 689, "약한 공격! %s에게 %d의 데미지를 주었습니다." ), strMobName, targetDamage);
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACK );				
			break;
		case HITTYPE_NORMAL:
			if( skillIndex == -1 )
			{
				strSysMessage.PrintF( _S( 410, "일반공격! %s에게 %d의 데미지를 주었습니다." ), strMobName, targetDamage );
			}
			else
			{				
				strSysMessage.PrintF( _S( 787, "스킬 공격! %s에게 %d의 데미지를 주었습니다." ), strMobName, targetDamage);			
			}
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACK );				
			break;
		case HITTYPE_STRONG:// 강하게			
			strSysMessage.PrintF(_S( 690, "강한 공격! %s에게 %d의 데미지를 주었습니다." ), strMobName, targetDamage);
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACK );				
			break;
		case HITTYPE_HARD:// 매우 강하게
			strSysMessage.PrintF( _S( 691, "매우 강한 공격! %s에게 %d의 데미지를 주었습니다." ), strMobName, targetDamage);	
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACK );				
			break;
		case HITTYPE_CRITICAL:// 크리티컬
			strSysMessage.PrintF( _S( 407, "크리티컬 히트! %s에게 %d의 데미지를 주었습니다." ), strMobName, targetDamage );
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACK );	
			m_bCriticalDamage = TRUE;			
			break;
		case HITTYPE_DEADLY:// 죽도록 강하게			
			strSysMessage.PrintF( _S( 692, "데들리 공격! %s에게 %d의 데미지를 주었습니다." ), strMobName, targetDamage);		
			_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ATTACK );	
			m_bCriticalDamage = TRUE;
			break;
		}
	}

//강동민 수정 시작	// 생산 취소
	virtual void CancelProduct()
	{
		m_bProduction		= FALSE;
		m_nProductionNum	= -1;
		// player.es이 IdleAnim()은 effect를 list에서 제거 하지 않는다.
		// 생산 애니메이션의 경우 effect가 애니메이션에 발라 있어, loop를 돌면서 랜더링 된다.
		// 애니메이션 관련 처리는 모두 CPlayerAnimator에서 하고 있었다.
		if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING) // 비행 모드에서는 무기를 보여주지 않는다.
		{
			return;
		}

		AppearWearingWeapon(FALSE);
	}
//강동민 수정 끝	// 생산 취소
#define IS_FLAG_ON(flagAll, flagVal) ( (flagAll) & (static_cast<SQUAD>((1L))<<(flagVal)) )
	//----------------------------------------------------------------
	//0315 kwon
	virtual void Read_net_Character(int type, int sub_type, CNetworkMessage *istr) 
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();

		FLOAT3D vDesiredPosition;
		ANGLE3D aDesiredAngle;
		CEntity* penEntity;		
		CPlacement3D plPlacement;

		SLONG cha_index;
		SBYTE wear_pos;
		SLONG item_index;

		if(istr == NULL)//시스템 메시지.
		{
			m_bWaitForSkillResponse = FALSE;
			m_nCurrentSkillNum = -1;
			m_nDesiredSkillNum = -1;
			return;
		}

		switch(type)
		{
		case MSG_WEARING:
			{	
				(*istr) >> cha_index;
				(*istr) >> wear_pos;
				(*istr) >> item_index;
				SLONG item_plus;
				(*istr) >> item_plus;

				INDEX realTakeOffIndex = -1;

				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, cha_index);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);
					penEntity = pTarget->GetEntity();

					if (penEntity != NULL)
					{
						SLONG tmpItem_plus = item_plus;
						bool bAccessory = false;
						int nAcceWearPos = 0;
						int nRelicEffectType = -1;

						if(wear_pos >= 7 && wear_pos < 10)
						{
							bAccessory = true;
							nAcceWearPos = wear_pos - 7;
						}
						
						if(	item_index != -1)//입기.
						{							
							((CCharacter*)penEntity)->WearingArmor(item_index, wear_pos);

							if (wear_pos < WEAR_COUNT)
							{
								pTarget->cha_itemPlus[wear_pos] = item_plus;
							}

							if (bAccessory == true)
							{
								pTarget->cha_itemAccessory[nAcceWearPos] = item_index;

								switch(item_index)
								{
								case 10951:	{nRelicEffectType = 0;}	break; // 천사의 유물
								case 10952: {nRelicEffectType = 1;}	break; // 대천사의 유물
								case 10953: {nRelicEffectType = 2;}	break; // 세라핌의 유물
								}
							}
						}			
						else //벗기.
						{
							// 헬멧은 헤어르 준비하고, 일단 헬멧을 지워야 된다..
							((CCharacter*)penEntity)->TakeOffWearing(wear_pos, pTarget);
							if (wear_pos < WEAR_COUNT)
							{
								pTarget->cha_itemPlus[wear_pos] = -1;
							}

							if ( ((CCharacter*)penEntity)->IsThereWearing(wear_pos) >= 0 )
							{
								tmpItem_plus = -1;
							}

							if (bAccessory == true)
							{
								bool DeleteRelicEffect = false;

								// 유물 아이템인지 확인
								switch(pTarget->cha_itemAccessory[nAcceWearPos])
								{
								case 10951:	
								case 10952: 
								case 10953: 
								{
									DeleteRelicEffect = true;
								}	
								break;
								}
								
								// 유물 아이템일 경우 이펙트 삭제
								pTarget->cha_itemAccessory[nAcceWearPos] = item_index;

								if (DeleteRelicEffect == true)
								{
									pTarget->cha_itemEffect.DeleteRelicEffect();
									nRelicEffectType = pTarget->GetShowAnotherRelicEffect();
								}
							}
						}

						realTakeOffIndex = ((CCharacter*)penEntity)->IsThereWearing(wear_pos);
						INDEX iCosWear = -1;
						if (wear_pos != WEAR_BACKWING)
						{
							iCosWear = ((CCharacter*)penEntity)->IsThereCostumeWearing(wear_pos);
						}
						else
						{
							iCosWear = ((CCharacter*)penEntity)->IsThereCostumeWearing(WEAR_COSTUME_BACKWING);
						}
						if ( iCosWear > 0)
						{
							realTakeOffIndex = iCosWear;
						}

						CModelInstance *pMI = ((CCharacter*)penEntity)->GetCurrentPlayerModelInstance();
												
						if(pMI)
						{
							pTarget->cha_itemEffect.Change(
								pTarget->m_nType
								, _pNetwork->GetItemData(realTakeOffIndex)
								, wear_pos
								, tmpItem_plus
								, &pMI->m_tmSkaTagManager
								, (pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption
								, _pNetwork->GetItemData(realTakeOffIndex)->GetSubType()
								);

							if (pTarget->cha_state & PLAYER_STATE_SUPPORTER)
							{
								CTString strEffectName = CTString("Item_support");
								pTarget->cha_itemEffect.AddAccessoryEffect(strEffectName, &pMI->m_tmSkaTagManager);
							}
							pTarget->cha_itemEffect.Refresh(&pMI->m_tmSkaTagManager, (pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption);
							pTarget->cha_statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
							
							if(nRelicEffectType != -1)
							{								
								CModelInstance *pCharMI = penEntity->GetModelInstance();
								if (pCharMI != NULL)
								{
									pTarget->cha_itemEffect.AddRelicEffect(nRelicEffectType, &pCharMI->m_tmSkaTagManager);
								}								
							}
						}
					}
				}
			}
			break;

		case MSG_CHANGE: //변신.
			{
				SBYTE			type;		// MSG_CHAR_PC or MSG_CHAR_NPC
				SLONG			cha_index;
				SLONG			mobnum;
				
				(*istr) >> type;

				if(type == MSG_CHANGE_START)
				{
					(*istr) >> cha_index;
					(*istr) >> mobnum;
					
					if(cha_index == _pNetwork->MyCharacterInfo.index)
					{					
						m_nChangeMonsterId = mobnum;
						m_bChanging = TRUE;	
						m_bMobChange = FALSE;//변신상태라도 다시 변신해야 한다.	

						const int iStopChangeItem = 521;
						
						// Notice 목록에 추가함.
						Notice* pNotice = GAMEDATAMGR()->GetNotice();
						if (pNotice != NULL)
						{
							pNotice->AddToNoticeList(iStopChangeItem, Notice::NOTICE_POLYMOPH);
						}

						UIMGR()->SetCSFlagOn(CSF_TELEPORT);
					}else
					{
						SLONG chaID = _pNetwork->SearchClientChaIndex(cha_index);
						if(chaID != -1)
						{
							if (_pNetwork->ga_World.EntityExists(chaID, penEntity)) 
							{	
								((CCharacter*)penEntity)->m_bMobChange = FALSE;
								((CCharacter*)penEntity)->SetPolymophMobIndex(mobnum);
								((CCharacter*)penEntity)->PolymophNow();							
							}
						}
					}
				}
				else if(type == MSG_CHANGE_STOP)
				{
					(*istr) >> cha_index;

					if(cha_index == _pNetwork->MyCharacterInfo.index)
					{	
						m_nChangeMonsterId = 0;
						m_bChanging = TRUE;		
						const int iStopChangeItem = 521;

						Notice* pNotice = GAMEDATAMGR()->GetNotice();

						if (pNotice != NULL)
						{
							pNotice->DelFromNoticeList(iStopChangeItem, Notice::NOTICE_POLYMOPH);
						}
						
						UIMGR()->SetCSFlagOn(CSF_TELEPORT);
					}
					else
					{
						SLONG chaID = _pNetwork->SearchClientChaIndex(cha_index);
						if(chaID != -1)
						{
							if (_pNetwork->ga_World.EntityExists(chaID, penEntity)) 
							{	
								((CCharacter*)penEntity)->SetPolymophMobIndex(-1);
								((CCharacter*)penEntity)->PolymophNow();
							}
						}
					}
				}
				else if(type == MSG_CHANGE_ERR)
				{
					UBYTE err;
					
					(*istr) >> err;
					DisplayChangeErr(err);

					// 김영환
					_pNetwork->Set_MyChar_MorphStatus_MORPH_END();
					//_pNetwork->MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_END;
				}											

			}
			break;

		case MSG_PC_REBIRTH:
			{
				SBYTE  type;
				SLONG  cha_index;
				SLONG  hp,maxHp,mp,maxMp;
				SLONG  CurrentHP, FullHP;
		
				(*istr) >> type;
				(*istr) >> cha_index;
				if (bHp_Percentage) // HP_PERCENTAGE MSG_PC_REBIRTH rep부분 [3/29/2013 Ranma]
				{
					FLOAT hp_percentage;
					(*istr) >> hp_percentage;
					(*istr) >> hp;
					(*istr) >> maxHp;
					CurrentHP = HP_FloatToInt(hp_percentage, 10);
					FullHP = 1000;
				}
				else
				{
					(*istr) >> hp;
					(*istr) >> maxHp;
					CurrentHP = hp;
					FullHP = maxHp;
				}
			
				(*istr) >> mp;
				(*istr) >> maxMp;
				
				if(cha_index == _pNetwork->MyCharacterInfo.index)
				{
					m_bReadySendSkillMessage		= TRUE;
					_pNetwork->MyCharacterInfo.hp = hp; 
					_pNetwork->MyCharacterInfo.maxHP = maxHp;
					_pNetwork->MyCharacterInfo.mp = mp; 
					_pNetwork->MyCharacterInfo.maxMP = maxMp;
					// 김영환
					_pNetwork->Set_MyChar_MorphStatus_MORPH_END();
					//_pNetwork->MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_END;
					//ttos_080428: 공성시 힐러의 리저렉션으로 부활시 MoveLock 걸림 방지
					if( pUIManager->GetSiegeWarfareNew()->GetWarState() &&
						_pNetwork->MyCharacterInfo.sbAttributePos & MATT_WAR &&
						_pNetwork->MyCharacterInfo.sbJoinFlagDratan != WCJF_NONE )
					{
					//	_pUISWDoc->SetUIState(SWS_NONE);
						if(pUIManager->GetSiegeWarfareNew()->GetWaitTime() > 0)
						{
							pUIManager->RearrangeOrder( UI_SIEGE_WARFARE_NEW, FALSE );
						}									
					}
					
					// ITS#4463 : BUG-FIXED 나이트셰도우 사망 시 날개 이팩트 및 혼 이팩트 오류 입니다. [10/12/2011 rumist]
					// 사망시 무기를 제거하지 않기때문에 flying 애니메이션으로 처리되어서 차후 flying->stand전환시
					// 혼 이펙트가 flying으로 전환된 채로 출력되고 있음.
					if ((_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING))
					{
						DeleteWearingWeapon(FALSE, TRUE);
						_pNetwork->MyCharacterInfo.bExtension = 0;
					}

					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Rebirth();
					break;
				}
				
				SLONG chaID = _pNetwork->SearchClientChaIndex(cha_index);
				if(chaID != -1)
				{
					if (_pNetwork->ga_World.EntityExists(chaID, penEntity)) 
					{	
						
						((CUnit*)penEntity)->m_nMaxiHealth		= FullHP;
						((CUnit*)penEntity)->m_nCurrentHealth	= CurrentHP;
						((CCharacterBase*)penEntity)->Rebirth();	
					}
				}

			}
			break;			

		case MSG_ITEM:
			{
				Read_net_Character_Item(sub_type, istr);
			}
			break;

		case MSG_ACTION:
			{
				ResponseClient::action* pPack = reinterpret_cast<ResponseClient::action*>(istr->GetBuffer());
				CUIManager* pUIManager = CUIManager::getSingleton();
								
				if(pPack->charIndex == _pNetwork->MyCharacterInfo.index)
				{
					CTString strSysMessage;					

					switch(pPack->actionvalue)
					{
						case ACTION_NUM_PK:
							if((pPack->state & PLAYER_STATE_PKMODE) && (pPack->state & PLAYER_STATE_PKMODEDELAY))
							{
								//해제중.
								strSysMessage.PrintF( _S( 693, "10초 후에 PK모드가 해제됩니다." ));		
								_pNetwork->MyCharacterInfo.pk_mode = 2;

							}
							else if(pPack->state & PLAYER_STATE_PKMODE)
							{
								if( GetPlayerWeapons()->m_penRayHitTmp && 
									GetPlayerWeapons()->m_penRayHitTmp->IsCharacter() )//1215 
								{
									GetPlayerWeapons()->m_penReservedTarget = GetPlayerWeapons()->m_penRayHitTmp;
									GetPlayerWeapons()->m_penRayHitTmp = NULL;
								}
								// wooss 051026 조건문 추가(애완동물 소환시 PvP토글문제로)
								if(!_pNetwork->m_bIsPvp) {_pNetwork->SetPvpMode();}//선언
								strSysMessage.PrintF( _S( 694, "PK 모드를 선언하였습니다!\n" ) );		
								_pNetwork->MyCharacterInfo.pk_mode = 1;
								
							}
							else
							{
								if(_pNetwork->m_bIsPvp) {_pNetwork->SetPvpMode();} //해제
								strSysMessage.PrintF( _S( 695, "PK 모드가 해제되었습니다!\n" ) );		
								_pNetwork->MyCharacterInfo.pk_mode = 0;
							}	
								_pNetwork->ClientSystemMessage( strSysMessage );				
							break;
						default:
							break;
					}
					
					break;
				}

				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pPack->charIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);
					ObjInfo* pInfo = ObjInfo::getSingleton();

					pTarget->SetChaState(pPack->state);
					
					penEntity = pTarget->GetEntity();
					//if (_pNetwork->ga_World.EntityExists(ct.m_nIdxClient, penEntity)) 
					{				
						((CUnit*)penEntity)->m_nPlayActionNum = (SLONG)pPack->actionvalue;

						switch(pPack->actionvalue)
						{
						case ACTION_NUM_GREET:
						case ACTION_NUM_SMILE:
						case ACTION_NUM_CRY	:
						case ACTION_NUM_NUMBER_ONE :
						case ACTION_NUM_HANDCLAP :
						case ACTION_NUM_REFUSE :
						case ACTION_NUM_GOOD_LOOKS :
						case ACTION_NUM_GOOD_LOOKS2 :
						case ACTION_NUM_BANTER :
						case ACTION_NUM_CHEER :
						case ACTION_NUM_COMBAT :
						case ACTION_NUM_SUBMISSION :
						case ACTION_NUM_WATER_SPREAD:
							((CUnit*)penEntity)->ActionNow();		
							break;


						case ACTION_NUM_SITDOWN:
							if(pPack->state & PLAYER_STATE_SITDOWN)
							{
								((CCharacter*)penEntity)->m_nSitDown = 2;//앉기.	
							}
							else
							{
								((CCharacter*)penEntity)->m_nSitDown = 0;//서기.	
								AppearWearingWeapon(m_bSkilling);
							}
							((CUnit*)penEntity)->ActionNow();		
							break;

						case ACTION_NUM_PK: 
							if((pPack->state & PLAYER_STATE_PKMODE) && (pPack->state & PLAYER_STATE_PKMODEDELAY))
							{
								((CCharacter*)penEntity)->m_nPkMode = 2;									
							}
							else if(pPack->state & PLAYER_STATE_PKMODE)
							{
								((CCharacter*)penEntity)->m_nPkMode = 1;										
							}
							else
							{
								((CCharacter*)penEntity)->m_nPkMode = 0;										
							}		

							if(pInfo->GetTargetEntity(eTARGET) == penEntity)//타겟팅이 되어있다면...
							{																
								penEntity->UpdateTargetInfo(((CCharacter*)((CEntity*) penEntity))->m_nMaxiHealth,
									((CCharacter*)((CEntity*) penEntity))->m_nCurrentHealth,
									((CCharacter*)((CEntity*) penEntity))->m_nPkMode, 
									((CCharacter*)((CEntity*) penEntity))->m_nPkState, 
									((CCharacter*)((CEntity*) penEntity))->m_nLegit );
							}
							if(pInfo->GetTargetEntity(eTARGET_REAL) == penEntity)
							{
								penEntity->SetTargetInfoReal( 0, 0,
									0,
									((CCharacter*)((CEntity*) penEntity))->m_nPkMode, 
									((CCharacter*)((CEntity*) penEntity))->m_nPkState, 
									0,		// llCount
									((CCharacter*)((CEntity*) penEntity))->m_nLegit, 
									pTarget
									);
							}

							break;
						}																															
					}
				}
			}
			break;

		case MSG_SKILL:
			{
				Read_net_Character_Skill(sub_type, istr);
			}
			break;		

		//0601 kwon 추가.
		case MSG_GOTO:
			{
				ResponseClient::goMsg* pPack = reinterpret_cast<ResponseClient::goMsg*>(istr->GetBuffer());
				
				// wooss 060515 ADD move lock form server -------------------------->>
				m_checkLock = pPack->hack;

				if(m_checkLock == 1) { 
					m_bLockMove = TRUE; 
					m_timeCnt = _pTimer->GetHighPrecisionTimer().GetSeconds();
				}
				else {
					//이동 된 후 모든 상태 초기화
					PlayerInit(false);
				}
				// -----------------------------------------------------------------<<

				_pNetwork->MyCharacterInfo.x = pPack->x;
				_pNetwork->MyCharacterInfo.z = pPack->z;
				_pNetwork->MyCharacterInfo.h = pPack->h;
				_pNetwork->MyCharacterInfo.r = pPack->r;
				_pNetwork->MyCharacterInfo.yLayer = pPack->cYlayer;//1013

				//0819
				_pNetwork->DeleteAllMob();
				//0825
				//Init_DeathTable();	 			

				m_fCameraAngle = 0.0f;

				//0824 액션 취소.
				m_bPlayAction = FALSE;
				m_nPlayActionNum = -1;
				m_nActionSit = 0;
				
				//m_bRunningSelfSkill = FALSE;	
				m_bDying			= FALSE;
				m_bRcvAtMsg			= TRUE;														
			//	m_bLockMove			= FALSE;
				m_bReserveMove		= FALSE;
				m_bProduction		= FALSE;
				m_nProductionNum	= -1;

				if (!(_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING))
				{
					AppearWearingWeapon(FALSE);
				}

				if (IsFlagOn(ENF_ALIVE))
				{
					// [redmine #7261] 텔레포트 및 존이동시에 케릭터가 공중에서 리젠되는 문제 수정(리젠시 높이가 10 -> 1m 수정)[3/7/2012 ldy1978220]
					GoTo( _pNetwork->MyCharacterInfo.x, _pNetwork->MyCharacterInfo.h + 1.0f/*+ 10.0f*/, _pNetwork->MyCharacterInfo.z,
							_pNetwork->MyCharacterInfo.r + 10.0f );
				}
				//CPrintF(TRANS("MY CHA GOTO :(%f,%f,%f)\n"),_pNetwork->MyCharacterInfo.x,_pNetwork->MyCharacterInfo.h,_pNetwork->MyCharacterInfo.z );		
			}
			break;

		case MSG_AT:
			{
				if( !m_bIsTransform )
				{		
					const int iJobType = en_pcCharacter.pc_iPlayerType;
					SetSkaModel(CJobInfo::getSingleton()->GetFileName(iJobType));
					CModelInstance* pMI = GetModelInstance();
					if(pMI)
					{
						//	김영환
						const SBYTE sbFaceStyle = _pNetwork->Get_MyChar_faceStyle();
						const SBYTE sbHairStyle = _pNetwork->Get_MyChar_hairStyle();
							//const SBYTE sbFaceStyle = _pNetwork->MyCharacterInfo.faceStyle;
					//const SBYTE sbHairStyle = _pNetwork->MyCharacterInfo.hairStyle;
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetCharacterAppearance(pMI, iJobType, sbHairStyle, sbFaceStyle);				
					}
					m_bTransforming = FALSE;
				}
	// Date : 2006-04-26(오전 11:45:09), By eons////////////////////
//	필요 없는 부분 같음( 변신체 초기화는 ReceiveGoToMessage()에서 처리 )
				m_bChanging = FALSE;				
				if(m_bMobChange)
				{
					ReturnChange(TRUE);
					const int iStopChangeItem = 521;

					Notice* pNotice = GAMEDATAMGR()->GetNotice();

					if (pNotice != NULL)
					{
						pNotice->DelFromNoticeList(iStopChangeItem, Notice::NOTICE_POLYMOPH);
					}
				}
										/////////////////////////////////////////////////////////////////	
			/*
			// 변신체일 경우.
			else if( m_bIsTransform )
			{
				ReturnSorcerer();
				const int iStopTransformItem = 522;
				pUIManager->GetNotice()->DelFromNoticeList(iStopTransformItem, NOTICE_TRANSFORM);
				pUIManager->GetNotice()->RefreshNoticeList();
			}
			*/

				GoTo( _pNetwork->MyCharacterInfo.x, _pNetwork->MyCharacterInfo.h + 0.5f, _pNetwork->MyCharacterInfo.z,
						_pNetwork->MyCharacterInfo.r );
				plPlacement.pl_OrientationAngle(1) = _pNetwork->MyCharacterInfo.r;
				m_bDying = FALSE;
				m_bRcvAtMsg = TRUE;
				m_nActionSit = 0;//서있는 상태로 시작.
				m_bPlayAction = 0;
				m_bSendStopMessage = TRUE;
				m_nLegit = 0;
				m_bStuned = FALSE;
				m_bWaitForSkillResponse = FALSE;
				//m_bRunningSelfSkill = FALSE;

				if(IsPvp())//시작시 pk모드 해제.
				{	
					_pNetwork->SetPvpMode();
				}
				en_plViewpoint.pl_OrientationAngle(1) = 0.0f;//월드 이동시 카메라는 항상 북쪽을 향하게 한다.
						SetFlags(GetFlags()|ENF_ALIVE);//0830
//강동민 수정 시작 다중 공격 작업	08.27
			// FIXME : 싱글던젼에서 죽었을때 문제가 되서 추가한 부분.
				SetPhysicsFlags(EPF_MODEL_WALKING);
				SetCollisionFlags(ECF_MODEL);
	//강동민 수정 끝 다중 공격 작업		08.27
//안태훈 수정 시작	//(Open beta)(2004-12-14)
				if(GetModelInstance())
				{
					CSkaTag tag;
					tag.SetName("__ROOT");
					tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
					GetModelInstance()->m_tmSkaTagManager.Register(&tag);
					tag.SetName("__TOP");
					tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
					FLOATaabbox3D aabb;
					GetModelInstance()->GetAllFramesBBox(aabb);
					tag.SetOffsetPos(0, aabb.Size()(2) * GetModelInstance()->mi_vStretch(2), 0);
					GetModelInstance()->m_tmSkaTagManager.Register(&tag);
					GetModelInstance()->m_tmSkaTagManager.SetOwner(this);
				}
	//안태훈 수정 끝	//(Open beta)(2004-12-14)
			//CPrintF(TRANS("MY CHA START POINT :(%f,%f,%f)\n"),_pNetwork->MyCharacterInfo.x,_pNetwork->MyCharacterInfo.h,_pNetwork->MyCharacterInfo.z );
				AppearWearingWeapon(FALSE);

				if(m_bIsTransform)
				{
					TransformSorcerer(m_iTransformType);
				}
				
				// edit by cpp2angel (044.12.20) : 자동 도움말
				if ( g_slZone == 4 ) // 드라탄이면..
				{
					_UIAutoHelp->SetInfo ( AU_MOVE_DRATAN );
				}	

						// ... End Edit
			// (eons) 신전 과 필드 이동시 지역표시 문제 수정
			// 단지 위치 표시만 하고 다시 Reset된다.
				ShowSignBoard ( _pNetwork->MyCharacterInfo.LocalNo );
				_pNetwork->MyCharacterInfo.LocalNo = 0;
				
					// WSS_DRATAN_SIEGEWARFARE 2007/10/15
			// Set Default Animation			
				if( _pNetwork->MyCharacterInfo.bConsensus )
				{
					_pNetwork->MyCharacterInfo.bConsensus = FALSE;
				}
			}
			break;

		// FIXME : 타겟만 정렬될 수 있다면.
		// FIXME : 하나로 묶일 수가 있는 부분임.
		// FIXME : MSG_MOVE와 관련되서 중복되는 부분이 엄청 많음.
		case MSG_MOVE:
			{
				ResponseClient::moveMsg* pPack = reinterpret_cast<ResponseClient::moveMsg*>(istr->GetBuffer());
				
				vDesiredPosition(1) = pPack->x;
				vDesiredPosition(2) = pPack->h+1;
				vDesiredPosition(3) = pPack->z;

				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pPack->charIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					//if (_pNetwork->ga_World.EntityExists(ct.m_nIdxClient,penEntity)) 
					penEntity = pTarget->GetEntity();

					pTarget->SetyLayer( pPack->cYlayer );

					if (pPack->moveType == MSG_MOVE_PLACE)
					{
						((CCharacterBase*)pTarget->m_pEntity)->StopNow();

						CPlacement3D place(vDesiredPosition, ANGLE3D(0, 0, 0));								
						((CUnit*)penEntity)->SetPlacement(place);								
						return;
					}

					if ( !(pPack->mapAttr & MATT_UNWALKABLE) )
					{
						pTarget->cha_sbAttributePos = pPack->mapAttr;
					}

					((CUnit*)penEntity)->m_vDesiredPosition = vDesiredPosition;	
					((CUnit*)penEntity)->m_aDesiredRotation = ANGLE3D(pPack->r, 0, 0);
					if(pPack->moveType==MSG_MOVE_RUN || pPack->moveType==MSG_MOVE_WALK || pPack->moveType == MSG_MOVE_FLY) //캐릭터 이동.
					{
						//ttos_080422 : 이속 감소시 스피트값이 음수 값이 올 수 있으므로 스피드 제한 걸어줌 
						// 이렇게 안하면 MovableEntity.es에서 GetRelativeHeading()함수 내에서 계산시 flow 발생함
						if (pPack->speed < 0 )
						{
							pPack->speed = 0.1f;
						}

						((CUnit*)penEntity)->m_fMoveSpeed = pPack->speed;								
						((CUnit*)penEntity)->MoveNow();	

						if(m_bProduction)
						{
							AppearWearingWeapon(FALSE);
						}

					}
					else if(pPack->moveType == MSG_MOVE_STOP) //캐릭터 정지
					{								
						((CUnit*)penEntity)->m_fMoveSpeed = 5.0f;
						((CUnit*)penEntity)->StopandTeleport();
					}
				}
			}
			break;

		default: 
			break;
		}
	}

	void Read_net_Character_Item(int sub_type, CNetworkMessage *istr)
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();
		SLONG item_index;
		SLONG cha_index;

		switch (sub_type)
		{
		case MSG_ITEM_GET:
			{
				CTString	strName;
				SQUAD		slCount;
				(*istr) >> strName;
				(*istr) >> item_index;
				(*istr) >> slCount;

				CTString strMessage;
				const char* szItemName = _pNetwork->GetItemName(item_index);
				// <FIX ME> 
				// 이 스택 안에서 getSingleton()호출시 singleton 객체가 NULL로 정의되어, 새로 생성하게 된다.
				// 그로인해, UIManager 객체를 정상적으로 사용할 수 없었다.
				// _SMgr, _S2Mgr

				if(pUIManager->IsCSFlagOn(CSF_EXPEDITION)) // [sora] 원정대 아이템 획득 추가
				{
					//strMessage.PrintF( _S(4682, "[원정대] %s님이 %s를 %ld개 획득하였습니다." ), strName, szItemName, slCount);
					strMessage.PrintF( _SMgr( pUIManager, 4682, "[원정대] %s님이 %s를 %ld개 획득하였습니다." ), strName, szItemName, slCount);
				}
				else
				{
					//strMessage.PrintF( _S2( 707, CTString(szItemName), "[파티원] %s님이 %s<를> %ld개 획득하였습니다." ), strName, szItemName, slCount);
					strMessage.PrintF( _S2Mgr( pUIManager, 707, szItemName, "[파티원] %s님이 %s<를> %ld개 획득하였습니다." ), strName, szItemName, slCount);
				}
				_pNetwork->ClientSystemMessage(strMessage);					
			}
			break;
		case MSG_ITEM_TAKE:
			{
				SBYTE sbType;
				(*istr) >> sbType;
				(*istr) >> cha_index;
				(*istr) >> item_index;	

				if( sbType == MSG_CHAR_PC && cha_index == _pNetwork->MyCharacterInfo.index )	
				{
				}
				else
				{
					CEntity *penEntity = NULL;
					if( _pNetwork->SearchEntityByNetworkID( cha_index, sbType, penEntity ) )
					{
						((CUnit*)penEntity)->m_nPlayActionNum = (SLONG)ACTION_NUM_PICKITEM;
						((CUnit*)penEntity)->ActionNow();						
					}
				}

				ACTORMGR()->RemoveObject(eOBJ_ITEM, item_index);
			}
			break;
		}
	}

	void Read_net_Character_Skill(int sub_type, CNetworkMessage* istr)
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();
		CEntity* penEntity;
		CEntity* penTargetEntity;

		switch (sub_type)
		{
		case MSG_SKILL_FIRE: //0804 스킬 파이어.
			{
				ResponseClient::skillFireMsg* pPack = reinterpret_cast<ResponseClient::skillFireMsg*>(istr->GetBuffer());
				ResponseClient::skillFireMsg::tag_list* pList = NULL;
				
				switch (pPack->skillIndex)
				{
				case 3:
				case 32:
				case 33:
				case 44:
				case 64:
				case 123:
				case 125:
				case 129:
					pPack->skillSpeed = 0;
					break;
				}
				
				bool bMy = false;

				switch (pPack->charType)
				{
				case MSG_CHAR_PC:
					{
						if(pPack->charIndex == _pNetwork->MyCharacterInfo.index)
						{
							bMy = true;
						}
					}
					break;
				case MSG_CHAR_PET:
					{
						ObjectBase* pObject  = ACTORMGR()->GetObject(eOBJ_PET, pPack->charIndex);

						if (pObject != NULL)
						{
							if (CPetTarget* pTarget = static_cast< CPetTarget* >(pObject))
							{
								if (pTarget->pet_OwnerIndex == _pNetwork->MyCharacterInfo.index)
								{
									bMy = true;
								}
							}
						}
					}
					break;
				case MSG_CHAR_ELEMENTAL:
					{
						ObjectBase* pObject  = ACTORMGR()->GetObject(eOBJ_SLAVE, pPack->charIndex);

						if (pObject != NULL)
						{
							if (CSlaveTarget* pTarget = static_cast< CSlaveTarget* >(pObject))
							{
								if (pTarget->slave_OwnerIndex == _pNetwork->MyCharacterInfo.index)
								{
									bMy = true;
								}
							}
						}
					}
					break;
				case MSG_CHAR_WILDPET:
					{
						ObjectBase* pObject  = ACTORMGR()->GetObject(eOBJ_WILDPET, pPack->charIndex);

						if (pObject != NULL)
						{
							if (CWildPetTarget* pTarget = static_cast< CWildPetTarget* >(pObject))
							{
								if (pTarget->m_nOwnerIndex == _pNetwork->MyCharacterInfo.index)
								{
									bMy = true;
								}
							}
						}
					}
					break;
				
				}

				if (bMy == true)
				{
					CSkill &SkillData = _pNetwork->GetSkillData(pPack->skillIndex);
					SkillData.SetStartTime();

					pUIManager->SetCSFlagOff(CSF_SKILLREADY);
					if(pPack->charIndex == _pNetwork->MyCharacterInfo.index)
					{
						return;
					}
				}


				if( pPack->skillIndex == HE_SHINING_ARROW || 
					pPack->skillIndex == HE_STORM_ARROW || 
					pPack->skillIndex == HE_DOUBLE_STING ||
					pPack->skillIndex == 128 //로그, 다이어 스트라이크					
					)
				{
					break;
				}

				if( _pNetwork->SearchEntityByNetworkID( pPack->charIndex, pPack->charType, penEntity ) )
				{					
					if( _pNetwork->SearchEntityByNetworkID( pPack->targetIndex, pPack->targetType, penTargetEntity ) )
					{
						((CUnit*)penEntity)->m_dcEnemies.Add(penTargetEntity);

						if ( penEntity != penTargetEntity)
						{
							((CUnit*)penEntity)->SetTargetEntity(penTargetEntity);
						}						
					}

					if (pPack->listCount > 0)
					{
						pList = new ResponseClient::skillFireMsg::tag_list[pPack->listCount];
						memcpy(pList , &pPack->list[0], sizeof(ResponseClient::skillFireMsg::tag_list) * pPack->listCount);
					}

					for( int t = 0; t < pPack->listCount; ++t )
					{
						CEntity *penTargetEntity = NULL;
						if( _pNetwork->SearchEntityByNetworkID( pList[t].multiIndex, pList[t].multiType, penTargetEntity ) )
						{						
							((CUnit*)penEntity)->m_dcEnemies.Add(penTargetEntity);

							if ( penTargetEntity->IsEnemy() && ((CUnit*)penTargetEntity)->m_nCurrentHealth > 0 )
							{
								((CUnit*)penEntity)->SetTargetEntity(penTargetEntity);
							}
						}
					}
					((CUnit*)penEntity)->m_nCurrentSkillNum = pPack->skillIndex;
					((CUnit*)penEntity)->m_fSkillSpeed = (100 - pPack->skillSpeed) / 100.0f;
					((CUnit*)penEntity)->SkillNow();
					SAFE_ARRAY_DELETE(pList);
					break;
				}
			}
			break;

		case MSG_SKILL_READY: 
			{
				ResponseClient::skillReadyMsg* pPack = reinterpret_cast<ResponseClient::skillReadyMsg*>(istr->GetBuffer());
											
				switch (pPack->skillIndex)
				{
				case 3:
				case 32:
				case 33:
				case 44:
				case 64:
				case 123:
				case 125:
				case 129:
					pPack->skillSpeed = 0;
					break;
				}
				// 내가 스킬을 쓸때
				if(pPack->charIndex == _pNetwork->MyCharacterInfo.index)//이제 스킬 파이어 메시지를 보내야한다.
				{
					if(pPack->skillIndex != -1)
					{
						SpellSkill(pPack->skillIndex);
						m_bWaitForSkillResponse = FALSE;
						m_bLockSkillCancel = TRUE;
					}
					break;
				}

				// 나를 제외한 오브젝트가 스킬을 쓸때
				if( _pNetwork->SearchEntityByNetworkID( pPack->charIndex, pPack->charType, penEntity) )
				{
					if( _pNetwork->SearchEntityByNetworkID( pPack->targetIndex, pPack->targetType, penTargetEntity) )
					{
						((CUnit*)penEntity)->SetTargetEntity(penTargetEntity);
						((CUnit*)penEntity)->m_nCurrentSkillNum = pPack->skillIndex;
						((CUnit*)penEntity)->m_fSkillSpeed = (100 - pPack->skillSpeed) / 100.0f;

						if(	pPack->skillIndex == HE_SHINING_ARROW || 
							pPack->skillIndex == HE_STORM_ARROW || 
							pPack->skillIndex == HE_DOUBLE_STING || 
							pPack->skillIndex == 128 //로그, 다이어 스트라이크								
							)
						{
							((CUnit*)penEntity)->SkillNow();
						}
						else
						{
							((CUnit*)penEntity)->PreSkillNow();																			
						}
						//CPrintF("Skill: %d -> %d \n",attackID,targetID);	
						break;
					}
				}
			}//0920
			break;

		case MSG_SKILL_CANCEL://1013
			{
				ResponseClient::skillCancel* pPack = reinterpret_cast<ResponseClient::skillCancel*>(istr->GetBuffer());

				pUIManager->SetCSFlagOff(CSF_SKILLREADY);
				
				if(pPack->charType == MSG_CHAR_PC)
				{					
					if(pPack->charIndex == _pNetwork->MyCharacterInfo.index)
					{
						CancelSkill(FALSE, g_iAutoAttack, FALSE);
						return;
					}
				}

				if( _pNetwork->SearchEntityByNetworkID( pPack->charIndex, pPack->charType, penEntity ) )
				{
					if( pPack->charType == MSG_CHAR_NPC || pPack->charType == MSG_CHAR_PET || pPack->charType == MSG_CHAR_WILDPET)
					{
						if(!_pNetwork->m_bSingleMode)
						{
							((CUnit*)penEntity)->StopNow();
						}
					}
					else
					{
						((CUnit*)penEntity)->StopNow();
					}
				}
			}
			break;
		}
	}
		
	INDEX GenderSound(INDEX iSound)
	{
		return iSound+m_iGender*GENDEROFFSET;
	}
	
	void AddBouble( FLOAT3D vPos, FLOAT3D vSpeedRelative)
	{
		ShellLaunchData &sld = m_asldData[m_iFirstEmptySLD];
		sld.sld_vPos = vPos;
		const FLOATmatrix3D &m = GetRotationMatrix();
		FLOAT3D vUp( m(1,2), m(2,2), m(3,2));
		sld.sld_vUp = vUp;
		sld.sld_vSpeed = vSpeedRelative*m;
		sld.sld_tmLaunch = _pTimer->CurrentTick();
		sld.sld_estType = ESL_BUBBLE;
		// move to next shell position
		m_iFirstEmptySLD = (m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
	}
	
	void ClearShellLaunchData( void)
	{
		// clear flying shells data array
		m_iFirstEmptySLD = 0;
		for( INDEX iShell=0; iShell<MAX_FLYING_SHELLS; iShell++) {
			m_asldData[iShell].sld_estType  = ESL_NONE;
			m_asldData[iShell].sld_tmLaunch = -100.0f;
		}
	}
	
	void AddBulletSpray( FLOAT3D vPos, EffectParticlesType eptType, FLOAT3D vStretch)
	{
		BulletSprayLaunchData &bsld = m_absldData[m_iFirstEmptyBSLD];
		bsld.bsld_vPos = vPos;
		bsld.bsld_vG = en_vGravityDir;
		bsld.bsld_eptType=eptType;
		bsld.bsld_iRndBase=FRnd()*123456;
		bsld.bsld_tmLaunch = _pTimer->CurrentTick();
		bsld.bsld_vStretch=vStretch;
		// move to bullet spray position
		m_iFirstEmptyBSLD = (m_iFirstEmptyBSLD+1) % MAX_BULLET_SPRAYS;
	}
	
	void ClearBulletSprayLaunchData( void)
	{
		m_iFirstEmptyBSLD = 0;
		for( INDEX iBulletSpray=0; iBulletSpray<MAX_BULLET_SPRAYS; iBulletSpray++)
		{
			m_absldData[iBulletSpray].bsld_tmLaunch = -100.0f;
		}
	}
	
	void AddGoreSpray( FLOAT3D vPos, FLOAT3D v3rdPos, SprayParticlesType sptType, FLOAT3D vSpilDirection,
		FLOATaabbox3D boxHitted, FLOAT fDamagePower, COLOR colParticles)
	{
		GoreSprayLaunchData &gsld = m_agsldData[m_iFirstEmptyGSLD];
		gsld.gsld_vPos = vPos;
		gsld.gsld_v3rdPos = v3rdPos;
		gsld.gsld_vG = en_vGravityDir;
		gsld.gsld_fGA = en_fGravityA;
		gsld.gsld_sptType = sptType;
		gsld.gsld_boxHitted = boxHitted;
		gsld.gsld_vSpilDirection = vSpilDirection;
		gsld.gsld_fDamagePower=fDamagePower;
		gsld.gsld_tmLaunch = _pTimer->CurrentTick();
		gsld.gsld_colParticles = colParticles;
		// move to bullet spray position
		m_iFirstEmptyGSLD = (m_iFirstEmptyGSLD+1) % MAX_GORE_SPRAYS;
	}
	
	void ClearGoreSprayLaunchData( void)
	{
		m_iFirstEmptyGSLD = 0;
		for( INDEX iGoreSpray=0; iGoreSpray<MAX_GORE_SPRAYS; iGoreSpray++)
		{
			m_agsldData[iGoreSpray].gsld_tmLaunch = -100.0f;
		}
	}
	
	void CPlayer(void) 
	{
		// clear flying shells data array
		ClearShellLaunchData();
		ClearBulletSprayLaunchData();
		ClearGoreSprayLaunchData();
		
		// add all messages from First Encounter
		//CheatAllMessagesDir("Data\\Messages\\weapons\\", CMF_READ);
		//CheatAllMessagesDir("Data\\Messages\\enemies\\", CMF_READ);
		// ... or not

		m_bNoAniGuildSkill = FALSE;
	}
	
	class CPlayerWeapons *GetPlayerWeapons(void)
	{
		ASSERT(m_penWeapons!=NULL);
		return (CPlayerWeapons *)&*m_penWeapons;
	}
	class CPlayerAnimator *GetPlayerAnimator(void)
	{
		ASSERT(m_penAnimator!=NULL);
		return (CPlayerAnimator *)&*m_penAnimator;
	}
	
	CPlayerSettings *GetSettings(void)
	{
		return (CPlayerSettings *)en_pcCharacter.pc_aubAppearance;
	}
	
	export void Copy(CEntity &enOther, ULONG ulFlags)
	{
		CPlayerEntity::Copy(enOther, ulFlags);
		CPlayer *penOther = (CPlayer *)(&enOther);
		//0105
		if(m_bMdl){
			m_moRender.Copy(penOther->m_moRender);
		}else{
			m_miRender.Copy(penOther->m_miRender);
		}
		//..    
		m_psLevelStats = penOther->m_psLevelStats;
		m_psLevelTotal = penOther->m_psLevelTotal;
		m_psGameStats  = penOther->m_psGameStats ;
		m_psGameTotal  = penOther->m_psGameTotal ;
	}
	
	// update smoothed (average latency)
	void UpdateLatency(FLOAT tmLatencyNow)
	{
		TIME tmNow = _pTimer->GetHighPrecisionTimer().GetSeconds();
		
		// if not enough time passed
		if (tmNow<m_tmLatencyLastAvg+hud_tmLatencySnapshot) {
			// just sum
			m_tmLatencyAvgSum += tmLatencyNow;
			m_ctLatencyAvg++;
			
			// if enough time passed
		} else {
			// calculate average
			if ( m_ctLatencyAvg > 0.0f || m_ctLatencyAvg < 0.0f)
			{
				m_tmLatency = m_tmLatencyAvgSum/m_ctLatencyAvg;
			}
			else
			{
				m_tmLatency = 0.0f;
			}
			// reset counters
			m_tmLatencyAvgSum = 0.0f;
			m_ctLatencyAvg = 0;
			m_tmLatencyLastAvg = tmNow;
		}
		
		if (_pNetwork->IsPlayerLocal(this)) {
			en_tmPing = m_tmLatency;
			net_tmLatencyAvg = en_tmPing;
		}
	}
	
	// check character data for invalid values
	void ValidateCharacter(void)
	{
		// if in single player or flyover
		if (GetSP()->sp_bSinglePlayer) {
			// always use default model
			CPlayerSettings *pps = (CPlayerSettings *)en_pcCharacter.pc_aubAppearance;
			memset(pps->ps_achModelFile, 0, sizeof(pps->ps_achModelFile));
		}
	}
	// parse gender from your name
	void ParseGender(CTString &strName)
	{
		if (strName.RemovePrefix("#male#")) {
			m_iGender = GENDER_MALE;
		} else if (strName.RemovePrefix("#female#")) {
			m_iGender = GENDER_FEMALE;
		} else {
			m_iGender = GENDER_MALE;
		}
	}

	/* Write to stream. */
	void Write_t( CTStream *ostr,BOOL bNetwork) // throw char *
	{
		CPlayerEntity::Write_t(ostr,bNetwork);

		ostr->Write_t(&m_psLevelStats, sizeof(m_psLevelStats));
		ostr->Write_t(&m_psLevelTotal, sizeof(m_psLevelTotal));
		ostr->Write_t(&m_psGameStats , sizeof(m_psGameStats ));
		ostr->Write_t(&m_psGameTotal , sizeof(m_psGameTotal ));
	}
	/* Read from stream. */
	void Read_t( CTStream *istr,BOOL bNetwork) // throw char *
	{ 
		CPlayerEntity::Read_t(istr,bNetwork);
		// clear flying shells data array
		ClearShellLaunchData();
		ClearBulletSprayLaunchData();
		ClearGoreSprayLaunchData();		
		
		istr->Read_t(&m_psLevelStats, sizeof(m_psLevelStats));
		istr->Read_t(&m_psLevelTotal, sizeof(m_psLevelTotal));
		istr->Read_t(&m_psGameStats , sizeof(m_psGameStats ));
		istr->Read_t(&m_psGameTotal , sizeof(m_psGameTotal ));
		
		// set your real appearance if possible
		ValidateCharacter();
		CTString strDummy;
		//0105
		if(m_bMdl){
			SetPlayerAppearance(&m_moRender, &en_pcCharacter, strDummy, /*bPreview=*/FALSE);
		}else{
			CreateAnimAndBoneIDs();//로드할때는 BoneID를 다시 불러들여야한다.
			GetModelInstance()->StretchModel(FLOAT3D(0.4f,0.4f,0.4f));
			SetPlayerAppearanceSka(&m_miRender, &en_pcCharacter, strDummy, FALSE);	
		}
		//..    
		ParseGender(strDummy);
//		m_ulFlags |= PLF_SYNCWEAPON;
		// setup light source
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
		//SetupLightSource();
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
		
		m_tmLastPicked = -99.0f;
	};
	
	/* Get static light source information. */
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
/*
	CLightSource *GetLightSource(void)
	{
		return &m_lsLightSource;
	};
*/
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
	
	// get in-game time for statistics
	TIME GetStatsInGameTimeLevel(void)
	{
		if(m_bEndOfLevel) {
			return m_psLevelStats.ps_tmTime;
		} else {
			return _pNetwork->GetGameTime()-m_tmLevelStarted;
		}
	}
	TIME GetStatsInGameTimeGame(void)
	{
		if(m_bEndOfLevel) {
			return m_psGameStats.ps_tmTime;
		} else {
			return m_psGameStats.ps_tmTime + (_pNetwork->GetGameTime()-m_tmLevelStarted);
		}
	}
	
	FLOAT GetStatsRealWorldTime(void)
	{
		time_t timeNow;
		if(m_bEndOfLevel) { 
			timeNow = m_iEndTime; 
		} else {
			time(&timeNow);
		}
		return (FLOAT)difftime( timeNow, m_iStartTime);
	}
	
	CTString GetStatsRealWorldStarted(void)
	{
		struct tm *newtime;
		newtime = localtime((const time_t *)&m_iStartTime);
		
		setlocale(LC_ALL, "");
		CTString strTimeline;
		char achTimeLine[256]; 
		strftime( achTimeLine, sizeof(achTimeLine)-1, "%a %x %H:%M", newtime);
		strTimeline = achTimeLine;
		setlocale(LC_ALL, "C");
		return strTimeline;
	}
	
	// fill in player statistics
	export void GetStats( CTString &strStats, const CompStatType csType, INDEX ctCharsPerRow)
	{
		
		// get proper type of stats
		/*if( csType==CST_SHORT) {
			GetShortStats(strStats);
		} else {
			ASSERT(csType==CST_DETAIL);
			
			strStats = "\n";
			_ctAlignWidth = Min(ctCharsPerRow, INDEX(60));
			
			if (GetSP()->sp_bCooperative) {
				if (GetSP()->sp_bSinglePlayer) {
					GetDetailStatsSP(strStats, 0);
				} else {
					GetDetailStatsCoop(strStats);
				}
			} else {
				GetDetailStatsDM(strStats);
			}
		}*/
	}
	
	// get short one-line statistics - used for savegame descriptions and similar
	void GetShortStats(CTString &strStats)
	{
		strStats.PrintF( TRANS("%s Kills: %d/%d"), 
			TimeToString(GetStatsInGameTimeLevel()), m_psLevelStats.ps_iKills, m_psLevelTotal.ps_iKills);
	}
	
	// get detailed statistics for deathmatch game
	void GetDetailStatsDM(CTString &strStats)
	{
		extern INDEX SetAllPlayersStats( INDEX iSortKey);
		extern CPlayer *_apenPlayers[NET_MAXGAMEPLAYERS];
		// determine type of game
		const BOOL bFragMatch = GetSP()->sp_bUseFrags;
		
		// fill players table
		const INDEX ctPlayers = SetAllPlayersStats(bFragMatch?5:3); // sort by frags or by score
		
		// get time elapsed since the game start
		strStats+=AlignString(CTString(0, "^cFFFFFF%s:^r\n%s", TRANS("TIME"), TimeToString(_pNetwork->GetGameTime())));
		strStats+="\n";
		
		// find maximum frags/score that one player has
		INDEX iMaxFrags = LowerLimit(INDEX(0));
		INDEX iMaxScore = LowerLimit(INDEX(0));
		{for(INDEX iPlayer=0; iPlayer<ctPlayers; iPlayer++) {
			CPlayer *penPlayer = _apenPlayers[iPlayer];
			iMaxFrags = Max(iMaxFrags, penPlayer->m_psLevelStats.ps_iKills);
			//iMaxScore = Max(iMaxScore, penPlayer->m_psLevelStats.ps_iScore);
		}}
		
		// print game limits
		const CSessionProperties &sp = *GetSP();
		if (sp.sp_iTimeLimit>0) {
			FLOAT fTimeLeft = ClampDn(sp.sp_iTimeLimit*60.0f - _pNetwork->GetGameTime(), 0.0f);
			strStats+=AlignString(CTString(0, "^cFFFFFF%s:^r\n%s", TRANS("TIME LEFT"), TimeToString(fTimeLeft)));
			strStats+="\n";
		}
		if (bFragMatch && sp.sp_iFragLimit>0) {
			INDEX iFragsLeft = ClampDn(sp.sp_iFragLimit-iMaxFrags, INDEX(0));
			strStats+=AlignString(CTString(0, "^cFFFFFF%s:^r\n%d", TRANS("FRAGS LEFT"), iFragsLeft));
			strStats+="\n";
		}
		/*if (!bFragMatch && sp.sp_iScoreLimit>0) {
			INDEX iScoreLeft = ClampDn(sp.sp_iScoreLimit-iMaxScore, INDEX(0));
			strStats+=AlignString(CTString(0, "^cFFFFFF%s:^r\n%d", TRANS("SCORE LEFT"), iScoreLeft));
			strStats+="\n";
		}*/
		strStats += "\n";
		
		CTString strRank = TRANS("NO.");
		CTString strFrag = bFragMatch ? TRANS("FRAGS"):TRANS("SCORE");
		CTString strPing = TRANS("PING");
		CTString strName = TRANS("PLAYER");
		INDEX ctRankChars = Max(strRank.Length(), INDEX(3)) ;
		INDEX ctFragChars = Max(strFrag.Length(), INDEX(7)) ;
		INDEX ctPingChars = Max(strPing.Length(), INDEX(5)) ;
		INDEX ctNameChars = Max(strName.Length(), INDEX(20));
		
		// header
		strStats += "^cFFFFFF";
		strStats += PadStringRight(strRank, ctRankChars)+" ";
		strStats += PadStringLeft (strFrag, ctFragChars)+" ";
		strStats += PadStringLeft (strPing, ctPingChars)+" ";
		strStats += PadStringRight(strName, ctNameChars)+" ";
		strStats += "^r";
		strStats += "\n\n";
		{for(INDEX iPlayer=0; iPlayer<ctPlayers; iPlayer++) {
			CTString strLine;
			CPlayer *penPlayer = _apenPlayers[iPlayer];
			INDEX iPing = ceil(penPlayer->en_tmPing*1000.0f);
//			INDEX iScore = bFragMatch ? penPlayer->m_psLevelStats.ps_iKills : penPlayer->m_psLevelStats.ps_iScore;
			CTString strName = penPlayer->GetPlayerName();
			
			strStats += PadStringRight(CTString(0, "%d", iPlayer+1), ctRankChars)+" ";
//			strStats += PadStringLeft (CTString(0, "%d", iScore),    ctFragChars)+" ";
			strStats += PadStringLeft (CTString(0, "%d", iPing),     ctPingChars)+" ";
			strStats += PadStringRight(strName,                      ctNameChars)+" ";
			strStats += "\n";
		}}
	}
	
	// get singleplayer statistics
	void GetDetailStatsCoop(CTString &strStats)
	{
		// first put in your full stats
		strStats += "^b"+CenterString(TRANS("YOUR STATS"))+"^r\n";
		strStats+="\n";
		GetDetailStatsSP(strStats, 1);
		
		// get stats from all players
		extern INDEX SetAllPlayersStats( INDEX iSortKey);
		extern CPlayer *_apenPlayers[NET_MAXGAMEPLAYERS];
		const INDEX ctPlayers = SetAllPlayersStats(3); // sort by score
		
		// for each player
		PlayerStats psSquadLevel = PlayerStats();
		PlayerStats psSquadGame  = PlayerStats();
		{for( INDEX iPlayer=0; iPlayer<ctPlayers; iPlayer++) {
			CPlayer *penPlayer = _apenPlayers[iPlayer];
			// add values to squad stats
			ASSERT( penPlayer!=NULL);
			PlayerStats psLevel = penPlayer->m_psLevelStats;
			PlayerStats psGame  = penPlayer->m_psGameStats ;
//			psSquadLevel.ps_iScore   += psLevel.ps_iScore   ;
			psSquadLevel.ps_iKills   += psLevel.ps_iKills   ;
			psSquadLevel.ps_iDeaths  += psLevel.ps_iDeaths  ;
//			psSquadLevel.ps_iSecrets += psLevel.ps_iSecrets ;
//			psSquadGame.ps_iScore    += psGame.ps_iScore   ;
			psSquadGame.ps_iKills    += psGame.ps_iKills   ;
			psSquadGame.ps_iDeaths   += psGame.ps_iDeaths  ;
//			psSquadGame.ps_iSecrets  += psGame.ps_iSecrets ;
		}}
		
		// add squad stats
		strStats+="\n";
		strStats += "^b"+CenterString(TRANS("SQUAD TOTAL"))+"^r\n";
		strStats+="\n";
		strStats+=CTString(0, "^cFFFFFF%s^r", TranslateConst(en_pwoWorld->GetName(), 0));
		strStats+="\n";
//		strStats+=AlignString(CTString(0, "  %s:\n%d", TRANS("SCORE"), psSquadLevel.ps_iScore));
//		strStats+="\n";
		strStats+=AlignString(CTString(0, "  %s:\n%d", TRANS("DEATHS"), psSquadLevel.ps_iDeaths));
		strStats+="\n";
		strStats+=AlignString(CTString(0, "  %s:\n%d/%d", TRANS("KILLS"), psSquadLevel.ps_iKills, m_psLevelTotal.ps_iKills));
		strStats+="\n";
//		strStats+=AlignString(CTString(0, "  %s:\n%d/%d", TRANS("SECRETS"), psSquadLevel.ps_iSecrets, m_psLevelTotal.ps_iSecrets));
//		strStats+="\n";
		strStats+="\n";
		strStats+=CTString("^cFFFFFF")+TRANS("TOTAL")+"^r\n";
//		strStats+=AlignString(CTString(0, "  %s:\n%d", TRANS("SCORE"), psSquadGame.ps_iScore));
//		strStats+="\n";
		strStats+=AlignString(CTString(0, "  %s:\n%d", TRANS("DEATHS"), psSquadGame.ps_iDeaths));
		strStats+="\n";
		strStats+=AlignString(CTString(0, "  %s:\n%d/%d", TRANS("KILLS"), psSquadGame.ps_iKills, m_psGameTotal.ps_iKills));
		strStats+="\n";
//		strStats+=AlignString(CTString(0, "  %s:\n%d/%d", TRANS("SECRETS"), psSquadGame.ps_iSecrets, m_psGameTotal.ps_iSecrets));
//		strStats+="\n";
		strStats+="\n";
		
		
		strStats+="\n";
		strStats += "^b"+CenterString(TRANS("OTHER PLAYERS"))+"^r\n";
		strStats+="\n";
		
		// for each player
		{for(INDEX iPlayer=0; iPlayer<ctPlayers; iPlayer++) {
			CPlayer *penPlayer = _apenPlayers[iPlayer];
			// if this one
			if (penPlayer==this) {
				// skip it
				continue;
			}
			// add his stats short
			strStats+="^cFFFFFF"+CenterString(penPlayer->GetPlayerName())+"^r\n\n";
			penPlayer->GetDetailStatsSP(strStats, 2);
			strStats+="\n";
		}}
	}
	
	// get singleplayer statistics
	void GetDetailStatsSP(CTString &strStats, INDEX iCoopType)
	{
		if (iCoopType<=1) 
		{
			if (m_bEndOfGame) 
			{
			/*
				if (GetSP()->sp_gdGameDifficulty==CSessionProperties::GD_EXTREME) 
				{
					strStats+=TRANS("^f4SERIOUS GAME FINISHED,\nMENTAL MODE IS NOW ENABLED!^F\n\n");
				} else 
				if (GetSP()->sp_bMental) {
					strStats+=TRANS("^f4YOU HAVE MASTERED THE GAME!^F\n\n");
				}
				*/
			}
		}
		
		if (iCoopType<=1) {
			// report total score info
//			strStats+=AlignString(CTString(0, "^cFFFFFF%s:^r\n%d", TRANS("TOTAL SCORE"), m_psGameStats.ps_iScore));
//			strStats+="\n";
			//strStats+=AlignString(CTString(0, "^cFFFFFF%s:^r\n%s", TRANS("DIFFICULTY"), GetDifficultyString()));
			//strStats+="\n";
			strStats+=AlignString(CTString(0, "^cFFFFFF%s:^r\n%s", TRANS("STARTED"), GetStatsRealWorldStarted()));
			strStats+="\n";
			strStats+=AlignString(CTString(0, "^cFFFFFF%s:^r\n%s", TRANS("PLAYING TIME"), TimeToString(GetStatsRealWorldTime())));
			strStats+="\n";
//			if( m_psGameStats.ps_iScore<=plr_iHiScore) {
//				strStats+=AlignString(CTString(0, "^cFFFFFF%s:^r\n%d", TRANS("HI-SCORE"), plr_iHiScore));
//			} else {
//				strStats+=TRANS("YOU BEAT THE HI-SCORE!");
//			}
			strStats+="\n\n";
		}
		
		// report this level statistics
		strStats+=CTString(0, "^cFFFFFF%s^r", TranslateConst(en_pwoWorld->GetName(), 0));
		strStats+="\n";
		if (iCoopType<=1) {
			if( m_bEndOfLevel) {
				strStats+=AlignString(CTString(0, "  %s:\n%s", TRANS("ESTIMATED TIME"), TimeToString(m_tmEstTime)));
				strStats+="\n";
				strStats+=AlignString(CTString(0, "  %s:\n%d", TRANS("TIME BONUS"), m_iTimeScore));
				strStats+="\n";
				strStats+="\n";
			}
			//    } else {
			//      strStats+=CTString("^cFFFFFF")+TRANS("THIS LEVEL")+"^r\n";
		}
//		strStats+=AlignString(CTString(0, "  %s:\n%d", TRANS("SCORE"), m_psLevelStats.ps_iScore));
//		strStats+="\n";
		strStats+=AlignString(CTString(0, "  %s:\n%d/%d", TRANS("KILLS"), m_psLevelStats.ps_iKills, m_psLevelTotal.ps_iKills));
		strStats+="\n";
		if (iCoopType>=1) {
			strStats+=AlignString(CTString(0, "  %s:\n%d", TRANS("DEATHS"), m_psLevelStats.ps_iDeaths, m_psLevelTotal.ps_iDeaths));
			strStats+="\n";
		}
//		strStats+=AlignString(CTString(0, "  %s:\n%d/%d", TRANS("SECRETS"), m_psLevelStats.ps_iSecrets, m_psLevelTotal.ps_iSecrets));
//		strStats+="\n";
		if (iCoopType<=1) {
			strStats+=AlignString(CTString(0, "  %s:\n%s", TRANS("TIME"), TimeToString(GetStatsInGameTimeLevel())));
			strStats+="\n";
		}
		strStats+="\n";
		
		// report total game statistics
		strStats+=CTString("^cFFFFFF")+TRANS("TOTAL")+"^r";
		strStats+="\n";
//		strStats+=AlignString(CTString(0, "  %s:\n%d", TRANS("SCORE"), m_psGameStats.ps_iScore));
//		strStats+="\n";
		strStats+=AlignString(CTString(0, "  %s:\n%d/%d", TRANS("KILLS"), m_psGameStats.ps_iKills, m_psGameTotal.ps_iKills));
		strStats+="\n";
		if (iCoopType>=1) {
			strStats+=AlignString(CTString(0, "  %s:\n%d", TRANS("DEATHS"), m_psGameStats.ps_iDeaths, m_psGameTotal.ps_iDeaths));
			strStats+="\n";
		}
		//strStats+=AlignString(CTString(0, "  %s:\n%d/%d", TRANS("SECRETS"), m_psGameStats.ps_iSecrets, m_psGameTotal.ps_iSecrets));
		//strStats+="\n";
		if (iCoopType<=1) {
			strStats+=AlignString(CTString(0, "  %s:\n%s", TRANS("GAME TIME"), TimeToString(GetStatsInGameTimeGame())));
			strStats+="\n";
		}
		strStats+="\n";
		
		// set per level outputs
		if (iCoopType<1) {
			if(m_strLevelStats!="") {
				strStats += CTString("^cFFFFFF")+TRANS("Per level statistics") +"^r\n\n" + m_strLevelStats;
			}
		}
	}
	
	void SayVoiceMessage(const CTFileName &fnmMessage)
	{
		if (GetSettings()->ps_ulFlags&PSF_NOQUOTES) {
			return;
		}		
//		PlaySound( m_soSpeech, fnmMessage, SOF_3D|SOF_VOLUMETRIC|SOF_VOICE);
	}

//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
/*
	// Setup light source
	void SetupLightSource(void)
	{
		// setup light source
		CLightSource lsNew;
		lsNew.ls_ulFlags = LSF_NONPERSISTENT|LSF_DYNAMIC;
		lsNew.ls_rHotSpot = 1.0f;
		lsNew.ls_colColor = C_WHITE;
		lsNew.ls_rFallOff = 2.5f;
		lsNew.ls_plftLensFlare = NULL;
		lsNew.ls_ubPolygonalMask = 0;
		lsNew.ls_paoLightAnimation = &m_aoLightAnimation;
		
		m_lsLightSource.ls_penEntity = this;
		m_lsLightSource.SetLightSource(lsNew);
	};
*/
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
	
	// play light animation
	void PlayLightAnim(INDEX iAnim, ULONG ulFlags) {
		if (m_aoLightAnimation.GetData()!=NULL) {
			m_aoLightAnimation.PlayAnim(iAnim, ulFlags);
		}
	};
	
	
	BOOL AdjustShadingParameters(FLOAT3D &vLightDirection, COLOR &colLight, COLOR &colAmbient) 
	{
		g_iPCMinBrightness = Clamp(g_iPCMinBrightness, (INDEX)0, (INDEX)255);

		BOOL bRecalcAmbient = FALSE;
		BOOL bRecalcDiffuse = FALSE;

		UBYTE ubAH, ubAS, ubAV;
		ColorToHSV(colAmbient, ubAH, ubAS, ubAV);
		UBYTE ubDH, ubDS, ubDV;
		ColorToHSV(colLight, ubDH, ubDS, ubDV);

		if(ubAV < g_iPCMinBrightness)
		{
			UBYTE ubAR, ubAG, ubAB;
			ColorToRGB(colAmbient, ubAR, ubAG, ubAB);
			ubAR = g_iPCMinBrightness;
			ubAG = g_iPCMinBrightness;
			ubAB = g_iPCMinBrightness;
			ColorToHSV(RGBToColor(ubAR, ubAG, ubAB), ubAH, ubAS, ubAV);
			bRecalcAmbient = TRUE;
		}
		if(ubAV < ubDV && g_iChaLightModifyValue < ubDV - ubAV)
		{
			INDEX value = (ubDV - ubAV - g_iChaLightModifyValue) >> 1;
			ubDV = Clamp(INDEX(ubDV) - value, INDEX(0), INDEX(255));
			ubAV = Clamp(INDEX(ubAV) + value, INDEX(0), INDEX(255));
			bRecalcAmbient = bRecalcDiffuse = TRUE;
		}

		if(bRecalcDiffuse) {colLight = HSVToColor(ubDH, ubDS, ubDV);}
		if(bRecalcAmbient) {colAmbient = HSVToColor(ubAH, ubAS, ubAV);}
		
		/*
		if( cht_bDumpPlayerShading)
		{
			ANGLE3D a3dHPB;
			DirectionVectorToAngles(-vLightDirection, a3dHPB);
			UBYTE ubAR, ubAG, ubAB;
			UBYTE ubCR, ubCG, ubCB;
			ColorToRGB(colAmbient, ubAR, ubAG, ubAB);
			ColorToRGB(colLight, ubCR, ubCG, ubCB);
			CPrintF("Ambient: %d,%d,%d, Color: %d,%d,%d, Direction HPB (%g,%g,%g)\n",
				ubAR, ubAG, ubAB, ubCR, ubCG, ubCB, a3dHPB(1), a3dHPB(2), a3dHPB(3));
		}
		
		// make models at least a bit bright in deathmatch
		if (!GetSP()->sp_bCooperative) {
			UBYTE ubH, ubS, ubV;
			ColorToHSV(colAmbient, ubH, ubS, ubV);
			ubV = ClampDn(ubV, (UBYTE)127);
			
			colAmbient = HSVToColor(ubH, ubS, ubV);            
		} else {
			UBYTE ubH, ubS, ubV;
			ColorToHSV(colAmbient, ubH, ubS, ubV);
			ubV = ClampDn(ubV, (UBYTE)30);
			colAmbient = HSVToColor(ubH, ubS, ubV);            
		}
		*/
		
		return CPlayerEntity::AdjustShadingParameters(vLightDirection, colLight, colAmbient);
	};
	
	// get a different model object for rendering
	CModelObject *GetModelForRendering(void)
	{
		// if not yet initialized
		if(!(m_ulFlags&PLF_INITIALIZED)) { 
			// return base model
			return GetModelObject();
		}
		
		// lerp player viewpoint
		CPlacement3D plView;
		plView.Lerp(en_plLastViewpoint, en_plViewpoint, _pTimer->GetLerpFactor());
		// body and head attachment animation
		//0105 1line 삭제
		//((CPlayerAnimator&)*m_penAnimator).BodyAndHeadOrientation(plView);
		((CPlayerAnimator&)*m_penAnimator).OnPreRender();
		// synchronize your appearance with the default model
		m_moRender.Synchronize(*GetModelObject());
		/*
		if (m_ulFlags&PLF_SYNCWEAPON) 
		{
			m_ulFlags &= ~PLF_SYNCWEAPON;
			GetPlayerAnimator()->SyncWeapon();
		}
		*/
		
		FLOAT tmNow = en_tmEntityTime;
		
		FLOAT fFading = 1.0f;
		if (m_tmFadeStart!=0) {
			FLOAT fFactor = (tmNow-m_tmFadeStart)/5.0f;
			fFactor = Clamp(fFactor, 0.0f, 1.0f);
			fFading*=fFactor;
		}
		
		// if invunerable after spawning
		FLOAT tmSpawnInvulnerability = GetSP()->sp_tmSpawnInvulnerability;
		if (tmSpawnInvulnerability>0 && tmNow-m_tmSpawned<tmSpawnInvulnerability) {
			// blink fast
			FLOAT fDelta = tmNow-m_tmSpawned;
			fFading *= 0.75f+0.25f*Sin(fDelta/0.5f*360);
		}
		
		COLOR colAlpha = m_moRender.mo_colBlendColor;
		colAlpha = (colAlpha&0xffffff00) + (COLOR(fFading*0xff)&0xff);
		m_moRender.mo_colBlendColor = colAlpha;
		
		// if not connected
		if (m_ulFlags&PLF_NOTCONNECTED) {
			// pulse slowly
			fFading *= 0.25f+0.25f*Sin(tmNow/2.0f*360);
			// if invisible
		} else if (m_tmInvisibility>tmNow) {
			FLOAT fIntensity=0.0f;
			if((m_tmInvisibility-tmNow)<3.0f)
			{
				fIntensity = 0.5f-0.5f*cos((m_tmInvisibility-tmNow)*(6.0f*3.1415927f/3.0f));
			}
			if (_ulPlayerRenderingMask == 1<<GetMyPlayerIndex()) {
				colAlpha = (colAlpha&0xffffff00)|(INDEX)(INVISIBILITY_ALPHA_LOCAL+(FLOAT)(254-INVISIBILITY_ALPHA_LOCAL)*fIntensity);
			} else if (TRUE) {
				if ((m_tmInvisibility-tmNow)<1.28f) {
					colAlpha = (colAlpha&0xffffff00)|(INDEX)(INVISIBILITY_ALPHA_REMOTE+(FLOAT)(254-INVISIBILITY_ALPHA_REMOTE)*fIntensity);
				} else if (TRUE) {
					colAlpha = (colAlpha&0xffffff00)|INVISIBILITY_ALPHA_REMOTE;
				}
			}
			m_moRender.mo_colBlendColor = colAlpha;
		}
		
		// use the appearance for rendering
		return &m_moRender;
	}
	
	//0105 모델 인스턴스 함수 추가. 이함수 안씀.
	CModelInstance *GetModelInstanceForRendering(void)
	{
		// if not yet initialized
		//초기화가 안됬었단 말인가..ㅡㅡ;
		//if(!(m_ulFlags&PLF_INITIALIZED)) { 
		//	return base model
		return GetModelInstance();
		//}
		
		// lerp player viewpoint
		CPlacement3D plView;
		
		plView.Lerp(en_plLastViewpoint, en_plViewpoint, _pTimer->GetLerpFactor());
		// body and head attachment animation
		//((CPlayerAnimator&)*m_penAnimator).BodyAndHeadOrientation(plView);
		((CPlayerAnimator&)*m_penAnimator).OnPreRender();
		//synchronize your appearance with the default model
		m_miRender.Synchronize(*GetModelInstance());
		
		FLOAT tmNow = en_tmEntityTime;
		
		FLOAT fFading = 1.0f;
		if (m_tmFadeStart!=0) {
			FLOAT fFactor = (tmNow-m_tmFadeStart)/5.0f;
			fFactor = Clamp(fFactor, 0.0f, 1.0f);
			fFading*=fFactor;
		}
		
		// if invunerable after spawning
		FLOAT tmSpawnInvulnerability = GetSP()->sp_tmSpawnInvulnerability;
		if (tmSpawnInvulnerability>0 && tmNow-m_tmSpawnedForInv<tmSpawnInvulnerability) {
			// blink fast
			FLOAT fDelta = tmNow-m_tmSpawnedForInv;
			fFading *= 0.75f+0.25f*Sin(fDelta/0.5f*360);
		}
		
		COLOR colAlpha = m_miRender.mi_colModelColor;
		colAlpha = (colAlpha&0xffffff00) + (COLOR(fFading*0xff)&0xff);
		m_miRender.mi_colModelColor = colAlpha;
		
		// if not connected
		if (m_ulFlags&PLF_NOTCONNECTED) {
			// pulse slowly
			fFading *= 0.25f+0.25f*Sin(tmNow/2.0f*360);
			// if invisible
		} else if (m_tmInvisibility>tmNow) {
			FLOAT fIntensity=0.0f;
			if((m_tmInvisibility-tmNow)<3.0f)
			{
				fIntensity = 0.5f-0.5f*cos((m_tmInvisibility-tmNow)*(6.0f*3.1415927f/3.0f));
			}
			if (_ulPlayerRenderingMask == 1<<GetMyPlayerIndex()) {
				colAlpha = (colAlpha&0xffffff00)|(INDEX)(INVISIBILITY_ALPHA_LOCAL+(FLOAT)(254-INVISIBILITY_ALPHA_LOCAL)*fIntensity);
			} else if (TRUE) {
				if ((m_tmInvisibility-tmNow)<1.28f) {
					colAlpha = (colAlpha&0xffffff00)|(INDEX)(INVISIBILITY_ALPHA_REMOTE+(FLOAT)(254-INVISIBILITY_ALPHA_REMOTE)*fIntensity);
				} else if (TRUE) {
					colAlpha = (colAlpha&0xffffff00)|INVISIBILITY_ALPHA_REMOTE;
				}
			}
			m_miRender.mi_colModelColor = colAlpha;
		}
		
		// use the appearance for rendering
		return &m_miRender;
	}
	//..
	
	// wrapper for action marker getting
	class CPlayerActionMarker *GetActionMarker(void) {
		return (CPlayerActionMarker *)&*m_penActionMarker;
	}
	
	// find main music holder if not remembered
	void FindMusicHolder(void)
	{
		if (m_penMainMusicHolder==NULL) {
			m_penMainMusicHolder = _pNetwork->GetEntityWithName("MusicHolder", 0);
		}
	}
	
	// update per-level stats
	void UpdateLevelStats(void)
	{
		// clear stats for this level
		m_psLevelStats = PlayerStats();
		
		// get music holder
		if (m_penMainMusicHolder==NULL) {
			return;
		}
		CMusicHolder &mh = (CMusicHolder&)*m_penMainMusicHolder;
		
		// assure proper count enemies in current world
		if (mh.m_ctEnemiesInWorld==0) {
			mh.CountEnemies();
		}
		// set totals for level and increment for game
		m_psLevelTotal.ps_iKills = mh.m_ctEnemiesInWorld;
		m_psGameTotal.ps_iKills += mh.m_ctEnemiesInWorld;
//		m_psLevelTotal.ps_iSecrets = mh.m_ctSecretsInWorld;
//		m_psGameTotal.ps_iSecrets += mh.m_ctSecretsInWorld;
	}
	
	// check if there is fuss
	BOOL IsFuss(void)
	{
		// if no music holder
		if (m_penMainMusicHolder==NULL) {
			// no fuss
			return FALSE;
		}
		// if no enemies - no fuss
		return ((CMusicHolder*)&*m_penMainMusicHolder)->m_cenFussMakers.Count()>0;
	}
	
	void SetDefaultMouthPitch(void)
	{
		m_soMouth.Set3DParameters(50.0f, 10.0f, 1.0f, 1.0f);
	}
	void SetRandomMouthPitch(FLOAT fMin, FLOAT fMax)
	{
		m_soMouth.Set3DParameters(50.0f, 10.0f, 1.0f, Lerp(fMin, fMax, FRnd()));
	}	
	
	// added: also shake view because of chainsaw firing
	void ApplyShaking(CPlacement3D &plViewer)
	{
		// chainsaw shaking
		FLOAT fT = _pTimer->GetLerpedCurrentTick();
		if (fT<m_tmChainShakeEnd)
		{
			m_fChainsawShakeDX = 0.03f*m_fChainShakeStrength*SinFast(fT*m_fChainShakeFreqMod*3300.0f);
			m_fChainsawShakeDY = 0.03f*m_fChainShakeStrength*SinFast(fT*m_fChainShakeFreqMod*2900.0f);
			
			plViewer.pl_PositionVector(1) += m_fChainsawShakeDX;
			plViewer.pl_PositionVector(3) += m_fChainsawShakeDY;
		}
		
		CWorldSettingsController *pwsc = GetWSC(this);
		if (pwsc==NULL || pwsc->m_tmShakeStarted<0) {
			return;
		}
		
		TIME tm = _pTimer->GetLerpedCurrentTick()-pwsc->m_tmShakeStarted;
		if (tm<0) {
			return;
		}
		FLOAT fDistance = (plViewer.pl_PositionVector-pwsc->m_vShakePos).Length();
		FLOAT fIntensity = IntensityAtDistance(pwsc->m_fShakeFalloff, 0, fDistance);
		FLOAT fShakeY, fShakeB, fShakeZ;
		if (!pwsc->m_bShakeFadeIn) {
			fShakeY = SinFast(tm*pwsc->m_tmShakeFrequencyY*360.0f)*
				exp(-tm*(pwsc->m_fShakeFade))*
				fIntensity*pwsc->m_fShakeIntensityY;
			fShakeB = SinFast(tm*pwsc->m_tmShakeFrequencyB*360.0f)*
				exp(-tm*(pwsc->m_fShakeFade))*
				fIntensity*pwsc->m_fShakeIntensityB;
			fShakeZ = SinFast(tm*pwsc->m_tmShakeFrequencyZ*360.0f)*
				exp(-tm*(pwsc->m_fShakeFade))*
				fIntensity*pwsc->m_fShakeIntensityZ;
		} else {
			FLOAT ootm = 1.0f/tm;
			fShakeY = SinFast(tm*pwsc->m_tmShakeFrequencyY*360.0f)*
				exp((tm-2)*ootm*(pwsc->m_fShakeFade))*
				fIntensity*pwsc->m_fShakeIntensityY;
			fShakeB = SinFast(tm*pwsc->m_tmShakeFrequencyB*360.0f)*
				exp((tm-2)*ootm*(pwsc->m_fShakeFade))*
				fIntensity*pwsc->m_fShakeIntensityB;
			fShakeZ = SinFast(tm*pwsc->m_tmShakeFrequencyZ*360.0f)*
				exp((tm-2)*ootm*(pwsc->m_fShakeFade))*
				fIntensity*pwsc->m_fShakeIntensityZ;
		}
		plViewer.pl_PositionVector(2) += fShakeY;
		plViewer.pl_PositionVector(3) += fShakeZ;
		plViewer.pl_OrientationAngle(3) += fShakeB;
		
	}
	
	COLOR GetWorldGlaring(void)
	{
		CWorldSettingsController *pwsc = GetWSC(this);
		if (pwsc==NULL || pwsc->m_tmGlaringStarted<0) {
			return 0;
		}
		TIME tm = _pTimer->GetLerpedCurrentTick();
		FLOAT fRatio = CalculateRatio(tm, pwsc->m_tmGlaringStarted, pwsc->m_tmGlaringEnded,
			pwsc->m_fGlaringFadeInRatio,  pwsc->m_fGlaringFadeOutRatio);
		COLOR colResult = (pwsc->m_colGlade&0xFFFFFF00)|(UBYTE(fRatio*255.0f));
		return colResult;
	}	
	
	void RenderHudPicFX(CDrawPort *pdp)
	{
		CWorldSettingsController *pwsc = GetWSC(this);
		if( pwsc!=NULL && pwsc->m_penHudPicFXHolder!=NULL)
		{
			CHudPicHolder &hpfx = (CHudPicHolder &) *pwsc->m_penHudPicFXHolder;
			hpfx.HudPic_Render(&hpfx, pdp);
		}
	}
	/************************************************************
	*                    RENDER GAME VIEW                      *
	************************************************************/
	
	// setup viewing parameters for viewing from player or camera
	void SetupView( CDrawPort *pdp, CAnyProjection3D &apr, CEntity *&penViewer, 
		CPlacement3D &plViewer, COLOR &colBlend, BOOL bCamera)
	{
		// read the exact placement of the view for this tick
		GetLerpedAbsoluteViewPlacement(plViewer);
//0507 kwon 삭제.에러나서.
//		ASSERT( IsValidFloat( plViewer.pl_OrientationAngle(1)) && IsValidFloat( plViewer.pl_OrientationAngle(2)) && IsValidFloat( plViewer.pl_OrientationAngle(3)));
		// get current entity that the player views from

		INDEX iViewState		= m_iViewState;
		CFixedCameraField* pField = FindPlayerInCameraField(this);
        if(pField && pField->m_penMarker) 
		{
			iViewState			= PVT_SCENECAMERA;
			plViewer			= (pField->m_penMarker)->GetPlacement();
			penViewer			= pField->m_penMarker;
			m_penCamera			= penViewer;
			_bInFixedCamera		= TRUE;
        }
		else
		{			
			penViewer = GetViewEntity(FALSE);
		
			// NOTE : 고정 카메라에서 3인칭 뷰로 바뀌는 경우, 
			// NOTE : m_penCamera가 고정 카메라 시점에서 이미 지정되어있어서 문제가 발생한다.
			// NOTE : 고로, 고정 카메라에서 본래의 뷰로 바뀌는 경우에는 m_penCamera를 NULL로 초기화한다.
			if(_bInFixedCamera)
			{
				_bInFixedCamera	= FALSE;
				m_penCamera		= NULL;
			}
		
			if( m_penCamera!=NULL && bCamera) 
			{
				iViewState = PVT_SCENECAMERA;
				plViewer = m_penCamera->GetLerpedPlacement();
				penViewer = m_penCamera;
			}
		}
		
		// init projection parameters
		CPerspectiveProjection3D prPerspectiveProjection;
		const FLOAT fMinFOV =   1.0f;
		const FLOAT fStdFOV =  90.0f;
		const FLOAT fMaxFOV = _bSeparatedTripleHead ? 120.0f : 160.0f;
		gfx_fTripleHeadFOVBias = Clamp( gfx_fTripleHeadFOVBias, 1.0f, 1.75f);
		FLOAT fFOVBias = 1.0f;
		if( _bTripleHead) 
		{ // apply triplehead FOV biasing
			if( _bSeparatedTripleHead) 
			{ // narrow FOV for 3-view setup
				fFOVBias = 1.0f / gfx_fTripleHeadFOVBias;
			} 
			else 
			{ // widen FOV for 1-view setup
				fFOVBias = gfx_fTripleHeadFOVBias;
			}
		} // clamp and apply
		plr_fFOV = Clamp( plr_fFOV, fMinFOV, fMaxFOV);
		ANGLE aFOV = plr_fFOV;
		
		// disable zoom in deathmatch (better not!)
		// if( !GetSP()->sp_bCooperative) { aFOV = 90.0f; }
		
//강동민 수정 시작 싱글 던젼 작업	08.05
		/*
		// if sniper active
		if( ((CPlayerWeapons&)*m_penWeapons).m_iCurrentWeapon==WEAPON_SNIPER) 
		{
			aFOV = Lerp( ((CPlayerWeapons&)*m_penWeapons).m_fSniperFOVlast,
				((CPlayerWeapons&)*m_penWeapons).m_fSniperFOV,
				_pTimer->GetLerpFactor());
		}
		
		if( m_pstState==PST_DIVE && iViewState==PVT_PLAYEREYES) 
		{
			TIME tmNow = _pTimer->GetLerpedCurrentTick();
			aFOV += sin(tmNow*0.79f)*2.0f;
		}
		*/
//강동민 수정 끝 싱글 던젼 작업		08.05
		//ApplyShaking(plViewer);
		CameraEffect(GetPlacement(), plViewer);
		aFOV *= fFOVBias; // triple-head adjustment
		
		// determine front clip plane
		const FLOAT fMinFCD = 0.06f;
		const FLOAT fMaxFCD = 0.25f;
		plr_fFrontClipDistance = Clamp( plr_fFrontClipDistance, 0.05f, 0.50f);
		if (!GetSP()->sp_bSinglePlayer && !GetSP()->sp_bCooperative) 
		{
			plr_fFrontClipDistance = 0.25;
		}
		FLOAT fFCD = plr_fFrontClipDistance;
		// adjust front clip plane for different FOV and swimming
		const FLOAT fAllowFCD = fMaxFCD - (aFOV-fStdFOV)/(fMaxFOV-fStdFOV) * (fMaxFCD-fMinFCD);
		fFCD = Min( fFCD, fAllowFCD);
		if( m_pstState==PST_SWIM && iViewState==PVT_PLAYEREYES) { fFCD *= 0.666f; }

		plr_fFarClipDistance = Clamp( plr_fFarClipDistance, plr_fFrontClipDistance+1.0f, 32768.0f);
		FLOAT fFarCD = plr_fFarClipDistance;
		
		colBlend = 0;
		if (iViewState == PVT_SCENECAMERA) 
		{
			CCamera *pcm = (CCamera*)&*m_penCamera;
			prPerspectiveProjection.FOVL() = 
				Lerp(pcm->m_fLastFOV, pcm->m_fFOV, _pTimer->GetLerpFactor());
			if (pcm->m_tmDelta>0.001f) 
			{
				FLOAT fFactor = (_pTimer->GetLerpedCurrentTick()-pcm->m_tmAtMarker)/pcm->m_tmDelta;
				fFactor = Clamp( fFactor, 0.0f, 1.0f);
				colBlend = LerpColor( pcm->m_colFade0, pcm->m_colFade1, fFactor);
			} 
			else 
			{
				colBlend = pcm->m_colFade0;
			}
		} 
		else 
		{
			prPerspectiveProjection.FOVL() = aFOV;
		}
		prPerspectiveProjection.ScreenBBoxL() = FLOATaabbox2D( FLOAT2D(0.0f, 0.0f),
													FLOAT2D((FLOAT)pdp->GetWidth(), (FLOAT)pdp->GetHeight()));
		prPerspectiveProjection.FrontClipDistanceL() = fFCD;
		prPerspectiveProjection.FarClipDistanceL() = fFarCD;
		prPerspectiveProjection.AspectRatioL() = 1.0f;
		// set up viewer position
		apr = prPerspectiveProjection;
		apr->ViewerPlacementL() = plViewer;
		apr->ObjectPlacementL() = CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0));
		apr->Prepare();
		prPlayerProjection = apr;
//		prPlayerProjection->Prepare();
	}
	
	// listen from a given viewer
	void ListenFromEntity(CEntity *penListener, const CPlacement3D &plSound)
	{
		FLOATmatrix3D mRotation;
		MakeRotationMatrixFast(mRotation, plSound.pl_OrientationAngle);
		sliSound.sli_penEntity = penListener;
		sliSound.sli_vPosition = plSound.pl_PositionVector;
		sliSound.sli_mRotation = mRotation;
		sliSound.sli_vSpeed    = en_vCurrentTranslationAbsolute;
		sliSound.sli_fVolume   = 1.0f;
		
		// if player is in water
		if( m_pstState==PST_DIVE) {
			// force environment type to underwater
			sliSound.sli_eEnvType = SEFX_UNDERWATER;
			sliSound.sli_fFilter  = 10.0f; // for engine mixer
		} else {
			// not in water - readout environment type and size from sector
			INDEX iEnv = 0;
			CBrushSector *pbsc = penListener->GetSectorFromPoint(plSound.pl_PositionVector);  // get sector around listener
			if( pbsc!=NULL) { iEnv = pbsc->GetEnvironmentType(); }
			CEnvironmentType &et  = GetWorld()->wo_aetEnvironmentTypes[iEnv];
			sliSound.sli_eEnvType = (SndEnvFX)et.et_iType;
			sliSound.sli_fFilter  = 0.0f;
		}
		
		// add this listener to list
		_pSound->Listen(sliSound);
	}
	
	// render dummy view (not connected yet)
	void RenderDummyView(CDrawPort *pdp)
	{
		// clear screen
		pdp->Fill( C_BLACK|CT_OPAQUE);
		
		// if not single player
		if (!GetSP()->sp_bSinglePlayer) {
			// print a message
			PIX pixDPWidth  = pdp->GetWidth();
			PIX pixDPHeight = pdp->GetHeight();
			FLOAT fScale = (FLOAT)pixDPHeight/480.0f;
			pdp->SetFont( _pfdDisplayFont);
			pdp->SetTextScaling( fScale/2);
			pdp->SetTextAspect( 1.0f);
			pdp->SetTextShadow(+2);
			CTString strMsg;
			strMsg.PrintF(TRANS("%s connected"), GetPlayerName());
			pdp->PutTextCXY( strMsg, pixDPWidth*0.5f, pixDPHeight*0.5f, SE_COL_BLUE_NEUTRAL_LT|CT_OPAQUE);
		}
	}
	
	//-----------------------------------------------------------------------------
	// Purpose: render view from player
	// Input  : *pdp - 
	//			bShowExtras - 
	// Output : 	void
	//-----------------------------------------------------------------------------
	void RenderPlayerView(CDrawPort *pdp, BOOL bShowExtras)
	{
		CAnyProjection3D apr;
		CEntity *penViewer;
		CPlacement3D plViewer;
		COLOR colBlend;
		
		// for each eye
		for( INDEX iEye=STEREO_LEFT; iEye<=(Stereo_IsEnabled()?STEREO_RIGHT:STEREO_LEFT); iEye++)
		{
			// setup stereo rendering
			Stereo_SetBuffer(iEye);
			_ulPlayerRenderingMask = 1 << GetMyPlayerIndex();
			_bTripleHead = pdp->IsTripleHead();
			_bSeparatedTripleHead = pdp->IsTripleHead() && _pShell->GetINDEX("gfx_bSeparateTripleHead")!=0;
			
			// 3-view triplehead?
			if( _bSeparatedTripleHead) 
			{
				// left
				CDrawPort dpL( pdp,-1);
				SetupView( &dpL, apr, penViewer, plViewer, colBlend, FALSE);
				const FLOAT fFOV = ((CPerspectiveProjection3D&)*apr).FOVR();
				apr->ViewerPlacementL().Rotate_Airplane( ANGLE3D(+fFOV,0,0));
				Stereo_AdjustProjection( *apr, iEye, 1);
				dpL.SetAsCurrent();
				RenderView( *en_pwoWorld, *penViewer, apr, dpL);
				// right
				CDrawPort dpR( pdp,+1);
				SetupView( &dpR, apr, penViewer, plViewer, colBlend, FALSE);
				apr->ViewerPlacementL().Rotate_Airplane( ANGLE3D(-fFOV,0,0));
				Stereo_AdjustProjection( *apr, iEye, 1);
				dpR.SetAsCurrent();
				RenderView( *en_pwoWorld, *penViewer, apr, dpR);
				// center
				CDrawPort dpC( pdp, 0);
				SetupView( &dpC, apr, penViewer, plViewer, colBlend, FALSE);
				Stereo_AdjustProjection( *apr, iEye, 1);
				dpC.SetAsCurrent();
				RenderView( *en_pwoWorld, *penViewer, apr, dpC);	
			}
			// not 3-view triplehead!
			else 
			{
				// setup view settings
				SetupView( pdp, apr, penViewer, plViewer, colBlend, FALSE);
				Stereo_AdjustProjection( *apr, iEye, 1);
				RenderView( *en_pwoWorld, *penViewer, apr, *pdp);				
			}
			
			_ulPlayerRenderingMask = 0;
			if( iEye==STEREO_LEFT) {
				// listen from here
				ListenFromEntity(this, plViewer);
			}
			
			// back to center view
			CDrawPort dpCenter( pdp, 0);
			dpCenter.SetAsCurrent();
			SetupView( &dpCenter, apr, penViewer, plViewer, colBlend, FALSE);
			Stereo_AdjustProjection( *apr, iEye, 1);
			
			//RenderScroll(  &dpCenter);
			//RenderTextFX(  &dpCenter);
			//RenderCredits( &dpCenter);
			RenderHudPicFX(&dpCenter);
			
			if( hud_bShowAll && bShowExtras) 
			{
				// let the player entity render its interface
				CPlacement3D plLight(_vViewerLightDirection, ANGLE3D(0,0,0));
				plLight.AbsoluteToRelative(plViewer);
				RenderHUD( *(CPerspectiveProjection3D*)(CProjection3D*)apr, &dpCenter, 
					plLight.pl_PositionVector, _colViewerLight, _colViewerAmbient, 
					penViewer==this && (GetFlags()&ENF_ALIVE), iEye);
			}
			// pull out
			pdp->SetAsCurrent();
			
			//0105 apr값을 얻기.
			m_apr =apr;
			//..
		}
		
		// done with stereo
		Stereo_SetBuffer(STEREO_BOTH);
		
		// determine and cache main drawport, size and relative scale
		PIX pixDPWidth  = pdp->GetWidth();
		PIX pixDPHeight = pdp->GetHeight();
		FLOAT fScale = (FLOAT)pixDPHeight/480.0f;
		
		/*
		// print center message
		if (_pTimer->CurrentTick()<m_tmCenterMessageEnd) {
			pdp->SetFont( _pfdDisplayFont);
			pdp->SetTextScaling( fScale/2);
			pdp->SetTextAspect( 1.0f);
			pdp->SetTextShadow(+2);
			pdp->PutTextCXY( m_strCenterMessage, pixDPWidth*0.5f, pixDPHeight*0.85f, C_WHITE|0xDD);
			// print picked item
		} else if (_pTimer->CurrentTick()<m_tmLastPicked+PICKEDREPORT_TIME) {
			pdp->SetFont( _pfdDisplayFont);
			pdp->SetTextScaling( fScale/2);
			pdp->SetTextAspect( 1.0f);
			pdp->SetTextShadow(+2);
			CTString strPicked;
			if (m_fPickedAmmount==0) {
				strPicked = m_strPickedName;
			} else {
				strPicked.PrintF("%s +%d", m_strPickedName, int(m_fPickedAmmount));
			}
			pdp->PutTextCXY( strPicked, pixDPWidth*0.5f, pixDPHeight*0.82f, C_WHITE|0xDD);
			if (!GetSP()->sp_bCooperative && !GetSP()->sp_bUseFrags && m_fPickedMana>=1) {
				CTString strValue;
				strValue.PrintF("%s +%d", TRANS("Value"), INDEX(m_fPickedMana));
				pdp->PutTextCXY( strValue, pixDPWidth*0.5f, pixDPHeight*0.85f, C_WHITE|0xDD);
			}
		}
		*/
		
		if (_pTimer->CurrentTick()<m_tmAnalyseEnd) {
			pdp->SetFont( _pfdDisplayFont);
			pdp->SetTextScaling( fScale/2);
			pdp->SetTextAspect( 1.0f);
			pdp->SetTextShadow(+2);
			UBYTE ubA = int(sin(_pTimer->CurrentTick()*10.0f)*127+128);
			pdp->PutTextCXY( TRANS("Analyzing..."), pixDPWidth*0.5f, pixDPHeight*0.2f, SE_COL_BLUE_NEUTRAL_LT|ubA);
		}
	}
	
	// render view from camera
	void RenderCameraView( CDrawPort *pdp, BOOL bListen)
	{
		CDrawPort dpCamera;
		CDrawPort *pdpCamera = pdp;
		if (m_penCamera!=NULL && ((CCamera&)*m_penCamera).m_bWideScreen) 
		{
			pdp->MakeWideScreen(&dpCamera);
			pdpCamera = &dpCamera;
		}
		
		pdpCamera->SetAsCurrent();
		
		CAnyProjection3D apr;
		CEntity *penViewer;

		CPlacement3D plViewer;
		COLOR colBlend;
		
		// for each eye
		for (INDEX iEye=STEREO_LEFT; iEye<=(Stereo_IsEnabled()?STEREO_RIGHT:STEREO_LEFT); iEye++) 
		{			
			// setup view settings
			SetupView( pdpCamera, apr, penViewer, plViewer, colBlend, TRUE);
			
			// setup stereo rendering
			Stereo_SetBuffer(iEye);
			Stereo_AdjustProjection(*apr, iEye, 1);
			
//강동민 수정 시작 클로즈 준비 작업	08.10
			extern BOOL _bWorldEditorApp;
			if(_bLoginProcess && !_bWorldEditorApp)
			{
				m_apr = apr;
			}
//강동민 수정 끝 클로즈 준비 작업		08.10
			
			// render the view
			ASSERT(IsValidFloat(plViewer.pl_OrientationAngle(1))&&IsValidFloat(plViewer.pl_OrientationAngle(2))&&IsValidFloat(plViewer.pl_OrientationAngle(3)));
			_ulPlayerRenderingMask = 1<<GetMyPlayerIndex();
			RenderView( *en_pwoWorld, *penViewer, apr, *pdpCamera);
			_ulPlayerRenderingMask = 0;
			
			// listen from there if needed
			if (bListen && iEye==STEREO_LEFT) {
				ListenFromEntity(penViewer, plViewer);
			}
		}
		Stereo_SetBuffer(STEREO_BOTH);
		
		CDrawPort dpCenter( pdp, 0);
		dpCenter.SetAsCurrent();
		
		//RenderScroll(  &dpCenter);
		//RenderTextFX(  &dpCenter);
		//RenderCredits( &dpCenter);
		RenderHudPicFX(&dpCenter);
		
		// add world glaring
		{
			pdpCamera->SetAsCurrent();
			COLOR colGlare = GetWorldGlaring();
			UBYTE ubR, ubG, ubB, ubA;
			ColorToRGBA(colGlare, ubR, ubG, ubB, ubA);
			if (ubA!=0) {
				pdpCamera->dp_ulBlendingRA += ULONG(ubR)*ULONG(ubA);
				pdpCamera->dp_ulBlendingGA += ULONG(ubG)*ULONG(ubA);
				pdpCamera->dp_ulBlendingBA += ULONG(ubB)*ULONG(ubA);
				pdpCamera->dp_ulBlendingA  += ULONG(ubA);
			}
			// do all queued screen blendings
			pdpCamera->BlendScreen();
		}
		
		pdp->SetAsCurrent();
		
		// camera fading
		if( (colBlend&CT_AMASK)!=0) {
			pdp->Fill(colBlend);
		}
	}
	
	
	//-----------------------------------------------------------------------------
	// Purpose: 
	// Input  : *pdp - 
	//			*pvUserData - 
	// Output : 	void
	//-----------------------------------------------------------------------------
	void RenderGameView( CDrawPort *pdp, void *pvUserData)
	{
		BOOL bShowExtras = (ULONG(pvUserData)&GRV_SHOWEXTRAS);
		
		// if not yet initialized
		if( !(m_ulFlags&PLF_INITIALIZED) || (m_ulFlags&PLF_DONTRENDER)) 
		{
			// render dummy view on the whole drawport
			pdp->SetAsCurrent();
			RenderDummyView(pdp);
			return; 
		}
		
		// if rendering real game view (not thumbnail, or similar)
		if( pvUserData!=0) 
		{
			// if rendered a game view recently
			CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
			if ((tvNow-_tvProbingLast).GetSeconds()<0.1) 
			{
				// allow probing
				_pGfx->gl_bAllowProbing = TRUE;
			} // mark
			_tvProbingLast = tvNow;
		}
		
		// check for dualhead
		const BOOL bAllowMultiHead = (GetSP()->sp_gmGameMode!=CSessionProperties::GM_FLYOVER) 
										&& (m_penActionMarker==NULL);
		const BOOL bDualHead   = pdp->IsDualHead()   && bAllowMultiHead;
		const BOOL bTripleHead = pdp->IsTripleHead() && bAllowMultiHead;
		const BOOL bMultiHead  = bDualHead || bTripleHead;
		
		// if dualhead, or no camera active
		if( bMultiHead || m_penCamera==NULL) 
		{
			if( bDualHead) 
			{
				// render left player view for dual-head
				CDrawPort dpView( pdp, 0);
				dpView.SetAsCurrent();
				RenderPlayerView( &dpView, bShowExtras);
			} 
			else 
			{
				RenderPlayerView( pdp, bShowExtras);
			}
		}
		
		// if camera active
		if( m_penCamera!=NULL) 
		{
			// make full or right camera view
			const INDEX iMonitor = (m_penActionMarker!=NULL) ? 3 : +1;
			CDrawPort dpView( pdp, iMonitor);
			dpView.SetAsCurrent();
			// draw it, listen if not multihead
			RenderCameraView( &dpView, !bDualHead);
			// if camera is not active
		} 
		else 
		{
			// if dualhead
			if( bDualHead) 
			{
				// render computer on secondary display
				cmp_ppenDHPlayer = this;
			}
		}
		// all done - lock back the original drawport
		pdp->SetAsCurrent();
	};
	
	
	
	
	/************************************************************
	*                   PRE/DO/POST MOVING                     *
	************************************************************/
	
	// premoving for soft player up-down movement
	void PreMoving(void) 
	{
		/*CPrintF("pos(%s): %g,%g,%g\n", GetPredictName(), 
		GetPlacement().pl_PositionVector(1),
		GetPlacement().pl_PositionVector(2),
		GetPlacement().pl_PositionVector(3));
		*/

/*		if(g_slZone==0 && GetPlacement().pl_PositionVector(2) < 150.0f)
		{ // 이게 머니 이게~~
			if (_pTimer->CurrentTick()>=m_tmNextAmbientOnce)
			{	// 물 속에서 기포 발생 처리 
				SpawnBubbles( 10+INDEX(FRnd()*5));
				m_tmNextAmbientOnce = _pTimer->CurrentTick()+5.0f+FRnd();
			}			
		}*/

		if (m_penAnimator != NULL) 
		{
			((CPlayerAnimator&)*m_penAnimator).StoreLast();
		}
		
		//safety - set correct view if needed
		if (GetFlags()&ENF_ALIVE && m_iViewState==PVT_PLAYERAUTOVIEW) 
		{
			m_iViewState = PVT_PLAYEREYES;      
		}
		
//0308
		en_vDesiredDirection = m_vDesiredPosition - GetPlacement().pl_PositionVector;
		//en_vDesiredDirection(2)=0.0f;
		if(en_vDesiredDirection.Length() != 0)
		{
			en_vDesiredDirection.Normalize();
		}

		CPlayerEntity::PreMoving();
	};
	
	// do moving
	void DoMoving(void) 
	{	
		CPlayerEntity::DoMoving();
		
		// do not animate banking if we are reapplying actions for local player prediction
		if (!IsReapplyingActions()) 
		{
			if (m_penAnimator != NULL) 
			{
				((CPlayerAnimator&)*m_penAnimator).AnimateBanking();
			}
			
			if (m_penView!=NULL) 
			{
				((CPlayerView&)*m_penView).DoMoving();
			}
			if (m_pen3rdPersonView!=NULL) 
			{
				((CPlayerView&)*m_pen3rdPersonView).DoMoving();
			} 
		}
	};
	
	// postmoving for soft player up-down movement
	void PostMoving(void)
	{
		CPlayerEntity::PostMoving();
		
		// never allow a player to be removed from the list of movers
		en_ulFlags &= ~ENF_INRENDERING;
		
		if (m_penAnimator != NULL) 
		{
			((CPlayerAnimator&)*m_penAnimator).AnimateSoftEyes();
		}
		
		//((CPlayerAnimator&)*m_penAnimator).AnimateRecoilPitch();
		
		// slowly increase mana with time, faster if player is not moving; (only if alive)
		if (GetFlags()&ENF_ALIVE)
		{
		/*
			m_fManaFraction += 
				ClampDn( 1.0f-en_vCurrentTranslationAbsolute.Length()/20.0f, 0.0f) * 20.0f
				* _pTimer->TickQuantum;
			INDEX iNewMana = m_fManaFraction;
			//m_iMana         += iNewMana;
			m_fManaFraction -= iNewMana;
			*/
		}
		
		// update ray hit for weapon target
		//GetPlayerWeapons()->UpdateTargetingInfo();
//0419 kwon 		
		//POINT pntMouse;
		//GetCursorPos(&pntMouse);	 	  
		//::ScreenToClient(_pInput->_Hwnd, &pntMouse);
		CPlacement3D plRay;	
		if(m_apr && !_pInput->IsRMouseButtonPressed() && !_pInput->inp_bFreeMode)//0807
		{
			plRay = GetPlayerWeapons()->GetMouseHitInformation( _pInput->inp_ptMousePos, m_apr, FALSE);					
			CEntity *penTarget = GetPlayerWeapons()->m_penRayHitTmp;
			CEntity *penCurrent = GetPlayerWeapons()->m_penRayHitNow;

			// FIXME : 왜 이렇게 해둔건지 잘 모르겠음.
			if( penCurrent != NULL && ( penCurrent->GetFlags() & ENF_ALIVE || penCurrent->GetFlags() & ENF_ITEM ) )
			{
				FLOAT MaxHealth;
				FLOAT CurrentHealth;
				int	  Level;
				int	  PkMode = -1;
				int   PkState, PkLegit;
				SQUAD	llItemSum = 0;
				int	nType = -1;
				CCharacterTarget* tCT =NULL;

				MaxHealth		= ((CUnit*)((CEntity*) penCurrent))->m_nMaxiHealth;
				CurrentHealth	= ((CUnit*)((CEntity*) penCurrent))->m_nCurrentHealth;
				Level			= ((CUnit*)((CEntity*) penCurrent))->m_nMobLevel;

				if(penCurrent->IsCharacter())
				{
					Level		= 0;
					PkMode		= ((CCharacter*)((CEntity*) penCurrent))->m_nPkMode;
					PkState		= ((CCharacter*)((CEntity*) penCurrent))->m_nPkState;
					PkLegit		= ((CCharacter*)((CEntity*) penCurrent))->m_nLegit;

					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, penCurrent->GetNetworkID());

					if (pObject != NULL)
					{
						CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);
						tCT = pTarget;
					}
				}
				else if(penCurrent->GetFlags()&ENF_ITEM)
					{
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_ITEM, penCurrent->GetNetworkID());

					if (pObject != NULL)
					{
						CItemTarget* pTarget = static_cast< CItemTarget* >(pObject);

						llItemSum = pTarget->item_llCount;
						nType = pTarget->m_nType;
					}
				}
				
				if(penCurrent->GetFlags()&ENF_ITEM)
				{
					penCurrent->SetTargetInfoReal(MaxHealth, CurrentHealth,
												Level, 
												0, 
												0, 
												llItemSum, -1, NULL, nType);
				}
				else if(PkMode != -1)
				{
					if( tCT != NULL )
					{
						penCurrent->SetTargetInfoReal(MaxHealth, CurrentHealth, 
												Level, 
												PkMode, 
												PkState, 
												0,			// llCount
												PkLegit,
												tCT
												);
					}
				}
				else
				{
					// FIXME : 마우스 오버만 해도 이 루틴이 호출되는데...-_-;;;
					// FIXME : 클릭했을때만 처리되야 되는거 아닌가???
					penCurrent->SetTargetInfoReal(MaxHealth, CurrentHealth,
												Level);
				}
			}
			else
			{
				//penCurrent->DelTargetInfoReal();
				INFO()->TargetClear(eTARGET_REAL);
			}
		}
////////////////
		if (!IsReapplyingActions()) 
		{
			if (m_pen3rdPersonView!=NULL) 
			{
				((CPlayerView&)*m_pen3rdPersonView).PostMoving();
			}
			if (m_penView!=NULL) 
			{
				((CPlayerView&)*m_penView).PostMoving();
			}
		}
		
		// clear action indicator
		m_ulFlags&=~PLF_APPLIEDACTION;
		
		// 플레이어가 무빙 브러쉬 위에 있다면...
		// if the player is standing on a moving brush
		if (en_penReference!=NULL && en_penReference->GetRenderType()==CEntity::RT_BRUSH && (en_penReference->GetPhysicsFlags()&EPF_MOVABLE)) 
		{
			// make it the player's parent
			SetParent(en_penReference);
			
			CMovableEntity *penRef = ((CMovableEntity*)(CEntity*)en_penReference);
			
			// if this is the first tick the player is on the brush
			if (penRef->m_iPlayerRefCounter == 0) 
			{
				// stop the brush (this effectively stops dead reckoning on the client)
				penRef->ForceFullStop();
				CEntityMessage emEntityMessage;
				// if this is the server, force sending a placement update for the brush and all it's children to clients
				if (_pNetwork->IsServer()) 
				{
					emEntityMessage.WritePlacementNotify(penRef->en_ulID,TRUE);
					_pNetwork->ga_srvServer.SendMessage(emEntityMessage);
					
					{FOREACHINLIST(CEntity, en_lnInParent, penRef->en_lhChildren, itenChild) 
					{
						if (itenChild->en_ulPhysicsFlags&EPF_MOVABLE) 
						{
							CMovableEntity *penChild = ((CMovableEntity*)(CEntity*)itenChild);
							
							penChild->ForceFullStop();
							emEntityMessage.WritePlacementNotify(penChild->en_ulID,TRUE);
							_pNetwork->ga_srvServer.SendMessage(emEntityMessage);
						}
					}}
				}
			}
			// it will not dead reckon for another five ticks (prevents jittering)
			penRef->m_iPlayerRefCounter = 5;
			
			m_iPlayerRefCounter = 5;
		} 
		else 
		{
			// if not standing on a moving brush, unparent the player
			SetParent(NULL);
			m_iPlayerRefCounter--;
		}
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
/*
		CTag *pTag = m_tmTagManager.Find("ET POS");
		CEntityTag *pEntityTag = NULL;
		if(pTag != NULL && pTag->GetType() == TT_ENTITY)
		{
			pEntityTag = (CEntityTag*)pTag;
			TagInfo info;
			info.m_fTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
			pEntityTag->AddNewTagInfo(info);
		}
*/
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
	}
	
	// set player parameters for unconnected state (between the server loads and player reconnects)
	void SetUnconnected(void)
	{
		if (m_ulFlags&PLF_NOTCONNECTED) 
		{
			return;
		}
		m_ulFlags |= PLF_NOTCONNECTED;
		
		// reset to a dummy state
		ForceFullStop();
		SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));
		SetCollisionFlags(GetCollisionFlags() & ~((ECBI_BRUSH|ECBI_MODEL)<<ECB_TEST));
		en_plLastViewpoint.pl_OrientationAngle = en_plViewpoint.pl_OrientationAngle = ANGLE3D(0,0,0);
		
		StartModelAnim(PLAYER_ANIM_STAND, 0);
		/*
		GetPlayerAnimator()->BodyAnimationTemplate(
			BODY_ANIM_NORMALWALK, BODY_ANIM_COLT_STAND, BODY_ANIM_SHOTGUN_STAND, BODY_ANIM_MINIGUN_STAND, 
			AOF_LOOPING|AOF_NORESTART);
		*/
	}
	
	// set player parameters for connected state
	void SetConnected(void)
	{
		if (!(m_ulFlags&PLF_NOTCONNECTED)) 
		{
			return;
		}
		m_ulFlags &= ~PLF_NOTCONNECTED;
		
		SetPhysicsFlags(GetPhysicsFlags() | (EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));
		SetCollisionFlags(GetCollisionFlags() | ((ECBI_BRUSH|ECBI_MODEL)<<ECB_TEST));
	}
	
	// check if player is connected or not
	BOOL IsConnected(void) const
	{
		return !(m_ulFlags&PLF_NOTCONNECTED);
	}
	
	/************************************************************
	*         DAMAGE OVERRIDE (PLAYER HAS ARMOR)               *
	************************************************************/
	
	
	// leave stain
	virtual void LeaveStain( BOOL bGrow)
	{
		ESpawnEffect ese;
		FLOAT3D vPoint;
		FLOATplane3D vPlaneNormal;
		FLOAT fDistanceToEdge;
		// get your size
		FLOATaabbox3D box;
		GetBoundingBox(box);
		
		// on plane
		if( GetNearestPolygon(vPoint, vPlaneNormal, fDistanceToEdge)) {
			// if near to polygon and away from last stain point
			if( (vPoint-GetPlacement().pl_PositionVector).Length()<0.5f
				&& (m_vLastStain-vPoint).Length()>1.0f ) {
				m_vLastStain = vPoint;
				FLOAT fStretch = box.Size().Length();
				ese.colMuliplier = C_WHITE|CT_OPAQUE;
				// stain
				if (bGrow) {
					ese.betType    = BET_BLOODSTAINGROW;
					ese.vStretch   = FLOAT3D( fStretch*1.5f, fStretch*1.5f, 1.0f);
				} else {
					ese.betType    = BET_BLOODSTAIN;
					ese.vStretch   = FLOAT3D( fStretch*0.75f, fStretch*0.75f, 1.0f);
				}
				ese.vNormal    = FLOAT3D( vPlaneNormal);
				ese.vDirection = FLOAT3D( 0, 0, 0);
				FLOAT3D vPos = vPoint+ese.vNormal/50.0f*(FRnd()+0.5f);
				CEntityPointer penEffect = CreateEntity( CPlacement3D(vPos, ANGLE3D(0,0,0)), CLASS_BASIC_EFFECT);
				penEffect->Initialize(ese);
			}
		}
	};

	void DamageImpact(enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
	{
		// if exploded
		if (GetRenderType()!=RT_MODEL) {
			// do nothing
			return;
		}
		
		if (dmtType == DMT_ABYSS || dmtType == DMT_SPIKESTAB) {
			return;
		}
		
		fDamageAmmount = Clamp(fDamageAmmount, 0.0f, 5000.0f);
		
		FLOAT fKickDamage = fDamageAmmount;
		if( (dmtType == DMT_EXPLOSION) || (dmtType == DMT_IMPACT)/* || (dmtType == DMT_CANNONBALL_EXPLOSION) */)
		{
			fKickDamage*=1.5;
		}
		if (dmtType==DMT_DROWNING || dmtType==DMT_CLOSERANGE) {
			fKickDamage /= 10;
		}
		if (dmtType==DMT_CHAINSAW)
		{
			fKickDamage /= 10;
		}
		
		// get passed time since last damage
		TIME tmNow = _pTimer->CurrentTick();
		TIME tmDelta = tmNow-m_tmLastDamage;
		m_tmLastDamage = tmNow;
		
		// fade damage out
		if (tmDelta>=_pTimer->TickQuantum*3) {
			m_vDamage=FLOAT3D(0,0,0);
		}
		// add new damage
		FLOAT3D vDirectionFixed;
		if (vDirection.ManhattanNorm()>0.5f) {
			vDirectionFixed = vDirection;
		} else {
			vDirectionFixed = -en_vGravityDir;
		}
		FLOAT3D vDamageOld = m_vDamage;
		m_vDamage+=(vDirectionFixed/*-en_vGravityDir/2*/)*fKickDamage;
		
		FLOAT fOldLen = vDamageOld.Length();
		FLOAT fNewLen = m_vDamage.Length();
		FLOAT fOldRootLen = Sqrt(fOldLen);
		FLOAT fNewRootLen = Sqrt(fNewLen);
		
		FLOAT fMassFactor = 200.0f/((EntityInfo*)GetEntityInfo())->fMass;
		
		if( !(en_ulFlags & ENF_ALIVE))
		{
			fMassFactor /= 3;
		}
		
		switch( dmtType)
		{
		case DMT_CLOSERANGE:
		case DMT_CHAINSAW:
		case DMT_DROWNING:
		case DMT_IMPACT:
		case DMT_BRUSH:
		case DMT_BURNING:
			// do nothing
			break;
		default:
			{
				if(fOldLen != 0.0f)
				{
					// cancel last push
					GiveImpulseTranslationAbsolute( -vDamageOld/fOldRootLen*fMassFactor);
				}
				
				/*
				FLOAT3D vImpuls = m_vDamage/fNewRootLen*fMassFactor;
				CPrintF( "Applied absolute translation impuls: (%g%g%g)\n",
				vImpuls(1),vImpuls(2),vImpuls(3));
				*/
				
				// push it back
				GiveImpulseTranslationAbsolute( m_vDamage/fNewRootLen*fMassFactor);
			}
		}
		
		if( m_fMaxDamageAmmount<fDamageAmmount)
		{
			m_fMaxDamageAmmount = fDamageAmmount;
		}
		// if it has no spray, or if this damage overflows it
		if ((m_tmSpraySpawned<=_pTimer->CurrentTick()-_pTimer->TickQuantum*8 || 
			m_fSprayDamage+fDamageAmmount>50.0f))
		{
			// spawn blood spray
			CPlacement3D plSpray = CPlacement3D( vHitPoint, ANGLE3D(0, 0, 0));
			m_penSpray = CreateEntity( plSpray, CLASS_BLOOD_SPRAY);
			m_penSpray->SetParent( this);
			ESpawnSpray eSpawnSpray;
			eSpawnSpray.colBurnColor=C_WHITE|CT_OPAQUE;
			
			if( m_fMaxDamageAmmount > 10.0f)
			{
				eSpawnSpray.fDamagePower = 3.0f;
			}
			else if(m_fSprayDamage+fDamageAmmount>50.0f)
			{
				eSpawnSpray.fDamagePower = 2.0f;
			}
			else
			{
				eSpawnSpray.fDamagePower = 1.0f;
			}
			
			eSpawnSpray.sptType = SPT_BLOOD;
			eSpawnSpray.fSizeMultiplier = 1.0f;
			
			// setup direction of spray
			FLOAT3D vHitPointRelative = vHitPoint - GetPlacement().pl_PositionVector;
			FLOAT3D vReflectingNormal;
			GetNormalComponent( vHitPointRelative, en_vGravityDir, vReflectingNormal);
			vReflectingNormal.Normalize();
			
			vReflectingNormal(1)/=5.0f;
			
			FLOAT3D vProjectedComponent = vReflectingNormal*(vDirection%vReflectingNormal);
			FLOAT3D vSpilDirection = vDirection-vProjectedComponent*2.0f-en_vGravityDir*0.5f;
			
			eSpawnSpray.vDirection = vSpilDirection;
			eSpawnSpray.eidOwner = this;
			
			// initialize spray
			m_penSpray->Initialize( eSpawnSpray);
			m_tmSpraySpawned = _pTimer->CurrentTick();
			m_fSprayDamage = 0.0f;
			m_fMaxDamageAmmount = 0.0f;
		}
		m_fSprayDamage+=fDamageAmmount;
	}
	
	/* Receive damage */
	void ReceiveDamage( CEntity *penInflictor, enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
	{
		if(dmtType == DMT_NONE) {return;}

		// don't harm yourself with knife or with rocket in easy/tourist mode
		if( penInflictor==this && (dmtType==DMT_CLOSERANGE || dmtType==DMT_CHAINSAW ||
			((dmtType==DMT_EXPLOSION/*||dmtType==DMT_CANNONBALL_EXPLOSION*/||dmtType==DMT_PROJECTILE) &&
			GetSP()->sp_gdGameDifficulty<=CSessionProperties::GD_EASY)) ) {
			return;
		}
		
		// if not connected
		if (m_ulFlags&PLF_NOTCONNECTED) {
			// noone can harm you
			return;
		}
		
		// god mode -> no one can harm you
		if( cht_bGod && CheatsEnabled() ) { return; }
		
		// if invulnerable, nothing can harm you except telefrag or abyss
		const TIME tmDelta = m_tmInvulnerability - en_tmEntityTime;
		if( tmDelta>0 && dmtType!=DMT_ABYSS && dmtType!=DMT_TELEPORT) { return; }
		
		// if invunerable after spawning
		FLOAT tmSpawnInvulnerability = GetSP()->sp_tmSpawnInvulnerability;
		if (tmSpawnInvulnerability>0 && en_tmEntityTime-m_tmSpawned<tmSpawnInvulnerability) {
			// ignore damage
			return;
		}
		
		/*
		// check for friendly fire
		if (!GetSP()->sp_bFriendlyFire && GetSP()->sp_bCooperative) {
			if (IsOfClass(penInflictor, "Player") && penInflictor!=this) {
				return;
			}
		}
		*/
		
		// ignore heat damage if dead
		if (dmtType==DMT_HEAT && !(GetFlags()&ENF_ALIVE)) {
			return;
		}
		
		// adjust for difficulty
		FLOAT fDifficultyDamage = GetSP()->sp_fDamageStrength;
		if( fDifficultyDamage<=1.0f || penInflictor!=this) {
			fDamageAmmount *= fDifficultyDamage;
		}
		
		// ignore zero damages
		if (fDamageAmmount<=0) {
			return;
		}
		
		FLOAT fSubHealth, fSubArmor;
		if( dmtType == DMT_DROWNING) {
			// drowning
			fSubHealth = fDamageAmmount;
		}
		else {
			// damage and armor
			fSubArmor  = fDamageAmmount*2.0f/3.0f;      // 2/3 on armor damage
			fSubHealth = fDamageAmmount - fSubArmor;    // 1/3 on health damage
			m_fArmor  -= fSubArmor;                     // decrease armor
			if( m_fArmor<0) {                          // armor below zero -> add difference to health damage
				fSubHealth -= m_fArmor;
				m_fArmor    = 0.0f;
			}
		}
		
		// if any damage
		if( fSubHealth>0) { 
			// if camera is active
			if (m_penCamera!=NULL) {
				// if the camera has onbreak
				CEntity *penOnBreak = ((CCamera&)*m_penCamera).m_penOnBreak;
				if (penOnBreak!=NULL) {
					// trigger it
					SendToTarget(penOnBreak, EET_TRIGGER, this);
					// if it doesn't
				} else {
					// just deactivate camera
					m_penCamera = NULL; 
				}
			}
		}
		
		// if the player is doing autoactions
		if (m_penActionMarker!=NULL) {
			// ignore all damage
			return;
		}
		/*		
		//안태훈 수정 시작
		ptr_tag pHitTag( new CWorldTag );
		pHitTag->SetOffsetPos(const_cast<FLOAT3D&>(vHitPoint));
		FLOAT3D vAxisY(0,1,0);
		
		FLOAT angle = acos(vDirection % vAxisY);
		FLOAT3D axis = vAxisY * vDirection;
		axis.Normalize();
		FLOATquat3D quat;
		quat.FromAxisAngle(axis, angle);
		pHitTag->SetOffsetRot(quat);
		
		switch(dmtType)
		{
		case DMT_BURNING:
			{
				m_fBurnLeftTime = m_fBurnTime;		
			} break;			
		default:
			{
				//CEffectGroup *pEG = CEffectGroupManager::Instance().Create("HIT BLOOD LINE");
				//pEG->AddTagForAllEffect(0, pHitTag);
				//pEG->Start( (FLOAT)_pTimer->GetHighPrecisionTimer().GetSeconds() );
			} break;
		};
		//안태훈 수정 끝
		*/

		int iJob = 0;
		int iHitType = 0;

		if(dmtType != DMT_NONE && fDamageAmmount > 0)
		{
			FLOAT3D vAxisY(0,1,0);
			FLOAT angle = acos(vDirection % vAxisY);
			FLOAT3D axis = vAxisY * vDirection;
			axis.Normalize();
			FLOATquat3D quat;
			quat.FromAxisAngle(axis, angle);

			ObjectBase* pObj = ACTORMGR()->GetObject(eOBJ_CHARACTER, m_iEnermyID);
	
			if (pObj != NULL)
			{
				CCharacterTarget* pCT = static_cast<CCharacterTarget*>(pObj);
	
				if (pCT != NULL)
				{
					iHitType = pCT->cha_iHitType;
				}
				iJob = pObj->GetType();
			}
	
			if (iHitType < 0 || iHitType >= DEF_HIT_TYPE_MAX)
			{
				iHitType = 0;
			}
	
			StartEffectGroup(szHitEffect[iJob][iHitType]		//Hardcording
				, _pTimer->CurrentTick(), vHitPoint, quat);
		}
				
		DamageImpact(dmtType, fSubHealth, vHitPoint, vDirection);
		
		// receive damage
		CPlayerEntity::ReceiveDamage( penInflictor, dmtType, fSubHealth, vHitPoint, vDirection);

		
		
		if(m_bCriticalDamageMe)
		{
			if( iJob == HEALER )
			{
				PlaySound(m_soMessage, GenderSound(SOUND_HEALER_DAMAGE), SOF_3D | SOF_VOLUMETRIC);
			}
			else if( iJob == MAGE )
			{
				PlaySound(m_soMessage, GenderSound(SOUND_MAGE_DAMAGE), SOF_3D | SOF_VOLUMETRIC);
			}
			else if( iJob == TITAN )
			{
				PlaySound(m_soMessage, GenderSound(SOUND_TITAN_DAMAGE), SOF_3D | SOF_VOLUMETRIC);
			}
			else if( iJob == KNIGHT )
			{
				PlaySound(m_soMessage, GenderSound(SOUND_KNIGHT_DAMAGE), SOF_3D | SOF_VOLUMETRIC);
			}
			else if( iJob == ROGUE )
			{
				PlaySound(m_soMessage, GenderSound(SOUND_ROGUE_ATTACK), SOF_3D | SOF_VOLUMETRIC);
			}
			else if( iJob == SORCERER )
			{
				PlaySound(m_soMessage, GenderSound(SOUND_SORCERER_ATTACK), SOF_3D | SOF_VOLUMETRIC);
			}
			else if( IsEXRogue(iJob) )	// [2012/08/27 : Sora] EX로그 추가
			{
				PlaySound(m_soMessage, GenderSound(SOUND_ROGUE_ATTACK), SOF_3D | SOF_VOLUMETRIC);
			}
			else if( IsEXMage(iJob) )	//2013/01/08 jeil EX메이지 추가 
			{
				PlaySound(m_soMessage, GenderSound(SOUND_MAGE_DAMAGE), SOF_3D | SOF_VOLUMETRIC);
			}
			m_bCriticalDamageMe = FALSE;
		}

		// red screen and hit translation
		if( fDamageAmmount>1.0f) {
			// !!!! this is obsolete, DamageImpact is used instead!
			if( dmtType==DMT_EXPLOSION || dmtType==DMT_PROJECTILE || dmtType==DMT_BULLET
				|| dmtType==DMT_IMPACT  /*  || dmtType==DMT_CANNONBALL || dmtType==DMT_CANNONBALL_EXPLOSION*/) 
			{
				//        GiveImpulseTranslationAbsolute( vDirection*(fDamageAmmount/7.5f)
				//                                        -en_vGravityDir*(fDamageAmmount/15.0f));
			}
			if( GetFlags()&ENF_ALIVE) {
				m_fDamageAmmount += fDamageAmmount;
				m_tmWoundedTime   = en_tmEntityTime;
			}
		}
		 
		// yell (this hurts)
		ESound eSound;
		eSound.EsndtSound = SNDT_PLAYER;
		eSound.penTarget  = this;
		SendEventInRange( eSound, FLOATaabbox3D( GetPlacement().pl_PositionVector, 10.0f));
		
		// play hurting sound
		if( dmtType==DMT_DROWNING) {
			SetRandomMouthPitch( 0.9f, 1.1f);
			//PlaySound( m_soMouth, GenderSound(SOUND_DROWN), SOF_3D|SOF_VOLUMETRIC);
			if(_pNetwork->IsPlayerLocal(this)) {IFeel_PlayEffect("WoundWater");}
			m_tmMouthSoundLast = _pTimer->CurrentTick();
			m_soLocalAmbientOnce.Set3DParameters( 25.0f, 5.0f, 0.8f, Lerp( 0.5f, 1.5f, FRnd()));
			//PlaySound( m_soLocalAmbientOnce, SOUND_WATERBUBBLES, SOF_3D|SOF_VOLUMETRIC|SOF_LOCAL);
			SpawnBubbles( 10+INDEX(FRnd()*10));
		} 
		else if( m_fDamageAmmount>1.0f) {
			// if not dead
			if (GetFlags()&ENF_ALIVE) {
				// determine corresponding sound
				INDEX iSound;
				char *strIFeel = NULL;
				if( m_fDamageAmmount<5.0f) {
					//iSound = GenderSound(SOUND_DEFAULT);
					iSound = GenderSound(SOUND_DEFAULT);
					
					strIFeel = "WoundWeak";
				}
				else if( m_fDamageAmmount<25.0f) {
					iSound = GenderSound(SOUND_DEFAULT);
					strIFeel = "WoundMedium";
				}
				else {
					iSound = GenderSound(SOUND_DEFAULT);
					strIFeel = "WoundStrong";
				}
				if( m_pstState==PST_DIVE) {
					iSound = GenderSound(SOUND_DEFAULT);
					strIFeel = "WoundWater";
				} // override for diving
				SetRandomMouthPitch( 0.9f, 1.1f);
				// give some pause inbetween screaming
				TIME tmNow = _pTimer->CurrentTick();
				/*
				if( (tmNow-m_tmScreamTime) > 1.0f) {
					m_tmScreamTime = tmNow;
//					PlaySound( m_soMouth, iSound, SOF_3D|SOF_VOLUMETRIC);					
					if(_pNetwork->IsPlayerLocal(this)) {IFeel_PlayEffect(strIFeel);}
				}
				*/
			}
		}
	};
	
	// should this player blow up (spawn debris)
	BOOL ShouldBlowUp(void) 
	{
		// blow up if
		return
			// allowed
			gam_bGibs && 
			// dead and
			(GetHealth()<=0) && 
			// has received large enough damage lately and
			(m_vDamage.Length() > _fBlowUpAmmount) &&
			// is not blown up already
			GetRenderType()==RT_MODEL;
	};

	// get cos of angle in direction in current gravity plane
	FLOAT GetPlaneFrustumAngle(const FLOAT3D &vDir)
	{
		FLOAT3D vPlaneDelta;
		// find vector from you to target in XZ plane
		GetNormalComponent(vDir, en_vGravityDir, vPlaneDelta);
		// find front vector
		FLOAT3D vFront = -GetRotationMatrix().GetColumn(3);
		FLOAT3D vPlaneFront;
		GetNormalComponent(vFront, en_vGravityDir, vPlaneFront);
		// make dot product to determine if you can see target (view angle)
		vPlaneDelta.SafeNormalize();
		vPlaneFront.SafeNormalize();
		return vPlaneDelta%vPlaneFront;
	};

	/************************************************************
	*                 OVERRIDEN FUNCTIONS                      *
	************************************************************/
	/* Entity info */
	void *GetEntityInfo(void)
	{
//안태훈 수정 시작	//(Modify Camera Behavior)(0.1)
		struct EntityInfo
		{
			EntityInfoBodyType Eeibt;     // body type
			FLOAT fMass;                  // mass (in kg)
			FLOAT vSourceCenter[3];       // body point (offset from handle) when entity look another entity
			FLOAT vTargetCenter[3];       // body point (offset from handle) when entity is target of look
		};
		static EntityInfo ei;
		ei.Eeibt = EIBT_FLESH;
		ei.fMass = 100.0f;
		ei.vSourceCenter[0] = 0;
		ei.vSourceCenter[1] = 1;
		ei.vSourceCenter[2] = 0;
		ei.vTargetCenter[0] = 0;
		ei.vTargetCenter[1] = 1;
		ei.vTargetCenter[2] = 0;
		switch (en_pcCharacter.pc_iPlayerType) {
		case TITAN:
			ei.vTargetCenter[1] = 1.00f;
			break;
		case MAGE:
			ei.vTargetCenter[1] = 0.95f;
			break;
		case HEALER:
			ei.vTargetCenter[1] = 1.00f;
			break;
		case KNIGHT:
			ei.vTargetCenter[1] = 1.00f;
			break;
		case ROGUE:
			ei.vTargetCenter[1] = 1.00f;
			break;
		case SORCERER:
			ei.vTargetCenter[1] = 1.00f;
			break;
		case 7:	// [2012/08/27 : Sora] EX로그 추가
			{
				if( IsEXRogue(en_pcCharacter.pc_iPlayerType) )
				{
					ei.vTargetCenter[1] = 1.00f;
				}
			}
			break;
		/*
		case 8:	//2013/01/08 jeil EX메이지 추가
			{
				if( IsEXMage(en_pcCharacter.pc_iPlayerType) )
				{
					ei.vTargetCenter[1] = 0.95f;
				}
			}
			break;
			*/
		}
		if( IsEXMage(en_pcCharacter.pc_iPlayerType) )	//2013/01/23 jeil EX메이지 체크 방식 수정 
				{
					ei.vTargetCenter[1] = 0.95f;
				}
		return &ei;
//안태훈 수정 끝	//(Modify Camera Behavior)(0.1)
	};
	
	/* Receive item */
	/*
	BOOL ReceiveItem(const CEntityEvent &ee)
	{
		// nothing picked
		return FALSE;
	};	
	*/
	
	// Change Player view
	void ChangePlayerView()
	{
		// change from eyes to 3rd person
		if (m_iViewState == PVT_PLAYEREYES) {
			// spawn 3rd person view camera
			ASSERT(m_pen3rdPersonView == NULL);
			if (m_pen3rdPersonView == NULL) {
				m_pen3rdPersonView = CreateEntity(GetPlacement(), CLASS_PLAYER_VIEW,WLD_AUTO_ENTITY_ID,FALSE);
				EViewInit eInit;
				eInit.eidOwner = this;
				eInit.eidCamera = (CEntity*) NULL;
				eInit.vtView = VT_3RDPERSONVIEW;
				eInit.bDeathFixed = FALSE;
				m_pen3rdPersonView ->Initialize(eInit,FALSE);
			}
			
			m_iViewState = PVT_3RDPERSONVIEW;
			
			// change from 3rd person to eyes
		} else if (m_iViewState == PVT_3RDPERSONVIEW) {
			m_iViewState = PVT_PLAYEREYES;
			
			// kill 3rd person view
			if (m_pen3rdPersonView != NULL) {
				((CPlayerView&)*m_pen3rdPersonView).SendEvent(EEnd(),FALSE);
				m_pen3rdPersonView = NULL;
			}
		}
	};
	
	/*
	// if computer is pressed
	void ComputerPressed(void)
	{
		// call computer if not holding sniper
		//if (GetPlayerWeapons()->m_iCurrentWeapon!=WEAPON_SNIPER){
		if (cmp_ppenPlayer==NULL && _pNetwork->IsPlayerLocal(this)) {
			cmp_ppenPlayer = this;
		}
		m_bComputerInvoked = TRUE;
		// clear analyses message
		m_tmAnalyseEnd = 0;
		m_bPendingMessage = FALSE;
		m_tmMessagePlay = 0;
		//}
	}
	*/	
	
	// if use is pressed
	void UsePressed(BOOL bOrComputer)
	{
		// cast ray from weapon
		CPlayerWeapons *penWeapons = GetPlayerWeapons();
		CEntity *pen = penWeapons->m_penRayHit;
		BOOL bSomethingToUse = FALSE;
		
		// if hit
		if (pen!=NULL) {
			// check switch/messageholder relaying by moving brush
			if (IsOfClass( pen, "Moving Brush")) {
				if (((CMovingBrush&)*pen).m_penSwitch!=NULL) {
					pen = ((CMovingBrush&)*pen).m_penSwitch;
				}
			}
			
			// if switch and near enough
			if (IsOfClass( pen, "Switch") && penWeapons->m_fRayHitDistance < 2.0f) {
				CSwitch &enSwitch = (CSwitch&)*pen;
				// if switch is useable
				if (enSwitch.m_bUseable) {
					// send it a trigger event
					SendToTarget(pen, EET_TRIGGER, this);
					bSomethingToUse = TRUE;
				}
			}
		}
		/*
		// if nothing usable under cursor, and may call computer
		if (!bSomethingToUse && bOrComputer) {
			// call computer
			ComputerPressed();
		}
		else if (!bSomethingToUse)
		{
			CPlayerWeapons *penWeapon = GetPlayerWeapons();
			
			// penWeapon->m_iWantedWeapon==WEAPON_SNIPER) =>
			// make sure that weapon transition is not in progress
			if (penWeapon->m_iCurrentWeapon==WEAPON_SNIPER && 
				penWeapon->m_iWantedWeapon==WEAPON_SNIPER) {
				if (m_ulFlags&PLF_ISZOOMING) {
					m_ulFlags&=~PLF_ISZOOMING;
					penWeapon->m_bSniping = FALSE;
					penWeapon->m_fSniperFOVlast = penWeapon->m_fSniperFOV = penWeapon->m_fSniperMaxFOV;      
					m_soSniperZoom.Stop();
					if(_pNetwork->IsPlayerLocal(this)) {IFeel_StopEffect("SniperZoom");}
				}
				else {
					penWeapon->m_bSniping = TRUE;
					m_ulFlags|=PLF_ISZOOMING;
					penWeapon->m_fSniperFOVlast = penWeapon->m_fSniperFOV = penWeapon->m_fMinimumZoomFOV;
					//PlaySound(m_soSniperZoom, SOUND_SNIPER_ZOOM, SOF_3D|SOF_VOLUMETRIC|SOF_LOOP);
					if(_pNetwork->IsPlayerLocal(this)) {IFeel_PlayEffect("SniperZoom");}
				}
			}
		}
		*/
	}
	
	
	/************************************************************
	*                      PLAYER ACTIONS                      *
	************************************************************/
	void SetGameEnd(void)
	{
		// start console for first player possible
		for(INDEX iPlayer=0; iPlayer<GetMaxPlayers(); iPlayer++) {
			CEntity *pen = GetPlayerEntity(iPlayer);
			if (pen!=NULL) {
				extern ENGINE_API BOOL _bDedicatedServer;
				if (cmp_ppenPlayer==NULL && (_pNetwork->IsPlayerLocal(pen) || _bDedicatedServer)) {
					cmp_ppenPlayer = (CPlayer*)pen;
					if (_pNetwork->IsServer()) {
						if (!GetSP()->sp_bCooperative) {
							for(INDEX i=0; i<NET_MAXGAMEPLAYERS; i++) {
								CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[i];
								if (plt.IsActive() && plt.plt_penPlayerEntity!=NULL) {
									CPlayer *ppl = (CPlayer*)plt.plt_penPlayerEntity;
									EPlayerStats epls;
									epls.iIndex = i;
//									epls.iFrags  = ppl->m_psGameStats.ps_iScore;
									epls.iKills  = ppl->m_psGameStats.ps_iKills;
									epls.iDeaths = ppl->m_psGameStats.ps_iDeaths;
									SendEvent(epls,TRUE);
								}
							}
						}
						SendEvent(EShowConsole(),TRUE);
					}
					break;
				}
			}
		}
		_pNetwork->SetGameFinished();
	}	
	
	FLOAT CalculateViewRotationDamping( void )
	{
		FLOAT fRotationDamping = 1.0f;
		/*
		// if we are zooming
		if (m_ulFlags&PLF_ISZOOMING) {      
			FLOAT fFirstUsedFOV = ClampUp( ((CPlayerWeapons &)*m_penWeapons).m_fSniperFOV, 50.0f );
			fRotationDamping = 0.6f * fFirstUsedFOV/((CPlayerWeapons &)*m_penWeapons).m_fSniperMaxFOV;
			//fRotationDamping = 0.4f * ((CPlayerWeapons &)*m_penWeapons).m_fSniperFOV/((CPlayerWeapons &)*m_penWeapons).m_fSniperMaxFOV;
			return fRotationDamping;
		}
		*/
		
		return fRotationDamping;
	}
	
	// Called to apply player action to player entity each tick.
	void ApplyAction( const CPlayerAction &paOriginal, FLOAT tmLatency)
	{
		
		if(!(m_ulFlags&PLF_INITIALIZED)) { return; }
		//    CPrintF("---APPLY: %g\n", paOriginal.pa_aRotation(1));

/*		
		// remember the time of the last applied action
		if (_pNetwork->IsServer()) 
		{
			m_iLastActionTime = paOriginal.pa_ulCreated;
		}
		*/
		
		// if was not connected
		if (m_ulFlags&PLF_NOTCONNECTED) 
		{
			// set connected state
			SetConnected();
		}
		// mark that the player is connected
		m_ulFlags |= PLF_APPLIEDACTION;
		
		// make a copy of action for adjustments
		CPlayerAction paAction = paOriginal;
		//CPrintF("applying(%s-%08x): %g\n", GetPredictName(), int(paAction.pa_llCreated),
		//  paAction.pa_vTranslation(3));
		
		FLOAT fRotationDamping = CalculateViewRotationDamping();
		
		// calculate delta from last received actions
		ANGLE3D aDeltaRotation;
		ANGLE3D aDeltaViewRotation;
		
		aDeltaRotation(1) = GetRotationDiff(m_iLastRotationH,paAction.pa_aRotation(1));
		aDeltaRotation(2) = GetRotationDiff(m_iLastRotationP,paAction.pa_aRotation(2));
		aDeltaRotation(3) = GetRotationDiff(m_iLastRotationB,paAction.pa_aRotation(3));
		
		
		aDeltaViewRotation(1) = GetRotationDiff(m_iLastViewRotationH,paAction.pa_aViewRotation(1));
		aDeltaViewRotation(2) = GetRotationDiff(m_iLastViewRotationP,paAction.pa_aViewRotation(2));
		aDeltaViewRotation(3) = GetRotationDiff(m_iLastViewRotationB,paAction.pa_aViewRotation(3));
		
		aDeltaRotation*=fRotationDamping;
		aDeltaViewRotation*=fRotationDamping;
		
		SetRotation(m_iLastRotationH, paAction.pa_aRotation(1));
		SetRotation(m_iLastRotationP, paAction.pa_aRotation(2));
		SetRotation(m_iLastRotationB, paAction.pa_aRotation(3));
		SetRotation(m_iLastViewRotationH, paAction.pa_aViewRotation(1));
		SetRotation(m_iLastViewRotationP, paAction.pa_aViewRotation(2));
		SetRotation(m_iLastViewRotationB, paAction.pa_aViewRotation(3));
		
		paAction.pa_aRotation     = aDeltaRotation;
		paAction.pa_aViewRotation = aDeltaViewRotation;
		
		// adjust rotations per tick
		paAction.pa_aRotation /= _pTimer->TickQuantum;
		paAction.pa_aViewRotation /= _pTimer->TickQuantum;
		
		CEntity *penMe = this;		
		
		// limit speeds against abusing
		// 속도 제한.
		paAction.pa_vTranslation(1) = Clamp( paAction.pa_vTranslation(1), -plr_fSpeed,   plr_fSpeed);
		paAction.pa_vTranslation(2) = Clamp( paAction.pa_vTranslation(2), -plr_fSpeedUp, plr_fSpeedUp);
		paAction.pa_vTranslation(3) = Clamp( paAction.pa_vTranslation(3), -plr_fSpeed,   plr_fSpeed);
		
		// if speeds are like walking
		if (Abs(paAction.pa_vTranslation(3))< plr_fSpeed/1.99f
			&&Abs(paAction.pa_vTranslation(1))< plr_fSpeed/1.99f) {
			// don't allow falling
			en_fStepDnHeight = 1.5f;
			// if speeds are like running
		} else {
			// allow falling
			en_fStepDnHeight = -1;
		}
		
		// limit diagonal speed against abusing
		FLOAT3D &v = paAction.pa_vTranslation;
		FLOAT fDiag = Sqrt(v(1)*v(1)+v(3)*v(3));
		if (fDiag>0.01f) 
		{
			FLOAT fDiagLimited = Min(fDiag, plr_fSpeed);
			FLOAT fFactor = fDiagLimited/fDiag;
			v(1)*=fFactor;
			v(3)*=fFactor;
		}
		
		ulButtonsNow		= paAction.pa_ulButtons;
		ulButtonsBefore		= m_ulLastButtons;
		ulNewButtons		= ulButtonsNow&~ulButtonsBefore;
		ulReleasedButtons	= (~ulButtonsNow)&(ulButtonsBefore);
		
		m_ulLastButtons		= ulButtonsNow;         // remember last buttons
		en_plLastViewpoint	= en_plViewpoint;    // remember last view point for lerping

		// FIXME : 왜 이렇게 한거지?-_-;;;
		if((g_slZone==0 || g_slZone==4) && !_bLoginProcess && _pNetwork->GetLoadingZone() == -1)
		{
			CheckField();
			PlayMusic();
		}
		else if( g_slZone == 7 && _pUISWDoc->IsWar() )
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayBGM( MUSIC_CASTLE_BATTLE );
		}

		// if alive
		if (GetFlags() & ENF_ALIVE) 
		{	
			// apply actions				
			AliveActions(paAction);			
			// if not alive rotate camera view and rebirth on fire
		}
		// 죽었을때 카메라 처리하는 부분... 
		else 
		{
			DeathActions(paAction);
		}		
		
		// keep latency for eventual printout
		UpdateLatency(tmLatency);		
	};	
	
	// Called when player is disconnected
	void Disconnect(void)
	{
		// remember name
		m_strName = GetPlayerName();
		// clear the character, so we don't get re-connected to same entity
		en_pcCharacter = CPlayerCharacter();
		// make main loop exit
		SendEvent(EDisconnected());
	};

#define TRANSFORMATION_TIME		1.8f

	// 소서러의 경우에만 해당하는 것으로,
	// 변신체로 변신함.
	void TransformSorcerer( int iType )
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();

		static double dTransformingTime = TRANSFORMATION_TIME;

		StopMove();	
		
		// 시간 계산 
		static bool s_bStart = true;
		static DOUBLE dStartAnimationTime = 0.0f;
				
		// 맨처음 들어올때 시작 시간 체크 
		// ITS#9559 : 소서러 강신 후 존이동하면 장비가 벗은채 서있는 버그
		// 애니메이션 타임 벌기. 실제 변신중이 애니메이션은 캐릭터에 붙어있음. 때문에 이러한 복잡한 구조가 적용됨.
		// 아래 변경된 m_bIsTransform 조건의 경우에는 m_bIsTransform == FALSE이면 변신 요청이므로 애니메이션을 랜더링 해야 하고
		// m_bIsTransform == TRUE이고 s_bStart == true 이면 변신 요청이므로 시간 딜레이가 필요하다.
		if( s_bStart && !m_bIsTransform )
		{
			dStartAnimationTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
			s_bStart = false;
		}

		DOUBLE dDelayForAnimate = _pTimer->GetHighPrecisionTimer().GetSeconds() - dStartAnimationTime;
		
		if( dDelayForAnimate < dTransformingTime )
		{
			return;
		}	

		CTString strNewLook;			
		//SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));
		//BOOL bSuccess	=  SetPlayerAppearanceSka(&m_miRender, NULL, strNewLook, FALSE, iType);
		CPlacement3D plPlacement;	
		plPlacement		= GetPlacement();
		plPlacement.pl_PositionVector(2) = plPlacement.pl_PositionVector(2) + 0.05f;
		SetPlacement(plPlacement);
				
		m_bTransforming = FALSE;
		m_bIsTransform	= TRUE;		
		//m_bMobChange	= TRUE;
		
		//	김영환
		_pNetwork->Set_MyChar_MorphStatus_EVOCATION();
		//_pNetwork->MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_EVOCATION;
		
		//ttos_080423 : 변신중 충돌 박스가 변경되어 충돌 처리가 제대로 이루어지지 않아서 기존 충돌 박스 유지 
		CCollisionInfo pciCollisionInfo= *(this->en_pciCollisionInfo);

		SetSkaModel( TransformInfo().GetFileName(iType) );

		*(this->en_pciCollisionInfo) = pciCollisionInfo;

		for( INDEX t = ANIM_BASIC_BEGIN; t < ANIM_BASIC_END; ++t )
		{
			idPlayerWhole_Animation[t]		= 		ska_GetIDFromStringTable(TransformInfo().GetAnimationName( iType, t ));
		}		
		
		//	김영환
		_pNetwork->Set_MyChar_Effect(1,CStatusEffect::R_NONE);
		//_pNetwork->MyCharacterInfo.itemEffect.Refresh(NULL, 1);
		//_pNetwork->MyCharacterInfo.statusEffect.Refresh(NULL, CStatusEffect::R_NONE);
		s_bStart = true;

		CEntity			*penPlEntity;
		penPlEntity = CEntity::GetPlayerEntity( 0 );

		CModelInstance* pMI = penPlEntity->en_pmiModelInstance;

		_pNetwork->MyCharacterInfo.itemEffect.AddHolyWaterEffect(_pNetwork->MyCharacterInfo.iHitEffectType,
			&(pMI->m_tmSkaTagManager));

		_pNetwork->MyCharacterInfo.itemEffect.RefreshPetStashEffect(&(pMI->m_tmSkaTagManager));
		_pNetwork->MyCharacterInfo.itemEffect.RefreshRelicEffect(&(pMI->m_tmSkaTagManager));
	};
	
	// 본래의 소서러 모습으로 돌아옴.
	void ReturnSorcerer()
	{
		//	김영환
		if(!m_bIsTransform && !ISEVOCATION(_pNetwork->Get_MyChar_MorphStatus()))
		//if(!m_bIsTransform && !ISEVOCATION(_pNetwork->MyCharacterInfo.eMorphStatus))
		{
			return;
		}
		StopMove();	
		CTString strNewLook;

		//SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));
		//BOOL bSuccess =  SetPlayerAppearanceSka(&m_miRender, &en_pcCharacter, strNewLook, FALSE);

		const int iJob = en_pcCharacter.pc_iPlayerType;
		SetSkaModel(CJobInfo::getSingleton()->GetFileName(iJob));

		CPlacement3D plPlacement;	
		plPlacement = GetPlacement();
		plPlacement.pl_PositionVector(2) = plPlacement.pl_PositionVector(2) + 0.05f;
		SetPlacement(plPlacement);

		CModelInstance* pMI = GetModelInstance();
		if(pMI)
		{
			pMI->mi_iFaceMesh		= -1;
			pMI->mi_iHairMesh		= -1;
			//	김영환
			const SBYTE sbFaceStyle = _pNetwork->Get_MyChar_faceStyle();
			const SBYTE sbHairStyle = _pNetwork->Get_MyChar_hairStyle();
			//const SBYTE sbFaceStyle = _pNetwork->MyCharacterInfo.faceStyle;
			//const SBYTE sbHairStyle = _pNetwork->MyCharacterInfo.hairStyle;
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetCharacterAppearance(pMI, iJob, sbHairStyle, sbFaceStyle);
		}

		for( INDEX t = ANIM_BASIC_BEGIN; t < ANIM_BASIC_END; ++t )
		{
			idPlayerWhole_Animation[t]		= 		ska_GetIDFromStringTable(CJobInfo::getSingleton()->GetAnimationName( iJob, t ));	
		}
		
		int nCosIndex = -1;

		for( int i = 0; i < WEAR_TOTAL; ++i )
		{
			nCosIndex = _pNetwork->MyWearCostItem[i].Item_Index;
			
			// 투명 코스튬이 아니어야 한다.
			if(i < WEAR_COSTUME_TOTAL && nCosIndex > 0 &&
				_pNetwork->GetItemData(nCosIndex)->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
			{
				if (i!=WEAR_HELMET || (i==WEAR_HELMET && (CTString)_pNetwork->GetItemData(_pNetwork->MyWearCostItem[i].Item_Index)->GetItemSmcFileName() != MODEL_TREASURE))
				{
					INDEX iPlayerType = en_pcCharacter.pc_iPlayerType;
					CModelInstance *pMI = GetCurrentPlayerModelInstance();		
					_pGameState->DeleteDefaultArmor( pMI, i, iPlayerType );
					_pGameState->WearingArmor(pMI, *_pNetwork->GetItemData(_pNetwork->MyWearCostItem[i].Item_Index));
				}
			}
			else if(_pNetwork->MyWearItem[i].IsEmptyItem() == FALSE)
			{
				if ( i == WEAR_BACKWING && _pNetwork->MyWearCostItem[WEAR_COSTUME_BACKWING].Item_Index > 0 )
				{
					continue;
				}
				CItems* tmpItem = &_pNetwork->MyWearItem[i];
				SWORD nTab, nIdx;
				nTab = _pNetwork->MyWearItem[i].Item_Tab;
				nIdx = _pNetwork->MyWearItem[i].InvenIndex;

				if (i!=WEAR_HELMET || (i==WEAR_HELMET && (CTString)tmpItem->ItemData->GetItemSmcFileName() != MODEL_TREASURE))
				{
					_pGameState->DeleteDefaultArmor( pMI, i, iJob );
					_pGameState->WearingArmor( pMI, *_pNetwork->MyWearItem[i].ItemData );
				}
			}
		}

		m_bIsTransform	= FALSE;
		m_bTransforming	= FALSE;
		// 김영환
		_pNetwork->Set_MyChar_MorphStatus_MORPH_END();
		//_pNetwork->MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_END;
		//m_bMobChange	= FALSE;
		//SetPhysicsFlags(GetPhysicsFlags() | EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY);
	};

	// 변신체로 변신.
	void MonsterChange(int type)
	{
		//TransformSorcerer( 0 );		
		StopMove();	

		CTString strNewLook;	

		CPlacement3D plPlacement;	
		plPlacement = GetPlacement();
		plPlacement.pl_PositionVector(2) = plPlacement.pl_PositionVector(2) + 0.05f;
		SetPlacement(plPlacement);
		
		m_bMobChange = TRUE;

		CMobData* MD = CMobData::getData( type );

		CStaticArray<struct ColisionBox>  AABox;
		AABox.CopyArray(GetModelInstance()->mi_cbAABox);
		
		SetSkaModel( MD->GetMobSmcFileName() );
		GetModelInstance()->mi_cbAABox.Clear();
		GetModelInstance()->mi_cbAABox = AABox;

		idPlayerWhole_Animation[ANIM_ATTACK_IDLE] = idPlayerWhole_Animation[ANIM_IDLE] = ska_GetIDFromStringTable( MD->GetAnimDefaultName() );
		idPlayerWhole_Animation[ANIM_RUN_2]		= idPlayerWhole_Animation[ANIM_RUN_1] = ska_GetIDFromStringTable( MD->GetAnimRunName() );
		idPlayerWhole_Animation[ANIM_WALK_2]	= idPlayerWhole_Animation[ANIM_WALK_1] = ska_GetIDFromStringTable( MD->GetAnimWalkName() );
		idPlayerWhole_Animation[ANIM_DIE]		= ska_GetIDFromStringTable( MD->GetAnimDeathName() );
		idPlayerWhole_Animation[ANIM_DAMAGE]	= ska_GetIDFromStringTable( MD->GetAnimWoundName() );
	
		_pNetwork->MyCharacterInfo.itemEffect.Refresh(NULL, 1);
		_pNetwork->MyCharacterInfo.statusEffect.Refresh(NULL, CStatusEffect::R_NONE);
	}
	
	// 변신체에서 본래모습으로 되돌아 옵니다.
	virtual void ReturnChange(BOOL bAppear)
	{
		//ReturnSorcerer();		
		if(!m_bMobChange && !ISTRANSFORMATION(_pNetwork->MyCharacterInfo.eMorphStatus))
		{
			return;
		}

		CJobInfo* pInfo = CJobInfo::getSingleton();

		StopMove();	
		CTString strNewLook;	

		CPlacement3D plPlacement;	
		plPlacement = GetPlacement();
		plPlacement.pl_PositionVector(2) = plPlacement.pl_PositionVector(2) + 0.05f;
		SetPlacement(plPlacement);

		const int iJob = en_pcCharacter.pc_iPlayerType;
		GetModelInstance()->Clear();
		SetSkaModel(pInfo->GetFileName(iJob));		

		CModelInstance* pMI = GetModelInstance();
		if(pMI)
		{
			pMI->mi_iFaceMesh		= -1;
			pMI->mi_iHairMesh		= -1;
			//	김영환
			const SBYTE sbFaceStyle = _pNetwork->Get_MyChar_faceStyle();
			const SBYTE sbHairStyle = _pNetwork->Get_MyChar_hairStyle();
			//const SBYTE sbFaceStyle = _pNetwork->MyCharacterInfo.faceStyle;
			//const SBYTE sbHairStyle = _pNetwork->MyCharacterInfo.hairStyle;
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetCharacterAppearance(pMI, iJob, sbHairStyle, sbFaceStyle);
		}

		if(bAppear)
		{
			pMI->StretchModel(FLOAT3D(1.0f,1.0f,1.0f));
		}
		else
		{
			pMI->StretchModel(FLOAT3D(0.1f,0.1f,0.1f));
		}

		// 몹으로 변신시 바꾼 ID값만 되돌린다.
		idPlayerWhole_Animation[ANIM_IDLE]			= ska_GetIDFromStringTable( pInfo->GetAnimationName( iJob, ANIM_IDLE ) );
		idPlayerWhole_Animation[ANIM_RUN_1]			= ska_GetIDFromStringTable( pInfo->GetAnimationName( iJob, ANIM_RUN_1 ) );
		idPlayerWhole_Animation[ANIM_WALK_1]		= ska_GetIDFromStringTable( pInfo->GetAnimationName( iJob, ANIM_WALK_1 ) );
		idPlayerWhole_Animation[ANIM_DIE]			= ska_GetIDFromStringTable( pInfo->GetAnimationName( iJob, ANIM_DIE ) );
		idPlayerWhole_Animation[ANIM_ATTACK_IDLE]	= ska_GetIDFromStringTable( pInfo->GetAnimationName( iJob, ANIM_ATTACK_IDLE ) );
		idPlayerWhole_Animation[ANIM_RUN_2]			= ska_GetIDFromStringTable( pInfo->GetAnimationName( iJob, ANIM_RUN_2 ) );
		idPlayerWhole_Animation[ANIM_WALK_2]		= ska_GetIDFromStringTable( pInfo->GetAnimationName( iJob, ANIM_WALK_2 ) );
		idPlayerWhole_Animation[ANIM_DAMAGE]		= ska_GetIDFromStringTable( pInfo->GetAnimationName( iJob, ANIM_DAMAGE ) );
		
		int nCosIndex = -1;

		for( int i = 0; i < WEAR_TOTAL; ++i )
		{
			nCosIndex = _pNetwork->MyWearCostItem[i].Item_Index;

			// 투명 코스튬이 아니어야 한다.
			if(i < WEAR_COSTUME_TOTAL && nCosIndex > 0 &&
				_pNetwork->GetItemData(nCosIndex)->IsFlag(nCosIndex) == false)
			{
				if (i!=WEAR_HELMET || (i==WEAR_HELMET && (CTString)_pNetwork->GetItemData(_pNetwork->MyWearCostItem[i].Item_Index)->GetItemSmcFileName() != MODEL_TREASURE))
				{
					INDEX iPlayerType = en_pcCharacter.pc_iPlayerType;
					CModelInstance *pMI = GetCurrentPlayerModelInstance();		
					_pGameState->DeleteDefaultArmor( pMI, i, iPlayerType );
					_pGameState->WearingArmor(pMI, *_pNetwork->GetItemData(_pNetwork->MyWearCostItem[i].Item_Index));
				}
			}
			else if(_pNetwork->MyWearItem[i].IsEmptyItem() == FALSE)
			{
				if ( i == WEAR_BACKWING && _pNetwork->MyWearCostItem[WEAR_COSTUME_BACKWING].Item_Index > 0 )
				{
					continue;
				}
				CItems* tmpItem = &_pNetwork->MyWearItem[i];
				SWORD nTab, nIdx;
				nTab = _pNetwork->MyWearItem[i].Item_Tab;
				nIdx = _pNetwork->MyWearItem[i].InvenIndex;

				if (i!=WEAR_HELMET || (i==WEAR_HELMET && (CTString)tmpItem->ItemData->GetItemSmcFileName() != MODEL_TREASURE))
				{
					_pGameState->DeleteDefaultArmor( pMI, i, iJob );
					_pGameState->WearingArmor( pMI, *_pNetwork->MyWearItem[i].ItemData );
				}
			}
		}

		m_bMobChange = FALSE;
		SetPhysicsFlags(GetPhysicsFlags() | EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY);
	}

	void MinMaximize()
	{
		static BOOL bMinimize = TRUE;

		if(m_bChanging)
		{
			if(bMinimize)//작아진다. 
			{
				static int cnt=0;//누적 카운트

				static unsigned int tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);

				if(cnt==0)
				{
					tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);		
					cnt++;
				}

				int time = ((unsigned int(_pTimer->GetLerpedCurrentTick()*1000) - tmStartTime )%1000)/100;
				if(time < 1)
				{
					return;
				}

				cnt = cnt+ time;

				if(cnt > 9)
				{
					cnt = 9;
				}		

				float fScale = 0.1f * (10 - cnt);

				if(cnt==9)
				{				
					if(!m_bMobChange)
					{
						MonsterChange(m_nChangeMonsterId);//다 작아졌으니 변신한다.
					}
					else
					{
						ReturnChange(FALSE);	//다 작아졌으니 본모습으로 돌아온다.
					}

					bMinimize = FALSE;	
					cnt = 0;			
				}			

				// fix. Stretch값이 0.1f에서 1로 변경 되는 현상.
				// MonsterChange(), ReturnChange(), 함수 내부에 SetSkaModel() 함수 호출시 Stretch값이 1로 세팅됨.
				GetModelInstance()->StretchModel(FLOAT3D( fScale, fScale,fScale ));
			}
			else//이제 커져야한다.
			{

				static int cnt=10;//누적 카운트

				static unsigned int tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);	
				
				if(cnt==10)
				{
					tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);		
					cnt--;

					if(m_nChangeMonsterId == GM_CHANGE)
					{
						static INDEX animID = ska_StringToID("cusedrich_skill");
						static FLOAT animTime = GetModelInstance()->GetAnimLength(animID);
				//		AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
						AddAnimation(animID, AN_NORESTART, 1.0f, PLAYER_ANIM_GROUP_WHOLEBODY, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
						StartEffectGroup("cusedrich_disguise", &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
					}
					else

					{
						StartEffectGroup("Polymorph", &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						PlaySound(m_soMessage, SOUND_POLYMOPH, SOF_3D | SOF_VOLUMETRIC);	

					}
				}
		

				int time = ((unsigned int(_pTimer->GetLerpedCurrentTick()*1000) - tmStartTime )%1000)/100;
				if(time < 1)
				{
					return;
				}

				cnt = cnt - time;

				if(cnt < 1)
				{
					cnt = 1;
				}		

				if(m_nChangeMonsterId == GM_CHANGE)
				{
					GetModelInstance()->StretchModel(FLOAT3D( GM_CHANGE_MUL*(11-cnt),GM_CHANGE_MUL*(11-cnt),GM_CHANGE_MUL*(11-cnt) ));
				}
				else 
				{
					GetModelInstance()->StretchModel(FLOAT3D( 0.1f*(11-cnt),0.1f*(11-cnt),0.1f*(11-cnt) ));
				}				

					
				if(cnt==1)
				{
					SetSkaColisionInfo(); //마지막에 충돌박스를 다시 셋팅해줘야 현재의 크기로 셋팅된다.
					m_bChanging = FALSE;	//변신이 완전 끝났다.	
					bMinimize = TRUE;	
					if(m_bMobChange == FALSE)
					{	// 인간으로 복귀
						// 김영환
						_pNetwork->Set_MyChar_MorphStatus_MORPH_END();						
					}
					else
					{
						// 김영환
						_pNetwork->Set_MyChar_MorphStatus_TRANSFORMATION();						
					}

					cnt = 10;						
					
					if(m_nPlayActionNum == ACTION_NUM_SELL)
					{
						m_nActionSit = 1;//앉는 모션 시작.
						if(_pNetwork->MyCharacterInfo.job == NIGHTSHADOW )
						{
							DeleteWearingWeapon(FALSE,FALSE);
						}
						m_bPlayAction = TRUE;								
					}

 					CPlacement3D plPlacement;	
 					plPlacement = GetPlacement();
 					plPlacement.pl_PositionVector(2) = plPlacement.pl_PositionVector(2) + 0.2f;
 					SetPlacement(plPlacement);

					_pNetwork->MyCharacterInfo.itemEffect.Refresh(&en_pmiModelInstance->m_tmSkaTagManager, 1);
					_pNetwork->MyCharacterInfo.statusEffect.Refresh(&en_pmiModelInstance->m_tmSkaTagManager, CStatusEffect::R_NONE);

					UIMGR()->SetCSFlagOff(CSF_TELEPORT);
				}
			}
		}
	}

	// 강신 관련 처리...
	void Transforming()
	{
		if(m_bTransforming)
		{
			if(!m_bIsTransform)
			{
				TransformSorcerer(m_iTransformType);
			}
			else
			{
				ReturnSorcerer();
			}
	
		//	m_bTransforming = FALSE;
		}
	}


	// Called when player character is changed
	void CharacterChanged(const CPlayerCharacter &pcNew) 
	{
		// remember original character
		CPlayerCharacter pcOrg = en_pcCharacter;
		
		// set the new character
		en_pcCharacter = pcNew;
		ValidateCharacter();
		
		// if the name has changed
		if (pcOrg.GetName()!=pcNew.GetName()) {
			// report that
			CPrintF(TRANS("%s is now known as %s\n"), 
				pcOrg.GetNameForPrinting(), pcNew.GetNameForPrinting());
		}
		
		// if appearance changed
		CPlayerSettings *ppsOrg = (CPlayerSettings *)pcOrg.pc_aubAppearance;
		CPlayerSettings *ppsNew = (CPlayerSettings *)pcNew.pc_aubAppearance;
		//바뀐 캐릭종류 이름이 같지 않다면, 캐릭을 change.
		//0217
		//    if (memcmp(ppsOrg->ps_achModelFile, ppsNew->ps_achModelFile, sizeof(ppsOrg->ps_achModelFile))!=0) {
		// update your real appearance if possible

		CTString strNewLook;
		//0105
		BOOL bSuccess;
		if(m_bMdl){
			bSuccess = SetPlayerAppearance(&m_moRender, &en_pcCharacter, strNewLook, /*bPreview=*/FALSE);
		}else{	 	  	
			bSuccess =  SetPlayerAppearanceSka(&m_miRender, &en_pcCharacter, strNewLook, FALSE);		
		}

// 강동민 수정 시작		// 캐릭터 외모 바꾸기
		const int iJobType = en_pcCharacter.pc_iPlayerType;
		SetSkaModel(CJobInfo::getSingleton()->GetFileName(iJobType));
// 강동민 수정 끝		// 캐릭터 외모 바꾸기
//안태훈 수정 시작	//(Open beta)(2004-12-14)
		if(GetModelInstance())
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			FLOATaabbox3D aabb;
			GetModelInstance()->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2) * GetModelInstance()->mi_vStretch(2), 0);
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
		}
//안태훈 수정 끝	//(Open beta)(2004-12-14)

		//..
		// if succeeded
		if (bSuccess) {
			ParseGender(strNewLook);
			// report that
		//	CPrintF(TRANS("%s now appears as %s\n"), 
		//		pcNew.GetNameForPrinting(), strNewLook);
			// if failed
		} else {
			// report that
			CPrintF(TRANS("Cannot change appearance for %s: setting '%s' is unavailable\n"), 
				pcNew.GetNameForPrinting(), (const char*)ppsNew->GetModelFilename());
		}
		// attach weapon to new appearance
		//0105
		/*
		if(m_bMdl){
			GetPlayerAnimator()->SyncWeapon();
		}
		*/
		//..
		//    }  //0217
/*		
		BOOL b3RDPersonOld = ppsOrg->ps_ulFlags&PSF_PREFER3RDPERSON;
		BOOL b3RDPersonNew = ppsNew->ps_ulFlags&PSF_PREFER3RDPERSON;
		if ((b3RDPersonOld && !b3RDPersonNew && m_iViewState==PVT_3RDPERSONVIEW)
			||(b3RDPersonNew && !b3RDPersonOld && m_iViewState==PVT_PLAYEREYES) ) {
			ChangePlayerView();
		}
*/
//강동민 수정 시작 클로즈 준비 작업	08.10
		//CreateAnimAndBoneIDs();
		FillAnimAndBoneIDs();
//강동민 수정 끝 클로즈 준비 작업		08.10
//0409 kwon 캐릭터 바꿀때 좀더 높이 생성.
		CPlacement3D plPlacement;	
		plPlacement = GetPlacement();
		plPlacement.pl_PositionVector(2) = plPlacement.pl_PositionVector(2) + 0.05f;
		SetPlacement(plPlacement);
		
//안태훈 수정 시작	//(For Performance)(0.1)
		CSkaTag tag;
		tag.SetName("__ROOT");
		tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
		en_pmiModelInstance->m_tmSkaTagManager.Register(&tag);
		tag.SetName("__TOP");
		tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
		FLOATaabbox3D aabb;
		GetModelInstance()->GetAllFramesBBox(aabb);
		tag.SetOffsetPos(0, aabb.Size()(2) * GetModelInstance()->mi_vStretch(2), 0);
		GetModelInstance()->m_tmSkaTagManager.Register(&tag);
//안태훈 수정 끝	//(For Performance)(0.1)
	};
	
	// 이기환 추가 (04.12.29) : ESC키로 취소되는 동작이 활성화 되어 있는지 체크
	BOOL CheckEscKey(void)
	{
		if( !m_bCanSkillCancel )
		{
			return FALSE;
		}

		CSkill* pSkill = &_pNetwork->GetSkillData(m_nCurrentSkillNum);

		if ((pSkill != NULL && pSkill->bCanCancel == FALSE) || m_bNoAniGuildSkill)
		{
			return FALSE;
		}

		if ( m_bSkilling || m_bForward || m_bStartAttack || m_bKeyMove )
		{
			return TRUE;
		}
	
		CEntity *penTarget		= GetPlayerWeapons()->m_penRayHitTmp;
		CEntity *penReservedTarget	= GetPlayerWeapons()->m_penReservedTarget;
		
		if ( penTarget != NULL )
		{
			if( penTarget->IsEnemy() || 
				( penTarget->IsCharacter() ) || 
				( penTarget->IsPet() ) ||
				( penTarget->IsSlave() ) ||
				( penTarget->IsWildPet() ) ||
				( penTarget->GetFlags()&ENF_ITEM ) || penTarget == this)
			{
				return TRUE;
			} 
		}

		if ( penReservedTarget != NULL )
		{
			if( penReservedTarget->IsEnemy() || 
				( penReservedTarget->IsCharacter()) || 
				( penReservedTarget->IsPet()) ||
				( penReservedTarget->IsSlave()) ||
				( penReservedTarget->IsWildPet()) ||
				( penReservedTarget->GetFlags()&ENF_ITEM ) || penReservedTarget == this)
			{
				return TRUE;
			} 
		}

		if(m_nCurrentSkillNum != -1 || m_nDesiredSkillNum != -1)
		{
			return TRUE;
		}
			
		return FALSE;
	}


	// Alive actions
	void AliveActions(const CPlayerAction &pa) 
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();

		// reset acceleration/deceleration to default values
		en_fAcceleration = plr_fAcceleration;
		en_fDeceleration = plr_fDeceleration;
		
		CPlayerAction paAction = pa;
		
		// if camera is active
		//카메라 뷰 상태에서 키보드를 누르면 카메라를 NULL로...처리함.
		
		extern BOOL _bWorldEditorApp;
		if (m_penCamera!=NULL && !_bInFixedCamera)
		{
			if(!_bLoginProcess || _bWorldEditorApp)
			{
				// ignore keyboard/mouse/joystick commands
				paAction.pa_vTranslation  = FLOAT3D(0,0,0);
				paAction.pa_aRotation     = ANGLE3D(0,0,0);
				paAction.pa_aViewRotation = ANGLE3D(0,0,0);
				// if fire or use is pressed
				if (ulNewButtons&(PLACT_FIRE|PLACT_USE)) 
				{
					// stop camera
					m_penCamera=NULL;
				}
			}
		} 
		else 
		{
			ButtonsActions(paAction);
		}
		
		// NOTE : 게임 ON 일때만 루틴을 타도록...
		// 테스트 게임 일때도...
		if( STAGEMGR()->GetCurStage() == eSTAGE_GAMEPLAY || _bInTestGame)
		{
			// do the actions
			ActiveActions(paAction);
		}
	}
	
	// Auto-actions
	// NOTE : 카메라 이벤트를 제외하곤 안쓰일 부분...
	void AutoActions(const CPlayerAction &pa) 
	{
		// if we are in 1st person view
		if (m_penCamera==NULL) 
		{
			// break the auto actions to make sure that player is not stuck in loop by accident
			EAutoAction eAutoAction;
			eAutoAction.eidFirstMarker = (CEntity*)NULL;
			SendEvent(eAutoAction);
		}
		
		// if fire, use or computer is pressed
		if (ulNewButtons&(PLACT_FIRE|PLACT_USE/*|PLACT_COMPUTER*/)) {
			if (m_penCamera!=NULL) {
				CEntity *penOnBreak = ((CCamera&)*m_penCamera).m_penOnBreak;
				if (penOnBreak!=NULL) {
					SendToTarget(penOnBreak, EET_TRIGGER, this);
				}
			}
		}
		
		CPlayerAction paAction = pa;
		// ignore keyboard/mouse/joystick commands
		paAction.pa_vTranslation  = FLOAT3D(0,0,0);
		paAction.pa_aRotation     = ANGLE3D(0,0,0);
		paAction.pa_aViewRotation = ANGLE3D(0,0,0);
		
		// if moving towards the marker is enabled
		if (m_fAutoSpeed>0) 
		{
			FLOAT3D vDelta = 
				m_penActionMarker->GetPlacement().pl_PositionVector-
				GetPlacement().pl_PositionVector;

			FLOAT fDistance = vDelta.Length();
			if (fDistance>0.1f) 
			{
				vDelta/=fDistance;
				ANGLE aDH = GetRelativeHeading(vDelta);
				
				// if should hit the marker exactly
				FLOAT fSpeed = m_fAutoSpeed;
				if (GetActionMarker()->m_paaAction==PAA_RUNANDSTOP) 
				{
					// adjust speed
					fSpeed = Min(fSpeed, fDistance/_pTimer->TickQuantum);
				}
				// adjust rotation
				if (Abs(aDH)>1.0f) 
				{
					if (fSpeed>m_fAutoSpeed-0.1f) 
					{
						aDH = Clamp(aDH, -10.0f, 10.0f);
					}
					paAction.pa_aRotation = ANGLE3D(aDH/_pTimer->TickQuantum,0,0);
				}
				// set forward speed
				paAction.pa_vTranslation = FLOAT3D(0,0,-fSpeed);
			}
		} 
		else 
		{
			paAction.pa_vTranslation = m_vAutoSpeed;
		}

		// do the actions
		if (!(m_ulFlags&PLF_AUTOMOVEMENTS)) {
			ActiveActions(paAction);
		}
	}
	
	void GetLerpedWeaponPosition( FLOAT3D vRel, CPlacement3D &pl)
	{
		pl = CPlacement3D( vRel, ANGLE3D(0,0,0));
		CPlacement3D plView;
		_bDiscard3rdView=GetViewEntity(FALSE)!=this;
		GetLerpedAbsoluteViewPlacement(plView);
		pl.RelativeToAbsolute( plView);
	}
	
	void SpawnBubbles( INDEX ctBubbles)
	{
		for( INDEX iBouble=0; iBouble<ctBubbles; iBouble++)
		{
			FLOAT3D vRndRel = FLOAT3D( (FRnd()-0.5f)*0.25f, -0.25f, -0.5f+FRnd()/10.0f);
			ANGLE3D aDummy = ANGLE3D(0,0,0);
			CPlacement3D plMouth = CPlacement3D( vRndRel, aDummy);
			
			plMouth.RelativeToAbsolute( en_plViewpoint);
			plMouth.RelativeToAbsolute( GetPlacement());
			FLOAT3D vRndSpd = FLOAT3D( (FRnd()-0.5f)*0.25f, (FRnd()-0.5f)*0.25f, (FRnd()-0.5f)*0.25f);
			plMouth.pl_PositionVector(2) += 0.5f;
			AddBouble( plMouth.pl_PositionVector, vRndSpd);
		}
	}
	
	/*
	void PlayPowerUpSound ( void ) 
	{
		m_soPowerUpBeep.Set3DParameters(50.0f, 10.0f, 1.0f, 1.0f);
		//PlaySound(m_soPowerUpBeep, SOUND_POWERUP_BEEP, SOF_3D|SOF_VOLUMETRIC|SOF_LOCAL);
	}
	*/
	
	void ActiveActions(const CPlayerAction &paAction)
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();
		ObjInfo* pInfo = ObjInfo::getSingleton();
		// translation
		FLOAT3D vTranslation = paAction.pa_vTranslation;

		// turbo speed cheat
		// 터보 스피드 치트를 썼을때...
// 		if (cht_fTranslationMultiplier && CheatsEnabled()) 
// 		{ 
// 			vTranslation *= cht_fTranslationMultiplier;
// 		}

		if( !GetSP()->sp_bCooperative)
		{
			vTranslation(1) *= 1.35f;
			vTranslation(3) *= 1.35f;
			//en_fDeceleration *= 0.8f;
		}
		
		CContentType &ctUp = GetWorld()->wo_actContentTypes[en_iUpContent];
		CContentType &ctDn = GetWorld()->wo_actContentTypes[en_iDnContent];
		PlayerState pstWanted = PST_STAND;
		BOOL bUpSwimable = (ctUp.ct_ulFlags&CTF_SWIMABLE) && en_fImmersionFactor<=0.99f;
		BOOL bDnSwimable = (ctDn.ct_ulFlags&CTF_SWIMABLE) && en_fImmersionFactor>=0.5f;
		
		// if considerably inside swimable content
		// 신경쓸 필요없음.
		if (bUpSwimable || bDnSwimable) 
		{
			// allow jumping
			m_ulFlags|=PLF_JUMPALLOWED;
			//CPrintF("swimable %f", en_fImmersionFactor);
			// if totaly inside
			if (en_fImmersionFactor>=0.99f || bUpSwimable) 
			{
				// want to dive
				pstWanted = PST_DIVE;
				// if only partially inside
			} 
			else 
			{
				// want to swim
				pstWanted = PST_SWIM;
			}
		} 
		// if not in swimable content
		else {
			// if has reference
			// 바닥에 닿아있는 상태라면....
			if (en_penReference!=NULL) 
			{
				// reset fall timer
				m_fFallTime = 0.0f;
			} 
				// if no reference
			// 아직 바닥에 닿지 않았음...
			else 
			{				
				// increase fall time
				// 떨어지는 시간을 증가시킴.
				m_fFallTime += _pTimer->TickQuantum;
			}
			// if not wanting to jump
			if (vTranslation(2) < 0.1f) 
			{
				// allow jumping
				m_ulFlags|=PLF_JUMPALLOWED;
			}
			
			// if falling
			if (m_fFallTime >= 0.5f) {
				// wants to fall
				pstWanted = PST_FALL;
			}
				// if not falling
			else
			{
				// wants to stand
				pstWanted = PST_STAND;
			}
		}
		{
			PlayerState pstOld = m_pstState; 
			
			// if different state needed
			if (pstWanted!=m_pstState) 
			{
				// check state wanted
				switch(pstWanted) 
				{
				// if wanting to stand
				case PST_STAND: 
					{
						// if can stand here
						if (ChangeCollisionBoxIndexNow(PLAYER_COLLISION_BOX_STAND)) 
						{
							en_plViewpoint.pl_PositionVector(2) = plr_fViewHeightStand;
							((CPlayerAnimator&)*m_penAnimator).Stand();
							m_pstState = PST_STAND;
						}
					}
					break;

				// if wanting to swim
				case PST_SWIM: 
					{
					}
					break;

				// if wanting to dive
				case PST_DIVE: 
					{					
					}
					break;

//강동민 수정 시작 벽타기	10.21
				// 점프했으니까 떨어져야겠지?
				case PST_FALL: 
					{
						// if can fall here
						if (ChangeCollisionBoxIndexNow(PLAYER_COLLISION_BOX_STAND)) 
						{
							m_pstState = PST_FALL;
							en_plViewpoint.pl_PositionVector(2) = plr_fViewHeightStand;
							((CPlayerAnimator&)*m_penAnimator).Fall();
						}
					}
					break;
				}
			}
			//0105 이동시키기
			//  if(GetPlayerWeapons()->m_fRayHitDistance < 100.0f){ // 거리가 멀면 이동하지 않는다.하늘같은 곳
			if(_pInput->m_bTcpIp)
			{
				if(m_bChanging && ISTRANSFORMATION(_pNetwork->MyCharacterInfo.eMorphStatus))
				{
					MinMaximize();
				}
				// 강신 중!!!
				else if(m_bTransforming && ISEVOCATION(_pNetwork->MyCharacterInfo.eMorphStatus))
				{
					Transforming();
				}
				else if(m_bStuned || CheckAction()) //0824 액션 
				{
					
				}
				else
				{		
					if(m_bWaitForSkillResponse && (_pTimer->CurrentTick() - m_tmSendSkillMessage > 5000.0f))
					{
						CTString strSysMessage;
						strSysMessage.PrintF(_S( 2083, "잘못된 조작으로 스킬을 취소합니다." ));		// 번역
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);

						//스킬 캔슬 메시지를 보낸다.
						m_bLockSkillCancel = FALSE;
						m_bWaitForSkillResponse = FALSE;
						_pNetwork->SendCancelSkillMessage();
						pUIManager->SetCSFlagOff( CSF_SKILL );		
						CancelSkill(FALSE, g_iAutoAttack, FALSE);
					}
						
					// 스킬 사용가능 유무 체크.
					if(!CheckSkill())//0713 kwon 
					{
						if(CheckTarget()) //0701 kwon
						{
							ULONG ulFlags = SetDesiredMovement(); 
						}
					}			
				}
			}
			else
			{
				// 움직이지 않음.
				m_vDesiredPosition = GetPlacement().pl_PositionVector;
			}

			//0804 자신의 위치와 카메라각도 세팅하기.
			_pNetwork->SetMyPosition(GetPlacement(),m_fCameraAngle);

			if(m_bSkilling)//0821
			{
				m_bForward = FALSE;			
				m_bKeyMove = FALSE;
			}
			
			// FIXED : 배고픔 or 충성도 0 일때 움직임 금지. [12/23/2010 rumist]
			if (m_bWildRide == TRUE && pInfo->GetMyApetInfo() != NULL)
			{
				if( pInfo->GetMyApetInfo()->m_nFaith <= 0 || pInfo->GetMyApetInfo()->m_nStm <= 0 )
				{
					m_bForward = FALSE;			
					m_bKeyMove = FALSE;
				}
			}

			if (IsSealPet(NULL) == true)
			{
				m_bForward = FALSE;			
				m_bKeyMove = FALSE;
			}

			// FIXME : 이동 루틴 이상함.
			// FIXME : 도대체 뭐가 뭔지 못알아보겠음.
			if( ( (m_bForward || m_bKeyMove) && !m_bHold && !m_bStuned) || ( m_bKeyMove && _pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING ) )
			{
				FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;

				// Edit Start : Dongmin
				FLOAT3D vDesirePosition = m_vDesiredPosition;
				FLOAT3D vOriginPosition = GetPlacement().pl_PositionVector;
				vDesirePosition(2) = 0.0f;
				vOriginPosition(2) = 0.0f;
				FLOAT fHeight = fabs( m_vDesiredPosition(2) - GetPlacement().pl_PositionVector(2) );
				FLOAT fDistance = (vOriginPosition - vDesirePosition).Length();
				FLOAT fAngle = ATan2(fHeight, fDistance);
				//CPrintF( "Angle : %f\n", fAngle );
				// NOTE : Angle은 몇도까지 경사를 올라갈수 있게 할 것인지...
				if( fAngle > MAX_MOVE_SLOPE_GRADE )		// 45도 이상은 올라갈 수 없음.
				{
					vTranslation = FLOAT3D(0.0f, 0.0f, 0.0f);
					//StopRotating();
					StopTranslating();
				}
				else
				{
					// Edit End eons move modifier
					FLOAT3D vDeltatmp = vDelta;
					FLOAT	fLength = plr_fSpeed;
					
					vDeltatmp.Normalize();

					vTranslation = (vDeltatmp*fLength);
/* 회전 값 보상을 하는 부분 삭제
					if(g_bIsRotating && !g_bIsRotateEnd)
					{
						if(aWantedHeadingRelativeTmp > -90 && aWantedHeadingRelativeTmp < 90 )
						{		
							vTranslation(3) = -fabs(g_vMove(3))*plr_fSpeed;
						}
						else if(aWantedHeadingRelativeTmp == -90 && aWantedHeadingRelativeTmp == 90 )
						{
							vTranslation(3) = 0;
						}
						else
						{		
							vTranslation(3) = fabs(g_vMove(3))*plr_fSpeed;
						}
		
						if(aWantedHeadingRelativeTmp > 0 && aWantedHeadingRelativeTmp < 180 )
						{		
							vTranslation(1) = -fabs(g_vMove(1))*plr_fSpeed;
						}
						else if(aWantedHeadingRelativeTmp == 0 || aWantedHeadingRelativeTmp == 180)
						{
							vTranslation(1) = 0;
						}
						else
						{
							vTranslation(1) = fabs(g_vMove(1))*plr_fSpeed;
						}
						vTranslation(2) = g_vMove(2);						
					}
					else
					{
						vTranslation(3) = -plr_fSpeed;
					}*/
				}							
			}

			// NOTE : 플레이어를 실질적으로 이동시키는 부분임.			
			SetDesiredTranslation(vTranslation);
			
			// set pitch and banking from the normal rotation into the view rotation
			en_plViewpoint.Rotate_HPB(ANGLE3D(
				(ANGLE)((FLOAT)paAction.pa_aRotation(1)*_pTimer->TickQuantum),
				(ANGLE)((FLOAT)paAction.pa_aRotation(2)*_pTimer->TickQuantum),
				(ANGLE)((FLOAT)paAction.pa_aRotation(3)*_pTimer->TickQuantum)));

			if (_pInput->inp_bFreeMode)
			{
				((CPlayerView&)*m_pen3rdPersonView).m_FreePosition = m_pen3rdPersonView->GetPlacement();
				((CPlayerView&)*m_pen3rdPersonView).m_FreePosition.Rotate_HPB(ANGLE3D(
				(ANGLE)((FLOAT)paAction.pa_aRotation(1)*_pTimer->TickQuantum),
				(ANGLE)((FLOAT)paAction.pa_aRotation(2)*_pTimer->TickQuantum),
				(ANGLE)((FLOAT)paAction.pa_aRotation(3)*_pTimer->TickQuantum)));
			}

			//m_fCameraAngle += paAction.pa_aRotation(1)*_pTimer->TickQuantum;
			m_fCameraAngle = en_plViewpoint.pl_OrientationAngle(1)*2;			
			// pitch and banking boundaries
			RoundViewAngle(en_plViewpoint.pl_OrientationAngle(2), PITCH_MAX);
			RoundViewAngle(en_plViewpoint.pl_OrientationAngle(3), BANKING_MAX);

			// play moving sounds
			FLOAT fWantSpeed = en_vDesiredTranslationRelative.Length();
			FLOAT fGoesSpeed = en_vCurrentTranslationAbsolute.Length();
			BOOL bOnGround		= (m_pstState == PST_STAND)/*||(m_pstState == PST_CROUCH)*/;
			BOOL bRunning = bOnGround && fWantSpeed>4.0f && fGoesSpeed>4.0f;
			BOOL bWalking = bOnGround && !bRunning && fWantSpeed>1.5f && fGoesSpeed>1.5f;
			BOOL bSwimming = (m_pstState == PST_SWIM) && fWantSpeed>2.0f && fGoesSpeed>2.0f;
			BOOL bDiving = (m_pstState == PST_DIVE) && fWantSpeed>2.0f && fGoesSpeed>2.0f;
			TIME tmNow = en_tmEntityTime;
			static INDEX iSoundWalkL = SOUND_WALK_MARBLE_IN_L;
			static INDEX iSoundWalkR = SOUND_WALK_MARBLE_IN_R;

			if( (ctDn.ct_ulFlags&CTF_SWIMABLE) && en_fImmersionFactor >= 0.1f )
			{
				iSoundWalkL = SOUND_DEFAULT;
				iSoundWalkR = SOUND_DEFAULT;
			}		// yjpark |<--
			else if (en_pbpoStandOn != NULL )		// Bsp
			{
				if( en_pbpoStandOn->bpo_ubPolygonAttribute > BPOA_NONE )
				{
					SBYTE	sbyLayer = ( ( en_pbpoStandOn->bpo_ubPolygonAttribute - 1 ) % 10 ) << 1;
					if( en_pbpoStandOn->bpo_ubPolygonAttribute >= BPOA_STAIR1F2F && en_pbpoStandOn->bpo_ubPolygonAttribute <= BPOA_STAIRWALL4F5F  )
					{
						sbyLayer += 1;
					}

					if ( !(_pNetwork->MyCharacterInfo.sbAttributePos & MATT_PEACE) )
					{
						_pNetwork->MyCharacterInfo.yLayer = sbyLayer;
					}
				}

				//UBYTE	ubJobOffset = _pNetwork->MyCharacterInfo.job * SOUND_BSP_COUNT;

				// 강신했을때는 날라다니기 때문에...
				if( m_bIsTransform )
				{
					if( m_iTransformType == CTransformInfo::TYPE_1 )			// 헬운드
					{
						iSoundWalkL = SOUND_WALK_HELL_WALK;
						iSoundWalkR = SOUND_WALK_HELL_RUN;
					}
					else if( m_iTransformType == CTransformInfo::TYPE_2 )		// 엘레넨
					{
						iSoundWalkL = SOUND_WALK_ELENEN_WALK;
						iSoundWalkR = SOUND_WALK_ELENEN_RUN;
					}
				}
				else if( m_bRide )
				{
					// 레어 펫 추가로 짝수 타입은 말...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iSoundWalkL = -1;
						iSoundWalkR = SOUND_WALK_HORSE_RUN;
					}
					else
					{
						iSoundWalkL = -1;
						iSoundWalkR = -1;
					}
				}
				else
				{
					switch( en_pbpoStandOn->bpo_bppProperties.bpp_ubSurfaceType )
					{
					case LC_SURFACE_MARBLE_IN:
						{
							iSoundWalkL = SOUND_WALK_MARBLE_IN_L;
							iSoundWalkR = SOUND_WALK_MARBLE_IN_R;
							//iSoundWalkL = SOUND_WALK_MARBLE_IN_L + ubJobOffset;
							//iSoundWalkR = SOUND_WALK_MARBLE_IN_R + ubJobOffset;
						}
						break;

					case LC_SURFACE_BLOCK_OUT:
						{
							iSoundWalkL = SOUND_WALK_BLOCK_OUT_L;
							iSoundWalkR = SOUND_WALK_BLOCK_OUT_R;
							//iSoundWalkL = SOUND_WALK_BLOCK_OUT_L + ubJobOffset;
							//iSoundWalkR = SOUND_WALK_BLOCK_OUT_R + ubJobOffset;
						}
						break;

					case LC_SURFACE_WOOD_OUT:
						{
							iSoundWalkL = SOUND_WALK_WOOD_OUT_L;
							iSoundWalkR = SOUND_WALK_WOOD_OUT_R;
							//iSoundWalkL = SOUND_WALK_WOOD_OUT_L + ubJobOffset;
							//iSoundWalkR = SOUND_WALK_WOOD_OUT_R + ubJobOffset;
						}
						break;
						
					case LC_SURFACE_WATER_SHALLOW_IN:
						{
							iSoundWalkL = SOUND_WATER_WALK_SHALLOW_L;
							iSoundWalkR = SOUND_WATER_WALK_SHALLOW_R;
						}
						break;

					default:
						{
							iSoundWalkL = SOUND_WALK_MARBLE_IN_L;
							iSoundWalkR = SOUND_WALK_MARBLE_IN_R;
							//iSoundWalkL = SOUND_WALK_MARBLE_IN_L + ubJobOffset;
							//iSoundWalkR = SOUND_WALK_MARBLE_IN_R + ubJobOffset;
						}
						break;
					}
				}
			}
			else		// Terrain
			{
				//UBYTE	ubJobOffset = _pNetwork->MyCharacterInfo.job * SOUND_TER_COUNT;
				FLOAT	fX = _pNetwork->MyCharacterInfo.x;
				FLOAT	fZ = _pNetwork->MyCharacterInfo.z;
				INDEX	iTerSound = TR_GetTerrainLayerSound( fX, fZ );

				if( iTerSound >= 0 )		// Inside terrain region
				{
					// WSS_DRATAN_SEIGEWARFARE 2007/09/13
					// 필요 없는 부분이라 삭제...
					// 드라탄 공성에서 2층에서 가끔 1층으로 빠지는 부분이 있어 수정...
					// _pNetwork->MyCharacterInfo.yLayer = 0;

					// 강신했을때는 날라다니기 때문에...
					if( m_bIsTransform )
					{
						if( m_iTransformType == CTransformInfo::TYPE_1 )			// 헬운드
						{
							iSoundWalkL = SOUND_WALK_HELL_WALK;
							iSoundWalkR = SOUND_WALK_HELL_RUN;
						}
						else if( m_iTransformType == CTransformInfo::TYPE_2 )		// 엘레넨
						{
							iSoundWalkL = SOUND_WALK_ELENEN_WALK;
							iSoundWalkR = SOUND_WALK_ELENEN_RUN;
						}
					}
					else if( m_bRide )
					{
						// 레어 펫 추가로 짝수 타입은 말...060822 wooss
						if( m_iRideType%2 == CPetInfo::PET_HORSE )					// 말을 타고 있을때...
						{
							iSoundWalkL = -1;
							iSoundWalkR = SOUND_WALK_HORSE_RUN;
						}
						else
						{
							iSoundWalkL = -1;
							iSoundWalkR = -1;
						}
					}
					else
					{
						FLOAT	fWaterDiff = _pNetwork->MyCharacterInfo.fWaterHeight - en_plPlacement.pl_PositionVector(2);
						if( fWaterDiff > 0.01f )
						{
							/*if( fWaterDiff < 0.7f )		// 얕은 물 (찰랑찰랑~~)
							{
							}
							else if( fWaterDiff > 캐릭터의 가슴까지(?)의 높이 )		// 깊은 물 (뽀글뽀글~~)
							{
							}
							else		// 상체가 물 밖으로 나온 상태에서 이동 (물 휘젓는 소리)
							{
							}*/
							iSoundWalkL = SOUND_WATER_WALK_SHALLOW_L;
							iSoundWalkR = SOUND_WATER_WALK_SHALLOW_R;
						}
						else
						{
							iTerSound <<= 1;
							iSoundWalkL = SOUND_TER_WALK_DIRT_L + iTerSound;
							iSoundWalkR = SOUND_TER_WALK_DIRT_R + iTerSound;
							//iSoundWalkL = SOUND_TER_WALK_DIRT_L + iTerSound + ubJobOffset;
							//iSoundWalkR = SOUND_TER_WALK_DIRT_R + iTerSound + ubJobOffset;
						}
					}
				}
			}		// yjpark     -->|
			//iSoundWalkL+=m_iGender*GENDEROFFSET;
			//iSoundWalkR+=m_iGender*GENDEROFFSET;
			if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
			{ // 비행 모드에서는 캐릭터에 등록된 Sound effect로 처리
				// sound skip
			}
			else if (bRunning) 
			{
				if (tmNow>m_tmMoveSound+plr_fRunSoundDelay) 
				{
					m_tmMoveSound = tmNow;
					m_bMoveSoundLeft = !m_bMoveSoundLeft;
					if (m_bMoveSoundLeft && iSoundWalkL != -1) 
					{
						PlaySound(m_soFootL, iSoundWalkL, SOF_3D|SOF_VOLUMETRIC);
					} 
					else if( iSoundWalkR != -1 )
					{
						PlaySound(m_soFootR, iSoundWalkR, SOF_3D|SOF_VOLUMETRIC);
					}
				}
			} 
			else if (bWalking) 
			{
				if (tmNow>m_tmMoveSound+plr_fWalkSoundDelay) 
				{
					m_tmMoveSound = tmNow;
					m_bMoveSoundLeft = !m_bMoveSoundLeft;
					if (m_bMoveSoundLeft && iSoundWalkL != -1) 
					{
						PlaySound(m_soFootL, iSoundWalkL, SOF_3D|SOF_VOLUMETRIC);
					} 
					else if( iSoundWalkR != -1 )
					{
						PlaySound(m_soFootR, iSoundWalkR, SOF_3D|SOF_VOLUMETRIC);
					}
				}
			} 
			else if (bDiving) 
			{
				if (tmNow>m_tmMoveSound+plr_fDiveSoundDelay) 
				{
					m_tmMoveSound = tmNow;
					m_bMoveSoundLeft = !m_bMoveSoundLeft;
					if (m_bMoveSoundLeft) 
					{
						//PlaySound(m_soFootL, GenderSound(SOUND_DIVE_L), SOF_3D|SOF_VOLUMETRIC);
					} 
					else 
					{
						//PlaySound(m_soFootR, GenderSound(SOUND_DIVE_R), SOF_3D|SOF_VOLUMETRIC);
					}
				}
			} 
			else if (bSwimming) 
			{
				if (tmNow>m_tmMoveSound+plr_fSwimSoundDelay) 
				{
					m_tmMoveSound = tmNow;
					m_bMoveSoundLeft = !m_bMoveSoundLeft;
					if (m_bMoveSoundLeft) 
					{
						//PlaySound(m_soFootL, GenderSound(SOUND_SWIM_L), SOF_3D|SOF_VOLUMETRIC);
					} 
					else 
					{
						//PlaySound(m_soFootR, GenderSound(SOUND_SWIM_R), SOF_3D|SOF_VOLUMETRIC);
					}
				}
			}
			
			// if player is almost out of air
			TIME tmBreathDelay = tmNow-en_tmLastBreathed;
			if (en_tmMaxHoldBreath-tmBreathDelay<20.0f) 
			{
				// play drowning sound once in a while
				if (m_tmMouthSoundLast+2.0f<tmNow) 
				{
					m_tmMouthSoundLast = tmNow;
					SetRandomMouthPitch(0.9f, 1.1f);
					//PlaySound(m_soMouth, GenderSound(SOUND_DROWN), SOF_3D|SOF_VOLUMETRIC);
				}
			}
			
			//////////////////////////////////////////////////////////////////////////
			
			//0713 kwon
			if(m_nCurrentSkillNum != -1 && m_bSkilling)
			{
				// 다이어 스트라이크만 처리되는거야???
				if(!SkillAnimationEx() && !HardCodingSkill(FALSE))
				{
					SkillAnimation();
				}
			}
			else
			{
				// animate player
				((CPlayerAnimator&)*m_penAnimator).AnimatePlayer();
			}
		}
	};

	BOOL SkillCondition(INDEX skillIndex, CEntityPointer penTarget, BOOL &bLostTarget, BOOL &bAutoAttack)
	{
		CSkill &skill = _pNetwork->GetSkillData(skillIndex);
		bLostTarget = TRUE;
		bAutoAttack = FALSE;
		switch(m_nCurrentSkillNum)
		{
		case 128://로그, 다이어 스트라이크, 현재 단검만 처리됨
			{
				BOOL bConfused = TRUE;

				if(penTarget->IsEnemy())
				{					
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, penTarget->GetNetworkID());

					if (pObject != NULL)
					{
						CMobTarget* pMT = static_cast< CMobTarget* >(pObject);
						if (pMT->mob_statusEffect.IsConfuse() == FALSE)
						{
							bConfused = FALSE;
						}
					}
				}
				else if(penTarget->IsCharacter())
				{
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, penTarget->GetNetworkID());

					if (pObject != NULL)
					{
						CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

						if (pTarget->cha_statusEffect.IsConfuse() == FALSE)
						{
							bConfused = FALSE;
						}
					}
				}
				else if(penTarget->IsSlave())
				{
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_SLAVE, penTarget->GetNetworkID());

					if (pObject != NULL)
					{
						CSlaveTarget* pTarget = static_cast< CSlaveTarget* >(pObject);

						if (pTarget->slave_statusEffect.IsConfuse() == FALSE)
						{
							bConfused = FALSE;
						}
					}
				}

				if(!bConfused)
				{
					CTString strMsg;
					strMsg.PrintF(_S( 1447, "%s 스킬을 쓰려면 목표가 혼란상태여야 합니다." ), skill.GetName());
					_pNetwork->ClientSystemMessage( strMsg, SYSMSG_ERROR );
					bLostTarget = FALSE;
					bAutoAttack = FALSE;
					return FALSE;
				}
			} return TRUE;

		// HP 가 자신의 체력의 50% 이하 일 때만 사용할수 있는 스킬 
		case 152://타이탄 퍼펙트 바디, 셀프스킬, 현재 hp가 max hp의 50%이하일때 발동가능
		case 153://타이탄 프렌지, 셀프스킬, 현재 hp가 max hp의 50%이하일때 발동가능
		case 228:// 타이탄 버서크
			{
				FLOAT fHP_PERCENT = 0.70f;
				if(_pNetwork->MyCharacterInfo.maxHP * fHP_PERCENT < _pNetwork->MyCharacterInfo.hp)
				{
					CTString strMsg;
					strMsg.PrintF( _S( 2084, "%s 스킬을 쓰려면 현재 hp가 최대 hp의 %.0f 퍼센트 이하여야 합니다." ), skill.GetName(), ( fHP_PERCENT * 100 ) );	// 번역
					_pNetwork->ClientSystemMessage( strMsg, SYSMSG_ERROR );
					return FALSE;
				}
			} return TRUE;
		}
		return TRUE;
	}

	BOOL SkillAnimationEx(void)
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();

		// 나이트 쉐도우 캐릭터만 처리한다.
		// Get Skill Data~
		CSkill& skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
		CEntityPointer penTarget = GetPlayerWeapons()->m_penRayHitTmp; // 공격 대상
		BOOL bEndSkill_Act = FALSE;

		FLOAT skillSpeed = (100 - _pNetwork->MyCharacterInfo.skillSpeed) / 100.0f;

		if (skill.GetJob() == NIGHTSHADOW)
		{
			if (skill.IsNeedTarget())
			{
				if(m_penStillTarget != NULL && (m_penStillTarget->IsEnemy() || m_penStillTarget->IsPet() || m_penStillTarget->IsSlave() || m_penStillTarget->IsCharacter()))
				{
					FLOAT3D vDelta = GetPlacement().pl_PositionVector - m_penStillTarget->GetPlacement().pl_PositionVector;
					vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.
					Rotate(-vDelta, 360.0f, TRUE);
				}
			}

			if (m_bSkilling && skill.IsNeedTarget())
			{ // 타겟이 명확하게 되어야 오류가 없음에 타겟 유효성 검사
				if (m_penStillTarget == NULL || m_penStillTarget->en_RenderType != RT_SKAMODEL || !(m_penStillTarget->GetFlags() & ENF_ALIVE))
				{
					bEndSkill_Act = TRUE;
				}
			}

			// 스킬 타입을 새로 정리하여 switch문으로 타입별로 처리하도록 하자.. (추후에)
			// 현재는 Skill Index로 하드 코딩 한다.
			switch(m_nCurrentSkillNum)
			{
			case 661: // 소울 드레인 (Connect 타입의 still동작으로 유지후 fire에서 효과 발생: 영혼 흡수와 지속 데미지를 주는 스킬)
				{
					if (m_penStillTarget == NULL || !m_penStillTarget->IsEnemy())
					{
						bEndSkill_Act = TRUE;
					}

					if (bEndSkill_Act)
					{
						_pNetwork->SendCancelSkillMessage();
						break;
					}

					if (m_iSkillEffectStep == 0) // Ready action
					{
						m_bSkilling = TRUE;
						++m_iSkillEffectStep;
						INDEX animID = ska_StringToID(skill.GetReadyAnim(_pNetwork->MyCharacterInfo.bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();

						AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);
						m_pSkillReadyEffect = StartEffectGroup(skill.GetReadyEffect(_pNetwork->MyCharacterInfo.bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // PJT_SOUL_DRAIN
					}

					if (m_iSkillEffectStep == 1) // Still action
					{
						if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime - 0.1f))
							//GetModelInstance()->GetAnimLength(ska_StringToID(skill.GetReadyAnim(_pNetwork->MyCharacterInfo.bExtension))) * 0.9f)
						{
							INDEX animID = ska_StringToID(skill.GetStillAnim(_pNetwork->MyCharacterInfo.bExtension));
							m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
							AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);
							
							StopEffectGroupIfValid(m_pSkillReadyEffect, 0.1f);

							m_pAttachPlayerEffect = StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
								&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // PJT_SOUL_DRAIN1 : This

							if (m_penStillTarget->en_RenderType == RT_SKAMODEL && m_penStillTarget->GetFlags() & ENF_ALIVE)
							{
								m_pAttachEnemyEffect = StartEffectGroup(skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension),
									&m_penStillTarget->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // SHK_SOUL_DRAIN1 : Enemy
							}
							
							m_pSkillTexture.SetData_t(CTFILENAME("Data\\Models\\Weapons\\GhostBuster\\Projectile\\Pink_Ray.tex"));
							m_bConnectEffect = TRUE; // Particle()을 통해서 Lighting 구현 시대 --> RenderParticle()에서 Draw한다.
							/*ShotConnect(this, m_penStillTarget, skill.GetDestDelay(_pNetwork->MyCharacterInfo.bExtension),
								skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension), // SHK_SOUL_DRAIN1(hit)
								skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension)); // SHK_SOUL_DRAIN(connect)*/

							++m_iSkillEffectStep;
						}

						return TRUE;
					}

					if (m_iSkillEffectStep == 2) // Fire action
					{
						if (_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > skill.GetDestDelay(_pNetwork->MyCharacterInfo.bExtension))
						{
							if (m_penStillTarget->en_RenderType == RT_SKAMODEL && m_penStillTarget->GetFlags() & ENF_ALIVE)
							{
								_pNetwork->SendSkillMessage(m_nCurrentSkillNum, m_penStillTarget, m_penStillTarget->GetNetworkID(), TRUE);
							}
							StopEffectGroupIfValid(m_pAttachPlayerEffect, 0.1f);
							StopEffectGroupIfValid(m_pAttachEnemyEffect, 0.1f);

							StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension),
								&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // END_SOUL_DRAIN : This
							
							m_bConnectEffect = FALSE;
							bEndSkill_Act = TRUE;
							break;
						}

						return TRUE;
					}
				}
				break;
			case 664: // 퓨리 오브 소울
				{
					if (m_penStillTarget)
					{
						if (m_iSkillEffectStep == 0)
						{
							m_bSkilling = TRUE;
							++m_iSkillEffectStep;
							INDEX animID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
							m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
							AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);
							StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
								&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // PJT_FURY_OF_SOUL : This

						}

						if (m_iSkillEffectStep == 1)
						{
							if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime - 0.1f))
							{
								_pNetwork->SendSkillMessage(m_nCurrentSkillNum, m_penStillTarget, m_penStillTarget->GetNetworkID(), TRUE);

								if (m_penStillTarget->en_RenderType == RT_SKAMODEL && m_penStillTarget->GetFlags() & ENF_ALIVE)
								{
									ShotMissile(this, "HEAD", m_penStillTarget, skill.GetMissileSpeed(_pNetwork->MyCharacterInfo.bExtension),
										skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension), skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension),
										false, 0.0f, 0.0f, 2);
								}

								bEndSkill_Act = TRUE;
								break;
							}
						}
						
						return TRUE;
					}
				}
				break;
			case 666: case 667: case 668: case 669: // 메스 드레인
				{
					if (m_penStillTarget)
					{
						if (!m_penStillTarget->IsEnemy())
						{
							bEndSkill_Act = TRUE;
							break;
						}

						if (m_iSkillEffectStep == 0) // Fire
						{
							m_bSkilling = TRUE;
							++m_iSkillEffectStep;
							INDEX animID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
							m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
							AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);
						}

						if (m_iSkillEffectStep == 1)
						{
							if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime - 0.1f))
							{
								if (m_dcEnemies.Count() > 0)
								{
									ENTITIES_ITERATOR Itr;

									for (Itr = m_dcEnemies.vectorSelectedEntities.begin(); Itr!=m_dcEnemies.vectorSelectedEntities.end(); ++Itr)
									{
										CEntity& tmpEntity = *(*Itr);

										if (tmpEntity.en_RenderType == RT_SKAMODEL && tmpEntity.GetFlags() & ENF_ALIVE)
										{
											StartEffectGroup(skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension),
												&tmpEntity.GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // SHK_SOUL_DRAIN1 : Enemy
										}
									}
								}

								StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension),
									&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // END_SOUL_DRAIN : This

								_pNetwork->SendSkillMessageInContainer(m_nCurrentSkillNum, m_dcEnemies, TRUE);
								
								bEndSkill_Act = TRUE;
								break;
							}

							return TRUE;
						}
					}
				}
				break;
			case 676 : // 소울 버스터
				{
					if (m_iSkillEffectStep == 0)
					{
						m_bSkilling = TRUE;
						++m_iSkillEffectStep;
						INDEX animID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						AddAnimation(animID, AN_NORESTART, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);

						StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // CST_SOUL_BUSTER : This
					}

					if (m_iSkillEffectStep == 1)
					{
						if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime - 0.1f))
						{
							++m_iSkillEffectStep;
							
							if (m_dcEnemies.Count() > 0)
							{
								ENTITIES_ITERATOR Itr;

								for (Itr = m_dcEnemies.vectorSelectedEntities.begin(); Itr!=m_dcEnemies.vectorSelectedEntities.end(); ++Itr)
								{
									CEntity& tmpEntity = *(*Itr);
									
									if (tmpEntity.en_RenderType == RT_SKAMODEL && tmpEntity.GetFlags() & ENF_ALIVE)
									{
										StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension),
											&tmpEntity.GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // SHK_SOUL_BUSTER : Enemy
									}
								}
							}

							DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, FLOAT3D(0,0,0), TRUE);
							_pNetwork->SendSkillMessageInContainer(m_nCurrentSkillNum, m_dcEnemies, TRUE);
							bEndSkill_Act = TRUE;
							break;
						}

						return TRUE;
					}
				}
				break;
			case 677: // 다크 블로우
				{
					if (m_penStillTarget)
					{
						if (m_iSkillEffectStep == 0)
						{
							m_bSkilling = TRUE;
							++m_iSkillEffectStep;
							INDEX animID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
							m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
							AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);
							StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
								&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // PJT_SOUL_DRAIN : This
						}

						if (m_iSkillEffectStep == 1)
						{
							if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime - 0.1f))
							{
								_pNetwork->SendSkillMessage(m_nCurrentSkillNum, m_penStillTarget, m_penStillTarget->GetNetworkID(), TRUE);
								if (m_penStillTarget->en_RenderType == RT_SKAMODEL && m_penStillTarget->GetFlags() & ENF_ALIVE)
								{
									ShotMissile(this, "LHAND", m_penStillTarget, skill.GetMissileSpeed(_pNetwork->MyCharacterInfo.bExtension),
										skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension), skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension),
										false, 0.0f, 0.0f, 2);
								}

								bEndSkill_Act = TRUE;
								break;
							}

							return TRUE;
						}
					}
				}
				break;
			case 764: case 765: case 766: // 오러오브 류
				{
					if (m_iSkillEffectStep == 0)
					{
						m_bSkilling = TRUE;
						++m_iSkillEffectStep;
						INDEX animID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);

						StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // CST_AURA_% : This
					}

					if (m_iSkillEffectStep == 1)
					{
						if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime - 0.1f))
						{
							_pNetwork->SendSkillMessage(m_nCurrentSkillNum, this, _pNetwork->MyCharacterInfo.index, TRUE);
							bEndSkill_Act = TRUE;
							break;
						}

						return TRUE;
					}
				}
				break;
			case 681 : // 코프 오브 나이트
				{
					if (m_penStillTarget)
					{
						if (m_iSkillEffectStep == 0)
						{
							m_bSkilling = TRUE;
							++m_iSkillEffectStep;
							_pNetwork->SendSkillMessage(m_nCurrentSkillNum, m_penStillTarget, m_penStillTarget->GetNetworkID(), TRUE);
							INDEX animID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
							m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
							AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);

							StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
								&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // CST_COPE_OF_LIGHT : This
						}

						if (m_iSkillEffectStep == 1)
						{
							if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime - 0.1f))
							{
								if (m_penStillTarget->en_RenderType == RT_SKAMODEL && m_penStillTarget->GetFlags() & ENF_ALIVE)
								{
									StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension),
										&m_penStillTarget->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // TGT_COPE_OF_LIGHT : Enemy
								}

								bEndSkill_Act = TRUE;
								break;
							}

							return TRUE;
						}
					}
				}
				break;
/*			case 683: // 데들리 라이트
				{
					if (m_penStillTarget)
					{
						if (m_iSkillEffectStep == 0)
						{
							m_bSkilling = TRUE;
							++m_iSkillEffectStep;
							INDEX animID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
							m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
							AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
							
							m_pSkillTexture.SetData_t(CTFILENAME("Data\\Models\\Weapons\\GhostBuster\\Projectile\\Pink_Ray.tex"));
							m_bConnectEffect = TRUE;

							if (m_penStillTarget->en_RenderType == RT_SKAMODEL && m_penStillTarget->GetFlags() & ENF_ALIVE)
							{
								StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension),
									&m_penStillTarget->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
							}
						}

						if (m_iSkillEffectStep == 1)
						{
							if (_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > skill.GetDelay(0, _pNetwork->MyCharacterInfo.bExtension))
							{
								bEndSkill_Act = TRUE;
								m_bConnectEffect = FALSE;
								m_penStillTarget = NULL;
								
								if (m_dcEnemies.Count() > 0)
								{
									DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, FLOAT3D(0,0,0), TRUE);
								}

								break;
							}

							return TRUE;
						}
					}
				}
				break;*/
			case 684: // 저지먼트
				{
					if (m_penStillTarget)
					{
						if (m_iSkillEffectStep == 0) // Fire
						{
							m_bSkilling = TRUE;
							++m_iSkillEffectStep;
							INDEX animID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
							m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
							AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);
							StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
								&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}

						if (m_iSkillEffectStep == 1)
						{
							if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime - 0.1f))
							{
								if (m_penStillTarget->en_RenderType == RT_SKAMODEL && m_penStillTarget->GetFlags() & ENF_ALIVE)
								{
									StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension),
										&m_penStillTarget->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // END_SOUL_DRAIN : This
								}
								
								if (m_dcEnemies.Count() > 0)
								{
									DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, FLOAT3D(0,0,0), TRUE);
								}

								_pNetwork->SendSkillMessageInContainer(m_nCurrentSkillNum, m_dcEnemies, TRUE);

								bEndSkill_Act = TRUE;

								break;
							}

							return TRUE;
						}
					}	
				}
				break;
			case 685: // 미스티 체인
				{
					if (m_penStillTarget)
					{
						if (m_iSkillEffectStep == 0)
						{
							m_bSkilling = TRUE;
							++m_iSkillEffectStep;
							INDEX animID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
							m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
							AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);
							StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
								&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
							m_pSkillTexture.SetData_t(CTFILENAME("Data\\Models\\Weapons\\GhostBuster\\Projectile\\Green_Ray.tex"));
							m_bConnectEffect = TRUE;
						}

						if (m_iSkillEffectStep == 1)
						{
							if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime - 0.1f))
							{
								++m_iSkillEffectStep;

								if (m_penStillTarget->en_RenderType == RT_SKAMODEL && m_penStillTarget->GetFlags() & ENF_ALIVE)
								{
									StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension),
										&m_penStillTarget->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}

							return TRUE;
						}

						if (m_iSkillEffectStep == 2)
						{
							if (_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > skill.GetDestDelay(_pNetwork->MyCharacterInfo.bExtension))
							{
								_pNetwork->SendSkillMessageInContainer(m_nCurrentSkillNum, m_dcEnemies, TRUE);

								if (m_dcEnemies.Count() > 0)
								{
									DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, FLOAT3D(0,0,0), TRUE);
								}
								
								bEndSkill_Act = TRUE;
								m_bConnectEffect = FALSE;
								m_penStillTarget = NULL;
								break;
							}

							return TRUE;
						}
					}
				}
				break;
			case 686: // 페인샷
			case 683: // 데들리 라이트
			case 673:
			case 1353:
				{
					if (m_penStillTarget)
					{
						if (m_iSkillEffectStep == 0)
						{
							m_bSkilling = TRUE;
							++m_iSkillEffectStep;
							INDEX animID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
							m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
							AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);
							StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
								&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}

						if (m_iSkillEffectStep == 1)
						{
							if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime - 0.1f))
							{
								++m_iSkillEffectStep;

								if (m_penStillTarget->en_RenderType == RT_SKAMODEL && m_penStillTarget->GetFlags() & ENF_ALIVE)
								{
									ShotMissile(this, "LHAND", m_penStillTarget, skill.GetMissileSpeed(_pNetwork->MyCharacterInfo.bExtension),
										skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension), skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension),
										false, 0.0f, 0.0f, 2, NULL, &m_dcEnemies);
								}

								_pNetwork->SendSkillMessageInContainer(m_nCurrentSkillNum, m_dcEnemies, TRUE);

								bEndSkill_Act = TRUE;
								break;
							}

							return TRUE;
						}
					}
				}
				break;
			case 687: // 커스오브 블러드
				{
					if (m_penStillTarget)
					{
						if (m_iSkillEffectStep == 0)
						{
							m_bSkilling = TRUE;
							++m_iSkillEffectStep;
							INDEX animID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
							m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
							AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);
							StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
								&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}

						if (m_iSkillEffectStep == 1)
						{
							if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime - 0.1f))
							{
								++m_iSkillEffectStep;
								
								if (m_penStillTarget->en_RenderType == RT_SKAMODEL && m_penStillTarget->GetFlags() & ENF_ALIVE)
								{
									StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension),
										&m_penStillTarget->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								//InflictDirectDamage(penTarget, this, DMT_NONE, 1, FLOAT3D(0,0,0), FLOAT3D(0,0,0));
								_pNetwork->SendSkillMessage(m_nCurrentSkillNum, m_penStillTarget, m_penStillTarget->GetNetworkID(), TRUE);
								bEndSkill_Act = TRUE;
								break;
							}

							return TRUE;
						}
					}
				}
				break;
			case 670: case 671: case 672: // 소울 스크림
				{
					if (m_iSkillEffectStep == 0) // Fire Action
					{
						m_bSkilling = TRUE;
						++m_iSkillEffectStep;
						INDEX animID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);
						StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // CST_SOUL_SCREAM
					}

					if (m_iSkillEffectStep == 1) // Fire Action : Send Fire message
					{
						if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime - 0.1f))
						{
							++m_iSkillEffectStep;

							if (m_dcEnemies.Count() > 0)
							{
								ENTITIES_ITERATOR Itr;

								for (Itr = m_dcEnemies.vectorSelectedEntities.begin(); Itr!=m_dcEnemies.vectorSelectedEntities.end(); ++Itr)
								{
									CEntity& tmpEntity = *(*Itr);
									
									if (tmpEntity.en_RenderType == RT_SKAMODEL && tmpEntity.GetFlags() & ENF_ALIVE)
									{
										StartEffectGroup(skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension),
											&tmpEntity.GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // PJT_SOUL_SCREAM
										
										StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension),
											&tmpEntity.GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // SHK_SOUL_SCREAM
									}
								}

								//DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, FLOAT3D(0,0,0), TRUE);
							}
							
							_pNetwork->SendSkillMessageInContainer(m_nCurrentSkillNum, m_dcEnemies, TRUE);

							bEndSkill_Act = TRUE;
							break;
						}

						return TRUE;
					}
				}
				break;
			}

			if (bEndSkill_Act) // 스킬 마무리
			{
				m_bReadySendSkillMessage = TRUE;

				g_bPostSkill = FALSE;	
				m_bStartPostSkill = TRUE;
				m_bWaitForSkillTarget = FALSE;

				m_iSkillEffectStep = 0;

				m_idCurrentSkillAnim = -1;//스킬 종료.
				m_tmSkillStartTime = 0.0f;
				m_bSkilling = FALSE;
				m_nCurrentSkillNum = -1;
				m_nDesiredSkillNum = -1;

				m_bLockMove = FALSE;
				m_bLockSkillCancel = FALSE;

				m_bCanSkillCancel = TRUE;

				NewClearState(CLEAR_STATE_LENGTH); 
				IdleAnim();				
				
				if(m_nReservedSkillNum != -1)//예약된 스킬이 있다면,
				{
					m_nCurrentSkillNum = m_nReservedSkillNum;
					m_nReservedSkillNum = -1;
				}
				else if (!g_iAutoAttack)
				{
					GetPlayerWeapons()->m_penRayHitTmp = NULL;//0709 이걸지우면 이전에 공격중이었으면 계속 공격. 몹쪽으로 이동중이었으면 계속 이동.
				}
				m_nReservedSkillNum = -1;
				m_penStillTarget = NULL;

				SetFlagOff(ENF_HIDDEN);
				StopRotating();
				
				pUIManager->SetCSFlagOff( CSF_SKILL );
				m_dcEnemies.Clear();
				m_bConnectEffect = FALSE;
				m_pSkillTexture.SetData(NULL);
				StopEffectGroupIfValid(m_pAttachPlayerEffect, 0.1f);
				StopEffectGroupIfValid(m_pAttachEnemyEffect, 0.1f);
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL HardCodingSkill(BOOL bOnlyTest)
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();

		if( m_nCurrentSkillNum != 128	//로그,다이어 스트라이크
		&& m_nCurrentSkillNum != 155	//타이탄, 더블 무브먼트.
		&& m_nCurrentSkillNum != 191	//로그, 데스 모션
		&& m_nCurrentSkillNum != 1540	// [2012/08/27 : Sora] EX로그 추가
		&& m_nCurrentSkillNum != 661	// 소울 드레인
		&& (m_nCurrentSkillNum >= 670 && m_nCurrentSkillNum <= 672) // 소울 스크림
		//&& m_nCurrentSkillNum != 169	//나이트, 테레키네시스
		)
		{
			return FALSE;
		}
		if(bOnlyTest) {return TRUE;}

		m_bLockSkillCancel = FALSE;
		BOOL bIdleAnim = TRUE;
		CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
		CEntityPointer penTarget = GetPlayerWeapons()->m_penRayHitTmp;
		//if(penTarget == NULL) {return TRUE;}
		if(m_afHardCodeValue.Count() == 0) {m_afHardCodeValue.New(32);}

		switch(m_nCurrentSkillNum)
		{
		case 128://로그, 다이어 스트라이크, 현재 단검만 처리됨
			{
				FLOAT fFirstAttackTime = 0.25f/* * _pNetwork->MyCharacterInfo.skillSpeed*/;
				//FLOAT fAttackTime = 0.7f * _pNetwork->MyCharacterInfo.skillSpeed;
				static INDEX iFireAnimID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
				static FLOAT fAttackTimeOnce = GetModelInstance()->GetAnimLength(iFireAnimID);
				static FLOAT fDistFromCB = 1.0f;
				static FLOAT fDamageTime = 999.0f;
				if(penTarget == NULL) {break;}
				
				if(m_iSkillEffectStep == 0)
				{
					++m_iSkillEffectStep;
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					//원래 위치와 방향 저장
					CPlacement3D pl = GetPlacement();
					m_afHardCodeValue[11] = pl.pl_OrientationAngle(1);
					m_afHardCodeValue[12] = pl.pl_OrientationAngle(2);
					m_afHardCodeValue[13] = pl.pl_OrientationAngle(3);
					m_afHardCodeValue[14] = pl.pl_PositionVector(1);
					m_afHardCodeValue[15] = pl.pl_PositionVector(2);
					m_afHardCodeValue[16] = pl.pl_PositionVector(3);
					_pNetwork->MyCharacterInfo.itemEffect.Refresh(NULL, 1);
					_pNetwork->MyCharacterInfo.statusEffect.Refresh(NULL, CStatusEffect::R_NONE);
					SetFlagOn(ENF_HIDDEN);
					FLOAT3D vDir = penTarget->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector;
					m_afHardCodeValue[9] = vDir.Length() / fFirstAttackTime;
					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
					//_pNetwork->SendSkillMessage(m_nCurrentSkillNum, MSG_CHAR_NPC, penTarget
						//, _pNetwork->SearchMobIndex(penTarget->en_ulID), FALSE);

					LONG lTargetIndex = -1;

					lTargetIndex = penTarget->GetNetworkID();
					_pNetwork->SendSkillMessage(m_nCurrentSkillNum, penTarget
						, lTargetIndex, TRUE);
					return TRUE;
				}
				//first
				if(m_iSkillEffectStep == 1)
				{
					if((_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime) < fFirstAttackTime)
					{
						FLOAT delta = _pTimer->GetLerpedCurrentTick() - m_afHardCodeValue[1];
						m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
						FLOAT3D vDir = penTarget->GetPlacement().pl_PositionVector
										- GetPlacement().pl_PositionVector;
						vDir.SafeNormalize();
						CPlacement3D plNew = GetPlacement();
						plNew.pl_PositionVector += vDir * m_afHardCodeValue[9] * delta;
						SetPlacement(plNew);
						//return TRUE;
					}
					++m_iSkillEffectStep;
					//적의 xz평면상의 크기 구하기
					FLOATaabbox3D aabb;
					penTarget->GetModelInstance()->GetCurrentColisionBox(aabb);
					FLOAT3D size = aabb.Size();
					m_afHardCodeValue[0] = sqrtf(size(1) * size(1) + size(3) * size(3)) * 0.5f;//적의크기반지름
					//캐릭터 투명화 끄기
					SetFlagOff(ENF_HIDDEN);
					_pNetwork->MyCharacterInfo.itemEffect.Refresh(&GetModelInstance()->m_tmSkaTagManager, 1);
					_pNetwork->MyCharacterInfo.statusEffect.Refresh(&GetModelInstance()->m_tmSkaTagManager, CStatusEffect::R_PARTY);
					//플레이어의 위치 옮기기
					CPlacement3D plNew;
					FLOAT fRadius = m_afHardCodeValue[0] + fDistFromCB;
					FLOAT3D deltaPos(fRadius*(0.5f*1.7320f/*root 3*/), 0, fRadius*0.5f);
					plNew.pl_PositionVector = penTarget->GetPlacement().pl_PositionVector + deltaPos;
					//plNew.pl_OrientationAngle = GetEulerAngleFromDir((-deltaPos).SafeNormalize());
					SetPlacement(plNew);
					//애니메이션 실시
					NewClearState(0);
					AddAnimation(iFireAnimID, AN_NORESTART, 1, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);

					if( !_pNetwork->MyCharacterInfo.bExtension )
					{
						PlaySound(m_soMessage, GenderSound(SOUND_DAIR_STRIKE_BLOW1), SOF_3D | SOF_VOLUMETRIC);
					}
					else
					{
						PlaySound(m_soMessage, GenderSound(SOUND_A_DAIR_STRIKE_BLOW1), SOF_3D | SOF_VOLUMETRIC);
					}

					StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
										&GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();//시간
					m_afHardCodeValue[2] = 2 * (m_afHardCodeValue[0] + fDistFromCB) / fAttackTimeOnce;//속도
					fDamageTime = skill.GetDelay(0, _pNetwork->MyCharacterInfo.bExtension);
					return TRUE;
				}
				if(m_iSkillEffectStep == 2)
				{
					//플레이어의 위치 옮기기
					FLOAT delta = _pTimer->GetLerpedCurrentTick() - m_afHardCodeValue[1];
					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
					FLOAT deltaDist = m_afHardCodeValue[2] * delta;
					CPlacement3D plNew;
					FLOAT3D deltaDir = (penTarget->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector).SafeNormalize();
					plNew.pl_PositionVector = GetPlacement().pl_PositionVector + deltaDir * deltaDist;
					//plNew.pl_OrientationAngle = GetEulerAngleFromDir(deltaDir);
					SetPlacement(plNew);

					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + fDamageTime)
					{
						FLOAT3D vHitPoint = penTarget->GetPlacement().pl_PositionVector + deltaDir * (-m_afHardCodeValue[0]);
						//InflictDirectDamage(penTarget, this, DMT_CLOSERANGE, 1, vHitPoint, deltaDir);
						StartEffectGroup("Normal Hit", _pTimer->GetLerpedCurrentTick(), vHitPoint, deltaDir);
						fDamageTime = 999.0f;
					}
					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + fAttackTimeOnce)
					{
						fDamageTime = skill.GetDelay(0, _pNetwork->MyCharacterInfo.bExtension);
						++m_iSkillEffectStep;
					}
					return TRUE;
				}
				//second
				if(m_iSkillEffectStep == 3)
				{
					++m_iSkillEffectStep;
					//플레이어의 위치 옮기기
					CPlacement3D plNew;
					FLOAT fRadius = m_afHardCodeValue[0] + fDistFromCB;
					FLOAT3D deltaPos(-fRadius*(0.5f*1.7320f/*root 3*/), 0, fRadius*0.5f);
					plNew.pl_PositionVector = penTarget->GetPlacement().pl_PositionVector + deltaPos;
					//plNew.pl_OrientationAngle = GetEulerAngleFromDir((-deltaPos).SafeNormalize());
					SetPlacement(plNew);
					//애니메이션 실시
					NewClearState(0);
					AddAnimation(iFireAnimID, AN_NORESTART, 1, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);

					if( !_pNetwork->MyCharacterInfo.bExtension )
					{
						PlaySound(m_soMessage, GenderSound(SOUND_DAIR_STRIKE_BLOW2), SOF_3D | SOF_VOLUMETRIC);
					}
					else
					{
						PlaySound(m_soMessage, GenderSound(SOUND_A_DAIR_STRIKE_BLOW2), SOF_3D | SOF_VOLUMETRIC);
					}

					StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
										&GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();//시간
					return TRUE;
				}
				if(m_iSkillEffectStep == 4)
				{
					//플레이어의 위치 옮기기
					FLOAT delta = _pTimer->GetLerpedCurrentTick() - m_afHardCodeValue[1];
					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
					FLOAT deltaDist = m_afHardCodeValue[2] * delta;
					CPlacement3D plNew;
					FLOAT3D deltaDir = (penTarget->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector).SafeNormalize();
					plNew.pl_PositionVector = GetPlacement().pl_PositionVector + deltaDir * deltaDist;
					//plNew.pl_OrientationAngle = GetEulerAngleFromDir(deltaDir);
					SetPlacement(plNew);

					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + fDamageTime)
					{
						FLOAT3D vHitPoint = penTarget->GetPlacement().pl_PositionVector + deltaDir * (-m_afHardCodeValue[0]);
						//InflictDirectDamage(penTarget, this, DMT_CLOSERANGE, 1, vHitPoint, deltaDir);
						StartEffectGroup("Normal Hit", _pTimer->GetLerpedCurrentTick(), vHitPoint, deltaDir);
						fDamageTime = 999.0f;
					}
					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + 2*fAttackTimeOnce)
					{
						fDamageTime = skill.GetDelay(0, _pNetwork->MyCharacterInfo.bExtension);
						++m_iSkillEffectStep;
					}
					return TRUE;
				}
				//third
				if(m_iSkillEffectStep == 5)
				{
					++m_iSkillEffectStep;
					//플레이어의 위치 옮기기
					CPlacement3D plNew;
					FLOAT fRadius = m_afHardCodeValue[0] + fDistFromCB;
					FLOAT3D deltaPos(0, 0, -fRadius*0.5f);
					plNew.pl_PositionVector = penTarget->GetPlacement().pl_PositionVector + deltaPos;
					//plNew.pl_OrientationAngle = GetEulerAngleFromDir((-deltaPos).SafeNormalize());
					SetPlacement(plNew);
					//애니메이션 실시
					NewClearState(0);
					AddAnimation(iFireAnimID, AN_NORESTART, 1, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);

					if( !_pNetwork->MyCharacterInfo.bExtension )
					{
						PlaySound(m_soMessage, GenderSound(SOUND_DAIR_STRIKE_BLOW3), SOF_3D | SOF_VOLUMETRIC);
					}
					else
					{
						PlaySound(m_soMessage, GenderSound(SOUND_A_DAIR_STRIKE_BLOW3), SOF_3D | SOF_VOLUMETRIC);
					}

					StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
										&GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();//시간
					return TRUE;
				}
				if(m_iSkillEffectStep == 6)
				{
					//플레이어의 위치 옮기기
					FLOAT delta = _pTimer->GetLerpedCurrentTick() - m_afHardCodeValue[1];
					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
					FLOAT deltaDist = m_afHardCodeValue[2] * delta;
					CPlacement3D plNew;
					FLOAT3D deltaDir = (penTarget->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector).SafeNormalize();
					plNew.pl_PositionVector = GetPlacement().pl_PositionVector + deltaDir * deltaDist;
					//plNew.pl_OrientationAngle = GetEulerAngleFromDir(deltaDir);
					SetPlacement(plNew);

					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + fDamageTime)
					{
						FLOAT3D vHitPoint = penTarget->GetPlacement().pl_PositionVector + deltaDir * (-m_afHardCodeValue[0]);
						//InflictDirectDamage(penTarget, this, DMT_CLOSERANGE, 1, vHitPoint, deltaDir);
						StartEffectGroup("Normal Hit", _pTimer->GetLerpedCurrentTick(), vHitPoint, deltaDir);
						fDamageTime = 999.0f;
					}
					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + 3*fAttackTimeOnce)
					{
						fDamageTime = skill.GetDelay(0, _pNetwork->MyCharacterInfo.bExtension);
						++m_iSkillEffectStep;
					}
					return TRUE;
				}
				//finalize
				if(m_iSkillEffectStep == 7)
				{
					InflictDirectDamage(penTarget, this, DMT_NONE, 0, FLOAT3D(0,0,0), FLOAT3D(0,0,0));
					CPlacement3D plNew;
					plNew.pl_OrientationAngle(1) = m_afHardCodeValue[11];
					plNew.pl_OrientationAngle(2) = m_afHardCodeValue[12];
					plNew.pl_OrientationAngle(3) = m_afHardCodeValue[13];
					plNew.pl_PositionVector(1) = m_afHardCodeValue[14];
					plNew.pl_PositionVector(2) = m_afHardCodeValue[15];
					plNew.pl_PositionVector(3) = m_afHardCodeValue[16];
					SetPlacement(plNew);
					StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
									, &penTarget->GetModelInstance()->m_tmSkaTagManager
									, _pTimer->GetLerpedCurrentTick());
					fDamageTime = 999.0f;
				}
			} break;
		case 155://타이탄 더블 무브먼트.
			{
				StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
								&GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				_pNetwork->SendSkillMessage(m_nCurrentSkillNum, this, _pNetwork->MyCharacterInfo.index, TRUE);
			} break;
		case 191://로그, 데스 모션
		case 1540:// [2012/08/27 : Sora] EX로그 추가
			{
				static INDEX animID = ska_StringToID(skill.GetFireAnim(_pNetwork->MyCharacterInfo.bExtension));
				static FLOAT animTime = GetModelInstance()->GetAnimLength(animID);
				if(m_iSkillEffectStep == 0)
				{
					++m_iSkillEffectStep;
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					_pNetwork->SendSkillMessage(m_nCurrentSkillNum, this, _pNetwork->MyCharacterInfo.index, TRUE);
					AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
					return TRUE;
				}
				if(m_iSkillEffectStep == 1)
				{
					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime <= animTime)
					{
						return TRUE;
					}
					
					bIdleAnim = FALSE;
					++m_iSkillEffectStep;
				}
			} break;
		default: 
			{
				return FALSE;
			}
			break;
		}

		m_bReadySendSkillMessage = TRUE;

		g_bPostSkill = FALSE;	
		m_bStartPostSkill = TRUE;
		m_bWaitForSkillTarget = FALSE;

		m_iSkillEffectStep = 0;

		m_idCurrentSkillAnim = -1;//스킬 종료.
		m_tmSkillStartTime = 0.0f;
		m_bSkilling = FALSE;
		m_nCurrentSkillNum = -1;
		m_nDesiredSkillNum = -1;

		m_bLockMove = FALSE;

		m_bCanSkillCancel = TRUE;

		if(bIdleAnim)
		{
			NewClearState(CLEAR_STATE_LENGTH); 
			IdleAnim();				
		}
		
		if(m_nReservedSkillNum != -1)//예약된 스킬이 있다면,
		{
			m_nCurrentSkillNum = m_nReservedSkillNum;
			m_nReservedSkillNum = -1;
		}
		else
		{
			// 자동 공격이 아니라면 타겟을 지워줌.
			if (!g_iAutoAttack)
			{
				GetPlayerWeapons()->m_penRayHitTmp = NULL;//0709 이걸지우면 이전에 공격중이었으면 계속 공격. 몹쪽으로 이동중이었으면 계속 이동.
			}
		}
		m_nReservedSkillNum = -1;

		SetFlagOff(ENF_HIDDEN);
		StopRotating();
		
		pUIManager->SetCSFlagOff( CSF_SKILL );

		return TRUE;
	}
	
	// Round view angle
	void RoundViewAngle(ANGLE &aViewAngle, ANGLE aRound) 
	{
		if (aViewAngle > aRound) 
		{
			aViewAngle = aRound;
		}
		if (aViewAngle < -aRound) 
		{
			aViewAngle = -aRound;
		}
	};
	
	// Death actions
	void DeathActions(const CPlayerAction &paAction) 
	{
		
		if (IsReapplyingActions()) {return;}
		SetDesiredTranslation(FLOAT3D(0,0,0));
		/*
		SetDesiredRotation(FLOAT3D(0,0,0));
		//0815 죽었을때도 카메라 움직이도록...
		// set heading, pitch and banking from the normal rotation into the camera view rotation
		if (m_penView!=NULL) {      
			en_plViewpoint.pl_PositionVector = FLOAT3D(0, 1, 0);
			en_plViewpoint.pl_OrientationAngle += (ANGLE3D(
				(ANGLE)((FLOAT)paAction.pa_aRotation(1)*_pTimer->TickQuantum),
				(ANGLE)((FLOAT)paAction.pa_aRotation(2)*_pTimer->TickQuantum),
				(ANGLE)((FLOAT)paAction.pa_aRotation(3)*_pTimer->TickQuantum)));
		}
		*/
		//0815
		en_plViewpoint.Rotate_HPB(ANGLE3D(
			(ANGLE)((FLOAT)paAction.pa_aRotation(1)*_pTimer->TickQuantum),
			(ANGLE)((FLOAT)paAction.pa_aRotation(2)*_pTimer->TickQuantum),
			(ANGLE)((FLOAT)paAction.pa_aRotation(3)*_pTimer->TickQuantum)));

		//m_fCameraAngle += paAction.pa_aRotation(1)*_pTimer->TickQuantum;
		m_fCameraAngle = en_plViewpoint.pl_OrientationAngle(1)*2;

		// pitch and banking boundaries
		RoundViewAngle(en_plViewpoint.pl_OrientationAngle(2), PITCH_MAX);
		RoundViewAngle(en_plViewpoint.pl_OrientationAngle(3), BANKING_MAX);

		if (ulReleasedButtons&PLACT_FIRE) 
		{
			//if( !GetSP()->sp_bCooperative)
			//{
				// rebirth
			//	SendEvent(EEnd());
			//	_pNetwork->SendRebirthMessage();				
			//} 
		}

		if (m_iViewState == PVT_PLAYEREYES) {
			// spawn 3rd person view camera
			//ASSERT(m_pen3rdPersonView == NULL);
			if (m_pen3rdPersonView == NULL) {
				m_pen3rdPersonView = CreateEntity(GetPlacement(), CLASS_PLAYER_VIEW,WLD_AUTO_ENTITY_ID,FALSE);
				EViewInit eInit;
				eInit.eidOwner = this;
				eInit.eidCamera = (CEntity*) NULL;
				eInit.vtView = VT_3RDPERSONVIEW;
				eInit.bDeathFixed = FALSE;
				m_pen3rdPersonView ->Initialize(eInit,FALSE);
			}
			
			m_iViewState = PVT_3RDPERSONVIEW;
			
			// change from 3rd person to eyes
		}

	};

	// Buttons actions
	void ButtonsActions( CPlayerAction &paAction)
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();

		// wooss 060515 ----------------------------------------->>
		if(m_checkLock==1){
			if((_pTimer->GetHighPrecisionTimer().GetSeconds() - m_timeCnt) > 3 ){
				m_bLockMove =FALSE; 
				m_checkLock =0;
				m_timeCnt = 0;
			}
		}

		//-------------------------------------------------------<<

		if (IsReapplyingActions()) 
		{
			return;
		}

		//wooss 050803
		//죽은상태에서 종료되었을 경우 다시 한번 Death를 알려준다 
		if(m_bDying && !m_bDeathChk )
		{
			DeathYou();
		}
		
		if(_pInput->GetInputDevice(1)->id_aicControls[KID_ARROWLEFT].ic_fValue==1.0f
		|| _pInput->GetInputDevice(1)->id_aicControls[KID_NUM4].ic_fValue==1.0f)
		{     
			en_plViewpoint.pl_OrientationAngle(1) += 3;		
		}
		if(_pInput->GetInputDevice(1)->id_aicControls[KID_ARROWRIGHT].ic_fValue==1.0f
		|| _pInput->GetInputDevice(1)->id_aicControls[KID_NUM6].ic_fValue==1.0f)
		{     
			en_plViewpoint.pl_OrientationAngle(1) -= 3;
		}
		if(_pInput->GetInputDevice(1)->id_aicControls[KID_ARROWUP].ic_fValue==1.0f
		|| _pInput->GetInputDevice(1)->id_aicControls[KID_NUM8].ic_fValue==1.0f)
		{     
			en_plViewpoint.pl_OrientationAngle(2) -= 3;		
		}
		if(_pInput->GetInputDevice(1)->id_aicControls[KID_ARROWDOWN].ic_fValue==1.0f
		|| _pInput->GetInputDevice(1)->id_aicControls[KID_NUM2].ic_fValue==1.0f)
		{     
			en_plViewpoint.pl_OrientationAngle(2) += 3;
		}
		
		if (_pInput->GetInputDevice(1)->id_aicControls[KID_SPACE].ic_fValue == 1.0f && 
			_pNetwork->m_ubGMLevel > 1 && // gm일때
			!pUIManager->IsFocusAllEditBox()) // 채팅중이 아닐때
		{
			if (_pInput->GetInputDevice(1)->id_aicControls[KID_LSHIFT].ic_fValue == 1.0f)
			{
				((CPlayerView&)*m_pen3rdPersonView).m_fCameraHeight -= 1.0f;
			}
			else
			{
				((CPlayerView&)*m_pen3rdPersonView).m_fCameraHeight += 1.0f;
			}
			
			FLOAT tmpDist = ((CPlayerView&)*m_pen3rdPersonView).m_fCameraHeight;
			((CPlayerView&)*m_pen3rdPersonView).m_fCameraHeight = Clamp(tmpDist, 0.0f, 10.0f);
		}
			
		if(en_plViewpoint.pl_OrientationAngle(2) >= 20.0f)//카메라 각도 제한하기.
		{
			en_plViewpoint.pl_OrientationAngle(2) = 20.0f;
		}

		if(_pInput->IsLMousePressed())
		{
			ulNewButtons |= PLACT_FIRE;

			_pInput->SetLMousePressed(FALSE);
		}		
		
		// 액션 중에는 움직이지 않게 하기 위해서 액션 내용 체크. [12/9/2009 rumist]
		// 앉기/서기/소셜 동작 중에는 입력이 되지 않는다. 또한, 에디트박스에 포커싱일때도
		// 움직이지 않는다. - 기획결정 : 김영철PD.
		// [100421 sora] 라카렛 사용중에 입력을 받지 않는다
		// Fix me : 텔레포트 상태에서는 움직일 수 없게 추가(캐릭터 상태 플래그를 UIManager가 갖고 있다. 캐릭터 플래그를 별도로 분리작업이 필요하겠다.
		if (pUIManager->IsFocusInAllEditbox() || IsSitting() || IsSocialActing() || 
			pUIManager->IsCSFlagOn(CSF_TELEPORT) || pUIManager->GetLacaBall()->IsVisible() || 
			pUIManager->GetReformSystem()->IsVisible() || _pNetwork->bMoveCharacterSelectUI == TRUE)
		{
			m_bKeyMove = FALSE; // 키보드 이동은 못하게 초기화
		}
		else
		{
			InputKey(0);			
		}
		
		if( _pInput->GetInputDevice(1)->id_aicControls[KID_ESCAPE].ic_fValue )
		{
			int test = 0;
		}
		
		//0826 //휠마우스클릭으로 방향 전환.
		static BOOL _bFlip = FALSE;
		if(_pInput->IsWheelPressed())
		{					
			if(_bFlip)
			{				
				en_plViewpoint.pl_OrientationAngle(1) = GetPlacement().pl_OrientationAngle(1)/2;//0.0f;
				//m_fCameraAngle = GetPlacement().pl_OrientationAngle(1);
			}
			else
			{
				en_plViewpoint.pl_OrientationAngle(1) = GetPlacement().pl_OrientationAngle(1)/2+90;//90.0f;
			//m_fCameraAngle = GetPlacement().pl_OrientationAngle(1)+180.f;
			}
			_bFlip = !_bFlip;

			_pInput->SetWheelPressed(FALSE);
		}

		// HOME 키를 누른것으로...ㄴ
		if(_pInput->IsHomePressed())
		{	
			en_plViewpoint.pl_OrientationAngle(1) = 0.0f;
			_pInput->SetHomePressed(FALSE);
		}

		///////////////////////////////////////////////////////////////////////
		// 이동 속도 적용하는 부분.
		//FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;

		// [070607: Su-won] 테스트 게임에서는 따로 설정
		// 테스트게임에서는 plr_fSpeed 콘솔 명령어를 사용해서 
		// 캐릭터의 이동속도를 설정할 수 있음.
		if( !_bInTestGame )
		{
			if(m_bRunningMode)
			{
				// [100107: selo] MyCharacterInfo.runspeed 대치
				plr_fSpeed = GetRunspeed();
			}		
			else
			{
				// [100107: selo] MyCharacterInfo.walkspeed 대치
				plr_fSpeed = GetWalkspeed();
			}
		}

		// 발바닥에 불붙이는거 때문에 쓰임.
		if(m_bSpeedUp)
		{
			m_tmSeriousSpeed = 1000.0f;
		}
		else
		{
			m_tmSeriousSpeed = 0.0f;
		}

		if( !_bInTestGame && 
			(m_bChanging || 
			m_bTransforming || 
			m_bStuned || 
			!m_apr || 
			m_bPlayAction || 
			_pNetwork->MyCharacterInfo.hp==0 
			/*|| pUIManager->IsCSFlagOn(CSF_CANNOT_MOVE_MASK)*/ //1102 체력이 0이라면 움직이지 않는다.
			))
		{
			return;
		}

		//static unsigned int tmClickTime = timeGetTime();

		///////////////////////////////////////////////////////////////////////
		// FIXME : 아래 코드가 정리가 안됨...-_-;;; 개판임.
		if (!m_bLockMove && (ulNewButtons&PLACT_FIRE || m_bReserveMove || _pInput->IsRMousePressed() 
			// 처음 게임을 시작하거나 어떤 행동을 하고 난 후 키보드 이동이 되지 않는 버그 
			// 이유는 아래 타겟이 없을경우에는 무조건 stopMove를 날려주기때문.
		||( GetPlayerWeapons()->m_penRayHitTmp == NULL && m_bKeyMove )
			 ) && !_pInput->inp_bFreeMode ) 
		{
			if(pUIManager->IsCSFlagOn(CSF_CANNOT_MOVE_MASK))
			{
				return;
			}
			((CPlayerView*)&*m_penView)->UnlockRotate();
            
			if( !m_bReserveMove )
			{
				CPlacement3D plRay;				
				plRay = GetPlayerWeapons()->GetMouseHitInformation( _pInput->inp_ptMousePos ,m_apr, TRUE);				
			}
			else
			{
				if(_pNetwork->IsPlayerLocal(this))
				{
					m_bForward = TRUE; //0107 우선 막아보자.
					//m_bKeyMove = TRUE;
				}
			    GetPlayerWeapons()->m_vRayHitTmp		= GetPlayerWeapons()->m_vRayHitReserve;
			    GetPlayerWeapons()->m_penRayHitTmp		= GetPlayerWeapons()->m_penRayHitReserve;
				GetPlayerWeapons()->m_fRayHitDistanceTmp= GetPlayerWeapons()->m_fRayHitDistanceReserve;
				
				m_vDesiredPosition  = FLOAT3D(GetPlayerWeapons()->m_vRayHitTmp(1),GetPlayerWeapons()->m_vRayHitTmp(2),GetPlayerWeapons()->m_vRayHitTmp(3));
				CEntity *penEnt = GetPlayerWeapons()->m_penRayHitTmp;
				if(penEnt != NULL && penEnt->GetFlags()&ENF_ALIVE)
				{
					m_vDesiredPosition  = penEnt->en_plPlacement.pl_PositionVector;
				}
				else
				{
					m_vDesiredPosition = GetPlayerWeapons()->m_vRayHitTmp;
				}				

				m_bReserveMove = FALSE;

				if(GetPlayerWeapons()->m_penRayHitTmp)
				{
					if( GetPlayerWeapons()->m_penRayHitTmp->GetFlags()&ENF_ALIVE || 
						GetPlayerWeapons()->m_penRayHitTmp->GetFlags()&ENF_ITEM )
					{
						GetPlayerWeapons()->m_penReservedTarget = GetPlayerWeapons()->m_penRayHitTmp;
					}
				}
			}

			// 마우스로 클릭하는 부분을 타겟으로 설정함...
				CEntity *penTarget = GetPlayerWeapons()->m_penReservedTarget;				
				if(penTarget != NULL)
				{
					if(penTarget->GetFlags()&ENF_ALIVE || penTarget->IsCharacter())//1211 죽은사람도 타겟팅이 가능하게 해야 한다.
					{
						FLOAT MaxHealth;
						FLOAT CurrentHealth;
						int	  Level;
						int	  mobIdx;	
						int	  PkMode = -1;
						int   PkState;
						int   PkLegit;
						int	  nSyndicateType = 0;
						int	  nSyndicateGrade = 0;
						bool  bRet = false;

						MaxHealth		= ((CUnit*)((CEntity*) penTarget))->m_nMaxiHealth;
						CurrentHealth	= ((CUnit*)((CEntity*) penTarget))->m_nCurrentHealth;
						Level			= ((CUnit*)((CEntity*) penTarget))->m_nMobLevel;
						mobIdx			= ((CUnit*)((CEntity*) penTarget))->m_nMobDBIndex;
						if (penTarget->en_pMobTarget != NULL)
						{
							nSyndicateType	= penTarget->en_pMobTarget->mob_iSyndicateType;
							nSyndicateGrade = penTarget->en_pMobTarget->mob_iSyndicateGrade;
						}

						if(penTarget->IsCharacter())
						{						
							Level			= 0;
							PkMode			= ((CCharacter*)((CEntity*) penTarget))->m_nPkMode;
							PkState			= ((CCharacter*)((CEntity*) penTarget))->m_nPkState;							
							PkLegit			= ((CCharacter*)((CEntity*) penTarget))->m_nLegit;

							if (penTarget->en_pCharacterTarget != NULL)
							{
								nSyndicateType  = penTarget->en_pCharacterTarget->cha_iSyndicateType;
								nSyndicateGrade = penTarget->en_pCharacterTarget->cha_iSyndicateGrade;
							}
						}

						if(PkMode != -1)
						{
							bRet = penTarget->SetTargetInfo(MaxHealth,CurrentHealth,FALSE,Level,PkMode,PkState,PkLegit);
						}
						else
						{
							bRet = penTarget->SetTargetInfo(MaxHealth,CurrentHealth,FALSE,Level,0,0,0,mobIdx);
														
						}

						if (bRet == true)
						{
							penTarget->SetTargetSyndicateInfo(nSyndicateType, nSyndicateGrade);
						}
					}
					else
					{
						INFO()->TargetClear(eTARGET);
						
						// 마우스 좌클릭 할 경우 무브 메시지를 서버에 새로 보내주기 위한 처리.
						// 무브 메시지가 1초에 한번씩 가기 때문에 동기화가 제대로 이루어 지지 않아 추가함.
						// 땅을 찍었을 경우에만 초기화 한다.  (몬스터 공격 시 문제 됨.)
						tmStartTime = 0;
					}
				}

//안태훈 수정 시작	//(5th Closed beta)(0.2)
			if(CEffectGroupManager::Instance().IsValidCreated(m_pPickingEffectGroup))
			{
				m_pPickingEffectGroup->Stop(PICKING_EFFECT_REMAIN_TIME);
			}
			else
			{
				m_pPickingEffectGroup = NULL;
			}
			if(GetPlayerWeapons()->m_penRayHitTmp != NULL)
			{
				if(GetPlayerWeapons()->m_penRayHitTmp->GetRenderType() == RT_TERRAIN)
				{
					m_pPickingEffectGroup = StartEffectGroup("Picking Terrain"		//Hardcording
						, _pTimer->GetLerpedCurrentTick(), m_vDesiredPosition, ANGLE3D(0,0,0));
				}
				else if( (GetPlayerWeapons()->m_penRayHitTmp->GetRenderType() == RT_BRUSH
					|| GetPlayerWeapons()->m_penRayHitTmp->GetRenderType() == RT_FIELDBRUSH)
					&& GetPlayerWeapons()->m_bPickConditionOk)
				{
					ANGLE3D angle = GetEulerAngleFromDir(GetPlayerWeapons()->m_vRayHitSurfaceNormal);
					// WSS_WALLMOVE_BUGFIX 070531-------------------------------------->>
					// 경사 45도 이상은 마우스 포인터도 나타나지 않게함
					if( angle(2) < MAX_MOVE_SLOPE_GRADE )
					{
						m_pPickingEffectGroup = StartEffectGroup("Picking BSP"		//Hardcording
						, _pTimer->GetLerpedCurrentTick(), m_vDesiredPosition, angle);
					}
					// ----------------------------------------------------------------<<
				}
			}
//안태훈 수정 끝	//(5th Closed beta)(0.2)

			_pInput->SetRMousePressed(FALSE);
					
			FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;
			FLOAT3D vDeltaTmp = vDelta;
			//vDeltaTmp(2) = 0.0f;
			FLOAT	fLength = vDeltaTmp.Length();

			if(GetPlayerWeapons()->m_penRayHitTmp!=NULL && GetPlayerWeapons()->m_penRayHitTmp->GetFlags()&ENF_ALIVE && m_nDesiredSkillNum !=-1)
			{
				//1231 사정거리안이라면 달려가지 않는다.
				CSkill &SkillData = _pNetwork->GetSkillData(m_nDesiredSkillNum);
				if( SkillData.GetFireRange() >= fLength )
				{
					return;
				}
			}

			///////////////////////////////////////////////////////////////////////
			// FIXME : 아래 코드 정리가 필요함!!!
			CEntity *penEnt = GetPlayerWeapons()->m_penRayHitTmp;
			
			if(penEnt != NULL && penEnt->GetFlags()&ENF_ALIVE && penEnt!=this)
			{
				if( penEnt->IsFirstExtraFlagOn(ENF_EX1_NPC) || 
					penEnt->IsFirstExtraFlagOn(ENF_EX1_PRODUCTION) || 
					penEnt->IsFirstExtraFlagOn(ENF_EX1_CURRENT_PET) || 
					penEnt->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE) ||
					penEnt->IsFirstExtraFlagOn(ENF_EX1_CURRENT_WILDPET))
				{
					return;
				}

				// PVP or RVR 일때... 캐릭터 공격 가능 체크?
				if(	_pNetwork->IsRvrZone() || IsPvp() || IsLegitTarget(penEnt) ||
					pUIManager->GetGuildBattle()->IsEnemy( penEnt->en_ulID ) || _pUISWDoc->IsWar() )
				{
					const int iJob = en_pcCharacter.pc_iPlayerType;

					// [100107: selo] MyCharacterInfo.attackrange 대치
					float fAttackDistance = GetAttackrange();					
					
					if(!IsAttacking() && m_nCurrentSkillNum!=-1)
					{	
						CSkill &SkillData = _pNetwork->GetSkillData(m_nCurrentSkillNum);

						m_fSkillDistance = SkillData.GetFireRange();

						if(fLength < m_fSkillDistance)
						{
							if(ulNewButtons&PLACT_FIRE && (m_bForward||m_bKeyMove))
							{
								m_bForward = FALSE;
								m_bKeyMove = FALSE;
							}			
							StopMove();	
							
							if(!IsVisibleCheckAll(penEnt))
							{
								CTString strSysMessage;
								strSysMessage.PrintF(_S( 2085, "가로막혀서 스킬을 사용할수 없습니다." ));	// 번역
								_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);	
								GetPlayerWeapons()->m_penReservedTarget = GetPlayerWeapons()->m_penRayHitTmp;
								GetPlayerWeapons()->m_penRayHitTmp = NULL;	
								
								m_nCurrentSkillNum = -1;
								return;
							}				
						}
						else
						{
							g_bIsRotateEnd = FALSE;							
							g_bFirstRotate = TRUE;
							g_bTargetDirection = vDelta;
							if( BUTTON_MOVE_ENABLE ) { SendMyNextMovePosition(vDelta); }
						}																															
					}
					else if(!IsAttacking() && m_nDesiredSkillNum!=-1)
					{	
						CSkill &SkillData = _pNetwork->GetSkillData(m_nDesiredSkillNum);
						m_fSkillDistance = SkillData.GetFireRange();

						if(fLength < m_fSkillDistance)
						{
							if(ulNewButtons&PLACT_FIRE && (m_bForward||m_bKeyMove))
							{
								m_bForward = FALSE;
								m_bKeyMove = FALSE;
							}			
							StopMove();		
							if(!IsVisibleCheckAll(penEnt))
							{
								CTString strSysMessage;
								strSysMessage.PrintF(_S( 2085, "가로막혀서 스킬을 사용할수 없습니다." ));	// 번역
								_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);	
								GetPlayerWeapons()->m_penReservedTarget = GetPlayerWeapons()->m_penRayHitTmp;
								GetPlayerWeapons()->m_penRayHitTmp = NULL;	
								
								m_nDesiredSkillNum = -1;
								return;
							}				
						}
						else
						{
							g_bIsRotateEnd = FALSE;							
							g_bFirstRotate = TRUE;
							g_bTargetDirection = vDelta;
							SendMyNextMovePosition(vDelta);
						}																															
					}
					else if( fLength < fAttackDistance && !IsAttacking() && m_nCurrentSkillNum == -1)
					{	
						if(ulNewButtons&PLACT_FIRE && (m_bForward||m_bKeyMove))
						{
							m_bForward = FALSE;
							m_bKeyMove = FALSE;
						}			
						StopMove();	
						if(!IsVisibleCheckAll(penEnt))
						{
							CTString strSysMessage;
							strSysMessage.PrintF(_S( 2086, "가로막혀서 공격할수 없습니다." ));	// 번역
							_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);	

							GetPlayerWeapons()->m_penReservedTarget = GetPlayerWeapons()->m_penRayHitTmp;
							GetPlayerWeapons()->m_penRayHitTmp = NULL;	

							return;
						}
						else
						{						
							BOOL bAttack = CheckAttackTarget( -1, penEnt, fLength );
							if( bAttack )
							{
								StartAttack();
							}
							else
							{
								StopAttack();
							}

						}
					}
///////////////
					else if(fLength >= 2.0f && !IsAttacking())
					{
						g_bIsRotateEnd = FALSE;							
						g_bFirstRotate = TRUE;
						g_bTargetDirection = vDelta;
						if( BUTTON_MOVE_ENABLE ) { SendMyNextMovePosition(vDelta); }
					}
				}
				else //pvp가 아니라면,
				{
					if( penEnt->IsCharacter() )
					{
						if(fLength >= 1.0f)
						{
							g_bIsRotateEnd = FALSE;							
							g_bFirstRotate = TRUE;
							g_bTargetDirection = vDelta;
							if( BUTTON_MOVE_ENABLE ) { SendMyNextMovePosition(vDelta); }
						}
					}
					else if(penEnt == this)		// Self?
					{

					}
					else if(penEnt->IsEnemy() || penEnt->IsPet() || penEnt->IsSlave())//몬스터라면,
					{
						const int iJob = en_pcCharacter.pc_iPlayerType;
						FLOAT fScaledSize = 0.0f;
						FLOAT fAttackDistance = _pNetwork->MyCharacterInfo.attackrange;

						if (penEnt->IsEnemy())
						{
							fScaledSize = CMobData::getData( ((CEnemy*)penEnt)->m_nMobDBIndex )->GetScaledSize();
							fAttackDistance += fScaledSize;
						}

						// 공격중이 아니고, 현재 사용하는 스킬이 있을때?
						if(!IsAttacking() && m_nCurrentSkillNum!=-1)
						{	
							CSkill &SkillData = _pNetwork->GetSkillData(m_nCurrentSkillNum);

							m_fSkillDistance = SkillData.GetFireRange();

							if(fLength < m_fSkillDistance)
							{
								if(ulNewButtons&PLACT_FIRE && (m_bForward||m_bKeyMove))
								{
									m_bForward = FALSE;
									m_bKeyMove = FALSE;
								}			
								StopMove();	

								if(!IsVisibleCheckAll(penEnt))
								{
									CTString strSysMessage;
									strSysMessage.PrintF(_S( 2085, "가로막혀서 스킬을 사용할수 없습니다." ));	// 번역
									_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);	
									GetPlayerWeapons()->m_penReservedTarget = GetPlayerWeapons()->m_penRayHitTmp;
									GetPlayerWeapons()->m_penRayHitTmp = NULL;	
									
									m_nCurrentSkillNum = -1;
									return;
								}		
												
							}
							else
							{
								g_bIsRotateEnd = FALSE;							
								g_bFirstRotate = TRUE;
								g_bTargetDirection = vDelta;
								if( BUTTON_MOVE_ENABLE ) { SendMyNextMovePosition(vDelta); }
							}																															
						}
						// 공격중이 아니고, 스킬 공격을 해야할때...
						else if(!IsAttacking() && m_nDesiredSkillNum!=-1)
						{	
							CSkill &SkillData = _pNetwork->GetSkillData(m_nDesiredSkillNum);

							m_fSkillDistance = SkillData.GetFireRange();

							if(fLength < m_fSkillDistance)
							{
								if(ulNewButtons&PLACT_FIRE && (m_bForward||m_bKeyMove))
								{
									m_bForward = FALSE;
									m_bKeyMove = FALSE;
								}			
								StopMove();	
								if(!IsVisibleCheckAll(penEnt))
								{
									CTString strSysMessage;
									strSysMessage.PrintF(_S( 2085, "가로막혀서 스킬을 사용할수 없습니다." ));	// 번역
									_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);	
									GetPlayerWeapons()->m_penReservedTarget = GetPlayerWeapons()->m_penRayHitTmp;
									GetPlayerWeapons()->m_penRayHitTmp = NULL;	
									
									m_nDesiredSkillNum = -1;
									return;
								}						
							}
							else
							{
								g_bIsRotateEnd = FALSE;							
								g_bFirstRotate = TRUE;
								g_bTargetDirection = vDelta;
								if( BUTTON_MOVE_ENABLE ) { SendMyNextMovePosition(vDelta); }
							}																															
						}
						// 공격중이 아니고, 일반 공격을 할수 있는 조건일때...
						// FIXME : 코드 중복이 심함.
						else if( fLength < fAttackDistance && !IsAttacking() && m_nCurrentSkillNum==-1)
						{	
							if(ulNewButtons&PLACT_FIRE && (m_bForward||m_bKeyMove))
							{
								m_bForward = FALSE;
								m_bKeyMove = FALSE;
							}			
							StopMove();		
							
							if(!IsVisibleCheckAll(penEnt))
							{
								CTString strSysMessage;
								strSysMessage.PrintF(_S( 2086, "가로막혀서 공격할수 없습니다." ));	// 번역
								_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);	

								GetPlayerWeapons()->m_penReservedTarget = GetPlayerWeapons()->m_penRayHitTmp;
								GetPlayerWeapons()->m_penRayHitTmp = NULL;	

								return;
							}						
							else
							{
								BOOL bAttack = CheckAttackTarget( -1, penEnt, fLength );
								if(bAttack)
								{
									StartAttack();
								}
								else
								{
									StopAttack();
								}								
							}
						}
						// 공격중이 아니고, 범위밖에 있을때...
						else if(fLength >= 2.0f && !IsAttacking())
						{
							g_bIsRotateEnd = FALSE;							
							g_bFirstRotate = TRUE;
							g_bTargetDirection = vDelta;
							if( BUTTON_MOVE_ENABLE ) { SendMyNextMovePosition(vDelta); }
						}
					}					
				}
			}
			else if(fLength >= 0.5f )
			{	
				//0625 kwon
				g_bIsRotateEnd = FALSE;							
				g_bFirstRotate = TRUE;
				g_bTargetDirection = vDelta;//이동은 멈춰도 회전은 이방향 끝까지 해야한다.
				
				// Date : 2005-09-16(오전 9:43:24), By Lee Ki-hwan
				// 에니메이션 할 때 이동메세지를 보내기때문에 이곳에서 이동메세지를 보낼 필요가 없음 
				// SendMyNextMovePosition(vDelta);
			}
		}
//0611 kwon 추가.		
		else if((ulNewButtons&PLACT_FIRE || m_bKeyMove) && m_bLockMove)
		{			
			CPlacement3D plRay;					

			// WSS_WALLMOVE_BUGFIX 070612
			m_bReserveMove = TRUE;
			plRay = GetPlayerWeapons()->GetMouseHitInformationReserve( _pInput->inp_ptMousePos, m_apr, TRUE, TRUE );
		}
		///////////////////////////////////////////////////////////////////////
		else if (IsAttacking())//0701 kwon
		{
			m_bForward = FALSE;//0114
			m_bKeyMove = FALSE;

			StopMove();		
		}
	
		if (m_iViewState == PVT_PLAYEREYES) {
			// spawn 3rd person view camera
			//ASSERT(m_pen3rdPersonView == NULL);
			if (m_pen3rdPersonView == NULL) {
				m_pen3rdPersonView = CreateEntity(GetPlacement(), CLASS_PLAYER_VIEW,WLD_AUTO_ENTITY_ID,FALSE);
				EViewInit eInit;
				eInit.eidOwner = this;
				eInit.eidCamera = (CEntity*) NULL;
				eInit.vtView = VT_3RDPERSONVIEW;
				eInit.bDeathFixed = FALSE;
				m_pen3rdPersonView ->Initialize(eInit,FALSE);
			}
			
			m_iViewState = PVT_3RDPERSONVIEW;
			
			// change from 3rd person to eyes
		}

		// apply center view
		if( ulButtonsNow&PLACT_CENTER_VIEW) {
			// center view with speed of 45 degrees per 1/20 seconds
			paAction.pa_aRotation(2) += Clamp( -en_plViewpoint.pl_OrientationAngle(2)/_pTimer->TickQuantum, -900.0f, +900.0f);
		}
	};
	
	/*
	void ApplySniperZoom( BOOL bZoomIn )
	{
		// do nothing if not holding sniper and if not in sniping mode
		if (((CPlayerWeapons&)*m_penWeapons).m_iCurrentWeapon!=WEAPON_SNIPER ||
			!((CPlayerWeapons&)*m_penWeapons).m_bSniping) {
			return;
		}
		BOOL bZoomChanged;
		if (((CPlayerWeapons&)*m_penWeapons).SniperZoomDiscrete(bZoomIn, bZoomChanged)) {
			if (bZoomChanged) { 
				//PlaySound(m_soSniperZoom, SOUND_SNIPER_QZOOM, SOF_3D|SOF_VOLUMETRIC); 
			}
			m_ulFlags|=PLF_ISZOOMING;
		}
		else
		{
			m_ulFlags&=~PLF_ISZOOMING;
			m_soSniperZoom.Stop();
			if(_pNetwork->IsPlayerLocal(this)) {IFeel_StopEffect("SniperZoom");}
		}
	}
	*/
	
	// check if cheats can be active
	BOOL CheatsEnabled(void)
	{
		return (GetSP()->sp_ctMaxPlayers==1||GetSP()->sp_bQuickTest) && m_penActionMarker==NULL && !_SE_DEMO;
	}

	
	/************************************************************
	*                 END OF PLAYER ACTIONS                    *
	************************************************************/
	// Get current placement that the player views from in absolute space.
	// 플레이어 뷰의 위치를 얻음.
	void GetLerpedAbsoluteViewPlacement(CPlacement3D &plView) 
	{
		if (!(m_ulFlags&PLF_INITIALIZED)) 
		{
			plView = GetPlacement();
			_bDiscard3rdView=FALSE;
			return;
		}

		BOOL bSharpTurning = TRUE;
		if ((!_pNetwork->IsPlayerLocal(this) && !_pNetwork->IsPlayingDemo()) || _pNetwork->IsPlayingDemo() || _pNetwork->IsGameFinished() )
		{
			bSharpTurning = FALSE;
		}

		// lerp player viewpoint
		FLOAT fLerpFactor = _pTimer->GetLerpFactor();
		
		if(m_bDontLerpView)
		{
			en_plLastViewpoint=en_plViewpoint;
			plView=en_plViewpoint;
			m_bDontLerpView=FALSE;
		}
		else
		{
			plView.Lerp(en_plLastViewpoint, en_plViewpoint, fLerpFactor);
		}
		
		FLOAT fOriginalPitch = plView.pl_OrientationAngle(2);
		
		// moving banking and soft eyes
		((CPlayerAnimator&)*m_penAnimator).ChangeView(plView);
		
		// body and head attachment animation
		
		// return player eyes view
		if (m_iViewState == PVT_PLAYEREYES || _bDiscard3rdView) 
		{
			CPlacement3D plPosLerped = GetLerpedPlacement();
			
			if (bSharpTurning) 
			{
				FLOAT fRotationDamping = CalculateViewRotationDamping();
				
				plView.pl_OrientationAngle(1) = en_plViewpoint.pl_OrientationAngle(1) + GetRotationDiff(m_iLastRotationH, m_aLocalRotation(1))*fRotationDamping; 
				plView.pl_OrientationAngle(2) = en_plViewpoint.pl_OrientationAngle(2) + GetRotationDiff(m_iLastRotationP, m_aLocalRotation(2))*fRotationDamping;
				plView.pl_OrientationAngle(3) = en_plViewpoint.pl_OrientationAngle(3) + GetRotationDiff(m_iLastRotationB, m_aLocalRotation(3))*fRotationDamping;
				
				// make sure it doesn't go out of limits
				RoundViewAngle(plView.pl_OrientationAngle(2), PITCH_MAX);
				RoundViewAngle(plView.pl_OrientationAngle(3), BANKING_MAX);
				
				// compensate for rotations that happen to the player without his/hers will
				// (rotating brushes, weird gravities...)
				// (these need to be lerped)
				ANGLE3D aCurr = GetPlacement().pl_OrientationAngle;
				ANGLE3D aLast = en_plLastPlacement.pl_OrientationAngle;
				ANGLE3D aDesired = en_aDesiredRotationRelative*_pTimer->TickQuantum;
				FLOATmatrix3D mCurr;      MakeRotationMatrixFast(mCurr, aCurr);
				FLOATmatrix3D mLast;      MakeRotationMatrixFast(mLast, aLast);
				FLOATmatrix3D mDesired;   MakeRotationMatrixFast(mDesired, aDesired);
				mDesired = en_mRotation*(mDesired*!en_mRotation);
				FLOATmatrix3D mForced = !mDesired*mCurr*!mLast; // = aCurr-aLast-aDesired;
				ANGLE3D aForced; DecomposeRotationMatrixNoSnap(aForced, mForced);
				if (aForced.MaxNorm()<1E-2) {
					aForced = ANGLE3D(0,0,0);
				}
				FLOATquat3D qForced; qForced.FromEuler(aForced);
				FLOATquat3D qZero;   qZero.FromEuler(ANGLE3D(0,0,0));
				FLOATquat3D qLerped = Slerp(fLerpFactor, qZero, qForced);
				FLOATmatrix3D m;
				qLerped.ToMatrix(m);
				m=m*mDesired*mLast;
				DecomposeRotationMatrixNoSnap(plPosLerped.pl_OrientationAngle, m);      
			}
			
			plView.RelativeToAbsoluteSmooth(plPosLerped);
			
			// 3rd person view
		} 
		else if (m_iViewState == PVT_3RDPERSONVIEW) 
		{
			plView = m_pen3rdPersonView->GetLerpedPlacement();
			// camera view for player auto actions
		} 
		else if (m_iViewState == PVT_PLAYERAUTOVIEW) 
		{
			plView = CPlacement3D(FLOAT3D(0,plr_fViewHeightStand,0), ANGLE3D(180,-90,0));
			plView.RelativeToAbsoluteSmooth(GetLerpedPlacement());
			// camera view for stored sequences
		}
		else 
		{
			ASSERTALWAYS("Unknown player view");
		}
		_bDiscard3rdView=FALSE;
	};
	
	// Get current entity that the player views from.
	CEntity *GetViewEntity(BOOL bCamera) 
	{
		// player eyes
		if (m_iViewState == PVT_PLAYEREYES) 
		{
			return this;
			// 3rd person view
		} 
		// 3인칭 뷰.
		else if (m_iViewState == PVT_3RDPERSONVIEW) 
		{
			if (m_ulFlags&PLF_ISZOOMING) {
				return this;
			}
			//0105 1line 삭제
			//if (((CPlayerView&)*m_pen3rdPersonView).m_fDistance>2.0f) {
			//0105
			if (((CPlayerView&)*m_pen3rdPersonView).m_fDistance>0.5f && !bCamera) {//시점변화 넘어가지 않게..
				//..
				return m_pen3rdPersonView;
			} else {
				return this;
			}
			// camera
		} 
		else if (m_iViewState == PVT_PLAYERAUTOVIEW) 
		{
			return this;
			// invalid view
		} 
		else 
		{
			ASSERTALWAYS("Unknown player view");
			return NULL;
		}
	};
	
	void RenderChainsawParticles(BOOL bThird)
	{
		FLOAT fStretch=1.0f;
		if( bThird)
		{
			fStretch=0.4f;
		}
		// render chainsaw cutting brush particles
		FLOAT tmNow = _pTimer->GetLerpedCurrentTick();
		for( INDEX iSpray=0; iSpray<MAX_BULLET_SPRAYS; iSpray++)
		{
			BulletSprayLaunchData &bsld = m_absldData[iSpray];
			FLOAT fLife=1.25f;
			if( tmNow > (bsld.bsld_tmLaunch+fLife)) { continue;}
			Particles_BulletSpray(bsld.bsld_iRndBase, bsld.bsld_vPos, bsld.bsld_vG,
				bsld.bsld_eptType, bsld.bsld_tmLaunch, bsld.bsld_vStretch*fStretch, 1.0f);
		}
		
		// render chainsaw cutting model particles
		for( INDEX iGore=0; iGore<MAX_GORE_SPRAYS; iGore++)
		{
			GoreSprayLaunchData &gsld = m_agsldData[iGore];
			FLOAT fLife=2.0f;
			if( tmNow > (gsld.gsld_tmLaunch+fLife)) { continue;}
			FLOAT3D vPos=gsld.gsld_vPos;
			if( bThird)
			{
				vPos=gsld.gsld_v3rdPos;
			}
			Particles_BloodSpray(gsld.gsld_sptType, vPos, gsld.gsld_vG, gsld.gsld_fGA,
				gsld.gsld_boxHitted, gsld.gsld_vSpilDirection,
				gsld.gsld_tmLaunch, gsld.gsld_fDamagePower*fStretch, gsld.gsld_colParticles);
		}
	}
	
	// Draw player interface on screen.
	void RenderHUD( CPerspectiveProjection3D &prProjection, CDrawPort *pdp,
		FLOAT3D vViewerLightDirection, COLOR colViewerLight, COLOR colViewerAmbient,
		BOOL bRenderWeapon, INDEX iEye)
	{
		CPlacement3D plViewOld = prProjection.ViewerPlacementR();
		//BOOL bSniping = ((CPlayerWeapons&)*m_penWeapons).m_bSniping;
		// render weapon models if needed
		// do not render weapon if sniping
		BOOL bRenderModels = _pShell->GetINDEX("gfx_bRenderModels");
		//0105 4line 삭제. 무기 그리지 않기.
		/*
		if( hud_bShowWeapon && bRenderModels && !bSniping) {
		// render weapons only if view is from player eyes
		((CPlayerWeapons&)*m_penWeapons).RenderWeaponModel( prProjection, pdp, 
		vViewerLightDirection, colViewerLight, colViewerAmbient, bRenderWeapon, iEye);
		}
		*/
		// if is first person
		if (m_iViewState == PVT_PLAYEREYES) {
			prProjection.ViewerPlacementL() = plViewOld;
			prProjection.Prepare();
			CAnyProjection3D apr;
			apr = prProjection;
			Stereo_AdjustProjection( *apr, iEye, 1);
			Particle_PrepareSystem( pdp, apr);
			Particle_PrepareEntity( 2.0f, FALSE, FALSE, TRUE, this);
			RenderChainsawParticles(FALSE);
			Particle_EndSystem();
		}
		
		// render crosshair if sniper zoom not active and not dead
		CPlacement3D plView;
		if (m_iViewState == PVT_PLAYEREYES) 
		{
			// player view
			plView = en_plViewpoint;
			plView.RelativeToAbsolute(GetPlacement());
		} 
		else if (m_iViewState == PVT_3RDPERSONVIEW) 
		{
			// camera view
			plView = ((CPlayerView&)*m_pen3rdPersonView).GetPlacement();
		}		
		// do screen blending
		ULONG ulR=255, ulG=0, ulB=0; // red for wounding
		ULONG ulA = m_fDamageAmmount*5.0f;
		
		// if less than few seconds elapsed since last damage
		FLOAT tmSinceWounding = en_tmEntityTime - m_tmWoundedTime;
		if( tmSinceWounding<4.0f && !IsReapplyingActions()) 
		{
			// decrease damage ammount
			if( tmSinceWounding<0.001f) { ulA = (ulA+64)/2; }
		}
		
		// add rest of blend ammount
		ulA = ClampUp( ulA, (ULONG)224);
		
		//0401
		static int cnt=0;
		/*
		if(_pInput->IsUKeyPressed()){
			
			cnt++;
			if(cnt==5){
				cnt=0;
			}		
			_pInput->SetUKeyPressed(FALSE);
		}

		if(_pInput->IsUKeyPressed()){  
				   CPlayerSource* ppls = _pNetwork->GetPlayerSource(this);
				   en_pcCharacter.pc_iPlayerType = WORM_AND_MAGE;
				   ppls->ChangeCharacter(en_pcCharacter);					   				   

			_pInput->SetUKeyPressed(FALSE);
		}
		*/
		if(cnt==4){
			ulR=255;
			ulG=0;
			ulB=0;
			ulA = 64;
		}
		else if(cnt==1){
			ulR=0;
			ulG=255;
			ulB=0;
			ulA = 64;
		}
		else if(cnt==2){
			ulR=0;
			ulG=0;
			ulB=255;
			ulA = 64;
		}
		else if(cnt==3){
			ulR=255;
			ulG=200;
			ulB=0;
			ulA = 64;
		}
		else if(cnt==0){
			ulR=255;
			ulG=0;
			ulB=0;
			ulA = 0;
		}
		
		//if (m_iViewState == PVT_PLAYEREYES) { //0401
		pdp->dp_ulBlendingRA += ulR*ulA;
		pdp->dp_ulBlendingGA += ulG*ulA;
		pdp->dp_ulBlendingBA += ulB*ulA;
		pdp->dp_ulBlendingA  += ulA;
		//}
		
		// add world glaring
		{
			COLOR colGlare = GetWorldGlaring();
			UBYTE ubR, ubG, ubB, ubA;
			ColorToRGBA(colGlare, ubR, ubG, ubB, ubA);
			if (ubA!=0) {
				pdp->dp_ulBlendingRA += ULONG(ubR)*ULONG(ubA);
				pdp->dp_ulBlendingGA += ULONG(ubG)*ULONG(ubA);
				pdp->dp_ulBlendingBA += ULONG(ubB)*ULONG(ubA);
				pdp->dp_ulBlendingA  += ULONG(ubA);
			}
		}
		
		// do all queued screen blendings
		pdp->BlendScreen();
		
		// render status info line (if needed)
		if( hud_bShowInfo) { 
			BOOL bSnooping = FALSE;
			CPlayer *penHUDPlayer = this;
			CPlayer *penHUDOwner  = this;
			
			// check if snooping is needed
			/*
			CPlayerWeapons *pen = (CPlayerWeapons*)&*penHUDPlayer->m_penWeapons;
			TIME tmDelta = _pTimer->CurrentTick() - pen->m_tmSnoopingStarted;
			if( tmDelta<plr_tmSnoopingTime) {
			ASSERT( pen->m_penTargeting!=NULL);
			penHUDPlayer = (CPlayer*)&*pen->m_penTargeting;
			bSnooping = TRUE;
			}
			*/
			//0205 인터페이스 안그리기.->다시 그리기.
			DrawHUD( penHUDPlayer, pdp, bSnooping, penHUDOwner);
		}
	}
	//0609 kwon 함수 추가.
	void RenderCursor(CDrawPort *pdp, void *pvUserData)
	{
		BOOL bShowExtras = (ULONG(pvUserData)&GRV_SHOWEXTRAS);	
	
		CAnyProjection3D apr;
		CEntity *penViewer;
		CPlacement3D plViewer;
		COLOR colBlend;
	
		// back to center view
		CDrawPort dpCenter( pdp, 0);
		dpCenter.SetAsCurrent();
		SetupView( &dpCenter, apr, penViewer, plViewer, colBlend, FALSE);
		Stereo_AdjustProjection( *apr, 0, 1);
			
		//RenderScroll(  &dpCenter);
		//RenderTextFX(  &dpCenter);
		//RenderCredits( &dpCenter);
		RenderHudPicFX(&dpCenter);
			
		if( hud_bShowAll && bShowExtras) 
		{
			CPlacement3D plView;
			if (m_iViewState == PVT_PLAYEREYES) 
			{
				// player view
				plView = en_plViewpoint;
				plView.RelativeToAbsolute(GetPlacement());
			}
			else if (m_iViewState == PVT_3RDPERSONVIEW) 
			{
				// camera view
				plView = ((CPlayerView&)*m_pen3rdPersonView).GetPlacement();
			}

			if (IsFlagOn(ENF_ALIVE)) 
			{
				if(!_pInput->IsRMouseButtonPressed())//0615 kwon 마우스 오른쪽 버튼으로 바꾸기.
				{//1211 휠버튼 DOWN이 아닐때 그려준다.							
				    CPerspectiveProjection3D &prProjection = *(CPerspectiveProjection3D*)(CProjection3D*)apr;
					((CPlayerWeapons&)*m_penWeapons).RenderCrosshair(prProjection, &dpCenter, plView);					
				}			
			}
		}
		// pull out
		pdp->SetAsCurrent();
	}
	
	/************************************************************
	*                  INITIALIZE PLAYER                       *
	************************************************************/
	
	void InitializePlayer( BOOL bFirstInit )
	{
		// set viewpoint position inside the entity
		en_plViewpoint.pl_OrientationAngle = ANGLE3D(0,0,0);
		en_plViewpoint.pl_PositionVector = FLOAT3D(0.0f, plr_fViewHeightStand, 0.0f);
		en_plLastViewpoint = en_plViewpoint;
		
		// clear properties
		m_ulFlags &= PLF_INITIALIZED|PLF_LEVELSTARTED|PLF_RESPAWNINPLACE;  // must not clear initialized flag
		m_fFallTime = 0.0f;
		m_pstState = PST_STAND;
		m_fDamageAmmount = 0.0f;
		m_tmWoundedTime  = 0.0f;
		m_tmInvisibility    = 0.0f, 
		m_tmInvulnerability = 0.0f, 
		m_tmSeriousDamage   = 0.0f, 
		m_tmSeriousSpeed    = 0.0f, 

		m_bIsTransform		= FALSE;
		m_bTransforming		= FALSE;
		m_iTransformType	= 0;

		m_nChangeMonsterId = 0;
		m_bChanging			= FALSE;
		m_bMobChange		= FALSE;
			
		// initialize animator
		((CPlayerAnimator&)*m_penAnimator).InitializeProperties();
		// restart weapons if needed
		GetPlayerWeapons()->SendEvent(EStart());
		
		// initialise last positions for particles
		Particles_AfterBurner_Prepare(this);
		
		// set flags
		SetPhysicsFlags(EPF_MODEL_WALKING|EPF_HASLUNGS|/*EPF_RT_SYNCHRONIZED*/EPF_PUSHABLE);
		SetCollisionFlags(ECF_MODEL|((ECBI_PLAYER)<<ECB_IS));
		SetFlags(GetFlags()|ENF_ALIVE);
		// animation
		//0105
		if(m_bMdl){
			StartModelAnim(PLAYER_ANIM_STAND, AOF_LOOPING);
		}else{
			StartSkaAnim(this, idPlayerWhole_Animation[ANIM_IDLE], AN_LOOPING, 0,0.5f);
		}
		//..
		PlayerTeleportCause eCause;
//0530 kwon 수정.
		if(!_cmiComm. IsNetworkOn()){
			//PlayerTeleportCause eCause;
			if (bFirstInit) {
				eCause = TPC_FIRSTINIT;
			} else {
				eCause = TPC_OTHER;
			}
			if(eCause == TPC_FIRSTINIT)
			{
				CSkaTag tag;
				tag.SetName("__ROOT");
				tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
				if(en_pmiModelInstance) { en_pmiModelInstance->m_tmSkaTagManager.Register(&tag); }
				tag.SetName("__TOP");
				tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
				FLOATaabbox3D aabb;
				GetModelInstance()->GetAllFramesBBox(aabb);
				tag.SetOffsetPos(0, aabb.Size()(2) * GetModelInstance()->mi_vStretch(2), 0);
				GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			}
			TeleportPlayer(WLT_FIXED, eCause);
		}
		else if(_cmiComm. IsNetworkOn()){//0815
			//PlayerTeleportCause eCause;
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
			if (bFirstInit) {
				eCause = TPC_FIRSTINIT;
			} else {
				eCause = TPC_OTHER;
			}
			if(eCause == TPC_FIRSTINIT)
			{
				if(GetModelInstance())
				{
					CSkaTag tag;
					tag.SetName("__ROOT");
					tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
					en_pmiModelInstance->m_tmSkaTagManager.Register(&tag);
					tag.SetName("__TOP");
					tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
					FLOATaabbox3D aabb;
					GetModelInstance()->GetAllFramesBBox(aabb);
					tag.SetOffsetPos(0, aabb.Size()(2) * GetModelInstance()->mi_vStretch(2), 0);
					GetModelInstance()->m_tmSkaTagManager.Register(&tag);
				}
			}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)	
		}

		//0610 kwon 추가
		if(!bFirstInit)//죽었을때...리젠.
		{
			GoTo( _pNetwork->MyCharacterInfo.x, _pNetwork->MyCharacterInfo.h, _pNetwork->MyCharacterInfo.z,
					_pNetwork->MyCharacterInfo.r );
			m_vDesiredPosition = GetPlacement().pl_PositionVector;
		}
	};

	FLOAT3D GetTeleportingOffset(void)
	{
		// find player index
		INDEX iPlayer = GetMyPlayerIndex();
		
		// create offset from marker
		const FLOAT fOffsetY = 0.1f;  // how much to offset up (as precaution not to spawn in floor)
		FLOAT3D vOffsetRel = FLOAT3D(0,fOffsetY,0);
		if (GetSP()->sp_bCooperative && !GetSP()->sp_bSinglePlayer) {
			INDEX iRow = iPlayer/4;
			INDEX iCol = iPlayer%4;
			vOffsetRel = FLOAT3D(-3.0f+iCol*2.0f, fOffsetY, -3.0f+iRow*2.0f);
		}
		
		return vOffsetRel;
	}

	void RemapLevelNames(INDEX &iLevel)
	{
		switch(iLevel) {
		case 10:
			iLevel = 1;
			break;
		case 11:
			iLevel = 2;
			break;
		case 12:
			iLevel = 3;
			break;
		case 13:
			iLevel = 4;
			break;
		case 14:
			iLevel = 5;
			break;
		case 15:
			iLevel = 6;
			break;
		case 21:
			iLevel = 7;
			break;
		case 22:
			iLevel = 8;
			break;
		case 23:
			iLevel = 9;
			break;
		case 24:
			iLevel = 10;
			break;
		case 31:
			iLevel = 11;
			break;
		case 32:
			iLevel = 12;
			break;
		case 33:
			iLevel = 13;
			break;
		default:
			iLevel = -1;
			break;
		}
	}
	
	void TeleportPlayer(enum WorldLinkType EwltType,PlayerTeleportCause eCause) 
	{
		INDEX iLevel = -1;
		CTString strLevelName = GetWorld()->wo_fnmFileName.FileName();
		
		//strLevelName.ScanF("%02d_", &iLevel);
		INDEX u, v;
		u = v = -1;
		strLevelName.ScanF("%01d_%01d_", &u, &v);
		iLevel = u*10+v;
		
		RemapLevelNames(iLevel);
		
		if (iLevel>0) {
			((CSessionProperties*)GetSP())->sp_ulLevelsMask|=1<<(iLevel-1);
		}
		
		if (_pNetwork->IsServer()) {
			m_ulFlags&=~PLF_DONTRENDER;
		}
		
		// find player index
		INDEX iPlayer = GetMyPlayerIndex();
		// player placement
		CPlacement3D plSet = GetPlacement();
		// teleport in dummy space to avoid auto teleport frag
		Teleport(CPlacement3D(FLOAT3D(32000.0f+100.0f*iPlayer, 32000.0f, 0), ANGLE3D(0, 0, 0)));//0315 기본 텔레포트
		// force yourself to standing state
		//0105
		if(m_bMdl){
			ForceCollisionBoxIndexChange(PLAYER_COLLISION_BOX_STAND);
		}else{		
			ForceCollisionBoxIndexChange(GetModelInstance()->GetColisionBoxIndex(idPlayer_Collision_Stand));
		}
		//..   
		en_plViewpoint.pl_PositionVector(2) = plr_fViewHeightStand;
		((CPlayerAnimator&)*m_penAnimator).m_bDisableAnimating = FALSE;
		((CPlayerAnimator&)*m_penAnimator).Stand();
		m_pstState = PST_STAND;
		
		// this will be set to TRUE only if we just started the level, and the first
		// automatic profile save was executed; it will also cause initialization of
		// weapons from the first marker
		BOOL bInitialMarker = FALSE;
		
		// create offset from marker
		FLOAT3D vOffsetRel = GetTeleportingOffset();
		
		// no player start initially
		BOOL bSetHealth = FALSE;      // for getting health from marker
		BOOL bAdjustHealth = FALSE;   // for getting adjusting health to 50-100 interval
		CEntity *pen = NULL;
		if (GetSP()->sp_bCooperative) 
		{
			if (cht_iGoToMarker>=0) {
				// try to find fast go marker
				CTString strPlayerStart;
				strPlayerStart.PrintF("Player Start - %d", (INDEX)cht_iGoToMarker);
				pen = _pNetwork->GetEntityWithName(strPlayerStart, 0);
				ETrigger etr;
				etr.penCaused = this;
				pen->SendEvent(etr);
				cht_iGoToMarker = -1;
				bSetHealth = TRUE;
				bAdjustHealth = FALSE;
				// if there is coop respawn marker
			} else if (m_penMainMusicHolder!=NULL && !(m_ulFlags&PLF_CHANGINGLEVEL)) {
				CMusicHolder *pmh = (CMusicHolder *)&*m_penMainMusicHolder;
				
				CEntity* penRespawnMarker;
				_pNetwork->ga_World.EntityExists(pmh->m_iRespawnMarker,penRespawnMarker);
				if (penRespawnMarker!=NULL) {
					// get it
					pen = penRespawnMarker;
					bSetHealth = TRUE;
					bAdjustHealth = FALSE;
				}
			}
			
			// if quick start is enabled (in wed)
			if (pen==NULL && GetSP()->sp_bQuickTest && m_strGroup=="") {
				// try to find quick start marker
				CTString strPlayerStart;
				strPlayerStart.PrintF("Player Quick Start");
				pen = _pNetwork->GetEntityWithName(strPlayerStart, 0);
				bSetHealth = TRUE;
				bAdjustHealth = FALSE;
			}
			// if no start position yet
			if (pen==NULL) {
				// try to find normal start marker
				CTString strPlayerStart;
				strPlayerStart.PrintF("Player Start - %s", m_strGroup);
				pen = _pNetwork->GetEntityWithName(strPlayerStart, 0);
				if (m_strGroup=="") {
					bSetHealth = TRUE;
					bAdjustHealth = FALSE;
				} else {
					if (EwltType==WLT_FIXED) {
						bSetHealth = FALSE;
						bAdjustHealth = TRUE;
					} else {
						bSetHealth = FALSE;
						bAdjustHealth = FALSE;
					}
				}
			}
			// if no start position yet
			if (pen==NULL) {
				// try to find normal start marker without group anyway
				CTString strPlayerStart;
				strPlayerStart.PrintF("Player Start - ");
				pen = _pNetwork->GetEntityWithName(strPlayerStart, 0);
				bSetHealth = TRUE;
				bAdjustHealth = FALSE;
			}
			// see if we are teleporting to the first marker in the world
			if (pen!=NULL && eCause!=TPC_OTHER) {
				bInitialMarker = TRUE;
			}
		} 		
		
		// if respawning in place
		if ((m_ulFlags&PLF_RESPAWNINPLACE) && pen!=NULL && !((CPlayerMarker*)&*pen)->m_bNoRespawnInPlace) {
			m_ulFlags &= ~PLF_RESPAWNINPLACE;
			// set default params
			SetHealth(TopHealth());
//			m_iMana  = GetSP()->sp_iInitialMana;
			m_fArmor = 0.0f;
			// teleport where you were when you were killed
			Teleport(CPlacement3D(m_vDied, m_aDied));
			
			// if start marker is found
		} else if (pen!=NULL) {
			// if there is no respawn marker yet
			if (m_penMainMusicHolder!=NULL) {
				CMusicHolder *pmh = (CMusicHolder *)&*m_penMainMusicHolder;
				if (pmh->m_iRespawnMarker==-1) {
					// set it
					pmh->m_iRespawnMarker = pen->en_ulID;
				}
			}
			
			CPlayerMarker &CpmStart = (CPlayerMarker&)*pen;
			// set player characteristics
			if (bSetHealth) {
				SetHealth(CpmStart.m_fHealth/100.0f*TopHealth());
//				m_iMana  = GetSP()->sp_iInitialMana;
				m_fArmor = CpmStart.m_fShield;
			} else if (bAdjustHealth) {
				FLOAT fHealth = GetHealth();
				FLOAT fTopHealth = TopHealth();
				if( fHealth < fTopHealth) {
					SetHealth(ClampUp(fHealth+fTopHealth/2.0f, fTopHealth));
				}
			} 
			
			// if should start in computer
			if (CpmStart.m_bStartInComputer && GetSP()->sp_bSinglePlayer) {
				// mark that
				if (_pNetwork->IsPlayerLocal(this)) {
					cmp_ppenPlayer = this;
				}
				if (_pNetwork->IsServer()) {
					SendEvent(EShowConsole(),TRUE);
				}
				cmp_bInitialStart = TRUE;
			}			

			
//안태훈 수정 시작	//(Zone Change System)(0.1)
/*
			// set weapons
			if (!GetSP()->sp_bCooperative) {
				if (eCause!=TPC_FIRSTINIT) {
					((CPlayerWeapons&)*m_penWeapons).m_bResetWeapon = TRUE;
				}
				((CPlayerWeapons&)*m_penWeapons).InitializeWeapons(CpmStart.m_iGiveWeapons, 0, 0,
					CpmStart.m_fMaxAmmoRatio);
			} else {
				if (eCause!=TPC_FIRSTINIT) {
					((CPlayerWeapons&)*m_penWeapons).m_bResetWeapon = TRUE;
				}
				if (eCause==TPC_WORLDCHANGED && !GetSP()->sp_bSinglePlayer && GetSP()->sp_bCooperative) {
					((CPlayerWeapons&)*m_penWeapons).m_bForceColt = TRUE;
				}
				//0105 2line 삭제
				//        ((CPlayerWeapons&)*m_penWeapons).InitializeWeapons(CpmStart.m_iGiveWeapons, 0, 0,
				//          CpmStart.m_fMaxAmmoRatio);
				//..
			}
*/
//안태훈 수정 끝	//(Zone Change System)(0.1)
			// start position relative to link
			if (EwltType == WLT_RELATIVE) {
				plSet.AbsoluteToRelative(_SwcWorldChange.plLink);   // relative to link position
				plSet.RelativeToAbsolute(CpmStart.GetPlacement());  // absolute to start marker position
				Teleport(plSet);
				// fixed start position
			} else if (EwltType == WLT_FIXED) {
				CPlacement3D plNew = CpmStart.GetPlacement();
				vOffsetRel*=CpmStart.en_mRotation;
				plNew.pl_PositionVector += vOffsetRel;
				Teleport(plNew);                                        //0315 실제 텔레포트
				//0325
				//0322 kwon
				if(en_plPlacement.pl_PositionVector(1)!=32000.0f)//기본 텔레포트가 아니라면,
				{	
					if(_cmiComm. IsNetworkOn())
					{
					}
				}
				// error -> teleport to zero
			} else {
				//0527 kwon 잠시 삭제.
				//ASSERTALWAYS("Unknown world link type");
				//Teleport(CPlacement3D(FLOAT3D(0, 0, 0)+vOffsetRel, ANGLE3D(0, 0, 0)));
//강동민 수정 시작 클로즈 준비 작업	08.16
				Teleport(CPlacement3D(FLOAT3D(_pNetwork->MyCharacterInfo.x, _pNetwork->MyCharacterInfo.h, _pNetwork->MyCharacterInfo.z),ANGLE3D(0, 0, 0)));
//강동민 수정 끝 클로즈 준비 작업		08.16
			}
			// if there is a start trigger target
			if(CpmStart.m_penTarget!=NULL) 
			{
				SendToTarget(CpmStart.m_penTarget, EET_TRIGGER, this);
			}
		// default start position
		} 
		else 
		{
			// set player characteristics
			SetHealth(TopHealth());
//			m_iMana = GetSP()->sp_iInitialMana;
			m_fArmor = 0.0f;
			// set weapons
			//      ((CPlayerWeapons&)*m_penWeapons).InitializeWeapons(0, 0, 0, 0); 
			// start position
			Teleport(CPlacement3D(FLOAT3D(0, 0, 0)+vOffsetRel, ANGLE3D(0, 0, 0)));
		}
		// send teleport event to all entities in range
		SendEventInRange(ETeleport(), FLOATaabbox3D(GetPlacement().pl_PositionVector, 200.0f));
		// stop moving
		ForceFullStop();
		
		// remember maximum health
		m_fMaxHealth = TopHealth();
		
		// if in singleplayer mode
		if (GetSP()->sp_bSinglePlayer && GetSP()->sp_gmGameMode!=CSessionProperties::GM_FLYOVER) {
			CWorldSettingsController *pwsc = GetWSC(this);
			if (pwsc!=NULL && pwsc->m_bNoSaveGame) {
				NOTHING;
			} else {
				// save quick savegame
				//_pShell->Execute("gam_bQuickSave=1;");
			}
		}
		// remember level start time
		if (!(m_ulFlags&PLF_LEVELSTARTED)) {
			m_ulFlags |= PLF_LEVELSTARTED;
			m_tmLevelStarted = _pNetwork->GetGameTime();
		}
/* //1005 처음 접속시 아이템 벗겨지는 버그 수정.초기화를 해서 갑옷이 벗어졌었음.
		// reset model appearance
		CTString strDummy;
		//0105
		if(m_bMdl){
			SetPlayerAppearance(GetModelObject(), NULL, strDummy, FALSE);
		}else{	
			SetPlayerAppearanceSka(GetModelInstance(), NULL, strDummy, FALSE);		
		}
		//..
		ValidateCharacter();
		//0105
		if(m_bMdl){
			SetPlayerAppearance(&m_moRender, &en_pcCharacter, strDummy, FALSE);
		}else{
			GetModelInstance()->StretchModel(FLOAT3D(0.4f,0.4f,0.4f));
			SetPlayerAppearanceSka(&m_miRender, &en_pcCharacter, strDummy, FALSE);			
		}
		//0318 텔레포트 직전에 캐릭터 바꿔주기.
		SetSkaModel(JobInfo().GetFileName(en_pcCharacter.pc_iPlayerType));
		
		//..
		ParseGender(strDummy);
*/
		//0105
//		if(m_bMdl){
//			GetPlayerAnimator()->SetWeapon();
//		}
		//..
//		m_ulFlags |= PLF_SYNCWEAPON;
		
		// spawn teleport effect
		SpawnTeleport();
		// return from editor model (if was fragged into pieces)
		SwitchToModel();
		
		m_tmSpawned = en_tmEntityTime;
		en_tmLastBreathed = en_tmEntityTime+0.1f;  // do not take breath when spawned in air
	};
	
	//0105 함수 추가
	void CreateAnimAndBoneIDs(void)
	{
		// if we don't render (deleted?), do nothing
		if (en_RenderType!=RT_SKAMODEL && en_RenderType!=RT_SKAEDITORMODEL) { return; }
		CModelInstance *pmi = GetModelInstance();
		ASSERT(pmi!=NULL);

		const int iJob = en_pcCharacter.pc_iPlayerType;

		for( int i = ANIM_START; i < ANIM_TOTAL; ++i )
		{
			ska_CreateAnimIDFromString(&idPlayerWhole_Animation[i], pmi, CJobInfo::getSingleton()->GetAnimationName( iJob, i ) );
		}
	}
		
//강동민 수정 시작 클로즈 준비 작업	08.10
	void FillAnimAndBoneIDs()
	{
			// if we don't render (deleted?), do nothing
		if (en_RenderType!=RT_SKAMODEL && en_RenderType!=RT_SKAEDITORMODEL) { return; }
		CModelInstance *pmi = GetModelInstance();
		ASSERT(pmi!=NULL);
		
		const int iJob = en_pcCharacter.pc_iPlayerType;

		for( int i = ANIM_START; i < ANIM_TOTAL; ++i )
		{
			idPlayerWhole_Animation[i]	= ska_GetIDFromStringTable( CJobInfo::getSingleton()->GetAnimationName( iJob, i ) );
		}
	}
//강동민 수정 끝 클로즈 준비 작업		08.10	

	void IdleAnim()
	{
		NewClearState(0.f);

		if(_pNetwork->MyWearItem[WEAR_WEAPON].IsEmptyItem() == FALSE)
		{
			if( !_pNetwork->MyCharacterInfo.bExtension || m_bMobChange )
			{
				if( m_bRide )
				{
					INDEX idBodyAnim = -1;
					if( m_bWildRide )
					{
						if( m_nWildPetType == 2 )
						{
							idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1];
						}
						else
						{
							idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_IDLE_1];
						}
					}
					// 레어 펫 추가로 짝수 타입은 말...060822 wooss
					else if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1];
					}
					else
					{
						idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_DRAGON_IDLE_1];							
					}					
					
					AddAnimation(idBodyAnim, AN_LOOPING|AN_NORESTART, 1.0f, PLAYER_ANIM_GROUP_WHOLEBODY, ESKA_MASTER_MODEL_INSTANCE );					
				}
				else
				{
					AddAnimation(idPlayerWhole_Animation[ANIM_ATTACK_IDLE], AN_LOOPING|AN_NORESTART, 1.0f, PLAYER_ANIM_GROUP_WHOLEBODY,
						ESKA_MASTER_MODEL_INSTANCE, 1.0f);	
				}
			}			
			else
			{
				if( m_bRide )
				{
					INDEX idBodyAnim = -1;
					if( m_bWildRide )
					{
						if( m_nWildPetType == 2 )
						{
							idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1];
						}
						else
						{
							idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_IDLE_1];
						}
					}
					// 레어 펫 추가로 짝수 타입은 말...060822 wooss
					else if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_2];						
					}
					else
					{
						idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_DRAGON_IDLE_2];						
					}
					
					AddAnimation(idBodyAnim, AN_LOOPING|AN_NORESTART, 1.0f, PLAYER_ANIM_GROUP_WHOLEBODY, ESKA_MASTER_MODEL_INSTANCE );					
				}
				else
				{
					if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
					{
						AddAnimation(idPlayerWhole_Animation[ANIM_FLYING_READY], AN_LOOPING|AN_NORESTART, 1.0f, 
							PLAYER_ANIM_GROUP_WHOLEBODY, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
					}
					else
					{
						AddAnimation(idPlayerWhole_Animation[ANIM_EXT_ATTACK_IDLE], AN_LOOPING|AN_NORESTART, 1.0f, PLAYER_ANIM_GROUP_WHOLEBODY,
							ESKA_MASTER_MODEL_INSTANCE, 1.0f);
					}
				}
			}
		}
		else
		{
			if( m_bRide )
			{
				INDEX idBodyAnim = -1;
				if( m_bWildRide )
				{
					if( m_nWildPetType == 2 )
					{
						idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1];
					}
					else
					{
						idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_IDLE_1];
					}
				}
				// 레어 펫 추가로 짝수 타입은 말...060822 wooss
				else if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1];					
				}
				else
				{
					idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_DRAGON_IDLE_1];
				}
				
				AddAnimation(idBodyAnim, AN_LOOPING|AN_NORESTART, 1.0f, PLAYER_ANIM_GROUP_WHOLEBODY, ESKA_MASTER_MODEL_INSTANCE );				
			}
			else
			{
				if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
				{
					AddAnimation(idPlayerWhole_Animation[ANIM_FLYING_READY], AN_LOOPING|AN_NORESTART, 1.0f, 
						PLAYER_ANIM_GROUP_WHOLEBODY, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
				}
				else
				{
					AddAnimation(idPlayerWhole_Animation[ANIM_IDLE], AN_LOOPING|AN_NORESTART, 1.0f, PLAYER_ANIM_GROUP_WHOLEBODY,
						ESKA_MASTER_MODEL_INSTANCE, 1.0f);
				}
			}
		}
	}
	
	void ChangeWholeAnim(INDEX idBodyAnim, ULONG ulFlags, FLOAT fStrength, FLOAT fFadeTime, FLOAT fSpeed)
	{
		static FLOAT	start_attack_time;
		static float	m_fBodyAnimTime;
		static FLOAT	start_action_time;
		static FLOAT	start_Product_time;
		static FLOAT	start_Damage_time;
		//m_fBodyAnimTime = en_pmiModelInstance->GetAnimLength(idBodyAnim);
		
		static int cnt = 0;
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
		static int effectCnt = 0;
		int nIsCount = 0;
		INDEX iCurrentIsAnim = -1;
		INDEX iAttackType = ATTACK1;	// 공격 타입을 결정하는 변수
		//CTag *pEnTag = this->m_tmTagManager.Find("ET POS");
		//if(pEnTag != NULL && pEnTag->GetType() == TT_ENTITY)
		//{
			//TagInfo info;
			//info.m_fTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
			//info.m_vPos = FLOAT3D(0,0,0);
			//info.m_qRot = FLOATquat3D(1,0,0,0);
			//((CEntityTag*)pEnTag)->AddNewTagInfo(info);
		//}
//안태훈 수정 시작	//(Fix Snd Effect Bug)(0.1)
		if(en_pmiModelInstance) { en_pmiModelInstance->m_tmSkaTagManager.SetOwner(this); }
//안태훈 수정 끝	//(Fix Snd Effect Bug)(0.1)
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
		FLOAT3D vTargetPos(0, 0, 0);
		FLOAT3D vDirection(0, 0, 0);

		CEntity *pen = GetPlayerWeapons()->m_penRayHitTmp;

		CUIManager* pUIManager = SE_Get_UIManagerPtr();
		ObjInfo* pInfo = ObjInfo::getSingleton();
	
		FLOAT3D vCurrentCenter(((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[0],
			((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[1],
			((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[2] );
		FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vCurrentCenter;
		FLOAT3D vTargetCenter(0, 0, 0);
		FLOAT size = 0;
		if(pen != NULL && pen->GetFlags()&ENF_ALIVE && pen->IsFirstExtraFlagOff(ENF_EX1_CLICK_OBJECT))
		{
			if(pen->en_RenderType == RT_SKAMODEL)
			{
				// Bounding Box를 이용하여 Effect를 찍을 위치를 찾는다.
				FLOATaabbox3D bbox;
				pen->GetModelInstance()->GetCurrentColisionBox(bbox);
				FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*단순히 원으로 하면 너무 큼. 사이즈 조절 상수*/;
				size = vRadius.Length();
			}
			vTargetCenter = FLOAT3D(((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[0],
				((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[1],
				((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[2] );
			vTargetPos = pen->en_plPlacement.pl_PositionVector + vTargetCenter;
			vDirection = vTargetPos - vCurrentPos;
			vDirection.Normalize();
			vTargetPos -= vDirection * size;
		}
		vDirection.Normalize();				

		// NOTE : 뛰는 중에 걷는 애니메이션이나 IDLE 애니메이션을 못하게 하기 위한 부분인듯...
		if( IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RUN_1]) || 
			IsAnimationPlaying(idPlayerWhole_Animation[ANIM_EXT_RUN_2]) || 
			IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_RUN_1]) || 
			IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_RUN_1]) || 
			IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_RUN_1]) || 
			IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RUN_2]) )
		{
			if((m_bForward||m_bKeyMove) 
			&& (idBodyAnim == idPlayerWhole_Animation[ANIM_WALK_1] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_WALK_2] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_WALK_1] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_WALK_1] ||
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_WALK_1] ||  
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_IDLE_1] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_IDLE_1] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_2] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_IDLE_2] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_IDLE_2] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_WALK_2] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_IDLE_1] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_IDLE]) )
			{
				return;
			}
		}

		// NOTE : 걷는중에 IDLE 애니메이션을 못하게 하기 위한 부분인듯...
		if( IsAnimationPlaying(idPlayerWhole_Animation[ANIM_WALK_1]) || 
			IsAnimationPlaying(idPlayerWhole_Animation[ANIM_EXT_WALK_2]) || 
			IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_WALK_1]) || 
			IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_WALK_1]) || 
			IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_WALK_1]) || 
			IsAnimationPlaying(idPlayerWhole_Animation[ANIM_WALK_2] ))
		{
			if(	(m_bForward||m_bKeyMove) && 
				(	idBodyAnim == idPlayerWhole_Animation[ANIM_IDLE] || 
					idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1] || 
					idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_2] || 
					idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_IDLE_1] || 
					idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_IDLE_2] || 
					idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_IDLE_1] || 
					idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_IDLE_2] || 
					idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_IDLE_1] ))
			{
				return;
			}
		}
		// 0402 kwon
		float _PreAttackTime, _DoAttackTime, _PostAttackTime;//, _PreDoAttackTime;	
		static BOOL bIsPreDoEnd = FALSE; //0731 임시.						
		if(IsAnimationPlaying(idBodyAnim))
		{
			//0807 액션을 위한 임시코드			
			if( IsAnimationPlaying(idPlayerWhole_Animation[ANIM_DAMAGE]) || 
				IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_DAMAGE]) || 
				IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_DAMAGE]) || 
				IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_DAMAGE]) || 
				IsAnimationPlaying(idPlayerWhole_Animation[ANIM_EXT_DAMAGE]))
			{
				if(_pTimer->CurrentTick() - start_Damage_time >= m_fBodyAnimTime-0.1f)
				{                    
					NewClearState(CLEAR_STATE_LENGTH);    
					IdleAnim();
				}
				return;				
			}
			else
			{				
			}
			
			if( ( IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SIT_CONTINUE]) 
				|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_SIT_CONTINUE]) 
				|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SIT_CONTINUE]) ) 
				&& m_bPlayAction)				
			{
				if(m_nPlayActionNum==ACTION_NUM_SITDOWN && m_nActionSit == 3)//앉기유지상태였다면,
				{
					NewClearState(CLEAR_STATE_LENGTH);    
					start_action_time = _pTimer->CurrentTick();
					if( m_bRide )
					{
						INDEX idBodyAnim = -1;
						// 레어 펫 추가로 짝수 타입은 말...060822 wooss
						if( m_iRideType%2 == CPetInfo::PET_HORSE )
						{
							idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_HORSE_STANDUP];							
						}
						else
						{
							idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_DRAGON_STANDUP];							
						}
												
						AddAnimation(idBodyAnim, AN_NORESTART, 1.0f, PLAYER_ANIM_GROUP_WHOLEBODY, ESKA_MASTER_MODEL_INSTANCE );
					}
					else
					{
						AddAnimation(idPlayerWhole_Animation[ANIM_STANDUP], AN_NORESTART, 1.0f, PLAYER_ANIM_GROUP_WHOLEBODY,
							ESKA_MASTER_MODEL_INSTANCE, 1.0f);	
					}
					//m_nActionSit = 3;	
					return;
				}
				return;
			}

			else if( idBodyAnim == idPlayerWhole_Animation[ANIM_MINE] 
					|| idBodyAnim ==idPlayerWhole_Animation[ANIM_COLLECT]
					|| idBodyAnim== idPlayerWhole_Animation[ANIM_GATHER] )
			{

				// ESC 클릭시 타겟을 리셋되므로...
				if (pen == NULL )
				{
					return;
				}
				const INDEX iMobIndex = ((CEnemy*)pen)->m_nMobDBIndex;
				CMobData* MD = CMobData::getData(iMobIndex);

				// WSS_DRATAN_SEIGEWARFARE 2007/08/02 ------------------------->>
				if( _pNetwork->MyCharacterInfo.bConsensus )
				{
					// Constraint
					if(!MD->IsLordSymbol())
					{
						IdleAnim();
						pUIManager->GetSiegeWarfareNew()->StopConsensus(_pNetwork->MyCharacterInfo.index);
						return;
					}
				}
				// ------------------------------------------------------------<<
				else 
				{					
					if(!MD->IsCollect())
					{
					
						if(_pNetwork->MyWearItem[WEAR_SHIELD].IsEmptyItem() == FALSE)
						//if(_pNetwork->MyCurrentWearing[WEAR_SHIELD].Item_Index != -1)
						{
							CTString strSysMessage;
							strSysMessage.PrintF( _S( 696, "방패를 장착하고서 생산을 할 수 없습니다." ) );		
							_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );					
							CancelProduct();
							IdleAnim();
							return;
						}

						// FIXME : 이렇게 체크하는 것보다, 서버에서 확인을 받은 다음에 해주는게 좋지 않을까???
						//const int iWeaponType = _pNetwork->MyCurrentWearing[WEAPON].pItemData->GetSubType();
						if(_pNetwork->MyWearItem[WEAR_WEAPON].IsEmptyItem() == TRUE)
						{
							CancelProduct();
							IdleAnim();
							return;
						}
						const int iWeaponType = _pNetwork->MyWearItem[WEAR_WEAPON].ItemData->GetSubType();
						// 생산도구를 장착하구 있을때...
						if(!(iWeaponType == CItemData::ITEM_WEAPON_MINING ||
							iWeaponType == CItemData::ITEM_WEAPON_GATHERING || 
							iWeaponType == CItemData::ITEM_WEAPON_CHARGE))
						{
							CancelProduct();
							IdleAnim();
							return;
						}

						CEntity *pen = GetPlayerWeapons()->m_penRayHitTmp;//1006
						if( !pen )
						{					
							CancelProduct();
							IdleAnim();
							return;
						}
						if( !((iWeaponType == CItemData::ITEM_WEAPON_MINING) && MD->IsMineral() || 
							(iWeaponType == CItemData::ITEM_WEAPON_GATHERING) && MD->IsCrops() ||
							(iWeaponType == CItemData::ITEM_WEAPON_CHARGE) && MD->IsEnergy()))
						{
							CancelProduct();
							IdleAnim();
							return;
						}
					}
				} 
				
				if(_pTimer->CurrentTick() - start_Product_time >= m_fBodyAnimTime)
				{
					start_Product_time = _pTimer->CurrentTick();
					m_fBodyAnimTime = en_pmiModelInstance->GetAnimLength(idBodyAnim);

					if(pen->IsFirstExtraFlagOn(ENF_EX1_PRODUCTION))		// 생산
					{
						ObjectBase* pObject = ACTORMGR()->GetObjectByCIndex(eOBJ_MOB, pen->en_ulID);

						if (pObject != NULL)
						{							
							if(MD->IsMineral())
							{								
								_pNetwork->SendSelectProduceMessage(pObject->GetSIndex(), m_nProduceItemDBIndex);
							}
							else if(MD->IsCrops())
							{								
								_pNetwork->SendSelectProduceMessage(pObject->GetSIndex(), m_nProduceItemDBIndex);
							}
							else if(MD->IsEnergy())
							{									
								_pNetwork->SendSelectProduceMessage(pObject->GetSIndex(), m_nProduceItemDBIndex);
							}
							else if(MD->IsCollect())
							{									
								_pNetwork->SendCollectProduceMessage(pObject->GetSIndex());

								// 수집 동작시 무기 탈착
								DeleteWearingWeapon(FALSE, FALSE);
							}
						}
					}
				}				
			}

			if(	IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SIT]) 
				|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_SIT]) 
				|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SIT]) 
				|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_STANDUP]) 
				|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_STANDUP]) 
				|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_STANDUP]) 
				|| IsAnimationPlaying(idPlayerWhole_Anim_Face3) 
				|| IsAnimationPlaying(idPlayerWhole_Anim_Face4))
			{	
				if(m_bPlayAction)
				{
					if(_pTimer->CurrentTick() - start_action_time >= m_fBodyAnimTime-0.1f)//1218
					{
						if( IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SIT]) || 
							IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_SIT]) || 
							IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SIT]) )
						{
							NewClearState(CLEAR_STATE_LENGTH);

							if( m_bRide )
							{
								INDEX idBodyAnim = -1;
								// 레어 펫 추가로 짝수 타입은 말...060822 wooss
								if( m_iRideType%2 == CPetInfo::PET_HORSE )
								{
									idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_HORSE_SIT_CONTINUE];									
								}
								else 
								{
									idBodyAnim = idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SIT_CONTINUE];
								}

								
								AddAnimation(idBodyAnim, AN_LOOPING|AN_NORESTART, 1.0f, PLAYER_ANIM_GROUP_WHOLEBODY, ESKA_MASTER_MODEL_INSTANCE );
							}
							else
							{
								AddAnimation(idPlayerWhole_Animation[ANIM_SIT_CONTINUE], AN_LOOPING|AN_NORESTART, 1.0f, PLAYER_ANIM_GROUP_WHOLEBODY,
									ESKA_MASTER_MODEL_INSTANCE, 1.0f);		
							}
							
							m_nPlayActionNum	= 3;//앉아있는 상태...
							m_nActionSit = 2;//앉아있는 상태...
							return;
						}

						m_bPlayAction = FALSE;		
						m_nPlayActionNum	= -1;
						m_nActionSit = 0;
						AppearWearingWeapon(m_bSkilling);
						NewClearState(CLEAR_STATE_LENGTH);
						IdleAnim();
					}		
					return;
				}
			}

			if(IsAnimationPlaying(idPlayerWhole_Animation[ANIM_PICK]) 
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_EXT_PICK])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_PICK])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_PICK])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_PICK])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SOCIAL_0]) || IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SOCIAL_1])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SOCIAL_2]) || IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SOCIAL_3])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SOCIAL_4]) || IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SOCIAL_5])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SOCIAL_6]) || IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SOCIAL_7])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SOCIAL_8]) || IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SOCIAL_9])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SOCIAL_10]) || IsAnimationPlaying(idPlayerWhole_Animation[ANIM_SOCIAL_11])
			/*
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_LEVELUP])			
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_SIT_CONTINUE])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_SKILL_1])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_SKILL_2])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_SKILL_3])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_HORSE_SKILL_4])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_LEVELUP])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SIT_CONTINUE])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SKILL_1])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SKILL_2])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SKILL_3])
			|| IsAnimationPlaying(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SKILL_4])
			*/
			)
			{
				if(m_bPlayAction)
				{
					if(_pTimer->CurrentTick() - start_action_time >= m_fBodyAnimTime)
					{						
						AppearWearingWeapon(FALSE);	
						m_bPlayAction = FALSE;		
						m_nPlayActionNum	= -1;					
						NewClearState(CLEAR_STATE_LENGTH);
						IdleAnim();
					}		
					return;
				}
			}
			// 현재 공격 애니메이션 동작인 것을 찾는다. (iCurrentIsAnim 에 등록)
			for (nIsCount=0; nIsCount<4;++nIsCount)
			{
				int nAcessNum = ANIM_ATTACK_1;
				if (IsAnimationPlaying(idPlayerWhole_Animation[nAcessNum+nIsCount]))
				{
					iCurrentIsAnim = nAcessNum+nIsCount;
					break;
				}
				
				nAcessNum = ANIM_EXT_ATTACK_1;
				if (IsAnimationPlaying(idPlayerWhole_Animation[nAcessNum+nIsCount]))
				{
					iCurrentIsAnim = nAcessNum+nIsCount;
					break;
				}
			}

			// 찾은 공격 애니메이션을 처리한다.
			if (iCurrentIsAnim > 0)
			{
				const int iJob = en_pcCharacter.pc_iPlayerType;
				
				// Attack Type을 결정한다.
				if (iCurrentIsAnim >= ANIM_EXT_ATTACK_1 && iCurrentIsAnim <= ANIM_EXT_ATTACK_4)
				{
					if (m_bIsTransform)
					{ // 변신체일 경우
						iAttackType = (iCurrentIsAnim - ANIM_EXT_ATTACK_1);
					}
					else
					{
						iAttackType = (iCurrentIsAnim - ANIM_EXT_ATTACK_1) + EXT_ATTACK1;
					}
				}
				else if (iCurrentIsAnim >= ANIM_ATTACK_1 && iCurrentIsAnim <= ANIM_ATTACK_4)
				{
					iAttackType = (iCurrentIsAnim - ANIM_ATTACK_1);
				}

				// 결정된 Attack type으로 공격 타이밍을 계산한다.(_DoAttackTime --> 공격 시점)
				if (m_bIsTransform)
				{	// 변신체일 경우
					_DoAttackTime = TransformInfo().GetImpactTime( m_iTransformType, iAttackType ) * _fAttackLengthMul;
				}
				else
				{
					_DoAttackTime = CJobInfo::getSingleton()->GetImpactTime( iJob, iAttackType ) * _fAttackLengthMul;
				}

				_PreAttackTime	= 0.10f;
				_PostAttackTime = en_pmiModelInstance->GetAnimLength(idPlayerWhole_Animation[iCurrentIsAnim]) * _fAttackLengthMul;
				TIME tmpTime = _pTimer->CurrentTick();

				if(cnt == -1)
				{	
					if(pen != NULL && pen == this)
					{
						m_bStartAttack = FALSE;	

						NewClearState(CLEAR_STATE_LENGTH);
						IdleAnim();
						return;
					}

					m_penAttackingEnemy = pen;
					if(pen != NULL && pen->GetFlags()&ENF_ALIVE)
					{
						// 내가 뭔가를 공격하는 경우에, 소환수도 협공 하도록...
						for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
						{
							CUISummon* pUISummon = (CUISummon*)pUIManager->GetUI(i);
							CEntity* pSummonEntity = pUISummon->GetSummonEntity();
							if( pSummonEntity )
							{									
								if( pSummonEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE) )
								{						
									if( m_penAttackingEnemy != ((CSlave*)pSummonEntity)->m_penEnemy )
									{
										pSummonEntity->SendEvent(ESlaveKilledEnemy());
									}
								}
							}
						}

						// 공격 펫도 같이 공격
						if(pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->bIsActive)
						{
							CEntity* pWildPetEntity = pInfo->GetMyApetInfo()->m_pEntity;
							if(pWildPetEntity)
							{
								if(pWildPetEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_WILDPET) &&
									(m_penAttackingEnemy != ((CWildPet*)pWildPetEntity)->m_penEnemy))
								{
									pWildPetEntity->SendEvent(ESlaveKilledEnemy());

								}									
							}
						}

						// 싱글 모드일 경우에는 에너미만 선택하지...
						if(_pNetwork->m_bSingleMode && m_penAttackingEnemy->IsEnemy())
						{							
							m_dcEnemies.Clear();
							m_dcEnemies.Add(m_penAttackingEnemy);							
							
							// 일반 공격에 대한 처리...
							// FIXME : 중복되는 코드.
							if( iJob == HEALER ||
								iJob == MAGE ||
								IsEXMage(iJob)			//2013/01/22 jeil EX메이지 추가
								)
							{
								// 힐러의 경우 공격 가능 범위를 좀 더 넓게 했음.					
								// 범위내에 있는 적의 목록을 얻어옴.
								_pNetwork->FindTargetsInRange(this, m_penAttackingEnemy, m_dcEnemies, 
									5.0f, 4, 45.0f);
							}
							else
							{
								// 범위내에 있는 적의 목록을 얻어옴.
								_pNetwork->FindTargetsInRange(this, m_penAttackingEnemy, m_dcEnemies, 
									2.0f, 2, 45.0f);
							}							
							_pNetwork->SendAttackMessageInContainer(m_dcEnemies);
						}
						else
						{
							// FIXME : 이쪽 루틴이 Enemy가 확실하다면, 이렇게 처리할 필요가 없을듯...
							if(m_penAttackingEnemy->IsEnemy())
							{
								const INDEX iMobIndex = ((CEnemy*)((CEntity*)m_penAttackingEnemy))->m_nMobDBIndex;
								FLOAT3D vDelta = GetPlacement().pl_PositionVector - m_penAttackingEnemy->GetPlacement().pl_PositionVector;
								vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.
								BOOL bAttack = CheckAttackTarget(-1, m_penAttackingEnemy, vDelta.Length());

								if (bAttack)
								{
									// 성주의 권좌의 경우 길드장만 공격 가능.
									if( iMobIndex == LORD_SYMBOL_INDEX )
									{
										_pNetwork->SendAttackSymbol();
									}
									else
									{
										_pNetwork->SendAttackMessage(this, pen, FALSE);
									}
								}
							}
							else
							{
								_pNetwork->SendAttackMessage(this, pen, FALSE);
							}
						}
					}					

					if( iJob == KNIGHT )
					{
						if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] || idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_3])
						{
							StartEffectGroup("Knight Attack 1"		//Hardcording
								, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
						else if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2] || idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_4])
						{
							StartEffectGroup("Knight Attack 2"		//Hardcording
								, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
						//----------------------- 로그 및 전직 -----------------------------
						else if(idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] || idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_3])
						{
							StartEffectGroup("Knight Attack 1_a"		//Hardcording
								, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
						else if(idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_2] || idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_4])
						{
							StartEffectGroup("Knight Attack 2_a"		//Hardcording
								, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
					}			
					else if( iJob == TITAN )
					{
						if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] || idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_3])
						{
							StartEffectGroup("Titan Attack 1"		//Hardcording
								, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
						else if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2] || idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_4])
						{
							StartEffectGroup("Titan Attack 2"		//Hardcording
								, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
						//----------------------- 로그 및 전직 -----------------------------
						else if(idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] || idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_3])
						{
							StartEffectGroup("Titan Attack 1_a"		//Hardcording
								, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
						else if(idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_2] || idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_4])
						{
							StartEffectGroup("Titan Attack 2_a"		//Hardcording
								, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
					}					
					else if( iJob == MAGE )
					{
						if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] )
						{
							StartEffectGroup( "mEnerBall0", &en_pmiModelInstance->m_tmSkaTagManager,
								_pTimer->GetLerpedCurrentTick() );
						}
						else if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2] )
						{
							StartEffectGroup( "mEnerBall1", &en_pmiModelInstance->m_tmSkaTagManager,
								_pTimer->GetLerpedCurrentTick() );
						}
						//----------------------- 로그 및 전직 -----------------------------
						else if( idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] )
						{
							StartEffectGroup( "mEnerBall0", &en_pmiModelInstance->m_tmSkaTagManager,
								_pTimer->GetLerpedCurrentTick() );
						}
						else if( idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_2] )
						{
							StartEffectGroup( "mEnerBall1", &en_pmiModelInstance->m_tmSkaTagManager,
								_pTimer->GetLerpedCurrentTick() );
						}
					}
					else if( iJob == ROGUE )
					{
						if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1])
						{
							StartEffectGroup("Rogue Attack 1"		//Hardcording
								, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
						else if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2])
						{
							StartEffectGroup("Rogue Attack 2"		//Hardcording
								, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
					}
					else if( iJob == SORCERER )
					{
					}
					else if( IsEXRogue(iJob) )	// [2012/08/27 : Sora] EX로그 추가
					{
						if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1])
						{
							StartEffectGroup("Rogue Attack 1"		//Hardcording
								, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
						else if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2])
						{
							StartEffectGroup("Rogue Attack 2"		//Hardcording
								, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
					}
					else if( IsEXMage(iJob) )	//2013/01/08 jeil EX메이지 추가
					{
						if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] )
						{
							StartEffectGroup( "mEnerBall0", &en_pmiModelInstance->m_tmSkaTagManager,
								_pTimer->GetLerpedCurrentTick() );
						}
						else if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2] )
						{
							StartEffectGroup( "mEnerBall1", &en_pmiModelInstance->m_tmSkaTagManager,
								_pTimer->GetLerpedCurrentTick() );
						}
						//----------------------- 로그 및 전직 -----------------------------
						else if( idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] )
						{
							StartEffectGroup( "mEnerBall0", &en_pmiModelInstance->m_tmSkaTagManager,
								_pTimer->GetLerpedCurrentTick() );
						}
						else if( idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_2] )
						{
							StartEffectGroup( "mEnerBall1", &en_pmiModelInstance->m_tmSkaTagManager,
								_pTimer->GetLerpedCurrentTick() );
						}
					}

					cnt = 0;
					return;
				}

				if(_pTimer->CurrentTick() - start_attack_time > _PreAttackTime && cnt == 0)
				{					
					cnt++;
					bIsPreDoEnd = TRUE; //0731 임시.
					return;
				}
				
				if(_pTimer->CurrentTick() - start_attack_time > _DoAttackTime && cnt == 1)
				{	
					SetHitEffect(idBodyAnim, _pNetwork->MyCharacterInfo.iHitEffectType, pen, iAttackType);

					if (_cmiComm. IsNetworkOn())
					{						
						if( m_dcEnemies.Count() > 0 )
						{
							DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, vTargetPos, FALSE);
							m_dcEnemies.Clear();
						}
						else
						{							
							if( iJob != HEALER &&	//0801 힐러는 화살이 직접 데미지를 준다.
								iJob != MAGE && 
								!( iJob == ROGUE && _pNetwork->MyCharacterInfo.bExtension ) && 
								!( IsEXRogue(iJob)  && _pNetwork->MyCharacterInfo.bExtension ) &&	// [2012/08/27 : Sora] EX로그 추가
								!IsEXMage(iJob) &&													//2013/01/08 jeil EX메이지 추가
								!( iJob == SORCERER && m_bIsTransform && m_iTransformType == CTransformInfo::TYPE_1 ) &&
								iJob != NIGHTSHADOW)
							{
								if(m_penAttackingEnemy!=NULL)//데미지 줘야 한다면 여기서 
								{							
									this->InflictDirectDamage(m_penAttackingEnemy, this , DMT_CLOSERANGE, 1, vTargetPos, vDirection);
								}
							}
						}						
					}

					cnt++;
					return;
				}

				if(_pTimer->CurrentTick() - start_attack_time >= _PostAttackTime-0.1f && cnt == 2)// m_fBodyAnimTime = 1.55//1006
				{
					start_attack_time = _pTimer->CurrentTick();		
					cnt=-1;//cnt = 0;

					m_penAttackingEnemy = NULL;
//					m_fEnemyHealth =  -1.0f;
					//0611 kwon 추가.
					m_bLockMove = FALSE;				
					//CPrintF("_PostAttackTime!!!  \n");		
													
					CEntity *pen = GetPlayerWeapons()->m_penRayHitTmp;//1006

					CEntity *penTarget = GetPlayerWeapons()->m_penReservedTarget;

					if(m_nDesiredSkillNum != -1)//0107
					{
						m_bStartAttack = FALSE;		
						
						if(m_bReservedCancel)
						{
							GetPlayerWeapons()->m_penRayHitTmp = NULL;
							m_bReservedCancel = FALSE;
							return;
						}
						
						UseSkill(m_nDesiredSkillNum);
						return;
					}

					if(!g_iAutoAttack || m_bReserveMove || m_nCurrentSkillNum!=-1 || m_bReservedCancel || (pen != NULL && !(pen->GetFlags()&ENF_ALIVE)) || pen == NULL || penTarget!=m_penAttackingEnemy)//0713 kwon//1006
					{
						//공격중지. 이동.						
						m_bStartAttack = FALSE;		
						//0807
						if(m_bReservedCancel||!g_iAutoAttack)
						{
							GetPlayerWeapons()->m_penRayHitTmp = NULL;
						}
						m_bReservedCancel = FALSE;

						NewClearState(/*fFadeTime*/CLEAR_STATE_LENGTH);
						IdleAnim();						
					}				
					return;
				}		
			}
			return;
		}
		cnt=0;//cnt = 0;
		effectCnt = 0;

		// 0220 kwon 아래 함수를 지우면 무한 애니가 된다.
			NewClearState(/*fFadeTime*/CLEAR_STATE_LENGTH);
		
		if( idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_WALK_1] || 
			idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_WALK_1] || 
			idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_WALK_1] )
		{
		}
		else if(idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1] ||
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_IDLE_1] ||
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_IDLE_1] )
		{
		}
		else if(idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_2] ||
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_IDLE_2] ||
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_IDLE_2] )
		{
		}
		else if(idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_RUN_1] ||
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_RUN_1] ||
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_RUN_1] )
		{
		}
		else if(idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_DAMAGE] ||
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_DAMAGE] ||
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_DAMAGE] )
		{
		}
		else if(idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_DIE] ||
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_DIE] ||
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_DIE] )
		{
		}		

		// Walk Animation???
		AddAnimation(idBodyAnim, ulFlags, fStrength, PLAYER_ANIM_GROUP_WHOLEBODY,
					ESKA_MASTER_MODEL_INSTANCE, fSpeed);		
		
		// FIXME : 무슨 루틴인지 잘 모르겠음...
		// FIXME : 1, 2번 애니만 공격???
		if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] || 
			idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2] || 
			IsAnimationPlaying(idPlayerWhole_Animation[ANIM_ATTACK_3]) || 
			IsAnimationPlaying(idPlayerWhole_Animation[ANIM_ATTACK_4]) ||
			idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] || 
			idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_2] || 
			idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_3] || 
			idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_4] )
		{
			float frandom = FRnd();

			// FIXME : PlaySound 부분은 es의 Component에 들어가야 되는 부분이므로, 정리하기 어려움...
			const int iJob = en_pcCharacter.pc_iPlayerType;
			if( iJob == KNIGHT )
			{
				if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] || idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_3])
				{
					StartEffectGroup("Knight Attack 1"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				else if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2] || idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_4])
				{
					StartEffectGroup("Knight Attack 2"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				//----------------------- 로그 및 전직 -----------------------------
				else if(idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] || idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_3])
				{
					StartEffectGroup("Knight Attack 1_a"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				else if(idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_2] || idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_4])
				{
					StartEffectGroup("Knight Attack 2_a"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				if(frandom <= 0.25f)
				{
					PlaySound(m_soMessage, GenderSound(SOUND_KNIGHT_ATTACK), SOF_3D | SOF_VOLUMETRIC);
				}
			}
			else if( iJob == TITAN )
			{
				if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] || idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_3])
				{
					StartEffectGroup("Titan Attack 1"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				else if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2] || idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_4])
				{
					StartEffectGroup("Titan Attack 2"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				//----------------------- 로그 및 전직 -----------------------------
				else if(idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] || idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_3])
				{
					StartEffectGroup("Titan Attack 1_a"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				else if(idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_2] || idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_4])
				{
					StartEffectGroup("Titan Attack 2_a"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				if(frandom <= 0.25f)
				{
					PlaySound(m_soMessage, GenderSound(SOUND_TITAN_ATTACK), SOF_3D | SOF_VOLUMETRIC);
				}
			}
			else if( iJob == MAGE )
			{
				if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] )
				{
					StartEffectGroup( "mEnerBall0", &en_pmiModelInstance->m_tmSkaTagManager,
										_pTimer->GetLerpedCurrentTick() );
				}
				else if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2] )
				{
					StartEffectGroup( "mEnerBall1", &en_pmiModelInstance->m_tmSkaTagManager,
										_pTimer->GetLerpedCurrentTick() );
				}
				//----------------------- 로그 및 전직 -----------------------------
				else if( idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] )
				{
					StartEffectGroup( "mEnerBall0_a", &en_pmiModelInstance->m_tmSkaTagManager,
										_pTimer->GetLerpedCurrentTick() );
				}
				else if( idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_2] )
				{
					StartEffectGroup( "mEnerBall1_a", &en_pmiModelInstance->m_tmSkaTagManager,
										_pTimer->GetLerpedCurrentTick() );
				}
				if(frandom <= 0.25f)
				{
					PlaySound(m_soMessage, GenderSound(SOUND_MAGE_ATTACK), SOF_3D | SOF_VOLUMETRIC);
				}
			}
			else if( iJob == HEALER )
			{
				if(frandom <= 0.25f)
				{
					PlaySound(m_soMessage, GenderSound(SOUND_HEALER_ATTACK), SOF_3D | SOF_VOLUMETRIC);
				}
			}
			else if( iJob == ROGUE )
			{
				if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1])
				{
					StartEffectGroup("Rogue Attack 1"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				else if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2])
				{
					StartEffectGroup("Rogue Attack 2"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				if(frandom <= 0.25f)
				{
					PlaySound(m_soMessage, GenderSound(SOUND_ROGUE_ATTACK), SOF_3D | SOF_VOLUMETRIC);
				}
			}
			else if( iJob == SORCERER )
			{
			/*
				if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] || idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_3])
				{
					StartEffectGroup("Titan Attack 1"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				else if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2] || idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_4])
				{
					StartEffectGroup("Titan Attack 2"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				//----------------------- 로그 및 전직 -----------------------------
				else if(idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] || idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_3])
				{
					StartEffectGroup("Titan Attack 1_a"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				else if(idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_2] || idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_4])
				{
					StartEffectGroup("Titan Attack 2_a"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				*/
				if(frandom <= 0.25f)
				{
					PlaySound(m_soMessage, GenderSound(SOUND_TITAN_ATTACK), SOF_3D | SOF_VOLUMETRIC);
				}
			}
			else if( IsEXRogue(iJob)  )	// [2012/08/27 : Sora] EX로그 추가
			{
				if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1])
				{
					StartEffectGroup("Rogue Attack 1"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				else if(idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2])
				{
					StartEffectGroup("Rogue Attack 2"		//Hardcording
						, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				if(frandom <= 0.25f)
				{
					PlaySound(m_soMessage, GenderSound(SOUND_ROGUE_ATTACK), SOF_3D | SOF_VOLUMETRIC);
				}
			}
			else if( IsEXMage(iJob)	)	//2013/01/08 jeil EX메이지 추가
			{
				if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] )
				{
					StartEffectGroup( "mEnerBall0", &en_pmiModelInstance->m_tmSkaTagManager,
										_pTimer->GetLerpedCurrentTick() );
				}
				else if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2] )
				{
					StartEffectGroup( "mEnerBall1", &en_pmiModelInstance->m_tmSkaTagManager,
										_pTimer->GetLerpedCurrentTick() );
				}
				//----------------------- 로그 및 전직 -----------------------------
				else if( idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] )
				{
					StartEffectGroup( "mEnerBall0_a", &en_pmiModelInstance->m_tmSkaTagManager,
										_pTimer->GetLerpedCurrentTick() );
				}
				else if( idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_2] )
				{
					StartEffectGroup( "mEnerBall1_a", &en_pmiModelInstance->m_tmSkaTagManager,
										_pTimer->GetLerpedCurrentTick() );
				}
				if(frandom <= 0.25f)
				{
					PlaySound(m_soMessage, GenderSound(SOUND_MAGE_ATTACK), SOF_3D | SOF_VOLUMETRIC);
				}
			}

			m_fBodyAnimTime = GetModelInstance()->GetAnimLength(idBodyAnim)*_fAttackLengthMul;//0.8f;//1009
			start_attack_time = _pTimer->CurrentTick();

			m_bLockMove = TRUE;
		
			m_penAttackingEnemy = pen;
			if(pen != NULL && pen->GetFlags()&ENF_ALIVE)
			{
				// 내가 뭔가를 공격하는 경우에, 소환수도 협공 하도록...
				for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
				{
					CUISummon* pUISummon = (CUISummon*)pUIManager->GetUI(i);
					CEntity* pSummonEntity = pUISummon->GetSummonEntity();
					if( pSummonEntity )
					{									
						if( pSummonEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE) )
						{
							if( m_penAttackingEnemy != ((CSlave*)pSummonEntity)->m_penEnemy )
							{
								pSummonEntity->SendEvent(ESlaveKilledEnemy());
							}
						}
					}
				}

				// 공격 펫도 같이 공격
				if(pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->bIsActive)
				{
					CEntity* pWildPetEntity = pInfo->GetMyApetInfo()->m_pEntity;
					if(pWildPetEntity)
					{
						if(pWildPetEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_WILDPET) &&
							(m_penAttackingEnemy != ((CWildPet*)pWildPetEntity)->m_penEnemy))
						{
							pWildPetEntity->SendEvent(ESlaveKilledEnemy());

						}									
					}
				}

				if(_pNetwork->m_bSingleMode && m_penAttackingEnemy->IsEnemy())
				{
					m_dcEnemies.Clear();
					m_dcEnemies.Add(m_penAttackingEnemy);				

					// 일반 공격에 대한 처리.
					if( iJob == HEALER || 
						iJob == MAGE ||
						iJob == NIGHTSHADOW ||
						IsEXMage(iJob)		//2013/01/22 jeil EX메이지 추가 
						)
					{
						// 범위내에 있는 적의 목록을 얻어옴.
						_pNetwork->FindTargetsInRange(this, m_penAttackingEnemy, m_dcEnemies, 
							5.0f, 4, 45.0f);
					}
					else
					{
						// 범위내에 있는 적의 목록을 얻어옴.
						_pNetwork->FindTargetsInRange(this, m_penAttackingEnemy, m_dcEnemies, 
							2.0f, 2, 45.0f);
					}					
					if(!m_bSendStopMessage)
					{
						m_bForward = TRUE;
						//m_bKeyMove = TRUE;
						StopMove();
					}
					_pNetwork->SendAttackMessageInContainer(m_dcEnemies);					
				}
				else
				{
					if(!m_bSendStopMessage)
					{
						m_bForward = TRUE;
						//m_bKeyMove = TRUE;
						StopMove();
					}

					// FIXME : 이쪽 루틴이 Enemy가 확실하다면, 이렇게 처리할 필요가 없을듯...
					if(m_penAttackingEnemy->IsEnemy())
					{
						const INDEX iMobIndex = ((CEnemy*)((CEntity*)m_penAttackingEnemy))->m_nMobDBIndex;		
						FLOAT3D vDelta = GetPlacement().pl_PositionVector - m_penAttackingEnemy->GetPlacement().pl_PositionVector;
						vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.
						BOOL bAttack = CheckAttackTarget(-1, m_penAttackingEnemy, vDelta.Length());

						if (bAttack)
						{
							// 성주의 권좌의 경우 길드장만 공격 가능.
							if( iMobIndex == LORD_SYMBOL_INDEX )
							{
								_pNetwork->SendAttackSymbol();
							}
							else
							{
								_pNetwork->SendAttackMessage(this, pen, FALSE);
							}
						}
					}
					else
					{
						_pNetwork->SendAttackMessage(this, pen, FALSE);
					}					
				}
			}
		} 
		else if( idBodyAnim == idPlayerWhole_Animation[ANIM_SIT] 		
				|| idBodyAnim ==idPlayerWhole_Animation[ANIM_STANDUP] //0824				
				|| idBodyAnim== idPlayerWhole_Anim_Face3 
				|| idBodyAnim==idPlayerWhole_Anim_Face4)//0807
		{
			SBYTE	state = 0;
			
			//if(!(m_nPlayActionNum == ACTION_NUM_SELL && pUIManager->GetPersonalShop()->IsVisible() && pUIManager->GetPersonalShop()->IsBuyShop()))//샾이 아닐때 메시지를 보낸다.
			if( !_bPersonalShop )
			{
				if( idBodyAnim == idPlayerWhole_Animation[ANIM_SIT]) //앉기.
				{
					_pNetwork->SendActionMessage(0, ACTION_NUM_SITDOWN, 1); //수정해야함. //0은 일반, 3은 앉기id, 1은 앉는다는 상태.
				}
				else //서기.
				{
					_pNetwork->SendActionMessage(0, ACTION_NUM_SITDOWN, 0);
				}
			}
			m_fBodyAnimTime = GetModelInstance()->GetAnimLength(idBodyAnim);
			start_action_time = _pTimer->CurrentTick();			
		}
		else if( idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_SIT] 
				|| idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SIT] 
				|| idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_STANDUP] 
				|| idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_STANDUP] )
		{
			SBYTE	state = 0;			
			
			if( !_bPersonalShop )
			{
				if( idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_SIT] || 
					idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SIT] ) //앉기.
				{
					_pNetwork->SendActionMessage(0, ACTION_NUM_SITDOWN, 1); //수정해야함. //0은 일반, 3은 앉기id, 1은 앉는다는 상태.
				}
				else //서기.
				{
					_pNetwork->SendActionMessage(0, ACTION_NUM_SITDOWN, 0);
				}
			}				
			start_action_time = _pTimer->CurrentTick();			
		}
		else if( idBodyAnim == idPlayerWhole_Animation[ANIM_MINE] 
				|| idBodyAnim ==idPlayerWhole_Animation[ANIM_COLLECT]
				|| idBodyAnim== idPlayerWhole_Animation[ANIM_GATHER])
		{
			m_fBodyAnimTime = GetModelInstance()->GetAnimLength(idBodyAnim);
			start_Product_time = _pTimer->CurrentTick();		
		}
		else if(idBodyAnim == idPlayerWhole_Animation[ANIM_PICK] 
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_PICK]
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_SOCIAL_0] 
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_SOCIAL_1] || idBodyAnim == idPlayerWhole_Animation[ANIM_SOCIAL_2]  
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_SOCIAL_3] || idBodyAnim == idPlayerWhole_Animation[ANIM_SOCIAL_4] 
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_SOCIAL_5] || idBodyAnim == idPlayerWhole_Animation[ANIM_SOCIAL_6]
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_SOCIAL_7] || idBodyAnim == idPlayerWhole_Animation[ANIM_SOCIAL_8]
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_SOCIAL_9] || idBodyAnim == idPlayerWhole_Animation[ANIM_SOCIAL_10]
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_SOCIAL_11]
		/*
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_SIT_CONTINUE] || idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_SKILL_1] 
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_SKILL_2] || idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_SKILL_3] 
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_SKILL_4] || idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_LEVELUP]
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SIT_CONTINUE] || idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SKILL_1] 
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SKILL_2] || idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SKILL_3] 
		|| idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SKILL_4] || idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_LEVELUP]
		*/
		)
		{
			m_fBodyAnimTime = GetModelInstance()->GetAnimLength(idBodyAnim);
			start_action_time = _pTimer->CurrentTick();			
		}		
		else if(idBodyAnim == idPlayerWhole_Animation[ANIM_DAMAGE] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_HORSE_DAMAGE] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DRAGON_DAMAGE] || 
				idBodyAnim == idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_DAMAGE] )
		{
			m_fBodyAnimTime = GetModelInstance()->GetAnimLength(idBodyAnim);
			start_Damage_time = _pTimer->CurrentTick();			
		}
	}

	void SetHitEffect(INDEX idBodyAnim, int nHitType, CEntity *pen, INDEX iAttackType)
	{
		const int iJob = en_pcCharacter.pc_iPlayerType;

		// 근거리 무기의 경우 사운드 + 카메라 쉐이크만 지정해준다.
		// Player.es, Character.es, Enemy.es의 ReceiveDamage()함수에서 이펙트 처리.
		
		switch( iJob )		
		{
			case TITAN:
				if (nHitType > 0)
				{
					PlaySound(m_soMessage, DEF_SOUND_HOLYWATER_HIT, SOF_3D | SOF_VOLUMETRIC);
					StartEffectGroup(DEF_CAM_SHAKE, _pTimer->CurrentTick(), this->en_plPlacement.pl_PositionVector, FLOAT3D(0, 0, 0));
					m_bCriticalDamage = FALSE;
				}
				else
				{
					if( _pNetwork->MyCharacterInfo.bExtension )
					{
						if(m_bCriticalDamage)
						{
							PlaySound(m_soMessage, GenderSound(SOUND_AXE_CRITICAL), SOF_3D | SOF_VOLUMETRIC);		
							m_bCriticalDamage = FALSE;
						}
						else
						{
							PlaySound(m_soMessage, GenderSound(SOUND_AXE_BLOW), SOF_3D | SOF_VOLUMETRIC);		
						}
					}
					else
					{							
						if(m_bCriticalDamage)
						{
							PlaySound(m_soMessage, GenderSound(SOUND_BIGSWORD_CRITICAL), SOF_3D | SOF_VOLUMETRIC);		
							m_bCriticalDamage = FALSE;
						}
						else
						{
							PlaySound(m_soMessage, GenderSound(SOUND_BIGSWORD_BLOW), SOF_3D | SOF_VOLUMETRIC);		
						}
					}
				}
				break;

			case HEALER: 
				//PlaySound(m_soMessage, GenderSound(SOUND_HEALER_NORMAL_IMPACT), SOF_3D | SOF_VOLUMETRIC);//0822 삭제.
				if(en_pmiModelInstance != NULL && pen != NULL && pen->IsFlagOn(ENF_ALIVE))
				{
					if(!_pNetwork->m_bSingleMode)
					{
						if( _pNetwork->MyCharacterInfo.bExtension )
						{
							if(pen->IsEnemy())
							{
								FLOAT3D vDelta = GetPlacement().pl_PositionVector - pen->GetPlacement().pl_PositionVector;
								vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.
								BOOL bAttack = CheckAttackTarget(-1, pen, vDelta.Length());
								if(bAttack)
								{
									ShotMissile(this, "STAFF", pen, 40.0f, szHitEffect[iJob][nHitType], "Normal Arrow Trace", m_bCriticalDamage!=0, 0.0f, 0.0f, 1);
								}
							}
							else
							{
								ShotMissile(this, "STAFF", pen, 40.0f, szHitEffect[iJob][nHitType], "Normal Arrow Trace", m_bCriticalDamage!=0, 0.0f, 0.0f, 1);
							}
						}
						else
						{
							if(pen->IsEnemy())
							{
								FLOAT3D vDelta = GetPlacement().pl_PositionVector - pen->GetPlacement().pl_PositionVector;
								vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.
								BOOL bAttack = CheckAttackTarget(-1, pen, vDelta.Length());
								if(bAttack)
								{
									ShotMissile(this, "RHAND", pen, 40.0f, szHitEffect[iJob][nHitType], "Normal Arrow Trace", m_bCriticalDamage!=0);
								}
							}
							else
							{
								ShotMissile(this, "RHAND", pen, 40.0f, szHitEffect[iJob][nHitType], "Normal Arrow Trace", m_bCriticalDamage!=0);
							}
						}
					}
					else
					{
						for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
							it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
						{																		
							CEntity *pEn = (*it);
							if(pEn != NULL && pEn->IsFlagOn(ENF_ALIVE))
							{
								if( _pNetwork->MyCharacterInfo.bExtension )
								{
									ShotMissile(this, "STAFF", pEn, 40.0f, szHitEffect[iJob][nHitType], "Normal Arrow Trace", m_bCriticalDamage!=0, 0.0f, 0.0f, 1);
								}
								else
								{
									ShotMissile(this, "RHAND", pEn, 40.0f, szHitEffect[iJob][nHitType], "Normal Arrow Trace", m_bCriticalDamage!=0);
								}
							}
						}
						m_dcEnemies.Clear();
					}
				}

				m_bCriticalDamage = FALSE;
				break;

			case MAGE:
				if( en_pmiModelInstance != NULL && pen != NULL )
				{
					FLOAT3D		vStartPos;
					FLOATquat3D	qStartRot;
					CTag		*pTag = NULL;

					if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] )
					{
						pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "WAND" );
						if( pTag == NULL )
						{
							pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
						}
						if( pTag != NULL )
						{
							vStartPos = pTag->CurrentTagInfo().m_vPos;
							qStartRot = pTag->CurrentTagInfo().m_qRot;
						}
					}
					else if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2] )
					{
						pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "LHAND" );
						if( pTag != NULL )
						{
							vStartPos = pTag->CurrentTagInfo().m_vPos;
							qStartRot = pTag->CurrentTagInfo().m_qRot;
						}
					}
					else if( idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] )
					{
						pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
						if( pTag != NULL )
						{
							vStartPos = pTag->CurrentTagInfo().m_vPos;
							qStartRot = pTag->CurrentTagInfo().m_qRot;
						}
					}
					else if( idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_2] )
					{
						pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
						if( pTag != NULL )
						{
							vStartPos = pTag->CurrentTagInfo().m_vPos;
							qStartRot = pTag->CurrentTagInfo().m_qRot;
						}
					}

					ASSERT( pTag != NULL );
					if( pTag != NULL )
					{
						if( !_pNetwork->m_bSingleMode && pen != NULL && pen->IsFlagOn(ENF_ALIVE))
						{
							if(pen->IsEnemy())
							{
								FLOAT3D vDelta = GetPlacement().pl_PositionVector - pen->GetPlacement().pl_PositionVector;
								vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.
								BOOL bAttack = CheckAttackTarget(-1, pen, vDelta.Length());
								if(bAttack)
								{
									ShotMagicContinued( this, vStartPos, qStartRot, pen, 35.0F,
													szHitEffect[iJob][nHitType], "mMissileEnerBall",
													m_bCriticalDamage != 0, 0 );
								}
							}
							else
							{
								ShotMagicContinued( this, vStartPos, qStartRot, pen, 35.0F,
													szHitEffect[iJob][nHitType], "mMissileEnerBall",
													m_bCriticalDamage != 0, 0 );
							}
						}
						else
						{
							for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
								it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
							{																			
								CEntity *pEn = (*it);
								if(pEn != NULL && pEn->IsFlagOn(ENF_ALIVE))
								{
									ShotMagicContinued( this, vStartPos, qStartRot, pEn, 35.0F,
														szHitEffect[iJob][nHitType], "mMissileEnerBall",
														m_bCriticalDamage != 0, 0 );
								}
							}
							m_dcEnemies.Clear();
						}
					}
				}
				m_bCriticalDamage = FALSE;
				break;

			case KNIGHT:
				if (nHitType > 0)
				{
					PlaySound(m_soMessage, DEF_SOUND_HOLYWATER_HIT, SOF_3D | SOF_VOLUMETRIC);
					StartEffectGroup(DEF_CAM_SHAKE, _pTimer->CurrentTick(), this->en_plPlacement.pl_PositionVector, FLOAT3D(0, 0, 0));
					m_bCriticalDamage = FALSE;
				}
				else
				{
					if(m_bCriticalDamage)
					{
						PlaySound(m_soMessage, GenderSound(SOUND_SWORD_CRITICAL), SOF_3D | SOF_VOLUMETRIC);		
						m_bCriticalDamage = FALSE;
					}
					else
					{
						PlaySound(m_soMessage, GenderSound(SOUND_SWORD_BLOW), SOF_3D | SOF_VOLUMETRIC);		
					}
				}
				break;
			case ROGUE:
				// 로그는 근거리 & 원거리 공격 가능.
				// 석궁을 들었을 때....
				if( _pNetwork->MyCharacterInfo.bExtension )
				{
					if(en_pmiModelInstance != NULL && pen != NULL)
					{
						if(!_pNetwork->m_bSingleMode && pen->IsFlagOn(ENF_ALIVE))
						{
							if(pen->IsEnemy())
							{
								FLOAT3D vDelta = GetPlacement().pl_PositionVector - pen->GetPlacement().pl_PositionVector;
								vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.
								BOOL bAttack = CheckAttackTarget(-1, pen, vDelta.Length());
								if(bAttack)
								{
									ShotMissile(this, "RHAND", pen, 40.0f, szHitEffect[iJob][nHitType], "Normal Arrow Trace", m_bCriticalDamage!=0);
								}
							}
							else
							{
								ShotMissile(this, "RHAND", pen, 40.0f, szHitEffect[iJob][nHitType], "Normal Arrow Trace", m_bCriticalDamage!=0);
							}
						}
						else
						{
							for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
								it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
							{																			
								CEntity *pEn = (*it);
								if(pEn != NULL && pEn->IsFlagOn(ENF_ALIVE))
								{
									ShotMissile(this, "RHAND", pEn, 40.0f, szHitEffect[iJob][nHitType], "Normal Arrow Trace", m_bCriticalDamage!=0);
								}
							}
							m_dcEnemies.Clear();
						}
					}
					m_bCriticalDamage = FALSE;
				}
				// 단검을 들었을 때...
				else
				{
					if (nHitType > 0)
					{
						PlaySound(m_soMessage, DEF_SOUND_HOLYWATER_HIT, SOF_3D | SOF_VOLUMETRIC);
						StartEffectGroup(DEF_CAM_SHAKE, _pTimer->CurrentTick(), this->en_plPlacement.pl_PositionVector, FLOAT3D(0, 0, 0));
						m_bCriticalDamage = FALSE;
					}
					else
					{
						if(m_bCriticalDamage)
						{
							PlaySound(m_soMessage, GenderSound(SOUND_DAGGER_CRITICAL), SOF_3D | SOF_VOLUMETRIC);		
							m_bCriticalDamage = FALSE;
						}
						else
						{
							PlaySound(m_soMessage, GenderSound(SOUND_DAGGER_BLOW), SOF_3D | SOF_VOLUMETRIC);		
						}
					}
				}
				break;

			case SORCERER:
				{
					if( m_bIsTransform && m_iTransformType == CTransformInfo::TYPE_1 )
					{
						if( en_pmiModelInstance != NULL && pen != NULL )
						{
							FLOAT3D		vStartPos;
							FLOATquat3D	qStartRot;
							CTag		*pTag = NULL;
							
							pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "LHAND" );
							
							if( pTag != NULL )
							{
								vStartPos = pTag->CurrentTagInfo().m_vPos;
								qStartRot = pTag->CurrentTagInfo().m_qRot;
							}

							ASSERT( pTag != NULL );
							if( pTag != NULL )
							{
								if( !_pNetwork->m_bSingleMode && pen != NULL && pen->IsFlagOn(ENF_ALIVE))
								{
									if(pen->IsEnemy())
									{
										FLOAT3D vDelta = GetPlacement().pl_PositionVector - pen->GetPlacement().pl_PositionVector;
										vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.

										BOOL bAttack = CheckAttackTarget( -1, pen, vDelta.Length() );
										if(bAttack)
										{
											// 첫번째 공격.
											if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] || 
												idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] || 
												idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_3] || 
												idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_3] )
											{
												ShotMagicContinued( this, vStartPos, qStartRot, pen, 35.0F,
																szHitEffect[iJob][nHitType], "so2_Att02_Missile",
																m_bCriticalDamage != 0, 0 );
											}
											else
											{														
												ShotMagicContinued( this, vStartPos, qStartRot, pen, 35.0F,
																"so2_Att01_Hit", "so2_Att01_Missile",
																m_bCriticalDamage != 0, 0 );
											}
										}
									}
									else
									{
										// 첫번째 공격.
										if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] || 
											idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] || 
											idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_3] || 
											idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_3] )
										{
											ShotMagicContinued( this, vStartPos, qStartRot, pen, 35.0F,
															szHitEffect[iJob][nHitType], "so2_Att02_Missile",
															m_bCriticalDamage != 0, 0 );
										}
										else
										{														
											ShotMagicContinued( this, vStartPos, qStartRot, pen, 35.0F,
															"so2_Att01_Hit", "so2_Att01_Missile",
															m_bCriticalDamage != 0, 0 );
										}
									}
								}
								else
								{
									for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
										it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
									{																			
										CEntity *pEn = (*it);
										if(pEn != NULL && pEn->IsFlagOn(ENF_ALIVE))
										{
											// 첫번째 공격.
											if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] || 
												idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] || 
												idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_3] || 
												idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_3] )
											{
												ShotMagicContinued( this, vStartPos, qStartRot, pen, 35.0F,
																szHitEffect[iJob][nHitType], "so2_Att02_Missile",
																m_bCriticalDamage != 0, 0 );
											}
											else
											{														
												ShotMagicContinued( this, vStartPos, qStartRot, pen, 35.0F,
																"so2_Att01_Hit", "so2_Att01_Missile",
																m_bCriticalDamage != 0, 0 );
											}
										}
									}
									m_dcEnemies.Clear();
								}
							}
						}
						m_bCriticalDamage = FALSE;
					}
					else
					{
						if (nHitType > 0)
						{
							PlaySound(m_soMessage, DEF_SOUND_HOLYWATER_HIT, SOF_3D | SOF_VOLUMETRIC);
							StartEffectGroup(DEF_CAM_SHAKE, _pTimer->CurrentTick(), this->en_plPlacement.pl_PositionVector, FLOAT3D(0, 0, 0));
							m_bCriticalDamage = FALSE;
						}
						else
						{
							if(m_bCriticalDamage)
							{
								PlaySound(m_soMessage, GenderSound(SOUND_SWORD_CRITICAL), SOF_3D | SOF_VOLUMETRIC);		
								m_bCriticalDamage = FALSE;
							}
							else
							{
								PlaySound(m_soMessage, GenderSound(SOUND_SWORD_BLOW), SOF_3D | SOF_VOLUMETRIC);		
							}
						}
					}
				}
				break;
			case NIGHTSHADOW:
				{
					if(en_pmiModelInstance != NULL && pen != NULL && pen->IsFlagOn(ENF_ALIVE))
					{
						CTString strMissile = CTString("ns_basic"); // 발사체 이펙트
						CTString strHit = szHitEffect[iJob][nHitType]; // 히트 이펙트

						if (_pNetwork->GetWearingEffect(WEAR_WEAPON, 1).Length() > 0)
						{
							strMissile = _pNetwork->GetWearingEffect(WEAR_WEAPON, 1);
						}

						if (_pNetwork->GetWearingEffect(WEAR_WEAPON, 2).Length() > 0)
						{
							if (nHitType == 0)
							{
								strHit = _pNetwork->GetWearingEffect(WEAR_WEAPON, 2);
							}
						}

						if(!_pNetwork->m_bSingleMode)
						{// 일단 적을 구분 하지 말자
//							if(pen->IsEnemy())
//							{
								FLOAT3D vDelta = GetPlacement().pl_PositionVector - pen->GetPlacement().pl_PositionVector;
								vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.
								BOOL bAttack = CheckAttackTarget(-1, pen, vDelta.Length());

								if(bAttack)
								{
									switch(iAttackType)
									{
									case ATTACK1: case ATTACK3:
									case EXT_ATTACK1: case EXT_ATTACK3:
										{ // Left Attack
											ShotMissile(this, "LHAND", pen, 40.0f, strHit, strMissile, m_bCriticalDamage!=0, 0.0f, 0.0f, 2);
										}
										break;
									case ATTACK2: case ATTACK4:
									case EXT_ATTACK2: case EXT_ATTACK4:
										{ // Right Attack
											ShotMissile(this, "RHAND", pen, 40.0f, strHit, strMissile, m_bCriticalDamage!=0, 0.0f, 0.0f, 2);
										}
										break;
									}
								}
//							}
							// 적이 아닌 것을 공격하는 것은 처리하지 않았다.
						}
						else
						{
							for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
								it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
							{
								CEntity *pEn = (*it);
								if(pEn != NULL && pEn->IsFlagOn(ENF_ALIVE))
								{
									switch(iAttackType)
									{
									case ATTACK1: case ATTACK3:
									case EXT_ATTACK1: case EXT_ATTACK3:
										{ // Left Attack
											ShotMissile(this, "LHAND", pen, 40.0f, strHit, strMissile, m_bCriticalDamage!=0);
										}
										break;
									case ATTACK2: case ATTACK4:
									case EXT_ATTACK2: case EXT_ATTACK4:
										{ // Right Attack
											ShotMissile(this, "RHAND", pen, 40.0f, strHit, strMissile, m_bCriticalDamage!=0);
										}
										break;
									}
								}
							}
							m_dcEnemies.Clear();
						}
					}

					m_bCriticalDamage = FALSE;
				}
				break;
			default:
				break;
		}

		if( IsEXRogue(iJob) ) // EX_ROGUE 일때
		{
			// 석궁을 들었을 때....
			if( _pNetwork->MyCharacterInfo.bExtension )
			{
				if(en_pmiModelInstance != NULL && pen != NULL)
				{
					if(!_pNetwork->m_bSingleMode && pen->IsFlagOn(ENF_ALIVE))
					{
						if(pen->IsEnemy())
						{
							FLOAT3D vDelta = GetPlacement().pl_PositionVector - pen->GetPlacement().pl_PositionVector;
							vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.
							BOOL bAttack = CheckAttackTarget(-1, pen, vDelta.Length());
							if(bAttack)
							{
								ShotMissile(this, "RHAND", pen, 40.0f, szHitEffect[iJob][nHitType], "Normal Arrow Trace", m_bCriticalDamage!=0);
							}
						}
						else
						{
							ShotMissile(this, "RHAND", pen, 40.0f, szHitEffect[iJob][nHitType], "Normal Arrow Trace", m_bCriticalDamage!=0);
						}
					}
					else
					{
						for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
							it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
						{																			
							CEntity *pEn = (*it);
							if(pEn != NULL && pEn->IsFlagOn(ENF_ALIVE))
							{
								ShotMissile(this, "RHAND", pEn, 40.0f, szHitEffect[iJob][nHitType], "Normal Arrow Trace", m_bCriticalDamage!=0);
							}
						}
						m_dcEnemies.Clear();
					}
				}
				m_bCriticalDamage = FALSE;
			}
			// 단검을 들었을 때...
			else
			{
				if (nHitType > 0)
				{
					PlaySound(m_soMessage, DEF_SOUND_HOLYWATER_HIT, SOF_3D | SOF_VOLUMETRIC);
					StartEffectGroup(DEF_CAM_SHAKE, _pTimer->CurrentTick(), this->en_plPlacement.pl_PositionVector, FLOAT3D(0, 0, 0));
					m_bCriticalDamage = FALSE;
				}
				else
				{
					if(m_bCriticalDamage)
					{
						PlaySound(m_soMessage, GenderSound(SOUND_DAGGER_CRITICAL), SOF_3D | SOF_VOLUMETRIC);		
						m_bCriticalDamage = FALSE;
					}
					else
					{
						PlaySound(m_soMessage, GenderSound(SOUND_DAGGER_BLOW), SOF_3D | SOF_VOLUMETRIC);		
					}
				}
			}
		}
		

		if( IsEXMage(iJob) )	//2013/01/23 jeil EX메이지 체크 방식 수정 
		{
			if( en_pmiModelInstance != NULL && pen != NULL )
			{
				FLOAT3D		vStartPos;
				FLOATquat3D	qStartRot;
				CTag		*pTag = NULL;

				if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_1] )
				{
					pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "WAND" );
					if( pTag == NULL )
					{
						pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
					}
					if( pTag != NULL )
					{
						vStartPos = pTag->CurrentTagInfo().m_vPos;
						qStartRot = pTag->CurrentTagInfo().m_qRot;
					}
				}
				else if( idBodyAnim == idPlayerWhole_Animation[ANIM_ATTACK_2] )
				{
					pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "LHAND" );
					if( pTag != NULL )
					{
						vStartPos = pTag->CurrentTagInfo().m_vPos;
						qStartRot = pTag->CurrentTagInfo().m_qRot;
					}
				}
				else if( idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_1] )
				{
					pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
					if( pTag != NULL )
					{
						vStartPos = pTag->CurrentTagInfo().m_vPos;
						qStartRot = pTag->CurrentTagInfo().m_qRot;
					}
				}
				else if( idBodyAnim == idPlayerWhole_Animation[ANIM_EXT_ATTACK_2] )
				{
					pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
					if( pTag != NULL )
					{
						vStartPos = pTag->CurrentTagInfo().m_vPos;
						qStartRot = pTag->CurrentTagInfo().m_qRot;
					}
				}
				ASSERT( pTag != NULL );
				if( pTag != NULL )
				{
					if( !_pNetwork->m_bSingleMode && pen != NULL && pen->IsFlagOn(ENF_ALIVE))
					{
						if(pen->IsEnemy())
						{
							FLOAT3D vDelta = GetPlacement().pl_PositionVector - pen->GetPlacement().pl_PositionVector;
							vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.
							BOOL bAttack = CheckAttackTarget(-1, pen, vDelta.Length());
							if(bAttack)
							{
								ShotMagicContinued( this, vStartPos, qStartRot, pen, 35.0F,
									szHitEffect[iJob][nHitType], "mMissileEnerBall_G",
									m_bCriticalDamage != 0, 0 );	//2013/01/21 jeil 아크메이지 기본 공격 이펙트 교체
							}
						}
						else
						{
							ShotMagicContinued( this, vStartPos, qStartRot, pen, 35.0F,
								szHitEffect[iJob][nHitType], "mMissileEnerBall_G",
								m_bCriticalDamage != 0, 0 );	//2013/01/21 jeil 아크메이지 기본 공격 이펙트 교체
						}
					}
					else
					{
						for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
							it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
						{																			
							CEntity *pEn = (*it);
							if(pEn != NULL && pEn->IsFlagOn(ENF_ALIVE))
							{
								ShotMagicContinued( this, vStartPos, qStartRot, pEn, 35.0F,
									szHitEffect[iJob][nHitType], "mMissileEnerBall_G",
									m_bCriticalDamage != 0, 0 );	//2013/01/21 jeil 아크메이지 기본 공격 이펙트 교체
							}
						}
						m_dcEnemies.Clear();
					}
				}

				m_bCriticalDamage = FALSE;
			}
		}
	}
	
	// spawn teleport effect
	void SpawnTeleport(void)
	{
		// if in singleplayer
		if (GetSP()->sp_bSinglePlayer) {
			// no spawn effects
			return;
		}
		ESpawnEffect ese;
		ese.colMuliplier = C_WHITE|CT_OPAQUE;
		ese.betType = BET_TELEPORT;
		ese.vNormal = FLOAT3D(0,1,0);
		FLOATaabbox3D box;
		GetBoundingBox(box);
		FLOAT fEntitySize = box.Size().MaxNorm()*2;
		ese.vStretch = FLOAT3D(fEntitySize, fEntitySize, fEntitySize);
		CEntityPointer penEffect = CreateEntity(GetPlacement(), CLASS_BASIC_EFFECT, WLD_AUTO_ENTITY_ID, FALSE);
		penEffect->Initialize(ese,FALSE);
	}
	
	// render particles
	void RenderParticles(void)
	{
		if(m_bNotRenderParticles)//0809
		{
			return;
		}

		FLOAT tmNow = en_tmEntityTime;
		
		// render empty shells
		Particles_EmptyShells( this, m_asldData);
		//0321
		//if(_pInput->SetUKeyPressed(TRUE)){
			Particles_RunningDust(this);
		//}
		
		(FLOAT&)m_fLastBurnTime = (FLOAT)_pTimer->GetHighPrecisionTimer().GetSeconds();
		if(m_fBurnLeftTime > 0)
		{
			FLOAT fRatio = 1.0f;
			if(m_fBurnLeftTime < m_fBurnFadeInTime)
			{
				fRatio = m_fBurnLeftTime / m_fBurnFadeInTime;
			}
			if(m_fBurnTime - m_fBurnLeftTime < m_fBurnFadeOutTime)
			{
				fRatio = (m_fBurnTime - m_fBurnLeftTime) / m_fBurnFadeOutTime;
			}
			fRatio *= m_fBurnRatio;
			Particles_Burning(this, m_fBurnPower, fRatio);
			FLOAT fCurrentTime = (FLOAT)_pTimer->GetHighPrecisionTimer().GetSeconds();
			m_fBurnLeftTime -= fCurrentTime - m_fLastBurnTime;
			if(m_fBurnLeftTime < 0.0f)
			{
				(FLOAT&)m_fBurnLeftTime = 0.0f;
			}
			(FLOAT&)m_fLastBurnTime = fCurrentTime;
		}
		
		if (Particle_GetViewer()==this) {
			Particles_ViewerLocal(this);
		}
		else
		{
			// if is not first person
			RenderChainsawParticles(TRUE);
			// glowing powerups
			if (GetFlags()&ENF_ALIVE){
				if (m_tmSeriousDamage>tmNow && m_tmInvulnerability>tmNow) {
					Particles_ModelGlow(this, Max(m_tmSeriousDamage,m_tmInvulnerability),PT_STAR08, 0.15f, 2, 0.03f, 0xff00ff00);
				} else if (m_tmInvulnerability>tmNow) {
					//0318
					//  Particles_ModelGlow(this, m_tmInvulnerability, PT_STAR05, 0.15f, 2, 0.03f, 0x3333ff00);
					Particles_ModelGlow(this, m_tmInvulnerability, PT_STAR05, 0.1f, 7, 0.03f, 0xff777700);
				} else if (m_tmSeriousDamage>tmNow) {
					//0318
					Particles_ModelGlow(this, m_tmSeriousDamage, PT_STAR08, 0.1f, 5, 0.03f, 0xff777700);
					//Particles_ModelGlow(this, m_tmSeriousDamage, PT_STAR08, 0.15f, 2, 0.03f, 0xff777700);
				}
				//if (m_tmSeriousSpeed>tmNow) {
				if (m_bConnectEffect && m_penStillTarget != NULL)
				{
					if (m_penStillTarget->en_RenderType == RT_SKAMODEL && m_penStillTarget->GetFlags() & ENF_ALIVE)
					{
						CTag* pSource_Tag = en_pmiModelInstance->m_tmSkaTagManager.Find("LHAND");
						CTag* pDest_Tag = m_penStillTarget->en_pmiModelInstance->m_tmSkaTagManager.Find("CENTER");

						Particles_Ghostbuster(pSource_Tag->CurrentTagInfo().m_vPos, pDest_Tag->CurrentTagInfo().m_vPos, 3, 1.0f, 1.0f, 33.3333333f
							, &m_pSkillTexture);
					}
					else
					{
						m_bConnectEffect = FALSE;
						m_pSkillTexture.SetData(NULL);
						StopEffectGroupIfValid(m_pAttachPlayerEffect, 0.1f);
						StopEffectGroupIfValid(m_pAttachEnemyEffect, 0.1f);
					}
				}

				if(m_bSpeedUp)
				{
					Particles_RunAfterBurner(this, m_tmSeriousSpeed, 0.3f, 0);
				}
				if (!GetSP()->sp_bCooperative) {
					CPlayerWeapons *wpn = GetPlayerWeapons();
					/*
					if (wpn->m_tmLastSniperFire == _pTimer->CurrentTick())
					{
						CAttachmentModelObject &amoBody = *GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO);
						FLOATmatrix3D m;
						MakeRotationMatrix(m, amoBody.amo_plRelative.pl_OrientationAngle);
						FLOAT3D vSource = wpn->m_vBulletSource + FLOAT3D(0.0f, 0.1f, -0.4f)*GetRotationMatrix()*m;
						Particles_SniperResidue(this, vSource , wpn->m_vBulletTarget);
					}
					*/
				}
			}
		}
		
		// spirit particles
		if( m_tmSpiritStart != 0.0f)
		{
			Particles_Appearing(this, m_tmSpiritStart);
		}
	}
	
	void TeleportToAutoMarker(CPlayerActionMarker *ppam) 
	{
		// if we are in coop
		if (GetSP()->sp_bCooperative && !GetSP()->sp_bSinglePlayer) {
			// for each player
			for(INDEX iPlayer=0; iPlayer<GetMaxPlayers(); iPlayer++) {
				CPlayer *ppl = (CPlayer*)GetPlayerEntity(iPlayer);
				if (ppl!=NULL) {
					// put it at marker
					CPlacement3D pl = ppam->GetPlacement();
					FLOAT3D vOffsetRel = ppl->GetTeleportingOffset();
					pl.pl_PositionVector += vOffsetRel*ppam->en_mRotation;
					ppl->Teleport(pl, FALSE);
					// remember new respawn place
					ppl->m_vDied = pl.pl_PositionVector;
					ppl->m_aDied = pl.pl_OrientationAngle;
				}
			}
		// otherwise
		} else {
			// put yourself at marker
			CPlacement3D pl = ppam->GetPlacement();
			FLOAT3D vOffsetRel = GetTeleportingOffset();
			pl.pl_PositionVector += vOffsetRel*ppam->en_mRotation;
			Teleport(pl, FALSE);
		}
	}
	
	// check whether this time we respawn in place or on marker
	void CheckDeathForRespawnInPlace(EPlayerDeath ePLDeath)
	{    
		// if respawning in place is not allowed
		if (!GetSP()->sp_bRespawnInPlace) {
			// skip further checks
			return;
		}
		// if killed by a player or enemy
		CEntity *penKiller = (CEntity*)ePLDeath.eidInflictor;
		if( IsOfClass( penKiller, &CPlayer_DLLClass)
			|| IsDerivedFromClass( penKiller, &CEnemyBase_DLLClass))
		{
			// mark for respawning in place
			m_ulFlags |= PLF_RESPAWNINPLACE;
			m_vDied = GetPlacement().pl_PositionVector;
			m_aDied = GetPlacement().pl_OrientationAngle;
		}
	}
	
	//0701	
	//0105 이동루틴 이식
	// set desired rotation and translation to go/orient towards desired position
	// and get the resulting movement type
	virtual ULONG SetDesiredMovement()
	{
		ULONG ulFlags = 0;
		//float m_fMoveFrequency = 0.1f; ///추가
		//float m_fMoveSpeed = 10.0f;
		//ANGLE m_aRotateSpeed = 360.0f;//18000.0f;//360.0f;
		BOOL m_bFirstRotate = TRUE; //회전먼저..
		
		// get delta to desired position
		FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;
		FLOAT3D vDeltaTmp = vDelta;
		
		if( GetPlayerWeapons()->m_penRayHitTmp != NULL && 
			GetPlayerWeapons()->m_penRayHitTmp == this )
		{
			// [2010/09/03 : Sora] 자신을 타겟했을때 키보드로 이동하는 경우 체크 수정
			if(!m_bKeyMove)
			{
				return 0;
			}
		}

		CEntity *penEnt = GetPlayerWeapons()->m_penRayHitTmp;
		CEntity *penTarget = GetPlayerWeapons()->m_penRayHitTmp;

		CUIManager* pUIManager = SE_Get_UIManagerPtr();

		// 내 소환수나 애완동물일 경우, 이동은 계속함...
		if( penTarget != NULL && ( penTarget->GetFirstExFlags() & ( ENF_EX1_CURRENT_PET | ENF_EX1_CURRENT_SLAVE | ENF_EX1_CURRENT_WILDPET) ) )
		{
			if((m_bForward||m_bKeyMove) && (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)) - tmStartTime > 1000)
			{
				SendMyNextMovePosition(vDelta);
			}
		}
		// FIXME : 엄청난 정리가 필요함...-_-;;;
		else if( penTarget != NULL && 
			(((penTarget->IsCharacter()) && 
			(((IsPvp() ||  _pUISWDoc->IsWar() || IsLegitTarget(penEnt) || 
			pUIManager->GetGuildBattle()->IsEnemy( penTarget->en_ulID )) && vDelta.Length() < 2.0f) || 
			(!IsPvp() && !(pUIManager->GetGuildBattle()->IsEnemy( penTarget->en_ulID )) && vDelta.Length() < 1.0f))) 
			|| vDelta.Length() < 0.5f))
		{
			m_vDesiredPosition = GetPlacement().pl_PositionVector;
			StopRotating();
			StopTranslating();			
			
			g_bIsRotateEnd = FALSE;
			
			//0317
			if((m_bForward||m_bKeyMove) && _cmiComm. IsNetworkOn())
			{
				StopMove();	
			}
			return 0.0f;
		}	
		else //1초가 지났다면 다음 포인트를 보낸다.
		{
			// NOTE : 바닥을 찍었을때나... 머 그럴때?
			if((m_bForward||m_bKeyMove))
			{
				if ((unsigned int(_pTimer->GetLerpedCurrentTick()*1000)) - tmStartTime > 1000)
				{
					SendMyNextMovePosition(vDelta);
				}
			}
		}
	
		if (!(m_bForward||m_bKeyMove))
		{ // 움직임이 멈추면 현재 위치로 갱신
			m_vDesiredPosition = GetPlacement().pl_PositionVector;
			m_vMyPositionTmp = GetPlacement().pl_PositionVector;
		}

		ANGLE aWantedHeadingRelative;
		
		// if we may rotate
		if (m_aRotateSpeed>0.0f && m_bFirstRotate) 
		{
			FLOAT3D vDir;			
/*			if(g_bFirstRotate)
			{
				vDir = g_bTargetDirection;
			}
			else*/
			{						
				vDir = vDelta;
			}
			vDir.SafeNormalize();
			
			aWantedHeadingRelative = GetRelativeHeading(vDir);
			
			// normalize it to [-180,+180] degrees
			aWantedHeadingRelative = NormalizeAngle(aWantedHeadingRelative);
			
			ANGLE aHeadingRotation;
			// if desired position is left
			if (aWantedHeadingRelative < -m_aRotateSpeed*m_fMoveFrequency) 
			{
				// start turning left
				aHeadingRotation = -(m_aRotateSpeed*m_fMoveFrequency);
				// if desired position is right
			} else if (aWantedHeadingRelative > m_aRotateSpeed*m_fMoveFrequency) 
			{
				// start turning right
				aHeadingRotation = +(m_aRotateSpeed*m_fMoveFrequency);
				// if desired position is more-less ahead
			} 
			else 
			{
				// keep just the adjusting fraction of speed 
				aHeadingRotation = aWantedHeadingRelative/m_fMoveFrequency;
			}
			//////////

			m_aHeadingRotation = aHeadingRotation;

			//CPrintF("PRE_ROTATE : %f \n",en_plViewpoint.pl_OrientationAngle(1));
			// start rotating
			SetDesiredRotation(ANGLE3D(aHeadingRotation, 0, 0));		
			
			if (Abs(aHeadingRotation)>1.0f) 
			{
				ulFlags |= MF_ROTATEH;
			}
			
			//0625 kwon
			//if (Abs(aHeadingRotation)<1.0f) {
			if(Abs(aWantedHeadingRelative)<=1.0f)
			{
				m_bFirstRotate = FALSE;
				
				//0625 kwon
				g_bIsRotating = FALSE;
				g_bIsRotateEnd = TRUE;			
				if(g_bFirstRotate)
				{
					g_bFirstRotate = FALSE;
				}
				
				return 0; //여기서 리턴.								
			}
			//0625 kwon
			g_bIsRotating = TRUE;
			
			// if we may not rotate
		}
		else 
		{
			// stop rotating
			SetDesiredRotation(ANGLE3D(0, 0, 0));
		}
		
		//0625 kwon // eons 여기도 필요 없다.
		/*if (m_fMoveSpeed>0.0f && !g_bIsRotateEnd)
		{
			aWantedHeadingRelativeTmp = aWantedHeadingRelative;
			FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;
			
			vDelta(2) = 0;
			vDelta.Normalize();
			
			ANGLE AngleRotate = GetPlacement().pl_OrientationAngle(1);			
			ANGLE radian = AngleRotate*PI/180.0f;			
			
			vDelta(1) = cos((90-fabs(aWantedHeadingRelativeTmp))*PI/180.0f);
			//MoveY = vDelta(2);
			vDelta(3) = sin((90-fabs(aWantedHeadingRelativeTmp))*PI/180.0f);

			// NOTE : g_vMove는 각도를 계산하기 위해서 필요한 값인듯...
			
			vDelta.Normalize();
			g_vMove = vDelta;
			
		}*/

		// if we may move
		if (m_fMoveSpeed>0.0f && !m_bFirstRotate) 
		{
			ulFlags |= MF_MOVEZ;			
		} 
		// if we may not move
		else 
		{
			// stop translating
			SetDesiredTranslation(FLOAT3D(0, 0, 0));
		}
		
		return ulFlags;
	}

	// FIXME : 아래 코드 엄청난 정리가 필요함ㅡ_ㅡ;;;
	//0701 kwon
	BOOL CheckTarget()
	{
		//	if(움직여야 한다면)return TRUE;
		//	else	return FALSE;	

		if(m_bStuned) {return FALSE;}

		if (m_bDying)
		{
			return FALSE;
		}

		static BOOL bInit = FALSE;
		if(!bInit)
		{
			m_vDesiredPosition = GetPlacement().pl_PositionVector;
			bInit=TRUE;
		}
		
		if(ulNewButtons&PLACT_FIRE && !m_bLockMove && !m_bHold)
		{
			((CPlayerView*)&*m_penView)->UnlockRotate();
			if(_pNetwork->IsPlayerLocal(this))
			{
				m_bForward = TRUE;
				//m_bKeyMove = TRUE;
			}		
		}

		CUIManager* pUIManager = SE_Get_UIManagerPtr();

		CEntity *penTarget = GetPlayerWeapons()->m_penRayHitTmp;

		if (m_penClickTarget != penTarget)
		{
			if (pUIManager->DoesMessageBoxExist(MSGCMD_CONFIRM_OBJECTCLICK))
			{
				pUIManager->CloseMessageBox(MSGCMD_CONFIRM_OBJECTCLICK);
			}

			m_penClickTarget.ep_pen = NULL;
		}
		
		if(m_nCurrentSkillNum != -1)
		{
			CSkill &SkillData = _pNetwork->GetSkillData(m_nCurrentSkillNum);//0807
			if(SkillData.GetTargetType() == CSkill::STT_SELF_ONE
			|| SkillData.GetTargetType() == CSkill::STT_SELF_RANGE
			|| SkillData.GetTargetType() == CSkill::STT_PARTY_ALL
			|| SkillData.GetTargetType() == CSkill::STT_GUILD_ALL
			|| SkillData.GetTargetType() == CSkill::STT_GUILD_SELF_RANGE
			|| SkillData.GetTargetType() == CSkill::STT_GUILD_MEMBER_SELF)
			{
				return FALSE;
			}
		}

		if(penTarget == this) //0806 타겟이 자신일경우.
		{
			// [2010/09/03 : Sora] 자신을 타겟했을때 키보드로 이동하는 경우 체크 수정
			if(!m_bKeyMove)
			{
				return FALSE;
			}
		}		

		if(penTarget != NULL)
		{
			// 내 소환수나 애완동물일 경우, 이동은 계속함... (소환 NPC 추가 : 몬스터 용병, 토템, 트랩, 패러사이트)
			if( penTarget->GetFirstExFlags() & ( ENF_EX1_CURRENT_PET | ENF_EX1_CURRENT_SLAVE | ENF_EX1_CURRENT_WILDPET | ENF_EX1_TOTEM | ENF_EX1_TRAP | ENF_EX1_SUICIDE ) )
			{
				return TRUE;
			}

			//몬스터라면,
			if( penTarget->GetFlags()&ENF_ALIVE && penTarget->IsEnemy()
				&& !(penTarget->IsFirstExtraFlagOn(ENF_EX1_NPC))
				&& !(penTarget->IsFirstExtraFlagOn(ENF_EX1_PRODUCTION))
				&& !(_pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_FAKEDEATH)) //데스모션 중이아니라면
				//&& !(penTarget->IsFirstExtraFlagOn(ENF_EX1_CURRENT_PET))
				)
			{
				CMobData* pMD = CMobData::getData(((CEnemy*)penTarget)->m_nMobDBIndex);

				if (pMD == NULL)
				{
					return TRUE;
				}

				// 떠있을 때에는 키보드 움직임만 활성화. [8/21/2009 rumist]
				if( _pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING && m_bKeyMove )
				{
					return TRUE;
				}

				// RVR존이고 내 결사대 타입과 몬스터 결사대 타입이 같다면
				if (_pNetwork->IsRvrZone() == TRUE && 
					_pNetwork->MyCharacterInfo.iSyndicateType == pMD->GetSyndicateType())
				{
					// 'W', 'S'로 이동가능 하게~
					if (m_bKeyMove == TRUE)
					{
						return TRUE;
					}
					// 그게 아니라면 멈춰랏!
					else
					{
						StopMove();
						return FALSE;
					}
				}
				
				if( (penTarget->IsFirstExtraFlagOn(ENF_EX1_MONSTER_MERCENARY))	|| (penTarget->IsFirstExtraFlagOn(ENF_EX1_TOTEM_ITEM)))
				{
					BOOL isAttackCheck = TRUE;

					if (IsPvp() == FALSE)
					{
						isAttackCheck = FALSE;
					}
					else
					{
						INDEX ownerIndex = 0;
						ObjectBase* pObject = ACTORMGR()->GetObjectByCIndex(eOBJ_MOB, penTarget->en_ulID);

						if (pObject != NULL)
						{
							CMobTarget* pMT = static_cast< CMobTarget* >(pObject);
							ownerIndex = pMT->mob_iOwnerIndex;
						}
												
						if (ownerIndex == _pNetwork->MyCharacterInfo.index)
						{
							isAttackCheck = FALSE;
						}
					}

					if (isAttackCheck == FALSE)
					{
						// 'W', 'S'로 이동가능 하게~
						if (m_bKeyMove == TRUE)
						{
							return TRUE;
						}
						else
						{
							StopMove();
							return FALSE;
						}
					}
				}

				CancelProduct();

				FLOAT3D vDelta = GetPlacement().pl_PositionVector - penTarget->GetPlacement().pl_PositionVector;
				vDelta(2) = 0.0f;
				FLOAT fScaledSize = pMD->GetScaledSize();
				
				const int iJob = en_pcCharacter.pc_iPlayerType;

				// [100107: selo] MyCharacterInfo.attackrange 대치
				float fAttackDistance = GetAttackrange();				
				fAttackDistance += fScaledSize;

				// FIXME : 코드 중복이 심함.
				if( vDelta.Length() < fAttackDistance && !IsAttacking() && m_nCurrentSkillNum==-1)
				{	
					if(ulNewButtons&PLACT_FIRE && (m_bForward))
					{
						m_bForward = FALSE;
					}			
					StopMove();

					if(!IsVisibleCheckAll(penTarget))
					{
						CTString strSysMessage;
						strSysMessage.PrintF(_S( 2086, "가로막혀서 공격할수 없습니다." ));	// 번역
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);	

						GetPlayerWeapons()->m_penReservedTarget = GetPlayerWeapons()->m_penRayHitTmp;
						GetPlayerWeapons()->m_penRayHitTmp = NULL;	

						return FALSE;
					}
					else
					{
						BOOL bAttack = CheckAttackTarget( -1, penTarget, vDelta.Length() );
						if(bAttack)
						{
							StartAttack();
						}
						else
						{
							StopAttack();
						}						
					}
					
					return FALSE;//SetDesiredMovement()실행 안함.
				}
				else if(IsAttacking())
				{
					Rotate(-vDelta, 1800.0f, TRUE);//360->1800
					return FALSE;
				}
				else
				{
					if(IsAttacking())//공격중에 몬스터가 이동해서 멀어졌다면,
					{
						StopAttack();						
					}

					if(vDelta.Length() >= 2.0f && !m_bHold)
					{
						if(!(m_bForward||m_bKeyMove))
						{
							tmStartTime = 0;
						}

						m_bForward = TRUE;			
						//m_bKeyMove = TRUE;
						m_vDesiredPosition = penTarget->GetPlacement().pl_PositionVector;
						//1230 여기서 move메시지를 보내야 한다.
						FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;
					
						if((m_bForward||m_bKeyMove) && (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)) - tmStartTime > 1000)
						{
							SendMyNextMovePosition(vDelta);
						} 
			
						//몬스터로 이동
						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}			
			}
			// 자원을 클릭했을때...
			else if( penTarget->IsFirstExtraFlagOn(ENF_EX1_PRODUCTION) )//1128		
			{
				// Mob Index를 얻습니다.
				CDLLEntityClass *pdecDLLClass	= penTarget->GetClass()->ec_pdecDLLClass;
				CEntityProperty &epProperty		= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 91);		// Mob Index
				const int iMobIndex				= ENTITYPROPERTY( &*penTarget, epProperty.ep_slOffset, INDEX);
				if(iMobIndex == -1)
				{	
					return TRUE;
				}

				CMobData* MD					= CMobData::getData(iMobIndex);
				FLOAT3D vDelta					= GetPlacement().pl_PositionVector - penTarget->GetPlacement().pl_PositionVector;
				vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.

				if( vDelta.Length() > (MD->GetAttackDistance() * 0.9f) && _cmiComm. IsNetworkOn())
				{
					return TRUE;
				}
			
				if(penTarget != NULL && penTarget->IsEnemy() && (penTarget->GetFlags()&ENF_ALIVE))
				{					
					Rotate(-vDelta, 360.0f, TRUE);
				}
						
				if(m_bForward)
				{	
					if(ulNewButtons&PLACT_FIRE && (m_bForward))
					{
						m_bForward = FALSE;
					}
																			
					StopMove();

					// 수집퀘스트용 npc일때...
					if( MD->IsCollectQuest())
					{							
						m_bProduction		= TRUE;
						m_nProductionNum	= 4;
						pUIManager->GetSelectResource()->OpenSelectResource(penTarget->en_ulID, m_nProductionNum);
						return TRUE;
					}

					// FIXME : 이렇게 체크하는 것보다, 서버에서 확인을 받은 다음에 해주는게 좋지 않을까???
					if(_pNetwork->MyWearItem[WEAR_SHIELD].IsEmptyItem() == FALSE)
					//if(_pNetwork->MyCurrentWearing[WEAR_SHIELD].Item_Index != -1)
					{
						CTString strSysMessage;
						strSysMessage.PrintF( _S( 696, "방패를 장착하고서 생산을 할 수 없습니다." ));		
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );					
						return TRUE;
					}

					// 애완동물을 탄 상태로 생산을 할수 없음.
					// if you riding wild pet, can't product items. [12/12/2010 rumist]
					if(m_bRide || m_bWildRide )
					{
						CTString strSysMessage;
						strSysMessage.PrintF(_S( 3074,"애완동물을 타고서 생산을 할 수 없습니다." ));		// 번역
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );					
						return TRUE;
					}

					int			nProduceType;
					CTString	strItemName;		// 생산물에 필요한 아이템 이름

					if ( MD->IsEnergy() )
					{
						nProduceType  = CItemData::ITEM_WEAPON_CHARGE;
					}
					else if ( MD->IsCrops() )
					{
						nProduceType = CItemData::ITEM_WEAPON_GATHERING;
					}
					else if ( MD->IsMineral() )
					{
						nProduceType  = CItemData::ITEM_WEAPON_MINING;
					}
		
					// FIXME : 미리 알고 있으면 루프를 돌 필요가 없는 부분임.
					CItemData::_map::iterator	iter = CItemData::getmapPtr()->begin();
					CItemData::_map::iterator	eiter = CItemData::getmapPtr()->end();

					for ( int i = 0; iter != eiter; ++iter)
					{
						CItemData* pItemData = (*iter).second;
						if (pItemData == NULL)
						{
							continue;
						}

						const char* szItemName = _pNetwork->GetItemName( pItemData->getindex() );
						
						if ( pItemData->GetType() == CItemData::ITEM_WEAPON )
						{
							if ( pItemData->GetSubType() == nProduceType )
							{
								if( pItemData->GetJob() & ( 1 << _pNetwork->MyCharacterInfo.job ) )
								{
									strItemName = szItemName;
								}
							}
						}
					}

					//const int iWeaponType = _pNetwork->MyCurrentWearing[WEAR_WEAPON].pItemData->GetSubType();
					if(_pNetwork->MyWearItem[WEAR_WEAPON].IsEmptyItem() == TRUE)
					{
						CTString strSysMessage;
						strSysMessage.PrintF( _S( 697, "생산도구를 장착하지 않았습니다." ) );		
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );	
						
						strSysMessage.PrintF( _S( 788, "%s를 착용하여 주십시오," ), strItemName );		
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );										
																
						return TRUE;
					}

					const int iWeaponType = _pNetwork->MyWearItem[WEAR_WEAPON].ItemData->GetSubType();

					// 생산도구를 장착하구 있을때...
					if(!(iWeaponType == CItemData::ITEM_WEAPON_MINING || 
						iWeaponType == CItemData::ITEM_WEAPON_GATHERING || 
						iWeaponType == CItemData::ITEM_WEAPON_CHARGE))
					{
						CTString strSysMessage;
						strSysMessage.PrintF ( _S( 697, "생산도구를 장착하지 않았습니다." ) );		
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );					

						strSysMessage.PrintF( _S( 788, "%s를 착용하여 주십시오," ), strItemName );		
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );										

						return TRUE;
					}	
					else
					{						
						// FIXME : 맘에 들지 않는 부분.
						// FIXME : 생산 관련되서 인덱스가 일치될 부분이 필요한듯...
						// 채굴 도구이면서 광물일때...
						if((iWeaponType == CItemData::ITEM_WEAPON_MINING) && MD->IsMineral())
						{
							m_bProduction		= TRUE;
							m_nProductionNum	= 1;
							pUIManager->GetSelectResource()->OpenSelectResource(penTarget->en_ulID, m_nProductionNum);
						}
						// 채집 도구이면서 작물일때...
						else if((iWeaponType == CItemData::ITEM_WEAPON_GATHERING) && MD->IsCrops())
						{							
							m_bProduction		= TRUE;
							m_nProductionNum	= 2;
							pUIManager->GetSelectResource()->OpenSelectResource(penTarget->en_ulID, m_nProductionNum);
						}
						// 차지 도구이면서 에너지일때...
						else if((iWeaponType == CItemData::ITEM_WEAPON_CHARGE) && MD->IsEnergy())
						{							
							m_bProduction		= TRUE;
							m_nProductionNum	= 3;
							pUIManager->GetSelectResource()->OpenSelectResource(penTarget->en_ulID, m_nProductionNum);
						}
						else
						{
							CTString strSysMessage;
							strSysMessage.PrintF( _S( 701, "생산도구가 올바르지 않습니다." ) );		
							_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );			

							strSysMessage.PrintF( _S( 788, "%s를 착용하여 주십시오," ), strItemName );		
							_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );										
							
							/*// Date : 2005-01-10,   By Lee Ki-hwan
							CTString strSysMessage;
							strSysMessage.PrintF( _S( 701, "생산도구가 올바르지 않습니다." ) );		
								_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );			
							*/		
							return TRUE;												
						}
					}
				}
				
				return FALSE;
			}
			else if (penTarget->IsFirstExtraFlagOn(ENF_EX1_CLICK_OBJECT) ) // 클릭 오브젝트일 경우
			{
				FLOAT3D vDelta = GetPlacement().pl_PositionVector - penTarget->GetPlacement().pl_PositionVector;
				vDelta(2) = 0.0f;

				FLOATaabbox3D box;
				penTarget->GetBoundingBox(box);
				FLOAT fBBoxLength = box.Size().Length() / 2 + 1.0f;

				// 키보드 터닝시 disconnected 막기 위한 코드. [11/18/2009 rumist]
				if( m_bKeyMove && pUIManager->DoesMessageBoxExist(MSGCMD_CONFIRM_OBJECTCLICK) )
				{
					pUIManager->CloseMessageBox(MSGCMD_CONFIRM_OBJECTCLICK);
					m_penClickTarget = NULL;
					return TRUE;
				}

				if (pUIManager->DoesMessageBoxExist(MSGCMD_CONFIRM_OBJECTCLICK) && (!m_bSendStopMessage) )
				{
					StopMove();
				}

				// 조건 체크 변경. [11/18/2009 rumist]
				if ( vDelta.Length() <= fBBoxLength && !pUIManager->DoesMessageBoxExist(MSGCMD_CONFIRM_OBJECTCLICK)
					&& m_penClickTarget != penTarget &&  _pInput->IsLMouseButtonPressed() )
				{
					StopMove();
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_YESNO, UI_NONE, MSGCMD_CONFIRM_OBJECTCLICK);
					MsgBoxInfo.AddString(_S(4683, "클릭 하시겠습니까?"));
					
					pUIManager->CreateMessageBox(MsgBoxInfo);
					m_penClickTarget = penTarget;
				}
				else if (vDelta.Length() > fBBoxLength )
				{
					if (pUIManager->DoesMessageBoxExist(MSGCMD_CONFIRM_OBJECTCLICK))
					{
						pUIManager->CloseMessageBox(MSGCMD_CONFIRM_OBJECTCLICK);
					}
					m_penClickTarget = NULL;
				}

				return TRUE;
			}
			else if(penTarget->GetFlags()&ENF_ITEM && !(_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING))
			{
				CancelProduct();

//				FLOAT3D vDelta = GetPlacement().pl_PositionVector - ((CViewingItems*)penTarget)->GetPlacement().pl_PositionVector;
				
				FLOAT3D vDelta = FLOAT3D(1000.0f,0.0f,0.0f);

				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_ITEM, penTarget->GetNetworkID());

				if (pObject != NULL)
				{
					CItemTarget* pTarget = static_cast< CItemTarget* >(pObject);

					SBYTE	sbLayerDiff = pObject->GetyLayer() - _pNetwork->MyCharacterInfo.yLayer;

						if( sbLayerDiff < -1 || sbLayerDiff > 1 )
						{
						CTString strSysMessage;
							strSysMessage.PrintF( _S( 1007, "현재의 위치에서 줍지 못하는 아이템입니다." ));
						_pNetwork->ClientSystemMessage( strSysMessage );

							if(ulNewButtons&PLACT_FIRE && (m_bForward))
							{
								m_bForward = FALSE;
							}

							StopMove();
							GetPlayerWeapons()->m_penRayHitTmp = NULL;
							return FALSE;
						}

					vDelta = GetPlacement().pl_PositionVector - pTarget->item_place;
				}				
				
				vDelta(2) = 0.0f; //0131 높이값 무시.
				
				//Picking할수 있는 거리
				if( vDelta.Length() <= 2.0f)
				{
					if(ulNewButtons&PLACT_FIRE && (m_bForward))
					{
						m_bForward = FALSE;
					}
					
					StopMove();
					
					/* [2011/12/29 : Sora] 데스모션 중에 액션을 사용 하면 캐릭터가 움직이지 않는 버그 확인
						기획측과 협의하여 데스모션 중에는 액션을 사용할 수 없도록 수정 */
					if( _pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_FAKEDEATH) )
					{
						return FALSE;
					}

					// 아이템을 주울수 있는 거리라면 아이템을 줍습니다.
					m_bPlayAction = TRUE;
					m_nPlayActionNum = ACTION_NUM_PICKITEM;
					PickItem(penTarget->GetNetworkID());
					GetPlayerWeapons()->m_penRayHitTmp = NULL;
					
					return FALSE;
				}
				//아이템으로 이동
				else
				{
					return TRUE;
				}
			}
			// server disconnect bug fix.[11/27/2009 rumist]
			// 선택후 회전시 지속적인 stop 메시지 호출로 인한 버그 수정.
			// 키보드시에는 들어오지 않는 루틴이 됨.
			else if((penTarget->GetFlags()&ENF_ALIVE) && (penTarget->IsCharacter()) && !m_bKeyMove )//캐릭터를 찍었을 경우.
			{
				CancelProduct();
				
				FLOAT3D vDelta = GetPlacement().pl_PositionVector - ((CCharacterBase*)penTarget)->GetPlacement().pl_PositionVector;
				vDelta(2) = 0.0f;
				
				if(!_pNetwork->IsRvrZone() && !IsPvp() && !_pUISWDoc->IsWar() && !IsLegitTarget(penTarget) && !(pUIManager->GetGuildBattle()->IsEnemy( penTarget->en_ulID )) //1203
					&& !pUIManager->GetSiegeWarfareNew()->GetWarState() ) // WSS_DRATAN_SEIGEWARFARE 2007/08/30 드라탄 공성 추가
				{
					// TO-KR-T20090903-003 Bug Fix. [9/4/2009 rumist]
					if(vDelta.Length() < 1.0f && !m_bSendStopMessage )//캐릭터와의 거리.
					{
						if(ulNewButtons&PLACT_FIRE && (m_bForward) )
						{
							m_bForward = FALSE;
						}
						
						StopMove();
						
						// 스톱시 움직임 방지.
						m_bClicked = false;
						
						SLONG	cha_index = -1;
						SBYTE	cha_shoptype = 0;

						ObjectBase* pObject = ACTORMGR()->GetObjectByCIndex(eOBJ_CHARACTER, penTarget->en_ulID);

						if (pObject != NULL)
						{
							CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

							cha_index		= pTarget->m_nIdxServer;
							cha_shoptype	= pTarget->cha_sbShopType;
						}


						if(cha_shoptype != PST_NOSHOP)
						{
							if(_pNetwork->MyCharacterInfo.sbShopType == PST_NOSHOP)
							{
								pUIManager->GetPersonalShop()->TradePersonalShop(cha_index, 0.0f, 0.0f, TRUE);
								GetPlayerWeapons()->m_penRayHitTmp = NULL;
								return TRUE;
							}
						}
						return FALSE;
					}
					else if(!m_bHold)
					{								
						if(vDelta.Length() >= 1.0f && !(m_bSkilling && m_bStartAttack))
						{
							//1230 여기서 move메시지를 보내야 한다.
							if(!(m_bForward))
							{
								tmStartTime = 0;
							}
							// 특정 거리 이하일 경우엔 들어와도 무시.
							if( vDelta.Length() < 8.0f && !m_bClicked && !( ulNewButtons&PLACT_FIRE ) )
							{
								return FALSE;
							}
							// 특정거리를 벋어나면 움직이는 모션 실시.
							// 이때는 지속적으로 스톱 메시지가 가기 전까지는 무조건 움직인다.
							m_bForward = TRUE;	
							m_bClicked = true;
							//m_bKeyMove = TRUE;
							m_vDesiredPosition = penTarget->GetPlacement().pl_PositionVector;
							
							FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;
							
							if((m_bForward) && (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)) - tmStartTime > 1000)
							{
								SendMyNextMovePosition(vDelta);
							} 
							
							
							return TRUE;//m_vDesiredPosition으로.
						}
						// 거리가 특정치 이하일때 클릭이벤트가 발생하면 계속적으로 달리는 애니메이션
						// 출력 버그가 있어 수정. //  [9/4/2009 rumist]
						else if( vDelta.Length() < 1.0f && !(m_bSkilling && m_bStartAttack) /*&& !m_bSendStopMessage*/ )
						{
							StopMove();						
							m_bClicked = false;
							return FALSE;
						}
						// 마찬가지로 따라가기시에 특정 거리를 벋어나면 스톱시킴.
						//  [9/4/2009 rumist]
						else if( vDelta.Length() > 30.0f && !(m_bSkilling && m_bStartAttack) /*&& !m_bSendStopMessage*/ )
						{
							StopMove();
							m_bClicked = false;
							return FALSE;
						}
					}
				}
				else//PVP라면,
				{
					// [100107: selo] MyCharacterInfo.attackrange 대치
					float fAttackDistance = GetAttackrange();
					const int iJob = en_pcCharacter.pc_iPlayerType;					

					// FIXME : 코드 중복이 심함.
					if( vDelta.Length() < fAttackDistance && !IsAttacking() && m_nCurrentSkillNum==-1)
					{	
						if(ulNewButtons&PLACT_FIRE && (m_bForward))
						{
							m_bForward = FALSE;
						}
									
						StopMove();		
						if(!IsVisibleCheckAll(penTarget))
						{
							CTString strSysMessage;
							strSysMessage.PrintF(_S( 2086, "가로막혀서 공격할수 없습니다." ));	// 번역
							_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);	

							GetPlayerWeapons()->m_penReservedTarget = GetPlayerWeapons()->m_penRayHitTmp;
							GetPlayerWeapons()->m_penRayHitTmp = NULL;	

							return FALSE;
						}	
						else
						{
							BOOL bAttack = CheckAttackTarget( -1, penTarget, vDelta.Length() );
							if(bAttack)
							{
								StartAttack();							
							}
							else
							{
								StopAttack();
							}								
						}
					
						return FALSE;//SetDesiredMovement()실행 안함.
					}
					else if(IsAttacking())
					{
						Rotate(-vDelta, 1800.0f, TRUE);//360->1800
						return FALSE;
					}
					else if(!m_bHold)
					{
						if(IsAttacking())//공격중에 캐릭터가 이동해서 멀어졌다면,
						{
							StopAttack();						
						}
							
						if(vDelta.Length() > 2.0f)
						{		
							//1230 여기서 move메시지를 보내야 한다.
							if(!(m_bForward||m_bKeyMove))
							{
								tmStartTime = 0;
							}

							m_bForward = TRUE;			
							//m_bKeyMove = TRUE;
							m_vDesiredPosition = penTarget->GetPlacement().pl_PositionVector;
					
							FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;	  
											
							if((m_bForward||m_bKeyMove) && (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)) - tmStartTime > 1000)
							{
								SendMyNextMovePosition(vDelta);
							} 
					
							return TRUE;
						}
						else
						{
							return FALSE;
						}
					}
				}
			}
			// 애완동물의 경우.
			// [2011/07/15 : Sora] 기획요청으로 전투펫 타입 추가
			else if( penTarget->IsPet() || penTarget->IsSlave() || penTarget->IsWildPet() )
			{
				CancelProduct();

				// [100107: selo] MyCharacterInfo.attackrange 대치
				float fAttackDistance = GetAttackrange();								

				FLOAT3D vDelta = GetPlacement().pl_PositionVector - penTarget->GetPlacement().pl_PositionVector;
				vDelta(2) = 0.0f;
				// FIXME : 코드 중복이 심함.
				if( vDelta.Length() < fAttackDistance && !IsAttacking() && m_nCurrentSkillNum==-1)
				{
					if(ulNewButtons&PLACT_FIRE && (m_bForward))
					{
						m_bForward = FALSE;
					}			
					StopMove();		

					if(!IsVisibleCheckAll(penTarget))
					{
						CTString strSysMessage;
						strSysMessage.PrintF("가로막혀서 공격할수 없습니다.");
						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);	

						GetPlayerWeapons()->m_penReservedTarget = GetPlayerWeapons()->m_penRayHitTmp;
						GetPlayerWeapons()->m_penRayHitTmp = NULL;	

						return FALSE;
					}
					else
					{
						BOOL bAttack = CheckAttackTarget(-1, penTarget, 0.0f );
						if(bAttack)
						{
							StartAttack();
						}
						else
						{
							StopAttack();
						}
					}
					
					return FALSE;//SetDesiredMovement()실행 안함.
				}
				else if(IsAttacking())
				{
					Rotate(-vDelta, 1800.0f, TRUE);//360->1800
					return FALSE;
				}
				else
				{
					if(IsAttacking())//공격중에 몬스터가 이동해서 멀어졌다면,
					{
						StopAttack();						
					}

					if(vDelta.Length() >= 2.0f && !m_bHold)
					{
						if(!(m_bForward||m_bKeyMove))
						{
							tmStartTime = 0;
						}

						m_bForward = TRUE;			
						//m_bKeyMove = TRUE;
						m_vDesiredPosition = penTarget->GetPlacement().pl_PositionVector;
						//1230 여기서 move메시지를 보내야 한다.
						FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;
					
						if((m_bForward||m_bKeyMove) && (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)) - tmStartTime > 1000)
						{
							SendMyNextMovePosition(vDelta);
						} 
			
						//몬스터로 이동
						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
			}
			else if(penTarget->IsFirstExtraFlagOn(ENF_EX1_NPC))		// NPC일 경우.
			{
				CancelProduct();

				// Mob Index를 얻습니다.
				CDLLEntityClass *pdecDLLClass	= penTarget->GetClass()->ec_pdecDLLClass;
				CEntityProperty &epProperty		= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 91);		// Mob Index
				const int iMobIndex				= ENTITYPROPERTY( &*penTarget, epProperty.ep_slOffset, INDEX);
				
				if(iMobIndex == -1)
				{	
					return TRUE;
				}

				const int iVirIdx				= penTarget->GetNetworkID();
				CMobData* MD					= CMobData::getData(iMobIndex);
				FLOAT3D	vNpcPos					= penTarget->GetPlacement().pl_PositionVector;
				FLOAT3D vDelta					= GetPlacement().pl_PositionVector - vNpcPos;

				vDelta(2) = 0.0f;

				// ITS#4369 : 덩치 큰 NPC 클릭 시 distance의 차이로 인해 정상 클릭 처리가 되지 않는 현상 수정. [10/10/2011 rumist]
				FLOATaabbox3D bbox;
				penTarget->GetModelInstance()->GetCurrentColisionBox(bbox);
				// get distance at boundary box.
				FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f;
				// called M-P length, sum distance both boundary box length and constant length.
				float fLength = vRadius.Length()+5.0f;

				// if distance between model and PC bigger than M-P length, return true.
				if( vDelta.Length() >= (fLength) && _cmiComm. IsNetworkOn())
				{
					return TRUE;
				}
				// 클릭된 몹의 UID가 동일한데 몹 인덱스가 다르면 NPC를 통한 UI열기를 막는다. 
				// 몹 index만 바꿔서 
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, penTarget->en_lNetworkID);

				if (pObject != NULL)
				{
					if (pObject->GetType() != iMobIndex)
					{
						return TRUE;
					}
				}
				
				if (_pNetwork->IsRvrZone() && MD->GetSyndicateType() != _pNetwork->MyCharacterInfo.iSyndicateType)
				{
					if(m_bForward )
					{
						StopMove();
						pUIManager->GetChattingUI()->AddSysMessage( _S(6090, "분쟁 지역에서 적대 관계인 경우 사용할 수 없는 기능입니다"), SYSMSG_ERROR);
					}

					return FALSE;
				}

				if( iMobIndex == 244 ) // 재정 관리인 
				{
					if(m_bForward )
					{				
						StopMove();

						if( _pNetwork->MyCharacterInfo.lGuildLevel < 4 ) 
						{
							pUIManager->GetGuildStash()->Message( MSGCMD_NULL, CTString( _S( 2556, "길드 레벨 3이하는 창고를 소유할 수 없습니다." ) ), UMBS_OK );
							return FALSE;
						}
			
						pUIManager->GetGuildStash()->OpenGuildStash();
						return FALSE;
					}
				}
				else if( iMobIndex >= 240 && iMobIndex <= 243 ) // 공성 워프 게이트
				{
					if(m_bForward )
					{				
						StopMove();
						pUIManager->GetGuildWarPortal()->OpenGuildWarPortal( iMobIndex );
						return FALSE;
					}
				}
				// WSS_DRATAN_SEIGEWARFARE 2007/08/07 ------------------------------------>>
				else if( iMobIndex >= 382 && iMobIndex <= 386 ) // 드라탄 공성 워프 게이트
				{
					if(m_bForward )
					{				
						StopMove();
						if( pUIManager->GetGuildWarPortal()->IsVisible() )
						{
							// WSS_DRATAN_SEIGEWARFARE 2007/09/14 
							return FALSE;
						}

						pUIManager->GetGuildWarPortal()->SetPortalIdx(iMobIndex);
						pUIManager->GetSiegeWarfareNew()->SendDratanPortalMessage( iMobIndex );
						return FALSE;
					}
				}			
				// -----------------------------------------------------------------------<<
				else if (MD->IsZoneMoving() &&  !m_bHold)			// 존이동
				{
					CEntity *pen = GetPlayerWeapons()->m_penRayHitTmp;
					if(!pUIManager->GetPortal()->IsVisible())
					{
						CEntityProperty &epPropertyZone	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_ZONEFLAGS_EX, 237);
						CEntityProperty &epPropertyExtra= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_ZONEFLAGS_EX, 238);
						ZONEFLAGS ulZoneFlag	= ENTITYPROPERTY( &*penTarget, epPropertyZone.ep_slOffset, ZONEFLAGS);
						ZONEFLAGS ulExtraFlag	= ENTITYPROPERTY( &*penTarget, epPropertyExtra.ep_slOffset, ZONEFLAGS);
					
						pUIManager->GetPortal()->ResetZoneList();

						for(INDEX i=0; i<sizeof(ZONEFLAGS)*8 && i<CZoneInfo::getSingleton()->GetZoneCount(); ++i)
						{
							if( !(ulZoneFlag & (((ZONEFLAGS)1) << i)) ) { continue; }
						
							for(INDEX j=0; j<sizeof(ZONEFLAGS)*8 && j<CZoneInfo::getSingleton()->GetExtraCount(i); ++j)
							{
								if( !(ulExtraFlag & (((ZONEFLAGS)1) << j)) ) { continue; }
								pUIManager->GetPortal()->AddToZoneList(i, j);
								pUIManager->GetPortal()->SetNpcIdx(MD->GetMobIndex());
							}
						}
					
						// [071123: Su-won] DRATAN_SIEGE_DUNGEON
						if( MD->GetMobIndex() == 468 )
						{
							pUIManager->GetPortal()->SetNpcIdx(MD->GetMobIndex());
							// 테오스 무덤 관리인을 통해서 퀘스트를 받을 수 있도록 수정
							pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_PORTAL, vNpcPos(1), vNpcPos(3) );
						}
						else if(pUIManager->GetPortal()->GetZoneListCount() )
						{
							pUIManager->GetPortal()->OpenPortal( vNpcPos(1), vNpcPos(3) );
						}
						GetPlayerWeapons()->m_penRayHitTmp = NULL;
						StopMove();
						return FALSE;
					}
				}
				else if(MD->GetMobIndex() == 482)		// ttos : 판매 대행 NPC
				{
					if(_pNetwork->MyCharacterInfo.sbShopType == PST_NOSHOP)
					{
						pUIManager->GetPersonalShop()->TradePersonalShop(MD->GetOwnerIndex(), 0.0f, 0.0f, TRUE);
						GetPlayerWeapons()->m_penRayHitTmp = NULL;
						return TRUE;
					}
				}
				else if( MD->IsAffinityNPC() )		// Affinity NPC. [rumist]
				{
					if(m_bForward )
					{
						StopMove();
						pUIManager->GetAffinity()->OpenAffinity( MD->GetMobIndex(), NULL, vNpcPos(1), vNpcPos(3) );
						return FALSE;
					}
				}
   				else if( MD->GetMobIndex() == 1106 )		// SOCKET SYSTEM NPC. [6/1/2010 rumist]
   				{
   					if( m_bForward )
   					{
  						StopMove();
   						pUIManager->OpenSocketSystem( MD->GetMobIndex(), NULL, vNpcPos(1), vNpcPos(3) );
   						return FALSE;
   					}
   				}
				else if( MD->GetMobIndex() == 1254 )		// royal rumble 관리인. [4/26/2011 rumist]
				{
					if( m_bForward )
					{
						StopMove();
						pUIManager->GetRadar()->OpenRoyalrumbleMgrMenu( MD->GetMobIndex() );
						return FALSE;
					}
				}
				else if( MD->GetMobIndex() == 1261 )		// 인챈트 마스터 닐린. (마스터 스톤) ; 05/11/11 trylord
				{
					if( m_bForward )
					{
						StopMove();
						pUIManager->GetChangeWeapon()->OpenChangeWeapon(iMobIndex, UI_CHANGEWEAPON, vNpcPos(1), vNpcPos(3));
						return FALSE;
					}
				}
				// tarian bloody mir 추가. [9/22/2011 rumist]
				else if( MD->GetMobIndex() == 1200 || MD->GetMobIndex() == 1198 )		// 스킬 마스터 푸치아 (타리안 어둠의 스킬마스터) ; 06/20/11 trylord
				{	// NPC캐쉬 템을 이용하여 여는 스킬마스터이나, 타리안에서는 NPC를 통하여 스킬마스터 사용
					if( m_bForward )
					{
						StopMove();
						// NPC 퀘스트가 있는지 요청 [11/14/2012 Ranma]
						pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_SKILLLEARN, vNpcPos(1), vNpcPos(3));
						return FALSE;
					}
				}
				else if(MD->IsShopper())					// 상인
				{
					if(m_bForward )
					{
						StopMove();
						pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_SHOP, vNpcPos(1), vNpcPos(3) );
						return FALSE;
					}
				}
				else if(MD->IsWareHouse())			// 창고
				{
					if(m_bForward )
					{
						StopMove();
						pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_WAREHOUSE, vNpcPos(1), vNpcPos(3) );
						return FALSE;
					}
				}
				else if(MD->IsRefiner())				// 연금술사
				{
					if(m_bForward )
					{				
						StopMove();
						pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_REFINE, vNpcPos(1), vNpcPos(3) );
						return FALSE;
					}
				}
				else if(MD->IsGuild())				// 길드 생성
				{
					if(m_bForward )
					{				
						StopMove();
						pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_GUILD, vNpcPos(1), vNpcPos(3) );
						return FALSE;
					}
				}
				else if(MD->IsResetStat())			// 스탯 초기화.
				{
					if(m_bForward )
					{				
						StopMove();
						pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_INITJOB, vNpcPos(1), vNpcPos(3) );
						return FALSE;
					}
				}
				else if(MD->IsChangeWeapon())		// 무기 교환
				{
					if(m_bForward )
					{				
						StopMove();
						pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_CHANGEWEAPON, vNpcPos(1), vNpcPos(3) );
						return FALSE;
					}
				}
				else if(MD->IsWarCastle())			// 공성 NPC
				{
					if(m_bForward )
					{				
						StopMove();
					
						// WSS_DRATAN_SEIGEWARFARE 2007/08/02 ----------------------------------------->>
						int tQtr;  // quarter state
						if( iMobIndex == DRATAN_MASTER_TOWER_INDEX )
						{					
							// 공성 지역에 있고 수성길드이면...							
							if(	_pNetwork->MyCharacterInfo.sbAttributePos & MATT_WAR &&
								_pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_OWNER )								
							{	
								pUIManager->GetQuest()->OpenQuest( iMobIndex, iVirIdx, FALSE, vNpcPos(1),vNpcPos(3) );								
							}
						}
						else if( iMobIndex == DRATAN_LORD_SYMBOL_INDEX ) // 드라탄 공성 크리스탈 // WSS_DRATAN_SEIGEWARFARE 2007/07/30
						{
							// 공성 지역에 있고 공성길드이면...							
							if(	_pNetwork->MyCharacterInfo.sbAttributePos & MATT_WAR &&
								_pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_ATTACK_GUILD &&
								!_pNetwork->MyCharacterInfo.bConsensus)
							{							
								pUIManager->GetSiegeWarfareNew()->SendConsensus((UBYTE)MSG_CASTLE_CRISTAL_RESPOND_START);
							}						 
						}
						else if( (tQtr=MD->IsMyQuarter())!= QUARTER_NONE)
						{
							if( tQtr==QUARTER_INSTALL && _pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_ATTACK_GUILD)
							{
								pUIManager->GetSiegeWarfareNew()->CreateBuyQuarterBox(MD->GetMobIndex());
							}
						}
						// ----------------------------------------------------------------------------<<
						else
						{
							pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_SIEGE_WARFARE, vNpcPos(1), vNpcPos(3) );
						}
						return FALSE;
					}
				}
				// WSS_DRATAN_SEIGEWARFARE 2007/08/06 -------------------------------------------------->>				
				else if( MD->IsCastleTower())
				{
					if(m_bForward )
					{				
						StopMove();
						// 드라탄 공성시 성주 길드이면 수리 메뉴가 나오게 한다.
						if( _pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_OWNER)
						{
							// 내외성 결계의 눈은 수리(X) WSS_DRATAN_SIEGEWARFARE 071003 
							if(MD->GetMobIndex() == 388 ||
								MD->GetMobIndex() == 389 ||
								MD->GetMobIndex() == 404 )
							{
								return FALSE;
							}

							pUIManager->GetSiegeWarfareNew()->SendTowerRepairStateRequest(MD->GetMobIndex());
						}
					}
				}
				// -------------------------------------------------------------------------------------<<
				else if( MD->IsSkillMaster() )			// 스킬 마스터
				{
					if( m_bForward )
					{				
						StopMove();
						pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_SKILLLEARN, vNpcPos(1), vNpcPos(3) );
						return FALSE;
					}				
				}
				else if( MD->IsSSkillMaster() )			// 스킬 마스터
				{
					if( MD->GetSpecialSkillMaster() == SSKILL_PROCESS_NPC )						
					{
						if( m_bForward )
						{				
							StopMove();
							pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_PROCESSNPC, vNpcPos(1), vNpcPos(3) );
							return FALSE;
						}
					}
					else
					{
						if( m_bForward )
						{				
							StopMove();
							pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_SKILLLEARN, vNpcPos(1), vNpcPos(3) );
							return FALSE;
						}
					}				
				}
				else if (MD->IsMakeItemNPC())
				{
					if(m_bForward )
					{
						StopMove();
						pUIManager->GetQuestAccept()->RequestQuest(iMobIndex, iVirIdx, UI_PRODUCTNPC,  vNpcPos(1), vNpcPos(3) );
						return FALSE;
					}
				}
				else if(MD->IsQuest())				// 퀘스트
				{
					if( m_bForward )
					{				
						StopMove();
						pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_QUEST, vNpcPos(1), vNpcPos(3) );
						return FALSE;
					}
				}
				// 도박 NPC
				// FIXME : 하드코딩한 부분.
				else if( iMobIndex == 192 )
				{
					if( m_bForward )
					{
						StopMove();
						pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_GAMBLE, vNpcPos(1), vNpcPos(3) );
						pUIManager->GetGamble()->OpenGamble( penTarget);
						return FALSE;
					}
				}
				// 캐쉬 문스톤 
				else if( iMobIndex == 271 )
				{
					if( m_bForward )
					{
						StopMove();
					//	pUIManager->GetQuestBookNew()->RequestQuest( iMobIndex, UI_GAMBLE, vNpcPos(1), vNpcPos(3) );
						pUIManager->GetGamble()->OpenGamble( penTarget ,TRUE);
						return FALSE;
					}
				}
				//우편함 Message Cube
				else if( iMobIndex == 1374 )
				{
					if( m_bForward)
					{
						StopMove();
						SE_Get_GameDataManagerPtr()->GetExpressData()->SetNPCInfo( iMobIndex, vNpcPos(1), vNpcPos(3) );
					}
				}

				// 리폼 시스템 [9/6/2012 Ranma]
				else if( iMobIndex == 1520 )
				{
					if( m_bForward )
					{
						StopMove();
						pUIManager->GetReformSystem()->OpenReformSystem(iMobIndex,vNpcPos(1), vNpcPos(3));
					}
				}
				// bloody mir npc 추가 [9/22/2011 rumist]
				else if( iMobIndex == 253 || iMobIndex == 1032 || iMobIndex == 1191 || iMobIndex == 1201)			// 애완동물 조련사.
				{
					if( m_bForward )
					{				
						StopMove();
						pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_PETTRAINING, vNpcPos(1), vNpcPos(3) );
						return FALSE;
					}
				}
				// 경비병, 근위대원, 케론경비대원 안내시스템 적용
				else if (iMobIndex == 32 || iMobIndex == 88 || iMobIndex == 160 || iMobIndex == 343)
				{
					if( m_bForward )
					{				
						StopMove();
						pUIManager->RearrangeOrder(UI_NPCHELP,TRUE);
						return FALSE;
					}
				}
				else if(MD->IsPeaceful())			// 평화 NPC(대화가능~)
				{
					if(_pNetwork->m_bSingleMode)
					{
						if( m_bForward )
						{
							StopMove();
						
							// 몹의 이벤트 타입을 확인 후, 그 타입에 따라서 퀘스트 확인, 아니면 이벤트 발생.
							if(((CEnemy*)penTarget)->m_EventType == EVENT_MOBCLICK)	// 몹 클릭시 특정 이벤트 발생일 경우.
							{
								((CEnemy*)penTarget)->ClickedEvent();
							}										 
							else
							{
								pUIManager->GetQuestAccept()->RequestQuest( iMobIndex, iVirIdx, UI_CHARACTERINFO, vNpcPos(1), vNpcPos(3) );
							}
							return FALSE;
						}
					}				
					return TRUE;
				}
			
			}
			else if(!m_bHold && //바닥을 찍었을 경우.
				!(( _pNetwork->MyCharacterInfo.job == ROGUE && IsAnimationPlaying(iDeathMotion) && // 데스 모션 동작 중이면서
				(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime) < en_pmiModelInstance->GetAnimLength(iDeathMotion)))) // 눕기 전까지
			{
				CancelProduct();
						
				if(IsAttacking())
				{
					if(m_penAttackingEnemy != NULL){//null이라면 공격 애니메이션이 끝까지 이르렀다는 뜻.
						return FALSE;
					}
					StopAttack();			
					StopMove();	
				}

				FLOAT3D vDelta = m_vDesiredPosition - GetPlacement().pl_PositionVector;

				if(penTarget->IsEnemy() && !(penTarget->GetFlags()&ENF_ALIVE))
				{	
					// FIXME : 어떤건 2.0이고 어떤건 1.0이구...왜???
					if(vDelta.Length() < 2.0f)//타겟지점과의 거리
					{		
						if(ulNewButtons&PLACT_FIRE && (m_bForward) )
						{
							m_bForward = FALSE;
						}
								
						StopMove();	
						
						g_bIsRotateEnd = FALSE;
						if(g_bFirstRotate)
						{
							BOOL bFinished = TRUE;
							bFinished = Rotate(g_bTargetDirection, 1800.0f, TRUE);//1800에서 720
							if(!bFinished)
							{					
								g_bFirstRotate = FALSE;
							}				
						}					
						return FALSE;
					}
					else
					{						

						return TRUE;//m_vDesiredPosition으로.				
					}
				}
			
				if(vDelta.Length() < 0.5f)//타겟지점과의 거리
				{		
					if(ulNewButtons&PLACT_FIRE && (m_bForward))
					{
						m_bForward = FALSE;
					}
							
					StopMove();	
					
					g_bIsRotateEnd = FALSE;
					
					//0625 kwon
					if(g_bFirstRotate)
					{				
						BOOL bFinished = TRUE;
						bFinished = Rotate(g_bTargetDirection, 1800.0f, TRUE);//1800에서 720
						if(!bFinished)
						{					
							g_bFirstRotate = FALSE;
						}				
					}					
					return FALSE;
				}
				else
				{
					return TRUE;//m_vDesiredPosition으로.				
				}
			}
		}
		//	m_penRayHitTmp이 Null일경우.
		else
		{
			if(IsAttacking())
			{
				//null이라면 공격 애니메이션이 끝까지 이르렀다는 뜻.
				if(m_penAttackingEnemy != NULL)
				{
					return FALSE;
				}
				StopAttack();			
			}
			StopMove();	
			
			return FALSE;
		}
		return FALSE;
	}

	void StopAttack()
	{
		m_bStartAttack = FALSE;
	}

	void StartAttack()
	{
		if (_pGameState->IsRestartGame() == TRUE)
		{
			UIMGR()->GetSystemMenu()->CancelRestart();
		}

		if(m_bMobChange)
		{
			return;
		}

		m_bStartAttack = TRUE;
	}

	BOOL IsAttacking()
	{
		return	m_bStartAttack;
	}
	
	BOOL IsProduct()
	{
		return m_bProduction;
	}	

	void StopMove()
	{	
		if(m_bForward || m_bKeyMove)
		{
			m_bForward = FALSE;
			m_bKeyMove = FALSE;
			extern BOOL	_bLoginProcess;
			if(_cmiComm. IsNetworkOn() && !_bLoginProcess)
			{
				if(!SE_Get_UIManagerPtr()->IsCSFlagOn(CSF_CANNOT_MOVE_MASK) && !m_bSendStopMessage)
				{
					// StopMove 패킷이 두번 가는 상황을 방지하기 위해 추가.
					if (tmStartTime > 0.001f)
					{
						_pNetwork->SendStopMessage(this, GetPlacement());
						tmStartTime = 0;
					}

					if (!SE_Get_UIManagerPtr()->GetSelectResource()->GetKeyMove())
					{
						SE_Get_UIManagerPtr()->GetSelectResource()->SetKeyMove(true);
					}
					m_bSendStopMessage = TRUE;
				}
			}
		}
		m_vDesiredPosition = GetPlacement().pl_PositionVector;//이동하려는 지점을 자신이 서있는 곳으로 셋팅.
		m_vMyPositionTmp = GetPlacement().pl_PositionVector;
		StopRotating();
		StopTranslating();	

		if(GetPlayerWeapons()->m_penRayHitClick != NULL) //0909 
		{			
			GetPlayerWeapons()->m_penRayHitClick = NULL;
		}
	//안태훈 수정 시작	//(5th Closed beta)(0.2)
		if(CEffectGroupManager::Instance().IsValidCreated(m_pPickingEffectGroup))
		{
			m_pPickingEffectGroup->Stop(PICKING_EFFECT_REMAIN_TIME);
			m_pPickingEffectGroup = NULL;
		}
	//안태훈 수정 끝	//(5th Closed beta)(0.2)
	}

	void ClearMove()
	{
		m_bForward = FALSE;
		m_bKeyMove = FALSE;

		StopMove();
	}
	
	void StopMoveNoSendStopMsg()
	{		
		m_bForward = FALSE;
		m_bKeyMove = FALSE;
		m_vDesiredPosition = GetPlacement().pl_PositionVector;//이동하려는 지점을 자신이 서있는 곳으로 셋팅.
		m_vMyPositionTmp = GetPlacement().pl_PositionVector;
		StopRotating();
		StopTranslating();	

		if(GetPlayerWeapons()->m_penRayHitClick != NULL) //0909 
		{			
			GetPlayerWeapons()->m_penRayHitClick = NULL;
		}
		if(CEffectGroupManager::Instance().IsValidCreated(m_pPickingEffectGroup))
		{
			m_pPickingEffectGroup->Stop(PICKING_EFFECT_REMAIN_TIME);
			m_pPickingEffectGroup = NULL;
		}
	}

	void PickItem(ULONG targetindex)
	{
		if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING) // 비행 모드에서는 아이템 못 주워요~
		{
			return;
		}

		_pNetwork->SendPickMessage( this, (ULONG)targetindex, TRUE );
		}

	virtual void PlayItemSound(BOOL bPick, BOOL bMoney)
	{
		if(bPick)
		{
			PlaySound(m_soMessage, SOUND_PICK_ITEM, SOF_3D | SOF_VOLUMETRIC);										
		}
		else 
		{
			if(bMoney)
			{
				PlaySound(m_soMessage, SOUND_DROP_MONEY, SOF_3D | SOF_VOLUMETRIC);										
			}
			else
			{
				PlaySound(m_soMessage, SOUND_DROP_ITEM, SOF_3D | SOF_VOLUMETRIC);										
			}
		}
	}

	void PlayButtonSound()
	{		
		PlaySound(m_soMessage, SOUND_BUTTON_CLICK, SOF_3D | SOF_VOLUMETRIC);	
	}
// ui Sound TO.DO
	void PlayJewelDullSound()
	{		
		PlaySound(m_soMessage, SOUND_UI_JEWEL_DULL, SOF_3D | SOF_VOLUMETRIC);	
	}

	void PlayJewelRefinementSound()
	{		
		PlaySound(m_soMessage, SOUND_UI_JEWEL_REFINEMENT, SOF_3D | SOF_VOLUMETRIC);	
	}

	void PlayJewelShinySound()
	{		
		PlaySound(m_soMessage, SOUND_UI_JEWEL_SHINY, SOF_3D | SOF_VOLUMETRIC);	
	}

	void PlayPetStashEffectSound()
	{
		PlaySound(m_soMessage, SOUND_UI_PETSTASH_CARD, SOF_3D | SOF_VOLUMETRIC);
	}

	void PlayPetStashCardSelSound()
	{
		PlaySound(m_soMessage, SOUND_UI_PETSTASH_CARD_SELECT, SOF_3D | SOF_VOLUMETRIC);
	}

	void PlayHolyWaterSound()
	{
		PlaySound(m_soMessage, SOUND_UI_HOLYWATER, SOF_3D | SOF_VOLUMETRIC);
	}

	virtual void PlayItemEffect(SLONG itemindex, SBYTE effecttype)
	{	
		
		if(itemindex != -1)
		{
			switch(itemindex)
			{
				case 391:
				default:
					PlaySound(m_soMessage, SOUND_POTION, SOF_3D | SOF_VOLUMETRIC);	
					break;
			}
			return;
		}
/*
		if(effecttype != -1)
		{

			switch(effecttype)
			{
				case 2://채광
				case 3://채집
				case 4://차지
					PlaySound(m_soMessage, SOUND_CRAK, SOF_3D | SOF_VOLUMETRIC);	
					break;
				//	PlaySound(m_soMessage, SOUND_MINE, SOF_3D | SOF_VOLUMETRIC);	
					break;
				case 5:
					PlaySound(m_soMessage, SOUND_PROCESS, SOF_3D | SOF_VOLUMETRIC);	
					break;
				case 6:				
					PlaySound(m_soMessage, SOUND_PRODUCT, SOF_3D | SOF_VOLUMETRIC);	
					break;
				default:
					break;
			}
			return;
		}
*/		
	}

	BOOL DetectFloorHit(FLOAT deltaX, FLOAT deltaY, FLOAT deltaZ, CPlacement3D& plPlacement)
	{
		// 이동하려는 위치를 구한다음에,
		FLOAT3D	vRayHit;
		plPlacement.pl_OrientationAngle = ANGLE3D(0,0,0);
		plPlacement.pl_PositionVector = FLOAT3D(GetPlacement().pl_PositionVector(1)+deltaX, 300.0f,GetPlacement().pl_PositionVector(3)+deltaZ);	
		
		FLOAT fMaxY = -9999999.0f;
		
		// 걸리는게 없는지 위에서 레이를 쏴서 테스트???
		// 아마도, 이동하고자 하는 곳이 바닥인지 아닌지를 판단하는거 같음...
		FLOAT3D vSource = plPlacement.pl_PositionVector;
		FLOAT3D vTarget = vSource;
		vTarget(2) -= 1000.0f;
		CCastRay crRay( this, vSource, vTarget);
		crRay.cr_ttHitModels = CCastRay::TT_NONE; // CCastRay::TT_FULLSEETHROUGH;
		crRay.cr_bHitTranslucentPortals = TRUE;
		crRay.cr_bPhysical = TRUE;
		GetWorld()->CastRay(crRay);

		// 충돌 Ray를 하나만 더 가져가 보자
		FLOATaabbox3D bbox;
		FLOAT3D mi_vStretch;
		GetModelInstance()->GetCurrentColisionBox(bbox);
		mi_vStretch = GetModelInstance()->mi_vStretch;
		bbox.maxvect *= mi_vStretch;
		bbox.minvect *= mi_vStretch;
		FLOAT3D vLength = bbox.Max() - bbox.Min();
		FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f;

		if (deltaY < 0.0f)
		{
			deltaY = 0.0f;
		}

		vSource = FLOAT3D(GetPlacement().pl_PositionVector(1), GetPlacement().pl_PositionVector(2)+(vLength.Length()*0.5f),GetPlacement().pl_PositionVector(3));
		vTarget = FLOAT3D(GetPlacement().pl_PositionVector(1)+deltaX, GetPlacement().pl_PositionVector(2)+(vLength.Length()*0.5f)+deltaY,GetPlacement().pl_PositionVector(3)+deltaZ);

		CCastRay TestcrRay(this, vSource, vTarget);
		TestcrRay.cr_ttHitModels = CCastRay::TT_COLLISIONBOX; // CCastRay::TT_FULLSEETHROUGH;
		TestcrRay.cr_bHitTranslucentPortals = TRUE;
		TestcrRay.cr_bPhysical = TRUE;
		GetWorld()->CastRay(TestcrRay);

		if( (crRay.cr_penHit != NULL) && (crRay.cr_vHit(2) > fMaxY) && TestcrRay.cr_penHit == NULL) 
		{
			fMaxY = crRay.cr_vHit(2);
			plPlacement.pl_PositionVector(2) += fMaxY-(plPlacement.pl_PositionVector(2)+0.1f);
			return TRUE;
		}

		return FALSE;
	}
// 서버에 Move 메시지를 전달하기전에, 이동할 위치에 충돌모델이 있는 판단하여, 충돌로 갈수 없다면, Move 메시지를 보내지 않는다.
// Collision detect Ray 를 추가하였음.(TestcrRay)
	void SendMyNextMovePosition(FLOAT3D vDelta)
	{
		if (_pGameState->IsRestartGame() == TRUE)
		{
			UIMGR()->GetSystemMenu()->CancelRestart();
		}

		if(_cmiComm. IsNetworkOn() )
		{	
			FLOAT length = vDelta.Length();
			FLOAT deltaX = 0.0f;// = m_vDesiredPosition(1) - GetPlacement().pl_PositionVector(1);
			FLOAT deltaZ = 0.0f;//m_vDesiredPosition(3) - GetPlacement().pl_PositionVector(3);
			FLOAT deltaY = 0.0f;
			FLOAT num = 0.0f;
			
			// 움직일 수 없는 경우 서버로의 통신이 필요없으므로 제한함. [1/31/2011 rumist]
			if( IsMovable() )
			{
				deltaX = m_vDesiredPosition(1) - GetPlacement().pl_PositionVector(1);
				deltaZ = m_vDesiredPosition(3) - GetPlacement().pl_PositionVector(3);
				deltaY = m_vDesiredPosition(2) - GetPlacement().pl_PositionVector(2);
				num = length/plr_fSpeed;
			}
			else
			{
				//deltaX = GetPlacement().pl_PositionVector(1);
				//deltaZ = GetPlacement().pl_PositionVector(3);
				//deltaY = m_vDesiredPosition(2) - GetPlacement().pl_PositionVector(2);
				num = 0.0f;
			}
			

			// 한번에 갈수 있는 거리라면...
			if(num <= 1.0f)
			{
				//이전에 메시지 보낼때와 비교해서 0.5보다 적게 갔다면,
				FLOAT3D vDelta = GetPlacement().pl_PositionVector - m_vMyPositionTmp;
				vDelta(2) = 0.0f;

				if(vDelta.Length() < 0.5f)
				{
					//y값을 고려하지 않은 거리 계산.
					FLOAT flength = Sqrt(deltaX*deltaX + deltaZ*deltaZ);
					
					if(flength < 0.5f)
					{
						StopMove();
						//m_bSendStopMessage = TRUE;
						return;
					}
				}

				CPlacement3D plPlacement;
				
				// 움직일 수 없는 경우 목표 위치가 아닌 현재 위치를 갱신한다. [1/31/2011 rumist]
				// 추후 방향만 측정하여 전달하는 방법 고민해보자.
				if( IsMovable() && DetectFloorHit(deltaX, deltaY, deltaZ, plPlacement))
				{
					plPlacement.pl_PositionVector = m_vDesiredPosition;
				}
				else
				{
					plPlacement.pl_PositionVector = GetPlacement().pl_PositionVector;
					//m_vDesiredPosition = GetPlacement().pl_PositionVector;
				}
				plPlacement.pl_OrientationAngle = ANGLE3D(GetPlacement().pl_OrientationAngle(1),0,0);
				_pNetwork->SendMoveMessage(this, plPlacement,plr_fSpeed);						
				m_vMyPositionTmp = GetPlacement().pl_PositionVector;
				m_bSendStopMessage = FALSE;
				tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000); //timeGetTime();
				return;
			}

			// 한번에 갈수 없는 거리라면...

			deltaX = deltaX/num;
			deltaZ = deltaZ/num;
			
			// 이동하려는 위치를 구한다음에,
			FLOAT3D	vRayHit;
			CPlacement3D plPlacement;
			plPlacement.pl_OrientationAngle = ANGLE3D(0,0,0);
			plPlacement.pl_PositionVector = FLOAT3D(GetPlacement().pl_PositionVector(1)+deltaX, 300.0f,GetPlacement().pl_PositionVector(3)+deltaZ);	
			
			FLOAT fMaxY = -9999999.0f;
			BOOL bFloorHitted = FALSE;
			
			// 걸리는게 없는지 위에서 레이를 쏴서 테스트???
			// 아마도, 이동하고자 하는 곳이 바닥인지 아닌지를 판단하는거 같음...
			FLOAT3D vSource = plPlacement.pl_PositionVector;
			FLOAT3D vTarget = vSource;
			vTarget(2) -= 1000.0f;
			CCastRay crRay( this, vSource, vTarget);
			crRay.cr_ttHitModels = CCastRay::TT_NONE; // CCastRay::TT_FULLSEETHROUGH;
			crRay.cr_bHitTranslucentPortals = TRUE;
			crRay.cr_bPhysical = TRUE;
			GetWorld()->CastRay(crRay);
			if( (crRay.cr_penHit != NULL) && (crRay.cr_vHit(2) > fMaxY)) 
			{
				fMaxY = crRay.cr_vHit(2);
				bFloorHitted = TRUE;
			}
			// 충돌 Ray를 하나만 더 가져가 보자
			FLOATaabbox3D bbox;
			FLOAT3D mi_vStretch;
			GetModelInstance()->GetCurrentColisionBox(bbox);
			mi_vStretch = GetModelInstance()->mi_vStretch;
			bbox.maxvect *= mi_vStretch;
			bbox.minvect *= mi_vStretch;
			FLOAT3D vLength = bbox.Max() - bbox.Min();
			FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f;

			if (deltaY < 0.0f)
			{
				deltaY = 0.0f;
			}

			vSource = FLOAT3D(GetPlacement().pl_PositionVector(1), GetPlacement().pl_PositionVector(2)+(vLength.Length()*0.5f),GetPlacement().pl_PositionVector(3));
			vTarget = FLOAT3D(GetPlacement().pl_PositionVector(1)+deltaX, GetPlacement().pl_PositionVector(2)+(vLength.Length()*0.5f)+deltaY,GetPlacement().pl_PositionVector(3)+deltaZ);

			//vSource = FLOAT3D(GetPlacement().pl_PositionVector(1)+deltaX, 300.0f,GetPlacement().pl_PositionVector(3)+deltaZ);
			//vTarget = vSource;
			//vTarget(2) -= 1000.0f;
			CCastRay TestcrRay(this, vSource, vTarget);
			TestcrRay.cr_ttHitModels = CCastRay::TT_COLLISIONBOX; // CCastRay::TT_FULLSEETHROUGH;
			TestcrRay.cr_bHitTranslucentPortals = TRUE;
			TestcrRay.cr_bPhysical = TRUE;
			GetWorld()->CastRay(TestcrRay);

			if( bFloorHitted)
			{
				plPlacement.pl_PositionVector(2) += fMaxY-(plPlacement.pl_PositionVector(2)+0.1f);
			}
			
			tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000); //timeGetTime();
				
			plPlacement.pl_OrientationAngle(1) = GetPlacement().pl_OrientationAngle(1);

			// 이동할 곳의 위치로 메세지를 보냄.
			if (TestcrRay.cr_penHit != NULL)
			{
				plPlacement.pl_PositionVector = GetPlacement().pl_PositionVector;
				//m_vDesiredPosition = GetPlacement().pl_PositionVector;
			}
			_pNetwork->SendMoveMessage(this, plPlacement,plr_fSpeed);
			m_vMyPositionTmp = GetPlacement().pl_PositionVector;
			m_bSendStopMessage = FALSE;
		}
	}	
	
	//0223 kwon 함수추가
	BOOL Rotate(FLOAT3D vDelta, ANGLE aRotateSpeed, BOOL bFirstRotate)
	{
		//float m_fMoveFrequency = 0.25f;//0.1f에서 0.25f
		static ANGLE _aHeadingRotation =0.0f;
		// if we may rotate
		if (aRotateSpeed>0.0f && bFirstRotate) {
			// get desired heading orientation
			FLOAT3D vDir = vDelta;
			vDir.SafeNormalize();
			ANGLE aWantedHeadingRelative = GetRelativeHeading(vDir);
			
			// normalize it to [-180,+180] degrees
			aWantedHeadingRelative = NormalizeAngle(aWantedHeadingRelative);
			
			ANGLE aHeadingRotation;
			// if desired position is left
			if (aWantedHeadingRelative < -aRotateSpeed*m_fMoveFrequency) {
				// start turning left
				aHeadingRotation = -aRotateSpeed;
				// if desired position is right
			} 
			else if (aWantedHeadingRelative > aRotateSpeed*m_fMoveFrequency) {
				// start turning right
				aHeadingRotation = +aRotateSpeed;
				// if desired position is more-less ahead
			} else {
				// keep just the adjusting fraction of speed 
				aHeadingRotation = aWantedHeadingRelative/m_fMoveFrequency;

				if(/*Abs(aHeadingRotation)<15.0f &&*/ _aHeadingRotation == aHeadingRotation)//0914
				{
				return FALSE;
				}

				_aHeadingRotation = aHeadingRotation;//0914
			}
			//////////
						
			// start rotating
			SetDesiredRotation(ANGLE3D(aHeadingRotation, 0, 0));

			if (Abs(aHeadingRotation)>1.0f) 
			{			
			}
			
			if (Abs(aHeadingRotation)<1.0f) 
			{				
				return FALSE;
			}
		// if we may not rotate
		} 
		else {
			// stop rotating
			SetDesiredRotation(ANGLE3D(0, 0, 0));
		}
		return TRUE;	  
	}
	
	// stop desired rotation
	void StopRotating() 
	{
		SetDesiredRotation(ANGLE3D(0, 0, 0));
	};
	
	// stop desired translation
	void StopTranslating() 
	{
		SetDesiredTranslation(FLOAT3D(0.0f, 0.0f, 0.0f));
	};

	FLOAT CalcDistanceInPlaneToDestination(void) 
	{
		// find vector from you to target in XZ plane
		FLOAT3D vNormal;
		GetNormalComponent(m_vDesiredPosition - GetPlacement().pl_PositionVector, en_vGravityDir, vNormal);
		return vNormal.Length();
	};
	
	void MoveNow()
	{
		if (_pGameState->IsRestartGame() == TRUE)
		{
			UIMGR()->GetSystemMenu()->CancelRestart();
		}
		m_bMoving = TRUE;
		SendEvent(EAutoAction());
		//	EAutoAction eAutoAction
	}

	void SetDefaultWearing()
	{

	}

	//0603 kwon 추가.
	// FIXME : (옷 입고, 벗는 부분) 중복두 많고, 엄청 꼬여 있음.
	virtual	void DeleteCurrentArmor(int weartype)
	{	
		//CItemData& ItemData = _pNetwork->MyCurrentWearing[weartype].ItemData;
		if (_pNetwork->MyWearItem[weartype].IsEmptyItem() == TRUE || 
			_pNetwork->MyWearItem[weartype].ItemData == NULL)
		{
			return;
		}

		CItemData* pItemData = _pNetwork->MyWearItem[weartype].ItemData;
		CModelInstance *pMI = GetCurrentPlayerModelInstance();
	
		if (pItemData->GetType() == CItemData::ITEM_SHIELD || pItemData->GetType() == CItemData::ITEM_WEAPON) //0808 무기도 이제 매쉬를 쓴다.
		{
			if(pItemData->GetSubType() == CItemData::ITEM_SHIELD_HEAD && (CTString)pItemData->GetItemSmcFileName() == MODEL_TREASURE)
 			{
 				DeleteDefaultArmor(pItemData->GetSubType());
 			}
 			else
 			{
				_pGameState->TakeOffArmor( pMI, *pItemData );
			}
		}

		//이줄 아래에서 MyCurrentWearing 배열에 있는 실제 CItem도 지워야 한다.
		//CPrintF(TRANS("DeleteCurrentArmor : weartype=%d \n"), weartype);	
	}
	
	virtual	void DeleteDefaultArmor(int type)
	{	
		//CPrintF("DeleteDefaultArmor : type=%d \n",type);
		INDEX iPlayerType = en_pcCharacter.pc_iPlayerType;
		CModelInstance *pMI = GetCurrentPlayerModelInstance();		
		_pGameState->DeleteDefaultArmor( pMI, type, iPlayerType );
		_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
		_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
	}

	// FIXME : 타입 인덱스가 잘만 맞으면 잘 처리될거 같은데...ㅡ.ㅡ
	virtual	void WearingDefaultArmor(int type)
	{
		CModelInstance *pMI = GetCurrentPlayerModelInstance();
		const INDEX iJob = en_pcCharacter.pc_iPlayerType;		
		_pGameState->DeleteDefaultArmor( pMI, type, iJob );

		MeshInstance *mi;
		CTFileName fnFileName;

		CJobInfo* pInfo = CJobInfo::getSingleton();

		if (pInfo == NULL)
		{
			return;
		}

		// FIXME : type 값과 입는 위치값이 다르기 때문에 문제 발생...
		int iWearPos = -1;
		switch( type )
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

		// 바지를 갈아 입을때는 치마도 같이...
		if( type == WEAR_PANTS )
		{
			// Mesh
			fnFileName = pInfo->GetMeshName( iJob, SKIRT );
			if(strlen( fnFileName ) > 0)
			{			
				mi = pMI->AddArmor( fnFileName );

				// Texture
				fnFileName = pInfo->GetTextureName( iJob, SKIRT );
				pMI->AddTexture_t( fnFileName, fnFileName.FileName(), mi );	

				// NormalMap
				fnFileName = pInfo->GetTexNormalName( iJob, SKIRT );
				if(strcmp(fnFileName, ""))
				{					
					pMI->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
				}
			}
		}

		if (type == WEAR_HELMET )// 헬멧 이외 장비 처리
		{
			int nhair = _pNetwork->MyCharacterInfo.hairStyle % 10; // 투구 해제시 디폴트 헤어를 입을 경우 기본 헤어가 산타모자일 경우 기본헤어를 구한다 
			ChangeHairMesh(pMI, iJob, nhair - 1);				   // 공식 : 기본헤어 + (빨간모자 10 HAIR_RED_CAP, 녹색모자 20 HAIR_GREEN_CAP) 
		}else{									
			// Mesh
			fnFileName = pInfo->GetMeshName( iJob, iWearPos );
			mi = pMI->AddArmor( fnFileName );

			// Texture
			fnFileName = pInfo->GetTextureName( iJob, iWearPos );
			pMI->AddTexture_t( fnFileName, fnFileName.FileName(), mi );

			// NormalMap
			fnFileName = pInfo->GetTexNormalName( iJob, iWearPos );
			if(strcmp(fnFileName, ""))
			{			
				pMI->AddTexture_t(fnFileName, fnFileName.FileName(), mi);			
			}	
		}

		//_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
		//_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
	}

	virtual BOOL IsSocialActing()
	{
		if(m_bPlayAction || m_nPlayActionNum!= -1)
		{
			return TRUE;	 
		}
		else
		{
			return FALSE;
		}
	}

	virtual BOOL IsPolymophing()
	{
		if(m_bMobChange || m_bChanging)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	// 변신체 중인가?
	virtual BOOL IsTransforming()
	{		
		if( m_bIsTransform || m_bTransforming)
		{
			return TRUE;
		}
		return FALSE;
	}

	virtual BOOL IsChanging()
	{
		if( m_bChanging || m_bTransforming )
		{
			return TRUE;
		}

		return FALSE;
	}

//사교동작시에 무기를 지웠다가 나타나게 하기.
	void DeleteWearingWeapon(BOOL bException, BOOL bSkillReady)
	{
	    if((bException && m_bDisappearWeapon) || (!bException && !m_bDisappearWeapon))
		//if(!m_bDisappearWeapon)
		{
			if (_pNetwork->MyWearItem[WEAR_BACKWING].IsEmptyItem() == FALSE && bSkillReady)
			{
				CModelInstance *pMI = GetCurrentPlayerModelInstance();

				CItemData* pItemData = _pNetwork->MyWearItem[WEAR_BACKWING].ItemData;
				_pGameState->TakeOffArmor(pMI, *pItemData);
				m_bDisappearWeapon = TRUE;
			}

			if(_pNetwork->MyWearItem[WEAR_WEAPON].IsEmptyItem() == FALSE)
			{
				CModelInstance *pMI = GetCurrentPlayerModelInstance();
				
				CItemData* pItemData = _pNetwork->MyWearItem[WEAR_WEAPON].ItemData;
				_pGameState->TakeOffArmor( pMI, *pItemData );
				_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
				_pNetwork->MyCharacterInfo.itemEffect.DeleteEffect(WEAR_WEAPON);
				_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
			}
			m_bDisappearWeapon = TRUE;
			//물 뿌리기 액션일 때
			if( m_nPlayActionNum ==ACTION_NUM_WATER_SPREAD )
			{
				//바가지 모델 추가
				CModelInstance *pMI = GetCurrentPlayerModelInstance();
				
				UBYTE ubJob = _pNetwork->MyCharacterInfo.job;
				MeshInstance *mi = pMI->AddArmor( _afnDishMeshName[ubJob][0] );
				pMI->AddTexture_t( _afnDishTextureName, _afnDishTextureName.FileName(),mi);	

				pMI->AddArmor( _afnDishMeshName[ubJob][1] );
				_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
				_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);

				//물뿌리는 이펙트 추가
				if( ubJob==0 || ubJob==1 || ubJob==5 || ubJob == 6)	//남자일때...
				{
					StartEffectGroup("water_sm"
							, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				else	//여자일때...
				{
					StartEffectGroup("water_sw"
							, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
			}
		}
	}

	void AppearWearingWeapon(BOOL bSkillEnd)
	{
		if(m_bDisappearWeapon)
		{
			if (_pNetwork->MyWearItem[WEAR_BACKWING].IsEmptyItem() == FALSE && bSkillEnd)
			{
				SWORD nTab, nIdx;
				nTab = _pNetwork->MyWearItem[WEAR_BACKWING].Item_Tab;
				nIdx = _pNetwork->MyWearItem[WEAR_BACKWING].InvenIndex;

				CModelInstance *pMI = GetCurrentPlayerModelInstance();

				_pGameState->WearingArmor( pMI, *_pNetwork->MyWearItem[WEAR_BACKWING].ItemData );
			}

			if (_pNetwork->MyWearItem[WEAR_WEAPON].IsEmptyItem() == FALSE)
			{
				SWORD nTab, nIdx;
				nTab = _pNetwork->MyWearItem[WEAR_WEAPON].Item_Tab;
				nIdx = _pNetwork->MyWearItem[WEAR_WEAPON].InvenIndex;

				CModelInstance *pMI = GetCurrentPlayerModelInstance();			

				//WearingArmor(Tab, Row, Col);
				_pGameState->WearingArmor( pMI, *_pNetwork->MyWearItem[WEAR_WEAPON].ItemData );
				//어피어 메세지가 인벤토리 정보 보다 먼저 들어 온다. 때문에 아이템 정보가 존이동시 없다. 
				//매출 오천 짜리 코드 입니다. 젠장..ㅜ.ㅜ 820420 - 1042920 인터넷 검색 요망 ㅋㅋㅋ
				if (!(nTab < 0 || nIdx))
				{
					CItems&	WearNormalItem = _pNetwork->MySlotItem[nTab][nIdx];
					_pNetwork->MyCharacterInfo.itemEffect.Change( _pNetwork->MyCharacterInfo.job
						, _pNetwork->GetItemData(_pNetwork->MyWearItem[WEAR_WEAPON].ItemData->GetItemIndex())
						, WEAR_WEAPON
						, WearNormalItem.Item_Plus
						, &pMI->m_tmSkaTagManager
						, 1, _pNetwork->GetItemData(_pNetwork->MyWearItem[WEAR_WEAPON].ItemData->GetItemIndex())->GetSubType() );
				}
				_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
				_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
			}
			
			//물 뿌리기 액션일 때 바가지 모델 삭제
			if( m_nPlayActionNum ==ACTION_NUM_WATER_SPREAD )
			{
				CModelInstance *pMI = GetCurrentPlayerModelInstance();
				
				pMI->DeleteMesh( _afnDishMeshName[_pNetwork->MyCharacterInfo.job][0] );
				pMI->DeleteMesh( _afnDishMeshName[_pNetwork->MyCharacterInfo.job][1] );

				GetPlayerWeapons()->m_penRayHitTmp =NULL;	//이 부분이 없으면 액션이 끝난 뒤에 캐릭터 쪽으로 찔끔 움직임...
				m_bWaitForSkillResponse = FALSE;	//이 부분이 없으면 연속으로 액션이 사용안됨...
				m_nCurrentSkillNum =-1;				//위줄이 있고 이 부분이 없으면 계속 애니메이션 반복...
				_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
				_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
			}
			m_bDisappearWeapon = FALSE;
		}				
	}

BOOL CheckSkillBuffer()
{
	if(m_bStartAttack)
	{
		return FALSE;
	}

	if(m_nDesiredSkillNum == -1 && m_nCurrentSkillNum == -1)
	{
		return FALSE;
	}

	if(m_nCurrentSkillNum != -1)
	{
		if(m_bSkilling) //스킬이 Spell중.
		{
			// EDIT : bs : 060322
			if ( m_nReservedSkillNum == m_nCurrentSkillNum)
			{
				m_nReservedSkillNum = -1;
			}
			// EDIT : bs : 060322
			if(m_nDesiredSkillNum != -1 && m_nReservedSkillNum == -1)
			{
				m_nReservedSkillNum		= m_nDesiredSkillNum; //메모라이즈
				m_nDesiredSkillNum		= -1;				
			}
			else if(m_nDesiredSkillNum != -1 && m_nReservedSkillNum != -1)//예약할 공간이 없다.
			{
				return TRUE;// Date : 2005-09-21(오후 2:34:30), By Lee Ki-hwan
			}
			else //이동후 스킬 사용 루틴.
			{
				return TRUE;
			}
		}
		else //현재 Current Skill이 대기중.
		{
            //예약된 스킬 시작시에도 이쪽으로 들어온다.
			if(!m_bStartAttack)
			{
				//CPrintF("Ready skill()!!!\n");
				return TRUE;
			}
			else //attack 애니메이션 중간.
			{
				return FALSE;
			}
		}
	}
	else //Current Skill 세팅.
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();
		ObjInfo* pInfo = ObjInfo::getSingleton();

		CEntity *penTargetReserve = GetPlayerWeapons()->m_penReservedTarget;

		CommandAttack();//0212 공격명령이 아니라 타겟 세팅이다.

		CEntity *penTarget = GetPlayerWeapons()->m_penRayHitTmp;
		if(GetPlayerWeapons()->m_penRayHitTmp 
			&& (GetPlayerWeapons()->m_penRayHitTmp->GetFlags()&ENF_ALIVE)
			&& ((GetPlayerWeapons()->m_penRayHitTmp->IsEnemy()) || (GetPlayerWeapons()->m_penRayHitTmp->IsPet()) || (GetPlayerWeapons()->m_penRayHitTmp->IsSlave())
			|| ((GetPlayerWeapons()->m_penRayHitTmp->IsCharacter()) || (GetPlayerWeapons()->m_penRayHitTmp->IsWildPet()) 
			&& (IsPvp()|| IsLegitTarget(penTarget) || _pUISWDoc->IsWar() || pUIManager->GetGuildBattle()->IsEnemy( penTarget->en_ulID ) )))//1203
		)
		{
			CSkill &SkillData = _pNetwork->GetSkillData(m_nDesiredSkillNum);
			int nSkillLevel;
			
			if ( SkillData.GetFlag() & SF_GUILD )
			{
				nSkillLevel = pUIManager->GetGuild()->GetGuildSkillLevel( SkillData.GetIndex() );
			}
			else if ( SkillData.GetJob() == PET_JOB )
			{ // 펫 스킬의 경우
				nSkillLevel = MY_INFO()->GetPetSkillLevel(pInfo->GetMyPetInfo()->lIndex, m_nDesiredSkillNum);
			}
			else if (SkillData.Skill_Data.job == WILDPET_JOB)
			{
				nSkillLevel = MY_INFO()->GetPetSkillLevel(0, m_nDesiredSkillNum);
			}
			else
			{
				nSkillLevel = MY_INFO()->GetSkillLevel(m_nDesiredSkillNum);
			}

		// Date : 2005-10-20(오전 9:18:39), By Lee Ki-hwan
		// nSkillLevel에 -1을 한 이유
		// GEtNeedMP의 Index 0~시작 실제 스킬 레벨은 1부터 시작 

			int needMP = SkillData.GetNeedMP( nSkillLevel - 1 );
			int iMPReducRate = pUIManager->GetNeedMPReductionRate();
			needMP -= (needMP*iMPReducRate/100);

			if(SkillData.GetToggle() == false && needMP > _pNetwork->MyCharacterInfo.mp)
			{
				m_nDesiredSkillNum = -1;
				_pNetwork->ClientSystemMessage( _S( 320, "MP가 부족합니다." ), SYSMSG_ERROR );
			}

			m_bWaitForSkillTarget	= FALSE;
			m_nCurrentSkillNum		= m_nDesiredSkillNum;//여기가 실제 스킬 적용시점.
			m_nDesiredSkillNum		= -1; //초기화.
		}
		else
		{
			//0805
			CSkill &SkillData = _pNetwork->GetSkillData(m_nDesiredSkillNum);
			
			int nSkillLevel;

			if (SkillData.Skill_Data.job == PET_JOB)
			{ // 펫 스킬의 경우
				nSkillLevel = MY_INFO()->GetPetSkillLevel(pInfo->GetMyPetInfo()->lIndex, m_nDesiredSkillNum);
			}
			else if (SkillData.Skill_Data.job == WILDPET_JOB)
			{
				nSkillLevel = MY_INFO()->GetPetSkillLevel(0, m_nDesiredSkillNum);
			}
			else if ( SkillData.GetFlag() & SF_GUILD )
			{
				nSkillLevel = pUIManager->GetGuild()->GetGuildSkillLevel( m_nDesiredSkillNum );
			}
			else
			{
				nSkillLevel = MY_INFO()->GetSkillLevel(m_nDesiredSkillNum);
			}

			if ( nSkillLevel == 0 ) 
			{// 0이상만 허용
				m_nDesiredSkillNum = -1;
				return TRUE;
			}

			int needMP = SkillData.GetNeedMP( nSkillLevel - 1 );
			int iMPReducRate = pUIManager->GetNeedMPReductionRate();
			needMP -= (needMP*iMPReducRate/100);

			if(SkillData.GetToggle() == false && needMP > _pNetwork->MyCharacterInfo.mp)
			{
				m_nDesiredSkillNum = -1;
				_pNetwork->ClientSystemMessage( _S( 320, "MP가 부족합니다." ), SYSMSG_ERROR );
			}
			else if(penTarget == this) //0807
			{
				m_bWaitForSkillTarget	= FALSE;
				m_nCurrentSkillNum		= m_nDesiredSkillNum;//여기가 실제 스킬 적용시점.
				m_nDesiredSkillNum		= -1; //초기화.
			}
			// 타 캐릭터에 대해서...
			else if(GetPlayerWeapons()->m_penRayHitTmp && (GetPlayerWeapons()->m_penRayHitTmp->IsCharacter()))
			{
				m_bWaitForSkillTarget	= FALSE;
				m_nCurrentSkillNum		= m_nDesiredSkillNum;//여기가 실제 스킬 적용시점.
				m_nDesiredSkillNum		= -1; //초기화.
			}
			else
			{
				// 타겟팅이 필요없는 스킬.
				if( !SkillData.IsNeedTarget() )				
				{
//					SetTargetMe();
					m_bWaitForSkillTarget	= FALSE;
					m_nCurrentSkillNum		= m_nDesiredSkillNum;//여기가 실제 스킬 적용시점.
					m_nDesiredSkillNum		= -1; //초기화.
					return TRUE;
				}

				m_bWaitForSkillTarget = TRUE;
			}
		}
	}
	return TRUE;
}

// 스킬 사용 가능 유무 체크.
virtual BOOL CheckSkill(void)
{
	if(m_bStuned) {return TRUE;}

	if(!CheckSkillBuffer())
	{
		return FALSE;
	}

	if(m_nCurrentSkillNum == -1)
	{
		return FALSE;
	}

	CUIManager* pUIManager = SE_Get_UIManagerPtr();

	if (pUIManager->IsCSFlagOn(CSF_PETRIDING) || pUIManager->IsCSFlagOn(CSF_TELEPORT))
	{
		return FALSE;
	}

	if(m_bSkilling || m_bStartAttack)
	{
		return TRUE;
	}
	if (pUIManager->GetInitJob()->IsFaceDecoMode())
	{
		//_pNetwork->ClientSystemMessage(_S( 5184, "외형 변경 중에는 스킬을 사용할 수 없습니다." ), SYSMSG_ERROR);
		m_nCurrentSkillNum = -1;
		return FALSE;
	}
	CEntity *penTarget = GetPlayerWeapons()->m_penRayHitTmp;	
	ObjInfo* pInfo = ObjInfo::getSingleton();
	CSkill &SkillData = _pNetwork->GetSkillData(m_nCurrentSkillNum);//0807
	int curlv = MY_INFO()->GetSkillLevel(m_nCurrentSkillNum);

	if (curlv == 0)
	{
		if ( SkillData.GetJob() == PET_JOB )
		{ // 펫 스킬의 경우
			if (pInfo->GetMyPetInfo()->lIndex < 0)
			{
				return FALSE;
			}

			LONG lRemain;

			if (IsSealPet(&lRemain) == true)
			{
				m_nCurrentSkillNum = -1;

				CTString str;
				str.PrintF(_S(2508, "펫 봉인해제 시간이 %d시간 %d분 남았습니다."), lRemain / 3600, (lRemain % 3600) / 60);
				UIMGR()->GetChattingUI()->AddSysMessage(str, SYSMSG_ERROR);

				return FALSE;
			}

			curlv = MY_INFO()->GetPetSkillLevel( pInfo->GetMyPetInfo()->lIndex, m_nCurrentSkillNum );
		}
	}
	
	if (m_nCurrentSkillNum != 688 && _pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
	{ // 비행 모드에서는 스킬을 사용할 수 없다.
		_pNetwork->ClientSystemMessage(_S( 4684, "비행 모드에서 사용할 수 없는 스킬입니다." ), SYSMSG_ERROR);
		m_nCurrentSkillNum = -1;
		return FALSE;
	}

	if (m_nCurrentSkillNum == 688 && !(_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING))
	{
		if (_pNetwork->MyWearItem[WEAR_BACKWING].IsEmptyItem() == TRUE)
		{
			_pNetwork->ClientSystemMessage(_S( 4685, "날개를 착용한 상태에서 사용이 가능합니다." ), SYSMSG_ERROR);
			m_nCurrentSkillNum = -1;
			return FALSE;
		}
	}

	// 사도 모드 일때
	if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_DARKNESS)
	{// 사도 모드 일때 사용할 수 있는 스킬인가?
		if (!(SkillData.Skill_Data.useState & SCT_DARKNESS))
		{
			_pNetwork->ClientSystemMessage(_S(4686, "사도 모드에서 사용할 수 없는 스킬입니다." ), SYSMSG_ERROR);
			m_nCurrentSkillNum = -1;
			return FALSE;
		}
	}

	if (_pNetwork->MyCharacterInfo.sbSoulCount < SkillData.Skill_Data.useSoulCount)
	{
		_pNetwork->ClientSystemMessage(_S(4719, "영혼의 갯수가 부족합니다."), SYSMSG_ERROR);
		m_nCurrentSkillNum = -1;
		return FALSE;
	}

	const int iFlag = SkillData.GetSorcererFlag();

	// 강신한 상태의 경우.
	if( m_bIsTransform )
	{
		// 강신체 스킬만 사용할 수 있음
		if( !( ( iFlag & ( SSF_USE_HELLOUND | SSF_USE_ELENEN ) ) ) )
		{
			m_nCurrentSkillNum = -1;
			return FALSE;			
		}		
	}
	// 그 외의 경우에는.
	else
	{
		// 소서러 플래그 값이 있는 상태에서, 캐릭터의 스킬이 아닌경우 사용할 수 없음.
		if( iFlag && !( iFlag & SSF_USE_CHARACTER ) )
		{
			m_nCurrentSkillNum = -1;
			return FALSE;
		}
	}

	if( penTarget == NULL && SkillData.IsNeedTarget() )
	{
		m_nCurrentSkillNum = -1;
		return FALSE;
	}

	if( penTarget != NULL && (penTarget->IsFirstExtraFlagOn(ENF_EX1_NPC) )&& SkillData.IsNeedTarget() )
	{
		m_nCurrentSkillNum = -1;
		return FALSE;
	}

	if ( SkillData.GetTargetType() == CSkill::STT_GUILD_ONE )
	{
		INDEX chaindex = penTarget->GetNetworkID();

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, chaindex);

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			if ( pTarget->cha_lGuildIndex != _pNetwork->MyCharacterInfo.lGuildIndex )
			{
				m_nCurrentSkillNum = -1;
				_pNetwork->ClientSystemMessage(_S( 3748, "길드원이 아닙니다." ), SYSMSG_ERROR);
				return FALSE;
			}
		}
	}
	BOOL bLostTarget, bAutoAttack;
	if(!SkillCondition(m_nCurrentSkillNum, penTarget, bLostTarget, bAutoAttack))
	{
		CancelSkill(FALSE, FALSE, FALSE);//스킬 캔슬.
		return FALSE;
	}

	if(m_bCannotUseSkill)
	{
		_pNetwork->ClientSystemMessage(_S( 1450, "현재 스킬을 사용할 수 없는 상태입니다." ), SYSMSG_ERROR);
		CancelSkill(FALSE, g_iAutoAttack, FALSE);
		return FALSE;
	}
	if (_pNetwork->MyWearItem[WEAR_WEAPON].IsEmptyItem() == FALSE)
	{
		int nUsingWeapon = 0;
		BOOL bCanUse = TRUE;

		if (SkillData.Skill_Data.useWeaponType0 != -1 && SkillData.Skill_Data.useWeaponType1 == -1)
		{
			nUsingWeapon = 1;
		}
		else if (SkillData.Skill_Data.useWeaponType0 == -1 && SkillData.Skill_Data.useWeaponType1 != -1)
		{
			nUsingWeapon = 2;
		}
		else if (SkillData.Skill_Data.useWeaponType0 != -1 && SkillData.Skill_Data.useWeaponType1 != -1)
		{
			nUsingWeapon = 3;
		}

		switch (nUsingWeapon)
		{
		case 1: // WeaponType0만 있을 때
			{
				if (_pNetwork->MyWearItem[WEAR_WEAPON].ItemData->GetSubType() != SkillData.Skill_Data.useWeaponType0)
				{
					bCanUse = FALSE;
				}
			}
			break;
		case 2: // WeaponType1만 있을 때
			{
				if (_pNetwork->MyWearItem[WEAR_WEAPON].ItemData->GetSubType() != SkillData.Skill_Data.useWeaponType1)
				{
					bCanUse = FALSE;
				}
			}
			break;
		case 3: // WeaponType0,WeaponType1 다 있을 때
			{
				if (_pNetwork->MyWearItem[WEAR_WEAPON].ItemData->GetSubType() != SkillData.Skill_Data.useWeaponType0 &&
					_pNetwork->MyWearItem[WEAR_WEAPON].ItemData->GetSubType() != SkillData.Skill_Data.useWeaponType1)
				{
					bCanUse = FALSE;
				}
			}
			break;
		}

		if (!bCanUse)
		{
			CTString strTemp;
			strTemp.PrintF( _S( 298, "%s 스킬을 스펠합니다."), SkillData.GetName() );
			_pNetwork->ClientSystemMessage( strTemp, SYSMSG_ERROR );
			_pNetwork->ClientSystemMessage( _S( 1451, "알맞는 무기를 장비해야 사용할 수 있는 스킬입니다."), SYSMSG_ERROR );
			CancelSkill(FALSE, g_iAutoAttack, FALSE);
			return FALSE;
		}

/*		if( (SkillData.Skill_Data.useWeaponType0 != -1 &&
			_pNetwork->pMyWearItem[WEAR_WEAPON]->pItemData->GetSubType() == SkillData.Skill_Data.useWeaponType0) )
		&& (SkillData.Skill_Data.useWeaponType1 != -1 &&
			_pNetwork->pMyWearItem[WEAR_WEAPON]->pItemData->GetSubType() != SkillData.Skill_Data.useWeaponType1) )
		{
			_pNetwork->ClientSystemMessage( _S( 1451, "알맞는 무기를 장비해야 사용할 수 있는 스킬입니다."), SYSMSG_ERROR );
			CancelSkill(FALSE, g_iAutoAttack, FALSE);
			return FALSE;
		}*/
	}

	if(!m_bStartAttack)//이동 중이라면
	{		
		if(SkillData.IsSingleDungeonSkill())
		{
			if(!_pNetwork->m_bSingleMode)
			{	
				_pNetwork->ClientSystemMessage( _S( 404, "퍼스널 던젼에서만 사용할 수 있는 스킬입니다." ), SYSMSG_ERROR );

				CancelSkill(FALSE, g_iAutoAttack, FALSE);
				return FALSE;
			}
		}

		// 타겟팅이 필요없는 스킬.
		if( !SkillData.IsNeedTarget() )
		{
			StopMove();
			if(!m_bWaitForSkillResponse)
			{
				if( SkillData.GetTargetNum(curlv) > 1 )
				{
					m_dcEnemies.Clear();

					// 주 타겟 에너미.								
					m_dcEnemies.Add(this);

					m_bWaitForSkillResponse = TRUE;

					_pNetwork->SendSkillMessageInContainer(m_nCurrentSkillNum, m_dcEnemies, FALSE);

					const char chTargetType = SkillData.GetTargetType();

					// 자기 중심으로 멀티 공격
					if( chTargetType == CSkill::STT_SELF_RANGE || chTargetType == CSkill::STT_GUILD_SELF_RANGE )
					{
						_pNetwork->FindTargetsInRange(this, this, m_dcEnemies, 
							SkillData.GetAppRange(), SkillData.GetTargetNum(curlv), 360.0f, m_nCurrentSkillNum );
					}

					m_tmSendSkillMessage = _pTimer->CurrentTick();
				}
				else
				{
					// 강신 스킬은 캐릭터 변환이므로 변환 스테이터스에 저장
					if(m_nCurrentSkillNum == 309 || m_nCurrentSkillNum == 313)
					{
						//	김영환	
						_pNetwork->Set_MyChar_MorphStatus_EVOCATION_BEGIN();
						//_pNetwork->MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_EVOCATION_BEGIN;
					}
					
					m_bWaitForSkillResponse = TRUE;
					_pNetwork->SendSkillMessage(m_nCurrentSkillNum, this, _pNetwork->MyCharacterInfo.index, FALSE);//우선 타겟은 몹만...					
					
					m_tmSendSkillMessage = _pTimer->CurrentTick();
				}
			}
		
			return TRUE;
		}
		else
		{
			if( penTarget )
			{
				if( !( penTarget->IsPlayer() || penTarget->IsEnemy() || penTarget->IsCharacter() || penTarget->IsPet() || penTarget->IsSlave() || penTarget->IsWildPet() ) )
				{
					return FALSE;
				}

				if( !CheckAttackTarget( m_nCurrentSkillNum, penTarget, 0.0f ) )
				{
					CTString strSysMessage;				
					strSysMessage.PrintF( _S( 406, "잘못된 타겟입니다. %s 스킬을 취소합니다." ), SkillData.GetName() );

					_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );

					CancelSkill(FALSE, g_iAutoAttack, FALSE);
					return FALSE;		
				}

				if(!IsVisibleCheckAll(penTarget))
				{
					CTString strSysMessage;
					strSysMessage.PrintF(_S( 2086, "가로막혀서 스킬을 사용할수 없습니다." ));	// 번역
					_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);	
					GetPlayerWeapons()->m_penReservedTarget = penTarget;
					penTarget = NULL;	
											
					m_nCurrentSkillNum = -1;
					return FALSE;
				}
				//사정거리 체크
				FLOAT3D vDelta = GetPlacement().pl_PositionVector - penTarget->GetPlacement().pl_PositionVector;		
				vDelta(2) = 0.0f;

				FLOAT3D vTargetPos(0, 0, 0);
				FLOAT3D vDirection(0, 0, 0);

				CEntity *pen = penTarget;
				FLOAT3D vCurrentCenter(((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[0],
					((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[1],
					((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[2] );
				FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vCurrentCenter;
				FLOAT3D vTargetCenter(0, 0, 0);
				FLOAT size = 0;
				if(pen != NULL && pen->GetFlags() & ENF_ALIVE)
				{
					if(pen->en_RenderType == RT_SKAMODEL)
					{
						// Bounding Box를 이용하여 Effect를 찍을 위치를 찾는다.
						FLOATaabbox3D bbox;
						pen->GetModelInstance()->GetCurrentColisionBox(bbox);
						FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*단순히 원으로 하면 너무 큼. 사이즈 조절 상수*/;
						size = vRadius.Length();
					}
					vTargetCenter = FLOAT3D(((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[0],
						((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[1],
						((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[2] );
					vTargetPos = pen->en_plPlacement.pl_PositionVector + vTargetCenter;
					vDirection = vTargetPos - vCurrentPos;
					vDirection.Normalize();
					vTargetPos -= vDirection * size;
				}
				vDirection.Normalize();
				
				FLOAT fScaledSize = 0.f;

				INDEX chaindex = penTarget->GetNetworkID();

				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, chaindex);

				if (pObject != NULL)
				{
					CMobTarget* pTarget = static_cast< CMobTarget* >(pObject);

					CMobData* MD = CMobData::getData(pTarget->m_nType);

					if (MD != NULL && MD->index > 0)
					{
						fScaledSize = MD->GetScaledSize();
					}
				}

				m_fSkillDistance = SkillData.GetFireRange();//0829
				if (vDelta.Length() <= (m_fSkillDistance + fScaledSize))
				{				
					//CPrintF("SpellSkill()!!!\n");
					if(ulNewButtons&PLACT_FIRE && (m_bForward))
					{
						m_bForward = FALSE;
					}			
					StopMove();

					//0805
					//SpellSkill(m_nCurrentSkillNum);//실제 스펠 시점.
	
					if(!m_bWaitForSkillResponse)
					{
						if((_pNetwork->MyWearItem[WEAR_WEAPON].IsEmptyItem() == TRUE) && (SkillData.Skill_Data.useState & SCT_WEAPON))
						{
							CTString strSysMessage = _S( 702, "무기를 장착해야 사용할수 있는 스킬입니다." );				
		
							_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );

							CancelSkill(FALSE, g_iAutoAttack, FALSE);
			
							return FALSE;
						}
						if(SkillData.Skill_Data.useState & SCT_WEAPON)//생산도구 착용시 스킬 사용 안되도록...
						{
							if(_pNetwork->MyWearItem[WEAR_WEAPON].IsEmptyItem() == FALSE)
							{
								// FIXME : CItemData 값을 읽어올때 미리 계산하면 처리하기 쉬운 부분...
								const int iWeaponType = _pNetwork->MyWearItem[WEAR_WEAPON].ItemData->GetSubType();
								if( iWeaponType == CItemData::ITEM_WEAPON_MINING ||
									iWeaponType == CItemData::ITEM_WEAPON_GATHERING || 
									iWeaponType == CItemData::ITEM_WEAPON_CHARGE )
								{
									CTString strSysMessage = _S( 702, "무기를 장착해야 사용할수 있는 스킬입니다." );				
	
									_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
	
									CancelSkill(FALSE, g_iAutoAttack, FALSE);
			
									return FALSE;
								}
							}
						}
						if((_pNetwork->MyWearItem[WEAR_SHIELD].IsEmptyItem() == TRUE) && (SkillData.Skill_Data.useState & SCT_SHIELD))
						{
							CTString strSysMessage;				
							
							strSysMessage.PrintF(_S( 2087, "방패를 장착해야 사용할수 있는 스킬입니다." ));	// 번역
							_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );

							CancelSkill(FALSE, g_iAutoAttack, FALSE);
			
							return FALSE;
						}
						
						SLONG MobIndex	= -1;
						if(penTarget != NULL)
						{
							MobIndex = penTarget->GetNetworkID();
						}

						m_penAttackingEnemy = penTarget;

						CUIManager* pUIManager = SE_Get_UIManagerPtr();

						// 내가 뭔가를 공격하는 경우에, 소환수도 협공 하도록...
						for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
						{
							CUISummon* pUISummon = (CUISummon*)pUIManager->GetUI(i);
							CEntity* pSummonEntity = pUISummon->GetSummonEntity();
							if( pSummonEntity )
							{									
								if( pSummonEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE) )
								{						
									if( m_penAttackingEnemy != ((CSlave*)pSummonEntity)->m_penEnemy )
									{
										pSummonEntity->SendEvent(ESlaveKilledEnemy());
									}
								}
							}
						}

						// 공격 펫도 같이 공격
						if(pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->bIsActive)
						{
							CEntity* pWildPetEntity = pInfo->GetMyApetInfo()->m_pEntity;
							if(pWildPetEntity)
							{
								if(pWildPetEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_WILDPET) &&
									(m_penAttackingEnemy != ((CWildPet*)pWildPetEntity)->m_penEnemy))
								{
									pWildPetEntity->SendEvent(ESlaveKilledEnemy());

								}									
							}
						}

						m_penStillTarget = penTarget; // 귀찮아

						m_bWaitForSkillResponse = TRUE;

						// 여기서 범위 공격 처리해줄것...
						if( SkillData.GetTargetNum(curlv) > 1 )
						{
							// 주 타겟 에너미.								
							m_dcEnemies.Clear();
							m_dcEnemies.Add(m_penAttackingEnemy);	// this need to ready status.

							_pNetwork->SendSkillMessageInContainer(m_nCurrentSkillNum, m_dcEnemies, FALSE);

							const char chTargetType = SkillData.GetTargetType();
							FLOAT fAngle = 360.0f;
							// 자기 중심으로 120도
							if( chTargetType == CSkill::STT_TARGET_D120 )
							{
								fAngle = 60.0f;								
							}							

							m_dcEnemies.Add(m_penAttackingEnemy);	// this need to fire status of target member.

							_pNetwork->FindTargetsInRange(this, m_penAttackingEnemy, m_dcEnemies, 
															SkillData.GetAppRange(), SkillData.GetTargetNum(curlv) - 1, fAngle, m_nCurrentSkillNum, chTargetType );
						}
						else
						{
							if(_pNetwork->m_bSingleMode)
							{
								if( CheckAttackTarget( m_nCurrentSkillNum, penTarget, 0.0f ) )
								{
									_pNetwork->SendSkillMessage(m_nCurrentSkillNum, penTarget, MobIndex, FALSE);//우선 타겟은 몹만...
									m_dcEnemies.Add(penTarget);
								}
								else
								{
									return FALSE;
								}
							}
							else
							{
								if( CheckAttackTarget( m_nCurrentSkillNum, penTarget, 0.0f ) )
								{
									//물뿌리기 스킬일 경우 물뿌리기 액션 실행
									if( m_nCurrentSkillNum ==436 )
									{
										((CPlayerEntity*)CEntity::GetPlayerEntity(0))->UseAction(ACTION_NUM_WATER_SPREAD);
									}
									else
									{
										_pNetwork->SendSkillMessage(m_nCurrentSkillNum, penTarget, MobIndex, FALSE);//우선 타겟은 몹만...									
									}
								}
								else
								{
									return FALSE;
								}
							}
						}
						
						m_tmSendSkillMessage = _pTimer->CurrentTick();
					}
					return TRUE;
				}
				else
				{
					// 이동.
					return FALSE;
				}
			}
			// 자신을 타겟으로 하는 경우...
			else if(penTarget == this)//0807
			{
				if(SkillData.GetType() == CSkill::ST_MAGIC || SkillData.GetType() == CSkill::ST_SUMMON_TOTEM_SKILL)
				{
					// 타겟이 필요한 스킬일 경우...(자기한테 쓰면 안됨.)
					if( SkillData.GetTargetType() == CSkill::STT_TARGET_ONE || 
						SkillData.GetTargetType() == CSkill::STT_TARGET_RANGE ||
						SkillData.GetTargetType() == CSkill::STT_TARGET_D120 ||
						SkillData.GetTargetType() == CSkill::STT_TARGET_RECT )
					{
						CTString strSysMessage;				
						strSysMessage.PrintF( _S( 406, "잘못된 타겟입니다. %s 스킬을 취소합니다." ), SkillData.GetName() );

						_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );

						CancelSkill(FALSE, g_iAutoAttack, FALSE);
						return FALSE;
					}
			
					StopMove();
						
					if(!m_bWaitForSkillResponse)
					{
						m_bWaitForSkillResponse = TRUE;
						_pNetwork->SendSkillMessage(m_nCurrentSkillNum, penTarget, _pNetwork->MyCharacterInfo.index, FALSE);//0이 플레이어.
						
						m_tmSendSkillMessage = _pTimer->CurrentTick();
					}

					return TRUE;
				}
				else
				{
					CTString strSysMessage;				
					strSysMessage.PrintF( _S( 406, "잘못된 타겟입니다. %s 스킬을 취소합니다." ), SkillData.GetName() );
					_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );

					CancelSkill(FALSE, g_iAutoAttack, FALSE);
					//penTarget = NULL;
					return FALSE;
				}
			}
			else //타겟이 없다면, 정지.스킬 발동. 계속 정지.
			{				
				m_bWaitForSkillTarget = TRUE;			
				return FALSE;
			}
		}
	}
	else if(m_bStartAttack) //공격중이라면,
	{
		return FALSE;
	}
	else //멈춘 상태라면//여긴 안들어온다~//0709
	{	
		//SpellSkill(m_nCurrentSkillNum);//스킬 발동//1105
		CPrintF("here...\n");
		return FALSE;	
		//return TRUE;
	}
}

void SpellSkill(INDEX m_nCurrentSkillNum)
{
	BOOL bHardcodingSkill = HardCodingSkill(TRUE);
	if(!m_bSendStopMessage)
	{					
		m_bForward = TRUE;
		//m_bKeyMove = TRUE;
		StopMove();
	}

	FLOAT skillSpeed = (100 - _pNetwork->MyCharacterInfo.skillSpeed) / 100.0f;

	//CPrintF("SpellSkill()!!! m_idCurrentSkillAnim=%f..\n",m_idCurrentSkillAnim);

	CSkill &SkillData = _pNetwork->GetSkillData(m_nCurrentSkillNum);
	
	INDEX iTempAnim = _pNetwork->MyCharacterInfo.bExtension;

	// 비행 스킬에서 토글 동작을 위해서 처리 /////////////////////////////////
	if (m_nCurrentSkillNum == 688 && !(_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING))
	{
		//ObtainModelInstance_t(GetModelInstance(), CTString("Data\\Item\\shield\\Nightshadow\\ns_fly.smc"));
		AddSkaModel(CTString("Data\\Item\\shield\\Nightshadow\\ns_fly.smc"));
		DeleteWearingWeapon(FALSE, TRUE);
		iTempAnim = 0;

		// Broadcast 시 에 상태가 바뀌므로 임시로 비행 상태로 만든다.
		_pNetwork->MyCharacterInfo.ulPlayerState |= PLAYER_STATE_FLYING;		
	}
	else if (m_nCurrentSkillNum == 688 && _pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
	{
		DelSkaModel(CTString("Data\\Item\\shield\\Nightshadow\\ns_fly.smc"));
		AppearWearingWeapon(TRUE);
		iTempAnim = 1;

		// Broadcast 시 에 상태가 바뀌므로 임시로 비행 상태를 꺼준다.
		_pNetwork->MyCharacterInfo.ulPlayerState &= ~PLAYER_STATE_FLYING;
	}
	//////////////////////////////////////////////////////////////////////////
	
	//if(!bHardcodingSkill)
	{
		if(SkillData.bCanCancel)//0815 디바인 실드는 캔슬 안된대요..ㅡㅡ
		{
			m_idCurrentSkillAnim = SkillData.idPlayer_Anim_Skill[iTempAnim][0];
			//CPrintF("m_idCurrentSkillAnim = %d\n",m_idCurrentSkillAnim);
		}
		if(m_idCurrentSkillAnim == -1)
		{
			SkillData.bCanCancel = FALSE;
			m_idCurrentSkillAnim = SkillData.idPlayer_Anim_Skill[iTempAnim][2];
			//CPrintF("m_idCurrentSkillAnim = %d\n",m_idCurrentSkillAnim);
		}
		if ( m_idCurrentSkillAnim == -1 && SkillData.GetFlag()&SF_GUILD)
		{
			m_bNoAniGuildSkill = TRUE;
		}
		if(m_idCurrentSkillAnim == -1 && !m_bNoAniGuildSkill)
		{
			//m_nCurrentSkillNum			= -1;
			//m_nDesiredSkillNum			= -1;
			return;
		}
	}
	if ( !m_bNoAniGuildSkill )
	{
		NewClearState(CLEAR_STATE_LENGTH);
	}

	m_fSkillAnimTime = GetModelInstance()->GetAnimLength(m_idCurrentSkillAnim) * skillSpeed;
	m_tmSkillStartTime = _pTimer->CurrentTick();
	m_bSkilling = TRUE;

	m_bLockMove = TRUE;

//0722 
	//if(!bHardcodingSkill)
	{
		if(SkillData.bCanCancel == FALSE || m_bNoAniGuildSkill)//0815 디바인 실드
		{
			g_bPreSkill = FALSE;
			g_bDoSkill = FALSE;
			g_bPostSkill = TRUE;
		}
		else{
			g_bPreSkill = TRUE;
			g_bDoSkill = FALSE;
			g_bPostSkill = FALSE;
		}	
		
		AddAnimation(m_idCurrentSkillAnim, AN_NORESTART, 1.0f, PLAYER_ANIM_GROUP_WHOLEBODY,
			ESKA_MASTER_MODEL_INSTANCE, skillSpeed);
	}

	SE_Get_UIManagerPtr()->SetCSFlagOn( CSF_SKILL );//1104
}

//0824
BOOL CheckAction()
{
	if(m_nPlayActionNum == -1)
	{
	return FALSE;
	}

	if(m_bSkilling || m_bStartAttack || (m_bForward || m_bKeyMove))
	{
		m_bPlayAction = FALSE;
		m_nPlayActionNum = -1;
		m_nActionSit = 0;
		
		//_pNetwork->ClientSystemMessage( _S( 405, "공격중이거나 스킬 사용중, 이동중에는 앉을수 없습니다." ), SYSMSG_ERROR );

		return FALSE;
	}

	if(m_bPlayAction)
	{
		StopMove();
		return TRUE;
	}	
	return FALSE;	
}

void SkillAnimation(void)
{
	static BOOL bEffect = TRUE;
	//static ULONG IndexStick = 1;
	static CEntity* penEntity;		
	
	//0822
	float _PreSkillTime;//, _DoSkillTime, _PostSkillTime;
	static BOOL  bPlayPreSkill  = TRUE; 
	static BOOL  bPlayDoSkill  = FALSE;
	static BOOL  bPlayPostSkill  = FALSE;
	static INDEX step = 0;	
	
	//	static INDEX iSkillEffectStep = 0;//1107 위치 이동.
	
	FLOAT3D vDelta;
	CEntity *penTarget = GetPlayerWeapons()->m_penRayHitTmp;
	
	CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
	
	CUIManager* pUIManager = SE_Get_UIManagerPtr();
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (skill.IsNeedTarget() && 
		!(skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_NOTHING && 
		  g_bPostSkill && 
		  !m_bReadySendSkillMessage))
	{
		if(penTarget != NULL && (penTarget->IsEnemy() || penTarget->IsPet() || penTarget->IsSlave() || penTarget->IsWildPet() ) )
		{
			vDelta = GetPlacement().pl_PositionVector - penTarget->GetPlacement().pl_PositionVector;
			vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.
			Rotate(-vDelta, 360.0f, TRUE);
		}
		else if(penTarget != NULL && penTarget == this) //타겟이 자신이다..
		{
			StopMove();
		}
		else if(penTarget != NULL && penTarget->IsCharacter() )//1211 죽은 캐릭도 타게팅 가능.
		{
			if(g_bPostSkill)
			{
				vDelta = GetPlacement().pl_PositionVector - penTarget->GetPlacement().pl_PositionVector;
				vDelta(2) = 0.0f; //우선 높이는 생각하지 않는다.
				Rotate(-vDelta, 360.0f, TRUE);
			}
		}
		// 타겟이 없는 경우?
		// (셀프 스킬???)
/*
		원거리 파티 등 유효하지 않은 타겟도 있으므로, 
		else if (bPlayPreSkill == TRUE)
		{			
			_pNetwork->SendSkillCancelMessage();
			m_bWaitForSkillResponse		= FALSE;//0103
			
			m_bReadySendSkillMessage	= TRUE;			
			g_bPostSkill				= FALSE;
			m_iSkillEffectStep			= 0;
			m_idCurrentSkillAnim		= -1;//스킬 종료.
			m_tmSkillStartTime			= 0.0f;
			m_bSkilling					= FALSE;
			m_nCurrentSkillNum			= -1;
			m_nDesiredSkillNum			= -1;
			m_bLockMove					= FALSE;
			m_bLockSkillCancel			= FALSE;
			
			if( penTarget == NULL && m_pSkillReadyEffect != NULL &&
				CEffectGroupManager::Instance().IsValidCreated( m_pSkillReadyEffect ) )
			{
				m_pSkillReadyEffect->Stop( 0.1f );
				m_pSkillReadyEffect = NULL;
			}
			
			bPlayPreSkill = TRUE;//0731 촬영용 임시코드
			step = 0;
			
			NewClearState(CLEAR_STATE_LENGTH); 
			IdleAnim();			
			
			m_nReservedSkillNum = -1;
			
			pUIManager->SetCSFlagOff( CSF_SKILL );
			
			return;//1216
		}
*/
	}
	else if(skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_NOTHING && 
			g_bPostSkill && 
			!m_bReadySendSkillMessage)
	{
		StopMove();
	}

	
	const int iJob = en_pcCharacter.pc_iPlayerType;
	
	m_bLockSkillCancel = FALSE;
	
	FLOAT skillSpeed = (100 - _pNetwork->MyCharacterInfo.skillSpeed) / 100.0f;

	//---------------------------------------------------
	// 스킬 사용전.
	if(g_bPreSkill)
	{
		if(m_iSkillEffectStep == 0)
		{
			++m_iSkillEffectStep;
			//시전 이펙트
			//CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
			m_pSkillReadyEffect = StartEffectGroup(skill.GetReadyEffect(_pNetwork->MyCharacterInfo.bExtension)
				, &en_pmiModelInstance->m_tmSkaTagManager
				, _pTimer->GetLerpedCurrentTick());
		}

		if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime))
		{  	
			CSkill &SkillData = _pNetwork->GetSkillData(m_nCurrentSkillNum);

			g_bPreSkill = FALSE;
			g_bDoSkill = TRUE;
			m_iSkillEffectStep = 0;
			
			bPlayPreSkill = TRUE;//0822 
			
			//NewClearState(CLEAR_STATE_LENGTH);    

			if (SkillData.GetReadyTime() > 0 && SkillData.idPlayer_Anim_Skill[_pNetwork->MyCharacterInfo.bExtension][1] > 0)
			{
				m_fSkillAnimTime = ((SkillData.GetReadyTime() / 10.f) - m_fSkillAnimTime) * skillSpeed;
			}
			else
			{
				m_fSkillAnimTime = 0.f;
			}

			m_tmSkillStartTime = _pTimer->CurrentTick();
			
			AddAnimation(SkillData.idPlayer_Anim_Skill[_pNetwork->MyCharacterInfo.bExtension][1], AN_NORESTART, 1.0f, PLAYER_ANIM_GROUP_WHOLEBODY,
				ESKA_MASTER_MODEL_INSTANCE, skillSpeed);

			LOG_DEBUG("SkillName : %s, StillTime : %f, Anim : %s", SkillData.GetName(), (float)m_fSkillAnimTime, SkillData.GetStillAnim(_pNetwork->MyCharacterInfo.bExtension));
		}
		else//0822
		{
			switch(m_nCurrentSkillNum) 
			{
			case HE_SELF_HEAL :
			case HE_PARTY_HEAL: //애니메이션 나눠져 있음.	
			case HE_HEAL:	//애니메이션 나눠져 있음.					
				_PreSkillTime = 0.0f;
				if(_pTimer->CurrentTick() - m_tmSkillStartTime > _PreSkillTime && bPlayPreSkill)		
				{								
					bPlayPreSkill = FALSE;								
					PlaySound(m_soMessage, GenderSound(SOUND_HEALER_HEAL_PRE), SOF_3D | SOF_VOLUMETRIC);									
					break;
				}					
				break;
				
			case HE_DEFENCE_MANA:
				_PreSkillTime = 0.0f;
				if(_pTimer->CurrentTick() - m_tmSkillStartTime > _PreSkillTime && bPlayPreSkill)		
				{								
					bPlayPreSkill = FALSE;								
					PlaySound(m_soMessage, GenderSound(SOUND_HEALER_DEFENCE_MANA_PRE), SOF_3D | SOF_VOLUMETRIC);									
					break;
				}
				break;
			}
		}
	}
	
	//---------------------------------------------------
	// 스킬 사용!!!
	if(g_bDoSkill)
	{
		m_iSkillEffectStep = 0;
		if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime))
		{
			g_bDoSkill = FALSE;
			g_bPostSkill = TRUE;
			
			CSkill &SkillData = _pNetwork->GetSkillData(m_nCurrentSkillNum);
			
			m_fSkillAnimTime = GetModelInstance()->GetAnimLength(SkillData.idPlayer_Anim_Skill[_pNetwork->MyCharacterInfo.bExtension][2]) * skillSpeed;
			m_tmSkillStartTime = _pTimer->CurrentTick();
			
			NewClearState(CLEAR_STATE_LENGTH);						
			
			AddAnimation(SkillData.idPlayer_Anim_Skill[_pNetwork->MyCharacterInfo.bExtension][2], AN_NORESTART, 1.0f, 
				PLAYER_ANIM_GROUP_WHOLEBODY, ESKA_MASTER_MODEL_INSTANCE, skillSpeed);
		}		
	}
	
	//---------------------------------------------------
	// 스킬 사용 후...
	if(g_bPostSkill)
	{
		CSkill &SkillData = _pNetwork->GetSkillData(m_nCurrentSkillNum);					
		
		if(m_bReadySendSkillMessage)
		{
			m_bStartPostSkill = FALSE;//1217
			
			CEntity *penTarget = GetPlayerWeapons()->m_penRayHitTmp;
			
			BOOL bFire = FALSE;

			if ( skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) != CSkill::MT_DASH )
			{
				if(	m_nCurrentSkillNum == HE_SHINING_ARROW || 
					m_nCurrentSkillNum == HE_STORM_ARROW || 
					m_nCurrentSkillNum == HE_DOUBLE_STING )
				{
					if(_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime-1.0f)
					{	
						bFire = TRUE;
					}
				}
				else
				{
					bFire = TRUE;
				}
			}
			
			//스킬 애니메이션 끝나기 0.8초전에 FIRE 메시지를 보낸다.
			//				if(_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime-1.0f)
			if(bFire)
			{
				// 타겟팅이 필요없는 스킬.
				//if(m_bRunningSelfSkill)				
				// 범위 스킬을 제외한 셀프 스킬일 경우.
				int lv = MY_INFO()->GetSkillLevel(m_nCurrentSkillNum);
				const char chTargetType = SkillData.GetTargetType();

				if (lv == 0)
				{
					if ( SkillData.GetJob() == PET_JOB )
					{ 
						// 펫 스킬의 경우
						lv = MY_INFO()->GetPetSkillLevel( pInfo->GetMyPetInfo()->lIndex, m_nCurrentSkillNum );
					}
				}

				// 자기 중심으로 멀티 공격
				if( SkillData.GetJob() != NIGHTSHADOW &&
					(chTargetType == CSkill::STT_SELF_RANGE || chTargetType == CSkill::STT_GUILD_SELF_RANGE) ||
					(SkillData.GetTargetNum(lv) > 1))
				{
					_pNetwork->SendSkillMessageInContainer(m_nCurrentSkillNum, m_dcEnemies, TRUE);
					m_bReadySendSkillMessage = FALSE;
				}

				if( !SkillData.IsNeedTarget() && SkillData.GetTargetNum(lv) <= 1)
				{
					CPrintF("------%f--SendSkillMessage\n", _pTimer->GetLerpedCurrentTick());
					_pNetwork->SendSkillMessage(m_nCurrentSkillNum, this, _pNetwork->MyCharacterInfo.index, TRUE);//0이 플레이어.
					m_bReadySendSkillMessage = FALSE;					
				}
				// 타겟이 에너미일때...
				else if( penTarget != NULL && SkillData.IsNeedTarget() )
				{
					m_penAttackingEnemy = penTarget;
					FLOAT3D vTargetPos(0, 0, 0);
					FLOAT3D vDirection(0, 0, 0);
					
					CEntity *pen = GetPlayerWeapons()->m_penRayHitTmp;
					FLOAT3D vCurrentCenter(((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[0],
						((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[1],
						((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[2] );
					FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vCurrentCenter;
					FLOAT3D vTargetCenter(0, 0, 0);
					FLOAT size = 0;
					if(pen != NULL && pen->GetFlags()&ENF_ALIVE)
					{
						if(pen->en_RenderType == RT_SKAMODEL)
						{
							// Bounding Box를 이용하여 Effect를 찍을 위치를 찾는다.
							FLOATaabbox3D bbox;
							pen->GetModelInstance()->GetCurrentColisionBox(bbox);
							FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*단순히 원으로 하면 너무 큼. 사이즈 조절 상수*/;
							size = vRadius.Length();
						}
						vTargetCenter = FLOAT3D(((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[0],
							((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[1],
							((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[2] );
						vTargetPos = pen->en_plPlacement.pl_PositionVector + vTargetCenter;
						vDirection = vTargetPos - vCurrentPos;
						vDirection.Normalize();
						vTargetPos -= vDirection * size;
					}
					vDirection.Normalize();	
					
					CSkill &SkillData = _pNetwork->GetSkillData(m_nCurrentSkillNum);
					if( SkillData.GetTargetType() == CSkill::STT_SELF_ONE || 
						SkillData.GetTargetType() == CSkill::STT_TARGET_ONE || 
						SkillData.GetTargetType() == CSkill::STT_PARTY_ONE ||
						SkillData.GetTargetType() == CSkill::STT_PARTY_ALL ||
						SkillData.GetTargetType() == CSkill::STT_GUILD_ONE ||
						SkillData.GetTargetType() == CSkill::STT_GUILD_ALL)
					{
						{	
							CPrintF("------%f--SendSkillMessage\n", _pTimer->GetLerpedCurrentTick());
							_pNetwork->SendSkillMessage(m_nCurrentSkillNum, penTarget, penTarget->GetNetworkID(), TRUE);//우선 타겟은 몹만...							
						}
					}					
					m_bReadySendSkillMessage = FALSE;	//1217
				}
				else if (penTarget == NULL && SkillData.IsNeedTarget())
				{
					// 타겟팅 스킬이지만 타겟이 사라짐.
					_pNetwork->SendCancelSkillMessage();
					m_bReadySendSkillMessage = FALSE;
				}
			}
		}
		
		//effect 처리 부분
		static std::vector<TagInfo> vecLastEffectInfo;
		if(m_nCurrentSkillNum == -1)
		{
			return;
		}
		
		CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);

		bool bAnimPlay = true;
		int nEvo = _pNetwork->MyCharacterInfo.nEvocationIndex;

		// 강신상태에서 강신해제시 스킬 애니메이션은 생략.
		if (nEvo > 0 && nEvo == m_nCurrentSkillNum)
		{
			bAnimPlay = false;
		}

		if( bAnimPlay == true && m_iSkillEffectStep == 0 && 
			(!skill.IsNeedTarget() || (penTarget != NULL && penTarget->en_pmiModelInstance != NULL && penTarget->en_RenderType == RT_SKAMODEL) ) )
		{
			++m_iSkillEffectStep;
			
			CEffectGroup *pFireEffect = StartEffectGroup(skill.GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension)
				, &en_pmiModelInstance->m_tmSkaTagManager,
				_pTimer->GetLerpedCurrentTick());

			if(m_pSkillReadyEffect != NULL && CEffectGroupManager::Instance().IsValidCreated(m_pSkillReadyEffect))
			{
				for(INDEX i=0; i<m_pSkillReadyEffect->GetEffectCount(); ++i)
				{
					CEffect *pEffect = m_pSkillReadyEffect->GetEffectKeyVector()[i].m_pCreatedEffect;
					if(pEffect != NULL && pEffect->GetType() == ET_SKA)
					{
						CSkaEffect *pSkaEffect = (CSkaEffect*)pEffect;
						TagInfo ti;
						ti.m_vPos = pSkaEffect->GetInstancePosition();
						ti.m_qRot = FLOATquat3D(1,0,0,0);
						vecLastEffectInfo.push_back(ti);
					}
				}
				m_pSkillReadyEffect->Stop(0.1f);
				m_pSkillReadyEffect = NULL;
			}
			
			if(pFireEffect != NULL && vecLastEffectInfo.empty())
			{
				pFireEffect->Process(_pTimer->GetLerpedCurrentTick());
				for(INDEX i=0; i<pFireEffect->GetEffectCount(); ++i)
				{
					CEffect *pEffect = pFireEffect->GetEffectKeyVector()[i].m_pCreatedEffect;
					if(pEffect != NULL && pEffect->GetType() == ET_SKA)
					{
						CSkaEffect *pSkaEffect = (CSkaEffect*)pEffect;
						TagInfo ti;
						ti.m_vPos = pSkaEffect->GetInstancePosition();
						ti.m_qRot = FLOATquat3D(1,0,0,0);
						vecLastEffectInfo.push_back(ti);
						pSkaEffect->SetLifeTime(0);
					}
				}
			}
		}

		FLOAT skillSpeed = (100 - _pNetwork->MyCharacterInfo.skillSpeed) / 100.0f;
		ASSERT(skill.GetDelay(m_iSkillEffectStep-1, _pNetwork->MyCharacterInfo.bExtension) * skillSpeed < m_fSkillAnimTime && "SkillAnimTime은 반드시 MissileSpeedFireTime보다 커야합니다.");
		
		// 이 부분에서 m_iSkillEffectStep의 값은 1~skill.GetDelayCount()이다.
		if( m_iSkillEffectStep > 0 && m_iSkillEffectStep <= skill.GetDelayCount(_pNetwork->MyCharacterInfo.bExtension)
			&& _pTimer->CurrentTick() - m_tmSkillStartTime > skill.GetDelay(m_iSkillEffectStep-1, _pNetwork->MyCharacterInfo.bExtension) * skillSpeed )
		{
			++m_iSkillEffectStep;
			
			if(skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_NONE)	// 근접 공격 - 마법 없음
			{
				FLOAT3D vHitPoint;
				FLOAT3D vHitDir;
				if( skill.IsNeedTarget() )
				{	GetTargetDirection(this, GetPlayerWeapons()->m_penRayHitTmp, vHitPoint, vHitDir); }
				else
				{
					GetTargetDirection(this, this, vHitPoint, vHitDir);
				}
				
				if(m_dcEnemies.Count() > 0 )
				{
					DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, vHitPoint, TRUE);
					m_dcEnemies.Clear();						
				}
				else
				{
					if( penTarget != NULL && !( penTarget->GetFirstExFlags() & ENF_EX1_CLICK_OBJECT ) )
					{
						//damage effect 처리
						this->InflictDirectDamage(penTarget, this, DMT_NONE, 0, vHitPoint, vHitDir);
					}
				}
				FLOAT3D vAxisY(0,1,0);
				FLOAT angle = acos(vHitDir % vAxisY);
				FLOAT3D axis = vAxisY * vHitDir;
				axis.Normalize();
				FLOATquat3D quat;
				quat.FromAxisAngle(axis, angle);
				StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
					, _pTimer->GetLerpedCurrentTick()
					, vHitPoint, quat);
			}
			else if(skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_ARROW )	// 화살
			{
				ASSERT(en_pmiModelInstance != NULL);
				
				if( m_dcEnemies.Count() > 0 )
				{
					for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
					it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
					{						
						CEntity &en = *(*it);
						if(en.IsFlagOn(ENF_ALIVE))
						{
							FLOAT fHoriOffset = 0.0f;
							FLOAT fVertiOffset = 0.0f;
							if(skill.GetDelayCount(_pNetwork->MyCharacterInfo.bExtension) > 1)
							{
								FLOAT fDist = (penTarget->en_plPlacement.pl_PositionVector - en_plPlacement.pl_PositionVector).Length();
								FLOAT factor = 1.3f * fDist / skill.GetFireRange();
								factor = Clamp(factor, 0.0f, 1.0f);
								if(m_iSkillEffectStep == 2) {fHoriOffset = +1.0f * factor;}
								if(m_iSkillEffectStep == 3) {fHoriOffset = -1.0f * factor;}
								//if(m_iSkillEffectStep == 4) {fVertiOffset = +0.0f * factor;}
								if(m_iSkillEffectStep == 5) {fVertiOffset = -1.0f * factor;}
							}
							if( _pNetwork->MyCharacterInfo.bExtension && ( _pNetwork->MyCharacterInfo.job != ROGUE && (!IsEXRogue(_pNetwork->MyCharacterInfo.job)) ) )	// [2012/08/27 : Sora] EX로그 추가
							{
								ShotMissile(this, "STAFF", &en, skill.GetMissileSpeed(_pNetwork->MyCharacterInfo.bExtension)
									, skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
									, skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension)
									, false, fHoriOffset, fVertiOffset, 1);
							}
							else
							{
								ShotMissile(this, "RHAND", &en, skill.GetMissileSpeed(_pNetwork->MyCharacterInfo.bExtension)
									, skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
									, skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension)
									, false, fHoriOffset, fVertiOffset);
							}
						}
					}
					m_dcEnemies.Clear();
				}
				else
				{
					if( penTarget != NULL && penTarget->IsFlagOn(ENF_ALIVE))
					{
						FLOAT fHoriOffset = 0.0f;
						FLOAT fVertiOffset = 0.0f;
						if(skill.GetDelayCount(_pNetwork->MyCharacterInfo.bExtension) > 1)
						{
							FLOAT fDist = (penTarget->en_plPlacement.pl_PositionVector - en_plPlacement.pl_PositionVector).Length();
							FLOAT factor = 1.3f * fDist / skill.GetFireRange();
							factor = Clamp(factor, 0.0f, 1.0f);
							if(m_iSkillEffectStep == 2) {fHoriOffset = +1.0f * factor;}
							if(m_iSkillEffectStep == 3) {fHoriOffset = -1.0f * factor;}
							//if(m_iSkillEffectStep == 4) {fVertiOffset = +0.0f * factor;}
							if(m_iSkillEffectStep == 5) {fVertiOffset = -1.0f * factor;}
						}
						if( _pNetwork->MyCharacterInfo.bExtension && ( _pNetwork->MyCharacterInfo.job != ROGUE && (!IsEXRogue(_pNetwork->MyCharacterInfo.job))  ) ) // 로그는 두번째 타잎이 활이다.	// [2012/08/27 : Sora] EX로그 추가
						{
							ShotMissile(this, "STAFF", penTarget, skill.GetMissileSpeed(_pNetwork->MyCharacterInfo.bExtension)
								, skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
								, skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension)
								, false, fHoriOffset, fVertiOffset, 1);
						}
						else
						{
							ShotMissile(this, "RHAND", penTarget, skill.GetMissileSpeed(_pNetwork->MyCharacterInfo.bExtension)
								, skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
								, skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension)
								, false, fHoriOffset, fVertiOffset);
						}
					}
				}					
			}
			else if(skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_DIRECT)	// 발사체 없음, 데미지 없음, 버프 효과 등
			{
				// 타겟팅이 필요없는 스킬.
				if( !skill.IsNeedTarget() )					
				{
					StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
						, &this->en_pmiModelInstance->m_tmSkaTagManager
						, _pTimer->GetLerpedCurrentTick());	
				}
				// 타겟팅이 필요한 스킬.
				else
				{
					if(penTarget != NULL && penTarget->GetFlags()&ENF_ALIVE && penTarget->en_pmiModelInstance != NULL)
					{
						StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
							, &penTarget->en_pmiModelInstance->m_tmSkaTagManager
							, _pTimer->GetLerpedCurrentTick());				
					}
				}
			}
			else if( skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_DIRECTDAMAGE )	// 발사체 없음, 데미지 있음
			{
				if( m_dcEnemies.Count() > 0 )
				{						
					for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
					it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
					{									
						CEntity &en = *(*it);
						if(en.IsFlagOn(ENF_ALIVE) && en.en_pmiModelInstance != NULL)
						{
							StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
								, &(en.en_pmiModelInstance->m_tmSkaTagManager)
								, _pTimer->GetLerpedCurrentTick());								
						}
					}
					
					// 메이지의 싱글던젼용 스킬을 쓰면 몹이 안죽길래...
					if(m_dcEnemies.Count() > 0)
					{
						FLOAT3D vHitPoint;
						FLOAT3D vHitDir;
						GetTargetDirection(this, GetPlayerWeapons()->m_penRayHitTmp, vHitPoint, vHitDir);
						DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, vHitPoint, TRUE);
						m_dcEnemies.Clear();
					}						
				}
				else
				{
					if( penTarget != NULL )
					{
						this->InflictDirectDamage( penTarget, this, DMT_NONE, 0, FLOAT3D(0,0,0), FLOAT3D(0,0,0) );
						
						if(penTarget->en_pmiModelInstance != NULL)
						{
							// Terrain은 제외하자
							if (penTarget->GetRenderType() != RT_TERRAIN)
							{
								StartEffectGroup(skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
									, &penTarget->en_pmiModelInstance->m_tmSkaTagManager
									, _pTimer->GetLerpedCurrentTick());
							}
						}
					}
				}					
			}
			else if((skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_CONTINUE ||
				skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_MAGECUTTER ||
				skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_DASH )
				&& !vecLastEffectInfo.empty())		// 시전중인 이펙트가 발사체가 되어 날아감
			{
				if( m_dcEnemies.Count() > 0 )
				{
					for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
					it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
					{															
						CEntity &en = *(*it);
						if(en.IsFlagOn(ENF_ALIVE))
						{
							ShotMagicContinued( this, vecLastEffectInfo[0].m_vPos, vecLastEffectInfo[0].m_qRot
								, &en, skill.GetMissileSpeed(_pNetwork->MyCharacterInfo.bExtension)
								, skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
								, skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension)
								, false, skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) );
						}
					}
					m_dcEnemies.Clear();
				}
				else
				{
					if( penTarget != NULL && penTarget->IsFlagOn(ENF_ALIVE))
					{
						ShotMagicContinued(this, vecLastEffectInfo[0].m_vPos, vecLastEffectInfo[0].m_qRot
							, penTarget, skill.GetMissileSpeed(_pNetwork->MyCharacterInfo.bExtension)
							, skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
							, skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension)
							, false, skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension));
					}
				}					
				vecLastEffectInfo.erase(vecLastEffectInfo.begin());
			}
			else if(skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_GOWAITBACK)
			{
				if( penTarget != NULL )
				{
					ShotGoWaitBack(this, vecLastEffectInfo[0].m_vPos, vecLastEffectInfo[0].m_qRot
						, penTarget, skill.GetMissileSpeed(_pNetwork->MyCharacterInfo.bExtension)
						, skill.GetDestDelay(_pNetwork->MyCharacterInfo.bExtension)
						, skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
						, skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension)
						, false);
				}
				vecLastEffectInfo.erase(vecLastEffectInfo.begin());
			}
			else if(skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_CONNECT)
			{
				if( penTarget != NULL )
				{
					ShotConnect(this, penTarget, skill.GetDestDelay(_pNetwork->MyCharacterInfo.bExtension)
						, skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
						, skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension));
				}
			}
		}
		
		if((_pTimer->CurrentTick() - m_tmSkillStartTime > m_fSkillAnimTime-0.1f))//1006
		{
			//m_bRunningSelfSkill = FALSE;
			if(m_nCurrentSkillNum != -1)
			{
				CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
				
				// 
				if( SkillData.GetTargetType() == CSkill::STT_SELF_ONE || 
					SkillData.GetTargetType() == CSkill::STT_TARGET_ONE ||
					SkillData.GetTargetType() == CSkill::STT_PARTY_ALL ||
					SkillData.GetTargetType() == CSkill::STT_GUILD_ONE ||
					SkillData.GetTargetType() == CSkill::STT_GUILD_ALL )
				{
					m_vDesiredPosition = GetPlacement().pl_PositionVector;
				}
			}
			
			m_bReadySendSkillMessage = TRUE;			
			
			g_bPostSkill = FALSE;	
			
			m_iSkillEffectStep = 0;
			vecLastEffectInfo.clear();
			
			m_idCurrentSkillAnim = -1;//스킬 종료.
			m_tmSkillStartTime = 0.0f;
			m_bSkilling = FALSE;
			m_bLockMove = FALSE;
			
			m_bStartPostSkill = TRUE;
			
			bPlayPreSkill = TRUE;//0731 촬영용 임시코드
			step = 0;
			m_bWaitForSkillTarget = FALSE;

			m_bCanSkillCancel = TRUE;
			//CPrintF("SkillAnimation() End!!! \n");
			
			NewClearState(CLEAR_STATE_LENGTH); 
			IdleAnim();
			m_bNoAniGuildSkill = FALSE;
			if(m_nReservedSkillNum != -1)//예약된 스킬이 있다면,
			{
				m_nDesiredSkillNum		= m_nReservedSkillNum;
				m_nReservedSkillNum		= -1;				
			}
			else
			{
				if( m_nCurrentSkillNum != -1 )
				{
					CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
					if( skill.GetFlag() & SF_FORHELP
					&& (skill.GetTargetType() != CSkill::STT_SELF_ONE
						|| skill.GetTargetType() != CSkill::STT_SELF_RANGE
						|| skill.GetTargetType() != CSkill::STT_PARTY_ALL 
						|| skill.GetTargetType() != CSkill::STT_GUILD_ONE
						|| skill.GetTargetType() != CSkill::STT_GUILD_ALL))
					{
						GetPlayerWeapons()->m_penRayHitTmp = NULL;
					}
				}
				
				//GetPlayerWeapons()->m_penRayHitTmp = NULL;//0709 이걸지우면 이전에 공격중이었으면 계속 공격. 몹쪽으로 이동중이었으면 계속 이동.
				if(!g_iAutoAttack)//자동공격이 아니라면 스킬후에 공격하지 않는다.
				{
					GetPlayerWeapons()->m_penRayHitTmp = NULL;
				}					
			}
			
			m_nCurrentSkillNum = -1;
			
			pUIManager->SetCSFlagOff( CSF_SKILL );
		}
		else
		{
			
			FLOAT3D vTargetPos(0, 0, 0);
			FLOAT3D vDirection(0, 0, 0);
			// TO-KR-T20090908-006 : Bug Fix. [9/8/2009 rumist]
			// 트리거 오브젝트 클릭후 확인창 상태에서 셀프 스킬을 사용할 경우 버그 수정.
			if( skill.IsNeedTarget() )
			{
				GetTargetDirection(this, GetPlayerWeapons()->m_penRayHitTmp, vTargetPos, vDirection);//0801 타겟 위치와 방향.
			}
			else
			{
				GetTargetDirection(this, this, vTargetPos, vDirection);//0801 타겟 위치와 방향.
			}

			
			switch(m_nCurrentSkillNum)
			{
			case HE_SELF_HEAL:
			case HE_PARTY_HEAL: //애니메이션 나눠져 있음.	
			case HE_HEAL:	//애니메이션 나눠져 있음.
				_PreSkillTime = 0.0f;
				if(_pTimer->CurrentTick() - m_tmSkillStartTime > _PreSkillTime && bPlayPreSkill)		
				{								
					bPlayPreSkill = FALSE;								
					PlaySound(m_soMessage, GenderSound(SOUND_HEALER_HEAL_DO), SOF_3D | SOF_VOLUMETRIC);									
					break;
				}	
				break;
				
			case HE_DEFENCE_MANA:
				_PreSkillTime = 0.0f;
				if(_pTimer->CurrentTick() - m_tmSkillStartTime > _PreSkillTime && bPlayPreSkill)		
				{								
					bPlayPreSkill = FALSE;								
					PlaySound(m_soMessage, GenderSound(SOUND_HEALER_DEFENCE_MANA_DO), SOF_3D | SOF_VOLUMETRIC);									
					break;
				}	
				break;					
			}
		}
	}
}
	
virtual void UseSkill(int skillIndex)
{	
	if (_pGameState->IsRestartGame() == TRUE)
	{
		UIMGR()->GetSystemMenu()->CancelRestart();
	}

	// 애완동물을 타고 있는 상태에서는 액티브 스킬을 사용할 수 없습니다.
	if( m_bRide )
	{
		return;
	}
	if(m_bPlayAction || IsTransform() || IsPolymophing())//0824 액션중일때는 스킬 사용되면 안된다.
	{
		return;
	}

	if (skillIndex == -1)
	{
		return;
	}

	CUIManager* pUIManager = SE_Get_UIManagerPtr();

	if (pUIManager->IsCSFlagOn(CSF_PETRIDING) || pUIManager->IsCSFlagOn(CSF_TELEPORT))
	{
		return;
	}

	// EDIT : bs : 060322
	CSkill &SkillData = _pNetwork->GetSkillData(skillIndex);

	if (m_nCurrentSkillNum != -1 && m_bSkilling)
	{
		if (m_nCurrentSkillNum == skillIndex || m_nReservedSkillNum == skillIndex)
		{
			return;
		}
	}
	else
	{
		// [100107: selo] MyCharacterInfo.magicspeed 대치
		DOUBLE	dDelayTime = DOUBLE( SkillData.GetReUseTime() + GetMagicspeed() ) / 10.0;
		DOUBLE	dElapsedTime = _pTimer->GetHighPrecisionTimer().GetSeconds() - SkillData.Skill_Data.Skill_StartTime;

		int nCoolTimeReductionRate = 0;

		if (SkillData.GetJob() != PET_JOB && SkillData.GetJob() != WILDPET_JOB && !(SkillData.GetFlag() & SF_GUILD) )
		{ // 스킬 쿨타임 감소는 캐릭터만 옵션이 붙는다.
			if( !( SkillData.Skill_Data.appState & SCT_NOCOOLTIME ) )
			{
				nCoolTimeReductionRate = SE_Get_UIManagerPtr()->GetCoolTimeReductionRate();
			}
			dDelayTime *= DOUBLE(100-nCoolTimeReductionRate)/100.0f;
		}
		
		if (dElapsedTime <= dDelayTime)
		{
			return;
		}
	}
	// EDIT : bs : 060322
	m_nDesiredSkillNum = skillIndex; 

	if(m_bSkilling || m_bStartAttack)//스킬 사용중 다른 스킬 사용하려 할때.
	{
		return;
	}

	// EDIT : bs : 060322
	// CSkill &SkillData = _pNetwork->GetSkillData(skillIndex);

	// NOTE : 스킬 애니메이션이 없으면 스킬을 사용할수 없음.
	/*
	INDEX iSkillAnim = SkillData.idPlayer_Anim_Skill[_pNetwork->MyCharacterInfo.bExtension][2];
	if(	iSkillAnim == -1 )
	{
		m_nDesiredSkillNum = -1;		
		CPrintF( "Warning : Cannot find Skill Animation Index!!!\n" );	
		return;
	}
	*/

	// 타겟팅이 필요없는 스킬.
	if( !SkillData.IsNeedTarget() )	
	{	
	}
	// 타겟팅이 필요한 스킬.
	else
	{
		CEntity *penTarget = GetPlayerWeapons()->m_penReservedTarget;
		if(penTarget)
		{			
			GetPlayerWeapons()->m_penRayHitTmp = penTarget;		
		}
	}
}

virtual void UsePetPickItem()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if(pInfo->GetMyPetInfo()->pen_pEntity)
	{
		((CPetBase*)pInfo->GetMyPetInfo()->pen_pEntity)->SearchNearItem();
		((CPetBase*)pInfo->GetMyPetInfo()->pen_pEntity)->FindNewItemTarget();
		return;
	}	
};

virtual void UsePetSkill(int skillIndex)
{	
	if(m_bPlayAction || m_bMobChange)//0824 액션중일때는 스킬 사용되면 안된다.
	{
		return;
	}

	CSkill &SkillData = _pNetwork->GetSkillData(skillIndex);

	const int iSkillType = SkillData.GetType();

	// 펫 액티브 스킬만!!!
	if( iSkillType != CSkill::ST_PET_SKILL_ACTIVE )
	{
		return;
	}

	m_nDesiredSkillNum = skillIndex; 

	if(m_bSkilling || m_bStartAttack)//스킬 사용중 다른 스킬 사용하려 할때.
	{
		return;
	}

	// 타겟팅이 필요없는 스킬.
	if( !SkillData.IsNeedTarget() )	
	{	
	}
	// 타겟팅이 필요한 스킬.
	else
	{
		CEntity *penTarget = GetPlayerWeapons()->m_penReservedTarget;
		if(penTarget)
		{			
			GetPlayerWeapons()->m_penRayHitTmp = penTarget;		
		}
	}
}

virtual void UseSlaveSkill(CEntity *pEntity, int skillIndex)
{	
	((CSlave*)pEntity)->UseSkill(skillIndex);
}

virtual void UseWildSkill(CEntity *pEntity, int skillIndex)
{	
 	if( m_bWildRide )
 	{
 		UseSkill(skillIndex);
 	}
 	else
 	{
		((CWildPet*)pEntity)->UseSkill(skillIndex);
 	}
}
//0807
virtual void UseAction(int ActionIndex)
{
	if (IsFlagOff(ENF_ALIVE))
	{
		return;
	}

	/* [2011/12/29 : Sora] 데스모션 중에 액션을 사용 하면 캐릭터가 움직이지 않는 버그 확인
		기획측과 협의하여 데스모션 중에는 액션을 사용할 수 없도록 수정 */
	if( _pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_FAKEDEATH) )
	{
		return;
	}
	
	CUIManager* pUIManager = SE_Get_UIManagerPtr();

	switch(ActionIndex)
	{
	case ACTION_NUM_SITDOWN: //앉기.서기.
	/*	if(m_bMobChange || (m_bRide && (g_iCountry == USA || g_iCountry == RUSSIA)))
		{
			m_bPlayAction = FALSE;
			m_nPlayActionNum = -1;
			m_nActionSit = 0;
			return;
		}*/

		if (m_bRide && m_bWildRide )
		{
			m_bPlayAction = FALSE;
			m_nPlayActionNum = -1;
			m_nActionSit = 0;
			return;
		}
		if( m_bPlayAction && 
			m_nPlayActionNum != -1 && 
			m_nPlayActionNum != ACTION_NUM_SITDOWN )
		{
			return;
		}

		if(m_nActionSit == 0)//서있는 상태.
		{			
			m_nActionSit = 1;//앉는 모션 시작.
			if(_pNetwork->MyCharacterInfo.job == NIGHTSHADOW )
			{
				DeleteWearingWeapon(FALSE,FALSE);
			}
		}
		else if(m_nActionSit == 2)//앉아있는 상태.
		{
			m_nActionSit = 3;//서는 모션 시작.
		}
		else 
		{
			break;
		}
		m_bPlayAction = TRUE;
		m_nPlayActionNum = ActionIndex;
		_bPersonalShop = FALSE;

		break;

	case ACTION_NUM_WALK_RUN:	
		m_bRunningMode = !m_bRunningMode;
		{			
		}
		if(m_bRunningMode)
		{
			// [100107: selo] MyCharacterInfo.runspeed 대치
			plr_fSpeed = GetRunspeed();
		}		
		else
		{
			// [100107: selo] MyCharacterInfo.walkspeed 대치
			plr_fSpeed = GetWalkspeed();
		}

		_pNetwork->SendActionMessage(0, ACTION_NUM_WALK_RUN, 0);
		break;
	case ACTION_NUM_PK:		
		// if you riding wild pet, can't pk. [12/12/2010 rumist]
		if(m_bRide||m_bWildRide)
		{
			return;
		}
// 		if (_pNetwork->IsRvrZone())
// 		{
// 			pUIManager->GetChattingUI()->AddSysMessage(_S( 5596, "분쟁 지역에서는 PVP모드로 전환할 수 없습니다." ), SYSMSG_ERROR);
// 			return;
// 		}
// KDM : BEGIN
		if( _pNetwork->MyCharacterInfo.sbAttributePos & MATT_WAR ||	// 공성 지역에 있다면?
				_pNetwork->MyCharacterInfo.level > 15 )
// KDM : END
		{
			if(IsLegit() == FALSE) // PK 해제시에만 풀수 없도록 수정
			{
				_pNetwork->SendActionMessage(0, ACTION_NUM_PK, 0);
			}
		}
		else 
		{
			_pNetwork->ClientSystemMessage( _S( 806, "15레벨 이하는 PVP를 할 수가 없습니다." ), SYSMSG_ERROR );	
		}
		break;
	case ACTION_NUM_WILDPET_RIDING:
		{
			if (_pNetwork->MyWearItem[WEAR_PET].IsEmptyItem() == FALSE && !pUIManager->IsCSFlagOn(CSF_SKILLREADY) && !m_bSkilling)
			{
				CItemData* pItemData = _pNetwork->GetItemData( _pNetwork->MyWearItem[WEAR_PET].Item_Index );
				ObjInfo* pInfo = ObjInfo::getSingleton();
				// BUG FIX : ITS#3544 [Pet] NPC변신 주문 서 사용 후 (P2)펫 마운트 불가 [8/4/2011 rumist]
				if( IsTransform() )
				{
					pUIManager->GetChattingUI()->AddSysMessage( _S( 2574, "변신중일때는 애완동물을 탈 수 없습니다." ), SYSMSG_ERROR );
					return;
				}
				// BUG FIX : LC-MX-20110506-001 일반 펫은 단축키로 탑승 금지. 변신상태일 경우에도 탑승금지. [5/18/2011 rumist]
				if( (pItemData->GetSubType() != CItemData::ACCESSORY_WILDPET) )
				{
					pUIManager->GetChattingUI()->AddSysMessage( _S( 5489, "펫을 탑승할 수 없습니다." ), SYSMSG_ERROR );
					return;
				}
				if (pInfo->GetMyApetInfo() == NULL)
				{
					pUIManager->GetChattingUI()->AddSysMessage( _S( 5489, "펫을 탑승할 수 없습니다." ), SYSMSG_ERROR );
					return;
				}
				// assertion error check. [7/8/2011 rumist]
				CWildPetData* rWildPet = CWildPetData::getData(pInfo->GetMyApetInfo()->m_nIndex);

				if( rWildPet != NULL && ( rWildPet->nMount[pInfo->GetMyApetInfo()->m_sbTransStat] < 1 ) )
				{
					pUIManager->GetChattingUI()->AddSysMessage( _S( 5489, "펫을 탑승할 수 없습니다." ), SYSMSG_ERROR );
					return;
				}
				// BUGFIX : pet 탑승 시도시 pet이 스킬을 사용중이라면 탑승할 수 없다. [6/30/2011 rumist]
				if ((pInfo->GetMyApetInfo()->m_pEntity == NULL) || pInfo->GetMyApetInfo()->m_pEntity->IsWildPet() && !pInfo->GetMyApetInfo()->m_pEntity->IsAvailableRide())
				{
					pUIManager->GetChattingUI()->AddSysMessage(_S(5489, "펫을 탑승할 수 없습니다."), SYSMSG_ERROR);
					return;
				}				
				if( _pNetwork->MyCharacterInfo.job == NIGHTSHADOW )
				{
					pUIManager->GetChattingUI()->AddSysMessage( _S( 5308, "나이트 셰도우는 탑승할 수 없습니다." ), SYSMSG_ERROR );
					return;
				}

				// 앉아있으면 탈 수 없다. [12/8/2010 rumist]
				if( ( (static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->IsSitting() ) )
				{
					pUIManager->GetChattingUI()->AddSysMessage( _S( 5310, "앉은 상태에서는 탑승할 수 없습니다." ), SYSMSG_ERROR );
					return;
				}

				// 상점 개설 준비중인지 네트워크 응답 체크 - 응답 대기중에는 탑승하면 안된다.
				if( _pNetwork->m_iNetworkResponse[MSG_PERSONALSHOP] != 0 )
				{
					return;
				}

				// 변신주문서 사용시 변신중일 때 탑승하지 않도록. [trylord ; 15/09/11]
				//	김영환
				if(!_pNetwork->Is_MyChar_MorphStatus_MORPH_END())
				//if(_pNetwork->MyCharacterInfo.eMorphStatus != CNetworkLibrary::MyChaInfo::eMORPH_END)
				{
					return;
				}
									
				_pNetwork->SendWildPetMountReq( _pNetwork->MyCharacterInfo.bWildPetRide );
			}
		}
		break;
	case ACTION_NUM_GREET:
	case ACTION_NUM_SMILE:
	case ACTION_NUM_CRY:	
	case ACTION_NUM_NUMBER_ONE :
	case ACTION_NUM_HANDCLAP :
	case ACTION_NUM_REFUSE :
	case ACTION_NUM_GOOD_LOOKS :
	case ACTION_NUM_GOOD_LOOKS2 :
	case ACTION_NUM_BANTER :
	case ACTION_NUM_CHEER :
	case ACTION_NUM_COMBAT :
	case ACTION_NUM_SUBMISSION :
	case ACTION_NUM_WATER_SPREAD:
		if(m_bRide||m_bWildRide)
		{
			return;
		}		
		
		if( m_bPlayAction && (m_nPlayActionNum == ActionIndex 
								|| m_nPlayActionNum == ACTION_NUM_WATER_SPREAD)		//물뿌리기 액션은 캔슬 안되게...
		)
		{	// 액션중 같은 액션을 누르면 
			return;
		}

		if(m_bMobChange || m_bIsTransform)
		{
			m_bPlayAction = FALSE;
			m_nPlayActionNum = -1;
			m_nActionSit = 0;
			return;
		}
		if(m_bSkilling || m_bStartAttack || (m_bForward || m_bKeyMove))
		{
			m_bPlayAction = FALSE;
			m_nPlayActionNum = -1;
			m_nActionSit = 0;
		
			//_pNetwork->ClientSystemMessage( _S( 405, "공격중이거나 스킬 사용중, 이동중에는 앉을수 없습니다." ), SYSMSG_ERROR );

			break;
		}
		else if(m_nActionSit != 2 && m_nActionSit != 1 && m_nActionSit != 3)		
		{
			m_nPlayActionNum = ActionIndex;
			m_bPlayAction = TRUE;
			DeleteWearingWeapon(FALSE, FALSE);
			_pNetwork->SendActionMessage(0, ActionIndex, 0, GetPlayerWeapons()->m_penRayHitTmp);
		}
		break;
	default:
		break;
	}
};

virtual void ClearMultiTargets()
{
	m_dcEnemies.Clear();
};

virtual BOOL IsLegit()
{
	if(m_nLegit !=0 && _pNetwork->MyCharacterInfo.pk_mode > 0) // PK 해제시에만 풀수 없도록 수정
	{
		CTString strSysMessage;
		strSysMessage.PrintF(_S( 858, "현재 당신에게 정당방위가 발동되어 있어서 PVP 해제가 불가능합니다." ));
		_pNetwork->ClientSystemMessage( strSysMessage);
		return TRUE;
	}
	return FALSE;
}

void SetTargetNull()
{
	GetPlayerWeapons()->m_penRayHitTmp = NULL;
	GetPlayerWeapons()->m_penReservedTarget = NULL;

	INFO()->TargetClear(eTARGET);
}

virtual void SetTargetMe()
{
	//GetPlayerWeapons()->m_penRayHitTmp = this;

	CEntity *penTarget = GetPlayerWeapons()->m_penRayHitTmp;
	CEntity *penTargetReserve = GetPlayerWeapons()->m_penReservedTarget;

	if(m_bWaitForSkillTarget)
	{
		GetPlayerWeapons()->m_penRayHitTmp = this;
		GetPlayerWeapons()->m_penReservedTarget = this;
	}
	else
	{
		GetPlayerWeapons()->m_penReservedTarget = this;
	}
	
	bool bRet = this->SetTargetInfo(_pNetwork->MyCharacterInfo.maxHP,_pNetwork->MyCharacterInfo.hp,TRUE,0,_pNetwork->MyCharacterInfo.pk_mode,_pNetwork->MyCharacterInfo.pkpenalty);
	
	if (bRet == true)
	{
		this->SetTargetSyndicateInfo(_pNetwork->MyCharacterInfo.iSyndicateType, _pNetwork->MyCharacterInfo.iSyndicateGrade);
	}
	//CPrintF("Target Me...\n");
};
virtual	BOOL IsSameTarget(CEntity* penEntity)
{
	return INFO()->GetTargetEntity(eTARGET) == penEntity;
}

virtual void SetTarget(CEntity* penEntity)
{
	//GetPlayerWeapons()->m_penRayHitTmp = penEntity;
	if(m_bWaitForSkillTarget)
	{
		GetPlayerWeapons()->m_penRayHitTmp = penEntity;
	}
	else
	{
		GetPlayerWeapons()->m_penReservedTarget = penEntity;
	}
	
	penEntity->SetTargetInfo(0, 0, FALSE, 0);
};

virtual void SetMobData(CEntity* penEntity, SLONG hp, SLONG maxHp, int level, BOOL bNpc, int mobIdx)
{
	((CUnit*)penEntity)->m_nMaxiHealth = maxHp;
	((CUnit*)penEntity)->m_nCurrentHealth = hp;
	((CUnit*)penEntity)->m_nMobLevel = level;
	((CEnemyBase*)penEntity)->m_bNpc = bNpc;
	((CUnit*)penEntity)->m_nMobDBIndex = mobIdx;
};

virtual void SetPetData(CEntity* penEntity, SLONG hp, SLONG maxHP)
{
	((CUnit*)penEntity)->m_nMaxiHealth = maxHP;
	((CUnit*)penEntity)->m_nCurrentHealth = hp;	
};

virtual void SetWildPetData(CEntity* penEntity, SLONG hp, SLONG maxHP)
{
	((CUnit*)penEntity)->m_nMaxiHealth = maxHP;
	((CUnit*)penEntity)->m_nCurrentHealth = hp;
	if( (CUnit*)penEntity )
	{
		((CUnit*)penEntity)->SendActEvent();
	}
};

virtual void SetElementalData(CEntity* penEntity, SLONG hp, SLONG maxHP )
{
	((CUnit*)penEntity)->m_nMaxiHealth = maxHP;
	((CUnit*)penEntity)->m_nCurrentHealth = hp;

	// 내 소환수의 경우 인터페이스의 HP를 갱신해줌.
	if (penEntity->IsFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE))
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();
		ObjInfo* pInfo = ObjInfo::getSingleton();
		CSlaveTargetInfom* pSlaveInfo = NULL;

		for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
		{
			CUISummon* pUISummon = (CUISummon*)pUIManager->GetUI(i);
			pSlaveInfo = pInfo->GetMySlaveInfo(i - UI_SUMMON_START);

			if (pUISummon->GetSummonEntity() && pUISummon->GetSummonIndex() == penEntity->GetNetworkID() && pSlaveInfo != NULL)
			{
				pSlaveInfo->fHealth	= hp;
				pSlaveInfo->fMaxHealth	= maxHP;
			}
		}
	}
};

virtual void SetElementalStatus(CEntity* penEntity, SBYTE sbAttackSpeed, SBYTE sbMagicSpeed, LONG lSkillSpeed, FLOAT fWalkSpeed, FLOAT fRunSpeed, FLOAT fAttackRange )
{
	((CUnit*)penEntity)->m_fWalkSpeed		= fWalkSpeed;
	((CUnit*)penEntity)->m_fAttackRunSpeed	= fRunSpeed;
	((CUnit*)penEntity)->m_fCloseRunSpeed	= fRunSpeed;
	((CUnit*)penEntity)->m_fAttackDistance	= fAttackRange;
	((CUnit*)penEntity)->m_fCloseDistance	= fAttackRange;
	((CUnit*)penEntity)->m_fStopDistance	= fAttackRange;
	((CUnit*)penEntity)->m_fSkillSpeed		= (100 - lSkillSpeed) / 100.0f;	
	static BOOL bHardCode = FALSE;
	static FLOAT AttackSpeed = 10.0f;
	if( bHardCode )
	{
		sbAttackSpeed = AttackSpeed;	
	}
	((CSlave*)penEntity)->SetAttackSpeed(sbAttackSpeed);

}

virtual void CommandAttack()//0910
{
	CEntity *penTarget = GetPlayerWeapons()->m_penRayHitTmp;
	CEntity *penTargetReserve = GetPlayerWeapons()->m_penReservedTarget;
	if(GetPlayerWeapons()->m_penReservedTarget && GetPlayerWeapons()->m_penReservedTarget->GetFlags()&ENF_ALIVE)
	{			
		GetPlayerWeapons()->m_penRayHitTmp = GetPlayerWeapons()->m_penReservedTarget;		
	}	
};

virtual BOOL CommandAction()
{
	CEntity *penTarget = GetPlayerWeapons()->m_penReservedTarget;

	if( penTarget==this )
	{
		return TRUE;
	}

	if( penTarget && (penTarget->GetFlags()&ENF_ALIVE && penTarget->IsCharacter()) )
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();

		if( (!IsPvp() || (IsPvp() && GAMEDATAMGR()->GetPartyInfo()->IsPartyMember(penTarget->GetNetworkID()))) &&
			!pUIManager->GetGuildBattle()->IsEnemy( penTarget->en_ulID )
		)
		{		
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
};

void InflictDirectDamage(CEntity *penToDamage, CEntity *penInflictor, enum DamageType dmtType,
	FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
{	 

	if(penToDamage == NULL) {return;}
	if( !(penToDamage->GetFlags()&ENF_ALIVE ) )	{ return; }

	CEntity *pen = penToDamage;	
	FLOAT3D vCurrentCenter(((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[0],
		((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[1],
		((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[2] );
	FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vCurrentCenter;
	FLOAT3D vTargetCenter(0, 0, 0);
	FLOAT3D vTargetPos(0, 0, 0);
	FLOAT3D vDirection2(0, 0, 0);
	FLOAT size = 0;
	if(pen != NULL && pen->GetFlags()&ENF_ALIVE)
	{
		if(pen->en_RenderType == RT_SKAMODEL)
		{
			// Bounding Box를 이용하여 Effect를 찍을 위치를 찾는다.
			FLOATaabbox3D bbox;
			pen->GetModelInstance()->GetCurrentColisionBox(bbox);
			FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*단순히 원으로 하면 너무 큼. 사이즈 조절 상수*/;

			// pen 모델의 사이즈 비율을 반영하여 처리
			/*FLOAT3D vbboxMax_stretch = bbox.maxvect * pen->GetModelInstance()->mi_vStretch;
			FLOAT3D vbboxMin_stretch = bbox.minvect * pen->GetModelInstance()->mi_vStretch;
			FLOAT3D vRadius = (vbboxMax_stretch - vbboxMin_stretch) * 0.5f;
			//vRadius *= pen->GetModelInstance()->mi_vStretch;*/
			size = vRadius.Length();
		}
		vTargetCenter = FLOAT3D(((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[0],
			((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[1],
			((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[2] );
		vTargetPos = pen->en_plPlacement.pl_PositionVector + vTargetCenter;
		vDirection2 = vTargetPos - vCurrentPos;
		vDirection2.Normalize();
		vTargetPos -= vDirection2 * size;
	}
	vDirection2.Normalize();

	ObjInfo* pInfo = ObjInfo::getSingleton();

	// FIXME : 데미지 처리하는 부분이 왜 이따위지?ㅡ.ㅡ
	if(penToDamage)
	{
		CPrintF("------%f--InflictDirectDamage\n", _pTimer->GetLerpedCurrentTick());
		INDEX preHealth, curHealth;
		preHealth = ((CUnit*)penToDamage)->m_nPreHealth;
		curHealth = ((CUnit*)penToDamage)->m_nCurrentHealth;

		if(preHealth!= -1)// && curHealth > preHealth)
		{
			// FIXME : 싱글던젼에서는 강제로 DamageAmmount를 1로 만들지 않음...
			if( !_pNetwork->m_bSingleMode )
			{
				fDamageAmmount = 1;
			}
			((CUnit*)penToDamage)->m_nCurrentHealth = ((CUnit*)penToDamage)->m_nPreHealth;			
		}
		else fDamageAmmount = 0;

		if(pInfo->GetTargetEntity(eTARGET) == penToDamage)//타겟팅이 되어있다면...//0121
		{
			if (pInfo->GetTargetEntity(eTARGET) == this)
			{
				penToDamage->UpdateTargetInfo(_pNetwork->MyCharacterInfo.maxHP,_pNetwork->MyCharacterInfo.hp);
			}
			else
			{
				penToDamage->UpdateTargetInfo(((CUnit*) penToDamage)->m_nMaxiHealth,((CUnit*) penToDamage)->m_nCurrentHealth);//1022
			}
		}
			
		if( pInfo->GetTargetCurHP(eTARGET) <= 0 )//0827
		{			
			if(penToDamage == pInfo->GetTargetEntity(eTARGET))
			{
				pInfo->TargetClear(eTARGET);
			}
		}
		
		if (penToDamage->IsEnemy() && preHealth >= 0) // Player가 Enemy(권좌)를 공격할 경우가 있다. (왜 서버로 부터 Enemy의 hp udpate를 받을 때 처리하지 않는가?)
		{
			const INDEX iMobIndex = ((CEnemy*)penToDamage)->m_nMobDBIndex;

			if (iMobIndex == LORD_SYMBOL_INDEX) // 메라크 공성 권좌 NPC(사이즈는 바뀌지 않고, 모델을 바꾼다)
			{ // enemy에서 사용하지 않는 m_nPlayActionNum를 사용한다.
				CMobData* MD = CMobData::getData(iMobIndex);
				FLOAT fMaxHealth = ((CUnit*)penToDamage)->m_nMaxiHealth;

				if ((curHealth <= fMaxHealth * 0.25f))
				{
					if (((CUnit*)penToDamage)->m_nPlayActionNum != 0)
					{
						penToDamage->SetSkaModel("data\\npc\\Gguard\\sword04.smc");
						penToDamage->GetModelInstance()->RefreshTagManager();
						penToDamage->GetModelInstance()->StretchModel(FLOAT3D(MD->GetScale(), MD->GetScale(), MD->GetScale()));
						((CUnit*)penToDamage)->m_nPlayActionNum = 0;
					}
				}
				else if ((curHealth <= fMaxHealth * 0.50f))
				{
					if (((CUnit*)penToDamage)->m_nPlayActionNum != 1)
					{
						penToDamage->SetSkaModel("data\\npc\\Gguard\\sword03.smc");
						penToDamage->GetModelInstance()->RefreshTagManager();
						penToDamage->GetModelInstance()->StretchModel(FLOAT3D(MD->GetScale(), MD->GetScale(), MD->GetScale()));
						((CUnit*)penToDamage)->m_nPlayActionNum = 1;
					}
				}
				else if ((curHealth <= fMaxHealth * 0.75f))
				{
					if (((CUnit*)penToDamage)->m_nPlayActionNum != 2)
					{
						penToDamage->SetSkaModel("data\\npc\\Gguard\\sword02.smc");
						penToDamage->GetModelInstance()->RefreshTagManager();
						penToDamage->GetModelInstance()->StretchModel(FLOAT3D(MD->GetScale(), MD->GetScale(), MD->GetScale()));
						((CUnit*)penToDamage)->m_nPlayActionNum = 2;
					}
				}
			}
		}

		((CUnit*)penToDamage)->m_nPreHealth = -1; //1103
	}

	// NOTE : 죽을때는 밀리도록 설정함.
	if(penToDamage->IsEnemy())
	{
		const INDEX iMobIndex = ((CEnemy*)penToDamage)->m_nMobDBIndex;		

		const int iJob = en_pcCharacter.pc_iPlayerType;
		if(!_pNetwork->m_bSingleMode)
		{
			CMobData* MD = CMobData::getData(iMobIndex);
			// 권좌나 공성탑은 밀리면 안됨.
			if(MD->IsLordSymbol() || MD->IsCastleTower())
			{
				CEntity::InflictDirectDamage(penToDamage, penInflictor, dmtType,
				fDamageAmmount,vTargetPos, vDirection2);
			}
			else if( ( iJob == TITAN || 
				iJob == KNIGHT || 
				( iJob == ROGUE && !_pNetwork->MyCharacterInfo.bExtension ) || 
				( IsEXRogue(iJob)  && !_pNetwork->MyCharacterInfo.bExtension ) || // [2012/08/27 : Sora] EX로그 추가
				( iJob == SORCERER && m_iTransformType != CTransformInfo::TYPE_1 ) )&&	// 임시 - 타격 이펙트가 있는지 검사를 해서 있을 경우에는 기본 이펙트 처리를 빼야 함.
				((CUnit*)((CEntity*)penToDamage))->m_nCurrentHealth <= 0 && penToDamage != this )			// 현재 메이지 근접 공격은 타격 이펙트가 따로 있기 때문에 이 루틴을 타면 안됨. 대신 밀리지 않고 있음.
			{
				CEntity::InflictDirectDamage(penToDamage, penInflictor, DMT_EXPLOSION,
				15.0f, vTargetPos, vDirection2);
			}
			else
			{
				CEntity::InflictDirectDamage(penToDamage, penInflictor, dmtType,
				fDamageAmmount,vTargetPos, vDirection2);
			}
		}
		// FIXME : 싱글 모드의 경우...
		// FIXME : 미사일의 경우와도 동일한데, 데미지 크기만 다름.
		// FIXME : 모두 다 Hardcodingㅡ.ㅡ
		else
		{
			// FIXME : 10번 월드는 특히...
			if( g_slZone == EXTREAM_SINGLEDUNGEON )
			{
				// FIXME : 일반 스킬을 쓸때도 엄청나게 멀리 밀리는 문제가 있어서,
				// FIXME : 싱글 던젼 스킬(DMT_EXPLOSION타입의...)을 슬때만 엄청나게 밀리도록 하려고...
				if( dmtType == DMT_EXPLOSION )
				{
					const int iJob = en_pcCharacter.pc_iPlayerType;
					FLOAT fDamageAmount	= 40;
					if( !((CEnemy*)penToDamage)->m_bBoss )
					{
						if( iJob == TITAN )
						{
							fDamageAmount	= 70;
						}
						else if( iJob == KNIGHT )
						{
							fDamageAmount	= 60;
						}		
					}
					else	// 보스 일 경우.
					{					
						fDamageAmount	= 15;
					}
					vDirection2(1) = 0.0f;
					vDirection2.Normalize();
					CEntity::InflictDirectDamage(penToDamage, penInflictor, DMT_EXPLOSION,
						fDamageAmount, vTargetPos, vDirection2);
				}
				else
				{
					CEntity::InflictDirectDamage(penToDamage, penInflictor, DMT_EXPLOSION,
						fDamageAmmount, vTargetPos, vDirection2);
				}
			}
			else
			{
				CEntity::InflictDirectDamage(penToDamage, penInflictor, DMT_EXPLOSION,
				fDamageAmmount, vTargetPos, vDirection2);
			}
		}
	}
	else if(penToDamage->IsCharacter())//캐릭터.
	{
		SE_Get_UIManagerPtr()->ShowDamage( penToDamage->en_ulID );		
		CEntity::InflictDirectDamage(penToDamage, penInflictor, dmtType,
			fDamageAmmount,vTargetPos, vDirection2);
	}
	else
	{
		const int iJob = en_pcCharacter.pc_iPlayerType;
		//if( !_pNetwork->m_bSingleMode )
		//{
			if( ( iJob == TITAN || iJob == KNIGHT || 
				!( iJob == SORCERER && m_iTransformType == CTransformInfo::TYPE_1) || 
				( iJob == ROGUE && !_pNetwork->MyCharacterInfo.bExtension ) || // 임시 - 타격 이펙트가 있는지 검사를 해서 있을 경우에는 기본 이펙트 처리를 빼야 함.
				( IsEXRogue(iJob)  && !_pNetwork->MyCharacterInfo.bExtension ) ) &&	// [2012/08/27 : Sora] EX로그 추가
				((CUnit*)((CEntity*)penToDamage))->m_nCurrentHealth <= 0 && penToDamage != this )			// 현재 메이지 근접 공격은 타격 이펙트가 따로 있기 때문에 이 루틴을 타면 안됨. 대신 밀리지 않고 있음.
			{
				CEntity::InflictDirectDamage(penToDamage, penInflictor, DMT_EXPLOSION,
				15.0f, vTargetPos, vDirection2);
			}
			else
			{
				CEntity::InflictDirectDamage(penToDamage, penInflictor, dmtType,
				fDamageAmmount,vTargetPos, vDirection2);
			}
		//}
	}

	((CLiveEntity*)((CEntity*)penToDamage))->SetHealth(1000.0f);

	// FIXME : 아래 코드 중복이 심함.
	if(penToDamage->IsEnemy())
	{
		if(((CUnit*)((CEntity*)penToDamage))->m_nCurrentHealth <= 0 && penToDamage != this)
		{
			CMobData* MD = CMobData::getData(((CEnemy*)penToDamage)->m_nMobDBIndex);
			if(!MD->IsCastleTower())
			{
				((CUnit*)((CEntity*)penToDamage))->DeathNow();
			}
			else 
			{
				// WSS_DRATAN_SEIGEWARFARE 2007/08/31
				// 공격 수호상중 드라탄 타워 예외처리 ( 마스터 타워 제외 )
				if (MD->GetMobIndex() >= DEF_MOB_ID_DRATAN_TOWER && 
					MD->GetMobIndex() <= DEF_MOB_ID_MASTER_TOWER)
				{
					((CUnit*)((CEntity*)penToDamage))->DeathNow();
				}
				else 
				{
					penToDamage->SetFlagOff(ENF_ALIVE);
				}
			}
	
			if(penToDamage == pInfo->GetTargetEntity(eTARGET))
			{
				pInfo->TargetClear(eTARGET);
			}		
		}
	}
	else
	{
		if(((CUnit*)((CEntity*)penToDamage))->m_nCurrentHealth <= 0 && penToDamage != this)
		{
			((CUnit*)((CEntity*)penToDamage))->DeathNow();
	
			if(penToDamage == pInfo->GetTargetEntity(eTARGET))
			{
				pInfo->TargetClear(eTARGET);
			}		
		}
	}
};

virtual void CancelSkill(BOOL bLostTarget, BOOL bAutoAttack, BOOL bSkillError)
{
	m_bStartPostSkill = TRUE;

	if (m_penStillTarget != NULL)
	{
		m_penStillTarget = NULL;
	}

	if (m_bConnectEffect)
	{
		m_bConnectEffect = FALSE;
		m_pSkillTexture.SetData(NULL);
		StopEffectGroupIfValid(m_pAttachPlayerEffect, 0.1f);
		StopEffectGroupIfValid(m_pAttachEnemyEffect, 0.1f);
	}

	if( g_bPostSkill && INFO()->GetTargetEntity(eTARGET) != NULL )
	{
		if(m_nCurrentSkillNum != -1)
		{
			_pNetwork->SendCancelSkillMessage();
		}

		return;
	}
	else if ( g_bPostSkill && INFO()->GetTargetEntity(eTARGET) == NULL )
	{ // 타겟이 없기 때문에 스킬이 사용되지 않는다. 취소 가능 상태로 전환 한다.
		g_bPostSkill = FALSE;
		m_bLockSkillCancel = FALSE;
	}
	
	if(m_bLockSkillCancel || (m_bWaitForSkillResponse && !bSkillError))
	{
		if(m_nCurrentSkillNum != -1)
		{
			_pNetwork->SendCancelSkillMessage();
		}

		return;
	}

	if ( _pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_FAKEDEATH) &&
		m_bStartAttack ) // 데스모션 적용중이고, 공격을 시작했을때
	{
		m_bStartAttack = FALSE; // 공격이 취소 되어야 한다.
	}

	if(m_bStartAttack)//0807
	{		
		m_nCurrentSkillNum = -1;
		m_bReservedCancel = !bAutoAttack;
		return;
	}
	else
	{
		if ( m_nCurrentSkillNum != -1 )
		{
			CSkill &SkillData = _pNetwork->GetSkillData(m_nCurrentSkillNum);
			if(SkillData.bCanCancel)
			{
				if((g_bPreSkill || g_bDoSkill) && !g_bPostSkill)
				{
					_pNetwork->SendCancelSkillMessage();
					m_nCurrentSkillNum = -1;
				}
			}
		}

		m_bReservedCancel = !bAutoAttack;
		m_bWaitForSkillResponse = FALSE;
	}
/*
	if(!m_bSkilling && m_bWaitForSkillResponse)//0103
	{
		m_bWaitForSkillResponse = FALSE;
		_pNetwork->SendSkillCancelMessage();
	}
*/
	if(((g_bPreSkill || g_bDoSkill) && !g_bPostSkill)|| !(g_bPreSkill||g_bDoSkill||g_bPostSkill) )
	{	
		//m_bRunningSelfSkill = FALSE;
		m_idCurrentSkillAnim = -1;//스킬 종료.
		m_tmSkillStartTime = 0.0f;
		m_bSkilling = FALSE;
		m_nCurrentSkillNum = -1;
		m_nDesiredSkillNum = -1;

		m_bLockMove = FALSE;

		m_nReservedSkillNum = -1;

		m_bWaitForSkillTarget = FALSE;

		m_bWaitForSkillResponse = FALSE;

		m_bCanSkillCancel = TRUE;

		m_vDesiredPosition =  GetPlacement().pl_PositionVector;	

		if(GetPlayerWeapons()->m_penRayHitTmp != NULL
			&& ((!bLostTarget && !bAutoAttack) || bLostTarget)
		) //0807 
		{
			//GetPlayerWeapons()->m_penRayHitTmp->DelTargetInfo();
			GetPlayerWeapons()->m_penRayHitTmp = NULL;
		}

		if(bLostTarget)
		{
			//0919			
			INFO()->TargetClear(eTARGET);
			
			// 타겟을 잃었다면 서버에 알려준다.
			if (STAGEMGR()->GetCurStage() == eSTAGE_GAMEPLAY && STAGEMGR()->GetNextStage() == eSTAGE_NONE)
			{
				_pNetwork->SendClickObject(-1);
			}

			if(GetPlayerWeapons()->m_penRayHitClick != NULL) //0909 
			{			
				GetPlayerWeapons()->m_penRayHitClick = NULL;
			}
			if(GetPlayerWeapons()->m_penReservedTarget != NULL) //0909 
			{			
				GetPlayerWeapons()->m_penReservedTarget = NULL;
			}
		}
		
		SE_Get_UIManagerPtr()->SetCSFlagOff( CSF_SKILL );

//안태훈 수정 시작	//(Open beta)(2004-12-21)
		if(m_pSkillReadyEffect != NULL && CEffectGroupManager::Instance().IsValidCreated(m_pSkillReadyEffect))
		{
			m_pSkillReadyEffect->Stop(0.1f);
			m_pSkillReadyEffect = NULL;
		}
		m_iSkillEffectStep = 0;
//안태훈 수정 끝	//(Open beta)(2004-12-21)

		g_bPreSkill=g_bDoSkill=g_bPostSkill=FALSE; // 스킬 사용중 Esc로 중단시 버그로 초기화(이동 불능 상태)
	}
	//m_bReadySendSkillMessage = TRUE; //0824
}

//0815 kwon	
virtual void Rebirth()
{	
	m_bDeathChk		= FALSE; // wooss 050928
	m_bDying		= FALSE;
	en_fHealth		= 1000000.0f;
	m_bRcvAtMsg		= TRUE;
	m_bLockMove		= FALSE;
	//_pNetwork->DeleteAllMob();

	CUIManager* pUIManager = SE_Get_UIManagerPtr();

	pUIManager->CloseMessageBox(MSGCMD_PC_DEATH);

	// resurrection scroll 관련 변경 사항 적용. [12/8/2009 rumist]
	pUIManager->GetResurrectionMsgBox()->CloseResurrectionMsgBox();

	SendEvent(EEnd());	
}

void CheckField()
{
	if(g_slZone == 0)
	{
		if(_pNetwork->MyCharacterInfo.x <= AREA_CASTLE_X0+100 || _pNetwork->MyCharacterInfo.x >= AREA_CASTLE_X1+100
				|| _pNetwork->MyCharacterInfo.z <= AREA_CASTLE_Z0-100 || _pNetwork->MyCharacterInfo.z >= AREA_CASTLE_Z1+100)		
		{
			m_bEnvironmentSound = FALSE;
		}
		else
		{
			m_bEnvironmentSound = FALSE;
		}

		if(_pNetwork->MyCharacterInfo.x >= AREA_CASTLE_X0 && _pNetwork->MyCharacterInfo.x <= AREA_CASTLE_X1
		&& _pNetwork->MyCharacterInfo.z >= AREA_CASTLE_Z0 && _pNetwork->MyCharacterInfo.z <= AREA_CASTLE_Z1)
		{
			m_bFieldArea = FALSE;
		}
		else //성밖
		{			
			m_bFieldArea = TRUE;
		}
	}
	else if(g_slZone == 4)//드라탄,
	{
		if(_pNetwork->MyCharacterInfo.x <= AREA_CASTLE_DRATAN_X0+100 || _pNetwork->MyCharacterInfo.x >= AREA_CASTLE_DRATAN_X1+100
				|| _pNetwork->MyCharacterInfo.z <= AREA_CASTLE_DRATAN_Z0-100 || _pNetwork->MyCharacterInfo.z >= AREA_CASTLE_DRATAN_Z1+100)		
		{
			m_bEnvironmentSound = FALSE;
		}
		else
		{
			m_bEnvironmentSound = FALSE;
		}

		if(_pNetwork->MyCharacterInfo.x >= AREA_CASTLE_DRATAN_X0 && _pNetwork->MyCharacterInfo.x <= AREA_CASTLE_DRATAN_X1
		&& _pNetwork->MyCharacterInfo.z >= AREA_CASTLE_DRATAN_Z0 && _pNetwork->MyCharacterInfo.z <= AREA_CASTLE_DRATAN_Z1)
		{
			m_bFieldArea = FALSE;
		}
		else
		{
			m_bFieldArea = TRUE;
		}
	}
	else //if(g_slZone == 1)
	{

	}
}

virtual void PlayBGM(CTFileName fnFileName)
{
	CEntity *penMusicHolder = _pNetwork->GetEntityWithName("MusicHolder", 0);
	ASSERT( penMusicHolder != NULL && "Cannot find Music-Holder!" );

	EChangeMusic ecm;
	ecm.fVolume		= m_fVolume;
	ecm.mtType		= m_mtType;
	ecm.bForceStart = TRUE;
	ecm.fnMusic		= fnFileName;
	penMusicHolder->SendEvent(ecm);
}

void PlayMusic()
{
	if(g_slZone!=0 && g_slZone!=4)
	{
		return;
	}

	CEntity *penMusicHolder = _pNetwork->GetEntityWithName("MusicHolder", 0);
	ASSERT( penMusicHolder != NULL && "Cannot find Music-Holder!" );

	EChangeMusic ecm;
	ecm.fVolume = m_fVolume;
	ecm.mtType  = m_mtType;
	ecm.bForceStart = TRUE;

	static BOOL _bEnviron = FALSE;

	if(m_bEnvironmentSound)
	{
		switch(g_slZone)
		{
			case 0://스타트존.
				ecm.fnMusic = MUSIC_FIELD_ENVIRON;
				break;
			case 1://던전1
				ecm.fnMusic = MUSIC_DUNGEON_ENVIRON;
				break;
			case 3://던전2
				ecm.fnMusic = MUSIC_DUNGEON_ENVIRON;
				break;
			case 4://드라탄
				ecm.fnMusic = MUSIC_DRATAN_ENVIRON;
				break;
			case 7:
				ecm.fnMusic = MUSIC_MERAC_FIELD;
				break;
			case 9:	// 메라크 던젼.
				ecm.fnMusic = MUSIC_DUNGEON_ENVIRON;
				break;
			case 15: // 에게하
				ecm.fnMusic = MUSIC_EGEHA_PEASE;
				break;
		}	
		
		penMusicHolder->SendEvent(ecm);
		return;
	}

	static BOOL _bField = FALSE;

	// FIXME : 아래 모하는 건지 모르겠음...-_-;;;
	if(_bField != m_bFieldArea)
	{
		_bField = m_bFieldArea;
		if(m_bFieldArea)
		{
			if(g_slZone == 0)
			{
				ecm.fnMusic = MUSIC_FIELD_AREA;
			//	ecm.fnMusic = MUSIC_FIELD_ENVIRON;
			}
			else if(g_slZone == 4)
			{
				ecm.fnMusic = MUSIC_FIELD_AREA;
			//	ecm.fnMusic = MUSIC_DRATAN_ENVIRON;
			}
			else if(g_slZone == 15)
			{
				ecm.fnMusic = MUSIC_FIELD_AREA;
			}
		}
		else
		{
			ecm.fnMusic = MUSIC_CASTLE_AREA;
		}
		penMusicHolder->SendEvent(ecm);
	}
}

//안태훈 수정 시작	//(5th Closed beta)(0.2)
virtual void AddDeathItem(CEntity *penEnemy, FLOAT3D pos, class CItemTarget *pItemTarget, class CItemData *pItemData)
{
	ASSERT(penEnemy != NULL && pItemTarget != NULL);
	((CEnemy*)penEnemy)->AddDeathItem(pos, pItemTarget, pItemData);
}
//안태훈 수정 끝	//(5th Closed beta)(0.2)

// 전사의 시스템 작업으로 추가 wooss 060908
virtual void DropDeathItem(CEntity *penEnemy)
{
	ASSERT(penEnemy != NULL);
	((CEnemy*)penEnemy)->DropDeathItem();
}

bool IsTestClient()
{
	extern UINT g_uiEntityVersion;
	extern ENGINE_API INDEX g_bTestClient;
	if( g_bTestClient && g_uiEntityVersion >= 10000 )
	{
		return true;
	}
	return false;
};

virtual void SearchNearItem() //1020
{
	FLOAT fDistance = 1000.0f;//충분히 큰 수.
	FLOAT fTmpDistance = -1.0f;
	CEntity	*pItemEntity = NULL;
	INDEX iItemID = -1;
	
	// if you riding wildpet, don't pickup items. [12/12/2010 rumist]
	if( m_bRide || m_bWildRide )
	{
		return;
	}

	/* [2011/12/29 : Sora] 데스모션 중에 액션을 사용 하면 캐릭터가 움직이지 않는 버그 확인
		기획측과 협의하여 데스모션 중에는 액션을 사용할 수 없도록 수정 */
	if( _pNetwork->MyCharacterInfo.statusEffect.IsState(EST_ASSIST_FAKEDEATH) )
	{
		return;
	}

	if(m_bPlayAction)
	{
		return;
	}

	ObjectBase* pObject = ACTORMGR()->GetObjectByDistance(GetPlacement().pl_PositionVector);

	if (pObject != NULL && pObject->m_eType == eOBJ_ITEM)
	{
		CItemTarget* pTarget = static_cast< CItemTarget* >(pObject);

		FLOAT3D vDelta = GetPlacement().pl_PositionVector - pTarget->item_place;
			vDelta(2) = 0.0f; //0131 높이값 무시.
		
		iItemID = pTarget->GetSIndex();
		fDistance = vDelta.Length();		
		pItemEntity = pTarget->GetEntity();
	}

	CPrintF("Near Item Distance : %f \n", fDistance);
	if(fDistance != 1000.0f && pItemEntity != NULL && fDistance<=20.0f)
	{

		if(fDistance <= 2.0f && iItemID != -1)
		{
			_UIAutoHelp->SetInfo ( AU_PICK_ITEM );
			m_bPlayAction = TRUE;
			m_nPlayActionNum = ACTION_NUM_PICKITEM;
			_pNetwork->SendPickMessage( this, iItemID, FALSE );
			return;
		}

		if(m_bLockMove)
		{
			return;
		}

		GetPlayerWeapons()->m_vRayHitTmp = pItemEntity->en_plPlacement.pl_PositionVector;
		GetPlayerWeapons()->m_penRayHitTmp = pItemEntity;

		if(_pNetwork->IsPlayerLocal(this))
		{
			m_bForward = TRUE;
			//m_bKeyMove = TRUE;	
		}
		
		m_vDesiredPosition  = FLOAT3D(GetPlayerWeapons()->m_vRayHitTmp(1),GetPlayerWeapons()->m_vRayHitTmp(2),GetPlayerWeapons()->m_vRayHitTmp(3));

		m_bReserveMove = FALSE;
	}
}

BOOL IsNotPersnalDungeon()
{
	if ( g_slZone != PD_STRAIA && g_slZone != PD_MINOC &&
		g_slZone != T_STRAIA && g_slZone != PD_AJAKA && g_slZone != PD_GATEofDIMENSION )
	{
		return TRUE;
	}

	return FALSE;
}

virtual void DeathYou()
{
	CUIManager* pUIManager = SE_Get_UIManagerPtr();

	if (pUIManager->m_DamageLogInfo.GetStart())
	{
		pUIManager->m_DamageLogInfo.EndDamageMode();
		_UIAutoHelp->SetGMNotice ( "데미지를 기록하는 모드를 종료합니다.", 0xFFAA33FF );
	}

	pUIManager->ClearDamageData();

	m_bSendStopMessage = TRUE;
	m_iSkillEffectStep = 0;
	m_bDying = TRUE;
	m_nLegit = 0;
	m_bStuned = FALSE;
	m_bHold = FALSE;
	m_bDeathChk = TRUE;

	m_bChanging = FALSE;
	m_bTransforming = FALSE;

	// 죽게되면 원래 상태로 복구.
	if(m_bMobChange || ISTRANSFORMATION(_pNetwork->MyCharacterInfo.eMorphStatus))
	{
		ReturnChange(TRUE);			
	}
	else if(m_bIsTransform || ISEVOCATION(_pNetwork->MyCharacterInfo.eMorphStatus))
	{
		ReturnSorcerer();
		m_iTransformType	= 0;
		EvocationStop(_pNetwork->MyCharacterInfo.index);
	}

	AppearWearingWeapon(FALSE);		

	if(GetPlayerWeapons()->m_penRayHitTmp != NULL) //0807 
	{							
		GetPlayerWeapons()->m_penRayHitTmp = NULL;
	}

	INFO()->TargetClear();
	
	if(GetPlayerWeapons()->m_penRayHitNow != NULL)
	{
		GetPlayerWeapons()->m_penRayHitNow = NULL;
	}
	
	m_penAttackingEnemy = NULL;
	m_bStartAttack = FALSE;								

	m_bLockSkillCancel = FALSE;
	m_bWaitForSkillResponse = FALSE;

	if(m_nCurrentSkillNum != -1)
	{		
		if(g_bPreSkill || g_bDoSkill || g_bPostSkill)
		{
			_pNetwork->SendCancelSkillMessage();
		}	
	}

	g_bPreSkill=g_bDoSkill=g_bPostSkill=FALSE;//0823
	CancelSkill(TRUE, g_iAutoAttack, FALSE);
	PlayerInit(true);

	_pNetwork->MyCharacterInfo.hp=0;
	_pNetwork->MyCharacterInfo.pk_mode   = 0; 
	m_bRcvAtMsg = FALSE;
									
	//0824 액션 취소.
	m_bPlayAction = FALSE;
	m_nPlayActionNum = -1;
	m_nActionSit = 0;		
	
	m_bProduction = FALSE;
	m_nProductionNum = -1;

	if(IsPvp())//죽으면 pk모드 해제.
	{
		_pNetwork->SetPvpMode();
	}

	//상태이상 정보를 초기화한다.
	_pNetwork->MyCharacterInfo.statusEffect.Reset();

	pUIManager->CloseMessageBox( MSGCMD_PC_DEATH );
	
	if(_pNetwork->m_iServerType == SERVER_TYPE_HARDCORE ) // 하드코어 서버이고
	{
		pUIManager->GetResurrectionMsgBox()->OpenResurrectionMsgBox( 300, TRUE );
		EPlayerDeath ePLDeath;
		SendEvent(ePLDeath);
		return;
	}	
	
	CTString strTitle,strMessage;
	// Date : 2006-05-02(오후 12:53:54), By eons
	// RAID 추가로 사망시 메시지 변경, 시간 제한 추가
	// used royal rumble only.
	if( _pNetwork->MyCharacterInfo.zoneNo == 38 )
	{
		pUIManager->GetResurrectionMsgBox()->OpenResurrectionMsgBox( 30, TRUE );
		EPlayerDeath ePLDeath;
		SendEvent(ePLDeath);
		return;
	}

	if(IsNotPersnalDungeon())
	{// 퍼스널 던전이 아니라면
		
		// WSS_DRATAN_SEIGEWARFARE 2007/10/11
		// WSS_DRATAN_SEIGEWARFARE 2007/08/22 
		// 드라탄 공성 부활 대기 추가
		// 공성 중이고 공성 참가 중이면...
		if( pUIManager->GetSiegeWarfareNew()->GetWarState() &&
			(_pNetwork->MyCharacterInfo.sbAttributePos & MATT_WAR ||  MyCheckDratanWarInside())
			&& _pNetwork->MyCharacterInfo.sbJoinFlagDratan != WCJF_NONE )
		{
			// [2011/07/13 : Sora] 부활대기창 여는 함수 추가
			pUIManager->GetSiegeWarfareNew()->OpenWaitTime();
			EPlayerDeath ePLDeath;
			SendEvent(ePLDeath);
			return;		
		}

		if(_pUIBuff->IsBuff(RECOVER_HEXP_ITEM) || _pUIBuff->IsBuff(RECOVER_HEXP_ITEM_LUCKY))
		{			
			strTitle = _S( 2088, "사용 확인" ); 
			CUIMsgBox_Info	MsgBoxInfo;

			// [sora] 사망시 메시지박스에 제한시간 추가
			strMessage = _S(4689,"캐릭터가 죽었습니다.\n경험치 복구 주문서를 \n사용하시겠습니까? \n취소시  부활위치로 이동합니다.(제한 시간 이후 자동으로 이동합니다.)");
			
			if( _pUIBuff->IsBuff(RECOVER_HEXP_ITEM) )
			{
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO|UMBS_USE_TIMER,UI_NONE, MSGCMD_USE_CONFIRM_HEXP);
			}
			else
			{
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO|UMBS_USE_TIMER,UI_NONE, MSGCMD_USE_CONFIRM_HEXP_LUCKY);
			}
			MsgBoxInfo.AddString(strMessage);
			MsgBoxInfo.SetMsgBoxTimer(300, FALSE);					

			pUIManager->CreateMessageBox(MsgBoxInfo);
			EPlayerDeath ePLDeath;
			SendEvent(ePLDeath);
			return;
			
		}  
		
		if(_pUIBuff->IsBuff(RECOVER_AEXP_ITEM) || _pUIBuff->IsBuff(RECOVER_AEXP_ITEM_LUCKY))
		{
			strTitle = _S( 2088, "사용 확인" ); 
			CUIMsgBox_Info	MsgBoxInfo;

			// [sora] 사망시 메시지박스에 제한시간 추가
			strMessage = _S(4690, "캐릭터가 죽었습니다.\n숙련도 복구 주문서를\n사용하시겠습니까? \n취소시  부활위치로 이동합니다.(제한 시간 이후 자동으로 이동합니다.)");
			if(_pUIBuff->IsBuff(RECOVER_AEXP_ITEM))
			{
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO|UMBS_USE_TIMER,UI_NONE, MSGCMD_USE_CONFIRM_AEXP);
			}
			else
			{
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO|UMBS_USE_TIMER,UI_NONE, MSGCMD_USE_CONFIRM_AEXP_LUCKY);
			}
			
			MsgBoxInfo.AddString(strMessage);
			MsgBoxInfo.SetMsgBoxTimer(300, FALSE);

			pUIManager->CreateMessageBox(MsgBoxInfo);
			EPlayerDeath ePLDeath;
			SendEvent(ePLDeath);
			return;
		} 
		
		if(_pUIBuff->IsBuff(REBIRTH_ITEM) || _pUIBuff->IsBuff(REBIRTH_ITEM_NEWBIE) || _pUIBuff->IsBuff(REBIRTH_ITEM_EVENT))
		{	
			strTitle=_S( 2088, "사용 확인" ); 
			CUIMsgBox_Info	MsgBoxInfo;
			
			// [sora] 사망시 메시지박스에 제한시간 추가
			strMessage = _S(4691, "캐릭터가 죽었습니다.\n부활 주문서를\n사용하시겠습니까? \n취소시  부활위치로 이동합니다.(제한 시간 이후 자동으로 이동합니다.)");
			MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO|UMBS_USE_TIMER,UI_NONE, MSGCMD_USE_CONFIRM_REBIRTH);
			MsgBoxInfo.AddString(strMessage);

			//[5/11/2011 ldy1978220] LC-DE-20110428-004 부활 주문서(846,2667) 사용 설정 변경(Gamigo 로컬만 ) 
			if ( IsGamigo( g_iCountry ) )
			{
				MsgBoxInfo.SetMsgBoxTimer(300, TRUE);
			}
			else 
			{
				MsgBoxInfo.SetMsgBoxTimer(300, FALSE);
			}
		
			pUIManager->CreateMessageBox(MsgBoxInfo); 
			EPlayerDeath ePLDeath;
			SendEvent(ePLDeath);
			return;
		}	

		if(_pUIBuff->IsBuff(REBIRTH_ITEM_PHOENIX) )
		{
			strTitle=_S( 2088, "사용 확인" );
			CUIMsgBox_Info	MsgBoxInfo;

			// [sora] 사망시 메시지박스에 제한시간 추가
			strMessage = _S(4692, "캐릭터가 죽었습니다.\n피닉스의부활을\n사용하시겠습니까? \n취소시  부활위치로 이동합니다.(제한 시간 이후 자동으로 이동합니다.)");
			MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO|UMBS_USE_TIMER,UI_NONE, MSGCMD_USE_CONFIRM_REBIRTH);
			MsgBoxInfo.AddString(strMessage);
			MsgBoxInfo.SetMsgBoxTimer(300, FALSE);

			pUIManager->CreateMessageBox(MsgBoxInfo); 
			EPlayerDeath ePLDeath;
			SendEvent(ePLDeath);
			return;
		}

	}
	else
	{	// 퍼스널 던전
		// 퍼스널 던전에서는 어떠한 부활 주문서를 사용할 수 없다.
		pUIManager->GetResurrectionMsgBox()->OpenResurrectionMsgBox( 300, TRUE );

		EPlayerDeath ePLDeath;
		SendEvent(ePLDeath);

		return;
	}
	pUIManager->GetResurrectionMsgBox()->OpenResurrectionMsgBox( 300 );

	EPlayerDeath ePLDeath;
	SendEvent(ePLDeath);
}

virtual BOOL IsAlreadyDie()
{
	return m_bDying;
}

virtual   void SetDie()
{
	m_bDying = TRUE;
}

virtual void PlayerInit(bool bDeath)
{
	m_bStartAttack = FALSE;	
	m_bLockSkillCancel = FALSE;
	m_bWaitForSkillResponse = FALSE;
	
	m_idCurrentSkillAnim = -1;//스킬 종료.
	m_tmSkillStartTime = 0.0f;
	m_bSkilling = FALSE;
	m_nCurrentSkillNum = -1;
	m_nDesiredSkillNum = -1;

	m_bReserveMove = FALSE;

	m_nReservedSkillNum = -1;

	m_bWaitForSkillTarget = FALSE;	

	m_bCanSkillCancel = TRUE;
	
	m_vDesiredPosition =  GetPlacement().pl_PositionVector;	
	if(GetPlayerWeapons()->m_penRayHitTmp != NULL) //0807 
	{	
		GetPlayerWeapons()->m_penRayHitTmp = NULL;
	}
	
	INFO()->TargetClear(eTARGET);
	if(GetPlayerWeapons()->m_penRayHitClick != NULL) //0909 
	{			
		GetPlayerWeapons()->m_penRayHitClick = NULL;
	}
	if(GetPlayerWeapons()->m_penReservedTarget != NULL) //0909 
	{			
		GetPlayerWeapons()->m_penReservedTarget = NULL;
	}
	
	CUIManager* pUIManager = SE_Get_UIManagerPtr();

	pUIManager->SetCSFlagOff( CSF_SKILL );	
	pUIManager->SetCSFlagOff( CSF_TELEPORT );
	
	// WSS_BUG_FIX 070815
	if(pUIManager->IsCSFlagOn(CSF_PERSONALSHOP))
	{
	  pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );  
	  _pNetwork->MyCharacterInfo.sbShopType = PST_NOSHOP;
	  _pNetwork->MyCharacterInfo.ShopMsg.Reset();
	  pUIManager->GetPersonalShop()->ResetShop();
	}

	if (bDeath == false)
	{
		IdleAnim();
		m_bLockMove = FALSE;
	}
	else
	{
		m_bLockMove = TRUE;
		SetFlagOff(ENF_HIDDEN);
	}
}

BOOL IsPvp()
{
	return _pNetwork->m_bIsPvp;
}

virtual	BOOL IsMoving()
{
	return m_bForward | m_bKeyMove;
}

virtual BOOL IsInputMovingKey()
{
	return ( ((INDEX)_pInput->GetInputDevice(1)->id_aicControls[KID_W].ic_fValue) |
			 ((INDEX)_pInput->GetInputDevice(1)->id_aicControls[KID_S].ic_fValue) |
			 ((INDEX)_pInput->GetInputDevice(1)->id_aicControls[KID_A].ic_fValue) |
			 ((INDEX)_pInput->GetInputDevice(1)->id_aicControls[KID_D].ic_fValue) );
}

BOOL IsInputRotateKey()
{
	return ( (_pInput->GetInputDevice(1)->id_aicControls[KID_W].ic_fValue) == 0.0f &&
			 (_pInput->GetInputDevice(1)->id_aicControls[KID_S].ic_fValue) == 0.0f &&
			 ((_pInput->GetInputDevice(1)->id_aicControls[KID_A].ic_fValue) > 0.0f ||
			 (_pInput->GetInputDevice(1)->id_aicControls[KID_D].ic_fValue) > 0.0f) );
}

virtual BOOL InputKey(INDEX Code)
{
	// 카메라 위치를 기준으로
	CPlayerAction pa;
	//m_bForward = TRUE;
	BOOL bButtonAction = FALSE;
	BOOL bRotate = FALSE;
	BOOL bMove = FALSE;
	static BOOL s_bButtonPressedType = 0;		// 1 : W // 2 : S
	static BOOL bPrevPressType = 0;

	CUIManager* pUIManager = SE_Get_UIManagerPtr();

	if (!pUIManager->GetSelectResource()->GetKeyMove())
	{
		return FALSE;
	}

	// packet 오버때문에 체크.
	if( !IsInputMovingKey() )
	{
		// 키보드 작동시 더이상 변수에 대한 보장이 없어서
		// 강제로 초기화 함.
		if (m_bKeyMove && (!m_bOnlyRotate))
		{
			if (!m_bSendStopMessage)
			{
				StopMove();
			}
		}

		m_bKeyMove = FALSE;
		return FALSE;
	}

	if (!m_bKeyMove && !m_bOnlyRotate) 
	{
		if (m_bForward /*&& (_pInput->GetInputDevice(1)->id_aicControls[KID_W].ic_fValue > 0.0f) ||
			(_pInput->GetInputDevice(1)->id_aicControls[KID_S].ic_fValue > 0.0f)*/) // 마우스 이동 중 이동 키 입력시 스탑 메시지를 보내고 이동한다.
		{
			if (!m_bSendStopMessage)
			{
				StopMove();
			}
		}
		m_bKeyMove = TRUE;
	}

	m_bOnlyRotate = FALSE;

	FLOATmatrix3D m;
	CPlacement3D plView;
	GetLerpedAbsoluteViewPlacement(plView);
	plView.pl_OrientationAngle(2) = 0.0f;
	plView.pl_OrientationAngle(3) = 0.0f;
	// 회전 행렬 만들기...
	MakeRotationMatrixFast(m, plView.pl_OrientationAngle);

	// 축 벡터 얻기.
	FLOAT3D vRight = m.GetColumn(1);
	FLOAT3D vUp    = m.GetColumn(2);
	FLOAT3D vFront = m.GetColumn(3);

//	FLOAT3D vMove = GetPlacement().pl_PositionVector - en_plViewpoint.pl_PositionVector;
//	FLOAT3D vUp = FLOAT3D(0.0f, 1.0f, 0.0f);
//	FLOAT3D vSide = FLOAT3D(1.0f, 0.0f, 0.0f);
//	vMove = vMove.Normalize(); // 정면
//	vSide = vMove * vUp; // 외적 (옆면)
	
	if ((!m_bHold || !m_bStuned))
	{
		if (_pInput->GetInputDevice(1)->id_aicControls[KID_W].ic_fValue == 1.0f)
		{
			s_bButtonPressedType = 1;
		}
		else if (_pInput->GetInputDevice(1)->id_aicControls[KID_S].ic_fValue == 1.0f)
		{
			s_bButtonPressedType = 2;
		}
		else 
		{
			if( !m_bSendStopMessage )
			{
				StopMove();
			}
			bMove = FALSE;
			bButtonAction = FALSE;
		}
	}

	//  [11/25/2009 rumist]
	// 공격 또는 스킬 사용중 캔슬을 하였을 경우 또는 키보드 입력시 행동을 마친 후 캔슬 하기 위한 부분.
	// 행동중에는 키보드가 먹히지 않는다.
	// 단 그 행동중 키보드 입력이 되었을 경우 버퍼에 쌓아 두고 타입에 따라 처리한다.
	// 공격중 or 스킬 사용 중 타격이 완료된 상태에서 키보드 입력이 있을 시 연속공격을 중단하고, 이동이 가능하도록 키보드 입력 처리
	if( s_bButtonPressedType )
	{
		if( ! ( IsAttacking() || IsSkilling() ) )
		{
			// 1초 단위로 이동 패킷을 전송하기 때문에 1.5배 앞 위치로 이동요청한다.
			if ( s_bButtonPressedType == 1 )// W
			{
				m_vDesiredPosition = en_plPlacement.pl_PositionVector - (vFront * (plr_fSpeed * 1.5f));
				bButtonAction = TRUE;
				bMove = TRUE;
			}
			else if( s_bButtonPressedType == 2 )// S
			{
				m_vDesiredPosition = en_plPlacement.pl_PositionVector + (vFront * (plr_fSpeed * 1.5f));
				bButtonAction = TRUE;
				bMove = TRUE;
			}
			
			if (bPrevPressType != s_bButtonPressedType)
			{
				bPrevPressType = s_bButtonPressedType;
				tmStartTime = 0;
			}

			s_bButtonPressedType = 0;
		}
	}

/* 	else if (_pInput->GetInputDevice(1)->id_aicControls[KID_Q].ic_fValue == 1.0f)
 	{
 		m_vDesiredPosition = en_plPlacement.pl_PositionVector - (vRight * (plr_fSpeed/_pTimer->TickQuantum));
 		bButtonAction = TRUE;
 		bMove = TRUE;
 	}
 	else if (_pInput->GetInputDevice(1)->id_aicControls[KID_E].ic_fValue == 1.0f)
 	{
 		m_vDesiredPosition = en_plPlacement.pl_PositionVector + (vRight * (plr_fSpeed/_pTimer->TickQuantum));
 		bButtonAction = TRUE;
 		bMove = TRUE;
 	}*/
	
	if (_pInput->GetInputDevice(1)->id_aicControls[KID_A].ic_fValue == 1.0f)
	{
		en_plViewpoint.pl_OrientationAngle(1) += 2;
		bButtonAction = TRUE;
		bRotate = TRUE;
	}
	
	if (_pInput->GetInputDevice(1)->id_aicControls[KID_D].ic_fValue == 1.0f)
	{
		en_plViewpoint.pl_OrientationAngle(1) -= 2;
		bButtonAction = TRUE;
		bRotate = TRUE;
	}

	if (!bButtonAction && _pInput->IsLMouseButtonPressed() )
	{
		//m_bForward = FALSE;
		m_bKeyMove = FALSE;
		return FALSE;
	}

	if ( !bMove && bRotate )
	{
/*		if (!m_bSendStopMessage)
		{
			StopMove();
		}*/
		m_bOnlyRotate = TRUE;
		m_bKeyMove = FALSE;
		return TRUE;
		//m_vDesiredPosition = en_plPlacement.pl_PositionVector;
	}

	if (bRotate == TRUE && bMove == TRUE)
	{
		// tmStartTime : 1초마다 무브 메시지를 보냄.
		// 이동 + 로테이션 중이라면 0.2초마다 한번씩 무브 메시지를 보내도록 추가.
		if ((unsigned int(_pTimer->GetLerpedCurrentTick()*1000)) - tmStartTime > 200)
		{
			tmStartTime = 0;
		}
	}
	
	if (!m_bOnlyRotate)
	{
		m_bSendStopMessage = FALSE;
	}

	if (_pGameState->IsRestartGame() == TRUE)
	{
		UIMGR()->GetSystemMenu()->CancelRestart();
	}

	return TRUE;
}

BOOL IsEXRogue( int job )	// [2012/08/27 : Sora] EX로그 추가
{
	return (charEXRogue && (job == 7));
}

BOOL IsEXMage( int job)		//2013/01/10 jeil EX메이지 추가
{
	if(charEXRogue){		//2013/01/23 jeil EX메이지 체크 루틴 수정 
		return (charEXMage && (job == 8));
	}else{
		return (charEXMage && (job == 7));
	}
}

virtual   void ClearTargetInfo(CEntity* penEntity)
{
	if(penEntity != NULL)
	{
		ObjInfo* pInfo = ObjInfo::getSingleton();

		if(penEntity == pInfo->GetTargetEntity(eTARGET))
		{
			pInfo->TargetClear(eTARGET);
			_pNetwork->SendClickObject(-1);
		}
		if(penEntity == pInfo->GetTargetEntity(eTARGET_REAL))
		{
			pInfo->TargetClear(eTARGET_REAL);
		}

		if(GetPlayerWeapons()->m_penRayHitTmp == penEntity)
		{
			GetPlayerWeapons()->m_penRayHitTmp = NULL;
		}
		if(GetPlayerWeapons()->m_penReservedTarget == penEntity)
		{
			GetPlayerWeapons()->m_penReservedTarget	= NULL;
		}
	}
}

virtual BOOL IsSkilling()
{
	return m_bSkilling;
}

virtual BOOL IsPetActing()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if( pInfo->GetMyPetInfo()->pen_pEntity )
	{
		if( ((CPet*)pInfo->GetMyPetInfo()->pen_pEntity)->IsCommandAnimationPlaying() )
		{
			return TRUE;
		}
	}
	return FALSE;
}

virtual BOOL IsActionSitting()
{
	if(m_nActionSit == 1 || m_nActionSit == 3)//앉는 도중, 서는 도중.
	{
		return TRUE;
	}
	return FALSE;
}

virtual BOOL IsSitting()
{
	if(m_nActionSit ==2)//앉아있는 상태.
	{
		return TRUE;
	}
	else if(m_nActionSit == 1 || m_nActionSit == 3)//앉는 도중, 서는 도중.
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



void LostTarget()
{
	INFO()->TargetClear(eTARGET);

	if(GetPlayerWeapons()->m_penRayHitTmp != NULL) //0909 
	{			
		GetPlayerWeapons()->m_penRayHitTmp = NULL;
	}
	if(GetPlayerWeapons()->m_penRayHitClick != NULL) //0909 
	{			
		GetPlayerWeapons()->m_penRayHitClick = NULL;
	}
	if(GetPlayerWeapons()->m_penReservedTarget != NULL) //0909 
	{			
		GetPlayerWeapons()->m_penReservedTarget = NULL;
	}
}


virtual BOOL IsIdle()
{
	if( IsSitting() || IsSkilling() || IsMoving() || IsAlreadyDie() )
	{
		// 뭔가 하는 중이다.
		return FALSE;
	}
	return TRUE;
}

// is transform or transformming [7/29/2011 rumist]
virtual const BOOL IsTransform() const
{
	if( !m_bChanging && !m_bMobChange )
	{
		return FALSE;
	}

	return TRUE;
}

virtual void SetSpeedUp(BOOL bMe, INDEX index, BOOL bSpeedup)
{
	if(bMe)
	{
		m_bSpeedUp = bSpeedup;
		return;
	}
	CEntity* penCharacter;
	if (_pNetwork->ga_World.EntityExists(index,penCharacter)) //공격자
	{		
		if(penCharacter->IsCharacter())
		{
			((CCharacterBase*)penCharacter)->m_bSpeedUp = bSpeedup;
		}
	}
	return;
	
}

virtual BOOL IsRidePet( CEntity *pCharacter )
{
	if( pCharacter->IsPlayer() )
	{
		return m_bRide;
	}
	else if( pCharacter->IsCharacter() )
	{
		((CCharacter*)pCharacter)->m_bRide;
	}
	return FALSE;
}

virtual void RidingPet(CEntity *pCharacter, INDEX iPetType )
{
	SBYTE sbPetType	= iPetType & 0xFF;
	SBYTE sbPetColor =iPetType >>8;

	if( pCharacter->IsPlayer() && !m_bRide )
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();

		// 마운트 상태에서 애완동물의 배고픔이 0이라면 움직이지 못함.
		if( MY_PET_INFO()->fHungry <= 0 )
		{
			pUIManager->SetCSFlagOn( CSF_MOUNT_HUNGRY );
		}

		if (pUIManager->IsCSFlagOn(CSF_PETRIDING))
		{
			pUIManager->SetCSFlagOff(CSF_PETRIDING);
		}

		m_bForward = FALSE;
		m_bKeyMove = FALSE;
		CancelSkill(TRUE, g_iAutoAttack, FALSE);
		StopAttack();
		StopMove();

		mi_qvOffset			= pCharacter->GetModelInstance()->mi_qvOffset;
		mi_iParentBoneID	= pCharacter->GetModelInstance()->mi_iParentBoneID;
		//DeleteModelInstance(m_pRiderModelInstance);

		_pNetwork->MyCharacterInfo.bPetRide = TRUE;
		m_bRide		= TRUE;
		m_iRideType = sbPetType;

		FLOAT3D vPos;
		ANGLE3D vAngle;

		// m_pRiderModelInstance -> SkaStudio에서 애완동물 탈 것에 캐릭터를 Child로 붙여놓은 ModelInstance
		m_pRiderModelInstance = ObtainModelInstance_t( PetInfo().GetFileName(sbPetType, 2) );


		// [070824: Su-won] PET_COLOR_CHANGE
		if( sbPetColor > 0 )
		{
			CTString strTexFile =PetInfo().GetColoredTexFileName(sbPetType, 2, sbPetColor);
			m_pRiderModelInstance ->mi_aMeshInst[0].mi_tiTextures[0].ti_toTexture.SetData_t( strTexFile );
		}

		QVect tmp_qvOfset;
		INDEX tmp_iParentBoneID;
		tmp_qvOfset = m_pRiderModelInstance->mi_qvRideOffset;
		tmp_iParentBoneID = m_pRiderModelInstance->mi_iRideParentBoneID;
		
		m_pRideCharModelInstance = CreateModelInstance( "" );
		m_pRideCharModelInstance->Copy( *pCharacter->GetModelInstance() );
		m_pRiderModelInstance->AddChild( m_pRideCharModelInstance, tmp_iParentBoneID );

		pCharacter->GetModelInstance()->Copy(*m_pRiderModelInstance);

		INDEX ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();
		if( ctmi > 0 )
		{
			CModelInstance& child = pCharacter->GetModelInstance()->mi_cmiChildren[0];
			child.mi_qvOffset = tmp_qvOfset;
		}

		//pCharacter->GetModelInstance()->StopAllAnimations(0.0f);
		pCharacter->GetModelInstance()->NewClearState(0.0f);
		ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();		
		//NewClearState(0.2f);
		if( ctmi > 0 )
		{
			// 캐릭터가 Child로 붙여진 상태이기 때문에, 이펙트를 표시할 태그매니저도 Child의 것을 사용하도록 함.
			CModelInstance &cmi = pCharacter->GetModelInstance()->mi_cmiChildren[0];			
			_pNetwork->MyCharacterInfo.itemEffect.Refresh(&cmi.m_tmSkaTagManager, 1);
			_pNetwork->MyCharacterInfo.statusEffect.Refresh(&cmi.m_tmSkaTagManager, CStatusEffect::R_NONE);
			//cmi.NewClearState(0.2f);
		}		
		IdleAnim();
	}
	else if( pCharacter->IsCharacter() )
	{
		((CCharacter*)pCharacter)->RidingPet( pCharacter, iPetType );
	}
}

virtual void LeavingPet(CEntity *pCharacter )
{
	if( pCharacter->IsPlayer() && m_bRide && !m_bWildRide)
	{
		CUIManager* pUIManager = SE_Get_UIManagerPtr();

		// 배고파서 이동할 수 없는 상태인데, 애완동물에서 내릴려고 한 경우...
		if( pUIManager->IsCSFlagOn( CSF_MOUNT_HUNGRY ) )
		{
			pUIManager->SetCSFlagOff( CSF_MOUNT_HUNGRY );
		}

		if (pUIManager->IsCSFlagOn(CSF_PETRIDING))
		{
			pUIManager->SetCSFlagOff(CSF_PETRIDING);
		}
		
		m_bForward = FALSE;
		m_bKeyMove = FALSE;
		CancelSkill(TRUE, g_iAutoAttack, FALSE);
		StopAttack();
		StopMove();
		
		//m_pRiderModelInstance->Clear();
		// 마운트 상태에서 본래의 플레이어 캐릭터는 Child 모델 인스턴스이다.
		INDEX ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();
		if( ctmi > 0 )
		{
			CModelInstance &cmi = pCharacter->GetModelInstance()->mi_cmiChildren[0];						
 			m_pRideCharModelInstance->Copy(cmi);

			// 복사한 플레이어 캐릭터를 다시 현재 유저가 컨트롤 하는 모델인스턴스로 복사해줌.
			pCharacter->GetModelInstance()->m_tmSkaTagManager.ClearChild();
			pCharacter->GetModelInstance()->Copy(*m_pRideCharModelInstance);		
			DeleteModelInstance(m_pRiderModelInstance);
			m_pRideCharModelInstance = NULL;
			m_pRiderModelInstance = NULL;
			pCharacter->GetModelInstance()->mi_qvOffset			= mi_qvOffset;
			pCharacter->GetModelInstance()->mi_iParentBoneID	= mi_iParentBoneID;
			pCharacter->GetModelInstance()->NewClearState(0.0f);
			
			_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pCharacter->GetModelInstance()->m_tmSkaTagManager, 1);
			_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pCharacter->GetModelInstance()->m_tmSkaTagManager, CStatusEffect::R_NONE);
		}
		_pNetwork->MyCharacterInfo.bPetRide = FALSE;
		m_bRide		= FALSE;
		m_bWildRide = FALSE;
		m_iRideType = -1;

		SetPhysicsFlags(EPF_MODEL_WALKING);
		SetCollisionFlags(ECF_MODEL);
	}
	else if( pCharacter->IsCharacter() )
	{
		((CCharacter*)pCharacter)->LeavingPet( pCharacter );
	}
}

void InitBeastAnimation(void)
{
	CJobInfo* pInfo = CJobInfo::getSingleton();

	if (pInfo == NULL)
	{
		return;
	}

	idPlayerWhole_Animation[ANIM_RIDE_HORSE_WALK_1] = ska_GetIDFromStringTable(pInfo->GetAnimationName( _pNetwork->MyCharacterInfo.job, ANIM_RIDE_HORSE_WALK_1));
	idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1] = ska_GetIDFromStringTable(pInfo->GetAnimationName( _pNetwork->MyCharacterInfo.job, ANIM_RIDE_HORSE_IDLE_1));
	idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_2] = ska_GetIDFromStringTable(pInfo->GetAnimationName( _pNetwork->MyCharacterInfo.job, ANIM_RIDE_HORSE_IDLE_2));
	idPlayerWhole_Animation[ANIM_RIDE_HORSE_RUN_1]	= ska_GetIDFromStringTable(pInfo->GetAnimationName( _pNetwork->MyCharacterInfo.job, ANIM_RIDE_HORSE_RUN_1));
	idPlayerWhole_Animation[ANIM_RIDE_HORSE_PICK] = ska_GetIDFromStringTable(pInfo->GetAnimationName( _pNetwork->MyCharacterInfo.job, ANIM_RIDE_HORSE_PICK));
	idPlayerWhole_Animation[ANIM_RIDE_HORSE_DAMAGE] = ska_GetIDFromStringTable(pInfo->GetAnimationName( _pNetwork->MyCharacterInfo.job, ANIM_RIDE_HORSE_DAMAGE));
	idPlayerWhole_Animation[ANIM_RIDE_HORSE_DIE]	= ska_GetIDFromStringTable(pInfo->GetAnimationName( _pNetwork->MyCharacterInfo.job, ANIM_RIDE_HORSE_DIE));
	idPlayerWhole_Animation[ANIM_RIDE_HORSE_LEVELUP] = ska_GetIDFromStringTable(pInfo->GetAnimationName( _pNetwork->MyCharacterInfo.job, ANIM_RIDE_HORSE_LEVELUP));
}

void SetBeastAnimation(CEntity *pWildPet)
{
	if (pWildPet != NULL)
	{
		INDEX animationIdx = ANIM_RIDE_DEMONBAT_WALK_1;
        if( m_nWildPetType == 2 )
		{
			animationIdx = ANIM_RIDE_HORSE_WALK_1;
		}

		idPlayerWhole_Animation[animationIdx++] = ((CWildPet*)pWildPet)->iAnimWildPet_Walk;
		idPlayerWhole_Animation[animationIdx++] = ((CWildPet*)pWildPet)->iAnimWildPet_Idle1;
		idPlayerWhole_Animation[animationIdx++] = ((CWildPet*)pWildPet)->iAnimWildPet_Idle2;
		idPlayerWhole_Animation[animationIdx++]	= ((CWildPet*)pWildPet)->iAnimWildPet_Run;
		idPlayerWhole_Animation[animationIdx++] = ((CWildPet*)pWildPet)->iAnimWildPet_Idle1;
		idPlayerWhole_Animation[animationIdx++] = ((CWildPet*)pWildPet)->iAnimWildPet_Damage;
		idPlayerWhole_Animation[animationIdx++]	= ((CWildPet*)pWildPet)->iAnimWildPet_Death;
		idPlayerWhole_Animation[animationIdx] = ((CWildPet*)pWildPet)->iAnimWildPet_Idle1;
	}
}

virtual void RidingWildPet(CEntity *pCharacter, CEntity *pWildPet, CTString strFileName )
{
	if (pWildPet != NULL)
	{
		pWildPet->en_EntityUseType = CEntity::EU_DUMMY;
	}

	if( pCharacter->IsPlayer() && !m_bRide )
	{

		//((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(pWildPet);
		(static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->ClearTargetInfo(pWildPet);

		m_bForward = FALSE;
		m_bKeyMove = FALSE;
		CancelSkill(TRUE, g_iAutoAttack, FALSE);
		StopAttack();
		StopMove();

		m_nWildPetType = pWildPet->en_pWildPetTarget->m_nType;
		SetBeastAnimation(pWildPet);

		m_pRidePetModelInstance = ObtainModelInstance_t(strFileName);
		m_pRidePetModelInstance->Copy(*pWildPet->GetModelInstance());

		mi_qvOffset			= pCharacter->GetModelInstance()->mi_qvOffset;
		mi_iParentBoneID	= pCharacter->GetModelInstance()->mi_iParentBoneID;

		_pNetwork->MyCharacterInfo.bWildPetRide = TRUE;
		m_bRide		= TRUE;
		m_bWildRide = TRUE;

		QVect tmp_qvOfset;
		INDEX tmp_iParentBoneID;

 		tmp_qvOfset = m_pRidePetModelInstance->mi_qvRideOffset;
 		tmp_iParentBoneID = m_pRidePetModelInstance->mi_iRideParentBoneID;

		// 유저가 컨트롤 하는 플레이어 캐릭터를 애완 동물 탈 것에 플레이어 캐릭터를 Child로 붙인 모델로 교체함.
		m_pRideCharModelInstance = CreateModelInstance("");
		m_pRideCharModelInstance->Copy(*pCharacter->GetModelInstance());
		m_pRidePetModelInstance->AddChild( m_pRideCharModelInstance, tmp_iParentBoneID );
		pCharacter->GetModelInstance()->Copy(*m_pRidePetModelInstance);

		INDEX ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();
		if( ctmi > 0 )
		{
			CModelInstance& child = pCharacter->GetModelInstance()->mi_cmiChildren[0];
			child.mi_qvOffset = tmp_qvOfset;
		}

		//pCharacter->GetModelInstance()->StopAllAnimations(0.0f);
		pCharacter->GetModelInstance()->NewClearState(0.0f);
		ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();		
		//NewClearState(0.2f);
		if( ctmi > 0 )
		{
			// 캐릭터가 Child로 붙여진 상태이기 때문에, 이펙트를 표시할 태그매니저도 Child의 것을 사용하도록 함.
			CModelInstance &cmit = pCharacter->GetModelInstance()->mi_cmiChildren[0];			
			_pNetwork->MyCharacterInfo.itemEffect.Refresh(&cmit.m_tmSkaTagManager, 1);
			_pNetwork->MyCharacterInfo.statusEffect.Refresh(&cmit.m_tmSkaTagManager, CStatusEffect::R_NONE);
			//cmi.NewClearState(0.2f);
		}

		IdleAnim();
		//DeleteModelInstance( pWildPet->GetModelInstance() );
		//pWildPet->GetModelInstance()->Clear();
		pWildPet->Destroy();
	}
	else if( pCharacter->IsCharacter() )
	{
		//((CCharacter*)pCharacter)->RidingWildPet( pCharacter, pWildPet );
		(static_cast<CCharacter*>(pCharacter))->RidingWildPet( static_cast<CCharacter*>(pCharacter), pWildPet, strFileName );
	}
}

virtual void LeavingWildPet(CEntity *pCharacter )
{
	if( pCharacter->IsPlayer() && m_bRide && m_bWildRide)
	{
		// 배고파서 이동할 수 없는 상태인데, 애완동물에서 내릴려고 한 경우...
		if( SE_Get_UIManagerPtr()->IsCSFlagOn( CSF_MOUNT_HUNGRY ) )
		{
			SE_Get_UIManagerPtr()->SetCSFlagOff( CSF_MOUNT_HUNGRY );
		}
		
		m_bForward = FALSE;
		m_bKeyMove = FALSE;
		CancelSkill(TRUE, g_iAutoAttack, FALSE);
		StopAttack();
		StopMove();
		
		// 마운트 상태에서 본래의 플레이어 캐릭터는 Child 모델 인스턴스이다.
		INDEX ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();
		if( ctmi > 0 )
		{
// 			// 본래의 플레이어 캐릭터를 임시적으로 복사해놓구서,
 			CModelInstance &cmi = pCharacter->GetModelInstance()->mi_cmiChildren[0];						
 			m_pRideCharModelInstance->Copy(cmi);

			// 복사한 플레이어 캐릭터를 다시 현재 유저가 컨트롤 하는 모델인스턴스로 복사해줌.
			pCharacter->GetModelInstance()->m_tmSkaTagManager.ClearChild();
			pCharacter->GetModelInstance()->Copy(*m_pRideCharModelInstance);
			

			DeleteModelInstance(m_pRidePetModelInstance);
			m_pRideCharModelInstance = NULL;
			m_pRidePetModelInstance = NULL;
			pCharacter->GetModelInstance()->mi_qvOffset			= mi_qvOffset;
			pCharacter->GetModelInstance()->mi_iParentBoneID	= mi_iParentBoneID;
			pCharacter->GetModelInstance()->NewClearState(0.0f);
			
			_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pCharacter->GetModelInstance()->m_tmSkaTagManager, 1);
			_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pCharacter->GetModelInstance()->m_tmSkaTagManager, CStatusEffect::R_NONE);
		}
		_pNetwork->MyCharacterInfo.bWildPetRide = FALSE;
		m_bRide		= FALSE;
		m_bWildRide = FALSE;
		InitBeastAnimation();
		
		IdleAnim();

		SetPhysicsFlags(EPF_MODEL_WALKING);
		SetCollisionFlags(ECF_MODEL);
	}
	else if( pCharacter->IsCharacter() )
	{
		//((CCharacter*)pCharacter)->LeavingWildPet( pCharacter );
		(static_cast<CCharacter*>(pCharacter))->LeavingWildPet( pCharacter );
	}
}
// 애니메이션 타임 설정.
void SetBodyAnimTime(float fAnimTime)
{
	CPlayerAnimator *penAnimator = (CPlayerAnimator*)((CEntity*)m_penAnimator);
	penAnimator->m_fBodyAnimTime = fAnimTime;
};

bool IsSealPet(LONG* remain)
{
	if (_pNetwork->MyCharacterInfo.bPetRide == TRUE)
	{
		if (_pNetwork->MyWearItem[WEAR_PET].IsEmptyItem() == FALSE)
		{
			LONG lRemain = UIMGR()->GetPetInfo()->GetRemainSealed(_pNetwork->MyWearItem[WEAR_PET].Item_Plus);

			if (lRemain > 0)
			{
				if (remain != NULL)
				{
					(*remain) = lRemain;					
				}

				return true;
			}			
		}
	}

	return false;
}

// WILDPET MOVABLE
// 와일드 펫 마운트 상태에서 배고픔이 0일때 움직임 금지. [1/28/2011 rumist]
// 이는 InputKey에서 움직이지 않되 내부적으로는 send가 가기때문에 제거를 위해 추가.
const BOOL IsMovable()
{
	LONG lRemain;

	if (IsSealPet(&lRemain) == true)
	{
		GetPlayerWeapons()->m_penRayHitTmp = NULL;

		CTString str;
		str = _S(6283, "펫이 봉인되었기 때문에 이동할 수 없습니다. 펫을 해제하시기 바랍니다.");
		UIMGR()->GetChattingUI()->AddSysMessage(str, SYSMSG_ERROR);

		str.PrintF(_S(2508, "펫 봉인해제 시간이 %d시간 %d분 남았습니다."), lRemain / 3600, (lRemain % 3600) / 60);
		UIMGR()->GetChattingUI()->AddSysMessage(str, SYSMSG_ERROR);

		return FALSE;
	}

	if( !_pNetwork->MyCharacterInfo.bWildPetRide )
	{
		return TRUE;
	}

	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->m_nStm > 0)
	{
		return TRUE;
	}
	
	GetPlayerWeapons()->m_penRayHitTmp = NULL;
	return FALSE;
}

virtual void SetRepairLegit(SLONG chainex, CCharacterTarget *Ct)
{
	CEntity* penTargetEntity;
	
	if (_pNetwork->ga_World.EntityExists(Ct->m_nIdxClient,penTargetEntity))
	{
		penTargetEntity->SetSecondExtraFlagOn( ENF_EX2_LEGIT );
		Ct->SetLegitimate(TRUE);
		((CCharacter*)penTargetEntity)->m_nLegit = 1;		
		((CCharacter*)penTargetEntity)->m_nPkMode = 3;	//해골 아이콘 들어간후에 적용.
	}
}

virtual void SetLegit(UBYTE ubType, SLONG chaindex)
{
	if(_pNetwork->MyCharacterInfo.index == chaindex)//자기 자신이 가해자일때.
	{
		CTString strSysMessage;

		if(ubType == MSG_RIGHT_ATTACK_ADD)
		{		
			// 051203
			SetSecondExtraFlagOn( ENF_EX2_LEGIT );

			m_nLegit = 1;					
//			strSysMessage.PrintF(_S( 859, "당신은 정당방위 타겟이 되었습니다." ));	
		}
		else if(ubType == MSG_RIGHT_ATTACK_DELAY)
		{
			// 051203
			SetSecondExtraFlagOn( ENF_EX2_LEGIT );

			m_nLegit = 2;
//			strSysMessage.PrintF(_S( 860, "당신에 대한 정당방위 룰이 5초후 해제됩니다." ));	
		}
		else if(ubType == MSG_RIGHT_ATTACK_DEL)
		{		
			// 051203
			SetSecondExtraFlagOff( ENF_EX2_LEGIT );

			m_nLegit = 0;			
//			strSysMessage.PrintF(_S( 861, "당신에 대한 정당방위 룰이 해제되었습니다." ));	
		}			
		_pNetwork->ClientSystemMessage( strSysMessage);
		return;
	}

	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, chaindex);

	if (pObject != NULL)
	{
		//if (_pNetwork->ga_World.EntityExists(ct.m_nIdxClient,penTargetEntity)) 

		CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);
		CEntity* penTargetEntity = pObject->GetEntity();

		CTString strSysMessage;

		if(ubType == MSG_RIGHT_ATTACK_ADD)
		{
			// 051203
			penTargetEntity->SetSecondExtraFlagOn( ENF_EX2_LEGIT );
			_pNetwork->AddLegitList(chaindex); // 정당방위 캐릭터 인덱스를 따로 저장
			pTarget->SetLegitimate(TRUE);
			((CCharacter*)penTargetEntity)->m_nLegit = 1;		
			((CCharacter*)penTargetEntity)->m_nPkMode = 3;	//해골 아이콘 들어간후에 적용.
			strSysMessage.PrintF(_S( 833, "%s 에 대하여 정당방위가 성립되었습니다." ), pTarget->m_strName.c_str());	
		}
		else if(ubType == MSG_RIGHT_ATTACK_DELAY)
		{
			// 051203
			penTargetEntity->SetSecondExtraFlagOn( ENF_EX2_LEGIT );

			((CCharacter*)penTargetEntity)->m_nLegit = 2;
			strSysMessage.PrintF(_S( 834, "%s 에 대한 정당방위가 5초후 해제됩니다." ), pTarget->m_strName.c_str());	
		}
		else if(ubType == MSG_RIGHT_ATTACK_DEL)
		{
			// 051203
			penTargetEntity->SetSecondExtraFlagOff( ENF_EX2_LEGIT );
			_pNetwork->DelLegitList(chaindex); // 정당방위 캐릭터 인덱스를 지운다.
			pTarget->SetLegitimate(FALSE);
			((CCharacter*)penTargetEntity)->m_nLegit = 0;
			((CCharacter*)penTargetEntity)->m_nPkMode = 2;//정당방위 풀리고 PVP모드로 돌아감.
			strSysMessage.PrintF(_S( 835, "%s 에 대한 정당방위가 해제되었습니다." ), pTarget->m_strName.c_str());	
		}

		_pNetwork->ClientSystemMessage( strSysMessage);
	}	
}

// 051203 부분 제거해도 될듯.
BOOL IsLegitTarget(CEntity* penEntity)
{
	if(penEntity == NULL || !(penEntity->GetFlags()&ENF_ALIVE) || !(penEntity->IsCharacter()))
	{
		return FALSE;
	}

	// 051203
	if( penEntity->IsSecondExtraFlagOn( ENF_EX2_LEGIT ) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	/*
	if(((CCharacter*)penEntity)->m_nLegit ==2 || ((CCharacter*)penEntity)->m_nLegit ==1)
	{
		return TRUE;
	}	
	else
	{
		return FALSE;
	}
	*/
}


void DisplayChangeErr(SBYTE	err)
{
	CTString strSysMessage;
	switch(err)
	{
		case MSG_CHANGE_ERR_DEAD:
			strSysMessage.PrintF( _S( 1015, "죽은 상태에서는 변신할 수 없습니다." ) );						
			break;
		case MSG_CHANGE_ERR_SITDOWN:
			strSysMessage.PrintF( _S( 1016, "앉은 상태에서는 변신할 수 없습니다." ) );						
			break;
		case MSG_CHANGE_ERR_WRAP:
			strSysMessage.PrintF( _S( 1017, "이동장치 사용 중에는 변신할 수 없습니다." ) );						
			break;
		case MSG_CHANGE_ERR_SKILL:
		 	strSysMessage.PrintF( _S( 1018, "스킬 사용 중에는 변신할 수 없습니다." ) );						
			break;
		case MSG_CHANGE_ERR_EXCHANGE:
		 	strSysMessage.PrintF( _S( 1019, "교환 중에는 변신할 수 없습니다." ) );						
			break;
		case MSG_CHANGE_ERR_PARTY:
			strSysMessage.PrintF( _S( 1020, "파티 신청 중에는 변신할 수 없습니다." ) );						
			break;
		case MSG_CHANGE_ERR_PK:
			strSysMessage.PrintF( _S( 1021, "PK 중에는 변신할 수 없습니다." ) );						
			break;
		case MSG_CHANGE_ERR_LEVEL:
			strSysMessage.PrintF( _S( 1022, "변신할 수 있는 레벨이 아닙니다." ) );						
			break;
		case MSG_CHANGE_ERR_ZONE:
			strSysMessage.PrintF( _S( 1023, "현재의 지역에서는 사용할 수 없습니다." ) );						
			break;
		case MSG_CHANGE_ERR_BATTLE:
			strSysMessage.PrintF( _S( 1024, "전투 중에는 변신할 수 없습니다." ) );			
			break;
	}
	
	_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);
}

virtual BOOL CheckChangeCondition(INDEX level, INDEX zone)
{
	if(IsAlreadyDie())
	{
		DisplayChangeErr(MSG_CHANGE_ERR_DEAD);
		return FALSE;
	}
	else if(IsSitting())
	{
		DisplayChangeErr(MSG_CHANGE_ERR_SITDOWN);
		return FALSE;
	}
	else if( SE_Get_UIManagerPtr()->IsCSFlagOn( CSF_TELEPORT ) )
	{
		DisplayChangeErr(MSG_CHANGE_ERR_WRAP);
		return FALSE;
	}	
	else if(IsSkilling())
	{
		DisplayChangeErr(MSG_CHANGE_ERR_SKILL);
		return FALSE;
	}
	else if(IsPvp())
	{
		DisplayChangeErr(MSG_CHANGE_ERR_PK);
		return FALSE;
	}
	else if(IsAttacking())
	{
		DisplayChangeErr(MSG_CHANGE_ERR_BATTLE);
		return FALSE;
	}
	else if(level > _pNetwork->MyCharacterInfo.level)
	{
		DisplayChangeErr(MSG_CHANGE_ERR_LEVEL);
		return FALSE;
	}
	else if(zone != g_slZone)
	{
		DisplayChangeErr(MSG_CHANGE_ERR_ZONE);
		return FALSE;
	} 
	else if(m_bPlayAction)
	{
		CTString strSysMessage;
		strSysMessage.PrintF( _S( 1013, "사교동작 또는 액션 중에는 변신할 수 없습니다." ));
		_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);	
		return FALSE;
	}
	else if(m_bForward || m_bKeyMove )
	{
		CTString strSysMessage;
		strSysMessage.PrintF( _S( 1014, "이동 중에는 변신할 수 없습니다." ));
		_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);	
		return FALSE;

	}
	else if(m_bRide) // 애완동물을 타고 있을때
	{
		CTString strSysMessage;
		strSysMessage.PrintF( _S( 2584, "애완동물을 타고 있는 중에는 변신할 수 없습니다." ) );
		_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);
		return FALSE;
	}
	else if( _pNetwork->MyCharacterInfo.nEvocationIndex > 0 )
	{
		CTString strSysMessage;
		strSysMessage.PrintF( _S( 2585, "강신 중에는 변신할 수 없습니다." ) );
		_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR);
		return FALSE;
	}

	return TRUE;
}


virtual void AppearChaPolymoph(CEntity* pentity, INDEX mobindex)
{
	if (pentity && pentity->IsCharacter())
	{	
		((CCharacter*)pentity)->MonsterChange(mobindex, TRUE);
	}
}

// eons PetTransform
virtual void TransfromPet(CEntity* pentity, INDEX mobIndex, INDEX mobSize)
{
	if (pentity && pentity->IsPet())
	{ // 펫 변신
		if (mobIndex > 0)
		{
			FLOAT fmobSize = 1.0f;

			if (mobSize > 0)
			{
				fmobSize = mobSize * 0.1f;
			}

			((CPet*)pentity)->TransformPetModel(mobIndex, fmobSize);
		}
		else
		{
			((CPet*)pentity)->ReturnPetModel();
		}
	}
}

virtual void ReturnTransformPet(CEntity* pentity)
{
	if (pentity && pentity->IsPet())
	{
		((CPet*)pentity)->ReturnPetModel();
	}
}

// 소서러 변신체로...
virtual void AppearChaTransform( CEntity* pEntity, INDEX iTransformType )
{
	if (pEntity && pEntity->IsCharacter())
	{	
		int nType = TransformInfo().GetType(iTransformType);
		((CCharacter*)pEntity)->TransformSorcerer(nType, TRUE);
	}
}

void StopPlayer() 
{
	StopMove();
}

  BOOL IsVisibleCheckAll(CEntity *penEntity) 
  {
    ASSERT(penEntity!=NULL);
    // get ray source and target

    FLOAT3D vSource, vTarget;
    GetPositionCastRay(this, penEntity, vSource, vTarget);
	
	vSource(2) += 1.8f;
	vTarget(2) += 1.0f;
    // cast the ray
    CCastRay crRay(this, vSource, vTarget);
    crRay.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;   // check for model collision box
    crRay.cr_bHitTranslucentPortals = FALSE;
    en_pwoWorld->CastRay(crRay);

	if(crRay.cr_penHit==NULL)
	{
		return TRUE;
	}
	if(	crRay.cr_penHit!=NULL && 
		(crRay.cr_penHit->IsCharacter() || 
		crRay.cr_penHit->IsEnemy() || 
		crRay.cr_penHit->IsPet() ||
		crRay.cr_penHit->IsSlave() ||
		crRay.cr_penHit->GetFlags()&ENF_ALIVE|| 
		crRay.cr_penHit->GetFlags()&ENF_ITEM))
	{
		return TRUE;
	}
    // if the ray hits wanted entity
    return crRay.cr_penHit==penEntity;
  };
  
	virtual void PlAddAccessoryEffect(CEntity *penEntity, CCharacterTarget* pTarget)
	{// 서포터 이벤트로 인한 캐릭터 후광 이펙트
		CTString strEffectName = CTString("Item_support");

		if (pTarget == NULL)
		{
			// 자기 캐릭터
			CModelInstance *pMI = ((CCharacter*)penEntity)->GetCurrentPlayerModelInstance();
			_pNetwork->MyCharacterInfo.itemEffect.AddAccessoryEffect(strEffectName, &pMI->m_tmSkaTagManager);		
		}
		else
		{ 
			// 타 캐릭터			
			CModelInstance *pMI = ((CCharacter*)penEntity)->GetCurrentPlayerModelInstance();
			pTarget->cha_itemEffect.AddAccessoryEffect(strEffectName, &pMI->m_tmSkaTagManager);
		}
	}
	

void SetTarget( SLONG targetID, SBYTE targetType )
{
	CEntity	*penTargetEntity;
	CEntity *penPlEntity = CEntity::GetPlayerEntity( 0 );
	CPlayerEntity *penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	ObjectBase* pObject = NULL;
	
	int		i;
	for (i = 0; i < eOBJ_MAX; ++i)
	{
		pObject = ACTORMGR()->GetObject((eOBJ_TYPE)i, targetID);

		if (pObject != NULL) {
			break;
		}
	}

	if (pObject != NULL)
	{
		penTargetEntity = pObject->GetEntity();

		if (penTargetEntity == NULL)
		{
			return;
		}

		FLOAT MaxHealth		= ((CUnit*)penTargetEntity)->m_nMaxiHealth;
		FLOAT CurrentHealth	= ((CUnit*)penTargetEntity)->m_nCurrentHealth;
		int Level			= ((CUnit*)penTargetEntity)->m_nMobLevel;
		int mobIdx			= ((CUnit*)penTargetEntity)->m_nMobDBIndex;
		int	  nSyndicateType = 0; 
		int	  nSyndicateGrade = 0;

		int	  PkMode = -1;
		int   PkState;
		int   PkLegit;
		bool  bRet = false;

		if (penTargetEntity->en_pMobTarget != NULL)
		{
			nSyndicateType = penTargetEntity->en_pMobTarget->mob_iSyndicateType;
			nSyndicateGrade = penTargetEntity->en_pMobTarget->mob_iSyndicateGrade;
		}
		if(penTargetEntity->IsCharacter())
		{						
			Level			= 0;
			PkMode			= ((CCharacter*)((CEntity*) penTargetEntity))->m_nPkMode;
			PkState			= ((CCharacter*)((CEntity*) penTargetEntity))->m_nPkState;							
			PkLegit			= ((CCharacter*)((CEntity*) penTargetEntity))->m_nLegit;

			if (penTargetEntity->en_pCharacterTarget != NULL)
			{
				nSyndicateType  = penTargetEntity->en_pCharacterTarget->cha_iSyndicateType;
				nSyndicateGrade = penTargetEntity->en_pCharacterTarget->cha_iSyndicateGrade;
			}
		}						

		penPlayerEntity->SetTarget( penTargetEntity );
		INFO()->TargetClear(eTARGET);

		if(PkMode != -1)
		{
			bRet = penTargetEntity->SetTargetInfo(MaxHealth,CurrentHealth,FALSE,Level,PkMode,PkState,PkLegit);
		}
		else
		{
			bRet = penTargetEntity->SetTargetInfo(MaxHealth,CurrentHealth,FALSE,Level,0,0,0,mobIdx);
		}

		if (bRet == true)
		{
			penTargetEntity->SetTargetSyndicateInfo(nSyndicateType, nSyndicateGrade);
		}
	}
}

void RaidSceneOutputProcessing(SLONG slOutputType, INDEX iObjIndex, INDEX iData)
{
	switch(slOutputType)
	{
	case OUTPUT_PAUSE_AREA: // Camera act
		{
			CZoneInfo::ObjInZone* tmpData;

			if ((tmpData = CZoneInfo::getSingleton()->GetInZoneData(iObjIndex)) != NULL)
			{
				if (tmpData->bActive)
				{
					if (iData == 1 && !tmpData->bOnTrigger)	{ // Start
						// Send Trigger
						SendTriggerEvent(iObjIndex, EET_TRIGGER);
						tmpData->bOnTrigger = TRUE;
					}
					else if (iData == 0 && tmpData->bOnTrigger){ // End
						// Send Trigger
						SendTriggerEvent(iObjIndex, EET_TRIGGER);
						tmpData->bOnTrigger = FALSE;
					}
				}
			}
		}
		break;
	case OBJECT_TOUCH_FIELD: // OBJECT_TOUCH_FIELD와 동일
		{

		}
		break;
	case OUTPUT_TOGGLE_GATE: // Door open or close
	case OBJECT_GATE:
		{
			CZoneInfo::ObjInZone* tmpData;

			if ((tmpData = CZoneInfo::getSingleton()->GetInZoneData(iObjIndex)) != NULL)
			{
				if (tmpData->bActive)
				{
					CEntity *penTargetObject = NULL;

					if (_pNetwork->ga_World.EntityExists(iObjIndex, penTargetObject))
					{
						EDoorControll eDoor;

						if (iData == 1 && !tmpData->bOnTrigger)	{ // Open
							// Send Trigger
							eDoor.bOpen = TRUE;
							SendToTarget(penTargetObject, EET_DOORCONTROLL, this, &eDoor);
							tmpData->bOnTrigger = TRUE;
						}
						else if (iData == 0 && tmpData->bOnTrigger){ // Close
							// Send Trigger
							eDoor.bOpen = FALSE;
							SendToTarget(penTargetObject, EET_DOORCONTROLL, this, &eDoor);
							tmpData->bOnTrigger = FALSE;
						}
					}
				}
			}
		}
		break;
	case OUTPUT_MODEL_HOLDER_PADOX:
	case OBJECT_PADOX_MODEL:
		{
			CZoneInfo::ObjInZone* tmpData;

			if ((tmpData = CZoneInfo::getSingleton()->GetInZoneData(iObjIndex)) != NULL)
			{
				if (tmpData->bActive)
				{
					CEntity *penTargetObject = NULL;
					if (_pNetwork->ga_World.EntityExists(iObjIndex, penTargetObject))
					{
						EChangeAnim eChangeAnim;

						if (iData == 1)
						{
							eChangeAnim.AnimType = ACTION_APPEAR;
							penTargetObject->SendEvent(eChangeAnim);
						}
						else if (iData == 0)
						{
							eChangeAnim.strAnimation = CTString("Padocs_appear");
							eChangeAnim.AnimType = ACTION_DISAPPEAR;
							penTargetObject->SendEvent(eChangeAnim);
						}
					}
				}
			}
		}
		break;
	case OUTPUT_TOGGLE_PORTAL:
	case OBJECT_PORTAL:
	case OBJECT_ITEM: // 아이템도 같은 동작이다.
		{
			CZoneInfo::ObjInZone* tmpData;

			if ((tmpData = CZoneInfo::getSingleton()->GetInZoneData(iObjIndex)) != NULL)
			{
				if (tmpData->bActive)
				{
					CEntity *penTargetObject = NULL;

					if (_pNetwork->ga_World.EntityExists(iObjIndex, penTargetObject))
					{
						if (iData == 1)
						{
							SendTriggerEvent(iObjIndex, EET_ACTIVATE);
						}
						else if (iData == 0)
						{
							SendTriggerEvent(iObjIndex, EET_DEACTIVATE);
						}
					}
				}
			}
		}
		break;
	case OUTPUT_MODEL_NPC_SAY:
		{
			SE_Get_UIManagerPtr()->GetChattingUI()->ShowNPCTalk( iObjIndex, iData );
		}
		break;
	}
}

virtual void ReceiveRaidScene(CNetworkMessage *istr)
{
	SLONG subType;
	(*istr) >> subType;

	if (subType == MSG_EX_RAID_SCENE_SEND_OUTPUT)
	{
		SLONG OutputType;
		INDEX ObjIndex;
		INDEX iData;

		(*istr) >> OutputType >> ObjIndex >> iData;

		RaidSceneOutputProcessing(OutputType, ObjIndex, iData);
	}
	else if (subType == MSG_EX_RAID_SCENE_SEND_PADOX_SKILL)
	{
		CZoneInfo::ObjInZone* tmpData;

		if ((tmpData = CZoneInfo::getSingleton()->GetInZoneData(CTString("PadocsObject"))) != NULL)
		{
			if (tmpData->bActive)
			{
				CEntity *penTargetObject = NULL;
				if (_pNetwork->ga_World.EntityExists(tmpData->iObjectID, penTargetObject))
				{
					EChangeAnim eChangeAnim;
					eChangeAnim.strAnimation = "padocs_skill01";
					eChangeAnim.AnimType = ACTION_SKILL;
					penTargetObject->SendEvent(eChangeAnim);
				}
			}
		}
	}
	else if (subType == MSG_EX_RAID_SCENE_SEND_OBJECT_REFRESH)
	{
		SLONG slType;
		INDEX ObjIndex, iData;

		(*istr) >> slType >> ObjIndex >> iData;

		RaidSceneOutputProcessing(slType, ObjIndex, iData);
	}
	else if (subType == MSG_EX_RAID_SCENE_SEND_OBJECT_ALL_STATE)
	{
		INDEX ZoneNo;
		INDEX nCount;
		SLONG ObjectType;
		INDEX ObjIndex;
		INDEX iData;

		(*istr) >> ZoneNo >> nCount;

		if (nCount > 0)
		{
			for (int i=0; i<nCount; ++i)
			{
				(*istr) >> ObjectType >> ObjIndex >> iData;

				RaidSceneOutputProcessing(ObjectType, ObjIndex, iData);
			}
		}
	}
	else if (subType == MSG_EX_RAID_SCENE_SEND_COUNT_DOWN)
	{
		SLONG slSeconds;
		CTString strCount;

		(*istr) >> slSeconds;

		strCount.PrintF(_S(5101, "%d초 후 파독스의 거처의 문이 닫힙니다"), slSeconds);

		_UIAutoHelp->SetGMNotice(strCount);
	}
}

// ----------------------------------------------------------------------------
// Name : SendTriggerEvent()
// Desc : 월드에 존재하는 오브젝트에게 Event를 보낸다.(EET_TRIGGER를 기본으로 사용한다.)
// ----------------------------------------------------------------------------
virtual void SendTriggerEvent(INDEX iTarget, INDEX eetEventType/* EET_TRIGGER */)
{
	if (iTarget == 0)
	{
		SendToTarget(m_pen3rdPersonView, (EventEType)eetEventType, this);
		return;
	}

	CEntity *penTargetObject = NULL;
	if (_pNetwork->ga_World.EntityExists(iTarget, penTargetObject))
	{
		SendToTarget(penTargetObject, (EventEType)eetEventType, this);
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveTriggerEvent()
// Desc : 레이드 오브젝트 이벤트를 받아 오브젝트에 Trigger 이벤트를 보낸다.
// ----------------------------------------------------------------------------
virtual void ReceiveTriggerEvent(INDEX iIn, INDEX iOut, INDEX iContinued, CNetworkMessage *iStr)
{
	INDEX iInputObject = iIn; // 이벤트 시작 오브젝트
	INDEX iOutputObject = iOut; // 이벤트에 의한 활성화 오브젝트

	// Trigger 이벤트는 iOutputObject에 보낸다.
	CEntity *EntTriggerObjIn = NULL, *EntTriggerObjOut = NULL;

	if (iIn > 0)
	{
		EntTriggerObjIn = _pNetwork->ga_World.EntityFromID(iIn);
	}

	if (iOut > 0)
	{
		EntTriggerObjOut = _pNetwork->ga_World.EntityFromID(iOut);
	}

	if (EntTriggerObjIn != NULL && iContinued == 0)
	{ // 원인 제공자를 자신으로 한다.
		SendToTarget(EntTriggerObjIn, EET_TRIGGER, this);
	}

	if (EntTriggerObjOut != NULL)
	{ // 원인 제공자를 자신으로 한다.
		SendToTarget(EntTriggerObjOut, EET_TRIGGER, this);
	}

	// Trigger가 이미 작동된 오브젝트 정보 처리
	if (iStr != NULL)
	{ 
		INDEX iObjectCount = 0;
		INDEX iActiveObj = -1;
		INDEX i;
		
		(*iStr) >> iObjectCount;

		for (i=0; i<iObjectCount; ++i)
		{
			(*iStr) >> iActiveObj;
			CEntity* EntObj = _pNetwork->ga_World.EntityFromID(iActiveObj);

			if (EntObj != NULL)
			{// 여기서 Trigger 이벤트를 준다. (원인 제공자는 없다.)
				SendToTarget(EntObj, EET_TRIGGER, NULL);
			}
		}
	}
}
// ----------------------------------------------------------------------------
// Name : GetClickObject()
// Desc : 레이드 시스템의 클릭가능한 오브젝트의 타겟을 리턴한다.
//			CEntityPointer를 사용하였지만, 참조카운팅은 사용하지 않고, 직접 접근으로 사용한다.
//			class생성시 NULL초기화를 위해 CEntityPointer를 사용하였다.
//			CEntityPointer생성시 CEntity* ep_pen = NULL; 이 됨
// ----------------------------------------------------------------------------
virtual CEntity* GetClickObject(void)
{
	return m_penClickTarget.ep_pen;
}


procedures:
	/************************************************************
	*                        M  A  I  N                        *
	************************************************************/
	Main(EPlayerInit ePLInit)
	{
		_pNetwork->MyCharacterInfo.statusEffect.SetType(CStatusEffect::T_PLAYER);
		// remember start time
		time((time_t *)&m_iStartTime);
		
		SetFlagOn(ENF_CLIENTHANDLING);
		
		m_bInitializeOverNet = FALSE;
		m_ctUnreadMessages = 0;
		// initialize. [3/21/2011 rumist]
		m_pRiderModelInstance = NULL;
		m_pRidePetModelInstance = NULL;
		m_pRideCharModelInstance = NULL;
		m_nWildPetType = -1;

		SetFlags(GetFlags()|ENF_CROSSESLEVELS|ENF_NOTIFYLEVELCHANGE);

		//0427
		m_penAttackingEnemy		 = NULL;
		
		//0105
		if(m_bMdl)
		{
			InitAsEditorModel();
		}
		else
		{
			InitAsSkaEditorModel();	
		}
		//..
		SetPhysicsFlags(EPF_MOVABLE | EPF_PUSHABLE | EPF_RT_SYNCHRONIZED | EPF_ABSOLUTETRANSLATE);
		SetPhysicsFlags(GetPhysicsFlags() & ~EPF_TRANSLATEDBYGRAVITY);

		// set default model for physics etc
		CTString strDummy;
		//0105
		if(m_bMdl)
		{
			SetPlayerAppearance(GetModelObject(), NULL, strDummy, /*bPreview=*/FALSE);
		}
		else
		{
			if (en_pmiModelInstance==NULL) 
			{
				en_pmiModelInstance = CreateModelInstance("");
			}
			SetPlayerAppearanceSka(GetModelInstance(), NULL, strDummy, FALSE,en_pcCharacter.pc_iPlayerType);
		}

		//..   
		// set your real appearance if possible
		ValidateCharacter();
		//0105
		if(m_bMdl)
		{
			SetPlayerAppearance(&m_moRender, &en_pcCharacter, strDummy, /*bPreview=*/FALSE);
		}
		else
		{
			CreateAnimAndBoneIDs();
			//0531 kwon 삭제.
			//GetModelInstance()->StretchModel(FLOAT3D(0.4f,0.4f,0.4f));
			SetPlayerAppearanceSka(&m_miRender, &en_pcCharacter, strDummy, FALSE);	
		}
		//..
		ParseGender(strDummy);
		SetFlagOn(ENF_RENDERREFLECTION);
		
		// if unsuccessful
		/*
		if (GetModelObject()->GetData()==NULL) {
			ASSERT(FALSE);
			// never proceed with initialization - player cannot work
			return;
		}
		*/

		//0601 kwon 추가.
		//SetDefaultWearing();
		
		//const FLOAT fSize = 2.1f/1.85f;
		//GetModelObject()->StretchModel(FLOAT3D(fSize, fSize, fSize));
		ModelChangeNotify();

		// spawn weapons, do not send create and init messages through network
		m_iWeaponsID = ePLInit.ulWeaponsID;
		if (ePLInit.bCreate) {
			m_penWeapons = CreateEntity(GetPlacement(), CLASS_PLAYER_WEAPONS, ePLInit.ulWeaponsID,FALSE);
		} else {
			m_penWeapons = _pNetwork->ga_World.EntityFromID(m_iWeaponsID);
		}
		EWeaponsInit eInitWeapons;
		eInitWeapons.eidOwner = this;
		m_penWeapons->Initialize(eInitWeapons,FALSE);

		m_iAnimatorID = ePLInit.ulAnimatorID;
		// spawn animator, do not send create and init messages through network
		if (ePLInit.bCreate) {
			m_penAnimator = CreateEntity(GetPlacement(), CLASS_PLAYER_ANIMATOR,ePLInit.ulAnimatorID,FALSE);
		} else {
			m_penAnimator = _pNetwork->ga_World.EntityFromID(m_iAnimatorID);
		}
		EAnimatorInit eInitAnimator;
		eInitAnimator.eidPlayer = this;
		m_penAnimator->Initialize(eInitAnimator,FALSE);
		
		//0605 kwon 시작시 바로 마우스 조작 가능하도록...
		if(_cmiComm. IsNetworkOn())
		{			
			_pInput->m_bTcpIp =TRUE; 
		}

		if (_pNetwork->IsServer()) {
			// wait a bit to allow other entities to start
			wait(0.2f) { // this is 4 ticks, it has to be at least more than musicchanger for enemy counting
				on (EBegin) : { resume; }
				on (ETimer) : { stop; }
				on (EDisconnected) : { 
					Destroy(); 
					return;
				}
				otherwise() : { resume; }
			}
		}
		
		// appear
		SwitchToModel();
		m_ulFlags|=PLF_INITIALIZED;
		//0321
		Particles_RunningDust_Prepare(this);
		
		// set initial vars
		en_tmMaxHoldBreath = 60.0f;
		en_fDensity = 1000.0f;    // same density as water - to be able to dive freely
		SetHealth(10000.0f);//0629 캐릭터의 체력.
		ModelChangeNotify();
		
//안태훈 수정 시작	//(Open beta)(2004-12-14)
		if(GetModelInstance())
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			FLOATaabbox3D aabb;
			GetModelInstance()->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2) * GetModelInstance()->mi_vStretch(2), 0);
			GetModelInstance()->m_tmSkaTagManager.Register(&tag);
		}
//안태훈 수정 끝	//(Open beta)(2004-12-14)
		
		// set sound default parameters
		m_soMouth.Set3DParameters(50.0f, 10.0f, 1.0f, 1.0f);
		m_soFootL.Set3DParameters(20.0f, 2.0f, 1.0f, 1.0f);
		m_soFootR.Set3DParameters(20.0f, 2.0f, 1.0f, 1.0f);
		m_soBody.Set3DParameters(25.0f, 5.0f, 1.0f, 1.0f);
		m_soMessage.Set3DParameters(30.0f, 5.0f, 1.0f, 1.0f);
		//m_soSniperZoom.Set3DParameters(25.0f, 5.0f, 1.0f, 1.0f);
		
		// setup light source
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
		//SetupLightSource();
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
		
		// set light animation if available
		try {
			m_aoLightAnimation.SetData_t(CTFILENAME("Data\\Animations\\BasicEffects.ani"));
		} catch (char *strError) {
			WarningMessage(TRANS("Cannot load Data\\Animations\\BasicEffects.ani: %s"), strError);
		}
		PlayLightAnim(LIGHT_ANIM_NONE, 0);
		
		wait() {
			on (EBegin) : { call FirstInit(); }
			on (ERebirth) : { 
				if (_pNetwork->IsServer()) {
					EPlayerRebirth epr;
					epr.iRespawnInPlace = m_ulFlags & PLF_RESPAWNINPLACE;
					SendEvent(epr,TRUE);
					call Rebirth();
				}
				resume;
			}
			on (EPlayerRebirth epr) : { 
				if (!_pNetwork->IsServer()) {
					if (epr.iRespawnInPlace) {
						m_ulFlags |= PLF_RESPAWNINPLACE;
					} else {
						m_ulFlags &= ~PLF_RESPAWNINPLACE;
					}
					
					call Rebirth(); 
				}
				resume;
			}
			on (EDeath eDeath) : {
				if(_pNetwork->MyCharacterInfo.hp ==0)
				{
				if(_pNetwork->IsServer()) {
					EPlayerDeath ePLDeath;
					ePLDeath.eidInflictor = (CEntity*) eDeath.eLastDamage.penInflictor;
					ePLDeath.vDirection   = eDeath.eLastDamage.vDirection;
					ePLDeath.vHitPoint    = eDeath.eLastDamage.vHitPoint;
					ePLDeath.fAmount      = eDeath.eLastDamage.fAmount;
					ePLDeath.dmtType      = eDeath.eLastDamage.dmtType;
					SendEvent(ePLDeath,TRUE);
					call Death(ePLDeath);
				}
				}
				resume;
			}
			on (EPlayerDeath ePLDeath) : {    
				if(_pNetwork->MyCharacterInfo.hp==0)
				{
				//if(!_pNetwork->IsServer()) {
					call Death(ePLDeath);
				//}
				}
				resume;
				
			}
			on (EDamage eDamage) : { call Wounded(eDamage); }
			on (EPreLevelChange) : 
			{ 
				m_ulFlags&=~PLF_INITIALIZED; 
				m_ulFlags|=PLF_CHANGINGLEVEL;
				m_ulFlags &= ~PLF_LEVELSTARTED;
				m_ulFlags|=PLF_DONTRENDER;
				LevelChangePlayerClear();
				resume; 
			}
			on (EPostLevelChange) : 
			{
				if (GetSP()->sp_bSinglePlayer || (GetFlags()&ENF_ALIVE)) 
				{
					call WorldChange(); 
				} 
				else 
				{
					call WorldChangeDead(); 
				}
			}
			on (EAutoLogin eAutologin) :
			{
				if (g_bAutoLogin || g_bAutoRestart)
				{
//					SE_Get_UIManagerPtr()->GetServerSelect()->ConnectToServer(_cmiComm.cci_szAddr, _cmiComm.cci_iPort);
					if (GameDataManager* pGameData = GameDataManager::getSingleton())
					{
						if (CServerSelect* pServerSelect = pGameData->GetServerData())
						{
							pServerSelect->ConnectToServer(_cmiComm.cci_szAddr, _cmiComm.cci_iPort);
						}
					}
				}
				resume;
			}

			on (ECameraStart eStart) : 
			{
				m_bDontLerpView	= TRUE;
				m_penCamera		= eStart.penCamera;

// 강동민 수정 시작
				// 카메라 워킹을 시작할때, 플레이어가 계속 뛰어가는 문제 때문에 처리함...
				if( !_bLoginProcess )
				{	
					// 카메라 워킹 시작전에 선택된 타겟을 멈추지 않고 계속 공격하는 문제 처리  [12/27/2006 Theodoric]
					CancelSkill(TRUE, g_iAutoAttack, FALSE);
					StopAttack();
					StopMove();					
					
					if( SE_Get_UIManagerPtr()->IsVisibleUIs() )
					{
						SE_Get_UIManagerPtr()->ToggleVisibleUIs();
					}
				}
// 강동민 수정 끝
				if ((g_bAutoLogin || g_bAutoRestart) && STAGEMGR()->GetCurStage() == eSTAGE_LOGIN)
				{
					EAutoLogin eAutologin;
					SendEvent(eAutologin, TRUE);
				}

				EStopLerp eStop;
				eStop.bActive = FALSE;
				m_penCamera->SendEvent(eStop, TRUE);

				// stop player
				if (m_penActionMarker==NULL) 
				{
					SetDesiredTranslation(FLOAT3D(0.0f, 0.0f, 0.0f));
					SetDesiredRotation(ANGLE3D(0.0f, 0.0f, 0.0f));
				}
				// stop firing
				((CPlayerWeapons&)*m_penWeapons).SendEvent(EReleaseWeapon());
				resume;
			}
			on (ECameraStop eCameraStop) : 
			{
//강동민 수정 시작 클로즈 준비 작업	08.10
				extern BOOL _bWorldEditorApp;
				if(_bLoginProcess && !_bWorldEditorApp)
				{
					EStopLerp eStop;
					eStop.bActive = TRUE;
					m_penCamera->SendEvent(eStop, TRUE);
					if (m_penCamera==eCameraStop.penCamera)
					{
						// m_penCamera			= NULL;
					}
				}
				else
//강동민 수정 끝 클로즈 준비 작업		08.10
				{
					if (m_penCamera==eCameraStop.penCamera) 
					{
						if( !SE_Get_UIManagerPtr()->IsVisibleUIs() && !_bWorldEditorApp)	//에디터에서는 켜지지 않게...
						{
							SE_Get_UIManagerPtr()->ToggleVisibleUIs();
						}
						m_penCamera = NULL;
					}
				}
				resume;
			}

/*
			on (ECenterMessage eMsg) : 
			{
				m_strCenterMessage = eMsg.strMessage;
				m_tmCenterMessageEnd = _pTimer->CurrentTick()+eMsg.tmLength;
				if (eMsg.mssSound==MSS_INFO) {
					m_soMessage.Set3DParameters(25.0f, 5.0f, 1.0f, 1.0f);
					//PlaySound(m_soMessage, SOUND_INFO, SOF_3D|SOF_VOLUMETRIC|SOF_LOCAL);
				}
				resume;
			}
			on (EComputerMessage eMsg) : 
			{
				ReceiveComputerMessage(eMsg.fnmMessage, CMF_ANALYZE);
				resume;
			}
			*/
			on (EVoiceMessage eMsg) : 
			{
				SayVoiceMessage(eMsg.fnmMessage);
				resume;
			}
			on (EKilledEnemy eke) : {
				if (_pNetwork->IsServer()) {
					EPlayerKilledEnemy epke;
					epke.eidEnemy = eke.penEnemy;
					epke.iScore =  eke.iScore;
					epke.eCauseOfDeath = eke.eCauseOfDeath;
					epke.eKillType = eke.eKillType;
					SendEvent(epke,TRUE);
				}
				resume;
			}
			on (EPlayerKilledEnemy epke) : 
			{
				m_psLevelStats.ps_iKills += 1;
				m_psGameStats.ps_iKills += 1;
				resume;
			}		
			on (EWeaponChanged) : 
			{
				/*
				// make sure we discontinue zooming (even if not changing from sniper)
				((CPlayerWeapons&)*m_penWeapons).m_bSniping=FALSE;
				m_ulFlags&=~PLF_ISZOOMING;
				m_soSniperZoom.Stop(); 
				if(_pNetwork->IsPlayerLocal(this)) 
				{IFeel_StopEffect("SniperZoom");}
				*/
				resume;
			}
			/*
			// EEnd should not arrive here
			on (EEnd) : {
				ASSERT(FALSE);
				resume;
			}
			*/
			// if player is disconnected
			on (EDisconnected) : {
				// exit the loop
				stop;
			}
			// support for jumping using bouncers
			on (ETouch eTouch) : 
			{
				/*
				if( IsOfClass( eTouch.penOther, "Bouncer")) 
				{
					JumpFromBouncer(this, eTouch.penOther);
					// play jump sound
					SetDefaultMouthPitch();
					//PlaySound(m_soMouth, GenderSound(SOUND_JUMP), SOF_3D|SOF_VOLUMETRIC);
					if(_pNetwork->IsPlayerLocal(this)) {IFeel_PlayEffect("Jump");}
				}
				*/
				resume;
			}
			on (ESound) : { resume; }
		}
		
		// we get here if the player is disconnected from the server
		
		// if we have some keys
		if (m_ulKeys!=0) {
			// find first live player
			CPlayer *penNextPlayer = NULL;
			for(INDEX iPlayer=0; iPlayer<GetMaxPlayers(); iPlayer++) {
				CPlayer *pen = (CPlayer*)&*GetPlayerEntity(iPlayer);
				if (pen!=NULL && pen!=this && (pen->GetFlags()&ENF_ALIVE) && !(pen->GetFlags()&ENF_DELETED) ) {
					penNextPlayer = pen;
				}
			}
			
			// if any found
			if (penNextPlayer!=NULL) {
				// transfer keys to that player
				CPrintF(TRANS("%s leaving, all keys transfered to %s\n"), 
					(const char*)m_strName, (const char*)penNextPlayer->GetPlayerName());
				penNextPlayer->m_ulKeys |= m_ulKeys;
			}
		}
		
		// spawn teleport effect
		SpawnTeleport();
		
		// cease to exist
		m_penWeapons->Destroy(FALSE);
		m_penAnimator->Destroy(FALSE);
		if (m_penView!=NULL) {
			m_penView->Destroy(FALSE);
		}
		if (m_pen3rdPersonView!=NULL) {
			m_pen3rdPersonView->Destroy(FALSE);
		}
		Destroy(FALSE);
		return;
	};	

	//
	/************************************************************
	*                       WOUNDED                            *
	************************************************************/
	Wounded(EDamage eDamage) {
		return;
	};
	
	
	/************************************************************
	*                     WORLD CHANGE                         *
	************************************************************/
	WorldChange() 
	{
		m_dcEnemies.Clear();
		if(IsPvp())//월드이동시 pk모드 해제.
		{	
			_pNetwork->SetPvpMode();
		}
		m_bLockMove = FALSE;
		m_bReserveMove = FALSE;
		/*
		// if in single player
		if (GetSP()->sp_bSinglePlayer) 
		{
			// mark world as visited
			CTString strDummy("1");
			SaveStringVar(GetWorld()->wo_fnmFileName.NoExt()+".vis", strDummy);
		}
		*/
		// find music holder on new world
		FindMusicHolder();
		// store group name
		m_strGroup = _SwcWorldChange.strGroup;
		TeleportPlayer((WorldLinkType)_SwcWorldChange.iType, TPC_WORLDCHANGED);
		// setup light source
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
		//SetupLightSource();
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
		
		/*
		// make sure we discontinue zooming
		CPlayerWeapons *penWeapon = GetPlayerWeapons();
		penWeapon->m_fSniperFOVlast = penWeapon->m_fSniperFOV = penWeapon->m_fSniperMaxFOV;      
		penWeapon->m_bSniping=FALSE;
		m_ulFlags&=~PLF_ISZOOMING;
		*/
		
		// turn off possible chainsaw engine sound
		m_soWeaponAmbient.Stop();
		
		GetPlayerAnimator()->InitializeProperties();
		
		// update per-level stats
		UpdateLevelStats();
		m_ulFlags |= PLF_INITIALIZED;
		m_ulFlags &= ~PLF_CHANGINGLEVEL;
//강동민 수정 시작 다중 공격 작업	08.22
		ShowCursor(TRUE);		

		INFO()->TargetClear();

//강동민 수정 끝 다중 공격 작업		08.22

//안태훈 수정 시작	//(5th Closed beta)(0.2)
		extern ENGINE_API FLOAT g_fFramePerSecond;
		g_fFramePerSecond = FLT_MAX;
//안태훈 수정 끝	//(5th Closed beta)(0.2)		

		int count = CMobData::getsize();
		if(_pNetwork && _pNetwork->m_bSingleMode && count)
		{
			CEntity* penPlayer = CEntity::GetPlayerEntity(0);
			if(std::find(_pNetwork->ga_World.m_vectorTargetNPC.begin(), 
				_pNetwork->ga_World.m_vectorTargetNPC.end(), 
				penPlayer) 
				== _pNetwork->ga_World.m_vectorTargetNPC.end())
				{
					_pNetwork->ga_World.m_vectorTargetNPC.push_back(penPlayer);
				}
		}
		return;
	};
	
	WorldChangeDead() 
	{
		if(IsPvp())//월드이동시 pk모드 해제.
		{	
			_pNetwork->SetPvpMode();
		}
	
		// forbid respawning in-place when changing levels while dead
		m_ulFlags &= ~PLF_RESPAWNINPLACE;

		/*		
		// if in single player
		if (GetSP()->sp_bSinglePlayer) {
			// mark world as visited
			CTString strDummy("1");
			SaveStringVar(GetWorld()->wo_fnmFileName.NoExt()+".vis", strDummy);
		}
		*/
		// find music holder on new world
		FindMusicHolder();
		// store group name
		
		autocall Rebirth() EReturn;
		
		// setup light source
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
		//SetupLightSource();
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
		
		// update per-level stats
		UpdateLevelStats();
		m_ulFlags |= PLF_INITIALIZED;
		m_ulFlags &= ~PLF_CHANGINGLEVEL;		
		m_bLockMove = FALSE;
		m_bReserveMove = FALSE;
		return;
	}
	
	/************************************************************
	*                       D E A T H                          *
	************************************************************/
	
	Death(EPlayerDeath ePLDeath)
	{
		CPlayerSource* ppls = _pNetwork->GetPlayerSource(this);
		if (ppls != NULL) {
			for (INDEX iAction=0;iAction<PLS_MAXLASTACTIONS;iAction++) {
				ppls->pls_apaLastActions[iAction].Clear();
			}
		}

		// store last view
		m_iLastViewState = m_iViewState;
		
		// mark player as death
		SetFlags(GetFlags()&~ENF_ALIVE);
		// stop player
		SetDesiredTranslation(FLOAT3D(0.0f, 0.0f, 0.0f));
		SetDesiredRotation(ANGLE3D(0.0f, 0.0f, 0.0f));

		en_plViewpoint.pl_OrientationAngle = ANGLE3D(0,0,0);
	
		if( !_pNetwork->MyCharacterInfo.bExtension || m_bMobChange )
		{
			if( m_bRide )
			{
				if( m_bWildRide )
				{
					if( m_nWildPetType == 2 )
					{
						ChangeWholeAnim(idPlayerWhole_Animation[ANIM_RIDE_HORSE_DIE], AN_NORESTART, 1.0f, 0.2f, 1.0f);
					}
					else
					{
						ChangeWholeAnim(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_DIE], AN_NORESTART, 1.0f, 0.2f, 1.0f);
					}
				}
				// 레어 펫 추가로 짝수 타입은 말...060822 wooss
				else if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					ChangeWholeAnim(idPlayerWhole_Animation[ANIM_RIDE_HORSE_DIE], AN_NORESTART, 1.0f, 0.2f, 1.0f);
				}
				else
				{
					ChangeWholeAnim(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_DIE], AN_NORESTART, 1.0f, 0.2f, 1.0f);
				}
			}
			else
			{
				//0610 kwon 추가.
				ChangeWholeAnim(idPlayerWhole_Animation[ANIM_DIE], AN_NORESTART, 1.0f, 0.2f, 1.0f);
			}
		}
		else
		{
			if( m_bRide )
			{
				if( m_bWildRide )
				{
					if( m_nWildPetType == 2 )
					{
						ChangeWholeAnim(idPlayerWhole_Animation[ANIM_RIDE_HORSE_DIE], AN_NORESTART, 1.0f, 0.2f, 1.0f);
					}
					else
					{
						ChangeWholeAnim(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_DIE], AN_NORESTART, 1.0f, 0.2f, 1.0f);
					}
				}
				// 레어 펫 추가로 짝수 타입은 말...060822 wooss
				else if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					ChangeWholeAnim(idPlayerWhole_Animation[ANIM_RIDE_HORSE_DIE], AN_NORESTART, 1.0f, 0.2f, 1.0f);
				}
				else
				{
					ChangeWholeAnim(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_DIE], AN_NORESTART, 1.0f, 0.2f, 1.0f);
				}
			}
			else
			{
				ChangeWholeAnim(idPlayerWhole_Animation[ANIM_EXT_DIE], AN_NORESTART, 1.0f, 0.2f, 1.0f);
			}
		}

		// set physic flags
		SetPhysicsFlags(EPF_MODEL_CORPSE);
		SetCollisionFlags(ECF_CORPSE);
		
		// set density to float out of water
		en_fDensity = 400.0f;
		
		// play sound
		if (m_pstState==PST_DIVE) {
			SetDefaultMouthPitch();
			//PlaySound(m_soMouth, GenderSound(SOUND_DEATHWATER), SOF_3D);
			if(_pNetwork->IsPlayerLocal(this)) {IFeel_PlayEffect("DeathWater");}
		} else {
			SetDefaultMouthPitch();
			//PlaySound(m_soMouth, GenderSound(SOUND_DEATH), SOF_3D);
			if(_pNetwork->IsPlayerLocal(this)) {IFeel_PlayEffect("Death");}
		}
		/* //0610 kwon 삭제.
		// initialize death camera view
		ASSERT(m_penView == NULL);
		if (m_penView == NULL) {
			m_penView = CreateEntity(GetPlacement(), CLASS_PLAYER_VIEW,WLD_AUTO_ENTITY_ID,FALSE);
			EViewInit eInit;
			eInit.eidOwner = this;
			eInit.eidCamera = (CEntity*) NULL;
			eInit.vtView = VT_PLAYERDEATH;
			eInit.bDeathFixed = ePLDeath.dmtType==DMT_ABYSS;
			m_penView->Initialize(eInit,FALSE);
		}
		*/
		/*
		if (ShouldBlowUp()) {
			BlowUp();
		} else {
			// leave a stain beneath
			LeaveStain(TRUE);
		}
		*/
		
		SetFlagOn(ENF_CLIENTHANDLING);
		m_iMayRespawn = 0;
		// wait for anim of death
		wait (1.2f) {
			on (EBegin) : {
				// set new view status
				//m_iViewState = PVT_PLAYERAUTOVIEW; //0610 kwon 삭제.
				resume;
			}
			// when anim is finished
			on (ETimer) : {
				// allow respawning
				m_iMayRespawn = 1;
				resume;
			}
			// when damaged
			on (EDamage eDamage) : { 
				if (eDamage.dmtType==DMT_ABYSS) {
					if (m_penView!=NULL) {
						((CPlayerView*)&*m_penView)->m_bFixed = TRUE;
					}
				}
				// if should blow up now (and not already blown up)
				/*
				if (ShouldBlowUp()) {
					// do it
					BlowUp();
				}
				*/
				resume; 
			}
			on (EDeath) : { resume; }
			// if player pressed fire
			on (EEnd) : { 
				stop; 
			}
			// if autoaction is received
			on (EAutoAction eAutoAction) : {
				// if we are in coop
				if (GetSP()->sp_bCooperative && !GetSP()->sp_bSinglePlayer) {
					// if the marker is teleport marker
					if (((CEntity*)eAutoAction.eidFirstMarker)!=NULL && 
						((CPlayerActionMarker*)(CEntity*)eAutoAction.eidFirstMarker)->m_paaAction == PAA_TELEPORT) {
						// teleport there
						TeleportToAutoMarker((CPlayerActionMarker*)(CEntity*)eAutoAction.eidFirstMarker);
					}
				}
				// ignore the actions
				resume;
			}
			on (EDisconnected) : { pass; }
//			on (EReceiveScore) : { pass; }
			on (EKilledEnemy) : { pass; }
			on (EPlayerKilledEnemy) : { pass; }
			on (EPreLevelChange) : { pass; }
			on (EPostLevelChange) : { pass; }
			on (ERebirth) : { stop; }
			on (EPlayerRebirth epr) : { 
				SetFlagOff(ENF_CLIENTHANDLING); 
				if (epr.iRespawnInPlace) {
					m_ulFlags |= PLF_RESPAWNINPLACE;
				} else {
					m_ulFlags &= ~PLF_RESPAWNINPLACE;
				}
				call Rebirth(); stop; 
			}
			on (EReturn) : { stop; }
			otherwise() : { resume; }
		}

		return ERebirth();
	};
	
	TheEnd() 
	{
	/*
		// if not playing demo
		if (!_pNetwork->IsPlayingDemo()) 
		{		
			// record high score in single player only
			if (GetSP()->sp_bSinglePlayer) 
			{
				_pShell->Execute("gam_iRecordHighScore=0;");
			}
		}
		// if current difficulty is serious
		if (GetSP()->sp_gdGameDifficulty==CSessionProperties::GD_EXTREME) 
		{
			// activate the mental mode
			_pShell->Execute("sam_bMentalActivated=1;");
		}
		*/
		
		// stop firing when end
		((CPlayerWeapons&)*m_penWeapons).SendEvent(EReleaseWeapon());
		
		// mark player as dead
		SetFlags(GetFlags()&~ENF_ALIVE);
		// stop player
		SetDesiredTranslation(FLOAT3D(0.0f, 0.0f, 0.0f));
		SetDesiredRotation(ANGLE3D(0.0f, 0.0f, 0.0f));
		
		// look straight
		StartModelAnim(PLAYER_ANIM_STAND, 0);
		/*
		((CPlayerAnimator&)*m_penAnimator).BodyAnimationTemplate(
			BODY_ANIM_NORMALWALK, BODY_ANIM_COLT_STAND, BODY_ANIM_SHOTGUN_STAND, BODY_ANIM_MINIGUN_STAND, 
			AOF_LOOPING|AOF_NORESTART);
		*/
		
		en_plViewpoint.pl_OrientationAngle = ANGLE3D(0,0,0);
		
		// call computer
		m_bEndOfGame = TRUE;
		SetGameEnd();
		
		wait () {
			on (EBegin) : { resume; }
//			on (EReceiveScore) : { pass; }
//			on (ECenterMessage) : { pass; }
			otherwise() : { resume; }
		}
	};
	
	/************************************************************
	*                      R E B I R T H                       *
	************************************************************/
	FirstInit() 
	{	
		// restore last view
		m_iViewState = m_iLastViewState;
		
		// stop and kill camera
		if (m_penView != NULL) {
			((CPlayerView&)*m_penView).SendEvent(EEnd());
			m_penView = NULL;
		}
		
		FindMusicHolder();
		
		// update per-level stats
		UpdateLevelStats();
		
		// initialize player (from PlayerMarker)
		InitializePlayer(TRUE);

/*		
		// add statistics message
		ReceiveComputerMessage(CTFILENAME("Data\\Messages\\Statistics\\Statistics.txt"), CMF_READ);
		if (GetSettings()->ps_ulFlags&PSF_PREFER3RDPERSON && !_pNetwork->IsPlayingDemo()) {
			ChangePlayerView();
		}
*/		
		return;
	};
	
	Rebirth() 
	{	
		// restore last view
		m_iViewState = m_iLastViewState;
		// clear ammunition
		if (!(m_ulFlags&PLF_RESPAWNINPLACE)) {
			GetPlayerWeapons()->ClearWeapons();
		}
		
		// stop and kill camera
		if (m_penView != NULL) {
			((CPlayerView&)*m_penView).SendEvent(EEnd());
			m_penView = NULL;
		}
		
		// stop and kill flame
		CEntityPointer penFlame = GetChildOfClass("Flame");
		if (penFlame!=NULL)
		{
			// send the event to stop burning
			EStopFlaming esf;
			esf.m_bNow=TRUE;
			penFlame->SendEvent(esf);
		}
		
		if (m_penView != NULL) {
			((CPlayerView&)*m_penView).SendEvent(EEnd());
			m_penView = NULL;
		}
		
		FindMusicHolder();
		
		// initialize player (from PlayerMarker)
		InitializePlayer(FALSE);

		SetFlagOff(ENF_CLIENTHANDLING);

		GoTo( _pNetwork->MyCharacterInfo.x, _pNetwork->MyCharacterInfo.h + 0.5f, _pNetwork->MyCharacterInfo.z,
						_pNetwork->MyCharacterInfo.r );
		
		return EReturn();
	};	
	
	// auto action - go to current marker
	AutoGoToMarker(EVoid)
	{
		ULONG ulFlags = AOF_LOOPING|AOF_NORESTART;
		
		INDEX iAnim = GetModelObject()->GetAnim();
		if( iAnim!=PLAYER_ANIM_STAND)
		{
			ulFlags |= AOF_SMOOTHCHANGE;
		}
		
		CPlayerAnimator &plan = (CPlayerAnimator&)*m_penAnimator;
		plan.m_bAttacking = FALSE;
		plan.BodyWalkAnimation();
		if (m_fAutoSpeed>plr_fSpeed/2) {
			StartModelAnim(PLAYER_ANIM_RUN, ulFlags);
		} else {
			StartModelAnim(PLAYER_ANIM_NORMALWALK, ulFlags);
		}
		
		// while not at marker
		while (
			(m_penActionMarker->GetPlacement().pl_PositionVector-
			GetPlacement().pl_PositionVector).Length()>1.0f) {
			// wait a bit
			autowait(_pTimer->TickQuantum);
		}
		
		// return to auto-action loop
		return EReturn();
	}
	
	// auto action - go to current marker and stop there
	AutoGoToMarkerAndStop(EVoid)
	{
		ULONG ulFlags = AOF_LOOPING|AOF_NORESTART;
		
		INDEX iAnim = GetModelObject()->GetAnim();
		if( iAnim!=PLAYER_ANIM_STAND)
		{
			ulFlags |= AOF_SMOOTHCHANGE;
		}
		
		CPlayerAnimator &plan = (CPlayerAnimator&)*m_penAnimator;
		plan.BodyWalkAnimation();
		if (m_fAutoSpeed>plr_fSpeed/2) {
			StartModelAnim(PLAYER_ANIM_RUN, ulFlags);
		} else {
			StartModelAnim(PLAYER_ANIM_NORMALWALK, ulFlags);
		}
		
		// while not at marker
		while (
			(m_penActionMarker->GetPlacement().pl_PositionVector-
			GetPlacement().pl_PositionVector).Length()>m_fAutoSpeed*_pTimer->TickQuantum*2.00f) {
			// wait a bit
			autowait(_pTimer->TickQuantum);
		}
		// disable auto speed
		m_fAutoSpeed = 0.0f;
		
		CPlayerAnimator &plan = (CPlayerAnimator&)*m_penAnimator;
		plan.BodyStillAnimation();
		StartModelAnim(PLAYER_ANIM_STAND, AOF_LOOPING|AOF_NORESTART);
		
		// stop moving
		ForceFullStop();
		
		// return to auto-action loop
		return EReturn();
	}
	
	// auto action - use an item
	AutoUseItem(EVoid)
	{
		
		// start pulling the item
		CPlayerAnimator &plan = (CPlayerAnimator&)*m_penAnimator;
		plan.BodyPullItemAnimation();
		//StartModelAnim(PLAYER_ANIM_STATUE_PULL, 0);
		
		autowait(0.2f);
		
		// item appears
		CPlayerActionMarker *ppam = GetActionMarker();
		/*
		if (IsOfClass(ppam->m_penItem, "KeyItem")) 
		{
			CModelObject &moItem = ppam->m_penItem->GetModelObject()->GetAttachmentModel(0)->amo_moModelObject;
			GetPlayerAnimator()->SetItem(&moItem);
		} else {
			CPrintF("error: AutoPickItem - check 'Item to pick'!\n");
		}
		*/
		
		autowait(2.20f-0.2f);
		
		// the item is in place
		CPlayerAnimator &plan = (CPlayerAnimator&)*m_penAnimator;
		plan.BodyRemoveItem();
		// if marker points to a trigger
		if (GetActionMarker()->m_penTrigger!=NULL) {
			// trigger it
			SendToTarget(GetActionMarker()->m_penTrigger, EET_TRIGGER, this);
		}
		
		// fake that player has passed through the door controller
		if (GetActionMarker()->m_penDoorController!=NULL) {
			EPass ePass;
			ePass.penOther = this;
			GetActionMarker()->m_penDoorController->SendEvent(ePass);
		}
		
		autowait(3.25f-2.20f);
		
		CPlayerAnimator &plan = (CPlayerAnimator&)*m_penAnimator;
		plan.BodyRemoveItem();
		
		// return to auto-action loop
		return EReturn();
	}
	
	// auto action - pick an item
	AutoPickItem(EVoid)
	{
		// start pulling the item
		CPlayerAnimator &plan = (CPlayerAnimator&)*m_penAnimator;
		plan.BodyPickItemAnimation();
		StartModelAnim(PLAYER_ANIM_KEYLIFT, 0);
		
		autowait(1.2f);
		
		// if marker points to a trigger
		if (GetActionMarker()->m_penTrigger!=NULL) {
			// trigger it
			SendToTarget(GetActionMarker()->m_penTrigger, EET_TRIGGER, this);
		}
		
		/*
		// item appears
		CPlayerActionMarker *ppam = GetActionMarker();
		if (IsOfClass(ppam->m_penItem, "KeyItem")) {
			CModelObject &moItem = ppam->m_penItem->GetModelObject()->GetAttachmentModel(0)->amo_moModelObject;
			GetPlayerAnimator()->SetItem(&moItem);
			EPass ePass;
			ePass.penOther = this;
			ppam->m_penItem->SendEvent(ePass);
		}
		*/
		
		autowait(3.6f-1.2f+GetActionMarker()->m_tmWait);
		
		CPlayerAnimator &plan = (CPlayerAnimator&)*m_penAnimator;
		plan.BodyRemoveItem();
		
		// return to auto-action loop
		return EReturn();
	}
	
	AutoFallDown(EVoid)
	{
		StartModelAnim(PLAYER_ANIM_BRIDGEFALLPOSE, 0);
		CModelObject &moBody = GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject;
		moBody.PlayAnim(BODY_ANIM_BRIDGEFALLPOSE, 0);
		
		autowait(GetActionMarker()->m_tmWait);
		
		// return to auto-action loop
		return EReturn();
	}

	AutoFallToAbys(EVoid)
	{
		StartModelAnim(PLAYER_ANIM_ABYSSFALL, AOF_LOOPING);
		CModelObject &moBody = GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject;
		moBody.PlayAnim(BODY_ANIM_ABYSSFALL, AOF_LOOPING);
		
		autowait(GetActionMarker()->m_tmWait);
		
		// return to auto-action loop
		return EReturn();
	}
	
	// auto action - look around
	AutoLookAround(EVoid)
	{
		StartModelAnim(PLAYER_ANIM_BACKPEDAL, 0);
		m_vAutoSpeed = FLOAT3D(0,0,plr_fSpeed/4/0.75f);
		CModelObject &moBody = GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject;
		moBody.PlayAnim(BODY_ANIM_NORMALWALK, 0);
		
		autowait(GetModelObject()->GetCurrentAnimLength()/2);
		
		m_vAutoSpeed = FLOAT3D(0,0,0);
		
		// start looking around
		StartModelAnim(PLAYER_ANIM_STAND, 0);
		CModelObject &moBody = GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject;
		moBody.PlayAnim(BODY_ANIM_LOOKAROUND, 0);
		CPlayerAnimator &plan = (CPlayerAnimator&)*m_penAnimator;
		
		// wait given time
		autowait(moBody.GetCurrentAnimLength()+0.1f);
		
		// return to auto-action loop
		return EReturn();
	}
	
	AutoTeleport(EVoid)
	{
		// teleport there
		TeleportToAutoMarker(GetActionMarker());
		
		// return to auto-action loop
		return EReturn();
	}	
	
	// perform player auto actions
	DoAutoActions(EVoid)
	{
		// don't look up/down
		en_plViewpoint.pl_OrientationAngle = ANGLE3D(0,0,0);
		// disable playeranimator animating
		CPlayerAnimator &plan = (CPlayerAnimator&)*m_penAnimator;
		plan.m_bDisableAnimating = TRUE;
		
		// while there is some marker
		while (m_penActionMarker!=NULL && IsOfClass(m_penActionMarker, "PlayerActionMarker")) {
			
			// if should wait
			if (GetActionMarker()->m_paaAction==PAA_WAIT) {
				// play still anim
				CModelObject &moBody = GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject;
				moBody.PlayAnim(BODY_ANIM_WAIT, AOF_NORESTART|AOF_LOOPING);
				// wait given time
				autowait(GetActionMarker()->m_tmWait);
			} else if (GetActionMarker()->m_paaAction==PAA_STOPANDWAIT) {
				// play still anim
				StartModelAnim(PLAYER_ANIM_STAND, 0);
				CModelObject &moBody = GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject;
				moBody.PlayAnim(BODY_ANIM_WAIT, AOF_NORESTART|AOF_LOOPING);
				// wait given time
				autowait(GetActionMarker()->m_tmWait);
				
				// if should teleport here
			} 
			/*
			else if (GetActionMarker()->m_paaAction==PAA_APPEARING) {
				autocall AutoAppear() EReturn;
			} 			
			else if (GetActionMarker()->m_paaAction==PAA_TRAVELING_IN_BEAM) {
				autocall TravellingInBeam() EReturn;
			} 
			*/
			else if (GetActionMarker()->m_paaAction==PAA_INTROSE_SELECT_WEAPON) {
				// order playerweapons to select weapon
				ESelectWeapon eSelect;
				eSelect.iWeapon = 1;
				((CPlayerWeapons&)*m_penWeapons).SendEvent(eSelect);
			} 
			/*
			else if (GetActionMarker()->m_paaAction==PAA_LOGO_FIRE_INTROSE) {
				autocall LogoFireMinigun() EReturn;
			} 
			else if (GetActionMarker()->m_paaAction==PAA_LOGO_FIRE_MINIGUN) {
				autocall LogoFireMinigun() EReturn;
				// if should appear here
			}
			*/
			else if (GetActionMarker()->m_paaAction==PAA_TELEPORT) {
				autocall AutoTeleport() EReturn;
				
				// if should wait for trigger
			} else if (GetActionMarker()->m_paaAction==PAA_WAITFOREVER) {
				// wait forever
				wait() {
					on (EBegin) : { resume; }
					otherwise() : { pass; }
				}
				// if should store weapon
			} 
			/*
			else if (GetActionMarker()->m_paaAction==PAA_STOREWEAPON) {
				autocall AutoStoreWeapon() EReturn;
				
				// if should draw weapon
			}
			*/
			else if (GetActionMarker()->m_paaAction==PAA_DRAWWEAPON) {
				// order playerweapons to select best weapon
				ESelectWeapon eSelect;
				eSelect.iWeapon = -4;
				((CPlayerWeapons&)*m_penWeapons).SendEvent(eSelect);
				
				// if should wait
			} else if (GetActionMarker()->m_paaAction==PAA_LOOKAROUND) {
				autocall AutoLookAround() EReturn;
				
				// if should use item
			} else if (GetActionMarker()->m_paaAction==PAA_USEITEM) {
				// use it
				autocall AutoUseItem() EReturn;
				
				// if should pick item
			} else if (GetActionMarker()->m_paaAction==PAA_PICKITEM) {
				// pick it
				autocall AutoPickItem() EReturn;
				
				// if falling from bridge
			} else if (GetActionMarker()->m_paaAction==PAA_FALLDOWN) {
				// fall
				autocall AutoFallDown() EReturn;
				
				// if releasing player
			} else if (GetActionMarker()->m_paaAction==PAA_RELEASEPLAYER) {
				if (m_penCamera!=NULL) {
					((CCamera*)&*m_penCamera)->m_bStopMoving=TRUE;
				}
				m_penCamera = NULL;
				// if currently not having any weapon in hand
				if (GetPlayerWeapons()->m_iCurrentWeapon == WEAPON_NONE) {
					// order playerweapons to select best weapon
					ESelectWeapon eSelect;
					eSelect.iWeapon = -4;
					((CPlayerWeapons&)*m_penWeapons).SendEvent(eSelect);
				}
				// sync weapon, just in case
				//m_ulFlags |= PLF_SYNCWEAPON;
				m_tmSpiritStart = 0;
				
				// if start computer
			} else if (GetActionMarker()->m_paaAction==PAA_STARTCOMPUTER) {
				// mark that
				if (_pNetwork->IsPlayerLocal(this) && GetSP()->sp_bSinglePlayer) {
					cmp_ppenPlayer = this;
					cmp_bInitialStart = TRUE;
				}
				
				// if start introscroll
			} 
			/*
			else if (GetActionMarker()->m_paaAction==PAA_STARTINTROSCROLL) {
				_pShell->Execute("sam_iStartCredits=1;");
				
				// if start credits
			} 
			else if (GetActionMarker()->m_paaAction==PAA_STARTCREDITS) {
				_pShell->Execute("sam_iStartCredits=2;");
				
				// if stop scroller
			} else if (GetActionMarker()->m_paaAction==PAA_STOPSCROLLER) {
				_pShell->Execute("sam_iStartCredits=-1;");
				
				// if should run to the marker
			}
			*/
			else if (GetActionMarker()->m_paaAction==PAA_RUN) {
				// go to it
				m_fAutoSpeed = plr_fSpeed*GetActionMarker()->m_fSpeed;
				autocall AutoGoToMarker() EReturn;
				
				// if should run to the marker and stop exactly there
			} else if (GetActionMarker()->m_paaAction==PAA_RUNANDSTOP) {
				// go to it
				m_fAutoSpeed = plr_fSpeed*GetActionMarker()->m_fSpeed;                                             
				autocall AutoGoToMarkerAndStop() EReturn;
				
				// if should record end-of-level stats
			} else if (GetActionMarker()->m_paaAction==PAA_RECORDSTATS) {
				
				if (GetSP()->sp_bSinglePlayer || GetSP()->sp_bPlayEntireGame) {
					// remeber estimated time
					m_tmEstTime = GetActionMarker()->m_tmWait;
					
				} else {
					SetGameEnd();
				}
			// if should show statistics to the player
			} else if (GetActionMarker()->m_paaAction==PAA_SHOWSTATS) {
				// call computer
				if (cmp_ppenPlayer==NULL && _pNetwork->IsPlayerLocal(this) && GetSP()->sp_bSinglePlayer) {
					m_bEndOfLevel = TRUE;
					cmp_ppenPlayer = this;
					m_ulFlags|=PLF_DONTRENDER;
					while(m_bEndOfLevel) {
						wait(_pTimer->TickQuantum) {
							on (ETimer) : { stop; }
//							on (EReceiveScore) : { pass; }
//							on (ECenterMessage) : { pass; }
							on (EPreLevelChange) : { pass; }
							on (EPostLevelChange) : { 
								if (_pNetwork->IsServer()) {
									//m_ulFlags&=~PLF_DONTRENDER;
									m_bEndOfLevel = FALSE;
								}
								pass; 
							}              
							otherwise() : { resume; }
						}
					}
					m_ulFlags&=!PLF_DONTRENDER;
				}
			// if end of entire game
			} else if (GetActionMarker()->m_paaAction==PAA_ENDOFGAME) {
				// record stats
				jump TheEnd();
			} else if (GetActionMarker()->m_paaAction==PAA_NOGRAVITY) {
				SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));
				if( GetActionMarker()->GetParent() != NULL)
				{
					SetParent(GetActionMarker()->GetParent());
				}
			} else if (GetActionMarker()->m_paaAction==PAA_TURNONGRAVITY) {
				SetPhysicsFlags(GetPhysicsFlags()|EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY);
				SetParent(NULL);
			}
			else if (TRUE) {
				ASSERT(FALSE);
			}
			
			// if marker points to a trigger
			if (GetActionMarker()->m_penTrigger!=NULL &&
				GetActionMarker()->m_paaAction!=PAA_PICKITEM) {
				// trigger it
				SendToTarget(GetActionMarker()->m_penTrigger, EET_TRIGGER, this);
			}
			
			// get next marker
			m_penActionMarker = GetActionMarker()->m_penTarget;
		}
		
		// disable auto speed
		m_fAutoSpeed = 0.0f;
		
		// must clear marker, in case it was invalid
		m_penActionMarker = NULL;
		
		// enable playeranimator animating
		CPlayerAnimator &plan = (CPlayerAnimator&)*m_penAnimator;
		plan.m_bDisableAnimating = FALSE;
		
		// return to main loop
		return EVoid();
	}
	
};
