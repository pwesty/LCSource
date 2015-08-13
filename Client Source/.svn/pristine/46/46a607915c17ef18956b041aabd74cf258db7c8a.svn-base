#include "StdH.h"

#include "ItemEffect.h"

#include <Engine/Effect/CEffectGroup.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Base/Timer.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Info/MyInfo.h>

extern UINT g_uiEngineVersion;

enum ePetStashEffect
{
	PET_STASH_GHOST = 0,
	PET_STASH_PHOENIX,
	PET_STASH_SEAHORSE,
	PET_STASH_DRAGON,
	PET_STASH_CUPID,
	PET_STASH_GENIE,
	PET_STASH_MAX
};

enum eRelicItemType
{
	eRELIC_10951 = 0, // 천사의 유물
	eRELIC_10952, // 대 천사의 유물
	eRELIC_10953, // 세라핌의 유물
	eRELIC_MAX
};

const CTString g_strPetStashEffectName[PET_STASH_MAX] = 
{
	"PT_ghost_pet", "PT_phoenix_pet", "PT_seahorse_pet", "PT_dragon_pet", "PT_cupid_pet", "PT_genie_pet"
};

const CTString g_strLoginEffectName = "LoginBase";

const CTString g_strRelicEffectName[eRELIC_MAX] = 
{
	"Angel_wing01", "Angel_wing02", "Angel_wing03"
};

CItemEffect::CItemEffect() 
	: m_pAccessroyEffect(NULL)
	, m_ItemERType(ER_NORMAL)
	, m_ItemERSubType(ERS_NORMAL)
	, m_nHolyWaterEffectType(0)
	, m_nCurRelicEffectType(-1)
{
	int i;
	for (i = 0; i < 2; ++i)
	{
		m_PlusEffectGourp[i].m_pEffectGroup = NULL;
		m_pHolyWaterEffectGroup[i] = NULL;
	}

	for (i = 0; i < WEAR_TOTAL; ++i)
	{
		m_ItemEffectGroup[i].m_pEffectGroup = NULL;
	}

	m_NickEffectGroup.m_pEffectGroup = NULL;
	m_PetStashEffectGroup.m_pEffectGroup = NULL;
	m_RelicEffectGroup.m_pEffectGroup = NULL;
}

CItemEffect::~CItemEffect()
{
	Clear();
}

CItemEffect::CItemEffect(const CItemEffect &other)
{
	m_PlusEffectGourp[0] = other.m_PlusEffectGourp[0];
	m_PlusEffectGourp[1] = other.m_PlusEffectGourp[1];

	for (int i=0; i<WEAR_TOTAL; i++)
	{
		m_ItemEffectGroup[i] = other.m_ItemEffectGroup[i];
	}

	m_NickEffectGroup = other.m_NickEffectGroup;
	m_PetStashEffectGroup = other.m_PetStashEffectGroup;
	m_RelicEffectGroup = other.m_RelicEffectGroup;
	
	memcpy(m_aItemPlus, other.m_aItemPlus, sizeof(int)*WEAR_TOTAL);
}

CItemEffect &CItemEffect::operator=(const CItemEffect &other)
{
	m_PlusEffectGourp[0] = other.m_PlusEffectGourp[0];
	m_PlusEffectGourp[1] = other.m_PlusEffectGourp[1];

	for (int i=0; i<WEAR_TOTAL; i++)
	{
		m_ItemEffectGroup[i] = other.m_ItemEffectGroup[i];
	}

	m_NickEffectGroup = other.m_NickEffectGroup;
	m_PetStashEffectGroup = other.m_PetStashEffectGroup;
	m_RelicEffectGroup = other.m_RelicEffectGroup;
	
	memcpy(m_aItemPlus, other.m_aItemPlus, sizeof(int)*WEAR_TOTAL);
	return *this;
}

