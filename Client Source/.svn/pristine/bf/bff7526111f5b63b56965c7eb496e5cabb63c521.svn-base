#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Network/PetTarget.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Network/CNetwork.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIWildPetInfo.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Info/MyInfo.h>

CPetTargetInfom::CPetTargetInfom(void)
{	
	Init();
}

CPetTargetInfom::~CPetTargetInfom(void)
{

}

void CPetTargetInfom::Init(void)
{
	lIndex			= -1;
	fMaxHealth		= 0.0f;
	fHealth			= 0.0f;
	fMaxHungry		= 0.0f;
	fHungry			= 0.0f;
	bIsActive		= FALSE;
	iLevel			= 0;	
	pen_pEntity		= NULL;
	iType			= -1;
	iAge			= -1;
	lAbility		= -1;
	TransformIndex	= 0;
	strNameCard		= CTString("");
}

/*
 *  Constructor.
 */
CPetTarget::CPetTarget(void) 
{
	m_eType = eOBJ_PET;

	m_nIdxServer			= -1;
	m_nType			= -1;                
	pet_iAge			= -1;                
	m_nIdxClient	= -1;
	m_yLayer			= 0;
	m_pEntity			= NULL;
	pet_pNormalEffect	= NULL;
	pet_sbAttributePos	= MATT_UNWALKABLE;
	pet_OwnerIndex		= -1;
	pet_OwnerName		= CTString("");
	pet_strNameCard		= CTString("");
}

/*
 *  Destructor.
 */
