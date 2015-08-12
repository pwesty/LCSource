406
%{
#include "StdH.h"
//0322
#include <Engine/Build.h>
//강동민 수정 시작 1차 버그 수정 작업	08.20
#include <Engine/GlobalDefinition.h>
#include "EntitiesMP/Common/CharacterDefinition.h"
#include <Engine/JobInfo.h>
//강동민 수정 끝 1차 버그 수정 작업		08.20

#include "ModelsMP/Player/SeriousSam/Player.h"
#include "ModelsMP/Player/SeriousSam/Body.h"
#include "ModelsMP/Player/SeriousSam/Head.h"

//nclude "Models/Weapons/Knife/KnifeItem.h"
//#include "Models/Weapons/Colt/ColtItem.h"
//nclude "Models/Weapons/Colt/ColtMain.h"
//#include "Models/Weapons/SingleShotgun/SingleShotgunItem.h"
//#include "Models/Weapons/SingleShotgun/Barrels.h"
//#include "Models/Weapons/DoubleShotgun/DoubleShotgunItem.h"
//#include "Models/Weapons/DoubleShotgun/Dshotgunbarrels.h"
//#include "Models/Weapons/TommyGun/TommyGunItem.h"
//#include "Models/Weapons/TommyGun/Body.h"
//#include "Models/Weapons/MiniGun/MiniGunItem.h"
//#include "Models/Weapons/MiniGun/Body.h"
//#include "Models/Weapons/GrenadeLauncher/GrenadeLauncherItem.h"
//#include "Models/Weapons/RocketLauncher/RocketLauncherItem.h"
//#include "ModelsMP/Weapons/Sniper/SniperItem.h"
//#include "ModelsMP/Weapons/Sniper/Sniper.h"
//#include "Models/Weapons/Pipebomb/StickItem.h"
//#include "ModelsMP/Weapons/Flamer/FlamerItem.h"
//#include "ModelsMP/Weapons/Flamer/Body.h"
//#include "ModelsMP/Weapons/Chainsaw/ChainsawItem.h"
//nclude "ModelsMP/Weapons/Chainsaw/ChainsawForPlayer.h"
//nclude "ModelsMP/Weapons/Chainsaw/BladeForPlayer.h"
//nclude "ModelsMP/Weapons/Chainsaw/Body.h"
//#include "Models/Weapons/Laser/LaserItem.h"
//#include "Models/Weapons/GhostBuster/GhostBusterItem.h"
//#include "Models/Weapons/GhostBuster/Effect01.h"
//nclude "Models/Weapons/Cannon/Cannon.h"
#include <Engine/PetInfo.h>
%}

uses "EntitiesMP/Player";
uses "EntitiesMP/PlayerWeapons";
uses "EntitiesMP/Enemy";
uses "EntitiesMP/PetBase";

// input parameter for animator
event EAnimatorInit {
  CEntityID eidPlayer,            // player that owns it
};

%{
// animator action
enum AnimatorAction {
  AA_JUMPDOWN = 0,
  AA_CROUCH,
  AA_RISE,
  AA_PULLWEAPON,
  AA_ATTACK,
};

// fire flare specific
#define FLARE_NONE 0
#define FLARE_REMOVE 1
#define FLARE_ADD 2

//#define	WEAPON	3
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

extern FLOAT plr_fBreathingStrength;
extern FLOAT plr_fViewDampFactor;
extern FLOAT plr_fViewDampLimitGroundUp;
extern FLOAT plr_fViewDampLimitGroundDn;
extern FLOAT plr_fViewDampLimitWater;

ENGINE_API extern INDEX idPlayerWhole_Animation[ANIM_TOTAL];


//..

void CPlayerAnimator_Precache(ULONG ulAvailable)
{
  CDLLEntityClass *pdec = &CPlayerAnimator_DLLClass;

  pdec->PrecacheTexture(TEX_REFL_BWRIPLES01      );
  pdec->PrecacheTexture(TEX_REFL_BWRIPLES02      );
  pdec->PrecacheTexture(TEX_REFL_LIGHTMETAL01    );
  pdec->PrecacheTexture(TEX_REFL_LIGHTBLUEMETAL01);
  pdec->PrecacheTexture(TEX_REFL_DARKMETAL       );
  pdec->PrecacheTexture(TEX_REFL_PURPLE01        );
  pdec->PrecacheTexture(TEX_SPEC_WEAK            );
  pdec->PrecacheTexture(TEX_SPEC_MEDIUM          );
  pdec->PrecacheTexture(TEX_SPEC_STRONG          );
  pdec->PrecacheModel(MODEL_FLARE02);
  pdec->PrecacheTexture(TEXTURE_FLARE02);
  // deleted by seo below 2 lines
  //pdec->PrecacheModel(MODEL_GOLDAMON);
  //pdec->PrecacheTexture(TEXTURE_GOLDAMON);
  pdec->PrecacheTexture(TEX_REFL_GOLD01);
  pdec->PrecacheClass(CLASS_REMINDER);

  // precache shells that drop when firing
//  extern void CPlayerWeaponsEffects_Precache(void);
//  CPlayerWeaponsEffects_Precache();

  // precache weapons player has
  // deleted by seo
  /*if ( ulAvailable&(1<<(WEAPON_KNIFE-1)) ) {
    pdec->PrecacheModel(MODEL_KNIFE                 );
    pdec->PrecacheTexture(TEXTURE_KNIFE);
  }

  if ( ulAvailable&(1<<(WEAPON_COLT-1)) ) {
    pdec->PrecacheModel(MODEL_COLT                  );
    pdec->PrecacheModel(MODEL_COLTCOCK              );
    pdec->PrecacheModel(MODEL_COLTMAIN              );
    pdec->PrecacheModel(MODEL_COLTBULLETS           );
    pdec->PrecacheTexture(TEXTURE_COLTMAIN          );  
    pdec->PrecacheTexture(TEXTURE_COLTBULLETS       );  
    pdec->PrecacheTexture(TEXTURE_COLTBULLETS       );  
  }

  if ( ulAvailable&(1<<(WEAPON_SINGLESHOTGUN-1)) ) {
    pdec->PrecacheModel(MODEL_SINGLESHOTGUN     );    
    pdec->PrecacheModel(MODEL_SS_SLIDER         );    
    pdec->PrecacheModel(MODEL_SS_HANDLE         );    
    pdec->PrecacheModel(MODEL_SS_BARRELS        );    
    pdec->PrecacheTexture(TEXTURE_SS_HANDLE);      
    pdec->PrecacheTexture(TEXTURE_SS_BARRELS);      
  }

  if ( ulAvailable&(1<<(WEAPON_DOUBLESHOTGUN-1)) ) {
    pdec->PrecacheModel(MODEL_DOUBLESHOTGUN        ); 
    pdec->PrecacheModel(MODEL_DS_HANDLE            ); 
    pdec->PrecacheModel(MODEL_DS_BARRELS           ); 
    pdec->PrecacheModel(MODEL_DS_SWITCH            ); 
    pdec->PrecacheTexture(TEXTURE_DS_HANDLE        );   
    pdec->PrecacheTexture(TEXTURE_DS_BARRELS       );   
    pdec->PrecacheTexture(TEXTURE_DS_SWITCH        );   
  }

  if ( ulAvailable&(1<<(WEAPON_TOMMYGUN-1)) ) {
    pdec->PrecacheModel(MODEL_TOMMYGUN              );
    pdec->PrecacheModel(MODEL_TG_BODY               );
    pdec->PrecacheModel(MODEL_TG_SLIDER             );
    pdec->PrecacheTexture(TEXTURE_TG_BODY           );  
  }

  if ( ulAvailable&(1<<(WEAPON_SNIPER-1)) ) {
    pdec->PrecacheModel(MODEL_SNIPER          ); 
    pdec->PrecacheModel(MODEL_SNIPER_BODY     ); 
    pdec->PrecacheTexture(TEXTURE_SNIPER_BODY );   
  }

  if ( ulAvailable&(1<<(WEAPON_MINIGUN-1)) ) {
    pdec->PrecacheModel(MODEL_MINIGUN          );     
    pdec->PrecacheModel(MODEL_MG_BARRELS       );     
    pdec->PrecacheModel(MODEL_MG_BODY          );     
    pdec->PrecacheModel(MODEL_MG_ENGINE        );     
    pdec->PrecacheTexture(TEXTURE_MG_BODY      );       
    pdec->PrecacheTexture(TEXTURE_MG_BARRELS   );       
  }
                                         
  if ( ulAvailable&(1<<(WEAPON_ROCKETLAUNCHER-1)) ) {
    pdec->PrecacheModel(MODEL_ROCKETLAUNCHER   );
    pdec->PrecacheModel(MODEL_RL_BODY          );
    pdec->PrecacheModel(MODEL_RL_ROTATINGPART  );
    pdec->PrecacheModel(MODEL_RL_ROCKET        );
    pdec->PrecacheTexture(TEXTURE_RL_BODY  );
    pdec->PrecacheTexture(TEXTURE_RL_ROCKET);
    pdec->PrecacheTexture(TEXTURE_RL_ROTATINGPART);
  }                                        

  if ( ulAvailable&(1<<(WEAPON_GRENADELAUNCHER-1)) ) {
    pdec->PrecacheModel(MODEL_GRENADELAUNCHER       ); 
    pdec->PrecacheModel(MODEL_GL_BODY               ); 
    pdec->PrecacheModel(MODEL_GL_MOVINGPART         ); 
    pdec->PrecacheModel(MODEL_GL_GRENADE            ); 
    pdec->PrecacheTexture(TEXTURE_GL_BODY           );   
    pdec->PrecacheTexture(TEXTURE_GL_MOVINGPART     );   
  }*/

/*
  if ( ulAvailable&(1<<(WEAPON_PIPEBOMB-1)) ) {
    pdec->PrecacheModel(MODEL_PIPEBOMB_STICK        );
    pdec->PrecacheModel(MODEL_PB_BUTTON             );
    pdec->PrecacheModel(MODEL_PB_SHIELD             );
    pdec->PrecacheModel(MODEL_PB_STICK              );
    pdec->PrecacheModel(MODEL_PB_BOMB               );
    pdec->PrecacheTexture(TEXTURE_PB_STICK          );  
    pdec->PrecacheTexture(TEXTURE_PB_BOMB           );  
  }
*/
  /*
  if ( ulAvailable&(1<<(WEAPON_FLAMER-1)) ) {
    pdec->PrecacheModel(MODEL_FLAMER      );
    pdec->PrecacheModel(MODEL_FL_BODY     );
    pdec->PrecacheModel(MODEL_FL_RESERVOIR);
    pdec->PrecacheModel(MODEL_FL_FLAME    );
    pdec->PrecacheTexture(TEXTURE_FL_BODY );  
    pdec->PrecacheTexture(TEXTURE_FL_FLAME);  
  }
  
  if ( ulAvailable&(1<<(WEAPON_CHAINSAW-1)) ) {
    pdec->PrecacheModel(MODEL_CHAINSAW      );
    pdec->PrecacheModel(MODEL_CS_BODY       );
    pdec->PrecacheModel(MODEL_CS_BLADE      );
    pdec->PrecacheModel(MODEL_CS_TEETH      );
    pdec->PrecacheTexture(TEXTURE_CS_BODY   );  
    pdec->PrecacheTexture(TEXTURE_CS_BLADE  );  
    pdec->PrecacheTexture(TEXTURE_CS_TEETH  );  
  }

  if ( ulAvailable&(1<<(WEAPON_LASER-1)) ) {
    pdec->PrecacheModel(MODEL_LASER     );
    pdec->PrecacheModel(MODEL_LS_BODY   );
    pdec->PrecacheModel(MODEL_LS_BARREL );
    pdec->PrecacheTexture(TEXTURE_LS_BODY  );  
    pdec->PrecacheTexture(TEXTURE_LS_BARREL);  
  }*/
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
  }
*/
  //if ( ulAvailable&(1<<(WEAPON_IRONCANNON-1)) /*||
  //   ulAvailable&(1<<(WEAPON_NUKECANNON-1)) */) {
    //pdec->PrecacheModel(MODEL_CANNON    );
    //pdec->PrecacheModel(MODEL_CN_BODY   );
//    pdec->PrecacheModel(MODEL_CN_NUKEBOX);
//    pdec->PrecacheModel(MODEL_CN_LIGHT);
    //pdec->PrecacheTexture(TEXTURE_CANNON);
  //}
}
%}

class export CPlayerAnimator: CRationalEntity {
name      "Player Animator";
thumbnail "";
features "CanBePredictable";

properties:
  1 CEntityPointer m_penPlayer,               // player which owns it

  5 BOOL m_bReference=FALSE,                  // player has reference (floor)
  6 FLOAT m_fLastActionTime = 0.0f,           // last action time for boring weapon animations
  7 INDEX m_iContent = 0,                     // content type index
  8 BOOL m_bWaitJumpAnim = FALSE,             // wait legs anim (for jump end)
  9 BOOL m_bCrouch = FALSE,                   // player crouch state
 10 BOOL m_iCrouchDownWait = FALSE,           // wait for crouch down
 11 BOOL m_iRiseUpWait = FALSE,               // wait for rise up
 12 BOOL m_bChangeWeapon = FALSE,             // wait for weapon change
 13 BOOL m_bSwim = FALSE,                     // player in water
 14 INDEX m_iFlare = FLARE_REMOVE,            // 0-none, 1-remove, 2-add
 15 INDEX m_iSecondFlare = FLARE_REMOVE,      // 0-none, 1-remove, 2-add
 16 BOOL m_bAttacking = FALSE,                // currently firing weapon/swinging knife
 19 FLOAT m_tmAttackingDue = -1.0f,           // when firing animation is due
 17 FLOAT m_tmFlareAdded = -1.0f,             // for better flare add/remove
 18 BOOL m_bDisableAnimating = FALSE,

// player soft eyes on Y axis
 20 FLOAT3D m_vLastPlayerPosition = FLOAT3D(0,0,0), // last player position for eyes movement
 21 FLOAT m_fEyesYLastOffset = 0.0f,                 // eyes offset from player position
 22 FLOAT m_fEyesYOffset = 0.0f,
 23 FLOAT m_fEyesYSpeed = 0.0f,                      // eyes speed
//0105
 24 BOOL m_bMdl =FALSE,//TRUE,// 
//..
 27 FLOAT m_fWeaponYLastOffset = 0.0f,                 // eyes offset from player position
 28 FLOAT m_fWeaponYOffset = 0.0f,
 29 FLOAT m_fWeaponYSpeed = 0.0f,                      // eyes speed
 // recoil pitch
// 24 FLOAT m_fRecoilLastOffset = 0.0f,   // eyes offset from player position
// 25 FLOAT m_fRecoilOffset = 0.0f,
// 26 FLOAT m_fRecoilSpeed = 0.0f,        // eyes speed

// player banking when moving
 30 BOOL m_bMoving = FALSE features(EPROPF_NETSEND),
 31 FLOAT m_fMoveLastBanking = 0.0f,
 32 FLOAT m_fMoveBanking = 0.0f,
 33 BOOL m_iMovingSide = 0 features(EPROPF_NETSEND), 
 34 BOOL m_bSidestepBankingLeft = FALSE,
 35 BOOL m_bSidestepBankingRight = FALSE,
 36 FLOAT m_fSidestepLastBanking = 0.0f,
 37 FLOAT m_fSidestepBanking = 0.0f,
 38 INDEX m_iWeaponLast = -1 features(EPROPF_NETSEND),
 39 FLOAT m_fBodyAnimTime = -1.0f features(EPROPF_NETSEND),

// ID of player entity that owns the animator - for init over net
 40 INDEX m_iPlayerID = -1 features(EPROPF_NETSEND),

