#include "stdh.h"
#include <Engine/Ska/ModelInstance.h>
#include <Engine/SlaveInfo.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>

#include <Engine/Base/Timer.h>

CSlaveInfo CSlaveInfo::m_instance;

#define SLAVE_FIRE_SMC	("Data\\NPC\\Summon\\summonfire.smc")
#define SLAVE_WATER_SMC	("Data\\NPC\\Summon\\summonwater.smc")
#define SLAVE_WIND_SMC	("Data\\NPC\\Summon\\summonwind.smc")
#define SLAVE_LAND_SMC	("Data\\NPC\\Summon\\summonland.smc")
#define SLAVE_GUARD_SMC ("Data\\NPC\\Soldier01\\rudgl.smc")

// ----------------------------------------------------------------------------
// Name : CSlaveInfo()
// Desc :
// ----------------------------------------------------------------------------
CSlaveInfo::CSlaveInfo() : 
m_pSlaveInfo( NULL )
{
	m_pSlaveInfo					= new sSlaveInfo[SLAVE_TOTAL];
	//-------------------------------------------------------
	m_pSlaveInfo[SLAVE_FIRE].strName					= "불의 정령";
	m_pSlaveInfo[SLAVE_FIRE].strFileName				= SLAVE_FIRE_SMC;

	m_pSlaveInfo[SLAVE_FIRE].aStrAnimationName[SLAVE_ANIM_WALK_1]		= "summon_fire_run";
	m_pSlaveInfo[SLAVE_FIRE].aStrAnimationName[SLAVE_ANIM_IDLE_1]		= "summon_fire_idle01";	
	m_pSlaveInfo[SLAVE_FIRE].aStrAnimationName[SLAVE_ANIM_IDLE_2]		= "summon_fire_idle01";	
	m_pSlaveInfo[SLAVE_FIRE].aStrAnimationName[SLAVE_ANIM_RUN_1]		= "summon_fire_run";			
	m_pSlaveInfo[SLAVE_FIRE].aStrAnimationName[SLAVE_ANIM_DAMAGE]		= "summon_fire_dam";
	m_pSlaveInfo[SLAVE_FIRE].aStrAnimationName[SLAVE_ANIM_DIE]			= "summon_fire_die";
	m_pSlaveInfo[SLAVE_FIRE].aStrAnimationName[SLAVE_ANIM_ATTACK_0]		= "summon_fire_att01";	
	m_pSlaveInfo[SLAVE_FIRE].aStrAnimationName[SLAVE_ANIM_ATTACK_1]		= "summon_fire_att02";		
	m_pSlaveInfo[SLAVE_FIRE].aStrAnimationName[SLAVE_ANIM_SKILL_0]		= "summon_fire_sof";
	m_pSlaveInfo[SLAVE_FIRE].aStrAnimationName[SLAVE_ANIM_SKILL_1]		= "summon_fire_sof";
	m_pSlaveInfo[SLAVE_FIRE].fWalkSpeed		= 5.0f;
	m_pSlaveInfo[SLAVE_FIRE].fRunSpeed		= 8.0f;

	//-------------------------------------------------------
	m_pSlaveInfo[SLAVE_WATER].strName					= "물의 정령";
	m_pSlaveInfo[SLAVE_WATER].strFileName				= SLAVE_WATER_SMC;

	m_pSlaveInfo[SLAVE_WATER].aStrAnimationName[SLAVE_ANIM_WALK_1]		= "summon_water_walk";
	m_pSlaveInfo[SLAVE_WATER].aStrAnimationName[SLAVE_ANIM_IDLE_1]		= "summon_water_idle01";	
	m_pSlaveInfo[SLAVE_WATER].aStrAnimationName[SLAVE_ANIM_IDLE_2]		= "summon_water_idle01";	
	m_pSlaveInfo[SLAVE_WATER].aStrAnimationName[SLAVE_ANIM_RUN_1]		= "summon_water_run";			
	m_pSlaveInfo[SLAVE_WATER].aStrAnimationName[SLAVE_ANIM_DAMAGE]		= "summon_water_dam";
	m_pSlaveInfo[SLAVE_WATER].aStrAnimationName[SLAVE_ANIM_DIE]			= "summon_water_die";
	m_pSlaveInfo[SLAVE_WATER].aStrAnimationName[SLAVE_ANIM_ATTACK_0]	= "summon_water_att01";	
	m_pSlaveInfo[SLAVE_WATER].aStrAnimationName[SLAVE_ANIM_ATTACK_1]	= "summon_water_att02";	
	m_pSlaveInfo[SLAVE_WATER].aStrAnimationName[SLAVE_ANIM_SKILL_0]		= "summonwater_jow";
	m_pSlaveInfo[SLAVE_WATER].aStrAnimationName[SLAVE_ANIM_SKILL_1]		= "summonwater_jow";
	m_pSlaveInfo[SLAVE_WATER].fWalkSpeed	= 5.0f;
	m_pSlaveInfo[SLAVE_WATER].fRunSpeed		= 8.0f;

	//-------------------------------------------------------
	m_pSlaveInfo[SLAVE_WIND].strName					= "바람의 정령";
	m_pSlaveInfo[SLAVE_WIND].strFileName				= SLAVE_WIND_SMC;

	m_pSlaveInfo[SLAVE_WIND].aStrAnimationName[SLAVE_ANIM_WALK_1]		= "summon_wind_walk";
	m_pSlaveInfo[SLAVE_WIND].aStrAnimationName[SLAVE_ANIM_IDLE_1]		= "summon_wind_idle01";	
	m_pSlaveInfo[SLAVE_WIND].aStrAnimationName[SLAVE_ANIM_IDLE_2]		= "summon_wind_idle02";	
	m_pSlaveInfo[SLAVE_WIND].aStrAnimationName[SLAVE_ANIM_RUN_1]		= "summon_wind_run";		
	m_pSlaveInfo[SLAVE_WIND].aStrAnimationName[SLAVE_ANIM_DAMAGE]		= "summon_wind_dam";
	m_pSlaveInfo[SLAVE_WIND].aStrAnimationName[SLAVE_ANIM_DIE]			= "summon_wind_die";
	m_pSlaveInfo[SLAVE_WIND].aStrAnimationName[SLAVE_ANIM_ATTACK_0]		= "summon_wind_att01";	
	m_pSlaveInfo[SLAVE_WIND].aStrAnimationName[SLAVE_ANIM_ATTACK_1]		= "summon_wind_att02";	
	m_pSlaveInfo[SLAVE_WIND].aStrAnimationName[SLAVE_ANIM_SKILL_0]		= "summon_wind_sow";
	m_pSlaveInfo[SLAVE_WIND].aStrAnimationName[SLAVE_ANIM_SKILL_1]		= "summon_wind_sow";
	m_pSlaveInfo[SLAVE_WIND].fWalkSpeed		= 5.0f;
	m_pSlaveInfo[SLAVE_WIND].fRunSpeed		= 8.0f;

	//-------------------------------------------------------
	m_pSlaveInfo[SLAVE_LAND].strName					= "대지의 정령";
	m_pSlaveInfo[SLAVE_LAND].strFileName				= SLAVE_LAND_SMC;

	m_pSlaveInfo[SLAVE_LAND].aStrAnimationName[SLAVE_ANIM_WALK_1]		= "summon_earth_run";
	m_pSlaveInfo[SLAVE_LAND].aStrAnimationName[SLAVE_ANIM_IDLE_1]		= "summon_earth_idle01";	
	m_pSlaveInfo[SLAVE_LAND].aStrAnimationName[SLAVE_ANIM_IDLE_2]		= "summon_earth_idle02";	
	m_pSlaveInfo[SLAVE_LAND].aStrAnimationName[SLAVE_ANIM_RUN_1]		= "summon_earth_run";			
	m_pSlaveInfo[SLAVE_LAND].aStrAnimationName[SLAVE_ANIM_DAMAGE]		= "summon_earth_dam";
	m_pSlaveInfo[SLAVE_LAND].aStrAnimationName[SLAVE_ANIM_DIE]			= "summon_earth_die";
	m_pSlaveInfo[SLAVE_LAND].aStrAnimationName[SLAVE_ANIM_ATTACK_0]		= "summon_earth_att01";	
	m_pSlaveInfo[SLAVE_LAND].aStrAnimationName[SLAVE_ANIM_ATTACK_1]		= "summon_earth_att02";	
	m_pSlaveInfo[SLAVE_LAND].aStrAnimationName[SLAVE_ANIM_SKILL_0]		= "summon_earth_hoe";
	m_pSlaveInfo[SLAVE_LAND].aStrAnimationName[SLAVE_ANIM_SKILL_1]		= "summon_earth_hoe";
	m_pSlaveInfo[SLAVE_LAND].fWalkSpeed		= 5.0f;
	m_pSlaveInfo[SLAVE_LAND].fRunSpeed		= 8.0f;
	//-------------------------------------------------------------
	m_pSlaveInfo[SLAVE_GUARD].strName					= "경비병";
	m_pSlaveInfo[SLAVE_GUARD].strFileName				= SLAVE_GUARD_SMC;

	m_pSlaveInfo[SLAVE_GUARD].aStrAnimationName[SLAVE_ANIM_WALK_1]		= "c_man_run";
	m_pSlaveInfo[SLAVE_GUARD].aStrAnimationName[SLAVE_ANIM_IDLE_1]		= "c_man_idle";	
	m_pSlaveInfo[SLAVE_GUARD].aStrAnimationName[SLAVE_ANIM_IDLE_2]		= "c_man_idle";	
	m_pSlaveInfo[SLAVE_GUARD].aStrAnimationName[SLAVE_ANIM_RUN_1]		= "c_man_run";			
	m_pSlaveInfo[SLAVE_GUARD].aStrAnimationName[SLAVE_ANIM_DAMAGE]		= "";
	m_pSlaveInfo[SLAVE_GUARD].aStrAnimationName[SLAVE_ANIM_DIE]			= "c_man_die";
	m_pSlaveInfo[SLAVE_GUARD].aStrAnimationName[SLAVE_ANIM_ATTACK_0]	= "c_man_att";	
	m_pSlaveInfo[SLAVE_GUARD].aStrAnimationName[SLAVE_ANIM_ATTACK_1]	= "c_man_att";	
	m_pSlaveInfo[SLAVE_GUARD].aStrAnimationName[SLAVE_ANIM_SKILL_0]		= "";
	m_pSlaveInfo[SLAVE_GUARD].aStrAnimationName[SLAVE_ANIM_SKILL_1]		= "";
	m_pSlaveInfo[SLAVE_GUARD].fWalkSpeed		= 5.0f;
	m_pSlaveInfo[SLAVE_GUARD].fRunSpeed		= 8.0f;
}

