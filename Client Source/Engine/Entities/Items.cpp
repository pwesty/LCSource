
#include "stdh.h"

#include <Engine/Entities/Items.h>
#include <Engine/Network/CNetwork.h>

CItems::CItems(void)
{	
	//Init(); 
	this->CItems::CItems(0);
}

CItems::CItems(int itemindex) 
{
	//ItemData = NULL;
	
	Init(); 
	Item_Index = itemindex;
	if(_pNetwork != NULL)
		ItemData = CItemData::getData(itemindex);
}

CItems::~CItems(void) 
{
	int		i;

	Item_Tab = -1;
	InvenIndex = -1;

	Item_Index = -1;
	Item_UniIndex = -1;

	Item_Plus = 0;
	Item_Flag = 0;
	Item_Used = -1;

	Item_Wearing = -1;

	Item_Sum = 0;
	Item_Price = 0;

	for( i = 0; i < MAX_OPTION_INC_ORIGIN; i++ )
	{
		Item_OptionType[i] = -1;
		Item_OptionLevel[i] = 0;
		Item_OriginOptionVar[i] = ORIGIN_VAR_DEFAULT;
	}
	
	ComItem_index = -1;

	Item_RareIndex =0;
	
	for( i = 0; i < MAX_ITEM_SKILL; i++ )
	{
		Item_SkillIndex[i] = -1;
		Item_SkillLevel[i] = -1;
	}

	Item_SocketCount = 0;
	Item_Toggle = false;
}

void CItems::Init(void)
{
	int		i;
	ItemData = NULL;

	Item_Tab = -1;
	InvenIndex = -1; 

	Item_Index = -1;
	Item_UniIndex = -1;
	Item_CashIndex = -1;

	Item_Plus = 0;
	Item_Flag = 0;
	Item_Used = -1;

	Item_Wearing = -1;

	Item_Sum = 0;
	Item_Price = 0;

	for( i = 0; i < MAX_OPTION_INC_ORIGIN; i++ )
	{
		Item_OptionType[i] = -1;
		Item_OptionLevel[i] = 0;
		Item_OriginOptionVar[i] = ORIGIN_VAR_DEFAULT;
	}

	Item_RareIndex =0;

	for ( i = 0; i < MAX_ITEM_SKILL; i++)
	{
		Item_SkillIndex[i] = -1;
		Item_SkillLevel[i] = -1;
	}

	Item_State_Plus = 0;
	InitSocketInfo();

#ifdef DURABILITY
	Item_durability_now = 0;
	Item_durability_max = 0;
#endif	//	DURABILITY

	m_bSkillDelay = FALSE;
	m_llTime = 0;
	Item_Toggle = false;
}

void CItems::SetPrice(SQUAD nPrice)
{
	Item_Price = nPrice;
}

void CItems::SetData( LONG lIndex, LONG lUniIndex, SBYTE sbTab, int inven_idx,
						ULONG ulPlus, ULONG ulFlag, LONG lComUniIndex, LONG lUsed, LONG lUsed2, SBYTE sbWearing, SQUAD llSum)
{
	Item_Index = lIndex;
	Item_UniIndex = lUniIndex;
	Item_Tab = sbTab;
	InvenIndex = inven_idx;
	Item_Plus = ulPlus;
	Item_Flag = ulFlag;
	ComItem_index = lComUniIndex;
	Item_Used = lUsed;
	Item_Used2 = lUsed2;
	Item_Wearing = sbWearing;
	Item_Sum	= llSum;
}

void CItems::InitOptionData(void) 
{
	for( int i = 0; i < MAX_OPTION_INC_ORIGIN; i++ )
	{
		Item_OptionType[i] = -1;
		Item_OptionLevel[i] = 0;
		Item_OriginOptionVar[i] = ORIGIN_VAR_DEFAULT;
	}
}

void CItems::SetOptionData(SBYTE sbIndex, SBYTE sbOptionType, LONG lOptionLevel, LONG lOriginOptionVar )
{
	Item_OptionType[sbIndex] = sbOptionType;
	Item_OptionLevel[sbIndex] = lOptionLevel;
	Item_OriginOptionVar[sbIndex] = lOriginOptionVar;
}

