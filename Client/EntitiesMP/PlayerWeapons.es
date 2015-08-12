402
%{
#include "StdH.h"
#include "GameMP/SEColors.h"
  
#include <Engine/Build.h>

#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMouseCursor.h>
// [KH_070420] 심플 팝업 관련 추가
#include <Engine/Interface/UISimplePop.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Network/Web.h>
#include <Engine/Help/Util_Help.h>

#include "EntitiesMP/Player.h"
#include "EntitiesMP/Bullet.h"
#include "EntitiesMP/Switch.h"
#include "EntitiesMP/PlayerView.h"
#include "EntitiesMP/PlayerAnimator.h"
#include "EntitiesMP/MovingBrush.h"
#include "EntitiesMP/MessageHolder.h"
#include "EntitiesMP/EnemyBase.h"
//#include "EntitiesMP/SeriousBomb.h"
//#include "EntitiesMP/Elemental.h"
//#include "EntitiesMP/Beast.h"
//#include "EntitiesMP/Gizmo.h"
//#include "EntitiesMP/LarvaOffspring.h"

//#include "EntitiesMP/ExotechLarva.h"
//#include "EntitiesMP/AirElemental.h"
//#include "EntitiesMP/BigHead.h"
//#include "EntitiesMP/CrateRider.h"
//#include "EntitiesMP/Santa.h"
//#include "EntitiesMP/Summoner.h"
//#include "EntitiesMP/Boneman.h"
//#include "EntitiesMP/Woman.h"
//#include "EntitiesMP/Walker.h"
//#include "EntitiesMP/CannonRotating.h"
//#include "EntitiesMP/CannonStatic.h"

#include "Models/Weapons/Knife/Knife.h"
//#include "Models/Weapons/Knife/KnifeItem.h"
//#include "Models/Weapons/Colt/Colt.h"
//#include "Models/Weapons/Colt/ColtMain.h"
//#include "Models/Weapons/SingleShotgun/SingleShotgun.h"
//#include "Models/Weapons/SingleShotgun/Barrels.h"
//#include "Models/Weapons/DoubleShotgun/DoubleShotgun.h"
//#include "Models/Weapons/DoubleShotgun/Dshotgunbarrels.h"
//#include "Models/Weapons/DoubleShotgun/HandWithAmmo.h"
//#include "Models/Weapons/TommyGun/TommyGun.h"
//#include "Models/Weapons/TommyGun/Body.h"
//#include "Models/Weapons/MiniGun/MiniGun.h"
//#include "Models/Weapons/MiniGun/Body.h"
//#include "Models/Weapons/GrenadeLauncher/GrenadeLauncher.h"
//#include "Models/Weapons/RocketLauncher/RocketLauncher.h"
//#include "Models/Weapons/Laser/Laser.h"
//#include "Models/Weapons/Laser/Barrel.h"
//#include "Models/Weapons/Cannon/Cannon.h"
//#include "Models/Weapons/Cannon/Body.h"
// Mission Pack weapons
//#include "ModelsMP/Weapons/Sniper/Sniper.h"
//#include "ModelsMP/Weapons/Sniper/Body.h"
//#include "ModelsMP/Weapons/Flamer/Flamer.h"
//#include "ModelsMP/Weapons/Flamer/Body.h"
//#include "ModelsMP/Weapons/Flamer/FuelReservoir.h"
//#include "ModelsMP/Weapons/Flamer/Flame.h"  
#include "ModelsMP/Weapons/Chainsaw/Chainsaw.h"
#include "ModelsMP/Weapons/Chainsaw/ChainSawForPlayer.h"
#include "ModelsMP/Weapons/Chainsaw/Body.h"
#include "ModelsMP/Weapons/Chainsaw/Blade.h"
#include "ModelsMP/Weapons/Chainsaw/Teeth.h"

// Mission Pack player body instead of the old one
#include "ModelsMP/Player/SeriousSam/Body.h"
#include "ModelsMP/Player/SeriousSam/Player.h"

ENGINE_API extern cWeb g_web;
extern INDEX hud_bShowWeapon;

extern const INDEX aiWeaponsRemap[19] = { 0,  1,  10,  2,  3,  4,  5,  6,  7,
                                          8,  9,  11, 13, 12, 14, 15, 16, 17, 18 };
%}

uses "EntitiesMP/Player";
uses "EntitiesMP/PlayerWeaponsEffects";
uses "EntitiesMP/Projectile";
uses "EntitiesMP/Bullet";
uses "EntitiesMP/BasicEffects";
//uses "EntitiesMP/WeaponItem";
//uses "EntitiesMP/AmmoItem";
//uses "EntitiesMP/AmmoPack";
uses "EntitiesMP/ModelHolder2";
//uses "EntitiesMP/Pipebomb";
//uses "EntitiesMP/GhostBusterRay";
//uses "EntitiesMP/CannonBall";
//uses "EntitiesMP/WeaponItem";

// input parameter for weapons
event EWeaponsInit {
  CEntityID eidOwner,        // who owns it
};

// select weapon
event ESelectWeapon {
  INDEX iWeapon,          // weapon to select
};

// boring weapon animations
event EBoringWeapon {};

// fire weapon
event EFireWeapon {};
// release weapon
event EReleaseWeapon {};
// reload weapon
event EReloadWeapon {};
// weapon changed - used to notify other entities
event EWeaponChanged {};


// select weapon - sent through net
event ENetSelectWeapon {
  INDEX iWeapon,          // weapon to select
};

// fire weapon
event ENetFireWeapon {};

// fire weapon
event ENetReleaseWeapon {};

// fire weapon - sent through net
event ENetReloadWeapon {};


// weapons (do not change order! - needed by HUD.cpp)
enum WeaponType {
  0 WEAPON_NONE               "", // don't consider this in WEAPONS_ALLAVAILABLEMASK
  1 WEAPON_KNIFE              "",
  //2 WEAPON_COLT               "",
//  3 WEAPON_DOUBLECOLT         "",
//  4 WEAPON_SINGLESHOTGUN      "",
//  5 WEAPON_DOUBLESHOTGUN      "",
  //6 WEAPON_TOMMYGUN           "",
//  7 WEAPON_MINIGUN            "",
  //8 WEAPON_ROCKETLAUNCHER     "",
  //9 WEAPON_GRENADELAUNCHER    "",
 10 WEAPON_CHAINSAW           "",
 11 WEAPON_FLAMER             "",
 12 WEAPON_LASER              "",
// 13 WEAPON_SNIPER             "",
// 14 WEAPON_IRONCANNON         "",
 15 WEAPON_LAST               "",
}; // see 'WEAPONS_ALLAVAILABLEMASK' -> (11111111111111 == 0x3FFF)

%{
// AVAILABLE WEAPON MASK
#define WEAPONS_ALLAVAILABLEMASK 0x3FFF

/*
#if BUILD_TEST
  #define WEAPONS_DISABLEDMASK (\
    (1<<(WEAPON_TOMMYGUN       -1))|\
    (1<<(WEAPON_GRENADELAUNCHER-1))|\
    (1<<(WEAPON_PIPEBOMB       -1))|\
    (1<<(WEAPON_FLAMER         -1))|\
    (1<<(WEAPON_LASER          -1))|\
    (1<<(WEAPON_GHOSTBUSTER    -1))|\
    (1<<(WEAPON_IRONCANNON     -1))|\
    (1<<(WEAPON_NUKECANNON     -1)))
#else 
  #define WEAPONS_DISABLEDMASK (0)
#endif
  */

#define MAX_WEAPONS 30


// MiniGun specific
	/*
#define MINIGUN_STATIC      0
#define MINIGUN_FIRE        1
#define MINIGUN_SPINUP      2
#define MINIGUN_SPINDOWN    3

#define MINIGUN_SPINUPTIME      0.5f
#define MINIGUN_SPINDNTIME      3.0f
#define MINIGUN_SPINUPSOUND     0.5f
#define MINIGUN_SPINDNSOUND     1.5f
#define MINIGUN_FULLSPEED       500.0f
#define MINIGUN_SPINUPACC       (MINIGUN_FULLSPEED/MINIGUN_SPINUPTIME)
#define MINIGUN_SPINDNACC       (MINIGUN_FULLSPEED/MINIGUN_SPINDNTIME)
#define MINIGUN_TICKTIME        (_pTimer->TickQuantum)
  */

// chainsaw specific
#define CHAINSAW_UPDATETIME     0.05f

// fire flare specific
#define FLARE_REMOVE 1
#define FLARE_ADD 2

// animation light specific
#define LIGHT_ANIM_MINIGUN 2
//#define LIGHT_ANIM_TOMMYGUN 3
#define LIGHT_ANIM_COLT_SHOTGUN 4
#define LIGHT_ANIM_NONE 5


// mana for ammo adjustment (multiplier)
#define MANA_AMMO (0.1f)

// position of weapon model -- weapon 0 is never used
static FLOAT wpn_fH[MAX_WEAPONS+1];
static FLOAT wpn_fP[MAX_WEAPONS+1];
static FLOAT wpn_fB[MAX_WEAPONS+1];
static FLOAT wpn_fX[MAX_WEAPONS+1];
static FLOAT wpn_fY[MAX_WEAPONS+1];
static FLOAT wpn_fZ[MAX_WEAPONS+1];
static FLOAT wpn_fFOV[MAX_WEAPONS+1];
static FLOAT wpn_fClip[MAX_WEAPONS+1];
static FLOAT wpn_fFX[MAX_WEAPONS+1];  // firing source
static FLOAT wpn_fFY[MAX_WEAPONS+1];
//static FLOAT wpn_fFZ[MAX_WEAPONS+1];
static INDEX wpn_iCurrent;
extern FLOAT hud_tmWeaponsOnScreen;

//0312 kwon
static INDEX _height;
static INDEX _width;

/*
// bullet positions
static FLOAT afSingleShotgunPellets[] =
{     -0.3f,+0.1f,    +0.0f,+0.1f,   +0.3f,+0.1f,
  -0.4f,-0.1f,  -0.1f,-0.1f,  +0.1f,-0.1f,  +0.4f,-0.1f
};
static FLOAT afDoubleShotgunPellets[] =
{
      -0.3f,+0.15f, +0.0f,+0.15f, +0.3f,+0.15f,
  -0.4f,+0.05f, -0.1f,+0.05f, +0.1f,+0.05f, +0.4f,+0.05f,
      -0.3f,-0.05f, +0.0f,-0.05f, +0.3f,-0.05f,
  -0.4f,-0.15f, -0.1f,-0.15f, +0.1f,-0.15f, +0.4f,-0.15f
};
*/

// sniper discrete zoom values - 4 (1x,2x,4x,6x)
// synchronize this with sniper properties (properties 233-237)
//static INDEX iSniperDiscreteZoomLevels = 4;
/*
static FLOAT afSniperZoom[] =
{
      90.0f,1.0f, 53.1f, 2.0f, 28.0f,4.0f, 14.2f,6.0f, 
       //7.2f,8.0f, 3.56f,10.0f ,1.8f,12.0f
};
*/

// crosshair console variables
static INDEX hud_bCrosshairFixed    = FALSE;
static INDEX hud_bCrosshairColoring = TRUE;
static FLOAT hud_fCrosshairScale    = 1.0f;
static FLOAT hud_fCrosshairOpacity  = 1.0f;
static FLOAT hud_fCrosshairRatio    = 0.5f;  // max distance size ratio
// misc HUD vars
static INDEX hud_bShowPlayerName = TRUE;
static INDEX hud_bShowCoords     = FALSE;
static FLOAT plr_tmSnoopingDelay = 1.0f; // seconds 
extern FLOAT plr_tmSnoopingTime  = 1.0f; // seconds 

// some static vars
static INDEX _iLastCrosshairType=-1;
static CTextureObject _toCrosshair;

// must do this to keep dependency catcher happy
/*
// deleted by seo
CTFileName fn1 = CTFILENAME("Textures\\Interface\\Crosshairs\\Crosshair1.tex");
CTFileName fn2 = CTFILENAME("Textures\\Interface\\Crosshairs\\Crosshair2.tex");
CTFileName fn3 = CTFILENAME("Textures\\Interface\\Crosshairs\\Crosshair3.tex");
CTFileName fn4 = CTFILENAME("Textures\\Interface\\Crosshairs\\Crosshair4.tex");
CTFileName fn5 = CTFILENAME("Textures\\Interface\\Crosshairs\\Crosshair5.tex");
CTFileName fn6 = CTFILENAME("Textures\\Interface\\Crosshairs\\Crosshair6.tex");
CTFileName fn7 = CTFILENAME("Textures\\Interface\\Crosshairs\\Crosshair7.tex");
*/

void CPlayerWeapons_Precache(ULONG ulAvailable)
{
  CDLLEntityClass *pdec = &CPlayerWeapons_DLLClass;

  // precache general stuff always
  /*
  // deleted by seo
  pdec->PrecacheTexture(TEX_REFL_BWRIPLES01      );
  pdec->PrecacheTexture(TEX_REFL_BWRIPLES02      );
  pdec->PrecacheTexture(TEX_REFL_LIGHTMETAL01    );
  pdec->PrecacheTexture(TEX_REFL_LIGHTBLUEMETAL01);
  pdec->PrecacheTexture(TEX_REFL_DARKMETAL       );
  pdec->PrecacheTexture(TEX_REFL_PURPLE01        );
  pdec->PrecacheTexture(TEX_SPEC_WEAK            );
  pdec->PrecacheTexture(TEX_SPEC_MEDIUM          );
  pdec->PrecacheTexture(TEX_SPEC_STRONG          );
  pdec->PrecacheTexture(TEXTURE_HAND             );
  pdec->PrecacheTexture(TEXTURE_FLARE01          );
  pdec->PrecacheModel(MODEL_FLARE01);
  pdec->PrecacheClass(CLASS_BULLET);
  pdec->PrecacheClass(CLASS_WEAPONITEM);
  pdec->PrecacheClass(CLASS_PROJECTILE, PRT_ROCKET); 

  // precache other weapons if available
  if ( ulAvailable&(1<<(WEAPON_KNIFE-1)) ) {
    pdec->PrecacheModel(MODEL_KNIFE                 );
    pdec->PrecacheModel(MODEL_KNIFEITEM             );
    pdec->PrecacheTexture(TEXTURE_KNIFEITEM         );
    pdec->PrecacheSound(SOUND_KNIFE_BACK            );
    pdec->PrecacheSound(SOUND_KNIFE_HIGH            );
    pdec->PrecacheSound(SOUND_KNIFE_LONG            );
    pdec->PrecacheSound(SOUND_KNIFE_LOW             );
  }

  if ( ulAvailable&(1<<(WEAPON_COLT-1)) ) {
    pdec->PrecacheModel(MODEL_COLT                  );
    pdec->PrecacheModel(MODEL_COLTCOCK              );
    pdec->PrecacheModel(MODEL_COLTMAIN              );
    pdec->PrecacheModel(MODEL_COLTBULLETS           );
    pdec->PrecacheTexture(TEXTURE_COLTMAIN          );  
    pdec->PrecacheTexture(TEXTURE_COLTCOCK          );  
    pdec->PrecacheTexture(TEXTURE_COLTBULLETS       );  
    pdec->PrecacheSound(SOUND_COLT_FIRE             );
    pdec->PrecacheSound(SOUND_COLT_RELOAD           );
  }

  if ( ulAvailable&(1<<(WEAPON_SINGLESHOTGUN-1)) ) {
    pdec->PrecacheModel(MODEL_SINGLESHOTGUN     );    
    pdec->PrecacheModel(MODEL_SS_SLIDER         );    
    pdec->PrecacheModel(MODEL_SS_HANDLE         );    
    pdec->PrecacheModel(MODEL_SS_BARRELS        );    
    pdec->PrecacheTexture(TEXTURE_SS_HANDLE     );    
    pdec->PrecacheTexture(TEXTURE_SS_BARRELS    );    
    pdec->PrecacheSound(SOUND_SINGLESHOTGUN_FIRE);    
  }

  if ( ulAvailable&(1<<(WEAPON_DOUBLESHOTGUN-1)) ) {
    pdec->PrecacheModel(MODEL_DOUBLESHOTGUN        ); 
    pdec->PrecacheModel(MODEL_DS_HANDLE            ); 
    pdec->PrecacheModel(MODEL_DS_BARRELS           ); 
    pdec->PrecacheModel(MODEL_DS_AMMO              ); 
    pdec->PrecacheModel(MODEL_DS_SWITCH            ); 
    pdec->PrecacheModel(MODEL_DS_HANDWITHAMMO      ); 
    pdec->PrecacheTexture(TEXTURE_DS_HANDLE        );   
    pdec->PrecacheTexture(TEXTURE_DS_BARRELS       );   
    pdec->PrecacheTexture(TEXTURE_DS_AMMO          );   
    pdec->PrecacheTexture(TEXTURE_DS_SWITCH        );   
    pdec->PrecacheSound(SOUND_DOUBLESHOTGUN_FIRE   ); 
    pdec->PrecacheSound(SOUND_DOUBLESHOTGUN_RELOAD ); 
  }

  if ( ulAvailable&(1<<(WEAPON_TOMMYGUN-1)) ) {
    pdec->PrecacheModel(MODEL_TOMMYGUN              );
    pdec->PrecacheModel(MODEL_TG_BODY               );
    pdec->PrecacheModel(MODEL_TG_SLIDER             );
    pdec->PrecacheTexture(TEXTURE_TG_BODY           );  
    pdec->PrecacheSound(SOUND_TOMMYGUN_FIRE         );
  }

  if ( ulAvailable&(1<<(WEAPON_SNIPER-1)) ) {
    pdec->PrecacheModel(MODEL_SNIPER          ); 
    pdec->PrecacheModel(MODEL_SNIPER_BODY     ); 
    pdec->PrecacheTexture(TEXTURE_SNIPER_BODY );   
    pdec->PrecacheSound(SOUND_SNIPER_FIRE     ); 
  }

  if ( ulAvailable&(1<<(WEAPON_MINIGUN-1)) ) {
    pdec->PrecacheModel(MODEL_MINIGUN          );     
    pdec->PrecacheModel(MODEL_MG_BARRELS       );     
    pdec->PrecacheModel(MODEL_MG_BODY          );     
    pdec->PrecacheModel(MODEL_MG_ENGINE        );     
    pdec->PrecacheTexture(TEXTURE_MG_BODY      );       
    pdec->PrecacheTexture(TEXTURE_MG_BARRELS   );       
    pdec->PrecacheSound(SOUND_MINIGUN_FIRE     );     
    pdec->PrecacheSound(SOUND_MINIGUN_ROTATE   );     
    pdec->PrecacheSound(SOUND_MINIGUN_SPINUP   );     
    pdec->PrecacheSound(SOUND_MINIGUN_SPINDOWN );     
    pdec->PrecacheSound(SOUND_MINIGUN_CLICK    );     
  }
                                         
  if ( ulAvailable&(1<<(WEAPON_ROCKETLAUNCHER-1)) ) {
    pdec->PrecacheModel(MODEL_ROCKETLAUNCHER     );   
    pdec->PrecacheModel(MODEL_RL_BODY            );   
    pdec->PrecacheModel(MODEL_RL_ROTATINGPART    );   
    pdec->PrecacheModel(MODEL_RL_ROCKET          );   
    pdec->PrecacheTexture(TEXTURE_RL_BODY        );     
    pdec->PrecacheTexture(TEXTURE_RL_ROCKET      );     
    pdec->PrecacheSound(SOUND_ROCKETLAUNCHER_FIRE);   
    pdec->PrecacheClass(CLASS_PROJECTILE, PRT_ROCKET);
  }                                        

  if ( ulAvailable&(1<<(WEAPON_GRENADELAUNCHER-1)) ) {
    pdec->PrecacheModel(MODEL_GRENADELAUNCHER       ); 
    pdec->PrecacheModel(MODEL_GL_BODY               ); 
    pdec->PrecacheModel(MODEL_GL_MOVINGPART         ); 
    pdec->PrecacheModel(MODEL_GL_GRENADE            ); 
    pdec->PrecacheTexture(TEXTURE_GL_BODY           );   
    pdec->PrecacheTexture(TEXTURE_GL_MOVINGPART     );
    pdec->PrecacheSound(SOUND_GRENADELAUNCHER_FIRE ); 
    pdec->PrecacheClass(CLASS_PROJECTILE, PRT_GRENADE);
  }
*/
/*
  if ( ulAvailable&(1<<(WEAPON_PIPEBOMB-1)) ) {
    pdec->PrecacheModel(MODEL_PIPEBOMB_STICK        );
    pdec->PrecacheModel(MODEL_PIPEBOMB_HAND         );
    pdec->PrecacheModel(MODEL_PB_BUTTON             );
    pdec->PrecacheModel(MODEL_PB_SHIELD             );
    pdec->PrecacheModel(MODEL_PB_STICK              );
    pdec->PrecacheModel(MODEL_PB_BOMB               );
    pdec->PrecacheTexture(TEXTURE_PB_STICK          );  
    pdec->PrecacheTexture(TEXTURE_PB_BOMB           );  
    pdec->PrecacheSound(SOUND_PIPEBOMB_FIRE         );
    pdec->PrecacheSound(SOUND_PIPEBOMB_OPEN         );
    pdec->PrecacheSound(SOUND_PIPEBOMB_THROW        );
    pdec->PrecacheClass(CLASS_PIPEBOMB);
  }
*/
  // deleted by seo
  /*if ( ulAvailable&(1<<(WEAPON_CHAINSAW-1))) {
    pdec->PrecacheModel(MODEL_CHAINSAW     );
    pdec->PrecacheModel(MODEL_CS_BODY      );
    pdec->PrecacheModel(MODEL_CS_BLADE     );
    pdec->PrecacheModel(MODEL_CS_TEETH     );
    pdec->PrecacheTexture(TEXTURE_CS_BODY  );  
    pdec->PrecacheTexture(TEXTURE_CS_BLADE );  
    pdec->PrecacheTexture(TEXTURE_CS_TEETH );              
    pdec->PrecacheSound(SOUND_CS_FIRE      );
    pdec->PrecacheSound(SOUND_CS_BEGINFIRE );
    pdec->PrecacheSound(SOUND_CS_ENDFIRE   );
    pdec->PrecacheSound(SOUND_CS_BRINGUP   );
    pdec->PrecacheSound(SOUND_CS_BRINGDOWN );
    pdec->PrecacheSound(SOUND_CS_IDLE      );
  }

  if ( ulAvailable&(1<<(WEAPON_FLAMER-1)) ) {
    pdec->PrecacheModel(MODEL_FLAMER      );
    pdec->PrecacheModel(MODEL_FL_BODY     );
    pdec->PrecacheModel(MODEL_FL_RESERVOIR);
    pdec->PrecacheModel(MODEL_FL_FLAME    );
    pdec->PrecacheTexture(TEXTURE_FL_BODY );  
    pdec->PrecacheTexture(TEXTURE_FL_FLAME);  
    pdec->PrecacheTexture(TEXTURE_FL_FUELRESERVOIR);  
    pdec->PrecacheSound(SOUND_FL_FIRE     );
    pdec->PrecacheSound(SOUND_FL_START    );
    pdec->PrecacheSound(SOUND_FL_STOP     );
    pdec->PrecacheClass(CLASS_PROJECTILE, PRT_FLAME);
  }


  if ( ulAvailable&(1<<(WEAPON_LASER-1)) ) {
    pdec->PrecacheModel(MODEL_LASER     );
    pdec->PrecacheModel(MODEL_LS_BODY   );
    pdec->PrecacheModel(MODEL_LS_BARREL );
    pdec->PrecacheTexture(TEXTURE_LS_BODY  );  
    pdec->PrecacheTexture(TEXTURE_LS_BARREL);  
    pdec->PrecacheSound(SOUND_LASER_FIRE);
    pdec->PrecacheClass(CLASS_PROJECTILE, PRT_LASER_RAY);
  }
  */
/*
  if ( ulAvailable&(1<<(WEAPON_GHOSTBUSTER-1)) ) {
    pdec->PrecacheModel(MODEL_GHOSTBUSTER     );
    pdec->PrecacheModel(MODEL_GB_BODY         );
    pdec->PrecacheModel(MODEL_GB_ROTATOR      );
    pdec->PrecacheModel(MODEL_GB_EFFECT1      );
    pdec->PrecacheModel(MODEL_GB_EFFECT1FLARE );
    pdec->PrecacheTexture(TEXTURE_GB_ROTATOR  );  
    pdec->PrecacheTexture(TEXTURE_GB_BODY     );  
    pdec->PrecacheTexture(TEXTURE_GB_LIGHTNING);  
    pdec->PrecacheTexture(TEXTURE_GB_FLARE    );  
    pdec->PrecacheSound(SOUND_GB_FIRE         );
    pdec->PrecacheClass(CLASS_GHOSTBUSTERRAY);
  }
  */
/*
  // deleted by seo
  if ( ulAvailable&(1<<(WEAPON_IRONCANNON-1)) ||
       ulAvailable&(1<<(WEAPON_NUKECANNON-1)) ) {
    pdec->PrecacheModel(MODEL_CANNON    );
    pdec->PrecacheModel(MODEL_CN_BODY   );
//    pdec->PrecacheModel(MODEL_CN_NUKEBOX);
    pdec->PrecacheTexture(TEXTURE_CANNON);
    pdec->PrecacheSound(SOUND_CANNON    );
    pdec->PrecacheSound(SOUND_CANNON_PREPARE);
    pdec->PrecacheClass(CLASS_CANNONBALL);
  }
  */

  // precache animator too
  extern void CPlayerAnimator_Precache(ULONG ulAvailable);
  CPlayerAnimator_Precache(ulAvailable);
  extern void CProjectile_PrecacheForPlayer(void); 
  CProjectile_PrecacheForPlayer();

}

void CPlayerWeapons_Init(void) {
  // declare weapon position controls
  _pShell->DeclareSymbol("user INDEX wpn_iCurrent;", &wpn_iCurrent);

  #include "Common/WeaponPositions.h"
  
  // declare crosshair and its coordinates
  _pShell->DeclareSymbol("persistent user INDEX hud_bCrosshairFixed;",    &hud_bCrosshairFixed);
  _pShell->DeclareSymbol("persistent user INDEX hud_bCrosshairColoring;", &hud_bCrosshairColoring);
  _pShell->DeclareSymbol("persistent user FLOAT hud_fCrosshairScale;",    &hud_fCrosshairScale);
  _pShell->DeclareSymbol("persistent user FLOAT hud_fCrosshairRatio;",    &hud_fCrosshairRatio);
  _pShell->DeclareSymbol("persistent user FLOAT hud_fCrosshairOpacity;",  &hud_fCrosshairOpacity);
                                  
  _pShell->DeclareSymbol("persistent user INDEX hud_bShowPlayerName;", &hud_bShowPlayerName);
  _pShell->DeclareSymbol("persistent user INDEX hud_bShowCoords;",     &hud_bShowCoords);

  _pShell->DeclareSymbol("persistent user FLOAT plr_tmSnoopingTime;",  &plr_tmSnoopingTime);
  _pShell->DeclareSymbol("persistent user FLOAT plr_tmSnoopingDelay;", &plr_tmSnoopingDelay);

  // precache base weapons
  CPlayerWeapons_Precache(0x03);
}

// weapons positions for raycasting and firing
/*
static FLOAT afKnifePos[4] = { -0.01f, 0.25f, 0.0f};
static FLOAT afColtPos[4] = { -0.01f, 0.1f, 0.0f};
static FLOAT afDoubleColtPos[4] = { -0.01f, 0.1f, 0.0f};
static FLOAT afSingleShotgunPos[4] = { 0.0f, 0.1f, 0.0f};
static FLOAT afDoubleShotgunPos[4] = {  0.0f, 0.1f, 0.0f};
static FLOAT afTommygunPos[4] = { 0.0f, 0.1f, 0.0f};
static FLOAT afMinigunPos[4] = { 0.0f, -0.075f, 0.0f};
static FLOAT afRocketLauncherPos[4] = { -0.175f, 0.19f, -0.23f};
static FLOAT afGrenadeLauncherPos[4] = { 0.0f, 0.16f, -1.42f};
static FLOAT afPipebombPos[4] = { 0.01f, 0.04f, -0.44f};
static FLOAT afFlamerPos[4] = { 0.0f, 0.18f, -0.62f};
static FLOAT afLaserPos[4] = {  0.0f, -0.095f, -0.65f};
static FLOAT afGhostBusterPos[4] = { 0.0f, 0.0f, -0.74f};
static FLOAT afCannonPos[4] = { 0.0f, 0.0f, -0.74f};
*/

// extra weapon positions for shells dropout
//static FLOAT afSingleShotgunShellPos[3] = { 0.2f, 0.0f, -0.31f};
//static FLOAT afDoubleShotgunShellPos[3] = { 0.0f, 0.0f, -0.5f};
//static FLOAT afTommygunShellPos[3] = { 0.2f, 0.0f, -0.31f};
//static FLOAT afMinigunShellPos[3] = { 0.2f, 0.0f, -0.31f};
//static FLOAT afMinigunShellPos3rdView[3] = { 0.2f, 0.2f, -0.31f};
//static FLOAT afSniperShellPos[3] = { 0.2f, 0.0f, -0.15f};

//static FLOAT afRightColtPipe[3] = { 0.07f, -0.05f, -0.26f};
//static FLOAT afSingleShotgunPipe[3] = { 0.2f, 0.0f, -1.25f};
//static FLOAT afDoubleShotgunPipe[3] = { 0.2f, 0.0f, -1.25f};
//static FLOAT afTommygunPipe[3] = { -0.06f, 0.1f, -0.6f};
//static FLOAT afMinigunPipe[3] = { -0.06f, 0.0f, -0.6f};
//static FLOAT afMinigunPipe3rdView[3] = { 0.25f, 0.3f, -2.5f};

//static FLOAT afLaserPos[4] = {  0.0f, -0.095f, -0.65f};
//static FLOAT afLaser1Pos[4] = { -0.115f, -0.05f, -0.65f};
//static FLOAT afLaser2Pos[4] = {  0.115f, -0.05f, -0.65f};
//static FLOAT afLaser3Pos[4] = { -0.145f, -0.14f, -0.8f};
//static FLOAT afLaser4Pos[4] = {  0.145f, -0.14f, -0.8f};

//#define TM_START m_aMiniGun
//#define F_OFFSET_CHG m_aMiniGunLast
//#define F_TEMP m_aMiniGunSpeed

// decrement ammo taking infinite ammo options in account
void DecAmmo(INDEX &ctAmmo, INDEX iDec = 1)
{
  if (_pNetwork->IsServer()) {
    ctAmmo-=iDec;
  }
}

extern ENGINE_API INDEX g_bSlaveNoTarget;
%}

class export CPlayerWeapons : CRationalEntity {
name      "Player Weapons";
thumbnail "";

properties:
  1 CEntityPointer m_penPlayer,       // player which owns it
  2 BOOL m_bFireWeapon = FALSE,       // weapon is firing
  3 BOOL m_bHasAmmo    = FALSE,       // weapon has ammo
  4 enum WeaponType m_iCurrentWeapon  = WEAPON_KNIFE features(EPROPF_NETSEND),    // currently active weapon (internal)
  5 enum WeaponType m_iWantedWeapon   = WEAPON_KNIFE features(EPROPF_NETSEND),     // wanted weapon (internal)
  6 enum WeaponType m_iPreviousWeapon = WEAPON_KNIFE features(EPROPF_NETSEND),   // previous active weapon (internal)
 11 INDEX m_iAvailableWeapons = 0x01 features(EPROPF_NETSEND),   // avaible weapons
 12 BOOL  m_bChangeWeapon = FALSE,      // change current weapon
 13 BOOL  m_bReloadWeapon = FALSE,      // reload weapon
 14 BOOL  m_bMirrorFire   = FALSE,      // fire with mirror model
 15 INDEX m_iAnim         = 0,          // temporary anim variable
 16 FLOAT m_fAnimWaitTime = 0.0f,       // animation wait time
 17 FLOAT m_tmRangeSoundSpawned = 0.0f, // for not spawning range sounds too often
// 23 BOOL  m_bSniperZoom = FALSE,        // zoom sniper
// 24 FLOAT m_fSniperFOV      = 90.0f,    // sniper FOV
// 28 FLOAT m_fSniperFOVlast  = 90.0f,    // sniper FOV for lerping

 18 CTString m_strLastTarget   = "",      // string for last target
 19 FLOAT m_tmTargetingStarted = -99.0f,  // when targeting started
 20 FLOAT m_tmLastTarget       = -99.0f,  // when last target was seen
 21 FLOAT m_tmSnoopingStarted  = -99.0f,  // is player spying another player
 22 CEntityPointer m_penTargeting,        // who is the target
 
 25 CModelObject m_moWeapon,               // current weapon model 
 26 CModelObject m_moWeaponSecond,         // current weapon second (additional) model
 27 FLOAT m_tmWeaponChangeRequired = 0.0f, // time when weapon change was required

 30 CEntityPointer m_penRayHit,         // entity hit by ray
 31 BOOL m_bTargetHitIsUsable = FALSE,  // is hit entity usable?
 32 FLOAT m_fRayHitDistance = 100.0f,   // distance from hit point
 34 FLOAT3D m_vRayHit     = FLOAT3D(0,0,0), // coordinates where ray hit
 35 FLOAT3D m_vRayHitLast = FLOAT3D(0,0,0), // for lerping
 36 FLOAT3D m_vBulletSource = FLOAT3D(0,0,0), // bullet launch position remembered here
 37 FLOAT3D m_vBulletTarget = FLOAT3D(0,0,0), // bullet hit (if hit) position remembered here

 // ammo for all weapons
 40 INDEX m_iBullets        = 0 features(EPROPF_NETSEND),
 41 INDEX m_iMaxBullets     = MAX_BULLETS,
 42 INDEX m_iShells         = 0 features(EPROPF_NETSEND),
 43 INDEX m_iMaxShells      = MAX_SHELLS,
 //44 INDEX m_iRockets        = 0 features(EPROPF_NETSEND),
 //45 INDEX m_iMaxRockets     = MAX_ROCKETS,
 //46 INDEX m_iGrenades       = 0 features(EPROPF_NETSEND),
 //47 INDEX m_iMaxGrenades    = MAX_GRENADES,
 48 INDEX m_iNapalm         = 0 features(EPROPF_NETSEND),
 49 INDEX m_iMaxNapalm      = MAX_NAPALM,
 50 INDEX m_iElectricity    = 0 features(EPROPF_NETSEND),
 51 INDEX m_iMaxElectricity = MAX_ELECTRICITY,
 52 INDEX m_iIronBalls      = 0 features(EPROPF_NETSEND),
 53 INDEX m_iMaxIronBalls   = MAX_IRONBALLS,
// 54 INDEX m_iNukeBalls      = 0 features(EPROPF_NETSEND),
// 55 INDEX m_iMaxNukeBalls   = MAX_NUKEBALLS,
// 54 INDEX m_iSniperBullets    = 0 features(EPROPF_NETSEND),
// 55 INDEX m_iMaxSniperBullets = MAX_SNIPERBULLETS,

// weapons specific
// knife
 70 INDEX m_iKnifeStand = 1,
// colt
 //71 INDEX m_iColtBullets = 6,
// minigun
 //72 FLOAT m_aMiniGun = 0.0f,
 //73 FLOAT m_aMiniGunLast = 0.0f,
 //74 FLOAT m_aMiniGunSpeed = 0.0f,

// lerped bullets fire
 76 FLOAT3D m_iLastBulletPosition = FLOAT3D(32000.0f, 32000.0f, 32000.0f),
 77 INDEX m_iBulletsOnFireStart = 0,
// sniper
// 78 FLOAT m_fSniperMaxFOV = 90.0f,
// 79 FLOAT m_fSniperMinFOV = 14.2f,
// 80 FLOAT m_fSnipingZoomSpeed = 2.0f,
// 81 BOOL  m_bSniping = FALSE,
// 82 FLOAT m_fMinimumZoomFOV  = 53.1f,
// 83 FLOAT m_tmLastSniperFire = 0.0f,
// pipebomb
//235 CEntityPointer m_penPipebomb,
//236 INDEX m_bPipeBombDropped = FALSE,
// flamer
 84 CEntityPointer m_penFlame,
// laser
 85 INDEX m_iLaserBarrel = 0,
// ghostbuster
//250 CEntityPointer m_penGhostBusterRay,
// fire flare
 86 INDEX m_iFlare = FLARE_REMOVE,       // 0-none, 1-remove, 2-add
 87 INDEX m_iSecondFlare = FLARE_REMOVE, // 0-none, 1-remove, 2-add
// cannon
 88 FLOAT m_fWeaponDrawPowerOld = 0,
 89 FLOAT m_fWeaponDrawPower = 0,
 90 FLOAT m_tmDrawStartTime = 0.0f,

