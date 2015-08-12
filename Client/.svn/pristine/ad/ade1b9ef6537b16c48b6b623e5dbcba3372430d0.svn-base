#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Network/ItemTarget.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Network/CNetwork.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Info/MyInfo.h>

/*
*  Constructor.
*/
CItemTarget::CItemTarget(void) 
{
	m_eType = eOBJ_ITEM;

	m_nIdxServer = -1;
	m_nType = -1;                
	m_nIdxClient = -1;
	m_yLayer = 0;
	m_pEntity = NULL;
	item_llCount = 0;
	item_place = FLOAT3D(0.0f,0.0f,0.0f);
	item_pDropItemEffect = NULL;
}

/*
*  Destructor.
*/
CItemTarget::~CItemTarget(void) 
{
	if(item_pDropItemEffect)
	{
		DestroyEffectGroupIfValid(item_pDropItemEffect);
		item_pDropItemEffect = NULL;
	}

	if (m_pEntity != NULL)
	{
		ObjInfo* pInfo = ObjInfo::getSingleton();

		if (m_pEntity == pInfo->GetTargetEntity(eTARGET_REAL))
		{
			pInfo->TargetClear(eTARGET_REAL);
		}

		if (m_pEntity == pInfo->GetTargetEntity(eTARGET))
		{
			pInfo->TargetClear(eTARGET);
		}

		if (m_pEntity->GetRenderType() == CEntity::RT_SKAMODEL)
			m_pEntity->GetModelInstance()->m_tmSkaTagManager.Unregister("__ROOT");
		
		m_pEntity->Destroy( FALSE );
		m_pEntity = NULL;
	}
}

void CItemTarget::SetData( INDEX index, INDEX type, CEntity* pEntity, SBYTE sbyLayer )
{	
	m_nIdxServer = index;
	m_nType = type;
	m_yLayer = sbyLayer;
	m_pEntity = pEntity;
}

void CItemTarget::SetData( INDEX index, const char *name, CEntity* pEntity, SBYTE sbyLayer, SQUAD count,FLOAT3D place, int type )
{	
	m_nIdxServer = index;	
	m_strName = name;
	m_yLayer = sbyLayer;
	m_pEntity = pEntity;
	item_llCount = count;
	item_place = place;
	m_nType = type;
}

void CItemTarget::SetClientItemId(INDEX index)
{	
	m_nIdxClient = index;	
}