 41 FLOAT m_fAttack1AnimTime = -1.0f,
 42 FLOAT m_fAttack2AnimTime = -1.0f,
 43 FLOAT m_fSkill1AnimTime = -1.0f,
 44 FLOAT m_fSkill2AnimTime = -1.0f,
 45 FLOAT m_fAttack3AnimTime = -1.0f,
 46 FLOAT m_fAttack4AnimTime = -1.0f,

 47 FLOAT m_fExtAttack1AnimTime = -1.0f,
 48 FLOAT m_fExtAttack2AnimTime = -1.0f,
 49 FLOAT m_fExtAttack3AnimTime = -1.0f,
 50 FLOAT m_fExtAttack4AnimTime = -1.0f,

{
  CModelObject *pmoModel;
}

components:
  1 class   CLASS_REMINDER              "Classes\\Reminder.ecl",
// ************** KNIFE **************
 // deleted by seo
 /*20 model   MODEL_KNIFE                 "Data\\Models\\Weapons\\Knife\\KnifeItem.mdl",
 22 texture TEXTURE_KNIFE               "Data\\Models\\Weapons\\Knife\\KnifeItem.tex",
 
// ************** COLT **************
 30 model   MODEL_COLT                  "Data\\Models\\Weapons\\Colt\\ColtItem.mdl",
 31 model   MODEL_COLTCOCK              "Data\\Models\\Weapons\\Colt\\ColtCock.mdl",
 32 model   MODEL_COLTMAIN              "Data\\Models\\Weapons\\Colt\\ColtMain.mdl",
 33 model   MODEL_COLTBULLETS           "Data\\Models\\Weapons\\Colt\\ColtBullets.mdl",
 34 texture TEXTURE_COLTBULLETS         "Data\\Models\\Weapons\\Colt\\ColtBullets.tex",
 35 texture TEXTURE_COLTMAIN            "Data\\Models\\Weapons\\Colt\\ColtMain.tex",
 36 texture TEXTURE_COLTCOCK            "Data\\Models\\Weapons\\Colt\\ColtCock.tex",

// ************** SINGLE SHOTGUN ************
 40 model   MODEL_SINGLESHOTGUN         "Data\\Models\\Weapons\\SingleShotgun\\SingleShotgunItem.mdl",
 41 model   MODEL_SS_SLIDER             "Data\\Models\\Weapons\\SingleShotgun\\Slider.mdl",
 42 model   MODEL_SS_HANDLE             "Data\\Models\\Weapons\\SingleShotgun\\Handle.mdl",
 43 model   MODEL_SS_BARRELS            "Data\\Models\\Weapons\\SingleShotgun\\Barrels.mdl",
 44 texture TEXTURE_SS_HANDLE           "Data\\Models\\Weapons\\SingleShotgun\\Handle.tex",
 45 texture TEXTURE_SS_BARRELS          "Data\\Models\\Weapons\\SingleShotgun\\Barrels.tex",

// ************** DOUBLE SHOTGUN **************
 50 model   MODEL_DOUBLESHOTGUN         "Data\\Models\\Weapons\\DoubleShotgun\\DoubleShotgunItem.mdl",
 51 model   MODEL_DS_HANDLE             "Data\\Models\\Weapons\\DoubleShotgun\\Dshotgunhandle.mdl",
 52 model   MODEL_DS_BARRELS            "Data\\Models\\Weapons\\DoubleShotgun\\Dshotgunbarrels.mdl",
 54 model   MODEL_DS_SWITCH             "Data\\Models\\Weapons\\DoubleShotgun\\Switch.mdl",
 56 texture TEXTURE_DS_HANDLE           "Data\\Models\\Weapons\\DoubleShotgun\\Handle.tex",
 57 texture TEXTURE_DS_BARRELS          "Data\\Models\\Weapons\\DoubleShotgun\\Barrels.tex",
 58 texture TEXTURE_DS_SWITCH           "Data\\Models\\Weapons\\DoubleShotgun\\Switch.tex",

// ************** TOMMYGUN **************
 70 model   MODEL_TOMMYGUN              "Data\\Models\\Weapons\\TommyGun\\TommyGunItem.mdl",
 71 model   MODEL_TG_BODY               "Data\\Models\\Weapons\\TommyGun\\Body.mdl",
 72 model   MODEL_TG_SLIDER             "Data\\Models\\Weapons\\TommyGun\\Slider.mdl",
 73 texture TEXTURE_TG_BODY             "Data\\Models\\Weapons\\TommyGun\\Body.tex",

// ************** MINIGUN **************
 80 model   MODEL_MINIGUN               "Data\\Models\\Weapons\\MiniGun\\MiniGunItem.mdl",
 81 model   MODEL_MG_BARRELS            "Data\\Models\\Weapons\\MiniGun\\Barrels.mdl",
 82 model   MODEL_MG_BODY               "Data\\Models\\Weapons\\MiniGun\\Body.mdl",
 83 model   MODEL_MG_ENGINE             "Data\\Models\\Weapons\\MiniGun\\Engine.mdl",
 84 texture TEXTURE_MG_BODY             "Data\\Models\\Weapons\\MiniGun\\Body.tex",
 99 texture TEXTURE_MG_BARRELS          "Data\\Models\\Weapons\\MiniGun\\Barrels.tex",

// ************** ROCKET LAUNCHER **************
 90 model   MODEL_ROCKETLAUNCHER        "Data\\Models\\Weapons\\RocketLauncher\\RocketLauncherItem.mdl",
 91 model   MODEL_RL_BODY               "Data\\Models\\Weapons\\RocketLauncher\\Body.mdl",
 92 texture TEXTURE_RL_BODY             "Data\\Models\\Weapons\\RocketLauncher\\Body.tex",
 93 model   MODEL_RL_ROTATINGPART       "Data\\Models\\Weapons\\RocketLauncher\\RotatingPart.mdl",
 94 texture TEXTURE_RL_ROTATINGPART     "Data\\Models\\Weapons\\RocketLauncher\\RotatingPart.tex",
 95 model   MODEL_RL_ROCKET             "Data\\Models\\Weapons\\RocketLauncher\\Projectile\\Rocket.mdl",
 96 texture TEXTURE_RL_ROCKET           "Data\\Models\\Weapons\\RocketLauncher\\Projectile\\Rocket.tex",

// ************** GRENADE LAUNCHER **************
100 model   MODEL_GRENADELAUNCHER       "Data\\Models\\Weapons\\GrenadeLauncher\\GrenadeLauncherItem.mdl",
101 model   MODEL_GL_BODY               "Data\\Models\\Weapons\\GrenadeLauncher\\Body.mdl",
102 model   MODEL_GL_MOVINGPART         "Data\\Models\\Weapons\\GrenadeLauncher\\MovingPipe.mdl",
103 model   MODEL_GL_GRENADE            "Data\\Models\\Weapons\\GrenadeLauncher\\GrenadeBack.mdl",
104 texture TEXTURE_GL_BODY             "Data\\Models\\Weapons\\GrenadeLauncher\\Body.tex",
105 texture TEXTURE_GL_MOVINGPART       "Data\\Models\\Weapons\\GrenadeLauncher\\MovingPipe.tex",

// ************** SNIPER **************
110 model   MODEL_SNIPER                "Data\\ModelsMP\\Weapons\\Sniper\\Sniper.mdl",
111 model   MODEL_SNIPER_BODY           "Data\\ModelsMP\\Weapons\\Sniper\\Body.mdl",
112 texture TEXTURE_SNIPER_BODY         "Data\\ModelsMP\\Weapons\\Sniper\\Body.tex",*/

/*
// ************** PIPEBOMB **************
110 model   MODEL_PIPEBOMB_STICK        "Data\\Models\\Weapons\\Pipebomb\\StickItem.mdl",
112 model   MODEL_PB_BUTTON             "Data\\Models\\Weapons\\Pipebomb\\Button.mdl",
113 model   MODEL_PB_SHIELD             "Data\\Models\\Weapons\\Pipebomb\\Shield.mdl",
114 model   MODEL_PB_STICK              "Data\\Models\\Weapons\\Pipebomb\\Stick.mdl",
115 model   MODEL_PB_BOMB               "Data\\Models\\Weapons\\Pipebomb\\Bomb.mdl",
116 texture TEXTURE_PB_STICK            "Data\\Models\\Weapons\\Pipebomb\\Stick.tex",
117 texture TEXTURE_PB_BOMB             "Data\\Models\\Weapons\\Pipebomb\\Bomb.tex",
*/

// ************** FLAMER **************
// deleted by seo
/*130 model   MODEL_FLAMER                "Data\\ModelsMP\\Weapons\\Flamer\\FlamerItem.mdl",
131 model   MODEL_FL_BODY               "Data\\ModelsMP\\Weapons\\Flamer\\Body.mdl",
132 model   MODEL_FL_RESERVOIR          "Data\\ModelsMP\\Weapons\\Flamer\\FuelReservoir.mdl",
133 model   MODEL_FL_FLAME              "Data\\ModelsMP\\Weapons\\Flamer\\Flame.mdl",
134 texture TEXTURE_FL_BODY             "Data\\ModelsMP\\Weapons\\Flamer\\Body.tex",
135 texture TEXTURE_FL_FLAME            "Data\\ModelsMP\\Effects\\Flame\\Flame.tex",
136 texture TEXTURE_FL_FUELRESERVOIR    "Data\\ModelsMP\\Weapons\\Flamer\\FuelReservoir.tex",

// ************** LASER **************
140 model   MODEL_LASER                 "Data\\Models\\Weapons\\Laser\\LaserItem.mdl",
141 model   MODEL_LS_BODY               "Data\\Models\\Weapons\\Laser\\Body.mdl",
142 model   MODEL_LS_BARREL             "Data\\Models\\Weapons\\Laser\\Barrel.mdl",
143 texture TEXTURE_LS_BODY             "Data\\Models\\Weapons\\Laser\\Body.tex",
144 texture TEXTURE_LS_BARREL           "Data\\Models\\Weapons\\Laser\\Barrel.tex",

// ************** CHAINSAW **************
150 model   MODEL_CHAINSAW              "Data\\ModelsMP\\Weapons\\Chainsaw\\ChainsawForPlayer.mdl",
151 model   MODEL_CS_BODY               "Data\\ModelsMP\\Weapons\\Chainsaw\\BodyForPlayer.mdl",
152 model   MODEL_CS_BLADE              "Data\\ModelsMP\\Weapons\\Chainsaw\\Blade.mdl",
153 model   MODEL_CS_TEETH              "Data\\ModelsMP\\Weapons\\Chainsaw\\Teeth.mdl",
154 texture TEXTURE_CS_BODY             "Data\\ModelsMP\\Weapons\\Chainsaw\\Body.tex",
155 texture TEXTURE_CS_BLADE            "Data\\ModelsMP\\Weapons\\Chainsaw\\Blade.tex",
156 texture TEXTURE_CS_TEETH            "Data\\ModelsMP\\Weapons\\Chainsaw\\Teeth.tex",
*/
// ************** GHOSTBUSTER **************
/*
150 model   MODEL_GHOSTBUSTER           "Data\\Models\\Weapons\\GhostBuster\\GhostBusterItem.mdl",
151 model   MODEL_GB_BODY               "Data\\Models\\Weapons\\GhostBuster\\Body.mdl",
152 model   MODEL_GB_ROTATOR            "Data\\Models\\Weapons\\GhostBuster\\Rotator.mdl",
153 model   MODEL_GB_EFFECT1            "Data\\Models\\Weapons\\GhostBuster\\Effect01.mdl",
154 model   MODEL_GB_EFFECT1FLARE       "Data\\Models\\Weapons\\GhostBuster\\EffectFlare01.mdl",
155 texture TEXTURE_GB_ROTATOR          "Data\\Models\\Weapons\\GhostBuster\\Rotator.tex",
156 texture TEXTURE_GB_BODY             "Data\\Models\\Weapons\\GhostBuster\\Body.tex",
157 texture TEXTURE_GB_LIGHTNING        "Data\\Models\\Weapons\\GhostBuster\\Lightning.tex",
158 texture TEXTURE_GB_FLARE            "Data\\Models\\Weapons\\GhostBuster\\EffectFlare.tex",
*/
// ************** CANNON **************
// deleted by seo
//170 model   MODEL_CANNON                "Data\\Models\\Weapons\\Cannon\\Cannon.mdl",
//171 model   MODEL_CN_BODY               "Data\\Models\\Weapons\\Cannon\\Body.mdl",
//173 texture TEXTURE_CANNON              "Data\\Models\\Weapons\\Cannon\\Body.tex",
// end
//174 model   MODEL_CN_NUKEBOX            "Data\\Models\\Weapons\\Cannon\\NukeBox.mdl",
//175 model   MODEL_CN_LIGHT              "Data\\Models\\Weapons\\Cannon\\Light.mdl",

// ************** AMON STATUE **************
// deleted by seo below 2 lines
//180 model   MODEL_GOLDAMON                "Data\\Models\\Ages\\Egypt\\Gods\\Amon\\AmonGold.mdl",
//181 texture TEXTURE_GOLDAMON              "Data\\Models\\Ages\\Egypt\\Gods\\Amon\\AmonGold.tex",

// ************** REFLECTIONS **************
200 texture TEX_REFL_BWRIPLES01         "Data\\Models\\ReflectionTextures\\BWRiples01.tex",
201 texture TEX_REFL_BWRIPLES02         "Data\\Models\\ReflectionTextures\\BWRiples02.tex",
202 texture TEX_REFL_LIGHTMETAL01       "Data\\Models\\ReflectionTextures\\LightMetal01.tex",
203 texture TEX_REFL_LIGHTBLUEMETAL01   "Data\\Models\\ReflectionTextures\\LightBlueMetal01.tex",
204 texture TEX_REFL_DARKMETAL          "Data\\Models\\ReflectionTextures\\DarkMetal.tex",
205 texture TEX_REFL_PURPLE01           "Data\\Models\\ReflectionTextures\\Purple01.tex",
206 texture TEX_REFL_GOLD01               "Data\\Models\\ReflectionTextures\\Gold01.tex",

// ************** SPECULAR **************
210 texture TEX_SPEC_WEAK               "Data\\Models\\SpecularTextures\\Weak.tex",
211 texture TEX_SPEC_MEDIUM             "Data\\Models\\SpecularTextures\\Medium.tex",
212 texture TEX_SPEC_STRONG             "Data\\Models\\SpecularTextures\\Strong.tex",

// ************** FLARES **************
250 model   MODEL_FLARE02               "Data\\Models\\Effects\\Weapons\\Flare02\\Flare.mdl",
251 texture TEXTURE_FLARE02             "Data\\Models\\Effects\\Weapons\\Flare02\\Flare.tex",


functions:
  // player animator is initialized through player.es
  virtual void InitializeFromNet()
  {
    return;
  }  
  