void CItemEffect::Clear()
{
	memset(m_PlusEffectGourp[0].szEffectName, 0, sizeof(m_PlusEffectGourp[0].szEffectName));
	memset(m_PlusEffectGourp[1].szEffectName, 0, sizeof(m_PlusEffectGourp[1].szEffectName));
	DestroyEffectGroupIfValid(m_PlusEffectGourp[0].m_pEffectGroup);
	DestroyEffectGroupIfValid(m_PlusEffectGourp[1].m_pEffectGroup);

	for (int i = 0; i < WEAR_TOTAL; ++i)
	{
		m_aItemPlus[i] = -1;
		memset(m_ItemEffectGroup[i].szEffectName, 0, sizeof(m_ItemEffectGroup[i].szEffectName));
		DestroyEffectGroupIfValid(m_ItemEffectGroup[i].m_pEffectGroup);
	}

	memset(m_NickEffectGroup.szEffectName, 0, sizeof(m_NickEffectGroup.szEffectName));
	memset(m_PetStashEffectGroup.szEffectName, 0, sizeof(m_PetStashEffectGroup.szEffectName));
	memset(m_RelicEffectGroup.szEffectName, 0, sizeof(m_RelicEffectGroup.szEffectName));

	DestroyEffectGroupIfValid(m_NickEffectGroup.m_pEffectGroup);
	DestroyEffectGroupIfValid(m_PetStashEffectGroup.m_pEffectGroup);
	DestroyEffectGroupIfValid(m_pHolyWaterEffectGroup[0]);
	DestroyEffectGroupIfValid(m_pHolyWaterEffectGroup[1]);
	DestroyEffectGroupIfValid(m_RelicEffectGroup.m_pEffectGroup);

	m_nHolyWaterEffectType = 0;
	m_nCurRelicEffectType = -1;
}

void CItemEffect::Refresh(CTagManager *pTM, SBYTE sbOption)
{
	AddHolyWaterEffect(m_nHolyWaterEffectType, pTM);
	RefreshPetStashEffect(pTM);
	RefreshRelicEffect(pTM);

	if( !(sbOption & 0x01) )
		pTM = NULL;

	DestroyEffectGroupIfValid(m_PlusEffectGourp[0].m_pEffectGroup);
	DestroyEffectGroupIfValid(m_PlusEffectGourp[1].m_pEffectGroup);

	if (m_PlusEffectGourp[0].szEffectName[0] != 0)
		m_PlusEffectGourp[0].m_pEffectGroup = StartEffectGroup(m_PlusEffectGourp[0].szEffectName, pTM, _pTimer->GetLerpedCurrentTick(), m_ItemERType, m_ItemERSubType);

	if (m_PlusEffectGourp[1].szEffectName[0] != 0)
		m_PlusEffectGourp[1].m_pEffectGroup = StartEffectGroup(m_PlusEffectGourp[1].szEffectName, pTM, _pTimer->GetLerpedCurrentTick(), m_ItemERType, m_ItemERSubType);
	
	RefreshAccessoryEffect(pTM);

	for (int i=0; i<WEAR_TOTAL; i++)
	{
		DestroyEffectGroupIfValid(m_ItemEffectGroup[i].m_pEffectGroup);

		if (m_ItemEffectGroup[i].szEffectName[0] != 0)
		{
			m_ItemEffectGroup[i].m_pEffectGroup = StartEffectGroup(m_ItemEffectGroup[i].szEffectName,
																	pTM, _pTimer->GetLerpedCurrentTick(), m_ItemERType, m_ItemERSubType);
		}
	}

	DestroyEffectGroupIfValid(m_NickEffectGroup.m_pEffectGroup);

	if (m_NickEffectGroup.szEffectName[0] != 0)
	{
		m_NickEffectGroup.m_pEffectGroup = StartEffectGroup(m_NickEffectGroup.szEffectName, pTM, _pTimer->GetLerpedCurrentTick(), m_ItemERType, m_ItemERSubType);
	}
}

