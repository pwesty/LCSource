#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Network/CharacterTarget.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Network/CNetwork.h>
#include <Engine/Entities/InternalClasses.h>
//#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UIGuildBattle.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Info/MyInfo.h>


#define		PLAYER_STATE_PKMODE			(1 << 3)	// PK 모드
#define		PLAYER_STATE_PKMODEDELAY	(1 << 4)	// PK 모드 해제 딜레이
/*
*  Constructor.
*/
CCharacterTarget::CCharacterTarget(void)
	: pGuildmark(NULL)
{
	m_eType = eOBJ_CHARACTER;

	m_nIdxServer = -1;
	m_nIdxClient = -1;
	
	m_nType = -1;                
	cha_iJob2 = 0;	
	cha_yLayer = 0;
	cha_BuffCount = 0;
	m_pEntity = NULL;
//	cha_pPet	= NULL;

//	cha_pFirstSummon	= NULL;
//	cha_pSecondSummon	= NULL;
	cha_state = 0;
	cha_pkstate = 0;
	cha_sbGuildRank = 0; 
	ChatMsg.Reset();
	ShopMsg.Reset();
	cha_bLegit = FALSE;	
	
	cha_lGuildIndex	= -1;
	cha_lGuildLevel	= -1;
	cha_lGuildPosition	= -1;
	cha_strGuildName	= CTString("");
	cha_ubGuildNameColor =0;				//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	cha_nChangeMobId    = -1;
	cha_nTransformIndex	= 0;
	cha_sbItemEffectOption = 0;
	cha_bExtension		= FALSE;
	cha_statusEffect.SetType(CStatusEffect::T_CHARACTER);
	cha_sbJoinFlagMerac		= WCJF_NONE;
	cha_sbJoinFlagDratan	= WCJF_NONE;		// WSS_DRATAN_SIEGEWARFARE 070720	
	cha_sbAttributePos = MATT_UNWALKABLE;
	cha_iPetType	= -1;
	cha_iPetAge		= -1;
	cha_bPetRide	= FALSE;
	cha_Label = -1; // [sora] 타겟 표식
	cha_NickType	= 0;		// 호칭 인덱스

	int i;
	for (i = 0; i < WEAR_COSTUME; ++i)
	{
		cha_CosWear[i] = -1;
		cha_itemPlus[i] = -1;
	}

	for (i = 0; i < WEAR_ACCESSORY_MAX; ++i)
	{
		cha_itemAccessory[i] = -1;
	}

	cha_iSyndicateType = 0;
	cha_iSyndicateGrade = 0;
	cha_iHitType = 0;
}

/*
*  Destructor.
*/
CCharacterTarget::~CCharacterTarget(void) 
{
	_pUISWDoc->StopEffect( m_nIdxServer );
	_pUISWDoc->StopGuildMasterEffect( m_nIdxServer );

	CUIManager* pUIMgr = CUIManager::getSingleton();
	ObjInfo* pInfo = ObjInfo::getSingleton();
	
	// 해당 캐릭터의 Index 바탕으로 Guild Effect를 삭제해줌
	pUIMgr->GetGuildBattle()->StopGuildEffect ( m_nIdxServer );
	// 해당 캐릭터의 타겟 이펙트를 없앰...
	pUIMgr->StopTargetEffect( m_nIdxServer );
	// WSS_DRATAN_SIEGEWARFARE 2007/10/14
	_pUISWDoc->StopConsensusEffect(m_nIdxServer); // WSS_DRATAN_SIEGEWARFARE 2007/10/14		

	cha_itemEffect.DelAccessoryEffect();	// 서포터 이펙트 등, 악세사리 이펙트 삭제
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(m_pEntity);
	cha_itemEffect.DeleteNickEffect();	// 호칭 이펙트 삭제
	cha_itemEffect.DeletePetStashEffect(); // 펫창고 이펙트 삭제

	for (int i = 0; i < cha_BuffCount; ++i)
	{
		cha_Buff[i].Destroy_pEG();
	}

	if (m_pEntity != NULL)
	{
		m_pEntity->en_pCharacterTarget = NULL;
		if (m_pEntity == pInfo->GetTargetEntity(eTARGET_REAL))
			pInfo->TargetClear(eTARGET_REAL);

		if (m_pEntity == pInfo->GetTargetEntity(eTARGET))
			pInfo->TargetClear(eTARGET);

		m_pEntity->Destroy( FALSE );
		m_pEntity = NULL;
	}

	SAFE_DELETE(pGuildmark);
}

