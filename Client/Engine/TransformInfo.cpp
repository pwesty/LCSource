#include "stdh.h"
#include <Engine/TransformInfo.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/SlaveInfo.h>

CTransformInfo CTransformInfo::m_instance;

#define SUMMON_TYPE1	("Data\\character\\Sorcerer\\sot1.smc")
#define SUMMON_TYPE2	("Data\\character\\Sorcerer\\sot2.smc")

// ----------------------------------------------------------------------------
// Name : CTransformInfo()
// Desc :
// ----------------------------------------------------------------------------
CTransformInfo::CTransformInfo() : 
m_pTransformInfo( NULL )
{
	m_pTransformInfo					= new sTransformInfo[TOTAL_TRANSFORM];
	//-------------------------------------------------------
	m_pTransformInfo[TYPE_2].strName					= "변신체1";
	m_pTransformInfo[TYPE_2].strFileName				= SUMMON_TYPE1;

	// FIXME : 애니메이션 이름은 공식이 없나???
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_WALK_1]		= "sot1_walk";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_WALK_2]		= "sot1_walk";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_IDLE]		= "sot1_idle01";	
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_RUN_1]		= "sot1_run";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_RUN_2]		= "sot1_run";	
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_ATTACK_IDLE]= "sot1_idle01";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_ATTACK_1]	= "sot1_att01";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_ATTACK_2]	= "sot1_att02";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_ATTACK_3]	= "sot1_att03";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_ATTACK_4]	= "sot1_att01";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_SIT]		= "sot1_sit_01";	// 앉기.
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_SIT_CONTINUE]= "sot1_sit_02";	// 앉아 있는 상태.
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_STANDUP]	= "sot1_sit_03";	// 일어서기.	
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_PICK]		= "sot1_pu";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_DAMAGE]		= "sot1_dam";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_DIE]		= "sot1_dam";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_EXT_IDLE_1]	= "sot1_idle01";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_EXT_RUN_2]	= "sot1_run";	
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_EXT_WALK_2]	= "sot1_walk";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_EXT_ATTACK_IDLE]= "sot1_idle01";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_EXT_ATTACK_1]= "sot1_att01";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_EXT_ATTACK_2]= "sot1_att02";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_EXT_ATTACK_3]= "sot1_att03";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_EXT_ATTACK_4]= "sot1_att01";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_EXT_PICK]	= "sot1_pu";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_EXT_DAMAGE]	= "sot1_dam";
	m_pTransformInfo[TYPE_2].aStrAnimationName[ANIM_EXT_DIE]	= "sot1_dam";

	m_pTransformInfo[TYPE_2].afImpactTimeTable[ATTACK1] = 0.66f;
	m_pTransformInfo[TYPE_2].afImpactTimeTable[ATTACK2] = 0.79f;
	m_pTransformInfo[TYPE_2].afImpactTimeTable[ATTACK3] = 0.68f;
	m_pTransformInfo[TYPE_2].afImpactTimeTable[ATTACK4] = 0.66f;
	SetInfo(TYPE_2, 309);
	//-------------------------------------------------------
	m_pTransformInfo[TYPE_1].strName					= "변신체2";
	m_pTransformInfo[TYPE_1].strFileName				= SUMMON_TYPE2;

	// FIXME : 애니메이션 이름은 공식이 없나???
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_WALK_1]		= "sot2_walk";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_WALK_2]		= "sot2_walk";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_IDLE]		= "sot2_idle_01";	
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_RUN_1]		= "sot2_run";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_RUN_2]		= "sot2_run";	
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_ATTACK_IDLE]= "sot2_idle_01";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_ATTACK_1]	= "sot2_att_01";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_ATTACK_2]	= "sot2_att_02";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_ATTACK_3]	= "sot2_att_01";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_ATTACK_4]	= "sot2_att_02";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_SIT]		= "sot2_sit_01";	// 앉기.
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_SIT_CONTINUE]= "sot2_sit_02";	// 앉아 있는 상태.
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_STANDUP]	= "sot2_sit_03";	// 일어서기.	
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_PICK]		= "sot2_pu";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_DAMAGE]		= "sot2_dam";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_DIE]		= "sot2_dam";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_EXT_IDLE_1]	= "sot2_idle_01";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_EXT_RUN_2]	= "sot2_run";	
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_EXT_WALK_2]	= "sot2_walk";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_EXT_ATTACK_IDLE]= "sot2_idle_01";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_EXT_ATTACK_1]= "sot2_att_01";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_EXT_ATTACK_2]= "sot2_att_02";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_EXT_ATTACK_3]= "sot2_att_01";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_EXT_ATTACK_4]= "sot2_att_02";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_EXT_PICK]	= "sot2_pu";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_EXT_DAMAGE]	= "sot2_dam";
	m_pTransformInfo[TYPE_1].aStrAnimationName[ANIM_EXT_DIE]	= "sot2_dam";	

	m_pTransformInfo[TYPE_1].afImpactTimeTable[ATTACK1] = 0.92f;
	m_pTransformInfo[TYPE_1].afImpactTimeTable[ATTACK2] = 0.96f;
	m_pTransformInfo[TYPE_1].afImpactTimeTable[ATTACK3] = 0.92f;
	m_pTransformInfo[TYPE_1].afImpactTimeTable[ATTACK4] = 0.96f;
	SetInfo(TYPE_1, 313);
}

// ----------------------------------------------------------------------------
// Name : ~CTransformInfo()
// Desc :
// ----------------------------------------------------------------------------
CTransformInfo::~CTransformInfo()
{
	if( m_pTransformInfo )
		delete [] m_pTransformInfo;
}

// ----------------------------------------------------------------------------
// Name : SetName()
// Desc :
// ----------------------------------------------------------------------------
void CTransformInfo::SetName( int iTransform, const CTString& strName )
{
	m_pTransformInfo[iTransform].strName = strName;
}

// ----------------------------------------------------------------------------
// Name : GetName()
// Desc :
// ----------------------------------------------------------------------------
CTString CTransformInfo::GetName( int iTransform )
{	
	return m_pTransformInfo[iTransform].strName;
}

// ----------------------------------------------------------------------------
// Name : GetFileName()
// Desc :
// ----------------------------------------------------------------------------
CTString CTransformInfo::GetFileName( int iTransform )
{
	return m_pTransformInfo[iTransform].strFileName;
}

// ----------------------------------------------------------------------------
// Name : GetImpactTime()
// Desc :
// ----------------------------------------------------------------------------
float CTransformInfo::GetImpactTime( int iTransform, int iAttackType )	const
{
	return m_pTransformInfo[iTransform].afImpactTimeTable[iAttackType];
}

// ----------------------------------------------------------------------------
// Name : GetAnimationName()
// Desc :
// ----------------------------------------------------------------------------
CTString CTransformInfo::GetAnimationName( int iTransform, int iAnimType )
{
	return m_pTransformInfo[iTransform].aStrAnimationName[iAnimType];
}

void CTransformInfo::SetInfo( int iTransform, int nIndex )
{
	m_pTransformInfo[iTransform].nSkillIndex = nIndex;
}

int CTransformInfo::GetSkillIndex( int iTransform )
{
	return m_pTransformInfo[iTransform].nSkillIndex;
}

int CTransformInfo::GetType(int nSkillIndex)
{
	for (int i = 0; i < TOTAL_TRANSFORM; ++i)
	{
		if (m_pTransformInfo[i].nSkillIndex == nSkillIndex)
			return i;
	}
	return -1;
}