void CItemEffect::DeleteEffect(const int wearPos, BOOL bClear)
{
	if (wearPos < WEAR_HELMET || wearPos > WEAR_TOTAL) return;

	if (bClear)
	{
		if (m_ItemEffectGroup[wearPos].szEffectName[0] != NULL)
		{
			memset(m_ItemEffectGroup[wearPos].szEffectName, 0, sizeof(m_ItemEffectGroup[wearPos].szEffectName));
		}
	}

	DestroyEffectGroupIfValid(m_ItemEffectGroup[wearPos].m_pEffectGroup);
}

//=========================================================================================
// CItemEffect::AddAccessoryEffect()
// Explain: 특정 악세사리 착용시 Effect적용( case MSG_EX_PLAYER_STATE_CHANGE: )
//=========================================================================================
BOOL CItemEffect::AddAccessoryEffect(const char* strEffectName, CTagManager *pTM)
{
	if ( !strEffectName || !pTM ) return FALSE;

	// Refresh
	DelAccessoryEffect(); // reset
	strcpy(m_szAccessoryEffectName, strEffectName); // add str
	m_pAccessroyEffect = StartEffectGroup(m_szAccessoryEffectName, pTM, _pTimer->GetLerpedCurrentTick()); // add effect

	return TRUE;
}
//=========================================================================================
// CItemEffect::RefreshAccessoryEffect()
// Explain: 특정 악세사리 착용시 적용된 Effect 재설정(모델이 변경사항이 있을 시 Effect가 떨어지지않게 재설정)
//=========================================================================================
void CItemEffect::RefreshAccessoryEffect(CTagManager *pTM)
{
	DestroyEffectGroupIfValid(m_pAccessroyEffect);

	if (m_szAccessoryEffectName != 0)
		m_pAccessroyEffect = StartEffectGroup(m_szAccessoryEffectName, pTM, _pTimer->GetLerpedCurrentTick(), m_ItemERType, m_ItemERSubType); // add effect
}

BOOL CItemEffect::AddWearEffect(const char* strEffectName, const int wearPos, CTagManager *pTM)
{
	if ( !strEffectName || !pTM || wearPos < WEAR_HELMET || wearPos > WEAR_TOTAL)
		return FALSE;

	DelWearEffect(wearPos); // reset
	strcpy(m_ItemEffectGroup[wearPos].szEffectName, strEffectName); // add EffectName
	m_ItemEffectGroup[wearPos].m_pEffectGroup = StartEffectGroup(m_ItemEffectGroup[wearPos].szEffectName,
																	pTM, _pTimer->GetLerpedCurrentTick()); // add effectgroup

	return TRUE;
}

BOOL CItemEffect::DelWearEffect(const int wearPos)
{
	if (wearPos < WEAR_HELMET || wearPos > WEAR_TOTAL)
		return FALSE;

	memset(m_ItemEffectGroup[wearPos].szEffectName, 0, sizeof(m_ItemEffectGroup[wearPos].szEffectName));
	DestroyEffectGroupIfValid(m_ItemEffectGroup[wearPos].m_pEffectGroup);
	return TRUE;
}

//=========================================================================================
// CItemEffect::DelAccessoryEffect()
// Explain: 특정 악세사리 탈착시 Effect제거( case MSG_EX_PLAYER_STATE_CHANGE: )
//=========================================================================================
BOOL CItemEffect::DelAccessoryEffect(void)
{
	if ( m_pAccessroyEffect )
	{ 
		memset(m_szAccessoryEffectName, 0, sizeof(m_szAccessoryEffectName));
		DestroyEffectGroupIfValid(m_pAccessroyEffect);
		return TRUE;
	}

	return FALSE;
}


