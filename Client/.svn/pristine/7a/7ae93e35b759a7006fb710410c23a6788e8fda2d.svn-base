#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Network/SlaveTarget.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Network/CNetwork.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/SlaveInfo.h>
#include <Engine/Info/MyInfo.h>

CSlaveTargetInfom::CSlaveTargetInfom(void)
{	
	Init();
}

CSlaveTargetInfom::~CSlaveTargetInfom(void)
{

}

void CSlaveTargetInfom::Init(void)
{
	lIndex			= -1;
	fMaxHealth		= 0.0f;
	fHealth			= 0.0f;
	bIsActive		= FALSE;
	pen_pEntity		= NULL;
	iType			= -1;		
}

/*
 *  Constructor.
 */
CSlaveTarget::CSlaveTarget(void) 
{
	m_eType = eOBJ_SLAVE;

	m_nIdxServer	= -1;
	m_nType			= -1;                	
	m_nIdxClient	= -1;
	m_yLayer			= 0;
	m_pEntity			= NULL;
	slave_pNormalEffect		= NULL;
	slave_sbAttributePos	= MATT_UNWALKABLE;
	slave_OwnerIndex		= -1;
	slave_BuffCount			= 0;
	slave_OwnerName		= CTString("");
	m_strName			= CTString("");
	slave_statusEffect.SetType(CStatusEffect::T_NPC);
}

/*
 *  Destructor.
 */
CSlaveTarget::~CSlaveTarget(void) 
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	if (slave_OwnerIndex == _pNetwork->MyCharacterInfo.index )
	{
		for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
		{
			CUISummon* pUISummon = (CUISummon*)pUIMgr->GetUI(i);
			if (pUISummon->GetSummonEntity() && pUISummon->GetSummonIndex() == m_nIdxServer)
			{
				pUISummon->WarpOfResetSummon();							
			}
		}
	}

	SlaveInfo().StopIdleEffect( m_nIdxServer );		

	//해당 소환수의 타겟 이펙트를 없앰...
	pUIMgr->StopTargetEffect( m_nIdxServer );	

	if (m_pEntity != NULL) 
	{
		ObjInfo* pInfo = ObjInfo::getSingleton();
		m_pEntity->en_pSlaveTarget = NULL;

		if (m_pEntity == pInfo->GetTargetEntity(eTARGET_REAL))
		{
			pInfo->TargetClear(eTARGET_REAL);
		}

		if (m_pEntity == pInfo->GetTargetEntity(eTARGET))
		{
			pInfo->TargetClear(eTARGET);
		}

		m_pEntity->Destroy( FALSE );
	}

	slave_statusEffect.Reset();

	if (slave_pNormalEffect != NULL)
	{
		DestroyEffectGroupIfValid(slave_pNormalEffect);
		slave_pNormalEffect = NULL;
	}
	
	slave_statusEffect.SetType(CStatusEffect::T_NPC);
}

CSlaveTarget::CSlaveTarget(const CSlaveTarget &other)
{
	m_eType = other.m_eType;
	m_nIdxServer	= other.m_nIdxServer;
	m_nType	= other.m_nType;	
	m_nIdxClient = other.m_nIdxClient;	
	m_pEntity = other.m_pEntity;
	m_strName	= other.m_strName;
	slave_OwnerName	= other.slave_OwnerName;
	m_yLayer = other.m_yLayer;
	memcpy(slave_Buff, other.slave_Buff, sizeof(slave_Buff[0])*20);
	//Hardcoding, status effect에 vtable이 추가되거나 상속이 되는 경우 문제 생김.
	memcpy(&slave_statusEffect, &other.slave_statusEffect, sizeof(slave_statusEffect));
	memset((void*)&other.slave_statusEffect, 0, sizeof(other.slave_statusEffect));
	slave_pNormalEffect = other.slave_pNormalEffect;
	slave_sbAttributePos	= other.slave_sbAttributePos;
	slave_OwnerIndex	= other.slave_OwnerIndex;
	slave_BuffCount		= other.slave_BuffCount;
	slave_statusEffect.Reset();
	memcpy(&slave_statusEffect, &other.slave_statusEffect, sizeof(slave_statusEffect));
	memset((void*)&other.slave_statusEffect, 0, sizeof(other.slave_statusEffect));
	slave_statusEffect.SetType(CStatusEffect::T_NPC);
}

CSlaveTarget &CSlaveTarget::operator=(const CSlaveTarget &other)
{
	m_eType = other.m_eType;
	m_nIdxServer = other.m_nIdxServer;
	m_nType = other.m_nType;	
	m_nIdxClient = other.m_nIdxClient;	
	m_pEntity = other.m_pEntity;
	m_strName = other.m_strName;
	slave_OwnerName = other.slave_OwnerName;
	m_yLayer = other.m_yLayer;
	//Hardcoding, status effect에 vtable이 추가되가나 상속이 되는 경우 문제 생김.
	memcpy(&slave_statusEffect, &other.slave_statusEffect, sizeof(slave_statusEffect));
	memset((void*)&other.slave_statusEffect, 0, sizeof(other.slave_statusEffect));
	slave_pNormalEffect = other.slave_pNormalEffect;
	slave_sbAttributePos	= other.slave_sbAttributePos;
	slave_OwnerIndex	= other.slave_OwnerIndex;
	slave_BuffCount		= other.slave_BuffCount;
	memcpy(slave_Buff, other.slave_Buff, sizeof(slave_Buff[0])*20);
	slave_statusEffect.Reset();
	memcpy(&slave_statusEffect, &other.slave_statusEffect, sizeof(slave_statusEffect));
	memset((void*)&other.slave_statusEffect, 0, sizeof(other.slave_statusEffect));
	slave_statusEffect.SetType(CStatusEffect::T_NPC);
	return *this;
}

void CSlaveTarget::RemoveBuff( SLONG slItemIndex, SLONG slSkillIndex )
{
	int iBuff;
	for( iBuff = 0; iBuff < slave_BuffCount; iBuff++ )
	{
		if( slave_Buff[iBuff].m_slItemIndex == slItemIndex &&
			slave_Buff[iBuff].m_slSkillIndex == slSkillIndex )
			break;
	}

	ASSERT( iBuff < slave_BuffCount );
	if( iBuff == slave_BuffCount )
		return;

	for( iBuff++; iBuff < slave_BuffCount; iBuff++ )
	{
		slave_Buff[iBuff - 1] = slave_Buff[iBuff];
	}

	slave_BuffCount--;
	slave_Buff[slave_BuffCount].Init();
}

//강동민 수정 끝 싱글 던젼 작업		07.27
#include <Engine/Network/Server.h>	// TEST
void CSlaveTarget::SetData( INDEX index, CTString& strName, CTString& strOwnerName, INDEX iOwnerIndex, INDEX iSlaveType, CEntity *pEntity, SBYTE sbyLayer)
{
	m_nIdxServer		= index;
	m_nType		= iSlaveType;    	
	m_strName		= strName;	
	slave_OwnerName	= strOwnerName;	
	m_pEntity		= pEntity;
	m_yLayer		= sbyLayer;
	slave_OwnerIndex	= iOwnerIndex;
	slave_statusEffect.SetType(CStatusEffect::T_NPC);
}

void CSlaveTarget::SetClientSlaveId(INDEX index)
{	
	m_nIdxClient = index;	
}