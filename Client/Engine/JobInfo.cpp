#include "stdh.h"
#include <Engine/JobInfo.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/ItemData.h>

#define PLAYER_TITAN	("Data\\character\\TITAN\\ti.smc")
#define PLAYER_MAGE		("Data\\character\\mage\\ma.smc")
#define PLAYER_HEALER	("Data\\character\\HEALER\\hw.smc")
#define PLAYER_KNIGHT	("Data\\character\\KNIGHT\\ni.smc")
#define PLAYER_ROGUE	("Data\\character\\ROGUE\\ro.smc")
#define PLAYER_SORCERER	("Data\\character\\Sorcerer\\so.smc")
#define PLAYER_NIGHTSHADOW ("Data\\character\\NightShadow\\ns.smc")
#ifdef CHAR_EX_ROGUE
	#define PLAYER_EX_ROGUE	("Data\\character\\ROGUE\\ro.smc")	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	#define PLAYER_EX_MAGE ("Data\\character\\mage\\ma.smc") // 2013/01/08 jeil EX메이지 추가 
#endif
// ----------------------------------------------------------------------------
// Name : CJobInfo()
// Desc :
// ----------------------------------------------------------------------------
CJobInfo::CJobInfo() : 
m_pJobInfo( NULL )
{
	m_pJobInfo					= new sJobInfo[TOTAL_JOB];
	//-------------------------------------------------------
	m_pJobInfo[TITAN].strName					= "타이탄";
	m_pJobInfo[TITAN].strFileName				= PLAYER_TITAN;

	// FIXME : 애니메이션 이름은 공식이 없나???
	// FIXME : 명명 규칙을 절대 안 지켜주기 때문에 이름이 다 엉망임~-_-;;;
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_WALK_1]		= "ti_walk_01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_WALK_2]		= "ti_walk_02";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_IDLE]			= "ti_idle_01";	
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RUN_1]			= "ti_run_01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RUN_2]			= "ti_run_02";	
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_ATTACK_IDLE]	= "ti_idle_02";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_ATTACK_1]		= "ti_att_01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_ATTACK_2]		= "ti_att_02";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_ATTACK_3]		= "ti_att_03";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_ATTACK_4]		= "ti_att_04";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SIT]			= "ti_sit_01";	// 앉기.
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SIT_CONTINUE]	= "ti_sit_02";	// 앉아 있는 상태.
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_STANDUP]		= "ti_sit_03";	// 일어서기.
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_MINE]			= "ti_mine";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_COLLECT]		= "ti_collect";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_GATHER]		= "ti_gather";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_PICK]			= "ti_pu";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_DAMAGE]		= "ti_dam";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_DIE]			= "ti_die_01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SOCIAL_0]		= "ti_so_00";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SOCIAL_1]		= "ti_so_01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SOCIAL_2]		= "ti_so_02";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SOCIAL_3]		= "ti_so_03";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SOCIAL_4]		= "ti_so_04";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SOCIAL_5]		= "ti_so_05";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SOCIAL_6]		= "ti_so_06";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SOCIAL_7]		= "ti_so_07";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SOCIAL_8]		= "ti_so_08";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SOCIAL_9]		= "ti_so_09";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SOCIAL_10]		= "ti_so_10";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_SOCIAL_11]		= "ti_water";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_EXT_IDLE_1]	= "ti_idle_01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_EXT_RUN_2]		= "ti_a_run";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_EXT_WALK_2]	= "ti_a_walk_02";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_EXT_ATTACK_IDLE]	= "ti_a_idle_02";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_EXT_ATTACK_1]	= "ti_a_att_01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_EXT_ATTACK_2]	= "ti_a_att_02";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_EXT_ATTACK_3]	= "ti_a_att_03";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_EXT_ATTACK_4]	= "ti_a_att_04";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_EXT_PICK]		= "ti_a_pu";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_EXT_DAMAGE]	= "ti_a_dam";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_EXT_DIE]		= "ti_a_die_01";

	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_WALK_1]		= "pet_horse3_walk";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_1]		= "pet_horse3_idle_01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_2]		= "pet_horse3_idle_01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_RUN_1]		= "pet_horse3_run";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_PICK]		= "pet_horse3_idle_01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_DAMAGE]		= "pet_horse3_dam";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_DIE]		= "pet_horse3_die";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_LEVELUP]	= "pet_horse3_lvup";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_SIT_CONTINUE]	= "pet_horse3_skill01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_1]	= "pet_horse3_skill02";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_2]	= "pet_horse3_skill03";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_3]	= "pet_horse3_skill03";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_4]	= "pet_horse3_skill04";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_SIT]	= "pet_horse3_skill01_1";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_HORSE_STANDUP]	= "pet_horse3_skill01_2";

	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_WALK_1]	= "pet_dragon03_walk";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_1]	= "pet_dragon03_idle01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_2]	= "pet_dragon03_idle02";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_RUN_1]		= "pet_dragon03_run";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_PICK]		= "pet_dragon03_idle01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_DAMAGE]	= "pet_dragon03_dam";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_DIE]		= "pet_dragon03_die";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_LEVELUP]	= "pet_dragon03_lvup";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_SIT_CONTINUE]	= "pet_dragon03_skill01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_1]	= "pet_dragon03_skill02";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_2]	= "pet_dragon03_skill03";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_3]	= "pet_dragon03_skill03";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_4]	= "pet_dragon03_skill04";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_SIT]	= "pet_dragon03_skill01_1";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DRAGON_STANDUP]	= "pet_dragon03_skill01_2";

	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_WALK_1]	= "pet_demonbat_walk";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_1]	= "pet_demonbat_idle01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_2]	= "pet_demonbat_idle02";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_RUN_1]		= "pet_demonbat_run";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_PICK]		= "pet_demonbat_idle01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_DAMAGE]	= "pet_demonbat_dam";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_DIE]		= "pet_demonbat_die";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_LEVELUP]	= "pet_demonbat_hungry";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT_CONTINUE]	= "pet_demonbat_idle01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_1]	= "pet_demonbat_skill01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_2]	= "pet_demonbat_skill02";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_3]	= "pet_demonbat_skill03";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_4]	= "pet_demonbat_idle01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT]	= "pet_demonbat_idle01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_RIDE_DEMONBAT_STANDUP]	= "pet_demonbat_idle01";

	m_pJobInfo[TITAN].aStrAnimationName[ANIM_FLYING_READY]	= "";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_FLYING_MOVE]	= "";

	m_pJobInfo[TITAN].aStrAnimationName[ANIM_LOGIN_GAMESTART]	= "ti_Login_01";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_LOGIN_IDLE01]		= "ti_Login_02";
	m_pJobInfo[TITAN].aStrAnimationName[ANIM_LOGIN_IDLE02]		= "ti_Login_03";

	m_pJobInfo[TITAN].afImpactTimeTable[ATTACK1] = 0.67f;
	m_pJobInfo[TITAN].afImpactTimeTable[ATTACK2] = 0.68f;
	m_pJobInfo[TITAN].afImpactTimeTable[ATTACK3] = 0.71f;
	m_pJobInfo[TITAN].afImpactTimeTable[ATTACK4] = 0.70f;
	m_pJobInfo[TITAN].afImpactTimeTable[EXT_ATTACK1] = 0.71f;
	m_pJobInfo[TITAN].afImpactTimeTable[EXT_ATTACK2] = 0.54f;
	m_pJobInfo[TITAN].afImpactTimeTable[EXT_ATTACK3] = 0.74f;
	m_pJobInfo[TITAN].afImpactTimeTable[EXT_ATTACK4] = 0.60f;

	m_pJobInfo[TITAN].iSkillWeaponType[0]			= CItemData::ITEM_WEAPON_BIGSWORD;		// 스킬 사용 가능 무기 타입
	m_pJobInfo[TITAN].iSkillWeaponType[1]			= CItemData::ITEM_WEAPON_AXE;

	// Mesh Name
	m_pJobInfo[TITAN].aStrMeshName[BODYDOWN]		= CTFileName("Data\\Character\\TITAN\\ti_bd_000.bm");			// BODYDOWN
	m_pJobInfo[TITAN].aStrMeshName[BODYUP]			= CTFileName("Data\\Character\\TITAN\\ti_bu_000.bm");			// BODYUP
	m_pJobInfo[TITAN].aStrMeshName[FOOT]			= CTFileName("Data\\Character\\TITAN\\ti_ft_000.bm");			// FOOT
	m_pJobInfo[TITAN].aStrMeshName[HAND]			= CTFileName("Data\\Character\\TITAN\\ti_hn_000.bm");			// HAND
	m_pJobInfo[TITAN].aStrMeshName[SKIRT]			= CTFileName("");												// SKIRT

	// Texture Name
	m_pJobInfo[TITAN].aStrTextureName[BODYDOWN]		= CTFileName("Data\\Character\\Titan\\ti_bd_000.tex");			// BODYDOWN
	m_pJobInfo[TITAN].aStrTextureName[BODYUP]		= CTFileName("Data\\Character\\Titan\\ti_bu_000.tex");			// BODYUP
	m_pJobInfo[TITAN].aStrTextureName[FOOT]			= CTFileName("Data\\Character\\Titan\\ti_ft_000.tex");			// FOOT
	m_pJobInfo[TITAN].aStrTextureName[HAND]			= CTFileName("Data\\Character\\Titan\\ti_hn_000.tex");			// HAND
	m_pJobInfo[TITAN].aStrTextureName[SKIRT]		= CTFileName("");												// SKIRT

	// Normal Texture Name
	m_pJobInfo[TITAN].aStrTexNormalName[BODYDOWN]	= CTFileName("Data\\Character\\Titan\\ti_bd_N_000.tex");		// BODYDOWN
	m_pJobInfo[TITAN].aStrTexNormalName[BODYUP]		= CTFileName("Data\\Character\\Titan\\ti_bu_N_000.tex");		// BODYUP
	m_pJobInfo[TITAN].aStrTexNormalName[FOOT]		= CTFileName("Data\\Character\\Titan\\ti_ft_N_000.tex");		// FOOT
	m_pJobInfo[TITAN].aStrTexNormalName[HAND]		= CTFileName("Data\\Character\\Titan\\ti_hn_N_000.tex");		// HAND
	m_pJobInfo[TITAN].aStrTexNormalName[SKIRT]		= CTFileName("");												// SKIRT
	//-------------------------------------------------------
	m_pJobInfo[KNIGHT].strName					= "나이트";
	m_pJobInfo[KNIGHT].strFileName				= PLAYER_KNIGHT;

	// FIXME : 애니메이션 이름은 공식이 없나???
	// FIXME : 명명 규칙을 절대 안 지켜주기 때문에 이름이 다 엉망임~-_-;;;
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_WALK_1]		= "ni_walk_01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_WALK_2]		= "ni_walk_02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_IDLE]			= "ni_idle_01";	
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RUN_1]		= "ni_run_01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RUN_2]		= "ni_run_02";	
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_ATTACK_IDLE]	= "ni_idle_02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_ATTACK_1]		= "ni_att_01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_ATTACK_2]		= "ni_att_02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_ATTACK_3]		= "ni_att_03";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_ATTACK_4]		= "ni_att_04";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SIT]			= "ni_sit_01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SIT_CONTINUE]	= "ni_sit_02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_STANDUP]		= "ni_sit_03";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_MINE]			= "ni_mine";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_COLLECT]		= "ni_collect";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_GATHER]		= "ni_gather";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_PICK]			= "ni_pu";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_DAMAGE]		= "ni_dam";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_DIE]			= "ni_die_01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SOCIAL_0]		= "ni_so_00";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SOCIAL_1]		= "ni_so_01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SOCIAL_2]		= "ni_so_02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SOCIAL_3]		= "ni_so_03";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SOCIAL_4]		= "ni_so_04";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SOCIAL_5]		= "ni_so_05";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SOCIAL_6]		= "ni_so_06";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SOCIAL_7]		= "ni_so_07";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SOCIAL_8]		= "ni_so_08";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SOCIAL_9]		= "ni_so_09";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SOCIAL_10]	= "ni_so_10";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_SOCIAL_11]	= "ni_water";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_EXT_IDLE_1]	= "ni_idle_01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_EXT_RUN_2]	= "ni_a_run_02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_EXT_WALK_2]	= "ni_a_walk_02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_EXT_ATTACK_IDLE]	= "ni_a_idle02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_EXT_ATTACK_1]	= "ni_a_att_01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_EXT_ATTACK_2]	= "ni_a_att_02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_EXT_ATTACK_3]	= "ni_a_att_03";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_EXT_ATTACK_4]	= "ni_a_att_04";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_EXT_PICK]		= "ni_a_pu";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_EXT_DAMAGE]	= "ni_a_dam";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_EXT_DIE]		= "ni_a_die_01";

	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_WALK_1]	= "pet_horse3_walk";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_1]	= "pet_horse3_idle_01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_2]	= "pet_horse3_idle_01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_RUN_1]	= "pet_horse3_run";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_PICK]		= "pet_horse3_idle_01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_DAMAGE]	= "pet_horse3_dam";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_DIE]		= "pet_horse3_die";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_LEVELUP]	= "pet_horse3_lvup";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_SIT_CONTINUE]	= "pet_horse3_skill01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_1]	= "pet_horse3_skill02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_2]	= "pet_horse3_skill03";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_3]	= "pet_horse3_skill03";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_4]	= "pet_horse3_skill04";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_SIT]	= "pet_horse3_skill01_1";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_HORSE_STANDUP]	= "pet_horse3_skill01_2";

	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_WALK_1]	= "pet_dragon03_walk";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_1]	= "pet_dragon03_idle01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_2]	= "pet_dragon03_idle02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_RUN_1]	= "pet_dragon03_run";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_PICK]		= "pet_dragon03_idle01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_DAMAGE]	= "pet_dragon03_dam";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_DIE]		= "pet_dragon03_die";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_LEVELUP]	= "pet_dragon03_lvup";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_SIT_CONTINUE]	= "pet_dragon03_skill01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_1]	= "pet_dragon03_skill02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_2]	= "pet_dragon03_skill03";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_3]	= "pet_dragon03_skill03";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_4]	= "pet_dragon03_skill04";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_SIT]	= "pet_dragon03_skill01_1";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DRAGON_STANDUP]	= "pet_dragon03_skill01_2";

	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_WALK_1]	= "pet_demonbat_walk";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_1]	= "pet_demonbat_idle01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_2]	= "pet_demonbat_idle02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_RUN_1]		= "pet_demonbat_run";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_PICK]		= "pet_demonbat_idle01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_DAMAGE]	= "pet_demonbat_dam";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_DIE]		= "pet_demonbat_die";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_LEVELUP]	= "pet_demonbat_hungry";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT_CONTINUE]	= "pet_demonbat_idle01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_1]	= "pet_demonbat_skill01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_2]	= "pet_demonbat_skill02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_3]	= "pet_demonbat_skill03";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_4]	= "pet_demonbat_idle01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT]	= "pet_demonbat_idle01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_RIDE_DEMONBAT_STANDUP]	= "pet_demonbat_idle01";

	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_FLYING_READY] = "";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_FLYING_MOVE] = "";

	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_LOGIN_GAMESTART]	= "ni_Login_01";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_LOGIN_IDLE01]		= "ni_Login_02";
	m_pJobInfo[KNIGHT].aStrAnimationName[ANIM_LOGIN_IDLE02]		= "ni_Login_03";

	m_pJobInfo[KNIGHT].afImpactTimeTable[ATTACK1] = 0.46f;
	m_pJobInfo[KNIGHT].afImpactTimeTable[ATTACK2] = 0.58f;
	m_pJobInfo[KNIGHT].afImpactTimeTable[ATTACK3] = 0.47f;
	m_pJobInfo[KNIGHT].afImpactTimeTable[ATTACK4] = 0.50f;
	m_pJobInfo[KNIGHT].afImpactTimeTable[EXT_ATTACK1] = 0.55f;
	m_pJobInfo[KNIGHT].afImpactTimeTable[EXT_ATTACK2] = 0.62f;
	m_pJobInfo[KNIGHT].afImpactTimeTable[EXT_ATTACK3] = 0.54f;
	m_pJobInfo[KNIGHT].afImpactTimeTable[EXT_ATTACK4] = 0.52f;

	// Mesh Name
	m_pJobInfo[KNIGHT].aStrMeshName[BODYDOWN]		= CTFileName("Data\\Character\\KNIGHT\\ni_bd_000.bm");			// BODYDOWN
	m_pJobInfo[KNIGHT].aStrMeshName[BODYUP]			= CTFileName("Data\\Character\\KNIGHT\\ni_bu_000.bm");			// BODYUP
	m_pJobInfo[KNIGHT].aStrMeshName[FOOT]			= CTFileName("Data\\Character\\KNIGHT\\ni_ft_000.bm");			// FOOT
	m_pJobInfo[KNIGHT].aStrMeshName[HAND]			= CTFileName("Data\\Character\\KNIGHT\\ni_hn_000.bm");			// HAND
	m_pJobInfo[KNIGHT].aStrMeshName[SKIRT]			= CTFileName("");												// SKIRT

	// Texture Name
	m_pJobInfo[KNIGHT].aStrTextureName[BODYDOWN]	= CTFileName("Data\\Character\\Knight\\ni_bd_000.tex");			// BODYDOWN
	m_pJobInfo[KNIGHT].aStrTextureName[BODYUP]		= CTFileName("Data\\Character\\Knight\\ni_bu_000.tex");			// BODYUP
	m_pJobInfo[KNIGHT].aStrTextureName[FOOT]		= CTFileName("Data\\Character\\Knight\\ni_ft_000.tex");			// FOOT
	m_pJobInfo[KNIGHT].aStrTextureName[HAND]		= CTFileName("Data\\Character\\Knight\\ni_hn_000.tex");			// HAND
	m_pJobInfo[KNIGHT].aStrTextureName[SKIRT]		= CTFileName("");												// SKIRT

	// Normal Texture Name
	m_pJobInfo[KNIGHT].aStrTexNormalName[BODYDOWN]	= CTFileName("Data\\Character\\Knight\\ni_bd_N_000.tex");		// BODYDOWN
	m_pJobInfo[KNIGHT].aStrTexNormalName[BODYUP]	= CTFileName("Data\\Character\\Knight\\ni_bu_N_000.tex");		// BODYUP
	m_pJobInfo[KNIGHT].aStrTexNormalName[FOOT]		= CTFileName("Data\\Character\\Knight\\ni_ft_N_000.tex");		// FOOT
	m_pJobInfo[KNIGHT].aStrTexNormalName[HAND]		= CTFileName("Data\\Character\\Knight\\ni_hn_N_000.tex");		// HAND
	m_pJobInfo[KNIGHT].aStrTexNormalName[SKIRT]		= CTFileName("");												// SKIRT

	m_pJobInfo[KNIGHT].iSkillWeaponType[0]			= CItemData::ITEM_WEAPON_KNIFE;		// 스킬 사용 가능 무기 타입
	m_pJobInfo[KNIGHT].iSkillWeaponType[1]			= CItemData::ITEM_WEAPON_TWOSWORD;
	//-------------------------------------------------------
	m_pJobInfo[HEALER].strName					= "힐러";
	m_pJobInfo[HEALER].strFileName				= PLAYER_HEALER;

	// FIXME : 애니메이션 이름은 공식이 없나???
	// FIXME : 명명 규칙을 절대 안 지켜주기 때문에 이름이 다 엉망임~-_-;;;
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_WALK_1]		= "hw_walk_01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_WALK_2]		= "hw_walk_02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_IDLE]			= "hw_idle_01";	
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RUN_1]		= "hw_run_01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RUN_2]		= "hw_run_02";	
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_ATTACK_IDLE]	= "hw_idle_02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_ATTACK_1]		= "hw_att_01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_ATTACK_2]		= "hw_att_02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_ATTACK_3]		= "hw_att_01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_ATTACK_4]		= "hw_att_02";

	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SIT]			= "hw_sit_01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SIT_CONTINUE]	= "hw_sit_02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_STANDUP]		= "hw_sit_03";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_MINE]			= "hw_mine";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_COLLECT]		= "hw_collect";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_GATHER]		= "hw_gather";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_PICK]			= "hw_pu";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_DAMAGE]		= "hw_dam";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_DIE]			= "hw_die_01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SOCIAL_0]		= "hw_so_00";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SOCIAL_1]		= "hw_so_01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SOCIAL_2]		= "hw_so_02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SOCIAL_3]		= "hw_so_03";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SOCIAL_4]		= "hw_so_04";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SOCIAL_5]		= "hw_so_05";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SOCIAL_6]		= "hw_so_06";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SOCIAL_7]		= "hw_so_07";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SOCIAL_8]		= "hw_so_08";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SOCIAL_9]		= "hw_so_09";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SOCIAL_10]	= "hw_so_10";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_SOCIAL_11]	= "hw_water";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_EXT_IDLE_1]	= "hw_a_idle_01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_EXT_RUN_2]	= "hw_a_run_02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_EXT_WALK_2]	= "hw_a_walk_02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_EXT_ATTACK_IDLE]	= "hw_a_idle02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_EXT_ATTACK_1]	= "hw_a_att01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_EXT_ATTACK_2]	= "hw_a_att02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_EXT_ATTACK_3]	= "hw_a_att01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_EXT_ATTACK_4]	= "hw_a_att02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_EXT_PICK]		= "hw_pu";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_EXT_DAMAGE]	= "hw_a_dam";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_EXT_DIE]		= "hw_a_die_01";

	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_WALK_1]	= "pet_horse3_walk";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_1]	= "pet_horse3_idle_01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_2]	= "pet_horse3_idle_01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_RUN_1]	= "pet_horse3_run";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_PICK]	= "pet_horse3_idle_01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_DAMAGE]	= "pet_horse3_dam";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_DIE]		= "pet_horse3_die";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_LEVELUP]	= "pet_horse3_lvup";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_SIT_CONTINUE]	= "pet_horse3_skill01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_1]	= "pet_horse3_skill02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_2]	= "pet_horse3_skill03";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_3]	= "pet_horse3_skill03";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_4]	= "pet_horse3_skill04";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_SIT]	= "pet_horse3_skill01_1";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_HORSE_STANDUP]	= "pet_horse3_skill01_2";

	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_WALK_1]	= "pet_dragon03_walk";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_1]	= "pet_dragon03_idle01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_2]	= "pet_dragon03_idle02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_RUN_1]	= "pet_dragon03_run";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_PICK]	= "pet_dragon03_idle01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_DAMAGE]	= "pet_dragon03_dam";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_DIE]		= "pet_dragon03_die";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_LEVELUP]	= "pet_dragon03_lvup";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_SIT_CONTINUE]	= "pet_dragon03_skill01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_1]	= "pet_dragon03_skill02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_2]	= "pet_dragon03_skill03";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_3]	= "pet_dragon03_skill03";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_4]	= "pet_dragon03_skill04";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_SIT]	= "pet_dragon03_skill01_1";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DRAGON_STANDUP]	= "pet_dragon03_skill01_2";

	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_WALK_1]	= "pet_demonbat_walk";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_1]	= "pet_demonbat_idle01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_2]	= "pet_demonbat_idle02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_RUN_1]		= "pet_demonbat_run";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_PICK]		= "pet_demonbat_idle01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_DAMAGE]	= "pet_demonbat_dam";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_DIE]		= "pet_demonbat_die";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_LEVELUP]	= "pet_demonbat_hungry";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT_CONTINUE]	= "pet_demonbat_idle01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_1]	= "pet_demonbat_skill01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_2]	= "pet_demonbat_skill02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_3]	= "pet_demonbat_skill03";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_4]	= "pet_demonbat_idle01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT]	= "pet_demonbat_idle01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_RIDE_DEMONBAT_STANDUP]	= "pet_demonbat_idle01";

	m_pJobInfo[HEALER].aStrAnimationName[ANIM_FLYING_READY] = "";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_FLYING_MOVE] = "";

	m_pJobInfo[HEALER].aStrAnimationName[ANIM_LOGIN_GAMESTART]	= "hw_Login_01";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_LOGIN_IDLE01]		= "hw_Login_02";
	m_pJobInfo[HEALER].aStrAnimationName[ANIM_LOGIN_IDLE02]		= "hw_Login_03";

	m_pJobInfo[HEALER].afImpactTimeTable[ATTACK1] = 0.44f;
	m_pJobInfo[HEALER].afImpactTimeTable[ATTACK2] = 0.44f;
	m_pJobInfo[HEALER].afImpactTimeTable[ATTACK3] = 0.44f;
	m_pJobInfo[HEALER].afImpactTimeTable[ATTACK4] = 0.44f;
	m_pJobInfo[HEALER].afImpactTimeTable[EXT_ATTACK1] = 0.28f;
	m_pJobInfo[HEALER].afImpactTimeTable[EXT_ATTACK2] = 0.30f;
	m_pJobInfo[HEALER].afImpactTimeTable[EXT_ATTACK3] = 0.28f;
	m_pJobInfo[HEALER].afImpactTimeTable[EXT_ATTACK4] = 0.30f;

	// Mesh Name
	m_pJobInfo[HEALER].aStrMeshName[BODYDOWN]		= CTFileName("Data\\Character\\HEALER\\hw_bd_000.bm");			// BODYDOWN
	m_pJobInfo[HEALER].aStrMeshName[BODYUP]			= CTFileName("Data\\Character\\HEALER\\hw_bu_000.bm");			// BODYUP
	m_pJobInfo[HEALER].aStrMeshName[FOOT]			= CTFileName("Data\\Character\\HEALER\\hw_ft_000.bm");			// FOOT
	m_pJobInfo[HEALER].aStrMeshName[HAND]			= CTFileName("Data\\Character\\HEALER\\hw_hn_000.bm");			// HAND
	m_pJobInfo[HEALER].aStrMeshName[SKIRT]			= CTFileName("Data\\Character\\HEALER\\hw_bs_000.bm");			// SKIRT

	// Texture Name
	m_pJobInfo[HEALER].aStrTextureName[BODYDOWN]	= CTFileName("Data\\Character\\Healer\\hw_bd_000.tex");			// BODYDOWN
	m_pJobInfo[HEALER].aStrTextureName[BODYUP]		= CTFileName("Data\\Character\\Healer\\hw_bu_000.tex");			// BODYUP
	m_pJobInfo[HEALER].aStrTextureName[FOOT]		= CTFileName("Data\\Character\\Healer\\hw_ft_000.tex");			// FOOT
	m_pJobInfo[HEALER].aStrTextureName[HAND]		= CTFileName("Data\\Character\\Healer\\hw_hn_000.tex");			// HAND
	m_pJobInfo[HEALER].aStrTextureName[SKIRT]		= CTFileName("Data\\Character\\Healer\\hw_bs_000.tex");			// SKIRT

	// Normal Texture Name
	m_pJobInfo[HEALER].aStrTexNormalName[BODYDOWN]	= CTFileName("");												// BODYDOWN
	m_pJobInfo[HEALER].aStrTexNormalName[BODYUP]	= CTFileName("");												// BODYUP
	m_pJobInfo[HEALER].aStrTexNormalName[FOOT]		= CTFileName("");												// FOOT
	m_pJobInfo[HEALER].aStrTexNormalName[HAND]		= CTFileName("");												// HAND
	m_pJobInfo[HEALER].aStrTexNormalName[SKIRT]		= CTFileName("");												// SKIRT

	m_pJobInfo[HEALER].iSkillWeaponType[0]			= CItemData::ITEM_WEAPON_BOW;		// 스킬 사용 가능 무기 타입
	m_pJobInfo[HEALER].iSkillWeaponType[1]			= CItemData::ITEM_WEAPON_WAND;
	//-------------------------------------------------------
	m_pJobInfo[MAGE].strName					= "메이지";
	m_pJobInfo[MAGE].strFileName				= PLAYER_MAGE;

	// FIXME : 애니메이션 이름은 공식이 없나???
	// FIXME : 명명 규칙을 절대 안 지켜주기 때문에 이름이 다 엉망임~-_-;;;
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_WALK_1]			= "ma_walk_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_WALK_2]			= "ma_walk_02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_IDLE]			= "ma_idle_01";	
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RUN_1]			= "ma_run_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RUN_2]			= "ma_run_02";	
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_ATTACK_IDLE]	= "ma_idle_02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_ATTACK_1]		= "ma_att_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_ATTACK_2]		= "ma_att_02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_ATTACK_3]		= "ma_att_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_ATTACK_4]		= "ma_att_02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SIT]			= "ma_sit_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SIT_CONTINUE]	= "ma_sit_02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_STANDUP]		= "ma_sit_03";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_MINE]			= "ma_mine";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_COLLECT]		= "ma_collect";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_GATHER]			= "ma_gather";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_PICK]			= "ma_pu";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_DAMAGE]			= "ma_dam";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_DIE]			= "ma_die_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SOCIAL_0]		= "ma_so_00";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SOCIAL_1]		= "ma_so_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SOCIAL_2]		= "ma_so_02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SOCIAL_3]		= "ma_so_03";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SOCIAL_4]		= "ma_so_04";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SOCIAL_5]		= "ma_so_05";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SOCIAL_6]		= "ma_so_06";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SOCIAL_7]		= "ma_so_07";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SOCIAL_8]		= "ma_so_08";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SOCIAL_9]		= "ma_so_09";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SOCIAL_10]		= "ma_so_10";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_SOCIAL_11]		= "ma_water";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_EXT_IDLE_1]		= "ma_idle_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_EXT_RUN_2]		= "ma_a_run_02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_EXT_WALK_2]		= "ma_a_walk_02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_EXT_ATTACK_IDLE]	= "ma_a_idle_02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_EXT_ATTACK_1]	= "ma_a_att_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_EXT_ATTACK_2]	= "ma_a_att_02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_EXT_ATTACK_3]	= "ma_a_att_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_EXT_ATTACK_4]	= "ma_a_att_02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_EXT_PICK]		= "ma_a_pu";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_EXT_DAMAGE]		= "ma_a_dam";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_EXT_DIE]		= "ma_a_die_01";

	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_WALK_1]	= "pet_horse3_walk";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_1]	= "pet_horse3_idle_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_2]	= "pet_horse3_idle_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_RUN_1]	= "pet_horse3_run";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_PICK]		= "pet_horse3_idle_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_DAMAGE]	= "pet_horse3_dam";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_DIE]		= "pet_horse3_die";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_LEVELUP]	= "pet_horse3_lvup";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_SIT_CONTINUE]	= "pet_horse3_skill01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_1]	= "pet_horse3_skill02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_2]	= "pet_horse3_skill03";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_3]	= "pet_horse3_skill03";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_4]	= "pet_horse3_skill04";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_SIT]	= "pet_horse3_skill01_1";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_HORSE_STANDUP]	= "pet_horse3_skill01_2";

	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_WALK_1]	= "pet_dragon03_walk";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_1]	= "pet_dragon03_idle01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_2]	= "pet_dragon03_idle02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_RUN_1]	= "pet_dragon03_run";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_PICK]		= "pet_dragon03_idle01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_DAMAGE]	= "pet_dragon03_dam";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_DIE]		= "pet_dragon03_die";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_LEVELUP]	= "pet_dragon03_lvup";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_SIT_CONTINUE]	= "pet_dragon03_skill01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_1]	= "pet_dragon03_skill02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_2]	= "pet_dragon03_skill03";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_3]	= "pet_dragon03_skill03";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_4]	= "pet_dragon03_skill04";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_SIT]	= "pet_dragon03_skill01_1";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_STANDUP]	= "pet_dragon03_skill01_2";

	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_WALK_1]	= "pet_demonbat_walk";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_1]	= "pet_demonbat_idle01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_2]	= "pet_demonbat_idle02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_RUN_1]		= "pet_demonbat_run";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_PICK]		= "pet_demonbat_idle01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_DAMAGE]	= "pet_demonbat_dam";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_DIE]		= "pet_demonbat_die";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_LEVELUP]	= "pet_demonbat_hungry";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT_CONTINUE]	= "pet_demonbat_idle01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_1]	= "pet_demonbat_skill01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_2]	= "pet_demonbat_skill02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_3]	= "pet_demonbat_skill03";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_4]	= "pet_demonbat_idle01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT]	= "pet_demonbat_idle01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_STANDUP]	= "pet_demonbat_idle01";

	m_pJobInfo[MAGE].aStrAnimationName[ANIM_FLYING_READY] = "";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_FLYING_MOVE] = "";

	m_pJobInfo[MAGE].aStrAnimationName[ANIM_LOGIN_GAMESTART]	= "ma_Login_01";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_LOGIN_IDLE01]		= "ma_Login_02";
	m_pJobInfo[MAGE].aStrAnimationName[ANIM_LOGIN_IDLE02]		= "ma_Login_03";

	m_pJobInfo[MAGE].afImpactTimeTable[ATTACK1] = 0.38f;
	m_pJobInfo[MAGE].afImpactTimeTable[ATTACK2] = 0.46f;
	m_pJobInfo[MAGE].afImpactTimeTable[ATTACK3] = 0.38f;
	m_pJobInfo[MAGE].afImpactTimeTable[ATTACK4] = 0.46f;
	m_pJobInfo[MAGE].afImpactTimeTable[EXT_ATTACK1] = 0.30f;
	m_pJobInfo[MAGE].afImpactTimeTable[EXT_ATTACK2] = 0.35f;
	m_pJobInfo[MAGE].afImpactTimeTable[EXT_ATTACK3] = 0.30f;
	m_pJobInfo[MAGE].afImpactTimeTable[EXT_ATTACK4] = 0.35f;

	// Mesh Name
	m_pJobInfo[MAGE].aStrMeshName[BODYDOWN]			= CTFileName("Data\\Character\\MAGE\\ma_bd_000.bm");			// BODYDOWN
	m_pJobInfo[MAGE].aStrMeshName[BODYUP]			= CTFileName("Data\\Character\\MAGE\\ma_bu_000.bm");			// BODYUP
	m_pJobInfo[MAGE].aStrMeshName[FOOT]				= CTFileName("Data\\Character\\MAGE\\ma_ft_000.bm");			// FOOT
	m_pJobInfo[MAGE].aStrMeshName[HAND]				= CTFileName("Data\\Character\\MAGE\\ma_hn_000.bm");			// HAND
	m_pJobInfo[MAGE].aStrMeshName[SKIRT]			= CTFileName("Data\\Character\\MAGE\\ma_bs_000.bm");			// SKIRT

	// Texture Name
	m_pJobInfo[MAGE].aStrTextureName[BODYDOWN]		= CTFileName("Data\\Character\\Mage\\ma_bd_000.tex");			// BODYDOWN
	m_pJobInfo[MAGE].aStrTextureName[BODYUP]		= CTFileName("Data\\Character\\Mage\\ma_bu_000.tex");			// BODYUP
	m_pJobInfo[MAGE].aStrTextureName[FOOT]			= CTFileName("Data\\Character\\Mage\\ma_ft_000.tex");			// FOOT
	m_pJobInfo[MAGE].aStrTextureName[HAND]			= CTFileName("Data\\Character\\Mage\\ma_hn_000.tex");			// HAND
	m_pJobInfo[MAGE].aStrTextureName[SKIRT]			= CTFileName("Data\\Character\\Mage\\ma_bs_000.tex");			// SKIRT

	// Normal Texture Name
	m_pJobInfo[MAGE].aStrTexNormalName[BODYDOWN]	= CTFileName("");												// BODYDOWN
	m_pJobInfo[MAGE].aStrTexNormalName[BODYUP]		= CTFileName("");												// BODYUP
	m_pJobInfo[MAGE].aStrTexNormalName[FOOT]		= CTFileName("");												// FOOT
	m_pJobInfo[MAGE].aStrTexNormalName[HAND]		= CTFileName("");												// HAND
	m_pJobInfo[MAGE].aStrTexNormalName[SKIRT]		= CTFileName("");												// SKIRT

	m_pJobInfo[MAGE].iSkillWeaponType[0]			= CItemData::ITEM_WEAPON_SSTAFF;		// 스킬 사용 가능 무기 타입
	m_pJobInfo[MAGE].iSkillWeaponType[1]			= CItemData::ITEM_WEAPON_STAFF;
	//-------------------------------------------------------
	m_pJobInfo[ROGUE].strName					= "로그";
	m_pJobInfo[ROGUE].strFileName				= PLAYER_ROGUE;

	// FIXME : 애니메이션 이름은 공식이 없나???
	// FIXME : 명명 규칙을 절대 안 지켜주기 때문에 이름이 다 엉망임~-_-;;;
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_WALK_1]		= "ro_walk_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_WALK_2]		= "ro_walk_02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_IDLE]			= "ro_idle_01";	
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RUN_1]			= "ro_run_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RUN_2]			= "ro_run_02";	
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_ATTACK_IDLE]	= "ro_idle_02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_ATTACK_1]		= "ro_att_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_ATTACK_2]		= "ro_att_02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_ATTACK_3]		= "ro_att_03";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_ATTACK_4]		= "ro_att_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SIT]			= "ro_sit_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SIT_CONTINUE]	= "ro_sit_02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_STANDUP]		= "ro_sit_03";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_MINE]			= "ro_mine";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_COLLECT]		= "ro_collect";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_GATHER]		= "ro_gather";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_PICK]			= "ro_pu";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_DAMAGE]		= "ro_dam";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_DIE]			= "ro_die_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SOCIAL_0]		= "ro_so_00";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SOCIAL_1]		= "ro_so_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SOCIAL_2]		= "ro_so_02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SOCIAL_3]		= "ro_so_03";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SOCIAL_4]		= "ro_so_04";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SOCIAL_5]		= "ro_so_05";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SOCIAL_6]		= "ro_so_06";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SOCIAL_7]		= "ro_so_07";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SOCIAL_8]		= "ro_so_08";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SOCIAL_9]		= "ro_so_09";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SOCIAL_10]		= "ro_so_10";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_SOCIAL_11]		= "ro_water";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_EXT_IDLE_1]	= "ro_idle_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_EXT_RUN_2]		= "ro_a_run_02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_EXT_WALK_2]	= "ro_a_walk_02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_EXT_ATTACK_IDLE]	= "ro_a_idle_02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_EXT_ATTACK_1]	= "ro_a_att_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_EXT_ATTACK_2]	= "ro_a_att_02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_EXT_ATTACK_3]	= "ro_a_att_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_EXT_ATTACK_4]	= "ro_a_att_02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_EXT_PICK]		= "ro_a_pu";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_EXT_DAMAGE]	= "ro_a_dam";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_EXT_DIE]		= "ro_a_die_01";

	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_WALK_1]	= "pet_horse3_walk";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_1]	= "pet_horse3_idle_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_2]	= "pet_horse3_idle_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_RUN_1]	= "pet_horse3_run";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_PICK]		= "pet_horse3_idle_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_DAMAGE]	= "pet_horse3_dam";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_DIE]		= "pet_horse3_die";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_LEVELUP]	= "pet_horse3_lvup";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_SIT_CONTINUE]	= "pet_horse3_skill01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_1]	= "pet_horse3_skill02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_2]	= "pet_horse3_skill03";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_3]	= "pet_horse3_skill03";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_4]	= "pet_horse3_skill04";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_SIT]	= "pet_horse3_skill01_1";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_STANDUP]	= "pet_horse3_skill01_2";

	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_WALK_1]	= "pet_dragon03_walk";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_1]	= "pet_dragon03_idle01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_2]	= "pet_dragon03_idle02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_RUN_1]		= "pet_dragon03_run";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_PICK]		= "pet_dragon03_idle01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_DAMAGE]	= "pet_dragon03_dam";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_DIE]		= "pet_dragon03_die";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_LEVELUP]	= "pet_dragon03_lvup";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_SIT_CONTINUE]	= "pet_dragon03_skill01";			// 계속 앉아 있기.
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_1]	= "pet_dragon03_skill02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_2]	= "pet_dragon03_skill03";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_3]	= "pet_dragon03_skill03";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_4]	= "pet_dragon03_skill04";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_SIT]	= "pet_dragon03_skill01_1";		// 앉으려고 하기
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_STANDUP]	= "pet_dragon03_skill01_2";		// 서있으려고 하기

	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_WALK_1]	= "pet_demonbat_walk";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_1]	= "pet_demonbat_idle01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_2]	= "pet_demonbat_idle02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_RUN_1]		= "pet_demonbat_run";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_PICK]		= "pet_demonbat_idle01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_DAMAGE]	= "pet_demonbat_dam";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_DIE]		= "pet_demonbat_die";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_LEVELUP]	= "pet_demonbat_hungry";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT_CONTINUE]	= "pet_demonbat_idle01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_1]	= "pet_demonbat_skill01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_2]	= "pet_demonbat_skill02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_3]	= "pet_demonbat_skill03";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_4]	= "pet_demonbat_idle01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT]	= "pet_demonbat_idle01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_STANDUP]	= "pet_demonbat_idle01";

	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_FLYING_READY] = "";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_FLYING_MOVE] = "";

	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_LOGIN_GAMESTART]	= "ro_Login_01";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_LOGIN_IDLE01]		= "ro_Login_02";
	m_pJobInfo[ROGUE].aStrAnimationName[ANIM_LOGIN_IDLE02]		= "ro_Login_03";

	m_pJobInfo[ROGUE].afImpactTimeTable[ATTACK1] = 0.59f;
	m_pJobInfo[ROGUE].afImpactTimeTable[ATTACK2] = 0.58f;
	m_pJobInfo[ROGUE].afImpactTimeTable[ATTACK3] = 0.65f;
	m_pJobInfo[ROGUE].afImpactTimeTable[ATTACK4] = 0.59f;
	m_pJobInfo[ROGUE].afImpactTimeTable[EXT_ATTACK1] = 0.28f;
	m_pJobInfo[ROGUE].afImpactTimeTable[EXT_ATTACK2] = 0.29f;
	m_pJobInfo[ROGUE].afImpactTimeTable[EXT_ATTACK3] = 0.28f;
	m_pJobInfo[ROGUE].afImpactTimeTable[EXT_ATTACK4] = 0.29f;

	// Mesh Name
	m_pJobInfo[ROGUE].aStrMeshName[BODYDOWN]		= CTFileName("Data\\Character\\Rogue\\ro_bd_000.bm");			// BODYDOWN
	m_pJobInfo[ROGUE].aStrMeshName[BODYUP]			= CTFileName("Data\\Character\\Rogue\\ro_bu_000.bm");			// BODYUP
	m_pJobInfo[ROGUE].aStrMeshName[FOOT]			= CTFileName("Data\\Character\\Rogue\\ro_ft_000.bm");			// FOOT
	m_pJobInfo[ROGUE].aStrMeshName[HAND]			= CTFileName("Data\\Character\\Rogue\\ro_hn_000.bm");			// HAND
	m_pJobInfo[ROGUE].aStrMeshName[SKIRT]			= CTFileName("");			// SKIRT

	// Texture Name
	m_pJobInfo[ROGUE].aStrTextureName[BODYDOWN]		= CTFileName("Data\\Character\\Rogue\\ro_bd_000.tex");			// BODYDOWN
	m_pJobInfo[ROGUE].aStrTextureName[BODYUP]		= CTFileName("Data\\Character\\Rogue\\ro_bu_000.tex");			// BODYUP
	m_pJobInfo[ROGUE].aStrTextureName[FOOT]			= CTFileName("Data\\Character\\Rogue\\ro_ft_000.tex");			// FOOT
	m_pJobInfo[ROGUE].aStrTextureName[HAND]			= CTFileName("Data\\Character\\Rogue\\ro_hn_000.tex");			// HAND
	m_pJobInfo[ROGUE].aStrTextureName[SKIRT]		= CTFileName("");			// SKIRT

	// Normal Texture Name
	m_pJobInfo[ROGUE].aStrTexNormalName[BODYDOWN]	= CTFileName("");												// BODYDOWN
	m_pJobInfo[ROGUE].aStrTexNormalName[BODYUP]		= CTFileName("");												// BODYUP
	m_pJobInfo[ROGUE].aStrTexNormalName[FOOT]		= CTFileName("");												// FOOT
	m_pJobInfo[ROGUE].aStrTexNormalName[HAND]		= CTFileName("");												// HAND
	m_pJobInfo[ROGUE].aStrTexNormalName[SKIRT]		= CTFileName("");												// SKIRT

	m_pJobInfo[ROGUE].iSkillWeaponType[0]			= CItemData::ITEM_WEAPON_DAGGER;		// 스킬 사용 가능 무기 타입
	m_pJobInfo[ROGUE].iSkillWeaponType[1]			= CItemData::ITEM_WEAPON_CROSSBOW;
	//-------------------------------------------------------		
	m_pJobInfo[SORCERER].strName					= "소서러";
	m_pJobInfo[SORCERER].strFileName				= PLAYER_SORCERER;

	// FIXME : 애니메이션 이름은 공식이 없나???
	// FIXME : 명명 규칙을 절대 안 지켜주기 때문에 이름이 다 엉망임~-_-;;;
	// FIXME : 소환수가 제일 엉망임!!! 반드시 SkaStudio에서 확인하고, 게임에서 한번 더 확인할것-_-;;;
	//m_pJobInfo[SORCERER].aStrAnimationName[ANIM_WALK_1]			= "so_walk_01";
	//m_pJobInfo[SORCERER].aStrAnimationName[ANIM_WALK_2]			= "so_walk_02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_WALK_1]			= "so_a_walk_002";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_WALK_2]			= "so_a_walk_002";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_IDLE]			= "so_idle_01";	
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RUN_1]			= "so_run_01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RUN_2]			= "so_run_02";	
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_ATTACK_IDLE]	= "so_idle_02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_ATTACK_1]		= "so_att_01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_ATTACK_2]		= "so_att_02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_ATTACK_3]		= "so_att_03";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_ATTACK_4]		= "so_att_01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SIT]			= "so_sit_01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SIT_CONTINUE]	= "so_sit_02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_STANDUP]		= "so_sit_03";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_MINE]			= "so_mine";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_COLLECT]		= "so_collect";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_GATHER]			= "so_gather";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_PICK]			= "so_pu";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_DAMAGE]			= "so_dam";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_DIE]			= "so_die";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SOCIAL_0]		= "so_so_00";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SOCIAL_1]		= "so_so_01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SOCIAL_2]		= "so_so_02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SOCIAL_3]		= "so_so_03";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SOCIAL_4]		= "so_so_04";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SOCIAL_5]		= "so_so_05";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SOCIAL_6]		= "so_so_06";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SOCIAL_7]		= "so_so_07";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SOCIAL_8]		= "so_so_08";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SOCIAL_9]		= "so_so_09";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SOCIAL_10]		= "so_so_10";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_SOCIAL_11]		= "so_water";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_EXT_IDLE_1]		= "so_a_idle_02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_EXT_RUN_2]		= "so_a_run_002";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_EXT_WALK_2]		= "so_a_walk_002";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_EXT_ATTACK_IDLE]= "so_a_idle_02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_EXT_ATTACK_1]	= "so_a_att_01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_EXT_ATTACK_2]	= "so_a_att_02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_EXT_ATTACK_3]	= "so_a_att_01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_EXT_ATTACK_4]	= "so_a_att_02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_EXT_PICK]		= "so_a_pu";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_EXT_DAMAGE]		= "so_a_dam";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_EXT_DIE]		= "so_a_die";

	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_WALK_1]	= "pet_horse3_walk";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_1]	= "pet_horse3_idle_01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_2]	= "pet_horse3_idle_01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_RUN_1]	= "pet_horse3_run";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_PICK]	= "pet_horse3_idle_01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_DAMAGE]	= "pet_horse3_dam";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_DIE]		= "pet_horse3_die";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_LEVELUP]	= "pet_horse3_lvup";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_SIT_CONTINUE]	= "pet_horse3_skill01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_1]	= "pet_horse3_skill02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_2]	= "pet_horse3_skill03";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_3]	= "pet_horse3_skill03";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_4]	= "pet_horse3_skill04";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_SIT]	= "pet_horse3_skill01_1";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_HORSE_STANDUP]	= "pet_horse3_skill01_2";

	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_WALK_1]	= "pet_dragon03_walk";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_1]	= "pet_dragon03_idle01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_2]	= "pet_dragon03_idle02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_RUN_1]	= "pet_dragon03_run";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_PICK]	= "pet_dragon03_idle01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_DAMAGE]	= "pet_dragon03_dam";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_DIE]	= "pet_dragon03_die";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_LEVELUP]= "pet_dragon03_lvup";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_SIT_CONTINUE]= "pet_dragon03_skill01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_1]= "pet_dragon03_skill02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_2]= "pet_dragon03_skill03";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_3]= "pet_dragon03_skill03";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_4]= "pet_dragon03_skill04";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_SIT]	= "pet_dragon03_skill01_1";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DRAGON_STANDUP]= "pet_dragon03_skill01_2";

	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_WALK_1]	= "pet_demonbat_walk";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_1]	= "pet_demonbat_idle01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_2]	= "pet_demonbat_idle02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_RUN_1]		= "pet_demonbat_run";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_PICK]		= "pet_demonbat_idle01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_DAMAGE]	= "pet_demonbat_dam";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_DIE]		= "pet_demonbat_die";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_LEVELUP]	= "pet_demonbat_hungry";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT_CONTINUE]	= "pet_demonbat_idle01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_1]	= "pet_demonbat_skill01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_2]	= "pet_demonbat_skill02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_3]	= "pet_demonbat_skill03";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_4]	= "pet_demonbat_idle01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT]	= "pet_demonbat_idle01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_RIDE_DEMONBAT_STANDUP]	= "pet_demonbat_idle01";

	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_FLYING_READY] = "";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_FLYING_MOVE] = "";

	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_LOGIN_GAMESTART]	= "so_Login_01";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_LOGIN_IDLE01]		= "so_Login_02";
	m_pJobInfo[SORCERER].aStrAnimationName[ANIM_LOGIN_IDLE02]		= "so_Login_03";

	m_pJobInfo[SORCERER].afImpactTimeTable[ATTACK1] = 0.57f;
	m_pJobInfo[SORCERER].afImpactTimeTable[ATTACK2] = 0.63f;
	m_pJobInfo[SORCERER].afImpactTimeTable[ATTACK3] = 0.52f;
	m_pJobInfo[SORCERER].afImpactTimeTable[ATTACK4] = 0.57f;
	m_pJobInfo[SORCERER].afImpactTimeTable[EXT_ATTACK1] = 0.55f;
	m_pJobInfo[SORCERER].afImpactTimeTable[EXT_ATTACK2] = 0.44f;
	m_pJobInfo[SORCERER].afImpactTimeTable[EXT_ATTACK3] = 0.55f;
	m_pJobInfo[SORCERER].afImpactTimeTable[EXT_ATTACK4] = 0.44f;

	// Mesh Name
	m_pJobInfo[SORCERER].aStrMeshName[BODYDOWN]			= CTFileName("Data\\Character\\Sorcerer\\so_bd_000.bm");			// BODYDOWN
	m_pJobInfo[SORCERER].aStrMeshName[BODYUP]			= CTFileName("Data\\Character\\Sorcerer\\so_bu_000.bm");			// BODYUP
	m_pJobInfo[SORCERER].aStrMeshName[FOOT]				= CTFileName("Data\\Character\\Sorcerer\\so_ft_000.bm");			// FOOT
	m_pJobInfo[SORCERER].aStrMeshName[HAND]				= CTFileName("Data\\Character\\Sorcerer\\so_hn_000.bm");			// HAND
	m_pJobInfo[SORCERER].aStrMeshName[SKIRT]			= CTFileName("");			// SKIRT

	// Texture Name
	m_pJobInfo[SORCERER].aStrTextureName[BODYDOWN]		= CTFileName("Data\\Character\\Sorcerer\\so_bd_000.tex");			// BODYDOWN
	m_pJobInfo[SORCERER].aStrTextureName[BODYUP]		= CTFileName("Data\\Character\\Sorcerer\\so_bu_000.tex");			// BODYUP
	m_pJobInfo[SORCERER].aStrTextureName[FOOT]			= CTFileName("Data\\Character\\Sorcerer\\so_ft_000.tex");			// FOOT
	m_pJobInfo[SORCERER].aStrTextureName[HAND]			= CTFileName("Data\\Character\\Sorcerer\\so_hn_000.tex");			// HAND
	m_pJobInfo[SORCERER].aStrTextureName[SKIRT]			= CTFileName("");			// SKIRT

	// Normal Texture Name
	m_pJobInfo[SORCERER].aStrTexNormalName[BODYDOWN]	= CTFileName("");												// BODYDOWN
	m_pJobInfo[SORCERER].aStrTexNormalName[BODYUP]		= CTFileName("");												// BODYUP
	m_pJobInfo[SORCERER].aStrTexNormalName[FOOT]		= CTFileName("");												// FOOT
	m_pJobInfo[SORCERER].aStrTexNormalName[HAND]		= CTFileName("");												// HAND
	m_pJobInfo[SORCERER].aStrTexNormalName[SKIRT]		= CTFileName("");												// SKIRT
	
	m_pJobInfo[SORCERER].iSkillWeaponType[0]			= CItemData::ITEM_WEAPON_POLEARM;		// 스킬 사용 가능 무기 타입
	m_pJobInfo[SORCERER].iSkillWeaponType[1]			= CItemData::ITEM_WEAPON_SCYTHE;

	m_pJobInfo[NIGHTSHADOW].strName					= "나이트 쉐도우";
	m_pJobInfo[NIGHTSHADOW].strFileName				= PLAYER_NIGHTSHADOW;

	// FIXME : 애니메이션 이름은 공식이 없나???
	// FIXME : 명명 규칙을 절대 안 지켜주기 때문에 이름이 다 엉망임~-_-;;;
	// FIXME : 소환수가 제일 엉망임!!! 반드시 SkaStudio에서 확인하고, 게임에서 한번 더 확인할것-_-;;;
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_WALK_1]			= "ns_walk_00";	// 무기가 없을때
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_WALK_2]			= "ns_walk_01";	// 무기가 있을때
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_IDLE]			= "ns_ready_00";	
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RUN_1]			= "ns_run_00";	// 무기가 없을때
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RUN_2]			= "ns_run_01";		// 무기가 있을때
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_ATTACK_IDLE]		= "ns_ready_01";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_ATTACK_1]		= "ns_att_01";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_ATTACK_2]		= "ns_att_03";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_ATTACK_3]		= "ns_att_02";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_ATTACK_4]		= "ns_att_04";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SIT]				= "ns_sit_00";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SIT_CONTINUE]	= "ns_sit_01";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_STANDUP]			= "ns_stand_00";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_MINE]			= "ns_mine_00";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_COLLECT]			= "ns_collect_00";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_GATHER]			= "ns_gather_00";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_PICK]			= "ns_pickup_00";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_DAMAGE]			= "ns_dam_00";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_DIE]				= "ns_die_00";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SOCIAL_0]		= "ns_so01_00";	// 인사
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SOCIAL_1]		= "ns_so02_00";	// 웃음
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SOCIAL_2]		= "ns_so03_00";	// 슬픔
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SOCIAL_3]		= "ns_so04_00";	// 최고
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SOCIAL_4]		= "ns_so05_00";	// 박수
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SOCIAL_5]		= "ns_so06_00";	// 거절
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SOCIAL_6]		= "ns_so07_00";	// 힘자랑
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SOCIAL_7]		= "ns_so08_00";	// 야유
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SOCIAL_8]		= "ns_so09_00";	// 환호
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SOCIAL_9]		= "ns_so10_00";	// 도전
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SOCIAL_10]		= "ns_so11_00";	// 복종
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_SOCIAL_11]		= "ns_water"; // 뭐니 이건?
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_EXT_IDLE_1]		= "ns_ready_00";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_EXT_RUN_2]		= "ns_run_01";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_EXT_WALK_2]		= "ns_walk_01";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_EXT_ATTACK_IDLE]	= "ns_ready_01";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_EXT_ATTACK_1]	= "ns_att_01";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_EXT_ATTACK_2]	= "ns_att_03";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_EXT_ATTACK_3]	= "ns_att_02";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_EXT_ATTACK_4]	= "ns_att_04";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_EXT_PICK]		= "ns_pickup_00";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_EXT_DAMAGE]		= "ns_dam_00";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_EXT_DIE]			= "ns_die_00";

	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_WALK_1]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_1]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_2]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_RUN_1]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_PICK]		= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_DAMAGE]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_DIE]		= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_LEVELUP]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_SIT_CONTINUE]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_1]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_2]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_3]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_4]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_SIT]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_HORSE_STANDUP]	= "";

	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_WALK_1]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_1]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_2]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_RUN_1]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_PICK]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_DAMAGE]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_DIE]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_LEVELUP]= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_SIT_CONTINUE]= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_1]= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_2]= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_3]= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_4]= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_SIT]	= "";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_RIDE_DRAGON_STANDUP]= "";

	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_FLYING_READY] = "ns_fly_stay_00";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_FLYING_MOVE] = "ns_fly_move_00";

	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_LOGIN_GAMESTART]	= "ns_Login_01";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_LOGIN_IDLE01]	= "ns_Login_02";
	m_pJobInfo[NIGHTSHADOW].aStrAnimationName[ANIM_LOGIN_IDLE02]		= "ns_Login_03";
 
	m_pJobInfo[NIGHTSHADOW].afImpactTimeTable[ATTACK1] = 0.25f;	// 0.6f
	m_pJobInfo[NIGHTSHADOW].afImpactTimeTable[ATTACK2] = 0.5f;	// 0.2f
	m_pJobInfo[NIGHTSHADOW].afImpactTimeTable[ATTACK3] = 0.35f;
	m_pJobInfo[NIGHTSHADOW].afImpactTimeTable[ATTACK4] = 0.35f;
	m_pJobInfo[NIGHTSHADOW].afImpactTimeTable[EXT_ATTACK1] = 0.25f;
	m_pJobInfo[NIGHTSHADOW].afImpactTimeTable[EXT_ATTACK2] = 0.5f;
	m_pJobInfo[NIGHTSHADOW].afImpactTimeTable[EXT_ATTACK3] = 0.35f;
	m_pJobInfo[NIGHTSHADOW].afImpactTimeTable[EXT_ATTACK4] = 0.35f;

	// Mesh Name
	m_pJobInfo[NIGHTSHADOW].aStrMeshName[BODYDOWN]			= CTFileName("Data\\Character\\NightShadow\\ns_bd_000.bm");			// BODYDOWN
	m_pJobInfo[NIGHTSHADOW].aStrMeshName[BODYUP]			= CTFileName("Data\\Character\\NightShadow\\ns_bu_000.bm");			// BODYUP
	m_pJobInfo[NIGHTSHADOW].aStrMeshName[FOOT]				= CTFileName("Data\\Character\\NightShadow\\ns_ft_000.bm");			// FOOT
	m_pJobInfo[NIGHTSHADOW].aStrMeshName[HAND]				= CTFileName("Data\\Character\\NightShadow\\ns_hn_000.bm");			// HAND
	m_pJobInfo[NIGHTSHADOW].aStrMeshName[SKIRT]			= CTFileName("");			// SKIRT

	// Texture Name
	m_pJobInfo[NIGHTSHADOW].aStrTextureName[BODYDOWN]		= CTFileName("Data\\Character\\NightShadow\\Texture\\ns_bd_000.tex");			// BODYDOWN
	m_pJobInfo[NIGHTSHADOW].aStrTextureName[BODYUP]		= CTFileName("Data\\Character\\NightShadow\\Texture\\ns_bu_000.tex");			// BODYUP
	m_pJobInfo[NIGHTSHADOW].aStrTextureName[FOOT]			= CTFileName("Data\\Character\\NightShadow\\Texture\\ns_ft_000.tex");			// FOOT
	m_pJobInfo[NIGHTSHADOW].aStrTextureName[HAND]			= CTFileName("Data\\Character\\NightShadow\\Texture\\ns_hn_000.tex");			// HAND
	m_pJobInfo[NIGHTSHADOW].aStrTextureName[SKIRT]			= CTFileName("");			// SKIRT

	// Normal Texture Name
	m_pJobInfo[NIGHTSHADOW].aStrTexNormalName[BODYDOWN]	= CTFileName("Data\\Character\\NightShadow\\Texture\\ns_ft_000.tex");												// BODYDOWN
	m_pJobInfo[NIGHTSHADOW].aStrTexNormalName[BODYUP]		= CTFileName("");												// BODYUP
	m_pJobInfo[NIGHTSHADOW].aStrTexNormalName[FOOT]		= CTFileName("");												// FOOT
	m_pJobInfo[NIGHTSHADOW].aStrTexNormalName[HAND]		= CTFileName("");												// HAND
	m_pJobInfo[NIGHTSHADOW].aStrTexNormalName[SKIRT]		= CTFileName("");												// SKIRT
	
	m_pJobInfo[NIGHTSHADOW].iSkillWeaponType[0]			= CItemData::ITEM_WEAPON_SOUL;		// 스킬 사용 가능 무기 타입
	m_pJobInfo[NIGHTSHADOW].iSkillWeaponType[1]			= CItemData::ITEM_WEAPON_SOUL;

	// [2012/08/27 : Sora] EX로그 추가