//-----------------------------------------------------------------------------------------
// CItemEffect::Change
// Explain: 
// Histroy 
//	- Date : 2005-04-07,Author: Lee Ki-hwan
//			2차 무기 아이템 처리 
//			int nItemSubType : 2차 무기 판단 GetSubTypeWeapon ( int nItemSubType ) 에 쓰인다.
//		
//------------------------------------------------------------------------------------------
void CItemEffect::Change(const int job, CItemData* ItemData, const int wearPosNew, const int itemPlusNewIn, CTagManager *pTM, SBYTE sbOption, int nItemSubType )
{
	//초기조건 검사
	if(job < 0 || job > TOTAL_JOB) return;
	if(wearPosNew < 0 || wearPosNew > WEAR_TOTAL) return;
	if(pTM == NULL) return;

	BOOL bRefresh = FALSE;

	//아이템 정보갱신
	int itemPlusNew = itemPlusNewIn < 0 ? -1 : itemPlusNewIn;
	m_aItemPlus[wearPosNew] = itemPlusNew;

	//무기 아니면 갑옷?
	char *pStrName = NULL;
	CTString strType;

	if(wearPosNew == WEAR_WEAPON)
	{
		// Date : 2005-04-07(오후 3:16:08), By Lee Ki-hwan
		int nNumWeapon = GetSubTypeWeapon ( nItemSubType ); 
		
		strType = "wp";
	
		// Date : 2005-04-07(오후 2:57:21), By Lee Ki-hwan
		if ( nNumWeapon > 1 )
		{
			strType.PrintF( "%s%d", "wp", nNumWeapon );	
		}

		memset(m_ItemEffectGroup[wearPosNew].szEffectName, 0, sizeof(m_ItemEffectGroup[wearPosNew].szEffectName));

		if (strlen(ItemData->GetArmorEffectName()) > 0)
		{
			if (itemPlusNewIn >= 0)// itemPlusNewIn < 0이면 장비를 벗을때로 판단
			{
				strcpy(m_ItemEffectGroup[wearPosNew].szEffectName, ItemData->GetArmorEffectName());
			}
		}
		
		if (job != NIGHTSHADOW)
		{ // 나이트 쉐도우는 무기자체가 Effect이므로
			//pStrName = &m_ItemEffectGroup[wearPosNew].szEffectName[0];
			pStrName = &m_PlusEffectGourp[0].szEffectName[0];
		}
		bRefresh = TRUE;
	}
	else if(wearPosNew == WEAR_JACKET || wearPosNew == WEAR_PANTS
		|| wearPosNew == WEAR_BOOTS || wearPosNew == WEAR_GLOVES)
	{
		int itemPlusMin = -1;
#		define MIN(A, B) ((A) < (B) ? (A) : (B))
		itemPlusMin = MIN( MIN(m_aItemPlus[WEAR_JACKET], m_aItemPlus[WEAR_PANTS])
						 , MIN(m_aItemPlus[WEAR_BOOTS], m_aItemPlus[WEAR_GLOVES]) );
#		undef MIN
		itemPlusNew = itemPlusMin; 
		strType = "am";
		pStrName = &m_PlusEffectGourp[1].szEffectName[0];
		bRefresh = TRUE;

		if (strlen(ItemData->GetArmorEffectName()) > 0)
		{
			memset(m_ItemEffectGroup[wearPosNew].szEffectName, 0, sizeof(m_ItemEffectGroup[wearPosNew].szEffectName));
			
			if (itemPlusNewIn >= 0)// itemPlusNewIn < 0이면 장비를 벗을때로 판단
			{
				strcpy(m_ItemEffectGroup[wearPosNew].szEffectName, ItemData->GetArmorEffectName());
			}
		}
	}
	else if(wearPosNew == WEAR_SHIELD || wearPosNew == WEAR_HELMET || wearPosNew == WEAR_BACKWING)
	{	
		if (strlen(ItemData->GetArmorEffectName()) > 0)
		{
			memset(m_ItemEffectGroup[wearPosNew].szEffectName, 0, sizeof(m_ItemEffectGroup[wearPosNew].szEffectName));
			
			if (itemPlusNewIn >= 0)// itemPlusNewIn < 0이면 장비를 벗을때로 판단
			{
				strcpy(m_ItemEffectGroup[wearPosNew].szEffectName, ItemData->GetArmorEffectName());
			}
		}
		bRefresh = TRUE;
	}
	else if(wearPosNew >= WEAR_ACCESSORY1 && wearPosNew < WEAR_ACCESSORY4)
	{
		if (strlen(ItemData->GetArmorEffectName()) > 0 && itemPlusNew >= 0)
		{
			memset(m_ItemEffectGroup[wearPosNew].szEffectName, 0, sizeof(m_ItemEffectGroup[wearPosNew].szEffectName));
			
			if (itemPlusNewIn >= 0)// itemPlusNewIn < 0이면 장비를 벗을때로 판단
			{
				strcpy(m_ItemEffectGroup[wearPosNew].szEffectName, ItemData->GetArmorEffectName());
			}
		}else if (itemPlusNew < 0)
		{
			m_ItemEffectGroup[wearPosNew].szEffectName[0] = 0;
		}

		bRefresh = TRUE;
	}

	if(strType.str_String == NULL || pStrName == NULL)
	{
		if(bRefresh) Refresh(pTM, sbOption);
		return;
	}

	//plus수치에 따른 이펙트 단계
	char *szLevel = 0;
	//직업 종류
	char *szJobName = NULL;

	// 성주 무기인 경우에는 6단계 레벨의 이펙트를 무조건 적용한다.
	if((CItemData::IsLordItem( ItemData->GetItemIndex() )||CItemData::IsUniqueItem(ItemData->GetItemIndex())) && itemPlusNew != -1)
	{
		szLevel = "6";
	}
	else 
	{
		switch(itemPlusNew)
		{
		case -1: case 0: case 1: case 2: case 3://음수값일 경우 장착 해제한 것임.
			{
				pStrName[0] = 0;
				Refresh(pTM, sbOption);
			}
			return;
		case 4: case 5: szLevel = "1";	break;
		case 6: case 7: szLevel = "2";	break;
		case 8: case 9: szLevel = "3";	break;
		case 10:case 11:szLevel = "4";	break;
		case 12:case 13:case 14: szLevel = "5";	break;
		default:		szLevel = "6";	break;
		}
		//if(szLevel == NULL) return;//필요없음.
	}

	std::string strFormat;

	switch(job)
	{
	case TITAN:		szJobName = "a_t";	strFormat = "i%s_%s_%s"; break;
	case MAGE:		szJobName = "_ma";	strFormat = "i%s_%s_%s"; break;
	case HEALER:	szJobName = "_hw";	strFormat = "i%s_%s_%s"; break;
	case KNIGHT:	szJobName = "a_k";	strFormat = "i%s_%s_%s"; break;
	case ROGUE:		szJobName = "h_ro";	strFormat = "i%s_%s_%s"; break;
	case SORCERER:	szJobName = "so";	strFormat = "%s_%s_%s";  break;
	case NIGHTSHADOW: return;
#ifdef CHAR_EX_ROGUE
	case EX_ROGUE:		szJobName = "h_ro";	strFormat = "i%s_%s_%s"; break;	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	case EX_MAGE:		szJobName = "_ma";	strFormat = "i%s_%s_%s"; break;	//2013/01/08 jeil EX메이지 추가 
#endif
	default:		ASSERTALWAYS("Unknown Character Type"); return;
	}
	//if(szJobName == NULL) return;//필요없음.
	sprintf(pStrName, strFormat.c_str(), szJobName, strType, szLevel);
	Refresh(pTM, sbOption);
}