 91 FLOAT m_tmFlamerStart=1e6,
 92 FLOAT m_tmFlamerStop=1e9,
 93 FLOAT m_tmLastChainsawSpray = 0.0f,

101 CEntityPointer m_penMusicHolder,
//102 CEntityPointer m_penSwitchHit,  // entity hit by switch ray

// ID of player entity that owns the animator - for init over net
110 INDEX m_iPlayerID = -1 features(EPROPF_NETSEND),
// if this is set, client will not reset item weapon model
111 BOOL m_bResetWeapon = TRUE features(EPROPF_NETSEND),
// if this is set, SetWeapon will change the weapon to single colt
//112 BOOL m_bForceColt = FALSE features(EPROPF_NETSEND),

//0105
113  FLOAT3D m_vRayHitTmp = FLOAT3D(0,0,0),
114  CEntityPointer m_penRayHitTmp,
115  FLOAT m_fRayHitDistanceTmp = 0.0f,
//0419 실시간 raycast
116  FLOAT3D m_vRayHitNow = FLOAT3D(0,0,0),
117  CEntityPointer m_penRayHitNow,
118  FLOAT m_fRayHitDistanceNow = 0.0f,

119  FLOAT3D m_vRayHitReserve = FLOAT3D(0,0,0),
120	 CEntityPointer m_penRayHitReserve,
121	 FLOAT m_fRayHitDistanceReserve = 0.0f,

123  CEntityPointer m_penRayHitClick,
125  CEntityPointer  m_penReservedTarget,
//안태훈 수정 시작	//(5th Closed beta)(0.2)
150  FLOAT3D m_vRayHitSurfaceNormal = FLOAT3D(0,1,0),
151  BOOL m_bPickConditionOk = FALSE,
//안태훈 수정 끝	//(5th Closed beta)(0.2)

{
  CEntity *penBullet;
  CPlacement3D plBullet;
  FLOAT3D vBulletDestination;
}

components:
  1 class   CLASS_PROJECTILE        "Classes\\Projectile.ecl",
  2 class   CLASS_BULLET            "Classes\\Bullet.ecl",
  3 class   CLASS_WEAPONEFFECT      "Classes\\PlayerWeaponsEffects.ecl",
  4 class   CLASS_PIPEBOMB          "Classes\\Pipebomb.ecl",
  5 class   CLASS_GHOSTBUSTERRAY    "Classes\\GhostBusterRay.ecl",
//  6 class   CLASS_CANNONBALL        "Classes\\CannonBall.ecl",
//  7 class   CLASS_WEAPONITEM        "Classes\\WeaponItem.ecl",
  8 class   CLASS_BASIC_EFFECT      "Classes\\BasicEffect.ecl",

// ************** HAND **************
 // deleted by seo
 //10 texture TEXTURE_HAND                "Data\\Models\\Weapons\\Hand.tex",

// ************** KNIFE **************
// deleted by seo
 /*20 model   MODEL_KNIFEITEM             "Data\\Models\\Weapons\\Knife\\KnifeItem.mdl",
 21 texture TEXTURE_KNIFEITEM           "Data\\Models\\Weapons\\Knife\\KnifeItem.tex",
 22 model   MODEL_KNIFE                 "Data\\Models\\Weapons\\Knife\\Knife.mdl",
 23 sound   SOUND_KNIFE_BACK            "data\\sounds\\Default.wav",
 24 sound   SOUND_KNIFE_HIGH            "data\\sounds\\Default.wav",
 25 sound   SOUND_KNIFE_LONG            "data\\sounds\\Default.wav",
 26 sound   SOUND_KNIFE_LOW             "data\\sounds\\Default.wav",
 
// ************** COLT **************
 30 model   MODEL_COLT                  "Data\\Models\\Weapons\\Colt\\Colt.mdl",
 31 model   MODEL_COLTCOCK              "Data\\Models\\Weapons\\Colt\\ColtCock.mdl",
 32 model   MODEL_COLTMAIN              "Data\\Models\\Weapons\\Colt\\ColtMain.mdl",
 33 model   MODEL_COLTBULLETS           "Data\\Models\\Weapons\\Colt\\ColtBullets.mdl",
 34 texture TEXTURE_COLTMAIN            "Data\\Models\\Weapons\\Colt\\ColtMain.tex",
 35 texture TEXTURE_COLTCOCK            "Data\\Models\\Weapons\\Colt\\ColtCock.tex",
 36 texture TEXTURE_COLTBULLETS         "Data\\Models\\Weapons\\Colt\\ColtBullets.tex",
 37 sound   SOUND_COLT_FIRE             "data\\sounds\\Default.wav",
 38 sound   SOUND_COLT_RELOAD           "data\\sounds\\Default.wav",

// ************** SINGLE SHOTGUN ************
 40 model   MODEL_SINGLESHOTGUN         "Data\\Models\\Weapons\\SingleShotgun\\SingleShotgun.mdl",
 41 model   MODEL_SS_SLIDER             "Data\\Models\\Weapons\\SingleShotgun\\Slider.mdl",
 42 model   MODEL_SS_HANDLE             "Data\\Models\\Weapons\\SingleShotgun\\Handle.mdl",
 43 model   MODEL_SS_BARRELS            "Data\\Models\\Weapons\\SingleShotgun\\Barrels.mdl",
 44 texture TEXTURE_SS_HANDLE           "Data\\Models\\Weapons\\SingleShotgun\\Handle.tex",
 45 texture TEXTURE_SS_BARRELS          "Data\\Models\\Weapons\\SingleShotgun\\Barrels.tex",
 46 sound   SOUND_SINGLESHOTGUN_FIRE    "Data\\Models\\Weapons\\SingleShotgun\\Sounds\\_Fire.wav",

// ************** DOUBLE SHOTGUN **************
 50 model   MODEL_DOUBLESHOTGUN         "Data\\Models\\Weapons\\DoubleShotgun\\DoubleShotgun.mdl",
 51 model   MODEL_DS_HANDLE             "Data\\Models\\Weapons\\DoubleShotgun\\Dshotgunhandle.mdl",
 52 model   MODEL_DS_BARRELS            "Data\\Models\\Weapons\\DoubleShotgun\\Dshotgunbarrels.mdl",
 53 model   MODEL_DS_AMMO               "Data\\Models\\Weapons\\DoubleShotgun\\Ammo.mdl",
 54 model   MODEL_DS_SWITCH             "Data\\Models\\Weapons\\DoubleShotgun\\Switch.mdl",
 55 model   MODEL_DS_HANDWITHAMMO       "Data\\Models\\Weapons\\DoubleShotgun\\HandWithAmmo.mdl",
 56 texture TEXTURE_DS_HANDLE           "Data\\Models\\Weapons\\DoubleShotgun\\Handle.tex",
 57 texture TEXTURE_DS_BARRELS          "Data\\Models\\Weapons\\DoubleShotgun\\Barrels.tex",
 58 texture TEXTURE_DS_AMMO             "Data\\Models\\Weapons\\DoubleShotgun\\Ammo.tex",
 59 texture TEXTURE_DS_SWITCH           "Data\\Models\\Weapons\\DoubleShotgun\\Switch.tex",
 60 sound   SOUND_DOUBLESHOTGUN_FIRE    "Data\\Models\\Weapons\\DoubleShotgun\\Sounds\\Fire.wav",
 61 sound   SOUND_DOUBLESHOTGUN_RELOAD  "Data\\Models\\Weapons\\DoubleShotgun\\Sounds\\Reload.wav",

// ************** TOMMYGUN **************
 70 model   MODEL_TOMMYGUN              "Data\\Models\\Weapons\\TommyGun\\TommyGun.mdl",
 71 model   MODEL_TG_BODY               "Data\\Models\\Weapons\\TommyGun\\Body.mdl",
 72 model   MODEL_TG_SLIDER             "Data\\Models\\Weapons\\TommyGun\\Slider.mdl",
 73 texture TEXTURE_TG_BODY             "Data\\Models\\Weapons\\TommyGun\\Body.tex",
 74 sound   SOUND_TOMMYGUN_FIRE         "Data\\Models\\Weapons\\TommyGun\\Sounds\\_Fire.wav",

// ************** MINIGUN **************
 80 model   MODEL_MINIGUN               "Data\\Models\\Weapons\\MiniGun\\MiniGun.mdl",
 81 model   MODEL_MG_BARRELS            "Data\\Models\\Weapons\\MiniGun\\Barrels.mdl",
 82 model   MODEL_MG_BODY               "Data\\Models\\Weapons\\MiniGun\\Body.mdl",
 83 model   MODEL_MG_ENGINE             "Data\\Models\\Weapons\\MiniGun\\Engine.mdl",
 84 texture TEXTURE_MG_BODY             "Data\\Models\\Weapons\\MiniGun\\Body.tex",
 99 texture TEXTURE_MG_BARRELS          "Data\\Models\\Weapons\\MiniGun\\Barrels.tex",
 85 sound   SOUND_MINIGUN_FIRE          "Data\\Models\\Weapons\\MiniGun\\Sounds\\Fire.wav",
 86 sound   SOUND_MINIGUN_ROTATE        "Data\\Models\\Weapons\\MiniGun\\Sounds\\Rotate.wav",
 87 sound   SOUND_MINIGUN_SPINUP        "Data\\Models\\Weapons\\MiniGun\\Sounds\\RotateUp.wav",
 88 sound   SOUND_MINIGUN_SPINDOWN      "Data\\Models\\Weapons\\MiniGun\\Sounds\\RotateDown.wav",
 89 sound   SOUND_MINIGUN_CLICK         "Data\\Models\\Weapons\\MiniGun\\Sounds\\Click.wav",

// ************** ROCKET LAUNCHER **************
 90 model   MODEL_ROCKETLAUNCHER        "Data\\Models\\Weapons\\RocketLauncher\\RocketLauncher.mdl",
 91 model   MODEL_RL_BODY               "Data\\Models\\Weapons\\RocketLauncher\\Body.mdl",
 92 texture TEXTURE_RL_BODY             "Data\\Models\\Weapons\\RocketLauncher\\Body.tex",
 93 model   MODEL_RL_ROTATINGPART       "Data\\Models\\Weapons\\RocketLauncher\\RotatingPart.mdl",
 94 texture TEXTURE_RL_ROTATINGPART     "Data\\Models\\Weapons\\RocketLauncher\\RotatingPart.tex",
 95 model   MODEL_RL_ROCKET             "Data\\Models\\Weapons\\RocketLauncher\\Projectile\\Rocket.mdl",
 96 texture TEXTURE_RL_ROCKET           "Data\\Models\\Weapons\\RocketLauncher\\Projectile\\Rocket.tex",
 97 sound   SOUND_ROCKETLAUNCHER_FIRE   "Data\\Models\\Weapons\\RocketLauncher\\Sounds\\_Fire.wav",

// ************** GRENADE LAUNCHER **************
100 model   MODEL_GRENADELAUNCHER       "Data\\Models\\Weapons\\GrenadeLauncher\\GrenadeLauncher.mdl",
101 model   MODEL_GL_BODY               "Data\\Models\\Weapons\\GrenadeLauncher\\Body.mdl",
102 model   MODEL_GL_MOVINGPART         "Data\\Models\\Weapons\\GrenadeLauncher\\MovingPipe.mdl",
103 model   MODEL_GL_GRENADE            "Data\\Models\\Weapons\\GrenadeLauncher\\GrenadeBack.mdl",
104 texture TEXTURE_GL_BODY             "Data\\Models\\Weapons\\GrenadeLauncher\\Body.tex",
105 texture TEXTURE_GL_MOVINGPART       "Data\\Models\\Weapons\\GrenadeLauncher\\MovingPipe.tex",
106 sound   SOUND_GRENADELAUNCHER_FIRE  "Data\\Models\\Weapons\\GrenadeLauncher\\Sounds\\_Fire.wav",

// ************** SNIPER **************
110 model   MODEL_SNIPER                "Data\\ModelsMP\\Weapons\\Sniper\\Sniper.mdl",
111 model   MODEL_SNIPER_BODY           "Data\\ModelsMP\\Weapons\\Sniper\\Body.mdl",
112 texture TEXTURE_SNIPER_BODY         "Data\\ModelsMP\\Weapons\\Sniper\\Body.tex",
113 sound   SOUND_SNIPER_FIRE           "Data\\ModelsMP\\Weapons\\Sniper\\Sounds\\Fire.wav",
//114 sound   SOUND_SNIPER_RELOAD         "ModelsMP\\Weapons\\Sniper\\Sounds\\Reload.wav",
//115 sound   SOUND_SNIPER_ZOOM           "ModelsMP\\Weapons\\Sniper\\Sounds\\Zoom.wav",
*/
/*
// ************** PIPEBOMB **************
110 model   MODEL_PIPEBOMB_STICK        "Data\\Models\\Weapons\\Pipebomb\\HandWithStick.mdl",
111 model   MODEL_PIPEBOMB_HAND         "Data\\Models\\Weapons\\Pipebomb\\HandWithBomb.mdl",
112 model   MODEL_PB_BUTTON             "Data\\Models\\Weapons\\Pipebomb\\Button.mdl",
113 model   MODEL_PB_SHIELD             "Data\\Models\\Weapons\\Pipebomb\\Shield.mdl",
114 model   MODEL_PB_STICK              "Data\\Models\\Weapons\\Pipebomb\\Stick.mdl",
115 model   MODEL_PB_BOMB               "Data\\Models\\Weapons\\Pipebomb\\Bomb.mdl",
116 texture TEXTURE_PB_STICK            "Data\\Models\\Weapons\\Pipebomb\\Stick.tex",
117 texture TEXTURE_PB_BOMB             "Data\\Models\\Weapons\\Pipebomb\\Bomb.tex",
118 sound   SOUND_PIPEBOMB_FIRE         "Data\\Models\\Weapons\\Pipebomb\\Sounds\\Fire.wav",
119 sound   SOUND_PIPEBOMB_OPEN         "Data\\Models\\Weapons\\Pipebomb\\Sounds\\Open.wav",
120 sound   SOUND_PIPEBOMB_THROW        "Data\\Models\\Weapons\\Pipebomb\\Sounds\\Throw.wav",
*/
// ************** FLAMER **************
/*
130 model   MODEL_FLAMER                "Data\\ModelsMP\\Weapons\\Flamer\\Flamer.mdl",
131 model   MODEL_FL_BODY               "Data\\ModelsMP\\Weapons\\Flamer\\Body.mdl",
132 model   MODEL_FL_RESERVOIR          "Data\\ModelsMP\\Weapons\\Flamer\\FuelReservoir.mdl",
133 model   MODEL_FL_FLAME              "Data\\ModelsMP\\Weapons\\Flamer\\Flame.mdl",
134 texture TEXTURE_FL_BODY             "Data\\ModelsMP\\Weapons\\Flamer\\Body.tex",
135 texture TEXTURE_FL_FLAME            "Data\\ModelsMP\\Effects\\Flame\\Flame.tex",
136 sound   SOUND_FL_FIRE               "Data\\ModelsMP\\Weapons\\Flamer\\Sounds\\Fire.wav",
137 sound   SOUND_FL_START              "Data\\ModelsMP\\Weapons\\Flamer\\Sounds\\Start.wav",
138 sound   SOUND_FL_STOP               "Data\\ModelsMP\\Weapons\\Flamer\\Sounds\\Stop.wav",
139 texture TEXTURE_FL_FUELRESERVOIR    "Data\\ModelsMP\\Weapons\\Flamer\\FuelReservoir.tex",


// ************** LASER **************
140 model   MODEL_LASER                 "Data\\Models\\Weapons\\Laser\\Laser.mdl",
141 model   MODEL_LS_BODY               "Data\\Models\\Weapons\\Laser\\Body.mdl",
142 model   MODEL_LS_BARREL             "Data\\Models\\Weapons\\Laser\\Barrel.mdl",
144 texture TEXTURE_LS_BODY             "Data\\Models\\Weapons\\Laser\\Body.tex",
145 texture TEXTURE_LS_BARREL           "Data\\Models\\Weapons\\Laser\\Barrel.tex",
146 sound   SOUND_LASER_FIRE            "Data\\Models\\Weapons\\Laser\\Sounds\\_Fire.wav",


// ************** CHAINSAW **************
150 model   MODEL_CHAINSAW              "Data\\ModelsMP\\Weapons\\Chainsaw\\Chainsaw.mdl",
151 model   MODEL_CS_BODY               "Data\\ModelsMP\\Weapons\\Chainsaw\\Body.mdl",
152 model   MODEL_CS_BLADE              "Data\\ModelsMP\\Weapons\\Chainsaw\\Blade.mdl",
160 model   MODEL_CS_TEETH              "Data\\ModelsMP\\Weapons\\Chainsaw\\Teeth.mdl",
153 texture TEXTURE_CS_BODY             "Data\\ModelsMP\\Weapons\\Chainsaw\\Body.tex",
154 texture TEXTURE_CS_BLADE            "Data\\ModelsMP\\Weapons\\Chainsaw\\Blade.tex",
161 texture TEXTURE_CS_TEETH            "Data\\ModelsMP\\Weapons\\Chainsaw\\Teeth.tex",
*/
155 sound   SOUND_CS_FIRE               "data\\sounds\\Default.wav",
156 sound   SOUND_CS_BEGINFIRE          "data\\sounds\\Default.wav",
157 sound   SOUND_CS_ENDFIRE            "data\\sounds\\Default.wav",
158 sound   SOUND_CS_BRINGUP            "data\\sounds\\Default.wav",
159 sound   SOUND_CS_IDLE               "data\\sounds\\Default.wav",
162 sound   SOUND_CS_BRINGDOWN          "data\\sounds\\Default.wav",


/*
// ************** GHOSTBUSTER **************
150 model   MODEL_GHOSTBUSTER           "Data\\Models\\Weapons\\GhostBuster\\GhostBuster.mdl",
151 model   MODEL_GB_BODY               "Data\\Models\\Weapons\\GhostBuster\\Body.mdl",
152 model   MODEL_GB_ROTATOR            "Data\\Models\\Weapons\\GhostBuster\\Rotator.mdl",
153 model   MODEL_GB_EFFECT1            "Data\\Models\\Weapons\\GhostBuster\\Effect01.mdl",
154 model   MODEL_GB_EFFECT1FLARE       "Data\\Models\\Weapons\\GhostBuster\\EffectFlare01.mdl",
155 texture TEXTURE_GB_ROTATOR          "Data\\Models\\Weapons\\GhostBuster\\Rotator.tex",
156 texture TEXTURE_GB_BODY             "Data\\Models\\Weapons\\GhostBuster\\Body.tex",
157 texture TEXTURE_GB_LIGHTNING        "Data\\Models\\Weapons\\GhostBuster\\Lightning.tex",
158 texture TEXTURE_GB_FLARE            "Data\\Models\\Weapons\\GhostBuster\\EffectFlare.tex",
159 sound   SOUND_GB_FIRE               "Data\\Models\\Weapons\\GhostBuster\\Sounds\\_Fire.wav",
*/
// ************** CANNON **************
/*
// deleted by seo
170 model   MODEL_CANNON                "Data\\Models\\Weapons\\Cannon\\Cannon.mdl",
171 model   MODEL_CN_BODY               "Data\\Models\\Weapons\\Cannon\\Body.mdl",
//172 model   MODEL_CN_NUKEBOX            "Data\\Models\\Weapons\\Cannon\\NukeBox.mdl",
173 texture TEXTURE_CANNON              "Data\\Models\\Weapons\\Cannon\\Body.tex",
174 sound   SOUND_CANNON                "Data\\Models\\Weapons\\Cannon\\Sounds\\Fire.wav",
175 sound   SOUND_CANNON_PREPARE        "Data\\Models\\Weapons\\Cannon\\Sounds\\Prepare.wav",
//175 model   MODEL_CN_LIGHT              "Data\\Models\\Weapons\\Cannon\\Light.mdl",

// ************** REFLECTIONS **************
200 texture TEX_REFL_BWRIPLES01         "Data\\Models\\ReflectionTextures\\BWRiples01.tex",
201 texture TEX_REFL_BWRIPLES02         "Data\\Models\\ReflectionTextures\\BWRiples02.tex",
202 texture TEX_REFL_LIGHTMETAL01       "Data\\Models\\ReflectionTextures\\LightMetal01.tex",
203 texture TEX_REFL_LIGHTBLUEMETAL01   "Data\\Models\\ReflectionTextures\\LightBlueMetal01.tex",
204 texture TEX_REFL_DARKMETAL          "Data\\Models\\ReflectionTextures\\DarkMetal.tex",
205 texture TEX_REFL_PURPLE01           "Data\\Models\\ReflectionTextures\\Purple01.tex",

// ************** SPECULAR **************
210 texture TEX_SPEC_WEAK               "Data\\Models\\SpecularTextures\\Weak.tex",
211 texture TEX_SPEC_MEDIUM             "Data\\Models\\SpecularTextures\\Medium.tex",
212 texture TEX_SPEC_STRONG             "Data\\Models\\SpecularTextures\\Strong.tex",

// ************** FLARES **************
250 model   MODEL_FLARE01               "Data\\Models\\Effects\\Weapons\\Flare01\\Flare.mdl",
251 texture TEXTURE_FLARE01             "Data\\Models\\Effects\\Weapons\\Flare01\\Flare.tex",
*/

functions:

BOOL CheckCharacterMove(CCastRay crRay)
{
	// WSS_WALLMOVE_BUGFIX 070612-------------------------------------->>
	// player.es SendMyNextPosition에서 처리시 캐릭터가 제자리 걸음을 하므로
	// 여기서 체크하여 이동치를 아예 없애줌
		
	BOOL bMoveCheck = TRUE;	
				
	// 싱글던전은 체크 제외 시킴(클라이언트 자체 충돌 체크 하므로)
	if ( CZoneInfo::getSingleton()->GetZoneType( g_slZone ) != ZONE_SDUNGEON )
	{
		// TODO : 터레인 체크 추가要	
		

		// BSP 체크 
//		if( m_penRayHitTmp->GetRenderType() == RT_BRUSH
//			|| m_penRayHitTmp->GetRenderType() == RT_FIELDBRUSH
//			&& m_bPickConditionOk)
		{							
			// 폴리곤의 속성이 UNWALKABLE이면 리턴
			if(crRay.cr_pbpoBrushPolygon!=NULL)
			{
				switch(crRay.cr_pbpoBrushPolygon->bpo_ubPolygonAttribute)
				{
					case BPOA_NONE:
					case BPOA_UNWALKABLE1F:
					case BPOA_UNWALKABLE2F:
					case BPOA_UNWALKABLE3F:
					case BPOA_UNWALKABLE4F:
					case BPOA_UNWALKABLE5F:
						bMoveCheck = FALSE;
						break;
					default:
						bMoveCheck = TRUE;
						break;
				}
			}
		}		
	}

	return bMoveCheck;
	// ----------------------------------------------------------<<
}

  void PlaySound(CSoundObject &so, SLONG idSoundComponent, SLONG slPlayType)
  {
    ASSERT(((CEntity*)m_penPlayer) != NULL);
//0218 kwon 서버가 아니라도 소리가 나도록 고침.
    if (!(((CPlayer*)(CEntity*)m_penPlayer)->IsReapplyingActions()) && _pNetwork->IsServer()) {
//if (!(((CPlayer*)(CEntity*)m_penPlayer)->IsReapplyingActions())) {
      CEntity::PlaySound(so, idSoundComponent, slPlayType);
    }
  };
  void PlaySound(CSoundObject &so, const CTFileName &fnmSound, SLONG slPlayType)
  {
    ASSERT(((CEntity*)m_penPlayer) != NULL);
    if (!(((CPlayer*)(CEntity*)m_penPlayer)->IsReapplyingActions()) && _pNetwork->IsServer()) {
      CEntity::PlaySound(so, fnmSound, slPlayType);
    }
  };

  // player weapon is initialized through player.es
  virtual void InitializeFromNet()
  {
    return;
  }  
  /*
   void GetWeaponName(CTString &strName) {
    switch (m_iCurrentWeapon) {
      case WEAPON_KNIFE:
        strName  = "Knife";
        break;
      case WEAPON_COLT:            
        strName  = "Shofield .45 w/ TMAR";
        break;
      case WEAPON_DOUBLECOLT:            
        strName  = "Shofield .45 w/ TMAR";
        break;
      case WEAPON_SINGLESHOTGUN:   
        strName  = "12 Gauge Pump Action Shotgun";
        break;
      case WEAPON_DOUBLESHOTGUN:   
        strName  = "Double Barrel Coach Gun";
        break;
      case WEAPON_TOMMYGUN:        
        strName  = "M1-A2 Tommygun";
        break;
      case WEAPON_SNIPER:        
        strName  = "RAPTOR 16mm Sniper";
        break;
      case WEAPON_MINIGUN:         
        strName  = "XM214-A Minigun";
        break;
      case WEAPON_ROCKETLAUNCHER:  
        strName  = "XPML21 Rocket Launcher";
        break;
      case WEAPON_GRENADELAUNCHER: 
        strName  = "MKIII Grenade Launcher";
        break;
      case WEAPON_FLAMER:          
        strName  = "XOP Flamethrower";
        break;
      case WEAPON_CHAINSAW:          
        strName  = "'Bonecracker' P-LAH Chainsaw";
        break;
      case WEAPON_LASER:           
        strName  = "XL2 Lasergun";
        break;
      case WEAPON_IRONCANNON:          
        strName  = "SBC Cannon";
        break;
      default:
        ASSERTALWAYS("Uknown weapon type");
    }
  }
  CTString GetWeaponNameFromIndex(INDEX iWeapon) {
    switch (iWeapon) {
      case WEAPON_KNIFE:
        return TRANS("Knife");
      case WEAPON_COLT:            
        return TRANS("Shofield .45 w/ TMAR");
      case WEAPON_DOUBLECOLT:            
        return TRANS("Double Shofield .45 w/ TMAR");
      case WEAPON_SINGLESHOTGUN:   
        return TRANS("12 Gauge Pump Action Shotgun");
      case WEAPON_DOUBLESHOTGUN:   
        return TRANS("Double Barrel Coach Gun");
      case WEAPON_TOMMYGUN:        
        return TRANS("M1-A2 Tommygun");
      case WEAPON_SNIPER:        
        return TRANS("RAPTOR 16mm Sniper");
      case WEAPON_MINIGUN:         
        return TRANS("XM214-A Minigun");
      case WEAPON_ROCKETLAUNCHER:  
        return TRANS("XPML21 Rocket Launcher");
      case WEAPON_GRENADELAUNCHER: 
        return TRANS("MKIII Grenade Launcher");
      case WEAPON_FLAMER:          
        return TRANS("XOP Flamethrower");
      case WEAPON_CHAINSAW:          
        return TRANS("'Bonecracker' P-LAH Chainsaw");
      case WEAPON_LASER:           
        return TRANS("XL2 Lasergun");
      case WEAPON_IRONCANNON:          
        return TRANS("SBC Cannon");
      default:
        ASSERTALWAYS("Uknown weapon type");
        return CTString("");
    }
  }
  */

  UWORD GetWeaponAmmo(INDEX iWeapon) 
  {
	  UWORD uwAmmo;
	  iWeapon = 0;
	  uwAmmo = 0;
	  /*
    // upper 5 bits are weapon index, lower 11 bits are ammo quantity
    // max 31 weapon, max 1023 rounds of ammo
    UWORD uwAmmo;
    switch (iWeapon) {
      case WEAPON_KNIFE : 
      case WEAPON_CHAINSAW : { 
        iWeapon = 0; uwAmmo = 0; break; 
      }
      case WEAPON_COLT : 
      case WEAPON_DOUBLECOLT : {      
        uwAmmo = m_iColtBullets; break; 
      }
      case WEAPON_SINGLESHOTGUN :
      case WEAPON_DOUBLESHOTGUN : {    
        uwAmmo = m_iShells; break; 
      }
      //case WEAPON_TOMMYGUN :
      case WEAPON_MINIGUN : {           
        uwAmmo = m_iBullets; break; 
      }
      case WEAPON_ROCKETLAUNCHER : {    
        uwAmmo = m_iRockets; break; 
      }
      case WEAPON_GRENADELAUNCHER : {   
        uwAmmo = m_iGrenades; break; 
      }
      case WEAPON_FLAMER : {            
        uwAmmo = m_iNapalm; break; 
      }
      case WEAPON_LASER : {             
        uwAmmo = m_iElectricity; break; 
      }
      case WEAPON_SNIPER : {            
        uwAmmo = m_iSniperBullets; break; 
      }
      case WEAPON_IRONCANNON : {        
        uwAmmo = m_iIronBalls; break; 
      }
      default : { iWeapon = WEAPON_NONE; uwAmmo=0; break; }
    }
	*/
    uwAmmo |= ((UWORD) iWeapon) << 11;

    return uwAmmo;
  }

  /*
  void SetWeaponAmmo(UWORD uwAmmo) {
    INDEX iWeapon = uwAmmo >> 11;    
    uwAmmo &= 0x07FF;
    switch (iWeapon) {
      case WEAPON_COLT :
      case WEAPON_DOUBLECOLT : {      
        m_iColtBullets = uwAmmo; break; 
      }
      case WEAPON_SINGLESHOTGUN :
      case WEAPON_DOUBLESHOTGUN : {    
        m_iShells = uwAmmo; break; 
      }
      case WEAPON_TOMMYGUN :
      case WEAPON_MINIGUN : {           
        m_iBullets = uwAmmo; break; 
      }
      case WEAPON_ROCKETLAUNCHER : {    
        m_iRockets = uwAmmo; break; 
      }
      case WEAPON_GRENADELAUNCHER : {   
        m_iGrenades = uwAmmo; break; 
      }
      case WEAPON_FLAMER : {            
        m_iNapalm = uwAmmo; break; 
      }
      case WEAPON_LASER : {             
        m_iElectricity = uwAmmo; break; 
      }
      case WEAPON_SNIPER : {            
        m_iSniperBullets = uwAmmo; break; 
      }
      case WEAPON_IRONCANNON : {        
        m_iIronBalls = uwAmmo; break; 
      }
      default : { break; }
    }

  }
  */

  void OnInitialize(const CEntityEvent &eeInput)
  {
    m_penMusicHolder = _pNetwork->GetEntityOfClass("MusicHolder", 0);
    CRationalEntity::OnInitialize(eeInput);
  }

  void Precache(void)
  {
    CPlayerWeapons_Precache(m_iAvailableWeapons);
  }
  CPlayer *GetPlayer(void)
  {
    ASSERT(m_penPlayer!=NULL);
    return (CPlayer *)&*m_penPlayer;
  }
  CPlayerAnimator *GetAnimator(void)
  {
    ASSERT(m_penPlayer!=NULL);
    return ((CPlayerAnimator*)&*((CPlayer&)*m_penPlayer).m_penAnimator);
  }

  CModelObject *GetChainSawTeeth(void)
  {
    CPlayer *ppl=GetPlayer();
    if(ppl==NULL) { return NULL;}
    CModelObject *pmoPlayer = ppl->GetModelObject();
    if(pmoPlayer==NULL) { return NULL;}
    CAttachmentModelObject *pamoTorso = pmoPlayer->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO);
    if(pamoTorso==NULL) { return NULL;}
    CAttachmentModelObject *pamoChainSaw = pamoTorso->amo_moModelObject.GetAttachmentModel(BODY_ATTACHMENT_MINIGUN);
    if(pamoChainSaw==NULL) { return NULL;}
    CAttachmentModelObject *pamoBlade = pamoChainSaw->amo_moModelObject.GetAttachmentModel(CHAINSAWFORPLAYER_ATTACHMENT_BLADE);
    if(pamoBlade==NULL) { return NULL;}
    CAttachmentModelObject *pamoTeeth = pamoBlade->amo_moModelObject.GetAttachmentModel(BLADE_ATTACHMENT_TEETH);
    if(pamoTeeth==NULL) { return NULL;}
    return &pamoTeeth->amo_moModelObject;
  }


  BOOL HoldingFire(void)
  {
    return m_bFireWeapon && !m_bChangeWeapon;
  }

  /*

  // render weapon model(s)
  void RenderWeaponModel( CPerspectiveProjection3D &prProjection, CDrawPort *pdp,
                          FLOAT3D vViewerLightDirection, COLOR colViewerLight, COLOR colViewerAmbient,
                          BOOL bRender, INDEX iEye)
  {
    _mrpModelRenderPrefs.SetRenderType( RT_TEXTURE|RT_SHADING_PHONG);

    // flare attachment
    //ControlFlareAttachment();

    if( !bRender || m_iCurrentWeapon==WEAPON_NONE
     || GetPlayer()->GetSettings()->ps_ulFlags&PSF_HIDEWEAPON) { return; }

    // nuke and iron cannons have the same view settings
    INDEX iWeaponData = m_iCurrentWeapon;

    // store FOV for Crosshair
    const FLOAT fFOV = ((CPerspectiveProjection3D &)prProjection).FOVL();
    CPlacement3D plView;
    plView = ((CPlayer&)*m_penPlayer).en_plViewpoint;
    plView.RelativeToAbsolute(m_penPlayer->GetPlacement());

    // added: chainsaw shaking
    CPlacement3D plWeapon;
    if (m_iCurrentWeapon==WEAPON_CHAINSAW) {
      CPlayer *plPlayer = (CPlayer*)&*m_penPlayer; 
      plWeapon = CPlacement3D ( FLOAT3D(wpn_fX[iWeaponData]+plPlayer->m_fChainsawShakeDX*0.35f,
                                        wpn_fY[iWeaponData]+plPlayer->m_fChainsawShakeDY*0.35f,
                                        wpn_fZ[iWeaponData]),
                                ANGLE3D(AngleDeg(wpn_fH[iWeaponData]),
                                        AngleDeg(wpn_fP[iWeaponData]),
                                        AngleDeg(wpn_fB[iWeaponData])));
    } else {
      plWeapon = CPlacement3D ( FLOAT3D(wpn_fX[iWeaponData],
                                        wpn_fY[iWeaponData],
                                        wpn_fZ[iWeaponData]),
                                ANGLE3D(AngleDeg(wpn_fH[iWeaponData]),
                                        AngleDeg(wpn_fP[iWeaponData]),
                                        AngleDeg(wpn_fB[iWeaponData])));
    }

    // make sure that weapon will be bright enough
    UBYTE ubLR,ubLG,ubLB, ubAR,ubAG,ubAB;
    ColorToRGB( colViewerLight,   ubLR,ubLG,ubLB);
    ColorToRGB( colViewerAmbient, ubAR,ubAG,ubAB);
    INDEX iMinDL = Min( Min(ubLR,ubLG),ubLB) -32;
    INDEX iMinDA = Min( Min(ubAR,ubAG),ubAB) -32;
    if( iMinDL<0) {
      ubLR = ClampUp( ubLR-iMinDL, (INDEX)255);
      ubLG = ClampUp( ubLG-iMinDL, (INDEX)255);
      ubLB = ClampUp( ubLB-iMinDL, (INDEX)255);
    }
    if( iMinDA<0) {
      ubAR = ClampUp( ubAR-iMinDA, (INDEX)255);
      ubAG = ClampUp( ubAG-iMinDA, (INDEX)255);
      ubAB = ClampUp( ubAB-iMinDA, (INDEX)255);
    }
    const COLOR colLight   = RGBToColor( ubLR,ubLG,ubLB);
    const COLOR colAmbient = RGBToColor( ubAR,ubAG,ubAB);
    const FLOAT tmNow = ((CPlayer *)&*m_penPlayer)->en_tmEntityTime;

    UBYTE ubBlend = INVISIBILITY_ALPHA_LOCAL;
    FLOAT tmInvisibility = ((CPlayer *)&*m_penPlayer)->m_tmInvisibility;
    //FLOAT tmSeriousDamage = ((CPlayer *)&*m_penPlayer)->m_tmSeriousDamage;
    //FLOAT tmInvulnerability = ((CPlayer *)&*m_penPlayer)->m_tmInvulnerability;
    if (tmInvisibility>tmNow) {
      FLOAT fIntensity=0.0f;      
      if((tmInvisibility-tmNow)<3.0f)
      {
        fIntensity = 0.5f-0.5f*cos((tmInvisibility-tmNow)*(6.0f*3.1415927f/3.0f));
        ubBlend =(INDEX)(INVISIBILITY_ALPHA_LOCAL+(FLOAT)(254-INVISIBILITY_ALPHA_LOCAL)*fIntensity);      
      }      
    }      
      
    // DRAW WEAPON MODEL
    //  Double colt - second colt in mirror
    //  Double shotgun - hand with ammo
    if( iWeaponData==WEAPON_DOUBLECOLT|| iWeaponData==WEAPON_DOUBLESHOTGUN )
    { 
      // prepare render model structure and projection
      CRenderModel rmMain;
      CPerspectiveProjection3D prMirror = prProjection;
      prMirror.ViewerPlacementL() =  plView;
      prMirror.FrontClipDistanceL() = wpn_fClip[iWeaponData];
      prMirror.DepthBufferNearL() = 0.0f;
      prMirror.DepthBufferFarL() = 0.1f;
      CPlacement3D plWeaponMirror( FLOAT3D(wpn_fX[iWeaponData], wpn_fY[iWeaponData], wpn_fZ[iWeaponData]),
                                   ANGLE3D(AngleDeg(wpn_fH[iWeaponData]), AngleDeg(wpn_fP[iWeaponData]),
                                           AngleDeg(wpn_fB[iWeaponData])));
      if( iWeaponData==WEAPON_DOUBLECOLT || iWeaponData==WEAPON_PIPEBOMB) {
        FLOATmatrix3D mRotation;
        MakeRotationMatrixFast(mRotation, plView.pl_OrientationAngle);
        plWeaponMirror.pl_PositionVector(1) = -plWeaponMirror.pl_PositionVector(1);
        plWeaponMirror.pl_OrientationAngle(1) = -plWeaponMirror.pl_OrientationAngle(1);
        plWeaponMirror.pl_OrientationAngle(3) = -plWeaponMirror.pl_OrientationAngle(3);
      }
      ((CPerspectiveProjection3D &)prMirror).FOVL() = AngleDeg(wpn_fFOV[iWeaponData]);
      CAnyProjection3D apr;
      apr = prMirror;
      Stereo_AdjustProjection(*apr, iEye, 0.1f);
      BeginModelRenderingView(apr, pdp);

      WeaponMovingOffset(plWeaponMirror.pl_PositionVector);
      plWeaponMirror.RelativeToAbsoluteSmooth(plView);
      rmMain.SetObjectPlacement(plWeaponMirror);

      rmMain.rm_colLight   = colLight;
      rmMain.rm_colAmbient = colAmbient;
      rmMain.rm_vLightDirection = vViewerLightDirection;
      rmMain.rm_ulFlags |= RMF_WEAPON; // TEMP: for Truform
      if (tmInvisibility>tmNow) {
        rmMain.rm_colBlend = (rmMain.rm_colBlend&0xffffff00)|ubBlend;
      }      
      
      m_moWeaponSecond.SetupModelRendering(rmMain);
      m_moWeaponSecond.RenderModel(rmMain);
      EndModelRenderingView();
    }

    // minigun specific (update rotation)
    if( iWeaponData==WEAPON_MINIGUN) { RotateMinigun(); }

    // prepare render model structure
    CRenderModel rmMain;
    prProjection.ViewerPlacementL() =  plView;
    prProjection.FrontClipDistanceL() = wpn_fClip[iWeaponData];
    prProjection.DepthBufferNearL() = 0.0f;
    prProjection.DepthBufferFarL() = 0.1f;
    ((CPerspectiveProjection3D &)prProjection).FOVL() = AngleDeg(wpn_fFOV[iWeaponData]);

    CAnyProjection3D apr;
    apr = prProjection;
    Stereo_AdjustProjection(*apr, iEye, 0.1f);
    BeginModelRenderingView(apr, pdp);

    WeaponMovingOffset(plWeapon.pl_PositionVector);
    plWeapon.RelativeToAbsoluteSmooth(plView);
    rmMain.SetObjectPlacement(plWeapon);

    rmMain.rm_colLight   = colLight;  
    rmMain.rm_colAmbient = colAmbient;
    rmMain.rm_vLightDirection = vViewerLightDirection;
    rmMain.rm_ulFlags |= RMF_WEAPON; // for Truform and occlusion culling!
    if(tmInvisibility>tmNow) {
      rmMain.rm_colBlend = (rmMain.rm_colBlend&0xffffff00)|ubBlend;
    }      
    
    m_moWeapon.SetupModelRendering(rmMain);
    m_moWeapon.RenderModel(rmMain);

    if (tmSeriousDamage>tmNow && tmInvulnerability>tmNow) {
      Particle_PrepareSystem(pdp, apr);
      Particle_PrepareEntity( 1, 0, 0, NULL);
      Particles_ModelGlow2(&m_moWeapon, plWeapon, Max(tmSeriousDamage, tmInvulnerability),PT_STAR08, 0.025f, 2, 0.01f, 0xff00ff00);
      Particle_EndSystem();
    } else if (tmInvulnerability>tmNow) {
      Particle_PrepareSystem(pdp, apr);
      Particle_PrepareEntity( 1, 0, 0, NULL);
      Particles_ModelGlow2(&m_moWeapon, plWeapon, tmInvulnerability, PT_STAR05, 0.025f, tmp_ai[1], 0.01f, 0x3333ff00);
      Particle_EndSystem();
    } else if (tmSeriousDamage>tmNow) {
      Particle_PrepareSystem(pdp, apr);
      Particle_PrepareEntity( 1, 0, 0, NULL);
      Particles_ModelGlow2(&m_moWeapon, plWeapon, tmSeriousDamage, PT_STAR08, 0.025f, 2, 0.01f, 0xff777700);
      Particle_EndSystem();
    }

    EndModelRenderingView();

    // restore FOV for Crosshair
    ((CPerspectiveProjection3D &)prProjection).FOVL() = fFOV;
  };
  */


  // Weapon moving offset
  void WeaponMovingOffset(FLOAT3D &plPos)
  {
    CPlayerAnimator &plan = (CPlayerAnimator&)*((CPlayer&)*m_penPlayer).m_penAnimator;
    FLOAT fXOffset = Lerp(plan.m_fMoveLastBanking, plan.m_fMoveBanking, _pTimer->GetLerpFactor()) * -0.02f;
    FLOAT fYOffset = Lerp(plan.m_fWeaponYLastOffset, plan.m_fWeaponYOffset, _pTimer->GetLerpFactor()) * 0.15f;
    fYOffset += (fXOffset * fXOffset) * 30.0f;
    plPos(1) += fXOffset;
    plPos(2) += fYOffset;
	/*
    // apply grenade launcher pumping
    if( m_iCurrentWeapon == WEAPON_GRENADELAUNCHER)
    {
      // obtain moving part attachment
      CAttachmentModelObject *amo = m_moWeapon.GetAttachmentModel(GRENADELAUNCHER_ATTACHMENT_MOVING_PART);
      FLOAT fLerpedMovement = Lerp(m_fWeaponDrawPowerOld, m_fWeaponDrawPower, _pTimer->GetLerpFactor());
      amo->amo_plRelative.pl_PositionVector(3) = fLerpedMovement;
      plPos(3) += fLerpedMovement/2.0f;
      if( m_tmDrawStartTime != 0.0f)
      {
        FLOAT tmPassed = _pTimer->GetLerpedCurrentTick()-m_tmDrawStartTime;
        plPos(1) += Sin(tmPassed*360.0f*10)*0.0125f*tmPassed/6.0f;
        plPos(2) += Sin(tmPassed*270.0f*8)*0.01f*tmPassed/6.0f;
      }
    }
    // apply cannon draw
    else if( (m_iCurrentWeapon == WEAPON_IRONCANNON) ||
             (m_iCurrentWeapon == WEAPON_NUKECANNON) )
    {
      FLOAT fLerpedMovement = Lerp(m_fWeaponDrawPowerOld, m_fWeaponDrawPower, _pTimer->GetLerpFactor());
      plPos(3) += fLerpedMovement;
      if( m_tmDrawStartTime != 0.0f)
      {
        FLOAT tmPassed = _pTimer->GetLerpedCurrentTick()-m_tmDrawStartTime;
        plPos(1) += Sin(tmPassed*360.0f*10)*0.0125f*tmPassed/2.0f;
        plPos(2) += Sin(tmPassed*270.0f*8)*0.01f*tmPassed/2.0f;
      }
    }
	*/
  };