// 아이템 플러스 수치 변경.
#define NEW_UPGRADE_ITEM
int CItems::CalculatePlusDamage( int iAttack, ULONG ulItemPlus, BOOL Rune )
{
	int nPlusValue = 0;

	if (Rune)
	{
		if (ulItemPlus != 0)
		{
			// [100112 sora] 업그레이드 공식 수정
			nPlusValue = (int)(iAttack*(pow((1.188f), (float)ulItemPlus) / 10));
		}else
		{
			nPlusValue = 0;
		}

		return nPlusValue;
	}
#ifdef NEW_UPGRADE_ITEM
	if( ulItemPlus >= 15 )
		nPlusValue = (int)( iAttack * pow( (1.07f), (float)ulItemPlus ) ) - iAttack;
	else if( ulItemPlus >= 11 )
	{
		nPlusValue = (int)( iAttack * pow( (1.07f), (float)ulItemPlus ) ) - iAttack;
	}
	else if( ulItemPlus >= 1 )
	{
		nPlusValue = (int)( iAttack * pow( (1.06f), (float)ulItemPlus ) ) - iAttack;
	}	
#else
	nPlusValue = (int)( iAttack * pow( ITEM_PLUS_COFACTOR, ulItemPlus ) ) - iAttack;
#endif
	return nPlusValue;
}
// [ldy1978220 2011/9/15] 1106 Update 인첸트 공식 변경 
int CItems::ItemUpgradeFuckingFunction(int iAttack, ULONG ulItemPlus, BOOL bRune /* = FALSE  */)
{
	// 공통 상수
	double plusFactor = 1.06 ;

	if( !bRune )
	{
		if (ulItemPlus > 10)
			plusFactor = 1.07;
	}
	else 
	{
		plusFactor = 1.188;
	}
	
	// 무기 / 방어구 공식이 같음
	const int itemUpgradeLimit = 17;

	// USA : plusFactor^27 , plusFactor^26 ETC: plusFactor^17 , plusFactor^16
	double func_value_1 = pow(plusFactor,itemUpgradeLimit);
	double func_value_2 = pow(plusFactor,itemUpgradeLimit-1);

	int nRetAttack = 0;

	if( bRune )
	{
		if ( ulItemPlus > itemUpgradeLimit )	// 오바되는 인첸트 공식
		{
			nRetAttack = (int)( iAttack + iAttack * (func_value_1/10) + (ulItemPlus-itemUpgradeLimit) * iAttack * ((func_value_1-func_value_2)/10) );
		}
		else	// 기본 공식
		{
			nRetAttack = (int)(iAttack + iAttack * pow(plusFactor, (double)ulItemPlus) / 10 );
		}
	}
	else
	{
		if ( ulItemPlus > itemUpgradeLimit )	// 오바되는 인첸트 공식
		{
			nRetAttack = (int)( iAttack * func_value_1 + (ulItemPlus-itemUpgradeLimit) * iAttack * (func_value_1-func_value_2) ) ;
		}
		else	// 기본 공식
		{
			nRetAttack = (int)(iAttack * pow(plusFactor, (double)ulItemPlus));
		}
							
	}
	return nRetAttack - iAttack;
}

int CItems::CalculateRuneItemBonus( int itemtype, ULONG ulItemPlus )
{
	if(ulItemPlus < 6) // +5까지는 보너스 없음
	{
		return 0;
	}

	int nBonusBase = 0;

	if(itemtype == CItemData::ITEM_WEAPON)
	{
		if (ulItemPlus >= 25)
			return 700;
		else if (ulItemPlus >= 20)
			return 550;

		nBonusBase = 45;
	}
	else if(itemtype == CItemData::ITEM_SHIELD)
	{
		if (ulItemPlus >= 25)
			return 400;
		else if (ulItemPlus >= 20)
			return 300;

		nBonusBase = 25;
	}

	// +6~+15까지만 보너스 적용 +15이상은 모두 10단위
	return (ulItemPlus >= 15 ? 10 : ulItemPlus - 5) * nBonusBase;
}

//착용레벨 감소 옵션(Index: 49)이 붙은 장비를 입고 있을 경우 감소되는 레벨량을 구함. 
int CItems::GetJewelLevelReduction()
{
	SBYTE		sbOptionType;
	LONG		lOptionLevel;
	if (Item_SocketJewelIndex[JEWEL_POS_CHAOS_SOCKET] > 0)
	{
		CItemData* pLevelItemdata = _pNetwork->GetItemData( Item_SocketJewelIndex[JEWEL_POS_CHAOS_SOCKET] );
		sbOptionType = pLevelItemdata->GetSocketOptionIndex();
		lOptionLevel = pLevelItemdata->GetSocketOptionLevel();
		
		if ( sbOptionType == OPTION_DOWN_LIMITLEVEL )
		{
			COptionData* podItem = COptionData::getData(sbOptionType );
			if (podItem == NULL)
				return 0;

			return podItem->GetValue( lOptionLevel - 1 );
		}
	}	
	return 0;
}

//해당 옵션이 없으면 0 리턴!
int CItems::GetWearLevelReduction()
{
	for( SBYTE sbOption = 0; sbOption < MAX_OPTION_INC_ORIGIN; ++sbOption )
	{
		SBYTE sbOptionType = GetOptionType( sbOption );
		LONG lOptionLevel = GetOptionLevel( sbOption );

		if( sbOptionType == -1 || lOptionLevel == 0 )
			break;

		//착용제한레벨 다운 옵션
		if( sbOptionType == OPTION_DOWN_LIMITLEVEL)
		{
			COptionData* podItem = COptionData::getData( sbOptionType );
			if (podItem == NULL)
				return 0;

			return podItem->GetValue( lOptionLevel - 1 );
		}
	}
	
	return 0;
}

void CItems::SetPetInfo(CTString petName, int petLeve, int petStr, int petCon, int petDex, int petInt)
{
	Item_PetInfo.pet_name = petName;
	Item_PetInfo.pet_level = petLeve;
	Item_PetInfo.pet_str = petStr;
	Item_PetInfo.pet_con = petCon;
	Item_PetInfo.pet_dex = petDex;
	Item_PetInfo.pet_int = petInt;
}

int CItems::GetAmountOfJewelryInserted()
{
	int i, count = 0;

	for( i = 0; i < GetSocketCount(); i++ )
	{
		if( GetSocketJewelIndex(i) > 0 )
			count++;
	}
	return count;
}

void CItems::SetDurability( int Now_value, int Max_value )
{
#ifdef DURABILITY
	Item_durability_now = Now_value;
	Item_durability_max = Max_value;
#endif	//	DURABILITY
}

void CItems::GetDurability( int &Now_value, int &Max_value )
{
#ifdef DURABILITY
	Now_value = Item_durability_now;
	Max_value = Item_durability_max;
#endif	//	DURABILITY
}

bool CItems::IsZeroDurability()
{
#ifdef DURABILITY
	if (Item_durability_now / 10 <= 0)
		return true;
#endif	//	DURABILITY
	return false;
}