  /* Read from stream. */
  void Read_t( CTStream *istr,BOOL bNetwork) // throw char *
  { 
    CRationalEntity::Read_t(istr,bNetwork);
  }

  void Precache(void)
  {
    INDEX iAvailableWeapons = ((CPlayerWeapons&)*(((CPlayer&)*m_penPlayer).m_penWeapons)).m_iAvailableWeapons;
    CPlayerAnimator_Precache(iAvailableWeapons);
  }
  
  CPlayer *GetPlayer(void)
  {
    return ((CPlayer*)&*m_penPlayer);
  }
  CModelObject *GetBody(void)
  {
    CAttachmentModelObject *pamoBody = GetPlayer()->GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO);
    if (pamoBody==NULL) {
      return NULL;
    }
    return &pamoBody->amo_moModelObject;
  }
  CModelObject *GetBodyRen(void)
  {
    CAttachmentModelObject *pamoBody = GetPlayer()->m_moRender.GetAttachmentModel(PLAYER_ATTACHMENT_TORSO);
    if (pamoBody==NULL) {
      return NULL;
    }
    return &pamoBody->amo_moModelObject;
  }

  // Set components
  void SetComponents(CModelObject *mo, ULONG ulIDModel, ULONG ulIDTexture,
                     ULONG ulIDReflectionTexture, ULONG ulIDSpecularTexture, ULONG ulIDBumpTexture) {
    // model data
    mo->SetData(GetModelDataForComponent(ulIDModel));
    // texture data
    mo->mo_toTexture.SetData(GetTextureDataForComponent(ulIDTexture));
    // reflection texture data
    if (ulIDReflectionTexture>0) {
      mo->mo_toReflection.SetData(GetTextureDataForComponent(ulIDReflectionTexture));
    } else {
      mo->mo_toReflection.SetData(NULL);
    }
    // specular texture data
    if (ulIDSpecularTexture>0) {
      mo->mo_toSpecular.SetData(GetTextureDataForComponent(ulIDSpecularTexture));
    } else {
      mo->mo_toSpecular.SetData(NULL);
    }
    // bump texture data
    if (ulIDBumpTexture>0) {
      mo->mo_toBump.SetData(GetTextureDataForComponent(ulIDBumpTexture));
    } else {
      mo->mo_toBump.SetData(NULL);
    }
    ModelChangeNotify();
  };

  // Add attachment model
  void AddAttachmentModel(CModelObject *mo, INDEX iAttachment, ULONG ulIDModel, ULONG ulIDTexture,
                          ULONG ulIDReflectionTexture, ULONG ulIDSpecularTexture, ULONG ulIDBumpTexture) {
    SetComponents(&mo->AddAttachmentModel(iAttachment)->amo_moModelObject, ulIDModel, 
                  ulIDTexture, ulIDReflectionTexture, ulIDSpecularTexture, ulIDBumpTexture);
  };

  // Add weapon attachment
  void AddWeaponAttachment(INDEX iAttachment, ULONG ulIDModel, ULONG ulIDTexture,
                           ULONG ulIDReflectionTexture, ULONG ulIDSpecularTexture, ULONG ulIDBumpTexture) {
    AddAttachmentModel(pmoModel, iAttachment, ulIDModel, ulIDTexture,
                       ulIDReflectionTexture, ulIDSpecularTexture, ulIDBumpTexture);
  };

  // set active attachment (model)
  void SetAttachment(INDEX iAttachment) {
    pmoModel = &(pmoModel->GetAttachmentModel(iAttachment)->amo_moModelObject);
  };

  /*
  // synchronize any possible weapon attachment(s) with default appearance
  void SyncWeapon(void)
  {
    CModelObject *pmoBodyRen = GetBodyRen();
    CModelObject *pmoBodyDef = GetBody();
    // for each weapon attachment
    for (INDEX iWeapon = BODY_ATTACHMENT_COLT_RIGHT; iWeapon<=BODY_ATTACHMENT_ITEM; iWeapon++) {
      CAttachmentModelObject *pamoWeapDef = pmoBodyDef->GetAttachmentModel(iWeapon);
      CAttachmentModelObject *pamoWeapRen = pmoBodyRen->GetAttachmentModel(iWeapon);
      // if it doesn't exist in either
      if (pamoWeapRen==NULL && pamoWeapDef==NULL) {
        // just skip it
        NOTHING;

      // if exists only in rendering model
      } else if (pamoWeapRen!=NULL && pamoWeapDef==NULL) {
        // remove it from rendering
        delete pamoWeapRen;

      // if exists only in default
      } else if (pamoWeapRen==NULL && pamoWeapDef!=NULL) {
        // add it to rendering
        pamoWeapRen = pmoBodyRen->AddAttachmentModel(iWeapon);
        pamoWeapRen->amo_plRelative = pamoWeapDef->amo_plRelative;
        pamoWeapRen->amo_moModelObject.Copy(pamoWeapDef->amo_moModelObject);

      // if exists in both
      } else {
        // just synchronize
        pamoWeapRen->amo_plRelative = pamoWeapDef->amo_plRelative;
        pamoWeapRen->amo_moModelObject.Synchronize(pamoWeapDef->amo_moModelObject);
      }
    }
  }
  */

  /*
  // set weapon
  void SetWeapon(void) 
  {  
    INDEX iWeapon = ((CPlayerWeapons&)*(((CPlayer&)*m_penPlayer).m_penWeapons)).m_iCurrentWeapon;
    if (m_iWeaponLast>WEAPON_NONE && m_iWeaponLast<WEAPON_LAST) {
      RemoveWeapon();
    }
    m_iWeaponLast = iWeapon;
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    pmoModel = &(pl.GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject);
    switch (iWeapon) {
    // *********** KNIFE ***********
      case WEAPON_KNIFE:
        AddWeaponAttachment(BODY_ATTACHMENT_KNIFE, MODEL_KNIFE,
                            TEXTURE_KNIFE, TEX_REFL_BWRIPLES02, TEX_SPEC_WEAK, 0);
        break;

    // *********** DOUBLE COLT ***********
      case WEAPON_DOUBLECOLT:
        AddWeaponAttachment(BODY_ATTACHMENT_COLT_LEFT, MODEL_COLT, TEXTURE_COLTMAIN, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_COLT_LEFT);
        AddWeaponAttachment(COLTITEM_ATTACHMENT_BULLETS, MODEL_COLTBULLETS,
                            TEXTURE_COLTBULLETS, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(COLTITEM_ATTACHMENT_COCK, MODEL_COLTCOCK,
                            TEXTURE_COLTCOCK, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(COLTITEM_ATTACHMENT_BODY, MODEL_COLTMAIN,
                            TEXTURE_COLTMAIN, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        SetAttachment(COLTITEM_ATTACHMENT_BODY);
        AddWeaponAttachment(COLTMAIN_ATTACHMENT_FLARE, MODEL_FLARE02, TEXTURE_FLARE02, 0, 0, 0);
        // reset to player body
        pmoModel = &(pl.GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject);

    // *********** COLT ***********
      case WEAPON_COLT:
        AddWeaponAttachment(BODY_ATTACHMENT_COLT_RIGHT, MODEL_COLT, TEXTURE_COLTMAIN, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_COLT_RIGHT);
        AddWeaponAttachment(COLTITEM_ATTACHMENT_BULLETS, MODEL_COLTBULLETS,
                            TEXTURE_COLTBULLETS, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(COLTITEM_ATTACHMENT_COCK, MODEL_COLTCOCK,
                            TEXTURE_COLTCOCK, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(COLTITEM_ATTACHMENT_BODY, MODEL_COLTMAIN,
                            TEXTURE_COLTMAIN, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        SetAttachment(COLTITEM_ATTACHMENT_BODY);
        AddWeaponAttachment(COLTMAIN_ATTACHMENT_FLARE, MODEL_FLARE02, TEXTURE_FLARE02, 0, 0, 0);
        break;

    // *********** SINGLE SHOTGUN ***********
      case WEAPON_SINGLESHOTGUN:
        AddWeaponAttachment(BODY_ATTACHMENT_SINGLE_SHOTGUN, MODEL_SINGLESHOTGUN, TEXTURE_SS_HANDLE, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_SINGLE_SHOTGUN);
        AddWeaponAttachment(SINGLESHOTGUNITEM_ATTACHMENT_BARRELS, MODEL_SS_BARRELS,
                            TEXTURE_SS_BARRELS, TEX_REFL_DARKMETAL, TEX_SPEC_WEAK, 0);
        AddWeaponAttachment(SINGLESHOTGUNITEM_ATTACHMENT_HANDLE, MODEL_SS_HANDLE,
                            TEXTURE_SS_HANDLE, TEX_REFL_DARKMETAL, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(SINGLESHOTGUNITEM_ATTACHMENT_SLIDER, MODEL_SS_SLIDER,
                            TEXTURE_SS_BARRELS, TEX_REFL_DARKMETAL, TEX_SPEC_MEDIUM, 0);
        SetAttachment(SINGLESHOTGUNITEM_ATTACHMENT_BARRELS);
        AddWeaponAttachment(BARRELS_ATTACHMENT_FLARE, MODEL_FLARE02, TEXTURE_FLARE02, 0, 0, 0);
        break;

    // *********** DOUBLE SHOTGUN ***********
      case WEAPON_DOUBLESHOTGUN:
        AddWeaponAttachment(BODY_ATTACHMENT_DOUBLE_SHOTGUN, MODEL_DOUBLESHOTGUN, TEXTURE_DS_HANDLE, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_DOUBLE_SHOTGUN);
        AddWeaponAttachment(DOUBLESHOTGUNITEM_ATTACHMENT_BARRELS, MODEL_DS_BARRELS,
                            TEXTURE_DS_BARRELS, TEX_REFL_BWRIPLES01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(DOUBLESHOTGUNITEM_ATTACHMENT_HANDLE, MODEL_DS_HANDLE,
                            TEXTURE_DS_HANDLE, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(DOUBLESHOTGUNITEM_ATTACHMENT_SWITCH, MODEL_DS_SWITCH,
                            TEXTURE_DS_SWITCH, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        SetAttachment(DOUBLESHOTGUNITEM_ATTACHMENT_BARRELS);
        AddWeaponAttachment(DSHOTGUNBARRELS_ATTACHMENT_FLARE, MODEL_FLARE02, TEXTURE_FLARE02, 0, 0, 0);
        break;


    // *********** TOMMYGUN ***********
      case WEAPON_TOMMYGUN:
        AddWeaponAttachment(BODY_ATTACHMENT_TOMMYGUN, MODEL_TOMMYGUN, TEXTURE_TG_BODY, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_TOMMYGUN);
        AddWeaponAttachment(TOMMYGUNITEM_ATTACHMENT_BODY, MODEL_TG_BODY, TEXTURE_TG_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(TOMMYGUNITEM_ATTACHMENT_SLIDER, MODEL_TG_SLIDER, TEXTURE_TG_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        SetAttachment(TOMMYGUNITEM_ATTACHMENT_BODY);
        AddWeaponAttachment(BODY_ATTACHMENT_FLARE, MODEL_FLARE02, TEXTURE_FLARE02, 0, 0, 0);
        break;

    // *********** SNIPER ***********
      case WEAPON_SNIPER:
        AddWeaponAttachment(BODY_ATTACHMENT_FLAMER, MODEL_SNIPER, TEXTURE_SNIPER_BODY, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_FLAMER);
        AddWeaponAttachment(SNIPERITEM_ATTACHMENT_BODY, MODEL_SNIPER_BODY, TEXTURE_SNIPER_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        SetAttachment(SNIPERITEM_ATTACHMENT_BODY);
        AddWeaponAttachment(BODY_ATTACHMENT_FLARE, MODEL_FLARE02, TEXTURE_FLARE02, 0, 0, 0);
        break;

    // *********** MINIGUN ***********
      case WEAPON_MINIGUN:
        AddWeaponAttachment(BODY_ATTACHMENT_MINIGUN, MODEL_MINIGUN, TEXTURE_MG_BODY, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_MINIGUN);
        AddWeaponAttachment(MINIGUNITEM_ATTACHMENT_BARRELS, MODEL_MG_BARRELS, TEXTURE_MG_BARRELS, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0); 
        AddWeaponAttachment(MINIGUNITEM_ATTACHMENT_BODY, MODEL_MG_BODY, TEXTURE_MG_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);          
        AddWeaponAttachment(MINIGUNITEM_ATTACHMENT_ENGINE, MODEL_MG_ENGINE, TEXTURE_MG_BARRELS, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);      
        SetAttachment(MINIGUNITEM_ATTACHMENT_BODY);
        AddWeaponAttachment(BODY_ATTACHMENT_FLARE, MODEL_FLARE02, TEXTURE_FLARE02, 0, 0, 0);
        break;

    // *********** ROCKET LAUNCHER ***********
      case WEAPON_ROCKETLAUNCHER:
        AddWeaponAttachment(BODY_ATTACHMENT_ROCKET_LAUNCHER, MODEL_ROCKETLAUNCHER, TEXTURE_RL_BODY, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_ROCKET_LAUNCHER);
        AddWeaponAttachment(ROCKETLAUNCHERITEM_ATTACHMENT_BODY, MODEL_RL_BODY, TEXTURE_RL_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(ROCKETLAUNCHERITEM_ATTACHMENT_ROTATINGPART, MODEL_RL_ROTATINGPART, TEXTURE_RL_ROTATINGPART, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(ROCKETLAUNCHERITEM_ATTACHMENT_ROCKET1, MODEL_RL_ROCKET, TEXTURE_RL_ROCKET, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(ROCKETLAUNCHERITEM_ATTACHMENT_ROCKET2, MODEL_RL_ROCKET, TEXTURE_RL_ROCKET, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(ROCKETLAUNCHERITEM_ATTACHMENT_ROCKET3, MODEL_RL_ROCKET, TEXTURE_RL_ROCKET, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(ROCKETLAUNCHERITEM_ATTACHMENT_ROCKET4, MODEL_RL_ROCKET, TEXTURE_RL_ROCKET, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        break;

    // *********** GRENADE LAUNCHER ***********
      case WEAPON_GRENADELAUNCHER:
        AddWeaponAttachment(BODY_ATTACHMENT_GRENADE_LAUNCHER, MODEL_GRENADELAUNCHER, TEXTURE_GL_BODY, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_GRENADE_LAUNCHER);
        AddWeaponAttachment(GRENADELAUNCHERITEM_ATTACHMENT_BODY, MODEL_GL_BODY, TEXTURE_GL_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(GRENADELAUNCHERITEM_ATTACHMENT_MOVING_PART, MODEL_GL_MOVINGPART, TEXTURE_GL_MOVINGPART, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(GRENADELAUNCHERITEM_ATTACHMENT_GRENADE, MODEL_GL_GRENADE, TEXTURE_GL_MOVINGPART, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        break;*/
      // end seo