//0218 kwon 사운드 플레이
void PlayAttackSound(CSoundObject &so, int what_sound, SLONG slPlayType){
	

	// deleted by seo
	/*switch(what_sound){
	case 1:
		idSoundComponent = SOUND_KNIFE_BACK;
		break;
	case 2:
		idSoundComponent = SOUND_KNIFE_LONG;
		break;

	default:
		idSoundComponent = SOUND_KNIFE_BACK;
		break;
	}

	PlaySound(so, idSoundComponent, SOF_3D|SOF_VOLUMETRIC);*/
};

  // cast a ray from weapon
  void UpdateTargetingInfo(void)
  {
    // crosshair start position from weapon
    CPlacement3D plCrosshair;
    FLOAT fFX = wpn_fFX[m_iCurrentWeapon];  // get weapon firing position
    FLOAT fFY = wpn_fFY[m_iCurrentWeapon];
    if (GetPlayer()->m_iViewState == PVT_3RDPERSONVIEW) {
      fFX = fFY = 0;
    }
    CalcWeaponPosition(FLOAT3D(fFX, fFY, 0), plCrosshair, FALSE);
    // cast ray
    CCastRay crRay( m_penPlayer, plCrosshair);
    crRay.cr_bHitTranslucentPortals = FALSE;
    crRay.cr_bPhysical = FALSE;
    crRay.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;
    GetWorld()->CastRay(crRay);
    // store required cast ray results
    m_vRayHitLast = m_vRayHit;  // for lerping purposes
    m_vRayHit   = crRay.cr_vHit;
    m_penRayHit = crRay.cr_penHit;
    m_fRayHitDistance = crRay.cr_fHitDistance;

/*
    // ************* cast switch seeking ray    
    CPlacement3D plView;
    GetPlayer()->GetLerpedAbsoluteViewPlacement(plView);
    CCastRay crSwitch( m_penPlayer, plView, 3.0f);
    crSwitch.cr_bHitTranslucentPortals = FALSE;
    crSwitch.cr_bPhysical = FALSE;
    crSwitch.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;
    crSwitch.cr_fTestR = 0.5f;
    GetWorld()->CastRay(crSwitch);
    // store required cast ray results -
    // store if hit switch or moving brush with switch
    m_penSwitchHit = NULL;
    if( IsOfClass( crSwitch.cr_penHit, &CMovingBrush_DLLClass)) {
      CEntity *penRelayedSwitch = ((CMovingBrush&)*crSwitch.cr_penHit).m_penSwitch;
      if( penRelayedSwitch !=NULL) {
        if( IsOfClass( penRelayedSwitch, &CSwitch_DLLClass)) {
          m_penSwitchHit = (CSwitch*)penRelayedSwitch;
        } else if( IsOfClass( penRelayedSwitch, &CMessageHolder_DLLClass)) {
          m_penSwitchHit = (CMessageHolder*)penRelayedSwitch;
        }
      }      
    } else if( IsOfClass( crSwitch.cr_penHit, &CSwitch_DLLClass)) {
      m_penSwitchHit = crSwitch.cr_penHit;
    } else if( IsOfClass( crSwitch.cr_penHit, &CMessageHolder_DLLClass)) {
      m_penSwitchHit = crSwitch.cr_penHit;
    }
    if( m_penSwitchHit!=NULL) {            
      m_penRayHit = m_penSwitchHit;  // for lerping purposes
      m_vRayHit   = crSwitch.cr_vHit;
      m_penRayHit = crSwitch.cr_penHit;
      m_fRayHitDistance = crSwitch.cr_fHitDistance;
    }
	*/
    // ************* cast switch seeking ray    

    TIME tmNow = _pTimer->CurrentTick();

    // initially, assume we hit an unusable entity
    m_bTargetHitIsUsable = FALSE;    
  }



  // Render Crosshair
  void RenderCrosshair( CProjection3D &prProjection, CDrawPort *pdp, CPlacement3D &plViewSource)
  {
	  CUIManager* pUIManager = SE_Get_UIManagerPtr();

    INDEX iCrossHair = GetPlayer()->GetSettings()->ps_iCrossHairType+1;

	if(pUIManager->IsMouseInsideUIs() || g_web.GetWebHandle())
	{
		return;
	}

	if (pUIManager->getCursorInGame() == false)
	{
		return;
	}

	CEntityPointer pEntity = m_penRayHitNow;

	// FIXME : 이쪽도 좀 깔끔하게 정리할것...
	if( GetPlayer()->m_bWaitForSkillTarget)
	{
		pUIManager->GetMouseCursor()->SetCursorType(UMCT_ATT_SKILL);
	}
	else if(pEntity!=NULL && pEntity->GetFlags()&ENF_ITEM)
	{
		pUIManager->GetMouseCursor()->SetCursorType(UMCT_PICK);
	}
	else if(pEntity!=NULL && pEntity->GetFlags()&ENF_ALIVE)
	{
		if(pEntity->IsFirstExtraFlagOn(ENF_EX1_NPC))
		{
			pUIManager->GetMouseCursor()->SetCursorType(UMCT_TALK);
		}
		else if(pEntity->IsFirstExtraFlagOn(ENF_EX1_PRODUCTION))
		{
			pUIManager->GetMouseCursor()->SetCursorType(UMCT_PRODUCE);
		}
		else if (pEntity->IsFirstExtraFlagOn(ENF_EX1_CLICK_OBJECT))
		{ // 오브젝트(ModelHolder3) 클릭.. 그냥 Talk
			pUIManager->GetMouseCursor()->SetCursorType(UMCT_TALK);
		}
		else
		{
			const BOOL bAttack = GetPlayer()->CheckAttackTarget( -1, pEntity, 0.0f );
			if( bAttack )
			{
				INDEX pc_Type = GetPlayer()->en_pcCharacter.pc_iPlayerType;
				BOOL bExtension = _pNetwork->MyCharacterInfo.bExtension;
				UIMCType cursorType = SE_Get_UtilHelpPtr()->GetPlayerAttackCursor(pc_Type, bExtension);
				pUIManager->GetMouseCursor()->SetCursorType(cursorType);
			}
			else
			{
				pUIManager->GetMouseCursor()->SetCursorType(UMCT_TALK);				
			}
		}
	}
	else
	{
		pUIManager->GetMouseCursor()->SetCursorType(UMCT_NORMAL);		
	}	

//0312 kwon
	_width = pdp->GetWidth();
	_height = pdp->GetHeight();

    // adjust crosshair type
    if( iCrossHair<=0) {
      iCrossHair  = 0;
      _iLastCrosshairType = 0;
    }

    // create new crosshair texture (if needed)
    if( _iLastCrosshairType != iCrossHair) {
      _iLastCrosshairType = iCrossHair;
      /*CTString fnCrosshair;
      fnCrosshair.PrintF( "Textures\\Interface\\Crosshairs\\Crosshair%d.tex", iCrossHair);
      try {
        // load new crosshair texture
        _toCrosshair.SetData_t( fnCrosshair);
      } catch( char *strError) { 
        // didn't make it! - reset crosshair
        CPrintF( strError);
        iCrossHair = 0;
        return;
      }*/		// yjpark - delete
    }
    COLOR colCrosshair = C_WHITE;
    TIME  tmNow = _pTimer->CurrentTick();

//0419
  if( m_penRayHitNow!=NULL)
    {
      CEntity *pen = m_penRayHitNow;
      // if alive 
      if( pen->GetFlags()&ENF_ALIVE)
      {
        // if player
        if( IsOfClass( pen, &CPlayer_DLLClass)) {
          // rememer when targeting begun  
          if( m_tmTargetingStarted==0) {
            m_penTargeting = pen;
            m_tmTargetingStarted = tmNow;
          }
          // keep player name, mana and health for eventual printout or coloring
//          m_fEnemyHealth = ((CPlayer*)pen)->GetHealth() / ((CPlayer*)pen)->m_fMaxHealth;
          m_strLastTarget.PrintF( "%s", ((CPlayer*)pen)->GetPlayerName());
		  /*
          if( GetSP()->sp_gmGameMode==CSessionProperties::GM_SCOREMATCH) {
            // add mana to player name
            CTString strMana="";
            strMana.PrintF( " (%d)", ((CPlayer*)pen)->m_iMana);
            m_strLastTarget += strMana;
          }
		  */
          FLOAT fDistanceToOther = DistanceTo(GetPlayer(), pen);
          if( hud_bShowPlayerName && fDistanceToOther<25.0f ) { m_tmLastTarget = tmNow+1.5f; }
        }
        // not targeting player
        else {
          // reset targeting
          m_tmTargetingStarted = 0; 
        }        
         // cannot snoop while firing
        if( m_bFireWeapon) { m_tmTargetingStarted = 0; }
      }
      // if not alive
      else
      {
        // not targeting player
        m_tmTargetingStarted = 0; 

        // check switch relaying by moving brush
        if( IsOfClass( pen, &CMovingBrush_DLLClass) && ((CMovingBrush&)*pen).m_penSwitch!=NULL) {
          pen = ((CMovingBrush&)*pen).m_penSwitch;
        }
        // if switch and near enough
        if( IsOfClass( pen, &CSwitch_DLLClass) && m_fRayHitDistance<2.5f) {
          CSwitch &enSwitch = (CSwitch&)*pen;
          m_bTargetHitIsUsable = TRUE;
          // if switch is useable
          if( enSwitch.m_bUseable) {
            // show switch message
            if( enSwitch.m_strMessage!="") { m_strLastTarget = enSwitch.m_strMessage; }
            else { m_strLastTarget = TRANS("Use"); }
            m_tmLastTarget = tmNow+0.5f;
          }
        }
      }
    }
    // if didn't hit anything
    else {
      // not targeting player
      m_tmTargetingStarted = 0; 
      // remember position ahead
//      FLOAT3D vDir = crRay.cr_vTarget-crRay.cr_vOrigin;
//      vDir.Normalize();
//      m_vRayHit = crRay.cr_vOrigin+vDir*50.0f;
    }

///////////////////////
//0419 실시간 타겟팅 레이 캐스트 적용
    // if hit anything
    FLOAT3D vOnScreen;
    FLOAT   fDistance = m_fRayHitDistanceNow;
    //const FLOAT3D vRayHit = Lerp( m_vRayHitLast, m_vRayHit, _pTimer->GetLerpFactor());
    const FLOAT3D vRayHit = m_vRayHitNow;  // lerping doesn't seem to work ???
    // if hit anything
    if( m_penRayHitNow!=NULL) {

      CEntity *pen = m_penRayHitNow;
      // do screen projection
      prProjection.ViewerPlacementL() = plViewSource;
      prProjection.ObjectPlacementL() = CPlacement3D( FLOAT3D(0.0f, 0.0f, 0.0f), ANGLE3D( 0, 0, 0));
      prProjection.Prepare();
      prProjection.ProjectCoordinate( vRayHit, vOnScreen);
      // if required, show enemy health thru crosshair color
 
//	  CPrintF("m_vRayHitNow %f %f %f\n",m_vRayHitNow(1),m_vRayHitNow(2),m_vRayHitNow(3));
      if( hud_bCrosshairColoring && _cmiComm.IsNetworkOn()) {

//0419
		m_tmLastTarget = 1000;
//0729 삭제.
//		m_strLastTarget.PrintF( "%s", pen->GetClass()->ec_pdecDLLClass->dec_strName);
      }
	  else{
		m_tmLastTarget = -99;
	  }

    }
    // if didn't hit anything
    else
    {
      // far away in screen center
      vOnScreen(1) = (FLOAT)pdp->GetWidth()  *0.5f;
      vOnScreen(2) = (FLOAT)pdp->GetHeight() *0.5f;
      fDistance    = 100.0f;
    }

    // if croshair should be of fixed position
    if( hud_bCrosshairFixed || GetPlayer()->m_iViewState == PVT_3RDPERSONVIEW) {
      // reset it to screen center
//0105 2line 지우기 //0205 수정
//      vOnScreen(1) = (FLOAT)pdp->GetWidth()  *0.5f;
//      vOnScreen(2) = (FLOAT)pdp->GetHeight() *0.5f;
//0105

	  //POINT pntMouse;
	  //::GetCursorPos(&pntMouse);
	  //::ScreenToClient(_pInput->_Hwnd, &pntMouse);
	  vOnScreen(1) = _pInput->inp_ptMousePos.x + 16;
	  vOnScreen(2) = pdp->GetHeight()-_pInput->inp_ptMousePos.y - 16;
//..

      //fDistance    = 100.0f;
    }
/*    
    // clamp console variables
	//0224 kwon 크로스헤어 크기 최소.0.5f->1.0f //0406
    hud_fCrosshairScale   = 1.0f;//Clamp( hud_fCrosshairScale,   0.1f, 2.0f);
    hud_fCrosshairRatio   = Clamp( hud_fCrosshairRatio,   0.1f, 1.0f);
    hud_fCrosshairOpacity = Clamp( hud_fCrosshairOpacity, 0.1f, 1.0f);
    const ULONG ulAlpha = NormFloatToByte( hud_fCrosshairOpacity);
    // draw crosshair if needed
    if( iCrossHair>0) {
      // determine crosshair size
      const FLOAT fMinD =   1.0f;
      const FLOAT fMaxD = 100.0f;
      fDistance = Clamp( fDistance, fMinD, fMaxD);
      const FLOAT fRatio   = (fDistance-fMinD) / (fMaxD-fMinD);
      const FLOAT fMaxSize = (FLOAT)pdp->GetHeight() / 480.0f;
      const FLOAT fMinSize = fMaxSize * hud_fCrosshairRatio;
      const FLOAT fSize    = 16;//0406 kwon 크기 고정 = 16 * Lerp( fMaxSize, fMinSize, fRatio) * hud_fCrosshairScale;
      // draw crosshair
      const FLOAT fI0 = + (PIX)vOnScreen(1) - fSize;
      const FLOAT fI1 = + (PIX)vOnScreen(1) + fSize;
      const FLOAT fJ0 = - (PIX)vOnScreen(2) - fSize +pdp->GetHeight();
      const FLOAT fJ1 = - (PIX)vOnScreen(2) + fSize +pdp->GetHeight();
      pdp->InitTexture( &_toCrosshair);
      pdp->AddTexture( fI0, fJ0, fI1, fJ1, colCrosshair|ulAlpha);
      pdp->FlushRenderingQueue();
    }

    // fetch drawport dimensions and make text scale factor
    const SLONG slDPWidth  = pdp->GetWidth();
    const SLONG slDPHeight = pdp->GetHeight();
    const FLOAT fTextScale = slDPHeight/480.0f/2;

    // if there is still time
    TIME tmDelta = m_tmLastTarget - tmNow;
    if( tmDelta>0) {
      // set font and scale
      pdp->SetFont( _pfdDisplayFont);
      pdp->SetTextScaling( fTextScale);
      pdp->SetTextAspect( 1.0f);
      pdp->SetTextShadow(+2);
      // do faded printout
      ULONG ulA = ulAlpha * Clamp( 2*tmDelta, 0.0f, 1.0f);
//0419 kwon 위치 수정
      //pdp->PutTextC( m_strLastTarget, slDPWidth*0.5f, slDPHeight*0.75f, SE_COL_BLUE_NEUTRAL|ulA);
	  pdp->PutTextC( m_strLastTarget, slDPWidth*0.5f, slDPHeight*0.01f, SE_COL_BLUE_NEUTRAL|ulA);
    }

    // printout crosshair world coordinates if needed
    if( hud_bShowCoords) { 
      // set font and scale
      pdp->SetFont( _pfdDisplayFont);
      pdp->SetTextScaling( fTextScale);
      pdp->SetTextAspect( 1.0f);
      pdp->SetTextShadow(+1);
      // do printout only if coordinates are valid
      const FLOAT fMax = Max( Max( vRayHit(1), vRayHit(2)), vRayHit(3));
      const FLOAT fMin = Min( Min( vRayHit(1), vRayHit(2)), vRayHit(3));
      if( fMax<+100000 && fMin>-100000) {
        CTString strCoords;
        strCoords.PrintF( "%.0f,%.0f,%.0f", vRayHit(1), vRayHit(2), vRayHit(3));
        pdp->PutTextC( strCoords, slDPWidth*0.5f, slDPHeight*0.10f, C_WHITE|CT_OPAQUE);
      }
    }
*/
  };
/*
//0105 마우스 클릭했을때 그곳의 위치 정보.
// obtain information about what was hitted with mouse
CPlacement3D GetMouseHitInformation( POINT point, CAnyProjection3D  prProjection )
{
 
 // CAnyProjection3D prProjection;

  prProjection->Prepare();
  // make the ray from viewer point through the mouse point, in current projection
  CPlacement3D plRay;
  //0311 kwon
  prProjection->RayThroughPoint(FLOAT3D((float)point.x,_height-(float)point.y, 0.0f), plRay);

//CalcWeaponPosition(FLOAT3D(1, 0, 0), plRay, FALSE);

  CCastRay crRay( m_penPlayer, plRay);  
    GetWorld()->CastRay(crRay);	
//	GetWorld()->ContinueCast(crRay);

//	if(!(crRay.cr_penHit->GetFlags()&ENF_ALIVE) && !(crRay.cr_penHit->GetFlags()&ENF_ZONING)){	
//		GetWorld()->ContinueCast(crRay);
//	return plRay;
//	}

	m_vRayHitTmp = crRay.cr_vHit;	
	m_penRayHitTmp = crRay.cr_penHit;
    m_fRayHitDistanceTmp = crRay.cr_fHitDistance; 

	return plRay;
 
}
//..
*/
CPlacement3D GetMouseHitInformation( POINT point, CAnyProjection3D  prProjection , BOOL bClick)
{
	
 // CAnyProjection3D prProjection;

  prProjection->Prepare();
  // make the ray from viewer point through the mouse point, in current projection
  CPlacement3D plRay;
  CPlacement3D plRayTmp;
  //0311 kwon
  prProjection->RayThroughPoint(FLOAT3D((float)point.x,_height-(float)point.y, 0.0f), plRay);

  plRayTmp = plRay;

	//CCastRay crRay( m_penPlayer, plRay);
	CCastRay crRay( GetPlayer()->GetViewEntity(TRUE), plRay );		// yjpark
//안태훈 수정 시작	//(Taiwan Closed beta)(0.2)
	crRay.cr_penIgnore = GetPlayer();	//주의 ContinuCast를 사용할 경우 cr_penIgnore에 다른 entity가 설정됨.
//안태훈 수정 끝	//(Taiwan Closed beta)(0.2)

//  crRay.cr_bHitTranslucentPortals = FALSE; 
	crRay.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;

    GetWorld()->CastRay(crRay);	

	if(	crRay.cr_penHit!=NULL && 
		!((crRay.cr_penHit->GetFlags()&ENF_ITEM) || (crRay.cr_penHit->IsCharacter()) || (crRay.cr_penHit->GetFlags()&ENF_ALIVE)	))
	{
		//CCastRay crRay( m_penPlayer, plRayTmp);  
		CCastRay crRay( GetPlayer()->GetViewEntity(TRUE), plRayTmp );	// yjpark
		crRay.cr_bHitTranslucentPortals = FALSE; 
		crRay.cr_ttHitModels = CCastRay::TT_NONE;
		GetWorld()->CastRay(crRay);	
		
		if(bClick)
		{
			m_penRayHitClick = crRay.cr_penHit;		
			
			if(_pInput->GetInputDevice(1)->id_aicControls[KID_LSHIFT].ic_fValue!=1.0f && !_pInput->IsRMousePressed())//Shift가 눌러있거나, 마우스 우클릭일경우에는 SKIP...
			{ 
				m_vRayHitTmp = crRay.cr_vHit;	//0910 직접 타겟팅.
				m_penRayHitTmp = crRay.cr_penHit;
				m_fRayHitDistanceTmp = crRay.cr_fHitDistance; 			
			}
			
			if(crRay.cr_penHit != NULL
			&& (crRay.cr_penHit->GetFlags()&ENF_ALIVE || crRay.cr_penHit->IsCharacter()))//1209 시체도 찍히게..
			{
				m_penReservedTarget = crRay.cr_penHit;	
			}
			else //이동해야지...
			{
				if(!_pInput->IsRMousePressed())
				{
					CPlayer &pl = (CPlayer&)*m_penPlayer;
					CEntity *penEnt = m_penRayHitTmp;
// WSS_WALLMOVE_BUGFIX 070612
					BOOL m_bMoveCheck = TRUE;

					if (m_penRayHitTmp != NULL)
					{
						if( m_penRayHitTmp->GetRenderType() == RT_BRUSH
							|| m_penRayHitTmp->GetRenderType() == RT_FIELDBRUSH
							&& m_bPickConditionOk)
						{
							m_bMoveCheck = CheckCharacterMove(crRay);
						}
					}

					if(m_bMoveCheck)
					{
						if(penEnt != NULL && penEnt->GetFlags()&ENF_ALIVE)
						{
							pl.m_vDesiredPosition  = penEnt->en_plPlacement.pl_PositionVector;
						}
						else
						{
							pl.m_vDesiredPosition  = FLOAT3D(crRay.cr_vHit(1),crRay.cr_vHit(2),crRay.cr_vHit(3));
						}
					}
				}
			}
		}
		m_vRayHitNow = crRay.cr_vHit;	
		m_penRayHitNow = crRay.cr_penHit;
		m_fRayHitDistanceNow = crRay.cr_fHitDistance; 
		
//안태훈 수정 시작	//(5th Closed beta)(0.2)
		m_bPickConditionOk = TRUE;
		if(crRay.cr_pbpoBrushPolygon != NULL)
		{
			m_vRayHitSurfaceNormal = crRay.cr_pbpoBrushPolygon->bpo_pbplPlane->bpl_plAbsolute;
			if(crRay.cr_pbpoBrushPolygon->bpo_ulFlags & BPOF_INVISIBLE) {m_bPickConditionOk = FALSE;}
		}
		else m_vRayHitSurfaceNormal = FLOAT3D(0,1,0);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
		return plRayTmp;
	}	
	
	if(bClick && crRay.cr_penHit)
	{
		m_penRayHitClick = crRay.cr_penHit;	

		if(_pInput->GetInputDevice(1)->id_aicControls[KID_LSHIFT].ic_fValue!=1.0f && !_pInput->IsRMousePressed())//Shift가 눌러있거나, 마우스 우클릭일경우에는 SKIP...
		{ 
			m_vRayHitTmp = crRay.cr_vHit;	//직접 타겟팅.
			m_penRayHitTmp = crRay.cr_penHit;
			m_fRayHitDistanceTmp = crRay.cr_fHitDistance;
 
			CPlayer &pl = (CPlayer&)*m_penPlayer;
			CEntity *penEnt = m_penRayHitTmp;
			if(penEnt != NULL && penEnt->GetFlags()&ENF_ALIVE)
			{
				pl.m_vDesiredPosition  = penEnt->en_plPlacement.pl_PositionVector;
			}
			else
			{
				pl.m_vDesiredPosition  = FLOAT3D(crRay.cr_vHit(1),crRay.cr_vHit(2),crRay.cr_vHit(3));
			}
//			pl.m_vDesiredPosition  = FLOAT3D(crRay.cr_vHit(1),crRay.cr_vHit(2),crRay.cr_vHit(3));
		}
		if(crRay.cr_penHit->GetFlags()&ENF_ALIVE || crRay.cr_penHit->IsCharacter())//1209 시체도 찍히게..
		{
			m_penReservedTarget = crRay.cr_penHit;
		}
		else //이동해야지...
		{
			if(!_pInput->IsRMousePressed())
			{
				CPlayer &pl = (CPlayer&)*m_penPlayer;
				CEntity *penEnt = m_penRayHitTmp;
				if(penEnt != NULL && penEnt->GetFlags()&ENF_ALIVE)
				{
					pl.m_vDesiredPosition  = penEnt->en_plPlacement.pl_PositionVector;
				}
				else
				{
					pl.m_vDesiredPosition  = FLOAT3D(crRay.cr_vHit(1),crRay.cr_vHit(2),crRay.cr_vHit(3));
				}
//				pl.m_vDesiredPosition  = FLOAT3D(crRay.cr_vHit(1),crRay.cr_vHit(2),crRay.cr_vHit(3));
			}
		}

	}
	m_vRayHitNow = crRay.cr_vHit;	
	m_penRayHitNow = crRay.cr_penHit;

	//전사의 축복일 경우 타켓팅하지 않음...
	if( m_penRayHitNow )
	{
		if( m_penRayHitNow->GetName() == CMobData::getData(310)->GetName() ||
			m_penRayHitNow->GetName() == CMobData::getData(311)->GetName() ||
			m_penRayHitNow->GetName() == CMobData::getData(312)->GetName() ||
			m_penRayHitNow->GetName() == CMobData::getData(313)->GetName() )
		{
			m_penRayHitNow =NULL;
			//m_penReservedTarget =NULL;
		}
	}

	m_fRayHitDistanceNow = crRay.cr_fHitDistance; 
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	if(crRay.cr_pbpoBrushPolygon != NULL)
	{
		m_vRayHitSurfaceNormal = crRay.cr_pbpoBrushPolygon->bpo_pbplPlane->bpl_plAbsolute;
	}
	else m_vRayHitSurfaceNormal = FLOAT3D(0,1,0);
//안태훈 수정 끝	//(5th Closed beta)(0.2)

	return plRay;
//강동민 수정 시작 싱글 던젼 작업	07.29
	//}
//강동민 수정 끝 싱글 던젼 작업		07.29 
}

CPlacement3D GetMouseHitInformationReserve( POINT point, CAnyProjection3D  prProjection , BOOL bClick, BOOL bReserve)
{	
	// CAnyProjection3D prProjection;
	
	prProjection->Prepare();
	// make the ray from viewer point through the mouse point, in current projection
	CPlacement3D plRay;
	CPlacement3D plRayTmp;
	//0311 kwon
	prProjection->RayThroughPoint(FLOAT3D((float)point.x,_height-(float)point.y, 0.0f), plRay);
	
	plRayTmp = plRay;
	
	//CCastRay crRay( m_penPlayer, plRay);  
	CCastRay crRay( GetPlayer()->GetViewEntity(TRUE), plRay );		// yjpark
	crRay.cr_penIgnore = GetPlayer();	//주의 ContinuCast를 사용할 경우 cr_penIgnore에 다른 entity가 설정됨.
	//  crRay.cr_bHitTranslucentPortals = FALSE; 
	crRay.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;
	
    GetWorld()->CastRay(crRay);	
	//0608 kwon
	if(crRay.cr_penHit!=NULL &&  !(crRay.cr_penHit->GetFlags()&ENF_ITEM))
	{
		if(crRay.cr_penHit!=NULL && !(crRay.cr_penHit->GetFlags()&ENF_ALIVE))
		{//아무것도 안걸렸다면...
			//CCastRay crRay( m_penPlayer, plRayTmp);
			CCastRay crRay( GetPlayer()->GetViewEntity(TRUE), plRayTmp );		// yjpark
			crRay.cr_bHitTranslucentPortals = FALSE; 
			crRay.cr_ttHitModels = CCastRay::TT_NONE;
			GetWorld()->CastRay(crRay);	
			
			if(bClick)
			{
				if(bReserve)
				{
// WSS_WALLMOVE_BUGFIX 070612									
					BOOL m_bMoveCheck = TRUE;
					m_bMoveCheck = CheckCharacterMove(crRay);
					if(!m_bMoveCheck)
					{
						((CPlayer&)*m_penPlayer).m_bReserveMove = FALSE;								
						return plRay;								
					}
					m_vRayHitReserve = crRay.cr_vHit;	
					m_penRayHitReserve = crRay.cr_penHit;
					m_fRayHitDistanceReserve = crRay.cr_fHitDistance; 
					
				}
				else
				{
					m_vRayHitTmp = crRay.cr_vHit;	
					m_penRayHitTmp = crRay.cr_penHit;
					m_fRayHitDistanceTmp = crRay.cr_fHitDistance; 			
				}
			}
			m_vRayHitNow = crRay.cr_vHit;	
			m_penRayHitNow = crRay.cr_penHit;
			m_fRayHitDistanceNow = crRay.cr_fHitDistance; 
			
			return plRayTmp;
		}	
	}
	
	if(bClick)
	{
		if(bReserve)
		{
			m_vRayHitReserve = crRay.cr_vHit;	
			m_penRayHitReserve = crRay.cr_penHit;
			m_fRayHitDistanceReserve = crRay.cr_fHitDistance; 
		}
		else
		{
			m_vRayHitTmp = crRay.cr_vHit;	
			m_penRayHitTmp = crRay.cr_penHit;
			m_fRayHitDistanceTmp = crRay.cr_fHitDistance; 
		}
	}
	m_vRayHitNow = crRay.cr_vHit;	
	m_penRayHitNow = crRay.cr_penHit;
	m_fRayHitDistanceNow = crRay.cr_fHitDistance; 
	
	return plRay; 
}