#ifdef CHAR_EX_ROGUE
	//-------------------------------------------------------
	m_pJobInfo[EX_ROGUE].strName				= "";
	m_pJobInfo[EX_ROGUE].strFileName				= PLAYER_EX_ROGUE;

	// FIXME : 애니메이션 이름은 공식이 없나???
	// FIXME : 명명 규칙을 절대 안 지켜주기 때문에 이름이 다 엉망임~-_-;;;
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_WALK_1]		= "ro_walk_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_WALK_2]		= "ro_walk_02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_IDLE]			= "ro_idle_01";	
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RUN_1]			= "ro_run_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RUN_2]			= "ro_run_02";	
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_ATTACK_IDLE]	= "ro_idle_02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_ATTACK_1]		= "exro_att_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_ATTACK_2]		= "exro_att_02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_ATTACK_3]		= "exro_att_03";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_ATTACK_4]		= "exro_att_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SIT]			= "ro_sit_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SIT_CONTINUE]	= "ro_sit_02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_STANDUP]		= "ro_sit_03";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_MINE]			= "ro_mine";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_COLLECT]		= "ro_collect";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_GATHER]		= "ro_gather";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_PICK]			= "ro_pu";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_DAMAGE]		= "ro_dam";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_DIE]			= "ro_die_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SOCIAL_0]		= "ro_so_00";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SOCIAL_1]		= "ro_so_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SOCIAL_2]		= "ro_so_02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SOCIAL_3]		= "ro_so_03";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SOCIAL_4]		= "ro_so_04";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SOCIAL_5]		= "ro_so_05";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SOCIAL_6]		= "ro_so_06";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SOCIAL_7]		= "ro_so_07";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SOCIAL_8]		= "ro_so_08";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SOCIAL_9]		= "ro_so_09";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SOCIAL_10]		= "ro_so_10";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_SOCIAL_11]		= "ro_water";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_EXT_IDLE_1]	= "ro_idle_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_EXT_RUN_2]		= "ro_a_run_02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_EXT_WALK_2]	= "ro_a_walk_02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_EXT_ATTACK_IDLE]	= "ro_a_idle_02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_EXT_ATTACK_1]	= "exro_a_att_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_EXT_ATTACK_2]	= "exro_a_att_02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_EXT_ATTACK_3]	= "exro_a_att_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_EXT_ATTACK_4]	= "exro_a_att_02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_EXT_PICK]		= "ro_a_pu";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_EXT_DAMAGE]	= "ro_a_dam";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_EXT_DIE]		= "ro_a_die_01";

	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_WALK_1]	= "pet_horse3_walk";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_1]	= "pet_horse3_idle_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_2]	= "pet_horse3_idle_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_RUN_1]	= "pet_horse3_run";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_PICK]		= "pet_horse3_idle_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_DAMAGE]	= "pet_horse3_dam";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_DIE]		= "pet_horse3_die";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_LEVELUP]	= "pet_horse3_lvup";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_SIT_CONTINUE]	= "pet_horse3_skill01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_1]	= "pet_horse3_skill02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_2]	= "pet_horse3_skill03";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_3]	= "pet_horse3_skill03";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_4]	= "pet_horse3_skill04";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_SIT]	= "pet_horse3_skill01_1";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_HORSE_STANDUP]	= "pet_horse3_skill01_2";

	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_WALK_1]	= "pet_dragon03_walk";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_1]	= "pet_dragon03_idle01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_2]	= "pet_dragon03_idle02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_RUN_1]		= "pet_dragon03_run";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_PICK]		= "pet_dragon03_idle01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_DAMAGE]	= "pet_dragon03_dam";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_DIE]		= "pet_dragon03_die";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_LEVELUP]	= "pet_dragon03_lvup";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_SIT_CONTINUE]	= "pet_dragon03_skill01";			// 계속 앉아 있기.
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_1]	= "pet_dragon03_skill02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_2]	= "pet_dragon03_skill03";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_3]	= "pet_dragon03_skill03";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_4]	= "pet_dragon03_skill04";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_SIT]	= "pet_dragon03_skill01_1";		// 앉으려고 하기
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DRAGON_STANDUP]	= "pet_dragon03_skill01_2";		// 서있으려고 하기

	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_WALK_1]	= "pet_demonbat_walk";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_1]	= "pet_demonbat_idle01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_2]	= "pet_demonbat_idle02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_RUN_1]		= "pet_demonbat_run";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_PICK]		= "pet_demonbat_idle01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_DAMAGE]	= "pet_demonbat_dam";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_DIE]		= "pet_demonbat_die";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_LEVELUP]	= "pet_demonbat_hungry";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT_CONTINUE]	= "pet_demonbat_idle01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_1]	= "pet_demonbat_skill01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_2]	= "pet_demonbat_skill02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_3]	= "pet_demonbat_skill03";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_4]	= "pet_demonbat_idle01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT]	= "pet_demonbat_idle01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_RIDE_DEMONBAT_STANDUP]	= "pet_demonbat_idle01";

	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_FLYING_READY] = "";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_FLYING_MOVE] = "";

	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_LOGIN_GAMESTART]	= "ro_ex_Login_01";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_LOGIN_IDLE01]		= "ro_ex_Login_02";
	m_pJobInfo[EX_ROGUE].aStrAnimationName[ANIM_LOGIN_IDLE02]		= "ro_ex_Login_03";	

	m_pJobInfo[EX_ROGUE].afImpactTimeTable[ATTACK1] = 0.59f;
	m_pJobInfo[EX_ROGUE].afImpactTimeTable[ATTACK2] = 0.58f;
	m_pJobInfo[EX_ROGUE].afImpactTimeTable[ATTACK3] = 0.65f;
	m_pJobInfo[EX_ROGUE].afImpactTimeTable[ATTACK4] = 0.59f;
	m_pJobInfo[EX_ROGUE].afImpactTimeTable[EXT_ATTACK1] = 0.28f;
	m_pJobInfo[EX_ROGUE].afImpactTimeTable[EXT_ATTACK2] = 0.29f;
	m_pJobInfo[EX_ROGUE].afImpactTimeTable[EXT_ATTACK3] = 0.28f;
	m_pJobInfo[EX_ROGUE].afImpactTimeTable[EXT_ATTACK4] = 0.29f;

	// Mesh Name
	m_pJobInfo[EX_ROGUE].aStrMeshName[BODYDOWN]		= CTFileName("Data\\Character\\Rogue\\ro_bd_000.bm");			// BODYDOWN
	m_pJobInfo[EX_ROGUE].aStrMeshName[BODYUP]			= CTFileName("Data\\Character\\Rogue\\ro_bu_000.bm");			// BODYUP
	m_pJobInfo[EX_ROGUE].aStrMeshName[FOOT]			= CTFileName("Data\\Character\\Rogue\\ro_ft_000.bm");			// FOOT
	m_pJobInfo[EX_ROGUE].aStrMeshName[HAND]			= CTFileName("Data\\Character\\Rogue\\ro_hn_000.bm");			// HAND
	m_pJobInfo[EX_ROGUE].aStrMeshName[SKIRT]			= CTFileName("");			// SKIRT

	// Texture Name
	m_pJobInfo[EX_ROGUE].aStrTextureName[BODYDOWN]		= CTFileName("Data\\Character\\Rogue\\ro_bd_000.tex");			// BODYDOWN
	m_pJobInfo[EX_ROGUE].aStrTextureName[BODYUP]		= CTFileName("Data\\Character\\Rogue\\ro_bu_000.tex");			// BODYUP
	m_pJobInfo[EX_ROGUE].aStrTextureName[FOOT]			= CTFileName("Data\\Character\\Rogue\\ro_ft_000.tex");			// FOOT
	m_pJobInfo[EX_ROGUE].aStrTextureName[HAND]			= CTFileName("Data\\Character\\Rogue\\ro_hn_000.tex");			// HAND
	m_pJobInfo[EX_ROGUE].aStrTextureName[SKIRT]		= CTFileName("");			// SKIRT

	// Normal Texture Name
	m_pJobInfo[EX_ROGUE].aStrTexNormalName[BODYDOWN]	= CTFileName("");												// BODYDOWN
	m_pJobInfo[EX_ROGUE].aStrTexNormalName[BODYUP]		= CTFileName("");												// BODYUP
	m_pJobInfo[EX_ROGUE].aStrTexNormalName[FOOT]		= CTFileName("");												// FOOT
	m_pJobInfo[EX_ROGUE].aStrTexNormalName[HAND]		= CTFileName("");												// HAND
	m_pJobInfo[EX_ROGUE].aStrTexNormalName[SKIRT]		= CTFileName("");												// SKIRT

	m_pJobInfo[EX_ROGUE].iSkillWeaponType[0]			= CItemData::ITEM_WEAPON_DAGGER;		// 스킬 사용 가능 무기 타입
	m_pJobInfo[EX_ROGUE].iSkillWeaponType[1]			= CItemData::ITEM_WEAPON_CROSSBOW;
	//-------------------------------------------------------		
