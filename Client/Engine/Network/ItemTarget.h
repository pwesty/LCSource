#ifndef __ITEMTARGET_H__
#define __ITEMTARGET_H__
//#include <Engine/Network/NetworkMessage.h>
#include <Engine/Object/ObjectBase.h>
#include <Engine/Effect/CEffectGroup.h>

class ENGINE_API CItemTarget : public ObjectBase
{
public:
	SQUAD   item_llCount;
	FLOAT3D item_place;
	CEffectGroup *item_pDropItemEffect;

public:
	
	/* Default constructor. */
	CItemTarget(void);
	/* Destructor. */
	~CItemTarget(void);
	
	void	SetData( INDEX index, INDEX type, CEntity* pEntity, SBYTE sbyLayer );
	void	SetData( INDEX index, const char *name, CEntity* pEntity, SBYTE sbyLayer, SQUAD count,FLOAT3D place, int type);
	void	SetClientItemId( INDEX index );
};

#endif //__ITEMTARGET_H__