/************************************************************
 *                      FIRE FLARE                          *
 ************************************************************/
  // show flare
  void ShowFlare(CModelObject &moWeapon, INDEX iAttachObject, INDEX iAttachFlare, FLOAT fSize) {
    CModelObject *pmo = &(moWeapon.GetAttachmentModel(iAttachObject)->amo_moModelObject);
    CAttachmentModelObject *pamo = pmo->GetAttachmentModel(iAttachFlare);
    pamo->amo_plRelative.pl_OrientationAngle(3) = (rand()*360.0f)/RAND_MAX;
    pmo = &(pamo->amo_moModelObject);
    pmo->StretchModel(FLOAT3D(fSize, fSize, fSize));
  };


  // hide flare
  void HideFlare(CModelObject &moWeapon, INDEX iAttachObject, INDEX iAttachFlare) {
    CModelObject *pmo = &(moWeapon.GetAttachmentModel(iAttachObject)->amo_moModelObject);
    pmo = &(pmo->GetAttachmentModel(iAttachFlare)->amo_moModelObject);
    pmo->StretchModel(FLOAT3D(0, 0, 0));
  };

  void SetFlare(INDEX iFlare, INDEX iAction)
  {
    CPlayerWeapons *pen = this;
    if (iFlare==0) {
      pen->m_iFlare = iAction;
      pen->GetPlayer()->GetPlayerAnimator()->m_iFlare = iAction;
    } else {
      pen->m_iSecondFlare = iAction;
      pen->GetPlayer()->GetPlayerAnimator()->m_iSecondFlare = iAction;
    }
  }

  /*
  // flare attachment
  void ControlFlareAttachment(void) 
  {

    CPlayerWeapons *pen = this;
    // second colt only
    if (m_iCurrentWeapon==WEAPON_DOUBLECOLT) {
      // add flare
      if (pen->m_iSecondFlare==FLARE_ADD) {
        pen->m_iSecondFlare = FLARE_REMOVE;
        ShowFlare(m_moWeaponSecond, COLT_ATTACHMENT_COLT, COLTMAIN_ATTACHMENT_FLARE, 1.0f);
      // remove flare
      } else if (pen->m_iSecondFlare==FLARE_REMOVE) {
        HideFlare(m_moWeaponSecond, COLT_ATTACHMENT_COLT, COLTMAIN_ATTACHMENT_FLARE);
      }
    }

    // add flare
    if (pen->m_iFlare==FLARE_ADD) {
      pen->m_iFlare = FLARE_REMOVE;
      switch(m_iCurrentWeapon) {
        case WEAPON_DOUBLECOLT: case WEAPON_COLT:
          ShowFlare(m_moWeapon, COLT_ATTACHMENT_COLT, COLTMAIN_ATTACHMENT_FLARE, 0.75f);
          break;
        case WEAPON_SINGLESHOTGUN:
          ShowFlare(m_moWeapon, SINGLESHOTGUN_ATTACHMENT_BARRELS, BARRELS_ATTACHMENT_FLARE, 1.0f);
          break;
        case WEAPON_DOUBLESHOTGUN:
          ShowFlare(m_moWeapon, DOUBLESHOTGUN_ATTACHMENT_BARRELS, DSHOTGUNBARRELS_ATTACHMENT_FLARE, 1.75f);
          break;
        case WEAPON_TOMMYGUN:
          ShowFlare(m_moWeapon, TOMMYGUN_ATTACHMENT_BODY, BODY_ATTACHMENT_FLARE, 0.5f);
          break;
        case WEAPON_SNIPER:
          ShowFlare(m_moWeapon, SNIPER_ATTACHMENT_BODY, BODY_ATTACHMENT_FLARE, 0.5f);
          break;
        case WEAPON_MINIGUN:
          ShowFlare(m_moWeapon, MINIGUN_ATTACHMENT_BODY, BODY_ATTACHMENT_FLARE, 1.25f);
          break;
      }
    // remove
    } else if (pen->m_iFlare==FLARE_REMOVE) {
      switch(m_iCurrentWeapon) {
        case WEAPON_DOUBLECOLT: case WEAPON_COLT:
          HideFlare(m_moWeapon, COLT_ATTACHMENT_COLT, COLTMAIN_ATTACHMENT_FLARE);
          break;
        case WEAPON_SINGLESHOTGUN:
          HideFlare(m_moWeapon, SINGLESHOTGUN_ATTACHMENT_BARRELS, BARRELS_ATTACHMENT_FLARE);
          break;
        case WEAPON_DOUBLESHOTGUN:
          HideFlare(m_moWeapon, DOUBLESHOTGUN_ATTACHMENT_BARRELS, DSHOTGUNBARRELS_ATTACHMENT_FLARE);
          break;
        case WEAPON_TOMMYGUN:
          HideFlare(m_moWeapon, TOMMYGUN_ATTACHMENT_BODY, BODY_ATTACHMENT_FLARE);
          break;
        case WEAPON_SNIPER:
          HideFlare(m_moWeapon, SNIPER_ATTACHMENT_BODY, BODY_ATTACHMENT_FLARE);
          break;
        case WEAPON_MINIGUN:
          HideFlare(m_moWeapon, MINIGUN_ATTACHMENT_BODY, BODY_ATTACHMENT_FLARE);
          break;
      }
    } else {
      ASSERT(FALSE);
    }
  };
  */


  // play light animation
  void PlayLightAnim(INDEX iAnim, ULONG ulFlags) {
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    if (pl.m_aoLightAnimation.GetData()!=NULL) {
      pl.m_aoLightAnimation.PlayAnim(iAnim, ulFlags);
    }
  };


  // Set weapon model for current weapon.
  // deleted by seo - 40822
  /*
  void SetCurrentWeaponModel(void) 
  {
    // WARNING !!! ---> Order of attachment must be the same with order in RenderWeaponModel()
    switch (m_iCurrentWeapon) {
      case WEAPON_NONE:
        break;
      // knife
      case WEAPON_KNIFE:
        SetComponents(this, m_moWeapon, MODEL_KNIFE, TEXTURE_HAND, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, KNIFE_ATTACHMENT_KNIFEITEM, MODEL_KNIFEITEM, 
                             TEXTURE_KNIFEITEM, TEX_REFL_BWRIPLES02, TEX_SPEC_WEAK, 0);
        m_moWeapon.PlayAnim(KNIFE_ANIM_WAIT1, 0);
        break;
      // colt
      case WEAPON_DOUBLECOLT: {
        SetComponents(this, m_moWeaponSecond, MODEL_COLT, TEXTURE_HAND, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeaponSecond, COLT_ATTACHMENT_BULLETS, MODEL_COLTBULLETS, TEXTURE_COLTBULLETS, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeaponSecond, COLT_ATTACHMENT_COCK, MODEL_COLTCOCK, TEXTURE_COLTCOCK, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeaponSecond, COLT_ATTACHMENT_COLT, MODEL_COLTMAIN, TEXTURE_COLTMAIN, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        CModelObject &mo = m_moWeaponSecond.GetAttachmentModel(COLT_ATTACHMENT_COLT)->amo_moModelObject;
        AddAttachmentToModel(this, mo, COLTMAIN_ATTACHMENT_FLARE, MODEL_FLARE01, TEXTURE_FLARE01, 0, 0, 0); }
        m_moWeaponSecond.StretchModel(FLOAT3D(-1,1,1));
        m_moWeaponSecond.PlayAnim(COLT_ANIM_WAIT1, 0);
      case WEAPON_COLT: {
        SetComponents(this, m_moWeapon, MODEL_COLT, TEXTURE_HAND, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, COLT_ATTACHMENT_BULLETS, MODEL_COLTBULLETS, TEXTURE_COLTBULLETS, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, COLT_ATTACHMENT_COCK, MODEL_COLTCOCK, TEXTURE_COLTCOCK, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, COLT_ATTACHMENT_COLT, MODEL_COLTMAIN, TEXTURE_COLTMAIN, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        CModelObject &mo = m_moWeapon.GetAttachmentModel(COLT_ATTACHMENT_COLT)->amo_moModelObject;
        AddAttachmentToModel(this, mo, COLTMAIN_ATTACHMENT_FLARE, MODEL_FLARE01, TEXTURE_FLARE01, 0, 0, 0);
        m_moWeapon.PlayAnim(COLT_ANIM_WAIT1, 0);
        break; }
      case WEAPON_SINGLESHOTGUN: {
        SetComponents(this, m_moWeapon, MODEL_SINGLESHOTGUN, TEXTURE_HAND, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, SINGLESHOTGUN_ATTACHMENT_BARRELS, MODEL_SS_BARRELS, TEXTURE_SS_BARRELS, TEX_REFL_DARKMETAL, TEX_SPEC_WEAK, 0);
        AddAttachmentToModel(this, m_moWeapon, SINGLESHOTGUN_ATTACHMENT_HANDLE, MODEL_SS_HANDLE, TEXTURE_SS_HANDLE, TEX_REFL_DARKMETAL, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, SINGLESHOTGUN_ATTACHMENT_SLIDER, MODEL_SS_SLIDER, TEXTURE_SS_BARRELS, TEX_REFL_DARKMETAL, TEX_SPEC_MEDIUM, 0);
        CModelObject &mo = m_moWeapon.GetAttachmentModel(SINGLESHOTGUN_ATTACHMENT_BARRELS)->amo_moModelObject;
        AddAttachmentToModel(this, mo, BARRELS_ATTACHMENT_FLARE, MODEL_FLARE01, TEXTURE_FLARE01, 0, 0, 0);
        m_moWeapon.PlayAnim(SINGLESHOTGUN_ANIM_WAIT1, 0);
        break; }
      case WEAPON_DOUBLESHOTGUN: {
        SetComponents(this, m_moWeapon, MODEL_DOUBLESHOTGUN, TEXTURE_HAND, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, DOUBLESHOTGUN_ATTACHMENT_BARRELS, MODEL_DS_BARRELS, TEXTURE_DS_BARRELS, TEX_REFL_BWRIPLES01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, DOUBLESHOTGUN_ATTACHMENT_HANDLE, MODEL_DS_HANDLE, TEXTURE_DS_HANDLE, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, DOUBLESHOTGUN_ATTACHMENT_SWITCH, MODEL_DS_SWITCH, TEXTURE_DS_SWITCH, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, DOUBLESHOTGUN_ATTACHMENT_AMMO, MODEL_DS_AMMO, TEXTURE_DS_AMMO, 0 ,0, 0);
        SetComponents(this, m_moWeaponSecond, MODEL_DS_HANDWITHAMMO, TEXTURE_HAND, 0, 0, 0);
        CModelObject &mo = m_moWeapon.GetAttachmentModel(DOUBLESHOTGUN_ATTACHMENT_BARRELS)->amo_moModelObject;
        AddAttachmentToModel(this, mo, DSHOTGUNBARRELS_ATTACHMENT_FLARE, MODEL_FLARE01, TEXTURE_FLARE01, 0, 0, 0);
        m_moWeaponSecond.StretchModel(FLOAT3D(1,1,1));
        m_moWeapon.PlayAnim(DOUBLESHOTGUN_ANIM_WAIT1, 0);
        break; }
      case WEAPON_TOMMYGUN: {
        SetComponents(this, m_moWeapon, MODEL_TOMMYGUN, TEXTURE_HAND, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, TOMMYGUN_ATTACHMENT_BODY, MODEL_TG_BODY, TEXTURE_TG_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, TOMMYGUN_ATTACHMENT_SLIDER, MODEL_TG_SLIDER, TEXTURE_TG_BODY, 0, TEX_SPEC_MEDIUM, 0);
        CModelObject &mo = m_moWeapon.GetAttachmentModel(TOMMYGUN_ATTACHMENT_BODY)->amo_moModelObject;
        AddAttachmentToModel(this, mo, BODY_ATTACHMENT_FLARE, MODEL_FLARE01, TEXTURE_FLARE01, 0, 0, 0);
        break; }
      case WEAPON_SNIPER: {
        SetComponents(this, m_moWeapon, MODEL_SNIPER, TEXTURE_SNIPER_BODY, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, SNIPER_ATTACHMENT_BODY, MODEL_SNIPER_BODY, TEXTURE_SNIPER_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        CModelObject &mo = m_moWeapon.GetAttachmentModel(SNIPER_ATTACHMENT_BODY)->amo_moModelObject;
        AddAttachmentToModel(this, mo, BODY_ATTACHMENT_FLARE, MODEL_FLARE01, TEXTURE_FLARE01, 0, 0, 0);
        break; }
      case WEAPON_MINIGUN: {
        SetComponents(this, m_moWeapon, MODEL_MINIGUN, TEXTURE_HAND, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, MINIGUN_ATTACHMENT_BARRELS, MODEL_MG_BARRELS, TEXTURE_MG_BARRELS, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, MINIGUN_ATTACHMENT_BODY, MODEL_MG_BODY, TEXTURE_MG_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, MINIGUN_ATTACHMENT_ENGINE, MODEL_MG_ENGINE, TEXTURE_MG_BARRELS, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        CModelObject &mo = m_moWeapon.GetAttachmentModel(MINIGUN_ATTACHMENT_BODY)->amo_moModelObject;
        AddAttachmentToModel(this, mo, BODY_ATTACHMENT_FLARE, MODEL_FLARE01, TEXTURE_FLARE01, 0, 0, 0);
        break; }
      case WEAPON_ROCKETLAUNCHER:
        SetComponents(this, m_moWeapon, MODEL_ROCKETLAUNCHER, TEXTURE_RL_BODY, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, ROCKETLAUNCHER_ATTACHMENT_BODY, MODEL_RL_BODY, TEXTURE_RL_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, ROCKETLAUNCHER_ATTACHMENT_ROTATINGPART, MODEL_RL_ROTATINGPART, TEXTURE_RL_ROTATINGPART, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, ROCKETLAUNCHER_ATTACHMENT_ROCKET1, MODEL_RL_ROCKET, TEXTURE_RL_ROCKET, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, ROCKETLAUNCHER_ATTACHMENT_ROCKET2, MODEL_RL_ROCKET, TEXTURE_RL_ROCKET, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, ROCKETLAUNCHER_ATTACHMENT_ROCKET3, MODEL_RL_ROCKET, TEXTURE_RL_ROCKET, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        break;
      case WEAPON_GRENADELAUNCHER:
        SetComponents(this, m_moWeapon, MODEL_GRENADELAUNCHER, TEXTURE_GL_BODY, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, GRENADELAUNCHER_ATTACHMENT_BODY, MODEL_GL_BODY, TEXTURE_GL_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, GRENADELAUNCHER_ATTACHMENT_MOVING_PART, MODEL_GL_MOVINGPART, TEXTURE_GL_MOVINGPART, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, GRENADELAUNCHER_ATTACHMENT_GRENADE, MODEL_GL_GRENADE, TEXTURE_GL_MOVINGPART, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        break;
      /*
      case WEAPON_PIPEBOMB:
        SetComponents(this, m_moWeapon, MODEL_PIPEBOMB_HAND, TEXTURE_HAND, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, HANDWITHBOMB_ATTACHMENT_BOMB, MODEL_PB_BOMB, TEXTURE_PB_BOMB, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        SetComponents(this, m_moWeaponSecond, MODEL_PIPEBOMB_STICK, TEXTURE_HAND, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeaponSecond, HANDWITHSTICK_ATTACHMENT_STICK, MODEL_PB_STICK, TEXTURE_PB_STICK, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeaponSecond, HANDWITHSTICK_ATTACHMENT_SHIELD, MODEL_PB_SHIELD, TEXTURE_PB_STICK, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeaponSecond, HANDWITHSTICK_ATTACHMENT_BUTTON, MODEL_PB_BUTTON, TEXTURE_PB_STICK, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        m_moWeaponSecond.StretchModel(FLOAT3D(1,1,1));
        break;
        */
      /*case WEAPON_FLAMER:
        SetComponents(this, m_moWeapon, MODEL_FLAMER, TEXTURE_HAND, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, FLAMER_ATTACHMENT_BODY, MODEL_FL_BODY, TEXTURE_FL_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, FLAMER_ATTACHMENT_FUEL, MODEL_FL_RESERVOIR, TEXTURE_FL_FUELRESERVOIR, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, FLAMER_ATTACHMENT_FLAME, MODEL_FL_FLAME, TEXTURE_FL_FLAME, 0, 0, 0);
        break;
      case WEAPON_CHAINSAW: {
        SetComponents(this, m_moWeapon, MODEL_CHAINSAW, TEXTURE_HAND, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, CHAINSAW_ATTACHMENT_CHAINSAW, MODEL_CS_BODY, TEXTURE_CS_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, CHAINSAW_ATTACHMENT_BLADE, MODEL_CS_BLADE, TEXTURE_CS_BLADE, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        CModelObject *pmo;
        pmo = &(m_moWeapon.GetAttachmentModel(CHAINSAW_ATTACHMENT_BLADE)->amo_moModelObject);
        AddAttachmentToModel(this, *pmo, BLADE_ATTACHMENT_TEETH, MODEL_CS_TEETH, TEXTURE_CS_TEETH, 0, 0, 0);
        break; }
      case WEAPON_LASER:
        SetComponents(this, m_moWeapon, MODEL_LASER, TEXTURE_HAND, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, LASER_ATTACHMENT_BODY, MODEL_LS_BODY, TEXTURE_LS_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, LASER_ATTACHMENT_LEFTUP,    MODEL_LS_BARREL, TEXTURE_LS_BARREL, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, LASER_ATTACHMENT_LEFTDOWN,  MODEL_LS_BARREL, TEXTURE_LS_BARREL, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, LASER_ATTACHMENT_RIGHTUP,   MODEL_LS_BARREL, TEXTURE_LS_BARREL, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, LASER_ATTACHMENT_RIGHTDOWN, MODEL_LS_BARREL, TEXTURE_LS_BARREL, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        break;
/*
      case WEAPON_GHOSTBUSTER:
        SetComponents(this, m_moWeapon, MODEL_GHOSTBUSTER, TEXTURE_HAND, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, GHOSTBUSTER_ATTACHMENT_BODY, MODEL_GB_BODY, TEXTURE_GB_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, GHOSTBUSTER_ATTACHMENT_ROTATOR, MODEL_GB_ROTATOR, TEXTURE_GB_ROTATOR, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddAttachmentToModel(this, m_moWeapon, GHOSTBUSTER_ATTACHMENT_EFFECT01, MODEL_GB_EFFECT1, TEXTURE_GB_LIGHTNING, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, GHOSTBUSTER_ATTACHMENT_EFFECT02, MODEL_GB_EFFECT1, TEXTURE_GB_LIGHTNING, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, GHOSTBUSTER_ATTACHMENT_EFFECT03, MODEL_GB_EFFECT1, TEXTURE_GB_LIGHTNING, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, GHOSTBUSTER_ATTACHMENT_EFFECT04, MODEL_GB_EFFECT1, TEXTURE_GB_LIGHTNING, 0, 0, 0);
        CModelObject *pmo;
        pmo = &(m_moWeapon.GetAttachmentModel(GHOSTBUSTER_ATTACHMENT_EFFECT01)->amo_moModelObject);
        AddAttachmentToModel(this, *pmo, EFFECT01_ATTACHMENT_FLARE, MODEL_GB_EFFECT1FLARE, TEXTURE_GB_FLARE, 0, 0, 0);
        pmo = &(m_moWeapon.GetAttachmentModel(GHOSTBUSTER_ATTACHMENT_EFFECT02)->amo_moModelObject);
        AddAttachmentToModel(this, *pmo, EFFECT01_ATTACHMENT_FLARE, MODEL_GB_EFFECT1FLARE, TEXTURE_GB_FLARE, 0, 0, 0);
        pmo = &(m_moWeapon.GetAttachmentModel(GHOSTBUSTER_ATTACHMENT_EFFECT03)->amo_moModelObject);
        AddAttachmentToModel(this, *pmo, EFFECT01_ATTACHMENT_FLARE, MODEL_GB_EFFECT1FLARE, TEXTURE_GB_FLARE, 0, 0, 0);
        pmo = &(m_moWeapon.GetAttachmentModel(GHOSTBUSTER_ATTACHMENT_EFFECT04)->amo_moModelObject);
        AddAttachmentToModel(this, *pmo, EFFECT01_ATTACHMENT_FLARE, MODEL_GB_EFFECT1FLARE, TEXTURE_GB_FLARE, 0, 0, 0);
        break;
        */
      /*case WEAPON_IRONCANNON:
//      case WEAPON_NUKECANNON:
        SetComponents(this, m_moWeapon, MODEL_CANNON, TEXTURE_CANNON, 0, 0, 0);
        AddAttachmentToModel(this, m_moWeapon, CANNON_ATTACHMENT_BODY, MODEL_CN_BODY, TEXTURE_CANNON, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
//        AddAttachmentToModel(this, m_moWeapon, CANNON_ATTACHMENT_NUKEBOX, MODEL_CN_NUKEBOX, TEXTURE_CANNON, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
//        AddAttachmentToModel(this, m_moWeapon, CANNON_ATTACHMENT_LIGHT, MODEL_CN_LIGHT, TEXTURE_CANNON, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        break;
    }
  };
  */



  /*
   *  >>>---  ROTATE MINIGUN  ---<<<
   */