#endif
#ifdef CHAR_EX_MAGE	//2013/01/08 jeil 임시로 일단 메이지것 복사해서 추가 나중에 리소스 나오면 수정 필요
					//2013/01/10 jeil EX메이지 추가 추가된 애니메이션 명으로 변경 나머진 기존 메이지와 동일 
	m_pJobInfo[EX_MAGE].strName					= "";
	m_pJobInfo[EX_MAGE].strFileName				= PLAYER_EX_MAGE;

	// FIXME : 애니메이션 이름은 공식이 없나???
	// FIXME : 명명 규칙을 절대 안 지켜주기 때문에 이름이 다 엉망임~-_-;;;
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_WALK_1]			= "ma_walk_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_WALK_2]			= "ma_walk_02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_IDLE]			= "ma_idle_01";	
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RUN_1]			= "ma_run_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RUN_2]			= "ma_run_02";	
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_ATTACK_IDLE]	= "ma_idle_02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_ATTACK_1]		= "exma_att_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_ATTACK_2]		= "exma_att_02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_ATTACK_3]		= "exma_att_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_ATTACK_4]		= "exma_att_02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SIT]			= "ma_sit_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SIT_CONTINUE]	= "ma_sit_02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_STANDUP]		= "ma_sit_03";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_MINE]			= "ma_mine";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_COLLECT]		= "ma_collect";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_GATHER]			= "ma_gather";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_PICK]			= "ma_pu";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_DAMAGE]			= "ma_dam";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_DIE]			= "ma_die_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SOCIAL_0]		= "ma_so_00";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SOCIAL_1]		= "ma_so_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SOCIAL_2]		= "ma_so_02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SOCIAL_3]		= "ma_so_03";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SOCIAL_4]		= "ma_so_04";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SOCIAL_5]		= "ma_so_05";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SOCIAL_6]		= "ma_so_06";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SOCIAL_7]		= "ma_so_07";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SOCIAL_8]		= "ma_so_08";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SOCIAL_9]		= "ma_so_09";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SOCIAL_10]		= "ma_so_10";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_SOCIAL_11]		= "ma_water";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_EXT_IDLE_1]		= "ma_idle_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_EXT_RUN_2]		= "ma_a_run_02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_EXT_WALK_2]		= "ma_a_walk_02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_EXT_ATTACK_IDLE]	= "ma_a_idle_02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_EXT_ATTACK_1]	= "exma_a_att_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_EXT_ATTACK_2]	= "exma_a_att_02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_EXT_ATTACK_3]	= "exma_a_att_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_EXT_ATTACK_4]	= "exma_a_att_02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_EXT_PICK]		= "ma_a_pu";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_EXT_DAMAGE]		= "ma_a_dam";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_EXT_DIE]		= "ma_a_die_01";

	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_WALK_1]	= "pet_horse3_walk";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_1]	= "pet_horse3_idle_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_IDLE_2]	= "pet_horse3_idle_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_RUN_1]	= "pet_horse3_run";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_PICK]		= "pet_horse3_idle_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_DAMAGE]	= "pet_horse3_dam";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_DIE]		= "pet_horse3_die";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_LEVELUP]	= "pet_horse3_lvup";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_SIT_CONTINUE]	= "pet_horse3_skill01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_1]	= "pet_horse3_skill02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_2]	= "pet_horse3_skill03";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_3]	= "pet_horse3_skill03";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_SKILL_4]	= "pet_horse3_skill04";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_SIT]	= "pet_horse3_skill01_1";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_HORSE_STANDUP]	= "pet_horse3_skill01_2";

	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_WALK_1]	= "pet_dragon03_walk";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_1]	= "pet_dragon03_idle01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_IDLE_2]	= "pet_dragon03_idle02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_RUN_1]	= "pet_dragon03_run";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_PICK]		= "pet_dragon03_idle01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_DAMAGE]	= "pet_dragon03_dam";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_DIE]		= "pet_dragon03_die";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_LEVELUP]	= "pet_dragon03_lvup";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_SIT_CONTINUE]	= "pet_dragon03_skill01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_1]	= "pet_dragon03_skill02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_2]	= "pet_dragon03_skill03";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_3]	= "pet_dragon03_skill03";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_SKILL_4]	= "pet_dragon03_skill04";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_SIT]	= "pet_dragon03_skill01_1";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DRAGON_STANDUP]	= "pet_dragon03_skill01_2";

	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_WALK_1]	= "pet_demonbat_walk";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_1]	= "pet_demonbat_idle01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_IDLE_2]	= "pet_demonbat_idle02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_RUN_1]		= "pet_demonbat_run";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_PICK]		= "pet_demonbat_idle01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_DAMAGE]	= "pet_demonbat_dam";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_DIE]		= "pet_demonbat_die";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_LEVELUP]	= "pet_demonbat_hungry";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT_CONTINUE]	= "pet_demonbat_idle01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_1]	= "pet_demonbat_skill01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_2]	= "pet_demonbat_skill02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_3]	= "pet_demonbat_skill03";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SKILL_4]	= "pet_demonbat_idle01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_SIT]	= "pet_demonbat_idle01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_RIDE_DEMONBAT_STANDUP]	= "pet_demonbat_idle01";

	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_FLYING_READY] = "";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_FLYING_MOVE] = "";

	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_LOGIN_GAMESTART]		= "ma_arc_login_01";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_LOGIN_IDLE01]		= "ma_arc_login_02";
	m_pJobInfo[EX_MAGE].aStrAnimationName[ANIM_LOGIN_IDLE02]			= "ma_arc_login_03";	

	m_pJobInfo[EX_MAGE].afImpactTimeTable[ATTACK1] = 0.38f;
	m_pJobInfo[EX_MAGE].afImpactTimeTable[ATTACK2] = 0.46f;
	m_pJobInfo[EX_MAGE].afImpactTimeTable[ATTACK3] = 0.38f;
	m_pJobInfo[EX_MAGE].afImpactTimeTable[ATTACK4] = 0.46f;
	m_pJobInfo[EX_MAGE].afImpactTimeTable[EXT_ATTACK1] = 0.30f;
	m_pJobInfo[EX_MAGE].afImpactTimeTable[EXT_ATTACK2] = 0.35f;
	m_pJobInfo[EX_MAGE].afImpactTimeTable[EXT_ATTACK3] = 0.30f;
	m_pJobInfo[EX_MAGE].afImpactTimeTable[EXT_ATTACK4] = 0.35f;

	// Mesh Name
	m_pJobInfo[EX_MAGE].aStrMeshName[BODYDOWN]			= CTFileName("Data\\Character\\MAGE\\ma_bd_000.bm");			// BODYDOWN
	m_pJobInfo[EX_MAGE].aStrMeshName[BODYUP]			= CTFileName("Data\\Character\\MAGE\\ma_bu_000.bm");			// BODYUP
	m_pJobInfo[EX_MAGE].aStrMeshName[FOOT]				= CTFileName("Data\\Character\\MAGE\\ma_ft_000.bm");			// FOOT
	m_pJobInfo[EX_MAGE].aStrMeshName[HAND]				= CTFileName("Data\\Character\\MAGE\\ma_hn_000.bm");			// HAND
	m_pJobInfo[EX_MAGE].aStrMeshName[SKIRT]			= CTFileName("Data\\Character\\MAGE\\ma_bs_000.bm");			// SKIRT

	// Texture Name
	m_pJobInfo[EX_MAGE].aStrTextureName[BODYDOWN]		= CTFileName("Data\\Character\\Mage\\ma_bd_000.tex");			// BODYDOWN
	m_pJobInfo[EX_MAGE].aStrTextureName[BODYUP]		= CTFileName("Data\\Character\\Mage\\ma_bu_000.tex");			// BODYUP
	m_pJobInfo[EX_MAGE].aStrTextureName[FOOT]			= CTFileName("Data\\Character\\Mage\\ma_ft_000.tex");			// FOOT
	m_pJobInfo[EX_MAGE].aStrTextureName[HAND]			= CTFileName("Data\\Character\\Mage\\ma_hn_000.tex");			// HAND
	m_pJobInfo[EX_MAGE].aStrTextureName[SKIRT]			= CTFileName("Data\\Character\\Mage\\ma_bs_000.tex");			// SKIRT

	// Normal Texture Name
	m_pJobInfo[EX_MAGE].aStrTexNormalName[BODYDOWN]	= CTFileName("");												// BODYDOWN
	m_pJobInfo[EX_MAGE].aStrTexNormalName[BODYUP]		= CTFileName("");												// BODYUP
	m_pJobInfo[EX_MAGE].aStrTexNormalName[FOOT]		= CTFileName("");												// FOOT
	m_pJobInfo[EX_MAGE].aStrTexNormalName[HAND]		= CTFileName("");												// HAND
	m_pJobInfo[EX_MAGE].aStrTexNormalName[SKIRT]		= CTFileName("");												// SKIRT

	m_pJobInfo[EX_MAGE].iSkillWeaponType[0]			= CItemData::ITEM_WEAPON_SSTAFF;		// 스킬 사용 가능 무기 타입
	m_pJobInfo[EX_MAGE].iSkillWeaponType[1]			= CItemData::ITEM_WEAPON_STAFF;