//------------------------------------------------------------------------------
// GetSubTypeWeapon
// Explain:  아이템의 서브 타입을 입력 받아 차수 를 리턴하는 함수
// Date : 2005-04-07,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
int CItemEffect::GetSubTypeWeapon ( int nSubType )
{
	// 2차 무기 판단
	if ( nSubType == CItemData::ITEM_WEAPON_CROSSBOW	// 석궁(로그)
	||	nSubType == CItemData::ITEM_WEAPON_AXE			// 도끼(타이탄)
	||	nSubType == CItemData::ITEM_WEAPON_WAND			// 완드(힐러)
	||	nSubType == CItemData::ITEM_WEAPON_TWOSWORD		// 이도류(나이트)
	||	nSubType == CItemData::ITEM_WEAPON_STAFF		// 스태프(메이지)
	||	nSubType == CItemData::ITEM_WEAPON_SCYTHE		// 사이더(소서러)
	) return 2;
	else return 1; // 현재 2차 무기 이외의 것들은 1차 무기로 판단
	
	return -1;
}

//------------------------------------------------------------------------------
// CItemEffect::DeleteNickEffect
// Explain : 호칭 이펙트 삭제
// Date : 2010-01-10
//------------------------------------------------------------------------------
void CItemEffect::DeleteNickEffect()
{
	memset(m_NickEffectGroup.szEffectName, 0, sizeof(m_NickEffectGroup.szEffectName));
	DestroyEffectGroupIfValid(m_NickEffectGroup.m_pEffectGroup);
}