// ----------------------------------------------------------------------------
// Name : ~CSlaveInfo()
// Desc :
// ----------------------------------------------------------------------------
CSlaveInfo::~CSlaveInfo()
{
	if( m_pSlaveInfo )
		delete [] m_pSlaveInfo;
}

// ----------------------------------------------------------------------------
// Name : SetName()
// Desc :
// ----------------------------------------------------------------------------
void CSlaveInfo::SetName( int iSlave, const CTString& strName )
{
	m_pSlaveInfo[iSlave].strName = strName;
}

// ----------------------------------------------------------------------------
// Name : GetName()
// Desc :
// ----------------------------------------------------------------------------
const CTString &CSlaveInfo::GetName( int iSlave ) const
{
	const int iSlaveType = iSlave;
	return m_pSlaveInfo[iSlaveType].strName;
}

// ----------------------------------------------------------------------------
// Name : GetFileName()
// Desc :
// ----------------------------------------------------------------------------
const CTString &CSlaveInfo::GetFileName( int iSlave ) const
{
	const int iSlaveType = iSlave;
	return m_pSlaveInfo[iSlaveType].strFileName;
}

// ----------------------------------------------------------------------------
// Name : GetAnimationName()
// Desc :
// ----------------------------------------------------------------------------
const CTString &CSlaveInfo::GetAnimationName( int iSlave, int iAnimType ) const
{
	return m_pSlaveInfo[iSlave].aStrAnimationName[iAnimType];
}