/*    // *********** PIPEBOMB ***********
      case WEAPON_PIPEBOMB:
        AddWeaponAttachment(BODY_ATTACHMENT_COLT_RIGHT, MODEL_PIPEBOMB_STICK, TEXTURE_PB_STICK, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_COLT_RIGHT);
        AddWeaponAttachment(STICKITEM_ATTACHMENT_STICK, MODEL_PB_STICK, TEXTURE_PB_STICK, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(STICKITEM_ATTACHMENT_SHIELD, MODEL_PB_SHIELD, TEXTURE_PB_STICK, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(STICKITEM_ATTACHMENT_BUTTON, MODEL_PB_BUTTON, TEXTURE_PB_STICK, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        // reset to player body
        pmoModel = &(pl.GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject);
        AddWeaponAttachment(BODY_ATTACHMENT_COLT_LEFT, MODEL_PB_BOMB, TEXTURE_PB_BOMB, TEX_REFL_LIGHTBLUEMETAL01, TEX_SPEC_MEDIUM, 0);
        break;
*/
    // *********** FLAMER ***********
      /*case WEAPON_FLAMER:
        AddWeaponAttachment(BODY_ATTACHMENT_FLAMER, MODEL_FLAMER, TEXTURE_FL_BODY, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_FLAMER);
        AddWeaponAttachment(FLAMERITEM_ATTACHMENT_BODY, MODEL_FL_BODY, TEXTURE_FL_BODY, TEX_REFL_BWRIPLES02, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(FLAMERITEM_ATTACHMENT_FUEL, MODEL_FL_RESERVOIR, TEXTURE_FL_FUELRESERVOIR, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(FLAMERITEM_ATTACHMENT_FLAME, MODEL_FL_FLAME, TEXTURE_FL_FLAME, 0, 0, 0);
        break;

    // *********** CHAINSAW ***********
      case WEAPON_CHAINSAW: {
        AddWeaponAttachment(BODY_ATTACHMENT_MINIGUN, MODEL_CHAINSAW, TEXTURE_CS_BODY, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_MINIGUN);
        AddWeaponAttachment(CHAINSAWFORPLAYER_ATTACHMENT_CHAINSAW, MODEL_CS_BODY, TEXTURE_CS_BODY, TEX_REFL_BWRIPLES02, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(CHAINSAWFORPLAYER_ATTACHMENT_BLADE, MODEL_CS_BLADE, TEXTURE_CS_BLADE, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        CModelObject *pmo = pmoModel;
        SetAttachment(CHAINSAWFORPLAYER_ATTACHMENT_BLADE);
        AddWeaponAttachment(BLADEFORPLAYER_ATTACHMENT_TEETH, MODEL_CS_TEETH, TEXTURE_CS_TEETH, 0, 0, 0);
        break; }

    // *********** LASER ***********
      case WEAPON_LASER:
        AddWeaponAttachment(BODY_ATTACHMENT_LASER, MODEL_LASER, TEXTURE_LS_BODY, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_LASER);
        AddWeaponAttachment(LASERITEM_ATTACHMENT_BODY, MODEL_LS_BODY, TEXTURE_LS_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(LASERITEM_ATTACHMENT_LEFTUP,    MODEL_LS_BARREL, TEXTURE_LS_BARREL, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(LASERITEM_ATTACHMENT_LEFTDOWN,  MODEL_LS_BARREL, TEXTURE_LS_BARREL, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(LASERITEM_ATTACHMENT_RIGHTUP,   MODEL_LS_BARREL, TEXTURE_LS_BARREL, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(LASERITEM_ATTACHMENT_RIGHTDOWN, MODEL_LS_BARREL, TEXTURE_LS_BARREL, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        break;*/
		// end seo

/*
    // *********** GHOSTBUSTER ***********
      case WEAPON_GHOSTBUSTER: {
        AddWeaponAttachment(BODY_ATTACHMENT_LASER, MODEL_GHOSTBUSTER, TEXTURE_GB_BODY, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_LASER);
        AddWeaponAttachment(GHOSTBUSTERITEM_ATTACHMENT_BODY, MODEL_GB_BODY, TEXTURE_GB_BODY, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(GHOSTBUSTERITEM_ATTACHMENT_ROTATOR, MODEL_GB_ROTATOR, TEXTURE_GB_ROTATOR, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        AddWeaponAttachment(GHOSTBUSTERITEM_ATTACHMENT_EFFECT01, MODEL_GB_EFFECT1, TEXTURE_GB_LIGHTNING, 0, 0, 0);
        AddWeaponAttachment(GHOSTBUSTERITEM_ATTACHMENT_EFFECT02, MODEL_GB_EFFECT1, TEXTURE_GB_LIGHTNING, 0, 0, 0);
        AddWeaponAttachment(GHOSTBUSTERITEM_ATTACHMENT_EFFECT03, MODEL_GB_EFFECT1, TEXTURE_GB_LIGHTNING, 0, 0, 0);
        AddWeaponAttachment(GHOSTBUSTERITEM_ATTACHMENT_EFFECT04, MODEL_GB_EFFECT1, TEXTURE_GB_LIGHTNING, 0, 0, 0);
        CModelObject *pmo = pmoModel;
        SetAttachment(GHOSTBUSTERITEM_ATTACHMENT_EFFECT01);
        AddWeaponAttachment(EFFECT01_ATTACHMENT_FLARE, MODEL_GB_EFFECT1FLARE, TEXTURE_GB_FLARE, 0, 0, 0);
        pmoModel = pmo;
        SetAttachment(GHOSTBUSTERITEM_ATTACHMENT_EFFECT02);
        AddWeaponAttachment(EFFECT01_ATTACHMENT_FLARE, MODEL_GB_EFFECT1FLARE, TEXTURE_GB_FLARE, 0, 0, 0);
        pmoModel = pmo;
        SetAttachment(GHOSTBUSTERITEM_ATTACHMENT_EFFECT03);
        AddWeaponAttachment(EFFECT01_ATTACHMENT_FLARE, MODEL_GB_EFFECT1FLARE, TEXTURE_GB_FLARE, 0, 0, 0);
        pmoModel = pmo;
        SetAttachment(GHOSTBUSTERITEM_ATTACHMENT_EFFECT04);
        AddWeaponAttachment(EFFECT01_ATTACHMENT_FLARE, MODEL_GB_EFFECT1FLARE, TEXTURE_GB_FLARE, 0, 0, 0);
        break; }
*/
    // *********** CANNON ***********
      /*case WEAPON_IRONCANNON:
//      case WEAPON_NUKECANNON:
        AddWeaponAttachment(BODY_ATTACHMENT_CANNON, MODEL_CANNON, TEXTURE_CANNON, 0, 0, 0);
        SetAttachment(BODY_ATTACHMENT_CANNON);
        AddWeaponAttachment(CANNON_ATTACHMENT_BODY, MODEL_CN_BODY, TEXTURE_CANNON, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
//        AddWeaponAttachment(CANNON_ATTACHMENT_NUKEBOX, MODEL_CN_NUKEBOX, TEXTURE_CANNON, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
//        AddWeaponAttachment(CANNON_ATTACHMENT_LIGHT, MODEL_CN_LIGHT, TEXTURE_CANNON, TEX_REFL_LIGHTMETAL01, TEX_SPEC_MEDIUM, 0);
        break;
      default:
        ASSERTALWAYS("Unknown weapon.");
    }*/
    // sync apperances
    //SyncWeapon();
  //};

  // set item
  void SetItem(CModelObject *pmo) {
	// deleted by seo 40815
    /*pmoModel = &(GetPlayer()->GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject);
    AddWeaponAttachment(BODY_ATTACHMENT_ITEM, MODEL_GOLDAMON,
                        TEXTURE_GOLDAMON, TEX_REFL_GOLD01, TEX_SPEC_MEDIUM, 0);
    if (pmo!=NULL) {
      CPlayer &pl = (CPlayer&)*m_penPlayer;
      CAttachmentModelObject *pamo = pl.GetModelObject()->GetAttachmentModelList(PLAYER_ATTACHMENT_TORSO, BODY_ATTACHMENT_ITEM, -1);
      pmoModel = &(pamo->amo_moModelObject);
      pmoModel->Copy(*pmo);
      pmoModel->StretchModel(FLOAT3D(1,1,1));
      pamo->amo_plRelative = CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0));
    }
    // sync apperances
    SyncWeapon();*/
  }

  // set player body animation
  void SetBodyAnimation(INDEX iAnimation, ULONG ulFlags) {
    // on weapon change skip anim
    if (m_bChangeWeapon) { return; }
    // on firing skip anim
    if (m_bAttacking) { return; }
    // play body anim
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    CModelObject &moBody = pl.GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject;
    moBody.PlayAnim(iAnimation, ulFlags);
    m_fBodyAnimTime = moBody.GetAnimLength(iAnimation);     // anim length
  };


/************************************************************
 *                      INITIALIZE                          *
 ************************************************************/
  void InitializeProperties(void) {
    // set internal properties
    m_bReference = TRUE;
    m_bWaitJumpAnim = FALSE;
    m_bCrouch = FALSE;
    m_iCrouchDownWait = 0;
    m_iRiseUpWait = 0;
    m_bChangeWeapon = FALSE;
    m_bSwim = FALSE;
    m_bAttacking = FALSE;

    // clear eyes offsets
    m_fEyesYLastOffset = 0.0f;
    m_fEyesYOffset = 0.0f;
    m_fEyesYSpeed = 0.0f;
    m_fWeaponYLastOffset = 0.0f;
    m_fWeaponYOffset = 0.0f;
    m_fWeaponYSpeed = 0.0f;
//    m_fRecoilLastOffset = 0.0f;
//    m_fRecoilOffset = 0.0f;
//    m_fRecoilSpeed = 0.0f;
    
    // clear moving banking
    m_bMoving = FALSE;
    m_fMoveLastBanking = 0.0f;
    m_fMoveBanking = 0.0f;
    m_iMovingSide = 0;
    m_bSidestepBankingLeft = FALSE;
    m_bSidestepBankingRight = FALSE;
    m_fSidestepLastBanking = 0.0f;
    m_fSidestepBanking = 0.0f;
//0105
	if(m_bMdl)
	{
    // weapon
    //SetWeapon();
    SetBodyAnimation(BODY_ANIM_COLT_STAND, AOF_LOOPING|AOF_NORESTART);
	}
//..
  };