CPetTarget::~CPetTarget(void) 
{
	CUIManager* pUIMgr = CUIManager::getSingleton();
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if(pet_pNormalEffect)
	{
		DestroyEffectGroupIfValid(pet_pNormalEffect);
		pet_pNormalEffect = NULL;
	}

	pUIMgr->StopTargetEffect(m_nIdxServer);

	if (pet_OwnerIndex == _pNetwork->MyCharacterInfo.index)
	{
		pInfo->GetMyPetInfo()->Init();
	}

	if (m_pEntity != NULL)
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(m_pEntity);

	if (m_pEntity != NULL) 
	{
		m_pEntity->en_pPetTarget = NULL;
		
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

	pet_statusEffect.Reset();
}

CPetTarget::CPetTarget(const CPetTarget &other)
{
	m_eType = eOBJ_PET;
	m_nIdxServer	= other.m_nIdxServer;
	m_nType	= other.m_nType;
	pet_iAge	= other.pet_iAge;
	m_nIdxClient = other.m_nIdxClient;	
	m_pEntity = other.m_pEntity;
	m_strName	= other.m_strName;
	pet_OwnerName	= other.pet_OwnerName;
	m_yLayer = other.m_yLayer;
	//Hardcoding, status effect에 vtable이 추가되거나 상속이 되는 경우 문제 생김.
	memcpy(&pet_statusEffect, &other.pet_statusEffect, sizeof(pet_statusEffect));
	memset((void*)&other.pet_statusEffect, 0, sizeof(other.pet_statusEffect));
	pet_pNormalEffect = other.pet_pNormalEffect;
	pet_sbAttributePos	= other.pet_sbAttributePos;
	pet_OwnerIndex = other.pet_OwnerIndex;
}

CPetTarget &CPetTarget::operator=(const CPetTarget &other)
{
	m_eType = other.m_eType;
	m_nIdxServer = other.m_nIdxServer;
	m_nType = other.m_nType;
	pet_iAge	= other.pet_iAge;
	m_nIdxClient = other.m_nIdxClient;	
	m_pEntity = other.m_pEntity;
	m_strName = other.m_strName;
	pet_OwnerName = other.pet_OwnerName;
	m_yLayer = other.m_yLayer;
	//Hardcoding, status effect에 vtable이 추가되가나 상속이 되는 경우 문제 생김.
	memcpy(&pet_statusEffect, &other.pet_statusEffect, sizeof(pet_statusEffect));
	memset((void*)&other.pet_statusEffect, 0, sizeof(other.pet_statusEffect));
	pet_pNormalEffect = other.pet_pNormalEffect;
	pet_sbAttributePos	= other.pet_sbAttributePos;
	pet_OwnerIndex = other.pet_OwnerIndex;
	pet_strNameCard =other.pet_strNameCard;
	return *this;
}

//강동민 수정 끝 싱글 던젼 작업		07.27
#include <Engine/Network/Server.h>	// TEST
void CPetTarget::SetData( INDEX index, CTString& strName, CTString& strOwnerName, INDEX iOwnerIndex, INDEX iPetType, INDEX iPetAge, CEntity *pEntity, SBYTE sbyLayer, CTString strNameCard)
{
	m_nIdxServer		= index;
	m_nType		= iPetType;    
	pet_iAge		= iPetAge;
	m_strName		= strName;	
	pet_OwnerName	= strOwnerName;	
	m_pEntity		= pEntity;
	m_yLayer		= sbyLayer;
	pet_OwnerIndex	= iOwnerIndex;
	pet_strNameCard = strNameCard;
}

void CPetTarget::SetClientPetId(INDEX index)
{	
	m_nIdxClient = index;	
}

CWildPetTarget::CWildPetTarget()
{
	m_eType = eOBJ_WILDPET;

	int		i;
	bIsActive = FALSE;
	bDeath = FALSE;
	m_nLevel = -1;
	m_nType	= -1;
	m_strName = "";
	m_nStr	= -1;
	m_nCon	= -1;
	m_nDex	= -1;
	m_nInt	= -1;
	m_nHP	= -1;
	m_nMaxHP	= -1;
	m_nMP	= -1;
	m_nMaxMP	= -1;
	m_nFaith	= -1;
	m_nStm		= -1;
	m_nMaxFaith	= -1;
	m_nMaxStm	= -1;
	m_nSpeed	= -1;
	m_nAISlot	= -1;
	m_nIdxServer = -1;
	m_nIdxClient = -1;
	m_pEntity = NULL;
	for( i = 0; i < STATE_END; i++)
	{m_nWildPetState[i] = -1;}

	for( i = 0; i < WILDPET_WEAR_TOTAL; i++)
	{
		m_nPetWearIndex[i] = -1;
		m_nPetWearPlus[i] =  -1;
	}
	m_sbTransStat = 0;
	m_bMount = FALSE;
}
CWildPetTarget::~CWildPetTarget()
{
	CUIManager* pUIMgr = CUIManager::getSingleton();
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (m_nOwnerIndex == _pNetwork->MyCharacterInfo.index)
	{
		pUIMgr->RearrangeOrder(UI_WILDPETTARGETINFO,FALSE);
		pInfo->SetMyApet(NULL);
		pUIMgr->GetWildPetInfo()->AIClear();
	}

	pUIMgr->StopTargetEffect(m_nIdxServer);

	if (m_pEntity != NULL) 
	{
		m_pEntity->en_pWildPetTarget = NULL;
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
}


CWildPetTarget::CWildPetTarget(const CWildPetTarget &other)
{
	(*this) = other;
}

CWildPetTarget &CWildPetTarget::operator=(const CWildPetTarget &other)
{
	m_eType = other.m_eType;
	bIsActive = other.bIsActive;
	m_nIndex = other.m_nIndex; // 펫 툴 인덱스
	m_nIdxServer = other.m_nIdxServer;
	m_nOwnerIndex = other.m_nOwnerIndex;
	m_nLevel = other.m_nLevel; // 공격 펫 레벨
	m_nLevelupPoint = other.m_nLevelupPoint; // 공격 펫 성장포인트
	m_nType = other.m_nType; // 공격 펫 타입
	m_strName = other.m_strName; // 공격 펫 이름
	// Bagic
	m_nStr = other.m_nStr; // 공격 펫 힘
	m_nStrPlus = other.m_nStrPlus; 	//	공격 펫 힘 추가
	m_nCon = other.m_nCon; // 공격 펫 체력
	m_nConPlus = other.m_nConPlus;		// 공격 펫 체력 추가	
	m_nDex = other.m_nDex;			// 공격 펫 민첩
	m_nDexPlus = other.m_nDexPlus;		// 공격 펫 민첩 추가
	m_nInt = other.m_nInt;			// 공력 펫 지혜
	m_nIntPlus = other.m_nIntPlus;		// 공격 펫 지혜 추가
	m_exp = other.m_exp;			// 공격 펫 경험치
	m_next_exp = other.m_next_exp;		// 공격 펫 맥스(레벨업) 경험치
	m_nHP = other.m_nHP;			// 공격 펫 HP
	m_nMP = other.m_nMP;			// 공격 펫 MP
	m_nMaxHP = other.m_nMaxHP;		// 공격 펫 Max HP
	m_nMaxMP = other.m_nMaxMP;		// 공격 펫 Max MP
	m_nFaith = other.m_nFaith;		// 공격 펫 충성심
	m_nStm = other.m_nStm;			// 공격 펫 배고픔
	m_nMaxFaith = other.m_nMaxFaith;	// 공격 펫 최대 충성심
	m_nMaxStm = other.m_nMaxStm;		// 공격 펫 최대 배고픔
	m_nSpeed = other.m_nSpeed;		// 공격 펫 이동 속도
	m_nAISlot = other.m_nAISlot;		// 공격 펫 AI 슬롯개수
	m_nIdxClient = other.m_nIdxClient; //공격 펫 Client World Index
	m_pEntity = other.m_pEntity;
	m_yLayer = other.m_yLayer;
	m_sbAttributePos = other.m_sbAttributePos;
	// state
	INDEX i;

	for (i=0; i<STATE_END; ++i)
	{
		m_nWildPetState[i] = other.m_nWildPetState[i];
		m_nWildPetStatePlus[i] = other.m_nWildPetStatePlus[i];
	}

	for (i=0; i<WILDPET_WEAR_TOTAL; ++i)
	{
		m_nPetWearIndex[i] = other.m_nPetWearIndex[i];
		m_nPetWearPlus[i] = other.m_nPetWearPlus[i];
	}

	m_sbTransStat = other.m_sbTransStat;
	m_bMount = other.m_bMount;

	return (*this);
}

void CWildPetTarget::SetWildPetToEntity(CEntity *pEntity, INDEX nindex, INDEX *nWearIndex, INDEX *nWearPlus)
{
	CEntity* tmpOwner = NULL;

	for(int i = 0; i < WILDPET_WEAR_TOTAL; i++)
	{
		m_nPetWearIndex[i] = nWearIndex[i];
		m_nPetWearPlus[i] =  nWearPlus[i];
	}
	CWildPetData* pt = CWildPetData::getData(nindex);
	m_pEntity = pEntity;
	
	CEntityProperty &eppropertyWalk	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 100));
	ENTITYPROPERTY(&*pEntity, eppropertyWalk.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(pt->strWalk[m_sbTransStat]);

	CEntityProperty &eppropertyIdle1 = *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 101));
	ENTITYPROPERTY(&*pEntity, eppropertyIdle1.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(pt->strIdle1[m_sbTransStat]);

	CEntityProperty &eppropertyIdle2	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 102));
	ENTITYPROPERTY(&*pEntity, eppropertyIdle2.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(pt->strIdle2[m_sbTransStat]);

	CEntityProperty &eppropertyRun	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 103));
	ENTITYPROPERTY(&*pEntity, eppropertyRun.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(pt->strRun[m_sbTransStat]);

	CEntityProperty &eppropertyDeath	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 104));
	ENTITYPROPERTY(&*pEntity, eppropertyDeath.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(pt->strDie[m_sbTransStat]);

	CEntityProperty &eppropertyDamage	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 105));
	ENTITYPROPERTY(&*pEntity, eppropertyDamage.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(pt->strDamage[m_sbTransStat]);

	CEntityProperty &eppropertyAttack1	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 106));
	ENTITYPROPERTY(&*pEntity, eppropertyAttack1.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(pt->strAttack1[m_sbTransStat]);

	CEntityProperty &eppropertyAttack2	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 107));
	ENTITYPROPERTY(&*pEntity, eppropertyAttack2.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(pt->strAttack2[m_sbTransStat]);

	CEntityProperty &eppropertyLevelup	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 109)); // 배고픔
	ENTITYPROPERTY(&*pEntity, eppropertyLevelup.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(pt->strLevelup[m_sbTransStat]);	

	CEntityProperty &eppropertysbAttributePos	= *(pEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 115));
	ENTITYPROPERTY(&*pEntity, eppropertysbAttributePos.ep_slOffset, INDEX)	= m_sbAttributePos;

	CDLLEntityClass *pdecDLLBaseClass	= pEntity->GetClass()->ec_pdecDLLClass->dec_pdecBase;
	CDLLEntityClass *pdecDLLBaseBaseClass	= pdecDLLBaseClass->dec_pdecBase;

	BOOL bAI = FALSE;
	if(m_nOwnerIndex == _pNetwork->MyCharacterInfo.index)
	{
		bAI = TRUE;
		tmpOwner = CEntity::GetPlayerEntity(0);
	}
	else
	{
		tmpOwner = _pNetwork->GetEntityByTypeIndex(0, m_nOwnerIndex);
	}

	CEntityProperty &epPropertyEntity	= *pdecDLLBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_ENTITYPTR, 1); //8);	// Owner
	ENTITYPROPERTY( &*pEntity, epPropertyEntity.ep_slOffset, CEntityPointer)		= tmpOwner;

	CEntityProperty &epPropertyUseAI	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 193);	// AI 사용 유무.
	ENTITYPROPERTY( &*pEntity, epPropertyUseAI.ep_slOffset, INDEX)		= bAI;

	// 이동 속도...
	CEntityProperty &epPropertyWalkSpeed		= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 10);	// NPC Walk Speed
	ENTITYPROPERTY( &*pEntity, epPropertyWalkSpeed.ep_slOffset, FLOAT)			= 3.5f;

	CEntityProperty &epPropertyAttackRunSpeed	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 12);	// NPC Attack Run Speed
	ENTITYPROPERTY( &*pEntity, epPropertyAttackRunSpeed.ep_slOffset, FLOAT)		= 7.0f;
	CEntityProperty &epPropertyCloseRunSpeed	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 14);	// NPC Close Run Speed
	ENTITYPROPERTY( &*pEntity, epPropertyCloseRunSpeed.ep_slOffset, FLOAT)		= 7.0f;

		// 공격 거리...
	CEntityProperty &epPropertyAttackDistance	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 20);	// Attack Distance
	ENTITYPROPERTY( &*pEntity, epPropertyAttackDistance.ep_slOffset, FLOAT)		= 4.0f;
	
	CEntityProperty &epPropertyStopDistance		= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 24);	// Attack Distance
	ENTITYPROPERTY( &*pEntity, epPropertyStopDistance.ep_slOffset, FLOAT)		= 3.0f;

	CEntityProperty &epPropertyCloseDistance	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 21);	// Attack Distance
	ENTITYPROPERTY( &*pEntity, epPropertyCloseDistance.ep_slOffset, FLOAT)		= 4.1f;
}

