#endif
}

// ----------------------------------------------------------------------------
// Name : ~CJobInfo()
// Desc :
// ----------------------------------------------------------------------------
CJobInfo::~CJobInfo()
{
	if( m_pJobInfo )
		delete [] m_pJobInfo;
}

// ----------------------------------------------------------------------------
// Name : SetName()
// Desc :
// ----------------------------------------------------------------------------
void CJobInfo::SetName( int iJob, const CTString& strName )
{
	m_pJobInfo[iJob].strName = strName;
}

// ----------------------------------------------------------------------------
// Name : SetExtensionName()
// Desc :
// ----------------------------------------------------------------------------
void CJobInfo::SetExtensionName( int iJob, int iType, const CTString& strName )
{
	m_pJobInfo[iJob].strExtensionName[iType] = strName;
}

// ----------------------------------------------------------------------------
// Name : GetName()
// Desc :
// ----------------------------------------------------------------------------
CTString CJobInfo::GetName( int iJob, int iJob2 )
{
	if (iJob < 0 || iJob >= TOTAL_JOB)
		return CTString("");

	if( iJob2 > 0 )
	{
		return GetExtensionName( iJob, iJob2 - 1 );
	}
	return m_pJobInfo[iJob].strName;
}

// ----------------------------------------------------------------------------
// Name : GetExtensionName()
// Desc :
// ----------------------------------------------------------------------------
CTString CJobInfo::GetExtensionName( int iJob, int iType )
{
	if (iType < 0 || iType >= 2)
		return CTString("");

	return m_pJobInfo[iJob].strExtensionName[iType];
}