/************************************************************
 *                ANIMATE BANKING AND SOFT EYES             *
 ************************************************************/
  // store for lerping
  void StoreLast(void) {
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    m_vLastPlayerPosition = pl.GetPlacement().pl_PositionVector;  // store last player position
    m_fEyesYLastOffset = m_fEyesYOffset;                          // store last eyes offset
    m_fWeaponYLastOffset = m_fWeaponYOffset;
//    m_fRecoilLastOffset = m_fRecoilOffset;
    m_fMoveLastBanking = m_fMoveBanking;                          // store last banking for lerping
    m_fSidestepLastBanking = m_fSidestepBanking;
  };

  // animate banking
  void AnimateBanking(void) {
    // moving -> change banking
    if (m_bMoving) {
      // move banking left
      if (m_iMovingSide == 0) {
        m_fMoveBanking += 0.35f;
        if (m_fMoveBanking > 1.0f) { 
          m_fMoveBanking = 1.0f;
          m_iMovingSide = 1;
        }
      // move banking right
      } else {
        m_fMoveBanking -= 0.35f;
        if (m_fMoveBanking < -1.0f) {
          m_fMoveBanking = -1.0f;
          m_iMovingSide = 0;
        }
      }
      const FLOAT fBankingSpeed = 0.4f;

      // sidestep banking left
      if (m_bSidestepBankingLeft) {
        m_fSidestepBanking += fBankingSpeed;
        if (m_fSidestepBanking > 1.0f) { m_fSidestepBanking = 1.0f; }
      }
      // sidestep banking right
      if (m_bSidestepBankingRight) {
        m_fSidestepBanking -= fBankingSpeed;
        if (m_fSidestepBanking < -1.0f) { m_fSidestepBanking = -1.0f; }
      }

    // restore banking
    } else {
      // move banking
      if (m_fMoveBanking > 0.0f) {
        m_fMoveBanking -= 0.1f;
        if (m_fMoveBanking < 0.0f) { m_fMoveBanking = 0.0f; }
      } else if (m_fMoveBanking < 0.0f) {
        m_fMoveBanking += 0.1f;
        if (m_fMoveBanking > 0.0f) { m_fMoveBanking = 0.0f; }
      }
      // sidestep banking
      if (m_fSidestepBanking > 0.0f) {
        m_fSidestepBanking -= 0.4f;
        if (m_fSidestepBanking < 0.0f) { m_fSidestepBanking = 0.0f; }
      } else if (m_fSidestepBanking < 0.0f) {
        m_fSidestepBanking += 0.4f;
        if (m_fSidestepBanking > 0.0f) { m_fSidestepBanking = 0.0f; }
      }
    }

    if (GetPlayer()->GetSettings()->ps_ulFlags&PSF_NOBOBBING) {
      m_fSidestepBanking = m_fMoveBanking = 0.0f;
    }
  };

  // animate soft eyes
  void AnimateSoftEyes(void) {
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    // find eyes offset and speed (differential formula realized in numerical mathematics)
    FLOAT fRelY = (pl.GetPlacement().pl_PositionVector-m_vLastPlayerPosition) %
                  FLOAT3D(pl.en_mRotation(1, 2), pl.en_mRotation(2, 2), pl.en_mRotation(3, 2));

    // if just jumped
    if (pl.en_tmJumped>_pTimer->CurrentTick()-0.5f) {
      fRelY = ClampUp(fRelY, 0.0f);
    }

    if (!pl.m_bReferenceMovingInY) {
      m_fEyesYOffset -= fRelY;
      m_fWeaponYOffset -= ClampUp(fRelY, 0.0f);
    }

    plr_fViewDampFactor      = Clamp(plr_fViewDampFactor      ,0.0f,1.0f);
    plr_fViewDampLimitGroundUp = Clamp(plr_fViewDampLimitGroundUp ,0.0f,2.0f);
    plr_fViewDampLimitGroundDn = Clamp(plr_fViewDampLimitGroundDn ,0.0f,2.0f);
    plr_fViewDampLimitWater  = Clamp(plr_fViewDampLimitWater  ,0.0f,2.0f);

    m_fEyesYSpeed = (m_fEyesYSpeed - m_fEyesYOffset*plr_fViewDampFactor) * (1.0f-plr_fViewDampFactor);
    m_fEyesYOffset += m_fEyesYSpeed;
    
    m_fWeaponYSpeed = (m_fWeaponYSpeed - m_fWeaponYOffset*plr_fViewDampFactor) * (1.0f-plr_fViewDampFactor);
    m_fWeaponYOffset += m_fWeaponYSpeed;

    if (m_bSwim) {
      m_fEyesYOffset = Clamp(m_fEyesYOffset, -plr_fViewDampLimitWater,  +plr_fViewDampLimitWater);
      m_fWeaponYOffset = Clamp(m_fWeaponYOffset, -plr_fViewDampLimitWater,  +plr_fViewDampLimitWater);
    } else {
      m_fEyesYOffset = Clamp(m_fEyesYOffset, -plr_fViewDampLimitGroundDn,  +plr_fViewDampLimitGroundUp);
      m_fWeaponYOffset = Clamp(m_fWeaponYOffset, -plr_fViewDampLimitGroundDn,  +plr_fViewDampLimitGroundUp);
    }
  };


  // change view
  void ChangeView(CPlacement3D &pl)
  {
    TIME tmNow = ((CMovableEntity*)(CEntity*)m_penPlayer)->en_tmEntityTime;

    if (!(GetPlayer()->GetSettings()->ps_ulFlags&PSF_NOBOBBING)) {
      // banking
      FLOAT fBanking = Lerp(m_fMoveLastBanking, m_fMoveBanking, _pTimer->GetLerpFactor());
      fBanking = fBanking * fBanking * Sgn(fBanking) * 0.25f;
      fBanking += Lerp(m_fSidestepLastBanking, m_fSidestepBanking, _pTimer->GetLerpFactor());
      fBanking = Clamp(fBanking, -5.0f, 5.0f);
      pl.pl_OrientationAngle(3) += fBanking;
    }

    // swimming
    if (m_bSwim) {
      pl.pl_OrientationAngle(1) += sin(tmNow*0.9)*2.0f;
      pl.pl_OrientationAngle(2) += sin(tmNow*1.7)*2.0f;
      pl.pl_OrientationAngle(3) += sin(tmNow*2.5)*2.0f;
    }
    // eyes up/down for jumping and breathing
    FLOAT fEyesOffsetY = Lerp(m_fEyesYLastOffset, m_fEyesYOffset, _pTimer->GetLerpFactor());
    fEyesOffsetY+= sin(tmNow*1.5)*0.05f * plr_fBreathingStrength;
    fEyesOffsetY = Clamp(fEyesOffsetY, -1.0f, 1.0f);
    pl.pl_PositionVector(2) += fEyesOffsetY;
  }