// ----------------------------------------------------------------------------
// Name : GetWalkSpeed()
// Desc :
// ----------------------------------------------------------------------------
FLOAT CSlaveInfo::GetWalkSpeed( int iSlave ) const
{
	return m_pSlaveInfo[iSlave].fWalkSpeed;
}

// ----------------------------------------------------------------------------
// Name : GetRunSpeed()
// Desc :
// ----------------------------------------------------------------------------
FLOAT CSlaveInfo::GetRunSpeed( int iSlave ) const
{
	return m_pSlaveInfo[iSlave].fRunSpeed;
}

// ----------------------------------------------------------------------------
// Name : SetSkillIndex()
// Desc :
// ----------------------------------------------------------------------------
void CSlaveInfo::SetSkillIndex( int iSlave, INDEX iSkillIndex )
{
	m_pSlaveInfo[iSlave].iSkillIndex = iSkillIndex;
}

// ----------------------------------------------------------------------------
// Name : GetSkillIndex()
// Desc :
// ----------------------------------------------------------------------------
INDEX CSlaveInfo::GetSkillIndex( int iSlave ) const
{
	return m_pSlaveInfo[iSlave].iSkillIndex;
}

// ----------------------------------------------------------------------------
// Name : GetSkillIndex()
// Desc :
// ----------------------------------------------------------------------------
void CSlaveInfo::InitSkillIndex()
{
	for( int i = 0; i < SLAVE_TOTAL; ++i )
	{
		m_pSlaveInfo[i].iSkillIndex = -1;
	}
}

