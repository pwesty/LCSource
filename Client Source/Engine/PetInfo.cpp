#include "stdh.h"
#include <Engine/PetInfo.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>

CPetInfo CPetInfo::m_instance;

#define PET_HORSE_CHILD		("Data\\NPC\\Pet\\Pet_Horse01.smc")
#define PET_HORSE_BOY		("Data\\NPC\\Pet\\Pet_Horse02.smc")
#define PET_HORSE_ADULT		("Data\\NPC\\Pet\\Horse\\Pet_Horse03.smc")

#define PET_DRAGON_CHILD	("Data\\NPC\\Pet\\Pet_Dragon01.smc")
#define PET_DRAGON_BOY		("Data\\NPC\\Pet\\Pet_Dragon02.smc")
#define PET_DRAGON_ADULT	("Data\\NPC\\Pet\\Pet_Dragon03.smc")

// Add rare Horse & Dragon 060822
#define PET_DRAGON_CHILD_PINK	("Data\\NPC\\Pet\\Pet_Dragon_Pink01.smc")
#define PET_DRAGON_BOY_PINK		("Data\\NPC\\Pet\\Pet_Dragon_Pink02.smc")
#define PET_DRAGON_ADULT_PINK	("Data\\NPC\\Pet\\Pet_Dragon_Pink03.smc")

#define PET_HORSE_CHILD_BLUE	("Data\\NPC\\Pet\\Pet_Horse_Blue01.smc")
#define PET_HORSE_BOY_BLUE		("Data\\NPC\\Pet\\Pet_Horse_Blue02.smc")
#define PET_HORSE_ADULT_BLUE	("Data\\NPC\\Pet\\Pet_Horse_Blue03.smc")