//------------------------------------------------------------------------------
// CItemEffect::AddNickEffect
// Explain : 호칭 이펙트 추가
// Date : 2010-01-10
//------------------------------------------------------------------------------
BOOL CItemEffect::AddNickEffect( const char* strEffectName, CTagManager *pTM )
{
	if ( !strEffectName || !pTM ) return FALSE;

	// Refresh
	DeleteNickEffect(); // reset
	strcpy(m_NickEffectGroup.szEffectName, strEffectName);
	m_NickEffectGroup.m_pEffectGroup = StartEffectGroup(m_NickEffectGroup.szEffectName, pTM, _pTimer->GetLerpedCurrentTick()); // add effect

	return TRUE;
}

//------------------------------------------------------------------------------
// CItemEffect::DeletePetStashEffect
// Explain : 펫 창고 이펙트 삭제
// Date : 2013-09-11
//------------------------------------------------------------------------------
void CItemEffect::DeletePetStashEffect()
{
	memset(m_PetStashEffectGroup.szEffectName, 0, sizeof(m_PetStashEffectGroup.szEffectName));
	DestroyEffectGroupIfValid(m_PetStashEffectGroup.m_pEffectGroup);
}

//------------------------------------------------------------------------------
// CItemEffect::AddPetStashEffect
// Explain : 펫 창고 이펙트 추가
// Date : 2013-09-11
//------------------------------------------------------------------------------
BOOL CItemEffect::AddPetStashEffect( int nEffectType, CTagManager *pTM )
{
	if ( nEffectType >= PET_STASH_MAX || !pTM ) return FALSE;

	// Refresh
	DeletePetStashEffect(); // reset
	if (nEffectType < 0)
		return TRUE;

	strcpy(m_PetStashEffectGroup.szEffectName, g_strPetStashEffectName[nEffectType]);
	m_PetStashEffectGroup.m_pEffectGroup = StartEffectGroup(m_PetStashEffectGroup.szEffectName, pTM, _pTimer->GetLerpedCurrentTick()); // add effect

	return TRUE;
}

//로그인 이펙트 추가
void CItemEffect::AddLoginEffect( CTagManager *pTM )
{
	if ( !pTM ) 
		return;

	DeleteLoginJinEffect();

	strcpy(m_ItemEffectGroup[0].szEffectName, g_strLoginEffectName);
	m_ItemEffectGroup[0].m_pEffectGroup = StartEffectGroup(m_ItemEffectGroup[0].szEffectName, pTM, _pTimer->GetLerpedCurrentTick()); // add effect
}

