#ifndef __MULTITARGET_H__
#define __MULTITARGET_H__

#include <vector>
#include <algorithm>
#include <Engine/Entities/Entity.h>

typedef std::vector<CEntityPointer>				ENTITIES_VECTOR;
typedef std::vector<CEntityPointer>::iterator	ENTITIES_ITERATOR;

class ENGINE_API CSelectedEntities
{
public:
	CSelectedEntities();	

	void	Add(CEntity* pEntity);
	void	Del(CEntityPointer pEntity);
	void	Clear();

	int		Count()
	{
		return vectorSelectedEntities.size();
	};

	BOOL	IsMember(CEntityPointer pEntity)
	{
		ENTITIES_ITERATOR it = std::find(vectorSelectedEntities.begin(), vectorSelectedEntities.end(), pEntity);
		if( it == vectorSelectedEntities.end() )
		{
			return FALSE;
		}
		return TRUE;
	};

	static bool	EntSelectedCompare(const CEntityPointer& l, const CEntityPointer& r);
	void	SelectedEntitiesSort(void);

	ENTITIES_VECTOR		vectorSelectedEntities;	
};

static CEntityPointer		g_EntSelectedCompareEntity = NULL;	// 비교 중심
#endif //__MULTITARGET_H__