// [070824: Su-won] PET_COLOR_CHANGE
CTString strColoredTexFile[6][62] =
{
	//작은 말
	{	"Data\\NPC\\Pet\\pet_horse01.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse01_1.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse01_2.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse01_3.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse01_4.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse01_5.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse01_6.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse01_7.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse01_8.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse01_9.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse01_10.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse01_8_heart.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse01_9_heart.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse01_3_heart.tex",
		"",	
		"",
		"",
		"",
		"",		
		"",
		"",
		"",
		"",
		"",		 
		"",
		"",
		"",
		"",
		"", 
		"",
		"",
		"",
		"",
		"", 
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		""
	},

	//보통 말
	{	"Data\\NPC\\Pet\\pet_horse02.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse02_1.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse02_2.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse02_3.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse02_4.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse02_5.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse02_6.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse02_7.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse02_8.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse02_9.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse02_10.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse02_8_heart.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse02_9_heart.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse02_3_heart.tex",
		"",	
		"",
		"",
		"",
		"",		
		"",
		"",
		"",
		"",
		"",		 
		"",
		"",
		"",
		"",
		"", 
		"",
		"",
		"",
		"",
		"", 
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		""
	},

	//타는 말
	{	"Data\\NPC\\Pet\\pet_horse03.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_1.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_2.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_3.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_4.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_5.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_6.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_7.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_8.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_9.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_10.tex",
		
		"Data\\NPC\\Pet\\textures\\pet_horse03_8_heart.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_9_heart.tex",		
		"Data\\NPC\\Pet\\textures\\pet_horse03_3_heart.tex",

		"Data\\NPC\\Pet\\textures\\pet_horse03_8_c.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_8_d.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_8_s.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_8_sk.tex",

		"Data\\NPC\\Pet\\textures\\pet_horse03_9_c.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_9_d.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_9_s.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_9_sk.tex",

		"Data\\NPC\\Pet\\textures\\pet_horse03_3_c.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_3_d.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_3_s.tex",		
		"Data\\NPC\\Pet\\textures\\pet_horse03_3_sk.tex",
		
		"Data\\NPC\\Pet\\textures\\pet_horse03_1_h.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_1_c.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_1_d.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_1_s.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_1_sk.tex",

		"Data\\NPC\\Pet\\textures\\pet_horse03_2_h.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_2_c.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_2_d.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_2_s.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_2_sk.tex",

		"Data\\NPC\\Pet\\textures\\pet_horse03_4_h.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_4_c.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_4_d.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_4_s.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_4_sk.tex",

		"Data\\NPC\\Pet\\textures\\pet_horse03_5_h.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_5_c.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_5_d.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_5_s.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_5_sk.tex",

		"Data\\NPC\\Pet\\textures\\pet_horse03_6_h.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_6_c.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_6_d.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_6_s.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_6_sk.tex",

		"Data\\NPC\\Pet\\textures\\pet_horse03_7_h.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_7_c.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_7_d.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_7_s.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse03_7_sk.tex",	
		
		"Data\\NPC\\Pet\\textures\\pet_horse_v01.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse_v02.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse_v03.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse_v04.tex",
		"Data\\NPC\\Pet\\textures\\pet_horse_v05.tex",	
		"Data\\NPC\\Pet\\textures\\pet_horse_v06.tex"
	},	

	//작은 용
	{	"Data\\NPC\\Pet\\pupu.tex",
		"Data\\NPC\\Pet\\textures\\pupu_1.tex",
		"Data\\NPC\\Pet\\textures\\pupu_2.tex",
		"Data\\NPC\\Pet\\textures\\pupu_3.tex",
		"Data\\NPC\\Pet\\textures\\pupu_4.tex",
		"Data\\NPC\\Pet\\textures\\pupu_5.tex",
		"Data\\NPC\\Pet\\textures\\pupu_6.tex",
		"Data\\NPC\\Pet\\textures\\pupu_7.tex",
		"Data\\NPC\\Pet\\textures\\pupu_8.tex",
		"Data\\NPC\\Pet\\textures\\pupu_9.tex",
		"Data\\NPC\\Pet\\textures\\pupu_10.tex",
		"",
		"",
		"",
		"",	
		"",
		"",
		"",
		"",		
		"",
		"",
		"",
		"",
		"",		 
		"",
		"",
		"",
		"",
		"", 
		"",
		"",
		"",
		"",
		"", 
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		""
	},

	//보통 용
	{	"Data\\NPC\\Pet\\pet_d_02.tex",
		"Data\\NPC\\Pet\\textures\\pet_d_01.tex",
		"Data\\NPC\\Pet\\textures\\pet_d_02.tex",
		"Data\\NPC\\Pet\\textures\\pet_d_03.tex",
		"Data\\NPC\\Pet\\textures\\pet_d_04.tex",
		"Data\\NPC\\Pet\\textures\\pet_d_05.tex",
		"Data\\NPC\\Pet\\textures\\pet_d_06.tex",
		"Data\\NPC\\Pet\\textures\\pet_d_07.tex",
		"Data\\NPC\\Pet\\textures\\pet_d_08.tex",
		"Data\\NPC\\Pet\\textures\\pet_d_09.tex",
		"Data\\NPC\\Pet\\textures\\pet_d_10.tex",
		"",
		"",	
		"",
		"",
		"",
		"",
		"",		
		"",
		"",
		"",
		"",
		"",		 
		"",
		"",
		"",
		"",
		"", 
		"",
		"",
		"",
		"",
		"", 
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		""
	},

	//타는 용
	{	"Data\\NPC\\Pet\\baroroques.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_1.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_2.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_3.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_4.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_5.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_6.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_7.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_8.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_9.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_10.tex",

		"Data\\NPC\\Pet\\textures\\baroroques_8_heart.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_9_heart.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_3_heart.tex",

		"Data\\NPC\\Pet\\textures\\baroroques_8_c.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_8_d.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_8_s.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_8_sk.tex",

		"Data\\NPC\\Pet\\textures\\baroroques_9_c.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_9_d.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_9_s.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_9_sk.tex",

		"Data\\NPC\\Pet\\textures\\baroroques_3_c.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_3_d.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_3_s.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_3_sk.tex",

		"Data\\NPC\\Pet\\textures\\baroroques_1_h.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_1_c.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_1_d.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_1_s.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_1_sk.tex",

		"Data\\NPC\\Pet\\textures\\baroroques_2_h.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_2_c.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_2_d.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_2_s.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_2_sk.tex",

		"Data\\NPC\\Pet\\textures\\baroroques_4_h.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_4_c.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_4_d.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_4_s.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_4_sk.tex",

		"Data\\NPC\\Pet\\textures\\baroroques_5_h.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_5_c.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_5_d.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_5_s.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_5_sk.tex",

		"Data\\NPC\\Pet\\textures\\baroroques_6_h.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_6_c.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_6_d.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_6_s.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_6_sk.tex",

		"Data\\NPC\\Pet\\textures\\baroroques_7_h.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_7_c.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_7_d.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_7_s.tex",
		"Data\\NPC\\Pet\\textures\\baroroques_7_sk.tex",
		"",
		"",
		"",
		"",
		"",
		""
	},	
};

