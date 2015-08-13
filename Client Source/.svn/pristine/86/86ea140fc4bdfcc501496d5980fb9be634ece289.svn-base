#include "stdh.h"
#include <Engine/Network/MultiTarget.h>
#include <Engine/Entities/EntityPointer.h>

CSelectedEntities::CSelectedEntities()
{
}

void CSelectedEntities::Add(CEntity* pEntity)
{
	if( pEntity == NULL)
	{
		return;
	}

	//if( pEntity->IsFlagOff( ENF_ALIVE ) )	
	//	return;

	CEntityPointer	pTempPointer	= pEntity;
	if( !IsMember( pTempPointer ) )
	{
		vectorSelectedEntities.push_back( pTempPointer );
	}
}

void CSelectedEntities::Del(CEntityPointer pEntity)
{
	ENTITIES_ITERATOR it = std::find(vectorSelectedEntities.begin(), vectorSelectedEntities.end(), pEntity);
	if( it != vectorSelectedEntities.end() )
	{
		vectorSelectedEntities.erase( it );			
	}	
}

void CSelectedEntities::Clear()
{	
	if( !vectorSelectedEntities.empty() )
	{
		for( ENTITIES_ITERATOR it = vectorSelectedEntities.begin();
			it != vectorSelectedEntities.end(); )
		{
			it = vectorSelectedEntities.erase( it );			
		}
		//vectorSelectedEntities.clear();			
	}
}

bool CSelectedEntities::EntSelectedCompare(const CEntityPointer& l, const CEntityPointer& r)
{
	FLOAT3D temp1 = l->GetPlacement().pl_PositionVector - g_EntSelectedCompareEntity->GetPlacement().pl_PositionVector;
	FLOAT3D temp2 = r->GetPlacement().pl_PositionVector - g_EntSelectedCompareEntity->GetPlacement().pl_PositionVector;

	return temp1.Length() < temp2.Length();
}

void CSelectedEntities::SelectedEntitiesSort(void)
{
	if (g_EntSelectedCompareEntity == NULL)
		return;

	if (!vectorSelectedEntities.empty())
	{
		std::sort(vectorSelectedEntities.begin(), vectorSelectedEntities.end(), EntSelectedCompare);
	}

	g_EntSelectedCompareEntity = NULL; // 사용후 초기화
}