// ----------------------------------------------------------------------------
// Name : SetSlaveDataToEntity()
// Desc :
// ----------------------------------------------------------------------------
void CSlaveInfo::SetSlaveDataToEntity( int iSlave, CEntity *pEntity, BOOL bUseAI )
{
	if(pEntity == NULL) 
		return;

	const int iSlaveType = iSlave;

	CEntityProperty &epPropertyWalk		= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 100));	// Walk1 Animation
	ENTITYPROPERTY( &*pEntity, epPropertyWalk.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iSlaveType, SLAVE_ANIM_WALK_1));

	CEntityProperty &epPropertyIdle1	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 101));	// Idle1 Animation
	ENTITYPROPERTY( &*pEntity, epPropertyIdle1.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iSlaveType, SLAVE_ANIM_IDLE_1));

	CEntityProperty &epPropertyIdle2	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 102));	// Idle2 Animation
	ENTITYPROPERTY( &*pEntity, epPropertyIdle2.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iSlaveType, SLAVE_ANIM_IDLE_2));

	CEntityProperty &epPropertyRun		= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 103));	// Run1 Animation
	ENTITYPROPERTY( &*pEntity, epPropertyRun.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iSlaveType, SLAVE_ANIM_RUN_1));

	CEntityProperty &epPropertyDie		= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 104));	// Die Animation
	ENTITYPROPERTY( &*pEntity, epPropertyDie.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iSlaveType, SLAVE_ANIM_DIE));

	CEntityProperty &epPropertyDamage	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 105));	// Damage Animation
	ENTITYPROPERTY( &*pEntity, epPropertyDamage.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iSlaveType, SLAVE_ANIM_DAMAGE));	

	CEntityProperty &epPropertyAttack	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 106));	// Attack Animation
	ENTITYPROPERTY( &*pEntity, epPropertyAttack.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iSlaveType, SLAVE_ANIM_ATTACK_0));

	CEntityProperty &epPropertySkill0	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 109));	// Skill0 Animation
	ENTITYPROPERTY( &*pEntity, epPropertySkill0.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iSlaveType, SLAVE_ANIM_SKILL_0));

	CEntityProperty &epPropertySkill1	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 110));	// Skill1 Animation
	ENTITYPROPERTY( &*pEntity, epPropertySkill1.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iSlaveType, SLAVE_ANIM_SKILL_1));

	CEntityProperty &epPropertyAttack2	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 120));	// Attack2 Animation
	ENTITYPROPERTY( &*pEntity, epPropertyAttack2.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(GetAnimationName(iSlaveType, SLAVE_ANIM_ATTACK_1));
	
	CEntityProperty &epPropertyName		= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_STRING, 200));	// Slave Name
	ENTITYPROPERTY( &*pEntity, epPropertyName.ep_slOffset, CTString)	= GetName(iSlaveType);

	CDLLEntityClass *pdecDLLBaseClass	= pEntity->GetClass()->ec_pdecDLLClass->dec_pdecBase;

	CEntityProperty &epPropertyType		= *pdecDLLBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 194);	// Slave Types
	ENTITYPROPERTY( &*pEntity, epPropertyType.ep_slOffset, INDEX)		= iSlaveType;

	CEntityProperty &epPropertyEntity	= *pdecDLLBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_ENTITYPTR, 8);	// Owner
	ENTITYPROPERTY( &*pEntity, epPropertyEntity.ep_slOffset, CEntityPointer)		= pEntity;

	CDLLEntityClass *pdecDLLBaseBaseClass	= pdecDLLBaseClass->dec_pdecBase;

	CEntityProperty &epPropertyUseAI	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 193);	// AI 사용 유무.
	ENTITYPROPERTY( &*pEntity, epPropertyUseAI.ep_slOffset, INDEX)		= bUseAI;

	// 이동 속도...
	CEntityProperty &epPropertyWalkSpeed		= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 10);	// NPC Walk Speed
	ENTITYPROPERTY( &*pEntity, epPropertyWalkSpeed.ep_slOffset, FLOAT)			= GetWalkSpeed(iSlaveType);	

	CEntityProperty &epPropertyAttackRunSpeed	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 12);	// NPC Attack Run Speed
	ENTITYPROPERTY( &*pEntity, epPropertyAttackRunSpeed.ep_slOffset, FLOAT)		= GetRunSpeed(iSlaveType);

	CEntityProperty &epPropertyCloseRunSpeed	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 14);	// NPC Close Run Speed
	ENTITYPROPERTY( &*pEntity, epPropertyCloseRunSpeed.ep_slOffset, FLOAT)		= GetRunSpeed(iSlaveType);

	// 공격 거리...
	CEntityProperty &epPropertyAttackDistance	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 20);	// Attack Distance
	ENTITYPROPERTY( &*pEntity, epPropertyAttackDistance.ep_slOffset, FLOAT)		= 4.0f;
	
	CEntityProperty &epPropertyStopDistance		= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 24);	// Stop Distance
	ENTITYPROPERTY( &*pEntity, epPropertyStopDistance.ep_slOffset, FLOAT)		= 3.0f;

	CEntityProperty &epPropertyCloseDistance	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 21);	// Close Distance
	ENTITYPROPERTY( &*pEntity, epPropertyCloseDistance.ep_slOffset, FLOAT)		= 4.1f;
}