// ----------------------------------------------------------------------------
// Name : CPetInfo()
// Desc :
// ----------------------------------------------------------------------------
CPetInfo::CPetInfo() : 
m_pPetInfo( NULL )
{
	m_pPetInfo					= new sPetInfo[TOTAL_PET];
	//-------------------------------------------------------
	m_pPetInfo[HORSE_CHILD].strName					= "Pony";
	m_pPetInfo[HORSE_CHILD].strFileName				= PET_HORSE_CHILD;

	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_WALK_1]		= "Pet_Horse01_walk";
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_IDLE_1]		= "Pet_Horse01_idle_01";	
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_IDLE_2]		= "Pet_Horse01_idle_02";	
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_RUN_1]		= "Pet_Horse01_run";		
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_PICK]		= "Pet_Horse01_pu";
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_DAMAGE]		= "Pet_Horse01_dam";
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_DIE]			= "Pet_Horse01_die";
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_LEVELUP]		= "Pet_Horse01_lvup";
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_SKILL_0]		= "Pet_Horse01_skill01";	// 고개 흔들기
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_SKILL_1]		= "Pet_Horse01_skill02";	// 뒷발길
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_SKILL_2]		= "Pet_Horse01_skill03";	// 말 울음.
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_SKILL_3]		= "Pet_Horse01_skill04";	// 
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_SKILL_4]		= "Pet_Horse01_skill05";	// 
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_SKILL_0_01]		= "Pet_Horse01_skill01_01";	// 고개 흔들기
	m_pPetInfo[HORSE_CHILD].aStrAnimationName[PET_ANIM_SKILL_0_02]		= "Pet_Horse01_skill01_02";	// 고개 흔들기
	m_pPetInfo[HORSE_CHILD].bRide									= FALSE;
	m_pPetInfo[HORSE_CHILD].fWalkSpeed								= 5.0f;
	m_pPetInfo[HORSE_CHILD].fRunSpeed								= 7.0f;

	//-------------------------------------------------------
	m_pPetInfo[HORSE_BOY].strName					="Horse";
	m_pPetInfo[HORSE_BOY].strFileName				= PET_HORSE_BOY;

	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_WALK_1]		= "Pet_Horse02_walk";
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_IDLE_1]		= "Pet_Horse02_idle_01";	
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_IDLE_2]		= "Pet_Horse02_idle_02";	
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_RUN_1]			= "Pet_Horse02_run";		
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_PICK]			= "Pet_Horse02_pu";
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_DAMAGE]		= "Pet_Horse02_dam";
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_DIE]			= "Pet_Horse02_die";
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_LEVELUP]		= "Pet_Horse02_lvup";
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_SKILL_0]		= "Pet_Horse02_skill01";	// 고개 흔들기
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_SKILL_1]		= "Pet_Horse02_skill02";	// 뒷발길
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_SKILL_2]		= "Pet_Horse02_skill03";	// 말 울음.
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_SKILL_3]		= "Pet_Horse02_skill04";	// 
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_SKILL_4]		= "Pet_Horse02_skill05";	// 
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_SKILL_0_01]		= "Pet_Horse02_skill01_01";	// 고개 흔들기
	m_pPetInfo[HORSE_BOY].aStrAnimationName[PET_ANIM_SKILL_0_02]		= "Pet_Horse02_skill01_02";	// 고개 흔들기
	m_pPetInfo[HORSE_BOY].bRide										= FALSE;
	m_pPetInfo[HORSE_BOY].fWalkSpeed								= 5.0f;
	m_pPetInfo[HORSE_BOY].fRunSpeed									= 8.0f;

	//-------------------------------------------------------
	m_pPetInfo[HORSE_ADULT].strName					= "Nightmare";
	m_pPetInfo[HORSE_ADULT].strFileName				= PET_HORSE_ADULT;

	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_WALK_1]		= "Pet_Horse3_walk";
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_IDLE_1]		= "Pet_Horse3_idle_01";	
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_IDLE_2]		= "Pet_Horse3_idle_01";	
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_RUN_1]		= "Pet_Horse3_run";		
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_PICK]		= "Pet_Horse3_idle_01";
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_DAMAGE]		= "Pet_Horse3_dam";
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_DIE]			= "Pet_Horse3_die";
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_LEVELUP]		= "Pet_Horse3_lvup";
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_SKILL_0]		= "Pet_Horse3_skill01";	// 앉기
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_SKILL_1]		= "Pet_Horse3_skill02";	// 
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_SKILL_2]		= "Pet_Horse3_skill03";	// 
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_SKILL_3]		= "Pet_Horse3_skill04";	// 
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_SKILL_4]		= "Pet_Horse3_skill05";	// 
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_SKILL_0_01]		= "Pet_Horse3_skill01_1";	// 
	m_pPetInfo[HORSE_ADULT].aStrAnimationName[PET_ANIM_SKILL_0_02]		= "Pet_Horse3_skill01_2";	// 
	m_pPetInfo[HORSE_ADULT].bRide									= TRUE;
	m_pPetInfo[HORSE_ADULT].fWalkSpeed								= 5.0f;
	m_pPetInfo[HORSE_ADULT].fRunSpeed								= 8.0f;

	//-------------------------------------------------------
	m_pPetInfo[DRAGON_CHILD].strName					= "Hatchling";
	m_pPetInfo[DRAGON_CHILD].strFileName				= PET_DRAGON_CHILD;

	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_WALK_1]		= "Pet_Dragon01_walk";
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_IDLE_1]		= "Pet_Dragon01_idle";	
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_IDLE_2]		= "Pet_Dragon01_idle";	
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_RUN_1]		= "Pet_Dragon01_run";		
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_PICK]		= "Pet_Dragon01_pu";
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_DAMAGE]		= "Pet_Dragon01_dam";
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_DIE]		= "Pet_Dragon01_die";
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_LEVELUP]	= "Pet_Dragon01_lvup";
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_SKILL_0]	= "Pet_Dragon01_skill01";	// 로어
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_SKILL_1]	= "Pet_Dragon01_skill02";	// 날개짓
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_SKILL_2]	= "Pet_Dragon01_skill03";	// 애교
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_SKILL_3]	= "Pet_Dragon01_skill04";	// 구애
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_SKILL_4]	= "Pet_Dragon01_skill05";	// 불뿜기
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_SKILL_0_01]	= "Pet_Dragon01_skill01_01";	// 로어
	m_pPetInfo[DRAGON_CHILD].aStrAnimationName[PET_ANIM_SKILL_0_02]	= "Pet_Dragon01_skill01_02";	// 로어
	m_pPetInfo[DRAGON_CHILD].bRide									= FALSE;
	m_pPetInfo[DRAGON_CHILD].fWalkSpeed								= 5.5f;
	m_pPetInfo[DRAGON_CHILD].fRunSpeed								= 7.0f;
	
	//-------------------------------------------------------
	m_pPetInfo[DRAGON_BOY].strName					= "Drake";
	m_pPetInfo[DRAGON_BOY].strFileName				= PET_DRAGON_BOY;

	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_WALK_1]		= "Pet_Dragon02_walk";
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_IDLE_1]		= "Pet_Dragon02_idle";	
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_IDLE_2]		= "Pet_Dragon02_idle02";	
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_RUN_1]		= "Pet_Dragon02_run";		
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_PICK]			= "Pet_Dragon02_pu";
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_DAMAGE]		= "Pet_Dragon02_dam";
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_DIE]			= "Pet_Dragon02_die";
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_LEVELUP]		= "Pet_Dragon02_lvup";
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_SKILL_0]		= "Pet_Dragon02_skill01";	// 로어
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_SKILL_1]		= "Pet_Dragon02_skill02";	// 날개짓
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_SKILL_2]		= "Pet_Dragon02_skill03";	// 애교
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_SKILL_3]		= "Pet_Dragon02_skill04";	// 구애
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_SKILL_4]		= "Pet_Dragon02_skill05";	// 불뿜기
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_SKILL_0_01]		= "Pet_Dragon02_skill01_01";	// 로어
	m_pPetInfo[DRAGON_BOY].aStrAnimationName[PET_ANIM_SKILL_0_02]		= "Pet_Dragon02_skill01_02";	// 로어
	m_pPetInfo[DRAGON_BOY].bRide										= FALSE;
	m_pPetInfo[DRAGON_BOY].fWalkSpeed								= 5.0f;
	m_pPetInfo[DRAGON_BOY].fRunSpeed								= 8.0f;

	//-------------------------------------------------------
	m_pPetInfo[DRAGON_ADULT].strName					= "Dragon";
	m_pPetInfo[DRAGON_ADULT].strFileName				= PET_DRAGON_ADULT;

	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_WALK_1]		= "Pet_Dragon03_walk";
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_IDLE_1]		= "Pet_Dragon03_idle01";	
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_IDLE_2]		= "Pet_Dragon03_idle02";	
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_RUN_1]		= "Pet_Dragon03_run";		
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_PICK]		= "Pet_Dragon03_idle01";
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_DAMAGE]		= "Pet_Dragon03_dam";
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_DIE]		= "Pet_Dragon03_die";
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_LEVELUP]	= "Pet_Dragon03_lvup";
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_SKILL_0]	= "Pet_Dragon03_skill01";	// 앉기
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_SKILL_1]	= "Pet_Dragon03_skill02";	//
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_SKILL_2]	= "Pet_Dragon03_skill03";	//
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_SKILL_3]	= "Pet_Dragon03_skill04";	//
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_SKILL_4]	= "Pet_Dragon03_skill05";	//
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_SKILL_0_01]	= "Pet_Dragon03_skill01_1";	//
	m_pPetInfo[DRAGON_ADULT].aStrAnimationName[PET_ANIM_SKILL_0_02]	= "Pet_Dragon03_skill01_2";	//
	m_pPetInfo[DRAGON_ADULT].bRide									= TRUE;
	m_pPetInfo[DRAGON_ADULT].fWalkSpeed								= 5.0f;
	m_pPetInfo[DRAGON_ADULT].fRunSpeed								= 8.0f;
	
	// Add rare horse & dragon
	// 레어 말
	m_pPetInfo[BLUE_HORSE_CHILD] = m_pPetInfo[MISTERY_HORSE_CHILD] = m_pPetInfo[HORSE_CHILD];
	m_pPetInfo[BLUE_HORSE_CHILD].strFileName =  PET_HORSE_CHILD_BLUE;
	m_pPetInfo[BLUE_HORSE_BOY] = m_pPetInfo[MISTERY_HORSE_BOY] = m_pPetInfo[HORSE_BOY];
	m_pPetInfo[BLUE_HORSE_BOY].strFileName =  PET_HORSE_BOY_BLUE;
	m_pPetInfo[BLUE_HORSE_ADULT] = m_pPetInfo[MISTERY_HORSE_ADULT] = m_pPetInfo[HORSE_ADULT];
	m_pPetInfo[BLUE_HORSE_ADULT].strFileName =  PET_HORSE_ADULT_BLUE;
	// 레어 용
	m_pPetInfo[PINK_DRAGON_CHILD] = m_pPetInfo[MISTERY_DRAGON_CHILD] = m_pPetInfo[DRAGON_CHILD];
	m_pPetInfo[PINK_DRAGON_CHILD].strFileName = PET_DRAGON_CHILD_PINK;
	m_pPetInfo[PINK_DRAGON_BOY] = m_pPetInfo[MISTERY_DRAGON_BOY] = m_pPetInfo[DRAGON_BOY];
	m_pPetInfo[PINK_DRAGON_BOY].strFileName = PET_DRAGON_BOY_PINK;
	m_pPetInfo[PINK_DRAGON_ADULT] = m_pPetInfo[MISTERY_DRAGON_ADULT] = m_pPetInfo[DRAGON_ADULT];
	m_pPetInfo[PINK_DRAGON_ADULT].strFileName = PET_DRAGON_ADULT_PINK;

}