// ----------------------------------------------------------------------------
// Name : GetFileName()
// Desc :
// ----------------------------------------------------------------------------
CTString CJobInfo::GetFileName( int iJob )
{
	return m_pJobInfo[iJob].strFileName;
}

// ----------------------------------------------------------------------------
// Name : GetSkillWeponType()
// Desc :
// ----------------------------------------------------------------------------
int CJobInfo::GetSkillWeponType( int iJob, int iType )		const
{
	return m_pJobInfo[iJob].iSkillWeaponType[iType];
}

// ----------------------------------------------------------------------------
// Name : GetImpactTime()
// Desc :
// ----------------------------------------------------------------------------
float CJobInfo::GetImpactTime( int iJob, int iAttackType )	const
{
	return m_pJobInfo[iJob].afImpactTimeTable[iAttackType];
}

// ----------------------------------------------------------------------------
// Name : GetAnimationName()
// Desc :
// ----------------------------------------------------------------------------
CTString CJobInfo::GetAnimationName( int iJob, int iAnimType )
{
	return m_pJobInfo[iJob].aStrAnimationName[iAnimType];
}

// ----------------------------------------------------------------------------
// Name : GetMeshName()
// Desc :
// ----------------------------------------------------------------------------
CTFileName CJobInfo::GetMeshName( int iJob, int iWearPos )
{
	return m_pJobInfo[iJob].aStrMeshName[iWearPos];
}

// ----------------------------------------------------------------------------
// Name : GetTextureName()
// Desc :
// ----------------------------------------------------------------------------
CTFileName CJobInfo::GetTextureName( int iJob, int iWearPos )
{
	return m_pJobInfo[iJob].aStrTextureName[iWearPos];
}

// ----------------------------------------------------------------------------
// Name : GetTexNormalName()
// Desc :
// ----------------------------------------------------------------------------
CTFileName CJobInfo::GetTexNormalName( int iJob, int iWearPos )
{
	return m_pJobInfo[iJob].aStrTexNormalName[iWearPos];
}