/************************************************************
 *                     ANIMATE PLAYER                       *
 ************************************************************/
  // body and head animation
  void BodyAndHeadOrientation(CPlacement3D &plView) {
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    CAttachmentModelObject *pamoBody = pl.GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO);
    ANGLE3D a = plView.pl_OrientationAngle;
    if (!(pl.GetFlags()&ENF_ALIVE)) {
      a = ANGLE3D(0,0,0);
    }
    pamoBody->amo_plRelative.pl_OrientationAngle = a;
    pamoBody->amo_plRelative.pl_OrientationAngle(3) *= 4.0f;
    
    CAttachmentModelObject *pamoHead = (pamoBody->amo_moModelObject).GetAttachmentModel(BODY_ATTACHMENT_HEAD);
    pamoHead->amo_plRelative.pl_OrientationAngle = a;
    pamoHead->amo_plRelative.pl_OrientationAngle(1) = 0.0f;
    pamoHead->amo_plRelative.pl_OrientationAngle(2) = 0.0f;
    pamoHead->amo_plRelative.pl_OrientationAngle(3) *= 4.0f;

    // forbid players from cheating by kissing their @$$
    const FLOAT fMaxBanking = 5.0f;
    pamoBody->amo_plRelative.pl_OrientationAngle(3) = Clamp(pamoBody->amo_plRelative.pl_OrientationAngle(3), -fMaxBanking, fMaxBanking);
    pamoHead->amo_plRelative.pl_OrientationAngle(3) = Clamp(pamoHead->amo_plRelative.pl_OrientationAngle(3), -fMaxBanking, fMaxBanking);
  };

  void WalkAnim()
  {
	CPlayer &pl = (CPlayer&)*m_penPlayer;
	if (_pNetwork->MyWearItem[WEAR_WEAPON].IsEmptyItem() == TRUE)	
	{
		if(pl.m_bRide)
		{
			if(pl.m_bWildRide)
			{
				if( pl.m_nWildPetType == 2 )
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_WALK_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}
				else
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_WALK_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
				}
			}
			else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_WALK_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
			}
			else
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_WALK_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
			}			
		}
		else if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
		{// 날고 있는 중
			SetWholeAnimation(idPlayerWhole_Animation[ANIM_FLYING_MOVE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
		}
		else
		{
			SetWholeAnimation(idPlayerWhole_Animation[ANIM_WALK_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
		}
	}
	else
	{
		if( !_pNetwork->MyCharacterInfo.bExtension || pl.m_bMobChange )
		{
			if(pl.m_bRide)
			{
				if(pl.m_bWildRide)
				{
					if( pl.m_nWildPetType == 2 )
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_WALK_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
					}
					else
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_WALK_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
					}
				}
				else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_WALK_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}
				else
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_WALK_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}				
			}
			else if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
			{// 날고 있는 중
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_FLYING_MOVE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
			}
			else
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_WALK_2], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
			}
		}
		else
		{
			if(pl.m_bRide)
			{
				if(pl.m_bWildRide)
				{
					if( pl.m_nWildPetType == 2 )
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_WALK_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
					}
					else
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_WALK_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
					}
				}
				else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_WALK_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}
				else
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_WALK_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}				
			}
			else if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
			{// 날고 있는 중
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_FLYING_MOVE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
			}
			else
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_EXT_WALK_2], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
			}
		}
	}
  };

  void RunAnim(float fRunSpeedMul)
  {
	CPlayer &pl = (CPlayer&)*m_penPlayer;
	if (_pNetwork->MyWearItem[WEAR_WEAPON].IsEmptyItem() == TRUE)
	{
		if( pl.m_bRide )
		{
			if(pl.m_bWildRide)
			{
				if( pl.m_nWildPetType == 2 )
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}
				else
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
				}
			}
			else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, fRunSpeedMul, TRUE);
			}
			else
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, fRunSpeedMul, TRUE);
			}			
		}
		else if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
		{// 날고 있는 중
			SetWholeAnimation(idPlayerWhole_Animation[ANIM_FLYING_MOVE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, TRUE);
		}
		else
		{
			SetWholeAnimation(idPlayerWhole_Animation[ANIM_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, fRunSpeedMul, TRUE);
		}
	}
	else
	{
		if( !_pNetwork->MyCharacterInfo.bExtension || pl.m_bMobChange )
		{
			if( pl.m_bRide )
			{
				if(pl.m_bWildRide)
				{
					if( pl.m_nWildPetType == 2 )
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
					}
					else
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
					}
				}
				else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, fRunSpeedMul, TRUE);
				}
				else
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, fRunSpeedMul, TRUE);
				}
			}
			else if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
			{// 날고 있는 중
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_FLYING_MOVE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, TRUE);
			}
			else
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RUN_2], AN_LOOPING|AN_NORESTART, 0.2f, fRunSpeedMul, TRUE);
			}
		}
		else
		{
			if( pl.m_bRide )
			{
				if(pl.m_bWildRide)
				{
					if( pl.m_nWildPetType == 2 )
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
					}
					else
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
					}
				}
				else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, fRunSpeedMul, TRUE);
				}
				else
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, fRunSpeedMul, TRUE);
				}			
				
			}
			else if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
			{// 날고 있는 중
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_FLYING_MOVE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, TRUE);
			}
			else
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_EXT_RUN_2], AN_LOOPING|AN_NORESTART, 0.2f, fRunSpeedMul, TRUE);
			}
		}
	}								
  };

  void PickAnim()
  {
	CPlayer &pl = (CPlayer&)*m_penPlayer;
	if( !_pNetwork->MyCharacterInfo.bExtension || pl.m_bMobChange)
	{
		if( pl.m_bRide )
		{
			if(pl.m_bWildRide)
			{
				if( pl.m_nWildPetType == 2 )
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_PICK], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}
				else
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_PICK], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
				}
			}
			else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_PICK], AN_NORESTART, 0.2f, 1.0f, FALSE);
			}
			else
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_PICK], AN_NORESTART, 0.2f, 1.0f, FALSE);
			}		
		}
		else
		{
			SetWholeAnimation(idPlayerWhole_Animation[ANIM_PICK], AN_NORESTART, 0.2f, 1.0f, FALSE);
		}
	}
	// 로그 및 전직...
	else
	{
		if( pl.m_bRide )
		{
			if(pl.m_bWildRide)
			{
				if( pl.m_nWildPetType == 2 )
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_PICK], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}
				else
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_PICK], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
				}
			}
			else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_PICK], AN_NORESTART, 0.2f, 1.0f, FALSE);
			}
			else
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_PICK], AN_NORESTART, 0.2f, 1.0f, FALSE);
			}			
		}
		else
		{
			SetWholeAnimation(idPlayerWhole_Animation[ANIM_EXT_PICK], AN_NORESTART, 0.2f, 1.0f, FALSE);
		}
	}
  };

  void IdleAnim()
  {
	CPlayer &pl = (CPlayer&)*m_penPlayer;
	if (_pNetwork->MyWearItem[WEAR_WEAPON].IsEmptyItem() == FALSE)
	{
		if (!(_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING))
		{
			pl.DeleteWearingWeapon(TRUE, FALSE);
		}

		if( !_pNetwork->MyCharacterInfo.bExtension || pl.m_bMobChange )
		{
			if(pl.m_bRide)
			{
				if(pl.m_bWildRide)
				{
					if( pl.m_nWildPetType == 2 )
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
					}
					else
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_IDLE_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
					}
				}
				else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}
				else
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_IDLE_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
				}
			}
			else if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
			{// 날고 있는 중
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_FLYING_READY], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
			}
			else
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_ATTACK_IDLE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
			}
		}
		else
		{
			if(pl.m_bRide)
			{
				if(pl.m_bWildRide)
				{
					if( pl.m_nWildPetType == 2 )
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
					}
					else
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_IDLE_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
					}
				}
				else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}
				else
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_IDLE_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}
			}
			else if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
			{// 날고 있는 중
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_FLYING_READY], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
			}
			else
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_EXT_ATTACK_IDLE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
			}
		}
	}
	else
	{
		if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
		{
			pl.DeleteWearingWeapon(TRUE, FALSE);
		}

		if(pl.m_bRide)
		{
			if(pl.m_bWildRide)
			{
				if( pl.m_nWildPetType == 2 )
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}
				else
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_IDLE_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
				}
			}
			else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_IDLE_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
			}
			else
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_IDLE_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
			}						
		}
		else if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
		{// 날고 있는 중
			SetWholeAnimation(idPlayerWhole_Animation[ANIM_FLYING_READY], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
		}
		else
		{
			SetWholeAnimation(idPlayerWhole_Animation[ANIM_IDLE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
		}
	}
  };

  void DamageAnim()
  {
	CPlayer &pl = (CPlayer&)*m_penPlayer;
	if( !_pNetwork->MyCharacterInfo.bExtension || pl.m_bMobChange )
	{
		if(pl.m_bRide)
		{
			if(pl.m_bWildRide)
			{
				if( pl.m_nWildPetType == 2 )
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_DAMAGE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}
				else
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_DAMAGE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
				}
			}
			else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_DAMAGE], AN_NORESTART, 0.2f, 1.0f,FALSE);
			}
			else
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_DAMAGE], AN_NORESTART, 0.2f, 1.0f,FALSE);
			}						
		}
		else if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
		{// 날고 있는 중
			SetWholeAnimation(idPlayerWhole_Animation[ANIM_FLYING_READY], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
		}
		else
		{
			SetWholeAnimation(idPlayerWhole_Animation[ANIM_DAMAGE], AN_NORESTART, 0.2f, 1.0f,FALSE);
		}
	}
	else
	{
		if(pl.m_bRide)
		{
			if(pl.m_bWildRide)
			{
				if( pl.m_nWildPetType == 2 )
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_DAMAGE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
				}
				else
				{
					SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_DAMAGE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
				}
			}
			else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_DAMAGE], AN_NORESTART, 0.2f, 1.0f,FALSE);
			}
			else
			{
				SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_DAMAGE], AN_NORESTART, 0.2f, 1.0f,FALSE);
			}
		}
		else if (_pNetwork->MyCharacterInfo.ulPlayerState & PLAYER_STATE_FLYING)
		{// 날고 있는 중
			SetWholeAnimation(idPlayerWhole_Animation[ANIM_FLYING_READY], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
		}		
		else
		{
			SetWholeAnimation(idPlayerWhole_Animation[ANIM_EXT_DAMAGE], AN_NORESTART, 0.2f, 1.0f,FALSE);
		}
	}
  };

  void AnimatePlayer(void) 
  {
	if (m_bDisableAnimating) 
	{
		return;
    }
    CPlayer &pl = (CPlayer&)*m_penPlayer;
	
    FLOAT3D vDesiredTranslation = pl.en_vDesiredTranslationRelative;
    FLOAT3D vCurrentTranslation = pl.en_vCurrentTranslationAbsolute * !pl.en_mRotation;
    ANGLE3D aDesiredRotation = pl.en_aDesiredRotationRelative;
    ANGLE3D aCurrentRotation = pl.en_aCurrentRotationAbsolute;
	
    // if player is moving
    if ( vDesiredTranslation.ManhattanNorm() > 0.01f ||
		 aDesiredRotation.ManhattanNorm() > 0.01f )
	{
		// prevent idle weapon animations
		m_fLastActionTime = _pTimer->CurrentTick();
    }
	
	if (m_bReference) 
	{
		if ( !m_bWaitJumpAnim && m_iCrouchDownWait==0 && m_iRiseUpWait==0 )
		{
			// standing
			if( !m_bCrouch )
			{
				const int iJob	= pl.en_pcCharacter.pc_iPlayerType;

				static int cnt=0;
				if(pl.m_bStartAttack)
				{
/////////////////////////////////////////////////////////////
					extern FLOAT _fAttackLengthMul;
					extern FLOAT _fAttackSpeedMul;

					CPlayer &pl = (CPlayer&)*m_penPlayer;
					// [100107: selo] MyCharacterInfo.attackspeed 대치
					int Speed = pl.GetAttackspeed();

					// 해당하는 에너미가 권좌라면... 공속 1.2초
					if( pl.m_penAttackingEnemy != NULL )
					{
						if(pl.m_penAttackingEnemy->IsEnemy())
						{
							const INDEX iMobIndex = ((CEnemy*)((CEntity*)pl.m_penAttackingEnemy))->m_nMobDBIndex;
							if(iMobIndex == LORD_SYMBOL_INDEX)
							{
								// 권좌 공격속도는 1.2초.
								Speed = 12;
							}
						}
					}
					CModelInstance* pMI = GetPlayer()->GetModelInstance();

					// 공격 타입을 결정한 랜덤 변수
					// frandom : 0.33, 0.66...
					float frandom = FRnd();
					INDEX iPlayingAni_AttackType = -1; // -1은 동작중인것이 없는것
					INDEX iRandom_AttackType = 0;
					FLOAT fPlayingAniTime[4];
					INDEX AcessNum = ANIM_ATTACK_1;
					INDEX i;

					// 어쩔수 없다.
					if (_pNetwork->MyCharacterInfo.bExtension)
					{
						fPlayingAniTime[0] = m_fExtAttack1AnimTime;
						fPlayingAniTime[1] = m_fExtAttack2AnimTime;
						fPlayingAniTime[2] = m_fExtAttack3AnimTime;
						fPlayingAniTime[3] = m_fExtAttack4AnimTime;
						AcessNum = ANIM_EXT_ATTACK_1;
					}
					else
					{
						fPlayingAniTime[0] = m_fAttack1AnimTime;
						fPlayingAniTime[1] = m_fAttack2AnimTime;
						fPlayingAniTime[2] = m_fAttack3AnimTime;
						fPlayingAniTime[3] = m_fAttack4AnimTime;
					}
					
					// 현재 플레이 중인 공격애니메이션 검색
					// AttackType 과 ExtendAttackType을 구분하여 처리(전직 하셨소?)
					for (i=0; i<4; ++i)
					{
						if (pMI->IsAnimationPlaying(idPlayerWhole_Animation[AcessNum+i]))
						{ // 동작중인 애니메이션을 찾았다.
							iPlayingAni_AttackType = AcessNum+i;
							iRandom_AttackType = i;
							break;
						}
					}

					// 여기까지 iPlayingAni_AttackType에 동작중인 애니메이션이 결정되었다.

					float fLength = pMI->GetAnimLength(idPlayerWhole_Animation[ANIM_ATTACK_1]) * 10.0f;
					
					if( _pNetwork->MyWearItem[WEAR_WEAPON].IsEmptyItem() == FALSE && 
						_pNetwork->MyCharacterInfo.bExtension )
					{
						fLength = pMI->GetAnimLength(idPlayerWhole_Animation[ANIM_EXT_ATTACK_1]) * 10.0f;
					}

					_fAttackSpeedMul	= 1.0f / (fLength / Speed);
					_fAttackLengthMul	= (float)Speed / fLength;
					
///////////////////////////////////////////////////////////////
					static float startTime = 0.0f;
					if(	iJob == TITAN || 
						iJob == HEALER || 
						iJob == KNIGHT || 
						iJob == MAGE || 
						iJob == ROGUE ||
						iJob == SORCERER ||
						iJob == NIGHTSHADOW||
						IsEXRogue(iJob)|| //0722	// [2012/08/27 : Sora] EX로그 추가
						IsEXMage(iJob))	//2013/01/08 jeil EX메이지 추가 
					{
						CModelInstance* pMI = GetPlayer()->GetModelInstance();
						ASSERT( pMI != NULL && "Invalid Model Instance!" );
						
						// FIXME : 일단 나름 정리 하였다.
						if (iPlayingAni_AttackType == -1 && !GetPlayer()->m_bLockMove)
						{ // 동작중인것이 없을때
							if (_pNetwork->MyCharacterInfo.bExtension)
							{
								m_fExtAttack1AnimTime = 0.0f;
								m_fExtAttack2AnimTime = 0.0f;
								m_fExtAttack3AnimTime = 0.0f;
								m_fExtAttack4AnimTime = 0.0f;
							}
							else
							{
								m_fAttack1AnimTime = 0.0f;
								m_fAttack2AnimTime = 0.0f;
								m_fAttack3AnimTime = 0.0f;
								m_fAttack4AnimTime = 0.0f;
							}

							if (frandom < 0.25f) { iRandom_AttackType = 0; }
							else if (frandom < 0.50f) { iRandom_AttackType = 1; }
							else if (frandom < 0.75f) { iRandom_AttackType = 2; }
							else { iRandom_AttackType = 3; }

							FLOAT fLength = pMI->GetAnimLength(idPlayerWhole_Animation[AcessNum+iRandom_AttackType]) * 10.0f;
							_fAttackSpeedMul	= 1.0f / (fLength / Speed);
							_fAttackLengthMul	= Speed / fLength;
							FLOAT tmpAniTime = fLength * _fAttackLengthMul;

							switch(iRandom_AttackType)
							{
							case 0:
								{
									if (_pNetwork->MyCharacterInfo.bExtension) {
										m_fExtAttack1AnimTime = tmpAniTime;
									} else {
										m_fAttack1AnimTime = tmpAniTime;
									}
								}
								break;
							case 1:
								{
									if (_pNetwork->MyCharacterInfo.bExtension) {
										m_fExtAttack2AnimTime = tmpAniTime;
									} else {
										m_fAttack2AnimTime = tmpAniTime;
									}
								}
								break;
							case 2:
								{
									if (_pNetwork->MyCharacterInfo.bExtension) {
										m_fExtAttack3AnimTime = tmpAniTime;
									} else {
										m_fAttack3AnimTime = tmpAniTime;
									}
								}
								break;
							case 3:
								{
									if (_pNetwork->MyCharacterInfo.bExtension) {
										m_fExtAttack4AnimTime = tmpAniTime;
									} else {
										m_fAttack4AnimTime = tmpAniTime;
									}
								}
								break;
							}
						}
						else if (iPlayingAni_AttackType >= 0 && // 동작중인 애니메이션이 있고
							fPlayingAniTime[iRandom_AttackType] > 0.0f && // 시간이 남았고,
							(fPlayingAniTime[iRandom_AttackType] - 0.1f < _pTimer->CurrentTick() - startTime) &&// 끝나갈때
							!GetPlayer()->m_bLockMove)
						{
							if (_pNetwork->MyCharacterInfo.bExtension)
							{
								m_fExtAttack1AnimTime = 0.0f;
								m_fExtAttack2AnimTime = 0.0f;
								m_fExtAttack3AnimTime = 0.0f;
								m_fExtAttack4AnimTime = 0.0f;
							}
							else
							{
								m_fAttack1AnimTime = 0.0f;
								m_fAttack2AnimTime = 0.0f;
								m_fAttack3AnimTime = 0.0f;
								m_fAttack4AnimTime = 0.0f;
							}
						
							if (frandom < 0.33f) { // 0번? 
								if (iRandom_AttackType != 0) {
									iRandom_AttackType = 0;
								} else { // 이미 ANIM_ATTACK1를 하고있었다면
									iRandom_AttackType = 1;
								}
							} else if (frandom < 0.66f) {
								if (iRandom_AttackType != 1) {
									iRandom_AttackType = 1;
								} else { // 이미 ANIM_ATTACK2를 하고있었다면
									iRandom_AttackType = 2;
								}
							} else {
								if (iRandom_AttackType != 2) {
									iRandom_AttackType = 2;
								} else { // 이미 ANIM_ATTACK3를 하고있었다면
									iRandom_AttackType = 3;
								}
							}

							FLOAT fLength = pMI->GetAnimLength(idPlayerWhole_Animation[AcessNum+iRandom_AttackType]) * 10.0f;
							_fAttackSpeedMul	= 1.0f / (fLength / Speed);
							_fAttackLengthMul	= Speed / fLength;
							FLOAT tmpAniTime = fLength * _fAttackLengthMul;

							switch(iRandom_AttackType)
							{
							case 0:
								{
									if (_pNetwork->MyCharacterInfo.bExtension) {
										m_fExtAttack1AnimTime = tmpAniTime;
									} else {
										m_fAttack1AnimTime = tmpAniTime;
									}
								}
								break;
							case 1:
								{
									if (_pNetwork->MyCharacterInfo.bExtension) {
										m_fExtAttack2AnimTime = tmpAniTime;
									} else {
										m_fAttack2AnimTime = tmpAniTime;
									}
								}
								break;
							case 2:
								{
									if (_pNetwork->MyCharacterInfo.bExtension) {
										m_fExtAttack3AnimTime = tmpAniTime;
									} else {
										m_fAttack3AnimTime = tmpAniTime;
									}
								}
								break;
							case 3:
								{
									if (_pNetwork->MyCharacterInfo.bExtension) {
										m_fExtAttack4AnimTime = tmpAniTime;
									} else {
										m_fAttack4AnimTime = tmpAniTime;
									}
								}
								break;
							}
						}

						if(m_fAttack1AnimTime > 0.0f || m_fExtAttack1AnimTime > 0.0f)
						{ // ANIM_EXT_ATTACK_1, ANIM_ATTACK_1
							SetWholeAnimation(idPlayerWhole_Animation[AcessNum],  AN_LOOPING|AN_NORESTART, 0.2f, _fAttackSpeedMul,FALSE);
						}
						else if(m_fAttack2AnimTime > 0.0f || m_fExtAttack2AnimTime > 0.0f)
						{ // ANIM_EXT_ATTACK_2, ANIM_ATTACK_2
							SetWholeAnimation(idPlayerWhole_Animation[AcessNum+1],  AN_LOOPING|AN_NORESTART, 0.2f, _fAttackSpeedMul,FALSE);
						}															
						else if(m_fAttack3AnimTime > 0.0f || m_fExtAttack3AnimTime > 0.0f)
						{ // ANIM_EXT_ATTACK_3, ANIM_ATTACK_3
							SetWholeAnimation(idPlayerWhole_Animation[AcessNum+2],  AN_LOOPING|AN_NORESTART, 0.2f, _fAttackSpeedMul,FALSE);
						}		
						else if(m_fAttack4AnimTime > 0.0f || m_fExtAttack4AnimTime > 0.0f)
						{ // ANIM_EXT_ATTACK_4, ANIM_ATTACK_4
							SetWholeAnimation(idPlayerWhole_Animation[AcessNum+3],  AN_LOOPING|AN_NORESTART, 0.2f, _fAttackSpeedMul,FALSE);
						}
					}
					else
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_ATTACK_1],  AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
					}
					m_fLastActionTime = _pTimer->CurrentTick();
				}
				// running anim
				else if (vDesiredTranslation.Length() > 3.0f && vCurrentTranslation.Length() > 3.0f) 
				{
					RunAnim( _pNetwork->MyCharacterInfo.runspeed );					
					m_fLastActionTime = _pTimer->CurrentTick();
				// walking anim
				}
				else if ( vDesiredTranslation.Length() > 1.4f && vCurrentTranslation.Length() > 1.4f ) 
				{
					WalkAnim();	
					m_fLastActionTime = _pTimer->CurrentTick();
					// left rotation anim
				} 
				else if ( aDesiredRotation(1) > 0.5f ) 
				{
					WalkAnim();	
					m_fLastActionTime = _pTimer->CurrentTick();
					// right rotation anim
				} 
				else if ( aDesiredRotation(1) < -0.5f ) 
				{
					WalkAnim();	
					m_fLastActionTime = _pTimer->CurrentTick();
					// standing anim
				}
				else 
				{				
					if( pl.m_bPlayAction && pl.m_nPlayActionNum != -1)
					{
						if(	pl.m_nPlayActionNum == ACTION_NUM_SITDOWN ||	// 앉기서기.							
							pl.m_nPlayActionNum == ACTION_NUM_SELL )		// 개인상점에서 팔기~
						{
							switch(pl.m_nActionSit)
							{
								case 1:
									{
										if( pl.m_bRide )
										{
											if(pl.m_bWildRide)
											{
												if( pl.m_nWildPetType == 2 )
												{
													SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_SIT], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
												}
												else
												{
													SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_SIT], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
												}
											}											
											else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
											{
												SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_SIT], AN_NORESTART, 0.2f, 1.0f,FALSE);
											}
											else
											{
												SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SIT], AN_NORESTART, 0.2f, 1.0f,FALSE);
											}
										}
										else
										{
											SetWholeAnimation(idPlayerWhole_Animation[ANIM_SIT], AN_NORESTART, 0.2f, 1.0f,FALSE);
										}
										break;
									}
								case 3:
									{
										if( pl.m_bRide )
										{
											if(pl.m_bWildRide)
											{
												if( pl.m_nWildPetType == 2 )
												{
													SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_STANDUP], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
												}
												else
												{
													SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_STANDUP], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
												}
											}	
											else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
											{
												SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_STANDUP], AN_NORESTART, 0.2f, 1.0f,FALSE);
											}
											else
											{
												SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_STANDUP], AN_NORESTART, 0.2f, 1.0f,FALSE);
											}											
										}
										else
										{
											SetWholeAnimation(idPlayerWhole_Animation[ANIM_STANDUP], AN_NORESTART, 0.2f, 1.0f,FALSE);							
										}
										break;
									}
								case 2:
									{
										if( pl.m_bRide )
										{
											if(pl.m_bWildRide)
											{
												if( pl.m_nWildPetType == 2 )
												{
													SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_SIT_CONTINUE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
												}
												else
												{
													SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DEMONBAT_SIT_CONTINUE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);	
												}
											}
											else if( pl.m_iRideType%2 == CPetInfo::PET_HORSE )
											{
												SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_HORSE_SIT_CONTINUE], AN_NORESTART, 0.2f, 1.0f,FALSE);
											}
											else
											{
												SetWholeAnimation(idPlayerWhole_Animation[ANIM_RIDE_DRAGON_SIT_CONTINUE], AN_NORESTART, 0.2f, 1.0f,FALSE);
											}
										}
										else
										{
											SetWholeAnimation(idPlayerWhole_Animation[ANIM_SIT_CONTINUE], AN_NORESTART, 0.2f, 1.0f,FALSE);							
										}
										break;
									}
								break;
							}
						}						
						else if(pl.m_nPlayActionNum == ACTION_NUM_PICKITEM)
						{
							PickAnim();							
						}
						else if(pl.m_nPlayActionNum == ACTION_NUM_GREET)
						{
							SetWholeAnimation(idPlayerWhole_Animation[ANIM_SOCIAL_0], AN_NORESTART, 0.2f, 1.0f,FALSE);
						}
						else if(pl.m_nPlayActionNum == ACTION_NUM_SMILE)
						{
							SetWholeAnimation(idPlayerWhole_Animation[ANIM_SOCIAL_1], AN_NORESTART, 0.2f, 1.0f,FALSE);
						}
						else if(pl.m_nPlayActionNum == ACTION_NUM_CRY)
						{
							SetWholeAnimation(idPlayerWhole_Animation[ANIM_SOCIAL_2], AN_NORESTART, 0.2f, 1.0f,FALSE);
						}
						else if(pl.m_nPlayActionNum == ACTION_NUM_NUMBER_ONE)
						{
							SetWholeAnimation(idPlayerWhole_Animation[ANIM_SOCIAL_3], AN_NORESTART, 0.2f, 1.0f,FALSE);
						}
						else if(pl.m_nPlayActionNum == ACTION_NUM_HANDCLAP)
						{
							SetWholeAnimation(idPlayerWhole_Animation[ANIM_SOCIAL_4], AN_NORESTART, 0.2f, 1.0f,FALSE);
						}
						else if(pl.m_nPlayActionNum == ACTION_NUM_REFUSE)
						{
							SetWholeAnimation(idPlayerWhole_Animation[ANIM_SOCIAL_5], AN_NORESTART, 0.2f, 1.0f,FALSE);
						}
						else if( pl.m_nPlayActionNum == ACTION_NUM_GOOD_LOOKS || 
								pl.m_nPlayActionNum == ACTION_NUM_GOOD_LOOKS2 )
						{
							SetWholeAnimation(idPlayerWhole_Animation[ANIM_SOCIAL_6], AN_NORESTART, 0.2f, 1.0f,FALSE);
						}
						else if(pl.m_nPlayActionNum == ACTION_NUM_BANTER)
						{
							SetWholeAnimation(idPlayerWhole_Animation[ANIM_SOCIAL_7], AN_NORESTART, 0.2f, 1.0f,FALSE);
						}
						else if(pl.m_nPlayActionNum == ACTION_NUM_CHEER)
						{
							SetWholeAnimation(idPlayerWhole_Animation[ANIM_SOCIAL_8], AN_NORESTART, 0.2f, 1.0f,FALSE);
						}
						else if(pl.m_nPlayActionNum == ACTION_NUM_COMBAT)
						{
							SetWholeAnimation(idPlayerWhole_Animation[ANIM_SOCIAL_9], AN_NORESTART, 0.2f, 1.0f,FALSE);
						}
						else if(pl.m_nPlayActionNum == ACTION_NUM_SUBMISSION)
						{
							SetWholeAnimation(idPlayerWhole_Animation[ANIM_SOCIAL_10], AN_NORESTART, 0.2f, 1.0f,FALSE);
						}							
						else if(pl.m_nPlayActionNum == ACTION_NUM_WATER_SPREAD)
						{
							SetWholeAnimation(idPlayerWhole_Animation[ANIM_SOCIAL_11], AN_NORESTART, 0.2f, 1.0f,FALSE);
						}
					}
					else if(!pl.m_bRide && pl.m_bProduction && pl.m_nProductionNum != -1)//1128
					{
//						if( !pMI->IsAnimationPlaying(idPlayerWhole_Animation[ANIM_GATHER]) )
						{
							switch(pl.m_nProductionNum)
							{
								case 1://채굴
									SetWholeAnimation(idPlayerWhole_Animation[ANIM_MINE],  AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
									break;
								case 2://채집
									SetWholeAnimation(idPlayerWhole_Animation[ANIM_COLLECT],  AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
									break;
								case 3://차지
									SetWholeAnimation(idPlayerWhole_Animation[ANIM_GATHER],  AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
									break;
								case 4://수집
									SetWholeAnimation(idPlayerWhole_Animation[ANIM_COLLECT],  AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
									break;
								//break;
							}
						}
					}
					// WSS_DRATAN_SEIGEWARFARE 2007/08/01
					else if( _pNetwork->MyCharacterInfo.bConsensus)
					{
						SetWholeAnimation(idPlayerWhole_Animation[ANIM_GATHER],  AN_LOOPING|AN_NORESTART, 0.2f, 1.0f,FALSE);
					}
					else
					{
						IdleAnim();
					}					
				}
			}
		}
		// no reference (in air)
      }
	  else 
	  {
		  m_bReference = TRUE;
      }	  
	  
	  // boring weapon animation
	  if (_pTimer->CurrentTick()-m_fLastActionTime > 10.0f)
	  {
		  m_fLastActionTime = _pTimer->CurrentTick();
		  ((CPlayerWeapons&)*pl.m_penWeapons).SendEvent(EBoringWeapon());
	  }
	  
	  // moving view change
	  // translating -> change banking
	  if (m_bReference != NULL && vDesiredTranslation.Length()>1.0f && vCurrentTranslation.Length()>1.0f) 
	  {
		  m_bMoving = TRUE;
		  // sidestep banking
		  FLOAT vSidestepSpeedDesired = vDesiredTranslation(1);
		  FLOAT vSidestepSpeedCurrent = vCurrentTranslation(1);
		  // right
		  if (vSidestepSpeedDesired>1.0f && vSidestepSpeedCurrent>1.0f) 
		  {
			  m_bSidestepBankingRight = TRUE;
			  m_bSidestepBankingLeft = FALSE;
			  // left
		  } 
		  else if (vSidestepSpeedDesired<-1.0f && vSidestepSpeedCurrent<-1.0f) 
		  {
			  m_bSidestepBankingLeft = TRUE;
			  m_bSidestepBankingRight = FALSE;
			  // none
		  } 
		  else 
		  {
			  m_bSidestepBankingLeft = FALSE;
			  m_bSidestepBankingRight = FALSE;
		  }
		  // in air (space) or not moving
	  } 
	  else 
	  {
		  m_bMoving = FALSE;
		  m_bSidestepBankingLeft = FALSE;
		  m_bSidestepBankingRight = FALSE;
	  }
  };

  // rise
  void Rise(void) 
  {
	  if (m_bDisableAnimating) 
	  {
      return;
    }
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    pl.StartModelAnim(PLAYER_ANIM_RISE, AOF_NORESTART);
    SpawnReminder(this, pl.GetModelObject()->GetAnimLength(PLAYER_ANIM_RISE), (INDEX) AA_RISE);
    m_iRiseUpWait++;
	  
	  // 쓸모없는 부분.
	  //m_bCrouch = FALSE;
  };

  // fall
  void Fall(void) 
  {
	  if (m_bDisableAnimating) 
	  {
      return;
    }
    CPlayer &pl = (CPlayer&)*m_penPlayer;

	  //0105
	  if(m_bMdl)
	  {
		pl.StartModelAnim(PLAYER_ANIM_JUMPSTART, AOF_NORESTART);
	  }
	  else
	  {
		//SetWholeAnimation(idPlayerWhole_Animation[Attack3, AN_NORESTART, 0.2f, 1.0f);
	}
	  //..
	  
	  // 쓸모없는 부분.
	  //m_bCrouch = FALSE;
    m_bReference = FALSE;
  };

  // swim
  void Swim(void) 
  {
	  if (m_bDisableAnimating) 
	  {
      return;
    }
    CPlayer &pl = (CPlayer&)*m_penPlayer;
	  //0105
	  if(m_bMdl)
	  {
		pl.StartModelAnim(PLAYER_ANIM_SWIM, AOF_LOOPING|AOF_NORESTART);
	  }
	  else
	  {
		SetWholeAnimation(idPlayerWhole_Animation[ANIM_RUN_1], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f, FALSE);
	}
	  //..
	  // 쓸모없는 부분.
	  //m_bCrouch = FALSE;
    m_bSwim = TRUE;
  };

  // stand
  void Stand(void) 
  {
	  if (m_bDisableAnimating) 
	  {
      return;
    }
    CPlayer &pl = (CPlayer&)*m_penPlayer;
	  //0105
	  if(m_bMdl)
	  {
		pl.StartModelAnim(PLAYER_ANIM_STAND, AOF_LOOPING|AOF_NORESTART);
	  }
	  else
	  {
		IdleAnim();
		//SetWholeAnimation(idPlayerWhole_Animation[ANIM_IDLE], AN_LOOPING|AN_NORESTART, 0.2f, 1.0f);
	}
	  //..
	  // 쓸모없는 부분.
	  //m_bCrouch = FALSE;
    m_bSwim = FALSE;
  };

  // fire/attack
  void FireAnimation(INDEX iAnim, ULONG ulFlags) {
    if (m_bSwim) {
      INDEX iWeapon = ((CPlayerWeapons&)*(((CPlayer&)*m_penPlayer).m_penWeapons)).m_iCurrentWeapon;
      switch (iWeapon) {
        case WEAPON_NONE:
          break;
        case WEAPON_KNIFE: 
			/*
		case WEAPON_COLT: 
		case WEAPON_DOUBLECOLT: //case WEAPON_PIPEBOMB:
          iAnim += BODY_ANIM_COLT_SWIM_STAND-BODY_ANIM_COLT_STAND;
          break;
		  */
        //case WEAPON_SINGLESHOTGUN: case WEAPON_DOUBLESHOTGUN: 
		//case WEAPON_TOMMYGUN:
        //case WEAPON_SNIPER: case WEAPON_LASER: case WEAPON_FLAMER: //case WEAPON_GHOSTBUSTER:
		//iAnim += BODY_ANIM_SHOTGUN_SWIM_STAND-BODY_ANIM_SHOTGUN_STAND;
        //  break;
        //case WEAPON_MINIGUN: 
		//case WEAPON_ROCKETLAUNCHER: case WEAPON_GRENADELAUNCHER:
        //case WEAPON_IRONCANNON: 
		case WEAPON_CHAINSAW: // case WEAPON_NUKECANNON:
          iAnim += BODY_ANIM_MINIGUN_SWIM_STAND-BODY_ANIM_MINIGUN_STAND;
          break;
      }
    }
    m_bAttacking = FALSE;
    m_bChangeWeapon = FALSE;
//0105
	if(m_bMdl){
		SetBodyAnimation(iAnim, ulFlags);
	}
//..
    if (!(ulFlags&AOF_LOOPING)) {
      SpawnReminder(this, m_fBodyAnimTime, (INDEX) AA_ATTACK);
      m_tmAttackingDue = _pTimer->CurrentTick()+m_fBodyAnimTime;
    }
    m_bAttacking = TRUE;
//0105 잠시 어택플래그 끄기.
	FireAnimationOff();
//..
  };

  void FireAnimationOff(void) {
    m_bAttacking = FALSE;
  };