// ----------------------------------------------------------------------------
// Name : ~CPetInfo()
// Desc :
// ----------------------------------------------------------------------------
CPetInfo::~CPetInfo()
{
	if( m_pPetInfo )
		delete [] m_pPetInfo;
}

// ----------------------------------------------------------------------------
// Name : SetName()
// Desc :
// ----------------------------------------------------------------------------
void CPetInfo::SetName( int iPet, const CTString& strName )
{
	m_pPetInfo[iPet].strName = strName;
}

// ----------------------------------------------------------------------------
// Name : GetName()
// Desc :
// ----------------------------------------------------------------------------
const CTString &CPetInfo::GetName( int iPet, int iAge ) const
{
	const int iPetType = iPet * MAX_PET_AGE + iAge;
	return m_pPetInfo[iPetType].strName;
}

// ----------------------------------------------------------------------------
// Name : GetFileName()
// Desc :
// ----------------------------------------------------------------------------
const CTString &CPetInfo::GetFileName( int iPet, int iAge ) const
{
	const int iPetType = iPet * MAX_PET_AGE + iAge;
	return m_pPetInfo[iPetType].strFileName;
}

// [070824: Su-won] PET_COLOR_CHANGE
// 펫 모델의 텍스쳐로 사용될 파일이름 반환
const CTString &CPetInfo::GetColoredTexFileName( int iPet, int iAge, int iColor ) const
{
	const int iPetType = iPet * MAX_PET_AGE + iAge;
	
	return strColoredTexFile[iPetType%6][iColor];
}


