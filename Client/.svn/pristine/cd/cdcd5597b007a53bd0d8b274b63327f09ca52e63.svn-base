#ifndef __SLAVETARGET_H__
#define __SLAVETARGET_H__


#include <Engine/Object/ObjectBase.h>
#include <Engine/Base/CTString.h>
#include <Engine/Interface/UIBuff.h>
#include <Engine/Entities/StatusEffect.h>
#include <Engine/Effect/CEffectGroup.h>

class  CSlaveTargetInfom
{
public:
	LONG	lIndex;
	BOOL	bIsActive;	
	FLOAT	fMaxHealth;
	FLOAT	fHealth;	
	int		iType;		

	CEntity	*pen_pEntity;	

public:
	CSlaveTargetInfom(void);
	~CSlaveTargetInfom(void);

	ENGINE_API void	Init();
};

// FIXME : 많은 정리 필요...ㅡ.ㅡ
class ENGINE_API CSlaveTarget : public ObjectBase
{
public:
	SBYTE			slave_BuffCount;
	BuffInfo		slave_Buff[20];
	CTString		slave_OwnerName;
	SBYTE			slave_yLayer;	
	CStatusEffect	slave_statusEffect;
	CEffectGroup	*slave_pNormalEffect;
	UWORD			slave_sbAttributePos;
	INDEX			slave_OwnerIndex;	
	
public:
	/* Default constructor. */
	CSlaveTarget(void);
	/* Destructor. */
	~CSlaveTarget(void);
	
	CSlaveTarget(const CSlaveTarget &other);
	CSlaveTarget &operator=(const CSlaveTarget &other);
	
	void	SetData( INDEX index, CTString& strName, CTString& strOwnerName, INDEX iOwnerIndex, INDEX iSlaveType, CEntity *pEntity, SBYTE sbyLayer );	
	void	SetClientSlaveId( INDEX index );
	void	AddBuff( BuffInfo &rBuffInfo )
	{
		slave_Buff[slave_BuffCount++] = rBuffInfo;
	}
	void	RemoveBuff( SLONG slItemIndex, SLONG slSkillIndex );

	void	ChangeStatus(CTagManager* pTM, SQUAD status)
	{
		if(slave_statusEffect.IsStatusChanged(status))
			slave_statusEffect.ChangeStatus(pTM, status, CStatusEffect::R_ENEMY);
	}
	void	ResetStatus()
	{
		slave_statusEffect.Reset();
	}
};

#endif //__SLAVETARGET_H__