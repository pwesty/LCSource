#ifndef SE_INCL_PLAYERSETTINGS_H
#define SE_INCL_PLAYERSETTINGS_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif
//강동민 수정 시작 로그인 처리 작업	07.07
#include <Engine/Base/Stream.h>
//강동민 수정 끝 로그인 처리 작업		07.07
/*
 * Class responsible for describing player appearance/settings
 */
class CPlayerSettings {
public:
  SBYTE ps_iModel; // multiplayer character
  char ps_achModelFile[16];     // filename of the player model (zero padded, not zero terminated)

#define PS_WAS_ONLYNEW  0 // select weapon if you didn't have it before
#define PS_WAS_NONE     1 // never autoselect
#define PS_WAS_ALL      2 // always autoselect
#define PS_WAS_BETTER   3 // only if the new weapon is 'better' than what you hold now
	SBYTE ps_iWeaponAutoSelect;   // type of weapon autoselection(PlayerWeapon에서 쓰임)

	SBYTE ps_iCrossHairType;      // type of crosshair used(타겟 마크를 그려줌)

#define PSF_HIDEWEAPON      (1L<<0)   // don't render weapon in 1st person
#define PSF_PREFER3RDPERSON (1L<<1)   // auto switch to 3rd person 
#define PSF_NOQUOTES        (1L<<2)   // don't tell quotes
#define PSF_AUTOSAVE        (1L<<3)   // auto save at specific locations
#define PSF_COMPSINGLECLICK (1L<<4)   // invoke computer with single click, not double clicks
#define PSF_SHARPTURNING    (1L<<5)   // use prescanning to eliminate mouse lag
#define PSF_NOBOBBING       (1L<<6)   // view bobbing on/off (for people with motion sickness problems)
#define PSF_INVERTLOOK      (1L<<7)   // invert look
  ULONG ps_ulFlags;   // various flags

  SBYTE ps_iSensitivity;

	// get filename for model(SetAppear~ 쪽에서 쓰이고 있음)
  CTFileName GetModelFilename(void) const
  {
    char achModelFile[MAX_PATH+1];
    memset(achModelFile, 0, sizeof(achModelFile));
    memcpy(achModelFile, ps_achModelFile, sizeof(ps_achModelFile));
    CTString strModelFile = "Data\\ModelsMP\\Player\\"+CTString(achModelFile)+".amc";
    if (!FileExists(strModelFile)) {
      strModelFile = "Data\\Models\\Player\\"+CTString(achModelFile)+".amc";
    }
    return strModelFile;
  }
};

//강동민 수정 시작 로그인 처리 작업	07.07
/*
// NOTE: never instantiate CPlayerSettings, as its size is not fixed to the size defined in engine
// use CUniversalPlayerSettings for instantiating an object
class CUniversalPlayerSettings {
public:
  union {
    CPlayerSettings ups_ps;
    UBYTE ups_aubDummy[NET_MAXSESSIONPROPERTIES];
  };

  // must have exact the size as allocated block in engine
  CUniversalPlayerSettings() { 
    ASSERT(sizeof(CPlayerSettings)<=MAX_PLAYERAPPEARANCE); 
    ASSERT(sizeof(CUniversalPlayerSettings)==MAX_PLAYERAPPEARANCE); 
  }
  operator CPlayerSettings&(void) { return ups_ps; }
};
*/
//강동민 수정 끝 로그인 처리 작업		07.07


#endif  /* include-once check. */

  