// ----------------------------------------------------------------------------
// Name : GetAnimationName()
// Desc :
// ----------------------------------------------------------------------------
const CTString &CPetInfo::GetAnimationName( int iPet, int iAnimType ) const
{ 
	return m_pPetInfo[iPet].aStrAnimationName[iAnimType];
}

// ----------------------------------------------------------------------------
// Name : IsRide()
// Desc :
// ----------------------------------------------------------------------------
BOOL CPetInfo::IsRide( int iPet, int iAge )
{
	if (iPet < 0 || iAge < 0)
		return FALSE;

	const int iPetType = iPet * MAX_PET_AGE + iAge;
	return m_pPetInfo[iPetType].bRide;
}

// ----------------------------------------------------------------------------
// Name : GetWalkSpeed()
// Desc :
// ----------------------------------------------------------------------------
FLOAT CPetInfo::GetWalkSpeed( int iPet, int iAge ) const
{
	const int iPetType = iPet * MAX_PET_AGE + iAge;
	return m_pPetInfo[iPetType].fWalkSpeed;
}

// ----------------------------------------------------------------------------
// Name : GetRunSpeed()
// Desc :
// ----------------------------------------------------------------------------
FLOAT CPetInfo::GetRunSpeed( int iPet, int iAge ) const
{
	const int iPetType = iPet * MAX_PET_AGE + iAge;
	return m_pPetInfo[iPetType].fRunSpeed;
}