CCharacterTarget::CCharacterTarget(const CCharacterTarget &other)
{
	m_eType = other.m_eType;
	m_nIdxServer = other.m_nIdxServer;
	m_nType = other.m_nType;
	cha_iJob2 = other.cha_iJob2;
	m_nIdxClient = other.m_nIdxClient;
	m_pEntity = other.m_pEntity;
	m_strName = other.m_strName;
	cha_pkstate = other.cha_pkstate;
	cha_state = other.cha_state;
	cha_sbShopType = other.cha_sbShopType;
	cha_yLayer = other.cha_yLayer;
	cha_BuffCount = other.cha_BuffCount;
	cha_lGuildIndex = other.cha_lGuildIndex;
	cha_lGuildLevel = other.cha_lGuildLevel;
	cha_lGuildPosition = other.cha_lGuildPosition;
	cha_strGuildName = other.cha_strGuildName;
	cha_ubGuildNameColor =other.cha_ubGuildNameColor;	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	cha_bExtension = other.cha_bExtension;
	memcpy(cha_Buff, other.cha_Buff, sizeof(cha_Buff[0])*BUFF_MAX_COUNT);
	ChatMsg = other.ChatMsg;
	ShopMsg = other.ShopMsg;
	cha_bLegit = other.cha_bLegit;	
	cha_nChangeMobId	= other.cha_nChangeMobId;
	cha_nTransformIndex	= other.cha_nTransformIndex;
	cha_sbItemEffectOption = other.cha_sbItemEffectOption;
	cha_sbJoinFlagMerac	= other.cha_sbJoinFlagMerac;
	cha_sbJoinFlagDratan= other.cha_sbJoinFlagDratan;	// WSS_DRATAN_SIEGEWARFARE 070720	
	cha_sbAttributePos = other.cha_sbAttributePos;
	cha_iPetType	= other.cha_iPetType;
	cha_iPetAge		= other.cha_iPetAge;
	cha_bPetRide	= other.cha_bPetRide;
	//Hardcoding, status나 item effect에 vtable이 추가되거나 상속이 되는 경우 문제 생김.
	cha_statusEffect.Reset();
	cha_itemEffect.Clear();
	memcpy(&cha_statusEffect, &other.cha_statusEffect, sizeof(cha_statusEffect));
	memset((void*)&other.cha_statusEffect, 0, sizeof(other.cha_statusEffect));
	memcpy(&cha_itemEffect, &other.cha_itemEffect, sizeof(cha_itemEffect));
	memset((void*)&other.cha_itemEffect, 0, sizeof(other.cha_itemEffect));
	cha_statusEffect.SetType(CStatusEffect::T_CHARACTER);
	cha_Label = other.cha_Label; // [sora] 타겟 표식
	cha_NickType = other.cha_NickType;

	int i;
	for (i = 0; i < WEAR_COSTUME_TOTAL; ++i)
	{
		cha_CosWear[i] = other.cha_CosWear[i];
		cha_itemPlus[i] = other.cha_itemPlus[i];
	}

	for (i = 0; i < WEAR_ACCESSORY_MAX; ++i)
	{
		cha_itemAccessory[i] = other.cha_itemAccessory[i];
	}

	if (other.pGuildmark != NULL)
		pGuildmark = (CUIGuildMarkIcon*)other.pGuildmark->Clone();
	
	SetSyndicate( other.cha_iSyndicateType, other.cha_iSyndicateGrade );
	cha_iHitType = other.cha_iHitType;
}