/*
  void RotateMinigun(void) {
    ANGLE aAngle = Lerp(m_aMiniGunLast, m_aMiniGun, _pTimer->GetLerpFactor());
    // rotate minigun barrels
    CAttachmentModelObject *amo = m_moWeapon.GetAttachmentModel(MINIGUN_ATTACHMENT_BARRELS);
    amo->amo_plRelative.pl_OrientationAngle(3) = aAngle;
  };
  */



  /*
   *  >>>---  SUPPORT (COMMON) FUNCTIONS  ---<<<
   */

  // calc weapon position for 3rd person view
  void CalcWeaponPosition3rdPersonView(FLOAT3D vPos, CPlacement3D &plPos, BOOL bResetZ) {
    plPos.pl_OrientationAngle = ANGLE3D(0, 0, 0);
    // weapon handle
    if (!m_bMirrorFire) {
      plPos.pl_PositionVector = FLOAT3D( wpn_fX[m_iCurrentWeapon], wpn_fY[m_iCurrentWeapon],
                                         wpn_fZ[m_iCurrentWeapon]);
    } else {
      plPos.pl_PositionVector = FLOAT3D( -wpn_fX[m_iCurrentWeapon], wpn_fY[m_iCurrentWeapon],
                                          wpn_fZ[m_iCurrentWeapon]);
    }
    // weapon offset
    if (!m_bMirrorFire) {
      plPos.RelativeToAbsoluteSmooth(CPlacement3D(vPos, ANGLE3D(0, 0, 0)));
    } else {
      plPos.RelativeToAbsoluteSmooth(CPlacement3D(vPos, ANGLE3D(0, 0, 0)));
    }
    plPos.pl_PositionVector(1) *= SinFast(wpn_fFOV[m_iCurrentWeapon]/2) / SinFast(90.0f/2);
    plPos.pl_PositionVector(2) *= SinFast(wpn_fFOV[m_iCurrentWeapon]/2) / SinFast(90.0f/2);
    plPos.pl_PositionVector(3) *= SinFast(wpn_fFOV[m_iCurrentWeapon]/2) / SinFast(90.0f/2);

    if (bResetZ) {
      plPos.pl_PositionVector(3) = 0.0f;
    }

    // player view and absolute position
    CPlacement3D plView = ((CPlayer &)*m_penPlayer).en_plViewpoint;
    plView.pl_PositionVector(2)= 1.25118f;
    plPos.RelativeToAbsoluteSmooth(plView);
    plPos.RelativeToAbsoluteSmooth(m_penPlayer->GetPlacement());
  };

  // calc weapon position
  void CalcWeaponPosition(FLOAT3D vPos, CPlacement3D &plPos, BOOL bResetZ) {
    plPos.pl_OrientationAngle = ANGLE3D(0, 0, 0);
    // weapon handle
    if (!m_bMirrorFire) {
      plPos.pl_PositionVector = FLOAT3D( wpn_fX[m_iCurrentWeapon], wpn_fY[m_iCurrentWeapon],
                                         wpn_fZ[m_iCurrentWeapon]);
	  /*
      if (m_bSniping) {
        plPos.pl_PositionVector = FLOAT3D( 0.0f, 0.0f, 0.0f );
      }
	  */
    } else {
      plPos.pl_PositionVector = FLOAT3D( -wpn_fX[m_iCurrentWeapon], wpn_fY[m_iCurrentWeapon],
                                          wpn_fZ[m_iCurrentWeapon]);
    }
    // weapon offset
    if (!m_bMirrorFire) {
      plPos.RelativeToAbsoluteSmooth(CPlacement3D(vPos, ANGLE3D(0, 0, 0)));
    } else {
      plPos.RelativeToAbsoluteSmooth(CPlacement3D(vPos, ANGLE3D(0, 0, 0)));
    }
    plPos.pl_PositionVector(1) *= SinFast(wpn_fFOV[m_iCurrentWeapon]/2) / SinFast(90.0f/2);
    plPos.pl_PositionVector(2) *= SinFast(wpn_fFOV[m_iCurrentWeapon]/2) / SinFast(90.0f/2);
    plPos.pl_PositionVector(3) *= SinFast(wpn_fFOV[m_iCurrentWeapon]/2) / SinFast(90.0f/2);

    if (bResetZ) {
      plPos.pl_PositionVector(3) = 0.0f;
    }

    // player view and absolute position
    CPlacement3D plView = ((CPlayer &)*m_penPlayer).en_plViewpoint;
    plView.pl_PositionVector(2)+= ((CPlayerAnimator&)*((CPlayer &)*m_penPlayer).m_penAnimator).
      m_fEyesYOffset;
    plPos.RelativeToAbsoluteSmooth(plView);
    plPos.RelativeToAbsoluteSmooth(m_penPlayer->GetPlacement());
  };

  // calc lerped weapon position
  void CalcLerpedWeaponPosition(FLOAT3D vPos, CPlacement3D &plPos, BOOL bResetZ)
  {
    plPos.pl_OrientationAngle = ANGLE3D(0, 0, 0);
    // weapon handle
    if (!m_bMirrorFire) {
      plPos.pl_PositionVector = FLOAT3D( wpn_fX[m_iCurrentWeapon], wpn_fY[m_iCurrentWeapon],
                                         wpn_fZ[m_iCurrentWeapon]);
      /*
	  if (m_bSniping) {
        plPos.pl_PositionVector = FLOAT3D( 0.0f, 0.0f, 0.0f );
      }
	  */
    } else {
      plPos.pl_PositionVector = FLOAT3D( -wpn_fX[m_iCurrentWeapon], wpn_fY[m_iCurrentWeapon],
                                          wpn_fZ[m_iCurrentWeapon]);
    }
    // weapon offset
    if (!m_bMirrorFire) {
      plPos.RelativeToAbsoluteSmooth(CPlacement3D(vPos, ANGLE3D(0, 0, 0)));
    } else {
      plPos.RelativeToAbsoluteSmooth(CPlacement3D(vPos, ANGLE3D(0, 0, 0)));
    }
    plPos.pl_PositionVector(1) *= SinFast(wpn_fFOV[m_iCurrentWeapon]/2) / SinFast(90.0f/2);
    plPos.pl_PositionVector(2) *= SinFast(wpn_fFOV[m_iCurrentWeapon]/2) / SinFast(90.0f/2);
    plPos.pl_PositionVector(3) *= SinFast(wpn_fFOV[m_iCurrentWeapon]/2) / SinFast(90.0f/2);

    if (bResetZ) {
      plPos.pl_PositionVector(3) = 0.0f;
    }

    // player view and absolute position
    CPlacement3D plRes;
    GetPlayer()->GetLerpedWeaponPosition( plPos.pl_PositionVector, plRes);
    plPos=plRes;
  };

  // calc weapon position
  void CalcWeaponPositionImprecise (FLOAT3D vPos, CPlacement3D &plPos, BOOL bResetZ, FLOAT fImprecissionAngle) {
    plPos.pl_OrientationAngle = ANGLE3D((FRnd()-0.5f)*fImprecissionAngle, (FRnd()-0.5f)*fImprecissionAngle, 0);
    // weapon handle
    if (!m_bMirrorFire) {
      plPos.pl_PositionVector = FLOAT3D( wpn_fX[m_iCurrentWeapon], wpn_fY[m_iCurrentWeapon], wpn_fZ[m_iCurrentWeapon]);
	  /*
      if (m_bSniping) {
        plPos.pl_PositionVector = FLOAT3D( 0.0f, 0.0f, 0.0f );
      }
	  */
    } 
	else
	  {
      plPos.pl_PositionVector = FLOAT3D( -wpn_fX[m_iCurrentWeapon], wpn_fY[m_iCurrentWeapon],
                                          wpn_fZ[m_iCurrentWeapon]);
    }
    // weapon offset
    if (!m_bMirrorFire) {
      plPos.RelativeToAbsoluteSmooth(CPlacement3D(vPos, ANGLE3D(0, 0, 0)));
    } else {
      plPos.RelativeToAbsoluteSmooth(CPlacement3D(vPos, ANGLE3D(0, 0, 0)));
    }
    plPos.pl_PositionVector(1) *= SinFast(wpn_fFOV[m_iCurrentWeapon]/2) / SinFast(90.0f/2);
    plPos.pl_PositionVector(2) *= SinFast(wpn_fFOV[m_iCurrentWeapon]/2) / SinFast(90.0f/2);
    plPos.pl_PositionVector(3) *= SinFast(wpn_fFOV[m_iCurrentWeapon]/2) / SinFast(90.0f/2);

    if (bResetZ) {
      plPos.pl_PositionVector(3) = 0.0f;
    }

    // player view and absolute position
    CPlacement3D plView = ((CPlayer &)*m_penPlayer).en_plViewpoint;
    plView.pl_PositionVector(2)+= ((CPlayerAnimator&)*((CPlayer &)*m_penPlayer).m_penAnimator).
      m_fEyesYOffset;
    plPos.RelativeToAbsoluteSmooth(plView);
    plPos.RelativeToAbsoluteSmooth(m_penPlayer->GetPlacement());
  };


  // setup 3D sound parameters
  void Setup3DSoundParameters(void)
  {
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    // initialize sound 3D parameters
    pl.m_soWeapon0.Set3DParameters( 50.0f, 5.0f, 1.0f, 1.0f);
    pl.m_soWeapon1.Set3DParameters( 50.0f, 5.0f, 1.0f, 1.0f);
    pl.m_soWeapon2.Set3DParameters( 50.0f, 5.0f, 1.0f, 1.0f);
    pl.m_soWeapon3.Set3DParameters( 50.0f, 5.0f, 1.0f, 1.0f);
    pl.m_soWeaponAmbient.Set3DParameters( 30.0f, 3.0f, 0.0f, 1.0f);
  };


  /*
   *  >>>---  FIRE FUNCTIONS  ---<<<
   */

  // cut in front of you with knife
	//-----------------------------------------------------------------------------
	// Purpose: 
	// Input  : fX - 
	//			fY - 
	//			fRange - 
	//			fWide - 
	//			fThickness - 
	//			fDamage - 
	// Output : 	BOOL
	//-----------------------------------------------------------------------------
  BOOL CutWithKnife(FLOAT fX, FLOAT fY, FLOAT fRange, FLOAT fWide, FLOAT fThickness, FLOAT fDamage) 
  {
    // knife start position
    CPlacement3D plKnife;
    CalcWeaponPosition(FLOAT3D(fX, fY, 0), plKnife, TRUE);

    // create a set of rays to test
    const FLOAT3D &vBase = plKnife.pl_PositionVector;
    FLOATmatrix3D m;
    MakeRotationMatrixFast(m, plKnife.pl_OrientationAngle);
    FLOAT3D vRight = m.GetColumn(1)*fWide;
    FLOAT3D vUp    = m.GetColumn(2)*fWide;
    FLOAT3D vFront = -m.GetColumn(3)*fRange;

    FLOAT3D vDest[5];
    vDest[0] = vBase+vFront;
    vDest[1] = vBase+vFront+vUp;
    vDest[2] = vBase+vFront-vUp;
    vDest[3] = vBase+vFront+vRight;
    vDest[4] = vBase+vFront-vRight;

    CEntity *penClosest = NULL;
    FLOAT fDistance = UpperLimit(0.0f);
    FLOAT3D vHit;
    FLOAT3D vDir;
    // for each ray
		for (INDEX i=0; i<5; i++) 
		{
      // cast a ray to find if any model
      CCastRay crRay( m_penPlayer, vBase, vDest[i]);
      crRay.cr_bHitTranslucentPortals = FALSE;
      crRay.cr_fTestR = fThickness;
      crRay.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;
      GetWorld()->CastRay(crRay);
      
      // if hit something
			if (crRay.cr_penHit!=NULL /*&& crRay.cr_penHit->GetRenderType()==RT_MODEL*/ && crRay.cr_fHitDistance<fDistance) 
			{
        penClosest = crRay.cr_penHit;
        fDistance = crRay.cr_fHitDistance;
        vDir = vDest[i]-vBase;
        vHit = crRay.cr_vHit;
        
				if (i==0) 
				{
          if(crRay.cr_penHit->GetRenderType()==RT_BRUSH)
          {
            INDEX iSurfaceType=crRay.cr_pbpoBrushPolygon->bpo_bppProperties.bpp_ubSurfaceType;
            EffectParticlesType eptType=GetParticleEffectTypeForSurface(iSurfaceType);
            
            FLOAT3D vNormal=crRay.cr_pbpoBrushPolygon->bpo_pbplPlane->bpl_plAbsolute;
            FLOAT3D vReflected = vDir-vNormal*(2.0f*(vNormal%vDir));
            ((CPlayer&)*m_penPlayer).AddBulletSpray( vBase+vFront, eptType, vReflected);
          }
          else if (crRay.cr_penHit->GetRenderType()==RT_MODEL || crRay.cr_penHit->GetRenderType()==RT_SKAMODEL)
          {
            BOOL bRender=TRUE;
            FLOAT3D vSpillDir=-((CPlayer&)*m_penPlayer).en_vGravityDir*0.5f;
            SprayParticlesType sptType=SPT_NONE;
            COLOR colParticles=C_WHITE|CT_OPAQUE;
            FLOAT fPower=4.0f;
            if( IsOfClass( crRay.cr_penHit, &CModelHolder2_DLLClass))
            {
              bRender=FALSE;
              CModelDestruction *penDestruction = ((CModelHolder2&)*crRay.cr_penHit).GetDestruction();
              if( penDestruction!=NULL)
              {
                bRender=TRUE;
                sptType= penDestruction->m_sptType;
              }
              CModelHolder2 *pmh2=(CModelHolder2*)crRay.cr_penHit;
              colParticles=pmh2->m_colBurning;
            }
            if( IsOfClass( crRay.cr_penHit, &CModelHolder3_DLLClass))
            {
              bRender=FALSE;
              CModelDestruction *penDestruction = ((CModelHolder3&)*crRay.cr_penHit).GetDestruction();
              if( penDestruction!=NULL)
              {
                bRender=TRUE;
                sptType= penDestruction->m_sptType;
              }
              CModelHolder3 *pmh3=(CModelHolder3*)crRay.cr_penHit;
              colParticles=pmh3->m_colBurning;
            }

            if(bRender)
            {
              FLOATaabbox3D boxCutted;
              if (crRay.cr_penHit->GetRenderType()==RT_MODEL) {                
                crRay.cr_penHit->en_pmoModelObject->GetCurrentFrameBBox( boxCutted);                
              } else if (crRay.cr_penHit->GetRenderType()==RT_SKAMODEL) {
                INDEX iColBox = crRay.cr_penHit->GetModelInstance()->GetCurrentColisionBoxIndex();
                ColisionBox cb = crRay.cr_penHit->GetModelInstance()->GetColisionBox(iColBox);
                boxCutted = FLOATaabbox3D(cb.Min(),cb.Max());             
              }

              ((CPlayer&)*m_penPlayer).AddGoreSpray( vBase+vFront, vHit, sptType,
                vSpillDir, boxCutted, fPower, colParticles);
            }
          }
          // don't search any more
          break;
        }        
      }
    }

    // if any model hit
		// 칼의 Ray에 먼가 부딫히면...
		if (penClosest!=NULL) 
		{
      // in deathmatches check for backstab
      if( !(GetSP()->sp_bCooperative) && IsOfClass( penClosest, &CPlayer_DLLClass)) {
        FLOAT3D vToTarget = penClosest->GetPlacement().pl_PositionVector - m_penPlayer->GetPlacement().pl_PositionVector;
        FLOAT3D vTargetHeading = FLOAT3D(0.0, 0.0, -1.0f)*penClosest->GetRotationMatrix();
        vToTarget.Normalize(); vTargetHeading.Normalize();
        if (vToTarget%vTargetHeading>0.65f/*CosFast(50.0f)*/)
        {
          //PrintCenterMessage(this, m_penPlayer, TRANS("Backstab!"), 4.0f, MSS_NONE);
          fDamage *= 4.0f;
        }
      }
      const FLOAT fDamageMul = GetSeriousDamageMultiplier(m_penPlayer);
      InflictDirectDamage(penClosest, m_penPlayer, DMT_CLOSERANGE, fDamage*fDamageMul, vHit, vDir);
      return TRUE;
    }
    return FALSE;
  };

  
  // cut in front of you with the chainsaw
  BOOL CutWithChainsaw(FLOAT fX, FLOAT fY, FLOAT fRange, FLOAT fWide, FLOAT fThickness, FLOAT fDamage) 
  {
    // knife start position
    CPlacement3D plKnife;
    CalcWeaponPosition(FLOAT3D(fX, fY, 0), plKnife, TRUE);

    // create a set of rays to test
    const FLOAT3D &vBase = plKnife.pl_PositionVector;
    FLOATmatrix3D m;
    MakeRotationMatrixFast(m, plKnife.pl_OrientationAngle);
    FLOAT3D vRight = m.GetColumn(1)*fWide;
    FLOAT3D vUp    = m.GetColumn(2)*fWide;
    FLOAT3D vFront = -m.GetColumn(3)*fRange;

    FLOAT3D vDest[3];
    vDest[0] = vBase+vFront;
    vDest[1] = vBase+vFront+vRight;
    vDest[2] = vBase+vFront-vRight;
    
    CEntity *penClosest = NULL;
    FLOAT fDistance = UpperLimit(0.0f);
    FLOAT3D vHit;
    FLOAT3D vDir;
    // for each ray
    for (INDEX i=0; i<3; i++) {
      // cast a ray to find if any model
      CCastRay crRay( m_penPlayer, vBase, vDest[i]);
      crRay.cr_bHitTranslucentPortals = FALSE;
      crRay.cr_fTestR = fThickness;
      crRay.cr_ttHitModels = CCastRay::TT_COLLISIONBOX;
      GetWorld()->CastRay(crRay);

      // if hit something
      if (crRay.cr_penHit!=NULL)
      {
        penClosest = crRay.cr_penHit;
        fDistance = crRay.cr_fHitDistance;
        vDir = vDest[i]-vBase;
        vDir.Normalize();
        vHit = crRay.cr_vHit;

        if(i==0)
        {
          if(crRay.cr_penHit->GetRenderType()==RT_BRUSH)
          {
            INDEX iSurfaceType=crRay.cr_pbpoBrushPolygon->bpo_bppProperties.bpp_ubSurfaceType;
            EffectParticlesType eptType=GetParticleEffectTypeForSurface(iSurfaceType);

            FLOAT3D vNormal=crRay.cr_pbpoBrushPolygon->bpo_pbplPlane->bpl_plAbsolute;
            FLOAT3D vReflected = vDir-vNormal*(2.0f*(vNormal%vDir));
            FLOAT3D vPos = vBase+vFront;
            ((CPlayer&)*m_penPlayer).AddBulletSpray( vPos, eptType, vReflected);
            EAddBulletSpray eabs;
            eabs.vPos = vPos;
            eabs.eptType = eptType;
            eabs.vReflected = vReflected;
            ((CPlayer&)*m_penPlayer).SendEvent(eabs,TRUE);


            // shake view
            ((CPlayer&)*m_penPlayer).m_fChainShakeStrength = 0.85f;
            ((CPlayer&)*m_penPlayer).m_fChainShakeFreqMod = 1.0f;
            ((CPlayer&)*m_penPlayer).m_tmChainShakeEnd = _pTimer->CurrentTick() + CHAINSAW_UPDATETIME*1.5f;

          }
          else if(crRay.cr_penHit->GetRenderType()==RT_MODEL || crRay.cr_penHit->GetRenderType()==RT_SKAMODEL)
          {
            BOOL bRender=TRUE;
            FLOAT3D vSpillDir=-((CPlayer&)*m_penPlayer).en_vGravityDir*0.5f;
            SprayParticlesType sptType=SPT_BLOOD;
            COLOR colParticles=C_WHITE|CT_OPAQUE;
            if( !IsDerivedFromClass( crRay.cr_penHit, &CEnemyBase_DLLClass)) {
              sptType=SPT_NONE;
            }
            FLOAT fPower=4.0f;
            //if( IsOfClass( crRay.cr_penHit, &CBoneman_DLLClass))   {sptType=SPT_BONES; fPower=6.0f;}
			/*
            if( IsOfClass( crRay.cr_penHit, &CGizmo_DLLClass) ||
                IsOfClass( crRay.cr_penHit, &CBeast_DLLClass))     {sptType=SPT_SLIME; fPower=4.0f;}
				*/
            //if( IsOfClass( crRay.cr_penHit, &CWoman_DLLClass))     {sptType=SPT_FEATHER; fPower=3.0f;}
            //if( IsOfClass( crRay.cr_penHit, &CElemental_DLLClass)) {sptType=SPT_LAVA_STONES; fPower=3.0f;}
            //if( IsOfClass( crRay.cr_penHit, &CWalker_DLLClass))    {sptType=SPT_ELECTRICITY_SPARKS; fPower=30.0f;}
            //if( IsOfClass( crRay.cr_penHit, &CAirElemental_DLLClass))    {sptType=SPT_AIRSPOUTS; fPower=6.0f;}
            //if( IsOfClass( crRay.cr_penHit, &CCannonRotating_DLLClass) ||
            //    IsOfClass( crRay.cr_penHit, &CCannonStatic_DLLClass)) {sptType=SPT_WOOD;}

            if( IsOfClass( crRay.cr_penHit, &CModelHolder2_DLLClass)) 
            {
              bRender=FALSE;
              CModelDestruction *penDestruction = ((CModelHolder2&)*crRay.cr_penHit).GetDestruction();
              CModelHolder2 *pmh2=(CModelHolder2*)crRay.cr_penHit;
              colParticles=pmh2->m_colBurning;
              if( penDestruction!=NULL)
              {
                bRender=TRUE;
                sptType= penDestruction->m_sptType;
                if(sptType==SPT_COLOREDSTONE)
                {
                  colParticles=MulColors(colParticles,penDestruction->m_colParticles);
                }
              }            
            }
            if( IsOfClass( crRay.cr_penHit, &CModelHolder3_DLLClass))
            {
              bRender=FALSE;
              CModelDestruction *penDestruction = ((CModelHolder3&)*crRay.cr_penHit).GetDestruction();
              CModelHolder3 *pmh3=(CModelHolder3*)crRay.cr_penHit;
              colParticles=pmh3->m_colBurning;
              if( penDestruction!=NULL)
              {
                bRender=TRUE;
                sptType= penDestruction->m_sptType;
                if(sptType==SPT_COLOREDSTONE)
                {
                  colParticles=MulColors(colParticles,penDestruction->m_colParticles);
                }
              }
            }
            FLOATaabbox3D boxCutted=FLOATaabbox3D(FLOAT3D(0,0,0),FLOAT3D(1,1,1));
            if(bRender && m_tmLastChainsawSpray+0.2f<_pTimer->CurrentTick())
            {
              // MODEL OBJECTS
              if (crRay.cr_penHit->GetRenderType()==RT_MODEL) {
                crRay.cr_penHit->en_pmoModelObject->GetCurrentFrameBBox( boxCutted);                
              // OR MODEL INSTANCES
              } else if (crRay.cr_penHit->GetRenderType()==RT_SKAMODEL) {
                INDEX iColBox = crRay.cr_penHit->GetModelInstance()->GetCurrentColisionBoxIndex();
                ColisionBox cb = crRay.cr_penHit->GetModelInstance()->GetColisionBox(iColBox);
                boxCutted = FLOATaabbox3D(cb.Min(),cb.Max());
              }
              ((CPlayer&)*m_penPlayer).AddGoreSpray( vBase+vFront, vHit, sptType,
                  vSpillDir, boxCutted, fPower, colParticles);
              EAddGoreSpray eags;
              eags.iEntityHit = crRay.cr_penHit->en_ulID;
              eags.vPos = vBase+vFront;
              eags.vHit = vHit;
              eags.sptType = sptType;
              eags.colParticles = colParticles;
              ((CPlayer&)*m_penPlayer).SendEvent(eags,TRUE);

              m_tmLastChainsawSpray = _pTimer->CurrentTick();
            }

            // shake view
            ((CPlayer&)*m_penPlayer).m_fChainShakeStrength = 1.1f;
            ((CPlayer&)*m_penPlayer).m_fChainShakeFreqMod = 1.0f;
            ((CPlayer&)*m_penPlayer).m_tmChainShakeEnd = _pTimer->CurrentTick() + CHAINSAW_UPDATETIME*1.5f;

          }
        }

        if( (crRay.cr_penHit->GetRenderType()==RT_MODEL || crRay.cr_penHit->GetRenderType()==RT_SKAMODEL) &&
            crRay.cr_fHitDistance<=fDistance)
        {
          // if this is primary ray
          if (i==0)
          {
            // don't search any more
            break;
          }
        }
      } else {
        // because we're firing, add just a slight shake
        ((CPlayer&)*m_penPlayer).m_fChainShakeStrength = 0.1f;
        ((CPlayer&)*m_penPlayer).m_fChainShakeFreqMod = 1.0f;
        ((CPlayer&)*m_penPlayer).m_tmChainShakeEnd = _pTimer->CurrentTick() + CHAINSAW_UPDATETIME*1.5f;
      }
    }
    // if any model hit
    if (penClosest!=NULL) {
      InflictDirectDamage(penClosest, m_penPlayer, DMT_CHAINSAW, fDamage, vHit, vDir);
      return TRUE;
    }
    return FALSE;
  };

  /*
  // prepare Bullet
  void PrepareSniperBullet(FLOAT fX, FLOAT fY, FLOAT fDamage, FLOAT fImprecission) {
    // bullet start position
    CalcWeaponPositionImprecise(FLOAT3D(fX, fY, 0), plBullet, TRUE, fImprecission);
    // create bullet
    penBullet = CreateEntity(plBullet, CLASS_BULLET);
    m_vBulletSource = plBullet.pl_PositionVector;
	// init bullet
    EBulletInit eInit;
    eInit.eidOwner = m_penPlayer;
    eInit.fDamage = fDamage;
    penBullet->Initialize(eInit);
  };
  */

  // prepare Bullet
  void PrepareBullet(FLOAT fX, FLOAT fY, FLOAT fDamage) {
    // bullet start position
    CalcWeaponPosition(FLOAT3D(fX, fY, 0), plBullet, TRUE);
    // create bullet
    penBullet = CreateEntity(plBullet, CLASS_BULLET);
    // init bullet
    EBulletInit eInit;
    eInit.eidOwner = m_penPlayer;
    eInit.fDamage = fDamage;
    penBullet->Initialize(eInit);
  };

  /*
  // fire one bullet
  void FireSniperBullet(FLOAT fX, FLOAT fY, FLOAT fRange, FLOAT fDamage, FLOAT fImprecission) {
    PrepareSniperBullet(fX, fY, fDamage, fImprecission);
    ((CBullet&)*penBullet).CalcTarget(fRange);
    ((CBullet&)*penBullet).m_fBulletSize = 0.1f;
    ((CBullet&)*penBullet).m_EdtDamage = DMT_SNIPER;
    // launch bullet
    ((CBullet&)*penBullet).LaunchBullet(TRUE, FALSE, TRUE);
    
    if (((CBullet&)*penBullet).m_vHitPoint != FLOAT3D(0.0f, 0.0f, 0.0f)) {
      m_vBulletTarget = ((CBullet&)*penBullet).m_vHitPoint;
    } else if (TRUE) {
      m_vBulletTarget = m_vBulletSource + FLOAT3D(0.0f, 0.0f, -500.0f)*((CBullet&)*penBullet).GetRotationMatrix();
      
    }
    
	  // bullet no longer needed
	  ((CBullet&)*penBullet).DestroyBullet();
  };
  */

  // fire one bullet
  void FireOneBullet(FLOAT fX, FLOAT fY, FLOAT fRange, FLOAT fDamage) {
    PrepareBullet(fX, fY, fDamage);
    ((CBullet&)*penBullet).CalcTarget(fRange);
    ((CBullet&)*penBullet).m_fBulletSize = 0.1f;

    // launch bullet
    ((CBullet&)*penBullet).LaunchBullet(TRUE, FALSE, TRUE);
    ((CBullet&)*penBullet).DestroyBullet();
  };

  // fire bullets (x offset is used for double shotgun)
  void FireShotgunBullets(FLOAT fX, FLOAT fY, FLOAT fRange, FLOAT fDamage, INDEX iBullets,
    FLOAT *afPositions, FLOAT fStretch, FLOAT fJitter) {
    PrepareBullet(fX, fY, fDamage);
    ((CBullet&)*penBullet).CalcTarget(fRange);
    ((CBullet&)*penBullet).m_fBulletSize = GetSP()->sp_bCooperative ? 0.1f : 0.3f;
    ((CBullet&)*penBullet).m_EdtDamage = DMT_SHOTGUN;
    // launch slugs
    INDEX iSlug;
    for (iSlug=0; iSlug<iBullets; iSlug++) {
      // launch bullet
      ((CBullet&)*penBullet).CalcJitterTargetFixed(
        afPositions[iSlug*2+0]*fRange*fStretch, afPositions[iSlug*2+1]*fRange*fStretch,
        fJitter*fRange*fStretch);
      ((CBullet&)*penBullet).LaunchBullet(iSlug<2, FALSE, TRUE);
    }
    ((CBullet&)*penBullet).DestroyBullet();
  };

  // fire one bullet for machine guns (tommygun and minigun)
  void FireMachineBullet(FLOAT fX, FLOAT fY, FLOAT fRange, FLOAT fDamage, 
    FLOAT fJitter, FLOAT fBulletSize)
  {
    fJitter*=fRange;  // jitter relative to range
    PrepareBullet(fX, fY, fDamage);
    ((CBullet&)*penBullet).CalcTarget(fRange);
    ((CBullet&)*penBullet).m_fBulletSize = fBulletSize;
    ((CBullet&)*penBullet).CalcJitterTarget(fJitter);
    ((CBullet&)*penBullet).LaunchBullet(TRUE, FALSE, TRUE);
    ((CBullet&)*penBullet).DestroyBullet();
  }

  /*
  // fire grenade
	//-----------------------------------------------------------------------------
	// Purpose: 
	// Input  : iPower - 
	// Output : 	void
	//-----------------------------------------------------------------------------
	void FireGrenade(INDEX iPower) 
	{
    // grenade start position
    CPlacement3D plGrenade;
    CalcWeaponPosition(
      FLOAT3D(wpn_fFX[WEAPON_GRENADELAUNCHER],wpn_fFY[WEAPON_GRENADELAUNCHER], 0), 
      plGrenade, TRUE);

    // create grenade
    CEntityPointer penGrenade = CreateEntity(plGrenade, CLASS_PROJECTILE);

    // init and launch grenade
    ELaunchProjectile eLaunch;
    eLaunch.eidLauncher = m_penPlayer;
    eLaunch.prtType = PRT_GRENADE;
    eLaunch.fSpeed = 20.0f+iPower*5.0f;
    penGrenade->Initialize(eLaunch);
  };

  // fire rocket
	//-----------------------------------------------------------------------------
	// Purpose: 
	// Output : 	void
	//-----------------------------------------------------------------------------
	void FireRocket(void) 
	{
    // rocket start position
    CPlacement3D plRocket;
    CalcWeaponPosition(
      FLOAT3D(wpn_fFX[WEAPON_ROCKETLAUNCHER],wpn_fFY[WEAPON_ROCKETLAUNCHER], 0), 
      plRocket, TRUE);
    // create rocket
    CEntityPointer penRocket= CreateEntity(plRocket, CLASS_PROJECTILE);
    // init and launch rocket
    ELaunchProjectile eLaunch;
    eLaunch.eidLauncher = m_penPlayer;
    eLaunch.prtType = PRT_ROCKET;
    penRocket->Initialize(eLaunch);
  };

  // drop pipebomb
  void DropPipebomb(void) {
    // pipebomb start position
    CPlacement3D plPipebomb;
    CalcWeaponPosition(
      FLOAT3D(wpn_fFX[WEAPON_PIPEBOMB],wpn_fFY[WEAPON_PIPEBOMB], 0), 
      plPipebomb, TRUE);
    // create pipebomb
    CEntityPointer penPipebomb = CreateEntity(plPipebomb, CLASS_PIPEBOMB);
    // init and drop pipebomb
    EDropPipebomb eDrop;
    eDrop.penLauncher = m_penPlayer;
    if (((CPlayer&)*m_penPlayer).en_plViewpoint.pl_OrientationAngle(2) > 10.0f) {
      eDrop.fSpeed = 30.0f;
    } else if (((CPlayer&)*m_penPlayer).en_plViewpoint.pl_OrientationAngle(2) > -20.0f) {
      eDrop.fSpeed = 20.0f;
    } else {
      eDrop.fSpeed = 5.0f;
    }
    penPipebomb->Initialize(eDrop);
    m_penPipebomb = penPipebomb;
  };*/

  // flamer source
  void GetFlamerSourcePlacement(CPlacement3D &plSource, CPlacement3D &plInFrontOfPipe) {
    CalcLerpedWeaponPosition(
      FLOAT3D(wpn_fFX[WEAPON_FLAMER],wpn_fFY[WEAPON_FLAMER], -0.15f), 
      plSource, FALSE);
    plInFrontOfPipe=plSource;
    FLOAT3D vFront;
    AnglesToDirectionVector( plSource.pl_OrientationAngle, vFront);
    plInFrontOfPipe.pl_PositionVector=plSource.pl_PositionVector+vFront*1.0f;
  };

  // fire flame
  void FireFlame(void) {
    // flame start position
    CPlacement3D plFlame;

    CalcWeaponPosition(FLOAT3D(wpn_fFX[WEAPON_FLAMER],wpn_fFY[WEAPON_FLAMER], -0.15f), plFlame, TRUE);

    // create flame
    CEntityPointer penFlame = CreateEntity(plFlame, CLASS_PROJECTILE);
    // init and launch flame
    ELaunchProjectile eLaunch;
    eLaunch.eidLauncher = m_penPlayer;
    eLaunch.prtType = PRT_FLAME;
    penFlame->Initialize(eLaunch);
    // link last flame with this one (if not NULL or deleted)
    if (m_penFlame!=NULL && !(m_penFlame->GetFlags()&ENF_DELETED)) {
      ((CProjectile&)*m_penFlame).m_penParticles = penFlame;
    }
    // link to player weapons
    ((CProjectile&)*penFlame).m_penParticles = this;
    // store last flame
    m_penFlame = penFlame;
  };
  
  /*
  // fire laser ray
  void FireLaserRay(void) 
  {
    // laser start position
    CPlacement3D plLaserRay;
    FLOAT fFX = wpn_fFX[WEAPON_LASER];  // get laser center position
    FLOAT fFY = wpn_fFY[WEAPON_LASER];
    FLOAT fLUX = 0.0f;
    FLOAT fRUX = 0.8f;
    FLOAT fLUY = 0.0f;
    FLOAT fRUY = 0.0f;
    FLOAT fLDX = -0.1f;
    FLOAT fRDX = 0.9f;
    FLOAT fLDY = -0.3f;
    FLOAT fRDY = -0.3f;

	// 쓸모없는 부분...
    if (((CPlayer *)&*m_penPlayer)->m_pstState==PST_CROUCH) 
	{
      fLUY = 0.3f;
      fRUY = 0.3f;
      fLDY = 0.1f;
      fRDY = 0.1f;
    }

    switch(m_iLaserBarrel) {
      case 0:   // barrel lu (*o-oo)
        CalcWeaponPosition(FLOAT3D(fFX+fLUX, fFY+fLUY, 0), plLaserRay, TRUE);
        break;
      case 1:   // barrel ld (oo-*o)
        CalcWeaponPosition(FLOAT3D(fFX+fLDX, fFY+fLDY, 0), plLaserRay, TRUE);
        break;
      case 2:   // barrel ru (o*-oo)
        CalcWeaponPosition(FLOAT3D(fFX+fRUX, fFY+fRUY, 0), plLaserRay, TRUE);
        break;
      case 3:   // barrel rd (oo-o*)
        CalcWeaponPosition(FLOAT3D(fFX+fRDX, fFY+fRDY, 0), plLaserRay, TRUE);
        break;
    }
    // create laser projectile
    CEntityPointer penLaser = CreateEntity(plLaserRay, CLASS_PROJECTILE);
    // init and launch laser projectile
    ELaunchProjectile eLaunch;
    eLaunch.eidLauncher = m_penPlayer;
    eLaunch.prtType = PRT_LASER_RAY;
    penLaser->Initialize(eLaunch);
  };

  // ghostbuster source
  void GetGhostBusterSourcePlacement(CPlacement3D &plSource) {
    CalcWeaponPosition(
      FLOAT3D(wpn_fFX[WEAPON_GHOSTBUSTER],wpn_fFY[WEAPON_GHOSTBUSTER], 0), 
      plSource, TRUE);
  };

  // fire ghost buster ray
  void FireGhostBusterRay(void) {
    // ray start position
    CPlacement3D plRay;
    GetGhostBusterSourcePlacement(plRay);
    // fire ray
    ((CGhostBusterRay&)*m_penGhostBusterRay).Fire(plRay);
  };
  */

  // fire cannon ball
  /*
  void FireCannonBall(INDEX iPower)
  {
    // cannon ball start position
    CPlacement3D plBall;
    CalcWeaponPosition(
      FLOAT3D(wpn_fFX[WEAPON_IRONCANNON],wpn_fFY[WEAPON_IRONCANNON], 0), 
      plBall, TRUE);
    // create cannon ball
    CEntityPointer penBall = CreateEntity(plBall, CLASS_CANNONBALL);
    // init and launch cannon ball
    ELaunchCannonBall eLaunch;
    eLaunch.eidLauncher = m_penPlayer;
    eLaunch.fLaunchPower = 60.0f+iPower*4.0f; // ranges from 60-140 (since iPower can be max 20)
    eLaunch.fSize = 3.0f;
	*/
    /*if( m_iCurrentWeapon == WEAPON_NUKECANNON)
    {
      eLaunch.cbtType = CBT_NUKE;
    }
    else
    {
    */
  /*
    eLaunch.cbtType = CBT_IRON;
    eLaunch.fDamage = -1.0f;  // automatic damage will be calculated
//    }
    penBall->Initialize(eLaunch);
  };
  */

  // weapon sound when firing
  void SpawnRangeSound( FLOAT fRange)
  {
    if( _pTimer->CurrentTick()>m_tmRangeSoundSpawned+0.5f) {
      m_tmRangeSoundSpawned = _pTimer->CurrentTick();
      ::SpawnRangeSound( m_penPlayer, m_penPlayer, SNDT_PLAYER, fRange);
    }
  };


  /*
   *  >>>---  WEAPON INTERFACE FUNCTIONS  ---<<<
   */
  // clear weapons
  void ClearWeapons(void) {
    // give/take weapons
    m_iAvailableWeapons = 0x03;
    //m_iColtBullets = 6;
    m_iBullets = 0;
    m_iShells = 0;
//    m_iRockets = 0;
//    m_iGrenades = 0;
    m_iNapalm = 0;
    m_iElectricity = 0;
    m_iIronBalls = 0;
//    m_iNukeBalls = 0;
 //   m_iSniperBullets = 0;
  };

  void ResetWeaponMovingOffset(void)
  {
    // reset weapon draw offset
    m_fWeaponDrawPowerOld = m_fWeaponDrawPower = m_tmDrawStartTime = 0;
  }

  // initialize weapons
  void InitializeWeapons(INDEX iGiveWeapons, INDEX iTakeWeapons, INDEX iTakeAmmo, FLOAT fMaxAmmoRatio)
  {
    ResetWeaponMovingOffset();
    // remember old weapons
    ULONG ulOldWeapons = m_iAvailableWeapons;
    // give/take weapons
    m_iAvailableWeapons &= ~iTakeWeapons;
    m_iAvailableWeapons |= 0x03|iGiveWeapons;
    m_iAvailableWeapons &= WEAPONS_ALLAVAILABLEMASK;
    // m_iAvailableWeapons &= ~WEAPONS_DISABLEDMASK;
    // find which weapons are new
    ULONG ulNewWeapons = m_iAvailableWeapons&~ulOldWeapons;
    // for each new weapon
    for(INDEX iWeapon=WEAPON_KNIFE; iWeapon<WEAPON_LAST; iWeapon++) {
      if ( ulNewWeapons & (1<<(iWeapon-1)) ) {
        // add default amount of ammo
//        AddDefaultAmmoForWeapon(iWeapon, fMaxAmmoRatio);
      }
    }

    // default ammo pack size
    FLOAT fModifier = ClampDn(GetSP()->sp_fAmmoQuantity, 1.0f);
    m_iMaxBullets        = ClampUp((INDEX) ceil(MAX_BULLETS*fModifier),       INDEX(999));
    m_iMaxShells         = ClampUp((INDEX) ceil(MAX_SHELLS*fModifier),        INDEX(999));
    //m_iMaxRockets        = ClampUp((INDEX) ceil(MAX_ROCKETS*fModifier),       INDEX(999));
    //m_iMaxGrenades       = ClampUp((INDEX) ceil(MAX_GRENADES*fModifier),      INDEX(999));
    m_iMaxNapalm         = ClampUp((INDEX) ceil(MAX_NAPALM*fModifier),        INDEX(999));
    m_iMaxElectricity    = ClampUp((INDEX) ceil(MAX_ELECTRICITY*fModifier),   INDEX(999));
//    m_iMaxNukeBalls    = ClampUp((INDEX) ceil(MAX_NUKEBALLS*fModifier),    INDEX(999));
    m_iMaxIronBalls      = ClampUp((INDEX) ceil(MAX_IRONBALLS*fModifier),     INDEX(999));
//    m_iMaxSniperBullets  = ClampUp((INDEX) ceil(MAX_SNIPERBULLETS*fModifier), INDEX(999));

    // take away ammo
    if( iTakeAmmo & (1<<AMMO_BULLETS))       {m_iBullets    = 0;}
    if( iTakeAmmo & (1<<AMMO_SHELLS))        {m_iShells     = 0;}
//    if( iTakeAmmo & (1<<AMMO_ROCKETS))       {m_iRockets    = 0;}
    //if( iTakeAmmo & (1<<AMMO_GRENADES))      {m_iGrenades   = 0;}
    if( iTakeAmmo & (1<<AMMO_NAPALM))        {m_iNapalm     = 0;}
    if( iTakeAmmo & (1<<AMMO_ELECTRICITY))   {m_iElectricity= 0;}
//    if( iTakeAmmo & (1<<AMMO_NUKEBALLS))     {m_iNukeBalls  = 0;}
    if( iTakeAmmo & (1<<AMMO_IRONBALLS))     {m_iIronBalls  = 0;}
    //if( iTakeAmmo & (1<<AMMO_SNIPERBULLETS)) {m_iSniperBullets = 0;}

    // precache eventual new weapons
    Precache();

    // clear temp variables for some weapons
    //m_aMiniGun = 0;
    //m_aMiniGunLast = 0;
    //m_aMiniGunSpeed = 0;

    if (m_bResetWeapon || _pNetwork->IsServer()) {
      // select best weapon
//      SelectNewWeapon();
      // this effectively prevents the client in the network game to reset weapon model for
      // a player that was in the game before client joined
      m_bResetWeapon = FALSE;
    }       

    m_iCurrentWeapon=m_iWantedWeapon;
    wpn_iCurrent = m_iCurrentWeapon;
    m_bChangeWeapon = FALSE;
    // set weapon model for current weapon
    //SetCurrentWeaponModel();
    PlayDefaultAnim();
    // remove weapon attachment
//    ((CPlayerAnimator&)*((CPlayer&)*m_penPlayer).m_penAnimator).RemoveWeapon();
    // add weapon attachment
//    ((CPlayerAnimator&)*((CPlayer&)*m_penPlayer).m_penAnimator).SetWeapon();
  };

  /*
  // get weapon ammo
  INDEX GetAmmo(void)
  {
    switch (m_iCurrentWeapon) {
      case WEAPON_KNIFE:           return 0;
      case WEAPON_COLT:            return m_iColtBullets;
      case WEAPON_DOUBLECOLT:      return m_iColtBullets;
      case WEAPON_SINGLESHOTGUN:   return m_iShells;
      case WEAPON_DOUBLESHOTGUN:   return m_iShells;
      case WEAPON_TOMMYGUN:        return m_iBullets;
      case WEAPON_MINIGUN:         return m_iBullets;
      case WEAPON_ROCKETLAUNCHER:  return m_iRockets;
      case WEAPON_GRENADELAUNCHER: return m_iGrenades;
      case WEAPON_SNIPER:          return m_iSniperBullets;
      case WEAPON_FLAMER:          return m_iNapalm;
      case WEAPON_CHAINSAW:        return 0;
      case WEAPON_LASER:           return m_iElectricity;
      //case WEAPON_IRONCANNON:      return m_iIronBalls;
    }
    return 0;
  };

  // get weapon max ammo (capacity)
  INDEX GetMaxAmmo(void)
  {
    switch (m_iCurrentWeapon) {
      case WEAPON_KNIFE:           return 0;
      case WEAPON_COLT:            return 6;
      case WEAPON_DOUBLECOLT:      return 6;
      case WEAPON_SINGLESHOTGUN:   return m_iMaxShells;
      case WEAPON_DOUBLESHOTGUN:   return m_iMaxShells;
      case WEAPON_TOMMYGUN:        return m_iMaxBullets;
      case WEAPON_MINIGUN:         return m_iMaxBullets;
      case WEAPON_ROCKETLAUNCHER:  return m_iMaxRockets;
      case WEAPON_GRENADELAUNCHER: return m_iMaxGrenades;
      case WEAPON_SNIPER:          return m_iMaxSniperBullets;
      case WEAPON_FLAMER:          return m_iMaxNapalm;
      case WEAPON_CHAINSAW:        return m_iMaxNapalm;
      case WEAPON_LASER:           return m_iMaxElectricity;
      //case WEAPON_IRONCANNON:      return m_iMaxIronBalls;
    }
    return 0;
  };
  */

  void CheatOpen(void)
  {
    if( IsOfClass( m_penRayHit, &CMovingBrush_DLLClass)) {
      m_penRayHit->SendEvent(ETrigger());
    }
  }

  // cheat give all
  void CheatGiveAll(void) {
    // all weapons
    m_iAvailableWeapons = WEAPONS_ALLAVAILABLEMASK;
    // m_iAvailableWeapons &= ~WEAPONS_DISABLEDMASK;
    // ammo for all weapons
    m_iBullets = m_iMaxBullets;
    m_iShells = m_iMaxShells;
//    m_iRockets = m_iMaxRockets;
//    m_iGrenades = m_iMaxGrenades;
    m_iNapalm = m_iMaxNapalm;
    m_iElectricity = m_iMaxElectricity;
    m_iIronBalls = m_iMaxIronBalls;
    //m_iNukeBalls = m_iMaxNukeBalls;
    // precache eventual new weapons
    //m_iSniperBullets = m_iMaxSniperBullets;
    Precache();
  };

  /*
   *  >>>---  RECEIVE FUNCTIONS  ---<<<
   */

  /*
  // clamp ammounts of all ammunition to maximum values
  void ClampAllAmmo(void)
  {
    m_iBullets       = ClampUp(m_iBullets,       m_iMaxBullets);
    m_iShells        = ClampUp(m_iShells,        m_iMaxShells);
    m_iRockets       = ClampUp(m_iRockets,       m_iMaxRockets);
    m_iGrenades      = ClampUp(m_iGrenades,      m_iMaxGrenades);
    m_iNapalm        = ClampUp(m_iNapalm,        m_iMaxNapalm);
    m_iElectricity   = ClampUp(m_iElectricity,   m_iMaxElectricity);
    m_iIronBalls     = ClampUp(m_iIronBalls,     m_iMaxIronBalls);
//    m_iNukeBalls   = ClampUp(m_iNukeBalls,     m_iMaxNukeBalls);
    m_iSniperBullets = ClampUp(m_iSniperBullets, m_iMaxSniperBullets);
  }
  */

  /*
   *  >>>---  WEAPON CHANGE FUNCTIONS  ---<<<
   */
  // get weapon from selected number
  WeaponType GetStrongerWeapon(INDEX iWeapon) {
    switch(iWeapon) {
      case 1: return WEAPON_CHAINSAW;
      //case 2: return WEAPON_DOUBLECOLT;
      //case 3: return WEAPON_DOUBLESHOTGUN;
      //case 4: return WEAPON_MINIGUN;
      //case 5: return WEAPON_ROCKETLAUNCHER;
      case 6: return WEAPON_FLAMER; 
      case 7: return WEAPON_LASER;
      //case 8: return WEAPON_IRONCANNON;
    }
    return WEAPON_NONE;
  };

  // get selected number for weapon
  INDEX GetSelectedWeapon(WeaponType EwtSelectedWeapon) {
    switch(EwtSelectedWeapon) {
      case WEAPON_KNIFE: case WEAPON_CHAINSAW: return 1;
      //case WEAPON_COLT: case WEAPON_DOUBLECOLT: return 2;
      //case WEAPON_SINGLESHOTGUN: case WEAPON_DOUBLESHOTGUN: return 3;
      //case WEAPON_TOMMYGUN: case WEAPON_MINIGUN: return 4;
      //case WEAPON_ROCKETLAUNCHER: case WEAPON_GRENADELAUNCHER: return 5;
      case WEAPON_FLAMER: 
	  //case WEAPON_SNIPER: 
		  return 6;
      case WEAPON_LASER: return 7;
      //case WEAPON_IRONCANNON: return 8;
    }
    return 0;
  };

  // get secondary weapon from selected one
  WeaponType GetAltWeapon(WeaponType EwtWeapon) {
    switch (EwtWeapon) {
      case WEAPON_KNIFE: return WEAPON_CHAINSAW;
      case WEAPON_CHAINSAW: return WEAPON_KNIFE;
      //case WEAPON_COLT: return WEAPON_DOUBLECOLT;
      //case WEAPON_DOUBLECOLT: return WEAPON_COLT;
		  /*
      case WEAPON_SINGLESHOTGUN: return WEAPON_DOUBLESHOTGUN;
      case WEAPON_DOUBLESHOTGUN: return WEAPON_SINGLESHOTGUN;
      case WEAPON_TOMMYGUN: 
	  return WEAPON_MINIGUN;
	  */
//      case WEAPON_MINIGUN: return WEAPON_TOMMYGUN;
//      case WEAPON_ROCKETLAUNCHER: return WEAPON_GRENADELAUNCHER;
//      case WEAPON_GRENADELAUNCHER: return WEAPON_ROCKETLAUNCHER;
      case WEAPON_FLAMER: 
//		  return WEAPON_SNIPER;
 //     case WEAPON_SNIPER: 
		  return WEAPON_FLAMER;
      //case WEAPON_LASER: return WEAPON_LASER;
      //case WEAPON_IRONCANNON: return WEAPON_IRONCANNON;
    }
    return WEAPON_NONE;
  };

  // select new weapon if possible
  BOOL WeaponSelectOk(WeaponType wtToTry)
  {
    // if player has weapon and has enough ammo
    if (((1<<(INDEX(wtToTry)-1))&m_iAvailableWeapons)
      &&HasAmmo(wtToTry)) {
      // if different weapon
      if (wtToTry!=m_iCurrentWeapon) {
        // initiate change
        //m_bHasAmmo = FALSE;
        m_iWantedWeapon = wtToTry;
        m_bChangeWeapon = TRUE;        
      }
      // selection ok
      return TRUE;
    // if no weapon or not enough ammo
    } else {
      // selection not ok
      return FALSE;
    }
  }
  /*
  // select new weapon when no more ammo
  void SelectNewWeapon() 
  {
    if (m_bForceColt) {
      m_bForceColt = FALSE;
      WeaponSelectOk(WEAPON_COLT);
      return;
    }

    switch (m_iCurrentWeapon) {
      case WEAPON_NONE: 
      case WEAPON_KNIFE: case WEAPON_COLT: case WEAPON_DOUBLECOLT: 
      case WEAPON_SINGLESHOTGUN: case WEAPON_DOUBLESHOTGUN:
      case WEAPON_TOMMYGUN: case WEAPON_MINIGUN: 
	  case WEAPON_SNIPER:
        WeaponSelectOk(WEAPON_MINIGUN)||
        WeaponSelectOk(WEAPON_TOMMYGUN)||
        WeaponSelectOk(WEAPON_DOUBLESHOTGUN)||
        WeaponSelectOk(WEAPON_SINGLESHOTGUN)||
        WeaponSelectOk(WEAPON_DOUBLECOLT)||
        WeaponSelectOk(WEAPON_COLT)||
        WeaponSelectOk(WEAPON_KNIFE);
        break;
      case WEAPON_IRONCANNON:
        WeaponSelectOk(WEAPON_ROCKETLAUNCHER)||
        WeaponSelectOk(WEAPON_GRENADELAUNCHER)||
        WeaponSelectOk(WEAPON_MINIGUN)||
        WeaponSelectOk(WEAPON_TOMMYGUN)||
        WeaponSelectOk(WEAPON_DOUBLESHOTGUN)||
        WeaponSelectOk(WEAPON_SINGLESHOTGUN)||
        WeaponSelectOk(WEAPON_DOUBLECOLT)||
        WeaponSelectOk(WEAPON_COLT)||
        WeaponSelectOk(WEAPON_KNIFE);
        break;
      case WEAPON_ROCKETLAUNCHER:
      case WEAPON_GRENADELAUNCHER:
        WeaponSelectOk(WEAPON_ROCKETLAUNCHER)||
        WeaponSelectOk(WEAPON_GRENADELAUNCHER)||
        WeaponSelectOk(WEAPON_MINIGUN)||
        WeaponSelectOk(WEAPON_TOMMYGUN)||
        WeaponSelectOk(WEAPON_DOUBLESHOTGUN)||
        WeaponSelectOk(WEAPON_SINGLESHOTGUN)||
        WeaponSelectOk(WEAPON_DOUBLECOLT)||
        WeaponSelectOk(WEAPON_COLT)||
        WeaponSelectOk(WEAPON_KNIFE);
        break;
      case WEAPON_LASER:  case WEAPON_FLAMER:  case WEAPON_CHAINSAW:
        WeaponSelectOk(WEAPON_LASER)||
        WeaponSelectOk(WEAPON_FLAMER)||
        WeaponSelectOk(WEAPON_MINIGUN)||
        WeaponSelectOk(WEAPON_TOMMYGUN)||
        WeaponSelectOk(WEAPON_DOUBLESHOTGUN)||
        WeaponSelectOk(WEAPON_SINGLESHOTGUN)||
        WeaponSelectOk(WEAPON_DOUBLECOLT)||
        WeaponSelectOk(WEAPON_COLT)||
        WeaponSelectOk(WEAPON_KNIFE);
        break;
      default:
        WeaponSelectOk(WEAPON_KNIFE);
        ASSERT(FALSE);
    }
  };
  */

  // does weapon have ammo
  BOOL HasAmmo(WeaponType EwtWeapon) {
    switch (EwtWeapon) {
      case WEAPON_KNIFE: 
	  //case WEAPON_COLT: case WEAPON_DOUBLECOLT: 
		  return TRUE;
      //case WEAPON_SINGLESHOTGUN: return (m_iShells>0);
      //case WEAPON_DOUBLESHOTGUN: return (m_iShells>1);
      //case WEAPON_TOMMYGUN: return (m_iBullets>0);
//      case WEAPON_SNIPER: return (m_iSniperBullets>0);
      //case WEAPON_MINIGUN: return (m_iBullets>0);
      //case WEAPON_ROCKETLAUNCHER: return (m_iRockets>0);
      //case WEAPON_GRENADELAUNCHER: return (m_iGrenades>0);
      case WEAPON_FLAMER: return (m_iNapalm>0);
      case WEAPON_CHAINSAW: return TRUE;
      //case WEAPON_LASER: return (m_iElectricity>0);
      //case WEAPON_IRONCANNON: return (m_iIronBalls>0);
    }
    return FALSE;
  };

  /*
   *  >>>---   DEFAULT ANIM   ---<<<
   */
  void PlayDefaultAnim(void) {
    switch(m_iCurrentWeapon) {
      case WEAPON_NONE:
        break;
      case WEAPON_KNIFE:
        switch (m_iKnifeStand) {
          case 1: m_moWeapon.PlayAnim(KNIFE_ANIM_WAIT1, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
          case 3: m_moWeapon.PlayAnim(KNIFE_ANIM_WAIT1, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
          default: ASSERTALWAYS("Unknown knife stand.");
        }
        break;

		/*
      case WEAPON_DOUBLECOLT:
        m_moWeaponSecond.PlayAnim(COLT_ANIM_WAIT1, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE);
      case WEAPON_COLT:
        m_moWeapon.PlayAnim(COLT_ANIM_WAIT1, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
      case WEAPON_SINGLESHOTGUN:
        m_moWeapon.PlayAnim(SINGLESHOTGUN_ANIM_WAIT1, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
      case WEAPON_DOUBLESHOTGUN:
        m_moWeapon.PlayAnim(DOUBLESHOTGUN_ANIM_WAIT1, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
      case WEAPON_TOMMYGUN:
        m_moWeapon.PlayAnim(TOMMYGUN_ANIM_WAIT1, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
      case WEAPON_SNIPER:
        m_moWeapon.PlayAnim(SNIPER_ANIM_WAIT01, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
      case WEAPON_MINIGUN:
        m_moWeapon.PlayAnim(MINIGUN_ANIM_WAIT1, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
      case WEAPON_ROCKETLAUNCHER:
        m_moWeapon.PlayAnim(ROCKETLAUNCHER_ANIM_WAIT1, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
      case WEAPON_GRENADELAUNCHER:
        m_moWeapon.PlayAnim(GRENADELAUNCHER_ANIM_WAIT1, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
      case WEAPON_PIPEBOMB:
        if (m_bPipeBombDropped) {
          m_moWeaponSecond.PlayAnim(HANDWITHSTICK_ANIM_STICKTHROWWAIT1, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
        } else {
          m_moWeapon.PlayAnim(HANDWITHBOMB_ANIM_BOMBWAIT2, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE);
          m_moWeaponSecond.PlayAnim(HANDWITHSTICK_ANIM_STICKWAIT1, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
        }
      case WEAPON_FLAMER:
        m_moWeapon.PlayAnim(FLAMER_ANIM_WAIT01, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
      case WEAPON_CHAINSAW:
        m_moWeapon.PlayAnim(CHAINSAW_ANIM_WAIT1, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
	    case WEAPON_LASER:
        m_moWeapon.PlayAnim(LASER_ANIM_WAIT01, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
      case WEAPON_GHOSTBUSTER:
        m_moWeapon.PlayAnim(GHOSTBUSTER_ANIM_WAIT03, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
      case WEAPON_IRONCANNON:
//      case WEAPON_NUKECANNON:
      //  m_moWeapon.PlayAnim(CANNON_ANIM_WAIT01, AOF_LOOPING|AOF_NORESTART|AOF_SMOOTHCHANGE); break;
	  */
      default:
        ASSERTALWAYS("Unknown weapon.");
    }
  };

  FLOAT KnifeBoring(void) {
    // play boring anim
    INDEX iAnim;
    switch (m_iKnifeStand) {
      case 1: iAnim = KNIFE_ANIM_WAIT1; break;
      case 3: iAnim = KNIFE_ANIM_WAIT1; break;
    }
    m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(iAnim);
  };

  /*
  FLOAT ColtBoring(void) {
    // play boring anim
    INDEX iAnim;
    switch (IRnd()%2) {
      case 0: iAnim = COLT_ANIM_WAIT3; break;
      case 1: iAnim = COLT_ANIM_WAIT4; break;
    }
    m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(iAnim);
  };

  FLOAT DoubleColtBoring(void) {
    // play boring anim for one colt
    INDEX iAnim;
    switch (IRnd()%2) {
      case 0: iAnim = COLT_ANIM_WAIT3; break;
      case 1: iAnim = COLT_ANIM_WAIT4; break;
    }
    if (IRnd()&1) {
      m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
      return m_moWeapon.GetAnimLength(iAnim);
    } else {
      m_moWeaponSecond.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
      return m_moWeaponSecond.GetAnimLength(iAnim);
    }
  };

  FLOAT SingleShotgunBoring(void) {
    // play boring anim
    INDEX iAnim;
    switch (IRnd()%2) {
      case 0: iAnim = SINGLESHOTGUN_ANIM_WAIT2; break;
      case 1: iAnim = SINGLESHOTGUN_ANIM_WAIT3; break;
    }
    m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(iAnim);
  };

  FLOAT DoubleShotgunBoring(void) {
    // play boring anim
    INDEX iAnim;
    switch (IRnd()%3) {
      case 0: iAnim = DOUBLESHOTGUN_ANIM_WAIT2; break;
      case 1: iAnim = DOUBLESHOTGUN_ANIM_WAIT3; break;
      case 2: iAnim = DOUBLESHOTGUN_ANIM_WAIT4; break;
    }
    m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(iAnim);
  };

  FLOAT TommyGunBoring(void) {
    // play boring anim
    INDEX iAnim;
    switch (IRnd()%2) {
      case 0: iAnim = TOMMYGUN_ANIM_WAIT2; break;
      case 1: iAnim = TOMMYGUN_ANIM_WAIT3; break;
    }
    m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(iAnim);
  };

  FLOAT SniperBoring(void) {
    // play boring anim
    INDEX iAnim;
    iAnim = SNIPER_ANIM_WAIT01;
    m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(iAnim);
  };

  FLOAT MiniGunBoring(void) {
    // play boring anim
    INDEX iAnim;
    switch (IRnd()%3) {
      case 0: iAnim = MINIGUN_ANIM_WAIT2; break;
      case 1: iAnim = MINIGUN_ANIM_WAIT3; break;
      case 2: iAnim = MINIGUN_ANIM_WAIT4; break;
    }
    m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(iAnim);
  };

  FLOAT RocketLauncherBoring(void) {
    // play boring anim
    m_moWeapon.PlayAnim(ROCKETLAUNCHER_ANIM_WAIT2, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(ROCKETLAUNCHER_ANIM_WAIT2);
  };

  FLOAT GrenadeLauncherBoring(void) {
    // play boring anim
    m_moWeapon.PlayAnim(GRENADELAUNCHER_ANIM_WAIT2, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(GRENADELAUNCHER_ANIM_WAIT2);
  };

  FLOAT PipeBombBoring(void) {
    if (IRnd()&1 && !m_bPipeBombDropped) {
      // play boring anim for hand with bomb
      INDEX iAnim;
      switch (IRnd()%4) {
        case 0: iAnim = HANDWITHBOMB_ANIM_BOMBWAIT1; break;
        case 1: iAnim = HANDWITHBOMB_ANIM_BOMBWAIT3; break;
        case 2: iAnim = HANDWITHBOMB_ANIM_BOMBWAIT4; break;
        case 3: iAnim = HANDWITHBOMB_ANIM_BOMBWAIT5; break;
      }
      m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
      return m_moWeapon.GetAnimLength(iAnim);
    } else {
      // play boring anim for hand with stick
      INDEX iAnimSecond;
      if (m_bPipeBombDropped) {
        switch (IRnd()%2) {
          case 0: iAnimSecond = HANDWITHSTICK_ANIM_STICKTHROWWAIT2; break;
          case 1: iAnimSecond = HANDWITHSTICK_ANIM_STICKTHROWWAIT3; break;
        }
      } else {
        switch (IRnd()%3) {
          case 0: iAnimSecond = HANDWITHSTICK_ANIM_STICKWAIT2; break;
          case 1: iAnimSecond = HANDWITHSTICK_ANIM_STICKWAIT3; break;
          case 2: iAnimSecond = HANDWITHSTICK_ANIM_STICKWAIT4; break;
        }
      }
      m_moWeaponSecond.PlayAnim(iAnimSecond, AOF_SMOOTHCHANGE);
      return m_moWeaponSecond.GetAnimLength(iAnimSecond);
    }
  };

  FLOAT FlamerBoring(void) {
    // play boring anim
    INDEX iAnim;
    switch (IRnd()%4) {
      case 0: iAnim = FLAMER_ANIM_WAIT02; break;
      case 1: iAnim = FLAMER_ANIM_WAIT03; break;
      case 2: iAnim = FLAMER_ANIM_WAIT04; break;
      case 3: iAnim = FLAMER_ANIM_WAIT05; break;
    }
    m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(iAnim);
  };
  
  FLOAT ChainsawBoring(void) {
    // play boring anim
    INDEX iAnim;
    switch (IRnd()%3) {
      case 0: iAnim = CHAINSAW_ANIM_WAIT2; break;
      case 1: iAnim = CHAINSAW_ANIM_WAIT3; break;
      case 2: iAnim = CHAINSAW_ANIM_WAIT4; break;
    }
    m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(iAnim);
  };

  FLOAT LaserBoring(void) {
    // play boring anim
    INDEX iAnim;
    iAnim = LASER_ANIM_WAIT02;
    m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(iAnim);
  };

  FLOAT GhostBusterBoring(void) {
    // play boring anim
    INDEX iAnim;
    switch (IRnd()%3) {
      case 0: iAnim = GHOSTBUSTER_ANIM_WAIT01; break;
      case 1: iAnim = GHOSTBUSTER_ANIM_WAIT02; break;
      case 2: iAnim = GHOSTBUSTER_ANIM_WAIT04; break;
    }
    m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(iAnim);
  };

  FLOAT CannonBoring(void) {
    // play boring anim
    INDEX iAnim;
    switch (IRnd()%3) {
      case 0: iAnim = CANNON_ANIM_WAIT02; break;
      case 1: iAnim = CANNON_ANIM_WAIT03; break;
      case 2: iAnim = CANNON_ANIM_WAIT04; break;
    }
    m_moWeapon.PlayAnim(iAnim, AOF_SMOOTHCHANGE);
    return m_moWeapon.GetAnimLength(iAnim);
  };
  */

  // find the weapon position in the remap array
  WeaponType FindRemapedPos(WeaponType wt)
  {
    for (INDEX i=0; i<18; i++)
    {
      if (aiWeaponsRemap[i]==wt) {
        return (WeaponType)i;
      }
    }
    ASSERT("Non-existant weapon in remap array!");
    return (WeaponType)0;
  }

  /*
  // get secondary weapon for a given primary weapon
  WeaponType PrimaryToSecondary(WeaponType wt)
  {
    if (wt==WEAPON_CHAINSAW) {
      return WEAPON_KNIFE;
    } 
	else if (wt==WEAPON_DOUBLECOLT) {
      return WEAPON_COLT;
    } 
	else if (wt==WEAPON_DOUBLESHOTGUN) {
      return WEAPON_SINGLESHOTGUN;
    } else if (wt==WEAPON_MINIGUN) {
      return WEAPON_TOMMYGUN;
    } 
	else if (wt==WEAPON_ROCKETLAUNCHER) {
      return WEAPON_GRENADELAUNCHER;     
    } 
	else if (wt==WEAPON_SNIPER) {
      return WEAPON_FLAMER;
    } 
	else 
	{
      return wt;
    }
  }
  // get primary weapon for a given secondary weapon
  WeaponType SecondaryToPrimary(WeaponType wt)
  {
    if (wt==WEAPON_KNIFE) {
      return WEAPON_CHAINSAW;
    } 
	else if (wt==WEAPON_COLT) {
      return WEAPON_DOUBLECOLT;
	  
    } 
	else if (wt==WEAPON_SINGLESHOTGUN) {
      return WEAPON_DOUBLESHOTGUN;
    } else if (wt==WEAPON_TOMMYGUN) {
      return WEAPON_MINIGUN;
    } 
	else if (wt==WEAPON_GRENADELAUNCHER) {
      return WEAPON_ROCKETLAUNCHER;       
    } 
	else if (wt==WEAPON_FLAMER) 
	{
      return WEAPON_SNIPER;
    } 
	else 
	{
      return wt;
    }
  }

  // find first possible weapon in given direction
  WeaponType FindWeaponInDirection(INDEX iDir)
  {
    WeaponType wtOrg = (WeaponType)m_iWantedWeapon;
    WeaponType wt = wtOrg;
    FOREVER {
      (INDEX&)wt += iDir;
      if (wt<WEAPON_KNIFE) {
        wt = WEAPON_IRONCANNON;
      }
      if (wt>WEAPON_IRONCANNON) {
        wt = WEAPON_KNIFE;
      }
      if (wt==wtOrg) {
        break;
      }
      if ( ( ((1<<(wt-1))&m_iAvailableWeapons) && HasAmmo(wt)) ) {
        return wt;
      }
    }
    return m_iWantedWeapon;
  }*/
  
  
  WeaponType FindWeaponInDirection(INDEX iDir)
  {
    INDEX wtOrg = FindRemapedPos(m_iWantedWeapon);
    INDEX wti = wtOrg;
    FOREVER 
	{
      (INDEX&)wti += iDir;
	  /*
      if (wti<1) 
	  {
        wti = WEAPON_IRONCANNON;
      }
	  */
      if (wti>14) 
	  {
        wti = WEAPON_KNIFE;
      }
      if (wti==wtOrg) 
	  {
        break;
      }
      WeaponType wt = (WeaponType) aiWeaponsRemap[wti];
      if ( ( ((1<<(wt-1))&m_iAvailableWeapons) && HasAmmo(wt)) ) {
        return wt;
      }
    }
    return m_iWantedWeapon;
  }

  // select new weapon
  void SelectWeaponChange(INDEX iSelect)
  {
    WeaponType EwtTemp;
    // mark that weapon change is required
     m_tmWeaponChangeRequired = ((CPlayerEntity*)(CEntity*)m_penPlayer)->en_tmEntityTime;

    // if storing current weapon
    if (iSelect==0) {
      m_bChangeWeapon = TRUE;
      m_iWantedWeapon = WEAPON_NONE;
      return;
    }


    // if restoring best weapon
    if (iSelect==-4) 
	{
		/*
      if (_pNetwork->IsServer()) 
	  {
        SelectNewWeapon();
      }
	  */
      return;
    }

    
    // if flipping weapon
    if (iSelect==-3) {      
      EwtTemp = GetAltWeapon(m_iWantedWeapon);

    // if selecting previous weapon
    } else if (iSelect==-2) {
      EwtTemp = FindWeaponInDirection(-1);

    // if selecting next weapon
    } else if (iSelect==-1) {
      EwtTemp = FindWeaponInDirection(+1);

    // if selecting directly
    } else {      
      // flip current weapon
      if (iSelect == GetSelectedWeapon(m_iWantedWeapon)) {
        EwtTemp = GetAltWeapon(m_iWantedWeapon);

      // change to wanted weapon
      } 
	  else 
	  {
        
        EwtTemp = GetStrongerWeapon(iSelect);

        // if weapon doesn't exist or doesn't have ammo, flip it
        if ( !((1<<(EwtTemp-1))&m_iAvailableWeapons) || !HasAmmo(EwtTemp)) {
          EwtTemp = GetAltWeapon(EwtTemp);
        }
      }
    }

    // wanted weapon exist and has ammo
    BOOL bChange = ( ((1<<(EwtTemp-1))&m_iAvailableWeapons) && HasAmmo(EwtTemp));
    if (bChange) {     
      m_iWantedWeapon = EwtTemp;
      m_bChangeWeapon = TRUE;
    }
  };
  /*

  void MinigunSmoke()
  {
    if( !hud_bShowWeapon)
    {
      return;
    }
    // smoke
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    if( pl.m_pstState!=PST_DIVE)
    {
      BOOL b3rdPersonView = TRUE;
      if(pl.m_penCamera==NULL && pl.m_pen3rdPersonView==NULL)
      {
        b3rdPersonView = FALSE;
      }

      INDEX ctBulletsFired = ClampUp(m_iBulletsOnFireStart-m_iBullets, INDEX(200));
      for( INDEX iSmoke=0; iSmoke<ctBulletsFired/10; iSmoke++)
      {
        ShellLaunchData *psldSmoke = &pl.m_asldData[pl.m_iFirstEmptySLD];
        CPlacement3D plPipe;
        if( b3rdPersonView)
        {
          CalcWeaponPosition3rdPersonView(FLOAT3D(afMinigunPipe3rdView[0],
            afMinigunPipe3rdView[1], afMinigunPipe3rdView[2]), plPipe, FALSE);
        }
        else
        {
          CalcWeaponPosition(FLOAT3D(afMinigunPipe[0], afMinigunPipe[1], afMinigunPipe[2]), plPipe, FALSE);
        }
        FLOATmatrix3D m;
        MakeRotationMatrixFast(m, plPipe.pl_OrientationAngle);
        psldSmoke->sld_vPos = plPipe.pl_PositionVector+pl.en_vCurrentTranslationAbsolute*iSmoke*_pTimer->TickQuantum;
        FLOAT3D vUp( m(1,2), m(2,2), m(3,2));
        psldSmoke->sld_vUp = vUp;
        psldSmoke->sld_tmLaunch = _pTimer->CurrentTick()+iSmoke*_pTimer->TickQuantum;
        psldSmoke->sld_estType = ESL_BULLET_SMOKE;
        psldSmoke->sld_fSize = 0.75f+ctBulletsFired/50.0f;
        FLOAT3D vSpeedRelative = FLOAT3D(-0.06f, FRnd()/4.0f, -0.06f);
        psldSmoke->sld_vSpeed = vSpeedRelative*m+pl.en_vCurrentTranslationAbsolute;
        pl.m_iFirstEmptySLD = (pl.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
      }
    }
  };

  BOOL SniperZoomDiscrete(INDEX iDirection, BOOL &bZoomChanged)
  {
    bZoomChanged = FALSE;
    // zoom in one zoom level
    if (iDirection>0) {
      for (INDEX i=0; i<iSniperDiscreteZoomLevels ; i++) {
        if (afSniperZoom[2*i]<m_fSniperFOV) {
          m_fSniperFOV=afSniperZoom[2*i];
          m_fSniperFOVlast=m_fSniperFOV;
          bZoomChanged = TRUE;
          break;
        }
      }
    // zoom out one zoom level
    } else {
      for (INDEX i=iSniperDiscreteZoomLevels ; i>0; i--) {
        if (afSniperZoom[2*i]>m_fSniperFOV) {
          m_fSniperFOV=afSniperZoom[2*i];
          m_fSniperFOVlast=m_fSniperFOV; 
          bZoomChanged = TRUE;
          break;
        }
      }
    }
    if (m_fSniperFOV<90.0f) { 
      m_bSniping = TRUE;
    }
    else {
      m_bSniping = FALSE;
    }
    return m_bSniping;
  };
  */

procedures:
  /*
   *  >>>---   M  A  I  N   ---<<<
   */
  Main(EWeaponsInit eInit) {
    // remember the initial parameters
    ASSERT((CEntity*)eInit.eidOwner!=NULL);
    m_penPlayer = eInit.eidOwner;
    m_iPlayerID = ((CEntity*)m_penPlayer)->en_ulID;

    // declare yourself as a void
    InitAsVoid();
    SetFlags(GetFlags()|ENF_CROSSESLEVELS|ENF_NOTIFYLEVELCHANGE);
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);
    SetFlagOn(ENF_CLIENTHANDLING);

    // set weapon model for current weapon
    //SetCurrentWeaponModel();

    // play default anim
    PlayDefaultAnim();    

    wait() {
      on (EBegin) : { call Idle(); }
      // select weapon - send the event through the network
      on (ESelectWeapon eSelect) : 
	  {
        if (_pNetwork->IsServer()) {
          SelectWeaponChange(eSelect.iWeapon);
          if (m_bChangeWeapon) {
            ENetSelectWeapon eNetSelect;
            eNetSelect.iWeapon = m_iWantedWeapon;
            SendEvent(eNetSelect,TRUE);
          }
        }
        resume;
      }
      // select weapon
      on (ENetSelectWeapon eNetSelect) : {
	  /* //0610 kwon 삭제.
        m_tmWeaponChangeRequired = ((CPlayerEntity*)(CEntity*)m_penPlayer)->en_tmEntityTime;
        m_iWantedWeapon = (WeaponType) eNetSelect.iWeapon;
        m_bChangeWeapon = TRUE;
        if (!(_pNetwork->IsServer())) {
          call ChangeWeapon();
        }
        */
        // try to change weapon
        //SelectWeaponChange(eNetSelect.iWeapon);
        resume;
      }
      // before level change
      on (EPreLevelChange) : { 
        // stop everything
        m_bFireWeapon = FALSE;
        call Stopped();
        resume;
      }
      on (EFireWeapon) : {
        if (_pNetwork->IsServer()) {
          SendEvent(ENetFireWeapon(),TRUE);
        }
        resume;
      }
      on (ENetFireWeapon) : {
        // start firing
        m_bFireWeapon = TRUE;
        resume;
      }

      on (EReleaseWeapon) : {
        // stop firing
        SendEvent(ENetReleaseWeapon(),TRUE);
        resume;
      }
      on (ENetReleaseWeapon) : {
        m_bFireWeapon = FALSE;
        resume;
      }
       // reload pressed
      on (EReloadWeapon) : {
        SendEvent(ENetReloadWeapon(),TRUE);
        resume;
      }
      on (ENetReloadWeapon) : {
        m_bReloadWeapon = TRUE;
        resume;
      }
      on (EStop) : { call Stopped(); }
      on (EEnd) : { stop; }
    }

    // cease to exist
    Destroy(FALSE);
    return;
  };

  /*
   *  >>>---   WEAPON CHANGE PROCEDURE  ---<<<
   */
  /*
  ChangeWeapon() 
  {
    // if really changing weapon, make sure sniping is off and notify owner of the change
    if (m_iCurrentWeapon!=m_iWantedWeapon) {
      m_fSniperFOV = m_fSniperFOVlast = m_fSniperMaxFOV;
      m_bSniping = FALSE;
      m_penPlayer->SendEvent(EWeaponChanged());
    }
    // weapon is changed
    m_bChangeWeapon = FALSE;
    // if this is not current weapon change it
    if (m_iCurrentWeapon!=m_iWantedWeapon) {
      // iron/nuke cannon changing is special
      if( (m_iCurrentWeapon == WEAPON_IRONCANNON) && (m_iWantedWeapon == WEAPON_NUKECANNON) )
      {
        autocall ChangeToNukeCannon() EEnd;
        // mark that weapon change has ended
        m_tmWeaponChangeRequired = 0.0f;
        jump Idle();
      }
      else if( (m_iCurrentWeapon == WEAPON_NUKECANNON) && (m_iWantedWeapon == WEAPON_IRONCANNON) )
      {
        autocall ChangeToIronCannon() EEnd;
        // mark that weapon change has ended
        m_tmWeaponChangeRequired = 0.0f;
        jump Idle();
      }

      // store current weapon
      m_iPreviousWeapon = m_iCurrentWeapon;
      autocall PutDown() EEnd;
      // set new weapon
      m_iCurrentWeapon = m_iWantedWeapon;
      // remember current weapon for console usage
      wpn_iCurrent = m_iCurrentWeapon;
      autocall BringUp() EEnd;
      // start engine sound if chainsaw
      if (m_iCurrentWeapon==WEAPON_CHAINSAW) {
        CPlayer &pl = (CPlayer&)*m_penPlayer;
        pl.m_soWeaponAmbient.Set3DParameters( 30.0f, 3.0f, 1.0f, 1.0f);        
        PlaySound(pl.m_soWeaponAmbient, SOUND_CS_IDLE, SOF_3D|SOF_VOLUMETRIC|SOF_LOOP);
        if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("ChainsawIdle");}
      }

    // knife change stand
    } else if (m_iWantedWeapon == WEAPON_KNIFE) {
      // mark that weapon change has ended
      m_tmWeaponChangeRequired = 0.0f;
      autocall ChangeKnifeStand() EEnd;
    }
    jump Idle();
  };
  
  // put weapon down
  PutDown() 
  {
    // start weapon put down animation
    switch (m_iCurrentWeapon) {
      case WEAPON_NONE:
        break;
      // knife have different stands
      case WEAPON_KNIFE: 
        if (m_iKnifeStand==1) {
          m_iAnim = KNIFE_ANIM_PULLOUT;
        } else if (m_iKnifeStand==3) {
          m_iAnim = KNIFE_ANIM_PULLOUT;
        }
        break;
      case WEAPON_DOUBLECOLT: case WEAPON_COLT:
        m_iAnim = COLT_ANIM_DEACTIVATE;
        break;
      case WEAPON_SINGLESHOTGUN:
        m_iAnim = SINGLESHOTGUN_ANIM_DEACTIVATE;
        break;
      case WEAPON_DOUBLESHOTGUN:
        m_iAnim = DOUBLESHOTGUN_ANIM_DEACTIVATE;
        break;
      case WEAPON_TOMMYGUN:
        m_iAnim = TOMMYGUN_ANIM_DEACTIVATE;
        break;
      case WEAPON_SNIPER:
        m_iAnim = SNIPER_ANIM_DEACTIVATE;
        break;
      case WEAPON_MINIGUN:
        m_iAnim = MINIGUN_ANIM_DEACTIVATE;
        break;
      case WEAPON_ROCKETLAUNCHER:
        m_iAnim = ROCKETLAUNCHER_ANIM_DEACTIVATE;
        break;
      case WEAPON_GRENADELAUNCHER:
        m_iAnim = GRENADELAUNCHER_ANIM_DEACTIVATE;
        break;
      case WEAPON_PIPEBOMB:
        m_iAnim = HANDWITHBOMB_ANIM_DEACTIVATE;
        break;
      case WEAPON_FLAMER:
        m_iAnim = FLAMER_ANIM_DEACTIVATE;
        break;
      case WEAPON_CHAINSAW: {
        CPlayer &pl = (CPlayer&)*m_penPlayer;
        PlaySound(pl.m_soWeaponAmbient, SOUND_CS_BRINGDOWN, SOF_3D|SOF_VOLUMETRIC);                
        if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_StopEffect("ChainsawIdle");}
        m_iAnim = CHAINSAW_ANIM_DEACTIVATE;
        break; }
      case WEAPON_LASER:
        m_iAnim = LASER_ANIM_DEACTIVATE;
        break;
      case WEAPON_GHOSTBUSTER:
        m_iAnim = GHOSTBUSTER_ANIM_DEACTIVATE;
        break;
      case WEAPON_IRONCANNON:
//      case WEAPON_NUKECANNON:
        m_iAnim = CANNON_ANIM_DEACTIVATE;
        break;
      default: ASSERTALWAYS("Unknown weapon.");
    }
    // start animator
    CPlayerAnimator &plan = (CPlayerAnimator&)*((CPlayer&)*m_penPlayer).m_penAnimator;
    plan.BodyPushAnimation();
    if (m_iCurrentWeapon==WEAPON_NONE) {
      return EEnd();
    }
      
    // --->>>  COLT -> DOUBLE COLT SPECIFIC  <<<---
    if (m_iCurrentWeapon==WEAPON_COLT && m_iWantedWeapon==WEAPON_DOUBLECOLT) {
      return EEnd();
    }

    // --->>>  DOUBLE COLT SPECIFIC  <<<---
    if (m_iCurrentWeapon==WEAPON_DOUBLECOLT) {
      m_moWeaponSecond.PlayAnim(m_iAnim, 0);
    }

    // --->>>  DOUBLE COLT -> COLT SPECIFIC  <<<---
    if (m_iCurrentWeapon==WEAPON_DOUBLECOLT && m_iWantedWeapon==WEAPON_COLT) {
      autowait(m_moWeapon.GetAnimLength(m_iAnim));
      return EEnd();
    }

    // --->>>  PIPEBOMB SPECIFIC  <<<---
    if (m_iCurrentWeapon==WEAPON_PIPEBOMB) {
      m_moWeapon.PlayAnim(m_iAnim, 0);
      m_moWeaponSecond.PlayAnim(HANDWITHSTICK_ANIM_DEACTIVATE, 0);
      autowait(Max(m_moWeapon.GetAnimLength(m_iAnim), m_moWeaponSecond.GetAnimLength(HANDWITHSTICK_ANIM_DEACTIVATE)));
      return EEnd();
    }

    // reload colts automagicaly when puting them away
    BOOL bNowColt = m_iCurrentWeapon==WEAPON_COLT || m_iCurrentWeapon==WEAPON_DOUBLECOLT;
    BOOL bWantedColt = m_iWantedWeapon==WEAPON_COLT || m_iWantedWeapon==WEAPON_DOUBLECOLT;
    if (bNowColt&&!bWantedColt) {
      m_iColtBullets = 6;
    }

    m_moWeapon.PlayAnim(m_iAnim, 0);
    autowait(m_moWeapon.GetAnimLength(m_iAnim));
    return EEnd();
  };
  */

/*
  // bring up weapon
  BringUp() 
  {
    // reset weapon draw offset
    ResetWeaponMovingOffset();
    // set weapon model for current weapon
    //SetCurrentWeaponModel();
    // start current weapon bring up animation
    switch (m_iCurrentWeapon) {
      case WEAPON_KNIFE: 
        m_iAnim = KNIFE_ANIM_PULL;
        m_iKnifeStand = 1;
        break;
      case WEAPON_COLT: case WEAPON_DOUBLECOLT:
        m_iAnim = COLT_ANIM_ACTIVATE;
        SetFlare(0, FLARE_REMOVE);
        SetFlare(1, FLARE_REMOVE);
        break;
      case WEAPON_SINGLESHOTGUN:
        m_iAnim = SINGLESHOTGUN_ANIM_ACTIVATE;
        SetFlare(0, FLARE_REMOVE);
        break;
      case WEAPON_DOUBLESHOTGUN:
        m_iAnim = DOUBLESHOTGUN_ANIM_ACTIVATE;
        SetFlare(0, FLARE_REMOVE);
        break;
      case WEAPON_TOMMYGUN:
        m_iAnim = TOMMYGUN_ANIM_ACTIVATE;
        SetFlare(0, FLARE_REMOVE);
        break;
      case WEAPON_SNIPER:
        m_iAnim = SNIPER_ANIM_ACTIVATE;
        SetFlare(0, FLARE_REMOVE);
        break;
      case WEAPON_MINIGUN: {
        CAttachmentModelObject *amo = m_moWeapon.GetAttachmentModel(MINIGUN_ATTACHMENT_BARRELS);
        m_aMiniGunLast = m_aMiniGun = amo->amo_plRelative.pl_OrientationAngle(3);
        m_iAnim = MINIGUN_ANIM_ACTIVATE;
        SetFlare(0, FLARE_REMOVE);
        break; }
      case WEAPON_ROCKETLAUNCHER:
        m_iAnim = ROCKETLAUNCHER_ANIM_ACTIVATE;
        break;
      case WEAPON_GRENADELAUNCHER:
        m_iAnim = GRENADELAUNCHER_ANIM_ACTIVATE;
        break;
      case WEAPON_FLAMER:
        m_iAnim = FLAMER_ANIM_ACTIVATE;
        break;
      case WEAPON_CHAINSAW: {
        m_iAnim = CHAINSAW_ANIM_ACTIVATE;
        CPlayer &pl = (CPlayer&)*m_penPlayer;
        pl.m_soWeaponAmbient.Set3DParameters( 30.0f, 3.0f, 1.0f, 1.0f);        
        PlaySound(pl.m_soWeaponAmbient, SOUND_CS_BRINGUP, SOF_3D|SOF_VOLUMETRIC|SOF_LOOP);        
        break; }
      case WEAPON_LASER:
        m_iAnim = LASER_ANIM_ACTIVATE;
        break;
      //case WEAPON_IRONCANNON:
//    case WEAPON_NUKECANNON:
      //  m_iAnim = CANNON_ANIM_ACTIVATE;
      //  break;
      case WEAPON_NONE:
        break;
      default: ASSERTALWAYS("Unknown weapon.");
    }
    // start animator
    CPlayerAnimator &plan = (CPlayerAnimator&)*((CPlayer&)*m_penPlayer).m_penAnimator;
    plan.BodyPullAnimation();

    // --->>>  DOUBLE COLT -> COLT SPECIFIC  <<<---
    if (m_iPreviousWeapon==WEAPON_DOUBLECOLT && m_iCurrentWeapon==WEAPON_COLT) {
      // mark that weapon change has ended
      m_tmWeaponChangeRequired -= hud_tmWeaponsOnScreen/2;
      return EEnd();
    }

    // --->>>  DOUBLE COLT SPECIFIC  <<<---
    if (m_iCurrentWeapon==WEAPON_DOUBLECOLT) {
      m_moWeaponSecond.PlayAnim(m_iAnim, 0);
    }

    // --->>>  COLT -> COLT DOUBLE SPECIFIC  <<<---
    if (m_iPreviousWeapon==WEAPON_COLT && m_iCurrentWeapon==WEAPON_DOUBLECOLT) {
      autowait(m_moWeapon.GetAnimLength(m_iAnim));
      // mark that weapon change has ended
      m_tmWeaponChangeRequired -= hud_tmWeaponsOnScreen/2;
      return EEnd();
    }

    // --->>>  PIPEBOMB SPECIFIC  <<<---
    if (m_iCurrentWeapon==WEAPON_PIPEBOMB) {
      m_moWeapon.PlayAnim(m_iAnim, 0);
      m_moWeaponSecond.PlayAnim(HANDWITHSTICK_ANIM_ACTIVATE, 0);
      autowait(Max(m_moWeapon.GetAnimLength(m_iAnim), m_moWeaponSecond.GetAnimLength(HANDWITHSTICK_ANIM_ACTIVATE)));
      m_bPipeBombDropped = FALSE;
      // mark that weapon change has ended
      m_tmWeaponChangeRequired -= hud_tmWeaponsOnScreen/2;
      return EEnd();
    }

    m_moWeapon.PlayAnim(m_iAnim, 0);
    autowait(m_moWeapon.GetAnimLength(m_iAnim));

    if( m_iCurrentWeapon == WEAPON_NUKECANNON)
    {
      autocall ChangeToNukeCannon() EEnd;
    }

    // mark that weapon change has ended
    m_tmWeaponChangeRequired -= hud_tmWeaponsOnScreen/2;

    return EEnd();
  };
  */


  /*
   *  >>>---   FIRE WEAPON   ---<<<
   */
  Fire()
  {
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    pl.m_soWeapon0.Stop();      // stop possible sounds
    // force ending of weapon change
    m_tmWeaponChangeRequired = 0;

    m_bFireWeapon = TRUE;
    m_bHasAmmo = HasAmmo(m_iCurrentWeapon);

    // if has no ammo select new weapon
    if (!m_bHasAmmo) 
	{
		/*
      if (_pNetwork->IsServer()) 
	  {
        SelectNewWeapon();
      }
	  */
      jump Idle();
    }

    // setup 3D sound parameters
    Setup3DSoundParameters();

    // start weapon firing animation for continuous firing
	/*
    if (m_iCurrentWeapon==WEAPON_MINIGUN) 
	{
      jump MiniGunSpinUp();
    } 
	else 
		if (m_iCurrentWeapon==WEAPON_FLAMER) {
      jump FlamerStart();
    } else if (m_iCurrentWeapon==WEAPON_CHAINSAW) {
      jump ChainsawFire();
  } else if (m_iCurrentWeapon==WEAPON_GHOSTBUSTER) {
      autocall GhostBusterStart() EEnd;
    } else if (m_iCurrentWeapon==WEAPON_LASER) {
      GetAnimator()->FireAnimation(BODY_ANIM_SHOTGUN_FIRESHORT, AOF_LOOPING);
    } 
	else if (m_iCurrentWeapon==WEAPON_TOMMYGUN) {
      autocall TommyGunStart() EEnd;
    } 	
	else if ((m_iCurrentWeapon==WEAPON_IRONCANNON) || (m_iCurrentWeapon==WEAPON_NUKECANNON)) 
	{
      jump CannonFireStart();
    }
	*/

    // clear last lerped bullet position
    m_iLastBulletPosition = FLOAT3D(32000.0f, 32000.0f, 32000.0f);

    // reset laser barrel (to start shooting always from left up barrel)
    //m_iLaserBarrel = 0;

    while (HoldingFire() && m_bHasAmmo) {
      // boring animation
      ((CPlayerAnimator&)*((CPlayer&)*m_penPlayer).m_penAnimator).m_fLastActionTime = _pTimer->CurrentTick();
      wait() {
        on (EBegin) : {
          // fire one shot
          switch (m_iCurrentWeapon) {
            case WEAPON_KNIFE: call SwingKnife(); break;
            //case WEAPON_COLT: call FireColt(); break;
            //case WEAPON_DOUBLECOLT: call FireDoubleColt(); break;
            //case WEAPON_SINGLESHOTGUN: call FireSingleShotgun(); break;
            //case WEAPON_DOUBLESHOTGUN: call FireDoubleShotgun(); break;
            //case WEAPON_TOMMYGUN: call FireTommyGun(); break;
            //case WEAPON_SNIPER: call FireSniper(); break;
            //case WEAPON_ROCKETLAUNCHER: call FireRocketLauncher(); break;
            //case WEAPON_GRENADELAUNCHER: call FireGrenadeLauncher(); break;
            //case WEAPON_LASER: call FireLaser(); break;
            default: ASSERTALWAYS("Unknown weapon.");
          }
          resume;
        }
        on (EEnd) : {
          stop;
        }
      }
    }

    // stop weapon firing animation for continuous firing
//    switch (m_iCurrentWeapon) {
    //  case WEAPON_TOMMYGUN: { jump TommyGunStop(); break; }
      //case WEAPON_MINIGUN: { jump MiniGunSpinDown(); break; }
//      case WEAPON_FLAMER: { jump FlamerStop(); break; }
//      case WEAPON_GHOSTBUSTER: { jump GhostBusterStop(); break; }
		  /*
      case WEAPON_LASER: { 
        GetAnimator()->FireAnimationOff();
        jump Idle();
                         }
						 */
  //    default: { jump Idle(); }
    //}
  };

    
  // ***************** SWING KNIFE *****************
  SwingKnife() {
    INDEX iSwing;

    // animator swing
    GetAnimator()->FireAnimation(BODY_ANIM_KNIFE_ATTACK, 0);
    // sound
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    // depending on stand choose random attack
    switch (m_iKnifeStand) {
      case 1:
        iSwing = IRnd()%2;
        switch (iSwing) {
          case 0: m_iAnim = KNIFE_ANIM_ATTACK01; m_fAnimWaitTime = 0.25f;
			// deleted by seo
            //PlaySound(pl.m_soWeapon0, SOUND_KNIFE_BACK, SOF_3D|SOF_VOLUMETRIC);
            if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Knife_back");}
            break;
          case 1: m_iAnim = KNIFE_ANIM_ATTACK02; m_fAnimWaitTime = 0.35f;
			// deleted by seo
            //PlaySound(pl.m_soWeapon1, SOUND_KNIFE_BACK, SOF_3D|SOF_VOLUMETRIC);
            if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Knife_back");}
            break;
        }
        break;
      case 3:
        iSwing = IRnd()%2;
        switch (iSwing) {
          case 0: m_iAnim = KNIFE_ANIM_ATTACK01; m_fAnimWaitTime = 0.50f;
			// deleted by seo
            //PlaySound(pl.m_soWeapon1, SOUND_KNIFE_BACK, SOF_3D|SOF_VOLUMETRIC);
            if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Knife_back");}
            break;
          case 1: m_iAnim = KNIFE_ANIM_ATTACK02; m_fAnimWaitTime = 0.50f;
			// deleted by seo
            //PlaySound(pl.m_soWeapon3, SOUND_KNIFE_BACK, SOF_3D|SOF_VOLUMETRIC);
            if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Knife_back");}
            break;
        }
        break;
    }
    m_moWeapon.PlayAnim(m_iAnim, 0);
    if (_pNetwork->IsServer()) {
      if (CutWithKnife(0, 0, 3.0f, 2.0f, 0.5f, ((GetSP()->sp_bCooperative) ? 100.0f : 50.0f))) {
        autowait(m_fAnimWaitTime);
      } else if (TRUE) {
        autowait(m_fAnimWaitTime/2);
        CutWithKnife(0, 0, 3.0f, 2.0f, 0.5f, ((GetSP()->sp_bCooperative) ? 100.0f : 50.0f));
        autowait(m_fAnimWaitTime/2);
      }
    }

    if (m_moWeapon.GetAnimLength(m_iAnim)-m_fAnimWaitTime>=_pTimer->TickQuantum) {
      autowait(m_moWeapon.GetAnimLength(m_iAnim)-m_fAnimWaitTime);
    }
    return EEnd();
  };
  /*
  
  // ***************** FIRE COLT *****************
  FireColt() {
    GetAnimator()->FireAnimation(BODY_ANIM_COLT_FIRERIGHT, 0);

    // fire bullet
    if (_pNetwork->IsServer()) {
      FireOneBullet(wpn_fFX[WEAPON_COLT], wpn_fFY[WEAPON_COLT], 500.0f,
        ((GetSP()->sp_bCooperative) ? 10.0f : 20.0f));
    }


    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Colt_fire");}
    SpawnRangeSound(40.0f);
    m_iColtBullets--;
    SetFlare(0, FLARE_ADD);
    PlayLightAnim(LIGHT_ANIM_COLT_SHOTGUN, 0);

    // sound
    CPlayer &pl = (CPlayer&)*m_penPlayer;
	// deleted by seo
    //PlaySound(pl.m_soWeapon0, SOUND_COLT_FIRE, SOF_3D|SOF_VOLUMETRIC);

    if( pl.m_pstState!=PST_DIVE)
    {
      // smoke
      ShellLaunchData &sldRight = pl.m_asldData[pl.m_iFirstEmptySLD];
      sldRight.sld_vPos = FLOAT3D(afRightColtPipe[0], afRightColtPipe[1], afRightColtPipe[2]);
      sldRight.sld_tmLaunch = _pTimer->CurrentTick();
      sldRight.sld_estType = ESL_COLT_SMOKE;
      pl.m_iFirstEmptySLD = (pl.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
    }

    // random colt fire
    INDEX iAnim;
    switch (IRnd()%3) {
      case 0: iAnim = COLT_ANIM_FIRE1; break;
      case 1: iAnim = COLT_ANIM_FIRE2; break;
      case 2: iAnim = COLT_ANIM_FIRE3; break;
    }
    m_moWeapon.PlayAnim(iAnim, 0);
    autowait(m_moWeapon.GetAnimLength(iAnim)-0.05f);
    m_moWeapon.PlayAnim(COLT_ANIM_WAIT1, AOF_LOOPING|AOF_NORESTART);

    // no more bullets in colt -> reload
    if (m_iColtBullets <= 0) {
      jump ReloadColt();
    }
    return EEnd();
  };

  // reload colt
  ReloadColt() {
    if (m_iColtBullets>=6) {
      return EEnd();
    }
    // sound
    CPlayer &pl = (CPlayer&)*m_penPlayer;
	// deleted by seo
    //PlaySound(pl.m_soWeapon1, SOUND_COLT_RELOAD, SOF_3D|SOF_VOLUMETRIC);

    m_moWeapon.PlayAnim(COLT_ANIM_RELOAD, 0);
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Colt_reload");}
    autowait(m_moWeapon.GetAnimLength(COLT_ANIM_RELOAD));
    m_iColtBullets = 6;
    return EEnd();
  };

  // ***************** FIRE DOUBLE COLT *****************
  FireDoubleColt() {
    // fire first colt - one bullet less in colt
    GetAnimator()->FireAnimation(BODY_ANIM_COLT_FIRERIGHT, 0);
    if (_pNetwork->IsServer()) {
      FireOneBullet(wpn_fFX[WEAPON_DOUBLECOLT], wpn_fFY[WEAPON_DOUBLECOLT], 500.0f, 10.0f);
    }
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Colt_fire");}
    
    CPlayer &pl1 = (CPlayer&)*m_penPlayer;
    if( pl1.m_pstState!=PST_DIVE)
    { // smoke
      ShellLaunchData &sldRight = pl1.m_asldData[pl1.m_iFirstEmptySLD];
      sldRight.sld_vPos = FLOAT3D(afRightColtPipe[0], afRightColtPipe[1], afRightColtPipe[2]);
      sldRight.sld_tmLaunch = _pTimer->CurrentTick();
      sldRight.sld_estType = ESL_COLT_SMOKE;
      pl1.m_iFirstEmptySLD = (pl1.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
    }

    SpawnRangeSound(50.0f);
    m_iColtBullets--;
    SetFlare(0, FLARE_ADD);
    PlayLightAnim(LIGHT_ANIM_COLT_SHOTGUN, 0);
    // sound
    CPlayer &plSnd = (CPlayer&)*m_penPlayer;
	// deleted by seo
    //PlaySound(plSnd.m_soWeapon0, SOUND_COLT_FIRE, SOF_3D|SOF_VOLUMETRIC);

    // random colt fire
    switch (IRnd()%3) {
      case 0: m_iAnim = COLT_ANIM_FIRE1; break;
      case 1: m_iAnim = COLT_ANIM_FIRE2; break;
      case 2: m_iAnim = COLT_ANIM_FIRE3; break;
    }
    m_moWeapon.PlayAnim(m_iAnim, 0);                  // play first colt anim
    autowait(m_moWeapon.GetAnimLength(m_iAnim)/2);    // wait half of the anim

    // fire second colt
    GetAnimator()->FireAnimation(BODY_ANIM_COLT_FIRELEFT, 0);
    m_bMirrorFire = TRUE;
    if (_pNetwork->IsServer()) {
      FireOneBullet(wpn_fFX[WEAPON_DOUBLECOLT], wpn_fFY[WEAPON_DOUBLECOLT], 500.0f, 10.0f);
    }
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Colt_fire");}

    CPlayer &pl2 = (CPlayer&)*m_penPlayer;
    if( pl2.m_pstState!=PST_DIVE)
    { // smoke
      ShellLaunchData &sldLeft = pl2.m_asldData[pl2.m_iFirstEmptySLD];
      sldLeft.sld_vPos = FLOAT3D(-afRightColtPipe[0], afRightColtPipe[1], afRightColtPipe[2]);
      sldLeft.sld_tmLaunch = _pTimer->CurrentTick();
      sldLeft.sld_estType = ESL_COLT_SMOKE;
      pl2.m_iFirstEmptySLD = (pl2.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
    }

    m_iSecondFlare = FLARE_ADD;
    ((CPlayerAnimator&)*((CPlayer&)*m_penPlayer).m_penAnimator).m_iSecondFlare = FLARE_ADD;
    PlayLightAnim(LIGHT_ANIM_COLT_SHOTGUN, 0);
    m_bMirrorFire = FALSE;
    // sound
    CPlayer &pl = (CPlayer&)*m_penPlayer;
	// deleted by seo
    //PlaySound(pl.m_soWeapon1, SOUND_COLT_FIRE, SOF_3D|SOF_VOLUMETRIC);

    m_moWeaponSecond.PlayAnim(m_iAnim, 0);
    autowait(m_moWeapon.GetAnimLength(m_iAnim)/2);    // wait half of the anim

    // no more bullets in colt -> reload
    if (m_iColtBullets <= 0) {
      jump ReloadDoubleColt();
    }
    return EEnd();
  };

  // reload double colt
  ReloadDoubleColt() {
    if (m_iColtBullets>=6) {
      return EEnd();
    }
    m_moWeapon.PlayAnim(COLT_ANIM_RELOAD, 0);
    // sound
    CPlayer &pl = (CPlayer&)*m_penPlayer;
	// deleted by seo
    //PlaySound(pl.m_soWeapon2, SOUND_COLT_RELOAD, SOF_3D|SOF_VOLUMETRIC);
    // wait half of reload time
    autowait(m_moWeapon.GetAnimLength(COLT_ANIM_RELOAD)/2);

    m_moWeaponSecond.PlayAnim(COLT_ANIM_RELOAD, 0);
    // sound
    CPlayer &pl = (CPlayer&)*m_penPlayer;
	// deleted by seo
    //PlaySound(pl.m_soWeapon3, SOUND_COLT_RELOAD, SOF_3D|SOF_VOLUMETRIC);
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Colt_reload");}

    // wait second halt minus half shortest fire animation
    autowait(m_moWeapon.GetAnimLength(COLT_ANIM_RELOAD)-0.25f);

    m_iColtBullets = 6;
    return EEnd();
  };


  // ***************** FIRE SINGLESHOTGUN *****************
  FireSingleShotgun()
  {
    // fire one shell
    if (m_iShells>0) {
      GetAnimator()->FireAnimation(BODY_ANIM_SHOTGUN_FIRELONG, 0);
      if (_pNetwork->IsServer()) {
        FireShotgunBullets(wpn_fFX[WEAPON_SINGLESHOTGUN], wpn_fFY[WEAPON_SINGLESHOTGUN], 
          500.0f, 10.0f, 7, afSingleShotgunPellets, 0.2f, 0.03f);
      }
      SpawnRangeSound(60.0f);
      if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Snglshotgun_fire");}
      DecAmmo(m_iShells, 1);
      SetFlare(0, FLARE_ADD);
      PlayLightAnim(LIGHT_ANIM_COLT_SHOTGUN, 0);
      m_moWeapon.PlayAnim(GetSP()->sp_bCooperative ? SINGLESHOTGUN_ANIM_FIRE1 : SINGLESHOTGUN_ANIM_FIRE1FAST, 0);
      // sound
      CPlayer &pl = (CPlayer&)*m_penPlayer;
	  // deleted by seo
      //PlaySound(pl.m_soWeapon0, SOUND_SINGLESHOTGUN_FIRE, SOF_3D|SOF_VOLUMETRIC);
      
      if( hud_bShowWeapon)
      {
        if( pl.m_pstState==PST_DIVE)
        {
          // bubble
          ShellLaunchData &sldBubble = pl.m_asldData[pl.m_iFirstEmptySLD];
          CPlacement3D plShell;
          CalcWeaponPosition(FLOAT3D(afSingleShotgunShellPos[0], 
            afSingleShotgunShellPos[1], afSingleShotgunShellPos[2]), plShell, FALSE);
          FLOATmatrix3D m;
          MakeRotationMatrixFast(m, plShell.pl_OrientationAngle);
          FLOAT3D vUp( m(1,2), m(2,2), m(3,2));
          sldBubble.sld_vPos = plShell.pl_PositionVector;
          sldBubble.sld_vUp = vUp;
          sldBubble.sld_tmLaunch = _pTimer->CurrentTick();
          sldBubble.sld_estType = ESL_BUBBLE;  
          FLOAT3D vSpeedRelative = FLOAT3D(0.3f, 0.0f, 0.0f);
          sldBubble.sld_vSpeed = vSpeedRelative*m;
          pl.m_iFirstEmptySLD = (pl.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
        }
        else
        {
          // smoke
          ShellLaunchData &sldPipe1 = pl.m_asldData[pl.m_iFirstEmptySLD];
          CPlacement3D plPipe;
          CalcWeaponPosition(FLOAT3D(afSingleShotgunPipe[0], afSingleShotgunPipe[1], afSingleShotgunPipe[2]), plPipe, FALSE);
          FLOATmatrix3D m;
          MakeRotationMatrixFast(m, plPipe.pl_OrientationAngle);
          FLOAT3D vUp( m(1,2), m(2,2), m(3,2));
          sldPipe1.sld_vPos = plPipe.pl_PositionVector;
          sldPipe1.sld_vUp = vUp;
          sldPipe1.sld_tmLaunch = _pTimer->CurrentTick();
          sldPipe1.sld_estType = ESL_SHOTGUN_SMOKE;
          FLOAT3D vSpeedRelative = FLOAT3D(0, 0.0f, -12.5f);
          sldPipe1.sld_vSpeed = vSpeedRelative*m;
          pl.m_iFirstEmptySLD = (pl.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
        }
      }

      autowait(GetSP()->sp_bCooperative ? 0.5f : 0.375);      
      CPlacement3D plShell;
      CalcWeaponPosition(FLOAT3D(afSingleShotgunShellPos[0], afSingleShotgunShellPos[1], afSingleShotgunShellPos[2]), plShell, FALSE);

      FLOATmatrix3D mRot;
      MakeRotationMatrixFast(mRot, plShell.pl_OrientationAngle);

      if( hud_bShowWeapon)
      {
        CPlayer *penPlayer = GetPlayer();
        ShellLaunchData &sld = penPlayer->m_asldData[penPlayer->m_iFirstEmptySLD];
        sld.sld_vPos = plShell.pl_PositionVector;
        FLOAT3D vSpeedRelative = FLOAT3D(FRnd()+2.0f, FRnd()+5.0f, -FRnd()-2.0f);
        sld.sld_vSpeed = vSpeedRelative*mRot;

        const FLOATmatrix3D &m = penPlayer->GetRotationMatrix();
        FLOAT3D vUp( m(1,2), m(2,2), m(3,2));
        sld.sld_vUp = vUp;
        sld.sld_tmLaunch = _pTimer->CurrentTick();
        sld.sld_estType = ESL_SHOTGUN;
        // move to next shell position
        penPlayer->m_iFirstEmptySLD = (penPlayer->m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
      }

        autowait(m_moWeapon.GetAnimLength(
          (GetSP()->sp_bCooperative ? SINGLESHOTGUN_ANIM_FIRE1:SINGLESHOTGUN_ANIM_FIRE1FAST) ) -
          (GetSP()->sp_bCooperative ? 0.5f : 0.375f) );
      if (_pNetwork->IsServer()) {
        // no ammo -> change weapon
        if (m_iShells<=0) { SelectNewWeapon(); }
      }
    } else {
      ASSERTALWAYS("SingleShotgun - Auto weapon change not working.");
      m_bFireWeapon = m_bHasAmmo = FALSE;
    }
    return EEnd();
  };

  // ***************** FIRE DOUBLESHOTGUN *****************
  FireDoubleShotgun() {
    // fire two shell
    if (m_iShells>1) {
      GetAnimator()->FireAnimation(BODY_ANIM_SHOTGUN_FIRELONG, 0);
      if (_pNetwork->IsServer()) {
        FireShotgunBullets(wpn_fFX[WEAPON_DOUBLESHOTGUN], wpn_fFY[WEAPON_DOUBLESHOTGUN], 
          500.0f, 10.0f, 14, afDoubleShotgunPellets, 0.3f, 0.03f);
      }
      SpawnRangeSound(70.0f);
      if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Dblshotgun_fire");}
      DecAmmo(m_iShells, 2);
      SetFlare(0, FLARE_ADD);
      PlayLightAnim(LIGHT_ANIM_COLT_SHOTGUN, 0);
      m_moWeapon.PlayAnim(GetSP()->sp_bCooperative ? DOUBLESHOTGUN_ANIM_FIRE : DOUBLESHOTGUN_ANIM_FIREFAST, 0);
      m_moWeaponSecond.PlayAnim(GetSP()->sp_bCooperative ? HANDWITHAMMO_ANIM_FIRE : HANDWITHAMMO_ANIM_FIREFAST, 0);
      // sound
      CPlayer &pl = (CPlayer&)*m_penPlayer;
      pl.m_soWeapon0.Set3DParameters( 50.0f, 5.0f, 1.0f, 1.0f);
	  // deleted by seo
      //PlaySound(pl.m_soWeapon0, SOUND_DOUBLESHOTGUN_FIRE, SOF_3D|SOF_VOLUMETRIC);

      if( hud_bShowWeapon)
      {
        if( pl.m_pstState==PST_DIVE)
        {
          // bubble (pipe 1)
          ShellLaunchData &sldBubble1 = pl.m_asldData[pl.m_iFirstEmptySLD];
          CPlacement3D plShell;
          CalcWeaponPosition(FLOAT3D(-0.11f, 0.1f, -0.3f), plShell, FALSE);
          
          FLOATmatrix3D m;
          MakeRotationMatrixFast(m, plShell.pl_OrientationAngle);
          FLOAT3D vUp( m(1,2), m(2,2), m(3,2));
          sldBubble1.sld_vPos = plShell.pl_PositionVector;
          sldBubble1.sld_vUp = vUp;
          sldBubble1.sld_tmLaunch = _pTimer->CurrentTick();
          sldBubble1.sld_estType = ESL_BUBBLE;  
          FLOAT3D vSpeedRelative = FLOAT3D(-0.1f, 0.0f, 0.01f);
          sldBubble1.sld_vSpeed = vSpeedRelative*m;
          pl.m_iFirstEmptySLD = (pl.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
          ShellLaunchData &sldBubble2 = pl.m_asldData[pl.m_iFirstEmptySLD];
          // bubble (pipe 2)
          sldBubble2 = sldBubble1;
          vSpeedRelative = FLOAT3D(0.1f, 0.0f, -0.2f);
          sldBubble2.sld_vSpeed = vSpeedRelative*m;
          pl.m_iFirstEmptySLD = (pl.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
        }
        else
        {
          // smoke (pipe 1)
          ShellLaunchData &sldPipe1 = pl.m_asldData[pl.m_iFirstEmptySLD];
          CPlacement3D plPipe;
          CalcWeaponPosition(FLOAT3D(afDoubleShotgunPipe[0], afDoubleShotgunPipe[1], afDoubleShotgunPipe[2]), plPipe, FALSE);
          FLOATmatrix3D m;
          MakeRotationMatrixFast(m, plPipe.pl_OrientationAngle);
          FLOAT3D vUp( m(1,2), m(2,2), m(3,2));
          sldPipe1.sld_vPos = plPipe.pl_PositionVector;
          sldPipe1.sld_vUp = vUp;
          sldPipe1.sld_tmLaunch = _pTimer->CurrentTick();
          sldPipe1.sld_estType = ESL_SHOTGUN_SMOKE;
          FLOAT3D vSpeedRelative = FLOAT3D(-1, 0.0f, -12.5f);
          sldPipe1.sld_vSpeed = vSpeedRelative*m;
          pl.m_iFirstEmptySLD = (pl.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
          // smoke (pipe 2)
          ShellLaunchData &sldPipe2 = pl.m_asldData[pl.m_iFirstEmptySLD];
          sldPipe2 = sldPipe1;
          vSpeedRelative = FLOAT3D(1, 0.0f, -12.5f);
          sldPipe2.sld_vSpeed = vSpeedRelative*m;
          pl.m_iFirstEmptySLD = (pl.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
        }
      }

      autowait(GetSP()->sp_bCooperative ? 0.25f : 0.15f);
      if (m_iShells>=2) {
        CPlayer &pl = (CPlayer&)*m_penPlayer;
		// deleted by seo
        //PlaySound(pl.m_soWeapon1, SOUND_DOUBLESHOTGUN_RELOAD, SOF_3D|SOF_VOLUMETRIC);
      }
      autowait( m_moWeapon.GetAnimLength(
        (GetSP()->sp_bCooperative ? DOUBLESHOTGUN_ANIM_FIRE : DOUBLESHOTGUN_ANIM_FIREFAST)) -
        (GetSP()->sp_bCooperative ? 0.25f : 0.15f) );

      if (_pNetwork->IsServer()) {
        // no ammo -> change weapon
        if (m_iShells<=1) { SelectNewWeapon(); }
      }
    } else {
      ASSERTALWAYS("DoubleShotgun - Auto weapon change not working.");
      m_bFireWeapon = m_bHasAmmo = FALSE;
    }
    return EEnd();
  };


  // ***************** FIRE TOMMYGUN *****************
  TommyGunStart()
  {
    m_iBulletsOnFireStart = m_iBullets;
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    pl.m_soWeapon0.Set3DParameters( 50.0f, 5.0f, 1.0f, 1.0f);
	// deleted by seo
    //PlaySound( pl.m_soWeapon0, SOUND_TOMMYGUN_FIRE, SOF_LOOP|SOF_3D|SOF_VOLUMETRIC);
    PlayLightAnim(LIGHT_ANIM_TOMMYGUN, AOF_LOOPING);
    GetAnimator()->FireAnimation(BODY_ANIM_SHOTGUN_FIRESHORT, AOF_LOOPING);
    return EEnd();
  };


  TommyGunStop()
  {
    // smoke
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    if( pl.m_pstState!=PST_DIVE && hud_bShowWeapon)
    {
      INDEX ctBulletsFired = ClampUp(m_iBulletsOnFireStart-m_iBullets, INDEX(100));
      for( INDEX iSmoke=0; iSmoke<ctBulletsFired/6.0; iSmoke++)
      {
        ShellLaunchData *psldSmoke = &pl.m_asldData[pl.m_iFirstEmptySLD];
        CPlacement3D plPipe;
        CalcWeaponPosition(FLOAT3D(afTommygunPipe[0], afTommygunPipe[1], afTommygunPipe[2]), plPipe, FALSE);
        FLOATmatrix3D m;
        MakeRotationMatrixFast(m, plPipe.pl_OrientationAngle);
        psldSmoke->sld_vPos = plPipe.pl_PositionVector+pl.en_vCurrentTranslationAbsolute*iSmoke*_pTimer->TickQuantum;
        FLOAT3D vUp( m(1,2), m(2,2), m(3,2));
        psldSmoke->sld_vUp = vUp;
        psldSmoke->sld_tmLaunch = _pTimer->CurrentTick()+iSmoke*_pTimer->TickQuantum;
        psldSmoke->sld_estType = ESL_BULLET_SMOKE;
        psldSmoke->sld_fSize = 0.5f+ctBulletsFired/75.0f;
        FLOAT3D vSpeedRelative = FLOAT3D(-0.06f, 0.0f, -0.06f);
        psldSmoke->sld_vSpeed = vSpeedRelative*m+pl.en_vCurrentTranslationAbsolute;
        pl.m_iFirstEmptySLD = (pl.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
      }
    }

    pl.m_soWeapon0.Fade( 0, 0.15f);
    PlayLightAnim(LIGHT_ANIM_NONE, 0);
    GetAnimator()->FireAnimationOff();
    jump Idle();
  };


  FireTommyGun()
  {
    // fire one bullet
    if (m_iBullets>0) {
      if (_pNetwork->IsServer()) {
        FireMachineBullet(wpn_fFX[WEAPON_TOMMYGUN], wpn_fFY[WEAPON_TOMMYGUN], 
          500.0f, 10.0f, ((GetSP()->sp_bCooperative) ? 0.01f : 0.03f),
          ((GetSP()->sp_bCooperative) ? 0.5f : 0.0f));
      }
      SpawnRangeSound(50.0f);
      if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Tommygun_fire");}
      DecAmmo(m_iBullets, 1);
      SetFlare(0, FLARE_ADD);
      m_moWeapon.PlayAnim(TOMMYGUN_ANIM_FIRE, AOF_LOOPING|AOF_NORESTART);

      // firing FX
      CPlacement3D plShell;
      CalcWeaponPosition(FLOAT3D(afTommygunShellPos[0], afTommygunShellPos[1], afTommygunShellPos[2]), plShell, FALSE);
      FLOATmatrix3D mRot;
      MakeRotationMatrixFast(mRot, plShell.pl_OrientationAngle);

      if( hud_bShowWeapon)
      {
        // empty bullet shell
        CPlayer &pl = *GetPlayer();
        ShellLaunchData &sld = pl.m_asldData[pl.m_iFirstEmptySLD];
        sld.sld_vPos = plShell.pl_PositionVector;
        FLOAT3D vSpeedRelative = FLOAT3D(FRnd()+2.0f, FRnd()+5.0f, -FRnd()-2.0f);
        const FLOATmatrix3D &m = pl.GetRotationMatrix();
        FLOAT3D vUp( m(1,2), m(2,2), m(3,2));
        sld.sld_vUp = vUp;
        sld.sld_vSpeed = vSpeedRelative*mRot;
        sld.sld_tmLaunch = _pTimer->CurrentTick();
        sld.sld_estType = ESL_BULLET;  
        pl.m_iFirstEmptySLD = (pl.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;

        // bubble
        if( pl.m_pstState==PST_DIVE)
        {
          ShellLaunchData &sldBubble = pl.m_asldData[pl.m_iFirstEmptySLD];
          CalcWeaponPosition(FLOAT3D(afTommygunShellPos[0], afTommygunShellPos[1], afTommygunShellPos[2]), plShell, FALSE);
          MakeRotationMatrixFast(mRot, plShell.pl_OrientationAngle);
          sldBubble.sld_vPos = plShell.pl_PositionVector;
          sldBubble.sld_vUp = vUp;
          sldBubble.sld_tmLaunch = _pTimer->CurrentTick();
          sldBubble.sld_estType = ESL_BUBBLE;  
          vSpeedRelative = FLOAT3D(0.3f, 0.0f, 0.0f);
          sldBubble.sld_vSpeed = vSpeedRelative*mRot;
          pl.m_iFirstEmptySLD = (pl.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
        }
      }

      autowait(0.1f);
      if (_pNetwork->IsServer()) {
        // no ammo -> change weapon
        if (m_iBullets<=0) { SelectNewWeapon(); }
      }
    } else {
      ASSERTALWAYS("TommyGun - Auto weapon change not working.");
      m_bFireWeapon = m_bHasAmmo = FALSE;
    }
    return EEnd();
  };

  // ***************** FIRE SNIPER *****************
  FireSniper() {
    if (m_iSniperBullets>0) {
      if (_pNetwork->IsServer()) {
        // fire one bullet
        if (m_bSniping) {
          FireSniperBullet(0.0f, 0.0f, 1500.0f, 
                           (GetSP()->sp_bCooperative) ? 300.0f : 90.0f, 0.0f);
        }
        else {
          FireSniperBullet(wpn_fFX[WEAPON_SNIPER], wpn_fFY[WEAPON_SNIPER], 1000.0f,
                           (GetSP()->sp_bCooperative) ? 75.0f : 30.0f, 5.0f);
        }
      }
      m_tmLastSniperFire = ((CPlayerEntity*)(CEntity*)m_penPlayer)->en_tmEntityTime;

      SpawnRangeSound(50.0f);
      DecAmmo(m_iSniperBullets, 1);
      if (!m_bSniping) {
        SetFlare(0, FLARE_ADD);
      }
      PlayLightAnim(LIGHT_ANIM_COLT_SHOTGUN, 0);

      // sound
      CPlayer &pl = (CPlayer&)*m_penPlayer;
      if( GetSP()->sp_bCooperative) {
        pl.m_soWeapon0.Set3DParameters( 50.0f, 10.0f, 1.0f, 1.0f);
      } else {
        pl.m_soWeapon0.Set3DParameters( 250.0f, 50.0f, 1.0f, 1.0f);
      }
	  // deleted by seo
      //PlaySound(pl.m_soWeapon0, SOUND_SNIPER_FIRE, SOF_3D|SOF_VOLUMETRIC);
      if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("SniperFire");}
      
      // animation
      m_moWeapon.PlayAnim(SNIPER_ANIM_FIRE, 0);
      
      autowait(1.0f);

      // firing FX
      CPlacement3D plShell;
      CalcWeaponPosition(FLOAT3D(afSniperShellPos[0], afSniperShellPos[1], afSniperShellPos[2]), plShell, FALSE);
      FLOATmatrix3D mRot;
      MakeRotationMatrixFast(mRot, plShell.pl_OrientationAngle);
      
      if( hud_bShowWeapon)
      {
        CPlayer *penPlayer = GetPlayer();
        ShellLaunchData &sld = penPlayer->m_asldData[penPlayer->m_iFirstEmptySLD];
        sld.sld_vPos = plShell.pl_PositionVector;
        FLOAT3D vSpeedRelative = FLOAT3D(FRnd()+2.0f, FRnd()+5.0f, -FRnd()-2.0f);
        sld.sld_vSpeed = vSpeedRelative*mRot;
        
        const FLOATmatrix3D &m = penPlayer->GetRotationMatrix();
        FLOAT3D vUp( m(1,2), m(2,2), m(3,2));
        sld.sld_vUp = vUp;
        sld.sld_tmLaunch = _pTimer->CurrentTick();
        sld.sld_estType = ESL_BULLET;
        // move to next shell position
        penPlayer->m_iFirstEmptySLD = (penPlayer->m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
        
        // bubble
        if( penPlayer->m_pstState==PST_DIVE)
        {
          ShellLaunchData &sldBubble = penPlayer->m_asldData[penPlayer->m_iFirstEmptySLD];
          CalcWeaponPosition(FLOAT3D(afTommygunShellPos[0], afTommygunShellPos[1], afTommygunShellPos[2]), plShell, FALSE);
          MakeRotationMatrixFast(mRot, plShell.pl_OrientationAngle);
          sldBubble.sld_vPos = plShell.pl_PositionVector;
          sldBubble.sld_vUp = vUp;
          sldBubble.sld_tmLaunch = _pTimer->CurrentTick();
          sldBubble.sld_estType = ESL_BUBBLE;  
          vSpeedRelative = FLOAT3D(0.3f, 0.0f, 0.0f);
          sldBubble.sld_vSpeed = vSpeedRelative*mRot;
          penPlayer->m_iFirstEmptySLD = (penPlayer->m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
        }
      }
      
      autowait(1.35f - 1.0f);
      
      // no ammo -> change weapon
      if (_pNetwork->IsServer()) {
        if (m_iSniperBullets<=0) { SelectNewWeapon(); }
      }
    } 
    else {
      ASSERTALWAYS("Sniper - Auto weapon change not working.");
      m_bFireWeapon = m_bHasAmmo = FALSE;
    }
    return EEnd();
  };
  
  // ***************** FIRE MINIGUN *****************
  MiniGunSpinUp() {
    // steady anim
    m_moWeapon.PlayAnim(MINIGUN_ANIM_WAIT1, AOF_LOOPING|AOF_NORESTART);
    // no boring animation
    ((CPlayerAnimator&)*((CPlayer&)*m_penPlayer).m_penAnimator).m_fLastActionTime = _pTimer->CurrentTick();
    // clear last lerped bullet position
    m_iLastBulletPosition = FLOAT3D(32000.0f, 32000.0f, 32000.0f);
    CPlayer &pl = (CPlayer&)*m_penPlayer;

    // initialize sound 3D parameters
    pl.m_soWeapon0.Stop();
    pl.m_soWeapon0.Set3DParameters( 50.0f, 5.0f, 1.0f, 1.0f);      // fire
    pl.m_soWeapon1.Set3DParameters( 50.0f, 5.0f, 0.7f, 1.0f);      // spinup/spindown/spin
    pl.m_soWeapon2.Set3DParameters( 50.0f, 5.0f, 0.5f, 1.0f);      // turn on/off click
  
    // spin start sounds
	// deleted by seo
    //PlaySound(pl.m_soWeapon2, SOUND_MINIGUN_CLICK, SOF_3D|SOF_VOLUMETRIC);
    //PlaySound(pl.m_soWeapon1, SOUND_MINIGUN_SPINUP, SOF_3D|SOF_VOLUMETRIC);
    //pl.m_soWeapon1.SetOffset((m_aMiniGunSpeed/MINIGUN_FULLSPEED)*MINIGUN_SPINUPSOUND);
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Minigun_rotateup");}
    // while not at full speed and fire is held
    while (m_aMiniGunSpeed<MINIGUN_FULLSPEED && HoldingFire()) {
      // every tick
      autowait(MINIGUN_TICKTIME);
      // increase speed
      m_aMiniGunLast = m_aMiniGun;
      m_aMiniGun+=m_aMiniGunSpeed*MINIGUN_TICKTIME;
      m_aMiniGunSpeed+=MINIGUN_SPINUPACC*MINIGUN_TICKTIME;
    }
    // do not go over full speed
    m_aMiniGunSpeed = ClampUp( m_aMiniGunSpeed, MINIGUN_FULLSPEED);

    // if not holding fire anymore
    if (!HoldingFire()) {
      // start spindown
      jump MiniGunSpinDown();
    }
    // start firing
    jump MiniGunFire();
  }

  MiniGunFire() {
    // spinning sound
    CPlayer &pl = (CPlayer&)*m_penPlayer;
	// deleted by seo
    //PlaySound(pl.m_soWeapon1, SOUND_MINIGUN_ROTATE, SOF_3D|SOF_LOOP|SOF_VOLUMETRIC);
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Minigun_rotate");}
  // if firing
    if(HoldingFire() && m_iBullets>0) {
      // play fire sound
	  // deleted by seo
      //PlaySound(pl.m_soWeapon0, SOUND_MINIGUN_FIRE, SOF_3D|SOF_LOOP|SOF_VOLUMETRIC);
      PlayLightAnim(LIGHT_ANIM_TOMMYGUN, AOF_LOOPING);
      GetAnimator()->FireAnimation(BODY_ANIM_MINIGUN_FIRESHORT, AOF_LOOPING);
    }

    m_iBulletsOnFireStart = m_iBullets;
    // while holding fire
    while (HoldingFire()) {
      // check for ammo pickup during empty spinning
      if (!m_bHasAmmo && m_iBullets>0) {
        CPlayer &pl = (CPlayer&)*m_penPlayer;
		// deleted by seo
        //PlaySound(pl.m_soWeapon0, SOUND_MINIGUN_FIRE, SOF_3D|SOF_LOOP|SOF_VOLUMETRIC);
        if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Minigun_fire");}
        PlayLightAnim(LIGHT_ANIM_TOMMYGUN, AOF_LOOPING);
        GetAnimator()->FireAnimation(BODY_ANIM_MINIGUN_FIRESHORT, AOF_LOOPING);
        m_bHasAmmo = TRUE;
      }

      // if has ammo
      if (m_iBullets>0) {
        // fire a bullet
        if (_pNetwork->IsServer()) {
          FireMachineBullet(wpn_fFX[WEAPON_MINIGUN], wpn_fFY[WEAPON_MINIGUN],
            750.0f, 10.0f, (GetSP()->sp_bCooperative) ? 0.01f : 0.03f, 
            ( (GetSP()->sp_bCooperative) ? 0.5f : 0.0f));
        }
        SpawnRangeSound(60.0f);
        DecAmmo(m_iBullets, 1);
        SetFlare(0, FLARE_ADD);
        
        CPlacement3D plShell;

        // if 1st person view
        CPlayer &pl = (CPlayer&)*m_penPlayer;
        if(pl.m_penCamera==NULL && pl.m_pen3rdPersonView==NULL)
        {
          CalcWeaponPosition(FLOAT3D(afMinigunShellPos[0], afMinigunShellPos[1], afMinigunShellPos[2]), plShell, FALSE);
        }
        // if 3rd person view
        else
        {          
          CalcWeaponPosition3rdPersonView(FLOAT3D(afMinigunShellPos3rdView[0],
            afMinigunShellPos3rdView[1], afMinigunShellPos3rdView[2]), plShell, FALSE);
        }

        FLOATmatrix3D mRot;
        MakeRotationMatrixFast(mRot, plShell.pl_OrientationAngle);

        if( hud_bShowWeapon)
        {
          CPlayer &pl = *GetPlayer();
          ShellLaunchData &sld = pl.m_asldData[pl.m_iFirstEmptySLD];
          sld.sld_vPos = plShell.pl_PositionVector;
          FLOAT3D vSpeedRelative = FLOAT3D(FRnd()+2.0f, FRnd()+5.0f, -FRnd()-2.0f);
          const FLOATmatrix3D &m = pl.GetRotationMatrix();
          FLOAT3D vUp( m(1,2), m(2,2), m(3,2));
          sld.sld_vUp = vUp;
          sld.sld_vSpeed = vSpeedRelative*mRot;
          sld.sld_tmLaunch = _pTimer->CurrentTick();
          sld.sld_estType = ESL_BULLET;
          // move to next shell position
          pl.m_iFirstEmptySLD = (pl.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;

          // bubble
          if( pl.m_pstState==PST_DIVE)
          {
            ShellLaunchData &sldBubble = pl.m_asldData[pl.m_iFirstEmptySLD];
            CalcWeaponPosition(FLOAT3D(afMinigunShellPos[0], afMinigunShellPos[1], afMinigunShellPos[2]), plShell, FALSE);
            MakeRotationMatrixFast(mRot, plShell.pl_OrientationAngle);
            sldBubble.sld_vPos = plShell.pl_PositionVector;
            sldBubble.sld_vUp = vUp;
            sldBubble.sld_tmLaunch = _pTimer->CurrentTick();
            sldBubble.sld_estType = ESL_BUBBLE;  
            vSpeedRelative = FLOAT3D(0.3f, 0.0f, 0.0f);
            sldBubble.sld_vSpeed = vSpeedRelative*mRot;
            pl.m_iFirstEmptySLD = (pl.m_iFirstEmptySLD+1) % MAX_FLYING_SHELLS;
          }
        }
      // if no ammo
      } else {
        if( m_bHasAmmo)
        {
          MinigunSmoke();
        }
        // stop fire sound
        m_bHasAmmo = FALSE;
        CPlayer &pl = (CPlayer&)*m_penPlayer;
        pl.m_soWeapon0.Stop();      // stop possible sounds
        PlayLightAnim(LIGHT_ANIM_NONE, AOF_LOOPING);
        GetAnimator()->FireAnimationOff();
      }
      autowait(MINIGUN_TICKTIME);
      // spin
      m_aMiniGunLast = m_aMiniGun;
      m_aMiniGun+=m_aMiniGunSpeed*MINIGUN_TICKTIME;
    }

    if( m_bHasAmmo)
    {
      MinigunSmoke();
    }

    GetAnimator()->FireAnimationOff();
    // stop fire sound
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    pl.m_soWeapon0.Stop();      // stop possible sounds
    PlayLightAnim(LIGHT_ANIM_NONE, AOF_LOOPING);
    // start spin down
    jump MiniGunSpinDown();
  }

  MiniGunSpinDown() {
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    // spin down sounds
	// deleted by seo
    //PlaySound(pl.m_soWeapon3, SOUND_MINIGUN_CLICK, SOF_3D|SOF_VOLUMETRIC);
    //PlaySound(pl.m_soWeapon1, SOUND_MINIGUN_SPINDOWN, SOF_3D|SOF_VOLUMETRIC);
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_StopEffect("Minigun_rotate");}
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Minigun_rotatedown");}
    //pl.m_soWeapon1.SetOffset((1-m_aMiniGunSpeed/MINIGUN_FULLSPEED)*MINIGUN_SPINDNSOUND);

    // while still spinning and should not fire
    while ( m_aMiniGunSpeed>0 && (!HoldingFire() || m_iBullets<=0)) {
      autowait(MINIGUN_TICKTIME);
      // spin
      m_aMiniGunLast = m_aMiniGun;
      m_aMiniGun+=m_aMiniGunSpeed*MINIGUN_TICKTIME;
      m_aMiniGunSpeed-=MINIGUN_SPINDNACC*MINIGUN_TICKTIME;

      if (_pNetwork->IsServer()) {
        if (m_iBullets<=0) {
          SelectNewWeapon();
        }
      }

      // if weapon should be changed
      if (m_bChangeWeapon) {
        // stop spinning immediately
        m_aMiniGunSpeed = 0.0f;
        m_aMiniGunLast = m_aMiniGun;
        GetAnimator()->FireAnimationOff();
        jump Idle();
      }
    }
    // clamp some
    m_aMiniGunSpeed = ClampDn( m_aMiniGunSpeed, 0.0f);
    // m_aMiniGunLast = m_aMiniGun; <- this was causing a bug!

    // if should fire
    if (HoldingFire() && m_iBullets>0) {
      // start spinup
      jump MiniGunSpinUp();
    }

    // no boring animation
    ((CPlayerAnimator&)*((CPlayer&)*m_penPlayer).m_penAnimator).m_fLastActionTime = _pTimer->CurrentTick();

    // if out of ammo
    if (m_iBullets<=0) { 
      // can wait without changing while holding fire - specific for movie sequence
      while(HoldingFire() && m_iBullets<=0) {
        autowait(0.1f);
      }
      if (_pNetwork->IsServer()) {
        if (m_iBullets<=0) {
          // select new weapon
          SelectNewWeapon(); 
        }
      }
    }
    jump Idle();
  };


  // ***************** FIRE ROCKETLAUNCHER *****************
  FireRocketLauncher()
  {
    // fire one grenade
    if (m_iRockets>0) {
      GetAnimator()->FireAnimation(BODY_ANIM_MINIGUN_FIRELONG, 0);
      m_moWeapon.PlayAnim(ROCKETLAUNCHER_ANIM_FIRE, 0);
      if (_pNetwork->IsServer()) {
        FireRocket();
      }

      SpawnRangeSound(20.0f);
      if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Rocketlauncher_fire");}
      DecAmmo(m_iRockets, 1);
      // sound
      CPlayer &pl = (CPlayer&)*m_penPlayer;
      if( pl.m_soWeapon0.IsPlaying()) {
	    // deleted by seo
        //PlaySound( pl.m_soWeapon1, SOUND_ROCKETLAUNCHER_FIRE, SOF_3D|SOF_VOLUMETRIC);
      } else {
		// deleted by seo
        //PlaySound( pl.m_soWeapon0, SOUND_ROCKETLAUNCHER_FIRE, SOF_3D|SOF_VOLUMETRIC);
      }

      autowait(0.05f);

      CModelObject *pmo = &(m_moWeapon.GetAttachmentModel(ROCKETLAUNCHER_ATTACHMENT_ROCKET1)->amo_moModelObject);
      pmo->StretchModel(FLOAT3D(0, 0, 0));

      autowait(m_moWeapon.GetAnimLength(ROCKETLAUNCHER_ANIM_FIRE)-0.05f);

      CModelObject *pmo = &(m_moWeapon.GetAttachmentModel(ROCKETLAUNCHER_ATTACHMENT_ROCKET1)->amo_moModelObject);
      pmo->StretchModel(FLOAT3D(1, 1, 1));

      if (_pNetwork->IsServer()) {
        // no ammo -> change weapon
        //if (m_iRockets<=0) { SelectNewWeapon(); }
      }
    } else {
  //    ASSERTALWAYS("RocketLauncher - Auto weapon change not working.");
      m_bFireWeapon = m_bHasAmmo = FALSE;
    }
    return EEnd();
  };

  // ***************** FIRE GRENADELAUNCHER *****************
  FireGrenadeLauncher()
  {
    TM_START = _pTimer->CurrentTick();
    // remember time for spring release
    F_TEMP = _pTimer->CurrentTick();

    F_OFFSET_CHG = 0.0f;
    m_fWeaponDrawPower = 0.0f;
    m_tmDrawStartTime = _pTimer->CurrentTick();
    while (HoldingFire() && ((_pTimer->CurrentTick()-TM_START)<0.75f) )
    {
      autowait(_pTimer->TickQuantum);
      INDEX iPower = INDEX((_pTimer->CurrentTick()-TM_START)/_pTimer->TickQuantum);
      F_OFFSET_CHG = 0.125f/(iPower+2);
      m_fWeaponDrawPowerOld = m_fWeaponDrawPower;
      m_fWeaponDrawPower += F_OFFSET_CHG;
    }
    m_tmDrawStartTime = 0.0f;


    // release spring and fire one grenade
    if (m_iGrenades>0)
    {
      // fire grenade
      INDEX iPower = INDEX((_pTimer->CurrentTick()-F_TEMP)/_pTimer->TickQuantum);
      if (_pNetwork->IsServer()) {
        FireGrenade( iPower);
      }
      SpawnRangeSound(10.0f);
      if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Gnadelauncher");}
      DecAmmo(m_iGrenades, 1);
      // sound
      CPlayer &pl = (CPlayer&)*m_penPlayer;
	  // deleted by seo
      //PlaySound(pl.m_soWeapon0, SOUND_GRENADELAUNCHER_FIRE, SOF_3D|SOF_VOLUMETRIC);
      GetAnimator()->FireAnimation(BODY_ANIM_MINIGUN_FIRELONG, 0);

      // release spring
      TM_START = _pTimer->CurrentTick();
      m_fWeaponDrawPowerOld = m_fWeaponDrawPower;
      while (m_fWeaponDrawPower>0.0f)
      {
        autowait(_pTimer->TickQuantum);
        m_fWeaponDrawPowerOld = m_fWeaponDrawPower;
        m_fWeaponDrawPower -= F_OFFSET_CHG;
        m_fWeaponDrawPower = ClampDn( m_fWeaponDrawPower, 0.0f);
        F_OFFSET_CHG = F_OFFSET_CHG*10;
      }

      // reset moving part's offset
      ResetWeaponMovingOffset();

      if (_pNetwork->IsServer()) {
        // no ammo -> change weapon
        if (m_iGrenades<=0)
        {
          if (_pNetwork->IsServer()) {
            //SelectNewWeapon();
          }
        }     
        else if( TRUE)
        {
          autowait(0.25f);
        }
      }
    } else {
//      ASSERTALWAYS("GrenadeLauncher - Auto weapon change not working.");
      m_bFireWeapon = m_bHasAmmo = FALSE;
    }

    return EEnd();
  };

  // ***************** FIRE PIPEBOMB *****************
  FirePipeBomb() {
    // drop one pipebomb
    if (m_iGrenades>=0) {
      // fire bomb
      if (m_bPipeBombDropped) {
        m_bPipeBombDropped = FALSE;
        m_moWeaponSecond.PlayAnim(HANDWITHSTICK_ANIM_STICKFIRE, 0);
        autowait(0.35f);
        // activate pipebomb
        SendToTarget(m_penPipebomb, EET_START);
        m_penPipebomb = NULL;
        // sound
        CPlayer &pl = (CPlayer&)*m_penPlayer;
        PlaySound(pl.m_soWeapon0, SOUND_PIPEBOMB_FIRE, SOF_3D|SOF_VOLUMETRIC);
        // no ammo -> change weapon
        if (m_iGrenades<=0) {
          autowait(m_moWeaponSecond.GetAnimLength(HANDWITHSTICK_ANIM_STICKFIRE)-0.35f);
          SelectNewWeapon();
          return EEnd();
        }
        // get new bomb
        AddAttachmentToModel(this, m_moWeapon, HANDWITHBOMB_ATTACHMENT_BOMB, MODEL_PB_BOMB,
                             TEXTURE_PB_BOMB, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        m_moWeapon.PlayAnim(HANDWITHBOMB_ANIM_ACTIVATE, 0);
        autowait(m_moWeapon.GetAnimLength(HANDWITHBOMB_ANIM_ACTIVATE));

      // drop bomb
      } else if (TRUE) {
        m_bPipeBombDropped = TRUE;
        // low drop
        if (((CPlayer&)*m_penPlayer).en_plViewpoint.pl_OrientationAngle(2) < -20.0f) {
          m_moWeapon.PlayAnim(HANDWITHBOMB_ANIM_BOMBTHROW1 ,0);
        // high drop
        } else {
          m_moWeapon.PlayAnim(HANDWITHBOMB_ANIM_BOMBTHROW2 ,0);
        }
        autowait(0.5f);
        // sound
        CPlayer &pl = (CPlayer&)*m_penPlayer;
        PlaySound(pl.m_soWeapon1, SOUND_PIPEBOMB_THROW, SOF_3D|SOF_VOLUMETRIC);
        SpawnRangeSound(5.0f);
        autowait(0.2f);
        // drop bomb
        DropPipebomb();
        DecAmmo(m_iGrenades, 1);
        RemoveAttachmentFromModel(m_moWeapon, HANDWITHBOMB_ATTACHMENT_BOMB);
        autowait(0.2f);
        // open stick shield
        m_moWeaponSecond.PlayAnim(HANDWITHSTICK_ANIM_STICKTHROW ,0);
        // sound
        CPlayer &pl = (CPlayer&)*m_penPlayer;
        PlaySound(pl.m_soWeapon2, SOUND_PIPEBOMB_OPEN, SOF_3D|SOF_VOLUMETRIC);
        autowait(m_moWeaponSecond.GetAnimLength(HANDWITHSTICK_ANIM_STICKTHROW));
      }
    } else {
      ASSERTALWAYS("Pipebomb - Auto weapon change not working.");
      m_bFireWeapon = m_bHasAmmo = FALSE;
    }
    return EEnd();
  };

  ReloadPipeBomb() {
    if (m_bPipeBombDropped) {
      m_bPipeBombDropped = FALSE;
      // close stick
      m_moWeaponSecond.PlayAnim(HANDWITHSTICK_ANIM_STICKRETURN, 0);
      // get new bomb
      AddAttachmentToModel(this, m_moWeapon, HANDWITHBOMB_ATTACHMENT_BOMB, MODEL_PB_BOMB,
                           TEXTURE_PB_BOMB, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
      m_moWeapon.PlayAnim(HANDWITHBOMB_ANIM_ACTIVATE, 0);
      autowait(m_moWeapon.GetAnimLength(HANDWITHBOMB_ANIM_ACTIVATE));
    }

    return EEnd();
  };


  // ***************** FIRE FLAMER *****************
  FlamerStart()
  {
    m_tmFlamerStart=_pTimer->CurrentTick();
    m_tmFlamerStop=1e9;
    
    m_moWeapon.PlayAnim(FLAMER_ANIM_FIRESTART, 0);
    autowait(m_moWeapon.GetAnimLength(FLAMER_ANIM_FIRESTART));
    // play fire sound
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    pl.m_soWeapon0.Set3DParameters( 50.0f, 10.0f, 1.0f, 0.31f);
    pl.m_soWeapon2.Set3DParameters( 50.0f, 10.0f, 1.0f, 0.3f);
	// deleted by seo
    //PlaySound(pl.m_soWeapon0, SOUND_FL_FIRE, SOF_3D|SOF_LOOP|SOF_VOLUMETRIC);
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("FlamethrowerFire");}
	// deleted by seo
    //PlaySound(pl.m_soWeapon2, SOUND_FL_START, SOF_3D|SOF_VOLUMETRIC);
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("FlamethrowerStart");}  
    if (_pNetwork->IsServer()) {
      FireFlame();    
    }
    DecAmmo(m_iNapalm, 1);
    autowait(0.05f);
    jump FlamerFire();
  };

  FlamerFire() {
    // while holding fire
    while (HoldingFire() && m_iNapalm>0) {
      // fire  
      if (_pNetwork->IsServer()) {
        FireFlame();
      }
      DecAmmo(m_iNapalm, 1);
      SpawnRangeSound(30.0f);
      autowait(0.1f);
    }

    if (m_iNapalm<=0) {
      m_bHasAmmo = FALSE;
    }

    jump FlamerStop();
  };


  FlamerStop()
  {
    m_tmFlamerStop=_pTimer->CurrentTick();
    CPlayer &pl = (CPlayer&)*m_penPlayer;
	// deleted by seo
    //PlaySound(pl.m_soWeapon0, SOUND_FL_STOP, SOF_3D|SOF_VOLUMETRIC);
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_StopEffect("FlamethrowerFire");}
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("FlamethrowerStop");}
    //PlaySound(pl.m_soWeapon1, SOUND_FL_STOP, SOF_3D|SOF_VOLUMETRIC);
    if (_pNetwork->IsServer()) {
      FireFlame(); 
    }

    // link last flame with nothing (if not NULL or deleted)
    if (m_penFlame!=NULL && !(m_penFlame->GetFlags()&ENF_DELETED)) {
      ((CProjectile&)*m_penFlame).m_penParticles = NULL;
      m_penFlame = NULL;
      TIME tmTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
    }
  
    m_moWeapon.PlayAnim(FLAMER_ANIM_FIREEND, 0);
    autowait(m_moWeapon.GetAnimLength(FLAMER_ANIM_FIREEND));

    if (_pNetwork->IsServer()) {
      if (m_iNapalm<=0) {
        // select new weapon
        SelectNewWeapon(); 
      }
    }
    jump Idle();
  };
  */

  
  // ***************** FIRE CHAINSAW *****************
  ChainsawFire()
  {
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    
    // set the firing sound level
    pl.m_soWeapon0.Set3DParameters( 50.0f, 5.0f, 1.0f, 1.0f);
    PlaySound( pl.m_soWeapon0, SOUND_CS_BEGINFIRE, SOF_3D|SOF_VOLUMETRIC);
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("ChainsawBeginFire");}
  
    // bring the chainsaw down to cutting height (fire position)
    m_moWeapon.PlayAnim(CHAINSAW_ANIM_WAIT2FIRE, 0);
    autowait(m_moWeapon.GetAnimLength(CHAINSAW_ANIM_WAIT2FIRE)-0.05f);

    CPlayerAnimator &pa=*GetAnimator();
    pa.FireAnimation(BODY_ANIM_MINIGUN_FIRELONG, 0);
    
    CModelObject *pmoTeeth=GetChainSawTeeth();
    if( pmoTeeth!=NULL)
    {
      pmoTeeth->PlayAnim(TEETH_ANIM_ROTATE, AOF_LOOPING|AOF_NORESTART);
    }

    // firing
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    // mute the chainsaw engine sound
    pl.m_soWeaponAmbient.Set3DParameters(30.0f, 3.0f, 0.5f, 1.0f);        
    
    PlaySound(pl.m_soWeapon0, SOUND_CS_FIRE, SOF_3D|SOF_LOOP|SOF_VOLUMETRIC);
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_StopEffect("ChainsawIdle");}
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("ChainsawFire");}

    m_moWeapon.PlayAnim(CHAINSAW_ANIM_FIRE, AOF_LOOPING|AOF_NORESTART);  

    // start teeth rotation
    CModelObject *pmo1 = &(m_moWeapon.GetAttachmentModel(CHAINSAW_ATTACHMENT_BLADE)->amo_moModelObject);
    CModelObject *pmo2 = &(pmo1->GetAttachmentModel(BLADE_ATTACHMENT_TEETH)->amo_moModelObject);
    pmo2->PlayAnim(TEETH_ANIM_ROTATE, AOF_LOOPING);

    while (HoldingFire())// && m_iNapalm>0)
    {
      autowait(CHAINSAW_UPDATETIME);
      if (_pNetwork->IsServer()) {
        // 200 damage per second
        CutWithChainsaw(0, 0, 3.0f, 2.0f, 1.0f,
                        ((GetSP()->sp_bCooperative) ? 200.0f : 250.0f)*CHAINSAW_UPDATETIME);
      }
    }
    
    // bring it back to idle position

    CPlayer &pl = (CPlayer&)*m_penPlayer;
    PlaySound(pl.m_soWeapon0, SOUND_CS_ENDFIRE, SOF_3D|SOF_VOLUMETRIC);
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_StopEffect("ChainsawFire");}
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("ChainsawEnd");}
    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("ChainsawIdle");}
    // restore volume to engine sound
    pl.m_soWeaponAmbient.Set3DParameters( 30.0f, 3.0f, 0.8f, 1.0f);
    
    m_moWeapon.PlayAnim(CHAINSAW_ANIM_FIRE2WAIT, 0);
    autowait(m_moWeapon.GetAnimLength(CHAINSAW_ANIM_FIRE2WAIT));

    // stop teeth rotation
    CModelObject *pmo1 = &(m_moWeapon.GetAttachmentModel(CHAINSAW_ATTACHMENT_BLADE)->amo_moModelObject);
    CModelObject *pmo2 = &(pmo1->GetAttachmentModel(BLADE_ATTACHMENT_TEETH)->amo_moModelObject);
    pmo2->PlayAnim(TEETH_ANIM_DEFAULT, 0);

    CModelObject *pmoTeeth=GetChainSawTeeth();
    if( pmoTeeth!=NULL)
    {
      pmoTeeth->PlayAnim(TEETH_ANIM_DEFAULT, 0);
    }

    jump Idle();
  };


  ChainsawBringUp()
  {
    // bring it back to idle position
    m_moWeapon.PlayAnim(CHAINSAW_ANIM_FIRE2WAIT, 0);
    autowait(m_moWeapon.GetAnimLength(CHAINSAW_ANIM_FIRE2WAIT));
    jump Idle();
  }


  /*
  // ***************** FIRE LASER *****************
  FireLaser()
  {
    // fire one cell
    if (m_iElectricity>0) {
      autowait(0.1f);
      m_moWeapon.PlayAnim(LASER_ANIM_FIRE, AOF_LOOPING|AOF_NORESTART);
      if (_pNetwork->IsServer()) {
        FireLaserRay();
      }
      if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Laser_fire");}
      DecAmmo(m_iElectricity, 1);
      // sound
      SpawnRangeSound(20.0f);
      CPlayer &pl = (CPlayer&)*m_penPlayer;
      // activate barrel anim
      switch(m_iLaserBarrel) {
        case 0: {   // barrel lu
          CModelObject *pmo = &(m_moWeapon.GetAttachmentModel(LASER_ATTACHMENT_LEFTUP)->amo_moModelObject);
          pmo->PlayAnim(BARREL_ANIM_FIRE, 0);
		  // deleted by seo
          //PlaySound(pl.m_soWeapon0, SOUND_LASER_FIRE, SOF_3D|SOF_VOLUMETRIC);
          break; }
        case 3: {   // barrel rd
          CModelObject *pmo = &(m_moWeapon.GetAttachmentModel(LASER_ATTACHMENT_RIGHTDOWN)->amo_moModelObject);
          pmo->PlayAnim(BARREL_ANIM_FIRE, 0);
		  // deleted by seo
          //PlaySound(pl.m_soWeapon1, SOUND_LASER_FIRE, SOF_3D|SOF_VOLUMETRIC);
          break; }
        case 1: {   // barrel ld
          CModelObject *pmo = &(m_moWeapon.GetAttachmentModel(LASER_ATTACHMENT_LEFTDOWN)->amo_moModelObject);
          pmo->PlayAnim(BARREL_ANIM_FIRE, 0);
		  // deleted by seo
          //PlaySound(pl.m_soWeapon2, SOUND_LASER_FIRE, SOF_3D|SOF_VOLUMETRIC);
          break; }
        case 2: {   // barrel ru
          CModelObject *pmo = &(m_moWeapon.GetAttachmentModel(LASER_ATTACHMENT_RIGHTUP)->amo_moModelObject);
            pmo->PlayAnim(BARREL_ANIM_FIRE, 0);
		  // deleted by seo
          //PlaySound(pl.m_soWeapon3, SOUND_LASER_FIRE, SOF_3D|SOF_VOLUMETRIC);
          break; }
      }
      // next barrel
      m_iLaserBarrel = (m_iLaserBarrel+1)&3;
      if (_pNetwork->IsServer()) {
        // no electricity -> change weapon
        //if (m_iElectricity<=0) { SelectNewWeapon(); }
      }
    } else {
      ASSERTALWAYS("Laser - Auto weapon change not working.");
      m_bFireWeapon = m_bHasAmmo = FALSE;
    }
    return EEnd();
  };


  // ***************** FIRE CANNON *****************
  
  CannonFireStart()
  {
    m_tmDrawStartTime = _pTimer->CurrentTick();
    TM_START = _pTimer->CurrentTick();
    F_OFFSET_CHG = 0.0f;
    m_fWeaponDrawPower = 0.0f;
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    if( m_iIronBalls&1) {
      pl.m_soWeapon0.Set3DParameters( 100.0f, 10.0f, 1.0f, 1.0f);
	  // deleted by seo
      //PlaySound( pl.m_soWeapon0, SOUND_CANNON_PREPARE, SOF_3D|SOF_VOLUMETRIC);
    } else {
      pl.m_soWeapon0.Set3DParameters( 150.0f, 15.0f, 1.0f, 1.0f);
	  // deleted by seo
      //PlaySound( pl.m_soWeapon1, SOUND_CANNON_PREPARE, SOF_3D|SOF_VOLUMETRIC);
    }

    if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Canon_prepare");}
    while (HoldingFire() && ((_pTimer->CurrentTick()-TM_START)<1.0f) )
    {
      autowait(_pTimer->TickQuantum);
      INDEX iPower = INDEX((_pTimer->CurrentTick()-TM_START)/_pTimer->TickQuantum);
      F_OFFSET_CHG = 0.25f/(iPower+2);
      m_fWeaponDrawPowerOld = m_fWeaponDrawPower;
      m_fWeaponDrawPower += F_OFFSET_CHG;
    }
    m_tmDrawStartTime = 0.0f;
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    // turn off the sound
    if( m_iIronBalls&1) {
      pl.m_soWeapon0.Stop();
    } else {
      pl.m_soWeapon1.Stop();
    }
    
    // fire one ball
    if ( ((m_iIronBalls>0) && (m_iCurrentWeapon == WEAPON_IRONCANNON)) )
    {
      INDEX iPower = INDEX((_pTimer->CurrentTick()-TM_START)/_pTimer->TickQuantum);
      GetAnimator()->FireAnimation(BODY_ANIM_MINIGUN_FIRELONG, 0);

      FLOAT fRange, fFalloff;
      if (GetSP()->sp_bCooperative) {
        fRange = 100.0f;
        fFalloff = 25.0f;
      } else if (TRUE) {
        fRange = 150.0f;
        fFalloff = 30.0f;
      }

      // adjust volume of cannon firing acording to launch power
      if( m_iIronBalls&1) {
        pl.m_soWeapon2.Set3DParameters( fRange, fFalloff, 0.8f+iPower*0.05f, 1.1f);
		// deleted by seo
        //PlaySound( pl.m_soWeapon2, SOUND_CANNON, SOF_3D|SOF_VOLUMETRIC);
      } else {
        pl.m_soWeapon3.Set3DParameters( fRange, fFalloff, 0.8f+iPower*0.05f, 0.9f);
		// deleted by seo
        //PlaySound( pl.m_soWeapon3, SOUND_CANNON, SOF_3D|SOF_VOLUMETRIC);
      }

      m_moWeapon.PlayAnim(CANNON_ANIM_FIRE, 0);
      if (_pNetwork->IsServer()) {
        FireCannonBall( iPower);
      }

      if(_pNetwork->IsPlayerLocal(m_penPlayer)) {IFeel_PlayEffect("Canon");}
      
      DecAmmo(m_iIronBalls, 1);

      SpawnRangeSound(30.0f);

      TM_START = _pTimer->CurrentTick();
      m_fWeaponDrawPowerOld = m_fWeaponDrawPower;
      while (m_fWeaponDrawPower>0.0f ||
        ((_pTimer->CurrentTick()-TM_START)<m_moWeapon.GetAnimLength(CANNON_ANIM_FIRE)) )
      {
        autowait(_pTimer->TickQuantum);
        m_fWeaponDrawPowerOld = m_fWeaponDrawPower;
        m_fWeaponDrawPower -= F_OFFSET_CHG;
        m_fWeaponDrawPower = ClampDn( m_fWeaponDrawPower, 0.0f);
        F_OFFSET_CHG = F_OFFSET_CHG*2;
      }

      // reset moving part's offset
      ResetWeaponMovingOffset();

      if (_pNetwork->IsServer()) 
	  {
        // no cannon balls -> change weapon
        if ( ((m_iIronBalls<=0) && (m_iCurrentWeapon == WEAPON_IRONCANNON) ) ||
             ((m_iNukeBalls<=0) && (m_iCurrentWeapon == WEAPON_NUKECANNON) ) )
        {
          SelectNewWeapon();
        }
      }
    }
    else
    {
//      ASSERTALWAYS("Cannon - Auto weapon change not working.");
      m_bFireWeapon = m_bHasAmmo = FALSE;
    }
    jump Idle();
  };
  */


  /*
   *  >>>---   RELOAD WEAPON   ---<<<
   */
  Reload() {
    m_bReloadWeapon = FALSE;

	/*
    // reload
    if (m_iCurrentWeapon == WEAPON_COLT) 
	{
      autocall ReloadColt() EEnd;
    } else if (m_iCurrentWeapon == WEAPON_DOUBLECOLT) 
	{
      autocall ReloadDoubleColt() EEnd;
    } else if (m_iCurrentWeapon == WEAPON_PIPEBOMB) {
      autocall ReloadPipeBomb() EEnd;
    }
	*/

    jump Idle();
  };


  /*
   *  >>>---   KNIFE STAND CHANGE   ---<<<
   */
  ChangeKnifeStand(EVoid) {
/*    if (m_iKnifeStand==1) {
      // change from knife stand 1 to stand 3
      m_moWeapon.PlayAnim(KNIFE_ANIM_STAND1TOSTAND3, 0);
      autowait(m_moWeapon.GetAnimLength(KNIFE_ANIM_STAND1TOSTAND3));
      m_iKnifeStand = 3;
    } else if (m_iKnifeStand==3) {
      // change from knife stand 3 to stand 1
      m_moWeapon.PlayAnim(KNIFE_ANIM_STAND3TOSTAND1, 0);
      autowait(m_moWeapon.GetAnimLength(KNIFE_ANIM_STAND3TOSTAND1));
      m_iKnifeStand = 1;
    }
    */
    return EEnd();
  };

  /*
  ChangeToIronCannon(EVoid)
  {
	CModelObject &moLight = m_moWeapon.GetAttachmentModel(CANNON_ATTACHMENT_LIGHT)->amo_moModelObject;
    moLight.PlayAnim( LIGHT_ANIM_DOWN, 0);
    autowait(moLight.GetAnimLength(LIGHT_ANIM_DOWN));

    CModelObject &moNukeBox = m_moWeapon.GetAttachmentModel(CANNON_ATTACHMENT_NUKEBOX)->amo_moModelObject;
    moNukeBox.PlayAnim( NUKEBOX_ANIM_CLOSE, 0);
    autowait(moNukeBox.GetAnimLength(NUKEBOX_ANIM_CLOSE));

    m_iPreviousWeapon = m_iCurrentWeapon;
    m_iCurrentWeapon = WEAPON_IRONCANNON;
    m_iWantedWeapon = m_iCurrentWeapon;

    return EEnd();
  }
  */