BOOL IsEXRogue( int job )	// [2012/08/27 : Sora] EX로그 추가
{
	return (charEXRogue && (job == 7));
}

BOOL IsEXMage( int job)		//2013/01/08 jeil EX메이지 추가 
{
	return (charEXMage && (job ==8));
}
  
/************************************************************
 *                  CHANGE BODY ANIMATION                   *
 ************************************************************/
  /*
  // body animation template
  void BodyAnimationTemplate(INDEX iNone, INDEX iColt, INDEX iShotgun, INDEX iMinigun, ULONG ulFlags) 
  {
    INDEX iWeapon = ((CPlayerWeapons&)*(((CPlayer&)*m_penPlayer).m_penWeapons)).m_iCurrentWeapon;
    switch (iWeapon) {
      case WEAPON_NONE:
        SetBodyAnimation(iNone, ulFlags);
        break;
      case WEAPON_KNIFE: case WEAPON_COLT: case WEAPON_DOUBLECOLT: // case WEAPON_PIPEBOMB:
        if (m_bSwim) { iColt += BODY_ANIM_COLT_SWIM_STAND-BODY_ANIM_COLT_STAND; }
//0105
        if(m_bMdl){
			SetBodyAnimation(iColt, ulFlags);
		}else{
			SetWholeAnimation(idPlayerWhole_Animation[Attack1, AN_LOOPING|AN_NORESTART, 0.2f, 1.0f);
		}
//..
        break;
      case WEAPON_SINGLESHOTGUN: case WEAPON_DOUBLESHOTGUN: 
	  case WEAPON_TOMMYGUN:
      case WEAPON_SNIPER: case WEAPON_LASER: case WEAPON_FLAMER: //case WEAPON_GHOSTBUSTER:
        if (m_bSwim) { iShotgun += BODY_ANIM_SHOTGUN_SWIM_STAND-BODY_ANIM_SHOTGUN_STAND; }
        SetBodyAnimation(iShotgun, ulFlags);
        break;
      case WEAPON_MINIGUN: case WEAPON_ROCKETLAUNCHER: case WEAPON_GRENADELAUNCHER:
      //case WEAPON_IRONCANNON: 
	  case WEAPON_CHAINSAW: // case WEAPON_NUKECANNON:
        if (m_bSwim) { iMinigun+=BODY_ANIM_MINIGUN_SWIM_STAND-BODY_ANIM_MINIGUN_STAND; }
        SetBodyAnimation(iMinigun, ulFlags);
        break;
      default: ASSERTALWAYS("Player Animator - Unknown weapon");
    }
  };
  */

  // walk
  void BodyWalkAnimation() 
  {
	  /*
    BodyAnimationTemplate(BODY_ANIM_NORMALWALK, 
      BODY_ANIM_COLT_STAND, BODY_ANIM_SHOTGUN_STAND, BODY_ANIM_MINIGUN_STAND, 
      AOF_LOOPING|AOF_NORESTART);
	  */
  };

  // stand
  void BodyStillAnimation() {
//0105
    if(m_bMdl)
	{
		/*
		  BodyAnimationTemplate(BODY_ANIM_WAIT, 
			  BODY_ANIM_COLT_STAND, BODY_ANIM_SHOTGUN_STAND, BODY_ANIM_MINIGUN_STAND, 
			  AOF_LOOPING|AOF_NORESTART);
			  */
    }
//..
  };