CCharacterTarget &CCharacterTarget::operator=(const CCharacterTarget &other)
{
	m_eType = other.m_eType;
	m_nIdxServer = other.m_nIdxServer;
	m_nType = other.m_nType;
	cha_iJob2 = other.cha_iJob2;
	m_nIdxClient = other.m_nIdxClient;
	m_pEntity = other.m_pEntity;
//	cha_pPet	= other.cha_pPet;
	//cha_pFirstSummon	= other.cha_pFirstSummon;
	//cha_pSecondSummon	= other.cha_pSecondSummon;
	m_strName = other.m_strName;
	cha_pkstate = other.cha_pkstate;
	cha_state = other.cha_state;
	cha_sbShopType = other.cha_sbShopType;
	//cha_sbNameLength = other.cha_sbNameLength;
	cha_yLayer = other.cha_yLayer;
	cha_BuffCount = other.cha_BuffCount;
	cha_lGuildIndex = other.cha_lGuildIndex;
	cha_lGuildLevel = other.cha_lGuildLevel;
	cha_lGuildPosition = other.cha_lGuildPosition;
	cha_strGuildName = other.cha_strGuildName;
	cha_ubGuildNameColor =other.cha_ubGuildNameColor;	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	cha_bExtension = other.cha_bExtension;
	memcpy(cha_Buff, other.cha_Buff, sizeof(cha_Buff[0])*BUFF_MAX_COUNT);
	ChatMsg = other.ChatMsg;
	ShopMsg = other.ShopMsg;
	cha_bLegit = other.cha_bLegit;	
	cha_nChangeMobId = other.cha_nChangeMobId;
	cha_nTransformIndex	= other.cha_nTransformIndex;
	cha_sbItemEffectOption = other.cha_sbItemEffectOption;
	cha_sbJoinFlagMerac = other.cha_sbJoinFlagMerac;
	cha_sbJoinFlagDratan = other.cha_sbJoinFlagDratan;	// WSS_DRATAN_SIEGEWARFARE 070720	
	cha_sbAttributePos = other.cha_sbAttributePos;
	cha_iPetType	= other.cha_iPetType;
	cha_iPetAge		= other.cha_iPetAge;
	cha_bPetRide	= other.cha_bPetRide;
	//Hardcoding, status나 item effect에 vtable이 추가되거나 상속이 되는 경우 문제 생김.
	cha_statusEffect.Reset();
	cha_itemEffect.Clear();
	memcpy(&cha_statusEffect, &other.cha_statusEffect, sizeof(cha_statusEffect));
	memset((void*)&other.cha_statusEffect, 0, sizeof(other.cha_statusEffect));
	memcpy(&cha_itemEffect, &other.cha_itemEffect, sizeof(cha_itemEffect));
	memset((void*)&other.cha_itemEffect, 0, sizeof(other.cha_itemEffect));
	cha_statusEffect.SetType(CStatusEffect::T_CHARACTER);
	cha_Label = other.cha_Label; // [sora] 타겟 표식
	cha_NickType = other.cha_NickType;

	int i;
	for (i = 0; i < WEAR_COSTUME_TOTAL; ++i)
	{
		cha_CosWear[i] = other.cha_CosWear[i];
		cha_itemPlus[i] = other.cha_itemPlus[i];
	}

	for (i = 0; i < WEAR_ACCESSORY_MAX; ++i)
	{
		cha_itemAccessory[i] = other.cha_itemAccessory[i];
	}

	if (other.pGuildmark != NULL)
		pGuildmark = (CUIGuildMarkIcon*)other.pGuildmark->Clone();

	SetSyndicate( other.cha_iSyndicateType, other.cha_iSyndicateGrade );
	cha_iHitType = other.cha_iHitType;
	return *this;
}

