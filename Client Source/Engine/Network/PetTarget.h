#ifndef __PETTARGET_H__
#define __PETTARGET_H__

#include <Engine/Object/ObjectBase.h>
#include <Engine/Base/CTString.h>
#include <Engine/Interface/UIBuff.h>
#include <Engine/Entities/StatusEffect.h>
#include <Engine/Effect/CEffectGroup.h>
#include <Engine/GlobalDefinition.h>

class  CPetTargetInfom
{
public:
	LONG	lIndex;
	BOOL	bIsActive;	
	FLOAT	fMaxHealth;
	FLOAT	fHealth;
	FLOAT	fMaxHungry;
	FLOAT	fHungry;
	int		iLevel;
	int		iType;
	int		iAge;
	LONG	lAbility;
	INDEX	TransformIndex;
	CTString	strNameCard;

	CEntity	*pen_pEntity;	

public:
	CPetTargetInfom(void);
	~CPetTargetInfom(void);

	ENGINE_API void	Init();
};

// FIXME : 많은 정리 필요...ㅡ.ㅡ
class ENGINE_API CPetTarget : public ObjectBase
{
public:
	INDEX			pet_iAge;
	CTString		pet_OwnerName;
	CStatusEffect	pet_statusEffect;
	CEffectGroup	*pet_pNormalEffect;
	UWORD			pet_sbAttributePos;
	INDEX			pet_OwnerIndex;	
	CTString		pet_strNameCard;
	
public:
	/* Default constructor. */
	CPetTarget(void);
	/* Destructor. */
	~CPetTarget(void);
	
	CPetTarget(const CPetTarget &other);
	CPetTarget &operator=(const CPetTarget &other);
	
	void	SetData( INDEX index, CTString& strName, CTString& strOwnerName, INDEX iOwnerIndex, INDEX iPetType, INDEX iPetAge, CEntity *pEntity, SBYTE sbyLayer, CTString strNameCard );	
	void	SetClientPetId( INDEX index );
	void	ChangeStatus(CTagManager* pTM, SQUAD status)
	{
		if(pet_statusEffect.IsStatusChanged(status))
			pet_statusEffect.ChangeStatus(pTM, status, CStatusEffect::R_ENEMY);
	}
	void	ResetStatus()
	{
		pet_statusEffect.Reset();
	}
};
enum STATETYPE
{
	STATE_ATTACK		= 0,	// 공격력
	STATE_MAGIC_ATTACK	= 1,	// 마법 공격력
	STATE_DEFENCE		= 2,	// 방어력	
	STATE_MAGIC_DEFENCE	= 3,	// 마법 방어력
	STATE_HITPOINT		= 4,	// 명중도
	STATE_AVOID			= 5,	// 회피도
// 	STATE_STRONG		= 6,	// 강인함	
// 	STATE_MAGIC_AVOID	= 7,	// 마법 회피도
// 	STATE_ATTACK_SPEED	= 8,	// 공격 속도
// 	STATE_RUN_SPEED		= 9,	// 이동 속도
// 	STATE_CRITICAL		= 10,	// 크리티컬 확률
// 	STATE_DEADLY		= 11,	// 데들리 확률
	STATE_END,
};

class ENGINE_API CWildPetTarget : public ObjectBase
{
public:
	BOOL		bIsActive;
	BOOL		bDeath;
	INDEX		m_nIndex;		// 펫 툴 인덱스
	INDEX		m_nOwnerIndex;
	INDEX		m_nLevel;		// 공격 펫 레벨
	INDEX		m_nLevelupPoint;	// 공격 펫 성장포인트
	// Bagic
	INDEX		m_nStr;			// 공격 펫 힘
	INDEX		m_nStrPlus;		//	공격 펫 힘 추가
	INDEX		m_nCon;			// 공격 펫 체력
	INDEX		m_nConPlus;		// 공격 펫 체력 추가	
	INDEX		m_nDex;			// 공격 펫 민첩
	INDEX		m_nDexPlus;		// 공격 펫 민첩 추가
	INDEX		m_nInt;			// 공력 펫 지혜
	INDEX		m_nIntPlus;		// 공격 펫 지혜 추가
	__int64		m_exp;			// 공격 펫 경험치
	__int64		m_next_exp;		// 공격 펫 맥스(레벨업) 경험치
	INDEX		m_nHP;			// 공격 펫 HP
	INDEX		m_nMP;			// 공격 펫 MP
	INDEX		m_nMaxHP;		// 공격 펫 Max HP
	INDEX		m_nMaxMP;		// 공격 펫 Max MP
	INDEX		m_nFaith;		// 공격 펫 충성심
	INDEX		m_nStm;			// 공격 펫 배고픔
	INDEX		m_nMaxFaith;	// 공격 펫 최대 충성심
	INDEX		m_nMaxStm;		// 공격 펫 최대 배고픔
	INDEX		m_nSpeed;		// 공격 펫 이동 속도
	INDEX		m_nAISlot;		// 공격 펫 AI 슬롯개수
	UBYTE		m_sbAttributePos;
	SBYTE		m_sbTransStat;
	BOOL		m_bMount;
	// state
	INDEX	m_nWildPetState[STATE_END];	// 공격 펫 상태
	INDEX	m_nWildPetStatePlus[STATE_END]; // 상태 플러스
	INDEX	m_nPetWearIndex[WILDPET_WEAR_TOTAL];
	INDEX	m_nPetWearPlus[WILDPET_WEAR_TOTAL];
public:
	CWildPetTarget();
	~CWildPetTarget();

	CWildPetTarget(const CWildPetTarget &other);
	CWildPetTarget &operator=(const CWildPetTarget &other);	

	void SetWildPetToEntity(CEntity *pEntity, INDEX nindex, INDEX *nWearIndex, INDEX *nWearPlus);

};

#endif //__PETTARGET_H__