//0105 함수추가
  // set player body animation
  void SetWholeAnimation(INDEX iAnimation, ULONG ulFlags, FLOAT fBlendTime, FLOAT fSpeed, BOOL bRun) // Run Anim는 speed가 value가 클수록 빠르고, 그외에 스피드는 작을 수록 빠르다.
  {

    // on weapon change skip anim
    if (m_bChangeWeapon) { return; }
    // on firing skip anim
    if (m_bAttacking) { return; }
    // play body anim

	if (bRun)
	{
		float fLength = GetPlayer()->GetModelInstance()->GetAnimLength(iAnimation) * 10.0f;
		FLOAT fTmpSpeed = 1.0f;

		if (fSpeed >= (fLength * 1.3f)) // 최대 30% 속도 애니메이션 처리
		{
			fSpeed = fLength * 1.3f;
		}

		fTmpSpeed = fSpeed / fLength;
		fSpeed = 1.0f / fTmpSpeed;
	}

    GetPlayer()->ChangeWholeAnim(iAnimation, ulFlags, 1.0f, fBlendTime, fSpeed);
    m_fBodyAnimTime = GetPlayer()->GetModelInstance()->GetAnimLength(iAnimation); // anim length		
  };  
  //..

  // push weapon
  void BodyPushAnimation() 
  {
    m_bAttacking = FALSE;
    m_bChangeWeapon = FALSE;
//	  BodyAnimationTemplate(BODY_ANIM_WAIT, 
//		  BODY_ANIM_COLT_REDRAW, BODY_ANIM_SHOTGUN_REDRAW, BODY_ANIM_MINIGUN_REDRAW, 0);
    m_bChangeWeapon = TRUE;
  };

  /*
  // remove weapon attachment
  void RemoveWeapon(void) 
  {
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    pmoModel = &(pl.GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject);
	  switch (m_iWeaponLast) 
	  {
      case WEAPON_NONE:
      case WEAPON_KNIFE:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_KNIFE);
        break;
      case WEAPON_DOUBLECOLT:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_COLT_LEFT);
        // reset to player body
        pmoModel = &(pl.GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject);
      case WEAPON_COLT:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_COLT_RIGHT);
        break;
      case WEAPON_SINGLESHOTGUN:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_SINGLE_SHOTGUN);
        break;
      case WEAPON_DOUBLESHOTGUN:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_DOUBLE_SHOTGUN);
        break;
      case WEAPON_TOMMYGUN:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_TOMMYGUN);
        break;
      case WEAPON_SNIPER:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_FLAMER);
        break;
      case WEAPON_MINIGUN:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_MINIGUN);
        break;
      case WEAPON_ROCKETLAUNCHER:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_ROCKET_LAUNCHER);
        break;
      case WEAPON_GRENADELAUNCHER:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_GRENADE_LAUNCHER);
        break;
		  case WEAPON_PIPEBOMB:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_COLT_RIGHT);
        // reset to player body
        pmoModel = &(pl.GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject);
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_COLT_LEFT);
		  break;
      case WEAPON_FLAMER:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_FLAMER);
        break;
      case WEAPON_CHAINSAW:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_MINIGUN);
        break;
      case WEAPON_LASER:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_LASER);
        break;
		  case WEAPON_GHOSTBUSTER:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_LASER);
		  break;
      case WEAPON_IRONCANNON:
		  //      case WEAPON_NUKECANNON:
        pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_CANNON);
        break;
      default:
        ASSERT(FALSE);
    }
    // sync apperances
    SyncWeapon();
  }
  */

  // pull weapon
  void BodyPullAnimation() {
    // remove old weapon
//    RemoveWeapon();

    // set new weapon
    //SetWeapon();

    // pull weapon
    m_bChangeWeapon = FALSE;
//    BodyAnimationTemplate(BODY_ANIM_WAIT, 
//      BODY_ANIM_COLT_DRAW, BODY_ANIM_SHOTGUN_DRAW, BODY_ANIM_MINIGUN_DRAW, 0);
    INDEX iWeapon = ((CPlayerWeapons&)*(((CPlayer&)*m_penPlayer).m_penWeapons)).m_iCurrentWeapon;
    if (iWeapon!=WEAPON_NONE) {
      m_bChangeWeapon = TRUE;
      SpawnReminder(this, m_fBodyAnimTime, (INDEX) AA_PULLWEAPON);
    }
    // sync apperances
//    SyncWeapon();
  };

  // pull item
  void BodyPullItemAnimation() {
    // remove old weapon
//    RemoveWeapon();

    // pull item
    m_bChangeWeapon = FALSE;
    SetBodyAnimation(BODY_ANIM_STATUE_PULL, 0);
    m_bChangeWeapon = TRUE;
    SpawnReminder(this, m_fBodyAnimTime, (INDEX) AA_PULLWEAPON);
    // sync apperances
//    SyncWeapon();
  };

  // pick item
  void BodyPickItemAnimation() {
    // remove old weapon
    //RemoveWeapon();

    // pick item
    m_bChangeWeapon = FALSE;
    SetBodyAnimation(BODY_ANIM_KEYLIFT, 0);
    m_bChangeWeapon = TRUE;
    SpawnReminder(this, m_fBodyAnimTime, (INDEX) AA_PULLWEAPON);
    // sync apperances
//    SyncWeapon();
  };

  // remove item
  void BodyRemoveItem() {
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    pmoModel = &(pl.GetModelObject()->GetAttachmentModel(PLAYER_ATTACHMENT_TORSO)->amo_moModelObject);
    pmoModel->RemoveAttachmentModel(BODY_ATTACHMENT_ITEM);
    // sync apperances
//    SyncWeapon();
  };


/************************************************************
 *                      FIRE FLARE                          *
 ************************************************************/
  void OnPreRender(void) {
//    ControlFlareAttachment();

    // Minigun Specific
    CPlayerWeapons &plw = (CPlayerWeapons&)*(((CPlayer&)*m_penPlayer).m_penWeapons);
	/*
    if (plw.m_iCurrentWeapon==WEAPON_MINIGUN) 
	{
      ANGLE aAngle = Lerp(plw.m_aMiniGunLast, plw.m_aMiniGun, _pTimer->GetLerpFactor());
      // rotate minigun barrels
      CPlayer &pl = (CPlayer&)*m_penPlayer;
      CAttachmentModelObject *pamo = pl.GetModelObject()->GetAttachmentModelList(
        PLAYER_ATTACHMENT_TORSO, BODY_ATTACHMENT_MINIGUN, MINIGUNITEM_ATTACHMENT_BARRELS, -1);
      if (pamo!=NULL) {
        pamo->amo_plRelative.pl_OrientationAngle(3) = aAngle;
      }
    }
	*/
  };

  // show flare
  void ShowFlare(INDEX iAttachWeapon, INDEX iAttachObject, INDEX iAttachFlare) {
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    CAttachmentModelObject *pamo = pl.GetModelObject()->GetAttachmentModelList(
      PLAYER_ATTACHMENT_TORSO, iAttachWeapon, iAttachObject, iAttachFlare, -1);
    if (pamo!=NULL) {
      pamo->amo_plRelative.pl_OrientationAngle(3) = (rand()*360.0f)/RAND_MAX;
      CModelObject &mo = pamo->amo_moModelObject;
      mo.StretchModel(FLOAT3D(1, 1, 1));
    }
  };

  // hide flare
  void HideFlare(INDEX iAttachWeapon, INDEX iAttachObject, INDEX iAttachFlare) {
    CPlayer &pl = (CPlayer&)*m_penPlayer;
    CAttachmentModelObject *pamo = pl.GetModelObject()->GetAttachmentModelList(
      PLAYER_ATTACHMENT_TORSO, iAttachWeapon, iAttachObject, iAttachFlare, -1);
    if (pamo!=NULL) {
      CModelObject &mo = pamo->amo_moModelObject;
      mo.StretchModel(FLOAT3D(0, 0, 0));
    }
  };

  /*
  // flare attachment
  void ControlFlareAttachment(void) 
  {
    if(!IsPredictionHead()) {
      return;
    }

    CPlayerAnimator *pen = this;

    INDEX iWeapon = ((CPlayerWeapons&)*(((CPlayer&)*pen->m_penPlayer).m_penWeapons)).m_iCurrentWeapon;
    // second colt only
    if (iWeapon==WEAPON_DOUBLECOLT) {
      // add flare
      if (pen->m_iSecondFlare==FLARE_ADD) {
        pen->m_iSecondFlare = FLARE_REMOVE;
        ShowFlare(BODY_ATTACHMENT_COLT_LEFT, COLTITEM_ATTACHMENT_BODY, COLTMAIN_ATTACHMENT_FLARE);
      // remove flare
      } else if (m_iSecondFlare==FLARE_REMOVE) {
        HideFlare(BODY_ATTACHMENT_COLT_LEFT, COLTITEM_ATTACHMENT_BODY, COLTMAIN_ATTACHMENT_FLARE);
      }
    }

    // add flare
    if (pen->m_iFlare==FLARE_ADD) {
      pen->m_iFlare = FLARE_REMOVE;
      pen->m_tmFlareAdded = _pTimer->CurrentTick();
      switch(iWeapon) {
        case WEAPON_DOUBLECOLT: case WEAPON_COLT:
          ShowFlare(BODY_ATTACHMENT_COLT_RIGHT, COLTITEM_ATTACHMENT_BODY, COLTMAIN_ATTACHMENT_FLARE);
          break;
        case WEAPON_SINGLESHOTGUN:
          ShowFlare(BODY_ATTACHMENT_SINGLE_SHOTGUN, SINGLESHOTGUNITEM_ATTACHMENT_BARRELS, BARRELS_ATTACHMENT_FLARE);
          break;
        case WEAPON_DOUBLESHOTGUN:
          ShowFlare(BODY_ATTACHMENT_DOUBLE_SHOTGUN, DOUBLESHOTGUNITEM_ATTACHMENT_BARRELS, DSHOTGUNBARRELS_ATTACHMENT_FLARE);
          break;
        case WEAPON_TOMMYGUN:
          ShowFlare(BODY_ATTACHMENT_TOMMYGUN, TOMMYGUNITEM_ATTACHMENT_BODY, BODY_ATTACHMENT_FLARE);
          break;
        case WEAPON_SNIPER:
          ShowFlare(BODY_ATTACHMENT_FLAMER, SNIPERITEM_ATTACHMENT_BODY, BODY_ATTACHMENT_FLARE);
          break;
        case WEAPON_MINIGUN:
          ShowFlare(BODY_ATTACHMENT_MINIGUN, MINIGUNITEM_ATTACHMENT_BODY, BODY_ATTACHMENT_FLARE);
          break;
      }
    // remove
    } else if (m_iFlare==FLARE_REMOVE &&
      _pTimer->CurrentTick()>pen->m_tmFlareAdded+_pTimer->TickQuantum) {
      switch(iWeapon) {
        case WEAPON_DOUBLECOLT: case WEAPON_COLT:
          HideFlare(BODY_ATTACHMENT_COLT_RIGHT, COLTITEM_ATTACHMENT_BODY, COLTMAIN_ATTACHMENT_FLARE);
          break;
        case WEAPON_SINGLESHOTGUN:
          HideFlare(BODY_ATTACHMENT_SINGLE_SHOTGUN, SINGLESHOTGUNITEM_ATTACHMENT_BARRELS, BARRELS_ATTACHMENT_FLARE);
          break;
        case WEAPON_DOUBLESHOTGUN:
          HideFlare(BODY_ATTACHMENT_DOUBLE_SHOTGUN, DOUBLESHOTGUNITEM_ATTACHMENT_BARRELS, DSHOTGUNBARRELS_ATTACHMENT_FLARE);
          break;
        case WEAPON_TOMMYGUN:
          HideFlare(BODY_ATTACHMENT_TOMMYGUN, TOMMYGUNITEM_ATTACHMENT_BODY, BODY_ATTACHMENT_FLARE);
          break;
        case WEAPON_SNIPER:
          HideFlare(BODY_ATTACHMENT_FLAMER, SNIPERITEM_ATTACHMENT_BODY, BODY_ATTACHMENT_FLARE);
          break;
        case WEAPON_MINIGUN:
          HideFlare(BODY_ATTACHMENT_MINIGUN, MINIGUNITEM_ATTACHMENT_BODY, BODY_ATTACHMENT_FLARE);
          break;
      }
    }
  };
  */


/************************************************************
 *                      PROCEDURES                          *
 ************************************************************/
procedures:
  Main(EAnimatorInit eInit) {
    // remember the initial parameters
    ASSERT((CEntity*)eInit.eidPlayer!=NULL);
    m_penPlayer = eInit.eidPlayer;
    m_iPlayerID = ((CEntity*)m_penPlayer)->en_ulID;

    // declare yourself as a void
    InitAsVoid();
    SetFlags(GetFlags()|ENF_CROSSESLEVELS);
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    // last action time for boring weapon animation
    m_fLastActionTime = _pTimer->CurrentTick();

    wait() {
      on (EBegin) : { resume; }
      on (EReminder er) : { call ReminderAction(er); }
      on (EEnd) : { stop; }
    }

    // cease to exist
    Destroy(FALSE);

    return;
  };

  ReminderAction(EReminder er) 
  {
    switch (er.iValue) 
	{
      case AA_JUMPDOWN: m_bWaitJumpAnim = FALSE; break;
		  // 쓸모없는 부분.
		  /*
      case AA_CROUCH: 
		  {
          m_iCrouchDownWait--; 
          if (_pNetwork->IsServer())
		  {
            ASSERT(m_iCrouchDownWait>=0); 
          } 
		  else if (m_iCrouchDownWait<0) 
		  {
            m_iCrouchDownWait=0;
          }
          break;
        }
		*/
      case AA_RISE: {
          m_iRiseUpWait--; 
          if (_pNetwork->IsServer()) {
            ASSERT(m_iRiseUpWait>=0); 
          } else if (m_iRiseUpWait<0) {
            m_iRiseUpWait=0;
          }
          break;
        }      
      case AA_PULLWEAPON: m_bChangeWeapon = FALSE; break;
      case AA_ATTACK: if(m_tmAttackingDue<=_pTimer->CurrentTick()) { m_bAttacking = FALSE; } break;
      default: ASSERTALWAYS("Animator - unknown reminder action.");
    }
    return EBegin();
  };
};