void CCharacterTarget::SetData( INDEX index, INDEX type, CTString &name, CEntity *pEntity, SBYTE sbyLayer,  UWORD state, SLONG pkstate)
{	
	m_nIdxServer = index;
	m_nType = type;         
	m_strName = name;
	cha_yLayer = sbyLayer;
	m_pEntity = pEntity;
	cha_state	= state; 
	cha_pkstate = pkstate;
	cha_statusEffect.SetType(CStatusEffect::T_CHARACTER);
}

void CCharacterTarget::SetClientChaId(INDEX index)
{	
	m_nIdxClient = index;
}

void CCharacterTarget::RemoveBuff( SLONG slItemIndex, SLONG slSkillIndex )
{
	int iBuff;
	for( iBuff = 0; iBuff < cha_BuffCount; iBuff++ )
	{
		if( cha_Buff[iBuff].m_slItemIndex == slItemIndex &&
			cha_Buff[iBuff].m_slSkillIndex == slSkillIndex )
			break;
	}

	ASSERT( iBuff < cha_BuffCount );
	if( iBuff == cha_BuffCount )
		return;

	cha_Buff[iBuff].Destroy_pEG();

	for( iBuff++; iBuff < cha_BuffCount; iBuff++ )
	{
		cha_Buff[iBuff - 1] = cha_Buff[iBuff];
	}

	cha_BuffCount--;
	cha_Buff[cha_BuffCount].Init();
}

int CCharacterTarget::GetPkState()
{
	if((cha_state & PLAYER_STATE_PKMODE) && (cha_state & PLAYER_STATE_PKMODEDELAY))
	{
		return	CHA_PVP_STATE_RELEASE;	
	}
	else if(cha_state & PLAYER_STATE_PKMODE)
	{
		return CHA_PVP_STATE_PK;
	}
	else
	{
		return CHA_PVP_STATE_PEACE;
	}
}

void CCharacterTarget::SetShopType( SBYTE sbType)
{
	cha_sbShopType = sbType;
}


void CCharacterTarget::SetChaState(SLONG state)
{
	cha_state = state;
}

void CCharacterTarget::SetChaPkState(SLONG pkstate)
{
	cha_pkstate = pkstate;
}

// wooss 070310 --------------------------------------------------------------->>
// kw : WSS_WHITEDAY_2007
// ----------------------------------------------------------------------------
// Name : IsBuff() wooss 050803
// Desc : find some buff in buff array
// ----------------------------------------------------------------------------
BOOL CCharacterTarget::IsBuff( SLONG itemIndex )
{
	for(int i=0; i<BUFF_MAX_COUNT;i++)
		if ((cha_Buff[i].m_slItemIndex)==itemIndex) return TRUE;
	return FALSE;	
}

CEffectGroup* CCharacterTarget::IsBuffBySkill( SLONG skillIndex )
{
	for(int i=0; i<BUFF_MAX_COUNT;i++)
	{
		if ((cha_Buff[i].m_slSkillIndex)==skillIndex) 
		{
			return cha_Buff[i].m_pEG;
		}
	}
	return NULL;	
}

void CCharacterTarget::SetSyndicate( INDEX iType, INDEX iGrade)
{
	cha_iSyndicateType = iType;
	cha_iSyndicateGrade = iGrade;
}

int CCharacterTarget::GetShowAnotherRelicEffect()
{
	int nShowEffect = -1;
	int nTemp = -1;

	for (int i = 0; i < WEAR_ACCESSORY_MAX; ++i)
	{
		if (cha_itemAccessory[i] <= -1)
			continue;

		if (cha_itemAccessory[i] == 10953)
			nTemp = 2;
		else if (cha_itemAccessory[i] == 10952)
			nTemp = 1;
		else if (cha_itemAccessory[i] == 10951)
			nTemp = 0;

		if (nTemp > nShowEffect)
			nShowEffect = nTemp;
	}

	return nShowEffect;
}

// ----------------------------------------------------------------------------<<