void CItemEffect::DeleteLoginJinEffect()
{
	memset(m_ItemEffectGroup[0].szEffectName, 0, sizeof(m_ItemEffectGroup[0].szEffectName));
	DestroyEffectGroupIfValid(m_ItemEffectGroup[0].m_pEffectGroup);
}

void CItemEffect::AddHolyWaterEffect( int nEffecttype, CTagManager *pTM )
{
	m_nHolyWaterEffectType = nEffecttype;

	if (m_pHolyWaterEffectGroup[0] != NULL)
		StopEffectGroupIfValid(m_pHolyWaterEffectGroup[0], 0.1f);

	if (m_pHolyWaterEffectGroup[1] != NULL)
		StopEffectGroupIfValid(m_pHolyWaterEffectGroup[1], 0.1f);

	if ( nEffecttype <= 0 || nEffecttype >= 5)
	{		
		return;
	}

	if ( pTM == NULL )
	{
		return;
	}

	CTag		*pTag = NULL;
	CTString strEffect[5] = 
	{
		"empty",
		"WEAPON_P_HOLYWATER01",
		"WEAPON_M_HOLYWATER01",
		"WEAPON_R_HOLYWATER01",
		"WEAPON_MI_HOLYWATER01"
	};

	if (pTag = pTM->Find("LHAND", TRUE))
	{
		m_pHolyWaterEffectGroup[0] = StartEffectGroup(strEffect[nEffecttype], _pTimer->GetLerpedCurrentTick(), (ptr_tag)pTag);
	}

	if (pTag = pTM->Find("RHAND", TRUE))
	{
		m_pHolyWaterEffectGroup[1] = StartEffectGroup(strEffect[nEffecttype], _pTimer->GetLerpedCurrentTick(), (ptr_tag)pTag);
	}
}

BOOL CItemEffect::AddRelicEffect( int nEffectType, CTagManager *pTM )
{
	if ( nEffectType >= eRELIC_MAX || !pTM ) return FALSE;

	int nTemp = m_nCurRelicEffectType;
	 
	DeleteRelicEffect(); // reset

	if(nTemp < nEffectType)
		m_nCurRelicEffectType = nEffectType;
	else
		m_nCurRelicEffectType = nTemp;

	if (m_nCurRelicEffectType < 0)
		return FALSE;

	strcpy(m_RelicEffectGroup.szEffectName, g_strRelicEffectName[m_nCurRelicEffectType]);
	m_RelicEffectGroup.m_pEffectGroup = StartEffectGroup(m_RelicEffectGroup.szEffectName, pTM, _pTimer->GetLerpedCurrentTick()); // add effect

	return TRUE;
}

void CItemEffect::DeleteRelicEffect()
{
	memset(m_RelicEffectGroup.szEffectName, 0, sizeof(m_RelicEffectGroup.szEffectName));
	DestroyEffectGroupIfValid(m_RelicEffectGroup.m_pEffectGroup);
	m_nCurRelicEffectType = -1;
}

void CItemEffect::RefreshPetStashEffect( CTagManager *pTM )
{
	DestroyEffectGroupIfValid(m_PetStashEffectGroup.m_pEffectGroup);

	if (m_PetStashEffectGroup.szEffectName[0] != 0 && pTM != NULL)
	{
		m_PetStashEffectGroup.m_pEffectGroup = StartEffectGroup(m_PetStashEffectGroup.szEffectName, pTM, _pTimer->GetLerpedCurrentTick(), m_ItemERType, m_ItemERSubType);
	}
}

void CItemEffect::RefreshRelicEffect( CTagManager *pTM )
{
	DestroyEffectGroupIfValid(m_RelicEffectGroup.m_pEffectGroup);

	if (m_RelicEffectGroup.szEffectName[0] != 0 && pTM != NULL)
	{
		m_RelicEffectGroup.m_pEffectGroup = StartEffectGroup(m_RelicEffectGroup.szEffectName, pTM, _pTimer->GetLerpedCurrentTick(), m_ItemERType, m_ItemERSubType);
	}
}