// ----------------------------------------------------------------------------
// Name : SetPetDataToEntity()
// Desc :
// ----------------------------------------------------------------------------
void CPetInfo::SetPetDataToEntity( int iPet, int iAge, CEntity *pEntity, BOOL bUseAI )
{
	if(pEntity == NULL) 
		return;

	const int iPetType = iPet * MAX_PET_AGE + iAge;

	CEntityProperty &epPropertyWalk		= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 100));	// Walk1 Animation
	ENTITYPROPERTY( &*pEntity, epPropertyWalk.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_WALK_1));

	CEntityProperty &epPropertyIdle1	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 101));	// Idle1 Animation
	ENTITYPROPERTY( &*pEntity, epPropertyIdle1.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_IDLE_1));

	CEntityProperty &epPropertyIdle2	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 102));	// Idle2 Animation
	ENTITYPROPERTY( &*pEntity, epPropertyIdle2.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_IDLE_2));

	CEntityProperty &epPropertyRun		= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 103));	// Run1 Animation
	ENTITYPROPERTY( &*pEntity, epPropertyRun.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_RUN_1));

	CEntityProperty &epPropertyDie		= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 104));	// Die Animation
	ENTITYPROPERTY( &*pEntity, epPropertyDie.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_DIE));

	CEntityProperty &epPropertyDamage	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 105));	// Damage Animation
	ENTITYPROPERTY( &*pEntity, epPropertyDamage.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_DAMAGE));

	CEntityProperty &epPropertyAttack		= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 106));	// Attack Animation
	ENTITYPROPERTY( &*pEntity, epPropertyAttack.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_PICK));

	CEntityProperty &epPropertyLevelUp	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 108));	// Level Up Animation
	ENTITYPROPERTY( &*pEntity, epPropertyLevelUp.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_LEVELUP));

	CEntityProperty &epPropertySkill0	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 109));	// Skill0 Animation
	ENTITYPROPERTY( &*pEntity, epPropertySkill0.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_SKILL_0));

	CEntityProperty &epPropertySkill1	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 110));	// Skill1 Animation
	ENTITYPROPERTY( &*pEntity, epPropertySkill1.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_SKILL_1));

	CEntityProperty &epPropertySkill2	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 111));	// Skill2 Animation
	ENTITYPROPERTY( &*pEntity, epPropertySkill2.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_SKILL_2));

	CEntityProperty &epPropertySkill3	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 112));	// Skill3 Animation
	ENTITYPROPERTY( &*pEntity, epPropertySkill3.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_SKILL_3));

	CEntityProperty &epPropertySkill4	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 113));	// Skill4 Animation
	ENTITYPROPERTY( &*pEntity, epPropertySkill4.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_SKILL_4));

	CEntityProperty &epPropertySkill5	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 114));	// Skill4 Animation
	ENTITYPROPERTY( &*pEntity, epPropertySkill5.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_SKILL_0_01));

	CEntityProperty &epPropertySkill6	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 115));	// Skill4 Animation
	ENTITYPROPERTY( &*pEntity, epPropertySkill6.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_SKILL_0_02));

	CEntityProperty &epPropertyPick	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 116));		// Pick Animation
	ENTITYPROPERTY( &*pEntity, epPropertyPick.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iPetType, PET_ANIM_PICK));
	
	CEntityProperty &epPropertyName		= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_STRING, 200));	// Pet Name
	ENTITYPROPERTY( &*pEntity, epPropertyName.ep_slOffset, CTString)	= GetName(iPet, iAge);

	CDLLEntityClass *pdecDLLBaseClass	= pEntity->GetClass()->ec_pdecDLLClass->dec_pdecBase;

	CEntityProperty &epPropertyAge		= *pdecDLLBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 192);	// Pet Age
	ENTITYPROPERTY( &*pEntity, epPropertyAge.ep_slOffset, INDEX)		= iAge;	

	CEntityProperty &epPropertyType		= *pdecDLLBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 194);	// Pet Types
	ENTITYPROPERTY( &*pEntity, epPropertyType.ep_slOffset, INDEX)		= iPet;

	CDLLEntityClass *pdecDLLBaseBaseClass	= pdecDLLBaseClass->dec_pdecBase;
	// 이동 속도...
	CEntityProperty &epPropertyWalkSpeed		= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 10);	// NPC Walk Speed
	ENTITYPROPERTY( &*pEntity, epPropertyWalkSpeed.ep_slOffset, FLOAT)			= GetWalkSpeed( iPet, iAge );	

	CEntityProperty &epPropertyAttackRunSpeed	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 12);	// NPC Attack Run Speed
	ENTITYPROPERTY( &*pEntity, epPropertyAttackRunSpeed.ep_slOffset, FLOAT)		= GetRunSpeed( iPet, iAge );

	CEntityProperty &epPropertyCloseRunSpeed	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 14);	// NPC Close Run Speed
	ENTITYPROPERTY( &*pEntity, epPropertyCloseRunSpeed.ep_slOffset, FLOAT)		= GetRunSpeed( iPet, iAge );

	CEntityProperty &epPropertyUseAI	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 193);	// AI 사용 유무.
	ENTITYPROPERTY( &*pEntity, epPropertyUseAI.ep_slOffset, INDEX)		= bUseAI;

	// 공격 거리...
	CEntityProperty &epPropertyAttackDistance	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 20);	// Attack Distance
	ENTITYPROPERTY( &*pEntity, epPropertyAttackDistance.ep_slOffset, FLOAT)		= 4.0f;
	
	CEntityProperty &epPropertyStopDistance		= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 24);	// Attack Distance
	ENTITYPROPERTY( &*pEntity, epPropertyStopDistance.ep_slOffset, FLOAT)		= 3.0f;

	CEntityProperty &epPropertyCloseDistance	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 21);	// Attack Distance
	ENTITYPROPERTY( &*pEntity, epPropertyCloseDistance.ep_slOffset, FLOAT)		= 4.1f;
}