/*  ChangeToNukeCannon(EVoid)
  {
    CModelObject &moNukeBox = m_moWeapon.GetAttachmentModel(CANNON_ATTACHMENT_NUKEBOX)->amo_moModelObject;
    moNukeBox.PlayAnim( NUKEBOX_ANIM_OPEN, 0);
    autowait(moNukeBox.GetAnimLength(NUKEBOX_ANIM_OPEN));

    CModelObject &moLight = m_moWeapon.GetAttachmentModel(CANNON_ATTACHMENT_LIGHT)->amo_moModelObject;
    moLight.PlayAnim( LIGHT_ANIM_UP, 0);
    autowait(moLight.GetAnimLength(LIGHT_ANIM_UP));
        
    m_iPreviousWeapon = m_iCurrentWeapon;
    m_iCurrentWeapon = WEAPON_NUKECANNON;
    m_iWantedWeapon = m_iCurrentWeapon;
    
    return EEnd();
  };
*/

  /*
   *  >>>---   BORING WEAPON ANIMATION   ---<<<
   */
  BoringWeaponAnimation() {
    // select new mode change animation
    FLOAT fWait = 0.0f;
    switch (m_iCurrentWeapon) {
      case WEAPON_KNIFE: fWait = KnifeBoring(); break;
      //case WEAPON_COLT: fWait = ColtBoring(); break;
      //case WEAPON_DOUBLECOLT: fWait = DoubleColtBoring(); break;
      //case WEAPON_SINGLESHOTGUN: fWait = SingleShotgunBoring(); break;
      //case WEAPON_DOUBLESHOTGUN: fWait = DoubleShotgunBoring(); break;
      //case WEAPON_TOMMYGUN: fWait = TommyGunBoring(); break;
      //case WEAPON_SNIPER: fWait = SniperBoring(); break;
      //case WEAPON_MINIGUN: fWait = MiniGunBoring(); break;
      //case WEAPON_ROCKETLAUNCHER: fWait = RocketLauncherBoring(); break;
      //case WEAPON_GRENADELAUNCHER: fWait = GrenadeLauncherBoring(); break;
//      case WEAPON_FLAMER: fWait = FlamerBoring(); break;
  //    case WEAPON_CHAINSAW: fWait = ChainsawBoring(); break;
      //case WEAPON_LASER: fWait = LaserBoring(); break;
      //case WEAPON_IRONCANNON: fWait = CannonBoring(); break;
      default: ASSERTALWAYS("Unknown weapon.");
    }
    if (fWait > 0.0f) { autowait(fWait); }

    return EBegin();
  };



  /*
   *  >>>---   NO WEAPON ACTION   ---<<<
   */
  Idle() {

    wait() {
      on (EBegin) : {
        // play default anim
        PlayDefaultAnim();

        // weapon changed
        if (m_bChangeWeapon) {
          if (_pNetwork->IsServer()) {
            ENetSelectWeapon eNetSelect;
            eNetSelect.iWeapon = m_iWantedWeapon;
            SendEvent(eNetSelect,TRUE);
          }
          resume;
        }
        // fire pressed start firing
        if (m_bFireWeapon) {
          if (_pNetwork->IsServer()) {
            SendEvent(ENetFireWeapon(),TRUE);
          }
          resume;
        }
        // reload pressed
        if (m_bReloadWeapon) 
		{
			/*
          if (m_iCurrentWeapon == WEAPON_COLT || m_iCurrentWeapon == WEAPON_DOUBLECOLT) 
		  {
            SendEvent(ENetReloadWeapon(),TRUE);
          }
		  */
          resume;
        }
        resume;
      }
      // select weapon - send the event through the network
      on (ESelectWeapon eSelect) : {
        if (_pNetwork->IsServer()) {
          SelectWeaponChange(eSelect.iWeapon);
          if (m_bChangeWeapon) {
            ENetSelectWeapon eNetSelect;
            eNetSelect.iWeapon = m_iWantedWeapon;
            SendEvent(eNetSelect,TRUE);
          }
        }
        resume;
      }
      // select weapon
      on (ENetSelectWeapon eNetSelect) : {

/* //0610 kwon 삭제.
        // try to change weapon        
        m_tmWeaponChangeRequired = ((CPlayerEntity*)(CEntity*)m_penPlayer)->en_tmEntityTime;
        m_iWantedWeapon = (WeaponType) eNetSelect.iWeapon;
        m_bChangeWeapon = TRUE;
        if (m_bChangeWeapon) {
          jump ChangeWeapon();
        }
*/        
        resume;
      }
      // fire pressed
      on (EFireWeapon) : {
        if (_pNetwork->IsServer()) {
          SendEvent(ENetFireWeapon(),TRUE);
        }
        resume;
      }
      on (ENetFireWeapon) : {
        jump Fire();
      }
      // reload pressed
      on (EReloadWeapon) : 
	  {
		  /*
        if (m_iCurrentWeapon == WEAPON_COLT || m_iCurrentWeapon == WEAPON_DOUBLECOLT) 
		{
          SendEvent(ENetReloadWeapon(),TRUE);
        }
		*/
        resume;
      }
      on (ENetReloadWeapon) : {
        jump Reload();        
      }      
      // boring weapon animation
      on (EBoringWeapon) : {
        call BoringWeaponAnimation();
      }
    }
  };

  // weapons wait here while player is dead, so that stupid animations wouldn't play
  Stopped()
  {
    // make sure we restore all rockets if we are holding the rocket launcher
	  /*
    if (m_iCurrentWeapon==WEAPON_ROCKETLAUNCHER) {
      CModelObject *pmo = &(m_moWeapon.GetAttachmentModel(ROCKETLAUNCHER_ATTACHMENT_ROCKET1)->amo_moModelObject);
      if (pmo) { pmo->StretchModel(FLOAT3D(1, 1, 1)); }
    }
	*/
    // kill all possible sounds, animations, etc
    ResetWeaponMovingOffset();
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    pl.m_soWeapon0.Stop();
    pl.m_soWeapon1.Stop();
    pl.m_soWeapon2.Stop();
    pl.m_soWeapon3.Stop();
    PlayLightAnim(LIGHT_ANIM_NONE, 0);
    wait() {
      // after level change
      on (EPostLevelChange) : { return EBegin(); };
      on (EStart) : { return EBegin(); };
      on (ENetReleaseWeapon) : { 
        m_bFireWeapon = FALSE;
        resume; 
      }      
      otherwise() : { resume; };
    }
  }
};
