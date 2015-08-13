#ifndef INCL_ITEMS_H
#define INCL_ITEMS_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <Engine/Entities/ItemData.h>
#include <Engine/Entities/OptionData.h>


#define	ITEM_PLUS_COFACTOR		(1.06f)			// 업그레이드 수치 공식에 필요한 인자

class  CItems
{
public:

	CItemData*	ItemData;

	SWORD	Item_Tab;
	SWORD	InvenIndex;
	SWORD	ServerIndex;

	LONG	Item_Index;
	LONG	Item_UniIndex;
	LONG	Item_CashIndex;
	
	ULONG	Item_Plus;
	ULONG	Item_Flag;
	LONG	Item_Used;
	LONG	Item_Used2;	

	SBYTE	Item_Wearing;

	SQUAD	Item_Sum;
	SQUAD	Item_Price;

	SBYTE	Item_OptionType[MAX_OPTION_INC_ORIGIN];
	LONG	Item_OptionLevel[MAX_OPTION_INC_ORIGIN];
	LONG	Item_OriginOptionVar[MAX_OPTION_INC_ORIGIN];
	LONG	ComItem_index;

	LONG	Item_RareIndex;
	sPetItem_Info	Item_PetInfo;
	
	SBYTE	Item_Belong;
	LONG	Item_SkillIndex[MAX_ITEM_SKILL];
	SBYTE	Item_SkillLevel[MAX_ITEM_SKILL];
	
	LONG	Item_State_Plus;

// socket system [5/10/2010 rumist]
	SBYTE	Item_SocketCount;
	LONG	Item_SocketJewelIndex[MAX_SOCKET_OPTION];

	bool	Item_Toggle;

#ifdef DURABILITY
	int	Item_durability_now;
	int	Item_durability_max;
#endif	//	DURABILITY
	
	BOOL	m_bSkillDelay;
	SQUAD	m_llTime;

public:
	/* Default constructor. */
	CItems(void);
	CItems(int itemindex); 
	/* Destructor. */
	~CItems(void);

	void	Init(void);
	void	SetData( LONG lIndex, LONG lUniIndex, SBYTE sbTab, int inven_idx, ULONG ulPlus,
						ULONG ulFlag, LONG lComUniIndex, LONG lUsed, LONG lUsed2, SBYTE sbWearing, SQUAD llSum);
	void	SetPrice(SQUAD nPrice);
	void	InitOptionData(void);
	void	SetOptionData( SBYTE sbIndex, SBYTE sbOptionType, LONG lOptionLevel, LONG lOriginOptionVar );
	void	SetRareIndex( LONG iRareIndex ) { Item_RareIndex =iRareIndex; }
	void	SetPetInfo(CTString petName, int petLeve, int petStr, int petCon, int petDex, int petInt); 

	SBYTE	GetOptionType( SBYTE sbOption ) const { return Item_OptionType[sbOption]; }
	LONG	GetOptionLevel( SBYTE sbOption ) const { return Item_OptionLevel[sbOption]; }
	LONG	GetOriginOptionVar( SBYTE sbOption ) const { return Item_OriginOptionVar[sbOption]; }

	void	SetItemBelong(SBYTE sbBelong)	{ Item_Belong = sbBelong; }
	SBYTE	GetItemBelong()					{ return Item_Belong; }
	inline void	SetItemSkill(SBYTE sbIndex, LONG lSkillIndex, SBYTE sbSkillLevel)
	{
		Item_SkillIndex[sbIndex] = lSkillIndex;
		Item_SkillLevel[sbIndex] = sbSkillLevel;
	}
	// socket system [5/10/2010 rumist]
	inline void InitSocketInfo()
	{
		Item_SocketCount = 0;
		memset( &Item_SocketJewelIndex[0], -1, sizeof(LONG) * MAX_SOCKET_OPTION );
	}

	inline void SetSocketCount( SBYTE sbCount )							{ Item_SocketCount = sbCount;		}
	inline void	SetSocketOption( SBYTE sbIndex, LONG lJewelIndex )
	{	Item_SocketJewelIndex[sbIndex] = lJewelIndex;	}
	const SBYTE GetSocketCount()	const								{ return Item_SocketCount;			}
	const LONG	GetSocketJewelIndex( SBYTE sbIndex )					{ return Item_SocketJewelIndex[sbIndex];	}
	int			GetAmountOfJewelryInserted();	// 소켓에 박혀 있는 보석의 Count
	const BOOL	IsSocketExist()	const									{ return (Item_SocketCount > 0);	}

	LONG	GetSetItemSkillIndex(SBYTE sbSkillpos) const { return Item_SkillIndex[sbSkillpos]; }
	SBYTE	GetSetItemSkillLevel(SBYTE sbSkillpos) const { return Item_SkillLevel[sbSkillpos]; }

	void SetItemPlus2(LONG lplus)	{ Item_State_Plus = lplus; }
	LONG GetItemPlus2()				{ return Item_State_Plus; }

	static	int CalculatePlusDamage( int iAttack, ULONG ulItemPlus, BOOL bRune = FALSE );
	static	int	CalculateRuneItemBonus( int itemtype,  ULONG ulItemPlus );
	// [ldy1978220 2011/9/15] 1106 Update 인첸트 공식 변경 
	static  int ItemUpgradeFuckingFunction( int iAttack, ULONG ulItemPlus, BOOL bRune = FALSE );

	// 2월 가미고 이슈 결합 [2/27/2013 Ranma]
	LONG GetComItem_index() { return ComItem_index; }
	
	inline BOOL IsFlag( int nFlag ) const 
	{
		if( Item_Flag & nFlag ) return TRUE;
		return FALSE;
	}
	
	int GetWearLevelReduction();			//착용레벨감소옵션(Index:49)이 붙은 장비를 입고 있을 경우 감소되는 레벨량 구함.
	int GetJewelLevelReduction();			//보석 착용레벨감소옵션(Index:49)이 붙은 장비를 입고 있을 경우 감소되는 레벨량 구함.
	BOOL	IsEmptyItem(void)
	{
		return (Item_UniIndex<0);
	}

	void SetServerIndex(SWORD index) { ServerIndex = index; }
	SWORD GetServerIndex() const { return ServerIndex; }

	void SetDurability(int Now_value, int Max_value);
	void GetDurability(int &Now_value, int &Max_value);
	bool IsZeroDurability();

	void SetSkillDelay( BOOL bSkillDelay ) { m_bSkillDelay = bSkillDelay; }
	BOOL	GetSkillDelay() const { return m_bSkillDelay; }

	void	SetUseItemStartTime( SQUAD llTime ) { m_llTime = llTime; }
	SQUAD	GetUseItemStartTime() const { return m_llTime; }

	bool	GetToggle()	{return Item_Toggle;	}
	void	SetToggle(bool bToggle)	{ Item_Toggle = bToggle;	}
	ULONG GetItemPlus() { return Item_Plus; }
};

// Function Object
struct FindItem
{
	LONG iNum;
	FindItem( CItems& item ) : iNum( item.Item_UniIndex ) {};
	bool operator() (const CItems& item)
	{
		if(item.Item_UniIndex == iNum)
		{
			return true;
		}
		else
			return false;
	}
};

#endif