//------------------------------------------------------------------------------
// CSlaveInfo::StartIdleEffect
// Explain:  
// Date : 2005-12-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CSlaveInfo::StartIdleEffect( SLONG slSlaveIndex, SBYTE sbElementalType, CEntity* penEntity )
{
	CTString strEffectName;

	switch( sbElementalType )
	{
	case SLAVE_FIRE:
		strEffectName = "sfire_Idle";
		break;
	case SLAVE_WIND:
		strEffectName = "swind_sow_idle";
		break;
	case SLAVE_LAND:
		return;
		break;
	case SLAVE_WATER:
		strEffectName = "swater_idle";
		break;

	}

	// Start Effect
	CEffectGroup *pEG = StartEffectGroup( strEffectName
		, &penEntity->en_pmiModelInstance->m_tmSkaTagManager
		,_pTimer->GetLerpedCurrentTick());
	
	std::map<SLONG, CEffectGroup *>::value_type tmpVal( slSlaveIndex, pEG);
	m_mapEG.insert(tmpVal);
}


//------------------------------------------------------------------------------
// CSlaveInfo::StopIdleEffect
// Explain:  
// Date : 2005-12-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CSlaveInfo::StopIdleEffect( SLONG slSlaveIndex,  bool bAllStop )
{
	if( bAllStop )
	{
		// All Character Effect Stop
		std::map<SLONG, CEffectGroup *>::iterator	itEG;
		
		for( itEG = m_mapEG.begin(); itEG != m_mapEG.end(); ++itEG )
		{
			if( (*itEG).second != NULL && CEffectGroupManager::Instance().IsValidCreated( (*itEG).second ) )
				(*itEG).second->Stop(0.0f);
		}
		
		m_mapEG.clear();
	}
	else
	{
		// 이펙터가 활성화 되어 있지 않다면 끝내라..
		if( m_mapEG.find( slSlaveIndex ) == m_mapEG.end() ) return;
		
		if( m_mapEG[slSlaveIndex] != NULL && CEffectGroupManager::Instance().IsValidCreated( m_mapEG[slSlaveIndex] ) )
			m_mapEG[slSlaveIndex]->Stop(0.0f);
		
		m_mapEG.erase( slSlaveIndex );
		
	}
}